#pragma once

#define GLEW_STATIC
#include "glew.h"

struct Grid
{
	GLuint vertex_count;

	GLuint vao;
};

struct Axis
{
	GLuint vertex_count;

	GLuint vao1;
	GLuint vao2;
};