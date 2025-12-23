#ifndef TETRISRAWBOARDDATA_H
#define TETRISRAWBOARDDATA_H

#include "ITetrisRawBoardData.h"

#include <memory>

namespace TetrisLib
{
    struct TetrisRawBoardDataImpl;
    class TetrisRawBoardData : public ITetrisRawBoardData
    {
    public:
        TetrisRawBoardData( int boardWidth, int boardHeight );

        void Reset() override;

        int GetWidth() const override;
        int GetHeight() const override;

        SpotColor GetSpotColor( int x, int y ) const override;
        void SetSpotColor( int x, int y, SpotColor spotColor ) override;

    private:
        std::unique_ptr<TetrisRawBoardDataImpl> _impl;
    };
}

#endif // TETRISRAWBOARDDATA_H
