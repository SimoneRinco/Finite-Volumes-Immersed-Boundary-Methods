#include "Config.hpp"

int main(int argc, char **argv)
{
    Config config;
    config.nx = 200;
    config.ny = 100;
    config.n_equations = 3;
    config.k = 0.1414;
    config.x_min = -10.0;
    config.x_max = 10.0;
    config.y_min = -5.0;

    config.save("config.json");

    return 0;
}
