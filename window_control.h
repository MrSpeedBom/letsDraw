#pragma once
//all geometrical meshes must be included here
#include "quad.h"
GLFWwindow* window;
unsigned int window_width=800,window_height=600;
void init_shaders()
{
    //all shaders must inisialize here
    vertex_1_shader=Shader(vertex_1_vertex_shader_data,vertex_1_fragment_shader_data);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ZERO,GL_ONE);
}
void update_shaders(float &c_time)
{
    glUniform1f(glGetUniformLocation(vertex_1_shader.ID,"current_time"),c_time);
}
GLFWwindow* initWindow(const GLuint WIDTH=800,const GLuint HEIGHT=600,bool alert=true)
{
    // Init GLFW
    window_width=WIDTH;
    window_height=HEIGHT;
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,true);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window=glfwCreateWindow(WIDTH,HEIGHT,"letsDraw",nullptr,nullptr);

    int screenWidth,screenHeight;
    glfwGetFramebufferSize(window,&screenWidth,&screenHeight);

    if(nullptr==window)
    {
        if(alert)
            std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return window;
    }

    glfwMakeContextCurrent(window);
    // Set this to true so GLEW knows to use a modern approach to retrieving
    // function pointers and extensions
    glewExperimental=GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if(GLEW_OK!=glewInit())
    {
        if(alert)
            std::cout<<"Failed to initialize GLEW"<<std::endl;
        return window;
    }

    // Define the viewport dimensions
    glViewport(0,0,screenWidth,screenHeight);
    if(alert)
        cout<<"window success!\n";

    build_list();
    init_shaders();
    global_timer.start();
    return window;
}
void start_drawing()
{
    unsigned int index;
    float wait_start=global_timer.current_time(),wait_time=0,c_time;
    float fps=60;
    Timer frame_timer;
    frame_timer.start();

    while(!glfwWindowShouldClose(window))
    {
        if(frame_timer.current_time()*fps<1)
            continue;

        c_time=global_timer.current_time();
        global_timer.pause();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
        update_shaders(c_time);

        while(!wait_queue.empty()&&wait_time<=(c_time-wait_start))
        {
            wait_time=wait_queue.front();
            wait_start=c_time;
            apply_front(c_time);
        }
        index=next_index[1];
        while(index!=1)
        {
            units_array[index].draw();
            index=next_index[index];
        }

        glfwSwapBuffers(window);
        if(wait_queue.empty()&&wait_time<=(c_time-wait_start))
            break;
        global_timer.resume();
        frame_timer.start();
    }
}
