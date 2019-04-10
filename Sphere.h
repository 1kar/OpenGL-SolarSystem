#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

class Sphere
{
private:
	std::vector<float> sphere_vertices;
	std::vector<float> sphere_texcoord;
	std::vector<int> sphere_indices;
	GLuint VBO, VAO, EBO;
	float radius = 1.0f;
	int sectorCount = 36;
	int stackCount = 18;

public:

	~Sphere()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	Sphere(float r, int sectors, int stacks)
	{
		radius = r;
		sectorCount = sectors;
		stackCount = stacks;


		/* GENERATE VERTEX ARRAY */
		float x, y, z, xy;                              // vertex position
		float lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = (float)(2 * M_PI / sectorCount);
		float stackStep = (float)(M_PI / stackCount);
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = (float)(M_PI / 2 - i * stackStep);        // starting from pi/2 to -pi/2
			xy = 1.02f * radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);				// r * cos(u) * sin(v)
				sphere_vertices.push_back(x);
				sphere_vertices.push_back(y);
				sphere_vertices.push_back(z);


				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				sphere_vertices.push_back(s);
				sphere_vertices.push_back(t);

			}
		}
		/* GENERATE VERTEX ARRAY */
		

		/* GENERATE INDEX ARRAY */
		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					sphere_indices.push_back(k1);
					sphere_indices.push_back(k2);
					sphere_indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					sphere_indices.push_back(k1 + 1);
					sphere_indices.push_back(k2);
					sphere_indices.push_back(k2 + 1);
				}
			}
		}
		/* GENERATE INDEX ARRAY */
		

		/* GENERATE VAO-EBO */
		//GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, (unsigned int)sphere_vertices.size() * sizeof(float), sphere_vertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)sphere_indices.size() * sizeof(unsigned int), sphere_indices.data(), GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		/* GENERATE VAO-EBO */


	}
	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,   
			(unsigned int)sphere_indices.size(),
			GL_UNSIGNED_INT,					
			(void*)0);
		glBindVertexArray(0);
	}
};


#endif
