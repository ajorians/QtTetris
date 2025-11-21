#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "TetrisBoardWidget.h"

struct MainWindowImpl{

    MainWindowImpl( MainWindow* parent )
    {
        _tetrisBoardWidget = new TetrisBoardWidget( parent );
        parent->setCentralWidget( _tetrisBoardWidget );
    }

private:
    TetrisBoardWidget* _tetrisBoardWidget;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _impl.reset( new MainWindowImpl( this ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}
