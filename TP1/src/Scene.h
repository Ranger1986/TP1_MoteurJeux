#ifndef SCENE_H
#define SCENE_H
#include <vector>

#include "Transform.h"

using namespace std;
/**
 * @brief Representation d'une scene 3D
 *
 */
class Scene {
   private:
    // liste des scenes dependantes de la scene
    vector<Scene *> children;

   public:
    // information de transformation de la scene
    Transform transform;
    // liste des triangles (3 unsigned short -> 1 triangle)
    vector<unsigned short> indices;
    // coordonnées des points
    vector<vec3> indexed_vertices;
    vec3 color;
    // coordonnées de texture TODO
    vector<float> texCoords;
    Scene();
    /**
     * @brief Ajoute une scene enfant a la scene.
     * la scene enfant subira la transformation de la scene mere
     * en plus de sa transformation
     *
     * @param child
     */
    void addChild(Scene *child);
    vector<Scene *> getChildren();
    vector<unsigned short> getIndices();
    vector<vec3> getVertices();
    vector<vec3> getColor();
    vector<float> getTexCoords();
    ~Scene();
};
#endif
