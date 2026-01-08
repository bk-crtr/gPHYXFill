#include <metal_stdlib>
using namespace metal;

struct OSCPoint {
    float2 position; // Normalized 0..1
    float4 color;
    float radius;
};

kernel void osc_kernel(texture2d<float, access::read_write> targetTexture [[texture(0)]],
                       constant OSCPoint *points [[buffer(0)]],
                       constant int &pointCount [[buffer(1)]],
                       uint2 gid [[thread_position_in_grid]])
{
    if (gid.x >= targetTexture.get_width() || gid.y >= targetTexture.get_height()) {
        return;
    }

    float2 pixelPos = float2(gid);
    float4 finalColor = targetTexture.read(gid);

    // Draw Lines
    float lineRadius = 1.0; // Thinner than points
    float4 lineColor = float4(1.0, 0.0, 1.0, 1.0); // Magenta (Reference)
    
    // Simple line connecting points sequentially (0-1, 1-2, etc.)
    // Note: ideally we should pass lines buffer, but for now we can infer from points
    // if the user wants closed loop or sequential. 
    // Let's check provided instructions: "Draw custom mask". Usually that's a closed loop.
    
    if (pointCount > 1) {
        for (int i = 0; i < pointCount; i++) {
             // Connect i to (i+1)%pointCount for closed loop
             int next = (i + 1) % pointCount;
             
             float2 pA = points[i].position * float2(targetTexture.get_width(), targetTexture.get_height());
             float2 pB = points[next].position * float2(targetTexture.get_width(), targetTexture.get_height());
             
             float2 pa = pixelPos - pA;
             float2 ba = pB - pA;
             float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
             float dist = length(pa - ba * h);
             
             if (dist < lineRadius + 1.0) {
                 float alpha = 1.0 - smoothstep(lineRadius - 0.5, lineRadius + 0.5, dist);
                 finalColor = mix(finalColor, lineColor, alpha);
             }
        }
    }

    // Draw Points (on top of lines)
    for (int i = 0; i < pointCount; i++) {
        float2 pointPx = points[i].position * float2(targetTexture.get_width(), targetTexture.get_height());
        float dist = distance(pixelPos, pointPx);
        
        if (dist < points[i].radius) {
            float smoothedAlpha = 1.0 - smoothstep(points[i].radius - 1.0, points[i].radius + 1.0, dist);
            finalColor = mix(finalColor, points[i].color, smoothedAlpha);
        }
    }

    targetTexture.write(finalColor, gid);
}
