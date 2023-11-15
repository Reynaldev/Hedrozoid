#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

const int SCREEN_WIDTH	= 800;
const int SCREEN_HEIGHT = 600;

const char* t2VertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* t2FragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 vertColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vertColor;\n"
	"}\0";

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
}

std::string getFilePath(std::string filename)
{
	std::string path(__FILE__);
	size_t index = path.rfind("\\") + 1;
	std::string filepath = path.substr(0, index) + filename;

	return filepath;
}

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hedrozoid", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window");
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		exit(-1);
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Check the maximum vertex attributes that can be declared
	int nrAttrib;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttrib);
	printf("Maximum number of vertex attributes supported: %d\n", nrAttrib);

	// First triangle vertices
	float t1Vertices[] = {
		// Positions			// Colors
		-0.4f,  0.8f, 0.0f,		1.0f, 0.0f, 0.0f,		// Center top
		-0.8f, -0.8f, 0.0f,		0.0f, 1.0f, 0.0f,		// Bottom left
		 0.0f, -0.8f, 0.0f,		0.0f, 0.0f, 1.0f		// Bottom right
	};
		
	// Second triangle vertices
	float t2Vertices[] = {
		0.0f,  0.8f, 0.0f,		// Top left
		0.8f,  0.8f, 0.0f,		// Top right
		0.4f, -0.8f, 0.0f		// Center bottom
	};

	std::string t1Vertex = getFilePath("FirstTriangleVertex.glsl");
	std::string t1Fragment = getFilePath("FirstTriangleFragment.glsl");

	std::string t2Vertex = getFilePath("SecondTriangleVertex.glsl");
	std::string t2Fragment = getFilePath("SecondTriangleFragment.glsl");

	//Shader t1Shader(t1Vertex, t1Fragment);
	//Shader t2Shader(t2Vertex, t2Fragment);

	Shader t1Shader("FirstTriangleVertex.glsl", "FirstTriangleFragment.glsl");
	Shader t2Shader("SecondTriangleVertex.glsl", "SecondTriangleFragment.glsl");

	// Buffers
	GLuint VBO[2], VAO[2];
	glGenBuffers(2, VBO);
	glGenVertexArrays(2, VAO);

	// First triangle
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t1Vertices), t1Vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	int t1VertColor = glGetUniformLocation(t1Shader.ID, "rotColor");

	// Second triangle
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t2Vertices), t2Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	int t2VertColor = glGetUniformLocation(t2Shader.ID, "color");
	int t2VertPos = glGetUniformLocation(t2Shader.ID, "offset");

	while (!glfwWindowShouldClose(window))
	{
		float timeValue = glfwGetTime();

		float tRed = (cos(timeValue) / 2.0f) + 0.5f;
		float tGreen = (sin(timeValue) / 2.0f) + 0.5f;
		float tBlue = (sin(timeValue + 35.0f) / 2.0f) + 0.5f;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// First triangle
		t1Shader.use();
		glUniform3f(t1VertColor, tRed, tGreen, tBlue);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Second triangle
		float t2YPos = (sin(timeValue) / 3.0f);

		t2Shader.use();
		glUniform4f(t2VertColor, tRed, tGreen, tBlue, 1.0f);
		glUniform3f(t2VertPos, 0.0f, t2YPos, 0.0f);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Log
		/*float sinVal = (sin(timeValue) / 2.0f) + 0.5f;
		float cosVal = (cos(timeValue) / 2.0f) + 0.5f;
		float midVal = (sin(timeValue + 30.0f) / 2.0f) + 0.5f;
		printf("Time: %fs. Sin: %f. Cos: %f. Mid: %f\n", 
			timeValue, sinVal, cosVal, midVal);*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

end:
	printf("Closing Hedrozoid");

	glfwTerminate();

	return 0;
}