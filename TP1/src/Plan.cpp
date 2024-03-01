#include "./Plan.h"

Plan::Plan(vec3 center, int width_vertices, int length_vertices)
{
    this->center=center;
    this->width_vertices=width_vertices;
    this->length_vertices=length_vertices;
    this->vertices_distance=1;
}
void Plan::drawPlan(std::vector<glm::vec3>& indexed_vertices, std::vector<unsigned short>& indices) {
    for (int i = 0; i < width_vertices; i++) {
        for (int j = 0; j < length_vertices; j++) {
            indexed_vertices.push_back(center + vec3(vertices_distance / (width_vertices - 1) * i, -vertices_distance / (length_vertices - 1) * j, 0));
        }
    }
    for (int i = 0; i < width_vertices - 1; i++) {
        for (int j = 0; j < length_vertices - 1; j++) {
            indices.push_back(i * length_vertices + j);
            indices.push_back((i + 1) * length_vertices + j);
            indices.push_back((i + 1) * length_vertices + j + 1);
            indices.push_back(i * length_vertices + j);
            indices.push_back(i * length_vertices + j + 1);
            indices.push_back((i + 1) * length_vertices + j + 1);
        }
    }
}
/*
Plan::~Plan()
{
}
*/
