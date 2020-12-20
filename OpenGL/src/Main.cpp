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
#include "Shader.h"





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
        Shader shader("res/shaders/basic.shader");
        shader.bind();


        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();


        float r = 0.0f;
        float increment = 0.05f;


        // vertex arrays:
        // allow to bind a vertex buffer with t

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            shader.setUniform4f("u_color", r, 0.3f, 0.8f, 1.0f);

            shader.bind();
            va.bind();
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
    }

    glfwTerminate();
    return 0;
}