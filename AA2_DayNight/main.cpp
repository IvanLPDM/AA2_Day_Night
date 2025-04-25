#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>


//Constantes
#define M_PI 3.141592f

//Enums
enum DayState { DAWN, NOON, DUSK, NIGHT };

//Variables
//Sun
float angle_sun = 0;
float radius_sun = 2;
float vel_sun = 1;
float rotation_sun = 0;

//Camara
float camX = 0.0f, camY = 0.5f, camZ = 2.0f;  // Posici�n
float camYaw = 0.0f, camPitch = 0.0f;        // Rotaci�n
float camSpeed = 0.05f;

int lastMouseX = 600, lastMouseY = 600;  // Centro inicial
bool firstMouse = true;

bool warpPointer = false;

DayState currentState = DAWN;



// Funci�n para inicializar la configuraci�n de OpenGL
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Establece el color de fondo en negro
    glEnable(GL_DEPTH_TEST);



    glutSetCursor(GLUT_CURSOR_NONE); //Raton invisible
}


//Movimiento del raton------------
void keyboard(unsigned char key, int x, int y) {
    float frontX = cos(camYaw) * cos(camPitch);
    float frontY = sin(camPitch);
    float frontZ = sin(camYaw) * cos(camPitch);

    float len = sqrt(frontX * frontX + frontY * frontY + frontZ * frontZ);
    frontX /= len;
    frontY /= len;
    frontZ /= len;

    float rightX = -sin(camYaw);
    float rightZ = cos(camYaw);

    switch (key) {
    case 'w':
        camX += frontX * camSpeed;
        camY += frontY * camSpeed;
        camZ += frontZ * camSpeed;
        break;
    case 's':
        camX -= frontX * camSpeed;
        camY -= frontY * camSpeed;
        camZ -= frontZ * camSpeed;
        break;
    case 'd':
        camX += rightX * camSpeed;
        camZ += rightZ * camSpeed;
        break;
    case 'a':
        camX -= rightX * camSpeed;
        camZ -= rightZ * camSpeed;
        break;
    case 27:  // ESC
        exit(0);
        break;
    }
    glutPostRedisplay();  // Actualiza la ventana
}

void passiveMouseMotion(int x, int y) {
    int centerX = 600;
    int centerY = 600;

    if (warpPointer) {
        warpPointer = false;  // Evita bucle infinito
        return;
    }

    float sensitivity = 0.002f;
    int dx = x - centerX;
    int dy = centerY - y;

    camYaw += dx * sensitivity;
    camPitch += dy * sensitivity;

    // Limitar pitch
    if (camPitch > M_PI / 2.0f - 0.01f) camPitch = M_PI / 2.0f - 0.01f;
    if (camPitch < -M_PI / 2.0f + 0.01f) camPitch = -M_PI / 2.0f + 0.01f;

    warpPointer = true;
    glutWarpPointer(centerX, centerY);  // Vuelve al centro

    glutPostRedisplay();
}

//Escena--------------------------
void drawHouse(float x, float y) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f); 
    glTranslatef(x, 0.2f, y);
    glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);  
    glTranslatef(x, 0.4f, y); 
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glRotatef(45, 0.0f, 0.0f, 1.0f);
    glutSolidCone(0.5, 0.5, 4, 4);
    glPopMatrix();
}

void drawTree(float x, float y) {
    glPushMatrix();
    glColor3f(0.55f, 0.27f, 0.07f);  
    glTranslatef(x, 0.0f, y);  
    glRotatef(-90, 1.0f, 0.0f, 0.0f);  
    glutSolidCone(0.1f, 1.0f, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0f, 1.0f, 0.0f); 
    glTranslatef(x, 1.0f, y);  
    glutSolidSphere(0.3, 20, 20);  
    glPopMatrix();
}

void drawStone(float x, float y) {
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(x, 0.0f, y); 
    glutSolidSphere(0.3, 20, 20);  
    glPopMatrix();
}


void drawObjects()
{
    // Dibujar la esfera en movimiento
    glPushMatrix();
    float x = radius_sun * cos(angle_sun * M_PI / 180.0f);
    float y = radius_sun * sin(angle_sun * M_PI / 180.0f);
    glTranslatef(x, y, 0.0f);  // Mover la esfera en un c�rculo
    glRotatef(rotation_sun, 0.0f, 0.0f, 1);
    glColor3f(1.0f, 1.0f, 0.0f);
    glutWireSphere(0.1f, 20, 20);
    glPopMatrix();

    //Ground
    glColor3f(0.2f, 0.6f, 0.2f);  // Color verde para el plano
    glBegin(GL_QUADS);
    glVertex3f(-4.0f, -0.05f, -4.0f);
    glVertex3f(-4.0f, -0.05f, 4.0f);
    glVertex3f(4.0f, -0.05f, 4.0f);
    glVertex3f(4.0f, -0.05f, -4.0f);
    glEnd();

    drawHouse(2,2);
    drawHouse(1,1);
    drawTree(0, 0);
    drawTree(2.2, 2.4);
    drawStone(.5, .5);
}

//Estado del d�a
void updateDayState() {
    if (angle_sun >= 45 && angle_sun < 135)
    {
        currentState = NOON;
    }
    else if (angle_sun >= 135 && angle_sun < 225)
    {
        currentState = DUSK;
    }
    else if (angle_sun >= 225 && angle_sun < 315)
    {
        currentState = NIGHT;
    }
    else
    {
        currentState = DAWN;
    }

    switch (currentState) {
    case DAWN:
        printf("Estado del d�a: Amanecer\n");
        break;
    case NOON:
        printf("Estado del d�a: Mediod�a\n");
        break;
    case DUSK:
        printf("Estado del d�a: Atardecer\n");
        break;
    case NIGHT:
        printf("Estado del d�a: Noche\n");
        break;
    }
}


// Funci�n para dibujar la escena
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Limpiar Buffer

    //Perspectiva
    glViewport(0, 0, 1200, 1200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 10.0);  // Proyecci�n en perspectiva

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Mouse Movement
    float frontX = cos(camYaw) * cos(camPitch);
    float frontY = sin(camPitch);
    float frontZ = sin(camYaw) * cos(camPitch);

    gluLookAt(camX, camY, camZ,
        camX + frontX, camY + frontY, camZ + frontZ,
        0.0f, 1.0f, 0.0f);

    drawObjects();

    glFlush();  //Final
}

void update(int value) {


    angle_sun += vel_sun;

    if (angle_sun >= 360.0f)
    {
        angle_sun -= 360.0f;
    }

    rotation_sun += 1.0f;
    if (rotation_sun > 360.0f) rotation_sun -= 360.0f;

    updateDayState();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


// Funci�n principal para inicializar y ejecutar el programa
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 1200);  // Tama�o de la ventana
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Base");

    init();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);  // Iniciar update
    glutPassiveMotionFunc(passiveMouseMotion); //Movimiento del rat�n

    glutMainLoop();  // Inicia el ciclo principal de GLUT
    return 0;
}