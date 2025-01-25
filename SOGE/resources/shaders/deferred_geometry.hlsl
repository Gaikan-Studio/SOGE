cbuffer Pipeline_ConstantBuffer : register(b0)
{
    float4x4 viewProjection;
}

Texture2D colorTexture : register(t0);
SamplerState colorTextureSampler : register(s0);

cbuffer Object_ConstantBuffer : register(b1)
{
    float4x4 model;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float shininess;
    float3 emissive;
    float3 color;
    bool hasColorTexture;
}

struct VS_Input
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float3 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float3 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

VS_Output VSMain(VS_Input input)
{
    VS_Output output = (VS_Output)0;

    output.position = mul(mul(viewProjection, model), float4(input.position, 1.0f));
    output.normal = normalize(mul(model, float4(input.normal, 0.0f)).xyz);
    output.color = input.color * color;
    output.texCoord = input.texCoord;

    return output;
}

typedef VS_Output PS_Input;

struct PS_Output
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
    float4 ambient : SV_Target2;
    float4 diffuse : SV_Target3;
    float4 specularShininess : SV_Target4;
    float4 emissive : SV_Target5;
};

PS_Output PSMain(PS_Input input)
{
    PS_Output output = (PS_Output)0;

    float4 color =
        hasColorTexture ? colorTexture.Sample(colorTextureSampler, input.texCoord) : float4(1.0f, 1.0f, 1.0f, 1.0f);

    output.albedo = float4(input.color, 1.0f) * color;
    output.normal = float4(input.normal, 1.0f);
    output.ambient = float4(ambient, 1.0f);
    output.diffuse = float4(diffuse, 1.0f);
    output.specularShininess = float4(specular, shininess);
    output.emissive = float4(emissive, 1.0f);

    return output;
}
