# ============================================================
# ICS2311 CAT - Makefile
# Compiles all 6 OpenGL programs individually
#
# Requirements:
#   - GCC compiler (MinGW-w64 on Windows or GCC on Linux/macOS)
#   - FreeGLUT library installed
#
# Windows (MinGW-w64) compilation flags:
#   -lglut32 -lglu32 -lopengl32
#
# Linux compilation flags:
#   -lGL -lGLU -lglut
# ============================================================

# Compiler
CC = gcc

# Compiler flags (add -Wall for all warnings)
CFLAGS = -Wall

# ── Windows (MinGW-w64) Linker Flags ──
# Uncomment these lines for Windows:
# LIBS = -lglut32 -lglu32 -lopengl32

# ── Linux Linker Flags ──
# Uncomment these lines for Linux:
LIBS = -lGL -lGLU -lglut -lm

# Target executables
TARGETS = Q1a_midpoint_circle \
          Q1b_triangle_circumcircle \
          Q2_ellipse_floodfill \
          Q3_square_rotation \
          Q4_polygon_red_fill \
          Q5_parabola_midpoint \
          Q6_cone

# Build all targets
all: $(TARGETS)

# ── Individual compile rules ──
Q1a_midpoint_circle: Q1a_midpoint_circle.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "Compiled Q1a: Midpoint Circle"

Q1b_triangle_circumcircle: Q1b_triangle_circumcircle.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "Compiled Q1b: Triangle + Circumcircle"

Q2_ellipse_floodfill: Q2_ellipse_floodfill.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "Compiled Q2: Ellipse + Flood Fill"

Q3_square_rotation: Q3_square_rotation.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "Compiled Q3: Square Rotation"

Q4_polygon_red_fill: Q4_polygon_red_fill.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "Compiled Q4: Polygon Red Fill"

Q5_parabola_midpoint: Q5_parabola_midpoint.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "Compiled Q5: Parabola Midpoint"

Q6_cone: Q6_cone.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "Compiled Q6: Cone"

# Clean all compiled files
clean:
	rm -f $(TARGETS)
	@echo "Cleaned all executables"

.PHONY: all clean
