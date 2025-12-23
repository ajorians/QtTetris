#include "TetrisAILib/TetrisAI.h"

#include <TetrisLib/ITetrisBoardData.h>

using namespace TetrisAILib;

namespace TetrisAILib
{
    struct TetrisAIImpl
    {
        TetrisAIImpl( std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData )
            : _tetrisBoardData( tetrisBoardData )
        {
        }

        std::optional<TetrisLib::Action> MakeMove()
        {
            if( !_tetrisBoardData->IsGameInProgress() )
                return {};

            auto currentPiece = _tetrisBoardData->GetCurrentPiece();
            if( !currentPiece )
                return{};

            return TetrisLib::Action::ZipDown;
        }

    private:
        std::shared_ptr<TetrisLib::ITetrisBoardData> _tetrisBoardData;
    };
}

TetrisAI::TetrisAI( std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData)
{
    _impl.reset( new TetrisAIImpl( tetrisBoardData ) );
}

TetrisAI::~TetrisAI()
{
}

std::optional<TetrisLib::Action> TetrisAI::MakeMove()
{
    return _impl->MakeMove();
}

