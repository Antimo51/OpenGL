教程地址 https://learnopengl-cn.github.io/
# OpenGL常见工作流
```cpp
// 创建对象 
unsigned int objectId = 0; glGenObject(1, &objectId); 
// 绑定对象至上下文 
glBindObject(GL_WINDOW_TARGET, objectId); 
// 设置当前绑定到 GL_WINDOW_TARGET 的对象的一些选项 
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800); 
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600); 
// 将上下文对象设回默认 
glBindObject(GL_WINDOW_TARGET, 0);
```
# 窗口
GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
```cpp
if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
    std::cout << "Failed to initialize GLAD" << std::endl; 
    return -1; 
}
```

视口：OpenGL渲染窗口的尺寸大小 `glViewport(0, 0, 800, 600);`
	处理过的OpenGL坐标范围只为-1到1，因此我们事实上将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)。
	对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用。
		`void framebuffer_size_callback(GLFWwindow* window, int width, int height);`
	我们还需要注册这个函数(glfwSFbSC)，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数(fsc)：
		`glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);`

渲染循环/Render Loop：
```cpp
while (!glfwWindowShouldClose(window)){
	  // 输入
	  processInput(window);

	  // 渲染指令
	  ...

	  // 检查并调用时间，交换缓冲
        glfwSwapBuffers(window);             // 交换颜色缓冲（一个储存着GLFW窗口每一个像素颜色值的大缓冲）绘制并输出
        glfwPollEvents();                    // 检查有没有什么触发事件
    }
```
双缓冲
	单缓冲：逐个绘制，类似CPU，画面易撕裂
	双缓冲：前缓冲保持显示图像，等后缓冲渲染好，迭代(Loop)时交换前后缓冲
缓冲位：`GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT`
	设置屏幕颜色——状态设置函数`glClearColor`
	清空缓存使用——状态使用函数`glClear`

```cpp
/* 获取并打印OpenGL版本信息 */
printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
printf("Renderer: %s\n", glGetString(GL_RENDERER));
printf("Vendor: %s\n", glGetString(GL_VENDOR));
```
# 三角形
输入3D坐标，输出2D像素的过程叫做==渲染管线==，使用一系列==着色器==，跑在GPU上

图元
	GL_TRIANGLES告诉gl这是一个封闭的三角形，内部要着色
	GL_LINE_STRIP告诉gl这是一条线，仅着这条线的色

渲染管线
	顶点着色器：输入顶点3D坐标（vec3），输出另一种3D坐标（vec4）
	几何着色器：（选）输入一组顶点，形成图元/生成新图元等
	图元装配：输入所有顶点，装配为图元的形状
	光栅化阶段：图元映射到屏幕上像素，输出片段；裁切\[-1, 1\]以外所有像素，提高执行效率
		片段：OpenGL渲染一个像素所需的所有数据
	片段着色器：计算像素的最终颜色
	Alpha测试与混合：利用片段/像素的深度、透明度等信息决定像素是否丢弃、混合
> 在现代OpenGL中，我们**必须**定义至少一个顶点着色器和一个片段着色器（因为GPU中没有默认的顶点/片段着色器）。出于这个原因，刚开始学习现代OpenGL的时候可能会非常困难，因为在你能够渲染自己的第一个三角形之前已经需要了解一大堆知识了。在本节结束你最终渲染出你的三角形的时候，你也会了解到非常多的图形编程知识。

## 顶点缓冲对象VBO
在显存中存一大堆顶点，显卡访问会特别快
OpenGL中的对象有一个独一无二的ID，这样生成：`unsigned int VBO; glGenBuffers(1, &VBO);`
VBO的缓冲类型是`GL_ARRAY_BUFFER`（这个叫目标），OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。`glBindBuffer(GL_ARRAY_BUFFER, VBO)`
此后，使用任何该目标上的缓冲调用都会用来配置当前绑定的VBO。调用`glBufferData`把数据复制到当前绑定缓冲的函数。
	三角形的位置数据不会改变，每次渲染调用时都保持原样，所以它的使用类型最好是`GL_STATIC_DRAW`。如果，比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是`GL_DYNAMIC_DRAW`或`GL_STREAM_DRAW`，这样就能确保显卡把数据放在能够高速写入的内存部分。

## 顶点着色器
如果我们打算做渲染的话，现代OpenGL需要我们至少设置一个顶点和一个片段着色器。
一个非常基础的GLSL顶点着色器的源代码：
```c
#version 330 core
layout (location = 0) in vec3 aPos;
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```
顶点着色器用于输入vec3坐标，输出vec4坐标；使用`in`关键字

顶点着色器需要在程序运行时动态编译
	使用着色器，需要创建一个着色器对象，仍然是用ID来引用；着色器类型为GL_VERTEX_SHADER
	`glCreateShader`创建该着色器
	`glShaderSource`把源代码附着到着色器上
	`glCompileShader`编译
	debug用`glGetShaderiv`检查是否成功，`glGetShaderInfoLog`获取错误信息

## 片段着色器
片段着色器用于计算像素最后的颜色输出
图形学中颜色用vec4向量表示：(red, green, blue, alpha)即RGBA
```c
#version 330 core
out vec4 FragColor;
void main(){
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
```
使用`out`关键字

编译过程与顶点着色器类似，着色器类型为GL_FRAGMENT_SHADER

## 着色器程序
多个着色器合并后，最终链接完成的版本/一个着色器程序对象
在渲染对象的时候激活这个着色器程序
合并/链接的过程很简单：输出连到下一个的输入即可；如果输出输入不匹配，会报错
	`glCreateProgram`创建对象
	`glAttachShader(shaderProgram, someShader)`附加上编译好的着色器
	最后`glLinkProgram`链接
	debug用`glGetProgramiv`检查是否成功，`glGetProgramInfoLog`获取错误信息
	最后`glUseProgram`激活
	（记得用`glDeleteShader`删除着色器对象，因为不再会用它们了）

## 链接顶点属性 & 顶点数组对象VAO
`glVertexAttribPointer`告诉OpenGL如何解析顶点数据
`glEnableVertexAttribArray(0)`以顶点属性位置值作为参数，启用顶点属性
	调用`glBindVertexArray`，告诉OpenGL我要设置一组特定的顶点数据配置，包括VBO EBO等缓冲区里包含的信息
	说完了之后用`glEnableVertexAttribArray`来启用这些配置

VAO可以像VBO一样被绑定，随后的任意顶点调用都可以按照绑定的方法

## 元素缓冲对象EBO/索引缓冲对象IBO
要画一个矩形，只需要存入四个顶点，然后用`indices`指引程序第一次画0 1 3的三角形，第二次画1 2 3
与VBO相同，这也是一个缓冲对象，缓冲类型为`GL_ELEMENT_ARRAY_BUFFER`

先生成，绑定，然后一样把data传进去，最后用`glDrawElements`绘制

# 事件处理
鼠标、键盘，教程里没有

## 鼠标
注册 `glfwSetMouseButtonCallback(window, mouse_button_callback); // 在一大堆glfw注册函数的地方`
定义 `void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); // 在最开头`
例子
```cpp
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        if (action == GLFW_PRESS){
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            std::cout << "Mouse pressed at (" << xPos << ", " << yPos << ")" << std::endl;
        }
    }
}
```
键盘类似

# 着色器
一个典型的着色器有下面的结构：
```c
#version version_number
in type in_variable_name;
in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

void main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```
GLSL语言，C风格
默认向量`vecn`，`float`类型，想要其他类型在前面加（比如`uvecn, ivecn FOR uint, int`）用法很自由
坐标xyzw，颜色rgba，纹理stpq

## 文件读取
写了一个类实现方便的读写：`shader.h`
```c
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        unsigned int ID; // 程序ID

        // 读取并构建着色器
        Shader(const char* vertexPath, const char* fragmentPath){
            // 1. 从文件中获取着色器
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            // 保证ifstream对象可以抛出异常
            vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            try{
                // open file
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // 读取文件缓冲内容 到数据流中
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close file
                vShaderFile.close();
                fShaderFile.close();
                // 转换数据流到string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch(std::ifstream::failure e){
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            // 2. 编译着色器
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success){
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            // fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success){
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPLATION_FAILED\n" << infoLog << std::endl;
            }

            // shader programme
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success){
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cout << "ERROR::PROGRAMME::LINKING_FAILED\n" << infoLog << std::endl;
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
        // 使用/激活程序
        void use(){
            glUseProgram(ID);
        }
        // uniform工具函数
        void setBool(const std::string &name, bool value) const{
            glUniform1i(glGetUniformLocation(ID, name.c_str()), int(value));
        }
        void setInt(const std::string &name, int value) const{
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const{
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
};

#endif
```

## 着色器的输入输出
所有着色器的输入输出都要连起来（数据类型和名字都要一样）
尽可能不要让输入或者输出悬空（即没有被引用到）

OpenGL确保至少有16个包含4分量的顶点属性可用，意思就是说顶点着色器里最多有16个vec4输入
顶点着色器 `<<` 顶点数据，使用 `layout (location = 0)`链接到顶点数据

Uniform
	应用程序从CPU上发程序到GPU
用Uniform声明的变量是全局的，可以随时被访问，使用`glGetUniformLocation`在`main.cpp`中索引到Uniform变量

# 纹理
或称为贴图
需要告诉OpenGL三角形每个顶点对应到纹理图片的位置。纹理图片左下角(0, 0)，右上角(1, 1)

纹理环绕方式：纹理坐标超出(0, 0)~(1, 1)时，会有一些选项提供应对方法-`GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER`，每个坐标轴都可以单独设置处理方法（纹理坐标轴名称str，对应xyz，使用函数`glTextParameteri`）
纹理过滤：Texel, addr. Texture Pixel. 纹理图片分辨率低时用的方法-`GL_NEAREST, GL_LINEAR`
多级渐远纹理：省空间，使远近处物体纹理视觉效果合理（略）

使用`stb_image.h`加载纹理
	把这个头文件放到`./include/`中即可

OpenGL中有16个纹理单元可以使用
一个纹理单元储存一个纹理及其相关的内容

# 坐标系统
局部空间：模型的样子
	放置（通过平移、旋转、缩放把模型放到世界中）
世界空间：把模型放到（游戏）世界中
	观察矩阵
观察空间：世界坐标转化为摄像机看到的坐标
	投影矩阵
裁剪空间：将变换到观察空间后看不到的点删掉
	透视除法：正交投影/透视投影（广角？）
屏幕空间

裁剪坐标$V_{clip}$
$$
V_{clip} = M_{projection} \cdot M_{view} \cdot M_{model} \cdot V_{local}
$$
最后顶点被放到`gl_Position`中，OpenGL会自动进行透视除法与裁剪
	顶点着色器中`gl_Position`的输入需要定义在裁剪空间中
（OpenGL有些地方可能是左手系，需要略微注意）

摄像机相关很多东西 直接看代码 代码简化了很多数学原理（爽）
> 学数学爽，而写代码简化数学结论也很爽。——沃茨基 · 硕得

some notes...
	鼠标最初位置设置为(300, 400)可能导致程序开始运行时突然狂转一下；可以强制让鼠标初始位置变成规定的那个值
	鼠标只改变了水平和竖直两个方向的转动，没有动顺时针逆时针的转动
	高效学习，我看了教程的理论部分就直接去写camera类了。任何摄像机都是有缺陷的，比如这个（FPS）摄像机仰角就不能超过90度
