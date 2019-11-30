#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;


	glm::vec3 specular;
	GLfloat shininess;
};

struct Light
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat attenuationPara;
};


class Node
{
protected:
	glm::mat4 model;
	glm::vec3 color;
	glm::vec3 modelCenter;
public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
	glm::vec3 getModelCenter() { return modelCenter; }

	virtual void draw(glm::mat4 C, GLint color) = 0;
	virtual void update() = 0;

	virtual void refreshCullingState(glm::mat4 C) = 0;
};

class Frustum
{
private:
	//8 parameters
	double FOV;
	float nearP, farP;
	float winLength, winHight;
	glm::vec3 center;
	glm::vec3 up;
	glm::vec3 eye;
public:
	Frustum();
	~Frustum();

	void setValue(double FOV, float nearP, float farP,
	float winLength, float winHight,
	glm::vec3 center,glm::vec3 up, glm::vec3 eye);

	void refresh();

	bool inFrustum(glm::vec3 position, float radius);

	float point2PlaneDist(glm::vec3 point, glm::vec3 pointOnPlane, glm::vec3 normal);

	double aspect;
	glm::vec3 direction;
	glm::vec3 right;

	//8 points
	glm::vec3 nearCenter;
	float nearHalfHight;
	float nearHalfLength;
	glm::vec3 nearTL;
	glm::vec3 nearTR;
	glm::vec3 nearBL;
	glm::vec3 nearBR;

	glm::vec3 farCenter;
	float farHalfHight;
	float farHalfLength;
	glm::vec3 farTL;
	glm::vec3 farTR;
	glm::vec3 farBL;
	glm::vec3 farBR;

	//normal of 6 sides
	glm::vec3 nearPNorm;
	glm::vec3 farPNorm;
	glm::vec3 topPNorm;
	glm::vec3 bottomPNorm;
	glm::vec3 leftPNorm;
	glm::vec3 rightPNorm;

};

class frustum
{
};

#endif

