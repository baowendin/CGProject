#pragma once
struct Material
{
    glm::vec4 ka;
    glm::vec4 kd;
    glm::vec4 ks;
    float shinness;
};

struct Attribute
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texture;
};