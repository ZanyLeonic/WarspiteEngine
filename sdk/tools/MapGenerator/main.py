import math
import json
import sys
from enum import Enum

class TileTypes(Enum):
    Grass = 1
    Water = 2
    Stone = 3

progress = 0.0

class Vector2(object):
    x = 0
    y = 0   
    
    def __init__(self, x=0, y=0):
        if (not isinstance(x, float) or
            not isinstance(y, float)):
            raise TypeError("Only type float are supported for this operation.") 
        self.x = x
        self.y = y

    def __add__(self, o):
        return Vector2(self.x + o.x, self.y + o.y)
    
    def __sub__(self, o):
        return Vector2(self.x - o.x, self.y - o.y)

    def length(self):
        return math.sqrt((self.x**2) + (self.y**2))

def interpolate(a0, a1, w):
    return (a1 -a0) * w + a0

def randomGradient (ix, iy):
    random = 2910.0 * math.sin(ix * 21942.0 + iy * 171324.0 + 8912.0) * math.cos(ix * 23157.0 * iy * 217832.0 + 9758.0)
    return Vector2(math.cos(random), math.sin(random))

def dotGridGradient(ix, iy, x, y):
    gradient = randomGradient(ix, iy)
    dx = x - float(ix)
    dy = y - float(iy)
    return (dx*gradient.x + dy*gradient.y)

def perlin(x, y):
    x0 = int(x)
    x1 = x0 + 1
    y0 = int(y)
    y1 = y0 + 1

    sx = x - float(x0)
    sy = y - float(y0)

    n0 = dotGridGradient(x0, y0, x, y) 
    n1 = dotGridGradient(x1, y1, x, y)
    ix0 = interpolate(n0, n1, sx)

    n0 = dotGridGradient(x0, y1, x, y)
    n1 = dotGridGradient(x1, y1, x, y)
    ix1 = interpolate(n0, n1, sx)

    return interpolate(ix0, ix1, sy) 

size = (256, 256)
scale = 100.0
noise = []
world = []

for i in range(size[0]):
    noise.append([])
    for j in range(size[1]):
        noise[i].append(0)

for i in range(size[0]):
    for j in range(size[1]):
        noise[i][j] = perlin(i / scale, j / scale)
        progress = ((i / size[0]) * 100)
        print("Generating noise {0}% done". format(round(progress, 2)), end="\r")
        # print("\nNoise: {0}".format(noise[i][j]))

for i in range(size[0]):
    for j in range(size[1]):
        v = noise[i][j]
        if (v < -0.1):
            world.append(TileTypes.Stone.value)
        elif (v < 0.4):
            world.append(TileTypes.Water.value)
        else:
            world.append(TileTypes.Grass.value)

mapt = None
with open(sys.argv[1], 'r') as f:
    mapt = json.load(f)

if (map is None):
    raise IOError("Cannot load map file.")

mapt['layers'][0]['data'] = world

with open(sys.argv[1], 'w') as f:
    json.dump(map, f, indent = 3)