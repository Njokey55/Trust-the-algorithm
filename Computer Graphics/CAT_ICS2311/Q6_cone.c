/*
 * ICS2311 CAT - QUESTION 6 [SOLID VERSION]
 * Program: Cone — radius=3, height=4, base center at (2,3)
 * Outer lining: Bold Red (#FF0000)
 * Fill: Yellow (#FFFF00)
 *
 * The cone (2D side-view) was already drawn with GL_LINES and GL_TRIANGLE_FAN
 * so no dotting issue existed. This version is clean and consistent.
 *
 * STRUCTURE:
 *   - Base: ellipse centered at (2,3), rx=3, ry=0.6 (foreshortened for 3D look)
 *   - Apex: (2, 7) — directly above center at height 4
 *   - Sides: lines from apex to left/right edges of base
 *
 * DRAWING ORDER (back-to-front for correct layering):
 *   1. Yellow filled triangle (body)
 *   2. Yellow filled ellipse (base)
 *   3. Red bold outline on top (sides + base ellipse rim)
 */

#include <GL/glut.h>
#include <math.h>

#define BASE_CX  2.0f
#define BASE_CY  3.0f
#define CONE_R   3.0f
#define CONE_H   4.0f
#define APEX_X   BASE_CX
#define APEX_Y   (BASE_CY + CONE_H)   /* = 7 */
#define BASE_RX  CONE_R
#define BASE_RY  (CONE_R * 0.2f)
#define SEGS     360    /* Segments for smooth ellipse */

/*
 * drawFilledEllipse()
 * Draws a solid filled ellipse using GL_TRIANGLE_FAN.
 * Center point is the hub; perimeter points fan around it.
 *   x(t) = cx + rx*cos(t)
 *   y(t) = cy + ry*sin(t)
 */
void drawFilledEllipse(float cx, float cy, float rx, float ry) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);  /* Hub of the fan */
        for (int i = 0; i <= SEGS; i++) {
            float a = 2.0f * M_PI * i / SEGS;
            glVertex2f(cx + rx*cos(a), cy + ry*sin(a));
        }
    glEnd();
}

/*
 * drawEllipseOutline()
 * Draws a solid ellipse outline using GL_LINE_LOOP.
 * Connects all perimeter points — no gaps.
 */
void drawEllipseOutline(float cx, float cy, float rx, float ry) {
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < SEGS; i++) {
            float a = 2.0f * M_PI * i / SEGS;
            glVertex2f(cx + rx*cos(a), cy + ry*sin(a));
        }
    glEnd();
}

void drawAxes() {
    glColor3f(0.4f, 0.4f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(-4.0f, 0.0f); glVertex2f(10.0f,  0.0f);
        glVertex2f( 0.0f,-2.0f); glVertex2f( 0.0f, 12.0f);
    glEnd();
    glBegin(GL_LINES);
    for (int i = -3; i <= 9; i++) {
        if (i != 0) {
            glVertex2f((float)i, -0.1f); glVertex2f((float)i,  0.1f);
            glVertex2f(-0.1f, (float)i); glVertex2f( 0.1f, (float)i);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    /* ── STEP 1: Yellow filled cone body (triangle) ── */
    glColor3f(1.0f, 1.0f, 0.0f);   /* Yellow #FFFF00 */
    glBegin(GL_TRIANGLES);
        glVertex2f(BASE_CX - CONE_R, BASE_CY);  /* Left base edge (-1, 3) */
        glVertex2f(BASE_CX + CONE_R, BASE_CY);  /* Right base edge (5, 3) */
        glVertex2f(APEX_X, APEX_Y);             /* Apex (2, 7) */
    glEnd();

    /* ── STEP 2: Yellow filled base ellipse ── */
    glColor3f(1.0f, 1.0f, 0.0f);
    drawFilledEllipse(BASE_CX, BASE_CY, BASE_RX, BASE_RY);

    /* ── STEP 3: Bold Red outline (sides + base ellipse) ── */
    glColor3f(1.0f, 0.0f, 0.0f);   /* Red #FF0000 */
    glLineWidth(3.0f);              /* Bold */

    /* Left slanting side: base-left → apex */
    glBegin(GL_LINES);
        glVertex2f(BASE_CX - CONE_R, BASE_CY);
        glVertex2f(APEX_X, APEX_Y);
        /* Right slanting side: base-right → apex */
        glVertex2f(BASE_CX + CONE_R, BASE_CY);
        glVertex2f(APEX_X, APEX_Y);
    glEnd();

    /* Base ellipse outline — solid loop */
    drawEllipseOutline(BASE_CX, BASE_CY, BASE_RX, BASE_RY);

    /* Mark key points */
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(6.0f);
    glBegin(GL_POINTS);
        glVertex2f(BASE_CX, BASE_CY);  /* Base center (2,3) */
        glVertex2f(APEX_X,  APEX_Y);   /* Apex (2,7) */
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-4.0, 10.0, -2.0, 12.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q6: Cone r=3 h=4 Base(2,3) - Red Outline Yellow Fill [Solid]");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
