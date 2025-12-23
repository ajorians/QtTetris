#include "TetrisAILib/TetrisAI.h"

#include <TetrisLib/Action.h>
#include <TetrisLib/ITetrisPiece.h>
#include <TetrisLib/ITetrisRawBoardData.h>
#include <TetrisLib/SpotInfo.h>
#include <TetrisLib/TetrisBoardData.h>

#include <cassert>
#include <vector>

using namespace TetrisAILib;

namespace
{
    struct PerformedChange
    {
        std::shared_ptr<const TetrisLib::ITetrisBoardData> PerformedTetrisBoardData;
        std::vector<TetrisLib::Action> Actions;
        int Score = -100;
    };
}

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
            PerformedChange root
            {
                .PerformedTetrisBoardData = _tetrisBoardData,
                .Actions = {},
                .Score = -100
            };

            std::vector resultingChanges = BuildResultingChanges( { root }, 2 );

            int indexWithHighest = -1;
            int currentHighest = -100;
            for( int i=0; i<resultingChanges.size(); i++ )
            {
                auto& change = resultingChanges[i];

                int score = ComputeHeristicScore( change );
                change.Score = score;

                if( score > currentHighest )
                {
                    currentHighest = score;
                    indexWithHighest = i;
                }
            }

            if( indexWithHighest >= 0 )
            {
                std::vector actions = resultingChanges[indexWithHighest].Actions;
                return actions[0];
            }

            return TetrisLib::Action::Nothing;
        }

        std::vector<PerformedChange> BuildResultingChanges( const std::vector<PerformedChange>& changes, int level ) const
        {
            if( level <= 0 )
            {
                return changes;
            }

            std::vector<PerformedChange> newChanges;
            for( const auto& change : changes )
            {
                std::vector moreChanges = BuildResultingChanges( change );

                newChanges.insert( newChanges.begin(), moreChanges.begin(), moreChanges.end() );
            }

            return BuildResultingChanges( newChanges, level - 1 );
        }

        std::vector<PerformedChange> BuildResultingChanges( const PerformedChange& change ) const
        {
            std::vector<PerformedChange> result;

            TetrisLib::Action actions[] =
            {
                TetrisLib::Action::Left,
                TetrisLib::Action::Right,
                TetrisLib::Action::Rotate,
                TetrisLib::Action::ZipDown
            };

            for( const TetrisLib::Action action : actions )
            {
                if( change.PerformedTetrisBoardData->GetCurrentPiece() == nullptr )
                    break;

                auto withAction = WithAction( change, action );
                result.push_back( withAction );
            }

            return result;
        }

        PerformedChange WithAction( const PerformedChange& change, const TetrisLib::Action& action ) const
        {
            std::shared_ptr<const TetrisLib::ITetrisBoardData> tetrisBoardData = change.PerformedTetrisBoardData;

            std::shared_ptr<const TetrisLib::ITetrisRawBoardData> tetrisRawBoardData = tetrisBoardData->GetRawBoard();
            std::shared_ptr<TetrisLib::ITetrisPiece> currentPiece = tetrisBoardData->GetCurrentPiece();

            std::shared_ptr<TetrisLib::ITetrisRawBoardData> clonedRawBoardData = tetrisRawBoardData->Clone();
            std::shared_ptr<TetrisLib::ITetrisBoardData> performedActionBoardData( new TetrisLib::TetrisBoardData( clonedRawBoardData ) );

            performedActionBoardData->SetCurrentPiece( currentPiece->Clone() );

            performedActionBoardData->PerformAction( action );

            PerformedChange result
            {
                .PerformedTetrisBoardData = performedActionBoardData,
                .Actions = change.Actions,
                .Score = -100
            };

            result.Actions.push_back( action );

            return result;
        }

        int ComputeHeristicScore( const PerformedChange& performedChange ) const
        {
            if( performedChange.PerformedTetrisBoardData->IsGameInProgress() == false )
            {
                return -100;//Would be a bad action
            }

            int linesHeight = GetMaxLinesHeight( performedChange.PerformedTetrisBoardData );
            int totalGaps = GetTotalGaps( performedChange.PerformedTetrisBoardData );

            //Not sure on the weights yet
            int score = linesHeight * -5 + totalGaps * -2;

            return score;
        }

        int GetMaxLinesHeight( const std::shared_ptr<const TetrisLib::ITetrisBoardData>& tetrisBoardData ) const
        {
            int maxLinesHeight = 0;

            for( int lineIndex = 0; lineIndex < tetrisBoardData->GetHeight(); lineIndex++ )
            {
                for( int x = 0; x < tetrisBoardData->GetWidth(); x++ )
                {
                    auto spotInfo = tetrisBoardData->GetSpotInfo( x, lineIndex );
                    if( spotInfo.SpotOrigin != TetrisLib::SpotOrigin::BoardPiece )
                        continue;

                    maxLinesHeight++;
                    break;
                }
            }

            return maxLinesHeight;
        }

        int GetTotalGaps( const std::shared_ptr<const TetrisLib::ITetrisBoardData>& tetrisBoardData ) const
        {
            int totalGaps = 0;
            for( int lineIndex = 0; lineIndex < tetrisBoardData->GetHeight() - 1; lineIndex++ )
            {
                for( int x = 0; x < tetrisBoardData->GetWidth(); x++ )
                {
                    auto spotInfo = tetrisBoardData->GetSpotInfo( x, lineIndex );
                    if( spotInfo.SpotOrigin == TetrisLib::SpotOrigin::Nothing )
                    {
                        //Is anything ever above it
                        for( int y=lineIndex + 1; y < tetrisBoardData->GetHeight(); y++)
                        {
                            auto aboveSpotInfo = tetrisBoardData->GetSpotInfo( x, y );
                            if( aboveSpotInfo.SpotOrigin == TetrisLib::SpotOrigin::BoardPiece )
                            {
                                totalGaps++;
                                break;
                            }
                        }
                    }
                }
            }

            return totalGaps;
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

