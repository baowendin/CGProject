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

extern float lightPosX;
extern float lightPosY;
extern float lightPosZ;
void reload();
void MyGui::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Settings");
	ImGui::Text("Light Position");
	ImGui::SliderFloat("x:", &lightPosX, -100.0f, 100.0f);
	ImGui::SliderFloat("y:", &lightPosY, -0.0f, 100.0f);
	ImGui::SliderFloat("z:", &lightPosZ, -100.0f, 100.0f);
	if (ImGui::Button("Quit"))
		glfwSetWindowShouldClose(window, true);
	if (ImGui::Button("ReloadModel"))
		reload();
	ImGui::End();

	ImGui::Render();
}

void MyGui::end() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

