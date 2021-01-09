#include "common.h"
#include "mygui.h"

static GLFWwindow *window;
void glfwSetWindowShouldClose(GLFWwindow*, int);

void MyGui::init(GLFWwindow *_window) {
	window = _window;
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("Fonts/consola.ttf", 16.0f);
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void MyGui::start() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

extern float lightPosX;
extern float lightPosY;
extern float lightPosZ;
void MyGui::render() {
	ImGui::Begin("Settings");
	ImGui::Text("Light Position");
	ImGui::SliderFloat("x:", &lightPosX, -20.0f, 20.0f);
	ImGui::SliderFloat("y:", &lightPosY, -20.0f, 20.0f);
	ImGui::SliderFloat("z:", &lightPosZ, -20.0f, 20.0f);
	if (ImGui::Button("Quit"))
		glfwSetWindowShouldClose(window, true);
	ImGui::End();

	ImGui::Render();
}

void MyGui::end() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

