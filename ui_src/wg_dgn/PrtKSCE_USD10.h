// PrtKSCE_USD10.h: interface for the CPrtKSCE_USD10 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTKSCE_USD10_H__INCLUDED_)
#define AFX_PRTKSCE_USD10_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\XLOut.h"
#include "PrtKSCE_USD05.h"

//#include "..\wg_base\MSExcel.h"
//#include "..\wg_base\XLParser.h"


class CPrtKSCE_USD10 : public CPrtKSCE_USD05
{

// Member Functions.
public:
	CPrtKSCE_USD10();
	virtual ~CPrtKSCE_USD10();


	//CPrtKSCE_USD10* Get_PrtKscePointer();


	void Print_Rating_Title(CString strTitle, CMSExcel* pXL);
	void Print_ElementIJ_KSCE10(T_ELEM_K ElemK, BOOL bPosiI, BOOL bPositive, CMSExcel* pXL);
	void Print_ElementIJ_Shear_KSCE10(T_ELEM_K ElemK, BOOL bPosiI, CMSExcel* pXL=NULL);
	void Print_ElementIJ_Tortion_KSCE10(T_ELEM_K ElemK, BOOL bPosiI, CMSExcel* pXL=NULL);


	void Print_BdcyDesign(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CMSExcel* pXL );
	void Print_CumsDesign(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CMSExcel* pXL=NULL);  
	void Print_CrmtDesign(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CMSExcel* pXL=NULL);  


	BOOL Print_BdcyDesign_Table(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce, int i, CMSExcel* pXL=NULL );
	BOOL Print_CumsDesign_Table(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE>& arCumsKsce, CMSExcel* pXL=NULL );

	

//  BOOL Get_DetailBdcyPos(T_ELEM_K ElemK);

	void Set_CMSExcel_KSCE10(CMSExcel* pXL, CString strBaseBook, int nDgnCode);

	void Set_CMSExcel_Flexure_theory(CMSExcel* pXL, int ifpsMethod);
	void Set_CMSExcel_Shear_theory(CMSExcel* pXL);
	void Set_CMSExcel_Torsion_theory(CMSExcel* pXL);

	void SetCompositeSect(BOOL bCompoSect) {m_bCompoSect = bCompoSect;};


	

	// Member Variables.
protected:
	CMSExcel*	 m_pXL; 
	CString m_strBaseBook;
	CString m_strChpt; 
	int m_nTotalRow;
	int m_nChapNum;

	double m_dFactorForce         ;
	double m_dFactorLength        ;
	double m_dFactorMoment        ;
	double m_dFactorInertiaMoment; 

	BOOL m_bCompoSect;

	void SetUnitConvertFactor();
	void Print_BdcyDesign0      (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign0_Comp (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code1 (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code2 (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code3 (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code4 (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code5 (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code6 (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code7 (T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign_Code10(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);

	void Print_BdcyDesign4(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign5(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign6(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);
	void Print_BdcyDesign8(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, CString strBaseSheet, int& stRow);

	void Print_CumsDesign0(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow);
	void Print_CumsDesign3(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow);
	void Print_CumsDesign4(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow);
	void Print_CumsDesign5(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow);
	void Print_CumsDesign6(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow);
	void Print_CumsDesign8(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow);

	void Print_CrmtDesign0(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);
	void Print_CrmtDesign3(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);
	void Print_CrmtDesign4(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);
	void Print_CrmtDesign6(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);
	void Print_CrmtDesign7(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);
	void Print_CrmtDesign8(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);

	void Print_CumsDesign4_torsion(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);
	void Print_CumsDesign5_torsion(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);
	void Print_CumsDesign6_torsion(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow);


};

#endif // !defined(AFX_PRTKSCE_USD10_H__INCLUDED_)
	