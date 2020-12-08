#pragma once
#include "base_component.h"

struct SharedUniforms {
public:
	GLuint model_id;
	GLuint normal_trans_id;

	SharedUniforms() {
		model_id = 0;
		normal_trans_id = 0;
	}

	SharedUniforms(const GLuint& program) {
		model_id = glGetUniformLocation(program, "model");
		normal_trans_id = glGetUniformLocation(program, "normal_transformation");
	}
};