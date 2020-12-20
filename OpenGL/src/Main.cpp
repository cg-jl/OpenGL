#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <fstream>
#include <string>
#include <sstream>

#include <utility>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"



struct ShaderSource {
    std::string vertexSource, fragmentSource;
};


static ShaderSource parseShader(const std::string& path) {
    std::fstream stream(path);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];
    size_t commentPos;
    while (std::getline(stream, line)) {
        if (line.find("//$shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
            continue;
        }
        if (type == ShaderType::NONE) continue;
        ss[(int)type] << line << '\n';
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    // TODO: Error handling
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // i dont care about the length
    glCompileShader(id);

    // `i` -> integer
    // `v` -> vector
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        // did not compile successfully
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "\x1b[1;31mFailed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: \x1b[m" << message << std::endl;
        glDeleteShader(id);
        return 0; // shader is not valid
    }

    return id;
}

static unsigned int createShader(const ShaderSource& src) {
    // shader source code -> compiled & linked -> shader ID to bind
    unsigned int programID = glCreateProgram();
    // create two shader objects
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, src.fragmentSource);
    unsigned int vs = compileShader(GL_VERTEX_SHADER, src.vertexSource);

    // Add sources together
    glAttachShader(programID, vs);
    glAttachShader(programID, fs);

    // Link them
    glLinkProgram(programID);

    // Validate them
    glValidateProgram(programID);

    // remove intermediate (obj-like) shader
    glDeleteShader(vs);
    glDeleteShader(fs);


    // we should call glDetachShader after linked (no debug mode)

    return programID;
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */

    /* Tell GLFW that I want core profile for OpenGL */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Vsync */
    glfwSwapInterval(1);

    /* Initialize GLEW */
    if (glewInit() != GLEW_OK) {
        std::cerr << "glewInit Failed\n";
        return -1;
    }


    std::cout << glGetString(GL_VERSION) << std::endl;


    // RENDERING PIPELINE

    // 1. Send data to the GPU
    float vertices[] = {
        -0.5f, -0.5f,// 0
        0.5f, -0.5f,// 1
        0.5f, 0.5f,// 2
        -0.5f, 0.5f,// 3
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    {

        // generate buffer
        VertexArray va;
        VertexBuffer vb(vertices, 4 * 2 * sizeof(float));


        VertexBuffer::Layout layout;
        layout.push<float>(2); // 2 floats per vertex
        va.addBuffer(vb, layout);

        // index buffer
        IndexBuffer ib(indices, 6);

        // 3. Bind (set as target) data
        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        // load shader
        ShaderSource src = parseShader("res/shaders/basic.shader");
        unsigned int shader = createShader(src);

        // bind shader
        GLCall(glUseProgram(shader));

        // get location of u_color by n
        int location = glGetUniformLocation(shader, "u_color");
        // if the variable it's unused, the compiler will remove it
        ASSERT(location != -1);


        va.unbind();
        GLCall(glUseProgram(0)); // no shader
        vb.unbind();
        ib.unbind();


        float r = 0.0f;
        float increment = 0.05f;


        // vertex arrays:
        // allow to bind a vertex buffer with t

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));




            // 4. Use target data on GPU
            // order:
            // 1. Vertex shader
            //  where is the vertex ? (positions, pass data into next stage)
            // - Gets called 3 times (triangle vertices)
            // - Could be used for optimizations onto the fragment shader (as it is called much less times)
            // 2. Fragment shader
            // runs once for every pixel rasterized -> determines output color -> 
            // issue draw call (no index buffer) -> GPU
            //glDrawArrays(GL_TRIANGLES, 0, 6); // 3 attributes -> calls vertex shader 3 times, buffer 0


            // send data to shaders (uniforms)
            // Uniforms are set per draw call
            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            // use our vertex and layout data
            va.bind();
            // use our index buffer to tell OpenGL how to draw 
            ib.bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


            if (r > 1.0f) increment = -0.05f;
            else if (r < 0.0f) increment = 0.05f;
            r += increment;

            // you should enable shaders

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