#include <cassert>
void GLClearError();
bool GLCheckError(const char *function, const char *file, int line);

#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) x
#endif

#ifdef _DEBUG
#define GLCall(x)                                                              \
  GLClearError();                                                              \
  (x);                                                                         \
  ASSERT(GLCheckError(#x, __FILE__, __LINE__));
#else
#warning "compiling without _DEBUG"
#define GLCall(x) x
#endif
