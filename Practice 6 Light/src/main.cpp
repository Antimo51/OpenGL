#include <glad/glad.h> // 必须放第一个！
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <camera.h>
#include <shader.h>
#include <mesh.h>
#include <model.h>

#include <iostream>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;
float SCR_PROPORTION = (float)SCR_WIDTH / (float)SCR_HEIGHT;

Camera camera(glm::vec3(6.0f, 5.0f, 8.0f), glm::vec3(0.0f, 1.0f, 0.0f), -127.0f, -25.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 0;
float lastY = 0;

int main(int argc, char* argv[]){
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", NULL, NULL);
    if (window == NULL){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);

    Shader modelShader("shader/vs.glsl", "shader/fs.glsl");
    Shader lightShader("shader/vs.glsl", "shader/light_fs.glsl");

    glm::vec4 lightPosBase(20.0f, 20.0f, 20.0f, 1.0f);
    glm::vec4 lightColor(1.0f, 1.0f, 1.0f, 1.0f);
    Model light("resources/models/ball.obj");
    Model cube("resources/models/cube.obj");

    // render loop
    while (!glfwWindowShouldClose(window)){
        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_PROPORTION, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 baseModel(1.0f);

        glm::mat4 rotateModel = glm::rotate(baseModel, glm::radians(currentFrame) * 10, glm::vec3(0.0f, 1.0f, 0.0f));
        // rotateModel = glm::rotate(rotateModel, glm::radians(currentFrame) * 20, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec4 lightPosv4 = rotateModel * lightPosBase;
        glm::vec3 lightPos(lightPosv4.x, lightPosv4.y * sin(currentFrame), lightPosv4.z);
        
        lightShader.use();
        glm::mat4 lightModel = glm::scale(baseModel, glm::vec3(0.1f, 0.1f, 0.1f));
        lightModel = glm::translate(lightModel, lightPos);
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setMat4("model", lightModel);
        lightShader.setVec4("lightColor", lightColor);
        light.Draw(lightShader);

        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);
        modelShader.setMat4("model", baseModel);
        modelShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        modelShader.setVec3("lightColor",  lightColor);
        modelShader.setVec3("lightPos", lightPos);
        modelShader.setVec3("viewPos", camera.Position);
        cube.Draw(modelShader);

        //glfw: swap buffers & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate
    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: 窗口大小一旦改变，就会触发此函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    SCR_PROPORTION = (float)width / (float)height;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float> (yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float> (xposIn);
    float ypos = static_cast<float> (yposIn);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        camera.ProcessMouseMovement(xoffset, yoffset);
}