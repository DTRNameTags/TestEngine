#include <iostream>
#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* fileName);


int main() {
	std::cout << "Hello, World!" << std::endl;

	int success;
	char infoLog[512];

	// Initialize GLFW
	glfwInit();

	// Set GLFW options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Set OpenGL profile to core
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
# ifdef __APPLE__
	// Set OpenGL forward compatibility for macOS porlly wont be used
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
# endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "Test Engine", NULL, NULL);
	// Check if window was created successfully
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD to load OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Shaders

	// compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSrc = loadShaderSrc("assets/vertex_core.glsl");
	const GLchar* vertShader = vertexShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glCompileShader(vertexShader);

	// catch stupid errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// compile fragment shader
	unsigned int fragmentShader[2];
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
	const GLchar* fragShader = fragmentShaderSrc.c_str();
	glShaderSource(fragmentShader[0], 1, &fragShader, NULL);
	glCompileShader(fragmentShader[0]);

	// catch more stupid errors
	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaderSrc = loadShaderSrc("assets/fragment_core2.glsl");
	fragShader = fragmentShaderSrc.c_str();
	glShaderSource(fragmentShader[1], 1, &fragShader, NULL);
	glCompileShader(fragmentShader[1]);

	// catch more stupid errors
	glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// create shader program
	unsigned int shaderProgram[2];
	shaderProgram[0] = glCreateProgram();

	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);

	// catch errors
	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	shaderProgram[1] = glCreateProgram();

	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);

	// catch errors
	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);

	// vertex array
	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f, // Top Right
	//	-0.5, 0.5, 0.0f, // Top Left
	//	-0.5f, -0.5f, 0.0f, // Bottom Left
	//	0.5f, -0.5f, 0.0f, // Bottom Right
	//};

	//unsigned int indicies[] = {
	//	0, 1, 2, // First Triangle
	//	2, 3, 0  // Second Triangle
	//};

	float vertices[] = {
		// First Triangle
		-0.5, -0.5f, 0.0f, 
		-0.25f, 0.5f, 0.0f,
		-0.1f, -0.5f, 0.0f,

		// Second Triangle
		0.5f, -0.5f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.1f, -0.5f, 0.0f
	};
	unsigned int indicies[] = {
		0, 1, 2,
		3, 4, 5
	};

	// VAO, VBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBI
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// setup EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	
	while (!glfwWindowShouldClose(window)) {
		// Process input
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw shapes
		glBindVertexArray(VAO);

		// First Triangle
		glUseProgram(shaderProgram[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// Second Triangle
		glUseProgram(shaderProgram[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));


		// send new fram to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// Adjust the viewport when the window is resized bc it wont bc bad lol
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	// Check if the esc key is pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShaderSrc(const char* fileName) {
	std::ifstream file(fileName);
	std::stringstream buffer;
	
	std::string ret = "";

	file.open(fileName);

	if (file.is_open()) {
		buffer << file.rdbuf();
		ret = buffer.str();
		file.close();
	} else {
		std::cout << "Failed to open shader file: " << fileName << std::endl;
	}

	file.close();

	return ret;
}