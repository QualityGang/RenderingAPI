using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

struct TextureSize
{
    public uint Width, Height, Depth;

    public TextureSize(uint width, uint height, uint depth)
    {
        Width  = width;
        Height = height;
        Depth  = depth;
    }
}
