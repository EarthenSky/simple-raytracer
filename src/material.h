#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend_util.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        color albedo;

    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector(); // random_in_hemisphere(rec.normal);

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal; // this aliases, but perhaps a bit more consistent than double precision?

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {
    public:
        color albedo;
        double fuzz;
    
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);  // just do a normal reflection
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());

            attenuation = albedo;

            // if the component of the scatter ray in the direction of the normal is positive
            return (dot(scattered.direction(), rec.normal) > 0); 
        }
};


#endif
