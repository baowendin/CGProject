#include "common.h"
#include "mygui.h"

static GLFWwindow *window;
static float sumTime = 0.0f;
static float deltaTimeDisplay = 0.0f;

extern float deltaTime;
extern float lightPosX;
extern float lightPosY;
extern float lightPosZ;

void reload();
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

void MyGui::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0), 0, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(320, 240));
	ImGui::SetNextWindowBgAlpha(0.33);

	sumTime += deltaTime;
	if (sumTime >= 1.0f) {		//update FPS every second
		sumTime = 0.0f;
		deltaTimeDisplay = deltaTime;
	}

	ImGui::Begin("Settings");
	ImGui::Text("FPS: %.1f", 1.0f / deltaTimeDisplay);
	ImGui::Text("Light Position");
	ImGui::SliderFloat("x", &lightPosX, -100.0f, 100.0f);
	ImGui::SliderFloat("y", &lightPosY, -0.0f, 100.0f);
	ImGui::SliderFloat("z", &lightPosZ, -100.0f, 100.0f);
	if (ImGui::Button("Quit"))
		glfwSetWindowShouldClose(window, true);
#ifdef _DEBUG
	if (ImGui::Button("ReloadModel"))
		reload();
#endif // _DEBUG
	ImGui::End();

	ImGui::Render();
}

void MyGui::end() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

