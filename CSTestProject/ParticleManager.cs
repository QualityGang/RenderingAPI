using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class ParticleManager<T>
{
	public class Particle
	{
		public IntPtr Texture;
		public Vector2 Position;
		public float Orientation;

		public Vector2 Scale = Vector2.One;

		public Color Tint;
		public float Duration;
		public float PercentLife = 1f;
		public T State;
	}

	private class CircularParticleArray
	{
		private int start;
		public int Start
		{
			get { return start; }
			set { start = value % list.Length; }
		}

		public int Count;
		public int Capacity { get { return list.Length; } }
		private Particle[] list;

		public CircularParticleArray(int capacity)
		{
			list = new Particle[capacity];
		}

		public Particle this[int i]
		{
			get { return list[(start + i) % list.Length]; }
			set { list[(start + i) % list.Length] = value; }
		}
	}

	private Action<Particle> updateParticle;
	private CircularParticleArray particleList;

	public ParticleManager(int capacity, Action<Particle> updateParticle)
	{
		this.updateParticle = updateParticle;
		particleList = new CircularParticleArray(capacity);

		for (int i = 0; i < capacity; i++)
			particleList[i] = new Particle();
	}

	public void CreateParticle(IntPtr texture, Vector2 position, Color tint, float duration, Vector2 scale, T state, float theta = 0)
	{
		Particle particle;

		if (particleList.Count == particleList.Capacity)
		{
			particle = particleList[0];
			particleList.Start++;
		}
		else
		{
			particle = particleList[particleList.Count];
			particleList.Count++;
		}

		particle.Texture = texture;
		particle.Position = position;
		particle.Tint = tint;

		particle.Duration = duration;
		particle.PercentLife = 1f;
		particle.Scale = scale;
		particle.Orientation = (float)(theta * (180 / Math.PI));
		particle.State = state;
	}

	public void Update()
	{
		int removalCount = 0;

		for (int i = 0; i < particleList.Count; i++)
		{
			var particle = particleList[i];
			updateParticle(particle);
			particle.PercentLife -= 1f / particle.Duration;

			Swap(particleList, i - removalCount, i);

			if (particle.PercentLife <= 0)
				removalCount++;
		}

		particleList.Count -= removalCount;
	}

	private static void Swap(CircularParticleArray list, int index1, int index2)
	{
		var temp = list[index1];
		list[index1] = list[index2];
		list[index2] = temp;
	}

	public void Draw(SpriteBatch spriteBatch)
	{
		for (int i = 0; i < particleList.Count; i++)
		{
			var particle = particleList[i];

			TextureSize texSize = GameRoot.Context.GetTexture2DSize(particle.Texture);

			Sprite sprite = new Sprite();
			sprite.SetPositionAnchor(0.5f, 0.5f);
			sprite.SetPosition(particle.Position.X, particle.Position.Y);
			sprite.SetSize(texSize.Width * particle.Scale.X, texSize.Height * particle.Scale.Y);
			sprite.SetSrcRect(0, 0, texSize.Width, texSize.Height);
			sprite.SetAngle(particle.Orientation);
			sprite.SetColor(particle.Tint);
			sprite.SetTexture(particle.Texture);

			spriteBatch.Draw(sprite);
		}
	}
}
