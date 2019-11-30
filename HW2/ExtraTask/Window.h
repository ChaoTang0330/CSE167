#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	//static Cube* cube;
	static PointCloud* currentObj;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, projectionLoc, viewLoc, modelLoc;// , colorLoc;
	static GLuint colorTypeLoc, viewPosLoc;
	static GLuint materialAmbientLoc, materialDiffuseLoc, materialSpecularLoc, materialShininessLoc;
	static GLuint lightPositionLoc, lightAmbientLoc, lightDiffuseLoc, lightSpecularLoc, lightAttenuationParaLoc;
	static GLuint lightDirectionLoc, lightCutoffLoc, lightExponentLoc;
	static GLuint dlightPositionLoc, dlightAmbientLoc, dlightDiffuseLoc, dlightSpecularLoc, dlightAttenuationParaLoc;
	static GLuint dlightDirectionLoc, dlightCutoffLoc, dlightExponentLoc;
	static GLint colorType;
	static GLint modelRotateEnable, lightRotateEnable;

	//Modify: 9/29
	static PointCloud* Window::bunnyPoints;
	static PointCloud* Window::dragonPoints;
	static PointCloud* Window::bearPoints;
	static GLfloat pointsSize;
	//static float winLength;
	static int shiftFlag;
	static glm::vec3 cursorPrevPos;

	static int mouseLeftButtonFlag;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
	static glm::vec3 Window::cursorPositionMapping(double xPos, double yPos);

	static Material material;
	static Light light,dlight;

	//LightObj
	static PointCloud* lightObj;
	static GLuint programLight, projectionLocLight, viewLocLight, modelLocLight, colorLocLight;
	static GLuint lightObjSwitchLoc, lightSwitchLoc;
	static GLuint lightSwitch;
};

#endif