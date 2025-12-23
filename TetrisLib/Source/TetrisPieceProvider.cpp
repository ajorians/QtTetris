#include "TetrisLib/TetrisPieceProvider.h"

#include "TetrisLib/ITetrisRawBoardData.h"
#include "TetrisLib/SpotColor.h"
#include "TetrisLib/TetrisPiece.h"

#include <cassert>
#include <utility>
#include <vector>

using namespace TetrisLib;

TetrisPieceProvider::TetrisPieceProvider()
{
}

std::unique_ptr<ITetrisPiece> TetrisPieceProvider::GetNextPiece( std::shared_ptr<ITetrisRawBoardData> tetrisRawBoardData )
{
    SpotColor spotColor = static_cast<SpotColor>( rand() % 7 + 1 );

    std::vector<std::pair<int, int>> relativePieces;

    if( spotColor == SpotColor::Red )//Staight
    {
        relativePieces.push_back( {-1, 0} );
        relativePieces.push_back( {0, 0} );
        relativePieces.push_back( {1, 0} );
        relativePieces.push_back( {2, 0} );
    }
    else if( spotColor == SpotColor::Yellow )//3 + bend
    {
        relativePieces.push_back( {-1, 0} );
        relativePieces.push_back( {0, 0} );
        relativePieces.push_back( {1, 0} );
        relativePieces.push_back( {1, 1} );
    }
    else if( spotColor == SpotColor::Purple )//3 + bend
    {
        relativePieces.push_back( {-1, 1} );
        relativePieces.push_back( {-1, 0} );
        relativePieces.push_back( {0, 0} );
        relativePieces.push_back( {1, 0} );
    }
    else if( spotColor == SpotColor::LiteBlue )//Square
    {
        relativePieces.push_back( {0, 0} );
        relativePieces.push_back( {1, 0} );
        relativePieces.push_back( {0, 1} );
        relativePieces.push_back( {1, 1} );
    }
    else if( spotColor == SpotColor::DarkBlue )//Bent
    {
        relativePieces.push_back( {-1, 0} );
        relativePieces.push_back( {0, 0} );
        relativePieces.push_back( {0, 1} );
        relativePieces.push_back( {1, 1} );
    }
    else if( spotColor == SpotColor::Green )//Bent
    {
        relativePieces.push_back( {-1, 1} );
        relativePieces.push_back( {0, 1} );
        relativePieces.push_back( {0, 0} );
        relativePieces.push_back( {1, 0} );
    }
    else if( spotColor == SpotColor::Gray )//T-shape
    {
        relativePieces.push_back( {0, 1} );
        relativePieces.push_back( {-1, 0} );
        relativePieces.push_back( {0, 0} );
        relativePieces.push_back( {1, 0} );
    }
    else
    {
        assert( false );//Problem :(
    }

    int x = tetrisRawBoardData->GetWidth() / 2;
    int y = tetrisRawBoardData->GetHeight() - 1/*index*/;

    std::unique_ptr<TetrisPiece> result( new TetrisPiece( tetrisRawBoardData, spotColor, relativePieces, x, y ) );

    if( !result->CanBePlaced() )
    {
        result->ApplyToBoard();
        return nullptr;
    }

    return result;
}
