#include "common.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

#include "model.h"
#include "sphere.h"
#include "skybox.h"
#include "camera.h"

#include "mygui.h"

#include <fstream>
#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <vector>
#include <iostream>




// window size change call back function
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// mouse movement call back function
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// scroll call back function
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// keyboard call back function
void process_input(GLFWwindow* window);
// render scene
void render_scene(Shader& shader, vector<Model*>& collection);
unsigned int init_texture(string path);

//temp for test shadow mapping
void renderQuad();

//load model from json file
void load_model(string path, vector<Model*>& collection);

// window settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

/*
* settings with an FPS camera
* timing setting is used to balance movement among different device
* last time position is used to calculate movement
*/
Camera camera(glm::vec3(10.0f, 7.0f, 10.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// last time position
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool first_mouse = true;
bool enableMouseControl = true;
//light
float lightPosX = 10.0f;
float lightPosY = 20.0f;
float lightPosZ = -20.0f;
glm::vec3 lightPos(lightPosX, lightPosY, lightPosZ);

// gui is displayed
bool displayGui = false;
// status of escape for the last 2 frame
int escapeKeyStatus[2] = {GLFW_RELEASE, GLFW_RELEASE};

//vector to store models
vector<Model*> model_collection;

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGProject", NULL, NULL);
	if (window == NULL)
	{
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	Shader ourShader("Shader/shader_texture.vs", "Shader/shader_texture.fs");
	Shader simpleDepthShader("Shader/depth_shader.vs", "Shader/depth_shader.fs");
	Shader debugDepthQuad("Shader/debug_quad.vs", "Shader/debug_quad.fs");

	// initalize model
	load_model("app.json", model_collection);
	camera.set_collection(&model_collection);
	/*
	Model sky("Model/skybox/skybox.obj");
	Model ground("Model/ground/ground.obj");
	Model house("Model/house/house.obj");
	Model tree("Model/tree_1/tree_1.obj");
	// TODO: add them into a vector which used for collision detect
	model_collection.push_back(&ground);
	model_collection.push_back(&house);
	model_collection.push_back(&tree);
	*/

	// light
	//glm::vec3 lightPos = glm::vec3(10.0f, 20.0f, -20.0f);
	ourShader.use();
	ourShader.setVec3("light.position", lightPos);
	ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
	ourShader.setVec3("light.diffuse", 0.4f, 0.4f, 0.4f);
	ourShader.setVec3("light.specular", 0.1f, 0.1f, 0.1f);

	ourShader.setInt("shadowMap", 1);

	// skybox ������������ҵ�ͼƬ����ˣ����Լ���˳����ܲ���׼
	vector<std::string> faces
	{
		"Texture/skybox/right.jpg",
		"Texture/skybox/left.jpg",
		"Texture/skybox/top.jpg",
		"Texture/skybox/bottom.jpg",
		"Texture/skybox/front.jpg",
		"Texture/skybox/back.jpg"
	};
	Skybox skybox(faces);

	// Ϊ��Ӱ��ͼ����֡����
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// ����2D����
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	// ���û��Ʒ�ʽ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// ������󶨵�FBO
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ����OpenGLѡ��
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);
	unsigned int test_texture = init_texture("Texture/skybox/front.jpg");
	
	//init imgui
	MyGui::init(window);

	// render loop
	// -----------	
	while (!glfwWindowShouldClose(window))
	{
		// time delta 
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		process_input(window);

		MyGui::start();
		if (displayGui)
			MyGui::render();
		lightPos = glm::vec3(lightPosX, lightPosY, lightPosZ);

		// Render depth of scene to a texture
		// clear color and depth buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render depth of scene to texture (from light's perspective)
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0, far_plane = 100;
		lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0.0, 0.0, 1.0));
		lightSpaceMatrix = lightProjection * lightView;
		// send matrix into shader
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		// rendering
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		render_scene(simpleDepthShader, model_collection);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*
		// ����������������Ƿ���ȷ
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		debugDepthQuad.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();
		*/

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.use();
		ourShader.setVec3("light.position", lightPos);
		ourShader.setMat4("view", view);
		// projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		// ������Ӱ��ͼ
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		render_scene(ourShader, model_collection);
		//// ͨ����ʱ�޸�light���ԣ�ʹ�����ǻ���������ղ�����һ�鳤�����ر���
		//ourShader.setBool("shadow_enabled", false);
		//ourShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
		//ourShader.setVec3("light.diffuse", 0.0f, 0.0f, 0.0f);
		//sky.Draw(ourShader);
		//// ����light
		//ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);		
		//ourShader.setVec3("light.diffuse", 0.7f, 0.7f, 0.7f);



		// skybox ����
		skybox.Paint(camera, projection);

		// render 2D Gui last
		if (displayGui)
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	MyGui::end();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow* window)
{
	//old
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, true);

	//	like "always@ (posedge escapeKeyStatus)"
	escapeKeyStatus[1] = escapeKeyStatus[0];
	escapeKeyStatus[0] = glfwGetKey(window, GLFW_KEY_ESCAPE);
	if (escapeKeyStatus[1] == GLFW_RELEASE &&
		escapeKeyStatus[0] == GLFW_PRESS) {
		displayGui = !displayGui;
		enableMouseControl = !enableMouseControl;
		glfwSetInputMode(window, GLFW_CURSOR, enableMouseControl ?
			GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

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
	if (first_mouse)
	{
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	if (enableMouseControl)
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
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
}

unsigned int init_texture(string path)
{
	if (path.size() == 0)
		return -1;
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

void render_scene(Shader& shader, vector<Model*>& collection)
{
	/*
	// glCullFace(GL_FRONT);
	shader.setBool("shadow_enabled", true);
	collection[0]->scale = glm::vec3(10, 10, 10);
	// shader.setMat4("model", glm::scale(glm::mat4(), glm::vec3(10, 10, 10)));
	collection[0]->Draw(shader);
	collection[1]->translate = glm::vec3(0, 4.5, 0);
	// shader.setMat4("model", glm::translate(glm::mat4(), glm::vec3(0, 4.5, 0)));
	collection[1]->Draw(shader);
	collection[2]->translate = glm::vec3(10, 4, 0);
	// shader.setMat4("model", glm::translate(glm::mat4(), glm::vec3(10, 4, 0)));
	collection[2]->Draw(shader);
	// glCullFace(GL_BACK);
	*/
	for (auto& item : collection)
	{
		shader.setBool("shadow_enabled", true);
		item->Draw(shader);
	}
}

// this part is used for testing shadow mapping 

unsigned int quadVAO, quadVBO;

void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void load_model(string path, vector<Model*>& collection)
{
	// ���ļ���ȡJson���ַ���
	ifstream is;
	is.open(path);
	if (!is.is_open())
	{
		cout << "Can't Open App Json File!" << endl;
		return;
	}
	string line;
	string file_string((istreambuf_iterator<char>(is)), istreambuf_iterator<char>());
	// ��ȡ���ò�����Model List

	rapidjson::Document dom;
	if (!dom.Parse(file_string.c_str()).HasParseError())
	{
		if (dom.HasMember("objects") && dom["objects"].IsArray())
		{
			const rapidjson::Value& arr = dom["objects"];
			for (int i = 0; i < arr.Size(); ++i)
			{
				const rapidjson::Value& item = arr[i];
				// ����ļ�·��
				if (!item.HasMember("path"))
				{
					cout << "Item " << i << " doesn't contains path" << endl;
					continue;
				}
				string path = item["path"].GetString();
				Model* model = new Model(path);

				//��ȡ��������
				vector<string> trans = {"scale", "translate", "rotate"};
				for (auto& s : trans)
				{
					if (!item.HasMember(s.c_str()))
						continue;
					
					float arr[3];
					int size;
					// ��ȡvalue
					size = item[s.c_str()].Size();
					if (size != 3)
					{
						cout << s << " size doesn't match" << endl;
					}

					for (int i = 0; i < 3; i++)
					{
						arr[i] = item[s.c_str()][i].GetDouble();
					}
					model->set_attribute(s, glm::vec3(arr[0], arr[1], arr[2]));
				}

				vector<string> float_attributes = {"angle"};
				//��ȡ��ת�Ƕ�
				for (auto attribute : float_attributes)
				{
					if (!item.HasMember(attribute.c_str()))
						continue;
					
					float angle = (float)item[attribute.c_str()].GetDouble();
					model->set_attribute(attribute, angle);
				}

				//�Ƿ������ײ���
				if (item.HasMember("collision"))
				{
					int flag = item["collision"].GetInt();
					if (flag == 0)
						model->handle_collision = false;
					else
						model->handle_collision = true;
				}
				// ����vector
				model->update_boundingbox();
				cout << model->boundingbox.max_pos.x << " " << model->boundingbox.max_pos.y << " " << model->boundingbox.max_pos.z << " " << endl;
				cout << model->boundingbox.min_pos.x << " " << model->boundingbox.min_pos.y << " " << model->boundingbox.min_pos.z << " " << endl;
				collection.push_back(model);
			}
		}

	}
}
