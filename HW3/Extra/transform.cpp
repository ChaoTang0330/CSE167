#include "transform.h"
#include "GLFW/glfw3.h"

Frustum* Transform::frustum;
int Transform::cullingFlag(0), Transform::cullingTimes(0), Transform::prevCullingTimes(0);
int Transform::numOfVisibility(0);
std::list<bool> Transform::cullingStatusList;
std::list<bool>::iterator Transform::listIterator = cullingStatusList.begin();
GLuint Transform::programID;

Transform::Transform(glm::mat4 initial, GLuint animate, int cullingLevel, float cullingRadius)//Node* child
{
	model = initial;
	//addChild(child);
	animateType = animate;
	Transform::cullingLevel = cullingLevel;
	Transform::cullingRadius = cullingRadius;
}

Transform::~Transform()
{
	
}

void Transform::rotateModel(glm::vec3 prevCursorPos, glm::vec3 curCursorPos)
{
	GLfloat degree;
	glm::vec3 normalVec = glm::cross(prevCursorPos, curCursorPos);
	if (glm::length(normalVec) > 0)
	{
		glm::mat4 rotateMatrix(1);
		normalVec = glm::normalize(normalVec);
		GLfloat cosdeg = glm::dot(prevCursorPos, curCursorPos);
		degree = glm::acos(cosdeg);
		//std::cout << "Cos = " << cosdeg << std::endl;
		//std::cout << "Degree = " << degree << std::endl;
		rotateMatrix = glm::rotate(rotateMatrix, degree, normalVec);
		TransM = rotateMatrix * TransM;
		modelCenter = rotateMatrix * glm::vec4(modelCenter, 1.0);
	}

}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::draw(glm::mat4 M, GLint color)
{
	if (cullingLevel != 0 && cullingFlag == 1 && !cullingStatusList.empty())
	{
		if (listIterator == cullingStatusList.end())
		{
			listIterator = cullingStatusList.begin();
		}
		if (*listIterator)
		{
			listIterator++;
			return;
		}
		listIterator++;
	}

	//std::cout << "transform draw" << std::endl;
	std::list<Node*>::iterator i;
	for (i = children.begin(); i != children.end(); i++)
	{
		GLint curColor;
		if (cullingLevel == 2 || (color == 2 && cullingLevel == 0))
		{
			(*i)->draw(M * model, 2);
		}
		else
		{
			(*i)->draw(M * model, 0);
		}
		
	}
}


void Transform::update()
{
	glm::mat4 T_1, R, T;
	GLfloat timeValue = glfwGetTime();
	GLfloat curAngle = 0.7 * sin(3.14*timeValue);
	switch (animateType)
	{
		case 1:
			T = glm::translate(glm::vec3(0, -1, 0));
			T_1 = glm::translate(glm::vec3(0, 1, 0));
			R = glm::rotate(curAngle - prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		case 2:
			T = glm::translate(glm::vec3(0, -1, 0));
			T_1 = glm::translate(glm::vec3(0, 1, 0));
			R = glm::rotate(-curAngle + prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		case 3:
			T = glm::translate(glm::vec3(0, 1, 0));
			T_1 = glm::translate(glm::vec3(0, -1, 0));
			R = glm::rotate(-curAngle + prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		case 4:
			T = glm::translate(glm::vec3(0, 1, 0));
			T_1 = glm::translate(glm::vec3(0, -1, 0));
			R = glm::rotate(curAngle - prevAngle, glm::vec3(1, 0, 0));
			model = T_1 * R * T * model;
			break;
		default:
			break;
	}

	prevAngle = curAngle;

	std::list<Node*>::iterator i;
	for (i = children.begin(); i != children.end(); i++)
	{
		(*i)->update();
	}
}

void Transform::printCullingTimes()
{
	if (cullingTimes != prevCullingTimes)
	{
		std::cout << "Current culling times: " << cullingTimes << std::endl;
	}
	prevCullingTimes = cullingTimes;
}

void Transform::refreshCullingState(glm::mat4 M)
{
	if (listIterator == cullingStatusList.begin())
	{
		cullingStatusList.clear();
		listIterator = cullingStatusList.begin();
	}

	if (cullingFlag == 1 && cullingLevel != 0)
	{
		glm::vec3 sphereCenter = glm::vec3(M * model * glm::vec4(0, 0, 0, 1));
		
		if (frustum->inFrustum(sphereCenter, cullingRadius))
		{
			if(cullingLevel == 1) numOfVisibility++;
			cullingStatusList.push_back(false);
			listIterator = cullingStatusList.end();
		}
		else
		{
			cullingTimes++;
			cullingStatusList.push_back(true);
			listIterator = cullingStatusList.end();
			return;
		}
	}

	std::list<Node*>::iterator i;
	for (i = children.begin(); i != children.end(); i++)
	{
		(*i)->refreshCullingState(M * model);
	}
}
