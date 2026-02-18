#include "StdAfx.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"
#include "../wg_db/AttrCtrl2.h"
#include "../wg_db/DB_ST_DT_MATL.h"

#include "MembTypeUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CMembTypeUtil::CMembTypeUtil()
{
    m_pDoc = CDBDoc::GetDocPoint();
}

CMembTypeUtil::~CMembTypeUtil()
{
}

bool CMembTypeUtil::IsSTL(const T_ELEM_K& ElemK)
{
    T_MATD_D MatD;
    if ( !m_pDoc->m_pAttrCtrl->GetDgnGenMatd(ElemK, MatD) )
    {
        if(MatD.Type!=_T("U")) ASSERT(0); 
        return false;
    }

    return MatD.Type == _T("S") ? true : false;
}

bool CMembTypeUtil::IsRC(const T_ELEM_K& ElemK)
{
    T_MATD_D MatD;
    if ( !m_pDoc->m_pAttrCtrl->GetDgnGenMatd(ElemK, MatD) )
    {
        if(MatD.Type!=_T("U")) ASSERT(0);
        return false;
    }

    return MatD.Type == _T("C") ? true : false;
}

bool CMembTypeUtil::IsSRC(const T_ELEM_K& ElemK)
{
    T_MATD_D MatD;
    if (!m_pDoc->m_pAttrCtrl->GetDgnGenMatd(ElemK, MatD))
    {
        if(MatD.Type!=_T("U")) ASSERT(0);
        return false;
    }

    return MatD.Type == _T("SRC") ? true : false;
}

bool CMembTypeUtil::IsBeam(const T_ELEM_K& ElemK)
{
    return m_pDoc->m_pAttrCtrl->IsBeamTypeElem(ElemK);
}

bool CMembTypeUtil::IsEffectiveBeam(const T_ELEM_K& ElemK)
{
	return m_pDoc->m_pAttrCtrl2->IsEBeamK(ElemK);
}

bool CMembTypeUtil::IsColumnStrip(const T_ELEM_K& ElemK)
{
	T_MEMB_K MembK = 0;
	if (m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK, MembK))
	{
		if (m_pDoc->m_pAttrCtrl2->GetMstrKByMemb(MembK) > 0)
		{ 
			return true;
		}
	}

	return false;
}

bool CMembTypeUtil::IsColumn(const T_ELEM_K& ElemK)
{
    return m_pDoc->m_pAttrCtrl->IsColmTypeElem(ElemK);
}

bool CMembTypeUtil::IsBrace(const T_ELEM_K& ElemK)
{
    return m_pDoc->m_pAttrCtrl->IsBraceTypeElem(ElemK);
}

bool CMembTypeUtil::IsWall(const T_ELEM_K& ElemK)
{
	return m_pDoc->m_pAttrCtrl->IsWallTypeElem(ElemK);
}

bool CMembTypeUtil::IsPlate(const T_ELEM_K& ElemK)
{
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) return false;
    return m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp);
}

bool CMembTypeUtil::IsSlab(const T_ELEM_K& ElemK)
{
    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
    if ( nMembType == D_MBTP_ETC )
        return true;

    return false;
}

bool CMembTypeUtil::IsRCWall(const T_ELEM_K& ElemK)
{
    if ( !IsRC(ElemK) )
    {
        return false;
    }

    return m_pDoc->m_pAttrCtrl->IsWallTypeElem(ElemK);
}

bool CMembTypeUtil::IsRCPlate(const T_ELEM_K& ElemK)
{
    if (!IsRC(ElemK))
    {
        return false;
    }

    return IsPlate(ElemK);
}

bool CMembTypeUtil::IsRCPlateBeam(const T_ELEM_K& ElemK)
{
    if ( !IsRC(ElemK) )
    {
        return false;
    }

    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
    if ( nMembType == D_MBTP_PLATEBEAM )
        return true;

    return false;
}

bool CMembTypeUtil::IsRCPlateColumn(const T_ELEM_K& ElemK)
{
    if ( !IsRC(ElemK) )
    {
        return false;
    }

    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
    if ( nMembType == D_MBTP_PLATECOLM )
        return true;

    return false;
}

bool CMembTypeUtil::IsRCShell(const T_ELEM_K& ElemK)
{
    if ( !IsRC(ElemK) )
    {
        return false;
    }

    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
    if ( nMembType == D_MBTP_SHELL )
        return true;

    return false;
}

bool CMembTypeUtil::IsSTLBeam(const T_ELEM_K& ElemK)
{
    if ( !IsSTL(ElemK) )
    {
        return false;
    }

    return IsBeam(ElemK);
}

bool CMembTypeUtil::IsSTLColumn(const T_ELEM_K& ElemK)
{
    if ( !IsSTL(ElemK) )
    {
        return false;
    }

    return IsColumn(ElemK);
}

bool CMembTypeUtil::IsSTLBrace(const T_ELEM_K& ElemK)
{
    if (!IsSTL(ElemK))
    {
        return false;
    }

    return IsBrace(ElemK);
}

bool CMembTypeUtil::IsRCBeam(const T_ELEM_K& ElemK)
{
    if ( !IsRC(ElemK) )
    {
        return false;
    }

    return IsBeam(ElemK);
}

bool CMembTypeUtil::IsRCColumn(const T_ELEM_K& ElemK)
{
    if ( !IsRC(ElemK) )
    {
        return false;
    }

    return IsColumn(ElemK);
}

bool CMembTypeUtil::IsRCBrace(const T_ELEM_K& ElemK)
{
    if (!IsRC(ElemK))
    {
        return false;
    }

    return IsBrace(ElemK);
}

int CMembTypeUtil::GetMemberType(const T_ELEM_K& ElemK)
{
    return m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
}

bool CMembTypeUtil::IsHaunchedBeam(const T_ELEM_K& ElemK)
{
    return m_pDoc->m_pAttrCtrl->ExistHcbmAssigned(ElemK);
}

int CMembTypeUtil::GetHaunckedMembKey(const T_ELEM_K& MembK, std::vector<T_MEMB_K>& vHcbmKey)
{
    if ( !IsHaunchedBeam(MembK) )
    {
        ASSERT(0); return 0;
    }

    auto* const pAttrCtrl = m_pDoc->m_pAttrCtrl;
    if ( !pAttrCtrl )
    {
        ASSERT(0); return 0;
    }

    T_HCBM_K HcbmK;
    if ( !pAttrCtrl->GetHcbmAssigned(MembK, HcbmK) )
    {
        ASSERT(0); return 0;
    }
 
    T_HCBM_D HcbmD;
    if ( !pAttrCtrl->GetHcbm(HcbmK, HcbmD) )
    {
        ASSERT(0); return 0;
    }

    vHcbmKey.clear();
    for( int nIdx = 0; nIdx < 3; ++nIdx )
    {
        const auto& HcbmKey = HcbmD.aElemListPart[nIdx].GetAt(0);
        vHcbmKey.push_back(HcbmKey);
    }

    return static_cast<int>(vHcbmKey.size());
}
