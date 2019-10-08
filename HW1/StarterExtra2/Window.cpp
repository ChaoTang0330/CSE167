#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
Cube * Window::cube;
PointCloud * Window::cubePoints;
//Modify: 9/29
PointCloud* Window::bunnyPoints;
PointCloud* Window::dragonPoints;
PointCloud* Window::bearPoints;
GLfloat Window::pointsSize(1);//control point size
//float Window::winLength(0);
int Window::shiftFlag(0);


// The object currently displaying.
Object * Window::currentObj;

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
GLuint Window::colorLoc; // Location of color in shader.

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	cube = new Cube(5.0f);
	// Create a point cloud consisting of cube vertices.
	cubePoints = new PointCloud("foo", 100);

	//Modify: Create point clouds for three objects, 9/29
	bunnyPoints = new PointCloud("bunny.obj", pointsSize);
	std::cout << "Finish loading  bunny.obj" << std::endl;
	dragonPoints = new PointCloud("dragon.obj", pointsSize);
	std::cout << "Finish loading  dragon.obj" << std::endl;
	bearPoints = new PointCloud("bear.obj", pointsSize);
	std::cout << "Finish loading bear.obj" << std::endl;

	// Set cube to be the first to display
	currentObj = cube;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete cubePoints;

	// Delete the shader program.
	glDeleteProgram(program);
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

	//Add scale, 9/30
	
	//Window::winLength = (float) (width < height) ? width : height;
	/*
	if (currentObj != NULL)
	{
		if (currentObj == bunnyPoints)
		{
			std::cout << "Scale Bunny, ";
			bunnyPoints->objScacle(winLength);
		}
		else if (currentObj == dragonPoints)
		{
			std::cout << "Scale Dragon, ";
			dragonPoints->objScacle(winLength);
		}
		else if (currentObj == bearPoints)
		{
			std::cout << "Scale Bear, ";
			bearPoints->objScacle(winLength);
		}
	}
	*/
	
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	currentObj->update(); //comment: spin 9/29
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
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

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
		case GLFW_KEY_1:
			// Set currentObj to cube
			currentObj = cube;
			break;
		case GLFW_KEY_2:
			// Set currentObj to cubePoints
			currentObj = cubePoints;
			break;

		/* *** Update 9/29 *** */
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			shiftFlag = 1;
			break;
		case GLFW_KEY_F1:
			// Set currentObj to bunny
			if (currentObj == dragonPoints)
			{
				bunnyPoints->trans(dragonPoints->objPoints);
			}
			else if (currentObj == bearPoints)
			{
				bunnyPoints->trans(bearPoints->objPoints);
			}
			currentObj = bunnyPoints;
			pointsSize = 1;
			//bunnyPoints->objScacle(Window::winLength);
			break;
		case GLFW_KEY_F2:
			// Set currentObj to dragonPoints
			if (currentObj == bunnyPoints)
			{
				dragonPoints->trans(bunnyPoints->objPoints);
			}
			else if (currentObj == bearPoints)
			{
				dragonPoints->trans(bearPoints->objPoints);
			}
			currentObj = dragonPoints;
			pointsSize = 1;
			//dragonPoints->objScacle(Window::winLength);
			break;
		case GLFW_KEY_F3:
			// Set currentObj to bearPoints
			if (currentObj == bunnyPoints)
			{
				bearPoints->trans(bunnyPoints->objPoints);
			}
			else if (currentObj == dragonPoints)
			{
				bearPoints->trans(dragonPoints->objPoints);
			}
			currentObj = bearPoints;
			pointsSize = 1;
			//bearPoints->objScacle(Window::winLength);
			break;
		case GLFW_KEY_P:
			// Larger point with shift
			if (shiftFlag == 1)
			{
				if (currentObj == bunnyPoints)
				{
					bunnyPoints->updatePointSize(++pointsSize);
					currentObj = bunnyPoints;
				}
				else if (currentObj == dragonPoints)
				{
					dragonPoints->updatePointSize(++pointsSize);
					currentObj = dragonPoints;
				}
				else if (currentObj == bearPoints)
				{
					bearPoints->updatePointSize(++pointsSize);
					currentObj = bearPoints;
				}
				shiftFlag = 0;
			}
			//break;
		//case GLFW_KEY_O:
			// Smaller point
			else if (pointsSize > 1)
			{
				if (currentObj == bunnyPoints)
				{
					bunnyPoints->updatePointSize(--pointsSize);
					currentObj = bunnyPoints;
				}
				else if (currentObj == dragonPoints)
				{
					dragonPoints->updatePointSize(--pointsSize);
					currentObj = dragonPoints;
				}
				else if (currentObj == bearPoints)
				{
					bearPoints->updatePointSize(--pointsSize);
					currentObj = bearPoints;
				}
				break;
			}
			
		/* *** Update 9/29 *** */

		default:
		break;
		}
	}
}
