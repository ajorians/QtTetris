#ifndef ITETRISPIECE_H
#define ITETRISPIECE_H

#include <memory>

namespace TetrisLib
{
    class ITetrisRawBoardData;

    enum class Direction;
    enum class SpotColor;

    class ITetrisPiece
    {
    public:
        virtual ~ITetrisPiece() {}

        virtual bool CanBePlaced( std::shared_ptr<ITetrisRawBoardData> boardData ) const = 0;
        virtual bool CanMove( std::shared_ptr<ITetrisRawBoardData> boardData, Direction direction ) const = 0;
        virtual bool CanRotate( std::shared_ptr<ITetrisRawBoardData> boardData ) const = 0;

        virtual bool Rotate( std::shared_ptr<ITetrisRawBoardData> boardData ) = 0;
        virtual bool Move( std::shared_ptr<ITetrisRawBoardData> boardData, Direction direction ) = 0;
        virtual bool MoveDownOneRow( std::shared_ptr<ITetrisRawBoardData> boardData ) = 0;//False means it couldn't move lower
        virtual bool IsAtSpot( int x, int y) const = 0;
        virtual bool WillBeAtSpot( std::shared_ptr<ITetrisRawBoardData> boardData, int x, int y ) const = 0;
        virtual SpotColor GetSpotColor() const = 0;

        virtual void ApplyToBoard( std::shared_ptr<ITetrisRawBoardData> boardData ) = 0;
    };
}

#endif // ITETRISPIECE_H
