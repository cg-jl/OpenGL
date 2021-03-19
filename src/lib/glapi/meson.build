glapi_cc = [
  'IndexBuffer.cc',
  'Renderer.cc',
  'Shader.cc',
  'Texture.cc',
  'VertexArray.cc',
  'VertexBuffer.cc'
]
glapi = library('glapi', sources : glapi_cc, include_directories : incdirs,  dependencies : [ stb_image_dep, dependency('glew'), dependency('gl')])
glapi_dep = declare_dependency(link_with : [glapi], )