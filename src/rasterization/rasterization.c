#include "rasterization.h"
#include "../drawing/draw.h"
#include "../math/math.h"
#include "../io/io.h"
#include "../math/transforms.h"

Model models[MODEL_COUNT] = {
    {
        .vertexBuffer = (Vec3*)&vertexData,
        .vertexCount = 3,
        .indexBuffer = (uint16*)&indexData,
        .indexBufferCount = 3
    }
};
Vec3 vertexData[VERTEX_DATA_TOTAL_SIZE_BYTES] = {
    {FP(0,0),FP(0,0x8000),FP(0,0)},
    {N_FP(0,0x8000),N_FP(0,0x8000),FP(0,0)},
    {FP(0,0x8000),N_FP(0,0x8000),FP(0,0)},
};
uint16 indexData[INDEX_DATA_TOTAL_SIZE_BYTES] = {
    0,1,2,
};

ObjectInstance testObject = {
    .model = &models[0],
    .transform = {
        .pos = {FP(0,0),FP(0,0),FP(0,0)},
        .rotation = {FP(0,0),FP(0,0),FP(0,0)},
        .scale = {FP(1,0),FP(1,0),FP(1,0)}
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
        
        // apply the objects transform matrix to the verticies
        Vec3 v0_t;
        Vec3 v1_t;
        Vec3 v2_t;
        applyTransform(v0, &v0_t, &object->transform);
        applyTransform(v1, &v1_t, &object->transform);
        applyTransform(v2, &v2_t, &object->transform);


        // do backface culling
        // if this determinant is more than 0, the verticies 
        // are oriented clockwise and turned away from the 
        // camera. Do not continue rasterization
        int32 clockDet = fpSub(fpMul(v0_t.x - v2_t.x, v2_t.y - v1_t.y), fpMul(v2_t.x - v1_t.x, v0_t.y - v2_t.y));
        if (clockDet >= 0) {
            // print("counter clockwise, returning \n");
            return;
        }

        // print("\n----\n");
        // print_dec(fpAdd(1,v0.x));
        // print(",");
        // print_dec(fpAdd(1,v0.y));
        // print("\n");
        // print_dec(fpAdd(1,v1.x));
        // print(",");
        // print_dec(fpMul(fpAdd(1,v1.y),2));
        // print("\n");
        // print_dec(fpMul(fpAdd(1,v2.x),2));
        // print(",");
        // print_dec(fpMul(fpAdd(1,v2.y),2));

        // transform the verticies from clip space (-1,1) to screen space
        int32 p0_x = FP_TOINT(fpMul(fpAdd(v0_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p0_y = FP_TOINT(fpMul(fpAdd(fpMul(v0_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        int32 p1_x = FP_TOINT(fpMul(fpAdd(v1_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p1_y = FP_TOINT(fpMul(fpAdd(fpMul(v1_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        int32 p2_x = FP_TOINT(fpMul(fpAdd(v2_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p2_y = FP_TOINT(fpMul(fpAdd(fpMul(v2_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        // print("\n----\n");
        // print_dec(p0_x);
        // print(",");
        // print_dec(p0_y);
        // print("\n");
        // print_dec(p1_x);
        // print(",");
        // print_dec(p1_y);
        // print("\n");
        // print_dec(p2_x);
        // print(",");
        // print_dec(p2_y);
        
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
        
        // print("\n----\n");
        // print_dec(xMin);
        // print(",");
        // print_dec(xMax);
        // print("\n");
        // print_dec(yMin);
        // print(",");
        // print_dec(yMax);
        
        // TODO: The triangle rasterization inside checking can be done 2x as effectively
        for (int i = xMin; i < xMax; i++) {
            for (int j = yMin; j < yMax; j++) {

                int32 iFp = i;
                int32 jFp = j;

                // compute the dot products to determine if the point (i,j) is inside the triangle
                int32 inside1 = (iFp-p0_x) * p01_y - (jFp-p0_y) * p01_x;
                int32 inside2 = (iFp-p1_x) * p12_y - (jFp-p1_y) * p12_x;
                int32 inside3 = (iFp-p2_x) * p20_y - (jFp-p2_y) * p20_x;

                if (inside1 >= 0 && inside2 >= 0 && inside3 >= 0) {
                    
                    // print("DSMAdkasp");
                    
                    // calculate the depth using barycentric coordinates
                    Mat3x4 eqSystBaryCentric = {
                        .vals = {
                            FP(p0_x,0), FP(p1_x,0), FP(p2_x,0), FP(i,0),
                            FP(p0_y,0), FP(p1_y,0), FP(p2_y,0), FP(j,0),
                            FP(1,0),    FP(1,0),    FP(1,0),    FP(1,0)
                        }
                    };
                    reduce(&eqSystBaryCentric); // do gauss jordan reduction to calculate the barycentric coords
                    int32 b0 = get(&eqSystBaryCentric, 0, 3);
                    int32 b1 = get(&eqSystBaryCentric, 1, 3);
                    int32 b2 = get(&eqSystBaryCentric, 2, 3);
                    
                    // linearly interpolate the depth
                    int32 depth = fpMul(b0,v0_t.z) + fpMul(b1,v1_t.z) + fpMul(b2,v2_t.z);
                    if (depth < getDepth(i,j)) {
                        draw(i,j,0xFF);
                        setDepth(i,j,depth);
                    }
                }
            } 
        } 
    }
}