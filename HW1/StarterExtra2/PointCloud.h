#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

/* Modify 9/29 */
#include<fstream>
#include<iostream>
#include<sstream>
#include<math.h>
#include<stdlib.h>

#include "Object.h"

#define REFRESH_STEP 1000

class PointCloud : public Object
{
private:
	
	GLuint vao, vbo;
	GLfloat pointSize;

	float radius = 0;// Modify 9/30
	int step = 0;

public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
	
	void readObjFile(std::string fileName, std::vector<glm::vec3>* readPoints); // Modify 9/29
	glm::vec3 maxCoordinate;// Modify 9/30
	glm::vec3 minCoordinate;// Modify 9/30

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> objPoints;
	
	void trans(std::vector<glm::vec3> previousPoints);
};

#endif

