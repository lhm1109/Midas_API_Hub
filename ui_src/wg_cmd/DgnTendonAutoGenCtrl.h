#pragma once

#include <vector>
#include <map>

#include "..\wg_db\AttrCtrl.h"
#include "DgnTendonAutoGenCtrlDef.h"


struct TndnTemplate;
class CDgnTendonTemplateCtrl;
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnTendonAutoGenCtrl
{
public:
	CDgnTendonAutoGenCtrl(CAttrCtrl* pAttrCtrl, CDgnTendonTemplateCtrl*	pTemplateCtrl);
	~CDgnTendonAutoGenCtrl(void);

	BOOL GetSectTypeName(T_SECT_K tSectKey, CString& strCode, CString& strType, CString& strName);
	virtual BOOL AutoGeneration(T_SECT_K tSectKey, T_TDNT_K tdntKey, T_TDGR_K tdgrKey, const CString& namePrefix, const T_AUTOTNDN_INFO& autoInfo);

	BOOL GetStringsCode(CStringArray*& pstrAry) { pstrAry = &m_aStrListCode; return TRUE; }
	long GetStringsType(const CString& strCode, CStringArray& strAry);
	long GetStringsName(const CString& strCode, const CString& strType, CStringArray& strAry);
	long GetStringsName_PSC_I(const CString& strCode, const CString& strType, CString& strDBName, CStringArray& strAry);

	void SetEpsilonUnit();

protected:
	//입력할 단면의 사이즈를 알아서 텐던이 찌그러지지 않게 offset값을 세팅한다.
	BOOL FindOffset(T_SECT_K tSectKey, T_AUTOTNDN_INFO& autoInfo);
	BOOL FindOffset_Custom(T_SECT_K tSectKey, T_AUTOTNDN_INFO& autoInfo);
	BOOL GetOffsect_PSC_I(CString& strDBName, T_SECT_K tSectKey, T_AUTOTNDN_INFO& autoInfo);

	//단면을 입력받아 해당단면의 code, dbname, dbnamesub등을 확인한다.... 이거 느린데... 어떻게 처리를 하지?
	// 1. 단면의 너비값등은 고정이므로 해당 값으로 먼저 필터링 한다...?
	BOOL AutoGenCheck( const T_SECT_D& tSectData, const CStringArray& aStrList, CString& strDBName, CString& strDBNameSub );
	BOOL AutoGenCheck_Custom( const T_SECT_D& tSectData, CString& strCode, CString& strType, CString& strName);
	BOOL AutoGenCheck_PSC_I(CString& strDBName, const T_SECT_D& tSectData, CString& strName);

	// 단면을 찾는 과정에서 폴리군, 사이즈, 등 검사 함수.
	BOOL IsSamePolygon(const T_GSEC_POLYGON& rPolygonOrg, const T_GSEC_POLYGON& rPolygonCmp);
	BOOL IsSameSectWithSize(int nSize, const double* dSect, const double* dComp);
	BOOL IsValidSect(const T_SECT_D& tSect);

	BOOL GetSectSize(T_SECT_D& tSectData, double& dHeight, double& dWidth, BOOL bDBSect = TRUE, BOOL bComposite = FALSE);
	BOOL GetStartIdxAndDir(  BOOL& bReverse, int& nIdxStart, double& dDiffX, double& dDiffY,
		const CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>& aVertexOrg,
		const CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>& aVertexComp );  

	//텐던 입력할때 사용성 편의를 위해 만든함수...
	void SetInit(unsigned int tdgrK, unsigned int tdntK, TndnTemplate* tndnInfo);
	void SetLine(double dH, double* dInterval, int nSize, TndnTemplate* tndnInfo, BOOL bSymm);
	void SetTempAry(int& nSize, double* dTempLen, double dFirst, ...);

	//가능한 텐던 자동생성 함수의 함수포인터와 아규먼트를 가져온다.
	pAutoGenFunc GetAutoGenFunc( const CString& strCode, const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg );
	pAutoGenFunc GetAutoGenFunc_UK( const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg );
	pAutoGenFunc GetAutoGenFunc_Italy(const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg);
	pAutoGenFunc GetAutoGenFunc_USA( const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg );
	pAutoGenFunc GetAutoGenFunc_AS( const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg );
	pAutoGenFunc GetAutoGenFunc_NZ( const CString& strName, const CString& strNameSub, T_AUTOTNDN_ARG& rArg );

	DEF_DECLARE_FUNCS(UK_SY    ); // AutoGenTndn_UK_SY    
	DEF_DECLARE_FUNCS(UK_SYE   ); // AutoGenTndn_UK_SYE   
	DEF_DECLARE_FUNCS(UK_TYE_BS); // AutoGenTndn_UK_TYE_BS
	DEF_DECLARE_FUNCS(UK_TY_BS ); // AutoGenTndn_UK_TY_BS 
	DEF_DECLARE_FUNCS(UK_YE    ); // AutoGenTndn_UK_YE    
	DEF_DECLARE_FUNCS(UK_Y     ); // AutoGenTndn_UK_Y     
	DEF_DECLARE_FUNCS(UK_M     ); // AutoGenTndn_UK_M     
	DEF_DECLARE_FUNCS(UK_UMB   ); // AutoGenTndn_UK_UMB   
	DEF_DECLARE_FUNCS(UK_MY    ); // AutoGenTndn_UK_MY    
	DEF_DECLARE_FUNCS(UK_MYE   ); // AutoGenTndn_UK_MYE   
	DEF_DECLARE_FUNCS(UK_U     ); // AutoGenTndn_UK_U     
	DEF_DECLARE_FUNCS(UK_SD    ); // AutoGenTndn_UK_SD    
	DEF_DECLARE_FUNCS(UK_W     ); // AutoGenTndn_UK_W     
	DEF_DECLARE_FUNCS(UK_T     ); // AutoGenTndn_UK_T     
	DEF_DECLARE_FUNCS(UK_U_SU  ); // AutoGenTndn_UK_U_SU  

	DEF_DECLARE_FUNCS(Italy_VH); // AutoGenTndn_Italy_VH

	DEF_DECLARE_FUNCS(USA_ODOT_B     ); // AutoGenTndn_USA_ODOT_B
	DEF_DECLARE_FUNCS(USA_ODOT_WF    ); // AutoGenTndn_USA_ODOT_WF
	DEF_DECLARE_FUNCS(USA_LADOT_LG	 ); // AutoGenTndn_USA_LADOT_LG
	DEF_DECLARE_FUNCS(USA_RIDOT_S	 ); // AutoGenTndn_USA_RIDOT_S
	DEF_DECLARE_FUNCS(USA_ILDOT_PPC  ); // AutoGenTndn_USA_ILDOT_PPC
	DEF_DECLARE_FUNCS(USA_ILDOT_BULB ); // AutoGenTndn_USA_ILDOT_BULB
	DEF_DECLARE_FUNCS(USA_ILDOT_IL   ); // AutoGenTndn_USA_ILDOT_IL
	DEF_DECLARE_FUNCS(USA_ILDOT_PD   ); // AutoGenTndn_USA_ILDOT_PD
	DEF_DECLARE_FUNCS(USA_massDOT_S  ); // AutoGenTndn_USA_massDOT_S
	DEF_DECLARE_FUNCS(USA_massDOT_NF ); // AutoGenTndn_USA_massDOT_NF
	DEF_DECLARE_FUNCS(USA_MoDOT_PSBXB); // AutoGenTndn_USA_MoDOT_PSBXB
	DEF_DECLARE_FUNCS(USA_TXDOT_B    ); // AutoGenTndn_USA_TXDOT_B
	DEF_DECLARE_FUNCS(USA_TXDOT_XB   ); // AutoGenTndn_USA_TXDOT_XB

	DEF_DECLARE_FUNCS(USA_ODOT_ASHT ); // AutoGenTndn_USA_ODOT_ASHT
	DEF_DECLARE_FUNCS(USA_ODOT_MASHT); // AutoGenTndn_USA_ODOT_MASHT

	DEF_DECLARE_FUNCS(AS_SUPER_T);         // AutoGenTndn_AS_SUPER_T
	DEF_DECLARE_FUNCS(AS_PLANK_GIRDER);    // AutoGenTndn_AS_PLANK_GIRDER
	DEF_DECLARE_FUNCS(AS_SUPER_T_RMS_2019);// AutoGenTndn_AS_SUPER_T_RMS_2019
	DEF_DECLARE_FUNCS(AS_PSC_Deck_Unit_TMR_QLD);// AutoGenTndn_AS_PSC_Deck_Unit_TMR_QLD

	DEF_DECLARE_FUNCS(NZ_SUPER_T);      // AutoGenTndn_NZ_SUPER_T
	DEF_DECLARE_FUNCS(NZ_I_GIRDER);     // AutoGenTndn_NZ_I_GIRDER
	DEF_DECLARE_FUNCS(NZ_HOLLOW_CORE);  // AutoGenTndn_NZ_HOLLOW_CORE
	DEF_DECLARE_FUNCS(NZ_SUPERHI);      // AutoGenTndn_NZ_SUPERHI

	DEF_DECLARE_FUNCS(TEST); // AutoGenTndn_TEST

protected:
	CAttrCtrl*	m_pAttrCtrl;  
	CDBDoc*     m_pDoc;

	CDgnTendonTemplateCtrl*     m_pTemplateCtrl;
	CStringArray                m_aStrListCode;
	CStringArray                m_aStrList[IDX_STR_LIST_SIZE];
	CArray<T_SECT_D, T_SECT_D&> m_raData;
	CArray<UINT, UINT>          m_raID;

	double   m_dCurUnitTolValu;
	double   m_dCurUnitTolSect;

	double   m_dSectSizeH;      //높이
	double   m_dSectSizeW;      //너비
	double   m_dSectSizeH_Org;  //적용할 단면의 너비.(텐던모양의 근거)
	double   m_dSectSizeW_Org;

	T_AUTOTNDN_INFO m_AutoInfo;

	T_TDNT_K m_keyTdnt;
	T_TDGR_K m_keyTdgr;
	CString	 m_strPrefix;
};
#include "HeaderPost.h"
