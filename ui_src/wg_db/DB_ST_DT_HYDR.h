#ifndef __DB_ST_DT_HYDR_H__
#define __DB_ST_DT_HYDR_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// hhnd ///
#define T_HHND_K unsigned int
struct T_HHND_D_600
{
	CString   strName;
	T_NODE_K  nNodeKey;
	int       nComp;
};
struct T_HHND_D_960
{
	CString   strName;
	T_NODE_K  nNodeKey;
	int       nComp;
	int       nType;      // 0:node, 1:element   // nType == 0 인 경우는 Node만 쓰이고, nType == 1인 경우는 Node, Element 모두 쓰인다.
	T_ELEM_K  nElemKey;
};
struct T_HHND_D
{
	CString   strName;
	T_NODE_K  nNodeKey;
	int       nComp;
	int       nType;      // 0:node, 1:element   // nType == 0 인 경우는 Node만 쓰이고, nType == 1인 경우는 Node, Element 모두 쓰인다.
	T_ELEM_K  nElemKey;
	CArray<T_ELEM_K, T_ELEM_K> aAvgElemK; // [MEC]
	T_HHND_D()
	{
	}
	T_HHND_D(T_HHND_D& src)
	{
		*this = src;
	}
	T_HHND_D& operator=(const T_HHND_D& src)
	{
		strName=src.strName;
		nNodeKey=src.nNodeKey;
		nComp=src.nComp;
		nType=src.nType;
		nElemKey=src.nElemKey;
		aAvgElemK.Copy(src.aAvgElemK);
		return *this;
	}
	void Initialize()
	{
		strName.Empty();
		nNodeKey = 0;
		nComp = 0;
		nType = 0;
		nElemKey = 0;
		aAvgElemK.RemoveAll();
	}
	void Convert600(T_HHND_D_600& rData)
	{
		strName = rData.strName;
		nNodeKey = rData.nNodeKey;
		nComp = rData.nComp;
		nType = 0;
		nElemKey = 0;
		aAvgElemK.RemoveAll();
	}
	void Convert960(T_HHND_D_960& rData)
	{
		strName = rData.strName;
		nNodeKey = rData.nNodeKey;
		nComp = rData.nComp;
		nType = rData.nType;
		nElemKey = rData.nElemKey;
		aAvgElemK.RemoveAll();
	}
};
struct T_HHND_D_CH_600
{
	char     strName[20];
	T_NODE_K nNodeKey;
	int      nComp;
};
struct T_HHND_D_CH_960
{
	char     strName[20];
	T_NODE_K nNodeKey;
	int      nComp;
	int      nType;      // 0:node, 1:element
	T_ELEM_K nElemKey;
};
struct T_HHND_D_CH
{
	char     strName[20];
	T_NODE_K nNodeKey;
	int      nComp;
	int      nType;      // 0:node, 1:element
	T_ELEM_K nElemKey;
	CArray<T_ELEM_K, T_ELEM_K> aAvgElemK;
	void ConvertToString(T_HHND_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nNodeKey=nNodeKey;
		rData.aAvgElemK.Copy(aAvgElemK);
		rData.nComp=nComp;
		rData.nType=nType;
		rData.nElemKey=nElemKey;
	}
	void ConvertToChar(T_HHND_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nNodeKey=rData.nNodeKey;
		nComp = rData.nComp;
		nType=rData.nType;
		nElemKey=rData.nElemKey;
		aAvgElemK.Copy(rData.aAvgElemK);
	}
	void Convert600(T_HHND_D_CH_600& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		nNodeKey = rData.nNodeKey;
		nComp = rData.nComp;
		nType = 0;
		nElemKey = 0;
		aAvgElemK.RemoveAll();
	}
	void Convert960(T_HHND_D_CH_960& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		nNodeKey = rData.nNodeKey;
		nComp = rData.nComp;
		nType = 0;
		nElemKey = 0;
		aAvgElemK.RemoveAll();
	}
};
struct T_HHND_UDRD_D
{
	T_HHND_K key;
	T_HHND_D data;
};
struct T_HHND_UDRD_D_CH_600
{
	T_HHND_K key;
	T_HHND_D_CH_600 data;
};
struct T_HHND_UDRD_D_CH_960
{
	T_HHND_K key;
	T_HHND_D_CH_960 data;
};
struct T_HHND_UDRD_D_CH
{
	T_HHND_K key;
	T_HHND_D_CH data;
};
struct T_HHND_UDRD_D_CH_RW
{
	T_HHND_K key;
	char     strName[20];
	T_NODE_K nNodeKey;
	int      nComp;
	int      nType;      // 0:node, 1:element
	T_ELEM_K nElemKey;
	int nCountAvgElem;
	void GetHhnd(T_HHND_K& rKey, T_HHND_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nNodeKey = nNodeKey;
		rData.nComp = nComp;
		rData.nType = nType;
		rData.nElemKey = nElemKey;
	}
	void SetHhnd(T_HHND_K& rKey, T_HHND_D_CH& rData)
	{
		key = rKey;
		memcpy(strName, rData.strName, sizeof(strName));
		nNodeKey = rData.nNodeKey;
		nComp = rData.nComp;
		nType = rData.nType;
		nElemKey = rData.nElemKey;
		nCountAvgElem = (int)rData.aAvgElemK.GetSize();
	}
	void Convert600(T_HHND_UDRD_D_CH_600& rData)
	{
		key = rData.key;
		memcpy(strName, rData.data.strName, sizeof(strName));
		nNodeKey = rData.data.nNodeKey;
		nComp = rData.data.nComp;
		nType = 0;
		nElemKey = 0;
		nCountAvgElem = 0;
	}
	void Convert960(T_HHND_UDRD_D_CH_960& rData)
	{
		key = rData.key;
		memcpy(strName, rData.data.strName, sizeof(strName));
		nNodeKey = rData.data.nNodeKey;
		nComp = rData.data.nComp;
		nType = rData.data.nType;
		nElemKey = rData.data.nElemKey;
		nCountAvgElem = 0;
	}
};
struct T_HHND_UNIT
{
	T_HHND_UNIT()
	{
		strName=D_UNITSYS_NONE;
		nNodeKey=D_UNITSYS_NONE;
		nComp=D_UNITSYS_NONE;
	}
	int strName;
	int nNodeKey;
	int nComp;
};
#define HASHSIZEHHND 31

/// hspt ///  hydration : specified temperature
//#define T_HSPT_K T_NODE_K
#define T_HSPT_KEY unsigned int
union T_HSPT_K
{
	T_HSPT_KEY keymap;
	struct
	{
		unsigned int entity : 20;
		unsigned int serial : 12;
	} key;
};
struct T_HSPT_D_431
{
	double dTemper;
};
struct T_HSPT_D
{
	double dTemper;
	T_BNGR_K GroupKey;
	void Initialize()
	{
		dTemper = 0.0;
		GroupKey = 0;
	}
	void Convert431(T_HSPT_D_431& rData)
	{
		dTemper = rData.dTemper;
		GroupKey = 0;
	}
};
struct T_HSPT_UDRD_D_431
{
	T_HSPT_KEY key;
	T_HSPT_D_431 data;
};
struct T_HSPT_UDRD_D
{
	T_HSPT_K key;
	T_HSPT_D data;
};
struct T_HSPT_UNIT
{
	T_HSPT_UNIT()
	{
		dTemper=D_UNITSYS_BASE_TEMPER;
	}
	int dTemper;
};
#define HASHSIZEHSPT 97

// hpce : Hydration Pipe Cooling Element
#define T_HPCE_K unsigned int
struct T_HPCE_BASE
{
	T_NODE_K KeyNode;
	void Initialize()
	{
		KeyNode=0;
	}
};
#define T_HPCE_DT CArray<T_HPCE_BASE, T_HPCE_BASE&>
struct T_HPCE_D
{
	CString strName;
	double dHeat;     // specific heat
	double dDensity;  // density
	double dTemper;   // Inlet Temperature
	double dFlowRate; // Flow Rate
	int    nStartTime;  // Inflow Start Time : Hour
	int    nEndTime;    // Inflow End Time   : Hour
	double dDiameter;   // Cooling pipe diameter
	double dConvecCoef;   // Convection Coefficient
	T_HPCE_DT aNode;
	unsigned int nSeq;
	void Initialize()
	{
		strName.Empty();
		dHeat = 0.0;
		dDensity = 0.0;
		dTemper = 0.0;
		dFlowRate = 0.0;
		nStartTime = 0;
		nEndTime = 0;
		dDiameter = 0.0;
		dConvecCoef = 0.0;
		aNode.RemoveAll();
		nSeq=0;
	}
	T_HPCE_D()
	{
	}
	T_HPCE_D(T_HPCE_D& src)
	{
		*this = src;
	}
	T_HPCE_D& operator=(T_HPCE_D& src)
	{
		strName=src.strName;
		dHeat = src.dHeat;
		dDensity = src.dDensity;
		dTemper = src.dTemper;
		dFlowRate = src.dFlowRate;
		nStartTime = src.nStartTime;
		nEndTime = src.nEndTime;
		dDiameter = src.dDiameter;
		dConvecCoef = src.dConvecCoef;
		aNode.Copy(src.aNode);
		nSeq = src.nSeq;
		return *this;
	}
};
struct T_HPCE_D_CH
{
	char strName[40];
	double dHeat;     
	double dDensity;  
	double dTemper;   
	double dFlowRate; 
	int    nStartTime;
	int    nEndTime;  
	double dDiameter; 
	double dConvecCoef;
	T_HPCE_DT aNode;
	unsigned int nSeq;
	void ConvertToString(T_HPCE_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.dHeat = dHeat;
		rData.dDensity = dDensity;
		rData.dTemper = dTemper;
		rData.dFlowRate = dFlowRate;
		rData.nStartTime = nStartTime;
		rData.nEndTime = nEndTime;
		rData.dDiameter = dDiameter;
		rData.dConvecCoef = dConvecCoef;
		rData.aNode.Copy(aNode);
		rData.nSeq = nSeq;
	}
	void ConvertToChar(T_HPCE_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		dHeat = rData.dHeat;
		dDensity = rData.dDensity;
		dTemper = rData.dTemper;
		dFlowRate = rData.dFlowRate;
		nStartTime = rData.nStartTime;
		nEndTime = rData.nEndTime;
		dDiameter = rData.dDiameter;
		dConvecCoef = rData.dConvecCoef;
		aNode.Copy(rData.aNode);
		nSeq = rData.nSeq;
	}
};
struct T_HPCE_UDRD_D
{
	T_HPCE_K key;
	T_HPCE_D data;
};
struct T_HPCE_UDRD_D_CH
{
	T_HPCE_K key;
	T_HPCE_D_CH data;
};
struct T_HPCE_UDRD_D_CH_RW
{
	T_HPCE_K key;
	char   strName[40];
	double dHeat;     
	double dDensity;  
	double dTemper;   
	double dFlowRate; 
	int    nStartTime;
	int    nEndTime;  
	double dDiameter; 
	double dConvecCoef;
	unsigned int nSeq;
	unsigned int nCountNode;
	void GetHpce(T_HPCE_K& rKey, T_HPCE_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.dHeat = dHeat;
		rData.dDensity = dDensity;
		rData.dTemper = dTemper;
		rData.dFlowRate = dFlowRate;
		rData.nStartTime = nStartTime;
		rData.nEndTime = nEndTime;
		rData.dDiameter = dDiameter;
		rData.dConvecCoef = dConvecCoef;
		rData.nSeq = nSeq;
	}
	void SetHpce(const T_HPCE_K Key, const T_HPCE_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		dHeat = rData.dHeat;
		dDensity = rData.dDensity;
		dTemper = rData.dTemper;
		dFlowRate = rData.dFlowRate;
		nStartTime = rData.nStartTime;
		nEndTime = rData.nEndTime;
		dDiameter = rData.dDiameter;
		dConvecCoef = rData.dConvecCoef;
		nSeq = rData.nSeq;
		nCountNode=(int)rData.aNode.GetSize();
	}
};
struct T_HPCE_UNIT
{
	T_HPCE_UNIT()
	{
		strName=D_UNITSYS_NONE;
		dHeat=D_UNITSYS_BASE_SPHEAT;
		dDensity=D_UNITSYS_BASE_DENSITY;
		dTemper=D_UNITSYS_BASE_TEMPER;
		dFlowRate=D_UNITSYS_BASE_FLOWRATE;
		nStartTime=D_UNITSYS_BASE_HOUR;
		nEndTime=D_UNITSYS_BASE_HOUR;
		dDiameter=D_UNITSYS_BASE_LENGTH;
		dConvecCoef=D_UNITSYS_BASE_CONVECTION;
		KeyNode=D_UNITSYS_NONE;
		nSeq=D_UNITSYS_NONE;;
	}
	int strName;
	int dHeat;     
	int dDensity;  
	int dTemper;   
	int dFlowRate; 
	int nStartTime;
	int nEndTime;  
	int dDiameter; 
	int dConvecCoef;
	int KeyNode;
	int nSeq;
};
#define HASHSIZEHPCE 11
#define D_HPCE_MINNUM       1   // 입력가능한 최소 번호
#define D_HPCE_MAXNUM    9999   // 입력가능한 최대 번호

struct T_CCFC_BASE
{
	double dTime;
	double dValue;
	void Initialize()
	{
		dTime=0.0;
		dValue=0.0;
	}
};
#define T_CCFC_DT CArray<T_CCFC_BASE, T_CCFC_BASE&>
#define T_CCFC_K unsigned int
struct T_CCFC_D
{
	CString   strFuncName;
	int       nFuncType;      // =1 Constant =2 User
	double    dScaleFactor;   // Scale Factor
	double    dCoefficient;   // nFuncType = 1
	T_CCFC_DT aFuncData;      // nFuncType = 2
	unsigned int nSeq;        // 순번

	void Initialize()
	{
		strFuncName.Empty();
		nFuncType = 1;
		dScaleFactor = 1.0;
		dCoefficient = 0.0;
		aFuncData.RemoveAll();
		nSeq = 0;
	}
	T_CCFC_D()
	{
	}
	T_CCFC_D(T_CCFC_D& src)
	{
		*this = src;
	}
	T_CCFC_D& operator=(T_CCFC_D& src)
	{
		strFuncName=src.strFuncName;
		nFuncType=src.nFuncType;
		dScaleFactor=src.dScaleFactor;
		dCoefficient=src.dCoefficient;
		aFuncData.Copy(src.aFuncData);
		nSeq=src.nSeq;
		return *this;
	}
};
struct T_CCFC_D_CH
{
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dCoefficient;
	T_CCFC_DT   aFuncData;
	unsigned int nSeq;
	void ConvertToString(T_CCFC_D& rData)
	{
		ConvertCharStr(strFuncName, rData.strFuncName, sizeof(strFuncName));
		rData.nFuncType=nFuncType;
		rData.dScaleFactor=dScaleFactor;
		rData.dCoefficient=dCoefficient;
		rData.aFuncData.Copy(aFuncData);
		rData.nSeq=nSeq;
	}
	void ConvertToChar(T_CCFC_D& rData)
	{
		ConvertStrChar(rData.strFuncName, strFuncName, sizeof(strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dCoefficient=rData.dCoefficient;
		aFuncData.Copy(rData.aFuncData);
		nSeq=rData.nSeq;
	}
};
struct T_CCFC_UDRD_D
{
	T_CCFC_K key;
	T_CCFC_D data;
};
struct T_CCFC_UDRD_D_CH
{
	T_CCFC_K key;
	T_CCFC_D_CH data;
};
struct T_CCFC_UDRD_D_CH_RW
{
	T_CCFC_K    key;
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dCoefficient;
	int         nFuncDataCount;
	unsigned int nSeq;
	void GetCcfc(T_CCFC_K& rKey, T_CCFC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strFuncName, strFuncName, sizeof(rData.strFuncName));
		rData.nFuncType=nFuncType;
		rData.dScaleFactor=dScaleFactor;
		rData.dCoefficient=dCoefficient;
		rData.nSeq=nSeq;
	}
	void SetCcfc(const T_CCFC_K Key, const T_CCFC_D_CH& rData)
	{
		key=Key;
		memcpy(strFuncName, rData.strFuncName, sizeof(rData.strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dCoefficient=rData.dCoefficient;
		nFuncDataCount=(int)rData.aFuncData.GetSize();
		nSeq=rData.nSeq;
	}
};
struct T_CCFC_UNIT
{
	T_CCFC_UNIT()
	{
		strFuncName=D_UNITSYS_NONE;
		nFuncType=D_UNITSYS_NONE;
		dScaleFactor=D_UNITSYS_NONE;
		dCoefficient=D_UNITSYS_BASE_CONVECTION;
		dTime=D_UNITSYS_BASE_HOUR;
		dValue=D_UNITSYS_BASE_CONVECTION;
		nSeq=D_UNITSYS_NONE;
	}
	int strFuncName;
	int nFuncType;
	int dScaleFactor;
	int dCoefficient;
	int dTime;
	int dValue;
	int nSeq;
};
#define HASHSIZECCFC 11
#define D_CCFC_MINNUM       1   // 
#define D_CCFC_MAXNUM    9999   // 

struct T_ETFC_BASE
{
	double dTime;
	double dValue;
	void Initialize()
	{
		dTime=0.0;
		dValue=0.0;
	}
};
#define T_ETFC_DT CArray<T_ETFC_BASE, T_ETFC_BASE&>
#define T_ETFC_K unsigned int
struct T_ETFC_D
{
	CString   strFuncName;
	int       nFuncType;      // =1 Constant =2 Sine Function =3 User
	double    dScaleFactor;
	double    dTemperature;   // nFuncType = 1
	double    dMaxTemp;       // nFuncType = 2
	double    dDelayTime;     // nFuncType = 2
	double    dMeanTemp;      // nFuncType = 2
	T_ETFC_DT aFuncData;      // nFuncType = 3
	unsigned int nSeq;        // 순번

	void Initialize()
	{
		strFuncName.Empty();
		nFuncType = 0;
		dScaleFactor = 1.0;
		dTemperature = 0.0;
		dMaxTemp = 0.0;
		dDelayTime = 0.0;
		dMeanTemp = 0.0;
		aFuncData.RemoveAll();
		nSeq = 0;
	}
	T_ETFC_D()
	{
	}
	T_ETFC_D(T_ETFC_D& src)
	{
		*this = src;
	}
	T_ETFC_D& operator=(T_ETFC_D& src)
	{
		strFuncName=src.strFuncName;
		nFuncType=src.nFuncType;
		dScaleFactor=src.dScaleFactor;
		dTemperature=src.dTemperature;
		dMaxTemp=src.dMaxTemp;
		dDelayTime=src.dDelayTime;
		dMeanTemp=src.dMeanTemp;
		aFuncData.Copy(src.aFuncData);
		nSeq=src.nSeq;
		return *this;
	}
};
struct T_ETFC_D_CH
{
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dMaxTemp;
	double      dDelayTime;
	double      dMeanTemp;
	T_ETFC_DT   aFuncData;
	unsigned int nSeq;
	void ConvertToString(T_ETFC_D& rData)
	{
		ConvertCharStr(strFuncName, rData.strFuncName, sizeof(strFuncName));
		rData.nFuncType=nFuncType;
		rData.dScaleFactor=dScaleFactor;
		rData.dTemperature=dTemperature;
		rData.dMaxTemp=dMaxTemp;
		rData.dDelayTime=dDelayTime;
		rData.dMeanTemp=dMeanTemp;
		rData.aFuncData.Copy(aFuncData);
		rData.nSeq=nSeq;
	}
	void ConvertToChar(T_ETFC_D& rData)
	{
		ConvertStrChar(rData.strFuncName, strFuncName, sizeof(strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dMaxTemp=rData.dMaxTemp;
		dDelayTime=rData.dDelayTime;
		dMeanTemp=rData.dMeanTemp;
		aFuncData.Copy(rData.aFuncData);
		nSeq=rData.nSeq;
	}
};
struct T_ETFC_UDRD_D
{
	T_ETFC_K key;
	T_ETFC_D data;
};
struct T_ETFC_UDRD_D_CH
{
	T_ETFC_K key;
	T_ETFC_D_CH data;
};
struct T_ETFC_UDRD_D_CH_RW
{
	T_ETFC_K    key;
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dMaxTemp;
	double      dDelayTime;
	double      dMeanTemp;
	int         nFuncDataCount;
	unsigned int nSeq;
	void GetEtfc(T_ETFC_K& rKey, T_ETFC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strFuncName, strFuncName, sizeof(rData.strFuncName));
		rData.nFuncType=nFuncType;
		rData.dScaleFactor=dScaleFactor;
		rData.dTemperature=dTemperature;
		rData.dMaxTemp=dMaxTemp;
		rData.dDelayTime=dDelayTime;
		rData.dMeanTemp=dMeanTemp;
		rData.nSeq=nSeq;
	}
	void SetEtfc(const T_ETFC_K Key, const T_ETFC_D_CH& rData)
	{
		key=Key;
		memcpy(strFuncName, rData.strFuncName, sizeof(rData.strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dMaxTemp=rData.dMaxTemp;
		dDelayTime=rData.dDelayTime;
		dMeanTemp=rData.dMeanTemp;
		nFuncDataCount=(int)rData.aFuncData.GetSize();
		nSeq=rData.nSeq;
	}
};
struct T_ETFC_UNIT
{
	T_ETFC_UNIT()
	{
		strFuncName=D_UNITSYS_NONE;
		nFuncType=D_UNITSYS_NONE;
		dScaleFactor=D_UNITSYS_NONE;
		dTemperature=D_UNITSYS_BASE_TEMPER;
		dMaxTemp=D_UNITSYS_BASE_TEMPER;
		dDelayTime=D_UNITSYS_BASE_HOUR;
		dMeanTemp=D_UNITSYS_BASE_TEMPER;
		dTime=D_UNITSYS_BASE_HOUR;
		dValue=D_UNITSYS_BASE_TEMPER;
		nSeq=D_UNITSYS_NONE;
	}
	int strFuncName;
	int nFuncType;
	int dScaleFactor;
	int dTemperature;
	int dMaxTemp;
	int dDelayTime;
	int dMeanTemp;
	int dTime;
	int dValue;
	int nSeq;
};
#define HASHSIZEETFC 11
#define D_ETFC_MINNUM       1   // 
#define D_ETFC_MAXNUM    9999   // 

struct T_HSFC_BASE
{
	double dTime;
	double dValue;
	void Initialize()
	{
		dTime=0.0;
		dValue=0.0;
	}
};
#define T_HSFC_DT CArray<T_HSFC_BASE, T_HSFC_BASE&>
#define T_HSFC_K unsigned int
struct T_HSFC_D_630
{
	CString   strFuncName;
	int       nFuncType;      // =1 Constant =2 Sine Function =3 User
	double    dScaleFactor;
	double    dTemperature;   // nFuncType = 1
	double    dK;             // nFuncType = 2
	double    dAlpha;         // nFuncType = 2
	T_HSFC_DT aFuncData;      // nFuncType = 3
	unsigned int nSeq;        // 순번
};
struct T_HSFC_D_650
{
	CString   strFuncName;
	int       nFuncType;      // =1 Constant =2 Sine Function =3 User
	double    dScaleFactor;
	double    dTemperature;   // nFuncType = 1
	double    dK;             // nFuncType = 2
	double    dAlpha;         // nFuncType = 2
	T_HSFC_DT aFuncData;      // nFuncType = 3
	unsigned int nSeq;        // 순번
	BOOL      bUseConcData;   // 6.4.0추가  nFuncType = 2
	int       nCementType;    // 6.4.0추가  nFuncType = 2   : 0=보통, 1=중용열, 2=조강, 3=고로, 4=플라이애쉬
	int       nTemperature;   // 6.4.0추가  nFuncType = 2   : 0=10도, 1=20도, 2=30도 
	double    dCementContent; // 6.4.0추가  nFuncType = 2
};
struct T_HSFC_D
{
	CString   strFuncName;
	int       nFuncType;      // =1 Constant =2 Sine Function =3 User
	double    dScaleFactor;
	double    dTemperature;   // nFuncType = 1
	double    dK;             // nFuncType = 2
	double    dAlpha;         // nFuncType = 2
	T_HSFC_DT aFuncData;      // nFuncType = 3
	unsigned int nSeq;        // 순번
	BOOL      bUseConcData;   // 6.4.0추가  nFuncType = 2
	int       nCementType;    // 6.4.0추가  nFuncType = 2   : 0=보통, 1=중용열, 2=조강, 3=고로, 4=플라이애쉬
	int       nTemperature;   // 6.4.0추가  nFuncType = 2   : 0=10도, 1=20도, 2=30도 
	double    dCementContent; // 6.4.0추가  nFuncType = 2
	int       nDataType;      // 6.6.0추가  nFuncType = 3인 경우 사용 0=Heat Source, 1=Adiabatic Temperature
	void Initialize()
	{
		strFuncName.Empty();
		nFuncType = 0;
		dScaleFactor = 1.0;
		dTemperature = 0.0;
		dK = 0.0;
		dAlpha = 0.0;
		aFuncData.RemoveAll();
		nSeq = 0;
		bUseConcData = FALSE;
		nCementType = 0;
		nTemperature = 0;
		dCementContent = 0.;
		nDataType = 0;
	}
	T_HSFC_D()
	{
	}
	T_HSFC_D(T_HSFC_D& src)
	{
		*this = src;
	}
	T_HSFC_D& operator=(T_HSFC_D& src)
	{
		strFuncName=src.strFuncName;
		nFuncType=src.nFuncType;
		dScaleFactor=src.dScaleFactor;
		dTemperature=src.dTemperature;
		dK=src.dK;
		dAlpha=src.dAlpha;
		aFuncData.Copy(src.aFuncData);
		nSeq=src.nSeq;
		bUseConcData   = src.bUseConcData  ;
		nCementType    = src.nCementType   ;
		nTemperature   = src.nTemperature  ;
		dCementContent = src.dCementContent;
		nDataType      = src.nDataType;
		return *this;
	}
	void Convert630(T_HSFC_D_630& rData)
	{
		strFuncName    = rData.strFuncName;
		nFuncType      = rData.nFuncType;
		dScaleFactor   = rData.dScaleFactor;
		dTemperature   = rData.dTemperature;
		dK             = rData.dK;
		dAlpha         = rData.dAlpha;
		aFuncData.Copy(rData.aFuncData);
		nSeq           = rData.nSeq;
		bUseConcData   = FALSE;
		nCementType    = 0;
		nTemperature   = 0;
		dCementContent = 0.;
		nDataType      = 0;
	}
	void Convert650(T_HSFC_D_650& rData)
	{
		strFuncName    = rData.strFuncName;
		nFuncType      = rData.nFuncType;
		dScaleFactor   = rData.dScaleFactor;
		dTemperature   = rData.dTemperature;
		dK             = rData.dK;
		dAlpha         = rData.dAlpha;
		aFuncData.Copy(rData.aFuncData);
		nSeq           = rData.nSeq;
		bUseConcData   = rData.bUseConcData;
		nCementType    = rData.nCementType;
		nTemperature   = rData.nTemperature;
		dCementContent = rData.dCementContent;
		nDataType      = 0;
	}
};
struct T_HSFC_D_CH_630
{
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dK;
	double      dAlpha;
	T_HSFC_DT   aFuncData;
	unsigned int nSeq;
};
struct T_HSFC_D_CH_650
{
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dK;
	double      dAlpha;
	T_HSFC_DT   aFuncData;
	unsigned int nSeq;
	BOOL      bUseConcData;   // 6.4.0추가  nFuncType = 2
	int       nCementType;    // 6.4.0추가  nFuncType = 2   : 0=보통, 1=중용열, 2=조강, 3=고로, 4=플라이애쉬
	int       nTemperature;   // 6.4.0추가  nFuncType = 2   : 0=10도, 1=20도, 2=30도 
	double    dCementContent; // 6.4.0추가  nFuncType = 2
};
struct T_HSFC_D_CH
{
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dK;
	double      dAlpha;
	T_HSFC_DT   aFuncData;
	unsigned int nSeq;
	BOOL      bUseConcData;   // 6.4.0추가  nFuncType = 2
	int       nCementType;    // 6.4.0추가  nFuncType = 2   : 0=보통, 1=중용열, 2=조강, 3=고로, 4=플라이애쉬
	int       nTemperature;   // 6.4.0추가  nFuncType = 2   : 0=10도, 1=20도, 2=30도 
	double    dCementContent; // 6.4.0추가  nFuncType = 2
	int       nDataType;      // 6.6.0추가  nFuncType = 3인 경우 사용 0=Heat Source, 1=Adiabatic Temperature
	void ConvertToString(T_HSFC_D& rData)
	{
		ConvertCharStr(strFuncName, rData.strFuncName, sizeof(strFuncName));
		rData.nFuncType=nFuncType;
		rData.dScaleFactor=dScaleFactor;
		rData.dTemperature=dTemperature;
		rData.dK=dK;
		rData.dAlpha=dAlpha;
		rData.aFuncData.Copy(aFuncData);
		rData.nSeq=nSeq;
		rData.bUseConcData   = bUseConcData  ;
		rData.nCementType    = nCementType   ;
		rData.nTemperature   = nTemperature  ;
		rData.dCementContent = dCementContent;
		rData.nDataType      = nDataType     ;
	}
	void ConvertToChar(T_HSFC_D& rData)
	{
		ConvertStrChar(rData.strFuncName, strFuncName, sizeof(strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dK=rData.dK;
		dAlpha=rData.dAlpha;
		aFuncData.Copy(rData.aFuncData);
		nSeq=rData.nSeq;
		bUseConcData   = rData.bUseConcData  ;
		nCementType    = rData.nCementType   ;
		nTemperature   = rData.nTemperature  ;
		dCementContent = rData.dCementContent;
		nDataType      = rData.nDataType     ;
	}
	void Convert630(T_HSFC_D_CH_630& rData)
	{
		memcpy(strFuncName, rData.strFuncName, sizeof(strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dK=rData.dK;
		dAlpha=rData.dAlpha;
		aFuncData.Copy(rData.aFuncData);
		nSeq=rData.nSeq;
		bUseConcData   = FALSE;
		nCementType    = 0;
		nTemperature   = 0;
		dCementContent = 0.;
		nDataType      = 0;
	}
	void Convert650(T_HSFC_D_CH_650& rData)
	{
		memcpy(strFuncName, rData.strFuncName, sizeof(strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dK=rData.dK;
		dAlpha=rData.dAlpha;
		aFuncData.Copy(rData.aFuncData);
		nSeq=rData.nSeq;
		bUseConcData   = rData.bUseConcData;
		nCementType    = rData.nCementType;
		nTemperature   = rData.nTemperature;
		dCementContent = rData.dCementContent;
		nDataType      = 0;
	}
};
struct T_HSFC_UDRD_D
{
	T_HSFC_K key;
	T_HSFC_D data;
};
struct T_HSFC_UDRD_D_CH_630
{
	T_HSFC_K key;
	T_HSFC_D_CH_630 data;
};
struct T_HSFC_UDRD_D_CH_650
{
	T_HSFC_K key;
	T_HSFC_D_CH_650 data;
};
struct T_HSFC_UDRD_D_CH
{
	T_HSFC_K key;
	T_HSFC_D_CH data;
};
struct T_HSFC_UDRD_D_CH_RW_630
{
	T_HSFC_K    key;
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dK;
	double      dAlpha;
	int         nFuncDataCount;
	unsigned int nSeq;
};
struct T_HSFC_UDRD_D_CH_RW_650
{
	T_HSFC_K    key;
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dK;
	double      dAlpha;
	int         nFuncDataCount;
	unsigned int nSeq;
	BOOL      bUseConcData;   // 6.4.0추가  nFuncType = 2
	int       nCementType;    // 6.4.0추가  nFuncType = 2   : 0=보통, 1=중용열, 2=조강, 3=고로, 4=플라이애쉬
	int       nTemperature;   // 6.4.0추가  nFuncType = 2   : 0=10도, 1=20도, 2=30도 
	double    dCementContent; // 6.4.0추가  nFuncType = 2
};
struct T_HSFC_UDRD_D_CH_RW
{
	T_HSFC_K    key;
	char        strFuncName[20];
	int         nFuncType;
	double      dScaleFactor;
	double      dTemperature;
	double      dK;
	double      dAlpha;
	int         nFuncDataCount;
	unsigned int nSeq;
	BOOL      bUseConcData;   // 6.4.0추가  nFuncType = 2
	int       nCementType;    // 6.4.0추가  nFuncType = 2   : 0=보통, 1=중용열, 2=조강, 3=고로, 4=플라이애쉬
	int       nTemperature;   // 6.4.0추가  nFuncType = 2   : 0=10도, 1=20도, 2=30도 
	double    dCementContent; // 6.4.0추가  nFuncType = 2
	int       nDataType;      // 6.6.0추가  nFuncType = 3인 경우 사용 0=Heat Source, 1=Adiabatic Temperature
	void GetHsfc(T_HSFC_K& rKey, T_HSFC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strFuncName, strFuncName, sizeof(rData.strFuncName));
		rData.nFuncType=nFuncType;
		rData.dScaleFactor=dScaleFactor;
		rData.dTemperature=dTemperature;
		rData.dK=dK;
		rData.dAlpha=dAlpha;
		rData.nSeq=nSeq;
		rData.bUseConcData   = bUseConcData  ;
		rData.nCementType    = nCementType   ;
		rData.nTemperature   = nTemperature  ;
		rData.dCementContent = dCementContent;
		rData.nDataType      = nDataType     ;
	}
	void SetHsfc(const T_HSFC_K Key, const T_HSFC_D_CH& rData)
	{
		key=Key;
		memcpy(strFuncName, rData.strFuncName, sizeof(rData.strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dK=rData.dK;
		dAlpha=rData.dAlpha;
		nFuncDataCount=(int)rData.aFuncData.GetSize();
		nSeq=rData.nSeq;
		bUseConcData   = rData.bUseConcData  ;
		nCementType    = rData.nCementType   ;
		nTemperature   = rData.nTemperature  ;
		dCementContent = rData.dCementContent;
		nDataType      = rData.nDataType     ;
	}
	void Convert630(T_HSFC_UDRD_D_CH_RW_630& rData)
	{
		key = rData.key;
		memcpy(strFuncName, rData.strFuncName, sizeof(strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dK=rData.dK;
		dAlpha=rData.dAlpha;
		nFuncDataCount = rData.nFuncDataCount;
		nSeq=rData.nSeq;
		bUseConcData   = FALSE;
		nCementType    = 0;
		nTemperature   = 0;
		dCementContent = 0.;
		nDataType      = 0;
	}
	void Convert650(T_HSFC_UDRD_D_CH_RW_650& rData)
	{
		key = rData.key;
		memcpy(strFuncName, rData.strFuncName, sizeof(strFuncName));
		nFuncType=rData.nFuncType;
		dScaleFactor=rData.dScaleFactor;
		dTemperature=rData.dTemperature;
		dK=rData.dK;
		dAlpha=rData.dAlpha;
		nFuncDataCount = rData.nFuncDataCount;
		nSeq=rData.nSeq;
		bUseConcData   = rData.bUseConcData;
		nCementType    = rData.nCementType;
		nTemperature   = rData.nTemperature;
		dCementContent = rData.dCementContent;
		nDataType      = 0;
	}
};
struct T_HSFC_UNIT
{
	T_HSFC_UNIT()
	{
		strFuncName=D_UNITSYS_NONE;
		nFuncType=D_UNITSYS_NONE;
		dScaleFactor=D_UNITSYS_NONE;
		dTemperature=D_UNITSYS_BASE_HEATSOURCE;
		dK=D_UNITSYS_BASE_TEMPER;
		dAlpha=D_UNITSYS_NONE;
		dTime=D_UNITSYS_BASE_HOUR;
		dValue=D_UNITSYS_BASE_HEATSOURCE;
		nSeq=D_UNITSYS_NONE;
	}
	int strFuncName;
	int nFuncType;
	int dScaleFactor;
	int dTemperature;
	int dK;
	int dAlpha;
	int dTime;
	int dValue;
	int nSeq;
};
#define HASHSIZEHSFC 11
#define D_HSFC_MINNUM       1   // 
#define D_HSFC_MAXNUM    9999   // 

#define T_HAHS_K T_ELEM_K
struct T_HAHS_D
{
	T_HSFC_K nFuncKey;
	void Initialize()
	{
		nFuncKey=0;
	}
};
struct T_HAHS_UDRD_D
{
	T_HAHS_K key;
	T_HAHS_D data;
};
struct T_HAHS_UNIT
{
	T_HAHS_UNIT()
	{
		nFuncKey=D_UNITSYS_NONE;
	} 
	int nFuncKey;
};
#define HASHSIZEHAHS 1001

//#define T_HECB_K T_ELEM_K
#define T_HECB_KEY unsigned int
union T_HECB_K
{
	T_HECB_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576
		unsigned int serial : 12;     // 0-4096
	}key;
};

struct T_HECB_D_431
{
	T_CCFC_K  nCcfcKey;
	T_ETFC_K  nFuncKey;
	int       nFaceNo;
};

struct T_HECB_D
{
	T_CCFC_K  nCcfcKey;
	T_ETFC_K  nFuncKey;
	int       nFaceNo;
	T_BNGR_K  GroupKey;
	void Initialize()
	{
		nCcfcKey=0;
		nFuncKey=0;
		nFaceNo=0;
	}
	void Convert431(T_HECB_D_431& rData)
	{
		nCcfcKey = rData.nCcfcKey;
		nFuncKey = rData.nFuncKey;
		nFaceNo = rData.nFaceNo;
		GroupKey = 0;
	}
};
struct T_HECB_UDRD_D_431
{
	T_HECB_K key;
	T_HECB_D_431 data;
};
struct T_HECB_UDRD_D
{
	T_HECB_K key;
	T_HECB_D data;
};
struct T_HECB_UNIT
{
	T_HECB_UNIT()
	{
		nCcfcKey=D_UNITSYS_NONE;
		nFuncKey=D_UNITSYS_NONE;
		nFaceNo=D_UNITSYS_NONE;
	} 
	int nCcfcKey;
	int nFuncKey;
	int nFaceNo;
};
#define HASHSIZEHECB 1001

struct T_HMTP_BASE
{
	double dTime;
	double dValue;
	void Initialize()
	{
		dTime=0.0;
		dValue=0.0;
	}
};
#define T_HMTP_DT CArray<T_HMTP_BASE, T_HMTP_BASE&>
#define T_HMTP_K unsigned int
struct T_HMTP_D
{
	CString   strMatlName;
	int       nMatlType;      // =1 Constant =2 Code =3 User
	double    dScaleFactor;
	double    dSpecificHeat;      
	double    dHeatConduction;
	T_HMTP_DT aFuncData;      // nMatlType = User(3)
	CString   strCode;        // nMatlType = Code(2)
	double    dCompStrength;  // nMatlType = Code(2)
	double    dFactorA;       // nMatlType = Code(2) Korea Standard, ACI
	double    dFactorB;       // nMatlType = Code(2) Korea Standard, ACI
	int       nCementType;    // nMatlType = Code(2) CEB-FIP, Ohzagi (RS=1, N,R=2, SL=3, Fly-ash=4)
	unsigned int nSeq;        // 순번

	void Initialize()
	{
		strMatlName.Empty();
		nMatlType = 0;
		dScaleFactor = 1.0;
		dSpecificHeat = 0.0;
		dHeatConduction = 0.0;
		aFuncData.RemoveAll();
		strCode.Empty();
		dCompStrength = 0.0;
		dFactorA = 0.0;
		dFactorB = 0.0;
		nCementType = 1;
		nSeq = 0;
	}
	T_HMTP_D()
	{
	}
	T_HMTP_D(T_HMTP_D& src)
	{
		*this = src;
	}
	T_HMTP_D& operator=(T_HMTP_D& src)
	{
		strMatlName=src.strMatlName;
		nMatlType=src.nMatlType;
		dScaleFactor=src.dScaleFactor;
		dSpecificHeat=src.dSpecificHeat;
		dHeatConduction=src.dHeatConduction;
		aFuncData.Copy(src.aFuncData);
		strCode=src.strCode;
		dCompStrength=src.dCompStrength;
		dFactorA=src.dFactorA;
		dFactorB=src.dFactorB;
		nCementType=src.nCementType;
		nSeq=src.nSeq;
		return *this;
	}
};
struct T_HMTP_D_CH
{
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	double      dSpecificHeat;
	double      dHeatConduction;
	T_HMTP_DT   aFuncData;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	void ConvertToString(T_HMTP_D& rData)
	{
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.nMatlType=nMatlType;
		rData.dScaleFactor=dScaleFactor;
		rData.dSpecificHeat=dSpecificHeat;
		rData.dHeatConduction=dHeatConduction;
		rData.aFuncData.Copy(aFuncData);
		ConvertCharStr(strCode, rData.strCode, sizeof(strCode));
		rData.dCompStrength=dCompStrength;
		rData.dFactorA=dFactorA;
		rData.dFactorB=dFactorB;
		rData.nCementType=nCementType;
		rData.nSeq=nSeq;
	}
	void ConvertToChar(T_HMTP_D& rData)
	{
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		aFuncData.Copy(rData.aFuncData);
		ConvertStrChar(rData.strCode, strCode, sizeof(strCode));
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
	}
};
struct T_HMTP_UDRD_D
{
	T_HMTP_K key;
	T_HMTP_D data;
};
struct T_HMTP_UDRD_D_CH
{
	T_HMTP_K key;
	T_HMTP_D_CH data;
};
struct T_HMTP_UDRD_D_CH_RW
{
	T_HMTP_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	double      dSpecificHeat;
	double      dHeatConduction;
	int         nFuncDataCount;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	void GetHmtp(T_HMTP_K& rKey, T_HMTP_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strMatlName, strMatlName, sizeof(rData.strMatlName));
		rData.nMatlType=nMatlType;
		rData.dScaleFactor=dScaleFactor;
		rData.dSpecificHeat=dSpecificHeat;
		rData.dHeatConduction=dHeatConduction;
		memcpy(rData.strCode, strCode, sizeof(rData.strCode));
		rData.dCompStrength=dCompStrength;
		rData.dFactorA=dFactorA;
		rData.dFactorB=dFactorB;
		rData.nCementType=nCementType;
		rData.nSeq=nSeq;
	}
	void SetHmtp(const T_HMTP_K Key, const T_HMTP_D_CH& rData)
	{
		key=Key;
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		nFuncDataCount=(int)rData.aFuncData.GetSize();
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
	}
};
struct T_HMTP_UNIT
{
	T_HMTP_UNIT()
	{
		strMatlName=D_UNITSYS_NONE;
		nMatlType=D_UNITSYS_NONE;
		dScaleFactor=D_UNITSYS_NONE;
		dSpecificHeat=D_UNITSYS_BASE_SPHEAT;
		dHeatConduction=D_UNITSYS_BASE_CONDUCTION;
		dTime=D_UNITSYS_BASE_HOUR;
		dValue=D_UNITSYS_BASE_ELAST;
		strCode=D_UNITSYS_NONE;
		dCompStrength=D_UNITSYS_BASE_STRESS;
		dFactorA=D_UNITSYS_NONE;
		dFactorB=D_UNITSYS_NONE;
		nCementType=D_UNITSYS_NONE;
		nSeq=D_UNITSYS_NONE;
	}
	int strMatlName;
	int nMatlType;
	int dScaleFactor;
	int dSpecificHeat;
	int dHeatConduction;
	int dTime;
	int dValue;
	int strCode;
	int dCompStrength;
	int dFactorA;
	int dFactorB;
	int nCementType;
	int nSeq;
};
#define HASHSIZEHMTP 11
#define D_HMTP_MINNUM       1   // 
#define D_HMTP_MAXNUM    9999   // 

#define T_HMAT_K T_MATL_K
struct T_HMAT_D
{
	T_HMTP_K HyMatlTypeKey;
	void Initialize()
	{
		HyMatlTypeKey=0;
	}
};
struct T_HMAT_UDRD_D
{
	T_HMAT_K key;
	T_HMAT_D data;
};
struct T_HMAT_UNIT
{
	T_HMAT_UNIT()
	{
		HyMatlTypeKey=D_UNITSYS_NONE;
	}
	int HyMatlTypeKey;
};
#define HASHSIZEHMAT HASHSIZEMATL

/// hcrp ///
#define T_HCRP_K unsigned int
struct T_HCRP_D
{
	int    nCode;   // 1:ACI, 2:CEB, 3:Modify Elasticity Modulus
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage

	double dCompStrength;
	double dRelativeHumidity;

	int    nCuringMethod; // 1:moist cure, 2:steam cure
	double dVolumeSurfaceRatio;
	double dSlump;
	double dFineAggPercent;
	double dAirContent;
	int    nConcreteAge;
	double dCementContent;
	int    nMoistCuringPeriod;
	int    nIter;
	double dTol;

	double dMemberSize;
	int    nCementType; // 1:RS, 2:N,R, 3:SL

	int    nN1;         // nCode=3
	double dPhi1;
	int    nN2;
	double dPhi2;

	void Initialize()
	{
		nCode=0;
		nType=0;
		dCompStrength=0.0;
		dRelativeHumidity=0.0;
		nCuringMethod=0;
		dVolumeSurfaceRatio=0.0;
		dSlump=0.0;
		dFineAggPercent=0.0;
		dAirContent=0.0;
		nConcreteAge=0;
		dCementContent=0.0;
		nMoistCuringPeriod=0;
		nIter=5;
		dTol=0.001;
		dMemberSize=0.0;
		nCementType=0;
		nN1=3;
		dPhi1=0.73;
		nN2=5;
		dPhi2=1.0;
	}
};
struct T_HCRP_UDRD_D
{
	T_HCRP_D data;
};
struct T_HCRP_UNIT
{
	T_HCRP_UNIT()
	{
		nCode = D_UNITSYS_NONE;
		nType = D_UNITSYS_NONE;
		dCompStrength = D_UNITSYS_BASE_STRESS;
		dRelativeHumidity = D_UNITSYS_NONE;
		nCuringMethod = D_UNITSYS_NONE;
		dVolumeSurfaceRatio = D_UNITSYS_BASE_LENGTH;
		dSlump = D_UNITSYS_BASE_LENGTH;
		dFineAggPercent = D_UNITSYS_NONE;
		nConcreteAge = D_UNITSYS_BASE_DAY;
		dCementContent = D_UNITSYS_BASE_DENSITY;
		nMoistCuringPeriod = D_UNITSYS_BASE_DAY;
		nIter = D_UNITSYS_NONE;
		dTol = D_UNITSYS_NONE;
		dMemberSize = D_UNITSYS_BASE_LENGTH;
		nCementType = D_UNITSYS_NONE;
		nN1=D_UNITSYS_BASE_DAY;
		dPhi1=D_UNITSYS_NONE;
		nN2=D_UNITSYS_BASE_DAY;
		dPhi2=D_UNITSYS_NONE;
	}
	int nCode;
	int nType;

	int dCompStrength;
	int dRelativeHumidity;

	int nCuringMethod;
	int dVolumeSurfaceRatio;
	int dSlump;
	int dFineAggPercent;
	int dAirContent;
	int nConcreteAge;
	int dCementContent;
	int nMoistCuringPeriod;
	int nIter;
	int dTol;

	int dMemberSize;
	int nCementType;

	int nN1;
	int dPhi1;
	int nN2;
	int dPhi2;
};
#define HASHSIZEHCRP 1

// HSTG
#define T_HSTG_K unsigned int
struct T_HSTG_LOAD_BASE
{
	T_LDGR_K LdgrKey;
	double   dDay;    // 하중이 재하되는 시점 (0.0 First, -1.0:Last, etc:User-Step) : 수화열에서는 First, Last 사용 않음
	void Initialize()
	{
		LdgrKey = 0;
		dDay = 0.0;
	}
};
struct T_HSTG_D_650
{
	CString StageName;
	CArray<double, double> aStep; // 단위:hour
	CArray<T_GRUP_K, T_GRUP_K> aActiveElem;
	CArray<T_BNGR_K, T_BNGR_K> aActiveBndr;
	CArray<T_BNGR_K, T_BNGR_K> aDeactiveBndr;
	unsigned int nStageId;
};
struct T_HSTG_D_750
{
	CString StageName;  
	CArray<double, double> aStep; // 단위:hour
	CArray<T_GRUP_K, T_GRUP_K> aActiveElem;
	CArray<T_BNGR_K, T_BNGR_K> aActiveBndr;
	CArray<T_BNGR_K, T_BNGR_K> aDeactiveBndr;
	CArray<T_HSTG_LOAD_BASE, T_HSTG_LOAD_BASE&> aActiveLoad;     // 6.6.0추가
	CArray<T_HSTG_LOAD_BASE, T_HSTG_LOAD_BASE&> aDeactiveLoad;   // 6.6.0추가
	double dInitTemperature;      // 6.6.0추가 
	unsigned int nStageId;
};
struct T_HSTG_D
{
	CString StageName;  
	CArray<double, double> aStep; // 단위:hour
	CArray<T_GRUP_K, T_GRUP_K> aActiveElem;
	CArray<T_BNGR_K, T_BNGR_K> aActiveBndr;
	CArray<T_BNGR_K, T_BNGR_K> aDeactiveBndr;
	CArray<T_HSTG_LOAD_BASE, T_HSTG_LOAD_BASE&> aActiveLoad;     // 6.6.0추가
	CArray<T_HSTG_LOAD_BASE, T_HSTG_LOAD_BASE&> aDeactiveLoad;   // 6.6.0추가
	double dInitTemperature;      // 6.6.0추가 
	BOOL bInitTemperature;        // 7.6.0추가
	unsigned int nStageId;
	void Initialize()
	{
		StageName.Empty();
		aStep.RemoveAll();
		aActiveElem.RemoveAll();
		aActiveBndr.RemoveAll();
		aDeactiveBndr.RemoveAll();
		aActiveLoad.RemoveAll();
		aDeactiveLoad.RemoveAll();
		dInitTemperature = 20.;
		bInitTemperature = FALSE;
		nStageId = 0;
	}
	T_HSTG_D() {}
	T_HSTG_D(const T_HSTG_D& src)
	{
		*this = src;
	}
	T_HSTG_D& operator=(const T_HSTG_D& src)
	{
		StageName = src.StageName;
		aStep.Copy(src.aStep);
		aActiveElem.Copy(src.aActiveElem);
		aActiveBndr.Copy(src.aActiveBndr);
		aDeactiveBndr.Copy(src.aDeactiveBndr);
		aActiveLoad.Copy(src.aActiveLoad);
		aDeactiveLoad.Copy(src.aDeactiveLoad);
		dInitTemperature = src.dInitTemperature;
		bInitTemperature = src.bInitTemperature;
		nStageId = src.nStageId;
		return *this;
	}  
};
/*
struct T_HSTG_D_CH_650
{
	char StageName[80];
	CArray<double, double> aStep; // 단위:hour
	CArray<T_GRUP_K, T_GRUP_K> aActiveElem;
	CArray<T_BNGR_K, T_BNGR_K> aActiveBndr;
	CArray<T_BNGR_K, T_BNGR_K> aDeactiveBndr;
	unsigned int nStageId;
};
*/
struct T_HSTG_D_CH
{
	char StageName[80];
	CArray<double, double> aStep; // 단위:hour
	CArray<T_GRUP_K, T_GRUP_K> aActiveElem;
	CArray<T_BNGR_K, T_BNGR_K> aActiveBndr;
	CArray<T_BNGR_K, T_BNGR_K> aDeactiveBndr;
	CArray<T_HSTG_LOAD_BASE, T_HSTG_LOAD_BASE&> aActiveLoad;     // 6.6.0추가
	CArray<T_HSTG_LOAD_BASE, T_HSTG_LOAD_BASE&> aDeactiveLoad;   // 6.6.0추가
	double dInitTemperature;      // 6.6.0추가 
	BOOL bInitTemperature;        // 7.6.0추가 
	unsigned int nStageId;
	void ConvertToString(T_HSTG_D& rData)
	{
		ConvertCharStr(StageName, rData.StageName, sizeof(StageName));
		rData.aStep.Copy(aStep);
		rData.aActiveElem.Copy(aActiveElem);
		rData.aActiveBndr.Copy(aActiveBndr);
		rData.aDeactiveBndr.Copy(aDeactiveBndr);
		rData.aActiveLoad.Copy(aActiveLoad);
		rData.aDeactiveLoad.Copy(aDeactiveLoad);
		rData.dInitTemperature = dInitTemperature;
		rData.bInitTemperature = bInitTemperature;
		rData.nStageId = nStageId;    
	}
	void ConvertToChar(T_HSTG_D& rData)
	{
		ConvertStrChar(rData.StageName, StageName, sizeof(StageName));
		aStep.Copy(rData.aStep);
		aActiveElem.Copy(rData.aActiveElem);
		aActiveBndr.Copy(rData.aActiveBndr);
		aDeactiveBndr.Copy(rData.aDeactiveBndr);
		aActiveLoad.Copy(rData.aActiveLoad);
		aDeactiveLoad.Copy(rData.aDeactiveLoad);
		dInitTemperature = rData.dInitTemperature;
		bInitTemperature = rData.bInitTemperature;
		nStageId = rData.nStageId;
	}
};

struct T_HSTG_UDRD_D
{
	T_HSTG_K key;
	T_HSTG_D data;
};
/*
struct T_HSTG_UDRD_D_CH_650
{
	T_HSTG_K key;
	T_HSTG_D_CH_650 data;
};
*/
struct T_HSTG_UDRD_D_CH
{
	T_HSTG_K key;
	T_HSTG_D_CH data;
};
struct T_HSTG_UDRD_D_CH_RW_650
{
	T_HSTG_K key;
	char StageName[80];
	int  nCountStep;
	int  nCountActiveElem;
	int  nCountActiveBndr;
	int  nCountDeactiveBndr;
	unsigned int nStageId;
};
struct T_HSTG_UDRD_D_CH_RW_750
{
	T_HSTG_K key;
	char StageName[80];
	int  nCountStep;
	int  nCountActiveElem;
	int  nCountActiveBndr;
	int  nCountDeactiveBndr;
	int  nCountActiveLoad;    // 6.6.0추가 
	int  nCountDeactiveLoad;  // 6.6.0추가 
	double dInitTemperature;  // 6.6.0추가 
	unsigned int nStageId;
};
struct T_HSTG_UDRD_D_CH_RW
{
	T_HSTG_K key;
	char StageName[80];
	int  nCountStep;
	int  nCountActiveElem;
	int  nCountActiveBndr;
	int  nCountDeactiveBndr;
	int  nCountActiveLoad;    // 6.6.0추가 
	int  nCountDeactiveLoad;  // 6.6.0추가 
	double dInitTemperature;  // 6.6.0추가 
	BOOL bInitTemperature;        // 7.6.0추가 
	unsigned int nStageId;
	void GetHstg(T_HSTG_K& rKey, T_HSTG_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.StageName, StageName, sizeof(StageName));
		rData.dInitTemperature = dInitTemperature;
		rData.bInitTemperature = bInitTemperature;
		rData.nStageId = nStageId;
	}
	void SetHstg(const T_HSTG_K Key, const T_HSTG_D_CH& rData)
	{
		key = Key;
		memcpy(StageName, rData.StageName, sizeof(StageName));
		nCountStep         = (int)rData.aStep.GetSize();
		nCountActiveElem   = (int)rData.aActiveElem.GetSize();
		nCountActiveBndr   = (int)rData.aActiveBndr.GetSize();
		nCountDeactiveBndr = (int)rData.aDeactiveBndr.GetSize();
		nCountActiveLoad   = (int)rData.aActiveLoad.GetSize();
		nCountDeactiveLoad = (int)rData.aDeactiveLoad.GetSize();
		dInitTemperature   = rData.dInitTemperature;
		bInitTemperature   = rData.bInitTemperature;
		nStageId = rData.nStageId;
	}
	void Convert650(T_HSTG_UDRD_D_CH_RW_650& rData)
	{
		key = rData.key;
		memcpy(StageName, rData.StageName, sizeof(StageName));
		nCountStep         = rData.nCountStep;
		nCountActiveElem   = rData.nCountActiveElem;
		nCountActiveBndr   = rData.nCountActiveBndr;
		nCountDeactiveBndr = rData.nCountDeactiveBndr;
		nCountActiveLoad   = 0;
		nCountDeactiveLoad = 0;
		dInitTemperature   = 20.;
		nStageId = rData.nStageId;
	}
	void Convert750(T_HSTG_UDRD_D_CH_RW_750& rData)
	{
		key = rData.key;
		memcpy(StageName, rData.StageName, sizeof(StageName));
		nCountStep         = rData.nCountStep;
		nCountActiveElem   = rData.nCountActiveElem;
		nCountActiveBndr   = rData.nCountActiveBndr;
		nCountDeactiveBndr = rData.nCountDeactiveBndr;
		nCountActiveLoad   = rData.nCountActiveLoad;
		nCountDeactiveLoad = rData.nCountDeactiveLoad;
		dInitTemperature   = rData.dInitTemperature;
		bInitTemperature   = TRUE;
		nStageId = rData.nStageId;
	}
};
struct T_HSTG_UNIT
{
	T_HSTG_UNIT()
	{
		aStep = D_UNITSYS_BASE_HOUR;
	}
	int aStep;
};
#define HASHSIZEHSTG 101
#define D_HSTG_STAG_START_KEY 100000

/// hhct ///
#define T_HHCT_K unsigned int
struct T_HHCT_D_431
{
	double dTheta;
	double dInitTemp;
	int    nEvaluation;   // 1:Center, 2:Gauss Point, 3:Nodal Point
	CArray<double, double> aTime;
	void Initialize()
	{
		dTheta = 0.5;
		dInitTemp = 20.0;
		nEvaluation = 2;
		aTime.RemoveAll();
	}
};
struct T_HHCT_D_610
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;   // 1:Center, 2:Gauss Point, 3:Nodal Point
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;
	double dTol;
};
struct T_HHCT_D_630
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;   // 1:Center, 2:Gauss Point, 3:Nodal Point
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;
	double dTol;
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
};
struct T_HHCT_D_641
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;   // 1:Center, 2:Gauss Point, 3:Nodal Point
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;              // nCreepCalcMethod == 0(General) 인 경우 사용
	double dTol;               // nCreepCalcMethod == 0(General) 인 경우 사용
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
	int    nCreepCalcMethod;   // 6.4.0 부터 추가  0=General, 1=Effective Modulus
	double dPhi1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	double dPhi2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	int    nDay1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용  
	int    nDay2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용 
};
struct T_HHCT_D_960
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;   // 1:Center, 2:Gauss Point, 3:Nodal Point
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;              // nCreepCalcMethod == 0(General) 인 경우 사용
	double dTol;               // nCreepCalcMethod == 0(General) 인 경우 사용
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
	int    nCreepCalcMethod;   // 6.4.0 부터 추가  0=General, 1=Effective Modulus
	double dPhi1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	double dPhi2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	int    nDay1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용  
	int    nDay2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용 
	BOOL   bUseEquivalentAge;  // 6.5.0 부터 추가 
};
struct T_HHCT_D
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;   // 1:Center, 2:Gauss Point, 3:Nodal Point
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;              // nCreepCalcMethod == 0(General) 인 경우 사용 - NS 버전부터 사용 안함
	double dTol;               // nCreepCalcMethod == 0(General) 인 경우 사용 - NS 버전부터 사용 안함
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
	int    nCreepCalcMethod;   // 6.4.0 부터 추가  0=General, 1=Effective Modulus
	double dPhi1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	double dPhi2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	int    nDay1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용  
	int    nDay2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용 
	BOOL   bUseEquivalentAge;  // 6.5.0 부터 추가

	int	   nMaxIterPerIncre;
	BOOL   bConv[3];		   // 0 : Displacement, 1 : Load, 2 : Work
	double dConv[3];		   // 0 : Displacement, 1 : Load, 2 : Work

	void Initialize()
	{
		nFinalStage = 0;
		FinalHstgKey = 0;
		dTheta = 1.0;
		dInitTemp = 20.0;
		nEvaluation = 2;
		bCreepShrinkage = TRUE; // 일본 요구로 Default값 변경 2004.5.11
		nType = 3;                         // 일본 요구로 Default값 변경 2004.5.11
		nIter = 5;
		dTol = 0.001;
		bIncludeSelfWeight = FALSE;
		dSelfWeightFactor = -1.0;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
		bUseEquivalentAge = TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	T_HHCT_D(){}
	T_HHCT_D(T_HHCT_D& src){*this = src;}
	T_HHCT_D& operator=(T_HHCT_D& src)
	{
		nFinalStage = src.nFinalStage;
		FinalHstgKey = src.FinalHstgKey;
		dTheta=src.dTheta;
		dInitTemp=src.dInitTemp;
		nEvaluation=src.nEvaluation;
		bCreepShrinkage=src.bCreepShrinkage;
		nType=src.nType;
		nIter=src.nIter;
		dTol=src.dTol;
		bIncludeSelfWeight= src.bIncludeSelfWeight;
		dSelfWeightFactor = src.dSelfWeightFactor;
		nCreepCalcMethod = src.nCreepCalcMethod;
		dPhi1 = src.dPhi1;           
		dPhi2 = src.dPhi2;
		nDay1 = src.nDay1; 
		nDay2 = src.nDay2;
		bUseEquivalentAge = src.bUseEquivalentAge;
		nMaxIterPerIncre = src.nMaxIterPerIncre;
		bConv[0] = src.bConv[0];
		bConv[1] = src.bConv[1];
		bConv[2] = src.bConv[2];
		dConv[0] = src.dConv[0];
		dConv[1] = src.dConv[1];
		dConv[2] = src.dConv[2];
		return *this;
	}
	void Convert431(T_HHCT_D_431& rData)
	{
		nFinalStage = 0;
		FinalHstgKey = 0;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		bCreepShrinkage = FALSE;
		nType = 1;
		nIter = 5;
		dTol = 0.001;
		bIncludeSelfWeight = FALSE;
		dSelfWeightFactor = -1.0;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert610(T_HHCT_D_610& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = FALSE;
		dSelfWeightFactor = -1.0;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert630(T_HHCT_D_630& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = rData.bIncludeSelfWeight;
		dSelfWeightFactor = rData.dSelfWeightFactor;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert641(T_HHCT_D_641& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = rData.bIncludeSelfWeight;
		dSelfWeightFactor = rData.dSelfWeightFactor;
		nCreepCalcMethod = rData.nCreepCalcMethod;
		dPhi1 = rData.dPhi1;           
		dPhi2 = rData.dPhi2;           
		nDay1 = rData.nDay1;           
		nDay2 = rData.nDay2;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert960(T_HHCT_D_960 rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = rData.bIncludeSelfWeight;
		dSelfWeightFactor = rData.dSelfWeightFactor;
		nCreepCalcMethod = rData.nCreepCalcMethod;
		dPhi1 = rData.dPhi1;
		dPhi2 = rData.dPhi2;
		nDay1 = rData.nDay1;
		nDay2 = rData.nDay2;
		bUseEquivalentAge = rData.bUseEquivalentAge;;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
};
struct T_HHCT_UDRD_D
{
	T_HHCT_D data;
};
struct T_HHCT_UDRD_D_RW_431
{
	T_HHCT_K key;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;
	unsigned int nNumTime;
};
struct T_HHCT_UDRD_D_RW_610
{
	T_HHCT_K key;
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;
	unsigned int nNumTime;
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;
	double dTol;
};
struct T_HHCT_UDRD_D_RW_630
{
	T_HHCT_K key;
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;
	unsigned int nNumTime;
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;
	double dTol;
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
};
struct T_HHCT_UDRD_D_RW_641
{
	T_HHCT_K key;
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;
	unsigned int nNumTime;
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;
	double dTol;
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
	int    nCreepCalcMethod;   // 6.4.0 부터 추가  0=General, 1=Effective Modulus
	double dPhi1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	double dPhi2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	int    nDay1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용  
	int    nDay2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용   
};
struct T_HHCT_UDRD_D_RW_960
{
	T_HHCT_K key;
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;
	unsigned int nNumTime;
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;
	double dTol;
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
	int    nCreepCalcMethod;   // 6.4.0 부터 추가  0=General, 1=Effective Modulus
	double dPhi1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	double dPhi2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	int    nDay1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용  
	int    nDay2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용   
	BOOL   bUseEquivalentAge;
};
struct T_HHCT_UDRD_D_RW
{
	T_HHCT_K key;
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_HSTG_K  FinalHstgKey;
	double dTheta;
	double dInitTemp;
	int    nEvaluation;
	unsigned int nNumTime;
	BOOL   bCreepShrinkage;
	int    nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int    nIter;
	double dTol;
	BOOL   bIncludeSelfWeight;
	double dSelfWeightFactor;
	int    nCreepCalcMethod;   // 6.4.0 부터 추가  0=General, 1=Effective Modulus
	double dPhi1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	double dPhi2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용
	int    nDay1;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용  
	int    nDay2;              // 6.4.0 부터 추가  nCreepCalcMethod == 1(Effective Modulus) 인 경우 사용   
	BOOL   bUseEquivalentAge;
	int	   nMaxIterPerIncre;
	BOOL   bConv[3];		   // 0 : Displacement, 1 : Load, 2 : Work
	double dConv[3];		   // 0 : Displacement, 1 : Load, 2 : Work

	void GetHhct(T_HHCT_K& rKey, T_HHCT_D& rData)
	{
		rKey=key;
		rData.nFinalStage = nFinalStage;
		rData.FinalHstgKey = FinalHstgKey;
		rData.dTheta = dTheta;
		rData.dInitTemp = dInitTemp;
		rData.nEvaluation = nEvaluation;
		rData.bCreepShrinkage = bCreepShrinkage;
		rData.nType = nType;
		rData.nIter = nIter;
		rData.dTol = dTol;
		rData.bIncludeSelfWeight = bIncludeSelfWeight;
		rData.dSelfWeightFactor = dSelfWeightFactor;
		rData.nCreepCalcMethod = nCreepCalcMethod;
		rData.dPhi1 = dPhi1;           
		rData.dPhi2 = dPhi2;           
		rData.nDay1 = nDay1;           
		rData.nDay2 = nDay2;   
		rData.bUseEquivalentAge = bUseEquivalentAge;
		rData.nMaxIterPerIncre = nMaxIterPerIncre;
		rData.bConv[0] = bConv[0];
		rData.bConv[1] = bConv[1];
		rData.bConv[2] = bConv[2];
		rData.dConv[0] = dConv[0];
		rData.dConv[1] = dConv[1];
		rData.dConv[2] = dConv[2];
	}
	void SetHhct(const T_HHCT_K Key, const T_HHCT_D& rData)
	{
		key=Key;
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		nNumTime = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = rData.bIncludeSelfWeight;
		dSelfWeightFactor = rData.dSelfWeightFactor;
		nCreepCalcMethod = rData.nCreepCalcMethod;
		dPhi1 = rData.dPhi1;           
		dPhi2 = rData.dPhi2;           
		nDay1 = rData.nDay1;           
		nDay2 = rData.nDay2;   
		bUseEquivalentAge = rData.bUseEquivalentAge;
		nMaxIterPerIncre = rData.nMaxIterPerIncre;
		bConv[0] = rData.bConv[0];
		bConv[1] = rData.bConv[1];
		bConv[2] = rData.bConv[2];
		dConv[0] = rData.dConv[0];
		dConv[1] = rData.dConv[1];
		dConv[2] = rData.dConv[2];
	}
	void Convert431(T_HHCT_UDRD_D_RW_431& rData)
	{
		key=rData.key;
		nFinalStage = 0;
		FinalHstgKey = 0;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		nNumTime = 0;
		bCreepShrinkage = FALSE;
		nType = 1;
		nIter = 5;
		dTol = 0.001;
		bIncludeSelfWeight = FALSE;
		dSelfWeightFactor = -1.0;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert610(T_HHCT_UDRD_D_RW_610& rData)
	{
		key=rData.key;
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		nNumTime = rData.nNumTime;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = FALSE;
		dSelfWeightFactor = -1.0;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert630(T_HHCT_UDRD_D_RW_630& rData)
	{
		key=rData.key;
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		nNumTime = rData.nNumTime;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = rData.bIncludeSelfWeight;
		dSelfWeightFactor = rData.dSelfWeightFactor;
		nCreepCalcMethod = 0;
		dPhi1 = 0.73;           
		dPhi2 = 1.0;           
		nDay1 = 3;           
		nDay2 = 5;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert641(T_HHCT_UDRD_D_RW_641& rData)
	{
		key=rData.key;
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		nNumTime = rData.nNumTime;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = rData.bIncludeSelfWeight;
		dSelfWeightFactor = rData.dSelfWeightFactor;
		nCreepCalcMethod = rData.nCreepCalcMethod;
		dPhi1 = rData.dPhi1;           
		dPhi2 = rData.dPhi2;           
		nDay1 = rData.nDay1;           
		nDay2 = rData.nDay2;
		bUseEquivalentAge= TRUE;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
	void Convert960(T_HHCT_UDRD_D_RW_960 rData)
	{
		key = rData.key;
		nFinalStage = rData.nFinalStage;
		FinalHstgKey = rData.FinalHstgKey;
		dTheta = rData.dTheta;
		dInitTemp = rData.dInitTemp;
		nEvaluation = rData.nEvaluation;
		nNumTime = rData.nNumTime;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bIncludeSelfWeight = rData.bIncludeSelfWeight;
		dSelfWeightFactor = rData.dSelfWeightFactor;
		nCreepCalcMethod = rData.nCreepCalcMethod;
		dPhi1 = rData.dPhi1;
		dPhi2 = rData.dPhi2;
		nDay1 = rData.nDay1;
		nDay2 = rData.nDay2;
		bUseEquivalentAge = rData.bUseEquivalentAge;
		nMaxIterPerIncre = 50;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
	}
};
struct T_HHCT_UNIT
{
	T_HHCT_UNIT()
	{
		dTheta = D_UNITSYS_NONE;
		dInitTemp = D_UNITSYS_BASE_TEMPER;
		nEvaluation = D_UNITSYS_NONE;
	}
	int dTheta;
	int dInitTemp;
	int nEvaluation;
};
#define HASHSIZEHHCT 1

#pragma pack(pop)

#endif  // __DB_ST_DT_HYDR_H__
