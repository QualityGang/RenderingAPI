using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class SpriteBatch
{
    private IntPtr nativeBatch;

    public SpriteBatch(IntPtr context)
    {
        nativeBatch = SpriteBatch_Create(context);
    }

    ~SpriteBatch()
    {
        SpriteBatch_Release(nativeBatch);
    }

    public void Begin(Window window, OrthographicCamera camera)
    {
        SpriteBatch_Begin(nativeBatch, window.GetRenderTarget(), camera.GetNativePtr());
    }

    public void End()
    {
        SpriteBatch_End(nativeBatch);
    }

    public void Draw(Sprite sprite)
    {
        SpriteBatch_Draw(nativeBatch, sprite.GetNativePtr());
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