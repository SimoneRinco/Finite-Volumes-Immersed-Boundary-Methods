#include "Config.hpp"
#include <cereal/archives/json.hpp>
#include <fstream>

template<class Archive>
void serialize(Archive& ar, Config& config)
{
    ar(config.nx,
        config.ny,
        config.n_equations,
        config.k,
        config.x_min,
        config.x_max,
        config.y_min);      
}

void Config::save(const std::string& filename) const
{
    std::ofstream ofs(filename);
    cereal::JSONOutputArchive ar(ofs);
    ar(*this);    
}

void Config::load(const std::string& filename)
{
    std::ifstream ifs(filename);
    cereal::JSONInputArchive ar(ifs);
    ar(*this);
}
