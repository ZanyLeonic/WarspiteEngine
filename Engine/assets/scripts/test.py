import engine

# Print currently implemented features
print("Engine attributes:")
for i in dir(engine):
    print("   %s" % str(i))

ws = None

try:
    ob = engine.ObjectParams(140, 140, 32, 32, "TestObject", 1, 1, 0, 0, 0, "", "", "")
    ws = engine.game.create_object("TestObject", ob)
except Exception as e:
    print("An error occurred:\n%s" % (str(e)))
    
print(ws)