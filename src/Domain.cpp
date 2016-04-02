#include "Domain.hpp"
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

template<class Archive>
void serialize(Archive& ar, Domain& domain)
{
    ar(domain.vertices);
}

void Domain::save(const std::string& filename) const
{
    std::ofstream ofs(filename);
    cereal::JSONOutputArchive ar(ofs);
    ar(*this);
}

void Domain::load(const std::string& filename)
{
    std::ifstream ifs(filename);
    cereal::JSONInputArchive ar(ifs);
    ar(*this);
}

