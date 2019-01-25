#include "grid.hpp"


namespace
{
  void check(std::ofstream& ofs)
  {
    if (!ofs.is_open())
    {
      std::cout << "Failed to open file" << std::endl;
      throw 1;
    }
  }
}

void grid::writeout_domain(const std::string& out_dir, bool save_interp_matrices)
{
  // Salvataggio delle caratteristiche del dominio

  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  std::ofstream out_file;

  //
  std::cout<<"Salvo vettore x\n";
  {
    std::ofstream out_file(out_dir + "/data/domain/x", ios::out);
    check(out_file);
    for (unsigned int i=0; i<Nx+3; i++)
    {
      out_file << x.at(i) << std::endl;
    }
  }

  //
  std::cout<<"Salvo vettore y\n";
  {
    std::ofstream out_file(out_dir + "/data/domain/y", ios::out);
    check(out_file);
    for (unsigned int i=0; i<Ny+3; i++)
    {
      out_file << y.at(i) << std::endl;
    }
  }

  //
  std::cout<<"Salvo centri cella\n";
  {
    std::ofstream out_file(out_dir + "/data/domain/cell_centers", ios::out);
    check(out_file);
    for (map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
    {
      out_file<< ((it->second).get_center())(0) <<' '<<((it->second).get_center())(1)<<std::endl;
    }
  }

  //
  std::cout<<"Salvo lati\n";
  {
    std::ofstream out_file(out_dir + "/data/domain/edges", ios::out);
    check(out_file);
    for (unsigned int i=1; i<=ccb.get_N_edges(); i++)
    {
      out_file
        << ccb.get_edge_vertex(BOUNDARY::left,i)(0)
        <<' '
        << ccb.get_edge_vertex(BOUNDARY::left,i)(1)
        <<' '
        << ccb.get_edge_vertex(BOUNDARY::right,i)(0)
        <<' '
        << ccb.get_edge_vertex(BOUNDARY::right,i)(1)
        <<std::endl;
    }
  }

  //
  std::cout<<"Salvo celle tagliate\n";
  {
    std::ofstream out_file(out_dir + "/data/domain/cutted_cells", ios::out);
    check(out_file);
    for (unsigned int i=1; i<=ccb.get_N_edges(); i++)
    {
      out_file<< ccb.get_label(i)(0) <<' ' <<ccb.get_label(i)(1) <<' ' <<i <<std::endl;
    }
  }

  std::cout<<"Salvo celle bagnate\n";
  {
    std::ofstream out_file(out_dir + "/data/domain/wet_cells", ios::out);
    check(out_file);
    for (std::vector<label>::iterator it = wet_cells.begin(); it != wet_cells.end(); it++)
    {
      out_file<< (*it)(0) <<' ' <<(*it)(1) <<std::endl;
    }
  }

  //
  std::cout<<"Salvo ghost cells\n";
  {
    std::ofstream out_file(out_dir + "/data/domain/ghost_cells", ios::out);
    check(out_file);
    for (map<label,ghost_cell>::iterator it = ghost_cells.begin(); it != ghost_cells.end(); it++)
    {
      out_file
        << (it->first)(0)
        << ' '
        << (it->first)(1)
        << ' '
        << (it->second).get_number_of_adj_wet_cells()
        << ' '
        << (it->second).get_number_of_adj_wet_cells()
        <<' ';

      std::vector<ghost_quantities> gqs = (it->second).get_GQs();
      for (std::vector<ghost_quantities>::iterator itgq = gqs.begin(); itgq!= gqs.end(); itgq++)
      {
        out_file
          << itgq->BP(0)
          << ' '
          << itgq->BP(1)
          << ' '
          << itgq->RP(0)
          << ' '
          << itgq->RP(1)
          << ' '
          << itgq->normal(0)
          << ' '
          << itgq->normal(1)
          << ' '
          << itgq->SW_corner(0)
          << ' '
          << itgq->SW_corner(1)
          << ' '
          << itgq->associated_WC(0)
          << ' '
          << itgq->associated_WC(1)
          << ' '
          << itgq->number_ghost_corners
          << ' '
          << itgq->edge_number
          << ' ';
      }

      // riempie di -1 laddove ci siano meno di 2 BP (diversi o coincidenti)

      if ((it->second).get_number_of_adj_wet_cells() < 3)
      {
        for (unsigned int i=0; i<12*(3-(it->second).get_number_of_adj_wet_cells()); i++)
        {
          out_file << -1 << ' ';
        }
      }
    }
  }

  // Salvataggio delle matrici. Utile solo per il debug con matlab

  if (save_interp_matrices == true)
  {
  std::cout<<"Salvo le matrici di interpolazione\n";

  int n=0;
  label SW_label;
  IMMERSED_BOUNDARY::M4 M4x4;
  IMMERSED_BOUNDARY::M8 M8x8;
  IMMERSED_BOUNDARY::M6 M6x6;
  IMMERSED_BOUNDARY::M3 M3x3;

  // salvataggio all_wet

  string file_dir(out_dir + "/data/domain/interp_matrices/all_wet/");
  string file_name;

  using It = map<label,IMMERSED_BOUNDARY::M4_LU>::iterator;
  for (It it = all_wet_matrices.begin(); it!=all_wet_matrices.end(); it++)
  {
    SW_label = it->first;
    M4x4 = it->second.reconstructedMatrix();
    file_name = IMMERSED_BOUNDARY::int2string(n);

    {
      std::ofstream out_file((file_dir + "SW_labels/" + file_name).c_str());
      check(out_file);
      out_file << SW_label(0) << ' ' << SW_label(1) << std::endl;
    }

    {
      std::ofstream out_file((file_dir + "M4x4/" + file_name).c_str());
      check(out_file);
      for (int i=0; i<4; i++)
      {
        for (int j=0; j<4; j++)
        {
          out_file<< M4x4(i,j) <<' ';
        }
        out_file<< std::endl;
      }
    }
    n++;
  }

  // salvataggio GPs

  n=0;

  file_dir = out_dir + "/data/domain/interp_matrices/GPs/";

  using It84 = map<label,IMMERSED_BOUNDARY::M8M4_LU>::iterator;
  for (It84 it = GPs_Matrices.begin(); it!=GPs_Matrices.end(); it++)
  {
    SW_label = it->first;
    M8x8 = it->second.first.reconstructedMatrix();
    M4x4 = it->second.second.reconstructedMatrix();
    file_name = IMMERSED_BOUNDARY::int2string(10000+n);

    {
      std::ofstream out_file((file_dir + "SW_labels/" + file_name).c_str());
      check(out_file);
      out_file << SW_label(0) <<' ' <<SW_label(1) <<std::endl;
    }

    {
      std::ofstream out_file((file_dir + "M8x8/" + file_name).c_str());
      check(out_file);
      for (int i=0; i<8; i++)
      {
        for (int j=0; j<8; j++)
        {
          out_file << M8x8(i,j) << ' ';
        }
        out_file << std::endl;
      }
    }

    {
      std::ofstream out_file((file_dir + "M4x4/" + file_name).c_str());

      for (int i=0; i<4; i++)
      {
        for (int j=0; j<4; j++)
        {
          out_file<< M4x4(i,j) <<' ';
        }

        out_file<< std::endl;
      }
    }
    n++;
  }

  // salvataggio dry_no_GPs

  n=0;

  file_dir = out_dir + "./data/domain/interp_matrices/dry_no_GP/";

  using It63 = map<label,IMMERSED_BOUNDARY::M6M3_LU_non_cc>::iterator;
  for (It63 it = noGPs_Matrices.begin(); it!=noGPs_Matrices.end(); it++)
  {
    SW_label = it->first;
    M6x6 = it->second.matrices.first.reconstructedMatrix();
    M3x3 = it->second.matrices.second.reconstructedMatrix();
    file_name = IMMERSED_BOUNDARY::int2string(10000+n);

    {
      std::ofstream out_file((file_dir + "SW_labels/" + file_name).c_str());
      out_file << SW_label(0) <<' ' <<SW_label(1) <<std::endl;
    }

    {
      std::ofstream out_file((file_dir + "non_considered_corner/" + file_name).c_str());
      out_file << it->second.non_cc <<std::endl;
    }

    {
      std::ofstream out_file((file_dir + "M6x6/" + file_name).c_str());
      check(out_file);

      for (int i=0; i<6; i++)
      {
        for (int j=0; j<6; j++)
        {
          out_file<< M6x6(i,j) <<' ';
        }

        out_file<< std::endl;
      }
    }

    {
      std::ofstream out_file((file_dir + "M3x3/" + file_name).c_str());

      for (int i=0; i<3; i++)
      {
        for (int j=0; j<3; j++)
        {
          out_file<< M3x3(i,j) <<' ';
        }

        out_file<< std::endl;
      }
    }
    n++;
  }
  } // end if (save_interp_matrices)
} // end method


void grid::writeout_BC_type()
{
  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  ofstream out_file;

  out_file.open("./data/domain/BC_type", ios::out);
  for (vector<unsigned int>::iterator it=type_of_boundary_conditions.begin(); it!=type_of_boundary_conditions.end(); it++)
  {
    out_file<<*it<<std::endl;
  }
  out_file.close();
}


void grid::writeout_unknowns(const std::string& file_name)
{
  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  ofstream out_file;

  out_file.open(file_name.c_str(), ios::out);

  for (map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
  {
    if (it->second.get_type() == CELL::wet)
    {

      //out_file<<it->first(0) <<' ' <<it->first(1) <<' ';
      // Le celle bagnate sono salvate nel file wet_cells
      for (unsigned int n=1; n<=N_equations; n++)
      {
        out_file<<it->second.get_unknown(n) <<' ';
      }
      out_file <<endl;
    }
  }
  out_file.close();
}

void grid::writeout_real_domain(
  const std::vector<std::pair<label, Point2d<double> > >& real_vertices)
{
  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  ofstream out_file;

  out_file.open("./data/domain/real_vertices", ios::out);

  using It = std::vector<std::pair<label, Point2d<double>>>::const_iterator;
  for (It it = real_vertices.begin(); it != real_vertices.end(); it++)
  {
    out_file << it->second(0) <<' ' <<it->second(1) <<endl;
  }

  out_file.close();
}
