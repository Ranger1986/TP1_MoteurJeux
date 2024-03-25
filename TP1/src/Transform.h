#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>

using namespace glm;
class Transform
{
private:
public:
    // rotation + skew + scale
    mat3 m;
    // translation
    vec3 t;
    Transform();
    Transform(mat3 matrice);
    Transform(vec3 translation);
    Transform(mat3 m, vec3 t);
    vec3 applyToPoint(vec3 p);
    vec3 applyToVector(vec3 v);

    ~Transform();
};
#endif