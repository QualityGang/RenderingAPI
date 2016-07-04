using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class GraphicsContext
{
    private IntPtr nativeContext;

    public GraphicsContext(IntPtr nativeContext)
    {
        this.nativeContext = nativeContext;
    }

    public TextureSize GetTexture2DSize(IntPtr texture)
    {
        TextureSize size;
        GraphicsContext_GetTexture2DSize(nativeContext, texture, out size);
        return size;
    }

	public void EnableAdditiveBlending()
	{
		GraphicsContext_EnableAdditiveBlending(nativeContext);
	}

	public void DisableAdditiveBlending()
	{
		GraphicsContext_DisableAdditiveBlending(nativeContext);
	}

    internal IntPtr GetNativePtr()
    {
        return nativeContext;
    }

    [DllImport("RenderingAPI.dll")]
    private static extern void GraphicsContext_GetTexture2DSize(IntPtr context, IntPtr texture, out TextureSize texSize);

	[DllImport("RenderingAPI.dll")]
	private static extern void GraphicsContext_EnableAdditiveBlending(IntPtr context);

	[DllImport("RenderingAPI.dll")]
	private static extern void GraphicsContext_DisableAdditiveBlending(IntPtr context);
}
