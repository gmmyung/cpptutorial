#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <HELLOWORLDConfig.h>
#include <stdlib.h>
#include <stddef.h>
#include <cmath>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.h>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow *window);
static unsigned int compileShader(const char *ShaderPath, int options);
static void initGLFW();
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

static Camera camera = Camera();

int main()
{
    glm::vec3 cubePositions[] = {
        glm::vec3( 2.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    float vertices[] = {
        // 위치              // 컬러             // 텍스처 좌표
        0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 우측 상단
        0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 우측 하단
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 좌측 하단
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // 좌측 상단
        0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 우측 상단
        0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 우측 하단
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 좌측 하단
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 좌측 상단
    };  
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3,    // second triangle
        4, 5, 7,   // first triangle
        5, 6, 7,    // second triangle
        0, 4, 1,   // first triangle
        1, 5, 4,   // second triangle
        2, 6, 7,   // first triangle
        7, 3, 2,   // second triangle
        2, 1, 6,   // first triangle
        1, 5, 6,   // second triangle
        0, 3, 4,   // first triangle
        4, 3, 7    // second triangle
    };  
    unsigned int VBO, VAO, EBO;
    const char *vertexShaderPath = "shaders/vertex.glsl";
    const char *fragmentShaderPath = "shaders/fragment.glsl";
    unsigned int shaderProgram;
    const char *imagePath = "textures/Joe_Biden_presidential_portrait.jpg";
    unsigned int texture1;
    int width, height, nrChannels;
    unsigned int transformLoc, viewLoc, projectionLoc, modelLoc;

    // Define transformations
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    // Initialize OpenGL
    initGLFW();

    // Create Window
    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
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
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);  
    glGenBuffers(1, &EBO);  
    glBindVertexArray(VAO);  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);  
    Shader ourShader(vertexShaderPath, fragmentShaderPath);
    
    // Load and create a texture from a file
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0); 
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Texture load Success!" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Get uniform location
    transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
    viewLoc = glGetUniformLocation(ourShader.ID, "view");
    modelLoc = glGetUniformLocation(ourShader.ID, "model");

    // Show Window
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);    

    // Main Rendering Loop
    while(!glfwWindowShouldClose(window))
    {
        // User Input
        camera.processInput(window);

        //Background Color
        glClearColor(1.0f, 1.0, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);  // Enable depth test
        glBindTexture(GL_TEXTURE_2D, texture1);
        ourShader.use();

        glBindVertexArray(VAO);
        trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(float(glfwGetTime()) * 100), glm::vec3(1.0, 1.0, 0.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getView()));
        
        
        
        for(unsigned int i = 0; i < 10; i++){
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle + float(glfwGetTime()) * 100), cubePositions[i]);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

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
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.mouse_callback(window, xpos, ypos);
}  