#ifndef SPOTCOLOR_H
#define SPOTCOLOR_H

namespace TetrisLib
{
    enum class SpotColor
    {
        Nothing,
        Red,//Staight
        Yellow,//3 + bend
        Purple,//3 + bend
        LiteBlue,//Square
        DarkBlue,//Bent
        Green,//Bent
        Gray//T-shape
    };
}

#endif // SPOTCOLOR_H
