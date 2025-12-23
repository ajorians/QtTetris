#include "TetrisLib/TetrisBoardData.h"

#include "TetrisLib/Direction.h"
#include "TetrisLib/SpotColor.h"
#include "TetrisLib/ITetrisPieceProvider.h"
#include "TetrisLib/TetrisPiece.h"
#include "TetrisLib/TetrisRawBoardData.h"

#include "TetrisLib/ITetrisPiece.h"

#include <cassert>

using namespace TetrisLib;

namespace TetrisLib
{
    struct TetrisBoardDataImpl
    {
        TetrisBoardDataImpl( std::shared_ptr<ITetrisPieceProvider> tetrisPieceProvider,
                            std::function<void()> redrawFunc )
            : _redrawFunc( redrawFunc )
            , _tetrisPieceProvider( tetrisPieceProvider )
        {
            Reset();
        }

        void Reset()
        {
            _boardData.reset( new TetrisRawBoardData( 10, 20 ) );
            _currentPiece.reset();
        }

        bool IsGameInProgress() const { return _gameInProgress; }

        int GetWidth() const { return _boardData->GetWidth(); }
        int GetHeight() const { return _boardData->GetHeight(); }

        SpotColor GetSpotColor( int x, int y ) const
        {
            if( _currentPiece && _currentPiece->IsAtSpot(x, y))
            {
                return _currentPiece->GetSpotColor();
            }

            return _boardData->GetSpotColor( x, y );
        }

        void RotatePiece()
        {
            if( _currentPiece && _currentPiece->Rotate() )
            {
                Update();
            }
        }

        void MovePiece( Direction direction )
        {
            if( _currentPiece && _currentPiece->Move( direction ) )
            {
                Update();
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

            Update();
        }

        void TimerDrop()
        {
            if( !_currentPiece )
            {
                CreateNewPiece();

                Update();
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

            Update();
        }

    private:
        void Update()
        {
            _redrawFunc();
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

        std::function<void()> _redrawFunc;

        std::shared_ptr<ITetrisRawBoardData> _boardData;
        std::shared_ptr<ITetrisPieceProvider> _tetrisPieceProvider;

        std::unique_ptr<ITetrisPiece> _currentPiece;
        bool _gameInProgress = true;
    };
}

TetrisBoardData::TetrisBoardData( std::shared_ptr<ITetrisPieceProvider> tetrisPieceProvider,
                                 std::function<void()> redrawFunc )
{
    _impl.reset( new TetrisBoardDataImpl( tetrisPieceProvider, redrawFunc));
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

SpotColor TetrisBoardData::GetSpotColor( int x, int y ) const
{
    return _impl->GetSpotColor( x, y );
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
