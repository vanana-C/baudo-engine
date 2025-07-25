#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "baudo_engine.h"
#include "object.h"
#include "shader.h"
#include "file.h"
#include "math_fn.h"
#include "opengl_fn.h"

const float rotation_model_motion=M_PI_2;
const float pos_model_motion=1;
const float scale_model_motion=1;

int baudo_engine_loop(SDL_Window* window,char* vao_file,char* bem_file)
{
	int return_value=1;

	projection_matrix projection=
	{
		GLM_MAT4_ZERO_INIT,
		90.0,
		800.0/500.0,
		0.1,
		100.0,
	};
	float pro_cotan=1.0/tan(projection.fov/2);
	glm_perspective(glm_rad(projection.fov),projection.aspect,projection.near,projection.far,projection.matrix);

	view_matrix camera=
	{
		GLM_MAT4_ZERO_INIT,
		{0,0,-3},
		{0,0,1},
		{0,1,0},
		0,
		M_PI_2
	};
	glm_lookat(camera.pos,camera.center,(vec3){0,1,0},camera.matrix);











	int select_model_type=0;
	int select_model=0;
	int* number_model;
	model_transformation** model_info;
	int number_vao=read_file(bem_file,&number_model,&model_info);


	mat4** model=malloc(sizeof(void*)*number_vao);

	for(int i=0;i<number_vao;i++)
	{
		model[i]=malloc(number_model[i]*sizeof(mat4));
		for(int j=0;j<number_model[i];j++)
			update_model(model[i][j],model_info[i][j]);
	}
















	vao_information* vao_info=read_vao(vao_file,number_vao);





	char* shader_code=read_shader_file("shader/standard.vert");
	GLuint vertex_shader=compile_shader(shader_code,GL_VERTEX_SHADER);
	free(shader_code);

	shader_code=read_shader_file("shader/standard.frag");
	GLuint fragment_shader=compile_shader(shader_code,GL_FRAGMENT_SHADER);
	free(shader_code);

	shader_code=read_shader_file("shader/select.vert");
	GLuint select_vertex_shader=compile_shader(shader_code,GL_VERTEX_SHADER);
	free(shader_code);

	shader_code=read_shader_file("shader/select.frag");
	GLuint select_fragment_shader=compile_shader(shader_code,GL_FRAGMENT_SHADER);
	free(shader_code);

	GLuint shader_program[2]=
	{
		create_shader_programe(vertex_shader,fragment_shader),
		create_shader_programe(select_vertex_shader,select_fragment_shader)
	};
















	uint32_t model_loc=glGetUniformLocation(shader_program[0],"model");
	uint32_t camera_loc=glGetUniformLocation(shader_program[0],"camera");
	uint32_t projection_loc=glGetUniformLocation(shader_program[0],"projection");

	uint32_t model_loc1=glGetUniformLocation(shader_program[1],"model");
	uint32_t camera_loc1=glGetUniformLocation(shader_program[1],"camera");
	uint32_t projection_loc1=glGetUniformLocation(shader_program[1],"projection");

	glEnable(GL_DEPTH_TEST);

	int window_width,window_height;
	SDL_GetWindowSize(window,&window_width,&window_height);
	SDL_WarpMouseInWindow(window,window_width/2,window_height/2);
	SDL_ShowCursor(SDL_DISABLE);

	const uint8_t* keystate=SDL_GetKeyboardState(NULL);
	bool movement_enable=true;
	SDL_Event event;
	bool running=true;







	//wxc=xyz tanslation
	//rty=xyz rotation
	//klm=scale






	while(running)
	{
		//event
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running=false;
					printf("hello\n");
					break;

				case SDL_KEYDOWN:
					if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE)
						movement_enable^=1;

					if(!movement_enable && (keystate[SDL_SCANCODE_R] || keystate[SDL_SCANCODE_T] || keystate[SDL_SCANCODE_Y]))
					{
						vec3 model_change={0,0,0};
						
						if(event.key.keysym.scancode==SDL_SCANCODE_W)
							model_change[0]=1;

						else if(event.key.keysym.scancode==SDL_SCANCODE_S)
							model_change[0]=-1;

						else if(event.key.keysym.scancode==SDL_SCANCODE_SPACE)
							model_change[1]=1;

						else if(event.key.keysym.scancode==SDL_SCANCODE_LCTRL)
							model_change[1]=-1;

						else if(event.key.keysym.scancode==SDL_SCANCODE_A)
							model_change[2]=1;

						else if(event.key.keysym.scancode==SDL_SCANCODE_D)
							model_change[2]=-1;

						else
							break;

						if(keystate[SDL_SCANCODE_R])
						{
							model_info[select_model_type][select_model].rotation[0]+=model_change[0]*rotation_model_motion;
							model_info[select_model_type][select_model].rotation[1]+=model_change[1]*rotation_model_motion;
							model_info[select_model_type][select_model].rotation[2]+=model_change[2]*rotation_model_motion;
						}
						else if(keystate[SDL_SCANCODE_T])
						{
							model_info[select_model_type][select_model].pos[0]+=model_change[0]*pos_model_motion;
							model_info[select_model_type][select_model].pos[1]+=model_change[1]*pos_model_motion;
							model_info[select_model_type][select_model].pos[2]+=model_change[2]*pos_model_motion;
						}
						else if(keystate[SDL_SCANCODE_Y])
						{
							model_info[select_model_type][select_model].scale[0]+=model_change[0]*scale_model_motion;
							model_info[select_model_type][select_model].scale[1]+=model_change[1]*scale_model_motion;
							model_info[select_model_type][select_model].scale[2]+=model_change[2]*scale_model_motion;
						}
						else
							break;
						update_model(model[select_model_type][select_model],model_info[select_model_type][select_model]);
						printf("model.pos={%f,%f,%f}\n",
							model_info[select_model_type][select_model].pos[0],
							model_info[select_model_type][select_model].pos[1],
							model_info[select_model_type][select_model].pos[2]);

						printf("model.rotation={%f,%f,%f}\n",
							model_info[select_model_type][select_model].rotation[0],
							model_info[select_model_type][select_model].rotation[1],
							model_info[select_model_type][select_model].rotation[2]);

						printf("model.scale={%f,%f,%f}\n\n",
							model_info[select_model_type][select_model].scale[0],
							model_info[select_model_type][select_model].scale[1],
							model_info[select_model_type][select_model].scale[2]);
					}
					if(keystate[SDL_SCANCODE_LCTRL] && event.key.keysym.scancode==SDL_SCANCODE_S)
					{
						save_file("test.bem",number_vao,number_model,model_info);
						printf("file saved.\n");
					}





					if(event.key.keysym.scancode==SDL_SCANCODE_P)
					{
						glm_mat4_copy(model[select_model_type][number_model[select_model_type]-1],model[select_model_type][select_model]);
						model_info[select_model_type][select_model]=model_info[select_model_type][number_model[select_model_type]-1];
						number_model[select_model_type]--;

						update_model(model[select_model_type][select_model],model_info[select_model_type][select_model]);
					}

					if(event.key.keysym.scancode==SDL_SCANCODE_RSHIFT)
					{


























						number_model[select_model_type]++;
						void* tmp_ptr=model_info[select_model_type];
						model_info[select_model_type]=malloc(number_model[select_model_type]*sizeof(model_transformation));
						memcpy(model_info[select_model_type],
							tmp_ptr,
							sizeof(model_transformation)*number_model[select_model_type]);
						new_model_transformation(&model_info[select_model_type][number_model[select_model_type]-1]);

						free(tmp_ptr);





						tmp_ptr=model[select_model_type];
						model[select_model_type]=malloc(number_model[select_model_type]*sizeof(mat4));
						memcpy(model[select_model_type],
							tmp_ptr,
							sizeof(mat4)*number_model[select_model_type]);
						free(tmp_ptr);
						update_model(model[select_model_type][number_model[select_model_type]-1],model_info[select_model_type][number_model[select_model_type]-1]);






























					}
					break;
				case SDL_MOUSEWHEEL:
					if(keystate[SDL_SCANCODE_LSHIFT])
					{
						select_model_type+=event.wheel.y;
						if(number_vao<select_model_type+1)
							select_model_type=0;
						if(select_model_type<0)
							select_model_type=number_vao-1;
					}
					else if(keystate[SDL_SCANCODE_Z])
					{
						model_info[select_model_type][select_model].pos[0]+=event.wheel.y;
						update_model(model[select_model_type][select_model],model_info[select_model_type][select_model]);
					}
					else
					{
						select_model+=event.wheel.y;
						if(number_model[select_model_type]<select_model+1)
							select_model=0;
						if(select_model<0)
							select_model=number_model[select_model_type]-1;
					}

					break;
				case SDL_WINDOWEVENT:
					if(event.window.event==SDL_WINDOWEVENT_SIZE_CHANGED)
					{
						window_width = event.window.data1;
						window_height = event.window.data2;
						glViewport(0,0,window_width,window_height);
						glm_perspective(glm_rad(projection.fov),projection.aspect,projection.near,projection.far,projection.matrix);
					}
					break;
				case SDL_MOUSEMOTION:
					//camera
					SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
					camera.degres_x+=(event.motion.x-window_width/2)*-0.01;
					if(camera.degres_x>=M_PI+M_PI)
					{
						camera.degres_x-=M_PI+M_PI;
					}
					if(camera.degres_x<0)
						camera.degres_x+=M_PI+M_PI;

					vec2 normalize_vec;
					if(camera.degres_x>=0 && camera.degres_x<M_PI_2)
					{
						normalize_vec[0]=1;
						normalize_vec[1]=-tanf(camera.degres_x);
					}
					else if(camera.degres_x>=M_PI_2 && camera.degres_x<M_PI)
					{
						normalize_vec[1]=-1;
						normalize_vec[0]=-tanf(camera.degres_x-M_PI_2);
					}
					else if(camera.degres_x>=M_PI && camera.degres_x<M_PI_2+M_PI)
					{
						normalize_vec[0]=-1;
						normalize_vec[1]=tanf(camera.degres_x-M_PI);
					}
					else if(camera.degres_x>=M_PI_2+M_PI && camera.degres_x<M_PI+M_PI)
					{
						normalize_vec[1]=1;
						normalize_vec[0]=tanf(camera.degres_x-M_PI_2-M_PI);
					}

					camera.degres_y+=(event.motion.y-window_height/2)*0.01;

					if(camera.degres_y>M_PI-0.001)
						camera.degres_y=M_PI-0.01;
					if(camera.degres_y<0.01)
						camera.degres_y=0.001;

					if(camera.degres_y<M_PI_2)
						camera.center[1]=tan(M_PI_2-camera.degres_y);
					else
						camera.center[1]=-tan(camera.degres_y-M_PI_2);
					

					glm_vec2_normalize(normalize_vec);
					camera.center[0]=normalize_vec[0];
					camera.center[2]=normalize_vec[1];

					SDL_WarpMouseInWindow(window,window_width/2,window_height/2);
					SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
					break;
			}
		}

		if(movement_enable)
		{
			if(keystate[SDL_SCANCODE_W])
			{
				camera.pos[2]+=camera.center[2]*0.1;
				camera.pos[0]+=camera.center[0]*0.1;
			}
			if(keystate[SDL_SCANCODE_S])
			{
				camera.pos[2]-=camera.center[2]*0.1;
				camera.pos[0]-=camera.center[0]*0.1;
			}
			if(keystate[SDL_SCANCODE_D])
			{
				camera.pos[2]+=camera.center[0]*0.1;
				camera.pos[0]-=camera.center[2]*0.1;
			}
			if(keystate[SDL_SCANCODE_A])
			{
				camera.pos[2]-=camera.center[0]*0.1;
				camera.pos[0]+=camera.center[2]*0.1;
			}
			if(keystate[SDL_SCANCODE_SPACE])
				camera.pos[1]+=0.1;
			if(keystate[SDL_SCANCODE_LCTRL])
				camera.pos[1]-=0.1;
		}

		glm_lookat(camera.pos,ADD_VEC3(camera.pos,camera.center),(vec3){0,1,0},camera.matrix);
		//render
		glClearColor(0.5,0.5,0.5,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		for(int i=0;i<number_vao;i++)
		{
			glBindVertexArray(vao_info[i].vao);
			glUseProgram(shader_program[vao_info[i].shader]);

			glUniformMatrix4fv(camera_loc,1,GL_FALSE,*camera.matrix);
			glUniformMatrix4fv(projection_loc,1,GL_FALSE,*projection.matrix);

			for(int j=0;j<number_model[i];j++)
			{
				glUniformMatrix4fv(model_loc,1,GL_FALSE,*model[i][j]);
				glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
			}
		}

		//select render
		glUseProgram(shader_program[1]);

		glUniformMatrix4fv(camera_loc1,1,GL_FALSE,*camera.matrix);
		glUniformMatrix4fv(projection_loc1,1,GL_FALSE,*projection.matrix);
		glUniformMatrix4fv(model_loc1,1,GL_FALSE,*model[select_model_type][select_model]);

		glDrawElements(GL_LINES,6,GL_UNSIGNED_INT,0);

		SDL_GL_SwapWindow(window);
		SDL_Delay(16);
	}
	return return_value;
}
