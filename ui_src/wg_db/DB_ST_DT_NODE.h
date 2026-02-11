#ifndef __DB_ST_DT_NODE_H__
#define __DB_ST_DT_NODE_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// node ///
#define T_NODE_K unsigned int
/**
@brief 절점의 정보를 가지고 있는 구조체.
*/
struct T_NODE_D
{
	double x;    /**< 글로벌 X 좌표 */   // L
	double y;    /**< 글로벌 Y 좌표 */   // L
	double z;    /**< 글로벌 Z 좌표 */   // L
	void Initialize()
	{
		x=0.;
		y=0.;
		z=0.;
	}
	void GetData(double xyz[3])
	{
		xyz[0] = x;
		xyz[1] = y;
		xyz[2] = z;
	}
	void GetData(double& dx,double& dy,double& dz)
	{
		dx = x;
		dy = y;
		dz = z;
	}
	void SetData(double xyz[3])
	{
		x = xyz[0];
		y = xyz[1];
		z = xyz[2];
	}
	void SetData(const double& dx,const double& dy,const double& dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	void SetData(T_NODE_D rData)
	{
		x = rData.x;
		y = rData.y;
		z = rData.z;
	}
	BOOL operator==(const T_NODE_D& src) const
	{
		if (x != src.x) return FALSE;
		if (y != src.y) return FALSE;
		if (z != src.z) return FALSE;
		return TRUE;
	}
};
inline T_NODE_D operator-(const T_NODE_D& a, const T_NODE_D& b) 
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}
inline T_NODE_D operator+(const T_NODE_D& a, const T_NODE_D& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}
inline T_NODE_D operator*(const T_NODE_D& a, double s)
{
	return { a.x * s, a.y * s, a.z * s };
}
inline T_NODE_D operator*(double s, const T_NODE_D& a)
{
	return a * s;
}

struct T_NODE_UDRD_D
{
	T_NODE_K key;
	T_NODE_D data;
};
struct T_NODE_UDRD_D_VIL
{
	T_NODE_K key;
	T_NODE_K keytemp;
	T_NODE_D data;
};
struct T_NODE_UNIT
{
	T_NODE_UNIT()
	{
		nCount=3;
		for(int i=0; i<nCount; i++)nUnit[i]=D_UNITSYS_BASE_LENGTH;
		x=nUnit[0];
		y=nUnit[1];
		z=nUnit[2];
	}
	int x;
	int y;
	int z;
	int nCount;
	int nUnit[3];
};

//#ifdef _MDEMO
//  #define HASHSIZENODE 101
//#else
	#define HASHSIZENODE 100003 // CMap 성능향상을 위해 prime number로 바꿔줌
//#endif
#define D_NODE_MINNUM        1   // 입력가능한 최소 절점번호
#define D_NODE_MAXNUM   999999   // 입력가능한 최대 절점번호

#define D_NODE_NUMBERINGTYPE_SMALLEST   1
#define D_NODE_NUMBERINGTYPE_LARGEST    2
#define D_NODE_NUMBERINGTYPE_USERDEFINE 3
struct T_NODE_CREATE_F
{
	UINT m_nNodeCreate_NumberingType;           // 절점번호 생성 방법
													// == D_NODE_NUMBERINGTYPE_SMALLEST   ; 사용안된 가장 작은번호 자동부여
													// == D_NODE_NUMBERINGTYPE_LARGEST    ; 사용안된 가장 큰번호 자동부여
													// == D_NODE_NUMBERINGTYPE_USERDEFINE ; 사용자 지정번호 이후 사용안된 가장 작은번호 자동부여
	UINT m_nNodeCreate_UserNumber;              // 절점번호 사용자지정생성시 부여될 절점 번호(m_bNodeCreate_AutoNumbering == FALSE)
													// 이번호 이후 사용안된 가장 작은 번호 부여, 부여후 자동 설정
};
struct T_NODE_EDIT_F
{
	BOOL m_bNodeEdit_AllowSamePoint;            // 동일위치 절점생성 허용여부
													// (참:판단없이 무조건 생성, 거짓:존재여부 판단하여 생성)
	double m_dblNodeEdit_SamePointTolerance;    // 동일위치 판별 범위(m_bNodeCreate_AllowSamePoint == FALSE)
													// (단위계는 기본 단위계)
	double m_dblNodeEdit_InterectTolerance;     // 절점과 프레임요소의 교차점 계산의 허용오차
													// (단위계는 기본 단위계)
	double m_dblNodeEdit_CoordinateTolerance;   // 평면비교등 일반 좌표비교 허용오차
													// (단위계는 기본 단위계)
};

#pragma pack(pop)

#endif  // __DB_ST_DT_NODE_H__
