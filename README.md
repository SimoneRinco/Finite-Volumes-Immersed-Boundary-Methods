# sysel: a finite volume immersed boundary solver

![Shallow Waters simulation in a closed square domain](http://drive.google.com/uc?export=view&id=1IKWfdbEJSSS_s2WbxxoCXjJ7n9foWJra)

This project contains the code of my Master thesis about Immersed Boundary Finite Volumes Methods applied to the Shallow Waters system.
The thesis can be found [here](https://www.politesi.polimi.it/handle/10589/51682?mode=full) (in Italian, sorry about that).

The code you see here is just intended to show what I did. By no means it aims to be used as it is, as it is very poorly designed (and also extremely inefficient). But it works and you can play with the Shallow Waters system changing the domain shape, the mesh and the initial conditions.
The code has been tested on Linux only and compiled with gcc 7.3.0.
In order to build the example to the following:

```bash
mkdir build && cd build # create a build directory
cmake ..
make
```
These commands create the executable ```ibfv_example``` in the build directory.

The simulator assumes configuration files and the directory tree used to store the output already exist when the simulation
is launched. In order to create those files execute (assuming you are in the root directory of this project):

```bash
./setup_dirs.sh mydata
```

This creates a directory called ```mydata``` with all the configuration files and the necessary subdirectories.

Now you can start a simulation:

```bash
cd build
./ibfv_example ../mydata
```

The output files with water height, Qx and Qy are stored in ```mydata/unknowns```.
The matlab scripts in the ```m_files``` directory contain the code that parses the simulation output files and plots the domain,
the cells and figures of the solution for each frame.

