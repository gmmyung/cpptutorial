#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  

class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath){
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();		
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);  // Compile Vertex Shader
        fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);  // Compile Fragment Shader

        // shader Program
        ID = linkShaderProgram(vertex, fragment);
        
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    unsigned int compileShader(char const* shaderSource, int option){
        int  success;
        char infoLog[512];
        // Create Shader and compile
        unsigned int Shader;
        std::cout << "Shader Compiling..." << std::endl;
        Shader = glCreateShader(option);
        glShaderSource(Shader, 1, &shaderSource, NULL);
        glCompileShader(Shader);
        // Shader compile status
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(Shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return 0;
        }else{
            std::cout << "Compile Success!" << std::endl;
            return Shader;
        }
    }
    unsigned int linkShaderProgram(unsigned int vertex, unsigned int fragment){
        int  success;
        char infoLog[512];
        unsigned int ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        return ID;
    }
    // use/activate the shader
    void use() 
    { 
        glUseProgram(ID);
    }  
    // delete the shader
    void deleteShader(){
        glDeleteProgram(ID);
    }
    // utility uniform functions
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    } 
};


  
#endif