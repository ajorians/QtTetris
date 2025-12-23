#include "TetrisBoardWidget.h"

#include "TetrisBoardPainter.h"

#include <TetrisLib/TetrisBoardData.h>
#include <TetrisLib/TetrisPieceProvider.h>

#include <TetrisLib/Direction.h>

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
        , _tetrisBoardPainter( _tetrisBoardData )
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
        _tetrisBoardPainter.PaintToWidget( _parent );
    }

private:

    QWidget* _parent;
    std::shared_ptr<TetrisLib::ITetrisBoardData> _tetrisBoardData;
    QTimer* _moveTimer;
    TetrisBoardPainter _tetrisBoardPainter;
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
