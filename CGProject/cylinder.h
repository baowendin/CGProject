#pragma once
#include "common.h"
#include<vector>
constexpr auto PI = 3.1415926f;
class Cylinder
{
	float height = 1, radius = 1;
	int sector_num = 50, height_num = 1;
	glm::vec3 surface_color, top_color, bottom_color;
	unsigned int VAO, VBO;

	void init_surface_data(std::vector<float>& data);
	void init_top_data(std::vector<float>& data);
	
public:
	Cylinder()
	{	
	}
	// resize the cylinder
	void init();
	void paint();
	void set_surface_color(glm::vec3 color)
	{
		this->surface_color = color;
	}
	void set_top_color(glm::vec3 color)
	{
		this->top_color = color;
	}
	void set_bottom_color(glm::vec3 color)
	{
		this->bottom_color = color;
	}
};