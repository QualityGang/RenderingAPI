#include "BloomSettings.h"

												// Thresh  Blur Bloom  Base  BloomSat BaseSat
const BloomSettings BloomSettings::Default	   = { 0.25f,  4,   1.25f, 1,    1,       1 };
const BloomSettings BloomSettings::Soft		   = { 0,      3,   1,     1,    1,       1 };
const BloomSettings BloomSettings::Desaturated = { 0.5f,   8,   2,     1,    0,       1 };
const BloomSettings BloomSettings::Saturated   = { 0.25f,  4,   2,     1,    2,       0 };
const BloomSettings BloomSettings::Blurry      = { 0,      2,   1,     0.1f, 1,       1 };
const BloomSettings BloomSettings::Subtle	   = { 0.5f,   2,   1,     1,    1,       1 };