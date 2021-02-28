#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

#ifdef _DEBUG
# define ASSERT(x) if (!(x)) __debugbreak()
#else
# define ASSERT(x) x
#endif


#ifdef _DEBUG
# define GLCall(x) \
     GLClearError();  \
     (x); \
     ASSERT(GLCheckError(#x, __FILE__, __LINE__));
#else
# define GLCall(x) x
#endif


