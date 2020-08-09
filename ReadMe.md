# TesselationShaderExample 

Example code for three applications of tesselation shaders.
- Line tesselation
- Quad tesselation
- Surface tesselation

![alt text](doc/line.jpeg =100x20)
![alt text](doc/quad.jpeg =100x20)
![alt text](doc/surface.jpeg =100x20)


# Prerequisites

- GLFW
- [Magnum Engine](https://github.com/mosra/magnum) 

# Build

```
mkdir build
cmake ..
```

# Usage

Switching between schemas :
- Line : `key 1`
- Quad : `key 2`
- Surface : `key 3`

Increasing-Decreasing tesselation :
- Line : `key Up` , `key Down`
- Quad : 
    - Outer tesselation :  `key Up` , `key Down`
    - Inner tesselation :  `key Right` , `key Left`
- Surface :  `key Up` , `key Down`