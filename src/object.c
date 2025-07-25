#include <cglm/cglm.h>
#include <GL/glew.h>

#include <stdio.h>

#include "object.h"

void update_model(mat4 model,model_transformation model_info)
{
	glm_mat4_identity(model);

	glm_scale(model,model_info.scale);

	mat4 rotation[3]=
	{
		GLM_MAT4_IDENTITY_INIT,
		GLM_MAT4_IDENTITY_INIT,
		GLM_MAT4_IDENTITY_INIT
	};
	
	glm_rotate_x(model,model_info.rotation[0],rotation[0]);
	glm_rotate_y(model,model_info.rotation[1],rotation[1]);
	glm_rotate_z(model,model_info.rotation[2],rotation[2]);

	for(int i=0;i<3;i++)
		glm_mat4_mul(rotation[i],model,model);

	glm_translated(model,model_info.pos);
}

void new_model_transformation(model_transformation* model_info)
{
	model_info->pos[0]=0;
	model_info->pos[1]=0;
	model_info->pos[2]=0;
	model_info->rotation[0]=0;
	model_info->rotation[1]=0;
	model_info->rotation[2]=0;
	model_info->scale[0]=1;
	model_info->scale[1]=1;
	model_info->scale[2]=1;
}
