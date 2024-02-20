#include <iostream>
using namespace std;
#include "letsDraw.h"
#include <iomanip>
int main()
{
	srand(time(0));
	window=initWindow(800,800);

    quad a[4];
    glm::vec4 col[4]={glm::vec4(1,0,0,1),glm::vec4(1,1,0,1),glm::vec4(0,1,0,1),glm::vec4(0,0,1,1)};
    float len=0.7;
    wait(1);
    for(int i=0;i<4;i++){
        a[i]=quad({((i%2)*2-1)*len/2,((i/2)*2-1)*len/2,0,0,0,0,0},len*9/10);
        a[i].set_color(col[i],0,0);
        add(a[i],.5);
        a[i].set_color(col[(i+1)%4],.5,.5);
    }

    for(int i=0;i<4;i++)
        a[i].move(glm::vec3(-((i%2)*2-1)*len*(i==1||i==2),-((i/2)*2-1)*len*(i==0||i==3),0),.5,0);
    wait(.5);
    for(int i=0;i<4;i++)
        a[i].move({-((i%2)*2-1)*len*(i==0||i==3),-((i/2)*2-1)*len*(i==1||i==2),0},.5,0);
    wait(.5);
    for(int i=0;i<4;i++)
        a[i].move({((i%2)*2-1)*len*(i==1||i==2),((i/2)*2-1)*len*(i==0||i==3),0},.5,0);
    wait(.5);
    for(int i=0;i<4;i++)
        a[i].move({((i%2)*2-1)*len*(i==0||i==3),((i/2)*2-1)*len*(i==1||i==2),0},.5,0);
    wait(.5);
    for(int i=0;i<4;i++)
    a[i].rotate({0,0,0},acos(-1)/2,.45,0);
    wait(0.5);

    for(int i=0;i<4;i++)
    a[i].rotate({0,0,0},acos(-1)/2,.45,0);
    wait(0.5);
    for(int i=0;i<4;i++)
    a[i].rotate({0,0,0},acos(-1)/2,.45,0);
    wait(0.5);
    for(int i=0;i<4;i++)
    a[i].rotate({0,0,0},acos(-1)/2,.45,0);
    wait(0.5);
    for(int i=0;i<4;i++)
    a[i].add_color({0,0,0,-1},1);
    wait(3);
    show_tree(1);xln;
    show_tree(2);xln;xln;
    start_drawing();
    show_tree(1);xln;
    show_tree(2);
}
