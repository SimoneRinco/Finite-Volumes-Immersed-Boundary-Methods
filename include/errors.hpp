/*
 * errors.hpp
 *
 *  Created on: 15/nov/2011
 *      Author: ubuntu
 */

#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include <iostream>
#include "Point2d.hpp"
#include "Point2d_def.hpp"
#include "immersed_boundary.hpp"

/*
 * La classe errors si limita a stampare il numero dell'errore nel quale si è occorso.
 */

namespace ERRORS
{
class errors
{
public:
	errors(const int&);
};
}

#endif /* ERRORS_HPP_ */
