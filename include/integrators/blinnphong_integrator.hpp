#ifndef BLINNPHONG_INTEGRATOR_HPP
#define BLINNPHONG_INTEGRATOR_HPP

#include "integrator.hpp"
#include "scene.hpp"
#include "materials/blinnphong_material.hpp"
#include "lights/light.hpp"
#include <algorithm>
#include <cmath> //

class BlinnPhongIntegrator : public Integrator {
public:
    BlinnPhongIntegrator() = default;

    std::optional<RGBColor> li(const Ray& ray, const Scene& sc) override {
        Surfel surfel; 

        // Busca a interseção mais próxima. Se não houver colisão, retorna vazio para a base usar o background
        if (!sc.aggregate_->intersect(ray, surfel)) {
            return {}; 
        }

        // Tenta converter o material para BlinnPhongMaterial
        auto mat = std::dynamic_pointer_cast<BlinnPhongMaterial>(surfel.mat_);
        
        // Se o material for de outro tipo (ex: Flat), retorna a cor direta dele
        if (!mat) {
            return surfel.mat_->getColor();
        }

        // Preparação dos vetores para o cálculo de iluminação
        RGBColor L_total(0, 0, 0);      
        vec3 V = normalize(ray.direction() * -1.0f); // inverso da direção do raio (fiquei com preguiça de criar um método específico para isso)
        vec3 N = normalize(surfel.n); 

        // Loop por todas as luzes da cena
        for (const auto& light : sc.lights_) {
            vec3 Li_dir; // Direção da luz (l)
            float dist;  // Distância até a luz
            
            // Usa .p (ponto de interseção) para calcular a direção da luz
            RGBColor intensity = light->sample_Li(surfel.p, Li_dir, dist);

            // Cálculo por componente de luz
            if (light->flags == LightFlag::Ambient) {
                // Componente Ambiente: L = Ia * ka
                L_total += intensity * mat->ka;
            } else {
                // Componente Difusa (Lambertiana): I * kd * max(0, n . l)
                float cos_theta = std::max(0.0f, dot(N, Li_dir));
                RGBColor diffuse = mat->kd * intensity * cos_theta;

                // Componente Especular (Blinn): I * ks * max(0, n . h)^g
                // h é o vetor médio (half-vector) entre V e L
                vec3 H = normalize(V + Li_dir); 
                float cos_alpha = std::max(0.0f, dot(N, H));
                RGBColor specular = mat->ks * intensity * std::pow(cos_alpha, mat->glossiness);

                L_total += diffuse + specular;
            }
        }

        return L_total;
    }
};

#endif // BLINNPHONG_INTEGRATOR_HPP