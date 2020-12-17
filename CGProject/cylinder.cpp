#include "cylinder.h"
void Cylinder::paint()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * sector_num * height_num + 3 * 2 * sector_num);
	glBindVertexArray(0);
}

void Cylinder::init_surface_data(std::vector<float>& data)
{
	data.clear();
	float step_theta = 2 * PI / sector_num;
	float step_height = height / height_num;
	for (int i = 0; i < sector_num; i++)
	{
		float theta = i * step_theta;
		for (int j = 0; j < height_num; j++)
		{
			float x = cos(theta) * radius;
			float y = sin(theta) * radius;
			float z = j * step_height;
			float x1 = cos(theta + step_theta) * radius;
			float y1 = sin(theta + step_theta) * radius;
			float z1 = (j + 1) * step_height;
			// 画一个矩形
			// 画三角形1
			data.push_back(x);
			data.push_back(y);
			data.push_back(z);
			data.push_back(surface_color.x);
			data.push_back(surface_color.y);
			data.push_back(surface_color.z);
			data.push_back(x1);
			data.push_back(y1);
			data.push_back(z);
			data.push_back(surface_color.x);
			data.push_back(surface_color.y);
			data.push_back(surface_color.z);
			data.push_back(x1);
			data.push_back(y1);
			data.push_back(z1);
			data.push_back(surface_color.x);
			data.push_back(surface_color.y);
			data.push_back(surface_color.z);

			// 画三角形2
			data.push_back(x1);
			data.push_back(y1);
			data.push_back(z1);
			data.push_back(surface_color.x);
			data.push_back(surface_color.y);
			data.push_back(surface_color.z);
			data.push_back(x);
			data.push_back(y);
			data.push_back(z1);
			data.push_back(surface_color.x);
			data.push_back(surface_color.y);
			data.push_back(surface_color.z);
			data.push_back(x);
			data.push_back(y);
			data.push_back(z);
			data.push_back(surface_color.x);
			data.push_back(surface_color.y);
			data.push_back(surface_color.z);

		}
	}
}

void Cylinder::init_top_data(std::vector<float>& data)
{
	float step_theta = 2 * PI / sector_num;
	// 顶部
	for (int i = 0; i < sector_num; i++)
	{
		float theta = i * step_theta;
		float x = cos(theta) * radius;
		float y = sin(theta) * radius;
		float x1 = cos(theta + step_theta) * radius;
		float y1 = sin(theta + step_theta) * radius;
		data.push_back(x);
		data.push_back(y);
		data.push_back(height);
		data.push_back(top_color.x);
		data.push_back(top_color.y);
		data.push_back(top_color.z);
		data.push_back(x1);
		data.push_back(y1);
		data.push_back(height);
		data.push_back(top_color.x);
		data.push_back(top_color.y);
		data.push_back(top_color.z);
		data.push_back(0);
		data.push_back(0);
		data.push_back(height);
		data.push_back(top_color.x);
		data.push_back(top_color.y);
		data.push_back(top_color.z);
	}

	//底部
	for (int i = 0; i < sector_num; i++)
	{
		float theta = i * step_theta;
		float x = cos(theta) * radius;
		float y = sin(theta) * radius;
		float x1 = cos(theta + step_theta) * radius;
		float y1 = sin(theta + step_theta) * radius;
		data.push_back(x1);
		data.push_back(y1);
		data.push_back(0);
		data.push_back(bottom_color.x);
		data.push_back(bottom_color.y);
		data.push_back(bottom_color.z);
		data.push_back(x);
		data.push_back(y);
		data.push_back(0);
		data.push_back(bottom_color.x);
		data.push_back(bottom_color.y);
		data.push_back(bottom_color.z);
		data.push_back(0);
		data.push_back(0);
		data.push_back(0);
		data.push_back(bottom_color.x);
		data.push_back(bottom_color.y);
		data.push_back(bottom_color.z);
	}
}

void Cylinder::init()
{
	std::vector<float> data;
	init_surface_data(data);
	init_top_data(data);

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