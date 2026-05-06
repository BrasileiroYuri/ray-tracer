#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "background.hpp"
#include "camera.hpp"
#include "orthographic_camera.hpp"
#include "perspective_camera.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "material.hpp"
#include <memory>
#include <optional>
#include <string>

struct RenderConfig {};

class Integrator {
public:
    virtual ~Integrator() = default;

    // O método render permanece genérico
    virtual void render(const Scene &sc) {
        auto width = cam_->widht();
        auto height = cam_->height();

        for (std::size_t j = 0; j < height; j++) {
            for (std::size_t i = 0; i < width; i++) {
                Ray ray = cam_->generateRay(i, j);
                auto color = li(ray, sc).value_or(
                    sc.backGroundColor_.sample(float(i) / float(width - 1),
                                               1.f - (float(j) / (float)(height - 1))));
                cam_->add(i, j, color);
            }
        }
    }

    void makeCamera(CameraConfig cc); 
    void write_image(std::string &f, bool ppm = false) const;

protected:
    std::unique_ptr<Camera> cam_ = nullptr;
    virtual std::optional<RGBColor> li(const Ray &ray, const Scene &sc) = 0;
};
#endif