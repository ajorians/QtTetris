#ifndef TETRISPIECEPROVIDER_H
#define TETRISPIECEPROVIDER_H

#include "TetrisLib/ITetrisPieceProvider.h"

namespace TetrisLib
{
    class TetrisPieceProvider : public TetrisLib::ITetrisPieceProvider
    {
    public:
        TetrisPieceProvider( int width, int height );

        std::shared_ptr<ITetrisPiece> GetNextPiece() override;

    private:
        int _width;
        int _height;
    };
}

#endif // TETRISPIECEPROVIDER_H
