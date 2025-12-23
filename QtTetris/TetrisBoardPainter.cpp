#include "TetrisBoardPainter.h"

#include <TetrisLib/TetrisBoardData.h>
#include <TetrisLib/TetrisPieceProvider.h>

#include <TetrisLib/Direction.h>
#include <TetrisLib/SpotColor.h>

#include <QtWidgets>

struct TetrisBoardPainterImpl
{
    TetrisBoardPainterImpl( std::shared_ptr<const TetrisLib::ITetrisBoardData> tetrisBoardData )
        : _tetrisBoardData( std::move( tetrisBoardData ) )
    {
    }

    void PaintToWidget(QWidget* widget)
    {
        QPainter painter(widget);

        painter.fillRect( widget->rect(), QBrush( Qt::black, Qt::SolidPattern));

        const auto[ pieceDrawWidth, pieceDrawHeight ] = GetPieceDrawSize( widget );

        painter.setRenderHint(QPainter::Antialiasing); // Enable anti-aliasing for smoother lines

        const int boardWidth = _tetrisBoardData->GetWidth();
        const int boardHeight = _tetrisBoardData->GetHeight();

        const int widgetWidth = widget->width();
        const int widgetHeight = widget->height();

        int drawnWidth = boardWidth * pieceDrawWidth;
        int left = ( widgetWidth - drawnWidth ) / 2;

        int drawnHeight = boardHeight * pieceDrawHeight;
        int top = ( widgetHeight - drawnHeight ) / 2;

        const int gapSpacing = 3;

        for( int x=0; x<boardWidth; x++)
        {
            for( int y=0; y<boardHeight; y++)
            {
                auto pieceColor = _tetrisBoardData->GetSpotColor( x, y );

                if( pieceColor == TetrisLib::SpotColor::Nothing )
                    continue;

                QColor colorPen = Qt::blue;
                QColor colorBrush = Qt::green;

                QPen pen = GetPen( pieceColor );
                QBrush brush = GetBrush( pieceColor );

                painter.setPen(pen);
                painter.setBrush(brush);

                int posFromBottom = boardHeight - 1/*index*/ - y;
                QRect rectPiece( left + x* pieceDrawWidth, top + posFromBottom* pieceDrawHeight, pieceDrawWidth, pieceDrawHeight);
                rectPiece.adjust( gapSpacing, gapSpacing, -gapSpacing, -gapSpacing );
                painter.drawRect( rectPiece );
            }
        }
    }

private:
    std::pair<int, int> GetPieceDrawSize( QWidget* widget ) const
    {
        int boardWidth = _tetrisBoardData->GetWidth();
        int widgetWidth = widget->width();

        int pixelsPerPieceWidth = static_cast<int>(
            widgetWidth / static_cast<double>( boardWidth ) );

        int boardHeight = _tetrisBoardData->GetHeight();
        int widgetHeight = widget->height();

        int pixelsPerPieceHeight = static_cast<int>(
            widgetHeight / static_cast<double>( boardHeight ) );

        return {pixelsPerPieceWidth, pixelsPerPieceHeight};
    }

    QPen GetPen( const TetrisLib::SpotColor& spotColor ) const
    {
        QColor colorPen;
        switch( spotColor )
        {
        default:
            assert( false );
        case TetrisLib::SpotColor::Red:
            colorPen = Qt::red;
        break;
        case TetrisLib::SpotColor::Yellow:
            colorPen = Qt::yellow;
        break;
        case TetrisLib::SpotColor::Purple:
            colorPen = Qt::darkMagenta;
        break;
        case TetrisLib::SpotColor::LiteBlue:
            colorPen = Qt::cyan;
        break;
        case TetrisLib::SpotColor::DarkBlue:
            colorPen = Qt::darkBlue;
        break;
        case TetrisLib::SpotColor::Green:
            colorPen = Qt::green;
        break;
        case TetrisLib::SpotColor::Gray:
            colorPen = Qt::gray;
        break;
        }

        return QPen(colorPen, 2, Qt::SolidLine);
    }

    QBrush GetBrush( const TetrisLib::SpotColor& spotColor ) const
    {
        QColor colorBrush;
        switch( spotColor )
        {
        default:
            assert( false );
        case TetrisLib::SpotColor::Red:
            colorBrush = Qt::red;
            break;
        case TetrisLib::SpotColor::Yellow:
            colorBrush = Qt::yellow;
            break;
        case TetrisLib::SpotColor::Purple:
            colorBrush = Qt::darkMagenta;
            break;
        case TetrisLib::SpotColor::LiteBlue:
            colorBrush = Qt::cyan;
            break;
        case TetrisLib::SpotColor::DarkBlue:
            colorBrush = Qt::darkBlue;
            break;
        case TetrisLib::SpotColor::Green:
            colorBrush = Qt::green;
            break;
        case TetrisLib::SpotColor::Gray:
            colorBrush = Qt::gray;
            break;
        }

        return QBrush(colorBrush, Qt::SolidPattern);
    }

    std::shared_ptr<const TetrisLib::ITetrisBoardData> _tetrisBoardData;
};

TetrisBoardPainter::TetrisBoardPainter( std::shared_ptr<const TetrisLib::ITetrisBoardData> tetrisBoardData)
{
    _impl.reset( new TetrisBoardPainterImpl( tetrisBoardData ) );
}

TetrisBoardPainter::~TetrisBoardPainter()
{
}

void TetrisBoardPainter::PaintToWidget( QWidget* widget )
{
    _impl->PaintToWidget( widget );
}
