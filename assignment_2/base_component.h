#pragma once
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <functional>

class BaseComponent {
public:
	virtual void create_component(const GLuint &program) = 0;

	virtual void translateX(std::function<float (float, float)> op) = 0;
	virtual void translateY(std::function<float (float, float)> op) = 0;
	virtual void translateZ(std::function<float (float, float)> op) = 0;


};