#pragma once

#define GLEW_STATIC
#include "dep/GLEW/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <fstream>
#include <sstream>

static GLuint CompileShader(GLuint type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	//error handling:
	int ret;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
	if (ret == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		fprintf(stderr, "failed to compile %s shader\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		fprintf(stderr, "%s\n", message);
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

static GLuint CreateShader(const char* vs_path, const char* fs_path) {
	GLuint program = glCreateProgram();

	std::ifstream v(vs_path);
	std::stringstream vs_text;
	vs_text << v.rdbuf();
	v.close();

	std::ifstream f(fs_path);
	std::stringstream fs_text;
	fs_text << f.rdbuf();
	f.close();

	GLuint vs = CompileShader(GL_VERTEX_SHADER, vs_text.str().c_str());
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fs_text.str().c_str());

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
