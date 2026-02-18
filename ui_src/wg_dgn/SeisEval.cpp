#include "stdafx.h"
#include "SeisEval.h"

#include "..\wg_base\ArrayUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\DbToolPmcv.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\ColumnDividerByInfillStrut.h"
#include "..\wg_db\SeisEvalResult.h"
#include "..\wg_db\SeisEvalDataTool.h"
#include "..\wg_db\ThisInfo.h"
#include "..\wg_db\PoverInfo.h"

#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_acs\Acs_ServiceDef.h"
#include "..\wg_rcs\Rcs_ServiceDef.h"

#include "DgnDataCtrl.h"
#include "DgnProgressDlg.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

enum EN_LOADTYPE_TH
{
	EN_LOADTYPE_NORMAL = 0,
	EN_LOADTYPE_MAX,
	EN_LOADTYPE_MIN,
	EN_LOADTYPE_ALL
};

using namespace std;
#include "..\MIT_Lib\rptostream.h"
#include <direct.h>

CSeisEval::CSeisEval(void)
{
    m_pDoc = CDBDoc::GetDocPoint();
    if ( m_pDoc == nullptr ) { ASSERT(0); return; }	

    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(m_CurrUnit);
    m_CodeUnit = m_CurrUnit;

    m_ColmDivByInfill.MakeData();	
}

CSeisEval::~CSeisEval(void)
{
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CurrUnit);
}

BOOL CSeisEval::InitializeHingeConBeam(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();
	SetHingeCalcParam(Param);

    rPhgtD.Initialize();
    rPhgtD.nASInteractionType = GetPhgtPQInteractionType();
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPhgtD);

	SetInitPhgtPropComponentCon(DOF_FZ, Param.abDOF[DOF_FZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
	SetInitPhgtPropComponentCon(DOF_MY, Param.abDOF[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeConColm(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();
	SetHingeCalcParam(Param);

    rPhgtD.Initialize();
    rPhgtD.nASInteractionType = GetPhgtPQInteractionType();
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_PMM, rPhgtD);	
    GetInitialHingeYieldSurface(rPhgtD.YieldSurface);

    SetInitPhgtPropComponentCon(DOF_FX, Param.abDOF[DOF_FX], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
    SetInitPhgtPropComponentCon(DOF_FY, Param.abDOF[DOF_FY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);	
    SetInitPhgtPropComponentCon(DOF_FZ, Param.abDOF[DOF_FZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
    SetInitPhgtPropComponentCon(DOF_MY, Param.abDOF[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL,    rPhgtD);
    SetInitPhgtPropComponentCon(DOF_MZ, Param.abDOF[DOF_MZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL,    rPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeConWall(const T_HINGE_CALC_PARAM& Param, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();
    SetHingeCalcParam(Param);

    rPhgtD.Initialize();
    rPhgtD.nASInteractionType = GetPhgtPQInteractionType();
    rPhgtD.nWallType  = 0;
    rPhgtD.nHingeType = 0; // skeleton.
    SetPhgtProp(D_PHGT_ELEM_WALL, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_PM, rPhgtD);	
    GetInitialHingeYieldSurface(rPhgtD.YieldSurface);

    SetInitPhgtPropComponentCon(DOF_FX, Param.abDOF[DOF_FX], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);	
    SetInitPhgtPropComponentCon(DOF_FZ, Param.abDOF[DOF_FZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
    SetInitPhgtPropComponentCon(DOF_MY, Param.abDOF[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_3EIL,    rPhgtD);	

    return TRUE;
}

BOOL CSeisEval::InitializeHingeStlBeam(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rPhgtD.Initialize();	
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_STL, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPhgtD);

    SetInitPhgtPropComponentStl(DOF_FZ, abDof[DOF_FZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rPhgtD);
    SetInitPhgtPropComponentStl(DOF_MY, abDof[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeStlColm(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rPhgtD.Initialize();	
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_STL, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_PMM, rPhgtD);

    SetInitPhgtPropComponentStl(DOF_FX, abDof[DOF_FX], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
    SetInitPhgtPropComponentStl(DOF_FY, abDof[DOF_FY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
    SetInitPhgtPropComponentStl(DOF_FZ, abDof[DOF_FZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
    SetInitPhgtPropComponentStl(DOF_MY, abDof[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rPhgtD);
    SetInitPhgtPropComponentStl(DOF_MZ, abDof[DOF_MZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeStlBrac(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rPhgtD.Initialize();	
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_STL, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPhgtD);

    SetInitPhgtPropComponentStl(DOF_FX, abDof[DOF_FX], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);
    SetInitPhgtPropComponentStl(DOF_MY, abDof[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rPhgtD);
    SetInitPhgtPropComponentStl(DOF_MZ, abDof[DOF_MZ], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeStlTrus(const BOOL abDof[6], OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rPhgtD.Initialize();	
    SetPhgtProp(D_PHGT_ELEM_TRUSS, D_PHGT_MATL_STL, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPhgtD);
    SetInitPhgtPropComponentCon(DOF_FX, abDof[DOF_FX], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeSrcBeam(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    /// 전단에 대한 힌지를 만들지 않음.
    rEncasePhgtD.Initialize();	
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rEncasePhgtD);
    SetInitPhgtPropComponentStl(DOF_MY, abDof[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rEncasePhgtD);

    rFilledPhgtD.Initialize();
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_STL, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rFilledPhgtD);
    SetInitPhgtPropComponentStl(DOF_MY, abDof[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rFilledPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeSrcColm(const BOOL abDof[6], OUT T_PHGT_D& rEncasePhgtD, OUT T_PHGT_D& rFilledPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    /// 기준에 명시가 되어 있지 않아 전단에 대한 힌지를 만들지 않음.
    rEncasePhgtD.Initialize();	
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_PMM, rEncasePhgtD);
    SetInitPhgtPropComponentStl(DOF_FX, abDof[DOF_FX], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rEncasePhgtD);
    SetInitPhgtPropComponentStl(DOF_MY, abDof[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rEncasePhgtD);

    rFilledPhgtD.Initialize();
    SetPhgtProp(D_PHGT_ELEM_BEAM, D_PHGT_MATL_STL, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_PMM, rFilledPhgtD);
    SetInitPhgtPropComponentStl(DOF_FX, abDof[DOF_FX], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_ELASTIC, rFilledPhgtD);
    SetInitPhgtPropComponentStl(DOF_MY, abDof[DOF_MY], D_PHGT_LOCATION_IJ, D_SKCV_FEMA, TRUE, INITSTIFF_6EIL, rFilledPhgtD);

    return TRUE;
}

BOOL CSeisEval::InitializeHingeInfill(const BOOL abDof[6], OUT T_PHGT_D& rPartPhgtD, OUT T_PHGT_D& rFullPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rPartPhgtD.Initialize();
    SetPhgtProp(D_PHGT_ELEM_TRUSS, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPartPhgtD);
    SetInitPhgtPropComponentInfill(DOF_FX, abDof[DOF_FX], D_PHGT_FEMA_IFST_WAIST, rPartPhgtD);

    rFullPhgtD.Initialize();
    SetPhgtProp(D_PHGT_ELEM_TRUSS, D_PHGT_MATL_CON, D_PHGT_DEFINITION_MTHETA_LUMP, INTER_NONE, rPartPhgtD);
    SetInitPhgtPropComponentInfill(DOF_FX, abDof[DOF_FX], D_PHGT_FEMA_IFST_INFILL, rFullPhgtD);

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeConBeam(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();
	SetHingeCalcParam(Param);

    int nCalcMethod = D_PHGT_CALC_USER;
    BOOL bSymIJ = FALSE;

	//memcpy(rPhgtD.bDOF, Param.abDOF, 6*sizeof(BOOL));

    if ( Param.abDOF[DOF_FZ])
    {
        rPhgtD.Skcv[DOF_FZ].nCalcMethod  = nCalcMethod;
        rPhgtD.Skcv[DOF_FZ].bSymmetricIJ = bSymIJ;
        rPhgtD.Skcv[DOF_FZ].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FZ] = rPhgtD.Skcv[DOF_FZ];
        CalcFzHingePropConBeam(ElemK, PORslt, Param.enHingeType, Param.dCBRatio, rPhgtD.Skcv[DOF_FZ].FEMA, rPhgtD.SkcvSub[DOF_FZ].FEMA);
    }
	
    if ( Param.abDOF[DOF_MY])
    {
        rPhgtD.Skcv[DOF_MY].nCalcMethod  = nCalcMethod;
        rPhgtD.Skcv[DOF_MY].bSymmetricIJ = bSymIJ;
        rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
        CalcMyHingePropConBeam(ElemK, PORslt, Param.enHingeType, Param.dCBRatio, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
    }	
	
    return TRUE;
}

void CSeisEval::SetHingeParamPMM(T_ELEM_K ElemK, int nUpdateMatl, const EN_NLANALTYPE& enNLType, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if (pDoc == nullptr) { ASSERT(0); return; }

    T_ELEM_D ElemD;
    if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return; }

    T_PHGE_K PhgeK;
    PhgeK.key.objK = ElemK;
    PhgeK.key.type = pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp) ? 2 : 1;

    BOOL bSelDof[6] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE};	
    rPhgtD.YieldSurface.nCalcMethod  = D_PHGT_CALC_AUTO;

    _DB_PMCV_USER_MATLSE MatlSE;
    GetMatlSE(ElemK, EN_EVALSTEP_2ND, EN_EVALMATL_AVG, MatlSE);

    CDbToolPmcv Pmcv;		
    Pmcv.SetPOHingeMode(TRUE);
    Pmcv.SetWallIDMode(TRUE);
    Pmcv.SetUseMatlSE(nUpdateMatl == D_IUHP_MATL_SEIS ? TRUE : FALSE, MatlSE, GetPmcvModeSE(enNLType));
    Pmcv.Cal_PmcvPhge_FEMA_SeisEval(PhgeK, rPhgtD, bSelDof);

    rPhgtD.YieldSurface.nCalcMethod  = D_PHGT_CALC_USER;
    rPhgtD.YieldSurface.bSymmetricIJ = FALSE;
    rPhgtD.YieldSurface.YieldSurfaceProp.bSymmetricYZ       = FALSE;
    rPhgtD.YieldSurface.YieldSurfaceProp.bSymmetricPlusMnus = FALSE;
    rPhgtD.YieldSurface.YieldSurfaceProp.bSymmetricY = FALSE;
    rPhgtD.YieldSurface.YieldSurfaceProp.bSymmetricZ = FALSE;
    rPhgtD.YieldSurface.YieldSurfacePropSub = rPhgtD.YieldSurface.YieldSurfaceProp;
}

BOOL CSeisEval::CalcUpdateHingeConColm(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();
	SetHingeCalcParam(Param);

    int nCalcMethod = D_PHGT_CALC_USER;
    BOOL bSymIJ = FALSE;
    
	//memcpy(rPhgtD.bDOF, Param.abDOF, 6*sizeof(BOOL));
	
	if ( Param.abDOF[DOF_FX] )
    {		
        T_PHGT_SKCV& rSkcvI = rPhgtD.Skcv[DOF_FX];
        T_PHGT_SKCV& rSkcvJ = rPhgtD.SkcvSub[DOF_FX];

		rSkcvI.nCalcMethod = nCalcMethod;
        rSkcvI.bSymmetricIJ = bSymIJ;  
        rSkcvI.nInitStiffMethod = INITSTIFF_ELASTIC;
        rSkcvJ = rSkcvI;
        CalcFxHingePropConColm(ElemK, PORslt, rSkcvI.FEMA, rSkcvJ.FEMA);

        const CString& strEnv = CProduct::GetTestEnvValue(_ULS(CONCOLM_ASYM));
        if (strEnv == _ULS(yes))
        {
            CalcAsymmetricStiffFxHingeConColm(ElemK, rSkcvI, rSkcvJ);
        }
    }	

	if ( Param.abDOF[DOF_FY] )
    {
        rPhgtD.Skcv[DOF_FY].nCalcMethod = nCalcMethod;
        rPhgtD.Skcv[DOF_FY].bSymmetricIJ = bSymIJ;  
        rPhgtD.Skcv[DOF_FY].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FY] = rPhgtD.Skcv[DOF_FY];
        CalcFyzHingePropConColm(ElemK, EN_DIR_Y, PORslt, Param.enHingeType, rPhgtD.Skcv[DOF_FY].FEMA, rPhgtD.SkcvSub[DOF_FY].FEMA);
    }
	
	if ( Param.abDOF[DOF_FZ] )
    {
        rPhgtD.Skcv[DOF_FZ].nCalcMethod = nCalcMethod;
        rPhgtD.Skcv[DOF_FZ].bSymmetricIJ = bSymIJ;  
        rPhgtD.Skcv[DOF_FZ].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FZ] = rPhgtD.Skcv[DOF_FZ];
        CalcFyzHingePropConColm(ElemK, EN_DIR_Z, PORslt, Param.enHingeType, rPhgtD.Skcv[DOF_FZ].FEMA, rPhgtD.SkcvSub[DOF_FZ].FEMA);
    }	

	if ( Param.abDOF[DOF_MY] )
    {
        rPhgtD.Skcv[DOF_MY].nCalcMethod = D_PHGT_CALC_AUTO;
        rPhgtD.Skcv[DOF_MY].bSymmetricIJ = FALSE;  
        rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
        CalcMyzHingePropConColm(ElemK, EN_DIR_Y, PORslt, Param.enHingeType, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
    }		

	if ( Param.abDOF[DOF_MZ] )
    {
        rPhgtD.Skcv[DOF_MZ].nCalcMethod = D_PHGT_CALC_AUTO;
        rPhgtD.Skcv[DOF_MZ].bSymmetricIJ = FALSE;  
        rPhgtD.Skcv[DOF_MZ].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MZ] = rPhgtD.Skcv[DOF_MZ];
        CalcMyzHingePropConColm(ElemK, EN_DIR_Z, PORslt, Param.enHingeType, rPhgtD.Skcv[DOF_MZ].FEMA, rPhgtD.SkcvSub[DOF_MZ].FEMA);
    }
	
    if ( IsIgnoreMomentHingeProp() )
    {
        rPhgtD.nInteractionType = 0;
        rPhgtD.bDOF[4] = FALSE;
        rPhgtD.bDOF[5] = FALSE;
    }

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeConWall(T_ELEM_K ElemK, const T_HINGE_CALC_PARAM& Param, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();
	SetHingeCalcParam(Param);

    int nCalcMethod = D_PHGT_CALC_USER;
    BOOL bSymIJ = FALSE;

	//memcpy(rPhgtD.bDOF, abDof, 6*sizeof(BOOL));

    if (Param.abDOF[DOF_FX])
    {
        T_PHGT_SKCV& rSkcvI = rPhgtD.Skcv[DOF_FX];
        T_PHGT_SKCV& rSkcvJ = rPhgtD.SkcvSub[DOF_FX];

        rSkcvI.nCalcMethod = nCalcMethod;
        rSkcvI.bSymmetricIJ = bSymIJ;  
        rSkcvI.nInitStiffMethod = INITSTIFF_ELASTIC;
        rSkcvJ = rSkcvI;
        CalcFxHingePropConWall(ElemK, rSkcvI.FEMA, rSkcvJ.FEMA);

        const CString& strEnv = CProduct::GetTestEnvValue(_ULS(CONWALL_ASYM));
        if (strEnv == _ULS(yes))
        {
            CalcAsymmetricStiffFxHingeConWall(ElemK, rSkcvI, rSkcvJ);
        }
    }

    if (Param.abDOF[DOF_FZ])
    {
        rPhgtD.Skcv[DOF_FZ].nCalcMethod = nCalcMethod;
        rPhgtD.Skcv[DOF_FZ].bSymmetricIJ = bSymIJ;  
        rPhgtD.Skcv[DOF_FZ].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FZ] = rPhgtD.Skcv[DOF_FZ];
        CalcFzHingePropConWall(ElemK, PORslt, rPhgtD.Skcv[DOF_FZ].FEMA, rPhgtD.SkcvSub[DOF_FZ].FEMA);
    }

    if (Param.abDOF[DOF_MY])
    {
        rPhgtD.Skcv[DOF_MY].nCalcMethod  = D_PHGT_CALC_AUTO;
        rPhgtD.Skcv[DOF_MY].bSymmetricIJ = FALSE;
        rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_3EIL;
        rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
        CalcMyHingePropConWall(ElemK, PORslt, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
    }

    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }

    // [PMS 5388-189] 벽체의 약축에 대한 modeling parameter는 기둥을 따라갑니다. (/w 이상현 prof.)
    if (ElemD.nSubType == 2) /// PLATE type의 벽체는 약축에 대한 힌지도 update.
    {
        if (Param.abDOF[DOF_FY])
        {
            rPhgtD.Skcv[DOF_FY].nCalcMethod = nCalcMethod;
            rPhgtD.Skcv[DOF_FY].bSymmetricIJ = bSymIJ;  
            rPhgtD.Skcv[DOF_FY].nInitStiffMethod = INITSTIFF_ELASTIC;
            rPhgtD.SkcvSub[DOF_FY] = rPhgtD.Skcv[DOF_FY];
            CalcFyHingePropConWall(ElemK, PORslt, rPhgtD.Skcv[DOF_FY].FEMA, rPhgtD.SkcvSub[DOF_FY].FEMA);
        }

        if (Param.abDOF[DOF_MZ])
        {
            rPhgtD.Skcv[DOF_MZ].nCalcMethod  = D_PHGT_CALC_AUTO;
            rPhgtD.Skcv[DOF_MZ].bSymmetricIJ = FALSE;
            rPhgtD.Skcv[DOF_MZ].nInitStiffMethod = INITSTIFF_6EIL;
            rPhgtD.SkcvSub[DOF_MZ] = rPhgtD.Skcv[DOF_MZ];
            CalcMzHingePropConWall(ElemK, PORslt, rPhgtD.Skcv[DOF_MZ].FEMA, rPhgtD.SkcvSub[DOF_MZ].FEMA);
        }
    }

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeStlBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    int nCalcMethod = D_PHGT_CALC_USER;
    BOOL bSymIJ = FALSE;

    if (abDof[DOF_FZ])
    {
        rPhgtD.Skcv[DOF_FZ].nCalcMethod = nCalcMethod;
        rPhgtD.Skcv[DOF_FZ].bSymmetricIJ = bSymIJ;  
        rPhgtD.Skcv[DOF_FZ].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FZ] = rPhgtD.Skcv[DOF_FZ];
        //CalcFzHingePropStlBeam(ElemK, PORslt, rPhgtD.Skcv[DOF_FZ].FEMA, rPhgtD.SkcvSub[DOF_FZ].FEMA);
    }

    if (abDof[DOF_MY])
    {
        rPhgtD.Skcv[DOF_MY].nCalcMethod  = nCalcMethod;
        rPhgtD.Skcv[DOF_MY].bSymmetricIJ = bSymIJ;
        rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
        CalcMyHingePropStlBeam(ElemK, PORslt, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
    }

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeStlColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    int nCalcMethod = D_PHGT_CALC_USER;
    BOOL bSymIJ = FALSE;

    if (abDof[DOF_FX])
    {
        rPhgtD.Skcv[DOF_FX].nCalcMethod = nCalcMethod;
        rPhgtD.Skcv[DOF_FX].bSymmetricIJ = bSymIJ;  
        rPhgtD.Skcv[DOF_FX].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FX] = rPhgtD.Skcv[DOF_FX];
        CalcFxHingePropStlColm(ElemK, rPhgtD.Skcv[DOF_FX].FEMA, rPhgtD.SkcvSub[DOF_FX].FEMA);
    }

    if (abDof[DOF_FY])
    {
        rPhgtD.Skcv[DOF_FY].nCalcMethod = nCalcMethod;
        rPhgtD.Skcv[DOF_FY].bSymmetricIJ = bSymIJ;  
        rPhgtD.Skcv[DOF_FY].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FY] = rPhgtD.Skcv[DOF_FY];
        //CalcFyzHingePropConColm(ElemK, EN_DIR_Y, PORslt, rPhgtD.Skcv[DOF_FY].FEMA, rPhgtD.SkcvSub[DOF_FY].FEMA);
    }

    if (abDof[DOF_FZ])
    {
        rPhgtD.Skcv[DOF_FZ].nCalcMethod = nCalcMethod;
        rPhgtD.Skcv[DOF_FZ].bSymmetricIJ = bSymIJ;  
        rPhgtD.Skcv[DOF_FZ].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FZ] = rPhgtD.Skcv[DOF_FZ];
        //CalcFyzHingePropConColm(ElemK, EN_DIR_Z, PORslt, rPhgtD.Skcv[DOF_FZ].FEMA, rPhgtD.SkcvSub[DOF_FZ].FEMA);
    }

    if (abDof[DOF_MY])
    {
        rPhgtD.Skcv[DOF_MY].nCalcMethod = D_PHGT_CALC_AUTO;
        rPhgtD.Skcv[DOF_MY].bSymmetricIJ = FALSE;  
        rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
        CalcMyzHingePropStlColm(ElemK, EN_DIR_Y, PORslt, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
    }

    if (abDof[DOF_MZ])
    {
        rPhgtD.Skcv[DOF_MZ].nCalcMethod = D_PHGT_CALC_AUTO;
        rPhgtD.Skcv[DOF_MZ].bSymmetricIJ = FALSE;  
        rPhgtD.Skcv[DOF_MZ].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MZ] = rPhgtD.Skcv[DOF_MZ];
        CalcMyzHingePropStlColm(ElemK, EN_DIR_Z, PORslt, rPhgtD.Skcv[DOF_MZ].FEMA, rPhgtD.SkcvSub[DOF_MZ].FEMA);
    }

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeStlBrac(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    int nCalcMethod = D_PHGT_CALC_USER;
    BOOL bSymIJ = FALSE;
    int nInitStiffMethod = 4; // Elastic

    if (abDof[DOF_FX])
    {
        T_PHGT_SKCV& rSkcv = rPhgtD.Skcv[DOF_FX];
        rSkcv.nCalcMethod  = nCalcMethod;
        rSkcv.bSymmetricIJ = bSymIJ;  
        rSkcv.nInitStiffMethod = INITSTIFF_ELASTIC; // Elastic
        rPhgtD.SkcvSub[DOF_FX] = rSkcv;
    }

    CSeisEvalDataTool Tool;
    BOOL bEndConstraint = Tool.IsEndConstraintRotation(ElemK); 
    if (!bEndConstraint)
    {
        if (abDof[DOF_FX]) CalcFxHingePropStlBrac(ElemK, rPhgtD.Skcv[DOF_FX].FEMA, rPhgtD.SkcvSub[DOF_FX].FEMA);	
    }
    else
    {
        if (abDof[DOF_FX])
        {
            CalcFxHingePropStlColm(ElemK, rPhgtD.Skcv[DOF_FX].FEMA, rPhgtD.SkcvSub[DOF_FX].FEMA);	
        }
        if (abDof[DOF_MY])
        {
            rPhgtD.Skcv[DOF_MY].nCalcMethod = D_PHGT_CALC_AUTO;
            rPhgtD.Skcv[DOF_MY].bSymmetricIJ = FALSE;  
            rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_6EIL;
            rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
            CalcMyzHingePropStlColm(ElemK, EN_DIR_Y, PORslt, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
        }
        if (abDof[DOF_MZ])
        {
            rPhgtD.Skcv[DOF_MZ].nCalcMethod = D_PHGT_CALC_AUTO;
            rPhgtD.Skcv[DOF_MZ].bSymmetricIJ = FALSE;  
            rPhgtD.Skcv[DOF_MZ].nInitStiffMethod = INITSTIFF_6EIL;
            rPhgtD.SkcvSub[DOF_MZ] = rPhgtD.Skcv[DOF_MY];
            CalcMyzHingePropStlColm(ElemK, EN_DIR_Z, PORslt, rPhgtD.Skcv[DOF_MZ].FEMA, rPhgtD.SkcvSub[DOF_MZ].FEMA);
        }
    }

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeStlTrus(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    if (abDof[DOF_FX])
    {
        rPhgtD.Skcv[DOF_FX].nCalcMethod  = D_PHGT_CALC_USER;
        rPhgtD.Skcv[DOF_FX].bSymmetricIJ = FALSE;  
        rPhgtD.Skcv[DOF_FX].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FX] = rPhgtD.Skcv[DOF_FX];
        if ( !CalcFxHingePropStlBrac(ElemK, rPhgtD.Skcv[DOF_FX].FEMA, rPhgtD.SkcvSub[DOF_FX].FEMA) )
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeSrcBeam(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    if (abDof[DOF_MY])
    {
        rPhgtD.Skcv[DOF_MY].nCalcMethod  = D_PHGT_CALC_USER;
        rPhgtD.Skcv[DOF_MY].bSymmetricIJ = FALSE;
        rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
        CalcMyHingePropSrcBeam(ElemK, PORslt, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
    }

    return TRUE;
}

BOOL CSeisEval::CalcUpdateHingeSrcColm(T_ELEM_K ElemK, const BOOL abDof[6], const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT T_PHGT_D& rPhgtD)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    int nCalcMethod = D_PHGT_CALC_USER;
    BOOL bSymIJ = FALSE;

    if (abDof[DOF_FX])
    {
        rPhgtD.Skcv[DOF_FX].nCalcMethod  = nCalcMethod;
        rPhgtD.Skcv[DOF_FX].bSymmetricIJ = bSymIJ;
        rPhgtD.Skcv[DOF_FX].nInitStiffMethod = INITSTIFF_ELASTIC;
        rPhgtD.SkcvSub[DOF_FX] = rPhgtD.Skcv[DOF_FX];
        CalcFxHingePropSrcColm(ElemK, rPhgtD.Skcv[DOF_FX].FEMA, rPhgtD.SkcvSub[DOF_FX].FEMA);
    }

    if (abDof[DOF_MY])
    {
        rPhgtD.Skcv[DOF_MY].nCalcMethod  = nCalcMethod;
        rPhgtD.Skcv[DOF_MY].bSymmetricIJ = bSymIJ;
        rPhgtD.Skcv[DOF_MY].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MY] = rPhgtD.Skcv[DOF_MY];
        CalcMyzHingePropSrcColm(ElemK, EN_DIR_Y, PORslt, rPhgtD.Skcv[DOF_MY].FEMA, rPhgtD.SkcvSub[DOF_MY].FEMA);
    }

    if (abDof[DOF_MZ])
    {
        rPhgtD.Skcv[DOF_MZ].nCalcMethod  = nCalcMethod;
        rPhgtD.Skcv[DOF_MZ].bSymmetricIJ = bSymIJ;
        rPhgtD.Skcv[DOF_MZ].nInitStiffMethod = INITSTIFF_6EIL;
        rPhgtD.SkcvSub[DOF_MZ] = rPhgtD.Skcv[DOF_MZ];
        CalcMyzHingePropSrcColm(ElemK, EN_DIR_Z, PORslt, rPhgtD.Skcv[DOF_MZ].FEMA, rPhgtD.SkcvSub[DOF_MZ].FEMA);
    }

    return TRUE;
}

BOOL CSeisEval::PrintSeisEval1stRsltSrcBeam4Memb()
{
    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aElemK;
    int nSizeBeam = Tool.GetEvalSrcBeamKeyList(aElemK);
    if (nSizeBeam < 1)
    {
        AfxMessageBox(_LS(IDS_DGN_NOT_EXIST_BEAM_EVAL_RESULT));
        return FALSE;
    }

    T_LCOM_K_LIST aLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, aLcomK);
    int nSizeLcom = static_cast<int>(aLcomK.GetSize());
    if (nSizeLcom < 1)
    {
        AfxMessageBox(_LS(IDS_DGN_NOT_EXIST_LCOM_EVAL));
        return FALSE;
    }

    CString strPathName = m_pDoc->GetPathName();

    CFileCtrl FileCtrl(strPathName);
    CString strOutFile = _T("");	
    if (CDBLib::ActivateGenNXAnal())
        strOutFile.Format(_T("%s_SrcBeam1st.xout"), FileCtrl.GetFilePathAndNameWithoutExtension());
    else
        strOutFile.Format(_T("%s_SrcBeam1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());

    rptwofstream fout(strOutFile, ios::trunc);

    T_UNIT_NAMEFACT CurUnitName;
    m_pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);

    fout<<_T("=====================================================")<<endl
        <<_T("     SRC BEAM SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<CurUnitName.strForce[m_CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[m_CurrUnit.nBase_Length]<<endl
        <<_T("=====================================================")<<endl
        <<_T("  STOR       SECT   ELEM   PL")<<endl
        <<_T("-----------------------------------------------------")<<endl;

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_ELEM_D ElemD;
    T_SECT_D SectD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strSect = _T("");

    T_EVAL1ST_STLBEAM EvalBeam;
    for (int i=0; i<nSizeBeam; ++i)
    {
        T_ELEM_K ElemK = aElemK[i];
        if (!CalcSeisEval1stRsltSrcBeam(ElemK, EvalBeam)) { ASSERT(FALSE); continue; }

        m_pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }

        if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }
        if (!m_pDoc->m_pAttrCtrl->GetSectDesign(ElemD.elpro, SectD)) { ASSERT(FALSE); return FALSE; }

        strElem.Format(_T("%6d"), ElemK);
        strStor.Format(_T("%6s"), StorD.StoryName);
        strSect.Format(_T("%10s"), SectD.SName);

        fout<<strStor<<_T(" ")<<strSect<<_T(" ")<<strElem<<_T("  ")<<GetPerformLevel4TextOut(EvalBeam.enLevel)<<endl;		
    }

    fout<<_T("-----------------------------------------------------")<<endl;
    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("SRC Beam Seismic Evaluate - 1ST STEP")), TRUE); 


    return TRUE;
}

BOOL CSeisEval::PrintSeisEval1stRsltSrcColm4Memb()
{
    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalSrcColmKeyList(aColmK);
    if (nSizeColm < 1)
    {
        AfxMessageBox(_LS(IDS_DGN_NOT_EXIST_COLM_EVAL_RESULT));
        return FALSE;
    }

    T_LCOM_K_LIST aLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, aLcomK);
    int nSizeLcom = static_cast<int>(aLcomK.GetSize());
    if (nSizeLcom < 1)
    {
        AfxMessageBox(_LS(IDS_DGN_NOT_EXIST_LCOM_EVAL));
        return FALSE;
    }

    CString strPathName = m_pDoc->GetPathName();

    CFileCtrl FileCtrl(strPathName);
    CString strOutFile = _T("");
    if (CDBLib::ActivateGenNXAnal())
        strOutFile.Format(_T("%s_SrcColm1st.xout"), FileCtrl.GetFilePathAndNameWithoutExtension());
    else
        strOutFile.Format(_T("%s_SrcColm1st.out"), FileCtrl.GetFilePathAndNameWithoutExtension());

    rptwofstream fout(strOutFile, ios::trunc);

    T_UNIT_NAMEFACT CurUnitName;
    m_pDoc->m_pUnitCtrl->GetUnitNameCurrent(CurUnitName);

    fout<<_T("=====================================================")<<endl
        <<_T("     SRC COLUMN SEISMIC EVALUATION RESULT (1ST STEP)")<<endl
        <<_T("    [UNIT] = ")<<CurUnitName.strForce[m_CurrUnit.nBase_Force]<<_T(", ")<<CurUnitName.strLength[m_CurrUnit.nBase_Length]<<endl
        <<_T("=====================================================")<<endl
        <<_T("  STOR       SECT   ELEM   PL")<<endl
        <<_T("-----------------------------------------------------")<<endl;

    T_STOR_K StorK;
    T_STOR_D StorD;
    T_ELEM_D ElemD;
    T_SECT_D SectD;
    T_LCOM_D LcomD;
    CString strElem = _T("");
    CString strStor = _T("");
    CString strSect = _T("");

    T_EVAL1ST_STLCOLM EvalColm;
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
        if (!CalcSeisEval1stRsltSrcColm(ElemK, EvalColm)) { ASSERT(FALSE); continue; }

        m_pDoc->m_pAttrCtrl->GetStorKeyByElemKey(StorK, ElemK);
        if (!m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD))  { ASSERT(FALSE); continue; }

        if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }
        if (!m_pDoc->m_pAttrCtrl->GetSectDesign(ElemD.elpro, SectD)) { ASSERT(FALSE); return FALSE; }

        strElem.Format(_T("%6d"), ElemK);
        strStor.Format(_T("%6s"), StorD.StoryName);
        strSect.Format(_T("%10s"), SectD.SName);

        fout<<strStor<<_T(" ")<<strSect<<_T(" ")<<strElem<<_T("  ")<<GetPerformLevel4TextOut(EvalColm.enLevel)<<endl;		
    }

    fout<<_T("-----------------------------------------------------")<<endl;
    fout.close();

    CDBLib::RunTextEditor(strOutFile, _STR(_T("SRC Column Seismic Evaluate - 1ST STEP")), TRUE); 

    return TRUE;
}

EN_LEVEL CSeisEval::GetPerformLevelForcCtrl(const double dDCR[3])
{
    if (dDCR[EN_LEVEL_CP] > 1.0) { return EN_LEVEL_CL; }
    if (dDCR[EN_LEVEL_LS] > 1.0) { return EN_LEVEL_CP; }
    if (dDCR[EN_LEVEL_IO] > 1.0) { return EN_LEVEL_LS; }
    return EN_LEVEL_IO;
}

CString CSeisEval::GetPerformLevel4TextOut(EN_LEVEL enLevel)
{
    switch (enLevel)
    {
    case EN_LEVEL_IO:      return _LS(IDS_TB_DGN_IO);
    case EN_LEVEL_LS:      return _LS(IDS_TB_DGN_LS);
    case EN_LEVEL_CP:      return _LS(IDS_TB_DGN_CP);
    case EN_LEVEL_CL:      return _LSX(**);
    case EN_LEVEL_INVALID: return _LSX(NO);
    default: ASSERT(FALSE); break;
    }
    return _T("  ");
}

EN_LEVEL CSeisEval::GetPerformLevel(const double dDCR[3])
{
    if (dDCR[EN_LEVEL_CP] > 1.0) return EN_LEVEL_CL;
    if (dDCR[EN_LEVEL_LS] > 1.0) return EN_LEVEL_CP;
    if (dDCR[EN_LEVEL_IO] > 1.0) return EN_LEVEL_LS;
    return EN_LEVEL_IO;
}

void CSeisEval::SetPerformLevel(OUT T_EVAL1ST_DCR& rDCR)
{
    rDCR.enLevel = GetPerformLevel(rDCR.dDCR);
}

void CSeisEval::SetPerformLevelForcCtrl(OUT T_EVAL1ST_DCR& rDCR)
{
    rDCR.enLevel = GetPerformLevelForcCtrl(rDCR.dDCR);
}

CString CSeisEval::GetValueForm(int nTotalDigit, int nUnderDigit, double dValue)
{
    BOOL bChange=FALSE;
    if (nUnderDigit==0)
    {
        if(fabs(dValue) >= 1.0*pow(10,nTotalDigit))	bChange = TRUE;
        else if(fabs(dValue) < 1.0*pow(10,0))	      bChange = TRUE;
    }
    else
    {
        if (fabs(dValue) >= 1.0*pow(10,nTotalDigit-nUnderDigit-1))	bChange = TRUE;
        else if(fabs(dValue) < 1.0*pow(10,-nUnderDigit))			bChange = TRUE;
    }

    // Add by ZINU.('00.9.5).
    // cRCS_Zero 보다 작은 경우 0.00~으로 출력 by eklee
    if (fabs(dValue) < 1.0e-7)	bChange = FALSE;

    CString strSymbol;
    CString strValue;
    if (nUnderDigit==0 && !bChange)
    {
        strSymbol.Format(_T("%%%dd"), nTotalDigit);
        strValue.Format(strSymbol, int(dValue));
    }
    else
    {
        if(bChange)	strSymbol.Format(_T("%%%d.%de"), nTotalDigit, nUnderDigit);
        else		strSymbol.Format(_T("%%%d.%df"), nTotalDigit, nUnderDigit);
        strValue.Format(strSymbol, dValue);
    }
    return strValue;
}

double CSeisEval::CalcVnmaxWall(const T_WALL_SHR_PARAM& WallShr)
{
    const T_CON_SHR_PARAM& Shr = WallShr.Comm;
    const double dfck = Shr.dFck;
    double dVnmax = 5.0 * sqrt(dfck) / 6.0 * Shr.dBv * Shr.dDv;
        
    if (WallShr.bVnmaxEQ)
    {
		const double dHw = WallShr.dTotalHw;
		const double dLw = WallShr.dLw;
		const double dAlphac = [dHw, dLw]()
		{
			const double dRatHwLw = dgn::SafeDiv(dHw, dLw);
			if (dgn::LE(dRatHwLw, 1.5))
			{
				return 1.0 / 4.0;
			}
			if (dgn::ME(dRatHwLw, 2.0))
			{
				return 1.0 / 6.0;
			}

			return 1.0 / 4.0 - 1.0 / 6.0 * (dRatHwLw - 1.5);
		}();

		const double dhw = Shr.dBv;
		const double dRhoN = Shr.dAsv / (dhw * Shr.dSv);
		const double dAcv = dhw * dLw; /// gross area. (see SEISMIC AND WIND DESIGN IOF CONCRETE BUILDINGS)
		const double dVnmaxEQ = dAcv * dAlphac * sqrt(dfck) + dAcv * dRhoN * Shr.dFys;
		dVnmax = min(dVnmax, dVnmaxEQ);
    }

    return dVnmax;
}


double CSeisEval::CalcVsmaxWall(const T_WALL_SHR_PARAM& WallShr, double dVc)
{
    const double dVnmax = CalcVnmaxWall(WallShr);
    return dVnmax - dVc; // <- max(dVnmax - dVc, 0.0); [GEN-9515]    
}

void CSeisEval::SetCodeUnit()
{
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);

    m_pDoc->m_pMatlDB->Create_RebarData(m_CodeUnit.nBase_Length, m_CodeUnit.nBase_Force, TRUE);	
    MakeRebarSizeMap4CodeUnit();
}

CSeisEvalResult* CSeisEval::GetSEResult()
{
    return m_pDoc->m_pPostCtrl->GetSeisEvalResult();
}

void CSeisEval::SetUphgParam(const double adTable[6], const double adParam[6], OUT T_UPHG_PARAM& rParam)
{
    memcpy(rParam.adTable, adTable, sizeof(double)*6);
    memcpy(rParam.adParam, adParam, sizeof(double)*6);
}

T_ELEM_K CSeisEval::GetElemShowKey(T_ELEM_K ElemK)
{
    CSeisEvalDataTool Tool;
    return Tool.GetElemShowKey(ElemK);    
}

int CSeisEval::GetLcomKeyListLinEval(T_ELEM_K ElemK, UINT unType, OUT T_LCOM_K_LIST& raLcomK)
{
    int nLcomType = D_LCOMTYPE_LINEAR_EVAL;

    T_LCOM_K_LIST aLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(nLcomType, aLcomK);
    const INT_PTR nSize = aLcomK.GetSize();

    T_LCOM_D LcomD;
    if (unType == EN_EVAL_GRAV)
    {
        for ( INT_PTR i=0; i<nSize; ++i )
        {
            T_LCOM_K CurLcomK = aLcomK[i];
            if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, CurLcomK, LcomD)) continue;
            if (LcomD.nActive == EN_LNEVAL_GRAV) { raLcomK.Add(CurLcomK); }
        }
    }
    else if (unType == EN_EVAL_SEIS)
    {
        T_SLCM_D SlcmD;
        if (m_pDoc->m_pAttrCtrl->GetSlcm(ElemK, SlcmD))
        {
            switch (SlcmD.nType)
            {
            case 0: // special load effect.
                {
                    for ( INT_PTR i=0; i<nSize; ++i )
                    {
                        T_LCOM_K CurLcomK = aLcomK[i];
                        if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, CurLcomK, LcomD)) continue;
                        if (LcomD.nActive == EN_LNEVAL_SPEC) { raLcomK.Add(CurLcomK); }
                    }
                }
                break;
            case 1: // vertical load effect.
                {
                    for ( INT_PTR i=0; i<nSize; ++i )
                    {
                        T_LCOM_K CurLcomK = aLcomK[i];
                        if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, CurLcomK, LcomD)) continue;
                        if (LcomD.nActive == EN_LNEVAL_VERT) { raLcomK.Add(CurLcomK); }
                    }
                }
                break;
            default: ASSERT(0);
                break;
            }		
        }
        else			
        {
            for ( INT_PTR i=0; i<nSize; ++i )
            {
                T_LCOM_K CurLcomK = aLcomK[i];
                if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, CurLcomK, LcomD)) continue;
                if (LcomD.nActive == EN_LNEVAL_SEIS ) { raLcomK.Add(CurLcomK); }
            }
        }
    }

    return static_cast<int>(raLcomK.GetSize());
}

int CSeisEval::GetLcomKeyList1stEval(T_ELEM_K ElemK, OUT T_LCOM_K_LIST& raLcomK)
{
    raLcomK.RemoveAll();

    T_LCOM_K_LIST aLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, aLcomK);
    
    const INT_PTR nSize = aLcomK.GetSize();

    T_LCOM_D LcomD;    
    T_SLCM_D SlcmD;
    bool bVertLcom = false;
    if ( m_pDoc->m_pAttrCtrl->GetSlcm(ElemK, SlcmD) )
    {
        if ( SlcmD.nType == 1 )
        {
            bVertLcom = true;
        }
    }

    if ( bVertLcom )
    {
        for ( INT_PTR i=0; i<nSize; ++i )
        {
            T_LCOM_K CurLcomK = aLcomK[i];
            if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcomK, LcomD) ) continue;
            if ( LcomD.nActive == EN_EVAL_VERT ) { raLcomK.Add(CurLcomK); }
        }
    }
    else
    {
        for ( INT_PTR i=0; i<nSize; ++i )
        {
            T_LCOM_K CurLcomK = aLcomK[i];
            if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, CurLcomK, LcomD) ) continue;
            if ( LcomD.nActive == EN_EVAL_SEIS ) { raLcomK.Add(CurLcomK); }
        }
    }
       
    return static_cast<int>(raLcomK.GetSize());
}

BOOL CSeisEval::GetSeisParam(const T_SEIS_D& SeisD, OUT T_SEIS_ITEM& rSeisItem)
{
    rSeisItem.Initialize();

    BOOL bValid = FALSE;
    switch (SeisD.nCodeType)
    {
    case KBC_E_2005:
        {
            const T_SEIS_KBC2005& SeisCode = SeisD.KBC2005;
            rSeisItem.dSDS  = SeisCode.dSds;
            rSeisItem.dSD1  = SeisCode.dSd1;
            rSeisItem.dIe   = SeisCode.dImportanceFactor;
            bValid = TRUE;
        }
        break;
    case KBC_E_2009:
        {
            const T_SEIS_KBC2009& SeisCode = SeisD.KBC2009;
            rSeisItem.dSDS  = SeisCode.dSds;
            rSeisItem.dSD1  = SeisCode.dSd1;
            rSeisItem.dIe   = SeisCode.dImportanceFactor;
            bValid = TRUE;
        }
        break;
    case KBC_E_2016:
        {
            const T_SEIS_KBC2015& SeisCode = SeisD.KBC2015;
            rSeisItem.dSDS  = SeisCode.dSds;
            rSeisItem.dSD1  = SeisCode.dSd1;
            rSeisItem.dIe   = SeisCode.dImportanceFactor;
            bValid = TRUE;
        }
        break;
    case KDS_E_2019:
        {
            const T_SEIS_KDS2019& SeisCode = SeisD.KDS2019;
            rSeisItem.dSDS  = SeisCode.dSds;
            rSeisItem.dSD1  = SeisCode.dSd1;
            rSeisItem.dIe   = SeisCode.dImportanceFactor;
            bValid = TRUE;
        }
        break;
    default:
        break;
    }
    return bValid;
}

BOOL CSeisEval::GetSpfcParam(const T_SPFC_D& SpfcD, OUT T_SEIS_ITEM& rSeisItem)
{
    rSeisItem.Initialize();

    BOOL bValid = FALSE;
    switch (SpfcD.nSpecCodeType)
    {
    case D_SPFC_CODE_KBC2005:
        {
            const T_SPFC_KBC_2005& SpfcCode = SpfcD.CodeParam.KBC2005;
            rSeisItem.dSDS = SpfcCode.dSds;
            rSeisItem.dSD1 = SpfcCode.dSd1;
            rSeisItem.dIe  = SpfcCode.dIe;
            bValid = TRUE;
        }
        break;
    case D_SPFC_CODE_KBC2009:
        {
            const T_SPFC_KBC_2009& SpfcCode = SpfcD.CodeParam.KBC2009;
            rSeisItem.dSDS = SpfcCode.dSds;
            rSeisItem.dSD1 = SpfcCode.dSd1;
            rSeisItem.dIe  = SpfcCode.dIe;
            bValid = TRUE;
        }
        break;
    case D_SPFC_CODE_KBC2015:
        {
            const T_SPFC_KBC_2016& SpfcCode = SpfcD.CodeParam.KBC2016;
            rSeisItem.dSDS = SpfcCode.dSds;
            rSeisItem.dSD1 = SpfcCode.dSd1;
            rSeisItem.dIe  = SpfcCode.dIe;
            bValid = TRUE;
        }
        break;
    case D_SPFC_CODE_KDS2019:
        {
            const T_SPFC_KDS2019& SpfcCode = SpfcD.CodeParam.KDS2019;
            rSeisItem.dSDS = SpfcCode.dSds;
            rSeisItem.dSD1 = SpfcCode.dSd1;
            rSeisItem.dIe  = SpfcCode.dIe;
            bValid = TRUE;
        }
        break;
    default:
        break;
    }

    return bValid;
}

double CSeisEval::GetConLambda(const T_MATD_D& MatdD)
{
    if (MatdD.Type != _LSX(C)) { return 1.0; }

    return MatdD.Data1.Design.bLambda ? MatdD.Data1.Design.dLambda : 1.0;
}

double CSeisEval::GetScaleUpFactor4MFact()
{
    T_SEME_D SemeD;
    if (m_pDoc->m_pAttrCtrl2->GetSeme(SemeD))
    {
        return SemeD.data.eval.dMFactor;
    }
    return 1.0;
}


BOOL CSeisEval::GetUseSeisHookWall(T_ELEM_K ElemK)
{
    /// 벽체는 내진갈고리 상세 적용 없음.
    return FALSE;
}

void CSeisEval::GetColmClearHeight(T_ELEM_K ElemK, OUT double& rdh0y, OUT double& rdh0z)
{
    rdh0y = rdh0z = 0.0;
    T_ELEM_K ElemShowK = GetElemShowKey(ElemK);
    T_SECH_D SechD;
    if (m_pDoc->m_pAttrCtrl->GetSech(ElemShowK, SechD))
    {
        rdh0y = SechD.dh0y;
        rdh0z = SechD.dh0z;
        return;
    }

    double dL = CMembCtrl::GetMembCtrl()->GetMembLengByIncludeElem(ElemK);
    rdh0y = rdh0z = dL;
}


EN_STLBEAM_SWAYTYPE CSeisEval::GetStlBeamSwayType(T_ELEM_K ElemK)
{	
    T_ELEM_K ElemShowK = GetElemShowKey(ElemK);
    T_LENG_D LengD;
    if (m_pDoc->m_pAttrCtrl->GetLeng(ElemShowK, LengD))
    {
        return LengD.bNotUseBLu ? EN_STLBEAM_NONSWAY : EN_STLBEAM_SWAY;
    }

    T_DSTL_D DstlD;
    if (!m_pDoc->m_pAttrCtrl->GetDstl(DstlD)) DstlD.Initialize();

    return DstlD.bAllFrameLaterallyBraced ? EN_STLBEAM_NONSWAY : EN_STLBEAM_SWAY;
}


UINT CSeisEval::GetPhgeType(const T_ELEM_K ElemK)
{
    T_ELEM_D ElemD;
    if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return 0; }

    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(ElemK);
    switch (nMembType)
    {
    case D_MBTP_BEAM:
    case D_MBTP_COLUMN:
        return D_PHGE_BEAMCOLUMN;
    case D_MBTP_WALL:
        return D_PHGE_WALL;
    case D_MBTP_BRACE:
        return m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp) ? D_PHGE_TRUSS : D_PHGE_BEAMCOLUMN;
    default:
        ASSERT(0);
        return 0;
    }    
}

int CSeisEval::GetPhgtPQInteractionType()
{
    // PO Global Control > RC Code > 내진성능평가 선택 시 PQ Interaction을 초기값으로 사용
    T_POGD_D PogdD;
    if (!m_pDoc->m_pAttrCtrl->GetPogd(PogdD)) PogdD.Initialize();
    return PogdD.bUseAutoCalcReference;
}

BOOL CSeisEval::GetInfillParam(T_ELEM_K ElemK, OUT T_MSIS_D& rMsisD)
{
    rMsisD.Initialize();

    T_ELEM_K ElemShowK = GetElemShowKey(ElemK);
    T_MISA_D MisaD;
    if (!m_pDoc->m_pAttrCtrl2->GetMisa(ElemShowK, MisaD)) { return FALSE; }

    return m_pDoc->m_pAttrCtrl2->GetMsis(MisaD.MsisK, rMsisD);	
}

bool CSeisEval::GetElemMatlSectCon(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SECM_D& rSecmD)
{
    CSeisEvalDataTool Tool;
    return Tool.GetElemMatlSectCon(ElemK, rSectD, rMatdD, rSecmD);
}

bool CSeisEval::GetElemMatlSectStl(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SESM_D& rSesmD)
{
    CSeisEvalDataTool Tool;
    return Tool.GetElemMatlSectStl(ElemK, rSectD, rMatdD, rSesmD);    
}

bool CSeisEval::GetElemMatlSectSrc(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SESM_D& rSesmD, OUT T_SECM_D& rSecmD)
{
    CSeisEvalDataTool Tool;
    return Tool.GetElemMatlSectSrc(ElemK, rSectD, rMatdD, rSesmD, rSecmD);
}

bool CSeisEval::GetElemMatlSectMas(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_SEMM_D& rSemmD)
{
    CSeisEvalDataTool Tool;
    return Tool.GetElemMatlSectMas(ElemK, rSectD, rMatdD, rSemmD);
}

bool CSeisEval::GetElemMatlThik(T_ELEM_K ElemK, OUT T_THIK_D& rThikD, OUT T_MATD_D& rMatdD, OUT T_SECM_D& rSecmD)
{
    CSeisEvalDataTool Tool;
    return Tool.GetElemMatlThik(ElemK, rThikD, rMatdD, rSecmD);
}

bool CSeisEval::GetElemMatlSectCon(T_ELEM_K ElemK, OUT T_SECT_D& rSectD, OUT T_MATD_D& rMatdD, OUT T_PDCM_D& rPdcmD)
{
	CSeisEvalDataTool Tool;
	return Tool.GetElemMatlSectCon(ElemK, rSectD, rMatdD, rPdcmD);
}

bool CSeisEval::GetElemMatlThik(T_ELEM_K ElemK, OUT T_THIK_D& rThikD, OUT T_MATD_D& rMatdD, OUT T_PDCM_D& rPdcmD)
{
	CSeisEvalDataTool Tool;
	return Tool.GetElemMatlThik(ElemK, rThikD, rMatdD, rPdcmD);
}

T_SECT_SECTBASE_D CSeisEval::GetSectBaseEndI(const T_SECT_D& SectD) 
{
    return SectD.SectBefore.SectI; 
}

T_SECT_SECTBASE_D CSeisEval::GetSectBaseEndJ(const T_SECT_D& SectD) 
{
    return SectD.SectBefore.nStype == D_SECT_TYPE_TAPERED ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI; 
}

EN_DIR CSeisEval::GetPairDirV(const EN_DIR& enDirM)
{
    switch (enDirM)
    {
    case EN_DIR_Y: { return EN_DIR_Z; }
    case EN_DIR_Z: { return EN_DIR_Y; }
    default: { ASSERT(0); return EN_DIR_Y; }
    }
}

EN_WALL_FORC_POS CSeisEval::GetWallForcPos(const EN_MEMBEND& enEnd)
{
    switch (enEnd)
    {
    case EN_MEMBEND_I: { return EN_WALL_FORC_POS_BOT; }
    case EN_MEMBEND_J: { return EN_WALL_FORC_POS_TOP; }
    default: { ASSERT(0); return EN_WALL_FORC_POS_BOT; }
    }
}

EN_BEAM_POS CSeisEval::GetBeamPos(const EN_MEMBEND& enEnd)
{
    switch (enEnd)
    {
    case EN_MEMBEND_I: { return EN_BEAM_POS_I; }
    case EN_MEMBEND_J: { return EN_BEAM_POS_J; }
    default: { ASSERT(0); return EN_BEAM_POS_I; }
    }
}

BOOL CSeisEval::GetEvalStrb1st(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRB_D& rStrbDisp, OUT T_STRB_D& rStrbForc)
{
    rStrbDisp.Initialize();
    rStrbForc.Initialize();

    T_LCOM_D LcomD;
    if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, LcomK, LcomD)) { return FALSE; }			
    if (LcomD.nActive == EN_EVAL_NONE) { return FALSE; }

    T_STRB_D StrbDispMin;
    if (!pMembCtrl->GetStrbDgn(ElemK, D_LOADCASE_COMB_SEISMIC, LcomK, &rStrbDisp, &StrbDispMin)) { ASSERT(0);	return FALSE; }
    if (!pMembCtrl->GetStrbForcCtrl_SeisEval(ElemK, LcomD, rStrbForc)) { ASSERT(0); return FALSE; }

    return TRUE;
}

BOOL CSeisEval::GetEvalStrt1st(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRT_D& rStrtDisp, OUT T_STRT_D& rStrtForc)
{
    rStrtDisp.Initialize();
    rStrtForc.Initialize();

    T_LCOM_D LcomD;
    if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, LcomK, LcomD)) { return FALSE; }			
    if (LcomD.nActive == EN_EVAL_NONE ) { return FALSE; }

    T_STRT_D StrtDispMin;
    if (!pMembCtrl->GetStrtDgn(ElemK, D_LOADCASE_COMB_SEISMIC, LcomK, &rStrtDisp, &StrtDispMin)) { ASSERT(0);	return FALSE; }
    if (!pMembCtrl->GetStrtForcCtrl_SeisEval(ElemK, LcomD, rStrtForc)) { ASSERT(0); return FALSE; }

    return TRUE;
}

BOOL CSeisEval::GetEvalStrw1st(CMembCtrl* pMembCtrl, UINT unInternalWallID, T_LCOM_K LcomK, OUT T_STRW_D& rStrwDisp, OUT T_STRW_D& rStrwForc)
{
    rStrwDisp.Initialize();
    rStrwForc.Initialize();

    T_LCOM_D LcomD;
    if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, LcomK, LcomD)) { return FALSE; }			
    if (LcomD.nActive == EN_EVAL_NONE ) { return FALSE; }

    T_STRW_D StrwDispMin;
    if (!pMembCtrl->GetStrwDgn(unInternalWallID, D_LOADCASE_COMB_SEISMIC, LcomK, &rStrwDisp, &StrwDispMin)) { ASSERT(0);	return FALSE; }
    if (!pMembCtrl->GetStrwForcCtrl_SeisEval(unInternalWallID, LcomD, rStrwForc)) { ASSERT(0); return FALSE; }

    return TRUE;
}

BOOL CSeisEval::GetEvalStrbLin(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRB_D& rStrbD)
{
    rStrbD.Initialize();

    T_LCOM_D LcomD;
    if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_LINEAR_EVAL, LcomK, LcomD)) { return FALSE; }			
    if (LcomD.nActive == EN_LNEVAL_NONE) { return FALSE; }

    T_STRB_D StrbMin;
    if (!pMembCtrl->GetStrbDgn(ElemK, D_LOADCASE_COMB_LINEAR_EVAL, LcomK, &rStrbD, &StrbMin)) { ASSERT(0);	return FALSE; }

    return TRUE;
}

BOOL CSeisEval::GetEvalStrtLin(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, T_LCOM_K LcomK, OUT T_STRT_D& rStrtD)
{
    rStrtD.Initialize();

    T_LCOM_D LcomD;
    if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_LINEAR_EVAL, LcomK, LcomD)) { return FALSE; }			
    if (LcomD.nActive == EN_LNEVAL_NONE ) { return FALSE; }

    T_STRT_D StrtMin;
    if (!pMembCtrl->GetStrtDgn(ElemK, D_LOADCASE_COMB_LINEAR_EVAL, LcomK, &rStrtD, &StrtMin)) { ASSERT(0);	return FALSE; }

    return TRUE;
}

BOOL CSeisEval::GetEvalStrwLin(CMembCtrl* pMembCtrl, UINT unInternalWallID, T_LCOM_K LcomK, OUT T_STRW_D& rStrwD)
{
    rStrwD.Initialize();

    T_LCOM_D LcomD;
    if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_LINEAR_EVAL, LcomK, LcomD)) { return FALSE; }			
    if (LcomD.nActive == EN_LNEVAL_NONE ) { return FALSE; }

    T_STRW_D StrwMin;
    if (!pMembCtrl->GetStrwDgn(unInternalWallID, D_LOADCASE_COMB_LINEAR_EVAL, LcomK, &rStrwD, &StrwMin)) { ASSERT(0);	return FALSE; }

    return TRUE;
}

BOOL CSeisEval::GetEvalStrb2nd4AddStep(T_ELEM_K ElemK, T_POLC_K PolcK, int nAddStepIdx, T_STRB_D &rStrbD)
{
    rStrbD.Initialize();
    if (nAddStepIdx < 0)  { ASSERT(0); return FALSE; }

    T_POAS_D PoasD;
    if (!m_pDoc->m_pAttrCtrl->GetPoas(PoasD))  { ASSERT(0); return FALSE; }
    int nStep = PoasD.aStep[nAddStepIdx].nRefStep-1;
    double dDistRatio = PoasD.aStep[nAddStepIdx].dDistRatio;

    T_STRB_D RefStrbD;
    if (!m_pDoc->m_pPostCtrl->GetStrbPushover(PolcK, nStep, ElemK, RefStrbD)) return FALSE;
    T_STRB_D NxtStrbD;
    if (!m_pDoc->m_pPostCtrl->GetStrbPushover(PolcK, nStep+1, ElemK, NxtStrbD)) return FALSE;
    GetStrbAddtionStep(RefStrbD, NxtStrbD, dDistRatio, rStrbD);

    return TRUE;
}

BOOL CSeisEval::GetEvalStrw2nd4AddStep(T_ELEM_K ElemK, T_POLC_K PolcK, int nAddStepIdx, T_STRW_D &rStrwD)
{
    rStrwD.Initialize();
    if (nAddStepIdx < 0)  { ASSERT(0); return FALSE; }

    T_POAS_D PoasD;
    if (!m_pDoc->m_pAttrCtrl->GetPoas(PoasD))  { ASSERT(0); return FALSE; }
    int nStep = PoasD.aStep[nAddStepIdx].nRefStep-1;
    double dDistRatio = PoasD.aStep[nAddStepIdx].dDistRatio;

    T_STRW_D RefStrwD;
    if (!m_pDoc->m_pPostCtrl->GetStrwPushover(PolcK, nStep, ElemK, RefStrwD)) return FALSE;
    T_STRW_D NxtStrwD;
    if (!m_pDoc->m_pPostCtrl->GetStrwPushover(PolcK, nStep+1, ElemK, NxtStrwD)) return FALSE;
    GetStrwAddtionStep(RefStrwD, NxtStrwD, dDistRatio, rStrwD);

    return TRUE;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_BEAM& raEval2nd)
{
    BOOL bCalcOK = FALSE;
    T_EVAL2ND_PHNE EvalPhneD;
    int nPhgeType = GetPhgeType(ElemK);
    for (int i=0; i<2; ++i) // I, J
    {
        T_EVAL2ND_BEAM_POS &PosRslt = (i==0) ? raEval2nd.EndI : raEval2nd.EndJ;
        int nPart = (i==0) ? 0 : 7;
        for ( int nDOF = 0 ; nDOF < 6 ; ++nDOF )
        {
            EvalPhneD.Initialize();
            if ( !m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD) ) { continue; }

            bCalcOK = TRUE;
            PosRslt.bPhne[nDOF] = TRUE;
            PosRslt.PhneD[nDOF] = EvalPhneD;
        }
    }

    return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_COLM& raEval2nd)
{
    BOOL bCalcOK = FALSE;
    T_EVAL2ND_PHNE EvalPhneD;
    int nPhgeType = GetPhgeType(ElemK);
    UINT enMaxLevel = EN_LEVEL_IO;
    for (int i=0; i<2; ++i) // End-I/ End-J
    {
        T_EVAL2ND_COLM_POS &PosRslt = (i==0) ? raEval2nd.PosI : raEval2nd.PosJ;
        int nPart = (i==0) ? 0 : 7;
        for (int nDOF=0; nDOF<6; ++nDOF)
        {
            EvalPhneD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD)) { continue; }

            bCalcOK = TRUE;
            PosRslt.bPhne[nDOF] = TRUE;
            PosRslt.PhneD[nDOF] = EvalPhneD;
            enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);            
        }
    }
    raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

    return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_WALL& raEval2nd)
{
    BOOL bCalcOK = FALSE;
    T_EVAL2ND_PHNE EvalPhneD;
    int nPhgeType = GetPhgeType(ElemK);
    UINT enMaxLevel = EN_LEVEL_IO;
    for (int i=0; i<2; ++i) // End-I/ End-J.
    {
        T_EVAL2ND_WALL_POS &PosRslt = (i==0) ? raEval2nd.PosJ : raEval2nd.PosI; // Wall은 nPart=0이 Top.
        int nPart = (i==0) ? 0 : 7;
        for (int nDOF=0; nDOF<6; ++nDOF)
        {
            EvalPhneD.Initialize();
            if ( !m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD) ) { continue; }

            bCalcOK = TRUE;
            PosRslt.bPhne[nDOF] = TRUE;
            PosRslt.PhneD[nDOF] = EvalPhneD;
            enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);
        }
    }
    raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

    return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConColmFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	BOOL bCalcOK = FALSE;
	T_EVAL2ND_PHNE EvalPhneD;
	int nPhgeType = GetPhgeType(ElemK);
	UINT enMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<2; ++i ) // End-I/ End-J
	{
		T_EVAL2ND_COLM_POS &PosRslt = (i==0) ? raEval2nd.PosI : raEval2nd.PosJ;
		int nPart = (i==0) ? 0 : 7;
		for ( int nDOF=0; nDOF<6; ++nDOF )
		{
			EvalPhneD.Initialize();
			if ( !m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD) ) { continue; }

			bCalcOK = TRUE;
			PosRslt.bPhne[nDOF] = TRUE;
			PosRslt.PhneD[nDOF] = EvalPhneD;
			enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);
		}
	}
	raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

	return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConWallFiber(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	BOOL bCalcOK = FALSE;
	T_EVAL2ND_PHNE EvalPhneD;
	int nPhgeType = GetPhgeType(ElemK);
	UINT enMaxLevel = EN_LEVEL_IO;
	for ( int i=0; i<2; ++i ) // End-I/ End-J.
	{
		T_EVAL2ND_WALL_POS &PosRslt = (i==0) ? raEval2nd.PosJ : raEval2nd.PosI; // Wall은 nPart=0이 Top.
		int nPart = (i==0) ? 0 : 7;
		for ( int nDOF=0; nDOF<6; ++nDOF )
		{
			EvalPhneD.Initialize();
			if ( !m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD) ) { continue; }

			bCalcOK = TRUE;
			PosRslt.bPhne[nDOF] = TRUE;
			PosRslt.PhneD[nDOF] = EvalPhneD;
			enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);
		}
	}
	raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

	return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubStlBeam(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBEAM& raEval2nd)
{
    BOOL bCalcOK = FALSE;
    T_EVAL2ND_PHNE EvalPhneD;
    int nPhgeType = GetPhgeType(ElemK);
    UINT enMaxLevel = EN_LEVEL_IO;
    for (int i=0; i<2; ++i) // I, J
    {
        T_EVAL2ND_STLBEAM_POS &PosRslt = (i==0) ? raEval2nd.EndI : raEval2nd.EndJ;
        int nPart = (i==0) ? 0 : 7;
        for ( int nDOF = 0 ; nDOF < 6 ; ++nDOF )
        {
            EvalPhneD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD)) continue;
                        
            bCalcOK = TRUE;
            PosRslt.bPhne[nDOF] = TRUE;
            PosRslt.PhneD[nDOF] = EvalPhneD;
            enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);
        }
    }
    raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

    return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubStlColm(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLCOLM& raEval2nd)
{
    BOOL bCalcOK = FALSE;
    T_EVAL2ND_PHNE EvalPhneD;
    int nPhgeType = GetPhgeType(ElemK);
    UINT enMaxLevel = EN_LEVEL_IO;
    for (int i=0; i<2; ++i) // End-I / End-J.
    {
        T_EVAL2ND_STLCOLM_POS &PosRslt = (i==0) ? raEval2nd.PosI : raEval2nd.PosJ;
        int nPart = (i==0) ? 0 : 7;
        for (int nDOF=0; nDOF<6; ++nDOF)
        {
            EvalPhneD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD)) continue;

            bCalcOK = TRUE;
            PosRslt.bPhne[nDOF] = TRUE;
            PosRslt.PhneD[nDOF] = EvalPhneD;
            enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);
        }
    }
    raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

    return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubStlBrac(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_STLBRAC& raEval2nd)
{
    BOOL bCalcOK = FALSE;
    T_EVAL2ND_PHNE EvalPhneD;
    int nPhgeType = GetPhgeType(ElemK);
    UINT enMaxLevel = EN_LEVEL_IO;
    for (int i=0; i<2; ++i) // End-I / End-J.
    {
        T_EVAL2ND_STLBRAC_POS &PosRslt = (i==0) ? raEval2nd.PosI : raEval2nd.PosJ;
        int nPart = (i==0) ? 0 : 7;
        for (int nDOF=0; nDOF<6; ++nDOF)
        {
            EvalPhneD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD)) continue;

            bCalcOK = TRUE;
            PosRslt.bPhne[nDOF] = TRUE;
            PosRslt.PhneD[nDOF] = EvalPhneD;
            enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);
        }
    }
    raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

    return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubInfWall(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT T_EVAL2ND_INFILL& raEval2nd)
{
    BOOL bCalcOK = FALSE;
    T_EVAL2ND_PHNE EvalPhneD;
    int nPhgeType = GetPhgeType(ElemK);
    UINT enMaxLevel = EN_LEVEL_IO;
    for ( int i = 0 ; i < 2 ; ++i ) // Top, Bot
    {
        T_EVAL2ND_INFILL_POS &PosRslt = (i==0) ? raEval2nd.EndI : raEval2nd.EndJ;
        int nPart = (i==0) ? 0 : 7;
        for (int nDOF=0; nDOF<6; ++nDOF)
        {
            EvalPhneD.Initialize();
            if(!m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, nPart, nDOF, EvalPhneD)) continue;

            bCalcOK = TRUE;
            PosRslt.bPhne[nDOF] = TRUE;
            PosRslt.PhneD[nDOF] = EvalPhneD;
            enMaxLevel = max(enMaxLevel, EvalPhneD.enLevel);
        }
    }
    raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

    return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConBeamTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, OUT T_EVAL2ND_BEAM& raEval2nd)
{
	CSeisEvalDataTool DataTool;

	UINT unMaxLevel = 0;
	BOOL bCalcOK = FALSE;	
	for ( int nDOF = 0; nDOF < 6; ++nDOF )
	{
		T_EVAL2ND_BEAM_POS& rPosI = raEval2nd.EndI;
		T_EVAL2ND_BEAM_POS& rPosJ = raEval2nd.EndJ;
		if ( !GetSeisEval2ndRsltSubConFramTH(ElemK, ThisK, nTimeIndex, 1, nDOF, rPosI.PhneD[nDOF], rPosJ.PhneD[nDOF]) ) { continue; }		

		bCalcOK = TRUE;
		rPosI.bPhne[nDOF] = TRUE;
		rPosJ.bPhne[nDOF] = TRUE;		

		UINT unCurLevel = max(raEval2nd.EndI.PhneD[nDOF].enLevel, raEval2nd.EndJ.PhneD[nDOF].enLevel);
		
		unMaxLevel = max(unMaxLevel, unCurLevel);
	}	

	raEval2nd.enLevel = (EN_LEVEL)unMaxLevel;

	return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConColmTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, OUT T_EVAL2ND_COLM& raEval2nd)
{
	UINT unMaxLevel = 0;
	BOOL bCalcOK = FALSE;
	for ( int nDOF = 0; nDOF < 6; ++nDOF )
	{
		T_EVAL2ND_COLM_POS& rPosI = raEval2nd.PosI;
		T_EVAL2ND_COLM_POS& rPosJ = raEval2nd.PosJ;
		if ( !GetSeisEval2ndRsltSubConFramTH(ElemK, ThisK, nTimeIndex, 1, nDOF, rPosI.PhneD[nDOF], rPosJ.PhneD[nDOF]) ) { continue; }

		bCalcOK = TRUE;
		rPosI.bPhne[nDOF] = TRUE;
		rPosJ.bPhne[nDOF] = TRUE;

		UINT unCurLevel = max(rPosI.PhneD[nDOF].enLevel, rPosJ.PhneD[nDOF].enLevel);
		unMaxLevel = max(unMaxLevel, unCurLevel);
	}

	raEval2nd.enLevel = (EN_LEVEL)unMaxLevel;

	return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConColmFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nSerialAgxp, OUT T_EVAL2ND_COLM& raEval2nd)
{
	BOOL bCalcOK = FALSE;
	T_EVAL2ND_PHNE EvalPhneD;	
 	UINT enMaxLevel = EN_LEVEL_IO;
	
	for ( int nDOF=0; nDOF<6; ++nDOF )
	{
		EvalPhneD.Initialize();

		T_EVAL2ND_COLM_POS& rPosI = raEval2nd.PosI;
		T_EVAL2ND_COLM_POS& rPosJ = raEval2nd.PosJ;

        switch (nDOF)
        {        
        case D_DOF_FY: /// fiber의 전단 검토는 힌지에서 자유도를 OFF해도 계산이 되어야 하므로 별도 처리.
        case D_DOF_FZ:        
            if (!GetSeisEval2ndRsltSubConFramTHFiberShear(ElemK, ThisK, nTimeIndex, 1, nDOF, rPosI.PhneD[nDOF], rPosJ.PhneD[nDOF])) { continue; }
            break;
        default:
            if (!GetSeisEval2ndRsltSubConFramTH(ElemK, ThisK, nTimeIndex, 1, nDOF, rPosI.PhneD[nDOF], rPosJ.PhneD[nDOF])) { continue; }
            break;
        }		

		bCalcOK = TRUE;
		rPosI.bPhne[nDOF] = TRUE;
		rPosJ.bPhne[nDOF] = TRUE;

		UINT unCurLevel = max(rPosI.PhneD[nDOF].enLevel, rPosJ.PhneD[nDOF].enLevel);
		enMaxLevel = max(enMaxLevel, unCurLevel);
	}
	raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

	return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConWallFiberTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nSerialAgxp, OUT T_EVAL2ND_WALL& raEval2nd)
{
	BOOL bCalcOK = FALSE;
 	T_EVAL2ND_PHNE EvalPhneD;
 	UINT enMaxLevel = EN_LEVEL_IO;		
	for ( int nDOF=0; nDOF<6; ++nDOF )
	{
		EvalPhneD.Initialize();
		
		T_EVAL2ND_WALL_POS& rPosI = raEval2nd.PosI;
		T_EVAL2ND_WALL_POS& rPosJ = raEval2nd.PosJ;		

        switch (nDOF)
        {
        case D_DOF_FY: /// fiber의 전단 검토는 힌지에서 자유도를 OFF해도 계산이 되어야 하므로 별도 처리.
        case D_DOF_FZ:
            if (!GetSeisEval2ndRsltSubConWallTHFiberShear(ElemK, ThisK, nTimeIndex, nHingeType, nDOF, rPosI, rPosJ)) { continue; }            
            break;
        default:            
            if (!GetSeisEval2ndRsltSubConWallTH(ElemK, ThisK, nTimeIndex, nHingeType, nDOF, rPosI.PhneD[nDOF], rPosJ.PhneD[nDOF])) { continue; }
            break;
        }

		bCalcOK = TRUE;
		rPosI.bPhne[nDOF] = TRUE;
		rPosJ.bPhne[nDOF] = TRUE;

		UINT unCurLevel = max(rPosI.PhneD[nDOF].enLevel, rPosJ.PhneD[nDOF].enLevel);
		enMaxLevel = max(enMaxLevel, unCurLevel);
	}

 	raEval2nd.enLevel = static_cast<EN_LEVEL>(enMaxLevel);

	return bCalcOK;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConFramTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ)
{
	rPhneDI.Initialize();
	rPhneDJ.Initialize();

	auto L_Convert = [nHingeType] (int nDOF, const T_NLHG_D& NlhgD, const T_NLHG_D& TimeD, const T_IEHP_FEMA& IehpFemaD, OUT T_EVAL2ND_PHNE& rPhneD)
	{
		const bool bGetPlus = NlhgD.dDisplacement < 0.0 ? false : true;
		rPhneD.nHingeType = nHingeType;
		rPhneD.nDof = nDOF;
		rPhneD.dForceMoment  = NlhgD.dForceMoment;
		rPhneD.dDisplacement = NlhgD.dDisplacement;
		rPhneD.dTime = TimeD.dDisplacement;  ///CTBPostIehgBeamSummary::GetRecValStr() 참고하여 동일한 time 출력하도록 처리.
		rPhneD.dDuctility = max(fabs(NlhgD.dDuctility_Pos), fabs(NlhgD.dDuctility_Neg));
		rPhneD.dDuctility_Pos = NlhgD.dDuctility_Pos;
		rPhneD.dDuctility_Neg = NlhgD.dDuctility_Neg;		
		rPhneD.dD1_Pos = NlhgD.dD1[0];
		rPhneD.dD1_Neg = NlhgD.dD1[1];

		auto L_GetPerfLevel = [] (double nStatus)
		{
			if ( EQ(nStatus, 11.0) || EQ(nStatus, 14.0) ) return EN_LEVEL_IO;
			if ( EQ(nStatus, 12.0) || EQ(nStatus, 15.0) ) return EN_LEVEL_LS;
			if ( EQ(nStatus, 13.0) || EQ(nStatus, 16.0) ) return EN_LEVEL_CP;
			if ( EQ(nStatus, 17.0) ) return EN_LEVEL_CL;
			return EN_LEVEL_IO;
		};

		const int nIdx = bGetPlus ? IDX_PLS : IDX_MNS;
		rPhneD.dIO = IehpFemaD.dIO[nIdx];
		rPhneD.dLS = IehpFemaD.dLS[nIdx];
		rPhneD.dCP = IehpFemaD.dCP[nIdx];

		UINT unLevelNeg = L_GetPerfLevel(fabs(NlhgD.nHingeStatusLevel_Neg));
		UINT unLevelPos = L_GetPerfLevel(fabs(NlhgD.nHingeStatusLevel_Pos));
		const UINT unLevel =  max(unLevelNeg, unLevelPos);
		
		(nDOF <= DOF_FZ ? rPhneD.enLevel_Force : rPhneD.enLevel_Deform) = unLevel;
		rPhneD.enLevel = unLevel;
	};	

	T_NLHG_D NlhgDI;
	T_NLHG_D NlhgDJ;
	T_NLHG_D TimeDI;
	T_NLHG_D TimeDJ;
	if ( !GetNlghTH(ElemK, ThisK, nTimeIndex, nDof, NlhgDI, NlhgDJ, TimeDI, TimeDJ) ) { return FALSE; }

	T_IEHG_D IehgD;
	if ( !m_pDoc->m_pAttrCtrl->GetIehgByElemK(ElemK, IehgD) ) { return FALSE; }
	T_IEHP_D IehpD;	
	if ( !m_pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD) ) { return FALSE; }
	
	const T_IEHP_FEMA& IehpFemaI = IehpD.AllProp.PROP[nDof].FEMA;
	const T_IEHP_FEMA& IehpFemaJ = [IehpD, nDof] ()
	{
		switch ( IehpD.nHingeLocation[nDof] )
		{
		case 0:
		case 1:
		case 3:
			return IehpD.AllProp.PROP[nDof].FEMA;
		case 2:
			return IehpD.AllProp.PROP[nDof].FEMA.bSymmetric ? IehpD.AllProp.PROP[nDof].FEMA : IehpD.AllSubProp.PROP[nDof].FEMA;
		default:
			ASSERT(0);
			return IehpD.AllProp.PROP[nDof].FEMA;
		}
	}();

	L_Convert(nDof, NlhgDI, TimeDI, IehpFemaI, rPhneDI);
	L_Convert(nDof, NlhgDJ, TimeDJ, IehpFemaJ, rPhneDJ);

	CalcP1FemaTypeIehg(ElemK, nDof, true, rPhneDI.dP1_Neg, rPhneDI.dP1_Pos);
	CalcP1FemaTypeIehg(ElemK, nDof, false, rPhneDJ.dP1_Neg, rPhneDJ.dP1_Pos);	

	if ( nDof <= DOF_FZ )
	{
		double adInitForc[2][6] = { { 0.0, }, };
		double dQGI = 0.0;
		double dQGJ = 0.0;
		T_THGC_D ThgcD;
		if ( m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD) && ThgcD.nInitLoadMethod == 0 && ThgcD.ThisK_DummyInitLoad != 0 )
		{
			CArray<double, double> aTimeStep;
			const T_THIS_K InitThisK = ThgcD.ThisK_DummyInitLoad;
			m_pDoc->m_pAttrCtrl->GetTimeStep(InitThisK, aTimeStep);
			const int nInitTimeIndex = aTimeStep.GetSize();
			if ( !GetNlghTH(ElemK, InitThisK, nInitTimeIndex, nDof, NlhgDI, NlhgDJ, TimeDI, TimeDJ) ) { return FALSE; }

			dQGI = NlhgDI.dForceMoment;
			dQGJ = NlhgDJ.dForceMoment;
		}

		CSeisEvalDataTool Tool;
		Tool.CalcEval2ndForcCtrlActionTH(ElemK, nDof, IehpFemaI, dQGI, rPhneDI);
		rPhneDI.enLevel = rPhneDI.enLevel_Force;
		Tool.CalcEval2ndForcCtrlActionTH(ElemK, nDof, IehpFemaJ, dQGJ, rPhneDJ);
		rPhneDJ.enLevel = rPhneDJ.enLevel_Force;
	}
		
	return TRUE;
}

BOOL CSeisEval::GetSeisEval2ndRsltSubConWallTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nHingeType, int nDof, OUT T_EVAL2ND_PHNE& rPhneDI, OUT T_EVAL2ND_PHNE& rPhneDJ)
{
	rPhneDI.Initialize();
	rPhneDJ.Initialize();

	auto L_Convert = [nHingeType] (int nDOF, const T_NLHG_D& NlhgD, const T_NLHG_D& TimeD, OUT T_EVAL2ND_PHNE& rPhneD)
	{
		rPhneD.nHingeType = nHingeType;
		rPhneD.nDof = nDOF;
		rPhneD.dForceMoment  = NlhgD.dForceMoment;
		rPhneD.dDisplacement = NlhgD.dDisplacement;
		rPhneD.dTime = TimeD.dDisplacement;  ///CTBPostIehgBeamSummary::GetRecValStr() 참고하여 동일한 time 출력하도록 처리.
		rPhneD.dDuctility = fabs(NlhgD.dDuctility_Pos) >= fabs(NlhgD.dDuctility_Neg) ? NlhgD.dDuctility_Pos : NlhgD.dDuctility_Neg;
		rPhneD.dDuctility_Pos = NlhgD.dDuctility_Pos;
		rPhneD.dDuctility_Neg = NlhgD.dDuctility_Neg;
		rPhneD.dD1_Pos = NlhgD.dD1[0];
		rPhneD.dD1_Neg = NlhgD.dD1[1];

		auto L_GetPerfLevel = [] (double nStatus)
		{
			if ( EQ(nStatus, 11.0) || EQ(nStatus, 14.0) ) return EN_LEVEL_IO;
			if ( EQ(nStatus, 12.0) || EQ(nStatus, 15.0) ) return EN_LEVEL_LS;
			if ( EQ(nStatus, 13.0) || EQ(nStatus, 16.0) ) return EN_LEVEL_CP;
			if ( EQ(nStatus, 17.0) ) return EN_LEVEL_CL;
			return EN_LEVEL_IO;
		};

		UINT unLevelNeg = L_GetPerfLevel(fabs(NlhgD.nHingeStatusLevel_Neg));
		UINT unLevelPos = L_GetPerfLevel(fabs(NlhgD.nHingeStatusLevel_Pos));
		rPhneD.enLevel = max(unLevelNeg, unLevelPos);
	};

	T_NLHG_D NlhgDI;
	T_NLHG_D NlhgDJ;
	T_NLHG_D TimeDI;
	T_NLHG_D TimeDJ;
	if ( !GetNlghTH(ElemK, ThisK, nTimeIndex, nDof, NlhgDI, NlhgDJ, TimeDI, TimeDJ) ) { return FALSE; }

	L_Convert(nDof, NlhgDI, TimeDI, rPhneDI);
	L_Convert(nDof, NlhgDJ, TimeDJ, rPhneDJ);

	CalcP1FemaTypeIehg(ElemK, nDof, true, rPhneDI.dP1_Neg, rPhneDI.dP1_Pos);
	CalcP1FemaTypeIehg(ElemK, nDof, false, rPhneDJ.dP1_Neg, rPhneDJ.dP1_Pos);

	return TRUE;
}

void CSeisEval::SetEvalConColm1stStrn(double dPnC, double dPnT, double dPeC, double dPeT,
                                      double dMey, double dMez, double dVny, double dVnz, OUT T_EVAL1ST_COLM_POS& rPos)
{
    rPos.dPnC = dPnC;
    rPos.dPnT = dPnT;
    rPos.dPeC = dPeC;
    rPos.dPeT = dPeT;
    rPos.dMey = dMey;
    rPos.dMez = dMez;
    rPos.dVny = dVny;
    rPos.dVnz = dVnz;		
}

void CSeisEval::SetEvalConBeam1stForc(const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_BEAM_POS& rPosI, OUT T_EVAL1ST_BEAM_POS& rPosJ)
{
    rPosI.dVUD = StrbDisp.dblForce[0][2];
    rPosI.dMUD = StrbDisp.dblForce[0][4];	
    rPosI.dVUF = StrbForc.dblForce[0][2];
	rPosI.dMUF = StrbForc.dblForce[0][4];

	rPosJ.dVUD = StrbDisp.dblForce[3][8];
	rPosJ.dMUD = StrbDisp.dblForce[3][10];
	rPosJ.dVUF = StrbForc.dblForce[3][8];
	rPosJ.dMUF = StrbForc.dblForce[3][10];
}

void CSeisEval::SetEvalConColm1stForc(const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_COLM_POS& rPosI, OUT T_EVAL1ST_COLM_POS& rPosJ)
{
	rPosI.dPUD  = StrbDisp.dblForce[0][0]*(-1.0);
	rPosI.dVUDy = StrbDisp.dblForce[0][1];
	rPosI.dVUDz = StrbDisp.dblForce[0][2];
	rPosI.dMUDy = StrbDisp.dblForce[0][4];
	rPosI.dMUDz = StrbDisp.dblForce[0][5];

	rPosI.dPUF  = StrbForc.dblForce[0][0]*(-1.0);
	rPosI.dVUFy = StrbForc.dblForce[0][1];
	rPosI.dVUFz = StrbForc.dblForce[0][2];
	rPosI.dMUFy = StrbForc.dblForce[0][4];
	rPosI.dMUFz = StrbForc.dblForce[0][5];

	rPosJ.dPUD  = StrbDisp.dblForce[3][6]*(-1.0);
	rPosJ.dVUDy = StrbDisp.dblForce[3][7];
	rPosJ.dVUDz = StrbDisp.dblForce[3][8];
	rPosJ.dMUDy = StrbDisp.dblForce[3][10];
	rPosJ.dMUDz = StrbDisp.dblForce[3][11];

	rPosJ.dPUF  = StrbForc.dblForce[3][6]*(-1.0);
	rPosJ.dVUFy = StrbForc.dblForce[3][7];
	rPosJ.dVUFz = StrbForc.dblForce[3][8];
	rPosJ.dMUFy = StrbForc.dblForce[3][10];
	rPosJ.dMUFz = StrbForc.dblForce[3][11];
}

void CSeisEval::SetEvalConWall1stForc(const T_STRW_D& StrwDisp, const T_STRW_D& StrwForc, OUT T_EVAL1ST_WALL_POS& rPosI, OUT T_EVAL1ST_WALL_POS& rPosJ)
{
	// T_STRW_D : Wall은 TOP=dblForce[0][*], BOTTOM=dblForce[1][*]
	auto l_SetForc = [] (const T_STRW_D& Disp, const T_STRW_D& Forc, EN_WALL_FORC_POS enPos, OUT T_EVAL1ST_WALL_POS& rPos)
	{
		rPos.dPUD = Disp.dblForce[enPos][DOF_FX]*(-1.0);
		rPos.dVUD = Disp.dblForce[enPos][DOF_FZ];
		rPos.dMUD = Disp.dblForce[enPos][DOF_MY];

		rPos.dPUF = Forc.dblForce[enPos][DOF_FX]*(-1.0);
		rPos.dVUF = Forc.dblForce[enPos][DOF_FZ];
		rPos.dMUF = Forc.dblForce[enPos][DOF_MY];
	};

	l_SetForc(StrwDisp, StrwForc, EN_WALL_FORC_POS_BOT, rPosI);
	l_SetForc(StrwDisp, StrwForc, EN_WALL_FORC_POS_TOP, rPosJ);
}

void CSeisEval::SetEvalSrcColm1stForc(const T_STRB_D& StrbDisp, const T_STRB_D& StrbForc, OUT T_EVAL1ST_STLCOLM_POS& rPosI, OUT T_EVAL1ST_STLCOLM_POS& rPosJ)
{
	rPosI.dPUD  = StrbDisp.dblForce[0][0];
	rPosI.dMUDy = StrbDisp.dblForce[0][4];
	rPosI.dMUDz = StrbDisp.dblForce[0][5];
	rPosI.dPUF  = StrbForc.dblForce[0][0];
	rPosI.dMUFy = StrbForc.dblForce[0][4];
	rPosI.dMUFz = StrbForc.dblForce[0][5];

	rPosJ.dPUD  = StrbDisp.dblForce[3][6];
	rPosJ.dMUDy = StrbDisp.dblForce[3][10];
	rPosJ.dMUDz = StrbDisp.dblForce[3][11];
	rPosJ.dPUF  = StrbForc.dblForce[3][6];
	rPosJ.dMUFy = StrbForc.dblForce[3][10];
	rPosJ.dMUFz = StrbForc.dblForce[3][11];
}

void CSeisEval::SetEvalConBeamLinForc(const T_STRB_D& StrbD, OUT T_EVALLIN_BEAM_POS& rPosI, OUT T_EVALLIN_BEAM_POS& rPosM, OUT T_EVALLIN_BEAM_POS& rPosJ)
{
	rPosI.dVd = StrbD.dblForce[0][2];
	rPosI.dMd = StrbD.dblForce[0][4];

	rPosM.dVd = max(fabs(StrbD.dblForce[0][8]), fabs(StrbD.dblForce[1][2]));
	rPosM.dMd = max(fabs(StrbD.dblForce[0][10]), fabs(StrbD.dblForce[1][4]));

	rPosM.dVd = max(fabs(rPosM.dVd), fabs(StrbD.dblForce[1][8]));
	rPosM.dMd = max(fabs(rPosM.dMd), fabs(StrbD.dblForce[1][10]));

	rPosM.dVd = max(fabs(rPosM.dVd), fabs(StrbD.dblForce[2][2]));
	rPosM.dMd = max(fabs(rPosM.dMd), fabs(StrbD.dblForce[2][4]));

	rPosM.dVd = max(fabs(rPosM.dVd), fabs(StrbD.dblForce[2][8]));
	rPosM.dMd = max(fabs(rPosM.dMd), fabs(StrbD.dblForce[2][10]));

	rPosM.dVd = max(fabs(rPosM.dVd), fabs(StrbD.dblForce[3][2]));
	rPosM.dMd = max(fabs(rPosM.dMd), fabs(StrbD.dblForce[3][4]));

	rPosJ.dVd = StrbD.dblForce[3][8];
	rPosJ.dMd = StrbD.dblForce[3][10];
}

BOOL CSeisEval::GetMatlSE(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_EVALMATL enMatl, OUT _DB_PMCV_USER_MATLSE& rMatlSE)
{
	T_ELEM_D ElemD;
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { ASSERT(0); return FALSE; }

	const T_MATL_K MatlK = ElemD.elmat;
	T_MATD_D MatdD;
	if ( !m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD) ) { ASSERT(0); return FALSE; }

	switch ( m_pDoc->m_pAttrCtrl2->GetSemeMethod() )
	{
	case D_SEME_METHOD_KISTEC2013:
	case D_SEME_METHOD_KISTEC2019:
	case D_SEME_METHOD_MOE2018:
	case D_SEME_METHOD_MOE2019:
		{
			rMatlSE.Initialize();

			CSeisEvalDataTool Tool;
			const CString& strType = MatdD.Type;
			if ( strType == _T("S") )
			{
				T_SESM_D SesmD;
				if ( !m_pDoc->m_pAttrCtrl2->GetSesm_Calulated(MatlK, SesmD) ) { ASSERT(0); return FALSE; }

				rMatlSE.dFy = Tool.GetFyStl(enStep, enMatl, SesmD);
			}
			else if ( strType == _T("C") )
			{
				T_SECM_D SecmD;
				if ( !m_pDoc->m_pAttrCtrl2->GetSecm_Calulated(MatlK, SecmD) ) { ASSERT(0); return FALSE; }

				rMatlSE.dFck = Tool.GetFc(enStep, enMatl, SecmD);
				rMatlSE.dFyr = Tool.GetFym(enStep, enMatl, SecmD);
				rMatlSE.dFys = Tool.GetFys(enStep, enMatl, SecmD);
			}
			else if ( strType == _T("SRC") )
			{
				T_SESM_D SesmD;
				if ( !m_pDoc->m_pAttrCtrl2->GetSesm_Calulated(MatlK, SesmD) ) { ASSERT(0); return FALSE; }
				T_SECM_D SecmD;
				if ( !m_pDoc->m_pAttrCtrl2->GetSecm_Calulated(MatlK, SecmD) ) { ASSERT(0); return FALSE; }

				rMatlSE.dFy  = Tool.GetFyStl(enStep, enMatl, SesmD);
				rMatlSE.dFck = Tool.GetFc(enStep, enMatl, SecmD);
				rMatlSE.dFyr = Tool.GetFym(enStep, enMatl, SecmD);
				rMatlSE.dFys = Tool.GetFys(enStep, enMatl, SecmD);
			}			
		}
		break;
	case D_SEME_METHOD_AIK_G_001_2021:
		{
			rMatlSE.Initialize();

			CSeisEvalDataTool Tool;
			const CString& strType = MatdD.Type;
			if ( strType == _T("C") )
			{
				T_PDCM_D PdcmD;
				if ( !m_pDoc->m_pAttrCtrl2->GetPdcm_Calulated(MatlK, PdcmD) ) { ASSERT(0); return FALSE; }

				rMatlSE.dFck = Tool.GetFc(enMatl, PdcmD);
				rMatlSE.dFyr = Tool.GetFym(enMatl, PdcmD);
				rMatlSE.dFys = Tool.GetFys(enMatl, PdcmD);
			}
		}
		break;
	default:
		{
			ASSERT(0);
		}		
		break;
	}
	
    return TRUE;
}

BOOL CSeisEval::GetPmcvData(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_EVALMATL enMatl, OUT _DB_PMCV_DATA& rPmcvy, OUT _DB_PMCV_DATA& rPmcvz)
{
    rPmcvy.Initialize();
    rPmcvz.Initialize();

    _DB_PMCV_USER_MATLSE MatlSE;
    if (!GetMatlSE(ElemK, enStep, enMatl, MatlSE)) { ASSERT(0); return FALSE; }
    
    BOOL bPOMode = enStep == EN_EVALSTEP_2ND ? TRUE : FALSE;

    CDbToolPmcv Pmcv;
    Pmcv.SetPOHingeMode(bPOMode);
    Pmcv.SetWallIDMode(TRUE);
    Pmcv.SetUseMatlSE(TRUE, MatlSE, GetPmcvModeSE(m_HingeCalcParam.enHingeType));

    _DB_PMCV_DATA_MAP mapPmcv;
    if ( Pmcv.Cal_PmcvData(PMCV_CODE_CON_ACI, PMCV_CODE_STL_ACI, ElemK, mapPmcv) > 0 ) { return FALSE; }

    if ( !mapPmcv.Lookup(1, rPmcvy) )
    {
        ASSERT(0);
        rPmcvy.Initialize(); 
    }
    if ( !mapPmcv.Lookup(3, rPmcvz) )
    {
        ASSERT(0);
        rPmcvz.Initialize(); 
    }
    return TRUE;
}

bool CSeisEval::ConvertPmcvData(const _DB_PMCV_DATA& srcPmcv, OUT T_PMCV_TABLE& rtarPmcv)
{
    INT_PTR nSize = srcPmcv.arPmcvUnit.GetSize();
    rtarPmcv.aPmcv.SetSize(nSize);

    _DB_PMCV_UNIT CurUnit;
    for ( INT_PTR i = 0; i < nSize ; ++i )
    {
        if (!srcPmcv.arPmcvUnit.Lookup(i+1, CurUnit)) { continue; }
        
        T_PMCV_PNT CurPnt;
        CurPnt.dPn = CurUnit.dPn;
        CurPnt.dMn = CurUnit.dMn;
        rtarPmcv.aPmcv.SetAt(i, CurPnt);
    }
    return true;
}

BOOL CSeisEval::GetPnSrcColm(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_EVALMATL enMatl, OUT double radPc[2], OUT double radPt[2])
{
    memset(radPc, 0.0, sizeof(double)*2);
    memset(radPt, 0.0, sizeof(double)*2);
    
    _DB_PMCV_USER_MATLSE MatlSE;
    if (!GetMatlSE(ElemK, enStep, enMatl, MatlSE)) { ASSERT(0); return FALSE; }

    BOOL bPOMode = enStep == EN_EVALSTEP_2ND ? TRUE : FALSE;

    CDbToolPmcv Pmcv;
    Pmcv.SetPOHingeMode(bPOMode);
    Pmcv.SetWallIDMode(TRUE);
    Pmcv.SetUseMatlSE(TRUE, MatlSE, EN_PMCV_MODE_SE_PO);

    Pmcv.CalPnSrcColm(PMCV_CODE_CON_ACI, PMCV_CODE_STL_ACI, ElemK, radPc, radPt);
    
    return TRUE;
}

BOOL CSeisEval::GetSectDim(const T_SECT_D& SectD, OUT double& rdB, OUT double& rdH)
{
    rdB = rdH = 0.0;

    const T_SECT_SECTBASE_D& SectBase = SectD.SectBefore.SectI;
    if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB)
    {
        rdH = SectBase.Size[0];
        rdB = SectBase.Size[1];		
        return TRUE;
    }	
    if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_T)
    {
        rdH = SectBase.Size[0];
        rdB = SectBase.Size[2];
        return TRUE;
    }
    if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR)
    {
        rdH = SectBase.Size[0];
        rdB = SectBase.Size[0];
        return TRUE;
    }
    if (SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P)
    {
        rdH = SectBase.Size[0];
        rdB = SectBase.Size[0];
        return TRUE;
    }

    ASSERT(FALSE);
    return FALSE;
}

double CSeisEval::GetRebarFy(double dFyByMatd)
{
    if (fabs(dFyByMatd) > 0.0) return dFyByMatd;

    CString strRebarCode = _T(""), strRebarName = _T("");
    CDBLib::GetDefaultConRbar(strRebarCode, strRebarName);

    T_MATL_REBAR RbarD;
    m_pDoc->m_pMatlDB->Create_RebarData(m_CodeUnit.nBase_Length, m_CodeUnit.nBase_Force, TRUE);	
    if (!m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strRebarName, RbarD)) RbarD.Initialize();

    return RbarD.B_fy;
}

double CSeisEval::GetRebarAs(const CString& strBarName)
{
    _DGN_REBAR_DATA Rbar;
    if (!m_mapRebarSize.Lookup(strBarName, Rbar)) return 0.0;
    return Rbar.dArea;
}

double CSeisEval::GetRebarDia(const CString& strBarName)
{
    _DGN_REBAR_DATA Rbar;
    if (!m_mapRebarSize.Lookup(strBarName, Rbar)) return 0.0;
    return Rbar.dDia;
}

double CSeisEval::CalcConBeamRhoBal(double dfck, double dfy)
{
    /// see [KISTEC 2013, 4.3.2.1], [KISTEC 2019, 5.4.3.1]
    double dBeta1  = CalcBeta1(dfck);
    return 0.85*dBeta1*dfck/dfy*600.0/(600.0+dfy);
}

void CSeisEval::CalcConBeamRho(double dB, double dH, const T_REBB_BASE& RebbBase, double dDc, OUT T_CONBEAM_FLEX& rFlex)
{
    const double dd = CalcDeffTensBarCentroid(dH, RebbBase, dDc);
    
    rFlex.dd   = dd;
    rFlex.dAs  = CalcAs(RebbBase);
    rFlex.dRho = fabs(dB*dd) < cEvalZero ? 0.0 : CalcAs(RebbBase)/(dB*dd);    
}

void CSeisEval::CalcConBeamFlex(double dB, double dH, double dfce, double dfye, const T_REBB_BASE& RebbBase, double dDc, OUT T_CONBEAM_FLEX& rFlex)
{
    const double dd  = CalcDeffTensBarCentroid(dH, RebbBase, dDc);
    const double dAs = CalcAs(RebbBase);
    const double da  = CalcStressBlockHeightConBeam(dfce, dfye, dAs, dB);

    rFlex.dd   = dd;
    rFlex.dAs  = dAs;
    rFlex.dRho = fabs(dB*dd) < cEvalZero ? 0.0 : CalcAs(RebbBase)/(dB*dd);
    rFlex.da   = da;
    rFlex.dMe  = fabs(dfce*dB) < cEvalZero ? 0.0 : dAs*dfye*(dd-da/2.0);  /// see [MOE2018, (5-1)]
}

void CSeisEval::SetCurveForcCtrlAxial(OUT double adCurv[2][9])
{
    adCurv[IDX_DDY][IDX_CURV_ME] = -1.011;
    adCurv[IDX_DDY][IDX_CURV_MD] = -1.01;
    adCurv[IDX_DDY][IDX_CURV_MC] = -1.01;
    adCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
    adCurv[IDX_DDY][IDX_CURV_A ] =  0.0;
    adCurv[IDX_DDY][IDX_CURV_B ] =  1.0;
    adCurv[IDX_DDY][IDX_CURV_C ] =  6.0;
    adCurv[IDX_DDY][IDX_CURV_D ] =  6.0;
    adCurv[IDX_DDY][IDX_CURV_E ] =  8.0;

    adCurv[IDX_FFY][IDX_CURV_ME] =  0.0;
    adCurv[IDX_FFY][IDX_CURV_MD] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_MC] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_MB] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_A ] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_B ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_C ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_D ] =  0.2; 
    adCurv[IDX_FFY][IDX_CURV_E ] =  0.2; 
}

void CSeisEval::SetCurveForcCtrlShear(OUT double adCurv[2][9])
{
    adCurv[IDX_DDY][IDX_CURV_ME] = -1.011; // D/DY -E
    adCurv[IDX_DDY][IDX_CURV_MD] = -1.01;
    adCurv[IDX_DDY][IDX_CURV_MC] = -1.01;
    adCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
    adCurv[IDX_DDY][IDX_CURV_A ] =  0.0;
    adCurv[IDX_DDY][IDX_CURV_B ] =  1.0;
    adCurv[IDX_DDY][IDX_CURV_C ] =  1.01;
    adCurv[IDX_DDY][IDX_CURV_D ] =  1.01;
    adCurv[IDX_DDY][IDX_CURV_E ] =  1.011;

    adCurv[IDX_FFY][IDX_CURV_ME] =  0.0; // F/FY -E
    adCurv[IDX_FFY][IDX_CURV_MD] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_MC] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_MB] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_A ] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_B ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_C ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_D ] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_E ] =  0.0; 
}

void CSeisEval::SetCurveDispCtrl(const double adParam[6], OUT double adCurv[2][9])
{
    SetCurveDispCtrl(adParam[HG_A], adParam[HG_B], adParam[HG_C], adCurv);
}

void CSeisEval::SetCurveDispCtrl(const double adParamP[6], const double adParamN[6], OUT double adCurv[2][9])
{
    SetCurveDispCtrl(adParamP[HG_A], adParamP[HG_B], adParamP[HG_C], adParamN[HG_A], adParamN[HG_B], adParamN[HG_C], adCurv);
}

void CSeisEval::SetCurveDispCtrl(double dDispC, double dDispE, double dForcD, OUT double adCurv[2][9])
{
    if ( dDispC == dDispE ) { dDispE = dDispC * 1.01; } /// 두 값이 같으면 힌지 check 에서 error 가 나므로, 1% 차이나게 처리.

    adCurv[IDX_DDY][IDX_CURV_ME] = -1.0*dDispE;
    adCurv[IDX_DDY][IDX_CURV_MD] = -1.0*dDispC;
    adCurv[IDX_DDY][IDX_CURV_MC] = -1.0*dDispC;
    adCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
    adCurv[IDX_DDY][IDX_CURV_A ] =  0.0;
    adCurv[IDX_DDY][IDX_CURV_B ] =  1.0;
    adCurv[IDX_DDY][IDX_CURV_C ] = dDispC;
    adCurv[IDX_DDY][IDX_CURV_D ] = dDispC;
    adCurv[IDX_DDY][IDX_CURV_E ] = dDispE;

    adCurv[IDX_FFY][IDX_CURV_ME] = -1.0*dForcD;
    adCurv[IDX_FFY][IDX_CURV_MD] = -1.0*dForcD; 
    adCurv[IDX_FFY][IDX_CURV_MC] = -1.1; 
    adCurv[IDX_FFY][IDX_CURV_MB] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_A ] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_B ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_C ] =  1.1; 
    adCurv[IDX_FFY][IDX_CURV_D ] =  dForcD; 
    adCurv[IDX_FFY][IDX_CURV_E ] =  dForcD; 
}

void CSeisEval::SetCurveDispCtrl(double dPDispC, double dPDispE, double dPForcD, double dMDispC, double dMDispE, double dMForcD, OUT double adCurv[2][9])
{
    if ( dPDispC == dPDispE ) { dPDispE = dPDispC * 1.01; } /// 두 값이 같으면 힌지 check 에서 error 가 나므로, 1% 차이나게 처리.
    if ( dMDispC == dMDispE ) { dMDispE = dMDispC * 1.01; } /// 두 값이 같으면 힌지 check 에서 error 가 나므로, 1% 차이나게 처리.

    adCurv[IDX_DDY][IDX_CURV_ME] = -1.0*dMDispE;
    adCurv[IDX_DDY][IDX_CURV_MD] = -1.0*dMDispC;
    adCurv[IDX_DDY][IDX_CURV_MC] = -1.0*dMDispC;
    adCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
    adCurv[IDX_DDY][IDX_CURV_A ] =  0.0;
    adCurv[IDX_DDY][IDX_CURV_B ] =  1.0;
    adCurv[IDX_DDY][IDX_CURV_C ] = dPDispC;
    adCurv[IDX_DDY][IDX_CURV_D ] = dPDispC;
    adCurv[IDX_DDY][IDX_CURV_E ] = dPDispE;

    adCurv[IDX_FFY][IDX_CURV_ME] = -1.0*dMForcD;
    adCurv[IDX_FFY][IDX_CURV_MD] = -1.0*dMForcD; 
    adCurv[IDX_FFY][IDX_CURV_MC] = -1.1; 
    adCurv[IDX_FFY][IDX_CURV_MB] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_A ] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_B ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_C ] =  1.1; 
    adCurv[IDX_FFY][IDX_CURV_D ] =  dPForcD; 
    adCurv[IDX_FFY][IDX_CURV_E ] =  dPForcD; 
}

bool CSeisEval::MakeWallShearParam(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_DIR enDir, bool bForcCtrl, OUT T_WALL_SHR_PARAM raShr[EN_MEMBEND_NUM])
{
    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        raShr[i].Initialize();
    }

    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if ( !m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey) ) { ASSERT(0); return false; }
    if ( !WallD.bLineWall ) { return false; }

    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if ( !GetElemMatlThik(ElemK, ThikD, MatdD, SecmD) ) { ASSERT(0); return false; }

    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if ( !m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD) ) { ASSERT(0); return false; }

    double dTw = WallD.dThickness;
    double dLw = WallD.dLength;
    
    T_WALL_SHR_PARAM Shr;
    GetWallMatlShear(enStep, SecmD, bForcCtrl, Shr.Comm);
    Shr.Comm.dLambda = GetConLambda(MatdD);
    Shr.Comm.dAsv = 2.0*GetRebarAs(RebwD.HorizonRebarName);
    Shr.Comm.dSv  = RebwD.dHorizonRebarSpace;

    CSeisEvalDataTool Tool;
    Shr.bVnmaxEQ = Tool.IsSpecialShearWallCondition();
    Shr.dLw = dLw;
    Shr.dTotalHw = GetContinuousWallTotalHw(ElemK, WallD.dHeight);
            
    UINT unDofV = 0;
    UINT unDofM = 0;
    switch ( enDir )
    {
    case EN_DIR_Y:
        {
            unDofV = DOF_FY;
            unDofM = DOF_MZ;

            int nDeffType = Tool.GetDeffTypeConWall(enStep);
            Shr.Comm.dBv = dLw;
            Shr.Comm.dDv = CalcDeffShear(nDeffType, dTw, RebwD.dDw);
            Shr.dLw = dTw;
        }
        break;
    case EN_DIR_Z:
        {
            unDofV = DOF_FZ;
            unDofM = DOF_MY;

            int nDeffType = Tool.GetDeffTypeConWall(enStep);
            Shr.Comm.dBv = dTw;
            Shr.Comm.dDv = CalcDeffShear(nDeffType, dLw, RebwD.dDe);
            Shr.dLw = dLw;
        }
        break;
    default: ASSERT(0);
        break;
    }

    for ( int i=0; i<EN_MEMBEND_NUM; ++i )
    {
        raShr[i] = Shr;
    }

    return true;
}

void CSeisEval::SetCurveDispCtrlStl(const double adParam[6], OUT double adCurv[2][9])
{
    double dDispC = adParam[HG_A];
    double dDispE = adParam[HG_B];
    double dForcD = adParam[HG_C];

    // [GEN-1527] 철골의 Q/QY 계산 수정.
    if ( dDispC == dDispE ) { dDispE = dDispC * 1.01; } /// 두 값이 같으면 힌지 check 에서 error 가 나므로, 1% 차이나게 처리.

    adCurv[IDX_DDY][IDX_CURV_ME] = -1.0*dDispE;
    adCurv[IDX_DDY][IDX_CURV_MD] = -1.0*dDispC;
    adCurv[IDX_DDY][IDX_CURV_MC] = -1.0*dDispC;
    adCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
    adCurv[IDX_DDY][IDX_CURV_A ] =  0.0;
    adCurv[IDX_DDY][IDX_CURV_B ] =  1.0;
    adCurv[IDX_DDY][IDX_CURV_C ] = dDispC;
    adCurv[IDX_DDY][IDX_CURV_D ] = dDispC;
    adCurv[IDX_DDY][IDX_CURV_E ] = dDispE;

    double dForcC = 1.0 + max(0.03*(dDispC - 1.0), 0.01);
    
    adCurv[IDX_FFY][IDX_CURV_ME] = -1.0*dForcD;
    adCurv[IDX_FFY][IDX_CURV_MD] = -1.0*dForcD; 
    adCurv[IDX_FFY][IDX_CURV_MC] = -1.0*dForcC; 
    adCurv[IDX_FFY][IDX_CURV_MB] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_A ] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_B ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_C ] =  dForcC; 
    adCurv[IDX_FFY][IDX_CURV_D ] =  dForcD; 
    adCurv[IDX_FFY][IDX_CURV_E ] =  dForcD; 
}

void CSeisEval::SetCurveDispCtrlStl(const double adParamP[6], const double adParamN[6], OUT double adCurv[2][9])
{
    double dPDispC = adParamP[HG_A];
    double dPDispE = adParamP[HG_B];
    double dPForcD = adParamP[HG_C];

    double dMDispC = adParamN[HG_A];
    double dMDispE = adParamN[HG_B];
    double dMForcD = adParamN[HG_C];

    // [GEN-1527] 철골의 Q/QY 계산 수정.
    if ( dPDispC == dPDispE ) { dPDispE = dPDispC * 1.01; } /// 두 값이 같으면 힌지 check 에서 error 가 나므로, 1% 차이나게 처리.
    if ( dMDispC == dMDispE ) { dMDispE = dMDispC * 1.01; } /// 두 값이 같으면 힌지 check 에서 error 가 나므로, 1% 차이나게 처리.

    adCurv[IDX_DDY][IDX_CURV_ME] = -1.0*dMDispE;
    adCurv[IDX_DDY][IDX_CURV_MD] = -1.0*dMDispC;
    adCurv[IDX_DDY][IDX_CURV_MC] = -1.0*dMDispC;
    adCurv[IDX_DDY][IDX_CURV_MB] = -1.0;
    adCurv[IDX_DDY][IDX_CURV_A ] =  0.0;
    adCurv[IDX_DDY][IDX_CURV_B ] =  1.0;
    adCurv[IDX_DDY][IDX_CURV_C ] = dPDispC;
    adCurv[IDX_DDY][IDX_CURV_D ] = dPDispC;
    adCurv[IDX_DDY][IDX_CURV_E ] = dPDispE;

    const double dPForcC = 1.0 + max(0.03*(dPDispC - 1.0), 0.01);
    const double dMForcC = 1.0 + max(0.03*(dMDispC - 1.0), 0.01);

    adCurv[IDX_FFY][IDX_CURV_ME] = -1.0*dMForcD;
    adCurv[IDX_FFY][IDX_CURV_MD] = -1.0*dMForcD; 
    adCurv[IDX_FFY][IDX_CURV_MC] = -1.0*dMForcC; 
    adCurv[IDX_FFY][IDX_CURV_MB] = -1.0; 
    adCurv[IDX_FFY][IDX_CURV_A ] =  0.0; 
    adCurv[IDX_FFY][IDX_CURV_B ] =  1.0; 
    adCurv[IDX_FFY][IDX_CURV_C ] =  dPForcC; 
    adCurv[IDX_FFY][IDX_CURV_D ] =  dPForcD; 
    adCurv[IDX_FFY][IDX_CURV_E ] =  dPForcD; 
}

void CSeisEval::SetAccetanceCriteria(const double adParam[6], OUT T_PHGT_FEMA& rPhgt)
{
    SetAccetanceCriteria(adParam[HG_IO], adParam[HG_LS], adParam[HG_CP], rPhgt);
}

void CSeisEval::SetAccetanceCriteria(const double adParamP[6], const double adParamN[6], OUT T_PHGT_FEMA& rPhgt)
{
    SetAccetanceCriteria(adParamP[HG_IO], adParamP[HG_LS], adParamP[HG_CP], adParamN[HG_IO], adParamN[HG_LS], adParamN[HG_CP], rPhgt);
}

void CSeisEval::SetAccetanceCriteria(double dIO, double dLS, double dCP, OUT T_PHGT_FEMA& rPhgt)
{	
    rPhgt.dIO[IDX_PLS] = dIO;
    rPhgt.dIO[IDX_MNS] = dIO;
    rPhgt.dLS[IDX_PLS] = dLS;
    rPhgt.dLS[IDX_MNS] = dLS;
    rPhgt.dCP[IDX_PLS] = dCP;
    rPhgt.dCP[IDX_MNS] = dCP;
}

void CSeisEval::SetAccetanceCriteria(double dPIO, double dPLS, double dPCP, double dMIO, double dMLS, double dMCP, OUT T_PHGT_FEMA& rPhgt)
{
    rPhgt.dIO[IDX_PLS] = dPIO;
    rPhgt.dIO[IDX_MNS] = dMIO;
    rPhgt.dLS[IDX_PLS] = dPLS;
    rPhgt.dLS[IDX_MNS] = dMLS;
    rPhgt.dCP[IDX_PLS] = dPCP;
    rPhgt.dCP[IDX_MNS] = dMCP;
}

double CSeisEval::CalcDDyFromAB(double dValue, double dThetay)
{
    double dThetaApp = (fabs(dThetay) < cThetaLimit2) ? cThetaLimit2 : fabs(dThetay);
    return 1.0 + dValue/dThetaApp;
}

double CSeisEval::CalcDDyFromDE(double dValue, double dThetay)
{
    double dThetaApp = (fabs(dThetay) < cThetaLimit2) ? cThetaLimit2 : fabs(dThetay);
    return dValue/dThetaApp;
}

double CSeisEval::CalcDistRatio(double dVal, double dValMin, double dValMax)
{
    if (dVal < dValMin) return 0.0;
    if (dVal > dValMax) return 1.0;
    return (dVal - dValMin)/(dValMax - dValMin);
}

double CSeisEval::CalcStressBlockHeightConBeam(double dfc, double dfy, double dAs, double dB)
{
    return fabs(dfc*dB) < cEvalZero ? 0.0 : dAs*dfy / (0.85*dfc*dB);
}

double CSeisEval::CalcMeConBeam(double dfce, double dB, double dH, double dfye, const T_REBB_BASE& RebbBase, double dDc)
{
    double dAs = CalcAs(RebbBase);
    double da  = CalcStressBlockHeightConBeam(dfce, dfye, dAs, dB);
    double dd  = CalcDeffTensBarCentroid(dH, RebbBase, dDc);

    return fabs(dfce*dB) < cEvalZero ? 0.0 : dAs*dfye*(dd-da/2.0);  /// see [MOE2018, (5-1)]
}

double CSeisEval::CalcVpConBeam(double dMeiP, double dMeiN, double dMejP, double dMejN, double dL)
{
    return fabs(dL) < cEvalZero ? 0.0 : max(dMeiP + dMejN, dMeiN + dMejP)/dL;
}

double CSeisEval::CalcVpConBeam(const double adMeTop[2], const double adMeBot[2], double dL)
{
    const double dMe1 = adMeTop[EN_MEMBEND_I] + adMeBot[EN_MEMBEND_J];
    const double dMe2 = adMeTop[EN_MEMBEND_J] + adMeBot[EN_MEMBEND_I];
    return fabs(dL) < cEvalZero ? 0.0 : max(dMe1, dMe2)/dL;
}

double CSeisEval::CalcVnConBeam(const T_CON_SHR_PARAM& Shr)
{
    const double dVc = CalcVcConBeam(Shr);
    const double dVs = CalcVsFram(Shr);
    return dVc + dVs;
}

double CSeisEval::CalcVcConBeam(const T_CON_SHR_PARAM& Shr)
{
    return 1.0/6.0 * sqrt(Shr.dFck) * Shr.dBv * Shr.dDv;
}

double CSeisEval::CalcVpConWall(double dMp, double dHw, const T_WALL_SHR_PARAM& Shr)
{
    const double dMu = Shr.dM;
    const double dVu = Shr.dV;    
    /// [JIRA, GEN-532] 벽체 휨지배/전단지배 판정.
    double dMV = fabs(dVu) < cEvalZero ? 0.0 : fabs(dMu/dVu);
    double dVp = fabs(dMV) < cEvalZero ? dMp/(0.5*dHw) : dMp/(dMV);
    return fabs(dVp);
}

EN_EVALCTRL CSeisEval::CalcColmCtrlType(double dMei, double dMej, double dL, double dVn)
{
    const double dVe = fabs(dL) < cEvalZero ? 0.0 : (dMei + dMej)/dL;
    return dVe > dVn ? EN_EVALCTRL_SHER : EN_EVALCTRL_FLEX;
}

EN_EVALCTRL CSeisEval::CalcBeamCtrlType(double dVp, double dVn)
{    
    return dVp > dVn ? EN_EVALCTRL_SHER : EN_EVALCTRL_FLEX;
}

EN_EVALCTRL CSeisEval::CalcWallCtrlType(double dMp, double dVn, double dHw, const T_WALL_SHR_PARAM& Shr)
{    
    const double dVp = CalcVpConWall(dMp, dHw, Shr);
    return dVp > dVn ? EN_EVALCTRL_SHER : EN_EVALCTRL_FLEX;    
}

void CSeisEval::GetFramMatlShear(EN_EVALSTEP enStep, const T_SECM_D& SecmD, OUT T_CON_SHR_PARAM& rShr)
{
    CSeisEvalDataTool Tool;
    rShr.dFck = Tool.GetFc(enStep, EN_EVALMATL_NOM, SecmD);
    rShr.dFys = Tool.GetFys(enStep, EN_EVALMATL_NOM, SecmD);    
}

void CSeisEval::GetFramMatlShear(const T_PDCM_D& Pdcmd, const EN_EVALMATL& enMatl, OUT T_CON_SHR_PARAM& rShr)
{
	CSeisEvalDataTool Tool;
	rShr.dFck = Tool.GetFc(enMatl, Pdcmd);
	rShr.dFys = Tool.GetFys(enMatl, Pdcmd);
}

double CSeisEval::CalcThetay(double dM, double dK)
{
    return fabs(dK*dM) == 0.0 ? cThetaLimit : fabs(dM/dK);
}

double CSeisEval::CalcShearIndex(double dV, const T_CON_SHR_PARAM& Shr)
{
    double dfck = Shr.dFck;
    double dB   = Shr.dBv;
    double dd   = Shr.dDv;
    return fabs(dfck*dB*dd) < cEvalZero ? cEvalMaxRat : fabs(dV)/(sqrt(dfck)*dB*dd);
}

double CSeisEval::CalcShearIndexWall(double dV, const T_WALL_SHR_PARAM& Shr)
{
    /// 벽체 강축 전단만 이 식 적용. 약축 전단은 기둥 식을 적용합니다.
    double dfck = Shr.Comm.dFck;
    double dtw   = Shr.Comm.dBv;
    double dlw  = Shr.dLw;
    return fabs(dfck*dtw*dlw) < cEvalZero ? cEvalMaxRat : fabs(dV)/(sqrt(dfck)*dtw*dlw);
}

double CSeisEval::CalcAxialIndex(double dP, double dfck, double dAg)
{
    return fabs(dAg*dfck) < cEvalZero ? cEvalMaxRat : max(dP, 0.0)/(dAg*dfck);
}

double CSeisEval::CalcVs(const T_CON_SHR_PARAM& Shr)
{
    const double dAsv = Shr.dAsv;
    const double dfy  = Shr.dFys;
    const double dd   = Shr.dDv;
    const double dS   = Shr.dSv;

    return fabs(dS) < cEvalZero ? cEvalMaxRat :  dAsv*dfy*dd / dS;
}

EN_PMCV_MODE_SE CSeisEval::GetPmcvModeSE(const EN_NLANALTYPE& enNLType)
{
    switch (enNLType)
    {
    case EN_NLANALTYPE::PO: return EN_PMCV_MODE_SE_PO;
	case EN_NLANALTYPE::TH: return EN_PMCV_MODE_SE_TH;
	default: ASSERT(0); return EN_PMCV_MODE_SE_PO;
    }
}

double CSeisEval::CalcVsFram(const T_CON_SHR_PARAM& Shr)
{
    const double dVscal = CalcVs(Shr);
    const double dVsmax = CalcVsmaxFram(Shr);
    return min(dVscal, dVsmax);
}

double CSeisEval::CalcVsWall(const T_WALL_SHR_PARAM& Shr, double dVc)
{
    const double dVscal = CalcVs(Shr.Comm);
    const double dVsmax = CalcVsmaxWall(Shr, dVc);
    return min(dVscal, dVsmax);
}

double CSeisEval::CalcK1_EVAL(const T_CON_SHR_PARAM& Shr)
{
    const double dS = Shr.dSv;
    const double dd = Shr.dDv;

    if (dgn::LE(dS, 0.5*dd)) { return 1.0; }
    if (dgn::LE(dS, dd)) { return 0.5; }
    return 0.0;
}

double CSeisEval::CalcK1_KBC(const T_CON_SHR_PARAM& Shr)
{
    const double dS = Shr.dSv;
    const double dd = Shr.dDv;

    return dgn::LE(dS, 0.5*dd) ? 1.0 : 0.0;
}

double CSeisEval::CalcAsWall(const T_WALL_D& WallD, const T_REBW_D& RebwD)
{
    CDbToolPmcv Pmcv;
    Pmcv.SetPOHingeMode(TRUE);

    const double dLw = WallD.dLength;
    int nEndBarArr = RebwD.nNumEndRebar/2;
    int nVerBarArr = Pmcv.Get_WallRbarArr(dLw, RebwD.dDe, RebwD.dVerticalRebarSpace, nEndBarArr, RebwD.dEndRebarSpace);
    return 4.0*(nEndBarArr*GetRebarAs(RebwD.EndRebarName) + nVerBarArr*GetRebarAs(RebwD.VerticalRebarName));
}

bool CSeisEval::MakeBeamShearParam(T_ELEM_K ElemK, EN_EVALSTEP enStep, OUT T_BEAM_SHR_PARAM raShr[EN_BEAM_POS_NUM])
{
    for (int i=0; i<EN_BEAM_POS_NUM; ++i)
    {
        raShr[i].Initialize();
    }

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { ASSERT(0); return false; } 
    if (SecmD.bUseDefaultVal) { return false; }

    double dB = 0.0;
    double dH = 0.0;
    if (!GetSectDim(SectD, dB, dH)) { ASSERT(0); return false; }

    T_REBB_D RebbD;
    if (!m_pDoc->m_pAttrCtrl->GetRebbByElemK(ElemK, RebbD)) { ASSERT(0); return false; }

    CSeisEvalDataTool Tool;
    const int    nDeffType = Tool.GetDeffTypeConBeam(enStep);
    const double dAsv1 = GetRebarAs(RebbD.SubRebarName);

    for (int i=0; i<EN_BEAM_POS_NUM; ++i)
    {
        T_BEAM_SHR_PARAM& rCur = raShr[i];        
        GetFramMatlShear(enStep, SecmD, rCur.ParamPos);

        T_REBB_BASE CurTop;
        T_REBB_BASE CurBot; 
        GetRebbBase(RebbD, static_cast<EN_BEAM_POS>(i), CurTop, CurBot);

        rCur.ParamPos.dBv  = dB;
        rCur.ParamPos.dDv  = CalcDeffShear(nDeffType, dH, CurBot, RebbD.dB);
        /// RC 보의 전단철근 정보는 Top에 저장되어 있습니다.
        rCur.ParamPos.dAsv = dAsv1 * CurTop.iSubRebarNum;
        rCur.ParamPos.dSv  = CurTop.dSubRebarSpace;

        rCur.ParamNeg = rCur.ParamPos;
        rCur.ParamNeg.dDv  = CalcDeffShear(nDeffType, dH, CurTop, RebbD.dT);
    }

    return true;
}

BOOL CSeisEval::GetRebbBase(const T_REBB_D& RebbD, EN_BEAM_POS enPos, OUT T_REBB_BASE& rRebbTop, OUT T_REBB_BASE& rRebbBot)
{
    rRebbTop.Initialize();
    rRebbBot.Initialize();

    BOOL bExist = TRUE;
    switch (enPos)
    {
    case EN_BEAM_POS_I:
        rRebbTop = RebbD.Top_I;
        rRebbBot = RebbD.Bot_I;
        break;
    case EN_BEAM_POS_M:
        rRebbTop = RebbD.Top_C;
        rRebbBot = RebbD.Bot_C;
        break;
    case EN_BEAM_POS_J:
        rRebbTop = RebbD.Top_J;
        rRebbBot = RebbD.Bot_J;
        break;
    default:
        ASSERT(0);
        bExist = FALSE;
        break;
    }
    return bExist;
}

double CSeisEval::CalcDeffTensBarCentroid(double dH, const T_REBB_BASE& RebbBase, double dDc)
{
    double dDistLayer = GetDistLayerConBeam();
    double dAs1st = RebbBase.nRebar1 * GetRebarAs(RebbBase.RebarName);
    double dAs2nd = RebbBase.nRebar2 * GetRebarAs(RebbBase.RebarName2nd);
    double dDc2nd = dDc + dDistLayer + GetRebarDia(RebbBase.RebarName)/2.0 + GetRebarDia(RebbBase.RebarName2nd)/2.0;
    double dSumAsidi = dDc*dAs1st + dDc2nd*dAs2nd;
    double dSumAsi   = dAs1st + dAs2nd;
    return dSumAsi > 0.0 ? dH - (dSumAsidi/dSumAsi) : dH - dDc;
}

double CSeisEval::CalcDeffShear(int nDeffType, double dH, const T_REBB_BASE& RebbBase, double dDc)
{
    /// [PMS 5835] RC 부재 전단강도 계산식 유효폭 산정 옵션 추가.
    double ddeff = 0.0;
    switch (nDeffType)
    {
    case D_SEME_EFF_DEPTH_AUTO: { ddeff = CalcDeffTensBarCentroid(dH, RebbBase, dDc); } break;
    case D_SEME_EFF_DEPTH_08H : { ddeff = 0.8*dH; } break;        
    default: ASSERT(FALSE); break;
    }
    return ddeff;
}

double CSeisEval::CalcAs(const T_REBB_BASE& RebbBase)
{
    return RebbBase.nRebar1 * GetRebarAs(RebbBase.RebarName) + RebbBase.nRebar2 * GetRebarAs(RebbBase.RebarName2nd);
}

void CSeisEval::IsUseConColmShearBarSmid(T_ELEM_K ElemK, EN_DIR enDir, OUT BOOL rabUse[EN_MEMBEND_NUM])
{
    rabUse[EN_MEMBEND_I] = FALSE;
    rabUse[EN_MEMBEND_J] = FALSE;

    auto l_IsCodeUseMidSpace =[](int nMethod) -> bool
    {
        switch (nMethod)
        {
        case D_SEME_METHOD_KISTEC2013: return false;
        case D_SEME_METHOD_KISTEC2019: return true;
        case D_SEME_METHOD_MOE2018:    return false;
        case D_SEME_METHOD_MOE2019:    return true; 
		case D_SEME_METHOD_AIK_G_001_2021: return false;
        default: ASSERT(0); return false;
        }
    };

    T_SEME_D SemeD;
    if (!m_pDoc->m_pAttrCtrl2->GetSeme(SemeD)) { SemeD.Initialize(); }
    if (!l_IsCodeUseMidSpace(SemeD.nMethod)) return;

    T_MEMB_K MembK;
    if (!m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK, MembK)) { return; }

    T_PARTIALCOLUMN_DATA PartialColm;
    if (!m_ColmDivByInfill.GetData(MembK, PartialColm)) { return; }

    T_COLBYIS_DOF CurDir;
    switch (enDir)
    { 
    case EN_DIR_Y: CurDir = PartialColm.inPlane;    break;
    case EN_DIR_Z: CurDir = PartialColm.outOfPlane; break;
    default: ASSERT(0); break;        
    }

    if (!CurDir.bModifiedByInfillStrut) { return; }

    T_ELEM_D ElemD;  
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ElemD.Initialize(); }

    T_NODE_D nodeI, nodeJ;
    if (ElemD.eltyp == WALL_EL)
    {
        if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], nodeI)) { ASSERT(0); }
        if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[3], nodeJ)) { ASSERT(0); }
    }
    else
    {
        if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], nodeI)) { ASSERT(0); }
        if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], nodeJ)) { ASSERT(0); }
    }

    BOOL bEndILower = (nodeI.z < nodeJ.z)? TRUE : FALSE;

    rabUse[EN_MEMBEND_I] = bEndILower ? TRUE  : FALSE;
    rabUse[EN_MEMBEND_J] = bEndILower ? FALSE : TRUE;
}

bool CSeisEval::MakeColmShearParamEval(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_DIR enDir, OUT T_COLM_SHR_PARAM raShr[EN_MEMBEND_NUM])
{
    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        raShr[i].Initialize();
    }

    T_SECT_D SectD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if (!GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD)) { ASSERT(0); return false; } 
    if (SecmD.bUseDefaultVal) { return false; }

    double dB = 0.0;
    double dH = 0.0;
    if (!GetSectDim(SectD, dB, dH)) { ASSERT(0); return false; }

    T_REBC_D RebcD;
    if (!m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD)) { ASSERT(0); return false; }

    BOOL abUseMid[EN_MEMBEND_NUM] = { FALSE, };
    if (enStep == EN_EVALSTEP_2ND)    
    {
        IsUseConColmShearBarSmid(ElemK, enDir, abUseMid);
    }

    T_SEME_D SemeD;
    if (!m_pDoc->m_pAttrCtrl2->GetSeme(SemeD)) { SemeD.Initialize(); }

    int anAutoK1[EN_EVALSTEP_NUM] = { SemeD.data.eval.nK1Factor_ln, SemeD.data.eval.nK1Factor_mf, SemeD.data.eval.nK1Factor_po };
    double adUserK1[EN_EVALSTEP_NUM] = { SemeD.data.eval.dUserK1_ln, SemeD.data.eval.dUserK1_mf, SemeD.data.eval.dUserK1_po };

    CSeisEvalDataTool Tool;

    const CString& strShape = SectD.SectBefore.Shape;
    const T_SECT_SECTBASE_D& CurSectBase = SectD.SectBefore.SectI;
    const int nDeffType = Tool.GetDeffTypeConColm(enStep);

    double dBv = 0.0;
    double dHv = 0.0;
    if (strShape == D_SECT_SHAPE_REG_SB)
    {
        switch (enDir)
        {
        case EN_DIR_Y:
            {
                dBv = CurSectBase.Size[0];
                dHv = CurSectBase.Size[1];                 
            }
            break;
        case EN_DIR_Z:
            {
                dBv = CurSectBase.Size[1];
                dHv = CurSectBase.Size[0];
            }
            break;
        default: ASSERT(0); break;
        }
    }
    else if (strShape == D_SECT_SHAPE_REG_SR)
    {
        dBv = CurSectBase.Size[0];
        dHv = CurSectBase.Size[0];
    }    
    else
    {
        ASSERT(0);
        return false;
    }

    int nShrMethod = Tool.GetShearMethodConColm(enStep);
    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        T_COLM_SHR_PARAM& rCur = raShr[i];
        GetFramMatlShear(enStep, SecmD, rCur.Comm);

        rCur.nShrMethod = nShrMethod;
        rCur.bAutoK1    = anAutoK1[enStep] == D_SEME_K1_AUTO ? TRUE : FALSE;
        rCur.dUserK1    = adUserK1[enStep];
        rCur.dAg        = CurSectBase.Stiffness.Area;

        rCur.Comm.dLambda = GetConLambda(MatdD);
        rCur.Comm.dBv = dBv;

        const BOOL bUseMid = abUseMid[i];
        rCur.Comm.dSv = bUseMid ? RebcD.dSubRebarSpace2 : RebcD.dSubRebarSpace;        

        if (strShape == D_SECT_SHAPE_REG_SB)
        {
            rCur.Comm.dDv  = CalcDeffShear(nDeffType, dHv, RebcD.dConcCenter);
            rCur.Comm.dAsv = CalcAsvShapeSB(RebcD, bUseMid, enDir);
        }
        else if (strShape == D_SECT_SHAPE_REG_SR)
        {
            rCur.Comm.dDv  = 0.8*dHv;
            rCur.Comm.dAsv = CalcAsvShapeSR(RebcD, bUseMid);
        }
        else ASSERT(0);
    }
    return true;
}

bool CSeisEval::MakeColmShearParamPdgn(T_ELEM_K ElemK, EN_NLANALTYPE enAnal, EN_DIR enDir, EN_EVALMATL enMatl, OUT T_COLM_SHR_PARAM raShr[EN_MEMBEND_NUM])
{
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		raShr[i].Initialize();
	}

	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_PDCM_D PdcmD;
	if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return false; }
	if ( PdcmD.bUseDefaultVal ) { return false; }

	double dB = 0.0;
	double dH = 0.0;
	if ( !GetSectDim(SectD, dB, dH) ) { ASSERT(0); return false; }

	T_REBC_D RebcD;
	if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { ASSERT(0); return false; }

	BOOL abUseMid[EN_MEMBEND_NUM] = { FALSE, };
	IsUseConColmShearBarSmid(ElemK, enDir, abUseMid);

	T_SEME_D SemeD;
	if ( !m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) ) { SemeD.Initialize(); }

	CSeisEvalDataTool Tool;

	const CString& strShape = SectD.SectBefore.Shape;
	const T_SECT_SECTBASE_D& CurSectBase = SectD.SectBefore.SectI;
	const int nDeffType = Tool.GetDeffTypeConColm(enAnal);

	double dBv = 0.0;
	double dHv = 0.0;
	if ( strShape == D_SECT_SHAPE_REG_SB )
	{
		switch ( enDir )
		{
		case EN_DIR_Y:
			{
				dBv = CurSectBase.Size[0];
				dHv = CurSectBase.Size[1];
			}
			break;
		case EN_DIR_Z:
			{
				dBv = CurSectBase.Size[1];
				dHv = CurSectBase.Size[0];
			}
			break;
		default: ASSERT(0); break;
		}
	}
	else if ( strShape == D_SECT_SHAPE_REG_SR )
	{
		dBv = CurSectBase.Size[0];
		dHv = CurSectBase.Size[0];
	}
	else
	{
		ASSERT(0);
		return false;
	}

	int nShrMethod = Tool.GetShearMethodConColm(enAnal);
	for ( int i=0; i<EN_MEMBEND_NUM; ++i )
	{
		T_COLM_SHR_PARAM& rCur = raShr[i];
		GetFramMatlShear(PdcmD, enMatl, rCur.Comm);

		rCur.nShrMethod = nShrMethod;
		rCur.bAutoK1    = FALSE;
		rCur.dUserK1    = 1.0;
		rCur.dAg        = CurSectBase.Stiffness.Area;

		rCur.Comm.dLambda = GetConLambda(MatdD);
		rCur.Comm.dBv = dBv;

		const BOOL bUseMid = abUseMid[i];
		rCur.Comm.dSv = bUseMid ? RebcD.dSubRebarSpace2 : RebcD.dSubRebarSpace;

		if ( strShape == D_SECT_SHAPE_REG_SB )
		{
			rCur.Comm.dDv  = CalcDeffShear(nDeffType, dHv, RebcD.dConcCenter);
			rCur.Comm.dAsv = CalcAsvShapeSB(RebcD, bUseMid, enDir);
		}
		else if ( strShape == D_SECT_SHAPE_REG_SR )
		{
			rCur.Comm.dDv  = 0.8*dHv;
			rCur.Comm.dAsv = CalcAsvShapeSR(RebcD, bUseMid);
		}
		else ASSERT(0);
	}
	return true;
}

double CSeisEval::CalcDeffShear(int nDeffType, double dH, double dd0)
{
    double ddeff = 0.0;
    switch (nDeffType)
    {
    case D_SEME_EFF_DEPTH_AUTO: { ddeff = dH - dd0; } break;
    case D_SEME_EFF_DEPTH_08H : { ddeff = 0.8*dH;   } break; // 고려하는 방향의 단면의 유효줌 (0.8h로 함. h=고려하는 방향의 전체단면춤)
    default: ASSERT(FALSE); break;
    }
    return ddeff;
}

double CSeisEval::CalcAsvShapeSB(const T_REBC_D& RebcD, BOOL bUserMid, EN_DIR enDir)
{
    if (bUserMid)
    {
        return RebcD.iSubRebarNum2[enDir] * GetRebarAs(RebcD.SubRebarName2);
    }
    return RebcD.iSubRebarNum[enDir] * GetRebarAs(RebcD.SubRebarName);
}

double CSeisEval::CalcAsvShapeSR(const T_REBC_D& RebcD, BOOL bUserMid)
{
    if (bUserMid)
    {
        return min(RebcD.iSubRebarNum2[0], RebcD.iSubRebarNum2[1]) * GetRebarAs(RebcD.SubRebarName2);
    }    
    return min(RebcD.iSubRebarNum[0], RebcD.iSubRebarNum[1]) * GetRebarAs(RebcD.SubRebarName);
}

bool CSeisEval::MakeWallShearParam(T_ELEM_K ElemK, EN_EVALSTEP enStep, EN_DIR enDir, OUT T_WALL_SHR_PARAM raShrAvg[EN_MEMBEND_NUM], OUT T_WALL_SHR_PARAM raShrNom[EN_MEMBEND_NUM])
{
    if ( !MakeWallShearParam(ElemK, enStep, enDir, false, raShrAvg) ) return false;
    if ( !MakeWallShearParam(ElemK, enStep, enDir, true, raShrNom) )  return false;
    return true;
}

bool CSeisEval::SetWallForcShearParam(const double adForc[2][6], EN_DIR enDirV, OUT T_WALL_SHR_PARAM raShrAvg[EN_MEMBEND_NUM], OUT T_WALL_SHR_PARAM raShrNom[EN_MEMBEND_NUM])
{
    const int nDOFV = enDirV == EN_DIR_Z ? DOF_FZ : DOF_FY;
    const int nDOFM = enDirV == EN_DIR_Z ? DOF_MY : DOF_MZ;

    for ( int i = 0; i < EN_MEMBEND_NUM; ++i )
    {
        const EN_WALL_FORC_POS& CurPos = GetWallForcPos(static_cast<EN_MEMBEND>(i));

        const double dPu = adForc[CurPos][DOF_FX] * (-1.0);
        const double dVu = adForc[CurPos][nDOFV];
        const double dMu = adForc[CurPos][nDOFM];

        raShrAvg[i].SetForc(dPu, dVu, dMu);
        raShrNom[i].SetForc(dPu, dVu, dMu);
    }
    return true;
}

void CSeisEval::GetWallMatlShear(EN_EVALSTEP enStep, const T_SECM_D& SecmD, bool bForcCtrl, OUT T_CON_SHR_PARAM& rShr)
{
    rShr.dFck = 0.0;
    rShr.dFys = 0.0;

    CSeisEvalDataTool Tool;
    bool bLinEval = Tool.IsLinEvalMethod(); /// 선형해석평가인 경우에는 수직철근강도를 공칭강도로 사용.

    switch ( enStep )
    {
    case EN_EVALSTEP_PRE:
        rShr.dFck = Tool.GetFc(enStep, EN_EVALMATL_NOM, SecmD);
        rShr.dFys = bLinEval ? Tool.GetFys(enStep, EN_EVALMATL_NOM, SecmD) : Tool.GetFys(enStep, EN_EVALMATL_AVG, SecmD);
        break;
    case EN_EVALSTEP_1ST:
    case EN_EVALSTEP_2ND:
        rShr.dFck = Tool.GetFc(enStep, EN_EVALMATL_NOM, SecmD);
        rShr.dFys = bForcCtrl ? Tool.GetFys(enStep, EN_EVALMATL_NOM, SecmD) : Tool.GetFys(enStep, EN_EVALMATL_AVG, SecmD);
        break;
    default: ASSERT(0);
        break;
    }
}

void CSeisEval::GetWallMatlShear(const T_PDCM_D& PdcmD, OUT T_CON_SHR_PARAM& rShr)
{
	rShr.dFck = 0.0;
	rShr.dFys = 0.0;

	CSeisEvalDataTool Tool;	
	rShr.dFck = Tool.GetFc(EN_EVALMATL_NOM, PdcmD);
	rShr.dFys = Tool.GetFys(EN_EVALMATL_NOM, PdcmD);
}

double CSeisEval::CalcDutilityFactor(double dMu, double dMe)
{
    return fabs(dMe) < cEvalZero ? 0.0 : fabs(dMu/(0.9*dMe));
}

double CSeisEval::CalcParamDDyFactStl(double dBTR, double dDTR, double adBTR[2], double adDTR[2], double dParamSC, double dParamNC)
{
    auto l_CalcParam = [](double dWTR, double dWTR4SC, double dWTR4NC, double dValSC, double dValNC)
    {
        if (dWTR < dWTR4SC) return dValSC;
        if (dWTR > dWTR4NC) return dValNC;

        return dValSC - (dWTR4SC - dWTR)*(dValSC - dValNC)/(dWTR4SC - dWTR4NC);
    };

    double dFlg = l_CalcParam(dBTR, adBTR[WTR_SC], adBTR[WTR_NC], dParamSC, dParamNC);
    double dWeb = l_CalcParam(dDTR, adDTR[WTR_SC], adDTR[WTR_NC], dParamSC, dParamNC);

    return min(dFlg, dWeb);
}

double CSeisEval::CalcStlSlenderness(T_ELEM_K ElemK, int nPosi, const T_SECT_SECTBASE_D& SectBaseD)
{
    CDgnDataCtrl DataCtrl;
    double dKy = 1.0;
    double dKz = 1.0;
    T_KFAC_D KfacD, KfacAutoD;
    if (DataCtrl.Get_DgnGenKfac(ElemK, KfacD, KfacAutoD))
    {
        dKy = KfacD.BKy;
        dKz = KfacD.BKz;
    }

    T_LENG_D LengD;
    double dLeng = 0.0;
    double dLy = 1.0;
    double dLz = 1.0;
    if (DataCtrl.Get_DgnGenLeng(ElemK, nPosi, LengD, dLeng)) 
    {
        dLy = LengD.BLy;
        dLz = LengD.BLz;
    }

    double dry = sqrt(SectBaseD.Stiffness.Ryy/SectBaseD.Stiffness.Area);
    double drz = sqrt(SectBaseD.Stiffness.Rzz/SectBaseD.Stiffness.Area);
    double drp = SectBaseD.Design.Rop;

    double dSlenp = drp > 0.0 ? min(dKy*dLy, dKz*dLz)/drp : 0.0;
    double dSleny = (dKy*dLy)/dry;
    double dSlenz = (dKz*dLz)/drz;
    return max(dSlenp, max(dSleny, dSlenz));
}

BOOL CSeisEval::CalcWTR(const CString& strShape, const T_SECT_SECTBASE_D& SectBaseD, OUT double& rdBTR, OUT double& rdDTR)
{
    if ( strShape == D_SECT_SHAPE_REG_L  ) { return CalcWTRShapeL(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_C  ) { return CalcWTRShapeH(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_H  ) { return CalcWTRShapeH(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_T  ) { return CalcWTRShapeT(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_B  ) { return CalcWTRShapeB(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_P  ) { return CalcWTRShapeP(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_2L ) { return CalcWTRShapeL(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_2C ) { return CalcWTRShape2C(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_SB ) { return CalcWTRShapeSB(SectBaseD.Size, rdBTR, rdDTR); }
    if ( strShape == D_SECT_SHAPE_REG_SR ) { return CalcWTRShapeSR(SectBaseD.Size, rdBTR, rdDTR); }

    ASSERT(0);
    return FALSE;
}

BOOL CSeisEval::CalcWTRShapeL(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{
    double dH  = adSize[0];
    double dB  = adSize[1];
    double dtw = adSize[2];
    double dtf = fabs(adSize[3]) < cEvalZero ? adSize[2] : adSize[3];

    rdBTR = dB/dtf;
    rdDTR = dH/dtw;

    return TRUE;
}

BOOL CSeisEval::CalcWTRShapeH(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{
    double dH  = adSize[0];
    double dB  = adSize[1];
    double dtw = adSize[2];
    double dtf = adSize[3];
    double dB2 = fabs(adSize[4]) < cEvalZero ? dB  : adSize[4];
    double dt2 = fabs(adSize[5]) < cEvalZero ? dtf : adSize[5];
    double dr1 = adSize[6];

    rdBTR = 0.5*dB/dtf;
    rdDTR = (dH - dtf - dt2 - 2.0*dr1)/dtw;

    return TRUE;
}

BOOL CSeisEval::CalcWTRShapeT(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{
    double dH  = adSize[0];
    double dB  = adSize[1];
    double dtw = adSize[2];
    double dtf = adSize[3];

    rdBTR = 0.5*dB/dtf;
    rdDTR = dH/dtw;

    return TRUE;
}

BOOL CSeisEval::CalcWTRShapeB(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{
    double dH  = adSize[0];
    double dB  = adSize[1];
    double dtw = adSize[2];
    double dtf = fabs(adSize[3]) < cEvalZero ? adSize[2] : adSize[3];
    double dt2 = fabs(adSize[5]) < cEvalZero ? dtf : adSize[5];

    rdBTR = (dB-2.0*dtw)/dtf;
    rdDTR = (dH-dtf-dt2)/dtw;

    return TRUE;
}

BOOL CSeisEval::CalcWTRShapeP(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{
    const double dD = adSize[0];
    const double dT = adSize[1];

    rdBTR = dgn::SafeDiv(dD, dT);
    rdDTR = rdBTR;

    return TRUE;
}

BOOL CSeisEval::CalcWTRShape2C(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{
    const double dH  = adSize[0];
    const double dB  = adSize[1];
    const double dtw = adSize[2];
    const double dtf = dgn::EQ0(adSize[3]) ? adSize[2] : adSize[3];

    rdBTR = dgn::SafeDiv(dB-2.0*dtw, dtf);
    rdDTR = dgn::SafeDiv(dH-2.0*dtf, dtw);

    return TRUE;
}

BOOL CSeisEval::CalcWTRShapeSB(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{
    const double dH = adSize[0];
    const double dB = adSize[1];

    rdBTR = max(dgn::SafeDiv(dH, dB), dgn::SafeDiv(dB, dH));
    rdDTR = rdBTR;

    return TRUE;
}

BOOL CSeisEval::CalcWTRShapeSR(const double adSize[], OUT double& rdBTR, OUT double& rdDTR)
{    
    rdBTR = 0.0;
    rdDTR = 0.0;

    return TRUE;
}

void CSeisEval::mathUCS2GCSAsABS(OUT double& rdx, OUT double& rdy, OUT double& rdz, double ucs[3][3])
{
    double dgx = rdx*fabs(ucs[0][0]) + rdy*fabs(ucs[1][0]) + rdz*fabs(ucs[2][0]);
    double dgy = rdx*fabs(ucs[0][1]) + rdy*fabs(ucs[1][1]) + rdz*fabs(ucs[2][1]);
    double dgz = rdx*fabs(ucs[0][2]) + rdy*fabs(ucs[1][2]) + rdz*fabs(ucs[2][2]);
    rdx = dgx, rdy = dgy, rdz = dgz;
}

UINT CSeisEval::GetColmLocalAxisType(T_ELEM_K ElemK)
{
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
    double dLocalVec[3][3] = { 0.0, };
    if (!m_pDoc->calcElemLocalVector(ElemD, dLocalVec)) { ASSERT(0); return FALSE; }

    const double dTol = 1.e-8;
    double dNormal [3] = { 0.0, };
    double dGlobalX[3] = { 1.0, 0.0, 0.0 };
    CMathFunc::mathNormalize(dLocalVec[1], dNormal);
    double dAngleX = fabs(CMathFunc::mathCrossAngle(dNormal, dGlobalX));
    if (fabs(dAngleX) < dTol || fabs(dAngleX-180.0) < dTol ) 
    {
        return EN_MEMBAXIS_LYGX; 
    }
    else if (fabs(dAngleX-90.0) < dTol)
    {
        return EN_MEMBAXIS_LYGY; 
    }
    else
    {
        return EN_MEMBAXIS_LYNONE;
    }	
}

UINT CSeisEval::GetWallLocalAxisType(T_ELEM_K ElemK)
{
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }

    T_NODE_D NodeD_I, NodeD_J;
    if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD_I)) { ASSERT(FALSE); return FALSE; }
    if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD_J)) { ASSERT(FALSE); return FALSE; }

    const double dTol = 1.e-8;
    double dLocalVec[3] = { NodeD_I.x - NodeD_J.x, NodeD_I.y -NodeD_J.y, 0.0 };
    double dNormal[3]   = { 0.0, }; 
    double dGlobalX[3]  = { 1.0, 0.0, 0.0 };

    CMathFunc::mathNormalize(dLocalVec, dNormal);
    double dAngleX = fabs(CMathFunc::mathCrossAngle(dNormal, dGlobalX));
    if (fabs(dAngleX) < dTol || fabs(dAngleX-180.0) < dTol ) 
    {
        return EN_MEMBAXIS_LYGX; 
    }
    else if (fabs(dAngleX-90.0) < dTol)
    {
        return EN_MEMBAXIS_LYGY; 
    }
    else
    {
        return EN_MEMBAXIS_LYNONE;
    }	
}

double CSeisEval::CalcDCRDisp(double dQUD, double dQCE, double dm)
{
    return fabs(dQCE*dm) < cEvalZero ? cEvalMaxRat : fabs(dQUD/(dQCE*dm));
}

double CSeisEval::CalcDCRForc(double dQUF, double dQCN)
{
    return fabs(dQCN) < cEvalZero ? cEvalMaxRat : fabs(dQUF/dQCN);
}

void CSeisEval::MultiplyScaleFactorMFact(double dFactor, OUT double radMFact[3])
{
    radMFact[EN_LEVEL_IO] *= dFactor;
    radMFact[EN_LEVEL_LS] *= dFactor;
    radMFact[EN_LEVEL_CP] *= dFactor;
}

void CSeisEval::SetFinalMe(double dMeorg, double dMeV, OUT T_EVAL1ST_COLM_DIR& rEvalDir)
{
    rEvalDir.Info.dMe = (rEvalDir.Info.enCtrlType == EN_EVALCTRL_SHER) ? dMeV : dMeorg;
}

double CSeisEval::CalcDCRM(double dMuy, double dMuz, double dMey, double dmFacy, double dMez, double dmFacz, double dAlpha)
{
    if (fabs(dmFacy*dMey) < cEvalZero || fabs(dmFacz*dMez) < cEvalZero)
    {
        return 99.99;
    }

    double dParamy = fabs(dMuy/(dmFacy*dMey));
    double dParamz = fabs(dMuz/(dmFacz*dMez));
    return pow(dParamy, dAlpha) + pow(dParamz, dAlpha);  /// FEMA356과 시안 예제집에서는 Root를 씌우지 않았음.
}

double CSeisEval::CalcDCRStlColm(EN_LEVEL enLevel, const T_EVAL1ST_STLCOLM_POS& EvalPos)
{
    double dDCR = 0.0;
    double dmFacy = EvalPos.dmFacy[enLevel];
    double dmFacz = EvalPos.dmFacz[enLevel];
    double dmFacT = EvalPos.dmFacT[enLevel];

    /// DCR 계산시 인장압축의 기준은 PUD의 부호로 고려함.
    if (EvalPos.dPUD < 0.0)
    {
        double dPUF = EvalPos.dPUF;
        double dPCL = EvalPos.dPCL;
        if (fabs(dPUF) > 0.5*dPCL)
        {
            dDCR = fabs(dPUF)/dPCL + fabs(EvalPos.dMUFy/EvalPos.dMCLy) + fabs(EvalPos.dMUFz/EvalPos.dMCLz);
        }
        else if (fabs(dPUF) < 0.2*dPCL)
        {
            dDCR = fabs(dPUF)/(2.0*dPCL) + fabs(EvalPos.dMUDy)/(dmFacy*EvalPos.dMCEy) + fabs(EvalPos.dMUDz)/(dmFacz*EvalPos.dMCEz);
        }
        else	
        {
            dDCR = fabs(dPUF)/dPCL + 8.0/9.0*(fabs(EvalPos.dMUDy)/(dmFacy*EvalPos.dMCEy) + fabs(EvalPos.dMUDz)/(dmFacz*EvalPos.dMCEz));
        }		
    }
    else
    {
        dDCR = fabs(EvalPos.dPUD/(dmFacT*EvalPos.dTCE)) + fabs(EvalPos.dMUDy/(dmFacy*EvalPos.dMCEy)) + fabs(EvalPos.dMUDz/(dmFacz*EvalPos.dMCEz));
    }		
    return dDCR;
}

BOOL CSeisEval::CheckConBeamForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVAL_CONRSLT& rBeamRslt)
{
    CServiceHeadOffice* pSVOffice = CServiceHeadOffice::Instance();
    if ( pSVOffice == nullptr ) { ASSERT(0); return FALSE; }

    rBeamRslt.ElemK  = ElemK;
    rBeamRslt.unType = unType;
    pSVOffice->ReqService(_ULS(rcs), EN_RCSSA_BEAM_SEIS_EVAL_LIN, (void*)&rBeamRslt);

    return TRUE;
}

BOOL CSeisEval::CheckConColmForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVAL_CONRSLT& rColmRslt)
{
    CServiceHeadOffice* pSVOffice = CServiceHeadOffice::Instance();
    if ( pSVOffice == nullptr ) { ASSERT(0); return FALSE; }

    rColmRslt.ElemK  = ElemK;
    rColmRslt.unType = unType;
    pSVOffice->ReqService(_ULS(rcs), EN_RCSSA_COLM_SEIS_EVAL_LIN, (void*)&rColmRslt);

    return TRUE;
}

BOOL CSeisEval::CheckConWallForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVAL_CONRSLT& rWallRslt)
{
    CServiceHeadOffice* pSVOffice = CServiceHeadOffice::Instance();
    if ( pSVOffice == nullptr ) { ASSERT(0); return FALSE; }

    rWallRslt.ElemK  = ElemK;
    rWallRslt.unType = unType;
    pSVOffice->ReqService(_ULS(rcs), EN_RCSSA_WALL_SEIS_EVAL_LIN, (void*)&rWallRslt);

    return TRUE;
}

BOOL CSeisEval::CheckStlSectFor1stEval(T_ELEM_K ElemK, OUT T_EVAL1ST_STLRSLT& rStlRslt)
{
    CServiceHeadOffice* pSVOffice = CServiceHeadOffice::Instance();
    if ( pSVOffice == nullptr ) { ASSERT(0); return FALSE; }

    rStlRslt.ElemK = ElemK;
    pSVOffice->ReqService(_ULS(acs), EN_ACSSA_CHK_SEIS_1STEVAL, (void*)&rStlRslt);

    return TRUE;
}

BOOL CSeisEval::CheckStlSectForLinEval(T_ELEM_K ElemK, UINT unType, OUT T_EVALLIN_STLRSLT& rStlRslt)
{
    CServiceHeadOffice* pSVOffice = CServiceHeadOffice::Instance();
    if ( pSVOffice == nullptr ) { ASSERT(FALSE); return FALSE; }

    rStlRslt.ElemK  = ElemK;
    rStlRslt.unType = unType;
    pSVOffice->ReqService(_ULS(acs), EN_ACSSA_CHK_SEIS_LINEVAL, (void*)&rStlRslt);

    return TRUE;
}

double CSeisEval::CalcMFactModifiedByLengTerm(double dmFact, double dCb, double dRatLbLpToLrLp)
{
    const double dme = dgn::EQ0(dRatLbLpToLrLp) ? dmFact : dCb*(dmFact - (dmFact - 1.0)*dRatLbLpToLrLp);
    return max(dme, 1.0); /// [MQC 20098] 계산된 값이 1보다 작거나 음수인 경우가 있어서 학교시설내진성능평가(2018)의 제한값을 고려합니다.
}

double CSeisEval::CalcMFactModifiedByStrnTerm(double dmFact, double dRatLbLpToLrLp)
{
    double dme = dmFact - (dmFact - 1.0)*dRatLbLpToLrLp;
    return max(dme, 1.0); /// [MQC 20098] 계산된 값이 1보다 작거나 음수인 경우가 있어서 학교시설내진성능평가(2018)의 제한값을 고려합니다.
}

BOOL CSeisEval::GetStrbAddtionStep(const T_STRB_D& RefStrbD, const T_STRB_D& NxtStrbD, double dDistRatio, OUT T_STRB_D& rStrbD)
{
    rStrbD.Initialize();
    for (int i=0; i<4; ++i)
    {
        for (int m=0; m<18; ++m)
        {
            rStrbD.dblForce[i][m] = RefStrbD.dblForce[i][m] + (NxtStrbD.dblForce[i][m] - RefStrbD.dblForce[i][m])*dDistRatio;
        }
    }
    return TRUE;
}

BOOL CSeisEval::GetStrwAddtionStep(const T_STRW_D& RefStrwD, const T_STRW_D& NxtStrwD, double dDistRatio, OUT T_STRW_D& rStrwD)
{
    rStrwD.Initialize();
    for (int i=0; i<2; ++i)
    {
        for (int m=0; m<6; ++m)
        {
            rStrwD.dblForce[i][m] = RefStrwD.dblForce[i][m] + (NxtStrwD.dblForce[i][m] - RefStrwD.dblForce[i][m])*dDistRatio;
        }
    }
    return TRUE;
}

double CSeisEval::GetThetay(const T_SEIS_PO_ANAL_RESULT_D& PORslt, int nPOS, int nDOF)
{
    return PORslt.dDisp[nPOS][nDOF] < 0.0 ? fabs(PORslt.dD1Neg[nPOS][nDOF]) : fabs(PORslt.dD1Pos[nPOS][nDOF]);
}

void CSeisEval::GetPORsltConBeamHingeMy(const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radV[2], OUT double radM[2], OUT double radDP[2], OUT double radDN[2])
{
    for (int i=0; i<2; ++i)
    {
        radV [i] = PORslt.dForce[i][DOF_FZ];
        radM [i] = PORslt.dForce[i][DOF_MY];
        radDP[i] = PORslt.dD1Pos[i][DOF_MY];
        radDN[i] = PORslt.dD1Neg[i][DOF_MY];
    }
}

void CSeisEval::GetPORsltConColmHingeFyz(UINT unDirV, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radmu[2])
{
    UINT unV = 0, unM = 0;
    switch (unDirV)
    {
    case EN_DIR_Y: unV = DOF_FY; unM = DOF_MZ; break;
    case EN_DIR_Z: unV = DOF_FZ; unM = DOF_MY; break;	
    default: ASSERT(FALSE); break;
    }

    for (int i=0; i<2; ++i)
    {
        radP [i] = PORslt.dForce[i][DOF_FX]*(-1.0);
        radV [i] = PORslt.dForce[i][unV];
        radM [i] = PORslt.dForce[i][unM];
        radmu[i] = PORslt.dDD1[i][unM];
    }
}

void CSeisEval::GetPORsltConColmHingeMyz(UINT unDirM, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radDy[2], OUT double radM1y[2], OUT double radM1z[2])
{
    UINT unV = 0, unM = 0;
    switch (unDirM)
    {
    case EN_DIR_Y: unV = DOF_FZ; unM = DOF_MY; break;
    case EN_DIR_Z: unV = DOF_FY; unM = DOF_MZ; break;	
    default: ASSERT(FALSE); break;
    }

    for (int i=0; i<2; ++i)
    {
        radP [i] = PORslt.dForce[i][DOF_FX]*(-1.0);
        radV [i] = PORslt.dForce[i][unV];
        radM [i] = PORslt.dForce[i][unM];
        radDy[i] = GetThetay(PORslt, i, unM);

        radM1y[i] = PORslt.dDisp[i][DOF_MY] < 0.0 ? PORslt.dP1Neg[i][DOF_MY] : PORslt.dP1Pos[i][DOF_MY];
        radM1z[i] = PORslt.dDisp[i][DOF_MZ] < 0.0 ? PORslt.dP1Neg[i][DOF_MZ] : PORslt.dP1Pos[i][DOF_MZ];
    }    
}

void CSeisEval::GetPORsltConWallHingeFy(const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2])
{
    for (int i=0; i<2; ++i)
    {
        radP[i] = PORslt.dForce[i][DOF_FX]*(-1.0);
        radV[i] = PORslt.dForce[i][DOF_FY];
        radM[i] = PORslt.dForce[i][DOF_MZ];		
    }
}

void CSeisEval::GetPORsltConWallHingeFz(const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radDyV[2])
{
    for (int i=0; i<2; ++i)
    {
        radP[i]   = PORslt.dForce[i][DOF_FX]*(-1.0);
        radV[i]   = PORslt.dForce[i][DOF_FZ];
        radM[i]   = PORslt.dForce[i][DOF_MY];		
        radDyV[i] = GetThetay(PORslt, i, DOF_FZ);
    }
}

void CSeisEval::GetPORsltConWallHingeMyz(UINT unDirM, const T_SEIS_PO_ANAL_RESULT_D& PORslt, OUT double radP[2], OUT double radV[2], OUT double radM[2], OUT double radDyM[2])
{
    UINT unV = 0, unM = 0;
    switch (unDirM)
    {
    case EN_DIR_Y: unV = DOF_FZ; unM = DOF_MY; break;
    case EN_DIR_Z: unV = DOF_FY; unM = DOF_MZ; break;	
    default: ASSERT(FALSE); break;
    }

    for (int i=0; i<2; ++i)
    {
        radP[i]   = PORslt.dForce[i][DOF_FX]*(-1.0);
        radV[i]   = PORslt.dForce[i][unV];
        radM[i]   = PORslt.dForce[i][unM];		
        radDyM[i] = GetThetay(PORslt, i, unM);
    }
}

void CSeisEval::GetPORsltForc(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT double adForc[2][6], OUT double adM1y[2], OUT double adM1z[2])
{
	for ( int i=0; i<2; ++i )
	{
		adM1y[i] = 0.0;
		adM1z[i] = 0.0;
		for ( int j=0; j<6; ++j )
		{
			adForc[i][j] = 0.0;
		}
	}
    
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { return; }

	const int nPhgeType = GetPhgeType(ElemK);

    T_PHGE_K PhgeK;
	if ( !m_pDoc->m_pAttrCtrl->MakeKeyPhge(nPhgeType, ElemK, PhgeK) ) { return; }

	T_PHGE_D PhgeD;
	if ( !m_pDoc->m_pAttrCtrl->GetPhge(PhgeK, PhgeD) ) { return; }

	T_PHGT_D PhgtD;
	if ( !m_pDoc->m_pAttrCtrl->GetPhgt(PhgeD.HingeTypeK, PhgtD) ) { return; }

	if ( nAddStepIdx >= 0 )
	{
		T_POAS_D PoasD;
		m_pDoc->m_pAttrCtrl->GetPoas(PoasD);
		nStep = PoasD.aStep[nAddStepIdx].nRefStep - 1;
	}

	const bool bFiber = PhgtD.nHingeType == 1 ? true : false;
	auto L_GetYieldStrn =[bFiber] (const T_EVAL2ND_PHNE& CurFhneD)
	{
		const bool bNegative = CurFhneD.dDisplacement < 0.0 ? true : false;
		if ( bFiber )
		{
			return bNegative ? CurFhneD.dP2_Neg : CurFhneD.dP2_Pos;
		}
		else
		{
			return bNegative ? CurFhneD.dP1_Neg : CurFhneD.dP1_Pos;
		}
	};

    for (int i=0; i<6; ++i)
    {
		if ( PhgtD.bDOF[i] )
		{		
			if ( PhgtD.nHingeType == 0 && PhgtD.nLoc[i]!=D_PHGT_LOCATION_IJ ) continue; // I&J만 고려
			if ( PhgtD.nHingeType == 1 && PhgtD.nLoc[i]==0 ) continue;

			const int nLoc = 2;
			for ( int j=0; j<nLoc; ++j )
			{
				T_EVAL2ND_PHNE FhneD;
				if ( !m_pDoc->m_pPostCtrl->GetPhnePushover4SeisEval(nPhgeType, ElemK, nAddStepIdx, PolcK, nStep, j, i, FhneD) ) continue;

				adForc[j][i] = FhneD.dForceMoment;

				switch ( i )
				{
				case DOF_MY: { adM1y[j] = L_GetYieldStrn(FhneD); } break;
				case DOF_MZ: { adM1z[j] = L_GetYieldStrn(FhneD); } break;
				default: break;
				}
			}
		}
		else
		{			
			if ( m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp) )
			{
				T_STRB_D StrbD;
				if ( m_pDoc->m_pPostCtrl->GetStrbPushover(PolcK, nStep, ElemK, StrbD) )
				{
					adForc[0][i] = StrbD.dblForce[0][i];
					adForc[1][i] = StrbD.dblForce[3][i+6];
				}
			}
			else if ( m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp) )
			{
				T_STRW_D StrwD;
				if ( m_pDoc->m_pPostCtrl->GetStrwPushover(PolcK, nStep, ElemK, StrwD) )
				{					
					adForc[0][i] = StrwD.dblForce[0][i];
					adForc[1][i] = StrwD.dblForce[1][i];
				}
			}			
		}
    }
}

void CSeisEval::GetPORsltForc(T_ELEM_K ElemK, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT double adForc[2][6])
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            adForc[i][j] = 0.0;
        }
    }

    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { return; }

    const int nPhgeType = GetPhgeType(ElemK);
    if (nPhgeType == 0) { return; }

    T_PHGE_K PhgeK;
    if (!m_pDoc->m_pAttrCtrl->MakeKeyPhge(nPhgeType, ElemK, PhgeK)) { return; }

    T_PHGE_D PhgeD;
    if (!m_pDoc->m_pAttrCtrl->GetPhge(PhgeK, PhgeD)) { return; }

    T_PHGT_D PhgtD;
    if (!m_pDoc->m_pAttrCtrl->GetPhgt(PhgeD.HingeTypeK, PhgtD)) { return; }

    if (nAddStepIdx >= 0)
    {
        T_POAS_D PoasD;
        m_pDoc->m_pAttrCtrl->GetPoas(PoasD);
        nStep = PoasD.aStep[nAddStepIdx].nRefStep - 1;
    }
       
    for (int i = 0; i < 6; ++i)
	{
		if (m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
		{
			T_STRB_D StrbD;
			if (m_pDoc->m_pPostCtrl->GetStrbPushover(PolcK, nStep, ElemK, StrbD))
			{
				adForc[0][i] = StrbD.dblForce[0][i];
				adForc[1][i] = StrbD.dblForce[3][i + 6];
			}
		}
		else if (m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp))
		{
			T_STRW_D StrwD;
			if (m_pDoc->m_pPostCtrl->GetStrwPushover(PolcK, nStep, ElemK, StrwD))
			{
				adForc[0][i] = StrwD.dblForce[0][i];
				adForc[1][i] = StrwD.dblForce[1][i];
			}
		}
	}
}

void CSeisEval::GetPORsltForcByInitialLoad(T_ELEM_K ElemK, OUT double adForc[2][6])
{
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            adForc[i][j] = 0.0;
        }
    }

    T_POGD_D PogdD;
    if (m_pDoc->m_pAttrCtrl->GetPogd(PogdD) && PogdD.nInitLoadMethod == 0)
    {
		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return; }

        BOOL bBeam = m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp);
        BOOL bTrus = m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp);
        BOOL bWall = m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp);

        const int nSize = PogdD.aInitLoad.GetSize();
        for (int i = 0; i < nSize; ++i)
        {
            const T_PVLC_BASE& CurPvlc = PogdD.aInitLoad[i];

            T_LCOM_D LcomD;
            if (!m_pDoc->m_pPostCtrl->MakeLoadComb(CurPvlc.LoadCaseType, CurPvlc.LoadCaseKey, LcomD)) { ASSERT(0); continue; }
            m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

            if (bBeam)
            {
                T_STRB_D StrbD;
                if (!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbD, NULL, NULL, TRUE)) { continue; }

                for (int j = 0; j < 6; ++j)
                {
                    adForc[0][j] += StrbD.dblForce[0][j] * CurPvlc.dScaleFactor;
                    adForc[1][j] += StrbD.dblForce[3][j + 6] * CurPvlc.dScaleFactor;
                }                
            }
            else if (bTrus)
            {
                T_STRT_D StrtD;
                if (!m_pDoc->m_pPostCtrl->GetStrtNew(ElemK, &StrtD, NULL, NULL)) { continue; }                
                adForc[0][0] += StrtD.dblForce[0] * CurPvlc.dScaleFactor;
                adForc[1][0] += StrtD.dblForce[1] * CurPvlc.dScaleFactor;
            }
            else if (bWall)
            {
                UINT unInternalWallID = 0;
                if (!m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID)) { return; }
                T_STRW_D StrwD;                
                if (!m_pDoc->m_pPostCtrl->GetStrwNew(unInternalWallID, &StrwD, NULL, NULL)) { continue; }

                for (int j = 0; j < 6; ++j)
                {
                    adForc[0][j] += StrwD.dblForce[0][j] * CurPvlc.dScaleFactor;
                    adForc[1][j] += StrwD.dblForce[1][j] * CurPvlc.dScaleFactor;
                }                
            }            
        }        
    }    
}

void CSeisEval::GetTHRsltForc(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex,  OUT double adForc[2][6])
{
	for ( int i=0; i<2; ++i )
	{
		for ( int j=0; j<6; ++j )
		{
			adForc[i][j] = 0.0;
		}
	}

	const UINT unLoadType = GetLoadMinMaxTypeTH(ThisK, nTimeIndex);
	
	T_ELEM_D ElemD;
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { return; }

	m_pDoc->m_pPostCtrl->SelectThis(ThisK);
	if ( m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp) )
	{
		T_STRB_D StrbD;	
		T_STRB_D TimeD;
		switch ( unLoadType )
		{
		case EN_LOADTYPE_NORMAL:
			{
				if ( !m_pDoc->m_pPostCtrl->GetStrbHistory(nTimeIndex, ElemK, StrbD) )
				{
					StrbD.Initialize();
				}
			}			
			break;
		case EN_LOADTYPE_ALL:
			{
				if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrbWithTime(ElemK, NULL, NULL, &StrbD, NULL, NULL, &TimeD) )
				{
					StrbD.Initialize();
				}
			}
			break;
		case EN_LOADTYPE_MAX:
			{
				if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrbWithTime(ElemK, &StrbD, NULL, NULL, &TimeD, NULL, NULL) )
				{
					StrbD.Initialize();
				}
			}
			break;
		case EN_LOADTYPE_MIN:
			{
				if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrbWithTime(ElemK, NULL, &StrbD, NULL, NULL, &TimeD, NULL) )
				{
					StrbD.Initialize();
				}
			}
			break;
		default:
			{
				ASSERT(0);
			}
			break;
		}		

		for ( int j=0; j<6; ++j )
		{
			adForc[0][j] = StrbD.dblForce[0][j];
			adForc[1][j] = StrbD.dblForce[3][j+6];
		}		
	}
	else if ( m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp) )
	{
		UINT unInternalWallID = 0;
		if ( !m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID) ) { return; }

		T_STRW_D StrwD;
		T_STRW_D TimeD;
		switch ( unLoadType )
		{
		case EN_LOADTYPE_NORMAL:
			{
				if ( !m_pDoc->m_pPostCtrl->GetStrwHistory(nTimeIndex, unInternalWallID, StrwD) )
				{
					StrwD.Initialize();
				}
			}
			break;
		case EN_LOADTYPE_ALL:
			{
				if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrwWithTime(unInternalWallID, NULL, NULL, &StrwD, NULL, NULL, &TimeD) )
				{
					StrwD.Initialize();
				}
			}
			break;
		case EN_LOADTYPE_MAX:
			{
				if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrwWithTime(unInternalWallID, &StrwD, NULL, NULL, &TimeD, NULL, NULL) )
				{
					StrwD.Initialize();
				}
			}
			break;
		case EN_LOADTYPE_MIN:
			{
				if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrwWithTime(unInternalWallID, NULL, &StrwD, NULL, NULL, &TimeD, NULL) )
				{
					StrwD.Initialize();
				}
			}
			break;
		default:
			{
				ASSERT(0);
			}
			break;
		}

		for ( int j=0; j<6; ++j )
		{
			adForc[0][j] = StrwD.dblForce[0][j];
			adForc[1][j] = StrwD.dblForce[1][j];
		}
	}
}

BOOL CSeisEval::GetNlghTH(T_ELEM_K ElemK, T_THIS_K ThisK, int nTimeIndex, int nDof, OUT T_NLHG_D& rNlhgDI, OUT T_NLHG_D& rNlhgDJ, OUT T_NLHG_D& rTimeDI, OUT T_NLHG_D& rTimeDJ)
{
    auto _L_ExistHingeLocation_I = [](int& nHingeLocation)
    {
        return nHingeLocation != 1 ? true : false;
    };

    auto _L_ExistHingeLocation_J = [](int& nHingeLocation)
	{
		return nHingeLocation != 0 ? true : false;
	};

	rNlhgDI.Initialize();
	rNlhgDJ.Initialize();
	rTimeDI.Initialize();
	
	T_INELASTICDYN_HINGE_INFO InelHngeInfo;
	if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetInelasticDynHngtDataElem(ElemK, InelHngeInfo) ) { return FALSE; }

	int nIndex = 0;	
	const BOOL bExist = [&]()
	{
		nIndex = -1;
		const int nSize = InelHngeInfo.aInelasticDynHingeInfo.GetSize();		
		for ( int i=0; i<nSize; ++i )
		{
			if ( nDof == InelHngeInfo.aInelasticDynHingeInfo[i].nHingeDirection )
			{
				nIndex = i;
				return TRUE;
			}
		}
		return FALSE;
	}();

	if ( !bExist ) { return FALSE; }
	if ( nIndex < 0 ) { return FALSE; }
	
	T_INELASTICDYN_HINGE_INFO_BASE *pHngeInfoBase = &(InelHngeInfo.aInelasticDynHingeInfo[nIndex]);	
	//if ( pHngeInfoBase->nHingeType == 0 && (pHngeInfoBase->nHingeLocation != 2 && pHngeInfoBase->nHingeLocation != 3) ) { return FALSE; } /// I&J 가 아니면 skip.	
	if ( pHngeInfoBase->nHingeType == 1 && pHngeInfoBase->nNumHinge == 0 ) { return FALSE; }
	if ( !m_pDoc->IsTHHysModelFEMA(pHngeInfoBase->nHysModel) && !pHngeInfoBase->bLumpType) { return FALSE; } /// FEMA type이 아니면 skip.
	
	m_pDoc->m_pPostCtrl->SelectThis(ThisK);
	const UINT unLoadType = GetLoadMinMaxTypeTH(ThisK, nTimeIndex);
	const int nStartHingeId = (unLoadType ==EN_LOADTYPE_NORMAL) ? pHngeInfoBase->nStartHingeId_SelectOuputOnly : pHngeInfoBase->nStartHingeId;

	BOOL bRotateDof = nDof >= DOF_MX ? TRUE : FALSE;	
	const int nHingeType = pHngeInfoBase->nHingeType;
	BOOL bForceSign = FALSE;

	int nStartHingeIdI = nStartHingeId;
	int nStartHingeIdJ = nDof == DOF_FX ? nStartHingeId : nStartHingeId + 1;
    if (pHngeInfoBase->nHingeLocation == 1) nStartHingeIdJ = nStartHingeIdI;

	switch ( unLoadType )
	{
	case EN_LOADTYPE_NORMAL:
		{
            if (_L_ExistHingeLocation_I(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetNlhgHistory(nTimeIndex, nStartHingeIdI, nHingeType, bRotateDof, rNlhgDI, nDof)) { return FALSE; }
            }
            if (_L_ExistHingeLocation_J(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetNlhgHistory(nTimeIndex, nStartHingeIdJ, nHingeType, bRotateDof, rNlhgDJ, nDof)) { return FALSE; }
            }
		}
		break;
	case EN_LOADTYPE_ALL:
        {
            if (_L_ExistHingeLocation_I(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetHgsm(nStartHingeIdI, nHingeType + 1, bRotateDof, NULL, NULL, &rNlhgDI, NULL, NULL, &rTimeDI, NULL, bForceSign, nDof)) { return FALSE; }
            }
            if (_L_ExistHingeLocation_J(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetHgsm(nStartHingeIdJ, nHingeType + 1, bRotateDof, NULL, NULL, &rNlhgDJ, NULL, NULL, &rTimeDJ, NULL, bForceSign, nDof)) { return FALSE; }
            }
		}
		break;
	case EN_LOADTYPE_MAX:
		{
            if (_L_ExistHingeLocation_I(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetHgsm(nStartHingeIdI, nHingeType + 1, bRotateDof, &rNlhgDI, NULL, NULL, &rTimeDI, NULL, NULL, NULL, bForceSign, nDof)) { return FALSE; }
            }
            if (_L_ExistHingeLocation_J(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetHgsm(nStartHingeIdJ, nHingeType + 1, bRotateDof, &rNlhgDJ, NULL, NULL, &rTimeDJ, NULL, NULL, NULL, bForceSign, nDof)) { return FALSE; }
            }
		}
		break;
	case EN_LOADTYPE_MIN:
		{
            if (_L_ExistHingeLocation_I(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetHgsm(nStartHingeIdI, nHingeType + 1, bRotateDof, NULL, &rNlhgDI, NULL, NULL, &rTimeDI, NULL, NULL, bForceSign, nDof)) { return FALSE; }
            }
            if (_L_ExistHingeLocation_J(pHngeInfoBase->nHingeLocation))
            {
                if (!m_pDoc->m_pPostCtrl->GetHgsm(nStartHingeIdJ, nHingeType + 1, bRotateDof, NULL, &rNlhgDJ, NULL, NULL, &rTimeDJ, NULL, NULL, bForceSign, nDof)) { return FALSE; }
            }
		}
		break;
	default:
		{
			ASSERT(0);
			return FALSE;
		}
		break;
	}
	
	return TRUE;
}

UINT CSeisEval::GetLoadMinMaxTypeTH(T_THIS_K ThisK, int nTimeIndex)
{
	CArray<double, double> aTimeStep;
	m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, aTimeStep);
	const int nTimeStepCount = aTimeStep.GetSize();

	if ( nTimeStepCount <= nTimeIndex )
	{
		if ( nTimeIndex == nTimeStepCount )
		{
			return EN_LOADTYPE_ALL;
		}
		else if ( nTimeIndex == nTimeStepCount+1 )
		{
			return EN_LOADTYPE_MAX;
		}
		else if ( nTimeIndex == nTimeStepCount+2 )
		{
			return EN_LOADTYPE_MIN;
		}
	}
	return EN_LOADTYPE_NORMAL;
}

void CSeisEval::CalcMFactTable2(const T_MEMBMFACT_TABLE_2& Table, double dA, double dB, OUT double radMFact[3])
{
    memset(radMFact, 0.0, sizeof(double)*3);

    double dRatLR = CalcDistRatio(dA, Table.dA1, Table.dA2);
    double dRatBT = CalcDistRatio(dB, Table.dB1, Table.dB2);

    for (int i=0; i<3; ++i)
    {
        const double dLB = Table.adTable_A1_B1[i];
        const double dRB = Table.adTable_A2_B1[i];
        const double dLT = Table.adTable_A1_B2[i];
        const double dRT = Table.adTable_A2_B2[i];
        radMFact[i] = CMathFunc::mathInterpolate(dLB, dRB, dLT, dRT, dRatLR, dRatBT);
    }
}

void CSeisEval::CalcMFactTable3(const T_MEMBMFACT_TABLE_3& Table, double dA, double dB, double dC, OUT double radMFact[3])
{
    memset(radMFact, 0.0, sizeof(double)*3);

    double dRatA = CalcDistRatio(dA, Table.dA1, Table.dA2);
    double dRatB = CalcDistRatio(dB, Table.dB1, Table.dB2);
    double dRatC = CalcDistRatio(dC, Table.dC1, Table.dC2);

    for ( int i=0; i<3; ++i )
    {
        const double dLB_C1 = Table.adTable_A1_B1_C1[i];
        const double dRB_C1 = Table.adTable_A2_B1_C1[i];
        const double dLT_C1 = Table.adTable_A1_B2_C1[i];
        const double dRT_C1 = Table.adTable_A2_B2_C1[i];        
        double dValC1 = CMathFunc::mathInterpolate(dLB_C1, dRB_C1, dLT_C1, dRT_C1, dRatA, dRatB);

        const double dLB_C2 = Table.adTable_A1_B1_C2[i];
        const double dRB_C2 = Table.adTable_A2_B1_C2[i];
        const double dLT_C2 = Table.adTable_A1_B2_C2[i];
        const double dRT_C2 = Table.adTable_A2_B2_C2[i];
        double dValC2 = CMathFunc::mathInterpolate(dLB_C2, dRB_C2, dLT_C2, dRT_C2, dRatA, dRatB);

        radMFact[i] = CMathFunc::mathInterpolate(dValC1, dValC2, dRatC);
    }   
}

void CSeisEval::CalcParamTable2(const T_MEMBPARAM_TABLE_2& Table, double dA, double dB, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double dRatLR = CalcDistRatio(dA, Table.dA1, Table.dA2);
    double dRatBT = CalcDistRatio(dB, Table.dB1, Table.dB2);

    for (int i=0; i<6; ++i)
    {
        const double dLB = Table.adTable_A1_B1[i];
        const double dRB = Table.adTable_A2_B1[i];
        const double dLT = Table.adTable_A1_B2[i];
        const double dRT = Table.adTable_A2_B2[i];
        radParam[i] = CMathFunc::mathInterpolate(dLB, dRB, dLT, dRT, dRatLR, dRatBT);
    }
}

void CSeisEval::CalcParamTable3(const T_MEMBPARAM_TABLE_3& Table, double dA, double dB, double dC, OUT double radParam[6])
{
    memset(radParam, 0.0, sizeof(double)*6);

    double dRatA = CalcDistRatio(dA, Table.dA1, Table.dA2);
    double dRatB = CalcDistRatio(dB, Table.dB1, Table.dB2);
    double dRatC = CalcDistRatio(dC, Table.dC1, Table.dC2);

    for (int i=0; i<6; ++i)
    {
        const double dLB_C1 = Table.adTable_A1_B1_C1[i];
        const double dRB_C1 = Table.adTable_A2_B1_C1[i];
        const double dLT_C1 = Table.adTable_A1_B2_C1[i];
        const double dRT_C1 = Table.adTable_A2_B2_C1[i];        
        double dValC1 = CMathFunc::mathInterpolate(dLB_C1, dRB_C1, dLT_C1, dRT_C1, dRatA, dRatB);

        const double dLB_C2 = Table.adTable_A1_B1_C2[i];
        const double dRB_C2 = Table.adTable_A2_B1_C2[i];
        const double dLT_C2 = Table.adTable_A1_B2_C2[i];
        const double dRT_C2 = Table.adTable_A2_B2_C2[i];
        double dValC2 = CMathFunc::mathInterpolate(dLB_C2, dRB_C2, dLT_C2, dRT_C2, dRatA, dRatB);

        radParam[i] = CMathFunc::mathInterpolate(dValC1, dValC2, dRatC);
    }   
}

void CSeisEval::CalcHingeParamConFromAB(double dThetay, double adTable[6], OUT double radParam[6])
{
    radParam[HG_A ] = CalcDDyFromAB(adTable[HG_A], dThetay);
    radParam[HG_B ] = CalcDDyFromAB(adTable[HG_B], dThetay);
    radParam[HG_C ] = adTable[HG_C];
    radParam[HG_IO] = CalcDDyFromAB(adTable[HG_IO], dThetay);
    radParam[HG_LS] = CalcDDyFromAB(adTable[HG_LS], dThetay);
    radParam[HG_CP] = CalcDDyFromAB(adTable[HG_CP], dThetay);
}

void CSeisEval::CalcHingeParamConFromDE(double dThetay, double adTable[6], OUT double radParam[6])
{
    radParam[HG_A ] = CalcDDyFromDE(adTable[HG_A], dThetay);
    radParam[HG_B ] = CalcDDyFromDE(adTable[HG_B], dThetay);
    radParam[HG_C ] = adTable[HG_C];
    radParam[HG_IO] = CalcDDyFromDE(adTable[HG_IO], dThetay);
    radParam[HG_LS] = CalcDDyFromDE(adTable[HG_LS], dThetay);
    radParam[HG_CP] = CalcDDyFromDE(adTable[HG_CP], dThetay);
}

void CSeisEval::CalcHingeParamStl(double adTable[6], OUT double radParam[6])
{
    radParam[HG_A ] = 1.0 + adTable[HG_A ];
    radParam[HG_B ] = 1.0 + adTable[HG_B ];
    radParam[HG_C ] =       adTable[HG_C ];
    radParam[HG_IO] = 1.0 + adTable[HG_IO];
    radParam[HG_LS] = 1.0 + adTable[HG_LS];
    radParam[HG_CP] = 1.0 + adTable[HG_CP];
}

int CSeisEval::GetColIndexMFactTable(int nSeisCompType, int nLevel)
{
    bool bPrimary = nSeisCompType == D_SCMT_TYPE_PRIMARY ? true : false;
    
    int nIdxLevel = 0;
    switch ( nLevel )
    {
    case EN_LEVEL_IO: { nIdxLevel = 0; } break;
    case EN_LEVEL_LS: { nIdxLevel = bPrimary ? 1 : 3; } break;
    case EN_LEVEL_CP: { nIdxLevel = bPrimary ? 2 : 4; } break;
    default: ASSERT(0); break;
    }
    return nIdxLevel;
}

int CSeisEval::GetColIndexParamTable(int nSeisCompType, int nParam)
{
    bool bPrimary = nSeisCompType == D_SCMT_TYPE_PRIMARY ? true : false;

    int nIdxParam = 0;
    switch ( nParam )
    {
    case HG_A:
    case HG_B:
    case HG_C:
    case HG_IO: 
        { nIdxParam = nParam; } 
        break;
    case HG_LS: { nIdxParam = bPrimary ? 4 : 6; } break;
    case HG_CP: { nIdxParam = bPrimary ? 5 : 7; } break;
    default: ASSERT(0); break;
    }
    return nIdxParam;
}


void CSeisEval::CalcEval1stConColmPos(double dAlpha, OUT T_EVAL1ST_COLM_POS& rEvalPos)
{
    const double dPn = rEvalPos.dPUF < 0.0 ? rEvalPos.dPnT : rEvalPos.dPnC;
    const double dPe = rEvalPos.dPUD < 0.0 ? rEvalPos.dPeT : rEvalPos.dPeC;
        
    for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP; ++i )
    {        
        const double dmForc = GetMFactForc(static_cast<EN_LEVEL>(i));

        if (rEvalPos.bForcCtrlP)
        {
            rEvalPos.DCRP.dDCR[i]  = CalcDCRForc(rEvalPos.dPUF, dPn*dmForc);
        }
        else
        {
            rEvalPos.DCRP.dDCR[i]  = CalcDCRDisp(rEvalPos.dPUD, dPe, rEvalPos.MfacP.admFac[i]);
        }
        
        rEvalPos.DCRVz.dDCR[i] = CalcDCRForc(rEvalPos.dVUFz, rEvalPos.dVnz*dmForc);
        rEvalPos.DCRVy.dDCR[i] = CalcDCRForc(rEvalPos.dVUFy, rEvalPos.dVny*dmForc);
        rEvalPos.DCRV.dDCR[i]  = max(rEvalPos.DCRVz.dDCR[i], rEvalPos.DCRVy.dDCR[i]);
        rEvalPos.DCRM.dDCR[i]  = CalcDCRM(rEvalPos.dMUDy, rEvalPos.dMUDz, rEvalPos.Diry.Info.dMe, rEvalPos.Diry.MfacM.admFac[i],
            rEvalPos.Dirz.Info.dMe, rEvalPos.Dirz.MfacM.admFac[i], dAlpha);
    }

    rEvalPos.DCRP.enLevel  = GetPerformLevel(rEvalPos.DCRP.dDCR);    
    rEvalPos.DCRVz.enLevel = GetPerformLevel(rEvalPos.DCRVz.dDCR);
    rEvalPos.DCRVy.enLevel = GetPerformLevel(rEvalPos.DCRVy.dDCR);
    rEvalPos.DCRV.enLevel  = GetPerformLevel(rEvalPos.DCRV.dDCR);
    rEvalPos.DCRM.enLevel  = GetPerformLevel(rEvalPos.DCRM.dDCR);

    rEvalPos.enLevel = max(rEvalPos.DCRP.enLevel, max(rEvalPos.DCRV.enLevel, rEvalPos.DCRM.enLevel));
}

void CSeisEval::CalcEval1stConWallPos(OUT T_EVAL1ST_WALL_POS& rEvalPos)
{
    const T_CONWALL_INFO& Info = rEvalPos.Info;
    if ( Info.bForceCtrl)
    {
        /// [GEN-1589] 하중지배거동인 경우에는 공칭강도에 의한 휨강도 고려.
        for ( int i = EN_LEVEL_IO; i <= EN_LEVEL_CP; ++i )
        {
            const double dmForc = GetMFactForc(static_cast<EN_LEVEL>(i));
            rEvalPos.DCRM.dDCR[i] = CalcDCRForc(rEvalPos.dMUF, Info.dMn*dmForc);
            rEvalPos.DCRV.dDCR[i] = CalcDCRForc(rEvalPos.dVUF, Info.dVn*dmForc);            
        }
        SetPerformLevelForcCtrl(rEvalPos.DCRM);
        SetPerformLevelForcCtrl(rEvalPos.DCRV);
    }
    else
    {       
        for ( int i = EN_LEVEL_IO ; i <= EN_LEVEL_CP ; ++i )
        {
            rEvalPos.DCRM.dDCR[i] = CalcDCRDisp(rEvalPos.dMUD, Info.dMe, rEvalPos.MfacMV.admFac[i]);
            rEvalPos.DCRV.dDCR[i] = CalcDCRDisp(rEvalPos.dVUD, Info.dVe, rEvalPos.MfacMV.admFac[i]);
        }				

        SetPerformLevel(rEvalPos.DCRM);
        SetPerformLevel(rEvalPos.DCRV);
    }	

    rEvalPos.enLevel = max(rEvalPos.DCRM.enLevel, rEvalPos.DCRV.enLevel);  /// 휨과 전단 성능수준을 각각 판단하고, 더 낮은 성능수준이 해당 위치의 성능수준임.
}

void CSeisEval::GetMn(int nPMMethod, EN_DIR enDir, const _DB_PMCV_DATA& Pmcv, const T_EVAL1ST_COLM_LCOM& EvalLcom, OUT double radMn[EN_MEMBEND_NUM])
{
    bool bDiry = enDir == EN_DIR_Y ? true : false;
        
    radMn[EN_MEMBEND_I] = CalcMn(nPMMethod, Pmcv, EvalLcom.PosI.dPUF, ( bDiry ? EvalLcom.PosI.dMUDy : EvalLcom.PosI.dMUDz ));
    radMn[EN_MEMBEND_J] = CalcMn(nPMMethod, Pmcv, EvalLcom.PosJ.dPUF, ( bDiry ? EvalLcom.PosJ.dMUDy : EvalLcom.PosJ.dMUDz ));        
}


double CSeisEval::CalcMn(int nPMMethod, const _DB_PMCV_DATA& Pmcv, double dPu, double dMu)
{
    double dMn = 0.0;
    switch ( nPMMethod )
    {
    case D_SEME_PMCM_E: dMn = CalcMnByEccen(Pmcv, dPu, fabs(dMu)); break;
    case D_SEME_PMCM_P: dMn = CalcMnByAxial(Pmcv, dPu); break;
    default: ASSERT(0); break;
    }
    return dMn;
}

double CSeisEval::CalcMnByAxial(const _DB_PMCV_DATA& Pmcv, double dPu)
{
    const double dLimUpon = pow(1.0e+7, 4);    

    double dPnMax = 0.0;
    double dPnMin = 0.0;
    double dPnPrev = (+1.0)*dLimUpon;
    double dPnNext = (-1.0)*dLimUpon;
    double dMnPrev = 0.0;
    double dMnNext = 0.0;
       
    POSITION pos = Pmcv.arPmcvUnit.GetStartPosition();
    while ( pos )
    {
        _DB_PMCV_UNIT PmcvUnit;
        PmcvUnit.Initialize();

        int nIndex = 0;        
        Pmcv.arPmcvUnit.GetNextAssoc(pos, nIndex, PmcvUnit);

        const double dPnCur = PmcvUnit.dPn;
        const double dMnCur = PmcvUnit.dMn;
        dPnMax = max(dPnCur, dPnMax);
        dPnMin = min(dPnCur, dPnMin);
        if ( dPnCur > dPu && dPnCur < dPnPrev )	
        {
            dPnPrev = dPnCur; 
            dMnPrev = dMnCur;
        }
        if ( dPnCur < dPu && dPnCur > dPnNext )
        {
            dPnNext = dPnCur;
            dMnNext = dMnCur;
        }
    }

    if ( dPu > dPnMax )
    {
        return 0.0; 
    }
    
    if ( dPu < dPnMin )
    {
        return 0.0;
    }
    
    if ( fabs(dPnPrev-dPnNext) < cEvalZero ) 
    {
        return dMnPrev; 
    }

    double dDivRatP = (dPnPrev-dPu)/(dPnPrev-dPnNext);
    return dMnPrev - (dMnPrev-dMnNext)*dDivRatP;    
}

double CSeisEval::CalcMnByEccen(const _DB_PMCV_DATA& Pmcv, double dPu, double dMu)
{
    if ( fabs(dMu) < cEvalZero ) 
    {
        return 0.0;
    }
    
    const double dEccnMax = 1.0e+28;
    const double dEccnFor = fabs(dMu) < cEvalZero ? ( dPu > 0.0 ? dEccnMax : -dEccnMax) : (dPu / fabs(dMu));

    CArray<T_POS_DBL_PAIR, T_POS_DBL_PAIR&> aPosDbl;
    POSITION pos = Pmcv.arPmcvUnit.GetStartPosition();
    while ( pos )
    {
        _DB_PMCV_UNIT PmcvUnit;
        PmcvUnit.Initialize();

        int nIndex = 0;        
        Pmcv.arPmcvUnit.GetNextAssoc(pos, nIndex, PmcvUnit);

        T_POS_DBL_PAIR PosDbl;
        PosDbl.nPos = nIndex;
        PosDbl.dVal = PmcvUnit.dPn;
        aPosDbl.Add(PosDbl);
    }

    qsort(aPosDbl.GetData(), aPosDbl.GetSize(), sizeof(T_POS_DBL_PAIR), CCompFunc::ComparePosDblPairA);

    double dMn = 0.0;
    int nSize = static_cast<int>(aPosDbl.GetSize());
    for ( int i = 1; i < nSize; ++i )
    {
        _DB_PMCV_UNIT CurPm;
        if ( !Pmcv.arPmcvUnit.Lookup(i, CurPm) ) { continue; } 
        _DB_PMCV_UNIT NxtPm;
        if ( !Pmcv.arPmcvUnit.Lookup(i+1, NxtPm) ) { continue; }

        const double dPnCur = CurPm.dPn;
        const double dMnCur = CurPm.dMn;
        const double dEccnCur = fabs( dMnCur ) < cEvalZero ? dEccnMax : (dPnCur / dMnCur);

        const double dPnNxt = NxtPm.dPn;
        const double dMnNxt = NxtPm.dMn;
        const double dEccnNxt = fabs( dMnNxt ) < cEvalZero ? (dPnNxt < 0.0 ? -dEccnMax : dEccnMax) : (dPnNxt / dMnNxt);

        if (fabs(dEccnFor - dEccnCur) < cEvalZero)
        {
            dMn = dMnCur;
            break;
        }
        if (fabs(dEccnFor - dEccnNxt) < cEvalZero)
        {
            dMn = dMnNxt;
            break;
        }
        if ( (dEccnFor - dEccnCur)*(dEccnFor - dEccnNxt) < 0.0 )
        {
            double adLine[2][2] = { { 0.0, 0.0 }, { dMu, dPu } };
            double adstP[2] = { CurPm.dMn, CurPm.dPn };
            double adEdP[2] = { NxtPm.dMn, NxtPm.dPn };
            double adCrP[2] = { 0.0, };
            if (CMathFunc::mathLineSegCross2D(adLine, adstP, adEdP, adCrP) > 0)
            {
                dMn = adCrP[0];
            }
            else
            {
                dMn = fabs(dMu) + cEvalZero;
            }
            break;
        }
    }

    return dMn;
}

void CSeisEval::SetVpColmInfo(double dMeEnd[2], double dMe, double dL, OUT T_CONCOLM_INFO& rInfo)
{
    rInfo.dMe  = dMe;
    rInfo.dMei = dMeEnd[EN_MEMBEND_I];
    rInfo.dMej = dMeEnd[EN_MEMBEND_J];    
    rInfo.dL   = dL;
    rInfo.dVp  = fabs(dL) < cEvalZero ? 0.0 : ( dMeEnd[EN_MEMBEND_I] + dMeEnd[EN_MEMBEND_J] )/dL;        
}

void CSeisEval::CalConColmLengthConsiderInfillStrutPosition(T_ELEM_K ElemK, double adL[2])
{
	adL[0] = 0.0;
	adL[1] = 0.0;

	T_POGD_D PogdD;
	if ( !m_pDoc->m_pAttrCtrl->GetPogd(PogdD) ) { PogdD.Initialize(); }

	T_SEME_D SemeD;
	if ( !m_pDoc->m_pAttrCtrl2->GetSeme(SemeD) ) { SemeD.Initialize(); }

	T_MEMB_K MembK = 0;
	if ( m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK, MembK) && PogdD.bAssignByMember && SemeD.data.eval.bColumnHingePosConsiderInfillStrut )
	{
		T_NODE_K aNodeK[4] = { 0, };
		if ( m_ColmDivByInfill.GetColumnNodeByInfillStrut(MembK, aNodeK) )
		{
			auto L_GetLength = [&] (T_NODE_K NodeK1, T_NODE_K NodeK2)
			{
				T_NODE_D NodeD1;
				if ( !m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeD1) ) { ASSERT(0); return 0.0; }

				T_NODE_D NodeD2;
				if ( !m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeD2) ) { ASSERT(0); return 0.0; }

				return CMathFunc::mathLength(NodeD1.x, NodeD1.y, NodeD1.z, NodeD2.x, NodeD2.y, NodeD2.z);
			};

			adL[0] = L_GetLength(aNodeK[0], aNodeK[1]);
			adL[1] = L_GetLength(aNodeK[2], aNodeK[3]);
		}
		else
		{
			CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
			adL[0] = pMembCtrl->GetMembLengByIncludeElem(ElemK);
			adL[1] = adL[0];
		}
	}
	else
	{
		CMembCtrl* pMembCtrl = CMembCtrl::GetMembCtrl();
		adL[0] = pMembCtrl->GetMembLengByIncludeElem(ElemK);
		adL[1] = adL[0];
	}
}

bool CSeisEval::CalcP1FemaTypeIehg(T_ELEM_K ElemK, int nDOF, bool bEndI, OUT double& rdP1Neg, OUT double& rdP1Pos) /// FEMA 형만 지원합니다.
{
	T_IEHG_D IehgD;
	if ( !m_pDoc->m_pAttrCtrl->GetIehgByElemK(ElemK, IehgD) ) { ASSERT(0); return false; }

	T_IEHP_D IehpD;
	if ( !m_pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD) ) { ASSERT(0); return false; }

    T_IEHG_K IehgK = m_pDoc->m_pAttrCtrl->MakeElemIehgK(ElemK);
    m_pDoc->m_pAttrCtrl->CalcIehpPropValue(IehpD, IehgK.key.objK, IehpD.AllProp.PROP, TRUE);
    m_pDoc->m_pAttrCtrl->CalcIehpPropValue(IehpD, IehgK.key.objK, IehpD.AllSubProp.PROP, TRUE, TRUE);
    m_pDoc->m_pAttrCtrl->CalcIehpYield(IehpD, IehgK.key.objK, IehpD.AllProp.PROP[6].YLDSUR, IehpD.AllSubProp.PROP[6].YLDSUR);

	auto L_GetP1 = [IehpD, nDOF] (const T_IEHP_YLDSUR& YLDSUR, const T_IEHP_PROP& PROP, int nIndex)
	{
		if ( (IehpD.nInteractionType == 1 || IehpD.nInteractionType == 2) && nDOF != 1 && nDOF != 2 )
		{
			// Iehp dialog의 Yield Surface... 참고..
			switch ( nDOF )
			{
			case 0: return YLDSUR.dPY[nIndex];
			case 3: return 0.0;
			case 4: return YLDSUR.dMY[0];
			case 5: return YLDSUR.dMY[1];
			default: return 0.0;
			}
		}
		else
		{
			switch ( IehpD.nHysModel[nDOF] )
			{
			case D_IEHP_FBIL:
			case D_IEHP_FINF:
			case D_IEHP_FTRI:
				{
					const T_IEHP_FEMA& FEMA = PROP.FEMA;
					return (nDOF < 3) ? FEMA.dYieldForce[nIndex] : FEMA.dYieldMoment[nIndex];
				}
			default:
				{					
					return 0.0;
				}
			}
		}
	};

	const T_IEHP_PROP& IehpProp = [IehpD, nDOF, bEndI]
	{
		if ( bEndI ) return IehpD.AllProp.PROP[nDOF];
		return IehpD.AllProp.PROP[nDOF].FEMA.bSymmetric ? IehpD.AllProp.PROP[nDOF] : IehpD.AllSubProp.PROP[nDOF];
	}();
		
	rdP1Pos = L_GetP1(IehpD.AllProp.PROP[6].YLDSUR, IehpProp, 0);
	rdP1Neg = L_GetP1(IehpD.AllProp.PROP[6].YLDSUR, IehpProp, 1);

	return true;
}


BOOL CSeisEval::CalcFzHingePropConBeam(T_ELEM_K ElemK, const T_SEIS_PO_ANAL_RESULT_D& PORslt, const EN_NLANALTYPE& enNLType, double dRatioMuMy, OUT T_PHGT_FEMA& rPhgtFemaI, OUT T_PHGT_FEMA& rPhgtFemaJ)
{
    T_BEAM_SHR_PARAM aShr[EN_BEAM_POS_NUM];
    if ( !MakeBeamShearParam(ElemK, EN_EVALSTEP_2ND, aShr) ) { ASSERT(0); return FALSE;}

    for (int i=0; i<EN_MEMBEND_NUM; ++i)
    {
        const EN_BEAM_POS& enPos = GetBeamPos(static_cast<EN_MEMBEND>(i));
        const T_CON_SHR_PARAM& CurShr = aShr[enPos].GetParam(FALSE);

        T_PHGT_FEMA& CurPhgt = i==0 ? rPhgtFemaI : rPhgtFemaJ;

        CurPhgt.bSymmetric = FALSE;
        CurPhgt.bUserDefineRatio = TRUE;
        CurPhgt.nUnloadStiffnessType = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
        SetCurveForcCtrlShear(CurPhgt.dForceMomentRatio);
        SetAccetanceCriteriaForcCtrl(CurPhgt);

        double dCurVn = CalcVnConBeam(CurShr);
        CurPhgt.dYieldForce[IDX_PLS] = dCurVn;
        CurPhgt.dYieldForce[IDX_MNS] = dCurVn;
    }
    return TRUE;
}

bool CSeisEval::MakeRebarSizeMap4CodeUnit()
{
    m_mapRebarSize.RemoveAll();

    int nConME = m_pDoc->m_pMatlDB->Get_RbarKindCon();	

    CStringArray aRBName;
    int nSize = m_pDoc->m_pMatlDB->GetRebarNameList(nConME, aRBName);
    for (int i=0; i<nSize; ++i)
    {
        _DGN_REBAR_DATA Rbar = m_pDoc->m_pMatlDB->Get_RebarData(aRBName[i]);		
        m_mapRebarSize.SetAt(aRBName[i], Rbar);
    }

    return true;
}

bool CSeisEval::CalcAsymmetricStiffFxHingeConColm(T_ELEM_K ElemK, OUT T_PHGT_SKCV& rSkcvI, OUT T_PHGT_SKCV& rSkcvJ)
{
    rSkcvI.bInitStiffAssymetric = TRUE;
    rSkcvI.nInitStiffMethod = 3;

    rSkcvJ = rSkcvI;

    T_SECT_D SectD;
    T_MATD_D MatdD;    
	switch ( m_pDoc->m_pAttrCtrl2->GetSemeMethod() )
	{
	case D_SEME_METHOD_KISTEC2013:
	case D_SEME_METHOD_KISTEC2019:
	case D_SEME_METHOD_MOE2018:
	case D_SEME_METHOD_MOE2019:
		{
			T_SECM_D SecmD;
			if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, SecmD) ) { ASSERT(0); return false; }
		}
		break;
	case D_SEME_METHOD_AIK_G_001_2021:
		{
			T_PDCM_D PdcmD;
			if ( !GetElemMatlSectCon(ElemK, SectD, MatdD, PdcmD) ) { ASSERT(0); return false; }
		}
		break;
	default:
		{
			ASSERT(0);
		}
		break;
	} 

    T_REBC_D RebcD;
    if ( !m_pDoc->m_pAttrCtrl->GetRebcByElemK(ElemK, RebcD) ) { ASSERT(0); return false; }

    const double dL  = m_pDoc->calcLAVElem(ElemK);
    const double dEc = MatdD.Data1.Analysis.Elast;
    
    double dEs = 0.0;
    double dAsI = 0.0;    
    m_pDoc->m_pAttrCtrl2->GetRebarEsAs(ElemK, 0, dEs, dAsI);
    double dAcI = SectD.SectBefore.SectI.Stiffness.Area - dAsI; /// Ac = Ag - As.
    CalcConAxialStiff(dEs, dAsI, dEc, dAcI, dL, rSkcvI);
    
    double dAsJ = 0.0;
    m_pDoc->m_pAttrCtrl2->GetRebarEsAs(ElemK, 1, dEs, dAsJ);
    double dAcJ = dAcI;
    if ( SectD.nStype == D_SECT_TYPE_TAPERED )
    {
        dAcJ = SectD.SectBefore.SectJ.Stiffness.Area - dAsJ; /// Ac = Ag - As.
    }
    CalcConAxialStiff(dEs, dAsJ, dEc, dAcJ, dL, rSkcvJ);
    
    return true;
}

bool CSeisEval::CalcAsymmetricStiffFxHingeConWall(T_ELEM_K ElemK, OUT T_PHGT_SKCV& rSkcvI, OUT T_PHGT_SKCV& rSkcvJ)
{
    rSkcvI.bInitStiffAssymetric = TRUE;
    rSkcvI.nInitStiffMethod = 3;

    rSkcvJ = rSkcvI;
        
    T_THIK_D ThikD;
    T_MATD_D MatdD;
    T_SECM_D SecmD;
    if ( !GetElemMatlThik(ElemK, ThikD, MatdD, SecmD) ) { ASSERT(0); return false; }

    T_WALL_KEY WallKey;
    T_WALL_D WallD;  WallD.Initialize();
    if (!m_pDoc->m_pPostCtrl->GetWallDataFromElemK(ElemK, WallD, &WallKey))  {  return false; }
    if ( !WallD.bLineWall ) { return false; }

    T_REBW_K RebwK;
    RebwK.keymap = WallKey;
    T_REBW_D RebwD;
    if ( !m_pDoc->m_pAttrCtrl->GetRebw(RebwK, RebwD) ) { ASSERT(0); return false; }
    
    const double dH  = WallD.dHeight;
    const double dEc = MatdD.Data1.Analysis.Elast;
    const double dAc = WallD.dThickness * WallD.dLength;

    double dEs = 0.0;
    double dAsI = 0.0;    
    m_pDoc->m_pAttrCtrl2->GetRebarEsAs(ElemK, 0, dEs, dAsI);
    double dAcI = dAc - dAsI; /// Ac = Ag - As.
    CalcConAxialStiff(dEs, dAsI, dEc, dAcI, dH, rSkcvI);

    double dAsJ = 0.0;
    m_pDoc->m_pAttrCtrl2->GetRebarEsAs(ElemK, 1, dEs, dAsJ);
    double dAcJ = dAc - dAsJ;    
    CalcConAxialStiff(dEs, dAsJ, dEc, dAcJ, dH, rSkcvJ);

    return true;
}

void CSeisEval::CalcConAxialStiff(double dEs, double dAs, double dEc, double dAc, double dL, OUT T_PHGT_SKCV& rSkcv)
{
    rSkcv.dInitStiffUserTheta  [0] = fabs(dL) < cEvalZero ? 0.0 : dEs * dAs / dL;             /// tension. (+)EsAs/L.
    rSkcv.dInitStiffUserTheta_m[0] = fabs(dL) < cEvalZero ? 0.0 : ( dEc*dAc + dEs*dAs ) /dL;  /// compression. (-)EA/L.
}

CString CSeisEval::GetDetTextFileName()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if ( pDoc == nullptr ) { ASSERT(0); return _T(""); }

    CString strPathName = pDoc->GetPathName();
    CFileCtrl FileCtrl(strPathName);
    CString csDir = FileCtrl.GetFilePath() + FileCtrl.GetFileNameWithoutExtension() + _T("_") + _T("UpdateHinge");

    return csDir + _T("\\Detail.txt");
}

void CSeisEval::SetPhgtProp(int nElemType, int nMatlType, int nDefinition, int nInterPM, OUT T_PHGT_D& rPhgtD)
{
    rPhgtD.nElemType = nElemType;
    rPhgtD.nMaterial = nMatlType;
    rPhgtD.nDefinition = nDefinition;
    rPhgtD.nInteractionType = nInterPM;
}

void CSeisEval::SetInitPhgtPropComponentCon(UINT unDOF, BOOL bDOF, int nLoc, int nSkcvType, BOOL bSymIJ, int nInitStiff, OUT T_PHGT_D& rPhgtD)
{
    rPhgtD.bDOF[unDOF] = bDOF;
	/// [GEN-6988] 자유도가 OFF 되어 있어도 FEAM 형태의 힌지로 설정해 달라는 요청으로 자유도에 관계없이 요 항목들은 같이 생성.
	rPhgtD.nLoc[unDOF] = nLoc;
	rPhgtD.nSkcvType[unDOF] = nSkcvType;
	rPhgtD.Skcv[unDOF].bSymmetricIJ = bSymIJ;
	rPhgtD.Skcv[unDOF].nInitStiffMethod = nInitStiff;

    if (bDOF)
    {        
        GetInitialHingePropCon(unDOF, rPhgtD.Skcv[unDOF].FEMA);
    }	
}

void CSeisEval::SetInitPhgtPropComponentStl(UINT unDOF, BOOL bDOF, int nLoc, int nSkcvType, BOOL bSymIJ, int nInitStiff, OUT T_PHGT_D& rPhgtD)
{
    rPhgtD.bDOF[unDOF] = bDOF;
	/// [GEN-6988] 자유도가 OFF 되어 있어도 FEAM 형태의 힌지로 설정해 달라는 요청으로 자유도에 관계없이 요 항목들은 같이 생성.
	rPhgtD.nLoc[unDOF] = nLoc;
	rPhgtD.nSkcvType[unDOF] = nSkcvType;
	rPhgtD.Skcv[unDOF].bSymmetricIJ = bSymIJ;
	rPhgtD.Skcv[unDOF].nInitStiffMethod = nInitStiff;
    
	if (bDOF)
    {
        GetInitialHingePropStl(unDOF, rPhgtD.Skcv[unDOF].FEMA);
    }
}

void CSeisEval::SetInitPhgtPropComponentInfill(UINT unDOF, BOOL bDOF, UINT unInfillType, OUT T_PHGT_D& rPhgtD)
{
    rPhgtD.bDOF[unDOF] = bDOF;
    if (bDOF)
    {
        rPhgtD.nLoc[unDOF]      = D_PHGT_LOCATION_CENTER;
        rPhgtD.nSkcvType[unDOF] = D_SKCV_FEMA_IFST;
        rPhgtD.Skcv[unDOF].nCalcMethod = D_PHGT_CALC_USER;
        rPhgtD.Skcv[unDOF].bSymmetricIJ = TRUE;		
        rPhgtD.Skcv[unDOF].FEMA.nInfillStrutType = unInfillType;
        GetInitialHingePropInfill(unDOF, rPhgtD.Skcv[unDOF].FEMA);
    }
}


void CSeisEval::GetInitialHingePropCon(UINT unDOF, OUT T_PHGT_FEMA& rPhgtFEMA)
{
    rPhgtFEMA.bSymmetric = TRUE;
    rPhgtFEMA.bUserDefineRatio = TRUE;

    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_ME] = -1000.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MD] =  -999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MC] =  -999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MB] =    -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_A ] =     0.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_B ] =     1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_C ] =   999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_D ] =   999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_E ] =  1000.0;

    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_ME] = -0.2;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MD] = -0.2;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MC] = -1.1;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MB] = -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_A ] =  0.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_B ] =  1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_C ] =  1.1;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_D ] =  0.2;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_E ] =  0.2;

    switch (unDOF)
    {
    case DOF_FX:
    case DOF_FY:
    case DOF_FZ:
        {
            rPhgtFEMA.dYieldForce[0] = 1.0;
            rPhgtFEMA.dYieldForce[1] = 1.0;
        }
        break;
    case DOF_MX:
    case DOF_MY:
    case DOF_MZ:
        {
            rPhgtFEMA.dYieldMoment[0] = 1.0;
            rPhgtFEMA.dYieldMoment[1] = 1.0;
        }
        break;
    default: ASSERT(FALSE); break;
    }

    rPhgtFEMA.dIO[0] = rPhgtFEMA.dIO[1] = 2.0;
    rPhgtFEMA.dLS[0] = rPhgtFEMA.dLS[1] = 4.0;
    rPhgtFEMA.dCP[0] = rPhgtFEMA.dCP[1] = 6.0;
}

void CSeisEval::GetInitialHingePropStl(UINT unDOF, OUT T_PHGT_FEMA& rPhgtFEMA)
{
    rPhgtFEMA.bSymmetric = TRUE;
    rPhgtFEMA.bUserDefineRatio = TRUE;

    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_ME] = -1000.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MD] =  -999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MC] =  -999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MB] =    -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_A ] =     0.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_B ] =     1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_C ] =   999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_D ] =   999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_E ] =  1000.0;

    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_ME] = -0.2;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MD] = -0.2;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MC] = -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MB] = -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_A ] =  0.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_B ] =  1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_C ] =  1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_D ] =  0.2;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_E ] =  0.2;

    switch (unDOF)
    {
    case DOF_FX:
    case DOF_FY:
    case DOF_FZ:
        {
            rPhgtFEMA.dYieldForce[0] = 1.0;
            rPhgtFEMA.dYieldForce[1] = 1.0;
        }
        break;
    case DOF_MX:
    case DOF_MY:
    case DOF_MZ:
        {
            rPhgtFEMA.dYieldMoment[0] = 1.0;
            rPhgtFEMA.dYieldMoment[1] = 1.0;
        }
        break;
    default: ASSERT(FALSE); break;
    }

    rPhgtFEMA.dIO[0] = rPhgtFEMA.dIO[1] = 2.0;
    rPhgtFEMA.dLS[0] = rPhgtFEMA.dLS[1] = 4.0;
    rPhgtFEMA.dCP[0] = rPhgtFEMA.dCP[1] = 6.0;
}

void CSeisEval::GetInitialHingePropInfill(UINT unDOF, OUT T_PHGT_FEMA& rPhgtFEMA)
{
    rPhgtFEMA.bSymmetric = TRUE;
    rPhgtFEMA.bUserDefineRatio = TRUE;

    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_ME] = -1000.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MD] =  -999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MC] =  -999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_MB] =    -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_A ] =     0.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_B ] =     1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_C ] =   999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_D ] =   999.0;
    rPhgtFEMA.dForceMomentRatio[IDX_DDY][IDX_CURV_E ] =  1000.0;

    UINT unInfillType = rPhgtFEMA.nInfillStrutType;
    double dQQyD = unInfillType==EN_MSIS_PARTIAL_HEIGHT ? 1.0 : 0.0;

    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_ME] =  0.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MD] = -dQQyD;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MC] = -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_MB] = -1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_A ] =  0.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_B ] =  1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_C ] =  1.0;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_D ] =  dQQyD;
    rPhgtFEMA.dForceMomentRatio[IDX_FFY][IDX_CURV_E ] =  0.0;

    switch (unDOF)
    {
    case DOF_FX:
    case DOF_FY:
    case DOF_FZ:
        {
            rPhgtFEMA.dYieldForce[IDX_PLS] = 1.0;
            rPhgtFEMA.dYieldForce[IDX_MNS] = 1.0;
        }
        break;
    case DOF_MX:
    case DOF_MY:
    case DOF_MZ:
        {
            rPhgtFEMA.dYieldMoment[IDX_PLS] = 1.0;
            rPhgtFEMA.dYieldMoment[IDX_MNS] = 1.0;
        }
        break;
    default: ASSERT(FALSE); break;
    }

    rPhgtFEMA.dIO[IDX_PLS] = rPhgtFEMA.dIO[IDX_MNS] = 2.0;
    rPhgtFEMA.dLS[IDX_PLS] = rPhgtFEMA.dLS[IDX_MNS] = 4.0;
    rPhgtFEMA.dCP[IDX_PLS] = rPhgtFEMA.dCP[IDX_MNS] = 6.0;
}

void CSeisEval::GetInitialHingeYieldSurface(OUT T_PHGT_YLDS& rPhgtYlds)
{
    rPhgtYlds.Initialize();
    rPhgtYlds.nCalcMethod  = D_PHGT_CALC_AUTO;
    rPhgtYlds.bSymmetricIJ = FALSE;
    rPhgtYlds.YieldSurfaceProp.dAxialForce[0] = 0.0005;
    rPhgtYlds.YieldSurfaceProp.dAxialForce[1] = 0.001 ;
    rPhgtYlds.YieldSurfaceProp.dMomentY[0][0] = 0.0005;
    rPhgtYlds.YieldSurfaceProp.dMomentY[1][0] = 0.0005;
    rPhgtYlds.YieldSurfaceProp.dMomentZ[0][0] = 0.0005;
    rPhgtYlds.YieldSurfaceProp.dMomentZ[1][0] = 0.0005;
    rPhgtYlds.YieldSurfaceProp.dMomentY[0][1] = 0.001 ;
    rPhgtYlds.YieldSurfaceProp.dMomentY[1][1] = 0.001 ;
    rPhgtYlds.YieldSurfaceProp.dMomentZ[0][1] = 0.001 ;
    rPhgtYlds.YieldSurfaceProp.dMomentZ[1][1] = 0.001 ;
    rPhgtYlds.YieldSurfacePropSub = rPhgtYlds.YieldSurfaceProp;
}

BOOL CSeisEval::IsBeamElemPosI(T_NODE_K NodeK, T_ELEM_K ElemK)
{
    T_ELEM_D ElemD;
    if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(FALSE); return FALSE; }
    if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) { return FALSE; }

    return ElemD.elnod[0] == NodeK ? TRUE : FALSE;
}

double CSeisEval::GetFactorPnmaxColm(const EN_EVALSTEP enStep, int nHoopType)
{
    double dFactorPnmax = 1.0;
    switch (enStep)
    {    
    case EN_EVALSTEP_2ND:  /// 2단계에서는 Pnmax를 고려하지 않는 것 같음.
        break;
    case EN_EVALSTEP_PRE:
    case EN_EVALSTEP_1ST:
        dFactorPnmax = nHoopType == 1 ? 0.80 : 0.85;
        break;
    default: ASSERT(0);
        break;
    }
    return dFactorPnmax;
}

double CSeisEval::GetFactorPnmaxWall(const EN_EVALSTEP enStep)
{
    double dFactorPnmax = 1.0;
    switch (enStep)
    {    
    case EN_EVALSTEP_2ND:  /// 2단계에서는 Pnmax를 고려하지 않는 것 같음.
        break;
    case EN_EVALSTEP_PRE:
    case EN_EVALSTEP_1ST:
        dFactorPnmax = 0.80; // 벽체는 Hoop Type만 고려.
        break;
    default: ASSERT(0);
        break;
    }
    return dFactorPnmax;
}

bool CSeisEval::GetEval1stConColmForcMap(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, const T_LCOM_K_LIST& aLcomK,
                                         OUT CMapEx<T_LCOM_K, T_LCOM_K, T_STRB_D, T_STRB_D&>& rmStrb)
{
    rmStrb.RemoveAll();

    if ( !m_pDoc->m_pAttrCtrl->ExistElem(ElemK) ) { return false; }

    INT_PTR nLcom = aLcomK.GetSize();
    rmStrb.InitHashTable((UINT)nLcom);

    T_STRB_D StrbDisp;
    T_STRB_D StrbForc;
    for ( INT_PTR i = 0 ; i < nLcom ; ++i )
    {
        const T_LCOM_K LcomK = aLcomK[i];
        if ( !GetEvalStrb1st(pMembCtrl, ElemK, LcomK, StrbDisp, StrbForc)) { continue; }

        rmStrb.SetAt(LcomK, StrbDisp);
    }
    return true;
}

bool CSeisEval::GetEval1stConWallForcMap(CMembCtrl* pMembCtrl, T_ELEM_K ElemK, const T_LCOM_K_LIST& aLcomK,
                                         OUT CMapEx<T_LCOM_K, T_LCOM_K, T_STRW_D, T_STRW_D&>& rmStrw)
{
    rmStrw.RemoveAll();

    if ( !m_pDoc->m_pAttrCtrl->ExistElem(ElemK) ) { return false; }

    UINT unInternalWallID = 0;
    if ( !m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID)) { return false; }

    INT_PTR nLcom = aLcomK.GetSize();
    rmStrw.InitHashTable((UINT)nLcom);

    T_STRW_D StrwDisp;
    T_STRW_D StrwForc;
    for ( INT_PTR i = 0 ; i < nLcom ; ++i )
    {
        const T_LCOM_K LcomK = aLcomK[i];
        if ( !GetEvalStrw1st(pMembCtrl, unInternalWallID, LcomK, StrwDisp, StrwForc)) { continue; }

        rmStrw.SetAt(LcomK, StrwDisp);
    }
    return true;
}

double CSeisEval::CalcBeta(T_ELEM_K InfillK, double dVm, const T_VFRAME_LIST& aVframe)
{
    double adFillVec[3][3] = { 0.0, };
    if (!m_pDoc->calcElemLocalVector(InfillK, adFillVec)) { return 0.0; }
    adFillVec[0][2] = 0.0;  /// x-y 평면에 대한 각도를 계산하려고...
    double adFillVecNor[3][3] = { 0.0, };
    CMathFunc::mathNormalize(adFillVec[0], adFillVecNor[0]);

    double dVframe = 0.0;
    INT_PTR nSize = aVframe.GetSize();
    for ( INT_PTR i=0; i<nSize; ++i )
    {
        const T_VFRAME& CurFram  = aVframe[i];
        const T_ELEM_K& CurElemK = CurFram.ElemK;
        if (!m_pDoc->m_pAttrCtrl->ExistElem(CurElemK)) { continue; }

        double adFramVec[3][3] = { 0.0, };
        if (!m_pDoc->calcElemLocalVector(CurElemK, adFramVec)) { continue; }

        double dAngy = CMathFunc::mathCrossAngle(adFillVecNor[0], adFramVec[1])*CMathFunc::m_trrad; // strut의 local x와 기둥의 local y가 이루는 각도.
        double dAngz = CMathFunc::mathCrossAngle(adFillVecNor[0], adFramVec[2])*CMathFunc::m_trrad; // strut의 local x와 기둥의 local y가 이루는 각도.	

        dVframe += fabs(CurFram.dVny*CMathFunc::mathCos(dAngy)) + fabs(CurFram.dVnz*CMathFunc::mathCos(dAngz));
    }

    return fabs(dVm) < cEvalZero ? 0.0 : fabs(dVframe/dVm);
}

void CSeisEval::CalcForc1stLcom(T_ELEM_K ElemK, CMembCtrl* pMembCtrl)
{
    T_ELEM_D ElemD;
    if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { return; }

    pMembCtrl->Set_LcomForTotl(D_LCOMTYPE_SEISMIC, TRUE, TRUE, TRUE, FALSE);

    if ( ElemD.eltyp == WALL_EL )
    {
        UINT unInternalWallID = 0;
        if (!m_pDoc->m_pPostCtrl->GetInternalWallId(ElemK, unInternalWallID)) { ASSERT(0); }
        pMembCtrl->Set_LcomForElem(ElemD.eltyp, unInternalWallID);
    }
    else
    {
        pMembCtrl->Set_LcomForElem(ElemD.eltyp, ElemK);
    }    
}

double CSeisEval::GetUpdateHingeVnConColm(double dVncal)
{
    if ( fabs(dVncal) < cEvalZero )
    {
        // [GEN-1807] RC 기둥의 업데이트 힌지 시에 계산된 전단강도 = 0이면, 1N로 넣어주세요. 
        // PO 힌지 속성에서 항복강도=0을 허용하지 않기 때문에 솔버에서 문제가 생깁니다. P-Q interaction 검토 시에는 솔버에서 해당값을 계산하기 때문에 1N으로 고려.
        return m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_FACTOR_MM, D_UNITSYS_BASE_FORCE, 1.0);
    }
    return dVncal;
}

double CSeisEval::GetContinuousWallTotalHw(T_ELEM_K ElemK, double dHw)
{
    int nWallID = m_pDoc->m_pPostCtrl->GetUserWallId(ElemK);

    _RCS_WALL_CONTINUOUS_INFO ContWallD;
    if (m_pDoc->m_pPostCtrl->GetWallContInfo(nWallID, ContWallD)) /// 이 데이터는 AnalCtrl에서 만들어지기 때문에 중립단위계입니다.
    {        
        const double dFactLeng = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(m_CodeUnit.nBase_Force, m_CodeUnit.nBase_Length,
            D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);
        return dFactLeng * ContWallD.dTotalHw;
    }
    return dHw;
}

bool CSeisEval::IsConColmConnectedInfill(T_ELEM_K ElemK)
{
    CSeisEvalDataTool Tool;
    return Tool.IsConColmConnectedInfill(ElemK);
}

BOOL CSeisEval::CalcEvalPreConColmAll(T_EVALPRE_COLM_MAP &rmPreConColm)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmPreConColm.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalConColmKeyList(aColmK);
    if (nSizeColm < 1)  return TRUE;

    T_EVALPRE_COLM EvalPre;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_PRE_CONCOLM);
    for (int i=0; i<nSizeColm; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aColmK[i];
        EvalPre.Initialize();
        ASSERT(0);
        //if (!CalcEvalPreConColm(ElemK, EvalPre)) { continue; }

        rmPreConColm.SetAt(ElemK, EvalPre);
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalPreConWallAll(T_EVALPRE_WALL_MAP &rmPreConWall)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmPreConWall.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aWallK;
    int nSizeWall = Tool.GetEvalConWallKeyList(aWallK);
    if (nSizeWall < 1)  return TRUE;

    T_EVALPRE_WALL EvalPre;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_PRE_CONWALL);
    for (int i=0; i<nSizeWall; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aWallK[i];
        EvalPre.Initialize();
        ASSERT(0);
        //if (!CalcEvalPreConWall(ElemK, EvalPre)) { continue; }

        rmPreConWall.SetAt(ElemK, EvalPre);
        ProDlg.Update_ProgressDlg(i+1, nSizeWall);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalLinConBeamAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_BEAM, T_EVALLIN_BEAM&> &rmLinConBeam)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmLinConBeam.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalConBeamKeyList(aBeamK);
    if (nSizeBeam < 1)  return TRUE;

    SE_ELEM_TYPE_K ElemTyepK;
    T_EVALLIN_BEAM EvalLin;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_CONBEAM);
    for (int i=0; i<nSizeBeam; ++i)
    {
        T_ELEM_K ElemK = aBeamK[i];
        ElemTyepK.Key.ElemK = ElemK;
        for (int k=0; k<2; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            UINT unType = (k==0) ? EN_EVAL_GRAV : EN_EVAL_SEIS;
            ElemTyepK.Key.TypeK = unType;
            EvalLin.Initialize();
            if (!CalcSeisEvalLinRsltConBeam(ElemK, unType, EvalLin)) { continue; }

            rmLinConBeam.SetAt(ElemTyepK.KeyMap, EvalLin);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeBeam);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalLinConColmAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_COLM, T_EVALLIN_COLM&> &rmLinConColm)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmLinConColm.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    INT_PTR nSizeColm = Tool.GetEvalConColmAllKeyList(aColmK);        
    if (nSizeColm < 1)  { return TRUE; }

    SE_ELEM_TYPE_K ElemTyepK;
    T_EVALLIN_COLM EvalLin;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_CONCOLM);
    for (INT_PTR i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
        ElemTyepK.Key.ElemK = ElemK;
        for (int k=0; k<2; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            UINT unType = (k==0) ? EN_EVAL_GRAV : EN_EVAL_SEIS;
            ElemTyepK.Key.TypeK = unType;
            EvalLin.Initialize();
            if (!CalcSeisEvalLinRsltConColm(ElemK, unType, EvalLin)) { continue; }

            rmLinConColm.SetAt(ElemTyepK.KeyMap, EvalLin);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalLinConWallAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_WALL, T_EVALLIN_WALL&> &rmLinConWall)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmLinConWall.RemoveAll();

    T_ELEM_K_LIST aWallK;
    CSeisEvalDataTool Tool;
    int nSizeWall = Tool.GetEvalConWallKeyList(aWallK);
    if (nSizeWall < 1)  return TRUE;

    SE_ELEM_TYPE_K ElemTyepK;
    T_EVALLIN_WALL EvalLin;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_CONWALL);
    for (int i=0; i<nSizeWall; ++i)
    {
        T_ELEM_K ElemK = aWallK[i];
        ElemTyepK.Key.ElemK = ElemK;
        for (int k=0; k<2; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            UINT unType = (k==0) ? EN_EVAL_GRAV : EN_EVAL_SEIS;
            ElemTyepK.Key.TypeK = unType;
            EvalLin.Initialize();
            if (!CalcSeisEvalLinRsltConWall(ElemK, unType, EvalLin)) { continue; }

            rmLinConWall.SetAt(ElemTyepK.KeyMap, EvalLin);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeWall);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalLinStlBeamAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBEAM, T_EVALLIN_STLBEAM&> &rmLinStlBeam)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmLinStlBeam.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalStlBeamKeyList(aBeamK);
    if (nSizeBeam < 1)  return TRUE;

    SE_ELEM_TYPE_K ElemTyepK;
    T_EVALLIN_STLBEAM EvalLin;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_STLBEAM);
    for (int i=0; i<nSizeBeam; ++i)
    {
        T_ELEM_K ElemK = aBeamK[i];
        ElemTyepK.Key.ElemK = ElemK;
        for (int k=0; k<2; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            UINT unType = (k==0) ? EN_EVAL_GRAV : EN_EVAL_SEIS;
            ElemTyepK.Key.TypeK = unType;
            EvalLin.Initialize();
            if (!CalcSeisEvalLinRsltStlBeam(ElemK, unType, EvalLin)) { continue; }

            rmLinStlBeam.SetAt(ElemTyepK.KeyMap, EvalLin);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeBeam);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalLinStlColmAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLCOLM, T_EVALLIN_STLCOLM&> &rmLinStlColm)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmLinStlColm.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalStlColmKeyList(aColmK);
    if (nSizeColm < 1)  return TRUE;

    SE_ELEM_TYPE_K ElemTyepK;
    T_EVALLIN_STLCOLM EvalLin;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_STLCOLM);
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
        ElemTyepK.Key.ElemK = ElemK;
        for (int k=0; k<2; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            UINT unType = (k==0) ? EN_EVAL_GRAV : EN_EVAL_SEIS;
            ElemTyepK.Key.TypeK = unType;
            EvalLin.Initialize();
            if (!CalcSeisEvalLinRsltStlColm(ElemK, unType, EvalLin)) { continue; }

            rmLinStlColm.SetAt(ElemTyepK.KeyMap, EvalLin);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalLinStlBracAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_STLBRAC, T_EVALLIN_STLBRAC&> &rmLinStlBrac)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmLinStlBrac.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBracK;
    int nSizeBrac = Tool.GetEvalStlBracKeyList(aBracK);
    if (nSizeBrac < 1)  return TRUE;

    SE_ELEM_TYPE_K ElemTyepK;
    T_EVALLIN_STLBRAC EvalLin;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_STLBRAC);
    for (int i=0; i<nSizeBrac; ++i)
    {
        T_ELEM_K ElemK = aBracK[i];
        ElemTyepK.Key.ElemK = ElemK;
        for (int k=0; k<2; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            UINT unType = (k==0) ? EN_EVAL_GRAV : EN_EVAL_SEIS;
            ElemTyepK.Key.TypeK = unType;
            EvalLin.Initialize();
            if (!CalcSeisEvalLinRsltStlBrac(ElemK, unType, EvalLin)) { continue; }

            rmLinStlBrac.SetAt(ElemTyepK.KeyMap, EvalLin);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeBrac);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalLinInfWallAll(CMapEx<SE_ELEM_TYPE_KEY, SE_ELEM_TYPE_KEY, T_EVALLIN_INFILL, T_EVALLIN_INFILL&> &rmLinInfWall)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmLinInfWall.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aInfWallK;
    int nSizeInfWall = Tool.GetEvalInfillFullTypeKeyList(aInfWallK);
    if (nSizeInfWall < 1)  return TRUE;

    SE_ELEM_TYPE_K ElemTyepK;
    T_EVALLIN_INFILL EvalLin;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_INFWALL);
    for (int i=0; i<nSizeInfWall; ++i)
    {
        T_ELEM_K ElemK = aInfWallK[i];
        ElemTyepK.Key.ElemK = ElemK;
        for (int k=0; k<2; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            UINT unType = (k==0) ? EN_EVAL_GRAV : EN_EVAL_SEIS;
            ElemTyepK.Key.TypeK = unType;
            EvalLin.Initialize();
            if (!CalcSeisEvalLinRsltInfill(ElemK, unType, EvalLin)) { continue; }

            rmLinInfWall.SetAt(ElemTyepK.KeyMap, EvalLin);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeInfWall);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEvalInfWallOutAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL_INFILL_OUT, T_EVAL_INFILL_OUT&> &rmInfWallOut)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rmInfWallOut.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aInfWallK;
    int nSizeInfWall = Tool.GetEvalInfillFullTypeKeyList(aInfWallK);
    if (nSizeInfWall < 1)  return TRUE;

    T_EVAL_INFILL_OUT EvalOut;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_LIN_INFWALL); ///## 이거 정의해야 함!
    for (int i=0; i<nSizeInfWall; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aInfWallK[i];

        if (!CalcSeisEvalInfillOut(ElemK, EvalOut)) { continue; }
        rmInfWallOut.SetAt(ElemK, EvalOut);
        ProDlg.Update_ProgressDlg(i+1, nSizeInfWall);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval1stConBeamAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_BEAM, T_EVAL1ST_BEAM&> &rm1stConBeam)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stConBeam.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalConBeamKeyList(aBeamK);
    if (nSizeBeam < 1)  return TRUE;

    T_EVAL1ST_BEAM Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_CONBEAM);
    for (int i=0; i<nSizeBeam; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aBeamK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltConBeam(ElemK, Eval1st)) { continue; }

        rm1stConBeam.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeBeam);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval1stConColmAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_COLM, T_EVAL1ST_COLM&> &rm1stConColm)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stConColm.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    INT_PTR nSizeColm = Tool.GetEvalConColmAllKeyList(aColmK);
    if (nSizeColm < 1)  { return TRUE; }

    T_EVAL1ST_COLM Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_CONCOLM);
    for (INT_PTR i=0; i<nSizeColm; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aColmK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltConColm(ElemK, Eval1st)) { continue; }

        rm1stConColm.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval1stConWallAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_WALL, T_EVAL1ST_WALL&> &rm1stConWall)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stConWall.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aWallK;
    int nSizeWall = Tool.GetEvalConWallKeyList(aWallK);
    if (nSizeWall < 1)  return TRUE;

    T_EVAL1ST_WALL Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_CONWALL);
    for (int i=0; i<nSizeWall; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aWallK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltConWall(ElemK, Eval1st)) { continue; }

        rm1stConWall.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeWall);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval1stStlBeamAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBEAM, T_EVAL1ST_STLBEAM&> &rm1stStlBeam)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stStlBeam.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalStlBeamKeyList(aBeamK);
    if (nSizeBeam < 1)  return TRUE;

    T_EVAL1ST_STLBEAM Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_STLBEAM);
    for (int i=0; i<nSizeBeam; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aBeamK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltStlBeam(ElemK, Eval1st)) { continue; }

        rm1stStlBeam.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeBeam);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval1stStlColmAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLCOLM, T_EVAL1ST_STLCOLM&> &rm1stStlColm)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stStlColm.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalStlColmKeyList(aColmK);
    if (nSizeColm < 1)  return TRUE;

    T_EVAL1ST_STLCOLM Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_STLCOLM);
    for (int i=0; i<nSizeColm; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aColmK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltStlColm(ElemK, Eval1st)) { continue; }

        rm1stStlColm.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval1stStlPanzAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLPANZ, T_EVAL1ST_STLPANZ&> &rm1stStlPanz)
{
    ASSERT(0); // 아직 지원 안함!

    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stStlPanz.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalStlColmKeyList(aColmK);
    if (nSizeColm < 1)  return FALSE;

    T_EVAL1ST_STLPANZ Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_STLPANZ);
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltStlPanz(ElemK, Eval1st)) { continue; }

        rm1stStlPanz.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }
    ProDlg.Delete_ProgressDlg();

    return rm1stStlPanz.GetCount() > 0 ? TRUE : FALSE;
}

BOOL CSeisEval::CalcEval1stStlBracAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_STLBRAC, T_EVAL1ST_STLBRAC&> &rm1stStlBrac)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stStlBrac.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBracK;
    int nSizeBrac = Tool.GetEvalStlBracKeyList( aBracK);
    if (nSizeBrac < 1)  return TRUE;

    T_EVAL1ST_STLBRAC Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_STLBRAC);
    for (int i=0; i<nSizeBrac; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aBracK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltStlBrac(ElemK, Eval1st)) { continue; }

        rm1stStlBrac.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeBrac);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval1stInfWallAll(CMapEx<T_ELEM_K, T_ELEM_K, T_EVAL1ST_INFILL, T_EVAL1ST_INFILL&> &rm1stInfWall)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm1stInfWall.RemoveAll();

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aInfWallK;
    int nSizeInfWall = Tool.GetEvalInfillFullTypeKeyList(aInfWallK);
    if (nSizeInfWall < 1)  return TRUE;

    T_EVAL1ST_INFILL Eval1st;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_1ST_INFWALL);
    for (int i=0; i<nSizeInfWall; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aInfWallK[i];
        Eval1st.Initialize();
        if (!CalcSeisEval1stRsltInfill(ElemK, Eval1st)) { continue; }

        rm1stInfWall.SetAt(ElemK, Eval1st);
        ProDlg.Update_ProgressDlg(i+1, nSizeInfWall);
    }
    ProDlg.Delete_ProgressDlg();

    return TRUE;
}

BOOL CSeisEval::CalcEval2ndConBeamAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_BEAM_ALL, T_EVAL2ND_BEAM_ALL&> &rm2ndConBeam)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm2ndConBeam.RemoveAll();
    if (!m_pDoc->m_pPostCtrl->PushOverAnalysis())  { ASSERT(0); return TRUE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalConBeamKeyList(aBeamK);

    T_KEY_LIST aPolcK; aPolcK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    int nPolc = static_cast<int>(aPolcK.GetSize());

    if (nSizeBeam < 1 || nPolc < 1)  return TRUE;

    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_2ND_CONBEAM);

    SE_ELEM_LOAD_K ElemLoadK;
    T_EVAL2ND_BEAM_ALL Eval2nd;
    for (int i=0; i<nSizeBeam; ++i)
    {
        T_ELEM_K ElemK = aBeamK[i];
        ElemLoadK.Key.ElemK = ElemK;
        for (int k=0; k<nPolc; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            T_POLC_K PolcK = aPolcK[k];
            ElemLoadK.Key.PolcK = PolcK;
            int nMaxStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
            Eval2nd.Initialize();
            Eval2nd.aSteps.SetSize(nMaxStep);
            for (int nStep=0; nStep<nMaxStep; ++nStep)
            {
                if (!CalcSeisEval2ndRsltConBeam(ElemK, PolcK, nStep, Eval2nd.aSteps[nStep])) { continue; }
            }
            rm2ndConBeam.SetAt(ElemLoadK.KeyMap, Eval2nd);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeBeam);
    }

    ProDlg.Delete_ProgressDlg();
    return TRUE;
}

BOOL CSeisEval::CalcEval2ndConColmAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_COLM_ALL, T_EVAL2ND_COLM_ALL&> &rm2ndConColm)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm2ndConColm.RemoveAll();
    if (!m_pDoc->m_pPostCtrl->PushOverAnalysis())  { ASSERT(0); return TRUE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalConColmKeyList(aColmK);

    T_KEY_LIST aPolcK; aPolcK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    int nPolc = static_cast<int>(aPolcK.GetSize());

    if (nSizeColm < 1 || nPolc < 1)  return TRUE;

    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_2ND_CONCOLM);
	
	std::map<UINT, UINT> mSerialHinge;
	Tool.GetSerialBeamHingeMapPO(mSerialHinge);
		
    SE_ELEM_LOAD_K ElemLoadK;
    T_EVAL2ND_COLM_ALL Eval2nd;
    for (int i=0; i<nSizeColm; ++i)
    {
        T_ELEM_K ElemK = aColmK[i];
			
		UINT nSeriaAgxp = mSerialHinge.find(ElemK) == mSerialHinge.end() ? 0 : mSerialHinge.at(ElemK);

        ElemLoadK.Key.ElemK = ElemK;
        for (int k=0; k<nPolc; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            T_POLC_K PolcK = aPolcK[k];
            ElemLoadK.Key.PolcK = PolcK;
            int nMaxStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
            Eval2nd.Initialize();
            Eval2nd.aSteps.SetSize(nMaxStep);
            for (int nStep=0; nStep<nMaxStep; ++nStep)
            {
                if (!CalcSeisEval2ndRsltConColm(ElemK, PolcK, nStep, nSeriaAgxp, Eval2nd.aSteps[nStep])) { continue; }
            }
            rm2ndConColm.SetAt(ElemLoadK.KeyMap, Eval2nd);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }

    ProDlg.Delete_ProgressDlg();
    return TRUE;
}

BOOL CSeisEval::CalcEval2ndConWallAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_WALL_ALL, T_EVAL2ND_WALL_ALL&> &rm2ndConWall)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm2ndConWall.RemoveAll();
    if (!m_pDoc->m_pPostCtrl->PushOverAnalysis())  { ASSERT(0); return TRUE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aWallK;
    int nSizeWall = Tool.GetEvalConWallKeyList( aWallK);

    T_KEY_LIST aPolcK; aPolcK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    int nPolc = static_cast<int>(aPolcK.GetSize());

    if (nSizeWall < 1 || nPolc < 1)  return TRUE;

    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_2ND_CONWALL);

	std::map<UINT, UINT> mSerialHinge;
	Tool.GetSerialWallHingeMapPO(mSerialHinge);

    SE_ELEM_LOAD_K ElemLoadK;
    T_EVAL2ND_WALL_ALL Eval2nd;
    for (int i=0; i<nSizeWall; ++i)
    {
        T_ELEM_K ElemK = aWallK[i];
		
		UINT nSeriaAgxp = mSerialHinge.find(ElemK) == mSerialHinge.end() ? 0 : mSerialHinge.at(ElemK);

        ElemLoadK.Key.ElemK = ElemK;
        for (int k=0; k<nPolc; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            T_POLC_K PolcK = aPolcK[k];
            ElemLoadK.Key.PolcK = PolcK;
            int nMaxStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
            Eval2nd.Initialize();
            Eval2nd.aSteps.SetSize(nMaxStep);
            for (int nStep=0; nStep<nMaxStep; ++nStep)
            {
                if (!CalcSeisEval2ndRsltConWall(ElemK, PolcK, nStep, nSeriaAgxp, Eval2nd.aSteps[nStep])) { continue; }
            }
            rm2ndConWall.SetAt(ElemLoadK.KeyMap, Eval2nd);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeWall);
    }

    ProDlg.Delete_ProgressDlg();
    return TRUE;
}

BOOL CSeisEval::CalcEval2ndStlBeamAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBEAM_ALL, T_EVAL2ND_STLBEAM_ALL&> &rm2ndStlBeam)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm2ndStlBeam.RemoveAll();
    if (!m_pDoc->m_pPostCtrl->PushOverAnalysis())  { ASSERT(0); return TRUE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBeamK;
    int nSizeBeam = Tool.GetEvalStlBeamKeyList(aBeamK);

    T_KEY_LIST aPolcK; aPolcK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    int nPolc = static_cast<int>(aPolcK.GetSize());

    if (nSizeBeam < 1 || nPolc < 1)  return TRUE;

    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_2ND_STLBEAM);

    SE_ELEM_LOAD_K ElemLoadK;
    T_EVAL2ND_STLBEAM_ALL Eval2nd;
    for (int i=0; i<nSizeBeam; ++i)
    {
        T_ELEM_K ElemK = aBeamK[i];
        ElemLoadK.Key.ElemK = ElemK;
        for (int k=0; k<nPolc; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            T_POLC_K PolcK = aPolcK[k];
            ElemLoadK.Key.PolcK = PolcK;
            int nMaxStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
            Eval2nd.Initialize();
            Eval2nd.aSteps.SetSize(nMaxStep);
            for (int nStep=0; nStep<nMaxStep; ++nStep)
            {
                if (!CalcSeisEval2ndRsltStlBeam(ElemK, PolcK, nStep, Eval2nd.aSteps[nStep])) { continue; }
            }
            rm2ndStlBeam.SetAt(ElemLoadK.KeyMap, Eval2nd);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeBeam);
    }

    ProDlg.Delete_ProgressDlg();
    return TRUE;
}

BOOL CSeisEval::CalcEval2ndStlColmAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLCOLM_ALL, T_EVAL2ND_STLCOLM_ALL&> &rm2ndStlColm)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm2ndStlColm.RemoveAll();
    if (!m_pDoc->m_pPostCtrl->PushOverAnalysis())  { ASSERT(0); return TRUE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aColmK;
    int nSizeColm = Tool.GetEvalStlColmKeyList(aColmK);

    T_KEY_LIST aPolcK; aPolcK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    int nPolc = static_cast<int>(aPolcK.GetSize());

    if (nSizeColm < 1 || nPolc < 1)  return TRUE;

    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_2ND_STLCOLM);

    SE_ELEM_LOAD_K ElemLoadK;
    T_EVAL2ND_STLCOLM_ALL Eval2nd;
    for (int i=0; i<nSizeColm; ++i)
    {
        if(ProDlg.m_bStopExecute)
            return FALSE;
        T_ELEM_K ElemK = aColmK[i];
        ElemLoadK.Key.ElemK = ElemK;
        for (int k=0; k<nPolc; ++k)
        {
            T_POLC_K PolcK = aPolcK[k];
            ElemLoadK.Key.PolcK = PolcK;
            int nMaxStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
            Eval2nd.Initialize();
            Eval2nd.aSteps.SetSize(nMaxStep);
            for (int nStep=0; nStep<nMaxStep; ++nStep)
            {
                if (!CalcSeisEval2ndRsltStlColm(ElemK, PolcK, nStep, Eval2nd.aSteps[nStep])) { continue; }
            }
            rm2ndStlColm.SetAt(ElemLoadK.KeyMap, Eval2nd);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeColm);
    }

    ProDlg.Delete_ProgressDlg();
    return TRUE;
}

BOOL CSeisEval::CalcEval2ndStlBracAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_STLBRAC_ALL, T_EVAL2ND_STLBRAC_ALL&> &rm2ndStlBrac)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm2ndStlBrac.RemoveAll();
    if (!m_pDoc->m_pPostCtrl->PushOverAnalysis())  { ASSERT(0); return TRUE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aBracK;
    int nSizeBrac = Tool.GetEvalStlBracKeyList(aBracK);

    T_KEY_LIST aPolcK; aPolcK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    int nPolc = static_cast<int>(aPolcK.GetSize());

    if (nSizeBrac < 1 || nPolc < 1)  return TRUE;

    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_2ND_STLBRAC);

    SE_ELEM_LOAD_K ElemLoadK;
    T_EVAL2ND_STLBRAC_ALL Eval2nd;
    for (int i=0; i<nSizeBrac; ++i)
    {
        T_ELEM_K ElemK = aBracK[i];
        ElemLoadK.Key.ElemK = ElemK;
        for (int k=0; k<nPolc; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            T_POLC_K PolcK = aPolcK[k];
            ElemLoadK.Key.PolcK = PolcK;
            int nMaxStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
            Eval2nd.Initialize();
            Eval2nd.aSteps.SetSize(nMaxStep);
            for (int nStep=0; nStep<nMaxStep; ++nStep)
            {
                if (!CalcSeisEval2ndRsltStlBrac(ElemK, PolcK, nStep, Eval2nd.aSteps[nStep])) { continue; }
            }
            rm2ndStlBrac.SetAt(ElemLoadK.KeyMap, Eval2nd);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeBrac);
    }

    ProDlg.Delete_ProgressDlg();
    return TRUE;
}

BOOL CSeisEval::CalcEval2ndInfWallAll(CMapEx<SE_ELEM_LOAD_KEY, SE_ELEM_LOAD_KEY, T_EVAL2ND_INFILL_ALL, T_EVAL2ND_INFILL_ALL&> &rm2ndInfWall)
{
    CCurUnitSaver UnitSave(TRUE);
    SetCodeUnit();

    rm2ndInfWall.RemoveAll();
    if (!m_pDoc->m_pPostCtrl->PushOverAnalysis())  { ASSERT(0); return TRUE; }

    CSeisEvalDataTool Tool;
    T_ELEM_K_LIST aInfWallK;
    int nSizeInfWall = Tool.GetEvalInfillFullTypeKeyList(aInfWallK);

    T_KEY_LIST aPolcK; aPolcK.RemoveAll();
    m_pDoc->m_pAttrCtrl->GetPolcKeyList(aPolcK);
    int nPolc = static_cast<int>(aPolcK.GetSize());

    if (nSizeInfWall < 1 || nPolc < 1)  return TRUE;

    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_SEIS_EVAL_2ND_INFWALL);

    SE_ELEM_LOAD_K ElemLoadK;
    T_EVAL2ND_INFILL_ALL Eval2nd;
    for (int i=0; i<nSizeInfWall; ++i)
    {
        T_ELEM_K ElemK = aInfWallK[i];
        ElemLoadK.Key.ElemK = ElemK;
        for (int k=0; k<nPolc; ++k)
        {
            if(ProDlg.m_bStopExecute)
                return FALSE;
            T_POLC_K PolcK = aPolcK[k];
            ElemLoadK.Key.PolcK = PolcK;
            int nMaxStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
            Eval2nd.Initialize();
            Eval2nd.aSteps.SetSize(nMaxStep);
            for (int nStep=0; nStep<nMaxStep; ++nStep)
            {
                if (!CalcSeisEval2ndRsltInfWall(ElemK, PolcK, nStep, Eval2nd.aSteps[nStep])) { continue; }
            }
            rm2ndInfWall.SetAt(ElemLoadK.KeyMap, Eval2nd);
        }
        ProDlg.Update_ProgressDlg(i+1, nSizeInfWall);
    }

    ProDlg.Delete_ProgressDlg();
    return TRUE;
}

BOOL CSeisEval::GetHingeMatlType(T_PHGE_K PhgeK, OUT int& nMaterial)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_PHGE_D PhgeD;
    if(!pDoc->m_pAttrCtrl->GetPhge(PhgeK, PhgeD))	{ ASSERT(0); return FALSE; }
    T_PHGT_D PhgtD;
    if(!pDoc->m_pAttrCtrl->GetPhgt(PhgeD.HingeTypeK, PhgtD))	{ ASSERT(0); return FALSE; }

    nMaterial = PhgtD.nMaterial;
    return TRUE;
}

BOOL CSeisEval::GetBeamColumnHingeMatlType(T_ELEM_K ElemK, OUT int& nMaterial)
{
    const int PHGE_BEAM_TYPE = 1;
    T_PHGE_K PhgeK;
    PhgeK.key.objK = ElemK;
    PhgeK.key.type = PHGE_BEAM_TYPE;
    return GetHingeMatlType(PhgeK, nMaterial);
}

BOOL CSeisEval::GetMatlLimitStrain(const int nMatl, OUT std::unordered_map<T_FIMP_K, double>& mapLimitStrain)
{
	T_DASC_D DascD;
	if (!m_pDoc->m_pAttrCtrl2->GetDasc(DascD)) { ASSERT(0); return FALSE; }

	bool bConc = (nMatl == 0 || nMatl == 1);
	if (bConc)
	{
		for (auto i = 0; i < DascD.aMatlConcData.GetSize(); ++i)
		{
			if (nMatl == 0 && !DascD.aMatlConcData[i].bConfined) continue;
			else if (nMatl == 1 && DascD.aMatlConcData[i].bConfined) continue;
			mapLimitStrain[DascD.aMatlConcData[i].MatlK] = DascD.aMatlConcData[i].dAllowStrain;
		}
	}
	else
	{
		for (auto i = 0; i < DascD.aMatlStlData.GetSize(); ++i)
		{
			if (nMatl == 3 && !DascD.aMatlStlData[i].bSeismic) continue;
			else if (nMatl == 2 && DascD.aMatlStlData[i].bSeismic) continue;
			mapLimitStrain[DascD.aMatlStlData[i].MatlK] = DascD.aMatlStlData[i].dAllowStrain;
		}
	}
	return (mapLimitStrain.size() > 0);
}

BOOL CSeisEval::GetColmAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	T_THIS_D ThisD;
	if (!m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD)) { ASSERT(0); return FALSE; }
	m_pDoc->m_pPostCtrl->SelectThis(ThisK);

	CArray<double, double> aTimeStep;
	int nSizeStep = m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, aTimeStep);
	
	std::unordered_map<T_FIMP_K, double> mapMatlLimitStrain;
	if (!GetMatlLimitStrain(nMatlType, mapMatlLimitStrain)) return FALSE;

	const int MAX_MATL = 6;
	CThisInfo* pThisInfo = m_pDoc->m_pPostCtrl->GetThisInfo();
	for (auto elemK : stElemK)
	{
		int nStartCell = 0, nNumPosition = 0, nNumCell = 0, nNumSectionNo = 0;
		if (!pThisInfo->GetFiberCellInfo(elemK, nStartCell, nNumPosition, nNumCell, FALSE)) continue;
		if (!pThisInfo->GetFiberCellSectInfo(elemK, nNumSectionNo)) continue;
		
		int nNumSection = 0;
		T_FIBR_K fibrK = 0;
		auto lamdaGetSectNum = [&](const T_ELEM_K ElemK, int& nSectNum, T_FIBR_K& FibrK)
		{
			T_IEHG_D IehgD;
			T_IEHP_D IehpD;
			if (!m_pDoc->m_pAttrCtrl->GetIehgByElemK(ElemK, IehgD)) return false;
			if (!m_pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD)) return false;
			if (IehpD.nHingeType == 1 && IehpD.nDefinition == 1) {} // exist Fiber cell 
			else return false;

			if (IehpD.nFiberAutoUser == 0) IehpD.FibrKey = IehgD.FiberKey;
			if (IehpD.FibrKey == 0) return false;

			FibrK = IehpD.FibrKey;
			nSectNum = IehpD.nSectionNum[0];
			return true;
		};
		if (!lamdaGetSectNum(elemK, nNumSection, fibrK)) continue;
		
		T_FIBR_D DataFibr;
		if (!m_pDoc->m_pAttrCtrl->GetFibr(fibrK, DataFibr)) continue;

		std::map<double, T_STRN_D> mapRatio;
		if(iTimeIdx >= 0)// STEP_NORMAL
		{
			for (auto i = 0; i < nNumSection; i++) /// BY SECTION POSITION. 
			{
				std::map<double, T_STRN_D> mapSectRatio;
				for (auto j = 0; j < DataFibr.aFibrBase.GetSize(); ++j)
				{
					const T_FIBR_BASE& CurCell = DataFibr.aFibrBase[j];
					T_FIMP_K FimpK = DataFibr.FimpKey[CurCell.FibrMatlID];
					T_FIMP_D FimpD;
					if (!m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD)) continue;
					auto itr = mapMatlLimitStrain.find(FimpK);
					if (itr == mapMatlLimitStrain.end()) continue;				
					if (itr->second == 0.0) continue;

					T_FBCH_D FbchD;
					const int nFiberPostSerial = nStartCell + i * nNumCell + j;
					m_pDoc->m_pPostCtrl->GetFbchHistory(iTimeIdx, nFiberPostSerial, FbchD);
					// 0: Only Compressive Strain for Conc.
					// 1: Only Tensile Strain for Steel
					const double dCurStrain = (FimpD.nMaterialType == 0) ? min(FbchD.dStrain, 0.0) : max(FbchD.dStrain, 0.0);				
					double dRatio = dgn::SafeDivAbs(dCurStrain, itr->second);
					mapSectRatio[dRatio] = T_STRN_D(dCurStrain, itr->second);
				}
				if (mapSectRatio.size() > 0) mapRatio.insert(*mapSectRatio.rbegin());
			}			
		}
		else // ALL/MAX/MIN
		{
			for (auto i = 0; i < nNumSection; i++) /// BY SECTION POSITION. 
			{
				T_BFBM_D BfbmD;
				if (!m_pDoc->m_pPostCtrl->GetBfbmHistory(nNumSectionNo + i, BfbmD)) continue;

				std::map<double, T_STRN_D> mapSectRatio;
				for (int j = 0; j < MAX_MATL; ++j)
				{
					const T_BFBM_BASE_D& curBfbmD = BfbmD.aData[j];
					if (curBfbmD.nFlag == 0) break;

					const int curCellIdx = curBfbmD.nCellIdx - (nStartCell + i * nNumCell);
					const T_FIBR_BASE& CurCell = DataFibr.aFibrBase[curCellIdx];
					T_FIMP_K FimpK = DataFibr.FimpKey[CurCell.FibrMatlID];
                    if (!m_pDoc->m_pAttrCtrl->ExistFimp(FimpK)) continue;

					auto itr = mapMatlLimitStrain.find(FimpK);
					if (itr == mapMatlLimitStrain.end()) continue;
					if (itr->second == 0.0) continue;

					double dRatio = dgn::SafeDivAbs((double)curBfbmD.dStrain, itr->second);
					mapSectRatio[dRatio] = T_STRN_D(curBfbmD.dStrain, itr->second);
				}
				if (mapSectRatio.size() > 0) mapRatio.insert(*mapSectRatio.rbegin());
			}
		}
		
		std::pair<double, T_STRN_D> pa; 
		pa.first = 0;
		if (mapRatio.size() > 0)
		{
			if (nStepType < 0 || nStepType == STEP_ALL)
			{
				for (auto& val : mapRatio)
				{
					if (abs(pa.first) < abs(val.first))
						pa = val;
				}
			}
			else if (nStepType == STEP_MIN) pa = *mapRatio.begin();
			else pa = *mapRatio.rbegin();
			mapValue[elemK].first = (isRatio) ? pa.first : pa.second.dStrain;
			mapValue[elemK].second = pa.second.dLimitStrain;
		}
	}
	return TRUE;
}

BOOL CSeisEval::GetWallAxialStrainListTHFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_THIS_K ThisK, int iTimeIdx, int nStepType, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	T_THIS_D ThisD;
	if (!m_pDoc->m_pAttrCtrl->GetThis(ThisK, ThisD)) { ASSERT(0); return FALSE; }
	m_pDoc->m_pPostCtrl->SelectThis(ThisK);

	CArray<double, double> aTimeStep;
	int nSizeStep = m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, aTimeStep);

	std::unordered_map<T_FIMP_K, double> mapMatlLimitStrain;
	if (!GetMatlLimitStrain(nMatlType, mapMatlLimitStrain)) return FALSE;
	
	const int MAX_MATL = 6;
	CThisInfo* pThisInfo = m_pDoc->m_pPostCtrl->GetThisInfo();
	for (auto elemK : stElemK)
	{
		int nStartCell = 0, nNumPosition = 0, nNumCell = 0, nNumSectionNo = 0;
		if (!pThisInfo->GetFiberCellInfo(elemK, nStartCell, nNumPosition, nNumCell, FALSE)) continue;
		if (!pThisInfo->GetFiberCellSectInfo(elemK, nNumSectionNo)) continue;

		int nNumSection = 0;
		T_FIBR_K fibrK = 0;
		auto lamdaGetSectNum = [&](const T_ELEM_K ElemK, int& nSectNum, T_FIBR_K& FibrK)
		{
			T_IEHG_D IehgD;
			T_IEHP_D IehpD;
			if (!m_pDoc->m_pAttrCtrl->GetIehgByElemK(ElemK, IehgD)) return false;
			if (!m_pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD)) return false;
			if (IehpD.nHingeType == 4 && IehpD.nDefinition == 1) {} // exist Fiber cell 
			else return false;

			if (IehpD.nFiberAutoUser == 0) IehpD.FibrKey = IehgD.FiberKey;
			if (IehpD.FibrKey == 0) return false;

			FibrK = IehpD.FibrKey;
			nSectNum = IehpD.nSectionNum[0];
			return true;
		};
		if (!lamdaGetSectNum(elemK, nNumSection, fibrK)) continue;

		T_FIBW_D DataFibw;
		if (!m_pDoc->m_pAttrCtrl->GetFibw(fibrK, DataFibw)) continue;

		std::map<double, T_STRN_D> mapRatio;
		if (iTimeIdx >= 0)// STEP_NORMAL
		{
			for (auto i = 0; i < nNumSection; i++) /// BY SECTION POSITION. 
			{
				std::map<double, T_STRN_D> mapSectRatio;
				for (auto j = 0; j < DataFibw.aFibwBase.GetSize(); ++j)
				{
					const T_FIBR_BASE& CurCell = DataFibw.aFibwBase[j];
					T_FIMP_K FimpK = DataFibw.FimpKey[CurCell.FibrMatlID];
					T_FIMP_D FimpD;
					if (!m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD)) continue;
					auto itr = mapMatlLimitStrain.find(FimpK);
					if (itr == mapMatlLimitStrain.end()) continue;
					if (itr->second == 0.0) continue;

					T_FBCH_D FbchD;
					const int nFiberPostSerial = nStartCell + i * nNumCell + j;
					m_pDoc->m_pPostCtrl->GetFbchHistory(iTimeIdx, nFiberPostSerial, FbchD);
					// 0: Only Compressive Strain for Conc.
					// 1: Only Tensile Strain for Steel
					const double dCurStrain = (FimpD.nMaterialType == 0) ? min(FbchD.dStrain, 0.0) : max(FbchD.dStrain, 0.0);
					double dRatio = dgn::SafeDivAbs(dCurStrain, itr->second);
					mapSectRatio[dRatio] = T_STRN_D(dCurStrain, itr->second);
				}
				if (mapSectRatio.size() > 0) mapRatio.insert(*mapSectRatio.rbegin());
			}
		}
		else // ALL/MAX/MIN
		{
			for (auto i = 0; i < nNumSection; i++) /// BY SECTION POSITION. 
			{
				T_BFBM_D WfbmD;
				if (!m_pDoc->m_pPostCtrl->GetWfbmHistory(nNumSectionNo + i, WfbmD)) continue;
			
				std::map<double, T_STRN_D> mapSectRatio;
				for (int j = 0; j < MAX_MATL; ++j)
				{
					const T_BFBM_BASE_D& curWfbmD = WfbmD.aData[j];
					if (curWfbmD.nFlag == 0) break;

					const int curCellIdx = curWfbmD.nCellIdx - (nStartCell + i * nNumCell);
					const T_FIBR_BASE& CurCell = DataFibw.aFibwBase[curCellIdx];
					T_FIMP_K FimpK = DataFibw.FimpKey[CurCell.FibrMatlID];
					if (!m_pDoc->m_pAttrCtrl->ExistFimp(FimpK)) continue;

					auto itr = mapMatlLimitStrain.find(FimpK);
					if (itr == mapMatlLimitStrain.end()) continue;
				
					double dRatio = dgn::SafeDivAbs((double)curWfbmD.dStrain, itr->second);
					mapSectRatio[dRatio] = T_STRN_D(curWfbmD.dStrain, itr->second);
				}
				if (mapSectRatio.size() > 0) mapRatio.insert(*mapSectRatio.rbegin());
			}
		}
		
		std::pair<double, T_STRN_D> pa;
		pa.first = 0;
		if(mapRatio.size() > 0)
		{
			if (nStepType < 0 || nStepType == STEP_ALL)
			{
				for (auto& itr : mapRatio)
				{
					if (abs(pa.first) < abs(itr.first))
						pa = itr;
				}
			}
			else if (nStepType == STEP_MIN) pa = *mapRatio.begin();
			else pa = *mapRatio.rbegin();

			mapValue[elemK].first = (isRatio) ? pa.first : pa.second.dStrain;
			mapValue[elemK].second = pa.second.dLimitStrain;
		}
	}
	return TRUE;
}

BOOL CSeisEval::GetColmAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	T_POLC_D PolcD;
	if (!m_pDoc->m_pAttrCtrl->GetPolc(PolcK, PolcD)) { ASSERT(0); return FALSE; }
	m_pDoc->m_pPostCtrl->SelectPolc(PolcK);

	int nSizeStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);
	
	std::unordered_map<T_FIMP_K, double> mapMatlLimitStrain;
	if (!GetMatlLimitStrain(nMatlType, mapMatlLimitStrain)) return FALSE;
	
	CPoverInfo* pPoverInfo = m_pDoc->m_pPostCtrl->GetPoverInfo();
	if (pPoverInfo == nullptr) return FALSE;

	for (auto elemK : stElemK)
	{
		int nStartCell = 0, nNumPosition = 0, nNumCell = 0, nNumSectionNo = 0;
		if (!pPoverInfo->GetFiberCellInfo_PO(elemK, nStartCell, nNumPosition, nNumCell)) continue;
		if (!pPoverInfo->GetFiberCellSectInfo_PO(elemK, nNumSectionNo)) continue;

		int nNumSection = 0;
		T_FIBR_K fibrK = 0;
		auto lamdaGetSectNum = [&](const T_ELEM_K ElemK, int& nSectNum, T_FIBR_K& FibrK)
		{
			//단면 갯수 저장
			T_PHGE_K PhgeK;
			T_PHGE_D PhgeD;
			T_PHGT_D PhgtD;
			if (!m_pDoc->m_pAttrCtrl->GetPhgeObj(D_PHGE_BEAMCOLUMN, ElemK, PhgeK, PhgeD)) return false;
			if (!m_pDoc->m_pAttrCtrl->GetPhgt(PhgeD.HingeTypeK, PhgtD)) return false;
			if (PhgtD.nHingeType == 1 && PhgtD.nDefinition == 2) {} // exist Fiber cell 
			else return false;

			if ( PhgtD.nFiberAutoUser == 0 ) PhgtD.FibrKey = PhgeD.FiberKey;
			if (PhgtD.FibrKey == 0) return false;

			FibrK = PhgtD.FibrKey;
			nSectNum = PhgtD.nLoc[0];
			return true;
		};
		if (!lamdaGetSectNum(elemK, nNumSection, fibrK)) continue;

		T_FIBR_D DataFibr;
		if (!m_pDoc->m_pAttrCtrl->GetFibr(fibrK, DataFibr)) continue;

		std::map<double, T_STRN_D> mapRatio;
		for (auto i = 0; i < nNumSection; i++) /// BY SECTION POSITION. 
		{
			std::map<double, T_STRN_D> mapSectRatio;
			for (auto j = 0; j < DataFibr.aFibrBase.GetSize(); ++j)
			{
				const T_FIBR_BASE& CurCell = DataFibr.aFibrBase[j];
				T_FIMP_K FimpK = DataFibr.FimpKey[CurCell.FibrMatlID];
				T_FIMP_D FimpD;
				if (!m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD)) continue;
				if (mapMatlLimitStrain.find(FimpK) == mapMatlLimitStrain.end()) continue;
				if (mapMatlLimitStrain[FimpK] == 0.0) continue;

				T_FBCH_D FbchD;
				const int nFiberPostSerial = nStartCell + i * nNumCell + j;
				m_pDoc->m_pPostCtrl->GetFbcpHistory(iStep-1, nFiberPostSerial, FbchD);
				// 0: Only Compressive Strain for Conc.
				// 1: Only Tensile Strain for Steel
				const double dCurStrain = (FimpD.nMaterialType == 0) ? min(FbchD.dStrain, 0.0) : max(FbchD.dStrain, 0.0);
				double dRatio = dgn::SafeDivAbs(dCurStrain, mapMatlLimitStrain[FimpK]);
				mapSectRatio[dRatio] = T_STRN_D(dCurStrain, mapMatlLimitStrain[FimpK]);				
			}
			if (mapSectRatio.size() > 0) mapRatio.insert(*mapSectRatio.rbegin());
		}
		std::pair<double, T_STRN_D> pa;
		pa.first = 0;
		if (mapRatio.size() > 0)
		{
			for (auto& itr : mapRatio)
			{
				if (abs(pa.first) < abs(itr.first))
					pa = itr;
			}
			mapValue[elemK].first = (isRatio) ? pa.first : pa.second.dStrain;
			mapValue[elemK].second = pa.second.dLimitStrain;
		}
	}
	return TRUE;
}

BOOL CSeisEval::GetWallAxialStrainListPOFiber(const std::set<T_ELEM_K>& stElemK, int nMatlType, T_POLC_K PolcK, int iStep, BOOL isRatio, OUT std::unordered_map<T_ELEM_K, std::pair<double, double>>& mapValue)
{
	T_POLC_D PolcD;
	if (!m_pDoc->m_pAttrCtrl->GetPolc(PolcK, PolcD)) { ASSERT(0); return FALSE; }
	m_pDoc->m_pPostCtrl->SelectPolc(PolcK);

	int nSizeStep = m_pDoc->m_pPostCtrl->GetCaseStep_PO(PolcK);

	std::unordered_map<T_FIMP_K, double> mapMatlLimitStrain;
	if (!GetMatlLimitStrain(nMatlType, mapMatlLimitStrain)) return FALSE;

	CPoverInfo* pPoverInfo = m_pDoc->m_pPostCtrl->GetPoverInfo();
	if (pPoverInfo == nullptr) return FALSE;

	for (auto elemK : stElemK)
	{
		int nStartCell = 0, nNumPosition = 0, nNumCell = 0, nNumSectionNo = 0;
		if (!pPoverInfo->GetFiberCellInfo_PO(elemK, nStartCell, nNumPosition, nNumCell)) continue;
		if (!pPoverInfo->GetFiberCellSectInfo_PO(elemK, nNumSectionNo)) continue;

		int nNumSection = 0;
		T_FIBR_K fibrK = 0;
		auto lamdaGetSectNum = [&](const T_ELEM_K ElemK, int& nSectNum, T_FIBR_K& FibrK)
		{
			//단면 갯수 저장
			T_PHGE_K PhgeK;
			T_PHGE_D PhgeD;
			T_PHGT_D PhgtD;
			if (!m_pDoc->m_pAttrCtrl->GetPhgeObj(D_PHGE_WALL, ElemK, PhgeK, PhgeD)) return false;
			if (!m_pDoc->m_pAttrCtrl->GetPhgt(PhgeD.HingeTypeK, PhgtD)) return false;
			if (PhgtD.nHingeType == 1 && PhgtD.nDefinition == 2) {} // exist Fiber cell 
			else return false;

			if ( PhgtD.nFiberAutoUser == 0 ) PhgtD.FibrKey = PhgeD.FiberKey;
			if (PhgtD.FibrKey == 0) return false;

			FibrK = PhgtD.FibrKey;
			nSectNum = PhgtD.nLoc[0];
			return true;
		};
		if (!lamdaGetSectNum(elemK, nNumSection, fibrK)) continue;

		T_FIBW_D DataFibw;
		if (!m_pDoc->m_pAttrCtrl->GetFibw(fibrK, DataFibw)) continue;

		std::map<double, T_STRN_D> mapRatio;
		for (auto i = 0; i < nNumSection; i++) /// BY SECTION POSITION. 
		{
			std::map<double, T_STRN_D> mapSectRatio;
			for (auto j = 0; j < DataFibw.aFibwBase.GetSize(); ++j)
			{
				const T_FIBR_BASE& CurCell = DataFibw.aFibwBase[j];
				T_FIMP_K FimpK = DataFibw.FimpKey[CurCell.FibrMatlID];
				T_FIMP_D FimpD;
				if (!m_pDoc->m_pAttrCtrl->GetFimp(FimpK, FimpD)) continue;
				if (mapMatlLimitStrain.find(FimpK) == mapMatlLimitStrain.end()) continue;
				if (mapMatlLimitStrain[FimpK] == 0.0) continue;

				T_FBCH_D FbchD;
				const int nFiberPostSerial = nStartCell + i * nNumCell + j;
				m_pDoc->m_pPostCtrl->GetFwcpHistory(iStep-1, nFiberPostSerial, FbchD);
				// 0: Only Compressive Strain for Conc.
				// 1: Only Tensile Strain for Steel
				const double dCurStrain = (FimpD.nMaterialType == 0) ? min(FbchD.dStrain, 0.0) : max(FbchD.dStrain, 0.0);
				double dRatio = dgn::SafeDivAbs(dCurStrain, mapMatlLimitStrain[FimpK]);
				mapSectRatio[dRatio] = T_STRN_D(dCurStrain, mapMatlLimitStrain[FimpK]);								
			}
			if (mapSectRatio.size() > 0) mapRatio.insert(*mapSectRatio.rbegin());
		}
		std::pair<double, T_STRN_D> pa;
		pa.first = 0;
		if (mapRatio.size() > 0)
		{
			for (auto& itr : mapRatio)
			{
				if (abs(pa.first) < abs(itr.first))
					pa = itr;
			}
			mapValue[elemK].first = (isRatio) ? pa.first : pa.second.dStrain;
			mapValue[elemK].second = pa.second.dLimitStrain;
		}
	}
	return TRUE;
}