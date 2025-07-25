#version 330

layout(location=0) in vec3 vertices;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	gl_Position=projection*camera*model*vec4(vertices,1);
}

