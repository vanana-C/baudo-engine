#version 330
in vec3 color;
out vec4 outcolor;
vec4 ambient=vec4(0.4,0.4,0.4,1);
void main()
{
	outcolor=ambient*vec4(color,1);
}
