#ifndef ITETRISPIECE_H
#define ITETRISPIECE_H

namespace TetrisLib
{
    enum class Direction;
    enum class SpotColor;

    class ITetrisPiece
    {
    public:
        virtual ~ITetrisPiece() {}

        virtual bool CanBePlaced() const = 0;

        virtual bool Rotate() = 0;
        virtual bool Move( Direction direction ) = 0;
        virtual bool MoveDownOneRow() = 0;//False means it couldn't move lower
        virtual bool IsAtSpot( int x, int y) const = 0;
        virtual bool WillBeAtSpot( int x, int y ) const = 0;
        virtual SpotColor GetSpotColor() const = 0;

        virtual void ApplyToBoard() = 0;
    };
}

#endif // ITETRISPIECE_H
