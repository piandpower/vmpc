#include <gui/geom/Crossings_NonZero.hpp>

#include <VecUtil.hpp>
#include <math/Math.hpp>

using namespace moduru::gui::geom;
using namespace std;

Crossings_NonZero::Crossings_NonZero(double xlo, double ylo, double xhi, double yhi) : Crossings(xlo, ylo, xhi, yhi)
{
	crosscounts = vector<int>(yranges.size() / 2);
}

bool Crossings_NonZero::covers(double ystart, double yend)
{
    int i = 0;
    while (i < limit) {
        auto ylo = yranges[i++];
        auto yhi = yranges[i++];
        if(ystart >= yhi) {
            continue;
        }
        if(ystart < ylo) {
            return false;
        }
        if(yend <= yhi) {
            return true;
        }
        ystart = yhi;
    }
    return (ystart >= yend);
}

void Crossings_NonZero::remove(int cur)
{
	limit -= 2;
	auto rem = limit - cur;
	if (rem > 0) {
		moduru::VecUtil::VecCopy(&yranges, cur + 2, &yranges, cur, rem);
		moduru::VecUtil::VecCopy(&crosscounts, cur / 2 + 1, &crosscounts, cur / 2, rem / 2);
	}
}

void Crossings_NonZero::insert(int cur, double lo, double hi, int dir)
{
    auto rem = limit - cur;
    auto oldranges = super::yranges;
    auto oldcounts = crosscounts;
    if(limit >= yranges.size()) {
        yranges = vector<double>(limit + 10);
        moduru::VecUtil::VecCopy(&oldranges, 0, &yranges, 0, cur);
        crosscounts = vector<int>((limit + 10) / 2);
        moduru::VecUtil::VecCopy(&oldcounts, 0, &crosscounts, 0, cur / 2);
    }
    if(rem > 0) {
        moduru::VecUtil::VecCopy(&oldranges, cur, &yranges, cur + 2, rem);
        moduru::VecUtil::VecCopy(&oldcounts, cur / 2, &crosscounts, cur / 2 + 1, rem / 2);
    }
    yranges[cur + 0] = lo;
    yranges[cur + 1] = hi;
    crosscounts[cur / 2] = dir;
    limit += 2;
}

void Crossings_NonZero::record(double ystart, double yend, int direction)
{
    if(ystart >= yend) {
        return;
    }
    auto cur = 0;
    while (cur < limit && ystart > yranges[cur + 1]) {
        cur += 2;
    }
    if(cur < limit) {
        auto rdir = crosscounts[cur / 2];
        auto yrlo = yranges[cur + 0];
        auto yrhi = yranges[cur + 1];
        if(yrhi == ystart && rdir == direction) {
            if(cur + 2 == limit) {
                yranges[cur + 1] = yend;
                return;
            }
            remove(cur);
            ystart = yrlo;
            rdir = crosscounts[cur / 2];
            yrlo = yranges[cur + 0];
            yrhi = yranges[cur + 1];
        }
        if(yend < yrlo) {
            insert(cur, ystart, yend, direction);
            return;
        }
        if(yend == yrlo && rdir == direction) {
            yranges[cur] = ystart;
            return;
        }
        if(ystart < yrlo) {
            insert(cur, ystart, yrlo, direction);
            cur += 2;
            ystart = yrlo;
        } else if(yrlo < ystart) {
            insert(cur, yrlo, ystart, rdir);
            cur += 2;
            yrlo = ystart;
        }
        auto newdir = rdir + direction;
        auto newend = moduru::math::Math::min(yend, yrhi);
        if(newdir == 0) {
            remove(cur);
        } else {
            crosscounts[cur / 2] = newdir;
            yranges[cur++] = ystart;
            yranges[cur++] = newend;
        }
        ystart = yrlo = newend;
        if(yrlo < yrhi) {
            insert(cur, yrlo, yrhi, rdir);
        }
    }
    if(ystart < yend) {
        insert(cur, ystart, yend, direction);
    }
}
