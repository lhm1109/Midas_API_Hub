#ifndef __DB_ST_DT_POST_H__
#define __DB_ST_DT_POST_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// cutting line
#define T_CUTL_K unsigned int
struct T_CUTL_D_761
{
	CString CutLineName;
	int nDir;  // 0, 1
	double Point1[3], Point2[3];
	COLORREF Color;
};
struct T_CUTL_D_CH_761
{
	char CutLineName[40];
	int nDir;  // 0, 1
	double Point1[3], Point2[3];
	COLORREF Color;
};
struct T_CUTL_UDRD_D_CH_761
{
	T_CUTL_K key;
	T_CUTL_D_CH_761 data;
};
struct T_CUTL_D
{
	CString CutLineName;
	int nDir;  // 0, 1
	double Point1[3], Point2[3];
	COLORREF Color;

	int nType; //0:일반후처리, 1:Slab Design, 2:Slab Shear check //770추가

	T_CUTL_D()
	{
	}
	T_CUTL_D(T_CUTL_D& src)
	{
		*this = src;
	}
	T_CUTL_D& operator=(const T_CUTL_D& src)
	{
		CutLineName = src.CutLineName;
		nDir  = src.nDir;
		for(int i=0; i<3; i++)Point1[i] = src.Point1[i];
		for(int j=0; j<3; j++)Point2[j] = src.Point2[j];
		Color = src.Color;
		nType = src.nType;
		return *this;
	}
	void Initialize()
	{
		CutLineName.Empty();
		nDir=0;
		memset(Point1, 0, sizeof(Point1));
		memset(Point2, 0, sizeof(Point2));
		Color=0;
		nType=0;
	}
	void Convert761(T_CUTL_D_761& rData)
	{
		CutLineName = rData.CutLineName;
		nDir  = rData.nDir;
		for(int i=0; i<3; i++)Point1[i] = rData.Point1[i];
		for(int j=0; j<3; j++)Point2[j] = rData.Point2[j];
		Color = rData.Color;
		nType = 0;
	}
};
struct T_CUTL_D_CH
{
	char CutLineName[40];
	int nDir;
	double Point1[3], Point2[3];
	COLORREF Color;
	int nType; //0:일반후처리, 1:Slab Design, 2:Slab Shear check //770추가
	void ConvertToString(T_CUTL_D& rData)
	{
		ConvertCharStr(CutLineName, rData.CutLineName, sizeof(CutLineName));
		rData.nDir=nDir;
		rData.Color=Color;
		memcpy(rData.Point1, Point1, sizeof(Point1));
		memcpy(rData.Point2, Point2, sizeof(Point2));
		rData.nType=nType;
	}
	void ConvertToChar(T_CUTL_D& rData)
	{
		ConvertStrChar(rData.CutLineName, CutLineName, sizeof(CutLineName));
		nDir=rData.nDir;
		Color=rData.Color;
		memcpy(Point1, rData.Point1, sizeof(Point1));
		memcpy(Point2, rData.Point2, sizeof(Point2));
		nType=rData.nType;
	}
	void Convert761(T_CUTL_D_CH_761& rData)
	{
		memcpy(CutLineName, rData.CutLineName, sizeof(CutLineName));
		nDir=rData.nDir;
		Color=rData.Color;
		memcpy(Point1, rData.Point1, sizeof(Point1));
		memcpy(Point2, rData.Point2, sizeof(Point2));
		nType=0;
	}
};
struct T_CUTL_UDRD_D
{
	T_CUTL_K key;
	T_CUTL_D data;
};
struct T_CUTL_UDRD_D_CH
{
	T_CUTL_K key;
	T_CUTL_D_CH data;
};
struct T_CUTL_UNIT
{
	T_CUTL_UNIT()
	{
		CutLineName=D_UNITSYS_NONE;
		nDir=D_UNITSYS_NONE;
		Point=D_UNITSYS_BASE_LENGTH;
		nType=D_UNITSYS_NONE;
	}
	int CutLineName;
	int nDir;
	int Point;
	int nType;
};
#define HASHSIZECUTL 11

// cutting line with plane
#define T_CLWP_K unsigned int
struct T_CLWP_D
{
	CString CutLineName;
	int nDir;  // 0, 1
	double Point1[3], Point2[3], Point3[3];
	COLORREF Color;
	T_CLWP_D()
	{
	}
	T_CLWP_D(const T_CLWP_D& src)
	{
		*this = src;
	}
	T_CLWP_D& operator=(const T_CLWP_D& src)
	{
		CutLineName = src.CutLineName;
		nDir  = src.nDir;
		for(int i=0; i<3; i++)Point1[i] = src.Point1[i];
		for(int j=0; j<3; j++)Point2[j] = src.Point2[j];
		for(int k=0; k<3; k++)Point3[k] = src.Point3[k];
		Color = src.Color;
		return *this;
	}
	void Initialize()
	{
		CutLineName.Empty();
		nDir=0;
		memset(Point1, 0, sizeof(Point1));
		memset(Point2, 0, sizeof(Point2));
		memset(Point3, 0, sizeof(Point3));
		Color=0;
	}
};
struct T_CLWP_D_CH
{
	char CutLineName[40];
	int nDir;
	double Point1[3], Point2[3], Point3[3];
	COLORREF Color;
	void ConvertToString(T_CLWP_D& rData)
	{
		ConvertCharStr(CutLineName, rData.CutLineName, sizeof(CutLineName));
		rData.nDir=nDir;
		rData.Color=Color;
		memcpy(rData.Point1, Point1, sizeof(Point1));
		memcpy(rData.Point2, Point2, sizeof(Point2));
		memcpy(rData.Point3, Point3, sizeof(Point3));
	}
	void ConvertToChar(T_CLWP_D& rData)
	{
		ConvertStrChar(rData.CutLineName, CutLineName, sizeof(CutLineName));
		nDir=rData.nDir;
		Color=rData.Color;
		memcpy(Point1, rData.Point1, sizeof(Point1));
		memcpy(Point2, rData.Point2, sizeof(Point2));
		memcpy(Point3, rData.Point3, sizeof(Point3));
	}
};
struct T_CLWP_UDRD_D
{
	T_CLWP_K key;
	T_CLWP_D data;
};
struct T_CLWP_UDRD_D_CH
{
	T_CLWP_K key;
	T_CLWP_D_CH data;
};
struct T_CLWP_UNIT
{
	T_CLWP_UNIT()
	{
		Point=D_UNITSYS_BASE_LENGTH;
	}
	int Point;
};
#define HASHSIZECLWP 11

// Cable Force Tuning - Result
#define T_CFTR_K unsigned int
struct T_CFTR_D
{
	CString strName;
	UINT GroupKey;
	int nType; // 0:BeamForce 1:TrussForce 2:Displacement 3:BeamStress
	int nDof; // 0:Fx ...
	int nXAxisDof; // 0:DX+ 1:DX- 2:DY+...
	int nXAxisType; // 0:Distance 1:Element
	T_CFTR_D()
	{
		Initialize();
	}
	T_CFTR_D(T_CFTR_D& src)
	{
		*this = src;
	}
	T_CFTR_D& operator=(const T_CFTR_D& src)
	{
		strName = src.strName;
		GroupKey = src.GroupKey;
		nType = src.nType;
		nDof = src.nDof;
		nXAxisDof = src.nXAxisDof;
		nXAxisType = src.nXAxisType;
		return *this;
	}
	void Initialize()
	{
		strName = "";
		GroupKey = 0;
		nType = 0;
		nDof = 0;
		nXAxisDof = 0;
		nXAxisType = 0;
	}
};
struct T_CFTR_D_CH
{
	char strName[40];
	UINT GroupKey;
	int nType;
	int nDof;
	int nXAxisDof;
	int nXAxisType;

	void ConvertToString(T_CFTR_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.GroupKey = GroupKey;
		rData.nType = nType;
		rData.nDof = nDof;
		rData.nXAxisDof = nXAxisDof;
		rData.nXAxisType = nXAxisType;
	}
	void ConvertToChar(T_CFTR_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		GroupKey = rData.GroupKey;
		nType = rData.nType;
		nDof = rData.nDof;
		nXAxisDof = rData.nXAxisDof;
		nXAxisType = rData.nXAxisType;
	}
};
struct T_CFTR_UDRD_D
{
	T_CFTR_K key;
	T_CFTR_D data;
};
struct T_CFTR_UDRD_D_CH
{
	T_CFTR_K key;
	T_CFTR_D_CH data;
};
struct T_CFTR_UNIT
{
	T_CFTR_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZECFTR 11

// unknown load factor constraint
#define T_ULFC_K unsigned int
struct T_ULFC_D_580
{
	CString ConstraintName;
	int nConstraintType; // 0: Reaction, 1:Displ. 2: Truss Force 3: Beam Force
	unsigned int ObjectID;  // Node or Elem Id
	int nPoint; // for Beam or Truss only, For Truss 0:I, 1:J, For Beam, 0:I 1:1/4, 2:2/4, 3:3/4, 4:J
	int nComponent; // for React & Displ. 0~5: DX~RZ, For Truss Always 0, For Beam, 0~5: Axial~MntZ
	int nEqualityCond; // 0: <= ,1: = , 2: >= 
	BOOL bValue; // TRUE: Value type, FALSE: Other Object Type
	double dValue;
	unsigned int OtherObject;
};
struct T_ULFC_D_CH_580
{
	char ConstraintName[40];
	int nConstraintType; // 0: Reaction, 1:Displ. 2: Truss Force 3: Beam Force
	unsigned int ObjectID;  // Node or Elem Id
	int nPoint; // for Beam or Truss only, For Truss 0:I, 1:J, For Beam, 0:I 1:1/4, 2:2/4, 3:3/4, 4:J
	int nComponent; // for React & Displ. 0~5: DX~RZ, For Truss Always 0, For Beam, 0~5: Axial~MntZ
	int nEqualityCond; // 0: <= ,1: = , 2: >= 
	BOOL bValue; // TRUE: Value type, FALSE: Other Object Type
	double dValue;
	unsigned int OtherObject;
};
struct T_ULFC_UDRD_D_CH_580
{
	T_ULFC_K key;
	T_ULFC_D_CH_580 data;
};
struct T_ULFC_D
{
	CString ConstraintName;
	int nConstraintType; // 0: Reaction, 1:Displ. 2: Truss Force 3: Beam Force
	unsigned int ObjectID;  // Node or Elem Id
	int nPoint; // for Beam or Truss only, For Truss 0:I, 1:J, For Beam, 0:I 1:1/4, 2:2/4, 3:3/4, 4:J
	int nComponent; // for React & Displ. 0~5: DX~RZ, For Truss Always 0, For Beam, 0~5: Axial~MntZ
	int nEqualityCond; // 0: equality,1: inequality
	BOOL bValue; // TRUE: Value type, FALSE: Other Object Type
	double dValue;
	unsigned int OtherObject;
	BOOL bUpperBound;      // 5.9.0 추가
	BOOL bLowerBound;      // 5.9.0 추가
	double dUpperBound;    // 5.9.0 추가 
	double dLowerBound;    // 5.9.0 추가 
	T_ULFC_D()
	{
	}
	T_ULFC_D(T_ULFC_D& src)
	{
		*this = src;
	}
	T_ULFC_D& operator=(const T_ULFC_D& src)
	{
		ConstraintName=src.ConstraintName;
		nConstraintType=src.nConstraintType;
		ObjectID=src.ObjectID;
		nPoint=src.nPoint;
		nComponent=src.nComponent;
		nEqualityCond=src.nEqualityCond;
		bValue=src.bValue;
		dValue=src.dValue;
		OtherObject=src.OtherObject;
		bUpperBound = src.bUpperBound;
		bLowerBound = src.bLowerBound;
		dUpperBound = src.dUpperBound;
		dLowerBound = src.dLowerBound;
		return *this;
	}
	void Initialize()
	{
		ConstraintName.Empty();
		nConstraintType=0;
		ObjectID=0;
		nPoint=0;
		nComponent=0;
		nEqualityCond=0;
		bValue=TRUE;
		dValue=0.;
		OtherObject=0;
		bUpperBound = FALSE;
		bLowerBound = FALSE;
		dUpperBound = 0.;
		dLowerBound = 0.;
	}
	void Convert580(T_ULFC_D_580& rData)
	{
		ConstraintName = rData.ConstraintName;
		nConstraintType= rData.nConstraintType;
		ObjectID = rData.ObjectID;
		nPoint   = rData.nPoint;
		nComponent = rData.nComponent;    
		if(rData.nEqualityCond == 1) nEqualityCond = 0;
		else nEqualityCond = 1;
		bValue = rData.bValue;
		dValue = rData.dValue;
		OtherObject = rData.OtherObject;
		bUpperBound = FALSE;
		bLowerBound = FALSE;
		dUpperBound = 0.;
		dLowerBound = 0.;
		if(rData.nEqualityCond == 0) // <=
		{
			bUpperBound = TRUE;
			if(rData.bValue) dUpperBound = rData.dValue;
		}
		else if(rData.nEqualityCond == 2)  // >= 
		{      
			bLowerBound = TRUE;
			if(rData.bValue) dLowerBound = rData.dValue;
		}
	}
};
struct T_ULFC_D_CH
{
	char ConstraintName[40];
	int nConstraintType; // 0: Reaction, 1:Displ. 2: Truss Force 3: Beam Force
	unsigned int ObjectID;  // Node or Elem Id
	int nPoint; // for Beam or Truss only, For Truss 0:I, 1:J, For Beam, 0:I 1:1/4, 2:2/4, 3:3/4, 4:J
	int nComponent; // for React & Displ. 0~5: DX~RZ, For Truss Always 0, For Beam, 0~5: Axial~MntZ
	int nEqualityCond; // 0: <= ,1: = , 2: >= 
	BOOL bValue; // TRUE: Value type, FALSE: Other Object Type
	double dValue;
	unsigned int OtherObject;
	BOOL bUpperBound;      // 5.9.0 추가
	BOOL bLowerBound;      // 5.9.0 추가
	double dUpperBound;    // 5.9.0 추가 
	double dLowerBound;    // 5.9.0 추가 
	void ConvertToString(T_ULFC_D& rData)
	{
		ConvertCharStr(ConstraintName, rData.ConstraintName, sizeof(ConstraintName));
		rData.nConstraintType=nConstraintType;
		rData.ObjectID=ObjectID;
		rData.nPoint=nPoint;
		rData.nComponent=nComponent;
		rData.nEqualityCond=nEqualityCond;
		rData.bValue=bValue;
		rData.dValue=dValue;
		rData.OtherObject=OtherObject;
		rData.bUpperBound = bUpperBound;
		rData.bLowerBound = bLowerBound;
		rData.dUpperBound = dUpperBound;
		rData.dLowerBound = dLowerBound;
	}
	void ConvertToChar(T_ULFC_D& rData)
	{
		ConvertStrChar(rData.ConstraintName, ConstraintName, sizeof(ConstraintName));
		nConstraintType=rData.nConstraintType;
		ObjectID=rData.ObjectID;
		nPoint=rData.nPoint;
		nComponent=rData.nComponent;
		nEqualityCond=rData.nEqualityCond;
		bValue=rData.bValue;
		dValue=rData.dValue;
		OtherObject=rData.OtherObject;
		bUpperBound = rData.bUpperBound;
		bLowerBound = rData.bLowerBound;
		dUpperBound = rData.dUpperBound;
		dLowerBound = rData.dLowerBound;
	}
	void Convert580(T_ULFC_D_CH_580& rData)
	{
		memcpy(ConstraintName, rData.ConstraintName, sizeof(ConstraintName));
		nConstraintType= rData.nConstraintType;
		ObjectID = rData.ObjectID;
		nPoint   = rData.nPoint;
		nComponent = rData.nComponent;    
		if(rData.nEqualityCond == 1) nEqualityCond = 0;
		else nEqualityCond = 1;
		bValue = rData.bValue;
		dValue = rData.dValue;
		OtherObject = rData.OtherObject;
		bUpperBound = FALSE;
		bLowerBound = FALSE;
		dUpperBound = 0.;
		dLowerBound = 0.;
		if(rData.nEqualityCond == 0) // <=
		{
			bUpperBound = TRUE;
			if(rData.bValue) dUpperBound = rData.dValue;
		}
		else if(rData.nEqualityCond == 2)  // >= 
		{      
			bLowerBound = TRUE;
			if(rData.bValue) dLowerBound = rData.dValue;
		}
	}
};
struct T_ULFC_UDRD_D
{
	T_ULFC_K key;
	T_ULFC_D data;
};
struct T_ULFC_UDRD_D_CH
{
	T_ULFC_K key;
	T_ULFC_D_CH data;
};
struct T_ULFC_UNIT
{
	T_ULFC_UNIT()
	{
		ConstraintName=D_UNITSYS_NONE;
		nConstraintType=D_UNITSYS_NONE;
		ObjectID=D_UNITSYS_NONE;
		nPoint=D_UNITSYS_NONE;
		nComponent=D_UNITSYS_NONE;
		nEqualityCond=D_UNITSYS_NONE;
		bValue=D_UNITSYS_NONE;
		dValue=D_UNITSYS_NONE;   // 단위가변 UnitCtrl에서 Data Option 따라 가변처리
		OtherObject=D_UNITSYS_NONE;
		dUpperBound = D_UNITSYS_NONE;
		dLowerBound = D_UNITSYS_NONE;
	}
	int ConstraintName;
	int nConstraintType;
	int ObjectID;
	int nPoint;
	int nComponent;
	int nEqualityCond;
	int bValue;
	int dValue;
	int OtherObject;
	int dUpperBound;
	int dLowerBound;
};
#define HASHSIZEULFC 11

// unknown load factor
#define T_ULFD_K unsigned int
struct T_ULFD_BASE
{
	T_STLD_K KeyStld;
	double WeightedFactor;
};
struct T_ULFD_D
{
	CString ItemName;
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_LCOM_K LComKey;
	CArray<T_ULFD_BASE, T_ULFD_BASE&> arUnknownData; // 미지하중 LoadCase Key, Weighted Factor
	CArray<T_ULFC_K, T_ULFC_K> arConstraint;
	BOOL bSimulEqMethod;  // Simultaneous Equations Method : 7.0.0 에서 추가
	T_ULFD_D()
	{
	}
	T_ULFD_D(T_ULFD_D& src)
	{
		*this = src;
	}
	T_ULFD_D& operator=(const T_ULFD_D& src)
	{
		ItemName=src.ItemName;
		nLoadFactorType=src.nLoadFactorType;
		nFunctionType=src.nFunctionType;
		LComKey=src.LComKey;
		arUnknownData.Copy(src.arUnknownData);
		arConstraint.Copy(src.arConstraint);
		bSimulEqMethod=src.bSimulEqMethod;
		return *this;
	}
	void Initialize()
	{
		ItemName.Empty();
		nLoadFactorType=0;
		nFunctionType=0;
		LComKey=0;
		arUnknownData.RemoveAll();
		arConstraint.RemoveAll();
		bSimulEqMethod = FALSE;
	}
};
struct T_ULFD_D_CH
{
	char ItemName[40];
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_LCOM_K LComKey;
	CArray<T_ULFD_BASE, T_ULFD_BASE&> arUnknownData; // 미지하중 LoadCase Key, Weighted Factor
	CArray<T_ULFC_K, T_ULFC_K> arConstraint;
	BOOL bSimulEqMethod;
	void ConvertToString(T_ULFD_D& rData)
	{
		ConvertCharStr(ItemName, rData.ItemName, sizeof(ItemName));
		rData.nLoadFactorType=nLoadFactorType;
		rData.nFunctionType=nFunctionType;
		rData.LComKey=LComKey;
		rData.arUnknownData.Copy(arUnknownData);
		rData.arConstraint.Copy(arConstraint);
		rData.bSimulEqMethod = bSimulEqMethod;
	}
	void ConvertToChar(T_ULFD_D& rData)
	{
		ConvertStrChar(rData.ItemName, ItemName, sizeof(ItemName));
		nLoadFactorType=rData.nLoadFactorType;
		nFunctionType=rData.nFunctionType;
		LComKey=rData.LComKey;
		arUnknownData.Copy(rData.arUnknownData);
		arConstraint.Copy(rData.arConstraint);
		bSimulEqMethod = rData.bSimulEqMethod;
	}
};
struct T_ULFD_UDRD_D
{
	T_ULFD_K key;
	T_ULFD_D data;
};
struct T_ULFD_UDRD_D_CH
{
	T_ULFD_K key;
	T_ULFD_D_CH data;
};
struct T_ULFD_UDRD_D_CH_RW_692
{
	T_ULFD_K key;
	char ItemName[40];
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_LCOM_K LComKey;
	int nCountUnknownData;
	int nCountConstraint;
};
struct T_ULFD_UDRD_D_CH_RW
{
	T_ULFD_K key;
	char ItemName[40];
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_LCOM_K LComKey;
	int nCountUnknownData;
	int nCountConstraint;
	BOOL bSimulEqMethod;
	void GetUlfd(T_ULFD_K& rKey, T_ULFD_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.ItemName, ItemName, sizeof(ItemName));
		rData.nLoadFactorType=nLoadFactorType;
		rData.nFunctionType=nFunctionType;
		rData.LComKey=LComKey;
		rData.bSimulEqMethod = bSimulEqMethod;
	}
	void SetUlfd(const T_ULFD_K Key, const T_ULFD_D_CH& rData)
	{
		key=Key;
		memcpy(ItemName, rData.ItemName, sizeof(ItemName));
		nLoadFactorType=rData.nLoadFactorType;
		nFunctionType=rData.nFunctionType;
		LComKey=rData.LComKey;
		nCountUnknownData=(int)rData.arUnknownData.GetSize();
		nCountConstraint=(int)rData.arConstraint.GetSize();
		bSimulEqMethod = rData.bSimulEqMethod;
	}
	void Convert692(const T_ULFD_UDRD_D_CH_RW_692& rData)
	{
		key = rData.key;
		memcpy(ItemName, rData.ItemName, sizeof(ItemName));
		nLoadFactorType = rData.nLoadFactorType;
		nFunctionType = rData.nFunctionType;
		LComKey = rData.LComKey;
		nCountUnknownData = rData.nCountUnknownData;
		nCountConstraint = rData.nCountConstraint;
		bSimulEqMethod = FALSE;
	}
};
struct T_ULFD_UNIT
{
	T_ULFD_UNIT()
	{
		ItemName=D_UNITSYS_NONE;
		nLoadFactorType=D_UNITSYS_NONE;
		nFunctionType=D_UNITSYS_NONE;
		LComKey=D_UNITSYS_NONE;
		KeyStld=D_UNITSYS_NONE;
		WeightedFactor=D_UNITSYS_NONE;
		ConstraintKey=D_UNITSYS_NONE;
	}
	int ItemName;
	int nLoadFactorType;
	int nFunctionType;
	int LComKey;
	int KeyStld;
	int WeightedFactor;
	int ConstraintKey;
};
#define HASHSIZEULFD 11

// unknown load factor for Construction Stage
#define T_ULFS_K unsigned int
struct T_ULFS_BASE_650
{
	T_STAG_K StagKey;
	double WeightedFactor;
};
struct T_ULFS_BASE_761
{
	T_STAG_K StagKey;
	double WeightedFactor;
	int    nStep;             // 6.6.0 추가
};
struct T_ULFS_BASE
{
	T_STAG_K StagKey;
	double WeightedFactor;
	int    nStep;             // 6.6.0 추가
	int    nDeadErection;     // 7.7.0 추가 (0:ALL, 1:Dead, 2:Erection)
	void Initialize()
	{
		StagKey = 0;
		WeightedFactor = 1.0;
		nStep = 1;
		nDeadErection = 0;
	}
};
struct T_ULFS_D
{
	CString ItemName;
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_STAG_K StagKey;
	CArray<T_ULFS_BASE, T_ULFS_BASE&> arUnknownData; // Construntion Stage Step Key(첫 단계부터 StagKey 단계까지, Weighted Factor
	CArray<T_ULFC_K, T_ULFC_K> arConstraint;
	BOOL bSimulEqMethod;
	T_ULFS_D()
	{
	}
	T_ULFS_D(T_ULFS_D& src)
	{
		*this = src;
	}
	T_ULFS_D& operator=(const T_ULFS_D& src)
	{
		ItemName=src.ItemName;
		nLoadFactorType=src.nLoadFactorType;
		nFunctionType=src.nFunctionType;
		StagKey=src.StagKey;
		arUnknownData.Copy(src.arUnknownData);
		arConstraint.Copy(src.arConstraint);
		bSimulEqMethod=src.bSimulEqMethod;
		return *this;
	}
	void Initialize()
	{
		ItemName.Empty();
		nLoadFactorType=0;
		nFunctionType=0;
		StagKey=0;
		arUnknownData.RemoveAll();
		arConstraint.RemoveAll();
		bSimulEqMethod = FALSE;
	}
};
struct T_ULFS_D_CH
{
	char ItemName[40];
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_STAG_K StagKey;
	CArray<T_ULFS_BASE, T_ULFS_BASE&> arUnknownData; // 미지하중 LoadCase Key, Weighted Factor
	CArray<T_ULFC_K, T_ULFC_K> arConstraint;
	BOOL bSimulEqMethod;
	void ConvertToString(T_ULFS_D& rData)
	{
		ConvertCharStr(ItemName, rData.ItemName, sizeof(ItemName));
		rData.nLoadFactorType=nLoadFactorType;
		rData.nFunctionType=nFunctionType;
		rData.StagKey=StagKey;
		rData.arUnknownData.Copy(arUnknownData);
		rData.arConstraint.Copy(arConstraint);
		rData.bSimulEqMethod = bSimulEqMethod;
	}
	void ConvertToChar(T_ULFS_D& rData)
	{
		ConvertStrChar(rData.ItemName, ItemName, sizeof(ItemName));
		nLoadFactorType=rData.nLoadFactorType;
		nFunctionType=rData.nFunctionType;
		StagKey=rData.StagKey;
		arUnknownData.Copy(rData.arUnknownData);
		arConstraint.Copy(rData.arConstraint);
		bSimulEqMethod = rData.bSimulEqMethod;
	}
};
struct T_ULFS_UDRD_D
{
	T_ULFS_K key;
	T_ULFS_D data;
};
struct T_ULFS_UDRD_D_CH
{
	T_ULFS_K key;
	T_ULFS_D_CH data;
};
struct T_ULFS_UDRD_D_CH_RW_650
{
	T_ULFS_K key;
	char ItemName[40];
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_STAG_K StagKey;
	int nCountUnknownData;
	int nCountConstraint;
};
struct T_ULFS_UDRD_D_CH_RW_692
{
	T_ULFS_K key;
	char ItemName[40];
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_STAG_K StagKey;
	int nCountUnknownData;
	int nCountConstraint;
	int nTemp;             // Data Size 맞추기 위한 임시변수
};
struct T_ULFS_UDRD_D_CH_RW
{
	T_ULFS_K key;
	char ItemName[40];
	int nLoadFactorType; // 0: Negative, 1: Both  2:Positive, 
	int nFunctionType; //  0: Linear sum,  1: Square sum 2: Maximum absolute
	T_STAG_K StagKey;
	int nCountUnknownData;
	int nCountConstraint;
	int nTemp;             // Data Size 맞추기 위한 임시변수
	BOOL bSimulEqMethod;  // 7.0.0에서 추가
	void GetUlfs(T_ULFS_K& rKey, T_ULFS_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.ItemName, ItemName, sizeof(ItemName));
		rData.nLoadFactorType=nLoadFactorType;
		rData.nFunctionType=nFunctionType;
		rData.StagKey=StagKey;  
		rData.bSimulEqMethod = bSimulEqMethod;
	}
	void SetUlfs(const T_ULFS_K Key, const T_ULFS_D_CH& rData)
	{
		key=Key;
		memcpy(ItemName, rData.ItemName, sizeof(ItemName));
		nLoadFactorType=rData.nLoadFactorType;
		nFunctionType=rData.nFunctionType;
		StagKey=rData.StagKey;
		nCountUnknownData=(int)rData.arUnknownData.GetSize();
		nCountConstraint=(int)rData.arConstraint.GetSize();  
		bSimulEqMethod = rData.bSimulEqMethod;
	}
	void Convert650(T_ULFS_UDRD_D_CH_RW_650& rData)
	{
		key = rData.key;
		memcpy(ItemName, rData.ItemName, sizeof(ItemName));
		nLoadFactorType = rData.nLoadFactorType;
		nFunctionType = rData.nFunctionType;
		StagKey = rData.StagKey;
		nCountUnknownData = rData.nCountUnknownData;
		nCountConstraint = rData.nCountConstraint;
		nTemp = 0;
		bSimulEqMethod = FALSE;
	}
	void Convert692(T_ULFS_UDRD_D_CH_RW_692& rData)
	{
		key = rData.key;
		memcpy(ItemName, rData.ItemName, sizeof(ItemName));
		nLoadFactorType = rData.nLoadFactorType;
		nFunctionType = rData.nFunctionType;
		StagKey = rData.StagKey;
		nCountUnknownData = rData.nCountUnknownData;
		nCountConstraint = rData.nCountConstraint;
		nTemp = rData.nTemp;
		bSimulEqMethod = FALSE;
	}
};
struct T_ULFS_UNIT
{
	T_ULFS_UNIT()
	{
		ItemName=D_UNITSYS_NONE;
		nLoadFactorType=D_UNITSYS_NONE;
		nFunctionType=D_UNITSYS_NONE;
		StagKey=D_UNITSYS_NONE;
		KeyStld=D_UNITSYS_NONE;
		WeightedFactor=D_UNITSYS_NONE;
		ConstraintKey=D_UNITSYS_NONE;
	}
	int ItemName;
	int nLoadFactorType;
	int nFunctionType;
	int StagKey;
	int KeyStld;
	int WeightedFactor;
	int ConstraintKey;
};
#define HASHSIZEULFS 11

// Define Diagram Group
#define T_DIAG_K unsigned int
struct T_DIAG_DOPT
{
	double dScaleFactor;
	COLORREF Color;
	int nFillType;  // 0:No Fill, 1:Line Fill, 2:Solid Fill
	BOOL bDisplayValue;
	int nDecimalPoints;
	BOOL bExp;
	BOOL bSetOrientation;
	double dAngle;
	char OutputSectLoc[8]; // '0':Off, '1':On, Index=0:I, 1:Center, 2:J, 3:Abs Max, 4:Min/Max, 5:All
	BOOL bMinMaxOnly;
	int nMinMaxOpt; // 0:Min&Max, 1:Abs Max, 2:Max, 3:Min
	double dLimitScale; // %
	void Initialize()
	{
		dScaleFactor = 1.0;
		Color = COLORREF(RGB(255, 0, 0));
		nFillType = 1;
		bDisplayValue = TRUE;
		nDecimalPoints = 3;
		bExp = FALSE;
		bSetOrientation = FALSE;
		dAngle = 0.0;
		OutputSectLoc[0] = '0';
		OutputSectLoc[1] = '0';
		OutputSectLoc[2] = '0';
		OutputSectLoc[3] = '0';
		OutputSectLoc[4] = '1';
		OutputSectLoc[5] = '0';
		bMinMaxOnly = FALSE;
		nMinMaxOpt = 0;
		dLimitScale = 0.1;
	}
};
struct T_DIAG_D
{
	CString GroupName;
	int nTypeOfResult;  // 0:Displacement, 1:Truss Force, 2:Truss Stress, 3:Beam Force, 4:Beam Stress, 5:PSC Beam Stress
	int nComponents;  // 0:DX, 1:DY, 2:DZ, 3:DXY, 4:DYZ, 5:DXZ, 6:DXYZ
										// 0:Fx, 1:Mx, 2:Fy, 3:Fz, 4:Fyz, 5:My, 6:Mz, 7:Myz
										// 0:Sig-xx(Axial), 1:Sig-xx, ...
										// 0:All, 1:Tens, 2:Comp (Force Filter(Truss Force) or Stress Filter(Truss Stress))
	int nSectPos;       // 0:Pos1, 1:Pos2, ..., 9:Pos10, 10:Max, 11:Min, 12:Max/Min, 13:Abs Max
											// PSC Beam Stress만 사용
	int nCombined;      // 0:Maximum, 1:1(-y+z), 2:2(+y+z), 3:3(+y-z), 4:4(-y-z)
											// Beam Stress만 사용
	int nBeamPscPart;   // 7.2.1 추가 - 0:Total, 1:Part1, 2:Part2, 3:Part3 ...
	                    // Beam Force/Moment, Beam Stress 에서만 사용 
	T_DIAG_DOPT DispOpt; // Display Option
	CArray<UINT, UINT> aElemK;
	T_DIAG_D()
	{
	}
	T_DIAG_D(T_DIAG_D& src)
	{
		*this = src;
	}
	T_DIAG_D& operator=(const T_DIAG_D& src)
	{
		GroupName=src.GroupName;
		nTypeOfResult=src.nTypeOfResult;
		nComponents=src.nComponents;
		nSectPos=src.nSectPos;
		nCombined=src.nCombined;
		nBeamPscPart=src.nBeamPscPart;
		DispOpt=src.DispOpt;
		aElemK.Copy(src.aElemK);
		return *this;
	}
	void Initialize()
	{
		GroupName.Empty();
		nTypeOfResult=0;
		nComponents=0;
		nSectPos=0;
		nCombined=0;
		nBeamPscPart=0;
		DispOpt.Initialize();
		aElemK.RemoveAll();
	}
};
struct T_DIAG_D_CH
{
	char GroupName[40];
	int nTypeOfResult;  // 0:Displacement, 1:Truss Force, 2:Truss Stress, 3:Beam Force, 4:Beam Stress, 5:PSC Beam Stress
	int nComponents;  // 0:DX, 1:DY, 2:DZ, 3:DXY, 4:DYZ, 5:DXZ, 6:DXYZ
										// 0:Fx, 1:Mx, 2:Fy, 3:Fz, 4:Fyz, 5:My, 6:Mz, 7:Myz
										// 0:Sig-xx(Axial), 1:Sig-xx, ...
	int nSectPos;       // 0:Pos1, 1:Pos2, ..., 9:Pos10, 10:Max, 11:Min, 12:Max/Min, 13:Abs Max
	int nCombined;      // 0:Maximum, 1:1(-y+z), 2:2(+y+z), 3:3(+y-z), 4:4(-y-z)
	T_DIAG_DOPT DispOpt; // Display Option
	CArray<UINT, UINT> aElemK;
	int nBeamPscPart;
	void ConvertToString(T_DIAG_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.nTypeOfResult=nTypeOfResult;
		rData.nComponents=nComponents;
		rData.nSectPos=nSectPos;
		rData.nCombined=nCombined;
		rData.DispOpt=DispOpt;
		rData.aElemK.Copy(aElemK);
		rData.nBeamPscPart=nBeamPscPart;
	}
	void ConvertToChar(T_DIAG_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		nTypeOfResult=rData.nTypeOfResult;
		nComponents=rData.nComponents;
		nSectPos=rData.nSectPos;
		nCombined=rData.nCombined;
		DispOpt=rData.DispOpt;
		aElemK.Copy(rData.aElemK);
		nBeamPscPart=rData.nBeamPscPart;
	}
};
struct T_DIAG_UDRD_D
{
	T_DIAG_K key;
	T_DIAG_D data;
};
struct T_DIAG_UDRD_D_CH
{
	T_DIAG_K key;
	T_DIAG_D_CH data;
};
struct T_DIAG_UDRD_D_CH_RW_720
{
	T_DIAG_K key;
	char GroupName[40];
	int nTypeOfResult;  // 0:Displacement, 1:Truss Force, 2:Truss Stress, 3:Beam Force, 4:Beam Stress, 5:PSC Beam Stress
	int nComponents;  // 0:DX, 1:DY, 2:DZ, 3:DXY, 4:DYZ, 5:DXZ, 6:DXYZ
										// 0:Fx, 1:Mx, 2:Fy, 3:Fz, 4:Fyz, 5:My, 6:Mz, 7:Myz
										// 0:Sig-xx(Axial), 1:Sig-xx, ...
	int nSectPos;       // 0:Pos1, 1:Pos2, ..., 9:Pos10, 10:Max, 11:Min, 12:Max/Min, 13:Abs Max
	int nCombined;      // 0:Maximum, 1:1(-y+z), 2:2(+y+z), 3:3(+y-z), 4:4(-y-z)
	T_DIAG_DOPT DispOpt; // Display Option
	int nCountElem;
};
struct T_DIAG_UDRD_D_CH_RW
{
	T_DIAG_K key;
	char GroupName[40];
	int nTypeOfResult;  // 0:Displacement, 1:Truss Force, 2:Truss Stress, 3:Beam Force, 4:Beam Stress, 5:PSC Beam Stress
	int nComponents;  // 0:DX, 1:DY, 2:DZ, 3:DXY, 4:DYZ, 5:DXZ, 6:DXYZ
										// 0:Fx, 1:Mx, 2:Fy, 3:Fz, 4:Fyz, 5:My, 6:Mz, 7:Myz
										// 0:Sig-xx(Axial), 1:Sig-xx, ...
	int nSectPos;       // 0:Pos1, 1:Pos2, ..., 9:Pos10, 10:Max, 11:Min, 12:Max/Min, 13:Abs Max
	int nCombined;      // 0:Maximum, 1:1(-y+z), 2:2(+y+z), 3:3(+y-z), 4:4(-y-z)
	T_DIAG_DOPT DispOpt; // Display Option
	int nCountElem;
	int nBeamPscPart;
	void GetDiag(T_DIAG_K& rKey, T_DIAG_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.GroupName, GroupName, sizeof(GroupName));
		rData.nTypeOfResult=nTypeOfResult;
		rData.nComponents=nComponents;
		rData.nSectPos=nSectPos;    
		rData.nCombined=nCombined;    
		rData.DispOpt=DispOpt;
		rData.nBeamPscPart=nBeamPscPart;
	}
	void SetDiag(const T_DIAG_K Key, const T_DIAG_D_CH& rData)
	{
		key=Key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nTypeOfResult=rData.nTypeOfResult;
		nComponents=rData.nComponents;
		nSectPos=rData.nSectPos;    
		nCombined=rData.nCombined;    
		DispOpt=rData.DispOpt;
		nBeamPscPart=rData.nBeamPscPart;
		nCountElem=(int)rData.aElemK.GetSize();    
	}
	void Convert720(T_DIAG_UDRD_D_CH_RW_720& rData)
	{
		key=rData.key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		nTypeOfResult=rData.nTypeOfResult;
		nComponents=rData.nComponents;
		nSectPos=rData.nSectPos;    
		nCombined=rData.nCombined;    
		DispOpt=rData.DispOpt;
		nCountElem=rData.nCountElem;
		nBeamPscPart=0;
	}
};
struct T_DIAG_UNIT
{
	T_DIAG_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEDIAG 11

// camb : camber control data (1개만 입력 가능)
#define T_CAMB_K unsigned int // always 1

struct T_CAMB_D_900
{
    T_GRUP_K BodyElemGroup_k;
    T_GRUP_K SupportNodeGroup_k;
    T_GRUP_K KeySegGroup_k;
    void Initialize()
    {
        BodyElemGroup_k = 0;
        SupportNodeGroup_k = 0;
        KeySegGroup_k = 0;
    }
};

struct T_CAMB_D
{
	T_GRUP_K BodyElemGroup_k;
	T_GRUP_K SupportNodeGroup_k;
	T_GRUP_K KeySegGroup_k;
    BOOL bRealDisp;
    BOOL bCambDisp;
	void Initialize()
	{
		BodyElemGroup_k = 0;
		SupportNodeGroup_k = 0;
		KeySegGroup_k = 0;
        bRealDisp = FALSE;
        bCambDisp = FALSE;
	}

    void Convert900(T_CAMB_D_900& rData)
    {
        Initialize();
        BodyElemGroup_k    = rData.BodyElemGroup_k;
        SupportNodeGroup_k = rData.SupportNodeGroup_k;
        KeySegGroup_k     = rData.KeySegGroup_k;
    }
};

struct T_CAMB_UDRD_D
{
	T_CAMB_D data;
};
struct T_CAMB_UNIT
{
	T_CAMB_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZECAMB 1

// gcmb : general camber control data (1개만 입력 가능)
#define T_GCMB_K unsigned int // always 1
struct T_GCMB_BASE
{
	T_GRUP_K KeyGrup;
	int      nDir;      // 1: +X, 2: -X, 3: +Y, 4: -Y
};
struct T_GCMB_D
{
	BOOL bStartPtZero;
	CArray<T_GCMB_BASE, T_GCMB_BASE> arGrupDir;
    BOOL bRealDisp;
    BOOL bCambDisp;
        
	void Initialize()
	{
		bStartPtZero = FALSE;
		arGrupDir.RemoveAll();   
        bRealDisp = FALSE;
        bCambDisp = FALSE;
	}
	T_GCMB_D()  { }
	T_GCMB_D(T_GCMB_D& src) {*this = src;}
	T_GCMB_D& operator=(const T_GCMB_D& src)
	{
		bStartPtZero = src.bStartPtZero;
		arGrupDir.Copy(src.arGrupDir);    
        bRealDisp = src.bRealDisp;
        bCambDisp = src.bCambDisp;
		return *this;
	}  
};
struct T_GCMB_UDRD_D
{  
	T_GCMB_D data;
};

struct T_GCMB_UDRD_D_RW_900
{
    BOOL bStartPtZero;
    unsigned int nCountGrupDir;
};

struct T_GCMB_UDRD_D_RW
{  
	BOOL bStartPtZero;
	unsigned int nCountGrupDir;  
    BOOL bRealDisp;
    BOOL bCambDisp;
	void GetGcmb(T_GCMB_D& rData)
	{
		rData.bStartPtZero = bStartPtZero;
        rData.bCambDisp = bCambDisp;
        rData.bRealDisp = bRealDisp;
	}
	void SetGcmb(const T_GCMB_D& rData)
	{
		bStartPtZero = rData.bStartPtZero;
		nCountGrupDir = (int)rData.arGrupDir.GetSize();    
        bRealDisp = rData.bRealDisp;
        bCambDisp = rData.bCambDisp;
	}  
    void Convert900(T_GCMB_UDRD_D_RW_900& rData)
    {
        bStartPtZero = rData.bStartPtZero;
        nCountGrupDir = rData.nCountGrupDir;
        bCambDisp = FALSE;
        bRealDisp = FALSE;
    }
};

struct T_GCMB_UNIT
{
	T_GCMB_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEGCMB 1

// Column Shortening
#define T_SCLM_K unsigned int
struct T_SCLM_D
{
	CString csName;
	double dX, dY;
	double dTol;
	int    nDispType;     // 0:Elastic, 1:Creep, 2:Shrinkage, 3:Toa
	int    nConstStage;   // 0:Up to casting, 1:Sub to casting, 2:Total
	//void Initialize(); --> use CAttrCtrl::InitializeSclm
};
struct T_SCLM_D_CH
{
	char    csName[32];
	double  dX, dY;
	double  dTol;
	int     nDispType;
	int     nConstStage;
	void ConvertToString(T_SCLM_D& rData)
	{
		ConvertCharStr(csName, rData.csName, sizeof(csName));
		rData.dX = dX;
		rData.dY = dY;
		rData.dTol = dTol;
		rData.nDispType = nDispType;
		rData.nConstStage = nConstStage;
	}
	void ConvertToChar(T_SCLM_D& rData)
	{
		ConvertStrChar(rData.csName, csName, sizeof(csName));
		dX = rData.dX;
		dY = rData.dY;
		dTol = rData.dTol;
		nDispType = rData.nDispType;
		nConstStage = rData.nConstStage;
	}
};
struct T_SCLM_UDRD_D
{
	T_SCLM_K key;
	T_SCLM_D data;
};
struct T_SCLM_UDRD_D_CH
{
	T_SCLM_K key;
	T_SCLM_D_CH data;
};
struct T_SCLM_UNIT
{
	T_SCLM_UNIT()
	{
		dXY = D_UNITSYS_BASE_LENGTH;
		dTol = D_UNITSYS_BASE_LENGTH;
	}
	int dXY;
	double dTol;
};
#define HASHSIZESCLM 11

// batch conversion from moving load trace to static load case
#define T_BCVT_TYPE_REAC      0
#define T_BCVT_TYPE_DEFORM    1
#define T_BCVT_TYPE_TRUSS     2
#define T_BCVT_TYPE_BEAM      3
#define T_BCVT_TYPE_PLATE     4
#define T_BCVT_TYPE_BEAM_STRS 5
#define T_BCVT_TYPE_ELNK      6
#define T_BCVT_TYPE_NLNK      7

#define T_BCVT_K unsigned int
struct T_BCVT_MVLD
{
	T_MVLD_K LoadCaseKey;
	int nMinMaxType;          // 0:Normal, 1:Max, 2:Min, 3:All
};
struct T_BCVT_D_861
{
	CString csName;
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
};
struct T_BCVT_D_865
{
	CString csName;
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
};

struct T_BCVT_D_870_Test // 삭제예정
{
	CString csName;
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
	int nFactorType;
};

struct T_BCVT_D
{
	CString csName;
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부 (원심력 여부로 사용)
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
	int nFactorType;

	BOOL bUseBrkForce; // Braking Force 사용 여부
	BOOL bUseTrkForce; // Tractive Force 사용 여부
	double dLongiH;    // Longitudinal Force 작용 위치
	double dSuElv;     // Super Elevation (%)
	double dTotalLength; // Total length of bridge
	
	T_BCVT_D()
	{
	}
	T_BCVT_D(T_BCVT_D& src)
	{
		*this = src;
	}
	T_BCVT_D& operator=(const T_BCVT_D& src)
	{
		csName=src.csName;
		nType=src.nType;
		arMovingLoadCase.Copy(src.arMovingLoadCase);
		arNodeElemKey.Copy(src.arNodeElemKey);
		memcpy(bParts, src.bParts, sizeof(bParts));
		memcpy(bComponents, src.bComponents, sizeof(bComponents));
		nCompType=src.nCompType;
		bUseHoriForce=src.bUseHoriForce;
		nHoriType=src.nHoriType;
		nCalcType=src.nCalcType;
		dRailH=src.dRailH;
		dRailV=src.dRailV;
		dRailR=src.dRailR;
		dRoadH=src.dRoadH;
		dRoadR=src.dRoadR;
		dPointQtk=src.dPointQtk;
		dDistqtk=src.dDistqtk;
		nDirType=src.nDirType;
		dRoadV=src.dRoadV;
		nFactorType=src.nFactorType;
		bUseBrkForce=src.bUseBrkForce;
		bUseTrkForce=src.bUseTrkForce;
		dLongiH=src.dLongiH;
		dSuElv =  src.dSuElv;
		dTotalLength = src.dTotalLength;

		return *this;
	}
	void Initialize()
	{
		csName.Empty();
		nType=0;
		arMovingLoadCase.RemoveAll();
		arNodeElemKey.RemoveAll();
		for (int i = 0; i < 5; i++) bParts[i] = FALSE;
		for (int i = 0; i < 8; i++) bComponents[i] = FALSE;
		nCompType=0;

		bUseHoriForce=FALSE;
		nHoriType=1;
		nCalcType=0;
		dRailH=0.0;
		dRailV=0.0;
		dRailR=0.0;
		dRoadH=0.0;
		dRoadR=0.0;
		dPointQtk=0.0;
		dDistqtk=0.0;
		nDirType=0;
		dRoadV=0.0;
		nFactorType=0;
		bUseBrkForce=FALSE;
		bUseTrkForce=FALSE;
		dLongiH=0.0;
		dSuElv= 0.0;
		dTotalLength = 0.0;
	}
};

struct T_BCVT_D_CH_861
{
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
};

struct T_BCVT_D_CH_865
{
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
};

struct T_BCVT_D_CH_870_Test
{
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
	int nFactorType;
};

struct T_BCVT_D_CH
{
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	CArray<T_BCVT_MVLD, T_BCVT_MVLD&> arMovingLoadCase;
	CArray<UINT, UINT> arNodeElemKey;   // nType=0,1 -> Node, nType=2,3,4 -> Elem
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
	int nFactorType;
	BOOL bUseBrkForce; 
	BOOL bUseTrkForce; 
	double dLongiH;    
	double dSuElv;
	double dTotalLength;

	void ConvertToString(T_BCVT_D& rData)
	{
		ConvertCharStr(csName, rData.csName, sizeof(csName));
		rData.nType=nType;
		memcpy(rData.bParts, bParts, sizeof(bParts));
		memcpy(rData.bComponents, bComponents, sizeof(bComponents));
		rData.arMovingLoadCase.Copy(arMovingLoadCase);
		rData.arNodeElemKey.Copy(arNodeElemKey);
		rData.nCompType=nCompType;
		rData.bUseHoriForce=bUseHoriForce;
		rData.nHoriType=nHoriType;
		rData.nCalcType=nCalcType;
		rData.dRailH=dRailH;
		rData.dRailV=dRailV;
		rData.dRailR=dRailR;
		rData.dRoadH=dRoadH;
		rData.dRoadR=dRoadR;
		rData.dPointQtk=dPointQtk;
		rData.dDistqtk=dDistqtk;
		rData.nDirType=nDirType;
		rData.dRoadV=dRoadV;
		rData.nFactorType=nFactorType;
		rData.bUseBrkForce=bUseBrkForce;
		rData.bUseTrkForce=bUseTrkForce;
		rData.dLongiH=dLongiH;
		rData.dSuElv=dSuElv;
		rData.dTotalLength = dTotalLength;

	}
	void ConvertToChar(T_BCVT_D& rData)
	{
		ConvertStrChar(rData.csName, csName, sizeof(csName));
		nType=rData.nType;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		arMovingLoadCase.Copy(rData.arMovingLoadCase);
		arNodeElemKey.Copy(rData.arNodeElemKey);
		nCompType=rData.nCompType;
		bUseHoriForce=rData.bUseHoriForce;
		nHoriType=rData.nHoriType;
		nCalcType=rData.nCalcType;
		dRailH=rData.dRailH;
		dRailV=rData.dRailV;
		dRailR=rData.dRailR;
		dRoadH=rData.dRoadH;
		dRoadR=rData.dRoadR;
		dPointQtk=rData.dPointQtk;
		dDistqtk=rData.dDistqtk;
		nDirType=rData.nDirType;
		dRoadV=rData.dRoadV;
		nFactorType=rData.nFactorType;
		bUseBrkForce=rData.bUseBrkForce;
		bUseTrkForce=rData.bUseTrkForce;
		dLongiH=rData.dLongiH;
		dSuElv=rData.dSuElv;
		dTotalLength = rData.dTotalLength;

	}
};
struct T_BCVT_UDRD_D_861
{
	T_BCVT_K key;
	T_BCVT_D_861 data;
};
struct T_BCVT_UDRD_D_865
{
	T_BCVT_K key;
	T_BCVT_D_865 data;
};
struct T_BCVT_UDRD_D_870_Test
{
	T_BCVT_K key;
	T_BCVT_D_870_Test data;
};
struct T_BCVT_UDRD_D
{
	T_BCVT_K key;
	T_BCVT_D data;
};

struct T_BCVT_UDRD_D_CH_861
{
	T_BCVT_K key;
	T_BCVT_D_CH_861 data;
};
struct T_BCVT_UDRD_D_CH_865
{
	T_BCVT_K key;
	T_BCVT_D_CH_865 data;
};
struct T_BCVT_UDRD_D_CH_870_Test
{
	T_BCVT_K key;
	T_BCVT_D_CH_870_Test data;
};
struct T_BCVT_UDRD_D_CH
{
	T_BCVT_K key;
	T_BCVT_D_CH data;
};

struct T_BCVT_UDRD_D_CH_RW_815
{
	T_BCVT_K key;
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	int nCountMovingLoadCase;
	int nCountNodeElemKey;
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
};
struct T_BCVT_UDRD_D_CH_RW_825
{
	T_BCVT_K key;
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	int nCountMovingLoadCase;
	int nCountNodeElemKey;
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
};

struct T_BCVT_UDRD_D_CH_RW_861
{
	T_BCVT_K key;
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	int nCountMovingLoadCase;
	int nCountNodeElemKey;
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
};
struct T_BCVT_UDRD_D_CH_RW_865
{
	T_BCVT_K key;
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	int nCountMovingLoadCase;
	int nCountNodeElemKey;
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
};

struct T_BCVT_UDRD_D_CH_RW_891
{
	T_BCVT_K key;
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	int nCountMovingLoadCase;
	int nCountNodeElemKey;
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
	int nFactorType;

	BOOL bUseBrkForce; 
	BOOL bUseTrkForce; 
	double dLongiH;    
};

struct T_BCVT_UDRD_D_CH_RW_895_test
{
	T_BCVT_K key;
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	int nCountMovingLoadCase;
	int nCountNodeElemKey;
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
	int nFactorType;

	BOOL bUseBrkForce; 
	BOOL bUseTrkForce; 
	double dLongiH;    
	double dSuElv;     // Super Elevation
};

struct T_BCVT_UDRD_D_CH_RW
{
	T_BCVT_K key;
	char csName[40];
	int nType;            // 0:Reaction, 1:Deformation, 2:Truss Force, 3:Beam Force, 4:Plate Force
	int nCountMovingLoadCase;
	int nCountNodeElemKey;
	BOOL bParts[5];       // i, 1/4, 1/2, 3/4, j
	BOOL bComponents[8];  // Fxx, Fyy, Fzz, Mxx, Myy, Mzz, Vxx, Vyy
	int nCompType; // Normal, Ssy, Ssz
	BOOL bUseHoriForce; // Horizontal Force 사용 여부
	int nHoriType; // 0:Road Traffic Action, 1:Rail Traffic Action
	int nCalcType; // 0:Auto Calculation, 1:User Input
	double dRailH;
	double dRailV;
	double dRailR;
	double dRoadH;
	double dRoadR;
	double dPointQtk;
	double dDistqtk;
	int nDirType; // 0:Convex, 1:Concave
	double dRoadV;
	int nFactorType;

	BOOL bUseBrkForce; 
	BOOL bUseTrkForce; 
	double dLongiH;    
	double dSuElv;     // Super Elevation
	double dTotalLength; // Total legnth of bridge

	void GetBcvt(T_BCVT_K& rKey, T_BCVT_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.csName, csName, sizeof(csName));
		rData.nType=nType;
		memcpy(rData.bParts, bParts, sizeof(bParts));
		memcpy(rData.bComponents, bComponents, sizeof(bComponents));
		rData.nCompType=nCompType;
		rData.bUseHoriForce=bUseHoriForce;
		rData.nHoriType=nHoriType;
		rData.nCalcType=nCalcType;
		rData.dRailH=dRailH;
		rData.dRailV=dRailV;
		rData.dRailR=dRailR;
		rData.dRoadH=dRoadH;
		rData.dRoadR=dRoadR;
		rData.dPointQtk=dPointQtk;
		rData.dDistqtk=dDistqtk;
		rData.nDirType=nDirType;
		rData.dRoadV=dRoadV;
		rData.nFactorType=nFactorType;
		rData.bUseBrkForce=bUseBrkForce; 
		rData.bUseTrkForce=bUseTrkForce; 
		rData.dLongiH=dLongiH;    
		rData.dSuElv=dSuElv;
		rData.dTotalLength = dTotalLength;
	}
	void SetBcvt(const T_BCVT_K Key, const T_BCVT_D_CH& rData)
	{
		key=Key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType=rData.nType;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		nCountMovingLoadCase=(int)rData.arMovingLoadCase.GetSize();
		nCountNodeElemKey=(int)rData.arNodeElemKey.GetSize();
		nCompType=rData.nCompType;
		bUseHoriForce=rData.bUseHoriForce;
		nHoriType=rData.nHoriType;
		nCalcType=rData.nCalcType;
		dRailH=rData.dRailH;
		dRailV=rData.dRailV;
		dRailR=rData.dRailR;
		dRoadH=rData.dRoadH;
		dRoadR=rData.dRoadR;
		dPointQtk=rData.dPointQtk;
		dDistqtk=rData.dDistqtk;
		nDirType=rData.nDirType;
		dRoadV=rData.dRoadV;
		nFactorType=rData.nFactorType;
		bUseBrkForce=rData.bUseBrkForce; 
		bUseTrkForce=rData.bUseTrkForce; 
		dLongiH     =rData.dLongiH;    
		dSuElv      =rData.dSuElv;
		dTotalLength = rData.dTotalLength;
	}
	void Convert895_test(T_BCVT_UDRD_D_CH_RW_895_test& rData)
	{
		key=rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType=rData.nType;
		nCountMovingLoadCase = rData.nCountMovingLoadCase;
		nCountNodeElemKey = rData.nCountNodeElemKey;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		nCompType=rData.nCompType;
		bUseHoriForce=rData.bUseHoriForce;
		nHoriType=rData.nHoriType;
		nCalcType=rData.nCalcType;
		dRailH=rData.dRailH;
		dRailV=rData.dRailV;
		dRailR=rData.dRailR;
		dRoadH=rData.dRoadH;
		dRoadR=rData.dRoadR;
		dPointQtk=rData.dPointQtk;
		dDistqtk=rData.dDistqtk;
		nDirType=rData.nDirType;
		dRoadV=rData.dRoadV;
		nFactorType=rData.nFactorType;
		bUseBrkForce=rData.bUseBrkForce; 
		bUseTrkForce=rData.bUseTrkForce; 
		dLongiH     =rData.dLongiH;    
		dSuElv=rData.dSuElv;
		dTotalLength = 0.0;
	}
	void Convert891(T_BCVT_UDRD_D_CH_RW_891& rData)
	{
		key=rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType=rData.nType;
		nCountMovingLoadCase = rData.nCountMovingLoadCase;
		nCountNodeElemKey = rData.nCountNodeElemKey;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		nCompType=rData.nCompType;
		bUseHoriForce=rData.bUseHoriForce;
		nHoriType=rData.nHoriType;
		nCalcType=rData.nCalcType;
		dRailH=rData.dRailH;
		dRailV=rData.dRailV;
		dRailR=rData.dRailR;
		dRoadH=rData.dRoadH;
		dRoadR=rData.dRoadR;
		dPointQtk=rData.dPointQtk;
		dDistqtk=rData.dDistqtk;
		nDirType=rData.nDirType;
		dRoadV=rData.dRoadV;
		nFactorType=rData.nFactorType;
		bUseBrkForce=rData.bUseBrkForce; 
		bUseTrkForce=rData.bUseTrkForce; 
		dLongiH     =rData.dLongiH;    
		dSuElv=0.0;
		dTotalLength = 0.0;
	}
	void Convert865(T_BCVT_UDRD_D_CH_RW_865& rData)
	{
		key=rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType=rData.nType;
		nCountMovingLoadCase = rData.nCountMovingLoadCase;
		nCountNodeElemKey = rData.nCountNodeElemKey;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		nCompType=rData.nCompType;
		bUseHoriForce=rData.bUseHoriForce;
		nHoriType=rData.nHoriType;
		nCalcType=rData.nCalcType;
		dRailH=rData.dRailH;
		dRailV=rData.dRailV;
		dRailR=rData.dRailR;
		dRoadH=rData.dRoadH;
		dRoadR=rData.dRoadR;
		dPointQtk=rData.dPointQtk;
		dDistqtk=rData.dDistqtk;
		nDirType=rData.nDirType;
		dRoadV=rData.dRoadV;
		nFactorType=0;
		bUseBrkForce=rData.bUseHoriForce;
		bUseTrkForce=FALSE;
		dLongiH=0.0;
		dSuElv=0.0;
		dTotalLength = 0.0;
	}
	void Convert861(T_BCVT_UDRD_D_CH_RW_861& rData)
	{
		key=rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType=rData.nType;
		nCountMovingLoadCase = rData.nCountMovingLoadCase;
		nCountNodeElemKey = rData.nCountNodeElemKey;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		nCompType=rData.nCompType;
		bUseHoriForce=rData.bUseHoriForce;
		nHoriType=rData.nHoriType;
		nCalcType=rData.nCalcType;
		dRailH=rData.dRailH;
		dRailV=rData.dRailV;
		dRailR=rData.dRailR;
		dRoadH=rData.dRoadH;
		dRoadR=rData.dRoadR;
		dPointQtk=rData.dPointQtk;
		dDistqtk=rData.dDistqtk;
		nDirType=rData.nDirType;
		dRoadV=0.0;
		nFactorType=0;
		bUseBrkForce=rData.bUseHoriForce;
		bUseTrkForce=FALSE;
		dLongiH=0.0;
		dSuElv=0.0;
		dTotalLength = 0.0;
	}
	void Convert825(T_BCVT_UDRD_D_CH_RW_825& rData)
	{
		key=rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType = rData.nType;
		nCountMovingLoadCase = rData.nCountMovingLoadCase;
		nCountNodeElemKey = rData.nCountNodeElemKey;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		nCompType=rData.nCompType;
		bUseHoriForce=FALSE;
		nHoriType=1;
		nCalcType=0;
		dRailH=0.0;
		dRailV=0.0;
		dRailR=0.0;
		dRoadH=0.0;
		dRoadR=0.0;
		dPointQtk=0.0;
		dDistqtk=0.0;
		nDirType=0;
		dRoadV=0.0;
		nFactorType=0;
		bUseBrkForce=FALSE;
		bUseTrkForce=FALSE;
		dLongiH=0.0;
		dSuElv=0.0;
		dTotalLength = 0.0;
	}
	void Convert815(T_BCVT_UDRD_D_CH_RW_815& rData)
	{
		key=rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType = rData.nType;
		nCountMovingLoadCase = rData.nCountMovingLoadCase;
		nCountNodeElemKey = rData.nCountNodeElemKey;
		memcpy(bParts, rData.bParts, sizeof(bParts));
		memcpy(bComponents, rData.bComponents, sizeof(bComponents));
		nCompType=0;
		bUseHoriForce=FALSE;
		nHoriType=1;
		nCalcType=0;
		dRailH=0.0;
		dRailV=0.0;
		dRailR=0.0;
		dRoadH=0.0;
		dRoadR=0.0;
		dRoadV=0.0;
		dPointQtk=0.0;
		dDistqtk=0.0;
		nDirType=0;
		dRoadV=0.0;
		nFactorType=0;
		bUseBrkForce=FALSE;
		bUseTrkForce=FALSE;
		dLongiH=0.0;
		dSuElv=0.0;
		dTotalLength = 0.0;
	}
};
struct T_BCVT_UNIT
{
	T_BCVT_UNIT()
	{
		dRailH=D_UNITSYS_BASE_LENGTH;
		dRailV=D_UNITSYS_BASE_LENGTH; // 속도 인데 단위가 /h 로 고정되어서 Length 로 Setting 한다.
		dRailR=D_UNITSYS_BASE_LENGTH;
		dRoadH=D_UNITSYS_BASE_LENGTH;
		dRoadR=D_UNITSYS_BASE_LENGTH;
		dRoadV=D_UNITSYS_BASE_LENGTH; // 속도 인데 단위가 /h 로 고정되어서 Length 로 Setting 한다.
		dPointQtk=D_UNITSYS_BASE_FORCE;
		dDistqtk=D_UNITSYS_BASE_UNITLOAD;
		dLongiH=D_UNITSYS_BASE_LENGTH;
	}
	int dRailH;
	int dRailV;
	int dRailR;
	int dRoadH;
	int dRoadR;
	int dRoadV;
	int dPointQtk;
	int dDistqtk;
	int dLongiH;
};
#define HASHSIZEBCVT 11

// Local Direction Force Sum
#define T_LDFS_K unsigned int
struct T_LDFS_COORD
{
	double x, y, z;
};
struct T_LDFS_D
{
	CString csName;
	int     nMode;      // 0:Polygon Select, 1:Line Select 
	unsigned int AnalType;    //D_LOADCASE_??? (Dialog에서 D_LOADCASE_???로 저장해서 변경함:04.06.25)
	unsigned int LoadCaseKey; 
	double dTolerance;
	BOOL bZVector;
	double dZVector[3];
	CArray<T_LDFS_COORD, T_LDFS_COORD&> arCoord;
	T_STAG_K StagKey; // 820 : 시공단계 추가.
	int      nStep;   // 820 : 시공단계 추가. 
	BOOL bBeam;       // 820   
	BOOL bPlate;      // 820      
	BOOL bSolid;      // 820 : nMode = 0 일 경우만 가능   
	BOOL bUseGrup;    // 820  
	T_GRUP_K GrupKey; // 820  
	int nLocationType; // 820 : Resultant Force Location 0=Auto, 1=User
	double dLocation[3]; // 820 : 사용자 입력 x, y, z
	int nMaxMin; // 820   
	T_LDFS_D()
	{
	}
	T_LDFS_D(T_LDFS_D& src)
	{
		*this = src;
	}
	T_LDFS_D& operator=(const T_LDFS_D& src)
	{
		csName = src.csName;
		nMode = src.nMode;
		AnalType = src.AnalType;
		LoadCaseKey = src.LoadCaseKey;
		dTolerance = src.dTolerance;
		bZVector = src.bZVector;
		memcpy(dZVector, src.dZVector, sizeof(dZVector));
		arCoord.Copy(src.arCoord);
		StagKey = src.StagKey;
		nStep = src.nStep;   
		bBeam = src.bBeam;
		bPlate = src.bPlate;
		bSolid = src.bSolid;
		bUseGrup = src.bUseGrup;
		GrupKey = src.GrupKey;
		nLocationType = src.nLocationType;
		memcpy(dLocation, src.dLocation, sizeof(dLocation)); 
		nMaxMin = src.nMaxMin;
		return *this;
	}
	void Initialize()
	{
		csName.Empty();
		nMode = 0;
		AnalType = 0;
		LoadCaseKey = 0;
		dTolerance = 0.0001;
		bZVector = FALSE;
		for (int i = 0; i < 3; i++) dZVector[i] = 0.0;
		arCoord.RemoveAll();
		StagKey = 0;
		nStep = 0;
		bBeam = TRUE;
		bPlate = TRUE;
		bSolid = TRUE;
		bUseGrup = FALSE;
		GrupKey = 0;
		nLocationType = 0;
		for (int i = 0; i < 3; i++) dLocation[i] = 0.0;
		nMaxMin = 0;
	}
};
struct T_LDFS_D_CH
{
	char csName[40];
	int     nMode;
	unsigned int AnalType;
	unsigned int LoadCaseKey; 
	double dTolerance;
	BOOL bZVector;
	double dZVector[3];
	CArray<T_LDFS_COORD, T_LDFS_COORD&> arCoord;
	T_STAG_K StagKey; // 820 : 시공단계 추가.
	int      nStep;   // 820 : 시공단계 추가. 
	BOOL bBeam;       // 820   
	BOOL bPlate;      // 820      
	BOOL bSolid;      // 820 : nMode = 0 일 경우만 가능   
	BOOL bUseGrup;    // 820  
	T_GRUP_K GrupKey; // 820  
	int nLocationType; // 820 
	double dLocation[3]; // 820 
	int nMaxMin; // 820 
	void ConvertToString(T_LDFS_D& rData)
	{
		ConvertCharStr(csName, rData.csName, sizeof(csName));
		rData.nMode = nMode;
		rData.AnalType = AnalType;
		rData.LoadCaseKey = LoadCaseKey;
		rData.dTolerance = dTolerance;
		rData.bZVector = bZVector;
		memcpy(rData.dZVector, dZVector, sizeof(dZVector));
		rData.arCoord.Copy(arCoord);
		rData.StagKey = StagKey;
		rData.nStep = nStep;
		rData.bBeam    = bBeam;
		rData.bPlate   = bPlate;
		rData.bSolid   = bSolid;
		rData.bUseGrup = bUseGrup;
		rData.GrupKey  = GrupKey;
		rData.nLocationType = nLocationType;
		memcpy(rData.dLocation, dLocation, sizeof(dLocation));    
		rData.nMaxMin = nMaxMin;
	}
	void ConvertToChar(T_LDFS_D& rData)
	{
		ConvertStrChar(rData.csName, csName, sizeof(csName));
		nMode = rData.nMode;
		AnalType = rData.AnalType;
		LoadCaseKey = rData.LoadCaseKey;
		dTolerance = rData.dTolerance;
		bZVector = rData.bZVector;
		memcpy(dZVector, rData.dZVector, sizeof(dZVector));
		arCoord.Copy(rData.arCoord);
		StagKey = rData.StagKey;
		nStep = rData.nStep;
		bBeam    = rData.bBeam;
		bPlate   = rData.bPlate;
		bSolid   = rData.bSolid;
		bUseGrup = rData.bUseGrup;
		GrupKey  = rData.GrupKey;
		nLocationType = rData.nLocationType;
		memcpy(dLocation, rData.dLocation, sizeof(dLocation));    
		nMaxMin = rData.nMaxMin;
	}  
};
struct T_LDFS_UDRD_D
{
	T_LDFS_K key;
	T_LDFS_D data;
};
struct T_LDFS_UDRD_D_CH
{
	T_LDFS_K key;
	T_LDFS_D_CH data;
};
struct T_LDFS_UDRD_D_CH_RW_815
{
	T_LDFS_K key;
	char csName[40];
	int     nMode;
	unsigned int AnalType;
	unsigned int LoadCaseKey; 
	double dTolerance;
	BOOL bZVector;
	double dZVector[3];
	int nCountCoord;
};
struct T_LDFS_UDRD_D_CH_RW
{
	T_LDFS_K key;
	char csName[40];
	int     nMode;
	unsigned int AnalType;
	unsigned int LoadCaseKey; 
	double dTolerance;
	BOOL bZVector;
	double dZVector[3];
	int nCountCoord;
	T_STAG_K StagKey; // 820 : 시공단계 추가.
	int      nStep;   // 820 : 시공단계 추가.  
	BOOL bBeam;       // 820   
	BOOL bPlate;      // 820      
	BOOL bSolid;      // 820 : nMode = 0 일 경우만 가능   
	BOOL bUseGrup;    // 820  
	T_GRUP_K GrupKey; // 820  
	int nLocationType; // 820 
	double dLocation[3]; // 820 
	int nMaxMin; // 820 
	void GetLdfs(T_LDFS_K& rKey, T_LDFS_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.csName, csName, sizeof(csName));
		rData.nMode = nMode;
		rData.AnalType = AnalType;
		rData.LoadCaseKey = LoadCaseKey;
		rData.dTolerance = dTolerance;
		rData.bZVector = bZVector;
		memcpy(rData.dZVector, dZVector, sizeof(dZVector));
		rData.StagKey = StagKey;   
		rData.nStep = nStep;
		rData.bBeam    = bBeam;
		rData.bPlate   = bPlate;
		rData.bSolid   = bSolid;
		rData.bUseGrup = bUseGrup;
		rData.GrupKey  = GrupKey;
		rData.nLocationType = nLocationType;
		memcpy(rData.dLocation, dLocation, sizeof(dLocation));    
		rData.nMaxMin = nMaxMin;
	}
	void SetLdfs(const T_LDFS_K Key, const T_LDFS_D_CH& rData)
	{
		key=Key;
		memcpy(csName, rData.csName, sizeof(csName));
		nMode = rData.nMode;
		AnalType = rData.AnalType;
		LoadCaseKey = rData.LoadCaseKey;
		dTolerance = rData.dTolerance;
		bZVector = rData.bZVector;
		memcpy(dZVector, rData.dZVector, sizeof(dZVector));
		nCountCoord = (int)rData.arCoord.GetSize();
		StagKey = rData.StagKey;
		nStep = rData.nStep;
		bBeam    = rData.bBeam;
		bPlate   = rData.bPlate;
		bSolid   = rData.bSolid;
		bUseGrup = rData.bUseGrup;
		GrupKey  = rData.GrupKey;
		nLocationType = rData.nLocationType;    
		memcpy(dLocation, rData.dLocation, sizeof(dLocation));   
		nMaxMin = rData.nMaxMin;      
	}
	void Convert815(const T_LDFS_UDRD_D_CH_RW_815& rData)
	{
		key = rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nMode = rData.nMode;
		AnalType = rData.AnalType;
		LoadCaseKey = rData.LoadCaseKey;
		dTolerance = rData.dTolerance;
		bZVector = rData.bZVector;
		memcpy(dZVector, rData.dZVector, sizeof(dZVector));
		nCountCoord = rData.nCountCoord;
		StagKey = 0;
		nStep = 0;
		bUseGrup = FALSE;
		GrupKey  = 0;    

		// 0:Solid Face Polyline, 1:Plate Edge Line, 2:Plate Edge Polyline
		// nMode가 변경되었으므로 여기서 바꿔준다.
		bBeam  = FALSE;
		bPlate = FALSE;
		bSolid = FALSE;
		if     (nMode == 0) bSolid = TRUE;
		else if(nMode == 1) bPlate = TRUE;
		else if(nMode == 2)
		{
			bPlate = TRUE;
			nMode = 0;
		}    
		nLocationType = 0;    
		for (int i = 0; i < 3; i++) dLocation[i] = 0.0;
		nMaxMin = 0;
	}
};
struct T_LDFS_UNIT
{
	T_LDFS_UNIT()
	{
		dTolerance = D_UNITSYS_BASE_LENGTH;
		dZVector = D_UNITSYS_BASE_LENGTH;
		dCoord = D_UNITSYS_BASE_LENGTH;
		dLocation = D_UNITSYS_BASE_LENGTH;
	}
	int dTolerance;
	int dZVector;
	int dCoord;
	int dLocation;
};
#define HASHSIZELDFS 11

// Force Summary Table
#define T_FSUM_K unsigned int
struct T_FSUM_D
{
	CString csName;  
	int     nType;              // 0:TypeI, 1:TypeII, 2:TypeIII
	int     nElemType;          // 2 :BEAM_EL, 4:PLATE_EL
	int     nMethod;            // 0:Abs, 1:MinMax (6.1.1에서 추가)
	CArray<UINT, UINT> aElem;
	CArray<int, int> aPart;     // 0:I, 2:1/4, 4:2/4, 6:3/4, 7:J
	CArray<UINT, UINT> aLoad;   // Load Combination or Load Key
	CArray<UINT, UINT> aLkind;  // m_aLoad, m_aLkind pair, load type
	CArray<int, int> aMinMax;   // -1:min, 0:normal, 1:max  
	CArray<int, int> aItem;     // 0:Axial, 1:Shear-y, 2:Shear-z, 3:Torsion, 4:Moment-y, 5:Moment-z  
	T_FSUM_D()
	{
	}
	T_FSUM_D(T_FSUM_D& src)
	{
		*this = src;
	}
	T_FSUM_D& operator=(const T_FSUM_D& src)
	{
		csName = src.csName;
		nType  = src.nType;
		nElemType = src.nElemType;
		nMethod = src.nMethod;
		aElem.Copy(src.aElem);
		aPart.Copy(src.aPart);
		aLoad.Copy(src.aLoad);
		aLkind.Copy(src.aLkind);
		aMinMax.Copy(src.aMinMax);        
		aItem.Copy(src.aItem);
		return *this;
	}
	void Initialize()
	{
		csName.Empty();
		nType = 0;
		nElemType = 2;
		nMethod = 0;
		aElem.RemoveAll();
		aPart.RemoveAll();
		aLoad.RemoveAll();
		aLkind.RemoveAll();
		aMinMax.RemoveAll();    
		aItem.RemoveAll();
	}
};
struct T_FSUM_D_CH
{
	char csName[40];
	int  nType;
	int  nElemType;   
	int  nMethod;
	CArray<UINT, UINT> aElem;
	CArray<int, int> aPart;     // 0:I, 2:1/4, 4:2/4, 6:3/4, 7:J
	CArray<UINT, UINT> aLoad;   // Load Combination or Load Key
	CArray<UINT, UINT> aLkind;  // m_aLoad, m_aLkind pair, load type
	CArray<int, int> aMinMax;   // -1:min, 0:normal, 1:max  
	CArray<int, int> aItem;     // 0:Axial, 1:Shear-y, 2:Shear-z, 3:Torsion, 4:Moment-y, 5:Moment-z  
	void ConvertToString(T_FSUM_D& rData)
	{
		ConvertCharStr(csName, rData.csName, sizeof(csName));
		rData.nType = nType;
		rData.nElemType = nElemType;
		rData.nMethod = nMethod;
		rData.aElem.Copy(aElem);
		rData.aPart.Copy(aPart);
		rData.aLoad.Copy(aLoad);
		rData.aLkind.Copy(aLkind);
		rData.aMinMax.Copy(aMinMax);    
		rData.aItem.Copy(aItem);    
	}
	void ConvertToChar(T_FSUM_D& rData)
	{
		ConvertStrChar(rData.csName, csName, sizeof(csName));
		nType = rData.nType;
		nElemType = rData.nElemType;
		nMethod = rData.nMethod;
		aElem.Copy(rData.aElem);
		aPart.Copy(rData.aPart);
		aLoad.Copy(rData.aLoad);
		aLkind.Copy(rData.aLkind);
		aMinMax.Copy(rData.aMinMax);
		aItem.Copy(rData.aItem);    
	}
};
struct T_FSUM_UDRD_D
{
	T_FSUM_K key;
	T_FSUM_D data;
};
struct T_FSUM_UDRD_D_CH
{
	T_FSUM_K key;
	T_FSUM_D_CH data;
};
struct T_FSUM_UDRD_D_CH_RW_610
{
	T_FSUM_K key;
	char csName[40];
	int nType;
	int nElemType; 
	int nCountElem;
	int nCountPart;
	int nCountLoad;
	int nCountLkind;
	int nCountMinMax;
	int nCountStep;
	int nCountItem;
};
struct T_FSUM_UDRD_D_CH_RW
{
	T_FSUM_K key;
	char csName[40];
	int nType;
	int nElemType; 
	int nMethod;
	int nCountElem;
	int nCountPart;
	int nCountLoad;
	int nCountLkind;
	int nCountMinMax;
	int nCountStep;
	int nCountItem;
	void GetFsum(T_FSUM_K& rKey, T_FSUM_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.csName, csName, sizeof(csName));
		rData.nType = nType;    
		rData.nElemType = nElemType;
		rData.nMethod = nMethod;
	}
	void SetFsum(const T_FSUM_K Key, const T_FSUM_D_CH& rData)
	{
		key=Key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType = rData.nType;
		nElemType = rData.nElemType;
		nMethod = rData.nMethod;
		nCountElem   = (int)rData.aElem.GetSize();
		nCountPart   = (int)rData.aPart.GetSize();
		nCountLoad   = (int)rData.aLoad.GetSize();
		nCountLkind  = (int)rData.aLkind.GetSize();
		nCountMinMax = (int)rData.aMinMax.GetSize();    
		nCountItem   = (int)rData.aItem.GetSize();
	}
	void Convert610(T_FSUM_UDRD_D_CH_RW_610& rData)
	{
		key=rData.key;
		memcpy(csName, rData.csName, sizeof(csName));
		nType = rData.nType;
		nElemType = rData.nElemType;
		nMethod = 0;  // v611
		nCountElem   = rData.nCountElem;
		nCountPart   = rData.nCountPart;
		nCountLoad   = rData.nCountLoad;
		nCountLkind  = rData.nCountLkind;
		nCountMinMax = rData.nCountMinMax;
		nCountItem   = rData.nCountItem;
	}
};
struct T_FSUM_UNIT
{
	T_FSUM_UNIT()
	{
		nType = D_UNITSYS_NONE;    
	}
	int nType;
};
#define HASHSIZEFSUM 11

// negative moment reduction
#define T_NMRD_K unsigned int // always 1
struct T_NMRD_NODE_BASE
{
	T_NODE_K NodeK;
	BOOL     bAuto;   // 폭 자동 계산이면 TRUE
	double   dWidth;
};
struct T_NMRD_CASE_BASE
{
	UINT     nLoadType; // D_LCOM_???
	UINT     LoadCaseK;
};
struct T_NMRD_D
{
	T_GRUP_K GrupK;
	CArray<T_NMRD_NODE_BASE, T_NMRD_NODE_BASE&> aNodeInfo;
	CArray<T_NMRD_CASE_BASE, T_NMRD_CASE_BASE> aCaseInfo;
	void Initialize()
	{
		GrupK = 0;
		aNodeInfo.RemoveAll();
		aCaseInfo.RemoveAll();
	}
	T_NMRD_D()  { }
	T_NMRD_D(T_NMRD_D& src) {*this = src;}
	T_NMRD_D& operator=(const T_NMRD_D& src)
	{
		GrupK = src.GrupK;
		aNodeInfo.Copy(src.aNodeInfo);    
		aCaseInfo.Copy(src.aCaseInfo);    
		return *this;
	}  
};
struct T_NMRD_UDRD_D
{  
	T_NMRD_D data;
};
struct T_NMRD_UDRD_D_RW
{  
	BOOL GrupK;
	unsigned int nCountNodeInfo;  
	unsigned int nCountCaseInfo;  
	void GetNmrd(T_NMRD_D& rData)
	{
		rData.GrupK = GrupK;
	}
	void SetNmrd(const T_NMRD_D& rData)
	{
		GrupK = rData.GrupK;
		nCountNodeInfo = (int)rData.aNodeInfo.GetSize();    
		nCountCaseInfo = (int)rData.aCaseInfo.GetSize();    
	}  
};
struct T_NMRD_UNIT
{
	T_NMRD_UNIT()
	{
		dWidth = D_UNITSYS_BASE_LENGTH;
	}
	int dWidth;
};
#define HASHSIZENMRD 1

// 설계용 단면력
#define T_IPFO_K unsigned int
struct T_IPFO_D
{
	CString strPathName;
	BOOL bImport;
	void Initialize()
	{
		strPathName.Empty();
		bImport = FALSE;
	}
};
struct T_IPFO_D_CH
{
	char strPathName[512];
	BOOL bImport;
	void ConvertToString(T_IPFO_D& rData)
	{
		ConvertCharStr(strPathName, rData.strPathName, sizeof(strPathName));
		rData.bImport=bImport;
	}
	void ConvertToChar(T_IPFO_D& rData)
	{
		ConvertStrChar(rData.strPathName, strPathName, sizeof(strPathName));
		bImport=rData.bImport;
	}
};
struct T_IPFO_UDRD_D
{
	T_IPFO_K key;
	T_IPFO_D data;
};
struct T_IPFO_UDRD_D_CH
{
	T_IPFO_K key;
	T_IPFO_D_CH data;
};
struct T_IPFO_UNIT
{
	T_IPFO_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEIPFO 1


// Girder-Stress Diagram Batch Group
#define T_GSBG_K unsigned int
struct T_GSBG_D
{
	CString  strName        ;
	BOOL     bIsBatch       ;
	T_KEY    bodyelem_grup_k;
	BOOL     bAllStage      ;
	BOOL     bReport        ;
	int      nBStrsComp     ;
	int      nBStrsCompSub  ;
	int      nMomentComp    ;
	int      n7thDOFType    ;
	int      nDgrmType      ; // 0:Stress, 1:Force/Moment
	double   dScale         ;
	COLORREF RGBColor       ;

	T_GSBG_D()
	{
	}
	T_GSBG_D(T_GSBG_D& src)
	{
		*this = src;
	}
	T_GSBG_D& operator=(const T_GSBG_D& src)
	{
		if(this==&src) return *this; 
		strName         = src.strName        ;
		bIsBatch        = src.bIsBatch       ;
		bodyelem_grup_k = src.bodyelem_grup_k;
		bAllStage       = src.bAllStage      ;
		bReport         = src.bReport        ;
		nBStrsComp      = src.nBStrsComp     ;
		nBStrsCompSub   = src.nBStrsCompSub  ;
		nMomentComp     = src.nMomentComp    ;
		n7thDOFType     = src.n7thDOFType    ;
		nDgrmType       = src.nDgrmType      ;
		dScale          = src.dScale         ;
		RGBColor        = src.RGBColor       ;
		return *this;
	}
	void Initialize()
	{
		strName         = "";
		bIsBatch        = FALSE;
		bodyelem_grup_k = 0;
		bAllStage       = FALSE;
		bReport         = FALSE;
		nBStrsComp      = 0;
		nBStrsCompSub   = 0;
		nMomentComp     = 0;
		n7thDOFType     = 0;
		nDgrmType       = 0;
		dScale          = 1;
		RGBColor        = RGB(0, 0, 0); // Black
	}
};
struct T_GSBG_D_CH
{
	char     strName[60]    ;
	BOOL     bIsBatch       ;
	T_KEY    bodyelem_grup_k;
	BOOL     bAllStage      ;
	BOOL     bReport        ;
	int      nBStrsComp     ;
	int      nBStrsCompSub  ;
	int      nMomentComp    ;
	int      n7thDOFType    ;
	int      nDgrmType      ; // 0:Stress, 1:Force/Moment
	double   dScale         ;
	COLORREF RGBColor       ;

	void ConvertToString(T_GSBG_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.bIsBatch        = bIsBatch       ;
		rData.bodyelem_grup_k = bodyelem_grup_k;
		rData.bAllStage       = bAllStage      ;
		rData.bReport         = bReport        ;
		rData.nBStrsComp      = nBStrsComp     ;
		rData.nBStrsCompSub   = nBStrsCompSub  ;
		rData.nMomentComp     = nMomentComp    ;
		rData.n7thDOFType     = n7thDOFType    ;
		rData.nDgrmType       = nDgrmType      ;
		rData.dScale          = dScale         ;
		rData.RGBColor        = RGBColor       ;
	}
	void ConvertToChar(T_GSBG_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		bIsBatch        = rData.bIsBatch       ;
		bodyelem_grup_k = rData.bodyelem_grup_k;
		bAllStage       = rData.bAllStage      ;
		bReport         = rData.bReport        ;
		nBStrsComp      = rData.nBStrsComp     ;
		nBStrsCompSub   = rData.nBStrsCompSub  ;
		nMomentComp     = rData.nMomentComp    ;
		n7thDOFType     = rData.n7thDOFType    ;
		nDgrmType       = rData.nDgrmType      ;
		dScale          = rData.dScale         ;
		RGBColor        = rData.RGBColor       ;
	}
};
struct T_GSBG_UDRD_D
{
	T_GSBG_K key;
	T_GSBG_D data;
};
struct T_GSBG_UDRD_D_CH
{
	T_GSBG_K key;
	T_GSBG_D_CH data;
};
struct T_GSBG_UDRD_D_CH_RW
{
	T_GSBG_K key;
	char     strName[60]    ;
	BOOL     bIsBatch       ;
	T_KEY    bodyelem_grup_k;
	BOOL     bAllStage      ;
	BOOL     bReport        ;
	int      nBStrsComp     ;
	int      nBStrsCompSub  ;
	int      nMomentComp    ;
	int      n7thDOFType    ;
	int      nDgrmType      ; // 0:Stress, 1:Force/Moment
	double   dScale         ;
	COLORREF RGBColor       ;

	void GetGsbg(T_GSBG_K& rKey, T_GSBG_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.bIsBatch        = bIsBatch       ;
		rData.bodyelem_grup_k = bodyelem_grup_k;
		rData.bAllStage       = bAllStage      ;
		rData.bReport         = bReport        ;
		rData.nBStrsComp      = nBStrsComp     ;
		rData.nBStrsCompSub   = nBStrsCompSub  ;
		rData.nMomentComp     = nMomentComp    ;
		rData.n7thDOFType     = n7thDOFType    ;
		rData.nDgrmType       = nDgrmType      ;
		rData.dScale          = dScale         ;
		rData.RGBColor        = RGBColor       ;
	}
	void SetGsbg(const T_GSBG_K Key, const T_GSBG_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		bIsBatch        = rData.bIsBatch       ;
		bodyelem_grup_k = rData.bodyelem_grup_k;
		bAllStage       = rData.bAllStage      ;
		bReport         = rData.bReport        ;
		nBStrsComp      = rData.nBStrsComp     ;
		nBStrsCompSub   = rData.nBStrsCompSub  ;
		nMomentComp     = rData.nMomentComp    ;
		n7thDOFType     = rData.n7thDOFType    ;
		nDgrmType       = rData.nDgrmType      ;
		dScale          = rData.dScale         ;
		RGBColor        = rData.RGBColor       ;  
	}
};
struct T_GSBG_UNIT
{
	T_GSBG_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEGSBG 11
// Dynamic Report - Unit of Special Tables
struct T_UNSP_BASE
{
	int   nUnitSys; // 0:Current Unit System, 1:Defined Unit System
	BOOL  bApplyAll;
	T_UNIT_INDEX UnitIdx;

	void Initialize()
	{
		nUnitSys = 0;
		bApplyAll = FALSE;
		UnitIdx.nBase_Length  = 0;
		UnitIdx.nBase_Force   = 0;
		UnitIdx.nBase_Heat    = 0;
		UnitIdx.nBase_Temper  = 0;
	}

	T_UNSP_BASE& operator=(const T_UNSP_BASE& src)
	{
		if(this==&src) return *this; 
		nUnitSys              = src.nUnitSys            ;
		bApplyAll             = src.bApplyAll           ;
		UnitIdx.nBase_Length  = src.UnitIdx.nBase_Length;
		UnitIdx.nBase_Force   = src.UnitIdx.nBase_Force ;
		UnitIdx.nBase_Heat    = src.UnitIdx.nBase_Heat  ;
		UnitIdx.nBase_Temper  = src.UnitIdx.nBase_Temper;
		return *this;
	}
};

#define T_UNSP_K unsigned int
struct T_UNSP_D
{
	T_UNSP_BASE SectSumm; // Section Summary
	T_UNSP_BASE ReinForc; // Reinforcement of Sections
	T_UNSP_BASE CompSect; // Composite Section for C.S.

	T_UNSP_D()
	{
	}
	T_UNSP_D(T_UNSP_D& src)
	{
		*this = src;
	}
	T_UNSP_D& operator=(const T_UNSP_D& src)
	{
		if(this==&src) return *this; 
		SectSumm = src.SectSumm;
		ReinForc = src.ReinForc;
		CompSect = src.CompSect;
		return *this;
	}
	void Initialize()
	{
		SectSumm.Initialize();
		ReinForc.Initialize();
		CompSect.Initialize();
	}
};
struct T_UNSP_UDRD_D
{
	T_UNSP_K key;
	T_UNSP_D data;
};
// String이 없어서 좀 간소하게 받음
struct T_UNSP_UDRD_D_RW
{
	T_UNSP_K key;
	T_UNSP_BASE SectSumm; // Section Summary
	T_UNSP_BASE ReinForc; // Reinforcement of Sections
	T_UNSP_BASE CompSect; // Composite Section for C.S.

	void GetUnsp(T_UNSP_K& rKey, T_UNSP_D& rData)
	{
		rKey=key;
		rData.SectSumm = SectSumm;
		rData.ReinForc = ReinForc;
		rData.CompSect = CompSect;
	}
	void SetUnsp(const T_UNSP_K rKey, const T_UNSP_D& rData)
	{
		key      = rKey;
		SectSumm = rData.SectSumm;
		ReinForc = rData.ReinForc;
		CompSect = rData.CompSect;  
	}
};
struct T_UNSP_UNIT
{
	T_UNSP_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEUNSP 11
#pragma pack(pop)

#endif  // __DB_ST_DT_POST_H__
