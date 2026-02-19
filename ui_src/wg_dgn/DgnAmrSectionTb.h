// DgnAmrSectionTb.h: interface for the CDgnAmrSectionTb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGNAMRSECTIONTB_H__)
#define __DGNAMRSECTIONTB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_DBDoc.h"

#define D_IMAGE_WIDTH   100
#define D_IMAGE_HEIGHT  100

#define D_SECTION_TB_SS 1   // Section Summary
#define D_SECTION_TB_RS 2   // Reinforcement of Sections
#define D_SECTION_TB_CS 3   // Composite Section for C.S.

struct T_SECTION_TB_UNIT_D
{
	double A, Asy, Asz, zp, zm, Ixx, Iyy, Izz, yp, ym;
	double EsEc, GsGc, DsDc, Ps, Pc;

	T_SECTION_TB_UNIT_D()
	{
		Initialize();
	}
	void Initialize()
	{
		A     = 0.;
		Asy   = 0.;
		Asz   = 0.;
		zp    = 0.;
		zm    = 0.; 
		Ixx   = 0.;
		Iyy   = 0.;
		Izz   = 0.;
		yp    = 0.;
		ym    = 0.;
		EsEc  = 0.;
		GsGc  = 0.;
		DsDc  = 0.;
		Ps    = 0.;
		Pc    = 0.;
	}
};

struct T_SECTION_TB_D
{
	int nType;
	T_SECTION_TB_UNIT_D Unit1;
	T_SECTION_TB_UNIT_D Unit2;
	
	T_SECTION_TB_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nType = -1;
		Unit1.Initialize();
		Unit2.Initialize();
	}
};

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnAmrSectionTb  
{
public:
	CDgnAmrSectionTb();
	virtual ~CDgnAmrSectionTb();

	//void SetSectTbData(UINT SectK);
	BOOL GenerateSectionTable(CString strFilePathName, CStringArray& arImagePath, int nTableType, UINT SectK, UINT StagK=0);
	
	BOOL GenerateSectionSum(CString strFilePathName, CStringArray& arImagePath);  // Section Summary
	BOOL GenerateType1(CString strFilePathName, CStringArray& arImagePath); // type 1: DB/User/Value(형상정보있음), type 3: PSC/SPC
	BOOL GenerateType2(CString strFilePathName);                            // type 2: Value(형상정보없음)
	BOOL GenerateType4(CString strFilePathName, CStringArray& arImagePath); // type 4: SRC
	BOOL GenerateType5(CString strFilePathName, CStringArray& arImagePath); // type 5: composite
	BOOL GenerateType6(CString strFilePathName, CStringArray& arImagePath); // type 6: Tapered
	BOOL GenerateType7(CString strFilePathName, CStringArray& arImagePath); // type 7: Tapered Composite
	
	BOOL GenerateSectionRein(CString strFilePathName, CStringArray& arImagePath, BOOL bConsiderJ); // Reinforcement of Section
	BOOL GenerateReinI (CString strFilePathName, CStringArray& arImagePath);  // I단만
	BOOL GenerateReinIJ(CString strFilePathName, CStringArray& arImagePath);  // I, J단 모두

	BOOL GenerateSectionComp(CString strFilePathName, CStringArray& arImagePath, int nType, UINT SectK); // Composite Section for C.S.
	BOOL GenerateCompType1(CString strFilePathName, CStringArray& arImagePath, UINT SectK);
	BOOL GenerateCompType2(CString strFilePathName);

protected:
	// Section Summary
	T_RSCT_STIFF_D  m_StiffInfo;
	T_RSCT_STIFF_D  m_AfterStiffInfo;

	// Reinforcement of Section
	T_RSCT_RBAR_D   m_RpscInfoI;
	T_RSCT_RBAR_D   m_RpscInfoJ;

	// Composite Section for C.S.
	CArray<T_RSCT_CSCT_D, T_RSCT_CSCT_D&> m_aCscsInfo;

	// 단위
	CString m_strArea;
	CString m_strLeng;
	CString m_strStif;
	CString m_strArea2; // 괄호 없이 단위만.
	CString m_strLeng2;
	CString m_strStif2;

	int  m_nDecimal;
	BOOL m_bExp;
	CString Format_StringDouble(CString csStr, double dValue);
};
#include "HeaderPost.h"

#endif // !defined(__DGNAMRSECTIONTB_H__)
