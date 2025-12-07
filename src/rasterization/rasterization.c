// Author: Isac Zobec

#include "rasterization.h"
#include "../drawing/draw.h"
#include "../math/math.h"
#include "../io/io.h"
#include "../math/transforms.h"
#include "camera.h"
#include "color.h"
#include "models.h"
#include "../performance_analysis/performance_analysis.h"

Vec3 lightVec = {N_FP(0,0x4000), FP(1,0x0000), N_FP(0,0x4000)};
int32 minLightLevel = FP(0,0x4000);

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

void initRasterization() {
    // normalize light vector
    normalize(&lightVec, &lightVec);

    // clear depth buffer
    clearDepthBuffer();
}

void drawModel(const ObjectInstance* object) {


    if (object->objectInstanceFlags & OBJECTINSTANCE_FLAG_HIDDEN) {return;}

    Model* model = object->model;

    for (int triangleIndex = 0; triangleIndex < model->indexBufferCount; triangleIndex += 3) {

        // get the verticies
        Vec3* v0 = model->vertexBuffer + *(model->indexBuffer + triangleIndex);
        Vec3* v1 = model->vertexBuffer + *(model->indexBuffer + triangleIndex + 1);
        Vec3* v2 = model->vertexBuffer + *(model->indexBuffer + triangleIndex + 2);

        
        // apply the objects transform matrix 
        Vec3 v0_model;
        Vec3 v1_model;
        Vec3 v2_model;
        applyTransform(v0, &v0_model, &object->transform);
        applyTransform(v1, &v1_model, &object->transform);
        applyTransform(v2, &v2_model, &object->transform);

        // calculate the normal of the face and the dot product with the light vec
        Vec3 v01;
        Vec3 v02;
        subVector(&v1_model, &v0_model, &v01);
        subVector(&v2_model, &v0_model, &v02);
        Vec3 cross;
        Vec3 normal;
        crossProduct(&v01, &v02, &cross);
        normalize(&cross, &normal);
        // print_dec(cross.x); print(" "); print_dec(cross.y); print(" "); print_dec(cross.z); print("\n");
        // print_dec(normal.x); print(" "); print_dec(normal.y); print(" "); print_dec(normal.z); print("\n");
        int32 lightDot;
        dotProduct(&lightVec, &normal, &lightDot);
        // lightDot = fpMul(lightDot, FP(30,0));
        // print_dec(lightDot); print("\n");
        lightDot = max(lightDot,minLightLevel);
        // print_dec(lightDot); print("\n");
        
        Vec3 v0_t;
        Vec3 v1_t;
        Vec3 v2_t;
        applyCameraTransform(&v0_model, &v0_t, &camera);
        applyCameraTransform(&v1_model, &v1_t, &camera);
        applyCameraTransform(&v2_model, &v2_t, &camera);

        // multiply x by aspect ratio
        v0_t.x = fpDiv(v0_t.x, ASPECT_RATIO);
        v1_t.x = fpDiv(v1_t.x, ASPECT_RATIO);
        v2_t.x = fpDiv(v2_t.x, ASPECT_RATIO);
        
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

        // transform the verticies from clip space (-1,1) to screen space
        int32 p0_x = FP_TOINT(fpMul(fpAdd(v0_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p0_y = FP_TOINT(fpMul(fpAdd(fpMul(v0_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        int32 p1_x = FP_TOINT(fpMul(fpAdd(v1_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p1_y = FP_TOINT(fpMul(fpAdd(fpMul(v1_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));

        int32 p2_x = FP_TOINT(fpMul(fpAdd(v2_t.x, FP(1,0)),FP(PIXEL_BUFFER_X_SIZE,0) >> 1));
        int32 p2_y = FP_TOINT(fpMul(fpAdd(fpMul(v2_t.y,FP(-1,0)), FP(1,0)),FP(PIXEL_BUFFER_Y_SIZE,0) >> 1));


        clearCsrRegs();


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
        for (int j = yMin; j < yMax; j++) {
            for (int i = xMin; i < xMax; i++) {

                // compute the edge functions to determine if the point (i,j) is inside the triangle, using cross products
                int32 area0 = (i-p0_x) * p01_y - (j-p0_y) * p01_x;
                int32 area1 = (i-p1_x) * p12_y - (j-p1_y) * p12_x;
                int32 area2 = (i-p2_x) * p20_y - (j-p2_y) * p20_x;

                if (area0 <= 0 && area1 <= 0 && area2 <= 0) {
                    
                    if (object->objectInstanceFlags & OBJECTINSTANCE_ONLY_RENDER_EDGES) {
                        if (area0 <= EDGE_LIMIT && area1 <= EDGE_LIMIT && area2 <= EDGE_LIMIT)
                            continue;
                    }

                    int32 depth;
                    if (object->objectInstanceFlags & OBJECTINSTANCE_FLAG_BOTTOM_DEPTH_CHECK) {
                        depth = 0x7FFFFFFD; // second possible positive 32 bit integer in twos complement;
                    } else {
                        int32 b0 = fpDiv(FP(area0,0),FP(totalArea,0));
                        int32 b1 = fpDiv(FP(area1,0),FP(totalArea,0));
                        int32 b2 = fpDiv(FP(area2,0),FP(totalArea,0));
                        
                        // source about perspective correct interpolation: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/perspective-correct-interpolation-vertex-attributes.html
                        // linearly interpolate the depth
                        int32 interpolatedOneOverZ = fpMul(b0,z_rec_0) + fpMul(b1,z_rec_1) + fpMul(b2,z_rec_2);
                        
                        if (interpolatedOneOverZ == 0) continue;
                        depth = fpDiv(N_FP(1,0),interpolatedOneOverZ);
                    }


                    if (depth <= getDepth(i,j)) {
                        // draw(i,j,0xFF);

                        // print_dec(lightDot); print("\n");
                        uint8 color = multiplyColor(object->color,lightDot);
                        // uint8 color = multiplyColor((uint8)triangleIndex+8,lightDot);

                        if (object->objectInstanceFlags & OBJECTINSTANCE_RENDER_TO_GIZMO_LAYER) {
                            drawToGizmoLayer(&gizmoLayer, i, j, color);
                        } else {
                            draw(i,j,color);
                            setDepth(i,j,depth);
                        }
                    }
                }
            } 
        } 
        incrementCsrCounterVars();
    }
}