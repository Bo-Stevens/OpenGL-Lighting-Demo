#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CustomShader.h"
#include "stb_image.h"
#include <iostream>

float deltaTime = 0.0f;
float lastFrame = 0.0f;
double lastX;
double lastY;
const float mouseSensitivity = 0.1f;
double yaw = -90;
double pitch;
bool firstMouseMovement = true;
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 cameraPos;
glm::vec3 cameraForward;
glm::vec3 cameraUp;
glm::vec3 cameraRight;

glm::vec3 lightPos(0.0f, 0.0f, -5.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouseMovement) {
		lastX = xPos;
		lastY = yPos;
		firstMouseMovement = false;
	}

	double xOffset = xPos - lastX;
	double yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;
	yaw += xOffset;
	pitch += yOffset;
	if (pitch > 89.0)
		pitch = 89.0;
	if (pitch < -89.0)
		pitch = -89.0;

	glm::vec3 direction;
	direction.y = sin(glm::radians(pitch));
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraForward = glm::normalize(direction);

	cameraRight = glm::normalize(glm::cross(up, cameraForward));
	cameraUp = glm::cross(cameraForward, cameraRight);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window) {
	float cameraSpeed = 2.5f * deltaTime;
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraForward;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraForward;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos -= glm::normalize(cameraRight) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos += glm::normalize(cameraRight) * cameraSpeed;
	}
}




int loadTexture(const char filename []) {
	unsigned int texture;
	int width, height, nrChannels;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Couldn't load texture " << std::endl;
	}

	stbi_image_free(data);
	return texture;
}

GLFWwindow* window;
int initOpengl() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(1920, 1080, "Learning", NULL, NULL);
	lastX = 960;
	lastY = 540;
	if (window == NULL) {
		std::cout << "We fuckd up" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Fuckd up GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 1920, 1080);
}



float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -5.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -7.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(1.0f,  5.5f, -2.0f),
	glm::vec3(-4.3f,  2.0f, -4.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f),
	glm::vec3(-4.5f,  3.0f, -10.5f),
	glm::vec3(5.3f,  6.5f, -1.5f),
	glm::vec3(-3.3f,  -5.0f, -5.5f),
	glm::vec3(-3.5f,  1.0f, 4.5f)
};

int main() {
	
	//
	//If opengl fails to initialize, return -1
	//
	if (initOpengl() == -1) {
		return -1;
	}

	Shader cubeShader("vShader.txt", "fShader.txt");
	Shader lightCubeShader("LightVShader.txt", "LightFShader.txt");

	//Vertex Array and Buffer
	unsigned int cubeVAO;
	//Vertex Buffer can have overlapping verts, so I'm using an element buffer instead
	unsigned int VBO;
	//unsigned int EBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(cubeVAO);



	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//Texture
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//unsigned int texture = loadTexture("container.jpg");
	//unsigned int texture2 = loadTexture("wall.jpg");
	
	//Unbinding buffer and array
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	

	//
	///Camera bullshit
	//
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
	cameraForward = glm::normalize(cameraPos - cameraTarget);
	cameraRight = glm::normalize(glm::cross(up, cameraForward));
	cameraUp = glm::cross(cameraForward, cameraRight);

	glm::mat4 view;
	
	//
	//Handles moving things from 3D coordinates back into 2D to actually display on the monitor
	//

	//Translates everything to world coordinates
	glm::mat4 model = glm::mat4(1.0f);


	//Projection coordinates (45 degree FOV)
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	//
	///Lighting
	//
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(cameraPos, cameraPos + cameraForward, cameraUp);
	//myShader.setInt("texture", 0);
	//myShader.setInt("texture2", 1);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window)) {

		//Input
		processInput(window);
		//Rendering
		cubeShader.use();
		cubeShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		cubeShader.setVec3("lightColor", lightColor);
		cubeShader.setVec3("lightPos", lightPos);
		cubeShader.setVec3("viewPos", cameraPos);
		//
		//Sets default values 
		//
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		//Binds texture to the currently loaded set of vertices
		//
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(cubeVAO);
		
		//
		//Binds uniforms in my vertex shader to handle 3D to 2D conversions
		//
		int modelLoc = glGetUniformLocation(cubeShader.ID, "model");
		int viewLoc = glGetUniformLocation(cubeShader.ID, "view");
		int projectionLoc = glGetUniformLocation(cubeShader.ID, "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//
		//Draws my vertices
		//
		for (int i = 1; i < 15; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 10.0f + i;
			model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//
		/// Camera movement
		//
		view = glm::lookAt(cameraPos, cameraPos + cameraForward, cameraUp);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		//lightPos.x = 1.0f + sin(glfwGetTime()) * 3.0f;
		//lightPos.z = sin(glfwGetTime() / 2.0f) * 4.0f;
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f));
		lightCubeShader.setMat4("model", model);
		lightCubeShader.setVec3("LightColor", lightColor);
		lightColor.x = 0.5f + sin(glfwGetTime()/4.0f)/2;
		lightColor.y = 0.5f + sin(glfwGetTime()/2.0f)/2;
		lightColor.z = 0.5f + sin(glfwGetTime() / 1.5f) / 2;
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}