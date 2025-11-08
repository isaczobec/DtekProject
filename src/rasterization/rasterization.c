#include "rasterization.h"
#include "../drawing/draw.h"
#include "../math/math.h"
#include "../io/io.h"

Model models[MODEL_COUNT] = {
    {
        .vertexBuffer = (Vec3*)&vertexData,
        .vertexCount = 3,
        .indexBuffer = (uint16*)&indexData,
        .indexBufferCount = 3
    }
};
Vec3 vertexData[VERTEX_DATA_TOTAL_SIZE_BYTES] = {
    {FP(0,0x8000),FP(0,0x8000),FP(1,0)},
    {FP(0,0),FP(0,0),FP(1,0)},
    {FP(1,0),FP(0,0),FP(1,0)},
};
uint16 indexData[INDEX_DATA_TOTAL_SIZE_BYTES] = {
    0,1,2
};

void drawModel(const Model* model) {

    for (int triangleIndex = 0; triangleIndex < model->indexBufferCount; triangleIndex += 3) {

        // get the verticies
        Vec3 v0 = *(model->vertexBuffer + *(model->indexBuffer + triangleIndex));
        Vec3 v1 = *(model->vertexBuffer + *(model->indexBuffer + triangleIndex + 1));
        Vec3 v2 = *(model->vertexBuffer + *(model->indexBuffer + triangleIndex + 2));
        
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
        int32 p0_x = fpMul(fpAdd(v0.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1);
        int32 p0_y = fpMul(fpAdd(fpMul(v0.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1);

        int32 p1_x = fpMul(fpAdd(v1.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1);
        int32 p1_y = fpMul(fpAdd(fpMul(v1.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1);

        int32 p2_x = fpMul(fpAdd(v2.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1);
        int32 p2_y = fpMul(fpAdd(fpMul(v2.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1);

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
        int32 xMin = FP_TOINT(min(min(p0_x,p1_x),p2_x));
        int32 xMax = FP_TOINT(max(max(p0_x,p1_x),p2_x));
        int32 yMin = FP_TOINT(min(min(p0_y,p1_y),p2_y));
        int32 yMax = FP_TOINT(max(max(p0_y,p1_y),p2_y));
        
        print("\n----\n");
        print_dec(xMin);
        print(",");
        print_dec(xMax);
        print("\n");
        print_dec(yMin);
        print(",");
        print_dec(yMax);
        
        for (int i = xMin; i < xMax; i++) {
            for (int j = yMin; j < yMax; j++) {

                int32 iFp = FP(i,0);
                int32 jFp = FP(j,0);

                // compute the dot products to determine if the point (i,j) is inside the triangle
                int32 inside1 = fpMul(iFp-p0_x, p01_y) + fpMul(jFp-p0_y, -p01_x);
                int32 inside2 = fpMul(iFp-p1_x, p12_y) + fpMul(jFp-p1_y, -p12_x);
                int32 inside3 = fpMul(iFp-p2_x, p20_y) + fpMul(jFp-p2_y, -p20_x);

                if (inside1 >= 0 && inside2 >= 0 && inside3 >= 0) {
                    draw(i,j,0xFF);
                }
            } 
        } 
    }
}