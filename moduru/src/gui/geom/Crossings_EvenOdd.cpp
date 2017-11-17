#include <gui/geom/Crossings_EvenOdd.hpp>

#include <VecUtil.hpp>

using namespace moduru::gui::geom;
using namespace std;

Crossings_EvenOdd::Crossings_EvenOdd(double xlo, double ylo, double xhi, double yhi) : Crossings(xlo, ylo, xhi, yhi)
{
}

bool Crossings_EvenOdd::covers(double ystart, double yend)
{
    return (limit == 2 && yranges[0] <= ystart && yranges[1] >= yend);
}

void Crossings_EvenOdd::record(double ystart, double yend, int direction)
{
    if(ystart >= yend) {
        return;
    }
    auto from = 0;
    while (from < limit && ystart > yranges[from + 1]) {
        from += 2;
    }
    auto to = from;
    while (from < limit) {
        auto yrlo = yranges[from++];
        auto yrhi = yranges[from++];
        if(yend < yrlo) {
            yranges[to++] = ystart;
            yranges[to++] = yend;
            ystart = yrlo;
            yend = yrhi;
            continue;
        }
        double yll, ylh, yhl, yhh;
        if(ystart < yrlo) {
            yll = ystart;
            ylh = yrlo;
        } else {
            yll = yrlo;
            ylh = ystart;
        }
        if(yend < yrhi) {
            yhl = yend;
            yhh = yrhi;
        } else {
            yhl = yrhi;
            yhh = yend;
        }
        if(ylh == yhl) {
            ystart = yll;
            yend = yhh;
        } else {
            if(ylh > yhl) {
                ystart = yhl;
                yhl = ylh;
                ylh = ystart;
            }
            if(yll != ylh) {
                yranges[to++] = yll;
                yranges[to++] = ylh;
            }
            ystart = yhl;
            yend = yhh;
        }
        if(ystart >= yend) {
            break;
        }
    }
    if(to < from && from < limit) {
        moduru::VecUtil::VecCopy(&yranges, from, &yranges, to, limit - from);
    }
    to += (limit - from);
    if(ystart < yend) {
		if (to >= yranges.size()) {
			auto newranges = vector<double>(to + 10);
			moduru::VecUtil::VecCopy(&yranges, 0, &newranges, 0, to);
			yranges = newranges;
		}
        yranges[to++] = ystart;
        yranges[to++] = yend;
    }
    limit = to;
}
