/*
 * find_in_list.cpp
 *
 *  Created on: 22/nov/2011
 *      Author: ubuntu
 */

#include "immersed_boundary.hpp"

template <typename T>
unsigned int IMMERSED_BOUNDARY::find_in_list(const T& element, const std::list<T>& l)
{
	// restitiusce la posizione nella lista dell'elemento cercato. Ritorna -1 se non trovato
	std::list<T> l2;
	typename std::list<T>::const_iterator it = l.begin();
	bool finded=false;
	unsigned int pos=-1;
	unsigned int counter=0;

	while ((finded==false) || it!=l.end())
	{
		if ((*it)==element)
			{
			pos=counter;
			finded=true;
			}
		else
		{
			counter++;
		}
	}
	return pos;
}
