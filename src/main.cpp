#include <SOIL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>

const int WIN_WIDTH = 800, WIN_HEIGHT = 600;

const GLchar* readFile(std::string fileName);

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "OpenPlayground", nullptr, nullptr);

    glfwMakeContextCurrent(win);

    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ebo
    GLuint elements[] = {
            0, 1, 2,
            2, 3, 0,
    };
    GLuint ebo;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    // triangle
    float vertices[] = {
            // position   color             tex coords
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-left
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // bottom-left
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vert shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertShaderSrc = readFile("src/vertex.glsl");
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // check vert shader compilation status
    char vertShaderMsgBuf[512];
    glGetShaderInfoLog(vertShader, 512, NULL, vertShaderMsgBuf);
    printf("vertex shader compilation messages: %s\n", vertShaderMsgBuf);

    // frag shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragShaderSrc = readFile("src/fragment.glsl");
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // check frag shader compilation status
    char fragShaderMsgBuf[512];
    glGetShaderInfoLog(fragShader, 512, NULL, fragShaderMsgBuf);
    printf("fragment shader compilation messages: %s\n", fragShaderMsgBuf);

    // create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // pos attrib
    GLuint posAttrib = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "position"));
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), 0);
    glEnableVertexAttribArray(posAttrib);

    // col attrib
    GLuint colAttrib = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "color"));
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) (2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(colAttrib);

    // tex coord attrib
    GLuint texCoordAttrib = static_cast<GLuint>(glGetAttribLocation(shaderProgram, "tex_coord"));
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GL_FLOAT), (void*) (5 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(texCoordAttrib);

    // tex
    GLuint tex[2];
    glGenTextures(2, tex);

    // tex 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int texWidth, texHeight;
    unsigned char* img = SOIL_load_image("res/sample.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    SOIL_free_image_data(img);

    glUniform1i(glGetUniformLocation(shaderProgram, "tex_kitten"), 0);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    // tex 2
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    img = SOIL_load_image("res/sample2.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    SOIL_free_image_data(img);

    glUniform1i(glGetUniformLocation(shaderProgram, "tex_puppy"), 1);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    // anisotropic filtering
    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

    // uniforms
    GLuint uniTime = static_cast<GLuint>(glGetUniformLocation(shaderProgram, "time"));

    auto startTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(win)) {
        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, GL_TRUE);
        }

        auto now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - startTime).count();

        glUniform1f(uniTime, time);

        // draw!
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glDeleteTextures(2, tex);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    glfwTerminate();

    return 0;
}

const char* readFile(std::string fileName)
{
    std::string res;
    std::ifstream file(fileName, std::ios::in);

    if (file.is_open()) {
        std::string line;

        while (getline(file, line)) {
            res.append(line).append("\n");
        }
    }
    else {
        std::cerr << "Could not open file '" << fileName << "': " << strerror(errno) << std::endl;
        exit(-1);
    }

    file.close();

    return res.c_str();
}
