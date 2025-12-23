#include "TetrisLib/TetrisPieceProvider.h"

#include "TetrisLib/ITetrisRawBoardData.h"
#include "TetrisLib/SpotColor.h"
#include "TetrisLib/TetrisPiece.h"

#include <cassert>
#include <utility>
#include <vector>

using namespace TetrisLib;

TetrisPieceProvider::TetrisPieceProvider( int width, int height )
    : _width( width )
    , _height( height )
{
}

std::shared_ptr<ITetrisPiece> TetrisPieceProvider::GetNextPiece()
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

    int x = _width / 2;
    int y = _height - 1/*index*/;

    std::shared_ptr<TetrisPiece> result( new TetrisPiece( spotColor, relativePieces, x, y ) );

    return result;
}
