# C, SLD2 RayTracing 2D Attempt
Libraries used: SDL2.h, math.h

## Main algorithm used: RayCasting algorithm
I draw all the rays from the light source and compute their paths, it they encounter an obstacle or exit from the screen, their path stops. <br>
This way, obstacles shadows are cast. <br>
Each ray is defined as a struct composed of a 2D Starting Point and an angle expressed in radiant. <br>
Circle light sources emit ray in every direction.  <br>
Since rays are manually placed and drawn, they are finite in number so this is obviously an approximation. <br>
![alt text](image.png)
