Texture2D    Texture : register(t0);
SamplerState Sampler : register(s0);

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};

float4 PSMain(VSOutput input) : SV_TARGET
{
	// Translate u and v into [-1 , 1] domain
	float u0 = (input.TexCoord.x * 2) - 1;
	float v0 = (input.TexCoord.y * 2) - 1;

	// Then, as u0 approaches 0 (the center), v should also approach 0 
	v0 = v0 * abs(u0);

	// Convert back from [-1,1] domain to [0,1] domain
	v0 = (v0 + 1) / 2;

	// We now have the coordinates for reading from the initial image
	float2 newCoords = float2(input.TexCoord.x ,v0);

	// Read for both horizontal and vertical direction and store them in separate channels
	float horizontal = Texture.Sample(Sampler, newCoords).r;
	float vertical = Texture.Sample(Sampler, newCoords.yx).r;

	return float4(horizontal, vertical, 0, 1);
}