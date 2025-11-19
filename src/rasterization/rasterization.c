#include "rasterization.h"
#include "../drawing/draw.h"
#include "../math/math.h"
#include "../io/io.h"
#include "../math/transforms.h"
#include "camera.h"

Model models[MODEL_COUNT] = {
    {
        .vertexBuffer = &vertexData[0],
        .vertexCount = 8,
        .indexBuffer = &indexData[0],
        .indexBufferCount = 36  // 12 triangles * 3 vertices each
    }
};

// Cube vertices (8 corners of a cube from -0.5 to 0.5)
Vec3 vertexData[VERTEX_DATA_TOTAL_SIZE_BYTES] = {
    // Front face
    {N_FP(0,0x8000), N_FP(0,0x8000), FP(0,0x8000)},  // 0: bottom-left-front
    {FP(0,0x8000),   N_FP(0,0x8000), FP(0,0x8000)},  // 1: bottom-right-front
    {FP(0,0x8000),   FP(0,0x8000),   FP(0,0x8000)},  // 2: top-right-front
    {N_FP(0,0x8000), FP(0,0x8000),   FP(0,0x8000)},  // 3: top-left-front
    
    // Back face
    {N_FP(0,0x8000), N_FP(0,0x8000), N_FP(0,0x8000)}, // 4: bottom-left-back
    {FP(0,0x8000),   N_FP(0,0x8000), N_FP(0,0x8000)}, // 5: bottom-right-back
    {FP(0,0x8000),   FP(0,0x8000),   N_FP(0,0x8000)}, // 6: top-right-back
    {N_FP(0,0x8000), FP(0,0x8000),   N_FP(0,0x8000)}, // 7: top-left-back
};

// Index buffer defining 12 triangles (2 per face, 6 faces)
uint16 indexData[INDEX_DATA_TOTAL_SIZE_BYTES] = {
    // Front face (z = +0.5)
    0, 1, 2,    0, 2, 3,
    
    // Back face (z = -0.5) 
    5, 4, 7,    5, 7, 6,
    
    // Left face (x = -0.5)
    4, 0, 3,    4, 3, 7,
    
    // Right face (x = +0.5)
    1, 5, 6,    1, 6, 2,
    
    // Top face (y = +0.5)
    3, 2, 6,    3, 6, 7,
    
    // Bottom face (y = -0.5)
    4, 5, 1,    4, 1, 0
};

ObjectInstance testObject = {
    .model = &models[0],
    .transform = {
        .pos = {FP(0,0),FP(0,0),FP(5,0)},
        .rotation = {FP(0,0),FP(0,0),FP(0,0)},
        .scale = {FP(0,0x8000),FP(0,0x8000),FP(0,0x8000)}
    }
};
ObjectInstance testObject2 = {
    .model = &models[0],
    .transform = {
        .pos = {FP(0,0),FP(0,0),FP(7,0)},
        .rotation = {FP(0,0),FP(0,0),FP(0,0)},
        .scale = {FP(0,0x8000),FP(0,0x8000),FP(0,0x8000)}
    }
};

int32 depthBuffer[TOTAL_PIXELS];

void clearDepthBuffer() {
    for (uint16 j = 0; j < PIXEL_BUFFER_Y_SIZE; j++)
    {
        for (uint16 i = 0; i < PIXEL_BUFFER_X_SIZE; i++)
        {
            depthBuffer[j*PIXEL_BUFFER_X_SIZE + i] = 0x7FFFFFFF; // largest possible positive 32 bit integer in twos complement
        }
    }
}
int32 getDepth(uint16 i, uint16 j) {
    return depthBuffer[j*PIXEL_BUFFER_X_SIZE + i];
}
void setDepth(uint16 i, uint16 j, int32 depth) {
    depthBuffer[j*PIXEL_BUFFER_X_SIZE + i] = depth;
}

void initRasterization() {
    clearDepthBuffer();
}

void drawModel(const ObjectInstance* object) {

    Model* model = object->model;

    for (int triangleIndex = 0; triangleIndex < model->indexBufferCount; triangleIndex += 3) {

        // get the verticies
        Vec3* v0 = model->vertexBuffer + *(model->indexBuffer + triangleIndex);
        Vec3* v1 = model->vertexBuffer + *(model->indexBuffer + triangleIndex + 1);
        Vec3* v2 = model->vertexBuffer + *(model->indexBuffer + triangleIndex + 2);
        
        // apply the objects transform matrix and then the camera transform to the verticies
        Vec3 v0_model;
        Vec3 v1_model;
        Vec3 v2_model;
        applyTransform(v0, &v0_model, &object->transform);
        applyTransform(v1, &v1_model, &object->transform);
        applyTransform(v2, &v2_model, &object->transform);

        Vec3 v0_t;
        Vec3 v1_t;
        Vec3 v2_t;
        applyTransform(&v0_model, &v0_t, &camera.cameraTransform);
        applyTransform(&v1_model, &v1_t, &camera.cameraTransform);
        applyTransform(&v2_model, &v2_t, &camera.cameraTransform);
        
        int32 view_z0 = v0_t.z;
        int32 view_z1 = v1_t.z;
        int32 view_z2 = v2_t.z;
        
        // check that the triangle is past the near clip plane but not past the far clip plane
        if (v0_t.z < camera.nearClip || v1_t.z < camera.nearClip || v2_t.z < camera.nearClip
            || v0_t.z > camera.farClip || v1_t.z > camera.farClip || v2_t.z > camera.farClip) 
                continue;
            
        // apply perspective: divide x and y components by z
        v0_t.x = fpDiv(v0_t.x, v0_t.z);
        v0_t.y = fpDiv(v0_t.y, v0_t.z);
        v1_t.x = fpDiv(v1_t.x, v1_t.z);
        v1_t.y = fpDiv(v1_t.y, v1_t.z);
        v2_t.x = fpDiv(v2_t.x, v2_t.z);
        v2_t.y = fpDiv(v2_t.y, v2_t.z);

        int32 z_rec_0 = fpDiv(FP(1,0), v0_t.z); 
        int32 z_rec_1 = fpDiv(FP(1,0), v1_t.z); 
        int32 z_rec_2 = fpDiv(FP(1,0), v2_t.z); 

        // do backface culling
        // if this determinant is more than 0, the verticies 
        // are oriented clockwise and turned away from the 
        // camera. Do not continue rasterization
        int32 clockDet = fpSub(fpMul(v1_t.x - v0_t.x, v2_t.y - v0_t.y), fpMul(v1_t.y - v0_t.y, v2_t.x - v0_t.x));
        if (clockDet >= 0) {
            continue;
        }

        // rescale so that near clip plane ends up at z=0, far clip plane at z=1
        v0_t.z = fpSub(v0_t.z, camera.nearClip);
        v1_t.z = fpSub(v1_t.z, camera.nearClip);
        v2_t.z = fpSub(v2_t.z, camera.nearClip);
        
        int32 clipLen = camera.farClip - camera.nearClip;
        v0_t.z = fpDiv(v0_t.z, clipLen);
        v1_t.z = fpDiv(v1_t.z, clipLen);
        v2_t.z = fpDiv(v2_t.z, clipLen);

        int32 d_z0 = fpMul(v0_t.z, z_rec_0);
        int32 d_z1 = fpMul(v1_t.z, z_rec_1);
        int32 d_z2 = fpMul(v2_t.z, z_rec_2);

        // transform the verticies from clip space (-1,1) to screen space
        int32 p0_x = FP_TOINT(fpMul(fpAdd(v0_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p0_y = FP_TOINT(fpMul(fpAdd(fpMul(v0_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        int32 p1_x = FP_TOINT(fpMul(fpAdd(v1_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p1_y = FP_TOINT(fpMul(fpAdd(fpMul(v1_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        int32 p2_x = FP_TOINT(fpMul(fpAdd(v2_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p2_y = FP_TOINT(fpMul(fpAdd(fpMul(v2_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        // calculate the differences
        int32 p01_x = p1_x - p0_x;
        int32 p12_x = p2_x - p1_x;
        int32 p20_x = p0_x - p2_x;
        int32 p01_y = p1_y - p0_y;
        int32 p12_y = p2_y - p1_y;
        int32 p20_y = p0_y - p2_y;
        
        // calculate the bounding box
        int32 xMin = max(min(min(p0_x,p1_x),p2_x),0);
        int32 xMax = min(max(max(p0_x,p1_x),p2_x),PIXEL_BUFFER_X_SIZE);
        int32 yMin = max(min(min(p0_y,p1_y),p2_y),0);
        int32 yMax = min(max(max(p0_y,p1_y),p2_y),PIXEL_BUFFER_Y_SIZE);
        
        int32 totalArea = p20_x * p01_y - p20_y * p01_x;
        if (totalArea == 0) continue;

        // TODO: The triangle rasterization inside checking can be done 2x as effectively
        for (int i = xMin; i < xMax; i++) {
            for (int j = yMin; j < yMax; j++) {

                // compute the edge functions to determine if the point (i,j) is inside the triangle, using cross products
                int32 area0 = (i-p0_x) * p01_y - (j-p0_y) * p01_x;
                int32 area1 = (i-p1_x) * p12_y - (j-p1_y) * p12_x;
                int32 area2 = (i-p2_x) * p20_y - (j-p2_y) * p20_x;

                if (area0 <= 0 && area1 <= 0 && area2 <= 0) {
                    
                    // calculate the depth using barycentric coordinates
                    // Mat3x4 eqSystBaryCentric = {
                    //     .vals = {
                    //         FP(p0_x,0), FP(p1_x,0), FP(p2_x,0), FP(i,0),
                    //         FP(p0_y,0), FP(p1_y,0), FP(p2_y,0), FP(j,0),
                    //         FP(1,0),    FP(1,0),    FP(1,0),    FP(1,0)
                    //     }
                    // };
                    // reduce(&eqSystBaryCentric); // do gauss jordan reduction to calculate the barycentric coords
                    // int32 b0 = get(&eqSystBaryCentric, 0, 3);
                    // int32 b1 = get(&eqSystBaryCentric, 1, 3);
                    // int32 b2 = get(&eqSystBaryCentric, 2, 3);

                    int32 b0 = fpDiv(FP(area0,0),FP(totalArea,0));
                    int32 b1 = fpDiv(FP(area1,0),FP(totalArea,0));
                    int32 b2 = fpDiv(FP(area2,0),FP(totalArea,0));

                    
                    
                    // linearly interpolate the depth
                    int32 interpolatedDepth = fpMul(b0,d_z0) + fpMul(b1,d_z1) + fpMul(b2,d_z2);
                    int32 interpolatedOneOverZ = fpMul(b0,z_rec_0) + fpMul(b1,z_rec_1) + fpMul(b2,z_rec_2);
                    
                    if (interpolatedOneOverZ == 0) continue;
                    int32 depth = fpDiv(interpolatedDepth,interpolatedOneOverZ);

                    if (depth < getDepth(i,j)) {
                        // draw(i,j,0xFF);
                        draw(i,j,(uint8)triangleIndex+8);
                        setDepth(i,j,depth);
                    }
                }
            } 
        } 
    }
}