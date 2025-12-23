#ifndef ITETRISBOARDDATA_H
#define ITETRISBOARDDATA_H

#include <memory>

namespace TetrisLib
{
    class ITetrisBoardObserver;
    class ITetrisPiece;
    class ITetrisRawBoardData;

    struct SpotInfo;

    enum class Action;
    enum class SpotColor;
    enum class Direction;

    class ITetrisBoardData
    {
    public:
        virtual ~ITetrisBoardData(){}

        virtual void AddObserver( ITetrisBoardObserver* observer ) = 0;
        virtual void RemoveObserver( ITetrisBoardObserver* observer ) = 0;

        virtual void Reset() = 0;

        virtual bool IsGameInProgress() const = 0;

        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;

        virtual SpotInfo GetSpotInfo( int x, int y ) const = 0;

        virtual std::shared_ptr<ITetrisPiece> GetCurrentPiece() const = 0;
        virtual void SetCurrentPiece( std::shared_ptr<ITetrisPiece> piece ) = 0;

        virtual std::shared_ptr<ITetrisRawBoardData> GetRawBoard() const = 0;

        virtual bool CanPerformAction( Action action ) const = 0;
        virtual void PerformAction( Action action ) = 0;

        //Can remove if desirable
        virtual void RotatePiece() = 0;
        virtual void MovePiece( Direction direction ) = 0;
        virtual void ZipPieceDown() = 0;

        virtual void TimerDrop() = 0;
    };
}

#endif // ITETRISBOARDDATA_H
