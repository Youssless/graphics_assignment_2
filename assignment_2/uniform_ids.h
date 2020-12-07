#pragma once
#include "base_component.h"

struct Uniforms {
public:
	GLuint ambient_colID;
	GLuint light_posID;
	GLuint viewID;
	GLuint projectionID;
	GLuint modelID;
	GLuint shininessID;
	GLuint normal_trans_id;

	Uniforms() {
		ambient_colID = 0;
		light_posID = 0;
		viewID = 0;
		modelID = 0;
		projectionID = 0;
		shininessID = 0;
		normal_trans_id = 0;
	}

	Uniforms(const GLuint& program) {
		ambient_colID = glGetUniformLocation(program, "ambient_colour");
		light_posID = glGetUniformLocation(program, "lightpos");
		viewID = glGetUniformLocation(program, "view");
		modelID = glGetUniformLocation(program, "model");
		projectionID = glGetUniformLocation(program, "projection");
		shininessID = glGetUniformLocation(program, "shininess");
		normal_trans_id = glGetUniformLocation(program, "normal_transformation");
	}
};