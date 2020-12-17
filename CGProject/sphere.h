#pragma once
#include "common.h"
#include "material.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include<vector>
class Sphere
{
	glm::vec3 center;
	Material material;
	float radius;
	unsigned int VAO, VBO;
	unsigned int uniformBlockIndex;
	int theta_num = 40, phi_num = 40;
	std::vector<float> point_attribute;
	void init()
	{
		std::vector<glm::vec3> points;
		float unit_theta = M_PI * 2 / theta_num, unit_phi = M_PI / phi_num;
		//生成phi_num * theta_num个点
		for (int phi = 0; phi < phi_num + 1; phi++)
			for (int theta = 0; theta < theta_num; theta++)
			{
				float phi_val = -M_PI_2 + unit_phi * phi;
				float theta_val = unit_theta * theta;
				points.push_back(
					glm::vec3(radius * cos(phi_val) * sin(theta_val),
						radius * cos(phi_val) * cos(theta_val),
						radius * sin(phi_val)
					) + center);
			}
		// 层序遍历用这些点画四边形
		for (int phi = 0; phi < phi_num; phi++)
		{
			glm::vec3 p[4];
			for (int theta = 0; theta < theta_num; theta++)
			{
				p[0] = points[phi * theta_num + theta];
				p[1] = points[phi * theta_num + (theta + 1) % theta_num];
				p[2] = points[(phi + 1) * theta_num + (theta + 1) % theta_num];
				p[3] = points[(phi + 1) * theta_num + theta];		
				// 两个三角形
				for (int i = 0; i < 3; i++)
				{
					glm::vec3 normal;
					normal = glm::normalize(p[i] - center);
					point_attribute.insert(point_attribute.end(), {p[i].x, p[i].y, p[i].z});
					point_attribute.insert(point_attribute.end(), {normal.x, normal.y, normal.z});
				}
				for (int i = 2; i < 5; i++)
				{
					glm::vec3 normal;
					normal = glm::normalize(p[i % 4] - center);
					point_attribute.insert(point_attribute.end(), {p[i % 4].x, p[i % 4].y, p[i % 4].z});
					point_attribute.insert(point_attribute.end(), {normal.x, normal.y, normal.z});
				}				
			}
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &uniformBlockIndex);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, point_attribute.size() * sizeof(float) + sizeof(Material), point_attribute.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockIndex);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), (void*)(&material), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

public:
	Sphere(glm::vec3 center, float radius, Material& material)
	{
		this->center = center;
		this->radius = radius;
		this->material = std::move(material);
		init();
	}

	void paint()
	{
		glBindVertexArray(VAO);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBlockIndex, 0, sizeof(Material));
		glDrawArrays(GL_TRIANGLES, 0, point_attribute.size() / 6);
		glBindVertexArray(0);
	}
};