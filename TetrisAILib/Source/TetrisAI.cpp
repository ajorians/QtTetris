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
        std::shared_ptr<TetrisLib::ITetrisBoardData> PerformedTetrisBoardData;
        TetrisLib::Action Action;
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
            std::vector resultingChanges = BuildResultingChanges( _tetrisBoardData );

            /*std::vector<int> maxLines;
            maxLines.push_back(GetMaxLinesHeight( _tetrisBoardData ) );
            for( int i=0; i<resultingChanges.size(); i++ )
            {
                maxLines.push_back(GetMaxLinesHeight( resultingChanges[i].PerformedTetrisBoardData ) );
            }*/

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
                return resultingChanges[indexWithHighest].Action;
            }

            return TetrisLib::Action::Nothing;
        }

        std::vector<PerformedChange> BuildResultingChanges( const std::shared_ptr<TetrisLib::ITetrisBoardData>& tetrisBoardData ) const
        {
            std::vector<PerformedChange> result;

            //Move left
            result.push_back( WithAction( tetrisBoardData, TetrisLib::Action::Left ) );

            //Move Right
            result.push_back( WithAction( tetrisBoardData, TetrisLib::Action::Right ) );

            //Rotate
            result.push_back( WithAction( tetrisBoardData, TetrisLib::Action::Rotate ) );

            //Zip Down
            result.push_back( WithAction( tetrisBoardData, TetrisLib::Action::ZipDown ) );

            return result;
        }

        PerformedChange WithAction( const std::shared_ptr<TetrisLib::ITetrisBoardData>& tetrisBoardData, TetrisLib::Action action ) const
        {
            std::shared_ptr<const TetrisLib::ITetrisRawBoardData> tetrisRawBoardData = tetrisBoardData->GetRawBoard();
            std::shared_ptr<const TetrisLib::ITetrisPiece> currentPiece = tetrisBoardData->GetCurrentPiece();

            std::shared_ptr<TetrisLib::ITetrisRawBoardData> clonedRawBoardData = tetrisRawBoardData->Clone();
            std::shared_ptr<TetrisLib::ITetrisBoardData> performedActionBoardData( new TetrisLib::TetrisBoardData( clonedRawBoardData ) );

            performedActionBoardData->SetCurrentPiece( currentPiece->Clone() );
            performedActionBoardData->PerformAction( action );

            if( action != TetrisLib::Action::ZipDown )
            {
                performedActionBoardData->PerformAction( TetrisLib::Action::ZipDown );
            }

            PerformedChange result
            {
                .PerformedTetrisBoardData = performedActionBoardData,
                .Action = action,
                .Score = -100
            };

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

        int GetMaxLinesHeight( const std::shared_ptr<TetrisLib::ITetrisBoardData>& tetrisBoardData ) const
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

        int GetTotalGaps( const std::shared_ptr<TetrisLib::ITetrisBoardData>& tetrisBoardData ) const
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

