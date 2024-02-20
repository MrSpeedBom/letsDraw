
#pragma once
#include "main_memory.h"
// TODO (10Home#3#): implement the following functions  ...
//.set_color(color)  push set color animation of the quad
//

class quad{
	unsigned int id=0;
public:
    unsigned int get_id(){
        return id;
    };
    unsigned int size(){
        return 4*vertex_1_data_length;
    }
	quad()
	{}
	~quad() {
		if(id)
			remove_unit(id);
	}
	quad(point a,point b,point c,point d){
		vector<float> VBO_DATA(4*vertex_1_data_length);
		build_vertex_1(&VBO_DATA[0],a);
		build_vertex_1(&VBO_DATA[vertex_1_data_length],b);
		build_vertex_1(&VBO_DATA[2*vertex_1_data_length],c);
		build_vertex_1(&VBO_DATA[3*vertex_1_data_length],d);
		vector<GLuint> elements={0, 1, 2,0,2,3};
		GLuint VBO,VAO,EBO;
		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glGenBuffers(1,&EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,VBO_DATA.size()*sizeof(VBO_DATA[0]),&VBO_DATA[0],GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,elements.size()*sizeof(elements[0]),&elements[0],GL_STATIC_DRAW);
		unsigned int prefix=0;
		for(int i=0; i<vertex_1_data_distripute_length; i++)
		{
			glVertexAttribPointer(i,vertex_1_data_distripute[i],GL_FLOAT,false,vertex_1_data_length*sizeof(float),(void*)(prefix*sizeof(float)));
			prefix+=vertex_1_data_distripute[i];
		}
		for(int i=0;i<vertex_1_data_distripute_length;i++)
			glEnableVertexAttribArray(i);
		glBindVertexArray(0);
		id=insert_unit({VAO,VBO,EBO,4*vertex_1_data_length,6,vertex_1_shader.ID});

	}
	quad(point center,double side_length=1):
		quad(center+point{-side_length/2,side_length/2},center+point{side_length/2,side_length/2},center+point{side_length/2,-side_length/2},center+point{-side_length/2,-side_length/2})
	{}
	quad(point center,double height,double width):
		quad(center+point{-width/2,height/2},center+point{width/2,height/2},center+point{width/2,-height/2},center+point{-width/2,-height/2})
	{ }
	void operator=(const quad &sq)
	{
		float buffer[vertex_1_data_length*4];
		glBindBuffer(GL_ARRAY_BUFFER,units_array[sq.id].VBO);
		glGetBufferSubData(GL_ARRAY_BUFFER,0,(vertex_1_data_length*4)*sizeof(float),buffer);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		vector<GLuint> elements={0, 1, 2,0,2,3};
		GLuint VBO,VAO,EBO;
		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);
		glGenBuffers(1,&EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,(vertex_1_data_length*4)*sizeof(float),buffer,GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,elements.size()*sizeof(elements[0]),&elements[0],GL_STATIC_DRAW);
		unsigned int prefix=0;
		for(int i=0; i<vertex_1_data_distripute_length; i++)
		{
			glVertexAttribPointer(i,vertex_1_data_distripute[i],GL_FLOAT,false,vertex_1_data_length*sizeof(float),(void*)(prefix*sizeof(float)));
			prefix+=vertex_1_data_distripute[i];
		}
		for(int i=0; i<vertex_1_data_distripute_length; i++)
			glEnableVertexAttribArray(i);
		glBindVertexArray(0);

        if(!id)
            id=insert_unit({VAO,VBO,EBO,4*vertex_1_data_length,6,vertex_1_shader.ID});
		else{
            units_array[id].clear();
            units_array[id]={VAO,VBO,EBO,4*vertex_1_data_length,6,vertex_1_shader.ID};
		}
	}
	void rotate(point center,float angle,float animation_time=1,float wait=0)
	{
		vector<float> cub(4*vertex_1_data_length);

		glBindBuffer(GL_ARRAY_BUFFER,units_array[id].VBO);
		glGetBufferSubData(GL_ARRAY_BUFFER,0,(vertex_1_data_length*4)*sizeof(float),&cub[0]);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		float c_time=global_timer.current_time(),speed=angle;
		if(animation_time!=0){
            speed/=animation_time;
		}else speed=0;
		for(int vertex=0; vertex<4; vertex++)
		{
			cub[vertex_1_data_length*vertex+7]=2;
			cub[vertex_1_data_length*vertex+8]=speed;
			cub[vertex_1_data_length*vertex+9]=angle;
			cub[vertex_1_data_length*vertex+10]=center.x;
			cub[vertex_1_data_length*vertex+11]=center.y;
			cub[vertex_1_data_length*vertex+12]=center.z;
		}
		push_to_queue(id,cub,wait);
	}
	void move(glm::vec3 vec,float animation_time=1,float wait=0)
	{
		vector<float> cub(4*vertex_1_data_length);
		glBindBuffer(GL_ARRAY_BUFFER,units_array[id].VBO);
		glGetBufferSubData(GL_ARRAY_BUFFER,0,(vertex_1_data_length*4)*sizeof(float),&cub[0]);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		float length=vec.length(),c_time=global_timer.current_time(),speed=length;
		if(vec.length()>0)
		vec=vec/length;
		if(animation_time!=0)
        speed/=animation_time;
        else speed=0;
		for(int vertex=0; vertex<4; vertex++)
		{
			cub[vertex_1_data_length*vertex+7]=1;
			cub[vertex_1_data_length*vertex+8]=speed;
			cub[vertex_1_data_length*vertex+9]=length;
			cub[vertex_1_data_length*vertex+10]=vec.x;
			cub[vertex_1_data_length*vertex+11]=vec.y;
			cub[vertex_1_data_length*vertex+12]=vec.z;
		}
		push_to_queue(id,cub,wait);
	}
	void add_color(glm::vec4 target_color,float animation_time=1,float wait=0)
	{
		vector<float> cub(4*vertex_1_data_length);
		glBindBuffer(GL_ARRAY_BUFFER,units_array[id].VBO);
		glGetBufferSubData(GL_ARRAY_BUFFER,0,(vertex_1_data_length*4)*sizeof(float),&cub[0]);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		float c_time=global_timer.current_time(),speed=target_color.length();
		if(animation_time==0)speed=0;
		else speed/=animation_time;
		for(int vertex=0; vertex<4; vertex++)
		{

			cub[vertex_1_data_length*vertex+14]=1;
			cub[vertex_1_data_length*vertex+15]=speed;
			cub[vertex_1_data_length*vertex+16]=target_color.r;
			cub[vertex_1_data_length*vertex+17]=target_color.g;
			cub[vertex_1_data_length*vertex+18]=target_color.b;
			cub[vertex_1_data_length*vertex+19]=target_color.a;
		}
		push_to_queue(id,cub,wait);
	}
	void set_color(glm::vec4 target_color,float animation_time=1,float wait=0){
	    vector<float> cub(4*vertex_1_data_length);
		glBindBuffer(GL_ARRAY_BUFFER,units_array[id].VBO);
		glGetBufferSubData(GL_ARRAY_BUFFER,0,(vertex_1_data_length*4)*sizeof(float),&cub[0]);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		float c_time=global_timer.current_time();
		for(int vertex=0; vertex<4; vertex++)
		{
			cub[vertex_1_data_length*vertex+14]=2;
			cub[vertex_1_data_length*vertex+15]=animation_time;
			cub[vertex_1_data_length*vertex+16]=target_color.r==-1?cub[vertex_1_data_length*vertex+3]:target_color.r;
			cub[vertex_1_data_length*vertex+17]=target_color.g==-1?cub[vertex_1_data_length*vertex+4]:target_color.g;
			cub[vertex_1_data_length*vertex+18]=target_color.b==-1?cub[vertex_1_data_length*vertex+5]:target_color.b;
			cub[vertex_1_data_length*vertex+19]=target_color.a==-1?cub[vertex_1_data_length*vertex+6]:target_color.a;
		}
		push_to_queue(id,cub,wait);
	}
	void fade_out(float animation_time=1,float wait=0){
	    set_color({-1,-1,-1,0},animation_time,wait);
    }
    void fade_in(float animation_time=1,float wait=0){
	    set_color({-1,-1,-1,1},animation_time,wait);
    }



};


