/*
 * Point2d_def.hpp
 *
 *  Created on: 07/nov/2011
 *      Author: ubuntu
 */

#ifndef POINT2D_DEF_HPP_
#define POINT2D_DEF_HPP_

#include "Point2d.hpp"
#include <iostream>

template <typename T>
Point2d<T>::Point2d()
: first(T())
, second(T())
{}

template <typename T>
Point2d<T>::Point2d(const T& first,const T& second): first(first), second(second) {}

template<typename T>
Point2d<T>::Point2d(const Point2d<T>& lhs): first(lhs.first), second(lhs.second) {}

template<typename T>
Point2d<T>::~Point2d(){}

template<typename T>
void Point2d<T>::operator=(const Point2d<T>& lhs)
{
	first = lhs.first;
	second = lhs.second;
}

template<typename T>
bool Point2d<T>::operator==(const Point2d<T>& lhs)
{
	return (first==lhs.first)&&(second==lhs.second);
}

template<typename T>
bool Point2d<T>::operator==(const Point2d<T>& lhs) const
{
	return (first==lhs.first)&&(second==lhs.second);
}

template<typename T>
T& Point2d<T>::operator()(const int& i)
{
  if (i==0)
  {
    return this->first;
  }
  else if (i==1)
  {
    return this->second;
  }

  throw 1;
}

template<typename T>
const T& Point2d<T>::operator()(const int& i) const
{
  if (i==0)
  {
    return this->first;
  }
  else if (i==1)
  {
    return this->second;
  }

  throw 1;
}


template<typename T>
void Point2d<T>::print()
{
	std::cout <<"("<< first <<"," << second <<")" <<std::endl;
}

template<typename T>
void Point2d<T>::print() const
{
	std::cout <<"("<< first <<"," << second <<")" <<std::endl;
}

template<typename T>
void Point2d<T>::set_values(const T& first,const T& second)
{
	this->first=first;
	this->second=second;
}



#endif /* POINT2D_DEF_HPP_ */
