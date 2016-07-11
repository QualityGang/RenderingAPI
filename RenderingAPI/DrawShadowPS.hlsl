Texture2D    Texture			: register(t0);
Texture2D    ShadowMapTexture	: register(t1);
SamplerState Sampler			: register(s0);

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color    : COLOR;
};


float GetShadowDistanceH(float2 TexCoord)
{
	float u = TexCoord.x;
	float v = TexCoord.y;

	u = abs(u - 0.5f) * 2;
	v = v * 2 - 1;
	float v0 = v / u;
	v0 = (v0 + 1) / 2;

	float2 newCoords = float2(TexCoord.x, v0);

	// Horizontal info was stored in the Red component
	return ShadowMapTexture.Sample(Sampler, newCoords).r;
}

float GetShadowDistanceV(float2 TexCoord)
{
	float u = TexCoord.y;
	float v = TexCoord.x;

	u = abs(u - 0.5f) * 2;
	v = v * 2 - 1;
	float v0 = v / u;
	v0 = (v0 + 1) / 2;

	float2 newCoords = float2(TexCoord.y, v0);

	// Vertical info was stored in the Green component
	return ShadowMapTexture.Sample(Sampler, newCoords).g;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
	// Distance of this pixel from the center
	float distance = length(input.TexCoord - 0.5f);

	// Distance stored in the shadow map
	float shadowMapDistance;

	// Coords in [-1,1]
	float nX = (input.TexCoord.x - 0.5f) * 2;
	float nY = (input.TexCoord.y - 0.5f) * 2;

	// Basic algebra to see which quadrant we are in. (y = x and y = -x lines form the quadrants)
	if (abs(nY) < abs(nX))
	{
		shadowMapDistance = GetShadowDistanceH(input.TexCoord); // We are in horizontral quadrants.
	}
	else
	{
		shadowMapDistance = GetShadowDistanceV(input.TexCoord); // We are in vertical quadrants.
	}

	// If distance to this pixel is lower than distance got from the Shadow Map, then we are not in the shadow

	float light = distance < shadowMapDistance ? 1 : 0;

	float4 result = light;
	result.b = length(input.TexCoord - 0.5f);
	result.a = 1;

	return result;
}