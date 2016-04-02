#pragma once
#include <string>

class Config
{
public:

    int nx = 0;
    int ny = 0;
    int n_equations = 0;
    double k = 0.0;
    double x_min = 0.0;
    double x_max = 0.0;
    double y_min = 0.0; 

    void save(const std::string& filename) const;
    void load(const std::string& filename);
};
