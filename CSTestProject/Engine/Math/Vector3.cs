using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

struct Vector3 : IEquatable<Vector3>
{
	public static readonly Vector3 Zero = new Vector3(0);
	public static readonly Vector3 One  = new Vector3(1);

	public float X, Y, Z;

	public Vector3(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	public Vector3(Vector2 vector, float z)
	{
		X = vector.X;
		Y = vector.Y;
		Z = z;
	}

	public Vector3(float value)
	{
		X = value;
		Y = value;
		Z = value;
	}

	public float Length()
	{
		return (float)Math.Sqrt(X * X + Y * Y + Z * Z);
	}

	public float LengthSquared()
	{
		return X * X + Y * Y + Z * Z;
	}

	public void Normalize()
	{
		float val = 1.0f / Length();
		X *= val;
		Y *= val;
		Z *= val;
	}

	public float Distance(Vector3 vector)
	{
		float dx = X - vector.X;
		float dy = Y - vector.Y;
		float dz = Z - vector.Z;
		return (float)Math.Sqrt(dx * dx + dy * dy + dz * dz);
	}

	public float DistanceSquared(Vector3 vector)
	{
		float dx = X - vector.X;
		float dy = Y - vector.Y;
		float dz = Z - vector.Z;
		return dx * dx + dy * dy + dz * dz;
	}

	public Vector3 Transform(Vector3 value, ref Quaternion rotation)
	{
		Vector3 result = new Vector3();

		float x = 2 * (rotation.Y * value.Z - rotation.Z * value.Y);
		float y = 2 * (rotation.Z * value.X - rotation.X * value.Z);
		float z = 2 * (rotation.X * value.Y - rotation.Y * value.X);

		result.X = value.X + x * rotation.W + (rotation.Y * z - rotation.Z * y);
		result.Y = value.Y + y * rotation.W + (rotation.Z * x - rotation.X * z);
		result.Z = value.Z + z * rotation.W + (rotation.X * y - rotation.Y * x);

		return result;
	}

	public void Clamp(Vector3 min, Vector3 max)
	{
		X = Math.Min(Math.Max(min.X, X), max.X);
		Y = Math.Min(Math.Max(min.Y, Y), max.Y);
		Z = Math.Min(Math.Max(min.Z, Z), max.Z);
	}

	public static Vector3 CatmullRom(Vector3 value1, Vector3 value2, Vector3 value3, Vector3 value4, float amount)
	{
		return new Vector3(
			MathUtils.CatmullRom(value1.X, value2.X, value3.X, value4.X, amount),
			MathUtils.CatmullRom(value1.Y, value2.Y, value3.Y, value4.Y, amount),
			MathUtils.CatmullRom(value1.Z, value2.Z, value3.Z, value4.Z, amount));
	}

	public static Vector3 operator-(Vector3 value)
	{
		value.X = -value.X;
		value.Y = -value.Y;
		value.Z = -value.Z;
		return value;
	}

	public static bool operator==(Vector3 value1, Vector3 value2)
	{
		return value1.X == value2.X && value1.Y == value2.Y && value1.Z == value2.Z;
	}

	public static bool operator!=(Vector3 value1, Vector3 value2)
	{
		return value1.X != value2.X || value1.Y != value2.Y || value1.Z != value2.Z;
	}

	public static Vector3 operator+(Vector3 value1, Vector3 value2)
	{
		value1.X += value2.X;
		value1.Y += value2.Y;
		value1.Z += value2.Z;
		return value1;
	}

	public static Vector3 operator-(Vector3 value1, Vector3 value2)
	{
		value1.X -= value2.X;
		value1.Y -= value2.Y;
		value1.Z -= value2.Z;
		return value1;
	}

	public static Vector3 operator*(Vector3 value1, Vector3 value2)
	{
		value1.X *= value2.X;
		value1.Y *= value2.Y;
		value1.Z *= value2.Z;
		return value1;
	}

	public static Vector3 operator*(Vector3 value, float scaleFactor)
	{
		value.X *= scaleFactor;
		value.Y *= scaleFactor;
		value.Z *= scaleFactor;
		return value;
	}

	public static Vector3 operator*(float scaleFactor, Vector3 value)
	{
		value.X *= scaleFactor;
		value.Y *= scaleFactor;
		value.Z *= scaleFactor;
		return value;
	}

	public static Vector3 operator/(Vector3 value1, Vector3 value2)
	{
		value1.X /= value2.X;
		value1.Y /= value2.Y;
		value2.Z /= value2.Z;
		return value1;
	}

	public static Vector3 operator/(Vector3 value1, float divider)
	{
		float factor = 1 / divider;
		value1.X *= factor;
		value1.Y *= factor;
		value1.Z *= factor;
		return value1;
	}

	public override bool Equals(object obj)
	{
		if (obj is Vector3)
			return Equals((Vector3)obj);

		return false;
	}

	public bool Equals(Vector3 other)
	{
		return X == other.X && Y == other.Y && Z == other.Z;
	}

	public override int GetHashCode()
	{
		return X.GetHashCode() + Y.GetHashCode() + Z.GetHashCode();
	}

	public override string ToString()
	{
		return X.ToString() + " " + Y.ToString() + " " + Z.ToString();
	}
}
