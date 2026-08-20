#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "engine/framebuffer.hpp"
#include "engine/rasterizer.hpp"
#include "engine/renderer.hpp"
#include "glm-like-lib/vectors.hpp"
#include "engine/object.hpp"

struct Camera{
    gll::Vec3 pos;
    //use gll::Vec3 frontDef{-1, 0, 0}; for correct front(its tmp mb)
    gll::Gfloat pitch = 0.0f;
    gll::Gfloat yaw = 0.0f;

    gll::Vec3 cameraFront() {
        return gll::Vec3{std::cos(pitch) * std::cos(yaw),
                std::cos(pitch) * std::sin(yaw),
                std::sin(pitch)
        }.normalized();
    }
};

void moveCamera(GLFWwindow* window, Camera& camera, gll::Gfloat deltaTime, gll::Gfloat speed = 3.0f) {
    gll::Gfloat rot_speed = speed / 2;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) camera.pos.z += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) camera.pos.z -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) camera.pos.y -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) camera.pos.y += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) camera.pos.x -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) camera.pos.x += speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.pitch += rot_speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.pitch -= rot_speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.yaw += rot_speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.yaw -= rot_speed * deltaTime;

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

    obj::OBJfile obj = obj::ParceObj("../cube.obj");

    egn::Mesh cube = obj::ConvertOBJfileToMesh(obj);
    
    //gll::Vec3 cameraPos = {4, 0, 0};
    gll::Gfloat lastTime = glfwGetTime();

    egn::Light sun;
    sun.direction = gll::Vec3(0.0f, 0.0f, 1.0f);
    sun.ambient   = 0.2f;
    sun.diffuse   = 0.7f;
    sun.specular  = 0.3f;
    sun.shininess = 32.0f;

    egn::Texture check = egn::Texture::loadFromFile("../texture.png");
    //gll::Vec3 cameraFront = {-1, 0, 0};
    gll::Gfloat angle = -20.0f;
    int fps = 0;
    int lastSec = 0;

    Camera camera;
    camera.pos = gll::Vec3(4, 0, 0);
    camera.yaw = M_PI;

    while (!glfwWindowShouldClose(window)) {
        gll::Gfloat curTime = glfwGetTime();
        gll::Gfloat deltaTime = curTime - lastTime;
        glfwPollEvents();
        moveCamera(window, camera, deltaTime);
        fb.clear(0xFF000000);

        gll::Mat4 S = gll::scale(gll::Mat4::identity(), {1, 1.3, 0.5});
        gll::Mat4 model = gll::rotate(S, angle, {1, 1, 0});
        gll::Mat4 view = gll::lookAt(camera.pos, camera.pos + camera.cameraFront(), {0, 0, 1});
        gll::Mat4 proj = gll::perspective(3.14159f / 4.0f, gll::Gfloat(static_cast<gll::Gfloat>(WIDTH) / HEIGHT), 0.1f, 100.0f);

        egn::drawMesh(cube, model, view, proj, fb, &check, &sun);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, fb.color());
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        angle += 0.5f * deltaTime;
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
