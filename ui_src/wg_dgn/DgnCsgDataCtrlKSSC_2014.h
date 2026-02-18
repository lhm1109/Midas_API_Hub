// DgnCsgDataCtrlKSSC_2014.h: interface for the CDgnCsgDataCtrlAASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCsgDataCtrlKSSC_2014_H__INCLUDED_)
#define AFX_DGNCsgDataCtrlKSSC_2014_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnCsgDataCtrlAASHTO12.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnCsgDataCtrlKSSC_2014 : public CDgnCsgDataCtrlAASHTO12
{
	// Member Functions.
public:
	CDgnCsgDataCtrlKSSC_2014();
	virtual ~CDgnCsgDataCtrlKSSC_2014();

	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);

	BOOL MakeMemberResultData(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData);

	virtual BOOL Check_Constructibility(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
	virtual BOOL Check_Strength        (T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
	virtual BOOL Check_Service         (T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
	virtual BOOL Check_Fatigue         (T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR);
	virtual BOOL Check_ShearConnector  (T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
	virtual BOOL Check_Stiffener       (T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);

//   virtual void ClassNewMembResult(CSG_MEMB_RESULT_D &MembR);
//   virtual void ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);
//   virtual void InitMembResult(CSG_MEMB_RESULT_D &MembR);
//   virtual void InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);

protected:  

	virtual BOOL IsChkFatigue2(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL IsSkipChkScon(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL IsBeforeSgldDL();
	virtual BOOL IsAppErLoad();
    virtual BOOL IsShearConnectorFatigue2(CSG_CHECK_RESULT_CODES &ChkRes) override final;

};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNCsgDataCtrlKSSC_2014_H__INCLUDED_)
