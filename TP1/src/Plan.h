#ifndef PLAN_H
#define PLAN_H
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>

#include <iostream>
#include <vector>
using namespace glm;
class Plan
{
private:
    vec3 center;
    int width_vertices;
    int length_vertices;
    float vertices_distance;
public:
    Plan(vec3 center, int width_vertices, int length_vertices);
    void drawPlan(std::vector<glm::vec3>& indexed_vertices, std::vector<unsigned short>& indices);
    //~Plan();
};
#endif