#ifndef TRIANGLE_MESH_HPP
#define TRIANGLE_MESH_HPP

#include "math.hpp"

struct TriangleMesh {

  std::vector<point3> vertices_;
  std::vector<vec3> vertices_;
  std::vector<point2> uvcoords_;

};

#endif //! TRIANGLE_MESH_HPP 
