import tkinter as tk
from tkinter import messagebox
import heapq

BG_MAIN = "#E8F5F2"
BG_PANEL = "#D6EEE8"
BG_ENTRY = "#F4FFFC"
BTN_COLOR = "#9FD8C5"
TEXT_COLOR = "#2F6F5E"
ACCENT = "#6BBFA3"

class PriorityQueue:
    def __init__(self):
        self.queue = []
        self.counter = 0

    def add(self, priority, name, data):
        heapq.heappush(self.queue, (-priority, self.counter, name, data))
        self.counter += 1

    def remove(self):
        if self.heap:
            return heapq.heappop(self.heap)
        return None

    def is_empty(self):
        return len(self.heap) == 0

    def size(self):
        return len(self.heap)

    def get_all(self):
        return self.heap

pq = PriorityQueue()

def compute_priority(age, temp, bp, severity):
    priority = (age * 0.4) + (severity * 10) + (temp * 0.3) + (bp * 0.2)

    if age < 12:
        priority += 10
    if age <= 2:
            priority += 10

    priority += severity * 5


    return min(int(priority), 200)

def add_patient():
    try:
        name = name_entry.get()
        age = int(age_entry.get())
        temp = float(temp_entry.get())
        bp = int(bp_entry.get())
        severity = int(severity_entry.get())

        if not name:
            raise ValueError

        priority = compute_priority(age, temp, bp, severity)

        data = {
            "age": age,
            "temp": temp,
            "bp": bp,
            "severity": severity
        }

        pq.add(priority, name, data)
        update_display()
        clear_inputs()

    except:
        messagebox.showerror("Error", "Please enter valid patient data")

def call_next():
    patient = pq.remove()
    if patient:
        _, _, name, data = patient
        messagebox.showinfo("Next Patient", f"Calling: {name}")
        update_display()
    else:
        messagebox.showinfo("Queue Empty", "No patients in queue")

def update_patient():
    try:
        name = name_entry.get()
        age = int(age_entry.get())
        temp = float(temp_entry.get())
        bp = int(bp_entry.get())
        severity = int(severity_entry.get())

        pq.heap = [p for p in pq.heap if p[2] != name]
        heapq.heapify(pq.heap)

        priority = compute_priority(age, temp, bp, severity)
        data = {"age": age, "temp": temp, "bp": bp, "severity": severity}
        pq.add(priority, name, data)

        update_display()
        clear_inputs()
    except:
        messagebox.showerror("Error", "Invalid update data")

def clear_inputs():
    name_entry.delete(0, tk.END)
    age_entry.delete(0, tk.END)
    temp_entry.delete(0, tk.END)
    bp_entry.delete(0, tk.END)
    severity_entry.delete(0, tk.END)
    name_entry.focus()

def update_display():
    display.delete(0, tk.END)

    for p in sorted(pq.get_all(), reverse=True):
        priority = -p[0]
        name = p[2]
        display.insert(tk.END, f"Priority: {priority}  |  Name: {name}")

    q_len = pq.size()
    queue_len_label.config(text=f"Queue Length: {q_len}")

    if pq.is_empty():
        empty_label.config(text="Queue Status: EMPTY")
    else:
        empty_label.config(text="Queue Status: ACTIVE")

def focus_next(event):
    event.widget.tk_focusNext().focus()
    return "break"

def shortcut_add(event): add_patient()
def shortcut_update(event): update_patient()
def shortcut_next(event): call_next()
def shortcut_clear(event): clear_inputs()

root = tk.Tk()
root.title("Hospital Adaptable Priority Queue System")
root.geometry("1100x600")
root.configure(bg=BG_MAIN)

left_frame = tk.Frame(root, bg=BG_PANEL, padx=30, pady=20)
left_frame.pack(side="left", fill="both", expand=True)

right_frame = tk.Frame(root, bg=BG_PANEL, padx=30, pady=30)
right_frame.pack(side="right", fill="both", expand=True)

tk.Label(left_frame, text="Patient Intake", bg=BG_PANEL, fg=TEXT_COLOR, font=("Arial", 18, "bold")).pack(pady=10)

form_frame = tk.Frame(left_frame, bg=BG_PANEL)
form_frame.pack(pady=20)

def field(label, row):
    tk.Label(form_frame, text=label, bg=BG_PANEL, fg=TEXT_COLOR, font=("Arial", 11)).grid(row=row, column=0, sticky="w", pady=10, padx=(0,20))
    e = tk.Entry(form_frame, bg=BG_ENTRY, fg=TEXT_COLOR, width=25, relief="solid", bd=1)
    e.grid(row=row, column=1, pady=10, ipady=6)
    return e

name_entry = field("Name:", 0)
age_entry = field("Age:", 1)
temp_entry = field("Temperature:", 2)
bp_entry = field("Blood Pressure:", 3)
severity_entry = field("Severity (1–5):", 4)

tk.Label(form_frame, text="Arrival Mode:", bg=BG_PANEL, fg=TEXT_COLOR).grid(row=5, column=0, sticky="w", pady=10, padx=(0,25))
arrival_var = tk.StringVar(value="Walk-in")
arrival_menu = tk.OptionMenu(form_frame, arrival_var, "Walk-in", "Wheelchair", "Ambulance")
arrival_menu.config(bg=BG_ENTRY, fg=TEXT_COLOR, width=22)
arrival_menu.grid(row=5, column=1, pady=10)

conscious_var = tk.BooleanVar(value=True)
tk.Checkbutton(form_frame, text="Conscious", variable=conscious_var, bg=BG_PANEL, fg=TEXT_COLOR).grid(row=6, column=0, sticky="w", pady=(5, 10))

name_entry.bind("<Return>", focus_next)
age_entry.bind("<Return>", focus_next)
temp_entry.bind("<Return>", focus_next)
bp_entry.bind("<Return>", focus_next)
severity_entry.bind("<Return>", lambda e: add_patient())

btn_frame = tk.Frame(left_frame, bg=BG_PANEL)
btn_frame.pack(pady=25)

def btn(text, cmd):
    return tk.Button(btn_frame, text=text, command=cmd, bg=BTN_COLOR, fg=TEXT_COLOR,
                     font=("Arial", 11, "bold"), padx=15, pady=8, relief="flat")

btn("Add Patient", add_patient).grid(row=0, column=0, padx=10, pady=10)
btn("Update Patient", update_patient).grid(row=0, column=1, padx=10, pady=10)
btn("Call Next", call_next).grid(row=1, column=0, padx=10, pady=10)
btn("Clear Inputs", clear_inputs).grid(row=1, column=1, padx=10, pady=10)

tk.Label(right_frame, text="Priority Queue Display", bg=BG_PANEL, fg=TEXT_COLOR,
         font=("Arial", 18, "bold")).pack(pady=10)

display = tk.Listbox(right_frame, width=50, height=18, bg=BG_ENTRY,
                     fg=TEXT_COLOR, font=("Consolas", 11), relief="flat")
display.pack(pady=15)

queue_len_label = tk.Label(right_frame, text="Queue Length: 0", bg=BG_PANEL, fg=TEXT_COLOR, font=("Arial", 12))
queue_len_label.pack(pady=5)

empty_label = tk.Label(right_frame, text="Queue Status: EMPTY", bg=BG_PANEL, fg=TEXT_COLOR, font=("Arial", 12, "bold"))
empty_label.pack(pady=5)

root.bind("<Control-a>", shortcut_add)
root.bind("<Control-u>", shortcut_update)
root.bind("<Control-n>", shortcut_next)
root.bind("<Control-l>", shortcut_clear)
root.bind("<Escape>", shortcut_clear)

root.mainloop()
