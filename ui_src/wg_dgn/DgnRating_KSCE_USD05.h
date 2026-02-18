// DgnBridgeLoadRating.h: interface for the CDgnKSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNBRATING_KSCE_USD05_H__INCLUDED_)
#define AFX_DGNBRATING_KSCE_USD05_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DgnPscCommon.h"
#include "DgnKSCE_USD05.h"

class CDgnRating_KSCE_USD05 : public CDgnKSCE_USD05
{
// Member Functions.
public:
	CDgnRating_KSCE_USD05();
	virtual ~CDgnRating_KSCE_USD05();
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);  
	void SetDataCtrlPointer4Rating(int nCodePSC, CCRCDataCtrl* pDataCtrl);

public:
	BOOL Get_RatingAlwStress(ElemPairK ElemK, BOOL bCompoSect, T_RKST_CASE& RkstCaseD);
	BOOL Get_RatingFlexuralStrength(ElemPairK ElemK, BOOL bCompoSect, T_RAKR_D& RakrD, T_RKMC_CASE& RkmcCaseD);
	BOOL Get_RatingFlexuralStrength_Print(CMSExcel* pXL, ElemPairK ElemK, int nIJ, BOOL bCompoSect, CString strTilteName, T_RAKR_D& RakrD, T_RKMC_CASE& RkmcCaseD);

protected:
	BOOL m_bTestMode;

	BOOL Calc_AllowbleStressMethod(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn,
																 T_RKST_BASE& RkstBaseD);
	//BOOL Calc_FlexuralStrengthMethod(int nSurveyMeth, double dpMn, _PSC_MATL_ELEM& MatlD, T_RKMC_BASE& RkmcBase);

};

#endif // !defined(AFX_DGNBRATING_KSCE_USD05_H__INCLUDED_)
