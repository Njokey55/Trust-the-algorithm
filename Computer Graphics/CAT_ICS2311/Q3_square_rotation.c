/*
 * ICS2311 CAT - QUESTION 3 [SOLID VERSION]
 * Program: Square A(0,4) B(3,4) C(4,0) D(0,0) — Translated then Rotated -76°
 *
 * This program already used GL_LINE_LOOP for the square (which is solid).
 * No fix needed for dotting — included here for completeness and consistency.
 *
 * TRANSFORMATIONS:
 *   1. Translate to centroid: tx = 1.75, ty = 2.0
 *   2. Rotate by θ = -76° (clockwise) around Z-axis
 *   3. Translate back
 *
 * OpenGL's glRotatef and glTranslatef handle the matrix math automatically.
 * glPushMatrix/glPopMatrix save and restore the matrix so only the
 * rotated copy is affected, while the original square stays in place.
 */

#include <GL/glut.h>
#include <math.h>

float squareX[] = { 0.0f, 3.0f, 4.0f, 0.0f };
float squareY[] = { 4.0f, 4.0f, 0.0f, 0.0f };
#define THETA -76.0f

float centroidX, centroidY;

void computeCentroid() {
    centroidX = (squareX[0]+squareX[1]+squareX[2]+squareX[3]) / 4.0f;  /* = 1.75 */
    centroidY = (squareY[0]+squareY[1]+squareY[2]+squareY[3]) / 4.0f;  /* = 2.0  */
}

/*
 * drawSquare()
 * GL_LINE_LOOP draws 4 sides and automatically closes back to the first vertex.
 * This is already solid — no gap issue.
 */
void drawSquare(float* xs, float* ys) {
    glBegin(GL_LINE_LOOP);
        glVertex2f(xs[0], ys[0]);  /* A(0,4) */
        glVertex2f(xs[1], ys[1]);  /* B(3,4) */
        glVertex2f(xs[2], ys[2]);  /* C(4,0) */
        glVertex2f(xs[3], ys[3]);  /* D(0,0) */
    glEnd();
}

void drawAxes() {
    glColor3f(0.4f, 0.4f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(-10.0f, 0.0f); glVertex2f(10.0f,  0.0f);
        glVertex2f(  0.0f,-10.0f); glVertex2f( 0.0f, 10.0f);
    glEnd();
    glBegin(GL_LINES);
    for (int i = -9; i <= 9; i++) {
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

    /* Original square in blue */
    glColor3f(0.0f, 0.0f, 0.8f);
    glLineWidth(2.0f);
    drawSquare(squareX, squareY);

    glColor3f(0.0f, 0.0f, 0.8f);
    glPointSize(7.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++) glVertex2f(squareX[i], squareY[i]);
    glEnd();

    /*
     * Apply transformation to draw rotated square (orange).
     * glPushMatrix saves state; transforms apply only inside this block.
     *   Step 1: Move centroid to origin
     *   Step 2: Rotate by -76°
     *   Step 3: Move centroid back
     */
    glPushMatrix();
        glTranslatef(centroidX, centroidY, 0.0f);   /* Step 3 (applied last) */
        glRotatef(THETA, 0.0f, 0.0f, 1.0f);         /* Step 2: rotate -76°   */
        glTranslatef(-centroidX, -centroidY, 0.0f); /* Step 1 (applied first)*/

        glColor3f(1.0f, 0.4f, 0.0f);
        glLineWidth(2.0f);
        drawSquare(squareX, squareY);

        glPointSize(7.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < 4; i++) glVertex2f(squareX[i], squareY[i]);
        glEnd();
    glPopMatrix();

    /* Mark rotation pivot (centroid) in red */
    glColor3f(0.8f, 0.0f, 0.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
        glVertex2f(centroidX, centroidY);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
}

int main(int argc, char** argv) {
    computeCentroid();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q3: Square Rotated -76 degrees [Solid]");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
