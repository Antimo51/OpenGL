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
            catch(std::ifstream::failure& e){
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
        void setVec2(const std::string &name, const glm::vec2 &value) const{ 
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec2(const std::string &name, float x, float y) const{ 
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
        }
        void setVec3(const std::string &name, const glm::vec3 &value) const{ 
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec3(const std::string &name, float x, float y, float z) const{ 
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
        }
        void setVec4(const std::string &name, const glm::vec4 &value) const{ 
            glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec4(const std::string &name, float x, float y, float z, float w) const{ 
            glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
        }
        void setMat2(const std::string &name, const glm::mat2 &mat) const{
            glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat3(const std::string &name, const glm::mat3 &mat) const{
            glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        void setMat4(const std::string &name, const glm::mat4 &mat) const{
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

        // 提取uniform值 待完善
        bool getBool(const std::string &name) const{
            GLint result;
            glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &result);
            return result != 0;
        }
        int getInt(const std::string &name) const{
            GLint result;
            glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &result);
            return static_cast<int> (result);
        }
        float getFloat(const std::string &name) const{
            GLfloat result;
            glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &result);
            return static_cast<float> (result);
        }

        // 处理报错
        private:
            void checkCompileErrors(GLuint shader, std::string type){
                GLint success;
                GLchar infoLog[1024];
                if (type != "PROGRAM"){
                    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                    if (!success){
                        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog 
                            << "\n -- --------------------------------------------------- -- " << std::endl;
                    }
                } else {
                    glGetProgramiv(shader, GL_LINK_STATUS, &success);
                    if (!success){
                        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog 
                            << "\n -- --------------------------------------------------- -- " << std::endl;
                    }
                }
            }
};

#endif