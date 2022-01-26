#include <iostream>
#include <adder.h>

//int add(int x, int y);
/*#include <OpenGL/OpenGL.h> 
#include <GLUT/GLUT.h> 

void myDisplay(){ 
    glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_POLYGON); 
    glVertex3f(-0.5, -0.5, 0.0); 
    glVertex3f(0.5, -0.5, 0.0); 
    glVertex3f(0.5, 0.5, 0.0); 
    glVertex3f(-0.5, 0.5, 0.0); 
    glEnd(); 
    glFlush(); 
} 
    
int main(int argc, char* argv[]) { 
    glutInit(&argc, argv); 
    glutCreateWindow("Test"); 
    glutDisplayFunc(myDisplay); 
    glutMainLoop(); 
    return 0; 
}*/
int main(){
    std::cout << "Hello World!" << std::endl;
    std::cout << add(1, 2);
    return 0;
}
