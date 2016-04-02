/*
 * Point2d.hpp
 *
 *  Created on: 07/nov/2011
 *      Author: ubuntu
 */

#ifndef POINT2D_HPP_
#define POINT2D_HPP_

template <typename T>
class Point2d{

	T first;
	T second;

public:
	Point2d();
	Point2d(const T& first,const T& second);
	Point2d(const Point2d&);
	~Point2d();

    template<class Archive>
    void serialize(Archive& ar)
    {
        ar(first, second);
    }

	void operator= (const Point2d&);
	bool operator== (const Point2d&);
	bool operator== (const Point2d&) const;
	T& operator() (const int& i);
	const T& operator() (const int& i) const;
	void print();
	void print() const;
	void set_values(const T&, const T&);
};


#endif /* POINT2D_HPP_ */
