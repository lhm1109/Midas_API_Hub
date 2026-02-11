// LoadCombCtrl.h: interface for the CLoadCombCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADCOMBCTRL_H__D5846021_0CC5_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_LOADCOMBCTRL_H__D5846021_0CC5_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\\wg_db\\wg_db_DBDoc.h"
#include <map>
#include <vector>
#include <tuple>
#include <unordered_set>

#include "HeaderPre.h"


#define ArINT		CArray<int,int>
#define ArUNT		CArray<unsigned int,unsigned int>

#define _LCASE_STATIC		0
#define _LCASE_EL			1
#define _LCASE_WL			2
#define _LCASE_ESP			3
#define _LCASE_ESV			4
#define _LCASE_EVT			5
#define _LCASE_LI			6
#define _LCASE_LJ			7
#define _LCASE_TS			8
#define _LCASE_TP			9
#define _LCASE_SUM			10
#define _LCASE_CONST_DL		11
#define _LCASE_CONST_LL		12
#define _LCASE_CONST_CR		13
#define _LCASE_CONST_SH		14
#define _LCASE_CRANE        15

#define _LCOM_SRSS_EL	101
#define _LCOM_SRSS_ESP	103

#define _LCOM_COMB_WL   201

enum EN_LOAD
{
    EN_LOAD_D   = 0U, // Dead Load
    EN_LOAD_DC ,      // Component and Attachments
    EN_LOAD_DD ,      // Downdrag
    EN_LOAD_DW ,      // Wearing Surfaces and Utilities
    EN_LOAD_L  ,      // Live Load
    EN_LOAD_IL ,      // Impact Load(Ãæ°ÝÇÏÁß)
    EN_LOAD_CRL,      // Crowd Load(±ºÁßÇÏÁß)
    EN_LOAD_PS ,      // Prestress Load
    EN_LOAD_CR ,      // Creep Load
    EN_LOAD_SH ,      // Shrinkage Load
    EN_LOAD_EP ,      // Earth Pressure Load
    EN_LOAD_EH ,      // horizontal earth pressure load
    EN_LOAD_EV ,      // vertical earth pressure load
    EN_LOAD_ES ,      // Earth Surcharge
    EN_LOAD_B  ,      // Buoyancy(ºÎ·Â) or uplift pressure(¾ç¾Ð·Â) Load
    EN_LOAD_T ,       // ¿Âµµº¯È­ ÇÏÁß(Local : Nodal, Element, Temperature Gradient, Beam Section Temperature)
    EN_LOAD_TPG ,     // ¿Âµµº¯È­ ÇÏÁß(Global : System Temperature)
    EN_LOAD_S  ,      // Snow Load
    EN_LOAD_STL,      // Settlement Load(ºÎµîÄ§ÇÏ)
    EN_LOAD_SM ,      // Settlement (not static)
    EN_LOAD_M  ,      // Moving Load, Eurocode 0ÀÏ °æ¿ì´Â not leadingÀÌ°í LM2°¡ ¾Æ´Ñ moving load case,  Characteristic
    EN_LOAD_CF ,      // centrifugal load(¿ø½ÉÇÏÁß)
    EN_LOAD_BRK,      // braking load(Á¦µ¿ÇÏÁß)
    EN_LOAD_IP ,      // China  ºù¾Ð·Â
    EN_LOAD_FR ,      // China, AASHTO LRFD98 ¸¶Âû·Â   
    EN_LOAD_W  ,      // wind load
    EN_LOAD_WL ,      // wind load added to moving vehicle
    EN_LOAD_WP ,      // Water Pressure Load
    EN_LOAD_FP ,      // À¯Ã¼¾Ð(Fluid pressure) : static
    EN_LOAD_SF ,      // À¯¼ö¾Ð(Stream flow pressure) : dynamic
    EN_LOAD_WPR,      // Wave pressure Load(ÆÄ¾Ð)
    EN_LOAD_E  ,      // Seismic load
    EN_LOAD_ESP,      // Respons Spectrum
    EN_LOAD_CO ,      // Collision Load(Ãæµ¹ÇÏÁß)
    EN_LOAD_CT ,      // vehicle collision load(Â÷·®Ãæµ¹ÇÏÁß)
    EN_LOAD_CV ,      // vessel collision load(¼±¹ÚÃæµ¹ÇÏÁß)
    EN_LOAD_ER ,      // Erection Load(°¡¼³ÇÏÁß)
    EN_LOAD_NUM
};

enum SEIS_EVAL_SUB_LOADTYPE
{
	GRAVITY = 0,  // Gravity
	LONGI_TYPE,   // Longitudinal (±³Ãà)
	TRANS_TYPE,   // Transverse (±³ÃàÁ÷°¢)
	VERTI_TYPE    // Vertical (¿¬Á÷)
};

enum ORTHOLOADCOMB_TYPE
{
	COMBTYPE_BOTH = 0,
	COMBTYPE_PLUS
};

enum EN_JSCE17
{
    EN_JSCE17_D   = 0U, // Dead Load
    EN_JSCE17_L  ,      // Live Load
    EN_JSCE17_IL ,      // Impact Load(Ãæ°ÝÇÏÁß)
    EN_JSCE17_CRL,      // Crowd Load(±ºÁßÇÏÁß)
    EN_JSCE17_PS ,      // Prestress Load
    EN_JSCE17_CR ,      // Creep Load
    EN_JSCE17_SH ,      // Shrinkage Load
    EN_JSCE17_EP ,      // Earth Pressure Load
    EN_JSCE17_WP ,      // Water Pressure Load
    EN_JSCE17_B  ,      // Buoyancy(ºÎ·Â) or uplift pressure(¾ç¾Ð·Â) Load
    EN_JSCE17_TG ,      // ¿Âµµº¯È­ ÇÏÁß(Global : System Temperature)
    EN_JSCE17_TL ,      // ¿Âµµº¯È­ ÇÏÁß(Local : Nodal, Element, Temperature Gradient, Beam Section Temperature)
    EN_JSCE17_S  ,      // Snow Load
    EN_JSCE17_STL,      // Settlement Load(ºÎµîÄ§ÇÏ)
    EN_JSCE17_SM ,      // Settlement (not static)
    EN_JSCE17_CF ,      // centrifugal load(¿ø½ÉÇÏÁß)
    EN_JSCE17_BRK,      // braking load(Á¦µ¿ÇÏÁß)
    EN_JSCE17_W  ,      // wind load
    EN_JSCE17_WL ,      // wind load added to moving vehicle
    EN_JSCE17_WPR,      // Wave pressure Load(ÆÄ¾Ð)
    EN_JSCE17_E  ,      // Seismic load
    EN_JSCE17_ESP,      // Respons Spectrum
    EN_JSCE17_CO ,      // Collision Load(Ãæµ¹ÇÏÁß)
    EN_JSCE17_ER ,      // Erection Load(°¡¼³ÇÏÁß)
    EN_JSCE17_NUM
};

enum EN_JSCE17_NO
{
    EN_JSCE17_NO_01 = 1U,// ¨ç D
    EN_JSCE17_NO_02,     // ¨è D+M+L
    EN_JSCE17_NO_03,     // ¨é D+TH
    EN_JSCE17_NO_04,     // ¨ê D+TH+WS
    EN_JSCE17_NO_05,     // ¨ë D+L+TH
    EN_JSCE17_NO_06,     // ¨ì D+M+L+WS+WL
    EN_JSCE17_NO_07,     // ¨í D+L+TH+WS+WL
    EN_JSCE17_NO_08,     // ¨î D+WS
    EN_JSCE17_NO_09,     // ¨ï D+TH+EQ
    EN_JSCE17_NO_10,     // ¨ð D+EQ(ESP)
    EN_JSCE17_NO_11,     // ¨ñ D+EQ(ESP)
    EN_JSCE17_NO_12      // ¨ò D+CO
};


struct JSCE17_LCOM
{
    EN_JSCE17_NO  enLcomNo;
    BOOL bMoving, bThermal, bEsp;

    void Set(EN_JSCE17_NO enLcomNo, BOOL bMoving, BOOL bThermal, BOOL bEsp)
    {
        this->enLcomNo = enLcomNo;
        this->bMoving  = bMoving;
        this->bThermal = bThermal;
        this->bEsp     = bEsp;
    }
};

enum EN_AS5100_NO
{
    //PE(= D + DW + EP + WP + PS + CR + SH + STL + SM + FP + B)
    EN_AS5100_ULS_01 = 1U,// ULS A: Minimum Strength and Stability - D + DW + EP + WP
    EN_AS5100_ULS_02_BRK,     // ULS B: PE + Moving
    EN_AS5100_ULS_02_CF,     // ULS B: PE + Moving
    EN_AS5100_ULS_03_BRK,     // ULS B: PE + Moving + Temp
    EN_AS5100_ULS_03_CF,     // ULS B: PE + Moving + Temp
    EN_AS5100_ULS_04_BRK,     // ULS B: PE + Moving + Wind
    EN_AS5100_ULS_04_CF,     // ULS B: PE + Moving + Wind
    EN_AS5100_ULS_05,     // ULS E: PE + CO
    EN_AS5100_ULS_06,     // ULS E: PE + CO + Moving
    EN_AS5100_ULS_07,     // ULS H: PE + Earthquake
    EN_AS5100_ULS_08,     // ULS J: PE + Wind
    EN_AS5100_ULS_09,     // ULS J: PE + Wind + Temp
    EN_AS5100_ULS_10,     // ULS K: PE + Temp
    EN_AS5100_ULS_11,     // ULS K: PE + Temp + Moving

    //1.SLS¿¡¼­ Moving, Wind, Thermal, Earthquake°¡ ÇÊ¼ö 3°³°¡ ´Ù ÀÖ´Â °æ¿ì
    EN_AS5100_SLS_01_BRK,       // SLS : PE + Moving + 0.5(Wind + Earthquake)
    EN_AS5100_SLS_01_CF,        // SLS : PE + Moving + 0.5(Wind + Earthquake)
    EN_AS5100_SLS_02_BRK,       // SLS : PE + Moving + 0.5(Temp + Earthquake)
    EN_AS5100_SLS_02_CF,        // SLS : PE + Moving + 0.5(Temp + Earthquake)
    EN_AS5100_SLS_03_BRK,       // SLS : PE + Moving + 0.5(Wind + Temp)
    EN_AS5100_SLS_03_CF,        // SLS : PE + Moving + 0.5(Wind + Temp)
    EN_AS5100_SLS_01_07_BRK ,   // SLS : PE + Moving + 0.7Wind
    EN_AS5100_SLS_01_07_CF  ,   // SLS : PE + Moving + 0.7Wind
    EN_AS5100_SLS_02_07_BRK ,   // SLS : PE + Moving + 0.7Temp
    EN_AS5100_SLS_02_07_CF  ,   // SLS : PE + Moving + 0.7Temp
    EN_AS5100_SLS_03_07_BRK ,   // SLS : PE + Moving + 0.7Earthquake
    EN_AS5100_SLS_03_07_CF  ,   // SLS : PE + Moving + 0.7Earthquake
    EN_AS5100_SLS_04,           // SLS : PE + Temp + 0.5(Wind + Earthquake)
    EN_AS5100_SLS_05_BRK,       // SLS : PE + Temp + 0.5(Moving + Wind)           
    EN_AS5100_SLS_05_CF,        // SLS : PE + Temp + 0.5(Moving + Wind)           
    EN_AS5100_SLS_06_BRK,       // SLS : PE + Temp + 0.5(Earthquake + Moving)
    EN_AS5100_SLS_06_CF,        // SLS : PE + Temp + 0.5(Earthquake + Moving)
    EN_AS5100_SLS_04_07     ,   // SLS : PE + Temp + 0.7Wind
    EN_AS5100_SLS_05_07_BRK ,   // SLS : PE + Temp + 0.7Moving           
    EN_AS5100_SLS_05_07_CF  ,   // SLS : PE + Temp + 0.7Moving           
    EN_AS5100_SLS_06_07     ,   // SLS : PE + Temp + 0.7Earthquake
    EN_AS5100_SLS_07,           // SLS : PE + Wind + 0.5(Earthquake + Temp)
    EN_AS5100_SLS_08_BRK,       // SLS : PE + Wind + 0.5(Temp + Moving),        
    EN_AS5100_SLS_08_CF,        // SLS : PE + Wind + 0.5(Temp + Moving),        
    EN_AS5100_SLS_09_BRK,       // SLS : PE + Wind + 0.5(Moving + Earthquake)
    EN_AS5100_SLS_09_CF,        // SLS : PE + Wind + 0.5(Moving + Earthquake)
    EN_AS5100_SLS_07_07     ,   // SLS : PE + Wind + 0.7Earthquake
    EN_AS5100_SLS_08_07     ,   // SLS : PE + Wind + 0.7Temp      
    EN_AS5100_SLS_09_07_BRK ,   // SLS : PE + Wind + 0.7Moving
    EN_AS5100_SLS_09_07_CF  ,   // SLS : PE + Wind + 0.7Moving
    EN_AS5100_SLS_10,           // SLS : PE + Earthquake + 0.5(Wind + Temp)
    EN_AS5100_SLS_11_BRK,       // SLS : PE + Earthquake + 0.5(Temp + Moving)
    EN_AS5100_SLS_11_CF,        // SLS : PE + Earthquake + 0.5(Temp + Moving)
    EN_AS5100_SLS_12_BRK,       // SLS : PE + Earthquake + 0.5(Moving + Wind)
    EN_AS5100_SLS_12_CF,        // SLS : PE + Earthquake + 0.5(Moving + Wind)
    EN_AS5100_SLS_10_07     ,   // SLS : PE + Earthquake + 0.7Wind
    EN_AS5100_SLS_11_07     ,   // SLS : PE + Earthquake + 0.7Temp
    EN_AS5100_SLS_12_07_BRK ,   // SLS : PE + Earthquake + 0.7Moving
    EN_AS5100_SLS_12_07_CF  ,   // SLS : PE + Earthquake + 0.7Moving
    EN_AS5100_Transfer_01,

    ////2.SLS¿¡¼­ Moving, Wind, Thermal, Earthquake Áß ÇÊ¼ö 3°³Áß 1°³°¡ ¾ø´Â °æ¿ì PE + Moving + 0.7(~)   PE + Temp + 0.7(~)  PE + Wind + 0.7(~)  PE + Earthquake + 0.7(~)
    ////3.SLS¿¡¼­ Moving, Wind, Thermal, Earthquake Áß ÇÊ¼ö 3°³Áß 2°³°¡ ¾ø´Â °æ¿ì PE + Moving            PE + Temp           PE + Wind           PE + Earthquake

    //Fatigue Limit State Design
    EN_AS5100_FLS_01      // FLS : 0.7 * Moving Load * (1 + alpha)
};


struct AS5100_LCOM
{
    EN_AS5100_NO  enLcomNo;

    void Set(EN_AS5100_NO enLcomNo)
    {
        this->enLcomNo    = enLcomNo;
    }
};

enum EN_BS5400_NO
{
    EN_BS5400_NO_01 = 1U,   // Combination 1 - Basic
    EN_BS5400_NO_02_01,     // Combination 2 - Wind only Dead Superimposed Dead 
    EN_BS5400_NO_02_02,     // Combination 2 - Wind Not only Dead Superimposed Dead 
    EN_BS5400_NO_02_03,     // Combination 2 - Wind Relieving Effect
    EN_BS5400_NO_03,        // Combination 3 - Temperature
    EN_BS5400_NO_04_01,     // Combination 4 - Collision
    EN_BS5400_NO_04_02,     // Combination 4 - Centrifugal
    EN_BS5400_NO_04_03,     // Combination 4 - Braking
    EN_BS5400_NO_05         // Combination 5 - Friction
};

enum EN_TMH07_NO
{
	EN_TMH07_NO_01 = 1U,   // Combination 1 - Basic
	EN_TMH07_NO_02_01,     // Combination 2 - Wind only Dead Superimposed Dead 
	EN_TMH07_NO_02_02,     // Combination 2 - Wind Not only Dead Superimposed Dead 
	EN_TMH07_NO_02_03,     // Combination 2 - Wind Relieving Effect
	EN_TMH07_NO_03,        // Combination 3 - Temperature
	EN_TMH07_NO_04_01,     // Combination 4 - Collision
	EN_TMH07_NO_04_02,     // Combination 4 - Centrifugal
	EN_TMH07_NO_04_03,     // Combination 4 - Braking
	EN_TMH07_NO_05,        // Combination 5 - Friction
	EN_TMH07_NO_EQ_ST,	   // Combination 6 - EQ ST
	EN_TMH07_NO_EQ_RS      // Combination 7 - EQ RS  
};

struct BS5400_LCOM
{
    EN_BS5400_NO  enLcomNo;
    BOOL bStrength;
    BOOL bMoving;
    BOOL bWind;

    void Set(EN_BS5400_NO enLcomNo, BOOL bStrength, BOOL bMoving, BOOL bWind)
    {
        this->enLcomNo  = enLcomNo;
        this->bStrength = bStrength;
        this->bMoving   = bMoving;
        this->bWind     = bWind;
    }
};

struct TMH07_LCOM
{
	EN_TMH07_NO  enLcomNo;
	BOOL bStrength;
	BOOL bMoving;
	BOOL bWind;
	BOOL bSeis;

	void Set(EN_TMH07_NO enLcomNo, BOOL bStrength, BOOL bMoving, BOOL bWind, BOOL bSeis)
	{
		this->enLcomNo = enLcomNo;
		this->bStrength = bStrength;
		this->bMoving = bMoving;
		this->bWind = bWind;
		this->bSeis = bSeis;
	}

};
enum EN_AASHTOSince12_NO
{
    EN_AASHTOSince12_NO_STR1 = 1U,  // Strength 1
    EN_AASHTOSince12_NO_STR2,       // Strength 2
    EN_AASHTOSince12_NO_STR3,       // Strength 3
    EN_AASHTOSince12_NO_STR4,       // Strength 4
    EN_AASHTOSince12_NO_STR5,       // Strength 5
    EN_AASHTOSince12_NO_ETR1_01,    // Extreme Event 1
    EN_AASHTOSince12_NO_ETR1_02,    // Extreme Event 1
    EN_AASHTOSince12_NO_ETR2_01,    // Extreme Event 2
    EN_AASHTOSince12_NO_ETR2_02,    // Extreme Event 2
    EN_AASHTOSince12_NO_ETR2_03,    // Extreme Event 2
    EN_AASHTOSince12_NO_SER1,       // Service 1
    EN_AASHTOSince12_NO_SER2,       // Service 2
    EN_AASHTOSince12_NO_SER3,       // Service 3
    EN_AASHTOSince12_NO_SER4,       // Service 4
    EN_AASHTOSince12_NO_FAT1,       // Fatigue 1
    EN_AASHTOSince12_NO_FAT2,       // Fatigue 2
    EN_AASHTOSince12_NO_SEIS_EQ,    // Seismic, DC+DW+MV+EQ
    EN_AASHTOSince12_NO_SEIS_RS,    // Seismic, DC+DW+MV+RS
};


struct AASHTOSince12_LCOM
{
    EN_AASHTOSince12_NO  enLcomNo;
    BOOL bStrength;
    BOOL bExtreme;
    BOOL bService;
    BOOL bFatigue;
    BOOL bMoving;
    BOOL bWind;
    BOOL bSeismic;

    void Set(EN_AASHTOSince12_NO enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, BOOL bMoving, BOOL bWind, BOOL bSeismic)
    {
        this->enLcomNo  = enLcomNo;
        this->bStrength = bStrength;
        this->bExtreme  = bExtreme;
        this->bService  = bService;
        this->bFatigue  = bFatigue;
        this->bMoving   = bMoving;
        this->bWind     = bWind;
        this->bSeismic  = bSeismic;
    }
};

enum EN_LOAD_CODE
{
	EN_LOADCODE_KDS2021 = 0,
	EN_LOADCODE_KDS2019 = 1
};

typedef std::tuple<int, int, int> EQEP_GROUP_K;  /// (Seismic Load Type, Load Case Key, Direction) load group key relation for underground load combination (defined for KDS 41)
typedef std::pair<int, int> TYPE_LCAS_K;  /// 1st = LoadCaseType, 2nd = T_STLD_K or T_SPLC_K

//Add by Hong,jiseon 2002.8.5 
class CombBase
{
protected:
	BOOL m_bGroup;
	int m_iGroupKey;
	//0 Auto , -1 ²»ÐèÒª 1±ØÒª
	int m_nNecessary;
public:		
	CString m_strType; //Loadcase type
	ArINT m_arLd;
	ArUNT m_arLdu;

	CombBase();
	virtual ~CombBase();
	CombBase(CString type,BOOL group, int grpkey, ArINT& iLoad, ArUNT& uLoad);
	CombBase(const CombBase& ld);
	CombBase& operator=(const CombBase& ld);

	void Set_Type(CString type);
	void SetNecessary(BOOL bNecessary);
	void SetNecessaryAuto();

	BOOL IsNecessary();
	CString Get_Type() {return m_strType;}
	BOOL IsGroup(){return m_bGroup;}
	BOOL IsGroup(int key);
	void Set_Group(BOOL bl){m_bGroup=bl;}
	void Set_GroupKey(int key);
	int Get_GroupKey(){return m_iGroupKey;}

	int Get_CaseType();
	BOOL Get_arLoad(ArINT& load);
	BOOL Get_arLoad(ArUNT& load);
	BOOL IsExist()	{return (m_arLd.GetSize()!=0 || m_arLdu.GetSize()!=0);}
	BOOL ExistEcc(UINT EspK);

};

// Coded by Seungjun MNet:No.2516 ('20061017)
struct _LCOM_ORTHO_BASE
{
	int iOrder;		// ¼ø¼­.
	int iOption;	// 0=X, 1=Y.
	void Initialize()
	{
		iOrder=0;
		iOption=0;
	}

	_LCOM_ORTHO_BASE() {}
	_LCOM_ORTHO_BASE(const _LCOM_ORTHO_BASE& rData) { *this = rData; }
	_LCOM_ORTHO_BASE& operator = (const _LCOM_ORTHO_BASE& rData)
	{
		iOrder   = rData.iOrder;
		iOption  = rData.iOption;
		return *this;
	}
};

// Coded by Seungjun MNet:No.2516 ('20061017)
struct _LCOM_ORTHO
{
	CArray<_LCOM_ORTHO_BASE,_LCOM_ORTHO_BASE> arLcomBase;

	void Initialize()
	{
		arLcomBase.RemoveAll();
	}

	_LCOM_ORTHO() {}
	_LCOM_ORTHO(const _LCOM_ORTHO& rData) { *this = rData; }
	_LCOM_ORTHO& operator = (const _LCOM_ORTHO& rData)
	{
		arLcomBase.Copy(rData.arLcomBase);
		return *this;
	}
};

struct _LCASE_DIR
{
	CArray<CString,CString&> arLcaseNa[2];	// 0=X-Dir, 1=Y-Dir.
	void Initialize()
	{
		for(int i=0; i<2; i++)	arLcaseNa[i].RemoveAll();
	}
	_LCASE_DIR& operator = (_LCASE_DIR& rData)
	{
		for(int i=0; i<2; i++)	arLcaseNa[i].Copy(rData.arLcaseNa[i]);
		return *this;
	}
};

// Coded by Seungjun MNet:No.2516 ('20061016)
struct _LCASE_ORTHO
{
	unsigned int iLcaseX;
	unsigned int iLcaseY;
	void Initialize()
	{
		iLcaseX=0;
		iLcaseY=0;
	}
	void SetReverse()
	{
		unsigned int tempKey = iLcaseX;
		iLcaseX=iLcaseY;
		iLcaseY=tempKey;
	}
	_LCASE_ORTHO()
	{
	}
	_LCASE_ORTHO(_LCASE_ORTHO& ortho)
	{
		*this = ortho;
	}
	_LCASE_ORTHO& operator=(const _LCASE_ORTHO& ortho)
	{
		iLcaseX = ortho.iLcaseX;
		iLcaseY = ortho.iLcaseY;
		return *this;
	}
};

struct _LCOM_IMPF // Imperfection Load
{
	_LCOM_IMPF(){Initialize();}
	T_STLD_K LCase1K;
	UINT     LCase2K;
	UINT     nAnalType; // D_LCOM_???
	int      nDir;      // 0:Global-X, 1:Global-Y
	double   dMutiRatio_CH;
	BOOL     bElem;
	void Initialize()
	{
		bElem = FALSE;
		LCase1K = 0;
		LCase2K = 0;
		nAnalType = 0;
		nDir = 0;
		dMutiRatio_CH = 1.0;
	}
};

struct _LCOM_GRAV_EFF // Gravity Effective Load (LL)
{
	T_STLD_K StldK;
	double   dFactor;
	_LCOM_GRAV_EFF() : StldK(0), dFactor(1.0) {}
};

struct _LCOM_GRAV_EFF_MV // Gravity Effective Load (MV)
{
	T_MVLD_K MvldK;
	double   dFactor;
	_LCOM_GRAV_EFF_MV() : MvldK(0), dFactor(0.5) {}
};

struct _LCOM_100_30_LC3 // Orthogonal Load (100:30 Rule)
{
	_LCOM_100_30_LC3(){Initialize();}
	BOOL bLCase3;   // True : Case 3 (X, Y, Z) False : Case2 (X, Y)
	UINT nLoadCaseType;
	UINT iLCKey1;   // X
	UINT iLCKey2;   // Y
	UINT iLCKey3;   // Z
	void Initialize()
	{
		bLCase3 = TRUE;
		nLoadCaseType = 0;
		iLCKey1 = 0;
		iLCKey2 = 0;
		iLCKey3 = 0;
	}
};


// Coded by Seungjun MNet:No.2516 ('20061017)
#define _ORTHO_KEY unsigned int
union _ORTHO_KEYS
{
	_ORTHO_KEY KeyMap;
	struct
	{
		unsigned int LcaseKey  : 20;   // 0-1048575
		unsigned int LcaseType : 12;   // 0-4095
	}Key;
};

struct _LCOM_BOTH // RUSSIA ÇÏÁßÁ¶ÇÕ¿¡¼­ Both ¼±ÅÃ½Ã Min, Max ±¸ºÐ¿ë
{
	int nBoth[D_LCOM_NUMCOMB]; // (Both ÀÏ¶§ 0:Max, 1:Min), (Both ¾Æ´Ò¶§ ¹«Á¶°Ç 0)

	void Initialize()
	{
		for(int i=0; i<D_LCOM_NUMCOMB; i++) nBoth[i] = 0;
	}

	_LCOM_BOTH() {}
	_LCOM_BOTH(const _LCOM_BOTH& rData) { *this = rData; }
	_LCOM_BOTH& operator = (const _LCOM_BOTH& rData)
	{
		for(int i=0; i<D_LCOM_NUMCOMB; i++) nBoth[i] = rData.nBoth[i];
		return *this;
	}
};

// PMS:4821 Seungjun-20140217 : ¿ëµµ¿¡ µû¸¥ psi factor Àû¿ëÇÏ¿© ÇÏÁßÁ¶ÇÕ »ý¼º
struct _LCOM_EC_PSI
{
	int    iStldK;
	double adPsi[3];
	void Initialize()
	{
		iStldK = 0;
		memset(adPsi, 0, sizeof(adPsi));
	}

	_LCOM_EC_PSI() {}
	_LCOM_EC_PSI(const _LCOM_EC_PSI& rData) { *this = rData; }
	_LCOM_EC_PSI& operator = (const _LCOM_EC_PSI& rData)
	{
		iStldK = rData.iStldK;
		memcpy(adPsi, rData.adPsi, sizeof(adPsi));
		return *this;
	}
};

// PN-S-10030 jsPark (16-03-23) ÇÏÁß Critical Factor Àû¿ë
struct _LCOM_UNIT
{
	unsigned int LcomK;
	double dFactor;
	CString strLcomName;

	void Initialize()
	{
		LcomK = 0;
		dFactor = 0.0;
		strLcomName.Empty();
	}

	_LCOM_UNIT()
	{

	}

	_LCOM_UNIT(const _LCOM_UNIT& src)
	{
		*this = src;
	}
	_LCOM_UNIT& operator=(const _LCOM_UNIT& rD)
	{
		LcomK        = rD.LcomK;
		dFactor      = rD.dFactor;
		strLcomName  = rD.strLcomName;

		return *this;
	}

};

struct _LCOM_IN
{
	CArray<_LCOM_UNIT, _LCOM_UNIT&> arVariableLoadCase;

	_LCOM_IN()
	{

	}

	_LCOM_IN(const _LCOM_IN& src)
	{
		*this = src;
	}

	_LCOM_IN& operator=(const _LCOM_IN& rD)
	{
		arVariableLoadCase.Copy(rD.arVariableLoadCase);
		return *this;
	}
};

struct _LCOM_FD
{
	CArray<double, double&> arCrticalF;

	_LCOM_FD& operator=(_LCOM_FD& rD)
	{
		arCrticalF.Copy(rD.arCrticalF);
		return *this;
	}
};

struct _LCOM_RES
{
	CArray<_LCOM_IN, _LCOM_IN&> arLcomD;

	_LCOM_RES& operator = (_LCOM_RES& rD)
	{
		arLcomD.Copy(rD.arLcomD);
		return *this;
	}

};

struct _LCOM_WIND_GROUP
{
	UINT unType;      // wind load combination type, see D_WIND_DIRCOMB_XXX
	T_STLD_K LcaseK;  // load case key of along wind.
	T_STLD_K LcaseKA; // load case key of across wind.
	T_STLD_K LcaseKT; // load case key of torsional wind.
	double dGD;
	double dk;	

	void Initialize()
	{
		unType  = 0;
		LcaseK  = 0;
		LcaseKA = 0;
		LcaseKT = 0;
		dGD = 0.0;
		dk  = 0.0;		
	}

	_LCOM_WIND_GROUP() {}
	_LCOM_WIND_GROUP(const _LCOM_WIND_GROUP& rData) { *this = rData; }
	_LCOM_WIND_GROUP& operator= (const _LCOM_WIND_GROUP& rData)
	{
		unType  = rData.unType;
		LcaseK  = rData.LcaseK;
		LcaseKA = rData.LcaseKA;
		LcaseKT = rData.LcaseKT;
		dGD     = rData.dGD;
		dk      = rData.dk;
		return *this;
	}
};

typedef struct T_LOADCOMB_ENGINEERINGSTRUCTURECOMMONCODEGB2019_D
{
	BOOL   bGB19Capc          ;//³ÐÔØÄÜÁ¦
	BOOL   bGB19Gen           ;//»ù±¾×éºÏ
	BOOL   bGB19Acdn          ;//Å¼È»×éºÏ
	BOOL   bGB19Serv          ;//Õý³£Ê¹ÓÃ
	BOOL   bGB19Elst          ;//µ¯ÐÔ½×¶Î
	BOOL   bGB19_Freq         ;//ÆµÓöÖµ
	BOOL   bGB19_QuasiPerm    ;//×¼ÓÀ¾ÃÖµ
	BOOL   bGB19_FreqComb     ;//ÆµÓö×éºÏ
	BOOL   bGB19_QuasiPermComb;//×¼ÓÀ¾Ã×éºÏ
	BOOL   bGB19_StandComb    ;//±ê×¼×éºÏ
	BOOL   bGB19_Factor       ;//µ÷ÕûÏµÊý
	double dGB19_Factor       ;//Ê¹ÓÃÄêÏÞµ÷ÕûÏµÊý
	BOOL   bGB19_PrestressLoss;//¿¼ÂÇÔ¤Ó¦Á¦ºÉÔØ¹¤¿öËðÊ§

	void Init(void)
	{
		bGB19Capc           = TRUE;//³ÐÔØÄÜÁ¦
		bGB19Gen            = TRUE;//»ù±¾×éºÏ
		bGB19Acdn           = TRUE;//Å¼È»×éºÏ
		bGB19Serv           = TRUE;//Õý³£Ê¹ÓÃ
		bGB19Elst           = TRUE;//µ¯ÐÔ½×¶Î
		bGB19_Freq          = TRUE;//ÆµÓöÖµ
		bGB19_QuasiPerm     = FALSE;//×¼ÓÀ¾ÃÖµ
		bGB19_FreqComb      = TRUE;//ÆµÓö×éºÏ
		bGB19_QuasiPermComb = TRUE;//×¼ÓÀ¾Ã×éºÏ
		bGB19_StandComb     = TRUE;//±ê×¼×éºÏ
		bGB19_Factor        = FALSE;//µ÷ÕûÏµÊý  
		dGB19_Factor        = 1.0 ;//Ê¹ÓÃÄêÏÞµ÷ÕûÏµÊý
		bGB19_PrestressLoss = TRUE;
	}
}TLCOMESCGB19D;

typedef CMapEx<T_STLD_K, T_STLD_K, _LCOM_WIND_GROUP, _LCOM_WIND_GROUP&> _LCOM_WIND_GROUP_MAP;

class __MY_EXT_CLASS__ CLoadCombCtrl
{
public:
	CLoadCombCtrl();
	virtual ~CLoadCombCtrl();

public:
	int m_LoadCombCount,m_LoadCombNameNo;
	CArray<T_LCOM_D, T_LCOM_D&> m_tempLcom;
	int  m_nDesignType; //Add HJS 2003.7.25 for Envelope LoadCombination

private:
	double m_dZeroLim;
	int m_nLcomType;
	BOOL m_bMovingSkip;
	CMap<int,int,double,double> m_ScaleData;
    std::map<TYPE_LCAS_K, double> m_ScaleDataUG;
    std::map<UINT, double> m_mLiveFact; /// f1 for Live load case (NSCP2015), 
	CMap<int,int,int,int> m_MovingData;
	CMap<int, int, int, int> m_lsdata; //TMH Load combination
	CMap<T_STLD_K, T_STLD_K, double, double> m_BrakingTractiveData;//For TMH7
	CMap<T_MVLD_K, T_MVLD_K, double, double> m_MVLDData;//For TMH7
	CArray<UINT, UINT> m_aCSOnlyK;    
    bool m_bRedundancyFact;
    double m_dRedundancyFact;  /// [NSCP 2015]

	// Envelope
public:
	int  m_nConstLoad;   // Add 2003. 8. 20
	CArray<int, int> m_arStrnK;
	CArray<int, int> m_arServK[3];
	CArray<int, int> m_arSpecK;
	CArray<int, int> m_arVertK;
    CArray<int, int> m_arStreK;
    CArray<int, int> m_arUGStrnK;
    CArray<int, int> m_arUGServK;
    CArray<int, int> m_arUGSpecK;
	BOOL Make_EnvelopeCombination(CDBDoc* pDoc, CString strCode, BOOL bOpenedDB=FALSE);
	BOOL Make_ExtendedEnvelope(CDBDoc* pDoc, CString strCode, BOOL bOpenedDB=FALSE);

private:
	BOOL m_bAddEnvelope; //Add 2003.7.25
	void MakeEnvLcom(const CArray<int,int>& aLcomK, const CString& strName, const CString& strDesc, OUT T_LCOM_D& rLcomD);
	BOOL MakeExtendEnvLcom(int nLcomType, const CArray<int,int>& aLcomK, int nType, const CString& strDgnTypeName, const CString& strDgnTypeDesc, const CString& strLcomTypeName, const CString& strLcomTypeDesc);
	BOOL Set_ExtendEnvLcom(const BOOL bExt,
		const CArray<int,int>& aLcomK, 
		const int nType, 
		const CString& strDgnTypeName, 
		const CString& strDgnTypeDesc, 
		const CString& strLcomTypeName, 
		const CString& strLcomTypeDesc,
		CArray<T_LCOM_D, T_LCOM_D&>& aLcomD);
	BOOL Set_SeismicEnvLcom(const BOOL bExt,
		const CArray<int,int>& aLcomK, 
		const int nType, 
		const CString& strDgnTypeName, 
		const CString& strDgnTypeDesc, 
		CArray<T_LCOM_D, T_LCOM_D&>& aLcomD);

	void RemoveDuplicateKeyForEnv();

public:
	void Set_AddEnvelope(BOOL bEnv){m_bAddEnvelope = bEnv;}
	void Set_ConstLoad(int n){m_nConstLoad = n;}
	void Set_LcomType(int nLcomType)	{m_nLcomType = nLcomType;}
	
	CString Get_StaticLoadCaseName(int iKey);
	CString Get_SpectrumLoadCaseName(int iKey, const CString& strLoadType=_T("ESP"));    
	CString Get_SettlementLoadCaseName(int iKey);    
	CString Get_MovingLoadLoadCaseName(int iKey);
	CString Get_LoadCombinationName(int nLcomType, int iKey);

	void Set_ScaleUpData(CMap<int,int,double,double>& arScaleData);
	BOOL Get_ScaleUpFactor(int nLoadCaseType, int iKey, double& dFactor);

    void Set_RedundancyFactor(bool bConsider, double dRF);

	void Set_MovingLoadData(CMap<int,int,int,int>& arMovingData);
	void Set_lsLoadData(CMap<int, int, int, int>& arMovingData);
	void Set_BrakingTractionData(CMap<T_STLD_K, T_STLD_K, double,double>& arMovingData);//TMH7
	void Set_MVLDData(CMap<T_MVLD_K, T_MVLD_K, int, int>& arMovingData);//TMH7
	void Set_DeadLoadData(CArray<UINT, UINT>& arDLData); // KOREA LRFD 11 - Fatigue

	BOOL Exist_MovingLoadData(int iKey);
	BOOL Exist_LongBrkTrct_Data(int iKey);
	double lngbrk_factors(int iKey);
    double Get_BSMovingLoadFactorData(int iKey, CString sComb);
	double Get_TMHMovingLoadFactorData(int iKey, CString sComb);
    void Check_LoadCombULS(int& iLoadCaseNo, int& iLcomType, T_LCOM_D& rData, _LCOM_BOTH& rBoth);
    void Check_LoadCombSLS(int& iLoadCaseNo, int& iLcomType, T_LCOM_D& rData, _LCOM_BOTH& rBoth);

	BOOL Is_ChinaCode();
	BOOL Is_TaiwanCode();
	BOOL Is_EuroCode();
	BOOL Is_IndiaCode();

	int Get_MaxLoadCombNameNo();
	int Get_CurrentLoadCombType();
	BOOL Delete_AllLoadComb();
	BOOL Set_DefaultLoadComb(int nOption, CString strCode);
	BOOL Set_LoadCombByCode(CString strCode);
	BOOL Set_LoadCombByCode4Civil(CString strCode);   // added by KYM  

	BOOL Set_DefaultLoadComb4JUD(BOOL bReplace); // Add by GAY. PMS:4215. ('11.01.17). ÀÏº» 2Â÷¼³°è¿ë ÇÏÁßÁ¶ÇÕ ÀÚµ¿»ý¼º Ã³¸®.
	BOOL Delete_LoadComb_AutoJUD();              // Add by GAY. PMS:XXXX. ('11.01.21). ÀÏº» 2Â÷¼³°è¿ë Tree¿¡¼­ ÀÚµ¿»ý¼ºÇÑ ÇÏÁßÁ¶ÇÕÀ» »èÁ¦. (T_LCOM_D>BOOL bElaticStage º¯¼ö È°¿Ë)

	//For IS800:2007 and IS456:2000 orthogonal load combinations - Added by Maharshi
	void AddOrthoKeysIfApplicableStatic(CombBase& cEL);
	void AddOrthoKeysIfApplicableESP(CombBase & cEL);

	// To set Load Cases.
	void Set_StaticCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData);
	void Set_WindOrEqCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	void Set_SpectrumCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
    void Set_SpectrumCase_ND(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);    
	void Set_SettleCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	void Set_MovingCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	void Set_CStageCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	void Set_THistoryCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	void Set_EccentricCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE); //03.09.22
    void Set_EccentricCase_ND(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE); //03.09.22
    void Set_GridCase(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	void Set_SrssCase(int iCombKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	void Set_CraneCase(int iCombKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	int  Set_STLCIncludeCS(int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, T_SGLD_K SgldK);
    void Set_SplcCase(const CString& strType, int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
    void Set_EspcCase(const CString& strType, int iCaseKey, double dFactor, int& iCaseNo, T_LCOM_D& rData, BOOL bInc=FALSE);
	// Get AnalType.
	int	 Get_AnalType(int iLcomType);

	// To set Load Combinations.
	void Set_LoadComb(int iOrder, T_LCOM_D& rData, BOOL& bSameLcom, CString strNamePrefix=_T(""));    
	void Set_LoadComb_Imperfection(int iOrder, int nActiveOld, T_LCOM_D& rData, BOOL& bSameLcom, CString strNamePrefix=_T(""));    
	void AddLComToTemp(T_LCOM_D& rData, BOOL& bSameLcom);
	void Before_Set_LoadComb(int iOrder, T_LCOM_D& rData, CString strNamePrefix);
	void Set_LoadComb_Service(T_LCOM_D& rData);

protected:
	BOOL GetUsedStldKeyList(CArray<T_STLD_K, T_STLD_K>& aKeyList);
	BOOL GetUsedMvldKeyList(CArray<UINT,UINT>& aMvldKeyList, CArray<UINT,UINT>& aMvldFreqKeyList,ArINT& aMCrlKeyList,ArUNT &aLeadingMLoad, ArUNT &aLeadingMLoad_ser1,ArUNT &aLeadingMLoad_ser2, ArUNT &aLeadingMLoadFreq_ser1,ArUNT &aLeadingMLoadFreq_ser2, ArUNT &aMLoad_F, ArUNT &aMLoad_S, CString strCode);
	//add by gongxing at 2015-10
	BOOL GetUsedMvldKeyListJTGD6015(CArray<UINT,UINT>& aMvldKeyList, CArray<UINT,UINT>& aMvldFreqKeyList, ArINT& aMCrlKeyList,
		ArUNT &aLeadingMLoad, ArUNT &aLeadingMLoad_ser1,ArUNT &aLeadingMLoad_ser2, ArUNT &aLeadingMLoadFreq_ser1,
		ArUNT &aLeadingMLoadFreq_ser2, ArUNT &aMLoad_F, ArUNT &aMLoad_S, CString strCode);

	BOOL GetUsedMvldPsi1KeyList(ArUNT &aPsi1FactorMLoad1,ArUNT &aPsi1FactorMLoad2,CString strCode);

	// MQC:4881-PARKHJ-20120619 : µ¿ÀÏÇÑ ÇÏÁßÁ¶ÇÕ Á¦°Å
	void RemoveDuplicateLcom(CArray<T_LCOM_D, T_LCOM_D&>& arLcomD);
	BOOL IsExistLcom(T_LCOM_D& CurLcomD, const CArray<T_LCOM_D, T_LCOM_D&>& arLcomD);
	BOOL IsSameLcomData(const T_LCOM_D& CurLcomD, const T_LCOM_D& PreLcomD);
	void Remove0FactorCase(T_LCOM_D& LComD);

	BOOL IsExistCombBase(CombBase* pCombBase);

	int m_nOrthoCombType;  // 0 : Both(+-), 1: Combination Plus Only(++)
public:
    int m_bSeismic;
	int m_nOrthoType;				// -1=No, 0=30 %, 1=SRSS.
	int  Get_OrthoType(){return m_nOrthoType;}
	void Set_OrthoCombType(int nType){m_nOrthoCombType = nType;}
	int  Get_OrthoCombType(){return m_nOrthoCombType;}
        
    void SetNonDissipative(BOOL bND) { m_bNonDissipative = bND; }

    void SetSlcmSpecFlag(BOOL bSpec);
    void SetSlcmVertFlag(BOOL bVert);
    void SetLcomUGFlag(BOOL bUG);
    void SetLcomUGSpecFlag(BOOL bUGSP);

protected:
    BOOL m_bSlcmSpecial;
    BOOL m_bSlcmVertical;
    BOOL m_bLcomUG;
    BOOL m_bLcomUGSpec;
    BOOL m_bModeUG;

	BOOL m_bUseOmega;				// Use Omega (Previous Lcom for SRSS).
	int m_iSetLcomForSRSS;	// 1=1st-Lcom, 2=SRSS, 3=Dgn-Lcom by SRSS.
	int m_iSetLcomForKBC;		// 0=Strength/Stress, 1=Special, 2=Vertical.
	// Add by ZINU.('06.07.28). NO:2341, Exist Accidental Eccentricity.
	BOOL m_bSetOnlyEccn;		// Only Set if Exist Eccns.
	CMap<int,int&,_LCASE_DIR,_LCASE_DIR&> m_arDirLcaseNa;
    CArray<T_LCOM_K, T_LCOM_K> m_arSrssElLcomK;
    CArray<T_LCOM_K, T_LCOM_K> m_arSrssEspLcomK;

    std::map<T_LCOM_K, TYPE_LCAS_K> m_mSrssLcomStepEPK;
    std::map<T_LCOM_K, TYPE_LCAS_K> m_mSrssLcomSeisEPK;
    std::map<TYPE_LCAS_K, std::vector<CString>> m_mStepEPSrssName;
    std::map<TYPE_LCAS_K, std::vector<CString>> m_mSeisEPSrssName;
    
	// Coded by Seungjun MNet:No.2516 ('20061016)
	CArray<_LCASE_ORTHO,_LCASE_ORTHO>	m_arStldPair;		// Search Couple LcaseK.
	CArray<_LCASE_ORTHO,_LCASE_ORTHO>	m_arSplcPair;		// Search Couple LcaseK.

	CMap<_ORTHO_KEY,_ORTHO_KEY,_LCOM_ORTHO,_LCOM_ORTHO>	m_arLcomOrtho;			// key=LcaseK.
	CArray<_LCOM_ORTHO,_LCOM_ORTHO>	m_arLcomOrthoSort;													// Sorting.
	CMap<T_STLD_K,T_STLD_K,double,double>	m_arSlcfStldK;	// Omega, Static Load Case.
	CMap<T_SPLC_K,T_SPLC_K,double,double>	m_arSlcfSplcK;	// Omega, Spectrum.
    CMap<T_STLD_K, T_STLD_K, double, double>	m_arSlcfStldK_IS;//IS18168 Live Load
	double m_dSlcmSds;			// Special Load Combination Factor (Sds).
	double m_dSlcmSpecial;	// Special Load Combination Factor.
	double m_dSlcmVertical;	// Vertical Load Combination Factor.
	// Add by GAY. PMS:4267. ('12.03.13). ¾ç¹æÇâ Á¶ÇÕ½Ã¿¡´Â ES¸¦ ¹èÁ¦ÇÏ°í »ý¼º.
	BOOL m_bCombWithEccn; // TRUE=Include Accidental Eccentricity Load, FALSE=Exclude Accidental Eccentricity Load.
	BOOL m_bOrthCombWithLeadingEccn; // NTC_DCEC_2017; othogonal ÇÏÁß Áß leading ÇÏÁß¸¸ eccentricity °í·Á

    std::map<EQEP_GROUP_K, std::vector<int>> m_mStatEP;
    std::map<EQEP_GROUP_K, std::vector<int>> m_mSeisEP;
    std::map<int, std::vector<T_LCOM_BASE>> m_mAddUGEP;  /// int = T_LCOM_D>nSeisLcomType, std::vector = additional lcom base list for earthpressure.
    double m_dModiFactoUGSTEP;
    
	_LCOM_WIND_GROUP_MAP m_mWindGroup;
	CArray<T_LCOM_K, T_LCOM_K> m_arWindCombLcomK;

	BOOL m_bHorEarth;
	double m_dHorEarth_Strength;
	double m_dHorEarth_Service;

    BOOL m_bNonDissipative;

private:
    void GetCombBaseSplc(BOOL bChinaCode, OUT CombBase& rcESP, OUT CombBase& rcESV);
    void GetCombBaseEspc(OUT CombBase& rcESP);
    void GetCombBaseSplcND(OUT CombBase& rcESPE);
    void GetCombBaseEspcND(OUT CombBase& rcESPE);
    void SetCombBaseConsiderCS(int nConstLoad, CombBase** cDL, CombBase** cCSD, CombBase** cLL, CombBase** cCSL, 
        CombBase** cCR = nullptr, CombBase** cCSC = nullptr, CombBase** cSH = nullptr, CombBase** cCSS = nullptr);
    BOOL IsExistLL(int nConstLoad, CombBase* cLL, CombBase* cCSL);

    size_t GetLcomBaseListUGEP(int nSeisLoadType, int nSeisK, double dFactStatEP, double dFactSeisEP, OUT std::vector<T_LCOM_BASE>& vLcomBase);
    void   GetLcomBaseListUGEP_SRSS(BOOL bUseOmega, int nSeisLoadType, int nSeisK, int nDir, OUT std::vector<T_LCOM_BASE>& vSTEP, OUT std::vector<T_LCOM_BASE>& vSEEP);
    void   SetModiFactorForUGSTEP(const CArray<double, double>& adFact, const CArray<CombBase, CombBase>& aCombBase);
        
    double GetPSLossFactorTransfer();
    double GetPSLossFactorService();
        
	//======================================================================
	// To Set_LDB 3~12 parameters    -------> Add by Hong,jiseon 2002.8.5
	//======================================================================
protected:
	void SetKeyDataforOrtho(int nLoadCaseType, unsigned int iKey1, unsigned int iKey2); // Coded by Seungjun MNet:No.2516 ('20061016)
	unsigned int GetKeyDataForOrtho(unsigned int iKey1, BOOL bSpec=FALSE);              // Coded by Seungjun MNet:No.2516 ('20061016)
	void GetLcomOrthoForSRSS(unsigned int LcaseKey, unsigned int LcaseType);                                    // Coded by Seungjun MNet:No.2516 ('20061017)

	void SetAddSeisLcomParam();
    void SetAddSeisLcomParamUG();
    void SetScaleFactorUG();
    void SetEQEPLoadGroupUG();    
    size_t GetUGEPSrssLcomBaseList(T_LCOM_K SrssEQLcomK, double dFactSeisEP, std::vector<std::vector<T_LCOM_BASE>>& raSTLcomBase);

    void SetScaleFactorLive();
    
    BOOL IsStageCS();

	void Make_LoadCombination(CString strReport, CArray<double, double>& arFactor, CArray<CombBase,CombBase>& arLoadcase, int nActive);
	int Make_LCB_SearchGroup(CString& strReport, CArray<CombBase,CombBase>& arLoadcase);
	BOOL Make_LCB_CreateDecision(CString& strReport, CArray<CombBase,CombBase>& arLoadcase, int iKey);
	void Make_LCB_ArrangeDescription(CString& strNew, int nActive); // Coded by Seungjun MNet:No.2393 ('20060925)
	void Make_LCB_Makelcb(CString& strReport, CArray<double, double>& arFactor, CArray<CombBase,CombBase>& arLoadcase, CArray<T_LCOM_D,T_LCOM_D&>& arData, int nActive);
	// Add by ZINU.('06.07.28). NO:2341, Exist Accidental Eccentricity.
	void Make_LCB_OnlyRespEccn(int iCK, CombBase* cESP);

	BOOL Make_LCB_Makelcb_Static(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData, int nActive);
	BOOL Make_LCB_Makelcb_Static(int LeadingLL, int& LC_No1, int& LC_No2, CString& strReport, double dFac1, double dFac2, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData, int nActive, double dfac); // Coded by Seungjun MNet:No.2393 ('20060922)
	BOOL Make_LCB_Makelcb_WL(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData, int nActive);
	BOOL Make_LCB_Makelcb_WL_Comb(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_TL_GB10(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData, int nActive);//add by maxiao 2013-10-10
	BOOL m_bGB50010_10;//add by maxiao 2013-10-10
	BOOL m_bGB50068_2018;//add by xuezc 2020-1-13
	BOOL m_bGB50010_19;
	BOOL Make_LCB_Makelcb_EL(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_EL_Taiwan(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D, T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_EL_100_30_30(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_ESP(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_ESP_100_30_30(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D, T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_ESP_Taiwan(BOOL bSameSign, int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_ESV(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_EVT(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_LI(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_LJ(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_TPTSSUM(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	BOOL Make_LCB_Makelcb_CONST(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);
	// Add by ZINU.('05.04.20). Consider SRSS.
	BOOL Make_LCB_Makelcb_SRSS_EL(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& raLcomD);
	BOOL Make_LCB_Makelcb_SRSS_ESP(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& raLcomD);

    BOOL Make_LCB_Makelcb_SRSS_EL_UGEP(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D, T_LCOM_D&>& arData);
    BOOL Make_LCB_Makelcb_SRSS_ESP_UGEP(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D, T_LCOM_D&>& arData);
	// Coded by Seungjun ('20070430) MNet:No.2778.
	BOOL Make_LCB_Makelcb_Crane(int& LC_No1, int& LC_No2, CString& strReport, double dFac, CombBase& cLoad, CArray<T_LCOM_D,T_LCOM_D&>& arData);


	BOOL Get_SpecialLcomOmega(int iLcasNo, int iAnalType, double& dOmega);
	void Set_SRSS(double dMainFactor, double dSubFactor);    
	void Set_PrevLcomSRSS(BOOL bUseOmega, double dMainFactor, double dSubFactor, CombBase* cEL, CombBase* cESP);

    void Set_PrevLcomSRSS_UGEP(BOOL bUseOmega);
    
	// Modify by GAY. PMS:4267. ('12.03.09). ¹æÇâÀÌ °°Àº Ç³ÇÏÁß, ÁöÁøÇÏÁß °¡Á®¿À±â.
	BOOL GetWind_StldAngle(T_STLD_K stldK, double &dAngle);
	int Get_SameDirWindSeis(CombBase* cWL, CombBase* cEL, CArray<CombBase, CombBase&>& arWind, CArray<CombBase, CombBase&>& arSeis);
	int Get_SameDirWindSplc(CombBase* cWL, CombBase* cESP, CArray<CombBase, CombBase&>& arWind, CArray<CombBase, CombBase&>& arSplc);
	// Modify by GAY. PMS:4450. ('12.03.21). 
	void Make_SubComb_EQ9_1_ACI318_11(int nA, BOOL bStageCS, double dPSS, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_2_ACI318_11(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cEPH, CombBase* cWP, CombBase* cLR, CombBase* cSL, CombBase* cRL,  CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_3_ACI318_11(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD,  CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_4_ACI318_11(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cLR, CombBase* cSL, CombBase* cRL,  CombBase* cWL,  CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_5_ACI318_11(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cSL, CombBase* cEL, CombBase* cESP, CombBase* cPSorTscPS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_6_ACI318_11(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_7_ACI318_11(int nA, BOOL bStageCS, double dPSS, 
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);

	void Make_SubComb_EQ9_1_ACI318_25(int nA, BOOL bStageCS, double dPSS, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_2_ACI318_25(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cEPH, CombBase* cWP, CombBase* cLR, CombBase* cSL, CombBase* cRL,  CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_3_ACI318_25(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD,  CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_4_ACI318_25(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cLR, CombBase* cSL, CombBase* cRL,  CombBase* cWL,  CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_5_ACI318_25(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cSL, CombBase* cEL, CombBase* cESP, CombBase* cPSorTscPS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_6_ACI318_25(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_7_ACI318_25(int nA, BOOL bStageCS, double dPSS, 
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);

	bool IsConsiderLateralSoil(CombBase* cEHP, CombBase* cWP);
	// Modify by GAY. PMS:4450. ('12.05.14). for ACI318-08.
	void Make_SubComb_EQ9_1_ACI318_08(int nA, BOOL bStageCS, double dPSS, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPSorTS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_2_ACI318_08(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cTL, CombBase* cCR, CombBase* cSH, CombBase* cCSC, CombBase* cCSS,
		CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cEP, CombBase* cWP,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_3_ACI318_08(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cWL,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_4_ACI318_08(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cWL,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_5_ACI318_08(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
		CombBase* cEL, CombBase* cESP, CombBase* cSL, CombBase* cPSorTS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_6_ACI318_08(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_7_ACI318_08(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP, CombBase* cEP, CombBase* cWP, CombBase* cPSorTScPS,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);

	void Make_SubComb_EQ1_KCI_USD12(int nA, BOOL bStageCS, double dPSS, 
		CombBase* cDL,  CombBase* cFP, CombBase* cPS, CombBase* cTS,  CombBase* cCSD,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ2_KCI_USD12(int nA, BOOL bStageCS, double dPSS, double dEV,
		CombBase* cDL,  CombBase* cLL, CombBase* cLR, CombBase* cSL,  CombBase* cRL,  CombBase* cTL,  CombBase* cSH,  CombBase* cCR, 
		CombBase* cFP,  CombBase* cPS, CombBase* cTS, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cEPH, 
		CombBase* cEPV,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ3_KCI_USD12(int nA, BOOL bStageCS, double dPSS, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL, CombBase* cLR, CombBase* cWL,  CombBase* cSL,  CombBase* cRL,  CombBase* cPS,  CombBase* cTS, 
		CombBase* cCSD, CombBase* cCSL,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ4_KCI_USD12(int nA, BOOL bStageCS, double dPSS, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL, CombBase* cLR, CombBase* cWL,  CombBase* cSL,  CombBase* cRL,  CombBase* cPS,  CombBase* cTS,
		CombBase* cCSD, CombBase* cCSL,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ5_KCI_USD12(int nA, BOOL bStageCS, double dPSS, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  CombBase* cPS,  CombBase* cESP, CombBase* cTS,  CombBase* cCSD, 
		CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ5_KCI_USD12_Case1(int nActive, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
		CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
		CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial);
	void Make_SubComb_EQ5_KCI_USD12_Case2(int nActive, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
		CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
		CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial);
	void Make_SubComb_EQ5_KCI_USD12_Case3(int nActive, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
		CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
		CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial);
	void Make_SubComb_EQ5_KCI_USD12_Case4(int nActive, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL,  CombBase* cEL, CombBase* cSL,  
		CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV,
		CombBase* cPSorTs, CString csPSorTS, double dPSS, BOOL bSlcmSpecial);
	void Make_SubComb_EQ6_KCI_USD12(int nA, BOOL bStageCS, double dPSS, double dEV,
		CombBase* cDL,  CombBase* cLL,  CombBase* cLR, CombBase* cSL,  CombBase* cRL,  CombBase* cTL,  CombBase* cSH,  CombBase* cCR, 
		CombBase* cFP,  CombBase* cPS,  CombBase* cTS, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cEPH, 
		CombBase* cEPV,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ7_KCI_USD12(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL,  CombBase* cWL, CombBase* cPS,  CombBase* cTS,  CombBase* cCSD, CombBase* cEPH, CombBase* cEPV,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ8_KCI_USD12(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL,  CombBase* cEL, CombBase* cPS,  CombBase* cESP, CombBase* cTS,  CombBase* cCSD, CombBase* cEPH, CombBase* cEPV,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_KCI_USD12_Serv(int nA, BOOL bExistLL, const CString &strPS, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
		CombBase* cLL,  CombBase* cCSL, CombBase* cTL, CombBase* cEPV, CombBase* cEPH,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL,
		CombBase* cWL, CombBase* cEL,  CombBase* cESP, CombBase* cPSorTS);
	void Make_SubComb_KCI_USD12_Serv_FDN(int nA, BOOL bExistLL, const CString &strPS, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
		CombBase* cLL,  CombBase* cCSL, CombBase* cTL, CombBase* cEPV, CombBase* cEPH,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL,
		CombBase* cWL, CombBase* cEL,  CombBase* cESP, CombBase* cPSorTS);
	void Make_SubComb_EQ9_1_ACI318_05(int nA, BOOL bStageCS, double dPSS, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_2_ACI318_05(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cTL, CombBase* cCR, CombBase* cSH, CombBase* cCSC, CombBase* cCSS,
		CombBase* cLL, CombBase* cCSL, CombBase* cEP, CombBase* cWP,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_3_ACI318_05(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_4_ACI318_05(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_5_ACI318_05(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL,
		CombBase* cEL, CombBase* cESP, CombBase* cSL, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_6_ACI318_05(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_7_ACI318_05(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP, CombBase* cEP, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTs,
		BOOL bSlcmSpecial, BOOL bSlcmVertical);

	// MQC_ab_5009_15 Add Colombia RC Code ; ÇÏÁßÁ¶ÇÕ ¼öÁ¤
	void Make_SubComb_EQ9_3_NSR_10(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL,                                                 
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTs, 
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_4_NSR_10(int nA, BOOL bStageCS, BOOL bExistLL, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL, CombBase* cPSorTs, 
		BOOL bSlcmSpecial, BOOL bSlcmVertical);
	void Make_SubComb_EQ9_6_NSR_10(int nA, BOOL bStageCS, double dPSS,
		CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEP, CombBase* cEPH, CombBase* cWP, CombBase* cPSorTs, 
		BOOL bSlcmSpecial, BOOL bSlcmVertical);

	void Make_SubComb_EQ1_AISC_ASD05(int nA, CombBase* cDL, CombBase* cFP, CombBase* cPS, CombBase* cCSD);
	void Make_SubComb_EQ2_AISC_ASD05(int nA, BOOL bExistWPEP,
		CombBase* cDL, CombBase* cLL, CombBase* cTL,  CombBase* cWP,  CombBase* cEP, 
		CombBase* cFP, CombBase* cPS, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, 
		CombBase* cEPV);
	void Make_SubComb_EQ3_AISC_ASD05(int nA, BOOL bExistWPEP,
		CombBase* cDL,  CombBase* cLL,  CombBase* cLR, CombBase* cSL, CombBase* cRL, 
		CombBase* cTL,  CombBase* cWP,  CombBase* cEP, CombBase* cFP, CombBase* cPS, 
		CombBase* cCSD, CombBase* cEPH, CombBase* cEPV);
	void Make_SubComb_EQ4_AISC_ASD05(int nA, BOOL bExistWPEP,
		CombBase* cDL,  CombBase* cLL,  CombBase* cLR, CombBase* cSL, CombBase* cRL, 
		CombBase* cTL,  CombBase* cWP,  CombBase* cEP, CombBase* cFP, CombBase* cPS, 
		CombBase* cCSD, CombBase* cEPH, CombBase* cEPV);
	void Make_SubComb_EQ5_AISC_ASD05(int nA, BOOL bExistWPEP,
		CombBase* cDL,  CombBase* cLL,  CombBase* cLR,  CombBase* cWL,  CombBase* cEL, 
		CombBase* cSL,  CombBase* cRL,  CombBase* cWP,  CombBase* cEP,  CombBase* cFP,  
		CombBase* cPS,  CombBase* cESP, CombBase* cCSD, CombBase* cEPH, CombBase* cEPV);
	void Make_SubComb_EQ6_AISC_ASD05(int nA, BOOL bExistWPEP, BOOL bExistLL,
		CombBase* cDL,  CombBase* cLL,  CombBase* cLR,  CombBase* cWL,  CombBase* cEL, 
		CombBase* cSL,  CombBase* cRL,  CombBase* cWP,  CombBase* cEP,  CombBase* cFP,  
		CombBase* cPS,  CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, 
		CombBase* cEPV);
	void Make_SubComb_EQ7_AISC_ASD05(int nA, BOOL bExistWPEP,
		CombBase* cDL,  CombBase* cWL,  CombBase* cWP, CombBase* cEP, CombBase* cPS, 
		CombBase* cCSD, CombBase* cEPH, CombBase* cEPV);
	void Make_SubComb_EQ8_AISC_ASD05(int nA, BOOL bExistWPEP,
		CombBase* cDL,  CombBase* cEL,  CombBase* cWP,  CombBase* cEP, CombBase* cPS, 
		CombBase* cESP, CombBase* cCSD, CombBase* cEPH, CombBase* cEPV);

	void Make_SubComb_KBC_USD09_Serv(int nA, BOOL bExistLL, const CString &strPS, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
		CombBase* cLL,  CombBase* cCSL, CombBase* cTL, CombBase* cEPV, CombBase* cEPH,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL,
		CombBase* cWL, CombBase* cEL,  CombBase* cESP, CombBase* cPSorTS);
	void Make_SubComb_KBC_USD09_Serv_FDN(int nA, BOOL bExistLL, const CString &strPS, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
		CombBase* cLL,  CombBase* cCSL, CombBase* cTL, CombBase* cEPV, CombBase* cEPH,
		CombBase* cLR, CombBase* cSL,  CombBase* cRL,
		CombBase* cWL, CombBase* cEL,  CombBase* cESP, CombBase* cPSorTS);

	/////////////////////////////////////////////////////////////////////////////////
	// for ASCE7-02 ASD.
	/////////////////////////////////////////////////////////////////////////////////
	void Make_SubComb_EQ1_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS,
		CombBase* cDL, CombBase* cFP, CombBase* cCSD, CombBase* cPSorTS);

	void Make_SubComb_EQ2_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistWPEP,
		CombBase* cDL, CombBase* cLL, CombBase* cWP,  CombBase* cEP, CombBase* cEPH,
		CombBase* cFP, CombBase* cTL, CombBase* cCSD, CombBase* cCSL, CombBase* cPSorTS);

	void Make_SubComb_EQ3_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistWPEP, BOOL bExistLSR,
		CombBase* cDL, CombBase* cLR,  CombBase* cSL, CombBase* cRL, 
		CombBase* cWP, CombBase* cEP,  CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cPSorTS);

	void Make_SubComb_EQ4_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistWPEP, BOOL bExistLSR,
		CombBase* cDL, CombBase* cLL, CombBase* cLR,  CombBase* cSL, CombBase* cRL, 
		CombBase* cWP, CombBase* cEP, CombBase* cEPH,
		CombBase* cFP, CombBase* cTL, CombBase* cCSD,  CombBase* cCSL, CombBase* cPSorTS);

	void Make_SubComb_EQ5_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistWPEP,
		CombBase* cDL, CombBase* cWL,  CombBase* cEL,  CombBase* cESP, 
		CombBase* cWP, CombBase* cEP,  CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cPSorTS);

	void Make_SubComb_EQ6_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistWPEP, BOOL bExistLSR, BOOL bExistLL,
		CombBase* cDL, CombBase* cLL,  CombBase* cLR,  CombBase* cSL, CombBase* cRL, 
		CombBase* cWL, CombBase* cEL,  CombBase* cESP, 
		CombBase* cWP, CombBase* cEP,  CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cCSL, CombBase* cPSorTS);

	void Make_SubComb_EQ7_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistWPEP,
		CombBase* cDL, CombBase* cWL, 
		CombBase* cWP, CombBase* cEP,  CombBase* cEPH,
		CombBase* cCSD, CombBase* cPSorTS);

	void Make_SubComb_EQ8_ASCE7_02_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistWPEP,
		CombBase* cDL, CombBase* cEL,  CombBase* cESP, 
		CombBase* cWP, CombBase* cEP,  CombBase* cEPH,
		CombBase* cCSD, CombBase* cPSorTS);

	/////////////////////////////////////////////////////////////////////////////////
	// for ASCE7-10 ASD.
	/////////////////////////////////////////////////////////////////////////////////
	void Make_SubComb_EQ1_ASCE7_10_LRFD(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPS);

	void Make_SubComb_EQ2_ASCE7_10_LRFD(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cTL, CombBase* cLL, CombBase* cCSL,
		CombBase* cEPH, CombBase* cWP, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPS);

	void Make_SubComb_EQ3_ASCE7_10_LRFD(int nA, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL,
		CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPS);

	void Make_SubComb_EQ4_ASCE7_10_LRFD(int nA, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL, 
		CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPS);

	void Make_SubComb_EQ5_ASCE7_10_LRFD(int nA, BOOL bExistLL, BOOL bSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cSL,
		CombBase* cEL, CombBase* cESP, CombBase* cPS);

	void Make_SubComb_EQ6_ASCE7_10_LRFD(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP, CombBase* cPS);

	void Make_SubComb_EQ7_ASCE7_10_LRFD(int nA, BOOL bSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP,
		CombBase* cEPH, CombBase* cWP, CombBase* cPS);
    
	void Make_SubComb_EQ1_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS,
		CombBase* cDL, CombBase* cFP, CombBase* cCSD, CombBase* cPSorTS);

	void Make_SubComb_EQ2_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS, 
		CombBase* cDL, CombBase* cLL,  CombBase* cWP,  CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cCSL, CombBase* cPSorTS);

	void Make_SubComb_EQ3_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistLSR,
		CombBase* cDL, CombBase* cLR,  CombBase* cSL, CombBase* cRL, 
		CombBase* cWP, CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cPSorTS);

	void Make_SubComb_EQ4_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistLSR,
		CombBase* cDL, CombBase* cLL,  CombBase* cLR,  CombBase* cSL, CombBase* cRL, 
		CombBase* cWP, CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD,  CombBase* cCSL, CombBase* cPSorTS);

	void Make_SubComb_EQ5_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS,
		CombBase* cDL, CombBase* cWL,  CombBase* cEL,  CombBase* cESP, 
		CombBase* cWP, CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cPSorTS);																		 

	void Make_SubComb_EQ6_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS, BOOL bExistLSR, BOOL bExistLL,
		CombBase* cDL, CombBase* cLL,  CombBase* cLR,  CombBase* cSL, CombBase* cRL, 
		CombBase* cWL, CombBase* cEL,  CombBase* cESP, 
		CombBase* cWP, CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cCSL, CombBase* cPSorTS);

	void Make_SubComb_EQ7_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS,
		CombBase* cDL,  CombBase* cWL, 
		CombBase* cWP,  CombBase* cEPH,
		CombBase* cCSD, CombBase* cPSorTS);

	void Make_SubComb_EQ8_ASCE7_10_ASD(int nA, double dPSS, const CString &strPSorTS,
		CombBase* cDL, CombBase* cEL,  CombBase* cESP, 
		CombBase* cWP, CombBase* cEPH,
		CombBase* cFP, CombBase* cCSD, CombBase* cPSorTS);	

    void Make_SubComb_EQ01_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPS);
    void Make_SubComb_EQ02_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cTL, 
        CombBase* cLL, CombBase* cCSL, CombBase* cEP, CombBase* cEPV, CombBase* cEPH, CombBase* cLR,
        CombBase* cSL, CombBase* cRL, CombBase* cPS);
    void Make_SubComb_EQ03_KBC2009(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD,
        CombBase* cLL, CombBase* cCSL, CombBase* cWL, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPS);
    void Make_SubComb_EQ04_KBC2009(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD, 
        CombBase* cLL, CombBase* cCSL, CombBase* cWL, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPS);
    void Make_SubComb_EQ05_KBC2009(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD,
        CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, CombBase* cSL, CombBase* cPS);
    void Make_SubComb_EQ05_KBC2009_Special(int nA, BOOL bExistLL, double dLf1, CombBase* cDL, CombBase* cCSD,
        CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, CombBase* cSL, CombBase* cPS);
    void Make_SubComb_EQ06_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEP,
        CombBase* cEPV, CombBase* cEPH, CombBase* cPS);
    void Make_SubComb_EQ07_KBC2009(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP,
        CombBase* cEP, CombBase* cEPV, CombBase* cEPH, CombBase* cPS);
    void Make_SubComb_EQ07_KBC2009_Special(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP,
        CombBase* cEP, CombBase* cEPV, CombBase* cEPH, CombBase* cPS);
    	
#pragma region /// [PMS:5002] KBC2016

#pragma region /// [PMS:5002] KBC2016 (for USD or LSD)
    void Make_SubComb_EQ01_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ02_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cTL, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ03_KBC2016(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ04_KBC2016(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ05_KBC2016(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cSL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ06_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ07_KBC2016(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
#pragma endregion

#pragma region /// [PMS:5002] KBC2016 (for ASD)
    void Make_SubComb_EQ08_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ09_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cTL,
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ10_KBC2016(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ11_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cTL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ12_KBC2016(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP,
        CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ13_KBC2016(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ14_KBC2016(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ15_KBC2016(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
#pragma endregion

#pragma endregion

#pragma region /// [GEN-3331] KDS 41 10 15 : 2021
    
#pragma region /// [GEN-3331] KDS 41 10 15 : 2021 (for USD or LSD)
    void Make_SubComb_EQ01_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ02_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cTL, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ03_KDS41_10_15_2021(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ04_KDS41_10_15_2021(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ05_KDS41_10_15_2021(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cSL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ06_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ07_KDS41_10_15_2021(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
#pragma endregion

#pragma region /// [GEN-3331] KDS 41 10 15 : 2021 (for ASD)
    void Make_SubComb_EQ08_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ09_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cTL,
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ10_KDS41_10_15_2021(int nActive, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ11_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cTL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ12_KDS41_10_15_2021(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP,
        CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ13_KDS41_10_15_2021(int nActive, BOOL bExistLL, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cLL, CombBase* cCSL, CombBase* cIL,
        CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ14_KDS41_10_15_2021(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cWL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ15_KDS41_10_15_2021(int nActive, BOOL bSlcmSpecial, CombBase* cDL, CombBase* cCSD, CombBase* cEPV, CombBase* cFP, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
#pragma endregion
    
#pragma endregion

#pragma region /// [GEN-3690] NSCP 2015

#pragma region /// [GEN-3690] NSCP 2015 (for USD)
    void Make_SubComb_EQ01_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ02_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cTL, CombBase* cLL, CombBase* cCSL, CombBase* cEPH, CombBase* cWP, 
        CombBase* cLR, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ03_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cLR, CombBase* cRL, CombBase* cWL, 
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ04_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cLR, CombBase* cRL, CombBase* cWL,
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ05_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP,
        CombBase* cPsTs, const CString& strPsTs, double dPSS, BOOL bSlcmSpecial);
    void Make_SubComb_EQ06_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ07_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP, CombBase* cEPH, CombBase* cWP, 
        CombBase* cPsTs, const CString& strPsTs, double dPSS, BOOL bSlcmSpecial);
#pragma endregion

#pragma region /// [GEN-3690] NSCP 2015 (for ASD)
    void Make_SubComb_EQ08_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cFP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ09_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP, CombBase* cFP, CombBase* cLL, CombBase* cCSL,
        CombBase* cTL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ10_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP, CombBase* cFP, CombBase* cLR, CombBase* cRL,
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ11_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP, CombBase* cFP, CombBase* cLL, CombBase* cCSL,
        CombBase* cTL, CombBase* cLR, CombBase* cRL, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ12_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP, CombBase* cFP, CombBase* cWL, CombBase* cEL,
        CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS, BOOL bSlcmSpecial);
#pragma endregion

#pragma region /// [GEN-3892] NSCP 2015 (for ALTERNATIVE)
    void Make_SubComb_EQ13_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEPH, CombBase* cWP, CombBase* cFP, CombBase* cLL, CombBase* cCSL,
        CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ14_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cWL, CombBase* cEPH, CombBase* cWP, 
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ15_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cEL, CombBase* cESP, CombBase* cEPH, CombBase* cWP, 
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ16_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cLR, CombBase* cRL,
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ17_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL, 
        CombBase* cPsTs, const CString& strPsTs, double dPSS);
    void Make_SubComb_EQ18_NSCP2015(int nActive, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEL, 
        CombBase* cESP, CombBase* cPsTs, const CString& strPsTs, double dPSS);
#pragma endregion

#pragma endregion

#pragma region /// [GEN-8768] TWN-USD112

	void Make_SubComb_TWN_USD112_EQ5_3_1a(int nA, CombBase* cDL, CombBase* cCSD, 
		CombBase* cFP, CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial);
	void Make_SubComb_TWN_USD112_EQ5_3_1b(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
		CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cLR, CombBase* cSL, CombBase* cRL, 
		CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial);
	void Make_SubComb_TWN_USD112_EQ5_3_1c(int nA, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
		CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL,
		CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial);
	void Make_SubComb_TWN_USD112_EQ5_3_1d(int nA, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cFP, 
		CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cWL, 
		CombBase* cPSorTs, double dPS, const CString& strPS, BOOL bSlcmSpecial);
	void Make_SubComb_TWN_USD112_EQ5_3_1e(int nA, BOOL bExistLL, CombBase* cDL, CombBase* cCSD, CombBase* cFP, 
		CombBase* cLL, CombBase* cCSL, CombBase* cIL, CombBase* cSL, CombBase* cEL, CombBase* cESP, 
		CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial);
	void Make_SubComb_TWN_USD112_EQ5_3_1f(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cWL, 
		CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial);
	void Make_SubComb_TWN_USD112_EQ5_3_1g(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cFP,
		CombBase* cEL, CombBase* cESP, CombBase* cPS, double dPS, const CString& strPS, BOOL bSlcmSpecial);

#pragma endregion

#pragma region /// Thailand(2021) - Strength
	void Make_SubComb_Thai_Strn_EQ1(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL);
	void Make_SubComb_Thai_Strn_EQ2(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL);
	void Make_SubComb_Thai_Strn_EQ3(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, BOOL bSlcmSpecial);
	void Make_SubComb_Thai_Strn_EQ4(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEP, CombBase* cFP);
	void Make_SubComb_Thai_Strn_EQ5(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cTL);
#pragma endregion

#pragma region /// Thailand(2021) - Service.
	void Make_SubComb_Thai_Serv_EQ1(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL);
	void Make_SubComb_Thai_Serv_EQ2(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cWL);
	void Make_SubComb_Thai_Serv_EQ3(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cEL, CombBase* cESP, BOOL bSlcmSpecial);
	void Make_SubComb_Thai_Serv_EQ4(int nA, CombBase* cDL, CombBase* cCSD, CombBase* cLL, CombBase* cCSL, CombBase* cTL, CombBase* cEP, CombBase* cFP);
#pragma endregion
        
	CString m_strCurrentCode; // Add 01.10.28 ´ë¸¸ ÇÏÁß°ú °°ÀÌ Æ¯ÀÌÇÑ ±âÁØ¿¡ ´ëÇØ Ã³¸®ÇÏ±â À§ÇØ

	// Change by ZINU.('02.12.18). Use overriding.
	void Set_LCB(int nAcitve, CString strReport,
		double dFac01=0.0, CombBase* pLoad01=NULL,
		double dFac02=0.0, CombBase* pLoad02=NULL,
		double dFac03=0.0, CombBase* pLoad03=NULL,
		double dFac04=0.0, CombBase* pLoad04=NULL,
		double dFac05=0.0, CombBase* pLoad05=NULL,
		double dFac06=0.0, CombBase* pLoad06=NULL,
		double dFac07=0.0, CombBase* pLoad07=NULL,
		double dFac08=0.0, CombBase* pLoad08=NULL,
		double dFac09=0.0, CombBase* pLoad09=NULL,
		double dFac10=0.0, CombBase* pLoad10=NULL,
		double dFac11=0.0, CombBase* pLoad11=NULL,
		double dFac12=0.0, CombBase* pLoad12=NULL,
		double dFac13=0.0, CombBase* pLoad13=NULL,
		double dFac14=0.0, CombBase* pLoad14=NULL,
		double dFac15=0.0, CombBase* pLoad15=NULL,
		double dFac16=0.0, CombBase* pLoad16=NULL,
		double dFac17=0.0, CombBase* pLoad17=NULL,
		double dFac18=0.0, CombBase* pLoad18=NULL,
		double dFac19=0.0, CombBase* pLoad19=NULL,
		double dFac20=0.0, CombBase* pLoad20=NULL,
		double dFac21=0.0, CombBase* pLoad21=NULL,
		double dFac22=0.0, CombBase* pLoad22=NULL);
	void Set_LCB(int nAcitve, CString strReport, const std::vector<std::pair<double, CombBase*>>& vCombBase);
	void Set_LCB(int iLLLeading, int nAcitve, CString strReport,
		double dFac01=0.0, CombBase* pLoad01=NULL,
		double dFac02=0.0, CombBase* pLoad02=NULL,
		double dFac03=0.0, CombBase* pLoad03=NULL,
		double dFac04=0.0, CombBase* pLoad04=NULL,
		double dFac05=0.0, CombBase* pLoad05=NULL,
		double dFac06=0.0, CombBase* pLoad06=NULL,
		double dFac07=0.0, CombBase* pLoad07=NULL,
		double dFac08=0.0, CombBase* pLoad08=NULL,
		double dFac09=0.0, CombBase* pLoad09=NULL,
		double dFac10=0.0, CombBase* pLoad10=NULL,
		double dFac11=0.0, CombBase* pLoad11=NULL,
		double dFac12=0.0, CombBase* pLoad12=NULL,
		double dFac13=0.0, CombBase* pLoad13=NULL,
		double dFac14=0.0, CombBase* pLoad14=NULL,
		double dFac15=0.0, CombBase* pLoad15=NULL,
		double dFac16=0.0, CombBase* pLoad16=NULL,
		double dFac17=0.0, CombBase* pLoad17=NULL,
		double dFac18=0.0, CombBase* pLoad18=NULL,
		double dFac19=0.0, CombBase* pLoad19=NULL,
		double dFac20=0.0, CombBase* pLoad20=NULL,
		double dFac21=0.0, CombBase* pLoad21=NULL,
		double dFac22=0.0, CombBase* pLoad22=NULL);
	// Add by SHIN ('11.08.23)
	void Set_LCB_JUD(int nAcitve, BOOL bDL, BOOL bLL, CString strReport,
		double dFac01=0.0, CombBase* pLoad01=NULL,
		double dFac02=0.0, CombBase* pLoad02=NULL,
		double dFac03=0.0, CombBase* pLoad03=NULL,
		double dFac04=0.0, CombBase* pLoad04=NULL,
		double dFac05=0.0, CombBase* pLoad05=NULL,
		double dFac06=0.0, CombBase* pLoad06=NULL,
		double dFac07=0.0, CombBase* pLoad07=NULL,
		double dFac08=0.0, CombBase* pLoad08=NULL,
		double dFac09=0.0, CombBase* pLoad09=NULL,
		double dFac10=0.0, CombBase* pLoad10=NULL,
		double dFac11=0.0, CombBase* pLoad11=NULL,
		double dFac12=0.0, CombBase* pLoad12=NULL,
		double dFac13=0.0, CombBase* pLoad13=NULL,
		double dFac14=0.0, CombBase* pLoad14=NULL,
		double dFac15=0.0, CombBase* pLoad15=NULL,
		double dFac16=0.0, CombBase* pLoad16=NULL,
		double dFac17=0.0, CombBase* pLoad17=NULL,
		double dFac18=0.0, CombBase* pLoad18=NULL,
		double dFac19=0.0, CombBase* pLoad19=NULL,
		double dFac20=0.0, CombBase* pLoad20=NULL,
		double dFac21=0.0, CombBase* pLoad21=NULL,
		double dFac22=0.0, CombBase* pLoad22=NULL);
	// Add by ZINU.('02.12.18).	To fix figure under point automatically.
	BOOL Get_RealLcaseFactor(double dFactor, CString sFactor, CString& strTxt);
	BOOL Get_RealLcaseName(const CString& sOrgLcNa, const CString& sCngLcNa, CString& strTxt);

	// Coded by Seungjun MNet:No.2532 ('20061025)
	BOOL IsExistedLcase(unsigned int Lcase, CombBase& cLoad, BOOL IsStatic=TRUE);
	void Write_GDisplayAnalString(CString strTxt, BOOL bClearPre=TRUE, BOOL bSpacePost=TRUE);
	void Check_OrthonalCases(CombBase& cEL, CombBase& cEVT, CombBase& cESP, CombBase& cESV, CombBase& cESPE);

	//=========================
	// Cold Form Steel Design Codes
	//=========================
	void MakeCFSComb_EURO_CFSD05(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cIL, CombBase* cCO);
	void MakeCFSComb_EURO_CFSD06(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cIL, CombBase* cCO);
	//Add by tss(2017.8.3) GB50018-02	
	void MakeSTLComb_GB50018_02 (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);

	void MakeRCComb_GB50010_19(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	//=========================
	// STEEL Design Codes
	//=========================
	void MakeSTLComb_AIK_ASD83   (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AISC_ASD89  (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AIK_LSD97   (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AIK_CFSD98  (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AISC_LRFD2K (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AISC_LRFD93 (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AIJ_ASD02   (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AISI_CFSD86 (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_AISI_CFSD08 (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_BS5950_90   (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_BS5950_2K   (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);  // Coded by Seungjun MNET:No.2086 BS5950-2K ('20060623)
	void MakeSTLComb_Eurocode3   (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_Eurocode3_05(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cIL, CombBase* cCO);
	void MakeSTLComb_GBJ17_88    (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCN1, CombBase* cCN2);//, CombBase* cBRK);
	void MakeSTLComb_GB50017_03  (CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCN1, CombBase* cCN2);// CombBase* cBRK);
	//add by maxiao(2015-9-25)GB50017-15
	void MakeSTLComb_GB50017_15( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCN1, CombBase* cCN2);// CombBase* cBRK);
	//add by xuezc(2018/2/8)GB50017-17
	void MakeSTLComb_GB50017_17(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	//add by tss(2022/03/11) GB55006_2021
	void MakeSTLComb_GB55006_2021(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	//add by tss(2019/9) GB 51249-2017
	void MakeSTLComb_GB51249_2017(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	//add by tss(20210729)GB55001-2021
	void MakeComb_GB55001_2021(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	//add by tss(2019/6/10)GB50068-18
	void MakeComb_GB50068_2018(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	void MakeComb_GB50068_2019(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	void MakeSTLComb_CSA_S16_01( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_IS800_1984( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_IRS_SBC_2003( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_IS800_2007( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cIL, CombBase* cCO, CombBase* cER,  CombBase* cESPE);// MNET:3774-Seungjun-20081029
	void MakeSTLComb_TWN_LSD90(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_TWN_ASD90(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_TWN_LSD96(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_TWN_ASD96(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSTLComb_KBC_ASD05( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL); // add by Seungjun (`06.06.14) No.2079
	void MakeSTLComb_KBC_LSD09( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV); // Add by GAY. PMS:4035. ('09.07.10).  
	void MakeSTLComb_AISC_LRFD05(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV); // Add by GAY. PMS:4061. ('09.09.04).
	void MakeSTLComb_AISC_ASD05( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV); // Add by GAY. PMS:4061. ('09.09.04).
	void MakeSTLComb_AISC_LRFD10(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL); // Add by GAY. PMS:XXXX. ('13.01.16).
	void MakeSTLComb_AISC_ASD10( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL); // Add by GAY. PMS:XXXX. ('13.01.16).
	void MakeSTLComb_AISC_LRFD16(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL); // Add by PHJ. PMS:5639. ('18.04.18).
	void MakeSTLComb_AISC_ASD16( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL); // Add by PHJ. PMS:5639. ('18.04.18).
	//=========================
	// RC Design Codes
	//=========================
	void MakeRCComb_AIK_USD94(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_KSCE_USD96(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_KCI_USD99(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_KCI_USD03(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_KBC_USD05(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_KBC_USD08(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cEPH, CombBase* cEPV);
	void MakeRCComb_KCI_USD12(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cEPH, CombBase* cEPV);
	void MakeRCComb_AIK_WSD2K(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_ACI318_89(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_ACI318_95(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_ACI318_99(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_BS8110_97(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_Eurocode2(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_Eurocode2_04(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL, CombBase* cCO, CombBase* cEE, CombBase* CESPE);  
	void MakeRCComb_AIJ_WSD99(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_GB50069_02(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cSF, CombBase* cEPH, CombBase* cEPV);// CombBase* cBRK);
	void MakeRCComb_GB50010_02(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2);// CombBase* cBRK);
	void MakeRCComb_GB50010_10(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2, CombBase* cCO, CombBase* cEX);
	void MakeRCComb_CSA_A23_94(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_ACI318_02(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_ACI318_05(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_ACI318_08(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL); // Add by GAY. PMS:4450. ('12.03.21)
	void MakeRCComb_ACI318_11(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL, CombBase* cEPH); // Add by GAY. PMS:4450. ('12.03.21)
	void MakeRCComb_ACI318_25(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL, CombBase* cEPH); // Add by GAY. PMS:4450. ('12.03.21)
	void MakeRCComb_NSR_10   (   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cEPH, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_IS456_2000(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_TWN_USD92(   CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_TWN_USD100(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeRCComb_TWN_USD112(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL);
	void MakeRCComb_NTC_DCEC_2017(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	BOOL MakeWindEnvelop(const CombBase* cWL,int ActiveType, int& MaxEnvelopId, CMap<int,int, int, int>& aEnvelopIdMap, int nType);
	void LcomW2Envelope(const int MaxStrEnvelopId,const int MaxSerEnvelopId,const  CMap<int,int, int, int>& aStrEnvelopIdMap,const CMap<int,int, int, int>& aSerEnvelopIdMap, int nType);
	//////////////////////////////////////////////////////////////////////////
	void MakeLCB1x1(int iCK, double dSDL, LPCTSTR strsDL, CombBase* sDL, double dDL, LPCTSTR strDL, CombBase* cAllDL, 
		double dLL1,  LPCTSTR strLL1, CombBase* cLL1);
	void MakeLCB2x2(int iCK, double dSDL, LPCTSTR strsDL, CombBase* sDL, double dDL, LPCTSTR strDL, CombBase* cAllDL, 
		double dLL1, double dLL2, LPCTSTR strLL1, CombBase* cLL1,  LPCTSTR strLL2, CombBase* cLL2);
	void MakeLCB3x3(int iCK, double dDL, LPCTSTR strDL, CombBase* cAllDL, 
		double dLL1,  LPCTSTR strLL1, CombBase* cLL1,  double dLL2=0, LPCTSTR strLL2=_T(""), CombBase* cLL2=nullptr,  double dLL3=0, 
		LPCTSTR strLL3=_T(""), CombBase* cLL3=nullptr,  double dLL4=0,  LPCTSTR strLL4=_T(""), CombBase* cLL4=nullptr);
	void MakeLCB3x3(int iCK, double dSDL, LPCTSTR strsDL, CombBase* sDL, double dDL, LPCTSTR strDL, CombBase* cAllDL, 
		double dLL1, double dLL2, LPCTSTR strLL1, CombBase* cLL1,  LPCTSTR strLL2, CombBase* cLL2,  LPCTSTR strLL3, CombBase* cLL3);
	void MakeLCBEQ_GB50069_02(int iCK,  LPCTSTR strsDL, CombBase* cDL,  LPCTSTR strsLL, CombBase* cLL, LPCTSTR strsEL, CombBase* cEL ,
		LPCTSTR strsVEL, CombBase* cVEL, LPCTSTR strWL, CombBase* cWL, BOOL bBasic);
	//
	void MakeLCBFull(int iCK, BOOL bBasic, double dSDL, LPCTSTR strsDL, CombBase* sDL, double dDL, LPCTSTR strDL, 
		CombBase* cAllDL, double dMainLL, double dSubLL, LPCTSTR strLL1, CombBase*pLL1, LPCTSTR strLL2=_T(""), CombBase*pLL2=NULL, 
		LPCTSTR strLL3=_T(""), CombBase*pLL3=NULL, LPCTSTR strLL4=_T(""), CombBase*pLL4=NULL);
	void MakeLCBFull(int iCK, double dSDL, LPCTSTR strsDL, CombBase* sDL, double dDL, LPCTSTR strDL, CombBase* cAllDL, const CArray<double, double>&arRatio, const CStringArray& arDes, const CArray<CombBase*, CombBase*>&arLoad);
	//=========================
	// SRC Design Codes
	//=========================
	void MakeSRCComb_AIK_SRC2K(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSRCComb_KBC_SRC05(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSRCComb_KBC_SRC09(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cEPH, CombBase* cEPV);
	void MakeSRCComb_SSRC79(     CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSRCComb_AIJ_SRC01(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeSRCComb_JGJ138_01(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cEVT, CombBase* cESV, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cCN1, CombBase* cCN2);//, CombBase* cBRK);
	void MakeSRCComb_TWN_SRC92(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);
	void MakeSRCComb_TWN_SRC100( CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS);

	//=========================
	// STEEL Design Codes
	//=========================
	void MakeALUComb_AA(  CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeALUComb_AA_ASD05(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
	void MakeALUComb_AA_LRFD05(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);

	//=========================
	// Building or Common Codes
	//=========================
	void MakeComb_KBC2016(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR, 
                          CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL, CombBase* cEPV);
    void MakeComb_KDS41_2018(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR,
                             CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL, CombBase* cEPV);
    void MakeComb_KDS41_2021(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cSL, CombBase* cRL, CombBase* cIP, CombBase* cTL, CombBase* cSH, CombBase* cCR,
        CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL, CombBase* cCSC, CombBase* cCSS, CombBase* cIL, CombBase* cEPV);
    void MakeComb_NSCP2015(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cRL, CombBase* cTL, CombBase* cSH, CombBase* cCR, 
        CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL,
        CombBase* cCSC, CombBase* cCSS);
    void MakeComb_NSCP2015_ASD(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cWL, CombBase* cEL, CombBase* cRL, CombBase* cTL, CombBase* cSH, CombBase* cCR,
        CombBase* cWP, CombBase* cEP, CombBase* cFP, CombBase* cPS, CombBase* cESP, CombBase* cTP, CombBase* cTS, CombBase* cSUM, CombBase* cCSD, CombBase* cCSL,
        CombBase* cCSC, CombBase* cCSS);
	void MakeComb_Thailand_2021(CombBase* cDL, CombBase* cLL, CombBase* cWL, CombBase* cEL, CombBase* cTL, CombBase* cWP, CombBase* cEP, 
		CombBase* cFP, CombBase* cESP, CombBase* cCSD, CombBase* cCSL);
    
#pragma region /// MAKE LOAD COMBINATION FOR SEISMIC EVALUATION

	void MakeSeisComb_KISTEC2013(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cSL, CombBase* cEL, CombBase* cESP);
	void MakeSeisComb_MOE2018(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cSL, CombBase* cEL, CombBase* cESP);    
	void MakeSeisComb_KISTEC2019(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cSL, CombBase* cEL, CombBase* cESP);
    void MakeSeisComb_MOE2018_Linear(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cSL, CombBase* cRL, CombBase* cEL, CombBase* cESP);    
	
#pragma endregion

	////////////////////////////////////////////////////////////////////////////
	// Add by GAY. PMS:4215. ('11.01.17). Load Combination for JUD.
	////////////////////////////////////////////////////////////////////////////
	void MakeComb_JUD(CombBase* cDL, CombBase* cLL, CombBase* cLR, CombBase* cSL, CombBase* cWLX, CombBase* cWLY, CombBase* cELX, CombBase* cELY, CombBase* cPS, CombBase* cESPX, CombBase* cESPY, CombBase* cCSD, CombBase* cCSL);
	//BOOL IsDirX_Wind_JUD(T_KEY StldK);
	//BOOL IsDirX_Seis_JUD(T_KEY StldK);
	BOOL IsDirX_Splc_JUD(T_KEY SplcK);

	// Modify by GAY. ('12.08.08). MQC:5041. °íÃþÀÎ °æ¿ì´Â ÁöÁøÇÏÁß Á¶ÇÕÀÌ Ç³ÇÏÁß+ÁöÁøÇÏÁßÀÇ Á¶ÇÕÀÌ µÇ¾î¾ß ÇÏ´Âµ¥,
	// °íÃþ + Ç³ÇÏÁßÀÌ Á¸ÀçÇÏÁö ¾Ê´Â °æ¿ì¿¡´Â ÁöÁøÇÏÁß¿¡ ´ëÇÑ Á¶ÇÕ¸¸ »ý¼ºÇÏµµ·Ï ÇÑ´Ù.
	BOOL IsCombinedWindSeis_ForChina(BOOL bHighrised, BOOL bExistWind);

	// get applied active index.
	int GetActiveIndex(int nActive, BOOL bSpecial, BOOL bVertical);
	int GetActiveIndex(int nA) { return nA + m_nConstLoad*100; };
    int ModifyActiveUG(int nActive);
    void ModifyLcomAddUGEP(OUT T_LCOM_D& rLcomD);

public:

    int Get_LoadCount(const EN_LOAD& enLcase);

	// To Strength Combinations.
	void Set_Dead_LRFD(CString strCode, ArINT& arDLoad, ArINT& arPLoad);
	void Set_DeadLive(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arPLoad);
	void Set_DeadLive_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arPLoad);
	void Set_DeadLiveSnow(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arSLoad, ArINT& arPLoad);
	void Set_DeadLiveSnow_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arSLoad, ArINT& arPLoad);
	void Set_DeadLiveRain_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arRLoad, ArINT& arPLoad);
	void Set_DeadLiveIce_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arIpLoad, ArINT& arPLoad);
	void Set_DeadLiveWind(CString strCode, ArINT&	arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadLiveWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadLiveSnowWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arSLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadLiveRainWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arRLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadLiveIceWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arIpLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadWind(CString strCode, ArINT& arDLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadLiveSpectrum(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArUNT& arEspLoad, ArINT& arPLoad);
	void Set_DeadLiveSpectrumSnow_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArUNT& arEspLoad, ArINT& arSLoad, ArINT& arPLoad);
	void Set_DeadLiveEq(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arELoad, ArINT& arPLoad);
	void Set_DeadLiveEqSnow_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arELoad, ArINT& arSLoad, ArINT& arPLoad);
	void Set_DeadSpectrum(CString strCode, ArINT& arDLoad, ArUNT& arEspLoad, ArINT& arPLoad);
	void Set_DeadSpectrum_LRFD(CString strCode, ArINT& arDLoad, ArUNT& arEspLoad, ArINT& arPLoad);
	void Set_DeadEq(CString strCode, ArINT& arDLoad, ArINT& arELoad, ArINT& arPLoad);
	void Set_DeadEq_LRFD(CString strCode, ArINT& arDLoad, ArINT& arELoad, ArINT& arPLoad);
	void Set_DeadLiveFluid(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arFLoad, ArINT& arPLoad);
	void Set_DeadLiveThermal(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arTLoad,
		ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPLoad);
	void Set_DeadThermal(CString strCode, ArINT& arDLoad, ArINT& arTLoad,ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPLoad);
	void Set_DeadFluid(CString strCode, ArINT& arDLoad, ArINT& arFLoad, ArINT& arPLoad);
	void Set_DeadLiveEp(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arEpLoad, ArINT& arPLoad);
	void Set_DeadLiveWp(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arWpLoad, ArINT& arPLoad);
	void Set_DeadLiveEpWp(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arEpLoad, ArINT& arWpLoad, ArINT& arPLoad);
	void Set_DeadEp(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArINT& arPLoad);
	void Set_DeadWp(CString strCode, ArINT& arDLoad, ArINT& arWpLoad, ArINT& arPLoad);
	void Set_DeadEpWp(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArINT& arWpLoad, ArINT& arPLoad);
	void Set_DeadLiveSh(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arShLoad, ArINT& arPLoad);
	void Set_DeadSh(CString strCode, ArINT& arDLoad, ArINT& arShLoad, ArINT& arPLoad);
	void Set_DeadLiveCr(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arCrLoad, ArINT& arPLoad);
	void Set_DeadCr(CString strCode, ArINT& arDLoad, ArINT& arCrLoad, ArINT& arPLoad);
	void Set_DeadSnow(CString strCode, ArINT& arDLoad, ArINT& arSLoad, ArINT& arPLoad);
	void Set_DeadSnow_LRFD(CString strCode, ArINT& arDLoad, ArINT& arSLoad, ArINT& arPLoad);
	void Set_DeadRain_LRFD(CString strCode, ArINT& arDLoad, ArINT& arRLoad, ArINT& arPLoad);
	void Set_DeadIce_LRFD(CString strCode, ArINT& arDLoad, ArINT& arIpLoad, ArINT& arPLoad);
	void Set_DeadRLiveWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arLrLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadSnowWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arSLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadRainWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arRLoad, ArINT& arWLoad, ArINT& arPLoad);
	void Set_DeadIceWind_LRFD(CString strCode, ArINT& arDLoad, ArINT& arIpLoad, ArINT& arWLoad, ArINT& arPLoad);

	// To Serviceability Combinations.
	void Set_DeadLive_Service(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad);
	void Set_DeadLiveWind_Service(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arWLoad);
	void Set_DeadLiveSpectrum_Service(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArUNT& arEspLoad);
	void Set_DeadLiveEq_Service(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arLrLoad, ArINT& arELoad);
	void Set_DeadSpectrum_Service(CString strCode, ArINT& arDLoad, ArUNT& arEspLoad);
	void Set_DeadEq_Service(CString strCode, ArINT& arDLoad, ArINT& arELoad);

	/////////////////////////////////////////////////////////
	// CIVIL Combinations for KRTA-BRG2K  ///////////////////
	// To Strength Combinations.
	void Set_CvlDeadMoveCfEpQ(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
		ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arCSLoad, int nType);
	void Set_CvlDeadMoveCfEpQG(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
		ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad,
		ArINT& arShLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad);	
	void Set_CvlDeadMoveCfEpQCo(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
		ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arCoLoad, ArINT& arCSLoad);	
	void Set_CvlDeadEpQW(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arCSLoad);
	void Set_CvlDeadEpQWG(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arTLoad, ArINT& arTpgLoad,
		ArINT& arCSLoad);
	void Set_CvlDeadEpQWCo(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arCoLoad,
		ArINT& arCSLoad);		
	void Set_CvlDeadMoveCfEpQWWlBrk(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad, 
		ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arBrkLoad,
		ArINT& arBkLoad, ArINT& arCSLoad);
	void Set_CvlDeadMoveCfEpQWWlBrkG(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad, 
		ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arBrkLoad, 
		ArINT& arBkLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad, ArINT& arShLoad,
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlDeadEpQE(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arELoad, ArINT& arCSLoad);		
	void Set_CvlDeadEpQEsp(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArINT& arBLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arWprLoad, ArUNT& arEspLoad, ArINT& arCSLoad);

	// To Serviceability Combinations.
	// KSCE-USD03Ãß°¡
	void Set_CvlKSCESer0(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCSLoad);
	void Set_CvlKSCESer0_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCSLoad);
	// ÁÖÇÏÁß + ÁÖÇÏÁß¿¡ ÇØ´çÇÏ´Â Æ¯¼öÇÏÁß(Moving Load Case)
	void Set_CvlKrtaSer0_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arSLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arWprLoad, ArINT& arCfLoad, ArINT& arCSLoad);
	// ÁÖÇÏÁß + ÁÖÇÏÁß¿¡ ÇØ´çÇÏ´Â Æ¯¼öÇÏÁß + ¿Âµµº¯È­ÀÇ ¿µÇâ(Moving Load Case)
	void Set_CvlKrtaSer1_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arSLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arWprLoad, ArINT& arCfLoad, ArINT& arTLoad, ArINT& arTpgLoad,
		ArINT& arCSLoad);
	// ÁÖÇÏÁß + ÁÖÇÏÁß¿¡ ÇØ´çÇÏ´Â Æ¯¼öÇÏÁß + Ç³ÇÏÁß(Moving Load Case)
	void Set_CvlKrtaSer2_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arSLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arWprLoad, ArINT& arCfLoad, ArINT& arWLoad, ArINT& arWlLoad,
		ArINT& arCSLoad);
	// ÁÖÇÏÁß + ÁÖÇÏÁß¿¡ ÇØ´çÇÏ´Â Æ¯¼öÇÏÁß + ¿Âµµº¯È­ÀÇ ¿µÇâ + Ç³ÇÏÁß(Moving Load Case)
	void Set_CvlKrtaSer3_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arSLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arWprLoad, ArINT& arCfLoad, ArINT& arTLoad, ArINT& arTpgLoad,
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arCSLoad);
	// ÁÖÇÏÁß + ÁÖÇÏÁß¿¡ ÇØ´çÇÏ´Â Æ¯¼öÇÏÁß + Ç³ÇÏÁß + Á¦µ¿ÇÏÁß(Moving Load Case)
	void Set_CvlKrtaSer4_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arSLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arWprLoad, ArINT& arCfLoad, ArINT& arWLoad, ArINT& arWlLoad,
		ArINT& arBrkLoad, ArINT& arCSLoad);
	// ÁÖÇÏÁß + ÁÖÇÏÁß¿¡ ÇØ´çÇÏ´Â Æ¯¼öÇÏÁß + Ãæµ¹ÇÏÁß(Moving Load Case)
	void Set_CvlKrtaSer5_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arSLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arWprLoad, ArINT& arCfLoad, ArINT& arCoLoad, ArINT& arCSLoad);
	// Ç³ÇÏÁß¸¸ °í·ÁÇÒ ¶§
	void Set_CvlKrtaSer6(CString strCode, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arCSLoad);	
	// Á¦µ¿ÇÏÁß¸¸ °í·ÁÇÒ ¶§
	void Set_CvlKrtaSer7(CString strCode, ArINT& arBrkLoad);
	// È°ÇÏÁß ¹× Ãæ°Ý ÀÌ¿ÜÀÇ ÁÖÇÏÁß + ÁöÁøÀÇ ¿µÇâ
	void Set_CvlKrtaSer8_1(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arEpLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, 
		ArINT& arELoad, ArINT& arCSLoad);
	// È°ÇÏÁß ¹× Ãæ°Ý ÀÌ¿ÜÀÇ ÁÖÇÏÁß + ÁöÁøÀÇ ¿µÇâ(Response Spectrum Load Case)
	void Set_CvlKrtaSer8_2(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arEpLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, 
		ArUNT& arEspLoad, ArINT& arCSLoad);
	// °¡¼³½Ã ÇÏÁß
	void Set_CvlKrtaSer9(CString strCode, ArINT& arErLoad, ArINT& arCSLoad);    
	// ½Ã°ø´Ü°èÇØ¼®¿¡ Æ÷ÇÔµÈ Tendon PrestressÇÏÁßÀÇ °í·Á
	void Set_CvlKrtaSer_CSPS(CString strCode, ArINT& arCSLoad);
	// ÁÖÇÏÁß + ÁÖÇÏÁß¿¡ ÇØ´çÇÏ´Â Æ¯¼öÇÏÁß + ½Ãµ¿ÇÏÁß ¶Ç´Â Á¦µ¿ÇÏÁß
	void Set_CvlKsceRail3(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arSLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arWprLoad, ArINT& arCfLoad, ArINT& arBrkLoad,
		ArINT& arCSLoad);
	// Ç³ÇÏÁß + ½Ãµ¿ÇÏÁß ¶Ç´Â Á¦µ¿ÇÏÁß
	void Set_CvlKsceRail8(CString strCode, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arBrkLoad, ArINT& arCSLoad);	
	// ÁÖÇÏÁß + Ãæµ¹ÇÏÁß
	void Set_CvlKsceRail10(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arEpLoad, ArINT& arWpLoad,
		ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arBLoad, ArINT& arCoLoad, ArINT& arCsLoad);

	//////////////////////////////////////////////////////////////
	// CIVIL Combinations for JTJ021-89, CJJ77-98  ///////////////
	// To Strength Combinations.
	void Set_CvlChStrType1M(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arCSLoad);
	void Set_CvlChStrType4M(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arCvLoad, ArINT& arCSLoad);
	void Set_CvlChStrType2M(CString strCode, ArINT& arDLoad, ArINT& arEpLoad, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad, ArINT& arTLoad,
		ArINT& arTpgLoad, ArINT& arBrkLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arWprLoad,
		ArINT& arIpLoad, ArINT& arFrLoad, ArINT& arShLoad, ArINT& arCrLoad, ArINT& arStlLoad, ArUNT& arSmLoad,
		ArINT& arBLoad, ArINT& arCSLoad);  
	void Set_CvlChStrType3Lc(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArINT& arLcLoad,
		ArINT& arCfLoad, ArINT& arLscLoad, ArINT& arCSLoad);
	void Set_CvlChStrType3M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArUNT& arMLoad,
		ArINT& arCfLoad, ArINT& arLscLoad, ArINT& arCSLoad);
	// To Serviceability Combinations.
	void Set_CvlChService1M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArINT& arShLoad,
		ArINT& arCrLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arBLoad, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad,  ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arCSLoad, int nActive);
	void Set_CvlChService2M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArINT& arShLoad,
		ArINT& arCrLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arBLoad, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad,  ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad, ArINT& arTLoad,
		ArINT& arTpgLoad, ArINT& arBrkLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arSfLoad, ArINT& arWprLoad,
		ArINT& arIpLoad, ArINT& arFrLoad,  ArINT& arCSLoad, int nActive);  
	void Set_CvlChService3Lc(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArINT& arLcLoad, 
		ArINT& arCfLoad, ArINT& arLscLoad, ArINT& arCSLoad, int nActive);
	void Set_CvlChService3M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArUNT& arMLoad, 
		ArINT& arCfLoad, ArINT& arLscLoad, ArINT& arCSLoad, int nActive);
	void Set_CvlChService4M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArINT& arShLoad,
		ArINT& arCrLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arBLoad, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad,  ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arCvLoad,
		ArINT& arCSLoad, int nActive);  
	void Set_CvlChStrSer6E(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArINT& arELoad,
		ArINT& arCSLoad, int nActive);
	void Set_CvlChStrSer6S(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEpLoad, ArUNT& arEspLoad,
		ArINT& arCSLoad, int nActive);
	void CLoadCombCtrl::Set_CvlCh_CSPS(CString strCode, ArINT& arCSLoad, int nActive, int nStr);

	////////////////////////////////////////////////////
	// CIVIL Combinations for AASHTO-Standard2K  ///////////////
	// To Strength Combinations.
	void Set_Chk_AASHTO16(BOOL bChkAASHTO16){m_bAASHTO_LRFD16 = bChkAASHTO16;}
    void Set_Chk_AASHTO18(BOOL bChkAASHTO18){m_bAASHTO_LRFD18 = bChkAASHTO18;}
	void Set_Chk_AASHTO20(BOOL bChkAASHTO20){m_bAASHTO_LRFD20 = bChkAASHTO20;}
	void Set_Chk_AASHTO24(BOOL bChkAASHTO24){m_bAASHTO_LRFD24 = bChkAASHTO24;}
	BOOL Get_Chk_AASHTO16(){return m_bAASHTO_LRFD16;}
    BOOL Get_Chk_AASHTO18(){return m_bAASHTO_LRFD18;}
	BOOL Get_Chk_AASHTO20(){return m_bAASHTO_LRFD20;}
	BOOL Get_Chk_AASHTO24(){return m_bAASHTO_LRFD24;}
	void Set_CvlAASHTOStr1_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arCSLoad);
	void Set_CvlAASHTOStr1A_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCrlLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr1B(CString strCode, ArINT& arDLoad, ArINT& arLpLoad, ArINT& arCrlLoad, ArINT& arIlpLoad, 
		ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad,  ArINT& arBLoad, ArINT& arSfLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr2(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arWLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr3_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arBkLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr4_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arRsLoad, ArINT& arShLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr5(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arWLoad, ArINT& arRsLoad, ArINT& arShLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr6_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arBkLoad, ArINT& arRsLoad, ArINT& arShLoad, ArINT& arTLoad,
		ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr7(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arELoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr7S(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArUNT& arEspLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr7_CNS91(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arELoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr7S_CNS91(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArUNT& arEspLoad_x, ArUNT& arEspLoad_y, ArUNT& arEspLoad_z, ArINT& arCSLoad);
	void Set_CvlAASHTOStr8_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr9(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arWLoad, ArINT& arIpLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOStr10_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCrlLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arCSLoad);
	// To Serviceability Combinations.
	void Set_CvlAASHTOSer1_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arCSLoad);
	void Set_CvlAASHTOSer1A_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer2(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arWLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer3_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arBkLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer4_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arRsLoad, ArINT& arShLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer5(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arWLoad, ArINT& arRsLoad, ArINT& arShLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer6_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arBkLoad, ArINT& arRsLoad, ArINT& arShLoad, ArINT& arTLoad,
		ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer7(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arELoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer7S(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArUNT& arEspLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer7_CNS91(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arELoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer7S_CNS91(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArUNT& arEspLoad_x, ArUNT& arEspLoad_y, ArUNT& arEspLoad_z, ArINT& arCSLoad);
	void Set_CvlAASHTOSer8(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arCrlLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer8_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer9(CString strCode, ArINT& arDLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arWLoad, ArINT& arIpLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer10(CString strCode, ArINT& arDLoad, ArINT& arLLoad, ArINT& arCrlLoad, ArINT& arIlLoad, 
		ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arCSLoad);
	void Set_CvlAASHTOSer10_M(CString strCode, ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad,
		ArINT& arCrlLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arCSLoad);

	//3 copy start
	///////////////////////////////////////////////////////
	// CIVIL Combinations for AASHTO-LRFD02 ///////////////
	// Strength and Service Combinations.
	void GetAashtoLrfdDFactor(CString& sDc1Fac, CString& sDc2Fac, CString& sDc3Fac, 
		CString& sDd1Fac, CString& sDd2Fac, CString& sDw1Fac, CString& sDw2Fac, CString& sDw3Fac, 
		CString& sEh1Fac, CString& sEh2Fac, CString& sEh3Fac, CString& sEv1Fac, CString& sEv2Fac, 
		CString& sEv3Fac, CString& sEs1Fac, CString& sEs2Fac, CString& sEs3Fac, CString& sEl1Fac,
		CString& sEl2Fac, 
		double& dDc1, double& dDc2, double& dDc3, double& dDd1, double& dDd2, 
		double& dDw1, double& dDw2, double& dDw3, double& dEh1, double& dEh2, double& dEh3, 
		double& dEv1, double& dEv2, double& dEv3, double& dEs1, double& dEs2, double& dEs3, 
		double& dEl1, double& dEl2, 
		int nStrType);   
	void Set_CvlAashtoLrfdStr(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCfLoad, 
		ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arBLoad,
		ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arFrLoad, ArINT& arTLoad, 
		ArINT& arCrLoad, ArINT& arShLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCSLoad, int nStrType);
	void Set_CvlAashtoLrfdStr_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad,
		ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arFrLoad,
		ArINT& arTLoad,  ArINT& arCrLoad, ArINT& arShLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCSLoad,
		int nStrType);
	void Set_CvlAashtoLrfdEtr_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad,
		ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arFrLoad, ArINT& arELoad, ArUNT& arEspLoad,
		ArINT& arIpLoad, ArINT& arCtLoad, ArINT& arCvLoad, int nEtrType);
	void Set_CvlAashtoLrfdSer_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad,
		ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arFrLoad, 
		ArINT& arTLoad,  ArINT& arCrLoad, ArINT& arShLoad, ArINT& arTpgLoad, ArINT& arStlLoad, ArUNT& arSmLoad,
		ArINT& arCSLoad, int nSerType);
	void Set_CvlAashtoLrfdFtg(ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCfLoad);
	void Set_CvlAashtoLrfdFtg_M(ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCfLoad);
	///////////////////////////////////////////////////////
	// CIVIL Combinations for AASHTO-LRFD08 ///////////////
	// Strength and Service Combinations.
	void GetAashtoLrfdD08Factor(CString& sDc1Fac, CString& sDc2Fac, CString& sDd1Fac, CString& sDd2Fac, CString& sDw1Fac, 
		CString& sDw2Fac, CString& sEh1Fac, CString& sEh2Fac, CString& sEv1Fac, CString& sEv2Fac, 
		CString& sEs1Fac, CString& sEs2Fac, CString& sEl1Fac, CString& sEl2Fac, CString& sPs1Fac, 
		CString& sPs2Fac, CString& sCr1Fac, CString& sCr2Fac, CString& sSh1Fac, CString& sSh2Fac, 
		double& dDc1, double& dDc2, double& dDd1, double& dDd2, double& dDw1, 
		double& dDw2, double& dEh1, double& dEh2, double& dEv1, double& dEv2, 
		double& dEs1, double& dEs2, double& dEl1, double& dEl2, double& dPs1, 
		double& dPs2, double& dCr1, double& dCr2, double& dSh1, double& dSh2, int nStrType);
	void Set_CvlAashtoLrfd08Str(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, ArINT& arEvLoad, 
		ArINT& arEsLoad, ArINT& arElLoad, ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arCfLoad, 
		ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad, 
		ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad,ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arFrLoad, ArINT& arTLoad,  ArINT& arCrLoad, ArINT& arShLoad, ArINT& arStlLoad, 
		ArUNT& arSmLoad, ArINT& arCSLoad, ArINT& arPsLoad, int nStrType);
	void Set_CvlAashtoLrfd08Str_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, ArINT& arEvLoad, 
		ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad,  ArINT& arLLoad,  ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arLsLoad, ArINT& arWpLoad, 
		ArINT& arFpLoad, ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad,ArINT& arWLoad, 
		ArINT& arWlLoad, ArINT& arFrLoad, ArINT& arTLoad,  ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arStlLoad,ArUNT& arSmLoad, ArINT& arCSLoad, ArINT& arPsLoad,int nStrType);
	void Set_CvlAashtoLrfd08Etr_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, ArINT& arEvLoad, 
		ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad,  ArINT& arLLoad,  ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arLsLoad, ArINT& arWpLoad, 
		ArINT& arFpLoad, ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad,ArINT& arFrLoad, 
		ArINT& arELoad,  ArUNT& arEspLoad,ArINT& arIpLoad, ArINT& arCtLoad, ArINT& arCvLoad, 
		ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, int nEtrType);
	void Set_CvlAashtoLrfd08Ser_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, ArINT& arEvLoad, 
		ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad,  ArINT& arLLoad,  ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arLsLoad, ArINT& arWpLoad, 
		ArINT& arFpLoad, ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad,ArINT& arWLoad, 
		ArINT& arWlLoad, ArINT& arFrLoad, ArINT& arTLoad,  ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arPsLoad, ArINT& arTpgLoad,ArINT& arStlLoad,ArUNT& arSmLoad, ArINT& arCSLoad, 
		int nSerType);
	void Set_CvlAashtoLrfd08Ftg(ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCfLoad);
	void Set_CvlAashtoLrfd08Ftg_M(ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCfLoad);
	///////////////////////////////////////////////////////
	// CIVIL Combinations for KSCE-LSD12 ///////////////
	// Strength and Service Combinations.
	void GetKoreaLrfd11Factor(CString& sDc1Fac, CString& sDc2Fac, CString& sDc3Fac, 
		CString& sDd1Fac, CString& sDd2Fac, CString& sDw1Fac, CString& sDw2Fac, CString& sDw3Fac, 
		CString& sEh1Fac, CString& sEh2Fac, CString& sEh3Fac, CString& sEv1Fac, CString& sEv2Fac, 
		CString& sEv3Fac, CString& sEs1Fac, CString& sEs2Fac, CString& sEs3Fac, CString& sEl1Fac,
		CString& sEl2Fac, CString& sPs1Fac, CString& sPs2Fac, CString& sCr1Fac, CString& sCr2Fac, 
		CString& sSh1Fac, CString& sSh2Fac, 
		double& dDc1, double& dDc2, double& dDc3, double& dDd1, double& dDd2, 
		double& dDw1, double& dDw2, double& dDw3, double& dEh1, double& dEh2, double& dEh3, 
		double& dEv1, double& dEv2, double& dEv3, double& dEs1, double& dEs2, double& dEs3, 
		double& dEl1, double& dEl2, double& dPs1, double& dPs2, double& dCr1, double& dCr2, 
		double& dSh1, double& dSh2,
		int nStrType);   
	void Set_CvlKoreaLrfd11Str(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCfLoad, 
		ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arBLoad, 
		ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arFrLoad, ArINT& arTLoad, 
		ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCSLoad, 
		int nStrType);
	void Set_CvlKoreaLrfd11Str_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad, 
		ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arFrLoad, 
		ArINT& arTLoad,  ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arCSLoad, int nStrType);
	void Set_CvlKoreaLrfd11Etr_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad, 
		ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arFrLoad, ArINT& arELoad, ArUNT& arEspLoad, 
		ArINT& arIpLoad, ArINT& arCtLoad, ArINT& arCvLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arCSLoad, int nEtrType);
	void Set_CvlKoreaLrfd11Ser_M(ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, 
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arElLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, 
		ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arFpLoad, 
		ArINT& arBLoad,  ArINT& arSfLoad, ArINT& arWprLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arFrLoad, 
		ArINT& arTLoad,  ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arTpgLoad, ArINT& arStlLoad, 
		ArUNT& arSmLoad, ArINT& arCSLoad, int nSerType);
	void Set_CvlKoreaLrfd11Ftg(ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCfLoad);
	void Set_CvlKoreaLrfd11Ftg_M(ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCrLoad, ArINT& arShLoad,
		ArUNT& arMLoad,  ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arCSLoad);
	void Set_CvlKoreaLrfd11Continue(ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad);
	void Set_CvlKoreaLrfd11GravLoad(ArINT& arDcLoad, ArINT& arDwLoad, ArUNT& arMLoad,  ArINT& arLLoad, ArINT& arSLoad);
	///////////////////////////////////////////////////////
	// CIVIL Combinations for KSCE-LSD15 ///////////////
	// Seismic Evaluation Auto Load Combinations.
	void GetCvlKoreaLSD15SeisEvalFactor(CString& sDc1Fac, CString& sDc2Fac,
		CString& sDw1Fac, CString& sDw2Fac,
		double& dDc1, double& dDc2, 
		double& dDw1, double& dDw2);   
	void Set_CvlKoreaLSD15SeisEval(ArINT& arDcLoad, ArINT& arDwLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arSLoad,
		ArUNT& arEspLoad);
	void Set_CvlKoreaLSD15SeisEval_100_30(int& LoadCaseNo, ArUNT& arEspLoad, CString& csPrefix, T_LCOM_D& rData);

	/////////////////////////////////////////////
	// CSA-S6S1-10 & CSA-S6-14(PJS)
	/////////////////////////////////////////////
	void GetCSAS6S110Factor(
		CString& sD1Fac,   CString& sD2Fac,   CString& sDw1Fac,  CString& sDw2Fac,  CString& sEp11Fac, 
		CString& sEp12Fac, CString& sEp21Fac, CString& sEp22Fac, CString& sEp31Fac, CString& sEp32Fac, 
		CString& sEp41Fac, CString& sEp42Fac, CString& sWp1Fac,  CString& sWp2Fac,  CString& sPs1Fac,  
		CString& sPs2Fac, 
		double& dD1,   double& dD2,   double& dDw1,  double& dDw2,  double& dEp11, 
		double& dEp12, double& dEp21, double& dEp22, double& dEp31, double& dEp32, 
		double& dEp41, double& dEp42, double& dWp1,  double& dWp2,	 double& dPs1,  
		double& dPs2);
	void Set_CvlCSAS6S110ULS_No1(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,     ArINT& arCvLoad);
	void Set_CvlCSAS6S110ULS_No2(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad);
	void Set_CvlCSAS6S110ULS_No3(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad);
	void Set_CvlCSAS6S110ULS_No4(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad,    BOOL bCode);
	//bCode == TRUE : CSA-S6-14, bCode == FALSE : CSA-S1S6-10,
	void Set_CvlCSAS6S110ULS_No5(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad);
	void Set_CvlCSAS6S110ULS_No6(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad);
	void Set_CvlCSAS6S110ULS_No7(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad);
	void Set_CvlCSAS6S110ULS_No8(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad);
	void Set_CvlCSAS6S110ULS_No9(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad, 
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,   
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arWLoad,     ArINT& arWlLoad,     ArINT& arStlLoad,    ArINT& arELoad,   ArUNT& arEspLoad,   
		ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad);
	void Set_CvlCSAS6S110SLS(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad,  
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,	 
		ArINT& arCrLoad,    ArINT& arShLoad,     ArINT& arTLoad,      ArINT& arTpgLoad, ArINT& arCSLoad,  
		ArINT& arStlLoad,   ArUNT& arSmLoad);
	void Set_CvlCSAS6S110FLS(
		ArINT& arCSADLoad,  ArINT& arCSADcLoad,  ArINT& arCSADwLoad,  ArINT& arWpLoad,  ArINT& arPsLoad,  
		ArINT& arLsLoad,    ArINT& arLLoad,      ArINT& arLpLoad,     ArINT& arIlLoad,  ArINT& arIlpLoad, 
		ArINT& arCfLoad,    ArINT& arBrkLoad,    ArINT& arBkLoad,     ArINT& arCrlLoad, ArUNT& arMLoad,
		ArINT& arCSLoad,    ArINT& arStlLoad,   ArUNT& arSmLoad);
	/////////////////////////////////////////////
	// SNiP 2.05.03-84*, SP 35.13330.2011
	/////////////////////////////////////////////
	void GetSP_35_13330_11Factor( 
		CString& sD1Fac,  CString& sD2Fac,   CString& sLIP1Fac,   CString& sLIP2Fac,  CString& sPL1Fac,   CString& sPL2Fac,         
		CString& sBL1Fac, CString& sBL2Fac,  CString& sPs1Fac,    CString& sPs2Fac,   CString& sEp1Fac,   CString& sEp2Fac,   
		CString& sWp1Fac, CString& sWp2Fac,  CString& sCrSh1Fac,  CString& sCrSh2Fac, CString& sStl1Fac,  CString& sStl2Fac, 
		double& dD1,   double& dD2,   double& dLIP1,   double& dLIP2,   double& dPL1,   double& dPL2,   
		double& dBL1,  double& dBL2,  double& dPs1,    double& dPs2,    double& dEp1,   double& dEp2,  
		double& dWp1,  double& dWp2,  double& dCrSh1,  double& dCrSh2, 	double& dStl1,  double& dStl2,
		BOOL b1stGrp);

	/*A ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad, ArINT& arBLLoad, ArINT& arPsLoad,   
	ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arWpLoad,   
	ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad - 1~6 */
	/*B ArINT& arLLoad, ArUNT& arMLoad - 7 */		
	/*C ArINT& arLsLoad - 8 */		
	/*D ArINT& arCfLoad - 9 */	
	/*E ArINT& arCtLoad - 10 */	
	/*F ArINT& arBrkLoad,  ArINT& arBkLoad - 11 */	
	/*G ArINT& arWLoad,   ArINT& arWlLoad - 12 */	
	/*H ArINT& arIpLoad - 13 */	
	/*I ArINT& arCvLoad - 14 */	
	/*J ArINT& arTLoad, ArINT& arTpgLoad - 15 */	
	/*K ArINT& arShvLoad - 16 */	
	/*L ArINT& arErLoad - 17 */	
	/*M ArINT& arELoad - 18 */	
	/*N ArINT& arFrLoad - 19 */	
	/* ArINT& arCSLoad */

	// 	void Set_CvlSP_35_13330_11(
	// 			ArINT& arDLoad,   ArINT& arPsLoad, ArINT& arEpLoad,  ArINT& arEhLoad,  ArINT& arEvLoad,  
	// 			ArINT& arEsLoad,  ArINT& arWpLoad, ArINT& arCrLoad,  ArINT& arShLoad,  ArINT& arStlLoad,
	// 			ArINT& arLLoad,   ArUNT& arMLoad,  ArINT& arLsLoad,  ArINT& arCfLoad,  ArINT& arCtLoad, 
	// 			ArINT& arBrkLoad, ArINT& arBkLoad, ArINT& arWLoad,   ArINT& arWlLoad,  ArINT& arIpLoad, 
	// 			ArINT& arCvLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arShvLoad, ArINT& arErLoad, 
	// 			ArINT& arELoad,   ArINT& arFrLoad, ArINT& arCSLoad,
	// 			BOOL b1stGrp);

	void Set_CvlSP_35_13330_11_1(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_2(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_3(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arBrkLoad, ArINT& arBkLoad, ArINT& arWLoad,  
		ArINT& arWlLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_4(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arWLoad,   ArINT& arWlLoad, ArINT& arIpLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arFrLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_5(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,   
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arIpLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, 
		ArINT& arFrLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_6(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arCvLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_7(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arBrkLoad, ArINT& arBkLoad,	 ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_8(CString strCode,		
		ArINT& arDLoad,   ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad,  ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad,  ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad,  ArINT& arWLoad,   ArINT& arWlLoad,  ArINT& arIpLoad, ArINT& arTLoad,  
		ArINT& arTpgLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_9(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arBrkLoad, ArINT& arBkLoad, ArINT& arWLoad,  
		ArINT& arWlLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_10(CString strCode,	
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arWLoad,   ArINT& arWlLoad, ArINT& arIpLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arFrLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_11(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arCvLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_12(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,   
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arIpLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, 
		ArINT& arFrLoad, ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_13(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arCvLoad,  ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_14(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arBrkLoad, ArINT& arBkLoad, ArINT& arWLoad,  
		ArINT& arWlLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_15(CString strCode, 
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arWLoad,   ArINT& arWlLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_16(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arBrkLoad, ArINT& arBkLoad,  ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_17(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arWLoad,   ArINT& arWlLoad, ArINT& arIpLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arFrLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_18(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arIpLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arErLoad, ArINT& arFrLoad, 
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_19(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arShvLoad, ArINT& arErLoad, ArINT& arFrLoad, 
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_20(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arIpLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_21(CString strCode,	
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arWLoad,   ArINT& arWlLoad, ArINT& arIpLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arFrLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_22(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,   
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arIpLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, 
		ArINT& arFrLoad, ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_23(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arIpLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_24(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arCvLoad, ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_25(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arCvLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_26(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arCvLoad,  ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_27(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, 
		ArINT& arCSLoad, 
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_28(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arBrkLoad, ArINT& arBkLoad, ArINT& arWLoad,  
		ArINT& arWlLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_29(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arWLoad,   ArINT& arWlLoad, ArINT& arIpLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arFrLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_30(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,   
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arIpLoad,	 ArINT& arTLoad,  ArINT& arTpgLoad, 
		ArINT& arFrLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_31(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arIpLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arErLoad, ArINT& arFrLoad, 
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_32(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arShvLoad, ArINT& arErLoad, ArINT& arFrLoad, 
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_33(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad,  ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad,  ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arShvLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_34(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arShvLoad, ArINT& arErLoad, ArINT& arFrLoad,	
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_35(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arErLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_36(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arIpLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arErLoad, ArINT& arFrLoad, 
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_37(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arShvLoad, ArINT& arErLoad, ArINT& arFrLoad,	
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_38(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arBrkLoad, ArINT& arBkLoad, ArINT& arELoad,   
		ArINT& arFrLoad, ArINT& arCSLoad,  ArUNT& arESpLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_39(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arFrLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_40(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arWLoad,   ArINT& arWlLoad, ArINT& arIpLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arFrLoad,  ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_41(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,   
		ArINT& arLsLoad, ArINT& arCtLoad,  ArINT& arIpLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, 
		ArINT& arFrLoad, ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_42(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arIpLoad, ArINT& arTLoad,   ArINT& arTpgLoad, ArINT& arErLoad, ArINT& arFrLoad, 
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_43(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arWLoad,  ArINT& arWlLoad, 
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arShvLoad, ArINT& arErLoad, ArINT& arFrLoad, 
		ArINT& arCSLoad,
		BOOL b1stGrp);
	void Set_CvlSP_35_13330_11_F1(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCfLoad,  ArINT& arCSLoad,
		int nCase);
	void Set_CvlSP_35_13330_11_F2(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arLLoad,  ArUNT& arMLoad,  
		ArINT& arLsLoad, ArINT& arCSLoad,
		int nCase);
	void Set_CvlSP_35_13330_11_F3(CString strCode,
		ArINT& arDLoad,  ArINT& arLIPLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arPsLoad,   
		ArINT& arEpLoad, ArINT& arEhLoad,  ArINT& arEvLoad,  ArINT& arEsLoad, ArINT& arWpLoad,   
		ArINT& arCrLoad, ArINT& arShLoad,  ArINT& arStlLoad, ArINT& arCSLoad);

	CString Get_CvlSP_35_13330_11_Title(int nGroup);

	//////////////PNS-10030//////////////////////////

	void Get_PNS10030_Factor(CString& sD1Fac,  CString& sD2Fac,   CString& sDc1Fac,   CString& sDc2Fac, CString& sDw1Fac,   CString& sDw2Fac,  
		CString& sPs1Fac,  CString& sPs2Fac,  CString& sWp1Fac,   CString& sWp2Fac, 
		double& dD1,   double& dD2,   double& dDc1,   double& dDc2,   double& dDw1,   double& dDw2,   double& dPs1,   
		double& dPs2,  double& dWp1,  double& dWp2,   BOOL b1stGrp);

	BOOL IsUsedFunc(unsigned int LcomK, const _LCOM_IN& UnitResD, int nDepth);
	BOOL Swap(_LCOM_IN& LoadInD, int i, int j);
	void SetCrticalFactor(_LCOM_IN& arrRestUnit, const _LCOM_FD& FactorInD);
	BOOL MakeLoadCombination4Var(_LCOM_IN& arrResUnit, int nDepth, _LCOM_IN& LoadInD, const _LCOM_FD& FactorInD, _LCOM_RES& rData);

	CString Get_PNS10030_Title(int nState);



	void SetCvl_PNS10030_1_P(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad, ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArUNT& arMLoad,    ArINT& arLLoad,    ArINT& arCfLoad,   ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_2_P(CString strCode,
		ArINT& arDLoad,     ArINT& arDcLoad,  ArINT& arDwLoad,   ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad, 
		ArINT& arStlLoad, ArUNT& arSmLoad,   ArUNT& arMLoad,    ArINT& arLLoad,    ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_3_P(CString strCode,
		ArINT& arDLoad,   ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arWLoad,  ArINT& arWlLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_4_P(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad,  ArINT& arDwLoad,   ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad,  ArINT& arIpLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_5_P(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arIpLoad, ArINT& arErLoad,  ArINT& arCSLoad, 
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_6_P(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arErLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_7_P(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arIpLoad,   ArINT& arCSLoad, 
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_8_P(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arIpLoad,  ArINT& arCSLoad, 
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_9_P(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_10_P(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_11_P(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,   ArINT& arStlLoad, ArUNT& arSmLoad,   ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arCfLoad, ArINT& arWLoad,  ArINT& arWlLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad, ArINT& arIpLoad,   ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_12_P(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,  ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arErLoad,   ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_13_P(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);


	void SetCvl_PNS10030_1_PD(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad, ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arCfLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_2_PD(CString strCode,
		ArINT& arDLoad,   ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,   ArINT& arWpLoad, ArINT& arEpLoad, 
		ArINT& arStlLoad, ArUNT& arSmLoad,   ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arCrlLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_3_PD(CString strCode,
		ArINT& arDLoad,   ArINT& arDcLoad,  ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arCfLoad,  ArINT& arBrkLoad, ArINT& arWLoad,  ArINT& arWlLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_4_PD(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad,  ArINT& arDwLoad,   ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad,  ArINT& arIpLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_5_PD(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arIpLoad, ArINT& arErLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_6_PD(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arErLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_7_PD(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arIpLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_8_PD(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arIpLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_9_PD(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arCSLoad, 
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_10_PD(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_11_PD(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,   ArINT& arStlLoad, ArUNT& arSmLoad,   ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arCfLoad, ArINT& arWLoad,  ArINT& arWlLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad, ArINT& arIpLoad,   ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_12_PD(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arErLoad, ArINT& arCSLoad, 
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_13_PD(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);

	void SetCvl_PNS10030_1_PW(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad, ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arCfLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_2_PW(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad, 
		ArINT& arStlLoad, ArUNT& arSmLoad,   ArUNT& arMLoad,  ArINT& arLLoad,  ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_3_PW(CString strCode,
		ArINT& arDLoad,   ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arWLoad,  ArINT& arWlLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_4_PW(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad,  ArINT& arDwLoad,   ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad,  ArINT& arIpLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_5_PW(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arIpLoad, ArINT& arErLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_6_PW(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arErLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_7_PW(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arIpLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_8_PW(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arWLoad, ArINT& arWlLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arIpLoad,  ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_9_PW(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,   ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_10_PW(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_11_PW(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,   ArINT& arStlLoad, ArUNT& arSmLoad,   ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arCfLoad, ArINT& arWLoad,  ArINT& arWlLoad,  ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arCrlLoad, ArINT& arIpLoad,   ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_12_PW(CString strCode,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad, ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arErLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);
	void SetCvl_PNS10030_13_PW(CString strCode,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arPsLoad,  ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arCrlLoad, ArINT& arCSLoad,
		ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad,
		BOOL bState);



	void SetCvl_PNS10030_SLS1(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArUNT& arMLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS2(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arCfLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS3(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arTLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS4(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arTpgLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS5(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arWLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS6(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arWlLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS7(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arBrkLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS8(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad,  ArINT& arIpLoad,
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);
	void SetCvl_PNS10030_SLS9(CString strCode,
		ArINT& arDLoad,    ArINT& arDcLoad,   ArINT& arDwLoad,   ArINT& arPsLoad,   ArINT& arWpLoad,  ArINT& arEpLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, 
		ArINT& arCSLoad,   ArINT& arEANNLoad, ArINT& arEANCLoad, ArINT& arEAMNLoad, ArINT& arEAMCLoad, 
		ArINT& arEPNNLoad, ArINT& arEPNCLoad, ArINT& arEPMNLoad, ArINT& arEPMCLoad, BOOL bState);


	// double Get_PNS10030_Critical(ArINT& arStlLoad,  ArUNT& arMLoad, ArINT& arLLoad,  ArINT& arCfLoad, ArINT& arTLoad,   
	//    ArINT& arWLoad, ArINT& arBrkLoad,  ArINT& arIpLoad,  ArINT& arErLoad);


	/////////////////////////////////////////////
	// IRC:6-2000, in India (KJH20030916)
	/////////////////////////////////////////////
	void Set_CvlIrc6_2000_No1_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad,
		ArINT& arSLoad, ArINT& arIlLoad, ArINT& arSfLoad, ArINT& arBkLoad, ArINT& arBrkLoad, ArINT& arFrLoad,
		ArINT& arCfLoad, ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arLsLoad,
		ArINT& arWpLoad, ArINT& arGeLoad, ArINT& arCSLoad, int nAct);
	void Set_CvlIrc6_2000_No2A_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad, ArINT& arSLoad, 
		ArINT& arIlLoad, ArINT& arSfLoad, ArINT& arBkLoad, ArINT& arBrkLoad, ArINT& arFrLoad, ArINT& arCfLoad,
		ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arLsLoad, ArINT& arWpLoad, 
		ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arRsLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad,
		ArINT& arShLoad, ArINT& arGeLoad, ArINT& arCSLoad, int nAct);
	void Set_CvlIrc6_2000_No2B_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad,
		ArINT& arIlLoad, ArINT& arSfLoad, ArINT& arBkLoad, ArINT& arBrkLoad, ArINT& arFrLoad, ArINT& arCfLoad,
		ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arLsLoad, ArINT& arWpLoad, 
		ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arRsLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad,
		ArINT& arShLoad, ArINT& arGeLoad, ArINT& arCSLoad, int nAct);
	void Set_CvlIrc6_2000_No3A_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad, ArINT& arSLoad, 
		ArINT& arIlLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arBkLoad, ArINT& arBrkLoad,
		ArINT& arFrLoad, ArINT& arCfLoad, ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad,
		ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arRsLoad, ArINT& arStlLoad,
		ArUNT& arSmLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arWprLoad, ArINT& arGeLoad, ArINT& arCSLoad, int nAct);
	void Set_CvlIrc6_2000_No3B_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad,
		ArINT& arIlLoad, ArINT& arBkLoad, ArINT& arBrkLoad, ArINT& arFrLoad, ArINT& arCfLoad, ArINT& arBLoad,
		ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arTLoad,
		ArINT& arTpgLoad, ArINT& arRsLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad, ArINT& arShLoad,
		ArINT& arWprLoad, ArINT& arGeLoad, ArINT& arCSLoad, int nAct);
	void Set_CvlIrc6_2000_No4_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad, ArINT& arSLoad, 
		ArINT& arIlLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arBkLoad, ArINT& arBrkLoad,
		ArINT& arFrLoad, ArINT& arCfLoad, ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad,
		ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad, ArINT& arShLoad,
		ArINT& arWprLoad, ArINT& arGeLoad, ArINT& arCSLoad, int nAct);
	void Set_CvlIrc6_2000_No5(ArINT& arDLoad, ArINT& arCtLoad, ArINT& arCSLoad, int nAct);
	void Set_CvlIrc6_2000_No6_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad,
		ArINT& arIlLoad, ArINT& arSfLoad, ArINT& arBkLoad, ArINT& arBrkLoad, ArINT& arFrLoad, ArINT& arCfLoad,
		ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arLsLoad, ArINT& arWpLoad, 
		ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arRsLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad,
		ArINT& arShLoad, ArINT& arELoad, ArUNT& arEspLoad, ArINT& arWprLoad, ArINT& arGeLoad, ArINT& arCSLoad, 
		int nEtype, int nAct);
	void Set_CvlIrc6_2000_No7_M(ArINT& arDLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arCrlLoad, ArINT& arSLoad, 
		ArINT& arIlLoad, ArINT& arCvLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arBkLoad,
		ArINT& arBrkLoad, ArINT& arFrLoad, ArINT& arCfLoad, ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad,
		ArINT& arEsLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arRsLoad,
		ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arGeLoad, ArINT& arCSLoad, 
		int nAct);
	void Set_CvlIrc6_2000_No8(ArINT& arDLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arFrLoad,
		ArINT& arBLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arLsLoad, ArINT& arWpLoad,
		ArINT& arErLoad, ArINT& arElLoad, ArINT& arGeLoad, ArINT& arCSLoad);
	void Set_CvlIrc6_2000_No9(ArINT& arDLoad, ArINT& arSfLoad, ArINT& arFrLoad, ArINT& arBLoad, ArINT& arEhLoad,
		ArINT& arEvLoad, ArINT& arEsLoad, ArINT& arLsLoad, ArINT& arWpLoad, ArINT& arErLoad, ArINT& arElLoad,
		ArINT& arELoad, ArUNT& arEspLoad, ArINT& arGeLoad, ArINT& arCSLoad, int nEtype);
	void GetLLDesc(int nML, int nL, CString strM, int nIL, double dFactor, CString& csDesc, CString& csPrefix);
	void GetELDesc(int nEH, int nEV, int nES, int nLS, int nWP, double dFactor, CString& csDesc, CString& csPrefix);

	/////////////////////////////////////////////
	// IRC:6 LSD, in India (PinakinIRC6LSD)
	/////////////////////////////////////////////
	BOOL Chk_CvlIrc6_variableLoads(int nCfl,int nBrk,int nWL,int nLL );
	void Set_CvlIrc6_Basic(ArINT& arDLoad,ArINT& arDcLoad,ArINT&  arDwLoad,ArINT& arPLLoad,ArINT& arEpLoad,
		ArINT& arEhLoad,ArINT& arLsLoad,ArINT& arLLoad,ArINT& arIlLoad,ArINT& arCfLoad,
		ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arPsLoad,ArINT& arBLoad,ArINT& arSfLoad,
		ArINT& arWpLoad,ArINT& arWLoad,ArINT& arCoLoad,ArINT& arCtLoad,ArINT& arCvLoad,
		ArINT& arELoad,ArINT& arCSLoad,	  ArINT& arErLoad,ArINT& arSLoad,ArUNT& arMLoad);

	void Set_CvlIrc6_Acci(ArINT& arDLoad,ArINT& arDcLoad,ArINT&  arDwLoad,ArINT& arPLLoad,ArINT& arEpLoad,
		ArINT& arEhLoad,ArINT& arLsLoad,ArINT& arLLoad,ArINT& arIlLoad,ArINT& arCfLoad,
		ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arPsLoad,ArINT& arBLoad,ArINT& arSfLoad,
		ArINT& arWpLoad,ArINT& arWLoad,ArINT& arCoLoad,ArINT& arCtLoad,ArINT& arCvLoad,
		ArINT& arELoad,ArINT& arCSLoad,ArINT& arErLoad,ArINT& arSLoad,ArUNT& arMLoad);

	void Set_CvlIrc6_Seis(ArINT& arDLoad,ArINT& arDcLoad,ArINT&  arDwLoad,ArINT& arPLLoad,ArINT& arEpLoad,
		ArINT& arEhLoad,ArINT& arLsLoad,ArINT& arLLoad,ArINT& arIlLoad,ArINT& arCfLoad,
		ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arPsLoad,ArINT& arBLoad,ArINT& arSfLoad,
		ArINT& arWpLoad,ArINT& arWLoad,ArINT& arCoLoad,ArINT& arCtLoad,ArINT& arCvLoad,
		ArINT& arELoad,ArINT& arCSLoad, ArINT& arErLoad,ArINT& arSLoad,ArUNT& arMLoad, 
		ArUNT& arEspLoad,int nEtype);

	void Set_CvlIrc6_Rare(ArINT& arDLoad,ArINT& arDcLoad,ArINT&  arDwLoad,ArINT& arPLLoad,ArINT& arEpLoad,
		ArINT& arEhLoad,ArINT& arLsLoad,ArINT& arLLoad,ArINT& arIlLoad,ArINT& arCfLoad,
		ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arPsLoad,ArINT& arBLoad,ArINT& arSfLoad,
		ArINT& arWpLoad,ArINT& arWLoad,ArINT& arCoLoad,ArINT& arCtLoad,ArINT& arCvLoad,
		ArINT& arELoad,ArINT& arCSLoad, ArINT& arErLoad,ArINT& arSLoad,ArUNT& arMLoad,
		ArINT& arShLoad,ArINT& arStlLoad,ArINT& arTLoad,ArINT& arCrLoad,ArUNT& arSmLoad, 
		ArINT& arTpgLoad);

	void Set_CvlIrc6_Freq(ArINT& arDLoad,ArINT& arDcLoad,ArINT&  arDwLoad,ArINT& arPLLoad,ArINT& arEpLoad,
		ArINT& arEhLoad,ArINT& arLsLoad,ArINT& arLLoad,ArINT& arIlLoad,ArINT& arCfLoad,
		ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arPsLoad,ArINT& arBLoad,ArINT& arSfLoad,
		ArINT& arWpLoad,ArINT& arWLoad,ArINT& arCoLoad,ArINT& arCtLoad,ArINT& arCvLoad,
		ArINT& arELoad,ArINT& arCSLoad,ArINT& arErLoad,ArINT& arSLoad,ArUNT& arMLoad,
		ArINT& arShLoad,ArINT& arStlLoad,ArINT& arTLoad,ArINT& arCrLoad,ArUNT& arSmLoad,
		ArINT& arTpgLoad);


	void Set_CvlIrc6_QP(ArINT& arDLoad,ArINT& arDcLoad,ArINT&  arDwLoad,ArINT& arPLLoad,ArINT& arEpLoad,
		ArINT& arEhLoad,ArINT& arLsLoad,ArINT& arLLoad,ArINT& arIlLoad,ArINT& arCfLoad,
		ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arPsLoad,ArINT& arBLoad,ArINT& arSfLoad,
		ArINT& arWpLoad,ArINT& arWLoad,ArINT& arCoLoad,ArINT& arCtLoad,ArINT& arCvLoad,
		ArINT& arELoad,ArINT& arCSLoad,ArINT& arErLoad,ArINT& arSLoad,ArUNT& arMLoad,
		ArINT& arShLoad,ArINT& arStlLoad,ArINT& arTLoad,ArINT& arCrLoad,ArUNT& arSmLoad,
		ArINT& arTpgLoad);
	////IRC6 New LC
	//Ultimate Limit State
	BOOL SetCvlIRC6_Traffic(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad, ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2);//Traffic Leading + Wind
	BOOL SetCvlIRC6_Traffic_No_Wind(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2 ,ArINT& arTLoad);//Traffic Leading + No Wind
	BOOL SetCvlIRC6_Traffic_NO_WIND_NO_THERMAL(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2);//Traffic Leading + Wind
	BOOL SetCvlIRC6_Wind(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2);// Wind Leading + Live
	BOOL SetCvlIRC6_Wind_No_Live(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2);// Wind Leading + No Live
	BOOL SetCvlIRC6_Therm1_With_Live(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,ArUNT &arMLoad,ArINT &arThermal,ArINT& arTLoad);//Thermal Leading + Adding Variable + LL
	BOOL SetCvlIRC6_Therm1_Without_Live(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,ArUNT &arMLoad,ArINT &arThermal,ArINT& arTLoad);//Thermal Leading + Adding Variable + No LL
	BOOL SetCvlIRC6_Snow(ArINT &arDLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arSLoad, ArINT &arCSLoad);
	BOOL SetCvlIRC6_Erection(ArINT &arDLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad, ArINT &arThermal, ArINT &arErLoad, ArINT &arCSLoad);
	//ULS - Seismic
	BOOL SetCvlIRC6_Seis_No_Wind(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT& arRsLoad, ArUNT& arEspLoad_X,ArUNT& arEspLoad_Y,ArUNT& arEspLoad_Z,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2);//Traffic Leading + No Wind
	BOOL SetCvlIRC6_Seis_Incl_Therm(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT& arRsLoad, ArUNT& arEspLoad_X,ArUNT& arEspLoad_Y,ArUNT& arEspLoad_Z,ArUNT &arMLoad,ArINT &arThermal,ArINT& arTLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2);//Traffic Leading + No Wind

	//ULS - Accidental
	BOOL SetCvlIRC6_ACCI_Traffic_No_Wind(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2);//Traffic Leading + No Wind
	BOOL SetCvlIRC6_ACCI_Therm1_Without_Live(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT& arCoLoad,    ArINT& arCtLoad,  	  ArINT& arCvLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,ArUNT &arMLoad,ArINT &arThermal,ArINT& arTLoad);//Thermal Leading + Adding Variable + No LL

    //ULS - Seismic - Static Seismic
    BOOL SetCvlIRC6_Seis_No_Wind_SS(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad, ArINT &arEvLoad,
        ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad, ArINT &arErLoad,
        ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
        ArINT &arCSLoad, ArINT& arRsLoad, ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, ArUNT &arMLoad, ArINT& arWprLoad, ArINT& arBLoad, ArINT& arWpLoad,
        ArINT& arELoad_x, ArINT& arELoad_y, ArINT& arELoad_z, ArUNT &arPsi1FactorMLoad1, ArUNT &arPsi1FactorMLoad2);//Traffic Leading + No Wind

    BOOL SetCvlIRC6_Seis_Incl_Therm_SS(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad, ArINT &arEvLoad,
        ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad, ArINT &arErLoad,
        ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
        ArINT &arCSLoad, ArINT& arRsLoad, ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, ArUNT &arMLoad, ArINT &arThermal, ArINT& arTLoad, ArINT& arWprLoad, ArINT& arBLoad, ArINT& arWpLoad,
        ArINT& arELoad_x, ArINT& arELoad_y, ArINT& arELoad_z, ArUNT &arPsi1FactorMLoad1, ArUNT &arPsi1FactorMLoad2);//Traffic Leading + No Wind

	//Serviceability Limit State
	BOOL SetCvlIRC6_SERV_Traffic(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR,int iIRC112Case);//Traffic Leading + Wind
	BOOL SetCvlIRC6_SERV_Traffic_No_Wind(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR,ArINT& arTLoad, int iIRC112Case);//Traffic Leading + No Wind
	BOOL SetCvlIRC6_SERV_Traffic_NO_WIND_NO_THERMAL(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR, int iIRC112Case);//Traffic Leading + Wind


	BOOL SetCvlIRC6_Wind_serv1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR, int iIRC112Case);
	BOOL SetCvlIRC6_Wind_serv2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR, int iIRC112Case);

	BOOL SetCvlIRC6_Therm_serv1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR,ArINT& arTLoad, int iIRC112Case);
	BOOL SetCvlIRC6_Therm_serv2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR,ArINT& arTLoad, int iIRC112Case);

	BOOL SetCvlIRC6_Snow_serv1(ArINT &arDLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arSLoad, ArINT &arCSLoad);
	BOOL SetCvlIRC6_Snow_serv2(ArINT &arDLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
		ArINT &arThermal, ArINT &arSLoad, ArINT &arCSLoad);

	BOOL SetCvlIRC6_Wind1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArUNT &arMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArUNT &arIgnorePsi1FactorMLoad, BOOL bETC);
	BOOL SetCvlIRC6_Wind2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad, ArINT &arSLoad,
		ArINT &arCSLoad, ArUNT &arIgnorePsi1FactorMLoad, BOOL bETC);
	BOOL SetCvlIRC6_Therm1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArUNT &arMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
		ArINT &arThermal, ArINT &arCSLoad);
	BOOL SetCvlIRC6_Therm2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
		ArINT &arThermal, ArINT &arSLoad, ArINT &arCSLoad);




	BOOL SetCvlIRC6_QP(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,ArINT &arEvLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,ArINT &arErLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArINT &arThermal,ArUNT &arMLoad,ArINT& arWprLoad,ArINT& arBLoad,ArINT& arWpLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,ArINT& arTLoad, int iIRC112Case);



	/////////////////////////////////////////////
	// JSCE02 (KYM20040309)
	/////////////////////////////////////////////
	void Set_CvlJSCE_No1_3(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, 
		ArINT& arTgLoad, ArINT& arTlLoad, ArINT& arSLoad, ArINT& arCSLoad, 
		ArINT& arGridD, ArINT& arGridDe, ArINT& arGridLv, ArINT& arGridCrl, ArINT& arGridLe, 
		int nStrType, BOOL bActive); // nStrType 1, 2, 3 ¹ø ÇÏÁßÁ¶ÇÕ ±¸ºÐº¯¼ö
	void Set_CvlJSCE_No1_3_M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arStlLoad, ArUNT& arSmLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, 
		ArINT& arTgLoad, ArINT& arTlLoad, ArINT& arSLoad, ArINT& arCSLoad, 
		ArINT& arGridD, ArINT& arGridDe, ArINT& arGridCrl, int nStrType, BOOL bActive); // nStrType 1, 2, 3 ¹ø ÇÏÁßÁ¶ÇÕ ±¸ºÐº¯¼ö);
	void Set_CvlJSCE_No4(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arELoad, ArUNT& arEspLoad, ArINT& arCSLoad,
		ArINT& arGridD, ArINT& arGridDe, int nEtrType, BOOL bActive);  // nEtrType : 11=ELoad, 12=EspLoad
	void Set_CvlJSCE_No5_7(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arCSLoad, 
		ArINT& arGridD, ArINT& arGridDe, ArINT& arGridLv, ArINT& arGridCrl, ArINT& arGridLe, 
		int nFacType, BOOL bActive);  // nFacType 5, 6, 7 ¹ø ÇÏÁßÁ¶ÇÕ ±¸ºÐº¯¼ö
	void Set_CvlJSCE_No5_7_M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
		ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arCSLoad, 
		ArINT& arGridD, ArINT& arGridDe, ArINT& arGridCrl, int nFacType, BOOL bActive);  // nFacType 5, 6, 7 ¹ø ÇÏÁßÁ¶ÇÕ ±¸ºÐº¯¼ö

    // PMS.5866 Civil JP °ÝÀÚÇØ¼® À§ÀÚµå ÇÏÁßÁ¶ÇÕ ÀÚµ¿»ý¼º ±âÁØÃß°¡ ¹× ±â´É°³¼±
    void Set_CvlJSCE_No3_1_2(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
                             ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, 
                             ArINT& arTgLoad, ArINT& arTlLoad, ArINT& arSLoad, ArINT& arCSLoad, ArINT& arWLoad, 
                             ArINT& arGridD, ArINT& arGridDe, ArINT& arGridLv, ArINT& arGridCrl, ArINT& arGridLe, 
                             int nStrType, BOOL bActive); // nStrType 1, 2¹ø ÇÏÁßÁ¶ÇÕ ±¸ºÐº¯¼ö
    void Set_CvlJSCE_No3_1_2_M(CString strCode, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, 
                               ArINT& arStlLoad, ArUNT& arSmLoad, ArUNT& arMLoad, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, 
                               ArINT& arTgLoad, ArINT& arTlLoad, ArINT& arSLoad, ArINT& arCSLoad, ArINT& arWLoad, 
                               ArINT& arGridD, ArINT& arGridDe, ArINT& arGridLv, ArINT& arGridCrl, ArINT& arGridLe, 
                               int nStrType, BOOL bActive); // nStrType 1, 2¹ø ÇÏÁßÁ¶ÇÕ ±¸ºÐº¯¼ö
    void Set_CvlJSCE_No3_3(CString strCode, ArINT& arWLoad, ArINT& arCSLoad, BOOL bActive); // Wind Load Only


    /////////////////////////////////////////////
    // JSCE17
    /////////////////////////////////////////////
    void    Set_LoadComb4CivilNameJSCE17(T_LCOM_D& rData, int nStrSer);
    BOOL    Check_JSCE17_LoadCase(const EN_JSCE17_NO& enLcomNo, BOOL bEsp=FALSE);

    int     Get_JSCE17_LCOM_List(CArray<JSCE17_LCOM, JSCE17_LCOM&>& raLcomList);
    BOOL    Get_JSCE17_Factor   (const EN_JSCE17_NO& enLcomNo, const UINT& nNum, double* raFactor, CString* rastrFactor);
    int     Get_JSCE17_LoadCount(const EN_JSCE17& enLcase);
    CString Get_JSCE17_LoadChar (const EN_JSCE17& enLcase);

    void Set_CvlJSCE17(const EN_JSCE17_NO& enLcomNo, 
                       ArINT& arDLoad  , ArINT& arLLoad  , ArINT& arIlLoad , ArINT& arCrlLoad, ArUNT& arMLoad , 
                       ArINT& arPsLoad , ArINT& arCrLoad , ArINT& arShLoad ,
                       ArINT& arEpLoad , ArINT& arWpLoad , ArINT& arBLoad  , 
                       ArINT& arTgLoad , ArINT& arTlLoad , ArINT& arSLoad  ,
                       ArINT& arStlLoad, ArUNT& arSmLoad , ArINT& arCfLoad , ArINT& arBrkLoad, 
                       ArINT& arWLoad  , ArINT& arWlLoad , ArINT& arWprLoad, 
                       ArINT& arELoad  , ArUNT& arEspLoad, ArINT& arCoLoad , ArINT& arErLoad , 
                       ArINT& arGridD  , ArINT& arGridDe , ArINT& arGridLv , ArINT& arGridCrl, ArINT& arGridLe,
                       ArINT& arCSLoad , 
                       BOOL bMoving=FALSE, BOOL bThermal=FALSE, BOOL bEsp=FALSE);
    
    void Add_CvlJSCE17_D       (const EN_JSCE17_NO& enLcomNo, ArINT& arDLoad  , ArINT& arGridD, ArINT& arGridDe, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_M       (const EN_JSCE17_NO& enLcomNo, ArUNT& arMLoad  , CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_L       (const EN_JSCE17_NO& enLcomNo, ArINT& arLLoad  , ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arGridLv , ArINT& arGridCrl, ArINT& arGridLe, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_PS_CR_SH(const EN_JSCE17_NO& enLcomNo, ArINT& arPsLoad , ArINT& arCrLoad, ArINT& arShLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_EP_WP_B (const EN_JSCE17_NO& enLcomNo, ArINT& arEpLoad , ArINT& arWpLoad, ArINT& arBLoad , CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_STL_SM  (const EN_JSCE17_NO& enLcomNo, ArINT& arStlLoad, ArUNT& arSmLoad , CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_CF_BRK  (const EN_JSCE17_NO& enLcomNo, ArINT& arCfLoad , ArINT& arBrkLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_CS      (const EN_JSCE17_NO& enLcomNo, ArINT& arCSLoad , CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlJSCE17_AddOne  (const EN_JSCE17_NO& enLcomNo, const EN_JSCE17& enLcase, ArINT& arLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bOnebyOne=FALSE);
    void Add_CvlJSCE17_AddBiDir(const EN_JSCE17_NO& enLcomNo, const EN_JSCE17& enLcase, ArINT& arLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bBiDir=TRUE);
    void Add_CvlJSCE17_AddBiDir(const EN_JSCE17_NO& enLcomNo, const EN_JSCE17& enLcase, ArUNT& arLoad, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bBiDir=TRUE);
#pragma region lcom_jtgd6004
	/////////////////////////////////////////////
	// JTG D60-04(Áß±¹ Åä¸ñ½Ã¹æ¼­ 2004) - KJH20041018
	/////////////////////////////////////////////
	BOOL Set_CvlJTJD60_Permanent(ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEvLoad, ArINT& arEhLoad,
		ArINT& arShLoad, ArINT& arCrLoad, ArINT& arBLoad, ArINT& arStlLoad, ArUNT& arSmLoad,
		int nType, CArray<T_LCOM_D, T_LCOM_D&>& arData);
	BOOL SetCvlJTJD60_Capacity(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, int nType);
	void SetCvlJTJD60_Capacity_TPG(T_LCOM_D rData, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		double dTFact, double dTpgFact, BOOL bFr, BOOL bAcdn, int nMsgType, int nStrSer=0, int nType=0);
	BOOL SetCvlJTJD60_Var1(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var2(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var3(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var4(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var56(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, int nType, int nMsgType);
	BOOL SetCvlJTJD60_ETC(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
		ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad,  BOOL bIp,  ArINT& arTLoad, ArINT& arTpgLoad,
		BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType);
	BOOL SetCvlJTJD60_Capacity_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType);
	BOOL SetCvlJTJD60_Var1_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad,ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var2_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad,ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var3_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var4_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var56_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	int  GetCountLcomCase(T_LCOM_D& rData);
	CArray<T_LCOM_D, T_LCOM_D&> m_aAcdnLcomD;
	BOOL SetCvlJTJD60_Accident_E(ArINT& arELoad);     // seismic load
	BOOL SetCvlJTJD60_Accident_Esp(ArUNT& arEspLoad); // response spectrum Load
	BOOL SetCvlJTJD60_Accident_Esp(ArUNT& arEspLoad,ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEhLoad, ArINT& arEvLoad,ArINT& arWpload, ArINT& arBLoad); // response spectrum seismic Load by maxiao 2009-10-30
	////add by Maxiao 2012-3-2.
	BOOL SetCvlCJJ166_2011_Accident_Esp(ArUNT& arEspLoad,ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEhLoad, ArINT& arEvLoad,ArINT& arWpLoad, ArINT& arBLoad,ArINT& arTLoad);

	BOOL SetCvlJTJD60_Accident_Ct(ArINT& arCtLoad);   // vessel collision load(Â÷·®Ãæµ¹ÇÏÁß)
	BOOL SetCvlJTJD60_Accident_Cv(ArINT& arCvLoad);   // vehicle collision load(¼±¹ÚÃæµ¹ÇÏÁß)
	BOOL SetCvlJTJD60_Service(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, BOOL bElast=FALSE);
	BOOL SetCvlJTJD60_Service_New(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType);
	double CalcMovingILFactor();
	BOOL SetCvlJTJD60_Var1_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var2_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var3_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var4_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_Var56_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_ETC_S(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
		ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad,  BOOL bIp,  ArINT& arTLoad,
		ArINT& arTpgLoad, BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, int nMsgType);
#pragma endregion
	/////////////////////////////////////////////////////////////////////////////////
	#pragma region lcom_jtgd6015
	//////////////////////////////////////////////////////////////////////////
	// JTG D60-15(¹«Â·ÇÅº­Éè¼ÆÍ¨ÓÃ¹æ·¶ 2015£©add by gongxing 20151022
	// JTG D60-15(¹«Â·ÇÅº­
	//ÓÀ¾Ã×÷ÓÃ×éºÏ
	BOOL Set_CvlJTGD60_15_Permanent(ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEvLoad, ArINT& arEhLoad,
		ArINT& arShLoad, ArINT& arCrLoad, ArINT& arBLoad, ArINT& arStlLoad, ArUNT& arSmLoad,
		int nType, CArray<T_LCOM_D, T_LCOM_D&>& arData);

	//»ù±¾×éºÏ
	BOOL SetCvlJTGD60_15_Capacity_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad,ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType);

	//Å¼È»×éºÏ
	/*
	*	nAcdType = 0 ±íÊ¾¹´Ñ¡ÁËÆµÓö
	*	nAcdType = 1 ±íÊ¾¹´Ñ¡ÁË×¼ÓÀ¾Ã
	*  modified by gongxing at 2016-04-13
	*/
	BOOL SetCvlJTGD60_15_Capacity_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad,int nType,int nAcdType);

	BOOL SetCvlJTJD60_15_Accident_Ct(ArINT& arCtLoad);   // vessel collision load(³µÁ¾×²»÷Á¦)
	BOOL SetCvlJTJD60_15_Accident_Cv(ArINT& arCvLoad);   // vehicle collision load(´¬Ö»»òÆ¯Á÷Îï×²»÷Á¦)

	//ÆµÓö¡¢×¼ÓÀ¾Ã¡¢±ê×¼×éºÏ
	BOOL SetCvlJTGD60_15_Service_New(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, ArINT& arIlpLoad,ArINT& arCoLoad,int nType);

	BOOL SetCvlJTGD60_15_Var1_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad,ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad,ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var2_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad,ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var3_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var4_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad,ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var56_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTJD60_15_ETC_M(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
		ArINT& arWLoad, BOOL bW,ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad,  BOOL bIp,  ArINT& arTLoad, ArINT& arTpgLoad,
		BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType);
	BOOL SetCvlJTJD60_15_ETC(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
		ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad,  BOOL bIp,  ArINT& arTLoad, ArINT& arTpgLoad,
		BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType);


	BOOL SetCvlJTJD60_15_Var1_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);//modified by gongxing at 2016-04-13

	BOOL SetCvlJTJD60_15_Var2_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad,  ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);//modified by gongxing at 2016-04-13

	BOOL SetCvlJTJD60_15_Var3_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);//modified by gongxing at 2016-04-13

	BOOL SetCvlJTJD60_15_Var4_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad,ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);//modified by gongxing at 2016-04-13

	BOOL SetCvlJTJD60_15_Var56_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);//modified by gongxing at 2016-04-13


	BOOL SetCvlJTGD60_15_Var1_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var2_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var3_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var4_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlJTGD60_15_Var56_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);

	BOOL SetCvlJTJD60_15_ETC_S(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, 
		ArINT& arBrkLoad, BOOL bBrk,ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad,
		ArINT& arTpgLoad, BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, int nMsgType);
	//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region lcom_EngneeringStructureCommonCode_GB19
	//¹¤³Ì½á¹¹Í¨ÓÃÉè¼Æ¹æ·¶
	//////////////////////////////////////////////////////////////////////////
	//ÓÀ¾Ã×÷ÓÃ×éºÏ
	BOOL Set_CvlESCGB19_Permanent(ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEvLoad, ArINT& arEhLoad,
		ArINT& arShLoad, ArINT& arCrLoad, ArINT& arBLoad, ArINT& arStlLoad, ArUNT& arSmLoad,
		int nType, CArray<T_LCOM_D, T_LCOM_D&>& arData);

	//»ù±¾×éºÏ
	BOOL SetCvlESCGB19_Capacity_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad,ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType);

	//Å¼È»×éºÏ
	/*
	*	nAcdType = 0 ±íÊ¾¹´Ñ¡ÁËÆµÓö
	*	nAcdType = 1 ±íÊ¾¹´Ñ¡ÁË×¼ÓÀ¾Ã
	*/
	BOOL SetCvlESCGB19_Capacity_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad,int nType,int nAcdType);

	BOOL SetCvlESCGB19_Accident_Ct(ArINT& arCtLoad);   // vessel collision load(³µÁ¾×²»÷Á¦)
	BOOL SetCvlESCGB19_Accident_Cv(ArINT& arCvLoad);   // vehicle collision load(´¬Ö»»òÆ¯Á÷Îï×²»÷Á¦)

	//ÆµÓö¡¢×¼ÓÀ¾Ã¡¢±ê×¼×éºÏ
	BOOL SetCvlESCGB19_Service_New(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, ArINT& arIlpLoad,ArINT& arCoLoad,int nType);

	BOOL SetCvlESCGB19_Var1_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad,ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad,ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var2_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad,ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var3_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var4_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad,ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var56_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_ETC_M(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
		ArINT& arWLoad, BOOL bW,ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad,  BOOL bIp,  ArINT& arTLoad, ArINT& arTpgLoad,
		BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType);
	BOOL SetCvlESCGB19_ETC(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
		ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad,  BOOL bIp,  ArINT& arTLoad, ArINT& arTpgLoad,
		BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType);


	BOOL SetCvlESCGB19_Var1_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);

	BOOL SetCvlESCGB19_Var2_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad,  ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);

	BOOL SetCvlESCGB19_Var3_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);

	BOOL SetCvlESCGB19_Var4_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad,ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);

	BOOL SetCvlESCGB19_Var56_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
		ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
		ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType,int nAcdType);


	BOOL SetCvlESCGB19_Var1_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var2_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var3_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var4_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);
	BOOL SetCvlESCGB19_Var56_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad,
		ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad,ArINT& arWlLoad, ArINT& arSfLoad,
		ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
		int nType, int nMsgType);

	BOOL SetCvlESCGB19_ETC_S(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, 
		ArINT& arBrkLoad, BOOL bBrk,ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad,
		ArINT& arTpgLoad, BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, int nMsgType);
	//////////////////////////////////////////////////////////////////////////
#pragma endregion
	/////////////////////////////////////////////
	// TB 10002.1-05(CHINA railway code ) - maxiao20070312
	/////////////////////////////////////////////

	//Get Moving Load of Railway: highway ¡¢CH-SL¡¢CH-NL¡¢CH-HL.
	BOOL GetRailMvldKeyList(ArUNT& aMvSLKeyList,ArUNT& aMvNLKeyList,ArUNT& aMvHLKeyList, ArUNT& aMvHKeyList,CString strCode);

	//get Response Spectrum Load of CHINA Railway
	BOOL Set_CvlTB10002_1_Esp(ArUNT& arEspLoad_x,ArUNT& arEspLoad_y);

	//Constant Load combination
	BOOL Set_CvlTB10002_1_Constant(ArINT& arDLoad, ArINT& arDcLoad, ArINT& arPsLoad, ArINT& arShLoad, ArINT& arCrLoad,
		ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arWpLoad, ArINT& arBLoad, ArINT& arStlLoad,ArUNT& arSmLoad,
		CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_1);  
	//Constant & living Load combination
	BOOL Set_CvlTB10002_1_ConLivLoad(CArray<T_LCOM_D, T_LCOM_D&>& arConData, ArUNT& arMvSlLoad, ArUNT& arMvNlLoad, ArUNT& arMvHlLoad,
		ArUNT& arMvldLoad,ArINT& arIlLoad,ArINT& arBkLoad,
		ArINT& arCfLoad, ArINT& arLLoad, CArray<T_LCOM_D, T_LCOM_D&>& arConLivData);
	//Main Load combination
	BOOL Set_CvlTB10002_1_Main(CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_1, CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_1_ConLiv, int iActive);
	//Main & Additional Load combination
	BOOL Set_CvlTB10002_1_MainAddition(CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_1, CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_1_ConLiv,
		ArINT& arBrkLoad,ArINT& arWLoad,ArINT& arFpLoad,ArINT& arIpLoad,ArINT& arTLoad,ArINT& arTpgLoad/*,arBylLoad(»ù´¡¶³ÕÍÁ¦)*/);   
	//Main & Special Load combination: use Constant & living Load combination
	BOOL set_CvlTB10002_1_MainSpecial(CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_1, CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_1_ConLiv,
		/*,*/ArINT& arCvLoad,ArINT& arCtLoad,ArINT& arErLoad/*,*/);
	//Main & Special Load combination: use Load include seismic.
	BOOL set_CvlTB10002_1_MainSpecial2(ArINT& arDLoad,ArINT& arDcLoad,ArINT& arEhLoad,ArINT& arEvLoad,ArINT& arWpLoad,ArINT& arBLoad,
		ArUNT& arMvSlLoad,ArUNT& arMvNlLoad,ArUNT& arMvHlLoad,ArINT& arCfLoad,ArUNT& arEspLoad_x,ArUNT& arEspLoad_y);
	//set W FP IP,this three Load case which is Separately Combination.
	void set_CvlTB10002_1_Load3(T_LCOM_D Lcom,ArINT& arWLoad,BOOL bW,ArINT& arFPLoad,BOOL bFP,ArINT& arIPLoad,BOOL bIP);

	//set T TPG,this Two Load case which is Separately Combination.
	void set_CvlTB10002_1_Load2(T_LCOM_D Lcom,ArINT& arTLoad,BOOL bT,ArINT& arTPGLoad,BOOL bTPG);

	/////////////////////////////////////////////
	// TB 10002-2017(CHINA railway code )
	/////////////////////////////////////////////

	//Get Moving Load of Railway: highway ¡¢CH-SL¡¢CH-NL
	BOOL Get_CvlTB10002_2017_RailMvldKeyList(ArUNT& aMvSLKeyList,ArUNT& aMvNLKeyList,ArUNT& aMvSNLKeyList, ArUNT& aMvldKeyList,CString strCode);

	//get Response Spectrum Load of CHINA Railway
	BOOL Set_CvlTB10002_2017_Esp(ArUNT& arEspLoad_x,ArUNT& arEspLoad_y);

	//Constant Load combination
	BOOL Set_CvlTB10002_2017_Constant(ArINT& arDLoad, ArINT& arDcLoad, ArINT& arPsLoad, ArINT& arShLoad, ArINT& arCrLoad,
		ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arWpLoad, ArINT& arBLoad, ArINT& arStlLoad,ArUNT& arSmLoad,
		CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_2017);  
	//Constant & living Load combination
	BOOL Set_CvlTB10002_2017_ConLivLoad(CArray<T_LCOM_D, T_LCOM_D&>& arConData, ArUNT& arMvSlLoad, ArUNT& arMvNlLoad, ArUNT& arMvSNlLoad,
		ArUNT& arMvldLoad,ArINT& arIlLoad,ArINT& arCfLoad, ArINT& arLLoad, ArINT& arCrlLoad, CArray<T_LCOM_D, T_LCOM_D&>& arConLivData);
	//Main Load combination
	BOOL Set_CvlTB10002_2017_Main(CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_2017, CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_2017_ConLiv, int iActive);
	//Main & Additional Load combination
	BOOL Set_CvlTB10002_2017_MainAddition(CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_2017, CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_2017_ConLiv,
		ArINT& arBrkLoad,ArINT& arFrLoad,ArINT& arWLoad,ArINT& arFpLoad,ArINT& arIpLoad,ArINT& arTLoad,ArINT& arTpgLoad);   
	//Main & Special Load combination: use Constant & living Load combination
	BOOL set_CvlTB10002_2017_MainSpecial(CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_2017, CArray<T_LCOM_D, T_LCOM_D&>& aLcom_TB10002_2017_ConLiv,
		/*,*/ArINT& arCvLoad,ArINT& arCtLoad,ArINT& arErLoad,ArINT& arBkLoad);
	//Main & Special Load combination: use Load include seismic.
	BOOL set_CvlTB10002_2017_MainSpecial2(ArINT& arDLoad,ArINT& arDcLoad,ArINT& arEhLoad,ArINT& arEvLoad,ArINT& arWpLoad,ArINT& arBLoad,
		ArUNT& arMvSlLoad,ArUNT& arMvNlLoad,ArUNT& arMvSNlLoad,ArINT& arCfLoad,ArUNT& arEspLoad_x,ArUNT& arEspLoad_y);
	//set W FP IP,this three Load case which is Separately Combination.
	void set_CvlTB10002_2017_Load3(T_LCOM_D Lcom,ArINT& arWLoad,BOOL bW,ArINT& arFPLoad,BOOL bFP,ArINT& arIPLoad,BOOL bIP);

	//set T TPG,this Two Load case which is Separately Combination.
	void set_CvlTB10002_2017_Load2(T_LCOM_D Lcom,ArINT& arTLoad,BOOL bT,ArINT& arTPGLoad,BOOL bTPG);

	///// Eurocode 4
	BOOL SetCvlEurocode4_Traffic(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2, BOOL bAccompay=FALSE);
    BOOL SetCvlEurocode4_Traffic_Therm(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
        ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
        ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
        ArINT &arCSLoad, ArINT &arTLoad, ArINT &arTPGLoad, BOOL bAccompay=FALSE);
	BOOL SetCvlEurocode4_Wind1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArUNT &arMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad,ArUNT &arIgnorePsi1FactorMLoad, BOOL bETC);
	BOOL SetCvlEurocode4_Wind2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad, ArINT &arSLoad,
		ArINT &arCSLoad, ArUNT &arIgnorePsi1FactorMLoad, BOOL bETC);
	BOOL SetCvlEurocode4_Therm1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArUNT &arMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
		ArINT &arThermal, ArINT &arThermalGradiant, ArINT &arCSLoad, ArUNT &arPsi1FactorMLoad1, ArUNT &arPsi1FactorMLoad2);
	BOOL SetCvlEurocode4_Therm2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
		ArINT &arThermal, ArINT &arThermalGradiant, ArINT &arSLoad, ArINT &arCSLoad);
	BOOL SetCvlEurocode4_Snow(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arSLoad, ArINT &arCSLoad);
	BOOL SetCvlEurocode4_Erection(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad, ArINT &arTLoad, ArINT &arTpgLoad, ArINT &arErLoad, ArINT &arCSLoad);
	BOOL SetCvlEurocode4_Traffic_serv1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad, ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,int nLeadingType);
	BOOL SetCvlEurocode4_Traffic_serv2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
        ArINT &arTLoad, ArINT &arTpgLoad, ArINT &arCSLoad, ArUNT &arPsi1FactorMLoad1,ArUNT &arPsi1FactorMLoad2,BOOL bFR, int nLeadingType);
	BOOL SetCvlEurocode4_Wind_serv1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArUNT &arMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arCSLoad, BOOL bFr);
	BOOL SetCvlEurocode4_Wind_serv2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad, ArINT &arSLoad,
		ArINT &arCSLoad);
	BOOL SetCvlEurocode4_Therm_serv1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArUNT &arMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad,
		ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
        ArINT &arTLoad, ArINT &arTpgLoad, ArINT &arCSLoad, BOOL bFr);
	BOOL SetCvlEurocode4_Therm_serv2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
        ArINT &arTLoad, ArINT &arTpgLoad, ArINT &arSLoad, ArINT &arCSLoad, BOOL bQP);
	BOOL SetCvlEurocode4_Snow_serv1(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
		ArINT &arSLoad, ArINT &arCSLoad);
	BOOL SetCvlEurocode4_Snow_serv2(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad,
        ArINT &arTLoad, ArINT &arTpgLoad, ArINT &arSLoad, ArINT &arCSLoad);
	BOOL SetCvlEurocode4_QP(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad,
		ArINT &arCrLoad, ArINT &arShLoad, ArINT &arPsLoad, ArUNT &arSmLoad, ArINT &arCSLoad);

    /////////////////////////////////////////////
    // AS 5100:7-2017 (KJH20180701)
    /////////////////////////////////////////////

    void GetAS_5100_2017Factor( 
        CString& sD1Fac,  CString& sD2Fac,  CString& sDw1Fac,  CString& sDw2Fac,  CString& sEp1Fac,		 CString& sEp2Fac,
        CString& sWp1Fac, CString& sWp2Fac,     
        double& dD1,  double& dD2,  double& dDw1,  double& dDw2,  double& dEp1,   double& dEp2,   
        double& dWp1, double& dWp2, BOOL b1stGrp);

    BOOL Check_AS5100_LoadCase(const EN_AS5100_NO& enLcomNo);
    CString Add_CvlAS5100_Title (const EN_AS5100_NO& enLcomNo);

    int Get_AS5100_LCOM_List(CArray<AS5100_LCOM, AS5100_LCOM&>& raLcomList);

    void Set_CvlAS5100_LoadFactorPerm(const EN_AS5100_NO& enLcomNo, double& dPsFactor, double& dCRSHFactor, double& dStlFactor, double& dWFBFactor);

    void GetAS5100FactorStrPermErec(int& nErec, int& nErection, double& dDFac, double& dDwFac, int& nn,CString& strcEL, CString& sDescription, CString& sPrefix);
    void GetAS5100FactorStrPSCRSHErec(double& dPsFactor, double& dCRSHFactor, BOOL& b1stGrp, int& nn, CString& sDescription, CString& sPrefix);

    void GetAS5100FactorStrLiveErec(const EN_AS5100_NO& enLcomNo, int& nErec, int& nErection, double& dWFactor, double& dTFactor, CString& strcEL, CString& sDescription, CString& sPrefix);
    void GetAS5100FactorStrBasicULS(const EN_AS5100_NO& enLcomNo, int& nDL, int& nDwL, int& nEpL, int& nWpL, int& nPsL, int& nCrL, int& nShL, int& nStL, int& nFpL, int& nBL, int& nSettle, 
        double& dDFactor, double& dDwFactor, double& dEpFactor, double& dGwFactor,  double& dPsFactor, double& dCRSHFactor, double& dStlFactor, double& dWFBFactor,                            
        CString&  strFixSm, CString& sDescription, CString& sPrefix);

    void GetAS5100FactorStrLive(const EN_AS5100_NO& enLcomNo, int& m_nMCount, int& nBrkL, int& nCfL, int& nTL, int& nTpgL, int& nWL, int& nEL, int& nEsp, int& nCoL,
        double& dCoFactor, double& dMFactor, double& dBCFactor, double& dTFactor, double& dWF, double& dEFactor, 
        CString& strFixE, CString& strFixM, CString& strFixW, CString& strFixT, CString& strFixTpg, CString& sDescription, CString& sPrefix);

    void GetAS5100TransientFactor(const EN_AS5100_NO& enLcomNo, double& dMFactor, double& dBCFactor, double& dTFactor, double& dWFactor, double& dEFactor, double& dCoFactor);

    void SetAS5100CSStagePerm(const EN_AS5100_NO& enLcomNo, int& nErec, int& nErection, double& dDFac, double& dDwFac, int& nn, int& nD, int& nDw, ArINT& arCSLoad, int& nBoth, int& nLoadCase, CString& strcEL, T_LCOM_D& rData);
    void SetAS5100CSStagePSCRSHE(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad, double& dPsFactor, double& dCRSHFactor, int& nn, int& nLoadCase, T_LCOM_D& rData);
    void SetAS5100CSStageTransient(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad, int& nErection, double& dTFactor, double& dWFactor, int& nn, int& nLoadCase, T_LCOM_D& rData);

    void SetAS5100IncludeCSMoving(const EN_AS5100_NO& enLcomNo, int& nMove, double& dMFactor, double& dBCFactor, double& dCoFactor,
        ArINT& arCoLoad, ArUNT& arMLoad, ArINT& arBrkLoad , ArINT& arCfLoad, int& nCoL, int& nBrkL, int& nCfL, int& nLoadCase, T_LCOM_D& rData);

    void SetAS5100IncludeCSTemp(const EN_AS5100_NO& enLcomNo, int& nTemp, int& nTempg, double& dTFactor, ArINT& arTLoad, ArINT& arTpgLoad, int& nTL, int& nTpgL, int& nLoadCase, T_LCOM_D& rData);

    void SetAS5100IncludeCSWind(const EN_AS5100_NO& enLcomNo, int& nWind, double& dWF, ArINT& arWLoad, int& nWL, int& nLoadCase, T_LCOM_D& rData);

    void SetAS5100IncludeCSEarth(const EN_AS5100_NO& enLcomNo, double& dEFactor, ArINT& arELoad, ArUNT& arEspLoad, int& nEL, int& nEsp, int& nLoadCase, int nEarthIdx, T_LCOM_D& rData);


    void SetAS5100IncludeCSBasic(const EN_AS5100_NO& enLcomNo, ArINT& arDLoad , ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad, 
        double& dDFactor, double& dDwFactor, double& dEpFactor, double& dGwFactor, int& nD, int& nDw, int& nEp, int& nWp,
        int& nDL, int& nDwL, int& nEpL, int& nWpL, int& nBoth, int& nLoadCase, T_LCOM_D& rData);

    void SetAS5100IncludeCSPerm(const EN_AS5100_NO& enLcomNo, ArINT& arFpLoad, ArINT& arBLoad , ArINT& arCrLoad, ArINT& arShLoad , ArINT& arPsLoad , ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad,
        double& dPsFactor, double& dCRSHFactor, double& dStlFactor, double& dWFBFactor, double& dCoFactor,
        int& nPsL, int& nCrL, int& nShL, int& nStL, int& nFpL, int& nBL, int& nSettle, int& nCoL, int& nLoadCase, T_LCOM_D& rData);

    void Set_CvlAS5100(const EN_AS5100_NO& enLcomNo, 
        ArINT& arDLoad , ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad , 
        ArINT& arFpLoad, ArINT& arBLoad , ArINT& arCrLoad, ArINT& arShLoad , ArINT& arPsLoad , ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad, 
        ArINT& arWLoad,  ArINT& arELoad , ArUNT& arEspLoad,  ArINT& arTLoad , ArINT& arTpgLoad, ArINT& arBrkLoad, ArINT& arCfLoad , ArUNT& arMLoad , ArINT& arCSLoad);

    void Set_CvlAS5100_ULS(const EN_AS5100_NO& enLcomNo, 
        ArINT& arDLoad, ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arBLoad,  ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, 
        ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad, ArINT& arCSLoad, ArUNT& arMLoad, ArINT& arBrkLoad , ArINT& arCfLoad, ArINT& arWLoad, ArINT& arTLoad,  ArINT& arTpgLoad,  ArINT& arELoad, ArUNT& arEspLoad, BOOL b1stGrp);

    void Set_CvlAS5100_SLS(const EN_AS5100_NO& enLcomNo, 
        ArINT& arDLoad, ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad, ArINT& arFpLoad, ArINT& arBLoad,  ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, 
        ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arCoLoad, ArINT& arCSLoad, ArUNT& arMLoad, ArINT& arBrkLoad , ArINT& arCfLoad, ArINT& arWLoad, ArINT& arTLoad,  ArINT& arTpgLoad,  ArINT& arELoad, ArUNT& arEspLoad, BOOL b1stGrp);

    void Set_CvlAS5100_Transfer(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad);

    void Set_CvlAS5100_FLS(const EN_AS5100_NO& enLcomNo, ArINT& arCSLoad, ArUNT& arMLoad); // Fatigue Limit State Design 0.7 * Moving Load * (1 + alpha)

	////////////////////INDIA IRS LOAD COMBINATION/////////////
	BOOL SetCvlIRS_ST_ULT_TYPE1(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1

	BOOL SetCvlIRS_ST_ULT_TYPE2_W(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad, ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1

	BOOL SetCvlIRS_ST_ULT_TYPE2_W_OTH(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1

	BOOL SetCvlIRS_ST_ULT_TYPE2_W_EP_OTH(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1

	BOOL SetCvlIRS_ST_ULT_TYPE2_EQ(int eql,ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArINT& arELoad,ArUNT &arMLoad, ArUNT &arSmLoad,ArUNT& arEspLoad_X,ArUNT& arEspLoad_Y,ArUNT& arEspLoad_Z);//Static Ultimate Type 1

	BOOL SetCvlIRS_ST_ULT_TYPE2_EQ_OTH(int eql,ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArINT& arELoad,ArUNT &arMLoad, ArUNT &arSmLoad,ArUNT& arEspLoad_X,ArUNT& arEspLoad_Y,ArUNT& arEspLoad_Z);//Static Ultimate Type 1

	BOOL SetCvlIRS_ST_ULT_TYPE2_EQ_EP_OTH(int eql,ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArINT& arELoad,ArUNT &arMLoad, ArUNT &arSmLoad,ArUNT& arEspLoad_X,ArUNT& arEspLoad_Y,ArUNT& arEspLoad_Z);//Static Ultimate Type 1

	BOOL SetCvlIRS_ST_ULT_TYPE3_T_TG_EP(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arTLoad,  ArINT &arThermal,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad, ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);
	BOOL SetCvlIRS_ST_ULT_TYPE4_T_EP(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arTLoad,  ArINT &arThermal,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad, ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);
	BOOL SetCvlIRS_ST_ULT_TYPE5(int dDrlCOunt,ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArINT &arDrlLoad,ArINT &arDrlLoad_Ult,ArINT &arDrlLoad_Ser, ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1
	////////////IRS CS LCOMB////////////
	BOOL SetCvlIRS_CS_ULT_TYPE1(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1
	////////////IRS Serviceability LCOMB////////////
	BOOL SetCvlIRS_ST_SER_TYPE1(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1
	BOOL SetCvlIRS_ST_SER_TYPE2_W_EP_OTH(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1
	BOOL SetCvlIRS_ST_SER_TYPE2_EQ_EP_OTH(int eql,ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArINT& arELoad,ArUNT &arMLoad, ArUNT &arSmLoad,ArUNT& arEspLoad_X,ArUNT& arEspLoad_Y,ArUNT& arEspLoad_Z);//Static Ultimate Type 1
	BOOL SetCvlIRS_ST_SER_TYPE3_T_TG_EP(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arTLoad,  ArINT &arThermal,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad, ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);
	BOOL SetCvlIRS_ST_SER_TYPE4_T_EP(ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arTLoad,  ArINT &arThermal,ArINT &arEpLoad,ArINT &arEhLoad,ArINT &arWLoad,ArINT &arWlLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad, ArINT &arCrlLoad,ArUNT &arMLoad, ArUNT &arSmLoad);
	BOOL SetCvlIRS_ST_SER_TYPE5(int dDrlCOunt,ArINT &arDLoad, ArINT &arBLoad,ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arPLLoad, ArINT &arBLLoad,ArINT &arStlLoad, 
		ArINT &arPsLoad,ArINT &arEpLoad,ArINT &arEhLoad, ArINT &arEvLoad,ArINT &arEsLoad,ArINT &arWpLoad,ArINT &arLsLoad,
		ArINT &arCrLoad, ArINT &arLLoad,ArINT &arCfLoad,ArINT &arBrkLoad, ArINT &arBkLoad,ArINT &arShLoad,
		ArINT &arCSLoad,ArINT &arCrlLoad,ArINT &arDrlLoad,ArINT &arDrlLoad_Ult,ArINT &arDrlLoad_Ser, ArUNT &arMLoad, ArUNT &arSmLoad);//Static Ultimate Type 1
	BOOL IsLcaseDefInCode();
	//////FLS ??
	//void SetCvl_AS5100_FLS1(CString strCode,
	//	ArINT& arDLoad,   ArINT& arDwLoad, ArINT& arEpLoad, ArINT& arWpLoad,   ArINT& arFpLoad,  ArINT& arBLoad,  ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, 
	//	ArINT& arStlLoad, ArINT& arCSLoad, ArINT& arTLoad,  ArINT& arTpgLoad,  ArINT& arBrkLoad, ArINT& arCfLoad, ArUNT& arMLoad_F,  ArINT& arELoad,  ArINT& arWLoad
	//	);

    /////////////////////////////////////////////
    // BD 21 (khj0102 2019/08/05) -> BS 5400 À¸·Î ÀÌ¸§ º¯°æ
    /////////////////////////////////////////////

    BOOL    Check_BS5400_LoadCase(const EN_BS5400_NO& enLcomNo);

    CString Add_CvlBS5400_Title (const EN_BS5400_NO& enLcomNo, BOOL bStrength);

    int Get_BS5400_LCOM_List(CArray<BS5400_LCOM, BS5400_LCOM&>& raLcomList);
   
    void Set_CvlBS5400(const EN_BS5400_NO& enLcomNo, 
        ArINT& arDLoad , ArINT& arDwLoad, ArINT& arDcLoad,  ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arEvLoad,  ArINT& arEhLoad, 
        ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad,  ArUNT& arMLoad,   ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arCoLoad,
        ArINT& arErLoad, ArINT& arTLoad,  ArINT& arTpgLoad, ArINT& arWLoad,   ArINT& arFrLoad, ArINT& arCSLoad,  
        BOOL bStrength = FALSE, BOOL bMoving = FALSE, BOOL bWind = FALSE);

    void Add_CvlBS5400_Title    (const EN_BS5400_NO& enLcomNo, BOOL bStrength, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_D        (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arDLoad  , double& dD1,  double& dD2,  CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_DW       (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arDwLoad , double& dDw1, double& dDw2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_DC       (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arDcLoad , double& dDc1, double& dDc2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_EV       (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arEvLoad , double& dEv1, double& dEv2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_EH       (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arEhLoad , double& dEh1, double& dEh2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    
    void Add_CvlBS5400_STL_SM   (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arStlLoad, ArUNT& arSmLoad,  double& dStl, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_PS_CR_SH (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arPsLoad , ArINT& arCrLoad,  ArINT& arShLoad, double& dPs, double& dCRSH, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_M        (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArUNT& arMLoad  , double& dMFactor, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_W        (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arWLoad  , double& dWFactor, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_T_TPG    (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad  , ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_CO       (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCoLoad , double& dCoFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_CF_BRK   (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArUNT& arMLoad  , ArINT& arCfLoad,  ArINT& arBrkLoad, double& dBrCfFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_FR       (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arFrLoad , double& dFrFactor, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    
    void Add_CvlBS5400_CS_D (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad , double& dD1,  double& dD2,  CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_CS_DW(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad , double& dDw1, double& dDw2, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_CS_DC(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad , double& dDc1, double& dDc2, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_CS_W     (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad , double& dErFac, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_CS_T     (const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad , double& dTFac, double& dTpgFac, double& dErFac, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlBS5400_CS_PSCRSH(const EN_BS5400_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad , double& dPs,   double& dCRSH, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);

    void GetBD_21Factor( 
        CString& sD1Fac,  CString& sD2Fac, CString& sDw1Fac, CString& sDw2Fac, CString& sDc1Fac,    CString& sDc2Fac,
        CString& sEv1Fac, CString& sEv2Fac,CString& sEh1Fac, CString& sEh2Fac, CString& sGf3ULSFac, CString& sGf3SLSFac,     
        double& dD1,  double& dD2,  double& dDw1,  double& dDw2,  double& dDc1,    double& dDc2,
        double& dEv1, double& dEv2, double& dEh1,  double& dEh2,  double& dGf3ULS, double& dGf3SLS, BOOL b1stGrp);

    void GetBD_21FactoredPerm(
        double& dD1,  double& dD2,  double& dDw1,  double& dDw2,  double& dDc1,    double& dDc2,
        double& dEv1, double& dEv2, double& dEh1,  double& dEh2,  double& dGf);

    /////////////////////////////////////////////
    // AASHTOLRFD 12, 16, 18 (khj0102 2019/09/02)
    /////////////////////////////////////////////

    BOOL    Check_AASHTOSince12_LoadCase(const EN_AASHTOSince12_NO& enLcomNo);

    CString Add_CvlAASHTOSince12_Title (const EN_AASHTOSince12_NO& enLcomNo);

    int Get_AASHTOSince12_LCOM_List(CArray<AASHTOSince12_LCOM, AASHTOSince12_LCOM&>& raLcomList);

    void GetAASHTOSince12Factor(CString& sDc1Fac, CString& sDc2Fac, CString& sDd1Fac, CString& sDd2Fac, CString& sDw1Fac, 
        CString& sDw2Fac, CString& sEh1Fac, CString& sEh2Fac, CString& sEv1Fac, CString& sEv2Fac, 
        CString& sEs1Fac, CString& sEs2Fac, CString& sEl1Fac, CString& sEl2Fac, CString& sPs1Fac, 
        CString& sPs2Fac, CString& sCr1Fac, CString& sCr2Fac, CString& sSh1Fac, CString& sSh2Fac, 
        double& dDc1, double& dDc2, double& dDd1, double& dDd2, double& dDw1, 
        double& dDw2, double& dEh1, double& dEh2, double& dEv1, double& dEv2, 
        double& dEs1, double& dEs2, double& dEl1, double& dEl2, double& dPs1, 
        double& dPs2, double& dCr1, double& dCr2, double& dSh1, double& dSh2, const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength);

    void Set_CvlAASHTOSince12(const EN_AASHTOSince12_NO& enLcomNo, 
        ArINT& arDcLoad, ArINT& arDdLoad, ArINT& arDwLoad, ArINT& arEhLoad, ArINT& arEvLoad, 
        ArINT& arEsLoad, ArINT& arElLoad, ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arWpLoad, ArINT& arSfLoad, 
        ArUNT& arMLoad,  ArINT& arCfLoad, ArINT& arBrkLoad,ArINT& arCrlLoad,ArINT& arLsLoad, ArINT& arWlLoad, 
        ArINT& arFpLoad, ArINT& arBLoad,  ArINT& arWprLoad,ArINT& arWLoad,  ArINT& arELoad,  ArUNT& arEspLoad,
        ArINT& arIpLoad, ArINT& arCtLoad, ArINT& arCvLoad, ArINT& arFrLoad, ArINT& arTLoad,  ArINT& arTpgLoad,
        ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arStlLoad,ArUNT& arSmLoad, ArINT& arCSLoad, 
        BOOL bStrength = FALSE, BOOL bExtreme = FALSE, BOOL bService = FALSE, BOOL bFatigue = FALSE, 
        BOOL bMoving = FALSE, BOOL bWind = FALSE, BOOL bSeis = FALSE);
   
    void Add_CvlAASHTOSince12_Title      (const EN_AASHTOSince12_NO& enLcomNo, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    
    void Add_CvlAASHTOSince12_Trans_Factor (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, double& dCfFac, double& dLFac, double& dWFac, double& dWpFac, double& dFrFac, double& dEFac);
    void Add_CvlAASHTOSince12_Temp_Factor  (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, double& dTFac, double& dTpgFac);
    
    //Strength, Extreme, Fatigue Àü¿ë ´ë½Å Fatigue´Â Steel CompositeÀÎ °æ¿ì¸¸
    void Add_CvlAASHTOSince12_DW         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDwLoad , double& dDw1, double& dDw2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_DC         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDcLoad , double& dDc1, double& dDc2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    //
    //Strength and Extreme Àü¿ë
    void Add_CvlAASHTOSince12_DD         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDdLoad , double& dDd1, double& dDd2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_EV         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEvLoad , double& dEv1, double& dEv2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_EH         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEhLoad , double& dEh1, double& dEh2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_ES         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEsLoad , double& dEs1, double& dEs2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_EL         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arElLoad , double& dEl1, double& dEl2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_PS         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arPsLoad , double& dPs1, double& dPs2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CR         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arCrLoad , double& dCr1, double& dCr2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_SH         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arShLoad , double& dSh1, double& dSh2, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    //
    //Strength and Service Àü¿ë
    void Add_CvlAASHTOSince12_STL_SM     (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arStlLoad, ArUNT& arSmLoad, double& dStl, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_T          (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arTLoad  , double& dTFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_W          (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arWLoad  , double& dWFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_WL         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arWlLoad , double& dWFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    //
    //Extreme Àü¿ë
    void Add_CvlAASHTOSince12_EQ         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arELoad , ArUNT& arEspLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CO         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arIpLoad, ArINT& arCtLoad, ArINT& arCvLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    //Seiscmic
    void Add_CvlAASHTOSince12_EQ_Ortho   (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arELoad , ArUNT& arEspLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    //
    //Strength, Extreme, Service Àü¿ë
    void Add_CvlAASHTOSince12_M          (const EN_AASHTOSince12_NO& enLcomNo, ArUNT& arMLoad  , ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arBrkLoad, double& dLFactor, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_FR         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arFrLoad , double& dFrFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_FL         (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arWpLoad,  ArINT& arFpLoad, ArINT& arBLoad, ArINT& arSfLoad, ArINT& arWprLoad, double& dWpFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CRL_LS     (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arCrlLoad, ArINT& arLsLoad, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    //
    //Service Àü¿ë
    void Add_CvlAASHTOSince12_SLS_DEAD   (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDdLoad , ArINT& arDwLoad, ArINT& arDcLoad ,double& dDead, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_SLS_EARTH  (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arEvLoad , ArINT& arEhLoad, ArINT& arEsLoad, ArINT& arElLoad, double& dDead, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_TPG        (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arTpgLoad, double& dTpgFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_SLS_PSCRSH (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arPsLoad , ArINT& arCrLoad , ArINT& arShLoad, double& dDead, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    //
    //Fatigue Àü¿ë
    void Add_CvlAASHTOSince12_FTG_DEAD   (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arDwLoad, ArINT& arDcLoad, double& dDead, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_FTG_M      (const EN_AASHTOSince12_NO& enLcomNo, ArUNT& arMLoad , ArINT& arLLoad,  ArINT& arIlLoad, ArINT& arCfLoad, double& dLFactor, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_FTG_NO_M   (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arLLoad , ArINT& arIlLoad, ArINT& arCfLoad, double& dLFactor, double& dBrCfFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D,  T_LCOM_D&>& raNewLcom);                                   
    //
    void Add_CvlAASHTOSince12_CS_DW  (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dDw1, double& dDw2, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CS_DC  (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dDc1, double& dDc2, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CS_LIVE    (const EN_AASHTOSince12_NO& enLcomNo, BOOL bFatigue, ArINT& arCSLoad , double& dDFactor, double& dLFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);

    void Add_CvlAASHTOSince12_CS_W    (const EN_AASHTOSince12_NO& enLcomNo, BOOL bFatigue, ArINT& arCSLoad , double& dDFactor, double& dWFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);

    void Add_CvlAASHTOSince12_CS_T    (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad , double& dDFactor, double& dTFactor, double& dTpgFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CS_STL  (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad , double& dDFactor, double& dStlFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CS_E    (const EN_AASHTOSince12_NO& enLcomNo, ArINT& arCSLoad, double& dEFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);

    void Add_CvlAASHTOSince12_CS_PS  (const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dPs1, double& dPs2, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
    void Add_CvlAASHTOSince12_CS_CR_SH(const EN_AASHTOSince12_NO& enLcomNo, BOOL bStrength, BOOL bExtreme, BOOL bService, BOOL bFatigue, ArINT& arCSLoad, double& dCr1, double& dCr2, double& dTFactor, double& dDFactor, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);


    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////// IRS SBC (INDIA - INDIAN RAILWAY STANDARDS STEEL BRIDGE CODE//////
    ///////////////////////////////////////////////////////////////////////////////////////////
    BOOL SetCvlIRSSBC_DW(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArUNT &arMLoad );//D+DC+BL+DW
    BOOL SetCvlIRSSBC_DW_Wind(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArINT &arWLoad, ArUNT &arMLoad );//D+DC+BL+L+IL+CF+LF+DW

    BOOL SetCvlIRSSBC_ICL(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad,ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad,ArINT &arLFLoad, ArUNT &arMLoad);//D+DC+BL+L+IL+CF+LF+DW
    BOOL SetCvlIRSSBC_ICL_Wind(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad,ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad,ArINT &arLFLoad,ArINT &arWLoad, ArUNT &arMLoad);//D+DC+BL+L+IL+CF+LF+DW
    BOOL SetCvlIRSSBC_ICL_T(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad,ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad,ArINT &arLFLoad,ArINT &arTLoad, ArUNT &arMLoad);//D+DC+BL+L+IL+CF+LF+DW+Temp

    BOOL SetCvlIRSSBC_EQ(int eql, ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad,
        ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad, ArINT &arLFLoad, ArINT &arELoad,
        ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, ArUNT &arMLoad);//Static Ultimate Type 1

    BOOL SetCvlIRSSBC_EQ1(int eql, ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad,
        ArINT &arLLoad, ArINT &arIlLoad, ArINT & arCfLoad, ArINT &arLFLoad, ArINT &arELoad,
        ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, ArUNT &arMLoad);//Static Ultimate Type 1

    BOOL SetCvlIRSSBC_DRL(ArINT &arDLoad, ArINT &arDcLoad, ArINT &arDwLoad, ArINT &arBLLoad, ArINT &arDrlLoad, ArUNT &arMLoad);//D+DC+BL+DW
   
	// Civil Load Combination
	// TMH07-2018 (khj0102 2019/08/05) -> BS 5400 À¸·Î ÀÌ¸§ º¯°æ
	/////////////////////////////////////////////

	BOOL    Check_TMH07_LoadCase(const EN_TMH07_NO& enLcomNo);

	CString Add_CvlTMH07_Title(const EN_TMH07_NO& enLcomNo, BOOL bStrength);

	int Get_TMH07_LCOM_List(CArray<TMH07_LCOM, TMH07_LCOM&>& raLcomList);

	void Set_CvlTMH07(const EN_TMH07_NO& enLcomNo,
		ArINT& arDLoad, ArINT& arDwLoad, ArINT& arDcLoad, ArINT& arStlLoad, ArUNT& arSmLoad, ArINT& arEvLoad, ArINT& arEhLoad,
		ArINT& arCrLoad, ArINT& arShLoad, ArINT& arPsLoad, ArUNT& arMLoad, ArINT& arCfLoad, ArINT& arBrkLoad, ArINT& arCoLoad,
		ArINT& arErLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arWLoad, ArINT& arFrLoad, ArINT& arCSLoad,
		BOOL bStrength = FALSE, BOOL bMoving = FALSE, BOOL bWind = FALSE);

	void Set_CvlTMH07EP_WP(const EN_TMH07_NO& enLcomNo, int m_nCodeBasedOrUserDef, int m_nCodeBasedVerticalEarthPressure, int m_nCodeBasedNonVerticalEarthPressure, double m_dUserDefDL, double m_dUserDefSIDL, double m_dEPRelieving,
		ArINT& arDLoad,  ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCSADLoad, ArINT& arCSADcLoad, ArINT& arCSADwLoad, ArINT& arPLLoad,  ArINT& arBLLoad, ArINT& arStlLoad,
		ArINT& arEsLoad, ArINT& arEpLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad,  ArINT& arWpLoad,  ArINT& arFpLoad, 
		ArINT& arLLoad,  ArINT& arLsLoad, ArINT& arCrLoad, ArINT& arErLoad, ArINT& arLIPLoad,ArINT& arCfLoad,  ArINT& arBrkLoad,
		ArINT& arBkLoad, ArINT& arCoLoad, ArINT& arCtLoad, ArINT& arWlLoad, ArINT& arWLoad,  ArINT& arSfLoad,
		ArINT& arWprLoad, ArINT& arELoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arTLoad,  ArINT& arTpgLoad,
		ArINT& arFrLoad, ArINT& arCSLoad, ArINT& arCrlLoad,ArUNT& arMLoad,  ArUNT &arSmLoad ,BOOL bStrength = FALSE, BOOL bMoving = FALSE, BOOL bWind = FALSE);

	void Set_CvlTMH07EP_WP_T_TPG(const EN_TMH07_NO& enLcomNo, int m_nCodeBasedOrUserDef, int m_nCodeBasedVerticalEarthPressure, int m_nCodeBasedNonVerticalEarthPressure, double m_dUserDefDL, double m_dUserDefSIDL, double m_dEPRelieving,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCSADLoad, ArINT& arCSADcLoad, ArINT& arCSADwLoad, ArINT& arPLLoad, ArINT& arBLLoad, ArINT& arStlLoad,
		ArINT& arEsLoad, ArINT& arEpLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
		ArINT& arLLoad, ArINT& arLsLoad, ArINT& arCrLoad, ArINT& arErLoad, ArINT& arLIPLoad, ArINT& arCfLoad, ArINT& arBrkLoad,
		ArINT& arBkLoad, ArINT& arCoLoad, ArINT& arCtLoad, ArINT& arWlLoad, ArINT& arWLoad, ArINT& arSfLoad,
		ArINT& arWprLoad, ArINT& arELoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arTLoad, ArINT& arTpgLoad,
		ArINT& arFrLoad, ArINT& arCSLoad, ArINT& arCrlLoad, ArINT& arASLoad, ArUNT& arMLoad, ArUNT &arSmLoad, BOOL bStrength = FALSE, BOOL bMoving = FALSE, BOOL bWind = FALSE);

	void Set_CvlTMH07_Seis(const EN_TMH07_NO& enLcomNo, int m_nCodeBasedOrUserDef, int m_nCodeBasedVerticalEarthPressure, int m_nCodeBasedNonVerticalEarthPressure, 
		double m_dUserDefDL_Max, double m_dUserDefDL_Min, double m_dUserDefSIDL_Max, double m_dUserDefSIDL_Min, double m_dEPRelieving,int m_dUserRdo_Dmax,int m_dUserRdo_Dmin,int m_dUserRdo_Dboth,int m_dUserRdo_SIDLmax,int m_dUserRdo_SIDLmin,int m_dUserRdo_SIDLboth,
		ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCSADLoad, ArINT& arCSADcLoad, ArINT& arCSADwLoad, ArINT& arPLLoad, ArINT& arBLLoad, ArINT& arStlLoad, 
		ArINT& arEsLoad, ArINT& arEpLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad, 
		ArINT& arLLoad, ArINT& arLsLoad, ArINT& arCrLoad, ArINT& arErLoad, ArINT& arLIPLoad, ArINT& arCfLoad, ArINT& arBrkLoad, 
		ArINT& arBkLoad, ArINT& arCoLoad, ArINT& arCtLoad, ArINT& arWlLoad, ArINT& arWLoad, ArINT& arSfLoad, 
		ArINT& arWprLoad, ArINT& arELoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arTLoad, ArINT& arTpgLoad,   
		ArINT& arFrLoad, ArINT& arCSLoad, ArINT& arCrlLoad, ArINT& arASLoad, ArINT& arRsLoad, ArUNT& arEspLoad, 
		ArUNT& arMLoad, ArUNT& arSmLoad, BOOL bStrength, BOOL bMoving, BOOL bWind, BOOL bSeis);

	void Add_CvlTMH07_EQ(const EN_TMH07_NO& enLcomNo, ArINT& arELoad, ArUNT& arEspLoad, ArINT& arCSLoad,CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bStrength,double& dGf3ULS,double& dGf3SLS);

	void Add_CvlTMH07_EQ_Ortho(const EN_TMH07_NO& enLcomNo, ArINT& arELoad, ArUNT& arEspLoad, CString& csPrefix_Tmp, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, BOOL bStrength, double& dGf3ULS, double& dGf3SLS);

	
	//void Set_CvlTMH07_Seis(int eql, const EN_TMH07_NO& enLcomNo, int m_nCodeBasedOrUserDef, int m_nCodeBasedVerticalEarthPressure, int m_nCodeBasedNonVerticalEarthPressure, double m_dUserDefDL, double m_dUserDefSIDL, double m_dEPRelieving,
	//	ArINT& arDLoad, ArINT& arDcLoad, ArINT& arDwLoad, ArINT& arCSADLoad, ArINT& arCSADcLoad, ArINT& arCSADwLoad, ArINT& arPLLoad, ArINT& arBLLoad, ArINT& arStlLoad,
	//	ArINT& arEsLoad, ArINT& arEpLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arBLoad, ArINT& arWpLoad, ArINT& arFpLoad,
	//	ArINT& arLLoad, ArINT& arLsLoad, ArINT& arCrLoad, ArINT& arErLoad, ArINT& arLIPLoad, ArINT& arCfLoad, ArINT& arBrkLoad,
	//	ArINT& arBkLoad, ArINT& arCoLoad, ArINT& arCtLoad, ArINT& arWlLoad, ArINT& arWLoad, ArINT& arSfLoad,
	//	ArINT& arWprLoad, ArINT& arELoad, ArINT& arShLoad, ArINT& arPsLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	//	ArINT& arFrLoad, ArINT& arCSLoad, ArINT& arCrlLoad, ArINT& arASLoad, ArINT& arELoad_x, ArINT& arELoad_y, ArINT& arELoad_z, ArINT& arRsLoad,
	//	ArUNT& arMLoad, ArUNT &arSmLoad, ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, BOOL bStrength = FALSE, BOOL bMoving = FALSE, BOOL bWind = FALSE);


	
	//BOOL Set_CvlTMH07_Seis_Incl_Therm_SS(ArINT &arDLoad, ArINT &arDdLoad, ArINT &arDwLoad, ArINT &arEpLoad, ArINT &arEsLoad, ArINT &arElLoad, ArINT &arEvLoad,
	//	ArINT &arCrLoad, ArINT &arShLoad, ArUNT &arLeadingMLoad, ArINT &arLsLoad, ArINT &arLLoad, ArINT &arLpLoad, ArINT &arIlLoad, ArINT &arErLoad,
	//	ArINT &arIlpLoad, ArINT &arCfLoad, ArINT &arBrkLoad, ArINT &arBkLoad, ArINT &arCrlLoad, ArINT &arPsLoad, ArINT &arWLoad, ArINT &arWlLoad, ArUNT &arSmLoad,
	//	ArINT &arCSLoad, ArINT& arRsLoad, ArUNT& arEspLoad_X, ArUNT& arEspLoad_Y, ArUNT& arEspLoad_Z, ArUNT &arMLoad, ArINT &arThermal, ArINT& arTLoad, ArINT& arWprLoad, ArINT& arBLoad, ArINT& arWpLoad,
	//	ArINT& arELoad_x, ArINT& arELoad_y, ArINT& arELoad_z, ArUNT &arPsi1FactorMLoad1, ArUNT &arPsi1FactorMLoad2);//Traffic Leading + No Wind

	
	void Add_CvlTMH07_Title(const EN_TMH07_NO& enLcomNo, BOOL bStrength, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_D(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arDLoad, double& dD1, double& dD2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_DW(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arDwLoad, double& dDw1, double& dDw2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_DC(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arDcLoad, double& dDc1, double& dDc2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_EV(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arEvLoad, double& dEv1, double& dEv2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS);
	void Add_CvlTMH07_EH(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arEhLoad, double& dEh1, double& dEh2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_ES(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arEsLoad, double& dEs1, double& dEs2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,int& m_nCodeBasedVerticalEarthPressure, double& dGf3ULS, double& dGf3SLS);
	void Add_CvlTMH07_LS(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arLsLoad, ArINT& arLLoad, ArUNT& arCombinedLoad, double& dMFactor, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CRL(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCrlLoad, double& dCrl1, double& dCrl2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,double& dGf3ULS,double& dGf3SLS);
	void Add_CvlTMH07_B(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arBLoad, double& dB1, double& dB2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& m_dEPRelieving,double& dGf3ULS, double& dGf3SLS);

	void Add_CvlTMH07_STL_SM(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arStlLoad, ArUNT& arSmLoad, double& dStl, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_PS_CR_SH(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arPsLoad, ArINT& arCrLoad, ArINT& arShLoad, ArINT& arCSLoad, double& dPs, double& dCRSH, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS);
	void Add_CvlTMH07_M(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArUNT& arMLoad, ArINT& arLLoad, ArUNT& arCombinedLoad, double& dMFactor, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_W(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arWLoad, double& dWFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& dGf3ULS, double& dGf3SLS);
	void Add_CvlTMH07_T_TPG(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad, ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_T(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad, ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_TPG(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arTLoad, ArINT& arTpgLoad, double& dTFactor, double& dTpgFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_FA(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arSfLoad, double& dTFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CF(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCfLoad, double& dTFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CO(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCoLoad, double& dCoFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_AS(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arASLoad, double& dCoFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_BRK(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arBrkLoad, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_ER(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arErLoad, double& dFrFactor, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);

	void Add_CvlTMH07_CS_D(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dD1, double& dD2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_DW(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dDw1, double& dDw2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_DC(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dDc1, double& dDc2, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_W(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dErFac, double& dGf3ULS, double& dGf3SLS, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_ER_LL(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dTFac, double& dTpgFac, double& dErFac, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_EP(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dEpFac, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS,int& m_nCodeBasedNonVerticalEarthPressure);
	void Add_CvlTMH07_CS_TPG(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dErFac, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_T(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dErFac, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_PSCRSH(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dPs, double& dCRSH, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom);
	void Add_CvlTMH07_CS_TP(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dPs, CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom,double& m_dEPRelieving, double& dGf3ULS, double& dGf3SLS);
	void Add_CvlTMH07_CS_EQ(const EN_TMH07_NO& enLcomNo, BOOL bStrength, ArINT& arCSLoad, double& dEqFactor,CArray<T_LCOM_D, T_LCOM_D&>& raNewLcom, double& dGf3ULS, double& dGf3SLS);

	void GetBD_21FactorTMH(
		CString& sD1Fac, CString& sD2Fac, CString& sDw1Fac, CString& sDw2Fac, CString& sDc1Fac, CString& sDc2Fac,
		CString& sEv1Fac, CString& sEv2Fac, CString& sEh1Fac, CString& sEh2Fac, CString& sEs1Fac, CString& sEs2Fac, CString& sCrl1Fac, CString& sCrl2Fac, CString& sGf3ULSFac, CString& sGf3SLSFac,
		double& dD1, double& dD2, double& dDw1, double& dDw2, double& dDc1, double& dDc2,
		double& dEv1, double& dEv2, double& dEh1, double& dEh2, double& dEs1, double& dEs2, double& dB1, double& dB2, double& dCrl1, double& dCrl2, double& dGf3ULS, double& dGf3SLS, BOOL b1stGrp, int& m_nCodeBasedVerticalEarthPressure,int& m_nCodeBasedNonVerticalEarthPressure,int& m_nCodeBasedOrUserDef,double& m_dUserDefDL_Max,double& m_dUserDefDL_Min,double& m_dUserDefSIDL_Max,double& m_dUserDefSIDL_Min,int& m_dUserRdo_Dmax,int& m_dUserRdo_Dmin,int& m_dUserRdo_Dboth, int& m_dUserRdo_SIDLmax, int& m_dUserRdo_SIDLmin, int& m_dUserRdo_SIDLboth, const EN_TMH07_NO& enLcomNo);

	void GetBD_21FactoredPermTMH(
		double& dD1, double& dD2, double& dDw1, double& dDw2, double& dDc1, double& dDc2,
		double& dEv1, double& dEv2, double& dEh1, double& dEh2, double& dGf);


	// Civil Load Combination
	void Set_LoadComb4Civil(T_LCOM_D& rData);
	void Set_LoadComb4CivilName(T_LCOM_D& rData, int nStrSer);  // ÀÌ¸§À» ³Ñ°Ü ¹ÞÀº ÇÏÁßÁ¶ÇÕ »ý¼º
	void Set_LoadComb4Civil(T_LCOM_D& rData, int nStrSer, int nType=0, int nMsgType=0);   // Envelope Load CombinationÀ» À§ÇÑ ÇÔ¼ö
	BOOL GetAdditionalMsg(CString &strMsg, int nMsgType);
	void Set_LoadComb4CivilNew(T_LCOM_D& rData, int nStrSer, int nType=0, int nMsgType=0);   // same to Set_LoadComb4Civil,but nType is useful.

	void RemoveSameLoadComb();
	BOOL IsSameLoadComb(T_LCOM_D& rData1, T_LCOM_D& rData2);
	CString GetNumberPostfix(CString str);

	void SetData(CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&>& mLcomPsiFactor)
	{
		m_mLcomPsiFactor.RemoveAll();
		UINT StldK;
		T_LPSI_D LpsiD;
		POSITION Pos = mLcomPsiFactor.GetStartPosition();
		while(Pos)
		{
			LpsiD.Initialize();
			mLcomPsiFactor.GetNextAssoc(Pos, StldK, LpsiD);
			m_mLcomPsiFactor.SetAt(StldK, LpsiD);
		}
	}

private:
	BOOL ExistInNewLcom(T_LCOM_D& LcomD, BOOL bChkDescription=FALSE);

protected:
	CMap<CString, LPCTSTR, int, int> m_mapLcom;   //Civil¿¡¼­ °°Àº ÀÌ¸§À» °É·¯ÁÙ¶§ ¾²ÀÓ
	int m_nDCount;  
	int m_nDcCount;  
	int m_nDdCount;  
	int m_nDwCount;  
	int m_nCSADCount;  
	int m_nCSADcCount;  
	int m_nCSADwCount;  
	int m_nEpCount;
	int m_nEhCount;
	int m_nEvCount;
	int m_nEsCount;
	int m_nElCount;
	int m_nLCount;  
	int m_nIlCount; 
	int m_nLcCount;    // China  ÆòÆÇ°úÂ÷ ¶Ç´Â ¿©´ëÂ÷¿¡ ÀÇÇÑ Live Load
	int m_nMsCount;    // China  ÆòÆÇ°úÂ÷ ¶Ç´Â ¿©´ëÂ÷¿¡ ÀÇÇÑ Moving Load
	int m_nLscCount;   // China  ÆòÆÇ°úÂ÷ ¶Ç´Â ¿©´ëÂ÷¿¡ ÀÇÇÑ »óÀçÈ°ÇÏÁß
	int m_nLsCount;    // China, AASHTO LRFD98  »óÀçÈ°ÇÏÁß
	int m_nLpCount;    // AASHTO  °úÀûÈ°ÇÏÁß
	int m_nIlpCount;   // AASHTO  °úÀûÈ°ÇÏÁß¿¡ ÀÇÇÑ Ãæ°ÝÇÏÁß
	int m_nWCount;  
	int m_nWlCount; 
	int m_nBrkCount; 
	int m_nECount;  
	int m_nCfCount; 
	int m_nCoCount; 
	int m_nCvCount;    // China, AASHTO LRFD98, ¼±¹ÚÃæµ¹ÇÏÁß
	int m_nCtCount; 
	int m_nStlCount;
	int m_nCrCount; 
	int m_nShCount; 
	int m_nTCount;  
	int m_nTpgCount;
	int m_nTgCount;  
	int m_nTlCount;  
	int m_nBCount;  
	int m_nWpCount;
	int m_nFpCount;
	int m_nSfCount;
	int m_nWprCount;
	int m_nPsCount;
	int m_nSCount; 
	int m_nErCount;
	int m_nCrlCount;   // China  ±ºÁßÇÏÁß
	int m_nIpCount;    // China  ºù¾Ð·Â, AASHTO
	int m_nFrCount;    // China, AASHTO LRFD98 ¸¶Âû·Â 
	int m_nRsCount;    // AASHTO  Rib Shortening
	int m_nBkCount;    // AASHTO  Longitudinal force from live load
	int m_nP1Num;
	int m_nP2Num;
	int m_nPPNum;    
	int m_nEspCount;
	int m_nMCount;
	int m_nMCrlCount;
	int m_nSmCount;
	int m_nGeCount;
	int m_nLeadingMCount;
	int m_nAccompanyMCount;
	int m_nThermalCount;
	int m_nPsi1FactorMLoad1;
	int m_nPsi1FactorMLoad2;
	int m_nEANNCount;     //Polish
	int m_nEANCCount;     //Polish
	int m_nEAMNCount;     //Polish
	int m_nEAMCCount;     //Polish
	int m_nEPNNCount;     //Polish
	int m_nEPNCCount;     //Polish
	int m_nEPMNCount;     //Polish
	int m_nEPMCCount;     //Polish

	int m_nMFCount;
	int m_nMSCount;

	int m_nCSCount;   // Construction Stage Load

	int m_nGDCount;    
	int m_nGDeCount; 
	int m_nGLvCount; 
	int m_nGCrlCount;
	int m_nGLeCount; 
	int m_nShvCount;

	int m_nLIPCount;
	int m_nPLCount;
	int m_nBLCount;

	int m_nDrlCount;  //India IRS Derailment Loads
    int m_nLFCount;  //INDIA IRS CBC
    int m_nRFCount;  //INDIA IRS CBC
    int m_nASCount;  //South African Code TMH7

	int m_nRepeat;     // AASHTO  1: Service Load DesignÀÇ Earth Pressure TypeÀÌ 0 
	//         2: Service Load DesignÀÇ Earth Pressure TypeÀÌ 1   
	//  int m_nChStrSer;   // China  Strength(0), Service(1) ±¸º°ÇÏ´Â º¯¼ö(°°Àº ÇÔ¼ö¸¦ »ç¿ëÇÏ±â À§ÇÔ)
	BOOL m_bAASHTO_LRFD16;  //AASHTO-LRFD16 Flag
    BOOL m_bAASHTO_LRFD18;  //AASHTO-LRFD17 Flag
	BOOL m_bAASHTO_LRFD20;  //AASHTO-LRFD20 Flag
	BOOL m_bAASHTO_LRFD24;  //AASHTO-LRFD24 Flag
	BOOL m_bChinaEnvelope;  // ChinaÀÇ Envelope¸¦ À§ÇÑ º¯¼ö

	CString m_strCSEL_LdType[D_STCT_EREC_MAX_CVL];  // Construction Stage Erection LoadÀÇ Load Type
	int m_CSEL_SgldK[D_STCT_EREC_MAX_CVL];
	CString m_CSEL_Sgld_Desc[D_STCT_EREC_MAX_CVL];

	BOOL CheckCSEL(int nErection, CString strLoadType);
	CString GetFactorString(double dF);
	BOOL CheckCS_SgldK(T_SGLD_K SrcK, int& n);
	CString GetJTGD602015MILoadName(const ArUNT& arMLoad, int m) const;
	CArray<int, int> m_aCSLoad;
public:
	CDBDoc* m_pDoc;
	BOOL m_bCurvedBridge;  // China : curved bridge
	int  m_nSerEpType;     // AASHTO : Service Load Design - Earth Pressure Type  
	int  m_nStrEpType;     // AASHTO : Load Factor Design - Earth Pressure Type(Culverts)
	int  m_nStrDlType;     // AASHTO : Load Factor Design - Dead Load Type
	double m_dCulvertH;    // AASHTO : Service Load Design - Lateral Earth Pressure Factor for Culverts
	double m_dCulvertV;    // AASHTO : Service Load Design - Vertical Earth Pressure Factor for Culverts
	double m_dAshLr98Modifier;   
	BOOL   m_bAshLr98CulvertChk;
	double m_dAshLr98Stl;  
	double m_dAshLr98Dc[2];  
	double m_dAshLr98Dd[2];
	double m_dAshLr98Dw[2];
	double m_dAshLr98Eh[2];
	double m_dAshLr98El[2];
	double m_dAshLr98Ev[2];
	double m_dAshLr98Es[2];
	int  m_nAshLr98DcNum;  
	int  m_nAshLr98DdNum;
	int  m_nAshLr98DwNum;
	int  m_nAshLr98EhNum;
	int  m_nAshLr98ElNum;
	int  m_nAshLr98EvNum;
	int  m_nAshLr98EsNum;
	int  m_nAshLr98Deform;

	double m_dAshLr08Modifier;  
	BOOL   m_bAshLr08CulvertChk;
	double m_dAshLr08Stl;  
	double m_dAshLr08Dc[2];  
	double m_dAshLr08Dd[2];
	double m_dAshLr08Dw[2];
	double m_dAshLr08Eh[2];
	double m_dAshLr08El[2];
	double m_dAshLr08Ev[2];
	double m_dAshLr08Es[2];
	double m_dAshLr08Ps[2];
	double m_dAshLr08Cr[2];
	double m_dAshLr08Sh[2];
	int  m_nAshLr08DcNum;  
	int  m_nAshLr08DdNum;
	int  m_nAshLr08DwNum;
	int  m_nAshLr08EhNum;
	int  m_nAshLr08ElNum;
	int  m_nAshLr08EvNum;
	int  m_nAshLr08EsNum;
	int  m_nAshLr08PsNum;
	int  m_nAshLr08CrNum;
	int  m_nAshLr08ShNum;
	int  m_nAshLr08Deform;

	BOOL m_bCSAnalysis;
	BOOL m_bLossFactor;
	double m_dLossFactorTransfer;
	double m_dLossFactorService;
	BOOL m_bJSCE02[7];
	BOOL m_bIrcServCond; // Service Condition
	BOOL m_bIrcConsCond; // Construction Condition
	BOOL m_bJTJD60Capc;
	BOOL m_bJTJD60Gen;
	BOOL m_bJTJD60Acdn;
	BOOL m_bJTJD60Serv;
	BOOL m_bJTJD60Elst;
	BOOL m_bTB05Main;//add by maxiao 2007-04-24
	BOOL m_bTB05MainAddition;
	BOOL m_bTB05MainSpecial;
	BOOL m_bTB10002_2017Main;
	BOOL m_bTB10002_2017MainAddition;
	BOOL m_bTB10002_2017MainSpecial;
	BOOL m_bCJJ166_2011Capc;//add by maxiao 2012-03-01
	BOOL m_bCJJ166_2011Gen;
	BOOL m_bCJJ166_2011Acdn;
	BOOL m_bCJJ166_2011Serv;
	BOOL m_bCJJ166_2011Elst;

	BOOL m_bJTG15Capc;//add by gongxing 2015-10-22 ³ÐÔØÄÜÁ¦
	BOOL m_bJTG15Gen;//»ù±¾×éºÏ
	BOOL m_bJTG15Acdn;//Å¼È»×éºÏ
	BOOL m_bJTG15Serv;//Õý³£Ê¹ÓÃ
	BOOL m_bJTG15Elst;//µ¯ÐÔ½×¶Î
	BOOL m_bJTG15_Freq;//ÆµÓöÖµ
	BOOL m_bJTG15_QuasiPerm;//×¼ÓÀ¾ÃÖµ
	BOOL m_bJTG15_FreqComb;//ÆµÓö×éºÏ
	BOOL m_bJTG15_QuasiPermComb;//×¼ÓÀ¾Ã×éºÏ
	BOOL m_bJTG15_StandComb;//±ê×¼×éºÏ
	BOOL m_bJTG15_Factor;//µ÷ÕûÏµÊý  IDC_CMD_LOADCOM_AUTO_FACTOR_EDT
	double m_dJTG15_Factor;//Ê¹ÓÃÄêÏÞµ÷ÕûÏµÊý
	BOOL m_bJTG15_PrestressLoss;    //¿¼ÂÇÔ¤Ó¦Á¦ºÉÔØ¹¤¿öËðÊ§
	BOOL m_bJTG15FoundationChk;//»ù´¡ÑéËã
	std::map<UINT,CString>m_mapJTGd6015MlLoad;//±ê¼ÇÒÆ¶¯ºÉÔØ¹¤¿ö¶ÔÓ¦µÄ³µÁ¾¡¢³µµÀÐÅÏ¢

	TLCOMESCGB19D m_tLcomESCGB19;//¹¤³Ì½á¹¹Í¨ÓÃ¹æ·¶

	int  m_nEC4Permanent;
	int  m_nEC4Prestress;
	int  m_nEC4Settlement;
	BOOL m_bEC4LVarTraff;
	BOOL m_bEC4LVarWind;
	BOOL m_bEC4LVarTherm;
	BOOL m_bEC4LVarSnow;
	BOOL m_bEC4LVarConst;
	int  m_nEC4LVarTraffType;

    BOOL m_bIS456_Cvl;  // IS456 gEN/cVL 
	BOOL m_bIRC6_LSDLVarTraff;
	BOOL m_bIRC6_LSDLVarWind;
	BOOL m_bIRC6_LSDLVarTherm;
	BOOL m_bIRC6_LSDLVarSnow;
	BOOL m_bIRC6_LSDLVarConst;
	BOOL m_bIRC6_IRC1122020;
	int  m_nIRC6_LSDLVarTraffType;


	CArray<T_MVLD_K, T_MVLD_K> m_aMvldChar;
	CArray<T_MVLD_K, T_MVLD_K> m_aMvldFreq;
	CArray<T_STLD_K, T_STLD_K> m_aIRSUlt;
	CArray<T_STLD_K, T_STLD_K> m_aIRSServ;
    CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPV;   //IRC 6 Special Vehicles
    CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPVSel;//IRC 6 Special Vehicles
	double m_dIRSUlt;
	double m_dIRSServ;
	BOOL   m_bIRSCons_crp_shr_DT_ULS;
	// Coded by Seungjun MNet:No.2393 ('20060919)
	double    m_dEC2LLPsi0;
	double    m_dEC2LLPsi1;
	double    m_dEC2LLPsi2;
	double    m_dEC2WLPsi0;
	double    m_dEC2WLPsi1;
	double    m_dEC2WLPsi2;
	// Coded by Seungjun ('20070719) MNet:No.2914.
	double    m_dEC2SLPsi0;
	double    m_dEC2SLPsi1;
	double    m_dEC2SLPsi2;
	// MNET:4820-SHJUNG-20140213
	double    m_dEC2TLPsi0;
	double    m_dEC2TLPsi1;
	double    m_dEC2TLPsi2;
	// MNET:4821-SHJUNG-20140221
	CMapEx<UINT, UINT, T_LPSI_D, T_LPSI_D&> m_mLcomPsiFactor;
	// Coded by Seungjun ('20070725) MNet:No.2914.
	double    m_dGamma_G;
	double    m_dGamma_Q;
	// Coded by Seungjun ('20071204) MNet:No.3095.
	double    m_dAlpha_H;
	// Add by GAY. PMS:4267. ('11.10.14).
	double    m_dGamma_L;  // È°ÇÏÁß Á¶Á¤°è¼ö.
	// Add by GAY. PMS:4267. ('11.12.08). Áß±¹½Å±âÁØ Ãß°¡.
	BOOL      m_bVertSeisAsMain; // ¼öÁ÷ÁöÁøÇÏÁßÀ» ÁÖÁöÁøÇÏÁßÀ¸·Î °í·Á.
	// Add by GAY. PMS:XXXX. ('12.03.21). Add ACI318-11.
	BOOL      m_bLateralSoil;
	double    m_dLateralSoilFactor;
	int       m_nWindLoadLevel; // 0=Strength, 1=Service.
	int       m_nSeisLoadLevel; // 0=Strenght, 1=Service.
	int       m_nStructType;	// 0=Group A,  1=Group B
	int       m_nLoadCode;
	//Add by tss(2019/09)
	double    m_dPhi_f;
	double    m_dPhi_q;
	CString   m_strFireCase;

	// KSCE-LSD12
	double m_dKoreaLr11Modifier;   
	BOOL   m_bKoreaLr11CulvertChk;
	double m_dKoreaLr11Stl;  
	double m_dKoreaLr11Dc[2];  
	double m_dKoreaLr11Dd[2];
	double m_dKoreaLr11Dw[2];
	double m_dKoreaLr11Eh[2];
	double m_dKoreaLr11El[2];
	double m_dKoreaLr11Ev[2];
	double m_dKoreaLr11Es[2];
	double m_dKoreaLr11Ps[2];
	double m_dKoreaLr11Cr[2];
	double m_dKoreaLr11Sh[2];
	int  m_nKoreaLr11DcNum;  
	int  m_nKoreaLr11DdNum;
	int  m_nKoreaLr11DwNum;
	int  m_nKoreaLr11EhNum;
	int  m_nKoreaLr11ElNum;
	int  m_nKoreaLr11EvNum;
	int  m_nKoreaLr11EsNum;
	int  m_nKoreaLr11PsNum;
	int  m_nKoreaLr11CrNum;
	int  m_nKoreaLr11ShNum;
	int  m_nKoreaLr11PSCRSHType;
	int  m_nKoreaLr11Deform;
	BOOL m_bLrfd11GravityLoadChk;
	double m_dLrfd11EffLoadFactor;

	// AASHTO LRFD12
	double m_dAshLr12Modifier;  
	BOOL   m_bAshLr12CulvertChk;
	double m_dAshLr12Stl;  
	double m_dAshLr12Dc[2];  
	double m_dAshLr12Dd[2];
	double m_dAshLr12Dw[2];
	double m_dAshLr12Eh[2];
	double m_dAshLr12El[2];
	double m_dAshLr12Ev[2];
	double m_dAshLr12Es[2];
	double m_dAshLr12Ps[2];
	double m_dAshLr12Cr[2];
	double m_dAshLr12Sh[2];
	int  m_nAshLr12DcType;
	int  m_nAshLr12DdType;
	int  m_nAshLr12DwType;
	int  m_nAshLr12EhType;
	int  m_nAshLr12ElType;
	int  m_nAshLr12EvType;
	int  m_nAshLr12EsType;
	int  m_nAshLr12PsType;
	int  m_nAshLr12CrType;
	int  m_nAshLr12ShType;
	int  m_nAshLr12Deform;

	// Imperfection Load
	CArray<_LCOM_IMPF, _LCOM_IMPF&> m_aImperfectionLoad;  

	/// data for auto-generation of seismic evaluation load combination.
	double m_dForcCtrlAct_C;
	double m_dForcCtrlAct_J;	
	CArray<T_STLD_K, T_STLD_K> m_aSelGravLoad;
	CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&> m_aEffectiveLoad;
	CArray<_LCOM_GRAV_EFF_MV, _LCOM_GRAV_EFF_MV&> m_aEffectiveMVLoad;
	CMapEx<T_STLD_K, T_STLD_K, _LCOM_GRAV_EFF, _LCOM_GRAV_EFF&> m_mapGravSeisEval;
	// 100:30 Load Case Data
	CArray<_LCOM_100_30_LC3, _LCOM_100_30_LC3&> m_a100_30_Load;

	//CMap<UINT, UINT, UINT, UINT> m_KoreaLr11DLData; // Fatigue - Dead Load Case
	UINT m_nKoreaLr11LL; // Fatigue - Live Load Case 
	CArray<UINT, UINT> m_aKoreaLr11DLData; // Fatigue - Dead Load Case

	BOOL m_bUlti1;
	BOOL m_bUlti2;
	BOOL m_bUlti3;
	BOOL m_bUlti4;
	BOOL m_bUlti5;
	BOOL m_bExtr1;
	BOOL m_bExtr2;
	BOOL m_bServ1;
	BOOL m_bServ2;
	BOOL m_bServ3;
	BOOL m_bServ4;
	BOOL m_bServ5;
	BOOL m_bFati1;

	// CSA-S6S1-10
	int m_nCSADNum;  
	int m_nCSADWNum;
	int m_nCSAEP1Num;
	int m_nCSAEP2Num;
	int m_nCSAEP3Num;
	int m_nCSAEP4Num;
	int m_nCSAWPNum;
	int m_nCSAPSNum;
	double m_dCSAD[2];  
	double m_dCSADW[2];
	double m_dCSAEP1[2];
	double m_dCSAEP2[2];
	double m_dCSAEP3[2];
	double m_dCSAEP4[2];
	double m_dCSAWP[2];	
	double m_dCSAPS[2];

	// SNiP 2.05.03-84*, SP 35.13330.2011
	int m_nBridgeType;
	int m_nSP3511DNum;
	int m_nSP3511LIPNum;
	int m_nSP3511PLNum;
	int m_nSP3511BLNum;
	int m_nSP3511PSNum;
	int m_nSP3511EPNum;
	int m_nSP3511WPNum;
	int m_nSP3511CRSHNum;
	int m_nSP3511STLNum;
	double m_dSP3511D[2];
	double m_dSP3511LIP[2];
	double m_dSP3511PL[2];
	double m_dSP3511BL[2];
	double m_dSP3511PS[2];
	double m_dSP3511EP[2];
	double m_dSP3511WP[2];
	double m_dSP3511CRSH[2];
	double m_dSP3511STL[2];

	//PN-85/S-10030
	int m_nPNS10030DistribType;
	int m_nPNS10030BridgeType;
	int m_nPNS10030DNum;     
	int m_nPNS10030DWNum;   
	int m_nPNS10030PSNum;   
	int m_nPNS10030WPNum;
	int m_nPNS10030DCNum;
	BOOL m_bPNS10030BaseType;
	BOOL m_bPNS10030AddType;
	BOOL m_bPNS10030UniqType;
	double m_dPNS10030D[2];
	double m_dPNS10030DC[2];
	double m_dPNS10030DW[2];
	double m_dPNS10030PS[2];
	double m_dPNS10030WP[2];

	// AS 5100-2017
	int m_nASBridgeType;
	int m_nAS5100DNum;
	int m_nAS5100DWNum;
	int m_nAS5100SLNum;
	int m_nAS5100GLNum;

	double m_dAS5100D[2];
	double m_dAS5100DW[2];
	double m_dAS5100SL[2];
	double m_dAS5100GL[2];

    // BD 21 -> BS 5400 À¸·Î ÀÌ¸§ º¯°æ
    int m_nBD21BridgeType;
    int m_nBD21DNum;
    int m_nBD21DWNum;
    int m_nBD21DCNum;
    int m_nBD21EVNum;
    int m_nBD21EHNum;
	int m_nBD21ESNum;
	int m_nBD21BNum;

    double m_dBD21D[2];
    double m_dBD21DW[2];
    double m_dBD21DC[2];
    double m_dBD21EV[2];
    double m_dBD21EH[2];
	double m_dBD21ES[2];

    double m_dBD21MVFLULS;
    double m_dBD21MVFLSLS;

    int m_nBDMovingType;

    double m_dBD21GF3ULS;
    double m_dBD21GF3SLS;

	//TMH7
	int m_nCodeBasedOrUserDef;//0-Code Based;1-User Defined
	int m_nCodeBasedVerticalEarthPressure;//0-Method 1 ; 1 - Method 2& 3
	int m_nCodeBasedNonVerticalEarthPressure;//0-Approximate ; 1 - Accurate
	double m_dUserDefDL;//Max,Min,Both
	double m_dUserDefDL_Max;
	double m_dUserDefDL_Min;
	double m_dUserDefSIDL;//Max,Min,Both
	double m_dUserDefSIDL_Max;//Max,Min,Both
	double m_dUserDefSIDL_Min;//Max,Min,Both
	double m_dEPRelieving;//From Input Dlg
	int m_dUserRdo_Dmax;
	int m_dUserRdo_Dmin;
	int m_dUserRdo_Dboth;
	int m_dUserRdo_SIDLmax;
	int m_dUserRdo_SIDLmin;
	int m_dUserRdo_SIDLboth;


	UINT m_nTMH7BrackingTractive; // Fatigue - Live Load Case 
	CMap<T_STLD_K, T_STLD_K, double, double> m_aTMH7BrackingTractiveData; // Bracking Tractive Data

	double m_dGamma_LL;   //AASHTO LRFD 2016 Load factor for live load

	CArray<T_STLD_K, T_STLD_K> m_aStldEp1;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp2;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp3;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp4;

	BOOL m_bSoilAsDL;
	BOOL m_bWatherAsLL;

	BOOL m_bLLRedunFactor;

private:   //CGXLongOperationÀ» À§ÇÑ º¯¼ö
	BOOL m_bShowStatus;
	CGXLongOperation *m_pLongOp;
	UINT m_nDone;
	UINT m_nPercentUnit;

	int  m_iLLDescript;
	int  m_nSameLcomId;

    //---------------------------------------------------------------
	// for Lcom Buffering (by hsshim)
private:		
	BOOL m_bIsLcomBuffOn;
	int  m_nTypeStrenService;
	CArray<T_LCOM_D,T_LCOM_D&> m_aLcomBuff;
	CArray<_LCOM_BOTH, _LCOM_BOTH&> m_aLcomBoth;

private:
	BOOL IsLcomBuffOn();
	void SetLcomBuffOn(BOOL bSet);
	void StartLcomBuff(int nStrSer);
	BOOL AddLcomBuff(T_LCOM_D& rData/*, int nStrSer*/);
	BOOL AddLcomBoth(_LCOM_BOTH& rData);
	void EndLcomBuff();

	void Set_LoadComb4CivilName_proxy(T_LCOM_D& rData, int nStrSer);  // ÀÌ¸§À» ³Ñ°Ü ¹ÞÀº ÇÏÁßÁ¶ÇÕ »ý¼º (proxy ÇÔ¼ö)
	int CompareLcomFactor(const T_LCOM_D& src, const _LCOM_BOTH& src_Both, const T_LCOM_D& tgt, const _LCOM_BOTH& tgt_Both);	// 

	//---------------------------------------------------------------

private:
	// PMS:4821 Seungjun-20140217 : ¿ëµµ¿¡ µû¸¥ psi factor Àû¿ëÇÏ¿© ÇÏÁßÁ¶ÇÕ »ý¼º
	void Get_EC_psi(const int& StldK, double& dpsi0, double& dpsi1, double& dpsi2);
	void Get_EC_psi_LL(const int& StldK, double& dFact1, double& dFact2, int nActive);
	double Get_EC_psi_TL(const int& StldK, const BOOL& bPsiT2, const int& nDescript, const int& nActive);
	double Get_EC_psi_SL(const int& StldK, const int& nPsiS);

private:
	int FilterPL_LoadCH(CArray<UINT,UINT>& aMvldKeyList, CString strCode);
	void MakeWindLoadGroupMapKBC2016(_LCOM_WIND_GROUP_MAP& rmWindGroup);	
	void Set_WindDirCombKBC2016();	
	void Set_LcomBase(UINT unAnalType, UINT LcaseK, double dFactor, T_LCOM_BASE& rLcomBase);
	BOOL AfterImperfectonLCom();  
	void Set_Each_LoadComb_Imperfection(T_LCOM_D& LComIn, BOOL& bSameLcom);
	void Set_Each_LoadComb_Imperfection_CH(T_LCOM_D& LComIn, BOOL& bSameLcom);

    void InitSrssLcomList();

private:
	void MakeGravLoadMapKISTEC(const CArray<T_STLD_K, T_STLD_K>& aGravLoad, const CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&>& aEffeLoad, OUT CMapEx<T_STLD_K,T_STLD_K,_LCOM_GRAV_EFF,_LCOM_GRAV_EFF&>& rmapGravLoad);

};

#include "HeaderPost.h"

#endif // !defined(AFX_LOADCOMBCTRL_H__D5846021_0CC5_11D4_888F_0000C0F30D4D__INCLUDED_)

//3 copy end

