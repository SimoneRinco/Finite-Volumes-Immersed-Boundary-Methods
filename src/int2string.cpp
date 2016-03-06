/*
 * int2string.cpp
 *
 *  Created on: 19/dic/2011
 *      Author: ubuntu
 */

#include "immersed_boundary.hpp"

 std::string IMMERSED_BOUNDARY::int2string(const int& number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
