#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <cglm/cglm.h>

typedef struct
{
	mat4 matrix;
	vec3 pos;
	vec3 center;
	vec3 up;
	float degres_x;
	float degres_y;
}view_matrix;

typedef struct
{
	mat4 matrix;
	float fov;
	float aspect;
	float near;
	float far;
}projection_matrix;

typedef struct
{
	vec3 pos;
	vec3 rotation;
	vec3 scale;
}model_transformation;

typedef struct
{
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint transormation;
	GLuint view;
	GLuint model;
}shader_program;

//function

void update_model(mat4 model,model_transformation model_info);

void new_model_transformation(model_transformation* model_info);

#endif
