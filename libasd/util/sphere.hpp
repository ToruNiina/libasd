#ifndef LIBASD_SPHERE_H
#define LIBASD_SPHERE_H
#include <libasd/point.hpp>

namespace asd
{

template<typename realT>
class sphere
{
  public:
    typedef realT real_type;
    typedef point<real_type, 3> position_type;

  public:
    sphere()  = default;
    ~sphere() = default;
    sphere(sphere const&) = default;
    sphere(sphere&&)      = default;
    sphere& operator=(sphere const&) = default;
    sphere& operator=(sphere&&)      = default;

    sphere(const real_type& r) : radius(r), center(0.,0.,0.){}
    sphere(const position_type& c) : radius(0), center(c){}
    sphere(const real_type& r, const position_type& c): radius(r), center(c){}

    real_type     radius;
    position_type center;
};

} // asd
#endif// LIBASD_SPHERE_H
