#include "Stdafx.h"
#include "wg_cmd.h"

#include "DrawDataObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBL_EPSILON 1E-10

inline void xGetCross(T_POINT& vc, const T_POINT& v1, const T_POINT& v2)
{
	vc.x = v1.y * v2.x - v1.x * v2.y;
	vc.y = v1.x * v2.y - v1.y * v2.x;
}

inline void xGetTriVp(const T_POINT& ap, const T_POINT& bp, const T_POINT& cp, T_POINT& vp)
{
	T_POINT ab(bp.x - ap.x, bp.y - ap.y);
	T_POINT bc(cp.x - bp.x, cp.y - bp.y);
	xGetCross(vp, ab, bc);
}

inline double xGetDot(const T_POINT& p1, const T_POINT& p2)
{
	return (p1.x * p2.x + p1.y * p2.y);
}

inline double xGetCosV(const T_POINT& ba, const T_POINT& bc)
{
	double	dot = xGetDot(ba, bc);
	double	len = ba.Length() * bc.Length();
	return (len < DBL_EPSILON ? -1.0 : dot / len); 
}

inline double xGetCos(const T_POINT& a, const T_POINT& b, const T_POINT& c)
{
	T_POINT ba(a.x-b.x, a.y-b.y);
	T_POINT bc(c.x-b.x, c.y-b.y);
	return xGetCosV(ba, bc);
}

inline BOOL xIsPointInTri(const T_POINT& ap, const T_POINT& bp, const T_POINT& cp, const T_POINT& pp)
{
	T_POINT dirABC, dirABP, dirBCP, dirCAP;
	xGetTriVp(ap, bp, cp, dirABC);
	xGetTriVp(ap, bp, pp, dirABP);
	xGetTriVp(bp, cp, pp, dirBCP);
	xGetTriVp(cp, ap, pp, dirCAP);
	if((xGetDot(dirABP, dirABC) > 0.0) && (xGetDot(dirBCP, dirABC) > 0.0) && (xGetDot(dirCAP, dirABC) > 0.0)) return TRUE;
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////
// 1. T_POINT

inline void T_POINT::fDivide(double tv, T_POINT& rp)
{
	if(fabs(tv) < DBL_EPSILON)
	{
		rp = *this;
	}
	else
	{
		rp.x = x / tv;
		rp.y = y / tv;
	}
}

inline void T_POINT::pDivide(const T_POINT& tp, T_POINT& rp)
{
	rp = *this;
	if(fabs(tp.x) > DBL_EPSILON) rp.x /= tp.x;
	if(fabs(tp.y) > DBL_EPSILON) rp.y /= tp.y;
}

inline void T_POINT::fEqualDivide(double tv)
{
	if(fabs(tv) > DBL_EPSILON)
	{
		x /= tv;
		y /= tv;
	}
}

inline void T_POINT::pEqualDivide(const T_POINT& tp)
{
	if(fabs(tp.x) > DBL_EPSILON) x /= tp.x;
	if(fabs(tp.y) > DBL_EPSILON) y /= tp.y;
}

inline BOOL T_POINT::IsEqual(const T_POINT& tp)
{
	if(fabs(x - tp.x) > DBL_EPSILON) return FALSE;
	if(fabs(y - tp.y) > DBL_EPSILON) return FALSE;

	return TRUE;
}

inline double T_POINT::GetLength(T_POINT& pnt)
{
	T_POINT vec;

	vec.x = pnt.x - x;
	vec.y = pnt.y - y;

	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

inline double T_POINT::GetLength(T_POINT& pnt, T_POINT& dir)
{
	T_POINT vec;
	double len;

	vec.x = pnt.x - x;
	vec.y = pnt.y - y;

	len = sqrt(vec.x * vec.x + vec.y * vec.y);

	if(len > 0.0)
	{
		dir.x = vec.x / len;
		dir.y = vec.y / len;
	}

	return len;
}

inline double T_POINT::GetLineProperty(const T_POINT& p2, double& sinv, double& cosv)
{
	double dis;  

	dis = sqrt((p2.y - y) * (p2.y - y) + (p2.x - x) * (p2.x - x));  

	if(dis < DBL_EPSILON) 
	{
		sinv = 0.0;  
		cosv = 1.0; 
		return dis; 
	}

	sinv = fabs(p2.y - y) < DBL_EPSILON ? 0.0 : (p2.y - y) / dis;
	cosv = fabs(p2.x - x) < DBL_EPSILON ? 0.0 : (p2.x - x) / dis;

	return dis;
}


/////////////////////////////////////////////////////////////////////////////
// 2. T_RECT

inline void T_RECT::fDivide(double tv, T_RECT& rr)
{
	if (fabs(tv) < DBL_EPSILON)
	{
		rr.left	= left;
		rr.top = top;
		rr.right = right;
		rr.bottom = bottom;
	}
	else
	{
		rr.left	= left / tv;
		rr.top = top / tv;
		rr.right = right / tv;
		rr.bottom = bottom / tv;
	}
}

inline void T_RECT::rDivide(const T_RECT& tr, T_RECT& rr)
{
	rr.left	= left;
	rr.top = top;
	rr.right = right;
	rr.bottom = bottom;
	if (fabs(tr.left) > DBL_EPSILON)	rr.left	/= tr.left;
	if (fabs(tr.top) > DBL_EPSILON)	rr.top /= tr.top;
	if (fabs(tr.right) > DBL_EPSILON)  rr.right /= tr.right;
	if (fabs(tr.bottom) > DBL_EPSILON) rr.bottom /= tr.bottom;
}

inline void T_RECT::fEqualDivide(double tv)
{
	if(fabs(tv) < DBL_EPSILON) return;
	left	/= tv;
	top		/= tv;
	right	/= tv;
	bottom	/= tv;
}

inline void T_RECT::rEqualDivide(const T_RECT& tr)
{
	if (fabs(tr.left) > DBL_EPSILON)   left /= tr.left;
	if (fabs(tr.top) > DBL_EPSILON)	   top	/= tr.top;
	if (fabs(tr.right) > DBL_EPSILON)  right /= tr.right;
	if (fabs(tr.bottom) > DBL_EPSILON) bottom /= tr.bottom;
}

inline BOOL T_RECT::IsEqual(const T_RECT& tr)
{
	if (fabs(left - tr.left) > ONE_EPSILON)	    return FALSE;
	if (fabs(top - tr.top) > ONE_EPSILON)		return FALSE;
	if (fabs(right - tr.right) > ONE_EPSILON)	return FALSE;
	if (fabs(bottom - tr.bottom) > ONE_EPSILON) return FALSE;
	return TRUE;
}

inline BOOL T_RECT::IsIn(const T_POINT& p)
{
	if (p.x - ONE_EPSILON < left)	return FALSE;
	if (p.x + ONE_EPSILON > right)	return FALSE;
	if (p.y - ONE_EPSILON < bottom)	return FALSE;
	if (p.y + ONE_EPSILON > top)	return FALSE;
	return TRUE;
}

inline BOOL T_RECT::IsOn(const T_POINT& p)
{
	BOOL OnRect = FALSE;

	if (fabs(p.x - left) < ONE_EPSILON || fabs(p.x - right) < ONE_EPSILON)
	{
		if (p.y - ONE_EPSILON < bottom && p.y + ONE_EPSILON < top) OnRect = TRUE;
	}
	else if (fabs(p.y - bottom) < ONE_EPSILON || fabs(p.y - top) < ONE_EPSILON)
	{
		if (p.x - ONE_EPSILON > left && p.y + ONE_EPSILON > right) OnRect = TRUE;
	}
	return OnRect;
}

inline BOOL T_RECT::IsCross(const T_RECT& tr)
{
	BOOL bx, by;
	if(fabs(left - tr.left) < ONE_EPSILON) bx = (right > left && tr.right > tr.left) ? TRUE : FALSE;
	else if(left < tr.left)   		       bx = right > tr.left ? TRUE : FALSE;
	else   								   bx = tr.right > left ? TRUE : FALSE;
	if(fabs(top - tr.top) < ONE_EPSILON)   by = (bottom < top && tr.bottom < tr.top) ? TRUE : FALSE;
	else if(top < tr.top)    	           by = tr.bottom < top ? TRUE : FALSE;
	else 	                               by = bottom < tr.top ? TRUE : FALSE;
	return bx * by;
}

inline BOOL T_RECT::IsInRect(const T_RECT& tr)
{
	BOOL bx = FALSE, by = FALSE;
	if(tr.left   > left   && tr.left   < right) bx = TRUE;
	if(tr.right  > left   && tr.right  < right) bx = TRUE; 
	if(tr.top    > bottom && tr.top    < top)   by = TRUE;
	if(tr.bottom > bottom && tr.bottom < top)   by = TRUE;
	return bx * by;
}

BOOL T_RECT::IsLineInRect(const T_POINT& p1, const T_POINT& p2, T_POINT cp[2])
{
	// 1. max. min test
	if(max(p1.x, p2.x) < left)	  return FALSE;
	if(min(p1.x, p2.x) > right)  return FALSE;
	if(max(p1.y, p2.y) < bottom) return FALSE;
	if(min(p1.y, p2.y) > top)	  return FALSE;

	int		n0, n1;
	double	rr;

	cp[0] = p1; 
	cp[1] = p2;

	// 2. cut by top_bottom line
	n0 = (cp[0].y < cp[1].y ? 0:1);
	n1 = (cp[0].y < cp[1].y ? 1:0);

	if((cp[0].y - bottom) * (cp[1].y - bottom) < 0.0)
	{
		rr = (cp[n0].y - bottom) / (cp[n0].y - cp[n1].y);
		DividePoint(cp[n0], cp[n1], rr, cp[n0]);
	}
	if((cp[0].y - top) * (cp[1].y - top) < 0.0)
	{
		rr = (cp[n1].y - top) / (cp[n1].y - cp[n0].y);
		DividePoint(cp[n1], cp[n0], rr, cp[n1]);
	}
	if(max(cp[0].x, cp[1].x) < left)	return FALSE;
	if(min(cp[0].x, cp[1].x) > right)	return FALSE;

	// 3. cut by left_right line
	n0 = (cp[0].x < cp[1].x ? 0 : 1);
	n1 = (cp[0].x < cp[1].x ? 1 : 0);

	if((cp[0].x - left) * (cp[1].x - left) < 0.0)
	{
		rr = (cp[n0].x - left) / (cp[n0].x - cp[n1].x);
		DividePoint(cp[n0], cp[n1], rr, cp[n0]);
	}
	if((cp[0].x - right) * (cp[1].x - right) < 0.0)
	{
		rr = (cp[n1].x - right) / (cp[n1].x - cp[n0].x);
		DividePoint(cp[n1], cp[n0], rr, cp[n1]);
	}

	return TRUE;
}

int T_RECT::IsTriInRect(T_POINT op[3], T_POINT cp[7])
{
	int i, j;
	// 1. max. min test
	if (max(op[0].x, max(op[1].x, op[2].x)) < left)	    return 0;
	if (min(op[0].x, min(op[1].x, op[2].x)) > right)	return 0;
	if (max(op[0].y, max(op[1].y, op[2].y)) < bottom)	return 0;
	if (min(op[0].y, min(op[1].y, op[2].y)) > top)	    return 0;

	// 2. seek position in rectangle
	int	pos[3];

	if(op[0].x < left)
	{	
		pos[0] = (op[0].y < bottom ? EN_RECT_LB : op[0].y > top ? EN_RECT_LT : EN_RECT_LC); 
	}
	else if(op[0].x > right)
	{	
		pos[0] = (op[0].y < bottom ? EN_RECT_RB : op[0].y > top ? EN_RECT_RT : EN_RECT_RC); 
	}
	else
	{
		pos[0] = (op[0].y < bottom ? EN_RECT_CB : op[0].y > top ? EN_RECT_CT : EN_RECT_CC); 
	}

	if(op[1].x < left)
	{
		pos[1] = (op[1].y < bottom ? EN_RECT_LB : op[1].y > top ? EN_RECT_LT : EN_RECT_LC); 
	}
	else if (op[1].x > right)
	{	
		pos[1]= (op[1].y < bottom ? EN_RECT_RB: op[1].y > top ? EN_RECT_RT : EN_RECT_RC); 
	}
	else
	{
		pos[1]= (op[1].y < bottom ? EN_RECT_CB: op[1].y > top ? EN_RECT_CT : EN_RECT_CC); 
	}

	if (op[2].x < left)
	{
		pos[2] = (op[2].y < bottom ? EN_RECT_LB : op[2].y > top ? EN_RECT_LT : EN_RECT_LC); 
	}
	else if(op[2].x > right)
	{
		pos[2] = (op[2].y < bottom ? EN_RECT_RB : op[2].y > top ? EN_RECT_RT : EN_RECT_RC); 
	}
	else
	{
		pos[2] = (op[2].y < bottom ? EN_RECT_CB : op[2].y > top ? EN_RECT_CT : EN_RECT_CC); 
	}

	// 3. three points in rectangle
	if(pos[0] == EN_RECT_CC && pos[1] == EN_RECT_CC && pos[2] == EN_RECT_CC)
	{
		cp[0] = op[0];	cp[1] = op[1];
		cp[2] = op[2];	return 3;
	}

	// 4. two points in rectangle
	int		n0 = -1;		// inside point
	int		n1 = -1;		// inside point
	int		n2 = -1;		// outside point
	int		num;
	double	rx, ry;
	double	sx, sy;

	if(pos[0] == EN_RECT_CC && pos[1] == EN_RECT_CC)		{ n0 = 0;  n1 = 1;  n2 = 2; }
	else if(pos[1] == EN_RECT_CC && pos[2] == EN_RECT_CC)	{ n0 = 1;  n1 = 2;  n2 = 0; }
	else if(pos[2] == EN_RECT_CC && pos[0] == EN_RECT_CC)	{ n0 = 2;  n1 = 0;  n2 = 1; }

	if(n0 >= 0)
	{
		num = 4;
		if(pos[n2] == EN_RECT_CT || pos[n2] == EN_RECT_CB)
		{
			sy = (pos[n2] == EN_RECT_CT ? top : bottom);
			ry = (sy - op[n1].y) / (op[n2].y - op[n1].y);
			cp[2].Set(op[n1].x + (op[n2].x - op[n1].x) * ry, sy);
			ry = (sy - op[n0].y) / (op[n2].y - op[n0].y);
			cp[3].Set(op[n0].x + (op[n2].x - op[n0].x) * ry, sy);
		}
		else
		{
			sx = (pos[n2] <= EN_RECT_LB ? left : right);
			rx = (sx - op[n1].x) / (op[n2].x - op[n1].x);
			cp[2].Set(sx, op[n1].y + (op[n2].y - op[n1].y) * rx);
			rx = (sx - op[n0].x) / (op[n2].x - op[n0].x);
			cp[3].Set(sx, op[n0].y + (op[n2].y - op[n0].y) * rx);

			if(cp[2].y > top && cp[3].y > top)				// EN_RECT_LT || EN_RECT_RT
			{
				ry = (top - op[n1].y) / (op[n2].y - op[n1].y);
				cp[2].Set(op[n1].y + (op[n2].y - op[n1].y) * ry, top);
				ry = (top - op[n0].y) / (op[n2].y - op[n0].y);
				cp[3].Set(op[n0].y + (op[n2].y - op[n0].y) * ry, top);
			}
			else if(cp[2].y > top || cp[3].y > top)		// EN_RECT_LT || EN_RECT_RT
			{
				if(cp[2].y > top)
				{
					cp[4] = cp[3];
					cp[3].Set(sx, top);
					ry = (top - op[n1].y) / (op[n2].y - op[n1].y);
					cp[2].Set(op[n1].y + (op[n2].y - op[n1].y) * ry, top);
				}
				else
				{
					cp[3].Set(sx, top);
					ry = (top - op[n0].y) / (op[n2].y - op[n0].y);
					cp[4].Set(op[n0].y + (op[n2].y - op[n0].y) * ry, top);
				}
				num= 5;
			}
			else if(cp[2].y < bottom && cp[3].y < bottom)	// EN_RECT_LB || EN_RECT_RB
			{
				ry = (bottom - op[n1].y) / (op[n2].y - op[n1].y);
				cp[2].Set(op[n1].y + (op[n2].y - op[n1].y) * ry, bottom);
				ry = (bottom - op[n0].y) / (op[n2].y - op[n0].y);
				cp[3].Set(op[n0].y + (op[n2].y - op[n0].y) * ry, bottom);
			}
			else if(cp[2].y < bottom || cp[3].y < bottom)	// EN_RECT_LB || EN_RECT_RB
			{
				if(cp[2].y < bottom)
				{
					cp[4] = cp[3];
					cp[3].Set(sx, bottom);
					ry = (bottom - op[n1].y) / (op[n2].y - op[n1].y);
					cp[2].Set(op[n1].y + (op[n2].y - op[n1].y) * ry, bottom);
				}
				else
				{
					cp[3].Set(sx, top);
					ry = (bottom - op[n0].y) / (op[n2].y - op[n0].y);
					cp[4].Set(op[n0].y + (op[n2].y - op[n0].y) * ry, bottom);
				}
				num = 5;
			}
		}
		cp[0] = op[n0];
		cp[1] = op[n1];

		return num;
	}

	// 5. one point in rectangle
	n0 = -1;		// inside point
	n1 = -1;		// outside point
	n2 = -1;		// outside point

	if(pos[0] == EN_RECT_CC)		{ n0 = 0;  n1 = 1;  n2 = 2; }
	else if(pos[1] == EN_RECT_CC)	{ n0 = 1;  n1 = 2;  n2 = 0; }
	else if(pos[2] == EN_RECT_CC)	{ n0 = 2;  n1 = 0;  n2 = 1; }

	if(n0 >= 0)
	{
		T_POINT	tp[6];

		tp[0] = op[n0];		tp[1] = op[n1];
		tp[2] = op[n0];		tp[3] = op[n2];
		tp[4] = op[n1];		tp[5] = op[n2];

		IsLineInRect(tp[0], tp[1], &cp[0]);		
		IsLineInRect(tp[2], tp[3], &cp[4]);		

		if(IsLineInRect(tp[4], tp[5], &cp[2]))	
		{
			cp[4] = cp[5];
			if(cp[3].GetLength(op[n1]) < cp[2].GetLength(op[n1]))
			{	
				tp[0] = cp[2];	
				cp[2] = cp[3];	
				cp[3] = tp[0]; 
			}

			num = 5;
			if(fabs(cp[1].x - cp[2].x) > ONE_EPSILON && fabs(cp[1].y - cp[2].y) > ONE_EPSILON)
			{
				num = 6;
				cp[5] = cp[4];
				cp[4] = cp[3];
				cp[3] = cp[2];
				cp[2].x = (op[n1].x < left ? left : right);
				cp[2].y = (op[n1].y < bottom ? bottom : top);
			}
			if(fabs(cp[3].x - cp[4].x) > ONE_EPSILON && fabs(cp[3].y - cp[4].y) > ONE_EPSILON)
			{
				num++;
				if(num == 6)
				{
					cp[5] = cp[4];
					cp[4].x = (op[n2].x < left ? left : right);
					cp[4].y = (op[n2].y < bottom ? bottom : top);
				}
				else // 7
				{
					cp[6] = cp[5];
					cp[5].x = (op[n2].x < left ? left : right);
					cp[5].y = (op[n2].y < bottom ? bottom : top);
				}
			}
		}
		else
		{
			T_POINT	LT(left, top);
			T_POINT	LB(left, bottom);
			T_POINT	RT(right, top);
			T_POINT	RB(right, bottom);

			BOOL	bLT = xIsPointInTri(op[0], op[1], op[2], LT);
			BOOL	bLB = xIsPointInTri(op[0], op[1], op[2], LB);
			BOOL	bRT = xIsPointInTri(op[0], op[1], op[2], RT);
			BOOL	bRB = xIsPointInTri(op[0], op[1], op[2], RB);
			int		nAdd = (bLT ? 1 : 0) + (bLB ? 1 : 0) + (bRT ? 1 : 0) + (bRB ? 1 : 0);

			switch(nAdd)
			{
			case 0:
				cp[2] = cp[5];
				num = 3;
				break;
			case 1:
				cp[2] = (bLT ? LT : bLB ? LB : bRT ? RT : RB);
				cp[3] = cp[5];
				num = 4;
				break;
			case 2:
				for(num = 2; num < 4; num++)
				{
					if(bLT)			{ cp[num] = LT;	bLT = FALSE; }
					else if(bLB)	{ cp[num] = LB;	bLB = FALSE; }
					else if(bRT)	{ cp[num] = RT;	bRT = FALSE; }
					else			{ cp[num] = RB;	bRB = FALSE; }
				}
				if(fabs(xGetCos(cp[1], cp[2], cp[3]) - PI / 2.0) > ONE_EPSILON)
				{
					tp[0] = cp[2];
					cp[2] = cp[3];
					cp[3] = tp[0]; 
				}

				cp[4] = cp[5];
				num = 5;
				break;
			case 3:
				if(! bLT)		{ cp[2] = LB;	cp[3] = RB;	cp[4] = RT; }
				else if(! bLB)	{ cp[2] = RB;	cp[3] = RT;	cp[4] = LT; }
				else if(! bRB)	{ cp[2] = RT;	cp[3] = LT;	cp[4] = LB; }
				else			{ cp[2] = LT;	cp[3] = LB;	cp[4] = RB; }

				if(fabs(xGetCos(cp[1], cp[2], cp[3]) - PI / 2.0) > ONE_EPSILON)
				{	
					tp[0] = cp[2];	
					cp[2] = cp[4];	
					cp[4] = tp[0]; 
				}
				num = 6;
				break;
			}
		}
		return num;
	}

	// 6. no point in rectangle
	int			ncp = 0;
	T_POINT	ccp[10];
	T_POINT	ttp[6];

	ttp[0] = op[0];		ttp[1] = op[1];
	ttp[2] = op[1];		ttp[3] = op[2];
	ttp[4] = op[2];		ttp[5] = op[0];
	if(IsLineInRect(ttp[0], ttp[1], &cp[0])) { ccp[ncp++] = cp[0];  ccp[ncp++] = cp[1]; }
	if(IsLineInRect(ttp[2], ttp[3], &cp[0])) { ccp[ncp++] = cp[0];  ccp[ncp++] = cp[1]; }
	if(IsLineInRect(ttp[4], ttp[5], &cp[0])) { ccp[ncp++] = cp[0];  ccp[ncp++] = cp[1]; }

	T_POINT	LT(left, top);
	T_POINT	LB(left, bottom);
	T_POINT	RT(right, top);
	T_POINT	RB(right, bottom);

	if(xIsPointInTri(op[0], op[1], op[2], LT)) ccp[ncp++] = LT;
	if(xIsPointInTri(op[0], op[1], op[2], LB)) ccp[ncp++] = LB;
	if(xIsPointInTri(op[0], op[1], op[2], RT)) ccp[ncp++] = RT;
	if(xIsPointInTri(op[0], op[1], op[2], RB)) ccp[ncp++] = RB;

	for(i = 0; i < ncp - 1; i++)
	{
		for(j = i + 1; j < ncp; j++)
		{
			if(ccp[i] != ccp[j]) continue;
			for(int k = j; k < ncp - 1; k++) ccp[k] = ccp[k + 1];
			ncp--;
		}
	}
	if(ncp < 3) return 0;

	for(i = 0; i < ncp - 1; i++)
	{
		for(j = i + 1; j < ncp; j++)
		{
			BOOL bChange = FALSE;

			n1 = fabs(ccp[i].x - left) < ONE_EPSILON ? 1 : fabs(ccp[i].y - bottom) < ONE_EPSILON ? 2 : fabs(ccp[i].x - right) < ONE_EPSILON ? 3 : 4;
			n2 = fabs(ccp[j].x - left) < ONE_EPSILON ? 1 : fabs(ccp[j].y - bottom) < ONE_EPSILON ? 2 : fabs(ccp[j].x - right) < ONE_EPSILON ? 3 : 4;

			if(n1 > n2) bChange = TRUE;
			else if(n1 == n2)
			{
				switch(n1)
				{
				case 1:	if(ccp[i].y < ccp[j].y) bChange = TRUE;	break;
				case 2:	if(ccp[i].x > ccp[j].x) bChange = TRUE;	break;
				case 3:	if(ccp[i].y > ccp[j].y) bChange = TRUE;	break;
				case 4:	if(ccp[i].x < ccp[j].x) bChange = TRUE;	break;
				}
			}
			if (bChange) { ttp[0] = ccp[i];  ccp[i] = ccp[j];  ccp[j] = ttp[0]; }
		}
	}

	memcpy(cp, ccp, sizeof(T_POINT) * ncp);

	return ncp;
}


/////////////////////////////////////////////////////////////////////////////
// 3. T_SEGMENT

BOOL T_SEGMENT::GetMidPoint(T_POINT& pt)
{
	if(fabs(Length) < 0.0) return FALSE;

	pt = (P1 + P2) / 2.0;

	return TRUE;
}

void T_SEGMENT::Reverse()
{
	T_POINT tempPt = P1;
	P1 = P2;
	P2 = P1; 
	BuildDirection();
}

void T_SEGMENT::SetPerpSegment()
{
	T_POINT DirPt;
	double  Save;

	DirPt = Dir;
	Save = DirPt.x;
	DirPt.x = -DirPt.y;
	DirPt.y = Save;
	SetDir(DirPt);
	CalcPt2();
	BuildDirection();
}

void T_SEGMENT::SlideAlong(double distance, T_POINT& newPt)
{
	newPt.x = P1.x + distance * Dir.x;
	newPt.y = P1.y + distance * Dir.y;
}

void T_SEGMENT::SetInfiniteLength(double dLeng)
{
	T_POINT po[2];
	SlideAlong(dLeng, po[0]);
	SlideAlong(-dLeng, po[1]);
	SetSegmentBuild(po);
}

void T_SEGMENT::SetTranslateP1(const T_POINT& pp)
{
	T_POINT Po;
	Po = pp - P1;

	P1 = pp;
	P2 = P2 + Po;
	BuildDirection();
}

void T_SEGMENT::SetTranslateP2(const T_POINT& pp)
{
	T_POINT Po;
	Po = pp - P2;

	P2 = pp;
	P1 = P1 + Po;
	BuildDirection();
}

void T_SEGMENT::SetTranslateMid(const T_POINT& pp)
{
	T_POINT mPt, Po;
	GetMidPoint(mPt);

	Po = pp - mPt;

	P1 += Po;
	P2 += Po;
	BuildDirection();
}

void T_SEGMENT::NormalDirection(BOOL clockwise, T_POINT& result)
{ 
	// in the x-y plane, no flattening of z
	// Clockwise means if a poly is built clockwise, setting clockwise to TRUE
	// will ensure that the normal direction will be towards the inside of the
	// poly.

	T_POINT temp;
	T_POINT dir = Dir;
	// round off the close to's
	temp = dir;

	// normalize vector
	double value = sqrt(temp.x * temp.x + temp.y * temp.y);
	temp.x = temp.x / value;
	temp.y = temp.y / value;

	if(fabs(temp.y) < DBL_EPSILON)
	{
		if (clockwise)
		{
			temp.x = -temp.x;
		}
		result.y = temp.x;
		result.x = 0.0;
	}
	else if(fabs(temp.x) < DBL_EPSILON)
	{
		if(! clockwise)
		{
			temp.y = -temp.y;
		}
		result.x = temp.y;
		result.y = 0.0;
	}
	// reverse directions of either x or y
	else if(temp.x > 0.0 && temp.y > 0.0 || temp.x < 0.0 && temp.y < 0.0) 
	{
		if(clockwise)
		{
			temp.x = -temp.x;
		}
		else
		{
			temp.y = -temp.y;
		}
		result.y = temp.x;
		result.x = temp.y;
	} 
	else if(temp.y > 0.0 && temp.x < 0.0 || temp.y < 0.0 && temp.x > 0.0)
	{
		if(clockwise)
		{
			temp.x = -temp.x;
		}
		else
		{
			temp.y = -temp.y;
		}
		result.y = temp.x;
		result.x = temp.y;
	}
}


void T_SEGMENT::ParallelSegmentAtDistance(double distance, T_SEGMENT& seg, BOOL clockwise)
{
	T_POINT p, dir;
	NormalDirection(clockwise, dir); 

	// TRUE assumes that the segment is oriented such that, if it were
	// part of a poly, and the poly is ordered in a clockwise manner, 
	// the segment created would be towards the inside of the poly.

	p = P1;
	T_POINT t1;
	t1.x = p.x + distance * dir.x;
	t1.y = p.y + distance * dir.y;

	p = P2;
	T_POINT t2;
	t2.x = p.x + distance * dir.x;
	t2.y = p.y + distance * dir.y;

	seg.SetSegmentBuild(t1, t2);
}

BOOL T_SEGMENT::SameDirection(const T_SEGMENT& seg)
{
	if(Dir == seg.Dir)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL T_SEGMENT::SameDirection(const T_POINT& p1, const T_POINT& p2)
{
	T_SEGMENT seg;
	seg.SetSegment(p1, p2);
	seg.BuildDirection();

	if(Dir == seg.Dir)
	{
		return TRUE;
	}
	return FALSE;
}