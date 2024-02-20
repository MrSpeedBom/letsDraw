#pragma once
#include "includes.h"
#include "VERTEX_INFO.h"
Timer global_timer;
//total memory taken 24*(MAXMESH+4)+4*2*(MAX_MESH+4)

// TODO (10Home#1#): implement a build function  ...
//change the arrays to vectors
//

const int MAX_MESH=10000;
unit units_array[MAX_MESH+4];
unsigned int next_index[MAX_MESH+4],prev_index[MAX_MESH+4];
bool full_index[MAX_MESH+4];
unsigned int list_size=0;
void build_list() {
	for(int i=3; i<MAX_MESH+4; i++)
		next_index[i]=i+1,prev_index[i]=i-1;
	prev_index[3]=MAX_MESH+4-1;
	next_index[MAX_MESH+4-1]=3;
	for(int i=0; i<3; i++)
		next_index[i]=prev_index[i]=i;
	//does not update later
	// 1,2,3 are roots for three linked listes
	//1 is for drawn objects
	//2 is for inactive objects
	//3 is for empty indices
	//0 represents null index (for error handling)
}
unsigned int insert_unit(unit u) {
	if(list_size==MAX_MESH)
	{
		cout<<"max mesh size reached:"<<MAX_MESH<<"\n";
		return 0;
	}
	list_size++;
	unsigned int id=next_index[3];
	next_index[prev_index[id]]=next_index[id];
	prev_index[next_index[id]]=prev_index[id];
	next_index[id]=next_index[2];
	prev_index[id]=2;
	prev_index[next_index[id]]=id;
	next_index[prev_index[id]]=id;
	full_index[id]=1;
	units_array[id]=u;
	return id;
}
void activate_unit(unsigned int id)
{
	if(!full_index[id])
	{
		cout<<"empty object can't be activated!\n";
		return;
	}
	next_index[prev_index[id]]=next_index[id];
	prev_index[next_index[id]]=prev_index[id];
	next_index[id]=1;
	prev_index[id]=prev_index[1];
	prev_index[next_index[id]]=id;
	next_index[prev_index[id]]=id;
}
void deactivate_unit(unsigned int id)
{
	if(!full_index[id])
	{
		cout<<"empty object can't be deactivated!\n";
		return;
	}
	next_index[prev_index[id]]=next_index[id];
	prev_index[next_index[id]]=prev_index[id];
	next_index[id]=next_index[2];
	prev_index[id]=2;
	prev_index[next_index[id]]=id;
	next_index[prev_index[id]]=id;
}
void remove_unit(unsigned int id)
{
	if(!full_index[id])
	{
		cout<<"empty object can't be removed!\n";
		return;
	}
	list_size--;
	next_index[prev_index[id]]=next_index[id];
	prev_index[next_index[id]]=prev_index[id];
	next_index[id]=next_index[3];
	prev_index[id]=3;
	prev_index[next_index[id]]=id;
	next_index[prev_index[id]]=id;
	units_array[id].clear();
	full_index[id]=0;
}
void show_tree(unsigned int root)
{
	unsigned int END=root;
	do
	{
		xout<<root;
		root=next_index[root];
	}while(root!=END);
	xln;
	do
	{
		xout<<next_index[root];
		root=next_index[root];
	}
	while(root!=END);
	xln;
	do
	{
		xout<<prev_index[root];
		root=next_index[root];
	}
	while(root!=END);
	xln;
}
queue<pair<unsigned int,vector<float>>> animation_queue;
queue<vector<unsigned int>> change_to_current_time_queue;
queue<float> wait_queue;

void push_to_queue(unsigned int id,const vector<float> &buffer,float wait=0)
{
	animation_queue.push({id,buffer});
	wait_queue.push(wait);
}
void apply_front(float &c_time) {
	if(animation_queue.size()==0)
	{
		cout<<"can't apply animation (queue is empty)!\n";
		return;
	}
	pair<unsigned int,vector<float>> aniation_buffer=animation_queue.front();
	if(aniation_buffer.first==0)//here goes animation not related to any mesh
	{
	    switch(int(aniation_buffer.second[0])){
	        case 1:{
	            activate_unit(aniation_buffer.second[1]);
	            break;
	        }
	        case 2:{
	            deactivate_unit(aniation_buffer.second[1]);
	            break;
	        }
	    }
		animation_queue.pop();
		wait_queue.pop();
		return;
	}
	vector<float> current_buffer(units_array[aniation_buffer.first].VBO_SIZE);
	glBindBuffer(GL_ARRAY_BUFFER,units_array[aniation_buffer.first].VBO);
	glGetBufferSubData(GL_ARRAY_BUFFER,0,(units_array[aniation_buffer.first].VBO_SIZE)*sizeof(float),&current_buffer[0]);
	for(int i=0; i<units_array[aniation_buffer.first].VBO_SIZE; i+=vertex_1_data_length)
	{
		apply_animation_1(&current_buffer[i],&aniation_buffer.second[i],c_time);
	}
	glBufferSubData(GL_ARRAY_BUFFER,0,(units_array[aniation_buffer.first].VBO_SIZE)*sizeof(float),&current_buffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	animation_queue.pop();
	wait_queue.pop();
}
