#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH	= 800;
const int SCREEN_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

	//float vertices[] = {
	//	 0.5f,  0.5f, 0.0f,  // top right
	//	 0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
	//};

	//unsigned int indices[] = {
	//	0, 1, 3,	// First triangle
	//	1, 2, 3		// Second triangle
	//};

	// First triangle vertices
	float t1Vertices[] = {
		-0.4f,  0.8f, 0.0f,		// Center top
		-0.8f, -0.8f, 0.0f,		// Bottom left
		 0.0f, -0.8f, 0.0f		// Bottom right
	};
		
	// Second triangle vertices
	float t2Vertices[] = {
		0.0f,  0.8f, 0.0f,		// Top left
		0.8f,  0.8f, 0.0f,		// Top right
		0.4f, -0.8f, 0.0f		// Center bottom
	};

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR. Vertex shader compilation failed.\n %s\n", infoLog);
		return -1;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR. Fragment shader compilation failed.\n %s\n", infoLog);
		return -1;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Error. Shader program linking failed.\n %s\n", infoLog);
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// First triangle
	unsigned int t1VBO, t1VAO;
	glGenBuffers(1, &t1VBO);
	glGenVertexArrays(1, &t1VAO);

	glBindVertexArray(t1VAO);
	glBindBuffer(GL_ARRAY_BUFFER, t1VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t1Vertices), t1Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Second triangle
	unsigned int t2VBO, t2VAO;
	glGenBuffers(1, &t2VBO);
	glGenVertexArrays(1, &t2VAO);

	glBindVertexArray(t2VAO);
	glBindBuffer(GL_ARRAY_BUFFER, t2VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t2Vertices), t2Vertices, GL_STATIC_DRAW);

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(t1VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram);
		glBindVertexArray(t2VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

end:
	printf("Closing Hedrozoid");

	glfwTerminate();

	return 0;
}