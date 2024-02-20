#pragma once
#include "includes.h"
const unsigned int vertex_1_data_length=7+(7)+(7);
const unsigned int vertex_1_data_distripute_length=11;
const unsigned int vertex_1_data_distripute[ ]={3,4,1,1,1,3,1,1,1,4,1};
string vertex_1_vertex_shader_data="#version 330 core  \n"
"layout(location=0) in vec3 position;    \n"//                                                     (0-2)
"layout(location=1) in vec4 color; \n"//                                                           (3-6)
"layout(location=2) in float animation_type; \n"                               //                  (7)
//0 nothing,1 movement,2 rotation
"layout(location=3) in float animation_speed; \n"//speed of the animation                          (8)
"layout(location=4) in float animation_limit; \n"//angle of  rotation or length of movement        (9)
"layout(location=5) in vec3 animation_data; \n"//center of rotation or the direction of movement   (10-12)
"layout(location=6) in float start_time; \n"                                                   //  (13)
""
"layout(location=7) in float color_animation_type; \n"                                         // (14)
 //0 nothing,1 add value,2 set value
"layout(location=8) in float color_animation_speed; \n"                                        // (15)
"layout(location=9) in vec4  color_animation_data; \n"                                          // (16-19)
"layout(location=10) in float color_animation_start_time; \n"                                    // (20)
"uniform float current_time;\n"
"out vec4 send_color;\n"
"vec4 normalized(vec4 v){\n"
"if(v.length()!=0)return v/v.length();\n"
"return v;\n"
"}\n"
"void main(){\n"
"vec3 get_pos=position;\n"
"send_color=color;"
""
"switch(int(animation_type)){"
"case 1:{\n"
"if(animation_speed!=0){\n"
"get_pos=position+animation_data*min((current_time-start_time)*animation_speed,animation_limit);\n"
"}else{\n"
"get_pos=position+animation_data*animation_limit;"
"}"
"break;}\n"//end case 1
"case 2:{\n"
"float t=min((current_time-start_time)*animation_speed,animation_limit);\n"
"if(animation_speed==0)\n"
"t=animation_limit;\n"
"get_pos=position-animation_data;\n"
"get_pos=vec3(get_pos.x*cos(t)-get_pos.y*sin(t),get_pos.x*sin(t)+get_pos.y*cos(t),get_pos.z)+animation_data;\n"
"break;}\n"//end case 2
"}\n"//end switch
"gl_Position=vec4(get_pos,1);\n"
"switch(int(color_animation_type)){\n"
"case 1:{\n"
"if(color_animation_speed!=0){\n"
"send_color=color+normalized(color_animation_data)*min((current_time-color_animation_start_time)*color_animation_speed,color_animation_data.length());\n"
"}else{\n"
"send_color=color+color_animation_data;\n"
"}\n"
"break;}\n"
"case 2:{\n"
"if(color_animation_speed!=0){\n"
"send_color=color+normalized(color_animation_data-color)*min((current_time-color_animation_start_time)*((color_animation_data-color).length()/color_animation_speed),(color_animation_data-color).length());\n"
"}else{\n"
"send_color=color_animation_data;\n"
"}\n"
"break;}\n"
"}\n"
"}";
string vertex_1_fragment_shader_data="#version 330 core  \n"
"in vec4 send_color;\n"
"out vec4 color;\n"
"void main(){\n"
"color=send_color;\n"
"}";
Shader vertex_1_shader;
void build_vertex_1(float *p,const point &vertex)
{
	p[0]=vertex.x;
	p[1]=vertex.y;
	p[2]=vertex.z;
	p[3]=vertex.r;
	p[4]=vertex.g;
	p[5]=vertex.b;
	p[6]=vertex.a;
	for(int i=7; i<vertex_1_data_length; i++)
		p[i]=0;
}
void refresh_vertex_1(float *p,float &c_time)
{
	glm::vec3 get_pos={p[0],p[1],p[2]},animation_data={p[10],p[11],p[12]};
	float limit=0;
	switch(int(p[7])){
	case 1:
	{
	    if(p[8]!=0){
            limit=min((c_time-p[13])*p[8],p[9]);
	    }else{
            limit=p[9];
	    }
	    get_pos=get_pos+animation_data*limit;
		break;
	}
	case 2:
	{
	    if(p[8]!=0){
            limit=min((c_time-p[13])*p[8],p[9]);
	    }else{
            limit=p[9];
	    }
		get_pos=get_pos-animation_data;
		get_pos=glm::vec3{get_pos.x*cos(limit)-get_pos.y*sin(limit),get_pos.x*sin(limit)+get_pos.y*cos(limit),get_pos.z}+animation_data;
		break;
	}
	}
	p[9]-=limit;
	if(p[9]<=0){p[7]=0;p[9]=0;}
	p[0]=get_pos.x;
	p[1]=get_pos.y;
	p[2]=get_pos.z;
	//here ends refreshing of movement animation
    glm::vec4 get_color=glm::vec4{p[3],p[4],p[5],p[6]},color_animation_data={p[16],p[17],p[18],p[19]};

    switch(int(p[14])){
    case 1:{
    if(p[15]!=0)
        {
            get_color=get_color+normalized(color_animation_data)*min((c_time-p[20])*p[15],(float)color_animation_data.length());
            color_animation_data-=normalized(color_animation_data)*min((c_time-p[20])*p[15],(float)color_animation_data.length());
        }
        else
        {
            get_color+=color_animation_data;
            color_animation_data={0,0,0,0};
        }
        if(color_animation_data==get_color)
        {
            p[14]=0;
        }
    break;
    }
    case 2:{
        if(p[15]!=0)
        {
        get_color=get_color+(color_animation_data-get_color)*min((c_time-p[20])*((color_animation_data-get_color).length()/p[15]),(float)(color_animation_data-get_color).length());
        }
        else
        {
            get_color=color_animation_data;
        }
        if(color_animation_data==get_color)
        {
            p[14]=0;
        }
    break;
    }
    }


    p[3]=get_color.r,p[4]=get_color.g,p[5]=get_color.b,p[6]=get_color.a;
    p[16]=color_animation_data.r,p[17]=color_animation_data.g,p[18]=color_animation_data.b,p[19]=color_animation_data.a;

}
void apply_animation_1(float *p,float *animation_data,float &c_time)
{
// TODO (10Home#2#): applying linear animations must combine last animation with the current one ...
//
	refresh_vertex_1(p,c_time);
	if(animation_data[7]!=0){
	for(int i=7; i<14; i++)
	{
		p[i]=animation_data[i];
	}
	p[13]=c_time;
	}
	if(animation_data[14]!=0){
	for(int i=14; i<21; i++)
	{
		p[i]=animation_data[i];
	}
	p[20]=c_time;
	}
}

