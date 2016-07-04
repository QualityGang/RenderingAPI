using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public enum ParticleType { None, Enemy, Bullet, IgnoreGravity }

struct ParticleState
{
	public Vector2 Velocity;
	public ParticleType Type;
	public float LengthMultiplier;

	public static void UpdateParticle(ParticleManager<ParticleState>.Particle particle)
	{
		var vel = particle.State.Velocity;

		particle.Position += vel;
		particle.Orientation = (float)(vel.ToAngle() * (180 / Math.PI));

		float speed = vel.Length();
		float alpha = Math.Min(1, Math.Min(particle.PercentLife * 2, speed));
		alpha *= alpha;

		particle.Tint.A = (byte)(255 * alpha);

		if (particle.State.Type == ParticleType.Bullet)
			particle.Scale.X = particle.State.LengthMultiplier * Math.Min(Math.Min(1f, 0.1f * speed + 0.1f), alpha);
		else
			particle.Scale.X = particle.State.LengthMultiplier * Math.Min(Math.Min(1f, 0.2f * speed + 0.1f), alpha);

		var pos = particle.Position;
		int width = (int)GameRoot.ScreenSize.X;
		int height = (int)GameRoot.ScreenSize.Y;

		if (pos.X < 0)
			vel.X = Math.Abs(vel.X);
		else if (pos.X > width)
			vel.X = -Math.Abs(vel.X);
		if (pos.Y < 0)
			vel.Y = Math.Abs(vel.Y);
		else if (pos.Y > height)
			vel.Y = -Math.Abs(vel.Y);

		if (particle.State.Type != ParticleType.IgnoreGravity)
		{
			foreach (var blackHole in EntityManager.BlackHoles)
			{
				var dPos = blackHole.Position - pos;
				float distance = dPos.Length();
				var n = dPos / distance;
				vel += 10000 * n / (distance * distance + 10000);
				
				if (distance < 400)
					vel += 45 * new Vector2(n.Y, -n.X) / (distance + 100);
			}
		}

		if (Math.Abs(vel.X) + Math.Abs(vel.Y) < 0.0000000001f)
			vel = Vector2.Zero;

		vel *= 0.97f;
		particle.State.Velocity = vel;
	}
}
