Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer cbReductionData : register(b0)
{
	float2 TextureDimensions;
}

struct VSInput
{
	float4   Position : POSITION;
	float2   TexCoord : TEXCOORD;
	float4   Color    : COLOR;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output;
	output.Position = float4(input.Position.x, input.Position.y, input.Position.z, 1) 
		+ float4(-TextureDimensions.x, TextureDimensions.y, 0, 0);
	output.TexCoord = input.TexCoord;

	return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	float2 color = Texture.Sample(Sampler, input.TexCoord);
	float2 colorR = Texture.Sample(Sampler, input.TexCoord + float2(TextureDimensions.x,0));
	float2 result = min(color, colorR);
	return float4(result,0,1);
}