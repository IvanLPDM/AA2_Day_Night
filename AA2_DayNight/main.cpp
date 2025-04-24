#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>


//Constantes
#define M_PI 3.141592f

//Variables
//Sun
float angle_sun = 0;
float radius_sun = 2;
float vel_sun = 1;
float rotation_sun = 0;

//Camara
float camX = 0.0f, camY = 0.5f, camZ = 2.0f;  // Posición
float camYaw = 0.0f, camPitch = 0.0f;        // Rotación
float camSpeed = 0.05f;

int lastMouseX = 600, lastMouseY = 600;  // Centro inicial
bool firstMouse = true;

bool warpPointer = false;

//Timer
int currentTime = 0;  // Tiempo en horas (0-23)
int currentMinute = 0;  // Minutos (0-59)
int currentSecond = 0;  // Segundos (0-59)

// Función para inicializar la configuración de OpenGL
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Establece el color de fondo en negro
    glEnable(GL_DEPTH_TEST);
    glutSetCursor(GLUT_CURSOR_NONE); //Raton invisible
}

void update(int value) {


    angle_sun += vel_sun;

    if (angle_sun >= 360.0f)
    {
        angle_sun -= 360.0f;
    }

    rotation_sun += 1.0f;
    if (rotation_sun > 360.0f) rotation_sun -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
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

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


void drawObjects()
{
    // Dibujar la esfera en movimiento
    glPushMatrix();
    float x = radius_sun * cos(angle_sun * M_PI / 180.0f);
    float y = radius_sun * sin(angle_sun * M_PI / 180.0f);
    glTranslatef(x, y, 0.0f);  // Mover la esfera en un círculo
    glRotatef(rotation_sun, 0.0f, 0.0f, 1);
    glColor3f(1.0f, 1.0f, 0.0f);  // Azul
    glutWireSphere(0.1f, 20, 20);
    glPopMatrix();

    //Ground
    glColor3f(0.2f, 0.6f, 0.2f);  // Color verde para el plano
    glBegin(GL_QUADS);
    glVertex3f(-2.0f, -0.05f, -2.0f);
    glVertex3f(-2.0f, -0.05f, 2.0f);
    glVertex3f(2.0f, -0.05f, 2.0f);
    glVertex3f(2.0f, -0.05f, -2.0f);
    glEnd();
}


// Función para dibujar la escena
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Limpiar Buffer

    //Perspectiva
    glViewport(0, 0, 1200, 1200);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 10.0);  // Proyección en perspectiva

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

    //Texto--------------------
        // Habilitar el modo ortográfico para el texto 2D
        glDisable(GL_DEPTH_TEST);  // Desactivar el buffer de profundidad para que el texto no sea ocultado por objetos 3D
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 1200, 0, 1200);  // Establece la proyección ortográfica para 2D

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Dibuja el texto en la pantalla
        glColor3f(1.0f, 1.0f, 1.0f);  // Blanco
        renderBitmapString(10, 900, GLUT_BITMAP_HELVETICA_18, "Simulacion Solar");

        // Restaurar matrices y habilitar de nuevo el buffer de profundidad
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_DEPTH_TEST);  // Habilitar el buffer de profundidad nuevamente


    glFlush();  //Final
}

// Función principal para inicializar y ejecutar el programa
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 1200);  // Tamaño de la ventana
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Base");

    init();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);  // Iniciar update
    glutPassiveMotionFunc(passiveMouseMotion); //Movimiento del ratón

    glutMainLoop();  // Inicia el ciclo principal de GLUT
    return 0;
}