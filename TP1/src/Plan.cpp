#include "Plan.h"

Plan::Plan(vec3 center, int width_vertices, int length_vertices) {
    this->center = center;
    this->width_vertices = width_vertices;
    this->length_vertices = length_vertices;
    this->cote_distance = 10;
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
            heights[i][j] = static_cast<float>(rand()) / RAND_MAX - 0.5;
        }
    }
}
void Plan::drawPlan(std::vector<glm::vec3>& indexed_vertices, std::vector<unsigned short>& indices,std::vector<float> &texCoords) {
    float half_width = cote_distance / 2;
    float half_length =  cote_distance / 2;
    for (int i = 0; i < width_vertices; i++) {
        for (int j = 0; j < length_vertices; j++) {
            indexed_vertices.push_back(center +
                                       vec3(cote_distance/length_vertices * j - half_width,
                                            heights[i][j],
                                            cote_distance/width_vertices * i - half_length));
            texCoords.push_back((float)i / (float)width_vertices);
            texCoords.push_back((float)j / (float)length_vertices);
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

            texCoords.push_back((float)(i + 1) / (float)width_vertices);
            texCoords.push_back((float)j / (float)length_vertices);

            texCoords.push_back((float)(i + 1) / (float)width_vertices);
            texCoords.push_back((float)(j + 1) / (float)length_vertices);

            texCoords.push_back((float)(i) / (float)width_vertices);
            texCoords.push_back((float)(j + 1) / (float)length_vertices);

            texCoords.push_back((float)(i) / (float)width_vertices);
            texCoords.push_back((float)j / (float)length_vertices);

            texCoords.push_back((float)(i + 1) / (float)width_vertices);
            texCoords.push_back((float)(j) / (float)length_vertices);

            texCoords.push_back((float)(i) / (float)width_vertices);
            texCoords.push_back((float)(j + 1) / (float)length_vertices);

        }
    }
}