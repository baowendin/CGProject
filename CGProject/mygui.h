#ifndef MYGUI_H
#define MYGUI_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace MyGui {
	void init(GLFWwindow *window);

	void render();

	void end();
};		//namespace MyGui

#endif	//MYGUI_H

