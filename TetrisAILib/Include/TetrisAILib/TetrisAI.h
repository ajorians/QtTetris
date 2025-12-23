#ifndef TETRISAI_H
#define TETRISAI_H

#include <memory>

namespace TetrisLib
{
class ITetrisBoardData;
}

namespace TetrisAILib
{
    struct TetrisAIImpl;
    class TetrisAI
    {
    public:
        TetrisAI( std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData );
        ~TetrisAI();

        void MakeMove();

    private:
        std::unique_ptr<TetrisAIImpl> _impl;
    };
}

#endif // TETRISAI_H
