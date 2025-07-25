#include <GL/glew.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "object.h"
#include "opengl_fn.h"

int read_file(char* file_name,int** number_model_ptr,model_transformation*** model_info_ptr)
{
	fprintf(stderr,"Warning reading a non compatible or a malicious file could create undefined probleme\n");
	FILE* file=fopen(file_name,"r");
	if(file==NULL)
	{
		printf("ERROR : '%s' does not exist.\n",file_name);
		return -1;
	}
	fseek(file,0,SEEK_END);
	long file_size=ftell(file);
	printf("file_size=%d\n",file_size);
	fseek(file,0,SEEK_SET);

	if(file_size<8)
	{
		fprintf(stderr,"the file is to short.\n");
		return -1;
	}

	int number_vao;
	fread(&number_vao,sizeof(int),1,file);
	printf("number_vao=%d\n",number_vao);
	*number_model_ptr=malloc(number_vao*sizeof(int));
	*model_info_ptr=malloc(number_vao*sizeof(void*));

	int* number_model=*number_model_ptr;
	model_transformation** model_info=*model_info_ptr;

	for(int i=0;i<number_vao;i++)
	{
		fread(&number_model[i],sizeof(int),1,file);
		printf("number_model[%d]=%d\n",i,number_model[i]);
		model_info[i]=malloc(sizeof(model_transformation)*number_model[i]);
		fread(model_info[i],sizeof(model_transformation),number_model[i],file);
	}

	fclose(file);
	return number_vao;
}

int read_CONF(char* file_name,int id_count,int* id_num,vec3** pos_table,vec3** rotation_table,vec3** scale_table)
{
	fprintf(stderr,"Warning reading a non compatible or a malicious file could create undefined probleme\n");
	FILE* file=fopen(file_name,"w");
	fwrite(&id_count,sizeof(int),1,file);
	for(int i=0;i<id_count;i++)
	{
		fwrite(&id_num[i],sizeof(int),1,file);
		for(int j=0;j<id_num[i];i++)
		{
			fwrite(pos_table,sizeof(vec3),1,file);
			fwrite(rotation_table,sizeof(vec3),1,file);
			fwrite(scale_table,sizeof(vec3),1,file);
		}
	}
	fprintf(file,"\0\0\0\0");
	fclose(file);
	return 0;
}

void* read_vao(char* file_name,int number_vao)
{
	fprintf(stderr,"Warning reading a non compatible or a malicious file could create undefined probleme\n");
	FILE* file=fopen(file_name,"r");

	vao_information* vao_info=malloc(number_vao*sizeof(vao_information*));

	for(int i=0;i<number_vao;i++)
	{
		glGenVertexArrays(1,&vao_info[i].vao);
		glBindVertexArray(vao_info[i].vao);

		int number_vertices;
		fread(&number_vertices,sizeof(int),1,file);
		printf("number_vertices=%d\n",number_vertices);
		float* vertices=malloc(number_vertices*sizeof(float)*6);
		fread(vertices,sizeof(float),number_vertices*6,file);
		for(int i=0;i<number_vertices*6;i++)
			printf("%f ",vertices[i]);
		printf("\n");


		int number_indice;
		fread(&number_indice,sizeof(int),1,file);
		printf("number_indice=%d\n",number_indice);
		int* indice=malloc(number_indice*sizeof(float)*3);
		fread(indice,sizeof(float),number_indice*3,file);
		for(int i=0;i<number_indice*3;i++)
			printf("%d ",indice[i]);
		printf("\n");


		glBindVertexArray(vao_info[i].vao);
	
		GLuint ebo;
		glGenBuffers(1,&ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(float)*number_indice*3,indice,GL_STATIC_DRAW);

		GLuint vbo;
		glGenBuffers(1,&vbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float)*number_vertices*6,vertices,GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);

		vao_info[i].shader=0;
		free(indice);
		free(vertices);
	}


	fclose(file);
	return vao_info;
}

int new_file(char* file_name)
{
	fprintf(stderr,"Warning reading a non compatible or a malicious file could create undefined probleme\n");
	FILE* file=fopen(file_name,"r");
	fclose(file);
	return 0;
}

int save_file(char* file_name,int number_vao,int* number_model,model_transformation** model_info)
{
	FILE* file=fopen(file_name,"w");
	fwrite(&number_vao,sizeof(int),1,file);
	for(int i=0;i<number_vao;i++)
	{
		fwrite(&number_model[i],sizeof(int),1,file);
		for(int j=0;j<number_model[i];j++)
			fwrite(&model_info[i][j],sizeof(model_transformation),1,file);
	}
	int end=0;
	fwrite(&end,sizeof(int),1,file);
	return 0;
}
