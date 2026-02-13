// DgnCsgDataCtrlAASHTO12.h: interface for the CDgnCsgDataCtrlAASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCsgDataCtrlCSA_S6_14_H__INCLUDED_)
#define AFX_DGNCsgDataCtrlCSA_S6_14_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnCsgDataCtrl.h"

#include "HeaderPre.h"

struct CSG_MEMB_RES_CSA_S6_D;
struct CSG_CHECK_RES_CSA_S6;

class __MY_EXT_CLASS__ CDgnCsgDataCtrlCSA_S6_14 : public CDgnCsgDataCtrl
{
	// Member Functions.
public:
	CDgnCsgDataCtrlCSA_S6_14();
	virtual ~CDgnCsgDataCtrlCSA_S6_14();

	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);

	virtual BOOL MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData) override;

	virtual BOOL Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Strength        (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Service         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_Fatigue         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR) override;
	virtual BOOL Check_ShearConnector  (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	virtual BOOL Check_BearingStiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
	
	virtual void GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy);

	virtual void ClassNewMembResult(CSG_MEMB_RESULT_D &MembR);
	virtual void ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);
	virtual void InitMembResult(CSG_MEMB_RESULT_D &MembR);
	virtual void InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);

protected:  

	virtual void SetScbrBase(BOOL bChk, int nLcomK4CS,  int nStageK,    int nStepK,      T_CSCH_REF &rData);
	virtual void SetScvrBase(BOOL bChk, int nLcomK4CS,  int nStageK,    int nStepK,      T_CSCH_REF &rData);
	virtual void SetCsbrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
	virtual void SetCsvrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
	virtual void SetCsslBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
	virtual void SetCsacBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
	virtual void SetCsfcBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
	virtual void SetCscrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
	virtual void SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
	virtual void SetCpfcStudBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
	virtual void SetCpfcSconBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);


	virtual void ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
	virtual void ConvertCsgScvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
	virtual void ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
	virtual void ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
	virtual void ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	//virtual void ConvertCsgFatiFlexRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	//virtual void ConvertCsgFatiShearRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiStudRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);
	virtual void ConvertCsgFatiSconRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);
	virtual void ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData); 
	virtual void ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData);
	virtual void ConvertCsgCsslMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
	virtual void ConvertCsgCptsMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);
	virtual void ConvertCsgCptsResLcom(CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
	virtual void ConvertCsgBstfMembRes(CSG_MEMB_RESULT_D& MembR, CSG_MEMB_POSD_KSCE_LSD& LcomResD, T_CSCH_REF& rData);

	void ConvertCsgFlangeClassCSA(CSG_MEMB_RES_CSA_S6_D &MembRes, CSG_CHECK_RES_CSA_S6 &ChkRes, int &nTopClass, int &nBotClass); 

	virtual BOOL IsChkFatiLoadStud();
	virtual BOOL IsSCMembChk();
};

class __MY_EXT_CLASS__ CDgnCsgDataCtrlCSA_S6_10 : public CDgnCsgDataCtrlCSA_S6_14
{
	// Member Functions.
public:
	CDgnCsgDataCtrlCSA_S6_10();
	virtual ~CDgnCsgDataCtrlCSA_S6_10();

};
#include "HeaderPost.h"

#endif // !defined(AFX_DGNCsgDataCtrlCSA_S6_14_H__INCLUDED_)
