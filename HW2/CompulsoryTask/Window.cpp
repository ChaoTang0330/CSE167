#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
//Cube * Window::cube;
//Modify: 9/29
PointCloud* Window::bunnyPoints;
PointCloud* Window::dragonPoints;
PointCloud* Window::bearPoints;
GLfloat Window::pointsSize(5);//control point size
//float Window::winLength(0);
int Window::mouseLeftButtonFlag(0);
glm::vec3 Window::cursorPrevPos;


// The object currently displaying.
PointCloud * Window::currentObj;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
//GLuint Window::colorLoc; // Location of color in shader.

GLuint Window::colorTypeLoc, Window::viewPosLoc;
GLuint Window::materialAmbientLoc, Window::materialDiffuseLoc;
GLuint Window::materialSpecularLoc, Window::materialShininessLoc;
GLuint Window::lightPositionLoc, Window::lightAmbientLoc;
GLuint Window::lightDiffuseLoc, Window::lightSpecularLoc, Window::lightAttenuationParaLoc;

Material Window::material;
Light Window::light;

GLint Window::colorType(1);
GLint Window::modelRotateEnable(1), Window::lightRotateEnable(0);

//lightModel
PointCloud* Window::lightObj;
GLuint Window::programLight, Window::projectionLocLight, Window::viewLocLight, Window::modelLocLight, Window::colorLocLight;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	programLight = LoadShaders("shaders/shaderLight.vert", "shaders/shaderLight.frag");
	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!programLight)
	{
		std::cerr << "Failed to initialize shaderLight program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");

	materialAmbientLoc = glGetUniformLocation(program, "material.ambient");
	materialDiffuseLoc = glGetUniformLocation(program, "material.diffuseLoc");
	materialSpecularLoc = glGetUniformLocation(program, "material.specular");
	materialShininessLoc = glGetUniformLocation(program, "material.shininess");

	lightPositionLoc = glGetUniformLocation(program, "light.position");
	lightAmbientLoc = glGetUniformLocation(program, "light.ambient");
	lightDiffuseLoc = glGetUniformLocation(program, "light.diffuse");
	lightSpecularLoc = glGetUniformLocation(program, "light.specular");
	lightAttenuationParaLoc = glGetUniformLocation(program, "light.attenuationPara");

	colorTypeLoc = glGetUniformLocation(program, "colorType");
	viewPosLoc = glGetUniformLocation(program, "viewPos");

	material.ambient = glm::vec3(0.1745, 0.01175, 0.01175);
	material.diffuse = glm::vec3(0.0, 0.0, 0.0);//(0.61424, 0.04136, 0.04136);
	material.specular = glm::vec3(0.727811, 0.626959, 0.626959);
	material.shininess = 8.0;
	light.position = glm::vec3(5.0, 5.0, 5.0);
	light.ambient = glm::vec3(0.6, 0.6, 0.6);
	light.diffuse = glm::vec3(0.9, 0.9, 0.9);
	light.specular = glm::vec3(1.0, 1.0, 1.0);
	light.attenuationPara = 0.2;


	//light shader
	glUseProgram(programLight);
	projectionLocLight = glGetUniformLocation(programLight, "projection");
	viewLocLight = glGetUniformLocation(programLight, "view");
	modelLocLight = glGetUniformLocation(programLight, "model");
	colorLocLight = glGetUniformLocation(programLight, "color");
	glUseProgram(program);

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	//cube = new Cube(5.0f);

	//Modify: Create point clouds for three objects, 9/29
	bunnyPoints = new PointCloud("bunny.obj", pointsSize);
	std::cout << "Finish loading bunny.obj" << std::endl;
	dragonPoints = new PointCloud("dragon.obj", pointsSize);
	std::cout << "Finish loading dragon.obj" << std::endl;
	bearPoints = new PointCloud("bear.obj", pointsSize);
	std::cout << "Finish loading bear.obj" << std::endl;

	
	// Set bunny to be the first to display
	currentObj = bunnyPoints;

	glUseProgram(programLight);
	lightObj = new PointCloud("sphere.obj", pointsSize);
	std::cout << "Finish loading sphere.obj" << std::endl;
	lightObj->scaleModel(0.06);
	lightObj->translateModel(light.position);
	glUseProgram(program);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete bunnyPoints, dragonPoints, bearPoints, lightObj;

	// Delete the shader program.
	glDeleteProgram(program);
	glDeleteProgram(programLight);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	/*
	std::cout << "resizeCallBack" << std::endl;
	std::cout << "width = " << width;
	std::cout << ", height = " << height << std::endl;
	*/
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);

}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	//currentObj->update(); //comment: spin 9/29
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Specify the values of the uniform variables we are going to use.
	glm::mat4 model = currentObj->getModel();
	glm::vec3 color = currentObj->getColor();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	//void glUniformMatrix4fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat * value);
	//void glUniform3fv(GLint location,GLsizei count,const GLfloat * value);
	//void glUniform1i(GLint location,GLint v0);
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(eye));

	//Change value
	//GLuint colorTypeLoc;
	//GLuint materialAmbientLoc, materialDiffuseLoc, materialSpecularLoc, materialShininessLoc;
	//GLuint lightPositionLoc, lightAmbientLoc, lightDiffuseLoc, lightSpecularLoc, lightAttenuationParaLoc;
	glUniform1i(colorTypeLoc, colorType);
	glUniform3fv(materialAmbientLoc, 1, glm::value_ptr(material.ambient));
	glUniform3fv(materialDiffuseLoc, 1, glm::value_ptr(material.diffuse));
	glUniform3fv(materialSpecularLoc, 1, glm::value_ptr(material.specular));
	glUniform1f(materialShininessLoc, material.shininess);
	glUniform3fv(lightPositionLoc, 1, glm::value_ptr(light.position));
	glUniform3fv(lightAmbientLoc, 1, glm::value_ptr(light.ambient));
	glUniform3fv(lightDiffuseLoc, 1, glm::value_ptr(light.diffuse));
	glUniform3fv(lightSpecularLoc, 1, glm::value_ptr(light.specular));
	glUniform1f(lightAttenuationParaLoc, light.attenuationPara);


	glUseProgram(programLight);
	glm::mat4 modelLight = lightObj->getModel();
	glUniformMatrix4fv(projectionLocLight, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLocLight, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLocLight, 1, GL_FALSE, glm::value_ptr(modelLight));
	glUniform3fv(colorLocLight, 1, glm::value_ptr(light.specular));
	//lightObj->translateModel(light.position - lightObj->getModelCenter());
	lightObj->draw();
	glUseProgram(program);


	// Render the object.
	currentObj->draw();

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_F1:
			// Set currentObj to bunny
			currentObj = bunnyPoints;
			material.ambient = glm::vec3(0.1745, 0.01175, 0.01175);
			material.diffuse = glm::vec3(0.0, 0.0, 0.0);
			material.specular = glm::vec3(0.727811, 0.626959, 0.626959);
			material.shininess = 8.0;
			break;
		case GLFW_KEY_F2:
			// Set currentObj to dragonPoints
			currentObj = dragonPoints;
			material.ambient = glm::vec3(0.1745, 0.01175, 0.01175);
			material.diffuse = glm::vec3(0.0, 0.0, 0.0);
			material.specular = glm::vec3(0.727811, 0.626959, 0.626959);
			material.shininess = 8.0;
			break;
		case GLFW_KEY_F3:
			// Set currentObj to bearPoints
			currentObj = bearPoints;
			material.ambient = glm::vec3(0.1745, 0.01175, 0.01175);
			material.diffuse = glm::vec3(0.0, 0.0, 0.0);
			material.specular = glm::vec3(0.727811, 0.626959, 0.626959);
			material.shininess = 8.0;
			break;

		case GLFW_KEY_N:
			if (colorType == 1)
			{
				colorType = 2;
			}
			else
			{
				colorType = 1;
			}
			break;

		case GLFW_KEY_1:
			modelRotateEnable = 1;
			lightRotateEnable = 0;
			break;

		case GLFW_KEY_2:
			modelRotateEnable = 0;
			lightRotateEnable = 1;
			break;

		case GLFW_KEY_3:
			modelRotateEnable = 1;
			lightRotateEnable = 1;
			break;

		default:
		break;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	GLfloat  factor = pow(1.1, yOffset);
	if (modelRotateEnable == 1)
	{
		currentObj->scaleModel(factor);
	}
	if (lightRotateEnable == 1)
	{
		glm::vec3 currentPosition = lightObj->getModelCenter();
		glm::vec3 destination = factor * currentPosition;
		lightObj->translateModel(destination-currentPosition);
		light.position = lightObj->getModelCenter();

	}
	
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mouseLeftButtonFlag = 1;
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			cursorPrevPos = cursorPositionMapping(xpos, ypos);
		}
		else if (action == GLFW_RELEASE)
		{
			mouseLeftButtonFlag = 0;
			
		}
	}
}

void Window::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	
	if (mouseLeftButtonFlag == 1)
	{
		glm::vec3 v;
		v = cursorPositionMapping(xPos, yPos);
		if (modelRotateEnable == 1)
		{
			currentObj->rotateModel(cursorPrevPos, v);
		}
		if (lightRotateEnable == 1)
		{
			lightObj->rotateModel(cursorPrevPos, v);
			light.position = lightObj->getModelCenter();
		}
		cursorPrevPos = v;
	}
}

glm::vec3 Window::cursorPositionMapping(double xPos, double yPos)
{
	glm::vec3 v;

	xPos = (xPos < 640.0f) ? xPos : 640.f;
	xPos = (xPos > 0.0f) ? xPos : 0.0f;
	xPos = (xPos < 480.0f) ? xPos : 480.f;
	xPos = (xPos > 0.0f) ? xPos : 0.0f;

	//v.x = (xPos - 640.0f / 2) / 400.0f; // sqrt(320^2 + 240^2)
	//v.y = (480.0f / 2 - yPos) / 400.0f;
	v.x = (xPos - 320.0f) / 320.0f;
	v.y = (240.0f - yPos) / 240.0f;

	//std::cout << "v.x = " << v.x << ", v.y = " << v.y << std::endl;

	v.z = sqrt(1.0f - v.x * v.x - v.y * v.y);

	v = glm::normalize(v);
	return v;
}