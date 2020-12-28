struct VertexIn {
    float3 Pos :POSITION;
    float2 texCoord:TEXCOORD;
};
struct VertexOut {
    float4 Pos : SV_POSITION;
    float2 texCoord:TEXCOORD;
};
VertexOut main(VertexIn vi)  {
    VertexOut vout;
    vout.Pos = float4(vi.Pos,1.0f);
    vout.texCoord = vi.texCoord;
    return vout;
}