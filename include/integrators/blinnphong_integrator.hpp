#ifndef BLINNPHONG_INTEGRATOR_HPP
#define BLINNPHONG_INTEGRATOR_HPP

#include "integrator.hpp"
#include "scene.hpp"
#include "lights/light.hpp"

class BlinnPhongIntegrator : public Integrator {
public:
    BlinnPhongIntegrator() = default;

    std::optional<RGBColor> li(const Ray& ray, const Scene& sc) override {
        Surfel surfel; 

        //  se não houver intersecção, o Integrator base usa o background
        if (!sc.aggregate_->intersect(ray, surfel)) {
            return {}; 
        }

        RGBColor L_total(0, 0, 0);      
        vec3 wo = normalize(ray.direction() * -1.0f); 
        vec3 n = normalize(surfel.n); 

        // 
        for (const auto& light : sc.lights_) {
            vec3 wi;    // Direção da luz que será preenchida pelo sample_Li
            float dist; 
            
            // Obtém a intensidade e a direção da luz (wi) para o ponto atual
            RGBColor intensity = light->sample_Li(surfel.p, wi, dist);

            if (light->flags == LightFlag::Ambient) {
                // Componente Ambiente: L = Ia * ka (usamos getColor para obter ka/kd base)
                L_total += intensity * surfel.mat_->getColor();
            } else {
                // O Integrator não sabe se é Blinn ou Flat, ele apenas pede o "espalhamento" da luz[cite: 3, 33]
                L_total += intensity * surfel.mat_->scatter(wo, wi, n);
            }
        }

        return L_total;
    }
};

#endif // !BLINNPHONG_INTEGRATOR_HPP