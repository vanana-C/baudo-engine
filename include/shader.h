#ifndef SHADER_H
#define SHADER_H

GLuint compile_shader(const char* shader_code,GLenum shader_type);

char* read_shader_file(const char* file_name);

GLuint create_shader_programe(GLuint vertex_shader,GLuint frag_shader);

#endif
