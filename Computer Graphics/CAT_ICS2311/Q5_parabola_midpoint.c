/*
 * ICS2311 CAT - QUESTION 5 [SOLID VERSION]
 * Program: Parabola x = y² for -10 ≤ y ≤ 10 (Midpoint Method)
 *
 * FIX: Instead of GL_POINTS (isolated dots with gaps),
 *      we store all computed points and draw with GL_LINE_STRIP,
 *      which connects consecutive points with line segments.
 *      Result: a smooth, solid parabola curve with no gaps.
 *
 * MIDPOINT METHOD WORKING (in comments below in the algorithm):
 *   Parabola: x = y²  →  F(x,y) = x - y² = 0
 *   Decision parameter at midpoint between E(x+1,y) and NE(x+1,y+1):
 *     p = F(x+1, y+0.5) = (x+1) - (y+0.5)²
 *   p < 0  → inside parabola → East step  (p_new = p + 1)
 *   p >= 0 → outside         → NE step    (p_new = p + 1 - 2y - 2; y++)
 *   Initial: p₀ = F(1, 0.5) = 1 - 0.25 = 0.75
 *
 * SYMMETRY: For each (x, y) computed in upper half (y ≥ 0),
 *           plot (x, -y) for the lower half.
 */

#include <GL/glut.h>
#include <math.h>

/* Storage for upper-half points (y: 0 → 10) */
#define MAX_PTS 500
float upperX[MAX_PTS], upperY[MAX_PTS];
int   uCount = 0;

/*
 * computeParabolaPoints()
 * Runs the Midpoint Algorithm and stores points for the upper half.
 * The lower half is the mirror (same x, negated y).
 */
void computeParabolaPoints() {
    float x = 0.0f, y = 0.0f;
    float p = 0.75f;   /* p₀ = F(1, 0.5) = 1 - 0.25 = 0.75 */

    uCount = 0;
    upperX[uCount] = x;
    upperY[uCount] = y;
    uCount++;

    while (y <= 10.0f) {
        x++;   /* Always step in x */

        if (p < 0) {
            /*
             * Midpoint inside parabola → EAST step (y unchanged)
             * p_new = p + 1
             */
            p = p + 1.0f;
        } else {
            /*
             * Midpoint outside parabola → NORTHEAST step (y increases)
             * p_new = p + 1 - 2y - 2
             */
            p = p + 1.0f - 2.0f * y - 2.0f;
            y++;
        }

        upperX[uCount] = x;
        upperY[uCount] = y;
        uCount++;
    }
}

void drawAxes() {
    glColor3f(0.4f, 0.4f, 0.4f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
        glVertex2f(-5.0f,   0.0f); glVertex2f(105.0f,  0.0f);
        glVertex2f(  0.0f,-12.0f); glVertex2f(  0.0f, 12.0f);
    glEnd();
    glBegin(GL_LINES);
    for (int i = 10; i <= 100; i += 10) {
        glVertex2f((float)i, -0.4f); glVertex2f((float)i,  0.4f);
    }
    for (int j = -10; j <= 10; j++) {
        if (j != 0) {
            glVertex2f(-0.5f, (float)j); glVertex2f(0.5f, (float)j);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    glColor3f(0.1f, 0.4f, 0.9f);
    glLineWidth(2.0f);

    /*
     * Draw UPPER half of parabola (y >= 0) as a connected line strip.
     * GL_LINE_STRIP connects each point to the next — no gaps.
     */
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i < uCount; i++)
            glVertex2f(upperX[i], upperY[i]);
    glEnd();

    /*
     * Draw LOWER half (y <= 0) by mirroring: same x, negated y.
     * Traverse in reverse so the strip goes from vertex outward smoothly.
     */
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i < uCount; i++)
            glVertex2f(upperX[i], -upperY[i]);
    glEnd();

    /* Mark vertex (0,0) */
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-5.0, 105.0, -12.0, 12.0);
}

int main(int argc, char** argv) {
    computeParabolaPoints();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Q5: Parabola x=y^2 Midpoint Method -10<=y<=10 [Solid]");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}
