using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

enum RenderingAPI
{
    Direct3D11
}

class GraphicsDevice
{
    private IntPtr nativeDevice;
    private GraphicsContext context;

    public GraphicsDevice(RenderingAPI api)
    {
        nativeDevice = GraphicsDevice_Create((uint)api);
        context = new GraphicsContext(GraphicsDevice_GetContext(nativeDevice));
    }

    ~GraphicsDevice()
    {
        GraphicsDevice_Release(nativeDevice);
	}

    public GraphicsContext GetContext()
    {
        return context;
    }

    public RenderingAPI GetAPI()
    {
        return (RenderingAPI)GraphicsDevice_GetAPI(nativeDevice);
    }

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr GraphicsDevice_Create(uint api);

    [DllImport("RenderingAPI.dll")]
    private static extern void GraphicsDevice_Release(IntPtr device);

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr GraphicsDevice_GetContext(IntPtr device);

    [DllImport("RenderingAPI.dll")]
    private static extern uint GraphicsDevice_GetAPI(IntPtr device);
}
