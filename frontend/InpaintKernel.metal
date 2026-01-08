#include <metal_stdlib>
using namespace metal;

// Basic inpainting kernel (Placeholder for PatchMatch or Clean Plate logic)
// Simply fills pixels within the mask using a solid color or simple blur for now
kernel void inpaint_kernel(texture2d<float, access::read>  sourceTexture  [[texture(0)]],
                           texture2d<float, access::write> destTexture    [[texture(1)]],
                           texture2d<float, access::read>  maskTexture    [[texture(2)]],
                           texture2d<float, access::read>  refTexture     [[texture(3)]],
                           constant float3x3 &homography                  [[buffer(0)]],
                           uint2 gid [[thread_position_in_grid]])
{
    if (gid.x >= destTexture.get_width() || gid.y >= destTexture.get_height()) {
        return;
    }

    float4 maskColor = maskTexture.read(gid);
    bool isInMask = maskColor.r > 0.5;

    if (isInMask) {
        // Map current pixel (gid) to reference frame using homography
        float3 pos = float3(float(gid.x), float(gid.y), 1.0);
        float3 mappedPos = homography * pos;
        
        uint2 refGid = uint2(mappedPos.x / mappedPos.z, mappedPos.y / mappedPos.z);
        
        if (refGid.x < refTexture.get_width() && refGid.y < refTexture.get_height()) {
            destTexture.write(refTexture.read(refGid), gid);
        } else {
            // Fallback: stay with source if out of bounds
            destTexture.write(sourceTexture.read(gid), gid);
        }
    } else {
        // Keep original
        destTexture.write(sourceTexture.read(gid), gid);
    }
}
