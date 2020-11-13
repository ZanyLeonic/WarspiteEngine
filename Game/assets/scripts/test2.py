import engine

print("Running callback script...")

def press_callback():
    print("A was pressed!")

def upress_callback():
    print("A was released!")

a = engine.inputh.add_action_keydown(4, press_callback)
b = engine.inputh.add_action_keyup(4, upress_callback)

if (a and b):
    print("Callbacks successfully registered!")
elif (a and not b):
    print("Up callback registered but down failed")
elif (not a and b):
    print("Down callback registered but up failed")
else:
    print("Callback registering failed.")
