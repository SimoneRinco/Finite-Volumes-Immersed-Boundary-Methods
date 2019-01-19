#include "finite_volume.hpp"
#include <string>
#include <sstream>

int main(int argc, char **argv)
{
  using namespace IMMERSED_BOUNDARY;
  using namespace FINITE_VOLUME;

  if (argc != 2)
  {
    std::cout << "Usage:" << std::endl;
    std::cout << "example <data_directory>" << std::endl;
    std::cout << "where <data_directory> is the directory "
      "containing the input file and where the results of the simulation "
      "will be stored. Use script setup_dirs.sh to create such a directory";
    std::cout << std::endl;
    return 1;
  }

  const std::string data_dir(argv[1]);
  const std::string input_file(data_dir + "/input_parameters.pot");
  const std::string vertices_file(data_dir + "/vertices.pot");

  grid G(input_file, vertices_file);

  const std::vector<label>& WC(G.get_wet_cells());
  unsigned int n_frame = 0;
  const double CFL = 0.5;
  const double dx = G.get_dx(), dy = G.get_dy();
  const double t_final = 1.0;
  const double dt_out = 0.1;
  double t = 0.0;
  double dt;

  bool save_output = false;

  // incognite: valori centrali e valori alle interfacce ->
  // -> limiti dall'interno della cella

  cell_unknown z(G,1);
  cell_unknown Qx(G,2);
  cell_unknown Qy(G,3);

  // valori delle incognite (a centro cella) nel nuovo istante temporale

  unknown new_z(G,1);
  unknown new_Qx(G,2);
  unknown new_Qy(G,3);

  ///////////////
  G.writeout_domain(); // salvataggio in ./data/domain
  // Imposizione delle condizioni iniziali in G
  initial_conditions(G);

  // salvataggio condizioni iniziali
  std::stringstream ss;
  ss << data_dir << "/data/unknowns/frame_";
  ss << n_frame;

  G.writeout_unknowns(ss.str());
  n_frame++;

  while (t < t_final)
  {
    if (save_output)
    {
      // (1) Salvataggio incognite (medie di cella nelle wet cells)
      std::cout << "Saving number " << n_frame << " at time " << t << std::endl;
      std::stringstream ss;
      ss << data_dir << "/data/unknowns/frame_";
      ss << n_frame;
      G.writeout_unknowns(ss.str());
      n_frame++;
      save_output = false;
    }
    // (2) Imposizione delle condizioni al bordo (medie di cella)
    // necessarie alla fase di ricostruzione
    G.boundary_conditions();
    // estrazione delle incognite nelle wet e ghost cells
    z.central.import_unknown();
    Qx.central.import_unknown();
    Qy.central.import_unknown();

    // (3) Ricostruzione lineare a pezzi delle variabili: calcolo dei valori
    // alle 4 interfacce (limiti interni) per ogni wet cell
    reconstruction(G, z, Qx, Qy, WC);

    // (4) Imposizione delle condizioni al contorno ai valori di interfaccia
    // necessarie all'applicazione del metodo centrato
    interfaces_boundary_conditions(G, z, Qx, Qy);

    // (5) calcolo di dt
    dt = time_step(z, Qx, Qy, WC, dx, dy, CFL);
    if (t + dt > dt_out * n_frame)
    {
      dt = dt_out * n_frame - t;
      save_output = true;
    }

    t += dt;
    std::cout << "dt=" << dt <<"; t= " << t << std::endl;
    // (6) calcolo incognite al tempo successivo
    evolution(z, Qx, Qy, new_z, new_Qx, new_Qy, WC, dx, dy, dt);
  }

  G.info();
}
