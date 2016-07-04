using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class SpriteBatch : IDisposable
{
    private IntPtr nativeBatch;

    public SpriteBatch(GraphicsContext context)
    {
        nativeBatch = SpriteBatch_Create(context.GetNativePtr());
    }

    ~SpriteBatch()
    {
    }

    public void Begin(IntPtr renderTarget, OrthographicCamera camera)
    {
        SpriteBatch_Begin(nativeBatch, renderTarget, camera != null ? camera.GetNativePtr() : IntPtr.Zero);
    }

    public void End()
    {
        SpriteBatch_End(nativeBatch);
    }

    public void Draw(Sprite sprite)
    {
        SpriteBatch_Draw(nativeBatch, sprite.GetNativePtr());
    }

	internal IntPtr GetNativePtr()
	{
		return nativeBatch;
	}

	public void Dispose()
	{
		SpriteBatch_Release(nativeBatch);
		nativeBatch = IntPtr.Zero;
		GC.SuppressFinalize(this);
	}

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr SpriteBatch_Create(IntPtr context);

    [DllImport("RenderingAPI.dll")]
    private static extern void SpriteBatch_Release(IntPtr batch);

    [DllImport("RenderingAPI.dll")]
    private static extern void SpriteBatch_Begin(IntPtr batch, IntPtr renderTarget, IntPtr camera);

    [DllImport("RenderingAPI.dll")]
    private static extern void SpriteBatch_End(IntPtr batch);

    [DllImport("RenderingAPI.dll")]
    private static extern void SpriteBatch_Draw(IntPtr batch, IntPtr sprite);
}