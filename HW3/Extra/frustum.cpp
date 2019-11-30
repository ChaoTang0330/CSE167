#include "node.h"

Frustum::Frustum()
{

}

Frustum::~Frustum()
{

}

void Frustum::setValue(double FOV, float nearP, float farP,
	float winLength, float winHight,
	glm::vec3 center, glm::vec3 up, glm::vec3 eye)
{
	Frustum::FOV = FOV;
	Frustum::nearP = nearP;
	Frustum::farP = farP;
	Frustum::winLength = winLength;
	Frustum::winHight = winHight;
	Frustum::center = center;
	Frustum::up = up;
	Frustum::eye = eye;

	refresh();
}

void Frustum::refresh()
{
	aspect = winLength / winHight;
	direction = glm::normalize(center - eye);
	right = glm::cross(direction, up);

	//8 points
	nearCenter = eye + direction * nearP;
	nearHalfHight = tan(glm::radians(FOV / 2)) * nearP;
	nearHalfLength = aspect * nearHalfHight;
	nearTL = nearCenter + up * nearHalfHight - right * nearHalfLength;
	nearTR = nearCenter + up * nearHalfHight + right * nearHalfLength;
	nearBL = nearCenter - up * nearHalfHight - right * nearHalfLength;
	nearBR = nearCenter - up * nearHalfHight + right * nearHalfLength;

	farCenter = eye + direction * farP;
	farHalfHight = tan(glm::radians(FOV / 2)) * farP;
	farHalfLength = aspect * farHalfHight;
	farTL = farCenter + up * farHalfHight - right * farHalfLength;
	farTR = farCenter + up * farHalfHight + right * farHalfLength;
	farBL = farCenter - up * farHalfHight - right * farHalfLength;
	farBR = farCenter - up * farHalfHight + right * farHalfLength;

	//normal of 6 sides
	nearPNorm = glm::normalize(glm::cross(nearTR - nearTL, nearBL - nearTL));
	farPNorm = glm::normalize(glm::cross(farBL - farTL, farTR - farTL));
	topPNorm = glm::normalize(glm::cross(farTR - farTL, nearTL - farTL));
	bottomPNorm = glm::normalize(glm::cross(nearBL - farBL, farBR - farBL));
	leftPNorm = glm::normalize(glm::cross(nearTL - farTL, farBL - farTL));
	rightPNorm = glm::normalize(glm::cross(farBR - farTR, nearTR - farTR));
}

bool Frustum::inFrustum(glm::vec3 position, float radius)
{
	float tempDist = point2PlaneDist(position, nearTL, nearPNorm);
	if (tempDist < -radius)
	{
		return false;
	}
	else
	{
		tempDist = point2PlaneDist(position, farTL, farPNorm);
		if (tempDist < -radius)
		{
			return false;
		}
		else
		{
			tempDist = point2PlaneDist(position, farTL, topPNorm);
			if (tempDist < -radius)
			{
				return false;
			}
			else
			{
				tempDist = point2PlaneDist(position, farBL, bottomPNorm);
				if (tempDist < -radius)
				{
					return false;
				}
				else
				{
					tempDist = point2PlaneDist(position, farBL, leftPNorm);
					if (tempDist < -radius)
					{
						return false;
					}
					else
					{
						tempDist = point2PlaneDist(position, farBR, rightPNorm);
						if (tempDist < -radius)
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

float Frustum::point2PlaneDist(glm::vec3 point, glm::vec3 pointOnPlane, glm::vec3 normal)
{
	return glm::dot((point - pointOnPlane), normal);
}