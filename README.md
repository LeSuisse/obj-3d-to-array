obj-3d-to-array
===============

Copyright 2012 Thomas Gerbet
Licensed under the Apache License 2.0

What is this?
------------
It is a tool who convert [3D OBJ model] [OBJ SPEC] files into C arrays.
I wrote this tool for a small university project in which I could not use an OBJ loader.
This is really quick & dirty work and should be use with caution in real project.

Usage
------------
./ConvertObj -in model.obj -out model_array.h

The header result file could be use this way:
```c++
#include "model_array.h"

void createStructure(const float* points, int size) const {
	glVertexPointer(3, GL_FLOAT, 0, points);
	glDrawArrays(GL_TRIANGLES, 0, size/3);
}

createStructure(array_name, sizeof(array_name)/sizeof(float));
```

Known Limitations
------------
* Process only geometric vertices, objects and faces
* Only faces with 3 or 4 vertices are managed
* Probably more…


[OBJ SPEC]: http://www.martinreddy.net/gfx/3d/OBJ.spec "OBJ specifications"
