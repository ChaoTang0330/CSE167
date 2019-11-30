#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>

#include<fstream>
#include<iostream>
#include<sstream>
#include<math.h>

#include "node.h"

class Transform : public Node
{
private:
	glm::mat4 TransM = glm::mat4(1);
	std::list<Node*> children;
	GLuint animateType;
	GLfloat prevAngle = 0;

	static int numOfVisibility, cullingTimes, prevCullingTimes;
	static Frustum *frustum;
	static int cullingFlag;
	static std::list<bool> cullingStatusList;
	static std::list<bool>::iterator listIterator;
	static GLuint programID;

	int cullingLevel;
	int cullingState = 0;

	float cullingRadius;

public:
	Transform(glm::mat4 initial, GLuint animate, int cullingLevel, float cullingRadius);//Node* child
	~Transform();

	void draw(glm::mat4 model, GLint color);
	void update();
	void refreshCullingState(glm::mat4 M);

	void addChild(Node* child);
	void rotateModel(glm::vec3 prevCursorPos, glm::vec3 curCursorPos);

	void resetVisibilityCount() 
	{	
		numOfVisibility = 0; 
		cullingTimes = 0; 
		listIterator = cullingStatusList.begin();
	}

	int getVisibilityCount() { return numOfVisibility; }
	void printCullingTimes();
	void setFrustrum(Frustum* curFrustum) { frustum = curFrustum; }

	void cullingEnable() { cullingFlag = (cullingFlag + 1) % 2; }

};

#endif // _TRANSFORM_H_