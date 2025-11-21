#ifndef TETRISPIECEPROVIDER_H
#define TETRISPIECEPROVIDER_H

#include "TetrisLib/ITetrisPieceProvider.h"

namespace TetrisLib
{
    class TetrisPieceProvider : public TetrisLib::ITetrisPieceProvider
    {
    public:
        TetrisPieceProvider();

        std::unique_ptr<ITetrisPiece> GetNextPiece( std::shared_ptr<ITetrisRawBoardData> tetrisRawBoardData ) override;
    };
}

#endif // TETRISPIECEPROVIDER_H
