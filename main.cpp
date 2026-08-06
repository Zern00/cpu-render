#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "engine/framebuffer.hpp"
#include "engine/rasterizer.hpp"
#include "engine/renderer.hpp"
#include "glm-like-lib/vectors.hpp"
#include "engine/object.hpp"

void moveCamera(GLFWwindow* window, gll::Vec3& cameraPos, float deltaTime, float speed = 3.0f) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) cameraPos.y += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) cameraPos.y -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) cameraPos.x += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) cameraPos.x -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) cameraPos.z += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) cameraPos.z -= speed * deltaTime;
}

const int WIDTH = 1000, HEIGHT = 1000;
const char* vert = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTex;
    out vec2 TexCoord;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        TexCoord = aTex;
    }
)";

const char* frag = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;
    uniform sampler2D screenTexture;
    void main() {
        FragColor = texture(screenTexture, TexCoord);
    }
)";

GLuint createShaderProgram(const char* vs, const char* fs) {
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs, NULL);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs, NULL);
    glCompileShader(fragment);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "glwfInit error";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "render", NULL, NULL);

    if (!window) {
        std::cerr << "window create error";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    float quadVertices[] = {
        -1.0f,  1.0f,      0.0f, 0.0f,
        -1.0f, -1.0f,      0.0f, 1.0f,
         1.0f, -1.0f,      1.0f, 1.0f,

        -1.0f,  1.0f,      0.0f, 0.0f,
         1.0f, -1.0f,      1.0f, 1.0f,
         1.0f,  1.0f,      1.0f, 0.0f
    };
    
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    GLuint shader = createShaderProgram(vert, frag);

    GLuint screenTexture;
    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    egn::Framebuffer fb(WIDTH, HEIGHT);

    obj::OBJfile obj = obj::ParceObj("../cow.obj");

    egn::Mesh cube = obj::ConvertOBJfileToMesh(obj);
    
    for (size_t i = 0; i < cube.vertices.size(); ++i) {
        egn::Vertex now = cube.vertices[i];
        std::cerr << now.pos.x << " " << now.pos.y << " " << now.pos.z << " / "
                  << now.normal.x << " " << now.normal.y << " " << now.normal.z << " / "
                  << now.uv.x << " " << now.uv.y << " / " << now.color << std::endl;
    }

    gll::Vec3 cameraPos = {0, 0, -13};
    float lastTime = glfwGetTime();

    egn::Light sun;
    sun.direction = gll::Vec3(0.0f, 5.0f, -3.0f);
    sun.ambient   = 0.2f;
    sun.diffuse   = 0.7f;
    sun.specular  = 0.3f;
    sun.shininess = 32.0f;

    egn::Texture check = egn::Texture::loadFromFile("../texture.png");
    gll::Vec3 cameraFront = {0, 0, 1};
    float angle = 0.0f;
    int fps = 0;
    int lastSec = 0;
    while (!glfwWindowShouldClose(window)) {
        float curTime = glfwGetTime();
        float deltaTime = curTime - lastTime;
        moveCamera(window, cameraPos, deltaTime);
        glfwPollEvents();
        fb.clear(0xFF000000);

        gll::Mat4 model = gll::rotate(gll::Mat4::identity(), angle, {0, 1, 1});
        gll::Mat4 view = gll::lookAt(cameraPos, cameraPos + cameraFront, {0, 1, 0});
        gll::Mat4 proj = gll::perspective(3.14159f / 4.0f, float(WIDTH / HEIGHT), 0.1f, 100.0f);

        egn::drawMesh(cube, model, view, proj, fb, &check, &sun);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, fb.color());
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        angle += 3.0f * deltaTime;
        lastTime = curTime;
        glfwSwapBuffers(window);
        ++fps;
        if (lastTime >= lastSec + 1) {
            std::cerr << fps << std::endl;
            fps = 0;
            lastSec = std::floor(lastTime);
        }
    }

    glDeleteTextures(1, &screenTexture);
    glDeleteProgram(shader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}