#ifndef __DB_ST_DT_MASS_H__
#define __DB_ST_DT_MASS_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// Nodal Mass
#define T_NMAS_K T_NODE_K
struct T_NMAS_D_650
{
	double Mass[6];
};
struct T_NMAS_D
{
	double Mass[6];
	double dAngle[3];         // 6.6.0 추가
	double LocalVector[3][3]; // 6.6.0 추가    
	void Initialize()
	{    
		memset(Mass,0,sizeof(Mass));
		memset(dAngle,0,sizeof(dAngle));        
		memset(LocalVector,0,sizeof(LocalVector));
		LocalVector[0][0]=1.;
		LocalVector[1][1]=1.;
		LocalVector[2][2]=1.;    
	}
	void Convert650(T_NMAS_D_650& rData)
	{
		memcpy(Mass, rData.Mass, sizeof(Mass));
		for(int i = 0; i < 3; i++) dAngle[i] = 0.;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				LocalVector[i][j] = 0.;
		LocalVector[0][0]=1.;
		LocalVector[1][1]=1.;
		LocalVector[2][2]=1.;    
	}
};
struct T_NMAS_UDRD_D_650
{
	T_NMAS_K key;
	T_NMAS_D_650 data;
};
struct T_NMAS_UDRD_D
{
	T_NMAS_K key;
	T_NMAS_D data;
};
struct T_NMAS_UNIT
{
	T_NMAS_UNIT()
	{
		nCount=6;
		nUnit[0]=D_UNITSYS_BASE_MASS;
		nUnit[1]=D_UNITSYS_BASE_MASS;
		nUnit[2]=D_UNITSYS_BASE_MASS;
		nUnit[3]=D_UNITSYS_BASE_ROMASS;
		nUnit[4]=D_UNITSYS_BASE_ROMASS;
		nUnit[5]=D_UNITSYS_BASE_ROMASS;
		for(int j=0; j<nCount; j++)Mass[j]=nUnit[j];
	}
	int Mass[6];
	int nCount;
	int nUnit[6];
};
#define HASHSIZENMAS HASHSIZENODE

// Diaphragm Mass
#define T_DMAS_KEY unsigned int
union T_DMAS_K
{
	T_DMAS_KEY keymap;
	struct
	{
		unsigned int entity : 16;   // story
		unsigned int serial : 16;   // id
	}key;
};
struct T_DMAS_POINT
{
	double dPointX;
	double dPointY;
	double dPointMass;
	double dRotationMass;
	void Initialize()
	{
		dPointX=0.;
		dPointY=0.;
		dPointMass=0.;
		dRotationMass=0.;
	}
};
struct T_DMAS_LINE
{
	double dPointX;
	double dPointY;
	double dLineMass;
	double dLength;
	void Initialize()
	{
		dPointX=0.;
		dPointY=0.;
		dLineMass=0.;
		dLength=0.;
	}
};
struct T_DMAS_TRIANGLE
{
	double dP1X;
	double dP1Y;
	double dP2X;
	double dP2Y;
	double dP3X;
	double dP3Y;
	double dAreaMass;
	void Initialize()
	{
		dP1X=0.;
		dP1Y=0.;
		dP2X=0.;
		dP2Y=0.;
		dP3X=0.;
		dP3Y=0.;
		dAreaMass=0.;
	}
};
struct T_DMAS_RECT
{
	double dPointX;
	double dPointY;
	double dLength1;
	double dLength2;
	double dAreaMass;
	void Initialize()
	{
		dPointX=0.;
		dPointY=0.;
		dLength1=0.;
		dLength2=0.;
		dAreaMass=0.;
	}
};
struct T_DMAS_CIRARC
{
	double dPointX;
	double dPointY;
	double dArcAngle;
	double dArcRadius;
	double dCenterAngle;
	double dLineMass;
	void Initialize()
	{
		dPointX=0.;
		dPointY=0.;
		dArcAngle=0.;
		dArcRadius=0.;
		dCenterAngle=0.;
		dLineMass=0.;
	}
};
struct T_DMAS_CIRAREA
{
	double dPointX;
	double dPointY;
	double dArcAngle;
	double dArcRadius;
	double dArcWidth;
	double dCenterAngle;
	double dAreaMass;
	void Initialize()
	{
		dPointX=0.;
		dPointY=0.;
		dArcAngle=0.;
		dArcRadius=0.;
		dArcWidth=0.;
		dCenterAngle=0.;
		dAreaMass=0.;
	}
};
struct T_DMAS_RESULT
{
	double dPointX;
	double dPointY;
	double dPointMass;
	double dRotationMass;
	void Initialize()
	{
		dPointX=0.;
		dPointY=0.;
		dPointMass=0.;
		dRotationMass=0.;
	}
};
struct T_DMAS_D
{
	int nMassType;        // 1=Point 2=Line 3=Triangle 4=Rect 5=Circular Arc 6=Circular Area
	T_DMAS_POINT    Point;
	T_DMAS_LINE     Line;
	T_DMAS_TRIANGLE Triangle;
	T_DMAS_RECT     Rect;
	T_DMAS_CIRARC   CirArc;
	T_DMAS_CIRAREA  CirArea;
	T_DMAS_RESULT   Result;
	void Initialize()
	{
		nMassType=0;
		Point.Initialize();
		Line.Initialize();
		Triangle.Initialize();
		Rect.Initialize();
		CirArc.Initialize();
		CirArea.Initialize();
		Result.Initialize();
	}
};
struct T_DMAS_UDRD_D
{
	T_DMAS_K key;
	T_DMAS_D data;
};
struct T_DMAS_UNIT
{
	T_DMAS_UNIT()
	{
		nMassType=D_UNITSYS_NONE;
		POINT_dPointX=D_UNITSYS_BASE_LENGTH;
		POINT_dPointY=D_UNITSYS_BASE_LENGTH;
		POINT_dPointMass=D_UNITSYS_BASE_MASS;
		POINT_dRotationMass=D_UNITSYS_BASE_ROMASS;
		LINE_dPointX=D_UNITSYS_BASE_LENGTH;
		LINE_dPointY=D_UNITSYS_BASE_LENGTH;
		LINE_dLineMass=D_UNITSYS_BASE_LINEMASS;
		LINE_dLength=D_UNITSYS_BASE_LENGTH;
		TRIANGLE_dP1X=D_UNITSYS_BASE_LENGTH;
		TRIANGLE_dP1Y=D_UNITSYS_BASE_LENGTH;
		TRIANGLE_dP2X=D_UNITSYS_BASE_LENGTH;
		TRIANGLE_dP2Y=D_UNITSYS_BASE_LENGTH;
		TRIANGLE_dP3X=D_UNITSYS_BASE_LENGTH;
		TRIANGLE_dP3Y=D_UNITSYS_BASE_LENGTH;
		TRIANGLE_dAreaMass=D_UNITSYS_BASE_AREAMASS;
		RECT_dPointX=D_UNITSYS_BASE_LENGTH;
		RECT_dPointY=D_UNITSYS_BASE_LENGTH;
		RECT_dLength1=D_UNITSYS_BASE_LENGTH;
		RECT_dLength2=D_UNITSYS_BASE_LENGTH;
		RECT_dAreaMass=D_UNITSYS_BASE_AREAMASS;
		CIRARC_dPointX=D_UNITSYS_BASE_LENGTH;
		CIRARC_dPointY=D_UNITSYS_BASE_LENGTH;
		CIRARC_dArcAngle=D_UNITSYS_BASE_DEGREE;
		CIRARC_dArcRadius=D_UNITSYS_BASE_LENGTH;
		CIRARC_dCenterAngle=D_UNITSYS_BASE_DEGREE;
		CIRARC_dLineMass=D_UNITSYS_BASE_LINEMASS;
		CIRAREA_dPointX=D_UNITSYS_BASE_LENGTH;
		CIRAREA_dPointY=D_UNITSYS_BASE_LENGTH;
		CIRAREA_dArcAngle=D_UNITSYS_BASE_DEGREE;
		CIRAREA_dArcRadius=D_UNITSYS_BASE_LENGTH;
		CIRAREA_dArcWidth=D_UNITSYS_BASE_LENGTH;
		CIRAREA_dCenterAngle=D_UNITSYS_BASE_DEGREE;
		CIRAREA_dAreaMass=D_UNITSYS_BASE_AREAMASS;
		RESULT_dPointX=D_UNITSYS_BASE_LENGTH;
		RESULT_dPointY=D_UNITSYS_BASE_LENGTH;
		RESULT_dPointMass=D_UNITSYS_BASE_MASS;
		RESULT_dRotationMass=D_UNITSYS_BASE_ROMASS;
	}
	int nMassType;
	int POINT_dPointX;
	int POINT_dPointY;
	int POINT_dPointMass;
	int POINT_dRotationMass;
	int LINE_dPointX;
	int LINE_dPointY;
	int LINE_dLineMass;
	int LINE_dLength;
	int TRIANGLE_dP1X;
	int TRIANGLE_dP1Y;
	int TRIANGLE_dP2X;
	int TRIANGLE_dP2Y;
	int TRIANGLE_dP3X;
	int TRIANGLE_dP3Y;
	int TRIANGLE_dAreaMass;
	int RECT_dPointX;
	int RECT_dPointY;
	int RECT_dLength1;
	int RECT_dLength2;
	int RECT_dAreaMass;
	int CIRARC_dPointX;
	int CIRARC_dPointY;
	int CIRARC_dArcAngle;
	int CIRARC_dArcRadius;
	int CIRARC_dCenterAngle;
	int CIRARC_dLineMass;
	int CIRAREA_dPointX;
	int CIRAREA_dPointY;
	int CIRAREA_dArcAngle;
	int CIRAREA_dArcRadius;
	int CIRAREA_dArcWidth;
	int CIRAREA_dCenterAngle;
	int CIRAREA_dAreaMass;
	int RESULT_dPointX;
	int RESULT_dPointY;
	int RESULT_dPointMass;
	int RESULT_dRotationMass;
}; 
#define HASHSIZEDMAS 101

// Load to Mass
#define D_LTOM_NUMLTOM_721  15
#define D_LTOM_NUMLTOM_841  50
#define D_LTOM_NUMLTOM  200
#define T_LTOM_K unsigned int
struct T_LTOM_BASE
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
	}
};
struct T_LTOM_D_721
{
	int nDirection;   // 1=X 2=Y 3=Z 4=XY 5=YZ 6=XZ 7=XYZ
	BOOL bNodalLoad;
	BOOL bBeamLoad;
	BOOL bFloorLoad;
	BOOL bPressureLoad;
	double dGravity;
	int nNumLtomCase;
	T_LTOM_BASE LtomCase[D_LTOM_NUMLTOM_721];
};
struct T_LTOM_D_841
{
	int nDirection;   // 1=X 2=Y 3=Z 4=XY 5=YZ 6=XZ 7=XYZ
	BOOL bNodalLoad;
	BOOL bBeamLoad;
	BOOL bFloorLoad;
	BOOL bPressureLoad;
	double dGravity;
	int nNumLtomCase;
	T_LTOM_BASE LtomCase[D_LTOM_NUMLTOM_841];
};
struct T_LTOM_D
{
	int nDirection;   // 1=X 2=Y 3=Z 4=XY 5=YZ 6=XZ 7=XYZ
	BOOL bNodalLoad;
	BOOL bBeamLoad;
	BOOL bFloorLoad;
	BOOL bPressureLoad;
	double dGravity;
	int nNumLtomCase;
	T_LTOM_BASE LtomCase[D_LTOM_NUMLTOM];
	void Initialize()
	{
		nDirection=0;
		bNodalLoad=FALSE;
		bBeamLoad=FALSE;
		bFloorLoad=FALSE;
		bPressureLoad=FALSE;
		dGravity=0.;
		nNumLtomCase=0;
		for(int i=0; i<D_LTOM_NUMLTOM; i++)LtomCase[i].Initialize();
	}
	void Convert721(T_LTOM_D_721& rData)
	{
		nDirection=rData.nDirection;
		bNodalLoad=rData.bNodalLoad;
		bBeamLoad=rData.bBeamLoad;
		bFloorLoad=rData.bFloorLoad;
		bPressureLoad=rData.bPressureLoad;
		dGravity=rData.dGravity;
		nNumLtomCase=rData.nNumLtomCase;
		int i;  
		for(int i = 0; i<D_LTOM_NUMLTOM; i++)     LtomCase[i].Initialize();
		for(    i=0; i<D_LTOM_NUMLTOM_721; i++) LtomCase[i]=rData.LtomCase[i];
	}
	void Convert841(T_LTOM_D_841& rData)
	{
		nDirection=rData.nDirection;
		bNodalLoad=rData.bNodalLoad;
		bBeamLoad=rData.bBeamLoad;
		bFloorLoad=rData.bFloorLoad;
		bPressureLoad=rData.bPressureLoad;
		dGravity=rData.dGravity;
		nNumLtomCase=rData.nNumLtomCase;
		int i;  
		for(int i = 0; i<D_LTOM_NUMLTOM; i++)     LtomCase[i].Initialize();
		for(    i=0; i<D_LTOM_NUMLTOM_841; i++) LtomCase[i]=rData.LtomCase[i];
	}
};
struct T_LTOM_UDRD_D
{
	T_LTOM_D data;
};
struct T_LTOM_UDRD_D_721
{
	T_LTOM_D_721 data;
};
struct T_LTOM_UDRD_D_841
{
	T_LTOM_D_841 data;
};
struct T_LTOM_UNIT
{
	T_LTOM_UNIT()
	{
		nDirection=D_UNITSYS_NONE;
		bNodalLoad=D_UNITSYS_NONE;
		bBeamLoad=D_UNITSYS_NONE;
		bFloorLoad=D_UNITSYS_NONE;
		bPressureLoad=D_UNITSYS_NONE;
		dGravity=D_UNITSYS_BASE_GRAVITY;
		nNumLtomCase=D_UNITSYS_NONE;
		LoadCaseKey=D_UNITSYS_NONE;
		dblFactor=D_UNITSYS_NONE;
	}
	int nDirection;
	int bNodalLoad;
	int bBeamLoad;
	int bFloorLoad;
	int bPressureLoad;
	int dGravity;
	int nNumLtomCase;
	int LoadCaseKey;
	int dblFactor;
};
#define HASHSIZELTOM 1

#pragma pack(pop)

#endif  // __DB_ST_DT_MASS_H__
