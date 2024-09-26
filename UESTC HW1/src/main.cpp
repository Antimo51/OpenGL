#include <glad/glad.h> // 必须放第一个！
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <cstring>
#include <cmath>

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float get_dist(float x1, float y1, float x2, float y2){ return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));}

// 着色器代码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

std::vector<glm::vec2> points;

int inputMode = 0;
enum MODE{ LINE, SQUARE, CIRCLE};

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

    // 注册各种函数
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 编译着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // VAO VBO EBO等
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*10000, NULL, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*5000, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(1, 1, 1, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        if (inputMode == SQUARE && points.size() == 2){
            float vertices[3000][2];
            unsigned int indices[3000];
            for(int i = 0; i < 3000; ++i) indices[i] = i;

            // Calculate the bounding rectangle
            float minX = std::min(points[0].x, points[1].x);
            float maxX = std::max(points[0].x, points[1].x);
            float minY = std::min(points[0].y, points[1].y);
            float maxY = std::max(points[0].y, points[1].y);
            float step = (maxY - minY) / 1500.0;

            for(int i = 0; i < 3000; i+=2){
                vertices[i][0] = minX;
                vertices[i+1][0] = maxX;
                vertices[i][1] = minY + i*step/2;
                vertices[i+1][1] = minY + i*step/2;
            }

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

            glDrawElements(GL_LINE_LOOP, 3000, GL_UNSIGNED_INT, 0);
        } else if(inputMode == LINE && points.size() == 2){
            float vertices[2][2];
            unsigned int indices[2] = {0, 1};

            for(int i = 0; i < 2; ++i){
                vertices[i][0] = points[i].x;
                vertices[i][1] = points[i].y;
            }

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

            glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
        } else if(inputMode == CIRCLE && points.size() == 2){
            float vertices[2500][2];
            unsigned int indices[5000];
            for(int i = 0; i < 5000; i+=2) indices[i]=0, indices[i+1]=(i>>1)+1;
            float dist = get_dist(points[0].x, points[0].y, points[1].x, points[1].y);
            float step = std::acos(-1.0)*2 / 2499.0;
            
            vertices[0][0] = points[0].x;
            vertices[0][1] = points[0].y;
            for(int i = 1; i < 2500; ++i){
                vertices[i][0] = vertices[0][0] + cos(i*step) * dist;
                vertices[i][1] = vertices[0][1] + sin(i*step) * dist;
            }

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

            glDrawElements(GL_LINE_LOOP, 5000, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // release resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate
    glfwTerminate();
    return 0;
}

// process all input
void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: 窗口大小一旦改变，就会触发此函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            GLfloat ndcX = 2.0f * xPos / SCR_WIDTH - 1.0f;
            GLfloat ndcY = 1.0f - (2.0f * yPos / SCR_HEIGHT);
            std::cout << "Mouse pressed at (" << ndcX << ", " << ndcY << ")" << std::endl;
            if(points.size() == 2) points.clear();
            points.push_back(glm::vec2(ndcX, ndcY));
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS){
        std::cout << "Key pressed: " << key << std::endl;
        switch (key){
            case GLFW_KEY_L:
                inputMode = LINE;
                break;
            case GLFW_KEY_Q:
                inputMode = SQUARE;
                break;
            case GLFW_KEY_C:
                inputMode = CIRCLE;
                break;
            default:
                break;
        }
    }
}