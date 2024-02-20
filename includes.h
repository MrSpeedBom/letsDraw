#pragma once
#define xout cout<<" "
#define xln cout<<"\n"

#include <iostream>
#include <math.h>
#include <vector>
#include <queue>
#include <glm/glm.hpp>
using namespace std;
#define GLEW_STATIC
#include <GL/glew.h>
#include "shader_s.h"
#include "Timer.h"
struct point
{
    float x,y,z;
    float r=1,g=1,b=1,a=1;
    point operator+(point p)
    {
        return point{x+p.x,y+p.y,z+p.z,r,g,b,a};
    }
    point operator-(point p)
    {
        return point{x-p.x,y-p.y,z-p.z,r,g,b,a};
    }
    point operator*(point p)
    {
        return point{x*p.x,y*p.y,z*p.z,r,g,b,a};
    }
    point operator/(float f)
    {
        return point{x/f,y/f,z/f,r,g,b,a};
    }
    point operator*(float f)
    {
        return point{x*f,y*f,z*f,r,g,b,a};
    }
    point operator-()
    {
        return point{-x,-y,-z,r,g,b,a};
    }
    float length()
    {
        return sqrt(x*x+y*y+z*z);
    }
    void out()
    {
        cout<<x<<" "<<y<<" "<<z<<" "<<r<<" "<<g<<" "<<b<<" "<<endl;
    }
};
template <typename T>
T normalized(T v){
if(v.length()==0)return v;
return v/float(v.length());
}
struct unit
{
    unsigned int VAO,VBO,EBO,VBO_SIZE=0,EBO_SIZE,shader_program;
    void draw()
    {
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,EBO_SIZE,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }
    void clear()
    {
        glDeleteBuffers(1,&VBO);
        glDeleteBuffers(1,&EBO);
        glDeleteVertexArrays(1,&VAO);
    }
};
