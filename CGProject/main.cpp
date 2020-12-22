#include "common.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include "model.h"
#include "sphere.h"
#include <iostream>
#include <filesystem>
#include "camera.h"
#include <iostream>
#include "skybox.h"

// window size change call back function
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// mouse movement call back function
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// scroll call back function
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// keyboard call back function
void processInput(GLFWwindow* window);
unsigned int init_texture(string path);

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/*
* settings with an FPS camera 
* timing setting is used to balance movement among different device
* last time position is used to calculate movement 
*/ 
Camera camera(glm::vec3(0.0f, 4.0f, 0.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// last time position
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGAssignment#8", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	Shader ourShader("Shader/shader_texture.vs", "Shader/shader_texture.fs");

	// Initalize model
	Model ourModel("Model/small_tree/unityexport.obj");
	Model sky("Model/skybox/skybox.obj");
	Model ground("Model/ground/ground.obj");
	Model house("Model/house/house.obj");
	// TODO: add them into a vector which used for collision detect

	// Light
	glm::vec3 lightPos = glm::vec3(0.0f, 50.0f, 0.0f);
	ourShader.use();
	ourShader.setVec3("light.position", lightPos);
	ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
	ourShader.setVec3("light.diffuse", 0.4f, 0.4f, 0.4f); 
	ourShader.setVec3("light.specular", 0.1f, 0.1f, 0.1f);
	ourShader.setInt("texture1", 0);

	//Skybox 这里可能是我找的图片标错了，所以加载顺序可能不标准
	vector<std::string> faces
	{
		"Texture/skybox/front.jpg",
		"Texture/skybox/back.jpg",
		"Texture/skybox/top.jpg",
		"Texture/skybox/bottom.jpg",
		"Texture/skybox/right.jpg",
		"Texture/skybox/left.jpg"
	};
	Skybox skybox(faces); 

	//rotate speed;
	int time = 0;
	float earth_rs = 20, mars_rs = 10, moon_rs = 30;
	glEnable(GL_DEPTH_TEST);
	// render loop
	// -----------	
	while (!glfwWindowShouldClose(window)) {
		// time delta 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// clear color and depth buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render container
		ourShader.use();
		// view matrix
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("model", glm::mat4());

		// 通过临时修改light属性，使得我们画出来的天空不会有一块长方形特别亮
		ourShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
		ourShader.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
		// sky.Draw(ourShader);	
		// 调回light
		ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);

		// 其他object绘制
		ourShader.setMat4("model", glm::scale(glm::mat4(), glm::vec3(10, 10, 10)));
		ground.Draw(ourShader);
		ourShader.setMat4("model", glm::translate(glm::mat4(), glm::vec3(0, 5, 0)));
		house.Draw(ourShader);

		// skybox 绘制
		skybox.Paint(camera, projection);
		glfwSwapBuffers(window);
		glfwPollEvents();
		time++;
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
	}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (first_mouse) {
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

unsigned int init_texture(string path)
{
	if (path.size() == 0)
		return -1;
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}