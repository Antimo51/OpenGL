#include <glad/glad.h> // 必须放第一个！
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <camera.h>
#include <shader.h>
#include <mesh.h>
#include <model.h>

#include <iostream>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int loadTexture(const char *path);

unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;
float SCR_PROPORTION = (float)SCR_WIDTH / (float)SCR_HEIGHT;

Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 0;
float lastY = 0;

int texParaIter;
int texParameters[] = {
    GL_NEAREST, 
    GL_LINEAR, 
    GL_NEAREST_MIPMAP_NEAREST, 
    GL_LINEAR_MIPMAP_NEAREST, 
    GL_NEAREST_MIPMAP_LINEAR, 
    GL_LINEAR_MIPMAP_LINEAR
};

int main(int argc, char* argv[]){
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LAB3 Texture", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);
    Shader shader("shader/vs.glsl", "shader/fs.glsl");

    float vertices[] = {
    //  position            texCord
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f
    };

    int texSerial[] = {0, 1, 0, 2};
    float rotateAngles[] = {90.0f, -90.0f, 90.0f, -90.0f};
    glm::vec3 rotateAxis[] = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };
    glm::vec3 phasePosition[] = {
        glm::vec3(-0.5, -0.5, -0.5),
        glm::vec3(-0.5,  0.5, -0.5),
        glm::vec3( 0.5, -0.5, -0.5),
        glm::vec3(-0.5, -0.5, -0.5),
        glm::vec3(-0.5, -0.5,  0.5),
        glm::vec3(-0.5,  0.5,  0.5),
        glm::vec3( 0.5, -0.5,  0.5),
        glm::vec3(-0.5, -0.5,  0.5)
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int tFloor = loadTexture("resources/floor.jpg");
    unsigned int tCeiling = loadTexture("resources/ceiling.jpg");
    unsigned int tWall = loadTexture("resources/wall.jpg");

    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = float(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_PROPORTION, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);

        shader.setInt("tex", 0);
        glBindVertexArray(VAO);
        for (int i = 0; i < 8; ++i) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, phasePosition[i]);
            model = glm::rotate(model, glm::radians(rotateAngles[i%4]), rotateAxis[i%4]);
            shader.setMat4("model", model);
            if(texSerial[i%4] == 0) glBindTexture(GL_TEXTURE_2D, tWall);
            if(texSerial[i%4] == 1) glBindTexture(GL_TEXTURE_2D, tCeiling);
            if(texSerial[i%4] == 2) glBindTexture(GL_TEXTURE_2D, tFloor);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParameters[texParaIter]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParameters[texParaIter]);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        //glfw: swap buffers & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate
    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow* window) {
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWNWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UPWARD, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        texParaIter = 0;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        texParaIter = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        texParaIter = 2;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        texParaIter = 3;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        texParaIter = 4;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        texParaIter = 5;
}

// glfw: 窗口大小一旦改变，就会触发此函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    SCR_PROPORTION = (float)width / (float)height;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float> (yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float> (xposIn);
    float ypos = static_cast<float> (yposIn);

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    // printf("mouse: %f %f\n", xoffset, yoffset);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        camera.ProcessMouseMovement(xoffset, yoffset);
}

unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    // printf("tex(%s) components=%d\n", path, nrComponents);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}