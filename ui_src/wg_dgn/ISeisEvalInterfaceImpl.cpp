#include "stdafx.h"
#include "ISeisEvalInterfaceImpl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\DbToolPmcv.h"

#include "..\wg_db\SeisEvalResult.h"

#include "SeisEvalKISTEC2013.h"
#include "SeisEvalKISTEC2019.h"
#include "SeisEvalMOE2018.h"
#include "SeisEvalMOE2019.h"
#include "SeisEvalAIK_G001_2021.h"

#include "SeisEvalPrint.h"
#include "DgnProgressDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ISeisEvalInterfaceImpl _g_Instance;
ISeisEvalInterfaceImpl::ISeisEvalInterfaceImpl(void)
{
}

ISeisEvalInterfaceImpl::~ISeisEvalInterfaceImpl(void)
{
}

void ISeisEvalInterfaceImpl::SetCalcPTR()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); }

    pDoc->m_pUnitCtrl->GetUnitIndexCurrent(m_CurrUnit);
    if ( m_pCalc != nullptr )
    {
        T_UNIT_INDEX OrgUnit = m_pCalc->GetCurrUnit();		
        pDoc->m_pUnitCtrl->SetUnitIndexCurrent(OrgUnit);
        delete m_pCalc;
    }

    T_SEME_D SemeD;
    if (!pDoc->m_pAttrCtrl2->GetSeme(SemeD)) { SemeD.Initialize(); }
    m_unCode = SemeD.nMethod;
    m_pCalc  = nullptr;
    switch (m_unCode)
    {
    case D_SEME_METHOD_KISTEC2013: m_pCalc = new CSeisEvalKISTEC2013(); break;
    case D_SEME_METHOD_MOE2018:    m_pCalc = new CSeisEvalMOE2018();    break; 
    case D_SEME_METHOD_MOE2019:    m_pCalc = new CSeisEvalMOE2019();    break; 
    case D_SEME_METHOD_KISTEC2019: m_pCalc = new CSeisEvalKISTEC2019(); break;
	case D_SEME_METHOD_AIK_G_001_2021: m_pCalc = new CSeisEvalAIK_G001_2021(); break;
    default: ASSERT(FALSE);	break;
    }   
}

void ISeisEvalInterfaceImpl::EndCalcPTR()
{
    if ( m_pCalc != nullptr ) { delete m_pCalc; m_pCalc = nullptr; }    
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeConBeam(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD)
{	
    if (!m_pCalc->InitializeHingeConBeam(Param, rPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeConColm(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD)
{
    if (!m_pCalc->InitializeHingeConColm(Param, rPhgtD)) return FALSE;

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeConWall(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD)
{
    if (!m_pCalc->InitializeHingeConWall(Param, rPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeConBeam(const T_HINGE_CALC_PARAM& Param, OUT T_IEHP_D& rIehpD)
{
	T_PHGT_D PhgtD;
	if ( !m_pCalc->InitializeHingeConBeam(Param, PhgtD) ) { return FALSE; }

	ConvertUnitPhgt_Code2Curr(PhgtD);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return pDoc->m_pAttrCtrl->ConvertPhgt2Iehp(PhgtD, rIehpD, TRUE, 1.65, TRUE, FALSE, FALSE);	
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeConColm(const T_HINGE_CALC_PARAM& Param, OUT T_IEHP_D& rIehpD)
{
	T_PHGT_D PhgtD;
	if ( !m_pCalc->InitializeHingeConColm(Param, PhgtD) ) { return FALSE; }
	
	ConvertUnitPhgt_Code2Curr(PhgtD);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return pDoc->m_pAttrCtrl->ConvertPhgt2Iehp(PhgtD, rIehpD, TRUE, 1.65, TRUE, FALSE, FALSE);
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeStlBeam(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    if (!m_pCalc->InitializeHingeStlBeam(abDof, rPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeStlColm(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    if (!m_pCalc->InitializeHingeStlColm(abDof, rPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeStlBrac(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    if (!m_pCalc->InitializeHingeStlBrac(abDof, rPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeStlTrus(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    if (!m_pCalc->InitializeHingeStlTrus(abDof, rPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeSrcBeam(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD)
{
    if (!m_pCalc->InitializeHingeSrcBeam(abDof, rEncasePhgtD, rFilledPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rEncasePhgtD);
    ConvertUnitPhgt_Code2Curr(rFilledPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeSrcColm(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD)
{
    if (!m_pCalc->InitializeHingeSrcColm(abDof, rEncasePhgtD, rFilledPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rEncasePhgtD);
    ConvertUnitPhgt_Code2Curr(rFilledPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeInfill(const BOOL abDof[6], OUT T_PHGT_D& rPartPhgtD, OUT T_PHGT_D& rFullPhgtD)
{
    if (!m_pCalc->InitializeHingeInfill(abDof, rPartPhgtD, rFullPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPartPhgtD);
    ConvertUnitPhgt_Code2Curr(rFullPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::InitializeHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    if (!m_pCalc->InitializeHingeInfill(ElemK, abDof, rPhgtD)) { return FALSE; }

    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeConBeam(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

	if ( rPhgtD.nDefinition != D_PHGT_DEFINITION_MTHETA_LUMP )
	{
		m_pCalc->InitializeHingeConBeam(Param, rPhgtD);
		rPhgtD.strName.Format(_T("C_BE_%d"), ElemK);
	}

    if (!m_pCalc->CalcUpdateHingeConBeam(ElemK, Param, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeConColm(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

	if ( rPhgtD.nDefinition != D_PHGT_DEFINITION_MTHETA_LUMP )
	{
		m_pCalc->InitializeHingeConColm(Param, rPhgtD);
		rPhgtD.strName.Format(_T("C_CO_%d"), ElemK);
	}

    m_pCalc->SetHingeParamPMM(ElemK, Param.nUpdateMaterial, Param.enHingeType, rPhgtD);
    if (!m_pCalc->CalcUpdateHingeConColm(ElemK, Param, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeConWall(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

	if ( rPhgtD.nDefinition != D_PHGT_DEFINITION_MTHETA_LUMP )
	{
		m_pCalc->InitializeHingeConWall(Param, rPhgtD);
		rPhgtD.strName.Format(_T("C_WL_%d"), ElemK);
	}

    m_pCalc->SetHingeParamPMM(ElemK, Param.nUpdateMaterial, Param.enHingeType, rPhgtD);
    if (!m_pCalc->CalcUpdateHingeConWall(ElemK, Param, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeStlBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환//
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

    if (!m_pCalc->CalcUpdateHingeStlBeam(ElemK, abDof, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeStlColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nUpdateMatl, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

    m_pCalc->SetHingeParamPMM(ElemK, nUpdateMatl, EN_NLANALTYPE::PO, rPhgtD);
    if (!m_pCalc->CalcUpdateHingeStlColm(ElemK, abDof, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeStlBrac(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nUpdateMatl, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

    m_pCalc->SetHingeParamPMM(ElemK, nUpdateMatl, EN_NLANALTYPE::PO, rPhgtD);
    if (!m_pCalc->CalcUpdateHingeStlBrac(ElemK, abDof, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeStlTrus(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

    if (!m_pCalc->CalcUpdateHingeStlTrus(ElemK, abDof, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeSrcBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

    if (!m_pCalc->CalcUpdateHingeSrcBeam(ElemK, abDof, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeSrcColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nUpdateMatl, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
    ConvertPhgt2CodeUnit(rPhgtD);

    m_pCalc->SetHingeParamPMM(ElemK, nUpdateMatl, EN_NLANALTYPE::PO, rPhgtD);
    if (!m_pCalc->CalcUpdateHingeSrcColm(ElemK, abDof, InpPORslt, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeInfill(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_MAP& mPORslt, OUT T_PHGT_D& rPhgtD)
{
    // Input Code단위계로 변환 //
    T_SEIS_PO_ANAL_MAP mPORsltCodeUnit;

    POSITION pos = mPORslt.GetStartPosition();
    while (pos)
    {        
        T_ELEM_K CurElemK = 0;
        T_SEIS_PO_ANAL_RESULT_D CurrUnit;
        mPORslt.GetNextAssoc(pos, CurElemK, CurrUnit);

        T_SEIS_PO_ANAL_RESULT_D CodeUnit = ConvertPORslt2CodeUnit(CurrUnit);
        mPORsltCodeUnit.SetAt(CurElemK, CodeUnit);
    }

    ConvertPhgt2CodeUnit(rPhgtD);

    if (!m_pCalc->CalcUpdateHingeInfill(ElemK, abDof, mPORsltCodeUnit, rPhgtD))
    {
        return FALSE;
    }
    ConvertUnitPhgt_Code2Curr(rPhgtD);
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeConBeam(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_IEHP_D& rIehpD)
{
	// Input Code단위계로 변환 //
	T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
	ConvertIehp2CodeUnit(rIehpD);

	T_PHGT_D PhgtD;
	InitializeHingeConBeam(Param, PhgtD);
	PhgtD.strName = rIehpD.strName;

	if ( !m_pCalc->CalcUpdateHingeConBeam(ElemK, Param, InpPORslt, PhgtD) )
	{
		ASSERT(0);
		return FALSE;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( !pDoc->m_pAttrCtrl->ConvertPhgt2Iehp(PhgtD, rIehpD, TRUE, 1.65, TRUE, FALSE, FALSE) )
	{
		ASSERT(0);
		return FALSE;
	}

	rIehpD.strName.Format(_T("C_BE_%d"), ElemK);

	ConvertUnitIehp_Code2Curr(rIehpD);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcUpdateHingeConColm(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_IEHP_D& rIehpD)
{
	// Input Code단위계로 변환 //
	T_SEIS_PO_ANAL_RESULT_D InpPORslt = ConvertPORslt2CodeUnit(PORslt);
	ConvertIehp2CodeUnit(rIehpD);

	T_PHGT_D PhgtD;
	InitializeHingeConColm(Param, PhgtD);
	PhgtD.strName = rIehpD.strName;
	m_pCalc->SetHingeParamPMM(ElemK, Param.nUpdateMaterial, Param.enHingeType, PhgtD);
	if ( !m_pCalc->CalcUpdateHingeConColm(ElemK, Param, InpPORslt, PhgtD) )
	{
		ASSERT(0);
		return FALSE;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if ( !pDoc->m_pAttrCtrl->ConvertPhgt2Iehp(PhgtD, rIehpD, TRUE, 1.65, TRUE, FALSE, FALSE) )
	{
		ASSERT(0);
		return FALSE;
	}

	rIehpD.strName.Format(_T("C_CO_%d"), ElemK);

	ConvertUnitIehp_Code2Curr(rIehpD);	

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetEvalPreConColmSimple(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre)
{
    if ( !m_pCalc->CalcEvalPreConColmSimple(ElemK, rEvalPre) ) return FALSE;

    auto l_ConvertCurUnit = [this](OUT T_EVALPRE_COLM_DIR& rDir)
    {
        rDir.dD   *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);
        rDir.dh0  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);
        rDir.dvsc *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_STRESS);
        rDir.dAsc *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_AREA);
        rDir.dVp  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dVn  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dCf  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dCs  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
    };

    l_ConvertCurUnit(rEvalPre.DirX);
    l_ConvertCurUnit(rEvalPre.DirY);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetEvalPreConWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre)
{
    if ( !m_pCalc->CalcEvalPreConWallSimple(ElemK, rEvalPre) ) return FALSE;

    rEvalPre.dTw *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);

    auto l_ConvertCurUnit = [this](OUT T_EVALPRE_WALL_DIR& rDir)
    {
        rDir.dLw  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);
        rDir.dvsw *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_STRESS);
        rDir.dAsw *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_AREA);
        rDir.dVn  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dCs  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);        
    };

    l_ConvertCurUnit(rEvalPre.DirX);
    l_ConvertCurUnit(rEvalPre.DirY);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetEvalPreInfWallSimple(T_ELEM_K ElemK, OUT T_EVALPRE_INFL& rEvalPre)
{
    if ( !m_pCalc->CalcEvalPreInfWallSimple(ElemK, rEvalPre) ) return FALSE;

    auto l_ConvertCurUnit = [this](OUT T_EVALPRE_INFL_DIR& rDir)
    {
        rDir.dvsm *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_STRESS);
        rDir.dAsm *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_AREA);
        rDir.dCs  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
    };

    l_ConvertCurUnit(rEvalPre.DirX);
    l_ConvertCurUnit(rEvalPre.DirY);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetEvalPreConColmGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_COLM& rEvalPre)
{
    if ( !m_pCalc->CalcEvalPreConColmGeneral(ElemK, rEvalPre) ) return FALSE;

    auto l_ConvertCurUnit = [this](OUT T_EVALPRE_COLM_DIR& rDir)
    {
        rDir.dD   *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);
        rDir.dh0  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);
        rDir.dvsc *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_STRESS);
        rDir.dAsc *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_AREA);
        rDir.dVp  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dVn  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dCf  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dCs  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
    };

    l_ConvertCurUnit(rEvalPre.DirX);
    l_ConvertCurUnit(rEvalPre.DirY);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetEvalPreConWallGeneral(T_ELEM_K ElemK, OUT T_EVALPRE_WALL& rEvalPre)
{
    if ( !m_pCalc->CalcEvalPreConWallGeneral(ElemK, rEvalPre) ) return FALSE;

    rEvalPre.dTw *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);

    auto l_ConvertCurUnit = [this](OUT T_EVALPRE_WALL_DIR& rDir)
    {
        rDir.dLw  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_LENGTH);
        rDir.dvsw *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_STRESS);
        rDir.dAsw *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_AREA);
        rDir.dVn  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);
        rDir.dCs  *= GetUnitFactorCode2Cur(D_UNITSYS_BASE_FORCE);        
    };

    l_ConvertCurUnit(rEvalPre.DirX);
    l_ConvertCurUnit(rEvalPre.DirY);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalLinRsltConBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_BEAM& raEvalLin)
{
    if (!m_pCalc->CalcSeisEvalLinRsltConBeam(ElemK, unType, raEvalLin)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    if (pUnitCtrl == nullptr) 
    {
        return FALSE;
    }

    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSELinConBeamIn(raEvalLin);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSELinConBeamOut(raEvalLin);       

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalLinRsltConColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_COLM& raEvalLin)
{
    if (!m_pCalc->CalcSeisEvalLinRsltConColm(ElemK, unType, raEvalLin)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    if (pUnitCtrl == nullptr)
    {
        return FALSE;
    }

    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSELinConColmIn(raEvalLin);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSELinConColmOut(raEvalLin);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalLinRsltConWall(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_WALL& raEvalLin)
{
    if (!m_pCalc->CalcSeisEvalLinRsltConWall(ElemK, unType, raEvalLin)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSELinConWallIn(raEvalLin);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSELinConWallOut(raEvalLin);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalLinRsltStlBeam(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBEAM& raEvalLin)
{
    if (!m_pCalc->CalcSeisEvalLinRsltStlBeam(ElemK, unType, raEvalLin)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSELinStlBeamIn(raEvalLin);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSELinStlBeamOut(raEvalLin);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalLinRsltStlColm(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLCOLM& raEvalLin)
{
    if (!m_pCalc->CalcSeisEvalLinRsltStlColm(ElemK, unType, raEvalLin)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSELinStlColmIn(raEvalLin);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSELinStlColmOut(raEvalLin);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalLinRsltStlBrac(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLBRAC& raEvalLin)
{
    if (!m_pCalc->CalcSeisEvalLinRsltStlBrac(ElemK, unType, raEvalLin)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSELinStlBracIn(raEvalLin);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSELinStlBracOut(raEvalLin);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalLinRsltInfWall(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_INFILL& raEvalLin)
{
    if (!m_pCalc->CalcSeisEvalLinRsltInfill(ElemK, unType, raEvalLin)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSELinInfWallIn(raEvalLin);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSELinInfWallOut(raEvalLin);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEvalRsltInfWallOut(T_ELEM_K ElemK, OUT T_EVAL_INFILL_OUT& raEval)
{
    if (!m_pCalc->CalcSeisEvalInfillOut(ElemK, raEval)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSEInfWallOutIn(raEval);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSEInfWallOutOut(raEval);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltConBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_BEAM& raEval1st)
{
    if ( !GetSEResult()->GetResultSE1stConBeam(ElemK, raEval1st) )
    {
        if ( !m_pCalc->CalcSeisEval1stRsltConBeam(ElemK, raEval1st) ) { return FALSE; }

        CUnitCtrl* pUnitCtrl = GetUnitCtrl();
        pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
        pUnitCtrl->ConvertUnitSE1stConBeamOut(raEval1st);
    }
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltConColm(T_ELEM_K ElemK, OUT T_EVAL1ST_COLM& raEval1st)
{
    if ( !GetSEResult()->GetResultSE1stConColm(ElemK, raEval1st) )
    {
        if ( !m_pCalc->CalcSeisEval1stRsltConColm(ElemK, raEval1st) ) return FALSE;

        CUnitCtrl* pUnitCtrl = GetUnitCtrl();
        pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
        pUnitCtrl->ConvertUnitSE1stConColmOut(raEval1st);
    }
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltConWall(T_ELEM_K ElemK, OUT T_EVAL1ST_WALL& raEval1st)
{
    if ( !GetSEResult()->GetResultSE1stConWall(ElemK, raEval1st) )
    {
        if ( !m_pCalc->CalcSeisEval1stRsltConWall(ElemK, raEval1st) ) { return FALSE; }

        CUnitCtrl* pUnitCtrl = GetUnitCtrl();
        pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
        pUnitCtrl->ConvertUnitSE1stConWallOut(raEval1st);
    }
    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltStlBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st)
{
    if (!m_pCalc->CalcSeisEval1stRsltStlBeam(ElemK, raEval1st)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE1stStlBeamIn(raEval1st);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE1stStlBeamOut(raEval1st);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltStlColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st)
{
    if (!m_pCalc->CalcSeisEval1stRsltStlColm(ElemK, raEval1st)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE1stStlColmIn(raEval1st);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE1stStlColmOut(raEval1st);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltStlPanz(T_ELEM_K ElemK, OUT T_EVAL1ST_STLPANZ& raEval1st)
{
    if (!m_pCalc->CalcSeisEval1stRsltStlPanz(ElemK, raEval1st)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE1stStlPanzIn(raEval1st);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE1stStlPanzOut(raEval1st);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltStlBrac(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBRAC& raEval1st)
{
    if (!m_pCalc->CalcSeisEval1stRsltStlBrac(ElemK, raEval1st)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE1stStlBracIn(raEval1st);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE1stStlBracOut(raEval1st);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltInfWall(T_ELEM_K ElemK, OUT T_EVAL1ST_INFILL& raEval1st)
{
    if (!m_pCalc->CalcSeisEval1stRsltInfill(ElemK, raEval1st)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE1stInfWallIn(raEval1st);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE1stInfWallOut(raEval1st);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltSrcBeam(T_ELEM_K ElemK, OUT T_EVAL1ST_STLBEAM& raEval1st)
{
    if (!m_pCalc->CalcSeisEval1stRsltSrcBeam(ElemK, raEval1st)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE1stStlBeamIn(raEval1st);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE1stStlBeamOut(raEval1st);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval1stRsltSrcColm(T_ELEM_K ElemK, OUT T_EVAL1ST_STLCOLM& raEval1st)
{
    if (!m_pCalc->CalcSeisEval1stRsltSrcColm(ElemK, raEval1st)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE1stStlColmIn(raEval1st);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE1stStlColmOut(raEval1st);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_BEAM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltConBeam(ElemK, PolcK, nStep, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndConBeamIn(raEval2nd);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndConBeamOut(raEval2nd);

    return TRUE;
}


BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_COLM& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConColm(ElemK, PolcK, nStep, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_WALL& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConWall(ElemK, PolcK, nStep, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltConColmAgbp(ElemK, PolcK, nStep, nSerialAgxp, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltConWallAgwp(ElemK, PolcK, nStep, nSerialAgxp, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl(); 
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndConWallIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndConWallOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBEAM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltStlBeam(ElemK, PolcK, nStep, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBeamIn(raEval2nd);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBeamOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLCOLM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltStlColm(ElemK, PolcK, nStep, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndStlColmIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndStlColmOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_STLBRAC& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltStlBrac(ElemK, PolcK, nStep, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBracIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBracOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, OUT T_EVAL2ND_INFILL& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltInfWall(ElemK, PolcK, nStep, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndInfWallIn(raEval2nd);    
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndInfWallOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConColmFiber(ElemK, PolcK, nStep, nSerialAgxp, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::GetSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConWallFiber(ElemK, PolcK, nStep, nSerialAgxp, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltConBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndConBeamIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndConBeamOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConColmAgbp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltConColmAgbp(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConWallAgwp(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltConWallAgwp(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndConWallIn(raEval2nd);	
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndConWallOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltStlBeam(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBeamIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBeamOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltStlColm(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndStlColmIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndStlColmOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltStlBrac(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBracIn(raEval2nd);	
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndStlBracOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd)
{
    if (!m_pCalc->CalcSeisEval2ndRsltInfWall(ElemK, PolcK, nStep, nAddStepIdx, raEval2nd)) return FALSE;

    CUnitCtrl* pUnitCtrl = GetUnitCtrl();
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
    pUnitCtrl->ConvertUnitSE2ndInfWallIn(raEval2nd);
    pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
    pUnitCtrl->ConvertUnitSE2ndInfWallOut(raEval2nd);

    return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConColmFiber(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConWallFiber(ElemK, PolcK, nStep, nAddStepIdx, nSerialAgxp, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConBeamTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, OUT T_EVAL2ND_BEAM& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConBeamTH(ElemK, ThisK, nTimeIndex, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConBeamIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConBeamOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConColmTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConColmTH(ElemK, ThisK, nTimeIndex, nSerialAgxt, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

	return TRUE;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConColmFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_COLM& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConColmFiberTH(ElemK, ThisK, nTimeIndex, nSerialAgxt, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConColmOut(raEval2nd);

	return TRUE;
}

bool ISeisEvalInterfaceImpl::ReCalcSeisEval2ndRsltConBeamLumpedTH(const T_EVAL2ND_BEAM_POS& InData, OUT T_EVAL2ND_BEAM_POS& rData)
{
	if ( !m_pCalc->ReCalcSeisEval2ndRsltConBeamLumpedTH(InData, rData) ) return false;

	return true;
}

bool ISeisEvalInterfaceImpl::ReCalcSeisEval2ndRsltConColmLumpedTH(const int nDOF, const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData)
{
    if ( !m_pCalc->ReCalcSeisEval2ndRsltConColmLumpedTH(nDOF, InData, rData) ) return false;

    return true;
}

bool ISeisEvalInterfaceImpl::ReCalcSeisEval2ndRsltConColmFiberTHRAngle(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData)
{
	if ( !m_pCalc->ReCalcSeisEval2ndRsltConColmFiberTHRAngle(InData, rData) ) return false;

	return true;
}

bool ISeisEvalInterfaceImpl::ReCalcSeisEval2ndRsltConColmFiberTHShear(const T_EVAL2ND_COLM_POS& InData, OUT T_EVAL2ND_COLM_POS& rData)
{
    if (!m_pCalc->ReCalcSeisEval2ndRsltConColmFiberTHShear(InData, rData)) return false;

    return true;
}

BOOL ISeisEvalInterfaceImpl::CalcSeisEval2ndRsltConWallFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nSerialAgxt, OUT T_EVAL2ND_WALL& raEval2nd)
{
	if ( !m_pCalc->CalcSeisEval2ndRsltConWallFiberTH(ElemK, ThisK, nTimeIndex, nSerialAgxt, raEval2nd) ) return FALSE;

	CUnitCtrl* pUnitCtrl = GetUnitCtrl();
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCodeUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallIn(raEval2nd);
	pUnitCtrl->SetUnitIndexCurrent(m_pCalc->GetCurrUnit());
	pUnitCtrl->ConvertUnitSE2ndConWallOut(raEval2nd);

	return TRUE;
}

bool ISeisEvalInterfaceImpl::ReCalcSeisEval2ndRsltConWallFiberTHRAngle(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData)
{
	if ( !m_pCalc->ReCalcSeisEval2ndRsltConWallFiberTHRAngle(InData, rData) ) return false;

	return true;
}

bool ISeisEvalInterfaceImpl::ReCalcSeisEval2ndRsltConWallFiberTHShear(const T_EVAL2ND_WALL_POS& InData, OUT T_EVAL2ND_WALL_POS& rData)
{
    if (!m_pCalc->ReCalcSeisEval2ndRsltConWallFiberTHShear(InData, rData)) return false;

    return true;
}

BOOL ISeisEvalInterfaceImpl::GetLevelSeisEval2ndConFramTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nDof, OUT UINT& runLevelI, OUT UINT& runLevelJ)
{
	return m_pCalc->GetLevelSeisEval2ndConFramTH(ElemK, ThisK, nTimeIndex, nDof, runLevelI, runLevelJ);
}

BOOL ISeisEvalInterfaceImpl::GetColmAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	return m_pCalc->GetColmAxialStrainListTHFiber(stElemK, nMatlType, ThisK, iTimeIdx, nStepType, isRatio, mapValue);
}

BOOL ISeisEvalInterfaceImpl::GetWallAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	return m_pCalc->GetWallAxialStrainListTHFiber(stElemK, nMatlType, ThisK, iTimeIdx, nStepType, isRatio, mapValue);
}

BOOL ISeisEvalInterfaceImpl::GetColmAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	return m_pCalc->GetColmAxialStrainListPOFiber(stElemK, nMatlType, PolcK, iStep, isRatio, mapValue);
}

BOOL ISeisEvalInterfaceImpl::GetWallAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	return m_pCalc->GetWallAxialStrainListPOFiber(stElemK, nMatlType, PolcK, iStep, isRatio, mapValue);
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltConBeam() 
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltConBeam();	
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltConColm()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltConColm();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltConWall()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltConWall();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltStlBeam()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltStlBeam();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltStlColm()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltStlColm();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltStlPanz()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltStlPanz();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltStlBrac()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltStlBrac();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltSrcBeam()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltSrcBeam();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltSrcColm()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltSrcColm();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltSrcBeam4Memb()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltSrcBeam4Memb();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEval1stRsltSrcColm4Memb()
{
    CSeisEvalPrint Print;
    return Print.Print1stRsltSrcBeam4Memb();
}

BOOL ISeisEvalInterfaceImpl::PrintSeisEvalLinRsltInfill()
{
    CSeisEvalPrint Print;
    return Print.PrintLinRsltInfWall();
}

void ISeisEvalInterfaceImpl::WriteMessageToAnalMsgWindow(const CString& strTxt)
{
    GActiveAnalysisViewer();
    GDisplayAnalString(strTxt);
}

double ISeisEvalInterfaceImpl::GetUnitFactorCode2Cur(int nUnitType)
{
    // Code -> Current 단위계로 변환////////////////////////////////////////////////////////	
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_UNIT_INDEX CodeUnit = m_pCalc->GetCodeUnit();
    return pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(CodeUnit.nBase_Force, CodeUnit.nBase_Length, nUnitType,  1.0);
}

double ISeisEvalInterfaceImpl::GetUnitFactorCur2Code(int nUnitType)
{
    // Current -> Code 단위계로 변환////////////////////////////////////////////////////////	
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_UNIT_INDEX CodeUnit = m_pCalc->GetCodeUnit();
    return pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(CodeUnit.nBase_Force, CodeUnit.nBase_Length, nUnitType,  1.0);
}

void ISeisEvalInterfaceImpl::ConvertPhgt2CodeUnit(OUT T_PHGT_D& rPhgtD)
{
    // Current단위계로 변환////////////////////////////////////////////////////////	
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    CCurUnitSaver UnitSave(TRUE);
    T_UNIT_INDEX CodeUnit = m_pCalc->GetCodeUnit();
    pDoc->m_pUnitCtrl->ConvertUnitPhgtIn(rPhgtD);
    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
    pDoc->m_pUnitCtrl->ConvertUnitPhgtOut(rPhgtD);
    UnitSave.Restore();
}

void ISeisEvalInterfaceImpl::ConvertUnitPhgt_Code2Curr(OUT T_PHGT_D& rPhgtD)
{
    // Current단위계로 변환////////////////////////////////////////////////////////	
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    CCurUnitSaver UnitSave(TRUE);
    T_UNIT_INDEX CodeUnit = m_pCalc->GetCodeUnit();
    pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
    pDoc->m_pUnitCtrl->ConvertUnitPhgtIn(rPhgtD);
    UnitSave.Restore();
    pDoc->m_pUnitCtrl->ConvertUnitPhgtOut(rPhgtD);
}

void ISeisEvalInterfaceImpl::ConvertIehp2CodeUnit(OUT T_IEHP_D& rIehpD)
{
	// Current단위계로 변환////////////////////////////////////////////////////////	
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CCurUnitSaver UnitSave(TRUE);
	T_UNIT_INDEX CodeUnit = m_pCalc->GetCodeUnit();
	pDoc->m_pUnitCtrl->ConvertUnitIehpIn(rIehpD, rIehpD.AllProp.PROP, TRUE);
	pDoc->m_pUnitCtrl->ConvertUnitIehpIn(rIehpD, rIehpD.AllSubProp.PROP, FALSE);
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	pDoc->m_pUnitCtrl->ConvertUnitIehpOut(rIehpD, rIehpD.AllProp.PROP, TRUE);
	pDoc->m_pUnitCtrl->ConvertUnitIehpOut(rIehpD, rIehpD.AllSubProp.PROP, FALSE);
	UnitSave.Restore();
}

void ISeisEvalInterfaceImpl::ConvertUnitIehp_Code2Curr(OUT T_IEHP_D& rIehpD)
{
	// Current단위계로 변환////////////////////////////////////////////////////////	
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CCurUnitSaver UnitSave(TRUE);
	T_UNIT_INDEX CodeUnit = m_pCalc->GetCodeUnit();
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	pDoc->m_pUnitCtrl->ConvertUnitIehpIn(rIehpD, rIehpD.AllProp.PROP, TRUE);
	pDoc->m_pUnitCtrl->ConvertUnitIehpIn(rIehpD, rIehpD.AllSubProp.PROP, FALSE);
	UnitSave.Restore();
	pDoc->m_pUnitCtrl->ConvertUnitIehpOut(rIehpD, rIehpD.AllProp.PROP, TRUE);
	pDoc->m_pUnitCtrl->ConvertUnitIehpOut(rIehpD, rIehpD.AllSubProp.PROP, FALSE);
}

T_SEIS_PO_ANAL_RESULT_D ISeisEvalInterfaceImpl::ConvertPORslt2CodeUnit(const T_SEIS_PO_ANAL_RESULT_D& PORslt)
{
    // Code단위계로 변환////////////////////////////////////////////////////////	
    T_SEIS_PO_ANAL_RESULT_D rPORslt = PORslt;

    double dFocF = GetUnitFactorCur2Code(D_UNITSYS_BASE_FORCE);
    double dMomF = GetUnitFactorCur2Code(D_UNITSYS_BASE_MOMENT);
    double dLenF = GetUnitFactorCur2Code(D_UNITSYS_BASE_LENGTH);
    for (int i=0; i<2; ++i)
    {
        rPORslt.dForce[i][DOF_FX] *= dFocF;
        rPORslt.dForce[i][DOF_FY] *= dFocF;
        rPORslt.dForce[i][DOF_FZ] *= dFocF;
        rPORslt.dForce[i][DOF_MX] *= dMomF;
        rPORslt.dForce[i][DOF_MY] *= dMomF;
        rPORslt.dForce[i][DOF_MZ] *= dMomF;

        rPORslt.dDisp[i][DOF_FX]  *= dLenF;

        rPORslt.dD1Pos[i][DOF_FX] *= dLenF;		
        rPORslt.dD1Neg[i][DOF_FX] *= dLenF;		

        rPORslt.dP1Neg[i][DOF_FX] *= dFocF;
        rPORslt.dP1Neg[i][DOF_FY] *= dFocF;
        rPORslt.dP1Neg[i][DOF_FZ] *= dFocF;
        rPORslt.dP1Neg[i][DOF_MY] *= dMomF;
        rPORslt.dP1Neg[i][DOF_MZ] *= dMomF;

        rPORslt.dP1Pos[i][DOF_FX] *= dFocF;
        rPORslt.dP1Pos[i][DOF_FY] *= dFocF;
        rPORslt.dP1Pos[i][DOF_FZ] *= dFocF;
        rPORslt.dP1Pos[i][DOF_MY] *= dMomF;
        rPORslt.dP1Pos[i][DOF_MZ] *= dMomF;
    }
    return rPORslt;
}

CUnitCtrl* ISeisEvalInterfaceImpl::GetUnitCtrl()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if (pDoc == nullptr) 
    {
        return nullptr;
    }
    return pDoc->m_pUnitCtrl;
}

CSeisEvalResult* ISeisEvalInterfaceImpl::GetSEResult()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if (pDoc == nullptr) 
    {
        return nullptr;
    }
    return pDoc->m_pPostCtrl->GetSeisEvalResult();
}

