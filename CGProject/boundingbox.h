#pragma once
#include <glm/glm.hpp>
#define INF 100000
/*
* 用bounding box（可以理解为一个长方体）描述物体的体积，用来碰撞检测。
* 当abs(x.center - y.center)任意一个小于(x.length + y.length)
* 则出现碰撞
* 
* 但是存在问题，暂时废弃
*/
class BoundingBox
{
public:
	glm::vec3 center, length;
	glm::vec3 max_pos, min_pos;
	BoundingBox()
	{
		max_pos = glm::vec3(-INF, -INF, -INF);
		min_pos = glm::vec3(INF, INF, INF);
	}
};