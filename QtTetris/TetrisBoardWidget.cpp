#include "TetrisBoardWidget.h"

#include "TetrisBoardPainter.h"

#include <TetrisLib/ITetrisBoardObserver.h>
#include <TetrisLib/TetrisBoardData.h>
#include <TetrisLib/TetrisPieceProvider.h>
#include <TetrisLib/TetrisRawBoardData.h>

#include <TetrisLib/Action.h>
#include <TetrisLib/Direction.h>

#include <TetrisAILib/TetrisAI.h>

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

#include <chrono>

using namespace std::chrono_literals;

struct TetrisBoardWidgetImpl : public TetrisLib::ITetrisBoardObserver
{
    TetrisBoardWidgetImpl( QWidget* parent, std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData  )
        : _parent( parent )
        , _tetrisBoardData( tetrisBoardData)
        , _tetrisBoardPainter( _tetrisBoardData )
        , _tetrisAI( _tetrisBoardData )
    {
        _tetrisBoardData->AddObserver( this );

        _tetrisPieceProvider.reset( new TetrisLib::TetrisPieceProvider( _tetrisBoardData->GetWidth(), _tetrisBoardData->GetHeight() ) );

        _moveTimer = new QTimer(_parent);
        QObject::connect(_moveTimer, &QTimer::timeout, [this]()
                         {
            _tetrisBoardData->TimerDrop();

            //Check if needs new piece
            if ( _tetrisBoardData->IsGameInProgress() && _tetrisBoardData->GetCurrentPiece() == nullptr )
            {
                auto nextPiece = _tetrisPieceProvider->GetNextPiece();
                _tetrisBoardData->SetCurrentPiece( nextPiece );
            }

            std::optional<TetrisLib::Action> move = _tetrisAI.MakeMove();
            if( move.has_value() )
            {
                _tetrisBoardData->PerformAction( move.value() );
            }

            _parent->update();
        });

        _moveTimer->start( 1s );
    }

    ~TetrisBoardWidgetImpl()
    {
        _tetrisBoardData->RemoveObserver( this );
    }

    //TetrisLib::ITetrisBoardObserver
    void ObserverUpdate() override
    {
        _parent->update();
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
    std::shared_ptr<TetrisLib::ITetrisPieceProvider> _tetrisPieceProvider;
    std::shared_ptr<TetrisLib::ITetrisBoardData> _tetrisBoardData;
    QTimer* _moveTimer;
    TetrisBoardPainter _tetrisBoardPainter;

    TetrisAILib::TetrisAI _tetrisAI;
};

TetrisBoardWidget::TetrisBoardWidget(QWidget *parent)
    : QWidget{parent}
{
    setFocusPolicy(Qt::StrongFocus);

    std::shared_ptr<TetrisLib::ITetrisRawBoardData> rawBoardData( new TetrisLib::TetrisRawBoardData( 10, 20 ) );
    std::shared_ptr<TetrisLib::ITetrisBoardData> tetrisBoardData( new TetrisLib::TetrisBoardData( rawBoardData ) );

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
