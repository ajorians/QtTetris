#include "TetrisBoardWidget.h"

#include <TetrisLib/TetrisBoardData.h>
#include <TetrisLib/TetrisPieceProvider.h>

#include <TetrisLib/Direction.h>
#include <TetrisLib/SpotColor.h>

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

#include <chrono>

using namespace std::chrono_literals;

struct TetrisBoardWidgetImpl
{
    TetrisBoardWidgetImpl( QWidget* parent, std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData  )
        : _parent( parent )
        , _tetrisBoardData( tetrisBoardData)
    {
        _moveTimer = new QTimer(_parent);
        QObject::connect(_moveTimer, &QTimer::timeout, [this]()
                         {
            _tetrisBoardData->TimerDrop();

            _parent->update();
        });

        _moveTimer->start( 1s );
    }

    void KeyPress( QKeyEvent *event )
    {
        if( event->key() == Qt::Key_Left )
        {
            _tetrisBoardData->MovePiece( TetrisLib::Direction::Left );
        }
        else if( event->key() == Qt::Key_Right )
        {
            _tetrisBoardData->MovePiece( TetrisLib::Direction::Right );
        }
        else if( event->key() == Qt::Key_Up )
        {
            _tetrisBoardData->RotatePiece();
        }
        else if( event->key() == Qt::Key_Space || event->key() == Qt::Key_Down )
        {
            _tetrisBoardData->ZipPieceDown();
        }
        else if( event->key() == Qt::Key_F2 )
        {
            _tetrisBoardData->Reset();
        }
    }

    void Paint( QPaintEvent* event )
    {
        QPainter painter(_parent);

        painter.fillRect( _parent->rect(), QBrush( Qt::black, Qt::SolidPattern));

        const auto[ pieceDrawWidth, pieceDrawHeight ] = GetPieceDrawSize();

        painter.setRenderHint(QPainter::Antialiasing); // Enable anti-aliasing for smoother lines

        const int boardWidth = _tetrisBoardData->GetWidth();
        const int boardHeight = _tetrisBoardData->GetHeight();

        const int widgetWidth = _parent->width();
        const int widgetHeight = _parent->height();

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
    std::pair<int, int> GetPieceDrawSize() const
    {
        int boardWidth = _tetrisBoardData->GetWidth();
        int widgetWidth = _parent->width();

        int pixelsPerPieceWidth = static_cast<int>(
            widgetWidth / static_cast<double>( boardWidth ) );

        int boardHeight = _tetrisBoardData->GetHeight();
        int widgetHeight = _parent->height();

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

    QWidget* _parent;
    std::shared_ptr<TetrisLib::ITetrisBoardData> _tetrisBoardData;
    QTimer* _moveTimer;
};

TetrisBoardWidget::TetrisBoardWidget(QWidget *parent)
    : QWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);

    auto redrawFunc = [this]()
    {
        this->update();
    };

    std::shared_ptr<TetrisLib::ITetrisPieceProvider> tetrisPieceProvider( new TetrisLib::TetrisPieceProvider() );

    std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData( new TetrisLib::TetrisBoardData( tetrisPieceProvider, redrawFunc ) );

    _impl.reset( new TetrisBoardWidgetImpl( this, tetrisBoardData ) );
}

void TetrisBoardWidget::keyPressEvent(QKeyEvent *event)
{
    _impl->KeyPress( event );
}

void TetrisBoardWidget::paintEvent(QPaintEvent *event)
{
    _impl->Paint( event );
}
