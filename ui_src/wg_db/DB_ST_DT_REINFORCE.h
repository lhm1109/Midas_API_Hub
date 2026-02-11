#pragma once
//#include "DB_ST_DT_SPEC.h"
//#include "DB_ST_DT_SEIS.h"
//#include "DB_ST_DN_JFD.h"
//#include "DB_ST_DT_SPEC.h"
//#include "DB_ST_DN.h"
#define MIDAS_SDK

#pragma pack(push, 1)
#pragma warning ( disable : 4819 )
#pragma warning ( disable : 4244 )

/************************************************************************/
/*      Initial deflection:T_INDF_D										*/
/************************************************************************/
#define T_INDF_K T_ELEM_K
#define HASHSIZEINDF HASHSIZEELEM
struct T_INDF_D
{
	double dOmegaWy;
	double dOmegaWz;

	char ReserveBuffer[40];

	void Initialize()
	{
		dOmegaWy = 0.0;
		dOmegaWz = 0.0;
		memset(ReserveBuffer, 0, sizeof(ReserveBuffer));
	}
	T_INDF_D()
	{
		Initialize();
	}
	T_INDF_D(const T_INDF_D& src)
	{
		T_INDF_D(); *this = src;
	}
	T_INDF_D& T_INDF_D::operator=(const T_INDF_D& src)
	{
		dOmegaWy = src.dOmegaWy;
		dOmegaWz = src.dOmegaWz;
		memcpy(ReserveBuffer, src.ReserveBuffer, sizeof(ReserveBuffer));
		return *this;
	}
};
struct T_INDF_UDRD_D
{
	T_INDF_K key;
	T_INDF_D data;
};

/************************************************************************/
/*      ReinForce Check Total Info:T_RCTI_D -discard-                   */
/************************************************************************/
#define T_RCTI_K unsigned int
#define HASHSIZERCTI 1
struct T_RCTI_D
{
	CString strAssessCode;
	int nBuildingType;
	CString strDesingCode;

	char ReserveBuffer[120];

	void Initialize()
	{
		strAssessCode.Empty();
		nBuildingType = 0;
		strDesingCode.Empty();
		memset(ReserveBuffer, 0, sizeof(ReserveBuffer));
	}
	T_RCTI_D() { Initialize(); }
	T_RCTI_D(const T_RCTI_D& src) { T_RCTI_D(); *this = src; }
	T_RCTI_D& T_RCTI_D::operator=(const T_RCTI_D&src)
	{
		strAssessCode = src.strAssessCode;
		nBuildingType = src.nBuildingType;
		strDesingCode = src.strDesingCode;
		memcpy(ReserveBuffer, src.ReserveBuffer, sizeof(ReserveBuffer));
		return *this;
	}
};
struct T_RCTI_D_CH
{
	char strAssessCode[40];
	int nBuildingType;
	char strDesingCode[40];

	char ReserveBuffer[120];

	void ConvertToString(T_RCTI_D& rData)
	{
		ConvertCharStr(strAssessCode, rData.strAssessCode, sizeof(strAssessCode));
		rData.nBuildingType = nBuildingType;
		ConvertCharStr(strDesingCode, rData.strDesingCode, sizeof(strDesingCode));
		memcpy(rData.ReserveBuffer, ReserveBuffer, sizeof(ReserveBuffer));
	}
	void ConvertToChar(T_RCTI_D& rData)
	{
		ConvertStrChar(rData.strAssessCode, strAssessCode, sizeof(strAssessCode));
		rData.nBuildingType = nBuildingType;
		ConvertStrChar(rData.strDesingCode, strDesingCode, sizeof(strDesingCode));
		memcpy(ReserveBuffer, rData.ReserveBuffer, sizeof(ReserveBuffer));
	}
};
struct T_RCTI_UDRD_D
{
	T_RCTI_D data;
};

/************************************************************************/
/*      ReinForce Design Parameter:T_RDPA_D                            */
/************************************************************************/
#define T_RDPA_K unsigned int
#define HASHSIZERDPA 1
struct T_RDPA_D
{
	int nReinForceType;
	int nStructureCategory;
	T_DSTL_D stlData;

	char ReserveBuffer[40];

	void Initialize()
	{
		nReinForceType = 1;
		nStructureCategory = 2;
		stlData.Initialize();
		memset(ReserveBuffer, 0, sizeof(ReserveBuffer));
	}
	T_RDPA_D() { Initialize(); }
	T_RDPA_D(const T_RDPA_D& src) { T_RDPA_D(); *this = src; }
	T_RDPA_D& T_RDPA_D::operator=(const T_RDPA_D& src)
	{
		nReinForceType = src.nReinForceType;
		nStructureCategory = src.nStructureCategory;
		stlData = src.stlData;
		memcpy(ReserveBuffer, src.ReserveBuffer, sizeof(ReserveBuffer));
		return *this;
	}
};
struct T_RDPA_D_CH
{
	int nReinForceType;
	int nStructureCategory;
	T_DSTL_D_CH stlData;
	char ReserveBuffer[40];
	void ConvertToChar(T_RDPA_D& src)
	{
		nReinForceType = src.nReinForceType;
		nStructureCategory = src.nStructureCategory;
		stlData.ConvertToChar(src.stlData);

	}
	void ConvertToString(T_RDPA_D& src)
	{
		src.nReinForceType = nReinForceType;
		src.nStructureCategory = nStructureCategory;
		stlData.ConvertToString(src.stlData);
	}
};

struct T_RDPA_UDRD_D
{
	T_RDPA_D data;
};

/************************************************************************/
/*      ReinForce Sect:T_RFST_D                            */
/************************************************************************/
//截面焊缝坐标
struct T_RFST_WELD_SUB_D
{
	BOOL bAuto;
	double dy;
	double dz;
	double dLength;	//长度
	double dSpace;	//间隔
};
//焊缝性质
struct T_RFST_WELD_D
{
	//nType : 0 连续焊缝, 1间隔焊缝
	int nType;
	double dHf;
	double dLength;
	double dSpace;
	CArray<T_RFST_WELD_SUB_D, T_RFST_WELD_SUB_D&> aWeldCoordinate;
	char ReserveBuffer[40];
	void Initialize()
	{
		nType = 0;
		dHf = 0.006;
		dLength = 0.4;
		dSpace = 0.2;
		aWeldCoordinate.RemoveAll();
		memset(ReserveBuffer, 0, sizeof(ReserveBuffer));
	}
	T_RFST_WELD_D() { Initialize(); }
	T_RFST_WELD_D(const T_RFST_WELD_D& src) { T_RFST_WELD_D(); *this = src; }
	T_RFST_WELD_D& T_RFST_WELD_D::operator=(const T_RFST_WELD_D& src)
	{
		nType = src.nType;
		dHf = src.dHf;
		dLength = src.dLength;
		dSpace = src.dSpace;
		aWeldCoordinate.Copy(src.aWeldCoordinate);
		memcpy(ReserveBuffer, src.ReserveBuffer, sizeof(ReserveBuffer));
		return *this;
	}
};
struct T_RFST_WELD_RDWR_D
{
	int nType;
	double dHf;
	double dLength;
	double dSpace;
	int nWeldCoordinate;
	char ReserveBuffer[40];
	void Set_T_RFST_WELD_D(T_RFST_WELD_D& src)const
	{
		src.nType = nType;
		src.dHf = dHf;
		src.dLength = dLength;
		src.dSpace = dSpace;
		src.aWeldCoordinate.SetSize(nWeldCoordinate);
		memcpy(src.ReserveBuffer, ReserveBuffer, sizeof(ReserveBuffer));

	}
	void Get_T_RFST_WELD_D(const T_RFST_WELD_D& src)
	{
		nType = src.nType;
		dHf = src.dHf;
		dLength = src.dLength;
		dSpace = src.dSpace;
		nWeldCoordinate = src.aWeldCoordinate.GetSize();
		memcpy(ReserveBuffer, src.ReserveBuffer, sizeof(ReserveBuffer));
	}
};

#define T_RFST_K unsigned int
#define T_RFST_K_LIST CArray<UINT, UINT>
#define HASHSIZERFST 101
struct T_RFST_D
{
	//nMatlK:0 同原单元的材料
	//nReinforceType:加固形式; 0-6对应加固形式一到加固形式六,
	//nConnectionType 加固连接方式0-2:螺栓连接 铆钉连接 焊接
	//nSideType 加固方式0-1:单边加固 双侧加固
	//bAutoCalcDeflection:自动计算残余焊接残余挠度
	//Sect[6]:暂时只有6个数据
	//ReserveBuffer:预留变量
	CString strReinforceSectName;
	T_SECT_K nOriginSectK;
	T_MATL_K nMatlK;
	int	nReinforceType;
	int nConnectionType;
	int nSideType;
	BOOL bAutoCalcDeflection;
	double dWeldOmegaY;
	double dWeldOmegaZ;
	T_RFST_WELD_D mWeldData;
	double Sect[6];
	T_SECT_STIFFNESS Stiffness;
	/*加固形式是任意截面时,选择的截面Key*/
	T_SECT_K nPSCSectK;
	char ReserveBuffer[120];

	void Initialize()
	{
		strReinforceSectName.Empty();
		nOriginSectK = 0;
		nMatlK = 0;
		nReinforceType = 0;
		nConnectionType = 1.0;
		nSideType = 1.0;
		bAutoCalcDeflection = FALSE;
		dWeldOmegaY = 0.0;
		dWeldOmegaZ = 0.0;
		mWeldData.Initialize();
		memset(Sect, 0, sizeof(Sect));
		Stiffness.Initialize();
		nPSCSectK = 0;
		memset(ReserveBuffer, 0, sizeof(ReserveBuffer));
	}
	T_RFST_D() { Initialize(); }
	T_RFST_D(const T_RFST_D& src) { T_RFST_D(); *this = src; }
	T_RFST_D& T_RFST_D::operator=(const T_RFST_D& src)
	{
		strReinforceSectName = src.strReinforceSectName;
		nOriginSectK = src.nOriginSectK;
		nMatlK = src.nMatlK;
		nReinforceType = src.nReinforceType;
		nConnectionType = src.nConnectionType;
		nSideType = src.nSideType;
		bAutoCalcDeflection = src.bAutoCalcDeflection;
		dWeldOmegaY = src.dWeldOmegaY;
		dWeldOmegaZ = src.dWeldOmegaZ;
		mWeldData = src.mWeldData;
		memcpy(Sect, src.Sect, sizeof(Sect));
		Stiffness = src.Stiffness;
		nPSCSectK = src.nPSCSectK;
		memcpy(ReserveBuffer, src.ReserveBuffer, sizeof(ReserveBuffer));
		return *this;
	}
};
struct T_RFST_D_CH_RW
{
	char strReinforceSectName[40];
	T_SECT_K nOriginSectK;
	T_MATL_K nMatlK;
	int	nReinforceType;
	int nConnectionType;
	int nSideType;
	BOOL bAutoCalcDeflection;
	double dWeldOmegaY;
	double dWeldOmegaZ;
	double dNet2GrossModulusRatioZ;
	T_RFST_WELD_RDWR_D mWeldData;
	double Sect[6];
	T_SECT_STIFFNESS Stiffness;
	T_SECT_K nPSCSectK;
	char ReserveBuffer[120];

	void ConvertToString(T_RFST_D& rData)
	{
		ConvertCharStr(strReinforceSectName, rData.strReinforceSectName, sizeof(strReinforceSectName));
		rData.nOriginSectK = nOriginSectK;
		rData.nMatlK = nMatlK;
		rData.nReinforceType = nReinforceType;
		rData.nConnectionType = nConnectionType;
		rData.nSideType = nSideType;
		rData.bAutoCalcDeflection = bAutoCalcDeflection;
		rData.dWeldOmegaY = dWeldOmegaY;
		rData.dWeldOmegaZ = dWeldOmegaZ;
		mWeldData.Set_T_RFST_WELD_D(rData.mWeldData);
		memcpy(rData.Sect, Sect, sizeof(Sect));
		rData.Stiffness = Stiffness;
		rData.nPSCSectK = nPSCSectK;
		memcpy(rData.ReserveBuffer, ReserveBuffer, sizeof(ReserveBuffer));
	}
	void ConvertToChar(T_RFST_D& rData)
	{
		ConvertStrChar(rData.strReinforceSectName, strReinforceSectName, sizeof(strReinforceSectName));
		nOriginSectK = rData.nOriginSectK;
		nMatlK = rData.nMatlK;
		nReinforceType = rData.nReinforceType;
		nConnectionType = rData.nConnectionType;
		nSideType = rData.nSideType;
		bAutoCalcDeflection = rData.bAutoCalcDeflection;
		dWeldOmegaY = rData.dWeldOmegaY;
		dWeldOmegaZ = rData.dWeldOmegaZ;
		mWeldData.Get_T_RFST_WELD_D(rData.mWeldData);
		memcpy(Sect, rData.Sect, sizeof(Sect));
		Stiffness = rData.Stiffness;
		nPSCSectK = rData.nPSCSectK;
		memcpy(ReserveBuffer, rData.ReserveBuffer, sizeof(ReserveBuffer));
	}

};
struct T_RFST_UDRD_D
{
	T_RFST_K key;
	T_RFST_D data;
};
struct T_RFST_UDRD_D_CH_RW
{
	T_RFST_K key;
	T_RFST_D_CH_RW data;
};
//加固截面的数据统一采样长度单位
struct T_RFST_UNIT
{
	T_RFST_UNIT()
	{
		Length = D_UNITSYS_BASE_LENGTH;
	}
	int Length;
};


/************************************************************************/
/*      Allocate Reinforced Sect:T_ARST_D                            */
/************************************************************************/
#define T_ARST_K T_ELEM_K
#define HASHSIZEARST 1001
struct T_ARST_D
{
	T_RFST_K ReinforceSectKey;
	//T_MATL_K matlK;
	char ReserveBuffer[40];

	void Initialize()
	{
		ReinforceSectKey = 0;
		//matlK = 0;
		memset(ReserveBuffer, 0, sizeof(ReserveBuffer));
	}
	T_ARST_D() { Initialize(); }
	T_ARST_D(const T_ARST_D& src) { T_ARST_D(); *this = src; }
	T_ARST_D& T_ARST_D::operator=(const T_ARST_D& src)
	{
		ReinforceSectKey = src.ReinforceSectKey;
		//matlK = src.matlK;
		memcpy(ReserveBuffer, src.ReserveBuffer, sizeof(ReserveBuffer));
		return *this;
	}
};
struct T_ARST_UDRD_D
{
	T_ARST_K key;
	T_ARST_D data;
};
#pragma pack(pop)



