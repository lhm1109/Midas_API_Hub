#ifndef __DB_ST_DT_BNDR_H__
#define __DB_ST_DT_BNDR_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// Supports
//#define T_CONS_K T_NODE_K
#define T_CONS_KEY unsigned int
#define CONS_SIZE 7
union T_CONS_K
{
	T_CONS_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_CONS_D_431
{
	unsigned char Constraint[8];    // 0-5 까지만 사용 '0'=free '1'=fix
};
struct T_CONS_UDRD_D_431
{
	T_NODE_K key;
	T_CONS_D_431 data;
};
struct T_CONS_D
{
	unsigned char Constraint[8];    // 0-7 까지만 사용 '0'=free '1'=fix
	T_BNGR_K GroupKey;
	void Initialize()
	{
		for(int i=0; i<8; i++)Constraint[i]='0';
		GroupKey = 0;
	}
	T_CONS_D() { Initialize(); }
	T_CONS_D(const T_CONS_D& src) { *this = src; }
	T_CONS_D& operator=(const T_CONS_D& src)
	{
		for (int i = 0; i < 8; i++)Constraint[i] = src.Constraint[i];
		GroupKey = src.GroupKey;

		return *this;
	}
	BOOL operator==(const T_CONS_D& src) const
	{
		for (int i = 0; i < 8; i++)
		{
			if (Constraint[i] != src.Constraint[i]) return FALSE;
		}
		if (GroupKey != src.GroupKey) return FALSE;

		return TRUE;
	}
	void Convert431(T_CONS_D_431& rData)
	{
		memcpy((void*)Constraint, (const void*)rData.Constraint, sizeof(Constraint));
		GroupKey = 0;
	}
};
struct T_CONS_UDRD_D
{
	T_CONS_K key;
	T_CONS_D data;
};
struct T_CONS_UNIT
{
	T_CONS_UNIT()
	{
		for(int j=0; j<8; j++)Constraint[j]=D_UNITSYS_NONE;
	}
	int Constraint[8];
};
#define HASHSIZECONS 1001

// Point Spring Supports
//#define T_NSPR_K T_NODE_K
#define T_NSPR_KEY T_NODE_K
union T_NSPR_K
{
	T_NSPR_KEY keymap;
	struct
	{
		unsigned int entity : 20; // 0-1048576
		unsigned int serial : 12; // 0-4096
	}key;
};
struct T_NSPR_D_431
{
	double Spring[6];
};

struct T_NSPR_D_711
{
	double Spring[6];
	T_BNGR_K GroupKey;
};

struct T_NSPR_D_712
{
	double Spring[6];
	T_BNGR_K GroupKey;
	int nGenCompTen;
	int nDirection;
	double dVx;
	double dVy;
	double dVz;
	double dStiffness;
};

struct T_NSPR_D_810
{
	double Spring[6];
	T_BNGR_K GroupKey;

	int nGenCompTen;   // v712추가, 0:General, 1:Compression, 2: Tension, 3:Multi-linear
	int nDirection;    // v712추가, nGenCompTen!=0일때 사용
	double dVx;        // v712추가, nGenCompTen!=0일때 사용
	double dVy;        // v712추가, nGenCompTen!=0일때 사용
	double dVz;        // v712추가, nGenCompTen!=0일때 사용
	double dStiffness; // v712추가, nGenCompTen!=0일때 사용

	int nMultiLinearType; // v720추가, 0:Unsymmetric, 1:Symmetric
	double ax, ay;     // v720추가, nGenCompTen==3일때 사용
	double bx, by;     // v720추가, nGenCompTen==3일때 사용
	double cx, cy;     // v720추가, nGenCompTen==3일때 사용
	double dx, dy;     // v720추가, nGenCompTen==3일때 사용
	double ex, ey;     // v720추가, nGenCompTen==3일때 사용
	double fx, fy;     // v720추가, nGenCompTen==3일때 사용
};

struct T_NSPR_D_830
{
	double Spring[6];
	T_BNGR_K GroupKey;

	int nGenCompTen;   // v712추가, 0:General, 1:Compression, 2: Tension, 3:Multi-linear
	int nDirection;    // v712추가, nGenCompTen!=0일때 사용
	double dVx;        // v712추가, nGenCompTen!=0일때 사용
	double dVy;        // v712추가, nGenCompTen!=0일때 사용
	double dVz;        // v712추가, nGenCompTen!=0일때 사용
	double dStiffness; // v712추가, nGenCompTen!=0일때 사용

	int nMultiLinearType; // v720추가, 0:Unsymmetric, 1:Symmetric
	double ax, ay;     // v720추가, nGenCompTen==3일때 사용
	double bx, by;     // v720추가, nGenCompTen==3일때 사용
	double cx, cy;     // v720추가, nGenCompTen==3일때 사용
	double dx, dy;     // v720추가, nGenCompTen==3일때 사용
	double ex, ey;     // v720추가, nGenCompTen==3일때 사용
	double fx, fy;     // v720추가, nGenCompTen==3일때 사용

	int nFromType;    // v811추가, 0: from Point Spring Support,  1: from Surface Spring Support (converting)
	double dEffArea;  // v811추가, if (nFromType==1) 절점의 강성이 분담하는 유효 면적,  else not used.
	double dKX;        // v811추가, if (nFromType==1) 사용자가 입력한 Kx,  else not used.
	double dKY;        // v811추가, if (nFromType==1) 사용자가 입력한 Ky,  else not used.
	double dKZ;        // v811추가, if (nFromType==1) 사용자가 입력한 Kz,  else not used.
};

struct T_NSPR_D_871
{
	double Spring[6];

	BOOL	 bDamping;      // v831 추라 // MNET:XXXX-JWKWON-20140115
	double dDamping[6];

	T_BNGR_K GroupKey;

	int nGenCompTen;   // v712추가, 0:General, 1:Compression, 2: Tension, 3:Multi-linear
	int nDirection;    // v712추가, nGenCompTen!=0일때 사용
	double dVx;        // v712추가, nGenCompTen!=0일때 사용
	double dVy;        // v712추가, nGenCompTen!=0일때 사용
	double dVz;        // v712추가, nGenCompTen!=0일때 사용
	double dStiffness; // v712추가, nGenCompTen!=0일때 사용

	int nMultiLinearType; // v720추가, 0:Unsymmetric, 1:Symmetric
	double ax, ay;     // v720추가, nGenCompTen==3일때 사용
	double bx, by;     // v720추가, nGenCompTen==3일때 사용
	double cx, cy;     // v720추가, nGenCompTen==3일때 사용
	double dx, dy;     // v720추가, nGenCompTen==3일때 사용
	double ex, ey;     // v720추가, nGenCompTen==3일때 사용
	double fx, fy;     // v720추가, nGenCompTen==3일때 사용

	// MNET:XXXX-HSSHIM-20130125 : Soil Pressure Contour 표현을 위해 추가.
	int nFromType;    // v811추가, 0: from Point Spring Support,  1: from Surface Spring Support (converting)
	double dEffArea;  // v811추가, if (nFromType==1) 절점의 강성이 분담하는 유효 면적,  else not used.
	double dKX;        // v811추가, if (nFromType==1) 사용자가 입력한 Kx,  else not used.
	double dKY;        // v811추가, if (nFromType==1) 사용자가 입력한 Ky,  else not used.
	double dKZ;        // v811추가, if (nFromType==1) 사용자가 입력한 Kz,  else not used.
};

struct T_NSPR_D_883
{
	double Spring[6];

	BOOL	 bDamping;      // v831 추라 // MNET:XXXX-JWKWON-20140115
	double dDamping[6];

	T_BNGR_K GroupKey;

	int nGenCompTen;   // v712추가, 0:General, 1:Compression, 2: Tension, 3:Multi-linear
	int nDirection;    // v712추가, nGenCompTen!=0일때 사용
	double dVx;        // v712추가, nGenCompTen!=0일때 사용
	double dVy;        // v712추가, nGenCompTen!=0일때 사용
	double dVz;        // v712추가, nGenCompTen!=0일때 사용
	double dStiffness; // v712추가, nGenCompTen!=0일때 사용

	int nMultiLinearType; // 875부터 사용 안함
	double ax, ay;        // 875부터 사용 안함
	double bx, by;        // 875부터 사용 안함
	double cx, cy;        // 875부터 사용 안함
	double dx, dy;        // 875부터 사용 안함
	double ex, ey;        // 875부터 사용 안함
	double fx, fy;        // 875부터 사용 안함

	// MNET:XXXX-HSSHIM-20130125 : Soil Pressure Contour 표현을 위해 추가.
	int nFromType;    // v811추가, 0: from Point Spring Support,  1: from Surface Spring Support (converting)
	double dEffArea;  // v811추가, if (nFromType==1) 절점의 강성이 분담하는 유효 면적,  else not used.
	double dKX;        // v811추가, if (nFromType==1) 사용자가 입력한 Kx,  else not used.
	double dKY;        // v811추가, if (nFromType==1) 사용자가 입력한 Ky,  else not used.
	double dKZ;        // v811추가, if (nFromType==1) 사용자가 입력한 Kz,  else not used.

	UINT MlfcK;  // Multi-Linear Function Key
};

struct T_NSPR_D
{
	double Spring[6];

	BOOL	 bDamping;      // v831 추라 // MNET:XXXX-JWKWON-20140115
	double dDamping[6];

	T_BNGR_K GroupKey;

	int nGenCompTen;   // v712추가, 0:General, 1:Compression, 2: Tension, 3:Multi-linear
	int nDirection;    // v712추가, nGenCompTen!=0일때 사용
	double dVx;        // v712추가, nGenCompTen!=0일때 사용
	double dVy;        // v712추가, nGenCompTen!=0일때 사용
	double dVz;        // v712추가, nGenCompTen!=0일때 사용
	double dStiffness; // v712추가, nGenCompTen!=0일때 사용

	int nMultiLinearType; // 875부터 사용 안함
	double ax, ay;        // 875부터 사용 안함
	double bx, by;        // 875부터 사용 안함
	double cx, cy;        // 875부터 사용 안함
	double dx, dy;        // 875부터 사용 안함
	double ex, ey;        // 875부터 사용 안함
	double fx, fy;        // 875부터 사용 안함

	// MNET:XXXX-HSSHIM-20130125 : Soil Pressure Contour 표현을 위해 추가.
	int nFromType;    // v811추가, 0: from Point Spring Support,  1: from Surface Spring Support (converting)
	double dEffArea;  // v811추가, if (nFromType==1) 절점의 강성이 분담하는 유효 면적,  else not used.
	double dKX;        // v811추가, if (nFromType==1) 사용자가 입력한 Kx,  else not used.
	double dKY;        // v811추가, if (nFromType==1) 사용자가 입력한 Ky,  else not used.
	double dKZ;        // v811추가, if (nFromType==1) 사용자가 입력한 Kz,  else not used.

	UINT MlfcK;  // Multi-Linear Function Key
	BOOL bFixed[6];

	void Initialize()
	{
		for(int i=0; i<6; i++) { Spring[i]=0.; dDamping[i]=0.; }
		bDamping=FALSE;
		GroupKey = 0;
		nGenCompTen = 0;
		nDirection = 0;
		dVx = 0.0;
		dVy = 0.0;
		dVz = 0.0;
		dStiffness = 0.0;
		nMultiLinearType = 0;
		ax = ay = 0.0;
		bx = by = 0.0;
		cx = cy = 0.0;
		dx = dy = 0.0;
		ex = ey = 0.0;
		fx = fy = 0.0;
		nFromType = 0;
		dEffArea = 0.0;
		dKX = dKY = dKZ = 0.0;
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
	void Convert431(T_NSPR_D_431& rData)
	{
		memcpy((void*)Spring, (const void*)rData.Spring, sizeof(Spring));
		GroupKey = 0;
		nGenCompTen = 0;
		nDirection = 0;
		dVx = 0.0;
		dVy = 0.0;
		dVz = 0.0;
		dStiffness = 0.0;
		nMultiLinearType = 0;
		ax = ay = 0.0;
		bx = by = 0.0;
		cx = cy = 0.0;
		dx = dy = 0.0;
		ex = ey = 0.0;
		fx = fy = 0.0;
		nFromType = 0;
		dEffArea = 0.0;
		dKX = dKY = dKZ = 0.0;
		bDamping = FALSE;
		for(int i=0; i<6; i++) { dDamping[i]=0.; }
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
	void Convert711(T_NSPR_D_711& rData)
	{
		memcpy((void*)Spring, (const void*)rData.Spring, sizeof(Spring));
		GroupKey = rData.GroupKey;
		nGenCompTen = 0;
		nDirection = 0;
		dVx = 0.0;
		dVy = 0.0;
		dVz = 0.0;
		dStiffness = 0.0;
		nMultiLinearType = 0;
		ax = ay = 0.0;
		bx = by = 0.0;
		cx = cy = 0.0;
		dx = dy = 0.0;
		ex = ey = 0.0;
		fx = fy = 0.0;
		nFromType = 0;
		dEffArea = 0.0;
		dKX = dKY = dKZ = 0.0;
		bDamping = FALSE;
		for(int i=0; i<6; i++) { dDamping[i]=0.; }
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
	void Convert712(T_NSPR_D_712& rData)
	{
		memcpy((void*)Spring, (const void*)rData.Spring, sizeof(Spring));
		GroupKey = rData.GroupKey;
		nGenCompTen = rData.nGenCompTen;
		nDirection = rData.nDirection;
		dVx = rData.dVx;
		dVy = rData.dVy;
		dVz = rData.dVz;
		dStiffness = rData.dStiffness;
		nMultiLinearType = 0;
		ax = ay = 0.0;
		bx = by = 0.0;
		cx = cy = 0.0;
		dx = dy = 0.0;
		ex = ey = 0.0;
		fx = fy = 0.0;
		nFromType = 0;
		dEffArea = 0.0;
		dKX = dKY = dKZ = 0.0;
		bDamping = FALSE;
		for(int i=0; i<6; i++) { dDamping[i]=0.; }
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
	void Convert810(T_NSPR_D_810& rData)
	{
		memcpy((void*)Spring, (const void*)rData.Spring, sizeof(Spring));
		GroupKey = rData.GroupKey;
		nGenCompTen = rData.nGenCompTen;
		nDirection = rData.nDirection;
		dVx = rData.dVx;
		dVy = rData.dVy;
		dVz = rData.dVz;
		dStiffness = rData.dStiffness;
		nMultiLinearType = rData.nMultiLinearType;
		ax = rData.ax ;
		ay = rData.ay ;
		bx = rData.bx ;
		by = rData.by ;
		cx = rData.cx ;
		cy = rData.cy ;
		dx = rData.dx ;
		dy = rData.dy ;
		ex = rData.ex ;
		ey = rData.ey ;
		fx = rData.fx ;
		fy = rData.fy ;
		nFromType = 0;
		dEffArea = 0.0;
		dKX = dKY = dKZ = 0.0;
		bDamping = FALSE;
		for(int i=0; i<6; i++) { dDamping[i]=0.; }
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
	void Convert830(T_NSPR_D_830& rData)
	{
		memcpy((void*)Spring, (const void*)rData.Spring, sizeof(Spring));
		GroupKey = rData.GroupKey;
		nGenCompTen = rData.nGenCompTen;
		nDirection = rData.nDirection;
		dVx = rData.dVx;
		dVy = rData.dVy;
		dVz = rData.dVz;
		dStiffness = rData.dStiffness;
		nMultiLinearType = rData.nMultiLinearType;
		ax = rData.ax ;
		ay = rData.ay ;
		bx = rData.bx ;
		by = rData.by ;
		cx = rData.cx ;
		cy = rData.cy ;
		dx = rData.dx ;
		dy = rData.dy ;
		ex = rData.ex ;
		ey = rData.ey ;
		fx = rData.fx ;
		fy = rData.fy ;
		nFromType = rData.nFromType;
		dEffArea  = rData.dEffArea;
		dKX = rData.dKX;
		dKY = rData.dKY;
		dKZ = rData.dKZ;

		bDamping = FALSE;
		for(int i=0; i<6; i++) { dDamping[i]=0.; }
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
	void Convert871(T_NSPR_D_871& rData)
	{
		memcpy((void*)Spring, (const void*)rData.Spring, sizeof(Spring));
		GroupKey = rData.GroupKey;
		nGenCompTen = rData.nGenCompTen;
		nDirection = rData.nDirection;
		dVx = rData.dVx;
		dVy = rData.dVy;
		dVz = rData.dVz;
		dStiffness = rData.dStiffness;
		nMultiLinearType = rData.nMultiLinearType;
		ax = rData.ax ;
		ay = rData.ay ;
		bx = rData.bx ;
		by = rData.by ;
		cx = rData.cx ;
		cy = rData.cy ;
		dx = rData.dx ;
		dy = rData.dy ;
		ex = rData.ex ;
		ey = rData.ey ;
		fx = rData.fx ;
		fy = rData.fy ;
		nFromType = rData.nFromType;
		dEffArea  = rData.dEffArea;
		dKX = rData.dKX;
		dKY = rData.dKY;
		dKZ = rData.dKZ;

		bDamping = rData.bDamping;
		for(int i=0; i<6; i++) { dDamping[i]=rData.dDamping[i]; }
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
	void Convert883(T_NSPR_D_883& rData)
	{
		memcpy((void*)Spring, (const void*)rData.Spring, sizeof(Spring));
		GroupKey = rData.GroupKey;
		nGenCompTen = rData.nGenCompTen;
		nDirection = rData.nDirection;
		dVx = rData.dVx;
		dVy = rData.dVy;
		dVz = rData.dVz;
		dStiffness = rData.dStiffness;
		nMultiLinearType = rData.nMultiLinearType;
		ax = rData.ax ;
		ay = rData.ay ;
		bx = rData.bx ;
		by = rData.by ;
		cx = rData.cx ;
		cy = rData.cy ;
		dx = rData.dx ;
		dy = rData.dy ;
		ex = rData.ex ;
		ey = rData.ey ;
		fx = rData.fx ;
		fy = rData.fy ;
		nFromType = rData.nFromType;
		dEffArea  = rData.dEffArea;
		dKX = rData.dKX;
		dKY = rData.dKY;
		dKZ = rData.dKZ;

		bDamping = rData.bDamping;
		for(int i=0; i<6; i++) { dDamping[i]=rData.dDamping[i]; }
		MlfcK = rData.MlfcK;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
	}
};

struct T_NSPR_UDRD_D_431
{
	T_NODE_K key;
	T_NSPR_D_431 data;
};

struct T_NSPR_UDRD_D_711
{
	T_NODE_K key;
	T_NSPR_D_711 data;
};

struct T_NSPR_UDRD_D_712
{
	T_NODE_K key;
	T_NSPR_D_712 data;
};

struct T_NSPR_UDRD_D_810
{
	T_NODE_K key;
	T_NSPR_D_810 data;
};

struct T_NSPR_UDRD_D_830
{
	T_NODE_K key;
	T_NSPR_D_830 data;
};

struct T_NSPR_UDRD_D_871
{
	T_NODE_K key;
	T_NSPR_D_871 data;
};

struct T_NSPR_UDRD_D_883
{
	T_NODE_K key;
	T_NSPR_D_883 data;
};

struct T_NSPR_UDRD_D
{
	T_NSPR_K key;
	T_NSPR_D data;
};
struct T_NSPR_UNIT
{
	T_NSPR_UNIT()
	{
		Spring[0]=D_UNITSYS_BASE_SPRING;
		Spring[1]=D_UNITSYS_BASE_SPRING;
		Spring[2]=D_UNITSYS_BASE_SPRING;
		Spring[3]=D_UNITSYS_BASE_ROSPRING;
		Spring[4]=D_UNITSYS_BASE_ROSPRING;
		Spring[5]=D_UNITSYS_BASE_ROSPRING;

		dDamping[0]=D_UNITSYS_BASE_FSEC_L;
		dDamping[1]=D_UNITSYS_BASE_FSEC_L;
		dDamping[2]=D_UNITSYS_BASE_FSEC_L;
		dDamping[3]=D_UNITSYS_BASE_FLSEC_R;
		dDamping[4]=D_UNITSYS_BASE_FLSEC_R;
		dDamping[5]=D_UNITSYS_BASE_FLSEC_R;

		dVx = D_UNITSYS_BASE_LENGTH;
		dVy = D_UNITSYS_BASE_LENGTH;
		dVz = D_UNITSYS_BASE_LENGTH;
		dStiffness = D_UNITSYS_BASE_SPRING;
		ax = bx = cx = dx = ex = fx = D_UNITSYS_BASE_LENGTH;
		ay = by = cy = dy = ey = fy = D_UNITSYS_BASE_FORCE;
		dEffArea = D_UNITSYS_BASE_AREA;
		dKX = D_UNITSYS_BASE_DENSITY;   // 단위 부피당 반력 
		dKY = D_UNITSYS_BASE_DENSITY;   // 단위 부피당 반력 
		dKZ = D_UNITSYS_BASE_DENSITY;   // 단위 부피당 반력 
	}
	int Spring[6];
	int dDamping[6];
	int dVx;
	int dVy;
	int dVz;
	int dStiffness;
	int ax, bx, cx, dx, ex, fx;
	int ay, by, cy, dy, ey, fy;
	int dEffArea;
	int dKX, dKY, dKZ;
};
#define HASHSIZENSPR HASHSIZENODE


/////////////////////////////////////////////////////////////////////
// Surface Spring Support
#define T_SSPS_KEY unsigned int
union T_SSPS_K
{
	T_SSPS_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

// in case nElementType is Frame->Local Axis
enum D_SSPS_FRAME_LOCAL
{
	D_SSPS_FRAME_LOCAL_Y, D_SSPS_FRAME_LOCAL_Z, D_SSPS_FRAME_LOCAL_X
};
struct T_SSPS_D
{
	T_BNGR_K GroupKey;

	int nElementType; // 0:Frame 1:Planar(Face) 2:Planar(Edge) 3:Solid(Face)
	int nSelection;   // 0:Node  1:Element
	int nEdgeFace;    // Frame->Local Axis  : 0:Local-y 1:Loacl-z, 2:Local-x
										// Planar(Edge)->Edge : 0:Edge #1 1:Edge #2 2:Edge #3 3:Edge #4
										// Solid(Face)->Face  : 0:Face #1 1:Face #2 2:Face #3 3:Face #4 4:Face #5 5:Face #6
	double dWidth;    // Width
	
	int nSpringType;  // 0:Linear 1:Comp-Only 2:Tens-Only
	double dModulus;  // Modulus of Subgrade Reaction(Ks)

	void Initialize()
	{
		GroupKey = 0;

		nElementType = 0;
		nSelection   = 1;
		nEdgeFace    = 0;
		dWidth       = 0.;

		nSpringType = 0;
		dModulus    = 0.;
	}
};

struct T_SSPS_UDRD_D
{
	T_SSPS_K key;
	T_SSPS_D data;
};
struct T_SSPS_UNIT
{
	T_SSPS_UNIT()
	{
		dWidth = D_UNITSYS_BASE_LENGTH;
		dModulus = D_UNITSYS_BASE_DENSITY;
	}
	int dWidth;
	int dModulus;
};
#define HASHSIZESSPS 997
/////////////////////////////////////////////////////////////////////


// General Spring Type
#define T_GSTP_K unsigned int
struct T_GSTP_D
{
	CString TypeName;
	double Spring[21];
	double Mass[21];         // 6.6.0추가
	double Damping[21];      // 6.6.0추가
	unsigned int nTypeId;    // 순번
	BOOL bStiffness;         // 7.9.5 추가
	BOOL bMass;              // 7.9.5 추가
	BOOL bDamping;           // 7.9.5 추가

	void Initialize()
	{
		TypeName.Empty();
		for(int i=0; i<21; i++)    
			Spring[i] = Mass[i] = Damping[i] = 0.;
		nTypeId=0;
		bStiffness = TRUE;
		bMass = FALSE;
		bDamping = FALSE;
	}
};
struct T_GSTP_D_CH_650
{
	char TypeName[20];
	double Spring[21];
	unsigned int nTypeId;    // 순번
};
struct T_GSTP_D_CH_791
{
	char TypeName[20];
	double Spring[21];
	double Mass[21];         // 6.6.0추가
	double Damping[21];      // 6.6.0추가
	unsigned int nTypeId;    // 순번
};
struct T_GSTP_D_CH
{
	char TypeName[20];
	double Spring[21];
	double Mass[21];         // 6.6.0추가
	double Damping[21];      // 6.6.0추가
	unsigned int nTypeId;    // 순번
	BOOL bStiffness;         // 7.9.5 추가
	BOOL bMass;              // 7.9.5 추가
	BOOL bDamping;           // 7.9.5 추가

	void ConvertToString(T_GSTP_D& rData)
	{
		ConvertCharStr(TypeName, rData.TypeName, sizeof(TypeName));
		for(int i=0; i<21; i++)
		{
			rData.Spring[i]=Spring[i];
			rData.Mass[i]=Mass[i];
			rData.Damping[i]=Damping[i];
		}
		rData.nTypeId=nTypeId;
		rData.bStiffness = bStiffness;
		rData.bMass = bMass;
		rData.bDamping = bDamping;
	}
	void ConvertToChar(T_GSTP_D& rData)
	{
		ConvertStrChar(rData.TypeName, TypeName, sizeof(TypeName));
		for(int i=0; i<21; i++)
		{
			Spring[i]=rData.Spring[i];
			Mass[i]=rData.Mass[i];
			Damping[i]=rData.Damping[i];
		}
		nTypeId=rData.nTypeId;
		bStiffness = rData.bStiffness;
		bMass = rData.bMass;
		bDamping = rData.bDamping;
	}
	void Convert650(T_GSTP_D_CH_650& rData)
	{
		memcpy(TypeName, rData.TypeName, sizeof(TypeName));    
		memcpy(Spring, rData.Spring, sizeof(Spring));
		for(int i=0; i<21; i++)
		{      
			Mass[i]    = 0.;
			Damping[i] = 0.;
		}
		nTypeId=rData.nTypeId;
		bStiffness = TRUE;
		bMass = FALSE;
		bDamping = FALSE;
	}
	void Convert791(T_GSTP_D_CH_791& rData)
	{
		memcpy(TypeName, rData.TypeName, sizeof(TypeName));    
		memcpy(Spring, rData.Spring, sizeof(Spring));
		memcpy(Mass, rData.Mass, sizeof(Mass));
		memcpy(Damping, rData.Damping, sizeof(Damping));
		nTypeId=rData.nTypeId;
		bStiffness = TRUE;
		bMass = FALSE;
		bDamping = FALSE;
	}
};
struct T_GSTP_UDRD_D
{
	T_GSTP_K key;
	T_GSTP_D data;
};
struct T_GSTP_UDRD_D_CH_650
{
	T_GSTP_K key;
	T_GSTP_D_CH_650 data;
};
struct T_GSTP_UDRD_D_CH_791
{
	T_GSTP_K key;
	T_GSTP_D_CH_791 data;
};
struct T_GSTP_UDRD_D_CH
{
	T_GSTP_K key;
	T_GSTP_D_CH data;
};
struct T_GSTP_UNIT
{
	T_GSTP_UNIT()
	{
		TypeName=D_UNITSYS_NONE;
		Spring[ 0]=D_UNITSYS_BASE_SPRING;         // 1 1
		Spring[ 1]=D_UNITSYS_BASE_SPRING;         // 2 2
		Spring[ 2]=D_UNITSYS_BASE_SPRING;         // 3 3
		Spring[ 3]=D_UNITSYS_BASE_ROSPRING;       // 4 4
		Spring[ 4]=D_UNITSYS_BASE_ROSPRING;       // 5 5 
		Spring[ 5]=D_UNITSYS_BASE_ROSPRING;       // 6 6
		Spring[ 6]=D_UNITSYS_BASE_SPRING;         // 2 1
		Spring[ 7]=D_UNITSYS_BASE_SPRING;         // 3 1
		Spring[ 8]=D_UNITSYS_BASE_CPSPRING;       // 4 1
		Spring[ 9]=D_UNITSYS_BASE_CPSPRING;       // 5 1
		Spring[10]=D_UNITSYS_BASE_CPSPRING;       // 6 1
		Spring[11]=D_UNITSYS_BASE_SPRING;         // 3 2
		Spring[12]=D_UNITSYS_BASE_CPSPRING;       // 4 2
		Spring[13]=D_UNITSYS_BASE_CPSPRING;       // 5 2
		Spring[14]=D_UNITSYS_BASE_CPSPRING;       // 6 2
		Spring[15]=D_UNITSYS_BASE_CPSPRING;       // 4 3
		Spring[16]=D_UNITSYS_BASE_CPSPRING;       // 5 3
		Spring[17]=D_UNITSYS_BASE_CPSPRING;       // 6 3
		Spring[18]=D_UNITSYS_BASE_ROSPRING;       // 5 4
		Spring[19]=D_UNITSYS_BASE_ROSPRING;       // 5 5
		Spring[20]=D_UNITSYS_BASE_ROSPRING;       // 6 5

		Mass[ 0]=D_UNITSYS_BASE_MASS;         // 1 1
		Mass[ 1]=D_UNITSYS_BASE_MASS;         // 2 2
		Mass[ 2]=D_UNITSYS_BASE_MASS;         // 3 3
		Mass[ 3]=D_UNITSYS_BASE_ROMASS;       // 4 4
		Mass[ 4]=D_UNITSYS_BASE_ROMASS;       // 5 5 
		Mass[ 5]=D_UNITSYS_BASE_ROMASS;       // 6 6
		Mass[ 6]=D_UNITSYS_BASE_MASS;         // 2 1
		Mass[ 7]=D_UNITSYS_BASE_MASS;         // 3 1
		Mass[ 8]=D_UNITSYS_BASE_CPMASS;       // 4 1
		Mass[ 9]=D_UNITSYS_BASE_CPMASS;       // 5 1
		Mass[10]=D_UNITSYS_BASE_CPMASS;       // 6 1
		Mass[11]=D_UNITSYS_BASE_MASS;         // 3 2
		Mass[12]=D_UNITSYS_BASE_CPMASS;       // 4 2
		Mass[13]=D_UNITSYS_BASE_CPMASS;       // 5 2
		Mass[14]=D_UNITSYS_BASE_CPMASS;       // 6 2
		Mass[15]=D_UNITSYS_BASE_CPMASS;       // 4 3
		Mass[16]=D_UNITSYS_BASE_CPMASS;       // 5 3
		Mass[17]=D_UNITSYS_BASE_CPMASS;       // 6 3
		Mass[18]=D_UNITSYS_BASE_ROMASS;       // 5 4
		Mass[19]=D_UNITSYS_BASE_ROMASS;       // 5 5
		Mass[20]=D_UNITSYS_BASE_ROMASS;       // 6 5

		Damping[ 0]=D_UNITSYS_BASE_FSEC_L;         // 1 1
		Damping[ 1]=D_UNITSYS_BASE_FSEC_L;         // 2 2
		Damping[ 2]=D_UNITSYS_BASE_FSEC_L;         // 3 3
		Damping[ 3]=D_UNITSYS_BASE_FLSEC_R;        // 4 4
		Damping[ 4]=D_UNITSYS_BASE_FLSEC_R;        // 5 5 
		Damping[ 5]=D_UNITSYS_BASE_FLSEC_R;        // 6 6
		Damping[ 6]=D_UNITSYS_BASE_FSEC_L;         // 2 1
		Damping[ 7]=D_UNITSYS_BASE_FSEC_L;         // 3 1
		Damping[ 8]=D_UNITSYS_BASE_CPDAMPING;      // 4 1
		Damping[ 9]=D_UNITSYS_BASE_CPDAMPING;      // 5 1
		Damping[10]=D_UNITSYS_BASE_CPDAMPING;      // 6 1
		Damping[11]=D_UNITSYS_BASE_FSEC_L;         // 3 2
		Damping[12]=D_UNITSYS_BASE_CPDAMPING;      // 4 2
		Damping[13]=D_UNITSYS_BASE_CPDAMPING;      // 5 2
		Damping[14]=D_UNITSYS_BASE_CPDAMPING;      // 6 2
		Damping[15]=D_UNITSYS_BASE_CPDAMPING;      // 4 3
		Damping[16]=D_UNITSYS_BASE_CPDAMPING;      // 5 3
		Damping[17]=D_UNITSYS_BASE_CPDAMPING;      // 6 3
		Damping[18]=D_UNITSYS_BASE_FLSEC_R;        // 5 4
		Damping[19]=D_UNITSYS_BASE_FLSEC_R;        // 5 5
		Damping[20]=D_UNITSYS_BASE_FLSEC_R;        // 6 5

		nTypeId=D_UNITSYS_NONE;
	}
	int TypeName;
	int Spring[21];
	int Mass[21];
	int Damping[21];
	int nTypeId;
};
#define HASHSIZEGSTP 101

// General Spring Supports
//#define T_GSPR_K T_NODE_K
#define T_GSPR_KEY unsigned int
union T_GSPR_K
{
	T_GSPR_KEY keymap;
	struct
	{
		unsigned int entity : 20;
		unsigned int serial : 12;
	} key;
};
struct T_GSPR_D_431
{
	T_GSTP_K SpringTypeKey;
};
struct T_GSPR_D
{
	T_GSTP_K SpringTypeKey;
	T_BNGR_K GroupKey;
	void Initialize()
	{
		SpringTypeKey=0;
		GroupKey = 0;
	}
	void Convert431(T_GSPR_D_431& rData)
	{
		SpringTypeKey = rData.SpringTypeKey;
		GroupKey = 0;
	}
};
struct T_GSPR_UDRD_D_431
{
	T_GSPR_KEY key;
	T_GSPR_D_431 data;
};
struct T_GSPR_UDRD_D
{
	T_GSPR_K key;
	T_GSPR_D data;
};
struct T_GSPR_UNIT
{
	T_GSPR_UNIT()
	{
		SpringTypeKey=D_UNITSYS_NONE;
	}
	int SpringTypeKey;
};
#define HASHSIZEGSPR HASHSIZENODE

// Elastic Link
#define T_ELNK_K unsigned int
enum D_ELNK_TYPE { D_ELNK_TYPE_GENERAL=0, D_ELNK_TYPE_RIGID, D_ELNK_TYPE_TENS, D_ELNK_TYPE_COMP, D_ELNK_TYPE_MULTI, D_ELNK_TYPE_SADDLE, D_ELNK_TYPE_RAIL, D_ELNK_TYPE_NUM };
struct T_ELNK_D_431
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only
	double dBetaAngle;
	double Spring[6];
};
struct T_ELNK_D_530
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only
	double dBetaAngle;
	double Spring[6];
	T_BNGR_K GroupKey;
};
struct T_ELNK_D_761
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only
	double dBetaAngle;
	double Spring[6];
	BOOL   bShearSprLoc;
	double dDistRatioSDy;
	double dDistRatioSDz;
	T_BNGR_K GroupKey;
};
struct T_ELNK_D_785
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only =4 Multi-Linear
	double dBetaAngle;
	double Spring[6];
	BOOL   bShearSprLoc;
	double dDistRatioSDy;
	double dDistRatioSDz;
	T_BNGR_K GroupKey;
	int    nSymmetric;     // 0:Symmetric 1:Unsymmetric
	int    nGridNum;       // Symmetric(3~5), Unsymmetric(3~10)
	double dDistance[10];   
	double dForce[10];     // Dx, Dy, Dz 일때 사용
	double dMoment[10];    // Rx, Ry, Rz 일떄 사용
	int    nDirection;     // 0:Dx 1:Dy 2:Dz 3:Rx 4:Ry 5:Rz
	double dDistRatioEndI; 
};
struct T_ELNK_D_871
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only =4 Multi-Linear
	double dBetaAngle;
	double Spring[6];
	BOOL   bShearSprLoc;
	double dDistRatioSDy;
	double dDistRatioSDz;
	T_BNGR_K GroupKey;
	int    nSymmetric;     // 0:Symmetric 1:Unsymmetric
	int    nGridNum;       // Symmetric(3~5), Unsymmetric(3~10)
	double dDistance[10];  
	double dRadian[10];    
	double dForce[10];     // Dx, Dy, Dz 일때 사용
	double dMoment[10];    // Rx, Ry, Rz 일떄 사용
	int    nDirection;     // 0:Dx 1:Dy 2:Dz 3:Rx 4:Ry 5:Rz
	double dDistRatioEndI; 
};
struct T_ELNK_D_883
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only =4 Multi-Linear
	double dBetaAngle;
	double Spring[6];
	BOOL   bShearSprLoc;
	double dDistRatioSDy;
	double dDistRatioSDz;
	T_BNGR_K GroupKey;
	int    nSymmetric;     // 875부터 사용 안함
	int    nGridNum;       // 875부터 사용 안함
	double dDistance[10];  // 875부터 사용 안함
	double dRadian[10];    // 875부터 사용 안함
	double dForce[10];     // 875부터 사용 안함
	double dMoment[10];    // 875부터 사용 안함
	int    nDirection;     // 0:Dx 1:Dy 2:Dz 3:Rx 4:Ry 5:Rz
	double dDistRatioEndI; 
	UINT   MlfcK; // Multi-Linear Function
};
struct T_ELNK_D_905
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only =4 Multi-Linear =5 Saddle
	double dBetaAngle;
	double Spring[6];
	BOOL   bShearSprLoc;
	double dDistRatioSDy;
	double dDistRatioSDz;
	T_BNGR_K GroupKey;
	int    nSymmetric;     // 875부터 사용 안함
	int    nGridNum;       // 875부터 사용 안함
	double dDistance[10];  // 875부터 사용 안함
	double dRadian[10];    // 875부터 사용 안함
	double dForce[10];     // 875부터 사용 안함
	double dMoment[10];    // 875부터 사용 안함
	int    nDirection;     // 0:Dx 1:Dy 2:Dz 3:Rx 4:Ry 5:Rz
	double dDistRatioEndI;
	UINT   MlfcK; // Multi-Linear Function
	BOOL   bFixed[6];
};

struct T_ELNK_D
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	int nSubOption;  // =0 General =1 Rigid =2 Tension Only =3 Compression Only =4 Multi-Linear =5 Saddle =6 Rail-Track-Interaction
	double dBetaAngle;
	double Spring[6];
	BOOL   bShearSprLoc;
	double dDistRatioSDy;
	double dDistRatioSDz;
	T_BNGR_K GroupKey;
	int    nSymmetric;     // 875부터 사용 안함
	int    nGridNum;       // 875부터 사용 안함
	double dDistance[10];  // 875부터 사용 안함
	double dRadian[10];    // 875부터 사용 안함
	double dForce[10];     // 875부터 사용 안함
	double dMoment[10];    // 875부터 사용 안함
	int    nDirection;     // 0:Dx 1:Dy 2:Dz 3:Rx 4:Ry 5:Rz
	double dDistRatioEndI; 
	UINT   MlfcK; // Multi-Linear Function
	BOOL   bFixed[6];
	UINT   RlfcK; // Rail-Track-Interaction Function

	void Initialize()
	{
		Node1=0;
		Node2=0;
		nSubOption=0;
		for(int i=0; i<6; i++)Spring[i]=0.;
		dBetaAngle=0.;
		bShearSprLoc = TRUE;
		dDistRatioSDy = 0.5;
		dDistRatioSDz = 0.5;
		GroupKey = 0;
		nSymmetric = 0;
		nGridNum   = 3;
		for(int i=0; i<10; i++) dDistance[i] = 0.;
		for(int i=0; i<10; i++) dRadian[i] = 0.;
		for(int i=0; i<10; i++) dForce[i] = 0.;
		for(int i=0; i<10; i++) dMoment[i] = 0.;
		nDirection = 0;
		dDistRatioEndI = 0.5;
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
		RlfcK = 0;
	}
	T_ELNK_D() {}
	T_ELNK_D(const T_ELNK_D& src) { *this = src; }
	T_ELNK_D& operator=(const T_ELNK_D& src)
	{
		Node1 = src.Node1;
		Node2 = src.Node2;
		nSubOption = src.nSubOption;
		dBetaAngle = src.dBetaAngle;
		for (int i = 0; i < 6; i++)Spring[i] = src.Spring[i];
		bShearSprLoc = src.bShearSprLoc;
		dDistRatioSDy = src.dDistRatioSDy;
		dDistRatioSDz = src.dDistRatioSDz;
		GroupKey = src.GroupKey;
		nSymmetric = src.nSymmetric;
		nGridNum = src.nGridNum;
		for (int i = 0; i < 10; i++) dDistance[i] = src.dDistance[i];
		for (int i = 0; i < 10; i++) dRadian[i] = src.dRadian[i];
		for (int i = 0; i < 10; i++) dForce[i] = src.dForce[i];
		for (int i = 0; i < 10; i++) dMoment[i] = src.dMoment[i];
		nDirection = src.nDirection;
		dDistRatioEndI = src.dDistRatioEndI;
		MlfcK = src.MlfcK;
		for (int i = 0; i < 6; i++) bFixed[i] = src.bFixed[i];
		RlfcK = src.RlfcK;

		return *this;
	}
	bool operator<(const T_ELNK_D &rhs)  const // CAttrCtrl::IsConvertMElnktoRElnk 함수에서 사용
	{
		return dForce[1] < rhs.dForce[1];
	}

	void Convert431(T_ELNK_D_431& rData)
	{
		Node1 = rData.Node1;
		Node2 = rData.Node2;
		nSubOption = rData.nSubOption;
		dBetaAngle = rData.dBetaAngle;
		memcpy(Spring, rData.Spring, sizeof(Spring));
		bShearSprLoc = FALSE;
		dDistRatioSDy = 0.5;
		dDistRatioSDz = 0.5;
		GroupKey = 0;
		nSymmetric = 0;
		nGridNum   = 3;
		for(int i=0; i<10; i++) dDistance[i] = 0.;
		for(int i=0; i<10; i++) dRadian[i] = 0.;
		for(int i=0; i<10; i++) dForce[i] = 0.;
		for(int i=0; i<10; i++) dMoment[i] = 0.;
		nDirection = 0;
		dDistRatioEndI = 0.5;
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
		RlfcK = 0;
	}
	void Convert530(T_ELNK_D_530& rData)
	{
		Node1 = rData.Node1;
		Node2 = rData.Node2;
		nSubOption = rData.nSubOption;
		dBetaAngle = rData.dBetaAngle;
		memcpy(Spring, rData.Spring, sizeof(Spring));
		bShearSprLoc = FALSE;
		dDistRatioSDy = 0.5;
		dDistRatioSDz = 0.5;
		GroupKey = rData.GroupKey;
		nSymmetric = 0;
		nGridNum   = 3;
		for(int i=0; i<10; i++) dDistance[i] = 0.;
		for(int i=0; i<10; i++) dRadian[i] = 0.;
		for(int i=0; i<10; i++) dForce[i] = 0.;
		for(int i=0; i<10; i++) dMoment[i] = 0.;
		nDirection = 0;
		dDistRatioEndI = 0.5;
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
		RlfcK = 0;
	}
	void Convert761(T_ELNK_D_761& rData)
	{
		Node1 = rData.Node1;
		Node2 = rData.Node2;
		nSubOption = rData.nSubOption;
		dBetaAngle = rData.dBetaAngle;
		memcpy(Spring, rData.Spring, sizeof(Spring));
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioSDy = rData.dDistRatioSDy;
		dDistRatioSDz = rData.dDistRatioSDz;
		GroupKey = rData.GroupKey;
		nSymmetric = 0;
		nGridNum   = 3;
		for(int i=0; i<10; i++) dDistance[i] = 0.;
		for(int i=0; i<10; i++) dRadian[i] = 0.;
		for(int i=0; i<10; i++) dForce[i] = 0.;
		for(int i=0; i<10; i++) dMoment[i] = 0.;
		nDirection = 0;
		dDistRatioEndI = 0.5;
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
		RlfcK = 0;
	}
	void Convert785(T_ELNK_D_785& rData)
	{
		Node1 = rData.Node1;
		Node2 = rData.Node2;
		nSubOption = rData.nSubOption;
		dBetaAngle = rData.dBetaAngle;
		memcpy(Spring, rData.Spring, sizeof(Spring));
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioSDy = rData.dDistRatioSDy;
		dDistRatioSDz = rData.dDistRatioSDz;
		GroupKey = rData.GroupKey;
		nSymmetric = rData.nSymmetric;
		nGridNum   = rData.nGridNum;

		// nDirection 이 0,1,2 일때는 dDistance 에 저장
		// nDirection 이 3,4,5 일때는 dRadian 에 저장
		for(int i=0; i<10; i++)
		{
			if(rData.nDirection == 0 || rData.nDirection == 1 || rData.nDirection == 2)
			{
				dDistance[i] = rData.dDistance[i];
				dRadian[i] = 0.;
			}
			else if(rData.nDirection == 3 || rData.nDirection == 4 || rData.nDirection == 5)
			{
				dDistance[i] = 0.;
				dRadian[i] = rData.dDistance[i];
			}
		}
		for(int i=0; i<10; i++) dForce[i] = rData.dForce[i];
		for(int i=0; i<10; i++) dMoment[i] = rData.dMoment[i];
		nDirection = rData.nDirection;
		dDistRatioEndI = rData.dDistRatioEndI;
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
		RlfcK = 0;
	}
	void Convert871(T_ELNK_D_871& rData)
	{
		Node1 = rData.Node1;
		Node2 = rData.Node2;
		nSubOption = rData.nSubOption;
		dBetaAngle = rData.dBetaAngle;
		memcpy(Spring, rData.Spring, sizeof(Spring));
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioSDy = rData.dDistRatioSDy;
		dDistRatioSDz = rData.dDistRatioSDz;
		GroupKey = rData.GroupKey;
		nSymmetric = rData.nSymmetric;
		nGridNum   = rData.nGridNum;
		for(int i=0; i<10; i++) dDistance[i] = rData.dDistance[i];
		for(int i=0; i<10; i++) dRadian[i] = rData.dRadian[i];
		for(int i=0; i<10; i++) dForce[i] = rData.dForce[i];
		for(int i=0; i<10; i++) dMoment[i] = rData.dMoment[i];
		nDirection = rData.nDirection;
		dDistRatioEndI = rData.dDistRatioEndI;
		MlfcK = NULL;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
		RlfcK = 0;
	}
	void Convert883(T_ELNK_D_883& rData)
	{
		Node1 = rData.Node1;
		Node2 = rData.Node2;
		nSubOption = rData.nSubOption;
		dBetaAngle = rData.dBetaAngle;
		memcpy(Spring, rData.Spring, sizeof(Spring));
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioSDy = rData.dDistRatioSDy;
		dDistRatioSDz = rData.dDistRatioSDz;
		GroupKey = rData.GroupKey;
		nSymmetric = rData.nSymmetric;
		nGridNum   = rData.nGridNum;
		for(int i=0; i<10; i++) dDistance[i] = rData.dDistance[i];
		for(int i=0; i<10; i++) dRadian[i] = rData.dRadian[i];
		for(int i=0; i<10; i++) dForce[i] = rData.dForce[i];
		for(int i=0; i<10; i++) dMoment[i] = rData.dMoment[i];
		nDirection = rData.nDirection;
		dDistRatioEndI = rData.dDistRatioEndI;
		MlfcK = rData.MlfcK;
		for(int i=0; i<6; i++) bFixed[i] = FALSE;
		RlfcK = 0;
	}
	void Convert905(T_ELNK_D_905& rData)
	{
		Node1 = rData.Node1;
		Node2 = rData.Node2;
		nSubOption = rData.nSubOption;
		dBetaAngle = rData.dBetaAngle;
		memcpy(Spring, rData.Spring, sizeof(Spring));
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioSDy = rData.dDistRatioSDy;
		dDistRatioSDz = rData.dDistRatioSDz;
		GroupKey = rData.GroupKey;
		nSymmetric = rData.nSymmetric;
		nGridNum = rData.nGridNum;
		for (int i = 0; i < 10; i++) dDistance[i] = rData.dDistance[i];
		for (int i = 0; i < 10; i++) dRadian[i] = rData.dRadian[i];
		for (int i = 0; i < 10; i++) dForce[i] = rData.dForce[i];
		for (int i = 0; i < 10; i++) dMoment[i] = rData.dMoment[i];
		nDirection = rData.nDirection;
		dDistRatioEndI = rData.dDistRatioEndI;
		MlfcK = rData.MlfcK;
		for (int i = 0; i < 6; i++) bFixed[i] = rData.bFixed[i];
		RlfcK = 0;
	}
};
struct T_ELNK_UDRD_D_431
{
	T_ELNK_K key;
	T_ELNK_D_431 data;
};
struct T_ELNK_UDRD_D_530
{
	T_ELNK_K key;
	T_ELNK_D_530 data;
};
struct T_ELNK_UDRD_D_761
{
	T_ELNK_K key;
	T_ELNK_D_761 data;
};
struct T_ELNK_UDRD_D_785
{
	T_ELNK_K key;
	T_ELNK_D_785 data;
};
struct T_ELNK_UDRD_D_871
{
	T_ELNK_K key;
	T_ELNK_D_871 data;
};
struct T_ELNK_UDRD_D_883
{
	T_ELNK_K key;
	T_ELNK_D_883 data;
};
struct T_ELNK_UDRD_D_905
{
	T_ELNK_K key;
	T_ELNK_D_905 data;
};
struct T_ELNK_UDRD_D
{
	T_ELNK_K key;
	T_ELNK_D data;
};
struct T_ELNK_UNIT
{
	T_ELNK_UNIT()
	{
		Node1=D_UNITSYS_NONE;
		Node2=D_UNITSYS_NONE;
		nSubOption=D_UNITSYS_NONE;
		dBetaAngle=D_UNITSYS_BASE_DEGREE;
		Spring[0]=D_UNITSYS_BASE_SPRING;
		Spring[1]=D_UNITSYS_BASE_SPRING;
		Spring[2]=D_UNITSYS_BASE_SPRING;
		Spring[3]=D_UNITSYS_BASE_ROSPRING;
		Spring[4]=D_UNITSYS_BASE_ROSPRING;
		Spring[5]=D_UNITSYS_BASE_ROSPRING;
		dDistance = D_UNITSYS_BASE_LENGTH;
		dRadian = D_UNITSYS_BASE_RADIAN;
		dForce  = D_UNITSYS_BASE_FORCE;
		dMoment = D_UNITSYS_BASE_MOMENT;
	}
	int Node1;
	int Node2;
	int nSubOption;
	int dBetaAngle;
	int Spring[6];
	int dDistance;
	int dRadian;
	int dForce;
	int dMoment;
};
#define HASHSIZEELNK 1001

// Beam End Release
//#define T_FRLS_K T_ELEM_K
#define T_FRLS_KEY unsigned int
union T_FRLS_K
{
	T_FRLS_KEY keymap;
	struct
	{
		unsigned int entity : 20;
		unsigned int serial : 12;
	} key;
};
struct T_FRLS_D_431
{
	unsigned char Release[2][6];    // '0'=fix '1'=release
	double Fixity[2][6];
};
struct T_FRLS_UDRD_D_431
{
	T_FRLS_KEY key;
	T_FRLS_D_431 data;
};

struct T_FRLS_D_610
{
	unsigned char Release[2][6];    // '0'=fix '1'=release
	double Fixity[2][6];
	T_BNGR_K GroupKey;
};
struct T_FRLS_UDRD_D_610
{
	T_FRLS_K key;
	T_FRLS_D_610 data;
};

struct T_FRLS_D_815
{
	unsigned char Release[2][6];    // '0'=fix '1'=release
	double Fixity[2][6];
	T_BNGR_K GroupKey;
	int  nType;                     // 0:Relative, 1:Value 6.2.0에서 추가
};
struct T_FRLS_UDRD_D_815
{
	T_FRLS_K key;
	T_FRLS_D_815 data;
};

#define FRLS_SIZE 7

// nType=1인 경우 Fixity 단위 변환할 것
struct T_FRLS_D
{
	unsigned char Release[2][8];    // '0'=fix '1'=release // FRLS_SIZE까지만 사용
	double Fixity[2][8];
	T_BNGR_K GroupKey;
	int  nType;                     // 0:Relative, 1:Value 6.2.0에서 추가
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			for(int j=0; j<8; j++)
			{
				Release[i][j]='0';
				Fixity[i][j]=0.;
			}
		}
		GroupKey = 0;
		nType = 0;
	}
	BOOL operator==(const T_FRLS_D& src) const
	{
		for (int i=0; i<2; i++)
		{
			for (int j=0; j<8; j++)
			{
				if(Release[i][j] != src.Release[i][j]) return FALSE;   
				if(Fixity[i][j]  != src.Fixity[i][j])  return FALSE; 
			}
			if(GroupKey != src.GroupKey) return FALSE;
			if(nType != src.nType) return FALSE;
		}
		return TRUE;
	}
	BOOL operator!=(const T_FRLS_D& src) const
	{
		return !( *this == src );
	}
	void Convert815(T_FRLS_D_815& rData)
	{
		Initialize();
		for (int i=0; i<2; i++)
		{
			for (int j=0; j<6; j++)
			{
				Release[i][j] = rData.Release[i][j];
				Fixity[i][j] = rData.Fixity[i][j];
			}
		}
		GroupKey = rData.GroupKey;
		nType = rData.nType;
	}
	void Convert610(T_FRLS_D_610& rData)
	{
		Initialize();
		for (int i=0; i<2; i++)
		{
			for (int j=0; j<6; j++)
			{
				Release[i][j] = rData.Release[i][j];
				Fixity[i][j] = rData.Fixity[i][j];
			}
		}    
		GroupKey = rData.GroupKey;
		nType = 0;
	}
	void Convert431(T_FRLS_D_431& rData)
	{
		Initialize();
		for (int i=0; i<2; i++)
		{
			for (int j=0; j<6; j++)
			{
				Release[i][j] = rData.Release[i][j];
				Fixity[i][j] = rData.Fixity[i][j];
			}
		}
		GroupKey = 0;
		nType = 0;
	}
};
struct T_FRLS_UDRD_D
{
	T_FRLS_K key;
	T_FRLS_D data;
};
struct T_FRLS_UNIT
{
	T_FRLS_UNIT()
	{
		dFxyz = D_UNITSYS_BASE_SPRING;
		dMxyz = D_UNITSYS_BASE_ROSPRING;
	}
	int dFxyz;
	int dMxyz;
};
#define HASHSIZEFRLS 1001

// Beam End Offset
//#define T_OFFS_K T_ELEM_K
#define T_OFFS_KEY unsigned int
union T_OFFS_K
{
	T_OFFS_KEY keymap;
	struct
	{
		unsigned int entity : 20;
		unsigned int serial : 12;
	} key;
};
struct T_OFFS_D_431
{
	int nOffsetType;    // 1=GLOBAL  2=ELEMENT LOCAL 3=PANEL ZONE EFFECT
	double Offset[6];       // L,L,L,L,L,L
};
struct T_OFFS_UDRD_D_431
{
	T_OFFS_KEY key;
	T_OFFS_D_431 data;
};

enum D_OFFS_TYPE { D_OFFS_TYPE_GLOBAL = 1, D_OFFS_TYPE_ELEM, D_OFFS_TYPE_PANEL, D_OFFS_TYPE_SECT };

struct T_OFFS_D
{
	int nOffsetType;    // 1=GLOBAL  2=ELEMENT LOCAL 3=PANEL ZONE EFFECT  4=SECTION OFFSET
	double Offset[6];       // L,L,L,L,L,L
	T_BNGR_K GroupKey;
	void Initialize()
	{
		nOffsetType=0;
		for(int i=0; i<6; i++)Offset[i]=0.;
		GroupKey = 0;
	}
	BOOL operator==(const T_OFFS_D& src) const
	{
		for (int i=0; i<6; i++)
		{
			if(Offset[i] != src.Offset[i]) return FALSE;               
		}
		if(GroupKey != src.GroupKey) return FALSE;
		if(nOffsetType != src.nOffsetType) return FALSE;
		return TRUE;
	}
	BOOL operator!=(const T_OFFS_D& src) const
	{
		return !( *this == src );
	}
	void Convert431(T_OFFS_D_431& rData)
	{
		nOffsetType = rData.nOffsetType;
		memcpy(Offset, rData.Offset, sizeof(Offset));
		GroupKey = 0;
	}
	void Convert900()
	{
		if (nOffsetType == 2)
		{
			//기존의 Offset 데이터는 Element Type일 경우 RGDi, RGDj 두 개로 RGDyi, RGDzi, RGDyj, RGDzj 값을 표현
			//RGDi to RGDyi, RGDzi
			Offset[2] = Offset[1] = Offset[0];
			//RGDj to RGDyj, RGDzj
			Offset[5] = Offset[4] = Offset[3];

			Offset[3] = Offset[0] = 0.;
		}
	}
};
struct T_OFFS_UDRD_D
{
	T_OFFS_K key;
	T_OFFS_D data;
};
struct T_OFFS_UNIT
{
	T_OFFS_UNIT()
	{
		nOffsetType = D_UNITSYS_NONE;
		Offset = D_UNITSYS_BASE_LENGTH;
	}
	int nOffsetType;
	int Offset;
};
#define HASHSIZEOFFS 1001

// Plate End Release
//#define T_PRLS_K T_ELEM_K
#define T_PRLS_KEY unsigned int
union T_PRLS_K
{
	T_PRLS_KEY keymap;
	struct
	{
		unsigned int entity : 20;
		unsigned int serial : 12;
	} key;
};
struct T_PRLS_D_413
{
	unsigned char Release[4][3];    // '0'=fix '1'=release
};
struct T_PRLS_D_431
{
	unsigned char Release[4][5];    // '0'=fix '1'=release
};
struct T_PRLS_D
{
	unsigned char Release[4][5];    // '0'=fix '1'=release
	T_BNGR_K GroupKey;
	void Initialize()
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<5; j++)Release[i][j]='0';
		}
		GroupKey = 0;
	}
	void Convert413(T_PRLS_D_413& rData)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)Release[i][j]='0';
			for(int j=2; j<5; j++)Release[i][j]=rData.Release[i][j-2];
		}
		GroupKey = 0;
	}
	void Convert431(T_PRLS_D_431& rData)
	{
		memcpy(Release, rData.Release, sizeof(Release));
		GroupKey = 0;
	}
};
struct T_PRLS_UDRD_D_413
{
	T_PRLS_KEY key;
	T_PRLS_D_413 data;
};
struct T_PRLS_UDRD_D_431
{
	T_PRLS_KEY key;
	T_PRLS_D_431 data;
};
struct T_PRLS_UDRD_D
{
	T_PRLS_K key;
	T_PRLS_D data;
};
struct T_PRLS_UNIT
{
	T_PRLS_UNIT()
	{
		Release=D_UNITSYS_NONE;
	}
	int Release;
};
#define HASHSIZEPRLS 1001

// Linear Constraints
#define T_MCON_KEY unsigned int
union T_MCON_K
{
	T_MCON_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_MCON_BASE
{
	T_NODE_K NodeK;
	double dCoeff;
	double dWeight;
	int nDof;
	void Initialize()
	{
		NodeK=0;
		dCoeff=0.;
		dWeight=0.;
		nDof=0;
	}
};
#define T_MCON_DT CArray<T_MCON_BASE, T_MCON_BASE&>
struct T_MCON_D
{
	int nType;          // 0:Explicit, 1:Weighted Displacement
	UINT nSlaveType;    // 0 또는 1로 이루어진 6자리 정수 예:110001
	T_MCON_DT arSlave;
	T_BNGR_K GroupKey;

	void Initialize()
	{
		nType=0;
		nSlaveType=0;
		arSlave.RemoveAll();
		GroupKey = 0;
	}
	T_MCON_D()
	{
	}
	T_MCON_D(T_MCON_D& src)
	{
		*this = src;
	}
	T_MCON_D& operator=(T_MCON_D& src)
	{
		nType=src.nType;
		nSlaveType=src.nSlaveType;
		arSlave.Copy(src.arSlave);
		GroupKey = src.GroupKey;
		return *this;
	}
};
struct T_MCON_UDRD_D
{
	T_MCON_K key;
	T_MCON_D data;
};
struct T_MCON_ITEM_UDRD_D
{
	T_MCON_K key;
	T_MCON_BASE slavekey;
};
struct T_MCON_UDRD_D_RW
{
	T_MCON_K key;
	int nType;        
	UINT nSlaveType;  
	int nSlaveNum;
	T_BNGR_K GroupKey;

	void GetMcon(T_MCON_K& rKey, T_MCON_D& rData)
	{
		rKey.keymap=key.keymap;
		rData.nType       = nType;
		rData.nSlaveType  = nSlaveType;
		rData.GroupKey    = GroupKey;
	}
	void SetMcon(const T_MCON_K Key, const T_MCON_D& rData)
	{
		key.keymap=Key.keymap;
		nType       = rData.nType;
		nSlaveType  = rData.nSlaveType;
		GroupKey    = rData.GroupKey;
		nSlaveNum   = (int)rData.arSlave.GetSize();
	}
};
struct T_MCON_UNIT
{
	T_MCON_UNIT()
	{
		nSlaveType=D_UNITSYS_NONE;
		arSlave=D_UNITSYS_NONE;
	}
	int nSlaveType;
	int arSlave;
};
#define HASHSIZEMCON 101
#define HASHSIZEMCONITEM HASHSIZENODE

// Rigid Link
#define T_RIGD_KEY unsigned int
#define T_RIGD_DT CArray<T_NODE_K, T_NODE_K>
union T_RIGD_K
{
	T_RIGD_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_RIGD_D_431
{
	unsigned int nSlaveType;    // 0 또는 1로 이루어진 6자리 정수 예:110001
	T_RIGD_DT arSlave;
};
struct T_RIGD_D
{
	unsigned int nSlaveType;    // 0 또는 1로 이루어진 6자리 정수 예:110001
	T_RIGD_DT arSlave;
	T_BNGR_K GroupKey;
	void Initialize()
	{
		nSlaveType=0;
		arSlave.RemoveAll();
		GroupKey = 0;
	}
	void Convert431(T_RIGD_D_431& rData)
	{
		nSlaveType = rData.nSlaveType;
		arSlave.Copy(rData.arSlave);
		GroupKey = 0;
	}
	T_RIGD_D()
	{
	}
	T_RIGD_D(T_RIGD_D& src)
	{
		*this = src;
	}
	T_RIGD_D& operator=(T_RIGD_D& src)
	{
		nSlaveType=src.nSlaveType;
		arSlave.Copy(src.arSlave);
		GroupKey = src.GroupKey;
		return *this;
	}
};
struct T_RIGD_UDRD_D_431
{
	T_RIGD_K key;
	T_RIGD_D_431 data;
};
struct T_RIGD_UDRD_D
{
	T_RIGD_K key;
	T_RIGD_D data;
};
struct T_RIGD_ITEM_UDRD_D
{
	T_RIGD_K key;
	T_NODE_K slavekey;
};
struct T_RIGD_RW_431
{
	T_NODE_K Master;
	unsigned int nSlaveType;    // 0 또는 1로 이루어진 6자리 정수 예:110001
	T_NODE_K Slave;
};
struct T_RIGD_RW
{
	T_NODE_K Master;
	unsigned int nSlaveType;    // 0 또는 1로 이루어진 6자리 정수 예:110001
	T_NODE_K Slave;
	T_BNGR_K GroupKey;
	void Convert431(T_RIGD_RW_431& rData)
	{
		Master = rData.Master;
		nSlaveType = rData.nSlaveType;
		Slave = rData.Slave;
		GroupKey = 0;
	}
};
struct T_RIGD_UNIT
{
	T_RIGD_UNIT()
	{
		nSlaveType=D_UNITSYS_NONE;
		arSlave=D_UNITSYS_NONE;
	}
	int nSlaveType;
	int arSlave;
};
#define HASHSIZERIGD 101
#define HASHSIZERIGDITEM HASHSIZENODE

// Diaphragm Release
#define T_DRLS_K T_NODE_K
struct T_DRLS_D
{
	int nTemp;
	void Initialize()
	{
		nTemp=0;
	}
};
struct T_DRLS_UDRD_D
{
	T_DRLS_K key;
	T_DRLS_D data;
};
struct T_DRLS_UNIT
{
	T_DRLS_UNIT()
	{
		nTemp=D_UNITSYS_NONE;
	}
	int nTemp;
};
#define HASHSIZEDRLS HASHSIZENODE
		

#define T_PZEF_K unsigned int
struct T_PZEF_D
{
	BOOL bOffset;
	double dOffsFact;
	int nOffsType;      // 1=Panel Zone의 경계위치 사용  2=강단이격길이에 의해 조정된 위치 사용
	void Initialize()
	{
		bOffset=FALSE;
		dOffsFact=1.;
		nOffsType=1;
	}
};
struct T_PZEF_UDRD_D
{
	T_PZEF_D data;
};
struct T_PZEF_UNIT
{
	T_PZEF_UNIT()
	{
		bOffset=D_UNITSYS_NONE;
		dOffsFact=D_UNITSYS_NONE;
		nOffsType=D_UNITSYS_NONE;
	}
	int bOffset;
	int dOffsFact;
	int nOffsType;
};
#define HASHSIZEPZEF 1

// Node Local Axis
#define T_SKEW_K T_NODE_K
struct T_SKEW_D_840
{
	int nType;  // 1=Angle 2=Point 3=Vector 4= Line Vector
	double dAngle[3];
	double dPoints[3][3];
	double dVector[2][3];
	double LocalVector[3][3];
	void Initialize()
	{
		nType = 0;
		memset(dAngle,0,sizeof(dAngle));
		memset(dPoints,0,sizeof(dPoints));
		memset(dVector,0,sizeof(dVector));
		memset(LocalVector,0,sizeof(LocalVector));
	}
};
struct T_SKEW_D
{
	int nType;  // 1=Angle 2=Point 3=Vector 4= Line Vector
	int nRefType; //Line Vector 일때 사용.
	double dAngle[3];
	double dPoints[3][3];
	double dVector[2][3];
	double LocalVector[3][3];	
	int nGlobalDirection;				//nType이 4 일 경우에 사용. 0 - global-x,		1 = global-y,		2 = global-z
	int nLocalDirection;				//nType이 4 일 경우에 사용. 0 = local-x ,		1 = local-y,		2 = local-z

	void Initialize()
	{
		nType = 0;
		memset(dAngle,0,sizeof(dAngle));
		memset(dPoints,0,sizeof(dPoints));
		memset(dVector,0,sizeof(dVector));
		memset(LocalVector,0,sizeof(LocalVector));
		nGlobalDirection = 0;
		nLocalDirection = 0;
		nRefType = 0;
	}
	void Convert840(T_SKEW_D_840& rData)
	{
		nType = rData.nType;
		memcpy(dAngle,			rData.dAngle,					sizeof(dAngle));
		memcpy(dPoints,			rData.dPoints,				sizeof(dPoints));
		memcpy(dVector,			rData.dVector,				sizeof(dVector));
		memcpy(LocalVector,	rData.LocalVector,		sizeof(LocalVector));
		nGlobalDirection = 0;
		nLocalDirection = 0;
		nRefType = 0;
	}
};
struct T_SKEW_UDRD_D_840
{
	T_SKEW_K key;
	T_SKEW_D_840 data;
};
struct T_SKEW_UDRD_D
{
	T_SKEW_K key;
	T_SKEW_D data;
};
struct T_SKEW_UNIT
{
	T_SKEW_UNIT()
	{
		dAngle=D_UNITSYS_BASE_DEGREE;
		dPoints=D_UNITSYS_BASE_LENGTH;
		dVector=D_UNITSYS_NONE;
		LocalVector=D_UNITSYS_NONE;
	}
	int dAngle;
	int dPoints;
	int dVector;
	int LocalVector;
};
#define HASHSIZESKEW HASHSIZENODE


// Effective Width for Steel Composite
#define T_EWSC_KEY unsigned int
union T_EWSC_K
{
	T_EWSC_KEY keymap;
	struct
	{
		unsigned int entity : 20; // element
		unsigned int serial : 12;
	} key;
};

struct T_EWSC_D
{
	T_BNGR_K GroupKey;

	double dSlabEffWidthL_I;
	double dSlabEffWidthM_I;
	double dSlabEffWidthR_I;

	BOOL   bJ;  
	double dSlabEffWidthL_J;
	double dSlabEffWidthM_J;
	double dSlabEffWidthR_J;


	void Initialize()
	{
		GroupKey = 0;
		dSlabEffWidthL_I = 0.0;
		dSlabEffWidthM_I = 0.0;
		dSlabEffWidthR_I = 0.0;

		bJ = FALSE;
		dSlabEffWidthL_J = 0.0;
		dSlabEffWidthM_J = 0.0;
		dSlabEffWidthR_J = 0.0;
	}
};

struct T_EWSC_UDRD_D
{
	T_EWSC_K key;
	T_EWSC_D data;
};

struct T_EWSC_UNIT
{
	T_EWSC_UNIT()
	{
		dSlabEffWidthL_I = D_UNITSYS_BASE_LENGTH;
		dSlabEffWidthM_I = D_UNITSYS_BASE_LENGTH;
		dSlabEffWidthR_I = D_UNITSYS_BASE_LENGTH;
		
		dSlabEffWidthL_J = D_UNITSYS_BASE_LENGTH;
		dSlabEffWidthM_J = D_UNITSYS_BASE_LENGTH;
		dSlabEffWidthR_J = D_UNITSYS_BASE_LENGTH;
	}

	int dSlabEffWidthL_I;
	int dSlabEffWidthM_I;
	int dSlabEffWidthR_I;

	int dSlabEffWidthL_J;
	int dSlabEffWidthM_J;
	int dSlabEffWidthR_J;
};
#define HASHSIZEEWSC 1001


// Effective Width Scale Factor
#define T_EWSF_KEY unsigned int
union T_EWSF_K
{
	T_EWSF_KEY keymap;
	struct
	{
		unsigned int entity : 20; // element
		unsigned int serial : 12;
	} key;
};
struct T_EWSF_D_650
{
	double dScaleFactor;
	T_BNGR_K GroupKey;
};
struct T_EWSF_D_721
{
	double dScaleFactor;
	T_BNGR_K GroupKey;
	double dZtopFactor;        // 6.6.0추가
	double dZbotFactor;        // 6.6.0추가
};
struct T_EWSF_D_970
{
	double dScaleFactor;
	T_BNGR_K GroupKey;
	double dZtopFactor;        // 6.6.0추가
	double dZbotFactor;        // 6.6.0추가

	BOOL   bJ;                 // 7.3.0 추가
	double dScaleFactor_j;     // 7.3.0 추가
	double dZtopFactor_j;      // 7.3.0 추가
	double dZbotFactor_j;      // 7.3.0 추가
};

struct T_EWSF_SUB_D
{
	double dScaleFactor;
	double dZtopFactor;
	double dZbotFactor;

	BOOL   bJ;
	double dScaleFactor_j;
	double dZtopFactor_j;
	double dZbotFactor_j;

	void Initialize()
	{
		dScaleFactor = 1.0;
		dZtopFactor = 1.0;
		dZbotFactor = 1.0;
		bJ = FALSE;
		dScaleFactor_j = 1.0;
		dZtopFactor_j = 1.0;
		dZbotFactor_j = 1.0;
	}
};

struct T_EWSF_D
{
	T_EWSF_SUB_D EwsfSubY;
	T_EWSF_SUB_D EwsfSubZ;
	T_BNGR_K GroupKey;
	
	void Initialize()
	{
		EwsfSubY.Initialize();
		EwsfSubZ.Initialize();
		GroupKey = 0;
	}
	void Convert650(T_EWSF_D_650& rData)
	{
		EwsfSubY.dScaleFactor = rData.dScaleFactor;
		GroupKey = rData.GroupKey;
		EwsfSubY.dZtopFactor = 1.0;
		EwsfSubY.dZbotFactor = 1.0;
		EwsfSubY.dScaleFactor_j = rData.dScaleFactor;
		EwsfSubY.dZtopFactor_j  = 1.0;
		EwsfSubY.dZbotFactor_j  = 1.0;
		EwsfSubZ.Initialize();
	}
	void Convert721(T_EWSF_D_721 &rData)
	{
		EwsfSubY.dScaleFactor = rData.dScaleFactor;
		GroupKey = rData.GroupKey;
		EwsfSubY.dZtopFactor = rData.dZtopFactor;
		EwsfSubY.dZbotFactor = rData.dZbotFactor;
		EwsfSubY.bJ = FALSE;
		EwsfSubY.dScaleFactor_j = EwsfSubY.dScaleFactor;
		EwsfSubY.dZtopFactor_j  = EwsfSubY.dZtopFactor;
		EwsfSubY.dZbotFactor_j  = EwsfSubY.dZbotFactor;

		EwsfSubZ.Initialize();
	}
	void Convert970(T_EWSF_D_970& rData)
	{
		EwsfSubY.dScaleFactor = rData.dScaleFactor;
		GroupKey = rData.GroupKey;
		EwsfSubY.dZtopFactor = rData.dZtopFactor;
		EwsfSubY.dZbotFactor = rData.dZbotFactor;
		EwsfSubY.bJ = rData.bJ;
		EwsfSubY.dScaleFactor_j = rData.dScaleFactor_j;
		EwsfSubY.dZtopFactor_j = rData.dZtopFactor_j;
		EwsfSubY.dZbotFactor_j = rData.dZbotFactor_j;

		EwsfSubZ.Initialize();
	}
};
struct T_EWSF_UDRD_D_650
{
	T_EWSF_K key;
	T_EWSF_D_650 data;
};
struct T_EWSF_UDRD_D_721
{
	T_EWSF_K key;
	T_EWSF_D_721 data;
};
struct T_EWSF_UDRD_D_970
{
	T_EWSF_K key;
	T_EWSF_D_970 data;
};
struct T_EWSF_UDRD_D
{
	T_EWSF_K key;
	T_EWSF_D data;
};
struct T_EWSF_UNIT
{
	T_EWSF_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEEWSF 1001

// Story Diaphragm Group
#define T_STDG_K T_STOR_K
struct T_STDG_D
{
	T_BNGR_K GroupKey;
	void Initialize()
	{
		GroupKey=0;
	}
};
struct T_STDG_UDRD_D
{
	T_STDG_K key;
	T_STDG_D data;
};
struct T_STDG_UNIT
{
	T_STDG_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZESTDG HASHSIZESTOR
		
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
// General Link Property
#define T_NLLP_K unsigned int

struct T_NLLP_VD
{
	double dDamperStiffness; 
	double dDamping;
	double dDampExp;
	double dBracingStiffness; 
	//BOOL   bRigidBracing;       // 7.0.0부터 삭제
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	double dRefVelocity;      // 6.3.1에서 추가
	double dTemp1;            // 6.4.0 추가 : Size 맞추기 위한 임시변수
	double dTemp2;            // 6.4.0 추가 : Size 맞추기 위한 임시변수 
	int    nVEDType;          // 7.0.0 추가 0:Maxwell, 1:Kelvin, 2:Maxwell+Kelvin
	void Initialize()
	{
		dDamperStiffness = 0.0;
		dDamping = 0.0;
		dDampExp = 1.0;
		dBracingStiffness = 0.0;
		//bRigidBracing = FALSE;    
		dRefVelocity = 1.0;
		dTemp1 = 0.;
		dTemp2 = 0.;
		nVEDType = 0;
	}
	int GetVEDType(BOOL bRigidBracing, double dDamperStiffness)
	{
		if (bRigidBracing) return 1;  // Kelvin
		if (dDamperStiffness == 0.0) return 0; // Maxwel
		return 2; // Maxwel + Kelvin
	}
};
struct T_NLLP_GAP
{
	double dStiffness;
	double dOpen;  
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dOpen = 0.0;    
	}
};
struct T_NLLP_HOOK
{
	double dStiffness;
	double dOpen;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dOpen = 0.0;    
	}
};
struct T_NLLP_HS
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dYieldingExp;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dYieldStrength = 0.0;
		dPostYieldStiffRatio = 0.1;
		dYieldingExp = 2.0;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;    
	}
};
struct T_NLLP_LRBI
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dYieldStrength = 0.0;
		dPostYieldStiffRatio = 0.1;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;    
	}
};
struct T_NLLP_FPSI
{
	double dStiffness;
	double dFricCoeffSlow;
	double dFricCoeffFast;
	double dRateParam;
	double dRadius;
	double dLoopParamA;  // auto calc (1.0 - |b|)
	double dLoopParamB;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dFricCoeffSlow = 0.0;
		dFricCoeffFast = 0.0;
		dRateParam = 0.0;
		dRadius = 0.0;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;    
	}
};

struct T_NLLP_TFPSI_905
{
	// 0:Outer Top, 1:Outer Bottom, 2:Inner Top, 3:Inner Bottom
	BOOL   bSymOut;
	double dStiffness[4];
	double dFricCoeffSlow[4];
	double dFricCoeffFast[4];
	double dRateParam[4];
	double dRadius[4];
	double dStopDist[4];
};
struct T_NLLP_TFPSI
{
	// 0:Outer Top, 1:Outer Bottom, 2:Inner Top, 3:Inner Bottom
	BOOL   bSymOut;
	double dStiffness[4];
	double dFricCoeffSlow[4];
	double dFricCoeffFast[4];
	double dRateParam[4];
	double dRadius[4];
	double dStopDist[4];
	double dHeightIn;
	double dHeightOut;

	void Initialize()
	{
		bSymOut = FALSE;

		for(int i=0; i<4; i++)
		{
			dStiffness[i] = 0.0;
			dFricCoeffSlow[i] = 0.0;
			dFricCoeffFast[i] = 0.0;
			dRateParam[i] = 0.0;
			dRadius[i] = 0.0;
			dStopDist[i] = 0.0;
		}

		dHeightIn = 0.0;
		dHeightOut = 0.0;
	}
};

struct T_NLLP_DFPSI
{
	// 0:Outer Top, 1:Outer Bottom
	double dAreaPressure;           // Area for Pressure
	double dRefAxialPressure;       // Reference axial pressure(po)
	double dStiffness[2];           // Stiffness(k)
	double dCoeff_uref[2];          // High-velocity coefficient of friction at reference axial pres.(uref)
	double dRateParam[2];           // Rate Param(a)
	double dRadius[2];              // Radius of Sliding Surface(R)
	double dStopDist[2];            // Stop Distance(d)
	double dRatioCoeff_uv[2];       // Ratio of coefficient of friction at small velocity to uref(uv)
	double dRatioCoeff_up[2];       // Ratio of coefficient of friction at high pressure to uref(up)
	double dParamCoeff_ap[2];       // Parameter for pressure dependent coeff.(ap)
	double dParamCoeff_hp[2];       // Parameter for pressure dependent coeff.(hp)

	void Initialize()
	{
		dAreaPressure = 0.;
		dRefAxialPressure = 0.;
		for(int i=0; i<2; i++)
		{
			dStiffness[i] = 0.0;
			dCoeff_uref[i] = 0.0;
			dRateParam[i] = 0.0;
			dRadius[i] = 0.0;
			dStopDist[i] = 0.0;
			dRatioCoeff_uv[i] = 1.0;
			dRatioCoeff_up[i] = 1.0;
			dParamCoeff_ap[i] = 1.0;
			dParamCoeff_hp[i] = 0.0;
		}
	}
};
union T_NLLP_PROP
{
	T_NLLP_VD    VD;      // Visco-elastic Damper
	T_NLLP_GAP   GAP;     // Gap
	T_NLLP_HOOK  HOOK;    // Hook
	T_NLLP_HS    HS;      // Hysteretic System
	T_NLLP_LRBI  LRBI;    // Lead Rubber Bearing Isolator
	T_NLLP_FPSI  FPSI;    // Friction Pendulum System Isolator
	T_NLLP_TFPSI TFPSI;   // Triple Friction Pendulum System Isolator  =>Max size 
	T_NLLP_DFPSI DFPSI;   // Double Friction Pendulum System Isolator 
};

#define D_NLLP_APPTYPE_ELEM      0
#define D_NLLP_APPTYPE_FORCE     1
#define D_NLLP_APPTYPE_SEISCTRL  2

struct T_NLLP_D
{
	CString     PropName;
	int         nType;            // 0:Visco, 1:Gap, 2:Hook, 3:Hysteretic, 4:Lead Rubber, 5:Friction Pendulum                                
	char        LDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	T_NLLP_PROP NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;
	double      dTotalMass;      // 6.0.0 추가
	BOOL        bUseMass;        // 6.0.0 추가
	CString     Description;
	unsigned int nSeq;    // 순번
	int         nApplicationType;   // 6.4.0추가:  0=Element, 1=Force
	int         nElemType;          // 6.4.0추가:  0=SPG(Spring), 1=DSP(Linear Dashpot), 2=SLD(Spring and Linear Dashpot)
																	//             3=SND1(Spring and Nonlinear Dashpot2), 4=SND1(Spring and Nonlinear Dashpot2) 
	double      LDP_dEffDamping[6]; // 6.4.0추가:
	UINT        ElemKey4D2E;        // 6.5.0추가: DYNA2E (회전용수철)
	double      dOtMoment4D2E;      // 6.5.0추가: DYNA2E (전도모멘트)
	int         nTemp700;           // 7.0.0추가: Size 맞추기(VD Size 동일하므로)

	double dWeightRatio_i;    // 8.1.1추가 // MNET:XXXX-HSSHIM-20130213: Group Damping - General Link 반영 
	double dMassRatio_i  ;    // 8.1.1추가 // MNET:XXXX-HSSHIM-20130213: Group Damping - General Link 
	
	int         nICoupld;							    // 8.2.0추가: 0=주대각만 입력(기존방식), 1=6X6행렬 입력방식
	double      LDP_dEffStif_Coupled[15]; // 8.2.0추가
	double      LDP_dEffDamp_Coupled[15]; // 8.2.0추가

	int nSeisType;	// 8.5.1추가: Valid when nApplicationType is 2
					// 0=Viscous/Oil Damper, 1=Viscoelastic Damper, 2=Steel Damper, 3=Hysteresis Isolator(MSS), 4=Isolator(MSS),5=Isolator(GB)
	T_KEY nSeisKey;   // 8.5.1추가: nSeisType= 0:T_SDVI_K, 1:T_SDVE_K, 2:T_SDST_K, 3:T_SDHY_K, 4:T_SDIS_K,5:T_ASGB_K

	void Initialize()
	{
		PropName.Empty();
		nApplicationType = 1;
		nType = 0;
		nElemType = 0;
		for (int i = 0; i < 8; i++)
			LDP_cDir[i] = NDP_cDir[i] = '0';
		for (int i = 0; i < 6; i++)
		{
			LDP_dEffStiff[i] = 0.0;
			LDP_dEffDamping[i] = 0.0;
			NDP_Prop[i].TFPSI.Initialize(); // 제일 큰 사이즈 초기화, 대화상자에서 초기화 다시 한다;;;;
		}
		bShearSprLoc = FALSE;
		dDistRatioU2 = 0.5;
		dDistRatioU3 = 0.5;
		dTotalWeight = 0.0;
		dTotalMass = 0.0;
		bUseMass = FALSE;
		Description.Empty();
		nSeq=0;
		ElemKey4D2E = 0;
		dOtMoment4D2E = 0.;
		nTemp700 = 0;

		dWeightRatio_i = 0.5;
		dMassRatio_i   = 0.5;

		nICoupld = 0;
		for(int i=0; i<15; i++ )
		{
			LDP_dEffStif_Coupled[i] = 0.0;
			LDP_dEffDamp_Coupled[i] = 0.0;
		}
		nSeisType=0;
		nSeisKey =0;
	}
};
struct T_NLLP_UDRD_D
{
	T_NLLP_K key;
	T_NLLP_D data;
};
struct T_NLLP_UNIT
{
	T_NLLP_UNIT()
	{
		LDP_dEffStiff[0] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[1] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[2] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[3] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStiff[4] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStiff[5] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffDamping[0] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamping[1] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamping[2] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamping[3] = D_UNITSYS_BASE_FLSEC_R;
		LDP_dEffDamping[4] = D_UNITSYS_BASE_FLSEC_R;
		LDP_dEffDamping[5] = D_UNITSYS_BASE_FLSEC_R;
		dTotalWeight = D_UNITSYS_BASE_WEIGHT;
		dTotalMass = D_UNITSYS_BASE_MASS;
		dStiffnessD = D_UNITSYS_BASE_SPRING;
		dStiffnessR = D_UNITSYS_BASE_ROSPRING;
		//dDampingD = D_UNITSYS_BASE_FSEC_L;
		//dDampingR = D_UNITSYS_BASE_FLSEC_R;
		dDampingD = D_UNITSYS_BASE_FORCE;     // 6.3.1에서 변경
		dDampingR = D_UNITSYS_BASE_MOMENT;    // 6.3.1에서 변경
		dOpenD = D_UNITSYS_BASE_LENGTH;
		dOpenR = D_UNITSYS_BASE_RADIAN;
		dYieldStrengthD = D_UNITSYS_BASE_FORCE;
		dYieldStrengthR = D_UNITSYS_BASE_MOMENT;
		dRadius = D_UNITSYS_BASE_LENGTH;
//    dEffectiveDampingD = D_UNITSYS_BASE_FSEC_L;      // 6.4.0부터 사용 않음
//    dEffectiveDampingR = D_UNITSYS_BASE_FLSEC_R;     // 6.4.0부터 사용 않음
		dRefVelocityD      = D_UNITSYS_BASE_VELOCITY;
		dRefVelocityR      = D_UNITSYS_BASE_FREQUENCY;
		dRateParam         = D_UNITSYS_BASE_SEC_L;
		dOtMoment4D2E      = D_UNITSYS_BASE_MOMENT;
		LDP_dEffStif_Coupled[ 0] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStif_Coupled[ 1] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStif_Coupled[ 2] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[ 3] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[ 4] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[ 5] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStif_Coupled[ 6] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[ 7] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[ 8] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[ 9] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[10] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[11] = D_UNITSYS_BASE_CPSPRING;
		LDP_dEffStif_Coupled[12] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStif_Coupled[13] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStif_Coupled[14] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffDamp_Coupled[ 0] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamp_Coupled[ 1] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamp_Coupled[ 2] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[ 3] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[ 4] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[ 5] = D_UNITSYS_BASE_FSEC_L;		
		LDP_dEffDamp_Coupled[ 6] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[ 7] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[ 8] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[ 9] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[10] = D_UNITSYS_BASE_CPDAMPING;
		LDP_dEffDamp_Coupled[11] = D_UNITSYS_BASE_CPDAMPING;		
		LDP_dEffDamp_Coupled[12] = D_UNITSYS_BASE_FLSEC_R;
		LDP_dEffDamp_Coupled[13] = D_UNITSYS_BASE_FLSEC_R;
		LDP_dEffDamp_Coupled[14] = D_UNITSYS_BASE_FLSEC_R;
		dStopDist = D_UNITSYS_BASE_LENGTH;
		dAreaPressure = D_UNITSYS_BASE_AREA;
		dRefAxialPressure = D_UNITSYS_BASE_PRESSURE;
		dHeightIn = D_UNITSYS_BASE_LENGTH;
		dHeightOut = D_UNITSYS_BASE_LENGTH;
	}
	int LDP_dEffStiff[6];
	int LDP_dEffDamping[6];
	int dTotalWeight;
	int dTotalMass;
	int dStiffnessD;
	int dStiffnessR;
	int dDampingD;
	int dDampingR;
	int dOpenD;
	int dOpenR;
	int dYieldStrengthD;
	int dYieldStrengthR;
	int dRadius;
//  int dEffectiveDampingD;
//  int dEffectiveDampingR;
	int dRefVelocityD;
	int dRefVelocityR;
	int dRateParam;
	int dOtMoment4D2E;
	int LDP_dEffStif_Coupled[15];
	int LDP_dEffDamp_Coupled[15];
	int dStopDist;
	int dAreaPressure;
	int dRefAxialPressure;
	int dHeightIn;
	int dHeightOut;
};
#define HASHSIZENLLP 11

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/*
// General Link Property
#define T_NLLP_K unsigned int

struct T_NLLP_VD_630
{
	double dDamperStiffness; 
	double dDamping;
	double dDampExp;
	double dBracingStiffness; 
	BOOL   bRigidBracing;
	double dEffectiveDamping;
};
struct T_NLLP_VD
{
	double dDamperStiffness; 
	double dDamping;
	double dDampExp;
	double dBracingStiffness; 
	BOOL   bRigidBracing;
	double dEffectiveDamping;
	double dRefVelocity;      // 6.3.1에서 추가
	void Initialize()
	{
		dDamperStiffness = 0.0;
		dDamping = 0.0;
		dDampExp = 1.0;
		dBracingStiffness = 0.0;
		bRigidBracing = FALSE;
		dEffectiveDamping = 0.0;
		dRefVelocity = 1.0;
	}
	void Convert630(const T_NLLP_VD_630& rData)
	{
		dDamperStiffness  = rData.dDamperStiffness;
		dDamping          = rData.dDamping;
		dDampExp          = rData.dDampExp;
		dBracingStiffness = rData.dBracingStiffness;
		bRigidBracing     = rData.bRigidBracing;
		dEffectiveDamping = rData.dEffectiveDamping;
		dRefVelocity      = 1.0;
	}
};
struct T_NLLP_GAP
{
	double dStiffness;
	double dOpen;
	double dEffectiveDamping;
	void Initialize()
	{
		dStiffness = 0.0;
		dOpen = 0.0;
		dEffectiveDamping = 0.0;
	}
};
struct T_NLLP_HOOK
{
	double dStiffness;
	double dOpen;
	double dEffectiveDamping;
	void Initialize()
	{
		dStiffness = 0.0;
		dOpen = 0.0;
		dEffectiveDamping = 0.0;
	}
};
struct T_NLLP_HS
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dYieldingExp;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
	double dEffectiveDamping;
	void Initialize()
	{
		dStiffness = 0.0;
		dYieldStrength = 0.0;
		dPostYieldStiffRatio = 0.1;
		dYieldingExp = 2.0;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;
		dEffectiveDamping = 0.0;
	}
};
struct T_NLLP_LRBI
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
	double dEffectiveDamping;
	void Initialize()
	{
		dStiffness = 0.0;
		dYieldStrength = 0.0;
		dPostYieldStiffRatio = 0.1;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;
		dEffectiveDamping = 0.0;
	}
};
struct T_NLLP_FPSI
{
	double dStiffness;
	double dFricCoeffSlow;
	double dFricCoeffFast;
	double dRateParam;
	double dRadius;
	double dLoopParamA;  // auto calc (1.0 - |b|)
	double dLoopParamB;
	double dEffectiveDamping;
	void Initialize()
	{
		dStiffness = 0.0;
		dFricCoeffSlow = 0.0;
		dFricCoeffFast = 0.0;
		dRateParam = 0.0;
		dRadius = 0.0;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;
		dEffectiveDamping = 0.0;
	}
};
union T_NLLP_PROP_630
{
	T_NLLP_VD_630 VD;
	T_NLLP_GAP    GAP;
	T_NLLP_HOOK   HOOK;
	T_NLLP_HS     HS;
	T_NLLP_LRBI   LRBI;
	T_NLLP_FPSI   FPSI;
};
union T_NLLP_PROP
{
	T_NLLP_VD   VD;     // Visco-elastic Damper
	T_NLLP_GAP  GAP;    // Gap
	T_NLLP_HOOK HOOK;   // Hook
	T_NLLP_HS   HS;     // Hysteretic System
	T_NLLP_LRBI LRBI;   // Lead Rubber Bearing Isolator
	T_NLLP_FPSI FPSI;   // Friction Pendulum System Isolator
	void Convert630(const T_NLLP_PROP_630& rData, int nType, int ix)
	{
		switch(nType)
		{
		case 0: VD.Convert630(rData.VD); break;
		case 1: GAP   = rData.GAP; break;
		case 2: HOOK  = rData.HOOK; break;
		case 3: HS    = rData.HS; break;
		case 4: 
			if (ix == 0 || ix == 3 || ix == 4 || ix == 5) GAP = rData.GAP;
			else LRBI  = rData.LRBI; 
			break;
		case 5: 
			if (ix == 0 || ix == 3 || ix == 4 || ix == 5) GAP = rData.GAP;
			else FPSI  = rData.FPSI; 
			break;
		}
	}
};

struct T_NLLP_D
{
	CString     PropName;
	int         nType;            // 0:Visco, 1:Gap, 2:Hook, 3:Hysteretic, 4:Lead Rubber, 5:Friction Pendulum
	char        LDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	T_NLLP_PROP NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;
	double      dTotalMass;      // 6.0.0 추가
	BOOL        bUseMass;        // 6.0.0 추가
	CString     Description;
	unsigned int nSeq;    // 순번
	void Initialize()
	{
		PropName.Empty();
		nType = 0;
		for (int i = 0; i < 8; i++)
			LDP_cDir[i] = NDP_cDir[i] = '0';
		for(int i = 0; i < 6; i++) 
		{
			LDP_dEffStiff[i] = 0.0;
			NDP_Prop[i].VD.Initialize();
		}
		bShearSprLoc = FALSE;
		dDistRatioU2 = 0.5;
		dDistRatioU3 = 0.5;
		dTotalWeight = 0.0;
		dTotalMass = 0.0;
		bUseMass = FALSE;
		Description.Empty();
		nSeq=0;
	}
};
struct T_NLLP_D_CH_590
{
	char        PropName[20];
	int         nType;            // 0:Visco, 1:Gap, 2:Hook, 3:Hysteretic, 4:Lead Rubber, 5:Friction Pendulum
	char        LDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	T_NLLP_PROP_630 NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;
	char        Description[80];
	unsigned int nSeq;    // 순번
};
struct T_NLLP_D_CH_630
{
	char        PropName[20];
	int         nType;            // 0:Visco, 1:Gap, 2:Hook, 3:Hysteretic, 4:Lead Rubber, 5:Friction Pendulum
	char        LDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	T_NLLP_PROP_630 NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;  
	double      dTotalMass;      // 6.0.0 추가
	BOOL        bUseMass;        // 6.0.0 추가  
	char        Description[80];
	unsigned int nSeq;    // 순번
};
struct T_NLLP_D_CH
{
	char        PropName[20];
	int         nType;
	char        LDP_cDir[8];
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];
	T_NLLP_PROP NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;  
	double      dTotalMass;       // 6.0.0 추가
	BOOL        bUseMass;         // 6.0.0 추가
	char        Description[80];
	unsigned int nSeq;
	int         n631Mark;         // 6.3.1에서 추가(Size 구분용)
	void ConvertToString(T_NLLP_D& rData)
	{
		ConvertCharStr(PropName, rData.PropName, sizeof(PropName));
		rData.nType = nType;
		memcpy(rData.LDP_cDir, LDP_cDir, sizeof(LDP_cDir));
		memcpy(rData.LDP_dEffStiff, LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(rData.NDP_cDir, NDP_cDir, sizeof(NDP_cDir));
		memcpy(rData.NDP_Prop, NDP_Prop, sizeof(NDP_Prop));
		rData.bShearSprLoc = bShearSprLoc;
		rData.dDistRatioU2 = dDistRatioU2;
		rData.dDistRatioU3 = dDistRatioU3;
		rData.dTotalWeight = dTotalWeight;
		rData.dTotalMass   = dTotalMass;
		rData.bUseMass = bUseMass;
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nSeq=nSeq;
	}
	void ConvertToChar(T_NLLP_D& rData)
	{
		ConvertStrChar(rData.PropName, PropName, sizeof(PropName));
		nType = rData.nType;
		memcpy(LDP_cDir, rData.LDP_cDir, sizeof(LDP_cDir));
		memcpy(LDP_dEffStiff, rData.LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(NDP_cDir, rData.NDP_cDir, sizeof(NDP_cDir));
		memcpy(NDP_Prop, rData.NDP_Prop, sizeof(NDP_Prop));
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioU2 = rData.dDistRatioU2;
		dDistRatioU3 = rData.dDistRatioU3;
		dTotalWeight = rData.dTotalWeight;
		dTotalMass   = rData.dTotalMass;
		bUseMass = rData.bUseMass;
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nSeq=rData.nSeq;
		n631Mark = 0;
	}
	void Convert590(T_NLLP_D_CH_590& rData)
	{
		memcpy(PropName, rData.PropName, sizeof(PropName));    
		nType = rData.nType;
		memcpy(LDP_cDir, rData.LDP_cDir, sizeof(LDP_cDir));
		memcpy(LDP_dEffStiff, rData.LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(NDP_cDir, rData.NDP_cDir, sizeof(NDP_cDir));
		for (int i = 0; i < 6; i++)
			NDP_Prop[i].Convert630(rData.NDP_Prop[i], nType, i);
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioU2 = rData.dDistRatioU2;
		dDistRatioU3 = rData.dDistRatioU3;
		dTotalWeight = rData.dTotalWeight;
		dTotalMass   = 0.;
		bUseMass = FALSE;
		memcpy(Description, rData.Description, sizeof(Description));
		nSeq=rData.nSeq;
	}
	void Convert630(T_NLLP_D_CH_630& rData)
	{
		memcpy(PropName, rData.PropName, sizeof(PropName));    
		nType = rData.nType;
		memcpy(LDP_cDir, rData.LDP_cDir, sizeof(LDP_cDir));
		memcpy(LDP_dEffStiff, rData.LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(NDP_cDir, rData.NDP_cDir, sizeof(NDP_cDir));
		for (int i = 0; i < 6; i++)
			NDP_Prop[i].Convert630(rData.NDP_Prop[i], nType, i);
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioU2 = rData.dDistRatioU2;
		dDistRatioU3 = rData.dDistRatioU3;
		dTotalWeight = rData.dTotalWeight;
		dTotalMass   = rData.dTotalMass;
		bUseMass     = rData.bUseMass;
		memcpy(Description, rData.Description, sizeof(Description));
		nSeq=rData.nSeq;
	}
};
struct T_NLLP_UDRD_D
{
	T_NLLP_K key;
	T_NLLP_D data;
};
struct T_NLLP_UDRD_D_CH_590
{
	T_NLLP_K key;
	T_NLLP_D_CH_590 data;
};
struct T_NLLP_UDRD_D_CH_630
{
	T_NLLP_K key;
	T_NLLP_D_CH_630 data;
};
struct T_NLLP_UDRD_D_CH
{
	T_NLLP_K key;
	T_NLLP_D_CH data;
};
struct T_NLLP_UNIT
{
	T_NLLP_UNIT()
	{
		LDP_dEffStiff[0] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[1] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[2] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[3] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStiff[4] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStiff[5] = D_UNITSYS_BASE_ROSPRING;
		dTotalWeight = D_UNITSYS_BASE_WEIGHT;
		dTotalMass = D_UNITSYS_BASE_MASS;
		dStiffnessD = D_UNITSYS_BASE_SPRING;
		dStiffnessR = D_UNITSYS_BASE_ROSPRING;
		//dDampingD = D_UNITSYS_BASE_FSEC_L;
		//dDampingR = D_UNITSYS_BASE_FLSEC_R;
		dDampingD = D_UNITSYS_BASE_FORCE;     // 6.3.1에서 변경
		dDampingR = D_UNITSYS_BASE_MOMENT;    // 6.3.1에서 변경
		dOpenD = D_UNITSYS_BASE_LENGTH;
		dOpenR = D_UNITSYS_BASE_RADIAN;
		dYieldStrengthD = D_UNITSYS_BASE_FORCE;
		dYieldStrengthR = D_UNITSYS_BASE_MOMENT;
		dRadius = D_UNITSYS_BASE_LENGTH;
		dEffectiveDampingD = D_UNITSYS_BASE_FSEC_L;
		dEffectiveDampingR = D_UNITSYS_BASE_FLSEC_R;
		dRefVelocityD      = D_UNITSYS_BASE_VELOCITY;
		dRefVelocityR      = D_UNITSYS_BASE_FREQUENCY;
		dRateParam         = D_UNITSYS_BASE_SEC_L;
	}
	int LDP_dEffStiff[6];
	int dTotalWeight;
	int dTotalMass;
	int dStiffnessD;
	int dStiffnessR;
	int dDampingD;
	int dDampingR;
	int dOpenD;
	int dOpenR;
	int dYieldStrengthD;
	int dYieldStrengthR;
	int dRadius;
	int dEffectiveDampingD;
	int dEffectiveDampingR;
	int dRefVelocityD;
	int dRefVelocityR;
	int dRateParam;
};
#define HASHSIZENLLP 11
*/

// General Link
#define T_NLNK_K unsigned int
struct T_NLNK_D_632
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	T_NLLP_K PropKey;
	double dBetaAngle;
	T_BNGR_K GroupKey;
};
struct T_NLNK_D_640
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	T_NLLP_K PropKey;  
	T_BNGR_K GroupKey;  
	double   dBetaAngle;      
	UINT     IehpKey;           // 6.4.0 추가
	int      nRefSystem;        // 6.4.0 추가  : 0=Element, 1=Global  
	int      nRotType;          // 6.4.0 추가  : 0=Angle 1=Point 2=Vector    
	double   dAngle[3];         // 6.4.0 추가
	double   dPoints[3][3];     // 6.4.0 추가
	double   dVector[2][3];     // 6.4.0 추가
	double   LocalVector[3][3]; // 6.4.0 추가  
};
struct T_NLNK_D
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	T_NLLP_K PropKey;  
	T_BNGR_K GroupKey;  
	double   dBetaAngle;      
	UINT     IehpKey;           // 6.4.0 추가
	int      nRefSystem;        // 6.4.0 추가  : 0=Element, 1=Global  
	int      nRotType;          // 6.4.0 추가  : 0=Angle 1=Point 2=Vector    
	double   dAngle[3];         // 6.4.0 추가
	double   dPoints[3][3];     // 6.4.0 추가
	double   dVector[2][3];     // 6.4.0 추가
	double   LocalVector[3][3]; // 6.4.0 추가    
	T_ELEM_K ElemKeyD2E;        // 6.5.0 추가(DYNA2E)
	void Initialize()
	{
		Node1=0;
		Node2=0;
		PropKey=0;    
		GroupKey = 0;
		dBetaAngle=0.0;
		IehpKey  = 0;    
		nRefSystem = 0;       
		nRotType = 0;
		memset(dAngle,0,sizeof(dAngle));
		memset(dPoints,0,sizeof(dPoints));
		memset(dVector,0,sizeof(dVector));
		memset(LocalVector,0,sizeof(LocalVector));
		ElemKeyD2E = 0;
	}
	void Convert632(T_NLNK_D_632& rData)
	{
		Node1       = rData.Node1;
		Node2       = rData.Node2;
		PropKey     = rData.PropKey;
		dBetaAngle  = rData.dBetaAngle;
		GroupKey    = rData.GroupKey;
		IehpKey     = 0;    
		nRefSystem  = 0;     
		nRotType    = 0;
		memset(dAngle,0,sizeof(dAngle));
		memset(dPoints,0,sizeof(dPoints));
		memset(dVector,0,sizeof(dVector));
		memset(LocalVector,0,sizeof(LocalVector));
		ElemKeyD2E = 0;
	}
	void Convert640(T_NLNK_D_640& rData)
	{
		Node1       = rData.Node1;
		Node2       = rData.Node2;
		PropKey     = rData.PropKey;
		dBetaAngle  = rData.dBetaAngle;
		GroupKey    = rData.GroupKey;
		IehpKey     = rData.IehpKey;    
		nRefSystem  = rData.nRefSystem;     
		nRotType    = rData.nRotType;
		memcpy(dAngle,rData.dAngle,sizeof(dAngle));
		memcpy(dPoints,rData.dPoints,sizeof(dPoints));
		memcpy(dVector,rData.dVector,sizeof(dVector));
		memcpy(LocalVector,rData.LocalVector,sizeof(LocalVector));
		ElemKeyD2E = 0;
	}
};
struct T_NLNK_UDRD_D_632
{
	T_NLNK_K key;
	T_NLNK_D_632 data;
};
struct T_NLNK_UDRD_D_640
{
	T_NLNK_K key;
	T_NLNK_D_640 data;
};
struct T_NLNK_UDRD_D
{
	T_NLNK_K key;
	T_NLNK_D data;
};
struct T_NLNK_UNIT
{
	T_NLNK_UNIT()
	{
		dBetaAngle=D_UNITSYS_BASE_DEGREE;
		dAngle=D_UNITSYS_BASE_DEGREE;
		dPoints=D_UNITSYS_BASE_LENGTH;
		dVector=D_UNITSYS_NONE;
		LocalVector=D_UNITSYS_NONE;
	}
	int dBetaAngle;
	int dAngle;
	int dPoints;
	int dVector;
	int LocalVector;
};
#define HASHSIZENLNK 1001

// Change General Link Property
#define T_CGLP_K unsigned int
struct T_CGLP_D
{
	T_BNGR_K GroupKey;    // Boundary Group Key
	T_NLNK_K GLinkK;      // General Link Key
	T_NLLP_K GLinkPropK;  // General Link Property Key
	void Initialize()
	{
		GroupKey = 0;
		GLinkK = 0;
		GLinkPropK = 0;
	}
};
struct T_CGLP_UDRD_D
{
	T_CGLP_K key;
	T_CGLP_D data;
};
struct T_CGLP_UNIT
{
	T_CGLP_UNIT()
	{
		None=D_UNITSYS_NONE;
	}
	int None;
};
#define HASHSIZECGLP  1001

// Constraint Label Direction
#define T_CLDR_K  T_NODE_K
#define D_CLDR_LXP  0
#define D_CLDR_LXM  1
#define D_CLDR_LYP  2
#define D_CLDR_LYM  3
#define D_CLDR_LZP  4
#define D_CLDR_LZM  5
struct T_CLDR_D
{
	int nDir; 
	// 0:Local x(+):D_CLDR_LXP, 1:Local x(-):D_CLDR_LXM,
	// 2:Local y(+):D_CLDR_LYP, 3:Local y(-):D_CLDR_LYM,
	// 4:Local z(+):D_CLDR_LZP, 5:Local z(-):D_CLDR_LZM
	void Initialize()
	{
		nDir = D_CLDR_LZP;
	}
};
struct T_CLDR_UDRD_D
{
	T_CLDR_K key;
	T_CLDR_D data;
};
struct T_CLDR_UNIT
{
	T_CLDR_UNIT()
	{
		None=D_UNITSYS_NONE;
	}
	int None;
};
#define HASHSIZECLDR  HASHSIZENODE

/////////////////////////////////////////////////////////////////////
// Soil Pressure 계산하기 위한 유효면적 데이터 [PMS 5123] 2015-08-13 : JWKWON
#define T_EARE_K unsigned int
struct T_EARE_D
{
	T_NODE_K NodeK;
	T_ELEM_K ElemK;

	double dEffectiveArea;    // 유효면적

	void Initialize()
	{
		NodeK = 0;
		ElemK = 0;

		dEffectiveArea  = 0.;
	}
};

struct T_EARE_UDRD_D
{
	T_EARE_K key;
	T_EARE_D data;
};
struct T_EARE_UNIT
{
	T_EARE_UNIT()
	{
		dEffectiveArea = D_UNITSYS_BASE_AREA;
	}
	int dEffectiveArea;
};
#define HASHSIZEEARE 997
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
// Seismic device properties - Vicous damper/Oil damper
#define T_SDVI_K unsigned int

struct T_SDVI_COMP_860
{
	BOOL   bDof;
	double Ce;
	double P1;
	double C1;
	double Alpha1;
	double K0;

	void Initialize()
	{
		bDof   = FALSE;
		Ce     = 0.;
		P1     = 0.;
		C1     = 0.;
		Alpha1 = 0.;
		K0     = 0.;
	}
};
struct T_SDVI_COMP
{
	BOOL   bDof;
	double Ce;
	double P1;
	double C1;
	double Alpha1;
	double K0;
	double Exfn_Py;
	double Exfn_Vy;
	double Exfn_DE;
	double Exfn_DC;
	double Exfn_Ce;
	BOOL   bExfn_Ce;

	void Initialize()
	{
		bDof   = FALSE;
		Ce     = 0.;
		P1     = 0.;
		C1     = 0.;
		Alpha1 = 0.;
		K0     = 0.;
		Exfn_Py  = 1.0;
		Exfn_Vy  = 1.0;
		Exfn_DE  = 0.3;
		Exfn_DC  = 1.0;
		Exfn_Ce  = 1.0;
		bExfn_Ce = FALSE;
	}
};

struct T_SDVI_D_851
{
	CString     strName;
	CString     strDesc;
	int         nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	CString     strDeviceType;
	CString     strProductName;
	CString     strTypeNumber;
	int         nDamperType;      // 0:Single Dashpot Model, 1:Kelvin(Voigt) Model, 2:Maxwell Model
	int         nDashpotType;     // 0:Linear Elastic Type, 1:Elastic Bilinear Type, 2:Exponential Function Type
	int         nInputType;       // 0:Damping Reduction Ratio(α1,α2), 1:Damping(C1,C2)
	T_SDVI_COMP_860 CompProp[6];
};
struct T_SDVI_D_860
{
	CString     strName;
	CString     strDesc;
	int         nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	CString     strDeviceType;
	CString     strCompany;       // v855 추가
	CString     strProductName;
	CString     strTypeNumber;
	int         nDamperType;      // 0:Single Dashpot Model, 1:Kelvin(Voigt) Model, 2:Maxwell Model
	int         nDashpotType;     // 0:Linear Elastic Type, 1:Elastic Bilinear Type, 2:Elastic Trilinear Type
	int         nInputType;       // 0:Damping Reduction Ratio(α1,α2), 1:Damping(C1,C2)
	T_SDVI_COMP_860 CompProp[6];
};
struct T_SDVI_D
{
	CString     strName;
	CString     strDesc;
	int         nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	CString     strDeviceType;
	CString     strCompany;       // v855 추가
	CString     strProductName;
	CString     strTypeNumber;
	int         nDamperType;      // 0:Single Dashpot Model, 1:Kelvin(Voigt) Model, 2:Maxwell Model
	int         nDashpotType;     // 0:Linear Elastic Type, 1:Elastic Bilinear Type, 2:Exponential Function Type
	int         nInputType;       // 0:Damping Reduction Ratio(α1,α2), 1:Damping(C1,C2)
	int         nInputType_Exfn;  // 0:P=Py*(V/Vy)^a; C=Py*(1/Vy)^a, 1:P=C*V^a
	T_SDVI_COMP CompProp[6];

	void Initialize()
	{
		strName.Empty();
		strDesc.Empty();
		nInputMethod = 0;
		strDeviceType.Empty();
		strCompany.Empty();
		strProductName.Empty();
		strTypeNumber.Empty();
		nDamperType = 0;
		nDashpotType = 0;
		nInputType = 0;
		nInputType_Exfn = 0;
		for(int i=0; i<6; i++) CompProp[i].Initialize();
	}

	// CompProp 6칸 중 1개만 활성화 할 수 있음(160217)
	// 활성화된 성분 인덱스를 반환
	int GetActiveDOF()
	{
		for(int i=0; i<6; i++)
		{
			if(CompProp[i].bDof) 
				return i;
		}
		
		ASSERT(0);
		return 0;
	}
	void Convert851(T_SDVI_D_851& rData)
	{
		strName = rData.strName;
		strDesc = rData.strDesc;
		nInputMethod = rData.nInputMethod;
		strDeviceType.Empty();  // 851버전에선 DB 없음(nInputMethod==0)
		strCompany.Empty();
		strProductName.Empty();
		strTypeNumber.Empty();
		nDamperType = rData.nDamperType;
		nDashpotType = rData.nDashpotType;
		nInputType = rData.nInputType;
		nInputType_Exfn = 0;

		for(int i=0; i<6; i++) 
		{
			CompProp[i].bDof   = rData.CompProp[i].bDof  ;
			CompProp[i].Ce     = rData.CompProp[i].Ce    ;
			CompProp[i].P1     = rData.CompProp[i].P1    ;
			CompProp[i].C1     = rData.CompProp[i].C1    ;
			CompProp[i].Alpha1 = rData.CompProp[i].Alpha1;
			CompProp[i].K0     = rData.CompProp[i].K0    ;
			CompProp[i].Exfn_Py  = 1.0;
			CompProp[i].Exfn_Vy  = 1.0;
			CompProp[i].Exfn_DE  = 0.3;
			CompProp[i].Exfn_DC  = 1.0;
			CompProp[i].Exfn_Ce  = 1.0;
			CompProp[i].bExfn_Ce = FALSE;
		}
	}
	void Convert860(T_SDVI_D_860& rData)
	{
		strName = rData.strName;
		strDesc = rData.strDesc;
		nInputMethod    = rData.nInputMethod;
		strDeviceType   = rData.strDeviceType ;
		strCompany      = rData.strCompany    ;
		strProductName  = rData.strProductName;
		strTypeNumber   = rData.strTypeNumber ;
		nDamperType = rData.nDamperType;
		nDashpotType = rData.nDashpotType;
		nInputType = rData.nInputType;
		nInputType_Exfn = 0;

		for(int i=0; i<6; i++) 
		{
			CompProp[i].bDof   = rData.CompProp[i].bDof  ;
			CompProp[i].Ce     = rData.CompProp[i].Ce    ;
			CompProp[i].P1     = rData.CompProp[i].P1    ;
			CompProp[i].C1     = rData.CompProp[i].C1    ;
			CompProp[i].Alpha1 = rData.CompProp[i].Alpha1;
			CompProp[i].K0     = rData.CompProp[i].K0    ;
			CompProp[i].Exfn_Py  = 1.0;
			CompProp[i].Exfn_Vy  = 1.0;
			CompProp[i].Exfn_DE  = 0.3;
			CompProp[i].Exfn_DC  = 1.0;
			CompProp[i].Exfn_Ce  = 1.0;
			CompProp[i].bExfn_Ce = FALSE;
		}
	}
};
struct T_SDVI_D_CH_851
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strProductName[40];
	char    strTypeNumber [40];
	int     nDamperType;      // 0:Single Dashpot Model, 1:Kelvin(Voigt) Model, 2:Maxwell Model
	int     nDashpotType;     // 0:Linear Elastic Type, 1:Elastic Bilinear Type, 2:Exponential Function Type
	int     nInputType;       // 0:Damping Reduction Ratio(α1,α2), 1:Damping(C1,C2)
	T_SDVI_COMP_860 CompProp[6];

	void ConvertToString(T_SDVI_D_851& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nDamperType = nDamperType;    
		rData.nDashpotType = nDashpotType;
		rData.nInputType = nInputType;
		memcpy(rData.CompProp, CompProp, sizeof(CompProp));
	}
};
struct T_SDVI_D_CH_860
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strCompany[40];
	char    strProductName[40];
	char    strTypeNumber [40];
	int     nDamperType;      // 0:Single Dashpot Model, 1:Kelvin(Voigt) Model, 2:Maxwell Model
	int     nDashpotType;     // 0:Linear Elastic Type, 1:Elastic Bilinear Type, 2:Exponential Function Type
	int     nInputType;       // 0:Damping Reduction Ratio(α1,α2), 1:Damping(C1,C2)
	T_SDVI_COMP_860 CompProp[6];

	void ConvertToString(T_SDVI_D_860& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nDamperType = nDamperType;    
		rData.nDashpotType = nDashpotType;
		rData.nInputType = nInputType;
		memcpy(rData.CompProp, CompProp, sizeof(CompProp));
	}
};
struct T_SDVI_D_CH
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strCompany[40];
	char    strProductName[40];
	char    strTypeNumber [40];
	int     nDamperType;      // 0:Single Dashpot Model, 1:Kelvin(Voigt) Model, 2:Maxwell Model
	int     nDashpotType;     // 0:Linear Elastic Type, 1:Elastic Bilinear Type, 2:Exponential Function Type
	int     nInputType;       // 0:Damping Reduction Ratio(α1,α2), 1:Damping(C1,C2)
	int     nInputType_Exfn;  // 0:P=Py*(V/Vy)^a; C=Py*(1/Vy)^a, 1:P=C*V^a
	T_SDVI_COMP CompProp[6];

	void ConvertToString(T_SDVI_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nDamperType = nDamperType;    
		rData.nDashpotType = nDashpotType;
		rData.nInputType = nInputType;
		rData.nInputType_Exfn = nInputType_Exfn;
		memcpy(rData.CompProp, CompProp, sizeof(CompProp));
	}
	void ConvertToChar(T_SDVI_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nInputMethod = rData.nInputMethod;
		ConvertStrChar(rData.strDeviceType, strDeviceType, sizeof(strDeviceType));
		ConvertStrChar(rData.strCompany, strCompany, sizeof(strCompany));
		ConvertStrChar(rData.strProductName, strProductName, sizeof(strProductName));
		ConvertStrChar(rData.strTypeNumber, strTypeNumber, sizeof(strTypeNumber));
		nDamperType  = rData.nDamperType; 
		nDashpotType = rData.nDashpotType;
		nInputType   = rData.nInputType;
		nInputType_Exfn   = rData.nInputType_Exfn;
		memcpy(CompProp, rData.CompProp, sizeof(CompProp));
	}
};
struct T_SDVI_UDRD_D_CH_851
{
	T_SDVI_K key;
	T_SDVI_D_CH_851 data;
};
struct T_SDVI_UDRD_D_CH_860
{
	T_SDVI_K key;
	T_SDVI_D_CH_860 data;
};
struct T_SDVI_UDRD_D
{
	T_SDVI_K key;
	T_SDVI_D data;
};
struct T_SDVI_UDRD_D_CH
{
	T_SDVI_K key;
	T_SDVI_D_CH data;
};

struct T_SDVI_UNIT
{
	T_SDVI_UNIT()
	{
		Alpha1  = D_UNITSYS_NONE;

		Ce_Dxyz = D_UNITSYS_BASE_FSEC_L;
		P1_Dxyz = D_UNITSYS_BASE_FORCE;
		C1_Dxyz = D_UNITSYS_BASE_FSEC_L;
		K0_Dxyz = D_UNITSYS_BASE_UNITFORCE;    
		Ce_Rxyz = D_UNITSYS_BASE_FLSEC_R;
		P1_Rxyz = D_UNITSYS_BASE_MOMENT;
		C1_Rxyz = D_UNITSYS_BASE_FLSEC_R;
		K0_Rxyz = D_UNITSYS_BASE_ROSPRING;
		Exfn_Py_Dxyz = D_UNITSYS_BASE_FORCE;
		Exfn_Vy_Dxyz = D_UNITSYS_BASE_VELOCITY;
		Exfn_DC_Dxyz = D_UNITSYS_BASE_FSEC_L;
		Exfn_Ce_Dxyz = D_UNITSYS_BASE_FSEC_L;
		Exfn_Py_Rxyz = D_UNITSYS_BASE_MOMENT;
		Exfn_Vy_Rxyz = D_UNITSYS_BASE_FREQUENCY;
		Exfn_DC_Rxyz = D_UNITSYS_BASE_FLSEC_R;
		Exfn_Ce_Rxyz = D_UNITSYS_BASE_FLSEC_R;

	}
	int Alpha1;
	int Ce_Dxyz;
	int P1_Dxyz;
	int C1_Dxyz;
	int K0_Dxyz;
	int Ce_Rxyz;
	int P1_Rxyz;
	int C1_Rxyz;
	int K0_Rxyz;
	int Exfn_Py_Dxyz;
	int Exfn_Vy_Dxyz;
	int Exfn_DC_Dxyz;
	int Exfn_Ce_Dxyz;
	int Exfn_Py_Rxyz;
	int Exfn_Vy_Rxyz;
	int Exfn_DC_Rxyz;
	int Exfn_Ce_Rxyz;
};
#define HASHSIZESDVI 101

/////////////////////////////////////////////////////
// Seismic device properties - Vicoelastic damper
#define T_SDVE_K unsigned int

#define D_SDVE_DX 1
#define D_SDVE_DY 2
#define D_SDVE_DZ 3
enum D_SDVE_MATL 
{
	D_SDVE_GR100=1,
	D_SDVE_GR300  ,
	D_SDVE_SR05   ,
	D_SDVE_GR400  ,
	D_SDVE_ISD111 ,
	D_SDVE_ISD111H,
	D_SDVE_CST    ,
	D_SDVE_TRC    ,
};

struct T_SDVE_D_851
{
	CString strName;
	CString strDesc;
	int     nInputMethod;    // 0:User Input, 1:Import Reference DataBase
	CString strDeviceType;
	CString strCompany;
	CString strProductName;
	CString strTypeNumber;

	int     nMatlType       ; // 1:GR100(VS1), 2:GR300(VS2), 3:SR05(VS3), 4:GR400(VS4), 5:3M ISD111, 6:3M ISD111H
	double  dShearArea      ;
	double  dThickness      ;
	double  dMultiplier     ;
	int     nDir            ; // 1:Dx, 2:Dy, 3:Dz
	double  dFreq           ;
	double  dScaleFactor    ;
	double  dDampingFactor  ;
	double  dReferenceT     ;
	BOOL    bConsiderTC     ;
	double  dLimitDeform    ;
	double  dEffStiffness   ;
	double  dEquiDamping    ;
};
struct T_SDVE_D_856
{
	CString strName;
	CString strDesc;
	int     nInputMethod;    // 0:User Input, 1:Import Reference DataBase
	CString strDeviceType;
	CString strCompany;
	CString strProductName;
	CString strTypeNumber;

	int     nMatlType       ; // 1:GR100(VS1), 2:GR300(VS2), 3:SR05(VS3), 4:GR400(VS4), 5:3M ISD111, 6:3M ISD111H, 7:CST Series
	double  dShearArea      ;
	double  dThickness      ;
	double  dMultiplier     ;
	int     nDir            ; // 1:Dx, 2:Dy, 3:Dz
	double  dFreq           ;
	double  dScaleFactor    ;
	double  dDampingFactor  ;
	double  dReferenceT     ;
	BOOL    bConsiderTC     ;
	double  dLimitDeform    ;
	double  dEffStiffness   ;
	double  dEquiDamping    ;
	BOOL    bUserSumitomoProp;     //v851 추가
	double  dUserSumitomoProp[20]; //v851 추가
};
struct T_SDVE_D
{
	CString strName;
	CString strDesc;
	int     nInputMethod;    // 0:User Input, 1:Import Reference DataBase
	CString strDeviceType;
	CString strCompany;
	CString strProductName;
	CString strTypeNumber;
	
	int     nMatlType       ; // 1:GR100(VS1), 2:GR300(VS2), 3:SR05(VS3), 4:GR400(VS4), 5:3M ISD111, 6:3M ISD111H, 7:CST Series
	double  dShearArea      ;
	double  dThickness      ;
	double  dMultiplier     ;
	int     nDir            ; // 1:Dx, 2:Dy, 3:Dz
	double  dFreq           ;
	double  dScaleFactor    ;
	double  dDampingFactor  ;
	double  dReferenceT     ;
	BOOL    bConsiderTC     ;
	double  dLimitDeform    ;
	double  dEffStiffness   ;
	double  dEquiDamping    ;
	BOOL    bUserSumitomoProp;     //v851 추가
	double  dUserSumitomoProp[20]; //v851 추가
	BOOL    bMountStiff;
	double  dMountStiff;
	BOOL    bKineticFric;
	double  dKineticFric;

	T_SDVE_D() { Initialize(); }

	void Initialize()
	{
		strName.Empty();
		strDesc.Empty();
		nInputMethod = 0;
		strDeviceType.Empty();
		strCompany.Empty();
		strProductName.Empty();
		strTypeNumber.Empty();

		nMatlType      = D_SDVE_GR100;
		dShearArea     = 0.;
		dThickness     = 0.;
		dMultiplier    = 1.;
		nDir           = D_SDVE_DX;
		dFreq          = 0.;
		dScaleFactor   = 1.;
		dDampingFactor = 1.;
		dReferenceT    = 20.;
		bConsiderTC    = FALSE;
		dLimitDeform   = 0.;
		dEffStiffness  = 0.;
		dEquiDamping   = 0.;
		bUserSumitomoProp=FALSE;
		memset(dUserSumitomoProp,0,sizeof(dUserSumitomoProp));
		bMountStiff = FALSE;
		dMountStiff = 0.0;
		bKineticFric = FALSE;
		dKineticFric = 0.0;

	}
	void Convert856(T_SDVE_D_856& rData)
	{
		strName = rData.strName;
		strDesc = rData.strDesc;
		nInputMethod = rData.nInputMethod;
		strDeviceType = rData.strDeviceType;
		strCompany = rData.strCompany;
		strProductName = rData.strProductName;
		strTypeNumber = rData.strTypeNumber;

		nMatlType      = rData.nMatlType     ;
		dShearArea     = rData.dShearArea    ;
		dThickness     = rData.dThickness    ;
		dMultiplier    = rData.dMultiplier   ;
		nDir           = rData.nDir          ;
		dFreq          = rData.dFreq         ;
		dScaleFactor   = rData.dScaleFactor  ;
		dDampingFactor = rData.dDampingFactor;
		dReferenceT    = rData.dReferenceT   ;
		bConsiderTC    = rData.bConsiderTC   ;
		dLimitDeform   = rData.dLimitDeform  ;
		dEffStiffness  = rData.dEffStiffness ;
		dEquiDamping   = rData.dEquiDamping  ;
		bUserSumitomoProp = rData.bUserSumitomoProp;
		for(int i=0; i<20; i++) dUserSumitomoProp[i] = rData.dUserSumitomoProp[i];
	}
	void Convert851(T_SDVE_D_851& rData)
	{
		strName = rData.strName;
		strDesc = rData.strDesc;
		nInputMethod = rData.nInputMethod;
		strDeviceType.Empty();  // 851버전에선 DB 없음(nInputMethod==0)
		strCompany.Empty();
		strProductName.Empty();
		strTypeNumber.Empty();

		nMatlType      = rData.nMatlType     ;
		dShearArea     = rData.dShearArea    ;
		dThickness     = rData.dThickness    ;
		dMultiplier    = rData.dMultiplier   ;
		nDir           = rData.nDir          ;
		dFreq          = rData.dFreq         ;
		dScaleFactor   = rData.dScaleFactor  ;
		dDampingFactor = rData.dDampingFactor;
		dReferenceT    = rData.dReferenceT   ;
		bConsiderTC    = rData.bConsiderTC   ;
		dLimitDeform   = rData.dLimitDeform  ;
		dEffStiffness  = rData.dEffStiffness ;
		dEquiDamping   = rData.dEquiDamping  ;
		bUserSumitomoProp = FALSE;
		memset(dUserSumitomoProp,0,sizeof(dUserSumitomoProp));
	}
};

struct T_SDVE_D_CH_851
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strCompany    [40];
	char    strProductName[40];
	char    strTypeNumber [40];

	int     nMatlType       ; // 1:GR100(VS1), 2:GR300(VS2), 3:SR05(VS3), 4:GR400(VS4), 5:3M ISD111, 6:3M ISD111H
	double  dShearArea      ;
	double  dThickness      ;
	double  dMultiplier     ;
	int     nDir            ; // 1:Dx, 2:Dy, 3:Dz
	double  dFreq           ;
	double  dScaleFactor    ;
	double  dDampingFactor  ;
	double  dReferenceT     ;
	BOOL    bConsiderTC     ;
	double  dLimitDeform    ;
	double  dEffStiffness   ;
	double  dEquiDamping    ;

	void ConvertToString(T_SDVE_D_851& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nMatlType      = nMatlType     ;
		rData.dShearArea     = dShearArea    ;
		rData.dThickness     = dThickness    ;
		rData.dMultiplier    = dMultiplier   ;
		rData.nDir           = nDir          ;
		rData.dFreq          = dFreq         ;
		rData.dScaleFactor   = dScaleFactor  ;
		rData.dDampingFactor = dDampingFactor;
		rData.dReferenceT    = dReferenceT   ;
		rData.bConsiderTC    = bConsiderTC   ;
		rData.dLimitDeform   = dLimitDeform  ;
		rData.dEffStiffness  = dEffStiffness ;
		rData.dEquiDamping   = dEquiDamping  ;
	}
};

struct T_SDVE_D_CH_856
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strCompany    [40];
	char    strProductName[40];
	char    strTypeNumber [40];

	int     nMatlType       ; // 1:GR100(VS1), 2:GR300(VS2), 3:SR05(VS3), 4:GR400(VS4), 5:3M ISD111, 6:3M ISD111H, 7:CST Series
	double  dShearArea      ;
	double  dThickness      ;
	double  dMultiplier     ;
	int     nDir            ; // 1:Dx, 2:Dy, 3:Dz
	double  dFreq           ;
	double  dScaleFactor    ;
	double  dDampingFactor  ;
	double  dReferenceT     ;
	BOOL    bConsiderTC     ;
	double  dLimitDeform    ;
	double  dEffStiffness   ;
	double  dEquiDamping    ;
	BOOL    bUserSumitomoProp;     //v851 추가
	double  dUserSumitomoProp[20]; //v851 추가

	void ConvertToString(T_SDVE_D_856& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nMatlType      = nMatlType     ;
		rData.dShearArea     = dShearArea    ;
		rData.dThickness     = dThickness    ;
		rData.dMultiplier    = dMultiplier   ;
		rData.nDir           = nDir          ;
		rData.dFreq          = dFreq         ;
		rData.dScaleFactor   = dScaleFactor  ;
		rData.dDampingFactor = dDampingFactor;
		rData.dReferenceT    = dReferenceT   ;
		rData.bConsiderTC    = bConsiderTC   ;
		rData.dLimitDeform   = dLimitDeform  ;
		rData.dEffStiffness  = dEffStiffness ;
		rData.dEquiDamping   = dEquiDamping  ;
		rData.bUserSumitomoProp = bUserSumitomoProp;
		memcpy(rData.dUserSumitomoProp,dUserSumitomoProp,sizeof(dUserSumitomoProp));
	}
};

struct T_SDVE_D_CH
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strCompany    [40];
	char    strProductName[40];
	char    strTypeNumber [40];

	int     nMatlType       ; // 1:GR100(VS1), 2:GR300(VS2), 3:SR05(VS3), 4:GR400(VS4), 5:3M ISD111, 6:3M ISD111H, 7:CST Series
	double  dShearArea      ;
	double  dThickness      ;
	double  dMultiplier     ;
	int     nDir            ; // 1:Dx, 2:Dy, 3:Dz
	double  dFreq           ;
	double  dScaleFactor    ;
	double  dDampingFactor  ;
	double  dReferenceT     ;
	BOOL    bConsiderTC     ;
	double  dLimitDeform    ;
	double  dEffStiffness   ;
	double  dEquiDamping    ;
	BOOL    bUserSumitomoProp;     //v851 추가
	double  dUserSumitomoProp[20]; //v851 추가
	BOOL    bMountStiff;
	double  dMountStiff;
	BOOL    bKineticFric;
	double  dKineticFric;

	void ConvertToString(T_SDVE_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nMatlType      = nMatlType     ;
		rData.dShearArea     = dShearArea    ;
		rData.dThickness     = dThickness    ;
		rData.dMultiplier    = dMultiplier   ;
		rData.nDir           = nDir          ;
		rData.dFreq          = dFreq         ;
		rData.dScaleFactor   = dScaleFactor  ;
		rData.dDampingFactor = dDampingFactor;
		rData.dReferenceT    = dReferenceT   ;
		rData.bConsiderTC    = bConsiderTC   ;
		rData.dLimitDeform   = dLimitDeform  ;
		rData.dEffStiffness  = dEffStiffness ;
		rData.dEquiDamping   = dEquiDamping  ;
		rData.bUserSumitomoProp = bUserSumitomoProp;
		memcpy(rData.dUserSumitomoProp,dUserSumitomoProp,sizeof(dUserSumitomoProp));
		rData.bMountStiff = bMountStiff;
		rData.dMountStiff = dMountStiff;
		rData.bKineticFric = bKineticFric;
		rData.dKineticFric = dKineticFric;
	}
	void ConvertToChar(T_SDVE_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nInputMethod = rData.nInputMethod;
		ConvertStrChar(rData.strDeviceType, strDeviceType, sizeof(strDeviceType));
		ConvertStrChar(rData.strCompany, strCompany, sizeof(strCompany));
		ConvertStrChar(rData.strProductName, strProductName, sizeof(strProductName));
		ConvertStrChar(rData.strTypeNumber, strTypeNumber, sizeof(strTypeNumber));
		nMatlType       = rData.nMatlType     ; 
		dShearArea      = rData.dShearArea    ; 
		dThickness      = rData.dThickness    ; 
		dMultiplier     = rData.dMultiplier   ; 
		nDir            = rData.nDir          ; 
		dFreq           = rData.dFreq         ; 
		dScaleFactor    = rData.dScaleFactor  ; 
		dDampingFactor  = rData.dDampingFactor; 
		dReferenceT     = rData.dReferenceT   ; 
		bConsiderTC     = rData.bConsiderTC   ; 
		dLimitDeform    = rData.dLimitDeform  ; 
		dEffStiffness   = rData.dEffStiffness ; 
		dEquiDamping    = rData.dEquiDamping  ; 
		bUserSumitomoProp = rData.bUserSumitomoProp;
		memcpy(dUserSumitomoProp,rData.dUserSumitomoProp,sizeof(dUserSumitomoProp));
		bMountStiff = rData.bMountStiff;
		dMountStiff = rData.dMountStiff;
		bKineticFric = rData.bKineticFric;
		dKineticFric = rData.dKineticFric;
	}
};
struct T_SDVE_UDRD_D
{
	T_SDVE_K key;
	T_SDVE_D data;
};
struct T_SDVE_UDRD_D_CH_851
{
	T_SDVE_K key;
	T_SDVE_D_CH_851 data;
};
struct T_SDVE_UDRD_D_CH_856
{
	T_SDVE_K key;
	T_SDVE_D_CH_856 data;
};
struct T_SDVE_UDRD_D_CH
{
	T_SDVE_K key;
	T_SDVE_D_CH data;
};
struct T_SDVE_UNIT
{
	T_SDVE_UNIT()
	{
		dShearArea      = D_UNITSYS_BASE_AREA;
		dThickness      = D_UNITSYS_BASE_LENGTH;
		dMultiplier     = D_UNITSYS_NONE;
		dFreq           = D_UNITSYS_NONE;
		dScaleFactor    = D_UNITSYS_NONE;
		dDampingFactor  = D_UNITSYS_NONE;
		dReferenceT     = D_UNITSYS_NONE;
		dLimitDeform    = D_UNITSYS_BASE_LENGTH;
		dEffStiffness   = D_UNITSYS_BASE_UNITFORCE;
		dEquiDamping    = D_UNITSYS_BASE_FSEC_L;
		memset(dSumitomoProp,0,sizeof(dSumitomoProp));
		dSumitomoProp[1]=D_UNITSYS_BASE_ELAST;
		dSumitomoProp[3]=D_UNITSYS_BASE_ELAST;
		dSumitomoProp[4]=D_UNITSYS_BASE_ELAST;
		dSumitomoProp[7]=D_UNITSYS_BASE_ELAST;
		dSumitomoProp[9]=D_UNITSYS_BASE_ELAST;
		dMountStiff=D_UNITSYS_BASE_UNITFORCE;
		dKineticFric=D_UNITSYS_BASE_FORCE;
	}
	int dShearArea    ;
	int dThickness    ;
	int dMultiplier   ;
	int dFreq         ;
	int dScaleFactor  ;
	int dDampingFactor;
	int dReferenceT   ;
	int dLimitDeform  ;
	int dEffStiffness ;
	int dEquiDamping  ;
	int dSumitomoProp[20];
	int dMountStiff;
	int dKineticFric;
};
#define HASHSIZESDVE 101

/////////////////////////////////////////////////////
// Seismic device properties - Steel Damper
#define T_SDST_K unsigned int

#define D_SDST_DX 1
#define D_SDST_DY 2
#define D_SDST_DZ 3
#define D_SDST_HYS_BL2 80
#define D_SDST_HYS_LY2 82
#define D_SDST_HYS_LY3 83
#define D_SDST_HYS_IK2 84

struct T_SDST_D
{
	CString strName;
	CString strDesc;
	int     nInputMethod;    // 0:User Input, 1:Import Reference DataBase
	CString strDeviceType;
	CString strCompany;
	CString strProductName;
	CString strTypeNumber;

	int     nDir   ; // 1:Dx, 2:Dy, 3:Dz
	int     nHysModel; // 80 : Degrading Bi Linear BL2 / Bilinear, 82 : Low yielding strength steel material model LY2 / Trilinear, 
										 // 83 : Low yielding strength steel material model LY3 / Trilinear, 84 : Steel Isotropic-Kinematic Hardening Model lk2 / Bilinear"
	double  dK0    ;
	double  dP1    ;
	double  dAlpha1;
	double  dAlpha2;
	double  dBeta  ;
	double  dTheta ;
	double  dGamma ;
	double  dKb    ;

	T_SDST_D() { Initialize(); }

	void Initialize()
	{
		strName.Empty();
		strDesc.Empty();
		nInputMethod = 0;
		strDeviceType.Empty();
		strCompany.Empty();
		strProductName.Empty();
		strTypeNumber.Empty();

		nDir      = D_SDST_DX;
		nHysModel = D_SDST_HYS_BL2;
		dK0     = 0.;
		dP1     = 0.;
		dAlpha1 = 0.;
		dAlpha2 = 0.;
		dBeta   = 0.;
		dTheta  = 1.5;
		dGamma  = 0.;
		dKb     = 0.;
	}

	// mgt등에서 쓸 때 편의함수
	int GetHysIdx()
	{
		if(nHysModel==D_SDST_HYS_BL2) return 0;
		else if(nHysModel==D_SDST_HYS_LY2) return 1;
		else if(nHysModel==D_SDST_HYS_LY3) return 2;
		else if(nHysModel==D_SDST_HYS_IK2) return 3;

		ASSERT(0); 
		return 0;
	}
	void SetIdxHys(int rHysIdx)
	{
		if(rHysIdx==0) nHysModel = D_SDST_HYS_BL2;
		else if(rHysIdx==1) nHysModel = D_SDST_HYS_LY2;
		else if(rHysIdx==2) nHysModel = D_SDST_HYS_LY3;
		else if(rHysIdx==3) nHysModel = D_SDST_HYS_IK2;
		else { ASSERT(0); nHysModel = D_SDST_HYS_BL2; }
	}
};
struct T_SDST_D_CH
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strCompany    [40];
	char    strProductName[40];
	char    strTypeNumber [40];

	int     nDir     ;
	int     nHysModel;                      
	double  dK0      ;
	double  dP1      ;
	double  dAlpha1  ;
	double  dAlpha2  ;
	double  dBeta    ;
	double  dTheta   ;
	double  dGamma   ;
	double  dKb      ;

	void ConvertToString(T_SDST_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nDir      = nDir     ;
		rData.nHysModel = nHysModel;
		rData.dK0       = dK0      ;
		rData.dP1       = dP1      ;
		rData.dAlpha1   = dAlpha1  ;
		rData.dAlpha2   = dAlpha2  ;
		rData.dBeta     = dBeta    ;
		rData.dTheta    = dTheta   ;
		rData.dGamma    = dGamma   ;
		rData.dKb       = dKb      ;
	}
	void ConvertToChar(T_SDST_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nInputMethod = rData.nInputMethod;
		ConvertStrChar(rData.strDeviceType, strDeviceType, sizeof(strDeviceType));
		ConvertStrChar(rData.strCompany, strCompany, sizeof(strCompany));
		ConvertStrChar(rData.strProductName, strProductName, sizeof(strProductName));
		ConvertStrChar(rData.strTypeNumber, strTypeNumber, sizeof(strTypeNumber));
		nDir      = rData.nDir     ; 
		nHysModel = rData.nHysModel; 
		dK0       = rData.dK0      ; 
		dP1       = rData.dP1      ; 
		dAlpha1   = rData.dAlpha1  ; 
		dAlpha2   = rData.dAlpha2  ; 
		dBeta     = rData.dBeta    ; 
		dTheta    = rData.dTheta   ; 
		dGamma    = rData.dGamma   ; 
		dKb       = rData.dKb      ; 
	}
};
struct T_SDST_UDRD_D
{
	T_SDST_K key;
	T_SDST_D data;
};
struct T_SDST_UDRD_D_CH
{
	T_SDST_K key;
	T_SDST_D_CH data;
};

struct T_SDST_UNIT
{
	T_SDST_UNIT()
	{
		dK0     = D_UNITSYS_BASE_UNITFORCE;
		dP1     = D_UNITSYS_BASE_FORCE;
		dAlpha1 = D_UNITSYS_NONE;
		dAlpha2 = D_UNITSYS_NONE;
		dBeta   = D_UNITSYS_NONE;
		dTheta  = D_UNITSYS_NONE;
		dGamma  = D_UNITSYS_NONE;
		dKb     = D_UNITSYS_BASE_UNITFORCE;
	}
	int dK0    ;
	int dP1    ;
	int dAlpha1;
	int dAlpha2;
	int dBeta  ;
	int dTheta ;
	int dGamma ;
	int dKb    ;
};
#define HASHSIZESDST 101

/////////////////////////////////////////////////////
// Seismic device properties - Hysteresis Damper
#define T_SDHY_K unsigned int

#define D_SDHY_HYS_NORTRI 1
#define D_SDHY_HYS_DEGRBL 80
#define D_SDHY_HYS_MODIRO 81

struct T_SDHY_D
{
	CString strName;
	CString strDesc;
	int     nInputMethod;    // 0:User Input, 1:Import Reference DataBase
	CString strDeviceType;
	CString strCompany;
	CString strProductName;
	CString strTypeNumber;

	int     nHysModel; // 1:NORMAL TRILINEAR, 80:Degrading Bi Linear, 81:Modified Ramberg Osgood 
	int     nNumMSS  ;
	double  dK0      ;
	double  dP1      ;
	double  dP2      ;
	double  dAlpha1  ;
	double  dAlpha2  ;
	double  dBeta    ;
	double  dPhi     ;
	double  dLambda  ;

	T_SDHY_D() { Initialize(); }

	void Initialize()
	{
		strName.Empty();
		strDesc.Empty();
		nInputMethod = 0;
		strDeviceType.Empty();
		strCompany.Empty();
		strProductName.Empty();
		strTypeNumber.Empty();

		nHysModel = D_SDHY_HYS_DEGRBL;
		nNumMSS = 8;
		dK0     = 0.;
		dP1     = 0.;
		dP2     = 0.;
		dAlpha1 = 0.;
		dAlpha2 = 0.;
		dBeta   = 0.;
		dPhi    = 0.;
		dLambda = 8.;
	}
	// mgt등에서 쓸 때 편의함수
	int GetHysIdx()
	{
		if(nHysModel==D_SDHY_HYS_DEGRBL) return 0;
		else if(nHysModel==D_SDHY_HYS_NORTRI) return 1;
		else if(nHysModel==D_SDHY_HYS_MODIRO) return 2;

		ASSERT(0); 
		return 0;
	}
	void SetIdxHys(int rHysIdx)
	{
		if(rHysIdx==0) nHysModel = D_SDHY_HYS_DEGRBL;
		else if(rHysIdx==1) nHysModel = D_SDHY_HYS_NORTRI;
		else if(rHysIdx==2) nHysModel = D_SDHY_HYS_MODIRO;
		else { ASSERT(0); nHysModel = D_SDHY_HYS_DEGRBL; }
	}
};
struct T_SDHY_D_CH
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strDeviceType [40];
	char    strCompany    [40];
	char    strProductName[40];
	char    strTypeNumber [40];

	int     nHysModel; // 1:NORMAL TRILINEAR, 80:Degrading Bi Linear, 81:Modified Ramberg Osgood 
	int     nNumMSS  ;
	double  dK0      ;
	double  dP1      ;
	double  dP2      ;
	double  dAlpha1  ;
	double  dAlpha2  ;
	double  dBeta    ;
	double  dPhi     ;
	double  dLambda  ;

	void ConvertToString(T_SDHY_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strDeviceType, rData.strDeviceType, sizeof(strDeviceType));
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nHysModel = nHysModel;
		rData.nNumMSS   = nNumMSS  ;
		rData.dK0       = dK0      ;
		rData.dP1       = dP1      ;
		rData.dP2       = dP2      ;
		rData.dAlpha1   = dAlpha1  ;
		rData.dAlpha2   = dAlpha2  ;
		rData.dBeta     = dBeta    ;
		rData.dPhi      = dPhi     ;
		rData.dLambda   = dLambda  ;
	}
	void ConvertToChar(T_SDHY_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nInputMethod = rData.nInputMethod;
		ConvertStrChar(rData.strDeviceType, strDeviceType, sizeof(strDeviceType));
		ConvertStrChar(rData.strCompany, strCompany, sizeof(strCompany));
		ConvertStrChar(rData.strProductName, strProductName, sizeof(strProductName));
		ConvertStrChar(rData.strTypeNumber, strTypeNumber, sizeof(strTypeNumber));
		nHysModel = rData.nHysModel; 
		nNumMSS   = rData.nNumMSS  ; 
		dK0       = rData.dK0      ; 
		dP1       = rData.dP1      ; 
		dP2       = rData.dP2      ; 
		dAlpha1   = rData.dAlpha1  ; 
		dAlpha2   = rData.dAlpha2  ; 
		dBeta     = rData.dBeta    ; 
		dPhi      = rData.dPhi     ; 
		dLambda   = rData.dLambda  ; 
	}
};
struct T_SDHY_UDRD_D
{
	T_SDHY_K key;
	T_SDHY_D data;
};
struct T_SDHY_UDRD_D_CH
{
	T_SDHY_K key;
	T_SDHY_D_CH data;
};

struct T_SDHY_UNIT
{
	T_SDHY_UNIT()
	{
		dK0     = D_UNITSYS_BASE_UNITFORCE;
		dP1     = D_UNITSYS_BASE_FORCE;
		dP2     = D_UNITSYS_BASE_FORCE;
		dAlpha1 = D_UNITSYS_NONE;
		dAlpha2 = D_UNITSYS_NONE;
		dBeta   = D_UNITSYS_NONE;
		dPhi    = D_UNITSYS_NONE;
		dLambda = D_UNITSYS_NONE;
	}
	int dK0    ;
	int dP1    ;
	int dP2    ;
	int dAlpha1;
	int dAlpha2;
	int dBeta  ;
	int dPhi   ;
	int dLambda;
};
#define HASHSIZESDHY 101

/////////////////////////////////////////////////////
// Seismic device properties - Hysteresis Damper
#define T_SDIS_K unsigned int

#define D_SDIS_DEV_LRB 85 // LRB
#define D_SDIS_DEV_NRB 86 // NRB
#define D_SDIS_DEV_HDR 87 // High Damping Rubber Isolator
#define D_SDIS_DEV_SLD 88 // Sliding Bearings

#define D_SDIS_HYS_MB  1
#define D_SDIS_HYS_MHD 2

struct T_SDIS_D
{
	CString strName;
	CString strDesc;
	
	int     nInputMethod;    // 0:User Input, 1:Import Reference DataBase  
	CString strCompany;
	CString strProductName;
	CString strTypeNumber;

	int     nDeviceType; // 85:Lead Rubber Bearings, 86:Natural Rubber Bearings
	int     nHysModel  ; // 1:Modified Bilinear, 2:Mod. HD Model (Hardin-Drnevich Model)
	int     nNumMSS    ;
	double  dKe        ;
	double  dTauk      ;
	double  dTauq      ;
	double  dAr        ;
	double  dTr        ;
	double  dKh        ;
	double  dKv        ;
	BOOL    bConsNonl  ; // Consider Vertical Direction Nonlinearity
	double  dBeta      ;
	double  dAlpha     ;
	double  dSigmav    ;
	double  dK0;
	double  dK2;
	double  dQd;
	double  dAs;
	int     nQd;
	double  dPi;
	double  dMu0;

	T_SDIS_D() { Initialize(); }

	void Initialize()
	{
		strName.Empty();
		strDesc.Empty();
		nInputMethod = 0;
		strCompany.Empty();
		strProductName.Empty();
		strTypeNumber.Empty();

		nDeviceType = D_SDIS_DEV_LRB;
		nHysModel   = D_SDIS_HYS_MB;
		nNumMSS     = 8;
		dKe         = 0.;
		dTauk       = 1.;
		dTauq       = 1.;
		dAr         = 1.;
		dTr         = 1.;
		dKh         = 1.;
		dKv         = 1.;
		bConsNonl   = FALSE;
		dBeta       = 0.1;
		dAlpha      = 0.001;
		dSigmav     = 1.;
		dK0         = 1.;
		dK2         = 0.9;
		dQd         = 1.;
		dAs         = 1.;
		nQd         = 1;
		dPi         = 1.;
		dMu0        = 0.01;
	}
	// mgt등에서 쓸 때 편의함수
	int GetDvtIdx()
	{
		if(nDeviceType==D_SDIS_DEV_LRB) return 0;
		else if(nDeviceType==D_SDIS_DEV_NRB) return 1;
		else if(nDeviceType==D_SDIS_DEV_HDR) return 2;
		else if(nDeviceType==D_SDIS_DEV_SLD) return 3;

		ASSERT(0); 
		return D_SDIS_DEV_LRB;
	}
	void SetIdxDvt(int rDvtIdx)
	{
		if(rDvtIdx==0) nDeviceType = D_SDIS_DEV_LRB;
		else if(rDvtIdx==1) nDeviceType = D_SDIS_DEV_NRB;
		else if(rDvtIdx==2) nDeviceType = D_SDIS_DEV_HDR;
		else if(rDvtIdx==3) nDeviceType = D_SDIS_DEV_SLD;
		else {ASSERT(0); nDeviceType = D_SDIS_DEV_LRB;}
	}
};
struct T_SDIS_D_CH
{
	char    strName[40];
	char    strDesc[80];
	int     nInputMethod;     // 0:User Input, 1:Import Reference DataBase
	char    strCompany    [40];
	char    strProductName[40];
	char    strTypeNumber [40];

	int     nDeviceType; // 85:Lead Rubber Bearings, 86:Natural Rubber Bearings
	int     nHysModel  ; // 1:Modified Bilinear, 2:Mod. HD Model (Hardin-Drnevich Model)
	int     nNumMSS    ;
	double  dKe        ;
	double  dTauk      ;
	double  dTauq      ;
	double  dAr        ;
	double  dTr        ;
	double  dKh        ;
	double  dKv        ;
	BOOL    bConsNonl  ; // Consider Vertical Direction Nonlinearity
	double  dBeta      ;
	double  dAlpha     ;
	double  dSigmav    ;
	double  dK0;
	double  dK2;
	double  dQd;
	double  dAs;
	int     nQd;
	double  dPi;
	double  dMu0;

	void ConvertToString(T_SDIS_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nInputMethod = nInputMethod;
		ConvertCharStr(strCompany, rData.strCompany, sizeof(strCompany));
		ConvertCharStr(strProductName, rData.strProductName, sizeof(strProductName));
		ConvertCharStr(strTypeNumber, rData.strTypeNumber, sizeof(strTypeNumber));
		rData.nDeviceType = nDeviceType;
		rData.nHysModel   = nHysModel  ;
		rData.nNumMSS     = nNumMSS    ;
		rData.dKe         = dKe        ;
		rData.dTauk       = dTauk      ;
		rData.dTauq       = dTauq      ;
		rData.dAr         = dAr        ;
		rData.dTr         = dTr        ;
		rData.dKh         = dKh        ;
		rData.dKv         = dKv        ;
		rData.bConsNonl   = bConsNonl  ;
		rData.dBeta       = dBeta      ;
		rData.dAlpha      = dAlpha     ;
		rData.dSigmav     = dSigmav    ;
		rData.dK0         = dK0     ;
		rData.dK2         = dK2     ;
		rData.dQd         = dQd     ;
		rData.dAs         = dAs     ;
		rData.nQd         = nQd     ;
		rData.dPi         = dPi     ;
		rData.dMu0        = dMu0    ;
	}
	void ConvertToChar(T_SDIS_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nInputMethod = rData.nInputMethod;
		ConvertStrChar(rData.strCompany, strCompany, sizeof(strCompany));
		ConvertStrChar(rData.strProductName, strProductName, sizeof(strProductName));
		ConvertStrChar(rData.strTypeNumber, strTypeNumber, sizeof(strTypeNumber));
		nDeviceType = rData.nDeviceType; 
		nHysModel   = rData.nHysModel  ; 
		nNumMSS     = rData.nNumMSS    ; 
		dKe         = rData.dKe        ; 
		dTauk       = rData.dTauk      ; 
		dTauq       = rData.dTauq      ; 
		dAr         = rData.dAr        ; 
		dTr         = rData.dTr        ; 
		dKh         = rData.dKh        ; 
		dKv         = rData.dKv        ; 
		bConsNonl   = rData.bConsNonl  ; 
		dBeta       = rData.dBeta      ; 
		dAlpha      = rData.dAlpha     ; 
		dSigmav     = rData.dSigmav    ; 
		dK0         = rData.dK0     ;
		dK2         = rData.dK2     ;
		dQd         = rData.dQd     ;
		dAs         = rData.dAs     ;
		nQd         = rData.nQd     ;
		dPi         = rData.dPi     ;
		dMu0        = rData.dMu0    ;
	}
};
struct T_SDIS_UDRD_D
{
	T_SDIS_K key;
	T_SDIS_D data;
};
struct T_SDIS_UDRD_D_CH
{
	T_SDIS_K key;
	T_SDIS_D_CH data;
};

struct T_SDIS_UNIT
{
	T_SDIS_UNIT()
	{
		dKe       = D_UNITSYS_BASE_UNITFORCE;
		dTauk     = D_UNITSYS_NONE;
		dTauq     = D_UNITSYS_NONE;
		dAr       = D_UNITSYS_BASE_AREA;
		dTr       = D_UNITSYS_BASE_LENGTH;
		dKh       = D_UNITSYS_BASE_UNITFORCE;
		dKv       = D_UNITSYS_BASE_UNITFORCE;
		dBeta     = D_UNITSYS_NONE;
		dAlpha    = D_UNITSYS_NONE;
		dSigmav   = D_UNITSYS_BASE_ELAST;
		dK0       = D_UNITSYS_BASE_UNITFORCE;
		dK2       = D_UNITSYS_BASE_UNITFORCE;
		dQd       = D_UNITSYS_BASE_FORCE;
		dAs       = D_UNITSYS_BASE_AREA;
		dPi       = D_UNITSYS_BASE_FORCE;
	}
	int dKe      ;
	int dTauk    ;
	int dTauq    ;
	int dAr      ;
	int dTr      ;
	int dKh      ;
	int dKv      ;
	int dBeta    ;
	int dAlpha   ;
	int dSigmav  ;
	int dK0;
	int dK2;
	int dQd;
	int dAs;
	int dPi;
};
#define HASHSIZESDIS 101

// Multi-Linear Function
struct T_MLFC_BASE
{
	double dValueX; // Disp, Radian
	double dValueY; // Force, Moment

	void Initialize()
	{
		dValueX = 0.0;
		dValueY = 0.0;		
	}
};

#define T_MLFC_DT CArray<T_MLFC_BASE, T_MLFC_BASE&>
#define T_MLFC_K unsigned int
struct T_MLFC_D
{
	CString FuncName;
	int     nFuncType; // 0:Force, 1:Moment
	BOOL    bSymmetric;
	T_MLFC_DT arFuncData;
	unsigned int nFuncId; // 성능 문제로 사용하지 않음.

	void Initialize()
	{
		FuncName.Empty();
		nFuncType = 0;
		bSymmetric = FALSE;
		arFuncData.RemoveAll();
		nFuncId = 0;
	}
	T_MLFC_D() {}
	T_MLFC_D(T_MLFC_D& src) { *this = src; }
	T_MLFC_D& operator=(T_MLFC_D& src)
	{
		FuncName = src.FuncName;
		nFuncType = src.nFuncType;
		bSymmetric = src.bSymmetric;
		arFuncData.Copy(src.arFuncData);
		nFuncId = src.nFuncId;

		return *this;
	}
};
struct T_MLFC_D_CH
{
	char FuncName[40];
	int nFuncType;
	BOOL bSymmetric;
	T_MLFC_DT arFuncData;
	unsigned int nFuncId;

	void ConvertToString(T_MLFC_D& rData)
	{
		ConvertCharStr(FuncName, rData.FuncName, sizeof(FuncName));
		rData.nFuncType = nFuncType;
		rData.bSymmetric = bSymmetric;
		rData.arFuncData.Copy(arFuncData);
		rData.nFuncId = nFuncId;
	}
	void ConvertToChar(T_MLFC_D& rData)
	{
		ConvertStrChar(rData.FuncName, FuncName, sizeof(FuncName));
		nFuncType = rData.nFuncType;
		bSymmetric = rData.bSymmetric;
		arFuncData.Copy(rData.arFuncData);
		nFuncId = rData.nFuncId;
	}
};
struct T_MLFC_UDRD_D
{
	T_MLFC_K key;
	T_MLFC_D data;
};
struct T_MLFC_UDRD_D_CH
{
	T_MLFC_K key;
	T_MLFC_D_CH data;
};
struct T_MLFC_UDRD_D_CH_RW
{
	T_MLFC_K key;
	char FuncName[40];
	int nFuncType;
	BOOL bSymmetric;
	int nCountFuncData;
	unsigned int nFuncId;

	void GetMlfc(T_MLFC_K& rKey, T_MLFC_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.FuncName, FuncName, sizeof(rData.FuncName));
		rData.nFuncType = nFuncType;
		rData.bSymmetric = bSymmetric;
		rData.nFuncId = nFuncId;
	}
	void SetMlfc(const T_MLFC_K Key, const T_MLFC_D_CH& rData)
	{
		key = Key;
		memcpy(FuncName, rData.FuncName, sizeof(FuncName));
		nFuncType = rData.nFuncType;
		bSymmetric = rData.bSymmetric;
		nCountFuncData = (int)rData.arFuncData.GetSize();
		nFuncId = rData.nFuncId;
	}
};
struct T_MLFC_D_CH_955
{
	char FuncName[20];
	int nFuncType;    
	BOOL bSymmetric;
	T_MLFC_DT arFuncData;
	unsigned int nFuncId;
	
	void ConvertToString(T_MLFC_D& rData)
	{
		ConvertCharStr(FuncName, rData.FuncName, sizeof(FuncName));
		rData.nFuncType = nFuncType;
		rData.bSymmetric = bSymmetric;
		rData.arFuncData.Copy(arFuncData);
		rData.nFuncId = nFuncId;
	}
	void ConvertToChar(T_MLFC_D& rData)
	{
		ConvertStrChar(rData.FuncName, FuncName, sizeof(FuncName));
		nFuncType = rData.nFuncType;
		bSymmetric = rData.bSymmetric;
		arFuncData.Copy(rData.arFuncData);
		nFuncId = rData.nFuncId;
	}
};
struct T_MLFC_UDRD_D_CH_955
{
	T_MLFC_K key;
	T_MLFC_D_CH_955 data;
};
struct T_MLFC_UDRD_D_CH_RW_955
{
	T_MLFC_K key;
	char FuncName[20];
	int nFuncType;
	BOOL bSymmetric;
	int nCountFuncData;
	unsigned int nFuncId;
	
	void GetMlfc(T_MLFC_K& rKey, T_MLFC_D_CH_955& rData)
	{
		rKey = key;
		memcpy(rData.FuncName, FuncName, sizeof(rData.FuncName));
		rData.nFuncType = nFuncType;
		rData.bSymmetric = bSymmetric;
		rData.nFuncId = nFuncId;
	}
};
struct T_MLFC_UNIT
{
	T_MLFC_UNIT()
	{
		dValueX[0] = D_UNITSYS_BASE_LENGTH;  // nFuncType == 0
		dValueX[1] = D_UNITSYS_BASE_RADIAN;  // nFuncType == 1
		dValueY[0] = D_UNITSYS_BASE_FORCE;   // nFuncType == 0
		dValueY[1] = D_UNITSYS_BASE_MOMENT;  // nFuncType == 1
	}
	int dValueX[2];
	int dValueY[2];
};
#define HASHSIZEMLFC 1001


//////////////////////////////////////////////////////////////////////////
// Rail-Track-Interaction-Elink Function
#define T_RLFC_K unsigned int
struct T_RLFC_D
{
	CString FuncName;
	int     nFuncType; // 0:Force, 1:Moment
	BOOL    bSymmetric;
	double  dLimDisp;  // Limited Disp.
	double  dUnLoadF;  // UnLoaded Fmax
	double  dLoadF;    // Loaded Fmax
	double  dLoadF_L;  // Left Loaded Fmax
	double  dLoadF_R;  // Right Loaded Fmax

	unsigned int nFuncId;

	void Initialize()
	{
		FuncName.Empty();
		nFuncType = 0;
		bSymmetric = TRUE;
		dLimDisp = 0.0;
		dUnLoadF = 0.0;
		dLoadF   = 0.0;
		dLoadF_L = 0.0;
		dLoadF_R = 0.0;
		nFuncId = 0;
	}
	T_RLFC_D() {}
	T_RLFC_D(T_RLFC_D& src) { *this = src; }
	T_RLFC_D& operator=(const T_RLFC_D& src)
	{
		FuncName = src.FuncName;
		nFuncType = src.nFuncType;
		bSymmetric = src.bSymmetric;
		dLimDisp = src.dLimDisp;
		dUnLoadF = src.dUnLoadF;
		dLoadF   = src.dLoadF;
		dLoadF_L = src.dLoadF_L;
		dLoadF_R = src.dLoadF_R;
		nFuncId = src.nFuncId;

		return *this;
	}
};

struct T_RLFC_D_CH
{
	char FuncName[20];
	int nFuncType;
	BOOL bSymmetric;
	double  dLimDisp;  // Limited Disp.
	double  dUnLoadF;  // UnLoaded Fmax
	double  dLoadF;    // Loaded Fmax
	double  dLoadF_L;  // Left Loaded Fmax
	double  dLoadF_R;  // Right Loaded Fmax
	unsigned int nFuncId;

	void ConvertToString(T_RLFC_D& rData)
	{
		ConvertCharStr(FuncName, rData.FuncName, sizeof(FuncName));
		rData.nFuncType = nFuncType;
		rData.bSymmetric = bSymmetric;
		rData.dLimDisp = dLimDisp;
		rData.dUnLoadF = dUnLoadF;
		rData.dLoadF   = dLoadF;
		rData.dLoadF_L = dLoadF_L;
		rData.dLoadF_R = dLoadF_R;
		rData.nFuncId = nFuncId;
	}
	void ConvertToChar(T_RLFC_D& rData)
	{
		ConvertStrChar(rData.FuncName, FuncName, sizeof(FuncName));
		nFuncType = rData.nFuncType;
		bSymmetric = rData.bSymmetric;
		dLimDisp = rData.dLimDisp;
		dUnLoadF = rData.dUnLoadF;
		dLoadF   = rData.dLoadF;
		dLoadF_L = rData.dLoadF_L;
		dLoadF_R = rData.dLoadF_R;
		nFuncId  = rData.nFuncId;
	}
};
struct T_RLFC_UDRD_D
{
	T_RLFC_K key;
	T_RLFC_D data;
};
struct T_RLFC_UDRD_D_CH
{
	T_RLFC_K key;
	T_RLFC_D_CH data;
};
struct T_RLFC_UDRD_D_CH_RW
{
	T_RLFC_K key;
	char FuncName[20];
	int nFuncType;
	BOOL bSymmetric;
	double  dLimDisp;  // Limited Disp.
	double  dUnLoadF;  // UnLoaded Fmax
	double  dLoadF;    // Loaded Fmax
	double  dLoadF_L;  // Left Loaded Fmax
	double  dLoadF_R;  // Right Loaded Fmax
	unsigned int nFuncId;

	void GetRlfc(T_RLFC_K& rKey, T_RLFC_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.FuncName, FuncName, sizeof(rData.FuncName));
		rData.nFuncType = nFuncType;
		rData.bSymmetric = bSymmetric;
		rData.dLimDisp = dLimDisp;
		rData.dUnLoadF = dUnLoadF;
		rData.dLoadF = dLoadF;
		rData.dLoadF_L = dLoadF_L;
		rData.dLoadF_R = dLoadF_R;
		rData.nFuncId = nFuncId;
	}
	void SetRlfc(const T_RLFC_K Key, const T_RLFC_D_CH& rData)
	{
		key = Key;
		memcpy(FuncName, rData.FuncName, sizeof(FuncName));
		nFuncType = rData.nFuncType;
		bSymmetric = rData.bSymmetric;
		dLimDisp = rData.dLimDisp;
		dUnLoadF = rData.dUnLoadF;
		dLoadF   = rData.dLoadF;
		dLoadF_L = rData.dLoadF_L;
		dLoadF_R = rData.dLoadF_R;

		nFuncId = rData.nFuncId;
	}
};
struct T_RLFC_UNIT
{
	T_RLFC_UNIT()
	{
		dLimDisp = D_UNITSYS_BASE_LENGTH;  
		dUnLoadF = D_UNITSYS_BASE_FORCE; 
		dLoadF   = D_UNITSYS_BASE_FORCE;
		dLoadF_L = D_UNITSYS_BASE_FORCE;
		dLoadF_R = D_UNITSYS_BASE_FORCE;
	}
	double  dLimDisp;  // Limited Disp.
	double  dUnLoadF;  // UnLoaded Fmax
	double  dLoadF  ;  // Loaded Fmax
	double  dLoadF_L;  // Left Loaded Fmax
	double  dLoadF_R;  // Right Loaded Fmax
};
#define HASHSIZERLFC 1001

/////////////////////////////////////////////////////////////////////////

// Rail-Track Interaction Elnk Type
#define T_RELT_K unsigned int
struct T_RELT_D
{
	T_ELNK_K ElnkKey;
	int nStatus;  // 0:Unloaded, 1:Loaded  2: Left Loaded  3: Right Loaded
	T_BNGR_K GroupKey;

	void Initialize()
	{
		ElnkKey = 0;
		nStatus = 0;
		GroupKey = 0;
	}
};
struct T_RELT_UDRD_D
{
	T_RELT_K key;
	T_RELT_D data;
};
struct T_RELT_UNIT
{
	T_RELT_UNIT()
	{
		nStatus = D_UNITSYS_NONE;
	}
	int nStatus;
};
#define HASHSIZERELT HASHSIZEELEM

//////////////////////////////////////////////////////////////////////////


// Ignore Elements for Load Cases
#define T_IELC_KEY unsigned int
union T_IELC_K
{
	T_IELC_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int stldid : 12;   // 0-4096
	}key;
};

struct T_IELC_D
{
	BOOL bIgnore;
	void Initialize()
	{
		bIgnore=FALSE;
	}
};
struct T_IELC_UDRD_D
{
	T_IELC_K key;
	T_IELC_D data;
};
struct T_IELC_UNIT
{
	T_IELC_UNIT()
	{
		bIgnore=D_UNITSYS_NONE;
	}
	int bIgnore;
};
#define HASHSIZEIELC HASHSIZEELEM



#pragma pack(pop)

#endif  // __DB_ST_DT_BNDR_H__
