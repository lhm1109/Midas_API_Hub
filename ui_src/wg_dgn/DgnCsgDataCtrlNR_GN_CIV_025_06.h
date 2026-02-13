#pragma once

#include "DgnCsgDataCtrl.h"

#include "HeaderPre.h"

struct CSG_MEMB_RES_AASHTO_LRFD_STR_D;

class __MY_EXT_CLASS__ CDgnCsgDataCtrlNR_GN_CIV_025_06 : public CDgnCsgDataCtrl
{
    // Member Functions.
public:
    CDgnCsgDataCtrlNR_GN_CIV_025_06();
    virtual ~CDgnCsgDataCtrlNR_GN_CIV_025_06();

    virtual void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);

    virtual BOOL MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD& CsgMembD, CSG_MEMB_RESULT_D& rData) override;

    virtual BOOL Check_Strength(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD& InD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes, CSG_CHK_RES_DTR_PARAM& DtrR) override;
    virtual BOOL Check_Service(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD& InD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes, CSG_CHK_RES_DTR_PARAM& DtrR) override;
    //virtual BOOL Check_ShearConnector(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD& InD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes, CSG_CHK_RES_DTR_PARAM& DtrR) override;

};

#include "HeaderPost.h"

