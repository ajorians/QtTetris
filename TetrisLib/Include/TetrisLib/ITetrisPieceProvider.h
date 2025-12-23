#ifndef ITETRISPIECEPROVIDER_H
#define ITETRISPIECEPROVIDER_H

#include <memory>

namespace TetrisLib
{
    class ITetrisPiece;

    class ITetrisPieceProvider
    {
    public:
        virtual ~ITetrisPieceProvider() {}

        virtual std::shared_ptr<ITetrisPiece> GetNextPiece() = 0;
    };
}

#endif // ITETRISPIECEPROVIDER_H
