#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "DgnCsgDataCtrlCS457_R1.h"
#include "..\wg_db\DBLib.h"
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "Dgn_CSGManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnCsgDataCtrlCS457_R1::CDgnCsgDataCtrlCS457_R1() : CDgnCsgDataCtrl()
{
}

CDgnCsgDataCtrlCS457_R1::~CDgnCsgDataCtrlCS457_R1()
{
}

void CDgnCsgDataCtrlCS457_R1::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
    CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;

    CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;

    m_pDataCtrl->Set_UnitIndex(CodeUnit);

    InitialData(m_nDgnCode);
}

BOOL CDgnCsgDataCtrlCS457_R1::MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData)
{
    if ( rData.pCS457==NULL ) return FALSE;

    BOOL bCurved =  (CsgMembD.Memb.Csgd.nConsiderPv==0)? FALSE : TRUE; // 0:Á÷¼±, 1:°î¼±

    m_pCsgManager->Check_MemberResult(CsgMembD, rData);

    CsgMembD.Memb.ChIf.nChkPos = 1;
    CsgMembD.Memb.ChIf.bCurved = bCurved;
    m_pCsgManager->Check_MemberResult(CsgMembD, rData);


    return TRUE;
}

BOOL CDgnCsgDataCtrlCS457_R1::Check_Strength(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    if ( MembRes.pCS457==NULL ) return FALSE;
    if ( ChkRes.pCS457==NULL ) return FALSE;
    if ( !m_pCsgManager->Check_StrengthRes(InD, MembRes, ChkRes) ) return FALSE;
//    int nMomentType = ChkRes.pAASHTO->StrengthD.FlexD[0].nCase > 0 ? 0 : 1;
//     BOOL bPositive = ChkRes.pAASHTO->StrengthD.FlexD[0].nCase > 0 ? TRUE : FALSE;
//     DtrR.dDTR_F = 0.0; // GetAASHTOFlexureRatio(ChkRes.pAASHTO->StrengthD, bPositive);
//     DtrR.dDTR_S = ChkRes.pAASHTO->StrengthD.ShearD.dRatio;

    return TRUE;
}

BOOL CDgnCsgDataCtrlCS457_R1::Check_Service(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    if ( MembRes.pCS457==NULL ) return FALSE;
    if ( ChkRes.pCS457==NULL ) return FALSE;
    BOOL bCheck = m_pCsgManager->Check_ServiceRes(InD, MembRes, ChkRes);
//    DtrR.dDTR_Ser = ChkRes.pAASHTO->ServiceD.dRatio;

    return bCheck;
}

BOOL CDgnCsgDataCtrlCS457_R1::Check_ShearConnector(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR)
{
    if ( MembRes.pCS457==NULL ) return FALSE;
    if ( ChkRes.pCS457==NULL ) return FALSE;
    BOOL bCheck = m_pCsgManager->Check_ShearConnRes(InD, MembRes, ChkRes);

    return bCheck;
}

