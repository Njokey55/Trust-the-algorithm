import tkinter as tk
import random
WINDOW_WIDTH = 400
WINDOW_HEIGHT = 600

CONTAINER_TOP = 70
CONTAINER_BOTTOM = 530
CONTAINER_LEFT = 100
CONTAINER_RIGHT = 300

CANDY_HEIGHT = 30
SPRING_BASE_Y = 530
SPRING_MAX_HEIGHT = 460  
SPRING_MIN_HEIGHT = 10

MAX_CANDIES = 14

COLORS = ["red", "green", "blue", "yellow", "orange", "purple", "pink", "cyan", "magenta", "lime", "teal", "lavender", "brown", "black"]

class CandyDispenser:
    def __init__(self, root):
        self.root = root
        self.root.title("Candy Dispenser Stack")

        self.stack = []

        self.main_frame = tk.Frame(root)
        self.main_frame.pack(padx=10, pady=10)

        self.canvas = tk.Canvas(self.main_frame, width=WINDOW_WIDTH, height=WINDOW_HEIGHT, bg="white")
        self.canvas.pack(side="left")

        control_frame = tk.Frame(self.main_frame,)
        control_frame.pack(side="left", padx=20)
        control_frame.pack_propagate(False)
        control_frame.config(width=400, height=600) #Fixed size

        self.top_label = tk.Label(control_frame, text="Top Candy: None", font=("Arial", 12))
        self.top_label.pack(pady=5)

        self.size_label = tk.Label(control_frame, text="Size: 0", font=("Arial", 12))
        self.size_label.pack(pady=5)

        self.empty_label = tk.Label(control_frame, text="Is Empty: Yes", font=("Arial", 12))
        self.empty_label.pack(pady=5)

        tk.Button(control_frame, text="PUSH", command=self.push_candy, width=15,bg="#8737bc").pack(pady=5)
        tk.Button(control_frame, text="POP", command=self.pop_candy, width=15,bg="#8737bc").pack(pady=5)
        #Draw initial graphics
        self.draw_container()
        self.draw_spring()
        self.draw_candies()

    def draw_container(self):
        self.canvas.create_rectangle(
            CONTAINER_LEFT, CONTAINER_TOP,
            CONTAINER_RIGHT, CONTAINER_BOTTOM,
            width=2, outline="black"
        )

    def draw_spring(self):
        self.canvas.delete("spring")
        self.canvas.delete("platform")

        spring_height = self.calculate_spring_height()
        y_bottom = SPRING_BASE_Y
        y_top = y_bottom - spring_height  #Top of the spring and bottom of platform

        x_center = (CONTAINER_LEFT + CONTAINER_RIGHT) // 2 #middle of the container
        y_top = y_bottom - spring_height 

        step = spring_height // 20
        amplitude = 20 # Amplitude of the spring coils; bigger= wider coils
        x = x_center - amplitude # Start from left side
        y = y_bottom # Start from bottom

        direction = 1 # Start moving right

        while y > y_top:
            next_x = x_center + amplitude * direction #alternates zigzag
            next_y = max(y_top, y - step)# Move up by step but not above top

            self.canvas.create_line(
                x, y,
                next_x, next_y,
                fill="black",
                width=2,
                tags="spring"
          ) 

            x, y = next_x, next_y #connect to next point
            direction *= -1

        self.canvas.create_rectangle(
            CONTAINER_LEFT + 3, y_top,
            CONTAINER_RIGHT - 3, y_top + 10,
            fill="gray", tags="platform"
        )

        self.platform_top = y_top 

    def draw_candies(self):
        self.canvas.delete("candy")
        if hasattr(self, "platform_top"):
            platform_y = self.platform_top
        else:
            platform_y = SPRING_BASE_Y - SPRING_MAX_HEIGHT

        for i, candy in enumerate(self.stack):
            y_top = platform_y - (i + 1) * CANDY_HEIGHT # Calculate top y position for each candy
            self.canvas.create_rectangle(
                CONTAINER_LEFT + 5,
                y_top,
                CONTAINER_RIGHT - 5,
                y_top + CANDY_HEIGHT,
                fill=candy,
                outline="black",
                tags="candy"
            )

    def push_candy(self):
        if len(self.stack) >= MAX_CANDIES:
            self.top_label.config(text="Error: Stack Overflow!", fg="red")
            return

        color = random.choice(COLORS)
        self.stack.append(color)
        self.update_scene()

    def pop_candy(self):
        if not self.stack:
            self.top_label.config(text="Dispenser is Empty", fg="blue")
            return

        self.stack.pop()
        self.update_scene()

    def calculate_spring_height(self):
        total_candy_height = len(self.stack) * CANDY_HEIGHT
        spring_height = SPRING_MAX_HEIGHT - total_candy_height
        return max(SPRING_MIN_HEIGHT, spring_height)

    def update_scene(self):
        self.draw_spring()
        self.draw_candies()
        self.update_info_display()

    def update_info_display(self):
    
        self.top_label.config(fg="black")

        if self.stack:
            self.top_label.config(text=f"Top Candy: {self.stack[-1]}") 
        else:
            self.top_label.config(text="Top Candy: None")

        self.size_label.config(text=f"Size: {len(self.stack)}")
        self.empty_label.config(text=f"Is Empty: {'Yes' if not self.stack else 'No'}")

root = tk.Tk()
app = CandyDispenser(root)
root.mainloop()
