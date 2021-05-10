#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend_util.h"

class material;

struct hit_record {
    point3 p; // hit point
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face; // is this hit on the front or back of a face.

    // I think this doesn't need to be inline?
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        auto incident_normal = front_face ? outward_normal : -outward_normal;
        normal = incident_normal;
    }
};

// abstract class
class hittable {
public:
    // NOTE: this is a pure virtual function which means it MUST be overwritten in the child class.
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0; 
};

#endif