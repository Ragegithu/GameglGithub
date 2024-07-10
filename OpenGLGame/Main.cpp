#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource 
{
    std::string VertexSource;
    std::string FragmentSource;
};
static ShaderProgramSource parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return{ ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(const std::string& source, unsigned int type) 
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id));

    //shader error handling
    int result; 
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        printf(message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
            -0.5f, -0.5f, //0
             0.5f, -0.5f, //1
             0.5f,  0.5f, //2
            -0.5f,  0.5f  //3
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };

        VertexArray va;
        VertexBuffer vb(vertex, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push(GL_FLOAT,2);
        va.AddBuffer(vb, layout);
        IndexBuffer ib(indices, 6);


        ShaderProgramSource source = parseShader("BasicShader.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        glUseProgram(shader);


        GLCALL(int color_location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(color_location != -1);

        glUseProgram(0);
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;

            glUseProgram(shader);

            glUniform4f(color_location, r, 0.6f, 1.0f, 1.0f);


            va.Bind();
            ib.Bind();

            GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
}
