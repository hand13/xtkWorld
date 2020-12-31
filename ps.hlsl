struct PixelIn{
    float4 Pos:SV_POSITION;
    float2 texCoord:TEXCOORD;
};
float4 main(PixelIn pin):SV_TARGET {
    float2 xy = float2(frac(pin.texCoord.x * 10),frac(pin.texCoord.y * 10));
    float len = distance(xy,float2(0.5f,0.5f));
    float col = clamp(len * 2,0.0f,1.0f);
    return float4(float3(1.0f,0.7f,0.3f) * col,1.0f);
}