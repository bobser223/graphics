//
// Created by Volodymyr Avvakumov on 11.04.2026.
//
#include <iostream>
#include <numeric>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ranges>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture_creator.h"


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






struct ModelOperator {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;


    ModelOperator(glm::mat4 model, glm::mat4 view, glm::mat4 projection) : model(model), view(view), projection(projection) {}
    ~ModelOperator() = default;
    glm::mat4 get_model() const {return model;}
    glm::mat4 get_view() const {return view;}
    glm::mat4 get_projection() const {return projection;}


    void rotate(float angle, glm::vec3 axis) {
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle, axis);
        this->model = rotation_matrix * model;
    }

    void translate(glm::vec3 translation) {
        this->model = glm::translate(model, translation);
    }

    void clear_model() {
        this->model = glm::mat4(1.0f);
    }
};

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
        PROJECT_PATH /"src"/"HW"/ "hw004"/ "cube.vert"
    );

    GLuint fragment_shader = createShaderFromFile(
        GL_FRAGMENT_SHADER,
        PROJECT_PATH /"src"/"HW"/ "hw004"/ "cube.frag");


    // Програма з шейдерів
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex_shader);
    glAttachShader(shaderProgram, fragment_shader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLuint texture1 = createTextureFromFile(
        PROJECT_PATH / "src" / "HW" / "data" / "hw003" / "texture1.jpeg"
    );
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
    GLint model_location = glGetUniformLocation(shaderProgram, "model");
    GLint view_location = glGetUniformLocation(shaderProgram, "view");
    GLint projection_location = glGetUniformLocation(shaderProgram, "projection");







    // x, y, r, g, b




    const std::vector<float> vertices = {
        // -------- Передня грань (червона) --------
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, /* текстурні координати */ 0.0, 0.0,  // лівий нижній
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, /* текстурні координати */ 1.0, 0.0,  // правий нижній
        1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f, /* текстурні координати */ 1.0, 1.0,  // правий верхній
        0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f, /* текстурні координати */ 0.0, 1.0,  // лівий верхній

       // -------- Задня грань (зелена) --------
        0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   /* текстурні координати */ 0.0, 0.0,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   /* текстурні координати */ 1.0, 0.0,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   /* текстурні координати */ 1.0, 1.0,
        0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   /* текстурні координати */ 0.0, 1.0,

       // -------- Ліва грань (синя) --------
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,/* текстурні координати */ 0.0, 0.0,
        0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,/* текстурні координати */ 1.0, 0.0,
        0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,/* текстурні координати */ 1.0, 1.0,
        0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,/* текстурні координати */ 0.0, 1.0,

       // -------- Права грань (жовта) --------
        1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   /* текстурні координати */ 0.0, 0.0,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   /* текстурні координати */ 1.0, 0.0,
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 0.0f,   /* текстурні координати */ 1.0, 1.0,
        1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 0.0f,   /* текстурні координати */ 0.0, 1.0,

       // -------- Нижня грань (бірюзова) --------
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,   /* текстурні координати */ 0.0, 0.0,
        0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   /* текстурні координати */ 1.0, 0.0,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   /* текстурні координати */ 1.0, 1.0,
        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,   /* текстурні координати */ 0.0, 1.0,

       // -------- Верхня грань (фіолетова) --------
        0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f,    /* текстурні координати */ 0.0, 0.0,
        1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f,    /* текстурні координати */ 1.0, 0.0,
        1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,    /* текстурні координати */ 1.0, 1.0,
        0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 1.0f,    /* текстурні координати */ 0.0, 1.0,
   };

    const std::vector<unsigned int> indices = {
        0, 1, 2,  2, 3, 0,       // передня
        4, 5, 6,  6, 7, 4,       // задня
        8, 9,10, 10,11, 8,       // ліва
       12,13,14, 14,15,12,       // права
       16,17,18, 18,19,16,       // нижня
       20,21,22, 22,23,20        // верхня
    };

    constexpr unsigned int vertices_per_rectangle = 4;

    constexpr size_t floats_per_vertex = 8;        // x, y, z, r, g, b, u, v
    constexpr size_t position_components = 3;      // x, y, z
    constexpr size_t color_components = 3;
    constexpr size_t texture_components = 2;

    constexpr size_t position_offset_floats = 0;
    constexpr size_t color_offset_floats = 3;
    constexpr size_t texture_offset_floats = 6;

    constexpr size_t vertex_stride_bytes = floats_per_vertex * sizeof(float);
    const size_t vertex_buffer_size_bytes = vertices.size() * sizeof(float);
    const size_t index_buffer_size_bytes = indices.size() * sizeof(unsigned int);

    const size_t total_float_count = vertices.size();
    const size_t total_vertex_count = vertices.size() / floats_per_vertex;
    const size_t total_index_count = indices.size();


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

    // texture coordinates: u, v
    glVertexAttribPointer(
        2,
        texture_components,
        GL_FLOAT,
        GL_FALSE,
        vertex_stride_bytes,
        (void*)(texture_offset_floats * sizeof(float))
    );
    glEnableVertexAttribArray(2);




    glBindVertexArray(0); // деактивувати VAO





    bool animation_paused = false;
    bool space_was_pressed = false;
    float paused_angle = 0.0f;
    float animation_time = 0.0f;
    auto last_frame_time = static_cast<float>(glfwGetTime());


    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 1.5f, 3.0f),   // позиція камери
        glm::vec3(0.0f, 0.0f, 0.0f),   // куди дивиться
        glm::vec3(0.0f, 1.0f, 0.0f)    // up
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        640.0f / 480.0f,
        0.1f,
        100.0f
    );


    auto model_operator = ModelOperator(glm::mat4(1.0f), view, projection);




    glm::vec3 position = glm::vec3(0.0f);
    float angle = 0.0f;
    glm::vec3 scale = glm::vec3(1.0f);

    float move_speed = 1.5f;
    float rotation_speed = glm::radians(180.0f);



    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    do
    {
        auto current_frame_time = static_cast<float>(glfwGetTime());
        float delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        // ---------- keyboard movement ----------
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            position.y += move_speed * delta_time;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            position.y -= move_speed * delta_time;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            position.x -= move_speed * delta_time;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            position.x += move_speed * delta_time;
        }

        // ---------- mouse hover ----------
        double mouse_x;
        double mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);

        int window_width;
        int window_height;
        glfwGetWindowSize(window, &window_width, &window_height);

        float mouse_ndc_x = static_cast<float>(2.0 * mouse_x / window_width - 1.0);
        float mouse_ndc_y = static_cast<float>(1.0 - 2.0 * mouse_y / window_height);

        float half_width = 0.5f;
        float half_height = 0.3f;

        bool is_mouse_over_rectangle =
            mouse_ndc_x >= position.x - half_width &&
            mouse_ndc_x <= position.x + half_width &&
            mouse_ndc_y >= position.y - half_height &&
            mouse_ndc_y <= position.y + half_height;

        if (is_mouse_over_rectangle) {
            angle += rotation_speed * delta_time;
        }


        // model = glm::rotate(
        //     model,
        //     angle,
        //     glm::vec3(0.0f, 1.0f, 0.0f)
        // );

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);

        model = glm::rotate(
            model,
            angle,
            glm::vec3(0.0f, 0.0f, 1.0f)
        );






        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(indices.size()),
            GL_UNSIGNED_INT,
            nullptr
        );

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));


    glDeleteTextures(1, &texture1);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();
    return 0;
}