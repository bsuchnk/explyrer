#pragma once

#define GLEW_STATIC
#include "dep/GLEW/glew.h"

struct Grid
{
	GLuint vertex_count;

	GLuint vao;
};

struct Axis
{
	GLuint vertex_count;

	GLuint vao;
};