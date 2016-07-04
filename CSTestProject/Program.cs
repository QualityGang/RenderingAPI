using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

static class GameRoot
{
	static GraphicsDevice device = new GraphicsDevice(RenderingAPI.Direct3D11);

	public static GraphicsContext Context = device.GetContext();
    public static Window Window = new Window(Context, "Title", 800, 600);
    public static Vector2 ScreenSize { get { return Window.GetSize(); } }
	public static ParticleManager<ParticleState> ParticleManager = new ParticleManager<ParticleState>(20480, ParticleState.UpdateParticle);

    static SpriteBatch spriteBatch = new SpriteBatch(Context);
	static Bloom bloom = new Bloom(Context);

	const int maxGrixPoints = 1600;
	static Vector2 gridSpacing = new Vector2((float)Math.Sqrt(ScreenSize.X * ScreenSize.Y / maxGrixPoints));
	public static Grid Grid = new Grid(new Rectangle(0, 0, ScreenSize.X, ScreenSize.Y), gridSpacing);

    public static void Initialize()
    {
		Art.Load();
        EntityManager.Add(PlayerShip.Instance);

		BloomSettings settings = new BloomSettings(0.25f, 4, 2, 1, 1.5f, 1);
		bloom.SetSettings(settings);
		bloom.SetRenderTarget(Window.GetRenderTarget());
    }

    public static void Update()
    {
		Input.Update();
		EnemySpawner.Update();
        EntityManager.Update();
		ParticleManager.Update();
		Grid.Update();
    }

    public static void Draw()
	{ 
        Window.Clear(Color.Black);

		//Context.EnableAdditiveBlending();
		spriteBatch.Begin(Window.GetRenderTarget(), null);
		Grid.Draw(spriteBatch);
        EntityManager.Draw(spriteBatch);
		ParticleManager.Draw(spriteBatch);
        spriteBatch.End();
		//Context.DisableAdditiveBlending();

		bloom.Apply(spriteBatch);
    }

	public static void Dispose()
	{
		Window.Dispose();
		spriteBatch.Dispose();
		Art.Dispose();
	}
}

class Program
{
    static void Main(string[] args)
    {
        WndEvent e = new WndEvent();

        GameRoot.Initialize();

        while (!e.IsQuitEvent())
        {
            if (GameRoot.Window.GetNextEvent(e))
            {
                GameRoot.Window.ProcessEvent(e);
            }
            else
            {
                GameRoot.Update();
                GameRoot.Draw();
                GameRoot.Window.SwapBuffers();
            }
		}

		//EntityManager.Clear();
	}
}