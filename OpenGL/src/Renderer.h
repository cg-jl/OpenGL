#pragma once

#include <GL/glew.h>


void GLClearError();
bool GLCheckError(const char* function, const char* file, int line);

#ifdef _DEBUG
# define ASSERT(x) if (!(x)) __debugbreak()
#else
# define ASSERT(x) x
#endif


#ifdef _DEBUG
# define GLCall(x) do { GLClearError(); (x); ASSERT(GLCheckError(#x, __FILE__, __LINE__)); } while(0)
#else
# define GLCall(x) x
#endif
