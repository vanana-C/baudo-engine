#include <GL/glew.h>

#include <stdio.h>
#include <stdlib.h>

#include "opengl_fn.h"

GLuint compile_shader(const char* shader_code,GLenum shader_type)
{
	GLuint return_shader=glCreateShader(shader_type);
	glShaderSource(return_shader,1,&shader_code,NULL);
	glCompileShader(return_shader);
	GLuint success=0;
	glGetShaderiv(return_shader,GL_COMPILE_STATUS,&success);
	if(!success)
	{
		char info_log[512];
		glGetShaderInfoLog(return_shader,512,NULL,info_log);
		printf("Compile ERROR;\n%s",info_log);
		return -1;
	}
	return return_shader;
}

char* read_shader_file(const char* file_name)
{
	FILE* file=fopen(file_name,"r");
	if(file==NULL)
	{
		fprintf(stderr,"ERROR : the file \"%s\" does not exist.\n",file_name);
		return NULL;
	}
	fseek(file,0,SEEK_END);
	long int sizeof_file=ftell(file);
	fseek(file,0,SEEK_SET);
	char* shader_code=malloc(sizeof_file+1);

	for(int i=0;i<sizeof_file;i++)
		shader_code[i]=fgetc(file);
	shader_code[sizeof_file]=0;

	return shader_code;
}

GLuint create_shader_programe(GLuint vertex_shader,GLuint frag_shader)
{
	GLuint shader_program=glCreateProgram();
	glAttachShader(shader_program,vertex_shader);
	glAttachShader(shader_program,frag_shader);
	glLinkProgram(shader_program);

	GLint success;
	glGetProgramiv(shader_program,GL_LINK_STATUS,&success);
	if(!success)
	{
		char* error_string;
		glGetProgramInfoLog(shader_program,512,NULL,error_string);
		fprintf(stderr,"Linking ERROR:\n%s\n",error_string);
	}
	glDetachShader(shader_program,vertex_shader);
	glDetachShader(shader_program,frag_shader);
	glDeleteShader(vertex_shader);
	return shader_program;
}

