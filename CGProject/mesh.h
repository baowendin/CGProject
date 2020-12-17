#pragma once
#include "common.h"
#include "shader.h"
#include<string>
#include<vector>
#include "material.h"
using namespace std;
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // texcoords
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture
{
    unsigned int id;
    string type;
    string path;
};



class Mesh
{
public:
    /*  网格数据  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Material material;
    unsigned int uniformBlockIndex;
    /*  函数  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Material material);
    
    void Draw(Shader& shader);
private:
    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;
    /*  函数  */
    void setupMesh();
};