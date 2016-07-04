using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Sprite
{
    private IntPtr nativeSprite;

    public Sprite()
    {
        nativeSprite = Sprite_Create();
    }

    ~Sprite()
    {
        Sprite_Release(nativeSprite);
	}

    public void SetPosition(float x, float y)
    {
        Sprite_SetPosition(nativeSprite, x, y);
    }

    public void SetDepth(float depth)
    {
        Sprite_SetDepth(nativeSprite, depth);
    }

    public void SetAngle(float degrees)
    {
        Sprite_SetAngle(nativeSprite, degrees);
    }

    public void SetSize(float width, float height)
    {
        Sprite_SetSize(nativeSprite, width, height);
    }

    public void SetColor(Color color)
    {
        Sprite_SetColor(nativeSprite, color);
    }

    public void SetSrcRect(float srcX, float srcY, float srcWidth, float srcHeight)
    {
        Sprite_SetSrcRect(nativeSprite, srcX, srcY, srcWidth, srcHeight);
    }

    public void SetTexture(IntPtr texture)
    {
        Sprite_SetTexture(nativeSprite, texture);
    }

    public void SetPositionAnchor(float anchorX, float anchorY)
    {
        Sprite_SetPositionAnchor(nativeSprite, anchorX, anchorY);
    }

    public void SetRotationAnchor(float anchorX, float anchorY)
    {
        Sprite_SetRotationAnchor(nativeSprite, anchorX, anchorY);
    }

    public float GetX()
    {
        return Sprite_GetX(nativeSprite);
    }
     
    public float GetY()
    {
        return Sprite_GetY(nativeSprite);
    }

    public float GetDepth()
    {
        return Sprite_GetDepth(nativeSprite);
    }

    public float GetWidth()
    {
        return Sprite_GetWidth(nativeSprite);
    }
     
    public float GetHeight()
    {
        return Sprite_GetHeight(nativeSprite);
    }
     
    public float GetAngle()
    {
        return Sprite_GetAngle(nativeSprite);
    }
     
    public Color GetColor()
    {
        return Sprite_GetColor(nativeSprite);
    }
     
    public IntPtr GetTexture()
    {
        return Sprite_GetTexture(nativeSprite);
    }

    public float GetPositionAnchorX()
    {
        return Sprite_GetPositionAnchorX(nativeSprite);
    }

    public float GetPositionAnchorY()
    {
        return Sprite_GetPositionAnchorY(nativeSprite);
    }

    public float GetRotationAnchorX()
    {
        return Sprite_GetRotationAnchorX(nativeSprite);
    }

    public float GetRotationAnchorY()
    {
        return Sprite_GetRotationAnchorY(nativeSprite);
    }

    internal IntPtr GetNativePtr()
    {
        return nativeSprite;
    }

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr Sprite_Create();

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_Release(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetPosition(IntPtr sprite, float x, float y);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetDepth(IntPtr sprite, float depth);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetAngle(IntPtr sprite, float degrees);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetSize(IntPtr sprite, float width, float height);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetColor(IntPtr sprite, Color color);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetSrcRect(IntPtr sprite, float srcX, float srcY, float srcWidth, float srcHeight);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetTexture(IntPtr sprite, IntPtr texture);

    //[DllImport("RenderingAPI.dll")]
    //private static extern void Sprite_SetEffect(IntPtr sprite, SpriteEffect effect);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetPositionAnchor(IntPtr sprite, float anchorX, float anchorY);

    [DllImport("RenderingAPI.dll")]
    private static extern void Sprite_SetRotationAnchor(IntPtr sprite, float anchorX, float anchorY);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetX(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetY(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetDepth(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetWidth(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetHeight(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetAngle(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern Color Sprite_GetColor(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr Sprite_GetTexture(IntPtr sprite);

    // GetSrcRect

    //[DllImport("RenderingAPI.dll")]
    //private static extern SpriteEffect Sprite_GetEffect(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetPositionAnchorX(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetPositionAnchorY(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetRotationAnchorX(IntPtr sprite);

    [DllImport("RenderingAPI.dll")]
    private static extern float Sprite_GetRotationAnchorY(IntPtr sprite);
}
