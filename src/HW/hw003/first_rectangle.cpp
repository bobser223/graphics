//
// Created by Volodymyr Avvakumov on 11.04.2026.
//
#include <iostream>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ranges>
#include <vector>

#include "reader.h"
#include "creator.h"


template<typename T>
std::vector<T> concatenateVectors(const std::vector<std::vector<T>>& vectors) {
    std::vector<T> result;
    result.reserve(std::accumulate(vectors.begin(), vectors.end(), 0, [](int sum, const std::vector<T>& v) { return sum + v.size(); }));
    for (const auto& v : vectors) {result.insert(result.end(), v.begin(), v.end());}
    return result;
}

std::vector<unsigned int> concatenateIndices(
    const std::vector<std::vector<unsigned int>>& indices,
    int vertices_per_shape
) {
    std::vector<unsigned int> result;

    result.reserve(
        std::accumulate(
            indices.begin(),
            indices.end(),
            size_t{0},
            [](size_t sum, const std::vector<unsigned int>& v) {
                return sum + v.size();
            }
        )
    );

    for (unsigned int shape_idx = 0; shape_idx < indices.size(); shape_idx++) {
        unsigned int vertex_offset = shape_idx * vertices_per_shape;

        for (unsigned int vertex_index : indices[shape_idx]) {
            result.push_back(vertex_index + vertex_offset);
        }
    }

    return result;
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
    window = glfwCreateWindow(640, 480, "Hello 3 Interpolated Rectangles", NULL, NULL);
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
        PROJECT_PATH /"src"/"HW"/ "hw002" / "triangle.vert"
    );

    GLuint fragment_shader = createShaderFromFile(
        GL_FRAGMENT_SHADER,
        PROJECT_PATH /"src"/"HW"/ "hw002" / "foo.frag");


    // Програма з шейдерів
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);





    // x, y, r, g, b

    std::vector<float> rectangle1_vertices = {
        // Rectangle 1 — left, warm gradient
        -0.9f, -0.5f,  1.0f, 0.2f, 0.1f,   // 0 left bottom — red/orange
        -0.4f, -0.5f,  1.0f, 0.8f, 0.0f,   // 1 right bottom — orange/yellow
        -0.9f,  0.1f,  0.8f, 0.0f, 0.2f,   // 2 left top — dark red

        -0.4f,  0.1f,  1.0f, 1.0f, 0.2f,   // 3 right top — yellow
    };

    std::vector<unsigned int> rectangle1_indices = {
        0, 1, 2,   // first triangle
        3, 1, 2    // second triangle
    };

    std::vector<float> rectangle2_vertices = {
        // Rectangle 2 — center, cold gradient
        -0.25f, -0.2f,  0.0f, 0.8f, 1.0f,  // 0 left bottom — cyan
         0.25f, -0.2f,  0.0f, 0.2f, 1.0f,  // 1 right bottom — blue
        -0.25f,  0.45f, 0.2f, 1.0f, 0.7f,  // 2 left top — mint

         0.25f,  0.45f, 0.4f, 0.0f, 1.0f,  // 3 right top — violet-blue

    };

    std::vector<unsigned int> rectangle2_indices = {
        0, 1, 2,   // first triangle
        3, 1, 2    // second triangle
    };

    std::vector<float> rectangle3_vertices = {
        // Rectangle 3 — right, purple/green gradient
        0.45f, -0.7f,  0.7f, 0.0f, 1.0f,  // 0 left bottom — purple
        0.9f,  -0.7f,  0.1f, 1.0f, 0.3f,  // 1 right bottom — green
        0.45f, -0.05f, 1.0f, 0.0f, 0.8f,  // 2 left top — pink

        0.9f,  -0.05f, 0.8f, 1.0f, 0.1f,  // 3 right top — lime
   };

    std::vector<unsigned int> rectangle3_indices = {
        0, 1, 2,   // first triangle
        3, 1, 2    // second triangle
    };



    std::vector<float> vertices = concatenateVectors<float>({
    rectangle1_vertices,
    rectangle2_vertices,
    rectangle3_vertices
});

    const unsigned int vertices_per_rectangle = 4;

    std::vector<unsigned int> indices = concatenateIndices(
        {rectangle1_indices, rectangle2_indices, rectangle3_indices},
        vertices_per_rectangle
    );

    const size_t floats_per_vertex = 5;        // x, y, r, g, b
    const size_t position_components = 2;      // x, y
    const size_t color_components = 3;         // r, g, b

    const size_t vertex_stride_bytes = floats_per_vertex * sizeof(float);
    const size_t vertex_buffer_size_bytes = vertices.size() * sizeof(float);
    const size_t index_buffer_size_bytes = indices.size() * sizeof(unsigned int);

    const size_t total_float_count = vertices.size();
    const size_t total_vertex_count = vertices.size() / floats_per_vertex;
    const size_t total_index_count = indices.size();
    // float vertices[vertex_count*values_per_vertex] = {
    //
    //
    // };

    GLuint VBO; // data - ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object
    GLuint EBO; // index buffer


    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind = activate
    glBufferData(
        GL_ARRAY_BUFFER,
        vertex_buffer_size_bytes,
        vertices.data(),
        GL_STATIC_DRAW
    );
    // EBO / index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        index_buffer_size_bytes,
        indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,                  // location - 0
        position_components,                  // 2 компоненти: x, y
        GL_FLOAT,           // тип даних
        GL_FALSE,           // не нормалізувати
        vertex_stride_bytes,  // stride: 5 float-а на вершину (кількість елементів в імпровізованому рядку verticies)
        (void*)0            // offset: починаємо з 0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,                              // location 1 for vertex shader
        color_components,                              // r, g, b
        GL_FLOAT,
        GL_FALSE,
        vertex_stride_bytes,
        (void*)(position_components * sizeof(float))     // після x, y
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
        glBindVertexArray(VAO);

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(total_index_count),
            GL_UNSIGNED_INT,
            0
        );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();
    return 0;
}