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
        TetrisPiece( std::shared_ptr<ITetrisRawBoardData> _boardData, SpotColor spotColor, const std::vector<std::pair<int, int>>& relativePieces );

        bool CanBePlaced() const override;

        bool Rotate() override;
        bool Move( Direction direction ) override;
        bool MoveDownOneRow() override;
        bool IsAtSpot( int x, int y) const override;
        SpotColor GetSpotColor() const override;

        void ApplyToBoard() override;

    private:
        std::unique_ptr<TetrisPieceImpl> _impl;
    };
}

#endif // TETRISPIECE_H
