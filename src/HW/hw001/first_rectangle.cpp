//
// Created by Volodymyr Avvakumov on 11.04.2026.
//
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../utils/shader_utils/reader.h"
#include "../utils/shader_utils/creator.h"

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
    window = glfwCreateWindow(640, 480, "Hello Interpolated Rectangle", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLuint vertex_shader = createShaderFromFile(
        GL_VERTEX_SHADER,
        PROJECT_PATH / "hw001" / "triangle.vert"
    );

    GLuint fragment_shader = createShaderFromFile(
        GL_FRAGMENT_SHADER,
        PROJECT_PATH / "hw001" / "foo.frag");


    // Програма з шейдерів
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    const size_t vertex_count = 6;
    const size_t values_per_vertex = 5;
    const size_t vertex_size = values_per_vertex * sizeof(float);
    float vertices[vertex_count*values_per_vertex] = {
        // x,     y,     r,    g,    b
        -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   // ліва нижня — червона
         0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   // права нижня — синя
         -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,    // ліва верхня — жовта

        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, //права верхня - зелена
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // права нижня - синя
        -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, // ліва верхня  — жовта

    };

    GLuint VBO; // data - ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind = activate
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // location - 0
        2,                  // 2 компоненти: x, y
        GL_FLOAT,           // тип даних
        GL_FALSE,           // не нормалізувати
        5 * sizeof(float),  // stride: 5 float-а на вершину (кількість елементів в імпровізованому рядку verticies)
        (void*)0            // offset: починаємо з 0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,                              // location 1 for vertex shader
        3,                              // r, g, b
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(2 * sizeof(float))      // після x, y
    );
    glEnableVertexAttribArray(1);




    glBindVertexArray(0); // деактивувати VAO




    /* Loop until the user closes the window */
    do
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();
    return 0;
}