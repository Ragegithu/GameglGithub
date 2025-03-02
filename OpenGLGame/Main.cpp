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
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

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
    window = glfwCreateWindow(960, 540, "Renderer", NULL, NULL);
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
            -50.0f,-50.0f, 0.0f, 0.0f,//0
             50.0f,-50.0f, 1.0f, 0.0f,//1
             50.0f, 50.0f, 1.0f, 1.0f,//2
            -50.0f, 50.0f, 0.0f, 1.0f //3
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

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window,true);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);
        glm::mat4 proj = glm::ortho(0.0f,960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));


        Texture texture("res/Jacketboy.png");
        texture.Bind(0);

        shader.Unbind();
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            shader.Bind();
       {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
       } 
       {
           glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
           glm::mat4 mvp = proj * view * model;
           shader.SetUniformMat4f("u_MVP", mvp);
           renderer.Draw(va, ib, shader);
       }
            ImGui::SliderFloat3("TranslationJacket", &translationA.x, 0.0f, 960.0f);
            ImGui::SliderFloat3("TranslationWaaa", &translationB.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
