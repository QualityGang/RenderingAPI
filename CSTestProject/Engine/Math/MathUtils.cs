using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class MathUtils
{
	public static Vector2 FromPolar(float radians, float magnitude)
	{
		return magnitude * new Vector2((float)Math.Cos(radians), (float)Math.Sin(radians));
	}

	public static float WrapAngle(float radians)
	{
		radians = (float)Math.IEEERemainder(radians, Math.PI * 2);

		if (radians <= -Math.PI)
			radians += (float)(Math.PI * 2);
		else if (radians > Math.PI)
			radians -= (float)(Math.PI * 2);

		return radians;
	}

	public static float Lerp(float value1, float value2, float amount)
	{
		return value1 + (value2 - value1) * amount;
	}

	public static float CatmullRom(float value1, float value2, float value3, float value4, float amount)
	{
		float amountSq = amount * amount;
		float amountCube = amountSq * amount;

		return ((2.0f * value2 +
			(-value1 + value3) * amount +
			(2.0f * value1 - 5.0f * value2 + 4.0f * value3 - value4) * amountSq +
			(3.0f * value2 - 3.0f * value3 - value1 + value4) * amountCube) * 0.5f);
	}
}
