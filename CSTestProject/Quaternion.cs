using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

struct Quaternion
{
	public float X, Y, Z, W;

	public Quaternion(float x, float y, float z, float w)
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
	}

	public static Quaternion Inverse(Quaternion quaternion)
	{
		Quaternion quaternion2;
		float num2 = (((quaternion.X * quaternion.X) + (quaternion.Y * quaternion.Y)) + (quaternion.Z * quaternion.Z)) + (quaternion.W * quaternion.W);
		float num = 1f / num2;
		quaternion2.X = -quaternion.X * num;
		quaternion2.Y = -quaternion.Y * num;
		quaternion2.Z = -quaternion.Z * num;
		quaternion2.W = quaternion.W * num;
		return quaternion2;
	}

	public static Quaternion Multiply(Quaternion quaternion1, Quaternion quaternion2)
	{
		Quaternion quaternion;
		float x = quaternion1.X;
		float y = quaternion1.Y;
		float z = quaternion1.Z;
		float w = quaternion1.W;
		float num4 = quaternion2.X;
		float num3 = quaternion2.Y;
		float num2 = quaternion2.Z;
		float num = quaternion2.W;
		float num12 = (y * num2) - (z * num3);
		float num11 = (z * num4) - (x * num2);
		float num10 = (x * num3) - (y * num4);
		float num9 = ((x * num4) + (y * num3)) + (z * num2);
		quaternion.X = ((x * num) + (num4 * w)) + num12;
		quaternion.Y = ((y * num) + (num3 * w)) + num11;
		quaternion.Z = ((z * num) + (num2 * w)) + num10;
		quaternion.W = (w * num) - num9;
		return quaternion;
	}

	public static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll)
	{
		Quaternion quaternion;
		float num9 = roll * 0.5f;
		float num6 = (float)Math.Sin(num9);
		float num5 = (float)Math.Cos(num9);
		float num8 = pitch * 0.5f;
		float num4 = (float)Math.Sin(num8);
		float num3 = (float)Math.Cos(num8);
		float num7 = yaw * 0.5f;
		float num2 = (float)Math.Sin(num7);
		float num = (float)Math.Cos(num7);
		quaternion.X = ((num * num4) * num5) + ((num2 * num3) * num6);
		quaternion.Y = ((num2 * num3) * num5) - ((num * num4) * num6);
		quaternion.Z = ((num * num3) * num6) - ((num2 * num4) * num5);
		quaternion.W = ((num * num3) * num5) + ((num2 * num4) * num6);
		return quaternion;
	}
}
