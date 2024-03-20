#include "Transform.h"
Transform::Transform()
{
    this->m=mat3{1.f};
    this->t=vec3{0.0f};
}
Transform::Transform(mat3 m, vec3 t)
{
    this->m=m;
    this->t=t;
}
vec3 Transform::applyToPoint(vec3 p){
    return m*p+t;
}
vec3 Transform::applyToVector(vec3 v){
    return m*v;
}
Transform::~Transform()
{
}
