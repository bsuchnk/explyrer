#pragma once

#define GLEW_STATIC
#include "dep/GLEW/glew.h"
#include <string>
#include <vector>
#include "dep/glm/glm.hpp"
#include "grid.h"

class Model
{
public:
	~Model();

	int loadPLY(std::string src);
	GLuint m_vertices_count;
	GLfloat* m_vertices;
	std::vector<GLuint> indices;

	glm::vec3 m_zpos;
	glm::vec3 m_max;
	glm::vec3 m_middle;
	glm::vec3 m_size;
	GLfloat m_size_max;

	glm::vec3 m_angle;

	glm::mat4 m_mvp;

	Grid grid;
	Axis axis;

	GLuint m_filling_sp;
	GLuint m_skeleton_sp;

	GLuint m_size_uniform;
	GLuint m_zpos_uniform;
	GLuint m_mvp_uniform;
	GLuint m_mvp_uniform_skeleton;

	GLuint m_vao;


	float xRotation = 0;
	float yRotation = 0;
	float zRotation = 0;


	void makeBuffers();
	void draw();
	void drawMesh();
	void refresh();
	void setShaderPrograms(GLuint filling, GLuint skeleton);
	void calculateMVP();
	void generateGrid();
	void drawGrid();
	void generateAxis();
	void drawAxis();
	void freeBuffers();
};