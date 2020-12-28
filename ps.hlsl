struct PixelIn{
    float4 Pos:SV_POSITION;
    float2 texCoord:TEXCOORD;
};
float4 main(PixelIn pin):SV_TARGET {
    float2 xy = float2(frac(pin.texCoord.x * 4),frac(pin.texCoord.y * 4));
    float len = distance(xy,float2(0.5f,0.5f));
    float col = clamp(len,0.0f,1.0f);
    return float4(col,col,col,1.0f);
}