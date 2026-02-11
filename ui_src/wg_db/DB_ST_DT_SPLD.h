#ifndef __DB_ST_DT_SPLD_H__
#define __DB_ST_DT_SPLD_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_IFCT_K unsigned int
struct T_IFCT_D
{
	BOOL bSaveToLoadCase;
	T_STLD_K LoadCaseKey;
	void Initialize()
	{
		bSaveToLoadCase=FALSE;
		LoadCaseKey=0;
	}
};
struct T_IFCT_UDRD_D
{
	T_IFCT_D data;
};
struct T_IFCT_UNIT
{
	T_IFCT_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		bSaveToLoadCase=D_UNITSYS_NONE;
	}
	int bSaveToLoadCase;
	int LoadCaseKey;
};
#define HASHSIZEIFCT 1

#define T_IFGS_K T_ELEM_K
struct T_IFGS_D
{
	int nDirection; // 0=Axial, 1=GX Proj. 2=GY Proj. 3=GZ Proj.
	double InitForce;       // F
	void Initialize()
	{
		nDirection=0;
		InitForce=0;
	}
};
struct T_IFGS_UDRD_D
{
	T_IFGS_K key;
	T_IFGS_D data;
};
struct T_IFGS_UNIT
{
	T_IFGS_UNIT()
	{
		nDirection=D_UNITSYS_NONE;
		InitForce=D_UNITSYS_BASE_FORCE;
	}
	int nDirection;
	int InitForce;
};
#define HASHSIZEIFGS 1001

// Settlement Analysis Control Data
#define T_SMCT_K unsigned int
struct T_SMCT_D_880
{
	BOOL bConcurrentCalc; // Plate Concurrent Force Calculation
};
struct T_SMCT_D
{
	BOOL bConcurrentCalc; // Plate Concurrent Force Calculation
	BOOL bConcurrentLink; // Elastic/General Links Concurrent Force

	void Initialize()
	{
		bConcurrentCalc = TRUE;
		bConcurrentLink = TRUE;
	}	

	void Convert880(T_SMCT_D_880& rData)
	{
		bConcurrentCalc = rData.bConcurrentCalc;
		bConcurrentLink = FALSE;
	}
};

struct T_SMCT_UDRD_D_880
{
	T_SMCT_D_880 data;
};

struct T_SMCT_UDRD_D
{
	T_SMCT_D data;
};

struct T_SMCT_UNIT
{
	T_SMCT_UNIT()
	{
		// Nothing
	}
};
#define HASHSIZESMCT 1

#define T_SMPT_DT CArray<T_NODE_K, T_NODE_K>
#define T_SMPT_K unsigned int
struct T_SMPT_D
{
	CString GroupName;
	double dSettle;
	T_SMPT_DT aNodeData;
	unsigned int GroupId;
	void Initialize()
	{
		GroupName.Empty();
		dSettle=0.;
		aNodeData.RemoveAll();
		GroupId=0;
	}
	T_SMPT_D()
	{
	}
	T_SMPT_D(T_SMPT_D& src)
	{
		*this = src;
	}
	T_SMPT_D& operator=(T_SMPT_D& src)
	{
		GroupName=src.GroupName;
		dSettle=src.dSettle;
		aNodeData.Copy(src.aNodeData);
		GroupId=src.GroupId;
		return *this;
	}
};
struct T_SMPT_D_CH
{
	char GroupName[20];
	double dSettle;
	T_SMPT_DT aNodeData;
	unsigned int GroupId;
	void ConvertToString(T_SMPT_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.dSettle=dSettle;
		rData.aNodeData.Copy(aNodeData);
		rData.GroupId=GroupId;
	}
	void ConvertToChar(T_SMPT_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		dSettle=rData.dSettle;
		aNodeData.Copy(rData.aNodeData);
		GroupId=rData.GroupId;
	}
};
struct T_SMPT_UDRD_D
{
	T_SMPT_K key;
	T_SMPT_D data;
};
struct T_SMPT_UDRD_D_CH
{
	T_SMPT_K key;
	T_SMPT_D_CH data;
};
struct T_SMPT_UDRD_D_CH_RW
{
	T_SMPT_K key;
	char GroupName[20];
	double dSettle;
	unsigned int GroupId;
	int nCountNodeData;
	void GetSmpt(T_SMPT_K& rKey, T_SMPT_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.GroupName, GroupName, sizeof(GroupName));
		rData.dSettle=dSettle;
		rData.GroupId=GroupId;
	}
	void SetSmpt(const T_SMPT_K Key, const T_SMPT_D_CH& rData)
	{
		key=Key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		dSettle=rData.dSettle;
		GroupId=rData.GroupId;
		nCountNodeData=(int)rData.aNodeData.GetSize();
	}
};
struct T_SMPT_UNIT
{
	T_SMPT_UNIT()
	{
		GroupName=D_UNITSYS_NONE;
		dSettle=D_UNITSYS_BASE_LENGTH;
		GroupId=D_UNITSYS_NONE;;
	}
	int GroupName;
	int dSettle;
	int GroupId;
};
#define HASHSIZESMPT 11
#define D_SMPT_MINNUM       1   // 입력가능한 최소 지점침하그룹
#define D_SMPT_MAXNUM      50   // 입력가능한 최대 지점침하그룹

#define T_SMLC_DT CArray<T_SMPT_K, T_SMPT_K>
#define T_SMLC_K unsigned int
struct T_SMLC_D
{
	CString LoadCaseName;
	CString Description;
	double dFactor;
	int nStMin;
	int nStMax;
	T_SMLC_DT aPointData;
	unsigned int LoadCaseId;
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dFactor=0.;
		nStMin=0;
		nStMax=0;
		aPointData.RemoveAll();
		LoadCaseId=0;
	}
	T_SMLC_D()
	{
	}
	T_SMLC_D(T_SMLC_D& src)
	{
		*this = src;
	}
	T_SMLC_D& operator=(T_SMLC_D& src)
	{
		LoadCaseName=src.LoadCaseName;
		Description=src.Description;
		dFactor=src.dFactor;
		nStMin=src.nStMin;
		nStMax=src.nStMax;
		aPointData.Copy(src.aPointData);
		LoadCaseId=src.LoadCaseId;
		return *this;
	}
};
struct T_SMLC_D_CH
{
	char LoadCaseName[20];
	char Description[80];
	double dFactor;
	int nStMin;
	int nStMax;
	T_SMLC_DT aPointData;
	unsigned int LoadCaseId;
	void ConvertToString(T_SMLC_D& rData)
	{
		ConvertCharStr(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.dFactor=dFactor;
		rData.nStMin=nStMin;
		rData.nStMax=nStMax;
		rData.aPointData.Copy(aPointData);
		rData.LoadCaseId=LoadCaseId;
	}
	void ConvertToChar(T_SMLC_D& rData)
	{
		ConvertStrChar(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		dFactor=rData.dFactor;
		nStMin=rData.nStMin;
		nStMax=rData.nStMax;
		aPointData.Copy(rData.aPointData);
		LoadCaseId=rData.LoadCaseId;
	}
};
struct T_SMLC_UDRD_D
{
	T_SMLC_K key;
	T_SMLC_D data;
};
struct T_SMLC_UDRD_D_CH
{
	T_SMLC_K key;
	T_SMLC_D_CH data;
};
struct T_SMLC_UDRD_D_CH_RW
{
	T_SMLC_K key;
	char LoadCaseName[20];
	char Description[80];
	double dFactor;
	int nStMin;
	int nStMax;
	unsigned int LoadCaseId;
	int nCountPointData;
	void GetSmlc(T_SMLC_K& rKey, T_SMLC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.dFactor=dFactor;
		rData.nStMin=nStMin;
		rData.nStMax=nStMax;
		rData.LoadCaseId=LoadCaseId;
	}
	void SetSmlc(const T_SMLC_K Key, const T_SMLC_D_CH& rData)
	{
		key=Key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dFactor=rData.dFactor;
		nStMin=rData.nStMin;
		nStMax=rData.nStMax;
		LoadCaseId=rData.LoadCaseId;
		nCountPointData=(int)rData.aPointData.GetSize();
	}
};
struct T_SMLC_UNIT
{
	T_SMLC_UNIT()
	{
		LoadCaseName=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		dFactor=D_UNITSYS_NONE;
		nStMin=D_UNITSYS_NONE;
		nStMax=D_UNITSYS_NONE;
		nSmpt=D_UNITSYS_NONE;
		LoadCaseId=D_UNITSYS_NONE;;
	}
	int LoadCaseName;
	int Description;
	int dFactor;
	int nStMin;
	int nStMax;
	int nSmpt;
	int LoadCaseId;
};
#define HASHSIZESMLC 11
#define D_SMLC_MINNUM       1   // 입력가능한 최소 하중조건번호
#define D_SMLC_MAXNUM    9999   // 입력가능한 최대 하중조건번호

// Erection Sequence Analysis for Building
#define D_ESEQ_NUMESEQ  15
#define T_ESEQ_K unsigned int
struct T_ESEQ_SEL
{
	int nType;    // 1:Material Type  2=Material Key
	T_MATL_K KeyMatl;   // if(nType==1)1:ALL 2:Steel 3:Concrete 4=SRC 5=USER
	T_STOR_K KeyStorModel;
	void Initialize()
	{
		nType=0;
		KeyMatl=0;
		KeyStorModel=0;
	}
};

struct T_ESEQ_BASE_590
{
	T_STOR_K KeyStorLoad;
	T_ESEQ_SEL EseqSel[D_ESEQ_NUMESEQ];
	void Initialize()
	{
		KeyStorLoad=0;
		for(int i=0; i<D_ESEQ_NUMESEQ; i++)EseqSel[i].Initialize();
	}
};
struct T_ESEQ_BASE
{
	T_STOR_K KeyStorLoad;
	T_ESEQ_SEL EseqSel[D_ESEQ_NUMESEQ];
	double dDuration;
	double dAge;
	void Initialize()
	{
		KeyStorLoad=0;
		for(int i=0; i<D_ESEQ_NUMESEQ; i++)EseqSel[i].Initialize();
		dDuration = 0.0;
		dAge = 0.0;
	}  
	T_ESEQ_BASE(){}
	T_ESEQ_BASE(const T_ESEQ_BASE& src){*this = src;}
	T_ESEQ_BASE& operator=(const T_ESEQ_BASE& src)
	{
		KeyStorLoad = src.KeyStorLoad;
		for(int i = 0; i < D_ESEQ_NUMESEQ; i++)
			EseqSel[i] = src.EseqSel[i];
		dDuration = src.dDuration;
		dAge = src.dAge;
		return *this;
	}
	void Convert590(T_ESEQ_BASE_590& rData)
	{
		KeyStorLoad=rData.KeyStorLoad;
		for(int i=0; i<D_ESEQ_NUMESEQ; i++) EseqSel[i] = rData.EseqSel[i];
		dDuration = 0.0;
		dAge = 0.0;
	}
};

struct T_ESEQ_D_590
{
	T_STLD_K StldKey;   // Construction Dead Load Case
	CArray<T_ESEQ_BASE_590, T_ESEQ_BASE_590&>arEseq;
	void Initialize()
	{
		StldKey=0;
		arEseq.RemoveAll();
	}  
};

struct T_ESEQ_D
{
	T_STLD_K StldKey;   // Construction Dead Load Case
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&>arEseq;
	void Initialize()
	{
		StldKey=0;
		arEseq.RemoveAll();
	}
	T_ESEQ_D(){}
	T_ESEQ_D(T_ESEQ_D& src){*this = src;}
	T_ESEQ_D& operator=(T_ESEQ_D& src)
	{
		StldKey=src.StldKey;
		arEseq.Copy(src.arEseq);
		return *this;
	}
	void Convert590(T_ESEQ_D_590& rData)
	{
		StldKey = rData.StldKey;
		arEseq.RemoveAll();
		for(int i = 0; i < rData.arEseq.GetSize(); i++)
		{
			T_ESEQ_BASE EseqBase;
			EseqBase.Convert590(rData.arEseq.GetAt(i));
			arEseq.Add(EseqBase);
		}    
	}
};
struct T_ESEQ_UDRD_D
{
	T_ESEQ_D data;
};
struct T_ESEQ_UDRD_D_RW_590
{
	T_ESEQ_K key;
	T_STLD_K StldKey;
	unsigned int nCountEseq;
};
struct T_ESEQ_UDRD_D_RW
{
	T_ESEQ_K key;
	T_STLD_K StldKey;
	unsigned int nCountEseq;
	int nTemp;
	void GetEseq(T_ESEQ_K& rKey, T_ESEQ_D& rData)
	{
		rKey=key;
		rData.StldKey=StldKey;
	}
	void SetEseq(const T_ESEQ_K Key, const T_ESEQ_D& rData)
	{
		key=Key;
		StldKey=rData.StldKey;
		nCountEseq=(int)rData.arEseq.GetSize();    
	}  
	void Convert590(T_ESEQ_UDRD_D_RW_590& rData)
	{
		key=rData.key;
		StldKey=rData.StldKey;
		nCountEseq=rData.nCountEseq;
		nTemp = 0;
	}
};
struct T_ESEQ_UNIT
{
	T_ESEQ_UNIT()
	{
		StldKey=D_UNITSYS_NONE;
		KeyStorLoad=D_UNITSYS_NONE;
		nType=D_UNITSYS_NONE;
		KeyMatl=D_UNITSYS_NONE;
		KeyStorModel=D_UNITSYS_NONE;
	}
	int StldKey;
	int KeyStorLoad;
	int nType;
	int KeyMatl;
	int KeyStorModel;
};
#define HASHSIZEESEQ 1

// Erection Sequence for wizard : ESEQ에 Duration, Age 추가
struct T_ESQW_ADD_STOR_BASE
{
	double dStartingDay;
	T_STOR_K StoryKey;
	void Initialize()
	{
		dStartingDay = 0.0;
		StoryKey = 0;
	}
};
struct T_ESQW_ADD_LOAD
{
	T_ESQW_ADD_LOAD() {}
	T_ESQW_ADD_LOAD(const T_ESQW_ADD_LOAD& src) { *this = src; }
	T_ESQW_ADD_LOAD& operator=(const T_ESQW_ADD_LOAD& rData)
	{
		StldKey = rData.StldKey;
		arStory.Copy(rData.arStory);
		return *this;
	}
	void Initialize()
	{
		StldKey = 0;
		arStory.RemoveAll();
	}
	T_STLD_K StldKey;
	CArray<T_ESQW_ADD_STOR_BASE, T_ESQW_ADD_STOR_BASE&> arStory;
};
#define T_ESQW_K unsigned int
struct T_ESQW_D_590
{
	double dDuration;
	double dAge;
	T_STLD_K StldKey;   // Construction Dead Load Case
	CArray<T_ESEQ_BASE_590, T_ESEQ_BASE_590&>arEseq;
	CArray<T_ESQW_ADD_LOAD, T_ESQW_ADD_LOAD&>arAddDeadLoad;
	void Initialize()
	{
		dDuration = 0.0;
		dAge = 0.0;
		StldKey=0;
		arEseq.RemoveAll();
		arAddDeadLoad.RemoveAll();
	}
};
struct T_ESQW_D
{
	T_STLD_K StldKey;   // Construction Dead Load Case
	CArray<T_ESEQ_BASE, T_ESEQ_BASE&>arEseq;
	CArray<T_ESQW_ADD_LOAD, T_ESQW_ADD_LOAD&>arAddDeadLoad;
	void Initialize()
	{
		StldKey=0;
		arEseq.RemoveAll();
		arAddDeadLoad.RemoveAll();
	}
	T_ESQW_D(){}
	T_ESQW_D(T_ESQW_D& src){*this = src;}
	T_ESQW_D& operator=(T_ESQW_D& src)
	{
		StldKey = src.StldKey;
		arEseq.Copy(src.arEseq);
		arAddDeadLoad.Copy(src.arAddDeadLoad);
		return *this;
	}
	void Convert590(T_ESQW_D_590& rData)
	{
		StldKey=rData.StldKey;
		arEseq.RemoveAll();
		for(int i = 0; i < rData.arEseq.GetSize(); i++)
		{
			T_ESEQ_BASE EseqBase;
			EseqBase.Convert590(rData.arEseq.GetAt(i));
			arEseq.Add(EseqBase);
		}     
		arAddDeadLoad.Copy(rData.arAddDeadLoad);
	}  
};
struct T_ESQW_UDRD_D
{
	T_ESQW_D data;
};
struct T_ESQW_UDRD_D_RW_590
{
	T_ESQW_K key;
	double dDuration;
	double dAge;
	T_STLD_K StldKey;
	unsigned int nCountEseq;
	unsigned int nCountAddDeadLoad;  
};
struct T_ESQW_UDRD_D_RW
{
	T_ESQW_K key;
	T_STLD_K StldKey;
	unsigned int nCountEseq;
	unsigned int nCountAddDeadLoad;
	void GetEsqw(T_ESQW_K& rKey, T_ESQW_D& rData)
	{
		rKey=key;
		rData.StldKey=StldKey;
	}
	void SetEsqw(const T_ESQW_K Key, const T_ESQW_D& rData)
	{
		key=Key;
		StldKey=rData.StldKey;
		nCountEseq=(int)rData.arEseq.GetSize();
		nCountAddDeadLoad=(int)rData.arAddDeadLoad.GetSize();
	}
	void Convert590(T_ESQW_UDRD_D_RW_590& rData)
	{
		key=rData.key;
		StldKey=rData.StldKey;
		nCountEseq=rData.nCountEseq;
		nCountAddDeadLoad=rData.nCountAddDeadLoad;
	}
};
struct T_ESQW_ADD_LOAD_RW_BASE
{
	T_STLD_K StldKey;
	int nCountStory;
};
struct T_ESQW_ADD_LOAD_RW
{
	CArray<T_ESQW_ADD_LOAD_RW_BASE, T_ESQW_ADD_LOAD_RW_BASE&> arAddDeadLoad;
	void SetEsqwAddLoad(T_ESQW_D& rData)
	{
		arAddDeadLoad.SetSize(rData.arAddDeadLoad.GetSize());
		for (int i = 0; i < arAddDeadLoad.GetSize(); i++)
		{
			arAddDeadLoad[i].StldKey = rData.arAddDeadLoad[i].StldKey;
			arAddDeadLoad[i].nCountStory = (int)rData.arAddDeadLoad[i].arStory.GetSize();
		}
	}
};
struct T_ESQW_UNIT
{
	T_ESQW_UNIT()
	{
		dDuration = D_UNITSYS_BASE_DAY;
		dAge = D_UNITSYS_BASE_DAY;
	}
	int dDuration;
	int dAge;
};
#define HASHSIZEESQW 1


// Pre-Combined LoadCases for Composite Bridge
#define T_PLCB_K unsigned int
struct T_PLCB_D
{
	CArray<T_STLD_K, T_STLD_K&>arLoad;
	void Initialize()
	{
		arLoad.RemoveAll();
	}
	T_PLCB_D(){}
	T_PLCB_D(T_PLCB_D& src){*this = src;}
	T_PLCB_D& operator=(T_PLCB_D& src)
	{
		arLoad.Copy(src.arLoad);
		return *this;
	}
};
struct T_PLCB_UDRD_D
{
	T_PLCB_D data;
};
struct T_PLCB_UDRD_D_RW
{
	T_PLCB_K key;
	unsigned int nCountarLoad;
	void GetPlcb(T_PLCB_K& rKey, T_PLCB_D& rData)
	{
		rKey=key;
	}
	void SetPlcb(const T_PLCB_K Key, const T_PLCB_D& rData)
	{
		key=Key;
		nCountarLoad=(int)rData.arLoad.GetSize();
	}
};
struct T_PLCB_UNIT
{
	T_PLCB_UNIT()
	{
		StldKey=D_UNITSYS_NONE;
	}
	int StldKey;
};
#define HASHSIZEPLCB 1

#pragma pack(pop)

#endif  // __DB_ST_DT_SPLD_H__
