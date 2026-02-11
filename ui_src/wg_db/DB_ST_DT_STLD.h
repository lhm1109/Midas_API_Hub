#ifndef __DB_ST_DT_STLD_H__
#define __DB_ST_DT_STLD_H__

#define MIDAS_SDK

#pragma pack(push, 1)

//#define T_BODF_K T_STLD_K
#define T_BODF_K unsigned int

struct T_BODF_D_431
{
	double SX, SY, SZ;
	void Initialize()
	{
		SX=0.;
		SY=0.;
		SZ=0.;
	}
};
struct T_BODF_UDRD_D_431
{
	T_STLD_K key;
	T_BODF_D_431 data;
};
struct T_BODF_D
{
	T_STLD_K LoadCaseKey;
	double SX, SY, SZ;
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey = 0;
		SX=0.;
		SY=0.;
		SZ=0.;
		GroupKey = 0;
	}
	void Convert431(T_STLD_K Key, T_BODF_D_431& rData)
	{
		LoadCaseKey = Key;
		SX = rData.SX;
		SY = rData.SY;
		SZ = rData.SZ;
		GroupKey = 0;
	}
};
struct T_BODF_UDRD_D
{
	T_BODF_K key;
	T_BODF_D data;
};
struct T_BODF_UNIT
{
	T_BODF_UNIT()
	{
		SX=D_UNITSYS_NONE;
		SY=D_UNITSYS_NONE;
		SZ=D_UNITSYS_NONE;
	}
	int SX;
	int SY;
	int SZ;
};
#define HASHSIZEBODF 1

#define T_CNLD_KEY unsigned int
union T_CNLD_K
{
	T_CNLD_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

struct T_CNLD_D_431
{
	T_STLD_K LoadCaseKey;
	double Force[6];        // F,F,F,F*L,F*L,F*L
	void Initialize()
	{
		LoadCaseKey=0;
		for(int i=0; i<6; i++)Force[i]=0.;
	}
};

struct T_CNLD_D_895
{
	T_STLD_K LoadCaseKey;
	double Force[6];        // F,F,F,F*L,F*L,F*L
	T_LDGR_K GroupKey;      // Load Group Key
	void Initialize()
	{
		LoadCaseKey=0;
		for(int i=0; i<6; i++)Force[i]=0.;
		GroupKey = 0;
	}
};

struct T_CNLD_D
{
	T_STLD_K LoadCaseKey;
	double Force[6];        // F,F,F,F*L,F*L,F*L
	T_LDGR_K GroupKey;      // Load Group Key
	CString strType;		// CH:Type Name
	void Initialize()
	{
		LoadCaseKey=0;
		for(int i=0; i<6; i++)Force[i]=0.;
		GroupKey = 0;
	}
	BOOL operator==(const T_CNLD_D& src) const
	{
		if (LoadCaseKey != src.LoadCaseKey)  return FALSE;
		for (int i=0; i<6; i++)
		{
			if (Force[i] != src.Force[i])      return FALSE;
		}
		if (GroupKey != src.GroupKey)        return FALSE;

		if (strType.Compare(src.strType) != 0)	return FALSE;

		return TRUE;
	}
};
struct T_CNLD_D_CH
{
	T_STLD_K LoadCaseKey;
	double Force[6];        // F,F,F,F*L,F*L,F*L
	T_LDGR_K GroupKey;      // Load Group Key
	char strType[80];		// CH:Type Name
	void Initialize()
	{
		LoadCaseKey=0;
		for(int i=0; i<6; i++)Force[i]=0.;
		GroupKey = 0;
		memset(strType, 0, sizeof(strType));
	}
	void ConvertToString(T_CNLD_D& rData)
	{
		rData.LoadCaseKey	= LoadCaseKey;
		rData.GroupKey		= GroupKey;
		memcpy(rData.Force, Force, sizeof(Force));
		ConvertCharStr(strType, rData.strType, sizeof(strType));
	}
	void ConvertToChar(T_CNLD_D& rData)
	{
		LoadCaseKey		= rData.LoadCaseKey;
		GroupKey		= rData.GroupKey;
		memcpy(Force, rData.Force, sizeof(rData.Force));
		ConvertStrChar(rData.strType, strType, sizeof(strType));
	}
	void Convert895(T_CNLD_D_895& rData)
	{
		LoadCaseKey		= rData.LoadCaseKey;
		GroupKey		= rData.GroupKey;
		memcpy(Force, rData.Force, sizeof(rData.Force));
		memset(strType, 0, sizeof(strType));
	}
	void Convert431(T_CNLD_D_431& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		memcpy((void*)Force, (const void*)rData.Force, sizeof(Force));
		GroupKey = 0;
		memset(strType, 0, sizeof(strType));
	}
};

struct T_CNLD_UDRD_D_431
{
	T_CNLD_K key;
	T_CNLD_D_431 data;
};
struct T_CNLD_UDRD_D_895
{
	T_CNLD_K key;
	T_CNLD_D_895 data;
};
struct T_CNLD_UDRD_D_CH
{
	T_CNLD_K key;
	T_CNLD_D_CH data;
};
struct T_CNLD_UDRD_D
{
	T_CNLD_K key;
	T_CNLD_D data;
};
struct T_CNLD_UNIT
{
	T_CNLD_UNIT()
	{
		nCount=7;
		nUnit[0]=D_UNITSYS_NONE;
		nUnit[1]=D_UNITSYS_BASE_FORCE;
		nUnit[2]=D_UNITSYS_BASE_FORCE;
		nUnit[3]=D_UNITSYS_BASE_FORCE;
		nUnit[4]=D_UNITSYS_BASE_MOMENT;
		nUnit[5]=D_UNITSYS_BASE_MOMENT;
		nUnit[6]=D_UNITSYS_BASE_MOMENT;
		LoadCaseKey=nUnit[0];
		Force[0]=nUnit[1];
		Force[1]=nUnit[2];
		Force[2]=nUnit[3];
		Force[3]=nUnit[4];
		Force[4]=nUnit[5];
		Force[5]=nUnit[6];
	}
	int LoadCaseKey;
	int Force[6];
	int nCount;
	int nUnit[7];
};
#define HASHSIZECNLD HASHSIZENODE

#define T_SDSP_KEY unsigned int
#define SDSP_SIZE 7
union T_SDSP_K
{
	T_SDSP_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_SDSP_D_431
{
	T_STLD_K LoadCaseKey;
	unsigned char Flag[8];  // 2개는 사용되지 않음, '1'=강제변위유효 '0'=없음
	double Displacement[6];     // F,F,F,F*L,F*L,F*L
	void Initialize()
	{
		LoadCaseKey=0;
		for(int i=0; i<8; i++)Flag[i]='0';
		for(int j=0; j<6; j++)Displacement[j]=0.;
	}
};
struct T_SDSP_D_815
{
	T_STLD_K LoadCaseKey;
	unsigned char Flag[8];  // 2개는 사용되지 않음, '1'=강제변위유효 '0'=없음
	double Displacement[6];     // F,F,F,F*L,F*L,F*L
	T_LDGR_K GroupKey;      // Load Group Key
	void Initialize()
	{
		LoadCaseKey=0;
		for(int i=0; i<8; i++)Flag[i]='0';
		for(int j=0; j<6; j++)Displacement[j]=0.;
	}
};
struct T_SDSP_D
{
	T_STLD_K LoadCaseKey;
	unsigned char Flag[8];  // 1개는 사용되지 않음, '1'=강제변위유효 '0'=없음
	double Displacement[SDSP_SIZE];     // F,F,F,F*L,F*L,F*L,F*L
	T_LDGR_K GroupKey;      // Load Group Key
	void Initialize()
	{
		LoadCaseKey=0;
		for(int i=0; i<8; i++)Flag[i]='0';
		for(int j=0; j<SDSP_SIZE; j++)Displacement[j]=0.;
	}
	void Convert431(T_SDSP_D_431& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		memcpy((void*)Flag, (const void*)rData.Flag, sizeof(Flag));
		memcpy((void*)Displacement, (const void*)rData.Displacement, sizeof(rData.Displacement));
		GroupKey = 0;
	}
	void Convert815(T_SDSP_D_815& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		memcpy((void*)Flag, (const void*)rData.Flag, sizeof(Flag));
		memcpy((void*)Displacement, (const void*)rData.Displacement, sizeof(rData.Displacement));
		GroupKey = rData.GroupKey;
	}
};
struct T_SDSP_UDRD_D_431
{
	T_SDSP_K key;
	T_SDSP_D_431 data;
};
struct T_SDSP_UDRD_D_815
{
	T_SDSP_K key;
	T_SDSP_D_815 data;
};
struct T_SDSP_UDRD_D
{
	T_SDSP_K key;
	T_SDSP_D data;
};
struct T_SDSP_UNIT
{
	T_SDSP_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		Flag=D_UNITSYS_NONE;
		Displacement[0]=D_UNITSYS_BASE_DISPLACEMENT;
		Displacement[1]=D_UNITSYS_BASE_DISPLACEMENT;
		Displacement[2]=D_UNITSYS_BASE_DISPLACEMENT;
		Displacement[3]=D_UNITSYS_BASE_RADIAN;
		Displacement[4]=D_UNITSYS_BASE_RADIAN;
		Displacement[5]=D_UNITSYS_BASE_RADIAN;
		Displacement[6]=D_UNITSYS_BASE_RADIAN;
	}
	int LoadCaseKey;
	int Flag;
	int Displacement[SDSP_SIZE];
};
#define HASHSIZESDSP HASHSIZENODE

#define D_BMLDTYPE_BEAM           1
#define D_BMLDTYPE_FLOOR          2
#define D_BMLDTYPE_TEMPORARY      3
#define D_BMLDTYPE_PRESTRESS      4
#define D_BMLDTYPE_MOVING         5
#define D_BMLDTYPE_LINE           6
#define D_BMLDTYPE_TYPICAL        7
#define T_BMLD_KEY unsigned int
union T_BMLD_K
{
	T_BMLD_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_BMLD_D_40
{
	T_STLD_K LoadCaseKey;
	int LoadCommandType;  // 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
	int LoadType;         // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	double DistanceRatio[4];  // 0.에서 1. 사이의 값
	double Force_Type1[4];    // LoadType=1 일 경우 사용
	double Force_Type2[4];    // LoadType=2 일 경우 사용
	double Force_Type3[4];    // LoadType=3 일 경우 사용
	double Force_Type4[4];    // LoadType=4 일 경우 사용
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandType=0;
		LoadType=0;
		LoadDirection=0;
		Projection=0;
		for(int i=0; i<4; i++)
		{
			DistanceRatio[i]=0.;
			Force_Type1[i]=0.;
			Force_Type2[i]=0.;
			Force_Type3[i]=0.;
			Force_Type4[i]=0.;
		}
	}
};
struct T_BMLD_UDRD_D_40
{
	T_BMLD_K key;
	T_BMLD_D_40 data;
};
struct T_BMLD_D_431
{
	T_STLD_K LoadCaseKey;
	int LoadCommandType;  // 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
	int LoadType;         // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=VECTOR
	double Vector[3];     // LoadDirection = 7 일때만 사용
	int Projection;       // 0,1  1=Projection
	double DistanceRatio[4];  // 0.에서 1. 사이의 값
	double Force_Type[4][4]; 
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandType=0;
		LoadType=0;
		LoadDirection=0;
		Projection=0;
		Vector[0]=0.;
		Vector[1]=0.;
		Vector[2]=0.;
		for(int i=0; i<4; i++)
		{
			DistanceRatio[i]=0.;
			for(int j=0; j<4; j++)Force_Type[i][j]=0.;
		}
	}
};
struct T_BMLD_UDRD_D_431
{
	T_BMLD_K key;
	T_BMLD_D_431 data;
};
struct T_BMLD_D_701
{
	T_STLD_K LoadCaseKey;
	int LoadCommandType;  // 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
	int LoadType;         // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=VECTOR
	double Vector[3];     // LoadDirection = 7 일때만 사용
	int Projection;       // 0,1  1=Projection
	double DistanceRatio[4];  // 0.에서 1. 사이의 값
	double Force_Type[4][4]; 
	T_LDGR_K GroupKey;
};
struct T_BMLD_UDRD_D_701
{
	T_BMLD_K key;
	T_BMLD_D_701 data;
};
struct T_BMLD_D_780
{
	T_STLD_K LoadCaseKey;
	int LoadCommandType;  // 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
	int LoadType;         // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=VECTOR
	double Vector[3];     // LoadDirection = 7 일때만 사용
	int Projection;       // 0,1  1=Projection
	double DistanceRatio[4];  // 0.에서 1. 사이의 값
	double Force_Type[4][4]; 
	T_LDGR_K GroupKey;
	BOOL bEccentricity;   // 7.0.2에서 추가
	int  nEccenDir;       // 7.0.2에서 추가 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ  
	double dDistI;        // 7.0.2에서 추가
	double dDistJ;        // 7.0.2에서 추가
	BOOL bDistJ;          // 7.0.2에서 추가
};
struct T_BMLD_UDRD_D_780
{
	T_BMLD_K key;
	T_BMLD_D_780 data;
};

struct T_BMLD_D_840
{
	T_STLD_K LoadCaseKey;     // LoadCase
	int LoadCommandType;      // 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
	int LoadType;             // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트, 5=압력하중
	int LoadDirection;        // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=VECTOR
	double Vector[3];         // LoadDirection = 7 일때만 사용
	int Projection;           // 0,1  1=Projection
	double DistanceRatio[4];  // 0.에서 1. 사이의 값
	double Force_Type[5][4];  // [0][i]=집중하중, [1][i]=집중모멘트, [2][i]=분포하중, [3][i]=분포모멘트, [4][i]=압력하중 
	T_LDGR_K GroupKey;        // Group
	BOOL bEccentricity;       // 7.0.2에서 추가 
	int  nEccenType;          // 7.8.5에서 추가 0:Centroid 1:Offset 
	int  nEccenDir;           // 7.0.2에서 추가 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ  
	double dDistI;            // 7.0.2에서 추가
	double dDistJ;            // 7.0.2에서 추가
	BOOL bDistJ;              // 7.0.2에서 추가
	BOOL bAdditionalH;        // Pressure 일 경우...  (7.8.5 추가)
	double dAdditionalDistI;  // Pressure 일 경우...  (7.8.5 추가)
	double dAdditionalDistJ;  // Pressure 일 경우...  (7.8.5 추가)
	BOOL bAdditionalDistJ;    // Pressure 일 경우...  (7.8.5 추가)
};
struct T_BMLD_UDRD_D_840
{
	T_BMLD_K key;
	T_BMLD_D_840 data;
};

struct T_BMLD_D
{
	T_STLD_K LoadCaseKey;     // LoadCase
	int LoadCommandType;      // 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
	int LoadType;             // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트, 5=압력하중
	int LoadDirection;        // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=VECTOR
	double Vector[3];         // LoadDirection = 7 일때만 사용
	int Projection;           // 0,1  1=Projection
	double DistanceRatio[4];  // 0.에서 1. 사이의 값
	double Force_Type[5][4];  // [0][i]=집중하중, [1][i]=집중모멘트, [2][i]=분포하중, [3][i]=분포모멘트, [4][i]=압력하중 
	T_LDGR_K GroupKey;        // Group
	BOOL bEccentricity;       // 7.0.2에서 추가 
	int  nEccenType;          // 7.8.5에서 추가 0:Centroid 1:Offset 
	int  nEccenDir;           // 7.0.2에서 추가 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ  
	double dDistI;            // 7.0.2에서 추가
	double dDistJ;            // 7.0.2에서 추가
	BOOL bDistJ;              // 7.0.2에서 추가
	BOOL bAdditionalH;        // Pressure 일 경우...  (7.8.5 추가)
	double dAdditionalDistI;  // Pressure 일 경우...  (7.8.5 추가)
	double dAdditionalDistJ;  // Pressure 일 경우...  (7.8.5 추가)
	BOOL bAdditionalDistJ;    // Pressure 일 경우...  (7.8.5 추가)
	CString Description;
	
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandType=0;
		LoadType=0;
		LoadDirection=0;
		Projection=0;
		Vector[0]=0.;
		Vector[1]=0.;
		Vector[2]=0.;
		for(int i=0; i<4; i++)
		{
			DistanceRatio[i]=0.;
		}
		for(int i=0; i<5; i++)
		{      
			for(int j=0; j<4; j++)Force_Type[i][j]=0.;
		}
		GroupKey=0;
		bEccentricity = FALSE;
		nEccenType = 0;
		nEccenDir = 2;  // LY
		dDistI = 0.0;
		dDistJ = 0.0;
		bDistJ = FALSE;
		bAdditionalH = FALSE;
		dAdditionalDistI = 0.0;
		dAdditionalDistJ = 0.0;
		bAdditionalDistJ = FALSE;
		Description.Empty();
	}
	// PARKHJ-20110518 : 일본2차설계 계산서에 사용하기 위해 추가.
	BOOL operator==(const T_BMLD_D& src) const
	{
		if (LoadCaseKey     != src.LoadCaseKey)     return FALSE;
		//if (LoadCommandType != src.LoadCommandType) return FALSE;  // 적용타입이 달라도 T_BMLD_D의 값은 같을수 있음.
		if (LoadType        != src.LoadType)        return FALSE;    // LoadCommandType 은 따로 비교할 것!
		if (LoadDirection   != src.LoadDirection)   return FALSE;
		for (int k=0; k<3; k++)
		{
			if (Vector[k]     != src.Vector[k])       return FALSE;
		}
		if (Projection      != src.Projection)      return FALSE;
		for (int k=0; k<4; k++                                    )
		{
			if (DistanceRatio[k] != src.DistanceRatio[k]) return FALSE;
		}
		for (int k=0; k<5; k++)
		{
			for (int i=0; i<4; i++)
			{
				if (Force_Type[k][i] != src.Force_Type[k][i]) return FALSE;
			}
		}
		if (GroupKey         != src.GroupKey)         return FALSE;
		if (bEccentricity    != src.bEccentricity)    return FALSE;
		if (nEccenType       != src.nEccenType)       return FALSE;
		if (nEccenDir        != src.nEccenDir)        return FALSE;
		if (dDistI           != src.dDistI)           return FALSE;
		if (dDistJ           != src.dDistJ)           return FALSE;
		if (bDistJ           != src.bDistJ)           return FALSE;
		if (bAdditionalH     != src.bAdditionalH)     return FALSE;
		if (dAdditionalDistI != src.dAdditionalDistI) return FALSE;
		if (dAdditionalDistJ != src.dAdditionalDistJ) return FALSE;
		if (bAdditionalDistJ != src.bAdditionalDistJ) return FALSE;
		
		//if (Description.Compare(src.Description) != 0)  return FALSE;
		
		return TRUE;
	}
};

struct T_BMLD_D_CH
{
	T_STLD_K LoadCaseKey;     // LoadCase
	int LoadCommandType;      // 1=BeamLoad  2=FloorLoad  3=Use Temporary 4=Prestress 5=MovingLoad 6=LineLoad 7=TypicalLoad
	int LoadType;             // 1=집중하중, 2=집중모멘트, 3=분포하중, 4=분포모멘트, 5=압력하중
	int LoadDirection;        // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=VECTOR
	double Vector[3];         // LoadDirection = 7 일때만 사용
	int Projection;           // 0,1  1=Projection
	double DistanceRatio[4];  // 0.에서 1. 사이의 값
	double Force_Type[5][4];  // [0][i]=집중하중, [1][i]=집중모멘트, [2][i]=분포하중, [3][i]=분포모멘트, [4][i]=압력하중 
	T_LDGR_K GroupKey;        // Group
	BOOL bEccentricity;       // 7.0.2에서 추가 
	int  nEccenType;          // 7.8.5에서 추가 0:Centroid 1:Offset 
	int  nEccenDir;           // 7.0.2에서 추가 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ  
	double dDistI;            // 7.0.2에서 추가
	double dDistJ;            // 7.0.2에서 추가
	BOOL bDistJ;              // 7.0.2에서 추가
	BOOL bAdditionalH;        // Pressure 일 경우...  (7.8.5 추가)
	double dAdditionalDistI;  // Pressure 일 경우...  (7.8.5 추가)
	double dAdditionalDistJ;  // Pressure 일 경우...  (7.8.5 추가)
	BOOL bAdditionalDistJ;    // Pressure 일 경우...  (7.8.5 추가)
	char Description[80];

	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandType=0;
		LoadType=0;
		LoadDirection=0;
		Projection=0;
		Vector[0]=0.;
		Vector[1]=0.;
		Vector[2]=0.;
		for(int i=0; i<4; i++)
		{
			DistanceRatio[i]=0.;
		}
		for(int i=0; i<5; i++)
		{
			for(int j=0; j<4; j++)Force_Type[i][j]=0.;
		}
		GroupKey=0;
		bEccentricity = FALSE;
		nEccenType = 0;
		nEccenDir = 2;  // LY
		dDistI = 0.0;
		dDistJ = 0.0;
		bDistJ = FALSE;
		bAdditionalH = FALSE;
		dAdditionalDistI = 0.0;
		dAdditionalDistJ = 0.0;
		bAdditionalDistJ = FALSE;
		memset(Description, 0, sizeof(Description));
	}

	void Convert40(T_BMLD_D_40& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandType=rData.LoadCommandType;
		LoadType=rData.LoadType;
		LoadDirection=rData.LoadDirection;
		Projection=rData.Projection;
		Vector[0]=0.;
		Vector[1]=0.;
		Vector[2]=0.;
		for(int i=0; i<4; i++)DistanceRatio[i]=rData.DistanceRatio[i];
		for(int i=0; i<4; i++)Force_Type[0][i]=rData.Force_Type1[i];
		for(int i=0; i<4; i++)Force_Type[1][i]=rData.Force_Type2[i];
		for(int i=0; i<4; i++)Force_Type[2][i]=rData.Force_Type3[i];
		for(int i=0; i<4; i++)Force_Type[3][i]=rData.Force_Type4[i];
		for(int i=0; i<4; i++)Force_Type[4][i]=0.0;
		GroupKey=0;
		bEccentricity = FALSE;
		nEccenType = 0;
		nEccenDir = 2;  // LY
		dDistI = 0.0;
		dDistJ = 0.0;
		bDistJ = FALSE;
		bAdditionalH = FALSE;
		dAdditionalDistI = 0.0;
		dAdditionalDistJ = 0.0;
		bAdditionalDistJ = FALSE;
		memset(Description, 0, sizeof(Description));
	}
	void Convert431(T_BMLD_D_431& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadCommandType = rData.LoadCommandType;
		LoadType = rData.LoadType;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		memcpy((void*)Vector, (const void*)rData.Vector, sizeof(Vector));
		memcpy((void*)DistanceRatio, (const void*)rData.DistanceRatio, sizeof(DistanceRatio));
		//memcpy((void*)Force_Type, (const void*)rData.Force_Type, sizeof(Force_Type));
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)Force_Type[i][j]=rData.Force_Type[i][j];
		}
		for(int i=0; i<4; i++)Force_Type[4][i]=0.0;
		GroupKey = 0;
		bEccentricity = FALSE;
		nEccenType = 0;
		nEccenDir = 2;  // LY
		dDistI = 0.0;
		dDistJ = 0.0;
		bDistJ = FALSE;
		bAdditionalH = FALSE;
		dAdditionalDistI = 0.0;
		dAdditionalDistJ = 0.0;
		bAdditionalDistJ = FALSE;
		memset(Description, 0, sizeof(Description));
	}
	void Convert701(T_BMLD_D_701& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadCommandType = rData.LoadCommandType;
		LoadType = rData.LoadType;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		memcpy((void*)Vector, (const void*)rData.Vector, sizeof(Vector));
		memcpy((void*)DistanceRatio, (const void*)rData.DistanceRatio, sizeof(DistanceRatio));
		//memcpy((void*)Force_Type, (const void*)rData.Force_Type, sizeof(Force_Type));
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)Force_Type[i][j]=rData.Force_Type[i][j];
		}
		for(int i=0; i<4; i++)Force_Type[4][i]=0.0;
		GroupKey = rData.GroupKey;
		bEccentricity = FALSE;
		nEccenType = 0;
		nEccenDir = 2;  // LY
		dDistI = 0.0;
		dDistJ = 0.0;
		bDistJ = FALSE;
		bAdditionalH = FALSE;
		dAdditionalDistI = 0.0;
		dAdditionalDistJ = 0.0;
		bAdditionalDistJ = FALSE;
		memset(Description, 0, sizeof(Description));
	}
	void Convert780(T_BMLD_D_780& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadCommandType = rData.LoadCommandType;
		LoadType = rData.LoadType;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		memcpy((void*)Vector, (const void*)rData.Vector, sizeof(Vector));
		memcpy((void*)DistanceRatio, (const void*)rData.DistanceRatio, sizeof(DistanceRatio));
		//memcpy((void*)Force_Type, (const void*)rData.Force_Type, sizeof(Force_Type));
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)Force_Type[i][j]=rData.Force_Type[i][j];
		}
		for(int i=0; i<4; i++)Force_Type[4][i]=0.0;
		GroupKey = rData.GroupKey;
		bEccentricity = rData.bEccentricity;
		nEccenType = 0;
		nEccenDir = rData.nEccenDir;
		dDistI = rData.dDistI;
		dDistJ = rData.dDistJ;
		bDistJ = rData.bDistJ;
		bAdditionalH = FALSE;
		dAdditionalDistI = 0.0;
		dAdditionalDistJ = 0.0;
		bAdditionalDistJ = FALSE;
		memset(Description, 0, sizeof(Description));
	}
	void Convert840(T_BMLD_D_840& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadCommandType = rData.LoadCommandType;
		LoadType = rData.LoadType;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		memcpy((void*)Vector, (const void*)rData.Vector, sizeof(Vector));
		memcpy((void*)DistanceRatio, (const void*)rData.DistanceRatio, sizeof(DistanceRatio));
		//memcpy((void*)Force_Type, (const void*)rData.Force_Type, sizeof(Force_Type));
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)Force_Type[i][j]=rData.Force_Type[i][j];
		}
		for(int i=0; i<4; i++)Force_Type[4][i]=rData.Force_Type[4][i];
		GroupKey = rData.GroupKey;
		bEccentricity = rData.bEccentricity;
		nEccenType = rData.nEccenType;
		nEccenDir = rData.nEccenDir;
		dDistI = rData.dDistI;
		dDistJ = rData.dDistJ;
		bDistJ = rData.bDistJ;
		bAdditionalH = rData.bAdditionalH;
		dAdditionalDistI = rData.dAdditionalDistI;
		dAdditionalDistJ = rData.dAdditionalDistJ;
		bAdditionalDistJ = rData.bAdditionalDistJ;
		memset(Description, 0, sizeof(Description));
	}
};

// DB에 직접 저장되는 구조체가 아니고 계산과정에서 활용됨.    
struct T_BMLD_UDRD_D
{
	T_BMLD_K key;
	T_BMLD_D data;
	BOOL bLocalM; // 편심 보하중을 모멘트 하중으로 추가 한것, CAttrCtrl::ConvertEccToLocalMoment 에서 사용   

	T_BMLD_UDRD_D()
	{
		Initialize();
	}
// 	T_BMLD_UDRD_D(T_BMLD_UDRD_D& src) { *this = src; }
// 	T_BMLD_UDRD_D& operator=(const T_BMLD_UDRD_D& src)
// 	{
// 		ElnkD = src.ElnkD;
// 		RlfcD = src.RlfcD;
// 
// 		return *this;
// 	}
	void Initialize()
	{
		key.keymap = 0;
		data.Initialize();
		bLocalM = FALSE;
	}
};

struct T_BMLD_UDRD_D_CH
{
	T_BMLD_K key;
	T_BMLD_D_CH data;
};

struct T_BMLD_UNIT
{
	T_BMLD_UNIT()
	{
		LoadCommandType=D_UNITSYS_NONE;
		LoadType=D_UNITSYS_NONE;
		LoadDirection=D_UNITSYS_NONE;
		Vector=D_UNITSYS_BASE_LENGTH;
		Projection=D_UNITSYS_NONE;
		DistanceRatio=D_UNITSYS_NONE;
		Force_Type[0]=D_UNITSYS_BASE_FORCE;
		Force_Type[1]=D_UNITSYS_BASE_MOMENT;
		Force_Type[2]=D_UNITSYS_BASE_UNITFORCE;
		Force_Type[3]=D_UNITSYS_BASE_UNITMOMENT;
		Force_Type[4]=D_UNITSYS_BASE_STRESS;
		dW=D_UNITSYS_BASE_UNITFORCE;
		dDist=D_UNITSYS_BASE_LENGTH;
		dAdditionalDist=D_UNITSYS_BASE_LENGTH;
	}
	int LoadCommandType;
	int LoadType;
	int LoadDirection;
	int Vector;
	int Projection;
	int DistanceRatio;
	int Force_Type[5];
	int dW;
	int dDist;
	int dAdditionalDist;
};
#define HASHSIZEBMLD 997

struct T_TBLD_D
{
	T_STLD_K LoadCaseKey;
	int LoadType;         // 1=등분포, 2=삼각형 분포, 3=사다리꼴 분포, 4=삼각2+집중1, 5=삼각3+집중2, 6=삼각4+집중3
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	double FloorLoad;
	double Width;
	double Factor;
	double dW;            // Sub-Beam Unit Self Weight
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		LoadType=0;
		LoadDirection=0;
		Projection=0;
		FloorLoad=0.;
		Width=0.;
		Factor=0.;
		dW = 0.0;
		GroupKey = 0;
	}
};

#define FBLD_NUM 8
#define T_FBLD_K unsigned int
struct T_FBLD_D
{
	CString FloorLoadTypeName;
	CString Description;
	T_STLD_K LoadCaseKey[FBLD_NUM];
	BOOL bSubBeamWeight[FBLD_NUM];
	double FloorLoad[FBLD_NUM];
	unsigned int FloorLoadTypeId;    // 순번

	T_FBLD_D(){}

	T_FBLD_D(T_FBLD_D& src)
	{
		*this = src;
	}

	T_FBLD_D& operator=(T_FBLD_D& src)
	{
		FloorLoadTypeName = src.FloorLoadTypeName;
		Description = src.Description;
		for(int i=0; i<FBLD_NUM; i++)
		{
			LoadCaseKey[i]=src.LoadCaseKey[i];
			bSubBeamWeight[i]=src.bSubBeamWeight[i];
			FloorLoad[i]=src.FloorLoad[i];
		}
		FloorLoadTypeId=src.FloorLoadTypeId;
		return *this;
	}

	void Initialize()
	{
		FloorLoadTypeName.Empty();
		Description.Empty();
		memset(LoadCaseKey, 0, sizeof(LoadCaseKey));
		memset(bSubBeamWeight, 0, sizeof(bSubBeamWeight));
		bSubBeamWeight[0]=TRUE;
		memset(FloorLoad, 0, sizeof(FloorLoad));
		FloorLoadTypeId=0;
	}
};
struct T_FBLD_D_CH_412
{
	char FloorLoadTypeName[40];
	char Description[80];
	T_STLD_K LoadCaseKey[4];
	double FloorLoad[4];
	unsigned int FloorLoadTypeId;    // 순번
};
struct T_FBLD_D_CH_790
{
	char FloorLoadTypeName[40];
	char Description[80];
	T_STLD_K LoadCaseKey[4];
	BOOL bSubBeamWeight[4];
	double FloorLoad[4];
	unsigned int FloorLoadTypeId;    // 순번
};
struct T_FBLD_D_CH
{
	char FloorLoadTypeName[40];
	char Description[80];
	T_STLD_K LoadCaseKey[FBLD_NUM];
	BOOL bSubBeamWeight[FBLD_NUM];
	double FloorLoad[FBLD_NUM];
	unsigned int FloorLoadTypeId;    // 순번
	void Initialize()
	{
		memset(FloorLoadTypeName, 0, sizeof(FloorLoadTypeName));
		memset(Description, 0, sizeof(Description));
		for(int i=0; i<FBLD_NUM; i++)
		{
			LoadCaseKey[i]=0;
			bSubBeamWeight[i]=FALSE;
			FloorLoad[i]=0.0;
		}
		FloorLoadTypeId=0;
	}
	void Convert412(T_FBLD_D_CH_412& rData)
	{
		Initialize();
		memcpy(FloorLoadTypeName, rData.FloorLoadTypeName, sizeof(FloorLoadTypeName));
		memcpy(Description, rData.Description, sizeof(Description));
		for(int i=0; i<4; i++)
		{
			LoadCaseKey[i]=rData.LoadCaseKey[i];
			bSubBeamWeight[i]=FALSE;
			if(LoadCaseKey[i] != 0)bSubBeamWeight[i]=TRUE;
			FloorLoad[i]=rData.FloorLoad[i];
		}
		FloorLoadTypeId=rData.FloorLoadTypeId;
	}
	void Convert790(T_FBLD_D_CH_790& rData)
	{
		Initialize();
		memcpy(FloorLoadTypeName, rData.FloorLoadTypeName, sizeof(FloorLoadTypeName));
		memcpy(Description, rData.Description, sizeof(Description));
		for(int i=0; i<4; i++)
		{
			LoadCaseKey[i]=rData.LoadCaseKey[i];
			bSubBeamWeight[i]=rData.bSubBeamWeight[i];
			FloorLoad[i]=rData.FloorLoad[i];
		}
		FloorLoadTypeId=rData.FloorLoadTypeId;
	}
};
struct T_FBLD_UDRD_D
{
	T_FBLD_K key;
	T_FBLD_D data;
};
struct T_FBLD_UDRD_D_CH_412
{
	T_FBLD_K key;
	T_FBLD_D_CH_412 data;
};
struct T_FBLD_UDRD_D_CH_790
{
	T_FBLD_K key;
	T_FBLD_D_CH_790 data;
};
struct T_FBLD_UDRD_D_CH
{
	T_FBLD_K key;
	T_FBLD_D_CH data;
};
struct T_FBLD_UNIT
{
	T_FBLD_UNIT()
	{
		FloorLoadTypeName=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		LoadCaseKey=D_UNITSYS_NONE;
		bSubBeamWeight=D_UNITSYS_NONE;
		FloorLoad=D_UNITSYS_BASE_PRESSURE;
		FloorLoadTypeId=D_UNITSYS_NONE;
	}
	int FloorLoadTypeName;
	int Description;
	int LoadCaseKey;
	int bSubBeamWeight;
	int FloorLoad;
	int FloorLoadTypeId;
}; 
#define HASHSIZEFBLD 31

#define T_FBLA_K unsigned int
#define D_FBLA_MAXNOD_550 15
#define D_FBLA_MAXNOD     32

#define D_FBLTYPE_ONEWAY        1
#define D_FBLTYPE_TWOWAY        2
#define D_FBLTYPE_POLY_CENTROID 3
#define D_FBLTYPE_POLY_LENGTH   4

struct T_FBLA_D_CH_431
{
	T_FBLD_K LoadTypeKey;
	int nDistributionType;  // 1=One Way, 2=Two Way;
	double LoadAngle;
	int nSubBeam;
	double SubBeamAngle;
	double SubBeamUnitWeight;
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	T_NODE_K nConnectivity[D_FBLA_MAXNOD_550]; // Loading Boundary Node;
	char Description[80];
};
struct T_FBLA_UDRD_D_CH_431
{
	T_FBLA_K key;
	T_FBLA_D_CH_431 data;
};

struct T_FBLA_D_CH_550
{
	T_FBLD_K LoadTypeKey;
	int nDistributionType;  // 1=One Way, 2=Two Way;
	double LoadAngle;
	int nSubBeam;
	double SubBeamAngle;
	double SubBeamUnitWeight;
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	T_NODE_K nConnectivity[D_FBLA_MAXNOD_550]; // Loading Boundary Node;
	char Description[80];
	T_LDGR_K GroupKey;
};
struct T_FBLA_UDRD_D_CH_550
{
	T_FBLA_K key;
	T_FBLA_D_CH_550 data;
};
struct T_FBLA_D_CH_671
{
	T_FBLD_K LoadTypeKey;
	int nDistributionType;  // 1=One Way, 2=Two Way, 3=Polygon-Centroid, 4=Polygon-Length
	double LoadAngle;
	int nSubBeam;
	double SubBeamAngle;
	double SubBeamUnitWeight;
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	T_NODE_K nConnectivity[D_FBLA_MAXNOD]; // Loading Boundary Node;
	char Description[80];
	T_LDGR_K GroupKey;
};
struct T_FBLA_UDRD_D_CH_671
{
	T_FBLA_K key;
	T_FBLA_D_CH_671 data;
};

struct T_FBLA_D
{
	T_FBLD_K LoadTypeKey;
	int nDistributionType;  // 1=One Way, 2=Two Way, 3=Polygon-Centroid, 4=Polygon-Length
	double LoadAngle;
	int nSubBeam;
	double SubBeamAngle;
	double SubBeamUnitWeight;
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	T_NODE_K nConnectivity[D_FBLA_MAXNOD]; // Loading Boundary Node;
	CString Description;
	T_LDGR_K GroupKey;
	BOOL bExcludeInnerElemArea;     // 6.8.0에서 추가
	BOOL bAllowPolygonTypeUnitArea; // 6.8.0에서 추가
	void Initialize()
	{
		LoadTypeKey=0;
		nDistributionType=0;
		LoadAngle=0;
		nSubBeam=0;
		SubBeamAngle=0.;
		SubBeamUnitWeight=0.;
		LoadDirection=0;
		Projection=0;
		memset(nConnectivity, 0, sizeof(nConnectivity));
		Description.Empty();
		GroupKey = 0;
		bExcludeInnerElemArea = FALSE;
		bAllowPolygonTypeUnitArea = FALSE;
	}
};
struct T_FBLA_D_CH
{
	T_FBLD_K LoadTypeKey;
	int nDistributionType;  // 1=One Way, 2=Two Way, 3=Polygon-Centroid, 4=Polygon-Length
	double LoadAngle;
	int nSubBeam;
	double SubBeamAngle;
	double SubBeamUnitWeight;
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	T_NODE_K nConnectivity[D_FBLA_MAXNOD]; // Loading Boundary Node;
	char Description[80];
	T_LDGR_K GroupKey;
	BOOL bExcludeInnerElemArea;     // 6.8.0에서 추가
	BOOL bAllowPolygonTypeUnitArea; // 6.8.0에서 추가
	void ConvertToString(T_FBLA_D& rData)
	{
		rData.LoadTypeKey = LoadTypeKey;
		rData.nDistributionType = nDistributionType;
		rData.LoadAngle = LoadAngle;
		rData.nSubBeam = nSubBeam;
		rData.SubBeamAngle = SubBeamAngle;
		rData.SubBeamUnitWeight = SubBeamUnitWeight;
		rData.LoadDirection = LoadDirection;
		rData.Projection = Projection;
		memcpy((void*)rData.nConnectivity, (const void*)nConnectivity, sizeof(nConnectivity));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.GroupKey = GroupKey;
		rData.bExcludeInnerElemArea = bExcludeInnerElemArea;
		rData.bAllowPolygonTypeUnitArea = bAllowPolygonTypeUnitArea;
	}
	void ConvertToChar(T_FBLA_D& rData)
	{
		LoadTypeKey = rData.LoadTypeKey;
		nDistributionType = rData.nDistributionType;
		LoadAngle = rData.LoadAngle;
		nSubBeam = rData.nSubBeam;
		SubBeamAngle = rData.SubBeamAngle;
		SubBeamUnitWeight = rData.SubBeamUnitWeight;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		memcpy((void*)nConnectivity, (const void*)rData.nConnectivity, sizeof(nConnectivity));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		GroupKey = rData.GroupKey;
		bExcludeInnerElemArea = rData.bExcludeInnerElemArea;
		bAllowPolygonTypeUnitArea = rData.bAllowPolygonTypeUnitArea;
	}
	void Convert431(T_FBLA_D_CH_431& rData)
	{
		LoadTypeKey = rData.LoadTypeKey;
		nDistributionType = rData.nDistributionType;
		LoadAngle = rData.LoadAngle;
		nSubBeam = rData.nSubBeam;
		SubBeamAngle = rData.SubBeamAngle;
		SubBeamUnitWeight = rData.SubBeamUnitWeight;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		for (int i = 0; i < D_FBLA_MAXNOD_550; i++)
			nConnectivity[i] = rData.nConnectivity[i];
		for (int i = D_FBLA_MAXNOD_550; i < D_FBLA_MAXNOD; i++)
			nConnectivity[i] = 0;
		memcpy((void*)Description, (const void*)rData.Description, sizeof(Description));
		GroupKey = 0;
		bExcludeInnerElemArea = FALSE;
		bAllowPolygonTypeUnitArea = FALSE;
	}
	void Convert550(T_FBLA_D_CH_550& rData)
	{
		LoadTypeKey = rData.LoadTypeKey;
		nDistributionType = rData.nDistributionType;
		LoadAngle = rData.LoadAngle;
		nSubBeam = rData.nSubBeam;
		SubBeamAngle = rData.SubBeamAngle;
		SubBeamUnitWeight = rData.SubBeamUnitWeight;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		//memcpy((void*)nConnectivity, (const void*)rData.nConnectivity, sizeof(nConnectivity));
		for (int i = 0; i < D_FBLA_MAXNOD_550; i++)
			nConnectivity[i] = rData.nConnectivity[i];
		for (int i = D_FBLA_MAXNOD_550; i < D_FBLA_MAXNOD; i++)
			nConnectivity[i] = 0;
		memcpy((void*)Description, (const void*)rData.Description, sizeof(Description));
		GroupKey = rData.GroupKey;
		bExcludeInnerElemArea = FALSE;
		bAllowPolygonTypeUnitArea = FALSE;
	}
	void Convert671(T_FBLA_D_CH_671& rData)
	{
		LoadTypeKey = rData.LoadTypeKey;
		nDistributionType = rData.nDistributionType;
		LoadAngle = rData.LoadAngle;
		nSubBeam = rData.nSubBeam;
		SubBeamAngle = rData.SubBeamAngle;
		SubBeamUnitWeight = rData.SubBeamUnitWeight;
		LoadDirection = rData.LoadDirection;
		Projection = rData.Projection;
		memcpy((void*)nConnectivity, (const void*)rData.nConnectivity, sizeof(nConnectivity));
		memcpy((void*)Description, (const void*)rData.Description, sizeof(Description));
		GroupKey = rData.GroupKey;
		bExcludeInnerElemArea = FALSE;
		bAllowPolygonTypeUnitArea = FALSE;
	}
};
struct T_FBLA_UDRD_D
{
	T_FBLA_K key;
	T_FBLA_D data;
};
struct T_FBLA_UDRD_D_CH
{
	T_FBLA_K key;
	T_FBLA_D_CH data;
};
struct T_FBLA_UNIT
{
	T_FBLA_UNIT()
	{
		nCount=10;
		nUnit[0]=D_UNITSYS_NONE;
		nUnit[1]=D_UNITSYS_NONE;
		nUnit[2]=D_UNITSYS_BASE_DEGREE;
		nUnit[3]=D_UNITSYS_NONE;
		nUnit[4]=D_UNITSYS_BASE_DEGREE;
		nUnit[5]=D_UNITSYS_BASE_UNITFORCE;
		nUnit[6]=D_UNITSYS_NONE;
		nUnit[7]=D_UNITSYS_NONE;
		nUnit[8]=D_UNITSYS_NONE;
		nUnit[9]=D_UNITSYS_NONE;
		LoadTypeKey=nUnit[0];
		nDistributionType=nUnit[1];  // 1=One Way, 2=Two Way;
		LoadAngle=nUnit[2];
		nSubBeam=nUnit[3];
		SubBeamAngle=nUnit[4];
		SubBeamUnitWeight=nUnit[5];
		LoadDirection=nUnit[6];    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
		Projection=nUnit[7];       // 0,1  1=Projection
		nConnectivity=nUnit[8]; // Loading Boundary Node;
		Description=nUnit[9];
	}
	int LoadTypeKey;
	int nDistributionType;  // 1=One Way, 2=Two Way;
	int LoadAngle;
	int nSubBeam;
	int SubBeamAngle;
	int SubBeamUnitWeight;
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ
	int Projection;       // 0,1  1=Projection
	int nConnectivity; // Loading Boundary Node;
	int Description;
	int nCount;
	int nUnit[10];
}; 
#define HASHSIZEFBLA HASHSIZEFBLD

// prestress/post-stress
#define T_PRST_KEY unsigned int
union T_PRST_K
{
	T_PRST_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_PRST_D_431
{
	T_STLD_K LoadCaseKey;
	int LoadType;  // 1=Prestress 2=Post-stress
	double dTension;
	double dDistanceI;
	double dDistanceM;
	double dDistanceJ;
	void Initialize()
	{
		LoadCaseKey=0;
		LoadType=0;
		dTension=0.;
		dDistanceI=0.;
		dDistanceM=0.;
		dDistanceJ=0.;
	}
};
struct T_PRST_D_640
{
	T_STLD_K LoadCaseKey;
	int LoadType;  // 1=Prestress 2=Post-stress
	double dTension;
	double dDistanceI;
	double dDistanceM;
	double dDistanceJ;
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		LoadType=0;
		dTension=0.;
		dDistanceI=0.;
		dDistanceM=0.;
		dDistanceJ=0.;
		GroupKey = 0;
	}
};
struct T_PRST_D_860
{
	T_STLD_K LoadCaseKey;
	int LoadType;  // 1=Prestress 2=Post-stress
	double dTension;
	double dDistanceI;
	double dDistanceM;
	double dDistanceJ;
	T_LDGR_K GroupKey;
	double dTensionJ;     // 6.5.0추가 : DYNA2E
	void Initialize()
	{
		LoadCaseKey=0;
		LoadType=0;
		dTension=0.;
		dDistanceI=0.;
		dDistanceM=0.;
		dDistanceJ=0.;
		GroupKey = 0;
		dTensionJ = 0.;
	}
};
struct T_PRST_D
{
	T_STLD_K LoadCaseKey;
	int LoadType;  // 1=Prestress 2=Post-stress
	int nDirection;   // 0=Local y, 1=Local z
	double dTension;
	double dDistanceI;
	double dDistanceM;
	double dDistanceJ;
	T_LDGR_K GroupKey;
	double dTensionJ;     // 6.5.0추가 : DYNA2E
	void Initialize()
	{
		LoadCaseKey=0;
		LoadType=0;
		dTension=0.;
		dDistanceI=0.;
		dDistanceM=0.;
		dDistanceJ=0.;
		GroupKey = 0;
		dTensionJ = 0.;
		nDirection = 1;
	}
	void Convert431(T_PRST_D_431& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadType = rData.LoadType;
		dTension = rData.dTension;
		dDistanceI = rData.dDistanceI;
		dDistanceM = rData.dDistanceM;
		dDistanceJ = rData.dDistanceJ;
		GroupKey = 0;
		dTensionJ = rData.dTension;
		nDirection = 1;
	}
	void Convert640(T_PRST_D_640& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadType    = rData.LoadType;
		dTension    = rData.dTension;
		dDistanceI  = rData.dDistanceI;
		dDistanceM  = rData.dDistanceM;
		dDistanceJ  = rData.dDistanceJ;
		GroupKey    = rData.GroupKey;
		dTensionJ   = rData.dTension;
		nDirection  = 1;
	}
	void Convert860(T_PRST_D_860& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadType    = rData.LoadType;
		dTension    = rData.dTension;
		dDistanceI  = rData.dDistanceI;
		dDistanceM  = rData.dDistanceM;
		dDistanceJ  = rData.dDistanceJ;
		GroupKey    = rData.GroupKey;
		dTensionJ   = rData.dTension;
		nDirection  = 1;
	}
};
struct T_PRST_UDRD_D_431
{
	T_PRST_K key;
	T_PRST_D_431 data;
};
struct T_PRST_UDRD_D_640
{
	T_PRST_K key;
	T_PRST_D_640 data;
};
struct T_PRST_UDRD_D_860
{
	T_PRST_K key;
	T_PRST_D_860 data;
};
struct T_PRST_UDRD_D
{
	T_PRST_K key;
	T_PRST_D data;
};
struct T_PRST_UNIT
{
	T_PRST_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		LoadType=D_UNITSYS_NONE;
		dTension=D_UNITSYS_BASE_FORCE;
		dDistanceI=D_UNITSYS_BASE_LENGTH;
		dDistanceM=D_UNITSYS_BASE_LENGTH;
		dDistanceJ=D_UNITSYS_BASE_LENGTH;
	}
	int LoadCaseKey;
	int LoadType;
	int dTension;
	int dDistanceI;
	int dDistanceM;
	int dDistanceJ;
};
#define HASHSIZEPRST 997

// pretension
#define T_PTNS_KEY unsigned int
union T_PTNS_K
{
	T_PTNS_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_PTNS_D_431
{
	T_STLD_K LoadCaseKey;
	double dTension;
	void Initialize()
	{
		LoadCaseKey=0;
		dTension=0.;
	}
};
struct T_PTNS_UDRD_D_431
{
	T_PTNS_K key;
	T_PTNS_D_431 data;
};
struct T_PTNS_D
{
	T_STLD_K LoadCaseKey;
	double dTension;
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		dTension=0.;
		GroupKey = 0;
	}
	void Convert431(T_PTNS_D_431& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		dTension = rData.dTension;
		GroupKey = 0;
	}
};
struct T_PTNS_UDRD_D
{
	T_PTNS_K key;
	T_PTNS_D data;
};
struct T_PTNS_UNIT
{
	T_PTNS_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		dTension=D_UNITSYS_BASE_FORCE;
	}
	int LoadCaseKey;
	int dTension;
};
#define HASHSIZEPTNS 997

// pressure
#define T_PRES_KEY unsigned int
union T_PRES_K
{
	T_PRES_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_PRES_D_40
{
	T_STLD_K LoadCaseKey;
	int LoadType;         // 1=Plate 2=PlaneStress/Axisymmetric/PlaneStrain 3=Solid
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, PlaneStress/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	double Force[5];      // PU, P1, P2, P3, P4
	void Initialize()
	{
		LoadCaseKey=0;
		LoadType=0;
		LoadDirection=0;
		for(int i=0; i<5; i++)Force[i]=0.;
	}
};
struct T_PRES_UDRD_D_40
{
	T_PRES_K key;
	T_PRES_D_40 data;
};
struct T_PRES_D_413
{
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure
	int LoadType;         // 1=Plate 2=PlaneStress/Axisymmetric/PlaneStrain 3=Solid
	int LoadDirection;    // 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, PlaneStress/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	double Force[5];      // PU, P1, P2, P3, P4
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandTypeNew=0;
		LoadType=0;
		LoadDirection=0;
		for(int i=0; i<5; i++)Force[i]=0.;
	}
};
struct T_PRES_UDRD_D_413
{
	T_PRES_K key;
	T_PRES_D_413 data;
};
struct T_PRES_D_431
{
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure
	int LoadType;         // 1=Plate 2=PlaneStress/Axisymmetric/PlaneStrain 3=Solid
	int nEdgeFace;        // Plate는 사용안함, PlaneStress/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	int LoadDirection;    // Plate 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, PlaneStress/Axi/PlaneStrain은 사용안함, Solid는 1=Normal, 2=GX, 3=GY, 4=GZ
	int Projection;       // Plate,Solid 에만 0,1  1=Projection 
	double Force[5];      // PU, P1, P2, P3, P4
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandTypeNew=0;
		LoadType=0;
		nEdgeFace=0;
		LoadDirection=0;
		Projection=0;
		for(int i=0; i<5; i++)Force[i]=0.;
	}
};
struct T_PRES_UDRD_D_431
{
	T_PRES_K key;
	T_PRES_D_431 data;
};
struct T_PRES_D_510
{
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure
	int LoadType;            // 1=PlateLoad 2=PlaneStress/Axisymmetric/PlaneStrain 3=Solid
	int nSubLoadType;        // 0=Pressure(Plate,PlaneStress/Axisymmetric/PlaneStrain,Solid), 1=Concentrated Load(Plate), 2=단위길이당(Plate)
	int nEdgeFace;           // PlateFace는 사용안함, PlateEdge/PlaneStress/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	int LoadDirection;       // PlateFace 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, PlateEdge/PlaneStress/Axi/PlaneStrain은 사용안함, Solid는 1=Normal, 2=GX, 3=GY, 4=GZ
	int Projection;          // Plate(Pressure), Solid(Pressure) 에만 0,1  1=Projection 
	double Force[5];         // PU, P1, P2, P3, P4          (nSubLoadType=0 인 경우 사용)
	double PointLoad;        // 집중하중                    (nSubLoadType=1 인 경우 사용)
	double dGX, dGY, dGZ;    // 집중하중의 N1위치에서의 거리(nSubLoadType=1 인 경우 사용)
	double EdgeLoad[3];      // PU, P1, P2 단위길이당 하중  (nSubLoadType=2 인 경우 사용)
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandTypeNew=0;
		LoadType=0;
		nSubLoadType = 0;
		nEdgeFace=0;
		LoadDirection=0;
		Projection=0;
		for(int i=0; i<5; i++)Force[i]=0.;
		PointLoad = 0.0;
		dGX = dGY = dGZ = 0.0;
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		GroupKey = 0;
	}
};
struct T_PRES_UDRD_D_510
{
	T_PRES_K key;
	T_PRES_D_510 data;
};
struct T_PRES_D_530
{
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure
	int LoadType;            // 1=Plate/PlaneStress/Wall Load 2=Axisymmetric/PlaneStrain 3=Solid
	int nSubLoadType;        // 0=Pressure(Plate,Axisymmetric/PlaneStrain,Solid), 1=Concentrated Load(Plate), 2=단위길이당 Edge Load(Plate/PlaneStress/Wall)
	int nEdgeFace;           // PlateFace는 사용안함, Plate(PlaneStress,Wall)Edge/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	int LoadDirection;       // PlateFace 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, Plate(PlaneStress,Wall)Edge/Axi/PlaneStrain은 사용안함, Solid는 1=Normal, 2=GX, 3=GY, 4=GZ
	int Projection;          // Plate(Pressure), PlaneStress(Pressure), Solid(Pressure) 에만 0,1  1=Projection 
	double Force[5];         // PU, P1, P2, P3, P4          (nSubLoadType=0 인 경우 사용)
	double EdgeLoad[3];      // PU, P1, P2 단위길이당 하중  (nSubLoadType=2 인 경우 사용)
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandTypeNew=0;
		LoadType=0;
		nSubLoadType = 0;
		nEdgeFace=0;
		LoadDirection=0;
		Projection=0;
		for(int i=0; i<5; i++)Force[i]=0.;
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		GroupKey = 0;
	}
};
struct T_PRES_UDRD_D_530
{
	T_PRES_K key;
	T_PRES_D_530 data;
};
struct T_PRES_D_895
{
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure  5=Moving Load
	int LoadType;            // 1=Plate/PlaneStress/Wall Load 2=Axisymmetric/PlaneStrain 3=Solid
	int nSubLoadType;        // 0=Pressure(Plate/PlaneStress,Axisymmetric/PlaneStrain,Solid), 1=Concentrated Load(x), 2=단위길이당 Edge Load(Plate/PlaneStress/Wall)
	int nEdgeFace;           // PlateFace=1, Plate(PlaneStress,Wall)Edge/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	int LoadDirectionNew;    // 0=Normal, 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=Vector
													 // Plate(Face) : Local(1~3), Global(4~6), Vector(7)
													 // Plate(Edge) : Normal(0), Local(1~3), Global(4~6), Vector(7)
													 // Solid(Face) : Normal(0), Local(1~3), Global(4~6), Vector(7)
													 // Plane Strain(Edge) : Normal(0), LX(1), LY(2), Vector(7)
													 // Axisymmetric(Edge) : Normal(0), LX(1), LY(2), Vector(7)
													 // Wall(Edge)  : Normal(0), Local(1~3), Global(4~6), Vector(7)
	int Projection;          // LoadDirection == Global(4, 5, 6)에만 0,1  1=Projection 
	double Force[5];         // PU, P1, P2, P3, P4          (nSubLoadType=0 인 경우 사용)
	double EdgeLoad[3];      // PU, P1, P2 단위길이당 하중  (nSubLoadType=2 인 경우 사용)
	double Vector[3];        // LoadDirection == Vector인 경우
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandTypeNew=0;
		LoadType=0;
		nSubLoadType = 0;
		nEdgeFace=1;
		LoadDirectionNew=0;
		Projection=0;
		for(int i=0; i<5; i++)Force[i]=0.;
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
	}
	void Convert40(T_PRES_D_40& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=1;
		LoadType=rData.LoadType;
		nSubLoadType = 0;
		nEdgeFace=1;
		if(LoadType == 2 || LoadType == 3)nEdgeFace=rData.LoadDirection;

		LoadDirectionNew=rData.LoadDirection;
		if(LoadType == 2 || LoadType == 3)LoadDirectionNew=0; // Normal

		Projection=0;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
	}
	void Convert413(T_PRES_D_413& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = 0;
		nEdgeFace=1;
		if(LoadType ==2 || LoadType ==3)nEdgeFace=rData.LoadDirection;

		LoadDirectionNew=rData.LoadDirection;
		if(LoadType == 2 || LoadType == 3)LoadDirectionNew=0; // Normal

		Projection=0;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
	}
	void Convert431(T_PRES_D_431& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = 0;
		nEdgeFace=rData.nEdgeFace;
		if (LoadType == 1) nEdgeFace = 1;

		LoadDirectionNew=rData.LoadDirection;
		if (LoadType == 2) LoadDirectionNew = 0;  // Normal
		else if (LoadType == 3)
		{
			if (rData.LoadDirection == 1) LoadDirectionNew = 0;  // Normal
			else LoadDirectionNew = rData.LoadDirection + 2;     // GX, GY, GZ
		}

		Projection=rData.Projection;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
	}
	void Convert510(T_PRES_D_510& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = rData.nSubLoadType;
		nEdgeFace=rData.nEdgeFace;
		if (LoadType == 1 && nSubLoadType == 0) nEdgeFace = 1;

		LoadDirectionNew=rData.LoadDirection;
		if (LoadType == 1 && nSubLoadType == 2) LoadDirectionNew = 0; // Normal
		else if (LoadType == 2) LoadDirectionNew = 0;  // Normal
		else if (LoadType == 3)
		{
			if (rData.LoadDirection == 1) LoadDirectionNew = 0;  // Normal
			else LoadDirectionNew = rData.LoadDirection + 2;     // GX, GY, GZ
		}

		Projection=rData.Projection;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = rData.EdgeLoad[i];
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = rData.GroupKey;
	}
	void Convert530(T_PRES_D_530& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = rData.nSubLoadType;
		nEdgeFace=rData.nEdgeFace;
		if (LoadType == 1 && nSubLoadType == 0) nEdgeFace = 1;

		LoadDirectionNew=rData.LoadDirection;
		if (LoadType == 1 && nSubLoadType == 2) LoadDirectionNew = 0; // Normal
		else if (LoadType == 2) LoadDirectionNew = 0;  // Normal
		else if (LoadType == 3)
		{
			if (rData.LoadDirection == 1) LoadDirectionNew = 0;  // Normal
			else LoadDirectionNew = rData.LoadDirection + 2;     // GX, GY, GZ
		}

		Projection=rData.Projection;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = rData.EdgeLoad[i];
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = rData.GroupKey;
	}
};
struct T_PRES_UDRD_D_895
{
	T_PRES_K key;
	T_PRES_D_895 data;
};

struct T_PRES_D
{
	T_STLD_K LoadCaseKey;
	int LoadCommandTypeNew;  // 1=Pressure  2=Hydrostatic Pressure  5=Moving Load
	int LoadType;            // 1=Plate/PlaneStress/Wall Load 2=Axisymmetric/PlaneStrain 3=Solid
	int nSubLoadType;        // 0=Pressure(Plate/PlaneStress,Axisymmetric/PlaneStrain,Solid), 1=Concentrated Load(x), 2=단위길이당 Edge Load(Plate/PlaneStress/Wall)
	int nEdgeFace;           // PlateFace=1, Plate(PlaneStress,Wall)Edge/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	int LoadDirectionNew;    // 0=Normal, 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, 7=Vector
	// Plate(Face) : Local(1~3), Global(4~6), Vector(7)
	// Plate(Edge) : Normal(0), Local(1~3), Global(4~6), Vector(7)
	// Solid(Face) : Normal(0), Local(1~3), Global(4~6), Vector(7)
	// Plane Strain(Edge) : Normal(0), LX(1), LY(2), Vector(7)
	// Axisymmetric(Edge) : Normal(0), LX(1), LY(2), Vector(7)
	// Wall(Edge)  : Normal(0), Local(1~3), Global(4~6), Vector(7)
	int Projection;          // LoadDirection == Global(4, 5, 6)에만 0,1  1=Projection 
	double Force[5];         // PU, P1, P2, P3, P4          (nSubLoadType=0 인 경우 사용)
	double EdgeLoad[3];      // PU, P1, P2 단위길이당 하중  (nSubLoadType=2 인 경우 사용)
	double Vector[3];        // LoadDirection == Vector인 경우
	T_LDGR_K GroupKey;
	unsigned int PSltKey;
	void Initialize()
	{
		LoadCaseKey=0;
		LoadCommandTypeNew=0;
		LoadType=0;
		nSubLoadType = 0;
		nEdgeFace=1;
		LoadDirectionNew=0;
		Projection=0;
		for(int i=0; i<5; i++)Force[i]=0.;
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
		PSltKey = 0;
	}
	void Convert40(T_PRES_D_40& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=1;
		LoadType=rData.LoadType;
		nSubLoadType = 0;
		nEdgeFace=1;
		if(LoadType == 2 || LoadType == 3)nEdgeFace=rData.LoadDirection;

		LoadDirectionNew=rData.LoadDirection;
		if(LoadType == 2 || LoadType == 3)LoadDirectionNew=0; // Normal

		Projection=0;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
		PSltKey = 0;
	}
	void Convert413(T_PRES_D_413& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = 0;
		nEdgeFace=1;
		if(LoadType ==2 || LoadType ==3)nEdgeFace=rData.LoadDirection;

		LoadDirectionNew=rData.LoadDirection;
		if(LoadType == 2 || LoadType == 3)LoadDirectionNew=0; // Normal

		Projection=0;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
		PSltKey = 0;
	}
	void Convert431(T_PRES_D_431& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = 0;
		nEdgeFace=rData.nEdgeFace;
		if (LoadType == 1) nEdgeFace = 1;

		LoadDirectionNew=rData.LoadDirection;
		if (LoadType == 2) LoadDirectionNew = 0;  // Normal
		else if (LoadType == 3)
		{
			if (rData.LoadDirection == 1) LoadDirectionNew = 0;  // Normal
			else LoadDirectionNew = rData.LoadDirection + 2;     // GX, GY, GZ
		}

		Projection=rData.Projection;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = 0.0;
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = 0;
		PSltKey = 0;
	}
	void Convert510(T_PRES_D_510& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = rData.nSubLoadType;
		nEdgeFace=rData.nEdgeFace;
		if (LoadType == 1 && nSubLoadType == 0) nEdgeFace = 1;

		LoadDirectionNew=rData.LoadDirection;
		if (LoadType == 1 && nSubLoadType == 2) LoadDirectionNew = 0; // Normal
		else if (LoadType == 2) LoadDirectionNew = 0;  // Normal
		else if (LoadType == 3)
		{
			if (rData.LoadDirection == 1) LoadDirectionNew = 0;  // Normal
			else LoadDirectionNew = rData.LoadDirection + 2;     // GX, GY, GZ
		}

		Projection=rData.Projection;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = rData.EdgeLoad[i];
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = rData.GroupKey;
		PSltKey = 0;
	}
	void Convert530(T_PRES_D_530& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = rData.nSubLoadType;
		nEdgeFace=rData.nEdgeFace;
		if (LoadType == 1 && nSubLoadType == 0) nEdgeFace = 1;

		LoadDirectionNew=rData.LoadDirection;
		if (LoadType == 1 && nSubLoadType == 2) LoadDirectionNew = 0; // Normal
		else if (LoadType == 2) LoadDirectionNew = 0;  // Normal
		else if (LoadType == 3)
		{
			if (rData.LoadDirection == 1) LoadDirectionNew = 0;  // Normal
			else LoadDirectionNew = rData.LoadDirection + 2;     // GX, GY, GZ
		}

		Projection=rData.Projection;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = rData.EdgeLoad[i];
		for (int i = 0; i < 3; i++) Vector[i] = 0.0;
		GroupKey = rData.GroupKey;
		PSltKey = 0;
	}
	void Convert895(T_PRES_D_895& rData)
	{
		LoadCaseKey=rData.LoadCaseKey;
		LoadCommandTypeNew=rData.LoadCommandTypeNew;
		LoadType=rData.LoadType;
		nSubLoadType = rData.nSubLoadType;
		nEdgeFace=rData.nEdgeFace;
		//if (LoadType == 1 && nSubLoadType == 0) nEdgeFace = 1;
		LoadDirectionNew=rData.LoadDirectionNew;
		Projection=rData.Projection;
		for(int i=0; i<5; i++)Force[i]=rData.Force[i];
		for (int i = 0; i < 3; i++) EdgeLoad[i] = rData.EdgeLoad[i];
		for (int i = 0; i < 3; i++) Vector[i] = rData.Vector[i];
		GroupKey = rData.GroupKey;
		PSltKey = 0;
	}
};
struct T_PRES_UDRD_D
{
	T_PRES_K key;
	T_PRES_D data;
};
struct T_PRES_UNIT
{
	T_PRES_UNIT()
	{
		Force=D_UNITSYS_BASE_PRESSURE;
		Vector=D_UNITSYS_BASE_LENGTH;
		EdgeLoad=D_UNITSYS_BASE_UNITLOAD;
	}
	int Force;
	int Vector;
	int EdgeLoad;
};
#define HASHSIZEPRES HASHSIZEELEM

// hydrostatic pressure
struct T_HPRS_D
{
	T_STLD_K LoadCaseKey;

//  v4.1.3
//  int LoadType;         // 1=Plate 2=PlaneStress/Axisymmetric/PlaneStrain 3=Solid
//  int LoadDirection;    // 1=+LZ, 2=-LZ, PlaneStress/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호

	int LoadType;         // 1=Plate 2=PlaneStress/Axisymmetric/PlaneStrain 3=Solid
	int nEdgeFace;        // Plate는 1, PlaneStress/Axi/PlaneStrain은 Edge 번호, Solid는 Face 번호
	//int LoadDirection;    // Plate 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, PlaneStress/Axi/PlaneStrain은 사용안함, Solid는 1=Normal, 2=GX, 3=GY, 4=GZ
	int LoadDirectionNew; // Plate 1=LX, 2=LY, 3=LZ, 4=GX, 5=GY, 6=GZ, PlaneStress/Axi/PlaneStrain은 사용안함, Solid는 0=Normal, 4=GX, 5=GY, 6=GZ
	int Projection;       // Plate,Solid 에만 0,1  1=Projection 

	int nGradientDirection;  // 1=-GX, 2=-GY, 3=-GZ
	double dReferenceLevel;
	double dConstantIntensity;
	double dGradientIntensity;
	T_LDGR_K GroupKey;
	int    nLoadType;     // 6.2.0 추가 : 0=Linear Load, 1=Curved Load
	int    nCurveType;    // 6.2.0 추가 : 0=ax^0.5 + b, 1=ax^2 + bx + c
	double dFactor_a;     // 6.2.0 추가 
	double dFactor_b;     // 6.2.0 추가  
	double dFactor_c;     // 6.2.0 추가 
	void Initialize()
	{
		LoadCaseKey=0;
		LoadType=0;
		nEdgeFace=1;
		//LoadDirection=0;
		LoadDirectionNew=0;
		Projection=0;
		nGradientDirection=0;
		dReferenceLevel=0.;
		dConstantIntensity=0.;
		dGradientIntensity=0.;
		GroupKey = 0;
		nLoadType = 0;
		nCurveType = 0;
		dFactor_a = 0.;
		dFactor_b = 0.;
		dFactor_c = 0.;  
	}
};
struct T_HPRS_UNIT
{
	T_HPRS_UNIT()
	{
		dReferenceLevel=D_UNITSYS_BASE_LENGTH;
		dConstantIntensity=D_UNITSYS_BASE_PRESSURE;
		dGradientIntensity=D_UNITSYS_BASE_DENSITY;
	}
	int dReferenceLevel;
	int dConstantIntensity;
	int dGradientIntensity;
};

// System Temperature
//#define T_STMP_K T_STLD_K
#define T_STMP_K unsigned int
struct T_STMP_D_431
{
	double dblSysTemp;
	void Initialize()
	{
		dblSysTemp=0.;
	}
};
struct T_STMP_UDRD_D_431
{
	T_STLD_K key;
	T_STMP_D_431 data;
};
struct T_STMP_D
{
	T_STLD_K LoadCaseKey;
	double dblSysTemp;
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey = 0;
		dblSysTemp=0.;
		GroupKey = 0;
	}
	void Convert431(T_STLD_K Key, T_STMP_D_431& rData)
	{
		LoadCaseKey = Key;
		dblSysTemp = rData.dblSysTemp;
		GroupKey = 0;
	}
};
struct T_STMP_UDRD_D
{
	T_STMP_K key;
	T_STMP_D data;
};
struct T_STMP_UNIT
{
	T_STMP_UNIT()
	{
		dblSysTemp=D_UNITSYS_BASE_TEMPER;
	}
	int dblSysTemp;
};
#define HASHSIZESTMP 1

// Nodal Temperature
#define T_NTMP_KEY unsigned int
union T_NTMP_K
{
	T_NTMP_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_NTMP_D_431
{
	T_STLD_K LoadCaseKey;
	double dblNodalTemp;    // T
	void Initialize()
	{
		LoadCaseKey=0;
		dblNodalTemp=0.;
	}
};
struct T_NTMP_UDRD_D_431
{
	T_NTMP_K key;
	T_NTMP_D_431 data;
};
struct T_NTMP_D
{
	T_STLD_K LoadCaseKey;
	double dblNodalTemp;    // T
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		dblNodalTemp=0.;
		GroupKey = 0;
	}
	void Convert431(T_NTMP_D_431& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		dblNodalTemp = rData.dblNodalTemp;
		GroupKey = 0;
	}
};
struct T_NTMP_UDRD_D
{
	T_NTMP_K key;
	T_NTMP_D data;
};
struct T_NTMP_UNIT
{
	T_NTMP_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		dblNodalTemp=D_UNITSYS_BASE_TEMPER;
	}
	int LoadCaseKey;
	int dblNodalTemp;
};
#define HASHSIZENTMP HASHSIZENODE

// Element Temperature
#define T_ETMP_KEY unsigned int
union T_ETMP_K
{
	T_ETMP_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_ETMP_D
{
	T_STLD_K LoadCaseKey;
	double dblElemTemp;    // T
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		dblElemTemp=0.;
		GroupKey = 0;
	}
};
struct T_ETMP_UDRD_D
{
	T_ETMP_K key;
	T_ETMP_D data;
};
struct T_ETMP_UNIT
{
	T_ETMP_UNIT()
	{
		dblElemTemp=D_UNITSYS_BASE_TEMPER;
	}
	int dblElemTemp;
};
#define HASHSIZEETMP 997

// Beam Section Temperature
#define T_BTMP_KEY unsigned int
union T_BTMP_K
{
	T_BTMP_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

//Special Provision of AREMA
#define T_SPVM_KEY unsigned int
union T_SPVM_K
{
	T_SPVM_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

#define D_BTMP_NUMSTMP_842  5  // hslee : 845이전에는 5개 고정 사이즈 였는데 이후에는 가변사이즈로 변경됨
#define D_BTMP_NUMSTMP  20

struct T_BTMP_STMP_692
{
	int    nMaterial; // 1:Element, 2:Input
	double dElast;
	double dThermal;
	double dB;        // PSC 사용안함
	double dH1;
	double dH2;       // PSC 사용안함
	double dT1;
	double dT2;
	void Initialize()
	{
		nMaterial = 0;
		dElast = 0.0;
		dThermal = 0.0;
		dB = 0.0;
		dH1 = 0.0;
		dH2 = 0.0;
		dT1 = 0.0;
		dT2 = 0.0;
	}
};
struct T_BTMP_STMP_730
{
	int    nMaterial; // 1:Element, 2:Input
	double dElast;
	double dThermal;
	double dB;        // PSC 사용안함
	double dH1;
	double dH2;       // PSC 사용안함
	double dT1;
	double dT2;
	BOOL   bAuto;     // 7.0.0추가 PSC만 사용
};
struct T_BTMP_STMP
{
	int    nMaterial; // 0:none, 1:Element, 2:Input
	double dElast;
	double dThermal;
	double dB;
	double dH1;
	double dH2;
	double dT1;
	double dT2;
	BOOL   bAuto; // 700추가 psc사용, 740부터는 사용안함
	int    nRef;  // 740추가, 0:top, 1:bottom
	int    nB;    // 740추가, 0:section, 1:user (dB사용)
	int    nH1;   // 740추가, 0:z1, 1:z2, 2:z3, 3:user (dH1사용)
	int    nH2;   // 740추가, 0:z1, 1:z2, 2:z3, 3:user (dH2사용)
	void Initialize()
	{
		nMaterial = 0;
		dElast = 0.0;
		dThermal = 0.0;
		dB = 0.0;
		dH1 = 0.0;
		dH2 = 0.0;
		dT1 = 0.0;
		dT2 = 0.0;
		bAuto = FALSE;
		nRef = 0;
		nB = 0;
		nH1 = nH2 = 3;
	}
};
struct T_BTMP_D_590 
{
	T_STLD_K  LoadCaseKey;
	int       nDirection; // 0:Local-y, 1:Local-z
	T_BTMP_STMP_692 SectTmp[D_BTMP_NUMSTMP_842];
	T_LDGR_K  GroupKey;  
	void Initialize()
	{
		LoadCaseKey=0;
		nDirection=1;  
		for(int i=0; i<D_BTMP_NUMSTMP_842; ++i)
			SectTmp[i].Initialize();
		GroupKey = 0;
	}
};
struct T_BTMP_D_692
{
	T_STLD_K  LoadCaseKey;
	int       nDirection; // 0:Local-y, 1:Local-z
	int       nRefPosition;  // 6.0.0추가 0:Centroid, 1:+End(Top), 2:-End(Bot)
	T_BTMP_STMP_692 SectTmp[D_BTMP_NUMSTMP_842];
	T_LDGR_K  GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		nDirection=1;
		nRefPosition = 0;
		for(int i=0; i<D_BTMP_NUMSTMP_842; ++i)
			SectTmp[i].Initialize();
		GroupKey = 0;
	}
};
struct T_BTMP_D_730
{
	T_STLD_K  LoadCaseKey;
	int       nDirection; // 0:Local-y, 1:Local-z
	int       nRefPosition;  // 6.0.0추가 0:Centroid, 1:+End(Top), 2:-End(Bot)
	T_BTMP_STMP_730 SectTmp[D_BTMP_NUMSTMP_842];
	T_LDGR_K  GroupKey;
	int       nType;      // 7.0.0추가 0:General, 1:PSC
};
struct T_BTMP_D_842
{
	T_STLD_K  LoadCaseKey;
	int       nDirection; // 0:Local-y, 1:Local-z
	int       nRefPosition;  // 6.0.0추가 0:Centroid, 1:+End(Top), 2:-End(Bot)
	T_BTMP_STMP SectTmp[D_BTMP_NUMSTMP_842];
	T_LDGR_K  GroupKey;
	int       nType;      // 7.0.0추가 0:General, 1:PSC/Composite
};
struct T_BTMP_D // 왜인지 모르겠는데... T_BTMP_D data = rData; 처럼 선언과 동시에 대입을 하면 컴파일 에러가 난다...
{
    enum enType
    {
        General       = 0,
        PSC_Composite = 1,
    };

	T_STLD_K  LoadCaseKey;
	int       nDirection; // 0:Local-y, 1:Local-z
	int       nRefPosition;  // 6.0.0추가 0:Centroid, 1:+End(Top), 2:-End(Bot)  
	CArray<T_BTMP_STMP, T_BTMP_STMP&> SectTmp;  
	T_LDGR_K  GroupKey;
	int       nType;      // 7.0.0추가 0:General, 1:PSC/Composite  
	T_BTMP_D()
	{
		Initialize(); // SetSize를 빼먹지 않게 하기 위해 Initialize를 불러줌
	}  
	void Initialize()
	{
		LoadCaseKey=0;
		nDirection=1;
		nRefPosition = 0;
		SectTmp.SetSize(D_BTMP_NUMSTMP);
		for(int i=0; i<SectTmp.GetSize(); ++i)
		{
			SectTmp[i].Initialize();
		}
		GroupKey = 0;
		nType = General;
	}  
	T_BTMP_D& operator=(T_BTMP_D& src)
	{
		LoadCaseKey = src.LoadCaseKey;
		nDirection = src.nDirection;
		nRefPosition = src.nRefPosition;
		SectTmp.RemoveAll();
		SectTmp.Copy(src.SectTmp);
		GroupKey = src.GroupKey;
		nType = src.nType;

		return *this;
	}
	void Convert590(T_BTMP_D_590& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		nDirection = rData.nDirection;
		nRefPosition = 0;
		for(int i=0; i<D_BTMP_NUMSTMP_842; i++)
		{
			SectTmp[i].nMaterial = rData.SectTmp[i].nMaterial;
			SectTmp[i].dElast    = rData.SectTmp[i].dElast;
			SectTmp[i].dThermal  = rData.SectTmp[i].dThermal;
			SectTmp[i].dB        = rData.SectTmp[i].dB;
			SectTmp[i].dH1       = rData.SectTmp[i].dH1;
			SectTmp[i].dH2       = rData.SectTmp[i].dH2;
			SectTmp[i].dT1       = rData.SectTmp[i].dT1;
			SectTmp[i].dT2       = rData.SectTmp[i].dT2;
			SectTmp[i].bAuto = FALSE;
			SectTmp[i].nRef = 0;
			SectTmp[i].nB = 1;
		  SectTmp[i].nH1 = SectTmp[i].nH2 = 3;
		}
		GroupKey = rData.GroupKey;
		nType = 0;
	}
	void Convert692(T_BTMP_D_692& rData)
	{
		LoadCaseKey  = rData.LoadCaseKey;
		nDirection   = rData.nDirection;
		nRefPosition = rData.nRefPosition;
		for(int i=0; i<D_BTMP_NUMSTMP_842; i++)
		{
			SectTmp[i].nMaterial = rData.SectTmp[i].nMaterial;
			SectTmp[i].dElast    = rData.SectTmp[i].dElast;
			SectTmp[i].dThermal  = rData.SectTmp[i].dThermal;
			SectTmp[i].dB        = rData.SectTmp[i].dB;
			SectTmp[i].dH1       = rData.SectTmp[i].dH1;
			SectTmp[i].dH2       = rData.SectTmp[i].dH2;
			SectTmp[i].dT1       = rData.SectTmp[i].dT1;
			SectTmp[i].dT2       = rData.SectTmp[i].dT2;
			SectTmp[i].bAuto = FALSE;
			SectTmp[i].nRef = 0;
			SectTmp[i].nB = 1;
		  SectTmp[i].nH1 = SectTmp[i].nH2 = 3;
		}
		GroupKey = rData.GroupKey;
		nType = 0;
	}
	void Convert730(T_BTMP_D_730 &rData)
	{
		LoadCaseKey  = rData.LoadCaseKey;
		nDirection   = rData.nDirection;
		nRefPosition = rData.nRefPosition;
		GroupKey = rData.GroupKey;
		nType = rData.nType;
		for(int i=0; i<D_BTMP_NUMSTMP_842; i++)
		{
			SectTmp[i].nMaterial = rData.SectTmp[i].nMaterial;
			SectTmp[i].dElast    = rData.SectTmp[i].dElast;
			SectTmp[i].dThermal  = rData.SectTmp[i].dThermal;
			SectTmp[i].dT1       = rData.SectTmp[i].dT1;
			SectTmp[i].dT2       = rData.SectTmp[i].dT2;
			SectTmp[i].bAuto     = FALSE;

			SectTmp[i].nRef = 0;
			if(nType==0)
			{
				SectTmp[i].nB  = 1; // user
				SectTmp[i].dB  = rData.SectTmp[i].dB;
				SectTmp[i].nH1 = SectTmp[i].nH2 = 3; // user
				SectTmp[i].dH1 = rData.SectTmp[i].dH1;
				SectTmp[i].dH2 = rData.SectTmp[i].dH2;
			}
			else
			{
				SectTmp[i].nB  = 0; // section
				SectTmp[i].dB  = 0.0;
				SectTmp[i].nH1 = 3; // user
				SectTmp[i].dH1 = 0.0;
				if(rData.SectTmp[i].bAuto)
				{
					SectTmp[i].nH2 = 0; // z1
					SectTmp[i].dH2 = 0.0;
				}
				else
				{
					SectTmp[i].nH2 = 3; // user
					SectTmp[i].dH2 = rData.SectTmp[i].dH1;
				}
				if(i>0) SectTmp[i].nMaterial = 0;
			}
		}
	}
	void Convert842(T_BTMP_D_842 &rData)
	{
		LoadCaseKey  = rData.LoadCaseKey;
		nDirection   = rData.nDirection;
		nRefPosition = rData.nRefPosition;
		GroupKey = rData.GroupKey;
		nType = rData.nType;
		SectTmp.SetSize(D_BTMP_NUMSTMP_842);
		for(int i=0; i<D_BTMP_NUMSTMP_842; i++)
		{
			SectTmp[i] = rData.SectTmp[i];
		}
	}
};
struct T_BTMP_UDRD_D_590
{
	T_BTMP_K key;
	T_BTMP_D_590 data;
};
struct T_BTMP_UDRD_D_692
{
	T_BTMP_K key;
	T_BTMP_D_692 data;
};
struct T_BTMP_UDRD_D_730
{
	T_BTMP_K key;
	T_BTMP_D_730 data;
};
struct T_BTMP_UDRD_D_842
{
	T_BTMP_K key;
	T_BTMP_D_842 data;
};
struct T_BTMP_UDRD_D
{
	T_BTMP_K key;
	T_BTMP_D data;
};
struct T_BTMP_UDRD_D_RW
{
	T_BTMP_K key;
	T_STLD_K  LoadCaseKey;
	int       nDirection; // 0:Local-y, 1:Local-z
	int       nRefPosition;  // 6.0.0추가 0:Centroid, 1:+End(Top), 2:-End(Bot)
	T_BTMP_STMP SectTmpNotUse[D_BTMP_NUMSTMP_842]; // 8.4.5부터는 사용 안함.. FileSize를 기존 버전보다 더 크게 하기 위해서 넣어둠
	int       nCountSectTmp;
	T_LDGR_K  GroupKey;
	int       nType;      // 7.0.0추가 0:General, 1:PSC/Composite
	void GetBtmp(T_BTMP_K& rKey, T_BTMP_UDRD_D& rData)
	{
		rKey = key;
		rData.key = key;
		rData.data.LoadCaseKey = LoadCaseKey;
		rData.data.nDirection = nDirection;
		rData.data.nRefPosition = nRefPosition;
		rData.data.SectTmp.SetSize(nCountSectTmp);
		rData.data.GroupKey = GroupKey;
		rData.data.nType = nType;
	}
	void SetBtmp(const T_BTMP_K rKey, const T_BTMP_UDRD_D& rData)
	{
		key = rKey;
		LoadCaseKey = rData.data.LoadCaseKey;
		nDirection = rData.data.nDirection;
		nRefPosition = rData.data.nRefPosition;
		nCountSectTmp = (int)rData.data.SectTmp.GetSize();
		GroupKey = rData.data.GroupKey;
		nType = rData.data.nType;
	}
};

struct T_BTMP_UNIT
{
	T_BTMP_UNIT()
	{
		dElast = D_UNITSYS_BASE_ELAST;
		dThermal = D_UNITSYS_BASE_THERMAL;
		dB=D_UNITSYS_BASE_LENGTH;
		dH=D_UNITSYS_BASE_LENGTH;
		dT=D_UNITSYS_BASE_TEMPER;
	}
	int dElast;
	int dThermal;
	int dB;
	int dH;
	int dT;
};
#define HASHSIZEBTMP 997

// Plate Thickness Temperature
#define T_PTMP_KEY unsigned int
union T_PTMP_K
{
	T_PTMP_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};

#define D_PTMP_NUMSTMP  10

struct T_PTMP_STMP
{
	double dH1;
	double dT1;
	int    nRef;  //-1: None, 0:top, 1:bottom
	void Initialize()
	{
		dH1 = 0.0;
		dT1 = 0.0;
		nRef = -1;
	}
};
struct T_PTMP_D 
{
	T_STLD_K  LoadCaseKey;
	int       nRefPosition;  // 0:+End(Top), 1:-End(Bot)  
	CArray<T_PTMP_STMP, T_PTMP_STMP&> SectTmp;
	T_LDGR_K  GroupKey;

	void Initialize()
	{
		LoadCaseKey  = 0;
		nRefPosition = 0;
		SectTmp.SetSize(D_PTMP_NUMSTMP);
		for (int i = 0; i < SectTmp.GetSize(); ++i)
		{
			SectTmp[i].Initialize();
		}
		GroupKey = 0;
	}

	T_PTMP_D() { Initialize();}
	T_PTMP_D(T_PTMP_D& src){*this = src;}
	T_PTMP_D& operator=(T_PTMP_D& src)
	{
		LoadCaseKey = src.LoadCaseKey;
		nRefPosition = src.nRefPosition;
		SectTmp.RemoveAll();
		SectTmp.Copy(src.SectTmp);
		GroupKey = src.GroupKey;
		return *this;
	}
};
struct T_PTMP_UDRD_D
{
	T_PTMP_K key;
	T_PTMP_D data;
};
struct T_PTMP_UDRD_D_RW
{
	T_PTMP_K key;
	T_STLD_K  LoadCaseKey;
	int       nRefPosition;  // 6.0.0추가 0:+End(Top), 1:-End(Bot)
	T_LDGR_K  GroupKey;
	int       nCountSectTmp;
	void GetPtmp(T_PTMP_K& rKey, T_PTMP_UDRD_D& rData)
	{
		rKey = key;
		rData.key = key;
		rData.data.LoadCaseKey = LoadCaseKey;
		rData.data.nRefPosition = nRefPosition;
		rData.data.SectTmp.SetSize(nCountSectTmp);
		rData.data.GroupKey = GroupKey;
	}
	void SetPtmp(const T_PTMP_K rKey, const T_PTMP_UDRD_D& rData)
	{
		key = rKey;
		LoadCaseKey = rData.data.LoadCaseKey;
		nRefPosition = rData.data.nRefPosition;
		nCountSectTmp = (int)rData.data.SectTmp.GetSize();
		GroupKey = rData.data.GroupKey;
	}
};

struct T_PTMP_UNIT
{
	T_PTMP_UNIT()
	{
		dH = D_UNITSYS_BASE_LENGTH;
		dT = D_UNITSYS_BASE_TEMPER;
	}
	int dH;
	int dT;
};
#define HASHSIZEPTMP 997

			
// Temperature Gradient
#define T_GTMP_KEY unsigned int
union T_GTMP_K
{
	T_GTMP_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_GTMP_D_431
{
	T_STLD_K LoadCaseKey;
	int nType;          // 1=Beam 2=Plate
	double dTz;
	BOOL bUsePropHz;
	double dHz;
	double dTy;         // nType=1, bUsePropHz=FALSE 일 경우에만 사용
	BOOL bUsePropHy;
	double dHy;         // nType=1, bUsePropHy=FALSE 일 경우에만 사용
	void Initialize()
	{
		LoadCaseKey=0;
		nType=0;
		dTz=0.;
		bUsePropHz=TRUE;
		dHz=0.;
		dTy=0.;
		bUsePropHy=TRUE;
		dHy=0.;
	}
};
struct T_GTMP_UDRD_D_431
{
	T_GTMP_K key;
	T_GTMP_D_431 data;
};
struct T_GTMP_D
{
	T_STLD_K LoadCaseKey;
	int nType;          // 1=Beam 2=Plate
	double dTz;
	BOOL bUsePropHz;
	double dHz;
	double dTy;         // nType=1, bUsePropHz=FALSE 일 경우에만 사용
	BOOL bUsePropHy;
	double dHy;         // nType=1, bUsePropHy=FALSE 일 경우에만 사용
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey=0;
		nType=0;
		dTz=0.;
		bUsePropHz=TRUE;
		dHz=0.;
		dTy=0.;
		bUsePropHy=TRUE;
		dHy=0.;
		GroupKey = 0;
	}
	void Convert431(T_GTMP_D_431& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		nType = rData.nType;
		dTz = rData.dTz;
		bUsePropHz = rData.bUsePropHz;
		dHz = rData.dHz;
		dTy = rData.dTy;
		bUsePropHy = rData.bUsePropHy;
		dHy = rData.dHy;
		GroupKey = 0;
	}
};
struct T_GTMP_UDRD_D
{
	T_GTMP_K key;
	T_GTMP_D data;
};
struct T_GTMP_UNIT
{
	T_GTMP_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		nType=D_UNITSYS_NONE;
		dTz=D_UNITSYS_BASE_TEMPER;
		bUsePropHz=D_UNITSYS_NONE;
		dHz=D_UNITSYS_BASE_LENGTH;
		dTy=D_UNITSYS_BASE_TEMPER;
		bUsePropHy=D_UNITSYS_NONE;
		dHy=D_UNITSYS_BASE_LENGTH;
	}
	int LoadCaseKey;
	int nType;
	int dTz;
	int bUsePropHz;
	int dHz;
	int dTy;
	int bUsePropHy;
	int dHy;
};
#define HASHSIZEGTMP 997

// Plane Load Type
#define T_PNLD_K unsigned int
struct T_PNLD_POINT
{
	double x, y;
	double dForce;
	double dMoment;
	void Initialize()
	{
		x = y = dForce = dMoment = 0.0;
	}
};
struct T_PNLD_LINE
{
	BOOL bUniform;
	int  nType;     // 0=Force, 1=Moment
	double x[2];
	double y[2];
	double dForce[2];
	double dMoment[2];
	void Initialize()
	{
		bUniform = TRUE;
		nType = 0;
		for (int i = 0; i < 2; i++)
			x[i] = y[i] = dForce[i] = dMoment[i] = 0.0;
	}
};
struct T_PNLD_AREA
{
	BOOL bUniform;
	BOOL b3Points;
	double x[4];
	double y[4];
	double dLoad[4];
	void Initialize()
	{
		bUniform = TRUE;
		b3Points = FALSE;
		for (int i = 0; i < 4; i++)
			x[i] = y[i] = dLoad[i] = 0.0;
	}
};
struct T_PNLD_D
{
	CString LoadTypeName;
	CString Description;
	int nLoadType;        // 0=Point, 1=Line, 2=Area
	CArray<T_PNLD_POINT, T_PNLD_POINT&> aPointLoad;
	T_PNLD_LINE LineLoad;
	T_PNLD_AREA AreaLoad;
	CArray<double, double> aCopyX;
	CArray<double, double> aCopyY;
	unsigned int nSeq;
	void Initialize()
	{
		LoadTypeName.Empty();
		Description.Empty();
		nLoadType = 0;
		aPointLoad.RemoveAll();
		LineLoad.Initialize();
		AreaLoad.Initialize();
		aCopyX.RemoveAll();
		aCopyY.RemoveAll();
		nSeq=0;
	}
	T_PNLD_D()
	{
	}
	T_PNLD_D(T_PNLD_D& src)
	{
		*this = src;
	}
	T_PNLD_D& operator=(T_PNLD_D& src)
	{
		LoadTypeName = src.LoadTypeName;
		Description = src.Description;
		nLoadType = src.nLoadType;
		aPointLoad.Copy(src.aPointLoad);
		LineLoad = src.LineLoad;
		AreaLoad = src.AreaLoad;
		aCopyX.Copy(src.aCopyX);
		aCopyY.Copy(src.aCopyY);
		nSeq=src.nSeq;
		return *this;
	}
};
struct T_PNLD_D_CH
{
	char LoadTypeName[40];
	char Description[80];
	int nLoadType;        // 0=Point, 1=Line, 2=Area
	CArray<T_PNLD_POINT, T_PNLD_POINT&> aPointLoad;
	T_PNLD_LINE LineLoad;
	T_PNLD_AREA AreaLoad;
	CArray<double, double> aCopyX;
	CArray<double, double> aCopyY;
	unsigned int nSeq;
	void ConvertToString(T_PNLD_D& rData)
	{
		ConvertCharStr(LoadTypeName, rData.LoadTypeName, sizeof(LoadTypeName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nLoadType = nLoadType;
		rData.aPointLoad.Copy(aPointLoad);
		rData.LineLoad = LineLoad;
		rData.AreaLoad = AreaLoad;
		rData.aCopyX.Copy(aCopyX);
		rData.aCopyY.Copy(aCopyY);
		rData.nSeq=nSeq;
	}
	void ConvertToChar(T_PNLD_D& rData)
	{
		ConvertStrChar(rData.LoadTypeName, LoadTypeName, sizeof(LoadTypeName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nLoadType = rData.nLoadType;
		aPointLoad.Copy(rData.aPointLoad);
		LineLoad = rData.LineLoad;
		AreaLoad = rData.AreaLoad;
		aCopyX.Copy(rData.aCopyX);
		aCopyY.Copy(rData.aCopyY);
		nSeq=rData.nSeq;
	}
};
struct T_PNLD_UDRD_D
{
	T_PNLD_K key;
	T_PNLD_D data;
};
struct T_PNLD_UDRD_D_CH
{
	T_PNLD_K key;
	T_PNLD_D_CH data;
};
struct T_PNLD_UDRD_D_CH_RW
{
	T_PNLD_K key;
	char LoadTypeName[40];
	char Description[80];
	int nLoadType;        // 0=Point, 1=Line, 2=Area
	int nCountPointLoad;
	T_PNLD_LINE LineLoad;
	T_PNLD_AREA AreaLoad;
	int nCountCopyX;
	int nCountCopyY;
	unsigned int nSeq;

	void GetPnld(T_PNLD_K& rKey, T_PNLD_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadTypeName, LoadTypeName, sizeof(LoadTypeName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.nLoadType = nLoadType;
		rData.LineLoad = LineLoad;
		rData.AreaLoad = AreaLoad;
		rData.nSeq = nSeq;
	}
	void SetPnld(const T_PNLD_K Key, const T_PNLD_D_CH& rData)
	{
		key=Key;
		memcpy(LoadTypeName, rData.LoadTypeName, sizeof(LoadTypeName));
		memcpy(Description, rData.Description, sizeof(Description));
		nLoadType = rData.nLoadType;
		nCountPointLoad = (int)rData.aPointLoad.GetSize();
		LineLoad = rData.LineLoad;
		AreaLoad = rData.AreaLoad;
		nCountCopyX = (int)rData.aCopyX.GetSize();
		nCountCopyY = (int)rData.aCopyY.GetSize();
		nSeq = rData.nSeq;
	}
};
struct T_PNLD_UNIT
{
	T_PNLD_UNIT()
	{
		x = y = D_UNITSYS_BASE_LENGTH;
		CopyXY = D_UNITSYS_BASE_LENGTH;
		dPointForce = D_UNITSYS_BASE_FORCE;
		dPointMoment = D_UNITSYS_BASE_MOMENT;
		dLineForce = D_UNITSYS_BASE_UNITFORCE;
		dLineMoment = D_UNITSYS_BASE_UNITMOMENT;
		dLoad = D_UNITSYS_BASE_PRESSURE;
	}
	int x, y;
	int dPointForce;
	int dPointMoment;
	int dLineForce;
	int dLineMoment;
	int dLoad;
	int CopyXY;
};
#define HASHSIZEPNLD 31

// assign plane loads
#define T_PNLA_K unsigned int
#define D_PNLA_MAXNOD 15
struct T_PNLA_D_530
{
	T_STLD_K  LoadCaseKey;
	T_PNLD_K  LoadTypeKey;
	int       nElementType; // 0:Plate, 1:Solid
	double    dPoint[3][3]; // Origin, Plane X axis, Plane Y axis
	double    dTolerance;
	int       nLoadDirection;    // 1=Normal, 2=GX, 3=GY, 4=GZ
	int       nProjection;       // 0,1  1=Projection
	BOOL      bLoadingArea;
	T_NODE_K  nConnectivity[D_PNLA_MAXNOD]; // Loading Boundary Node;
	CString   Description;
	T_LDGR_K  GroupKey;
	void Initialize()
	{
		LoadTypeKey = 0;
		nElementType = 1;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				dPoint[i][j] = 0.0;
		dTolerance = 0.0;
		nLoadDirection = 4;
		nProjection = 0;
		bLoadingArea = FALSE;
		memset(nConnectivity, 0, sizeof(nConnectivity));
		Description.Empty();
		GroupKey = 0;
	}
};
struct T_PNLA_D
{
	T_STLD_K  LoadCaseKey;
	T_PNLD_K  LoadTypeKey;
	int       nElementType; // 0:Plate, 1:Solid
	double    dPoint[3][3]; // Origin, Plane X axis, Plane Y axis
	double    dTolerance;
	int       nElemSelection; // 0:Elements on the Loading Plane, 1:Elements in the Group
	T_GRUP_K  ElemGroupKey;
	int       nFaceNo;        // 1-6
	int       nLoadDirection;    // 0=Normal(Loading Plane), 1=Normal(Element), 2=GX, 3=GY, 4=GZ
	int       nProjection;       // 0=No, 1=Load Dirction, 2=Loading Plane
	BOOL      bLoadingArea;
	T_NODE_K  nConnectivity[D_PNLA_MAXNOD]; // Loading Boundary Node;
	CString   Description;
	T_LDGR_K  GroupKey;
	void Initialize()
	{
		LoadTypeKey = 0;
		nElementType = 1;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				dPoint[i][j] = 0.0;
		dTolerance = 0.0;
		nElemSelection = 0;
		ElemGroupKey = 0;
		nFaceNo = 1;
		nLoadDirection = 0;
		nProjection = 0;
		bLoadingArea = FALSE;
		memset(nConnectivity, 0, sizeof(nConnectivity));
		Description.Empty();
		GroupKey = 0;
	}
	void Convert530(T_PNLA_D_530& rData)
	{
		LoadTypeKey = rData.LoadTypeKey;
		nElementType = rData.nElementType;
		memcpy(dPoint, rData.dPoint, sizeof(dPoint));
		dTolerance = rData.dTolerance;
		nElemSelection = 0;
		ElemGroupKey = 0;
		nFaceNo = 1;
		if (rData.nLoadDirection == 1) nLoadDirection = 0; // Normal -> Normal(Loading Plane)
		else nLoadDirection = rData.nLoadDirection;
		if (rData.nProjection) nProjection = 1; // Projection Yes -> Load Direction
		else nProjection = 0;
		bLoadingArea = rData.bLoadingArea;
		memcpy(nConnectivity, rData.nConnectivity, sizeof(nConnectivity));
		Description = rData.Description;
		GroupKey = rData.GroupKey;
	}
};
struct T_PNLA_D_CH_530
{
	T_STLD_K  LoadCaseKey;
	T_PNLD_K  LoadTypeKey;
	int       nElementType; // 0:Plate, 1:Solid
	double    dPoint[3][3]; // Origin, Plane X axis, Plane Y axis
	double    dTolerance;
	int       nLoadDirection;    // 1=Normal, 2=GX, 3=GY, 4=GZ
	int       nProjection;       // 0,1  1=Projection
	BOOL      bLoadingArea;
	T_NODE_K  nConnectivity[D_PNLA_MAXNOD]; // Loading Boundary Node;
	char      Description[80];
	T_LDGR_K  GroupKey;
};
struct T_PNLA_D_CH
{
	T_STLD_K  LoadCaseKey;
	T_PNLD_K  LoadTypeKey;
	int       nElementType; // 0:Plate, 1:Solid
	double    dPoint[3][3]; // Origin, Plane X axis, Plane Y axis
	double    dTolerance;
	int       nElemSelection; // 0:Elements on the Loading Plane, 1:Elements in the Group
	T_GRUP_K  ElemGroupKey;
	int       nFaceNo;        // 1-6
	int       nLoadDirection;    // 0=Normal(Loading Plane), 1=Normal(Element), 2=GX, 3=GY, 4=GZ
	int       nProjection;       // 0=No, 1=Load Dirction, 2=Loading Plane
	BOOL      bLoadingArea;
	T_NODE_K  nConnectivity[D_PNLA_MAXNOD]; // Loading Boundary Node;
	char      Description[80];
	T_LDGR_K  GroupKey;
	void ConvertToString(T_PNLA_D& rData)
	{
		rData.LoadCaseKey = LoadCaseKey;
		rData.LoadTypeKey = LoadTypeKey;
		rData.nElementType = nElementType;
		memcpy(rData.dPoint, dPoint, sizeof(dPoint));
		rData.dTolerance = dTolerance;
		rData.nElemSelection = nElemSelection;
		rData.ElemGroupKey = ElemGroupKey;
		rData.nFaceNo = nFaceNo;
		rData.nLoadDirection = nLoadDirection;
		rData.nProjection = nProjection;
		rData.bLoadingArea = bLoadingArea;
		memcpy(rData.nConnectivity, nConnectivity, sizeof(nConnectivity));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.GroupKey = GroupKey;
	}
	void ConvertToChar(T_PNLA_D& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadTypeKey = rData.LoadTypeKey;
		nElementType = rData.nElementType;
		memcpy(dPoint, rData.dPoint, sizeof(dPoint));
		dTolerance = rData.dTolerance;
		nElemSelection = rData.nElemSelection;
		ElemGroupKey = rData.ElemGroupKey;
		nFaceNo = rData.nFaceNo;
		nLoadDirection = rData.nLoadDirection;
		nProjection = rData.nProjection;
		bLoadingArea = rData.bLoadingArea;
		memcpy(nConnectivity, rData.nConnectivity, sizeof(nConnectivity));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		GroupKey = rData.GroupKey;
	}
	void Convert530(T_PNLA_D_CH_530& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		LoadTypeKey = rData.LoadTypeKey;
		nElementType = rData.nElementType;
		memcpy(dPoint, rData.dPoint, sizeof(dPoint));
		dTolerance = rData.dTolerance;
		nElemSelection = 0;
		ElemGroupKey = 0;
		nFaceNo = 1;
		if (rData.nLoadDirection == 1) nLoadDirection = 0; // Normal -> Normal(Loading Plane)
		else nLoadDirection = rData.nLoadDirection;
		if (rData.nProjection) nProjection = 1; // Projection Yes -> Load Direction
		else nProjection = 0;
		bLoadingArea = rData.bLoadingArea;
		memcpy(nConnectivity, rData.nConnectivity, sizeof(nConnectivity));
		memcpy(Description, rData.Description, sizeof(Description));
		GroupKey = rData.GroupKey;
	}
};
struct T_PNLA_UDRD_D
{
	T_PNLA_K key;
	T_PNLA_D data;
};
struct T_PNLA_UDRD_D_CH_530
{
	T_PNLA_K key;
	T_PNLA_D_CH_530 data;
};
struct T_PNLA_UDRD_D_CH
{
	T_PNLA_K key;
	T_PNLA_D_CH data;
};
struct T_PNLA_UNIT
{
	T_PNLA_UNIT()
	{
		dPoint = D_UNITSYS_BASE_LENGTH;
		dTolerance = D_UNITSYS_BASE_LENGTH;
	}
	int dPoint;
	int dTolerance;
}; 
#define HASHSIZEPNLA HASHSIZEPNLD

//////////////////////////////////////////////////////////////////////////
struct T_PNLOAD_D
{
	T_ELEM_K KeyElem;        // 하중적용요소번호
	T_STLD_K LoadCaseKey;    // 정적하중조건번호
	int nLoadType;           // 1=Point 2=Line 3=Area
	int nFaceNo;             // Face 번호, Plate는 사용안함 (1부터)
	int nLoadDirection;      // 0=Normal(Loading Plane), 1=Normal(Element), 2=GX, 3=GY, 4=GZ
	int nProjection;         // 0=No, 1=LoadDirection 2=LoadingPlane
	BOOL bTriLoad;           // Area Load인 경우 삼각형하중(FALSE면 사각형하중)
	double dPointLoad;
	double dLineLoad[2];
	double dAreaLoad[4];
	double dLoadPoint[4][2]; // 요소 또는 Face의 첫번째점 기준의 위치좌표
	void Initialize()
	{
		KeyElem = 0;
		LoadCaseKey = 0;
		nLoadType = 0;
		nFaceNo = 0;
		nLoadDirection = 0;
		nProjection = 0;
		bTriLoad = FALSE;
		dPointLoad = 0.;
		dLineLoad[0] = dLineLoad[1] = 0.;
		dAreaLoad[0] = dAreaLoad[1] = dAreaLoad[2] = dAreaLoad[3] = 0.;
		for (int i = 0; i < 4; i++)
		{
			dLoadPoint[i][0] = dLoadPoint[i][1] = 0.;
		}
	}

	T_PNLOAD_D() { Initialize(); }
	T_PNLOAD_D(const T_PNLOAD_D& rData) { *this = rData; }
	T_PNLOAD_D& operator=(const T_PNLOAD_D& src)
	{
		KeyElem = src.KeyElem;
		LoadCaseKey = src.LoadCaseKey;
		nLoadType = src.nLoadType;
		nFaceNo = src.nFaceNo;
		nLoadDirection = src.nLoadDirection;
		nProjection = src.nProjection;
		bTriLoad = src.bTriLoad;
		dPointLoad = src.dPointLoad;
		dLineLoad[0] = src.dLineLoad[0];
		dLineLoad[1] = src.dLineLoad[1];
		for (int i = 0; i < 4; i++)
		{
			dAreaLoad[i] = src.dAreaLoad[i];
			dLoadPoint[i][0] = src.dLoadPoint[i][0];
			dLoadPoint[i][1] = src.dLoadPoint[i][1];
		}
		return *this;
	}

};
//////////////////////////////////////////////////////////////////////////

// Finishing Material Loads
#define T_FMLD_KEY unsigned int
union T_FMLD_K
{
	T_FMLD_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;   // 0-4096
	}key;
};
struct T_FMLD_D
{
	T_STLD_K LoadCaseKey;
	int nCoveringType;      // 0:Envelop, 1:Fill, 2:Surround
	int nCoveringRange[4];  // 0:None, 1:Half, 2:Full
	double dThickness;
	double dDensity;
	int    nDirection;      // 1:GX, 2:GY, 3:GZ
	double dScaleFactor;
	T_LDGR_K GroupKey;
	void Initialize()
	{
		LoadCaseKey = 0;
		nCoveringType = 0;
		for (int i = 0; i < 4; i++) nCoveringRange[i] = 0;
		dThickness = 0.0;
		dDensity = 0.0;
		nDirection = 3;
		dScaleFactor = -1.0;
		GroupKey = 0;
	}
	BOOL operator==(const T_FMLD_D& src) const
	{
		if (LoadCaseKey    != src.LoadCaseKey)    return FALSE;
		if (nCoveringType  != src.nCoveringType)  return FALSE;
		for (int k=0; k<4; k++)
		{
			if (nCoveringRange[k] != src.nCoveringRange[k])  return FALSE;
		}
		if (dThickness     != src.dThickness)     return FALSE;
		if (dDensity       != src.dDensity)       return FALSE;
		if (nDirection     != src.nDirection)     return FALSE;
		if (dScaleFactor   != src.dScaleFactor)   return FALSE;
		if (GroupKey       != src.GroupKey)       return FALSE;
		
		return TRUE;
	}
};
struct T_FMLD_UDRD_D
{
	T_FMLD_K key;
	T_FMLD_D data;
};
struct T_FMLD_UNIT
{
	T_FMLD_UNIT()
	{
		dThickness = D_UNITSYS_BASE_LENGTH;
		dDensity = D_UNITSYS_BASE_DENSITY;
	}
	int dThickness;
	int dDensity;
};
#define HASHSIZEFMLD 997

#pragma region Steel Fire Load
//Flue-gas Temperature
struct T_FLUE_BASE
{
	int dblPeriod;
	double dblValue;
	void Initialize()
	{
		dblPeriod=0;
		dblValue=0.;
	}
};
//#define T_FLUE_DT CArray<T_FLUE_BASE, T_FLUE_BASE&>
#define T_FLUE_K unsigned int
struct T_FLUE_D
{
	int nType;//0 Design Code; 1 User
	CString strFlueName;
	CString strCodeName;
	int nSubType;//0 fibre;1 Hydrocarbon
	double dTime;
	double dFactor;
	CArray<T_FLUE_BASE, T_FLUE_BASE&> arFlueFunData;
	void Initialize()
	{
		nType = 0;
		strFlueName = _T("");
		strCodeName = _T("");
		arFlueFunData.RemoveAll();
		nSubType = 0;
		dTime = 0;
		dFactor = 1.0;
	}
	T_FLUE_D(){Initialize();}
	T_FLUE_D(T_FLUE_D& src)
	{
		*this = src;
	}
	T_FLUE_D& operator=(const T_FLUE_D& src)
	{
		if(this == &src)
			return *this;
		nType = src.nType;
		strFlueName = src.strFlueName;
		strCodeName = src.strCodeName;
		arFlueFunData.Copy(src.arFlueFunData);
		nSubType = src.nSubType;
		dTime = src.dTime;
		dFactor = src.dFactor;
		return *this;
	}
};
struct T_FLUE_UDRD_D
{
	T_FLUE_K key;
	T_FLUE_D data;
};
struct T_FLUE_UNIT
{
	T_FLUE_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
struct T_FLUE_D_RW
{
	T_FLUE_K rFlueK;
	int nType;
	char FlueName[28];
	char CodeName[28];
	int nSubType;
	double dTime;
	double dFactor;
	void SetData(T_FLUE_K FlueK_,T_FLUE_D& rData)
	{
		rFlueK = FlueK_;
		nType = rData.nType;
		ConvertStrChar(rData.strFlueName,FlueName,sizeof(FlueName));
		ConvertStrChar(rData.strCodeName,CodeName,sizeof(CodeName));
		nSubType = rData.nSubType;
		dTime = rData.dTime;
		dFactor = rData.dFactor;
	}
	void GetData(T_FLUE_K& FlueK,T_FLUE_D& rData)
	{
		FlueK = rFlueK;
		rData.nType = nType;
		ConvertCharStr(FlueName,rData.strFlueName,sizeof(FlueName));
		ConvertCharStr(CodeName,rData.strCodeName,sizeof(CodeName));
		rData.nSubType = nSubType;
		rData.dTime = dTime;
		rData.dFactor = dFactor;
	}
};
#define HASHSIZEFLUE 997

//Steel Time-Temperatue Fun
#define  T_STTF_K unsigned int
struct T_STTF_D
{
	double dFactor;
	CString strSttfName;
	CArray<T_FLUE_BASE,T_FLUE_BASE&> arTimeTempData;
	void Initialize()
	{
		dFactor = 1.0;
		strSttfName.Empty();
		arTimeTempData.RemoveAll();
	}
	T_STTF_D(){Initialize();}
	T_STTF_D(T_STTF_D& src)
	{
		*this = src;
	}
	T_STTF_D& operator=(const T_STTF_D& src)
	{
		if(this == &src)
			return *this;
		dFactor = src.dFactor;
		arTimeTempData.Copy(src.arTimeTempData);
		strSttfName = src.strSttfName;
		return *this;
	}
};
struct T_STTF_UDRD_D
{
	T_STTF_K key;
	T_STTF_D data;
};
struct T_STTF_D_RW
{
	T_STTF_K rSttfK;
	double dFactor;
	char SttfName[28];
	void SetData(T_STTF_K SttfK,T_STTF_D& rData)
	{
		rSttfK = SttfK;
		dFactor = rData.dFactor;
		ConvertStrChar(rData.strSttfName,SttfName,sizeof(SttfName));
	}
	void GetData(T_STTF_K& SttfK,T_STTF_D& rData)
	{
		SttfK = rSttfK;
		rData.dFactor = dFactor;
		ConvertCharStr(SttfName,rData.strSttfName,sizeof(SttfName));
	}
};
#define HASHSIZESTTF 15

//Fire Paramenters
#define  T_FIPA_K unsigned int
struct T_FIPA_D
{
	enum FireType
	{
		LIGHT_EXPANSION = 0,
		LIGHT_NOEXPANSION,
		NO_LIGHT_FIRECONTAIN,
		NO_FIRECONTAIN
	};
	double		dfireResTime;//fire resistance time
	FireType	firetype;
	double		dThickness;
	double		dRadiance;
	double		dResistance;//The equivalent thermal resistance
	double		dConductivity;//Equivalent heat conductivity
	double		dDensity;
	double		dSHeat;//specific heat
	void Initialize()
	{
		dfireResTime = 0.0;
		firetype = LIGHT_EXPANSION;
		dThickness = 0.0;
		dResistance = 0.0;
		dRadiance	= 0.0;
		dConductivity = 0.0;
		dDensity = 0.0;
		dSHeat = 0.0;
	}
	T_FIPA_D(){Initialize();}
};
struct T_FIPA_UDRD_D
{
	T_FIPA_K key;
	T_FIPA_D data;
};
#define HASHSIZEFIPA 15
//Fire Sect Shape factor
#define T_FSSF_K unsigned int
struct T_FSSF_D
{
	BOOL bAutoCalc;
	int nAutoType;
	double dFactor;
	void Initialize()
	{
		bAutoCalc = TRUE;
		nAutoType = 0;
		dFactor = 0.0;
	}
	T_FSSF_D(){Initialize();}
};
struct T_FSSF_UDRD_D
{
	T_FSSF_K key;
	T_FSSF_D data;
};
#define HASHSIZEFSSF 15
//Fire Temperature
#define FIRE_LOAD_CASE_NAME _LS(IDS_DB_FIRE_LOAD_CASE_)
#pragma endregion 


#define D_EXLD_NUMEXLD_856  15
#define T_EXLD_K unsigned int
struct T_EXLD_D_856
{
	int nNumExldCase;
	T_STLD_K ExldCase[D_EXLD_NUMEXLD_856];
};
struct T_EXLD_D
{
	int nNumExldCase;
	CArray<T_STLD_K, T_STLD_K&> ExldCase;  

	void Initialize()
	{
		nNumExldCase=0;
		ExldCase.RemoveAll();
	}
	T_EXLD_D() {Initialize();}
	T_EXLD_D(T_EXLD_D& rData) { *this = rData; }
	T_EXLD_D& operator=(T_EXLD_D& src)
	{
		nNumExldCase = src.nNumExldCase;
		ExldCase.RemoveAll();
		ExldCase.Copy(src.ExldCase);

		return *this;
	}

	void Convert856(T_EXLD_D_856 &rData)
	{
		nNumExldCase  = rData.nNumExldCase;

		ExldCase.SetSize(nNumExldCase);
		for(int i=0; i<nNumExldCase; i++)
		{
			ExldCase[i] = rData.ExldCase[i];
		}
	}
};

struct T_EXLD_UDRD_D_856
{
	T_EXLD_D_856 data;
};
struct T_EXLD_UDRD_D
{
	T_EXLD_D data;
};
struct T_EXLD_UDRD_D_RW
{
	int nNumExldCase;

	void GetExld(T_EXLD_UDRD_D& rData)
	{
		rData.data.nNumExldCase = nNumExldCase;
		rData.data.ExldCase.SetSize(nNumExldCase);
	}
	void SetExld(T_EXLD_UDRD_D& rData)
	{
		nNumExldCase = rData.data.nNumExldCase;
		// ExldCase 는 Array 형식으로 저장함.
	}
};


struct T_EXLD_UNIT
{
	T_EXLD_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEEXLD 1

#define T_EQMF_K unsigned int

struct T_EQMF_D    // 부재의 평형상태 절점력 및 부재력
{
	int nType;     // 1=Truss, 2=Beam, 3=Elastic Link, 4=General Link
	UINT ElemKey;  
	double dNodeForce[2][6];
	double dElemForce[2][6];  // 6.7.1부터 이 변수는 호환 목적 이외에 사용하지 않음
	void Initialize()
	{
		nType=1;
		ElemKey=0;    
		for(int i=0; i<6; i++)
		{
			dNodeForce[0][i] = dNodeForce[1][i] = 0.;
			dElemForce[0][i] = dElemForce[1][i] = 0.;
		}
	}
	T_EQMF_D() {}
	T_EQMF_D(T_EQMF_D& rData) { *this = rData; }
	T_EQMF_D& operator=(T_EQMF_D& src)
	{ 
		nType   = src.nType;
		ElemKey = src.ElemKey;
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				dNodeForce[i][j] = src.dNodeForce[i][j];
				dElemForce[i][j] = src.dElemForce[i][j];
			}
		}
		return *this;
	}
};
struct T_EQMF_UDRD_D
{
	T_EQMF_K key;
	T_EQMF_D data;
};
struct T_EQMF_UNIT
{
	T_EQMF_UNIT()
	{
		nForce[0] = D_UNITSYS_BASE_FORCE;
		nForce[1] = D_UNITSYS_BASE_FORCE;
		nForce[2] = D_UNITSYS_BASE_FORCE;
		nForce[3] = D_UNITSYS_BASE_MOMENT;
		nForce[4] = D_UNITSYS_BASE_MOMENT;
		nForce[5] = D_UNITSYS_BASE_MOMENT;
	}
	int nForce[6];  
};
#define HASHSIZEEQMF 997

#define T_INMF_K unsigned int

struct T_INMF_D    // 부재의 평형상태 절점력 및 부재력
{
	int nType;     // 1=Truss, 2=Beam, 3=Elastic Link, 4=General Link
	UINT ElemKey;  
	double dElemForce[2][6];
	void Initialize()
	{
		nType=1;
		ElemKey=0;    
		for(int i=0; i<6; i++)
		{
			dElemForce[0][i] = dElemForce[1][i] = 0.;
		}
	}
	T_INMF_D() {}
	T_INMF_D(T_INMF_D& rData) { *this = rData; }
	T_INMF_D& operator=(T_INMF_D& src)
	{ 
		nType   = src.nType;
		ElemKey = src.ElemKey;
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 6; j++)
			{
				dElemForce[i][j] = src.dElemForce[i][j];
			}
		}
		return *this;
	}
	void ConvertFromEqmf(T_EQMF_D& rData)
	{
		nType = rData.nType;     // 1=Truss, 2=Beam, 3=Elastic Link, 4=General Link
		ElemKey = rData.ElemKey;  
		memcpy(dElemForce, rData.dElemForce, sizeof(dElemForce));
	}
};
struct T_INMF_UDRD_D
{
	T_INMF_K key;
	T_INMF_D data;
};
struct T_INMF_UNIT
{
	T_INMF_UNIT()
	{
		nForce[0] = D_UNITSYS_BASE_FORCE;
		nForce[1] = D_UNITSYS_BASE_FORCE;
		nForce[2] = D_UNITSYS_BASE_FORCE;
		nForce[3] = D_UNITSYS_BASE_MOMENT;
		nForce[4] = D_UNITSYS_BASE_MOMENT;
		nForce[5] = D_UNITSYS_BASE_MOMENT;
	}
	int nForce[6];  
};
#define HASHSIZEINMF 997


//////////////////////////////////////////////////
// 여기부터 작업
// Nodal Body Force 
#define T_NBOF_K unsigned int
struct T_NBOF_D_660
{
	T_STLD_K LoadCaseKey;
	T_LDGR_K LdgrKey;  
	BOOL    bUseGrup;
	T_GRUP_K GrupKey;
	CArray<T_NODE_K, T_NODE_K> arKeyNode;    
	BOOL    bNodalMass;
	BOOL    bLoadToMass;
	BOOL    bStructMass;
	double  dFactorX;
	double  dFactorY;
	double  dFactorZ;
};
struct T_NBOF_D
{
	T_STLD_K LoadCaseKey;
	T_LDGR_K LdgrKey;  
	BOOL    bUseGrup;
	T_GRUP_K GrupKey;
	CArray<T_NODE_K, T_NODE_K> arKeyNode;    
	BOOL    bNodalMass;
	BOOL    bLoadToMass;
	BOOL    bStructMass;
	BOOL    bGridMass;   // 6.7.0 추가
	double  dFactorX;
	double  dFactorY;
	double  dFactorZ;
	void Initialize()
	{
		LoadCaseKey = 0;
		LdgrKey = 0;
		bUseGrup = FALSE;
		GrupKey = 0;
		arKeyNode.RemoveAll();    
		bNodalMass  = TRUE;
		bLoadToMass = TRUE;
		bStructMass = TRUE;
		bGridMass   = TRUE;
		dFactorX = 0.;
		dFactorY = 0.;
		dFactorZ = 0.;
	}
	T_NBOF_D() { }
	T_NBOF_D(T_NBOF_D& src)
	{
		*this = src;
	}
	T_NBOF_D& operator=(T_NBOF_D& src)
	{    
		LoadCaseKey = src.LoadCaseKey;
		LdgrKey     = src.LdgrKey;
		bUseGrup    = src.bUseGrup;
		GrupKey     = src.GrupKey;
		arKeyNode.Copy(src.arKeyNode);    
		bNodalMass  = src.bNodalMass;
		bLoadToMass = src.bLoadToMass;
		bStructMass = src.bStructMass;
		bGridMass   = src.bGridMass;
		dFactorX    = src.dFactorX;
		dFactorY    = src.dFactorY;
		dFactorZ    = src.dFactorZ;
		return *this;
	}
};
struct T_NBOF_UDRD_D_660
{
	T_NBOF_K key;
	T_NBOF_D_660 data;
};
struct T_NBOF_UDRD_D
{
	T_NBOF_K key;
	T_NBOF_D data;
};
struct T_NBOF_ITEM_UDRD_D
{
	T_NBOF_K key;
	T_NODE_K nodekey;
};
struct T_NBOF_UDRD_D_RW_660
{  
	T_NBOF_K key;
	T_STLD_K LoadCaseKey;
	T_LDGR_K LdgrKey;  
	BOOL    bUseGrup;
	T_GRUP_K GrupKey;
	int     nCountKeyNode;    
	BOOL    bNodalMass;
	BOOL    bLoadToMass;
	BOOL    bStructMass;  
	double  dFactorX;
	double  dFactorY;
	double  dFactorZ;
};
struct T_NBOF_UDRD_D_RW
{  
	T_NBOF_K key;
	T_STLD_K LoadCaseKey;
	T_LDGR_K LdgrKey;  
	BOOL    bUseGrup;
	T_GRUP_K GrupKey;
	int     nCountKeyNode;    
	BOOL    bNodalMass;
	BOOL    bLoadToMass;
	BOOL    bStructMass;
	BOOL    bGridMass;   // 6.7.0 추가
	double  dFactorX;
	double  dFactorY;
	double  dFactorZ;
	void GetNbof(T_NBOF_K& rKey, T_NBOF_D& rData)
	{
		rKey              = key;
		rData.LoadCaseKey = LoadCaseKey;
		rData.LdgrKey     = LdgrKey;
		rData.bUseGrup    = bUseGrup;
		rData.GrupKey     = GrupKey;    
		rData.bNodalMass  = bNodalMass;
		rData.bLoadToMass = bLoadToMass;
		rData.bStructMass = bStructMass;
		rData.bGridMass   = bGridMass;
		rData.dFactorX    = dFactorX;
		rData.dFactorY    = dFactorY;
		rData.dFactorZ    = dFactorZ;
	}
	void SetNbof(const T_NBOF_K Key, const T_NBOF_D& rData)
	{
		key         = Key;
		LoadCaseKey = rData.LoadCaseKey;
		LdgrKey     = rData.LdgrKey;
		bUseGrup    = rData.bUseGrup;
		GrupKey     = rData.GrupKey;    
		nCountKeyNode = (int)rData.arKeyNode.GetSize();
		bNodalMass  = rData.bNodalMass;
		bLoadToMass = rData.bLoadToMass;
		bStructMass = rData.bStructMass;
		bGridMass   = rData.bGridMass;
		dFactorX    = rData.dFactorX;
		dFactorY    = rData.dFactorY;
		dFactorZ    = rData.dFactorZ;
	}
	void Convert660(T_NBOF_UDRD_D_RW_660& rData)
	{
		key         = rData.key;
		LoadCaseKey = rData.LoadCaseKey;
		LdgrKey     = rData.LdgrKey;
		bUseGrup    = rData.bUseGrup;
		GrupKey     = rData.GrupKey;    
		nCountKeyNode = rData.nCountKeyNode;
		bNodalMass  = rData.bNodalMass;
		bLoadToMass = rData.bLoadToMass;
		bStructMass = rData.bStructMass;
		bGridMass   = FALSE;
		dFactorX    = rData.dFactorX;
		dFactorY    = rData.dFactorY;
		dFactorZ    = rData.dFactorZ;
	}  
};
struct T_NBOF_UNIT
{
	T_NBOF_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZENBOF 1

// Crane Load Type
#define T_CRAN_K unsigned int
struct T_CRAN_D
{
	CString LoadTypeName;
	CArray<UINT, UINT> arNode;
	CArray<UINT, UINT> arNode2;
	double dMaxAxleLoad;
	double dMinAxleLoad;
	double dLongitudinal;
	double dTransverse;
	double dLeftEcc;
	double dRightEcc;
	double dVerticalEcc;
	double dWidth;
	double dTolerance;
	unsigned int nSeq;

	void Initialize()
	{
		LoadTypeName.Empty();
		arNode.RemoveAll();
		arNode2.RemoveAll();
		dMaxAxleLoad = 0.;
		dMinAxleLoad = 0.;
		dLongitudinal = 0.;
		dTransverse = 0.;
		dLeftEcc = 0.;
		dRightEcc = 0.;
		dVerticalEcc = 0.;
		dWidth = 0.;
		dTolerance = 0.;
		nSeq = 0;
	}
	T_CRAN_D()
	{
	}
	T_CRAN_D(T_CRAN_D& src)
	{
		*this = src;
	}
	T_CRAN_D& operator=(T_CRAN_D& src)
	{
		LoadTypeName = src.LoadTypeName;
		arNode.Copy(src.arNode);
		arNode2.Copy(src.arNode2);
		dMaxAxleLoad  = src.dMaxAxleLoad;
		dMinAxleLoad  = src.dMinAxleLoad;
		dLongitudinal = src.dLongitudinal;
		dTransverse   = src.dTransverse;
		dLeftEcc      = src.dLeftEcc;
		dRightEcc     = src.dRightEcc;
		dVerticalEcc  = src.dVerticalEcc;
		dWidth        = src.dWidth;
		dTolerance    = src.dTolerance;
		nSeq          = src.nSeq;

		return *this;
	}
};
struct T_CRAN_D_CH
{
	char LoadTypeName[40];
	CArray<UINT, UINT> arNode;
	CArray<UINT, UINT> arNode2;
	double dMaxAxleLoad;
	double dMinAxleLoad;
	double dLongitudinal;
	double dTransverse;
	double dLeftEcc;
	double dRightEcc;
	double dVerticalEcc;
	double dWidth;
	double dTolerance;
	unsigned int nSeq;

	void ConvertToString(T_CRAN_D& rData)
	{
		ConvertCharStr(LoadTypeName, rData.LoadTypeName, sizeof(LoadTypeName));
		rData.arNode.Copy(arNode);
		rData.arNode2.Copy(arNode2);
		rData.dMaxAxleLoad  = dMaxAxleLoad;
		rData.dMinAxleLoad  = dMinAxleLoad;
		rData.dLongitudinal = dLongitudinal;
		rData.dTransverse   = dTransverse;
		rData.dLeftEcc      = dLeftEcc;
		rData.dRightEcc     = dRightEcc;
		rData.dVerticalEcc  = dVerticalEcc;
		rData.dWidth        = dWidth;
		rData.dTolerance    = dTolerance;
		rData.nSeq          = nSeq;
	}
	void ConvertToChar(T_CRAN_D& rData)
	{
		ConvertStrChar(rData.LoadTypeName, LoadTypeName, sizeof(LoadTypeName));
		arNode.Copy(rData.arNode);
		arNode2.Copy(rData.arNode2);
		dMaxAxleLoad  = rData.dMaxAxleLoad;
		dMinAxleLoad  = rData.dMinAxleLoad;
		dLongitudinal = rData.dLongitudinal;
		dTransverse   = rData.dTransverse;
		dLeftEcc      = rData.dLeftEcc;
		dRightEcc     = rData.dRightEcc;
		dVerticalEcc  = rData.dVerticalEcc;
		dWidth        = rData.dWidth;
		dTolerance    = rData.dTolerance;
		nSeq          = rData.nSeq;
	}
};
struct T_CRAN_UDRD_D
{
	T_CRAN_K key;
	T_CRAN_D data;
};
struct T_CRAN_UDRD_D_CH
{
	T_CRAN_K key;
	T_CRAN_D_CH data;
};
struct T_CRAN_UDRD_D_CH_RW
{
	T_CRAN_K key;
	char LoadTypeName[40];
	int nCountNode;
	int nCountNode2;
	double dMaxAxleLoad;
	double dMinAxleLoad;
	double dLongitudinal;
	double dTransverse;
	double dLeftEcc;
	double dRightEcc;
	double dVerticalEcc;
	double dWidth;
	double dTolerance;
	unsigned int nSeq;

	void GetCran(T_CRAN_K& rKey, T_CRAN_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadTypeName, LoadTypeName, sizeof(LoadTypeName));
		rData.dMaxAxleLoad  = dMaxAxleLoad;
		rData.dMinAxleLoad  = dMinAxleLoad;
		rData.dLongitudinal = dLongitudinal;
		rData.dTransverse   = dTransverse;
		rData.dLeftEcc      = dLeftEcc;
		rData.dRightEcc     = dRightEcc;
		rData.dVerticalEcc  = dVerticalEcc;
		rData.dWidth        = dWidth;
		rData.dTolerance    = dTolerance;
		rData.nSeq          = nSeq;
	}
	void SetCran(const T_CRAN_K Key, const T_CRAN_D_CH& rData)
	{
		key=Key;
		memcpy(LoadTypeName, rData.LoadTypeName, sizeof(LoadTypeName));
		nCountNode    = (int)rData.arNode.GetSize();
		nCountNode2   = (int)rData.arNode2.GetSize();
		dMaxAxleLoad  = rData.dMaxAxleLoad;
		dMinAxleLoad  = rData.dMinAxleLoad;
		dLongitudinal = rData.dLongitudinal;
		dTransverse   = rData.dTransverse;
		dLeftEcc      = rData.dLeftEcc;
		dRightEcc     = rData.dRightEcc;
		dVerticalEcc  = rData.dVerticalEcc;
		dWidth        = rData.dWidth;
		dTolerance    = rData.dTolerance;
		nSeq          = rData.nSeq;
	}
};
struct T_CRAN_UNIT
{
	T_CRAN_UNIT()
	{
		dMaxAxleLoad  = D_UNITSYS_BASE_FORCE;
		dMinAxleLoad  = D_UNITSYS_BASE_FORCE;
		dLongitudinal = D_UNITSYS_BASE_FORCE;
		dTransverse   = D_UNITSYS_BASE_FORCE;
		dLeftEcc      = D_UNITSYS_BASE_LENGTH;
		dRightEcc     = D_UNITSYS_BASE_LENGTH;
		dVerticalEcc  = D_UNITSYS_BASE_LENGTH;
		dWidth        = D_UNITSYS_BASE_LENGTH;
		dTolerance    = D_UNITSYS_BASE_LENGTH;
	}
	int dMaxAxleLoad;
	int dMinAxleLoad;
	int dLongitudinal;
	int dTransverse;
	int dLeftEcc;
	int dRightEcc;
	int dVerticalEcc;
	int dWidth;
	int dTolerance;
};
#define HASHSIZECRAN 5

// Crane Load Number
#define T_NCRN_K unsigned int
struct T_NCRN_D
{
	int nNum;
	void Initialize()
	{
		nNum=1;
	}
};
struct T_NCRN_UDRD_D
{
	T_NCRN_D data;
};
struct T_NCRN_UNIT
{
	T_NCRN_UNIT()
	{
		nNum=D_UNITSYS_NONE;
	}
	int nNum;
};
#define HASHSIZENCRN 1

struct T_LPSI_D
{
	int nIndex; // 그리드에 출력시 정렬용으로만 사용. (1부터 시작)
	T_STLD_K StldKey;
	CString LoadCaseName;
	CString LoadCaseType;
	double dPsi0;
	double dPsi1;
	double dPsi2;

	void Initialize()
	{
		nIndex = 0;
		StldKey = NULL;
		LoadCaseName.Empty();
		LoadCaseType.Empty();
		dPsi0=0.;
		dPsi1=0.;
		dPsi2=0.;
	}
};

// Pressure Load Type
#define T_PSLT_K unsigned int
enum {PSLT_NUM=8};
struct T_PSLT_D
{
	CString  strName;
	CString  strDesc;
	int      nElemType; // 0:Plate/Plane Stress(Face), 1:Plate/Plane Stress(Edge), 2:Solid(Face), 3:Plane Strain(Edge), 4:Axisymmetric(Edge), 5:Wall(Edge)
	unsigned int PsltId; // 순번
	T_STLD_K LoadCaseKey[PSLT_NUM];
	int      nLoadType  [PSLT_NUM];
	double   dLoadP1    [PSLT_NUM];
	double   dLoadP2    [PSLT_NUM];
	double   dLoadP3    [PSLT_NUM];
	double   dLoadP4    [PSLT_NUM];
	COLORREF color;
	T_PSLT_D() { Initialize(); }
	void Initialize()
	{
		strName = _T("");
		strDesc = _T("");
		nElemType = 0;
		PsltId = 0;
		color = RGB(255,0,0);
		memset(LoadCaseKey, 0, sizeof(T_STLD_K)*PSLT_NUM);
		memset(nLoadType  , 0, sizeof(int     )*PSLT_NUM);
		memset(dLoadP1    , 0, sizeof(double  )*PSLT_NUM);
		memset(dLoadP2    , 0, sizeof(double  )*PSLT_NUM);
		memset(dLoadP3    , 0, sizeof(double  )*PSLT_NUM);
		memset(dLoadP4    , 0, sizeof(double  )*PSLT_NUM);
	}
};
struct T_PSLT_D_CH
{
	char    strName[40];
	char    strDesc[80];
	int      nElemType; // 0:Plate/Plane Stress(Face), 1:Plate/Plane Stress(Edge), 2:Solid(Face), 3:Plane Strain(Edge), 4:Axisymmetric(Edge), 5:Wall(Edge)
	unsigned int PsltId; // 순번
	T_STLD_K LoadCaseKey[PSLT_NUM];
	int      nLoadType  [PSLT_NUM];
	double   dLoadP1    [PSLT_NUM];
	double   dLoadP2    [PSLT_NUM];
	double   dLoadP3    [PSLT_NUM];
	double   dLoadP4    [PSLT_NUM];
	COLORREF color;
	void ConvertToString(T_PSLT_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nElemType = nElemType;
		rData.PsltId = PsltId;
		rData.color = color;
		for(int i=0; i<PSLT_NUM; i++)
		{
			rData.LoadCaseKey[i] = LoadCaseKey[i];
			rData.nLoadType  [i] = nLoadType  [i];
			rData.dLoadP1    [i] = dLoadP1    [i];
			rData.dLoadP2    [i] = dLoadP2    [i];
			rData.dLoadP3    [i] = dLoadP3    [i];
			rData.dLoadP4    [i] = dLoadP4    [i];
		}
	}
	void ConvertToChar(T_PSLT_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nElemType = rData.nElemType;
		PsltId    = rData.PsltId;
		color	= rData.color;
		for(int i=0; i<PSLT_NUM; i++)
		{
			LoadCaseKey[i] = rData.LoadCaseKey[i];
			nLoadType  [i] = rData.nLoadType  [i];
			dLoadP1    [i] = rData.dLoadP1    [i];
			dLoadP2    [i] = rData.dLoadP2    [i];
			dLoadP3    [i] = rData.dLoadP3    [i];
			dLoadP4    [i] = rData.dLoadP4    [i];
		}
	}
};
struct T_PSLT_UDRD_D
{
	T_PSLT_K key;
	T_PSLT_D data;
};
struct T_PSLT_UDRD_D_CH
{
	T_PSLT_K key;
	T_PSLT_D_CH data;
};
struct T_PSLT_UDRD_D_CH_RW_895
{
	T_PSLT_K key;
	char    strName[40];
	char    strDesc[80];
	int      nElemType; // 0:Plate/Plane Stress(Face), 1:Plate/Plane Stress(Edge), 2:Solid(Face), 3:Plane Strain(Edge), 4:Axisymmetric(Edge), 5:Wall(Edge)
	unsigned int PsltId; // 순번
	T_STLD_K LoadCaseKey[PSLT_NUM];
	int      nLoadType  [PSLT_NUM];
	double   dLoadP1    [PSLT_NUM];
	double   dLoadP2    [PSLT_NUM];
	double   dLoadP3    [PSLT_NUM];
	double   dLoadP4    [PSLT_NUM];
	void GetPslt(T_PSLT_K& rKey, T_PSLT_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDesc, strDesc, sizeof(strDesc));
		rData.nElemType = nElemType;
		rData.PsltId    = PsltId;
		for(int i=0; i<PSLT_NUM; i++)
		{
			rData.LoadCaseKey[i] = LoadCaseKey[i];
			rData.nLoadType  [i] = nLoadType  [i];
			rData.dLoadP1    [i] = dLoadP1    [i];
			rData.dLoadP2    [i] = dLoadP2    [i];
			rData.dLoadP3    [i] = dLoadP3    [i];
			rData.dLoadP4    [i] = dLoadP4    [i];
		}
	}
	void SetPslt(const T_PSLT_K Key, const T_PSLT_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDesc, rData.strDesc, sizeof(strDesc));
		nElemType = rData.nElemType;
		PsltId    = rData.PsltId;
		for(int i=0; i<PSLT_NUM; i++)
		{
			LoadCaseKey[i] = rData.LoadCaseKey[i];
			nLoadType  [i] = rData.nLoadType  [i];
			dLoadP1    [i] = rData.dLoadP1    [i];
			dLoadP2    [i] = rData.dLoadP2    [i];
			dLoadP3    [i] = rData.dLoadP3    [i];
			dLoadP4    [i] = rData.dLoadP4    [i];
		}
	}
};
struct T_PSLT_UDRD_D_CH_RW
{
	T_PSLT_K key;
	char    strName[40];
	char    strDesc[80];
	int      nElemType; // 0:Plate/Plane Stress(Face), 1:Plate/Plane Stress(Edge), 2:Solid(Face), 3:Plane Strain(Edge), 4:Axisymmetric(Edge), 5:Wall(Edge)
	unsigned int PsltId; // 순번
	T_STLD_K LoadCaseKey[PSLT_NUM];
	int      nLoadType  [PSLT_NUM];
	double   dLoadP1    [PSLT_NUM];
	double   dLoadP2    [PSLT_NUM];
	double   dLoadP3    [PSLT_NUM];
	double   dLoadP4    [PSLT_NUM];
	COLORREF color;
	void GetPslt(T_PSLT_K& rKey, T_PSLT_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDesc, strDesc, sizeof(strDesc));
		rData.nElemType = nElemType;
		rData.PsltId    = PsltId;
		rData.color		= color;
		for(int i=0; i<PSLT_NUM; i++)
		{
			rData.LoadCaseKey[i] = LoadCaseKey[i];
			rData.nLoadType  [i] = nLoadType  [i];
			rData.dLoadP1    [i] = dLoadP1    [i];
			rData.dLoadP2    [i] = dLoadP2    [i];
			rData.dLoadP3    [i] = dLoadP3    [i];
			rData.dLoadP4    [i] = dLoadP4    [i];
		}
	}
	void SetPslt(const T_PSLT_K Key, const T_PSLT_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDesc, rData.strDesc, sizeof(strDesc));
		nElemType = rData.nElemType;
		PsltId    = rData.PsltId;
		color		= rData.color;
		for(int i=0; i<PSLT_NUM; i++)
		{
			LoadCaseKey[i] = rData.LoadCaseKey[i];
			nLoadType  [i] = rData.nLoadType  [i];
			dLoadP1    [i] = rData.dLoadP1    [i];
			dLoadP2    [i] = rData.dLoadP2    [i];
			dLoadP3    [i] = rData.dLoadP3    [i];
			dLoadP4    [i] = rData.dLoadP4    [i];
		}
	}
	void Convert895(T_PSLT_UDRD_D_CH_RW_895& rData)
	{
		key=rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDesc, rData.strDesc, sizeof(strDesc));
		nElemType = rData.nElemType;
		PsltId    = rData.PsltId;
		color		= RGB(255,0,0);
		for(int i=0; i<PSLT_NUM; i++)
		{
			LoadCaseKey[i] = rData.LoadCaseKey[i];
			nLoadType  [i] = rData.nLoadType  [i];
			dLoadP1    [i] = rData.dLoadP1    [i];
			dLoadP2    [i] = rData.dLoadP2    [i];
			dLoadP3    [i] = rData.dLoadP3    [i];
			dLoadP4    [i] = rData.dLoadP4    [i];
		}
	}
};

struct T_PSLT_UNIT
{
	int Pressure;
	int UnitLoad;
	T_PSLT_UNIT()
	{
		Pressure = D_UNITSYS_BASE_PRESSURE;
		UnitLoad = D_UNITSYS_BASE_UNITLOAD;
	}
};
#define HASHSIZEPSLT 997


#define T_IPDT_K T_STOR_K

struct T_IPDT_D
{
	double dHeight;
	double dColNumX; // 기둥 갯수 실수로도 입력 가능
	double dColNumY; // 기둥 갯수 실수로도 입력 가능
	BOOL   bUserCoeff;
	double dUserCoefX;
	double dUserCoefY;

	void Initialize()
	{
		dHeight = 0.0;
		dColNumX = 0;
		dColNumY = 0;
		bUserCoeff = FALSE;
		dUserCoefX = 0.0;
		dUserCoefY = 0.0;
	}
	T_IPDT_D()
	{
	}
	T_IPDT_D(T_IPDT_D& src)
	{
		*this = src;
	}
};
struct T_IPDT_D_RW
{
	T_IPDT_K key;
	double dHeight;
	double dColNumX; // 기둥 갯수 실수로도 입력 가능
	double dColNumY; // 기둥 갯수 실수로도 입력 가능
	BOOL   bUserCoeff;
	double dUserCoefX;
	double dUserCoefY;

	void GetIpdt(T_IPDT_K& rKey, T_IPDT_D& rData)
	{    
		rKey             = key;
		rData.dHeight    = dHeight;
		rData.dColNumX   = dColNumX;
		rData.dColNumY   = dColNumY;
		rData.bUserCoeff = bUserCoeff;
		rData.dUserCoefX = dUserCoefX;
		rData.dUserCoefY = dUserCoefY;
	}
	void SetIpdt(const T_IPDT_K Key, const T_IPDT_D& rData)
	{    
		key        = Key;
		dHeight    = rData.dHeight;    
		dColNumX   = rData.dColNumX;    
		dColNumY   = rData.dColNumY;    
		bUserCoeff = rData.bUserCoeff; 
		dUserCoefX = rData.dUserCoefX; 
		dUserCoefY = rData.dUserCoefY;
	}
};
struct T_IPDT_UDRD_D
{
	T_IPDT_K key;
	T_IPDT_D data;
};
struct T_IPDT_UNIT
{
	T_IPDT_UNIT()
	{    
		dHeight = D_UNITSYS_BASE_LENGTH;    
	}
	int dHeight;    
};
#define HASHSIZEIPDT HASHSIZESTOR


#define T_IPCR_K unsigned int

struct T_IPCR_CH_GB
{
		BOOL m_bElem;
		BOOL m_bAutoCalK;
		double m_dK;
		BOOL m_bAutoE0L;
		double m_dE0L;
		void Initial()
		{
				m_bAutoCalK = TRUE;
				m_dK = 0.004;
				m_bAutoE0L = TRUE;
				m_dE0L = 0.004;
				m_bElem = FALSE;
		}
};

struct T_IPCR_D
{
	CString strName;
	CString strImpLCaseName; // Imperfection load Case 이름
	CString strDesc;
	UINT LoadCaseKey;
	UINT AnalType; // D_LOADCASE_???
	int nDir; //default 0:Global-X 1:Global-Y                  
	int nMinMax; // 0:All, -1:Min, 1:Max
	// 0 default 1 china
	int m_nCode;
	T_IPCR_CH_GB m_CHData;

	void Initialize()
	{
		strName = "";
		strImpLCaseName = "";
		strDesc = "";
		LoadCaseKey = NULL;
		AnalType = 0;
		nDir = 0;
		nMinMax = 0;
		m_nCode = 0;
		m_CHData.Initial();
	}
	T_IPCR_D()
	{
	}
	T_IPCR_D(T_IPCR_D& src)
	{
		*this = src;
	}
	T_IPCR_D& operator=(T_IPCR_D& src)
	{
		strName = src.strName;
		strImpLCaseName = src.strImpLCaseName;
		strDesc = src.strDesc;
		LoadCaseKey = src.LoadCaseKey;
		AnalType = src.AnalType;    
		nDir = src.nDir;
		nMinMax = src.nMinMax;    
		m_nCode = src.m_nCode;
		m_CHData.m_bAutoCalK = src.m_CHData.m_bAutoCalK;
		m_CHData.m_bAutoE0L  = src.m_CHData.m_bAutoE0L;
		m_CHData.m_dK        = src.m_CHData.m_dK;
		m_CHData.m_dE0L      = src.m_CHData.m_dE0L;
		m_CHData.m_bElem     = src.m_CHData.m_bElem;
		return *this;
	}
};
struct T_IPCR_D_CH
{
	char strName[20];
	char strImpLCaseName[80];  
	char strDesc[80];
	UINT LoadCaseKey;
	UINT AnalType; // D_LOADCASE_???
	int nDir; // 0:Global-X 1:Global-Y
	int nMinMax; // 0:All, -1:Min, 1:Max

	int m_nCode;
	T_IPCR_CH_GB m_CHData;

	void ConvertToString(T_IPCR_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strImpLCaseName, rData.strImpLCaseName, sizeof(strImpLCaseName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.LoadCaseKey = LoadCaseKey;
		rData.AnalType = AnalType;    
		rData.nDir = nDir;
		rData.nMinMax = nMinMax;    
		rData.m_nCode = m_nCode;
		rData.m_CHData.m_bAutoCalK = m_CHData.m_bAutoCalK;
		rData.m_CHData.m_bAutoE0L  = m_CHData.m_bAutoE0L;
		rData.m_CHData.m_dK        = m_CHData.m_dK;
		rData.m_CHData.m_dE0L      = m_CHData.m_dE0L;
		rData.m_CHData.m_bElem      = m_CHData.m_bElem;
	}
	void ConvertToChar(T_IPCR_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strImpLCaseName, strImpLCaseName, sizeof(strImpLCaseName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		LoadCaseKey = rData.LoadCaseKey;
		AnalType = rData.AnalType;    
		nDir= rData.nDir;
		nMinMax= rData.nMinMax;

		m_nCode = rData.m_nCode;
		m_CHData.m_bAutoCalK = rData.m_CHData.m_bAutoCalK;
		m_CHData.m_bAutoE0L  = rData.m_CHData.m_bAutoE0L;
		m_CHData.m_dK        = rData.m_CHData.m_dK;
		m_CHData.m_dE0L      = rData.m_CHData.m_dE0L;
		m_CHData.m_bElem     = rData.m_CHData.m_bElem;
	}
};
struct T_IPCR_UDRD_D
{
	T_IPCR_K key;
	T_IPCR_D data;
};
struct T_IPCR_UDRD_D_CH
{
	T_IPCR_K key;
	T_IPCR_D_CH data;
};
struct T_IPCR_UNIT
{
	T_IPCR_UNIT()
	{    
		// Nothing 
	}  
};
//////////////////////////////////////////////////////////////////////////
struct T_IPCR_D_CH_865
{
		char strName[20];
		char strImpLCaseName[80];  
		char strDesc[80];
		UINT LoadCaseKey;
		UINT AnalType; // D_LOADCASE_???
		int nDir; // 0:Global-X 1:Global-Y
		int nMinMax; // 0:All, -1:Min, 1:Max

		void ConvertToString(T_IPCR_D& rData)
		{
				rData.Initialize();
				ConvertCharStr(strName, rData.strName, sizeof(strName));
				ConvertCharStr(strImpLCaseName, rData.strImpLCaseName, sizeof(strImpLCaseName));
				ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
				rData.LoadCaseKey = LoadCaseKey;
				rData.AnalType = AnalType;    
				rData.nDir = nDir;
				rData.nMinMax = nMinMax;    
		}
};

struct T_IPCR_UDRD_D_CH_865
{
		T_IPCR_K key;
		T_IPCR_D_CH_865 data;
};
#define HASHSIZEIPCR HASHSIZESTOR

// PMS 5859 : Area Pressure Loads
enum D_ARPR_DIR_NORMAL { D_ARPR_DIR_NORMAL, D_ARPR_DIR_GX, D_ARPR_DIR_GY, D_ARPR_DIR_GZ, D_ARPR_DIR_VECTOR };
enum D_ARPR_SEL { D_ARPR_SEL_GROUP, D_ARPR_SEL_ELEMENT };
#define T_ARPR_K unsigned int
struct T_ARPR_D
{
	T_STLD_K	StldK		;	
	T_LDGR_K	LdgrK		;	
	int			nDirection	;	// D_ARPR_DIR
	double		dInnerPnt[3];	// 공간구조의 경우 Nomal을 구하기위한 좌표
	double		dVector[3]	;	// nDirection == D_ARPR_DIR_VECTOR일 때
	BOOL		bProjection	;	// nDirection == D_ARPR_DIR_GX,Y,Z일 때
	double		dP1			;
	int			nSelection	;	// D_ARPR_SEL
	T_LAPL_K	LaplK		;   // 결국 DB에는 Lapl단위로 저장이 된다..
	CArray<T_ELEM_K, T_ELEM_K> aElemK;

	void Initialize()
	{
		StldK		 = 0;
		LdgrK		 = 0;
		nDirection	 = D_ARPR_DIR_NORMAL;
		memset(dInnerPnt, 0, sizeof(dInnerPnt));
		memset(dVector, 0, sizeof(dVector));
		bProjection	 = FALSE;
		dP1			 = 0;
		nSelection	 = D_ARPR_SEL_GROUP;
		LaplK		 = 0;
		aElemK.RemoveAll();
	}
	T_ARPR_D()
	{
		Initialize();
	}
	T_ARPR_D(T_ARPR_D& src)
	{
		*this = src;
	}
	T_ARPR_D& operator=(const T_ARPR_D& src)
	{
		StldK		 = src.StldK		;
		LdgrK		 = src.LdgrK		;
		nDirection	 = src.nDirection	;
		memcpy(dInnerPnt, src.dInnerPnt, sizeof(dInnerPnt));
		memcpy(dVector, src.dVector, sizeof(dVector));
		bProjection	 = src.bProjection	;
		dP1			 = src.dP1			;
		nSelection	 = src.nSelection	;
		LaplK		 = src.LaplK		;
		aElemK.Copy(src.aElemK);
		return *this;
	}
};

struct T_ARPR_UDRD_D
{
	T_ARPR_K key;
	T_ARPR_D data;
};

struct T_ARPR_UDRD_D_RW
{
	T_ARPR_K key;

	T_STLD_K	StldK		;	
	T_LDGR_K	LdgrK		;	// Load Group
	int			nDirection	;	// D_ARPR_DIR
	double		dInnerPnt[3];	// 공간구조의 경우 Nomal을 구하기위한 좌표
	double		dVector[3]	;	// nDirection == D_ARPR_DIR_VECTOR일 때
	BOOL		bProjection	;	// nDirection == D_ARPR_DIR_GX,Y,Z일 때
	double		dP1			;
	int			nSelection	;	// D_ARPR_SEL
	T_LAPL_K	LaplK		;   // Loading Area Plane
	int			nElemKSize	;

	void GetArpr(T_ARPR_K& rKey, T_ARPR_D& rData)
	{
		rKey = key;
		rData.StldK		 = StldK		;
		rData.LdgrK		 = LdgrK		;
		rData.nDirection = nDirection	;
		memcpy(rData.dInnerPnt, dInnerPnt, sizeof(rData.dInnerPnt));
		memcpy(rData.dVector, dVector, sizeof(rData.dVector));
		rData.bProjection= bProjection	;
		rData.dP1		 = dP1			;
		rData.nSelection = nSelection	;
		rData.LaplK		 = LaplK		;
	}
	void SetArpr(const T_ARPR_K tKey, const T_ARPR_D& rData)
	{
		key = tKey;
		StldK		= rData.StldK		;
		LdgrK		= rData.LdgrK		;
		nDirection	= rData.nDirection	;
		memcpy(dInnerPnt, rData.dInnerPnt, sizeof(dInnerPnt));
		memcpy(dVector, rData.dVector, sizeof(dVector));
		bProjection	= rData.bProjection	;
		dP1			= rData.dP1			;
		nSelection	= rData.nSelection	;
		LaplK		= rData.LaplK		;
		nElemKSize	= (int)rData.aElemK.GetSize();
	}
};

struct T_ARPR_UNIT
{
	T_ARPR_UNIT()
	{
		dInnerPnt = D_UNITSYS_BASE_LENGTH;
		dVector	  = D_UNITSYS_BASE_LENGTH;
		dP1		  = D_UNITSYS_BASE_PRESSURE;
	}

	int dInnerPnt;
	int dVector;
	int dP1;
}; 

#define HASHSIZEARPR 101

#define T_STBK_K unsigned int
#define D_STBK_MAXNOD     2
struct T_STBK_D
{
	T_STLD_K LoadCaseKey; // v955 부터 사용 안함
	T_LDGR_K LoadGrpKey; // Load Group Key

	T_NODE_K node1;
	T_NODE_K node2;
	double dDx;
	double dDy;
	double dDz;

	void Initialize()
	{
		LoadCaseKey = 0;
		LoadGrpKey = 0;
		node1 = 0;
		node2 = 0;
		dDx = 0.0;
		dDy = 0.0;
		dDz = 0.0;
	}
	BOOL operator==(const T_STBK_D& src) const
	{
		if (LoadCaseKey != src.LoadCaseKey) return FALSE;
		if (LoadGrpKey != src.LoadGrpKey) return FALSE;
		if (node1 != src.node1) return FALSE;
		if (node2 != src.node2) return FALSE;
		if (dDx != src.dDx || dDy != src.dDy || dDz != src.dDz) return FALSE;
		return TRUE;
	}
};

struct T_STBK_UDRD_D
{
	T_STBK_K key;
	T_STBK_D data;
};
struct T_STBK_UNIT
{
	T_STBK_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		LoadGrpKey=D_UNITSYS_NONE;
		Node1=D_UNITSYS_NONE;
		Node2=D_UNITSYS_NONE;
		dDx = D_UNITSYS_BASE_LENGTH;
		dDy = D_UNITSYS_BASE_LENGTH;
		dDz = D_UNITSYS_BASE_LENGTH;
	}
	int LoadCaseKey;
	int LoadGrpKey;
	int Node1;
	int Node2;
	int dDx;
	int dDy;
	int dDz;	
}; 
#define HASHSIZESTBK 1001

#define T_ULCB_K unsigned int
struct T_ULCB_D
{
	T_ELEM_K ElemKey;
	T_STLD_K LoadCaseKey; // v955 부터 사용 안함
	T_LDGR_K LoadGrpKey;
	double dLength;

	void Initialize()
	{
		ElemKey = 0;
		LoadCaseKey = 0;
		LoadGrpKey = 0;
		dLength = 0.0;
	}
	BOOL operator==(const T_ULCB_D& src) const
	{
		if (ElemKey != src.ElemKey) return FALSE;
		if (LoadCaseKey != src.LoadCaseKey) return FALSE;
		if (LoadGrpKey != src.LoadGrpKey) return FALSE;
		if (dLength != src.dLength) return FALSE;
		return TRUE;
	}
};

struct T_ULCB_UDRD_D
{
	T_ULCB_K key;
	T_ULCB_D data;
};
struct T_ULCB_UNIT
{
	T_ULCB_UNIT()
	{
		ElemKey = D_UNITSYS_NONE;
		LoadCaseKey = D_UNITSYS_NONE;
		LoadGrpKey = D_UNITSYS_NONE;
		dLength = D_UNITSYS_BASE_LENGTH;
	}
	int ElemKey;
	int LoadCaseKey;
	int LoadGrpKey;
	int dLength;
}; 
#define HASHSIZEULCB 1001

// Load Cobmination Live Load Type
#define T_LLLT_K T_STLD_K
struct T_LLLT_D
{
	enum enLiveLoadType
	{
		EN_TYPE_LLMAX = 0,
		EN_TYPE_LLINS,
		EN_TYPE_LLMED,
	};
	enLiveLoadType LLType;
	int nIndex;				// 테이블에서 보여주는 index용, 다른 의미는 없음
	void Initialize()
	{
		LLType = EN_TYPE_LLMAX;
		nIndex = 0;
	}
	BOOL operator==(const T_LLLT_D& src) const
	{
		if (LLType != src.LLType) return FALSE;
		if (nIndex != src.nIndex) return FALSE;
		return TRUE;
	}
};

struct T_LLLT_UDRD_D
{
	T_LLLT_K key;
	T_LLLT_D data;
};
struct T_LLLT_UNIT
{
	T_LLLT_UNIT()
	{
		LLType = D_UNITSYS_NONE;
	}
	int LLType;
};
#define HASHSIZELLLT 1001
////////////////////////////////////
////////////////////////////////////
/*
struct T_EQMF_BASE
{
	int  nType;     // 0=Truss, 1=Beam, 2=Elnk
	UINT ElemKey;   // nType이 0, 1일 때는 Element Key, nType이 2일 때는 Elastic Key
	double dForceI[6];
	double dForceJ[6];
	void Initialize()
	{
		nType = 0;
		ElemKey = 0;
		for(int i = 0; i < 6; i++) dForceI[i] = dForceJ[i] = 0.;    
	}
};
struct T_EQMF_D
{
	T_STLD_K StldKey;
	CArray<T_EQMF_BASE, T_EQMF_BASE&> aEqmfBaseNode;
	CArray<T_EQMF_BASE, T_EQMF_BASE&> aEqmfBaseMemb;
	void Initialize()
	{
		StldKey = 0;
		aEqmfBaseNode.RemoveAll();
		aEqmfBaseMemb.RemoveAll();
	}
	T_EQMF_D() {}
	T_EQMF_D(T_EQMF_D& rData) { *this = rData; }
	T_EQMF_D& operator=(T_EQMF_D& src)
	{
		StldKey         = src.StldKey;
		aEqmfBaseNode.Copy(src.aEqmfBaseNode);
		aEqmfBaseMemb.Copy(src.aEqmfBaseMemb);
		return *this;
	}
};
struct T_EQMF_UDRD_D
{  
	T_EQMF_D data;
};
struct T_EQMF_UDRD_D_RW
{  
	T_STLD_K StldKey;
	int    nCountBaseNode;
	int    nCountBaseMemb;
	void GetEqmf(T_EQMF_D& rData)
	{
		rData.StldKey = StldKey;    
	}
	void SetEqmf(T_EQMF_D& rData)
	{
		StldKey = rData.StldKey;   
		nCountBaseNode = rData.aEqmfBaseNode.GetSize();
		nCountBaseMemb = rData.aEqmfBaseMemb.GetSize();
	}
};
struct T_EQMF_UNIT
{
	T_EQMF_UNIT()
	{
		nForce[0] = D_UNITSYS_BASE_FORCE;
		nForce[1] = D_UNITSYS_BASE_FORCE;
		nForce[2] = D_UNITSYS_BASE_FORCE;
		nForce[3] = D_UNITSYS_BASE_MOMENT;
		nForce[4] = D_UNITSYS_BASE_MOMENT;
		nForce[5] = D_UNITSYS_BASE_MOMENT;
	}
	int nForce[6];  
};
#define HASHSIZEEQMF 1
*/

#pragma pack(pop)

#endif  // __DB_ST_DT_STLD_H__
