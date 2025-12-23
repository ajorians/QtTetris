#include "TetrisLib/TetrisPiece.h"

#include "TetrisLib/Direction.h"
#include "TetrisLib/SpotColor.h"
#include "TetrisLib/ITetrisRawBoardData.h"

using namespace TetrisLib;

namespace TetrisLib
{
    struct TetrisPieceImpl
    {
        TetrisPieceImpl( SpotColor spotColor,
                        const std::vector<std::pair<int, int>>& relativePieces,
                        int x,
                        int y )
            : _spotColor( spotColor )
            , _relativePieces( relativePieces )
            , _x( x )
            , _y( y )
        {
        }

        bool CanBePlaced( std::shared_ptr<ITetrisRawBoardData> boardData ) const
        {
            return DoRelativeSpotsWork( boardData, _relativePieces );
        }

        bool Rotate( std::shared_ptr<ITetrisRawBoardData> boardData )
        {
            std::vector<std::pair<int, int>> newRelativeSpots;
            for( const auto& [relX, relY] : _relativePieces )
            {
                newRelativeSpots.push_back( { relY, relX * -1 } );
            }

            if( !DoRelativeSpotsWork( boardData, newRelativeSpots ) )
                return false;

            _relativePieces = newRelativeSpots;

            return true;
        }

        bool Move( std::shared_ptr<ITetrisRawBoardData> boardData, Direction direction )
        {
            if( !CanMove( boardData, direction ) )
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

        bool MoveDownOneRow( std::shared_ptr<ITetrisRawBoardData> boardData )
        {
            return Move( boardData, Direction::Down );
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

        bool WillBeAtSpot( std::shared_ptr<ITetrisRawBoardData> boardData, int x, int y ) const
        {
            if( IsAtSpot( x, y ) )
                return false;

            TetrisPieceImpl previewPiece( *this );
            while( previewPiece.MoveDownOneRow( boardData ) ){}

            return previewPiece.IsAtSpot( x, y );
        }

        SpotColor GetSpotColor() const
        {
            return _spotColor;
        }

        void ApplyToBoard( std::shared_ptr<ITetrisRawBoardData> boardData )
        {
            for( const auto& [relX, relY] : _relativePieces )
            {
                int x = _x + relX;
                int y = _y + relY;

                boardData->SetSpotColor( x, y, _spotColor );
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

        bool CanMove( std::shared_ptr<ITetrisRawBoardData> boardData, Direction direction ) const
        {
            std::vector newRelativeSpots = GetNewRelativeWithDirection( direction );

            return DoRelativeSpotsWork( boardData, newRelativeSpots );
        }

        bool DoRelativeSpotsWork( std::shared_ptr<ITetrisRawBoardData> boardData, const std::vector<std::pair<int, int>> relativeSpots ) const
        {
            for( const auto& [relX, relY] : relativeSpots )
            {
                int x = _x + relX;
                int y = _y + relY;

                if( x < 0 || x >= boardData->GetWidth() )
                    return false;

                if( y < 0 )
                    return false;

                auto spotColorAtSpot = boardData->GetSpotColor( x, y );
                if( spotColorAtSpot != SpotColor::Nothing )
                    return false;
            }

            return true;
        }

        int _x = 0;
        int _y = 0;
        SpotColor _spotColor;
        std::vector<std::pair<int, int>> _relativePieces;
    };
}

TetrisPiece::TetrisPiece( SpotColor spotColor,
                         const std::vector<std::pair<int, int>>& relativePieces,
                         int x,
                         int y )
{
    _impl.reset( new TetrisPieceImpl( spotColor, relativePieces, x, y ) );
}

bool TetrisPiece::CanBePlaced( std::shared_ptr<ITetrisRawBoardData> boardData ) const
{
    return _impl->CanBePlaced( boardData );
}

bool TetrisPiece::Rotate( std::shared_ptr<ITetrisRawBoardData> boardData )
{
    return _impl->Rotate( boardData );
}

bool TetrisPiece::Move( std::shared_ptr<ITetrisRawBoardData> boardData, Direction direction )
{
    return _impl->Move( boardData, direction );
}

bool TetrisPiece::MoveDownOneRow( std::shared_ptr<ITetrisRawBoardData> boardData )
{
    return _impl->MoveDownOneRow( boardData );
}

bool TetrisPiece::IsAtSpot( int x, int y) const
{
    return _impl->IsAtSpot( x, y );
}

bool TetrisPiece::WillBeAtSpot( std::shared_ptr<ITetrisRawBoardData> boardData, int x, int y ) const
{
    return _impl->WillBeAtSpot( boardData, x, y );
}

SpotColor TetrisPiece::GetSpotColor() const
{
    return _impl->GetSpotColor();
}

void TetrisPiece::ApplyToBoard( std::shared_ptr<ITetrisRawBoardData> boardData )
{
    _impl->ApplyToBoard( boardData );
}
