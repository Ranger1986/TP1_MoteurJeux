#include "Scene.h"
Scene::Scene(/* args */)
{

}
void Scene::addChild(Scene *child){
    children.push_back(child);
}
vector<Scene *> Scene::getChildren(){
    return children;
}
vector<vec3> Scene::getVertices(){
    vector<vec3> temp_vertices = indexed_vertices;
    vector<vec3> result_vertices;
    for (size_t i = 0; i < children.size(); i++)
    {
        vector<vec3> child_vertices = children[i]->getVertices();
        for (size_t j = 0; j < child_vertices.size(); j++)
        {
            temp_vertices.push_back(child_vertices[j]);
        }
    }
    for (size_t i = 0; i < temp_vertices.size(); i++)
    {
        result_vertices.push_back(transform.applyToPoint(temp_vertices[i]));
    }
    return result_vertices;
}
vector<vec3> Scene::getColor(){
    vector<vec3> indexed_color;
    for (size_t i = 0; i < indexed_vertices.size(); i++)
    {
        indexed_color.push_back(color);
    }
    for (size_t i = 0; i < children.size(); i++)
    {
        vector<vec3> child_color = children[i]->getColor();
        for (size_t j = 0; j < child_color.size(); j++)
        {
            indexed_color.push_back(child_color[j]);
        }
    }
    return indexed_color;

}
vector<unsigned short> Scene::getIndices(){
    vector<unsigned short> result_indices = indices;
    long indices_size= indexed_vertices.size();
    for (size_t i = 0; i < children.size(); i++)
    {
        vector<unsigned short> child_indices = children[i]->getIndices();
        for (size_t j = 0; j < child_indices.size(); j++)
        {
            result_indices.push_back(indices_size+child_indices[j]);
        }
        indices_size+=children[i]->getVertices().size();
    }
    return result_indices;
}
vector<float> Scene::getTexCoords(){
    vector<float> result_texCoords = texCoords;
    for (size_t i = 0; i < children.size(); i++)
    {
        vector<float> child_texCoords = children[i]->getTexCoords();
        for (size_t j = 0; j < child_texCoords.size(); j++)
        {
            result_texCoords.push_back(child_texCoords[j]);
        }
    }
    return result_texCoords;
}

Scene::~Scene()
{
}
