#ifndef SCENE_H
#define SCENE_H
#include <vector>

#include "Transform.h"

using namespace std;

class Scene
{
private:
    vector<Scene> children;
public:
    Transform transform;
    vector<unsigned short> indices;
    vector<vec3> indexed_vertices;
    vector<float> texCoords;
    Scene(/* args */);
    void addChild(Scene child);
    vector<unsigned short> getIndices();
    vector<vec3> getVertices();
    vector<float> getTexCoords();
    ~Scene();
};
#endif
