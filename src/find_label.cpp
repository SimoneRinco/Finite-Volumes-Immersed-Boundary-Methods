#include "immersed_boundary.hpp"

IMMERSED_BOUNDARY::label IMMERSED_BOUNDARY::find_label(
  const std::vector<double>& x,
  const std::vector<double>& y,
  const Point2d<double>& p)
{
  unsigned int i = 0;
  unsigned int j = 0;

  if (p(0) < x.at(0))
  {
    std::cout << "Errore: vertice ";
    p.print();
    std::cout << "fuori dai limiti indicati\n";
    throw 1;
  }

  while (p(0) > x.at(i))
  {
    i++;
    if (i >= x.size())
    {
      std::cout << "Errore: vertice ";
      p.print();
      std::cout << "fuori dai limiti indicati\n";
      throw 1;
    }
  }

  if (p(1) < y.at(0))
  {
    std::cout << "Errore: vertice ";
    p.print();
    std::cout << "fuori dai limiti indicati\n";
    throw 1;
  }
  while (p(1) > y.at(j))
  {
    j++;
    if (j >= y.size())
    {
      std::cout<< "Errore: vertice ";
      p.print();
      std::cout<< "fuori dai limiti indicati\n";
      throw 1;
    }
  }

  return label(i-1, j-1);
}
