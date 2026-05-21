#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"
#include <memory>

class Triangle : public Shape {
private:

  int *v_; /* Vertices */
  int *n_; /* Normais */
  int *uv_; /* Coodernadas u e v */

  std::shared_ptr<TriangleMesh> tmesh_; /* Malha de triângulos. Owner dos triângulos de uma superfície única. */

  bool backface_;

public:

  bool intersect(const Ray &r, Surfel &s) const override {
	  return true;
  }

};

#endif //! TRIANGLE_HPP
