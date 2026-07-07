import tkinter as tk
from tkinter import messagebox, ttk

class PriorityQueue:
    def __init__(self):
        self.queue = []

    def compute_priority(self, age, temp, bp,):
    
        base_priority = (0.40 * age) + (0.30 * temp) + (0.30 * bp)
        priority = base_priority

        if age <= 12:
            priority += 10
        if age <= 2:
            priority += 10

        return priority

    def add_patient(self, name, age, temp, bp ):
        priority = self.compute_priority(age, temp, bp,)
        self.queue.append((name, priority, age, temp, bp,))
        self.queue.sort(key=lambda x: x[1], reverse=True)

    def remove_patient(self, name):
        self.queue = [p for p in self.queue if p[0] != name]

    def update_patient(self, name, age, temp, bp,):
        self.remove_patient(name)
        self.add_patient(name, age, temp, bp,)

    def call_next(self):
        if self.is_empty():
            return None
        return self.queue.pop(0)

    def is_empty(self):
        return len(self.queue) == 0

    def get_all(self):
        return self.queue

pq = PriorityQueue()

def add_patient():
    try:
        name = name_entry.get()
        age = int(age_entry.get())
        temp = float(temp_entry.get())
        bp = int(bp_entry.get())

        if name == "":
            messagebox.showerror("Error", "Name cannot be empty")
            return

        if temp < 30 or temp > 42:
            messagebox.showerror("Error", "Temperature must be between 30 and 42 °C")
            return

        if bp < 50 or bp > 250:
             messagebox.showerror("Error", "Blood Pressure must be between 50 and 250 mmHg")
             return
 
        pq.add_patient(name, age, temp, bp,)
        refresh()
        clear_inputs()

    except:
        messagebox.showerror("Error", "Enter valid numeric values")

def update_patient():
    try:
        name = name_entry.get()
        age = int(age_entry.get())
        temp = float(temp_entry.get())
        bp = int(bp_entry.get())
        
        pq.update_patient(name, age, temp, bp)
        refresh()
        clear_inputs()
        messagebox.showinfo("Updated", f"Updated priority for {name}")

        if temp < 30 or temp > 42:
          messagebox.showerror("Error", "Temperature must be between 30 and 42 °C")
          return

        if bp < 50 or bp > 250:
            messagebox.showerror("Error", "Blood Pressure must be between 50 and 250 mmHg")
            return
    except:
        messagebox.showerror("Error", "Enter valid numeric values")

def call_next():
    patient = pq.call_next()
    if patient:
        messagebox.showinfo("Next Patient", f"Serving: {patient[0]}")
    else:
        messagebox.showinfo("Info", "Queue is empty")
    refresh()

def clear_inputs():
    name_entry.delete(0, tk.END)
    age_entry.delete(0, tk.END)
    temp_entry.delete(0, tk.END)
    bp_entry.delete(0, tk.END)

def load_selected_patient(event):
    selection = listbox.curselection()
    if not selection:
        return

    index = selection[0]
    patient = pq.get_all()[index]

    name, priority, age, temp, bp,= patient

    name_entry.delete(0, tk.END)
    name_entry.insert(0, name)

    age_entry.delete(0, tk.END)
    age_entry.insert(0, age)

    temp_entry.delete(0, tk.END)
    temp_entry.insert(0, temp)

    bp_entry.delete(0, tk.END)
    bp_entry.insert(0, bp)

def refresh():
    listbox.delete(0, tk.END)

    queue = pq.get_all()
    length = len(queue)

    length_label.config(text=f"Queue Length: {length}")

    if pq.is_empty():
        status_label.config(text="Queue Status: Empty")
    else:
        status_label.config(text="Queue Status: Not Empty")

    for i, p in enumerate(queue):
        name = p[0]
        priority = round(p[1], 2)
        text = f"{name:<25} | Priority: {priority:6.2f}"
        listbox.insert(tk.END, text)

def focus_next(event):
     event.widget.tk_focusNext().focus()
     return "break"

root = tk.Tk()
root.title("Hospital Priority Queue System")
root.geometry("1050x1020")

main_frame = tk.Frame(root, bg="#edf4f0")
main_frame.pack(padx=10, pady=10, fill="both", expand=True)

left_frame = tk.Frame(main_frame, bd=2, relief="groove", padx=10, pady=10, bg="#dceee5")
left_frame.grid(row=0, column=0, padx=20, sticky="n")

right_frame = tk.Frame(main_frame, bd=2, relief="groove", padx=10, pady=10, bg="#dceee5")
right_frame.grid(row=0, column=1, padx=20, sticky="n")

label_font = ("Arial", 12, "bold")
entry_font = ("Arial", 10)
button_font = ("Arial", 10, "bold")

tk.Label(left_frame, text="Patient Intake Form", font=label_font, fg="#2f5d50", bg="#dceee5").pack(pady=5)

tk.Label(left_frame, text="Patient Name", bg="#dceee5", fg="#2f5d50").pack(anchor="w", pady=(5, 3))
name_entry = tk.Entry(left_frame, width=25, bg="#DDFFF5")
name_entry.pack(pady=(0, 8))

tk.Label(left_frame, text="Age", bg="#dceee5", fg="#2f5d50").pack(anchor="w", pady=(5, 3))
age_entry = tk.Entry(left_frame, width=25, bg="#DDFFF5")
age_entry.pack(pady=(0, 8))

tk.Label(left_frame, text="Temperature", bg="#dceee5", fg="#2f5d50").pack(anchor="w", pady=(5, 3))
temp_entry = tk.Entry(left_frame, width=25, bg="#DDFFF5")
temp_entry.pack(pady=(0, 8))

tk.Label(left_frame, text="Blood Pressure", bg="#dceee5", fg="#2f5d50").pack(anchor="w", pady=(5, 3))
bp_entry = tk.Entry(left_frame, width=25, bg="#DDFFF5")
bp_entry.pack(pady=(0, 8))

name_entry.bind("<Return>", focus_next) 
age_entry.bind("<Return>", focus_next)
temp_entry.bind("<Return>", focus_next)
bp_entry.bind("<Return>", focus_next)
bp_entry.bind("<Return>", lambda e: add_patient())

tk.Button(left_frame, text="Add Patient", bg="#5a9c88", fg="#083B31", width=22, command=add_patient).pack(pady=4)
tk.Button(left_frame, text="Update Priority", bg="#5a9c88", fg="#083B31", width=22, command=update_patient).pack(pady=4)
tk.Button(left_frame, text="Call Next Patient", bg="#5a9c88", fg="#083B31", width=22, command=call_next).pack(pady=4)
tk.Button(left_frame, text="Clear Inputs", bg="#c97b7b", fg="#083B31", width=22, command=clear_inputs).pack(pady=4)

tk.Label(right_frame, text="Current Priority Queue", font=("Arial", 12, "bold"), fg="#2f5d50", bg="#dceee5").pack(pady=5)

status_label = tk.Label(right_frame, text="Queue Status: Empty", font=("Arial", 10, "bold"), fg="#2f5d50", bg="#dceee5")
status_label.pack(pady=2)

length_label = tk.Label(right_frame, text="Queue Length: 0", font=("Arial", 10, "bold"), fg="#2f5d50", bg="#dceee5")
length_label.pack(pady=2)

listbox = tk.Listbox(right_frame, width=85, height=18, bg="#f4fbf8", fg="#2f5d50", font=("Courier", 10))
listbox.pack()

listbox.bind("<<ListboxSelect>>", load_selected_patient)

root.mainloop()
