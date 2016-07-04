using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class BlackHole : Entity
{
	private static Random rand = new Random();
	private int hitpoints = 10;

	public BlackHole(Vector2 position)
	{
		image = Art.BlackHole.GetTexture2D();
		Position = position;

		TextureSize imageSize = GameRoot.Context.GetTexture2DSize(image);
		Radius = (imageSize.Width + imageSize.Height) / 4.0f;
	}

	public void Hit()
	{
		hitpoints--;

		if (hitpoints <= 0)
		{
			IsExpired = true;
		}

		float hue = (float)((3 * rand.NextFloat(0, 60)) % 6);
		Color color = ColorUtils.HSVToColor(hue, 0.25f, 1);
		const int numParticles = 40;
		float startOffset = rand.NextFloat(0, (float)(2 * Math.PI) / numParticles);

		for (int i = 0; i < numParticles; i++)
		{
			Vector2 sprayVel = MathUtils.FromPolar((float)(2 * Math.PI) * i / numParticles + startOffset, rand.NextFloat(8, 16));
			Vector2 pos = Position + 2f * sprayVel;
			var state = new ParticleState()
			{
				Velocity = sprayVel,
				LengthMultiplier = 1,
				Type = ParticleType.IgnoreGravity
			};

			GameRoot.ParticleManager.CreateParticle(Art.LineParticle.GetTexture2D(), pos, color, 90, new Vector2(1.5f), state);
		}
	}

	public void Kill()
	{
		hitpoints = 1;
		Hit();
	}

	public override void Update()
	{
		var entities = EntityManager.GetNearbyEntities(Position, 250);

		foreach (var entity in entities)
		{
			if (entity is Enemy && !(entity as Enemy).IsActive)
				continue;

			if (entity is Bullet)
				entity.Velocity += (entity.Position - Position).ScaleTo(0.3f);
			else
			{
				var dPos = Position - entity.Position;
				var length = dPos.Length();

				entity.Velocity += dPos.ScaleTo(MathUtils.Lerp(2, 0, length / 250f));
			}
		}

		GameRoot.Grid.ApplyImplosiveForce((float)Math.Sin(3.14 / 2) * 10 + 20, new Vector3(Position, 0), 200);
	}

	public override void Draw(SpriteBatch spriteBatch)
	{
		base.Draw(spriteBatch);
	}
}
