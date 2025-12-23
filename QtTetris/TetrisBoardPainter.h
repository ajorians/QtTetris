#ifndef TETRISBOARDPAINTER_H
#define TETRISBOARDPAINTER_H

#include <memory>

namespace TetrisLib
{
class ITetrisBoardData;
}

class QWidget;
struct TetrisBoardPainterImpl;

class TetrisBoardPainter
{
public:
    explicit TetrisBoardPainter( std::shared_ptr<const TetrisLib::ITetrisBoardData> tetrisBoardData );
    ~TetrisBoardPainter();

    void PaintToWidget(QWidget* widget);

private:
    std::unique_ptr<TetrisBoardPainterImpl> _impl;
};

#endif // TETRISBOARDPAINTER_H
