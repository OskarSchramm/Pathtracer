
A very simple (slow) pathtracing demo
Supports:
Reflection
Refraction

Directional Light
Sky color
Spheres
AABBs

Material Types:
Normal
Glass
Mirror

Check scene.txt for how a scene text file should look like

Edit scene.txt, 
or make a new one and change in code, line: 22 in RayTracer.cpp

Edit CScene.h
line 173 & 174
for how many rays per pixel, and how many bounces you'd like

100 rays per pixel
and 2 max bounces
produces a pretty decent image
in ~60 seconds

scene_backup.png was rendered with: 
1000 rays per pixel
and 4 max bounces
very good image
in > 10 minutes
