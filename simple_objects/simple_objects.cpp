// simple_objects.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaider.h"
#include "camera.h"
#include "model.h"
#include "asteroids_arr.h"
#include "skybox.h"

#include <iostream>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
bool injump = false;
float pos_y = 0.f;
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubemap(vector<std::string> faces);

Camera camera(glm::vec3(350.0f, 80.0f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, " OPG"  , NULL, NULL); // создание окна 
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	
	
	Shader ourShader("array_shaider.vs", "fragm_shaider.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader asteroidShader("asteroids.vs", "asteroids.fs");
	
	Model ourModel("rock/SP.obj");
	Model rock("rock/R1.obj");
	Model rock2("rock/R2.obj");
	Model rock3("rock/R3.obj");
	Model rock4("rock/R4.obj");
	Skybox skybox;

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	asteroid ST(rock, 8000, 130, 8);
	asteroid ST2(rock2, 20000, 160, 12);
	asteroid ST3(rock3, 40000, 185, 25);
	asteroid ST4(rock4, 20000, 230, 18);
	//asteroid ST5(rock, 6000, 210, 8);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.f, 1.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		model = glm::scale(model, glm::vec3(90.f,90.f,90.f));
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 viewPos = glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z);
		glm::vec3 lightColor = glm::vec3(0.65f, 0.65f, 0.65f);
		glm::vec3 lightPos = glm::vec3(500.f, 0.f, 0.f);
		
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.5f, 0.2f));

		asteroidShader.use();
		asteroidShader.setVec3("viewPos", viewPos);
		asteroidShader.setVec3("lightPos", lightPos);
		asteroidShader.setMat4("view", view);
		asteroidShader.setMat4("projection", projection);
		asteroidShader.setVec3("lightColor", lightColor);
		
		ourShader.use();
		ourShader.setVec3("viewPos", viewPos);
		ourShader.setVec3("lightPos", lightPos);
		ourShader.setMat4("model", model);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);
		ourShader.setVec3("lightColor", lightColor);
		
		ourModel.Draw(ourShader);

		glm::mat4 modell = glm::mat4(1.0f);
		/*modell = glm::translate(modell, glm::vec3(0.f, 0.f, (float)glfwGetTime() * 2));
		modell = glm::rotate(modell, glm::radians(-(float)glfwGetTime() / 100), glm::vec3(0.f, 1.f, 0.f));*/

		asteroidShader.use();
		asteroidShader.setMat4("model", modell);
		asteroidShader.setInt("texture_diffuse1", 0);


		ST.Draw(rock);
		ST2.Draw(rock2);
		ST3.Draw(rock3);
		ST4.Draw(rock4);
		//ST5.Draw(rock);

		glDepthFunc(GL_LEQUAL);  

		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); 

		skyboxShader.use();
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		skybox.Draw_skybox();
		glDepthFunc(GL_LESS);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	

	glfwTerminate();
	return 0;

}

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
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
