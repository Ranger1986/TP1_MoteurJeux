#include "Transform.h"
Transform::Transform()
{
    this->m=mat3{1.f};
    this->t=vec3{0.0f};
}
Transform::Transform(mat3 matrice)
{
    this->m=matrice;
    this->t=vec3{0.0f};
}
Transform::Transform(vec3 translation)
{
    this->m=mat3{1.f};
    this->t=translation;
}
Transform::Transform(mat3 m, vec3 t)
{
    this->m=m;
    this->t=t;
}
void Transform::rotx(float angle){
    mat3 mat_rot={
        1,0,0,
        0,cos(radians(angle)),-sin(radians(angle)),
        0,sin(radians(angle)),cos(radians(angle)),
    };
    this->m=m*mat_rot;
}
void Transform::roty(float angle){
    mat3 mat_rot={
        cos(radians(angle)),0,sin(radians(angle)),
        0,1,0,
        -sin(radians(angle)),0,cos(radians(angle))
    };
    this->m=m*mat_rot;
}
void Transform::rotz(float angle){
    mat3 mat_rot={
        cos(radians(angle)),-sin(radians(angle)),0,
       sin(radians(angle)),cos(radians(angle)),0,
       0,0,1
    };
    this->m=m*mat_rot;
}
void Transform::translate(vec3 translation){
    this->t=translation;
}
void Transform::scale(float ratio){
    this->m=m*(mat3(1.0)*ratio);
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
