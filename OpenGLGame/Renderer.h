#pragma once
#include <GL/glew.h>
#include "Renderer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"


#define ASSERT(x) if (!(x)) __debugbreak()
#define GLCALL(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* func, const char* file, int line);

class Renderer
{
private:


public:

	void Clear();
	void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};
