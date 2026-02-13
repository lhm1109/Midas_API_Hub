// DgnCsgDataCtrlAASHTO12.h: interface for the CDgnCsgDataCtrlAASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCsgDataCtrlKSCE_LSD15_H__INCLUDED_)
#define AFX_DGNCsgDataCtrlKSCE_LSD15_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnCsgDataCtrl.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCsgDataCtrlKSCE_LSD15 : public CDgnCsgDataCtrl
{
	// Member Functions.
public:
	CDgnCsgDataCtrlKSCE_LSD15();
	virtual ~CDgnCsgDataCtrlKSCE_LSD15();

	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);

	virtual BOOL MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData) override;

	virtual BOOL Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Strength        (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Service         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Fatigue         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR) override;

	virtual void GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy);

	virtual void ClassNewMembResult(CSG_MEMB_RESULT_D &MembR);
	virtual void ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);
	virtual void InitMembResult(CSG_MEMB_RESULT_D &MembR);
	virtual void InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);

protected:  

	virtual void SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
	virtual void SetCsfcFlexBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
	virtual void SetCsfcShearBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);

	virtual void ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
	virtual void ConvertCsgScvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
	virtual void ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
	virtual void ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
	virtual void ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiFlexRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiShearRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData); 
	virtual void ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData);
	virtual void ConvertCsgCsslMembNoLcomRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSBR_D &CsbrResD, T_CSCH_REF &rData);

	virtual BOOL IsChkFatiTranFlex();
	virtual BOOL IsSCMembChk();

};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNCsgDataCtrlKSCE_LSD15_H__INCLUDED_)
