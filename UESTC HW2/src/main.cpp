#include <glad/glad.h> // 必须放第一个！
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float SCR_PROPORTION = (float)SCR_WIDTH / (float)SCR_HEIGHT;
float lasX = SCR_WIDTH / 2.0f;
float lasY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool objRotateFlag = false;
bool lastPressed = false;
float rotateAngle = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

const float PI = 3.14159265358979323846f; // 定义π的值
const int STACKS = 30; // 球体的纵向切片数
const int SLICES = 30; // 球体的横向切片数

float ballVertices[STACKS * SLICES * 3 * 2]; // 用于存储球体顶点的数组

void createSphere() {
    // 使用两层循环遍历球体的纬度和经度
    for (int stack = 0; stack < STACKS; ++stack) {
        float phi = PI * float(stack) / float(STACKS); // 当前纬度
        float phiNext = PI * float(stack + 1) / float(STACKS); // 下一个纬度
        for (int slice = 0; slice < SLICES; ++slice) {
            float theta = 2 * PI * float(slice) / float(SLICES); // 当前经度
            float thetaNext = 2 * PI * float(slice + 1) / float(SLICES); // 下一个经度

            // 计算当前点的正弦和余弦值
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            float sinPhiNext = sin(phiNext);
            float cosPhiNext = cos(phiNext);

            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            float sinThetaNext = sin(thetaNext);
            float cosThetaNext = cos(thetaNext);

            // 计算球体的顶点坐标
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 0] = sinPhi * cosTheta;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 1] = cosPhi;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 2] = sinPhi * sinTheta;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 3] = sinPhiNext * cosTheta;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 4] = cosPhiNext;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 5] = sinPhiNext * sinTheta;

            // 重复顶点以形成三角形带
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 6] = sinPhi * cosTheta;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 7] = cosPhi;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 8] = sinPhi * sinTheta;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 9] = sinPhiNext * cosTheta;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 10] = cosPhiNext;
            ballVertices[stack * (SLICES * 3 * 2) + slice * 6 + 11] = sinPhiNext * sinTheta;
        }
    }
}

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

    Shader axisShader("shader/vs.glsl", "shader/fs.glsl");
    Shader geoShader("shader/geo_vs.glsl", "shader/fs.glsl");

    glEnable(GL_DEPTH_TEST);

    float axisLength = 10.2f;
    float axisVertices[] = {
        // 位置             // 颜色
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 
        axisLength, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 
        0.0f, axisLength, 0.0f,  1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, axisLength,  1.0f, 0.0f, 0.0f
    };

    glm::vec3 cubePos(0.0f, 0.0f, 1.0f);
    float cubeVertices[] = {
        0.5f, 0.5f, 0.5f, 
        0.5f, -0.5f, 0.5f, 
        -0.5f, -0.5f, 0.5f, 
        -0.5f, 0.5f, 0.5f, 
        0.5f, 0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, 0.5f, -0.5f
    };
    int cubeIndices[] = {       // 这里数据类型写成float去了，被硬控一小时
        0, 1, 2, 2, 3, 0, 
        3, 2, 6, 6, 7, 3, 
        7, 6, 5, 5, 4, 7, 
        4, 5, 1, 1, 0, 4, 
        0, 4, 7, 7, 3, 0, 
        1, 2, 6, 6, 5, 1
    };

    createSphere();
    glm::vec3 ballPos(1.2f, 0.0f, 0.3f);

    unsigned int axisVAO, axisVBO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int ballVAO, ballVBO;
    glGenVertexArrays(1, &ballVAO);
    glGenBuffers(1, &ballVBO);
    glBindVertexArray(ballVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ballVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertices), ballVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)){
        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_PROPORTION, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 baseModel(1.0f);
        if (objRotateFlag) rotateAngle += deltaTime;

        axisShader.use();
        axisShader.setMat4("projection", projection);
        axisShader.setMat4("view", view);
        axisShader.setMat4("model", baseModel);
        glBindVertexArray(axisVAO);
        glDrawArrays(GL_LINES, 0, 6);

        glm::mat4 cubeModel = glm::rotate(baseModel, glm::radians(rotateAngle * 50), glm::vec3(0.0, 1.0, 0.0));
        cubeModel = glm::translate(cubeModel, cubePos);
        geoShader.use();
        geoShader.setMat4("projection", projection);
        geoShader.setMat4("view", view);
        geoShader.setMat4("model", cubeModel);
        geoShader.setVec3("cameraPos", camera.Position);
        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glm::mat4 ballModel = glm::rotate(baseModel, glm::radians(rotateAngle * 50), glm::vec3(0.0, 1.0, 0.0));
        ballModel = glm::translate(ballModel, ballPos);
        ballModel = glm::scale(ballModel, glm::vec3(0.4, 0.4, 0.4));
        geoShader.setMat4("model", ballModel);
        glBindVertexArray(ballVAO);
        glDrawArrays(GL_TRIANGLES, 0, 5400);

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
    
    bool currentRPressed = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    if (!lastPressed && currentRPressed) objRotateFlag ^= 1;
    lastPressed = currentRPressed;
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // 会与移动按钮冲突，所以改为按R切换是否旋转
    //     objRotateFlag = false;
}

// glfw: 窗口大小一旦改变，就会触发此函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    SCR_PROPORTION = (float)width / (float)height;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float> (yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float> (xposIn);
    float ypos = static_cast<float> (yposIn);

    float xoffset = xpos - lasX;
    float yoffset = lasY - ypos;
    lasX = xpos;
    lasY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        camera.ProcessMouseMovement(xoffset, yoffset);
}