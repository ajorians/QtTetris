#ifndef SPOTINFO_H
#define SPOTINFO_H

#include "SpotColor.h"

namespace TetrisLib
{
    enum class SpotOrigin
    {
        Nothing,
        BoardPiece,
        CurrentPiece,
        PreviewPiece
    };

    struct SpotInfo
    {
        SpotColor SpotColor;
        SpotOrigin SpotOrigin;
    };
}

#endif // SPOTINFO_H
