 import engine

 # Print currently implemented features
 print("Engine attributes:")
 for i in dir(engine):
     print("   %s" % str(i))

 ws = None

 # Create a new test object
 try:
     ob = engine.ObjectParams(140, 140, 32, 32, "TestObject", 1, 1, 0, 0, 0, "", "PythonObject", "TestObject")
     ws = engine.level.create_object("TestObject", ob)
 except Exception as e:
     print("An error occurred:\n%s" % (str(e)))

 # Print its info + location
 print(ws)
 print(ws.get_position())

 # Find the player and change their location
 na = engine.level.find_wobject("Player")
 na.set_position(engine.Vector2D(64,64))

 # Find the Object we just created
 da = engine.level.find_wobject("PythonObject")
 da.set_position(engine.Vector2D(0,0))

 # Compare them
 print("Comparing ws with da: ", end="")
 if da == ws:
     print("They are the same")
 else:
     print("They are different")