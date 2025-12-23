#include "TetrisLib/TetrisBoardData.h"

#include "TetrisLib/Action.h"
#include "TetrisLib/Direction.h"
#include "TetrisLib/SpotColor.h"
#include "TetrisLib/SpotInfo.h"
#include "TetrisLib/ITetrisPieceProvider.h"
#include "TetrisLib/ITetrisRawBoardData.h"
#include "TetrisLib/TetrisPiece.h"

#include "TetrisLib/ITetrisPiece.h"
#include "TetrisLib/ITetrisBoardObserver.h"

#include <cassert>
#include <set>

using namespace TetrisLib;

namespace TetrisLib
{
    struct TetrisBoardDataImpl
    {
        TetrisBoardDataImpl( std::shared_ptr<ITetrisRawBoardData> boardData,
                            std::shared_ptr<ITetrisPieceProvider> tetrisPieceProvider )
            : _boardData( boardData )
            , _tetrisPieceProvider( tetrisPieceProvider )
        {
            Reset();
        }

        void AddObserver( ITetrisBoardObserver* observer )
        {
            _observers.insert( observer );
        }

        void RemoveObserver( ITetrisBoardObserver* observer )
        {
            auto it = _observers.find( observer );
            if( it != _observers.end() )
            {
                _observers.erase( it );
            }
        }

        void Reset()
        {
            _boardData->Reset();
            _currentPiece.reset();

            ObserverUpdate();
        }

        bool IsGameInProgress() const { return _gameInProgress; }

        int GetWidth() const { return _boardData->GetWidth(); }
        int GetHeight() const { return _boardData->GetHeight(); }

        SpotInfo GetSpotInfo( int x, int y ) const
        {
            SpotColor spotColor = _boardData->GetSpotColor( x, y );
            SpotOrigin spotOrigin = SpotOrigin::Nothing;

            if( spotColor != SpotColor::Nothing )
            {
                spotOrigin = SpotOrigin::BoardPiece;
            }

            if( _currentPiece )
            {
                if( _currentPiece->IsAtSpot( x, y ) )
                {
                    spotColor = _currentPiece->GetSpotColor();
                    spotOrigin = SpotOrigin::CurrentPiece;
                }
                else if( _currentPiece->WillBeAtSpot( x, y ) )
                {
                    spotColor = _currentPiece->GetSpotColor();
                    spotOrigin = SpotOrigin::PreviewPiece;
                }
            }

            SpotInfo result
            {
                .SpotColor = spotColor,
                .SpotOrigin = spotOrigin
            };

            return result;
        }

        std::shared_ptr<ITetrisPiece> GetCurrentPiece() const
        {
            return _currentPiece;
        }

        void PerformAction( Action action )
        {
            if( action == Action::Left )
            {
                MovePiece( Direction::Left );
            }
            else if( action == Action::Right )
            {
                MovePiece( Direction::Right );
            }
            else if( action == Action::Rotate )
            {
                RotatePiece();
            }
            else if( action == Action::ZipDown )
            {
                ZipPieceDown();
            }
        }

        void RotatePiece()
        {
            if( _currentPiece && _currentPiece->Rotate() )
            {
                ObserverUpdate();
            }
        }

        void MovePiece( Direction direction )
        {
            if( _currentPiece && _currentPiece->Move( direction ) )
            {
                ObserverUpdate();
            }
        }

        void ZipPieceDown()
        {
            if( !_currentPiece )
                return;

            while( _currentPiece->Move( Direction::Down ) )
            {
            }

            _currentPiece->ApplyToBoard();
            _currentPiece.reset();

            //Check for completed lines
            RemoveCompletedLines();

            CreateNewPiece();

            ObserverUpdate();
        }

        void TimerDrop()
        {
            if( !_currentPiece )
            {
                CreateNewPiece();

                ObserverUpdate();
                return;
            }

            if( !_currentPiece->MoveDownOneRow() )
            {
                //Piece reached bottom
                _currentPiece->ApplyToBoard();
                _currentPiece.reset();

                //Check for completed lines
                RemoveCompletedLines();
            }

            ObserverUpdate();
        }

    private:
        void ObserverUpdate()
        {
            for( ITetrisBoardObserver* observer : _observers )
            {
                observer->ObserverUpdate();
            }
        }

        void CreateNewPiece()
        {
            assert( _currentPiece == nullptr );//Maybe in the future will allow changing out the current piece mid-play

            if( !_gameInProgress )
                return;

            _currentPiece = _tetrisPieceProvider->GetNextPiece( _boardData );

            if( _currentPiece == nullptr )
            {
                //Game over
                _gameInProgress = false;
            }
        }

        void RemoveCompletedLines()
        {
            //This needs to be in order from top to bottom as I'll loop in that order
            std::vector<int> completedLineIndexes = FindCompletedLines();

            while( !completedLineIndexes.empty() )
            {
                RemoveCompletedLine( completedLineIndexes.front() );
                completedLineIndexes.erase( completedLineIndexes.begin() );
            }
        }

        std::vector<int> FindCompletedLines() const
        {
            //This needs to be in order from top to bottom as I'll loop in that order
            const int boardWidth = GetWidth();
            const int boardHeight = GetHeight();

            std::vector<int> result;
            for( int y = boardHeight-1; y >= 0; y-- )
            {
                bool allFilled = true;
                for( int x = 0; x < boardWidth; x++ )
                {
                    if( _boardData->GetSpotColor(x, y ) == SpotColor::Nothing )
                    {
                        allFilled = false;
                        break;
                    }
                }

                if( allFilled )
                {
                    result.push_back( y );
                }
            }

            return result;
        }

        void RemoveCompletedLine( int lineIndex )
        {
            const int boardWidth = GetWidth();
            const int boardHeight = GetHeight();

            for( int x = 0; x < boardWidth; x++ )
            {
                for( int y = lineIndex; y < boardHeight; y++ )
                {
                    auto spotColorAbove = _boardData->GetSpotColor(x, y + 1 );
                    _boardData->SetSpotColor(x, y, spotColorAbove );
                }
            }
        }

        std::shared_ptr<ITetrisRawBoardData> _boardData;
        std::shared_ptr<ITetrisPieceProvider> _tetrisPieceProvider;
        std::set<ITetrisBoardObserver*> _observers;

        std::shared_ptr<ITetrisPiece> _currentPiece;
        bool _gameInProgress = true;
    };
}

TetrisBoardData::TetrisBoardData( std::shared_ptr<ITetrisRawBoardData> boardData,
                                 std::shared_ptr<ITetrisPieceProvider> tetrisPieceProvider )
{
    _impl.reset( new TetrisBoardDataImpl( boardData, tetrisPieceProvider ) );
}

void TetrisBoardData::AddObserver( ITetrisBoardObserver* observer )
{
    _impl->AddObserver( observer );
}

void TetrisBoardData::RemoveObserver( ITetrisBoardObserver* observer )
{
    _impl->RemoveObserver( observer );
}

void TetrisBoardData::Reset()
{
    _impl->Reset();
}

bool TetrisBoardData::IsGameInProgress() const
{
    return _impl->IsGameInProgress();
}

int TetrisBoardData::GetWidth() const
{
    return _impl->GetWidth();
}

int TetrisBoardData::GetHeight() const
{
    return _impl->GetHeight();
}

SpotInfo TetrisBoardData::GetSpotInfo( int x, int y ) const
{
    return _impl->GetSpotInfo( x, y );
}

std::shared_ptr<ITetrisPiece> TetrisBoardData::GetCurrentPiece() const
{
    return _impl->GetCurrentPiece();
}

void TetrisBoardData::PerformAction( Action action )
{
    _impl->PerformAction( action );
}

void TetrisBoardData::RotatePiece()
{
    _impl->RotatePiece();
}

void TetrisBoardData::MovePiece( Direction direction )
{
    _impl->MovePiece( direction );
}

void TetrisBoardData::ZipPieceDown()
{
    _impl->ZipPieceDown();
}

void TetrisBoardData::TimerDrop()
{
    _impl->TimerDrop();
}
