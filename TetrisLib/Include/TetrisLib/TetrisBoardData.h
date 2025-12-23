#ifndef TETRISBOARDDATA_H
#define TETRISBOARDDATA_H

#include "ITetrisBoardData.h"

#include <functional>
#include <memory>

namespace TetrisLib
{
    class ITetrisPieceProvider;
    struct TetrisBoardDataImpl;

   class TetrisBoardData : public ITetrisBoardData
   {
   public:
       TetrisBoardData( std::shared_ptr<ITetrisPieceProvider> tetrisPieceProvider,
                       std::function<void()> redrawFunc );

       void Reset() override;

       bool IsGameInProgress() const override;

       int GetWidth() const override;
       int GetHeight() const override;

       SpotColor GetSpotColor( int x, int y ) const override;

       void RotatePiece() override;
       void MovePiece( Direction direction ) override;
       void ZipPieceDown() override;

       void TimerDrop() override;

   private:
       std::unique_ptr<TetrisBoardDataImpl> _impl;
   };
}

#endif // TETRISBOARDDATA_H
