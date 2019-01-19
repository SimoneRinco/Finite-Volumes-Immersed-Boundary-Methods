#include "grid.hpp"


void grid::writeout_domain(const bool& save_interp_matrices)

{
  // Salvataggio delle caratteristiche del dominio

  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  ofstream OutFile;

  //
  std::cout<<"Salvo vettore x\n";

  OutFile.open("./data/domain/x", ios::out);
  for (unsigned int i=0; i<Nx+3; i++)
  {
    OutFile<<x.at(i)<<std::endl;
  }
  OutFile.close();

  //
  std::cout<<"Salvo vettore y\n";

  OutFile.open("./data/domain/y", ios::out);
  for (unsigned int i=0; i<Ny+3; i++)
  {
    OutFile<<y.at(i)<<std::endl;
  }
  OutFile.close();

  //
  std::cout<<"Salvo centri cella\n";

  OutFile.open("./data/domain/cell_centers", ios::out);
  for (map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
  {
    OutFile<< ((it->second).get_center())(0) <<' '<<((it->second).get_center())(1)<<std::endl;
  }
  OutFile.close();



  //
  std::cout<<"Salvo lati\n";

  OutFile.open("./data/domain/edges", ios::out);
  for (unsigned int i=1; i<=ccb.get_N_edges(); i++)
  {
    OutFile<< ccb.get_edge_vertex(BOUNDARY::left,i)(0) <<' ' <<ccb.get_edge_vertex(BOUNDARY::left,i)(1) <<' ' <<ccb.get_edge_vertex(BOUNDARY::right,i)(0) <<' ' <<ccb.get_edge_vertex(BOUNDARY::right,i)(1) <<std::endl;
  }
  OutFile.close();

  //
  std::cout<<"Salvo celle tagliate\n";

  OutFile.open("./data/domain/cutted_cells", ios::out);
  for (unsigned int i=1; i<=ccb.get_N_edges(); i++)
  {
    OutFile<< ccb.get_label(i)(0) <<' ' <<ccb.get_label(i)(1) <<' ' <<i <<std::endl;
  }
  OutFile.close();

  /*
  // Stesso output del precedente: usato solo in fase di debug
  std::cout<<"Salvo celle tagliate (2)\n";

  OutFile.open("./data/domain/cutted_cells_2", ios::out);
    for (map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
    {
      if (it->second.get_boundary() == CELL::cut)
      {
      OutFile<< it->first(0) <<' ' <<it->first(1) <<std::endl;
      }
    }
    OutFile.close();
  */

  std::cout<<"Salvo celle bagnate\n";

  OutFile.open("./data/domain/wet_cells", ios::out);
  for (std::vector<label>::iterator it = wet_cells.begin(); it != wet_cells.end(); it++)
  {
    OutFile<< (*it)(0) <<' ' <<(*it)(1) <<std::endl;
  }
  OutFile.close();

  //
  std::cout<<"Salvo ghost cells\n";

  OutFile.open("./data/domain/ghost_cells", ios::out);

  for (map<label,ghost_cell>::iterator it=ghost_cells.begin(); it!=ghost_cells.end(); it++)
  {
    OutFile<< (it->first)(0) <<' '<<(it->first)(1)<<' ' <<(it->second).get_number_of_adj_wet_cells() <<' ' <<(it->second).get_number_of_adj_wet_cells() <<' ';

    std::vector<ghost_quantities> gqs = (it->second).get_GQs();
    for (std::vector<ghost_quantities>::iterator itgq = gqs.begin(); itgq!= gqs.end(); itgq++)
    {
      OutFile<< itgq->BP(0) <<' ';
      OutFile<< itgq->BP(1) <<' ';
      OutFile<< itgq->RP(0) <<' ';
      OutFile<< itgq->RP(1) <<' ';
      OutFile<< itgq->normal(0) <<' ';
      OutFile<< itgq->normal(1) <<' ';
      OutFile<< itgq->SW_corner(0) <<' ';
      OutFile<< itgq->SW_corner(1) <<' ';
      OutFile<< itgq->associated_WC(0) <<' ';
      OutFile<< itgq->associated_WC(1) <<' ';
      OutFile<< itgq->number_ghost_corners <<' ';
      OutFile<< itgq->edge_number <<' ';
    }

      // riempie di -1 laddove ci siano meno di 2 BP (diversi o coincidenti)

      if ((it->second).get_number_of_adj_wet_cells() < 3)
      {
        for (unsigned int i=0; i<12*(3-(it->second).get_number_of_adj_wet_cells()); i++)
        {
          OutFile<< -1 <<' ';
        }
      }


    OutFile<<std::endl;
  }
  OutFile.close();

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

  string file_dir("./data/domain/interp_matrices/all_wet/");
  string file_name;

  for (map<label,IMMERSED_BOUNDARY::M4_LU>::iterator it = all_wet_matrices.begin(); it!=all_wet_matrices.end(); it++)
  {
    SW_label = it->first;
    M4x4 = it->second.reconstructedMatrix();
    file_name = IMMERSED_BOUNDARY::int2string(n);

    OutFile.open((file_dir + "SW_labels/" + file_name).c_str());

    OutFile << SW_label(0) <<' ' <<SW_label(1) <<std::endl;

    OutFile.close();

    OutFile.open((file_dir + "M4x4/" + file_name).c_str());

    for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
      {
        OutFile<< M4x4(i,j) <<' ';
      }

      OutFile<< std::endl;
    }

    OutFile.close();
    n++;
  }

  // salvataggio GPs

  n=0;

  file_dir = "./data/domain/interp_matrices/GPs/";

  for (map<label,IMMERSED_BOUNDARY::M8M4_LU>::iterator it = GPs_Matrices.begin(); it!=GPs_Matrices.end(); it++)
  {
    SW_label = it->first;
    M8x8 = it->second.first.reconstructedMatrix();
    M4x4 = it->second.second.reconstructedMatrix();
    file_name = IMMERSED_BOUNDARY::int2string(10000+n);

    OutFile.open((file_dir + "SW_labels/" + file_name).c_str());

    OutFile << SW_label(0) <<' ' <<SW_label(1) <<std::endl;

    OutFile.close();

    OutFile.open((file_dir + "M8x8/" + file_name).c_str());

    for (int i=0; i<8; i++)
    {
      for (int j=0; j<8; j++)
      {
        OutFile<< M8x8(i,j) <<' ';
      }

      OutFile<< std::endl;
    }

    OutFile.close();

    OutFile.open((file_dir + "M4x4/" + file_name).c_str());

    for (int i=0; i<4; i++)
    {
      for (int j=0; j<4; j++)
      {
        OutFile<< M4x4(i,j) <<' ';
      }

      OutFile<< std::endl;
    }

    OutFile.close();
    n++;
  }

  // salvataggio dry_no_GPs

  n=0;

  file_dir = "./data/domain/interp_matrices/dry_no_GP/";

  for (map<label,IMMERSED_BOUNDARY::M6M3_LU_non_cc>::iterator it = noGPs_Matrices.begin(); it!=noGPs_Matrices.end(); it++)
  {
    SW_label = it->first;
    M6x6 = it->second.matrices.first.reconstructedMatrix();
    M3x3 = it->second.matrices.second.reconstructedMatrix();
    file_name = IMMERSED_BOUNDARY::int2string(10000+n);

    OutFile.open((file_dir + "SW_labels/" + file_name).c_str());
    OutFile << SW_label(0) <<' ' <<SW_label(1) <<std::endl;
    OutFile.close();

    OutFile.open((file_dir + "non_considered_corner/" + file_name).c_str());
    OutFile << it->second.non_cc <<std::endl;
    OutFile.close();

    OutFile.open((file_dir + "M6x6/" + file_name).c_str());

    for (int i=0; i<6; i++)
    {
      for (int j=0; j<6; j++)
      {
        OutFile<< M6x6(i,j) <<' ';
      }

      OutFile<< std::endl;
    }

    OutFile.close();

    OutFile.open((file_dir + "M3x3/" + file_name).c_str());

    for (int i=0; i<3; i++)
    {
      for (int j=0; j<3; j++)
      {
        OutFile<< M3x3(i,j) <<' ';
      }

      OutFile<< std::endl;
    }

    OutFile.close();
    n++;
  }
  } // end if (save_interp_matrices)
} // end method


void grid::writeout_BC_type()
{
  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  ofstream OutFile;

  OutFile.open("./data/domain/BC_type", ios::out);
  for (vector<unsigned int>::iterator it=type_of_boundary_conditions.begin(); it!=type_of_boundary_conditions.end(); it++)
  {
    OutFile<<*it<<std::endl;
  }
  OutFile.close();
}


void grid::writeout_unknowns(const std::string& file_name)
{
  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  ofstream OutFile;

  OutFile.open(file_name.c_str(), ios::out);

  for (map<label,cell>::iterator it=cells.begin(); it!=cells.end(); it++)
  {
    if (it->second.get_type() == CELL::wet)
    {

      //OutFile<<it->first(0) <<' ' <<it->first(1) <<' ';
      // Le celle bagnate sono salvate nel file wet_cells
      for (unsigned int n=1; n<=N_equations; n++)
      {
        OutFile<<it->second.get_unknown(n) <<' ';
      }
      OutFile <<endl;
    }
  }
  OutFile.close();
}

void grid::writeout_real_domain(
  const std::vector<std::pair<label, Point2d<double> > >& real_vertices)
{
  using namespace std;
  cout.setf(ios::scientific, ios::floatfield);
  ofstream OutFile;

  OutFile.open("./data/domain/real_vertices", ios::out);

  using It = std::vector<std::pair<label, Point2d<double>>>::const_iterator;
  for (It it = real_vertices.begin(); it != real_vertices.end(); it++)
  {
    OutFile << it->second(0) <<' ' <<it->second(1) <<endl;
  }

  OutFile.close();
}
