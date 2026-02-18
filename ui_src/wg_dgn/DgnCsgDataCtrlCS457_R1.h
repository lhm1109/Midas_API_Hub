// DgnCsgDataCtrlAASHTO12.h: interface for the CDgnCsgDataCtrlAASHTO12 class.
    //
    //////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCsgDataCtrlCS457_R1_H__INCLUDED_)
#define AFX_DGNCsgDataCtrlCS457_R1_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnCsgDataCtrl.h"

#include "HeaderPre.h"

struct CSG_MEMB_RES_AASHTO_LRFD_STR_D;

class __MY_EXT_CLASS__ CDgnCsgDataCtrlCS457_R1: public CDgnCsgDataCtrl
{
    // Member Functions.
public:
    CDgnCsgDataCtrlCS457_R1();
    virtual ~CDgnCsgDataCtrlCS457_R1();

    virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);

    virtual BOOL MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData) override;

    virtual BOOL Check_Strength(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
    virtual BOOL Check_Service(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;
    virtual BOOL Check_ShearConnector(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR) override;

};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNCsgDataCtrlCS457_R1_H__INCLUDED_)
