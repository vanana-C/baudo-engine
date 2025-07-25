#ifndef FILE_H
#define FILE_H

#define FILE_BEM 1
#define FILE_VBO 2
#define FILE_CONF 4

#include "object.h"

int read_file(char* file_name,int** number_model_ptr,model_transformation*** model_info_ptr);

int read_CONF(char* file_name,int id_count,int* id_num,vec3** pos_table,vec3** rotation_table,vec3** scale_table);

void* read_vao(char* file_name,int number_vao);

int new_file(char* file_name);

int save_file(char* file_name,int number_vao,int* number_model,model_transformation** model_info);

#endif
