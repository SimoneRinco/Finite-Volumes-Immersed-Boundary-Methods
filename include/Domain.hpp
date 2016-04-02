#pragma once
#include <Point2d_def.hpp>
#include <vector>
#include <string>

struct Domain
{
    std::vector<Point2d<double>> vertices;

    void save(const std::string& filename) const;
    void load(const std::string& filename);
};
