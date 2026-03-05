cbuffer ConstantBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 pos : POSITION;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    float4 p = float4(input.pos,1);

    p = mul(p, world);
    p = mul(p, view);
    p = mul(p, projection);

    output.pos = p;

    return output;
}