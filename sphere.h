#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    point3 center;
    double radius;

public:
    sphere() {}
    sphere(point3 cen, double r) : center(cen), radius(r) {};

    // This virtual is telling us that this function is technically virtual, no
    // matter what we do.
    virtual bool hit(
        const ray& r, double t_min, double t_max, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a; // try further root if first was invalid.
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root; // This makes sense because we're solving for t.
    rec.p = r.at(rec.t);

    // Division by radius makes it a unit vector b/c the vector from center to  
    // the hit point has magnitude = radius.
    vec3 outward_normal = (rec.p - center) / radius; //rec.normal = (rec.p - center) / radius;  
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif