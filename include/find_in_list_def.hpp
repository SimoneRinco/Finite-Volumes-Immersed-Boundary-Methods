#include "immersed_boundary.hpp"

template <typename T>
unsigned int IMMERSED_BOUNDARY::find_in_list(const T& element, const std::list<T>& l)
{
  // restitiusce la posizione nella lista dell'elemento cercato. Ritorna -1 se non trovato
  std::list<T> l2;
  typename std::list<T>::const_iterator it = l.begin();
  unsigned int pos = -1;
  unsigned int counter = 0;

  while (it!=l.end())
  {
    if ((*it)==element)
    {
      pos=counter;
      break;
    }
    else
    {
      counter++;
    }
  }
  return pos;
}
