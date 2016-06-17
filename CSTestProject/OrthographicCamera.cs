using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class OrthographicCamera
{
    private IntPtr nativeCamera;

    public OrthographicCamera(float x, float y, float viewportWidth, float viewportHeight)
    {
        nativeCamera = OrthographicCamera_Create(x, y, viewportWidth, viewportHeight);
    }

    ~OrthographicCamera()
    {
        OrthographicCamera_Release(nativeCamera);
    }

    public void Update()
    {
        OrthographicCamera_Update(nativeCamera);
    }

    public void SetPosition(float x, float y)
    {
        OrthographicCamera_SetPosition(nativeCamera, x, y);
    }

    public void SetViewport(float viewportWidth, float viewportHeight)
    {
        OrthographicCamera_SetViewport(nativeCamera, viewportWidth, viewportHeight);
    }

    public void SetAngle(float deg)
    {
        OrthographicCamera_SetAngle(nativeCamera, deg);
    }

    public void SetZoom(float zoom)
    {
        OrthographicCamera_setZoom(nativeCamera, zoom);
    }

    public float GetX()
    {
        return OrthographicCamera_GetX(nativeCamera);
    }

    public float GetY()
    {
        return OrthographicCamera_GetY(nativeCamera);
    }

    public float GetZoom()
    {
        return OrthographicCamera_GetZoom(nativeCamera);
    }

    public float GetAngle()
    {
        return OrthographicCamera_GetAngle(nativeCamera);
    }

    internal IntPtr GetNativePtr()
    {
        return nativeCamera;
    }

    [DllImport("RenderingAPI.dll")]
    private static extern IntPtr OrthographicCamera_Create(float x, float y, float viewportWidth, float viewportHeight);

    [DllImport("RenderingAPI.dll")]
    private static extern void OrthographicCamera_Release(IntPtr camera);

    [DllImport("RenderingAPI.dll")]
    private static extern void OrthographicCamera_Update(IntPtr camera);

    [DllImport("RenderingAPI.dll")]
    private static extern void OrthographicCamera_SetPosition(IntPtr camera, float x, float y);

    [DllImport("RenderingAPI.dll")]
    private static extern void OrthographicCamera_SetViewport(IntPtr camera, float viewportWidth, float viewportHeight);

    [DllImport("RenderingAPI.dll")]
    private static extern void OrthographicCamera_SetAngle(IntPtr camera, float deg);

    [DllImport("RenderingAPI.dll")]
    private static extern void OrthographicCamera_setZoom(IntPtr camera, float zoom);

    [DllImport("RenderingAPI.dll")]
    private static extern float OrthographicCamera_GetX(IntPtr camera);

    [DllImport("RenderingAPI.dll")]
    private static extern float OrthographicCamera_GetY(IntPtr camera);

    //[DllImport("RenderingAPI.dll")]
    //private static extern float OrthographicCamera_GetViewportWidth(IntPtr camera);

    //[DllImport("RenderingAPI.dll")]
    //private static extern float OrthographicCamera_GetViewportHeight(IntPtr camera);

    [DllImport("RenderingAPI.dll")]
    private static extern float OrthographicCamera_GetZoom(IntPtr camera);

    [DllImport("RenderingAPI.dll")]
    private static extern float OrthographicCamera_GetAngle(IntPtr camera);
}
