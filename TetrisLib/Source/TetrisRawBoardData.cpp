#include "TetrisLib/TetrisRawBoardData.h"

#include "TetrisLib/SpotColor.h"

#include <algorithm>
#include <vector>

using namespace TetrisLib;

namespace TetrisLib
{
    struct TetrisRawBoardDataImpl
    {
        TetrisRawBoardDataImpl( int boardWidth, int boardHeight )
            : _boardWidth( boardWidth )
            , _boardHeight( boardHeight )
        {
            const int totalNumSpots = boardWidth * boardHeight;

            _boardData.resize( totalNumSpots );
            std::fill( _boardData.begin(), _boardData.end(), SpotColor::Nothing );
        }

        int GetWidth() const
        {
            return _boardWidth;
        }

        int GetHeight() const
        {
            return _boardHeight;
        }

        SpotColor GetSpotColor( int x, int y ) const
        {
            if( x < 0 || x >= _boardWidth || y < 0 || y >= _boardHeight )
                return SpotColor::Nothing;

            const int pos = y * _boardWidth + x;
            return _boardData[pos];
        }

        void SetSpotColor( int x, int y, SpotColor spotColor )
        {
            if( x < 0 || x >= _boardWidth || y < 0 || y >= _boardHeight )
                return;

            const int pos = y * _boardWidth + x;
            _boardData[pos] = spotColor;
        }

    private:
        std::vector<SpotColor> _boardData;
        const int _boardWidth;
        const int _boardHeight;
    };
}

TetrisRawBoardData::TetrisRawBoardData( int boardWidth, int boardHeight )
{
    _impl.reset( new TetrisRawBoardDataImpl( boardWidth, boardHeight ));
}

int TetrisRawBoardData::GetWidth() const
{
    return _impl->GetWidth();
}

int TetrisRawBoardData::GetHeight() const
{
    return _impl->GetHeight();
}

SpotColor TetrisRawBoardData::GetSpotColor( int x, int y ) const
{
    return _impl->GetSpotColor( x, y );
}

void TetrisRawBoardData::SetSpotColor( int x, int y, SpotColor spotColor )
{
    _impl->SetSpotColor( x, y, spotColor );
}
