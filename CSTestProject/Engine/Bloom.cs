using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Bloom
{
	IntPtr nativeBloom;

	public Bloom(GraphicsContext context)
	{
		nativeBloom = Bloom_Create(context.GetNativePtr());
	}

	~Bloom()
	{
		Bloom_Release(nativeBloom);
	}

	public void Apply(SpriteBatch batch)
	{
		Bloom_Apply(nativeBloom, batch.GetNativePtr());
	}

	public void SetRenderTarget(IntPtr renderTarget)
	{
		Bloom_SetRenderTarget(nativeBloom, renderTarget);
	}

	public IntPtr GetRenderTarget()
	{
		return Bloom_GetRenderTarget(nativeBloom);
	}

	public void SetSettings(BloomSettings settings)
	{
		Bloom_SetSettings(nativeBloom, settings);
	}

	public BloomSettings GetSettings()
	{
		return Bloom_GetSettings(nativeBloom);
	}

	[DllImport("RenderingAPI.dll")]
	private static extern IntPtr Bloom_Create(IntPtr context);

	[DllImport("RenderingAPI.dll")]
	private static extern void Bloom_Release(IntPtr bloom);

	[DllImport("RenderingAPI.dll")]
	private static extern void Bloom_Apply(IntPtr bloom, IntPtr batch);

	[DllImport("RenderingAPI.dll")]
	private static extern void Bloom_SetRenderTarget(IntPtr bloom, IntPtr renderTarget);

	[DllImport("RenderingAPI.dll")]
	private static extern IntPtr Bloom_GetRenderTarget(IntPtr bloom);

	[DllImport("RenderingAPI.dll")]
	private static extern void Bloom_SetSettings(IntPtr bloom, BloomSettings settings);

	[DllImport("RenderingAPI.dll")]
	private static extern BloomSettings Bloom_GetSettings(IntPtr bloom);
}
