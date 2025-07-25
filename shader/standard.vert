#version 330
layout(location=0) in vec3 triangle;
layout(location=1) in vec3 col;
uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
out vec3 color;
void main()
{
	color=col;
	gl_Position=projection*camera*model*vec4(triangle,1);
}
