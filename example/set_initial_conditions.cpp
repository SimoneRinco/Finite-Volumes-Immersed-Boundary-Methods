#include "finite_volume.hpp"

namespace INITIAL_CONDITIONS
{
  double z_init(const Point2d<double>& p)
  {
    auto l2_norm2 = p(0) * p(0) + p(1) * p(1);
    if(l2_norm2 < 0.5 * 0.5)
    {
      return 2.0;
    }
    else
    {
      return 1.0;
    }
  }

  double Qx_init(const Point2d<double>& p)
  {
    return 0.0;
  }

  double Qy_init(const Point2d<double>& p)
  {
    return 0.0;
  }
}

void FINITE_VOLUME::initial_conditions(grid& G)
{
  G.set_unknown_values(INITIAL_CONDITIONS::z_init, 1);
  G.set_unknown_values(INITIAL_CONDITIONS::Qx_init, 2);
  G.set_unknown_values(INITIAL_CONDITIONS::Qy_init, 3);
}
