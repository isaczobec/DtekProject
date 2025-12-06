# Software rasterizer + Zarch-like game

## 2 Objective and Requirements
*These paragraphs should answer the questions: What should your project do and which are the main requirements?*

The main practical function of the Software Rasterizer is to display and project 3D graphics to the 2D screen.
In more specific terms, the input to the rasterizer is a collection of 3D objects residing somewhere in the world 
space, and the position and rotation of a camera. The output is a buffer of bytes that represent a 2D image/
projection of the 3D world.

We originally required that the software rasterizer must produce a reasonable amount of frames per second, for a smooth 
experience. However, due to time constraints we could not get a good fps, which caused us to change what game we made.

Originally, we had planned to make a copy of the game Zarch, a 3d shoot'em up from the 80's. But, due to the low frame rate of the rasterizer, we decided that we should go for something turn based instead. Thus we decided to make chess instead.

The requirements for the chess engine is that it should be able to do all legal moves in chess, which includes for example: en passant, check, checkmate, castling and promotion.
 
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

the values of $\alpha, \beta, \gamma$ are obtained by using a cross product to calculate the area of each subtriangle (this area divided by the total area of the triangle is equal to the barycentric coordinate). 

However, the $Z$ coordinate of a point is not linear over a triangle. Instead, we use the following formula to interpolate over it:

$$Z_{pixel} = \frac{1}{\alpha/Z_1 + \beta/Z_2 + \gamma/Z_3}$$

(we used [this](https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes.html) page as a reference).

#### Light value calculation

To give the models a sense of depth, it is nice to make each face have a reasonable light value. This was achieved by calculating the normal vector $N$ for each triangle using a cross product of two of its edge vectors. Then, we define a global ''light direction'' (unit) vector $L$, which signifies from where the light in the world is coming. We then compute the dot product $N \cdot L$ of theese, which is equal of the cosine of the angle between them. The more negative the value is, the greater the angle between the light vector and the normal, and the more the face should be illuminated. We simply multiply each channel of the color of a face with this light value to get the final color.

### Implementation

To perform the math mentioned above, we need to be able to store fractional values. To do this, we decided upon using 16.16 fixed point signed arithmetic. this means that bit 31 has a value of $-2^{15}$, and bits 30 to 0 have a value of $2^{k-16}$. We then implemented basic operations and then vector operations in this format, and functions to convert between regular integers to fixed point.

To store the 3D models in memory, we created a `vec3` struct to represent an arbitrary point or vertex. theese were be stored in an array, representing a mesh. Also, we stored an index buffer for each model, which contains groups of 3 indicies into the array of vertecies, to indicate a triangle. This saves memory space and prevents storing the same vertex multiple times in some cases. The models will live in static memory for the duration of the program.

We also wrote a python script to parse/convert `.obj` files to theese arrays, so that we dont have to write the values ourselves.

For the rasterization: We simply write calculated color values to the `320x240` VGA-buffer. For the rasterization, we iterate iterate over all triangles that end up in the screen space, and iterate over the pixels inside the bounding box of the triangle, and do the inside-test for each one. Triangles failing the backface-culling test are be skipped.

Instead of computing the actual matricies for transforming all the verticies into world space, we only store the necessary values for each object and perform the necessary calculations. This is because the matrix operations sometimes calculate redundant values, and may use more memory than needed.

The depth value is stored in a depth buffer, which holds the same amount of pixels as in the screen. Before we overwrite the color/pixel buffer with a pixel in a triangle, we check that the depth is less than the value in the depth buffer, and if so, we overwrite the depth buffer with the new value. Before we start rendering a new frame, we clear the depth buffer (reset all pixels to the max value).

The instances of different models (object instances) were stored in either a tightly packed array belonging to a chunk (a partition of the world) if it is close enough to the camera, otherwise it is stored in a single tightly packed array. The point of this was to be able to only consider objects in chunks that are close enough to the player when rendering, but after changing our plans from Zarch to Chess the need for this disappeared. In addition, since the address of object instances may change over its lifetime (if it comes into render distance or switches chunks) might change, objects are refered to using an `ObjectHandle` struct with a managed pointer to an object instance. 

### Chess Engine

In order to make the chess engine runnable on the dtek board, we chose to use multiple uint64s to represent the board Where bit 0 is A8, increasing to the right and then going to the next rank, ending at bit 63 as H1. One for each type of piece per side and one for each side in total and one for every piece. This lets us use bitwise operators in order to compare boardstates. For example, to see if apiece is captured, we can simple AND the position we moved to with the enemy board in order to see if a piece was present. Similarly, we can use bitshifts to update positions, for example, moving a white pawn forwards is equivalent to shifting its position right by 8. This requires a couple extra rules so a piece can't walk out of the board and appear on the other side.

When a move is made, we check all the legal moves of the selected tile, and look if the attempted move was legal. The conditions that are checked is it a move that piece could make, for example a rook can't go diagonal, does the move put your own king in check, is the piece blocked in a direction, does the piece capture an enemy and if applicable, special moves such as en passant or castling.

The engine sets special flags depending on the current game state. Normally, it's inProgress, if your king is in check, it's check, if you've been checkmated, it's checkmate and if you have to promote a piece it's promote.

Checking if the king is in check is done by iterating through every type of piece, placing one on the same square as your king, and checking if it could capture an enemy piece that is of the same type as itself. This is done so we can avoid iterating through every enemy piece and checking their legal moves, instead we just have to check the legal move of 5 different pieces, which on average is much faster.

Castling is essentially just a hardcoded check if the king or corrosponding rook has moved this game, and if not checking that all squares between them are clear and that the king does not move through check.

En Passant is implemented by having an "enPassantBoard", which updates the corrosponding bit if a pawn is moved two steps forward, resetting on the next turn. This board is then checked when looking at the legal moves of a pawn.

Checkmate is implemented by checking that no enemy piece has any legal moves, this is only checked if the enemy king is in check, as it is very computationally heavy.

Promotion is very simple, if a pawn moves to the final rank, the move is interrupted and the gamestate flag is set to promotion. Then the function promote, takes which piece to promote to, validates that it's a piece that you can promote to, and finishes the move.

## 4 Verification 
*The verification paragraph should answer: How did you test or verify that you solution is correct?*

Since our project is of a very visual nature, we knew that the software rasterizer was working when it displayed images that are reasonable and mirror what we intend to render. For example, it was easy to see when depth ordering and perspective projection began working correctly.

For the chess_engine, in development, we set up test scenarios that tested the thing that was being worked on. The bugs that slipped through there are then easily findable when trying to play the actual game, for example by trying to castle it's easy to find if you are unable to castle.

## 5 Contribution

We divided the work on this project very cleanly, Isac made the rasterizer, while Linus made the underlying chess engine. We had some debugging and feedback on eachothers code, but since we both had the best understanding of our own work, it was still fixed by the one who wrote that part of the code.

## 6 Reflecitions

We learned a lot from the project, e.g. how a compiler toolchain works, how c-code corresponds to machine code instructions, and how to structure a low level c project.


The whole chunking system would likely have generated large performance benefits if we would have created our original game idea, where objects would often have moved in and out of view and render distance. The chunks allow visible objects to be tightly packed in memory, while being able to check in $O(1)$ time check if all objects in a chunk should be rendered (determine if the camera is angled toward or away from the chunk, skip trying to render the objects in its array if it is facing away). However, since all objects are now constantly visible, the only effect of the chunk system was rather to marginally decrease performance. 
