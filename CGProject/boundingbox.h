#pragma once
#include <glm/glm.hpp>

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
	BoundingBox(glm::vec3 center, glm::vec3 length)
	{

	}
};