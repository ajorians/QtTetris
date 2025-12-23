#ifndef ITETRISBOARDDATA_H
#define ITETRISBOARDDATA_H

namespace TetrisLib
{
    struct SpotInfo;

    enum class SpotColor;
    enum class Direction;

    class ITetrisBoardData
    {
    public:
        virtual ~ITetrisBoardData(){}

        virtual void Reset() = 0;

        virtual bool IsGameInProgress() const = 0;

        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;

        virtual SpotInfo GetSpotInfo( int x, int y ) const = 0;

        virtual void RotatePiece() = 0;
        virtual void MovePiece( Direction direction ) = 0;
        virtual void ZipPieceDown() = 0;

        virtual void TimerDrop() = 0;
    };
}

#endif // ITETRISBOARDDATA_H
