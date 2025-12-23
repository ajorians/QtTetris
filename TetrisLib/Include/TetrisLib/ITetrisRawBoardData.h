#ifndef ITETRISRAWBOARDDATA_H
#define ITETRISRAWBOARDDATA_H

#include <memory>

namespace TetrisLib
{
    enum class SpotColor;

    class ITetrisRawBoardData
    {
        public:
        virtual ~ITetrisRawBoardData() {}

        virtual std::shared_ptr<ITetrisRawBoardData> Clone() const = 0;

        virtual void Reset() = 0;

        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;

        virtual SpotColor GetSpotColor( int x, int y ) const = 0;
        virtual void SetSpotColor( int x, int y, SpotColor spotColor ) = 0;
    };
}

#endif // ITETRISRAWBOARDDATA_H
