from game import Vector2D, WarspiteObject

v = Vector2D()
o = WarspiteObject()

v.set_x(10)
v.set_y(25)

o.set_position(v)

print (v)
print (o)
print (o.get_position())