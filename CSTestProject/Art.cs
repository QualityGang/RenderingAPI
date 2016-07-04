using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

static class Art
{
    public static Bitmap Player;
    public static Bitmap Seeker;
    public static Bitmap Wanderer;
    public static Bitmap Bullet;
    public static Bitmap Pointer;
	public static Bitmap BlackHole;
	public static Bitmap LineParticle;
	public static Bitmap Pixel;

	public static void Load()
	{
		Player       = new Bitmap(GameRoot.Context, "Art/Player.png");
		Seeker       = new Bitmap(GameRoot.Context, "Art/Seeker.png");
		Wanderer     = new Bitmap(GameRoot.Context, "Art/Wanderer.png");
		Bullet       = new Bitmap(GameRoot.Context, "Art/Bullet.png");
		Pointer      = new Bitmap(GameRoot.Context, "Art/Pointer.png");
		BlackHole    = new Bitmap(GameRoot.Context, "Art/Black Hole.png");
		LineParticle = new Bitmap(GameRoot.Context, "Art/Laser.png");

		Pixel = new Bitmap(GameRoot.Context, 1, 1);
		Pixel.SetPixel(0, Color.White);
	}

	public static void Dispose()
	{
		Player.Dispose();
		Seeker.Dispose();
		Wanderer.Dispose();
		Bullet.Dispose();
		Pointer.Dispose();
		BlackHole.Dispose();
		LineParticle.Dispose();
	}
}
