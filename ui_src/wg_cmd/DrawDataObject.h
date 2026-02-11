#pragma once

#include "DrawData.h"
#include "DrawDataFormat.h"

#include "HeaderPre.h"
#pragma pack(push, 1)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POINT (실제 Point 정보)

struct __MY_EXT_CLASS__ T_POINT 
{
	double x;
	double y;
	double bulge;  // + : 반시계방향(진행방향의 우측), - : 시계방향(진행방향의 좌측), 0 : 직선 (Line중심에서 호의 중심까지 거리 )
	
	T_POINT() { memset(this, 0, sizeof(T_POINT)); }
	T_POINT(const T_POINT& p)		{ x = p.x;   y = p.y;  bulge = p.bulge; }
	T_POINT(const POINT& p)			{ x = p.x;   y = p.y; }
	T_POINT(double x1, double y1)	{ x = x1;	 y = y1; }
	T_POINT(const double xy[2])		{ x = xy[0]; y = xy[1]; }
	//- Constructor for a T_POINT starting at tail and pointing
	T_POINT(const T_POINT& tail, const T_POINT& head) { x = head.x - tail.x; y = head.y - tail.y; }

	// Operators returning T_POINT values
	T_POINT operator + (double v) const			{ T_POINT rp(x+v, y+v);  return rp; }
	T_POINT operator - (double v) const			{ T_POINT rp(x-v, y-v);  return rp; }
	T_POINT operator * (double v) const			{ T_POINT rp(x*v, y*v);  return rp; }
	T_POINT operator / (double v)				{ T_POINT rp; fDivide(v, rp);  return rp; }
	T_POINT operator + (const T_POINT& p) const { T_POINT rp(x+p.x, y+p.y);  return rp; }
	T_POINT operator - (const T_POINT& p) const { T_POINT rp(x-p.x, y-p.y);  return rp; }
	T_POINT operator * (const T_POINT& p) const { T_POINT rp(x*p.x, y*p.y);  return rp; }
	T_POINT operator / (const T_POINT& p)       { T_POINT rp; pDivide(p, rp);  return rp; }

	// Additional Operations
	void operator =  (double v)			{ x = v;   y = v;  }
	void operator += (double v)			{ x += v;  y += v; }
	void operator -= (double v)			{ x -= v;  y -= v; }
	void operator *= (double v)			{ x *= v;  y *= v; }
	void operator /= (double v)			{ fEqualDivide(v); }
	void operator += (const T_POINT& p) { x += p.x;  y += p.y; }
	void operator -= (const T_POINT& p) { x -= p.x;  y -= p.y; }
	void operator *= (const T_POINT& p) { x *= p.x;  y *= p.y; }
	void operator /= (const T_POINT& p) { pEqualDivide(p); }
	T_POINT operator - () const			{ return T_POINT(-x, -y); }

	const T_POINT& operator = (const T_POINT& p) { memcpy(this, &p, sizeof(T_POINT)); return *this; }
	BOOL operator == (const T_POINT& p)	{ return IsEqual(p); }
	BOOL operator != (const T_POINT& p)	{ return !(operator == (p)); }
	
	// Function 
	void Init() { memset(this, 0, sizeof(T_POINT)); }

	//- Heading: Set and Inquire Functions
	void Set(const double xv, const double yv) { x = xv; y = yv; }
	//- Change vector components to {x}, {y}
	void Set(const double xyz[2])				{ x = xyz[0]; y = xyz[1]; } 
	//- Change vector components to xy[0], xy[1]
	void Set(const T_POINT& tail, const T_POINT& head)  { x = head.x - tail.x; y = head.y - tail.y; }
	//- Change vector to go from tail to head.
	void Set(const T_POINT& to_copy) { *this = to_copy; }

	void GetXY(double &xx, double &yy) { xx = x;  yy = y;  } //- Get x, y  components
	void GetXY(double xy[2])           { xy[0] = x;  xy[1] = y; } //- Get xy tuple

	BOOL CompareData(T_POINT& data, double tol = ONE_EPSILON) 
	{
		if(fabs(x - data.x) < tol && fabs(y - data.y) < tol ) return TRUE;
		return FALSE;
	}

	double   Length() const					 { return sqrt(x * x + y * y); }
	double   Length2() const				 { return (x * x + y * y); }
	double   Length_Squared() const			 { return (x * x + y * y); } 
	double   Length(const T_POINT& p) const  { return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)); }
	double   Length2(const T_POINT& p) const { return ((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)); }
	T_POINT& Length(const double new_length) 
	{
		double len = this->Length();
		x *= new_length / len;  
		y *= new_length / len;  
		return *this;
	}
	double GetLength(T_POINT& pnt); 
	double GetLength(T_POINT& pnt, T_POINT& dir); 

	void Move(double disX, double disY) 
	{
		x += disX;  
		y += disY; 
	}
	
	void fDivide(double tv, T_POINT& rp);
	void pDivide(const T_POINT& tp, T_POINT& rp);
	void fEqualDivide(double tv);
	void pEqualDivide(const T_POINT& tp);
	BOOL IsEqual(const T_POINT& tp);

	double GetLineProperty(const T_POINT& p2, double& sinv, double& cosv);
};

inline double GetLength(const T_POINT& n1, const T_POINT& n2)
{
	return sqrt((n1.x-n2.x)*(n1.x-n2.x) + (n1.y-n2.y)*(n1.y-n2.y));
}

// X방향 Sorting을 위한 함수 
inline int ComparePointX(const void* d1, const void* d2)
{
	int cmp = (*((T_POINT*)d1)).x == (*((T_POINT*)d2)).x ? 0 : (*((T_POINT*)d1)).x > (*((T_POINT*)d2)).x ? 1 : -1;
	if(fabs((*((T_POINT*)d1)).x - (*((T_POINT*)d2)).x) < ONE_EPSILON) cmp = 0;
	return cmp;
}
// Y방향 Sorting을 위한 함수 
inline int ComparePointY(const void* d1, const void* d2)
{
	int cmp = (*((T_POINT*)d1)).y == (*((T_POINT*)d2)).y ? 0 : (*((T_POINT*)d1)).y > (*((T_POINT*)d2)).y ? 1 : -1;
	if(fabs((*((T_POINT*)d1)).y - (*((T_POINT*)d2)).y) < ONE_EPSILON) cmp = 0;
	return cmp;	
}

inline void DividePoint(const T_POINT& p1, const T_POINT& p2, double pos, T_POINT& cp)
{
	cp.x = p1.x * (1.0 - pos) + p2.x * pos;
	cp.y = p1.y * (1.0 - pos) + p2.y * pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_RECT 
 
enum
{
	EN_RECT_LT,
	EN_RECT_LC, 
	EN_RECT_LB,
	EN_RECT_CT, 
	EN_RECT_CC,
	EN_RECT_CB,
	EN_RECT_RT, 
	EN_RECT_RC, 
	EN_RECT_RB
};

struct __MY_EXT_CLASS__ T_RECT
{
	double	left;
	double	top;
	double	right;
	double	bottom;

	// Constructors
	T_RECT() { memset(this, 0, sizeof(T_RECT)); }
	T_RECT(const T_RECT& r)								{ left = r.left;  top = r.top; right = r.right;	bottom = r.bottom; }
	T_RECT(const T_POINT& p1, const T_POINT& p2)		{ left = p1.x;	  top = p1.y;  right = p2.x;	bottom= p2.y; }
	T_RECT(const POINT& p1, const POINT& p2)			{ left = p1.x;    top = p1.y;  right = p2.x;	bottom= p2.y; }
	T_RECT(double x1, double y1, double x2, double y2) { left = x1;	  top = y1;		right = x2;		bottom= y2; }

	// Operators returning T_POINT values
	T_RECT operator + (double v) const			{ T_RECT r(left+v, top+v, right+v, bottom+v);  return r; }
	T_RECT operator - (double v) const			{ T_RECT r(left-v, top-v, right-v, bottom-v);  return r; }
	T_RECT operator * (double v) const			{ T_RECT r(left*v, top*v, right*v, bottom*v);  return r; }
	T_RECT operator / (double v)				{ T_RECT r; fDivide(v, r);  return r; }
	T_RECT operator + (const T_RECT& r) const	{ T_RECT rr(left+r.left, top+r.top, right+r.right, bottom+r.bottom);  return rr; }
	T_RECT operator - (const T_RECT& r) const	{ T_RECT rr(left-r.left, top-r.top, right-r.right, bottom-r.bottom);  return rr; }
	T_RECT operator * (const T_RECT& r) const	{ T_RECT rr(left*r.left, top*r.top, right*r.right, bottom*r.bottom);  return rr; }
	T_RECT operator / (const T_RECT& r)			{ T_RECT rr;  rDivide(r, rr);  return rr; }

	// Additional Operations
	void operator = (double v)			{ left = v;   top = v;   right = v;   bottom = v;  }
	void operator += (double v)			{ left += v;  top += v;  right += v;  bottom += v; }
	void operator -= (double v)			{ left -= v;  top -= v;  right -= v;  bottom -= v; }
	void operator *= (double v)			{ left *= v;  top *= v;  right *= v;  bottom *= v; }
	void operator /= (double v)			{ fEqualDivide(v); }
	void operator += (const T_RECT& r)	{ left += r.left;  top += r.top;  right += r.right;  bottom += r.bottom; }
	void operator -= (const T_RECT& r)	{ left -= r.left;  top -= r.top;  right -= r.right;  bottom -= r.bottom; }
	void operator *= (const T_RECT& r)	{ left *= r.left;  top *= r.top;  right *= r.right;  bottom *= r.bottom; }
	void operator /= (const T_RECT& r)	{ rEqualDivide(r); }

	const T_RECT& operator = (const T_RECT& r)	{ left = r.left;  top = r.top;  right = r.right;  bottom = r.bottom;  return *this; }
	BOOL operator == (const T_RECT& r)			{ return IsEqual(r); }
	BOOL operator != (const T_RECT& r)			{ return !(operator == (r)); }

	// Operations
	void   Init() { memset(this, 0, sizeof(T_RECT)); }
	void   Set(double x1, double y1, double x2, double y2) { left= x1;  top= y1;  right= x2;  bottom= y2; }
	double Height() const	 { return top - bottom; }
	double Width()  const	 { return right - left; }
	double CenterH() const	 { return (left + right) / 2.0; }
	double CenterV() const	 { return (bottom + top) / 2.0; }

	void fDivide(double tv, T_RECT& rr);
	void rDivide(const T_RECT& tr, T_RECT& rr);
	void fEqualDivide(double tv);
	void rEqualDivide(const T_RECT& tr);
	BOOL IsEqual(const T_RECT& tr);
	
	BOOL IsIn(const T_POINT& p); 
	BOOL IsOn(const T_POINT& p); 
	BOOL IsCross(const T_RECT& r); 
	BOOL IsInRect(const T_RECT& r);

	BOOL IsLineInRect(const T_POINT& p1, const T_POINT& p2, T_POINT cp[2]);
	int  IsTriInRect(T_POINT op[3], T_POINT cp[7]);
};

typedef CArray<T_POINT, T_POINT>	nrT_POINT;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_LINE 

struct __MY_EXT_CLASS__ T_LINE // DGN삽도를 위한 Line정보
{
	POINT  P1; // 시작점
	POINT  P2; // 끝점
	double dThik;    // 라인 두께
	int    nAlign;   // 선의 Align 정보 (1:Center, 2:Left, 3:Right)

	T_LINE() { Init(); }
	T_LINE(const T_LINE& pData) { *this = pData; }
	T_LINE& operator = (const T_LINE& pData)
	{
		memcpy(this, &pData, sizeof(T_LINE));
		return *this;
	}
	T_LINE operator + (const T_LINE& pData) const 
	{
		T_LINE data; 
		data.P1.x = P1.x + pData.P1.x;
		data.P1.y = P1.y + pData.P1.y;
		data.P2.x = P2.x + pData.P2.x;
		data.P2.y = P2.y + pData.P2.y;
		return data;
	}
	T_LINE operator - (const T_LINE& pData) const 
	{
		T_LINE data; 
		data.P1.x = P1.x - pData.P1.x;
		data.P1.y = P1.y - pData.P1.y;
		data.P2.x = P2.x - pData.P2.x;
		data.P2.y = P2.y - pData.P2.y;
		return data;
	}

	void Init() { memset(this, 0, sizeof(T_LINE)); }
	
	void Move(int disX, int disY) 	
	{
		P1.x += disX;    
		P1.y += disY;  
		P2.x += disX; 
		P2.y += disY; 
	}

	void GetBoundary(LONG& nXmax, LONG& nXmin, LONG& nYmax, LONG& nYmin)
	{
		nXmax = max(P1.x, P2.x);
		nXmin = min(P1.x, P2.x);
		nYmax = max(P1.y, P2.y);
		nYmin = min(P1.y, P2.y);
	}

	BOOL IsEqual(T_LINE& rData)
	{
		if(P1.x   != rData.P1.x)   return FALSE;
		if(P1.y   != rData.P1.y)   return FALSE;
		if(P2.x   != rData.P2.x)   return FALSE;
		if(P2.x   != rData.P2.x)   return FALSE;
		if(dThik  != rData.dThik ) return FALSE;
		if(nAlign != rData.nAlign) return FALSE;
		return TRUE;
	}
};

typedef CArray<T_LINE, T_LINE> nrT_LINE;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_LINE_R 

struct __MY_EXT_CLASS__ T_LINE_R // DGN삽도를 위한 실제 Line정보
{
	T_POINT P1;		 // 시작점
	T_POINT P2;		 // 끝점
	double dThik;    // 라인 두께
	int    nAlign;   // 선의 Align 정보 (1:Center, 2:Left, 3:Right)

	T_LINE_R() { Init(); }
	T_LINE_R(const T_LINE_R& pData) { *this = pData; }
	T_LINE_R& operator = (const T_LINE_R& pData)
	{
		P1 = pData.P1;
		P2 = pData.P2;
		dThik = pData.dThik;
		nAlign = pData.nAlign;
		return *this;
	}
	T_LINE_R operator + (const T_LINE_R& pData) const 
	{
		T_LINE_R data; 
		data.P1.x = P1.x + pData.P1.x;
		data.P1.y = P1.y + pData.P1.y;
		data.P2.x = P2.x + pData.P2.x;
		data.P2.y = P2.y + pData.P2.y;
		return data;
	}
	T_LINE_R operator - (const T_LINE_R& pData) const 
	{
		T_LINE_R data; 
		data.P1.x = P1.x - pData.P1.x;
		data.P1.y = P1.y - pData.P1.y;
		data.P2.x = P2.x - pData.P2.x;
		data.P2.y = P2.y - pData.P2.y;
		return data;
	}

	void Init()
	{
		P1.Init(); 
		P2.Init();
		dThik = 0.0;
		nAlign = 0;
	}

	void Move(double disX, double disY)
	{ 
		P1.x += disX;   
		P1.y += disY;  
		P2.x += disX;   
		P2.y += disY; 
	}

	void GetBoundary(double& dXmax, double& dXmin, double& dYmax, double& dYmin)
	{
		dXmax = max(P1.x, P2.x);
		dXmin = min(P1.x, P2.x);
		dYmax = max(P1.y, P2.y);
		dYmin = min(P1.y, P2.y);
	}

	BOOL IsEqual(T_LINE_R& rData)
	{
		if(P1     != rData.P1)     return FALSE;
		if(P2     != rData.P2)     return FALSE;
		if(dThik  != rData.dThik ) return FALSE;
		if(nAlign != rData.nAlign) return FALSE;
		return TRUE;
	}
};
typedef CArray<T_LINE_R, T_LINE_R> nrT_LINE_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CIRCLE 

struct __MY_EXT_CLASS__ T_CIRCLE // DGN삽도를 위한 Circle정보
{
	POINT CenterP;// 중앙좌표
	int   Radius; // 반지름
	
	T_CIRCLE()	{ Init(); }
	T_CIRCLE(const T_CIRCLE& pData) { *this = pData; }
	T_CIRCLE& operator = (const T_CIRCLE& pData)
	{
		memcpy(this, &pData, sizeof(T_CIRCLE));
		return *this;
	}
	
	void Init() { memset(this, 0, sizeof(T_CIRCLE)); }

	void Move(int disX, int disY)
	{
		CenterP.x += disX;  
		CenterP.y += disY; 
	}
};
typedef CArray<T_CIRCLE, T_CIRCLE> nrT_CIRCLE;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CIRCLE_R 

struct __MY_EXT_CLASS__ T_CIRCLE_R // DGN삽도를 위한 실제 Circle정보
{
	T_POINT CenterP;// 중앙좌표
	double  Radius; // 반지름
	
	T_CIRCLE_R() { Init(); }
	T_CIRCLE_R(const T_CIRCLE_R& pData) { *this = pData; }
	T_CIRCLE_R& operator = (const T_CIRCLE_R& pData)
	{
		CenterP = pData.CenterP;
		Radius = pData.Radius; 
		return *this;
	}
	
	void Init()
	{
		CenterP.Init();
		Radius = 0.0;
	}

	void Move(double disX, double disY)
	{ 
		CenterP.x += disX; 
		CenterP.y += disY;
	}
};
typedef CArray<T_CIRCLE_R, T_CIRCLE_R> nrT_CIRCLE_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DONUT 

struct __MY_EXT_CLASS__ T_DONUT // DGN삽도를 위한 Donut정보
{
	POINT CenterP;   // 중앙좌표
	int   OutRadius; // 바깥원반지름
	int   InRadius;  // 안쪽원 반지름
	
	T_DONUT() { Init(); }
	T_DONUT(const T_DONUT& pData)	{*this = pData;}
	T_DONUT& operator = (const T_DONUT& pData)
	{
		memcpy(this, &pData, sizeof(T_DONUT));
		return *this;
	}
	
	void Init() { memset(this, 0, sizeof(T_DONUT)); }
	
	void Move(int disX, int disY)
	{
		CenterP.x += disX; 
		CenterP.y += disY; 
	}
};
typedef CArray<T_DONUT, T_DONUT> nrT_DONUT;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DONUT_R 

struct __MY_EXT_CLASS__ T_DONUT_R // DGN삽도를 위한 실제 Donut정보
{
	T_POINT CenterP;   // 중앙좌표
	double  OutRadius; // 바깥원반지름
	double  InRadius;  // 안쪽원 반지름
	
	T_DONUT_R()	{ Init(); }
	T_DONUT_R(const T_DONUT_R& pData) { *this = pData; }
	T_DONUT_R& operator = (const T_DONUT_R& pData)
	{
		CenterP = pData.CenterP; 
		OutRadius = pData.OutRadius;
		InRadius = pData.InRadius;  
		return *this;
	}
	
	void Init()
	{ 
		CenterP.Init();  
		OutRadius = 0.0;
		InRadius = 0.0;
	}

	void Move(double disX, double disY)
	{ 
		CenterP.x += disX;  
		CenterP.y += disY;
	}
};
typedef CArray<T_DONUT_R, T_DONUT_R> nrT_DONUT_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARC 

struct __MY_EXT_CLASS__ T_ARC // DGN삽도를 위한 Arc정도 (Angle Arc : 타원형 Acr는 지원하지 않음, 채움 지원하지 않음)
{
	POINT CenterP;    // 중앙좌표
	int   Radius;     // 반지름
	float StartAngle; // 시작각도(x방향을 기준으로 반시계방향 +)
	float SweepAngle; // 내부각도(반시계방향 +) 
	
	T_ARC()	{ Init(); }
	T_ARC(const T_ARC& pData) { *this = pData; }
	T_ARC& operator = (const T_ARC& pData)
	{
		memcpy(this, &pData, sizeof(T_ARC));	
		return *this;
	}
	
	void Init() { memset(this, 0, sizeof(T_ARC)); }
	
	void Move(int disX, int disY)
	{ 
		CenterP.x += disX; 
		CenterP.y += disY;
	}
}; 
typedef CArray<T_ARC, T_ARC> nrT_ARC;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ARC_R 

struct __MY_EXT_CLASS__ T_ARC_R // DGN삽도를 위한 실제 Arc정도 (타원형 Acr는 지원하지 않음)
{
	T_POINT CenterP;    // 중앙좌표
	double  Radius;     // 반지름
	double  StartAngle; // 시작각도
	double  SweepAngle; // 내부각도(반시계방향 +) 
	
	T_ARC_R()	{ Init(); }
	T_ARC_R(const T_ARC_R& pData) { *this = pData; }
	T_ARC_R& operator = (const T_ARC_R& pData)
	{
		CenterP = pData.CenterP;
		Radius = pData.Radius; 
		StartAngle = pData.StartAngle; 
		SweepAngle = pData.SweepAngle; 
		return *this;
	}

	void Init()
	{
		CenterP.Init();  
		Radius = 0.0; 
		StartAngle = 0.0;
		SweepAngle = 0.0;
	}
	
	void Move(double disX, double disY)
	{ 
		CenterP.x += disX; 
		CenterP.y += disY;
	}
};
typedef CArray<T_ARC_R, T_ARC_R> nrT_ARC_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYLINE 

struct __MY_EXT_CLASS__ T_POLYLINE // DGN삽도를 위한 Polyline정보
{
	nrPOINT arPoint;

	T_POLYLINE() { Init(); }
	T_POLYLINE(const T_POLYLINE& pData) { *this = pData; }
	T_POLYLINE& operator = (const T_POLYLINE& pData)
	{
		arPoint.Copy(pData.arPoint);
		return *this;
	}
	
	void Init() { arPoint.RemoveAll(); }

	void Move(int disX, int disY)
	{ 
		int i;
		for(i = 0; i < arPoint.GetSize(); i++) 
		{
			arPoint[i].x += disX;    
			arPoint[i].y += disY;			
		}
	}
};
typedef CArray<T_POLYLINE, T_POLYLINE> nrT_POLYLINE;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYLINE_R 

struct __MY_EXT_CLASS__ T_POLYLINE_R // DGN삽도를 위한 실제 Polyline정보
{
	nrT_POINT arPoint;

	T_POLYLINE_R() { Init(); }
	T_POLYLINE_R(const T_POLYLINE_R& pData) { *this = pData; }
	T_POLYLINE_R& operator = (const T_POLYLINE_R& pData)
	{
		arPoint.Copy(pData.arPoint);    
		return *this;
	}

	void Init() { arPoint.RemoveAll(); }

	void Move(double disX, double disY)
	{ 
		int i;
		for(i = 0; i < arPoint.GetSize(); i++)
		{
			arPoint[i].Move(disX, disY);
		}
	}
};
typedef CArray<T_POLYLINE_R, T_POLYLINE_R> nrT_POLYLINE_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYGON 

struct __MY_EXT_CLASS__ T_POLYGON // DGN삽도를 위한 Polyline정보
{
	nrPOINT arPoint;

	T_POLYGON() { Init(); }
	T_POLYGON(const T_POLYGON& pData) { *this = pData; }
	T_POLYGON& operator = (const T_POLYGON& pData)
	{
		arPoint.Copy(pData.arPoint);
		return *this;
	}

	void Init() { arPoint.RemoveAll(); }

	BOOL IsEqual(T_POLYGON& rData)
	{
		int nSize = (int)arPoint.GetSize();
		if(nSize != (int)rData.arPoint.GetSize()) return FALSE;
		for(int i = 0; i < nSize; i++)
		{ 
			if(arPoint[i].x != rData.arPoint[i].x) return FALSE; 
			if(arPoint[i].y != rData.arPoint[i].y) return FALSE; 
		}
		return TRUE;
	}

	void Move(int disX, int disY)
	{ 
		int i;
		for(i = 0; i < arPoint.GetSize(); i++) 
		{
			arPoint[i].x += disX;    
			arPoint[i].y += disY;			
		}
	}

	void GetBoundary(LONG& nXmax, LONG& nXmin, LONG& nYmax, LONG& nYmin)
	{
		nXmax = nXmin = nYmax = nYmin = 0;
		int nSize = (int)arPoint.GetSize();
		if(nSize > 0)
		{
			nXmax = nXmin = arPoint[0].x;
			nYmax = nYmin = arPoint[0].y;
		}
		for(int i = 1; i < nSize ; i++)
		{
			nXmax = max(nXmax, arPoint[i].x);
			nXmin = min(nXmin, arPoint[i].y);
			nYmax = max(nYmax, arPoint[i].x);
			nYmin = min(nYmin, arPoint[i].y);
		}
	}
	void Transfer(POINT dOffset)
	{
		int nSize = arPoint.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			arPoint[i].x += dOffset.x;
			arPoint[i].y += dOffset.y;
		}
	}
	void Rotate(LONG angle)// 반시계 방향으로 회전시킴
	{
		double RAngle = angle * PI / 180.0;
		double sin_v = sin(RAngle);
		double cos_v = cos(RAngle);
		for(int i = 0; i < arPoint.GetSize(); i++)
		{
			POINT& tmp = arPoint[i];
			arPoint[i].x = LONG(tmp.x * cos_v - tmp.y * sin_v);
			arPoint[i].y = LONG(tmp.x * sin_v + tmp.y * cos_v);
		}
	}
	void Mirror(int axis)
	{
		switch(axis)
		{
		case 0: // x axis mirror
			{
				for (int i = 0; i < arPoint.GetSize(); i++)
				{
					arPoint[i].y = -arPoint[i].y;
				}
			}
			break;
		case 1: // y axis mirror
			{
			for (int i = 0; i < arPoint.GetSize(); i++) arPoint[i].x = -arPoint[i].x;
			}
			break;
		}
	}
	// 좌표순서를 뒤집음
	void ReverseOrder()
	{
		int nSize = arPoint.GetSize();
		POINT pointUnit1, pointUnit2;
		int iRemnant = nSize % 2;
		int nHalfSize = (nSize - iRemnant) / 2;
		int iPosi;

		for(int i = 0; i < nHalfSize; ++i)
		{
			iPosi = nSize - i - 1;
			pointUnit1 = arPoint[i];
			pointUnit2 = arPoint[iPosi];
			arPoint[i] = pointUnit2;
			arPoint[iPosi] = pointUnit1;
		}
	}
};
typedef CArray<T_POLYGON, T_POLYGON> nrT_POLYGON;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_POLYGON 

struct __MY_EXT_CLASS__ T_POLYGON_R // DGN삽도를 위한 실제 Polyline정보
{
	nrT_POINT arPoint;

	T_POLYGON_R() { Init(); }
	T_POLYGON_R(const T_POLYGON_R& pData) { *this = pData; }
	T_POLYGON_R& operator = (const T_POLYGON_R& pData)
	{
		arPoint.Copy(pData.arPoint);
		return *this;
	}

	void Init() { arPoint.RemoveAll(); }

	BOOL IsEqual(T_POLYGON_R& rData)
	{
		int nSize = arPoint.GetSize();
		if(nSize != rData.arPoint.GetSize()) return FALSE;
		for(int i = 0; i < nSize; i++)
		{ 
			if(arPoint[i] != rData.arPoint[i]) return FALSE; 			
		}
		return TRUE;
	}

	void Move(double disX, double disY)
	{ 
		int i;
		for(i = 0; i < arPoint.GetSize(); i++) 
		{
			arPoint[i].x += disX;    
			arPoint[i].y += disY;			
		}
	}

	void GetBoundary(double& dXmax, double& dXmin, double& dYmax, double& dYmin)
	{
		dXmax = dXmin = dYmax = dYmin = 0.0;
		int nSize = arPoint.GetSize();
		if(nSize > 0)
		{
			dXmax = dXmin = arPoint[0].x;
			dYmax = dYmin = arPoint[0].y;
		}
		for(int i = 1; i < nSize ; i++)
		{
			dXmax = max(dXmax, arPoint[i].x);
			dXmin = min(dXmin, arPoint[i].x);
			dYmax = max(dYmax, arPoint[i].y);
			dYmin = min(dYmin, arPoint[i].y);
		}
	}
	void Transfer(T_POINT dOffset)
	{
		int nSize = arPoint.GetSize();
		for(int i = 0; i < nSize; i++)
		{
			arPoint[i].x += dOffset.x;
			arPoint[i].y += dOffset.y;
		}
	}
	void Rotate(double angle)// 반시계 방향으로 회전시킴
	{
		angle = angle * PI / 180.0;
		double sin_v = sin(angle);
		double cos_v = cos(angle);
		for(int i = 0; i < arPoint.GetSize(); i++)
		{
			T_POINT& tmp = arPoint[i];
			arPoint[i].x = tmp.x * cos_v - tmp.y * sin_v;
			arPoint[i].y = tmp.x * sin_v + tmp.y * cos_v;
		}
	}
	void Mirror(int axis)
	{
		switch(axis)
		{
		case 0: // x axis mirror
			{
			for (int i = 0; i < arPoint.GetSize(); i++) arPoint[i].y = -arPoint[i].y;
			}
			break;
		case 1: // y axis mirror
			{
			for (int i = 0; i < arPoint.GetSize(); i++) arPoint[i].x = -arPoint[i].x;
			}
			break;
		}
	}
	// 좌표순서를 뒤집음
	void ReverseOrder()
	{
		int nSize = arPoint.GetSize();
		T_POINT pointUnit1, pointUnit2;
		int iRemnant = nSize % 2;
		int nHalfSize = (nSize - iRemnant) / 2;
		int iPosi;

		for(int i = 0; i < nHalfSize; ++i)
		{
			iPosi = nSize - i - 1;
			pointUnit1 = arPoint[i];
			pointUnit2 = arPoint[iPosi];
			arPoint[i] = pointUnit2;
			arPoint[iPosi] = pointUnit1;
		}
	}
};
typedef CArray<T_POLYGON_R, T_POLYGON_R> nrT_POLYGON_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_TEXT 

// 문자(strText)는 아래의 기호에 따라 윗첨자, 아래첨자, 줄바꿈을 실행할 수 있습니다.
//  - 아래첨자 : 시작위치에 '`'를 끝위치에 '$'를 사용합니다.
//  - 윗첨자   : 시작위치에 '^'를 끝위치에 '$'를 사용합니다.
//  - 줄바꿈   : '\n'를 사용합니다.

struct __MY_EXT_CLASS__ T_TEXT // DGN삽도를 위한 Text정보
{
	TLongStr      szText;  // 문자열 
	POINT         PPosition;// 기준좌표
	BOOL          bCurrFormat;
	T_TEXT_FORMAT CurrFormat;
	
	T_TEXT()	{ Init(); }
	T_TEXT(const T_TEXT& pData) { *this = pData; }
	T_TEXT& operator = (const T_TEXT& pData)
	{
		PPosition = pData.PPosition;  
		szText = pData.szText;  		
		bCurrFormat = pData.bCurrFormat; 
		CurrFormat = pData.CurrFormat;  		
		return *this;
	}
	
	void Init()
	{
		PPosition.x = 0; 
		PPosition.y = 0; 
		szText = _T("");
		bCurrFormat = FALSE;  
		CurrFormat.Init();
	}
	
	void Move(int disX, int disY)
	{ 
		PPosition.x += disX;   
		PPosition.y += disY; 
	}
};
typedef CArray<T_TEXT, T_TEXT> nrT_TEXT;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_TEXT_R 

struct __MY_EXT_CLASS__ T_TEXT_R // DGN삽도를 위한 실제 Text정보
{
	TLongStr      szText;  // 문자열
	T_POINT       PPosition;// 기준좌표
	BOOL		  bCurrFormat;
	T_TEXT_FORMAT CurrFormat;
	
	T_TEXT_R() { Init(); }
	T_TEXT_R(const T_TEXT_R& pData) { *this = pData; }
	T_TEXT_R& operator = (const T_TEXT_R& pData)
	{
		PPosition   = pData.PPosition;   
		szText      = pData.szText;  	
		bCurrFormat = pData.bCurrFormat;
		CurrFormat  = pData.CurrFormat;  		
		return *this;
	}

	void Init()
	{
		PPosition.Init(); 
		szText = _T("");
		bCurrFormat = FALSE;  
		CurrFormat.Init();
	}

	void Move(double disX, double disY)
	{
		PPosition.x += disX; 
		PPosition.y += disY; 
	}
};
typedef CArray<T_TEXT_R, T_TEXT_R> nrT_TEXT_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMENSION 

struct __MY_EXT_CLASS__ T_DIMENSION // DGN삽도를 위한 Dimension정보
{
	TStr  szText;  // 문자열
	POINT P1;      // 시작점
	POINT P2;      // 끝점	
	// ※DefaultAngle : P1와 P2가 Pixel단위에서 동일하나 실제 거리를 가지게 되는 경우 화면상에 표시될 각도
	BOOL   bDefaultAngle; // DefaultAngle값을 사용할지 여부
	double dDefaultAngle; // DefaultAngle값(x방향을 기준으로 반시계방향 +)
	
	T_DIMENSION() { Init(); }
	T_DIMENSION(const T_DIMENSION& pData) { *this = pData; }
	T_DIMENSION& operator = (const T_DIMENSION& pData)
	{
		memcpy(this, &pData, sizeof(T_DIMENSION));
		return *this;
	}
	
	void Init() { memset(this, 0, sizeof(T_DIMENSION)); }

	void Move(int disX, int disY)
	{
		P1.x += disX;  
		P1.y += disY;  
		P2.x += disX; 
		P2.y += disY;
	}
};
typedef CArray<T_DIMENSION, T_DIMENSION> nrT_DIMENSION;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_DIMENSION_R 

struct __MY_EXT_CLASS__ T_DIMENSION_R // DGN삽도를 위한 실제 Dimension정보
{
	TStr    szText;  // 문자열
	T_POINT P1;      // 시작점
	T_POINT P2;      // 끝점
	
	T_DIMENSION_R()	{ Init(); }
	T_DIMENSION_R(const T_DIMENSION_R& pData) { *this = pData; }
	T_DIMENSION_R& operator = (const T_DIMENSION_R& pData)
	{
		P1 = pData.P1; 
		P2 = pData.P2; 
		szText = pData.szText; 
		return *this;
	}
	
	void Init() 
	{
		P1.Init();
		P2.Init(); 
		szText = _T("");
	}

	void Move(double disX, double disY)
	{
		P1.x += disX;  
		P1.y += disY;   
		P2.x += disX;  
		P2.y += disY; 
	}
};
typedef CArray<T_DIMENSION_R, T_DIMENSION_R> nrT_DIMENSION_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ANGLE

struct __MY_EXT_CLASS__ T_ANGLE // DGN삽도를 위한 Angle치수선 정보
{
	TStr    szText;     // 문자열
	POINT   CenterP;    // 중앙좌표
	float   StartAngle; // 시작각도
	float   SweepAngle; // 내부각도(반시계방향 +) 
	
	T_ANGLE()	{ Init(); }
	T_ANGLE(const T_ANGLE& pData) { *this = pData; }
	T_ANGLE& operator = (const T_ANGLE& pData)
	{
		memcpy(this, &pData, sizeof(T_ANGLE));
		return *this;
	}
	
	void Init() { memset(this, 0, sizeof(T_ANGLE)); }
	
	void Move(int disX, int disY)
	{ 
		CenterP.x += disX; 
		CenterP.y += disY;
	}
};
typedef CArray<T_ANGLE, T_ANGLE> nrT_ANGLE;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_ANGLE_R

struct __MY_EXT_CLASS__ T_ANGLE_R // DGN삽도를 위한 실제 Angle치수선 정보
{
	TStr    szText;     // 문자열
	T_POINT CenterP;    // 중앙좌표
	double  StartAngle; // 시작각도
	double  SweepAngle; // 내부각도(반시계방향 +) 
	
	T_ANGLE_R()	{ Init(); }
	T_ANGLE_R(const T_ANGLE_R& pData) { *this = pData; }
	T_ANGLE_R& operator = (const T_ANGLE_R& pData)
	{
		CenterP = pData.CenterP;  
		StartAngle = pData.StartAngle;
		SweepAngle = pData.SweepAngle;  
		szText = pData.szText;  
		return *this;
	}
	
	void Init()
	{
		CenterP.Init();
		StartAngle = 0.0;
		SweepAngle = 0.0;  
		szText = _T("");
	}
	
	void Move(double disX, double disY)
	{ 
		CenterP.x += disX; 
		CenterP.y += disY; 
	}
};
typedef CArray<T_ANGLE_R, T_ANGLE_R> nrT_ANGLE_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_LEADERLINE

struct __MY_EXT_CLASS__ T_LEADERLINE // DGN삽도를 위한 지시선정보
{
	TStr szText;   // 문자열
	BOOL bStrDir;  // 문자열의 방향
	
	nrPOINT arPoint;
	
	T_LEADERLINE() { Init(); }
	T_LEADERLINE(const T_LEADERLINE& pData) { *this = pData; }
	T_LEADERLINE& operator = (const T_LEADERLINE& pData)
	{
		szText = pData.szText; 
		bStrDir = pData.bStrDir;
		arPoint.Copy(pData.arPoint);
		return *this;
	}
	
	void Init() 
	{
		arPoint.RemoveAll(); 
		szText = _T("");  
		bStrDir = TRUE;
	}
	
	void Move(int disX, int disY)
	{ 
		int i;
		for(i = 0; i < arPoint.GetSize(); i++) 
		{
			arPoint[i].x += disX;    
			arPoint[i].y += disY;			
		}
	}
};
typedef CArray<T_LEADERLINE, T_LEADERLINE> nrT_LEADERLINE;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_LEADERLINE_R

struct __MY_EXT_CLASS__ T_LEADERLINE_R // DGN삽도를 위한 실제 지시선정보
{
	TStr  szText;   // 문자열
	BOOL  bStrDir;  // 문자열의 방향
	
	nrT_POINT arPoint;
		
	T_LEADERLINE_R() { Init(); }
	T_LEADERLINE_R(const T_LEADERLINE_R& pData) { *this = pData; }
	T_LEADERLINE_R& operator = (const T_LEADERLINE_R& pData)
	{
		szText = pData.szText;
		bStrDir = pData.bStrDir;
		arPoint.Copy(pData.arPoint);
		return *this;
	}

	void Init()
	{
		arPoint.RemoveAll();
		szText = _T(""); 
		bStrDir = TRUE;
	}

	void Move(double disX, double disY)
	{
		int i;
		for(i = 0; i < arPoint.GetSize(); i++) 
		{
			arPoint[i].Move(disX, disY);
		}
	}
};
typedef CArray<T_LEADERLINE_R, T_LEADERLINE_R> nrT_LEADERLINE_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_INFO_AXIS

// Chart 하위 구조체 ---------------------------------------

struct __MY_EXT_CLASS__ T_CHART_INFO_AXIS
{	
	BOOL     bAxis;					// 축을 표시할지 여부
	TStr     szName;				// 축 제목
	BOOL     bAxisName;				// 축 제목을 표시할지 여부
	BOOL     bMajorGrid;			// 주눈금선을 표시할지 여부
	BOOL     bMinorGrid;			// 보조눈금선을 표시할지 여부
	int      nDecimal;				// 소수점이하 자리수 표시(-1일때에는 전체표시)
	double   dNotchHeightRate;		// 눈금의 높이(음수일때에는 글자크기의 1/3적용)
	int      nMajorNotchType;		// 주눈금형식     (0:없음 1:바깥쪽 2:안쪽   3:교차)
	int      nMinorNotchType;		// 보조눈금형식   (0:없음 1:바깥쪽 2:안쪽   3:교차)
	int      nNotchLabelType;		// 눈금레이블형식 (0:없음 1:높은쪽 2:낮은쪽 3:축의옆)
	BOOL     bMaxValueAuto;			// 최대값을 자동으로 찾을지 여부
	double   dMaxValue;				// 최대값
	BOOL     bMinValueAuto;			// 최소값을 자동으로 찾을지 여부
	double   dMinValue;				// 최소값
	BOOL     bMajorSpaceAuto;		// 주단위 간격을 자동으로 찾을지 여부
	double   dMajorSpace;			// 주단위 간격
	BOOL     bMinorSpaceAuto;		// 보조단위 간격을 자동으로 찾을지 여부
	double   dMinorSpace;			// 보조단위 간격
	BOOL     bCrossPosiAuto;		// 타축과의 교점을 자동으로 찾을지 여부
	double   dCrossPosi;			// 타축과의 교점
	BOOL     bReverse;				// 축 값을 거꾸로	
	BOOL     nNotchCtrlPointType;	// 축의 기준점형식(1:높은쪽 2:낮은쪽 3:원점기준)
	
	T_CHART_INFO_AXIS()	{ Init(); }
	T_CHART_INFO_AXIS(const T_CHART_INFO_AXIS& pData) { *this = pData; }
	T_CHART_INFO_AXIS& operator = (const T_CHART_INFO_AXIS& pData)
	{
		memcpy(this, &pData, sizeof(T_CHART_INFO_AXIS));
		return *this;
	}

	void Init()
	{		
		bAxis            = TRUE;	// 축을 표시할지 여부
		szName           = _T("");  // 축 제목
		bAxisName	     = FALSE;	// 축 제목을 표시할지 여부
		bMajorGrid       = TRUE;	// 주눈금선을 표시할지 여부
		bMinorGrid       = FALSE;	// 보조눈금선을 표시할지 여부
		nDecimal         = -1;		// 소수점이하 자리수 표시(-1일때에는 전체표시)
		dNotchHeightRate = -1.0;	// 눈금의 높이(음수일때에는 글자크기의 1/3적용)
		nMajorNotchType  = 2;		// 주눈금형식     (0:없음 1:바깥쪽 2:안쪽   3:교차)
		nMinorNotchType  = 0;		// 보조눈금형식   (0:없음 1:바깥쪽 2:안쪽   3:교차)
		nNotchLabelType  = 2;		// 눈금레이블형식 (0:없음 1:높은쪽 2:낮은쪽 3:축의옆)
		bMaxValueAuto    = TRUE;	// 최소값을 자동으로 찾을지 여부
		dMaxValue        = 0.0;		// 최소값
		bMinValueAuto    = TRUE;	// 최대값을 자동으로 찾을지 여부
		dMinValue        = 0.0;		// 최대값
		bMajorSpaceAuto  = TRUE;	// 주단위 간격을 자동으로 찾을지 여부
		dMajorSpace      = 0.0;		// 주단위 간격
		bMinorSpaceAuto  = TRUE;	// 보조단위 간격을 자동으로 찾을지 여부
		dMinorSpace      = 0.0;		// 보조단위 간격
		bCrossPosiAuto   = TRUE;	// 타축과의 교점을 자동으로 찾을지 여부
		dCrossPosi       = 0.0;		// 타축과의 교점
		bReverse         = FALSE;	// 축 값을 거꾸로
		nNotchCtrlPointType = 2;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_INFO

struct __MY_EXT_CLASS__ T_CHART_INFO // Chart 틀의 세부 표시정보 (개별표시형식 및 범위)
{
	EN_CHART_TYPE     nType;		// 그래프 표시형식(현재09.06.06 분산형만 지원)
	TLongStr          szTitle;		// Chart 제목
	BOOL			  bTitle;       // 제목표시여부
	BOOL			  bRemark;      // 범례표시여부
	BOOL			  bOutDataCut;  // 축의 영역범위 밖의 그래프 정보 Cutting여부
	T_CHART_INFO_AXIS xAxis;
	T_CHART_INFO_AXIS yAxis;
	
	T_CHART_INFO()	{ Init(); }
	T_CHART_INFO(const T_CHART_INFO& pData) { *this = pData; }
	T_CHART_INFO& operator = (const T_CHART_INFO& pData)
	{
		nType    = pData.nType;
		szTitle  = pData.szTitle;
		bTitle   = pData.bTitle;
		bRemark  = pData.bRemark;
		bOutDataCut = pData.bOutDataCut;
		xAxis    = pData.xAxis;
		yAxis    = pData.yAxis;
		return *this;
	}

	void Init()
	{
		nType    = EN_CHART_TYPE_DISPERSAL;
		szTitle  = _T("");
		bTitle   = FALSE;
		bRemark  = FALSE;
		bOutDataCut = TRUE;
		xAxis.Init();
		yAxis.Init();
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_DATA_INFO

struct __MY_EXT_CLASS__ T_CHART_DATA_INFO // Chart 개별 Data의 세부 표시정보
{
	TStr szName;  
	
	EN_CHART_DATANAME_POSI_TYPE nNameType;		// Data Name의 표시 위치(Default = Remark내)
	EN_POSITION_TYPE            nNamePosiType; // Data Name의 표시 위치가 시작점 or 끝점일때 기준점으로 부터의 글자위치
	
	BOOL		   bLine;
	BOOL		   bMark;
	BOOL		   bFillMark;
	EN_POINT_TYPE markType;
	COLORREF	   lineColor;       // 선색
	COLORREF	   markLineColor;   // 표식 전경색
	COLORREF	   markFillColor;   // 표식 배경색
	int			   lineStyle;       // 선형식(PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME)
	int			   lineWidth;       // 선두께
	int			   markSize;        // 표식 크기
	
	T_CHART_DATA_INFO()	{ Init(); }
	T_CHART_DATA_INFO(const T_CHART_DATA_INFO& pData) { *this = pData; }
	T_CHART_DATA_INFO& operator = (const T_CHART_DATA_INFO& pData)
	{
		szName        = pData.szName       ;
		nNameType      = pData.nNameType     ;
		nNamePosiType  = pData.nNamePosiType ;
		bLine          = pData.bLine         ;
		bMark          = pData.bMark         ;
		bFillMark      = pData.bFillMark     ;
		markType       = pData.markType      ;
		lineColor      = pData.lineColor     ;
		markLineColor  = pData.markLineColor ;
		markFillColor  = pData.markFillColor ;
		lineStyle      = pData.lineStyle     ;
		lineWidth      = pData.lineWidth     ;
		markSize       = pData.markSize      ;
		return *this;
	}

	void Init()
	{
		szName    = _T("");
		nNameType = EN_CHART_DATANAME_POSI_REMARK;
		nNamePosiType = EN_POSITION_TYPE_CENTER_TOP;
		bLine     = TRUE;
		bMark     = TRUE;
		bFillMark = TRUE;
		markType  = EN_POINT_TYPE_DOT;
		lineColor     = RGB(  0,  0,  0); // 선색
		markLineColor = RGB(  0,  0,  0); // 표식 전경색
		markFillColor = RGB(255,255,255); // 표식 배경색
		lineStyle     = PS_SOLID;         // 선형식(PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL, PS_INSIDEFRAME)
		lineWidth     = 1;                // 선두께
		markSize      = 3;                // 표식 크기
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_FRAME

struct __MY_EXT_CLASS__ T_CHART_FRAME
{
	nrT_TEXT arText;
	nrT_LINE arRect;	
		
	T_CHART_FRAME()	{ Init(); }
	T_CHART_FRAME(const T_CHART_FRAME& pData) { *this = pData; }
	T_CHART_FRAME& operator = (const T_CHART_FRAME& pData)
	{
		arText.Copy(pData.arText);
		arRect.Copy(pData.arRect);
		return *this;
	}

	void Init()
	{
		arText.RemoveAll();  
		arRect.RemoveAll();  
	}

	void Move(int disX, int disY)
	{ 
		int i;
		for(i = 0; i < arText.GetSize(); i++) 
		{
			arText[i].Move(disX, disY);
		}
		for(i = 0; i < arRect.GetSize(); i++) 
		{
			arRect[i].Move(disX, disY);
		}
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_AXIS

struct __MY_EXT_CLASS__ T_CHART_AXIS
{
	T_TEXT title;
	
	nrT_TEXT arText;
	nrT_LINE arLine;	
	nrT_LINE arMajorGrid;
	nrT_LINE arMinorGrid;

	T_CHART_AXIS()	{ Init(); }
	T_CHART_AXIS(const T_CHART_AXIS& pData) { *this = pData; }
	T_CHART_AXIS& operator = (const T_CHART_AXIS& pData)
	{
		title = pData.title;
		arText.Copy(pData.arText);
		arLine.Copy(pData.arLine);
		arMajorGrid.Copy(pData.arMajorGrid);
		arMinorGrid.Copy(pData.arMinorGrid);
		return *this;
	}

	void Init()
	{
		title.Init();
		arText.RemoveAll();  
		arLine.RemoveAll();  
		arMajorGrid.RemoveAll();
		arMinorGrid.RemoveAll();
	}

	void Move(int disX, int disY)
	{ 
		int i;
		for(i = 0; i < arText.GetSize(); i++)
		{
			arText[i].Move(disX, disY);
		}
		for(i = 0; i < arLine.GetSize(); i++)
		{
			arLine[i].Move(disX, disY);
		}
		for(i = 0; i < arMajorGrid.GetSize(); i++)
		{
			arMajorGrid[i].Move(disX, disY);
		}
		for(i = 0; i < arMinorGrid.GetSize(); i++) 
		{
			arMinorGrid[i].Move(disX, disY);
		}
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_DATA

struct __MY_EXT_CLASS__ T_CHART_DATA
{
	nrPOINT arPoint;
	nrT_LINE arLine;

	T_LINE remarkLine;
	POINT  remarkPoint;

	T_CHART_DATA_INFO Info;

	T_CHART_DATA()	{ Init(); }
	T_CHART_DATA(const T_CHART_DATA& pData) { *this = pData; }
	T_CHART_DATA& operator = (const T_CHART_DATA& pData)
	{
		arPoint.Copy(pData.arPoint);
		arLine.Copy(pData.arLine);
		remarkLine = pData.remarkLine;
		remarkPoint = pData.remarkPoint;
		Info = pData.Info;
		return *this;
	}

	void Init()
	{
		arPoint.RemoveAll();  
		arLine.RemoveAll();
		remarkLine.Init();
		remarkPoint.x = 0;
		remarkPoint.y = 0;
		Info.Init();
	}

	void Move(int disX, int disY)
	{ 
		int i;
		for(i = 0; i < arPoint.GetSize(); i++) 
		{
			arPoint[i].x += disX;
			arPoint[i].y += disY;
		}
		for(i = 0; i < arLine.GetSize(); i++) 
		{
			arLine[i].Move(disX, disY);
		}
		
		remarkLine.Move(disX, disY);
		remarkPoint.x += disX;   
		remarkPoint.y += disY;
	}
};
typedef CArray<T_CHART_DATA, T_CHART_DATA> nrT_CHART_DATA;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_DATA_R

struct __MY_EXT_CLASS__ T_CHART_DATA_R
{	
	nrT_POINT			arPoint;
	T_CHART_DATA_INFO	Info;
	
	T_CHART_DATA_R()	{ Init(); }
	T_CHART_DATA_R(const T_CHART_DATA_R& pData) { *this = pData; }
	T_CHART_DATA_R& operator = (const T_CHART_DATA_R& pData)
	{
		arPoint.Copy(pData.arPoint);
		Info = pData.Info;
		return *this;
	}

	void Init()
	{
		arPoint.RemoveAll();  
		Info.Init();
	}
};
typedef CArray<T_CHART_DATA_R, T_CHART_DATA_R> nrT_CHART_DATA_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART

// Chart 주 구조체 --------------------------------------
struct __MY_EXT_CLASS__ T_CHART
{	
	// 그래프, 축, 그리드, 바탕, 범례정보는 변환된 단위객체들의 집합으로 가지고 있음
	// 따라서 scale이 변경되는것이 아니라면 단위객체를 생성하는 것이 아니라, 이미생성된 단위객체들 활용함
	T_CHART_FRAME frame;  // Title, 틀 구성정보
	T_CHART_FRAME remark; // Remark 구성정보	
	T_CHART_AXIS  xAxis;  // x축 구성정보
	T_CHART_AXIS  yAxis;  // y축 구성정보
	T_CHART_INFO  Info;    

	nrT_CHART_DATA arData; // 그래프 구성정보
	
	T_CHART()	{ Init(); }
	T_CHART(const T_CHART& pData) { *this = pData; }
	T_CHART& operator = (const T_CHART& pData)
	{
		frame  = pData.frame;
		remark = pData.remark;		
		xAxis  = pData.xAxis;		
		yAxis  = pData.yAxis;
		Info   = pData.Info;

		arData.Copy(pData.arData);
		return *this;
	}

	void Init()
	{
		frame.Init();  
		remark.Init();
		xAxis.Init();
		yAxis.Init();
		Info.Init();
		arData.RemoveAll();
	}

	void Move(int disX, int disY)
	{ 				
		frame.Move(disX, disY);
		remark.Move(disX, disY);	
		xAxis.Move(disX, disY); 
		yAxis.Move(disX, disY); 

		int i;
		for(i = 0; i < arData.GetSize(); i++) 
		{
			arData[i].Move(disX, disY);
		}
	}
};
typedef CArray<T_CHART, T_CHART> nrT_CHART;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_CHART_R

struct __MY_EXT_CLASS__ T_CHART_R
{	
	T_POINT          titlePosi;
	T_LINE_R         framePosi;
	T_LINE_R         remarkPosi;
	T_CHART_INFO     Info;
	nrT_CHART_DATA_R arData;
	
	T_CHART_R()	{ Init(); }
	T_CHART_R(const T_CHART_R& pData) { *this = pData; }
	T_CHART_R& operator = (const T_CHART_R& pData)
	{
		titlePosi  = pData.titlePosi;
		framePosi  = pData.framePosi;		
		remarkPosi = pData.remarkPosi;	
		Info       = pData.Info;

		arData.Copy(pData.arData);
		return *this;
	}

	void Init()
	{
		titlePosi.Init();  
		framePosi.Init();
		remarkPosi.Init();
		arData.RemoveAll(); 
	}

	void Move(double disX, double disY)
	{ 				
		titlePosi.Move(disX, disY);
		framePosi.Move(disX, disY);	
		remarkPosi.Move(disX, disY); 
	}
};
typedef CArray<T_CHART_R, T_CHART_R> nrT_CHART_R;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_HATCHING_D

struct __MY_EXT_CLASS__ T_HATCHING_D // Hatching을 그리기 위한 임시저장용 구조체 (사용자가 직접사용할 수 없으며 Hatching기능 사용시 내부적으로 만들어짐)
{
	nrPOINT      arPoint;
	nrT_LINE	 arLine;
	nrT_LINE	 arRect;
	nrT_CIRCLE	 arCircle;
	nrT_ARC		 arArc;
	nrT_POLYLINE arPolygon;
	nrT_POLYLINE arPolyline;
	
	T_HATCHING_D() { Init(); }
	T_HATCHING_D(const T_HATCHING_D& pData)	 { *this = pData; }
	T_HATCHING_D& operator = (const T_HATCHING_D& pData)
	{
		arPoint.Copy(pData.arPoint);
		arLine.Copy(pData.arLine);
		arRect.Copy(pData.arRect);
		arCircle.Copy(pData.arCircle);
		arArc.Copy(pData.arArc);
		arPolygon.Copy(pData.arPolygon);
		arPolyline.Copy(pData.arPolyline);
		return *this;
	}

	void Init()
	{
		arPoint.RemoveAll();
		arLine.RemoveAll(); 
		arRect.RemoveAll(); 
		arCircle.RemoveAll();  
		arArc.RemoveAll();  
		arPolygon.RemoveAll();
		arPolyline.RemoveAll(); 
	}

	void Move(int disX, int disY)
	{ 
		int i;
		for(i = 0; i < arPoint.GetSize(); i++)
		{
			arPoint[i].x += disX;
			arPoint[i].y += disY;
		}
		for(i = 0; i < arLine.GetSize(); i++)
		{
			arLine[i].Move(disX, disY);
		}
		for(i = 0; i < arRect.GetSize(); i++)
		{
			arRect[i].Move(disX, disY);
		}
		for(i = 0; i < arCircle.GetSize(); i++)
		{
			arCircle[i].Move(disX, disY);
		}
		for(i = 0; i < arArc.GetSize(); i++)
		{
			arArc[i].Move(disX, disY);
		}
		for(i = 0; i < arPolygon.GetSize(); i++)
		{
			arPolygon[i].Move(disX, disY);
		}
		for(i = 0; i < arPolyline.GetSize(); i++)
		{
			arPolyline[i].Move(disX, disY);
		}
	}
};
typedef CArray<T_HATCHING_D, T_HATCHING_D> nrT_HATCHING_D;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_HATCHING_D

struct __MY_EXT_CLASS__ T_SEGMENT
{	
	T_POINT P1;
	T_POINT P2;
	T_POINT Dir; 		// using p1 as the base, 벡터 
	double  Length; 	// length of the line

	T_SEGMENT() { Init(); }
	T_SEGMENT(const T_SEGMENT& pData) { *this = pData; }
	T_SEGMENT(T_POINT& pt1, T_POINT& pt2)
	{
		Init();
		P1 = pt1;  P2 = pt2;
		BuildDirection();
	}

	// Operations
	T_SEGMENT& operator = (const T_SEGMENT& segdata)
	{
		memcpy(this, &segdata, sizeof(T_SEGMENT));
		return *this;
	}
	BOOL operator == (T_SEGMENT& data) { return CompareSegment(data); }
	BOOL operator != (T_SEGMENT& data) { return ! CompareSegment(data); }

	// Function
	void Init()  { memset(this, 0, sizeof(T_SEGMENT)); }
	void Clear() { memset(this, 0, sizeof(T_SEGMENT)); }

	BOOL CompareSegment(T_SEGMENT& seg)
	{
		if((P1 == seg.P1 && P2 == seg.P2) || (P1 == seg.P2 && P2 == seg.P1)) return TRUE;
		return FALSE;
	}

	void SetPt1(const T_POINT& pt) { P1 = pt; };
	void SetPt2(const T_POINT& pt) { P2 = pt; };
	void SetDir(const T_POINT& pt) { Dir = pt; };

	inline void SetSegment(const T_POINT& p1, const T_POINT& p2) { P1 = p1; P2 = p2; }
	inline void SetSegment(const T_POINT pt[2])				      { P1 = pt[0]; P2 = pt[1]; }
	inline void BuildDirection() { Length = P1.GetLength(P2, Dir); }
	inline void SetSegmentBuild(const T_POINT& p1, const T_POINT& p2) 
	{
		P1 = p1; P2 = p2;
		Length = P1.GetLength(P2, Dir);
	}
	inline void SetSegmentBuild(const T_POINT pt[2]) 
	{
		P1 = pt[0]; P2 = pt[1];
		Length = P1.GetLength(P2, Dir);
	}

	void SlideAlong(double distance, T_POINT& newPt);
	void CalcPt2() { SlideAlong(Length, P2); };
	void Reverse(); 

	T_POINT& GetDirection() { return Dir; };
	double   GetLength()    { return Length; };
	BOOL     GetMidPoint(T_POINT& pt);    

	void SetPerpSegment();
	void SetInfiniteLength(double dLeng = 999999999); // 무한직선을 만든다. 
	void SetTranslateP1(const T_POINT& pp);  // P1을 pp로 평행이동한다(P2도 변한다) CYW++
	void SetTranslateP2(const T_POINT& pp);  // P2을 pp로 평행이동한다(P1도 변한다) CYW++
	void SetTranslateMid(const T_POINT& pp); // 중간점을 pp로 평행이동한다. (P1, P2 가 다 변한다) CYW++

	void NormalDirection(BOOL clockwise, T_POINT& result);
	void ParallelSegmentAtDistance(double distance, T_SEGMENT& seg, BOOL clockwise = TRUE);
	BOOL SameDirection(const T_SEGMENT& seg);
	BOOL SameDirection(const T_POINT& p1, const T_POINT& p2);
};
typedef CArray<T_SEGMENT, T_SEGMENT> nrT_SEGMENT;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_EX_CROSS_ID

struct T_EX_CROSS_ID
{
	int nID;
	int nBoundID;

	T_EX_CROSS_ID()	{ Init(); }
	T_EX_CROSS_ID(const T_EX_CROSS_ID& pData) { *this = pData; }
	T_EX_CROSS_ID& operator = (const T_EX_CROSS_ID& pData)
	{
		nID = pData.nID;
		nBoundID = pData.nBoundID;
		return *this;
	}

	void Init()
	{
		nID =  -1;
		nBoundID = -1; 
	}
};
typedef CArray<T_EX_CROSS_ID, T_EX_CROSS_ID> nrT_EX_CROSS_ID;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_EX_POLYLINE

struct T_EX_POLYLINE
{
	T_POLYLINE    Poly;
	T_EX_CROSS_ID StartID;
	T_EX_CROSS_ID EndID;	

	T_EX_POLYLINE()	{ Init(); }
	T_EX_POLYLINE(const T_EX_POLYLINE& pData) { *this = pData; }
	T_EX_POLYLINE& operator = (const T_EX_POLYLINE& pData)
	{
		Poly = pData.Poly;
		StartID = pData.StartID;
		EndID = pData.EndID;
		return *this;
	}

	void Init()
	{
		Poly.Init(); 
		StartID.Init();
		EndID.Init();
	}
};
typedef CArray<T_EX_POLYLINE, T_EX_POLYLINE> nrT_EX_POLYLINE;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// T_LINE_ID

struct T_LINE_ID    // Value-General Section에서 라인 정보로 사용
{
	int    v1, v2;   // Vertex ID
	double dThik;    // 라인 두께
	int    nAlign;   // 선의 Align 정보 (1:Center,2:Left,3:Right)
	double dA;       // 회전거리
	double dVF;      // 전단응력

	void Init()
	{
		v1 = v2 = 0;
		dThik = 0.0;
		nAlign = 0;
		dA = dVF = 0.0;
	}	
	void GetBoundary(T_POLYGON_R& PolygonD, double& dXmax, double& dXmin, double& dYmax, double& dYmin)
	{
		dXmax = dXmin = dYmax = dYmin = 0.0;
		int nSize = PolygonD.arPoint.GetSize();
		T_POINT vertex1, vertex2;
		BOOL bIsV1 = (v1<nSize);
		BOOL bIsV2 = (v2<nSize);

		if(bIsV1 && bIsV2)
		{
			dXmax = max(PolygonD.arPoint[v1].x, PolygonD.arPoint[v2].x);
			dXmin = min(PolygonD.arPoint[v1].x, PolygonD.arPoint[v2].x);
			dYmax = max(PolygonD.arPoint[v1].y, PolygonD.arPoint[v2].y);
			dYmin = min(PolygonD.arPoint[v1].y, PolygonD.arPoint[v2].y);
		}
		else if(bIsV1)
		{
			dXmax = dXmin = PolygonD.arPoint[v1].x;
			dYmax = dYmin = PolygonD.arPoint[v1].y;
		}	
		else if(bIsV2)
		{
			dXmax = dXmin = PolygonD.arPoint[v2].x;
			dYmax = dYmin = PolygonD.arPoint[v2].y;
		}		
	}
	BOOL IsEqual(T_LINE_ID& rData)
	{
		if(v1 != rData.v1) return FALSE;
		if(v2 != rData.v2) return FALSE;
		if(dThik  != rData.dThik ) return FALSE;
		if(nAlign != rData.nAlign) return FALSE;
		if(dA != rData.dA) return FALSE;
		if(dVF != rData.dVF) return FALSE;
		return TRUE;
	}
};
typedef CArray<T_LINE_ID, T_LINE_ID> nrT_LINE_ID;


#pragma pack(pop)
#include "HeaderPost.h"