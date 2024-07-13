#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Renderer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    if (glewInit() != GLEW_OK)
    {
        printf("Failed Initialize");
    }
    {
        float vertex[] =
        {
            -0.5f, -0.5f, 0.0f, 0.0f,//0
             0.5f, -0.5f, 1.0f, 0.0f,//1
             0.5f,  0.5f, 1.0f, 1.0f,//2
            -0.5f,  0.5f, 0.0f, 1.0f //3
        };

        unsigned int indices[] = 
        {
            0,1,2,
            2,3,0
        };

        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCALL(glEnable(GL_BLEND));

        VertexArray va;
        VertexBuffer vb(vertex, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push(GL_FLOAT,2);
        layout.Push(GL_FLOAT, 2);
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);
        Shader shader("BasicShader.shader");
        shader.Bind();

        Renderer renderer;

        glm::mat4 proj = glm::ortho(-2.0, 2.0, -1.5, 1.5, -1.0, 1.0);

        Texture texture("res/Jacketboy.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniformMat4f("u_MVP", proj);

        shader.Unbind();
        va.Unbind();
        vb.Unbind();
        ib.Unbind();


        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            //shader.SetUniform4f("u_Color", r, 0.2f, 0.4f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;



            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}
