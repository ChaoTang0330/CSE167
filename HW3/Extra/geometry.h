#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

#include<fstream>
#include<iostream>
#include<sstream>
#include<math.h>

#include "node.h"

class Geometry : public Node
{
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> pointsNormal;
	std::vector<glm::ivec3> faces;
	//std::vector<glm::vec3> pointsColor;

	glm::vec3 maxCoordinate;
	glm::vec3 minCoordinate;

	glm::mat4 currentModel;

	GLuint vao, vbos[2], ebo;
	float radius = 0;

	static glm::mat4 projection;
	//static glm::mat4 view;
	//static glm::vec3 eye, center, up,projectionLoc, viewLoc, ;
	static GLuint program, modelLoc, colorLoc;

public:
	Geometry(std::string objFilename);
	Geometry();
	~Geometry();

	void draw(glm::mat4 model, GLint color);
	void update();
	void refreshCullingState(glm::mat4 model) {}

	void readObjFile(std::string fileName);
	void setModelMatrix(glm::mat4 model);

	void setShaderProID(GLuint programID);

	void initialization(std::string objFilename);
	
	

};

class Sphere :public::Geometry
{
	
private:
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> pointsNormal;
	std::vector<glm::ivec3> faces;
	//std::vector<glm::vec3> pointsColor;

	glm::vec3 maxCoordinate;
	glm::vec3 minCoordinate;

	glm::mat4 currentModel;

	GLuint vao, vbos[2], ebo;
	float radius = 0;

	static glm::mat4 projection;
	//static glm::mat4 view;
	//static glm::vec3 eye, center, up,projectionLoc, viewLoc, ;
	static GLuint program, modelLoc, colorLoc;
	static GLuint sphereFlag;

public:
	Sphere(std::string objFilename);
	void readObjFile(std::string fileName);
	void draw(glm::mat4 model, GLint color);
	void setSphereFlag(GLuint flag);
	void setColor(GLint color);
	void setProgramID(GLuint ID);
};

#endif // _GEOMETRY_H_
