#pragma once
#include "common.h"
#include<vector>
class Circle
{
	int sector_num = 50;
	float* data_arr;
	glm::vec3 color;
	unsigned int VAO, VBO;
public:
	Circle() {}

	void set_color(glm::vec3 color)
	{
		this->color = color;
	}

	void init()
	{
		float PI = 3.1415926f;
		std::vector<float> data;
		float step_theta = 2 * PI / sector_num;
		for (int i = 0; i < sector_num; i++)
		{
			float theta = i * step_theta;
			float x = cos(theta);
			float y = sin(theta);
			float x1 = cos(theta + step_theta);
			float y1 = sin(theta + step_theta);
			data.push_back(x);
			data.push_back(y);
			data.push_back(0);
			data.push_back(color.x);
			data.push_back(color.y);
			data.push_back(color.z);
			data.push_back(x1);
			data.push_back(y1);
			data.push_back(0);
			data.push_back(color.x);
			data.push_back(color.y);
			data.push_back(color.z);
			data.push_back(0);
			data.push_back(0);
			data.push_back(0);
			data.push_back(color.x);
			data.push_back(color.y);
			data.push_back(color.z);
		}
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void paint()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3 * sector_num);
		glBindVertexArray(0);

	}
};