#ifndef TETRISPIECE_H
#define TETRISPIECE_H

#include "ITetrisPiece.h"

#include <memory>
#include <utility>
#include <vector>

namespace TetrisLib
{
    class ITetrisRawBoardData;

    struct TetrisPieceImpl;
    class TetrisPiece : public ITetrisPiece
    {
    public:
        TetrisPiece( SpotColor spotColor,
                    const std::vector<std::pair<int, int>>& relativePieces,
                    int x,
                    int y );

        std::shared_ptr<ITetrisPiece> Clone() const override;

        bool CanBePlaced( std::shared_ptr<ITetrisRawBoardData> boardData ) const override;
        bool CanMove( std::shared_ptr<ITetrisRawBoardData> boardData, Direction direction ) const override;
        bool CanRotate( std::shared_ptr<ITetrisRawBoardData> boardData ) const override;

        bool Rotate( std::shared_ptr<ITetrisRawBoardData> boardData ) override;
        bool Move( std::shared_ptr<ITetrisRawBoardData> boardData, Direction direction ) override;
        bool MoveDownOneRow( std::shared_ptr<ITetrisRawBoardData> boardData ) override;
        bool IsAtSpot( int x, int y) const override;
        bool WillBeAtSpot( std::shared_ptr<ITetrisRawBoardData> boardData, int x, int y ) const override;
        SpotColor GetSpotColor() const override;

        void ApplyToBoard( std::shared_ptr<ITetrisRawBoardData> boardData ) override;

    private:
        std::unique_ptr<TetrisPieceImpl> _impl;
    };
}

#endif // TETRISPIECE_H
