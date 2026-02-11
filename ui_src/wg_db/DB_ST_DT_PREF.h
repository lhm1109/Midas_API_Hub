#ifndef __DB_ST_DT_PREF_H__
#define __DB_ST_DT_PREF_H__

#define MIDAS_SDK


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

#pragma pack(push, 1)

enum enPrefDgnThread
{
	enPrefDgnThreadSingle = 0,
	enPrefDgnThreadHalf,
	enPrefDgnThreadMax
};

// Preference
struct __MY_EXT_CLASS__ T_PREFER_GENERAL
{
	CString UserName;
	CString strTempFilePath;
	CString strLogoBmpPath;
	int nRecentlyProject;
	BOOL bAutoSave;
	int nAutoSaveSecond;
	BOOL bMakeBackup;
	int nOptDefFileOpen;
	CString strFileOpenPath;
	int nLanguageID;
};

struct __MY_EXT_CLASS__ T_PREFER_VIEW
{
	double dblModelSize;
	double dblZoomSpeed;
	double dblGridSpaceX;
	double dblGridSpaceY;
	BOOL bGridDisplay;
	int nViewPoint;  // 0:ISO 1:X-Y
	BOOL bSnapPointGrid;
	BOOL bSnapLineGrid;
	BOOL bSnapNode;
	BOOL bSnapElement;
	int nVerticalPos;  // 0:Left 1:Right
	BOOL bViewCube;
	BOOL bSnapTracing;
};

struct __MY_EXT_CLASS__ T_PREFER_DATA
{
	double dblRegardZero;
	double dblDuplicateTolerance;
	double dblIntersectTolerance;
	double dblCoordinateTolerance;
};

struct __MY_EXT_CLASS__ T_PREFER_RESULTS
{
	BOOL   bEnvelopeAllSign;
	int    nCountryCode;      // 0:KR, 1:CN, 2:JP, 3:EN 4: RUS
	BOOL   bSimpleLegend;
};

struct __MY_EXT_CLASS__ T_PREFER_NOTICE
{
	BOOL   bShowNotice;
	BOOL   bUseLocalHelp;
	int		 nStartPageContents; //0: 기존 스타트페이지, 1: 지정한 홈페이지
	CString strHomePageURL;
};

struct __MY_EXT_CLASS__ T_PREFER_PROPERTY
{
	CString SteelMaterialDBName;
	CString ConcreteMaterialDBName;
	CString ConcreteMaterialCodeName;
	CString SectionDBName;
};

struct __MY_EXT_CLASS__ T_PREFER_UNITTYPE
{
	BOOL bAskNewProject;
	int nLengthUnit;
	int nForceUnit;
	int nHeatUnit;  // 700에서 추가 0:Kcal, 1:J, 2:Btu (단위 변환은 안함)
	int nTemperUnit; // 700에서 추가 0:Celsius, 1:Fahrenheit (단위 변환은 안함)
};

struct __MY_EXT_CLASS__ T_PREFER_FORMAT
{
	BOOL bAutoChange;
	BOOL bExponential;
	int nDimension;
	int nDisplacement;
	int nDimensionless;
	int nRotation;
	int nMass;
	int nWeight;
	int nForce;
	int nUnitForce;
	int nMoment;
	int nUnitMoment;
	int nStress;
	int nAcceleration;
};

struct __MY_EXT_CLASS__ T_PREFER_DGNCODE
{
	CString SteelCode;
	CString CFSteelCode;
	CString AluliumCode;
	CString ConcCode;
	CString ConcRebarMatlCode;
	CString ConcRebarMatlDB;
	CString SRCCode;
	CString SRCRebarMatlCode;
	CString SRCRebarMatlDB;
	CString RCCode;
	CString RatingSTL;
	CString RatingRC;
	CString RatingPSC;
	CString PSCCode;
	CString PGCode;
	CString StlOrthoDeck;
	CString BrdgSeisEval;
	int			OutputLanguage;

	//(2009.06.17) Add by Unsang :: National Annex for EC2:04 / EC2-2:05
	int     nConcNationalAnnex;
	//(2009.11.10) Add by Unsang :: National Annex for EC2-2:05, PSC
	int     nPscNationalAnnex;
	int     nStlNationalAnnex; // PMS:4588 DNKIM-20130311 EC3 National Annex : Sweden 추가
	int     nCfsNationalAnnex;

	//(2023.03.20)
	int     nPgNationalAnnex; // Composite
	int     nStlReportLanguage;
	int     nConcReportLanguage;
	int     nPscReportLanguage;
	int     nPgReportLanguage;

	int     nPscReportUnit;  // 0:SI 1:US
	int     nPgReportUnit;  // 0:SI 1:US

	//(2023.05.15)
	int     nStlReportUnit;  // 0:SI 1:US
	//(2024.01.16)
	int     nConcReportUnit; // 0:SI 1:US

	// Design Engine Perform thread
	int nDgnThread;

};

struct __MY_EXT_CLASS__ T_PREFER_LOADCODE
{
	CString WLCode;
	CString SLCode;
	CString RSCode;
	CString MLCode;
		
	int     nWLNationalAnnex;
	int     nSLNationalAnnex;
	int     nRSNationalAnnex;
};

struct __MY_EXT_CLASS__ T_PREFER_FLOAD_BASE
{
	CString strName;
	double  dValue;
};

struct __MY_EXT_CLASS__ T_PREFER_FLOAD
{
	CArray<T_PREFER_FLOAD_BASE, T_PREFER_FLOAD_BASE&> LoadList;

	T_PREFER_FLOAD(){};
	T_PREFER_FLOAD(const T_PREFER_FLOAD& Src)
	{
		LoadList.Copy(Src.LoadList);
	}

	void Initialize()
	{
		int nLoad = (int)LoadList.GetSize();
		for(int i = 0; i < nLoad ; i++)
		{
			LoadList.RemoveAll();
		}
	}
	T_PREFER_FLOAD& operator=(const T_PREFER_FLOAD&Src)
	{
		LoadList.Copy(Src.LoadList);
		return *this;
	}


};

struct __MY_EXT_CLASS__ T_PREFER_LOADVAL
{
	int m_nFormatType; // (0) General (1) Fixed (2) Scientific 
	int m_DefLoadValDecPoint;
	int m_DefLoadValPlaces  ;
};

struct __MY_EXT_CLASS__ T_PREFERENCE
{
	T_PREFER_GENERAL  General;
	T_PREFER_VIEW     View;
	T_PREFER_DATA     Data;
	T_PREFER_RESULTS  Results;
	T_PREFER_PROPERTY Property;
	T_PREFER_UNITTYPE Unit;
	T_PREFER_FORMAT   Format;
	T_PREFER_DGNCODE  DgnCode;
	T_PREFER_LOADCODE LoadCode;
	T_PREFER_FLOAD    FLoadDB;
	T_PREFER_NOTICE   Notice;
	T_PREFER_LOADVAL  LoadVal;

	void Initialize();
};

struct __MY_EXT_CLASS__ T_PREFER_GENERAL_UNIT
{
	T_PREFER_GENERAL_UNIT();
	
	int UserName;
	int strTempFilePath;
	int strLogoBmpPath;
	int nRecentlyProject;
	int bAutoSave;
	int nAutoSaveSecond;
	int bMakeBackup;
	int nOptDefFileOpen;
	int strFileOpenPath;
	int nCount;
};

struct __MY_EXT_CLASS__ T_PREFER_VIEW_UNIT
{
	T_PREFER_VIEW_UNIT();
	
	int dblModelSize;
	int dblZoomSpeed;
	int dblGridSpaceX;
	int dblGridSpaceY;
	int bGridDisplay;
	int nViewPoint;  // 0:ISO 1:X-Y
	int bSnapPointGrid;
	int bSnapLineGrid;
	int bSnapNode;
	int bSnapElement;
	int bSnapTracing;
	int nCount;
	int nUnit[11];
};

struct __MY_EXT_CLASS__ T_PREFER_DATA_UNIT
{
	T_PREFER_DATA_UNIT();

	int dblRegardZero;
	int dblDuplicateTolerance;
	int dblIntersectTolerance;
	int dblCoordinateTolerance;
	int nCount;
	int nUnit[4];
};

struct __MY_EXT_CLASS__ T_PREFER_RESULTS_UNIT
{
	T_PREFER_RESULTS_UNIT();

	int bEnvelopeAllSign;
	int nCount;
	int bSimpleLegend;
	int nUnit[1];
};

struct __MY_EXT_CLASS__ T_PREFER_PROPERTY_UNIT
{
	T_PREFER_PROPERTY_UNIT();

	int SteelMaterialDBName;
	int ConcreteMaterialDBName;
	int ConcreteMaterialCodeName;
	int SectionDBName;
	int nCount;
	int nUnit[4];
};

struct __MY_EXT_CLASS__ T_PREFER_UNITTYPE_UNIT
{
	T_PREFER_UNITTYPE_UNIT();
	
	int bAskNewProject;
	int nLengthUnit;
	int nForceUnit;
	int nCount;
	int nUnit[3];
};

struct __MY_EXT_CLASS__ T_PREFER_FORMAT_UNIT
{
	T_PREFER_FORMAT_UNIT();
	
	int bAutoChange;
	int bExponential;
	int nDimension;
	int nDisplacement;
	int nDimensionless;
	int nRotation;
	int nMass;
	int nWeight;
	int nForce;
	int nDistForce;
	int nMoment;
	int nDistMoment;
	int nStress;
	int nAcceleration;
	int nCount;
	int nUnit[14];
};

struct __MY_EXT_CLASS__ T_PREFER_DGNCODE_UNIT
{
	T_PREFER_DGNCODE_UNIT();
	
	int SteelCode;
	int ConcCode;
	int ConcRebarMatlCode;
	int ConcRebarMatlDB;
	int SRCCode;
	int SRCRebarMatlCode;
	int SRCRebarMatlDB;
	int OutputLanguage;
	int RCCode;
	int RatingSTL;
	int RatingRC;
	int RatingPSC;
	int PSCCode;
	int PGCode;
	int StlOrthoDeck;
	int BrdgSeisEval;
	//
	int nConcNationalAnnex;
	int nPscNationalAnnex;
	int nStlNationalAnnex;
	//
	int nPgNationalAnnex;
	int nStlReportLang;
	int nConcReportLang;
	int nPscReportLang;
	int nPgReportLang;
};

struct __MY_EXT_CLASS__ T_PREFER_UNIT
{
	T_PREFER_GENERAL_UNIT  General;
	T_PREFER_VIEW_UNIT     View;
	T_PREFER_DATA_UNIT     Data;
	T_PREFER_RESULTS_UNIT  Results;
	T_PREFER_PROPERTY_UNIT Property;
	T_PREFER_UNITTYPE_UNIT Unit;
	T_PREFER_FORMAT_UNIT   Format;
	T_PREFER_DGNCODE_UNIT  DgnCode;
};

struct __MY_EXT_CLASS__ T_ANALOPTION
{
	int nAnalMethod;    //0:Skyline, 1:Sparse, 2:Band, 3:Multi Frontal
	int nMemoryType;    //0:Auto, 1:User
	int nMemoryQuantity;      //nMemoryType = 1일때만 사용 Mbyte
	int nMemoryQuantityGB;    //nMemoryType = 1일때만 사용 Gbyte
	int nUseMultiProcessor;   //nAnalMethod = 3일때만 사용
	int nNumMultiProcessor;   //nAnalMethod = 3일때만 사용
	int nUseGPUAccel;         //nAnalMethod = 3일때만 사용   // Enable GPU Acceleration

	void Initialize()
	{
		nAnalMethod = 3;
		nMemoryType = 0;
		nMemoryQuantity = 64;
		nMemoryQuantityGB = 1;
		nUseMultiProcessor = 0;
		nNumMultiProcessor = 1;
		nUseGPUAccel = 0;
	}
};

namespace ANALOPTION_MEC
{
	enum EquationSolver : int {
		AUTO = 1,
		MFS = 2,
		AMG = 3,
		DENSE = 4,
	};

	constexpr int DefaultNumMultiProcessor = 4;
}

struct __MY_EXT_CLASS__ T_ANALOPTION_MEC
{
	int nAnalMethod;
	int nNumMultiProcessor;
	bool bUseGPUAccel;

	void Initialize()
	{
		using namespace ANALOPTION_MEC;

		nAnalMethod = EquationSolver::AUTO;
		nNumMultiProcessor = DefaultNumMultiProcessor;
		bUseGPUAccel = FALSE;
	}
};

// Design Analysis Control 대화상자에서 입력한 값
struct __MY_EXT_CLASS__ T_DGNANALOPTION
{
	BOOL bConsiderStiff; // Consider the Stiffness of Beam or Plate
	int	 nStiffType    ; // 0:Beam, 1:Plate
	BOOL bRelieveSettle; // Relieve Differential Settlement

	void Initialize()
	{
		bConsiderStiff = FALSE;
		nStiffType     = 0;
		bRelieveSettle = FALSE;
	}
};

#pragma pack(pop)

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

#endif  // __DB_ST_DT_PREF_H__
