#ifndef __DB_ST_DT_TRLN_H__
#define __DB_ST_DT_TRLN_H__

#define MIDAS_SDK

#pragma pack(push, 1)

enum E_TDATA_LAYER { _TDATA_LAYER_CENTER = 0, _TDATA_LAYER_COLUMN, _TDATA_LAYER_WALL, _TDATA_LAYER_DIM, _TDATA_LAYER_BUBBLE, _TDATA_LAYER_UNDEFINED, _TDATA_LAYER_NUM, _TDATA_LAYER_REFLINE };
enum E_TDATA_GEOMTYPE { _TDATA_GEOMTYPE_LINE, _TDATA_GEOMTYPE_ARC, _TDATA_GEOMTYPE_CIRCLE, _TDATA_GEOMTYPE_ELLIPSE, _TDATA_GEOMTYPE_TEXT, _TDATA_GEOMTYPE_NUM };

struct T_TDATALineInfo
{
	long  lLinePattern;
	float fLineWidth;
	COLORREF colorValue;
	long alphaValue;
	E_TDATA_LAYER eLayer;
	T_TDATALineInfo()
	{
		init();
	}
	void init()
	{
		fLineWidth = 1.0f;
		lLinePattern = 0;
		alphaValue = 255;
		colorValue = RGB(128, 128, 128);
		eLayer = _TDATA_LAYER_CENTER;
	}
	T_TDATALineInfo& operator=(const T_TDATALineInfo& rData)
	{
		lLinePattern = rData.lLinePattern;
		fLineWidth = rData.fLineWidth;
		colorValue = rData.colorValue;
		alphaValue = rData.alphaValue;
		eLayer = rData.eLayer;
		return *this;
	}
};

#define T_TDAT_K unsigned int
#define HASHSIZETDAT    10001
struct T_TDAT_D
{
	T_TDATALineInfo	tDataLineInfo;

	// Geom Info
	E_TDATA_GEOMTYPE eGeomType;
	MArrayVec3d	maPos;		// Position
	MArrayd		maVal;		// Radius, Distance...
	MArrayVec3d	maVec;		// Vector Only. ( ex. Normal )
	CString	maText;
	void Initialize()
	{
		eGeomType = _TDATA_GEOMTYPE_LINE;
		maPos.RemoveAll();
		maVal.RemoveAll();
		maVec.RemoveAll();
		maText.Empty();
	}
	BOOL IsValid()
	{
		switch (eGeomType)
		{
		case _TDATA_GEOMTYPE_LINE:
			return (maPos.GetSize() == 2 && maVal.GetSize() == 0 && maVec.GetSize() == 0);
		case _TDATA_GEOMTYPE_ARC:
			return (maPos.GetSize() == 2 && maVal.GetSize() == 0 && maVec.GetSize() == 2);
		case _TDATA_GEOMTYPE_CIRCLE:
			return (maPos.GetSize() == 1 && maVal.GetSize() == 1 && maVec.GetSize() == 2);
		case _TDATA_GEOMTYPE_ELLIPSE:
			return (maPos.GetSize() == 1 && maVal.GetSize() == 2 && maVec.GetSize() == 2);
		case _TDATA_GEOMTYPE_TEXT:
			return (maPos.GetSize() == 2 && maVal.GetSize() == 1 && maVec.GetSize() == 1);
		default:
			break;
		}
		return FALSE;
	}
	T_TDAT_D& operator=(const T_TDAT_D& src)
	{
		tDataLineInfo = src.tDataLineInfo;
		eGeomType = src.eGeomType;
		maPos = src.maPos;
		maVal = src.maVal;
		maVec = src.maVec;
		maText = src.maText;
		return *this;
	}
};
struct T_TDAT_D_CH
{
	T_TDATALineInfo	tDataLineInfo;

	// Geom Info
	E_TDATA_GEOMTYPE eGeomType;
	MArrayVec3d	maPos;		// Position
	MArrayd		maVal;		// Radius, Distance...
	MArrayVec3d	maVec;		// Vector Only. ( ex. Normal )
	char maText[40];
	
	void ConvertToString(T_TDAT_D& rData)
	{
		rData.tDataLineInfo = tDataLineInfo;
		rData.eGeomType = eGeomType;
		rData.maPos.Copy(maPos);
		rData.maVal.Copy(maVal);
		rData.maVec.Copy(maVec);
		rData.maText = maText;

		ConvertCharStr(maText, rData.maText, sizeof(maText));

	}
	void ConvertToChar(T_TDAT_D& rData)
	{
		tDataLineInfo = rData.tDataLineInfo;
		eGeomType = rData.eGeomType;
		maPos.Copy(rData.maPos);
		maVal.Copy(rData.maVal);
		maVec.Copy(rData.maVec);
		ConvertStrChar(rData.maText, maText, sizeof(maText));
	}
};

struct T_TDAT_UDRD_D
{
	T_TDAT_K key;
	T_TDAT_D data;
};
struct T_TDAT_UDRD_D_CH
{
	T_TDAT_K key;
	T_TDAT_D_CH data;
};

struct T_TDAT_UDRD_D_CH_RW
{
	T_TDAT_K key;
	T_TDATALineInfo	tDataLineInfo;

	// Geom Info
	E_TDATA_GEOMTYPE eGeomType;
	int nCountPos;
	int nCountVal;
	int nCountVec;

	char maText[40];
	

	void GetTdat(T_TDAT_K& rKey, T_TDAT_D_CH& rData)
	{
		rKey = key;
		rData.tDataLineInfo = tDataLineInfo;
		rData.eGeomType = eGeomType;
		rData.maPos.SetSize(nCountPos);
		rData.maVal.SetSize(nCountVal);
		rData.maVec.SetSize(nCountVec);
		memcpy(rData.maText, maText, sizeof(maText));
	}
	void SetTdat(const T_TDAT_K Key, const T_TDAT_D_CH& rData)
	{
		key = Key;
		tDataLineInfo = rData.tDataLineInfo;
		eGeomType = rData.eGeomType;
		nCountPos = rData.maPos.GetSize();
		nCountVal = rData.maVal.GetSize();
		nCountVec = rData.maVec.GetSize();
		memcpy(maText, rData.maText, sizeof(maText));
	}
};

struct T_TDAT_UNIT
{
	int unitPos;
	int unitVal;
	int unitVec;

	T_TDAT_UNIT()
	{
		unitPos = D_UNITSYS_BASE_LENGTH;
		unitVal = D_UNITSYS_BASE_LENGTH;
		unitVec = D_UNITSYS_BASE_LENGTH;
	}
};

struct TAxis
{
	CString AxisName;
	MVector3d vStart;
	MVector3d vEnd;
	double dBulge;            //추후 곡선 중심선 지원시
	BOOL bCurved;			  //곡선이면 true
	TAxis()
	{
		memset(vStart.vec, 0, sizeof(double) * 3);
		memset(vEnd.vec, 0, sizeof(double) * 3);
		dBulge = 0;
		bCurved = FALSE;

	}
};

struct TBubble
{
	CString strBubble;
	MVector3d vCenter;
	MVector3d vRef;
	T_KEY AxisKey;
	TBubble()
	{
		memset(vCenter.vec, 0, sizeof(double) * 3);
		memset(vRef.vec, 0, sizeof(double) * 3);
		AxisKey = 0;
	}
};

struct TAxisDim
{
	MVector3d	vStart;
	MVector3d	vEnd;
	MVector3d	vRef;
	int			nDimType;               //1. Aligned, 2. Rotated, 3. Angular, 4. Diameter, 5. Radial
	CString		strDim;					//원본도면에 치수선의 사용자값이 설정되어 있을 때
	double		dStrHeight;
	double		dStrAngle;
	double		dAngle;					//원본도면에 값이 설정되어 있을 때(넣지 않아도 됨)
	int			nTextAligned;			//텍스트 방향 옵션
	double		dTextCalulatedRotate;	//옵션 적용 되서 계산된 각도 nGen 에서만 사용
	TAxisDim()
	{
		memset(vStart.vec, 0, sizeof(double) * 3);
		memset(vEnd.vec, 0, sizeof(double) * 3);
		memset(vRef.vec, 0, sizeof(double) * 3);
		dStrHeight = 0;
		dStrAngle = 0;
		nDimType = 0;
		dAngle = 0;
		nTextAligned = 0;
		dTextCalulatedRotate = 0;
	}
};

struct TAxis_CH
{
	char AxisName[40];
	MVector3d vStart;
	MVector3d vEnd;
	double dBulge;            //추후 곡선 중심선 지원시
	BOOL bCurved;			  //곡선이면 true
	void ConvertToString(TAxis& rData)
	{
		ConvertCharStr(AxisName, rData.AxisName, sizeof(AxisName));
		rData.vStart = vStart;
		rData.vEnd = vEnd;
		rData.dBulge = dBulge;
		rData.bCurved = bCurved;
	}
	void ConvertToChar(TAxis& rData)
	{
		ConvertStrChar(rData.AxisName, AxisName, sizeof(AxisName));
		vStart = rData.vStart;
		vEnd = rData.vEnd;
		dBulge = rData.dBulge;
		bCurved = rData.bCurved;
	}
};
struct TBubble_CH
{
	char strBubble[40];
	MVector3d vCenter;
	MVector3d vRef;
	T_KEY AxisKey;
	void ConvertToString(TBubble& rData)
	{
		ConvertCharStr(strBubble, rData.strBubble, sizeof(strBubble));
		rData.vCenter = vCenter;
		rData.vRef = vRef;
		rData.AxisKey = AxisKey;
	}
	void ConvertToChar(TBubble& rData)
	{
		ConvertStrChar(rData.strBubble, strBubble, sizeof(strBubble));
		vCenter = rData.vCenter;
		vRef = rData.vRef;
		AxisKey = rData.AxisKey;
	}
};

struct TAxisDim_CH
{
	MVector3d	vStart;
	MVector3d	vEnd;
	MVector3d	vRef;
	int			nDimType;               //1. Aligned, 2. Rotated, 3. Angular, 4. Diameter, 5. Radial
	char		strDim[40];					//원본도면에 치수선의 사용자값이 설정되어 있을 때
	double		dStrHeight;
	double		dStrAngle;
	double		dAngle;					//원본도면에 값이 설정되어 있을 때(넣지 않아도 됨)
	int			nTextAligned;			//텍스트 방향 옵션
	double		dTextCalulatedRotate;	//옵션 적용 되서 계산된 각도 nGen 에서만 사용
	void ConvertToString(TAxisDim& rData)
	{
		rData.vStart = vStart;
		rData.vEnd = vEnd;
		rData.vRef = vRef;
		rData.nDimType = nDimType;
		ConvertCharStr(strDim, rData.strDim, sizeof(strDim));
		rData.dStrHeight = dStrHeight;
		rData.dStrAngle = dStrAngle;
		rData.dAngle = dAngle;
		rData.nTextAligned = nTextAligned;
		rData.dTextCalulatedRotate = dTextCalulatedRotate;
	}
	void ConvertToChar(TAxisDim& rData)
	{
		vStart = rData.vStart;
		vEnd = rData.vEnd;
		vRef = rData.vRef;
		nDimType = rData.nDimType;
		ConvertStrChar(rData.strDim, strDim, sizeof(strDim));
		dStrHeight = rData.dStrHeight;
		dStrAngle = rData.dStrAngle;
		dAngle = rData.dAngle;
		nTextAligned = rData.nTextAligned;
		dTextCalulatedRotate = rData.dTextCalulatedRotate;
	}
};

#define T_TDAS_K unsigned int
#define HASHSIZETDAS    1001
struct T_TDAS_D
{
	CString	strName;

	MArrayui        	arItrTData;		// Tracing Set에 속한 Tracing Data 들
	MArrayui			arItrTDataText;		// Tracing Set에 속한 Tracing Data Text들
	MArray < TAxis >	arTAxis;
	MArray < TBubble >	arTBubble;
	MArray < TAxisDim > arTAxisDim;

	void Initialize()
	{
		strName = _T("");
		arItrTData.RemoveAll();
		arItrTDataText.RemoveAll();
		arTAxis.RemoveAll();
		arTBubble.RemoveAll();
		arTAxisDim.RemoveAll();
	}
	
	T_TDAS_D& operator=(const T_TDAS_D& src)
	{
		strName = src.strName;
		arItrTData = src.arItrTData;
		arItrTDataText = src.arItrTDataText;
		arTAxis = src.arTAxis;
		arTBubble = src.arTBubble;
		arTAxisDim = src.arTAxisDim;
		
		return *this;
	}
};
struct T_TDAS_D_CH
{
	char	strName[40];

	MArrayui         	arItrTData;		// Tracing Set에 속한 Tracing Data 들
	MArrayui			arItrTDataText;		// Tracing Set에 속한 Tracing Data Text들
	MArray < TAxis_CH >	arTAxis;
	MArray < TBubble_CH >	arTBubble;
	MArray < TAxisDim_CH > arTAxisDim;

	void ConvertToString(T_TDAS_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.arItrTData.Copy(arItrTData);
		rData.arItrTDataText.Copy(arItrTDataText);
		rData.arTAxis.SetSize(arTAxis.GetSize());
		rData.arTBubble.SetSize(arTBubble.GetSize());
		rData.arTAxisDim.SetSize(arTAxisDim.GetSize());
		for (int i = 0; i < arTAxis.GetSize(); i++)
		{
			arTAxis[i].ConvertToString(rData.arTAxis[i]);
		}
		for (int i = 0; i < arTBubble.GetSize(); i++)
		{
			arTBubble[i].ConvertToString(rData.arTBubble[i]);
		}
		for (int i = 0; i < arTAxisDim.GetSize(); i++)
		{
			arTAxisDim[i].ConvertToString(rData.arTAxisDim[i]);
		}

	}
	void ConvertToChar(T_TDAS_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		arItrTData.Copy(rData.arItrTData);
		arItrTDataText.Copy(rData.arItrTDataText);
		arTAxis.SetSize(rData.arTAxis.GetSize());
		arTBubble.SetSize(rData.arTBubble.GetSize());
		arTAxisDim.SetSize(rData.arTAxisDim.GetSize());
		for (int i = 0; i < arTAxis.GetSize(); i++)
		{
			arTAxis[i].ConvertToChar(rData.arTAxis[i]);
		}
		for (int i = 0; i < arTBubble.GetSize(); i++)
		{
			arTBubble[i].ConvertToChar(rData.arTBubble[i]);
		}
		for (int i = 0; i < arTAxisDim.GetSize(); i++)
		{
			arTAxisDim[i].ConvertToChar(rData.arTAxisDim[i]);
		}
	}
};

struct T_TDAS_UDRD_D
{
	T_TDAS_K key;
	T_TDAS_D data;
};
struct T_TDAS_UDRD_D_CH
{
	T_TDAS_K key;
	T_TDAS_D_CH data;
};

struct T_TDAS_UDRD_D_CH_RW
{
	T_TDAS_K key;
	char	strName[40];
	int nCountTData;		// Tracing Set에 속한 Tracing Data 들
	int nCountTDataText;	//	MTArrayITR		arItrTDataText;		// Tracing Set에 속한 Tracing Data Text들
	int nCountTAxis;
	int nCountTBubble;
	int nCountTAxisDim;

	void GetTdas(T_TDAS_K& rKey, T_TDAS_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.arItrTData.SetSize(nCountTData);
		rData.arItrTDataText.SetSize(nCountTDataText);
		rData.arTAxis.SetSize(nCountTAxis);
		rData.arTBubble.SetSize(nCountTBubble);
		rData.arTAxisDim.SetSize(nCountTAxisDim);
	}
	void SetTdas(const T_TDAS_K Key, const T_TDAS_D_CH& rData)
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));
		nCountTData = rData.arItrTData.GetSize();
		nCountTDataText = rData.arItrTDataText.GetSize();
		nCountTAxis = rData.arTAxis.GetSize();
		nCountTBubble = rData.arTBubble.GetSize();
		nCountTAxisDim = rData.arTAxisDim.GetSize();
	}
};

#pragma pack(pop)

#endif  // __DB_ST_DT_TRLN_H__
