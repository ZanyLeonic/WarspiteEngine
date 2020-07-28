import engine

print(dir(engine))
print(engine.level)

try:
    print(engine.inputh.is_keydown(engine.WS_Scancode.A))
except Exception as e:
    print(e)