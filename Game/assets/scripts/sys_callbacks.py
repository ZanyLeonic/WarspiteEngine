import engine

def test():
    print("This is from a Python script!")

cb = engine.game.get_callback_handler()

cb.add_callback("pyTest", test)

if (cb.get_callback("pyTest")):
    print("Registered callback!")