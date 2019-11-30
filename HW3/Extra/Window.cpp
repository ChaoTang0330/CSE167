#include "Window.h"

int Window::width;
int Window::height;

int Window::cullingEnable(0);
int Window::cullingRefreshFlag(0);

const char* Window::windowTitle = "GLFW Project";

// Objects to display.

int Window::mouseLeftButtonFlag(0);
glm::vec3 Window::cursorPrevPos;


// The object currently displaying.
Transform *Window::currentRot, *Window::Army;
Transform *Window::robot, *Window::leftLeg2Robot, *Window::rightLeg2Robot, *Window::leftArm2Robot, *Window::rightArm2Robot;
Transform *Window::head2Robot, *Window::leftEye2Head, *Window::rightEye2Head, *Window::leftAntenna2Head, *Window::rightAntenna2Head;
Geometry *Window::tempGeo;
Sphere *Window::sphere;
glm::mat4 Window::projection; // Projection matrix.


GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::program; // The shader program id.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view;
GLuint Window::viewLoc; // Location of view in shader.

Frustum *Window::frustrum;
glm::vec3  Window::eye, Window::center, Window::up;
float  Window::FOV, Window::nearP, Window::farP;
int Window::cullingDemoMode(0);

/* move to geometry

GLuint Window::projectionLoc; // Location of projection in shader.

GLuint Window::modelLoc; // Location of model in shader.
//GLuint Window::colorLoc; // Location of color in shader.
*/

//GLuint Window::colorTypeLoc, Window::viewPosLoc;
//GLuint Window::materialAmbientLoc, Window::materialDiffuseLoc;
//GLuint Window::materialSpecularLoc, Window::materialShininessLoc;
//GLuint Window::lightPositionLoc, Window::lightAmbientLoc;
//GLuint Window::lightDiffuseLoc, Window::lightSpecularLoc, Window::lightAttenuationParaLoc;

//Material Window::material;
//Light Window::light;

//GLint Window::colorType(1);
GLint Window::modelRotateEnable(1), Window::lightRotateEnable(0);

//lightModel
//PointCloud* Window::lightObj;
//GLuint Window::programLight, Window::projectionLocLight, Window::viewLocLight, Window::modelLocLight, Window::colorLocLight;

bool Window::initializeProgram() {
	
	frustrum = new Frustum();
	eye = glm::vec3(0, 10, 45); // Camera position.
	center = glm::vec3(0, 0, 0); // The point we are looking at.
	up = glm::vec3(0, 1, 0); // The up direction of the camera.
	FOV = 60.0;
	nearP = 1.0;
	farP = 1000.0;
	// View matrix, defined by eye, center and up.
	view = glm::lookAt(eye, center, up);

	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	//programLight = LoadShaders("shaders/shaderLight.vert", "shaders/shaderLight.frag");
	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	/*
	if (!programLight)
	{
		std::cerr << "Failed to initialize shaderLight program" << std::endl;
		return false;
	}
	*/

	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	/* move to geometry
	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	*/

	//material & light
	/*
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

	material.ambient = glm::vec3(0.19225, 0.19225, 0.19225);
	material.diffuse = glm::vec3(0.0, 0.0, 0.0);//(0.61424, 0.04136, 0.04136);
	material.specular = glm::vec3(0.508273,0.508273,0.508273);
	material.shininess = 8.0;
	light.position = glm::vec3(5.0, 5.0, 5.0);
	light.ambient = glm::vec3(0.6, 0.2, 0.2);
	light.diffuse = glm::vec3(0.9, 0.5, 0.5);
	light.specular = glm::vec3(1.0, 0.6, 0.6);
	light.attenuationPara = 0.025;
	*/


	//light shader
	/*
	glUseProgram(programLight);
	projectionLocLight = glGetUniformLocation(programLight, "projection");
	viewLocLight = glGetUniformLocation(programLight, "view");
	modelLocLight = glGetUniformLocation(programLight, "model");
	colorLocLight = glGetUniformLocation(programLight, "color");
	glUseProgram(program);
	*/

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	//cube = new Cube(5.0f);

	//Modify: Create point clouds for three objects, 9/29
	//Transform* Window::robot, Window::leftLeg2Robot, Window::rightLeg2Robot, Window::leftArm2Robot, Window::rightArm2Robot;
	//Transform* Window::head2Robot, Window::leftEye2Head, Window::rightEye2Head, Window::leftAntenna2Head, Window::rightAntenna2Head;
	glm::mat4 R, T, S;

	robot = new Transform(glm::mat4(1), 0, 0, 0);
	robot->setFrustrum(frustrum);
	tempGeo = new Geometry("body_s.obj");
	tempGeo->setShaderProID(program);
	robot->addChild(tempGeo);

	
	T = glm::translate(glm::vec3(-1.2, 0, 0));
	leftArm2Robot = new Transform(T, 1, 0, 0);
	T = glm::translate(glm::vec3(1.2, 0, 0));
	rightArm2Robot = new Transform(T, 2, 0, 0);
	T = glm::translate(glm::vec3(-0.5, -1.5, 0));
	leftLeg2Robot = new Transform(T, 3, 0, 0);
	T = glm::translate(glm::vec3(0.5, -1.5, 0));
	rightLeg2Robot = new Transform(T, 4, 0, 0);

	T = glm::translate(glm::vec3(0,1.6,0));
	head2Robot = new Transform(T, 0, 0, 0);

	T = glm::translate(glm::vec3(-0.4, 0, 0.75));
	leftEye2Head = new Transform(T, 0, 0, 0);
	T = glm::translate(glm::vec3(0.4, 0, 0.75));
	rightEye2Head = new Transform(T, 0, 0, 0);

	R = glm::rotate(0.2f,glm::vec3(0,0,1));
	S = glm::scale(glm::vec3(0.35, 0.1, 0.35));
	T = glm::translate(glm::vec3(-0.5, 0.7, 0));
	leftAntenna2Head = new Transform(T*S*R, 0, 0, 0);
	R = glm::rotate(-0.2f, glm::vec3(0, 0, 1));
	T = glm::translate(glm::vec3(0.5, 0.7, 0));
	rightAntenna2Head = new Transform(T*S*R, 0, 0, 0);

	
	robot->addChild(leftLeg2Robot);
	robot->addChild(rightLeg2Robot);
	robot->addChild(leftArm2Robot);
	robot->addChild(rightArm2Robot);
	robot->addChild(head2Robot);

	tempGeo = new Geometry("limb_s.obj");
	leftLeg2Robot->addChild(tempGeo);
	rightLeg2Robot->addChild(tempGeo);
	leftArm2Robot->addChild(tempGeo);
	rightArm2Robot->addChild(tempGeo);
	head2Robot->addChild(new Geometry("head_s.obj"));

	head2Robot->addChild(leftEye2Head);
	head2Robot->addChild(rightEye2Head);
	head2Robot->addChild(leftAntenna2Head);
	head2Robot->addChild(rightAntenna2Head);

	tempGeo = new Geometry("eyeball_s.obj");
	leftEye2Head->addChild(tempGeo);
	rightEye2Head->addChild(tempGeo);
	tempGeo = new Geometry("antenna_s.obj");
	leftAntenna2Head->addChild(tempGeo);
	rightAntenna2Head->addChild(tempGeo);

	sphere = new Sphere("sphere.obj");
	S = glm::scale(glm::vec3(2.5, 2.5, 2.5));
	Transform* sphere2Cluster = new Transform(S, 0, 0, 0);
	sphere2Cluster->addChild(sphere);
	S = glm::scale(glm::vec3(16.78, 16.78, 16.78));
	Transform* sphere2Army = new Transform(S, 0, 0, 0);
	sphere2Army->addChild(sphere);

	sphere->setProgramID(program);

	Transform* Cluster = new Transform(glm::mat4(1), 0, 0, 0);
	for (float i = -10.0; i <= 10.0; i = i + 5.0)
	{
		for (float j = -10.0; j <= 10.0; j = j + 5.0)
		{
			T = glm::translate(glm::vec3(i, 0, j));
			Transform* rot2Cluster = new Transform(T, 0, 1, 4.0);
			rot2Cluster->addChild(robot);
			rot2Cluster->addChild(sphere2Cluster);
			Cluster->addChild(rot2Cluster);
		}
	}

	Army = new Transform(glm::mat4(1), 0, 0, 0);
	for (int k = 0; k < 4; k++)
	{
		T = glm::translate(glm::vec3(-16.78 +(k%2)*33.56, 0, 16.78-(k/2)*33.56));
		Transform* Cluster2Army = new Transform(T, 0, 2, 18.0);
		Cluster2Army->addChild(Cluster);
		Cluster2Army->addChild(sphere2Army);
		Army->addChild(Cluster2Army);
	}
	

	// Set bunny to be the first to display
	currentRot = robot;

	/*
	glUseProgram(programLight);
	lightObj = new PointCloud("sphere.obj", pointsSize);
	std::cout << "Finish loading sphere.obj" << std::endl;
	lightObj->scaleModel(0.06);
	lightObj->translateModel(light.position);
	glUseProgram(program);
	*/

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	//delete bunnyPoints, dragonPoints, bearPoints, lightObj;

	// Delete the shader program.
	glDeleteProgram(program);
	//glDeleteProgram(programLight);
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
	/*
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);
*/
}

void Window::idleCallback(GLFWwindow* window)
{
	// Perform any updates as necessary. 
	//currentObj->update(); //comment: spin 9/29
	if (cullingRefreshFlag == 1)
	{
		Army->resetVisibilityCount();
		frustrum->setValue(FOV, nearP, farP, (double)width, (double)height,
			center, up, eye);
		Army->refreshCullingState(glm::mat4(1));
		Army->printCullingTimes();

		std::string curNum = std::to_string(Army->getVisibilityCount());
		char newTitle[20] = "Visible Robot: ";
		strcat(newTitle, curNum.c_str());
		const char* title = newTitle;
		glfwSetWindowTitle(window, title);
		cullingRefreshFlag = 0;
	}

	currentRot->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Specify the values of the uniform variables we are going to use.
	
	//glm::mat4 model = currentRot->getModel(); // put into geometry

	//glm::vec3 color = currentObj->getColor();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // put into geometry

	//glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	//void glUniformMatrix4fv(GLint location,GLsizei count,GLboolean transpose,const GLfloat * value);
	//void glUniform3fv(GLint location,GLsizei count,const GLfloat * value);
	//void glUniform1i(GLint location,GLint v0);
	//glUniform3fv(viewPosLoc, 1, glm::value_ptr(eye));

	//Change value
	//GLuint colorTypeLoc;
	//GLuint materialAmbientLoc, materialDiffuseLoc, materialSpecularLoc, materialShininessLoc;
	//GLuint lightPositionLoc, lightAmbientLoc, lightDiffuseLoc, lightSpecularLoc, lightAttenuationParaLoc;
	
	/*
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
	*/

	/* light
	glUseProgram(programLight);
	glm::mat4 modelLight = lightObj->getModel();
	glUniformMatrix4fv(projectionLocLight, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLocLight, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLocLight, 1, GL_FALSE, glm::value_ptr(modelLight));
	glUniform3fv(colorLocLight, 1, glm::value_ptr(light.specular));
	//lightObj->translateModel(light.position - lightObj->getModelCenter());
	lightObj->draw();
	glUseProgram(program);
	*/


	// Set the viewport size.
	glViewport(0, 0, width, height);

	if (cullingDemoMode == 1)
	{
		// Set the projection matrix.
		Window::projection = glm::perspective((double)glm::radians(FOV + 20.0), (double)width / (double)height,
			(double)nearP, (double)farP);
	}
	else
	{
		// Set the projection matrix.
		Window::projection = glm::perspective((double)glm::radians(FOV), (double)width / (double)height,
			(double)nearP, (double)farP);
	}

	// Render the object.
	Army->draw(glm::mat4(1), 0);

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
/*
		case GLFW_KEY_F1:
			// Set currentObj to bunny
			currentObj = bunnyPoints;
			material.ambient = glm::vec3(0.19225,0.19225,0.19225);
			material.diffuse = glm::vec3(0.0, 0.0, 0.0);
			material.specular = glm::vec3(0.508273, 0.508273, 0.508273);
			material.shininess = 8.0;
			break;
		case GLFW_KEY_F2:
			// Set currentObj to dragonPoints
			currentObj = dragonPoints;
			material.ambient = glm::vec3(0.1,0.18725,0.1745) * 2.f;
			material.diffuse = glm::vec3(0.396, 0.74151, 0.69102);
			material.specular = glm::vec3(0.0, 0.0, 0.0);
			material.shininess = 8.0;
			break;
		case GLFW_KEY_F3:
			// Set currentObj to bearPoints
			currentObj = bearPoints;
			material.ambient = glm::vec3(0.0215,0.1745,0.0215);
			material.diffuse = glm::vec3(0.07568,0.61424,0.07568);
			material.specular = glm::vec3(0.633,0.727811,0.633);
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

*/
		case GLFW_KEY_S:
			sphere->setSphereFlag(1);
			break;
		case GLFW_KEY_C:
			if (cullingEnable == 0)
			{
				std::cout << "Turn on culling." << std::endl;
				Army->cullingEnable();
				Army->resetVisibilityCount();
				frustrum->setValue(FOV, nearP, farP, (double)width, (double)height,
					center, up, eye);
				Army->refreshCullingState(glm::mat4(1));
				Army->printCullingTimes();

				std::string curNum = std::to_string(Army->getVisibilityCount());
				char newTitle[20] = "Visible Robot: ";
				strcat(newTitle, curNum.c_str());
				const char* title = newTitle;
				glfwSetWindowTitle(window, title);
				cullingEnable = 1;
			}
			else
			{
				cullingEnable = 0;
				Army->cullingEnable();
				
				const char* title = "GLFW project";
				glfwSetWindowTitle(window, title);
				std::cout << "Turn off culling." << std::endl;
			}
			
			break;
		case GLFW_KEY_D:
			cullingDemoMode = (cullingDemoMode + 1) % 2;
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
	//GLfloat  factor = pow(1.05, yOffset);
	/*
	if (modelRotateEnable == 1)
	{
		//currentRot->scaleModel(factor);
	}
	if (lightRotateEnable == 1)
	{
		//glm::vec3 currentPosition = lightObj->getModelCenter();
		//glm::vec3 destination = factor * currentPosition;
		//lightObj->translateModel(destination-currentPosition);
		//light.position = lightObj->getModelCenter();

	}*/
	//FOV = FOV * factor;
	FOV = FOV + yOffset/4;
	FOV = (FOV > 120) ? 120 : FOV;
	FOV = (FOV < 5) ? 5 : FOV;

	if (cullingEnable == 1)
	{
		cullingRefreshFlag == 1;
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
		/*
		if (modelRotateEnable == 1)
		{
			//currentObj->rotateModel(cursorPrevPos, v);
		}
		if (lightRotateEnable == 1)
		{
			//lightObj->rotateModel(cursorPrevPos, v);
			//light.position = lightObj->getModelCenter();
		}*/
		rotateCamera(cursorPrevPos, v);
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

void Window::rotateCamera(glm::vec3 prevCursorPos, glm::vec3 curCursorPos)
{
	GLfloat degree;
	glm::vec3 normalVec = glm::cross(prevCursorPos, curCursorPos);
	if (glm::length(normalVec) > 0)
	{
		glm::vec3 direction = center - eye;
		glm::mat4 rotateMatrix(1);
		normalVec = glm::normalize(normalVec);
		GLfloat cosdeg = glm::dot(prevCursorPos, curCursorPos);
		degree = 0.5 * glm::acos(cosdeg);
		//std::cout << "Cos = " << cosdeg << std::endl;
		//std::cout << "Degree = " << degree << std::endl;
		rotateMatrix = glm::rotate(rotateMatrix, degree, normalVec);
		//model = rotateMatrix * model;
		//modelCenter = rotateMatrix * glm::vec4(modelCenter, 1.0);
		direction = glm::vec3(rotateMatrix * glm::vec4(direction, 0));
		center = direction + eye;
		view = glm::lookAt(eye, center, up);

		if (cullingEnable == 1)
		{
			cullingRefreshFlag = 1;
		}
	}

}