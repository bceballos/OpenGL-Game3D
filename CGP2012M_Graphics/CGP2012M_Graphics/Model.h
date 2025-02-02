#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "ShaderClass.h"

class Model
{
public:
	//vertex/texture coord/normal containers for the model
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	//interleaved vertex, texture, normal data
	//more efficient than separate VBO's
	std::vector<GLfloat> modelData;

	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	//GLuint EBO;
	//shader for the model
	Shader vsh, fsh;
	GLuint shaderProgram;

	//constructor
	Model() 
	{
		//shaders
		//vSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.vert");
		//fSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.frag");
		vsh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.vert");
		fsh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.frag");

		vsh.getShader(1);
		fsh.getShader(2);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vsh.shaderID);
		glAttachShader(shaderProgram, fsh.shaderID);
		glLinkProgram(shaderProgram);

		glDeleteShader(vsh.shaderID);
		glDeleteShader(fsh.shaderID);
	};



	void setBuffers()
	{
		////interleave the vertex/texture/normal data
		for (int i = 0; i < indices.size(); i+=3)
		{
			modelData.push_back(vertices[indices[i]].x);
			modelData.push_back(vertices[indices[i]].y);
			modelData.push_back(vertices[indices[i]].z);
			modelData.push_back(texCoords[indices[i+1]].x);
			modelData.push_back(texCoords[indices[i+1]].y);
			modelData.push_back(normals[indices[i+2]].x);
			modelData.push_back(normals[indices[i+2]].y);
			modelData.push_back(normals[indices[i+2]].z);
	
		}
		std::cout << sizeof(modelData[0]) << std::endl;
		std::cout << modelData.size() << std::endl;
		//
		//OpenGL buffers
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);
		//initialise the index buffer
		//glGenBuffers(1, &this->EBO);

		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//buffer size from vector: vector size (number of elements) * size of one element
		glBufferData(GL_ARRAY_BUFFER, modelData.size()*sizeof(modelData[0]) , &modelData[0], GL_STATIC_DRAW);
		// Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//set the texture attribute pointer
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//set normal attrib pointer
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		//set up the EBO

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
		
		//Unbind the VAO
		glBindVertexArray(0);
	}

	void render()
	{
		//draw the square 
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size()*sizeof(vertices));
		glBindVertexArray(0);
	}

};