// DgnCsgDataCtrlAASHTO12.h: interface for the CDgnCsgDataCtrlAASHTO12 class.
	//
	//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCsgDataCtrlAASHTO12_H__INCLUDED_)
#define AFX_DGNCsgDataCtrlAASHTO12_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnCsgDataCtrl.h"

#include "HeaderPre.h"

struct CSG_MEMB_RES_AASHTO_LRFD_STR_D;

class __MY_EXT_CLASS__ CDgnCsgDataCtrlAASHTO12 : public CDgnCsgDataCtrl
{
	// Member Functions.
public:
	CDgnCsgDataCtrlAASHTO12();
	virtual ~CDgnCsgDataCtrlAASHTO12();

	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);

	virtual BOOL MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData) override;

	virtual BOOL Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Strength        (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Service         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Fatigue         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR) override;
	virtual BOOL Check_ShearConnector  (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Stiffener       (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_BearingStiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;

	virtual void GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy);

	virtual void ClassNewMembResult(CSG_MEMB_RESULT_D &MembR);
	virtual void ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);
	virtual void InitMembResult(CSG_MEMB_RESULT_D &MembR);
	virtual void InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);

protected:
	BOOL MakeMemberResDataAASHTO(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData);

	virtual BOOL FindFatiCrRes4Lcom(int nFLoadType, int nIJ, BOOL *pbChkIJ, 
																	int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo,
																	CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
																	CSG_MEMB_RESULT_D &MembRes,
																	int *pnFatigueII,
																	int *pnLoadTop, int *pnLoadBot,
																	double *pdLoadTopRat, double *pdLoadBotRat,
																	CSG_CHK_RES_DTR_PARAM &DtrR,
																	CSG_CHECK_RESULT_CODES &ChkResD,
																	T_CSCH_REF &RsltRef,
																	CSG_FATI_CR_RSLT &FRes) override;

	virtual BOOL FindFatiSCCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
																		int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
																		CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
																		CSG_MEMB_RESULT_D &MembRes,
																		int *pnSCcount,
																		double *pdSCmaxRat,
																		CSG_CHK_RES_DTR_PARAM &DtrR,
																		CSG_CHECK_RESULT_CODES &ChkResD,
																		T_CSCH_REF &RsltRef) override;

	virtual BOOL FindFatiSCLoadCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
																				int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
																				CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
																				CSG_MEMB_RESULT_D &MembRes,
																				int *pnFatigueII_SC,
																				int *pnSCFati,
																				double *pdSCFatigueRat,
																				CSG_CHK_RES_DTR_PARAM &DtrR,
																				CSG_CHECK_RESULT_CODES &ChkMaResD,
																				CSG_CHECK_RESULT_CODES &ChkResD,
																				T_CSCH_REF &RsltRef,
																				CSG_FATI_CR_RSLT &FRes) override;

	virtual void DecisionFatigueRes(CSG_FATI_CR_RSLT &FRes, T_CSFC_D &CsfcResD, T_CSCR_D &CscrResD);

	double GetAASHTOFlexureRatio(CSG_MEMB_RES_AASHTO_LRFD_STR_D &Inp, BOOL bPositive);

	virtual void SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
	virtual void SetCsfcFlexBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
	virtual void SetCsfcShearBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);


	virtual void ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
	virtual void ConvertCsgScvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
	virtual void ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
	virtual void ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
	virtual void ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiShearRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData); 
	virtual void ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData);
	virtual void ConvertCsgCsslMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgBstfMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);


	virtual BOOL IsChkFatigue2(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL IsChkFatigueCr(int nFatiLcomType, int nFatigueType2);
	virtual BOOL IsSkipChkScon(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL IsFatigue1(BOOL bFatigueI);
    virtual BOOL IsShearConnectorFatigue2(CSG_CHECK_RESULT_CODES &ChkRes);
};

class __MY_EXT_CLASS__ CDgnCsgDataCtrlAASHTO07 : public CDgnCsgDataCtrlAASHTO12
{
	// Member Functions.
public:
	CDgnCsgDataCtrlAASHTO07();
	virtual ~CDgnCsgDataCtrlAASHTO07();

protected:
	virtual BOOL IsFatigue1(BOOL bFatigueI);
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNCsgDataCtrlAASHTO12_H__INCLUDED_)
