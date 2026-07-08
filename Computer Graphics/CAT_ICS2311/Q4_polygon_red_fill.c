/*
 * ICS2311 CAT - QUESTION 4 [SOLID VERSION]
 * Program: Filled Polygon with Shades of Red
 * Vertices: (8,4)(2,4)(0,8)(3,12)(7,12)(10,8)
 *
 * GL_POLYGON already produces a solid filled shape — no dotting issue here.
 * GL_LINE_LOOP for the outline is also solid by nature.
 * This version is clean and consistent with the rest.
 *
 * SHADES OF RED:
 *   Each vertex gets a different red intensity (R varies, G=B=0).
 *   OpenGL interpolates colors between vertices (Gouraud shading),
 *   creating a smooth gradient across the polygon surface.
 */

#include <GL/glut.h>

#define NUM_VERTS 6
float polyX[NUM_VERTS] = { 8.0f,  2.0f,  0.0f,  3.0f,  7.0f, 10.0f };
float polyY[NUM_VERTS] = { 4.0f,  4.0f,  8.0f, 12.0f, 12.0f,  8.0f };

/* Red intensity per vertex — varies from dark to bright red */
float redShade[NUM_VERTS] = { 0.5f, 0.3f, 0.6f, 1.0f, 0.8f, 0.7f };

void drawAxes() {
    glColor3f(0.4f, 0.4f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(-2.0f, 0.0f); glVertex2f(14.0f,  0.0f);
        glVertex2f( 0.0f,-2.0f); glVertex2f( 0.0f, 15.0f);
    glEnd();
    glBegin(GL_LINES);
    for (int i = -1; i <= 13; i++) {
        if (i != 0) {
            glVertex2f((float)i, -0.2f); glVertex2f((float)i,  0.2f);
            glVertex2f(-0.2f, (float)i); glVertex2f( 0.2f, (float)i);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    /*
     * Draw FILLED polygon with red shading.
     * GL_POLYGON fills the convex area, interpolating vertex colors.
     * Each vertex has a different shade of red (#FF0000 family).
     */
    glBegin(GL_POLYGON);
        for (int i = 0; i < NUM_VERTS; i++) {
            glColor3f(redShade[i], 0.0f, 0.0f);  /* Shade of red, no green/blue */
            glVertex2f(polyX[i], polyY[i]);
        }
    glEnd();

    /* Draw solid red outline on top */
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < NUM_VERTS; i++)
            glVertex2f(polyX[i], polyY[i]);
    glEnd();

    /* Mark vertices */
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(7.0f);
    glBegin(GL_POINTS);
        for (int i = 0; i < NUM_VERTS; i++)
            glVertex2f(polyX[i], polyY[i]);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 14.0, -2.0, 15.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q4: Polygon with Red Shading [Solid]");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
