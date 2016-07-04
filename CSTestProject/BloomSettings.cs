
struct BloomSettings
{
    public float BloomThreshold;
    public float BlurAmount;
    public float BloomIntensity;
    public float BaseIntensity;
    public float BloomSaturation;
    public float BaseSaturation;

    public BloomSettings(float bloomThreshold,  float blurAmount,
                         float bloomIntensity,  float baseIntensity,
                         float bloomSaturation, float baseSaturation)
    {
        BloomThreshold  = bloomThreshold;
        BlurAmount      = blurAmount;
        BloomIntensity  = bloomIntensity;
        BaseIntensity   = baseIntensity;
        BloomSaturation = bloomSaturation;
        BaseSaturation  = baseSaturation;
    }
        

    /// <summary>
    /// Table of preset bloom settings, used by the sample program.
    /// </summary>
    public static BloomSettings[] PresetSettings =
    {
                                         // Thresh  Blur Bloom  Base  BloomSat BaseSat
        /* Default     */ new BloomSettings(0.25f,  4,   1.25f, 1,    1,       1),
        /* Soft        */ new BloomSettings(0,      3,   1,     1,    1,       1),
        /* Desaturated */ new BloomSettings(0.5f,   8,   2,     1,    0,       1),
        /* Saturated   */ new BloomSettings(0.25f,  4,   2,     1,    2,       0),
        /* Blurry      */ new BloomSettings(0,      2,   1,     0.1f, 1,       1),
		/* Subtle      */ new BloomSettings(0.5f,   2,   1,     1,    1,       1),
    };
}
