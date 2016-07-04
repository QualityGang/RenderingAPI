using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Bullet : Entity
{
	static Random rand = new Random();

	public Bullet(Vector2 position, Vector2 velocity)
	{
		image = Art.Bullet.GetTexture2D();
		Position = position;
		Velocity = velocity;
		Orientation = (float)(Velocity.ToAngle() * (180 / Math.PI));
		Radius = 8;
	}

	public override void Update()
	{
		if (Velocity.LengthSquared() > 0)
			Orientation = (float)(Velocity.ToAngle() * (180 / Math.PI));

		Position += Velocity;

		GameRoot.Grid.ApplyExplosiveForce(0.5f * Velocity.Length(), new Vector3(Position, 0), 8);

		Rectangle screenBounds = new Rectangle(0, 0, GameRoot.ScreenSize.X, GameRoot.ScreenSize.Y);

		if (!screenBounds.Overlaps(Position))
		{
			IsExpired = true;

			for (int i = 0; i < 30; i++)
			{
				ParticleState state = new global::ParticleState();
				state.Velocity = rand.NextVector2(0, 9);
				state.Type = ParticleType.Bullet;
				state.LengthMultiplier = 1;

				GameRoot.ParticleManager.CreateParticle(Art.LineParticle.GetTexture2D(), Position, Color.LightBlue, 50, Vector2.One, state);
			}
		}
	}
}
