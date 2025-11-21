#ifndef ITETRISPIECEPROVIDER_H
#define ITETRISPIECEPROVIDER_H

#include <memory>

namespace TetrisLib
{
    class ITetrisPiece;
    class ITetrisRawBoardData;

    class ITetrisPieceProvider
    {
    public:
        virtual ~ITetrisPieceProvider() {}

        virtual std::unique_ptr<ITetrisPiece> GetNextPiece( std::shared_ptr<ITetrisRawBoardData> tetrisRawBoardData ) = 0;
    };
}

#endif // ITETRISPIECEPROVIDER_H
