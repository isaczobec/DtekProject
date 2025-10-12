# Software rasterizer + Zarch-like game

## 2 Objective and Requirements
*These paragraphs should answer the questions: What should your project do and which are the main requirements?*

The main practical function of the Software Rasterizer is to display and project 3D graphics to the 2D screen.
In more specific terms, the input to the rasterizer is a collection of 3D objects residing somewhere in the world 
space, and the position and rotation of a camera. The output is a buffer of bytes that represent a 2D image/
projection of the 3D world.

We require that the software rasterizer must produce a reasonable amount of frames per second, for a smooth 
experience. However, due to time constraints we do not require more sophisticated 3D effects, such as textures, 
ambient occlusion, or shadows.

For the game, we intend to create a simplified copy of the game Zarch (1987), a 3d shoot 'em up. In Zarch, the goal is to defend a map from waves of enemy crafts, with each wave more dangerous than the last.
When an enemy hits the ground, everything nearby gets infected. The player loses if the entire map gets infected or if they get hit by an enemy. 
The player controls a ship with a thruster that causes it to fly straight up. The player can control pitch and yaw in order to navigate with the ship. The ship is equpped with a rapid-fire autocannon.
We intend to include all of the above, but there is alot of other features in Zarch that could be included if we have the time.


## 3 Solution 
*The solution paragraph should state: How did you design and implement your solution?*

### The maths

#### Triangle rasterization
Starting from the lowest level, the rasterizer draws pixels to the screen by checking if individual pixels lies
within a triangle. Thus, all meshes of 3D objects will be made up of a collection of triangles, formed by a collection of vertecies.

The way we determine if a point lies inside a triangle is using the dot product. To see if the 2D vector $Z$ lies
within the triangle made up by the points $P_1, P_2, P_3$ (the order is important), we begin by checking which side of each individual side of the triangle lies on. If $P_2 - P_1$ has the components $(x_{12}, y_{12})'$, then $Z$ is on the left side of the first side iff $(y_{12}, -x_{12}) \cdot (Z - P_1) > 0$. If this is true for all three sides, and the points ${P_1, P_2, P_3}$ lie in a clockwise manner, then $Z$ lies inside the triangle. This follows from triangles being convex shapes.

We would like to avoid rasterizing triangles that are oriented in a clockwise manner, because as we will see, theese triangles are facing away from the camera and should never be visible if we have constructed our meshes correctly. We can check if our triangle is counter oriented counter clockwise using determinants. If the points of the triangle are ${(x_1, y_1), (x_2, y_2), (x_3, y_3)}$, then it is counter clockwise iff 

$$\det \left| \begin{array}{cc}x_1 - x_3 & x_3 - x_2 \\ y_1 - y_3 & y_3 - y_2\end{array} \right| > 0.$$

We skip a rasterizing a triangle immediately if it fails this test. This is called backface culling.

#### Transforms

The next issue is detemining which objects residing in the world space that should be projected onto the screen; the objects must be transformed from *world space* into *screen space*. This amounts to transforming the coordinates of all objects the camera can see into a box with depth (z) 1, and width (x) and height (y) 2, such that the dimensions of the box are from -1 to 1 for x and y, and 0 to 1 for z. Everything that ends up in this box will be rasterized and visible on the screen.

This process is done by using matrix transforms. The process for transforming a single object into screen space is as follows: We must begin by transforming the object into world space, since the objects vertex coordinates are stored with their origin being at $\vec{0}$, with no scaling or rotation. This is done by first applying a scaling matrix $S_W$:
$$
S_W=
\left(
\begin{array}{cccc}
    s_x & 0 & 0 & 0 \\
    0 & s_y & 0 & 0 \\
    0 & 0 & s_z & 0 \\
    0 & 0 & 0   & 1 \\
\end{array}
\right)
$$
where $s_x$ is the scaling factor for the x-axis, and so on. Next we must apply rotation around the x, y or z axes of the objects. This is done by multiplying with rotation matricies. For instance, rotating $\phi$ degrees on the x-axis is done by multiplying with the rotation matrix $R_W^{(x)}$:

$$
R_W^{(x)}=
\left(\begin{array}{ccc}
1 & 0 & 0 \\
0 & \cos (\phi) & -\sin (\phi) \\
0 & \sin (\phi) & \cos (\phi)
\end{array}\right) 
.$$

for y and z the matricies are similair. For position in the world, we multiply with a "world position" matrix $P_W$:
$$
P_W=
\left(
\begin{array}{cccc}
    1 & 0 & 0 & p_x \\
    0 & 1 & 0 & p_y \\
    0 & 0 & 1 & p_z \\
    0 & 0 & 0   & 1 \\
\end{array}
\right)
.$$
Note that for the vertex positions, we include a fourth dimension component that is always set to 1. this means that multiplying a vertex $V$ with this matrix is equivalent to adding $(p_x, p_y, p_z)$ to it.

So to bring the vertex $V$ to world space, we calculate:
$$V' = P_W R_W^{(x)}R_W^{(y)}R_W^{(z)} S_W V .$$

Next, we must move only what is visible in camera space to screen space. The first step is to, similairly to above, apply a position matrix containing the negative position of the camera, to move the camera to coordinates $\vec{0}$. Next, we apply the inverse rotations of the camera, like before. Now, we divide the verticies x and y positions by their $Z$ value to compensate for perspective: things far away appear closer togheter. Next, we move the camera back, so that the "near clip plane" intersects the $XY$-plane. Next, we apply a scale matrix so that the near clip plane occupies $(-1,1) \times (-1,1)$ in the $XY$-plane, and the far clip plane intersects the corresponding space in the plane $Z=1$.

#### Depth interpolation

After transforming all triangles to this space, we must rasterize them one by one. To make sure that objects that are behind others are not drawn on top of theese, we need to keep track of and compare the depths of different points on theese triangles. To determine the depth of an arbitrary point inside a triangle, we calculate its barycentric coordinates $\alpha, \beta, \gamma$ for the point $X$, which satisfy for a triangle with points $P_1, P_2, P_3$

$$\alpha, \beta, \gamma = 1$$
$$\alpha P_1 + \beta P_2 + \gamma P_3 = X$$

the values of $\alpha, \beta, \gamma$ are obtained by solving the linear equation system. To compute the Z value of $X$, we use the weigthed average of the Z-value of points in the triangle, weighted by the barycentric coordinates. 

### Implementation

To store the 3D models in memory, we will create a `vec3` struct to represent an arbitrary point or vertex. theese will be stored in an ordered list, representing a mesh. Optionally, we will store an index buffer for each model, which contains groups of 3 indicies into the array of vertecies, to indicate a triangle. This saves memory space and prevents storing the same vertex multiple times in some cases. The models will live in static memory for the duration of the program.

We may also write a script to parse/convert `.obj` files to theese arrays, so that we dont have to write the values ourselves.

For the rasterization: We will write to the `320x240` VGA-buffer, and swap the front and back buffers once we have finnished rendering a frame. For the rasterization, we will iterate over all triangles that end up in the screen space, and iterate over the pixels inside the bounding box of the triangle, and do the inside-test for each one. Triangles failing the backface-culling test will automatically be skipped.

Instead of computing the actual matricies for transforming all the verticies into world space, we will only store the necessary values for each object and perform the necessary calculations. This is because the matrix operations sometimes calculate redundant values, and may use more memory than needed.

The depth value will be stored in a depth buffer, while holds the same amount of pixels as in the screen. Before we overwrite the color/pixel buffer with a pixel in a triangle, we check that the depth is less than the value in the depth buffer, and if so, we overwrite the depth buffer with the new value.

### Gameplay and game functions

For the functions of the game, we will reuse a lot of the math from the rasterizer in order to implement the game logic. For example using the already implemented vec3 struct in order to note player and enemy position, or a line pointing from the nose of the ship to see whether a shot hits an enemy or not.

Random numbers, used for example for map generation and enemy spawns, will be implemented using xorshift. It xors an input with a bitshifted copy of itself repeatedly. This is chosen as it's very time and memory efficient, which is exactly what we need for real-time number generation. The resulting number is nowhere close to truly random, but it is good enough for what we need. An example of xorshift in C can be seen below.

```
uint32_t xorshift(uint32_t x)
{
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}
```

For input, the current idea is to get a joystick for controlling the ship, which will interact with the dtek-board using the GPIO pins. moving the joystick will control yaw and pitch while pressing down the joystick will use the booster and using the dtek-boards button will fire the weapon. In order to convert the analog input of a joystick to digital for the GPIO connections on the dtek-board, an MCP3008 chip, an anolog to digital converter will be used. The input method is subject to change if we find another solution that we think is better.

## 4 Verification 
*The verification paragraph should answer: How did you test or verify that you solution is correct?*


Since our project is of a very visual nature, we will know that the software rasterizer is working when it displays images that are reasonable and mirror what we intend to render. We might also target a certain framerate as a performance target, for instance 15 fps. We will have a couple tests for edge-cases, in order to check that the rasterizer performes as expected.

For the game aspect, there will be tests for different scenarios that can occur, for example hitting an enemy, controlling the ship or when the entire map gets infected. Some test cases will be written to test this easieer.

## 5 Contributions

The current intention is that Isac will mainly be focusing on the implementation of the Rasterizer and graphics while Linus will focus on the implementation of game functions and input. This is not set in stone and we will both collaborate a lot on both parts of the project.

## 6 Reflections

There is quite a bit to do on this project, so some parts of the game or input might get cut down if we do not have the time, but the main focus is to have a rasterizer and being able to display it. The game can be changed to be very simple if we run out of time. 

