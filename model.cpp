#include "model.h"
#include <fstream>
#include <iostream>
#include "dep/glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <wx/datetime.h>


void Model::draw()
{
	//temporary:
	calculateMVP();

	glBindVertexArray(m_vao);

	glUseProgram(m_filling_sp);
	glUniformMatrix4fv(m_mvp_uniform, 1, GL_FALSE, &m_mvp[0][0]);
	glUniform3f(m_zpos_uniform, m_zpos.x, m_zpos.y, m_zpos.z);
	glUniform3f(m_size_uniform, m_size.x, m_size.y, m_size.z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
}

void Model::drawMesh()
{
	glUseProgram(m_skeleton_sp);
	glUniformMatrix4fv(m_mvp_uniform_skeleton, 1, GL_FALSE, &m_mvp[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1 + m_size_max / 2500);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
}

void Model::calculateMVP()
{
	const float k = 0.8;
	glm::mat4 Projection = glm::ortho(
		-m_size_max * k,
		m_size_max * k,
		-m_size_max * k,
		m_size_max * k,
		0.00001f,
		200000.0f
	);


	GLfloat max_size = std::max(m_size.x, std::max(m_size.y, m_size.z));
	glm::mat4 View = glm::lookAt(
		glm::vec3(max_size * 1.1, max_size * 1.1, max_size * 1.1),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1)
	);


	glm::mat4 Model = glm::mat4(1.0f);
	Model = glm::rotate(Model, xRotation, glm::vec3(1, 0, 0));
	Model = glm::rotate(Model, yRotation, glm::vec3(0, 1, 0));
	Model = glm::rotate(Model, zRotation, glm::vec3(0, 0, 1));


	m_mvp = Projection * View * Model;
}

int Model::loadPLY(std::string src)
{
	std::ifstream f;
	f.open(src);
	if (!f.is_open()) {
		std::cerr << "cannot open file\n";
		return -1;
	}

	unsigned int vertex_n = 0;
	unsigned int index_n = 0;

	std::string comm;
	f >> comm;
	if (comm != "ply") {
		std::cerr << "wrong file format\n";
		return -1;
	}
	while (comm != "end_header") {
		f >> comm;
		if (comm == "element") {
			std::string type;
			f >> type;
			if (type == "vertex")
				f >> vertex_n;
			else if (type == "face")
				f >> index_n;
		}
	}

	m_vertices_count = vertex_n * 3;
	m_vertices = new GLfloat[m_vertices_count];

	if (vertex_n > 0) {
		f >> m_vertices[0] >> m_vertices[1] >> m_vertices[2];

		m_zpos = {
			m_vertices[0],
			m_vertices[1],
			m_vertices[2]
		};
	}
	for (unsigned int i = 3; i < m_vertices_count; i += 3) {
		f >> m_vertices[i] >> m_vertices[i + 1] >> m_vertices[i + 2];

		bool x = m_zpos.x > 1.0;
		m_zpos.x = std::min(m_zpos.x, m_vertices[i]);
		m_zpos.y = std::min(m_zpos.y, m_vertices[i + 1]);
		m_zpos.z = std::min(m_zpos.z, m_vertices[i + 2]);

		m_max.x = std::max(m_max.x, m_vertices[i]);
		m_max.y = std::max(m_max.y, m_vertices[i + 1]);
		m_max.z = std::max(m_max.z, m_vertices[i + 2]);
	}

	m_middle = {
		(m_max.x + m_zpos.x) * 0.5f,
		(m_max.y + m_zpos.y) * 0.5f,
		(m_max.z + m_zpos.z) * 0.5f
	};
	m_size = {
		m_max.x - m_zpos.x,
		m_max.y - m_zpos.y,
		m_max.z - m_zpos.z
	};

	for (unsigned int i = 0; i < m_vertices_count; i += 3) {
		m_vertices[i + 0] -= m_middle.x;
		m_vertices[i + 1] -= m_middle.y;
		m_vertices[i + 2] -= m_middle.z;
	}
	m_zpos -= m_middle;
	m_max -= m_middle;

	m_size_max = std::max(m_size.x, std::max(m_size.y, m_size.z));

	int count = 0;
	indices.clear();
	for (unsigned int i = 0; i < index_n; i++)
	{
		f >> count;
		if (count != 3 && count != 4) {
			std::cerr << "unsupported index count: " << count << "\n";
			return -1;
		}

		if (count == 3) {
			GLuint a, b, c;
			f >> a >> b >> c;
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
		}
		if (count == 4) {
			GLfloat a, b, c, d;
			f >> a >> b >> c >> d;
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(d);
		}
	}

	f.close();
	return 0;
}

void Model::makeBuffers()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices_count * sizeof(GLfloat), m_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void Model::setShaderPrograms(GLuint filling, GLuint skeleton)
{
	m_filling_sp = filling;
	m_skeleton_sp = skeleton;
	m_mvp_uniform = glGetUniformLocation(m_filling_sp, "MVP");
	m_size_uniform = glGetUniformLocation(m_filling_sp, "size");
	m_zpos_uniform = glGetUniformLocation(m_filling_sp, "zpos");
	m_mvp_uniform_skeleton = glGetUniformLocation(m_skeleton_sp, "MVP");
}

void Model::generateGrid()
{
	int cnt = 5;
	GLfloat px, py, pz;
	px = m_size.x / cnt;
	py = m_size.y / cnt;
	pz = m_size.z / cnt;

	//int idk = m_size.x / 5;
	int index = 0;
	float dl = m_size_max / 50;
	int xcnt = (int)(m_size.x / dl);
	int ycnt = (int)(m_size.y / dl);
	int zcnt = (int)(m_size.z / dl);
	grid.vertex_count = cnt * 6 * (xcnt + ycnt + zcnt);

	GLfloat* vertices = (GLfloat*)malloc(grid.vertex_count * sizeof(GLfloat));
	for (int i = 0; i < cnt; i++) {
		for (int j = 0; j < xcnt; j++) {
			vertices[index + 0] = m_zpos.x + j * dl;
			vertices[index + 1] = m_zpos.y + py * i;
			vertices[index + 2] = m_zpos.z;

			vertices[index + 3] = m_zpos.x + j * dl;
			vertices[index + 4] = m_zpos.y;
			vertices[index + 5] = m_zpos.z + pz * i;

			index += 6;
		}
	}
	for (int i = 0; i < cnt; i++) {
		for (int j = 0; j < ycnt; j++) {
			vertices[index + 0] = m_zpos.x + px * i;
			vertices[index + 1] = m_zpos.y + j * dl;
			vertices[index + 2] = m_zpos.z;

			vertices[index + 3] = m_zpos.x;
			vertices[index + 4] = m_zpos.y + j * dl;
			vertices[index + 5] = m_zpos.z + pz * i;

			index += 6;
		}
	}
	for (int i = 0; i < cnt; i++) {
		for (int j = 0; j < zcnt; j++) {
			vertices[index + 0] = m_zpos.x;
			vertices[index + 1] = m_zpos.y + py * i;
			vertices[index + 2] = m_zpos.z + j * dl;

			vertices[index + 3] = m_zpos.x + px * i;
			vertices[index + 4] = m_zpos.y;
			vertices[index + 5] = m_zpos.z + j * dl;

			index += 6;
		}
	}

	glGenVertexArrays(1, &grid.vao);
	glBindVertexArray(grid.vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, grid.vertex_count * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
}

void Model::drawGrid()
{
	glBindVertexArray(grid.vao);

	glUseProgram(m_skeleton_sp);
	glUniformMatrix4fv(m_mvp_uniform_skeleton, 1, GL_FALSE, &m_mvp[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPointSize(2);

	glDrawArrays(GL_POINTS, 0, grid.vertex_count);
}

void Model::generateAxis()
{
	//AXIS:
	axis.vertex_count = 4;
	const int vertex_size = 4 * 3;
	GLfloat vertices[vertex_size] = {
		m_max.x, m_zpos.y, m_max.z,
		m_max.x, m_zpos.y, m_zpos.z,
		m_max.x, m_max.y, m_zpos.z,
		m_zpos.x, m_max.y, m_zpos.z,
	};

	glGenVertexArrays(1, &axis.vao);
	glBindVertexArray(axis.vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
}

void Model::drawAxis()
{
	glBindVertexArray(axis.vao);

	glUseProgram(m_skeleton_sp);
	glUniformMatrix4fv(m_mvp_uniform_skeleton, 1, GL_FALSE, &m_mvp[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glLineWidth(2);

	glDrawArrays(GL_LINE_STRIP, 0, axis.vertex_count);
}

void Model::freeBuffers()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteVertexArrays(1, &grid.vao);
	glDeleteVertexArrays(1, &axis.vao);

	delete m_vertices;
}
