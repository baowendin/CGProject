#pragma once
#include "boundingbox.h"
#include "shader.h"
class Object
{
public:
	virtual void paint() = 0;
	// virtual BoundingBox get_boundingbox() = 0;
};