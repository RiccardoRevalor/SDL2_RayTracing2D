# C, SLD2 RayTracing 2D Attempt
Libraries used: SDL2.h, math.h

## Main algorithm used: RayCasting algorithm
I draw all the rays from the light source and compute their paths, it they encounter an obstacle or exit from the screen, their path stops. <br>
This way, obstacles shadows are cast. <br>
Each ray is defined as a struct composed of a 2D Starting Point and an angle expressed in radiant. <br>
Circle light sources emit ray in every direction.  <br>
Since rays are manually placed and drawn, they are finite in number so this is obviously an approximation. <br>
By moving the mouse around and pressing the right button, you can move the light source(s) and the emitted rays will change.
![alt text](image.png)
