#include "./Plan.h"

Plan::Plan(vec3 center, int width_vertices, int length_vertices) {
    this->center = center;
    this->width_vertices = width_vertices;
    this->length_vertices = length_vertices;
    this->vertices_distance = 1;
    for (int i = 0; i < width_vertices; i++) {
        vector<float> witdh_height;
        for (int i = 0; i < length_vertices; i++) {
            witdh_height.push_back(0);
        }
        heights.push_back(witdh_height);
    }
}
void Plan::setRandomHeight() {
    for (int i = 0; i < width_vertices; i++) {
        for (int j = 0; j < length_vertices; j++) {
            heights[i][j]=static_cast<float>(rand())/RAND_MAX-0.5;
        }
    }
}
void Plan::drawPlan(std::vector<glm::vec3>& indexed_vertices, std::vector<unsigned short>& indices) {
    for (int i = 0; i < width_vertices; i++) {
        for (int j = 0; j < length_vertices; j++) {
            indexed_vertices.push_back(center + 
                vec3(vertices_distance / (length_vertices - 1) * j,
                    heights[i][j],
                    vertices_distance / (width_vertices - 1) * i));
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
