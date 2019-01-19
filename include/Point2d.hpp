#pragma once

template <typename T>
class Point2d
{
public:
  Point2d();
  Point2d(const T& first,const T& second);
  Point2d(const Point2d&);
  ~Point2d();
  void operator= (const Point2d&);
  bool operator== (const Point2d&);
  bool operator== (const Point2d&) const;
  T& operator() (const int& i);
  const T& operator() (const int& i) const;
  void print();
  void print() const;
  void set_values(const T&, const T&);
private:
  T first;
  T second;
};
