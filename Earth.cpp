#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <cmath>
#include <cstdlib>

// Global variables
GLuint earthTextureID;
GLuint moonTextureID;
GLuint atmosphereTextureID;
GLfloat earthRotationAngle = 0.0f;
GLfloat moonOrbitAngle = 0.0f;
GLfloat moonRotationAngle = 0.0f;
GLfloat sunlightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat sunlightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat sunlightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat sunlightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat atmosphereColor[] = { 0.1f, 0.1f, 0.6f, 0.4f };
GLfloat terrainColor[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat rainIntensity = 0.0f;
bool enableRain = true; // Option to enable/disable rain effect

void loadTextures() {
    // Load Earth's texture
    glGenTextures(1, &earthTextureID);
    glBindTexture(GL_TEXTURE_2D, earthTextureID);
// add texture loading
    // Load Moon's texture
    glGenTextures(1, &moonTextureID);
    glBindTexture(GL_TEXTURE_2D, moonTextureID);

    // Load atmosphere texture
    glGenTextures(1, &atmosphereTextureID);
    glBindTexture(GL_TEXTURE_2D, atmosphereTextureID);
//here}

void renderEarth() {
    // Enable lighting and set up material properties for Earth
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, sunlightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunlightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunlightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunlightSpecular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, terrainColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, terrainColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, terrainColor);

    // Enable texture mapping for Earth
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earthTextureID);

    // Render the Earth
    glPushMatrix();
    glRotatef(earthRotationAngle, 0.0f, 1.0f, 0.0f);
    GLUquadric* earthQuadric = gluNewQuadric();
    gluQuadricTexture(earthQuadric, GL_TRUE);
    gluSphere(earthQuadric, 1.0, 50, 50);
    gluDeleteQuadric(earthQuadric);
    glPopMatrix();

    // Render the atmosphere effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, atmosphereTextureID);
    glMaterialfv(GL_FRONT, GL_AMBIENT, atmosphereColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, atmosphereColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, atmosphereColor);
    glPushMatrix();
    glRotatef(earthRotationAngle, 0.0f, 1.0f, 0.0f);
    GLUquadric* atmosphereQuadric = gluNewQuadric();
    gluQuadricTexture(atmosphereQuadric, GL_TRUE);
    gluSphere(atmosphereQuadric, 1.03, 50, 50);
    gluDeleteQuadric(atmosphereQuadric);
    glPopMatrix();
    glDisable(GL_BLEND);

    // Disable texture mapping for Earth
    glDisable(GL_TEXTURE_2D);

    // Disable lighting for Earth
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

void renderMoon() {
    // Enable lighting and set up material properties for Moon
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, sunlightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunlightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunlightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunlightSpecular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, terrainColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, terrainColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, terrainColor);

    // Enable texture mapping for Moon
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, moonTextureID);

    // Calculate Moon's position
    GLfloat moonOrbitRadius = 3.0f;
    GLfloat moonX = moonOrbitRadius * cos(moonOrbitAngle);
    GLfloat moonZ = moonOrbitRadius * sin(moonOrbitAngle);

    // Render the Moon
    glPushMatrix();
    glTranslatef(moonX, 0.0f, moonZ);
    glRotatef(moonRotationAngle, 0.0f, 1.0f, 0.0f);
    GLUquadric* moonQuadric = gluNewQuadric();
    gluQuadricTexture(moonQuadric, GL_TRUE);
    gluSphere(moonQuadric, 0.3, 50, 50);
    gluDeleteQuadric(moonQuadric);
    glPopMatrix();

    // Disable texture mapping for Moon
    glDisable(GL_TEXTURE_2D);

    // Disable lighting for Moon
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

void renderRain() {
    if (!enableRain) {
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, rainIntensity); // Set rain color and intensity
    glBegin(GL_LINES);
    for (int i = 0; i < 100; i++) {
        GLfloat x = static_cast<GLfloat>(rand()) / RAND_MAX * 2.0f - 1.0f;
        GLfloat y = static_cast<GLfloat>(rand()) / RAND_MAX * 2.0f - 1.0f;
        GLfloat z = static_cast<GLfloat>(rand()) / RAND_MAX * 2.0f - 1.0f;
        glVertex3f(x, y, z);
        glVertex3f(x, y - 0.1f, z); // Adjust raindrop length
    }
    glEnd();
    glDisable(GL_BLEND);
}

void update() {
    // Update Earth's rotation
    earthRotationAngle += 0.1f;

    // Update Moon's orbit and rotation
    moonOrbitAngle += 0.05f;
    moonRotationAngle += 0.3f;

    // Update rain intensity
    GLfloat maxIntensity = 0.5f;
    rainIntensity += 0.01f;
    if (rainIntensity > maxIntensity) {
        rainIntensity = 0.0f;
    }

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    renderEarth();
    renderMoon();
    renderRain();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q' || key == 27) {
        exit(0);
    } else if (key == 'r' || key == 'R') {
        enableRain = !enableRain; // Toggle rain effect
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Render Earth");

    loadTextures();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(update);

    glutMainLoop();
    return 0;
}
