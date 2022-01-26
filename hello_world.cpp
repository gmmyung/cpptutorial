#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HELLOWORLDConfig.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);

static const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";



int main()
{
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  
        0.5f, -0.5f, 0.0f,  
        -0.5f, -0.5f, 0.0f,  
        -0.5f,  0.5f, 0.0f   
    };
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    // Initialize OpenGL
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
    
    // Create Shader and compile
    unsigned int vertexShader;
    std::cout << "Shader Compiling..." << std::endl;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    

    // Shader compile status
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }else{
        std::cout << "Compile Success!" << std::endl;
    }

    // Compile Fragment Shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Shader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

    // Set vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);



        // Display Image (Swap Front/Back Buffer)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    std::cout << "Window Closed" << std::endl;
    return 0;
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