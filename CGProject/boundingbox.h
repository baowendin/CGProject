#pragma once
#include <glm/glm.hpp>
#define INF 100000
/*
* ��bounding box���������Ϊһ�������壩��������������������ײ��⡣
* ��abs(x.center - y.center)����һ��С��(x.length + y.length)
* �������ײ
* 
* ���Ǵ������⣬��ʱ����
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