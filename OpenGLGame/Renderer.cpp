#include "Renderer.h"
#include <iostream>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* func, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << error << func << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear()
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader)const
{
    GLCALL(va.Bind());
    GLCALL(shader.Bind());
    GLCALL(ib.Bind());
    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
