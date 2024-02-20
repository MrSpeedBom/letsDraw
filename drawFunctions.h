#pragma once
#include "quad.h"
template <typename T>
void add(T& mesh,float wait=0)
{
    animation_queue.push({0,{1,mesh.get_id()}});
	wait_queue.push(wait);
}
template <typename T>
void remove(T& mesh,float wait=0)
{
    animation_queue.push({0,{2,mesh.get_id()}});
	wait_queue.push(wait);
}
void wait(float t)
{
	animation_queue.push({0,{0}});
	wait_queue.push(t);
}
