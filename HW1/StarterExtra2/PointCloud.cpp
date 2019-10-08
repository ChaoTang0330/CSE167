#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */

	/*
	points =
	{
		glm::vec3(-2.5, 2.5, 2.5),
		glm::vec3(-2.5, -2.5, 2.5),
		glm::vec3(2.5, -2.5, 2.5),
		glm::vec3(2.5, 2.5, 2.5),
		glm::vec3(-2.5, 2.5, -2.5),
		glm::vec3(-2.5, -2.5, -2.5),
		glm::vec3(2.5, -2.5, -2.5),
		glm::vec3(2.5, 2.5, -2.5)
	};
	*/

	/* Modify, support multiple objects, 9/29 */
	if(objFilename == "foo")
	{
		points = 
		{
			glm::vec3(-2.5, 2.5, 2.5),
			glm::vec3(-2.5, -2.5, 2.5),
			glm::vec3(2.5, -2.5, 2.5),
			glm::vec3(2.5, 2.5, 2.5),
			glm::vec3(-2.5, 2.5, -2.5),
			glm::vec3(-2.5, -2.5, -2.5),
			glm::vec3(2.5, -2.5, -2.5),
			glm::vec3(2.5, 2.5, -2.5)
		};
	}
	else
	{
		readObjFile(objFilename, &points);
	}

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
	//Find max & min
	maxCoordinate = points[0];
	minCoordinate = points[0];
	//refresh min and max
	for (int i = 1; i < points.size(); i++)
	{
		if (points[i].x < minCoordinate.x)
		{
			minCoordinate.x = points[i].x;
		}
		else if (points[i].x > maxCoordinate.x)
		{
			maxCoordinate.x = points[i].x;
		}

		if (points[i].y < minCoordinate.y)
		{
			minCoordinate.y = points[i].y;
		}
		else if (points[i].y > maxCoordinate.y)
		{
			maxCoordinate.y = points[i].y;
		}

		if (points[i].z < minCoordinate.z)
		{
			minCoordinate.z = points[i].z;
		}
		else if (points[i].z > maxCoordinate.z)
		{
			maxCoordinate.z = points[i].z;
		}
	}
	//Centralize
	//calculate central point
	float xLength = maxCoordinate.x + minCoordinate.x;
	float yLength = maxCoordinate.y + minCoordinate.y;
	float zLength = maxCoordinate.z + minCoordinate.z;
	float tempRadius = 0;
	//Moving to center
	for (int i = 0; i < points.size(); i++)
	{
		points[i].x = points[i].x - (float)0.5 * xLength;
		points[i].y = points[i].y - (float)0.5 * yLength;
		points[i].z = points[i].z - (float)0.5 * zLength;
		tempRadius = sqrt(pow(points[i].x, 2) + pow(points[i].y, 2) + pow(points[i].z, 2));
		radius = (tempRadius > radius) ? tempRadius : radius;
	}
	//std::cout << "radius = " << radius << std::endl;
	
	//Scale
	float winLength = 480;
	if (radius > 0)
	{
		float factor = 50;
		float scalePara = winLength / (radius * factor);
		glm::vec3 point;
		for (int i = 0; i < points.size(); i++)
		{
			points[i].x = points[i].x * scalePara;
			points[i].y = points[i].y * scalePara;
			points[i].z = points[i].z * scalePara;
		}
	}
	objPoints = points;

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Set point size.
	glPointSize(pointSize);
	// Draw points 
	glDrawArrays(GL_POINTS, 0, points.size());
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	spin(0.1f);

	if (step < REFRESH_STEP)
	{
		//Moving points to final destination
		for (int i = 0; i < objPoints.size(); i++)
		{
			points[i].x = points[i].x + (objPoints[i].x - points[i].x) / REFRESH_STEP * step;
			points[i].y = points[i].y + (objPoints[i].y - points[i].y) / REFRESH_STEP * step;
			points[i].z = points[i].z + (objPoints[i].z - points[i].z) / REFRESH_STEP * step;
		}
		for (int i = objPoints.size(); i < points.size(); i++)
		{
			points[i].x = points[i].x + (objPoints[objPoints.size()-1].x - points[i].x) / REFRESH_STEP * step;
			points[i].y = points[i].y + (objPoints[objPoints.size()-1].y - points[i].y) / REFRESH_STEP * step;
			points[i].z = points[i].z + (objPoints[objPoints.size()-1].z - points[i].z) / REFRESH_STEP * step;
		}
		step++;

		// Bind to the VAO.
		glBindVertexArray(vao);

		// Bind to the first VBO. We will use it to store the points.
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// Pass in the data.
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
			points.data(), GL_STATIC_DRAW);
		// Enable vertex attribute 0. 
		// We will be able to access points through it.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		// Unbind from the VBO.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Unbind from the VAO.
		glBindVertexArray(0);
		//draw();
	}
	
}

void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	PointCloud::pointSize = size;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

/* Read obj files, 9/29 */
void PointCloud::readObjFile(std::string fileName, std::vector<glm::vec3> *readPoints)
{
	std::ifstream objFile("PointObj/" + fileName); // The obj file we are reading.

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point.
				readPoints->push_back(point);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << fileName << std::endl;
	}

	objFile.close();

	return;
}

void PointCloud::trans(std::vector<glm::vec3> previousPoints)
{
	points = previousPoints;
	for (int i = points.size(); i < objPoints.size(); i++)
	{
		points.push_back(points[points.size()-1]);
	}
	step = 0;
}