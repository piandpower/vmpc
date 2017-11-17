#ifndef BASICSTRUCTS
#define BASICSTRUCTS

//#include "Containers.h"
//#include "IPlugOSDetect.h"

//#include "../swell/swell.h"
//#include "../lice/lice_text.h"

#include <algorithm>

struct IColor
{
  int A, R, G, B;
  IColor(int a = 255, int r = 0, int g = 0, int b = 0) : A(a), R(r), G(g), B(b) {}
  bool operator==(const IColor& rhs) { return (rhs.A == A && rhs.R == R && rhs.G == G && rhs.B == B); }
  bool operator!=(const IColor& rhs) { return !operator==(rhs); }
  bool Empty() const { return A == 0 && R == 0 && G == 0 && B == 0; }
  void Clamp() { A = std::min(A, 255); R = std::min(R, 255); G = std::min(G, 255); B = std::min(B, 255); }
};

struct IRECT
{
  int L, T, R, B;

  IRECT() { L = T = R = B = 0; }
  IRECT(int l, int t, int r, int b) : L(l), R(r), T(t), B(b) {}
  //IRECT(int x, int y, IBitmap* pBitmap) : L(x), T(y), R(x + pBitmap->W), B(y + pBitmap->H / pBitmap->N) {}

  bool Empty() const
  {
    return (L == 0 && T == 0 && R == 0 && B == 0);
  }

  void Clear()
  {
    L = T = R = B = 0;
  }

  bool operator==(const IRECT& rhs) const
  {
    return (L == rhs.L && T == rhs.T && R == rhs.R && B == rhs.B);
  }

  bool operator!=(const IRECT& rhs) const
  {
    return !(*this == rhs);
  }

  inline int W() const { return R - L; }
  inline int H() const { return B - T; }
  inline float MW() const { return 0.5f * (float) (L + R); }
  inline float MH() const { return 0.5f * (float) (T + B); }

  inline IRECT Union(IRECT* pRHS)
  {
    if (Empty()) { return *pRHS; }
    if (pRHS->Empty()) { return *this; }
    return IRECT(std::min(L, pRHS->L), std::min(T, pRHS->T), std::max(R, pRHS->R), std::max(B, pRHS->B));
  }

  inline IRECT Intersect(IRECT* pRHS)
  {
    if (Intersects(pRHS))
    {
      return IRECT(std::max(L, pRHS->L), std::max(T, pRHS->T), std::min(R, pRHS->R), std::min(B, pRHS->B));
    }
    return IRECT();
  }

  inline bool Intersects(IRECT* pRHS)
  {
    return (!Empty() && !pRHS->Empty() && R >= pRHS->L && L < pRHS->R && B >= pRHS->T && T < pRHS->B);
  }

  inline bool Contains(IRECT* pRHS)
  {
    return (!Empty() && !pRHS->Empty() && pRHS->L >= L && pRHS->R <= R && pRHS->T >= T && pRHS->B <= B);
  }

  inline bool Contains(int x, int y)
  {
    return (!Empty() && x >= L && x < R && y >= T && y < B);
  }

  inline void Constrain(int* x, int* y)
  {
    if (*x < L)
    {
      *x = L;
    }
    else if (*x > R)
    {
      *x = R;
    }

    if (*y < T)
    {
      *y = T;
    }
    else if (*y > B)
    {
      *y = B;
    }
  }

  inline IRECT SubRectVertical(int numSlices, int sliceIdx)
  {
    float heightOfSubRect = (float(H()) / numSlices);
    int t = (int) heightOfSubRect * sliceIdx;

    return IRECT(L, T + t, R, T + t + heightOfSubRect);
  }

  inline IRECT SubRectHorizontal(int numSlices, int sliceIdx)
  {
    float widthOfSubRect = (float(W()) / numSlices);
    int l = widthOfSubRect * sliceIdx;

    return IRECT(L + l, T, L + l + widthOfSubRect, B);
  }
  
  inline IRECT GetPadded(int padding)
  {
    return IRECT(L-padding, T-padding, R+padding, B+padding);
  }
  
  inline IRECT GetPadded(int padL, int padT, int padR, int padB)
  {
    return IRECT(L+padL, T+padT, R+padR, B+padB);
  }
  
  inline IRECT GetHPadded(int padding)
  {
    return IRECT(L-padding, T, R+padding, B);
  }

  inline IRECT GetVPadded(int padding)
  {
    return IRECT(L, T-padding, R, B+padding);
  }
  
  void Clank(IRECT* pRHS)
  {
    if (L < pRHS->L)
    {
      R = std::min(pRHS->R - 1, R + pRHS->L - L);
      L = pRHS->L;
    }
    if (T < pRHS->T)
    {
      B = std::min(pRHS->B - 1, B + pRHS->T - T);
      T = pRHS->T;
    }
    if (R >= pRHS->R)
    {
      L = std::max(pRHS->L, L - (R - pRHS->R + 1));
      R = pRHS->R - 1;
    }
    if (B >= pRHS->B)
    {
      T = std::max(pRHS->T, T - (B - pRHS->B + 1));
      B = pRHS->B - 1;
    }
  }
};

#endif