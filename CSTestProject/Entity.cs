using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

abstract class Entity
{
    protected IntPtr image;
    protected Color color = Color.White;

    public Vector2 Position, Velocity;
    public float Orientation;
    public float Radius = 20;
    public bool IsExpired;

    public Vector2 Size
    {
        get
        {
            if (image == null)
                return Vector2.Zero;

            TextureSize size = GameRoot.Context.GetTexture2DSize(image);
            return new Vector2(size.Width, size.Height);
        }
    }

    public abstract void Update();

    public virtual void Draw(SpriteBatch spriteBatch)
    {
		Sprite sprite = new Sprite();
		sprite.SetPositionAnchor(0.5f, 0.5f);
		sprite.SetPosition(Position.X, Position.Y);
		sprite.SetSize(Size.X, Size.Y);
		sprite.SetSrcRect(0, 0, Size.X, Size.Y);
		sprite.SetAngle(Orientation);
		sprite.SetColor(color);
		sprite.SetTexture(image);

		spriteBatch.Draw(sprite);
	}
}
