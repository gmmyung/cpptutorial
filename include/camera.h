#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>


class Camera
{
private:
    const float mouseSensitivity = 0.05;
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f); 
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    float deltaTime = 0.0f;	// time spent by last frame
    float lastFrame = 0.0f; // time of last frame
    float pitch = 0.0f;
    float yaw = -90.0f;
    float lastX = 400, lastY = 400; // last mouse position
    float cameraSpeed = 0.00f;
    bool firstMouse = true;

public:
    Camera(GLFWwindow* window){
        GLFWwindow* window_ = window;
    }
    ~Camera();
    void setTime(){
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cameraSpeed = 2.5f * deltaTime;
    }
    void processInput(GLFWwindow* window){
        setTime();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            moveForward();
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            moveBackward();
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            moveLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            moveRight();
        }
    }
    void moveForward(){
        cameraPos += cameraSpeed * cameraFront;
    }
    void moveBackward(){
        cameraPos -= cameraSpeed * cameraFront;
    }
    void moveLeft(){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    void moveRight(){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    void mouse_callback(GLFWwindow* window, double xpos, double ypos){
        if(firstMouse){
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; 
        lastX = xpos;
        lastY = ypos;

        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }  
    glm::mat4 view(){
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

};
