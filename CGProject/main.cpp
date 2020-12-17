#include "common.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"
#include "model.h"
#include "cylinder.h"
#include "circle.h"
#include "box.h"
#include "sphere.h"
#include "rectangle.h"
#include <iostream>
#include <filesystem>

#include <iostream>
constexpr auto EPSILON = 0.01;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int init_texture(string path);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Cylinder cylinder;
Circle circle, car_light;
Box box;
Rectangle glass;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 70.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// -----------------------------------	
	Shader ourShader("shader.vs", "shader.fs");
	Model ourModel("Resources/car/Low-Poly-Racing-Car.obj");
	unsigned int earth_texture = init_texture("Texture/earth.jpg");

	// Sun
	Material material;
	material.ka = glm::vec4(1, 1, 1, 1);
	material.kd = glm::vec4(2 * 247.0f / 255, 2 * 92.0f / 255, 2 * 47.0f / 255, 1);
	material.ks = glm::vec4(0.5, 0.5, 0.5, 1);
	material.shinness = 32;
	Sphere sun(glm::vec3(0, 0, 0), 10, material, earth_texture);

	// Earth
	material.ka = glm::vec4(0.4, 0.4, 0.4, 1);
	material.kd = glm::vec4(129.0f / 255, 199.0f / 255, 212.0f / 255, 1);
	material.ks = glm::vec4(0.5, 0.5, 0.5, 1);
	material.shinness = 32;
	Sphere earth(glm::vec3(0, 0, 0), 3, material);

	// Moon
	material.ka = glm::vec4(0.4, 0.4, 0.4, 1);
	material.kd = glm::vec4(186.0f / 255, 145.0f / 255, 50.0f / 255, 1);
	material.ks = glm::vec4(0.5, 0.5, 0.5, 1);
	material.shinness = 32;
	Sphere moon(glm::vec3(0, 0, 0), 1, material);
	// Mars
	material.ka = glm::vec4(0.4, 0.4, 0.4, 1);
	material.kd = glm::vec4(135.0f / 255, 102.0f / 255, 51.0f / 255, 1);
	material.ks = glm::vec4(0.5, 0.5, 0.5, 1);
	material.shinness = 32;
	Sphere mars(glm::vec3(0, 0, 0), 5, material);

	// Light
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
	ourShader.setVec3("light.position", lightPos);
	ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
	ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	ourShader.setVec3("light.specular", 0.1f, 0.1f, 0.1f);

	//rotate speed;
	int time = 0;
	float earth_rs = 20, mars_rs = 10, moon_rs = 30;
	glEnable(GL_DEPTH_TEST);
	// render loop
	// -----------	
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----	
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind Texture
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		
		// render container
		ourShader.use();	
		ourShader.setVec3("viewPos", cameraPos);
		
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH * 1.0 / SCR_HEIGHT), 0.1f, 100.0f);
		ourShader.setMatrix4f("view", view);
		ourShader.setMatrix4f("projection", projection);	
		// paint soloar system
		glm::mat4 model;	
		ourShader.setMatrix4f("model", model);
		sun.paint();
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(earth_rs), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(23, 0, 0));				
		ourShader.setMatrix4f("model", model);
		earth.paint();
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(moon_rs), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(5, 0, 0));
		ourShader.setMatrix4f("model", model);
		moon.paint();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(mars_rs), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(0, 35, 0));
		ourShader.setMatrix4f("model", model);
		mars.paint();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		time++;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------


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
	float cameraSpeed = 0.1f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraUp;
}

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
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
		return;
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
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}