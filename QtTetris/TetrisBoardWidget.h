#ifndef TETRISBOARDWIDGET_H
#define TETRISBOARDWIDGET_H

#include <QWidget>

#include <memory>

struct TetrisBoardWidgetImpl;

class TetrisBoardWidget : public QWidget
{
public:
    explicit TetrisBoardWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    std::unique_ptr<TetrisBoardWidgetImpl> _impl;
};

#endif // TETRISBOARDWIDGET_H
