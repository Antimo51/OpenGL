#include <glad/glad.h> // 必须放第一个！
#include <GLFW/glfw3.h>

#include <camera.h>
#include <shader.h>
#include <model.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float SCR_PROPORTION = (float)SCR_WIDTH / (float)SCR_HEIGHT;
int changeLoc;
float setCol[6];

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

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader Lshader("shader/vs.glsl", "shader/Lfs.glsl");
    Shader Rshader("shader/vs.glsl", "shader/Rfs.glsl");

    // establish a triangle
    float vertices[] = { 
        -0.9, -0.2, 0, 
        -0.1, -0.2, 0,
        -0.5, 0.8, 0,
        0.9, -0.2, 0, 
        0.1, -0.2, 0,
        0.5, 0.8, 0
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 先绑VAO 再绑VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    int lasttime = 1;
    while (!glfwWindowShouldClose(window)){
        if (lasttime == (int)glfwGetTime()){
            lasttime += 1;
            if (changeLoc < 6) setCol[changeLoc] = min(1.0f, setCol[changeLoc] + 0.1f);
            else setCol[changeLoc - 6] = max(0.0f, setCol[changeLoc - 6] - 0.1f);
        }
        float currentFrame = static_cast<float> (glfwGetTime());
        for (int i = 0; i < 6; ++i) printf("%f ", setCol[i]); puts("");
        // input
        processInput(window);

        // render
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glViewport(0, 0, SCR_WIDTH/2, SCR_HEIGHT/2);
        Lshader.use();
        Lshader.setVec3("setCol", setCol[0], setCol[1], setCol[2]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // glViewport(SCR_WIDTH/2, 0, SCR_WIDTH/2, SCR_HEIGHT/2);
        Rshader.use();
        Rshader.setVec3("setCol", setCol[3], setCol[4], setCol[5]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glBindVertexArray(0);

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
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
    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) changeLoc = 0;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) changeLoc = 1;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) changeLoc = 2;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) changeLoc = 3;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) changeLoc = 4;
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) changeLoc = 5;
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) changeLoc = 6;
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) changeLoc = 7;
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) changeLoc = 8;
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) changeLoc = 9;
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) changeLoc = 10;
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) changeLoc = 11;
}

// glfw: 窗口大小一旦改变，就会触发此函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    SCR_PROPORTION = (float)width / (float)height;
}
