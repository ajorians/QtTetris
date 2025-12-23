#ifndef ITETRISBOARDOBSERVER_H
#define ITETRISBOARDOBSERVER_H

namespace TetrisLib
{
    class ITetrisBoardObserver
    {
    public:
        virtual ~ITetrisBoardObserver(){}

        virtual void ObserverUpdate() = 0;
    };
}

#endif // ITETRISBOARDOBSERVER_H
