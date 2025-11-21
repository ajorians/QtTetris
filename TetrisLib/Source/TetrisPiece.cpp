#include "TetrisLib/TetrisPiece.h"

#include "TetrisLib/Direction.h"
#include "TetrisLib/SpotColor.h"
#include "TetrisLib/ITetrisRawBoardData.h"

using namespace TetrisLib;

namespace TetrisLib
{
    struct TetrisPieceImpl
    {
        TetrisPieceImpl( std::shared_ptr<ITetrisRawBoardData> boardData, SpotColor spotColor, const std::vector<std::pair<int, int>>& relativePieces )
            : _boardData( boardData )
            , _spotColor( spotColor )
            , _relativePieces( relativePieces )
        {
            _x = _boardData->GetWidth() / 2;
            _y = _boardData->GetHeight() - 1/*index*/;
        }

        bool CanBePlaced() const
        {
            return DoRelativeSpotsWork( _relativePieces );
        }

        bool Rotate()
        {
            std::vector<std::pair<int, int>> newRelativeSpots;
            for( const auto& [relX, relY] : _relativePieces )
            {
                newRelativeSpots.push_back( { relY, relX * -1 } );
            }

            if( !DoRelativeSpotsWork( newRelativeSpots ) )
                return false;

            _relativePieces = newRelativeSpots;

            return true;
        }

        bool Move( Direction direction )
        {
            if( !CanMove( direction ) )
                return false;

            if( direction == Direction::Left )
            {
                _x--;
            }
            else if( direction == Direction::Right )
            {
                _x++;
            }
            else if( direction == Direction::Down )
            {
                _y--;
            }

            return true;
        }

        bool MoveDownOneRow()
        {
            return Move( Direction::Down );
        }

        bool IsAtSpot( int x, int y ) const
        {
            for( const auto& [relX, relY] : _relativePieces )
            {
                if( x == _x + relX && y == _y + relY )
                    return true;
            }

            return false;
        }

        SpotColor GetSpotColor() const
        {
            return _spotColor;
        }

        void ApplyToBoard()
        {
            for( const auto& [relX, relY] : _relativePieces )
            {
                int x = _x + relX;
                int y = _y + relY;

                _boardData->SetSpotColor( x, y, _spotColor );
            }
        }

    private:

        std::vector<std::pair<int, int>> GetNewRelativeWithDirection( Direction direction ) const
        {
            std::vector<std::pair<int, int>> newRelativeSpots;

            int changeX = 0, changeY = 0;
            if( direction == Direction::Left )
            {
                changeX = -1;
            }
            else if( direction == Direction::Right )
            {
                changeX = 1;
            }
            else if( direction == Direction::Down )
            {
                changeY = -1;
            }

            for( const auto& [relX, relY] : _relativePieces )
            {
                newRelativeSpots.push_back( { relX + changeX, relY + changeY } );
            }

            return newRelativeSpots;
        }

        bool CanMove( Direction direction ) const
        {
            std::vector newRelativeSpots = GetNewRelativeWithDirection( direction );

            return DoRelativeSpotsWork( newRelativeSpots );
        }

        bool DoRelativeSpotsWork( const std::vector<std::pair<int, int>> relativeSpots ) const
        {
            for( const auto& [relX, relY] : relativeSpots )
            {
                int x = _x + relX;
                int y = _y + relY;

                if( x < 0 || x >= _boardData->GetWidth() )
                    return false;

                if( y < 0 )
                    return false;

                auto spotColorAtSpot = _boardData->GetSpotColor( x, y );
                if( spotColorAtSpot != SpotColor::Nothing )
                    return false;
            }

            return true;
        }

        std::shared_ptr<ITetrisRawBoardData> _boardData;

        int _x = 0;
        int _y = 0;
        SpotColor _spotColor;
        std::vector<std::pair<int, int>> _relativePieces;
    };
}

TetrisPiece::TetrisPiece( std::shared_ptr<ITetrisRawBoardData> boardData, SpotColor spotColor, const std::vector<std::pair<int, int>>& relativePieces )
{
    _impl.reset( new TetrisPieceImpl( boardData, spotColor, relativePieces ) );
}

bool TetrisPiece::CanBePlaced() const
{
    return _impl->CanBePlaced();
}

bool TetrisPiece::Rotate()
{
    return _impl->Rotate();
}

bool TetrisPiece::Move( Direction direction )
{
    return _impl->Move( direction );
}

bool TetrisPiece::MoveDownOneRow()
{
    return _impl->MoveDownOneRow();
}

bool TetrisPiece::IsAtSpot( int x, int y) const
{
    return _impl->IsAtSpot( x, y );
}

SpotColor TetrisPiece::GetSpotColor() const
{
    return _impl->GetSpotColor();
}

void TetrisPiece::ApplyToBoard()
{
    _impl->ApplyToBoard();
}
