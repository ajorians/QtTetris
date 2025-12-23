#include "TetrisAILib/TetrisAI.h"

#include <TetrisLib/ITetrisBoardData.h>

#include <vector>

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

            TetrisLib::Action bestAction = FindBestAction();
            return bestAction;
        }

    private:
        TetrisLib::Action FindBestAction()
        {
            std::vector<std::shared_ptr<TetrisLib::ITetrisBoardData>> resultingChanges = BuildResultingChanges( _tetrisBoardData );





            return TetrisLib::Action::Nothing;
        }

        std::vector<std::shared_ptr<TetrisLib::ITetrisBoardData>> BuildResultingChanges( std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData )
        {
            return {};
        }


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

