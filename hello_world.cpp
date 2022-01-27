#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HELLOWORLDConfig.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <cmath>
#include "shader.h"

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);
static unsigned int compileShader(const char *ShaderPath, int options);
static void initGLFW();

int main()
{
    //colorless vertices
    /*
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  
        0.5f, -0.5f, 0.0f,  
        -0.5f, -0.5f, 0.0f,  
        -0.5f,  0.5f, 0.0f   
    };*/

    float vertices[] = {
        // Position          // Color
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    
    };    
    unsigned int indices[] = {  
        0, 1, 2,   // first triangle
        //1, 2, 3    // second triangle
    };  

    // Initialize OpenGL
    initGLFW();
    std::cout << "Initialized OpenGL" << std::endl;

    // Create Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

     // GLAD configure
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    
    std::cout << "Initialized GLAD" << std::endl;

    // Set and bind Vertex Buffer Object & Vertex Array Object $ Element Buffer Object
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);  // Generate Vertex Array Object
    glGenBuffers(1, &VBO);  // Generate Vertex Buffer Object
    glGenBuffers(1, &EBO);  // Generate Element Buffer Object
    glBindVertexArray(VAO);  // Bind Vertex Array Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind Vertex Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  // Bind Element Buffer Object
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Copy vertices to Vertex Buffer Object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  // Copy indices to Element Buffer Object
    std::cout << "VBO & VAO Set" << std::endl;

    char *vertexShaderPath = "shaders/vertex.glsl";
    char *fragmentShaderPath = "shaders/fragment.glsl";
    unsigned int shaderProgram;
    Shader ourShader(vertexShaderPath, fragmentShaderPath);
    
    /*
    // Compile Vertex Shader & Fragment Shader
    unsigned int vertexShader;
    unsigned int fragmentShader;
    vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    std::cout << "Compiled Vertex Shader & Fragment Shader" << std::endl;

    // Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 
    */

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Show Window
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
      
    // Main Rendering Loop
    while(!glfwWindowShouldClose(window))
    {
        // User Input
        processInput(window);

        

        //Render Images
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        ourShader.use();
        /*
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        */

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        // Display Image (Swap Front/Back Buffer)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ourShader.deleteShader();
    glfwTerminate();
    std::cout << "Window Closed" << std::endl;
    return 0;
}

static void initGLFW(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

static void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
