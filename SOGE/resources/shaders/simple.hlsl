cbuffer VS_ConstantBuffer : register(b0)
{
    float4x4 mvp;
}

struct VS_Input
{
    float3 position : POSITION0;
    float4 color : COLOR0;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.position = mul(mvp, float4(input.position, 1.0f));
    output.color = input.color;

    return output;
}

typedef VS_Output PS_Input;

float4 PSMain(PS_Input input) : SV_Target
{
    float4 color = input.color;
    return color;
}
