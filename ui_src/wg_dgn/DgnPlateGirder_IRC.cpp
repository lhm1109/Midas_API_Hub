// DgnPlateGirder_IRC.cpp: implementation of the CDgnPlateGirder_IRC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnPlateGirder_IRC.h"

#include "CRCForceCtrl.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\PlateGirderDesign.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\MembCtrl.h"
#include "..\wg_db\PathUtil.h"

#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\DesignLoadRating.h"
#include "..\wg_db\SelfWgt.h"

#include "limits.h"
#include "math.h"

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include "..\MIT_Lib\rptostream.h"	// for rptwofstream
#endif


// CDgnPlateGirder_IRC
//////////////////////////////////////////////////////////////////////

CDgnPlateGirder_IRC::CDgnPlateGirder_IRC()
{
    //m_pDoc = CDBDoc::GetDocPoint();
    //ASSERT(m_pDoc);
    m_dZero=1.0E-07;    

    CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
    m_bTestMode = csDesignOutput==_ULS(yes) ? TRUE : FALSE;

    m_ClassSectMap.InitHashTable(10001);
    m_LcomPgbrMap.InitHashTable(10001);
    m_LcomPglsMap.InitHashTable(10001);
    m_EffeSectMap.InitHashTable(10001);  
    //
    m_dPi = 4.0*atan(1.0);

    InitPartialFactor();

    m_pMembCtrl = CMembCtrl::GetMembCtrl();
    m_bPrintMode = FALSE;

    m_pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();

    m_CpgdD.Initialize();  
    m_pDoc->m_pAttrCtrl2->GetDgnCpgd(m_CpgdD);

    m_pDgnSectUtil = new CDgnSectUtil();
    m_dBeffFactor=0.0;
    m_dMfRd=m_dHighShearCond1=m_dHighDhearCond2=m_dBetaFactHighShear=0.0;
}

CDgnPlateGirder_IRC::~CDgnPlateGirder_IRC()
{
    if(m_pDgnSectUtil != nullptr){delete m_pDgnSectUtil;}
}

///////////////////////////////////////////
// For Connection to CRCDataCtrl.
void CDgnPlateGirder_IRC::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
    CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
    m_pDataCtrl->Set_UnitIndex(CodeUnit);	
    //m_pDataCtrl->Get_CurrUnitIndex(CodeUnit);	
    m_CodeUnit = CodeUnit;

    Make_PlateGirderDgnData();
}

void CDgnPlateGirder_IRC::InitPartialFactor()
{
    T_CPGD_D CpgdD;
    CpgdD.Initialize();  
    m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

    m_dGamma_C    = CpgdD.dGamma_C; 
    m_dGamma_S    = CpgdD.dGamma_S;
    m_dGamma_M0   = CpgdD.dGamma_M0_IRC;
    m_dGamma_M1   = CpgdD.dGamma_M1;
    m_dGamma_V    = CpgdD.dGamma_V;
    m_dGamma_Ff   = CpgdD.dGamma_Ff;
    m_dGamma_Mf   = CpgdD.dGamma_Mf;
    m_dGamma_Mfs  = CpgdD.dGamma_Mfs;
    m_dGamma_M_ser = CpgdD.dGamma_M_ser;

    m_dAlpha = 0.608;
    m_iType =0;
    m_PgbrBase.Initialize();

    bOuter=bissimplysup = FALSE;//if dfalse then inner element
    dIRCL = m_dTotalLength = 0.0;

}

BOOL CDgnPlateGirder_IRC::Make_ClassSection(ElemPairK ElemK, T_PGBS_D& PgbsD, T_PGBR_LCOM& PgbrLcomD)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc);

    //+++++++++++++++++++++++++++++++++++++++
    // CurrUnit -> CodeUnit.
    //  m_pDataCtrl->Get_BendingResistanceForPlateGirderCurrToCode(PgbrLcomD);
    //  m_pDataCtrl->Get_BeamStressResistanceForPlateGirderCurrToCode(PgbsD);
    //+++++++++++++++++++++++++++++++++++++++
    BOOL bCalcOK=TRUE;
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;  
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
        return FALSE;
    }
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
        return FALSE;
    }
    CPG_RBAR_ELEM RbarElem;
    if(Get_CpgRbarElem(ElemK, RbarElem)) bRebar=TRUE;

    CPG_SCON_ELEM SconElem;
    if(Get_CpgSconElem(ElemK, SconElem)) bShearConnector=TRUE;


    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PgbrLcomD.PgbrBase[i].LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = i;
        //
        T_CLAS_BASE ClassD;
        ClassD.Initialize();

        double aForce[6]={0.0,0.0,0.0,0.0,0.0,0.0};
        double dStress[4];
        aForce[0] = PgbrLcomD.PgbrBase[i].dFx;
        aForce[4] = PgbrLcomD.PgbrBase[i].dMy;
        aForce[5] = PgbrLcomD.PgbrBase[i].dMz;
        double dMa_Ed = PgbrLcomD.PgbrBase[i].dMa_Ed;
        //aForce[3] = PgbrLcomD.PgbrBase[i].dMa_Ed;
        //
        for(int j=0; j<4; j++)
            dStress[j] = PgbsD.PgbsBase[i].ChkGirder.dStress[j];

        // Longitudinal Stiffener  

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);

        if(PgbrLcomD.PgbrBase[i].bChk==FALSE) continue;

        T_GLINE_DGN_D GLineDgn = SectElem.GLineDgn[i];
        T_SECT_SECTBASE_D SectBaseD;
        T_GENL_DATA GLineData;
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);

        //    T_DLINE_CALC_D DLineCalcD;
        //    if(!Get_ClassifyClassSection(bGirder, bPosiI, aForce, dStress, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, DLineCalcD, ClassD)) continue;    
        //    //
        //    m_ClassSectMap.SetAt(ComfKey,ClassD);
    }
    //+++++++++++++++++++++++++++++++++++++++
    // CodeUnit -> CurrUnit.
    //  m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
    //  m_pDataCtrl->Get_BeamStressResistanceForPlateGirderCodeToCurr(PgbsD);
    //+++++++++++++++++++++++++++++++++++++++

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Make_ClassInD(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC& ClassInD)
{
    switch (m_pDgnSectUtil->Get_RealSectType(&rSectD.SectD))
    {
    case D_SECT_TYPE_COMPO_I: 	{Make_ClassInD_I(bPosiI, aGirderForce, aCptForce, rSectD, ClassInD);m_iType=2; break;}
    case D_SECT_TYPE_COMPO_B:	{Make_ClassInD_Box(bPosiI, aGirderForce, aCptForce, rSectD, ClassInD);m_iType=4; break;}
    case D_SECT_TYPE_COMPO_TUB:	{Make_ClassInD_Tub(bPosiI, aGirderForce, aCptForce, rSectD, ClassInD);m_iType=4; break;}
    default:	{ASSERT(0);	 return FALSE;}		
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Make_ClassInD_I(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC& ClassInD)
{
    // Get Force
    double dFx_a = aGirderForce[0],		dMy_a = aGirderForce[4],	dMz_a = aGirderForce[5];
    double dFx_c = aCptForce[0],		dMy_c = aCptForce[4],		dMz_c = aCptForce[5];

    // Get Stiffness
    T_SECT_STIFFNESS GirderStiff, PosiStiffD, NegaStiffD;
    Get_GirderStiffnessofCompo(bPosiI, rSectD.SectD, GirderStiff);
    GetNonComSectStiffD(rSectD, bPosiI, NegaStiffD);
    GetRebarSectStiffD(rSectD, bPosiI, PosiStiffD);
    const T_SECT_STIFFNESS& SectStiffD = dMy_c > 0.0 ? PosiStiffD : NegaStiffD;

    double dA_a	= GirderStiff.Area, dIyy_a = GirderStiff.Ryy,	dIzz_a =  GirderStiff.Rzz;
    double dA_c	= SectStiffD.Area,	dIyy_c = SectStiffD.Ryy,	dIzz_c = SectStiffD.Rzz;

    m_dRyy = dIyy_a;
    m_dRzz = dIzz_a;
    //
    ClassInD.Initialize();

    // 0 : Outer, 1 : Inner
    double dy_a1 = 0.0, dz_a1 = 0.0, dy_a2 = 0.0, dz_a2 = 0.0; // Girder
    double dy_c1 = 0.0, dz_c1 = 0.0, dy_c2 = 0.0, dz_c2 = 0.0; // Composite
    double dc = 0.0, dt = 0.0;
    double dStress_Out = 0.0, dStress_In = 0.0;

    // Make Top Flange
    // Top Left
    ClassInD.aTopFlangeD.SetSize(2);
    double dbft_l	= m_pDgnSectUtil->Get_Size_bft_l(&rSectD.SectD, bPosiI);
    double dtw		= m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dtft		= m_pDgnSectUtil->Get_Size_tft(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx1; dy_a2 = GirderStiff.dx1 + dbft_l - 0.5*dtw;
    dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
    dy_c1 = SectStiffD.dx1; dy_c2 = SectStiffD.dx1 + dbft_l - 0.5*dtw;
    dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[0].nType		= 0;
    ClassInD.aTopFlangeD[0].dc			= dc;
    ClassInD.aTopFlangeD[0].dt			= dt;
    ClassInD.aTopFlangeD[0].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[0].dStressInter= dStress_In;

    // Top Right
    double dbft_r	= m_pDgnSectUtil->Get_Size_bft_r(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx2; dy_a2 = GirderStiff.dx2 - dbft_r + 0.5*dtw;
    dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
    dy_c1 = SectStiffD.dx2; dy_c2 = SectStiffD.dx2 - dbft_r + 0.5*dtw;
    dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[1].nType		= 0;
    ClassInD.aTopFlangeD[1].dc			= dc;
    ClassInD.aTopFlangeD[1].dt			= dt;
    ClassInD.aTopFlangeD[1].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[1].dStressInter= dStress_In;


    // Make Bottom Flange
    // Bottom Right
    ClassInD.aBotFlangeD.SetSize(2);
    double dbfb_r	= m_pDgnSectUtil->Get_Size_bfb_r(&rSectD.SectD, bPosiI);
    double dtfb		= m_pDgnSectUtil->Get_Size_tfb(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx3; dy_a2 = GirderStiff.dx3 - dbfb_r + 0.5*dtw;
    dz_a1 = GirderStiff.dy3; dz_a2 = GirderStiff.dy3;
    dy_c1 = SectStiffD.dx3; dy_c2 = SectStiffD.dx3 - dbfb_r + 0.5*dtw;
    dz_c1 = SectStiffD.dy3; dz_c2 = SectStiffD.dy3;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[0].nType		= 0;
    ClassInD.aBotFlangeD[0].dc			= dc;
    ClassInD.aBotFlangeD[0].dt			= dt;
    ClassInD.aBotFlangeD[0].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[0].dStressInter= dStress_In;

    // Bottom Left
    double dbfb_l	= m_pDgnSectUtil->Get_Size_bfb_l(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx4; dy_a2 = GirderStiff.dx4 + dbfb_l - 0.5*dtw;
    dz_a1 = GirderStiff.dy4; dz_a2 = GirderStiff.dy4;
    dy_c1 = SectStiffD.dx4; dy_c2 = SectStiffD.dx4 + dbfb_l - 0.5*dtw;
    dz_c1 = SectStiffD.dy4; dz_c2 = SectStiffD.dy4;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[1].nType		= 0;
    ClassInD.aBotFlangeD[1].dc			= dc;
    ClassInD.aBotFlangeD[1].dt			= dt;
    ClassInD.aBotFlangeD[1].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[1].dStressInter= dStress_In;

    // Make Web
    ClassInD.aWebD.SetSize(1);
    dy_a1 = GirderStiff.dx1 + dbft_l;	dy_a2 = GirderStiff.dx4 + dbfb_l;
    dz_a1 = GirderStiff.dy1 - dtft;		dz_a2 = GirderStiff.dy4 + dtfb;
    dy_c1 = SectStiffD.dx1 + dbft_l;	dy_c2 = SectStiffD.dx4 + dbfb_l;
    dz_c1 = SectStiffD.dy1 - dtft;		dz_c2 = SectStiffD.dy4 + dtfb;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dz_a1 - dz_a2);
    dt = dtw;
    ClassInD.aWebD[0].nType			= 1;
    ClassInD.aWebD[0].dc			= dc;
    ClassInD.aWebD[0].dt			= dt;
    ClassInD.aWebD[0].dStressOuter	= dStress_Out;
    ClassInD.aWebD[0].dStressInter	= dStress_In;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Make_ClassInD_Box(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC& ClassInD)
{
    ClassInD.Initialize();

    // Get Force
    double dFx_a = aGirderForce[0],		dMy_a = aGirderForce[4],	dMz_a = aGirderForce[5];
    double dFx_c = aCptForce[0],		dMy_c = aCptForce[4],		dMz_c = aCptForce[5];

    // Get Stiffness
    T_SECT_STIFFNESS GirderStiff, PosiStiffD, NegaStiffD;
    Get_GirderStiffnessofCompo(bPosiI, rSectD.SectD, GirderStiff);
    GetNonComSectStiffD(rSectD, bPosiI, NegaStiffD);
    GetRebarSectStiffD(rSectD, bPosiI, PosiStiffD);
    const T_SECT_STIFFNESS& SectStiffD = dMy_c > 0.0 ? PosiStiffD : NegaStiffD;

    double dA_a	= GirderStiff.Area, dIyy_a = GirderStiff.Ryy,	dIzz_a =  GirderStiff.Rzz;
    double dA_c	= SectStiffD.Area,	dIyy_c = SectStiffD.Ryy,	dIzz_c = SectStiffD.Rzz;
    m_dRyy = dIyy_a;
    m_dRzz = dIzz_a;
    // 0 : Outer, 1 : Inner
    double dy_a1 = 0.0, dz_a1 = 0.0, dy_a2 = 0.0, dz_a2 = 0.0; // Girder
    double dy_c1 = 0.0, dz_c1 = 0.0, dy_c2 = 0.0, dz_c2 = 0.0; // Composite
    double dc = 0.0, dt = 0.0;
    double dStress_Out = 0.0, dStress_In = 0.0;

    // Make Top Flange
    ClassInD.aTopFlangeD.SetSize(3);

    // Top Left
    double dbft_l	= m_pDgnSectUtil->Get_Size_bft_l(&rSectD.SectD, bPosiI);
    double dtwl		= m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dtft		= m_pDgnSectUtil->Get_Size_tft(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx1; dy_a2 = GirderStiff.dx1 + dbft_l - dtwl;
    dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
    dy_c1 = SectStiffD.dx1; dy_c2 = SectStiffD.dx1 + dbft_l - dtwl;
    dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[0].nType		= 0;
    ClassInD.aTopFlangeD[0].dc			= dc;
    ClassInD.aTopFlangeD[0].dt			= dt;
    ClassInD.aTopFlangeD[0].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[0].dStressInter= dStress_In;

    // Top Middle
    double dbft_m	= m_pDgnSectUtil->Get_Size_bft_m(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx1 + dbft_l;	dy_a2 = dy_a1 + dbft_m;
    dz_a1 = GirderStiff.dy1;			dz_a2 = GirderStiff.dy1;
    dy_c1 = SectStiffD.dx1 + dbft_l;	dy_c2 = dy_c1 + dbft_m;
    dz_c1 = SectStiffD.dy1;				dz_c2 = SectStiffD.dy1;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[1].nType		= 1;
    ClassInD.aTopFlangeD[1].dc			= dc;
    ClassInD.aTopFlangeD[1].dt			= dt;
    ClassInD.aTopFlangeD[1].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[1].dStressInter= dStress_In;

    // Top Right
    double dbft_r	= m_pDgnSectUtil->Get_Size_bft_r(&rSectD.SectD, bPosiI);
    double dtwr		= m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
    dy_a1 = GirderStiff.dx2; dy_a2 = GirderStiff.dx2 - dbft_r + dtwr;
    dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
    dy_c1 = SectStiffD.dx2; dy_c2 = SectStiffD.dx2 - dbft_r + dtwr;
    dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[2].nType		= 0;
    ClassInD.aTopFlangeD[2].dc			= dc;
    ClassInD.aTopFlangeD[2].dt			= dt;
    ClassInD.aTopFlangeD[2].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[2].dStressInter= dStress_In;


    // Make Bottom Flange
    ClassInD.aBotFlangeD.SetSize(3);

    // Bottom Right
    double dbfb_r	= m_pDgnSectUtil->Get_Size_bfb_r(&rSectD.SectD, bPosiI);
    double dtfb		= m_pDgnSectUtil->Get_Size_tfb(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx3; dy_a2 = GirderStiff.dx3 - dbfb_r + dtwr;
    dz_a1 = GirderStiff.dy3; dz_a2 = GirderStiff.dy3;
    dy_c1 = SectStiffD.dx3; dy_c2 = SectStiffD.dx3 - dbfb_r + dtwr;
    dz_c1 = SectStiffD.dy3; dz_c2 = SectStiffD.dy3;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[0].nType		= 0;
    ClassInD.aBotFlangeD[0].dc			= dc;
    ClassInD.aBotFlangeD[0].dt			= dt;
    ClassInD.aBotFlangeD[0].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[0].dStressInter= dStress_In;

    // Bottom Middle
    double dbfb_l	= m_pDgnSectUtil->Get_Size_bfb_l(&rSectD.SectD, bPosiI);
    double dbfb_m	= m_pDgnSectUtil->Get_Size_bfb_m(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx4 + dbfb_l;		dy_a2 = dy_a1 + dbfb_m;
    dz_a1 = GirderStiff.dy4;				dz_a2 = GirderStiff.dy4;
    dy_c1 = SectStiffD.dx4 + dbfb_l;		dy_c2 = dy_c1 + dbfb_m;
    dz_c1 = SectStiffD.dy4;					dz_c2 = SectStiffD.dy4;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[1].nType		= 1;
    ClassInD.aBotFlangeD[1].dc			= dc;
    ClassInD.aBotFlangeD[1].dt			= dt;
    ClassInD.aBotFlangeD[1].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[1].dStressInter= dStress_In;

    // Bottom Left
    dy_a1 = GirderStiff.dx4;	dy_a2 = GirderStiff.dx4 + dbfb_l - dtwl;
    dz_a1 = GirderStiff.dy4;	dz_a2 = GirderStiff.dy4;
    dy_c1 = SectStiffD.dx4;		dy_c2 = SectStiffD.dx4 + dbfb_l - dtwl;
    dz_c1 = SectStiffD.dy4;		dz_c2 = SectStiffD.dy4;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[2].nType		= 0;
    ClassInD.aBotFlangeD[2].dc			= dc;
    ClassInD.aBotFlangeD[2].dt			= dt;
    ClassInD.aBotFlangeD[2].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[2].dStressInter= dStress_In;

    // Make Web
    ClassInD.aWebD.SetSize(2);

    // Left Web
    dy_a1 = GirderStiff.dx1 + dbft_l - 0.5*dtwl;	dy_a2 = GirderStiff.dx4 + dbfb_l - 0.5*dtwl;
    dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
    dy_c1 = SectStiffD.dx1 + dbft_l - 0.5*dtwl;		dy_c2 = SectStiffD.dx4 + dbfb_l - 0.5*dtwl;
    dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 0, bPosiI);
    dt = dtwl;
    ClassInD.aWebD[0].nType			= 1;
    ClassInD.aWebD[0].dc			= dc;
    ClassInD.aWebD[0].dt			= dt;
    ClassInD.aWebD[0].dStressOuter	= dStress_Out;
    ClassInD.aWebD[0].dStressInter	= dStress_In;

    // Right Web
    dy_a1 = GirderStiff.dx2 - dbft_r + 0.5*dtwr;	dy_a2 = GirderStiff.dx2 - dbfb_r + 0.5*dtwr;
    dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
    dy_c1 = SectStiffD.dx2 - dbft_r + 0.5*dtwr;		dy_c2 = SectStiffD.dx2 - dbfb_r + 0.5*dtwr;
    dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 1, bPosiI);
    dt = dtwr;
    ClassInD.aWebD[1].nType			= 1;
    ClassInD.aWebD[1].dc			= dc;
    ClassInD.aWebD[1].dt			= dt;
    ClassInD.aWebD[1].dStressOuter	= dStress_Out;
    ClassInD.aWebD[1].dStressInter	= dStress_In;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Make_ClassInD_Tub(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC& ClassInD)
{
    ClassInD.Initialize();

    // Get Force
    double dFx_a = aGirderForce[0],		dMy_a = aGirderForce[4],	dMz_a = aGirderForce[5];
    double dFx_c = aCptForce[0],		dMy_c = aCptForce[4],		dMz_c = aCptForce[5];

    // Get Stiffness
    T_SECT_STIFFNESS GirderStiff, PosiStiffD, NegaStiffD;
    Get_GirderStiffnessofCompo(bPosiI, rSectD.SectD, GirderStiff);
    GetNonComSectStiffD(rSectD, bPosiI, NegaStiffD);
    GetRebarSectStiffD(rSectD, bPosiI, PosiStiffD);
    const T_SECT_STIFFNESS& SectStiffD = dMy_c > 0.0 ? PosiStiffD : NegaStiffD;

    double dA_a	= GirderStiff.Area, dIyy_a = GirderStiff.Ryy,	dIzz_a =  GirderStiff.Rzz;
    double dA_c	= SectStiffD.Area,	dIyy_c = SectStiffD.Ryy,	dIzz_c = SectStiffD.Rzz;
    m_dRyy = dIyy_a;
    m_dRzz = dIzz_a;
    // 0 : Outer, 1 : Inner
    double dy_a1 = 0.0, dz_a1 = 0.0, dy_a2 = 0.0, dz_a2 = 0.0; // Girder
    double dy_c1 = 0.0, dz_c1 = 0.0, dy_c2 = 0.0, dz_c2 = 0.0; // Composite
    double dc = 0.0, dt = 0.0;
    double dStress_Out = 0.0, dStress_In = 0.0;

    // Make Top Flange
    ClassInD.aTopFlangeD.SetSize(4);

    // Top Left - Left Part
    double dbft_ll	= m_pDgnSectUtil->Get_Size_bft_ll(&rSectD.SectD, bPosiI);
    double dtwl		= m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dtft		= m_pDgnSectUtil->Get_Size_tft(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx1; dy_a2 = GirderStiff.dx1 + dbft_ll - dtwl;
    dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
    dy_c1 = SectStiffD.dx1; dy_c2 = SectStiffD.dx1 + dbft_ll - dtwl;
    dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[0].nType		= 0;
    ClassInD.aTopFlangeD[0].dc			= dc;
    ClassInD.aTopFlangeD[0].dt			= dt;
    ClassInD.aTopFlangeD[0].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[0].dStressInter= dStress_In;

    // Top Left - Right Part
    double dbft_l = m_pDgnSectUtil->Get_Size_bft_l(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx1 + dbft_ll + dtwl; dy_a2 = GirderStiff.dx1 + dbft_l;
    dz_a1 = GirderStiff.dy1; dz_a2 = GirderStiff.dy1;
    dy_c1 = SectStiffD.dx1 + dbft_ll + dtwl; dy_c2 = SectStiffD.dx1 + dbft_l;
    dz_c1 = SectStiffD.dy1; dz_c2 = SectStiffD.dy1;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[1].nType		= 0;
    ClassInD.aTopFlangeD[1].dc			= dc;
    ClassInD.aTopFlangeD[1].dt			= dt;
    ClassInD.aTopFlangeD[1].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[1].dStressInter= dStress_In;


    // Top Right - Right Part
    double dbft_rr	= m_pDgnSectUtil->Get_Size_bft_rr(&rSectD.SectD, bPosiI);
    double dtwr		= m_pDgnSectUtil->Get_Size_tw(&rSectD.SectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
    dy_a1 = GirderStiff.dx2; dy_a2 = GirderStiff.dx2 - dbft_rr + dtwr;
    dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
    dy_c1 = SectStiffD.dx2; dy_c2 = SectStiffD.dx2 - dbft_rr + dtwr;
    dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[2].nType		= 0;
    ClassInD.aTopFlangeD[2].dc			= dc;
    ClassInD.aTopFlangeD[2].dt			= dt;
    ClassInD.aTopFlangeD[2].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[2].dStressInter= dStress_In;

    // Top Right - Left Part
    double dbft_r	= m_pDgnSectUtil->Get_Size_bft_r(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx2 - dbft_rr; dy_a2 = GirderStiff.dx2 - dbft_r;
    dz_a1 = GirderStiff.dy2; dz_a2 = GirderStiff.dy2;
    dy_c1 = SectStiffD.dx2- dbft_rr; dy_c2 = SectStiffD.dx2 - dbft_r;
    dz_c1 = SectStiffD.dy2; dz_c2 = SectStiffD.dy2;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtft;
    ClassInD.aTopFlangeD[3].nType		= 0;
    ClassInD.aTopFlangeD[3].dc			= dc;
    ClassInD.aTopFlangeD[3].dt			= dt;
    ClassInD.aTopFlangeD[3].dStressOuter= dStress_Out;
    ClassInD.aTopFlangeD[3].dStressInter= dStress_In;


    // Make Bottom Flange
    ClassInD.aBotFlangeD.SetSize(3);

    // Bottom Right
    double dbfb_r	= m_pDgnSectUtil->Get_Size_bfb_r(&rSectD.SectD, bPosiI);
    double dtfb		= m_pDgnSectUtil->Get_Size_tfb(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx3; dy_a2 = GirderStiff.dx3 - dbfb_r + dtwr;
    dz_a1 = GirderStiff.dy3; dz_a2 = GirderStiff.dy3;
    dy_c1 = SectStiffD.dx3; dy_c2 = SectStiffD.dx3 - dbfb_r + dtwr;
    dz_c1 = SectStiffD.dy3; dz_c2 = SectStiffD.dy3;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[0].nType		= 0;
    ClassInD.aBotFlangeD[0].dc			= dc;
    ClassInD.aBotFlangeD[0].dt			= dt;
    ClassInD.aBotFlangeD[0].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[0].dStressInter= dStress_In;

    // Bottom Middle
    double dbfb_l	= m_pDgnSectUtil->Get_Size_bfb_l(&rSectD.SectD, bPosiI);
    double dbfb_m	= m_pDgnSectUtil->Get_Size_bfb_m(&rSectD.SectD, bPosiI);
    dy_a1 = GirderStiff.dx4 + dbfb_l;		dy_a2 = dy_a1 + dbfb_m;
    dz_a1 = GirderStiff.dy4;				dz_a2 = GirderStiff.dy4;
    dy_c1 = SectStiffD.dx4 + dbfb_l;		dy_c2 = dy_c1 + dbfb_m;
    dz_c1 = SectStiffD.dy4;					dz_c2 = SectStiffD.dy4;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[1].nType		= 1;
    ClassInD.aBotFlangeD[1].dc			= dc;
    ClassInD.aBotFlangeD[1].dt			= dt;
    ClassInD.aBotFlangeD[1].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[1].dStressInter= dStress_In;

    // Bottom Left
    dy_a1 = GirderStiff.dx4;	dy_a2 = GirderStiff.dx4 + dbfb_l - dtwl;
    dz_a1 = GirderStiff.dy4;	dz_a2 = GirderStiff.dy4;
    dy_c1 = SectStiffD.dx4;		dy_c2 = SectStiffD.dx4 + dbfb_l - dtwl;
    dz_c1 = SectStiffD.dy4;		dz_c2 = SectStiffD.dy4;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = fabs(dy_a1 - dy_a2);
    dt = dtfb;
    ClassInD.aBotFlangeD[2].nType		= 0;
    ClassInD.aBotFlangeD[2].dc			= dc;
    ClassInD.aBotFlangeD[2].dt			= dt;
    ClassInD.aBotFlangeD[2].dStressOuter= dStress_Out;
    ClassInD.aBotFlangeD[2].dStressInter= dStress_In;

    // Make Web
    ClassInD.aWebD.SetSize(2);

    // Left Web
    dy_a1 = GirderStiff.dx1 + dbft_ll - 0.5*dtwl;	dy_a2 = GirderStiff.dx4 + dbfb_l - 0.5*dtwl;
    dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
    dy_c1 = SectStiffD.dx1 + dbft_ll - 0.5*dtwl;	dy_c2 = SectStiffD.dx4 + dbfb_l - 0.5*dtwl;
    dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 0, bPosiI);
    dt = dtwl;
    ClassInD.aWebD[0].nType			= 1;
    ClassInD.aWebD[0].dc			= dc;
    ClassInD.aWebD[0].dt			= dt;
    ClassInD.aWebD[0].dStressOuter	= dStress_Out;
    ClassInD.aWebD[0].dStressInter	= dStress_In;

    // Right Web
    dy_a1 = GirderStiff.dx2 - dbft_rr + 0.5*dtwr;	dy_a2 = GirderStiff.dx2 - dbfb_r + 0.5*dtwr;
    dz_a1 = GirderStiff.dy1 - dtft;					dz_a2 = GirderStiff.dy4 + dtfb;
    dy_c1 = SectStiffD.dx2 - dbft_rr + 0.5*dtwr;	dy_c2 = SectStiffD.dx2 - dbfb_r + 0.5*dtwr;
    dz_c1 = SectStiffD.dy1 - dtft;					dz_c2 = SectStiffD.dy4 + dtfb;
    dStress_Out = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a1, dz_a1);
    dStress_Out += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c1, dz_c1);
    dStress_In = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dA_a, dIyy_a, dIyy_a, dy_a2, dz_a2);
    dStress_In += Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dA_c, dIyy_c, dIyy_c, dy_c2, dz_c2);
    dc = m_pDgnSectUtil->Get_Size_Inclind_hw(&rSectD.SectD, 1, bPosiI);
    dt = dtwr;
    ClassInD.aWebD[1].nType			= 1;
    ClassInD.aWebD[1].dc			= dc;
    ClassInD.aWebD[1].dt			= dt;
    ClassInD.aWebD[1].dStressOuter	= dStress_Out;
    ClassInD.aWebD[1].dStressInter	= dStress_In;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_BendingResistanceForPlateGirder(ElemPairK ElemK, T_PGBR_LCOM& PgbrLcomD)
{	
    //+++++++++++++++++++++++++++++++++++++++
    // CurrUnit -> CodeUnit.
    //  m_pDataCtrl->Get_BendingResistanceForPlateGirderCurrToCode(PgbrLcomD);
    //+++++++++++++++++++++++++++++++++++++++
    BOOL bCalcOK=TRUE;
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bTranStiff=FALSE;
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
        return FALSE;
    }
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
        return FALSE;
    }
    CPG_RBAR_ELEM RbarElem;
    if(Get_CpgRbarElem(ElemK, RbarElem)) bRebar=TRUE;

    CPG_SCON_ELEM SconElem;
    if(Get_CpgSconElem(ElemK, SconElem)) bShearConnector=TRUE;

    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) bTranStiff=TRUE;

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);

        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PgbrLcomD.PgbrBase[i].LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = bPosiI ? 0 : 1;

        T_CLAS_BASE ClassD;
        ClassD.Initialize();
        m_ClassSectMap.Lookup(ComfKey,ClassD);  

        // Longitudinal Stiffener  
        CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);

        // Transverse Stiffener
        CPG_TRST_POSI TrstPosi;
        TrstPosi = TrstElem.TrstPosi[i];

        if(PgbrLcomD.PgbrBase[i].bChk==FALSE) continue; 
        T_PGBR_DETAIL PgbrDetail;
        //if(!Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, PgbrLcomD.PgbrBase[i], PgbrDetail)) continue;    
        m_LcomPgbrMap.SetAt(ComfKey,PgbrLcomD.PgbrBase[i]);
    }
    //+++++++++++++++++++++++++++++++++++++++
    // CodeUnit -> CurrUnit.
    //m_pDataCtrl->Get_BendingResistanceForPlateGirderCodeToCurr(PgbrLcomD);
    //+++++++++++++++++++++++++++++++++++++++  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_VerticalShearResistanceForPlateGirder(ElemPairK ElemK, T_PRVS_D& PrvsD)
{
    //+++++++++++++++++++++++++++++++++++++++
    // CurrUnit -> CodeUnit.
    //m_pDataCtrl->Get_VerticalShearResistanceForPlateGirderCurrToCode(PrvsD);
    //+++++++++++++++++++++++++++++++++++++++
    BOOL bCalcOK=TRUE;
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_VerticalShearResistanceForPlateGirderCodeToCurr(PrvsD);
        return FALSE;
    }
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_VerticalShearResistanceForPlateGirderCodeToCurr(PrvsD);
        return FALSE;
    }
    CPG_RBAR_ELEM RbarElem;
    if(Get_CpgRbarElem(ElemK, RbarElem)) bRebar=TRUE;

    CPG_SCON_ELEM SconElem;
    SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) bShearConnector=TRUE;
    //
    CPG_LSTI_ELEM LstiElem;
    LstiElem.Initialize();
    if(Get_CpgLstiElem(ElemK, LstiElem)) bLongStiff=TRUE;
    //
    CPG_TRST_ELEM TrstElem;
    TrstElem.Initialize();
    if(Get_CpgTrstElem(ElemK, TrstElem)) bTranStiff=TRUE;
    //
    CPG_TSES_ELEM TsesElem;
    TsesElem.Initialize();
    if(Get_CpgTsesElem(ElemK, TsesElem)) bEndSupport=TRUE;

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PrvsD.PrvsBase[i].LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = bPosiI ? 0 : 1;

        T_CLAS_BASE ClassD;
        ClassD.Initialize();
        m_ClassSectMap.Lookup(ComfKey,ClassD);  

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);    
        // Longitudinal Stiffener  
        CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
        arLstiPosi.Copy(LstiElem.LstiPosi[i].arLstiPosi);
        // Transverse Stiffener
        CPG_TRST_POSI TrstPosi;
        TrstPosi = TrstElem.TrstPosi[i];
        // End Support
        CPG_TSES_POSI TsesPosi;
        TsesPosi = TsesElem.TsesPosi[i];    

        if(PrvsD.PrvsBase[i].bChk==FALSE) continue; 
        T_PRVS_DETAIL PrvsDetail;
        //if(!Calc_VerticalShearResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, PrvsD.PrvsBase[i], PrvsDetail)) continue;    
    }
    //+++++++++++++++++++++++++++++++++++++++
    // CodeUnit -> CurrUnit.
    //m_pDataCtrl->Get_VerticalShearResistanceForPlateGirderCodeToCurr(PrvsD);
    //+++++++++++++++++++++++++++++++++++++++  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LateralTorsionalBucklingResistanceForPlateGirder(ElemPairK ElemK, T_PLTB_D& PltbD)
{
    //+++++++++++++++++++++++++++++++++++++++
    // CurrUnit -> CodeUnit.
    //m_pDataCtrl->Get_LateralTorsionalBucklingResistanceForPlateGirderCurrToCode(PltbD);
    //+++++++++++++++++++++++++++++++++++++++
    BOOL bCalcOK=TRUE;
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem))
    {
        // CodeUnit -> CurrUnit
        //m_pDataCtrl->Get_LateralTorsionalBucklingResistanceForPlateGirderCodeToCurr(PltbD);
        return FALSE;
    }
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem))
    {
        // CodeUnit -> CurrUnit
        //m_pDataCtrl->Get_LateralTorsionalBucklingResistanceForPlateGirderCodeToCurr(PltbD);
        return FALSE;
    }
    CPG_RBAR_ELEM RbarElem;
    if(Get_CpgRbarElem(ElemK, RbarElem)) bRebar=TRUE;

    CPG_SCON_ELEM SconElem;
    SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) bShearConnector=TRUE;
    //
    CPG_LSTI_ELEM LstiElem;
    LstiElem.Initialize();
    if(Get_CpgLstiElem(ElemK, LstiElem)) bLongStiff=TRUE;
    //
    CPG_TRST_ELEM TrstElem;
    TrstElem.Initialize();
    if(Get_CpgTrstElem(ElemK, TrstElem)) bTranStiff=TRUE;
    //
    CPG_TSES_ELEM TsesElem;
    TsesElem.Initialize();
    if(Get_CpgTsesElem(ElemK, TsesElem)) bEndSupport=TRUE;
    //
    CPG_LTBD_ELEM LtbdElem;
    LtbdElem.Initialize();
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) bLtbData=TRUE;
    //
    CPG_MEMB_ELEM MembElem;
    MembElem.Initialize();
    if(Get_CpgMembElem(ElemK, MembElem)) bMembData=TRUE;  

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PltbD.PltbBase[i].LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = bPosiI ? 0 : 1;

        T_CLAS_BASE ClassD;
        ClassD.Initialize();
        m_ClassSectMap.Lookup(ComfKey,ClassD);  

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);    
        // Longitudinal Stiffener  
        CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
        arLstiPosi.Copy(LstiElem.LstiPosi[i].arLstiPosi);
        // Transverse Stiffener
        CPG_TRST_POSI TrstPosi;
        TrstPosi = TrstElem.TrstPosi[i];
        // End Support
        CPG_TSES_POSI TsesPosi;
        TsesPosi = TsesElem.TsesPosi[i];    
        // Later-torsional buckling data
        CPG_LTBD_POSI LtbdPosi;
        LtbdPosi = LtbdElem.LtbdPosi[i];
        // Member data, Lu
        CPG_MEMB_POSI MembPosi;
        MembPosi = MembElem.MembPosi[i];

        if(PltbD.PltbBase[i].bChk==FALSE) continue;   
        T_PLTB_DETAIL PltbDetail;
        if(!Calc_LateralTorsionalBucklingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, LtbdPosi, MembPosi, ClassD, PltbD.PltbBase[i], PltbDetail)) continue;    
    }
    //+++++++++++++++++++++++++++++++++++++++
    // CodeUnit -> CurrUnit
    //m_pDataCtrl->Get_LateralTorsionalBucklingResistanceForPlateGirderCodeToCurr(PltbD);
    //+++++++++++++++++++++++++++++++++++++++  


    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_TransverseForceResistanceForPlateGirder(ElemPairK ElemK, T_PRTF_D& PrtfD)
{
    //+++++++++++++++++++++++++++++++++++++++
    // CurrUnit -> CodeUnit.
    //m_pDataCtrl->Get_TransverseForceResistanceForPlateGirderCurrToCode(PrtfD);
    //+++++++++++++++++++++++++++++++++++++++
    BOOL bCalcOK=TRUE;
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bScon=FALSE;
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_TransverseForceResistanceForPlateGirderCodeToCurr(PrtfD);
        return FALSE;
    }
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_TransverseForceResistanceForPlateGirderCodeToCurr(PrtfD);
        return FALSE;
    }
    CPG_RBAR_ELEM RbarElem;
    if(Get_CpgRbarElem(ElemK, RbarElem)) bRebar=TRUE;

    CPG_SCON_ELEM SconElem;
    SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) bShearConnector=TRUE;
    //
    CPG_LSTI_ELEM LstiElem;
    LstiElem.Initialize();
    if(Get_CpgLstiElem(ElemK, LstiElem)) bLongStiff=TRUE;
    //
    CPG_TRST_ELEM TrstElem;
    TrstElem.Initialize();
    if(Get_CpgTrstElem(ElemK, TrstElem)) bTranStiff=TRUE;
    //
    CPG_TSES_ELEM TsesElem;
    TsesElem.Initialize();
    if(Get_CpgTsesElem(ElemK, TsesElem)) bEndSupport=TRUE;
    //
    CPG_BCDT_ELEM BcdtElem;
    BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) bBuckCoeff=TRUE;



    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PrtfD.PrtfBase[i].LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = bPosiI ? 0 : 1;

        T_CLAS_BASE ClassD;
        ClassD.Initialize();
        m_ClassSectMap.Lookup(ComfKey,ClassD);  

        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);    
        // Longitudinal Stiffener  
        CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
        arLstiPosi.Copy(LstiElem.LstiPosi[i].arLstiPosi);
        // Transverse Stiffener
        CPG_TRST_POSI TrstPosi;
        TrstPosi = TrstElem.TrstPosi[i];
        // End Support
        CPG_TSES_POSI TsesPosi;
        TsesPosi = TsesElem.TsesPosi[i]; 
        // Buckling coefficients for different types of load application.
        CPG_BCDT_POSI BcdtPosi;
        BcdtPosi = BcdtElem.BcdtPosi[i]; 

        // Shear Connector.
        CPG_SCON_POSI SconPosi;
        SconPosi = SconElem.SconPosi[i] ;

        if(PrtfD.PrtfBase[i].bChk==FALSE) continue;    
        if(!bBuckCoeff || (bBuckCoeff && !BcdtPosi.bUseTfType))
        {
            PrtfD.PrtfBase[i].bChk = FALSE;
            continue;
        }
        T_PRTF_DETAIL PrtfDetail;
        if(!Calc_TransverseForceResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, BcdtPosi, ClassD, PrtfD.PrtfBase[i], PrtfDetail,SconPosi)) continue;    
    }
    //+++++++++++++++++++++++++++++++++++++++
    // CodeUnit -> CurrUnit.
    //m_pDataCtrl->Get_TransverseForceResistanceForPlateGirderCodeToCurr(PrtfD);
    //+++++++++++++++++++++++++++++++++++++++  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LongitudinalShearResistanceForPlateGirder(ElemPairK ElemK, T_PGLS_D& PglsD)
{
    //+++++++++++++++++++++++++++++++++++++++
    // CurrUnit -> CodeUnit.
    //m_pDataCtrl->Get_LongitudinalShearResistanceForPlateGirderCurrToCode(PglsD);
    //+++++++++++++++++++++++++++++++++++++++
    BOOL bCalcOK=TRUE;
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_LongitudinalShearResistanceForPlateGirderCodeToCurr(PglsD);
        return FALSE;
    }
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_LongitudinalShearResistanceForPlateGirderCodeToCurr(PglsD);
        return FALSE;
    }
    CPG_RBAR_ELEM RbarElem;
    if(Get_CpgRbarElem(ElemK, RbarElem)) bRebar=TRUE;

    CPG_SCON_ELEM SconElem;
    SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) bShearConnector=TRUE;
    //
    CPG_LSTI_ELEM LstiElem;
    LstiElem.Initialize();
    if(Get_CpgLstiElem(ElemK, LstiElem)) bLongStiff=TRUE;
    //
    CPG_TRST_ELEM TrstElem;
    TrstElem.Initialize();
    if(Get_CpgTrstElem(ElemK, TrstElem)) bTranStiff=TRUE;
    //
    CPG_TSES_ELEM TsesElem;
    TsesElem.Initialize();
    if(Get_CpgTsesElem(ElemK, TsesElem)) bEndSupport=TRUE;

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PglsD.PglsBase[i].LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = bPosiI ? 0 : 1;

        T_CLAS_BASE ClassD;
        ClassD.Initialize();
        m_ClassSectMap.Lookup(ComfKey,ClassD);  

        // Shear Connector.
        CPG_SCON_POSI SconPosi;
        SconPosi = SconElem.SconPosi[i];
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);    
        // Longitudinal Stiffener  
        CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
        arLstiPosi.Copy(LstiElem.LstiPosi[i].arLstiPosi);
        // Transverse Stiffener
        CPG_TRST_POSI TrstPosi;
        TrstPosi = TrstElem.TrstPosi[i];
        // End Support
        CPG_TSES_POSI TsesPosi;
        TsesPosi = TsesElem.TsesPosi[i];    

        if(PglsD.PglsBase[i].bChk==FALSE) continue;
        if(!bShearConnector || (bShearConnector && !SconPosi.bUseShear)) 
        {
            PglsD.PglsBase[i].bChk = FALSE;
            continue;
        }
        T_PGLS_DETAIL PglsDetail;
        BOOL bSLS=FALSE;
        double dks=m_CpgdD.dks;
        //if(!Calc_LongitudinalShearResistance(ElemK, bPosiI, bSLS, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, PglsD.PglsBase[i], PglsDetail)) continue;    
        m_LcomPglsMap.SetAt(ComfKey,PglsD.PglsBase[i]);
    }
    //+++++++++++++++++++++++++++++++++++++++
    // CodeUnit -> CurrUnit.
    //m_pDataCtrl->Get_LongitudinalShearResistanceForPlateGirderCodeToCurr(PglsD);
    //+++++++++++++++++++++++++++++++++++++++  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_FatigueResistanceForPlateGirder(ElemPairK ElemK, T_PGRF_D& PgrfD, double dt_Ld)
{
    //+++++++++++++++++++++++++++++++++++++++
    // CurrUnit -> CodeUnit.
    //m_pDataCtrl->Get_FatigueResistanceForPlateGirderCurrToCode(PgrfD);
    //+++++++++++++++++++++++++++++++++++++++
    BOOL bCalcOK=TRUE;
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLamdaData=FALSE;
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_FatigueResistanceForPlateGirderCodeToCurr(PgrfD);
        return FALSE;
    }
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem))
    {
        // CodeUnit. -> CurrUnit
        //m_pDataCtrl->Get_FatigueResistanceForPlateGirderCodeToCurr(PgrfD);
        return FALSE;
    }
    CPG_RBAR_ELEM RbarElem;
    if(Get_CpgRbarElem(ElemK, RbarElem)) bRebar=TRUE;

    CPG_SCON_ELEM SconElem;
    SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) bShearConnector=TRUE;
    //
    CPG_LSTI_ELEM LstiElem;
    LstiElem.Initialize();
    if(Get_CpgLstiElem(ElemK, LstiElem)) bLongStiff=TRUE;
    //
    CPG_TRST_ELEM TrstElem;
    TrstElem.Initialize();
    if(Get_CpgTrstElem(ElemK, TrstElem)) bTranStiff=TRUE;
    //
    CPG_TSES_ELEM TsesElem;
    TsesElem.Initialize();
    if(Get_CpgTsesElem(ElemK, TsesElem)) bEndSupport=TRUE;
    //
    CPG_FRDT_ELEM FrdtElem;
    FrdtElem.Initialize();
    if(Get_CpgFrdtElem(ElemK, FrdtElem)) bLamdaData=TRUE;


    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PgrfD.PgrfBase[i].LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = bPosiI ? 0 : 1;

        T_CLAS_BASE ClassD;
        ClassD.Initialize();
        m_ClassSectMap.Lookup(ComfKey,ClassD);  

        // Shear Connector.
        CPG_SCON_POSI SconPosi;
        SconPosi = SconElem.SconPosi[i];
        // RBar
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
        arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);    
        // Longitudinal Stiffener  
        CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
        arLstiPosi.Copy(LstiElem.LstiPosi[i].arLstiPosi);
        // Transverse Stiffener
        CPG_TRST_POSI TrstPosi;
        TrstPosi = TrstElem.TrstPosi[i];
        // End Support
        CPG_TSES_POSI TsesPosi;
        TsesPosi = TsesElem.TsesPosi[i];    
        // Lamda_v2, Lamda_v4
        CPG_FRDT_POSI FrdtPosi;
        FrdtPosi = FrdtElem.FrdtPosi[i];

        if(PgrfD.PgrfBase[i].bChk==FALSE) continue;
        if(!bShearConnector || (bShearConnector && !SconPosi.bUseShear))  
        {
            PgrfD.PgrfBase[i].bChk=FALSE;
            continue;
        }
        T_PGRF_DETAIL PgrfDetail;
        if(!Calc_FatigueResistance(ElemK, bPosiI, dt_Ld, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, FrdtPosi, ClassD, PgrfD.PgrfBase[i], PgrfDetail)) continue;    
    }
    //+++++++++++++++++++++++++++++++++++++++
    // CodeUnit -> CurrUnit.
    //m_pDataCtrl->Get_FatigueResistanceForPlateGirderCodeToCurr(PgrfD);
    //+++++++++++++++++++++++++++++++++++++++  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_BeamStressResistanceForPlateGirder(ElemPairK ElemK, T_PGBS_D& PgbsD)
{
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_DesignResultExist(BOOL* abChkItem)
{
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_PlateGirderDgnResult(BOOL* abChkItem)
{
    //////////////////////////////////////////////////////////////////////////
    // 1. Get General Input Data
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc);
    CPlateGirderDesign* pPGD = pDoc->m_pPostCtrl->GetPlateGirder();
    ASSERT(pPGD);

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);  

	ArrElemPairKey aElemPairKey;
    m_pDataCtrl->GetPgdElemKeyList(aElemPairKey);
    int iTotalElement = aElemPairKey.GetSize();

    int iDgnStatus = pDoc->GetDesignStatus();
    pDoc->SetDesignStatus(6);

    T_CPGD_D CpgdD; CpgdD.Initialize();  
    if(!pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD)){ASSERT(0);}
    BOOL bCheckStrength[6]={0};
    BOOL bCheckService[2]={0};
    for(int i=0; i < D_CPG_CHECK_ITEM; i++) {abChkItem[i]=FALSE;}
    for(int i=0; i < 6; i++) 
    {    
        bCheckStrength[i] = CpgdD.bStrength[i];
    }
    for(int i=0; i<2; i++)
    {
        bCheckService[i] = CpgdD.bService[i];
    }
    double dt_Ld = CpgdD.dt_Ld;  
    double dk1 = CpgdD.dk1;
    double dk2 = CpgdD.dk2;
    double dk3 = CpgdD.dk3;
    double dks = CpgdD.dks;

    // Moment 최대만 찾아서 설계?. 
    // Strength 전체 하중조합으로 하고 
    int iStrnLcomSize = m_pForcCtrl->Get_LcomNumForStrn();
    int iServLcomTypeSize = m_pForcCtrl->Get_LcomNumForSLS();

    T_GENL_DATA GLineData;
    T_GLINE_DGN_D GLineDgn;  
    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();  

    //////////////////////////////////////////////////////////////////////////
    // 2. Repeat to check element
    for(int i=0; i<iTotalElement; i++)
    {
        auto ElemK = aElemPairKey[i];

        BOOL bChkIJ[2] = {FALSE, FALSE};
        bChkIJ[0] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 1);
        bChkIJ[1] = m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK, 2);

        if(bChkIJ[0] || bChkIJ[1])
        {      
            if(iStrnLcomSize > 0)
            {
                for(int j = 0; j < 6; ++j)
                {
                    if(bCheckStrength[j]) {abChkItem[j] = TRUE;}
                }
            }

            if(iServLcomTypeSize > 0)
            {
                for(int j = 0; j < 2; ++j)
                {
                    if(bCheckService[j]) {abChkItem[j+6] = TRUE;}
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // 3. Get Design Data
        BOOL bRebar=FALSE;
        BOOL bShearConnector=FALSE;
        BOOL bLongStiff=FALSE;
        BOOL bTranStiff=FALSE;
        BOOL bEndSupport=FALSE;
        BOOL bLtbData=FALSE;
        BOOL bMembData=FALSE;
        BOOL bBuckCoeff=FALSE;
        BOOL bLamdaData=FALSE;

        CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
        if(!Get_CpgMatlElem(ElemK, MatlElem)) {continue;}
        //
        CPG_SECT_ELEM SectElem; SectElem.Initialize();
        if(Get_CpgSectElem(ElemK, SectElem)) {}    
        //
        CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
        if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
        //
        CPG_SCON_ELEM SconElem; SconElem.Initialize();
        if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
        //
        CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
        if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
        //
        CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
        if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
        //
        CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
        if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
        //
        CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
        if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
        //
        CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
        if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE;  }
        //
        CPG_BCDT_ELEM BcdtElem;
        BcdtElem.Initialize();
        if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
        //
        CPG_FRDT_ELEM FrdtElem;
        FrdtElem.Initialize();
        if(Get_CpgFrdtElem(ElemK, FrdtElem)) {bLamdaData=TRUE;}

        BOOL bIsGenSect = SectElem.bGenSect;
        BOOL bTapered	= m_pDgnSectUtil->Is_TaperedSect(&SectElem.SectD);

        // 4. Get Member Data
        BOOL bReverse;
        ElemPairK showElemK = { 0,0 }, IendElemK = { 0,0 }, JendElemK = { 0,0 };
        GetEndElemOfMember(ElemK, showElemK, IendElemK, JendElemK);
        CArray<T_ELEM_K, T_ELEM_K> arMembElemList;

        if (ElemK.second == EN_EL_BEAM)
        {
            if(m_pMembCtrl->GetElemListByIncludeElem(ElemK.first, bReverse, showElemK.first, arMembElemList))
            {
                int iCountElem	= arMembElemList.GetSize();
                IendElemK			= {arMembElemList[0], EN_EL_BEAM};
                JendElemK			= {arMembElemList[iCountElem-1], EN_EL_BEAM };

                if(iCountElem==1) bissimplysup=TRUE;
            }
        }
        else ASSERT(0);

        m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
        m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

        // 5. Get Design Force
        int nFortype = 0;
        ADGNFORCE aForceLcom;
        if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{continue;}

        ADGNSTRESS aStressLcom;
        ADGNSTRSADD aStressAdd;    
        if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum))	{continue;}

        T_STAG_K nGdLastStagK=0;
        int nLastStepNo=0;
        m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);

        memset(m_dZbar_sb,	0,	sizeof(m_dZbar_sb));
        memset(m_dCzm_sb,	0,	sizeof(m_dCzm_sb));
        memset(m_dHt,		0,	sizeof(m_dHt));
        memset(m_aY_sb,		0,	sizeof(m_aY_sb));
        memset(m_aZ_sb,		0,	sizeof(m_aZ_sb));
        Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0], m_aY_sb[0], m_aZ_sb[0]);
        Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1], m_aY_sb[1], m_aZ_sb[1]);

        T_PGBR_LCOM PgbrPosiRat; PgbrPosiRat.Initialize();
        T_PGBR_LCOM PgbrNegaRat; PgbrNegaRat.Initialize();	  
        T_RHOC_LCOM RhocPosiRat; RhocPosiRat.Initialize();
        T_RHOC_LCOM RhocNegaRat; RhocNegaRat.Initialize();	  
        T_PRVS_D PrvsRat; PrvsRat.Initialize();        
        T_PLTB_D PltbRat; PltbRat.Initialize();
        T_PRTF_D PrtfRat; PrtfRat.Initialize();
        T_PGLS_D PglsRat; PglsRat.Initialize();
        T_PGRF_D PgrfRat; PgrfRat.Initialize();
        T_PGBS_D PgbsRat; PgbsRat.Initialize();
        T_PLSS_D PlssRat; PlssRat.Initialize();
        T_PGBS_GR_BASE PgbsGrCrD;
        T_PGBS_SB_BASE PgbsSbCrD;
        T_PGBS_BAR_BASE PgbsBarCrD;

        for(int j=0; j<2; j++)
        {
            PgbrPosiRat.PgbrBase[j].ElemK = ElemK.first;
            PgbrNegaRat.PgbrBase[j].ElemK = ElemK.first;
            PrvsRat.PrvsBase[j].ElemK		= ElemK.first;
            PltbRat.PltbBase[j].ElemK		= ElemK.first;
            PrtfRat.PrtfBase[j].ElemK		= ElemK.first;
            PglsRat.PglsBase[j].ElemK		= ElemK.first;
            PgrfRat.PgrfBase[j].ElemK		= ElemK.first;
            PgbsRat.PgbsBase[j].ElemK		= ElemK.first;
            PlssRat.PlssBase[j].ElemK		= ElemK.first;
        }

        int iMaxMin=0, iDgnLcomNo = 0, iOrgLcomNo = 0, iSerLcomCount = 0, iMaxMinType = 0, iServLcomType = 0;
        int iForceLcomSize = aForceLcom.GetSize();	
        CString strOrgLcomNa=_T("");
        double dC4 = 0.0;

        int iPosiCount[2] = {0,0}, iNegaCount[2] = {0,0};
        double dPosiMaxRatio[2] = {0.0, 0.0}, dNegaMaxRatio[2] = {0.0, 0.0};
        int iVCount[2]={0, 0};
        double dVMaxRatio[2]={0.0, 0.0};
        int iLTBCount[2]={0, 0};
        double dLTBMaxRatio[2]={0.0, 0.0};
        int iRTFCount[2]={0, 0};
        double dRTFMaxRatio[2]={0.0, 0.0};    
        int iLSCount[2]={0, 0};
        double dLSMaxRatio[2]={0.0, 0.0};    
        int iRFCount[2]={0, 0};
        double dRFMaxRatio[2]={0.0};    
        int iStressGrSLS_Count[2]={0, 0}; // girder stress
        double dStressGrMaxRatio[2]={0.0, 0.0};
        int iStressSbSLS_Count[2]={0, 0}; // slab stress
        double dStressSbMaxRatio[2]={0.0, 0.0};
        int iStressBarSLS_Count[2]={0, 0}; // Rebar stress
        double dStressBarMaxRatio[2]={0.0, 0.0};
        int iLS_SLS_Count[2]={0, 0};
        double dLS_SLSMaxRatio[2]={0.0, 0.0};

        //   //Pinakin for finding Lcom for longitudinal shear
        //   for(int j=0; j < iForceLcomSize; j++)
        //   {
        //     iDgnLcomNo = j+1;
        //     if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
        //     {
        //       ASSERT(0);
        //     }	
        // 
        //     BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(j+1);
        //     BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(j+1); 
        //     if(bSLSLcom) {iServLcomType = m_pForcCtrl->Get_ServLcomType(iDgnLcomNo);}
        //     const _DGN_FORC_CRC& ForceCrc = aForceLcom.GetAt(j);
        //     const _DGN_STRS_CRC& StressCrc = aStressLcom.GetAt(j);
        //       //Lcomb data
        //       _DGN_LCOM LcomDesign;
        //       INT_PTR nLcomSize = m_pForcCtrl->Get_LcomDataCount();
        // 
        //       INT_PTR nChkLcomCount = 0;
        // 
        //       double dMaxFactor=0.0;
        //       int    nChkLcomK=-1;      
        // 
        // 
        // 
        // 
        //       // I-End
        //       _DGN_FORC_CRC MembEndForce;
        //     double dM1=0.0, dV1=0.0, dM2=0.0, dV2=0.0;
        // 
        //     if(bULSLcom && bCheckStrength[2] && !bIsGenSect)
        //     {
        //       m_pForcCtrl->Get_LcomDataForDesign_LcomK(IendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);        
        //       dM1 = MembEndForce.dMuy[0];
        //       dV1 = MembEndForce.dFzz[0];
        //       
        //       //
        //       MembEndForce.Initialize();
        //       m_pForcCtrl->Get_LcomDataForDesign_LcomK(JendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);        
        //       dM2 = MembEndForce.dMuy[1];
        //       dV2 = MembEndForce.dFzz[1];
        //       // Get, C4 
        //       if(m_pDataCtrl->Get_C4valueForMomentType(iDgnLcomNo, showElemK, arMembElemList, dC4)) {}
        //       else {dC4=0.0;}
        // 
        //       
        //       
        //       ADGNFORCE aPreForceLcom;
        //       m_pForcCtrl->Get_BeforeForce4PreCompo(IendElemK, aPreForceLcom, iForceLcomSize);
        // 
        // 
        // 
        //       _DGN_FORC_CRC FcsDL;
        //       _DGN_FORC_CRC FcsEL1;
        //       _DGN_FORC_CRC FcsEL2;
        //       _DGN_FORC_CRC FcsEL3;
        //       _DGN_FORC_CRC FcsEL;
        //       _DGN_FORC_CRC ForceNC;
        //       m_pForcCtrl->Get_CSGirderForce4CompoBeforeNew(IendElemK, FcsDL, FcsEL1, FcsEL2, FcsEL3, FcsEL);
        // 
        // 
        // 
        // 
        //     }
        // 
        // 
        //   }


        CArray<T_ELEM_K, T_ELEM_K> arElemKeyList1;
        m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList1);
        int dNoOfElems = arElemKeyList1.GetSize();
        double dTotLength = 0;
        for (int p=0; p<dNoOfElems; p++)
        {
            T_ELEM_K ElemK1 = arElemKeyList1[p];
            dTotLength += m_pDoc->calcLAVElem(ElemK1); 

        }

        m_dTotalLength = dTotLength;


        for(int j=0; j < iForceLcomSize; j++)
        {
            iDgnLcomNo = j+1;
            if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
            {
                ASSERT(0);
            }		


            CString strExecuteMsg;
            strExecuteMsg.Format(_LS(IDS_DGN_CPG_LCOM_INFO),ElemK, iOrgLcomNo, iDgnLcomNo);
            GSaveHistoryNF(strExecuteMsg);

            BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(j+1);
            BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(j+1);
            BOOL bFatLcom = m_pForcCtrl->Is_LcomForFati(j+1);
            if(bSLSLcom) {iServLcomType = m_pForcCtrl->Get_ServLcomType(iDgnLcomNo);}
            const _DGN_FORC_CRC& ForceCrc = aForceLcom.GetAt(j);
            const _DGN_STRS_CRC& StressCrc = aStressLcom.GetAt(j);

            //////////////////////////////////////////////////////////////////////////      

            T_PGBR_LCOM PgbrLcomD; PgbrLcomD.Initialize();
            T_PRVS_D PrvsD; PrvsD.Initialize();
            T_PLTB_D PltbD; PltbD.Initialize();      
            T_PRTF_D PrtfD; PrtfD.Initialize();
            T_PGLS_D PglsD; PglsD.Initialize();
            T_PGRF_D PgrfD; PgrfD.Initialize();
            T_PGBS_D PgbsD; PgbsD.Initialize();
            T_PLSS_D PlssD; PlssD.Initialize();

            // I-End
            _DGN_FORC_CRC MembEndForce;
            double dM1=0.0, dV1=0.0, dM2=0.0, dV2=0.0;
            double dIdispZ=0.0, dJdispZ=0.0;
            if(bULSLcom && bCheckStrength[2] && !bIsGenSect)
            {
                m_pForcCtrl->Get_LcomDataForDesign_LcomK(IendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);
                dM1 = MembEndForce.dMuy[0];
                dV1 = MembEndForce.dFzz[0];
                //
                MembEndForce.Initialize();
                m_pForcCtrl->Get_LcomDataForDesign_LcomK(JendElemK, iDgnLcomNo, TRUE, FALSE, MembEndForce);
                dM2 = MembEndForce.dMuy[1];
                dV2 = MembEndForce.dFzz[1];
                // Get, C4 
                ArrElemPairKey aMembElemPairK;
                CDBLib::ConvertToElemPairKey(EN_EL_BEAM, arMembElemList, aMembElemPairK);
                if(m_pDataCtrl->Get_C4valueForMomentType(iDgnLcomNo, showElemK, aMembElemPairK, dC4)) {}
                else {dC4=0.0;}

                // Get Disp.

                m_pDataCtrl->Get_Deflection4NodeOfElem(ElemK, iDgnLcomNo, dIdispZ, dJdispZ, ENUM_KS05);



            }

            // Jaeoh. Modification. 이대근 부장님 요청 
            CPG_FORC_STAG ForceStage;      ForceStage.Initialize();
            if(!m_pDataCtrl->Get_GirderCSLoadOfLCOM(StctD, ElemK, nGdLastStagK, nLastStepNo, iDgnLcomNo, ForceStage)) {ASSERT(0);}

            for(int k=0; k < 2; k++)	// Repeat I-End & J-End.
            {		    
                if(!bChkIJ[k]) {continue;}

                BOOL bPosiI = k==0 ? TRUE : FALSE;
                m_ClassBase.Initialize();
                GLineData.Initialize();
                GLineDgn.Initialize();
                double dFxx = ForceCrc.dFxx[k];
                double dFyy = ForceCrc.dFyy[k];
                double dFzz = ForceCrc.dFzz[k];
                double dMux = ForceCrc.dMux[k];
                double dMuy = ForceCrc.dMuy[k];
                double dMuz = ForceCrc.dMuz[k];
                double dMa_Ed = ForceStage.MaxForce.dMuy[k];
                double aGirderForce[6] = {ForceStage.MaxForce.dFxx[k], ForceStage.MaxForce.dFyy[k], ForceStage.MaxForce.dFzz[k],
                    ForceStage.MaxForce.dMux[k], ForceStage.MaxForce.dMuy[k], ForceStage.MaxForce.dMuz[k]};
                m_dPu = dFxx;
                m_dMuy = dMuy;
                m_dMuz = dMuz;
                double aForce[6] = {dFxx, dFyy, dFzz, dMux, dMuy, dMuz};
                double aCptForce[6]={0.0};
                for(int l = 0 ; l < 6; ++l){aCptForce[l] = aForce[l] - aGirderForce[l];}

                double aStress[4]={StressCrc.dStl[k], StressCrc.dStr[k], StressCrc.dSbr[k], StressCrc.dSbl[k]};

                if(dMuy > -m_dZero) // 정모멘트 인데 Stage 모멘트가 부모멘트면 0으로..
                {
                    if(dMa_Ed<0.0) {dMa_Ed = 0.0;}
                }
                else
                {
                    if(dMa_Ed>0.0) {dMa_Ed = 0.0;}
                }
                UINT nPosi = (k==0 ? 1 : 2);      
                T_PGBR_BASE PgbrGd, PgbrBase; 
                T_PRVS_BASE PrvsGd, PrvsBase; 
                T_PLTB_BASE PltbGd, PltbBase; 
                T_PRTF_BASE PrtfBase; 
                T_PGLS_BASE PglsGd, PglsBase; 
                T_PGRF_BASE PgrfBase; 
                T_PGBS_BASE PgbsBase; 
                T_PLSS_BASE PlssGd, PlssBase; 

                if(bIsGenSect)
                {
                    GLineDgn = SectElem.GLineDgn[k];
                    Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
                }

                // RBar
                CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
                arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);  

                // Longitudinal Stiffener  
                CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
                arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);

                // Shear Connector.
                CPG_SCON_POSI SconPosi = SconElem.SconPosi[k];

                // Transverse Stiffener
                CPG_TRST_POSI TrstPosi = TrstElem.TrstPosi[k];

                // End Support
                CPG_TSES_POSI TsesPosi = TsesElem.TsesPosi[k];    

                // Later-torsional buckling data
                CPG_LTBD_POSI LtbdPosi = LtbdElem.LtbdPosi[k];

                // Member data, Lu
                CPG_MEMB_POSI MembPosi = MembElem.MembPosi[k];

                // Buckling coefficients for different types of load application.
                CPG_BCDT_POSI BcdtPosi = BcdtElem.BcdtPosi[k]; 

                // Lamda_v2, Lamda_v4
                CPG_FRDT_POSI FrdtPosi = FrdtElem.FrdtPosi[k];

                //Pinakin added for effective width of slab IRC

                if(bissimplysup) dIRCL = m_dTotalLength;//MembPosi.dLu;

                if (i==0 || i==iTotalElement) bOuter =TRUE;

                _DGN_FORC_CRC ForceLcase;
                _DGN_STRS_ADD StressAdd;
                _DGN_STRS_CRC StressLcase;
                _DGN_SHRS_ADD StssLcase;

                _DGN_FORC_CRC ForcePart1;
                _DGN_STRS_CRC StressPart1;
                _DGN_FORC_CRC ForcePart2;
                _DGN_STRS_CRC StressPart2;

                m_pForcCtrl->Get_Stld(ElemK, iDgnLcomNo, 0, FALSE, ForceLcase, StressAdd, StressLcase, StssLcase,
                    ForcePart1, StressPart1, ForcePart2, StressPart2);
                m_dpgls_shear =ForceLcase.dFzz[0];
                // 6. Get Section Class
                T_CLAS_BASE ClassD; ClassD.Initialize(); m_DLineCalcD.Initialize();        
                if(!Get_ClassifyClassSection(bPosiI, aGirderForce, aCptForce, MatlElem, SectElem, arRbarPosi, SconElem, 
                    GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK)){ASSERT(0);}      
                m_ClassBase = ClassD;
                //
                SetPgbrBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PgbrGd, PgbrBase);        

                // 7. Flexure Resistance (Bending)
                T_PGBR_DETAIL PgbrDetail; 
                if(!bIsGenSect) {Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, PgbrGd, PgbrBase, PgbrDetail);	    }
                else            {Calc_BendingResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, TrstPosi, ClassD, GLineData, GLineDgn, PgbrGd, PgbrBase, PgbrDetail);}
                PgbrLcomD.BeforePgbr[k] = PgbrGd;  
                PgbrLcomD.PgbrBase[k]	= PgbrBase;  
                m_PgbrBase				= PgbrBase;
                m_PgbrDetail			= PgbrDetail;

                T_RHOC_BASE Rho_C;
                SetRhocBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, PgbrDetail, Rho_C);

                if(bULSLcom) // Get Critical Flexure Design Result
                {
                    if(PgbrBase.dMy > -m_dZero)
                    {
                        if(iPosiCount[k]==0)
                        { 
                            dPosiMaxRatio[k] = PgbrBase.dRatio;
                            PgbrPosiRat.BeforePgbr[k] = PgbrGd; 
                            PgbrPosiRat.PgbrBase[k] = PgbrBase; 
                            RhocPosiRat.Rho_C[k] = Rho_C;
                        }
                        else
                        {
                            if(dPosiMaxRatio[k] < PgbrBase.dRatio)
                            { 
                                dPosiMaxRatio[k] = PgbrBase.dRatio;
                                PgbrPosiRat.BeforePgbr[k] = PgbrGd; 
                                PgbrPosiRat.PgbrBase[k] = PgbrBase; 
                                RhocPosiRat.Rho_C[k] = Rho_C;
                            }
                        }
                        iPosiCount[k]++;
                    }
                    else
                    {
                        if(iNegaCount[k]==0) 
                        { 
                            dNegaMaxRatio[k] = PgbrBase.dRatio;
                            PgbrNegaRat.BeforePgbr[k] = PgbrGd; 
                            PgbrNegaRat.PgbrBase[k] = PgbrBase; 
                            RhocNegaRat.Rho_C[k] = Rho_C;
                        }
                        else
                        {
                            if(dNegaMaxRatio[k] < PgbrBase.dRatio)
                            { 
                                dNegaMaxRatio[k] = PgbrBase.dRatio;
                                PgbrNegaRat.BeforePgbr[k] = PgbrGd; 
                                PgbrNegaRat.PgbrBase[k] = PgbrBase; 
                                RhocNegaRat.Rho_C[k] = Rho_C;
                            }
                        }
                        iNegaCount[k]++;
                    }
                }

                // 8. Vertical Shear Resistance
                if(bULSLcom && bCheckStrength[1])
                {
                    T_PRVS_DETAIL PrvsDetail; 
                    SetPrvsBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PrvsGd, PrvsBase);
                    if(!bIsGenSect) {Calc_VerticalShearResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, PrvsGd, PrvsBase, PrvsDetail,PgbrBase);				  }
                    else            {Calc_VerticalShearResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arLstiPosi, TrstPosi, TsesPosi, ClassD, GLineData, GLineDgn, PrvsGd, PrvsBase, PrvsDetail);}
                    PrvsD.BeforePrvs[k] = PrvsGd;
                    PrvsD.PrvsBase[k] = PrvsBase;

                    if(iVCount[k]==0)
                    { 
                        dVMaxRatio[k] = PrvsBase.dRatio; 
                        PrvsRat.BeforePrvs[k] = PrvsGd; 
                        PrvsRat.PrvsBase[k] = PrvsBase; 
                    }
                    else
                    {
                        if(dVMaxRatio[k] < PrvsBase.dRatio) 
                        {
                            dVMaxRatio[k]=PrvsBase.dRatio;
                            PrvsRat.BeforePrvs[k] = PrvsGd; 
                            PrvsRat.PrvsBase[k] = PrvsBase; 
                        }
                    }
                    iVCount[k]++;
                }

                // 9. Lateral Torsional Buckling
                if(bULSLcom && !bIsGenSect && bCheckStrength[2])
                {
                    T_PLTB_DETAIL PltbDetail; 
                    SetPltbBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, dV1, dV2, dM1, dM2, dC4, PltbGd, PltbBase);
                    Calc_LateralTorsionalBucklingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, LtbdPosi, MembPosi, ClassD, PltbBase, PltbDetail);
                    PltbD.BeforePltb[k] = PltbGd;
                    PltbD.PltbBase[k] = PltbBase;

                    if(iLTBCount[k]==0) 
                    { 
                        dLTBMaxRatio[k] = PltbBase.dRatio; 
                        PltbRat.BeforePltb[k] = PltbGd; 
                        PltbRat.PltbBase[k] = PltbBase; 
                    }
                    else
                    {
                        if(dLTBMaxRatio[k]<PltbBase.dRatio)
                        {
                            dLTBMaxRatio[k]=PltbBase.dRatio; 
                            PltbRat.BeforePltb[k] = PltbGd; 
                            PltbRat.PltbBase[k] = PltbBase; 
                        }
                    }
                    iLTBCount[k]++;
                }

                // 10. Transverse Force Resistance
                if(bULSLcom && !bIsGenSect && bCheckStrength[3])
                {
                    SetPrtfBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aForce, PrtfBase);
                    // Modify, Jaeoh. MQC. 3797, [1/21/2011]
                    BOOL bCalcPrtf = FALSE;
                    if(PrtfBase.bChk)     
                    {
                        if(!bBuckCoeff || (bBuckCoeff && !BcdtPosi.bUseTfType)) 
                        {
                            PrtfBase.bChk = FALSE;
                            bCalcPrtf = FALSE;
                        }
                        else bCalcPrtf = TRUE;            
                    }
                    else bCalcPrtf = FALSE;

                    if(bCalcPrtf)
                    {
                        T_PRTF_DETAIL PrtfDetail; 
                        Calc_TransverseForceResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, BcdtPosi, ClassD, PrtfBase, PrtfDetail,SconPosi);
                        PrtfD.PrtfBase[k] = PrtfBase;
                        if(iRTFCount[k]==0) { dRTFMaxRatio[k] = PrtfBase.dRatio; PrtfRat.PrtfBase[k] = PrtfBase; }
                        else
                        {
                            if(dRTFMaxRatio[k]<PrtfBase.dRatio) { dRTFMaxRatio[k]=PrtfBase.dRatio;  PrtfRat.PrtfBase[k] = PrtfBase; }
                        }
                        iRTFCount[k]++;
                    }
                }

                // 11. Longitudinal Shear Resistance
                T_PGLS_DETAIL PglsDetail;
                BOOL bCalcPgls = FALSE;
                if(bULSLcom && bCheckStrength[4])
                {
                    SetPglsBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PglsGd, PglsBase);

                    if (SconPosi.dDia>0.0)
                    {
                        bCalcPgls = TRUE;
                        if(!bIsGenSect) {Calc_LongitudinalShearResistance(ElemK, bPosiI, bSLSLcom, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, PglsGd, PglsBase, PglsDetail);		  }
                        else            {Calc_LongitudinalShearResistance4GenSect(ElemK, bPosiI, bSLSLcom, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, PglsGd, PglsBase, PglsDetail);}
                        PglsD.BeforePgls[k] = PglsGd;
                        PglsD.PglsBase[k] = PglsBase;
                        m_PglsBase = PglsBase;
                        m_dpgls_shear =ForceLcase.dFzz[0];
                        // Modify by GAY. PMS:4216. ('12.05.17). 합성전 거더의 설계결과 할당 누락 수정.
                        if(iLSCount[k]==0) { dLSMaxRatio[k] = PglsBase.dRatio; PglsRat.BeforePgls[k] = PglsGd; PglsRat.PglsBase[k] = PglsBase; }
                        else
                        {
                            if(dLSMaxRatio[k]<PglsBase.dRatio) { dLSMaxRatio[k]=PglsBase.dRatio; PglsRat.BeforePgls[k] = PglsGd;  PglsRat.PglsBase[k] = PglsBase; }
                        }

                    }
                    else bCalcPgls = FALSE;

                    iLSCount[k]++;
                }

                // 12. Fatigue Resistance
                if(bFatLcom && !bIsGenSect && bCheckStrength[5])
                {
                    // Fatigue Resistance를 단독으로 사용할 경우 Longitudinal Shear Resistance가 먼저 선행되어야 하기 때문에..
                    if(!bIsGenSect && !bCheckStrength[4])
                    {
                        SetPglsBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aGirderForce, aCptForce, PglsGd, PglsBase);
                        Calc_LongitudinalShearResistance(ElemK, bPosiI, bSLSLcom, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, PglsGd, PglsBase, PglsDetail);
                        m_PglsBase = PglsBase;
                    }

                    T_PGRF_DETAIL PgrfDetail; 
                    SetPgfrBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, aForce, PgrfBase);
                    Calc_FatigueResistance(ElemK, bPosiI, dt_Ld, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, FrdtPosi, ClassD, PgrfBase, PgrfDetail);
                    PgrfD.PgrfBase[k] = PgrfBase;
                    if(iRFCount[k]==0) { dRFMaxRatio[k] = PgrfBase.dRatio; PgrfRat.PgrfBase[k] = PgrfBase; }
                    else
                    {
                        if(dRFMaxRatio[k]<PgrfBase.dRatio) { dRFMaxRatio[k]=PgrfBase.dRatio;  PgrfRat.PgrfBase[k] = PgrfBase; }
                    }
                    iRFCount[k]++;
                }

                // 13. Check Stress
                if(bSLSLcom && bCheckService[0])
                {
                    T_PGBS_DETAIL PgbsDetail; 
                    SetPgbsBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PgbsBase);
                    Calc_StressCheck4SLS(ElemK, bPosiI, bIsGenSect, MatlElem, SectElem, arRbarPosi, GLineData, GLineDgn, dk1, dk2, dk3, PgbsBase, PgbsDetail);
                    PgbsD.PgbsBase[k] = PgbsBase;
                    PgbsRat.PgbsBase[k].bChk = bChkIJ[k];
                    PgbsRat.PgbsBase[k].ElemK = ElemK.first;
                    PgbsRat.PgbsBase[k].nPosi = nPosi;
                    // Girder
                    if(iServLcomType==3) // Characteristic
                    {
                        if(iStressGrSLS_Count[k]==0) 
                        { 
                            dStressGrMaxRatio[k] = PgbsBase.ChkGirder.dMaxRatio;
                            PgbsRat.PgbsBase[k].ChkGirder = PgbsBase.ChkGirder; //PgbsGrCrD = PgbsBase.ChkGirder;
                        }
                        else
                        {
                            if(dStressGrMaxRatio[k]<PgbsBase.ChkGirder.dMaxRatio)
                            {
                                dStressGrMaxRatio[k] = PgbsBase.ChkGirder.dMaxRatio;              
                                PgbsRat.PgbsBase[k].ChkGirder = PgbsBase.ChkGirder; //PgbsGrCrD = PgbsBase.ChkGirder;
                            }
                        }
                        iStressGrSLS_Count[k]++;
                    }
                    // Slab
                    if(iServLcomType==1 || iServLcomType==3) // 1=Quasi, 3=Characteristic
                    {
                        if(iStressSbSLS_Count[k]==0) 
                        { 
                            dStressSbMaxRatio[k] = PgbsBase.ChkSlab.dMaxRatio_sb;  
                            PgbsRat.PgbsBase[k].ChkSlab = PgbsBase.ChkSlab; //PgbsSbCrD = PgbsBase.ChkSlab;
                        }
                        else
                        {
                            if(dStressSbMaxRatio[k]<PgbsBase.ChkSlab.dMaxRatio_sb)
                            {
                                dStressSbMaxRatio[k] = PgbsBase.ChkSlab.dMaxRatio_sb;
                                PgbsRat.PgbsBase[k].ChkSlab = PgbsBase.ChkSlab; //PgbsSbCrD = PgbsBase.ChkSlab;
                            }
                        }
                        iStressSbSLS_Count[k]++;
                    }
                    // Rebar
                    if(iStressBarSLS_Count[k]==0) 
                    { 
                        dStressBarMaxRatio[k] = PgbsBase.ChkBar.dMaxRatio_bar;  
                        PgbsRat.PgbsBase[k].ChkBar = PgbsBase.ChkBar; //PgbsBarCrD = PgbsBase.ChkBar;
                    }
                    else
                    {
                        if(dStressBarMaxRatio[k]<PgbsBase.ChkBar.dMaxRatio_bar)
                        {
                            dStressBarMaxRatio[k] = PgbsBase.ChkBar.dMaxRatio_bar;
                            PgbsRat.PgbsBase[k].ChkBar = PgbsBase.ChkBar; //PgbsBarCrD = PgbsBase.ChkBar;
                        }
                    }
                    iStressBarSLS_Count[k]++;
                    //
                    BOOL bChkGirder=TRUE, bChkSlab=TRUE, bChkRebar=TRUE;
                    if(PgbsRat.PgbsBase[k].ChkGirder.LcomK!=0 && !PgbsRat.PgbsBase[k].ChkGirder.bOK)			{bChkGirder=FALSE; }
                    if(PgbsRat.PgbsBase[k].ChkSlab.LcomK!=0 && !PgbsRat.PgbsBase[k].ChkSlab.bChk_Stress_sb)	{bChkSlab = FALSE; }
                    if(PgbsRat.PgbsBase[k].ChkBar.LcomK!=0 && !PgbsRat.PgbsBase[k].ChkBar.bChk_Stress_bar)	{bChkRebar = FALSE;}

                    if(bChkGirder && bChkSlab && bChkRebar) {PgbsRat.PgbsBase[k].bOK = TRUE;}
                    else {PgbsRat.PgbsBase[k].bOK = FALSE;}

                }

                // 14. Check Longitudianl Shear for SLS
                if(bSLSLcom && bCheckService[1])
                {
                    T_PLSS_DETAIL PlssDetail; 
                    SetPlssBaseData(bChkIJ[k], ElemK, nPosi, iDgnLcomNo, iOrgLcomNo, iMaxMinType, iServLcomType, aGirderForce, aCptForce, PlssGd, PlssBase);
                    Calc_LongitudinalShearResistance4SLS(ElemK, bPosiI, bIsGenSect, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, PlssGd, PlssBase, PlssDetail);
                    PlssD.BeforePlss[k] = PlssGd;
                    PlssD.PlssBase[k] = PlssBase;          
                    // Modify by GAY. PMS:4216. ('12.05.17). 합성전 거더의 설계결과 할당 누락 수정.
                    if(iLS_SLS_Count[k]==0) { dLS_SLSMaxRatio[k] = PlssBase.dRatio; PlssRat.BeforePlss[k] = PlssGd;  PlssRat.PlssBase[k] = PlssBase; }
                    else
                    {
                        if(dLS_SLSMaxRatio[k]<PlssBase.dRatio) { dLS_SLSMaxRatio[k]=PlssBase.dRatio; PlssRat.BeforePlss[k] = PlssGd;  PlssRat.PlssBase[k] = PlssBase; }
                    }
                    iLS_SLS_Count[k]++;
                }


            }

            // 15. Save Design Result for Each Design Lcom
            if(bULSLcom)
            {
                // Bending Resistance
                // if(bCheckStrength[0]) Bending은 다른 검토에서도 필요하기 때문에 결과를 쓴다. 
                if(!pPGD->WriteBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrLcomD)){ASSERT(0);}

                // Vertical Shear Resistance
                if(bCheckStrength[1]) {if(!pPGD->WriteVerticalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PrvsD)){ASSERT(0);}}
                // Lateral Torsional Buckling
                if(bCheckStrength[2]) {if(!pPGD->WriteLateralTorsionalBucklingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PltbD)){ASSERT(0);}}
                // Transverse Force Resistance
                if(bCheckStrength[3]) {if(!pPGD->WriteTransverseForceResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PrtfD)){ASSERT(0);}}
                // Longitudinal Shear Resistance
                if(bCheckStrength[4]) {if(!pPGD->WriteLongitudinalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PglsD)){ASSERT(0);}}
                // Fatigue Resistance
                if(bCheckStrength[5]) {if(!pPGD->WriteFatigueResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgrfD)){ASSERT(0);}}
            }

            if(bSLSLcom)
            {
                if(bCheckService[0]) {if(!pPGD->WriteBeamStressResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbsD)){ASSERT(0);}}
                //
                if(bCheckService[1]) {if(!pPGD->WriteLongitudinalShearResistanceServiceResult(ElemK, iDgnLcomNo, iMaxMin, PlssD)){ASSERT(0);}}
            }

        }
        // Critical for each Element
        iDgnLcomNo=0;

        // Positive, Min Ratio
        iMaxMin=1;
        if(!pPGD->WriteBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrPosiRat)){ASSERT(0);}
        if(!pPGD->WriteBendingRhocResult(ElemK, iDgnLcomNo, iMaxMin, RhocPosiRat)){ASSERT(0);}

        // Negative, Min Ratio
        iMaxMin=2;
        if(!pPGD->WriteBendingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbrNegaRat)){ASSERT(0);}
        if(!pPGD->WriteBendingRhocResult(ElemK, iDgnLcomNo, iMaxMin, RhocNegaRat)){ASSERT(0);}

        iMaxMin=1;
        if(bCheckStrength[1]) {if(!pPGD->WriteVerticalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PrvsRat)){ASSERT(0);}}
        if(bCheckStrength[2]) {if(!pPGD->WriteLateralTorsionalBucklingResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PltbRat)){ASSERT(0);}}
        if(bCheckStrength[3]) {if(!pPGD->WriteTransverseForceResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PrtfRat)){ASSERT(0);}}
        if(bCheckStrength[4]) {if(!pPGD->WriteLongitudinalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PglsRat)){ASSERT(0);}}
        if(bCheckStrength[5]) {if(!pPGD->WriteFatigueResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgrfRat)){ASSERT(0);}}
        if(bCheckService[0]) {if(!pPGD->WriteBeamStressResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PgbsRat)){ASSERT(0);}}
        if(bCheckService[1]) {if(!pPGD->WriteLongitudinalShearResistanceServiceResult(ElemK, iDgnLcomNo, iMaxMin, PlssRat)){ASSERT(0);}}
    }  

    return TRUE;
}


BOOL CDgnPlateGirder_IRC::Get_BendingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBR_BASE& PgbrGd,
                                                                        const T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
    //
    CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
    //
    CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
    //
    CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
    if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE;} 
    //
    CPG_BCDT_ELEM BcdtElem;  BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
    //
    CPG_FRDT_ELEM FrdtElem;  FrdtElem.Initialize();
    if(Get_CpgFrdtElem(ElemK, FrdtElem)) {bLamdaData=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) ASSERT(0);

    BOOL bIsGenSect=SectElem.bGenSect;
    m_dEs		= MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
    int nFortype = 0;
    ADGNFORCE aForceLcom;
    if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{return FALSE;}
    if(m_pForcCtrl->Get_LcomNumForStrn()==0)	{return FALSE;}

    ADGNSTRESS aStressLcom;
    ADGNSTRSADD aStressAdd;    
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum)) {return FALSE;}

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1]);

    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");
    m_ClassBase.Initialize();
    T_GENL_DATA GLineData; GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn; GLineDgn.Initialize();

    iDgnLcomNo = PgbrBase.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }		    

    int k = bPosiI ? 0 : 1;
    double aForceGd[6] = {0.0};
    double aForce[6] = {0.0};
    aForceGd[0] = PgbrGd.dFx;
    aForceGd[4] = PgbrGd.dMy;
    aForceGd[5] = PgbrGd.dMz;
    aForce[0] = PgbrBase.dFx;
    aForce[4] = PgbrBase.dMy;
    aForce[5] = PgbrBase.dMz;  

    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);
    // Transverse Stiffener
    CPG_TRST_POSI TrstPosi;
    TrstPosi = TrstElem.TrstPosi[k];

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();
    Get_ClassifyClassSection(bPosiI, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    T_PGBR_BASE TempPgbrGd   = PgbrGd;
    T_PGBR_BASE TempPgbrBase = PgbrBase;
    if(!bIsGenSect) {Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, TempPgbrGd, TempPgbrBase, PgbrDetail);		  }
    else            {Calc_BendingResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, TrstPosi, ClassD, GLineData, GLineDgn, TempPgbrGd, TempPgbrBase, PgbrDetail);}  


    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_VerticalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRVS_BASE& PrvsGd, const T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
    //
    CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
    //
    CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
    //
    CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
    if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE;}  
    //
    CPG_BCDT_ELEM BcdtElem;  BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
    //
    CPG_FRDT_ELEM FrdtElem;  FrdtElem.Initialize();
    if(Get_CpgFrdtElem(ElemK, FrdtElem)) {bLamdaData=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) {ASSERT(0);}

    m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    int nFortype =0;
    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
    ADGNFORCE aForceLcom;
    if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{return FALSE;}
    if(m_pForcCtrl->Get_LcomNumForStrn()==0)	return FALSE;

    ADGNSTRESS aStressLcom;
    ADGNSTRSADD aStressAdd;    
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum)) {return FALSE;}

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1]);

    int iMaxMin=0;
    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");

    m_ClassBase.Initialize();
    T_GENL_DATA GLineData; GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn;  GLineDgn.Initialize();

    iDgnLcomNo = PrvsBase.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }		    

    int k = bPosiI ? 0 : 1;
    BOOL bIsGenSect = SectElem.bGenSect;
    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);
    // Transverse Stiffener
    CPG_TRST_POSI TrstPosi = TrstElem.TrstPosi[k];
    // End Support
    CPG_TSES_POSI TsesPosi = TsesElem.TsesPosi[k];    

    // Bending 검토가 선행되어야 한다. 
    T_PGBR_LCOM PgbrLcomD;    
    if(!m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PgbrLcomD)) {return FALSE;}

    T_PGBR_BASE PgbrGd   = PgbrLcomD.BeforePgbr[k];
    T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[k];
    T_PGBR_DETAIL PgbrDetail;

    double aForceGd[6] = {0.0};
    double aForce[6] = {0.0};
    aForceGd[0] = PgbrGd.dFx;
    aForceGd[4] = PgbrGd.dMy;
    aForceGd[5] = PgbrGd.dMz;
    aForce[0] = PgbrBase.dFx;
    aForce[4] = PgbrBase.dMy;
    aForce[5] = PgbrBase.dMz;  

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();
    Get_ClassifyClassSection(bPosiI, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    if(!bIsGenSect) {Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, PgbrGd, PgbrBase, PgbrDetail);		  }
    else            {Calc_BendingResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, TrstPosi, ClassD, GLineData, GLineDgn, PgbrGd, PgbrBase, PgbrDetail);}  
    m_PgbrBase = PgbrBase;
    m_PgbrDetail = PgbrDetail;

    T_PRVS_BASE TempPrvsGd = PrvsGd;
    T_PRVS_BASE TempPrvsBase = PrvsBase;
    if(!bIsGenSect) {Calc_VerticalShearResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, TempPrvsGd, TempPrvsBase, PrvsDetail,PgbrBase);				  }
    else            {Calc_VerticalShearResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arLstiPosi, TrstPosi, TsesPosi, ClassD, GLineData, GLineDgn, TempPrvsGd, TempPrvsBase, PrvsDetail);}

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LateralTorsionalBucklingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail,BOOL bspcl)
{
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
    //
    CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
    //
    CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
    //
    CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
    if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE; } 
    //
    CPG_BCDT_ELEM BcdtElem;  BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
    //
    CPG_FRDT_ELEM FrdtElem;  FrdtElem.Initialize();
    if(Get_CpgFrdtElem(ElemK, FrdtElem)) {bLamdaData=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) {ASSERT(0);}

    m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    int nFortype=0;
    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
    ADGNFORCE aForceLcom;
    if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{return FALSE;}
    if(m_pForcCtrl->Get_LcomNumForStrn()==0)	{return FALSE;}

    ADGNSTRESS aStressLcom;
    ADGNSTRSADD aStressAdd;    
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum)) {return FALSE;}

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1]);

    int iMaxMin=0;
    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");    
    m_ClassBase.Initialize();
    T_GENL_DATA GLineData; GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn;  GLineDgn.Initialize();

    iDgnLcomNo = PltbBase.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }		    
    int k = bPosiI ? 0 : 1;      

    BOOL bIsGenSect = SectElem.bGenSect;
    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);
    // Transverse Stiffener
    CPG_TRST_POSI TrstPosi = TrstElem.TrstPosi[k];
    // End Support
    CPG_TSES_POSI TsesPosi = TsesElem.TsesPosi[k];    
    // Later-torsional buckling data
    CPG_LTBD_POSI LtbdPosi = LtbdElem.LtbdPosi[k];
    // Member data, Lu
    CPG_MEMB_POSI MembPosi = MembElem.MembPosi[k];

    // Bending 검토가 선행되어야 한다. 
    T_PGBR_LCOM PgbrLcomD;    
    if(!m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PgbrLcomD)) {return FALSE;}

    T_PGBR_BASE PgbrGd   = PgbrLcomD.BeforePgbr[k];
    T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[k];

    double aForceGd[6] = {0.0};
    double aForce[6] = {0.0};
    aForceGd[0] = PgbrGd.dFx;
    aForceGd[4] = PgbrGd.dMy;
    aForceGd[5] = PgbrGd.dMz;
    aForce[0] = PgbrBase.dFx;
    aForce[4] = PgbrBase.dMy;
    aForce[5] = PgbrBase.dMz;  

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();
    Get_ClassifyClassSection(bPosiI, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    T_PGBR_DETAIL PgbrDetail;
    if(!bIsGenSect) {Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, PgbrGd, PgbrBase, PgbrDetail);		  }
    else            {Calc_BendingResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, TrstPosi, ClassD, GLineData, GLineDgn, PgbrGd, PgbrBase, PgbrDetail);}  
    m_PgbrBase = PgbrBase;
    m_PgbrDetail = PgbrDetail;
    /*PltbBase.dMa_Ed = m_PgbrBase.dMa_Ed;*/
    T_PLTB_BASE TempPltbBase = PltbBase;  
    if (bspcl)
    {
        TempPltbBase.dMa_Ed = m_PgbrBase.dMa_Ed;
        Calc_LateralTorsionalBucklingResistance_Neg(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, LtbdPosi, MembPosi, ClassD, TempPltbBase, PltbDetail,m_PgbrBase.dMa_Ed);

    }
    else  Calc_LateralTorsionalBucklingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, LtbdPosi, MembPosi, ClassD, TempPltbBase, PltbDetail);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_TransverseForceResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail)
{
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;
    BOOL bScon=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
    //
    CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
    //
    CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
    //
    CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
    if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE;}  
    //
    CPG_BCDT_ELEM BcdtElem;  BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
    //
    CPG_FRDT_ELEM FrdtElem;  FrdtElem.Initialize();
    if(Get_CpgFrdtElem(ElemK, FrdtElem)) {bLamdaData=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) {ASSERT(0);}

    m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1]);

    int iMaxMin=0;
    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");

    m_ClassBase.Initialize();
    T_GENL_DATA GLineData;	GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn;	GLineDgn.Initialize();

    iDgnLcomNo = PrtfBase.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }		    

    int k = bPosiI ? 0 : 1;
    BOOL bIsGenSect=SectElem.bGenSect;
    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);
    // Transverse Stiffener
    CPG_TRST_POSI TrstPosi = TrstElem.TrstPosi[k];
    // End Support
    CPG_TSES_POSI TsesPosi = TsesElem.TsesPosi[k];    
    // Buckling coefficients for different types of load application.
    CPG_BCDT_POSI BcdtPosi = BcdtElem.BcdtPosi[k];

    CPG_SCON_POSI SconPosi;
    SconPosi = SconElem.SconPosi[k] ;
    // Bending 검토가 선행되어야 한다. 
    T_PGBR_LCOM PgbrLcomD;    
    if(!m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PgbrLcomD)) {return FALSE;}

    T_PGBR_BASE PgbrGd   = PgbrLcomD.BeforePgbr[k];
    T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[k];

    double aForceGd[6] = {0.0};
    double aForce[6] = {0.0};
    aForceGd[0] = PgbrGd.dFx;
    aForceGd[4] = PgbrGd.dMy;
    aForceGd[5] = PgbrGd.dMz;
    aForce[0] = PgbrBase.dFx;
    aForce[4] = PgbrBase.dMy;
    aForce[5] = PgbrBase.dMz;  

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();
    Get_ClassifyClassSection(bPosiI, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    T_PGBR_DETAIL PgbrDetail;
    if(!bIsGenSect) {Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, PgbrGd, PgbrBase, PgbrDetail);		  }
    else            {Calc_BendingResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, TrstPosi, ClassD, GLineData, GLineDgn, PgbrGd, PgbrBase, PgbrDetail);}  
    m_PgbrBase = PgbrBase;
    m_PgbrDetail = PgbrDetail;

    T_PRTF_BASE TempPrtfBase = PrtfBase;
    Calc_TransverseForceResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, BcdtPosi, ClassD, TempPrtfBase, PrtfDetail,SconPosi);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LongitudinalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGLS_BASE& PglsGd, const T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
    //
    CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
    //
    CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
    //
    CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
    if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE;}  
    //
    CPG_BCDT_ELEM BcdtElem;  BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) {ASSERT(0);}

    m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    int nFortype=0;
    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
    ADGNFORCE aForceLcom;
    if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{return FALSE;}

    //   CArray<_DGN_LCOM,_DGN_LCOM> aLcomDesignDL;m_pForcCtrl->Get_LcomListDL(aLcomDesignDL);
    //   CArray<_DGN_LCOM,_DGN_LCOM> aLcomDesignLL;m_pForcCtrl->Get_LcomListLL(aLcomDesignLL);

    //   double dp1;
    //   for(int i=0; i<aLcomDesignDL.GetSize(); i++)
    //   {
    //     _DGN_LCOM LcomDgn = aLcomDesignDL.GetAt(i);
    //     if (LcomDgn.OriginalLcomNo == PglsBase.LcomK)
    //     {
    //       dp1 = LcomDgn.DesignLcomKey;
    //     }
    //   }


    ADGNSTRESS aStressLcom;
    ADGNSTRSADD aStressAdd;    
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum)) {return FALSE;}

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1]);

    int iMaxMin=0;
    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");

    m_ClassBase.Initialize();
    T_GENL_DATA GLineData;	GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn;	GLineDgn.Initialize();

    iDgnLcomNo = PglsBase.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }	
    _DGN_FORC_CRC_EX ForcLcom; ForcLcom.Initialize();
    //Pinakin add


    //Pinakin end
    T_PRVS_D PrvsD;PrvsD.Initialize();
    if(!m_pAnalysisResult->GetCpgVerticalShearResistanceResult(ElemK, iDgnLcomNo, iMaxMin, PrvsD))
    {
        return FALSE;
    }


    int k = bPosiI ? 0 : 1;
    BOOL bIsGenSect = SectElem.bGenSect;
    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);
    // Shear Connector.
    CPG_SCON_POSI SconPosi = SconElem.SconPosi[k];
    // Transverse Stiffener
    CPG_TRST_POSI TrstPosi = TrstElem.TrstPosi[k];
    // End Support
    CPG_TSES_POSI TsesPosi = TsesElem.TsesPosi[k];    

    // Bending 검토가 선행되어야 한다. 
    T_PGBR_LCOM PgbrLcomD;    
    if(!m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PgbrLcomD)) {return FALSE;}

    T_PGBR_BASE PgbrGd   = PgbrLcomD.BeforePgbr[k];
    T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[k];

    double aForceGd[6] = {0.0};
    double aForce[6] = {0.0};
    aForceGd[0] = PgbrGd.dFx;
    aForceGd[4] = PgbrGd.dMy;
    aForceGd[5] = PgbrGd.dMz;
    aForce[0] = PgbrBase.dFx;
    aForce[4] = PgbrBase.dMy;
    aForce[5] = PgbrBase.dMz;  

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();
    Get_ClassifyClassSection(bPosiI, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    T_PGBR_DETAIL PgbrDetail;
    if(!bIsGenSect) {Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, PgbrGd, PgbrBase, PgbrDetail);		  }
    else            {Calc_BendingResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, TrstPosi, ClassD, GLineData, GLineDgn, PgbrGd, PgbrBase, PgbrDetail);}  
    m_PgbrBase = PgbrBase;
    m_PgbrDetail = PgbrDetail;

    _DGN_FORC_CRC ForceLcase;
    _DGN_STRS_ADD StressAdd;
    _DGN_STRS_CRC StressLcase;
    _DGN_SHRS_ADD StssLcase;

    _DGN_FORC_CRC ForcePart1;
    _DGN_STRS_CRC StressPart1;
    _DGN_FORC_CRC ForcePart2;
    _DGN_STRS_CRC StressPart2;

    m_pForcCtrl->Get_Stld(ElemK, iDgnLcomNo, 0, FALSE, ForceLcase, StressAdd, StressLcase, StssLcase,
        ForcePart1, StressPart1, ForcePart2, StressPart2);
    m_dpgls_shear =ForceLcase.dFzz[0];

    T_PGLS_BASE TempPglsGd = PglsGd;
    T_PGLS_BASE TempPglsBase = PglsBase;
    TempPglsBase.dv_L_Ed2  = m_dpgls_shear; 
    BOOL bSLS = TRUE;
    double dks = m_CpgdD.dks;
    if(!bIsGenSect) {Calc_LongitudinalShearResistance(ElemK, bPosiI, bSLS, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, TempPglsGd, TempPglsBase, PglsDetail);		  }
    else            {Calc_LongitudinalShearResistance4GenSect(ElemK, bPosiI, bSLS, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, TempPglsGd, TempPglsBase, PglsDetail);}

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_FatigueResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc); 

    T_CPGD_D CpgdD; CpgdD.Initialize();  
    if(!pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD)){ASSERT(0);}
    double dt_Ld = CpgdD.dt_Ld;  

    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
    //
    CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
    //
    CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
    //
    CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
    if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE;}  
    //
    CPG_BCDT_ELEM BcdtElem;  BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
    //
    CPG_FRDT_ELEM FrdtElem;  FrdtElem.Initialize();
    if(Get_CpgFrdtElem(ElemK, FrdtElem)) {bLamdaData=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) {ASSERT(0);}

    m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    int nFortype= 0;
    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
    ADGNFORCE aForceLcom;
    if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{return FALSE;}
    if(m_pForcCtrl->Get_LcomNumForStrn()==0)	{return FALSE;}

    ADGNSTRESS aStressLcom;
    ADGNSTRSADD aStressAdd;    
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum)) {return FALSE;}

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1]);

    int iMaxMin=0;
    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");

    m_ClassBase.Initialize();
    T_GENL_DATA GLineData;	GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn; GLineDgn.Initialize();

    iDgnLcomNo = PgrfBase.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }		

    int k = bPosiI ? 0 : 1;
    BOOL bIsGenSect=SectElem.bGenSect;

    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);
    // Shear Connector.
    CPG_SCON_POSI SconPosi = SconElem.SconPosi[k];
    // Transverse Stiffener
    CPG_TRST_POSI TrstPosi = TrstElem.TrstPosi[k];
    // End Support
    CPG_TSES_POSI TsesPosi = TsesElem.TsesPosi[k]; 
    // Lamda_v2, Lamda_v4
    CPG_FRDT_POSI FrdtPosi = FrdtElem.FrdtPosi[k];

    // Bending 검토가 선행되어야 한다. 
    T_PGBR_LCOM PgbrLcomD;    
    if(!m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PgbrLcomD)) {return FALSE;}

    T_PGBR_BASE PgbrGd   = PgbrLcomD.BeforePgbr[k];
    T_PGBR_BASE PgbrBase = PgbrLcomD.PgbrBase[k];

    double aForceGd[6] = {0.0};
    double aForce[6] = {0.0};
    aForceGd[0] = PgbrGd.dFx;
    aForceGd[4] = PgbrGd.dMy;
    aForceGd[5] = PgbrGd.dMz;
    aForce[0] = PgbrBase.dFx;
    aForce[4] = PgbrBase.dMy;
    aForce[5] = PgbrBase.dMz;  

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();
    Get_ClassifyClassSection(bPosiI, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    T_PGBR_DETAIL PgbrDetail;
    if(!bIsGenSect) {Calc_BendingResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, ClassD, PgbrGd, PgbrBase, PgbrDetail);		  }
    else            {Calc_BendingResistance4GenSect(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, TrstPosi, ClassD, GLineData, GLineDgn, PgbrGd, PgbrBase, PgbrDetail);}  
    m_PgbrBase = PgbrBase;
    m_PgbrDetail = PgbrDetail;

    T_PGLS_D PglsD;
    T_PGLS_BASE PglsGd;
    T_PGLS_BASE PglsBase;  
    T_PGLS_DETAIL PglsDetail;
    BOOL bSLS = TRUE;
    double dks = m_CpgdD.dks;
    if(m_pAnalysisResult->GetCpgLongitudinalShearResistanceResult(ElemK,iDgnLcomNo,iMaxMin, PglsD))
    {
        m_PglsBase = PglsD.PglsBase[k];
    }
    else
    {
        if(!bIsGenSect) Calc_LongitudinalShearResistance(ElemK, bPosiI, bSLS, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, 
            TrstPosi, TsesPosi, ClassD, dks, PglsGd, PglsBase, PglsDetail);
        m_PglsBase = PglsBase;
    }  

    T_PGRF_BASE TempPgrfBase = PgrfBase;
    Calc_FatigueResistance(ElemK, bPosiI, dt_Ld, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, FrdtPosi, 
        ClassD, TempPgrfBase, PgrfDetail);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_BeamStressDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail)
{
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) {ASSERT(0);}

    m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    int nFortype =0;
    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
    ADGNFORCE aForceLcom;
    if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{return FALSE;}
    if(m_pForcCtrl->Get_LcomNumForStrs()==0)	{return FALSE;}

    ADGNSTRESS aStressLcom;
    ADGNSTRSADD aStressAdd;    
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum)) {return FALSE;}

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    memset(m_aY_sb,0,sizeof(m_aY_sb));
    memset(m_aZ_sb,0,sizeof(m_aZ_sb));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0], m_aY_sb[0], m_aZ_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1], m_aY_sb[1], m_aZ_sb[1]);

    int iMaxMin=0;
    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");

    m_ClassBase.Initialize();
    T_GENL_DATA GLineData;	GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn;	GLineDgn.Initialize();

    iDgnLcomNo = PgbsBase.ChkGirder.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }		  
    const _DGN_FORC_CRC& ForceCrc = aForceLcom[iDgnLcomNo-1];
    const _DGN_STRS_CRC& StressCrc = aStressLcom[iDgnLcomNo-1];

    int k = bPosiI ? 0 : 1;
    double dMuy	= ForceCrc.dMuy[k];
    double dMa_Ed = ForceStage.MaxForce.dMuy[k];
    double aForce_Gr[6] = {ForceStage.MaxForce.dFxx[k], ForceStage.MaxForce.dFyy[k], ForceStage.MaxForce.dFzz[k], 
        ForceStage.MaxForce.dMux[k], ForceStage.MaxForce.dMuy[k], ForceStage.MaxForce.dMuz[k]};
    double aForce[6] = {ForceCrc.dFxx[k], ForceCrc.dFyy[k], ForceCrc.dFzz[k], 
        ForceCrc.dMux[k], ForceCrc.dMuy[k], ForceCrc.dMuz[k]};
    double aStress[4]={StressCrc.dStl[k],  StressCrc.dStr[k],  StressCrc.dSbr[k],  StressCrc.dSbl[k]};

    if(dMuy > -m_dZero) // 정모멘트 인데 Stage 모멘트가 부모멘트면 0으로..
    {
        if(dMa_Ed<0.0) {dMa_Ed = 0.0;}
    }
    else
    {
        if(dMa_Ed>0.0) {dMa_Ed = 0.0;}
    }

    BOOL bIsGenSect = SectElem.bGenSect;
    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();
    Get_ClassifyClassSection(bPosiI, aForce_Gr, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    T_PGBS_BASE TempPgbsBase = PgbsBase;
    Calc_StressCheck4SLS(ElemK, bPosiI, bIsGenSect, MatlElem, SectElem, arRbarPosi, GLineData, GLineDgn, m_CpgdD.dk1, m_CpgdD.dk2, m_CpgdD.dk3, TempPgbsBase, PgbsDetail);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LongitudinalShearResistanceServiceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLSS_BASE& PlssGd, const T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail)
{
    BOOL bRebar=FALSE;
    BOOL bShearConnector=FALSE;
    BOOL bLongStiff=FALSE;
    BOOL bTranStiff=FALSE;
    BOOL bEndSupport=FALSE;
    BOOL bLtbData=FALSE;
    BOOL bMembData=FALSE;
    BOOL bBuckCoeff=FALSE;
    BOOL bLamdaData=FALSE;

    CPG_MATL_ELEM MatlElem; MatlElem.Initialize();  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}
    //
    CPG_SECT_ELEM SectElem; SectElem.Initialize();
    if(Get_CpgSectElem(ElemK, SectElem)) {}    
    //
    CPG_RBAR_ELEM RbarElem; RbarElem.Initialize();
    if(Get_CpgRbarElem(ElemK, RbarElem)) {bRebar=TRUE;}
    //
    CPG_SCON_ELEM SconElem; SconElem.Initialize();
    if(Get_CpgSconElem(ElemK, SconElem)) {bShearConnector=TRUE;}
    //
    CPG_LSTI_ELEM LstiElem; LstiElem.Initialize();    
    if(Get_CpgLstiElem(ElemK, LstiElem)) {bLongStiff=TRUE;}
    //
    CPG_TRST_ELEM TrstElem; TrstElem.Initialize();    
    if(Get_CpgTrstElem(ElemK, TrstElem)) {bTranStiff=TRUE;}
    //
    CPG_TSES_ELEM TsesElem; TsesElem.Initialize();    
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bEndSupport=TRUE;}
    //
    CPG_LTBD_ELEM LtbdElem; LtbdElem.Initialize();    
    if(Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbData=TRUE;}
    //
    CPG_MEMB_ELEM MembElem; MembElem.Initialize();    
    if(Get_CpgMembElem(ElemK, MembElem)) {bMembData=TRUE; } 
    //
    CPG_BCDT_ELEM BcdtElem;  BcdtElem.Initialize();
    if(Get_CpgBcdtElem(ElemK, BcdtElem)) {bBuckCoeff=TRUE;}
    //
    CPG_FRDT_ELEM FrdtElem;  FrdtElem.Initialize();
    if(Get_CpgFrdtElem(ElemK, FrdtElem)) {bLamdaData=TRUE;}
    //
    CPG_FORC_STAG ForceStage;  ForceStage.Initialize();
    if(!m_pDataCtrl->Get_StageForce4CPG(ElemK, ForceStage, TRUE)) {ASSERT(0);}

    m_dEs = MatlElem.MatdD.Data1.Analysis.Elast;
    m_dPoisson = MatlElem.MatdD.Data1.Analysis.Poisson;

    int nFortype=0;
    int iLcomNum = m_pForcCtrl->Get_LcomDataCount();
    ADGNFORCE aForceLcom;
    if(!m_pForcCtrl->Get_LcomDataForDesign(ElemK, nFortype, TRUE, FALSE, aForceLcom, iLcomNum))	{return FALSE;}
    if(m_pForcCtrl->Get_LcomNumForStrs()==0)	{return FALSE;}

    ADGNSTRESS aStressLcom;
    ADGNSTRSADD aStressAdd;    
    if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressAdd, iLcomNum)) {return FALSE;}

    memset(m_dZbar_sb,0,sizeof(m_dZbar_sb));
    memset(m_dCzm_sb,0,sizeof(m_dCzm_sb));
    memset(m_dHt,0,sizeof(m_dHt));
    Get_CompositeGeneralSectionInfo(TRUE, SectElem.SectD, m_dZbar_sb[0], m_dCzm_sb[0], m_dCzp_sb[0], m_dHt[0], m_dArea_sb[0], m_dBc_sb[0]);
    Get_CompositeGeneralSectionInfo(FALSE, SectElem.SectD, m_dZbar_sb[1], m_dCzm_sb[1], m_dCzp_sb[1], m_dHt[1], m_dArea_sb[1], m_dBc_sb[1]);

    int iMaxMin=0;
    int iDgnLcomNo = 0;
    int iOrgLcomNo = 0;
    int iSerLcomCount = 0;
    int iMaxMinType = 0;  
    CString strOrgLcomNa=_T("");

    m_ClassBase.Initialize();
    T_GENL_DATA GLineData;	GLineData.Initialize();
    T_GLINE_DGN_D GLineDgn;	GLineDgn.Initialize();

    iDgnLcomNo = PlssBase.LcomK;
    if(!m_pForcCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iSerLcomCount, iMaxMinType))
    {
        ASSERT(0);
        return FALSE;
    }		    
    int k = bPosiI ? 0 : 1;
    const _DGN_FORC_CRC& ForceCrc = aForceLcom[iDgnLcomNo-1];
    const _DGN_STRS_CRC& StressCrc = aStressLcom[iDgnLcomNo-1];

    double dMuy = ForceCrc.dMuy[k];
    double dMa_Ed = ForceStage.MaxForce.dMuy[k];
    double aForce_Gr[6] = {ForceStage.MaxForce.dFxx[k], ForceStage.MaxForce.dFyy[k], ForceStage.MaxForce.dFzz[k], 
        ForceStage.MaxForce.dMux[k], ForceStage.MaxForce.dMuy[k], ForceStage.MaxForce.dMuz[k]};
    double aForce[6] = {ForceCrc.dFxx[k], ForceCrc.dFyy[k], ForceCrc.dFzz[k], ForceCrc.dMux[k], ForceCrc.dMuy[k], ForceCrc.dMuz[k]};
    double aStress[4]={StressCrc.dStl[k],  StressCrc.dStr[k],	  StressCrc.dSbr[k],	  StressCrc.dSbl[k]};

    if(dMuy > -m_dZero) // 정모멘트 인데 Stage 모멘트가 부모멘트면 0으로..
    {
        if(dMa_Ed < 0.0) {dMa_Ed = 0.0;}
    }
    else
    {
        if(dMa_Ed > 0.0) {dMa_Ed = 0.0;}
    } 

    BOOL bIsGenSect=SectElem.bGenSect;
    if(bIsGenSect)
    {
        GLineDgn = SectElem.GLineDgn[k];
        Get_GenLineDataFromGLineDgn(GLineDgn, GLineData);
    }

    // RBar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
    arRbarPosi.Copy(RbarElem.RbarPosi[k].arRbarPosi);    
    // Longitudinal Stiffener  
    CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
    arLstiPosi.Copy(LstiElem.LstiPosi[k].arLstiPosi);
    // Shear Connector.
    CPG_SCON_POSI SconPosi = SconElem.SconPosi[k];
    // Transverse Stiffener
    CPG_TRST_POSI TrstPosi = TrstElem.TrstPosi[k];
    // End Support
    CPG_TSES_POSI TsesPosi = TsesElem.TsesPosi[k];    

    T_CLAS_BASE ClassD; ClassD.Initialize();
    m_DLineCalcD.Initialize();  
    Get_ClassifyClassSection(bPosiI, aForce_Gr, aForce, MatlElem, SectElem, arRbarPosi, SconElem, GLineData, GLineDgn, m_DLineCalcD, ClassD,ElemK);
    m_ClassBase = ClassD;

    T_PLSS_BASE TempPlssGd = PlssGd;
    T_PLSS_BASE TempPlssBase = PlssBase;
    Calc_LongitudinalShearResistance4SLS(ElemK, bPosiI, bIsGenSect, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, m_CpgdD.dks, TempPlssGd, TempPlssBase, PlssDetail);

    return TRUE;
}

void CDgnPlateGirder_IRC::Set_PrintMode(BOOL bPrint)
{
    m_bPrintMode = bPrint;
}

BOOL CDgnPlateGirder_IRC::IsPrintMode()
{
    return m_bPrintMode;
}


// 6.2.1.2 Plastic resistance moment Mpl,Rd of a composite cross-section.
BOOL CDgnPlateGirder_IRC::Calc_BendingResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem,
                                                 CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,
                                                 CPG_SCON_ELEM& SconElem, CPG_TRST_POSI& TrstPosi, T_CLAS_BASE& ClassD, 
                                                 T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dfck		= MatlElem.dfck;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;  
    double dgam_M0	= m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c		= m_dGamma_C;  
    double dfcd		=  0.54* dfck/dgam_c;
    double dfsd		= fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;

    // 2. Section.  
    T_SECT_D& SectD = SectElem.SectD;
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();	
    T_SECT_STIFFNESS GirderStiff;	GirderStiff.Initialize();

    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

    // Section's Dimension
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);  
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTF2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dhw	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD); 
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD); 
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD); 
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
    double ddc  = GirderStiff.Czp + dHh + 0.5*dtc;
    double ddc1  = GirderStiff.Czm ;
    double dZp = dB1*dTF1*(dH-dTF1) + 0.25*dTw*(dH-2*dTF1)*(dH-2*dTF1);
    //for elastic section zpe
    double dpart1= dTF1+(((0.5*GirderStiff.Area) - (dB1*dTF1))/dTw_t);
    double dytop = ( (dB1*dTF1) >(0.5*GirderStiff.Area))? dpart1:(dpart1-0.5*dTF1);
    double dyweb  = (dB1*dTF1 + (dTw_t*dhw) > (0.5*GirderStiff.Area))? (dpart1-dTF1-0.5*dhw):0;
    double dybot  = 0;
    BOOL bfact1 = (dB1*dTF1)>(0.5*GirderStiff.Area) ?TRUE:FALSE;
    BOOL bfact2 = (((dB1*dTF1) + (dTw_t*dhw))<(0.5*GirderStiff.Area))? TRUE:FALSE;
    if (bfact1 && bfact2) dybot = dpart1-dTF1-dTF2 ;
    else dybot = dpart1-dhw-dTF1-0.5*dTF2 ;
    double dWebfact = dyweb>0? ((dTw_t*dyweb*dyweb/4.0)+(dTw_t*(dhw-dyweb)*(dhw-dyweb)/4.0)) :dTw_t*dhw;
    double dzpe = dWebfact+ (dB1*dTF1*abs(dytop) + dB2*dTF2*abs(dybot));

    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // 3. Force   
    double aForceGd[6]	= {PgbrGd.dFx, 0.0, 0.0, 0.0, PgbrGd.dMy, PgbrGd.dMz}; // Girder
    double aForce[6]		= {PgbrBase.dFx,	0.0, 0.0, 0.0, PgbrBase.dMy, PgbrBase.dMz};   // Composite 
    double dFx = PgbrBase.dFx, dMy = PgbrBase.dMy, dMz = PgbrBase.dMz;
    double dNc = 0.0, dNc_el = 0.0, dNc_f = 0.0, dMpl_Rd = 0.0, dMel_Rd = 0.0,  dM_Rd = 0.0;
    double dMa_Ed = PgbrBase.dMa_Ed;
    double dMc_Ed = PgbrBase.dMy;
    double dM_Ed_c = dMa_Ed + dMc_Ed;
    BOOL bPositive = dMy > -m_dZero ? TRUE : FALSE;

    double dAlpha = m_dAlpha;  
    double dtfc = bPositive ? dTF1	: dTF2;// Thick. of compression flange 
    double dbfc = bPositive ? dB1		: dB2; // Width. of compression flange 

    // 4. Decide Section Class
    BOOL bEquivalentClass2 = FALSE;
    if((ClassD.iFlangeClass==1 || ClassD.iFlangeClass==2) && ClassD.iWebClass==3) {bEquivalentClass2 = TRUE;}	  
    int iClassSect = ClassD.iClassSect;

    // 5. Calc. PNA considering effective width of web
    double dPlasticNA = 0.0, dPlasticNAEff = 0.0, dEffeLen = 0.0, dHoleLen = 0.0;
    BOOL bMembTemp=FALSE;
    CPG_MEMB_ELEM MembElemTemp;  MembElemTemp.Initialize();
    if(!Get_CpgMembElem(ElemK, MembElemTemp)) {bMembTemp=FALSE;}

    CPG_MEMB_POSI MembPosiTemp;
    MembPosiTemp = bPosiI ? MembElemTemp.MembPosi[0] : MembElemTemp.MembPosi[1];
    double dL			= MembPosiTemp.dLu; // span length between the rigid supports.

    Get_PlasticNA(bPosiI, bPositive, dL,MatlElem, SectD, arRbarPosi, dPlasticNA);
    BOOL bCheckEffPlasticNAxis = TRUE;
    if(bEquivalentClass2)   // EN 1993-1-1, 6.2.2.4 Effective properties of cross sections with class 3 webs and class1 or 2 flanges, 48p
    {
        bCheckEffPlasticNAxis = Get_PlasticNAEff(bPosiI, bPositive, MatlElem, SectD, arRbarPosi, dPlasticNA, dPlasticNAEff, dEffeLen, dHoleLen);
    }

    // 6. Check Non-linear
    double dXpl = !bEquivalentClass2 ? dHt - dPlasticNA : dHt - dPlasticNAEff;
    double dXplOverh = dHt < m_dZero ? 0.0 : dXpl/dHt;
    BOOL bNonLinear = dXplOverh > 0.4 ? TRUE : FALSE;

    // 0 = Plastic resistance moment 
    // 1 = Non-linear resistance to bendingdMpl_Rd_cal
    // 2 = Elastic resistance to bending.  

    double dN_slab  = 0.0,	dNg_top = 0.0,	dNg_bot = 0.0,	dNr_top = 0.0,	dNr_bot = 0.0;
    double dNa_topc = 0.0,	dNa_topt = 0.0,	dNa_webc = 0.0, dNa_webt =0.0,	dNa_botc = 0.0, dNa_bott =0.;       
    double dNa_webc1 = 0.0,	dNa_webc2 = 0.0,dNa_web_Effe = 0.0,  dDistT = 0.0, dDistB = 0.0, dDistW = 0.0;
    double dDistTc = 0.0, dDistTt = 0.0,  dDistWc = 0.0, dDistWt = 0.0,  dDistBc = 0.0, dDistBt = 0.0;
    double dDistWc1 = 0.0, dDistWc2 = 0.0, dDistW1=0.0, dDistW2=0.0, dCompfsdAsidsi=0.0, dTensfsdAsidsi=0.0;
    double dCompfsdAsi_sum = 0.0, dTensfsdAsi_sum = 0.0;
    //calculate eta and lambda
    double dEta = 0.0;
    if      (dfck < 60.0+cDGN_Zero)  dEta = 0.80;
    else if (dfck < 110.0+cDGN_Zero) dEta = 0.80 - (dfck-60.0)/500.0;
    else	  				                 dEta = 0.70; 

    double dLambda = 0.0;
    if      (dfck <60.0+cDGN_Zero) dLambda = 1.0;//IRC112:2011,A2-35)
    else if (dfck <110.0+cDGN_Zero) dLambda = 1.0 - (dfck-60.0)/250.0;//IRC112:2011,A2-36)
    else                           dLambda = 0.8;

    // 7. Calc. Plastic Bending Resistance
    double   dB1Sect3 = 0.0; 
    double   dB2Sect3 = 0.0;
    double   dhwSect3 = 0.0;
    if (iClassSect==3)
    {
        //modify Dimensions
        if(dPlasticNA > dH) // PNA in Slab
        {
            dB1Sect3 = 2.0*(9.4*Get_Epsilon(dfyt)*dTF1  )   + dTw ;         
            dB2Sect3 = 2.0*(9.4*Get_Epsilon(dfyb)*dTF2  )   + dTw ; 
            dhwSect3 = dhw ;//+ 0.5*(dTF1+dTF2);

        }
        else
        {
            if(dPlasticNA > dH-dTF1) // PNA in Top Flange
            {
                dB1Sect3 = 2.0*(9.4*Get_Epsilon(dfyt)*dTF1  )   + dTw ;         
                dB2Sect3 = dB2; 
                dhwSect3 = dhw ;

            }
            else /*if(dPlasticNA < dH-dTF1 && dPlasticNA > dTF2)*/ // PNA in Web
            {

                dB1Sect3 = dB1;        
                dB2Sect3 = 2.0*(9.4*Get_Epsilon(dfyb)*dTF2  )   + dTw ; 
                dhwSect3 = dhw ;
            }

        }
    }




    double dNa_top = (iClassSect==3)? dB1Sect3*dTF1*dfyd_t:dB1*dTF1*dfyd_t;
    double dNa_bot = (iClassSect==3)? dB2Sect3*dTF2*dfyd_b:dB2*dTF2*dfyd_b;
    double dNa_web = (iClassSect==3)? dhwSect3*dTw_t*dfyd_w:dhw*dTw_t*dfyd_w;



    if(!bEquivalentClass2 || !bCheckEffPlasticNAxis)
    {    
        if(bPositive) // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
        {
            if(dPlasticNA > dH) // PNA in Slab
            {
                if (iClassSect==3)
                {
                    dDistT = dHt - 0.5*dXpl - (dH-0.5*dTF1);
                    dDistB = dHt - 0.5*dXpl - 0.5*dTF2;
                    dDistW = dHt - 0.5*dXpl - dTF2 - 0.5*dhw;

                    dNc_f = dfcd*dBc*dtc;//dEta * dLambda * dfcd* dBc*dtc
                    double dxu = (dNa_top+dNa_bot+dNa_web)/(dNc_f/dtc);dDistT=dxu;
                    double da_c    = 0.58*dxu;dDistWc1 = da_c;
                    double da_st = ddc - (dxu - dtc*0.5);dDistTc=da_st;

                    dNg_bot = dNa_top + dNa_bot + dNa_web;
                    dMpl_Rd = dNg_bot*da_c + (dNa_top+dNa_bot+dNa_web)*da_st;//dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
                    dN_slab = dNc_f;
                    dNg_top = dfcd* dBc*dtc;
                }
                else
                {
                    dDistT = dHt - 0.5*dXpl - (dH-0.5*dTF1);
                    dDistB = dHt - 0.5*dXpl - 0.5*dTF2;
                    dDistW = dHt - 0.5*dXpl - dTF2 - 0.5*dhw;
                    dNc_f = dBc*dXpl *dfcd;//dEta * dLambda * dfcd* dBc*dtc

                    double da_c    = 0.58*dXpl;dDistWc1 = da_c;
                    double da_st = ddc - (dXpl - dtc*0.5);dDistTc=da_st;

                    dMpl_Rd = dNc_f*da_c + (dNa_top+dNa_bot+dNa_web)*da_st;//dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
                    dN_slab = dNc_f;        
                    dNg_bot = dNa_top + dNa_bot + dNa_web;
                    dNg_top = dfcd* dBc*dtc;
                }



            }
            else
            {
                dNc_f = dfcd* dBc*dtc;
                if(dPlasticNA > dH-dTF1) // PNA in Top Flange
                {
                    if (iClassSect==3)
                    {
                        double dTfc = dH - dPlasticNA;
                        double dY = (dTF1 * 0.5)*(dNc_f+dNa_top-dNa_web-dNa_bot)/dNa_top ;
                        double dxu = dtc+dTF1  -dY;dXpl=dxu;
                        double dTft = dPlasticNA - (dH-dTF1);
                        dNa_topc = dB1*(dxu-dtc-dHh)*dfyd_t;
                        dNa_topt = dB1*(dtc+dHh+dTF1 -dxu )*dfyd_t;
                        dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
                        dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
                        dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                        dDistW2 =dY;// dHt - 0.5*dtc - dTF2 - 0.5*dhw;

                        double da_c    = dXpl - (0.42*dtc);dDistWc1 = da_c;
                        double da_topc = (dXpl - dtc)*0.5;dDistTc=da_topc;
                        double da_topt = (ddc1 - (dXpl - 0.5*dtc));dDistTt=da_topt;
                        double da_web  = da_topt + (dhw*0.5);dDistW=da_web;
                        double da_bot  = da_topt + dhw + (dTF2*0.5);dDistB = da_bot;
                        //
                        dMpl_Rd = dNc_f*da_c + 2.0*dNa_topc*da_topc +  (dNa_top+dNa_bot+dNa_web)*da_topt;
                        //dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
                        dN_slab = dNc_f;  
                        dNg_top = dNa_topc;
                        dNg_bot = dNa_topt + dNa_bot + dNa_web;
                    }
                    else
                    {
                        double dTfc = dH - dPlasticNA;
                        double dTft = dPlasticNA - (dH-dTF1);
                        dNa_topc = dB1*dTfc*dfyd_t;
                        dNa_topt = dB1*dTft*dfyd_t;
                        dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
                        dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
                        dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                        dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;

                        double da_c    = dXpl - (0.42*dtc);dDistWc1 = da_c;
                        double da_topc = (dXpl - dtc)*0.5;dDistTc=da_topc;
                        double da_topt = (ddc - (dXpl - 0.5*dtc));dDistTt=da_topt;
                        double da_web  = da_topt + (dhw*0.5);dDistW=da_web;
                        double da_bot  = da_topt + dhw + (dTF2*0.5);dDistB = da_bot;
                        //
                        dMpl_Rd = dNc_f*da_c + 2.0*dNa_topc*da_topc +  (dNa_top+dNa_bot+dNa_web)*da_topt;
                        //dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
                        dN_slab = dNc_f;  
                        dNg_top = dNa_topc;
                        dNg_bot = dNa_topt + dNa_bot + dNa_web;
                    }

                }
                else if(dPlasticNA < dH-dTF1 && dPlasticNA > dTF2) // PNA in Web
                {
                    if (iClassSect==3)
                    {
                        double dWc = (dH-dTF1) - dPlasticNA;
                        double dY = (dhw * 0.5)*(dNc_f+dNa_top+dNa_web-dNa_bot)/dNa_web ;
                        double dWt = dPlasticNA - dTF2;dDistW=dY;
                        double dxu =dtc+dTF1+dhw -dY;dXpl=dxu;
                        double dTft = dPlasticNA - (dH-dTF1);
                        dNa_webc = (dxu-dtc-dHh-dTF1)*dTw_t*dfyd_w;
                        dNa_webt = (dtc+dHh+dTF1+dhw-dxu)*dTw_t*dfyd_w;
                        dDistWc = 0.5*dtc + dHh + dTF1 + 0.5*dWc;
                        dDistWt = 0.5*dtc + dHh + dTF1 + dWc + 0.5*dWt;
                        dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                        dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
                        //

                        dNa_topc = dB1*dfyd_t*(dxu-dtc-dHh);
                        dNa_topt = dB1*dfyd_t*(dH-dPlasticNA);
                        double da_c    = dxu - (0.42*dtc);dDistWc1 = da_c;
                        double da_topc = dxu - dtc - dHh - 0.5*dTF1;dDistTc=da_topc;
                        double da_topt = (dxu - dtc - dHh - dTF1)*0.5;dDistTt=da_topt;
                        double da_web  = (ddc - (dXpl - 0.5*dtc));dDistB=da_web;
                        double da_bot  = da_topt + dhw + (dTF2*0.5);
                        //
                        dMpl_Rd = dNc_f*da_c + 2.0*(dNa_top*da_topc + dNa_webc*da_topt) +  (dNa_top+dNa_bot+dNa_web)*da_web;
                        //dMpl_Rd = -dNa_top*dDistT - dNa_webc*dDistWc + dNa_webt*dDistWt + dNa_bot*dDistB;
                        dN_slab = dNc_f;  
                        dNg_top = dNa_top + dNa_web + dNc_f;
                        dNg_bot =  dNa_bot;
                    }
                    else
                    {
                        double dWc = (dH-dTF1) - dPlasticNA;
                        double dWt = dPlasticNA - dTF2;dDistW=dWt;
                        double dTft = dPlasticNA - (dH-dTF1);
                        dNa_webc = dWc*dTw_t*dfyd_w;
                        dNa_webt = dWt*dTw_t*dfyd_w;
                        dDistWc = 0.5*dtc + dHh + dTF1 + 0.5*dWc;
                        dDistWt = 0.5*dtc + dHh + dTF1 + dWc + 0.5*dWt;
                        dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                        dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
                        //
                        dNa_topc = dB1*dfyd_t*(dXpl-dtc-dHh);
                        dNa_topt = dB1*dfyd_t*(dH-dPlasticNA);
                        double da_c    = dXpl - (0.42*dtc);dDistWc1 = da_c;
                        double da_topc = dXpl - dtc - dHh - 0.5*dTF1;dDistTc=da_topc;
                        double da_topt = (dXpl - dtc - dHh - dTF1)*0.5;dDistTt=da_topt;
                        double da_web  = (ddc - (dXpl - 0.5*dtc));dDistB=da_web;
                        double da_bot  = da_topt + dhw + (dTF2*0.5);
                        //
                        dMpl_Rd = dNc_f*da_c + 2.0*(dNa_top*da_topc + dNa_webc*da_topt) +  (dNa_top+dNa_bot+dNa_web)*da_web;
                        //dMpl_Rd = -dNa_top*dDistT - dNa_webc*dDistWc + dNa_webt*dDistWt + dNa_bot*dDistB;
                        dN_slab = dNc_f;  
                        dNg_top = dNa_top + dNa_web + dNc_f;
                        dNg_bot =  dNa_bot;
                    }

                }
                else if(dPlasticNA < dTF2) // PNA in Bottom Flange
                {
                    double dBfc = dTF2 - dPlasticNA;
                    double dBft = dPlasticNA;
                    double dTft = dPlasticNA - (dH-dTF1);
                    dNa_botc = dB2*dBfc*dfyd_b;
                    dNa_bott = dB2*dBft*dfyd_b;
                    dDistBc = 0.5*dtc + dHh + dTF1 + dhw + 0.5*dBfc;
                    dDistBt = 0.5*dtc + dHh + dTF1 + dhw + dBfc + 0.5*dBft;
                    dDistT= dHt - 0.5*dtc - (dH-0.5*dTF1);
                    dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
                    //
                    dNa_topc = dB1*dfyd_t*(dXpl-dtc-dHh);
                    dNa_topt = dB1*dfyd_t*(dH-dPlasticNA);
                    double da_c    = dXpl - (dEta*dtc*0.5);
                    double da_topc = (dXpl - dtc)*0.5;
                    double da_topt = (dTF1 - (dTF1 - dTft))*0.5;
                    double da_web  = da_topt + (dhw*0.5);
                    double da_bot  = da_topt + dhw + (dTF2*0.5);
                    //
                    dMpl_Rd = dNc_f*da_c + dNa_topc*da_topc +  dNa_topt*da_topt + dNa_web*da_web + dNa_bot*da_bot;
                    //dMpl_Rd = -dNa_top*dDistT - dNa_web*dDistW - dNa_botc*dDistBc + dNa_bott*dDistBt;
                    dN_slab = dNc_f;  
                    dNg_top = dNa_top + dNa_web + dNa_botc;
                    dNg_bot = dNa_bott;
                }
            }
        }    
        else // Negative moment, Negative일 경우는 강도 계산할 때 PlasticNA가 기준
        {
            dCompfsdAsidsi=0.0;
            dTensfsdAsidsi=0.0;
            dCompfsdAsi_sum = dTensfsdAsi_sum = 0.0;
            double ddsi = 0.0, dAsi = 0.0, dNAdsi = 0.0;
            double dGap = (dH+dHh+0.5*dtc) - dPlasticNA;   
            double dTopArea,dBotArea;dTopArea=0.0;dBotArea=0.0;
            double dCover = 0.0;
            for(int i=0; i< arRbarPosi.GetSize(); i++)
            {
                _DGN_RBAR_CRC& Rbar = arRbarPosi[i];
                dNAdsi = dGap + Rbar.dz;
                dAsi = Rbar.dArea;
                double dFbari = dAsi*dfsd;

                if(dNAdsi < 0.0)
                {
                    dCompfsdAsidsi += dFbari*fabs(dNAdsi);
                    dCompfsdAsi_sum += dFbari;
                    dBotArea+=dAsi;
                }
                else
                {
                    dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
                    dTensfsdAsi_sum += dFbari;
                    dTopArea+=dAsi;
                }
                dCover = 0.5*dtc - Rbar.dz;
            }
            dNr_top = dTensfsdAsi_sum;
            dNr_bot = dCompfsdAsi_sum;

            if(dPlasticNA > dH) // PNA in Slab
            {
                dDistT = dPlasticNA - (dH-0.5*dTF1);
                dDistW = dPlasticNA - (0.5*dhw+dTF2);
                dDistB = dPlasticNA - 0.5*dTF2;      
                //
                dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
                dN_slab = dTensfsdAsi_sum;  
                dNg_top = 0.0;
                dNg_bot = dNa_top + dNa_web + dNa_bot;
            }
            else
            {
                if(dPlasticNA > dH-dTF1) // PNA in Top Flange
                {
                    double dTfc = dPlasticNA - (dH-dTF1);
                    double dTft = dH - dPlasticNA;

                    dNa_topc = dB1*dTfc*dfyd_t;
                    dNa_topt = dB1*dTft*dfyd_t;
                    dDistTc  = 0.5*dTfc;
                    dDistTt  = 0.5*dTft;
                    dDistB   = dPlasticNA - 0.5*dTF2;
                    dDistW   = dPlasticNA - (dTF2+0.5*dhw);        
                    //
                    double dfactz = ( (dTensfsdAsi_sum-dNa_web)*0.5/dB1/dfyd_t) + 0.5*dhw  ;dDistB=dfactz;

                    double dxufrmtop = dtc+dHh + ( (dTensfsdAsi_sum-dNa_web)*0.5/dB1/dfyd_t) ;dDistTt=dxufrmtop;

                    dMpl_Rd = (dZp * dfyd_t) + (dTensfsdAsi_sum*dtc) - ((dTF1*(dNa_top+dNa_web + dNa_bot-dTensfsdAsi_sum))/(4.0*dfyd_t))  ;
                    dNa_botc = dZp;
                    //dMpl_Rd = dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
                    dN_slab = dTensfsdAsi_sum;  
                    dNg_top = dNa_top+dTensfsdAsi_sum;
                    dNg_bot = dNa_web + dNa_bot;
                }
                else if(dPlasticNA < dH-dTF1 && dPlasticNA > dTF2) // PNA in Web
                {
                    if (iClassSect==3)
                    {
                        //For Elastic Section - PNA in Web - Negative
                        double dWc = dPlasticNA - dTF2; 
                        double dWt = (dH-dTF1) - dPlasticNA;
                        dNa_webc = dWc*dTw_t*dfyd_w;
                        dNa_webt = dWt*dTw_t*dfyd_w;
                        dDistWc = 0.5*dWc;
                        dDistWt = 0.5*dWt;
                        dDistT = (dH-0.5*dTF1) - dPlasticNA;
                        dDistB = dzpe;//dPlasticNA - 0.5*dTF2;  
                        double dfacty = dTensfsdAsi_sum/(2.0*dTw_t*dfyd_w);dDistTt=dfacty;
                        double dModZpfact = dTF1 + (((0.5*GirderStiff.Area) - (dB1*dTF1))/dTw_t);
                        double dfactxu = dtc+dHh + dTF1+ ( dModZpfact*0.5) - dfacty;dDistB=dfactxu;
                        double dfactz = (dhw + dTF2 + dTF1)*0.5 +dtc- dCover -  dfacty*0.5;dDistT=dfactz;
                        double dfactzp = 0;
                        dDistTc=dTensfsdAsi_sum;
                        //          double da_c    = dXpl - (0.42*dtc);dDistWc1 = da_c;
                        //           double da_topc = dXpl - dtc - dHh - 0.5*dTF1;dDistTc=da_topc;
                        //           double da_topt = (dXpl - dtc - dHh - dTF1)*0.5;dDistTt=da_topt;
                        //           double da_web  = (ddc - (dXpl - 0.5*dtc));dDistB=da_web;
                        //           double da_bot  = da_topt + dhw + (dTF2*0.5);
                        dNa_botc = dzpe;

                        dMpl_Rd = dzpe * dfyd_b + dfactz*dTensfsdAsi_sum; //dNa_top*dDistT + dNa_webc*dDistWc + dNa_webt*dDistWt+dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
                        dN_slab = dTensfsdAsi_sum;  
                        dNg_top = dTensfsdAsi_sum+dNa_top + dNa_web;
                        dNg_bot = dNa_bot;
                    }
                    else
                    {
                        //For Plastic Section - PNA in Web - Negative
                        double dWc = dPlasticNA - dTF2; 
                        double dWt = (dH-dTF1) - dPlasticNA;
                        dNa_webc = dWc*dTw_t*dfyd_w;
                        dNa_webt = dWt*dTw_t*dfyd_w;
                        dDistWc = 0.5*dWc;
                        dDistWt = 0.5*dWt;
                        dDistT = (dH-0.5*dTF1) - dPlasticNA;
                        dDistB = dPlasticNA - 0.5*dTF2;  
                        double dxufrmtop = dtc+dHh + dTF1+ ( GirderStiff.Czp*0.5) - dPlasticNA;dDistTt=dxufrmtop;
                        double dfactz = (dhw + dTF2 + dTF1)*0.5 + dtc - dCover - 0.5*dPlasticNA;dDistB=dfactz;
                        dDistTc=dTensfsdAsi_sum;
                        //          double da_c    = dXpl - (0.42*dtc);dDistWc1 = da_c;
                        //           double da_topc = dXpl - dtc - dHh - 0.5*dTF1;dDistTc=da_topc;
                        //           double da_topt = (dXpl - dtc - dHh - dTF1)*0.5;dDistTt=da_topt;
                        //           double da_web  = (ddc - (dXpl - 0.5*dtc));dDistB=da_web;
                        //           double da_bot  = da_topt + dhw + (dTF2*0.5);
                        dNa_botc = dZp;

                        dMpl_Rd = dZp * dfyd_b + dfactz*dNr_top; //dNa_top*dDistT + dNa_webc*dDistWc + dNa_webt*dDistWt+dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
                        dN_slab = dTensfsdAsi_sum;  
                        dNg_top = dTensfsdAsi_sum+dNa_top + dNa_web;
                        dNg_bot = dNa_bot;
                    }
                }
                else if(dPlasticNA < dTF2) // PNA in Bottom Flange
                {
                    double dBfc = dPlasticNA;
                    double dBft = dTF2 - dPlasticNA;
                    dNa_botc = dB2*dBfc*dfyd_b;
                    dNa_bott = dB2*dBft*dfyd_b;
                    dDistBc = 0.5*dBfc;
                    dDistBt = 0.5*dBft;
                    dDistT = (dH-0.5*dTF1)-dPlasticNA;
                    dDistW = (dTF2+0.5*dhw)-dPlasticNA;
                    //
                    dMpl_Rd = dNa_top*dDistT + dNa_web*dDistW + dNa_botc*dDistBc + dNa_bott*dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
                    dN_slab = dTensfsdAsi_sum;  
                    dNg_top = dNa_top + dNa_web + dNa_botc;
                    dNg_bot = dNa_bott;
                }
            }
        }
    }
    else // needed to considering effective depth of web
    {    
        if(bPositive) // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
        {
            if(dPlasticNAEff > dH) // PNA in Slab
            {
                if(dXpl>dtc)
                {
                    dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                    dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                    dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
                    dNc_f = dEta * dLambda * dfcd* dBc*dtc;//dNc_f = dBc*dtc*dAlpha*dfcd;
                }
                else if(dXpl<dtc)
                {
                    dDistT = dHt - 0.5*dXpl - (dH-0.5*dTF1);
                    dDistB = dHt - 0.5*dXpl - 0.5*dTF2;
                    dDistW = dHt - 0.5*dXpl - dTF2 - 0.5*dhw;
                    dNc_f = dEta * dLambda * dfcd* dBc*dXpl;//dNc_f = dBc*dXpl*dAlpha*dfcd;
                }
                //
                dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
                dN_slab = dNc_f;        
                dNg_bot = dNa_top + dNa_bot + dNa_web;
            }
            else
            {
                dNc_f = dBc*dtc*dAlpha*dfcd;

                if(dPlasticNAEff > dH-dTF1) // PNA in Top Flange
                {
                    double dTfc = dH - dPlasticNAEff;
                    double dTft = dPlasticNAEff - (dH-dTF1);
                    dNa_topc = dB1*dTfc*dfyd_t;
                    dNa_topt = dB1*dTft*dfyd_t;
                    dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
                    dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
                    dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                    dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
                    //
                    dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
                    dN_slab = dNc_f;  
                    dNg_top = dNa_topc;
                    dNg_bot = dNa_topt + dNa_bot + dNa_web;
                }
                else if(dPlasticNAEff < dH-dTF1 && dPlasticNAEff > dTF2) // PNA in Web
                {
                    double dWc1 = dEffeLen;
                    double dWc2 = dEffeLen;
                    double dWt = dhw-dEffeLen*2.0-dHoleLen;
                    dNa_webc1 = dWc1*dTw_t*dfyd_w;
                    dNa_webc2 = dWc1*dTw_t*dfyd_w;
                    dNa_webt = dWt*dTw_t*dfyd_w;
                    dDistWc1 = 0.5*dtc + dHh + dTF1 + 0.5*dWc1;
                    dDistWc2 = 0.5*dtc + dHh + dTF1 + dWc1 + dHoleLen + 0.5*dWc2;
                    dDistWt  = 0.5*dtc + dHh + dTF1 + dWc1 + dWc2+dHoleLen + 0.5*dWt;
                    dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                    dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
                    //
                    dMpl_Rd = -dNa_top*dDistT-dNa_webc1*dDistWc1-dNa_webc2*dDistWc2+dNa_webt*dDistWt+dNa_bot*dDistB;
                    dN_slab = dNc_f;  
                    dNg_top = dNa_top + dNa_webc1 + dNa_webc2;
                    dNg_bot = dNa_webt + dNa_bot;
                }
                else if(dPlasticNAEff < dTF2) // PNA in Bottom Flange
                {
                    double dBfc = dTF2 - dPlasticNAEff;
                    double dBft = dPlasticNAEff;
                    dNa_botc = dB2*dBfc*dfyd_b;
                    dNa_bott = dB2*dBft*dfyd_b;
                    dDistBc = 0.5*dtc + dHh + dTF1 + dhw + 0.5*dBfc;
                    dDistBt = 0.5*dtc + dHh + dTF1 + dhw + dBfc + 0.5*dBft;
                    dDistT= dHt - 0.5*dtc - (dH-0.5*dTF1);
                    dNa_web_Effe = dTw_t*dEffeLen*dfyd_w;
                    dDistW1 = 0.5*dtc + dHh + 0.5*dEffeLen;
                    dDistW2 = 0.5*dtc + dHh + dEffeLen + dHoleLen + 0.5*dEffeLen;
                    //
                    dMpl_Rd = -dNa_top*dDistT  - dNa_web_Effe*dDistW1 - dNa_web_Effe*dDistW2 - dNa_botc*dDistBc + dNa_bott*dDistBt;
                    dN_slab = dNc_f;  
                    dNg_top = dNa_top + dNa_web_Effe + dNa_web_Effe + dNa_botc;
                    dNg_bot = dNa_bott;
                }
            }
        }
        else // Negative moment, Negative일 경우는 강도 계산할 때 PlasticNA가 기준
        {
            dCompfsdAsidsi = dTensfsdAsidsi=0.0;
            dCompfsdAsi_sum = dTensfsdAsi_sum = 0.0;
            double ddsi=0.0;      
            double dAsi=0.0;
            double dNAdsi=0.0;
            double dGap = (dH+dHh+0.5*dtc) - dPlasticNAEff;            
            double dFbari = 0.0;

            for(int i=0; i<arRbarPosi.GetSize(); i++)
            {
                const _DGN_RBAR_CRC& crRbar = arRbarPosi[i];
                dNAdsi = dGap + crRbar.dz;
                dAsi = crRbar.dArea;
                dFbari = dAsi*dfsd;

                if(dNAdsi < 0.0)
                {
                    dCompfsdAsidsi += dFbari*fabs(dNAdsi);
                    dCompfsdAsi_sum += dFbari;
                }
                else
                {
                    dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
                    dTensfsdAsi_sum += dFbari;
                }
            }
            dNr_top = dTensfsdAsi_sum;
            dNr_bot = dCompfsdAsi_sum;

            if(dPlasticNAEff > dH) // PNA in Slab
            {
                dDistT	= dPlasticNAEff - (dH-0.5*dTF1);
                dDistW1 = dPlasticNAEff - (0.5*dEffeLen+dTF2);
                dDistW2 = dPlasticNAEff - (+0.5*dEffeLen+dHoleLen+dEffeLen+dTF2);
                dDistB	= dPlasticNAEff - 0.5*dTF2;      
                dNa_web_Effe = dEffeLen*dTw_t*dfyd_w;
                //
                dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web_Effe*dDistW1 + dNa_web_Effe*dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
                dN_slab = dTensfsdAsi_sum;  
                dNg_top = 0.0;
                dNg_bot = dNa_top + dNa_bot + dNa_web_Effe + dNa_web_Effe;
            }
            else
            {
                if(dPlasticNAEff > dH-dTF1) // PNA in Top Flange
                {
                    double dTfc = dPlasticNAEff - (dH-dTF1);
                    double dTft = dH - dPlasticNAEff;
                    dNa_topc	= dB1*dTfc*dfyd_t;
                    dNa_topt	= dB1*dTft*dfyd_t;
                    dDistTc	= 0.5*dTfc;
                    dDistTt	= 0.5*dTft;
                    dDistB	= dPlasticNAEff - 0.5*dTF2;
                    dDistW1	= dPlasticNAEff - (0.5*dEffeLen + dTF2);
                    dDistW2	= dPlasticNAEff - (+0.5*dEffeLen + dHoleLen + dEffeLen+dTF2);
                    dNa_web_Effe = dEffeLen*dTw_t*dfyd_w;
                    //
                    dMpl_Rd = dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web_Effe*dDistW1 + dNa_web_Effe*dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
                    dN_slab = dTensfsdAsi_sum;  
                    dNg_top = dNa_topc;
                    dNg_bot = dNa_topt + dNa_bot + dNa_web_Effe + dNa_web_Effe;
                }
                else if(dPlasticNAEff < dH-dTF1 && dPlasticNAEff > dTF2) // PNA in Web
                {
                    double dWc1 = dEffeLen; 
                    double dWc2 = dEffeLen; 
                    double dWt = (dH-dTF1) - dPlasticNAEff;
                    dNa_webc1 = dWc1*dTw_t*dfyd_w;
                    dNa_webc2 = dWc2*dTw_t*dfyd_w;
                    dNa_webt = dWt*dTw_t*dfyd_w;
                    dDistWc1 = 0.5*dWc1;
                    dDistWc2 = dWc1+dHoleLen+dWc2*0.5;
                    dDistWt = 0.5*dWt;
                    dDistT = (dH-0.5*dTF1) - dPlasticNAEff;
                    dDistB = dPlasticNAEff - 0.5*dTF2;        
                    //
                    dMpl_Rd = dNa_top*dDistT + dNa_webc1*dDistWc1 + dNa_webc2*dDistWc2 + dNa_webt*dDistWt+dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
                    dN_slab = dTensfsdAsi_sum;  
                    dNg_top = dNa_top + dNa_webt;
                    dNg_bot = dNa_webc1 + dNa_webc2 + dNa_bot;
                }
                else if(dPlasticNAEff<dTF2) // PNA in Bottom Flange
                {
                    double dBfc = dPlasticNAEff;
                    double dBft = dTF2 - dPlasticNAEff;
                    dNa_botc = dB2*dBfc*dfyd_b;
                    dNa_bott = dB2*dBft*dfyd_b;
                    dDistBc = 0.5*dBfc;
                    dDistBt = 0.5*dBft;
                    dDistT = (dH-0.5*dTF1)-dPlasticNAEff;
                    dDistW = (dTF2+0.5*dhw)-dPlasticNAEff;
                    //
                    dMpl_Rd = dNa_top*dDistT + dNa_web*dDistW + dNa_botc*dDistBc + dNa_bott*dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
                    dN_slab = dTensfsdAsi_sum;  
                    dNg_top = dNa_top + dNa_web + dNa_botc;
                    dNg_bot = dNa_bott;
                }
            }
        }
    }

    // 8. For shear resistance
    double dMpl_Rd_s = 0.0, dMf_Rd = 0.0, dPlasticNA_NeglWeb = 0.0;
    BOOL bNeglectWeb = TRUE;
    if(bEquivalentClass2)
    {Calc_PlasticResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, PgbrBase, dPlasticNA, dMpl_Rd_s);}
    else 
    {dMpl_Rd_s = dMpl_Rd;}
    //

    Get_PlasticNA(bPosiI, bPositive, dL,MatlElem, SectD, arRbarPosi, dPlasticNA_NeglWeb, bNeglectWeb);
    Calc_PlasticResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, PgbrBase, dPlasticNA_NeglWeb, dMf_Rd, bNeglectWeb);
    PgbrBase.dMf_Rd		= dMf_Rd;
    PgbrBase.dMpl_Rd_s	= dMpl_Rd_s;
    m_dMfRd = m_dMfRd==0.0?dMf_Rd:min(m_dMfRd,dMf_Rd);
    // Consider EC3, 6.2.1.2(2)
    BOOL bS420orS460 = IsMaterialS420orS460(dfy);
    double dBeta = 1.0;
    if(bPositive && bS420orS460)
    {
        if(!Get_BetaReductionFactor(dXpl, dHt, dBeta)){dBeta=1.0;}
    }

    // Beta 적용은 정모멘트만.
    PgbrBase.dMpl_Rd	= dMpl_Rd;	 // Modify by GAY. PMS:4216. ('12.05.15). M_Rd = Beta*M_pl_Rd.
    PgbrBase.dNc_f	= dNc_f;
    PgbrBase.dNc		= dFx;       // Nc       
    PgbrBase.dNc_el	= 0.0;       // Nc,el    

    // Elastic Resistance
    Calc_ElasticResistance(ElemK, bPosiI, bPositive, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, arLstiPosi, SconElem, TrstPosi, PgbrBase, PgbrDetail);

    // Non-linear Resistance
    if(bPositive && bS420orS460 && bNonLinear)
    {Calc_NonLinearResistance(ElemK, bPosiI, PgbrBase);}
    else
    {
        if(iClassSect==1 || iClassSect==2)
        {
            PgbrBase.dM_Rd = PgbrBase.dMpl_Rd;
        }
        else
        {
            if (iClassSect==3 && bPositive) PgbrBase.dM_Rd = PgbrBase.dMpl_Rd;      
            else 	PgbrBase.dM_Rd = fabs(PgbrBase.dMel_Rd);

            if(dM_Ed_c * PgbrBase.dMel_Rd < 0.0){dM_Ed_c = 0.0;} // MQC 13393.
        }
    }


    // 9. Save Result
    PgbrBase.iTopClass		= ClassD.iTopFlgClass;
    PgbrBase.iBotClass		= ClassD.iBotFlgClass;
    PgbrBase.iWebClass		= ClassD.iWebClass;
    PgbrBase.iSectClass		= ClassD.iClassSect;  
    PgbrBase.bEquiClass2		= bEquivalentClass2;   
    PgbrBase.dPlasticNA		= dPlasticNA;          
    PgbrBase.dPlasticNAEff	= dPlasticNAEff;       
    PgbrBase.dEffeLen			= dEffeLen;                    
    PgbrBase.dHoleLen			= dHoleLen;
    PgbrBase.dMel_Rd          = fabs(PgbrBase.dMel_Rd);
    PgbrBase.dRatio			= fabs(PgbrBase.dM_Rd) < m_dZero ? fabs(dM_Ed_c)/m_dZero : fabs(dM_Ed_c)/fabs(PgbrBase.dM_Rd);
    PgbrBase.bOK				= PgbrBase.dRatio < 1.0 ? TRUE : FALSE;
    PgbrBase.dh				= dHt;
    PgbrBase.dXpl				= dXpl; 
    PgbrBase.dXplOverh		= dXplOverh;
    PgbrBase.dAlpha			= dAlpha;
    PgbrBase.dNa_top			= dNa_top;
    PgbrBase.dNa_web			= dNa_web;
    PgbrBase.dNa_bot			= dNa_bot;  
    PgbrBase.dNa_topc			= dNa_topc;
    PgbrBase.dNa_topt			= dNa_topt;          
    PgbrBase.dNa_webc			= dNa_webc;
    PgbrBase.dNa_webt			= dNa_webt;          
    PgbrBase.dNa_botc			= dNa_botc;
    PgbrBase.dNa_bott			= dNa_bott;          
    PgbrBase.dNa_webc1		= dNa_webc1;
    PgbrBase.dNa_webc2		= dNa_webc2;        
    PgbrBase.dNa_web_Effe		= dNa_web_Effe;                
    PgbrBase.dDistT			= dDistT;
    PgbrBase.dDistB			= dDistB;
    PgbrBase.dDistW			= dDistW;      
    PgbrBase.dDistTc			= dDistTc;
    PgbrBase.dDistTt			= dDistTt;            
    PgbrBase.dDistWc			= dDistWc;
    PgbrBase.dDistWt			= dDistWt;            
    PgbrBase.dDistBc			= dDistBc;
    PgbrBase.dDistBt			= dDistBt;            
    PgbrBase.dDistWc1			= dDistWc1;
    PgbrBase.dDistWc2			= dDistWc2;          
    PgbrBase.dDistW1			= dDistW1;
    PgbrBase.dDistW2			= dDistW2;            
    PgbrBase.dCompfsdAsidsi	= dCompfsdAsidsi;
    PgbrBase.dTensfsdAsidsi	= dTensfsdAsidsi;
    PgbrBase.dBeta			= m_dTotalLength;     

    PgbrBase.dTopClassRat		= max(ClassD.dBTR[0],ClassD.dBTR[1]);
    PgbrBase.dBotClassRat		=  max(ClassD.dBTR[2],ClassD.dBTR[3]);
    PgbrBase.dWebClassRat		= ClassD.dBTR[4];
    PgbrBase.bTrSt = TrstPosi.bUseStif;
    PgbrBase.dTRSTPitch = TrstPosi.dPitch;
    PgbrBase.dBeffFactor = m_dBeffFactor;
    //COnditions for Minimum Thickness
    double dEtaIRC = sqrt(250.0/m_dfy);
    int nCondstage1 = 0;
    BOOL bCondStage1 = FALSE;
    int nCondstage2 = 0;
    BOOL bCondStage2 = FALSE;
    if (PgbrBase.bTrSt)
    {
        //Transverse stiffener provided
        if (PgbrBase.dTRSTPitch<= 3.0* m_dH && PgbrBase.dTRSTPitch > m_dH)
        {
            nCondstage1 = 1;
            (PgbrBase.dWebClassRat<(200.0*dEtaIRC)) ? bCondStage1=TRUE : bCondStage1=FALSE;
        }
        else if (PgbrBase.dTRSTPitch<=  m_dH && PgbrBase.dTRSTPitch > 0.74*m_dH)
        {
            nCondstage1 = 2;
            ((PgbrBase.dTRSTPitch/m_dTw)<(200.0*dEtaIRC)) ? bCondStage1=TRUE : bCondStage1=FALSE;
        }
        else if (PgbrBase.dTRSTPitch<=  m_dH)
        {
            nCondstage1 = 3;
            (PgbrBase.dWebClassRat<(270.0*dEtaIRC)) ? bCondStage1=TRUE : bCondStage1=FALSE;
        }
        else if (PgbrBase.dTRSTPitch> 3.0* m_dH)
        {
            nCondstage1 = 4;
            bCondStage1 = TRUE ;
        }

        if (PgbrBase.dTRSTPitch> 1.5* m_dH )
        {
            nCondstage2 = 5;
            (PgbrBase.dWebClassRat<(345.0*dEtaIRC*dEtaIRC)) ? bCondStage2=TRUE : bCondStage2=FALSE;
        }
        else
        {
            nCondstage2 = 6;
            (PgbrBase.dWebClassRat<(345.0*dEtaIRC)) ? bCondStage2=TRUE : bCondStage2=FALSE;
        }

    }
    else
    {
        //Transverse stiffener NOT provided

        nCondstage1 = 1;
        (PgbrBase.dWebClassRat<(200.0*dEtaIRC)) ? bCondStage1=TRUE : bCondStage1=FALSE;


        nCondstage2 = 5;
        (PgbrBase.dWebClassRat<(345.0*dEtaIRC*dEtaIRC)) ? bCondStage2=TRUE : bCondStage2=FALSE;


    }
    PgbrBase.nCondStage1 = nCondstage1;
    PgbrBase.nCondStage2 = nCondstage2;
    PgbrBase.bCondStage1 = bCondStage1;
    PgbrBase.bCondStage2 = bCondStage2;


    // 10. For Design Result Print
    if(IsPrintMode())
    {
        T_STRESS_INFO_I StressInfoD;
        T_SECT_STIFFNESS SectStiffD = bPositive ? RebarSect : NonConSect;
        GetStressInfo(bPosiI, SectD, aForceGd, aForce, GirderStiff, SectStiffD, StressInfoD);	  

        PgbrDetail.StressInfoHshapeD = StressInfoD;
        PgbrDetail.dMpl_Rd_cal = dMpl_Rd;
        if(bPositive)
        {
            PgbrDetail.dN_slab = dN_slab;
        }
        else
        {
            PgbrDetail.dN_slab = 0.0;
        }
        PgbrDetail.dNr_top = dNr_top;
        PgbrDetail.dNr_bot = dNr_bot;
        PgbrDetail.dNg_top = dNg_top;
        PgbrDetail.dNg_bot = dNg_bot;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_BetaReductionFactor(double dXpl, double dh, double& dBeta)
{  
    if(dh < m_dZero || dXpl < 0.0){return FALSE;}

    double dRat = dXpl/dh;
    // y=ax + b
    double da = (0.85-1.0)/(0.4-0.15);
    double db = 1.0 - 0.15*da; // b = y-ax;

    if(dRat < 0.15 - m_dZero)			{dBeta = 1.0;}
    else if(dRat > 0.15 && dRat < 0.4){dBeta = da*dRat + db;}
    else if(dRat > 0.4 - m_dZero)		{dBeta = 0.85;}
    else ASSERT(0);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_PlasticResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_PGBR_BASE& PgbrBase, double dPlasticNA, double& dMpl_Rd, BOOL bNeglectWeb)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dfck		= MatlElem.dfck;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;
    double dgam_M0	= m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c		= m_dGamma_C;  
    double dfcd		=  0.54* dfck/dgam_c;
    double dfsd		= fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;



    // 2. Section.  
    T_SECT_D& SectD	= SectElem.SectD;
    T_SECT_STIFFNESS GirderStiff;	GirderStiff.Initialize();
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dB1		= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTF1		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2		= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTF2		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dhw		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);  
    double dBc		= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc		= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh		= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt		= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
    double ddc    = GirderStiff.Czp + dHh + 0.5*dtc;

    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t		= fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b		= fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w		= fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // 3. Force
    double dMy = PgbrBase.dMy;
    double dNc = 0.0;
    double dNc_f = 0.0;
    BOOL bPositive = dMy > -m_dZero ? TRUE : FALSE;  
    double dAlpha = m_dAlpha;  
    double dtfc  = bPositive ? dTF1: dTF2;	// Thick. of Compression Flange
    double dbfc  = bPositive ? dB1: dB2;		// Width. of Compression Flange
    //calculate eta and lambda
    double dEta = 0.0;
    if      (dfck < 60.0+cDGN_Zero)  dEta = 0.80;
    else if (dfck < 110.0+cDGN_Zero) dEta = 0.80 - (dfck-60.0)/500.0;
    else	  				                 dEta = 0.70; 

    double dLambda = 0.0;
    if      (dfck <60.0+cDGN_Zero) dLambda = 1.0;//IRC112:2011,A2-35)
    else if (dfck <110.0+cDGN_Zero) dLambda = 1.0 - (dfck-60.0)/250.0;//IRC112:2011,A2-36)
    else                           dLambda = 0.8;
    // 4. Calc. Plastic Resistance for Bending
    double dPlasticNAEff = 0.0, dEffeLen = 0.0, dHoleLen = 0.0; 
    double dNa_top	= dB1*dTF1*dfyd_t;
    double dNa_bot	= dB2*dTF2*dfyd_b;
    double dNa_web	= bNeglectWeb ? 0.0 : dhw*dTw_t*dfyd_w;
    double dXpl		= dHt - dPlasticNA;

    // dXpl중심에서 거리.
    double dDistT=0.0, dDistB=0.0, dDistW=0.0;

    if(bPositive) // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
    {
        if(dPlasticNA > dH) // PNA in Slab
        {
            if(dXpl > dtc)
            {
                dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
                dNc_f = dfcd* dBc*dtc;
                dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
            }
            else if(dXpl < dtc)
            {
                dDistT = dHt - 0.5*dXpl - (dH-0.5*dTF1);
                dDistB = dHt - 0.5*dXpl - 0.5*dTF2;
                dDistW = dHt - 0.5*dXpl - dTF2 - 0.5*dhw;

                double da_c    = 0.58*dXpl;
                double da_st = ddc - (dXpl - dtc*0.5);//dDistTc=da_topc;

                dNc_f = dBc*dXpl*dfcd;//dEta * dLambda * dfcd* dBc*dtc
                dMpl_Rd = dNc_f*da_c + (dNa_top+dNa_bot+dNa_web)*da_st;//dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
            }


        }
        else
        {
            if(dPlasticNA > dH-dTF1)  // PNA in Top Flange
            {
                dNc_f = dfcd* dBc*dtc;
                double dTfc = dH - dPlasticNA;
                double dTft = dPlasticNA - (dH-dTF1);
                double dNa_topc = dB1*dTfc*dfyd_t;
                double dNa_topt = dB1*dTft*dfyd_t;
                double dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
                double dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
                dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;

                double da_c    = dXpl - (0.42*dtc);
                double da_topc = (dXpl - dtc)*0.5;
                double da_topt = (ddc - (dXpl - 0.5*dtc));
                double da_web  = da_topt + (dhw*0.5);dDistW=da_web;
                double da_bot  = da_topt + dhw + (dTF2*0.5);dDistB = da_bot;
                //
                dMpl_Rd = dNc_f*da_c + 2.0*dNa_topc*da_topc +  (dNa_top+dNa_bot+dNa_web)*da_topt;
                //dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
            }
            else if(dPlasticNA < dH-dTF1 && dPlasticNA > dTF2) // PNA in Web
            {
                double dWc = (dH-dTF1) - dPlasticNA;
                double dWt = dPlasticNA - dTF2;
                double dNa_webc = bNeglectWeb ? 0.0 : dWc*dTw_t*dfyd_w;
                double dNa_webt = bNeglectWeb ? 0.0 : dWt*dTw_t*dfyd_w;
                double dDistWc = 0.5*dtc + dHh + dTF1 + 0.5*dWc;
                double dDistWt = 0.5*dtc + dHh + dTF1 + dWc + 0.5*dWt;
                dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
                //
                dMpl_Rd = -dNa_top*dDistT - dNa_webc*dDistWc - dNa_webt*dDistWt + dNa_bot*dDistB;
            }
            else if(dPlasticNA < dTF2) // PNA in Bottom Flange
            {
                double dBfc = dTF2 - dPlasticNA;
                double dBft = dPlasticNA;
                double dNa_botc = dB2*dBfc*dfyd_b;
                double dNa_bott = dB2*dBft*dfyd_b;
                double dDistBc = 0.5*dtc + dHh + dTF1 + dhw + 0.5*dBfc;
                double dDistBt = 0.5*dtc + dHh + dTF1 + dhw + dBfc + 0.5*dBft;
                dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dhw;
                //
                dMpl_Rd = -dNa_top*dDistT - dNa_web*dDistW - dNa_botc*dDistBc + dNa_bott*dDistBt;
            }
        }
    }
    else // Negative일 경우는 강도 계산할 때 PlasticNA가 기준. 
    {
        double dCompfsdAsidsi=0.0;
        double dTensfsdAsidsi=0.0;
        double ddsi=0.0;   
        double dGap = (dH+dHh+0.5*dtc) - dPlasticNA;    

        for(int i = 0; i< arRbarPosi.GetSize(); i++)
        {
            const _DGN_RBAR_CRC& Rbar = arRbarPosi[i];
            double dNAdsi = dGap + Rbar.dz;
            double dAsi = Rbar.dArea;
            if(dNAdsi < 0.0)
            {
                dCompfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
            }
            else
            {
                dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
            }
        }

        if(dPlasticNA > dH) // PNA in Slab
        {
            dDistT = dPlasticNA - (dH-0.5*dTF1);
            dDistW = dPlasticNA - (0.5*dhw+dTF2);
            dDistB = dPlasticNA - 0.5*dTF2;      
            //
            dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
        }
        else
        {
            if(dPlasticNA > dH-dTF1) // PNA in Top Flange
            {
                double dTfc = dPlasticNA - (dH-dTF1);
                double dTft = dH - dPlasticNA;
                double dNa_topc = dB1*dTfc*dfyd_t;
                double dNa_topt = dB1*dTft*dfyd_t;
                double dDistTc = 0.5*dTfc;
                double dDistTt = 0.5*dTft;
                dDistB = dPlasticNA - 0.5*dTF2;
                dDistW = dPlasticNA - (dTF2+0.5*dhw);        
                //
                dMpl_Rd = dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
            }
            else if(dPlasticNA < dH-dTF1 && dPlasticNA > dTF2) // PNA in Web
            {
                double dWc = dPlasticNA - dTF2; 
                double dWt = (dH-dTF1) - dPlasticNA;
                double dNa_webc = bNeglectWeb ? 0.0 : dWc*dTw_t*dfyd_w;
                double dNa_webt = bNeglectWeb ? 0.0 : dWt*dTw_t*dfyd_w;
                double dDistWc = 0.5*dWc;
                double dDistWt = 0.5*dWt;
                dDistT = (dH-0.5*dTF1) - dPlasticNA;
                dDistB = dPlasticNA - 0.5*dTF2;        
                //
                dMpl_Rd = dNa_top*dDistT + dNa_webc*dDistWc + dNa_webt*dDistWt + dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
            }
            else if(dPlasticNA < dTF2) // PNA in Bottom Flange
            {
                double dBfc = dPlasticNA;
                double dBft = dTF2 - dPlasticNA;
                double dNa_botc = dB2*dBfc*dfyd_b;
                double dNa_bott = dB2*dBft*dfyd_b;
                double dDistBc = 0.5*dBfc;
                double dDistBt = 0.5*dBft;
                dDistT = (dH-0.5*dTF1)-dPlasticNA;
                dDistW = (dTF2+0.5*dhw)-dPlasticNA;
                //
                dMpl_Rd = dNa_top*dDistT + dNa_web*dDistW + dNa_botc*dDistBc + dNa_bott*dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
            }
        }
    }

    return TRUE;
}

// 6.2.1.5 Elastic resistance to bending.
BOOL CDgnPlateGirder_IRC::Calc_ElasticResistance(ElemPairK ElemK, BOOL bPosiI, BOOL bPositive, double aForceGd[6], double aForce[6], CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,
                                                 CPG_SCON_ELEM& SconElem, CPG_TRST_POSI& TrstPosi, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dEs		= MatdD.Data1.Analysis.Elast;
    double dfck		= MatlElem.dfck;
    double dEcm		= MatlElem.dEcm;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;
    double dgam_M0	= m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c		= m_dGamma_C;  
    double dfcd		= 0.54* dfck/dgam_c;
    double dfsd		= fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;

    // 2. Section.
    T_SECT_D& SectD = SectElem.SectD;
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();

    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    //   // Dimension
    //   double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    //   double dTF2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    //   double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    //   double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    //   double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
    //   double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    //   double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    //   double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    //   double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    //   double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;

    // Section's Dimension
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);  
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTF2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dhw	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD); 
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD); 
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD); 
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);


    double dZp = dB1*dTF1*(dH-dTF1) + 0.25*dTw*(dH-2*dTF1)*(dH-2*dTF1);

    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // 3. Force
    double dMel_Rd = 0.0, dNc_el=0.0;
    double dFx	= aForce[0]; 
    double dMy	= aForce[4]; 
    double dMa_Ed	= aForceGd[4];
    double dMc_Ed	= dMy;

    T_CLAS_BASE ClassD = m_ClassBase;
    int iClassSect = ClassD.iClassSect;

    T_SECT_STIFFNESS EffeStlSect;  EffeStlSect.Initialize();
    T_SECT_STIFFNESS EffeComSect;  EffeComSect.Initialize();
    //
    double da = TrstPosi.dPitch;

    T_SECT_STIFFNESS GirderStiff;
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);
    double ddc  = GirderStiff.Czp + dHh + 0.5*dtc;

    //for elastic section zpe
    double dpart1= dTF1+(((0.5*GirderStiff.Area) - (dB1*dTF1))/dTw_t);
    double dytop = ( (dB1*dTF1) >(0.5*GirderStiff.Area))? dpart1:(dpart1-0.5*dTF1);
    double dyweb  = (dB1*dTF1 + (dTw_t*dhw) > (0.5*GirderStiff.Area))? (dpart1-dTF1-0.5*dhw):0;
    double dybot  = 0;
    BOOL bfact1 = (dB1*dTF1)>(0.5*GirderStiff.Area) ?TRUE:FALSE;
    BOOL bfact2 = (((dB1*dTF1) + (dTw_t*dhw))<(0.5*GirderStiff.Area))? TRUE:FALSE;
    if (bfact1 && bfact2) dybot = dpart1-dTF1-dTF2 ;
    else dybot = dpart1-dhw-dTF1-0.5*dTF2 ;
    double dWebfact = dyweb>0? ((dTw_t*dyweb*dyweb/4.0)+(dTw_t*(dhw-dyweb)*(dhw-dyweb)/4.0)) :dTw_t*dhw;
    double dzpe = dWebfact+ (dB1*dTF1*abs(dytop) + dB2*dTF2*abs(dybot));


    double aForce2[6];  memset(aForce2,0,sizeof(aForce2));
    aForce2[0] = aForceGd[0];

    double dDelta_z_a=0.0, dDelta_z_c=0.0;
    double dDelta_Ma_y=0.0, dDelta_Mc_y=0.0;

    T_DGNLINE_LBD_BR RhocBeforeFx, RhocAfterFx, RhocBeforeMy, RhocAfterMy;
    T_SECT_STIFFNESS TempStiffness, EffStiffness4Fx, CompEffStiffness4Fx;
    TempStiffness.Initialize(); EffStiffness4Fx.Initialize();CompEffStiffness4Fx.Initialize();

    if(iClassSect==4 && aForceGd[0]< -m_dZero)
    {  
        T_DGNLINE_LBD_BR RhocAfterFxTemp;
        Get_EffectiveSectionProperty4DB(bPosiI, aForce2, da, MatlElem, SectElem, arRbarPosi, arLstiPosi, ClassD, EffStiffness4Fx, TempStiffness, RhocBeforeFx, RhocAfterFxTemp);    
        dDelta_z_a = GirderStiff.Czm - EffStiffness4Fx.Czm; 
        dDelta_Ma_y = dDelta_z_a * fabs(aForceGd[0]); 
    }
    else
    {
        EffStiffness4Fx = GirderStiff;
    }

    memset(aForce2,0,sizeof(aForce2));  // Composite일 때 작용한 Force
    aForce2[0] = aForce[0];
    if(iClassSect==4 && dFx < -m_dZero)
    {  
        T_DGNLINE_LBD_BR RhocBeforeFxTemp;
        Get_EffectiveSectionProperty4DB(bPosiI, aForce2, da, MatlElem, SectElem, arRbarPosi, arLstiPosi, ClassD, TempStiffness, CompEffStiffness4Fx, RhocBeforeFxTemp, RhocAfterFx);

        if(bPositive)
        {
            dDelta_z_c = RebarSect.Czm - CompEffStiffness4Fx.Czm;
            dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
        }
        else
        {
            dDelta_z_c = NonConSect.Czm - CompEffStiffness4Fx.Czm;
            dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
        }
    }
    else
    {    
        CompEffStiffness4Fx = RebarSect;
    }

    double dMy_gir = aForceGd[4] + dDelta_Ma_y;
    double dMy_com = dMy         + dDelta_Mc_y;

    // 모멘트에 대하여
    memset(aForce2,0,sizeof(aForce2));
    aForce2[4] = aForceGd[4]; 
    T_SECT_STIFFNESS EffStiffness4My; EffStiffness4My.Initialize();
    T_SECT_STIFFNESS CompEffStiffness4My; CompEffStiffness4My.Initialize();

    if(iClassSect==4)
    {
        T_DGNLINE_LBD_BR RhocAfterMyTemp;
        Get_EffectiveSectionProperty4DB(bPosiI, aForce2, da, MatlElem, SectElem, arRbarPosi, arLstiPosi, ClassD, 
            EffStiffness4My, TempStiffness, RhocBeforeMy, RhocAfterMyTemp);  
    }
    else
    {
        EffStiffness4My = GirderStiff;
    }  

    memset(aForce2,0,sizeof(aForce2));
    aForce2[4] = dMy; 
    if(iClassSect==4)
    {
        T_DGNLINE_LBD_BR RhocBeforeMyTemp;
        Get_EffectiveSectionProperty4DB(bPosiI, aForce2, da, MatlElem, SectElem, arRbarPosi, arLstiPosi, ClassD, 
            TempStiffness, CompEffStiffness4My,RhocBeforeMyTemp, RhocAfterMy, TRUE);  
    }
    else
    {
        CompEffStiffness4My = bPositive ? RebarSect : NonConSect;
    }  

    m_EffeStlSect = EffeStlSect = EffStiffness4My; // My에 대한 Effective stiffness
    m_EffeComSect = EffeComSect = CompEffStiffness4My; // My에 대한 Composite Effective stiffness

    // Before
    double dStl_Area = EffStiffness4Fx.Area;
    double dStl_Iyy  = EffStiffness4My.Ryy;
    double dStl_Izz  = EffStiffness4My.Rzz;
    double dStl_Czm  = EffStiffness4My.Czm;
    double dStl_Czp  = EffStiffness4My.Czp;
    // Composite
    double dArea_c = CompEffStiffness4Fx.Area;
    double dIyy_c  = CompEffStiffness4My.Ryy;  
    double dIzz_c  = CompEffStiffness4My.Rzz;
    double dCzm_c  = CompEffStiffness4My.Czm;
    double dCzp_c  = CompEffStiffness4My.Czp;                                        

    double dSIGa_top = 0.0;
    double dSIGa_bot = 0.0;  

    // Rebar  
    double dRbarZ_max = 0.0;
    int iRbarSize = arRbarPosi.GetSize();
    for(int i = 0; i < iRbarSize; i++)
    {
        const _DGN_RBAR_CRC& RbarD = arRbarPosi[i];
        if(i==0)					{dRbarZ_max = RbarD.dz;}
        else {if(dRbarZ_max<RbarD.dz){dRbarZ_max = RbarD.dz;}}
    }  

    double dCompfsdAsidsi=0.0;
    double dTensfsdAsidsi=0.0;
    double dCompfsdAsi_sum=0.0 ;
    double dTensfsdAsi_sum=0.0 ;
    double ddsi = 0.0, dAsi = 0.0, dNAdsi = 0.0;
    double dGap = (dH+dHh+0.5*dtc) ;   
    double dTopArea,dBotArea;dTopArea=0.0;dBotArea=0.0;
    double dCover = 0.0;
    for(int i=0; i< arRbarPosi.GetSize(); i++)
    {
        _DGN_RBAR_CRC& Rbar = arRbarPosi[i];
        dNAdsi = dGap + Rbar.dz;
        dAsi = Rbar.dArea;
        double dFbari = dAsi*dfsd;

        if(dNAdsi < 0.0)
        {
            dCompfsdAsidsi += dFbari*fabs(dNAdsi);
            dCompfsdAsi_sum += dFbari;
            dBotArea+=dAsi;
        }
        else
        {
            dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
            dTensfsdAsi_sum += dFbari;
            dTopArea+=dAsi;
        }
        dCover = 0.5*dtc - Rbar.dz;
    }

    double dFx_a = aForceGd[0];  
    double dMz_a = 0.0;    
    double dStrStlTop = Calc_StressOfPosition(dFx_a, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, dStl_Czp);
    double dStrComTop = Calc_StressOfPosition(dFx,   dMy_com, 0.0,   dArea_c,   dIyy_c,   dIzz_c,   0.0, dCzp_c);
    double dStrStlBot = Calc_StressOfPosition(dFx_a, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, -dStl_Czm); 
    double dStrComBot = Calc_StressOfPosition(dFx,   dMy_com, 0.0,   dArea_c,   dIyy_c,   dIzz_c,   0.0, -dCzm_c);
    dSIGa_top = dStrStlTop + dStrComTop; // (-) 압축, (+) 인장.
    dSIGa_bot = dStrStlBot + dStrComBot;

    double dn = dEcm < m_dZero ? 0.0 : dEs/dEcm;
    double dCentToSlabTop = dHt - dCzm_c;
    double dStrSlab_top = dn < m_dZero ? 0.0 : Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToSlabTop) / dn;  
    double dCentToRbar  = dHt - dtc/2.0 + dRbarZ_max - dCzm_c;  //dRbarZ_max : 슬래브 도심에서 철근까지 거리.
    double dStrRbar_top = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToRbar);

    double dM_Ed_top	= fabs(dSIGa_top*(dIyy_c/dCzp_c)); // For shear resistance
    double dM_Ed_bot	= fabs(dSIGa_bot*(dIyy_c/dCzm_c));
    double dM_Ed		= max(dM_Ed_top, dM_Ed_bot);

    // k : 수정 2013.10.28  // Compression(-), Tension(+)
    //     수정 2014.04.17  :  개념 : Sig_a + k*Sig_c = Sig_y
    //                                Sig_c 와 Sig_y의 부호는 같도록 한다.
    //     수정 2014.04.29  :   Sig_a + Sig_Dela + k*Sig_c + Sig_Delc  = Sig_y

    //double dk_slab_temp = dStrSlab_top==0.0 ? dfcd/m_dZero : fabs(dfcd/dStrSlab_top);
    double dSigma_sl_gir     = 0.0; // 합성전 슬래브 단면 없다.
    double dSigma_sl_My      = (dIyy_c * dn < m_dZero) ?  1.0/m_dZero : (-1.)*dMy           *dCentToSlabTop/dIyy_c /dn;   
    double dSigma_sl_DelMy   = (dIyy_c * dn < m_dZero) ?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *dCentToSlabTop/dIyy_c /dn;   
    double dfcd_temp         = (dSigma_sl_My + dSigma_sl_DelMy < 0.0) ? (-1.0)*dfcd : dfcd; 
    double dk_slab           = fabs(dSigma_sl_My) <  m_dZero ? dfcd/m_dZero : (dfcd_temp-dSigma_sl_gir-dSigma_sl_DelMy)/(dSigma_sl_My);

    //double dk_rbar_temp = dStrRbar_top==0.0 ? dfsd/m_dZero : fabs(dfsd/dStrRbar_top);
    double dSigma_rb_gir     = 0.0; // 합성전 철근 없다.
    double dSigma_rb_My      = (dIyy_c < m_dZero) ?  1.0/m_dZero : (-1.)*dMy           *dCentToRbar/dIyy_c;
    double dSigma_rb_DelMy   = (dIyy_c < m_dZero) ?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *dCentToRbar/dIyy_c;  
    double dfsd_temp         = (dSigma_rb_My + dSigma_rb_DelMy<0.0) ? (-1.0)*dfsd : dfsd; 
    double dk_rbar           = fabs(dSigma_rb_My) <  m_dZero ? dfsd/m_dZero : (dfsd_temp-dSigma_rb_gir-dSigma_rb_DelMy)/(dSigma_rb_My);

    //double dk_top_temp = dStrComTop==0.0 ? (dfyd_t-fabs(dStrStlTop))/m_dZero : (dfyd_t-fabs(dStrStlTop))/fabs(dStrComTop);
    double dSigma_top_gir    = (dStl_Iyy < m_dZero) ?  1.0/m_dZero : (-1.)*dMy_gir       *dStl_Czp/dStl_Iyy;
    double dSigma_top_My     = (dIyy_c < m_dZero) ?  1.0/m_dZero : (-1.)*dMy           *dCzp_c/dIyy_c;
    double dSigma_top_DelMy  = (dIyy_c < m_dZero) ?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *dCzp_c/dIyy_c;
    double dfyd_t_temp       = (dSigma_top_My + dSigma_top_DelMy<0.0) ? (-1.0)*dfyd_t : dfyd_t; 
    double dk_top            = fabs(dSigma_top_My) <  m_dZero  ? 1.0/m_dZero : (dfyd_t_temp-dSigma_top_gir-dSigma_top_DelMy)/(dSigma_top_My);

    //double dk_bot_temp = dStrComBot==0.0 ? (dfyd_b-fabs(dStrStlBot))/m_dZero : (dfyd_b-fabs(dStrStlBot))/fabs(dStrComBot);
    double dSigma_bot_gir    = (dStl_Iyy < m_dZero) ?  1.0/m_dZero : (-1.)*dMy_gir       *(-dStl_Czm)/dStl_Iyy;
    double dSigma_bot_My     = (dIyy_c < m_dZero) ?  1.0/m_dZero : (-1.)*dMy           *(-dCzm_c)/dIyy_c;
    double dSigma_bot_DelMy  = (dIyy_c < m_dZero) ?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *(-dCzm_c)/dIyy_c;
    double dfyd_b_temp       = (dSigma_bot_My + dSigma_bot_DelMy<0.0) ? (-1.0)*dfyd_t : dfyd_t; 
    double dk_bot            = fabs(dSigma_bot_My) <  m_dZero  ? 1.0/m_dZero : (dfyd_b_temp-dSigma_bot_gir-dSigma_bot_DelMy)/(dSigma_bot_My);

    BOOL bPositive2 = (dMy_com > -m_dZero) ? TRUE : FALSE;
    double dk_min = Calc_kmin(bPositive2, dk_rbar, dk_slab, dk_top, dk_bot);
    double dMel_Rd_temp = dMa_Ed + dk_min*dMy +dDelta_Mc_y; // (20140313 수정) : k는 축력의 영향을 고려안한 모멘트에만 추가.
    dMel_Rd = dMel_Rd_temp;

    // Positive 일 때만 계산.
    double dSig_c_mean_temp = 0.0;
    double dNc_el_temp  =  0.0;

    double dStop = dHt-dCzm_c;
    double dSig_c_mean = 0.0;
    if(bPositive)
    {
        //double dza_sc = dHt - dStl_Czm - 0.5*dtc; // Ma
        // [4/11/2014 joahn], dSig_c_mean 값은 거더를 포함하지 않는 것으로 수정, 최성기 CJ님 요청
        dSig_c_mean = dk_min*dMc_Ed/dIyy_c*(dStop-0.5*dtc)/dn; 
        dNc_el      = dSig_c_mean * (dBc*dtc);
    }
    //Elastic moment calculation
    double dxe = dTopArea*(0.5*dH + dtc ) / (dStl_Area+dTopArea+dHh);
    double dIc = dStl_Iyy + (dStl_Area*dxe*dxe) + dTopArea*(0.5*dH +dtc+dHh-dxe)*(0.5*dH +dtc+dHh-dxe);
    double dfs = abs(dMa_Ed)*(0.5*dH)/dStl_Iyy;
    dMel_Rd = abs(dMa_Ed)+((dfyd_t-dfs)*dIc/(0.5*dH+dxe));
    //Limiting elastic stress for servicabilitty should not sxceed 0.87fy
    dNc_el = m_dfy*0.87;
    // Save Result
    PgbrBase.dMel_Rd		= dMel_Rd;
    PgbrBase.dNc_el		= dNc_el;
    PgbrBase.dElasticNA	= dMy > -m_dZero ? RebarSect.Czm : NonConSect.Czm;
    PgbrBase.dElasticNAEff= EffeComSect.Czm;// 
    //
    PgbrBase.dStrStlTop = dStrStlTop;
    PgbrBase.dStrComTop = dStrComTop;
    PgbrBase.dStrStlBot = dStrStlBot;
    PgbrBase.dStrComBot = dStrComBot;  
    PgbrBase.dSIGa_top = dSIGa_top;
    PgbrBase.dSIGa_bot = dSIGa_bot;  
    PgbrBase.dk_min = dk_min;       

    PgbrDetail.dM_Ed_top      = dxe;
    PgbrDetail.dM_Ed_bot      = dIc;
    PgbrDetail.dM_Ed          = dfs;
    PgbrDetail.EffGirderFx    = EffStiffness4Fx;
    PgbrDetail.EffCompositeFx = CompEffStiffness4Fx;
    PgbrDetail.EffGirder      = EffeStlSect;
    PgbrDetail.EffComposite   = EffeComSect;

    if(IsPrintMode())
    {
        PgbrDetail.dDelMa_Ed = dDelta_Ma_y;
        PgbrDetail.dDelMc_Ed = dDelta_Mc_y;
    }  

    PgbrDetail.RhocBeforeFx = RhocBeforeFx;
    PgbrDetail.RhocAfterFx  = RhocAfterFx;
    PgbrDetail.RhocBeforeMy = RhocBeforeMy;
    PgbrDetail.RhocAfterMy  = RhocAfterMy;

    return TRUE;
}

double CDgnPlateGirder_IRC::Calc_kmin(BOOL bPositive, double dk_rbar, double dk_slab, double dk_top, double dk_bot)
{
    double dk_rbar_ab = fabs(dk_rbar);
    double dk_slab_ab = fabs(dk_slab);
    double dk_top_ab  = fabs(dk_top);
    double dk_bot_ab  = fabs(dk_bot);

    double dk_min = 0.0;
    double dk_min_ab = 0.0;

    if(bPositive)
    {
        double dk_ab[3] = {dk_slab_ab, dk_top_ab, dk_bot_ab};
        double dk[3]    = {dk_slab   , dk_top   , dk_bot};

        for(int i=0; i<3; i++)
        {
            if(i==0)
            {
                dk_min_ab = dk_ab[i];
                dk_min    = dk[i];
            }
            else       
            {
                if(dk_min_ab > dk_ab[i])
                {
                    dk_min_ab = dk_ab[i];
                    dk_min    = dk[i];
                }
            }
        }
    }
    else
    {
        double dk_ab[3] = {dk_rbar_ab, dk_top_ab, dk_bot_ab};
        double dk[3]    = {dk_rbar   , dk_top   , dk_bot};

        for(int i=0; i<3; i++)
        {
            if(i==0)
            {
                dk_min_ab = dk_ab[i];
                dk_min    = dk[i];
            }
            else       
            {
                if(dk_min_ab > dk_ab[i])
                {
                    dk_min_ab = dk_ab[i];
                    dk_min    = dk[i];
                }
            }
        }
    }

    return dk_min;
}

// 6.2.1.4 Non-linear resistance to bending.
BOOL CDgnPlateGirder_IRC::Calc_NonLinearResistance(ElemPairK ElemK, BOOL bPosiI, T_PGBR_BASE& PgbrBase)
{
    double dNc		= PgbrBase.dNc;
    double dNc_el		= PgbrBase.dNc_el;
    double dNc_f		= PgbrBase.dNc_f;
    double dMa_Ed		= PgbrBase.dMa_Ed;
    double dMc_Ed		= PgbrBase.dMy;
    double dMpl_Rd	= PgbrBase.dMpl_Rd;  
    double dMel_Rd	= PgbrBase.dMel_Rd;
    double dM_Rd		= 0.0;

    if(dNc < 0.0)  // Concrete slab under compression
    {
        if(fabs(dNc)<=dNc_el)
        {
            dM_Rd = fabs(dNc_el) < m_dZero ? dMa_Ed : dMa_Ed+(dMel_Rd-dMa_Ed)*fabs(dNc)/dNc_el;
        }
        else if(dNc_el<=fabs(dNc) && fabs(dNc)<=dNc_f)
        {
            dM_Rd = fabs(dNc_f-dNc_el) < m_dZero ? dMel_Rd : dMel_Rd+(dMpl_Rd-dMel_Rd)*(fabs(dNc)-dNc_el)/(dNc_f-dNc_el);
        }
        else ASSERT(0);
    }
    else
    {
        dM_Rd = dMel_Rd;
    }

    PgbrBase.dM_Rd = dM_Rd;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_VerticalShearResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                                       CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, 
                                                       T_CLAS_BASE& ClassD, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail,T_PGBR_BASE& PgbrBase)
{
    T_VERTICAL_SHAER_RES_D_IRC ResD;

    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    int iMatClass		= MatlElem.iMatClass;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dgam_M0	= m_dGamma_M0;
    double dEta		= Get_Eta4SteelGrade(iMatClass, dfy);

    // 2. Section.
    T_SECT_D& SectD	= SectElem.SectD;
    bool bBoxSectT	= m_pDgnSectUtil->Is_StlBoxSect(&SectD);
    double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dHw		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dhw_in	= m_pDgnSectUtil->Get_Size_Inclind_hw(&SectD, bPosiI);
    double dfyw		= Get_fy4Thick(dfy, dfy1, dTw);
    double dEpsi	= Get_Epsilon(dfyw);

    // 3. Force
    double aForce[6]	= {PrvsBase.dFx, 0.0, 0.0, 0.0, PrvsBase.dMy, PrvsBase.dMz};
    double dFx		= PrvsBase.dFx;
    double dMy		= PrvsBase.dMy;
    double dMz		= PrvsBase.dMz;
    double dMa_Ed		= PrvsBase.dMa_Ed;
    double dMc_Ed		= dMy;
    double dV_Ed_a	= PrvsGd.dFz;
    double dV_Ed_c	= PrvsBase.dFz;
    double dV_Ed		= PrvsBase.dV_Ed;

    // 4. Calc. Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)
    double dAv	 = dHw*dTw_t;
    double dVpl_Rd = dgam_M0 < m_dZero ? 0.0 : dAv*(dfyw/sqrt(3.0))/dgam_M0;

    // 5. Calc. Shear buckling resistance, EN 1993-1-5,5 (22page)
    double dVb_Rd = 0.0;
    Calc_Vb_Rd(bPosiI, MatlElem, SectD, arLstiPosi, TrstPosi, TsesPosi, PrvsBase, ClassD, dVb_Rd, ResD);

    // 6. Determine V_Rd (Vertical Shear Resistance)
    double dV_Rd		= 0.0;
    double dRatio		= 0.0;

    if(ResD.ShearResD[0].bIsShearBuck || ResD.ShearResD[1].bIsShearBuck) // Case 1. Shear Buckling Check is needed 
    {
        dV_Rd  = dVb_Rd;
        dRatio = fabs(dVb_Rd) < m_dZero ? fabs(dV_Ed)/m_dZero : fabs(dV_Ed)/dVb_Rd;
    }
    else // Case 2. Shear Buckling Check is not needed 
    {
        dV_Rd  = dVpl_Rd;
        dRatio = fabs(dVpl_Rd) < m_dZero ? fabs(dV_Ed)/m_dZero : fabs(dV_Ed)/dVpl_Rd;    
    }

    if(dRatio>1.0){PrvsBase.bOK = FALSE;}    
    else			{PrvsBase.bOK = TRUE;}    

    if(PrvsBase.bOK && ResD.ShearResD[0].bEta3Exceed05)
    {
        if(ClassD.iClassSect==3 || ClassD.iClassSect==4)
        {
            if(!ResD.ShearResD[0].bCombinedEffe){PrvsBase.bOK = FALSE;}   
        }
    }

    // 7. Save Vertical Shear Check Result
    PrvsBase.iTopClass		= ClassD.iTopFlgClass;
    PrvsBase.iBotClass		= ClassD.iBotFlgClass;
    PrvsBase.iWebClass		= ClassD.iWebClass;
    PrvsBase.iSectClass		= ClassD.iClassSect;    
    PrvsBase.dM_Ed			= ResD.ShearResD[0].dM_Ed;  
    PrvsBase.dMf_Rd			= ResD.ShearResD[0].dMf_Rd;
    PrvsBase.dMpl_Rd			= ResD.ShearResD[0].dMpl_Rd;  
    PrvsBase.dVpl_Rd			= dVpl_Rd;
    PrvsBase.dVbw_Rd			= ResD.dVbw_Rd;
    PrvsBase.dVbf_Rd			= ResD.dVbf_Rd;
    PrvsBase.dVb_Rd			= dVb_Rd;
    PrvsBase.dV_Rd			= dV_Rd;
    PrvsBase.dEta1			= ResD.ShearResD[0].dBar_Eta1;
    PrvsBase.dEta1_lim		= ResD.ShearResD[0].dBar_Eta1_Lim;
    PrvsBase.dEta3			= ResD.ShearResD[0].dBar_Eta3;
    PrvsBase.dCombinedEffe	= ResD.ShearResD[0].dCombinedEffe;
    PrvsBase.bCombinedEffe	= ResD.ShearResD[0].bCombinedEffe;
    PrvsBase.bEta3Exceed05	= ResD.ShearResD[0].bEta3Exceed05;  
    PrvsBase.dRatio			= dRatio;
    PrvsBase.dAv				= dAv;        
    PrvsBase.deta				= dEta;       
    PrvsBase.dIsl_org			= ResD.ShearResD[0].dIsl_org;   
    PrvsBase.dIsl				= ResD.ShearResD[0].dIsl;         
    PrvsBase.dXw				= ResD.ShearResD[0].dXw;        
    PrvsBase.dMax_Bar_Lamda_w = ResD.ShearResD[0].dMax_Bar_Lamda_w;
    PrvsBase.dc				= ResD.ShearResD[0].dc;
    PrvsBase.dbf				= ResD.ShearResD[0].dbf;         
    PrvsBase.dtf				= ResD.ShearResD[0].dtf;         
    PrvsBase.dVb_Rd_org		= ResD.ShearResD[0].dVb_Rd_org;  
    PrvsBase.dVb_Rd_Lim		= ResD.ShearResD[0].dVb_Rd_Lim;  
    PrvsBase.bEta1Ratio		= FALSE;

    //For IRC High Shear Report 
    PrvsBase.m_dBetaFactHighShear = (2.0*abs(PrvsBase.dV_Ed/PrvsBase.dVpl_Rd) -1)*(2.0*abs(PrvsBase.dV_Ed/PrvsBase.dVpl_Rd) -1);
    PrvsBase.m_dHighShearCond1 = m_PgbrBase.dMpl_Rd - (PrvsBase.m_dBetaFactHighShear*(m_PgbrBase.dMpl_Rd-m_dMfRd));

    PrvsBase.m_dHighDhearCond2 = 1.2*m_dRyy*m_dfy / m_dGamma_M0;
    PrvsBase.m_dMfRd = m_dMfRd;


    Convert_VerbShearResD(ResD.ShearResD[0], PrvsBase.DetailResD[0]);
    if(bBoxSectT) {Convert_VerbShearResD(ResD.ShearResD[1], PrvsBase.DetailResD[1]);}

    // 8. Save Vertical Shear Check Result for Print
    CArray<T_PRVS_Vbw_Rd_Param,T_PRVS_Vbw_Rd_Param&> aGenWebVbwRdParam;

    if(IsPrintMode())
    {
        int nWebSize = bBoxSectT ? 2 : 1;
        for(int nWeb = 0; nWeb < nWebSize; ++nWeb)
        {
            T_PRVS_Vbw_Rd_Param CalcVbwParam;
            const T_VERTICAL_SHAER_RES_UNIT_D_IRC& Res = ResD.ShearResD[nWeb];
            double dAlpha_p = Get_Alpha4Lamda_w(Res.dRigidPitch, dhw_in);
            CalcVbwParam.iStiffSize = Res.iLongStiffSize;
            CalcVbwParam.dc		=	Res.dc;
            CalcVbwParam.dVbf_Rd	=	Res.dVbf_Rd;

            if(Res.iLongStiffSize > 0)
            {      
                double dAlpha_si = Get_Alpha4Lamda_w(Res.dai_cr, Res.dhi_cr);
                Set_Calc_Vbw_SubPanel_Param(Res.nLSsize_cr, Res.dhi_cr, dTw, dAlpha_si, Res.dk_tau_si_cr, Res.dk_tau_sti_cr, 
                    Res.dk_tau_st_lti_cr, Res.dIsli_cr, Res.dMax_Lamda_wi, CalcVbwParam.Lamda_w_SubD);
            }

            int nPartType = 2; //Web
            CalcVbwParam.nPartType	= nPartType; 
            CalcVbwParam.bStiffened	= Res.bStiffened;
            CalcVbwParam.bIsShearBuck = Res.bIsShearBuck;

            if(nPartType==2) // Web
            {
                Set_Calc_Vbw_Param(dAlpha_p, Res.dk_tau_p, Res.dk_tau_st_p, Res.dk_tau_st_Limit_p, Res.dIsl, dhw_in, dTw, 
                    Res.dLamda_w_part, Res.dLamda_w_cr, Res.iLamda_w_Type, CalcVbwParam);
                Set_Calc_Vbw_Param2(Res.iLamda_w_scope, Res.dXw, Res.dVbw_Rd, dVb_Rd, Res.dVb_Rd_Lim, dV_Ed, Res.dBar_Eta3, CalcVbwParam);
                CalcVbwParam.dVpl_Rd = dVpl_Rd;

                aGenWebVbwRdParam.Add(CalcVbwParam);        
            }    
        }

        PrvsDetail.StressInfoHshapeD		= m_PgbrDetail.StressInfoHshapeD;
        PrvsDetail.iEndSupportType		= ResD.ShearResD[0].iEndpost;
        PrvsDetail.dPlasticNA				= m_PgbrBase.dPlasticNA;
        PrvsDetail.dN_slab				= m_PgbrDetail.dN_slab;
        PrvsDetail.dNg_top				= m_PgbrDetail.dNg_top;    
        PrvsDetail.dNg_bot				= m_PgbrDetail.dNg_bot;    
        PrvsDetail.dMf_Rd_cal				= ResD.ShearResD[0].dMf_Rd;
        PrvsDetail.dMf_N_Rd				= ResD.ShearResD[0].dMf_Rd_factor;
        PrvsDetail.dReduFactor4N_Ed		= ResD.ShearResD[0].dFactor_fr;  
        PrvsDetail.nShearBuckCount		= ResD.ShearResD[0].nShearBuckCount;
        PrvsDetail.aGenWebVbwRdParam.Copy(aGenWebVbwRdParam);
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_Eta4SteelMaterial(int iMatClass, double dgam0, double dgam1, double& dEta)
{
    dEta = 0.0;
    if(dgam0==0.0) {return FALSE;}
    if(iMatClass==1) // S235, S275, S355 계열.
    {
        dEta = 1.20*dgam1/dgam0;
    }
    else if(iMatClass==2) // S420, S460 계열.
    {
        dEta = 1.05*dgam1/dgam0;
    }
    else if(iMatClass==3) // etc.
    {
        dEta = 1.00*dgam1/dgam0;
    }
    else
        ASSERT(0);

    return TRUE;
}

// Modify by GAY. PMS:4216. ('12.05.11). 
double CDgnPlateGirder_IRC::Get_Eta4SteelGrade(int nMatClass, double dFy1)
{
    // See EN1993-1-5, 5.1(2).
    // η should be defined in the National Annex of EN1993-1-5.
    // η = 1.2 for structural steel up to and including S460.
    // η = 1.0 for higher steel grades.
    double dEta = 0.0;
    if(nMatClass==1 || nMatClass==2) 
    {
        dEta = 1.2;  // EN05(S) List의 Steel Grade. (S235~S460)
    }
    else if(nMatClass==3)
    {
        if(dFy1>460.0) {dEta = 1.0;}
        else           {dEta = 1.2;}
    }
    else ASSERT(0);

    return dEta;
}

BOOL CDgnPlateGirder_IRC::Calc_LateralTorsionalBucklingResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem,
                                                                  CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,
                                                                  CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, CPG_LTBD_POSI& LtbdPosi, 
                                                                  CPG_MEMB_POSI& MembPosi, T_CLAS_BASE& ClassD, T_PLTB_BASE& PltbBase, 
                                                                  T_PLTB_DETAIL& PltbDetail)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dEs		= MatdD.Data1.Analysis.Elast;
    double dPoissonS	= MatdD.Data1.Analysis.Poisson;
    double dGs		= 77000.0;//dEs/(1.0+dPoissonS);
    double dgam_M0	= m_dGamma_M0;
    double dfyd		= fabs(dgam_M0) < m_dZero ? dfy : dfy/dgam_M0;

    // 2. Section.
    T_SECT_D& SectD	= SectElem.SectD;
    BOOL bTapered		= m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_STIFFNESS NonConSect, RebarSect;	  NonConSect.Initialize();	RebarSect.Initialize();
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    // Get Section Dimension
    BOOL bSectI_Type = (m_pDgnSectUtil->Get_RealSectType(&SectD) == D_SECT_TYPE_COMPO_I);
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTF2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dHw	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);

    m_dH=dH	;
    m_dHw=dTw		;
    m_dTw_t=dTw_t	;
    m_dB1=dB1		;
    m_dtf1=dTF1	;
    m_dB2=dB2		;
    m_dtf2=dTF2	;
    m_dHw=dHw		;
    m_dHh=dHh		;
    m_dtc=dtc		;
    // 3. Force
    double dMy    = PltbBase.dMy;
    double dMa_Ed = PltbBase.dMa_Ed;
    double dN_Ed  = PltbBase.dN_Ed;
    double dM_Ed  = PltbBase.dM_Ed;
    BOOL	bPositiveM	=	dMy > -m_dZero ? TRUE : FALSE;

    T_PGBR_BASE PgbrBase = m_PgbrBase;
    double dMpl_Rd = PgbrBase.dMpl_Rd; // PGBR에서 결정된 값
    double dMel_Rd = PgbrBase.dMel_Rd; // PGBR에서 결정된 값

    ///////////////////////////////////////////////////////////////////////
    T_SECT_STIFFNESS StiffData;  StiffData.Initialize();
    if(!bTapered) {StiffData = SectD.SectBefore.Stiffness;}
    else			{StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;}

    // Girder Stiffness	
    double dArea_g	= StiffData.Area;
    double dIxx_g		= StiffData.Rxx;
    double dIyy_g		= StiffData.Ryy;
    double dIzz_g		= StiffData.Rzz;
    double dCzp_g		= StiffData.Czp;
    double dCzm_g		= StiffData.Czm;

    T_SECT_STIFFNESS CompoSect = bPositiveM ? RebarSect : NonConSect;
    double dArea_c = CompoSect.Area;
    double dIyy_c  = CompoSect.Ryy;
    double dCzp_c  = CompoSect.Czp;
    double dCzm_c  = CompoSect.Czm;  

    // 6.4 Lateral-torsional buckling of composite beams
    int iClassSect = ClassD.iClassSect;
    double dM_Rd	= 0.0;

    if(iClassSect == 1 || iClassSect==2)		{dM_Rd = dMpl_Rd;}
    else if(iClassSect == 3 || iClassSect==4)	{dM_Rd = dMel_Rd;}
    else ASSERT(0);
    //LTB Length

    T_CGLT_D CgltD;
    CgltD.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        m_pDoc->m_pAttrCtrl->GetCglt(ElemK.first, CgltD);
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
        T_CGLV_D CglvD;
        m_pDoc->m_pAttrCtrl->GetCglv(ElemK.first, CglvD);
        CglvD.ConvertToCglt(CgltD);
    }
    else ASSERT(0);
    
    BOOL bUseData[2];
    double dLengthData[2];
    for(int j=0; j<2; ++j)
    {
        bUseData[j] = CgltD.LTdata[j].bUseData;
        dLengthData[j]       = CgltD.LTdata[j].dl;
        //     LtbdElem.LtbdPosi[j].dCd      = CgltD.LTdata[j].dCd;           
        //     LtbdElem.LtbdPosi[j].iAlpha   = CgltD.LTdata[j].iAlpha;
        //     LtbdElem.LtbdPosi[j].da       = CgltD.LTdata[j].da;
    }

    // Lateral torsional bucking curves-General case, EN 1993-1-1, 6.3.2.2, 61p, 
    double dLamda_LT	= 0.0;
    if       (bUseData[0] == TRUE) m_dTotalLength = dLengthData[0];
    else if  (bUseData[1] == TRUE) m_dTotalLength = dLengthData[1];

    double dL			= m_dTotalLength;//MembPosi.dLu; // span length between the rigid supports.
    double db			= bPositiveM ? dB1 : dB2;
    double dtf		= bPositiveM ? dTF1 : dTF2;
    double dAwc		= 0.0;
    double dAf		= 0.0;

    // Awc는 Class 4 일 경우는 effective Area를 고려. 
    if(dCzm_c < dH-dTF1)
    { 
        dAwc	= bPositiveM ? (dH-dTF1-dCzm_c)*dTw_t : (dCzm_c-dTF2)*dTw_t;
        dAf	= bPositiveM ? dB1*dTF1 : dB2*dTF2;
    }
    else
    {
        dAwc	= bPositiveM ? 0.0 : dHw*dTw_t;
        dAf	= bPositiveM ? 0.0 : dB2*dTF2;
    }

    double dV1 = PltbBase.dV1; // member개념으로 요소의 I단 전단력
    double dV2 = PltbBase.dV2; // J단 전단력
    double dM1 = PltbBase.dM1; // I 단 모멘트
    double dM2 = PltbBase.dM2; // J 단 모멘트
    double dCd = LtbdPosi.dCd; // spring stiffness
    double dl = LtbdPosi.dl;  //distance between the springs.  
    double dcc = fabs(dl) < m_dZero ? 0.0 : dCd/dl;
    double dIzz_bf = dTF2*pow(dB2,3)/12.0; // Bottom flange, z축 단면이차모멘트, Design guide, 1993-2 참조
    double dgamma = (dEs*dIzz_bf==0.0) ? 0.0 : dcc*pow(dL,4)/(dEs*dIzz_bf);  
    double dNu = 0.0;

    if(fabs(dV1) < fabs(dV2))	{dNu = fabs(dV2) < m_dZero ? 0.0 : fabs(dV1/dV2);}
    else						{dNu = fabs(dV1) < m_dZero ? 0.0 : fabs(dV2/dV1);}
    double dPHI = Get_PHIform(dM1,dM2,dNu);

    double dm1 = 0.0, dm2 = 0.0, dm_min = 0.0, dm = 0.0;
    Get_m_Value(dNu,dPHI,dgamma,dm1,dm2,dm_min,dm);    
    // Designer's guide to EN 1994-2
    // Eurocode 4: Design of steel and composite structures, (D6.14), 102p    
    Get_NonDimensionalSlenderness(dL,db,dfy,dEs,dm,dAwc,dAf,dLamda_LT);  

    int iCase			= Get_BucklingCurveForCrossSection(dH,db, bSectI_Type);
    double dAlpha_LT	= Get_ImperfectionFactor(iCase);  
    double dPHI_LT	= Get_PHI_LT(dAlpha_LT, dLamda_LT);
    double dX_LT		= 0.0; // reduction factor for lateral-torsional buckling to the relative slenderness Lamda_LT
    double dX_LT_org	= 0.0;
    dX_LT = Get_X_LT(dPHI_LT,dLamda_LT,dX_LT_org);

    // design buckling resistance moment
    double dMb_Rd = dX_LT*dM_Rd;

    // N_Ed 만 작용하는 것으로 게산한다. Designer's guide to EN 1994-2, Example 6.6 참조   
    double dm_N   = 1.0; //
    double dAwc_N = dHw*dTw_t;
    double dAf_N  = dAf;
    double dLamda_LT_N = 0.0;  
    Get_NonDimensionalSlenderness(dL,db,dfy,dEs,dm_N,dAwc_N,dAf_N,dLamda_LT_N);  
    double dPHI_LT_N = Get_PHI_LT(dAlpha_LT, dLamda_LT_N);
    double dX_LT_N_org=0.0;
    double dX_LT_N = Get_X_LT(dPHI_LT_N, dLamda_LT_N, dX_LT_N_org);
    double dNb_Rd = dX_LT_N*dArea_c*dfyd;
    //
    double dN_Rat = fabs(dNb_Rd) < m_dZero ? 0.0 : fabs(dN_Ed)/dNb_Rd;
    double dM_Rat = fabs(dMb_Rd) < m_dZero ? 0.0 : fabs(dM_Ed)/dMb_Rd;
    double dCombRat = dN_Rat + dM_Rat;

    int iAlpha		= LtbdPosi.iAlpha;
    double dEa		= dEs; // girder
    double dI2		= CompoSect.Ryy; // girder+steel
    double dv_a		= dPoissonS; // Poisson's ratio for Steel
    double dhs		= dH-0.5*(dTF1+dTF2);
    double da			= LtbdPosi.da;
    double dk1		= fabs(da) < m_dZero ? 0.0 : double(iAlpha)*dEa*dI2/da;
    double dk2_val1	= 4.0*(1.0-dv_a*dv_a)*dhs;
    double dk2		= fabs(dk2_val1) < m_dZero ? 0.0 : dEa*pow(dTw,3)/dk2_val1;
    double dks		= fabs(dk1 + dk2) < m_dZero ? 0.0 : dk1*dk2/(dk1+dk2);

    //
    double dC4		= PltbBase.dC4;
    double dI_at		= dIxx_g; // St.Venant torsional stiffness of the steel section  
    double dI_afz		= dTF2*pow(dB2,3)/12.0; // ???,  is the flexural stiffness of bottom flange respect the z axis of the steel section
    double dAa		= dArea_g;
    double dA			= RebarSect.Area; // 전체환산단면.
    double dIay		= dIyy_g;
    double dIaz		= dIzz_g;
    double dix2		= dAa < m_dZero ? 0.0 : (dIay+dIaz)/dAa;
    double dhc		= dH-0.5*(dTF1+dTF2);
    double dSCzm_g	= Get_ShearCenter_H(dHw,dTF1,dB1,dTF2,dB2); // shear center
    double dZs		= dCzm_g - dSCzm_g; // is the distance between section centre of gravity _T("C") and shear barycentre(positive if shear barycentre is lower than _T("C"))
    double dZc		= dH+dHh+0.5*dtc-dCzm_g;
    double de			= fabs(dAa*dZc*(dA-dAa))< m_dZero ? 0.0 : dA*dIay/(dAa*dZc*(dA-dAa));
    double dZf		= dIaz < m_dZero ? 0.0 : dhs*dI_afz/dIaz;
    double dZj_val2	= Get_Zj_Second_Value(dH,dHw,dTw,dB1,dTF1,dB2,dTF2,dCzm_g,dAa,dIay);
    double dZj		= dZs - dZj_val2; 

    m_dArea = dArea_g;
    //
    if(dI_afz > 0.5*dIaz)
    {
        dZj = fabs(dIaz) < m_dZero ? 0.0 : 0.4*dhs*(2.0*dI_afz/dIaz-1.0);
    }
    else
    {    
        dZj = dZs - dZj_val2;
    }  

    double dkc_val1	= dIay < m_dZero		? 0.0 : dhs*dIyy_c/dIay;
    double dkc_val2	= fabs(de) < m_dZero	? 0.0 : (pow((dZf-dZs),2)+dix2)/de+2.0*(dZf-dZj);
    double dkc		= fabs(dkc_val2) < m_dZero  ? 0.0 : dkc_val1/dkc_val2;

    double dMcr_val = (dGs*dI_at+dks*dL*dL/(m_dPi*m_dPi))*dEa*dI_afz;
    double dMcr = fabs(dL) < m_dZero? 0.0 : dkc*dC4/dL*sqrt(dMcr_val);

    //FOR IRC calculation
    double dIt = ((dB1*dTF1*dTF1*dTF1) + (dB2*dTF2*dTF2*dTF2) + (dHw*dTw*dTw*dTw))/3.0 ;
    double dMcrIRC = sqrt((m_dPi*m_dPi*dEs*dIyy_g/(dL*dL))*(77000.0*dIt + ((m_dPi*m_dPi*dEs*StiffData.dIw/(dL*dL)))));

    double dLamdaLtIRC= sqrt(1.0*dIxx_g*m_dfy /dMcrIRC );


    // Save Lateral Torsional Buckling Check Result
    PltbBase.bOK			= dCombRat > 1.0 ? FALSE : TRUE;
    PltbBase.iSectClass	= ClassD.iClassSect;  
    PltbBase.dMpl_Rd		= dMpl_Rd;       
    PltbBase.dMel_Rd		= dMel_Rd;       
    PltbBase.dNb_Rd		= dNb_Rd;        
    PltbBase.dMb_Rd		= dMb_Rd;        
    PltbBase.dInterRat	= dCombRat;   
    PltbBase.dMcr			= dMcr;           
    PltbBase.dRatio		= dCombRat;
    PltbBase.dM_Rd		= dM_Rd;         // M_Rd               [Unit=Moment]
    PltbBase.dcc			= dcc;           // Cd/l               [Unit=Stress]
    PltbBase.dgamma		= dgamma;        //                    [Unit=None]
    PltbBase.dMu			= dNu;           // V1/V2 or V1/V2     [Unit=None] 위에는 dNu라고 되어 있음.
    PltbBase.dPHI			= dPHI;          //                    [Unit=None]
    PltbBase.dm1			= dm1;
    PltbBase.dm2			= dm2;           //                    [Unit=None]
    PltbBase.dm_min		= dm_min;
    PltbBase.dm			= dm;            //                    [Unit=None]
    PltbBase.dAwc			= dAwc;          //                    [Unit=Area]
    PltbBase.dAf			= dAf;           //                    [Unit=Area]
    PltbBase.dLamda_LT	= dLamda_LT;     //                    [Unit=None]
    PltbBase.dAlpha_LT	= dAlpha_LT;     //                    [Unit=None]
    PltbBase.dPHI_LT		= dPHI_LT;       //                    [Unit=None]
    PltbBase.dX_LT_org	= dX_LT_org;     //                    [Unit=None]
    PltbBase.dX_LT		= dX_LT;         //                    [Unit=None]
    PltbBase.dX_LT_N		= dX_LT_N;
    PltbBase.dk1			= dk1;
    PltbBase.dk2			= dk2;
    PltbBase.dks			= dks;           //                    [Unit=None]
    PltbBase.dI_at		= dI_at;         //                    [Unit=L^4]
    PltbBase.dI_afz		= dI_afz;        //                    [Unit=L^4]
    PltbBase.dkc			= dkc;

    if(IsPrintMode())
    {    
        PltbDetail.dMpl_Rd	= dMpl_Rd;
        PltbDetail.dMel_Rd	= dMel_Rd;
        PltbDetail.dL			= m_dTotalLength;
        PltbDetail.dhs		= dhs;
        PltbDetail.da			= da;
        PltbDetail.dEa		= dEa;
        PltbDetail.dGs		= dGs;
        PltbDetail.dI2		= dI2;
        PltbDetail.iAlpha		= iAlpha;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_LateralTorsionalBucklingResistance_Neg(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem,
                                                                      CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,
                                                                      CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, CPG_LTBD_POSI& LtbdPosi, 
                                                                      CPG_MEMB_POSI& MembPosi, T_CLAS_BASE& ClassD, T_PLTB_BASE& PltbBase, 
                                                                      T_PLTB_DETAIL& PltbDetail,double dmaed)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dEs		= MatdD.Data1.Analysis.Elast;
    double dPoissonS	= MatdD.Data1.Analysis.Poisson;
    double dGs		= 77000.0;//dEs/(1.0+dPoissonS);
    double dgam_M0	= m_dGamma_M0;
    double dfyd		= fabs(dgam_M0) < m_dZero ? dfy : dfy/dgam_M0;

    // 2. Section.
    T_SECT_D& SectD	= SectElem.SectD;
    BOOL bTapered		= m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_STIFFNESS NonConSect, RebarSect;	  NonConSect.Initialize();	RebarSect.Initialize();
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    // Get Section Dimension
    BOOL bSectI_Type = (m_pDgnSectUtil->Get_RealSectType(&SectD) == D_SECT_TYPE_COMPO_I);
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTF2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dHw	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);

    m_dH=dH	;
    m_dHw=dTw		;
    m_dTw_t=dTw_t	;
    m_dB1=dB1		;
    m_dtf1=dTF1	;
    m_dB2=dB2		;
    m_dtf2=dTF2	;
    m_dHw=dHw		;
    m_dHh=dHh		;
    m_dtc=dtc		;
    // 3. Force
    double dMy    = PltbBase.dMy;
    double dMa_Ed = dmaed;
    double dN_Ed  = PltbBase.dN_Ed;
    double dM_Ed  = PltbBase.dM_Ed;
    BOOL	bPositiveM	=	dMy > -m_dZero ? TRUE : FALSE;

    T_PGBR_BASE PgbrBase = m_PgbrBase;
    double dMpl_Rd = PgbrBase.dMpl_Rd; // PGBR에서 결정된 값
    double dMel_Rd = PgbrBase.dMel_Rd; // PGBR에서 결정된 값

    ///////////////////////////////////////////////////////////////////////
    T_SECT_STIFFNESS StiffData;  StiffData.Initialize();
    if(!bTapered) {StiffData = SectD.SectBefore.Stiffness;}
    else			{StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;}

    // Girder Stiffness	
    double dArea_g	= StiffData.Area;
    double dIxx_g		= StiffData.Rxx;
    double dIyy_g		= StiffData.Ryy;
    double dIzz_g		= StiffData.Rzz;
    double dCzp_g		= StiffData.Czp;
    double dCzm_g		= StiffData.Czm;

    T_SECT_STIFFNESS CompoSect = bPositiveM ? RebarSect : NonConSect;
    double dArea_c = CompoSect.Area;
    double dIyy_c  = CompoSect.Ryy;
    double dCzp_c  = CompoSect.Czp;
    double dCzm_c  = CompoSect.Czm;  

    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // 6.4 Lateral-torsional buckling of composite beams
    int iClassSect = ClassD.iClassSect;
    double dM_Rd	= 0.0;

    if(iClassSect == 1 || iClassSect==2)		{dM_Rd = dMpl_Rd;}
    else if(iClassSect == 3 || iClassSect==4)	{dM_Rd = dMel_Rd;}
    else ASSERT(0);

    // Lateral torsional bucking curves-General case, EN 1993-1-1, 6.3.2.2, 61p, 
    double dLamda_LT	= 0.0;
    double dL			= m_dTotalLength;//MembPosi.dLu; // span length between the rigid supports.
    double db			= bPositiveM ? dB1 : dB2;
    double dtf		= bPositiveM ? dTF1 : dTF2;
    double dAwc		= 0.0;
    double dAf		= 0.0;

    // Awc는 Class 4 일 경우는 effective Area를 고려. 
    if(dCzm_c < dH-dTF1)
    { 
        dAwc	= bPositiveM ? (dH-dTF1-dCzm_c)*dTw_t : (dCzm_c-dTF2)*dTw_t;
        dAf	= bPositiveM ? dB1*dTF1 : dB2*dTF2;
    }
    else
    {
        dAwc	= bPositiveM ? 0.0 : dHw*dTw_t;
        dAf	= bPositiveM ? 0.0 : dB2*dTF2;
    }

    double dV1 = PltbBase.dV1; // member개념으로 요소의 I단 전단력
    double dV2 = PltbBase.dV2; // J단 전단력
    double dM1 = PltbBase.dM1; // I 단 모멘트
    double dM2 = PltbBase.dM2; // J 단 모멘트
    double dCd = LtbdPosi.dCd; // spring stiffness
    double dl = LtbdPosi.dl;  //distance between the springs.  
    double dcc = fabs(dl) < m_dZero ? 0.0 : dCd/dl;
    double dIzz_bf = dTF2*pow(dB2,3)/12.0; // Bottom flange, z축 단면이차모멘트, Design guide, 1993-2 참조
    double dgamma = (dEs*dIzz_bf==0.0) ? 0.0 : dcc*pow(dL,4)/(dEs*dIzz_bf);  
    double dNu = 0.0;

    if(fabs(dV1) < fabs(dV2))	{dNu = fabs(dV2) < m_dZero ? 0.0 : fabs(dV1/dV2);}
    else						{dNu = fabs(dV1) < m_dZero ? 0.0 : fabs(dV2/dV1);}
    double dPHI = Get_PHIform(dM1,dM2,dNu);

    double dm1 = 0.0, dm2 = 0.0, dm_min = 0.0, dm = 0.0;
    Get_m_Value(dNu,dPHI,dgamma,dm1,dm2,dm_min,dm);    
    // Designer's guide to EN 1994-2
    // Eurocode 4: Design of steel and composite structures, (D6.14), 102p    
    Get_NonDimensionalSlenderness(dL,db,dfy,dEs,dm,dAwc,dAf,dLamda_LT);  

    int iCase			= Get_BucklingCurveForCrossSection(dH,db, bSectI_Type);
    double dAlpha_LT	= Get_ImperfectionFactor(iCase);  
    double dPHI_LT	= Get_PHI_LT(dAlpha_LT, dLamda_LT);
    double dX_LT		= 0.0; // reduction factor for lateral-torsional buckling to the relative slenderness Lamda_LT
    double dX_LT_org	= 0.0;
    dX_LT = Get_X_LT(dPHI_LT,dLamda_LT,dX_LT_org);

    // design buckling resistance moment
    double dMb_Rd = dX_LT*dM_Rd;

    // N_Ed 만 작용하는 것으로 게산한다. Designer's guide to EN 1994-2, Example 6.6 참조   
    double dm_N   = 1.0; //
    double dAwc_N = dHw*dTw_t;
    double dAf_N  = dAf;
    double dLamda_LT_N = 0.0;  
    Get_NonDimensionalSlenderness(dL,db,dfy,dEs,dm_N,dAwc_N,dAf_N,dLamda_LT_N);  
    double dPHI_LT_N = Get_PHI_LT(dAlpha_LT, dLamda_LT_N);
    double dX_LT_N_org=0.0;
    double dX_LT_N = Get_X_LT(dPHI_LT_N, dLamda_LT_N, dX_LT_N_org);
    double dNb_Rd = dX_LT_N*dArea_c*dfyd;
    //
    double dN_Rat = fabs(dNb_Rd) < m_dZero ? 0.0 : fabs(dN_Ed)/dNb_Rd;
    double dM_Rat = fabs(dMb_Rd) < m_dZero ? 0.0 : fabs(dM_Ed)/dMb_Rd;
    double dCombRat = dN_Rat + dM_Rat;

    int iAlpha		= LtbdPosi.iAlpha;
    double dEa		= dEs; // girder
    double dI2		= CompoSect.Ryy; // girder+steel
    double dv_a		= dPoissonS; // Poisson's ratio for Steel
    double dhs		= dH-0.5*(dTF1+dTF2);
    double da			= LtbdPosi.da;
    double dk1		= fabs(da) < m_dZero ? 0.0 : double(iAlpha)*dEa*dI2/da;
    double dk2_val1	= 4.0*(1.0-dv_a*dv_a)*dhs;
    double dk2		= fabs(dk2_val1) < m_dZero ? 0.0 : dEa*pow(dTw,3)/dk2_val1;
    double dks		= fabs(dk1 + dk2) < m_dZero ? 0.0 : dk1*dk2/(dk1+dk2);

    //
    double dC4		= PltbBase.dC4;
    double dI_at		= dIxx_g; // St.Venant torsional stiffness of the steel section  
    double dI_afz		= dTF2*pow(dB2,3)/12.0; // ???,  is the flexural stiffness of bottom flange respect the z axis of the steel section
    double dAa		= dArea_g;
    double dA			= RebarSect.Area; // 전체환산단면.
    double dIay		= dIyy_g;
    double dIaz		= dIzz_g;
    double dix2		= dAa < m_dZero ? 0.0 : (dIay+dIaz)/dAa;
    double dhc		= dH-0.5*(dTF1+dTF2);
    double dSCzm_g	= Get_ShearCenter_H(dHw,dTF1,dB1,dTF2,dB2); // shear center
    double dZs		= dCzm_g - dSCzm_g; // is the distance between section centre of gravity _T("C") and shear barycentre(positive if shear barycentre is lower than _T("C"))
    double dZc		= dH+dHh+0.5*dtc-dCzm_g;
    double de			= fabs(dAa*dZc*(dA-dAa))< m_dZero ? 0.0 : dA*dIay/(dAa*dZc*(dA-dAa));
    double dZf		= dIaz < m_dZero ? 0.0 : dhs*dI_afz/dIaz;
    double dZj_val2	= Get_Zj_Second_Value(dH,dHw,dTw,dB1,dTF1,dB2,dTF2,dCzm_g,dAa,dIay);
    double dZj		= dZs - dZj_val2; 

    m_dArea = dArea_g;
    //
    if(dI_afz > 0.5*dIaz)
    {
        dZj = fabs(dIaz) < m_dZero ? 0.0 : 0.4*dhs*(2.0*dI_afz/dIaz-1.0);
    }
    else
    {    
        dZj = dZs - dZj_val2;
    }  

    double dkc_val1	= dIay < m_dZero		? 0.0 : dhs*dIyy_c/dIay;
    double dkc_val2	= fabs(de) < m_dZero	? 0.0 : (pow((dZf-dZs),2)+dix2)/de+2.0*(dZf-dZj);
    double dkc		= fabs(dkc_val2) < m_dZero  ? 0.0 : dkc_val1/dkc_val2;

    double dMcr_val = (dGs*dI_at+dks*dL*dL/(m_dPi*m_dPi))*dEa*dI_afz;
    double dMcr = fabs(dL) < m_dZero? 0.0 : dkc*dC4/dL*sqrt(dMcr_val);

    //FOR IRC calculation
    double dzpe = (dTw*dHw*dHw)/4.0 + dB1*dTF1*(dCzm_g - 0.5*dTF1) + dB2*dTF2*(dCzm_g - 0.5*dTF2) ;
    double dIt = ((dB1*dTF1*dTF1*dTF1) + (dB2*dTF2*dTF2*dTF2) + (dHw*dTw*dTw*dTw))/3.0 ;
    double dMcrIRC = sqrt((m_dPi*m_dPi*dEs*SectD.SectBefore.Stiffness.Ryy/(dL*dL))*(77000.0*dIt + ((m_dPi*m_dPi*dEs*StiffData.dIw/(dL*dL)))));
    dMcr=dMcrIRC;
    double dLamdaLtIRC= sqrt(1.0*dzpe*m_dfy /dMcrIRC );
    dLamda_LT=dLamdaLtIRC;
    dAlpha_LT =  Get_PHI_LT(dAlpha_LT, dLamda_LT_N);
    dX_LT = Get_X_LT(dAlpha_LT,dLamda_LT,dX_LT_org);

    // Save Lateral Torsional Buckling Check Result
    PltbBase.bOK			= dCombRat > 1.0 ? FALSE : TRUE;
    PltbBase.iSectClass	= ClassD.iClassSect;  
    PltbBase.dMpl_Rd		= dMpl_Rd;       
    PltbBase.dMel_Rd		= dMel_Rd;       
    PltbBase.dNb_Rd		= dNb_Rd;        
    PltbBase.dMb_Rd		= dMb_Rd;        
    PltbBase.dInterRat	= dCombRat;   
    PltbBase.dMcr			= dMcr;           
    PltbBase.dRatio		= dCombRat;
    PltbBase.dM_Rd		= dM_Rd;         // M_Rd               [Unit=Moment]
    PltbBase.dcc			= dcc;           // Cd/l               [Unit=Stress]
    PltbBase.dgamma		= dgamma;        //                    [Unit=None]
    PltbBase.dMu			= dNu;           // V1/V2 or V1/V2     [Unit=None] 위에는 dNu라고 되어 있음.
    PltbBase.dPHI			= dPHI;          //                    [Unit=None]
    PltbBase.dm1			= dm1;
    PltbBase.dm2			= dm2;           //                    [Unit=None]
    PltbBase.dm_min		= dm_min;
    PltbBase.dm			= dm;            //                    [Unit=None]
    PltbBase.dAwc			= dAwc;          //                    [Unit=Area]
    PltbBase.dAf			= dAf;           //                    [Unit=Area]
    PltbBase.dLamda_LT	= dLamda_LT;     //                    [Unit=None]
    PltbBase.dAlpha_LT	= dAlpha_LT;     //                    [Unit=None]
    PltbBase.dPHI_LT		= dPHI_LT;       //                    [Unit=None]
    PltbBase.dX_LT_org	= dX_LT_org;     //                    [Unit=None]
    PltbBase.dX_LT		= dX_LT;         //                    [Unit=None]
    PltbBase.dX_LT_N		= dX_LT_N;
    PltbBase.dk1			= dk1;
    PltbBase.dk2			= dk2;
    PltbBase.dks			= dks;           //                    [Unit=None]
    PltbBase.dI_at		= dI_at;         //                    [Unit=L^4]
    PltbBase.dI_afz		= dI_afz;        //                    [Unit=L^4]
    PltbBase.dkc			= dkc;
    PltbBase.dMa_Ed   = dmaed ;
    if(IsPrintMode())
    {    
        PltbDetail.dMpl_Rd	= dMcrIRC;
        PltbDetail.dMel_Rd	= dLamdaLtIRC;
        PltbDetail.dL			= m_dTotalLength;
        PltbDetail.dhs		= dX_LT;
        PltbDetail.da			= dmaed;
        PltbDetail.dEa		= PgbrBase.dNa_botc*min(dfyd_t,min(dfyd_b,dfyd_w));
        PltbDetail.dGs		= dLamda_LT;
        PltbDetail.dI2		= dAlpha_LT;
        PltbDetail.iAlpha		= iAlpha;
    }

    return TRUE;
}


double CDgnPlateGirder_IRC::Get_Zj_Second_Value(double dH, double dHw, double dtw, double dB1, double dtf1, double dB2, double dtf2, 
                                                double dCzm, double dAa, double dIay)
{
    double dVal = 0.0;  
    if(dIay < m_dZero) {return dVal;}
    // H형 일 경우만.

    // integral( z*y^2 dA )
    double dA_tf = dB1*dtf1;
    double dA_bf = dB2*dtf2;
    double dIy_tf = dB1*pow(dtf1,3)/12.0;
    double dIy_bf = dB2*pow(dtf2,3)/12.0;
    double dZ_tf_cm = (dH-0.5*dtf1)-dCzm;
    double dZ_bf_cm = dCzm-0.5*dtf2;
    double dIntegral_zy2_tf = (dIy_tf+dA_tf*pow(dZ_tf_cm,2))*dZ_tf_cm;
    double dIntegral_zy2_bf = (dIy_bf+dA_bf*pow(dZ_bf_cm,2))*dZ_bf_cm;
    double dIntegral_dzy2 = dIntegral_zy2_tf +  dIntegral_zy2_bf;

    // integral( z*z^2 dA )  
    double dIntegral_z3_tf = 2.0*dtf1*pow(0.5*dB1,4); // 2*[tf1*z^4/4]^(dB1/2)
    double dIntegral_z3_bf = 2.0*dtf2*pow(0.5*dB2,4);
    double dIntegral_z3_w  = 2.0*dHw*pow(0.5*dtw,4);
    double dIntegral_z3 = dIntegral_z3_tf + dIntegral_z3_bf + dIntegral_z3_w;

    dVal = dAa/(2.0*dIay)*(dIntegral_dzy2 + dIntegral_z3);

    return dVal;
}

double CDgnPlateGirder_IRC::Get_ShearCenter_H(double dh, double dt1, double db1, double dt2, double db2)
{  
    // h = top flange 중심과 bottom flange 중심 사이의 거리 
    // h2 = bottom flange 중심에서 전단중심 사이의 거리
    double dh2_val1 = dt1*pow(db1,3)*dh;
    double dh2_val2 = dt1*pow(db1,3) + dt2*pow(db2,3);
    double dh2 = dh2_val2 < m_dZero ? 0.0 : dh2_val1/dh2_val2;

    double dZsc = dh2 + 0.5*dt2;
    return dh2;  
}

double CDgnPlateGirder_IRC::Get_fy4Thick(double dfy, double dfy1, double dt)
{
    double dfy4thick = dt<40.0 || fabs(dt-40.0)<m_dZero ? dfy : dfy1;
    return dfy4thick;
}

BOOL CDgnPlateGirder_IRC::Get_WidthThickRatio4Web(BOOL bStiffened, double deta, double dhw, double dtw, double dEpsi, double dk_tau, double& dCheck_lim, BOOL& bCheck)
{
    if(dtw < m_dZero|| fabs(deta) < m_dZero || dk_tau < 0.0) {return FALSE;}
    double dVal_lim=0.0;

    if(!bStiffened)
    {
        dVal_lim = 67.0*dEpsi; //IRC 24 2010 Cl 509.4.2.1 pg.92
    }
    else
    {
        dVal_lim = 67.0*dEpsi*sqrt(dk_tau/5.35);
    }
    dCheck_lim = dVal_lim;
    double dWTRatio = dtw <  m_dZero ? 0.0 : dhw/dtw;

    if(dWTRatio > dVal_lim)	{bCheck = TRUE;	}
    else						{bCheck = FALSE;}

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_Vbw_Rd(double dXw, double dfyw, double dAvw, double dGamma_M1)
{
    if(dGamma_M1<=m_dZero) {return 0.0;}
    double dVbw_Rd = dXw*dfyw*dAvw/(sqrt(3.0)*dGamma_M1);
    return dVbw_Rd;
}

double CDgnPlateGirder_IRC::Get_Vbw_Rd_Limit(double dEta, double dfyw, double dAvw, double dGamma_M1)
{
    if(dGamma_M1<=m_dZero) {return 0.0;}
    double dVb_Rd_Limit = dEta*dfyw*dAvw/(sqrt(3.0)*dGamma_M1);
    return dVb_Rd_Limit;
}

double CDgnPlateGirder_IRC::Get_tau_b_Rd(double dX, double dfyf, double dGamma_M1)
{
    if(dGamma_M1<=m_dZero) {return 0.0;}
    double dVbw_Rd = dX*dfyf/(sqrt(3.0)*dGamma_M1);
    return dVbw_Rd;
}

double CDgnPlateGirder_IRC::Get_ReductonFactorOfFlange4N_Ed(double dGamma_M2, double dfy_f1, double dfy_f2, double dN_Ed, double dAf1, double dAf2)
{
    // Modify by GAY. PMS:4216. ('12.05.11). 압축인 경우만 계산하며, 계산할 때는 절대값으로 고려.
    if(dN_Ed > 0.0) { ASSERT(0); return 1.0; }

    double dFactor_param = (dAf1*dfy_f1 + dAf2*dfy_f2)/dGamma_M2;
    double dfactor = fabs(dFactor_param) < m_dZero ? 0.0 : 1.0-fabs(dN_Ed)/dFactor_param;
    return dfactor;
}

int CDgnPlateGirder_IRC::Get_Lamda_w_Type4Shear(BOOL bLongStiff, CPG_TRST_POSI& TrstPosi)
{
    int iType=0;  
    if(TrstPosi.bUseStif)
    {
        BOOL bInterTransverse=TRUE; // 사용했으면 보강된 것으로 
        //if(TrstPosi.bInterNonRigidStif || TrstPosi.bInterRigidStif) bInterTransverse=TRUE;  // intermediate transverse stiffener가 있다고 봄. 
        if(bInterTransverse/* || bLongStiff*/) iType = 2; // EN 1993-1-5, 5.3(3), b) // MQC 11106 Transverse Stiff있는 경우는 모두 이 케이스로 고려
        //else iType = 1;    
    }
    else
    {
        /*if(bLongStiff) */iType = 1;  // 그 외에는 모두 EN 1993-1-5, 5.3(3), a)식으로 고려
    }

    return iType;
}

double CDgnPlateGirder_IRC::Get_Alpha4Lamda_w(double da, double dhw)
{
    double dAlpha = dhw < m_dZero ? 0.0 : da/dhw;
    return dAlpha;
}

BOOL CDgnPlateGirder_IRC::IsMaterialS420orS460(double dfy)
{
    BOOL bMatOK	= FALSE;
    BOOL bS420	= fabs(dfy-420.0) < m_dZero ? TRUE : FALSE;
    BOOL bS460	= fabs(dfy-460.0) < m_dZero ? TRUE : FALSE;

    if(bS420 || bS460) bMatOK = TRUE;
    return bMatOK;
}

double CDgnPlateGirder_IRC::Get_RatioOfInteractionMV(double deta1, double dBendingRat, double deta3)
{
    double dRatio = deta1+(1.0-dBendingRat)*pow((2.0*deta3-1.0),2);
    return dRatio;
}

BOOL CDgnPlateGirder_IRC::GetStressInfo(bool bPosiI, const T_SECT_D& crSectD, double aGirderForce[6], double aCptForce[6], T_SECT_STIFFNESS& GdStiffD, T_SECT_STIFFNESS& StiffD, T_STRESS_INFO_I& rData)
{
    double dArea_a = GdStiffD.Area;
    double dIy_a   = GdStiffD.Ryy;
    double dIz_a   = GdStiffD.Rzz;
    double dArea   = StiffD.Area;
    double dIy     = StiffD.Ryy;
    double dIz     = StiffD.Rzz;

    double dFx_a = aGirderForce[0];
    double dMy_a = aGirderForce[4];
    double dMz_a = aGirderForce[5];
    double dFx_c = aCptForce[0];
    double dMy_c = aCptForce[4];
    double dMz_c = aCptForce[5];

    double aStress_Pos[4]={0.0}; // 0=TL, 1=TR, 2 = BR, 3 = BL
    Get_StressOfPosition(aGirderForce, aCptForce, GdStiffD, StiffD, aStress_Pos);

    double dy1=0.0, dy2=0.0, dz1=0.0, dz2=0.0;
    double dy1_a=0.0, dz1_a=0.0, dy1_c=0.0, dz1_c=0.0;
    double dy2_a=0.0, dz2_a=0.0, dy2_c=0.0, dz2_c=0.0;
    double dStr_a1 = 0.0, dStr_c1 = 0.0;
    double dStr_a2=0.0, dStr_c2=0.0;
    double dStr1=0.0, dStr2=0.0;  

    T_STRESS_LINE_INFO FlgLine[4]; // 0=TL, 1=TR, 2 = BR, 3 = BL
    BOOL bTop = TRUE, bLeft = TRUE;
    for(int i = 0; i < 4; ++i)  
    {
        FlgLine[i].Initialize();
        bTop	= (i == 0 || i == 1) ? TRUE : FALSE;
        bLeft = (i == 0 || i == 3) ? TRUE : FALSE;
        GetCentFlgLinePosD(bPosiI, bTop, bLeft, crSectD, GdStiffD, dy1_a, dz1_a, dy2_a, dz2_a);
        GetCentFlgLinePosD(bPosiI, bTop, bLeft, crSectD, StiffD, dy1_c, dz1_c, dy2_c, dz2_c);
        dStr_a1 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy1_a, dz1_a);
        dStr_c1 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea,   dIy,   dIz,   dy1_c, dz1_c);
        dStr_a2 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy2_a, dz2_a);
        dStr_c2 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea,   dIy,   dIz,   dy2_c, dz2_c);
        dStr1  = dStr_a1 + dStr_c1;  
        dStr2  = dStr_a2 + dStr_c2;  
        FlgLine[i].dy1 = dy1_c;
        FlgLine[i].dz1 = dz1_c;
        FlgLine[i].dy2 = dy2_c, 
            FlgLine[i].dz2 = dz2_c;
        FlgLine[i].dStr1 = dStr1;
        FlgLine[i].dStr2 = dStr2;	  
    }

    T_STRESS_LINE_INFO WebLine[2]; // 0 = Left, 1 = Right
    int nWebSize  = m_pDgnSectUtil->Get_WebPartNum(&crSectD);

    for(int i = 0; i < nWebSize; ++i)
    {
        WebLine[i].Initialize();
        bLeft = (i == 0) ? TRUE : FALSE;
        GetCentWebLinePosD(bPosiI, bLeft, crSectD, GdStiffD, dy1_a, dz1_a, dy2_a, dz2_a);
        GetCentWebLinePosD(bPosiI, bLeft, crSectD, StiffD, dy1_c, dz1_c, dy2_c, dz2_c);
        dStr_a1 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy1_a, dz1_a);
        dStr_c1 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea,   dIy,   dIz,   dy1_c, dz1_c);
        dStr_a2 = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy2_a, dz2_a);
        dStr_c2 = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea,   dIy,   dIz,   dy2_c, dz2_c);
        dStr1  = dStr_a1 + dStr_c1;
        dStr2  = dStr_a2 + dStr_c2;  
        WebLine[i].dy1 = dy1_c;
        WebLine[i].dz1 = dz1_c;
        WebLine[i].dy2 = dy2_c, 
            WebLine[i].dz2 = dz2_c;
        WebLine[i].dStr1 = dStr1;
        WebLine[i].dStr2 = dStr2;	 
    }

    rData.FlgTL	= FlgLine[0];
    rData.FlgTR	= FlgLine[1];
    rData.Web		= WebLine[0];
    rData.FlgBL	= FlgLine[3];
    rData.FlgBR	= FlgLine[2];  
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_TransverseForceResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem,
                                                         CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,  
                                                         CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, 
                                                         CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, CPG_BCDT_POSI& BcdtPosi,
                                                         T_CLAS_BASE& ClassD, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail,CPG_SCON_POSI& SconPosi)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dEs		= MatdD.Data1.Analysis.Elast;
    double dgam_M0	= m_dGamma_M0;
    double dgam_M1	= m_dGamma_M1;

    // 2. Section.
    T_SECT_D& SectD	 = SectElem.SectD;
    T_SECT_STIFFNESS RebarSect; RebarSect.Initialize();
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    // Dimension
    bool bBoxSect	= m_pDgnSectUtil->Is_StlBoxSect(&SectD);
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dfyt	= dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw	= dTw < 40.0 ? dfy : dfy1;
    double dfyd	= fabs(dgam_M0) < m_dZero ? dfy : dfy/dgam_M0;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    const T_SECT_STIFFNESS& EffeStlSect = m_EffeStlSect;
    const T_SECT_STIFFNESS& EffeComSect = m_EffeComSect;  

    // 3. Force
    double dN_Ed  = PrtfBase.dFx;
    double dMy_Ed = PrtfBase.dMy;
    double dMz_Ed = PrtfBase.dMz;
    double dF_Ed	= PrtfBase.dF_Ed;

    // Before
    double dStl_Iyy  = EffeStlSect.Ryy;
    double dStl_Czm  = EffeStlSect.Czm;
    double dStl_Czp  = EffeStlSect.Czp;
    double dStl_zDim = dStl_Czp+dStl_Czm;
    double dStl_Stop = dStl_Czp;
    // Composite
    double dIyy	= EffeComSect.Ryy;  
    double dIzz	= EffeComSect.Rzz;  
    double dCzm	= EffeComSect.Czm;
    double dCzp	= EffeComSect.Czp;
    double dzDim	= dCzm + dCzp;
    double dStop	= dH - dCzm;
    double dy1 = EffeComSect.dx1;   double dz1 = EffeComSect.dy1;
    double dy2 = EffeComSect.dx1;	  double dz2 = EffeComSect.dy1;
    double dy3 = EffeComSect.dx3;	  double dz3 = EffeComSect.dy3;
    double dy4 = EffeComSect.dx4;	  double dz4 = EffeComSect.dy4;

    double dWy_eff[4] = {0.0, 0.0, 0.0, 0.0}, dWz_eff[4] = {0.0, 0.0, 0.0, 0.0}; // 0=TL, 1=TR, 2=BR, 3=BL
    dWy_eff[0] = fabs(dy1) < m_dZero ? 0.0 : dIyy/dz1;  dWz_eff[0] = fabs(dz1) < m_dZero ? 0.0 : dIzz/dy1;
    dWy_eff[1] = fabs(dy2) < m_dZero ? 0.0 : dIyy/dz2;  dWz_eff[1] = fabs(dz2) < m_dZero ? 0.0 : dIzz/dy2;
    dWy_eff[2] = fabs(dy3) < m_dZero ? 0.0 : dIyy/dz3;  dWz_eff[2] = fabs(dz3) < m_dZero ? 0.0 : dIzz/dy3;
    dWy_eff[3] = fabs(dy4) < m_dZero ? 0.0 : dIyy/dz4;  dWz_eff[3] = fabs(dz4) < m_dZero ? 0.0 : dIzz/dy4;

    // 1993-1-5, (4.15), 21p
    int iEta1_max_posi	= 0;
    double dAeff			= m_PgbrDetail.EffCompositeFx.Area;
    double dey_N			= RebarSect.Czm - m_PgbrDetail.EffCompositeFx.Czm;
    double dez_N			= RebarSect.Cym - m_PgbrDetail.EffCompositeFx.Cym;
    double dEta1[4]		= {0.0, 0.0, 0.0, 0.0};
    double dEta1_max		= 0.0;
    double dEta1_fx		= fabs(dgam_M0*dfy*dAeff) < m_dZero ? 0.0 : dN_Ed/(dfy*dAeff/dgam_M0);

    if(dgam_M0 > 0.0)
    {
        for(int i=0; i<4; i++)// 0=TL, 1=TR, 2=BR, 3=BL
        { 
            double dRat_y = fabs(dfy*dWy_eff[i]) < m_dZero ? 0.0 : (-1.)*(dMy_Ed+dN_Ed*dey_N)/(dfy*dWy_eff[i]/dgam_M0);
            double dRat_z = fabs(dfy*dWz_eff[i]) < m_dZero ? 0.0 : (-1.)*(dMz_Ed+dN_Ed*dez_N)/(dfy*dWz_eff[i]/dgam_M0);  

            // Compression(-), Tension(+)
            dEta1[i] = dEta1_fx + dRat_y; // + dRat_z;   // 수정. [4/25/2012] z방향 텀은 빼기로 함. 이대근 BJ님 동의  
            if(i==0)
            {
                dEta1_max = fabs(dEta1[0]);
                iEta1_max_posi = i;
            }
            else
            {
                if(dEta1_max < fabs(dEta1[i]))
                {
                    dEta1_max = fabs(dEta1[i]);
                    iEta1_max_posi = i;
                }
            }
        }
    }

    // EN 1993-1-5, 26page.
    T_DGN_STL_STIFFENER_PART WebStiffD;
    m_pDgnSectUtil->Get_Web_StiffenerD(true, &SectD, WebStiffD);
    Get_ConvertSectStiffenerD(WebStiffD, arLstiPosi);
    BOOL bIsStiffener = arLstiPosi.GetSize() > 0;
    int iLoadAppType	= BcdtPosi.iLoadAppType;
    double da			= BcdtPosi.da;
    double dSs		= BcdtPosi.dSs;
    double dC			= BcdtPosi.dC;
    double dEpsi		= Get_Epsilon(dfyw);
    double dIsl1		= 0.0;
    double db1		= 0.0;
    double dk_F		= 0.0;  

    if(bIsStiffener)
    {
        const CPG_LSTI_DATA& LstiD = arLstiPosi[0];
        double dI_ls = 0.0;
        db1 = LstiD.dz-LstiD.dt*0.5;

        if(!Get_IslOfLongiStiff(LstiD, dTw, dEpsi, dIsl1)){dIsl1=0.0;}
    }  

    // Modify by GAY. PMS:4216. ('12.05.15). 계산서 출력을 위한 Gamma_s_limit 적용전 계산값 저장.
    int ik_F_calc_type = 0;  // 0=Type(a), 1=Type(b), 2=Type(c)
    double dGamma_s = 0.0, dGamma_s_lim = 0.0, dGamma_s_cal = 0.0;

    if(iLoadAppType == 0)
    {
        double dRat		= da < m_dZero ? 0.0 : dh/da;
        double db1hw	= dh < m_dZero ? 0.0 : db1/dh;
        double db1a		= da < m_dZero ? 0.0 : db1/da;

        if((0.05 - m_dZero < db1hw && db1hw < 0.3 + m_dZero) && (db1a < 0.3 +m_dZero))
        {
            dGamma_s_cal	= (dh*dTw < m_dZero) ? 0.0 : 10.9*dIsl1/(dh*pow(dTw,3));
            dGamma_s_lim	= (dh*da < m_dZero)  ? 0.0 : 13.0*pow(da/dh,3)+210*(0.3-db1/da); 
            dGamma_s		= dGamma_s_cal < dGamma_s_lim ? dGamma_s_cal : dGamma_s_lim;
            dk_F			= 6.0+2.0*pow(dh/da,2)+(5.44*db1/da-0.21)*sqrt(dGamma_s);
            ik_F_calc_type= 1;
        }
        else
        {
            dk_F = 6.0+2.0*pow(dRat,2);
            ik_F_calc_type=2;
        }
    }
    else if(iLoadAppType==1)
    {
        double dRat = da < m_dZero ? 0.0 : dh/da;
        dk_F = 3.5+2.0*pow(dRat,2);
    }
    else if(iLoadAppType==2)
    {
        double dRat = dh < m_dZero ? 0.0 : (dSs+dC)/dh;
        dk_F = 2.0+6.0*dRat;
        if(dk_F > 6.0){dk_F = 6.0;}
    }
    else ASSERT(0);

    //!/ (6.3) 과 (6.9) 식에 모순이 있다.
    // effective loaded length
    double dbf	= dB1;
    if(bBoxSect)
    {
        double dbf_LR[2] = {0.0, 0.0};
        Get_bf4c_DB(bPosiI, SectD, true, 0, dEpsi, dbf_LR[0]);
        Get_bf4c_DB(bPosiI, SectD, true, 1, dEpsi, dbf_LR[1]);
        dbf = dbf_LR[0] + dbf_LR[1];
    }

    double dm1	= dfyw*dTw < m_dZero ? 0.0 : dfyt*dbf/(dfyw*dTw);
    double dRat	= dTF1 < m_dZero ? 0.0 : dh/dTF1;
    double dm2	= 0.02*pow(dRat,2);
    double dl_y = 0.0, dl_e=0.0, dl_y1=0.0, dl_y2=0.0; // effective loaded length

    if(iLoadAppType==0 || iLoadAppType==1)
    {
        dl_y = dSs + 2.0*dTF1*(1.0+sqrt(dm1+dm2));
    }
    else if(iLoadAppType==2)
    {
        dl_e = dfyw*dh < m_dZero ? 0.0 : dk_F*dEs*pow(dTw,2)/(2.0*dfyw*dh);
        if(dl_e > dSs+dC){dl_e = dSs+dC;}

        double dl_y1_val = dTF1 < m_dZero ? dm1/2.0+dm2 : dm1/2.0+pow(dl_e/dTF1,2)+dm2;
        dl_y1	= dl_e+dTF1*sqrt(dl_y1_val);
        dl_y2	= dl_e+dTF1*sqrt(dm1+dm2);
        dl_y	= min(dl_y1,dl_y2);
    }
    else ASSERT(0);

    // reduction factor
    double dFcr		= fabs(dh)		< m_dZero ? 0.0 : 0.9*dk_F*dEs*pow(dTw,3)/dh;
    double dLamda1	= fabs(dFcr)	< m_dZero ? 0.0 : dl_y*dTw*dfyw/dFcr;
    double dLamda_F	= dLamda1		< 0.0 ? 0.0 : sqrt(dLamda1);
    double dX_F		= fabs(dLamda_F) < m_dZero ? 0.0 : 0.5/dLamda_F; // reduction factor
    if(dX_F > 1.0){dX_F = 1.0;}


    double dSHear_plane_length = 2.0*SconPosi.dhsc+dB1;
    double dFrd1 = 0.632*dSHear_plane_length*sqrt(dfy);
    double dFrd2 = 0.232 * dSHear_plane_length*sqrt(dfy);	

    double dLeff		= dX_F*dl_y;
    double dF_Rd		= fabs(dgam_M1) < m_dZero ? 0.0 : max(dFrd1,dFrd2)/dgam_M1;
    double dEta2		= fabs(dF_Rd) < m_dZero  ? 0.0 : fabs(dF_Ed)/dF_Rd;
    double dInterRat	= dEta2 ; // (7.2), 29page 적용하면 수정 

    // Save Transverse Force Check Result
    PrtfBase.bOK			= dInterRat < 1.4 + m_dZero ? TRUE : FALSE;	
    PrtfBase.iSectClass	= ClassD.iClassSect;   
    PrtfBase.dF_Rd		= dF_Rd;         
    PrtfBase.dEta1		= dEta1_max;         // EN 1993-1-5, (4.14), 21page
    PrtfBase.dEta2		= dEta2;         
    PrtfBase.dInterRat	= dInterRat;     // Eta2+0.8*Eta1<=1.4  [Unit=None]  
    PrtfBase.dRatio		= dInterRat;       //
    PrtfBase.dey_N		= dey_N;         // ey,N                [Unit=Length]
    PrtfBase.dez_N		= dez_N;         // ez,N                [Unit=Length]
    PrtfBase.dIsl1		= dIsl1;         // Isl,1               [Unit=Length^4]
    PrtfBase.dgamma_s		= dGamma_s;      // gamma,s             [Unit=None]
    PrtfBase.dk_F			= dk_F;          // k,F                 [Unit=None]
    PrtfBase.db1			= db1;
    PrtfBase.dm1			= dm1;
    PrtfBase.dm2			= dm2;      // m1,m2               [Unit=None]
    PrtfBase.dly1			= dl_y1;
    PrtfBase.dly2			= dl_y2;    // ly1, ly2            [Unit=Length]
    PrtfBase.dle			= dl_e;
    PrtfBase.dly			= dl_y;      // le, ly              [Unit=Length]
    PrtfBase.dFcr			= dFcr;          // Fcr                 [Unit=Force]
    PrtfBase.dLamda_F		= dLamda_F;      // Lamda,F             [Unit=None]
    PrtfBase.dX_F			= dX_F;          // X,F                 [Unit=None]
    PrtfBase.dL_eff		= dSHear_plane_length;        // L,eff               [Unit=Length]
    for(int k=0; k<4; k++){PrtfBase.dEta1_p[k] = dEta1[k];}    // eta1, 4지점 [Unit=None]

    if(IsPrintMode())
    {
        PrtfDetail.iEta1_max_posi	= iEta1_max_posi;
        PrtfDetail.dWy_eff			= dWy_eff[iEta1_max_posi];
        PrtfDetail.dWz_eff			= dWz_eff[iEta1_max_posi];
        PrtfDetail.iLoadAppType		= iLoadAppType;
        PrtfDetail.bIsStiffener		= bIsStiffener;
        PrtfDetail.ik_F_calc_type	= ik_F_calc_type;
        PrtfDetail.dGamma_s_limit	= dGamma_s_lim;
        PrtfDetail.dGamma_s_cal		= dGamma_s_cal;
        PrtfDetail.dhw				= dh;
        PrtfDetail.da				= da;
        PrtfDetail.dSs				= dSs;
        PrtfDetail.dC				= dC;
    }

    return TRUE;
}


BOOL CDgnPlateGirder_IRC::Calc_LongitudinalShearResistance(ElemPairK ElemK, BOOL bPosiI, BOOL bSLS, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                                           CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,                                 
                                                           T_CLAS_BASE& ClassD, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dEs		= MatdD.Data1.Analysis.Elast;
    double dfck		= MatlElem.dfck;
    double dEcm		= MatlElem.dEcm;
    double dgam_M0	= m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c		= m_dGamma_C;
    double dgam_M1	= m_dGamma_M1;
    double dgam_V		= m_dGamma_V;    
    double dfcd		= 0.67* dfck/dgam_c;

    // 2. Section.
    int iClassSect	= ClassD.iClassSect;
    T_SECT_D& SectD	= SectElem.SectD;
    BOOL bTapered		= m_pDgnSectUtil->Is_TaperedSect(&SectD);

    T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

    // Dimension
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 3. Force
    const T_PGBR_BASE& PgbrBase = m_PgbrBase;
    double dNc = 0.0, dMc_Ed = 0.0, dM_Rd = 0.0, dMb_Rd = 0.0, dN_Ed = 0.0;
    double dMy		= PglsBase.dMy;
    double dMy_a		= PglsGd.dMy;
    double dV_Ed		= PglsBase.dFz;
    double dMa_Ed		= PglsBase.dMa_Ed;
    double dMpl_Rd	= PgbrBase.dMpl_Rd; // PGBR에서 결정된 값.
    double dMel_Rd	= PgbrBase.dMel_Rd; // PGBR에서 결정된 값.
    double dNc_el		= PgbrBase.dNc_el;
    double dNc_f		= PgbrBase.dNc_f;  
    double dM_Ed		= dMy_a + dMy;  

    // 4. Get Shear Connector Input Data
    // EN 1994-2, 6.6 Shear connection, 46page
    int iRat_sc_type	= 0;
    int iNum			= SconPosi.iNum;
    double dhsc		= SconPosi.dhsc;
    double dfu_sc		= SconPosi.dfu;
    double dDia_sc	= SconPosi.dDia;
    double dSt_sc		= SconPosi.dSt;
    double dSpace		= SconPosi.dSpace;
    double dAlpha		= 0.0;
    double dRat_sc	= dDia_sc < m_dZero ? 0.0 : dhsc/dDia_sc;

    // 5. Calc. Shear Strength of Shear connector
    // 6.6.3.1(1), 52p, EN 1994-2  
    if(3.0 - m_dZero < dRat_sc && dRat_sc < 4.0 + m_dZero)
    {
        iRat_sc_type = 1;
        dAlpha = 0.2*(dRat_sc+1.0);
    }
    else if(dRat_sc > 4.0)
    {
        iRat_sc_type = 2;
        dAlpha = 1.0;
    }
    else
    { 
        iRat_sc_type = 3;
        if(dAlpha>1.0)	{dAlpha = 1.0;}
        else			{dAlpha = 0.2*(dRat_sc+1.0);}
    }

    // Modify, Jaeoh. [4/28/2009] P_Rd 계산방법 수정 , 이혜연 사원님 요청 
    int iCheck_P_Rd	= 0;
    double dP_Rd1		= dfu_sc*m_dPi*0.001*pow(dDia_sc,2)/4.0/dgam_V;
    double dP_Rd2		= 0.36*dAlpha*pow(dDia_sc,2)*sqrt(dfck*dEcm)/dgam_V;
    double dP_Rd		= 0.0;
    double dP_Rd_SLS	= 0.0;

    if(dP_Rd1 < dP_Rd2)  // EN 1994. 6.6.3.1(1), (6.18), (6.19)
    {
        dP_Rd = dP_Rd1;
        iCheck_P_Rd = 0;
    }
    else
    {
        dP_Rd = dP_Rd2;
        iCheck_P_Rd = 1;
    }  
    //Pinakin IRC22 -2008 Table 7 pg.42
    if (dDia_sc>=25.0)
    {
        if (dfck<=25.0)		 dP_Rd = 103000.00;
        else if (dfck<=30) dP_Rd = 118000.00;
        else if (dfck<=40) dP_Rd = 146000.00;
        else							 dP_Rd = 154000.00;
    }
    else if (dDia_sc>=22.0)
    {
        if		(dfck<=25.0) dP_Rd = 79000.00;
        else if (dfck<=30) dP_Rd = 91000.00;
        else if (dfck<=40) dP_Rd = 113000.00;
        else							 dP_Rd = 119000.00;
    }
    else if (dDia_sc>=20.0)
    {
        if (dhsc>=100)
        {
            if		(dfck<=25.0) dP_Rd = 66000.00;
            else if (dfck<=30) dP_Rd = 75000.00;
            else if (dfck<=40) dP_Rd = 93000.00;
            else							 dP_Rd = 99000.00;
        }
        else
        {
            if		(dfck<=25.0) dP_Rd = 62000.00;
            else if (dfck<=30) dP_Rd = 71000.00;
            else if (dfck<=40) dP_Rd = 89000.00;
            else							 dP_Rd = 99000.00;
        }

    }
    else if (dDia_sc>=16.0)
    {
        if		(dfck<=25.0) dP_Rd = 42000.00;
        else if (dfck<=30) dP_Rd = 48000.00;
        else if (dfck<=40) dP_Rd = 60000.00;
        else							 dP_Rd = 63000.00;
    }
    else if (dDia_sc>=12.0)
    {
        if		(dfck<=25.0) dP_Rd = 24000.00;
        else if (dfck<=30) dP_Rd = 27000.00;
        else if (dfck<=40) dP_Rd = 34000.00;
        else							 dP_Rd = 35000.00;
    }
    else 
    {
        ASSERT(0); dP_Rd = 0.0;
        CString strExecuteMsg;
        strExecuteMsg.Format(_T("Check Shear Connector Dia.Refer IRC22-2008 Table 7 pg.42"));
        GSaveHistoryNF(strExecuteMsg);	
    }
    //End pinakin


    if(bSLS) dP_Rd_SLS = dP_Rd; 

    // 6. Calc. Shear Force & Shear Flow
    double dAz_Slab	= 0.0, dAz_Hunch = 0.0, dZbar = 0.0, dIyy_Org = 0.0; // 철근 고려 안한. Original Section Stiffness.  
    double dErat		= dEcm < m_dZero ? 0.0 : dEs/dEcm;   // Es/Ec, Section에 있는 값.

    if(!bTapered)
    {
        dZbar = SectD.SectAfter.Stiffness.Czm;
        dIyy_Org = SectD.SectAfter.Stiffness.Ryy;
    }
    else
    {
        dZbar = bPosiI ? SectD.SectAfter.SectI.Stiffness.Czm : SectD.SectAfter.SectJ.Stiffness.Czm;
        dIyy_Org = bPosiI ? SectD.SectAfter.SectI.Stiffness.Ryy : SectD.SectAfter.SectJ.Stiffness.Ryy;
    }

    double dHg = GirderStiff.Czm + GirderStiff.Czp;
    if(dHg - m_dZero > dZbar)
    {
        dAz_Slab  = dErat < m_dZero ? 0.0 : (0.67*dBc*dtc/dErat)*((dHt-0.5*dtc)-dZbar);
        dAz_Hunch = dErat < m_dZero ? 0.0 : (dB1*dHh/dErat)*((dHt-dtc-0.5*dHh)-dZbar);
    }
    else
    {
        dAz_Slab  = dErat < m_dZero ? 0.0 : (GirderStiff.Area)*(dZbar-GirderStiff.Czm);  
        dAz_Hunch = 0.0;
    }

    double dv_L_Rd	= 0.0;
    if(bSLS) {dv_L_Rd = dSpace < m_dZero ? 0.0 : dP_Rd_SLS*((double)iNum)/dSpace;}
    else     {dv_L_Rd = dSpace < m_dZero ? 0.0 : dP_Rd*((double)iNum)/dSpace;	   }

    // Modification, [12/5/2011] 이대근 bj님 요청 
    // 정모멘트이면서 Class가 1,2이고 dM_Ed-dMel_Rd>0 인 단면은 dV_L_Ed = (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd) 사용
    // 정모멘트 이면서 위 경우가 아니거나, 부모멘트인 경우는 V_L_Ed = V_Ed
    double dV_L_Ed	= 0.0; // Shear Force
    double dv_L_Ed	= 0.0; // Shear flow
    double de_d		= 0.0; // e_d = 0, b_eff = Bc defined in Section Data, 07.06.** 답변메일 내용으로 반영 
    double dLength_v	= de_d + dBc; // EN 1994-2, 6.6.2.3, Figure 6.12, 49p~51p 참고.
    double dAzI		= dIyy_Org < m_dZero ? 0.0 : (dAz_Slab+dAz_Hunch)/dIyy_Org;
    if((iClassSect==1 || iClassSect==2) && dM_Ed > -m_dZero && dM_Ed-dMel_Rd > 0.0)
    {
        dV_L_Ed = fabs(dMpl_Rd-dMel_Rd)< m_dZero ? 0.0 : (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd);
        dv_L_Ed = fabs(dLength_v)< m_dZero ? 0.0 : dV_L_Ed / dLength_v;
    }
    else
    {
        dV_L_Ed = dV_Ed;
        dv_L_Ed = fabs(dV_Ed)*dAzI;
    }

    double dv_Ed		= dv_L_Ed/(2.0*dtc);
    double dnu		= 0.6*(1.0-dfck/310.0);//From IRC112 Eq.10.6 Pg.90
    double dTheta_f	= m_dPi/180.0*45.0; // 45로 가정
    double dv_Ed_lim	= dnu*0.67*dfcd*sin(dTheta_f)*cos(dTheta_f);// Designers' guide to EN 1994-2, 128p

    // 7. Save Longitudinal Force Check Result Data
    PglsBase.iSectClass	= ClassD.iClassSect;  
    PglsBase.dNc			= dNc;           
    PglsBase.dNc_el		= dNc_el;        
    PglsBase.dNc_f		= dNc_f;   
    PglsBase.dMpl_Rd		= dMpl_Rd;       
    PglsBase.dMel_Rd		= dMel_Rd;   
    PglsBase.dV_L_Ed		= dV_L_Ed;
    PglsBase.dv_L_Ed2		=   m_dpgls_shear;     // Slab로 계산된.    
    PglsBase.dP_Rd1		= dP_Rd1;
    PglsBase.dP_Rd2		= dP_Rd2;
    PglsBase.dP_Rd		= dP_Rd;         
    PglsBase.iCheck_P_Rd	= iCheck_P_Rd;
    PglsBase.dv_L_Rd		= dv_L_Rd;     // P_Rd*(Stud개수)/간격
    PglsBase.dv_Ed		= dv_Ed;       // v_Ed(Slab의..)      
    PglsBase.dv_Ed_lim	= dv_Ed_lim;   // v_Ed(Slab의..)      
    PglsBase.dRatio		= fabs(dv_L_Rd) < m_dZero ? fabs(dv_L_Ed)/m_dZero : fabs(dv_L_Ed)/fabs(dv_L_Rd);
    PglsBase.bOK			= PglsBase.dRatio < 1.0 + m_dZero ? TRUE : FALSE;
    PglsBase.dAlpha		= dAlpha;
    PglsBase.dAzI			= dAzI;
    PglsDetail.dmuy_c		= dAzI*dIyy_Org;
    PglsDetail.dP_Rd_SLS	= dP_Rd_SLS;

    if(IsPrintMode())
    {
        PglsDetail.dfu = dfu_sc;
        PglsDetail.iRat_sc_type = iRat_sc_type;
        PglsDetail.iNum = iNum;
        PglsDetail.dDsc = dDia_sc;
        PglsDetail.dhsc = dhsc;
        PglsDetail.dSpace = dSpace;
        PglsDetail.dLv = dLength_v;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_FatigueResistance(ElemPairK ElemK, BOOL bPosiI, double dt_Ld, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                                 CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, CPG_FRDT_POSI& FrdtPosi,                                
                                                 T_CLAS_BASE& ClassD, T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail)
{  
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dfck		= MatlElem.dfck;
    double dfcm		= MatlElem.dfcm;
    double dfctm		= MatlElem.dfctm;
    double dEcm		= MatlElem.dEcm;
    double dgam_M0	= m_dGamma_M0;
    double dgam_Ff	= m_dGamma_Ff; 
    double dgam_Mfs	= m_dGamma_Mfs;  

    // 2. Section.
    int iClassSect	= ClassD.iClassSect;

    // 3. Force
    double dMy		= PgrfBase.dMy;
    double dV_Ed		= PgrfBase.dFz;

    // Get Shear Connector Input Data
    // EN 1994-2, 6.6 Shear connection, 46page
    int iNum			= SconPosi.iNum;
    double dhsc		= SconPosi.dhsc;
    double dfu_sc		= SconPosi.dfu;
    double dDia_sc	= SconPosi.dDia;
    double dSt_sc		= SconPosi.dSt;
    double dSpace		= SconPosi.dSpace;
    double dAlpha		= 0.0;

    double dv_L_Ed		= m_PglsBase.dv_L_Ed;
    double dLamda_v1		= 1.55; // // 6.8.6.2(4), 76page
    double dLamda_v2		= FrdtPosi.dLamda_v2;
    double dLamda_v3		= 0.0;
    double dLamda_v4		= FrdtPosi.dLamda_v4;
    double dDel_Tau_E_2	= 0.0;
    double dDel_Tau_c		= 0.0;

    // The range of shear force per connector
    double dFsc		= (iNum == 0) ? 0.0 : dv_L_Ed*dSpace/((double)iNum);
    double dA_sc		= m_dPi*pow(dDia_sc,2)/4.0;
    double dDel_Tau	= dA_sc < m_dZero ? 0.0 : dFsc/dA_sc;

    // EN 1994-2, 6.8.6.2 Shear connection, 76page
    // EN 1993-2, 9.5.2 Damage equivalence factors λ for road bridges, 42page
    double dQ0			= 480.0; // 480 kN
    double dN0			= 0.5*10E6;
    double dN_Obs			= 0.5*10E6; // The UK's National Annex to EN 1991-2 may modify this value
    double dSigma_niQi5	= 8.051*10E18;
    double dSigma_ni		= 1.0*10E6;
    double dQm1			= pow(dSigma_niQi5/dSigma_ni, 1./5.); //kN  

    //dLamda_v2 = dQ0==0.0 || dN0==0.0 ? 0.0 : dQm1/dQ0*pow((dN_Obs/dN0),1./5.);

    dLamda_v3 = pow(dt_Ld/100.0, 1./8.); // EN 1994-2 §6.8.6.2(4) we should consider 1/8 instead 1/5

    //dLamda_v4 = 1.0; // ?? EN 1993-2, 9.5.2(6), 44page

    double dLamda_v = dLamda_v1*dLamda_v2*dLamda_v3*dLamda_v4;
    // 
    dDel_Tau_E_2 = dLamda_v*dDel_Tau;
    // EN 1994-2, 6.8.3(3), 72page.
    dDel_Tau_c = 90.0; // is the reference value at Nc=2*10^6 cycles with Δτ_c equal to 90 N/mm^2
    //FOR IRC22 2008 Cl.606.4.2 pg.43 Table 8 in N
    if (dDia_sc<=16)  dDel_Tau_c = 15000.0;
    else if (dDia_sc<=20)  dDel_Tau_c = 23000.0;
    else if (dDia_sc<=22)  dDel_Tau_c = 29000.0;
    else									 dDel_Tau_c = 37000.0;
    //
    //double dRatio = fabs(dgam_Mfs*dDel_Tau_c) < m_dZero ? dgam_Ff*dDel_Tau_E_2/m_dZero : dgam_Ff*dDel_Tau_E_2/(dDel_Tau_c/dgam_Mfs);
    double dRatio =  dFsc/dDel_Tau_c;
    dRatio = fabs(dRatio);

    //For Member check of Fatigue
    double dN_sc = dt_Ld;
    double dF_fn = dLamda_v4;
    double dGamma_mft = 1.1;// 
    double dT_fn = dLamda_v2;
    bool bFat_Check_req = FALSE;
    double dParam1 = 27.0/m_dGamma_Mf;
    double dParam2 = 5000000*pow((dParam1/dFsc),3);
    if (dFsc>dParam1 || dN_sc>= dParam2) bFat_Check_req = TRUE;
    else bFat_Check_req = FALSE;

    // 
    double dF_f = 0.0;
    double dTau_f = 0.0;
    if (dN_sc<=5000000)
    {
        dF_f   = dF_fn * pow(5000000/dN_sc,1.0/3.0);
        dTau_f = dT_fn * pow(5000000/dN_sc,1.0/5.0);
    }
    else
    {
        dF_f   = dF_fn * pow(5000000/dN_sc,1.0/5.0);
        dTau_f = dT_fn * pow(5000000/dN_sc,1.0/5.0);
    }
    T_SECT_D& SectD = SectElem.SectD;
    double dTw_t	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
    double dCorrection_Factor = min(pow((25.0/dTw_t),0.25),1.0);

    //Design Fatigue Strength of a member

    double dF_jd   = dCorrection_Factor*dF_fn/dGamma_mft;
    double dTau_jd = dCorrection_Factor*dT_fn/dGamma_mft;


    //End Mamber check FATIGUE
    // Save Result
    PgrfBase.bOK			= dRatio > 1.0 ? FALSE : TRUE;
    PgrfBase.dDel_Tau     = dDel_Tau;      // Δτ                [Unit=Stress]
    PgrfBase.dLamda_v     = dLamda_v;      // λv                 [Unit=None]
    PgrfBase.dLamda_v1    = dF_fn;     // λv1                [Unit=None]
    PgrfBase.dLamda_v2    = dT_fn;     // λv2                [Unit=None]
    PgrfBase.dLamda_v3    = dF_jd;     // λv3                [Unit=None]
    PgrfBase.dLamda_v4    = dTau_jd;     // λv4                [Unit=None]
    PgrfBase.dDel_Tau_E_2 = dCorrection_Factor;  // Δτ_E,2            [Unit=Stress]
    PgrfBase.dDel_Tau_c   = dDel_Tau_c;    // Δτ_c              [Unit=Stress]
    PgrfBase.dGamma_Ff    = dLamda_v4;       //                     [Unit=None]
    PgrfBase.dGamma_Mfs   = dLamda_v2;      //                     [Unit=None]
    PgrfBase.dRatio       = dRatio;        // rFf*Δτ_E,2/(Δτ_c/rMf,s)  [Unit=None]  
    PgrfBase.dv_L_Ed      = dv_L_Ed;
    PgrfBase.dFsc         = dFsc;
    PgrfBase.dAsc         = dA_sc;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_StressCheck4SLS(ElemPairK ElemK, BOOL bPosiI, BOOL bIsGenSect, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                               const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, double dk1, double dk2, double dk3, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail)
{
    // 1. Material.
    T_MATD_D&  MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dEs		= MatdD.Data1.Analysis.Elast;
    double dEcm		= MatlElem.dEcm;
    double dfck		= MatlElem.dfck;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;
    double dgam_M_ser = m_dGamma_M_ser;

    // 2. Section.
    T_SECT_D& SectD = SectElem.SectD;
    int iSectShapeType = Get_SectShapeType(SectD);
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    const T_SECT_SECTBASE_D& SectBase2D = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
    T_SECT_STIFFNESS NonConSect, RebarSect, OrgSect; 
    NonConSect.Initialize(); RebarSect.Initialize(); OrgSect.Initialize();
    GetPureSectStiffD(SectElem, bPosiI, OrgSect);
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    // Dimension
    double dHt	= iSectShapeType == D_SECT_TYPE_COMPO_G ? Get_Height4Gen(bPosiI) : m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI); 
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI); 
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI); 
    double dTF1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI); 
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI); 
    double dTF2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI); 
    double dHw	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_Hh(&SectD);

    // 3. Force
    // Before
    double dFx_a		= PgbsBase.ChkGirder.aBeforeForce[0];
    double dFz_a		= PgbsBase.ChkGirder.aBeforeForce[2];
    double dMy_a		= PgbsBase.ChkGirder.aBeforeForce[4];
    double dMz_a		= PgbsBase.ChkGirder.aBeforeForce[5];
    // After
    double dFx		= PgbsBase.ChkGirder.aForce[0];  
    double dFz		= PgbsBase.ChkGirder.aForce[2];
    double dMy		= PgbsBase.ChkGirder.aForce[4];
    double dMz		= PgbsBase.ChkGirder.aForce[5];
    //
    double dV_Ed		= dFz_a + dFz;  
    BOOL bPositiveMy	= dMy > -m_dZero;

    // 4. Girder Stress Check
    T_SECT_STIFFNESS GirderStiffD;  GirderStiffD.Initialize();
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiffD);  
    T_SECT_STIFFNESS CalcStressStiff = bPositiveMy ? RebarSect : NonConSect;

    double dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4;
    double dx1_a, dy1_a, dx2_a, dy2_a, dx3_a, dy3_a, dx4_a, dy4_a;
    double dArea, dAsy, dAsz, dRxx, dRyy, dRzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea;
    double dArea_a, dAsy_a, dAsz_a, dRxx_a, dRyy_a, dRzz_a, dCyp_a, dCym_a, dCzp_a, dCzm_a, dQyb_a, dQzb_a, dWArea_a;

    GirderStiffD.GetStiff(dArea_a, dAsy_a, dAsz_a, dRxx_a, dRyy_a, dRzz_a, dCyp_a, dCym_a, dCzp_a, dCzm_a, dQyb_a, dQzb_a, dWArea_a);
    GirderStiffD.GetPosition(dx1_a, dy1_a, dx2_a, dy2_a, dx3_a, dy3_a, dx4_a, dy4_a);  
    CalcStressStiff.GetStiff(dArea, dAsy, dAsz, dRxx, dRyy, dRzz, dCyp, dCym, dCzp, dCzm, dQyb, dQzb, dWArea);
    CalcStressStiff.GetPosition(dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4);

    double aStressG[4]={0.0}, aStressC[4]={0.0};
    double aStress4[4]={0.0}, aStressRat[4]={0.0};

    aStressG[0] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx1_a, dy1_a);
    aStressG[1] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx2_a, dy2_a);
    aStressG[2] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx3_a, dy3_a);
    aStressG[3] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dRyy_a, dRzz_a, dx4_a, dy4_a);

    aStressC[0] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx1, dy1);
    aStressC[1] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx2, dy2);
    aStressC[2] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx3, dy3);
    aStressC[3] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx4, dy4);

    for(int i=0; i<4; i++){aStress4[i] = aStressG[i] + aStressC[i];}

    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;


    double dSigma_ser_lt	= fabs(dgam_M_ser) < m_dZero ? 0.0 : 0.87*dfyt;  
    double dMaxStressRat	= 0.0;
    double dMaxStress		= 0.0;
    int iMaxStressRatIdx	= 0;

    for(int i=0; i<4; i++)
    {
        aStressRat[i] = fabs(dSigma_ser_lt) < m_dZero ? 0.0 : fabs(aStress4[i]) / dSigma_ser_lt;
        if(i==0)
        {
            iMaxStressRatIdx	= 0;
            dMaxStress		= aStress4[i];
            dMaxStressRat		= aStressRat[i];
        }
        else
        {
            if(dMaxStressRat<aStressRat[i])
            {
                iMaxStressRatIdx = i;
                dMaxStress		= aStress4[i];
                dMaxStressRat	= aStressRat[i];
            }
        }
    }

    BOOL bChkStress_gr = dMaxStressRat > 1.0 ? FALSE : TRUE;

    // Check Shear stress, 합성되기전 거더의 도심에서 계산하자. 
    double dbw = 0.0, dQ = 0.0;
    double dAv_web = 0.0;
    double dtau_Ed_ser = 0.0;

    BOOL bAveShearStress = TRUE;
    if(bAveShearStress) // 평균 전단 응력. 
    {    
        if(iSectShapeType == D_SECT_TYPE_COMPO_I || iSectShapeType == D_SECT_TYPE_COMPO_B || iSectShapeType == D_SECT_TYPE_COMPO_TUB)
        {
            dAv_web = dHw*dTw_t;
            dtau_Ed_ser = dAv_web < m_dZero ? 0.0 : dV_Ed/dAv_web; //Get_ShearStress(dFz, dQ, RebarSect.Ryy, dbw);    
        }
        else if(iSectShapeType==D_SECT_TYPE_COMPO_G)
        {
            Get_ThicknessOfWeb4GenSect(GLineDgn, dbw, dAv_web);
            dtau_Ed_ser = dAv_web < m_dZero ? 0.0 : dV_Ed/dAv_web; //Get_ShearStress(dFz, dQ, RebarSect.Ryy, dbw);
        }
        else ASSERT(0);
    }
    else // 도심의 전단응력으로 적용할 때 사용. 
    {
        ASSERT(0);
    }

    double dTau_ser_lt	= fabs(dgam_M_ser) < m_dZero ? 0.0 :0.87* dfyt/(sqrt(3.0)); 
    BOOL bChkShearStress	= fabs(dtau_Ed_ser)> dTau_ser_lt ? FALSE : TRUE;

    double dStress_comb2	= pow(dMaxStress,2) + 3.0*pow(dtau_Ed_ser,2);
    double dStress_comb	= sqrt(dStress_comb2);
    BOOL bChkStress_comb	= dStress_comb > dSigma_ser_lt ? FALSE : TRUE;
    BOOL bOK_gr = (bChkStress_gr && bChkShearStress && bChkStress_comb);

    memcpy(PgbsBase.ChkGirder.dStress, aStress4, sizeof(PgbsBase.ChkGirder.dStress));
    memcpy(PgbsBase.ChkGirder.dRatio, aStressRat, sizeof(PgbsBase.ChkGirder.dRatio));

    PgbsBase.ChkGirder.dMaxStress		= -1.0*dMaxStress;
    PgbsBase.ChkGirder.dMaxRatio		= dMaxStressRat;
    PgbsBase.ChkGirder.iIndex_cr		= iMaxStressRatIdx;
    PgbsBase.ChkGirder.dAlwStress		= dSigma_ser_lt;
    PgbsBase.ChkGirder.bChk_Stress	= bChkStress_gr;
    PgbsBase.ChkGirder.dtau_w			= fabs(dtau_Ed_ser);
    PgbsBase.ChkGirder.dAlw_tau		= dTau_ser_lt;
    PgbsBase.ChkGirder.bChk_tau		= bChkShearStress;
    PgbsBase.ChkGirder.dStress_cb		= dStress_comb;
    PgbsBase.ChkGirder.dAlw_stress_cb = dSigma_ser_lt;
    PgbsBase.ChkGirder.bChk_Stress_cb = bChkStress_comb;
    PgbsBase.ChkGirder.bOK			= bOK_gr;

    //////////////////////////////////////////////////////////////////////////
    // 5. Slab stress Check
    double dyGap	= CalcStressStiff.Cym - OrgSect.Cym; // 도심의 차이 
    double dn		= dEcm < m_dZero ? 0.0 : dEs/dEcm;
    double aSlabStress4[4]={0.0}, aSlabStressRat[4]={0.0};
    double dx1_s, dy1_s, dx2_s, dy2_s, dx3_s, dy3_s, dx4_s, dy4_s; 

    if(iSectShapeType==D_SECT_TYPE_COMPO_I || iSectShapeType==D_SECT_TYPE_COMPO_B || iSectShapeType==D_SECT_TYPE_COMPO_TUB)
    {
        dx1_s = -0.5*dBc - dyGap,	dy1_s = dHt-CalcStressStiff.Czm;
        dx2_s =  0.5*dBc - dyGap,	dy2_s = dy1_s;
        dx3_s =  dx2_s,				dy3_s = dH+dHh-CalcStressStiff.Czm;
        dx4_s =  dx1_s,				dy4_s = dy3_s;
    }
    else if(iSectShapeType==D_SECT_TYPE_COMPO_G)
    {
        double aYcr_sb[4]={0.0}, aZcr_sb[4]={0.0};
        double dZbar_sb = Get_ZcentroidofSlab(bPosiI);  
        int nPosI = bPosiI ? 0 : 1;

        for(int i=0; i<4; i++) 
        {
            aYcr_sb[i] = m_aY_sb[nPosI][i];
            aZcr_sb[i] = m_aZ_sb[nPosI][i];
        }

        dx1_s = aYcr_sb[0] - dyGap;
        dx2_s = aYcr_sb[1] - dyGap;
        dx3_s = aYcr_sb[2] - dyGap;
        dx4_s = aYcr_sb[3] - dyGap;
        //
        double dZgap_cent_sb = dZbar_sb-CalcStressStiff.Czm;
        dy1_s = aZcr_sb[0] + dZgap_cent_sb;
        dy2_s = aZcr_sb[1] + dZgap_cent_sb;
        dy3_s = aZcr_sb[2] + dZgap_cent_sb;
        dy4_s = aZcr_sb[3] + dZgap_cent_sb;
    }
    else ASSERT(0);

    if(bPositiveMy) // 균열일경우는 계산하지 않는다. 
    {
        aSlabStress4[0] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx1_s, dy1_s);
        aSlabStress4[1] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx2_s, dy2_s);
        aSlabStress4[2] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx3_s, dy3_s);
        aSlabStress4[3] = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dx4_s, dy4_s);    
    }

    for(int i=0; i<4; i++) {aSlabStress4[i] = dn < m_dZero ? 0.0 : aSlabStress4[i]/dn;}

    double dSigma_c_lt	= 0.0;
    int iLcomSLSType		= PgbsBase.ChkSlab.LcomType;

    if(iLcomSLSType==1)       {dSigma_c_lt = 0.33*dfck;} // 1=Quasi-permanent
    else if(iLcomSLSType==3)  {dSigma_c_lt = dk1*dfck;} // 3=Characteristic
    //else ASSERT(0); 계산은 다하자.

    double dMaxSlabStressRat	= 0.0;
    double dMaxSlabStress		= 0.0;
    int iMaxSlabStressRatIdx	= 0;

    for(int i=0; i<4; i++)
    {
        aSlabStressRat[i] = fabs(dSigma_c_lt) < m_dZero ? 0.0 : fabs(aSlabStress4[i]) / dSigma_c_lt;
        if(i==0)
        {
            iMaxSlabStressRatIdx = 0;
            dMaxSlabStress = aSlabStress4[i];
            dMaxSlabStressRat = aSlabStressRat[i];
        }
        else
        {
            if(dMaxSlabStressRat<aSlabStressRat[i])
            {
                iMaxSlabStressRatIdx = i;
                dMaxSlabStress = aSlabStress4[i];
                dMaxSlabStressRat = aSlabStressRat[i];
            }
        }
    }
    BOOL bChkSlabStress = dMaxSlabStressRat > 1.0 ? FALSE : TRUE;

    PgbsBase.ChkSlab.dStress_sb = -1.0*dMaxSlabStress;
    PgbsBase.ChkSlab.dAlwStress_sb = dSigma_c_lt;
    PgbsBase.ChkSlab.dMaxRatio_sb = dMaxSlabStressRat;
    PgbsBase.ChkSlab.bChk_Stress_sb = bChkSlabStress;  

    //////////////////////////////////////////////////////////////////////////
    // 6. Rebar Stress
    double dMaxStress_bar	= 0.0;
    double dSigma_s_lt	= dk3 * dfsk;

    double dy_ro = 0.0, dz_ro = 0.0;
    // CRCDataCtrl.cpp 참조. Get_CpgRbar(T_ELEM_K ElemK, BOOL bPosI, BOOL bCrack, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi)

    int iRebarSize = arRbarPosi.GetSize();
    for(int i=0; i < iRebarSize; i++)
    {
        const _DGN_RBAR_CRC& Rbar = arRbarPosi[i];
        if(iSectShapeType==D_SECT_TYPE_COMPO_I || iSectShapeType==D_SECT_TYPE_COMPO_B || iSectShapeType==D_SECT_TYPE_COMPO_TUB)
        {
            dy_ro = Rbar.dy-dyGap; // Slab의 도심기준으로 되어 있음. 
            dz_ro = (dHt-0.5*dtc+Rbar.dz) - CalcStressStiff.Czm; //Rbar.dz - CalcStressStiff.Czm;  // Rbar.dz=Slab 도심(dtc/2) 에서 떨어진 거리로
        }
        else if(iSectShapeType==D_SECT_TYPE_COMPO_G)
        {
            int iGenPart = SectBase2D.aGeneralPart.GetSize();
            dy_ro = Rbar.dy-dyGap;
            dz_ro = Rbar.dz - CalcStressStiff.Czm;
            //dz_ro = bPosiI ? m_dZbar_sb[0]-m_dCzm_sb[0] : m_dZbar_sb[1]-m_dCzm_sb[1];
            //dz_ro += Rbar.dz;
        }

        double dStress_bar = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dRyy, dRzz, dy_ro, dz_ro);
        dStress_bar = dStress_bar < 0.0 ? max(dStress_bar, -dfsk) : min(dStress_bar, dfsk);
        if(i==0)
        {
            dMaxStress_bar = dStress_bar;
        }
        else if(fabs(dMaxStress_bar)<fabs(dStress_bar))
        {
            dMaxStress_bar = dStress_bar;
        }
    }

    double dMaxRebarStressRat = fabs(dSigma_s_lt) < m_dZero ? 0.0 : fabs(dMaxStress_bar)/dSigma_s_lt;
    BOOL bChkRebarStress =  dMaxRebarStressRat > 1.0 ? FALSE : TRUE; 

    // Save Result Data
    PgbsBase.ChkBar.dStress_bar		= dMaxStress_bar;       
    PgbsBase.ChkBar.dAlwStress_bar	= dSigma_s_lt;
    PgbsBase.ChkBar.dMaxRatio_bar		= dMaxRebarStressRat;
    PgbsBase.ChkBar.bChk_Stress_bar	= bChkRebarStress;  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_LongitudinalShearResistance4SLS(ElemPairK ElemK, BOOL bPosiI, BOOL bIsGenSect, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                                               CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,                                 
                                                               T_CLAS_BASE& ClassD, double dks, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail)
{
    //For construction reasons, it is not normally planned to change the number of
    //studs per unit length continuously. The bridge total length is therefore divided
    //into n segments of length l,i i∈[1,n]. A number N,i∈[1,n] of studs is then
    //arranged in each one (constant density per segment).
    T_PGLS_BASE PglsBase;
    PglsBase.bChk     = PlssBase.bChk    ;
    PglsBase.ElemK    = PlssBase.ElemK   ;
    PglsBase.nPosi    = PlssBase.nPosi   ;
    PglsBase.LcomK    = PlssBase.LcomK   ;
    PglsBase.OrgLcomK = PlssBase.OrgLcomK;
    PglsBase.iMax     = PlssBase.iMax    ;	
    PglsBase.dFx      = PlssBase.aForce[0]; 
    PglsBase.dFz      = PlssBase.aForce[2];
    PglsBase.dMy      = PlssBase.aForce[4];           
    PglsBase.dMz      = PlssBase.aForce[5];           

    T_PGLS_BASE PglsGd;
    PglsGd.dFx      = PlssGd.aForce[0]; 
    PglsGd.dFz      = PlssGd.aForce[2];
    PglsGd.dMy      = PlssGd.aForce[4];           
    PglsGd.dMz      = PlssGd.aForce[5];           

    T_PGLS_DETAIL PglsDetail;
    if (SconPosi.dDia>0.0)
    {
        if(!bIsGenSect) {Calc_LongitudinalShearResistance(ElemK, bPosiI, TRUE, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, PglsGd, PglsBase, PglsDetail);		  }
        else            {Calc_LongitudinalShearResistance4GenSect(ElemK, bPosiI, TRUE, MatlElem, SectElem, SconPosi, arRbarPosi, arLstiPosi, TrstPosi, TsesPosi, ClassD, dks, PglsGd, PglsBase, PglsDetail);}

    }
    else return TRUE;
    // Modify by GAY. PMS:4216. ('12.05.18). 데이터 할당 누락부분 수정.
    PlssBase.iSectClass	= PglsBase.iSectClass ; 
    PlssBase.dAlpha		= PglsBase.dAlpha     ;
    PlssBase.dNc			= PglsBase.dNc        ;           
    PlssBase.dNc_el		= PglsBase.dNc_el     ;        
    PlssBase.dNc_f		= PglsBase.dNc_f      ;         
    PlssBase.dMa_Ed		= PglsBase.dMa_Ed     ;        
    PlssBase.dMc_Ed		= PglsBase.dMc_Ed     ;        
    PlssBase.dMpl_Rd		= PglsBase.dMpl_Rd    ;       
    PlssBase.dMel_Rd		= PglsBase.dMel_Rd    ;       
    PlssBase.dV_L_Ed		= PglsBase.dV_L_Ed    ;       
    PlssBase.dv_L_Ed		= PglsBase.dv_L_Ed    ; 
    PlssBase.dv_L_Ed2		= PglsBase.dv_L_Ed2   ;
    PlssBase.dP_Rd1		= PglsBase.dP_Rd1     ;
    PlssBase.dP_Rd2		= PglsBase.dP_Rd2     ;
    PlssBase.dP_Rd		= PglsBase.dP_Rd      ;
    PlssBase.dP_Rd_SLS	= PglsDetail.dP_Rd_SLS;
    PlssBase.iCheck_P_Rd	= PglsBase.iCheck_P_Rd;
    PlssBase.dv_L_Rd		= PglsBase.dv_L_Rd    ;       
    PlssBase.dv_Ed		= PglsBase.dv_Ed      ;
    PlssBase.dv_Ed_lim	= PglsBase.dv_Ed_lim  ;
    PlssBase.dRatio		= fabs(PglsBase.dv_L_Rd) < m_dZero? fabs(PglsBase.dv_L_Ed/m_dZero): fabs(PglsBase.dv_L_Ed/PglsBase.dv_L_Rd);  
    PlssBase.dAzI			= PglsBase.dAzI; // Q/Iy  
    PlssBase.bOK			= fabs(PglsBase.dv_L_Ed) > fabs(PglsBase.dv_L_Rd) ? FALSE : TRUE;

    PlssDetail.dfu          = PglsDetail.dfu;
    PlssDetail.iRat_sc_type = PglsDetail.iRat_sc_type;
    PlssDetail.iNum         = PglsDetail.iNum;
    PlssDetail.dDsc         = PglsDetail.dDsc;
    PlssDetail.dhsc         = PglsDetail.dhsc;
    PlssDetail.dSpace       = PglsDetail.dSpace;   
    PlssDetail.dLv          = PglsDetail.dLv;
    PlssDetail.dmuy_c       = PglsDetail.dmuy_c;    

    return TRUE;
}


BOOL CDgnPlateGirder_IRC::Calc_BendingResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CPG_TRST_POSI& TrstPosi,
                                                         T_CLAS_BASE& ClassD, const T_GENL_DATA& GLineData, T_GLINE_DGN_D& GLineDgn, 
                                                         T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
    // Material.
    T_MATD_D MatdD = MatlElem.MatdD;
    // Steel.
    double dfy  = MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    // Concrete
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    // Rebar
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; // Steel로..

    // Material
    double dgam_M0  = m_dGamma_M0;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;

    double dfcd		= 0.67* dfck/dgam_c;
    double dfsd = dgam_sr==0.0 ? dfsk : dfsk/dgam_sr;

    double dAlpha = m_dAlpha;
    // Section.  
    T_SECT_D SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_SECTBASE_D SectBaseD = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
    T_SECT_STIFFNESS NonConSect, RebarSect; 
    NonConSect.Initialize();   RebarSect.Initialize();

    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    double dCzp = SectBaseD.Stiffness.Czp;
    double dCzm = SectBaseD.Stiffness.Czm;
    double dHg = dCzp + dCzm; // Height of girder

    BOOL bEquivalentClass2 = FALSE;
    int iClassSect=ClassD.iClassSect;

    //if((ClassD.iFlangeClass==1 || ClassD.iFlangeClass==2) && ClassD.iWebClass==3)
    //  bEquivalentClass2 = TRUE;  

    PgbrBase.iTopClass  = ClassD.iTopFlgClass;
    PgbrBase.iBotClass  = ClassD.iBotFlgClass;
    PgbrBase.iWebClass  = ClassD.iWebClass;
    PgbrBase.iSectClass = ClassD.iClassSect;  
    PgbrBase.bEquiClass2   = bEquivalentClass2;   

    //double dFx=0., dFy=0., dFz=0., dMx=0., dMy=0., dMz=0.;
    double dNc=0.0;
    double dNc_el=0.0;
    double dNc_f=0.0;
    double dMpl_Rd=0.0;  
    double dMel_Rd=0.0;
    double dM_Rd=0.0;
    double aForceGd[6]={0.0}; // Girder
    double aForce[6]={0.0};   // Composite에만 
    aForceGd[0] = PgbrGd.dFx;
    aForceGd[4] = PgbrGd.dMy;
    aForceGd[5] = PgbrGd.dMz;
    aForce[0] = PgbrBase.dFx;
    aForce[4] = PgbrBase.dMy;
    aForce[5] = PgbrBase.dMz;
    double dMa_Ed = PgbrBase.dMa_Ed;
    double dMc_Ed = PgbrBase.dMy;
    double dM_Ed_c = dMa_Ed + dMc_Ed;

    double dPlasticNA = 0.0;
    double dPlasticNAEff = 0.0;
    BOOL bPositive = dMc_Ed>=0.0 ? TRUE : FALSE;
    dPlasticNA = GetPlasticNA();
    //Get_PlasticNA4GenSect(bPosiI, bPositive, MatlElem, SectBaseD, arRbarPosi, GLineData, GLineDgn, dPlasticNA);
    // General Section인 경우는 Effective class 2 web을 적용하기 힘들다. 
    // EN 1993-1-1, 6.2.2.4 Effective properties of cross sections with class 3 webs and class1 or 2 flanges, 48p
    //if(bEquivalentClass2)
    //    Get_PlasticNAEff(bPosiI, bPositive, MatlElem, SectData, arRbarPosi, dPlasticNAEff, dEffeLen, dHoleLen);

    double dHt = Get_Height4Gen(bPosiI);
    double dXpl = !bEquivalentClass2 ? dHt - dPlasticNA : dHt - dPlasticNAEff;
    double dXplOverh = dHt==0.0 ? 0.0 : dXpl/dHt; // Ht : 전체높이.
    BOOL bNonLinear=FALSE;
    if(dXplOverh>0.4) bNonLinear = TRUE;
    //
    // 0=Plastic resistance moment 
    // 1=Non-linear resistance to bending
    // 2=Elastic resistance to bending.  

    // 
    Get_PlasticMoment4GenSect(bPosiI, bPositive, MatlElem, SectBaseD, arRbarPosi, GLineData, GLineDgn, dPlasticNA, dMpl_Rd, PgbrDetail);
    // dNc_f.
    if(bPositive) Get_Nc_fofSlab(bPosiI, dfcd, dXpl, dHt, SectBaseD, GLineDgn, dNc_f);

    Get_ElasticResistance4GenSect(ElemK, bPosiI, bPositive, aForceGd, aForce, MatlElem, SectElem, arRbarPosi, TrstPosi, GLineData, GLineDgn, PgbrBase, PgbrDetail);

    // 6.2.1.2(2)  
    double dBeta=1.0;
    BOOL bS420orS460 = IsMaterialS420orS460(dfy);
    dBeta=1.0;
    if(bPositive)
    {
        if(bS420orS460)
        {
            if(!Get_BetaReductionFactor(dXpl, dHt, dBeta))
                dBeta=1.0;
        }
    }

    PgbrBase.dMpl_Rd = dMpl_Rd;

    // For Shear Resistance
    double dMf_Rd=0.0;
    Get_Mf_Rd_PlasticMoment4GenSect(bPosiI, bPositive, MatlElem, SectBaseD, arRbarPosi, GLineData, GLineDgn, dMf_Rd);
    PgbrBase.dMpl_Rd_s = dMpl_Rd;
    PgbrBase.dMf_Rd = dMf_Rd;

    // Non-linear Resistance
    if(aForce[4]>=0.0 && bS420orS460 && bNonLinear)
        Calc_NonLinearResistance(ElemK, bPosiI, PgbrBase);
    else
        PgbrBase.dM_Rd = iClassSect==1 || iClassSect==2 ? dBeta*PgbrBase.dMpl_Rd : PgbrBase.dMel_Rd;

    PgbrBase.dPlasticNA    = dPlasticNA;          
    PgbrBase.dPlasticNAEff = dPlasticNAEff;       
    //PgbrBase.dEffeLen = dEffeLen;                    
    //PgbrBase.dHoleLen = dHoleLen;                    

    PgbrBase.dRatio = PgbrBase.dM_Rd==0.0 ? fabs(dM_Ed_c)/m_dZero : fabs(dM_Ed_c)/fabs(PgbrBase.dM_Rd);

    if(PgbrBase.dRatio<1.0)
        PgbrBase.bOK = TRUE;
    else 
        PgbrBase.bOK = FALSE;

    // For Detail
    PgbrBase.dh = dHt;
    PgbrBase.dXpl = dXpl; 
    PgbrBase.dXplOverh = dXplOverh;
    PgbrBase.dAlpha = dAlpha;
    PgbrBase.dCompfsdAsidsi = 0.0; //dCompfsdAsidsi;
    PgbrBase.dTensfsdAsidsi = 0.0; //dTensfsdAsidsi;
    PgbrBase.dBeta = dBeta;     
    //PgbrBase.dStrStlTop = dStrStlTop;
    //PgbrBase.dStrComTop = dStrComTop;
    //PgbrBase.dStrStlBot = dStrStlBot;
    //PgbrBase.dStrComBot = dStrComBot;  
    //PgbrBase.dSIGa_top = dSIGa_top;
    //PgbrBase.dSIGa_bot = dSIGa_bot;  
    //PgbrBase.dk_min = dk_min;     
    PgbrBase.dNc_f = dNc_f;
    //PgbrBase.dNc_el = dNc_el;

    if(IsPrintMode())
    {
        PgbrDetail.GLineDgn = GLineDgn;
        PgbrDetail.StressInfo4Class = m_DLineCalcD;
        PgbrDetail.dMpl_Rd_cal = dMpl_Rd;

    }  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_VerticalShearResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,
                                                               T_CLAS_BASE& ClassD, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail)
{
    // Material.
    T_MATD_D MatdD = MatlElem.MatdD;
    int iMatClass = MatlElem.iMatClass;
    // Steel.
    double dfy1  = MatdD.Data1.Design.S_Fy1;
    double dfy2 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    // Concrete
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    // Rebar
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; // Steel로..

    // Material
    double dgam_M0 = m_dGamma_M0;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;  
    double dgam_M1 = m_dGamma_M1;
    double dgam_M2 = m_dGamma_M0;  
    // Modify by GAY. PMS:4216. ('12.05.11).
    double dEta = Get_Eta4SteelGrade(iMatClass, dfy1);
    //   double dEta=0.0; // η may be conservativel taken equal 1.0 
    //   Get_Eta4SteelMaterial(iMatClass, dgam_M0, dgam_M1, dEta);

    double dfcd		= 0.67* dfck/dgam_c;
    double dfsd = dgam_sr==0.0 ? dfsk : dfsk/dgam_sr;

    // Section.  
    T_SECT_D SectD	= SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_SECTBASE_D SectBaseD = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
    T_SECT_STIFFNESS NonConSect; NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;  RebarSect.Initialize();  
    T_SECT_STIFFNESS OrgSect; OrgSect.Initialize();

    GetPureSectStiffD(SectElem, bPosiI, OrgSect);
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    // Girder Stiffness	
    T_SECT_STIFFNESS GirderStiffD;  GirderStiffD.Initialize();
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiffD);  

    // Force
    double aForce[6]={0.0,0.0,0.0,0.0,0.0,0.0};
    double dFx=0., dMy=0., dMz=0.;
    double dNc=0.0;
    double dNc_el=0.0;
    double dNc_f=0.0;
    double dMa_Ed=0.0;
    double dMc_Ed=0.0;
    double dMel_Rd=0.0;
    double dM_Rd=0.0;
    dFx    = aForce[0] = PrvsBase.dFx;
    dMy    = aForce[4] = PrvsBase.dMy;
    dMz    = aForce[5] = PrvsBase.dMz;
    dMa_Ed = PrvsBase.dMa_Ed;
    dMc_Ed = dMy;
    double dV_Ed_a = PrvsGd.dFz;
    double dV_Ed_c = PrvsBase.dFz;
    double dV_Ed = PrvsBase.dV_Ed;

    T_SECT_STIFFNESS CompoSect;
    CompoSect = dMy>=0.0 ? RebarSect : NonConSect;
    double dIyy_c = CompoSect.Ryy;
    double dCzp_c = CompoSect.Czp;
    double dCzm_c = CompoSect.Czm;  

    double dIyy_a = GirderStiffD.Ryy;

    // Shear buckling resistance, EN 1993-1-5,5 (22page)
    BOOL bEndSupport = TsesPosi.bUseStif ? TRUE : FALSE;
    int iEndpost = TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post,
    //BOOL bLongStiff = arLstiPosi.GetSize()==0 ? FALSE : TRUE;
    BOOL bTranStiff = TrstPosi.bUseStif;

    double dRigidPitch = TrstPosi.dPitch;
    double dInterRigPitch = TrstPosi.dDistRstif;
    double dNonRigPitch = TrstPosi.dDistNRstif;

    T_GENL_DATA EffGLineD = GLineData;
    T_GLINE_DGN_D EffGLineDgn;
    double da = dRigidPitch;

    int iClassSect = ClassD.iClassSect;

    T_DLINE_CALC_D DLineInfo; // 데이터를 받아와야함.
    DLineInfo.iClassFlgTop = m_ClassBase.iTopFlgClass;
    DLineInfo.iClassWeb    = m_ClassBase.iWebClass;
    DLineInfo.iClassFlgBot = m_ClassBase.iBotFlgClass;

    BOOL bAfter = TRUE;
    Get_EffectiveSectData(bPosiI, bAfter, dfy1, dfy2, da, aForce, SectElem, GLineData, DLineInfo, EffGLineD, EffGLineDgn);    
    CArray<T_DLINE_PART_D,T_DLINE_PART_D&> aTopFlangePart;
    CArray<T_DLINE_PART_D,T_DLINE_PART_D&> aBotFlangePart;
    CArray<T_DLINE_PART_D,T_DLINE_PART_D&> aWebPart;

    int i=0,j=0,k=0;
    Get_DLinePartData(DLineInfo, aTopFlangePart, aBotFlangePart, aWebPart);

    // EN 1993-1-5 7.1 (28page)
    // 7.1 Interaction between shear force, bending moment and axial force
    double dMf_Rd = m_PgbrBase.dMf_Rd;
    double dMpl_Rd = m_PgbrBase.dMpl_Rd_s; // Bending의 Mpl_Rd와 다를 수 있다. 

    double dM_Ed = m_PgbrDetail.dM_Ed; // for M,Ed/Mpl,Rd
    double dN_Ed = dFx;
    double dFactor_fr = 1.0;
    // 압축일때만 
    double dfy_ft=dfy1, dfy_fb=dfy1;
    double dAf_t=0.0, dAf_b=0.0;
    double dtf_t=0.0, dtf_b=0.0;
    Get_SectPropOfDLinePart(aTopFlangePart, dAf_t, dtf_t);
    Get_SectPropOfDLinePart(aBotFlangePart, dAf_b, dtf_b);
    if(dN_Ed<0.0) dFactor_fr = Get_ReductonFactorOfFlange4N_Ed(dgam_M2, dfy_ft, dfy_fb, dN_Ed, dAf_t, dAf_b);
    double dMf_Rd_factor = dFactor_fr * dMf_Rd;

    double dBendingRat = dMpl_Rd==0.0 ? 0.0 : dMf_Rd_factor/dMpl_Rd;
    double dBar_Eta1 = dMpl_Rd==0.0 ? 0.0 : max(dM_Ed/dMpl_Rd, dBendingRat); // Modify by GAY. See EN1993-1-5:2004(E), EQ(7.1)
    double dBar_Eta3 = 0.0;

    int iPartSize = DLineInfo.aPartLine.GetSize();
    int iGLinePartSize = GLineDgn.aPartLine.GetSize();
    ASSERT(iPartSize==iGLinePartSize);
    int iWebSize = aWebPart.GetSize();
    double dV_Edi = iWebSize==0 ? 0.0 : dV_Ed/(double)iWebSize; // Web의 수로 나눈다.   

    int iLamda_w_Type=0;
    int iLamda_w_Type_sub=2;
    double dLamda_w_sub_max=0.0;
    double dLamda_w_line_max=0.0;
    double dLamda_w_part_max=0.0;  
    double dLamda_w_part=0.0;  
    double da_si_cr=0.0, db_si_cr=0.0, dt_si_cr=0.0, dIsl_si_cr=0.0, dEpsilon_si_cr=0.0;
    double dk_tau_si_cr=0.0, dk_tau_st_si_cr=0.0, dk_tau_st_Limit_si_cr=0.0;
    double dIsli_cr=0.0;
    double dLamda_w_si_cr=0.0;  
    double deta3_max=0.0;
    double dRatio_Interac_MV_max=0.0;
    int iShearCheckPart=0;
    int iPartCount=0;

    double dVw_Rd_sum=0.0;
    double dVpl_Rd_sum=0.0;
    CArray<T_PRVS_Vbw_Rd_Param,T_PRVS_Vbw_Rd_Param&> aGenWebVbwRdParam;
    CArray<T_PRVS_Vbw_Rd_Param,T_PRVS_Vbw_Rd_Param&> aGenFlangeVbwRdParam;
    int nShearBuckCount=0;
    int nSubPannelCalcType=1;
    for(i=0; i<iPartSize; i++)
    {
        T_PRVS_Vbw_Rd_Param CalcVbwParam;
        T_DLINE_PART_D DLinePart = DLineInfo.aPartLine[i];
        T_GLINE_PART_DGN GLinePart = GLineDgn.aPartLine[i];
        int nPartType = GLinePart.nPartType;
        CalcVbwParam.strGroupName = GLinePart.strGroupName;
        double db_p = DLinePart.db_in;
        double dt_p = DLinePart.dt_in;
        double dAv_p = db_p*dt_p;

        double dfy_p = Get_fy4Thick(dfy1, dfy2, dt_p);
        double dEpsilon_p = Get_Epsilon(dfy_p);
        double dCheck_lim = 0.0;
        BOOL bStiffened = FALSE;
        BOOL bIsShearBuck=FALSE;
        double dk_tau_p = 0.0, dk_tau_st_p=0.0, dk_tau_st_Limit_p=0.0;
        int iLongStiffSize_p=0;
        double dIsl_part_sum=0.0;
        int iInternalSize = DLinePart.aInternal.GetSize();
        double dk_tau_i=0.0;
        int iInternalCount=0;
        int iSubPanelCount=0;
        for(j=0; j<iInternalSize; j++)
        {
            iInternalCount++;
            T_DGNLINE_LBD DLineLBD = DLinePart.aInternal[j];
            T_GLINE_DST GLineDST = GLinePart.aInternal[j];
            int iSubPanelSize = DLineLBD.aDgnBaseD.GetSize();      
            for(k=0; k<iSubPanelSize; k++)
            {
                iSubPanelCount++;
                T_GLINE_BASE GLineBaseD = GLineDST.aLineDgn[k];
                double db_si = GLineBaseD.db;
                double dt_si = GLineBaseD.dt;
                double dfy_si = Get_fy4Thick(dfy1, dfy2, dt_si);
                double dEpsilon_si = Get_Epsilon(dfy_si);
                double dIsl_si=0.0;
                double dk_tau_si=0.0;
                double dk_tau_st_si=0.0;
                double dk_tau_st_Limit_si=0.0;
                double dLamda_w_si=0.0;
                //Get_ShearBucklingCoeff(iLongStiffSize, db_i, da, dt_i, dIsl_part_sum, dk_tau_i);
                Get_SlendernessParamLamda(iLamda_w_Type_sub, 0, db_si, da, dt_si, dIsl_si, dEpsilon_si, dk_tau_si, dk_tau_st_si, dk_tau_st_Limit_si, dLamda_w_si);
                if(iSubPanelCount==1)
                {
                    dLamda_w_sub_max = dLamda_w_si;
                    db_si_cr=db_si, dt_si_cr=dt_si, dIsl_si_cr=dIsl_si, dEpsilon_si_cr=dEpsilon_si;
                    dk_tau_si_cr=dk_tau_si, dk_tau_st_si_cr=dk_tau_st_si, dk_tau_st_Limit_si_cr=dk_tau_st_Limit_si;
                    dIsli_cr = dIsl_si;
                    dLamda_w_si_cr=dLamda_w_si;
                    //nSubPannelCalcType = Get_SubPannelCalcType(0);
                }
                else
                {
                    if(dLamda_w_sub_max<dLamda_w_si) 
                    {
                        dLamda_w_sub_max = dLamda_w_si;
                        db_si_cr=db_si, dt_si_cr=dt_si, dIsl_si_cr=dIsl_si, dEpsilon_si_cr=dEpsilon_si;
                        dk_tau_si_cr=dk_tau_si, dk_tau_st_si_cr=dk_tau_st_si, dk_tau_st_Limit_si_cr=dk_tau_st_Limit_si;
                        dIsli_cr = dIsl_si;
                        dLamda_w_si_cr=dLamda_w_si;
                        //nSubPannelCalcType = Get_SubPannelCalcType(0);
                    }
                }
            }
            int iLongStiff = Get_StiffenerSize(GLineDST); //GLineDST.aStiffener.GetSize();
            BOOL bInternalLongStiff = iLongStiff>0 ? TRUE : FALSE;
            iLamda_w_Type = Get_Lamda_w_Type4Shear(bInternalLongStiff, TrstPosi); // Internal의 Lamda,w type 결정       
            double db_i = GLineDST.db;
            double dt_i = GLineDST.dt;
            double dfy_i = Get_fy4Thick(dfy1, dfy2, dt_i);
            double dEpsilon_i = Get_Epsilon(dfy_i);
            double dIsl_i = DLineLBD.dIsl_sh_sum*(1.0/3.0);
            double dk_tau_i=0.0;
            double dk_tau_st_i=0.0;
            double dk_tau_st_Limit_i=0.0;
            double dLamda_w_i=0.0;
            iLongStiffSize_p += iLongStiff;
            dIsl_part_sum += dIsl_i;
            //Get_ShearBucklingCoeff(iLongStiffSize, db_i, da, dt_i, dIsl_part_sum, dk_tau_i);
            Get_SlendernessParamLamda(iLamda_w_Type, iLongStiff, db_i, da, dt_i, dIsl_i, dEpsilon_i, dk_tau_i, dk_tau_st_i, dk_tau_st_Limit_i, dLamda_w_i);
            if(iInternalCount==1)
            {
                dLamda_w_line_max = dLamda_w_i;
            }
            else
            {
                if(dLamda_w_line_max<dLamda_w_i) dLamda_w_line_max = dLamda_w_i;
            }      
        }    
        if(iInternalCount<=0) continue; // internal이 있는 part만 계산하자. 
        iShearCheckPart++;

        double dtau_Ed_a=0.0, dtau_Ed_c=0.0;
        double dtau_Ed=0.0, dtau_Ed_ave=0.0, dtau_b_Rd=0.0, dQflg_a=0.0, dQflg_c=0.0, dbf=0.0;
        double dCzm_gap=0.0;
        if(nPartType==0 || nPartType==1) // Flange
        {
            // Before, tau_Ed,a
            dCzm_gap = OrgSect.Czm - GirderStiffD.Czm;
            Get_MomentofAreaOfFlange(i, dCzm_gap, OrgSect, GLineDgn, dQflg_a);
            dtau_Ed_a = dIyy_a*dt_p==0.0 ? 0.0 : dV_Ed_a*dQflg_a/(dIyy_a*dt_p);            

            // After, tau_Ed,c
            dCzm_gap = 0.0;
            Get_MomentofAreaOfFlange(i, dCzm_gap, OrgSect, GLineDgn, dQflg_c);
            dtau_Ed_c = dIyy_c*dt_p==0.0 ? 0.0 : dV_Ed_c*dQflg_c/(dIyy_c*dt_p);
            dtau_Ed = fabs(dtau_Ed_a + dtau_Ed_c);
            dtau_Ed_ave = 0.5*dtau_Ed;
        }

        BOOL bPartLongStiff = iLongStiffSize_p>0 ? TRUE : FALSE;
        iLamda_w_Type = Get_Lamda_w_Type4Shear(bPartLongStiff , TrstPosi); // Internal의 Lamda,w type 결정 
        bStiffened = iLamda_w_Type==1 || iLamda_w_Type==2;    

        Get_SlendernessParamLamda(iLamda_w_Type, iLongStiffSize_p, db_p, da, dt_p, dIsl_part_sum, dEpsilon_p, dk_tau_p, dk_tau_st_p, dk_tau_st_Limit_p, dLamda_w_part);
        Get_WidthThickRatio4Web(bStiffened, dEta, db_p, dt_p, dEpsilon_p, dk_tau_p, dCheck_lim, bIsShearBuck);
        if(bIsShearBuck) 
        {
            if(nPartType==2) nShearBuckCount++; // Web일 경우만. 
        }
        // sub 보다는 커야한다.
        double dLamda_w_cr = max(dLamda_w_sub_max, dLamda_w_part); 

        double dXwi=0.0;
        int iLamda_w_scope=0;
        Get_ContributionFromWebFactor(iEndpost,dEta,dLamda_w_cr,dXwi, iLamda_w_scope,m_dfy/1.1);
        double dVbw_Rd_p = Get_Vbw_Rd(dXwi, dfy_p, dAv_p, dgam_M1);
        double dVb_Rd_Limit = Get_Vbw_Rd_Limit(dEta, dfy_p, dAv_p, dgam_M1);
        // Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)
        double dVpl_Rd_p = dgam_M0==0.0 ? 0.0 : dEta*dAv_p*(dfy_p/sqrt(3.0))/dgam_M0; 
        if(nPartType==2) dVpl_Rd_sum += dVpl_Rd_p; // Web일 경우만 

        double dV_Rd_p = 0.0;
        double dEta3_p = 0.0;
        double dRatio_Interac_MV_part = 0.0;
        double dRho_sh=0.0; 
        if(nPartType==2)
        {
            if(bIsShearBuck)
            {
                dV_Rd_p = min(dVbw_Rd_p, dVb_Rd_Limit);
                dVw_Rd_sum += dV_Rd_p; // 각 Part가 제한값을 넘을 수 없으므로 dVbw_Rd_p를 더하지 말고 dV_Rd_p로 하자. 
            }
            else
            {
                dV_Rd_p = dVpl_Rd_p;
            }
            dEta3_p = dV_Rd_p==0.0 ? fabs(dV_Edi)/m_dZero : fabs(dV_Edi)/dV_Rd_p;
            //
            if(iClassSect==1 || iClassSect==2)
            {
                // 6.2.2.4 Bending and vertical shear. EN 1994-2,2005
                double dRho_sh_sqrt = dV_Rd_p==0.0 ? 0.0 : (2.0*fabs(dV_Ed_c/dV_Rd_p)-1.0);
                dRho_sh = pow(dRho_sh_sqrt,2);
                dRho_sh = min(dRho_sh,1.0); // 1보다는 크면 안된다.
            }
            else if(iClassSect==3 || iClassSect==4)
            {
                dRatio_Interac_MV_part = Get_RatioOfInteractionMV(dBar_Eta1, dBendingRat, dEta3_p);
            }
        }
        else if(nPartType==0 || nPartType==1)
        {
            dtau_b_Rd = Get_tau_b_Rd(dEta, dfy_p, dgam_M1);
            dEta3_p = dtau_b_Rd==0.0 ? fabs(dtau_Ed_ave)/m_dZero : fabs(dtau_Ed_ave)/dtau_b_Rd;
        }
        else ASSERT(0);

        if(i==0)
        {
            deta3_max = dEta3_p;
            dRatio_Interac_MV_max = dRatio_Interac_MV_part;
        }
        else
        {
            if(dEta3_p<deta3_max) deta3_max = dEta3_p;
            if(dRatio_Interac_MV_max<dRatio_Interac_MV_part) dRatio_Interac_MV_max = dRatio_Interac_MV_part;
        }


        if(IsPrintMode())
        {      
            double dAlpha_p = Get_Alpha4Lamda_w(da, db_p);
            CalcVbwParam.iStiffSize = iLongStiffSize_p;
            if(iLongStiffSize_p>0)
            {      
                int nLongStiffSize_sub_temp = 0; // Sub 이므로 보강재가 없다. 
                double dAlpha_si = Get_Alpha4Lamda_w(da, db_si_cr);
                Set_Calc_Vbw_SubPanel_Param(nLongStiffSize_sub_temp, db_si_cr, dt_si_cr, dAlpha_si, dk_tau_si_cr, dk_tau_st_si_cr, dk_tau_st_Limit_si_cr, dIsli_cr, dLamda_w_si_cr, CalcVbwParam.Lamda_w_SubD);
            }

            CalcVbwParam.nPartType = nPartType;
            CalcVbwParam.bStiffened = bStiffened;
            CalcVbwParam.bIsShearBuck = bIsShearBuck;
            CalcVbwParam.dVpl_Rd = dVpl_Rd_p;
            if(nPartType==2) // Web
            {
                Set_Calc_Vbw_Param(dAlpha_p, dk_tau_p, dk_tau_st_p, dk_tau_st_Limit_p, dIsl_part_sum, db_p, dt_p, dLamda_w_part, dLamda_w_cr, iLamda_w_Type, CalcVbwParam);
                Set_Calc_Vbw_Param2(iLamda_w_scope, dXwi, dVbw_Rd_p, dV_Rd_p, dVb_Rd_Limit, dV_Edi, dEta3_p, CalcVbwParam);                
                CalcVbwParam.dRatio_InteractionMV = dRatio_Interac_MV_part;

                aGenWebVbwRdParam.Add(CalcVbwParam);        
            }    
            else if(nPartType==0 || nPartType==1) // Flange
            {
                Set_Calc_Vbw_Param(dAlpha_p, dk_tau_p, dk_tau_st_p, dk_tau_st_Limit_p, dIsl_part_sum, db_p, dt_p, dLamda_w_part, dLamda_w_cr, iLamda_w_Type, CalcVbwParam);        
                Set_Calc_Vbw_Param2(iLamda_w_scope, dXwi, dVbw_Rd_p, dV_Rd_p, dVb_Rd_Limit, dV_Edi, dEta3_p, CalcVbwParam);
                Set_Calc_FlangeShearstress_Param(dtau_Ed_a, dtau_b_Rd, dV_Ed_a, dQflg_a, dIyy_a, dt_p, CalcVbwParam);
                CalcVbwParam.iLamda_w_scope = iLamda_w_scope;
                //
                CalcVbwParam.dQi_c = dQflg_c;
                CalcVbwParam.dIyyi_c = dIyy_c;
                CalcVbwParam.dtau_Ed_c = dtau_Ed_c;
                CalcVbwParam.dtau_Ed_sum = dtau_Ed;
                CalcVbwParam.dtau_Ed_ave = dtau_Ed_ave;

                aGenFlangeVbwRdParam.Add(CalcVbwParam);
            }
            else ASSERT(0);
        }    
    }

    // 각 Part별 계산에 적용할 Lamda_w_max를 찾는다. 



    // Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)
    double dAv = 0.0;
    double dV_Rd = 0.0;
    double dVpl_Rd = dVpl_Rd_sum;

    dAv = dEta*m_dHw*m_dTw;
    //dVpl_Rd = dgam_M0==0.0 ? 0.0 : dAv*(dfyd_w/sqrt(3.0))/dgam_M0; 

    //
    double dVb_Rd=dVw_Rd_sum;
    double dVb_Rd_org=dVw_Rd_sum;
    double dVbw_Rd=dVw_Rd_sum;
    double dVbf_Rd=0.0;
    double dVb_Rd_Limit = 0.0;
    //
    double dXw = 0.0;
    double dLamda_w = 0.0;
    double dk_tau = 0.0;


    BOOL bEta3Exceed05=FALSE;
    BOOL bCombinedEffe=FALSE;
    BOOL bEta1Ratio=FALSE;
    if(deta3_max>0.5) bEta3Exceed05 = TRUE;


    // Contribution from flange, EN 1993-1-5 , 25page

    double dRatio = 0.0;

    if(nShearBuckCount>0)
    {
        dV_Rd  = dVb_Rd;
        dRatio = dVb_Rd==0.0 ? fabs(dV_Ed)/m_dZero : fabs(dV_Ed)/dVb_Rd;
    }
    else
    {
        dV_Rd  = dVpl_Rd;
        dRatio = dVpl_Rd==0.0 ? fabs(dV_Ed)/m_dZero : fabs(dV_Ed)/dVpl_Rd;    
    }

    if(dRatio>1.0)
        PrvsBase.bOK = FALSE;
    else 
        PrvsBase.bOK = TRUE;


    if(bEta3Exceed05)
    {
        if(dRatio_Interac_MV_max<=1.0) bCombinedEffe = TRUE;
        else                           bCombinedEffe = FALSE;         
    }
    else
        bCombinedEffe = TRUE;

    if(PrvsBase.bOK && bEta3Exceed05)
    {
        if(ClassD.iClassSect==3 || ClassD.iClassSect==4)
        {
            if(!bCombinedEffe)
                PrvsBase.bOK = FALSE;    
        }
    }

    PrvsBase.iTopClass  = ClassD.iTopFlgClass;
    PrvsBase.iBotClass  = ClassD.iBotFlgClass;
    PrvsBase.iWebClass  = ClassD.iWebClass;
    PrvsBase.iSectClass = ClassD.iClassSect;    
    PrvsBase.dM_Ed = dM_Ed;  
    PrvsBase.dMf_Rd = dMf_Rd;
    PrvsBase.dMpl_Rd = dMpl_Rd;  
    PrvsBase.dVpl_Rd = dVpl_Rd;
    PrvsBase.dVbw_Rd = dVbw_Rd;
    PrvsBase.dVbf_Rd = dVbf_Rd;
    PrvsBase.dVb_Rd = dVb_Rd;
    PrvsBase.dV_Rd  = dV_Rd;
    PrvsBase.dEta1 = dBar_Eta1;
    PrvsBase.dEta3 = deta3_max;
    PrvsBase.dCombinedEffe = dRatio_Interac_MV_max; // Unit = None
    PrvsBase.bCombinedEffe = bCombinedEffe;
    PrvsBase.bEta3Exceed05 = bEta3Exceed05;  
    PrvsBase.dRatio = dRatio;
    //
    PrvsBase.dAv = dAv;        
    PrvsBase.deta = dEta;       
    //PrvsBase.dIsl_org = dIsl_org;   
    //PrvsBase.dIsl = dIsl;         
    PrvsBase.dXw = dXw;        
    //PrvsBase.dMax_Bar_Lamda_w = dMax_Bar_Lamda_w;
    //PrvsBase.dc = dc;
    //PrvsBase.dbf = dbf;         
    //PrvsBase.dtf = dtf;         
    PrvsBase.dVb_Rd_org = dVb_Rd_org;  
    //PrvsBase.dVb_Rd_Lim = dVb_Rd_Lim;  
    PrvsBase.bEta1Ratio = FALSE;


    // Detail
    PrvsDetail.GLineDgn = GLineDgn;

    if(IsPrintMode())
    {
        PrvsDetail.GLineDgn = GLineDgn;
        PrvsDetail.StressInfo4Class = m_DLineCalcD;
        PrvsDetail.aGenWebVbwRdParam.Copy(aGenWebVbwRdParam);
        PrvsDetail.aGenFlangeVbwRdParam.Copy(aGenFlangeVbwRdParam);
        PrvsDetail.iEndSupportType = iEndpost;
        PrvsDetail.dPlasticNA = m_PgbrBase.dPlasticNA;
        PrvsDetail.dN_slab = m_PgbrDetail.dN_slab;
        PrvsDetail.dNg_top = m_PgbrDetail.dNg_top;    
        PrvsDetail.dNg_bot = m_PgbrDetail.dNg_bot;    
        PrvsDetail.dMf_Rd_cal = dMf_Rd;
        PrvsDetail.dMf_N_Rd = dMf_Rd_factor;
        PrvsDetail.dReduFactor4N_Ed=dFactor_fr;  
        PrvsDetail.nShearBuckCount = nShearBuckCount;

    }  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_LongitudinalShearResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, BOOL bSLS, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                                                   CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,                                 
                                                                   T_CLAS_BASE& ClassD, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail)
{
    // Material.
    T_MATD_D MatdD = MatlElem.MatdD;
    // Steel.
    double dfy  = MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    // Concrete
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    // Rebar
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; // Steel로..

    // Material
    double dgam_M0 =m_dGamma_M0;
    double dgam_sr=m_dGamma_S;
    double dgam_c =m_dGamma_C;
    //
    double dgam_M1=m_dGamma_M1;
    //double dgam_M2=1.0;
    double dgam_V=m_dGamma_V;  

    double dfcd		= 0.67* dfck/dgam_c;
    double dfsd = dgam_sr==0.0 ? dfsk : dfsk/dgam_sr;

    // Section.
    T_SECT_D SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_SECTBASE_D SectBase = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
    T_SECT_STIFFNESS OrgSect;		OrgSect.Initialize();

    GetPureSectStiffD(SectElem, bPosiI, OrgSect);
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

    double dBc  =  Get_BcofSlab(bPosiI);  
    double dArea_sb = Get_AreaofSlab(bPosiI);
    double dCzp_sb = Get_CzpofSlab(bPosiI);
    double dCzm_sb = Get_CzmofSlab(bPosiI);
    double dtc  = dCzp_sb+dCzm_sb; // 직사각형
    double dZbar_sb = Get_ZcentroidofSlab(bPosiI);  
    // 전체높이
    double dHt  = Get_Height4Gen(bPosiI); 

    // Force
    double aForce[6]={0.0,0.0,0.0,0.0,0.0,0.0};
    double dFx=0., dMy=0., dMz=0.;
    double dNc=0.0;
    double dMc_Ed=0.0;
    double dM_Rd=0.0;
    double dMb_Rd=0.0;  
    double dN_Ed=0.0;    
    double dMy_a = PglsGd.dMy;
    dFx    = aForce[0] = PglsBase.dFx;
    dMy    = aForce[4] = PglsBase.dMy;
    dMz    = aForce[5] = PglsBase.dMz;
    double dV_Ed  = PglsBase.dFz;
    double dMa_Ed = PglsBase.dMa_Ed;

    T_PGBR_BASE PgbrBase = m_PgbrBase;
    double dMpl_Rd = PgbrBase.dMpl_Rd;
    double dMel_Rd = PgbrBase.dMel_Rd;
    double dNc_el  = PgbrBase.dNc_el;
    double dNc_f   = PgbrBase.dNc_f;  
    double dM_Ed  = dMy_a + dMy;

    ///////////////////////////////////////////////////////////////////////
    int iClassSect = ClassD.iClassSect;

    // EN 1994-2, 6.6 Shear connection, 46page
    int iNum = SconPosi.iNum;
    double dhsc    = SconPosi.dhsc;
    double dfu_sc  = SconPosi.dfu;
    double dDia_sc = SconPosi.dDia;
    double dSt_sc  = SconPosi.dSt;
    double dSpace  = SconPosi.dSpace;
    double dAlpha = 0.0;
    double dRat_sc = dDia_sc==0.0 ? 0.0 : dhsc/dDia_sc;
    int iRat_sc_type = 0;
    // 6.6.3.1(1), 52p, EN 1994-2
    if(3.0<=dRat_sc && dRat_sc<=4.0)
    {
        dAlpha = 0.2*(dRat_sc+1.0);
        iRat_sc_type = 1;
    }
    else if(dRat_sc>4.0)
    {
        dAlpha = 1.0;
        iRat_sc_type = 2;
    }
    else
    {
        dAlpha = 0.2*(dRat_sc+1.0);
        if(dAlpha>1.0)
            dAlpha = 1.0;
        iRat_sc_type = 3;
    }

    // Modify, Jaeoh. [4/28/2009] P_Rd 계산방법 수정 , 이혜연 사원님 요청 
    double dP_Rd1 = 0.8*dfu_sc*m_dPi*pow(dDia_sc,2)/4.0/dgam_V;
    double dP_Rd2 = dgam_V==0.0 ? 0.0 : 0.29*dAlpha*pow(dDia_sc,2)*sqrt(dfck*dEcm)/dgam_V;
    double dP_Rd = 0.0;
    double dP_Rd_SLS = 0.0;
    int iCheck_P_Rd = 0;
    if(dP_Rd1<dP_Rd2)  // 6.6.3.1(1), (6.18), (6.19)
    {
        dP_Rd = dP_Rd1;
        iCheck_P_Rd = 0;
    }
    else
    {
        dP_Rd = dP_Rd2;
        iCheck_P_Rd = 1;
    }  
    if(bSLS) dP_Rd_SLS = dks*dP_Rd;

    double dAz_Slab  = 0.0;
    double dAz_Hunch = 0.0;
    double dErat = dEcm == 0.0 ? 0.0 : dEs/dEcm;   // Es/Ec, Section에 있는 값.
    double dZbar = 0.0; // 철근 고려 안한. Original Section Stiffeness.  
    double dIyy_Org = 0.0;

    dZbar = OrgSect.Czm;
    dIyy_Org = OrgSect.Ryy;

    double dHg = GirderStiff.Czm+GirderStiff.Czp;
    if(dHg-dZbar>=0.0)
    {
        dAz_Slab  = dErat==0.0 ? 0.0 : (dArea_sb/dErat)*(dZbar_sb-dZbar);  
    }
    else
    {
        dAz_Slab  = dErat==0.0 ? 0.0 : (GirderStiff.Area)*(dZbar-GirderStiff.Czm);      
    }

    double de_d = 0.0; // e_d = 0, b_eff = Bc defined in Section Data, 07.06.** 답변메일 내용으로 반영 
    double dLength_v = de_d + dBc; // EN 1994-2, 6.6.2.3, Figure 6.12, 49p~51p 참고.
    double dAzI = dIyy_Org==0.0 ? 0.0 : (dAz_Slab)/dIyy_Org;
    //
    double dv_L_Ed = 0.0; // Shear flow
    double dv_L_Rd = 0.0;
    if(bSLS) dv_L_Rd = dSpace==0.0 ? 0.0 : dP_Rd_SLS*((double)iNum)/dSpace;
    else     dv_L_Rd = dSpace==0.0 ? 0.0 : dP_Rd*((double)iNum)/dSpace;
    //
    // Modification, [12/5/2011] 이대근 bj님 요청 
    // 정모멘트이면서 Class가 1,2이고 dM_Ed-dMel_Rd>0 인 단면은 dV_L_Ed = (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd) 사용
    // 정모멘트 이면서 위 경우가 아니거나, 부모멘트인 경우는 V_L_Ed = V_Ed
    double dV_L_Ed = 0.0;
    if((iClassSect==1 || iClassSect==2) && dM_Ed>=0.0 && dM_Ed-dMel_Rd>0.0)
    {
        dV_L_Ed = dMpl_Rd-dMel_Rd==0.0 ? 0.0 : (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd);
        dv_L_Ed = dLength_v==0.0 ? 0.0 : dV_L_Ed / dLength_v;
    }
    else
    {
        dV_L_Ed = dV_Ed;
        dv_L_Ed = fabs(dV_Ed)*dAzI;
    }

    //double dv_L_Ed2 = dLength_v==0.0 ? 0.0 : dV_L_Ed / dLength_v;
    //  
    double dv_Ed=dv_L_Ed/(2.0*dtc);
    double dnu = 0.6*(1.0-dfck/250.0);
    double dTheta_f = m_dPi/180.0*45.0; // 45로 가정
    double dv_Ed_lim =  dnu*dfcd*sin(dTheta_f)*cos(dTheta_f);// Designers' guide to EN 1994-2, 128p


    PglsBase.iSectClass = ClassD.iClassSect;  
    PglsBase.dNc       = dNc;           
    PglsBase.dNc_el    = dNc_el;        
    PglsBase.dNc_f     = dNc_f;         
    //PglsBase.dMa_Ed   = 0.0;        
    //PglsBase.dMc_Ed   = 0.0;        
    PglsBase.dMpl_Rd   = dMpl_Rd;       
    PglsBase.dMel_Rd   = dMel_Rd;   
    PglsBase.dV_L_Ed   = dV_L_Ed;
    PglsBase.dv_L_Ed   = dv_L_Ed;     // Slab로 계산된.    
    PglsBase.dv_L_Ed2  = m_dpgls_shear;    // V_L,Ed = Nc-Nc,el로 계산된.
    PglsBase.dP_Rd1    = dP_Rd1;
    PglsBase.dP_Rd2    = dP_Rd2;
    PglsBase.dP_Rd     = dP_Rd;         
    PglsBase.iCheck_P_Rd = iCheck_P_Rd;
    PglsBase.dv_L_Rd   = dv_L_Rd;     // P_Rd*(Stud개수)/간격
    PglsBase.dv_Ed     = dv_Ed;       // v_Ed(Slab의..)      
    PglsBase.dv_Ed_lim = dv_Ed_lim;   // v_Ed(Slab의..)      
    //
    PglsBase.dRatio = dv_L_Rd==0.0 ? fabs(dv_L_Ed)/m_dZero : fabs(dv_L_Ed)/fabs(dv_L_Rd);
    if(PglsBase.dRatio<1.0+m_dZero)
    {
        PglsBase.bOK = TRUE;
        //if(dv_Ed>dv_Ed_lim) PglsBase.bOK = FALSE;    
    }
    else
    {
        PglsBase.bOK = FALSE;
    }

    //
    PglsBase.dAlpha = dAlpha;
    PglsBase.dAzI = dAzI;

    PglsDetail.dmuy_c = dAzI*dIyy_Org;
    PglsDetail.dP_Rd_SLS = dP_Rd_SLS;
    if(IsPrintMode())
    {
        PglsDetail.dfu = dfu_sc;
        PglsDetail.iRat_sc_type = iRat_sc_type;
        PglsDetail.iNum = iNum;
        PglsDetail.dDsc = dDia_sc;
        PglsDetail.dhsc = dhsc;
        PglsDetail.dSpace = dSpace;
        PglsDetail.dLv = dLength_v;    
    }  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_PlasticMoment4GenSect(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, T_SECT_SECTBASE_D& SectBaseD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, double dPlasticNA, double& dMpl_Rd, T_PGBR_DETAIL& PgbrDetail)
{
    // Material.
    T_MATD_D MatdD = MatlElem.MatdD;
    // Steel.
    double dfy1  = MatdD.Data1.Design.S_Fy1;
    double dfy2 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    // Concrete
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    // Rebar
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; // Steel로..

    // Material
    double dgam_M0  = m_dGamma_M0;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;

    double dfcd		= 0.67* dfck/dgam_c;
    double dfsd = dgam_sr==0.0 ? dfsk : dfsk/dgam_sr;

    double dCzp = SectBaseD.Stiffness.Czp;
    double dCzm = SectBaseD.Stiffness.Czm;

    double dCpz = dCzp; // 철근정보를 어떻게 만들수 있나.??
    double dHt = Get_Height4Gen(bPosiI);
    double dHg = 0.0; // girder height, ??

    double dNeuC = bPositive ? dHt - dPlasticNA : dPlasticNA;
    // dPlasticNA는 정,부모멘트의 방향을 고려.

    double dCzp_slab=Get_CzpofSlab(bPosiI);
    double dCzm_slab=Get_CzmofSlab(bPosiI);
    double dHsb = dCzp_slab + dCzm_slab;
    double dZcen_BotPos=dHt-dHsb; // girder bottom에서 Slab bottom까지 거리   

    double aArea[4];
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aTopLine, aBotLine, aHCutLine;
    CArray<double,double&> aHCutTopThick;
    T_GSEC_POLYGON PolyD;
    double aZcen[4]={0.0}; // 자르는 직선기준으로 잘려진 단면의 도심 
    // Convert
    T_SECT_SECTBASE_D CalcSectBaseD; CalcSectBaseD.Initialize();
    Get_ConvertSectBase4GenLineData(SectBaseD, GLineDgn, CalcSectBaseD);
    CSectUtil::Get_SectPropertyOfCuttingArea(dPlasticNA, CalcSectBaseD, aArea, aZcen, PolyD, aTopLine, aBotLine, aHCutLine, aHCutTopThick);

    double dDist_PNAtoSlabTop=0.0, dDist_PNAtoSlabBot=0.0;
    if(dPlasticNA>dZcen_BotPos) 
    {
        dDist_PNAtoSlabTop = aZcen[0];
        dDist_PNAtoSlabBot = aZcen[1];
    }
    else
    {
        dDist_PNAtoSlabTop = dHt-dPlasticNA-dCzp_slab;
        dDist_PNAtoSlabBot = 0.0;    
    }

    double dMpl_gt=0.0, dMpl_gb=0.0; // girder top, bottom
    double dMpl_lgt=0.0, dMpl_lgb=0.0;
    double dMpl_st=0.0, dMpl_sb=0.0; // slab 압축
    double dMpl_rt=0.0, dMpl_rb=0.0; // Rebar 인장.   

    int i=0;
    dMpl_Rd=0.0;
    BOOL bYaxis=TRUE;
    // Rebar
    double dNr_top = 0.0, dNr_bot = 0.0;
    int iRbarSize = arRbarPosi.GetSize();
    if(!bPositive)
    {
        for(i=0; i<iRbarSize; i++)
        {
            _DGN_RBAR_CRC RbarD = arRbarPosi[i];
            double dAs = RbarD.dArea;
            double dzr = RbarD.dz-dPlasticNA; // 확인해야함. 
            double dN_bari = dAs*dfsd;
            double dMpl_ri = dAs*dfsd*fabs(dzr);
            if(dzr>=0.0) 
            {
                dNr_top += dN_bari;
                dMpl_rt += dMpl_ri;
            }
            else
            {
                dNr_bot += dN_bari;
                dMpl_rb += dMpl_ri;
            }
        }    
    }

    // girder
    double aMaxPt[2]={0.0}, aMinPt[2]={0.0};
    CSectUtil::Get_MaxMinPoint4GenPart(CalcSectBaseD, aMaxPt, aMinPt);
    double dCzm_NAz = aMinPt[1] + dPlasticNA; // 소성 중립축 좌표.

    double dNg_top=0.0, dNg_bot=0.0;
    double dNg_ltop=0.0, dNg_lbot=0.0;
    Get_PlasticMomentOfDividedPart(dfy1, dfy2, dCzm_NAz, PolyD, aTopLine, dMpl_gt, dNg_top);
    Get_PlasticMomentOfDividedPart(dfy1, dfy2, dCzm_NAz, PolyD, aBotLine, dMpl_gb, dNg_bot);
    if(aHCutLine.GetSize()>0)
        Get_PlasticMomentOfHoriDividedLine(dfy1, dfy2, dCzm_NAz, PolyD, aHCutLine, aHCutTopThick, dMpl_lgt, dMpl_lgb, dNg_ltop, dNg_lbot);

    // Slab
    double A_slab_top = aArea[0];
    double A_slab_bot = aArea[1];  
    double dN_slab_top = m_dAlpha*A_slab_top*dfcd;
    double dN_slab_bot = m_dAlpha*A_slab_bot*dfcd;
    dMpl_st = dN_slab_top*fabs(dDist_PNAtoSlabTop);
    dMpl_sb = dN_slab_bot*fabs(dDist_PNAtoSlabBot);

    double dN_slab=0.0;  
    if(bPositive)
    {      
        dN_slab = dN_slab_top;
        dMpl_Rd = (dMpl_gt+dMpl_lgt) + dMpl_gb + dMpl_st;
    }
    else
    {
        dN_slab = dN_slab_bot;
        dMpl_Rd = (dMpl_gt+dMpl_lgb) + dMpl_gb + dMpl_rt + dMpl_rb;    
    }

    if(IsPrintMode())
    {
        PgbrDetail.dN_slab = dN_slab;
        PgbrDetail.dNr_top = dNr_top;
        PgbrDetail.dNr_bot = dNr_bot;
        PgbrDetail.dNg_top = dNg_top + dNg_ltop;
        PgbrDetail.dNg_bot = dNg_bot + dNg_lbot;

    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_Mf_Rd_PlasticMoment4GenSect(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, T_SECT_SECTBASE_D& SectBaseD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, const T_GENL_DATA& GLineData, T_GLINE_DGN_D& GLineDgn, double& dMf_Rd)
{
    int i=0;
    double dPlasNA_sh=0.0;
    T_GLINE_DGN_D GLineDgnMoD = GLineDgn;
    GLineDgnMoD.aModifiedLine.RemoveAll();
    int nWebLineSize = m_aWebLineId.GetSize();    
    CMap<int,int&,int,int&> mapWebLine;
    for(i=0; i<nWebLineSize; i++)
    {
        int nWebId = m_aWebLineId[i];
        mapWebLine.SetAt(nWebId,nWebId);
    }

    int nMoLineSize=GLineDgn.aModifiedLine.GetSize(); 
    for(i=0; i<nMoLineSize; i++)
    {
        int nLineId = 0;    
        if(!mapWebLine.Lookup(i, nLineId))
        {
            GLineDgnMoD.aModifiedLine.Add(GLineDgn.aModifiedLine[i]);
        }    
    }

    double dPlasticNAOfMf_Rd=0.0;
    Get_PlasticNA4GenSect(bPosiI, bPositive, MatlElem, SectBaseD, arRbarPosi, GLineData, GLineDgnMoD, dPlasticNAOfMf_Rd);

    T_PGBR_DETAIL TempPgbrDetail;
    Get_PlasticMoment4GenSect(bPosiI, bPositive, MatlElem, SectBaseD, arRbarPosi, GLineData, GLineDgnMoD, dPlasticNAOfMf_Rd, dMf_Rd, TempPgbrDetail);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ElasticResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, BOOL bPositive, double aForceGd[6], double aForce[6], CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CPG_TRST_POSI& TrstPosi,
                                                        const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail)
{
    // Material.
    T_MATD_D MatdD = MatlElem.MatdD;
    // Steel.
    double dfy1  = MatdD.Data1.Design.S_Fy1;
    double dfy2 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    // Concrete
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    // Rebar
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; // Steel로..

    // Material
    double dgam_M0  = m_dGamma_M0;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;

    double dfcd		= 0.67* dfck/dgam_c;
    double dfsd = dgam_sr==0.0 ? dfsk : dfsk/dgam_sr;

    // Section.
    T_SECT_D SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_SECTBASE_D SectBase = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
    T_SECT_STIFFNESS NonConSect; NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;  RebarSect.Initialize();

    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

    double dHt = Get_Height4Gen(bPosiI);

    // Force
    double dFx=0., dMy=0., dMz=0.;  
    double dMa_Ed=0., dMc_Ed=0.0;
    double dMel_Rd=0.0;
    double dNc_el=0.0;
    dFx = aForce[0];
    dMy = aForce[4];
    dMz = aForce[5];  
    dMc_Ed = dMy;
    dMa_Ed = aForceGd[4];

    int iClassSect = PgbrBase.iSectClass;
    //
    double dForce2[6];
    T_DLINE_CALC_D DLineInfo;
    DLineInfo.iClassFlgTop = m_ClassBase.iTopFlgClass;
    DLineInfo.iClassWeb    = m_ClassBase.iWebClass;
    DLineInfo.iClassFlgBot = m_ClassBase.iBotFlgClass;

    // 축력에 대하여.
    memset(dForce2,0,sizeof(dForce2));
    dForce2[0] = aForceGd[0];
    //T_GENL_DATA EffGLineGdD = GLineData;
    T_GENL_DATA EffGLineD = GLineData;
    T_GLINE_DGN_D EffGLineDgn;
    double da = TrstPosi.dPitch;
    //
    double dDelta_z_a=0.0, dDelta_z_c=0.0;
    double dDelta_Ma_y=0.0, dDelta_Mc_y=0.0;

    // Girder만 있을 경우       
    T_SECT_STIFFNESS TempEffStiff4Fx; TempEffStiff4Fx.Initialize();
    T_SECT_STIFFNESS TempCompEffStiff4Fx; TempCompEffStiff4Fx.Initialize();

    T_SECT_STIFFNESS GirderEffStiff4Fx; GirderEffStiff4Fx.Initialize();
    BOOL bAfter = FALSE;
    if(iClassSect==4 && dForce2[0]<-m_dZero) // 압축일 경우만 계산한다. 
    {
        Get_EffectiveSectData(bPosiI, bAfter, dfy1, dfy2, da, dForce2, SectElem, GLineData, DLineInfo, EffGLineD, EffGLineDgn);      
        Get_EffectivePropertyForClass4(ElemK, bPosiI, bPositive, SectBase, EffGLineD, GirderEffStiff4Fx, TempCompEffStiff4Fx);
        dDelta_z_a = GirderStiff.Czm - GirderEffStiff4Fx.Czm; 
        dDelta_Ma_y = dDelta_z_a * fabs(dForce2[0]); 
    }
    else
    {
        GirderEffStiff4Fx = GirderStiff;
    }

    // Composite일 때 작용한 Force
    EffGLineD.Initialize();
    EffGLineD = GLineData;
    memset(dForce2,0,sizeof(dForce2));
    dForce2[0] = aForce[0];  
    bAfter = TRUE;
    T_SECT_STIFFNESS CompEffStiffness4Fx; CompEffStiffness4Fx.Initialize();
    if(iClassSect==4 && dFx<-m_dZero) // 압축일 경우만 계산한다. 
    {
        Get_EffectiveSectData(bPosiI, bAfter, dfy1, dfy2, da, dForce2, SectElem, GLineData, DLineInfo, EffGLineD, EffGLineDgn);    

        Get_EffectivePropertyForClass4(ElemK, bPosiI, bPositive, SectBase, EffGLineD, TempEffStiff4Fx, CompEffStiffness4Fx);

        if(bPositive)
        {
            dDelta_z_c = RebarSect.Czm - CompEffStiffness4Fx.Czm;
            dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
        }
        else
        {
            dDelta_z_c = NonConSect.Czm - CompEffStiffness4Fx.Czm;
            dDelta_Mc_y = dDelta_z_c * fabs(dFx); // if dDelta_z<0, negative moment.
        }
    }
    else
    {    
        CompEffStiffness4Fx = RebarSect; // 축력에 관한 것이므로.. 
    }  


    // 모멘트에 대하여.
    // Girder
    EffGLineD.Initialize();
    EffGLineD = GLineData;
    memset(dForce2,0,sizeof(dForce2));
    dForce2[4] = aForceGd[4]; 
    T_SECT_STIFFNESS GirderEffStiff4My; GirderEffStiff4My.Initialize();
    T_SECT_STIFFNESS TempCompEffStiffness4My; TempCompEffStiffness4My.Initialize();
    bAfter = FALSE;
    if(iClassSect==4)
    {
        Get_EffectiveSectData(bPosiI, bAfter, dfy1, dfy2, da, dForce2, SectElem, GLineData, DLineInfo, EffGLineD, EffGLineDgn);  
        Get_EffectivePropertyForClass4(ElemK, bPosiI, bPositive, SectBase, EffGLineD, GirderEffStiff4My, TempCompEffStiffness4My);
    }
    else
    {
        GirderEffStiff4My = GirderStiff;
    }

    // Composite
    EffGLineD.Initialize();
    EffGLineD = GLineData;
    memset(dForce2,0,sizeof(dForce2));
    dForce2[4] = dMy; 
    bAfter = TRUE;
    T_SECT_STIFFNESS TempEffStiffness4My; TempEffStiffness4My.Initialize();
    T_SECT_STIFFNESS CompEffStiffness4My; CompEffStiffness4My.Initialize();
    if(iClassSect==4)
    {
        Get_EffectiveSectData(bPosiI, bAfter, dfy1, dfy2, da, dForce2, SectElem, GLineData, DLineInfo, EffGLineD, EffGLineDgn);  
        Get_EffectivePropertyForClass4(ElemK, bPosiI, bPositive, SectBase, EffGLineD, TempEffStiffness4My, CompEffStiffness4My);
    }
    else
    {
        CompEffStiffness4My = bPositive ? RebarSect : NonConSect;
    }

    double dMy_gir = aForceGd[4] + dDelta_Ma_y;
    double dMy_com = dMy         + dDelta_Mc_y;

    CArray<T_DLINE_PART_D,T_DLINE_PART_D&> aTopFlangePart;
    CArray<T_DLINE_PART_D,T_DLINE_PART_D&> aBotFlangePart;
    CArray<T_DLINE_PART_D,T_DLINE_PART_D&> aWebPart;

    Get_DLinePartData(DLineInfo, aTopFlangePart, aBotFlangePart, aWebPart);
    double dAf_t=0.0, dAf_b=0.0;
    double dtf_top = 0.0;
    double dtf_bot = 0.0;
    Get_SectPropOfDLinePart(aTopFlangePart, dAf_t, dtf_top);
    Get_SectPropOfDLinePart(aBotFlangePart, dAf_b, dtf_bot);

    //
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;  
    double dfyd_t = dgam_M0==0.0 ? dfyt : dfyt/dgam_M0;
    double dfyd_b = dgam_M0==0.0 ? dfyb : dfyb/dgam_M0;

    T_SECT_STIFFNESS EffeStlSect; EffeStlSect.Initialize();
    T_SECT_STIFFNESS EffeComSect; EffeComSect.Initialize();

    m_EffeStlSect = EffeStlSect = GirderEffStiff4My; // Fx에 대한 Effective stiffness
    m_EffeComSect = EffeComSect = CompEffStiffness4My; // My에 대한 Effective stiffness

    // Before
    double dStl_Area = GirderEffStiff4Fx.Area;
    double dStl_Iyy  = GirderEffStiff4My.Ryy;
    double dStl_Izz  = GirderEffStiff4My.Ryy;
    double dStl_Czm  = GirderEffStiff4My.Czm;
    double dStl_Czp  = GirderEffStiff4My.Czp;
    double dStl_zDim = dStl_Czp+dStl_Czm;
    double dStl_Stop = dStl_Czp;
    // Composite
    double dArea_c = CompEffStiffness4Fx.Area;
    double dIyy_c  = CompEffStiffness4My.Ryy;  
    double dIzz_c  = CompEffStiffness4My.Rzz;
    double dCzm_c  = CompEffStiffness4My.Czm;
    double dCzp_c  = CompEffStiffness4My.Czp;
    double dStop = 0.0;

    // Rebar  
    int i=0;
    double dRbarZ_max = 0.0;
    int iRbarSize = arRbarPosi.GetSize();
    for(i=0; i<iRbarSize; i++)
    {
        _DGN_RBAR_CRC RbarD = arRbarPosi[i];
        if(i==0) { dRbarZ_max = RbarD.dz; }
        else
        {
            if(dRbarZ_max<RbarD.dz) 
            { 
                dRbarZ_max = RbarD.dz; 
            }
        }
    }  

    double dSIGa_top=0.0;
    double dSIGa_bot=0.0;  

    // 이대근 부장님과 확인필요.   
    // Modify, Jaeoh. [6/2/2011] Mel,Rd 계산방법 수정.
    // 정모멘트 : Slab Top, girder top, bottom 검토해서 불리한..
    // 부모멘트 : Slab Rebar, girder top, bottom 검토해서 불리한..
    double dFx_a=aForceGd[0];  
    double dMz_a=0.0;    
    double dStrStlTop = Calc_StressOfPosition(dFx_a, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, dStl_Czp);
    double dStrComTop = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCzp_c);
    double dStrStlBot = Calc_StressOfPosition(dFx_a, dMy_gir, dMz_a, dStl_Area, dStl_Iyy, dStl_Izz, 0.0, -dStl_Czm); 
    double dStrComBot = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, -dCzm_c);
    dSIGa_top = dStrStlTop + dStrComTop; // (-) 압축, (+) 인장.
    dSIGa_bot = dStrStlBot + dStrComBot;

    double dn = dEcm==0.0 ? 0.0 : dEs/dEcm;
    double dCentToSlabTop = dHt - dCzm_c;
    double dStrSlab_top = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToSlabTop);  
    dStrSlab_top = dn==0.0 ? 0.0 : dStrSlab_top/dn;
    double dCentToRbar = dRbarZ_max - dCzm_c;
    double dStrRbar_top = Calc_StressOfPosition(dFx, dMy_com, 0.0, dArea_c, dIyy_c, dIzz_c, 0.0, dCentToRbar);

    double dM_Ed_top = dSIGa_top*(dIyy_c/dCzp_c); // For shear resistance
    double dM_Ed_bot = dSIGa_bot*(dIyy_c/dCzm_c);
    dM_Ed_top = fabs(dM_Ed_top);
    dM_Ed_bot = fabs(dM_Ed_bot);
    double dM_Ed = max(dM_Ed_top, dM_Ed_bot);

    double dCzp_sb = Get_CzpofSlab(bPosiI);

    ////// 
    // k : 수정 2013.10.28  // Compression(-), Tension(+)
    //     수정 2014.04.17  :  개념 : Sig_a + k*Sig_c = Sig_y
    //                                Sig_c 와 Sig_y의 부호는 같도록 한다.
    //     수정 2014.04.29  :   Sig_a + Sig_Dela + k*Sig_c + Sig_Delc  = Sig_y

    //double dk_slab_temp = dStrSlab_top==0.0 ? dfcd/m_dZero : fabs(dfcd/dStrSlab_top);
    double dSigma_sl_gir     = 0.0; // 합성전 슬래브 단면 없다.
    double dSigma_sl_My      = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dMy           *dCentToSlabTop/dIyy_c /dn;   
    double dSigma_sl_DelMy   = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *dCentToSlabTop/dIyy_c /dn;   
    double dfcd_temp         = (dSigma_sl_My+dSigma_sl_DelMy<0.0)? (-1.0)*dfcd : dfcd; 
    double dk_slab           = dSigma_sl_My==0.0 ? dfcd/m_dZero : (dfcd_temp-dSigma_sl_gir-dSigma_sl_DelMy)/(dSigma_sl_My);

    //double dk_rbar_temp = dStrRbar_top==0.0 ? dfsd/m_dZero : fabs(dfsd/dStrRbar_top);
    double dSigma_rb_gir     = 0.0; // 합성전 철근 없다.
    double dSigma_rb_My      = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dMy           *dCentToRbar/dIyy_c;
    double dSigma_rb_DelMy   = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *dCentToRbar/dIyy_c;  
    double dfsd_temp         = (dSigma_rb_My+dSigma_rb_DelMy<0.0)? (-1.0)*dfsd : dfsd; 
    double dk_rbar           = dSigma_rb_My==0.0 ? dfsd/m_dZero : (dfsd_temp-dSigma_rb_gir-dSigma_rb_DelMy)/(dSigma_rb_My);

    //double dk_top_temp = dStrComTop==0.0 ? (dfyd_t-fabs(dStrStlTop))/m_dZero : (dfyd_t-fabs(dStrStlTop))/fabs(dStrComTop);
    double dSigma_top_gir    = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dMy_gir       *dStl_Czp/dStl_Iyy;
    double dSigma_top_My     = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dMy           *dCzp_c/dIyy_c;
    double dSigma_top_DelMy  = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *dCzp_c/dIyy_c;
    double dfyd_t_temp       = (dSigma_top_My+dSigma_top_DelMy<0.0)? (-1.0)*dfyd_t : dfyd_t; 
    double dk_top            = dSigma_top_My==0.0 ? 1.0/m_dZero : (dfyd_t_temp-dSigma_top_gir-dSigma_top_DelMy)/(dSigma_top_My);

    //double dk_bot_temp = dStrComBot==0.0 ? (dfyd_b-fabs(dStrStlBot))/m_dZero : (dfyd_b-fabs(dStrStlBot))/fabs(dStrComBot);
    double dSigma_bot_gir    = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dMy_gir       *(-dStl_Czm)/dStl_Iyy;
    double dSigma_bot_My     = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dMy           *(-dCzm_c)/dIyy_c;
    double dSigma_bot_DelMy  = (dIyy_c==0.0)?  1.0/m_dZero : (-1.)*dDelta_Mc_y   *(-dCzm_c)/dIyy_c;
    double dfyd_b_temp       = (dSigma_bot_My+dSigma_bot_DelMy<0.0)? (-1.0)*dfyd_t : dfyd_t; 
    double dk_bot            = dSigma_bot_My==0.0 ? 1.0/m_dZero : (dfyd_b_temp-dSigma_bot_gir-dSigma_bot_DelMy)/(dSigma_bot_My);
    //////

    BOOL bPositive2 = (dMy_com >=0.0)? TRUE : FALSE;
    double dk_min = Calc_kmin(bPositive2, dk_rbar, dk_slab, dk_top, dk_bot); 
    //   if(bPositive)
    //   {
    //     dk_min = min(dk_slab, min(dk_top, dk_bot));
    //     // Modify, Jaeoh. 이대근 부장님 요청. [6/10/2011], k<0 일경우 dMel_Rd를 fy관련 하여 계산해주기.
    //   }
    //   else
    //   {
    //     dk_min = min(dk_rbar, min(dk_top, dk_bot));    
    //   }

    //   if(dk_min<0.0)
    //   {
    //     double dMel_Rd_k_top = Get_Mel_RdOfElastResistance(dfyd_t, dFx_a, dMy_gir, dStl_Area, dStl_Iyy, dStl_Czp);
    //     double dMel_Rd_k_bot = Get_Mel_RdOfElastResistance(dfyd_b, dFx_a, dMy_gir, dStl_Area, dStl_Iyy, -dStl_Czm);
    //     dMel_Rd = min(dMel_Rd_k_top, dMel_Rd_k_bot);
    //   }
    //   else
    {
        //dMel_Rd = fabs(dMy_gir) + dk_min*fabs(dMy_com);   
        //double dMel_Rd_temp = dMy_gir + dk_min*dMy_com;
        double dMel_Rd_temp = dMa_Ed + dk_min*dMy +dDelta_Mc_y;
        dMel_Rd = fabs(dMel_Rd_temp);
    }

    // Positive 일 때만 계산.
    if(bPositive)
    {
        //double dza_sc = dHt - dStl_Czm - dCzp_sb; // Ma
        // [4/11/2014 joahn], dSig_c_mean 값은 거더를 포함하지 않는 것으로 수정, 최성기 CJ님 요청
        double dzc_sc = dHt - dCzm_c - dCzp_sb;
        double dSig_c_mean = dk_min*dMc_Ed/dIyy_c*dzc_sc/dn; // Slab중심에서의 응력 , dMa_Ed/dStl_Iyy*dza_sc
        double dNc_el_calc = dSig_c_mean * Get_AreaofSlab(bPosiI);
        dNc_el = fabs(dNc_el_calc);
    }

    PgbrBase.dMel_Rd = dMel_Rd;
    PgbrBase.dNc_el = dNc_el;
    PgbrBase.dElasticNA = dMy>=0.0 ? RebarSect.Czm : NonConSect.Czm;
    PgbrBase.dElasticNAEff = EffeComSect.Czm;// 
    //
    PgbrBase.dStrStlTop = dStrStlTop;
    PgbrBase.dStrComTop = dStrComTop;
    PgbrBase.dStrStlBot = dStrStlBot;
    PgbrBase.dStrComBot = dStrComBot;  
    PgbrBase.dSIGa_top = dSIGa_top;
    PgbrBase.dSIGa_bot = dSIGa_bot;  
    PgbrBase.dk_min = dk_min; 
    //////////////////////////////////////////////////////////////////////////
    PgbrDetail.dM_Ed_top      = dM_Ed_top;
    PgbrDetail.dM_Ed_bot      = dM_Ed_bot;
    PgbrDetail.dM_Ed          = dM_Ed;

    // BF의 dRho_c
    double dRho_c_BF = 1.0;
    if(DLineInfo.aPartLine.GetSize()<3 || DLineInfo.aPartLine[2].aInternal.GetSize()==0)
        dRho_c_BF= 1.0;
    else
        dRho_c_BF = DLineInfo.aPartLine[2].aInternal[0].dRho_c;

    PgbrDetail.dRho_c = dRho_c_BF;
    PgbrDetail.EffGirderFx    = GirderEffStiff4Fx;
    PgbrDetail.EffCompositeFx = CompEffStiffness4Fx;
    PgbrDetail.EffGirder      = EffeStlSect;
    PgbrDetail.EffComposite   = EffeComSect;


    if(IsPrintMode())
    {
        PgbrDetail.dDelMa_Ed = dDelta_Ma_y;
        PgbrDetail.dDelMc_Ed = dDelta_Mc_y;
    }  

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_Mel_RdOfElastResistance(double dfy, double dFx, double dMy, double dA, double dIy, double dz)
{
    if(dA < m_dZero || dIy < m_dZero || dz < m_dZero) {return 0.0;}
    double dMel_Rd_min = 0.0;

    double dIz=1.0;
    double dStress_a = Calc_StressOfPosition(dFx, dMy, 0.0, dA, dIy, dIz, 0.0, dz);
    if(dStress_a>0.0)
    {
        dMel_Rd_min = dIy/fabs(dz)*(dfy-dFx/dA);
    }
    else
    {
        dMel_Rd_min = dIy/fabs(dz)*(-dfy-dFx/dA);    
    }
    dMel_Rd_min = fabs(dMel_Rd_min);

    return dMel_Rd_min;
}

BOOL CDgnPlateGirder_IRC::Get_EffectivePropertyForClass4(ElemPairK ElemK, BOOL bPosiI, BOOL bPositive, const T_SECT_SECTBASE_D& SectBase, T_GENL_DATA& EffGLineD, T_SECT_STIFFNESS& GirderEffStiff, T_SECT_STIFFNESS& CompoEffStiff)
{
    T_SECT_D CalcSectD; CalcSectD.Initialize();      
    CalcSectD.SectBefore.Shape=D_SECT_SHAPE_REG_GEN;
    CalcSectD.SectBefore.SectI.aOuterPolygon.Add(EffGLineD.PolyD);

    int iEffLineSize = m_aEffLineIx.GetSize();
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aEffLine;
    aEffLine.SetSize(iEffLineSize);
    for(int i=0; i<iEffLineSize; i++)
    {
        int iEffLineIx = m_aEffLineIx[i];    
        aEffLine.SetAt(i, EffGLineD.aLine[iEffLineIx]);
    }  
    CalcSectD.SectBefore.SectI.aLine.Copy(aEffLine);  

    // vertex정보를 보고싶을 때 
    //Print_EffectivePolyAndLineData(EffGLineD.PolyD, aEffLine);

    // girder만 있을경우 
    CSectUtil::Get_LinetypeSectProperty(CalcSectD);
    GirderEffStiff = CalcSectD.SectBefore.SectI.Stiffness;

    // Composite 경우. 
    // Girder + Slab
    // Tapered가 아니여도 J단 값을 Set 해야함.
    CalcSectD.Initialize();
    CalcSectD.nStype = D_SECT_TYPE_TAPERED;
    CalcSectD.SectBefore.nStype = D_SECT_TYPE_COMPO_G;
    CalcSectD.SectBefore.SectI = SectBase;
    CalcSectD.SectBefore.SectI.aLine.Copy(aEffLine); // I이지만. aEffLine은 bPosiI에 데이터를 따른다. 

    int iStaIx=0, iEndIx=0;
    int iGenPartSize = CalcSectD.SectBefore.SectI.aGeneralPart.GetSize(); 
    for(int i=0; i<iGenPartSize; i++)
    {
        T_GSEC_PART GsecPartD = SectBase.aGeneralPart[i];
        if(GsecPartD.bPlane)
        {
            //
        }
        else // Line
        {      
            iStaIx = GsecPartD.IdxStart[0]; // 0=Polygon
            iEndIx = GsecPartD.IdxEnd[0];
            int iSize = iEndIx - iStaIx;
            if(iSize!=1) { ASSERT(0); return FALSE; }
            // Effective polygon으로 교체      
            CalcSectD.SectBefore.SectI.aOuterPolygon.SetAt(iStaIx, EffGLineD.PolyD);      
            GsecPartD.IdxStart[2] = 0;
            GsecPartD.IdxEnd[2] = aEffLine.GetSize();
            CalcSectD.SectBefore.SectI.aGeneralPart.SetAt(i, GsecPartD);
        }

    }
    CalcSectD.SectBefore.SectJ = CalcSectD.SectBefore.SectI;

    m_pDoc->m_pSectDB->CalcSectCompo_G(CalcSectD, TRUE);

    BOOL bScaleFactor=FALSE;
    T_SECT_INFO_D SectInfoD;
    double dScaleFactor[7] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; 
    double dPosition = bPosiI ? 0.0 : 1.0;
    BOOL bDgn=TRUE;
    CSectUtil::Get_SectInfoData(ElemK, dPosition, SectInfoD, bDgn);
    if(bPositive) SectInfoD.bCracked = FALSE;
    else          SectInfoD.bCracked = TRUE;
    SectInfoD.StiffMainD.Stiffness = CalcSectD.SectBefore.SectI.Stiffness;
    SectInfoD.StiffMainD.dYbar = CalcSectD.SectBefore.SectI.Design.YBar;
    SectInfoD.StiffMainD.dZbar = CalcSectD.SectBefore.SectI.Design.ZBar;
    SectInfoD.aSectPart[0].Stiff.Stiffness = GirderEffStiff;
    //SectInfoD.aSectPart[0].Stiff.dYbar = 
    SectInfoD.aSectPart[0].Stiff.dZbar = GirderEffStiff.Czm;

    T_SECT_PROP_D SectPropD;
    CSectUtil::Get_GeneralSectProperty(bScaleFactor, SectInfoD, dScaleFactor, SectPropD);    
    if(SectPropD.aSectProp.GetSize()>0)
        CompoEffStiff = SectPropD.aSectProp[0].StiffRbar.Stiffness;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_PlasticMomentOfDividedPart(double dfy1, double dfy2, double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, double& dMpl, double& dNgir)
{
    dMpl=0.0;
    int i=0;
    int iLineSize = aLineData.GetSize();  
    double dOutMaxPt[2]={0.};
    double dOutMinPt[2]={0.};
    //  T_GSEC_POLYGON TempPolyD = PolyD;
    //  CArray<T_GSEC_LINE, T_GSEC_LINE&> aTempLineData;
    //  aTempLineData.Copy(aLineData);
    //  CSectUtil::Get_MaxMinPoint4LineType(TempPolyD, aTempLineData, dOutMaxPt, dOutMinPt);


    for(i=0; i<iLineSize; i++)
    {
        T_GSEC_LINE LineD = aLineData[i];
        int v1 = LineD.v1;
        int v2 = LineD.v2;

        T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
        T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
        double dx1 = vtx1.dX;
        double dy1 = vtx1.dY;
        double dx2 = vtx2.dX;
        double dy2 = vtx2.dY;
        double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
        double dt = LineD.dThik;
        double dAl = dbi*dt;    
        double dfy = Get_fy4Thick(dfy1, dfy2, dt);
        double dfyd = dfy/m_dGamma_M0;
        double dzc = 0.5*(dy1+dy2) - dCzm_NAz;
        double dMpli = fabs(dzc)*dAl*dfyd;
        dMpl += dMpli;  
        dNgir += dAl*dfyd;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_PlasticMomentOfHoriDividedLine(double dfy1, double dfy2, double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, const CArray<double, double&>& aCutTopThick,
                                                             double& dMpl_top, double& dMpl_bot, double& dNgir_top, double& dNgir_bot)
{
    dMpl_top=dMpl_bot=0.0;
    dNgir_top=dNgir_bot=0.0;
    int i=0;
    int iLineSize = aLineData.GetSize();  
    double dOutMaxPt[2]={0.};
    double dOutMinPt[2]={0.};

    for(i=0; i<iLineSize; i++)
    {
        T_GSEC_LINE LineD = aLineData[i];
        int v1 = LineD.v1;
        int v2 = LineD.v2;    
        int nAlign = LineD.nAlign;         

        T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
        T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
        double dx1 = vtx1.dX;
        double dy1 = vtx1.dY;
        double dx2 = vtx2.dX;
        double dy2 = vtx2.dY;
        double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
        double dt = LineD.dThik;  
        double dt_t = aCutTopThick[i];
        double dt_b = dt - dt_t;
        double dAl_t = dbi*dt_t;    
        double dAl_b = dbi*dt_b;    
        double dfy = Get_fy4Thick(dfy1, dfy2, dt);
        double dfyd = dfy/m_dGamma_M0;

        T_GSEC_POLYGON LinePolyD;
        CSectUtil::GetCoorPointOfLine(nAlign, dx1, dy1, dx2, dy2, dt, LinePolyD);
        double dp1y = LinePolyD.aVertex[0].dY;
        double dp2y = LinePolyD.aVertex[1].dY;
        double dzc_t=0.0, dzc_b=0.0;

        if(dp2y<dp1y)            
        {
            dzc_t = (dp1y-0.5*dt_t) - dCzm_NAz;
            dzc_b = (dp2y+0.5*dt_b) - dCzm_NAz;
        }
        else if(dp1y<dp2y)
        {
            dzc_t = (dp2y-0.5*dt_t) - dCzm_NAz;
            dzc_b = (dp1y+0.5*dt_b) - dCzm_NAz;
        }    

        double dMpli_t = fabs(dzc_t)*dAl_t*dfyd;
        double dMpli_b = fabs(dzc_b)*dAl_b*dfyd;
        dMpl_top += dMpli_t;  
        dMpl_bot += dMpli_b;
        dNgir_top += dAl_t*dfyd;
        dNgir_bot += dAl_b*dfyd;
    }

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_Lamda3_DesignLifeInYears(int iYears) 
{
    double dLamda=0.0;

    switch(iYears)
    {
    case 50:      {dLamda = 0.871;      break;}
    case 60:      {dLamda = 0.903;      break;}
    case 70:      {dLamda = 0.931;      break;}
    case 80:      {dLamda = 0.956;      break;}
    case 90:      {dLamda = 0.979;      break;}
    case 100:     {dLamda = 1.000;      break;}
    case 120:     {dLamda = 1.037;      break;}
    default:      {dLamda = 0.871;      		}
    }

    return dLamda;
}

BOOL CDgnPlateGirder_IRC::Get_NonDimensionalSlenderness(double dL, double db, double dfy, double dEs, double dm, double dAwc, double dAf, double& dLamda)
{
    if(dEs*dm < m_dZero|| db < m_dZero)
    {
        dLamda = 0.0;
    }
    else
    {
        if(dAf < m_dZero)
            dLamda = 1.103*dL/db*sqrt(dfy/(dEs*dm));
        else
            dLamda = 1.103*dL/db*sqrt(dfy/(dEs*dm))*sqrt(1.0+dAwc/(3.0*dAf));
    }

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_PHIform(double dM1, double dM2, double dNu)
{
    double dMrat = 0.0;
    if(dM2*dM1 < m_dZero) // 부호가 다르면.
    {
        dMrat = 0.0;
    }
    else
    {
        if(fabs(dM1)>=fabs(dM2))	{dMrat = fabs(dM1) < m_dZero ? 0.0 : dM2/dM1; }
        else						{dMrat = fabs(dM2) < m_dZero  ? 0.0 : dM1/dM2;}
    }
    double dPHI = fabs(1.0+dNu) < m_dZero ? 0.0 : 2.0*(1.0-dMrat)/(1.0+dNu);

    return dPHI;
}

BOOL CDgnPlateGirder_IRC::Get_m_Value(double dNu, double dPHI, double dgamma, double& dm1, double& dm2, double& dm_min, double& dm)
{
    dm1 = 1.0+0.44*(1.0+dNu)*pow(dPHI,1.5)+(3.0+2.0*dPHI)*dgamma/(350.0-50.0*dNu);
    dm2 = 1.0+0.44*(1.0+dNu)*pow(dPHI,1.5)+(0.195+(0.05+dNu/100.0)*dPHI)*pow(dgamma,0.5);
    dm = dm_min = min(dm1, dm2);
    if(dm < 1.0){dm = 1.0;}

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_PHI_LT(double dAlpha, double dLamda_LT)
{
    double dPHI_LT = 0.5*(1.0+dAlpha*(dLamda_LT-0.2)+pow(dLamda_LT,2));
    return dPHI_LT;
}

double CDgnPlateGirder_IRC::Get_X_LT(double dPHI_LT, double dLamda_LT, double& dX_LT_org)
{
    double dX_LT = 0.0;
    double dX_LT_Parm1 = dPHI_LT*dPHI_LT - dLamda_LT*dLamda_LT;
    if(dX_LT_Parm1<0.0){dX_LT_Parm1 = 0.0;}

    dX_LT_org = fabs(dPHI_LT+sqrt(dX_LT_Parm1)) < m_dZero ? 0.0 : 1.0/(dPHI_LT+sqrt(dX_LT_Parm1));
    dX_LT = dX_LT_org;

    if(dX_LT>1.0){dX_LT = 1.0;}

    return dX_LT;
}


int CDgnPlateGirder_IRC::Get_BucklingCurveForCrossSection(double dh, double db, BOOL bIsSectI/* = TRUE*/)
{
    int iType;
    double dRatio = db < m_dZero ? 0.0 : dh/db;
    // Buckling curve
    // a0=0, a=1, b=2, c=3, d=4    
    if(dRatio < 2.0 + m_dZero && bIsSectI)
    {
        iType = 3; // c    
    }
    else
    {
        iType = 4; // d
    }

    return iType;
}

double CDgnPlateGirder_IRC::Get_ImperfectionFactor(int iCase)
{
    // Table 6.3: Imperfection factors for lateral torsional buckling curves. EN 1993-1-1, 6.3.2.2, 61p
    //IRC24 2010 Table 2 cl.507.1.1 and Cl.507.1.2.1 pg.48
    double dAlpha_LT=0.0;
    switch(iCase)
    {
    case 1:     {dAlpha_LT = 0.21;      break;}
    case 2:     {dAlpha_LT = 0.21;      break;}
    case 3:     {dAlpha_LT = 0.49;      break;}
    case 4:     {dAlpha_LT = 0.49;      break;}
    default:	{						break;}
    }

    return dAlpha_LT;
}

BOOL CDgnPlateGirder_IRC::Get_IslOfLongiStiff(int iType, double dh, double dt, double dtw, double dEpsi, double& dIsl)
{
    if(dh < m_dZero|| dt < m_dZero || dtw < m_dZero || dEpsi < m_dZero) {return FALSE;}

    double dIzz_sl = dt*pow(dh,3)/12.0;
    double dhw = dt+2.0*(15.0*dEpsi*dtw);
    double dIzz_mo = dhw*pow(dtw,3)/12.0;

    if(iType==1)
    {
        double dAw = dhw*dtw;
        double dAls = dh*dt;
        double dArea_t = dAw + dAls;
        double dZcen = (dAw*0.5*dtw+dAls*(dtw+0.5*dh))/dArea_t;
        double dd  = (dtw+0.5*dh) - dZcen;
        double dd2 = dZcen - 0.5*dtw;
        dIsl = dIzz_sl + dAls*pow(dd,2) + dIzz_mo + dAw*pow(dd2,2);
    }
    else if(iType==2)
    {
        double dAls = dh*dt;
        double dd = (0.5*dt+0.5*dh);
        dIsl = 2.0*(dIzz_sl + dAls*pow(dd,2)) + dIzz_mo;
    }
    else 
    {
        ASSERT(0);
        return FALSE;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_IslOfLongiStiff(const CPG_LSTI_DATA& InD, double dtw, double dEpsi, double& dIsl)
{
    double dIzz_sl	= InD.dIyy;
    double dhw		= InD.dt + 2.0*(15.0*dEpsi*dtw);
    double dIzz_mo	= dhw*pow(dtw,3)/12.0;
    double dAw		= dhw*dtw;
    double dAls		= InD.dArea;
    double dArea_t	= dAw + dAls;
    double dZcen	= (dAw*0.5*dtw + dAls*(dtw+InD.dCzm))/dArea_t;
    double dd		= (dtw+InD.dCzm) - dZcen;
    double dd2		= dZcen - 0.5*dtw;
    dIsl = dIzz_sl + dAls*pow(dd,2) + dIzz_mo + dAw*pow(dd2,2);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_IslOfLongStfn4GS(double dEpsi, T_GLINE_STIFFENER& StfnD, double dtw, double& dIsl)
{
    int nType = StfnD.nType;
    int nPosType = StfnD.nPosLine;
    double dZst = StfnD.dZbar;
    double dAst = StfnD.dA;
    double dIst = StfnD.dIy; 
    double dMoLen = 2.0*(15.0*dEpsi*dtw);
    if(nType==0)
    {
        dMoLen += StfnD.dSize[1];
    }
    else if(nType==1)
    {
        dMoLen += StfnD.dSize[2];
    }
    else if(nType==2)
    {
        dMoLen += 2.0*(dMoLen+StfnD.dSize[3]);
    }
    else ASSERT(0);

    double dAw = dMoLen*dtw;
    double dIw = dMoLen*pow(dtw,3)/12.0;
    if(nPosType==0 || nPosType==1) // Stiffener 하나. 
    {
        double dArea_t = dAw + dAst;
        double dZcen = dArea_t==0.0 ? 0.0 : (dAw*0.5*dtw+dAst*(dtw+dZst))/dArea_t;
        double dIsl_mo = dIw + dAw*pow(dZcen-0.5*dtw,2);
        double dIsl_st = dIst + dAst*pow(dtw+dZst-dZcen,2);
        dIsl = dIsl_mo + dIsl_st;
    }
    else if(nPosType==2)
    {
        double dIsl_st = dIst + dAst*pow(0.5*dtw+dZst,2);
        dIsl = dIw + 2.0*dIsl_st;
    }
    else
        ASSERT(0);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_SlendernessParamLamda(int iType, int iLongStiffSize, double dhw, double da, double dt, double dIsl, double dEpsi, 
                                                    double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit, double& dLamda)
{
    //   if(iType==1)
    //   {
    // 	  if(dt < m_dZero || dEpsi< m_dZero) {return FALSE;}
    // 		dLamda = dhw/(86.4*dt*dEpsi);
    //   }
    //   else if(iType==2)
    {
        if(/*da < m_dZero ||*/ dhw < m_dZero|| dt < m_dZero|| dEpsi < m_dZero) {return FALSE;}
        Get_ShearBucklingCoeff(iLongStiffSize, dhw,da,dt,dIsl,dk_tau, dk_tau_st, dk_tau_st_Limit);//dk_tau is k_v acc to IRC
        if(dk_tau < m_dZero) {return FALSE;}
        //dLamda = dhw/(37.4*dt*dEpsi*sqrt(dk_tau));
        double dTau_cr_e = dk_tau*m_dPi*m_dPi*m_dEs/(12.0*(1.0-pow(m_dPoisson,2)) * pow(dhw/dt,2));
        dk_tau_st_Limit = dTau_cr_e;
        dLamda = sqrt((m_dfy/1.1) / (sqrt(3.0)*dTau_cr_e));
    }
    //else ASSERT(0);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ShearBucklingCoeff(int iLongStiffSize, double dhw, double da, double dt, double dIsl, double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit)
{ 

    //IRC 22 Pg.22
    if(da == m_dZero || da == 0.0 ||dhw < m_dZero || dt < m_dZero) {dk_tau = 5.35;return FALSE;}
    // 
    //   double dk_rat = dIsl/(pow(dt,3)*dhw);
    //   double dk_tau_st1 = 9.0*pow(dhw/da,2);
    //   double dk_tau_st2 = pow(dk_rat,3);
    //   dk_tau_st = dk_tau_st1*pow(dk_tau_st2, 1./4.);
    //   dk_tau_st_Limit = 2.1/dt*pow(dIsl/dhw,1./3.);
    // 
    //   if(dk_tau_st < dk_tau_st_Limit){dk_tau_st = dk_tau_st_Limit;}
    // 
    //   double dk_tau1 = 6.3+0.18*dk_rat;
    //   double dk_tau2 = 2.2*pow(dk_rat,1./3.);
    //   double dAlpha = da/dhw;

    // EN 1993-1-5, A.3 Shear buckling coefficients
    if(iLongStiffSize==1 || iLongStiffSize==2)
    {
        //     if(dAlpha<3.0)
        //     {
        //       dk_tau = 4.1+dk_tau1/pow(dAlpha,2);      
        //     }
        //     else
        {
            //Transverse stiffener at support
            dk_tau = 5.35;//+4.0*pow(dhw/da,2);      
        }    
    }
    else // 없거나 3개이상 
    {
        if(da/dhw<1.0)	{dk_tau = 4.0+5.35*pow(dhw/da,2);}
        else				{dk_tau = 5.35+4.0*pow(dhw/da,2);}          
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ContributionFromWebFactor(int iEndpostType, double dEta, double dLamda, double& dXw, int& iLamda_w_scope,double dfyw)
{
    //IRC22 Pg.21

    if(dEta < m_dZero || dLamda < m_dZero) {return FALSE;}

    if(iEndpostType==0) // Rigid end post
    {
        if(dLamda<0.8)
        {
            dXw = dfyw/sqrt(3.0);
            iLamda_w_scope = 0;
        }
        else if(0.8 <  dLamda + m_dZero && dLamda < 1.2)
        {
            dXw = (1.0 - 0.8*(dLamda-0.8))*(dfyw/sqrt(3.0));    
            iLamda_w_scope = 1;
        }
        else if(dLamda>1.2)
        {
            dXw =dfyw/(dLamda*dLamda*sqrt(3.0));    
            iLamda_w_scope = 2;
        }
    }
    else if(iEndpostType==1) // Non-rigid end post
    {
        if(dLamda<0.8)
        {
            dXw = dfyw/sqrt(3.0);
            iLamda_w_scope = 0;
        }
        else if(0.8 <  dLamda + m_dZero && dLamda < 1.2)
        {
            dXw = (1.0 - 0.8*(dLamda-0.8))*(dfyw/sqrt(3.0));    
            iLamda_w_scope = 1;
        }
        else if(dLamda>1.2)
        {
            dXw =dfyw/(dLamda*dLamda*sqrt(3.0));    
            iLamda_w_scope = 2;
        }
    }
    else ASSERT(0);  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_bf4c_DB(bool bPosiI, T_SECT_D& SectD, bool bTopF, int nWebPart, double dEpsilon, double& dbf)
{
    dbf = 0.0;

    int nRealSectType = m_pDgnSectUtil->Get_RealSectType(&SectD);

    switch (nRealSectType)
    {
    case D_SECT_TYPE_COMPO_I:
        {
            if(bTopF)
            {
                double dtf		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
                double dbft		= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
                double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
                double dbf_lim	= 15.0*dEpsilon*dtf;
                double dbf_h	= (dbft-dTw)*0.5;

                if(dbf_h > dbf_lim)	{dbf = dTw+dbf_lim*2.0;}
                else				{dbf = dbft;}
            }
            else
            {
                double dtf		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
                double dbft		= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
                double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
                double dbf_lim	= 15.0*dEpsilon*dtf;
                double dbf_h	= (dbft-dTw)*0.5;

                if(dbf_h > dbf_lim)	{dbf = dTw+dbf_lim*2.0;}
                else                {dbf = dbft;}
            }
            break;
        }
    case D_SECT_TYPE_COMPO_B:
        {
            if(bTopF)
            {
                double dtf		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
                double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
                double dbft_out	= m_pDgnSectUtil->Get_Size_bft_l(&SectD, bPosiI) - dTw; // left and right side is same size of box section
                double dbft_in	= 0.5*m_pDgnSectUtil->Get_Size_bft_m(&SectD, bPosiI);
                double dbf_lim	= 15.0*dEpsilon*dtf;

                double dbft_eff_out	= dbft_out > dbf_lim ?  dbf_lim : dbft_out;
                double dbft_eff_in	= dbft_in > dbf_lim ?  dbf_lim : dbft_in;

                dbf = dbft_eff_out + dbft_eff_in + dTw;
            }
            else
            {
                double dtf		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
                double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
                double dbft_out	= m_pDgnSectUtil->Get_Size_bfb_l(&SectD, bPosiI) - dTw;
                double dbft_in	= m_pDgnSectUtil->Get_Size_bfb_m(&SectD, bPosiI);
                double dbf_lim  = 15.0*dEpsilon*dtf;

                double dbft_eff_out	= dbft_out > dbf_lim ?  dbf_lim : dbft_out;
                double dbft_eff_in	= dbft_in > dbf_lim ?  dbf_lim : dbft_in;

                dbf = dbft_eff_out + dbft_eff_in + dTw;
            }
            break;
        }
    case D_SECT_TYPE_COMPO_TUB:
        {
            if(bTopF)
            {
                double dtf		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
                double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
                double dbft_l	= m_pDgnSectUtil->Get_Size_bft_ll(&SectD, bPosiI) - dTw; // left and right side is same size of box section
                double dbft_r	= m_pDgnSectUtil->Get_Size_bft_l(&SectD, bPosiI) - m_pDgnSectUtil->Get_Size_bft_ll(&SectD, bPosiI);
                double dbf_lim	= 15.0*dEpsilon*dtf;

                double dbft_eff_l	= dbft_l > dbf_lim ?  dbf_lim : dbft_l;
                double dbft_eff_r	= dbft_r > dbf_lim ?  dbf_lim : dbft_r;

                dbf = dbft_eff_l + dbft_eff_r + dTw;
            }
            else
            {
                double dtf		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
                double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
                double dbft_out	= m_pDgnSectUtil->Get_Size_bfb_l(&SectD, bPosiI) - dTw;
                double dbft_in	= m_pDgnSectUtil->Get_Size_bfb_m(&SectD, bPosiI);
                double dbf_lim  = 15.0*dEpsilon*dtf;

                double dbft_eff_out	= dbft_out > dbf_lim ?  dbf_lim : dbft_out;
                double dbft_eff_in	= dbft_in > dbf_lim ?  dbf_lim : dbft_in;

                dbf = dbft_eff_out + dbft_eff_in + dTw;
            }
            break;
        }
    default:
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ClassifyClassSection(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_MATL_ELEM& MatlElem, 
                                                   CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
                                                   CPG_SCON_ELEM& SconElem, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn,
                                                   T_DLINE_CALC_D& DLineCalcD, T_CLAS_BASE& ClassD, ElemPairK ElemK)
{
    // 1. Material.
    T_MATD_D& MatdD	= MatlElem.MatdD;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;

    // 2. Section.  
    T_SECT_D& SectD	= SectElem.SectD;
    BOOL bTapered		= m_pDgnSectUtil->Is_TaperedSect(&SectD);
    int nSectType		= m_pDgnSectUtil->Get_RealSectType(&SectD);
    double dB1		= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTF1		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2		= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTF2		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);	
    double dtc		= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dgamma_s	= m_dGamma_M0;

    // Get Section Dimension
    BOOL bSectI_Type = (nSectType == D_SECT_TYPE_COMPO_I);
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dHw	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);

    double dfyt = MatlElem.MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatlElem.MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatlElem.MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t	= fabs(m_dGamma_M0) < m_dZero ? dfyt : dfyt/m_dGamma_M0;
    double dfyd_b	= fabs(m_dGamma_M0) < m_dZero ? dfyb : dfyb/m_dGamma_M0;
    double dfyd_w	= fabs(m_dGamma_M0) < m_dZero ? dfyw : dfyw/m_dGamma_M0;

    m_dH=dH	;
    m_dHw=dTw		;
    m_dTw_t=dTw_t	;
    m_dB1=dB1		;
    m_dtf1=dTF1	;
    m_dB2=dB2		;
    m_dtf2=dTF2	;
    m_dHw=dHw		;
    m_dHh=dHh		;
    m_dtc=dtc		;

    T_SECT_SECTBASE_D& SectBase = bTapered && !bPosiI ? SectD.CmpTapJ : SectD.SectBefore.SectI;  
    T_SECT_STIFFNESS GirderStiff, PureSect, NonConSect, RebarSect;
    GirderStiff.Initialize();PureSect.Initialize();NonConSect.Initialize();RebarSect.Initialize();
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);  		
    GetPureSectStiffD(SectElem, bPosiI, PureSect);  	
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    // 3. Force
    double dFx = aCptForce[0];
    double dMy = aCptForce[4];
    double dMz = aCptForce[5];
    m_dPu = dFx;
    m_dMuy=dMy;
    m_dMuz = dMz;
    m_dfy = dfy;

    // girder
    T_SECT_STIFFNESS GirderStiffD;
    GirderStiffD.Initialize();
    if(!bTapered)  
        GirderStiffD = SectD.SectBefore.Stiffness;
    else
        GirderStiffD = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;

    double dArea_g = GirderStiffD.Area;
    double dIyy_g  = GirderStiffD.Ryy;
    double dIzz_g  = GirderStiffD.Rzz;
    double dYbar_g = GirderStiffD.Cym;
    double dZbar_g = GirderStiffD.Czm;

    m_dZbar =dZbar_g;
    m_dYbar = dYbar_g;
    m_dArea = dArea_g;
    m_dRyy=dIyy_g;
    m_dRzz = dIzz_g;

    // 정모멘트만 IRC 22 2008 603.1.3 검토
    BOOL bPositiveMy = dMy > -m_dZero ? TRUE : FALSE;
    double dtfc = bPositiveMy ? dTF1 : dTF2; // Thick of Compression Flange
    double dbfc = bPositiveMy ? dB1 : dB2; //  Width of Compression Flange
    BOOL bChkRestrained = bPositiveMy; 

    // Shear connector.
    const CPG_SCON_POSI& SconPosi = bPosiI ? SconElem.SconPosi[0] : SconElem.SconPosi[1];
    BOOL bShearConn	= SconPosi.bUseShear; // Use Shear Connector (default=TRUE)
    int iSconNum		= SconPosi.iNum;      // Number of connectors(transverse)  
    double dSconSt	= SconPosi.dSt;       // C to C  
    double dSconSpace = SconPosi.dSpace;    // Longitudinal spacing 
    double dSconDia   = SconPosi.dDia;

    //IRC22 2008 603.1.3 and cl.606.9
    BOOL bSconCtoCLimit		= FALSE;
    BOOL bSconClearDistLimit	= FALSE;
    BOOL bSconLongCtoCLimit	= FALSE;
    double dSconCtoCLt		= (dfy < m_dZero)? 0.0 : 21.0*dtfc*sqrt(250./dfy);
    double dSconClearDistLt	= (dfy < m_dZero)? 0.0 : 14.0*dtfc*sqrt(250./dfy);
    double dSconLongCtoCLt	= min(4.0*dtc, 600.0);

    // Is steel compression flange restrained by shear connectors to clause 603.1.3?
    if(bChkRestrained)
    {
        if(dSconSt < dSconCtoCLt)			{bSconCtoCLimit = TRUE;}
        if(dSconSpace < dSconLongCtoCLt)	{bSconLongCtoCLimit = TRUE;}

        double dSconClearDist = (dbfc - ((iSconNum-1)*dSconSt + dSconDia))*0.5;
        if(dSconClearDist < dSconClearDistLt) {bSconClearDistLimit = TRUE;}
    }

    T_SECT_STIFFNESS SectStiffD = bPositiveMy ? RebarSect : NonConSect;

    double dPlasticNA = 0.0;
    int iFlangeClass = 0 , iWebClass = 0;
    BOOL bEffeSection = FALSE;
    BOOL bFlangeDeter	= TRUE; // Modify, Jaeoh.(08.03.14) 이대근 CJ님 요청으로 합성단면에서 콘크리트 슬래브에 연결되는 top flange는 자동으로 class 1으로 분류

    if(nSectType == D_SECT_TYPE_COMPO_G)
    {    
        Get_PlasticNA4GenSect(bPosiI, bPositiveMy, MatlElem, SectBase, arRbarPosi, GLineData, GLineDgn, dPlasticNA);
        SetPlasticNA(dPlasticNA);

        Calc_ClassifyClassSection4GenSect(bChkRestrained, dfy, dfy1, aGirderForce, aCptForce, dPlasticNA, GirderStiff, SectStiffD, PureSect, GLineData, GLineDgn, DLineCalcD);

        ClassD.iFlangeClass = DLineCalcD.iClassFlg;
        ClassD.iTopFlgClass = DLineCalcD.iClassFlgTop;
        ClassD.iBotFlgClass = DLineCalcD.iClassFlgBot;
        ClassD.iWebClass    = DLineCalcD.iClassWeb;
        ClassD.iClassSect   = DLineCalcD.iClass;

        m_aWebLineId.RemoveAll();
        m_aTopFlgLineId.RemoveAll();
        m_aBotFlgLineId.RemoveAll();
        Get_PartLineId(2, GLineDgn, m_aWebLineId); // Web
        Get_PartLineId(0, GLineDgn, m_aTopFlgLineId); // Top
        Get_PartLineId(1, GLineDgn, m_aBotFlgLineId); // Bottom
    }
    else
    {
        BOOL bMembTemp=FALSE;
        CPG_MEMB_ELEM MembElemTemp;  MembElemTemp.Initialize();
        if(!Get_CpgMembElem(ElemK, MembElemTemp)) {bMembTemp=FALSE;}

        CPG_MEMB_POSI MembPosiTemp;
        MembPosiTemp = bPosiI ? MembElemTemp.MembPosi[0] : MembElemTemp.MembPosi[1];
        double dL			= MembPosiTemp.dLu; // span length between the rigid supports.

        Get_PlasticNA(bPosiI, bPositiveMy,dL, MatlElem, SectD, arRbarPosi, dPlasticNA);
        SetPlasticNA(dPlasticNA);

        T_CLASS_IN_D_IRC ClassInD;
        Make_ClassInD(bPosiI, aGirderForce, aCptForce, SectElem, ClassInD);
        // 	if(!Get_SectionClass(bPosiI, bChkRestrained, SectD, ClassInD, MatdD.Data1.Design, ClassD))
        // 	{
        // 		ASSERT(0);
        // 		return FALSE;
        // 	}
        if(!Get_SectionClass(bPosiI, bChkRestrained, SectD, ClassInD, MatdD, ClassD))
        {
            ASSERT(0);
            return FALSE;
        }

    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_SectionClass(bool bPosiI, BOOL bChkRestrained, const T_SECT_D& SectD, const T_CLASS_IN_D_IRC& crClassInD, 
                                           const T_MATL_DESIGN& DgnMatD, T_CLAS_BASE& ClassD)
{
    double dfy	= DgnMatD.S_Fy1;
    double dfy1 = DgnMatD.S_Fy2;

    int iClass = 0, iClassSect = 0;
    int iClassFlg = 0, iClassWeb = 0;
    int iClassFlg_T = 0, iClassFlg_B = 0;
    double dIRCCLass = Calc_Class_General();
    m_bIsRolledSection = SectD.SectBefore.BuiltUpFlag;
    // 1. Top Flange Class
    int nTopPartSize = crClassInD.aTopFlangeD.GetSize();
    for(int nTop = 0; nTop < nTopPartSize; ++nTop)
    {
        iClass = 0;
        const T_CLASS_IN_UNIT_D_IRC& ClassInD = crClassInD.aTopFlangeD[nTop];
        CLASS_BASE_UNIT_IRC ClassResD;
        if(ClassInD.nType == 0)		{Calc_OutstandClassUnit(ClassInD, dfy, dfy1, iClass, ClassResD);}
        else if(ClassInD.nType == 1){Calc_InternalPlateClassUnit(bPosiI, SectD, ClassInD, dfy, dfy1, iClass, ClassResD, false);}
        else {ASSERT(0); return FALSE;}

        iClassFlg_T = bChkRestrained ? 1 : max(iClassFlg_T, ClassResD.iClass);

        if(nTop == 0 || iClassFlg_T <= ClassResD.iClass)
        {
            int nIndexT				= nTop == 0 ? 0 : 1;
            ClassD.iClass[nIndexT]	= ClassResD.iClass;
            ClassD.dPsi[nIndexT]	= ClassResD.dPsi;
            ClassD.dk_sig[nIndexT]	= ClassResD.dk_sig;
            ClassD.dRho[nIndexT]	= ClassResD.dRho;
            ClassD.dbeff[nIndexT]	= ClassResD.dbeff;
            ClassD.dbe1[nIndexT]	= ClassResD.dbe1;
            ClassD.dbe2[nIndexT]	= ClassResD.dbe2;
            ClassD.dBTR[nIndexT]	= ClassResD.dBTR;
            ClassD.dEpsilon			  = ClassResD.dEpsilon;
        }
    }

    // 2. Bottom Flange Class
    int nBotPartSize = crClassInD.aBotFlangeD.GetSize();
    for(int nBot = 0; nBot < nBotPartSize; ++nBot)
    {
        iClass = 0;
        const T_CLASS_IN_UNIT_D_IRC& ClassInD = crClassInD.aBotFlangeD[nBot];
        CLASS_BASE_UNIT_IRC ClassResD;
        if(ClassInD.nType == 0)		{Calc_OutstandClassUnit(ClassInD, dfy, dfy1, iClass, ClassResD);}
        else if(ClassInD.nType == 1){Calc_InternalPlateClassUnit(bPosiI, SectD, ClassInD, dfy, dfy1, iClass, ClassResD,false);}
        else {ASSERT(0); return FALSE;}

        iClassFlg_B = max(iClassFlg_B, ClassResD.iClass);

        if(nBot == 0 || iClassFlg_B <= ClassResD.iClass)
        {
            int nIndexB				= nBot == 0 ? 2 : 3;
            ClassD.iClass[nIndexB]	= ClassResD.iClass;
            ClassD.dPsi[nIndexB]	= ClassResD.dPsi;
            ClassD.dk_sig[nIndexB]	= ClassResD.dk_sig;
            ClassD.dRho[nIndexB]	= ClassResD.dRho;
            ClassD.dbeff[nIndexB]	= ClassResD.dbeff;
            ClassD.dbe1[nIndexB]	= ClassResD.dbe1;
            ClassD.dbe2[nIndexB]	= ClassResD.dbe2;
            ClassD.dBTR[nIndexB]	= ClassResD.dBTR;
            ClassD.dEpsilon			  = ClassResD.dEpsilon;
        }
    }

    iClassFlg = max(iClassFlg_T, iClassFlg_B);

    // 3. Web Class
    int nWebPartSize = crClassInD.aWebD.GetSize();
    for(int nWeb = 0; nWeb < nWebPartSize; ++nWeb)
    {
        iClass = 0;
        const T_CLASS_IN_UNIT_D_IRC& ClassInD = crClassInD.aWebD[nWeb];
        CLASS_BASE_UNIT_IRC ClassResD;
        if(ClassInD.nType == 1){Calc_InternalPlateClassUnit(bPosiI, SectD, ClassInD, dfy, dfy1, iClass, ClassResD);}
        else {ASSERT(0); return FALSE;}

        if(iClassWeb < ClassResD.iClass)
        {
            iClassWeb			= ClassResD.iClass;
            ClassD.iWebDeter	= ClassResD.iWebDeter;
            ClassD.iClass[4]	= ClassResD.iClass;
            ClassD.dPsi[4]		= ClassResD.dPsi;
            ClassD.dk_sig[4]	= ClassResD.dk_sig;
            ClassD.dRho[4]		= ClassResD.dRho;
            ClassD.dbeff[4]		= ClassResD.dbeff;
            ClassD.dbe1[4]		= ClassResD.dbe1;
            ClassD.dbe2[4]		= ClassResD.dbe2;
            ClassD.dBTR[4]	= ClassResD.dBTR;
            ClassD.dEpsilon			  = ClassResD.dEpsilon;
        }
    }

    iClassSect = max(iClassFlg, iClassWeb);

    if(iClassWeb==3 && (iClassFlg==1 || iClassFlg==2)) // EN 1993-1-1 : 2005, 5.5.2(11)
    {
        iClassSect = 2;
    }

    ClassD.iTopFlgClass	= iClassFlg_T;
    ClassD.iBotFlgClass	= iClassFlg_B;
    ClassD.iFlangeClass = iClassFlg;
    ClassD.iWebClass	= iClassWeb;
    ClassD.iClassSect	= iClassSect;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_SectionClass(bool bPosiI, BOOL bChkRestrained, const T_SECT_D& SectD, const T_CLASS_IN_D_IRC& crClassInD, 
                                           const T_MATD_D& DgnMatD, T_CLAS_BASE& ClassD)
{
    //double dfy	= DgnMatD.S_Fy1; DgnMatD.Data1.DesignTopFlange.S_Fy1
    //double dfy1 = DgnMatD.S_Fy2;

    int iClass = 0, iClassSect = 0;
    int iClassFlg = 0, iClassWeb = 0;
    int iClassFlg_T = 0, iClassFlg_B = 0;
    double dIRCCLass = Calc_Class_General();
    m_bIsRolledSection = SectD.SectBefore.BuiltUpFlag;
    // 1. Top Flange Class
    int nTopPartSize = crClassInD.aTopFlangeD.GetSize();
    for(int nTop = 0; nTop < nTopPartSize; ++nTop)
    {
        iClass = 0;
        const T_CLASS_IN_UNIT_D_IRC& ClassInD = crClassInD.aTopFlangeD[nTop];
        CLASS_BASE_UNIT_IRC ClassResD;
        if(ClassInD.nType == 0)		{Calc_OutstandClassUnit(ClassInD, DgnMatD.Data1.DesignTopFlange.S_Fy1, DgnMatD.Data1.DesignTopFlange.S_Fy1, iClass, ClassResD);}
        else if(ClassInD.nType == 1){Calc_InternalPlateClassUnit(bPosiI, SectD, ClassInD,DgnMatD.Data1.DesignTopFlange.S_Fy1, DgnMatD.Data1.DesignTopFlange.S_Fy1, iClass, ClassResD, false);}
        else {ASSERT(0); return FALSE;}

        iClassFlg_T = /*bChkRestrained ? 1 :*/ max(iClassFlg_T, ClassResD.iClass);

        if(nTop == 0 || iClassFlg_T <= ClassResD.iClass)
        {
            int nIndexT				= nTop == 0 ? 0 : 1;
            ClassD.iClass[nIndexT]	= ClassResD.iClass;
            ClassD.dPsi[nIndexT]	= ClassResD.dPsi;
            ClassD.dk_sig[nIndexT]	= ClassResD.dk_sig;
            ClassD.dRho[nIndexT]	= ClassResD.dRho;
            ClassD.dbeff[nIndexT]	= ClassResD.dbeff;
            ClassD.dbe1[nIndexT]	= ClassResD.dbe1;
            ClassD.dbe2[nIndexT]	= ClassResD.dbe2;
            ClassD.dBTR[nIndexT]	= ClassResD.dBTR;
            ClassD.dEpsilon			  = ClassResD.dEpsilon;
        }
    }

    // 2. Bottom Flange Class
    int nBotPartSize = crClassInD.aBotFlangeD.GetSize();
    for(int nBot = 0; nBot < nBotPartSize; ++nBot)
    {
        iClass = 0;
        const T_CLASS_IN_UNIT_D_IRC& ClassInD = crClassInD.aBotFlangeD[nBot];
        CLASS_BASE_UNIT_IRC ClassResD;
        if(ClassInD.nType == 0)		{Calc_OutstandClassUnit(ClassInD, DgnMatD.Data1.DesignBotFlange.S_Fy1, DgnMatD.Data1.DesignBotFlange.S_Fy1, iClass, ClassResD);}
        else if(ClassInD.nType == 1){Calc_InternalPlateClassUnit(bPosiI, SectD, ClassInD, DgnMatD.Data1.DesignBotFlange.S_Fy1, DgnMatD.Data1.DesignBotFlange.S_Fy1, iClass, ClassResD,false);}
        else {ASSERT(0); return FALSE;}

        iClassFlg_B = max(iClassFlg_B, ClassResD.iClass);

        if(nBot == 0 || iClassFlg_B <= ClassResD.iClass)
        {
            int nIndexB				= nBot == 0 ? 2 : 3;
            ClassD.iClass[nIndexB]	= ClassResD.iClass;
            ClassD.dPsi[nIndexB]	= ClassResD.dPsi;
            ClassD.dk_sig[nIndexB]	= ClassResD.dk_sig;
            ClassD.dRho[nIndexB]	= ClassResD.dRho;
            ClassD.dbeff[nIndexB]	= ClassResD.dbeff;
            ClassD.dbe1[nIndexB]	= ClassResD.dbe1;
            ClassD.dbe2[nIndexB]	= ClassResD.dbe2;
            ClassD.dBTR[nIndexB]	= ClassResD.dBTR;
            ClassD.dEpsilon			  = ClassResD.dEpsilon;
        }
    }

    iClassFlg = max(iClassFlg_T, iClassFlg_B);

    // 3. Web Class
    int nWebPartSize = crClassInD.aWebD.GetSize();
    for(int nWeb = 0; nWeb < nWebPartSize; ++nWeb)
    {
        iClass = 0;
        const T_CLASS_IN_UNIT_D_IRC& ClassInD = crClassInD.aWebD[nWeb];
        CLASS_BASE_UNIT_IRC ClassResD;
        if(ClassInD.nType == 1){Calc_InternalPlateClassUnit(bPosiI, SectD, ClassInD,DgnMatD.Data1.DesignWeb.S_Fy1, DgnMatD.Data1.DesignWeb.S_Fy1, iClass, ClassResD);}
        else {ASSERT(0); return FALSE;}

        if(iClassWeb < ClassResD.iClass)
        {
            iClassWeb			= ClassResD.iClass;
            ClassD.iWebDeter	= ClassResD.iWebDeter;
            ClassD.iClass[4]	= ClassResD.iClass;
            ClassD.dPsi[4]		= ClassResD.dPsi;
            ClassD.dk_sig[4]	= ClassResD.dk_sig;
            ClassD.dRho[4]		= ClassResD.dRho;
            ClassD.dbeff[4]		= ClassResD.dbeff;
            ClassD.dbe1[4]		= ClassResD.dbe1;
            ClassD.dbe2[4]		= ClassResD.dbe2;
            ClassD.dBTR[4]	= ClassResD.dBTR;
            ClassD.dEpsilon			  = ClassResD.dEpsilon;
        }
    }

    iClassSect = max(iClassFlg, iClassWeb);

    if(iClassWeb==3 && (iClassFlg==1 || iClassFlg==2)) // EN 1993-1-1 : 2005, 5.5.2(11)
    {
        iClassSect = 2;
    }

    ClassD.iTopFlgClass	= iClassFlg_T;
    ClassD.iBotFlgClass	= iClassFlg_B;
    ClassD.iFlangeClass = iClassFlg;
    ClassD.iWebClass	= iClassWeb;
    ClassD.iClassSect	= iClassSect;

    return TRUE;
}

double CDgnPlateGirder_IRC::interpolate(double x1, double x2, double p1, double p2, double x)
{
    return ( p1 + (p2-p1)*(x-x1)/(x2-x1) ) ;
}

BOOL CDgnPlateGirder_IRC::Get_PlasticNA(BOOL bPosiI, BOOL bPositive, double dL,CPG_MATL_ELEM& MatlElem, const T_SECT_D& crSectD, 
                                        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, double& dPlasticNA, BOOL bNeglectWeb)
{
    // 1. Initialize
    dPlasticNA = 0.0;
    double dPlasticNAIRC = 0.0;
    // 2. Section
    double dH	= m_pDgnSectUtil->Get_Size_H(&crSectD, bPosiI);  
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&crSectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&crSectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);
    double dh	= m_pDgnSectUtil->Get_Size_hw(&crSectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&crSectD); 
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&crSectD); 
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&crSectD); 
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&crSectD, bPosiI);   

    double dZp = dB1*dTf1*(dh-dTf1) + 0.25*dTw*(dh-2*dTf1)*(dh-2*dTf1);
    // 3. Material
    double dgam_M0  = m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c	= m_dGamma_C;
    double dfy		= MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfck		= MatlElem.dfck;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;
    double dfcd		= 0.54* dfck/dgam_c;
    double dfsd		= fabs(dgam_sr)	< m_dZero ? dfsk : dfsk/dgam_sr;
    double dfyt = MatlElem.MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatlElem.MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatlElem.MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t	= fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b	= fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w	= fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;
    BOOL bPlasticClass=TRUE;

    double dbt		= 0.0;
    double dbc		= 0.0;
    double dc		= 0.0;
    double dAlpha	= 0.0;
    double dPsi		= 0.0;
    double dk_sig	= 0.0;
    //calculation for beff factor

    dIRCL = m_dTotalLength;
    if ((dBc/dIRCL)>0.5)
    {
        //ASSERT(0);
        //return FALSE;
    }
    double dBeffFact = 0.5;
    double dLimL = dIRCL/8;
    double dLimB = dBc/2;
    if (bissimplysup)
    {	
        if (bOuter)
        {
            double dBeff = min(dLimL,dBc/2.0)+dLimB;
            dBeffFact = dBeff/dBc;
        }
        else
        {
            double dBeff = min(dIRCL/4.0,dBc);
            dBeffFact = dBeff/dBc;
        }
    }
    else
    {
        //Continuous girders checked at internal supports only
        double dRatio = dBc/dIRCL;
        double dPsi = 0.0;
        if(dRatio<=0.02) dPsi	= interpolate(0.02,0,0.77,1,dRatio);
        else if(dRatio<=0.05) dPsi	= interpolate(0.05,0.02,0.58,0.77,dRatio);
        else if(dRatio<=0.10) dPsi	= interpolate(0.1,0.05,0.41,0.58,dRatio);
        else if(dRatio<=0.20) dPsi	= interpolate(0.2,0.1,0.24,0.41,dRatio);
        else if(dRatio<=0.30) dPsi	= interpolate(0.3,0.2,0.15,0.24,dRatio);
        else if(dRatio<=0.40) dPsi	= interpolate(0.4,0.3,0.12,0.15,dRatio);
        else if(dRatio<=0.50) dPsi	= interpolate(0.5,0.4,0.11,0.12,dRatio);
        else dPsi = 1.0;

        if (bOuter)
        {
            double dBeff = dPsi*0.925*dBc;
            dBeffFact = dBeff/dBc;
        }
        else
        {
            double dBeff = dPsi*dBc;
            dBeffFact = dBeff/dBc;
        }


    }
    // 	if (bissimplysup)
    // 	{
    // 		dBeffFact = 1.0;
    // 	}
    // 4. Calc. Each Part's Force

    //double dConcF	= dBc*dtc*m_dAlpha*dfcd;

    //calculate eta and lambda
    double dEta = 0.0;
    if      (dfck < 60.0+cDGN_Zero)  dEta = 0.80;
    else if (dfck < 110.0+cDGN_Zero) dEta = 0.80 - (dfck-60.0)/500.0;
    else	  				                 dEta = 0.70; 

    double dLambda = 0.0;
    if      (dfck <60.0+cDGN_Zero) dLambda = 1.0;//IRC112:2011,A2-35)
    else if (dfck <110.0+cDGN_Zero) dLambda = 1.0 - (dfck-60.0)/250.0;//IRC112:2011,A2-36)
    else                           dLambda = 0.8;

    // 7. Calc. Plastic Bending Resistance

    double dConcF	=    dfcd* dBc*dtc;
    double dStopfF	= dB1*dTf1*dfyd_t;
    double dSbotfF	= dB2*dTf2*dfyd_b;
    double dSwebF	= dH*dTw_t*dfyd_w;


    double dIRCAlpha = m_dfy/(0.36*dfck*1.1);
    double dIRCxu = 0.0;
    //double dBeffFact = 0.5;
    if(dBeffFact<0.000000001)  dBeffFact = 0.5;
    m_dBeffFactor = dBeffFact;
    dIRCxu=dIRCAlpha*m_dArea/(dBeffFact*dBc);
    double dIRCConcF	= 0.54*dfck*dBeffFact*dBc*dIRCxu/m_dGamma_C;
    double dIRCStopfF	= dB1*dTf1*dfy/1.1;
    double dIRCSbotfF	= dB2*dTf2*dfy/1.1;
    double dIRCSwebF	= bNeglectWeb ? 0.0 : (dh*dTw_t*dfy/1.1);
    double dGirderF = dStopfF + dSbotfF + dSwebF;  

    double dSlabArea = dBeffFact*dBc*dtc;
    double dSteelArea = dIRCAlpha*m_dArea;
    double dSlabSteel = dSlabArea + (2*dIRCAlpha*dB1*dTf1);
    if (bPositive)
    {
        if (dSlabArea>dSteelArea)
        {
            //PNA in slab
            dPlasticNAIRC = dIRCAlpha*m_dArea/(dBeffFact*dBc);
        }
        else if (dSlabArea>dSteelArea && dSteelArea<dSlabSteel)
        {
            //PNA in steel flange
            dPlasticNAIRC=dtc+((dSteelArea-dSlabArea)/(2.0*dB1*dIRCAlpha));
        }
        else if (dSlabSteel<dSteelArea)
        {
            //PNA in web
            dPlasticNAIRC=dtc+dTf1+(((dIRCAlpha*(m_dArea-(dB1*dTf1+dB2*dTf2)))-dSlabArea)/(2.0*dTw_t*dIRCAlpha));
        }

    }
    else
    {
        double dRebarF	= 0.0;
        // Rebar Info.
        for(int i=0; i < arRbarPosi.GetSize(); i++)
        {
            _DGN_RBAR_CRC Rbar = arRbarPosi[i];
            double dAsi = Rbar.dArea;
            dRebarF += dAsi*dfsd/1.15;
        }

        double dMp = dZp*m_dfy/1.15;
        double dFs = dRebarF;
        double dYbar = 1.1*dFs / (2.0*m_dfy*dTw);
        dPlasticNAIRC = dYbar;



    }
    // 5. Calc. PNA EC
    if(bPositive)	// Positive Moment.
    {
        if(dConcF > dGirderF) // PNA in Concrete Slab
        {
            double dDeltaF = dConcF - dGirderF;
            double dDeltaNA = dBc*m_dAlpha*dfcd < m_dZero ? 0.0 : dDeltaF/(dBc*dfcd);      
            dPlasticNA = dH + dHh + dDeltaNA;
            dbc = 0.0;
            dAlpha = dh < m_dZero ? 0.0 : dbc/dh;

        }
        else if(dConcF < dGirderF)
        {
            if(!bNeglectWeb)
            {
                if((dConcF+dStopfF) > (dGirderF-dStopfF)) // PNA in  Top flange
                {
                    double dComp = dConcF+dStopfF;
                    double dTens = dGirderF-dStopfF;        
                    double dDeltaNA = dB1*dfyd_t < m_dZero ? 0.0 : (dComp-dTens)/(2.0*dB1*dfyd_t); // Comp-x*dB1*dfyd_t = Tens+x*dB1*dfyd_t
                    dPlasticNA = dH - dTf1 + dDeltaNA;
                }
                else if((dConcF+dStopfF+dSwebF)>dSbotfF) // PNA in  Web
                {
                    double dComp = dConcF+dStopfF+dSwebF;
                    double dTens = dSbotfF;        
                    double dDeltaNA = dTw_t*dfyd_w < m_dZero ? 0.0 : (dComp-dTens)/(2.0*dTw_t*dfyd_w); // Comp-x*dTw_t*dfyd_w = Tens+x*dTw_t*dfyd_w
                    dPlasticNA = dTf2 + dDeltaNA;
                }
                else if((dConcF+dStopfF+dSwebF)<dSbotfF) // PNA in  Bottom flange
                {
                    double dComp = dConcF+dStopfF+dSwebF;
                    double dTens = dSbotfF;        
                    double dDeltaNA = dB2*dfyd_b < m_dZero ? 0.0 : (dTens-dComp)/(2.0*dB2*dfyd_b); // Comp+x*dB2*dfyd_b = Tens-x*dB2*dfyd_b
                    dPlasticNA = dTf2 - dDeltaNA;
                }
                else ASSERT(0);
            }
            else
            {
                if((dConcF+dStopfF)>(dGirderF-dStopfF)) // PNA in Top flange
                {
                    double dComp = dConcF+dStopfF;
                    double dTens = dGirderF-dStopfF;        
                    double dDeltaNA = dB1*dfyd_t < m_dZero ? 0.0 : (dComp-dTens)/(2.0*dB1*dfyd_t); // Comp-x*dB1*dfyd_t = Tens+x*dB1*dfyd_t
                    dPlasticNA = dH - dTf1 + dDeltaNA;
                }
                else if((dConcF+dStopfF+dSwebF)<dSbotfF)// PNA in Bottom flange
                {
                    double dComp = dConcF+dStopfF+dSwebF;
                    double dTens = dSbotfF;        
                    double dDeltaNA = dB2*dfyd_b < m_dZero ? 0.0 : (dTens-dComp)/(2.0*dB2*dfyd_b); // Comp+x*dB2*dfyd_b = Tens-x*dB2*dfyd_b
                    dPlasticNA = dTf2 - dDeltaNA;
                }
            }
        }
        //dPlasticNA = min(dPlasticNA,dPlasticNAIRC);
    }
    else // Negative Moment
    {
        double dRebarF	= 0.0;
        // Rebar Info.
        for(int i=0; i < arRbarPosi.GetSize(); i++)
        {
            _DGN_RBAR_CRC Rbar = arRbarPosi[i];
            double dAsi = Rbar.dArea;
            dRebarF += dAsi*dfsd;
        }

        if(dGirderF>dRebarF)
        {
            if(!bNeglectWeb)
            {
                if((dRebarF+dStopfF)>(dGirderF-dStopfF)) // PNA in  Top flange
                {
                    double dTens = dRebarF+dStopfF;
                    double dComp = dGirderF-dStopfF;        
                    double dDeltaNA = dB1*dfyd_t < m_dZero ? 0.0 : (dTens-dComp)/(2.0*dB1*dfyd_t); // Comp-x*dB1 = Tens+x*dB1
                    dPlasticNA = dH - dTf1 + dDeltaNA;
                }
                else if((dRebarF+dStopfF+dSwebF)>dSbotfF) // PNA in  Web
                {
                    double dTens = dRebarF+dStopfF+dSwebF;
                    double dComp = dSbotfF;        
                    double dDeltaNA = dTw_t*dfyd_w < m_dZero ? 0.0 : (dRebarF)/(2.0*dTw_t*dfyd_w); // Comp-x*dTw_t = Tens+x*dTw_t
                    dPlasticNA = dDeltaNA;//dTf2 + dDeltaNA;
                }
                else if((dRebarF+dStopfF+dSwebF)<dSbotfF) // PNA in  Bottom flange
                {
                    double dTens = dRebarF+dStopfF+dSwebF;
                    double dComp = dSbotfF;
                    double dDeltaNA = dB2*dfyd_b < m_dZero ? 0.0 : (dComp-dTens)/(2.0*dB2*dfyd_b); // Comp+x*dB2 = Tens-x*dB2
                    dPlasticNA = dTf2 - dDeltaNA;
                }
                else ASSERT(0);
            }
            else
            {
                if((dRebarF+dStopfF)>(dGirderF-dStopfF)) // PNA in Top flange
                {
                    double dTens = dRebarF+dStopfF;
                    double dComp = dGirderF-dStopfF;        
                    double dDeltaNA = dB1*dfyd_t < m_dZero ? 0.0 : (dTens-dComp)/(2.0*dB1*dfyd_t); // Comp-x*dB1 = Tens+x*dB1
                    dPlasticNA = dH - dTf1 + dDeltaNA;
                }
                else if((dRebarF+dStopfF+dSwebF)<dSbotfF) // PNA in Bottom flange
                {
                    double dTens = dRebarF+dStopfF+dSwebF;
                    double dComp = dSbotfF;
                    double dDeltaNA = dB2*dfyd_b < m_dZero ? 0.0 : (dComp-dTens)/(2.0*dB2*dfyd_b); // Comp+x*dB2 = Tens-x*dB2
                    dPlasticNA = dTf2 - dDeltaNA;
                }
            }
        }
        else
        {
            // Iteration으로 찾아야함. 하지만 수렴 안하는 경우도 있을것임.
            // 그럴때는 rebar끝과 Top flange 중간으로 함.      
            int iCount=0;      
            double dCompF = dGirderF;
            double dTensF = dRebarF;
            double dDeltaF = 0.0;
            double dNeuC1 = dH;
            double dNeuC2 = dHt;
            double dTol=1.0; // 1%
            double dToler=0.0;
            double dNeuC = (dHt-0.5*dtc);
            int iIter = 20;
            do
            {
                double dGap = (dHt-0.5*dtc) - dNeuC;
                double dNAdsi=0.0;
                double dCompfsdAs=0.0;
                double dTensfsdAs=0.0;

                for(int i=0; i<arRbarPosi.GetSize(); i++)
                {
                    _DGN_RBAR_CRC Rbar;
                    Rbar = arRbarPosi[i];
                    // PlasticNA에서 떨어진 거리.        
                    double dNAdsi = dGap + Rbar.dz;
                    double dAsi = Rbar.dArea;
                    if(dNAdsi<0.0) // 압축.
                    {
                        dCompfsdAs += dAsi*dfsd;
                    }
                    else
                    {
                        dTensfsdAs += dAsi*dfsd;
                    }
                }

                dCompF = dGirderF + dCompfsdAs;
                dTensF = dTensfsdAs;
                dDeltaF = dCompF-dTensF;  
                dToler = (fabs(dCompF) < m_dZero ? 0.0 : dTensF/dCompF*100.0);        

                if(fabs(dToler) < dTol) break;
                if(iCount > iIter)break;

                if(dDeltaF<=0.0) {dNeuC2 = dNeuC;}
                else             {dNeuC1 = dNeuC;}
                dPlasticNA = 0.5*(dNeuC1 + dNeuC2);        
                dNeuC = dPlasticNA;

                ++iCount;
            }while(fabs(dToler)>dTol);

        }
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_PlasticNA4GenSect(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, T_SECT_SECTBASE_D& SectBaseD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, 
                                                const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, double& dPlasticNA)
{
    int i=0;
    // Material
    double dgam_M0  = m_dGamma_M0;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;

    double dfy  = MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfck = MatlElem.dfck;
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dfcd  = 0.67* dfck/dgam_c;
    double dfsd = dgam_sr==0.0 ? dfsk : dfsk/dgam_sr;
    double dfyd = dgam_M0==0.0 ? dfy : dfy/dgam_M0;  

    double dCpz=0.0; // 철근정보는 어떻게 해야하나??

    double dHt = Get_Height4Gen(bPosiI);

    double dCcDel=0.0;
    double dTOL2 = 0.1;
    double dToler = 0.0;

    double dNeuC = 0.5*dHt;
    // 중립축 결정을 위해서. 
    BOOL bPosiForce=TRUE;
    double dNeuC1 = dHt;
    double dNeuC2 = 0.0;

    double dCompForce=0., dTensForce=0.0, dForce_ratio=0.0;
    double dPe_Sum=0., dMe_Sum=0., dep_Sum=0., dAps_Sum=0.;

    double dAs_top=0.0, dAs_bot=0.0; // Slab
    double dAr_top=0.0, dAr_bot=0.0; // Rebar
    double dAg_top=0.0, dAg_bot=0.0; // Girder
    double aArea[4];
    double dFcc=0.0, dFtc=0.0; // Fcc : Force compression concrete, Ftc : Force tension concrete
    double dFcg=0.0, dFtg=0.0;
    double dFcg_cl=0.0, dFtg_cl=0.0; // cut line
    double dFcr=0.0, dFtr=0.0;

    // Convert
    T_SECT_SECTBASE_D CalcSectBaseD; CalcSectBaseD.Initialize();
    Get_ConvertSectBase4GenLineData(SectBaseD, GLineDgn, CalcSectBaseD);

    int iCount=0;
    BOOL bYaxis=TRUE;
    T_GSEC_POLYGON PolyD;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aTopLine, aBotLine, aHCutLine;
    CArray<double,double&> aHCutTopThick;
    double aZcen[4];
    do 
    {
        dAr_top = dAr_bot = 0.0;
        dFcg_cl = dFtg_cl = 0.0;
        PolyD.Initialize();
        aTopLine.RemoveAll();
        aBotLine.RemoveAll();
        aHCutLine.RemoveAll();
        aHCutTopThick.RemoveAll();

        double dcutZpos=bPositive ? dHt-dNeuC : dNeuC;
        CSectUtil::Get_SectPropertyOfCuttingArea(dcutZpos, CalcSectBaseD, aArea, aZcen, PolyD, aTopLine, aBotLine, aHCutLine, aHCutTopThick);
        dAs_top = aArea[0];
        dAs_bot = aArea[1];
        dAg_top = aArea[2];
        dAg_bot = aArea[3];    

        // arTranRbarPosi의 정보는 도심중심인지 알아야함. 
        int iRbarSize = arRbarPosi.GetSize();
        for(i=0; i<iRbarSize; i++)
        {
            _DGN_RBAR_CRC RbarD = arRbarPosi[i];
            if(RbarD.dz-dcutZpos>=0.0) dAr_top += RbarD.dArea;
            else                       dAr_bot += RbarD.dArea;
        }    

        if(bPositive)
        {
            // 정모멘트일 경우 철근은 무시. 
            dFcc = m_dAlpha * dfcd * dAs_top;
            dFtc = 0.0;
            Get_ForceLineGS(dfy, dfy1, PolyD, aTopLine, dFcg); // dFcg = dfyd * dAg_top;
            Get_ForceLineGS(dfy, dfy1, PolyD, aBotLine, dFtg); // dFtg = dfyd * dAg_bot;
            Get_TopBotForceLineGS(dfy, dfy1, PolyD, aHCutLine, aHCutTopThick, dFcg_cl, dFtg_cl);
            dFtr = dfsd * dAr_bot;
        }
        else
        {
            dFcc = m_dAlpha * dfcd * dAs_bot;
            dFtc = 0.0;
            Get_ForceLineGS(dfy, dfy1, PolyD, aBotLine, dFcg); // dFcg = dfyd * dAg_bot;
            Get_ForceLineGS(dfy, dfy1, PolyD, aTopLine, dFtg); // dFtg = dfyd * dAg_top; 
            Get_TopBotForceLineGS(dfy, dfy1, PolyD, aHCutLine, aHCutTopThick, dFtg_cl, dFcg_cl);
            dFtr = dAr_top * dfsd;
            dFcr = 0.0;//dAr_bot * dfsd;
        }
        dCompForce = dFcc + dFcg + dFcg_cl;
        dTensForce = dFtg + dFtr + dFtg_cl;
        dForce_ratio = dTensForce==0.0 ? fabs(dCompForce/m_dZero) : fabs(dCompForce/dTensForce);
        dCcDel = dTensForce - dCompForce;
        dToler = dForce_ratio-1.0; //(dCompForce==0.0 ? 0.0 : dCcDel/dCompForce);

        //---------------------------------------------------------------
        //PSC_FRCR_NEUT_AXIS_D NeutAxisD;
        //NeutAxisD.dx = dc_neu;
        //NeutAxisD.dCompF_c = dCc;
        //NeutAxisD.dCompF_s = dCsc;
        //NeutAxisD.dTensF_p = dTp;
        //NeutAxisD.dTensF_s = dTst;
        //NeutAxisD.dRatio   = dForce_ratio;
        //arNeutAxisInfo.Add(NeutAxisD);
        //---------------------------------------------------------------

        if(fabs(dCcDel)<dTOL2) break;
        if(iCount>m_Iteration-1) break;

        if(dCcDel>=0.0) dNeuC2 = dNeuC; // 인장력이 클경우
        else            dNeuC1 = dNeuC; // 압축력이 클경우

        dNeuC = 0.5*(dNeuC1 + dNeuC2);  //Get neutral axis position for next iteration    

        ++iCount;

    } while(fabs(dCcDel)>dTOL2);


    dPlasticNA = bPositive ? dHt - dNeuC : dNeuC; // dNeuC는 압축영역이다.


    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_PlasticNAEff(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, const T_SECT_D& crSectD, 
                                           CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, double dPlasticNAxis, 
                                           double& dEffPlasticNA, double& dEffeLen, double& dHoleLen)
{
    // 1. Initialize
    dEffPlasticNA = dPlasticNAxis;
    dHoleLen = 0.0;   dEffeLen = 0.0;

    // 2. Section
    double dH	= m_pDgnSectUtil->Get_Size_H(&crSectD, bPosiI);  
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&crSectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t= m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&crSectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);
    double dhw	= m_pDgnSectUtil->Get_Size_hw(&crSectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&crSectD); 
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&crSectD); 
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&crSectD); 
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&crSectD, bPosiI);  

    // 3. Material
    double dgam_M0	= m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c	= m_dGamma_C;
    double dfy		= MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfck		= MatlElem.dfck;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;
    double dfcd		=  0.67* dfck/dgam_c;
    double dfsd		= fabs(dgam_sr) < m_dZero  ? dfsk : dfsk/dgam_sr;
    double dfyt = MatlElem.MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatlElem.MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatlElem.MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;;
    double dfyd_t	= fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b	= fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w	= fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;
    double dRebarF	= 0.0;

    BOOL bPlasticClass = TRUE;    
    double dfy_r	= (dTw < 40.0 + m_dZero)? dfy : dfy1;
    double dEpsi	= Get_Epsilon(dfy_r);

    // 4. Calc. Each Part's Force
    // EN 1993-1-1, 6.2.2.4 Effective properties of cross section with class 3 webs and class 1 or 2 flanges 
    //IRC 22 2008 603.1.3 (3)
    double dConcF		= dBc*dtc*m_dAlpha*dfcd;
    double dStopfF		= dB1*dTf1*dfyd_t;
    double dSbotfF		= dB2*dTf2*dfyd_b;
    double dSwebF		= dhw*dTw_t*dfyd_w;
    dEffeLen			= 20.0*dEpsi*dTw; // Figure 6.3:Effective class 2 web for compression flange
    double dEffeLen2	= 2.0*dEffeLen;

    if(dEffeLen2 > dhw) {return FALSE;}

    double dBwc = 0.0; // Web's height under compression for N.A. 

    if(bPositive)
    {
        if(dH-dTf1 > dPlasticNAxis) // dPlasticNAxis>dTf2
        {
            dBwc = dH-dTf1 - dPlasticNAxis;
        }
        else {return FALSE;}
    }
    else
    {
        if(dTf2 < dPlasticNAxis) // dPlasticNAxis<dH-dTf1
        {
            dBwc = dPlasticNAxis - dTf2;
        }    
        else {return FALSE;}
    }

    double dWebEffeArea		= dEffeLen2 * dTw_t;  
    double dWebEffFc		= dWebEffeArea * dfyd_w;
    double dGirderF			= dStopfF + dSbotfF + dSwebF;  
    double dGirderFeff		= dStopfF + dSbotfF + dWebEffeArea;  
    //
    double dN_top = 0.0, dN_bot = 0.0;
    double dN_bf_ew = 0.0; // bottom flange + web effective force
    double dN_tf_ew = 0.0; // top flange + web effective force
    double dEffeLendHoleLen = 0.0;

    // 5. Calc. Eff.PNA
    if(bPositive)
    {
        // 중립축 위부터 결정한다. 
        dN_top   = dConcF + dStopfF + dWebEffFc;
        dN_bf_ew = dSbotfF;
        if(dN_top < dN_bf_ew) {return FALSE;}

        double dN_top_minus_bf_ew	= dN_top - dN_bf_ew;
        double dWebLenP				= dfyd_w*dTw_t < m_dZero ?  0.0 : dN_top_minus_bf_ew/(dfyd_w*dTw_t);
        double dWebEffBotLen		= dWebLenP + dEffeLen;
        if(dWebEffBotLen > dhw-dEffeLen) {return FALSE;} // 계산한 길이가 Top의 web와 겹치면 안된다.

        dEffPlasticNA	= dTf2 + dWebLenP;    
        dHoleLen		= dhw - dWebEffBotLen - dEffeLen;
    }
    else
    {
        for(int i=0; i < arRbarPosi.GetSize(); i++)
        {
            const _DGN_RBAR_CRC& Rbar = arRbarPosi[i];
            double dAsi = Rbar.dArea;
            dRebarF += dAsi*dfsd;
        }

        dN_bot   = dSbotfF + dWebEffFc;
        dN_tf_ew = dRebarF + dStopfF;
        if(dN_bot < dN_tf_ew) {return FALSE;}

        double dN_bot_minus_tf_ew	= dN_bot - dN_tf_ew;
        double dWebLenP				= dfyd_w*dTw_t < m_dZero ?  0.0 : dN_bot_minus_tf_ew/(dfyd_w*dTw_t);
        double dWebEffTopLen		= dWebLenP + dEffeLen;
        if(dWebEffTopLen > dhw-dEffeLen) {return FALSE;}

        dEffPlasticNA	= dH - dTf1 - dWebLenP;    
        dHoleLen		= dhw - dWebEffTopLen - dEffeLen;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveSectionProperty(BOOL bPosiI, double dMy, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_CLAS_BASE& ClassD, 
                                                       T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect)
{
    EffeStlSect.Initialize();
    EffeComSect.Initialize();

    int i=0;
    double dMc_Ed = dMy;

    // Material
    double dgam_M0 =m_dGamma_M0;
    double dgam_sr=m_dGamma_S;
    double dgam_c =m_dGamma_C;

    double dfy  = MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfck = MatlElem.dfck;
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dfcd =  0.67* dfck/dgam_c;
    double dfsd = dgam_sr==0.0 ? 0.0 : dfsk/dgam_sr;

    // Section.  
    T_SECT_D SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_SECTBASE_D SectBase = bTapered && !bPosiI ? SectD.CmpTapJ : SectD.SectBefore.SectI;

    //T_SECT_STIFFNESS StlSect; 
    //StlSect.Initialize();
    T_SECT_STIFFNESS NonConSect;    NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;     RebarSect.Initialize();
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    //
    T_SECT_STIFFNESS GirderStiff;
    if(!bTapered)
    {
        GirderStiff = SectD.SectBefore.Stiffness;
    }
    else
    {
        GirderStiff = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;  // girder
    }

    //
    T_SECT_STIFFNESS SectTemp;
    //SectTemp = dMc_Ed>=0.0 ? RebarSect : NonConSect;

    // Dimension	
    double dH   = SectBase.Size[0]+SectBase.Size[3]+SectBase.Size[5];
    double dTw  = SectBase.Size[1];
    double dB1  = SectBase.Size[2];
    double dTF1 = SectBase.Size[3];
    double dB2  = SectBase.Size[4];
    double dTF2 = SectBase.Size[5];	
    double dr1  = SectBase.Size[6];
    double dr2  = SectBase.Size[7];
    if(dB2==0.)	dB2  = dB1;
    if(dTF2==0.)	dTF2 = dTF1;
    double dh = SectBase.Size[0]; // C, web의 높이.

    double dScc = SectD.SectAfter.SectI.Size[1];  // C.T.C.
    double dEr  = SectD.SectBefore.Matl_Elast;    // Es/Ec
    double dDr  = SectD.SectBefore.Matl_Density;  // Ds/Dc
    double dBc  = SectD.SectAfter.SectJ.Size[0];
    double dtc  = SectD.SectAfter.SectJ.Size[1];
    double dHh  = SectD.SectAfter.SectJ.Size[2];
    //
    double dHt = dH+dHh+dtc; // 전체높이.  

    double dbc=0.0;
    //double dc=0.0;
    //double dAlpha=0.0;
    //double dPsi=0.0;
    //double dk_sig=0.0;

    // Parameter  
    int iClassSect = ClassD.iClassSect;
    int iWebClass  = ClassD.iWebClass;
    int iWebDeter  = ClassD.iWebDeter;
    double dPsi  = ClassD.dPsi[4];
    double dbeff = ClassD.dbeff[4];
    double dbe1  = ClassD.dbe1[4];
    double dbe2  = ClassD.dbe2[4];
    //double dbc   = ClassD.d

    double dSubWidth = 0.0;
    double dSubZbar = 0.0;
    double dSubArea = 0.0;
    double dSubIyy  = 0.0;
    //
    double dEffArea=0.0;
    double dEffCzp=0.0;
    double dEffCzm=0.0;
    double dEffIyy=0.0;  

    //!/ moment와 iWebDeter가 다를 경우 처리 해야함. 
    // Modify, 작용한 Moment만으로는 ClassD.iWebDeter을 if문으로 판단하는 것은 부적절 하여 막음.
    /*
    if(dMc_Ed>=0.0)
    {
    if(ClassD.iWebDeter==2)
    ASSERT(0);
    }
    else
    {
    if(ClassD.iWebDeter==1)
    ASSERT(0);
    }
    */

    // Gross Section
    if(iClassSect==1 || iClassSect==2 || iClassSect==3)
    {
        // 
        EffeStlSect = GirderStiff;
        EffeComSect = dMc_Ed>=0.0 ? RebarSect : NonConSect;
    }
    else if(iClassSect==4)
    {
        for(i=0; i<2; i++)
        {
            if(i==0)
            {
                SectTemp = GirderStiff;
            }
            else
            {
                SectTemp = dMc_Ed>=0.0 ? RebarSect : NonConSect;
            }
            //
            if(1.0-m_dZero<dPsi && dPsi<1.0+m_dZero)
            {
                dSubWidth = dh-dbe1-dbe2;
                dSubZbar = dTF2+dbe2+0.5*dSubWidth;
            }
            else if(0.0<=dPsi && dPsi<1.0)
            {
                if(iWebDeter==1) // Sigma1 = Top.
                {
                    dSubWidth = dh-dbe1-dbe2;
                    dSubZbar = dTF2+dbe2+0.5*dSubWidth;
                }
                else if(iWebDeter==2) // Sigma2 = Bot.
                {
                    dSubWidth = dh-dbe1-dbe2;
                    dSubZbar = dTF2+dbe1+0.5*dSubWidth;
                }
                else
                {
                    ASSERT(0);
                }
            }
            else if(dPsi<0.0)
            {      
                if(iWebDeter==1) // 정모멘트와 같은 의미.
                {
                    dbc = dH-SectTemp.Czm-dTF1;
                    if(dbc<0.0) 
                        ASSERT(0);
                    dSubWidth = dbc-dbe1-dbe2;
                    dSubZbar = SectTemp.Czm+dbe2+0.5*dSubWidth;
                }
                else if(iWebDeter==2) // 부모멘트와 같은 의미.
                {
                    dbc = NonConSect.Czm-dTF2;
                    if(dbc<0.0) 
                        ASSERT(0);
                    dSubWidth = dbc-dbe1-dbe2;
                    dSubZbar = dTF2+dbe1+0.5*dSubWidth;
                }
                else
                {
                    ASSERT(0);
                }
            }
            //
            dSubArea = dSubWidth*dTw;
            dSubIyy  = dTw*pow(dSubWidth,3)/12.0;      

            if(dSubWidth<0.0)
                ASSERT(0);

            // 정모멘트 부모멘트에 따라 Effective Area가 다르다. 둘다 고려해야 한다. 
            dEffArea = SectTemp.Area - dSubArea;
            dEffCzm  = (SectTemp.Area*SectTemp.Czm-dSubArea*dSubZbar)/dEffArea;
            dEffCzp  = SectTemp.Czp; 
            dEffIyy  = SectTemp.Ryy+SectTemp.Area*pow((SectTemp.Czm-dEffCzm),2)
                - (dSubIyy+dSubArea*pow((dSubZbar-dEffCzm),2));  
            //
            if(i==0)
            {
                EffeStlSect = SectTemp;      
                EffeStlSect.Czm  = dEffCzm;
                EffeStlSect.Czp  = dEffCzp;
                EffeStlSect.Area = dEffArea;
                EffeStlSect.Ryy  = dEffIyy;  
                EffeStlSect.dy1  = SectTemp.dy1+(SectTemp.Czm-dEffCzm);
                EffeStlSect.dy2  = SectTemp.dy2+(SectTemp.Czm-dEffCzm);
                EffeStlSect.dy3  = SectTemp.dy3+(SectTemp.Czm-dEffCzm);
                EffeStlSect.dy4  = SectTemp.dy4+(SectTemp.Czm-dEffCzm);
            }
            else
            {
                EffeComSect = SectTemp;      
                EffeComSect.Czm  = dEffCzm;
                EffeComSect.Czp  = dEffCzp;
                EffeComSect.Area = dEffArea;
                EffeComSect.Ryy  = dEffIyy;  
                EffeComSect.dy1  = SectTemp.dy1+(SectTemp.Czm-dEffCzm);
                EffeComSect.dy2  = SectTemp.dy2+(SectTemp.Czm-dEffCzm);
                EffeComSect.dy3  = SectTemp.dy3+(SectTemp.Czm-dEffCzm);
                EffeComSect.dy4  = SectTemp.dy4+(SectTemp.Czm-dEffCzm);
            }
        }
    }
    else ASSERT(0);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveSectionProperty4DB(BOOL bPosiI, double* aForce, double da, CPG_MATL_ELEM& MatlElem, 
                                                          CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, 
                                                          CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, T_CLAS_BASE& ClassD, 
                                                          T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect,  
                                                          T_DGNLINE_LBD_BR& RhocBefore, T_DGNLINE_LBD_BR& RhocAfter, 
                                                          BOOL bCompoStiffData/*=FALSE*/)
{
    EffeStlSect.Initialize();
    EffeComSect.Initialize();

    // 1. Material
    double dgam_M0	= m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c		= m_dGamma_C;
    double dfy1		= MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy2		= MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfck		= MatlElem.dfck;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;
    double dfcd		=  0.67* dfck/dgam_c;
    double dfsd		= fabs(dgam_sr) < m_dZero ? 0.0 : dfsk/dgam_sr;
    double dFx		= aForce[0];
    double dMy		= aForce[4];
    double dMz		= aForce[5];
    double dMc_Ed		= dMy;
    BOOL bPositive = dMy > -m_dZero ? TRUE : FALSE;

    // 2. Section.  
    T_SECT_D& SectD = SectElem.SectD;
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();  
    T_SECT_STIFFNESS GirderStiff; GirderStiff.Initialize();

    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

    T_DGNLINE_LBD_BR RhoC_Before; 
    T_SECT_STIFFNESS GirderEffeStiffD;
    Get_EffeSectPropBeforeDB(bPosiI, dfy1, dfy2, aForce, SectD, da, GirderStiff, arLstiPosi, GirderEffeStiffD, RhoC_Before);

    T_SECT_STIFFNESS CompoStiff;  CompoStiff.Initialize();
    if(bPositive) {CompoStiff = RebarSect; }
    else          {CompoStiff = NonConSect;}

    T_DGNLINE_LBD_BR RhoC_After; 
    T_SECT_STIFFNESS CompoEffeStiffD;
    Get_EffeSectPropAfterDB(bPosiI, dfy1, dfy2, aForce, da, MatlElem, SectElem, arRbarPosi, arLstiPosi, CompoEffeStiffD, 
        RhoC_After, bCompoStiffData);

    if(ClassD.iClassSect==4)
    {
        EffeStlSect = GirderEffeStiffD;
        EffeComSect = CompoEffeStiffD;
    }
    else
    {
        EffeStlSect = GirderStiff;
        EffeComSect = CompoStiff;
    }

    RhocBefore = RhoC_Before;
    RhocAfter  = RhoC_After;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffeSectPropBeforeDB(bool bPosiI, double dfy1, double dfy2, double* aForce, T_SECT_D& SectD, double da, 
                                                   T_SECT_STIFFNESS& GirderStiff, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, 
                                                   T_SECT_STIFFNESS& EffeStiff, T_DGNLINE_LBD_BR& RhoC)
{
    double dFx = aForce[0];
    double dMy = aForce[4];
    double dMz = aForce[5];

    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dtw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dtf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dtf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dfyt	= Get_fy4Thick(dfy1, dfy2, dtf1);
    double dfyb	= Get_fy4Thick(dfy1, dfy2, dtf2);
    double dfyw	= Get_fy4Thick(dfy1, dfy2, dtw);
    double dAg	= GirderStiff.Area;
    double dIy	= GirderStiff.Ryy;
    double dIz	= GirderStiff.Rzz;
    double dCym	= GirderStiff.Cym;
    double dCzm	= GirderStiff.Czm;

    int iStiffSize = arLstiPosi.GetSize();
    CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&> aLineDgnBase;
    Get_LineInfo4SectDB(bPosiI, SectD, GirderStiff, arLstiPosi, aLineDgnBase);

    double dPoisson = m_dPoisson;
    double dEs = m_dEs;
    T_DGNLINE_LBD LineLBD;
    Get_LineDgnInfo4SectDB(bPosiI, dfy1, dfy2, dEs, dPoisson, aForce, SectD, da, GirderStiff, arLstiPosi, aLineDgnBase, LineLBD); 
    RhoC.iClass = LineLBD.iClass;
    RhoC.dxi    = LineLBD.dxi;
    RhoC.dRho_c = LineLBD.dRho_c; 

    double dArea_eff=0.0, dCzm_eff=0.0, dIy_eff=0.0;
    BOOL bPositive = (dMy > -m_dZero) ? TRUE : FALSE; 
    Get_EffectiveCzmData4Sect(bPosiI, bPositive, SectD, arLstiPosi, LineLBD, dArea_eff, dCzm_eff, dIy_eff);

    double dheight = GirderStiff.Czm + GirderStiff.Czp;
    double decc_y = 0.0;
    double decc_z = GirderStiff.Czm - dCzm_eff;  
    EffeStiff		= GirderStiff;
    EffeStiff.Area = dArea_eff;
    EffeStiff.Czm  = dCzm_eff;
    EffeStiff.Czp  = dheight - dCzm_eff;
    EffeStiff.Ryy  = dIy_eff;
    EffeStiff.dy1 += decc_z;
    EffeStiff.dy2 += decc_z;
    EffeStiff.dy3 += decc_z;
    EffeStiff.dy4 += decc_z;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffeSectPropAfterDB(BOOL bPosiI, double dfy1, double dfy2, double* aForce, double da, 
                                                  CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, 
                                                  CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, T_SECT_STIFFNESS& EffeStiff, 
                                                  T_DGNLINE_LBD_BR& RhoC, BOOL bCompoStiffData/*=FALSE*/)
{
    // 1. Material
    double dgam_M0	= m_dGamma_M0;
    double dgam_sr	= m_dGamma_S;
    double dgam_c		= m_dGamma_C;
    double dEgir		= MatlElem.MatdD.Data1.Analysis.Elast;
    double dEslab		= MatlElem.MatdD.Data2.Analysis.Elast;
    double dPg		= MatlElem.MatdD.Data1.Analysis.Poisson;
    double dPS		= MatlElem.MatdD.Data2.Analysis.Poisson;

    // 2. Section.  
    T_SECT_D& SectD = SectElem.SectD; 
    T_SECT_STIFFNESS NonConSect;   NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;  RebarSect.Initialize(); 

    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dhw	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dtw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);
    double dBs	= bPosiI ? SectD.SectAfter.SectI.Size[0] : SectD.CmpTapJ.Size[59];  // Slab Width
    int    iNg	= SectD.SectAfter.SectI.BuiltUpFlag;  // Number of Main Girder

    // 3. Force
    double dFx = aForce[0], dMy = aForce[4], dMz = aForce[5];
    BOOL bPositive = dMy > -m_dZero ? TRUE : FALSE;

    T_SECT_STIFFNESS GirderStiff;
    Get_GirderStiffnessofCompo(bPosiI, SectD, GirderStiff);

    T_SECT_STIFFNESS CompoStiff;  CompoStiff.Initialize();
    if(bPositive) {CompoStiff = RebarSect; }
    else          {CompoStiff = NonConSect;}

    T_SECT_STIFFNESS CompoGiderEffeStiffD;
    Get_EffeSectPropBeforeDB(bPosiI, dfy1, dfy2, aForce, SectD, da, CompoStiff, arLstiPosi, CompoGiderEffeStiffD, RhoC);

    // Slab, Rebar 적용..
    BOOL bScaleFactor=FALSE;
    T_SECT_INFO_D SectInfoD;

    // Section.
    double dScaleFactor[7] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; 

    // Girder
    T_SECT_PART_D GirderPartD;
    GirderPartD.Stiff.Stiffness = CompoGiderEffeStiffD;
    GirderPartD.Stiff.dYbar = CompoGiderEffeStiffD.Cym;
    GirderPartD.Stiff.dZbar = CompoGiderEffeStiffD.Czm;
    GirderPartD.dElast = dEgir;
    GirderPartD.dPoisson = dPg;
    SectInfoD.aSectPart.Add(GirderPartD);

    CString strCodeNa=_T(""), strRbarNa=_T("");
    CDBLib::GetDefaultConRbar(strCodeNa, strRbarNa);
    T_MATL_REBAR MatlRbarD; MatlRbarD.Initialize();
    m_pDoc->m_pMatlDB->GetRebarData(strCodeNa, strRbarNa, MatlRbarD);
    double dEr = MatlRbarD.B_Elast;
    double dDr = MatlRbarD.B_Density;

    // Slab  
    T_SECT_INFO_D SlabSectInfoD;
    T_SECT_PROP_D SlabSectPropD;  
    T_SECT_PART_D SlabPartD;
    T_SECT_STIFFNESS SlabOrgStiffD; SlabOrgStiffD.Initialize();
    T_SECT_STIFFNESS SlabRbarStiffD; SlabRbarStiffD.Initialize();
    Get_StiffnessOfSB(dBs, dBc, dtc, iNg, SlabOrgStiffD);

    double dZslab_sta		= dH+dHh;
    double dZcent_slb		= dHt-0.5*dtc;
    SlabPartD.dElast		= dEr;
    SlabPartD.dPoisson	= 0.3;
    SlabPartD.Stiff.Stiffness = SlabOrgStiffD;
    SlabPartD.Stiff.dYbar = SlabOrgStiffD.Cym;
    SlabPartD.Stiff.dZbar = dZcent_slb;

    int iRbarSize = arRbarPosi.GetSize();

    if(iRbarSize > 0)
    {
        CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arTempRbarPosi;    
        SlabPartD.aRbarData.SetSize(iRbarSize);
        arTempRbarPosi.Copy(arRbarPosi);
        //Get_ConvertRbarPoint4Org(RebarSect.Cym, dZslab_sta, arTempRbarPosi); // 원점 좌표로 수정.  
        Get_ConvertRbarPoint4Org(0.0, 0.5*dtc, arTempRbarPosi);                // 슬래브 바닥 중심으로 수정.  
        //Get_ConvertRbarPoint4Org(RebarSect.Cym, dZcent_slb, arTempRbarPosi); // 슬래브 도심 기준에서 원점 좌표로 수정. 

        for(int i=0; i < iRbarSize; i++)
        {
            _DGN_RBAR_CRC& RbarCrc = arTempRbarPosi[i];
            T_GSEC_RBAR GsecRbar;
            GsecRbar.dEs = dEr; 
            GsecRbar.dDs = dDr;
            Get_ConvertRbarCrcToGsecRbar(RbarCrc, GsecRbar);
            SlabPartD.aRbarData.SetAt(i, GsecRbar);
        }
    }

    if(bPositive) // Slab + Rebar
    {
        SlabSectInfoD.aSectPart.Add(SlabPartD);
        SlabSectInfoD.StiffMainD.Stiffness = SlabOrgStiffD;
        SlabSectInfoD.StiffMainD.dYbar = SlabOrgStiffD.Cym;
        SlabSectInfoD.StiffMainD.dZbar = dZcent_slb;    
        CSectUtil::Get_GeneralSectProperty(bScaleFactor, SlabSectInfoD, dScaleFactor, SlabSectPropD);
        SlabRbarStiffD = SlabSectPropD.aSectProp[0].StiffRbar.Stiffness;
        SlabPartD.Stiff.Stiffness = SlabRbarStiffD;
        SlabPartD.Stiff.dYbar = SlabSectPropD.aSectProp[0].StiffRbar.dYbar;
        SlabPartD.Stiff.dZbar = SlabSectPropD.aSectProp[0].StiffRbar.dZbar;
    }
    else  // Rebar
    {    
        T_SECT_STIFFNESS RbarStiffD;
        CSectUtil::Get_Stiffness4Rebar(SlabPartD.aRbarData, RbarStiffD);
        SlabPartD.Stiff.Stiffness = RbarStiffD;
        SlabPartD.Stiff.dYbar = RbarStiffD.Cym;
        SlabPartD.Stiff.dZbar = RbarStiffD.Czm + dH + dHh;
        SlabPartD.Stiff.Stiffness.Czm = RbarStiffD.Czm + dH + dHh;
    }
    SectInfoD.aSectPart.Add(SlabPartD);

    T_SECT_PROP_D SectPropD;
    CSectUtil::Get_GeneralSectProperty(bScaleFactor, SectInfoD, dScaleFactor, SectPropD);

    if(!bCompoStiffData) 
    {
        EffeStiff = SectPropD.aSectProp[0].StiffPure.Stiffness;
    }
    else 
    {		
        Get_StiffnessOfEffeSectCompo(SectInfoD, EffeStiff); // 강성 재계산
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_StiffnessOfEffeSectCompo(T_SECT_INFO_D& SectInfoD, T_SECT_STIFFNESS& EffeStiff)
{
    int nPartSize = SectInfoD.aSectPart.GetSize();
    if(nPartSize<=0) {return FALSE;}

    double dAreaSum = 0.0;
    double dArea_z_Sum = 0.0;
    double dRyy_Sum = 0.0;
    for(int i=0; i<nPartSize; ++i )
    {
        T_SECT_PART_D &PART      = SectInfoD.aSectPart[i]; 
        T_SECT_STIFFNESS &Stiffness = PART.Stiff.Stiffness;

        dAreaSum    += Stiffness.Area;
        dArea_z_Sum += Stiffness.Area*Stiffness.Czm;
    }

    double dCzm = dAreaSum < m_dZero ? 0.0 : dArea_z_Sum/dAreaSum;

    for(int i=0; i<nPartSize; ++i )
    {
        T_SECT_PART_D &PART			= SectInfoD.aSectPart[i]; 
        T_SECT_STIFFNESS &Stiffness = PART.Stiff.Stiffness;

        double dGap_z = dCzm - Stiffness.Czm;
        dRyy_Sum += Stiffness.Ryy + Stiffness.Area*pow(dGap_z,2);
    }

    T_SECT_PART_D &PART = SectInfoD.aSectPart[0]; 

    EffeStiff = PART.Stiff.Stiffness; // 거더의 강성을 저장한다.
    double dH_gr_temp = EffeStiff.Czp+EffeStiff.Czm;

    EffeStiff.Area = dAreaSum; // 설계에 필요한 기본 테이터를 저장한다.
    EffeStiff.Ryy  = dRyy_Sum;
    EffeStiff.Czm  = dCzm;
    EffeStiff.Czp  = dH_gr_temp-dCzm;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveCzmData4Sect(bool bPosiI, BOOL bPositive, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, 
                                                    T_DGNLINE_LBD& LineLBD, double& dA_eff, double& dCzm_eff, double& dIy_eff)
{
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dtw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dtf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dtf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);

    CArray<T_STIFF_CALC_D, T_STIFF_CALC_D&> aStiffCalcD;
    T_STIFF_CALC_D StiffCalcD;

    // Top Flange Part
    double dA_tf		= dB1*dtf1;
    double dz_tf		= dH-0.5*dtf1;
    double dAz_tf		= dA_tf*dz_tf;
    StiffCalcD.dArea	= dA_tf;
    StiffCalcD.dIy	= Get_InertiaOfSB(dtf1, dB1);
    StiffCalcD.dz		= dz_tf;
    aStiffCalcD.Add(StiffCalcD);

    // Bottom Flange Part
    double dA_bf		= dB2*dtf2;
    double dz_bf		= 0.5*dtf2;
    double dAz_bf		= dA_bf*dz_bf;
    StiffCalcD.dArea	= dA_bf;
    StiffCalcD.dIy	= Get_InertiaOfSB(dtf2, dB2);
    StiffCalcD.dz		= dz_bf;
    aStiffCalcD.Add(StiffCalcD);

    double dArea_eff	= dA_tf + dA_bf;    
    double dAz_eff	= dAz_tf + dAz_bf;

    int iSubLineSize		= LineLBD.aDgnBaseD.GetSize();
    int iLineStressType	= LineLBD.iLineStressType;
    int iCompPanelSize	= LineLBD.iCompPanelSize;
    BOOL bCompStr = FALSE, bDbCurve = FALSE;  

    // -1=양연인장 
    // 0=양연압축이고 aDgnBaseD의 첫번째 dStr1이 Sig1(Sig1은 압축응력이며 둘 중 큰값)
    // 1=양연압축이고 aDgnBaseD의 마지막 dStr2가 Sig1
    // 2=양연압축/인장 aDgnBaseD의 첫번째 dStr1이 Sig1
    // 3=양연압축/인장 aDgnBaseD의 마지막 dStr2가 Sig1
    if(iLineStressType==0 || iLineStressType==2)
    {
        double dCurrZ = dH - dtf1;
        for(int i=0; i < iSubLineSize; i++)
        {      
            T_GLINE_DGN_B LineDgnB = LineLBD.aDgnBaseD[i];
            T_SUB_PANEL_BASE SubPanD = LineDgnB.SubPanelD;
            CPG_LSTI_DATA LstiD; LstiD.Initialize();
            double dbs = LineDgnB.db;     
            double dts = LineDgnB.dt;
            double db_blank =SubPanD.db_blank_eff;
            bCompStr = LineDgnB.bCompStr;
            bDbCurve = LineDgnB.bDbCurve;

            if(bCompStr)
            {
                if(!bDbCurve)
                { 
                    double db_sup_eff = SubPanD.db_sup_eff;          
                    double dA_sup_eff = SubPanD.dA_sup_eff;
                    double dz_sup_eff = dCurrZ-0.5*SubPanD.db_sup_eff;
                    double dAz_sup_eff = dA_sup_eff*dz_sup_eff;
                    StiffCalcD.dArea = dA_sup_eff;
                    StiffCalcD.dIy = Get_InertiaOfSB(db_sup_eff, dts);
                    StiffCalcD.dz = dz_sup_eff;
                    aStiffCalcD.Add(StiffCalcD);

                    //////////////////////////////////////////////////////////////////////////    
                    double db_inf_eff = SubPanD.db_inf_eff;
                    double dA_inf_eff = SubPanD.dA_inf_eff;
                    double dz_inf_eff = dCurrZ-dbs+0.5*SubPanD.db_inf_eff;
                    double dAz_inf_eff = dA_sup_eff*dz_inf_eff;
                    StiffCalcD.dArea = dA_inf_eff;
                    StiffCalcD.dIy = Get_InertiaOfSB(db_inf_eff, dts);
                    StiffCalcD.dz = dz_inf_eff;
                    aStiffCalcD.Add(StiffCalcD);          

                    dArea_eff += dA_sup_eff + dA_inf_eff;            
                    dAz_eff += dAz_sup_eff + dAz_inf_eff; 
                }
                else
                {
                    // Test ******************************************
                    // 압축 큰 곳 유효길이
                    double db_sup_eff = SubPanD.db_sup_eff;          
                    double dA_sup_eff = SubPanD.dA_sup_eff;
                    double dz_sup_eff = 0.0;
                    if(bPositive) {dz_sup_eff = dCurrZ-0.5*SubPanD.db_sup_eff;	  }
                    else          {dz_sup_eff = dCurrZ-dbs+0.5*SubPanD.db_sup_eff;} 

                    double dAz_sup_eff	= dA_sup_eff*dz_sup_eff;
                    StiffCalcD.dArea	= dA_sup_eff;
                    StiffCalcD.dIy		= Get_InertiaOfSB(db_sup_eff, dts);
                    StiffCalcD.dz		= dz_sup_eff;
                    aStiffCalcD.Add(StiffCalcD);

                    // 압축 작은 곳 유효길이
                    double db_inf_eff = SubPanD.db_inf_eff;
                    double dA_inf_eff = SubPanD.dA_inf_eff;
                    double dz_inf_eff =0.0;
                    if(bPositive) {dz_inf_eff = dCurrZ-dbs+0.5*SubPanD.db_inf_eff;			 }
                    else          {dz_inf_eff = dCurrZ- SubPanD.dbt - 0.5*SubPanD.db_inf_eff;}

                    double dAz_inf_eff	= dA_sup_eff*dz_inf_eff;
                    StiffCalcD.dArea	= dA_inf_eff;
                    StiffCalcD.dIy		= Get_InertiaOfSB(db_inf_eff, dts);
                    StiffCalcD.dz		= dz_inf_eff;
                    aStiffCalcD.Add(StiffCalcD);          

                    dArea_eff += dA_sup_eff  + dA_inf_eff;            
                    dAz_eff   += dAz_sup_eff + dAz_inf_eff; 

                    //////////////////////////////////////////////////////////////////////////     
                    // 인장축
                    double dLen_ten = fabs(LineDgnB.dz2-LineDgnB.dz0);
                    double dA_ten = dLen_ten*LineDgnB.dt;

                    double dz_ten = 0.0;
                    if(bPositive) {dz_ten = dCurrZ-dbs+0.5*dLen_ten;} 
                    else          {dz_ten = dCurrZ-0.5*dLen_ten; 	}

                    double dAz_ten = dA_ten*dz_ten;
                    StiffCalcD.dArea = dA_ten;
                    StiffCalcD.dIy = Get_InertiaOfSB(dLen_ten, dts);
                    StiffCalcD.dz = dz_ten;
                    aStiffCalcD.Add(StiffCalcD);

                    dArea_eff += dA_ten;  
                    dAz_eff   += dAz_ten; 
                }        
            }
            else // 인장
            {        
                double dA_ten	= dbs*dts;
                double dz_ten	= dCurrZ-0.5*dbs;
                double dAz_ten	= dA_ten*dz_ten;
                StiffCalcD.dArea = dA_ten;
                StiffCalcD.dIy = Get_InertiaOfSB(dbs, dts);
                StiffCalcD.dz = dz_ten;
                aStiffCalcD.Add(StiffCalcD);

                dArea_eff += dA_ten;
                dAz_eff += dA_ten*dz_ten;
            }

            if(iSubLineSize>1) 
            {
                if(i<iSubLineSize-1) 
                {
                    LstiD = arLstiPosi[i];
                    double db_sf = LstiD.dWidth+dts;
                    double dt_sf = LstiD.dt;
                    double dA_sf = LstiD.dArea + dts*LstiD.dt; // web 일부분 + stiffener
                    double dz_sf = dH-dtf1-LstiD.dz;
                    double dAz_sf = dA_sf*dz_sf;
                    StiffCalcD.dArea = dA_sf;
                    StiffCalcD.dIy = Get_InertiaOfSB(dt_sf, db_sf);
                    StiffCalcD.dz = dz_sf;
                    aStiffCalcD.Add(StiffCalcD);          

                    dArea_eff += dA_sf;          
                    dAz_eff += dAz_sf;
                }
            }
            dCurrZ -= dbs + LstiD.dt;
        }
    }
    else if(iLineStressType==1 || iLineStressType==3)
    {
        double dCurrZ = dtf2;
        for(int i=0; i < iSubLineSize; i++)
        {      
            int idx=iSubLineSize-1-i;
            T_GLINE_DGN_B LineDgnB = LineLBD.aDgnBaseD[idx];
            T_SUB_PANEL_BASE SubPanD = LineDgnB.SubPanelD;
            CPG_LSTI_DATA LstiD; LstiD.Initialize();
            double dbs = LineDgnB.db;     
            double dts = LineDgnB.dt;
            bCompStr = LineDgnB.bCompStr;
            bDbCurve = LineDgnB.bDbCurve;

            if(bCompStr)
            {
                if(!bDbCurve)
                { 
                    double db_sup_eff = SubPanD.db_sup_eff;          
                    double dA_sup_eff = SubPanD.dA_sup_eff;
                    double dz_sup_eff = dCurrZ+0.5*SubPanD.db_sup_eff;
                    double dAz_sup_eff = dA_sup_eff*dz_sup_eff;
                    StiffCalcD.dArea = dA_sup_eff;
                    StiffCalcD.dIy = Get_InertiaOfSB(db_sup_eff, dts);
                    StiffCalcD.dz = dz_sup_eff;
                    aStiffCalcD.Add(StiffCalcD);

                    //////////////////////////////////////////////////////////////////////////    
                    double db_inf_eff = SubPanD.db_inf_eff;
                    double dA_inf_eff = SubPanD.dA_inf_eff;
                    double dz_inf_eff = dCurrZ+dbs-0.5*SubPanD.db_inf_eff;
                    double dAz_inf_eff = dA_sup_eff*dz_inf_eff;
                    StiffCalcD.dArea = dA_inf_eff;
                    StiffCalcD.dIy = Get_InertiaOfSB(db_inf_eff, dts);
                    StiffCalcD.dz = dz_inf_eff;
                    aStiffCalcD.Add(StiffCalcD);          

                    dArea_eff += dA_sup_eff + dA_inf_eff;            
                    dAz_eff += dAz_sup_eff + dAz_inf_eff; 
                }
                else
                {
                    double db_sup_eff = SubPanD.db_sup_eff;          
                    double dA_sup_eff = SubPanD.dA_sup_eff;
                    double dz_sup_eff = dCurrZ+0.5*SubPanD.db_sup_eff;
                    double dAz_sup_eff = dA_sup_eff*dz_sup_eff;
                    StiffCalcD.dArea = dA_sup_eff;
                    StiffCalcD.dIy = Get_InertiaOfSB(db_sup_eff, dts);
                    StiffCalcD.dz = dz_sup_eff;
                    aStiffCalcD.Add(StiffCalcD);

                    //////////////////////////////////////////////////////////////////////////          
                    double dLen_ten = fabs(LineDgnB.dz2-LineDgnB.dz0);
                    double dA_ten = dLen_ten*LineDgnB.dt;
                    double dz_ten = dCurrZ+dbs-0.5*dLen_ten;          
                    double dAz_ten = dA_ten*dz_ten;
                    StiffCalcD.dArea = dA_ten;
                    StiffCalcD.dIy = Get_InertiaOfSB(dLen_ten, dts);
                    StiffCalcD.dz = dz_ten;
                    aStiffCalcD.Add(StiffCalcD);

                    dArea_eff += dA_sup_eff + dA_ten;  
                    dAz_eff += dAz_sup_eff + dAz_ten; 
                }        
            }
            else // 인장
            {        
                double dA_ten = dbs*dts;
                double dz_ten = dCurrZ+0.5*dbs;
                double dAz_ten = dA_ten*dz_ten;
                StiffCalcD.dArea = dA_ten;
                StiffCalcD.dIy = Get_InertiaOfSB(dbs, dts);
                StiffCalcD.dz = dz_ten;
                aStiffCalcD.Add(StiffCalcD);

                dArea_eff += dA_ten;
                dAz_eff += dA_ten*dz_ten;
            }
            if(iSubLineSize>1) 
            {
                if(idx<iSubLineSize-1) 
                {
                    LstiD = arLstiPosi[idx];
                    double db_sf = LstiD.dWidth+dts;
                    double dt_sf = LstiD.dt;
                    double dA_sf = LstiD.dArea + dts*LstiD.dt; // web 일부분 + stiffener
                    double dz_sf = dH-dtf1-LstiD.dz;
                    double dAz_sf = dA_sf*dz_sf;
                    StiffCalcD.dArea = dA_sf;
                    StiffCalcD.dIy = Get_InertiaOfSB(dt_sf, db_sf);
                    StiffCalcD.dz = dz_sf;
                    aStiffCalcD.Add(StiffCalcD);          

                    dArea_eff += dA_sf;          
                    dAz_eff += dAz_sf;
                }
            }
            dCurrZ -= dbs + LstiD.dt;
        }
    }
    else if(iLineStressType==-1)
    {    
        StiffCalcD.dArea = dh*dtw;
        StiffCalcD.dIy = Get_InertiaOfSB(dh, dtw);
        StiffCalcD.dz = dtf2+0.5*dh;
        aStiffCalcD.Add(StiffCalcD);          

        dArea_eff += dh*dtw;
        dAz_eff += dh*dtw*(dtf2+0.5*dh);
    }
    else 
        ASSERT(0);

    dCzm_eff = dArea_eff < m_dZero ? 0.0 : dAz_eff/dArea_eff;

    double dCzm_eff2=0.0, dIy_eff2=0.0;
    Get_StiffnessOfEffeSect(aStiffCalcD, dCzm_eff2, dIy_eff2);

    dA_eff	= dArea_eff;
    dCzm_eff	= dCzm_eff2;
    dIy_eff	= dIy_eff2;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LineDgnInfo4SectDB(bool bPosiI, double dfy1, double dfy2, double dEs, double dPoisson, double* aForce, T_SECT_D& SectD, 
                                                 double da, T_SECT_STIFFNESS& AppStiff, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, 
                                                 CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aLineDgnBase, T_DGNLINE_LBD& LineLBD)
{
    double dFx = aForce[0], dMy = aForce[4], dMz = aForce[5];
    //
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dtw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dtf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dtf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dArea	= AppStiff.Area;
    double dIy	= AppStiff.Ryy;
    double dIz	= AppStiff.Rzz;

    double dy1=0., dz1=0., dy2=0., dz2=0., dy3=0., dz3=0., dy4=0., dz4=0.;
    AppStiff.GetPosition(dy1, dz1, dy2, dz2, dy3, dz3, dy4, dz4);
    double dbi=0.0, dti=0.0;
    double dyi1=0.0, dyi2=0.0, dzi1=0.0, dzi2=0.0; 

    int iStressType = 0, iNtype = 0,iCompPanelSize = 0;
    double dStressI=0.0, dStressJ=0.0, dStr1=0.0, dStr2=0.0, df1=0.0, df2=0.0;
    double dbc_sum=0.0, dbt_sum=0.0, dk_sig=0.0, dLamda_p=0.0, dRho=0.0, dPsi=0.0, dbs=0.0, dts=0.0;
    BOOL bInternal=TRUE;
    BOOL bIs1_sig1=FALSE;
    BOOL bLineDbCurve=FALSE;

    int iLineDgnSize = aLineDgnBase.GetSize();  

    for(int i=0; i < iLineDgnSize; i++)
    {
        T_GLINE_DGN_B LineDgn = aLineDgnBase[i];
        dbs = LineDgn.db;
        dts = LineDgn.dt;
        double dfyi = Get_fy4Thick(dfy1, dfy2, dts);
        double dEpsilon = Get_Epsilon(dfyi);

        double dStr1_temp = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, LineDgn.dy1, LineDgn.dz1);
        double dStr2_temp = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, LineDgn.dy2, LineDgn.dz2);

        // test	*************************
        if(dMy>=0.0) // Positive
        {
            dStr1 = dStr1_temp;
            dStr2 = dStr2_temp;
        }
        else
        {
            dStr1 = dStr2_temp;
            dStr2 = dStr1_temp;
        }
        // ******************************

        Get_f1_f2(dStr1, dStr2, df1, df2, bIs1_sig1);
        LineDgn.dStr1     = dStr1;
        LineDgn.dStr2     = dStr2;
        LineDgn.df1       = df1;   // psi 계산 위한 응력값. (f1: 압축응력(+) 큰 것   
        LineDgn.df2       = df2;
        LineDgn.bIs1_sig1 = bIs1_sig1;
        BOOL bCompStr	= Is_CompStress(dStr1, dStr2);
        BOOL bDbCurve	= Is_DoubleCurvature(dStr1, dStr2);
        LineDgn.bCompStr = bCompStr;
        LineDgn.bDbCurve = bDbCurve;   

        if(i==0) { dStressI = dStr1, dStressJ = dStr2; }
        else     { dStressJ = dStr2; }

        iStressType = Get_StressDistributionType4Internal(dStr1, dStr2);
        T_SUB_PANEL_BASE SubPanelD;
        if(bCompStr)
        {      
            iCompPanelSize++;
            dPsi     = Get_Psi(df1, df2);
            dk_sig   = Get_InternalBucklingfactor(dPsi);
            dLamda_p = Get_Lamda_p(dbs, dts, dEpsilon, dk_sig);
            Get_ReductionFactorRho(bInternal, dbs, dts, dLamda_p, dPsi, dRho);

            SubPanelD.nType    = iStressType;
            SubPanelD.dPsi     = dPsi;
            SubPanelD.dk_sigma = dk_sig;
            SubPanelD.dLamda_p = dLamda_p;
            SubPanelD.dRho     = dRho;
            int iNtype=0, iNtype2=0;
            BOOL bGrossWidth=TRUE;
            double dbb=0.0, db_eff=0.0;

            if(bInternal)
            {       
                if(!bDbCurve)
                {           
                    dbc_sum += dbs;
                    iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);           
                    // Gross width
                    bGrossWidth = TRUE;
                    dbb = Get_Width4StiffenedPlates(iNtype, dbs, dPsi); 
                    Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dts, SubPanelD);
                    //
                    dbb = Get_Width4StiffenedPlates(iNtype2, dbs, dPsi); 
                    Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dts, SubPanelD);
                    // Effective width
                    bGrossWidth = FALSE;
                    db_eff = dRho*dbs;
                    dbb = Get_Width4StiffenedPlates(iNtype, db_eff, dPsi);
                    Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dts, SubPanelD);
                    //
                    dbb = Get_Width4StiffenedPlates(iNtype2, db_eff, dPsi);
                    Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dts, SubPanelD);   

                    SubPanelD.db_blank_eff =  dbs - db_eff; 
                }
                else
                {
                    // test  *******************
                    if(iLineDgnSize==1)
                    {
                        bLineDbCurve=TRUE; // 하나라도 있으면. 

                        double dbeff = 0.0;
                        double dbe1  = 0.0;
                        double dbe2  = 0.0;
                        Get_EffectiveWidth4Internal(dPsi, dbs, dRho, dbeff, dbe1, dbe2);

                        double dbc=0.0; // 압축영역길이.
                        Get_CompWidthOfDoubleCurvatrue(LineDgn, dbc);

                        dbc_sum += dbeff;       // 유효압축영역
                        dbt_sum += dbs - dbc;   // 인장영역
                        SubPanelD.dbc = dbc;
                        SubPanelD.dbt = dbs - dbc;

                        SubPanelD.db_sup_eff = dbe1;        // 압축 큰곳
                        SubPanelD.dA_sup_eff = dbe1 * dts;
                        SubPanelD.db_inf_eff = dbe2;       
                        SubPanelD.dA_inf_eff = dbe2 * dts;
                        SubPanelD.db_blank_eff =  dbc - dbeff;

                    }// *****************************
                    else
                    {
                        bLineDbCurve=TRUE; // 하나라도 있으면. 
                        double dbc=0.0;
                        Get_CompWidthOfDoubleCurvatrue(LineDgn, dbc);
                        dbc_sum += dbc;         // 압축영역
                        dbt_sum += dbs - dbc;   // 인장영역
                        SubPanelD.db_sup = dbc;
                        iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);          
                        dbb = Get_Width4StiffenedPlates(iNtype, dbc, dPsi, TRUE);
                        SubPanelD.db_sup_eff = dbb;          
                    }
                }
            }    
        }
        else // Tension
        {

        }

        LineDgn.SubPanelD = SubPanelD;    
        aLineDgnBase.SetAt(i, LineDgn);
    }
    LineLBD.aDgnBaseD.Copy(aLineDgnBase);  

    int iLineStressType=-1;
    BOOL bIs1_f1 = FALSE;
    double df1_temp=0.0, df2_temp=0.0;
    Get_f1_f2(dStressI, dStressJ, df1_temp, df2_temp, bIs1_f1);
    if(!bLineDbCurve)
    {      
        if(bIs1_f1) iLineStressType = 0;
        else        iLineStressType = 1;
    }
    else
    {
        if(bIs1_f1) iLineStressType = 2;
        else        iLineStressType = 3;
    }

    // Stiffener
    double dAsl_sum=0.0;
    double dIsl_sum=0.0;
    double dIp=0.0;
    int iSubPanelSize = iLineDgnSize;
    if(bInternal && iSubPanelSize>1)
    {
        LineLBD.aDgnStfnD.SetSize(iSubPanelSize-1);
        dIp = Get_Ip_4BendingOfPlate(dh, dtw, dPoisson);
        for(int i=0; i < iSubPanelSize-1; i++)
        {
            //T_GLINE_STIFFENER StfnD = LineDst.aStiffener[i];
            CPG_LSTI_DATA LstiD = arLstiPosi[i];
            double db_ls = LstiD.dWidth;
            double dt_ls = LstiD.dt;
            double dA_ls = LstiD.dArea;
            T_GLINE_DGN_B PreLineDgn = aLineDgnBase[i];
            T_GLINE_DGN_B NextLineDgn = aLineDgnBase[i+1];
            int iPreStrType  = PreLineDgn.SubPanelD.nType;
            int iNextStrType = NextLineDgn.SubPanelD.nType;
            double db_inf=0.0, db_sup=0.0, db_inf_eff=0.0, db_sup_eff=0.0, dt_inf=0.0, dt_sup=0.0;

            if(iPreStrType==Internal_Stress_Type1 || iPreStrType==Internal_Stress_Type2)
            {
                db_inf = PreLineDgn.SubPanelD.db_inf;
                db_sup = NextLineDgn.SubPanelD.db_sup;
                db_inf_eff = PreLineDgn.SubPanelD.db_inf_eff;
                db_sup_eff = NextLineDgn.SubPanelD.db_sup_eff;
                dt_inf = PreLineDgn.dt;
                dt_sup = NextLineDgn.dt;
            }
            else if(iPreStrType==Internal_Stress_Type3)
            {
                db_inf = NextLineDgn.SubPanelD.db_inf;
                db_sup = PreLineDgn.SubPanelD.db_sup;
                db_inf_eff = NextLineDgn.SubPanelD.db_inf_eff;
                db_sup_eff = PreLineDgn.SubPanelD.db_sup_eff;
                dt_inf = NextLineDgn.dt;
                dt_sup = PreLineDgn.dt;
            }
            else
            {
                // 인장. 
            }

            double dt_m = dt_inf;
            double dA_sf = LstiD.dArea;//StfnD.dA;
            double dI_sf = Get_InertiaOfStiffener(db_ls, dt_ls);
            double dZbar_sf = 0.5*db_ls;

            double dA_inf = db_inf*dt_inf;      
            double dA_sup = db_sup*dt_sup;
            double dA_mc  = dt_ls*dt_m;
            double dAsl = dA_inf + dA_sup + dA_mc + dA_sf;
            double dSf_bar = 0.5*dt_m+dZbar_sf;
            double dXsl = fabs(dAsl) < m_dZero ? 0.0 : dSf_bar*dA_sf/dAsl; // 모재 두께의 1/2이 기준. 
            double dI_inf = db_inf*pow(dt_inf,3)/12.0;
            double dI_sup = db_sup*pow(dt_sup,3)/12.0;
            double dI_mc  = dt_ls*pow(dt_m,3)/12.0;
            double dIsl = (dI_inf+dI_sup+dI_mc)+(dA_inf+dA_sup+dA_mc)*pow(dXsl,2) + dI_sf + dA_sf*pow((dSf_bar-dXsl),2);
            double de1 = dXsl;
            double de2 = dSf_bar-dXsl;
            double dSigma_sl = Get_Sigma_cr_sl4ColumnTypeBehaviour(dEs, dIsl, dAsl, da);
            //
            double dA_inf_eff = db_inf_eff*dt_inf;
            double dA_sup_eff = db_sup_eff*dt_sup;
            double dAsl_eff   = dA_inf_eff + dA_sup_eff + dA_sf;      

            dAsl_sum += dAsl;
            dIsl_sum += dIsl;

            T_GLINE_DGN_STFN DgnStfn;
            DgnStfn.dA  = dA_sf;
            DgnStfn.dIy = dI_sf;
            DgnStfn.dZbar = dZbar_sf;
            DgnStfn.db_inf = db_inf;
            DgnStfn.db_inf = db_inf_eff;
            DgnStfn.db_sup = db_sup;
            DgnStfn.db_sup_eff = db_sup_eff;
            DgnStfn.dAsl = dAsl;
            DgnStfn.dIsl = dIsl;
            DgnStfn.dXsl = dXsl;
            DgnStfn.de1 = de1;
            DgnStfn.de2 = de2;
            DgnStfn.de = max(de1,de2);
            DgnStfn.dAsl_eff = dAsl_eff;      
            DgnStfn.dSigma_sl = dSigma_sl;

            LineLBD.aDgnStfnD.SetAt(i, DgnStfn);
        }    
    }

    //  
    LineLBD.dbc = dbc_sum;
    LineLBD.dbt = dbt_sum;
    LineLBD.iLineStressType = iLineStressType;
    LineLBD.iCompPanelSize = iCompPanelSize;
    LineLBD.dAsl_sum = dAsl_sum;
    LineLBD.dIsl_sum = dIsl_sum;
    LineLBD.dIp = dIp;    

    //    
    double dfyt	= Get_fy4Thick(dfy1, dfy2, dtf1);
    double dfyb	= Get_fy4Thick(dfy1, dfy2, dtf2);
    double dfyw	= Get_fy4Thick(dfy1, dfy2, dtw);
    Get_ElasticCriticalPlateBucklingData(dfyb, da, dEs, dPoisson, aForce, LineLBD);
    //
    BOOL bIsOpenStfn = TRUE; // I형은 직사각형 보강재만 입력가능 
    Get_ColumnBucklingData(dfyb, da, dEs, dPoisson, bIsOpenStfn, aForce, LineLBD);

    T_DGNLINE_PTB PlateBuckD = LineLBD.PlateBuckD;
    double dSigma_cr_p = PlateBuckD.dSigma_cr_p;
    double dRho_p = PlateBuckD.dRho;

    T_DGNLINE_CTB ColumnBuckD = LineLBD.ColumnBuckD;
    double dSigma_cr_c = ColumnBuckD.dSigma_cr_c;
    double dXc = ColumnBuckD.dXc;

    double dxi = Get_xi(dSigma_cr_p, dSigma_cr_c);
    double dRho_c = Get_Rho_c_FinalReductionFactor(dRho_p, dXc, dxi);

    LineLBD.dxi = dxi;
    LineLBD.dRho_c = dRho_c;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LineInfo4SectDB(bool bPosiI, T_SECT_D& SectD, T_SECT_STIFFNESS& AppStiffD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, 
                                              CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aLineDgnBase)
{
    double dH	= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dtw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dtf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dtf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dCym	= AppStiffD.Cym;
    double dCzm	= AppStiffD.Czm;

    int iStiffSize = arLstiPosi.GetSize();  
    int iLineDgnSize = iStiffSize + 1;
    aLineDgnBase.SetSize(iLineDgnSize);

    for(int i=0; i < iLineDgnSize; i++)
    {
        CPG_LSTI_DATA LstiD; LstiD.Initialize();
        if(iStiffSize!=i) {LstiD = arLstiPosi[i];}
        T_GLINE_DGN_B LineDgn;
        LineDgn.iLineType = 0; // 0=Internal, 1=Outstand
        // 먼저 좌하단 원점좌표 기준으로 만들자. 
        double dyi = dCym, dzi1 = 0.0, dzi2 = 0.0, dbi = 0.0;
        double dStiffSta = dH-dtf1;

        if(i==0) // first
        {
            if(iLineDgnSize==1)
            {
                dzi1 = dH-dtf1;
                dzi2 = dtf2;        
            }
            else
            {
                dzi1 = dH-dtf1;
                dzi2 = dzi1-(LstiD.dz-0.5*LstiD.dt);        
            }
        }
        else if(iLineDgnSize-1==i) // last
        {
            CPG_LSTI_DATA PreLstiD = arLstiPosi[i-1];
            dzi1 = dStiffSta-(PreLstiD.dz+0.5*PreLstiD.dt);
            dzi2 = dtf2;
        }
        else // mid
        {
            CPG_LSTI_DATA PreLstiD = arLstiPosi[i-1];
            dzi1 = dStiffSta-(PreLstiD.dz+0.5*PreLstiD.dt);
            dzi2 = dStiffSta-(LstiD.dz-0.5*LstiD.dt);
        }
        dbi = dzi1-dzi2;
        LineDgn.dy1 = dyi;
        LineDgn.dz1 = dzi1;
        LineDgn.dy2 = dyi;
        LineDgn.dz2 = dzi2;
        LineDgn.db = dbi;
        LineDgn.dt = dtw;
        aLineDgnBase.SetAt(i, LineDgn);
    }

    // 도심기준으로 좌표를 변환 
    Get_ConvertPoint4Centroid(dCym, dCzm, aLineDgnBase);

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_InertiaOfStiffener(double db, double dt)
{
    double dIy = dt*pow(db,3)/12.0;
    return dIy;
}

BOOL CDgnPlateGirder_IRC::Get_StressOfPosition(double aGirderForce[6], double aCptForce[6], T_SECT_STIFFNESS& GdStiffD, T_SECT_STIFFNESS& StiffD, double aStress[4])
{
    double dArea_a = GdStiffD.Area;
    double dIy_a   = GdStiffD.Ryy;
    double dIz_a   = GdStiffD.Rzz;
    double dArea = StiffD.Area;
    double dIy   = StiffD.Ryy;
    double dIz   = StiffD.Rzz;

    double dFx_a = aGirderForce[0];
    double dMy_a = aGirderForce[4];
    double dMz_a = aGirderForce[5];
    double dFx_c = aCptForce[0];
    double dMy_c = aCptForce[4];
    double dMz_c = aCptForce[5];

    double aStress_a[4]={0.0}, aStress_c[4]={0.0};
    // Girder
    aStress_a[0] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx1, GdStiffD.dy1);
    aStress_a[1] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx2, GdStiffD.dy2);
    aStress_a[2] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx3, GdStiffD.dy3);
    aStress_a[3] = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, GdStiffD.dx4, GdStiffD.dy4);
    // Composite
    aStress_c[0] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx1, StiffD.dy1);
    aStress_c[1] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx2, StiffD.dy2);
    aStress_c[2] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx3, StiffD.dy3);
    aStress_c[3] = Calc_StressOfPosition(dFx_c, dMy_c, dMz_c, dArea, dIy, dIz, StiffD.dx4, StiffD.dy4);

    for(int i=0; i<4; i++) 
    {aStress[i] = aStress_a[i] + aStress_c[i];}

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ConvertPoint4Centroid(double dCym, double dCzm, CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aLineDgnBase)
{
    // CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&> aLineDgnBase의 좌표가 원점 기준일때만 적용
    int iLineDgnSize = aLineDgnBase.GetSize();
    // 도심기준으로 좌표를 변환 
    for(int i=0; i<iLineDgnSize; i++)
    {
        T_GLINE_DGN_B LineDgn = aLineDgnBase[i];
        LineDgn.dy1 -= dCym;
        LineDgn.dy2 -= dCym;
        LineDgn.dz1 -= dCzm;
        LineDgn.dz2 -= dCzm;   
        aLineDgnBase.SetAt(i, LineDgn);
    }
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_InternalWebClassClassification(bool bPosiI, int iBotClass, double aForce[6], double dStress[4], CPG_MATL_ELEM& MatlElem, 
                                                             const T_SECT_D& crSectD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, 
                                                             int& iClass, T_CLAS_BASE& ClasBase)
{
    iClass				= 0;
    ClasBase.iWebDeter	= 0;
    ClasBase.dPsi[4]		= 0.0; 
    ClasBase.dk_sig[4]	= 0.0;
    ClasBase.dRho[4]		= 0.0;
    ClasBase.dbeff[4]		= 0.0;
    ClasBase.dbe1[4]		= 0.0;
    ClasBase.dbe2[4]		= 0.0;

    double dStressTL = dStress[0];
    double dStressTR = dStress[1];
    double dStressBR = dStress[2];
    double dStressBL = dStress[3];

    if(dStressTL > 0.0 && dStressTR > 0.0 && dStressBR > 0.0 && dStressBL > 0.0)
    {
        iClass = 1;
        return TRUE;
    }

    double dFx = aForce[0];
    double dMy = aForce[4];
    double dMz = aForce[5];
    BOOL bPositiveMy = dMy > -m_dZero;

    // Material
    double dgam_M0 = m_dGamma_M0;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;

    double dfy	= MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy1	= MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfck	= MatlElem.dfck;
    double dfsk	= MatlElem.MatdD.MainRebarData.B_fy;
    double dfcd	=  0.67* dfck/dgam_c;
    double dfsd	= dfsk/dgam_sr;

    // Section
    double dH		= m_pDgnSectUtil->Get_Size_H(&crSectD, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&crSectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&crSectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);	
    double dh		= m_pDgnSectUtil->Get_Size_hw(&crSectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&crSectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&crSectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&crSectD);

    double dfyt = MatlElem.MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatlElem.MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatlElem.MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    double dPlasticNA = 0.0;
    double dConcF = 0.0, dRebarF = 0.0;
    double dStopfF = 0.0, dSbotfF=0.0, dSwebF=0.0, dGirderF=0.0;
    BOOL bPlasticClass = FALSE;
    BOOL bDeterClass = TRUE;

    double dfy_r = (dTw < 40.0 + m_dZero) ? dfy : dfy1;
    double dEpsi = Get_Epsilon(dfy_r);
    double dct = dh/dTw;
    double dbt=0.0, dbc=0.0, dc=0.0;
    double dAlpha=0.0;
    double dPsi=0.0;
    double dk_sig=0.0;
    double dLamda_p=0.0;
    double dRho  = 0.0;
    double dbeff = 0.0;
    double dbe1  = 0.0, dbe2 = 0.0;

    int iWebDeter = 0;
    BOOL bInternal = TRUE;
    BOOL bTensionWeb = FALSE;

    if(bPositiveMy)
    {
        if(dTf2 < m_dPlasticNA && m_dPlasticNA < dTf2+dh + m_dZero) // PNA in Web
        {
            dbc = dTf2 + dh - m_dPlasticNA;
        }
        else if(dTf2 > m_dPlasticNA) // PNA in Bottom Flange
        {
            dbc = dh;
        }
        else if(dTf2 + dh < m_dPlasticNA) // PNA in Top Flange
        {
            bTensionWeb = TRUE;
        }

        dAlpha = dbc/dh;
    }
    else
    {
        if(dTf2 < m_dPlasticNA && m_dPlasticNA < dTf2 + dh + m_dZero) // PNA in Web
        {
            dbc = m_dPlasticNA - dTf2;
        }
        else if(dTf2 > m_dPlasticNA)// PNA in Bottom Flange
        {
            bTensionWeb = TRUE;
        }
        else if(dTf2 + dh < m_dPlasticNA) // PNA in Top Flange
        {
            dbc = dh;
        }

        dAlpha = dbc/dh;
    }

    if(!bTensionWeb)
    {
        iClass = Get_WebPlaticClass(dct, dEpsi, dAlpha);    
    }
    else
    {
        iClass = 1;    
    }

    // Plastic으로 결정이 되면 리턴.  
    double dStressT = (dStressTL + dStressTR)/2.0;
    double dStressB = (dStressBL + dStressBR)/2.0;  

    if(iClass>2)
    {
        if(dStressT==0.0 && dStressB==0.0)
        {
            iClass = 1;
            bDeterClass = TRUE;      
        }
        else if(dStressT<0.0 && dStressB<0.0)
        {
            dAlpha = 1.0;
            dbc = dh;
            if(fabs(dStressT-dStressB)<m_dZero)
            {      
                dPsi = 1.0;
                dk_sig = Get_InternalBucklingfactor(dPsi);
                dLamda_p = Get_Lamda_p(dh, dTw, dEpsi, dk_sig);
                Get_ReductionFactorRho(bInternal, dh, dTw, dLamda_p, dPsi, dRho);
                dbeff = dRho*dbc;
                dbe1 = 0.5*dbeff;
                dbe2 = 0.5*dbeff;
            }
            else
            {
                if(bPositiveMy)
                {
                    dPsi = dStressB/dStressT;
                    iWebDeter = 1; // Top = Sigma1
                }
                else
                {
                    dPsi = dStressT/dStressB;
                    iWebDeter = 2; // Bot = Sigma1
                }
                dk_sig = Get_InternalBucklingfactor(dPsi);
                dLamda_p = Get_Lamda_p(dh, dTw, dEpsi, dk_sig);
                Get_ReductionFactorRho(bInternal, dh, dTw, dLamda_p, dPsi, dRho);

                dbeff = dRho*dbc;
                dbe1 = 2.0/(5.0-dPsi)*dbeff;
                dbe2 = dbeff-dbe1;
            }          
            //
            double dr1,dr2;
            Calc_r1r2(dr1,dr2);
            iClass = Get_WebElasticClass(dAlpha, dct,dEpsi,dr2);
            bDeterClass = TRUE;
        }
        else if(dStressT*dStressB<=0.0)
        {
            double dx0=0.0;
            if(dStressT<0.0 && dStressB>=0.0)
            {        
                dbc = fabs(dStressT)/(fabs(dStressT)+fabs(dStressB))*dh;
                dAlpha = dbc/dh;
                dPsi = dStressB/dStressT;
                iWebDeter = 1; // Top = Sigma1
            }
            else if(dStressT>=0.0 && dStressB<0.0)
            {
                dbc = fabs(dStressB)/(fabs(dStressT)+fabs(dStressB))*dh;
                dAlpha = dbc/dh;
                dPsi = dStressT/dStressB;
                iWebDeter = 2; // Bot = Sigma1
            }
            dk_sig = Get_InternalBucklingfactor(dPsi);
            dLamda_p = Get_Lamda_p(dh, dTw, dEpsi, dk_sig);
            Get_ReductionFactorRho(bInternal, dh, dTw, dLamda_p, dPsi, dRho);
            dbeff = dRho*dh/(1.0-dPsi);
            dbe1 = 0.4*dbeff;
            dbe2 = 0.6*dbeff;
            //
            double dr1,dr2;
            Calc_r1r2(dr1,dr2);
            iClass = Get_WebElasticClass(dAlpha, dct,dEpsi,dr2);
            bDeterClass = TRUE;
        }
    }

    ClasBase.iWebDeter = iWebDeter;
    ClasBase.iClass[4] = iClass;
    ClasBase.dPsi[4]   = dPsi; 
    ClasBase.dk_sig[4] = dk_sig;
    ClasBase.dRho[4]   = dRho;
    ClasBase.dbeff[4]  = dbeff;
    ClasBase.dbe1[4]   = dbe1;
    ClasBase.dbe2[4]   = dbe2;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_InternalPlateClassUnit(bool bPosiI, const T_SECT_D& SectD, const T_CLASS_IN_UNIT_D_IRC& ClassInD, double dfy, double dfy1,
                                                      int& iClass, CLASS_BASE_UNIT_IRC& ClassBase, bool bWeb)
{
    // Initialize 
    iClass = 0;
    ClassBase.Initialize();

    double dStress1		= ClassInD.dStressOuter; 
    double dStress2		= ClassInD.dStressInter;
    double dRangeStress	= fabs(dStress1 - dStress2);
    double dc = ClassInD.dc; 
    double dt = ClassInD.dt;
    double dWTR = dt < m_dZero ? 0.0 : dc / dt;
    double dfy_r = (dt< 40.0 + m_dZero) ? dfy : dfy1;
    double dEpsi = Get_Epsilon(dfy_r);

    if(dStress1 > 0.0 && dStress2 > 0.0)
    {
        iClass = 1; 
        ClassBase.iClass = iClass;
        return TRUE;
    }

    double dAlpha = 0.0, dbc = 0.0, dbt = 0.0;
    double dPsi = 0.0, dk_sig = 0.0, dLamda_p = 0.0, dRho = 0.0;
    double dbeff=0.0, dbe1=0.0, dbe2=0.0;
    int iWebDeter = 0;
    BOOL bInternal = TRUE, bTensionWeb = FALSE, bDeterClass = FALSE;
    BOOL bPositiveMy = TRUE;
    double dTf2 = m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh	= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);

    if(bPositiveMy)
    {
        if(dTf2 < m_dPlasticNA && m_dPlasticNA < dTf2+dh + m_dZero) // PNA in Web
        {
            dbc = dTf2 + dh - m_dPlasticNA;
        }
        else if(dTf2 > m_dPlasticNA) // PNA in Bottom Flange
        {
            dbc = dh;
        }
        else if(dTf2 + dh < m_dPlasticNA) // PNA in Top Flange
        {
            bTensionWeb = TRUE;
        }

        dAlpha = dh < m_dZero ? 0.0 : dbc/dh;
    }
    else
    {
        if(dTf2 < m_dPlasticNA && m_dPlasticNA < dTf2 + dh + m_dZero) // PNA in Web
        {
            dbc = m_dPlasticNA - dTf2;
        }
        else if(dTf2 > m_dPlasticNA)// PNA in Bottom Flange
        {
            bTensionWeb = TRUE;
        }
        else if(dTf2 + dh < m_dPlasticNA) // PNA in Top Flange
        {
            dbc = dh;
        }

        dAlpha = dh < m_dZero ? 0.0 : dbc/dh;
    }

    if(!bTensionWeb)
    {
        iClass = Get_WebPlaticClass(dWTR, dEpsi, dAlpha);    
    }
    else
    {
        iClass = 1;    
    }	

    if(iClass > 2) // EN 1993-1-5:2006, 4.4, Elastic Case, not Class 1 or 2
    {
        if(fabs(dStress1 * dStress2) < m_dZero)
        {
            iClass = 1;
            bDeterClass = TRUE;      
        }
        else if(dStress1 < 0.0 && dStress2 < 0.0)
        {
            dAlpha	= 1.0;
            dbc		= dc;
            if(fabs(dStress1 - dStress2)<m_dZero)
            {      
                dPsi		= 1.0;
                dk_sig		= Get_InternalBucklingfactor(dPsi);
                dLamda_p	= Get_Lamda_p(dc, dt, dEpsi, dk_sig);
                Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
                dbeff = dRho*dbc;
                dbe1 = 0.5*dbeff;
                dbe2 = 0.5*dbeff;
            }
            else
            {
                if(fabs(dStress1) > fabs(dStress2))
                {
                    dPsi = dStress2/dStress1;
                    iWebDeter = 1; // Top = Sigma1
                }
                else
                {
                    dPsi = dStress1/dStress2;
                    iWebDeter = 2; // Bot = Sigma1
                }

                dk_sig		= Get_InternalBucklingfactor(dPsi);
                dLamda_p	= Get_Lamda_p(dc, dt, dEpsi, dk_sig);
                Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);

                dbeff	= dRho*dbc;
                dbe1	= 2.0/(5.0-dPsi)*dbeff;
                dbe2	= dbeff-dbe1;
            }          
            double dr1,dr2;
            Calc_r1r2(dr1,dr2);
            iClass = Get_WebElasticClass(dAlpha, dWTR,dEpsi,dr2);

            bDeterClass = TRUE;
        }
        else if(dStress1 * dStress2 <  m_dZero)
        {
            double dx0 = 0.0;
            if(dStress1 < 0.0 && dStress2 > -m_dZero)
            {        
                dbc = fabs(dStress1)/(fabs(dStress1)+fabs(dStress2))*dc;
                dAlpha = dbc/dc;
                dPsi = dStress2/dStress1;
                iWebDeter = 1; // Top = Sigma1
            }
            else if(dStress1 > -m_dZero && dStress2 < 0.0)
            {
                dbc = fabs(dStress2)/(fabs(dStress1)+fabs(dStress2))*dc;
                dAlpha = dbc/dc;
                dPsi = dStress1/dStress2;
                iWebDeter = 2; // Bot = Sigma1
            }
            dk_sig = Get_InternalBucklingfactor(dPsi);
            dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
            Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
            dbeff = dRho*dc/(1.0-dPsi);
            dbe1 = 0.4*dbeff;
            dbe2 = 0.6*dbeff;
            //
            double dr1,dr2;
            Calc_r1r2(dr1,dr2);
            iClass = Get_WebElasticClass(dAlpha, dWTR, dEpsi,dr2);
            bDeterClass = TRUE;
        }
    }
    int iclassF,iclassW;
    Calc_Class_General(iclassF,iclassW);
    // save result
    ClassBase.iClass	= iClass;
    ClassBase.iWebDeter = iWebDeter;
    ClassBase.dPsi		= dPsi;
    ClassBase.dk_sig	= dk_sig;
    ClassBase.dRho		= dRho;
    ClassBase.dbeff		= dbeff;
    ClassBase.dbe1		= dbe1;
    ClassBase.dbe2		= dbe2;
    ClassBase.dBTR		= dWTR;
    ClassBase.dEpsilon = dEpsi;
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_OutstandClassUnit(const T_CLASS_IN_UNIT_D_IRC& ClassInD, double dfy, double dfy1, 
                                                 int& iClass,CLASS_BASE_UNIT_IRC& ClassBase)
{
    ClassBase.Initialize();
    iClass = 0;

    double dStressOuter	= ClassInD.dStressOuter;
    double dStressInner	= ClassInD.dStressInter; 
    double dc			= ClassInD.dc;
    double dt			= ClassInD.dt;

    // 1. Part subject to compression
    int iClass_N = 0, iClass_NM = 0,iClassF=0;
    double dfy_r = (dt < 40.0 + m_dZero) ? dfy : dfy1;
    double dEpsi = Get_Epsilon(dfy_r);
    double dBTR = dt < m_dZero ? 0.0 : dc / dt;
    BOOL bIsCompression = FALSE;
    double  Eps = dEpsi;
    double 	dBeta2f = 0, dBeta3f = 0; 

    if(dStressOuter < 0.0 && dStressInner < 0.0 && fabs(dStressOuter - dStressInner) < m_dZero)
    {
        bIsCompression = TRUE;
    }
    else bIsCompression = FALSE;

    int iFlClass=0;//0-BTR1 Highest,1-BTR2 Highest,2-BTR3 Highest

    // 	//IRC does not differentiate between tension and compression
    //Bending+compression case included
    // 		if     (dBTR < 9.4*dEpsi  + m_dZero){iClass_N = 1;}      
    // 		else if(dBTR < 10.5*dEpsi + m_dZero){iClass_N = 2;}
    // 		else if(dBTR < 15.7*dEpsi + m_dZero){iClass_N = 3;}
    // 		else							    {iClass_N = 4;}  
    // 		if (m_iType!=45)
    // 		{
    // 			if(dBTR <= 42.*Eps)	  	iClassF = 3;
    // 			else							      iClassF = 4;
    // 
    // 			dBeta2f = 42.*Eps; dBeta3f = 42.*Eps; 
    // 			
    // 		}
    // 		else
    // 		{
    // 			//For compound flange 3 width to thickness ratios need to be considered and worst of them to be used for design
    // 			double iClassF_1=0.0;
    // 			double BTR1=Calc_BTR_HC1();
    // 			
    // 			if(BTR1 <= 9.4*Eps)		  	iClassF_1 = 1;
    // 			else if(BTR1 <= 10.5*Eps) iClassF_1 = 2;
    // 			else if(BTR1 <= 15.7*Eps)	iClassF_1 = 3;
    // 			else							        iClassF_1 = 4;
    // 
    // 			double  dBeta2f_1 = 10.5*Eps; 
    // 			double dBeta3f_1  = 15.7*Eps; 
    // 
    // 			//------------------
    // 			double iClassF_2=0.0;
    // 			double BTR2=Calc_BTR_HC2();
    // 			if(BTR2 <= 29.3*Eps)	  	iClassF_2 = 1;
    // 			else if(BTR2 <= 33.5*Eps) iClassF_2 = 2;
    // 			else if(BTR2 <= 42.0*Eps)	iClassF_2 = 3;
    // 			else							        iClassF_2 = 4;
    // 
    // 			double dBeta2f_2 = 33.5*Eps;
    // 			double dBeta3f_2 = 42.0*Eps; 
    // 
    // 			//Get Worst of Both
    // 			dBTR=max(BTR1,BTR2);
    // 			if (dBTR==BTR1)
    // 			{
    // 				iClassF=iClassF_1;
    // 				dBeta2f=dBeta2f_1;
    // 				dBeta3f=dBeta3f_1;
    // 				iFlClass=0;
    // 			}
    // 			else
    // 			{
    // 				iClassF=iClassF_2;
    // 				dBeta2f=dBeta2f_2;
    // 				dBeta3f=dBeta3f_2;
    // 				iFlClass=1;
    // 			}
    // 
    // 
    // 			//-----------------------------
    // 			double iClassF_3=0.0;
    // 			double BTR3=Calc_BTR_HC3();
    // 			if(BTR3 <= 8.4*Eps)		  	iClassF_3 = 1;
    // 			else if(BTR3 <= 9.4*Eps)  iClassF_3 = 2;
    // 			else if(BTR3 <= 13.6*Eps)	iClassF_3 = 3;
    // 			else							        iClassF_3 = 4;
    // 
    // 			double dBeta2f_3 = 9.4*Eps;
    // 			double dBeta3f_3 = 13.6*Eps;
    // 
    // 			//Get Worst of All
    // 			dBTR=max(dBTR,BTR3);
    // 			if (dBTR==BTR3)
    // 			{
    // 				iClassF=iClassF_3;
    // 				dBeta2f=dBeta2f_3;
    // 				dBeta3f=dBeta3f_3;
    // 				iFlClass=2;
    // 			}
    // 
    // 
    // 			//Print_IS800_07_016_1(iClassF, dBTR, Eps, m_dfyf,BTR1,BTR2,BTR3,iFlClass);//Change printing of report
    // 		}
    // 	

    // 2. Part subject to bending and compression
    int iStressType = 0;
    double dbc = 0.0, dbt = 0.0, dCalc_k_sig = 0.0;
    double dAlpha = 0.0, dPsi = 0.0, dk_sig = 0.0, dLamda_p = 0.0,dRho = 0.0, dbeff = 0.0, dx0 = 0.0;
    BOOL bTens = FALSE, bInternal = FALSE;
    // 
    double da = dc < m_dZero ? 0.0 : (dStressOuter - dStressInner) / dc;

    if(dStressOuter < m_dZero && dStressInner < m_dZero)
    {
        dAlpha	= 1;
        dbt		= 0.0;
        dbc		= dc;

        if(fabs(dStressOuter) > fabs(dStressInner))
        {
            iStressType = 1;
            dPsi = dStressInner/dStressOuter;
            dk_sig = Get_OutstandBucklingfactor(iStressType, dPsi);      
        }
        else if(fabs(dStressOuter) < fabs(dStressInner))
        {
            iStressType = 2;
            dPsi = dStressOuter/dStressInner;
            dk_sig = Get_OutstandBucklingfactor(iStressType,dPsi);    
        }
        else
        {
            iStressType	= 1;
            dPsi		= 1.0;
            dk_sig		= 0.43;
        }

        dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
        Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
        dbeff = dRho*dc;
    }
    else if(dStressOuter * dStressInner < 0.0)
    {
        dx0 = fabs(da) < m_dZero ? 0.0 : -dStressInner / da;
        ASSERT(dx0 > m_dZero);

        if(dStressOuter < 0.0)
        {
            iStressType = 1;
            dPsi = dStressInner/dStressOuter;
            if(dPsi < -3.0){dPsi=-3.0;}
        }
        else 
        {
            iStressType = 2;
            dPsi = dStressOuter/dStressInner;
            if(dPsi < -1.0){dPsi=-1.0;}
        }

        dk_sig	= Get_OutstandBucklingfactor(iStressType,dPsi);
        dbc		= dStressInner < -cDGN_Zero ? dx0 : dc- dx0;
        dbt		= dc - dbc;
        dAlpha	= fabs(dbt) < m_dZero ? 0.0 : dbc/dbt;

        dLamda_p	= Get_Lamda_p(dc, dt, dEpsi, dk_sig);
        Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
        dbeff		= dRho*dc/(1.0-dPsi);
    }
    else if(dStressOuter > 0.0 && dStressInner > 0.0)
    {
        bTens	= TRUE;
        dPsi	= 0.0;
        dk_sig	= 0.0;
        dbt		= dc;
        dbc		= 0.0;
        dAlpha	= fabs(dbt) < m_dZero ? 0.0 : dbc/dbt;
        dLamda_p = Get_Lamda_p(dc, dt, dEpsi, dk_sig);
        Get_ReductionFactorRho(bInternal, dc, dt, dLamda_p, dPsi, dRho);
        dbeff = dRho*dc/(1.0-dPsi);
    }
    else 
    {ASSERT(0);}
    iFlClass = max(iFlClass,iClassF);
    if(!bTens){iClass_NM = Get_PartClass(iStressType, dBTR, dEpsi, dAlpha, dk_sig);}		  
    else      {iClass_NM = 1;}		  

    int iclassF,iclassW;
    Calc_Class_General(iclassF,iclassW,dfy_r,dBTR);

    ClassBase.iClass = iclassF;
    ClassBase.dPsi	 = dPsi;  
    ClassBase.dk_sig = dk_sig;
    ClassBase.dRho	 = dRho;
    ClassBase.dbeff	 = dbeff;
    ClassBase.dBTR	 = dBTR;
    ClassBase.dEpsilon = dEpsi;
    iClass = iclassF;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_ClassifyClassSection4GenSect(BOOL bChkRestrained, double dfy1, double dfy2, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD,
                                                            const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& DLineCalcD)
{
    T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
    aLine.Copy(GLineData.aLine);
    int iGLineSize = aLine.GetSize();
    if(iGLineSize<=0) return FALSE;

    CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&> aPartLine;
    aPartLine.Copy(GLineDgn.aPartLine);
    int iPartSize = aPartLine.GetSize();
    DLineCalcD.aPartLine.SetSize(iPartSize);

    int i=0;
    int iSectClass=0;
    int iWebClass=0;
    int iFlangeClass=0;
    int iFlangeTopClass=0;
    int iFlangeBotClass=0;
    int iWebCount=0, iFlangeCount=0;
    int iFlgTopCount=0, iFlgBotCount=0;

    // 
    if(bChkRestrained)
    {
        for(i=0; i<iPartSize; i++)
        {    
            T_DLINE_PART_D DLinePartD;
            T_GLINE_PART_DGN PartDgnD = aPartLine[i];
            DLinePartD.nPartType = PartDgnD.nPartType;
            int iTempBotClass=1;
            Calc_PartLineClassSection4GenSect(dfy1, dfy2, iTempBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, PartDgnD, DLinePartD);
            DLineCalcD.aPartLine.SetAt(i, DLinePartD);
            int nPartType = PartDgnD.nPartType;
            int iPartClass=DLinePartD.iClass;
            if(nPartType==2)// web
            {      
                iWebCount++;
                if(iWebCount==1) iWebClass = iPartClass;
                else
                {
                    if(iWebClass<iPartClass) iWebClass = iPartClass;
                }      
            }
            else if(nPartType==0)
            {
                iFlgTopCount++;
                if(iFlgTopCount==1) iFlangeTopClass = iPartClass;
                else
                {
                    if(iFlangeTopClass<iPartClass) iFlangeTopClass = iPartClass;
                }            
            }
            else if(nPartType==1)
            {
                iFlgBotCount++;
                if(iFlgBotCount==1) iFlangeBotClass = iPartClass;
                else
                {
                    if(iFlangeBotClass<iPartClass) iFlangeBotClass = iPartClass;
                }            
            }
        }
        if(bChkRestrained) iFlangeTopClass = 1; 
    }
    else
    {
        // Bottom
        for(i=0; i<iPartSize; i++)
        {    
            T_DLINE_PART_D DLinePartD;
            T_GLINE_PART_DGN PartDgnD = aPartLine[i];
            int nPartType = PartDgnD.nPartType;
            DLinePartD.nPartType = nPartType;            
            if(nPartType!=1) continue;
            int iTempBotClass=1;
            Calc_PartLineClassSection4GenSect(dfy1, dfy2, iTempBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, PartDgnD, DLinePartD);
            DLineCalcD.aPartLine.SetAt(i, DLinePartD);
            int iPartClass=DLinePartD.iClass;
            iFlgBotCount++;
            if(iFlgBotCount==1) iFlangeBotClass = iPartClass;
            else
            {
                if(iFlangeBotClass<iPartClass) iFlangeBotClass = iPartClass;
            }      
        }
        // Web
        for(i=0; i<iPartSize; i++)
        {    
            T_DLINE_PART_D DLinePartD;
            T_GLINE_PART_DGN PartDgnD = aPartLine[i];
            int nPartType = PartDgnD.nPartType;
            DLinePartD.nPartType = nPartType;            
            if(nPartType!=2) continue;
            Calc_PartLineClassSection4GenSect(dfy1, dfy2, iFlangeBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, PartDgnD, DLinePartD);
            DLineCalcD.aPartLine.SetAt(i, DLinePartD);
            int iPartClass=DLinePartD.iClass;
            iWebCount++;
            if(iWebCount==1) iWebClass = iPartClass;
            else
            {
                if(iWebClass<iPartClass) iWebClass = iPartClass;
            }      
        }
        // Top
        for(i=0; i<iPartSize; i++)
        {    
            T_DLINE_PART_D DLinePartD;
            T_GLINE_PART_DGN PartDgnD = aPartLine[i];
            int nPartType = PartDgnD.nPartType;
            DLinePartD.nPartType = nPartType;            
            if(nPartType!=0) continue;
            Calc_PartLineClassSection4GenSect(dfy1, dfy2, iFlangeBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, PartDgnD, DLinePartD);
            DLineCalcD.aPartLine.SetAt(i, DLinePartD);
            int iPartClass=DLinePartD.iClass;
            iFlgTopCount++;
            if(iFlgTopCount==1) iFlangeTopClass = iPartClass;
            else
            {
                if(iFlangeTopClass<iPartClass) iFlangeTopClass = iPartClass;
            }            
        }

    }

    //
    iFlangeClass = max(iFlangeTopClass, iFlangeBotClass);
    iSectClass = max(iWebClass, iFlangeClass);

    //
    DLineCalcD.iClass = iSectClass;
    DLineCalcD.iClassFlg =iFlangeClass;
    DLineCalcD.iClassWeb = iWebClass;
    DLineCalcD.iClassFlgTop = iFlangeTopClass;
    DLineCalcD.iClassFlgBot = iFlangeBotClass;  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_PartLineClassSection4GenSect(double dfy1, double dfy2, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD,
                                                            T_DLINE_PART_D& DLinePartD)
{
    int i=0;

    double dFx=dForce[0];
    double dMy=dForce[4];
    double dMz=dForce[5];

    int nPartType = PartDgnD.nPartType;
    int iClass=0;
    // Outstand  
    int iOutstandSize = PartDgnD.aOutstand.GetSize();  
    int iOutstandClass=0;
    if(iOutstandSize>0) DLinePartD.aOutstand.SetSize(iOutstandSize);
    for(i=0; i<iOutstandSize; i++)
    {
        T_DGNLINE_LBD DLineLbd;
        T_GLINE_DST LineDstD = PartDgnD.aOutstand[i];
        int iLineClass=0;
        Calc_SubLineClassSection4GenSect(nPartType, dfy1, dfy2, iBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, LineDstD, DLineLbd, iLineClass);

        if(i==0) iOutstandClass = iLineClass;
        else     iOutstandClass = max(iLineClass,iOutstandClass);
        DLineLbd.iClass = iLineClass;
        DLinePartD.aOutstand.SetAt(i, DLineLbd);
    }

    // Internal
    int iInternalSize = PartDgnD.aInternal.GetSize();  
    int iInternalClass=0;
    if(iInternalSize>0) DLinePartD.aInternal.SetSize(iInternalSize);
    for(i=0; i<iInternalSize; i++)
    {
        T_DGNLINE_LBD DLineLbd;
        T_GLINE_DST LineDstD = PartDgnD.aInternal[i];
        int iLineClass=0;
        Calc_SubLineClassSection4GenSect(nPartType, dfy1, dfy2, iBotClass, aGirderForce, dForce, dPlasticNA, GirStiffD, CptStiffD, OrgStiffD, GLineData, LineDstD, DLineLbd, iLineClass);

        if(i==0) iInternalClass = iLineClass;
        else     iInternalClass = max(iLineClass,iInternalClass);
        DLineLbd.iClass = iLineClass;
        DLinePartD.aInternal.SetAt(i, DLineLbd);
    } 

    int iClassOfSect = max(iOutstandClass, iInternalClass);

    DLinePartD.iClassOut=iOutstandClass;
    DLinePartD.iClassIn =iInternalClass;
    DLinePartD.iClass = iClassOfSect;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_SubLineClassSection4GenSect(int nPartType, double dfy1, double dfy2, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_DGNLINE_LBD& DLineLbd, int& iClass)
{
    iClass=0;
    int i=0;
    double dFx_a=aGirderForce[0];
    double dMy_a=aGirderForce[4];
    double dMz_a=aGirderForce[5];
    //
    double dFx=dForce[0];
    double dMy=dForce[4];
    double dMz=dForce[5];
    BOOL bPositive = dMy>=0.0;
    //
    double dArea_a = GirStiffD.Area;
    double dIy_a   = GirStiffD.Ryy;
    double dIz_a   = GirStiffD.Rzz;
    double dArea = CptStiffD.Area;
    double dIy   = CptStiffD.Ryy;
    double dIz   = CptStiffD.Rzz;

    BOOL bInternal = LineDst.nType==0 ? TRUE : FALSE;
    BOOL bIs1_Sig1=TRUE;
    BOOL bSig1IsFree=TRUE;  

    int iLineDgnSize = LineDst.aLineDgn.GetSize();
    //LineLBD.aDgnBaseD.SetSize(iLineDgnSize);

    double db_t = LineDst.db;
    double dt_t = LineDst.dt;  
    double dfy = Get_fy4Thick(dfy1, dfy2, dt_t);
    double dEpsilon = Get_Epsilon(dfy);

    double dCzm_gap_ao = OrgStiffD.Czm - GirStiffD.Czm; // org와 거더만 있을 때의 차이 
    double dCzm_gap_co = OrgStiffD.Czm - CptStiffD.Czm; // org와 합성 과의 차이 

    //Pinakin added for Section classification r1,r2
    m_dPu = dFx;
    m_dMuy=dMy;
    m_dMuz = dMz;
    m_dfy = dfy;
    m_dZbar = OrgStiffD.Czm;
    m_dYbar =  OrgStiffD.Cym;
    //End pinakin
    // Plastic N.A와 차이. 
    double dCzm_gap_pl = OrgStiffD.Czm - dPlasticNA;

    double dStrEnd1=0.0, dStrEnd2=0.0;  
    double dLen_c=0.0, dLen_t=0.0;

    DLineLbd.aDgnBaseD.SetSize(iLineDgnSize);
    for(i=0; i<iLineDgnSize; i++)
    {
        T_GLINE_DGN_B LineDgnB;
        T_GLINE_BASE LineBaseD = LineDst.aLineDgn[i];    
        double dy1_a = LineBaseD.dy1; // Panel의 양끝.
        double dz1_a = LineBaseD.dz1 + dCzm_gap_ao;
        double dy2_a = LineBaseD.dy2;
        double dz2_a = LineBaseD.dz2 + dCzm_gap_ao;
        //
        double dy1 = LineBaseD.dy1; // Panel의 양끝.
        double dz1 = LineBaseD.dz1 + dCzm_gap_co;
        double dy2 = LineBaseD.dy2;
        double dz2 = LineBaseD.dz2 + dCzm_gap_co;
        //
        double dy1_p = LineBaseD.dy1; // Panel의 양끝.
        double dz1_p = LineBaseD.dz1 + dCzm_gap_pl;
        double dy2_p = LineBaseD.dy2;
        double dz2_p = LineBaseD.dz2 + dCzm_gap_pl;
        //
        double dbs = LineBaseD.db;
        double dts = LineBaseD.dt;
        //
        double dStr1_a = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy1_a, dz1_a);
        double dStr2_a = Calc_StressOfPosition(dFx_a, dMy_a, dMz_a, dArea_a, dIy_a, dIz_a, dy2_a, dz2_a);
        double dStr1_c = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy1, dz1);
        double dStr2_c = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy2, dz2);
        double dStr1 = dStr1_a + dStr1_c;
        double dStr2 = dStr2_a + dStr2_c;

        LineDgnB.db = LineBaseD.db;  
        LineDgnB.dt = LineBaseD.dt;          // thickness,
        LineDgnB.dy1 = dy1;
        LineDgnB.dz1 = dz1;     // 양끝단의 도심에서 떨어진 거리.
        LineDgnB.dy2 = dy2;
        LineDgnB.dz2 = dz2;
        LineDgnB.dStr1 = dStr1;
        LineDgnB.dStr2 = dStr2;

        if(i==0) 
        {
            dt_t = LineBaseD.dt;
            dStrEnd1 = dStr1;
            dStrEnd2 = dStr2;       
        }
        else if(i==iLineDgnSize-1)
        {
            dt_t = min(dt_t, LineBaseD.dt);
            dStrEnd2 = dStr2;
        }
        else
        {
            dt_t = min(dt_t, LineBaseD.dt);
        }

        double dLc=0.0, dLt=0.0;
        Get_CompLengthOfPlastic(bPositive, dy1_p, dz1_p, dy2_p, dz2_p, dLc, dLt);
        dLen_c += dLc;
        dLen_t += dLt;    

        DLineLbd.aDgnBaseD.SetAt(i,LineDgnB);
    }

    BOOL bPlasticCompStr=dLen_c>0.0;
    BOOL bCompStr = Is_CompStress(dStrEnd1, dStrEnd2);
    BOOL bDbCurve = Is_DoubleCurvature(dStrEnd1, dStrEnd2);     
    double dct = dt_t==0.0 ? 0.0 : db_t/dt_t;

    double df1=0.0, df2=0.0;
    double dbc=0.0, dbt=0.0;
    double dAlpha=0.0;
    double dPsi=0.0;
    double dk_sigma=0.0;
    double dAlpha_p = db_t==0.0 ? 0.0 : dLen_c/db_t;  

    int iClass_p = 0.0;
    BOOL bChkPlastic=TRUE;
    // 무조건 Bending and Compression 으로.. 합성보에서는 대부분이 Plastic, Elastic Axis에서 상하대칭이 아니기 때문. 
    int iInternalStrType=2; 
    int iOutstandStrType=0;
    // class 1, class 2 검토 
    if(dLen_c>0.0)
    {
        if(bPositive)
        {
            if(bInternal)
            {
                bChkPlastic=TRUE;
                iClass_p = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha_p, dPsi);
            }
            else
            {
                double dk_sigma=0.0;
                iOutstandStrType=1;

                dk_sigma = Get_OutstandBucklingfactor(Outstand_Stress_Type1,dPsi);
                iClass_p = Get_PartClass(iOutstandStrType, dct, dEpsilon, dAlpha_p, dk_sigma);
            }
        }
        else 
        {
            if(iBotClass<3)
            {
                if(bInternal)
                {
                    bChkPlastic=TRUE;
                    iClass_p = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha_p, dPsi);
                }
                else
                {
                    double dk_sigma=0.0;
                    iOutstandStrType=1;

                    dk_sigma = Get_OutstandBucklingfactor(Outstand_Stress_Type1,dPsi);
                    iClass_p = Get_PartClass(iOutstandStrType, dct, dEpsilon, dAlpha_p, dk_sigma);
                }
            }
        }
    }
    else
    {
        iClass_p = 1; // 인장. 
    }

    if(bCompStr)
    {     
        //
        Get_f1_f2(dStrEnd1, dStrEnd2, df1, df2, bIs1_Sig1);
        dPsi = Get_Psi(df1, df2);

        //
        if(!bDbCurve) 
        {
            dAlpha = 1.0; // 
        }
        else
        {
            if(bIs1_Sig1) Get_b1b2DbCurvature(db_t, df1, df2, dbc, dbt);
            else          Get_b1b2DbCurvature(db_t, df2, df1, dbc, dbt);
            dAlpha = db_t==0.0 ? 0.0 : dbc/db_t;
        }    

        if(bInternal)
        {       
            if(!bPositive && nPartType==2 && iBotClass>2)
            {
                // 부모멘트 이면서 Bottom class가 3이상이면 class 3부터 검토한다. 
                bChkPlastic=FALSE;
                iClass = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha, dPsi);
            }
            else
            {
                if(iClass_p>2) 
                {
                    bChkPlastic=FALSE;
                    iClass = Get_InternalPartClass(bChkPlastic, iInternalStrType, dct, dEpsilon, dAlpha, dPsi);
                }
                else 
                    iClass = iClass_p;
            }
        }
        else
        {
            double dk_sigma=0.0;
            if(!bDbCurve) iOutstandStrType=3;
            else 
            {
                if(LineDst.b1isFixed)
                {
                    if(bIs1_Sig1) iOutstandStrType=1;
                    else          iOutstandStrType=2;
                }
                else 
                {
                    if(bIs1_Sig1) iOutstandStrType=2;
                    else          iOutstandStrType=1;
                }        
            }

            bSig1IsFree = Is_Sig1IsFree4Outstand(bIs1_Sig1);
            int iStressType = Get_StressDistributionType4Outstand(bSig1IsFree, df1, df2);
            if(iStressType==Outstand_Stress_Type1 || iStressType==Outstand_Stress_Type3)
                iOutstandStrType = 3;
            else if(iStressType==Outstand_Stress_Type2 || iStressType==Outstand_Stress_Type4)
                iOutstandStrType = 2;        

            dk_sigma = Get_OutstandBucklingfactor(iStressType,dPsi);
            iClass = Get_PartClass(iOutstandStrType, dct, dEpsilon, dAlpha, dk_sigma);
        }
    }
    else
    {
        iClass = 1;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Is_Sig1IsFree4Outstand(BOOL bIs1_Sig1)
{
    return bIs1_Sig1 ? FALSE : TRUE; // Outstand일 경우 0번째에 자유단이 아닌 다른 Line과 연결된 vertex가 있다. 
}

double CDgnPlateGirder_IRC::Get_InternalBucklingfactor(double dPsi)
{  
    BOOL bPsi_Limit=FALSE;
    if(dPsi<-3.0) 
    {
        bPsi_Limit=TRUE;
        dPsi = -3.0;
    }

    double dk_sig=0.0;

    if(1.0-m_dZero < dPsi && dPsi < 1.0+m_dZero)			{dk_sig = 4.0;							}
    else if(0.0 < dPsi && dPsi < 1.0)						{dk_sig = 8.2/(1.05+dPsi);				}
    else if(fabs(dPsi) < m_dZero)							{dk_sig = 7.81;							}
    else if(-1.0<dPsi && dPsi<0.0)						{dk_sig = 7.81-6.29*dPsi+9.78*dPsi*dPsi;}
    else if(-1.0-m_dZero<dPsi && dPsi<-1.0+m_dZero)		{dk_sig = 23.9;							}
    else if(bPsi_Limit || (-3.0<=dPsi && dPsi<-1.0))		{dk_sig = 5.98*(1.0-dPsi)*(1.0-dPsi);	}

    return dk_sig;
}

double CDgnPlateGirder_IRC::Get_OutstandBucklingfactor(int iType, double dPsi)
{
    double dk_sig=0.0;
    if(iType==1)
    {
        if(1.0-m_dZero<dPsi && dPsi<1.0+m_dZero)				{dk_sig = 0.43;							}
        else if(-1.0-m_dZero<dPsi && dPsi<-1.0+m_dZero)			{dk_sig = 0.85;						}
        else													{dk_sig = 0.57-0.21*dPsi+0.07*dPsi*dPsi;}
    }
    else if(iType==2)
    {
        if(1.0-m_dZero<dPsi && dPsi<1.0+m_dZero)			{dk_sig = 0.43;						  }
        else if(0.0<dPsi && dPsi<1.0)						{dk_sig = 0.578/(dPsi+0.34);		  }
        else if(dPsi==0.0)									{dk_sig = 1.7;						  }
        else if(-1.0<dPsi && dPsi<0.0)						{dk_sig = 1.7-5.0*dPsi+17.1*dPsi*dPsi;} // Modify, Jaeoh. [7/12/2010] 0.5 -> 5.0
        else if(-1.0-m_dZero<dPsi && dPsi<-1.0+m_dZero)     {dk_sig = 23.8;						  }
        else
            ASSERT(0);
    }
    //else 
    //  ASSERT(0);

    return dk_sig;
}

int CDgnPlateGirder_IRC::Get_WebPlaticClass(double dct, double Eps, double dAlpha)
{
    int iClass = 0;
    double dLimit1 = 0.0, dLimit2 = 0.0;
    double dMaxValue = pow(1.0/m_dZero,5);
    double dr1,dr2;
    Calc_r1r2(dr1,dr2);
    double dHTR = Calc_HTR();
    if(dr1 < 0.0) // if r1 is negative
    {
        if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)         iClass= 1;
        else if(dHTR <= (105.*Eps/(1+dr1)) && dHTR <= 42.*Eps)   iClass= 2;
        else if(dHTR <= (126.*Eps/(1+2*dr2)) && dHTR <= 42.*Eps) iClass= 3;
        else                                                     iClass= 4;

    }
    else  // if r1 is positive
    {
        if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)           iClass = 1;
        else if(dHTR <= (105.*Eps/(1+1.5*dr1)) && dHTR <= 42.*Eps) iClass = 2;
        else if(dHTR <= (126.*Eps/(1+2.0*dr2)) && dHTR <= 42.*Eps) iClass = 3;
        else                                                       iClass = 4;



    }    

    return iClass;
}

int CDgnPlateGirder_IRC::Get_WebElasticClass(double dAlpha, double dct, double Eps, double dPsi)
{
    int iClass = 0;

    double dLimit1 = 0.0, dLimit2 = 0.0;
    double dMaxValue = pow(1.0/m_dZero,5);
    double dr1,dr2;
    Calc_r1r2(dr1,dr2);
    double dHTR = Calc_HTR();
    if(dr1 < 0.0) // if r1 is negative
    {
        if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)         iClass= 1;
        else if(dHTR <= (105.*Eps/(1+dr1)) && dHTR <= 42.*Eps)   iClass= 2;
        else if(dHTR <= (126.*Eps/(1+2*dr2)) && dHTR <= 42.*Eps) iClass= 3;
        else                                                     iClass= 4;

    }
    else  // if r1 is positive
    {
        if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)           iClass = 1;
        else if(dHTR <= (105.*Eps/(1+1.5*dr1)) && dHTR <= 42.*Eps) iClass = 2;
        else if(dHTR <= (126.*Eps/(1+2.0*dr2)) && dHTR <= 42.*Eps) iClass = 3;
        else                                                       iClass = 4;



    }    

    return iClass;
}

int CDgnPlateGirder_IRC::Get_PartClass(int iStreType, double dct, double dEpsi, double dAlpha, double dk)
{
    int iClass=1;
    double dLimit1 = 0.0;
    double dLimit2 = 0.0;
    double dLimit3 = 0.0;
    /*  BOOL bCompression=FALSE;*/

    //   if(dAlpha < m_dZero){ASSERT(0);}
    //   if(dk<0.0){ASSERT(0);}
    // 
    //   if(1.0-m_dZero<dAlpha && dAlpha<1.0+m_dZero){bCompression = TRUE;}

    if(iStreType==1)
    {
        dLimit1 = 8.4*dEpsi;
        dLimit2 = 9.4*dEpsi;
        dLimit3 =  13.6*dEpsi ;

        if(dct<dLimit1)     {iClass = 1;}
        else if(dct<dLimit2){iClass = 2;}
        else if(dct<dLimit3){iClass = 3;}
        else				{iClass = 4;}
    }
    else if(iStreType==2)
    {
        dLimit1 = 9.4*dEpsi;
        dLimit2 = 10.5*dEpsi;
        dLimit3 =  15.7*dEpsi ;

        if(dct<dLimit1)     {iClass = 1;}
        else if(dct<dLimit2){iClass = 2;}
        else if(dct<dLimit3){iClass = 3;}
        else				{iClass = 4;}
    }
    else if(iStreType==3) // 주의: Compression, Table 5.2의 첫번째. 
    {
        dLimit1 =  9.4*dEpsi;
        dLimit2 =  10.5*dEpsi;
        dLimit3 =   15.7*dEpsi ;

        if(dct<dLimit1)     {iClass = 1;}
        else if(dct<dLimit2){iClass = 2;}
        else if(dct<dLimit3){iClass = 3;}
        else				{iClass = 4;}
    }
    else 
        ASSERT(0);

    return iClass;
}

int CDgnPlateGirder_IRC::Get_InternalPartClass(BOOL bChkPlastic, int iStreType, double dct, double dEpsi, double dAlpha, double dPsi)
{
    int iClass=0;
    //For IRC cl. IRC24 2010 503.7.2
    //r1->dAlpha
    //r2->dPsi
    //In IRC ct is HTR..chek if wrong
    double dr1 = 0.0, dr2 = 0.0;
    Calc_r1r2(dr1,dr2);
    double dLim_bend1 = 84.0*dEpsi;
    double dLim_bend2 = 105.0*dEpsi;
    double dLim_bend3 = 126.0*dEpsi;
    double dLim_comp1 = 39.3*dEpsi;
    double dLim_comp2 = 33.5*dEpsi;
    double dLim_comp3 = 42.0*dEpsi;
    double dLim_bendcomp11 = 84.0*dEpsi/(dr1+1.0);
    double dLim_bendcomp12 = 84.0*dEpsi/(dr1+1.0);
    double dLim_bendcomp21 = 105.0*dEpsi/(dr1+1.0);
    double dLim_bendcomp22 = 105.0*dEpsi/((1.5*dr1)+1.0);
    double dLim_bendcomp31 = 126.0*dEpsi/(1.0+2.0*dr2);
    double dLim_bendcomp32 = 126.0*dEpsi/(1.0+2.0*dr2);

    // Part subject to bending
    if(iStreType==0)
    {
        if(dct<dLim_bend1 || fabs(dct-dLim_bend1)<m_dZero)		{iClass = 1;}
        else if(dct<dLim_bend2 || fabs(dct-dLim_bend2)<m_dZero)	{iClass = 2;}
        else if(dct<dLim_bend3 || fabs(dct-dLim_bend3)<m_dZero)	{iClass = 3;}
        else														{iClass = 4;}
    }
    // Part subject to compression
    else if(iStreType==1)
    {
        if(dct<dLim_comp1 || fabs(dct-dLim_comp1)<m_dZero)		{iClass = 1;}
        else if(dct<dLim_comp2 || fabs(dct-dLim_comp2)<m_dZero)	{iClass = 2;}
        else if(dct<dLim_comp3 || fabs(dct-dLim_comp3)<m_dZero)	{iClass = 3;}
        else														{iClass = 4;}
    }
    // Part subject to bending and compression
    else 
    {
        if(bChkPlastic)
        {
            if(dAlpha>0.5)
            {
                if(dct<dLim_bendcomp11 || fabs(dct-dLim_bendcomp11)<m_dZero)
                {
                    iClass = 1;
                }
                else if(dct<dLim_bendcomp12 || fabs(dct-dLim_bendcomp12)<m_dZero)
                {
                    iClass = 2;
                }
                else {iClass = 3;}
            }
            else 
            {
                if(dct<dLim_bendcomp21 || fabs(dct-dLim_bendcomp21)<m_dZero)
                {
                    iClass = 1;
                }
                else if(dct<dLim_bendcomp22 || fabs(dct-dLim_bendcomp22)<m_dZero)
                {
                    iClass = 2;
                }
                else {iClass = 3;}
            }
        }
        else
        {
            if(dPsi>-1.0)
            {
                if(dct<dLim_bendcomp31 || fabs(dct-dLim_bendcomp31)<m_dZero)
                {iClass = 3;}
                else
                {iClass = 4;}
            }
            else
            {
                if(dct<dLim_bendcomp32 || fabs(dct-dLim_bendcomp32)<m_dZero)
                {iClass = 3;}
                else
                {iClass = 4;}
            }
        }

    }

    return iClass;
}

BOOL CDgnPlateGirder_IRC::Get_ReductionFactorRho(BOOL bInternal, double dBar_b, double dt, double dLamda_p, double dPsi, double& dRho)
{
    dRho = 1.0;

    if(bInternal)
    {
        if(dLamda_p < 0.673 + m_dZero)
        {
            dRho = 1.0;
        }
        else
        {
            double dVal = 3.0+dPsi;

            if(dVal < 0.0) {dVal = 0.0;}

            dRho = min(1.0, (dLamda_p-0.055*dVal)/pow(dLamda_p,2));
        }
    }
    else
    {
        if(dLamda_p < 0.748 + m_dZero)
        {
            dRho = 1.0;
        }
        else
        {
            dRho = min(1.0, (dLamda_p-0.188)/pow(dLamda_p,2));
        }
    }

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_Lamda_p(double db, double dt, double dEpsilon, double dk_sig)
{
    if(dt < m_dZero) {return 0.0;}
    double dLamda_param = 28.4*dEpsilon*sqrt(dk_sig);
    double dbtr = db/dt;
    double dLamda = fabs(dLamda_param) < m_dZero ? 0.0 : dbtr / dLamda_param;

    return dLamda;
}

double CDgnPlateGirder_IRC::Get_Epsilon(double dfy)
{
    if(dfy<=0.0)
    {
        ASSERT(0);
        return 0;
    }
    else
    {
        return sqrt(250./dfy);
    }
}


BOOL CDgnPlateGirder_IRC::Get_EffectiveSectData(BOOL bPosiI, BOOL bAfter, double dfy1, double dfy2, double da, double* dForce, CPG_SECT_ELEM& SectElem, const T_GENL_DATA& GLineData, T_DLINE_CALC_D& DLineInfo, T_GENL_DATA& EffGLineD, T_GLINE_DGN_D& EffGLineDgn)
{
    double dFx = dForce[0];
    double dMy = dForce[4];
    double dMz = dForce[5];

    T_SECT_D SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    int nSectType = m_pDgnSectUtil->Get_RealSectType(&SectD);
    int nPosId = bPosiI ? 0 : 1;
    T_SECT_STIFFNESS Stiffness;    
    if(!bAfter)
    {
        Get_GirderStiffnessofCompo(bPosiI, SectElem.SectD, Stiffness);     
    }
    else
    {    
        if(dMy>=0.0) Stiffness = SectElem.SectPosi[nPosId].RebarSect;
        else         Stiffness = SectElem.SectPosi[nPosId].NonConSect;
    }
    double dCzm_org = SectElem.SectPosi[nPosId].PureSect.Czm;
    double dCzm_gap = dCzm_org - Stiffness.Czm;

    double dArea = Stiffness.Area;
    double dIy   = Stiffness.Ryy;
    double dIz   = Stiffness.Rzz;
    double aStiff[3] = {dArea, dIy, dIz};

    T_GLINE_DGN_D GLineDgn = bPosiI ? SectElem.GLineDgn[0] : SectElem.GLineDgn[1];

    Get_CalcSectInfo4EffeSect(dfy1, dfy2, da, dForce, dCzm_gap, aStiff, GLineData, GLineDgn, DLineInfo);

    m_aEffLineIx.RemoveAll();
    CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&> aPartLine;
    aPartLine.Copy(GLineDgn.aPartLine);
    int iPartSize = aPartLine.GetSize();
    EffGLineDgn.aPartLine.SetSize(iPartSize);
    int i=0, j=0, k=0;
    for(i=0; i<iPartSize; i++)
    {
        T_GLINE_PART_DGN PartDgnD = aPartLine[i];
        T_DLINE_PART_D DlinePartD = DLineInfo.aPartLine[i];
        if     (PartDgnD.nPartType==0) DlinePartD.iClass = DLineInfo.iClassFlgTop;
        else if(PartDgnD.nPartType==1) DlinePartD.iClass = DLineInfo.iClassFlgBot;
        else if(PartDgnD.nPartType==2) DlinePartD.iClass = DLineInfo.iClassWeb;      

        Get_EffectiveSectInfo4PartLine(dForce, dCzm_gap, aStiff, GLineData, PartDgnD, DlinePartD, EffGLineD);    
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveSectInfo4PartLine(double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, const T_DLINE_PART_D& DLinePart, T_GENL_DATA& EffGLineD)
{
    int i=0;

    // Outstand  
    int iOutstandSize = PartDgnD.aOutstand.GetSize();
    for(i=0; i<iOutstandSize; i++)
    {
        T_GLINE_DST GLineDST = PartDgnD.aOutstand[i];
        T_DGNLINE_LBD LineLBD = DLinePart.aOutstand[i];
        Get_EffectiveSectInfo4PlateElements(DLinePart.iClass, aForce, dCzm_gap, dStiff, GLineData, GLineDST, LineLBD, EffGLineD);    
    }

    // Internal
    int iInternalSize = PartDgnD.aInternal.GetSize();  
    for(i=0; i<iInternalSize; i++)
    {
        T_GLINE_DST GLineDST = PartDgnD.aInternal[i];
        T_DGNLINE_LBD LineLBD = DLinePart.aInternal[i];
        Get_EffectiveSectInfo4PlateElements(DLinePart.iClass, aForce, dCzm_gap, dStiff, GLineData, GLineDST, LineLBD, EffGLineD);    
    }    

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveSectInfo4PlateElements(int iClass, double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, T_GLINE_DST& GLineDST, T_DGNLINE_LBD& LineLBD, T_GENL_DATA& EffGLineD)
{
    int iLineShape = GLineDST.nType; // 0=Internal, 1=Outstand
    BOOL bInternal = iLineShape==0 ? TRUE : FALSE;  

    int i=0,j=0;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
    int iPanelSize = LineLBD.aDgnBaseD.GetSize();
    for(i=0; i<iPanelSize; i++)
    {
        T_GLINE_BASE LineBaseD = GLineDST.aLineDgn[i];
        T_GLINE_DGN_B LineDgnB = LineLBD.aDgnBaseD[i];
        int iStressType = LineDgnB.iStressType;    

        Get_EffectivePanelInformation(iClass, bInternal, iStressType, aForce, dCzm_gap, dStiff, GLineData, LineBaseD, LineDgnB, EffGLineD);    
    }
    // Stiffener
    int nStfnSize = GLineDST.aStiffener.GetSize();
    int nLBDStfnSize = LineLBD.aDgnStfnD.GetSize();
    if(nStfnSize==nLBDStfnSize)
    {
        for(i=0; i<nStfnSize; i++)
        {
            T_GLINE_DGN_STFN LineStfnD = LineLBD.aDgnStfnD[i];
            int nStfnLineSize = LineStfnD.aStfnDgnBaseD.GetSize();
            for(j=0; j<nStfnLineSize; j++)
            {
                int nLid = GLineDST.aStiffener[i].aLineIx[j];
                T_GSEC_LINE StLine = GLineData.aLine[nLid];
                T_GLINE_DGN_B LineDgnB = LineLBD.aDgnStfnD[i].aStfnDgnBaseD[j];
                int iStressType = LineDgnB.iStressType;
                T_GLINE_BASE LineBaseD;
                LineBaseD.aLineIx.Add(GLineDST.aStiffener[i].aLineIx[j]);
                LineBaseD.db = LineDgnB.db;
                LineBaseD.dt = LineDgnB.dt;
                LineBaseD.dy1 = LineDgnB.dy1;
                LineBaseD.dz1 = LineDgnB.dz1;
                LineBaseD.dy2 = LineDgnB.dy2;
                LineBaseD.dz2 = LineDgnB.dz2;
                LineBaseD.iLineType = LineDgnB.iLineType;
                LineBaseD.ve1 = StLine.v1;
                LineBaseD.ve2 = StLine.v2;
                //LineBaseD.b1isFixed = 

                Get_EffectivePanelInformation(iClass, bInternal, iStressType, aForce, dCzm_gap, dStiff, GLineData, LineBaseD, LineDgnB, EffGLineD); 
            }
        }
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ConvertVertexId(int vs, int v1, int v2, int& vc1, int& vc2)
{
    if     (vs==v1) { vc1 = v1, vc2 = v2; }
    else if(vs==v2) { vc1 = v2, vc2 = v1; }
    else ASSERT(0);  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ConvertSectStiffenerD(const T_DGN_STL_STIFFENER_PART& InD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi)
{
    int nStiffNum = arLstiPosi.GetSize();

    if(nStiffNum > 0)
    {
        for(int nSize = 0; nSize < nStiffNum; ++nSize)
        {
            T_STIFF_SHAPE_D InD;
            InD.nType	= 0;
            InD.dSize[0]= arLstiPosi[nSize].dWidth;
            InD.dSize[1]= arLstiPosi[nSize].dt;
            m_pDgnSectUtil->Calc_Stiffener_Area(0, InD, arLstiPosi[nSize].dArea);
            m_pDgnSectUtil->Calc_Stiffener_Iyy(0, InD, arLstiPosi[nSize].dIyy, arLstiPosi[nSize].dCzp, arLstiPosi[nSize].dCzm);
            m_pDgnSectUtil->Calc_Stiffener_Izz(0, InD, arLstiPosi[nSize].dIzz, arLstiPosi[nSize].dCyp, arLstiPosi[nSize].dCym);
        }
        return TRUE;
    }

    nStiffNum = InD.arStiffD.GetSize();
    arLstiPosi.SetSize(nStiffNum);

    for(int i = 0; i < nStiffNum; ++i)
    {
        arLstiPosi[i].dArea	= InD.arStiffD[i].dArea;
        arLstiPosi[i].dIyy	= InD.arStiffD[i].dIyy;
        arLstiPosi[i].dCzp	= InD.arStiffD[i].dCzp;
        arLstiPosi[i].dCzm	= InD.arStiffD[i].dCzm;
        arLstiPosi[i].dIzz	= InD.arStiffD[i].dIzz;
        arLstiPosi[i].dCyp	= InD.arStiffD[i].dCyp;
        arLstiPosi[i].dCym	= InD.arStiffD[i].dCym;
        arLstiPosi[i].dz	= InD.arStiffD[i].dDist;
        arLstiPosi[i].dWidth= InD.arStiffD[i].dSize[0];
        arLstiPosi[i].dt	= InD.arStiffD[i].dSize[1];
    }

    return TRUE;
}

// LineBaseD 정보는 0번째 Index가 Sigma1이 데이터 이어야 한다. 
// 이 함수 들어오기전에 0번째 Index가 Sigma1이 아니면 LineBaseD를 Convert한 후 계산.
BOOL CDgnPlateGirder_IRC::Get_EffectivePanelInformation(int iClass, BOOL bInternal, int iStressType, double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, 
                                                        const T_GLINE_DGN_B& LineDgnB, T_GENL_DATA& EffLineD)
{  
    int i=0;
    int iLineSize = LineBaseD.aLineIx.GetSize();  

    double dFx=aForce[0];
    double dMy=aForce[4];
    double dMz=aForce[5];
    //
    double dArea = dStiff[0];
    double dIy   = dStiff[1];
    double dIz   = dStiff[2];

    int iEffPolyVtxSize = EffLineD.PolyD.aVertex.GetSize();
    int iEffLineSize = EffLineD.aLine.GetSize();
    int iNextLineIdx=iEffLineSize;
    int iNextVtxIdx=iEffPolyVtxSize;

    CArray<int, int&> aEffLineIx;

    T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
    aLine.Copy(GLineData.aLine);

    double dbe1 = LineDgnB.SubPanelD.db_sup_eff;
    double dbe2 = LineDgnB.SubPanelD.db_inf_eff;

    double db = LineDgnB.db;
    double dbc = LineDgnB.SubPanelD.dbc;
    double dbt = LineDgnB.SubPanelD.dbt;
    int SubStrType = LineDgnB.SubPanelD.nType;
    double dbe2StartLen = 0.0;
    if(SubStrType==2) // Double curvature.
    {
        dbe2StartLen = LineDgnB.SubPanelD.db_sup-dbe2;
    }
    else
    {
        dbe2StartLen = db-dbe2;
    }

    double dPreLen = 0.0, dCurLen = 0.0;  

    double dxe=0., dye=0.;
    double dxs=0., dys=0.; // Start점 
    int v1=0, v2=0;

    BOOL bCheckLine=FALSE;  
    T_GSEC_VERTEX VtxDiv;

    BOOL bIs1_Sig1 = LineDgnB.bIs1_sig1;
    int vs = bIs1_Sig1 ? LineBaseD.ve1 : LineBaseD.ve2;
    for(i=0; i<iLineSize; i++)
    {
        int iLIx = bInternal && !bIs1_Sig1 ? (iLineSize-1)-i : i;
        int iLineIx = LineBaseD.aLineIx[iLIx];
        T_GSEC_LINE LineD = aLine[iLineIx];    
        int v1 = LineD.v1;
        int v2 = LineD.v2;
        //////////////////////////////////////////////////////////////////////////
        int vc1=0, vc2=0;
        Get_ConvertVertexId(vs, v1, v2, vc1, vc2);
        vs = vc2;
        LineD.v1 = vc1, LineD.v2 = vc2;
        //////////////////////////////////////////////////////////////////////////    
        T_GSEC_VERTEX vtx1 = GsecPolyD.aVertex[vc1];
        T_GSEC_VERTEX vtx2 = GsecPolyD.aVertex[vc2];
        double dx1 = vtx1.dX;
        double dy1 = vtx1.dY;
        double dx2 = vtx2.dX;
        double dy2 = vtx2.dY;
        double dy1_s = vtx1.dY + dCzm_gap;
        double dy2_s = vtx2.dY + dCzm_gap;
        double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
        double dt = LineD.dThik;    
        double dStr1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx1, dy1_s);
        double dStr2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx2, dy2_s);
        BOOL bCompStr = Is_CompStress(dStr1, dStr2);
        BOOL bDbCurve = Is_DoubleCurvature(dStr1, dStr2);   
        if(i==0) { dxs=dx1, dys=dy1; }

        T_GSEC_VERTEX VtxDiv;
        dCurLen += dbi;    
        if(bInternal)
        {
            if(LineDgnB.bCompStr && iClass==4)
            {
                // be1에 대해 
                if(dCurLen<dbe1 || fabs(dCurLen-dbe1)<m_dZero) // 나누어지는 Line을 찾기 위해.
                {                    
                    aEffLineIx.Add(iLineIx); // 기존 데이터로 가능.           
                }
                else if(dCurLen>dbe1)
                {
                    if(dPreLen<dbe1)
                    {
                        double dlen = dbe1-dPreLen;
                        Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
                        aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
                        //
                        VtxDiv.Set(dxe, dye);
                        EffLineD.PolyD.aVertex.Add(VtxDiv);
                        T_GSEC_LINE ELine = LineD;
                        //ELine.v1 = v1;
                        ELine.v2 = iNextVtxIdx++;
                        EffLineD.aLine.Add(ELine);
                    }
                }
                // be2에 대해 
                if(dbe2StartLen<dCurLen)
                {
                    if(dPreLen<dbe2StartLen)
                    {
                        double dlen = dbe2StartLen-dPreLen;
                        Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
                        aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
                        VtxDiv.Set(dxe, dye);
                        EffLineD.PolyD.aVertex.Add(VtxDiv);
                        T_GSEC_LINE ELine = LineD;
                        ELine.v1 = iNextVtxIdx++;
                        //ELine.v2 = v2;
                        EffLineD.aLine.Add(ELine);
                    }
                    else 
                    {
                        aEffLineIx.Add(iLineIx);
                    }
                }
            }
            else // 인장.
            { 
                aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.      
            }      
        }
        else // Outstand
        {
            if(iClass==4)
            {
                // !!!한쪽이 지지된 곳이 LineBaseD.aLineIx의 0번째 Index      
                if(iStressType==0 || iStressType==1) // Outstand_Stress_Type3 와 같음.
                {
                    // be1에 대해 
                    if(dCurLen<dbe1 || fabs(dCurLen-dbe1)<m_dZero) // 나누어지는 Line을 찾기 위해.
                    {                    
                        aEffLineIx.Add(iLineIx); // 기존 데이터로 가능.           
                    }
                    else if(dCurLen>dbe1)
                    {
                        if(dPreLen<dbe1)
                        {
                            double dlen = dbe1-dPreLen;
                            Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
                            aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
                            //
                            VtxDiv.Set(dxe, dye);
                            EffLineD.PolyD.aVertex.Add(VtxDiv);
                            T_GSEC_LINE ELine = LineD;
                            //ELine.v1 = v1;
                            ELine.v2 = iNextVtxIdx++;
                            EffLineD.aLine.Add(ELine);
                        }
                    }
                }
                else if(iStressType==2) //Outstand_Stress_Type4 와 같음. 
                {
                    // beff에 대해 
                    if(dCurLen<dbe1 || fabs(dCurLen-dbe1)<m_dZero) // 나누어지는 Line을 찾기 위해.
                    {                    
                        aEffLineIx.Add(iLineIx); // 기존 데이터로 가능.           
                    }
                    else if(dCurLen>dbe1)
                    {
                        if(dPreLen<dbe1)
                        {
                            double dlen = dbe1-dPreLen;
                            Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
                            aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
                            //
                            VtxDiv.Set(dxe, dye);
                            EffLineD.PolyD.aVertex.Add(VtxDiv);
                            T_GSEC_LINE ELine = LineD;
                            //ELine.v1 = v1;
                            ELine.v2 = iNextVtxIdx++;
                            EffLineD.aLine.Add(ELine);
                        }
                    }
                    //
                    if(dCurLen>dbc)          
                    {
                        if(dPreLen<dbc)
                        {
                            double dlen = dbc-dPreLen;
                            Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
                            aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
                            VtxDiv.Set(dxe, dye);
                            EffLineD.PolyD.aVertex.Add(VtxDiv);
                            T_GSEC_LINE ELine = LineD;
                            ELine.v1 = iNextVtxIdx++;
                            //ELine.v2 = v2;
                            EffLineD.aLine.Add(ELine);
                        }
                        else 
                        {
                            aEffLineIx.Add(iLineIx);
                        }
                    }
                }
                else if(iStressType==3) //Outstand_Stress_Type2 와 같음. 
                {
                    double dEffLen=dbt+dbe1;
                    if(dCurLen<dEffLen || fabs(dCurLen-dEffLen)<m_dZero)
                    {
                        aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.
                    }
                    else if(dCurLen>dEffLen)
                    {
                        if(dPreLen<dEffLen)
                        {
                            double dlen = dEffLen-dPreLen;
                            Get_PointCoor4beff(dlen, dx1, dy1, dx2, dy2, dxe, dye);
                            aEffLineIx.Add(iNextLineIdx); iNextLineIdx++;
                            VtxDiv.Set(dxe, dye);
                            EffLineD.PolyD.aVertex.Add(VtxDiv);
                            T_GSEC_LINE ELine = LineD;
                            //ELine.v1 = v1;
                            ELine.v2 = iNextVtxIdx++;
                            EffLineD.aLine.Add(ELine);
                        }
                    }
                }
                else // 인장.
                {
                    aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.  
                }
            }
            else
                aEffLineIx.Add(iLineIx);  // 기존 데이터로 가능.  
        }
        dPreLen = dCurLen;
    }

    if(aEffLineIx.GetSize()>0) m_aEffLineIx.Append(aEffLineIx);
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveSect4PartLine(double dfy1, double dfy2, double* dForce, double dArea, double dIy, double dIz, const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, 
                                                     T_GENL_DATA& EffGLineD, T_GLINE_PART_DGN& EffPartDgnD)
{
    int i=0, k=0;

    EffPartDgnD = PartDgnD;
    // Outstand  
    int iOutstandSize = PartDgnD.aOutstand.GetSize();
    if(iOutstandSize>0) EffPartDgnD.aOutstand.SetSize(iOutstandSize);
    for(i=0; i<iOutstandSize; i++)
    {
        T_GLINE_DST EffLineDst;
        T_GLINE_DST LineDstD = PartDgnD.aOutstand[i];
        Get_EffectiveSection4StiffenedPlateElements(dfy1, dfy2, dForce, dArea, dIy, dIz, GLineData, LineDstD, EffGLineD, EffLineDst);
        EffPartDgnD.aOutstand.SetAt(i, EffLineDst);
    }

    // Internal
    int iInternalSize = PartDgnD.aInternal.GetSize();  
    if(iInternalSize>0) EffPartDgnD.aInternal.SetSize(iInternalSize);
    for(i=0; i<iInternalSize; i++)
    {
        T_GLINE_DST EffLineDst;
        T_GLINE_DST LineDstD = PartDgnD.aInternal[i];
        Get_EffectiveSection4StiffenedPlateElements(dfy1, dfy2, dForce, dArea, dIy, dIz, GLineData, LineDstD, EffGLineD, EffLineDst);
        EffPartDgnD.aInternal.SetAt(i, EffLineDst);
    }    

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveWidth4Internal(double dPsi, double db, double dRho, double& dbeff, double& dbe1, double& dbe2)
{
    if(fabs(dPsi-1.0)<m_dZero)
    {
        dbeff = dRho * db;
        dbe1 = dbe2 = 0.5*dbeff;
    }
    else if(-m_dZero < dPsi && dPsi<1.0)
    {
        dbeff = dRho * db;
        dbe1 = 2./(5.-dPsi)*dbeff;
        dbe2 = dbeff - dbe1;
    }
    else if(dPsi<0.0)
    {
        dbeff = dRho*db/(1.-dPsi);
        dbe1 = 0.4*dbeff;
        dbe2 = 0.6*dbeff;
    }
    else
    {
        dbeff = dbe1 = dbe2 = 0.0;
        return FALSE;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectiveWidth4Outstand(int nType, double dPsi, double dc, double dRho, double& dbeff)
{
    // nType : 1993-1-5, Table 4.2, 0=(첫버째,두번째) 1=(세번째,네버째)
    // 계산은 같기 때문에 굳이 나눌 필요는 없음. 
    if(nType==Outstand_Stress_Type1 || nType==Outstand_Stress_Type2)
    {
        if(-m_dZero < dPsi && (dPsi<1.0 || fabs(dPsi-1.0)<m_dZero))
        {
            dbeff = dRho * dc;
        }
        else if(dPsi<0.0)
        {
            dbeff = dRho*dc/(1.-dPsi);
        }
    }
    else if(nType==Outstand_Stress_Type3 || nType==Outstand_Stress_Type4)
    {
        if(-m_dZero < dPsi && (dPsi<1.0 || fabs(dPsi-1.0)<m_dZero))
        {
            dbeff = dRho * dc;
        }
        else if(dPsi<0.0)
        {
            dbeff = dRho*dc/(1.-dPsi);
        }
    }
    else
    {
        ASSERT(0);
        dbeff = 0.0;
        return FALSE;
    }
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_CalcSectInfo4EffeSect(double dfy1, double dfy2, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& DlineInfo)
{
    T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
    aLine.Copy(GLineData.aLine);
    int iGLineSize = aLine.GetSize();
    if(iGLineSize<=0) return FALSE;

    CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&> aPartLine;
    aPartLine.Copy(GLineDgn.aPartLine);
    int iPartSize = aPartLine.GetSize();
    DlineInfo.aPartLine.SetSize(iPartSize);
    int i=0;
    for(i=0; i<iPartSize; i++)
    {
        T_DLINE_PART_D DLinePart;
        T_GLINE_PART_DGN EffPartDgnD;
        T_GLINE_PART_DGN PartDgnD = aPartLine[i];
        Get_CalcSectPartLine4EffeSect(dfy1, dfy2, m_dEs, m_dPoisson, da, dForce, dCzm_gap, dStiff, GLineData, PartDgnD, DLinePart);
        DlineInfo.aPartLine.SetAt(i, DLinePart);
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_CalcSectPartLine4EffeSect(double dfy1, double dfy2, double dE, double dPoisson, double da, double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_DLINE_PART_D& DLinePart)
{
    int i=0, j=0, k=0;

    double db_in=0.0, dt_in=0.0;;
    double dt_out=0.0, dt_int=0.0;
    // Outstand  
    int iOutstandSize = PartDgnD.aOutstand.GetSize();  
    if(iOutstandSize>0) DLinePart.aOutstand.SetSize(iOutstandSize);
    DLinePart.nPartType = PartDgnD.nPartType;
    for(i=0; i<iOutstandSize; i++)
    {
        T_DGNLINE_LBD DLineLbd;
        T_GLINE_DST LineDstD = PartDgnD.aOutstand[i];
        db_in += LineDstD.db;
        double dti = LineDstD.dt;
        if(i==0) dt_out = dti;
        else     dt_out = min(dt_out, dti);
        Get_LocalBucklingDataOfSubPanel(dfy1, dfy2, dE, dPoisson, da, aForce, dCzm_gap, dStiff, GLineData, LineDstD, DLineLbd);
        DLinePart.aOutstand.SetAt(i, DLineLbd);    
    }

    // Internal
    int iInternalSize = PartDgnD.aInternal.GetSize();  
    if(iInternalSize>0) DLinePart.aInternal.SetSize(iInternalSize);
    for(i=0; i<iInternalSize; i++)
    {
        T_DGNLINE_LBD DLineLbd;
        T_GLINE_DST LineDstD = PartDgnD.aInternal[i];
        db_in += LineDstD.db;
        double dti = LineDstD.dt;
        if(i==0) dt_int = dti;
        else     dt_int = min(dt_int, dti);
        Get_LocalBucklingDataOfSubPanel(dfy1, dfy2, dE, dPoisson, da, aForce, dCzm_gap, dStiff, GLineData, LineDstD, DLineLbd);
        double dfyi=Get_fy4Thick(dfy1, dfy2, LineDstD.dt);

        int nOpenStfnSize=0;
        int nClosedStfnSize=0;
        int nStfnSizeOfInt = LineDstD.aStiffener.GetSize();
        for(j=0; j<nStfnSizeOfInt; j++)
        {
            // Stiffener를 계산하기 위해.
            int nStfnLineSize = LineDstD.aStiffener[j].aLineIx.GetSize();
            T_GLINE_DGN_STFN StfnGLineD= DLineLbd.aDgnStfnD[j];
            for(k=0; k<nStfnLineSize; k++)
            {        
                int nStfnType = LineDstD.aStiffener[j].nType;
                int nLid = LineDstD.aStiffener[j].aLineIx[k];
                T_GSEC_LINE StLine = GLineData.aLine[nLid];
                T_GLINE_BASE StGLine;
                if(nStfnType==0)
                {
                    StGLine.iLineType = 1;
                    nOpenStfnSize++;
                }
                else if(nStfnType==1)
                {
                    if(k==0) StGLine.iLineType = 0;
                    else     StGLine.iLineType = 1;
                    nOpenStfnSize++;
                }
                else if(nStfnType==2)
                {
                    StGLine.iLineType = 0;
                    nClosedStfnSize++;
                }
                else ASSERT(0);

                StGLine.aLineIx.Add(nLid);
                T_GSEC_VERTEX vtx1 = GLineData.PolyD.aVertex[StLine.v1];
                T_GSEC_VERTEX vtx2 = GLineData.PolyD.aVertex[StLine.v2];
                double dy1 = vtx1.dX;
                double dz1 = vtx1.dY;
                double dy2 = vtx2.dX;
                double dz2 = vtx2.dY;
                double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
                StGLine.db = dLen;
                StGLine.dt = StLine.dThik;
                StGLine.dy1 = dy1;
                StGLine.dz1 = dz1;
                StGLine.dy2 = dy2;
                StGLine.dz2 = dz2;
                //
                T_GLINE_DST StfnLineDst;
                StfnLineDst.aLineDgn.Add(StGLine);        
                T_DGNLINE_LBD StfnDLineLbd;        
                Get_LocalBucklingDataOfSubPanel(dfy1, dfy2, dE, dPoisson, da, aForce, dCzm_gap, dStiff, GLineData, StfnLineDst, StfnDLineLbd);
                StfnGLineD.aStfnDgnBaseD.Add(StfnDLineLbd.aDgnBaseD[0]);             
            }
            DLineLbd.aDgnStfnD.SetAt(j, StfnGLineD);      
        }

        Get_ElasticCriticalPlateBucklingData(dfyi, da, dE, dPoisson, aForce, DLineLbd);
        //    
        BOOL bIsOpenStfn = nOpenStfnSize>=nClosedStfnSize; // 같거나 크면 open stiffener
        Get_ColumnBucklingData(dfyi, da, dE, dPoisson, bIsOpenStfn, aForce, DLineLbd);

        T_DGNLINE_PTB PlateBuckD = DLineLbd.PlateBuckD;
        double dSigma_cr_p = PlateBuckD.dSigma_cr_p;
        double dRho_p = PlateBuckD.dRho;

        T_DGNLINE_CTB ColumnBuckD = DLineLbd.ColumnBuckD;
        double dSigma_cr_c = ColumnBuckD.dSigma_cr_c;
        double dXc = ColumnBuckD.dXc;

        double dxi = Get_xi(dSigma_cr_p, dSigma_cr_c);
        double dRho_c = Get_Rho_c_FinalReductionFactor(dRho_p, dXc, dxi);

        DLineLbd.dxi = dxi;
        DLineLbd.dRho_c = dRho_c;

        DLinePart.aInternal.SetAt(i, DLineLbd);    
    } 

    if(dt_out>0 && dt_int>0) dt_in = min(dt_out, dt_int);
    else if(dt_out>0 && dt_int==0.0) dt_in = dt_out;
    else if(dt_int>0 && dt_out==0.0) dt_in = dt_int;
    //else ASSERT(0); //!/

    DLinePart.db_in = db_in;
    DLinePart.dt_in = dt_in;

    return TRUE;
}


BOOL CDgnPlateGirder_IRC::Get_ElasticCriticalPlateBucklingData(double dfy, double da, double dE, double dPoisson, double* aForce, T_DGNLINE_LBD& DLineLbd)
{
    int iPanelSize		= DLineLbd.aDgnBaseD.GetSize();
    int iLineStressType	= DLineLbd.iLineStressType;
    int iCompPanelSize	= DLineLbd.iCompPanelSize;

    double db = DLineLbd.dbc + DLineLbd.dbt;
    double dt = DLineLbd.dt;
    double dPsi = DLineLbd.dPsi;

    T_DGNLINE_PTB PlateBuckD;
    T_DGNLINE_LBD CompLineLbd;
    Get_ConvertCompPanelDgnLineLBD(DLineLbd, CompLineLbd);

    double dLamda_p = 0.0;
    if(iCompPanelSize==1)
    {
        Get_Sigma_cr_p_4Stiffener(da, dE, dPoisson, aForce, CompLineLbd, DLineLbd);
        double dEpsilon = Get_Epsilon(dfy);
        double dk_sig = CompLineLbd.aDgnBaseD[0].SubPanelD.dk_sigma;
        dLamda_p = Get_Lamda_p(db, dt, dEpsilon, dk_sig);
        //
    }
    else if(iCompPanelSize>1) 
    {
        Get_Sigma_cr_p_4Stiffener(da, dE, dPoisson, aForce, CompLineLbd, DLineLbd);
        PlateBuckD = DLineLbd.PlateBuckD;
        double dSigma_cr_p = PlateBuckD.dSigma_cr_p;
        double dAc = PlateBuckD.dAc;
        double dAc_eff_loc = PlateBuckD.dAc_eff_loc;    
        double dBeta_A_c = dAc==0.0 ? 0.0 : dAc_eff_loc/dAc;
        dLamda_p = Get_Lamda_p4PlateTypeBehaviour(dBeta_A_c, dfy, dSigma_cr_p);
    }  

    T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[0];
    BOOL bInternal = LineDgnB.iLineType==0 ? TRUE : FALSE;
    double dRho = 0.0;
    Get_ReductionFactorRho(bInternal, db, dt, dLamda_p, dPsi, dRho);    

    DLineLbd.PlateBuckD.dLamda_p = dLamda_p;
    DLineLbd.PlateBuckD.dRho     = dRho;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ConvertCompPanelDgnLineLBD(const T_DGNLINE_LBD& DLineLbd, T_DGNLINE_LBD& CompDLineLbd)
{
    int iLineStressType = DLineLbd.iLineStressType;
    int iPanelSize = DLineLbd.aDgnBaseD.GetSize();
    CompDLineLbd = DLineLbd;
    if(iLineStressType==0 || iLineStressType==2) {return TRUE;}

    if(iLineStressType==1 || iLineStressType==3)  // 1=양연압축이고 aDgnBaseD의 마지막 dStr2가 Sig1, 3=양연압축/인장 aDgnBaseD의 마지막 dStr2가 Sig1
    {
        CompDLineLbd.aDgnBaseD.RemoveAll();
        CompDLineLbd.aDgnBaseD.SetSize(iPanelSize);

        for(int i=0; i< iPanelSize; i++)
        {      
            int iPanIdx = iPanelSize-1-i;
            T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[iPanIdx];
            int iSubStressType = LineDgnB.SubPanelD.nType;      
            CompDLineLbd.aDgnBaseD.SetAt(i,LineDgnB);
        }

        int iStfnSize = DLineLbd.aDgnStfnD.GetSize();
        CompDLineLbd.aDgnStfnD.RemoveAll();
        CompDLineLbd.aDgnStfnD.SetSize(iStfnSize);

        for(int i=0; i< iStfnSize; i++)
        {
            int iStfnIdx=iStfnSize-1-i;
            T_GLINE_DGN_STFN StfnD = DLineLbd.aDgnStfnD[iStfnIdx];
            CompDLineLbd.aDgnStfnD.SetAt(i, StfnD);
        }
    }

    return TRUE;  
}

BOOL CDgnPlateGirder_IRC::Get_ColumnBucklingData(double dfy, double da, double dE, double dPoisson, BOOL bIsOpenStfn, double* aForce,
                                                 T_DGNLINE_LBD& DLineLbd)
{
    int iPanelSize = DLineLbd.aDgnBaseD.GetSize();

    int iCompPanelSize=0;
    for(int i=0; i<iPanelSize; i++)
    {
        T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[i];
        if(LineDgnB.SubPanelD.nType!=-1) iCompPanelSize++;
    }

    double dt_t = DLineLbd.dt;
    double dSigma_cr_c=0.0;
    if(iCompPanelSize==1)
    {
        dSigma_cr_c = Get_Sigma_cr_c4ColumnTypeBehaviour(dE, dt_t, dPoisson, da);
        DLineLbd.ColumnBuckD.dSigma_cr_c = dSigma_cr_c;
    }
    else if(iCompPanelSize>1)
    {    

        T_DGNLINE_PTB PlateBuckD = DLineLbd.PlateBuckD;

        T_GLINE_DGN_STFN DgnStfnD = DLineLbd.aDgnStfnD[0];
        double dAsl1 = DgnStfnD.dAsl;
        double dIsl1 = DgnStfnD.dIsl;
        double dAsl1_eff = DgnStfnD.dAsl_eff;
        double dIsl1_eff = DgnStfnD.dIsl_eff;
        double dBeta_A_c = dAsl1==0.0 ? 0.0 : dAsl1_eff/dAsl1;
        double dSigma_sl = DgnStfnD.dSigma_sl;

        T_GLINE_DGN_B LineDgnB = DLineLbd.aDgnBaseD[0];
        int nCase_IpfF = bIsOpenStfn ? 3 : 2;
        double dalpha = Get_ImperfectionFactor(nCase_IpfF); // EN1993-1-5, 4.5.3(5) alpha for stiffened plate
        double dbc_t = DLineLbd.dbc;
        double db1 = LineDgnB.db;
        dSigma_cr_c = dbc_t/(dbc_t - db1)*dSigma_sl;    

        double dLamda_c = Get_Lamda_p4PlateTypeBehaviour(dBeta_A_c, dfy, dSigma_cr_c);
        double di = dAsl1==0.0 ? 0.0 : sqrt(dIsl1/dAsl1);
        double de = DgnStfnD.de;
        double di_div_e = de==0.0 ? 0.0 : di/de;
        double dalpha_E = di_div_e==0.0 ? 0.0 : dalpha+0.09/(di_div_e);
        double dPhi = Get_PHI_LT(dalpha_E, dLamda_c);
        double dX_org=0.0;
        double dXc = Get_X_LT(dPhi, dLamda_c, dX_org);

        T_DGNLINE_CTB ColumnBuckD;    
        ColumnBuckD.dBeta_A_c = dBeta_A_c;
        ColumnBuckD.dSigma_cr_c = dSigma_cr_c;
        ColumnBuckD.dSigma_sl = dSigma_sl;
        ColumnBuckD.dLamda_c  = dLamda_c ;
        ColumnBuckD.di        = di       ;
        ColumnBuckD.dalpha_E  = dalpha_E ;
        ColumnBuckD.dPhi      = dPhi     ;
        ColumnBuckD.dXc       = dXc      ;
        DLineLbd.ColumnBuckD = ColumnBuckD;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_Sigma_cr_p_4Stiffener(double da, double dE, double dPoisson, double* aForce, const T_DGNLINE_LBD& CompDLineLbd,
                                                    T_DGNLINE_LBD& DLineLbd)
{
    int iCompPanelSize= DLineLbd.iCompPanelSize;
    if(iCompPanelSize<1) {return TRUE;}

    double dFx = aForce[0];
    double dMy = aForce[4];
    double dMz = aForce[5];
    int iLineStressType = DLineLbd.iLineStressType;  

    int i=0;    
    double dbc_t = DLineLbd.dbc;  
    double dbt_t = DLineLbd.dbt; 
    double dt_t = DLineLbd.dt;
    double db_t = dbc_t + dbt_t;  
    double dSigma_cr_p=0.0;

    int iPanelSize = DLineLbd.aDgnBaseD.GetSize();
    int idx=0;      
    // 1=양연압축이고 aDgnBaseD의 마지막 dStr2가 Sig1, 3=양연압축/인장 aDgnBaseD의 마지막 dStr2가 Sig1

    double db1=0.0, db2=0.0, db3=0.0;
    double dt1=0.0, dt2=0.0, dt3=0.0;
    double db=0.0, dt=0.0;  
    double dB1=0.0, dB2=0.0;  
    double dAc=0.0, dAc_eff_loc=0.0;

    T_GLINE_DGN_STFN DgnStfnD;
    T_DGNLINE_PTB CalcPlateBuckD;
    Get_AreaAndEffArea4Line(CompDLineLbd, dAc, dAc_eff_loc);
    CalcPlateBuckD.dAc = dAc;
    CalcPlateBuckD.dAc_eff_loc = dAc_eff_loc;

    if(iCompPanelSize==1)
    {
        db1 = CompDLineLbd.aDgnBaseD[0].db;    
        dt1 = CompDLineLbd.aDgnBaseD[0].dt;

        double dk_sig = CompDLineLbd.aDgnBaseD[0].SubPanelD.dk_sigma;
        double dSigma_E = Get_Sigma_E(dE, db_t, dt_t, dPoisson);  
        dSigma_cr_p = dk_sig * dSigma_E;

        CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;
    }
    else if(iCompPanelSize==2)
    {
        db1 = CompDLineLbd.aDgnBaseD[0].db;
        db2 = CompDLineLbd.aDgnBaseD[1].db;
        dt1 = CompDLineLbd.aDgnBaseD[0].dt;
        dt2 = CompDLineLbd.aDgnBaseD[1].dt;
        dB1 = db1 + db2;
        dt = min(dt1, dt2);
        DgnStfnD = DLineLbd.aDgnStfnD[0];
        double da_c1=0.0;
        double dAsl1= DgnStfnD.dAsl;
        double dIsl1= DgnStfnD.dIsl;
        double dSigma_sl1= DgnStfnD.dSigma_sl;    
        // Asl를 T_GLINE_DGN_B 에 추가. 
        double dSigma_cr_sl1 = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, dB1, db1, db2, dAsl1, dIsl1, da, da_c1);
        dSigma_cr_p = fabs(dbc_t - db1) < m_dZero? 0.0 : dbc_t/(dbc_t - db1)*dSigma_cr_sl1;
        CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;

    }
    else if(iCompPanelSize==3)
    {
        // Stiffener I    
        db1 = CompDLineLbd.aDgnBaseD[0].db;
        db2 = CompDLineLbd.aDgnBaseD[1].db;
        db3 = CompDLineLbd.aDgnBaseD[2].db;
        dt1 = CompDLineLbd.aDgnBaseD[0].dt;
        dt2 = CompDLineLbd.aDgnBaseD[1].dt;
        dt3 = CompDLineLbd.aDgnBaseD[2].dt;
        dB1 = db1 + db2;     
        dt = min(dt1, dt2);

        DgnStfnD = DLineLbd.aDgnStfnD[0];
        double da_c1=0.0;
        double dAsl1= DgnStfnD.dAsl;
        double dIsl1= DgnStfnD.dIsl;
        double dSigma_sl1= DgnStfnD.dSigma_sl;
        // Asl를 T_GLINE_DGN_B 에 추가. 
        double dSigma_cr_sl1 = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, dB1, db1, db2, dAsl1, dIsl1, da, da_c1);
        double dSigma_cr_p1 = fabs(dbc_t - db1) < m_dZero ? 0.0 : dbc_t/(dbc_t - db1)*dSigma_cr_sl1; // 사다리꼴이면?

        // Stiffener II
        db1 = CompDLineLbd.aDgnBaseD[1].db;
        db2 = CompDLineLbd.aDgnBaseD[2].db;
        dt1 = CompDLineLbd.aDgnBaseD[1].dt;
        dt2 = CompDLineLbd.aDgnBaseD[2].dt;
        dB2 = db1 + db2;
        dt = min(dt1, dt2);

        DgnStfnD = DLineLbd.aDgnStfnD[1];
        double da_c2= 0.0;
        double dAsl2= DgnStfnD.dAsl;
        double dIsl2= DgnStfnD.dIsl;
        double dSigma_sl2=DgnStfnD.dSigma_sl;
        // Asl를 T_GLINE_DGN_B 에 추가. 
        double dSigma_cr_sl2 = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, dB2, db1, db2, dAsl2, dIsl2, da, da_c2);
        double dSigma_cr_p2 = fabs(dbc_t - dB1) < m_dZero ? 0.0 : dbc_t/(dbc_t - dB1)*dSigma_cr_sl2;

        // Lumped stiffener
        double da_c_lumped=0.0;
        double dAsl_lumped = dAsl1 + dAsl2;
        double dIsl_lumped = dIsl1 + dIsl2;
        double dFsl1 = dAsl1 * dSigma_sl1;
        double dFsl2 = dAsl2 * dSigma_sl2;
        double db_lumped1 = (dFsl1*db1 + dFsl2*db2)/(dFsl1+dFsl2);
        double db_lumped2 = db_t - db_lumped1;    
        double dSigma_cr_sl_lumped = Get_Sigma_cr_sl4SimplifiedModel(dE, dPoisson, dt, db_t, db_lumped1, db_lumped2, dAsl_lumped, dIsl_lumped, da, da_c_lumped);
        double dSigma_cr_p_lumped = fabs(dbc_t - db_lumped1) < m_dZero ? 0.0 : dbc_t/(dbc_t - db_lumped1)*dSigma_cr_sl_lumped;

        //
        dSigma_cr_p = min(min(dSigma_cr_p1,dSigma_cr_p2),dSigma_cr_p_lumped); // min
        CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;
    }
    else if(iCompPanelSize>3)
    {    
        double dAp      = DLineLbd.dAp;
        double dIp      = DLineLbd.dIp;
        double dAsl_sum = DLineLbd.dAsl_sum;
        double dIsl_sum = DLineLbd.dIsl_sum;
        double dalpha   = DLineLbd.dalpha;
        double dgamma   = dIp==0.0 ? 0.0 : dIsl_sum/dIp;
        double dPsi     = DLineLbd.dPsi;
        double ddelta   = dAp==0.0 ? 0.0 : dAsl_sum/dAp;
        double dalpha_lim = 0.0;
        double dk_sigma_p = Get_k_sigma_p(dalpha, dgamma, dPsi, ddelta, dalpha_lim);
        double dSigma_E = Get_Sigma_E(dE, db_t, dt_t, dPoisson);

        dSigma_cr_p = dk_sigma_p * dSigma_E;

        CalcPlateBuckD.dgamma = dgamma;
        CalcPlateBuckD.dalpha_lt = dalpha;
        CalcPlateBuckD.ddelta = ddelta;
        CalcPlateBuckD.dk_sigma_p = dk_sigma_p;
        CalcPlateBuckD.dSigma_cr_p = dSigma_cr_p;

    }
    else
        ASSERT(0);

    DLineLbd.PlateBuckD = CalcPlateBuckD;
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_Sigma_cr_p_4MultifleStiffener(double da, double dE, double dPoisson, double* aForce, T_DGNLINE_LBD& DLineLbd)
{
    int i=0;    
    double dFx = aForce[0];
    double dMy = aForce[4];
    double dMz = aForce[5];
    int iLineStressType = DLineLbd.iLineStressType;

    double dbc_t = DLineLbd.dbc;  
    double dbt_t = DLineLbd.dbt; 
    double db_t = dbc_t + dbt_t;
    double dt_t = DLineLbd.dt;
    double dSigma_cr_p=0.0;

    int iPanelSize = DLineLbd.aDgnBaseD.GetSize();
    int idx=0;    
    T_DGNLINE_LBD CompDLineLbd;

    T_GLINE_DGN_STFN DgnStfnD;

    double dAp      = DLineLbd.dAp;
    double dIp      = DLineLbd.dIp;
    double dAsl_sum = DLineLbd.dAsl_sum;
    double dIsl_sum = DLineLbd.dIsl_sum;
    double dalpha   = DLineLbd.dalpha;
    double dgamma   = dIp==0.0 ? 0.0 : dIsl_sum/dIp;
    double dPsi     = DLineLbd.dPsi;
    double ddelta   = dAsl_sum/dAp;
    double dalpha_lim = 0.0;
    double dk_sigma_p = Get_k_sigma_p(dalpha, dgamma, dPsi, ddelta, dalpha_lim);
    double dSigma_E = Get_Sigma_E(dE, db_t, dt_t, dPoisson);

    dSigma_cr_p = dk_sigma_p * dSigma_E;

    DLineLbd.PlateBuckD.dSigma_cr_p = dSigma_cr_p;
    return TRUE;
}

double CDgnPlateGirder_IRC::Get_Sigma_E(double dEs, double db, double dt, double dPoisson)
{
    double dPi = m_dPI;
    double dSigma = (dPi*dPi)*dEs*(dt*dt)/(12.0*(1.0-(dPoisson*dPoisson))*(db*db));

    return dSigma;
}

double CDgnPlateGirder_IRC::Get_k_sigma_p(double dalpha, double dgamma, double dPsi, double ddelta, double& dalpha_lim)
{
    double dk_sigma_p=0.0;

    if(dPsi<0.5) dPsi = 0.5; // Annex A, A.1
    if(dalpha<0.5) dalpha = 0.5; // Annex A, A.1

    double dk_param0 = (dPsi+1.0)*(1.0+ddelta);
    if(dk_param0==0.0) return 0.0;

    double dVal = pow(dgamma, 1./4.);

    if(dalpha<dVal || fabs(dalpha-dVal)<m_dZero)
    {
        double dk_param1 = 1.0+pow(dalpha, 2);    
        dk_sigma_p = 2.0*(dk_param1*dk_param1 + dgamma -1.0)/(dalpha*dalpha*dk_param0);
    }
    else if(dalpha>dVal)
    {
        dk_sigma_p = 4.0*(1.0+sqrt(dgamma))/dk_param0;
    }
    else 
        ASSERT(0);

    return dk_sigma_p;
}

double CDgnPlateGirder_IRC::Get_Ip_4BendingOfPlate(double db, double dt, double dPoisson)
{
    if(fabs(dPoisson-1.0)<m_dZero) {return 0.0;}
    double dIp = db*pow(dt,3)/(12.0*(1.0-dPoisson*dPoisson));

    return dIp;
}

double CDgnPlateGirder_IRC::Get_Width4StiffenedPlates(int iNtype, double db, double dPsi, BOOL bSup)
{
    double dwidth=0.0;
    if(iNtype==Stiffened_Plate_inf)
    {
        dwidth = (3.0-dPsi)/(5.0-dPsi)*db;
    }
    else if(iNtype==Stiffened_Plate_sup)
    {
        dwidth = 2.0/(5.0-dPsi)*db;
    }
    else if(iNtype==Stiffened_Plate_sup_dbc)
    {
        if(bSup) {dwidth = 0.4*db;}
        else     {dwidth = 0.6*db;}
    }
    else
    {
        ASSERT(0);
        dwidth = -1.0;
    }

    return dwidth;
}

int CDgnPlateGirder_IRC::Get_StreeTypeOfStiffenedPlates(double df1, double df2, int& iStrType2)
{
    int iType=-1;
    BOOL bCompStr = Is_CompStress(df1, df2);
    BOOL bDbCurve = Is_DoubleCurvature(df1, df2);

    if(bCompStr)
    {
        if(!bDbCurve)
        {
            if(fabs(df1)<fabs(df2))
            {
                iType = Stiffened_Plate_inf;
                iStrType2 = Stiffened_Plate_sup;
            }
            else if(fabs(df1)>fabs(df2) || fabs(df1-df2)<m_dZero)
            {
                iType = Stiffened_Plate_sup;
                iStrType2 = Stiffened_Plate_inf;
            }
        }
        else
        {
            iType = Stiffened_Plate_sup_dbc;      
        }
    }
    else
    {
        iType = -1; // 인장
    }

    return iType;
}

BOOL CDgnPlateGirder_IRC::Get_AreaAndEffArea4Line(const T_DGNLINE_LBD& DgnLineLBD, double& dAc, double& dAc_eff_loc)
{
    int iLineStressType = DgnLineLBD.iLineStressType;
    int iCompPanelSize = DgnLineLBD.iCompPanelSize;
    int iPanelSize = DgnLineLBD.aDgnBaseD.GetSize();
    int iStfnSize = DgnLineLBD.aDgnStfnD.GetSize();

    for(int i=0; i<iPanelSize; i++)
    {
        T_GLINE_DGN_B LineDgnB = DgnLineLBD.aDgnBaseD[i];
        T_SUB_PANEL_BASE SubPanelD = LineDgnB.SubPanelD;
        if(!LineDgnB.bCompStr) continue;  // aDgnBaseD정보는 0번째부터 압축이 들어있어야함. 

        if(iCompPanelSize==1)
        {
        }
        else
        {
            if(i==0)
            {
                dAc += SubPanelD.dA_inf; // dA_sup는 edge로..
                dAc_eff_loc += SubPanelD.dA_inf_eff;
            }
            else if(i==iCompPanelSize-1)
            {
                dAc += SubPanelD.dA_sup; // dA_inf는 edge로..
                dAc_eff_loc += SubPanelD.dA_sup_eff;
            }
            else 
            {
                dAc += SubPanelD.dA_sup + SubPanelD.dA_inf;
                dAc_eff_loc += SubPanelD.dA_inf_eff + SubPanelD.dA_sup_eff;
            }
        }    
    }
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_MomentofAreaOfFlange(int iChkPartIx, double dCzm_gap, T_SECT_STIFFNESS& StiffD, const T_GLINE_DGN_D& GLineDgn, double& dQflg)
{
    if(iChkPartIx>GLineDgn.aPartLine.GetSize()-1) return FALSE; // Data가 없으면 error
    dQflg=0.0;

    int i=0,j=0;
    double dCym = StiffD.Cym;
    double dCzm = StiffD.Czm;
    T_GLINE_PART_DGN GLinePartDgn = GLineDgn.aPartLine[iChkPartIx];

    CArray<T_GSEC_POLYGON,T_GSEC_POLYGON &> aGsecPolygon;
    T_GSEC_POLYGON PolygonD = GLineDgn.ModifiedGsecPolyD;
    aGsecPolygon.SetSize(1);
    aGsecPolygon.SetAt(0, PolygonD);
    double dMaxPt[2], dMinPt[2];
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aTempLineData;
    aTempLineData.Copy(GLineDgn.aModifiedLine);
    CSectUtil::Get_MaxMinPoint4LineType(PolygonD, aTempLineData, dMaxPt, dMinPt);

    double dYcent = dMinPt[0]+dCym; // 도심의 좌표.
    double dZcent = dMinPt[1]+dCzm;

    CArray<int,int&> aPartLine;
    int iOutSize = GLinePartDgn.aOutstand.GetSize();
    for(i=0; i<iOutSize; i++)
    {
        T_GLINE_DST GLineDstD = GLinePartDgn.aOutstand[i];
        int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
        for(j=0; j<iLineDgnSize; j++)
        {
            T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[j];
            aPartLine.Append(GLineBaseD.aLineIx);
        }    
    }
    int iIntSize = GLinePartDgn.aInternal.GetSize();
    for(i=0; i<iIntSize; i++)
    {
        T_GLINE_DST GLineDstD = GLinePartDgn.aInternal[i];
        int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
        for(j=0; j<iLineDgnSize; j++)
        {
            T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[j];
            aPartLine.Append(GLineBaseD.aLineIx);
        }    
    }

    int iPartLineSize = aPartLine.GetSize();

    for(i=0; i<iPartLineSize; i++)
    {
        int iLineIx = aPartLine[i];
        T_GSEC_LINE GLineD = GLineDgn.aModifiedLine[iLineIx];
        double dt = GLineD.dThik;
        double dy1 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v1].dX;
        double dz1 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v1].dY + dCzm_gap;
        double dy2 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v2].dX;
        double dz2 = GLineDgn.ModifiedGsecPolyD.aVertex[GLineD.v2].dY + dCzm_gap;

        double dyi_max = max(dy1,dy2);
        BOOL bCalcLine=FALSE;    
        double dQi=0.0;
        double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
        double dAi = dt*dLen;
        double dzc = 0.5*(dz1+dz2);    
        //    if(dyi_max<dYcent || fabs(dyi_max-dYcent)<m_dZero)
        //    {      
        //      double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
        //      dAi = dt*dLen;
        //      dzc = 0.5*(dz1+dz2);      
        //    }
        //    else if(dy1<dYcent && dYcent<dy2)
        //    {
        //      double dcutLength = CMathFunc::mathLength(dy1, dz1, dYcent, dz2);
        //      dAi = dt*dcutLength;
        //      dzc = 0.5*(dz1+dz2);
        //    }
        //    else if(dy2<dYcent && dYcent<dy1)
        //    {
        //      double dcutLength = CMathFunc::mathLength(dy2, dz2, dYcent, dz2);
        //      dAi = dt*dcutLength;
        //      dzc = 0.5*(dz1+dz2);
        //    }
        //    else 
        //      continue;

        double dZdis = fabs(dzc);
        dQi = dAi * dZdis;
        dQflg += dQi;    
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_Nc_fofSlab(BOOL bPoisI, double dfcd, double dXpl, double dHt, T_SECT_SECTBASE_D& SectBaseD, const T_GLINE_DGN_D& GLineDgn, double& dNc_f)
{
    double dCzm_s = Get_CzmofSlab(bPoisI);
    double dCzp_s = Get_CzpofSlab(bPoisI);
    double dHs = dCzm_s + dCzp_s;

    double dArea_s=0.0;
    int iPartSize = SectBaseD.aGeneralPart.GetSize();
    double dzpos = dHt-dXpl;
    for(int i=0; i<iPartSize; i++)
    {
        T_GSEC_PART PartD = SectBaseD.aGeneralPart[i];
        if(PartD.bPlane)
        {
            if(dXpl<dHs)
            {
                double aArea[4]; 
                memset(aArea,0,sizeof(aArea));
                CArray<T_GSEC_LINE, T_GSEC_LINE&> aTopLine, aBotLine, aHCutLine;
                CArray<double,double&> aHCutTopThick;
                T_GSEC_POLYGON PolyD;
                double aZcen[4]; // 자르는 직선기준으로 잘려진 단면의 도심 
                memset(aZcen,0,sizeof(aZcen));
                T_SECT_SECTBASE_D CalcSectBaseD; CalcSectBaseD.Initialize();
                Get_ConvertSectBase4GenLineData(SectBaseD, GLineDgn, CalcSectBaseD);
                CSectUtil::Get_SectPropertyOfCuttingArea(dzpos, CalcSectBaseD, aArea, aZcen, PolyD, aTopLine, aBotLine, aHCutLine, aHCutTopThick);
                dArea_s = aArea[0];        
            }
            else
            {
                dArea_s = PartD.Stiffness.Area;
            }      
            break;
        }    
    }  
    dNc_f = dfcd * dArea_s;

    return TRUE;
}

int CDgnPlateGirder_IRC::Get_StiffenerSize(const T_GLINE_DST& GLineDST)
{
    int iStfnSize=0;
    int iInputStfnSize = GLineDST.aStiffener.GetSize();
    for(int i=0; i<iInputStfnSize; i++)
    {
        int iStfnType = GLineDST.aStiffener[i].nType;
        if(iStfnType==0 || iStfnType==1) iStfnSize++;
        else if(iStfnType==2) iStfnSize += 2;
        else ASSERT(0);
    }

    return iStfnSize;
}

double CDgnPlateGirder_IRC::Get_Lamda_p4PlateTypeBehaviour(double dBeta_A_c, double dfy, double dSigma_cr_p)
{
    if(dSigma_cr_p<=0.0) return 0.0;
    double dLamda_p=sqrt(dBeta_A_c*dfy/dSigma_cr_p);

    return dLamda_p;
}

double CDgnPlateGirder_IRC::Get_Sigma_cr_c4ColumnTypeBehaviour(double dEs, double dt, double dnu, double da)
{
    if(da==0.0) return 0.0;
    double dSigma_cr_c = (m_dPI*m_dPI)*dEs*(dt*dt)/(12.*(1.-dnu*dnu)*(da*da));
    return dSigma_cr_c;
}

double CDgnPlateGirder_IRC::Get_Sigma_cr_sl4ColumnTypeBehaviour(double dEs, double dI_sl, double dA_sl, double da)
{
    if(dA_sl==0.0 || da==0.0) {return 0.0;}
    double dSigma_sl = (m_dPI*m_dPI)*dEs*dI_sl/(dA_sl*(da*da));
    return dSigma_sl;
}

double CDgnPlateGirder_IRC::Get_Sigma_cr_sl4SimplifiedModel(double dE, double dPoisson, double dt, double db, double db1, double db2, 
                                                            double dAsl, double dIsl, double da, double& da_c)
{
    if(dAsl==0.0 || db1==0.0 || db2==0.0 || fabs(dPoisson-1.0)<m_dZero) {return 0.0;}

    double dSigma=0.0;
    double dPi = CMathFunc::m_pi;

    double da_c_param = dIsl*pow(db1,2)*pow(db2,2)/(pow(dt,3)*db);
    da_c = 4.33*pow(da_c_param, 1.0/4.0);

    if(da>da_c || fabs(da-da_c)<m_dZero)
    {
        dSigma = 1.05*dE/dAsl*(sqrt(dIsl*pow(dt,3)*db))/(db1*db2);
    }
    else 
    {
        double dSigma_param1 = 4.0*pow(dPi,2)*(1.0-pow(dPoisson,2))*dAsl*pow(db1,2)*pow(db2,2);
        dSigma = fabs(da) < m_dZero ? 0.0 : pow(dPi,2)*dE*dIsl/(dAsl*pow(da,2)) + dE*pow(dt,3)*db*pow(da,2)/dSigma_param1;
    }  

    return dSigma;
}

double CDgnPlateGirder_IRC::Get_xi(double dSigma_cr_p, double dSigma_cr_c)
{
    double dxi = dSigma_cr_c==0.0 ? dSigma_cr_p/m_dZero : dSigma_cr_p/dSigma_cr_c;
    if(dxi<0.0) dxi = 0.0;
    else if(dxi>1.0) dxi = 1.0;

    return dxi;
}

double CDgnPlateGirder_IRC::Get_Rho_c_FinalReductionFactor(double dRho, double dXc, double dxi)
{
    double dRho_c = (dRho-dXc)*dxi*(2.0-dxi)+dXc;

    return dRho_c;
}


BOOL CDgnPlateGirder_IRC::Get_EffectiveSection4StiffenedPlateElements(double dfy1, double dfy2, double* dForce, double dArea, double dIy, double dIz, 
                                                                      const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_GENL_DATA& EffGLineD, T_GLINE_DST& EffLineDst)
{
    int i=0;

    T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
    aLine.Copy(GLineData.aLine);
    int iGLineSize = aLine.GetSize();
    if(iGLineSize<=0) return FALSE;

    EffGLineD = GLineData;  
    EffLineDst = LineDst;  

    double dFx=dForce[0];
    double dMy=dForce[4];
    double dMz=dForce[5];
    double dStiff[3]={dArea, dIy, dIz};

    int iLineDgnSize = LineDst.aLineDgn.GetSize();
    double dSig1=0., dSig2=0., dPsi=0.;  
    double dk_sig=0.0, dLamda_p=0.0;
    double dRho=0.0;
    double dbeff=0., dbe1=0., dbe2=0.;
    BOOL bInternal=TRUE;
    int iOutstandType=0;
    BOOL bIs1_Sig1=TRUE;
    int iStressType=-1;
    double dxe=0.0, dye=0.0, dze=0.0;
    BOOL b1stIdx=TRUE;
    int iStaVtx=0;  
    for(i=0; i<iLineDgnSize; i++)
    {
        T_GLINE_BASE EffLineBaseD;
        T_GLINE_BASE LineBaseD = LineDst.aLineDgn[i]; // Panel
        int iLineSize = LineBaseD.aLineIx.GetSize();
        bInternal = LineBaseD.iLineType==0 ? TRUE : FALSE;
        double dy1 = LineBaseD.dy1; // Panel의 양끝.
        double dz1 = LineBaseD.dz1;
        double dy2 = LineBaseD.dy2;
        double dz2 = LineBaseD.dz2;
        double dStr1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy1, dz1);
        double dStr2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy2, dz2);
        BOOL bCompStr = Is_CompStress(dStr1, dStr2);
        BOOL bDbCurve = Is_DoubleCurvature(dStr1, dStr2);
        int iDivLineIx = 0;


        double dbs = LineBaseD.db;
        double dts = LineBaseD.dt;
        double dfyi = Get_fy4Thick(dfy1, dfy2, dts);
        double dEpsilon = Get_Epsilon(dfy1);
        Get_f1_f2(dStr1, dStr2, dSig1, dSig2, bIs1_Sig1);
        if(bCompStr)
        {      
            dPsi = Get_Psi(dSig1, dSig2);
            dk_sig = Get_InternalBucklingfactor(dPsi);
            dLamda_p = Get_Lamda_p(dbs, dts, dEpsilon, dk_sig);
            Get_ReductionFactorRho(bInternal, dbs, dts, dLamda_p, dPsi, dRho);
            if(bInternal) 
            {
                EffGLineD.Initialize();
                b1stIdx = bIs1_Sig1 ? TRUE : FALSE;
                iStressType = Get_StressDistributionType4Internal(dStr1, dStr2);
                Get_EffectiveWidth4Internal(dPsi, dbs, dRho, dbeff, dbe1, dbe2);
                // Sigma1        
                if(Get_EffectivePanelInfo(bInternal, iStressType, TRUE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD))
                {          
                }
                // Sigma2                 
                if(Get_EffectivePanelInfo(bInternal, iStressType, FALSE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD))
                {          
                }
                ASSERT(0); // Set하기전에 EffLineBaseD의 내부정보를 만들 함수가 필요. 
                EffLineDst.aLineDgn.SetAt(i, EffLineBaseD);        
            }
            else
            {
                BOOL bSig1IsFree=TRUE;
                iStressType = Get_StressDistributionType4Outstand(bSig1IsFree, dStr1, dStr2);
                Get_EffectiveWidth4Outstand(iOutstandType, dPsi, dbs, dRho, dbe1);
                if(iStressType==Outstand_Stress_Type1 || iStressType==Outstand_Stress_Type2 || iStressType==Outstand_Stress_Type3)
                {
                    if(Get_EffectivePanelInfo(bInternal, iStressType, b1stIdx, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD))
                    {          
                    }          
                    EffLineDst.aLineDgn.SetAt(i, EffLineBaseD);        
                }
                else if(iStressType==Outstand_Stress_Type4)
                {
                    // Sigma1        
                    if(Get_EffectivePanelInfo(bInternal, iStressType, TRUE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD))
                    {          
                    }
                    // Sigma2                 
                    if(Get_EffectivePanelInfo(bInternal, iStressType, FALSE, iStaVtx, dForce, dStiff, dbe1, dbe2, GLineData, LineBaseD, EffGLineD, EffLineBaseD))
                    {          
                    }
                    ASSERT(0); // Set하기전에 EffLineBaseD의 내부정보를 만들 함수가 필요. 
                    EffLineDst.aLineDgn.SetAt(i, EffLineBaseD);        
                }
                else ASSERT(0);
            }
        }    
        else // 인장일 경우.
        {
            EffLineDst.aLineDgn.SetAt(i, LineBaseD);
        }
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_EffectivePanelInfo(BOOL bInternal, int iStressType, BOOL b1stIdx, int ve, double* dForce, double dStiff[3], double dbe1, double dbe2, 
                                                 const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, T_GENL_DATA& EffLineD, T_GLINE_BASE& EffLineBaseD)
{
    // b1stIdx : 첫번째 Index부터냐...
    int i=0;
    int iLineSize = LineBaseD.aLineIx.GetSize();  

    double dFx=dForce[0];
    double dMy=dForce[4];
    double dMz=dForce[5];
    //
    double dArea = dStiff[0];
    double dIy   = dStiff[1];
    double dIz   = dStiff[2];

    int iEffPolyVtxSize = EffLineD.PolyD.aVertex.GetSize();
    int iEffLineSize = EffLineD.aLine.GetSize();

    CArray<int, int&> aEffLineIx;

    T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
    aLine.Copy(GLineData.aLine);

    int iIx=0, iIx2=0;
    double dbei = dbe1;  

    double dxe=0., dye=0.;
    int v1=0, v2=0, v3=0, v4=0;
    int vcn=0; // Line 2개중 연결된 vertex
    int vI=0, vJ=0;

    BOOL bCheckLine=FALSE;
    int iConnVtx = 0;
    T_GSEC_VERTEX VtxDiv;
    for(i=0; i<iLineSize; i++)
    {
        if(b1stIdx) 
        {
            iIx = i;
            iIx2 = i+1;
        }
        else
        {
            iIx = iLineSize - 1;
            iIx2 = iLineSize - 2;
        }
        int iLix = LineBaseD.aLineIx[iIx]; 
        T_GSEC_LINE LineD = aLine[iLix]; // 순차적으로 연결된 정보이어야함 
        T_GSEC_LINE Line2D;    

        v1 = LineD.v1;
        v2 = LineD.v2;  
        if(iLineSize==1)
        {
            if     (ve==v1) { vI=v1, vJ=v2; }
            else if(ve==v2) { vI=v2, vJ=v1; }
            else ASSERT(0);      
        }
        else
        {
            if(iLineSize-1==i)
            {
                if     (vcn==v1) { vI=v1, vJ=v2; }
                else if(vcn==v2) { vI=v2, vJ=v1; }
                else ASSERT(0);
            }
            else
            {
                Line2D = aLine[iIx2];
                v3 = Line2D.v1;
                v4 = Line2D.v2;
                vcn = Get_ConnectedVertexId(v1, v2, v3, v4);
                if     (v1==vcn) { vI=v2, vJ=v1; }
                else if(v2==vcn) { vI=v1, vJ=v2; }
                else ASSERT(0);
            }      
        }

        if(b1stIdx) iConnVtx = v2;
        else        iConnVtx = v1;
        T_GSEC_VERTEX vtx1 = GsecPolyD.aVertex[vI];
        T_GSEC_VERTEX vtx2 = GsecPolyD.aVertex[vJ];
        double dx1 = vtx1.dX;
        double dy1 = vtx1.dY;
        double dx2 = vtx2.dX;
        double dy2 = vtx2.dY;
        double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
        double dt = LineD.dThik;    
        double df1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx1, dy1);
        double df2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx2, dy2);
        BOOL bCompStr = Is_CompStress(df1, df2);
        BOOL bDbCurve = Is_DoubleCurvature(df1, df2);    

        BOOL b1stIsComp = TRUE;
        if(i==0)
        {
            b1stIsComp = df1<0.0 ? TRUE : FALSE;
        }

        if(bInternal)
        {
            if(iStressType==Internal_Stress_Type1 || iStressType==Internal_Stress_Type2)
            {
                if(dbi<dbei || fabs(dbi-dbei)<m_dZero) // 나누어지는 Line을 찾기 위해.
                {
                    dbei -= dbi;        
                    aEffLineIx.Add(iLix);
                    continue;        
                }
                Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);        
            }
            else if(iStressType==Internal_Stress_Type3)
            {
                if(!bCompStr) // 인장이면 
                {
                    aEffLineIx.Add(iLix);
                    continue;
                }
                else // 압축이면
                {          
                    if(bDbCurve)
                    {
                        double db1=0.0, db2=0.0;            
                        if(b1stIsComp)
                        {              
                            Get_PointCoor4beff(dbi, dx1, dy1, dx2, dy2, dxe, dye);
                        }
                        else
                        {  
                            double dbti=0.0;
                            Get_b1b2DbCurvature(dbi, df1, df2, db1, db2);              
                            if(df1>0.0) // 1쪽이 인장이면 
                            {                
                                if(dbe2>db2) // be2가 더 큰경우 
                                {
                                    dbei -= db2;
                                    aEffLineIx.Add(iLix);
                                    continue;
                                }
                                else // be2가 작을경우, 같을경우 포함. 
                                {                  
                                    dbti = db1+dbe2;
                                    Get_PointCoor4beff(dbti, dx1, dy1, dx2, dy2, dxe, dye);
                                }
                            }
                            else // 2쪽이 인장이면 
                            {
                                if(dbe2>db1) // be2가 더 큰경우 
                                {
                                    dbei -= db1;
                                    aEffLineIx.Add(iLix);
                                    continue;                  
                                }
                                else // be2가 작을경우, 같을경우 포함.  
                                {                  
                                    dbti = db2+dbe2;
                                    Get_PointCoor4beff(dbti, dx2, dy2, dx1, dy1, dxe, dye);
                                }
                            }
                        }
                    }
                    else // single curvature
                    {
                        if(dbi<dbei || fabs(dbi-dbei)<m_dZero) // 나누어지는 Line을 찾기 위해.
                        {
                            dbei -= dbi;        
                            aEffLineIx.Add(iLix);
                            continue;        
                        }
                        Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);            
                    }
                }        
            }

            VtxDiv.dX = dxe;
            VtxDiv.dY = dye;
            EffLineD.PolyD.aVertex.Add(VtxDiv);
            T_GSEC_LINE ELine1 = LineD;
            ELine1.v1 = vI;
            ELine1.v2 = iEffPolyVtxSize;        
            EffLineD.aLine.Add(ELine1);
            aEffLineIx.Add(iEffLineSize);
            break;

        } // End. Internal 
        else
        {   
            if(iStressType==Outstand_Stress_Type1 || iStressType==Outstand_Stress_Type3)
            {
                if(dbi<dbei || fabs(dbi-dbei)<m_dZero) // 나누어지는 Line을 찾기 위해.
                {
                    dbei -= dbi;        
                    aEffLineIx.Add(iLix);
                    continue;        
                }
                Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);        
            }
            else if(iStressType==Outstand_Stress_Type2)
            {
                if(!bCompStr) // 인장이면 
                {
                    aEffLineIx.Add(iLix);
                    continue;
                }
                else // 압축이면
                {          
                    if(bDbCurve)
                    {
                        double db1=0.0, db2=0.0;            
                        double dbti=0.0;
                        Get_b1b2DbCurvature(dbi, df1, df2, db1, db2);              
                        if(df1>0.0) // 1쪽이 인장이면 
                        {    
                            // Outstand에서는 be1이 b_eff를 
                            if(dbe1>db2) // be2가 더 큰경우 
                            {
                                dbei -= db2;
                                aEffLineIx.Add(iLix);
                                continue;
                            }
                            else // be2가 작을경우, 같을경우 포함. 
                            {                
                                dbti = db1+dbe1;
                                Get_PointCoor4beff(dbti, dx1, dy1, dx2, dy2, dxe, dye);
                            }
                        }
                        else // 2쪽이 인장이면 
                        {
                            if(dbe1>db1) // be2가 더 큰경우 
                            {
                                dbei -= db1;
                                aEffLineIx.Add(iLix);
                                continue;                  
                            }
                            else // be2가 작을경우, 같을경우 포함.  
                            {                
                                dbti = db2+dbe1;
                                Get_PointCoor4beff(dbti, dx2, dy2, dx1, dy1, dxe, dye);
                            }
                        }
                    }
                    else // single curvature
                    {
                        if(dbi<dbei || fabs(dbi-dbei)<m_dZero) // 나누어지는 Line을 찾기 위해.
                        {
                            dbei -= dbi;        
                            aEffLineIx.Add(iLix);
                            continue;        
                        }
                        Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);            
                    }
                }        
            }
            else if(iStressType==Outstand_Stress_Type4)
            {
                if(!bCompStr)
                {
                    aEffLineIx.Add(iLix);
                    continue;        
                }
                else
                {
                    if(bDbCurve)
                    {
                        double db1=0.0, db2=0.0;            
                        double dbti=0.0;
                        Get_b1b2DbCurvature(dbi, df1, df2, db1, db2);
                        if(df1>0.0) // 1쪽이 인장이면 
                        {    
                            Get_PointCoor4beff(db1, dx1, dy1, dx2, dy2, dxe, dye);
                        }
                        else // 2쪽이 인장이면 
                        {
                            Get_PointCoor4beff(db2, dx1, dy1, dx2, dy2, dxe, dye);
                        }
                    }
                    else
                    {
                        if(dbi<dbei || fabs(dbi-dbei)<m_dZero) // 나누어지는 Line을 찾기 위해.
                        {
                            dbei -= dbi;        
                            aEffLineIx.Add(iLix);
                            continue;        
                        }
                        Get_PointCoor4beff(dbei, dx1, dy1, dx2, dy2, dxe, dye);
                    }
                }

            }
            //
            VtxDiv.dX = dxe;
            VtxDiv.dY = dye;
            EffLineD.PolyD.aVertex.Add(VtxDiv);
            T_GSEC_LINE ELine1 = LineD;
            ELine1.v1 = vI;
            ELine1.v2 = iEffPolyVtxSize;        
            EffLineD.aLine.Add(ELine1);
            aEffLineIx.Add(iEffLineSize);
            break;     

        }
    }

    EffLineBaseD.aLineIx.Copy(aEffLineIx);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LocalBucklingDataOfSubPanel(double dfy1, double dfy2, double dE, double dPoisson, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, 
                                                          const T_GLINE_DST& LineDst, T_DGNLINE_LBD& LineLBD)
{
    int i=0;
    double dFx=dForce[0];
    double dMy=dForce[4];
    double dMz=dForce[5];
    //
    double dArea = dStiff[0];
    double dIy   = dStiff[1];
    double dIz   = dStiff[2];

    double dPsi=0.0;
    double dk_sig=0.0;
    double dLamda_p=0.0;
    BOOL bInternal=TRUE;
    BOOL bIs1_Sig1=TRUE;
    BOOL bSig1IsFree=TRUE;  

    int iLineDgnSize = LineDst.aLineDgn.GetSize();
    LineLBD.aDgnBaseD.SetSize(iLineDgnSize);

    double db_t = 0.0, dt_t=0.0;
    double dbc_sum=0.0, dbt_sum=0.0;
    double dAp_sum=0.0;
    CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&> aPanelDgnBaseD; // Stiffener정보를 위해.  
    double dStressI=0.0, dStressJ=0.0;
    BOOL bLineCompStr=FALSE;
    BOOL bLineDbCurve=FALSE;
    int iLineStressType=-1;
    int iCompPanelSize=0;  
    for(i=0; i<iLineDgnSize; i++)
    {
        T_GLINE_DGN_B LineDgnBase;
        T_GLINE_BASE LineBaseD = LineDst.aLineDgn[i]; // 의 !!!dy1,dz1, dy2,dz2는 순차적인 값이 맞음
        bInternal = LineBaseD.iLineType==0 ? TRUE : FALSE;
        double dbi = LineBaseD.db;
        double dti = LineBaseD.dt;
        double dApi = dbi*dti;
        dAp_sum += dApi;
        double dfyi = Get_fy4Thick(dfy1, dfy2, dti);
        double dEpsilon = Get_Epsilon(dfyi);
        db_t += dbi;
        if(i==0) dt_t = dti;
        else     dt_t = min(dt_t, dti);
        double dy1 = LineBaseD.dy1; // Panel의 양끝.
        double dz1 = LineBaseD.dz1 + dCzm_gap;
        double dy2 = LineBaseD.dy2;
        double dz2 = LineBaseD.dz2 + dCzm_gap;
        double dbs = LineBaseD.db;
        double dts = LineBaseD.dt;
        double dStr1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy1, dz1);
        double dStr2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dy2, dz2);
        if(IsValueZero(dStr1)) dStr1 = 0.0;
        if(IsValueZero(dStr2)) dStr2 = 0.0;
        BOOL bCompStr = Is_CompStress(dStr1, dStr2);
        BOOL bDbCurve = Is_DoubleCurvature(dStr1, dStr2);       
        int iStressType=-1;
        double dy0=0.0, dz0=0.0;
        double dSig1=0.0, dSig2=0.0;

        Get_f1_f2(dStr1, dStr2, dSig1, dSig2, bIs1_Sig1);    

        // vertex정보가 순차적으로 있다고 가정(계산 들어오기 전에 Line정보를 array에 순차적으로 해주어야함) 
        if(i==0)
        {
            dStressI = dStr1;
            dStressJ = dStr2;      
        }
        else
        {
            dStressJ = dStr2;
        }

        if(bCompStr) 
        {
            iCompPanelSize++;
            bLineCompStr=TRUE; // 하나라도 압축이 있으면 압축.
        }
        if(bDbCurve) bLineDbCurve=TRUE; // 하나라도 복곡률이 있으면 복곡률

        dPsi = Get_Psi(dSig1, dSig2);
        if(bCompStr)
        {
            if(bInternal)
            {
                iStressType = Get_StressDistributionType4Internal(dStr1, dStr2);
                dk_sig = Get_InternalBucklingfactor(dPsi);
            }
            else 
            {
                int iOutType=-1;
                bSig1IsFree = Is_Sig1IsFree4Outstand(bIs1_Sig1);
                iStressType = Get_StressDistributionType4Outstand(bSig1IsFree, dStr1, dStr2);
                if(iStressType==Outstand_Stress_Type1 || iStressType==Outstand_Stress_Type2)
                    iOutType = 1;
                else if(iStressType==Outstand_Stress_Type3 || iStressType==Outstand_Stress_Type4)
                    iOutType = 2;        
                dk_sig = Get_OutstandBucklingfactor(iOutType, dPsi);      
            }
        }

        Set_LineDgnBase(LineBaseD.iLineType, LineBaseD.db, LineBaseD.dt, dy1, dz1, dy2, dz2, dSig1, dSig2, bCompStr, bDbCurve, bIs1_Sig1, dy0, dz0, LineDgnBase);
        LineDgnBase.dStr1 = dStr1;
        LineDgnBase.dStr2 = dStr2;
        int iStressType4EffSect = Get_StressType4EffSect(bCompStr, bDbCurve, bIs1_Sig1);
        LineDgnBase.iStressType = iStressType4EffSect;

        double dbb=0.0, db_eff=0.0;
        double dRho=0.0;
        dLamda_p = Get_Lamda_p(dbs, dts, dEpsilon, dk_sig);
        Get_ReductionFactorRho(bInternal, dbs, dts, dLamda_p, dPsi, dRho);

        T_SUB_PANEL_BASE SubPanelD;
        SubPanelD.nType=iStressType;
        SubPanelD.dPsi = dPsi;
        SubPanelD.dk_sigma = dk_sig;
        SubPanelD.dLamda_p = dLamda_p;
        SubPanelD.dRho = dRho;
        int iNtype=0, iNtype2=0;
        BOOL bGrossWidth=TRUE;
        if(bInternal)
        {       
            // 
            if(bCompStr)
            {
                if(!bDbCurve)
                {
                    dbc_sum += dbi;          
                    iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);           
                    // Gross width
                    bGrossWidth = TRUE;
                    dbb = Get_Width4StiffenedPlates(iNtype, dbs, dPsi); 
                    Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dti, SubPanelD);
                    //
                    dbb = Get_Width4StiffenedPlates(iNtype2, dbs, dPsi); 
                    Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dti, SubPanelD);
                    // Effective width
                    bGrossWidth = FALSE;
                    db_eff = dRho*dbs;
                    dbb = Get_Width4StiffenedPlates(iNtype, db_eff, dPsi);
                    Set_CalcSubPanel(bGrossWidth, iNtype, dbb, dti, SubPanelD);
                    //
                    dbb = Get_Width4StiffenedPlates(iNtype2, db_eff, dPsi);
                    Set_CalcSubPanel(bGrossWidth, iNtype2, dbb, dti, SubPanelD);          
                }
                else
                {          
                    double dbc=0.0;
                    Get_CompWidthOfDoubleCurvatrue(dForce, dCzm_gap, dArea, dIy, dIz, GLineData, LineBaseD, dbc);
                    // Effective width
                    bGrossWidth = FALSE;
                    db_eff = dRho*dbc;
                    SubPanelD.db_sup = dbc;
                    iNtype = Get_StreeTypeOfStiffenedPlates(dStr1, dStr2, iNtype2);          
                    dbb = Get_Width4StiffenedPlates(iNtype, db_eff, dPsi, TRUE);
                    SubPanelD.db_sup_eff = dbb;
                    SubPanelD.dA_sup_eff = dbb * dti;
                    //
                    dbb = Get_Width4StiffenedPlates(iNtype2, db_eff, dPsi, FALSE);
                    SubPanelD.db_inf_eff = dbb;
                    SubPanelD.dA_inf_eff = dbb * dti;
                    //          
                    dbc_sum += dbc;
                    dbt_sum += dbi-dbc;          
                }
            }
            else
            {
                dbt_sum += dbi;
                // 인장.
            }
        }
        else // outstand
        {
            if(bCompStr)
            {
                Get_EffectiveWidth4Outstand(iStressType, dPsi, dbi, dRho, db_eff);
                if(!bDbCurve)
                {
                    dbc_sum += dbi;
                    SubPanelD.db_sup = dbi; // db_sup에 넣자. 
                    SubPanelD.dA_sup = dbi * dti;
                    SubPanelD.db_sup_eff = db_eff;
                }
                else
                {
                    double dbc=0.0;
                    Get_CompWidthOfDoubleCurvatrue(dForce, dCzm_gap, dArea, dIy, dIz, GLineData, LineBaseD, dbc);
                    dbc_sum += dbc;
                    dbt_sum += dbi-dbc;
                    SubPanelD.db_sup = dbc; // db_sup에 넣자. 
                    SubPanelD.dA_sup = dbc * dti;
                    SubPanelD.db_sup_eff = db_eff;
                }
            }
            else
            {
                dbt_sum += dbi;
            }
        }   
        LineDgnBase.SubPanelD = SubPanelD;
        aPanelDgnBaseD.Add(LineDgnBase); // 압축이든 인장이든 Data는 넣어주자.    

        LineLBD.aDgnBaseD.SetAt(i, LineDgnBase);    
    }

    if(bLineCompStr)
    {
        BOOL bIs1_f1=FALSE;
        double df1_temp=0.0, df2_temp=0.0;
        Get_f1_f2(dStressI, dStressJ, df1_temp, df2_temp, bIs1_f1);
        if(!bLineDbCurve)
        {      
            if(bIs1_f1) iLineStressType = 0;
            else        iLineStressType = 1;
        }
        else
        {
            if(bIs1_f1) iLineStressType = 2;
            else        iLineStressType = 3;
        }
    }
    else
    {
        iLineStressType = -1;
    }

    // Stiffener
    int iDgnLineSerialIx=0;
    CArray<int,int&> aLineDgnIndex4Stfn;
    for(i=0; i<LineDst.aStiffener.GetSize(); i++)
    {
        int iStfnType = LineDst.aStiffener[i].nType;
        if(iStfnType==0 || iStfnType==1)
        {
            aLineDgnIndex4Stfn.Add(i);
        }
        else if(iStfnType==2)
        {
            aLineDgnIndex4Stfn.Add(i);
            aLineDgnIndex4Stfn.Add(i);
        }
        else ASSERT(0);
    }
    double dAsl_sum=0.0;
    double dIsl_sum=0.0;
    double dIsl_sh_sum=0.0;
    double dIp=0.0;
    int iSubPanelSize = aPanelDgnBaseD.GetSize();  // 압축만..
    if(bInternal && iSubPanelSize>1)
    {
        LineLBD.aDgnStfnD.SetSize(iSubPanelSize-1);
        dIp = Get_Ip_4BendingOfPlate(db_t, dt_t, dPoisson);
        for(i=0; i<iSubPanelSize-1; i++)
        {
            int iStfnIx = aLineDgnIndex4Stfn[i];
            T_GLINE_STIFFENER StfnD = LineDst.aStiffener[iStfnIx];
            T_GLINE_DGN_B PreLineDgn = aPanelDgnBaseD[i];
            T_GLINE_DGN_B NextLineDgn = aPanelDgnBaseD[i+1];
            int iPreStrType  = PreLineDgn.SubPanelD.nType;
            int iNextStrType = NextLineDgn.SubPanelD.nType;
            double db_inf=0.0, db_sup=0.0;
            double db_inf_eff=0.0, db_sup_eff=0.0;
            double dt_inf=0.0, dt_sup=0.0;
            if(iPreStrType==Internal_Stress_Type1 || iPreStrType==Internal_Stress_Type2)
            {
                db_inf = PreLineDgn.SubPanelD.db_inf;
                db_sup = NextLineDgn.SubPanelD.db_sup;
                db_inf_eff = PreLineDgn.SubPanelD.db_inf_eff;
                db_sup_eff = NextLineDgn.SubPanelD.db_sup_eff;
                dt_inf = PreLineDgn.dt;
                dt_sup = NextLineDgn.dt;
            }
            else if(iPreStrType==Internal_Stress_Type3)
            {
                db_inf = NextLineDgn.SubPanelD.db_inf;
                db_sup = PreLineDgn.SubPanelD.db_sup;
                db_inf_eff = NextLineDgn.SubPanelD.db_inf_eff;
                db_sup_eff = PreLineDgn.SubPanelD.db_sup_eff;
                dt_inf = NextLineDgn.dt;
                dt_sup = PreLineDgn.dt;
            }
            else
            {
                // 인장. 
            }

            double dfyi = Get_fy4Thick(dfy1, dfy2, dt_t);
            double dEpsilon = Get_Epsilon(dfyi);
            int nPosLine = StfnD.nPosLine; // Stiffener 위치 0:좌측 1:우측 2:양측
            double dt_m = dt_inf;
            double dA_sf = StfnD.dA;
            double dI_sf = StfnD.dIy;        
            if(StfnD.nType==2) // U-rib 경우. 
            {
                dA_sf *= 0.5;
                dI_sf *= 0.5;
            }
            double dZbar_sf = StfnD.dZbar;
            double dA_inf = db_inf*dt_inf;      
            double dA_sup = db_sup*dt_sup;
            double dAsl = dA_inf + dA_sup + dA_sf;
            double dSf_bar = 0.5*dt_m+dZbar_sf;
            double dXsl = dAsl==0.0 ? 0.0 : dSf_bar*dA_sf/dAsl; // 모재 두께의 1/2이 기준. 
            double dI_inf = db_inf*pow(dt_inf,3)/12.0;
            double dI_sup = db_sup*pow(dt_sup,3)/12.0;
            double dIsl = dI_inf+dI_sup+(dA_inf+dA_sup)*pow(dXsl,2) + dI_sf + dA_sf*pow((dSf_bar-dXsl),2);
            double de1 = dXsl;
            double de2 = dSf_bar-dXsl;
            double dIsl_sh = 0.0;

            if(StfnD.nType==0 || StfnD.nType==1)
            {
                Get_IslOfLongStfn4GS(dEpsilon, StfnD, dt_m, dIsl_sh);
            }
            else if(StfnD.nType==2)
            {
                if(i%2==0)
                    Get_IslOfLongStfn4GS(dEpsilon, StfnD, dt_m, dIsl_sh);
            }      

            dAsl_sum += dAsl;
            dIsl_sum += dIsl;
            dIsl_sh_sum += dIsl_sh;

            T_GLINE_DGN_STFN DgnStfn;
            DgnStfn.dA  = dA_sf;
            DgnStfn.dIy = dI_sf;
            DgnStfn.dZbar = dZbar_sf;
            DgnStfn.db_inf = db_inf;
            DgnStfn.db_inf = db_inf_eff;
            DgnStfn.db_sup = db_sup;
            DgnStfn.db_sup_eff = db_sup_eff;
            DgnStfn.dAsl = dAsl;
            DgnStfn.dIsl = dIsl;
            DgnStfn.dIsl_sh = dIsl_sh;
            DgnStfn.dXsl = dXsl;
            DgnStfn.de1 = dXsl;
            DgnStfn.de2 = dSf_bar-dXsl;
            DgnStfn.de = max(de1,de2);
            DgnStfn.dSigma_sl = LineLBD.aDgnBaseD[i].dStr2;

            LineLBD.aDgnStfnD.SetAt(i, DgnStfn);
        }    
    }

    //  
    LineLBD.dalpha = LineDst.db==0.0 ? 0.0 : da/LineDst.db;
    LineLBD.dbc = dbc_sum;
    LineLBD.dbt = dbt_sum;
    LineLBD.dt  = dt_t;
    LineLBD.iLineStressType = iLineStressType;
    LineLBD.iCompPanelSize = iCompPanelSize;
    LineLBD.dAsl_sum = dAsl_sum;
    LineLBD.dIsl_sum = dIsl_sum;
    LineLBD.dIsl_sh_sum = dIsl_sh_sum;
    LineLBD.dAp = dAp_sum;
    LineLBD.dIp = dIp;    

    return TRUE;
}

void CDgnPlateGirder_IRC::Set_CalcSubPanel(BOOL bGrossWidth, int iNtype, double dbb, double dti, T_SUB_PANEL_BASE& rData)
{
    if(bGrossWidth)
    {
        if(iNtype==Stiffened_Plate_inf) 
        {
            rData.db_inf = dbb;
            rData.dA_inf = dbb * dti;
        }
        else if(iNtype==Stiffened_Plate_sup) 
        {
            rData.db_sup = dbb;
            rData.dA_sup = dbb * dti;
        }
        else ASSERT(0);
    }
    else
    {
        if(iNtype==Stiffened_Plate_inf) 
        {
            rData.db_inf_eff = dbb;
            rData.dA_inf_eff = dbb * dti;
        }
        else if(iNtype==Stiffened_Plate_sup) 
        {
            rData.db_sup_eff = dbb;
            rData.dA_sup_eff = dbb * dti;
        }
        else ASSERT(0);
    }
}

int CDgnPlateGirder_IRC::Get_StressType4EffSect(BOOL bCompStr, BOOL bDbCurve, BOOL bIs1_Sig1)
{
    int iStrType=-1;
    if(bCompStr)
    {
        if(!bDbCurve)
        {
            if(bIs1_Sig1) iStrType = 0; // 0=양연압축 이고 dStr1이 f1
            else          iStrType = 1; // 1=양연압축 이고 dStr2이 f1
        }
        else
        {
            if(bIs1_Sig1) iStrType = 2; // 2=양연압축/인장 이고 dStr1이 f1
            else          iStrType = 3; // 3=양연압축/인장 이고 dStr2이 f1
        }
    }
    return iStrType;
}

BOOL CDgnPlateGirder_IRC::Get_CompWidthOfDoubleCurvatrue(T_GLINE_DGN_B& LineDgnB, double& dbcc)
{
    double dStr1 = LineDgnB.dStr1;
    double dStr2 = LineDgnB.dStr2;
    double db = LineDgnB.db;  
    double dy1 = LineDgnB.dy1;
    double dz1 = LineDgnB.dz1;
    double dy2 = LineDgnB.dy2;  
    double dz2 = LineDgnB.dz2;
    double dye = 0.0, dze=0.0;
    if(!Is_DoubleCurvature(dStr1, dStr2)) {return FALSE;}
    double df1 = LineDgnB.df1;
    double df2 = LineDgnB.df2;
    double dbc=0.0, dbt=0.0;
    BOOL bIs1_Sig1=LineDgnB.bIs1_sig1;  

    if(bIs1_Sig1) 
    {
        Get_b1b2DbCurvature(db, df1, df2, dbc, dbt);
        Get_PointCoor4beff(dbc, dy1, dz1, dy2, dz2, dye, dze);    
    }
    else
    {
        Get_b1b2DbCurvature(db, df2, df1, dbc, dbt);      
        Get_PointCoor4beff(dbc, dy2, dz2, dy1, dz1, dye, dze);
    }
    LineDgnB.dy0 = dye;
    LineDgnB.dz0 = dze;

    dbcc = dbc;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_CompWidthOfDoubleCurvatrue(double* dForce, double dCzm_gap, double dArea, double dIy, double dIz, const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, double& dbc)
{
    // b1stIdx : 첫번째 Index부터냐...
    int i=0;
    int iLineSize = LineBaseD.aLineIx.GetSize();  

    double dFx=dForce[0];
    double dMy=dForce[4];
    double dMz=dForce[5];
    //

    T_GSEC_POLYGON GsecPolyD = GLineData.PolyD;
    CArray<T_GSEC_LINE, T_GSEC_LINE&> aLine;
    aLine.Copy(GLineData.aLine);

    int iIx=0, iIx2=0;
    double dbei = 0.0;  

    double dxe=0., dye=0.;
    int v1=0, v2=0, v3=0, v4=0;
    int vcn=0; // Line 2개중 연결된 vertex
    int vI=0, vJ=0;

    BOOL bCheckLine=FALSE;
    int iConnVtx = 0;
    T_GSEC_VERTEX VtxDiv; 
    double dbt=0.0;
    double db1=0.0, db2=0.0;
    BOOL b1stIsComp = TRUE;
    for(i=0; i<iLineSize; i++)
    {    
        int iLix = LineBaseD.aLineIx[i];     
        T_GSEC_LINE LineD = aLine[iLix]; // 순차적으로 연결된 정보이어야함     
        v1 = LineD.v1;
        v2 = LineD.v2;  
        if(i==0)
        {
            vI = LineBaseD.ve1;
            vJ = CSectUtil::Get_NextVertexIndex(vI,v1,v2); // 다른쪽 끝단을 알기위해.
        }
        else
        {
            vI = vJ;
            vJ = CSectUtil::Get_NextVertexIndex(vJ,v1,v2);
        }

        T_GSEC_VERTEX vtx1 = GsecPolyD.aVertex[vI];
        T_GSEC_VERTEX vtx2 = GsecPolyD.aVertex[vJ];
        double dx1 = vtx1.dX;
        double dy1 = vtx1.dY + dCzm_gap;
        double dx2 = vtx2.dX;
        double dy2 = vtx2.dY + dCzm_gap;
        double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
        double dt = LineD.dThik;    
        double df1 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx1, dy1);
        double df2 = Calc_StressOfPosition(dFx, dMy, dMz, dArea, dIy, dIz, dx2, dy2);
        BOOL bCompStr = Is_CompStress(df1, df2);
        BOOL bDbCurve = Is_DoubleCurvature(df1, df2);    


        if(i==0)
        {
            b1stIsComp = df1<0.0 ? TRUE : FALSE;
        }

        if(bDbCurve)
        {        
            Get_b1b2DbCurvature(dbi, df1, df2, db1, db2); 
            if(df1<df2) 
            {
                dbc += db1;
                dbt += db2;
            }
            else 
            {
                dbc += db2;
                dbt += db1;
            }        
        }
        else
        {
            if(bCompStr) dbc += dbi;
            else         dbt += dbi;
        }      
    }

    return TRUE;
}

int CDgnPlateGirder_IRC::Get_ConnectedVertexId(int v1, int v2, int v3, int v4)
{
    int vcn=-1;

    if(v2==v3) vcn=v2;
    else if(v2==v4) vcn=v2;
    else if(v1==v3) vcn=v1;
    else if(v1==v4) vcn=v1;
    else
        ASSERT(0);

    return vcn;
}


BOOL CDgnPlateGirder_IRC::Get_PointCoor4beff(double dbe, double dx1, double dy1, double dx2, double dy2, double& dx, double& dy)
{
    // x1, y1이 좌표는 계산하고자 하는 시작점의 좌표로..
    double dX = dx2 - dx1;
    double dY = dy2 - dy1;
    double dX_len = fabs(dX);
    double dY_len = fabs(dY);
    double da = 0.0; // 기울기
    double db = 0.0; // y절편
    double dLength = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
    if(fabs(dX) < m_dZero) // 수직.
    {
        if(dy1>dy2)
        { 
            dx = dx1;
            dy = dy1 - dbe;
        }
        else
        {
            dx = dx1;
            dy = dy1 + dbe;
        }
    }
    else
    {
        da = dY/dX;
        db = dy1 - da*dx1; // y=ax+b -> b=y-ax
        if(da>0) // 기울기 +
        { 
            double dtheta = atan(fabs(da));
            double dx_len = dbe*cos(dtheta);
            if(dx1<dx2) // 시작점이 왼쪽.
            {
                //double dLen1 = dLength - dbe;
                //double dx_len = Get_traiangleRatioVal(dLen1, dLength, dX_len);
                dx = dx1 + dx_len;        
            }
            else // 시작점이 오른쪽
            {
                //double dx_len = Get_traiangleRatioVal(dbe, dLength, dX_len);
                dx = dx1 - dx_len;
            }
            dy = Get_yOfStrightLine(da, db, dx);
        }
        else if(da<0) // 기울기 -
        {
            double dtheta = atan(fabs(da));
            double dx_len = dbe*cos(dtheta);
            if(dx1<dx2) // 시작점이 왼쪽.
            {
                //double dx_len = Get_traiangleRatioVal(dbe, dLength, dX_len);
                //dx = dx1 + dx_len;
                dx = dx1 + dx_len;
            }
            else // 시작점이 오른쪽
            {
                double dLen1 = dLength - dbe;
                //double dx_len = Get_traiangleRatioVal(dLen1, dLength, dX_len);
                dx = dx1 - dx_len;        
            }
            dy = Get_yOfStrightLine(da, db, dx);
        }
        else // 수평
        {
            if(dx1<dx2)
            {
                dy = dy1;
                dx = dx1 + dbe;
            }
            else
            {
                dy = dy1;
                dx = dx1 - dbe;
            }
        }
    }  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_CompLengthOfPlastic(BOOL bPositive, double dy1, double dz1, double dy2, double dz2, double& dLen_c, double& dLen_t)
{
    double dc=0.0, dt=0.0;
    double dLen = CMathFunc::mathLength(dy1, dz1, dy2, dz2);
    if(dz1*dz2<0.0) // 
    {
        if(dz1>0.0)
        {
            dc = dLen*fabs(dz1)/(fabs(dz1)+fabs(dz2));
            dt = dLen - dc;
        }
        else
        {
            dt = dLen*fabs(dz1)/(fabs(dz1)+fabs(dz2));
            dc = dLen - dt;
        }
    }
    else if(dz1*dz2>0.0)
    {
        if(dz1>0.0) 
        {
            dc = dLen; 
            dt =0.0;
        }
        else
        { 
            dc = 0.0;
            dt = dLen; 
        }
    }
    else
    {
        if(dz1*dz2==0.0)
        {
        }
        else if(dz1==0.0)
        {
            if(dz2>0.0) { dc = dLen; dt = 0.0; }        
            else        { dc = 0.0;  dt = dLen; }        
        }
        else if(dz2==0.0)
        {
            if(dz1>0.0) { dc = dLen; dt = 0.0; }        
            else        { dc = 0.0;  dt = dLen; }        
        }
    }

    if(bPositive)
    {
        dLen_c = dc;
        dLen_t = dt;
    }
    else
    {
        dLen_c = dt;
        dLen_t = dc;
    }

    return TRUE;
}


double CDgnPlateGirder_IRC::Get_Psi(double dSig1, double dSig2)
{
    double dPsi = dSig1==0.0 ? dSig2/m_dZero : dSig2/dSig1;

    return dPsi;
}

BOOL CDgnPlateGirder_IRC::Get_Psi(double dStr1, double dStr2, double& dSig1, double& dSig2, double& dPsi)
{
    BOOL bIs1_Sig1=FALSE;
    Get_f1_f2(dStr1, dStr2, dSig1, dSig2, bIs1_Sig1);
    dPsi = Get_Psi(dSig1, dSig2);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_GLineDgnPartData(const T_GLINE_DGN_D& GLineDgnD, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aTopFlangePart, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aBotFlangePart, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aWebPart)
{
    aTopFlangePart.RemoveAll();
    aBotFlangePart.RemoveAll();
    aWebPart.RemoveAll();
    int i=0;
    int iPartLineSize = GLineDgnD.aPartLine.GetSize();
    for(i=0; i<iPartLineSize; i++)
    {
        T_GLINE_PART_DGN GLinePartD = GLineDgnD.aPartLine[i];
        int nPartType = GLinePartD.nPartType;
        if     (nPartType==0) aTopFlangePart.Add(GLinePartD);
        else if(nPartType==1) aBotFlangePart.Add(GLinePartD);
        else if(nPartType==2) aWebPart.Add(GLinePartD);    
        else ASSERT(0);
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_DLinePartData(const T_DLINE_CALC_D& DLineInfo, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aTopFlangePart, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aBotFlangePart, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aWebPart)
{
    aTopFlangePart.RemoveAll();
    aBotFlangePart.RemoveAll();
    aWebPart.RemoveAll();
    int i=0;
    int iPartLineSize = DLineInfo.aPartLine.GetSize();
    for(i=0; i<iPartLineSize; i++)
    {
        T_DLINE_PART_D DLinePartD = DLineInfo.aPartLine[i];
        int nPartType = DLinePartD.nPartType;
        if     (nPartType==0) aTopFlangePart.Add(DLinePartD);
        else if(nPartType==1) aBotFlangePart.Add(DLinePartD);
        else if(nPartType==2) aWebPart.Add(DLinePartD);    
        else ASSERT(0);
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_CompositeGeneralSectionInfo(BOOL bPosiI, const T_SECT_D& SectD, double& dZbar_sb, double& dCzm_sb, double& dCzp_sb, double& dHt, double& dArea_sb, double& dBc_sb, double* aYcr, double* aZcr)
{
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    T_SECT_SECTBASE_D SectBaseD = bTapered && !bPosiI ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI;
    int nBeforePart = SectBaseD.nBeforePart; // (General Composite) Before 의 Part 수
    int iPartSize = SectBaseD.aGeneralPart.GetSize();
    //T_SECT_STIFFNESS GirderStiffness = SectBaseD.aGeneralPart[nBeforePart-1].StiffnessSum;
    //double dCzp_g = GirderStiffness.Czp;
    //double dCzm_g = GirderStiffness.Czm;
    //double dHg = dCzp_g + dCzm_g;

    double aMaxPt[2]={0.0}, aMinPt[2]={0.0};
    CSectUtil::Get_MaxMinPoint4GenPart(SectBaseD, aMaxPt, aMinPt);

    double dArea_slab=0.0;
    double dZbar_slab=0.0;
    double dCzp_slab=0.0, dCzm_slab=0.0;
    double dCyp_slab=0.0, dCym_slab=0.0;
    for(int i=0; i<iPartSize; i++)
    {
        T_GSEC_PART PartD = SectBaseD.aGeneralPart[i];
        if(PartD.bPlane)
        {
            dArea_slab = PartD.Stiffness.Area;
            dZbar_slab = PartD.ZBar;
            dCyp_slab = PartD.Stiffness.Cyp;
            dCym_slab = PartD.Stiffness.Cym;
            dCzp_slab = PartD.Stiffness.Czp;
            dCzm_slab = PartD.Stiffness.Czm;
            if(aYcr!=NULL && aZcr!=NULL)
            {
                aYcr[0] = PartD.Stiffness.dx1, aZcr[0] = PartD.Stiffness.dy1;
                aYcr[1] = PartD.Stiffness.dx2, aZcr[1] = PartD.Stiffness.dy2;
                aYcr[2] = PartD.Stiffness.dx3, aZcr[2] = PartD.Stiffness.dy3;
                aYcr[3] = PartD.Stiffness.dx4, aZcr[3] = PartD.Stiffness.dy4;
            }      
            break;
        }    
    }  

    dZbar_sb = dZbar_slab;
    dCzm_sb = dCzm_slab;
    dCzp_sb = dCzp_slab;
    dArea_sb = dArea_slab;
    dHt = fabs(aMaxPt[1]-aMinPt[1]);
    dBc_sb = dCym_slab + dCyp_slab;

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_Height4Gen(BOOL bPosiI)
{
    double dHt = bPosiI ? m_dHt[0] : m_dHt[1];
    return dHt;
}

double CDgnPlateGirder_IRC::Get_ZcentroidofSlab(BOOL bPosiI)
{
    double dZbar_slab = bPosiI ? m_dZbar_sb[0] : m_dZbar_sb[1];
    return dZbar_slab;
}
double CDgnPlateGirder_IRC::Get_CzmofSlab(BOOL bPosiI)
{
    double dCzm_slab = bPosiI ? m_dCzm_sb[0] : m_dCzm_sb[1];
    return dCzm_slab;
}
double CDgnPlateGirder_IRC::Get_CzpofSlab(BOOL bPosiI)
{
    double dCzp_slab = bPosiI ? m_dCzp_sb[0] : m_dCzp_sb[1];
    return dCzp_slab;
}
double CDgnPlateGirder_IRC::Get_AreaofSlab(BOOL bPosiI)
{
    double dArea_slab = bPosiI ? m_dArea_sb[0] : m_dArea_sb[1];
    return dArea_slab;
}
double CDgnPlateGirder_IRC::Get_BcofSlab(BOOL bPosiI)
{
    double dBc_slab = bPosiI ? m_dBc_sb[0] : m_dBc_sb[1];
    return dBc_slab;  
}


void CDgnPlateGirder_IRC::Set_LineDgnBase(int iLineType, double db, double dt, double dy1, double dz1, double dy2, double dz2, double df1, double df2,
                                          BOOL bCompStr, BOOL bDbCurve, BOOL bIs1_sig1, double dy0, double dz0, T_GLINE_DGN_B& LineDgnBase)
{
    LineDgnBase.iLineType = iLineType;
    LineDgnBase.db = db;
    LineDgnBase.dt = dt;
    LineDgnBase.dy1 = dy1;
    LineDgnBase.dz1 = dz1;
    LineDgnBase.dy2 = dy2;
    LineDgnBase.dz2 = dz2;
    LineDgnBase.df1 = df1;
    LineDgnBase.df2 = df2;
    LineDgnBase.bCompStr = bCompStr;
    LineDgnBase.bDbCurve = bDbCurve;
    LineDgnBase.bIs1_sig1 = bIs1_sig1;
    LineDgnBase.dy0 = dy0;
    LineDgnBase.dz0 = dz0;    
}


BOOL CDgnPlateGirder_IRC::Calc_Vb_Rd(bool bPosiI, CPG_MATL_ELEM& MatlElem, T_SECT_D& SectD,  CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,  
                                     CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, T_PRVS_BASE& PrvsBase, T_CLAS_BASE& ClassD, 
                                     double& dVb_Rd, T_VERTICAL_SHAER_RES_D_IRC& ResD)
{
    // 1. Initailize
    dVb_Rd = 0.0;

    // 2. Get Basic Input Data
    int iMatClass	= MatlElem.iMatClass;
    double dgam_M1	= m_dGamma_M1;
    double dhw		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dfy		= MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfyw		= MatlElem.MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;Get_fy4Thick(dfy, dfy1, dTw);
    double dEta		= Get_Eta4SteelGrade(iMatClass, dfy);

    // 2. Calc. Vbw_Rd (Contribution of web)
    double dVbw_Rd	 = 0.0;
    if(!Calc_Vbw_Rd(bPosiI, MatlElem, SectD, arLstiPosi, TrstPosi, TsesPosi, dVbw_Rd, ResD)){ASSERT(0);}

    // 3. Calc. Vbf_Rd (Contribution of flange)
    double dVbf_Rd = 0.0;
    //if(!Calc_Vbf_Rd(bPosiI, MatlElem.MatdD, SectD, PrvsBase, ClassD, dVbf_Rd, ResD)){ASSERT(0);}	

    // 4. Calc. Vb_Rd
    double dVb_Rd_lim = 0.0, dVb_Rd_org = 0.0;
    dVb_Rd = dVb_Rd_org = dVbw_Rd + dVbf_Rd;

    // 5. Check Limit Shear Buckling Resistance Value Cl.509.4.1 pg 90
    double dVb_Rd_Lim =  dfyw*dhw*dTw_t/(sqrt(3.0));

    if(dVb_Rd > dVb_Rd_Lim){dVb_Rd = dVb_Rd_Lim;}

    // 6. Save Result Data
    ResD.ShearResD[0].dVb_Rd_org		= dVb_Rd_org;
    ResD.ShearResD[0].dVb_Rd_Lim		= dVb_Rd_Lim;
    ResD.ShearResD[1].dVb_Rd_org		= dVb_Rd_org;
    ResD.ShearResD[1].dVb_Rd_Lim		= dVb_Rd_Lim;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_Vbw_Rd(bool bPosiI, CPG_MATL_ELEM& MatlElem, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,  
                                      CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, double& dVbw_Rd, 
                                      T_VERTICAL_SHAER_RES_D_IRC& ResD)
{
    // 1. Initailize & Get Basic Input Data
    dVbw_Rd			= 0.0;
    bool bBoxSect	= m_pDgnSectUtil->Is_StlBoxSect(&SectD);
    int iMatClass	= MatlElem.iMatClass;
    int iEndpost	= TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post
    double dhw		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dTw_t	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_BOTH, bPosiI);
    double dfy		= MatlElem.MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatlElem.MatdD.Data1.Design.S_Fy2;
    double dfyw		= MatlElem.MatdD.Data1.DesignWeb.S_Fy1 ; //Get_fy4Thick(dfy, dfy1, dTw);
    double dEta		= Get_Eta4SteelGrade(iMatClass, dfy);

    T_DGN_STL_STIFFENER_PART WebStiffD;
    m_pDgnSectUtil->Get_Web_StiffenerD(true, &SectD, WebStiffD);
    Get_ConvertSectStiffenerD(WebStiffD, arLstiPosi);

    // 2. Calc. Lambda_w
    double dLambda_w = 0.0;
    if(!Calc_Lambda_w(bPosiI, MatlElem, SectD, arLstiPosi, TrstPosi, TsesPosi,  dLambda_w, ResD.ShearResD[0])){ASSERT(0);} // Left Web

    // 3 . Calc Xi Factor
    int iLamda_w_scope[2]	= {0,0};
    double dXw[2]			= {0.0, 0.0};

    if(!Get_ContributionFromWebFactor(iEndpost, dEta, dLambda_w, dXw[0], iLamda_w_scope[0],dfyw)){ASSERT(0);}

    // 4. Calc. Vbw_Rd
    double dVbw_Rd_LR[2] = {0.0, 0.0}; // Left Web, Right Web
    double dgam_M1 = m_dGamma_M1;
    //dVbw_Rd_LR[0] = fabs(dgam_M1) < m_dZero ? 0.0 : dXw[0]*dfyw*dhw*dTw/(sqrt(3.0)*dgam_M1);
    dVbw_Rd_LR[0] = fabs(dgam_M1) < m_dZero ? 0.0 : dXw[0]*dhw*dTw;
    // Repeat for right web
    if(bBoxSect) // Right Web
    {
        WebStiffD.Initialize(); arLstiPosi.RemoveAll();
        m_pDgnSectUtil->Get_Web_StiffenerD(false, &SectD, WebStiffD);
        Get_ConvertSectStiffenerD(WebStiffD, arLstiPosi);

        if(!Calc_Lambda_w(bPosiI, MatlElem, SectD, arLstiPosi, TrstPosi, TsesPosi,  dLambda_w, ResD.ShearResD[1])){ASSERT(0);}
        if(!Get_ContributionFromWebFactor(iEndpost, dEta, dLambda_w, dXw[1], iLamda_w_scope[1],dfyw)){ASSERT(0);}
        dVbw_Rd_LR[1] = fabs(dgam_M1) < m_dZero ? 0.0 : dXw[1]*dhw*dTw;
    }

    dVbw_Rd = dVbw_Rd_LR[0] + dVbw_Rd_LR[1];

    // 5. Save Result
    ResD.dVbw_Rd					= dVbw_Rd;

    // Save for Left Web Part
    ResD.ShearResD[0].dVbw_Rd		= dVbw_Rd_LR[0];
    ResD.ShearResD[0].dXw			= dXw[0];
    ResD.ShearResD[0].iEndpost		= iEndpost;
    ResD.ShearResD[0].iLamda_w_scope= iLamda_w_scope[0];

    if(bBoxSect) 	// Save for Right Web Part
    {
        ResD.ShearResD[1].dVbw_Rd		= dVbw_Rd_LR[1];
        ResD.ShearResD[1].dXw			= dXw[1];
        ResD.ShearResD[1].iEndpost		= iEndpost;
        ResD.ShearResD[1].iLamda_w_scope= iLamda_w_scope[1];
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_Vbf_Rd(bool bPosiI,T_MATD_D& MatdD, T_SECT_D& SectD, T_PRVS_BASE& PrvsBase, T_CLAS_BASE& ClassD,
                                      double& dVbf_Rd, T_VERTICAL_SHAER_RES_D_IRC& ResD)
{
    // 1. Initailize
    dVbf_Rd = 0.0;
    double dVbf_Rd_LR[2] = {0.0, 0.0};

    // 2. Get Input Data
    T_PGBR_BASE PgbrBase = m_PgbrBase;  
    bool bBoxSect	= m_pDgnSectUtil->Is_StlBoxSect(&SectD);
    double dhw_in	= m_pDgnSectUtil->Get_Size_Inclind_hw(&SectD, bPosiI);
    double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1		= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTF1		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2		= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTF2		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dgam_M1	= m_dGamma_M1;
    double dgam_M2	= m_dGamma_M0;
    double dEpsi	= Get_Epsilon(dfyw);	

    // EN 1993-1-5 7.1 (28page)
    // 7.1 Interaction between shear force, bending moment and axial force
    double dMf_Rd	= PgbrBase.dMf_Rd;
    double dMpl_Rd	= PgbrBase.dMpl_Rd_s; // Bending의 Mpl_Rd와 다를 수 있다.
    double dM_Ed	= m_PgbrDetail.dM_Ed;
    double dN_Ed	= PrvsBase.dFx;	
    double dV_Ed	= PrvsBase.dV_Ed;
    double dFactor_fr = 1.0;

    // In case of compression  
    double dAf_top	= dB1*dTF1;
    double dAf_bot	= dB2*dTF2;
    if(dN_Ed < 0.0) {dFactor_fr = Get_ReductonFactorOfFlange4N_Ed(dgam_M2, dfyt, dfyb, dN_Ed, dAf_top, dAf_bot);}
    double dMf_Rd_factor = dFactor_fr * dMf_Rd;

    //
    int iClassSect			= ClassD.iClassSect;
    double dVbw_Rd			= ResD.dVbw_Rd;
    double dBendingRat		= fabs(dMpl_Rd) < m_dZero ? 0.0 : dMf_Rd_factor/dMpl_Rd;
    double dBar_Eta1		= fabs(dMpl_Rd) < m_dZero ? 0.0 : fabs(dM_Ed)/dMpl_Rd; 
    double dBar_Eta1_Lim	= dBendingRat;
    double dBar_Eta3		= fabs(dVbw_Rd) < m_dZero ? 0.0 : fabs(dV_Ed)/dVbw_Rd;   

    //
    double dCombinedEffe = dBar_Eta1+(1.0-dBendingRat)*pow((2.0*dBar_Eta3-1.0),2);  

    BOOL bEta3Exceed05 = FALSE, bCombinedEffe = FALSE, bEta1Ratio = FALSE;
    if(dBar_Eta3 > 0.5){bEta3Exceed05 = TRUE;}

    if(dBar_Eta1 > dBendingRat - m_dZero)
    {
        bEta1Ratio = TRUE;
        if(dCombinedEffe < 1.0 + m_dZero){bCombinedEffe = TRUE;}
    }
    else
    {
        bCombinedEffe = TRUE;
    }

    // Contribution from flange, EN 1993-1-5 , 25page
    double dbf[2]	= {0.0, 0.0};
    double dtf		= 0.0;
    double dfyf		= 0.0;
    bool bTopFlange = true;
    if(dAf_top < dAf_bot + m_dZero)
    {
        bTopFlange	= true;
        dtf			=  m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
        dfyf		= dfyt; // Modify by GAY. PMS:4216. ('12.05.17). Design strength -> Nominal strength.
    }
    else
    {
        bTopFlange	= false;
        dtf			=  m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
        dfyf		= dfyb; // Modify by GAY. PMS:4216. ('12.05.17). Design strength -> Nominal strength.
    }

    Get_bf4c_DB(bPosiI, SectD, bTopFlange, 0, dEpsi, dbf[0]);

    double da	= ResD.ShearResD[0].dDist4Shr;  // _T("a") is the distance between transverse stiffeners 
    double dc_LR[2]={0.0, 0.0};

    dc_LR[0] = (dTw* dhw_in *dfyw < m_dZero) ? 0.0 : da*(0.25+1.6*dbf[0]*pow(dtf,2)*dfyf/(dTw*pow(dhw_in,2)*dfyw));

    if(fabs(dMf_Rd_factor) > m_dZero && dM_Ed < dMf_Rd_factor)
    {
        dVbf_Rd_LR[0] = fabs(dc_LR[0]*dgam_M1) < m_dZero ? 0.0 : dbf[0]*pow(dtf,2)*dfyf/(dc_LR[0]*dgam_M1)*(1.0-pow(dM_Ed/dMf_Rd_factor,2));
    }
    else // Do not consider shear buckling resistance of flange's contribution
    {
        dVbf_Rd_LR[0] = 0.0;
    }

    if(bBoxSect)
    {
        Get_bf4c_DB(bPosiI, SectD, bTopFlange, 1, dEpsi, dbf[1]);

        dc_LR[1] = (dTw* dhw_in *dfyw < m_dZero) ? 0.0 : da*(0.25+1.6*dbf[1]*pow(dtf,2)*dfyf/(dTw*pow(dhw_in,2)*dfyw));

        if(fabs(dMf_Rd_factor) > m_dZero && dM_Ed < dMf_Rd_factor)
        {
            dVbf_Rd_LR[1] = fabs(dc_LR[1]*dgam_M1) < m_dZero ? 0.0 : dbf[1]*pow(dtf,2)*dfyf/(dc_LR[1]*dgam_M1)*(1.0-pow(dM_Ed/dMf_Rd_factor,2));
        }
        else // Do not consider shear buckling resistance of flange's contribution
        {
            dVbf_Rd_LR[1] = 0.0;
        }
    }

    dVbf_Rd = dVbf_Rd_LR[0] + dVbf_Rd_LR[1];
    dVbf_Rd = 0.0;//IRC 24 does not consider shear resisted by flanges so considering it as 0
    // Save Result
    ResD.dVbf_Rd = dVbf_Rd;

    ResD.ShearResD[0].bEta3Exceed05	= bEta3Exceed05;
    ResD.ShearResD[0].bCombinedEffe	= bCombinedEffe;
    ResD.ShearResD[0].dVbf_Rd		= dVbf_Rd_LR[0];
    ResD.ShearResD[0].dM_Ed			= dM_Ed;
    ResD.ShearResD[0].dMf_Rd		= dMf_Rd;
    ResD.ShearResD[0].dFactor_fr	= dFactor_fr;
    ResD.ShearResD[0].dMf_Rd_factor	= dMf_Rd_factor;
    ResD.ShearResD[0].dMpl_Rd		= dMpl_Rd;
    ResD.ShearResD[0].dBar_Eta1		= dBar_Eta1;
    ResD.ShearResD[0].dBar_Eta1_Lim	= dBar_Eta1_Lim;
    ResD.ShearResD[0].dBar_Eta3		= dBar_Eta3	;
    ResD.ShearResD[0].dCombinedEffe	= dCombinedEffe;
    ResD.ShearResD[0].dbf			= dbf[0];
    ResD.ShearResD[0].dtf			= dtf;
    ResD.ShearResD[0].dc			= dc_LR[0];

    if(bBoxSect)
    {
        ResD.ShearResD[1].bEta3Exceed05	= bEta3Exceed05;
        ResD.ShearResD[1].bCombinedEffe	= bCombinedEffe;
        ResD.ShearResD[1].dVbf_Rd		= dVbf_Rd_LR[1];
        ResD.ShearResD[1].dM_Ed			= dM_Ed;
        ResD.ShearResD[1].dMf_Rd		= dMf_Rd;
        ResD.ShearResD[1].dFactor_fr	= dFactor_fr;
        ResD.ShearResD[1].dMf_Rd_factor	= dMf_Rd_factor;
        ResD.ShearResD[1].dMpl_Rd		= dMpl_Rd;
        ResD.ShearResD[1].dBar_Eta1		= dBar_Eta1;
        ResD.ShearResD[1].dBar_Eta1_Lim	= dBar_Eta1_Lim;
        ResD.ShearResD[1].dBar_Eta3		= dBar_Eta3	;
        ResD.ShearResD[1].dCombinedEffe	= dCombinedEffe;
        ResD.ShearResD[1].dbf			= dbf[1];
        ResD.ShearResD[1].dtf			= dtf;
        ResD.ShearResD[1].dc			= dc_LR[1];
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Calc_Lambda_w(bool bPosiI, CPG_MATL_ELEM& MatdElemD, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,  
                                        CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, double& dLambda_w, 
                                        T_VERTICAL_SHAER_RES_UNIT_D_IRC& ResD)
{
    // 1. Initailize
    dLambda_w	= 0.0;

    // 2. Get Input Data
    int iEndpost			= TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post
    int iLongStiffSize		= arLstiPosi.GetSize();
    int iMatClass			= MatdElemD.iMatClass;
    BOOL bPartLongStiff		= iLongStiffSize > 0 ? TRUE : FALSE;
    BOOL bLongStiff			= iLongStiffSize == 0 ? FALSE : TRUE;
    BOOL bEndSupport		= TsesPosi.bUseStif;
    BOOL bTranStiff			= TrstPosi.bUseStif;
    BOOL bInterRigidStif	= TrstPosi.bInterRigidStif;
    BOOL bInterNonRigidStif = TrstPosi.bInterNonRigidStif;
    double dCosA			= fabs(cos(m_pDgnSectUtil->Get_Size_WebSlope(&SectD, 0, bPosiI)));
    double dhw				= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dhw_in			= m_pDgnSectUtil->Get_Size_Inclind_hw(&SectD, bPosiI);
    double dTw				= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dfy				= MatdElemD.MatdD.Data1.Design.S_Fy1;
    double dfy1				= MatdElemD.MatdD.Data1.Design.S_Fy2;
    double dfyw				= MatdElemD.MatdD.Data1.DesignWeb.S_Fy1 ; //Get_fy4Thick(dfy, dfy1, dTw);
    double dEpsi			= Get_Epsilon(dfyw);
    double dInterRigPitch	= TrstPosi.dDistRstif;
    double dNonRigPitch		= TrstPosi.dDistNRstif;
    double dRigidPitch		= TrstPosi.dPitch;
    double dEta				= Get_Eta4SteelGrade(iMatClass, dfy);

    // 2. a // 5.3(3) The slenderness parameter (Lamda_w) , EN 1993-1-5 : 2004, 23page
    double dMinValue		= -1.0/pow(m_dZero,10);
    double dMax_Lamda_wi	= dMinValue;
    double dMax_Bar_Lamda_w = dMinValue;

    // Calc. Isl
    double dIsl = 0.0, dIsl_org = 0.0;

    for(int i=0; i < iLongStiffSize; i++)
    {
        CPG_LSTI_DATA& LstiD = arLstiPosi[i];
        double dh_ls = LstiD.dWidth;
        double dt_ls = LstiD.dt;
        double dI_ls = 0.0;
        if(!Get_IslOfLongiStiff(LstiD, dTw, dEpsi, dI_ls)){dI_ls = 0.0;}
        dIsl += dI_ls;
    }  	
    dIsl_org = dIsl; 
    dIsl = 1.0/3.0*dIsl;// 5.3(4) The second moment of area of a longitudinal stiffener should be reduced to 1/3 of their actual value when calculating k_tau

    // Check the Whole Web
    BOOL bIsShearBuck = FALSE;
    double dk_tau_p = 0.0, dk_tau_st_p = 0.0, dk_tau_st_Limit_p = 0.0, dLamda_w_part = 0.0, dCheck_lim = 0.0;

    int iLamda_w_Type = Get_Lamda_w_Type4Shear(bPartLongStiff , TrstPosi);//2 or 1
    BOOL bStiffened = (iLamda_w_Type == 1 || iLamda_w_Type == 2);  

    if(Get_SlendernessParamLamda(iLamda_w_Type, iLongStiffSize, dhw_in, dRigidPitch, dTw, dIsl, dEpsi, dk_tau_p, dk_tau_st_p, 
        dk_tau_st_Limit_p, dLamda_w_part)){dMax_Bar_Lamda_w = dLamda_w_part;}
    else{ASSERT(0);}

    int nShearBuckCount = 0;
    if(!Get_WidthThickRatio4Web(bStiffened, dEta, dhw_in, dTw, dEpsi, dk_tau_p, dCheck_lim, bIsShearBuck)){ASSERT(0);}
    if(bIsShearBuck) {nShearBuckCount++;}	

    // Check
    int nLSsize_cr = 0;
    double dLamda_wi = 0.0, dhwi = 0.0, dai  = 0.0, dk_taui = 0.0, dk_tau_st_i = 0.0, dk_tau_st_Limit_i = 0.0, dDist4Shr = 0.0;
    double dhi_cr = 0.0, dai_cr = 0.0, dk_tau_si_cr = 0.0 ,dk_tau_sti_cr = 0.0, dk_tau_st_lti_cr = 0.0, dIsli_cr = 0.0;	

    if(!bInterRigidStif && !bInterNonRigidStif) // Case 1.
    {
        Get_SlendernessParamLamda(iLamda_w_Type, iLongStiffSize, dhw_in, dRigidPitch, dTw, dIsl, dEpsi, dk_taui, dk_tau_st_i, 
            dk_tau_st_Limit_i, dLamda_wi);
        if(dMax_Lamda_wi < dLamda_wi) 
        {                    
            dMax_Lamda_wi		= dLamda_wi;
            dhi_cr				= dhwi;
            dai_cr				= dRigidPitch;
            dk_tau_si_cr		= dk_taui;
            dk_tau_sti_cr		= dk_tau_st_i;
            nLSsize_cr			= iLongStiffSize;
            dk_tau_st_lti_cr	= dk_tau_st_Limit_i;
            dIsli_cr			= dIsl;
        }
        dDist4Shr = dRigidPitch;
    }
    else if(!bInterRigidStif && bInterNonRigidStif) // Case 2.
    {
        double dDist1 = dNonRigPitch;
        double dDist2 = dRigidPitch-dNonRigPitch;

        for(int j=0; j<2; j++) // Column
        {          
            dai = j==0 ? dDist1 : dDist2; 
            dk_taui = 0.0;
            dLamda_wi = 0.0;
            Get_SlendernessParamLamda(2, iLongStiffSize, dhw_in, dai, dTw, dIsl, dEpsi, dk_taui, dk_tau_st_i, dk_tau_st_Limit_i, dLamda_wi);                
            if(dMax_Lamda_wi < dLamda_wi) 
            {                    
                dMax_Lamda_wi		= dLamda_wi;
                dhi_cr				= dhwi;
                dai_cr				= dai;
                dk_tau_si_cr		= dk_taui;
                dk_tau_sti_cr		= dk_tau_st_i;
                nLSsize_cr			= iLongStiffSize;
                dk_tau_st_lti_cr	= dk_tau_st_Limit_i;
                dIsli_cr			= dIsl;
            }
        }          
        dDist4Shr = max(dDist1, dDist2);
    }
    else if(bInterRigidStif && !bInterNonRigidStif) // Case 3.
    {
        double dDist1 = dInterRigPitch;
        double dDist2 = dRigidPitch-dInterRigPitch;

        for(int j=0; j<2; j++) // Column
        {          
            dai = j==0 ? dDist1 : dDist2; 
            dk_taui = 0.0;
            dLamda_wi = 0.0;
            Get_SlendernessParamLamda(2,iLongStiffSize,dhw_in,dai,dTw,dIsl,dEpsi,dk_taui,dk_tau_st_i, dk_tau_st_Limit_i,dLamda_wi);  

            if(dMax_Lamda_wi<dLamda_wi) 
            {                    
                dMax_Lamda_wi		= dLamda_wi;
                dhi_cr				= dhwi;
                dai_cr				= dai;
                dk_tau_si_cr		= dk_taui;
                dk_tau_sti_cr		= dk_tau_st_i;
                nLSsize_cr			= iLongStiffSize;
                dk_tau_st_lti_cr	= dk_tau_st_Limit_i;
                dIsli_cr			= dIsl;
            }
        }
        dDist4Shr = max(dDist1, dDist2);
    }
    else if(bInterRigidStif && bInterNonRigidStif) // Case 4.
    {
        for(int j=0; j < 4; j++) // Column
        {        
            if(dInterRigPitch < dNonRigPitch + m_dZero) // Inter-Non-rigid가 Inter-rigid보다 멀리 있다.
            {
                if(j==0)		{dai = dInterRigPitch;					} // a1
                else if(j==1)	{dai = dNonRigPitch - dInterRigPitch;	} // a2
                else if(j==2)	{dai = dRigidPitch - dNonRigPitch;		} // a3
                else			{dai = dRigidPitch - dInterRigPitch;	} // a4
            }
            else
            {
                if(j==0)		{dai = dNonRigPitch;					} // a1
                else if(j==1)	{dai = dInterRigPitch - dNonRigPitch;	} // a2
                else if(j==2)	{dai = dRigidPitch - dInterRigPitch;	} // a3
                else			{dai = dInterRigPitch;					} // a4
            }

            dk_taui = 0.0;
            dLamda_wi = 0.0;
            Get_SlendernessParamLamda(2,iLongStiffSize,dhw_in,dai,dTw,dIsl,dEpsi,dk_taui,dk_tau_st_i, dk_tau_st_Limit_i,dLamda_wi); 

            if(dMax_Lamda_wi<dLamda_wi) 
            {
                dMax_Lamda_wi		= dLamda_wi;
                dhi_cr				= dhwi;
                dai_cr				= dai;
                dk_tau_si_cr		= dk_taui;
                dk_tau_sti_cr		= dk_tau_st_i;
                nLSsize_cr			= iLongStiffSize;
                dk_tau_st_lti_cr	= dk_tau_st_Limit_i;
                dIsli_cr			= dIsl;
            }
        }

        double dDist[3] ={0.0, 0.0, 0.0};

        for(int j=0; j<3; j++)
        {
            if(dInterRigPitch < dNonRigPitch + m_dZero) // Inter-Non-rigid가 Inter-rigid보다 멀리 있다.
            {
                if(j==0)		{dDist[0] = dInterRigPitch;					} // a1
                else if(j==1)	{dDist[1] = dNonRigPitch - dInterRigPitch;	} // a2
                else if(j==2)	{dDist[2] = dRigidPitch - dNonRigPitch;		} // a3
            }
            else if(dInterRigPitch>dNonRigPitch)
            {
                if(j==0)		{dDist[0] = dNonRigPitch;					} // a1
                else if(j==1)	{dDist[1] = dInterRigPitch - dNonRigPitch;	} // a2
                else if(j==2)	{dDist[2] = dRigidPitch - dInterRigPitch;	} // a3
            }
        }
        //
        double dMaxDist=0;
        for(int j=0; j<3; j++)
        {
            if(j==0)
            {
                dMaxDist = dDist[0];
            }
            else
            {
                if(dMaxDist < dDist[j]) {dMaxDist = dDist[j];}        
            }
        }
        dDist4Shr = dMaxDist;
    }        

    // 5.3(5) For webs with longitudinal stiffeners the slenderness parameter (lamda_w) in (3)
    // should not be taken as less than , EN 1993-1-5 : 2004, 24page
    if(bLongStiff && bTranStiff)
    {
        int iLongStiffSize_sub = 0;
        double dIsl_i = 0.0;

        for(int i=0; i < iLongStiffSize + 1; i++)
        {
            if(i==0)
            {
                dhwi = dCosA < m_dZero ? 0.0 : arLstiPosi[i].dz / dCosA;
            }
            else if(i > 0 && i < iLongStiffSize)
            {
                dhwi = dCosA < m_dZero ? 0.0 : (arLstiPosi[i].dz - arLstiPosi[i-1].dz) / dCosA; 
            }
            else
            {
                dhwi = dCosA < m_dZero ? 0.0 : (dhw - arLstiPosi[i-1].dz) / dCosA;
            }

            if(!bInterRigidStif && !bInterNonRigidStif) // Case 1.
            {
                dai = dRigidPitch;
                dk_taui = 0.0, dLamda_wi = 0.0;
                Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi, dk_taui, dk_tau_st_i, 
                    dk_tau_st_Limit_i, dLamda_wi);                

                if(dMax_Lamda_wi < dLamda_wi) 
                {
                    dMax_Lamda_wi		= dLamda_wi;
                    dhi_cr				= dhwi;
                    dai_cr				= dai;
                    dk_tau_si_cr		= dk_taui;
                    dk_tau_sti_cr		= dk_tau_st_i;
                    nLSsize_cr			= iLongStiffSize_sub;
                    dk_tau_st_lti_cr	= dk_tau_st_Limit_i;
                    dIsli_cr			= dIsl_i;
                }
            }
            else if(!bInterRigidStif && bInterNonRigidStif) // Case 2.
            {
                for(int j=0; j<2; j++) // Column
                {  
                    dk_taui = 0.0; dLamda_wi = 0.0;
                    dai = j==0 ? dNonRigPitch : dRigidPitch-dNonRigPitch; 

                    Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi,dk_taui, dk_tau_st_i, 
                        dk_tau_st_Limit_i ,dLamda_wi);                

                    if(dMax_Lamda_wi<dLamda_wi) 
                    {
                        dMax_Lamda_wi = dLamda_wi;
                        dhi_cr = dhwi;
                        dai_cr = dai;
                        nLSsize_cr = iLongStiffSize_sub;
                        dk_tau_si_cr=dk_taui; 
                        dk_tau_sti_cr = dk_tau_st_i;
                        dk_tau_st_lti_cr = dk_tau_st_Limit_i;
                        dIsli_cr = dIsl_i;
                        //nSubPannelCalcType = Get_SubPannelCalcType(iLongStiffSize);
                    }
                }          
            }
            else if(bInterRigidStif && !bInterNonRigidStif) // Case 3.
            {
                for(int j=0; j<2; j++) // Column
                {     
                    dk_taui = 0.0;	dLamda_wi = 0.0;
                    dai = j==0 ? dInterRigPitch : dRigidPitch-dInterRigPitch; 

                    Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi, dk_taui, dk_tau_st_i, 
                        dk_tau_st_Limit_i,dLamda_wi);                
                    if(dMax_Lamda_wi < dLamda_wi) 
                    {
                        dMax_Lamda_wi		= dLamda_wi;
                        dhi_cr				= dhwi;
                        dai_cr				= dai;
                        nLSsize_cr			= iLongStiffSize_sub;
                        dk_tau_si_cr		= dk_taui;
                        dk_tau_sti_cr		= dk_tau_st_i;
                        dk_tau_st_lti_cr	= dk_tau_st_Limit_i;
                        dIsli_cr			= dIsl_i;
                    }
                }
            }
            else if(bInterRigidStif && bInterNonRigidStif) // Case 4.
            {
                for(int j=0; j<4; j++) // Column
                { 
                    double dai = 0.0;
                    dk_taui = 0.0; dLamda_wi = 0.0;
                    if(dInterRigPitch<dNonRigPitch) // Inter-Non-rigid가 Inter-rigid보다 멀리 있다.
                    {
                        if(j==0)		{dai = dInterRigPitch;					} // a1
                        else if(j==1)	{dai = dNonRigPitch - dInterRigPitch;	} // a2
                        else if(j==2)	{dai = dRigidPitch - dNonRigPitch;		} // a3
                        else			{dai = dRigidPitch - dInterRigPitch;	} // a4
                    }
                    else if(dInterRigPitch>dNonRigPitch)
                    {
                        if(j==0)		{dai = dNonRigPitch;					} // a1
                        else if(j==1)	{dai = dInterRigPitch - dNonRigPitch;	} // a2
                        else if(j==2)	{dai = dRigidPitch - dInterRigPitch;	} // a3
                        else			{dai = dInterRigPitch;					} // a4
                    }
                    //            

                    Get_SlendernessParamLamda(2, iLongStiffSize_sub, dhwi, dai, dTw, dIsl_i, dEpsi, dk_taui, dk_tau_st_i, 
                        dk_tau_st_Limit_i, dLamda_wi);                

                    if(dMax_Lamda_wi < dLamda_wi) 
                    {
                        dMax_Lamda_wi		= dLamda_wi;
                        dhi_cr				= dhwi;
                        dai_cr				= dai;
                        nLSsize_cr			= iLongStiffSize_sub;
                        dk_tau_si_cr		= dk_taui;
                        dk_tau_sti_cr		= dk_tau_st_i;
                        dk_tau_st_lti_cr	= dk_tau_st_Limit_i;
                        dIsli_cr			= dIsl_i;
                    }
                }
            }
        }
    }

    // contribution from the web factor
    // 5.3(5) For webs with longitudinal stiffeners the slenderness parameter (lamda_w) in (3) should not be taken as less than ,EN 1993-1-5 : 2004, 24page   
    //if(dMax_Bar_Lamda_w<dMax_Lamda_wi)
    //  dMax_Bar_Lamda_w = dMax_Lamda_wi;
    if(fabs(dMax_Bar_Lamda_w-dMinValue) < m_dZero) {dMax_Bar_Lamda_w = 0.0;} // If do not find value, set 0.0
    if(fabs(dMax_Lamda_wi-dMinValue) < m_dZero)		{dMax_Lamda_wi = 0.0;}	// If do not find value, set 0.0

    double dLamda_w_cr = max(dMax_Bar_Lamda_w, dMax_Lamda_wi);

    // Save Result
    dLambda_w				= dMax_Bar_Lamda_w;
    ResD.bIsShearBuck		= bIsShearBuck;
    ResD.nShearBuckCount	= nShearBuckCount;
    ResD.bStiffened			= bStiffened;
    ResD.dIsl				= dIsl;
    ResD.dIsl_org			= dIsl_org;
    ResD.dMax_Bar_Lamda_w	= dMax_Bar_Lamda_w;
    ResD.dRigidPitch		= dRigidPitch;
    ResD.iLongStiffSize		= iLongStiffSize;
    ResD.dDist4Shr			= dDist4Shr;

    ResD.dhi_cr				= dhi_cr			;	
    ResD.dai_cr				= dai_cr			;	
    ResD.dk_tau_si_cr		= dk_tau_si_cr		;
    ResD.dk_tau_sti_cr		= dk_tau_sti_cr		;
    ResD.nLSsize_cr			= nLSsize_cr		;	
    ResD.dk_tau_st_lti_cr	= dk_tau_st_lti_cr	;
    ResD.dIsli_cr			= dIsli_cr			;
    ResD.dMax_Lamda_wi		= dMax_Lamda_wi		;

    ResD.dk_tau_p			= dk_tau_p;
    ResD.dk_tau_st_p		= dk_tau_st_p;
    ResD.dk_tau_st_Limit_p	= dk_tau_st_Limit_p;
    ResD.dLamda_w_part		= dLamda_w_part;
    ResD.dLamda_w_cr		= dLamda_w_cr;
    ResD.iLamda_w_Type		= iLamda_w_Type;

    return TRUE;
}

void CDgnPlateGirder_IRC::Set_Calc_Vbw_Param(double dAlpha, double dk_tau, double dk_tau_st, double dk_tau_st_limit, double dIsl, double dhw, double dt, 
                                             double dLamda_w, double dLamda_w_cr, int iLamda_w_Type, T_PRVS_Vbw_Rd_Param& rData)
{
    rData.dAlpha = dAlpha;
    rData.dk_tau = dk_tau;
    rData.dk_tau_st = dk_tau_st;
    rData.dk_tau_st_limit = dk_tau_st_limit;
    rData.dIsl = dIsl;
    rData.dhw = dhw;
    rData.dt = dt;
    rData.dLamda_w = dLamda_w;
    rData.dLamda_w_cr = dLamda_w_cr;
    rData.iLamda_w_Type = iLamda_w_Type;
}

void CDgnPlateGirder_IRC::Set_Calc_Vbw_Param2(int iLamda_w_scope, double dXw, double dVbw_Rd, double dV_Rd, double dVb_Rd_lim, double dV_Edi, double deta3, T_PRVS_Vbw_Rd_Param& rData)
{
    rData.iLamda_w_scope = iLamda_w_scope;
    rData.dXw = dXw;
    rData.dVbw_Rd = dVbw_Rd;
    rData.dV_Rd = dV_Rd;
    rData.dV_Rd_Limit = dVb_Rd_lim;
    rData.dV_Edi = dV_Edi;
    rData.deta3 = deta3;
}

void CDgnPlateGirder_IRC::Set_Calc_Vbw_SubPanel_Param(int nLongStfnSize, double dhw, double dt, double dAlpha, double dk_tau, double dk_tau_st, double dk_tau_st_lt, double dIsl, double dLamda_w, T_PRVS_Vbw_Rd_Sub_Panel& rData)
{
    rData.nLongStfnSize = nLongStfnSize;
    rData.dhw = dhw;
    rData.dt  = dt;
    rData.dAlpha  = dAlpha;
    rData.dk_tau  = dk_tau;
    rData.dk_tau_st = dk_tau_st;
    rData.dk_tau_st_lt = dk_tau_st_lt;
    rData.dIsl = dIsl;
    rData.dLamda_w = dLamda_w;
}

void CDgnPlateGirder_IRC::Set_Calc_FlangeShearstress_Param(double dtau_Ed, double dtau_b_Rd, double dV_Edi, double dQi, double dIyyi, double dtf, T_PRVS_Vbw_Rd_Param& rData)
{
    rData.dtau_Ed = dtau_Ed;
    rData.dtau_b_Rd = dtau_b_Rd;
    rData.dV_Edi = dV_Edi;
    rData.dQi = dQi;
    rData.dIyyi = dIyyi;
    rData.dt = dtf;
}



void CDgnPlateGirder_IRC::SetPgbrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGBR_BASE& rPgbrGd, T_PGBR_BASE& rData)
{
    rPgbrGd.bChk     = rData.bChk     = bChk;
    rPgbrGd.ElemK    = rData.ElemK    = ElemK.first;
    rPgbrGd.nPosi    = rData.nPosi    = nPosi;
    rPgbrGd.LcomK	   = rData.LcomK	   = iDgnLcomNo;
    rPgbrGd.OrgLcomK = rData.OrgLcomK = iOrgLcomNo;		    
    rPgbrGd.iMax	   = rData.iMax	   = iMaxMinType;

    rPgbrGd.dFx = aForceGd[0];
    rPgbrGd.dMy	= aForceGd[4]; 
    rPgbrGd.dMz	= aForceGd[5];

    rData.dFx	= aForce[0]; //- aForceGd[0];
    rData.dMy	= aForce[4]; //- aForceGd[4]; 
    rData.dMz	= aForce[5]; //- aForceGd[5];
    rData.dMa_Ed = aForceGd[4];
    rData.dMc_Ed = rData.dMy; 
}

BOOL CDgnPlateGirder_IRC::SetRhocBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, const T_PGBR_DETAIL& PgbrDetail, T_RHOC_BASE& Rho_C)
{
    Rho_C.bChk = bChk; // 계산을 통과하면 TRUE
    Rho_C.ElemK     = ElemK.first;
    Rho_C.nPosi     = nPosi;
    Rho_C.LcomK	    = iDgnLcomNo;
    Rho_C.OrgLcomK  = iOrgLcomNo;		    
    Rho_C.iMax	    = iMaxMinType;

    Rho_C.dRho_c      = PgbrDetail.dRho_c;
    Rho_C.dAa_eff_Fx  = PgbrDetail.EffGirderFx.Area; 
    Rho_C.dIya_eff_Fx = PgbrDetail.EffGirderFx.Ryy;  
    Rho_C.dIza_eff_Fx = PgbrDetail.EffGirderFx.Rzz;  
    Rho_C.dAc_eff_Fx  = PgbrDetail.EffCompositeFx.Area;  
    Rho_C.dIyc_eff_Fx = PgbrDetail.EffCompositeFx.Ryy;   
    Rho_C.dIzc_eff_Fx = PgbrDetail.EffCompositeFx.Rzz;   

    Rho_C.dAa_eff_My  = PgbrDetail.EffGirder.Area; 
    Rho_C.dIya_eff_My = PgbrDetail.EffGirder.Ryy;  
    Rho_C.dIza_eff_My = PgbrDetail.EffGirder.Rzz;  
    Rho_C.dAc_eff_My  = PgbrDetail.EffComposite.Area; 
    Rho_C.dIyc_eff_My = PgbrDetail.EffComposite.Ryy;  
    Rho_C.dIzc_eff_My = PgbrDetail.EffComposite.Rzz;  

    Rho_C.RhocBeforeFx = PgbrDetail.RhocBeforeFx; 
    Rho_C.RhocAfterFx  = PgbrDetail.RhocAfterFx; 
    Rho_C.RhocBeforeMy = PgbrDetail.RhocBeforeMy; 
    Rho_C.RhocAfterMy  = PgbrDetail.RhocAfterMy; 

    return TRUE;
}

void CDgnPlateGirder_IRC::SetPrvsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PRVS_BASE& rPrvsGd, T_PRVS_BASE& rData)
{
    rPrvsGd.bChk     = rData.bChk = bChk;
    rPrvsGd.ElemK    = rData.ElemK = ElemK.first;
    rPrvsGd.nPosi    = rData.nPosi = nPosi;
    rPrvsGd.LcomK	   = rData.LcomK	= iDgnLcomNo;
    rPrvsGd.OrgLcomK = rData.OrgLcomK = iOrgLcomNo;		    
    rPrvsGd.iMax	   = rData.iMax	= iMaxMinType;    

    rPrvsGd.dFx = aForceGd[0];
    rPrvsGd.dFz = aForceGd[2];
    rPrvsGd.dMy	= aForceGd[4]; 
    rPrvsGd.dMz	= aForceGd[5];

    rData.dFx	= aForce[0];
    rData.dFz = aForce[2];
    rData.dMy	= aForce[4]; 
    rData.dMz	= aForce[5];
    rData.dN_Ed = aForceGd[0]+aForce[0];
    rData.dV_Ed = aForceGd[2]+aForce[2];
    rData.dMa_Ed = aForceGd[4];
}

void CDgnPlateGirder_IRC::SetPltbBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce,
                                          double dV1, double dV2, double dM1, double dM2, double dC4, T_PLTB_BASE& rPltbGd, T_PLTB_BASE& rData)
{
    rPltbGd.bChk     = rData.bChk = bChk;
    rPltbGd.ElemK    = rData.ElemK = ElemK.first;
    rPltbGd.nPosi    = rData.nPosi = nPosi;
    rPltbGd.LcomK	   = rData.LcomK	= iDgnLcomNo;
    rPltbGd.OrgLcomK = rData.OrgLcomK = iOrgLcomNo;		    
    rPltbGd.iMax	   = rData.iMax	= iMaxMinType;        

    rPltbGd.dFx	= aForceGd[0];
    rPltbGd.dMy	= aForceGd[4]; 
    rPltbGd.dMz	= aForceGd[5];

    rData.dFx	= aForce[0];
    rData.dMy	= aForce[4]; 
    rData.dMz	= aForce[5];

    rData.dN_Ed = aForceGd[0]+aForce[0];
    rData.dM_Ed = aForce[4];
    rData.dV1	= dV1;
    rData.dV2	= dV2;
    rData.dM1	= dM1;
    rData.dM2	= dM2;
    rData.dC4 = dC4;		        

}

void CDgnPlateGirder_IRC::SetPrtfBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PRTF_BASE& rData)
{
    rData.bChk = bChk;
    rData.ElemK = ElemK.first;
    rData.nPosi = nPosi;
    rData.LcomK	= iDgnLcomNo;
    rData.OrgLcomK = iOrgLcomNo;		    
    rData.iMax	= iMaxMinType;        
    rData.dFx	= aForce[0];  
    rData.dMy	= aForce[4]; 
    rData.dMz	= aForce[5];
    rData.dN_Ed = aForce[0];
    rData.dMy_Ed = aForce[4];
    rData.dMz_Ed = aForce[5];
    rData.dF_Ed =  aForce[2];  
}

void CDgnPlateGirder_IRC::SetPglsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGLS_BASE& rPglsGd, T_PGLS_BASE& rData)
{
    rData.bChk = bChk;
    rData.ElemK = ElemK.first;
    rData.nPosi = nPosi;
    rData.LcomK	= iDgnLcomNo;
    rData.OrgLcomK = iOrgLcomNo;		    
    rData.iMax	= iMaxMinType;        

    rPglsGd.dFx = aForceGd[0];
    rPglsGd.dFz = aForceGd[2];
    rPglsGd.dMy	= aForceGd[4]; 
    rPglsGd.dMz	= aForceGd[5];

    rData.dFx	= aForce[0];
    rData.dFz = aForce[2];
    rData.dMy	= aForce[4]; 
    rData.dMz	= aForce[5];
    rData.dMa_Ed = aForceGd[4];
}

void CDgnPlateGirder_IRC::SetPgfrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PGRF_BASE& rData)
{
    rData.bChk = bChk;
    rData.ElemK = ElemK.first;
    rData.nPosi = nPosi;
    rData.LcomK	= iDgnLcomNo;
    rData.OrgLcomK = iOrgLcomNo;		    
    rData.iMax	= iMaxMinType;        
    rData.dFx	= aForce[0];  
    rData.dFz	= aForce[2];
    rData.dMy	= aForce[4]; 
    rData.dMz	= aForce[5];
}

void CDgnPlateGirder_IRC::SetPgbsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType,
                                          double* aForceGd, double* aForce, T_PGBS_BASE& rData)
{
    rData.bChk = bChk;
    rData.ElemK = ElemK.first;
    rData.nPosi = nPosi;
    rData.ChkGirder.LcomK = iDgnLcomNo;
    rData.ChkGirder.OrgLcomK = iOrgLcomNo;
    rData.ChkGirder.LcomType = iLcomType;
    rData.ChkGirder.iMax	= iMaxMinType;
    memcpy(rData.ChkGirder.aBeforeForce, aForceGd, sizeof(rData.ChkGirder.aBeforeForce));
    memcpy(rData.ChkGirder.aForce, aForce, sizeof(rData.ChkGirder.aForce));
    //
    rData.ChkSlab.LcomK = iDgnLcomNo;
    rData.ChkSlab.OrgLcomK = iOrgLcomNo;
    rData.ChkSlab.LcomType = iLcomType;
    rData.ChkSlab.iMax	= iMaxMinType;
    memcpy(rData.ChkSlab.aForce, aForce, sizeof(rData.ChkSlab.aForce));
    //
    rData.ChkBar.LcomK = iDgnLcomNo;
    rData.ChkBar.OrgLcomK = iOrgLcomNo;
    rData.ChkBar.LcomType = iLcomType;
    rData.ChkBar.iMax	= iMaxMinType;
    memcpy(rData.ChkBar.aForce, aForce, sizeof(rData.ChkBar.aForce));

}

void CDgnPlateGirder_IRC::SetPlssBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType, double* aForceGd, double* aForce, T_PLSS_BASE& rPlssGd, T_PLSS_BASE& rData)
{
    rData.bChk = bChk;
    rData.ElemK = ElemK.first;
    rData.nPosi = nPosi;
    rData.LcomK	= iDgnLcomNo;
    rData.OrgLcomK = iOrgLcomNo;
    rData.LcomType = iLcomType;
    rData.iMax	= iMaxMinType;        

    memcpy(rPlssGd.aForce, aForceGd, sizeof(rPlssGd.aForce));
    memcpy(rData.aForce, aForce, sizeof(rData.aForce));

    rData.dMa_Ed = aForceGd[4];    
}

BOOL CDgnPlateGirder_IRC::Get_MaterialData(ElemPairK ElemK, double& dfy, double& dfy1, double& dEs, double& dfck, double& dEcm, double& dfsk, double& dErs)
{
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {return FALSE;}

    // Material
    const T_MATD_D& MatdD = MatlElem.MatdD;
    // Steel.
    dfy  = MatdD.Data1.Design.S_Fy1;
    dfy1 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    dEs  = MatdD.Data1.Analysis.Elast;
    // Concrete
    dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    dEcm  = MatlElem.dEcm;
    // Rebar
    dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    dErs = dEs; // Steel로..

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_LineEndStress4LineDgnBase(const CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aDgnBaseD, double& dStr1, double& dStr2)
{
    int iDgnLineSize = aDgnBaseD.GetSize();
    if(iDgnLineSize<=0) {return FALSE;}

    dStr1 = aDgnBaseD[0].dStr1; // array 시작.
    dStr2 = aDgnBaseD[iDgnLineSize-1].dStr2; // array 끝.

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_ForceLineGS(double dfy1, double dfy2, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, double& dTotalForce)
{
    dTotalForce = 0.0;
    int nLineSize = aLine.GetSize();
    for(int i=0; i<nLineSize; i++)
    {
        T_GSEC_LINE LineD = aLine[i];
        int v1 = LineD.v1;
        int v2 = LineD.v2;

        T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
        T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
        double dx1 = vtx1.dX;
        double dy1 = vtx1.dY;
        double dx2 = vtx2.dX;
        double dy2 = vtx2.dY;
        double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
        double dt = LineD.dThik;
        double dAl = dbi*dt;    
        double dfy = Get_fy4Thick(dfy1, dfy2, dt);
        double dfyd = dfy/m_dGamma_M0;
        double dForcei = dAl*dfyd;
        dTotalForce += dForcei;    
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_TopBotForceLineGS(double dfy1, double dfy2, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, const CArray<double, double&>& aTopThick, 
                                                double& dTopForce, double& dBotForce)
{
    dTopForce = dBotForce = 0.0;
    int nLineSize = aLine.GetSize();
    for(int i=0; i<nLineSize; i++)
    {
        T_GSEC_LINE LineD = aLine[i];
        int v1 = LineD.v1;
        int v2 = LineD.v2;

        T_GSEC_VERTEX vtx1 = PolyD.aVertex[v1];
        T_GSEC_VERTEX vtx2 = PolyD.aVertex[v2];
        double dx1 = vtx1.dX;
        double dy1 = vtx1.dY;
        double dx2 = vtx2.dX;
        double dy2 = vtx2.dY;
        double dbi = CMathFunc::mathLength(dx1, dy1, dx2, dy2);
        double dt = LineD.dThik;
        double dt_top = aTopThick[i];
        double dt_bot = dt-dt_top;
        double dAl_top = dbi*dt_top;    
        double dAl_bot = dbi*dt_bot;    
        double dfy = Get_fy4Thick(dfy1, dfy2, dt);
        double dfyd = dfy/m_dGamma_M0;
        double dForceTopi = dAl_top*dfyd;
        double dForceBoti = dAl_bot*dfyd;
        dTopForce += dForceTopi;    
        dBotForce += dForceBoti;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_PartLineId(int nPartId, const T_GLINE_DGN_D& GLineDgnD, CArray<int,int&>& aWebLineId)
{
    int i=0,j=0,k=0;
    int iPartLineSize = GLineDgnD.aPartLine.GetSize();  
    for(i=0; i<iPartLineSize; i++)
    {
        T_GLINE_PART_DGN GLinePartD = GLineDgnD.aPartLine[i];
        int nPartType = GLinePartD.nPartType;
        if(nPartType==nPartId) 
        {      
            int iOutSize = GLinePartD.aOutstand.GetSize();
            for(j=0; j<iOutSize; j++)
            {
                T_GLINE_DST GLineDstD = GLinePartD.aOutstand[j];
                int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
                for(k=0; k<iLineDgnSize; k++)
                {
                    T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
                    aWebLineId.Append(GLineBaseD.aLineIx);
                }    
                int iStfnLineSize = GLineDstD.aStiffener.GetSize();
                for(k=0; k<iStfnLineSize; k++)
                {
                    T_GLINE_STIFFENER StfnLineD = GLineDstD.aStiffener[j];
                    aWebLineId.Append(StfnLineD.aLineIx);
                }
            }
            int iIntSize = GLinePartD.aInternal.GetSize();
            for(j=0; j<iIntSize; j++)
            {
                T_GLINE_DST GLineDstD = GLinePartD.aInternal[j];
                int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
                for(k=0; k<iLineDgnSize; k++)
                {
                    T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
                    aWebLineId.Append(GLineBaseD.aLineIx);
                }    
                int iStfnLineSize = GLineDstD.aStiffener.GetSize();
                for(k=0; k<iStfnLineSize; k++)
                {
                    T_GLINE_STIFFENER StfnLineD = GLineDstD.aStiffener[k];
                    aWebLineId.Append(StfnLineD.aLineIx);
                }
            }
        }
    }

    return TRUE;
}

double CDgnPlateGirder_IRC::Get_ShearStress(double dV, double dQ, double dIyy, double dbw)
{
    double dtau = dIyy*dbw==0.0 ? 0.0 : fabs(dV)*dQ/(dIyy*dbw);
    return dtau;
}

double CDgnPlateGirder_IRC::Get_Q_FirstMomentOfGirderCentroid(double* aSize, double dZcen_g, double dZcen_c)
{
    double dQ=0.0;
    if(dZcen_c<dZcen_g) return dQ;
    double dH=0., dTw=0., dB1=0., dTF1=0., dB2=0., dTF2=0., dHw=0.;
    Get_SectionSizeOfSect_I(aSize, dH, dTw, dB1, dTF1, dB2, dTF2, dHw);
    if(dZcen_g>dTF2)
    {
        double dQ_flg_b = (dTF2*dB2)*(dZcen_c-0.5*dTF2);
        double dQ_web = (dZcen_g-dTF2)*dTw*(dZcen_c-(dTF2+0.5*(dZcen_g-dTF2)));
        dQ = dQ_flg_b + dQ_web;
    }
    else 
    {
        double dQ_flg_b = (dZcen_g*dB2)*(dZcen_c-0.5*dZcen_g);
        dQ = dQ_flg_b;
    }

    return dQ;
}

BOOL CDgnPlateGirder_IRC::Get_ThicknessOfWeb4GenSect(const T_GLINE_DGN_D& GLineDgn, double& dbw,  double& dAweb)
{
    dbw=0.0, dAweb=0.0;
    int iPartSize = GLineDgn.aPartLine.GetSize();  
    int i=0, j=0, k=0;
    for(i=0; i<iPartSize; i++)
    {
        T_GLINE_PART_DGN PartLineD = GLineDgn.aPartLine[i];
        if(PartLineD.nPartType==2) // 2=Web
        {
            dbw += PartLineD.dt_in;
            //////////////////////////////////////////////////////////////////////////      

            int iOutSize = PartLineD.aOutstand.GetSize();
            for(j=0; j<iOutSize; j++)
            {
                T_GLINE_DST GLineDstD = PartLineD.aOutstand[j];
                int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
                for(k=0; k<iLineDgnSize; k++)
                {
                    T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
                    double dAwi = GLineBaseD.db * GLineBaseD.dt;
                    dAweb += dAwi;
                }    
            }
            int iIntSize = PartLineD.aInternal.GetSize();
            for(j=0; j<iIntSize; j++)
            {
                T_GLINE_DST GLineDstD = PartLineD.aInternal[j];
                int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
                for(k=0; k<iLineDgnSize; k++)
                {
                    T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
                    double dAwi = GLineBaseD.db * GLineBaseD.dt;
                    dAweb += dAwi;
                }    
            }
        }
    }

    return TRUE;
}


BOOL CDgnPlateGirder_IRC::Get_ThicknessOfFlange4GenSect(const T_GLINE_DGN_D& GLineDgn, double& dt_top, double& dt_bot)
{
    dt_top = dt_bot = 0.0;
    int iPartSize = GLineDgn.aPartLine.GetSize();  

    //  double dt_top_min=0.0;
    //  double dt_bot_min=0.0;
    //  int i=0, j=0, k=0;
    //  for(i=0; i<iPartSize; i++)
    //  {
    //    T_GLINE_PART_DGN PartLineD = GLineDgn.aPartLine[i];
    //    if(PartLineD.nPartType==0) // 0=Top
    //    {            
    //      int iOutSize = PartLineD.aOutstand.GetSize();
    //      for(j=0; j<iOutSize; j++)
    //      {
    //        T_GLINE_DST GLineDstD = PartLineD.aOutstand[j];
    //        int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
    //        for(k=0; k<iLineDgnSize; k++)
    //        {
    //          T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
    //          double dAwi = GLineBaseD.db * GLineBaseD.dt;
    //          dAweb += dAwi;
    //        }    
    //      }
    //      int iIntSize = PartLineD.aInternal.GetSize();
    //      for(j=0; j<iIntSize; j++)
    //      {
    //        T_GLINE_DST GLineDstD = PartLineD.aInternal[j];
    //        int iLineDgnSize = GLineDstD.aLineDgn.GetSize();
    //        for(k=0; k<iLineDgnSize; k++)
    //        {
    //          T_GLINE_BASE GLineBaseD = GLineDstD.aLineDgn[k];
    //          double dAwi = GLineBaseD.db * GLineBaseD.dt;
    //          dAweb += dAwi;
    //        }    
    //      }
    //    }
    //  }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::IsValueZero(double dValue)
{
    BOOL bIsZero=FALSE;
    if(fabs(dValue)<m_dZero) bIsZero = TRUE;
    if (!_finite(dValue)) bIsZero = TRUE;
    return bIsZero;
}

BOOL CDgnPlateGirder_IRC::Get_SectPropOfDLinePart(CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aDLinePart, double& dArea, double& dt)
{
    int i=0, j=0;
    int nPartSize = aDLinePart.GetSize();
    double dt_out = 0.0;
    double dt_int = 0.0;
    for(i=0; i<nPartSize; ++i)
    {
        double dAout = 0.0;
        double dt_out_i = 0.0;
        Get_SectPropOfLineLBD(aDLinePart[i].aOutstand, dAout, dt_out_i);
        //
        double dAint = 0.0;
        double dt_int_i = 0.0;
        Get_SectPropOfLineLBD(aDLinePart[i].aInternal, dAint, dt_int_i);
        dArea += dAout + dAint;
        double dt_pi = 0.0;
        if(dt_out_i>0.0 && dt_int_i>0.0) dt_pi = min(dt_out_i, dt_int_i);
        else if(dt_out_i>0.0)            dt_pi = dt_out_i;
        else if(dt_int_i>0.0)            dt_pi = dt_int_i;
        else ASSERT(0);
        if(i==0) dt = dt_pi;
        else     dt = min(dt, dt_pi);
    }
    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Get_SectPropOfLineLBD(CArray<T_DGNLINE_LBD,T_DGNLINE_LBD&>& aDLineLBD, double& dArea, double& dt)
{
    int i=0;
    int nLineLBDSize = aDLineLBD.GetSize();
    for(i=0; i<nLineLBDSize; ++i)
    {
        double dArea_i = aDLineLBD[i].dAp;
        double dti     = aDLineLBD[i].dt;
        dArea += dArea_i;
        if(i==0) dt = dti;
        else     dt = min(dt, dti);
    }
    return TRUE;
}

void CDgnPlateGirder_IRC::GetPureSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData)
{
    if(bPosI)	{rData = SectElemD.SectPosi[0].PureSect;}
    else		{rData = SectElemD.SectPosi[1].PureSect;}
}

void CDgnPlateGirder_IRC::GetNonComSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData)
{
    if(bPosI)	{rData = SectElemD.SectPosi[0].NonConSect;}
    else		{rData = SectElemD.SectPosi[1].NonConSect;}
}

void CDgnPlateGirder_IRC::GetRebarSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData)
{
    if(bPosI)	{rData = SectElemD.SectPosi[0].RebarSect;}
    else		{rData = SectElemD.SectPosi[1].RebarSect;}
}

double CDgnPlateGirder_IRC::GetPlasticNA()
{
    return m_dPlasticNA;
}

void CDgnPlateGirder_IRC::SetPlasticNA(const double& dPlasticNA)
{
    m_dPlasticNA = dPlasticNA;
}

void CDgnPlateGirder_IRC::Convert_VerbShearResD(const T_VERTICAL_SHAER_RES_UNIT_D_IRC& InD, T_PRVS_BASE_UNIT& rData)
{
    rData.dVpl_Rd				=	InD.dVpl_Rd;
    rData.dVbw_Rd				=	InD.dVbw_Rd;
    rData.dVbf_Rd				=	InD.dVbf_Rd;
    rData.dIsl_org				=	InD.dIsl_org;
    rData.dIsl					=	InD.dIsl;    
    rData.dXw					=	InD.dXw;     
    rData.dMax_Bar_Lamda_w		=	InD.dMax_Bar_Lamda_w;
    rData.dc					=	InD.dc;
    rData.dbf					=	InD.dbf		;       
    rData.dtf					=	InD.dtf		;       
    rData.dVb_Rd_org			=	InD.dVb_Rd_org;
    rData.dVb_Rd_Lim			=	InD.dVb_Rd_Lim;
}

double CDgnPlateGirder_IRC::GetOutStandFlgLineLengD(BOOL bTop, BOOL bLeft, bool bPosiI, const T_SECT_D& crSectD)
{
    double dOutStandLength = 0.0;
    int nSectType = m_pDgnSectUtil->Get_RealSectType(&crSectD);
    switch(nSectType)
    {
    case D_SECT_TYPE_COMPO_I:
        {
            const double dtw = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
            if(bTop)
            {
                dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bft_l(&crSectD, bPosiI) - dtw * 0.5 : m_pDgnSectUtil->Get_Size_bft_r(&crSectD, bPosiI) - dtw * 0.5;
            }
            else
            {
                dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bfb_l(&crSectD, bPosiI) - dtw * 0.5 : m_pDgnSectUtil->Get_Size_bfb_r(&crSectD, bPosiI) - dtw * 0.5;
            }
            break;
        }
    case D_SECT_TYPE_COMPO_B:
        {
            const double dtwL = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
            const double dtwR = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
            if (bTop)
            {
                dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bft_l(&crSectD, bPosiI) - dtwL * 0.5 : m_pDgnSectUtil->Get_Size_bft_r(&crSectD, bPosiI) - dtwR; ///MUSTCHECK : right part
            }
            else
            {
                dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bfb_l(&crSectD, bPosiI) - dtwL * 0.5 : m_pDgnSectUtil->Get_Size_bfb_r(&crSectD, bPosiI) - dtwR;
            }
            break;
        }
    case D_SECT_TYPE_COMPO_TUB:
        {
			const double dtwL = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
			const double dtwR = m_pDgnSectUtil->Get_Size_tw(&crSectD, CDgnSectUtil::EN_WEB_THK_RIGHT, bPosiI);
            if(bTop)
            {
                dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bft_ll(&crSectD, bPosiI) - dtwL : m_pDgnSectUtil->Get_Size_bft_rr(&crSectD, bPosiI) - dtwR;
            }
            else
            {
                dOutStandLength = bLeft ? m_pDgnSectUtil->Get_Size_bfb_l(&crSectD, bPosiI) - dtwL : m_pDgnSectUtil->Get_Size_bfb_r(&crSectD, bPosiI) - dtwR;
            }
            break;
        }
    default:
        {
            ASSERT(0);
            break;
        }
    }

    return dOutStandLength;
}

BOOL CDgnPlateGirder_IRC::GetCentFlgLinePosD(bool bPosiI, BOOL bTop, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD, 
                                             double& dy1, double& dz1, double& dy2, double& dz2)
{
    dy1 = dy2 = dz1 = dz2 = 0.0;
    double dOutFlgLength = GetOutStandFlgLineLengD(bTop, bLeft, bPosiI , crSectD);

    if(bTop)
    {
        dy1 = bLeft ? crStiffD.dx1 : crStiffD.dx2;
        dz1	= bLeft ? crStiffD.dy1 : crStiffD.dy2;
        dy2 = bLeft ? crStiffD.dx1 + dOutFlgLength : crStiffD.dx2 - dOutFlgLength;
        dz2 = dz1;
    }
    else
    {
        dy1 = bLeft ? crStiffD.dx4 : crStiffD.dx3;
        dz1	= bLeft ? crStiffD.dy4 : crStiffD.dy3;
        dy2 = bLeft ? crStiffD.dx4 + dOutFlgLength : crStiffD.dx3 - dOutFlgLength;
        dz2 = dz1;
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::GetCentWebLinePosD(bool bPosiI, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD, 
                                             double& dy1, double& dz1, double& dy2, double& dz2)
{
    dy1 = dy2 = dz1 = dz2 = 0.0;
    const CDgnSectUtil::EN_WEB_THK enWebThk = bLeft ? CDgnSectUtil::EN_WEB_THK_LEFT : CDgnSectUtil::EN_WEB_THK_RIGHT;
    double dtw	= m_pDgnSectUtil->Get_Size_tw(&crSectD, enWebThk, bPosiI);
    double dtft = m_pDgnSectUtil->Get_Size_tft(&crSectD, bPosiI);
    double dtfb = m_pDgnSectUtil->Get_Size_tfb(&crSectD, bPosiI);
    double dOutFlgLength_T = GetOutStandFlgLineLengD(TRUE, bLeft, bPosiI, crSectD);
    double dOutFlgLength_B = GetOutStandFlgLineLengD(FALSE, bLeft, bPosiI, crSectD);

    if(bLeft)
    {
        dy1 = crStiffD.dx1 + dOutFlgLength_T + 0.5 *dtw;
        dz1 = crStiffD.dy1 - dtft;

        dy2 = crStiffD.dx4 + dOutFlgLength_B + 0.5 *dtw;
        dz2 = crStiffD.dy4 + dtfb;
    }
    else
    {
        dy1 = crStiffD.dx2 - dOutFlgLength_T - 0.5 *dtw;
        dz1 = crStiffD.dy1 - dtft;

        dy2 = crStiffD.dx3 - dOutFlgLength_B - 0.5 *dtw;
        dz2 = crStiffD.dy4 + dtfb;
    }

    return TRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Detail Print
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BOOL CDgnPlateGirder_IRC::Execute_Detail_SteelPlateGirder(ArrElemPairKey& arElemK, BOOL* bPgdChk)
{
    int iElementNum = arElemK.GetSize();
    if(iElementNum==0) {return FALSE;}

    m_strUnitMM = _T(" mm");
    m_strUnitMM2 = _T(" mm^2");
    m_strUnitMM3 = _T(" mm^3");
    m_strUnitMM4 = _T(" mm^4");
    m_strUnitStress = _T(" N/mm^2");  
    m_strUnitForce  = _T(" N");
    m_strUnitForce2 = _T(" N/mm");
    m_strUnitMoment = _T(" N.mm");

    //
    T_UNIT_INDEX CurrUnit;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    T_UNIT_INDEX CodeUnit = m_pDataCtrl->Get_CodeUnitIndex();
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
    //

    //m_pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();

    m_strTestDgnFile.Format(_T("%s\\DataForPlateGirder.txt"), CPathUtil::GetOrMakeMIDASTempFolder());
    rptwofstream fout(m_strTestDgnFile, ios::trunc);

    for(int i=0; i<iElementNum; i++)
    {
        auto ElemK = arElemK[i];		
        Detail_Header(ElemK);

        for(int j=0; j<2; j++)
        {      
            UINT nPosi = (j==0 ? 1 : 2);
            if(m_pDataCtrl->Get_ChkFlagPlateGirder(ElemK,nPosi))
            {
                BOOL bPosiI = j==0 ? TRUE : FALSE;
                Detail_SectionProperty_Material(ElemK, bPosiI);
                if(bPgdChk[0]) {Detail_BendingResistance(ElemK, bPosiI);				 }
                if(bPgdChk[1]) {Detail_VerticalShearResistance(ElemK, bPosiI);			 }
                if(bPgdChk[2]) {Detail_LateralTorsionalBucklingResistance(ElemK, bPosiI);}
                if(bPgdChk[3]) {Detail_TransverseForceResistance(ElemK, bPosiI);		 }
                if(bPgdChk[4]) {Detail_LongitudinalShearResistance(ElemK, bPosiI);		 }
                if(bPgdChk[5]) {Detail_FatigueResistance(ElemK, bPosiI);				 }
            }
        }
    }

    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_Header(ElemPairK ElemK)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    T_ELEM_D ElemData;
    ElemData.Initialize();
    if (ElemK.second == EN_EL_BEAM)
    {
        m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemData);
    }    

    //int iElemType = ElemData.eltyp;
    int iSectNo	 = ElemData.elpro;
    int iMatlNo	 = CDBLib::GetMaterialKey4ElemPairK(ElemK);  
    if (ElemK.second == EN_EL_VBEAM)
    {
        iSectNo = ElemK.first;
    }

    //CString strLcomNo=_T("");	strLcomNo.Format(_T("%6s"), LcomDesign.DesignLcomNa);
    CString strElemNo=_T("");	strElemNo.Format(_T("%6d"), ElemK.first);
    CString strSectNo=_T("");	strSectNo.Format(_T("%6d"), iSectNo);
    CString strMatlNo=_T("");	strMatlNo.Format(_T("%6d"), iMatlNo);  

    fout<<endl
        <<_T("======================================================================================")
        <<endl;

    fout<<endl
        <<_T("  *. MEMBER NO    = ")<<strElemNo<<endl
        <<_T("  *. MATERIAL NO  = ")<<strMatlNo<<endl
        <<_T("  *. SECTION NO   = ")<<strSectNo<<endl      
        <<endl;

    //<<_T("      *. LOADCOMB NO = ")<<strLcomNo<<

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_SectionProperty_Material(ElemPairK ElemK, BOOL bPosiI)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem)) ASSERT(0);
    //
    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) ASSERT(0);

    // 1. Section.  
    T_SECT_D& SectD	= SectElem.SectD;
    BOOL bTapered		= m_pDgnSectUtil->Is_TaperedSect(&SectD);
    double dH			= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1		= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTf1		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2		= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTf2		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh			= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc		= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc		= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh		= m_pDgnSectUtil->Get_Size_Hh(&SectD); 
    double dHt		= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 2. Material
    const T_MATD_D& MatdD = MatlElem.MatdD;
    double dfy			= MatdD.Data1.Design.S_Fy1;
    double dfy1			= MatdD.Data1.Design.S_Fy2;
    double dfck			= MatlElem.dfck;
    double dfsk			= MatlElem.MatdD.MainRebarData.B_fy;
    double dEcm			= MatlElem.dEcm;
    double dgam_M0		= m_dGamma_M0;
    double dgam_sr		= m_dGamma_S;
    double dgam_c			= m_dGamma_C;
    double dfcd			=   0.67* dfck/dgam_c;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfsd			= fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;
    double dfyd_t			= fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b			= fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w			= fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;
    // girder
    T_SECT_STIFFNESS GirderStiffD;
    GirderStiffD.Initialize();
    if(!bTapered)  
        GirderStiffD = SectD.SectBefore.Stiffness;
    else
        GirderStiffD = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;

    double dArea_g = GirderStiffD.Area;
    double dIyy_g  = GirderStiffD.Ryy;
    double dIzz_g  = GirderStiffD.Rzz;
    double dYbar_g = GirderStiffD.Cym;
    double dZbar_g = GirderStiffD.Czm;

    // Composite section
    T_SECT_STIFFNESS CompoStiffD;
    CompoStiffD.Initialize();
    if(!bTapered)  
        CompoStiffD = SectD.SectAfter.Stiffness;
    else
        CompoStiffD = bPosiI ? SectD.SectAfter.SectI.Stiffness : SectD.SectAfter.SectJ.Stiffness;

    double dArea = CompoStiffD.Area;
    double dIyy  = CompoStiffD.Ryy;
    double dIzz  = CompoStiffD.Rzz;
    double dYbar = CompoStiffD.Cym;
    double dZbar = CompoStiffD.Czm;
    //
    T_SECT_STIFFNESS RebarSect; RebarSect.Initialize();
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    double dArea_r = RebarSect.Area;
    double dIyy_r  = RebarSect.Ryy;
    double dIzz_r  = RebarSect.Rzz;
    double dYbar_r = RebarSect.Cym;
    double dZbar_r = RebarSect.Czm;  

    CString strPosi = bPosiI ? _T("I") : _T("J");
    CString strPosition;      strPosition.Format(_T("%6s"), strPosi);
    //
    CString strArea_g;	 strArea_g.Format(_T("%16g"), dArea_g);
    CString strIyy_g;	   strIyy_g.Format(_T("%16g"), dIyy_g);
    CString strIzz_g;	   strIzz_g.Format(_T("%16g"), dIzz_g);
    CString strYbar_g;   strYbar_g.Format(_T("%16g"), dYbar_g);
    CString strZbar_g;   strZbar_g.Format(_T("%16g"), dZbar_g);
    //
    CString strArea;	 strArea.Format(_T("%16g"), dArea);
    CString strIyy;	   strIyy.Format(_T("%16g"), dIyy);
    CString strIzz;	   strIzz.Format(_T("%16g"), dIzz);
    CString strYbar;   strYbar.Format(_T("%16g"), dYbar);
    CString strZbar;   strZbar.Format(_T("%16g"), dZbar);
    //
    CString strArea_r;	 strArea_r.Format(_T("%16g"), dArea_r);
    CString strIyy_r;	   strIyy_r.Format(_T("%16g"), dIyy_r);
    CString strIzz_r;	   strIzz_r.Format(_T("%16g"), dIzz_r);
    CString strYbar_r;   strYbar_r.Format(_T("%16g"), dYbar_r);
    CString strZbar_r;   strZbar_r.Format(_T("%16g"), dZbar_r);

    fout<<endl
        <<_T("--------------------------------------------------------------------------------------")<<endl      
        <<_T("  *. POSITION     = ")<<strPosition<<endl      
        <<endl;
    // Section demension
    fout<<_T("  *. Section Size")<<endl
        <<_T("   -Girder")<<endl
        <<_T("     H   = ")<<Get_String_Format(dH)<<m_strUnitMM<<endl      
        <<_T("     tw  = ")<<Get_String_Format(dTw)<<m_strUnitMM<<endl
        <<_T("     Hw  = ")<<Get_String_Format(dh)<<m_strUnitMM<<endl
        <<_T("     B1  = ")<<Get_String_Format(dB1)<<m_strUnitMM<<endl
        <<_T("     tf1 = ")<<Get_String_Format(dTf1)<<m_strUnitMM<<endl
        <<_T("     B2  = ")<<Get_String_Format(dB2)<<m_strUnitMM<<endl
        <<_T("     tf2 = ")<<Get_String_Format(dTf2)<<m_strUnitMM<<endl
        <<endl
        <<_T("   -Slab")<<endl
        <<_T("     Bc   = ")<<Get_String_Format(dBc)<<m_strUnitMM<<endl
        <<_T("     tc   = ")<<Get_String_Format(dtc)<<m_strUnitMM<<endl
        <<_T("     Hh   = ")<<Get_String_Format(dHh)<<m_strUnitMM<<endl
        <<endl;

    // Gross section
    fout<<_T("  *. Section Property(Girder)")<<endl
        <<_T("     Area = ")<<strArea_g<<m_strUnitMM2<<endl
        <<_T("     Iyy  = ")<<strIyy_g<<m_strUnitMM4<<endl
        <<_T("     Izz  = ")<<strIzz_g<<m_strUnitMM4<<endl
        <<_T("     Ybar = ")<<strYbar_g<<m_strUnitMM<<endl
        <<_T("     Zbar = ")<<strZbar_g<<m_strUnitMM<<endl
        <<endl;  		

    // Gross section
    fout<<_T("  *. Section Property(Composite)")<<endl
        <<_T("     Area = ")<<strArea<<m_strUnitMM2<<endl
        <<_T("     Iyy  = ")<<strIyy<<m_strUnitMM4<<endl
        <<_T("     Izz  = ")<<strIzz<<m_strUnitMM4<<endl
        <<_T("     Ybar = ")<<strYbar<<m_strUnitMM<<endl
        <<_T("     Zbar = ")<<strZbar<<m_strUnitMM<<endl
        <<endl;  		

    // Rebar
    fout<<_T("  *. Section Property(Composite-R)")<<endl
        <<_T("     Area = ")<<strArea_r<<m_strUnitMM2<<endl
        <<_T("     Iyy  = ")<<strIyy_r<<m_strUnitMM4<<endl
        <<_T("     Izz  = ")<<strIzz_r<<m_strUnitMM4<<endl
        <<_T("     Ybar = ")<<strYbar_r<<m_strUnitMM<<endl
        <<_T("     Zbar = ")<<strZbar_r<<m_strUnitMM<<endl
        <<endl;  		

    // Material
    fout<<_T("  *. Material")<<endl      
        <<_T("     fy     = ")<<Get_String_Format(dfy)<<m_strUnitStress<<endl
        <<_T("     fy1    = ")<<Get_String_Format(dfy1)<<m_strUnitStress<<endl
        <<_T("     fy,tf  = ")<<Get_String_Format(dfyd_t)<<m_strUnitStress<<endl
        <<_T("     fy,bf  = ")<<Get_String_Format(dfyd_b)<<m_strUnitStress<<endl
        <<_T("     fy,w   = ")<<Get_String_Format(dfyd_w)<<m_strUnitStress<<endl
        <<_T("     fck    = ")<<Get_String_Format(dfck)<<m_strUnitStress<<endl
        <<_T("     fcd    = ")<<Get_String_Format(dfcd)<<m_strUnitStress<<endl
        <<_T("     fsk    = ")<<Get_String_Format(dfsk)<<m_strUnitStress<<endl
        <<_T("     fsd    = ")<<Get_String_Format(dfsd)<<m_strUnitStress<<endl
        <<_T("     Ecm    = ")<<Get_String_Format(dEcm)<<m_strUnitStress<<endl
        <<endl;


    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_BendingResistance(ElemPairK ElemK, BOOL bPosiI)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    BOOL bSect=TRUE, bMatl=TRUE;
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem)) {bSect=FALSE;}

    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {bMatl=FALSE;}

    // 1. Section.  
    T_SECT_D& SectD	= SectElem.SectD;
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw		= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1		= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTf1		= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2		= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTf2		= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc		= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc		= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh		= m_pDgnSectUtil->Get_Size_Hh(&SectD); 
    double dHt		= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 2. Material
    const T_MATD_D& MatdD = MatlElem.MatdD;
    double dfy			= MatdD.Data1.Design.S_Fy1;
    double dfy1			= MatdD.Data1.Design.S_Fy2;
    double dfck			= MatlElem.dfck;
    double dgam_M0		= m_dGamma_M0;
    double dgam_c			= m_dGamma_C;
    double dfcd			=   0.67* dfck/dgam_c;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t			= fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b			= fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w			= fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // Load combination
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
        _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
        _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};

    ////////////////////////////////////////////////////////////////////
    BOOL bPositive;
    double dXpl;
    double dXplOverh=0.;  
    double dPlasticNA=0., dPlasticNAEff=0.;
    /*
    double dNa_topc=0., dNa_topt=0.;          
    double dNa_webc=0., dNa_webt=0.;          
    double dNa_botc=0., dNa_bott=0.;          
    double dNa_webc1=0., dNa_webc2=0.;        
    double dNa_web_Effe=0.;                
    double dDistT=0., dDistB=0., dDistW=0.;      
    double dDistTc=0., dDistTt=0.;            
    double dDistWc=0., dDistWt=0.;            
    double dDistBc=0., dDistBt=0.;            
    double dDistWc1=0., dDistWc2=0.;          
    double dDistW1=0., dDistW2=0.;            
    double dCompfsdAsidsi=0., dTensfsdAsidsi=0.;
    double dBeta=0.;  
    // elastic
    double dStrStlTop=0., dStrComTop=0., dStrStlBot=0., dStrComBot=0.;  
    double dSIGa_top=0., dSIGa_bot=0.;  
    double dk_min=0.;   
    */
    /////////////////////////////////////////////////////////////////////  

    for(int i=0; i<2; i++) // 0=positive, 1=negative
    {
        int iMaxMin = i==0 ? 1 : 2;
        T_PGBR_LCOM PgbrLcomD;
        PgbrLcomD.Initialize();
        if(!m_pAnalysisResult->GetPgbrResultData(ElemK, iMaxMin, PgbrLcomD)) {continue;}
        //
        T_PGBR_BASE PgbrBase;
        if(bPosiI) {PgbrBase = PgbrLcomD.PgbrBase[0];}
        else 		 {PgbrBase = PgbrLcomD.PgbrBase[1];}

        if(!PgbrBase.bChk) continue;
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        dXpl      = PgbrBase.dXpl ;
        dXplOverh = PgbrBase.dXplOverh;  

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        bPositive = PgbrBase.dMy > -m_dZero ? TRUE : FALSE;
        BOOL bEquiClass2=PgbrBase.bEquiClass2;
        // Lcom
        CString strLoadComb;    
        T_LCOM_D LcomData;      
        if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PgbrBase.OrgLcomK, LcomData)) 
        {strLoadComb.Format(_T("%s(%s)"),LcomData.LoadCombName, aType[PgbrBase.iMax]);}
        else 
        {strLoadComb = _T("-");}
        // Class
        CString strTopFlange; strTopFlange.Format(_T("Class %d"),PgbrBase.iTopClass);
        CString strBotFlange; strBotFlange.Format(_T("Class %d"),PgbrBase.iBotClass);
        CString strWeb; strWeb.Format(_T("Class %d"),PgbrBase.iWebClass);
        CString strSectClass;
        if(bEquiClass2)
            strSectClass = _T("Effective Class 2");
        else
            strSectClass.Format(_T("Class %d"),PgbrBase.iSectClass);

        fout<<endl
            <<_T("--------------------------------------------------------------------------------------")<<endl      
            <<endl
            <<_T("  *. Bending Resistance")<<endl
            <<endl
            <<_T("  -  Load Combination     = ")<<strLoadComb<<endl      
            <<endl;

        fout<<_T("  -  Force")<<endl      
            <<_T("     Ma,Ed = ")<<Get_String_Format(PgbrBase.dMa_Ed)<<m_strUnitMoment<<endl
            <<_T("     Mc,Ed = ")<<Get_String_Format(PgbrBase.dMc_Ed)<<m_strUnitMoment<<endl
            <<endl;

        fout<<_T("  -  Classification of cross sections")<<endl
            <<_T("     Top flange    = ")<<strTopFlange<<endl
            <<_T("     Bottom flange = ")<<strBotFlange<<endl
            <<_T("     Web           = ")<<strWeb<<endl
            <<_T("     Section       = ")<<strSectClass<<endl
            <<endl;

        //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
        fout<<_T("  -  Plastic resistance moment Mpl,Rd")<<endl;        

        if(bEquiClass2)
        {
            fout<<_T("     Effective Plastic NA    = ")<<Get_String_Format(PgbrBase.dPlasticNAEff)<<m_strUnitMM<<endl;

        }
        else
        {
            fout<<_T("     Plastic NA = ")<<Get_String_Format(PgbrBase.dPlasticNA)<<m_strUnitMM<<endl;
        }


        fout<<_T("     Xpl        = ")<<Get_String_Format(PgbrBase.dXpl)<<m_strUnitMM<<endl
            <<_T("     Xpl/h      = ")<<Get_String_Format(PgbrBase.dXplOverh)<<endl        
            <<_T("     Alpha      = ")<<Get_String_Format(PgbrBase.dAlpha)<<endl
            <<endl;

        if(!bEquiClass2)
        {
            // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
            if(bPositive)
            {
                if(PgbrBase.dPlasticNA>dH)
                {
                    if(dXpl>dtc)
                    {
                        //dNc_f = dBc*dtc*dAlpha*dfcd;
                        //fout<<_T("     Nc,f       = ")<<_T("Bc*tc*Alpha*fcd")<<Get_String_Format(PgbrBase.dNc_f)<<m_strUnitForce<<endl;            
                    }
                    else if(dXpl<dtc)
                    {
                        //dNc_f = dBc*dXpl*dAlpha*dfcd;
                        //fout<<_T("     Nc,f       = ")<<_T("Bc*Xpl*Alpha*fcd")<<Get_String_Format(PgbrBase.dNc_f)<<m_strUnitForce<<endl;
                    } 
                    // dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
                    //
                    fout<<_T("   - Mpl,Rd")<<endl
                        <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                        <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                        <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                        <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                        <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                        <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                        <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot*Dist.BF = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                        <<endl;          
                }
                else
                {
                    if(PgbrBase.dPlasticNA>dH-dTf1) // Top에 위치할 때 
                    {
                        //dTfc = dH - dPlasticNA;
                        //dTft = dPlasticNA - (dH-dTF1);
                        //dNa_topc = dB1*dTfc*dfyd_t;
                        //dNa_topt = dB1*dTft*dfyd_t;
                        //dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
                        //dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
                        //dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                        //dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dh;
                        //
                        //dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TFc  = ")<<Get_String_Format(PgbrBase.dDistTc)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                            <<_T("     Dist.TFt  = ")<<Get_String_Format(PgbrBase.dDistTt)<<m_strUnitMM<<endl
                            <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top,c  = ")<<Get_String_Format(PgbrBase.dNa_topc)<<m_strUnitForce<<endl
                            <<_T("     Na,top,t  = ")<<Get_String_Format(PgbrBase.dNa_topt)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("-Na,top,c*Dist.TFc + Na,top,t*Dist.TFt + Na,web*Dist.W + Na,bot*Dist.BF = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                    else if(PgbrBase.dPlasticNA<dH-dTf1 && PgbrBase.dPlasticNA>dTf2) // Web에 위치할 때 
                    {
                        //double dWc = (dH-dTF1) - dPlasticNA;
                        //double dWt = dPlasticNA - dTF2;
                        //dNa_webc = dWc*dTw*dfyd_w;
                        //dNa_webt = dWt*dTw*dfyd_w;
                        //dDistWc = 0.5*dtc + dHh + dTF1 + 0.5*dWc;
                        //dDistWt = 0.5*dtc + dHh + dTF1 + dWc + 0.5*dWt;
                        //dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                        //dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
                        //
                        //dMpl_Rd = -dNa_top*dDistT-dNa_webc*dDistWc+dNa_webt*dDistWt+dNa_bot*dDistB;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                            <<_T("     Dist.Wc   = ")<<Get_String_Format(PgbrBase.dDistWc)<<m_strUnitMM<<endl
                            <<_T("     Dist.Wt   = ")<<Get_String_Format(PgbrBase.dDistWt)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web,c  = ")<<Get_String_Format(PgbrBase.dNa_webc)<<m_strUnitForce<<endl
                            <<_T("     Na,web,t  = ")<<Get_String_Format(PgbrBase.dNa_webt)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("-Na,top*Dist.TF - Na,web,c*Dist.Wc + Na,web,t*Dist.Wt + Na,bot*Dist.BF = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                    else if(PgbrBase.dPlasticNA<dTf2) // Bot에 위치할 때.
                    {
                        //double dBfc = dTF2 - dPlasticNA;
                        //double dBft = dPlasticNA;
                        //dNa_botc = dB2*dBfc*dfyd_b;
                        //dNa_bott = dB2*dBft*dfyd_b;
                        //dDistBc = 0.5*dtc + dHh + dTF1 + dh + 0.5*dBfc;
                        //dDistBt = 0.5*dtc + dHh + dTF1 + dh + dBfc + 0.5*dBft;
                        //dDistT= dHt - 0.5*dtc - (dH-0.5*dTF1);
                        //dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dh;
                        //
                        //dMpl_Rd = -dNa_top*dDistT  - dNa_web*dDistW - dNa_botc*dDistBc + dNa_bott*dDistBt;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                            <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                            <<_T("     Dist.BFc  = ")<<Get_String_Format(PgbrBase.dDistBc)<<m_strUnitMM<<endl
                            <<_T("     Dist.BFt  = ")<<Get_String_Format(PgbrBase.dDistBt)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                            <<_T("     Na,bot,c  = ")<<Get_String_Format(PgbrBase.dNa_botc)<<m_strUnitForce<<endl
                            <<_T("     Na,bot,t  = ")<<Get_String_Format(PgbrBase.dNa_bott)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("-Na,top*Dist.TF - Na,web*Dist.W - Na,bot,c*Dist.BFc + Na,bot,t*Dist.BFt = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                }
            }
            // Negative일 경우는 강도 계산할 때 PlasticNA가 기준. 
            else // Negative moment.
            {
                // Rebar 계산.
                /*
                dCompfsdAsidsi=0.0;
                dTensfsdAsidsi=0.0;
                double ddsi=0.0;      
                double dAsi=0.0;
                double dNAdsi=0.0;
                double dGap = (dH+dHh+0.5*dtc) - dPlasticNA;    
                for(i=0; i<arRbarPosi.GetSize(); i++)
                {
                _DGN_RBAR_CRC Rbar;
                Rbar = arRbarPosi[i];
                // PlasticNA에서 떨어진 거리.        
                dNAdsi = dGap + Rbar.dz;
                dAsi = Rbar.dArea;
                if(dNAdsi<0.0) // 압축.
                {
                dCompfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
                }
                else
                {
                dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
                }
                }
                */

                fout<<_T("   - Force in the reinforcement")<<endl
                    <<_T("     SUM(fsd*As*ds),C  = ")<<Get_String_Format(PgbrBase.dCompfsdAsidsi)<<m_strUnitMoment<<endl
                    <<_T("     SUM(fsd*As*ds),T  = ")<<Get_String_Format(PgbrBase.dTensfsdAsidsi)<<m_strUnitMoment<<endl
                    <<endl;          

                if(PgbrBase.dPlasticNA>dH)
                {
                    //dDistT = dPlasticNA - (dH-0.5*dTF1);
                    //dDistW = dPlasticNA - (0.5*dh+dTF2);
                    //dDistB = dPlasticNA - 0.5*dTF2;      
                    //
                    //dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
                    fout<<_T("   - Mpl,Rd")<<endl
                        <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Plastic NA)")<<endl
                        <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                        <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                        <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                        <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                        <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                        <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                        <<endl;          
                }
                else
                {
                    if(PgbrBase.dPlasticNA>dH-dTf1) // Top에 위치할 때 
                    {
                        //double dTfc = dPlasticNA - (dH-dTF1);
                        //double dTft = dH - dPlasticNA;
                        //dNa_topc = dB1*dTfc*dfyd_t;
                        //dNa_topt = dB1*dTft*dfyd_t;
                        //dDistTc  = 0.5*dTfc;
                        //dDistTt  = 0.5*dTft;
                        //dDistB   = dPlasticNA - 0.5*dTF2;
                        //dDistW   = dPlasticNA - (dTF2+0.5*dh);        
                        //
                        //dMpl_Rd = dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW + dCompfsdAsidsi + dTensfsdAsidsi;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TFc   = ")<<Get_String_Format(PgbrBase.dDistTc)<<m_strUnitMM<<_T("  (from Plastic NA)")<<endl
                            <<_T("     Dist.TFt   = ")<<Get_String_Format(PgbrBase.dDistTt)<<m_strUnitMM<<endl
                            <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top,c  = ")<<Get_String_Format(PgbrBase.dNa_topc)<<m_strUnitForce<<endl
                            <<_T("     Na,top,t  = ")<<Get_String_Format(PgbrBase.dNa_topt)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("Na,top,t*Dist.TFt + Na,top,c*Dist.TFc + Na,web*Dist.W + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                    else if(PgbrBase.dPlasticNA<dH-dTf1 && PgbrBase.dPlasticNA>dTf2) // Web에 위치할 때 
                    {
                        //double dWc = dPlasticNA - dTF2; 
                        //double dWt = (dH-dTF1) - dPlasticNA;
                        //dNa_webc = dWc*dTw*dfyd_w;
                        //dNa_webt = dWt*dTw*dfyd_w;
                        //dDistWc = 0.5*dWc;
                        //dDistWt = 0.5*dWt;
                        //dDistT = (dH-0.5*dTF1) - dPlasticNA;
                        //dDistB = dPlasticNA - 0.5*dTF2;        
                        //
                        //dMpl_Rd = dNa_top*dDistT+dNa_webc*dDistWc+dNa_webt*dDistWt+dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Plastic NA)")<<endl
                            <<_T("     Dist.Wc   = ")<<Get_String_Format(PgbrBase.dDistWc)<<m_strUnitMM<<endl
                            <<_T("     Dist.Wt   = ")<<Get_String_Format(PgbrBase.dDistWt)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web,c  = ")<<Get_String_Format(PgbrBase.dNa_webc)<<m_strUnitForce<<endl
                            <<_T("     Na,web,t  = ")<<Get_String_Format(PgbrBase.dNa_webt)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web,t*Dist.Wt + Na,web,c*Dist.Wc + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                    else if(PgbrBase.dPlasticNA<dTf2) // Bot에 위치할 때.
                    {
                        //double dBfc = dPlasticNA;
                        //double dBft = dTF2 - dPlasticNA;
                        //dNa_botc = dB2*dBfc*dfyd_b;
                        //dNa_bott = dB2*dBft*dfyd_b;
                        //dDistBc = 0.5*dBfc;
                        //dDistBt = 0.5*dBft;
                        //dDistT = (dH-0.5*dTF1)-dPlasticNA;
                        //dDistW = (dTF2+0.5*dh)-dPlasticNA;
                        //
                        //dMpl_Rd = dNa_top*dDistT + dNa_web*dDistW + dNa_botc*dDistBc + dNa_bott*dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Plastic NA)")<<endl
                            <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                            <<_T("     Dist.BFc  = ")<<Get_String_Format(PgbrBase.dDistBc)<<m_strUnitMM<<endl
                            <<_T("     Dist.BFt  = ")<<Get_String_Format(PgbrBase.dDistBt)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                            <<_T("     Na,bot,c  = ")<<Get_String_Format(PgbrBase.dNa_botc)<<m_strUnitForce<<endl
                            <<_T("     Na,bot,t  = ")<<Get_String_Format(PgbrBase.dNa_bott)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot,t*Dist.BFt + Na,bot,c*Dist.BFc +SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                }
            }
        }
        else
        {
            // Positive일 경우는 강도 계산할 때 압축콘크리트 중심(Xpl*0.5)에서 계산한다.
            if(bPositive)
            {
                if(PgbrBase.dPlasticNAEff>dH)
                {
                    if(dXpl>dtc)
                    {
                        //dNc_f = dBc*dtc*dAlpha*dfcd;
                    }
                    else if(dXpl<dtc)
                    {
                        //dNc_f = dBc*dXpl*dAlpha*dfcd;
                    }
                    //
                    //dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web*dDistW;
                    fout<<_T("   - Mpl,Rd")<<endl
                        <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                        <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                        <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                        <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                        <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                        <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                        <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web*Dist.W + Na,bot*Dist.BF = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                        <<endl;          
                }
                else
                {
                    if(PgbrBase.dPlasticNAEff>dH-dTf1) // Top에 위치할 때 
                    {
                        //double dTfc = dH - dPlasticNA;
                        //double dTft = dPlasticNA - (dH-dTF1);
                        //dNa_topc = dB1*dTfc*dfyd_t;
                        //dNa_topt = dB1*dTft*dfyd_t;
                        //dDistTc = 0.5*dtc + dHh + 0.5*dTfc;
                        //dDistTt = 0.5*dtc + dHh + dTfc + 0.5*dTft;
                        //dDistB = dHt - 0.5*dtc - 0.5*dTF2;
                        //dDistW = dHt - 0.5*dtc - dTF2 - 0.5*dh;
                        //
                        //dMpl_Rd = -dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web*dDistW;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TFc  = ")<<Get_String_Format(PgbrBase.dDistTc)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                            <<_T("     Dist.TFt  = ")<<Get_String_Format(PgbrBase.dDistTt)<<m_strUnitMM<<endl
                            <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top,c  = ")<<Get_String_Format(PgbrBase.dNa_topc)<<m_strUnitForce<<endl
                            <<_T("     Na,top,t  = ")<<Get_String_Format(PgbrBase.dNa_topt)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("-Na,top,c*Dist.TFc + Na,top,t*Dist.TFt + Na,web*Dist.W + Na,bot*Dist.BF = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          

                    }
                    else if(PgbrBase.dPlasticNAEff<dH-dTf1 && PgbrBase.dPlasticNAEff>dTf2) // Web에 위치할 때 
                    {
                        //double dWc1 = dEffeLen;
                        //double dWc2 = dEffeLen;
                        //double dWt = dh-dEffeLen*2.0-dHoleLen;
                        //dNa_webc1 = dWc1*dTw*dfyd_w;
                        //dNa_webc2 = dWc1*dTw*dfyd_w;
                        //dNa_webt = dWt*dTw*dfyd_w;
                        //dDistWc1 = 0.5*dtc + dHh + dTF1 + 0.5*dWc1;
                        //dDistWc2 = 0.5*dtc + dHh + dTF1 + dWc1 + dHoleLen + 0.5*dWc2;
                        //dDistWt  = 0.5*dtc + dHh + dTF1 + dWc1 + dWc2+dHoleLen + 0.5*dWt;
                        //dDistT = dHt - 0.5*dtc - (dH-0.5*dTF1);
                        //dDistB = dHt - 0.5*dtc - 0.5*dTF2;        
                        //
                        //dMpl_Rd = -dNa_top*dDistT-dNa_webc1*dDistWc1-dNa_webc2*dDistWc2+dNa_webt*dDistWt+dNa_bot*dDistB;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                            <<_T("     Dist.Wc1  = ")<<Get_String_Format(PgbrBase.dDistWc1)<<m_strUnitMM<<endl
                            <<_T("     Dist.Wc2  = ")<<Get_String_Format(PgbrBase.dDistWc2)<<m_strUnitMM<<endl
                            <<_T("     Dist.Wt   = ")<<Get_String_Format(PgbrBase.dDistWt)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web,c1 = ")<<Get_String_Format(PgbrBase.dNa_webc1)<<m_strUnitForce<<endl
                            <<_T("     Na,web,c2 = ")<<Get_String_Format(PgbrBase.dNa_webc2)<<m_strUnitForce<<endl
                            <<_T("     Na,web,t  = ")<<Get_String_Format(PgbrBase.dNa_webt)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("-Na,top*Dist.TF - Na,web,c1*Dist.Wc1 - Na,web,c2*Dist.Wc2 + Na,web,t*Dist.Wt + Na,bot*Dist.BF = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          

                    }
                    else if(PgbrBase.dPlasticNAEff<dTf2) // Bot에 위치할 때.
                    {
                        //double dBfc = dTF2 - dPlasticNA;
                        //double dBft = dPlasticNA;
                        //dNa_botc = dB2*dBfc*dfyd_b;
                        //dNa_bott = dB2*dBft*dfyd_b;
                        //dDistBc = 0.5*dtc + dHh + dTF1 + dh + 0.5*dBfc;
                        //dDistBt = 0.5*dtc + dHh + dTF1 + dh + dBfc + 0.5*dBft;
                        //dDistT= dHt - 0.5*dtc - (dH-0.5*dTF1);
                        //dNa_web_Effe = dTw*dEffeLen*dfyd_w;
                        //dDistW1 = 0.5*dtc + dHh + 0.5*dEffeLen;
                        //dDistW2 = 0.5*dtc + dHh + dEffeLen + dHoleLen + 0.5*dEffeLen;
                        //
                        //dMpl_Rd = -dNa_top*dDistT  - dNa_web_Effe*dDistW1 - dNa_web_Effe*dDistW2 - dNa_botc*dDistBc + dNa_bott*dDistBt;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Concrete Center)")<<endl
                            <<_T("     Dist.W1   = ")<<Get_String_Format(PgbrBase.dDistW1)<<m_strUnitMM<<endl
                            <<_T("     Dist.W2   = ")<<Get_String_Format(PgbrBase.dDistW2)<<m_strUnitMM<<endl                
                            <<_T("     Dist.BFc  = ")<<Get_String_Format(PgbrBase.dDistBc)<<m_strUnitMM<<endl
                            <<_T("     Dist.BFt  = ")<<Get_String_Format(PgbrBase.dDistBt)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web_Effe)<<m_strUnitForce<<endl                
                            <<_T("     Na,bot,c  = ")<<Get_String_Format(PgbrBase.dNa_botc)<<m_strUnitForce<<endl
                            <<_T("     Na,bot,t  = ")<<Get_String_Format(PgbrBase.dNa_bott)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("-Na,top*Dist.TF - Na,web*Dist.W1 - Na,web*Dist.W2 - Na,bot,c*Dist.BFc + Na,bot,t*Dist.BFt = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                }
            }
            // Negative일 경우는 강도 계산할 때 PlasticNA가 기준. 
            else // Negative moment.
            {
                /*
                // Rebar 계산.
                dCompfsdAsidsi=0.0;
                dTensfsdAsidsi=0.0;
                double ddsi=0.0;      
                double dAsi=0.0;
                double dNAdsi=0.0;
                double dGap = (dH+dHh+0.5*dtc) - dPlasticNA;    
                for(i=0; i<arRbarPosi.GetSize(); i++)
                {
                _DGN_RBAR_CRC Rbar;
                Rbar = arRbarPosi[i];
                // PlasticNA에서 떨어진 거리.        
                dNAdsi = dGap + Rbar.dz;
                dAsi = Rbar.dArea;
                if(dNAdsi<0.0) // 압축.
                {
                dCompfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
                }
                else
                {
                dTensfsdAsidsi += dAsi*dfsd*fabs(dNAdsi);
                }
                }
                */
                //
                fout<<_T("   - Force in the reinforcement")<<endl
                    <<_T("     SUM(fsd*As*ds),C  = ")<<Get_String_Format(PgbrBase.dCompfsdAsidsi)<<m_strUnitMoment<<endl
                    <<_T("     SUM(fsd*As*ds),T  = ")<<Get_String_Format(PgbrBase.dTensfsdAsidsi)<<m_strUnitMoment<<endl
                    <<endl;          


                if(PgbrBase.dPlasticNAEff>dH)
                {
                    //dDistT = dPlasticNAEff - (dH-0.5*dTF1);
                    //dDistW1 = dPlasticNAEff - (0.5*dEffeLen+dTF2);
                    //dDistW2 = dPlasticNAEff - (+0.5*dEffeLen+dHoleLen+dEffeLen+dTF2);
                    //dDistB = dPlasticNAEff - 0.5*dTF2;      
                    //dNa_web_Effe = dEffeLen*dTw*dfyd_w;
                    //
                    //dMpl_Rd = dNa_top*dDistT + dNa_bot*dDistB + dNa_web_Effe*dDistW1 + dNa_web_Effe*dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
                    fout<<_T("   - Mpl,Rd")<<endl
                        <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Effective Plastic NA)")<<endl
                        <<_T("     Dist.W1   = ")<<Get_String_Format(PgbrBase.dDistW1)<<m_strUnitMM<<endl
                        <<_T("     Dist.W2   = ")<<Get_String_Format(PgbrBase.dDistW2)<<m_strUnitMM<<endl
                        <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                        <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                        <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web_Effe)<<m_strUnitForce<<endl
                        <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                        <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web*Dist.W1 + Na,web*Dist.W2 + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                        <<endl;          
                }
                else
                {
                    if(PgbrBase.dPlasticNAEff>dH-dTf1) // Top에 위치할 때 
                    {
                        //double dTfc = dPlasticNA - (dH-dTF1);
                        //double dTft = dH - dPlasticNA;
                        //dNa_topc = dB1*dTfc*dfyd_t;
                        //dNa_topt = dB1*dTft*dfyd_t;
                        //dDistTc = 0.5*dTfc;
                        //dDistTt = 0.5*dTft;
                        //dDistB = dPlasticNAEff - 0.5*dTF2;
                        //dDistW1 = dPlasticNAEff - (0.5*dEffeLen+dTF2);
                        //dDistW2 = dPlasticNAEff - (+0.5*dEffeLen+dHoleLen+dEffeLen+dTF2);
                        //dNa_web_Effe = dEffeLen*dTw*dfyd_w;
                        //
                        //dMpl_Rd = dNa_topc*dDistTc + dNa_topt*dDistTt + dNa_bot*dDistB + dNa_web_Effe*dDistW1 + dNa_web_Effe*dDistW2 + dCompfsdAsidsi + dTensfsdAsidsi;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TFc  = ")<<Get_String_Format(PgbrBase.dDistTc)<<m_strUnitMM<<_T("  (from Effective Plastic NA)")<<endl
                            <<_T("     Dist.TFt  = ")<<Get_String_Format(PgbrBase.dDistTt)<<m_strUnitMM<<endl
                            <<_T("     Dist.W1   = ")<<Get_String_Format(PgbrBase.dDistW1)<<m_strUnitMM<<endl
                            <<_T("     Dist.W2   = ")<<Get_String_Format(PgbrBase.dDistW2)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top,c  = ")<<Get_String_Format(PgbrBase.dNa_topc)<<m_strUnitForce<<endl
                            <<_T("     Na,top,t  = ")<<Get_String_Format(PgbrBase.dNa_topt)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web_Effe)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("Na,top,t*Dist.TFt + Na,top,c*Dist.TFc + Na,web*Dist.W1 + Na,web*Dist.W2 + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                    else if(PgbrBase.dPlasticNAEff<dH-dTf1 && PgbrBase.dPlasticNAEff>dTf2) // Web에 위치할 때 
                    {
                        //double dWc1 = dEffeLen; 
                        //double dWc2 = dEffeLen; 
                        //double dWt = (dH-dTF1) - dPlasticNAEff;
                        //dNa_webc1 = dWc1*dTw*dfyd_w;
                        //dNa_webc2 = dWc2*dTw*dfyd_w;
                        //dNa_webt = dWt*dTw*dfyd_w;
                        //dDistWc1 = 0.5*dWc1;
                        //dDistWc2 = dWc1+dHoleLen+dWc2*0.5;
                        //dDistWt = 0.5*dWt;
                        //dDistT = (dH-0.5*dTF1) - dPlasticNAEff;
                        //dDistB = dPlasticNAEff - 0.5*dTF2;        
                        //
                        //dMpl_Rd = dNa_top*dDistT+dNa_webc1*dDistWc1+dNa_webc2*dDistWc2+dNa_webt*dDistWt+dNa_bot*dDistB + dCompfsdAsidsi + dTensfsdAsidsi;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Effective Plastic NA)")<<endl
                            <<_T("     Dist.Wc1  = ")<<Get_String_Format(PgbrBase.dDistWc1)<<m_strUnitMM<<endl
                            <<_T("     Dist.Wc2  = ")<<Get_String_Format(PgbrBase.dDistWc2)<<m_strUnitMM<<endl
                            <<_T("     Dist.Wt   = ")<<Get_String_Format(PgbrBase.dDistWt)<<m_strUnitMM<<endl
                            <<_T("     Dist.BF   = ")<<Get_String_Format(PgbrBase.dDistB)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web,c1 = ")<<Get_String_Format(PgbrBase.dNa_webc1)<<m_strUnitForce<<endl
                            <<_T("     Na,web,c2 = ")<<Get_String_Format(PgbrBase.dNa_webc2)<<m_strUnitForce<<endl
                            <<_T("     Na,web,t  = ")<<Get_String_Format(PgbrBase.dNa_webt)<<m_strUnitForce<<endl
                            <<_T("     Na,bot    = ")<<Get_String_Format(PgbrBase.dNa_bot)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web,t*Dist.Wt + Na,web,c1*Dist.Wc1 + Na,web,c2*Dist.Wc2 + Na,bot*Dist.BF + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                    else if(PgbrBase.dPlasticNAEff<dTf2) // Bot에 위치할 때.
                    {
                        //double dBfc = dPlasticNAEff;
                        //double dBft = dTF2 - dPlasticNAEff;
                        //dNa_botc = dB2*dBfc*dfyd_b;
                        //dNa_bott = dB2*dBft*dfyd_b;
                        //dDistBc = 0.5*dBfc;
                        //dDistBt = 0.5*dBft;
                        //dDistT = (dH-0.5*dTF1)-dPlasticNAEff;
                        //dDistW = (dTF2+0.5*dh)-dPlasticNAEff;
                        //
                        //dMpl_Rd = dNa_top*dDistT + dNa_web*dDistW + dNa_botc*dDistBc + dNa_bott*dDistBt + dCompfsdAsidsi + dTensfsdAsidsi;
                        fout<<_T("   - Mpl,Rd")<<endl
                            <<_T("     Dist.TF   = ")<<Get_String_Format(PgbrBase.dDistT)<<m_strUnitMM<<_T("  (from Effective Plastic NA)")<<endl
                            <<_T("     Dist.W    = ")<<Get_String_Format(PgbrBase.dDistW)<<m_strUnitMM<<endl
                            <<_T("     Dist.BFc  = ")<<Get_String_Format(PgbrBase.dDistBc)<<m_strUnitMM<<endl
                            <<_T("     Dist.BFt  = ")<<Get_String_Format(PgbrBase.dDistBt)<<m_strUnitMM<<endl
                            <<_T("     Na,top    = ")<<Get_String_Format(PgbrBase.dNa_top)<<m_strUnitForce<<endl
                            <<_T("     Na,web    = ")<<Get_String_Format(PgbrBase.dNa_web_Effe)<<m_strUnitForce<<endl                
                            <<_T("     Na,bot,c  = ")<<Get_String_Format(PgbrBase.dNa_botc)<<m_strUnitForce<<endl
                            <<_T("     Na,bot,t  = ")<<Get_String_Format(PgbrBase.dNa_bott)<<m_strUnitForce<<endl
                            <<_T("     Mpl,Rd    = ")<<_T("Na,top*Dist.TF + Na,web*Dist.Wt + Na,bot,t*Dist.BFt + Na,bot,c*Dist.BFc + SUM(fsd*As*ds),T + SUM(fsd*As*ds),C = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
                            <<endl;          
                    }
                }
            }
        }

        BOOL bS420, bS460;
        if(bPositive)
        {
            // 6.2.1.2(2)
            bS420 = dfy-m_dZero>=420.0 && dfy+m_dZero<=420.0 ? TRUE : FALSE;
            bS460 = dfy-m_dZero>=460.0 && dfy+m_dZero<=460.0 ? TRUE : FALSE;      
            fout<<_T("   - Reduction factor beta")<<endl
                <<_T("     Xpl/h    = ")<<Get_String_Format(PgbrBase.dXplOverh)<<endl
                <<_T("     Beta     = ")<<Get_String_Format(PgbrBase.dBeta)<<endl
                <<endl;
        }

        // Beta 적용은 정모멘트만.
        //PgbrBase.dMpl_Rd = dBeta*dMpl_Rd;
        //PgbrBase.dNc_f = dNc_f;
        //PgbrBase.dNc = dFx;          // Nc       
        //PgbrBase.dNc_el = 0.0;       // Nc,el    

        // Elastic Resistance
        //Calc_ElasticResistance(ElemK, bPosiI, MatlElem, SectElem, arRbarPosi, SconElem, PgbrBase);
        T_COMF_KEY ComfKey;
        ComfKey.DgnLcomK = PgbrBase.LcomK;
        ComfKey.ElemK = ElemK.first;
        ComfKey.nPos = bPosiI ? 0 : 1;

        T_SECT_STIFFNESS EffeStlSect;    EffeStlSect.Initialize();
        T_SECT_STIFFNESS EffeComSect;    EffeComSect.Initialize();
        //
        CPG_SECT_EFFE EffeSect;
        EffeSect.Initialize();
        //    
        m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCpgSectEffeResult(ElemK, bPosiI, PgbrBase.LcomK, EffeSect);    
        //m_EffeSectMap.Lookup(ComfKey, EffeSect);
        EffeStlSect = EffeSect.EffeStlSect;
        EffeComSect = EffeSect.EffeComSect;

        // Before
        double dStl_Iyy  = EffeStlSect.Ryy;
        double dStl_Czm  = EffeStlSect.Czm;
        double dStl_Czp  = EffeStlSect.Czp;
        double dStl_zDim = dStl_Czp+dStl_Czm;
        double dStl_Stop = dStl_Czp;
        // Composite
        double dIyy  = EffeComSect.Ryy;  
        double dCzm  = EffeComSect.Czm;
        double dCzp  = EffeComSect.Czp;
        double dStop = dH - dCzm;

        //double dSIGa_top=0.0;
        //double dSIGa_bot=0.0;  

        //double dStrStlTop = dMa_Ed/dStl_Iyy*(-dStl_Stop); // dStl_Iyy 이것도 Effective 이어야함. 
        //double dStrComTop = dMc_Ed/dIyy*(-dStop);
        //double dStrStlBot = dMa_Ed/dStl_Iyy*dStl_Czm;
        //double dStrComBot = dMc_Ed/dIyy*dCzm;
        //dSIGa_top = dStrStlTop + dStrComTop; // (-) 압축, (+) 인장.
        //dSIGa_bot = dStrStlBot + dStrComBot;

        fout<<_T("   - Elastic resistance moment Mel,Rd")<<endl
            <<_T("   ()Effective stiffness for girder")<<endl
            <<_T("     Iyy_stl  = ")<<Get_String_Format(dStl_Iyy)<<m_strUnitMM4<<endl
            <<_T("     Zt_stl   = ")<<Get_String_Format(dStl_Czp)<<m_strUnitMM<<endl
            <<_T("     Zb_stl   = ")<<Get_String_Format(dStl_Czm)<<m_strUnitMM<<endl
            <<endl;

        fout<<_T("   ()Effective stiffness for Composite")<<endl
            <<_T("     Iyy_com  = ")<<Get_String_Format(dIyy)<<m_strUnitMM4<<endl
            <<_T("     Zt_com   = ")<<Get_String_Format(dStop)<<m_strUnitMM<<endl
            <<_T("     Zb_com   = ")<<Get_String_Format(dCzm)<<m_strUnitMM<<endl
            <<endl;

        fout<<_T("     Sigma,top_stl  = Ma,Ed/Iyy_stl*Zt_stl = ")<<Get_String_Format(PgbrBase.dStrStlTop)<<m_strUnitStress<<endl
            <<_T("     Sigma,bot_stl  = Ma,Ed/Iyy_stl*Zb_stl = ")<<Get_String_Format(PgbrBase.dStrStlBot)<<m_strUnitStress<<endl
            <<_T("     Sigma,top_com  = Mc,Ed/Iyy_com*Zt_com = ")<<Get_String_Format(PgbrBase.dStrComTop)<<m_strUnitStress<<endl
            <<_T("     Sigma,bot_com  = Mc,Ed/Iyy_com*Zb_com = ")<<Get_String_Format(PgbrBase.dStrComTop)<<m_strUnitStress<<endl
            <<_T("     Sigma,top      = Sigma,top_stl + Sigma,top_com = ")<<Get_String_Format(PgbrBase.dSIGa_top)<<m_strUnitStress<<endl
            <<_T("     Sigma,bot      = Sigma,bot_stl + Sigma,bot_com = ")<<Get_String_Format(PgbrBase.dSIGa_top)<<m_strUnitStress<<endl
            <<endl;

        if(fabs(PgbrBase.dSIGa_top)>=fabs(PgbrBase.dSIGa_bot))
        {
            //dk_min = dStrComTop==0.0 ? 0.0 : (dfy-fabs(dStrStlTop))/fabs(dStrComTop);
            fout<<_T("     Sigma,top>=Sigma,bot")<<endl
                <<_T("     k    = (fy-ABS(Sigma,top_stl))/ABS(Sigma,top_com) = ")<<Get_String_Format(PgbrBase.dk_min)<<endl
                <<endl;
        }
        else
        {
            //dk_min = dStrComTop==0.0 ? 0.0 : (dfy-fabs(dStrStlBot))/fabs(dStrComBot);
            fout<<_T("     Sigma,top<Sigma,bot")<<endl
                <<_T("     k    = (fy-ABS(Sigma,bot_stl))/ABS(Sigma,bot_com) = ")<<Get_String_Format(PgbrBase.dk_min)<<endl
                <<endl;
        } 

        //dMel_Rd = fabs(dMa_Ed) + dk_min*fabs(dMc_Ed);
        fout<<_T("     Mel,Rd = Ma,Ed + k*Mc,Ed = ")<<Get_String_Format(PgbrBase.dMel_Rd)<<m_strUnitMoment<<endl
            <<endl;

        // Positive 일 때만 계산.
        //if(dMy>=0.0)
        //{
        //  dNc_el = dk_min*dMc_Ed/dIyy*(dStop-0.5*dtc);
        //}

        BOOL bNonLinear =  FALSE;
        if(dXplOverh>0.4){bNonLinear = TRUE;}

        // Non-linear Resistance
        if(bPositive && (bS420 || bS460) && bNonLinear)
        {
            //dNc_el = dk_min*dMc_Ed/dIyy*(dStop-0.5*dtc);
            //Calc_NonLinearResistance(ElemK, bPosiI, PgbrBase);

            fout<<_T("  -  Non-linear resistance moment M,Rd")<<endl
                <<_T("     Nc,el = k*Mc,Ed/Iyy_com*(Zt_com-0.5*tc) = ")<<Get_String_Format(PgbrBase.dNc_el)<<m_strUnitForce<<endl;
            if(fabs(PgbrBase.dNc)<=PgbrBase.dNc_el)
            {
                //dM_Rd = dNc_el==0.0 ? dMa_Ed : dMa_Ed+(dMel_Rd-dMa_Ed)*fabs(dNc)/dNc_el;
                fout<<_T("     M_Rd = Ma,Ed+(Mel,Rd-Ma,Ed)*Nc/Nc,el")<<Get_String_Format(PgbrBase.dM_Rd)<<m_strUnitMoment<<endl;
            }
            else if(PgbrBase.dNc_el<=fabs(PgbrBase.dNc) && fabs(PgbrBase.dNc)<=PgbrBase.dNc_f)
            {
                if(dXpl>dtc)
                {
                    //dNc_f = dBc*dtc*dAlpha*dfcd;
                    fout<<_T("     Nc,f = Bc*tc*Alpha*fcd = ")<<Get_String_Format(PgbrBase.dNc_f)<<m_strUnitForce<<endl;            
                }
                else if(dXpl<dtc)
                {
                    //dNc_f = dBc*dXpl*dAlpha*dfcd;
                    fout<<_T("     Nc,f = Bc*Xpl*Alpha*fcd = ")<<Get_String_Format(PgbrBase.dNc_f)<<m_strUnitForce<<endl;
                } 

                //dM_Rd = (dNc_f-dNc_el)==0.0 ? dMel_Rd : dMel_Rd+(dMpl_Rd-dMel_Rd)*(fabs(dNc)-dNc_el)/(dNc_f-dNc_el);
                fout<<_T("     M_Rd = Mel,Rd+(Mpl,Rd-Mel,Ed)*(Nc-Nc,el)/(Nc,f-Nc,el) = ")<<Get_String_Format(PgbrBase.dM_Rd)<<m_strUnitMoment<<endl;            
            }
            else 
            {
            }
        }
        else
        {
            //PgbrBase.dM_Rd = iClassSect==1 || iClassSect==2 ? PgbrBase.dMpl_Rd : PgbrBase.dMel_Rd;
            if(PgbrBase.iSectClass==1 || PgbrBase.iSectClass==2)
                fout<<_T("     M,Rd = Mpl,Rd = ")<<Get_String_Format(PgbrBase.dM_Rd)<<m_strUnitMoment<<endl;
            else
                fout<<_T("     M,Rd = Mel,Rd = ")<<Get_String_Format(PgbrBase.dM_Rd)<<m_strUnitMoment<<endl;

        }
    }

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_VerticalShearResistance(ElemPairK ElemK, BOOL bPosiI)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    T_PRVS_D PrvsD;
    if(!m_pAnalysisResult->GetPrvsResultData(ElemK, PrvsD)) {return FALSE;}

    T_PRVS_BASE PrvsBase;
    PrvsBase = bPosiI ? PrvsD.PrvsBase[0] : PrvsD.PrvsBase[1];

    BOOL bSect=TRUE, bMatl=TRUE, bTses=TRUE;
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem)) {bSect=FALSE;}

    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {bMatl=FALSE;}

    CPG_TSES_ELEM TsesElem;
    if(Get_CpgTsesElem(ElemK, TsesElem)) {bTses=FALSE;}

    CPG_TSES_POSI TsesPosi;
    TsesPosi = bPosiI ? TsesElem.TsesPosi[0] : TsesElem.TsesPosi[1];    

    // 1. Section.  
    T_SECT_D& SectD = SectElem.SectD;
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 2. Material
    T_MATD_D& MatdD	= MatlElem.MatdD;
    int iMatClass		= MatlElem.iMatClass;
    double dfy		= MatdD.Data1.Design.S_Fy1;
    double dfy1		= MatdD.Data1.Design.S_Fy2;
    double dfu		= MatdD.Data1.Design.S_Fu;
    double dEs		= MatdD.Data1.Analysis.Elast;
    double dfck		= MatlElem.dfck;
    double dfcm		= MatlElem.dfcm;
    double dfctm		= MatlElem.dfctm;
    double dEcm		= MatlElem.dEcm;
    double dfsk		= MatlElem.MatdD.MainRebarData.B_fy;
    double dErs		= dEs; 
    double dgam_M0	= m_dGamma_M0;
    double dgam_M1	= m_dGamma_M1;
    double dgam_sr	= m_dGamma_S;
    double dgam_c		= m_dGamma_C;  
    double dfcd		=  0.67* dfck/dgam_c;
    double dfsd		= fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t		= fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b		= fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w		= fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // Load combination
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
        _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
        _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};

    // Lcom
    CString strLoadComb;    
    T_LCOM_D LcomData;      
    if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PrvsBase.OrgLcomK, LcomData)) 
    {strLoadComb.Format(_T("%s(%s)"),LcomData.LoadCombName, aType[PrvsBase.iMax]);}
    else 
    {strLoadComb = _T("-");}

    //
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    T_SECT_STIFFNESS StiffData;  StiffData.Initialize();
    BOOL bTapered =  m_pDgnSectUtil->Is_TaperedSect(&SectD);
    if(!bTapered)  
    {StiffData = SectD.SectBefore.Stiffness;}
    else
    {StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;}

    // Girder Stiffness	
    double dIyy_f = StiffData.Ryy;
    double dIzz_f = StiffData.Rzz;
    double dCzp_f = StiffData.Czp;
    double dCzm_f = StiffData.Czm;

    //T_SECT_STIFFNESS CompoSect;
    //CompoSect = dMy>=0.0 ? RebarSect : NonConSect;
    //double dIyy_c = CompoSect.Ryy;
    //double dCzp_c = CompoSect.Czp;
    //double dCzm_c = CompoSect.Czm;  

    fout<<endl
        <<_T("--------------------------------------------------------------------------------------")<<endl      
        <<endl
        <<_T("  *. Resistance to vertical shear")<<endl
        <<endl
        <<_T("    - Load Combination     = ")<<strLoadComb<<endl   
        <<_T("      V,Ed = ")<<Get_String_Format(PrvsBase.dV_Ed)<<m_strUnitForce<<endl
        <<endl;

    if(iMatClass==1) // S235, S275, S355 계열.
    {
        //dEta = 1.20*dgam1/dgam0;
        fout<<_T("     eta    = 1.20*gamma1/gamma0 = ")<<Get_String_Format(PrvsBase.deta)<<endl;
    }
    else if(iMatClass==2) // S420, S460 계열.
    {
        //dEta = 1.05*dgam1/dgam0;
        fout<<_T("     eta    = 1.05*gamma1/gamma0 = ")<<Get_String_Format(PrvsBase.deta)<<endl;
    }
    else if(iMatClass==3) // etc.
    {
        //dEta = 1.00*dgam1/dgam0;
        fout<<_T("     eta    = 1.00*gamma1/gamma0 = ")<<Get_String_Format(PrvsBase.deta)<<endl;
    }
    else
    {
    }


    // Plastic resistance to vertical shear, EN 1993-1-1: 2005, 6.2.6 (50page)

    //dAv = dEta*dHw*dTw;
    //dVpl_Rd = dgam_M0==0.0 ? 0.0 : dAv*(dfyd_w/sqrt(3.0))/dgam_M0;
    fout<<_T("     Av     = eta*Hw*tw = ")<<Get_String_Format(PrvsBase.dAv)<<m_strUnitMM2<<endl
        <<_T("     Vpl,Rd = Av*(fyd/Root(3))/gamma,M0 = ")<<Get_String_Format(PrvsBase.dVpl_Rd)<<m_strUnitForce<<endl
        <<endl;  

    // Shear buckling resistance, EN 1993-1-5,5 (22page)
    BOOL bEndSupport = TsesPosi.bUseStif ? TRUE : FALSE;
    int iEndpost = TsesPosi.iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post,
    //BOOL bLongStiff = arLstiPosi.GetSize()==0 ? FALSE : TRUE;
    //BOOL bTranStiff = TrstPosi.bUseStif;


    //

    // I_sl is the sum of the stiffness of the individual stiffeners. EN 1993-1-5 : 2004, 42page
    //
    fout<<_T("   - The second moment of area of a longitudinal stiffener")<<endl
        <<_T("     I,sl   = ")<<Get_String_Format(PrvsBase.dIsl_org)<<m_strUnitMM4<<endl
        <<_T("     I,sl   = 1/3*I,sl = ")<<Get_String_Format(PrvsBase.dIsl)<<m_strUnitMM4<<endl
        <<endl;  

    //
    fout<<_T("   - Contribution from thw web Xw to shear buckling resistance")<<endl;
    if(!bEndSupport || (bEndSupport && iEndpost==2)) //2=No end post
    {
        // 이 경우 기준이 없음. 그래서 dXw=1
        //dXw = 1.0;  
        //
        //dDist4Shr = 1.0; //stiffener가 없으면 1로 한다. 
        fout<<_T("     Xw  = ")<<Get_String_Format(PrvsBase.dXw)<<endl;
    }
    else
    {    
        //
        //if(!bLongStiff && bTranStiff)

        // 5.3(3) The slenderness parameter (Lamda_w) , EN 1993-1-5 : 2004, 23page

        //
        fout<<_T("     Lamda,w  = ")<<Get_String_Format(PrvsBase.dMax_Bar_Lamda_w)<<endl
            <<_T("     Xw       = ")<<Get_String_Format(PrvsBase.dXw)<<endl;

        //
    }

    // contribution from the web factor
    // 5.3(5) For webs with longitudinal stiffeners the slenderness parameter (lamda_w) in (3) should not be taken as less than ,EN 1993-1-5 : 2004, 24page   
    //if(dMax_Bar_Lamda_w<dMax_Lamda_wi)
    //  dMax_Bar_Lamda_w = dMax_Lamda_wi;

    //Get_ContributionFromWebFactor(iEndpost,dEta,dMax_Bar_Lamda_w,dXw);

    //dVbw_Rd = dgam_M1==0.0 ? 0.0 : dXw*dfyd_w*dHw*dTw/(sqrt(3.0)*dgam_M1);

    fout<<endl
        <<_T("   - Contribution from the web")<<endl
        <<_T("     Vbw,Rd  = Xw*fyw*hw*tw/(Root(3)*gamma1) = ")<<Get_String_Format(PrvsBase.dVbw_Rd)<<endl
        <<endl;


    // EN 1993-1-5 7.1 (28page)
    // 7.1 Interaction between shear force, bending moment and axial force
    //double dMf_Rd = 0.0;
    //double dMpl_Rd = 0.0;
    //double dWtop_f = dCzp_f==0.0 ? 0.0 : dIyy_f/dCzp_f;
    //double dWbot_f = dCzm_f==0.0 ? 0.0 : dIyy_f/dCzm_f;
    //double dWtop_c = dCzp_c==0.0 ? 0.0 : dIyy_c/dCzp_c;
    //double dWbot_c = dCzm_c==0.0 ? 0.0 : dIyy_c/dCzm_c;
    //

    //dMf_Rd = PrvsBase.dMf_Rd;
    //dMpl_Rd = PrvsBase.dMpl_Rd; // Bending의 Mpl_Rd와 다를 수 있다. 

    //CPG_SECT_EFFE EffeSect;
    //EffeSect.Initialize();
    //if(!m_EffeSectMap.Lookup(ComfKey, EffeSect))
    //  ASSERT(0);

    //T_SECT_STIFFNESS EffeStlSect;
    //EffeStlSect.Initialize();
    //T_SECT_STIFFNESS EffeComSect;
    //EffeComSect.Initialize();

    //EffeStlSect = EffeSect.EffeStlSect;
    //EffeComSect = EffeSect.EffeComSect;

    // Before
    /*
    double dStl_Iyy  = EffeStlSect.Ryy;
    double dStl_Czm  = EffeStlSect.Czm;
    double dStl_Czp  = EffeStlSect.Czp;
    double dStl_zDim = dStl_Czp+dStl_Czm;
    double dStl_Stop = dStl_Czp;
    // Composite
    double dIyy  = EffeComSect.Ryy;  
    double dCzm  = EffeComSect.Czm;
    double dCzp  = EffeComSect.Czp;
    double dzDim = dCzm+dCzp;
    double dStop = dH - dCzm;

    double dStrStlTop = dMa_Ed/dStl_Iyy*(-dStl_Stop); // dStl_Iyy 이것도 Effective 이어야함. 
    double dStrComTop = dMc_Ed/dIyy*(-dStop);
    double dStrStlBot = dMa_Ed/dStl_Iyy*dStl_Czm;
    double dStrComBot = dMc_Ed/dIyy*dCzm;
    double dSIGa_top = dStrStlTop + dStrComTop;
    double dSIGa_bot = dStrStlBot + dStrComBot;

    double dM_Ed_top = dSIGa_top*(dIyy/dStop);
    double dM_Ed_bot = dSIGa_bot*(dIyy/dCzm);;
    double dM_Ed = max(fabs(dM_Ed_top), fabs(dM_Ed_bot));
    double dV_Ed = PrvsBase.dV_Ed;

    double dBendingRat = dMpl_Rd==0.0 ? 0.0 : dMf_Rd/dMpl_Rd;
    double dBar_Eta1 = dMpl_Rd==0.0 ? 0.0 : dM_Ed/dMpl_Rd;
    double dBar_Eta3 = dVbw_Rd==0.0 ? 0.0 : fabs(dV_Ed)/dVbw_Rd;

    double dCombinedEffe=0.0;
    dCombinedEffe = dBar_Eta1+(1.0-dBendingRat)*pow((2.0*dBar_Eta3-1.0),2);

    BOOL bEta3Exceed05=FALSE;
    BOOL bCombinedEffe=FALSE;
    if(dBar_Eta3>0.5)
    bEta3Exceed05 = TRUE;

    */

    // Contribution from flange, EN 1993-1-5 , 25page
    //double dAf_top = dB1*dTF1;
    //double dAf_bot = dB2*dTF2;  
    //double dbf=0.0;
    //double dtf=0.0;
    //double dfyf=0.0;
    //double da = dDist4Shr;  // _T("a") is the distance bethween transverse stiffeners 

    fout<<_T("   - Contribution from the flanges")<<endl
        <<_T("     M,Ed  = ")<<Get_String_Format(PrvsBase.dM_Ed)<<m_strUnitMoment<<endl
        <<_T("     Mf,Rd = ")<<Get_String_Format(PrvsBase.dMf_Rd)<<m_strUnitMoment<<endl
        <<_T("     fyw   = ")<<Get_String_Format(dfyd_w)<<m_strUnitStress<<endl
        <<_T("     fyf   = ")<<Get_String_Format(dfyd_t)<<m_strUnitStress<<endl
        <<_T("     bf    = ")<<Get_String_Format(PrvsBase.dbf)<<m_strUnitMM<<endl
        <<_T("     tf    = ")<<Get_String_Format(PrvsBase.dtf)<<m_strUnitMM<<endl;


    /*
    if(dAf_top<=dAf_bot)
    {
    dfyf = dfyd_t;
    dtf = dTF1;
    double dbf_lim = 15.0*dEpsi*dtf;
    double dbf_h = (dB1-dTw)*0.5;
    if(dbf_h>dbf_lim)
    dbf = dTw+dbf_lim*2.0;
    else
    dbf = dB1;    
    }
    else
    {
    dfyf = dfyd_b;
    dtf = dTF2;
    double dbf_lim = 15.0*dEpsi*dtf;
    double dbf_h = (dB2-dTw)*0.5;
    if(dbf_h>dbf_lim)
    dbf = dTw+dbf_lim*2.0;
    else
    dbf = dB2;    
    }
    */

    //double dc = dTw*pow(dHw,2)*dfyd_w==0.0 ? 0.0 : da*(0.25+1.6*dbf*pow(dtf,2)*dfyf/(dTw*pow(dHw,2)*dfyd_w));
    fout<<_T("     c = a*(0.25+1.6*bf*tf^2*fyf/(tw*hw^2*fyw) = ")<<Get_String_Format(PrvsBase.dc)<<m_strUnitMM<<endl;
    if(PrvsBase.dMf_Rd!=0.0 && PrvsBase.dM_Ed<PrvsBase.dMf_Rd)
    {
        //dVbf_Rd = dc*dgam_M1==0.0 ? 0.0 : dbf*pow(dtf,2)*dfyf/(dc*dgam_M1)*(1.0-pow(dM_Ed/dMf_Rd,2));
        fout<<_T("     Vbf,Rd  = bf*tf^2*fyf/(c*gamma,M1)*(1-(M,Ed/Mf,Rd)^2) = ")<<Get_String_Format(PrvsBase.dVbf_Rd)<<m_strUnitForce<<endl
            <<endl;
    }
    else
    {
        //dVbf_Rd = 0.0;
        fout<<_T("     Mf,Rd = 0 or M,Ed > Mf,Rd")<<endl
            <<_T("     Vbf,Rd  = ")<<Get_String_Format(PrvsBase.dVbf_Rd)<<m_strUnitForce<<endl
            <<endl;
    }  

    //
    //dVb_Rd = dVbw_Rd + dVbf_Rd;
    fout<<_T("     Vb,Rd = Vbw,Rd + Vbf,Rd  = ")<<Get_String_Format(PrvsBase.dVb_Rd_org)<<m_strUnitForce<<endl;
    //double dVb_Rd_Lim = dgam_M1==0.0 ? 0.0 : dEta*dfyd_w*dHw*dTw/(sqrt(3.0)*dgam_M1);
    if(PrvsBase.dVb_Rd_org>PrvsBase.dVb_Rd_Lim)
    {
        //dVb_Rd = dVb_Rd_Lim;
        fout<<_T("     Vb,Rd,Limit = eta*fyw*Hw*Tw/(Root(3)*gamma,M1) = ")<<Get_String_Format(PrvsBase.dVb_Rd_Lim)<<m_strUnitForce<<endl
            <<_T("     Vb,Rd > Vb,Rd,Limit")<<endl
            <<_T("     Vb,Rd = Vb,Rd,Limit = ")<<Get_String_Format(PrvsBase.dVb_Rd)<<m_strUnitForce<<endl
            <<endl;
    }

    //double dBendingRat = dMpl_Rd==0.0 ? 0.0 : dMf_Rd/dMpl_Rd;
    //double dBar_Eta1 = dMpl_Rd==0.0 ? 0.0 : dM_Ed/dMpl_Rd;
    //double dBar_Eta3 = dVbw_Rd==0.0 ? 0.0 : fabs(dV_Ed)/dVbw_Rd;

    //double dCombinedEffe=0.0;
    //dCombinedEffe = dBar_Eta1+(1.0-dBendingRat)*pow((2.0*dBar_Eta3-1.0),2);
    if(PrvsBase.bEta3Exceed05 && PrvsBase.bEta1Ratio)
    {
        fout<<_T("    - Eta3>0.5 and Eta1>Mf,Rd/Mpl,Rd ")<<endl
            <<_T("     Eta1  = M,Ed/Mpl,Rd = ")<<Get_String_Format(PrvsBase.dEta1)<<endl
            <<_T("     Eta3  = V,Ed/Vbw,Rd = ")<<Get_String_Format(PrvsBase.dEta3)<<endl;

        CString strSign; strSign = PrvsBase.bCombinedEffe ? _T("  <  1.0") : _T("  >  1.0");
        fout<<_T("     Eta1+(1-Mf,Rd/Mpl,Rd)*(2*Eta3-1)^2 = ")<<Get_String_Format(PrvsBase.dCombinedEffe)<<strSign<<endl
            <<endl;
    }


    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_LateralTorsionalBucklingResistance(ElemPairK ElemK, BOOL bPosiI)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    T_PLTB_D PltbD;
    if(!m_pAnalysisResult->GetPltbResultData(ElemK, PltbD)) {return FALSE;}

    T_PLTB_BASE PltbBase;
    PltbBase = bPosiI ? PltbD.PltbBase[0] : PltbD.PltbBase[1];

    BOOL bSect=TRUE, bMatl=TRUE, bTses=TRUE, bLtbd=TRUE, bMemb=TRUE;
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem)) {bSect=FALSE;}

    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {bMatl=FALSE;}

    CPG_TSES_ELEM TsesElem;
    if(!Get_CpgTsesElem(ElemK, TsesElem)) {bTses=FALSE;}

    CPG_TSES_POSI TsesPosi;
    TsesPosi = bPosiI ? TsesElem.TsesPosi[0] : TsesElem.TsesPosi[1];    

    CPG_LTBD_ELEM LtbdElem;  LtbdElem.Initialize();
    if(!Get_CpgLtbdElem(ElemK, LtbdElem)) {bLtbd=FALSE;}
    // Later-torsional buckling data
    CPG_LTBD_POSI LtbdPosi;
    LtbdPosi = bPosiI ? LtbdElem.LtbdPosi[0] : LtbdElem.LtbdPosi[1];

    CPG_MEMB_ELEM MembElem;  MembElem.Initialize();
    if(!Get_CpgMembElem(ElemK, MembElem)) {bMemb=FALSE;}
    // Member data, Lu
    CPG_MEMB_POSI MembPosi;
    MembPosi = bPosiI ? MembElem.MembPosi[0] : MembElem.MembPosi[1];

    // 1. Section.  
    T_SECT_D& SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 2. Material
    T_MATD_D& MatdD = MatlElem.MatdD;
    int iMatClass = MatlElem.iMatClass;
    double dfy  = MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    double dPoissonS = MatdD.Data1.Analysis.Poisson;
    double dGs  = dEs/(1.0+dPoissonS); //MatdD.Data1.Analysis.ShearModulus; ?
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs;
    double dgam_M0 = m_dGamma_M0;
    double dgam_M1 = m_dGamma_M1;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;
    double dfcd =  0.67* dfck/dgam_c;
    double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // Load combination
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
        _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
        _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};

    // Lcom
    CString strLoadComb;    
    T_LCOM_D LcomData;      
    if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PltbBase.OrgLcomK, LcomData)) 
        strLoadComb.Format(_T("%s(%s)"),LcomData.LoadCombName, aType[PltbBase.iMax]);
    else 
        strLoadComb = _T("-");

    //
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    T_SECT_STIFFNESS StiffData;
    StiffData.Initialize();
    if(!bTapered)  
        StiffData = SectD.SectBefore.Stiffness;
    else
        StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;

    // Girder Stiffness	
    double dArea_g = StiffData.Area;
    double dIxx_g = StiffData.Rxx;
    double dIyy_g = StiffData.Ryy;
    double dIzz_g = StiffData.Rzz;
    double dCzp_g = StiffData.Czp;
    double dCzm_g = StiffData.Czm;

    T_SECT_STIFFNESS CompoSect;
    CompoSect = PltbBase.dMy>=0.0 ? RebarSect : NonConSect;
    double dArea_c = CompoSect.Area;
    double dIyy_c  = CompoSect.Ryy;
    double dCzp_c  = CompoSect.Czp;
    double dCzm_c  = CompoSect.Czm;  

    //
    T_PGBR_D PgbrD;
    PgbrD.Initialize();
    T_PGBR_LCOM PgbrLcomD;
    PgbrLcomD.Initialize();
    int iDgnLcomNo = PltbBase.LcomK;
    int iMaxMin=0;
    if(!m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PgbrLcomD)) 
    {
    }    

    T_PGBR_BASE PgbrBase;
    PgbrBase = bPosiI ? PgbrLcomD.PgbrBase[0] : PgbrLcomD.PgbrBase[1];
    //dMpl_Rd = PgbrBase.dMpl_Rd;
    //dMel_Rd = PgbrBase.dMel_Rd;


    fout<<endl
        <<_T("--------------------------------------------------------------------------------------")<<endl
        <<endl
        <<_T("  *. Resistance to lateral-torsional buckling")<<endl
        <<endl
        <<_T("    - Load Combination     = ")<<strLoadComb<<endl   
        <<_T("      N,Ed    = ")<<Get_String_Format(PltbBase.dN_Ed)<<m_strUnitForce<<endl
        <<_T("      M,Ed    = ")<<Get_String_Format(PltbBase.dM_Ed)<<m_strUnitMoment<<endl
        <<_T("      V1      = ")<<Get_String_Format(PltbBase.dV1)<<m_strUnitForce<<endl
        <<_T("      V2      = ")<<Get_String_Format(PltbBase.dV2)<<m_strUnitForce<<endl
        <<_T("      M1      = ")<<Get_String_Format(PltbBase.dM1)<<m_strUnitMoment<<endl
        <<_T("      M2      = ")<<Get_String_Format(PltbBase.dM2)<<m_strUnitMoment<<endl
        <<_T("      Mpl,Rd  = ")<<Get_String_Format(PgbrBase.dMpl_Rd)<<m_strUnitMoment<<endl
        <<_T("      Mel,Rd  = ")<<Get_String_Format(PgbrBase.dMel_Rd)<<m_strUnitMoment<<endl
        <<endl;
    /*
    int iClassSect = ClassD.iClassSect;

    // 6.4 Lateral-torsional buckling of composite beams

    double dX_LT = 0.0; // reduction factor for lateral-torsional buckling to the relative slenderness Lamda_LT
    if(iClassSect == 1 || iClassSect==2)
    dM_Rd = dMpl_Rd;
    else if(iClassSect == 3)
    dM_Rd = dMel_Rd;
    else if(iClassSect == 4)      
    dM_Rd = dMel_Rd;  
    else ASSERT(0);
    */
    // Lateral torsional bucking curves-General case, EN 1993-1-1, 6.3.2.2, 61p, 

    //double dLamda_LT = 0.0;
    double dL = MembPosi.dLu; // span length between the rigid supports. L은 어디서 입력 받는가?.
    double db = PltbBase.dMy>=0.0 ? dB1 : dB2;
    double dtf = PltbBase.dMy>=0.0 ? dTf1 : dTf2;
    //double dAwc = 0.0;
    //double dAf = 0.0;
    // Awc는 Class 4 일 경우는 effective Area를 고려. 
    //if(dCzm_c<dH-dTF1)
    //{ 
    //  dAwc = dMy>=0.0 ? (dH-dTF1-dCzm_c)*dTw : (dCzm_c-dTF2)*dTw;
    //  dAf = dMy>=0.0 ? dB1*dTF1 : dB2*dTF2;
    //}
    //else
    //{
    //  dAwc = dMy>=0.0 ? 0.0 : dHw*dTw;
    //  dAf = dMy>=0.0 ? 0.0 : dB2*dTF2;
    //}

    //double dV1 = PltbBase.dV1; // member개념으로 요소의 I단 전단력
    //double dV2 = PltbBase.dV2; // J단 전단력
    //double dM1 = PltbBase.dM1; // I 단 모멘트
    //double dM2 = PltbBase.dM2; // J 단 모멘트
    //double dCd = LtbdPosi.dCd; // spring stiffness
    //double dl = LtbdPosi.dl;  //distance between the springs.  
    //double dcc = dl==0.0 ? 0.0 : dCd/dl;
    //double dgamma = dcc*pow(dL,4)/(dEs*dIyy_c);  
    //double dNu = 0.0; 
    //if(fabs(dV1)>=fabs(dV2))
    //  dNu = dV1==0.0 ? 0.0 : fabs(dV2/dV1);
    //else
    //  dNu = dV2==0.0 ? 0.0 : fabs(dV1/dV2);
    //
    //double dPHI = Get_PHIform(dM1,dM2,dNu);
    //
    //double dm = 0.0;
    //Get_m_Value(dNu,dPHI,dgamma,dm);    
    // Designer's guide to EN 1994-2
    // Eurocode 4: Design of steel and composite structures, (D6.14), 102p    
    //Get_NonDimensionalSlenderness(dL,db,dfy,dEs,dm,dAwc,dAf,dLamda_LT);  

    // 
    fout<<_T("    - Mb,Rd")<<endl     
        <<_T("      L     = ")<<Get_String_Format(dL)<<m_strUnitMM<<endl
        <<_T("      c     = Cd/l = ")<<Get_String_Format(PltbBase.dcc)<<m_strUnitStress<<endl
        <<_T("      gamma = c*L^4/(E*I) = ")<<Get_String_Format(PltbBase.dgamma)<<endl
        <<endl;

    if(fabs(PltbBase.dV1)>=fabs(PltbBase.dV2))
        fout<<_T("      mu  = V2/V1 = ")<<Get_String_Format(PltbBase.dMu)<<endl;
    else
        fout<<_T("      mu  = V1/V2 = ")<<Get_String_Format(PltbBase.dMu)<<endl;

    fout<<_T("      PHI = 2(1-M2/M1)/(1+mu) = ")<<Get_String_Format(PltbBase.dPHI)<<endl       
        <<endl;

    fout<<_T("      m1  = 1+0.44*(1+mu)*PHI^1.5+(3+2*PHI)*gamma/(350-50*mu) = ")<<Get_String_Format(PltbBase.dm1)<<endl
        <<_T("      m2  = 1+0.44*(1+mu)*PHI^1.5+(0.195+(0.05+mu/100)*PHI)*gamma^0.5 = ")<<Get_String_Format(PltbBase.dm2)<<endl
        <<_T("      m   = Min(m1,m2) = ")<<Get_String_Format(PltbBase.dm_min)<<endl;
    if(PltbBase.dm_min>1.0)
    {
        fout<<_T("      m > 1.0 --> m = ")<<Get_String_Format(PltbBase.dm)<<endl;
    }

    //int iCase = Get_BucklingCurveForCrossSection(dH,db);
    //double dAlpha_LT = Get_ImperfectionFactor(iCase);  
    //double dPHI_LT = Get_PHI_LT(dAlpha_LT, dLamda_LT);
    //dX_LT = Get_X_LT(dPHI_LT,dLamda_LT);
    //
    //double dPHI_LT = 0.5*(1.0+dAlpha*(dLamda_LT-0.2)+pow(dAlpha,2));
    //
    //dX_LT = dPHI_LT+sqrt(dX_LT_Parm1)==0.0 ? 0.0 : 1.0/(dPHI_LT+sqrt(dX_LT_Parm1));
    //if(dX_LT>1.0) 
    //  dX_LT = 1.0;

    // dLamda = 1.103*dL/db*sqrt(dfy/(dEs*dm))*sqrt(1.0+dAwc/(3.0*dAf));

    fout<<_T("      Alpha,LT = ")<<Get_String_Format(PltbBase.dAlpha_LT)<<endl     
        <<_T("      Lamda,LT = 1.103*L/b*Root(fy/(Es*m))*Root(1+Awc/(3*Af)) = ")<<Get_String_Format(PltbBase.dLamda_LT)<<endl
        <<_T("      PHI,LT   = 0.5*(1.0+Alpha,LT*(Lamda,LT-0.2)+Lamda,LT^2) = ")<<Get_String_Format(PltbBase.dPHI_LT)<<endl
        <<_T("      X,LT     = 1/(PHI,LT+Root(PHI,LT^2-Lamda,LT^2)) = ")<<Get_String_Format(PltbBase.dX_LT_org)<<endl;
    if(PltbBase.dX_LT>1.0)
    {
        fout<<_T("      X,LT > 1.0") 
            <<_T("      X,LT     = ")<<Get_String_Format(PltbBase.dX_LT)<<endl;
    }
    fout<<endl;
    //
    fout<<_T("      M,Rd     = ")<<Get_String_Format(PltbBase.dM_Rd)<<m_strUnitMoment<<endl     
        <<_T("      Mb,Rd    = X,LT*M,Rd = ")<<Get_String_Format(PltbBase.dMb_Rd)<<m_strUnitMoment<<endl
        <<endl;


    // design buckling resistance moment
    //dMb_Rd = dX_LT*dM_Rd;

    // N_Ed  
    //dm = 1.0;
    //dAwc = 0.0; // class 4면 effe Area, 아니면 Plastic NA에 대한 Web의 Area.
    //dAf=dAf;
    //double dLamda_LT_N=dLamda_LT;    
    //double dPHI_LT_N = Get_PHI_LT(dAlpha_LT, dLamda_LT_N);
    //double dX_LT_N = Get_X_LT(dPHI_LT_N, dLamda_LT_N);
    //double dNb_Rd = dX_LT_N*dArea_c*dfyd;
    //
    //double dN_Rat = dNb_Rd==0.0 ? 0.0 : fabs(dN_Ed)/dNb_Rd;
    //double dM_Rat = dMb_Rd==0.0 ? 0.0 : fabs(dM_Ed)/dMb_Rd;
    //double dCombRat = dN_Rat + dM_Rat;

    //
    fout<<_T("    - Nb,Rd")<<endl 
        <<_T("      X,LT,N  = ")<<Get_String_Format(PltbBase.dX_LT_N)<<endl
        <<_T("      Nb,Rd   = X,LT,N*Area*fyd = ")<<Get_String_Format(PltbBase.dNb_Rd)<<m_strUnitForce<<endl
        <<endl;
    fout<<_T("      Combined Ratio = N,Ed/Nb,Rd + M,Ed/Mb,Rd = ")<<Get_String_Format(PltbBase.dInterRat)<<endl
        <<endl;


    // U frame model 나중에 고려.
    int iAlpha = LtbdPosi.iAlpha;
    //double dMcr=0.0, dMcr_val=0.0;
    //double dk1=0.0, dk2=0.0;
    //double dk2_val1=0.0;
    // double dks=0.0;
    double dEa=dEs; // girder
    double dI2=CompoSect.Ryy; // girder+steel
    double dv_a=dPoissonS; // Poisson's ratio for Steel
    double dhs = dH-0.5*(dTf1+dTf2);
    double da = LtbdPosi.da;

    //dk1 = da==0.0 ? 0.0 : iAlpha*dEa*dI2/da;
    //dk2_val1=4.0*(1.0-dv_a*dv_a)*dhs;
    //dk2 = dk2_val1==0.0 ? 0.0 : dEa*pow(dTw,3)/dk2_val1;
    //dks = dk1+dk2==0.0 ? 0.0 : dk1*dk2/(dk1+dk2);

    //
    fout<<_T("    - U frame model")<<endl 
        <<_T("      Alpha = ")<<Get_String_Format(iAlpha)<<endl
        <<_T("      hs    = ")<<Get_String_Format(dhs)<<m_strUnitMM<<endl
        <<_T("      a     = ")<<Get_String_Format(da)<<m_strUnitMM<<endl
        <<_T("      Ea    = ")<<Get_String_Format(dEa)<<m_strUnitStress<<endl
        <<_T("      G     = ")<<Get_String_Format(dGs)<<m_strUnitStress<<endl
        <<_T("      I2    = ")<<Get_String_Format(dI2)<<m_strUnitMM4<<endl
        <<_T("      k1    = Alpha*Ea*I2/a =")<<Get_String_Format(PltbBase.dk1)<<endl
        <<_T("      k2    = Ea*tw^3/(4*(1-nu,a^2)*hs) =")<<Get_String_Format(PltbBase.dk2)<<endl
        <<_T("      ks    = k1*k2/(k1+k2) = ")<<Get_String_Format(PltbBase.dks)<<endl      
        <<endl;

    //
    //double dkc=0.0, dkc_val1=0.0, dkc_val2=0.0;  
    //double dC4=PltbBase.dC4;
    //double dI_at=dIxx_g; // St.Venant torsional stiffness of the steel section  
    //double dI_afz=dTF2*pow(dB2,3)/12.0; // ???,  is the flexural stiffness of bottom flange respect the z axis of the steel section
    /*
    double dAa  = dArea_g;
    double dA   = RebarSect.Area; // 전체환산단면.
    double dIay = dIyy_g;
    double dIaz = dIzz_g;
    double dix2 = dAa==0.0 ? 0.0 : (dIay+dIaz)/dAa;
    double dhc  = dH-0.5*(dTf1+dTf2);
    double dSCzm_g = Get_ShearCenter_H(dh,dTF1,dB1,dTF2,dB2); // shear center
    double dZs  = dCzm_g - dSCzm_g; // is the distance between section centre of gravity _T("C") and shear barycentre(positive if shear barycentre is lower than _T("C"))
    double dZc  = dH+dHh+0.5*dtc-dCzm_g;
    double de   = dAa*dZc*(dA-dAa)==0.0 ? 0.0 : dA*dIay/(dAa*dZc*(dA-dAa));
    double dZf  = dIaz==0.0 ? 0.0 : dhs*dI_afz/dIaz;
    double dZj_val2 = Get_Zj_Second_Value(dH,dHw,dTw,dB1,dTF1,dB2,dTF2,dCzm_g,dAa,dIay);  
    double dZj = dZs - dZj_val2; 
    */

    fout<<_T("      C4     = ")<<Get_String_Format(PltbBase.dC4)<<endl
        <<_T("      I,at   = ")<<Get_String_Format(PltbBase.dI_at)<<m_strUnitMM4<<endl
        <<_T("      I,afz  = ")<<Get_String_Format(PltbBase.dI_afz)<<m_strUnitMM4<<endl
        <<_T("      kc     = ")<<Get_String_Format(PltbBase.dkc)<<endl
        <<_T("      Mcr    = kc*C4/L*((G*I,at+ks*L^2/pi^2)*Ea*I,afz)^(1/2) =")<<Get_String_Format(PltbBase.dMcr)<<m_strUnitMoment<<endl
        <<endl;
    //


    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_TransverseForceResistance(ElemPairK ElemK, BOOL bPosiI)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    T_PRTF_D PrtfD;
    if(!m_pAnalysisResult->GetPrtfResultData(ElemK, PrtfD)) {return FALSE;}

    T_PRTF_BASE PrtfBase;
    PrtfBase = bPosiI ? PrtfD.PrtfBase[0] : PrtfD.PrtfBase[1];

    BOOL bSect=TRUE, bMatl=TRUE, bBcdt=TRUE;
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem)) {bSect=FALSE;}

    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {bMatl=FALSE;}

    CPG_BCDT_ELEM BcdtElem;
    BcdtElem.Initialize();
    if(!Get_CpgBcdtElem(ElemK, BcdtElem)) {bBcdt=FALSE;}
    //
    CPG_BCDT_POSI& BcdtPosi = bPosiI ? BcdtElem.BcdtPosi[0] : BcdtElem.BcdtPosi[1];

    // 1. Section.  
    T_SECT_D& SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 2. Material
    T_MATD_D& MatdD = MatlElem.MatdD;
    int iMatClass = MatlElem.iMatClass;
    double dfy  = MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    double dPoissonS = MatdD.Data1.Analysis.Poisson;
    double dGs  = dEs/(1.0+dPoissonS); 
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; 
    double dgam_M0 = m_dGamma_M0;
    double dgam_M1 = m_dGamma_M1;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;  
    double dfcd =  0.67* dfck/dgam_c;
    double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // Load combination
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
        _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
        _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};

    // Lcom
    CString strLoadComb;    
    T_LCOM_D LcomData;      
    if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PrtfBase.OrgLcomK, LcomData)) 
        strLoadComb.Format(_T("%s(%s)"),LcomData.LoadCombName, aType[PrtfBase.iMax]);
    else 
        strLoadComb = _T("-");

    //
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    T_SECT_STIFFNESS StiffData;
    StiffData.Initialize();
    if(!bTapered)  
        StiffData = SectD.SectBefore.Stiffness;
    else
        StiffData = bPosiI ? SectD.SectBefore.SectI.Stiffness : SectD.SectBefore.SectJ.Stiffness;

    fout<<endl
        <<_T("--------------------------------------------------------------------------------------")<<endl
        <<endl
        <<_T("  *. Resistance to Transverse Force")<<endl
        <<endl
        <<_T("    - Load Combination     = ")<<strLoadComb<<endl   
        <<_T("      N,Ed    = ")<<Get_String_Format(PrtfBase.dN_Ed)<<m_strUnitForce<<endl
        <<_T("      My,Ed    = ")<<Get_String_Format(PrtfBase.dMy_Ed)<<m_strUnitMoment<<endl
        <<_T("      Mz,Ed    = ")<<Get_String_Format(PrtfBase.dMz_Ed)<<m_strUnitMoment<<endl
        <<_T("      F,Ed     = ")<<Get_String_Format(PrtfBase.dF_Ed)<<m_strUnitForce<<endl
        <<endl;


    T_SECT_STIFFNESS CompSect;
    CompSect = PrtfBase.dMy_Ed>=0.0 ? RebarSect : NonConSect;

    ///////////////////////////////////////////////////
    T_COMF_KEY ComfKey;
    ComfKey.DgnLcomK = PrtfBase.LcomK;
    ComfKey.ElemK = ElemK.first;
    ComfKey.nPos = bPosiI ? 0 : 1;

    CPG_SECT_EFFE EffeSect;
    EffeSect.Initialize();
    //    
    m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCpgSectEffeResult(ElemK, bPosiI, PrtfBase.LcomK, EffeSect);    

    T_SECT_STIFFNESS EffeStlSect;
    EffeStlSect.Initialize();
    T_SECT_STIFFNESS EffeComSect;
    EffeComSect.Initialize();

    EffeStlSect = EffeSect.EffeStlSect;
    EffeComSect = EffeSect.EffeComSect;

    // Before
    double dStl_Iyy  = EffeStlSect.Ryy;
    double dStl_Czm  = EffeStlSect.Czm;
    double dStl_Czp  = EffeStlSect.Czp;
    double dStl_zDim = dStl_Czp+dStl_Czm;
    double dStl_Stop = dStl_Czp;
    // Composite
    double dIyy  = EffeComSect.Ryy;  
    double dIzz  = EffeComSect.Rzz;  
    double dCzm  = EffeComSect.Czm;
    double dCzp  = EffeComSect.Czp;
    double dzDim = dCzm+dCzp;
    double dStop = dH - dCzm;
    double dy1 = EffeComSect.dx1;
    double dy2 = EffeComSect.dx1;
    double dy3 = EffeComSect.dx3;
    double dy4 = EffeComSect.dx4;
    double dz1 = EffeComSect.dy1;
    double dz2 = EffeComSect.dy1;
    double dz3 = EffeComSect.dy3;
    double dz4 = EffeComSect.dy4;

    double dAeff = 0.0;
    //double dey_N = CompSect.Cym-EffeComSect.Cym;
    //double dez_N = CompSect.Czm-EffeComSect.Czm;
    double dEta1[4]={0., 0., 0., 0.,};
    double dEta1_max=0.0;
    double dWy_eff[4]; // 0=TL, 1=TR, 2=BR, 3=BL
    double dWz_eff[4];
    //double dEta1_fx = dgam_M0==0.0 || dfy*dAeff==0.0 ? 0.0 : dN_Ed/(dfy*dAeff/dgam_M0);
    dWy_eff[0] = dy1==0.0 ? 0.0 : dIyy/dy1;
    dWy_eff[1] = dy2==0.0 ? 0.0 : dIyy/dy2;
    dWy_eff[2] = dy3==0.0 ? 0.0 : dIyy/dy3;
    dWy_eff[3] = dy4==0.0 ? 0.0 : dIyy/dy4;
    //
    dWz_eff[0] = dz1==0.0 ? 0.0 : dIzz/dz1;
    dWz_eff[1] = dz2==0.0 ? 0.0 : dIzz/dz2;
    dWz_eff[2] = dz3==0.0 ? 0.0 : dIzz/dz3;
    dWz_eff[3] = dz4==0.0 ? 0.0 : dIzz/dz4;

    fout<<_T("    - Eta1, 1993-1-5, (4.15) ")<<endl
        <<_T("      ey,N      = ")<<Get_String_Format(PrtfBase.dey_N)<<m_strUnitMM<<endl
        <<_T("      ez,N      = ")<<Get_String_Format(PrtfBase.dez_N)<<m_strUnitMM<<endl
        <<_T("      Wy,eff,TL = ")<<Get_String_Format(dWy_eff[0])<<m_strUnitMM3<<endl
        <<_T("      Wy,eff,TR = ")<<Get_String_Format(dWy_eff[1])<<m_strUnitMM3<<endl
        <<_T("      Wy,eff,BR = ")<<Get_String_Format(dWy_eff[2])<<m_strUnitMM3<<endl
        <<_T("      Wy,eff,BL = ")<<Get_String_Format(dWy_eff[3])<<m_strUnitMM3<<endl
        <<_T("      Wz,eff,TL = ")<<Get_String_Format(dWz_eff[0])<<m_strUnitMM3<<endl
        <<_T("      Wz,eff,TR = ")<<Get_String_Format(dWz_eff[1])<<m_strUnitMM3<<endl
        <<_T("      Wz,eff,BR = ")<<Get_String_Format(dWz_eff[2])<<m_strUnitMM3<<endl
        <<_T("      Wz,eff,BL = ")<<Get_String_Format(dWz_eff[3])<<m_strUnitMM3<<endl    
        <<endl;

    /*
    if(dgam_M0>0)
    {
    // Compression(-), Tension(+)    
    for(int i=0; i<4; i++)
    {       
    double dRat_y = dfy*dWy_eff[i]==0.0 ? 0.0 : (-1.)*(dMy_Ed+dN_Ed*dey_N)/(dfy*dWy_eff[i]/dgam_M0);
    double dRat_z = dfy*dWz_eff[i]==0.0 ? 0.0 : (-1.)*(dMz_Ed+dN_Ed*dez_N)/(dfy*dWz_eff[i]/dgam_M0);
    dEta1[i] = dEta1_fx + dRat_y + dRat_z;    
    if(i==0)
    {
    dEta1_max = fabs(dEta1[0]);
    }
    else
    {
    if(dEta1_max<fabs(dEta1[i]))
    {
    dEta1_max = fabs(dEta1[i]);
    }
    }
    }
    }
    */

    fout<<_T("      Eta1,TL   = ")<<Get_String_Format(PrtfBase.dEta1_p[0])<<endl
        <<_T("      Eta1,TR   = ")<<Get_String_Format(PrtfBase.dEta1_p[1])<<endl
        <<_T("      Eta1,BR   = ")<<Get_String_Format(PrtfBase.dEta1_p[2])<<endl
        <<_T("      Eta1,BL   = ")<<Get_String_Format(PrtfBase.dEta1_p[3])<<endl
        <<_T("      Eta1      = ")<<Get_String_Format(PrtfBase.dEta1)<<endl
        <<endl;

    // EN 1993-1-5, 26page.
    int iLoadAppType = BcdtPosi.iLoadAppType;
    double da  = BcdtPosi.da;
    double dSs = BcdtPosi.dSs;
    double dC  = BcdtPosi.dC;  

    //
    /*
    double dIsl1 = 0.0;
    double db1 = 0.0;
    BOOL bExistLsti = arLstiPosi.GetSize()>0 ? TRUE : FALSE;
    if(bExistLsti)
    {
    CPG_LSTI_DATA LstiD = arLstiPosi[0];
    double dh_ls = LstiD.dWidth;
    double dt_ls = LstiD.dt;
    double dI_ls = 0.0;
    db1 = LstiD.dz-LstiD.dt*0.5;
    if(!Get_IslOfLongiStiff(1,dh_ls, dt_ls, dTw, dEpsi, dIsl1))      
    dIsl1=0.0;
    } 
    */

    fout<<_T("    - Eta2, 1993-1-5, (6.14) ")<<endl
        <<_T("      Calc. k,F")<<endl
        <<_T("      a  = ")<<Get_String_Format(da)<<m_strUnitMM<<endl
        <<_T("      Ss = ")<<Get_String_Format(dSs)<<m_strUnitMM<<endl;


    double db1 = PrtfBase.db1;
    double dRat = da==0.0 ? 0.0 : dh/da;
    double db1hw = dh==0.0 ? 0.0 : db1/dh;
    double db1a  = da==0.0 ? 0.0 : db1/da;

    // 0=Type(a), 1=Type(b), 2=Type(c)
    if(iLoadAppType==0)
    {
        fout<<_T("      Type(a)")<<endl;

        if((0.05<=db1hw && db1hw<=0.3) && (db1a<=0.3))
        {
            /*
            double dGamma_s = dh*dTw==0.0 ? 0.0 : 10.9*dIsl1/(dh*pow(dTw,3));
            double dGamma_s_lim = dh*da==0.0 ? 0.0 : 13.0*pow(da/dh,3)+210*(0.3-db1/da);      
            if(dGamma_s>dGamma_s_lim)
            dGamma_s = dGamma_s_lim;
            dk_F = 6.0+2.0*pow(dh/da,2)+(5.44*db1/da-0.21)*sqrt(dGamma_s);      
            */
            //
            fout<<_T("      0.05<b1/hw<0.3 , b1/a<0.3")<<endl
                <<_T("      Isl,1   = ")<<Get_String_Format(PrtfBase.dIsl1)<<m_strUnitMM4<<endl
                <<_T("      gamma,s = ")<<Get_String_Format(PrtfBase.dgamma_s)<<m_strUnitMM4<<endl
                <<_T("      k,F     = 6+2*[hw/a]^2+[5.44*b1/a-0.21]*Root(gamma,s) = ")<<Get_String_Format(PrtfBase.dk_F)<<endl
                <<endl;
        }
        else
        {
            //dk_F = 6.0+2.0*pow(dRat,2);
            fout<<_T("      k,F   = 6+2*(hw/a)^2 = ")<<Get_String_Format(PrtfBase.dk_F)<<endl;
        }
    }
    else if(iLoadAppType==1)
    {
        //double dRat = da==0.0 ? 0.0 : dh/da;
        //dk_F = 3.5+2.0*pow(dRat,2);
        fout<<_T("      Type(b)")<<endl
            <<_T("      k,F   = 3.5+2*(hw/a)^2 = ")<<Get_String_Format(PrtfBase.dk_F)<<endl;
    }
    else if(iLoadAppType==2)
    {
        //double dRat = dh==0.0 ? 0.0 : (dSs+dC)/dh;
        //dk_F = 2.0+6.0*dRat;
        fout<<_T("      Type(c)")<<endl
            <<_T("      k,F   = 2+6*(Ss+c)/hw<6 = ")<<Get_String_Format(PrtfBase.dk_F)<<endl;
    }
    else
    {    
    }
    fout<<endl;

    //!/ (6.3) 과 (6.9) 식에 모순이 있다.
    double dX_F = 0.0; // reduction factor
    double dl_y = 0.0; // effective loaded length

    // effective loaded length
    double dbf = dB1;
    double dtf = dTf1;
    //double dm1 = dfyd_w*dTw==0.0 ? 0.0 : dfyd_t*dbf/(dfyd_w*dTw);
    //double dm2 = 0.0;

    //double dRat = dtf==0.0 ? 0.0 : dh/dtf;
    //dm2 = 0.02*pow(dRat,2);
    /*
    if(dLamda_F>0.5)
    {
    double dRat = dtf==0.0 ? 0.0 : dh/dtf;
    dm2 = 0.02*pow(dRat,2);
    }
    else
    {
    dm2 = 0.0;
    }
    */

    fout<<_T("    - Calc. ly")<<endl
        <<_T("      m1  = ")<<Get_String_Format(PrtfBase.dm1)<<endl
        <<_T("      m2  = ")<<Get_String_Format(PrtfBase.dm2)<<endl;

    // 
    if(iLoadAppType==0 || iLoadAppType==1)
    {
        //dl_y = dSs + 2.0*dtf*(1.0+sqrt(dm1+dm2));
        if(iLoadAppType==0)
            fout<<_T("      Type(a)")<<endl;
        else
            fout<<_T("      Type(b)")<<endl;
        fout<<_T("      ly  = Ss+2*tf*(1+Root(m1+m2)) = ")<<Get_String_Format(PrtfBase.dly)<<m_strUnitMM<<endl;  
    }
    else if(iLoadAppType==2)
    {    
        /*
        double dl_e = 2.0*dfyd_w*dh==0.0 ? 0.0 : dk_F*dEs*pow(dTw,2)/(2.0*dfyd_w*dh);
        if(dl_e>dSs+dC)
        dl_e = dSs+dC;
        double dl_y1_val = dtf==0.0 ? dm1/2.0+dm2 : dm1/2.0+pow(dl_e/dtf,2)+dm2;
        double dl_y1 = dl_e+dtf*sqrt(dl_y1_val);
        double dl_y2 = dl_e+dtf*sqrt(dm1+dm2);
        dl_y = min(dl_y1,dl_y2);
        */

        fout<<_T("      Type(c)")<<endl
            <<_T("      le   = k,F*E*tw^2/(2*fyw*hw) = ")<<Get_String_Format(PrtfBase.dle)<<m_strUnitMM<<endl
            <<_T("      ly1  = le+tf*Root(m1/2+(le/tf)^2+m2) = ")<<Get_String_Format(PrtfBase.dly1)<<m_strUnitMM<<endl
            <<_T("      ly2  = le+tf*Root(m1+m2) = ")<<Get_String_Format(PrtfBase.dly2)<<m_strUnitMM<<endl
            <<_T("      ly   = min(ly1,ly2) = ")<<Get_String_Format(PrtfBase.dly)<<m_strUnitMM<<endl
            <<endl;
    }
    else
    {
    }

    // reduction factor
    //double dFcr = dh==0.0 ? 0.0 : 0.9*dk_F*dEs*pow(dTw,3)/dh;
    //double dLamda1 = dFcr==0.0 ? 0.0 : dl_y*dTw*dfyd_w/dFcr;
    //double dLamda_F = dLamda1<0.0 ? 0.0 : sqrt(dLamda1);
    //dX_F = dLamda_F==0.0 ? 0.0 : 0.5/dLamda_F;
    //if(dX_F>1.0)
    //    dX_F = 1.0;
    //
    //double dLeff = dX_F*dl_y;
    //double dF_Rd = dgam_M1==0.0 ? 0.0 : dfyd_w*dLeff*dTw/dgam_M1;
    //  
    //double dEta2 = dF_Rd==0.0 ? 0.0 : fabs(dF_Ed)/dF_Rd;

    fout<<endl
        <<_T("    - Calc. F,Rd")<<endl
        <<_T("      Fcr     = 0.9*k,F*E*tw^3/hw = ")<<Get_String_Format(PrtfBase.dFcr)<<m_strUnitForce<<endl
        <<_T("      Lamda,F = Root(ly*tw*fyw/Fcr) = ")<<Get_String_Format(PrtfBase.dLamda_F)<<endl
        <<_T("      X,F     = 0.5/Lamda,F<1.0 = ")<<Get_String_Format(PrtfBase.dX_F)<<endl
        <<_T("      L,eff   = X,F*ly = ")<<Get_String_Format(PrtfBase.dL_eff)<<m_strUnitMM<<endl
        <<_T("      F,Rd    = fyw*L,eff*tw/gamma,M1 = ")<<Get_String_Format(PrtfBase.dF_Rd)<<m_strUnitForce<<endl
        <<_T("      Eta,2    = F,Ed/F,Rd = ")<<Get_String_Format(PrtfBase.dEta2)<<endl
        <<endl;

    //double dInterRat = dEta2 + 0.8*dEta1_max; // 
    fout<<_T("      Eta2 + 0.8*Eta1 < 1.4 = ")<<Get_String_Format(PrtfBase.dInterRat)<<endl
        <<endl;  

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_LongitudinalShearResistance(ElemPairK ElemK, BOOL bPosiI)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    T_PGLS_D PglsD;
    if(!m_pAnalysisResult->GetPglsResultData(ElemK, PglsD)) {return FALSE;}

    T_PGLS_BASE PglsBase;
    PglsBase = bPosiI ? PglsD.PglsBase[0] : PglsD.PglsBase[1];

    BOOL bSect=TRUE, bMatl=TRUE, bScon=TRUE;
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem)) {bSect=FALSE;}

    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) {bMatl=FALSE;}

    CPG_SCON_ELEM SconElem;
    SconElem.Initialize();
    if(!Get_CpgSconElem(ElemK, SconElem)) {bScon=FALSE;}

    // Shear Connector.
    CPG_SCON_POSI SconPosi = bPosiI ? SconElem.SconPosi[0] : SconElem.SconPosi[1];

    // 1. Section.  
    T_SECT_D SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 2. Material
    T_MATD_D& MatdD = MatlElem.MatdD;
    int iMatClass = MatlElem.iMatClass;
    double dfy  = MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    double dPoissonS = MatdD.Data1.Analysis.Poisson;
    double dGs  = dEs/(1.0+dPoissonS); 
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; 
    double dgam_M0 = m_dGamma_M0;
    double dgam_M1 = m_dGamma_M1;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;  
    double dfcd =  0.67* dfck/dgam_c;
    double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // Load combination
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
        _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
        _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};

    // Lcom
    CString strLoadComb;    
    T_LCOM_D LcomData;      
    if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PglsBase.OrgLcomK, LcomData)) 
        strLoadComb.Format(_T("%s(%s)"),LcomData.LoadCombName, aType[PglsBase.iMax]);
    else 
        strLoadComb = _T("-");

    //
    T_SECT_STIFFNESS NonConSect;	NonConSect.Initialize();
    T_SECT_STIFFNESS RebarSect;	RebarSect.Initialize();
    GetNonComSectStiffD(SectElem, bPosiI, NonConSect);
    GetRebarSectStiffD(SectElem, bPosiI, RebarSect);

    //
    T_PGBR_D PgbrD;
    PgbrD.Initialize();
    T_PGBR_LCOM PgbrLcomD;
    PgbrLcomD.Initialize();
    int iDgnLcomNo = PglsBase.LcomK;
    int iMaxMin=0;
    if(!m_pAnalysisResult->GetCpgBendingResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PgbrLcomD)) 
    {
    }    

    T_PGBR_BASE PgbrBase;
    PgbrBase = bPosiI ? PgbrLcomD.PgbrBase[0] : PgbrLcomD.PgbrBase[1];
    //dMpl_Rd = PgbrBase.dMpl_Rd;

    double dMpl_Rd = PgbrBase.dMpl_Rd;
    double dMel_Rd = PgbrBase.dMel_Rd;
    double dNc_el  = PgbrBase.dNc_el;
    double dNc_f   = PgbrBase.dNc_f;  
    //double dM_Ed  = dMy;

    fout<<endl
        <<_T("--------------------------------------------------------------------------------------")<<endl
        <<endl
        <<_T("  *. Resistance to Longitudinal Shear")<<endl
        <<endl
        <<_T("    - Load Combination     = ")<<strLoadComb<<endl   
        <<_T("      Mpl,Rd  = ")<<Get_String_Format(dMpl_Rd)<<m_strUnitMoment<<endl
        <<_T("      Mel,Rd  = ")<<Get_String_Format(dMel_Rd)<<m_strUnitMoment<<endl
        <<_T("      Nc,el   = ")<<Get_String_Format(dNc_el)<<m_strUnitForce<<endl
        <<_T("      Nc,d    = ")<<Get_String_Format(dNc_f)<<m_strUnitForce<<endl
        <<_T("      M,Ed    = ")<<Get_String_Format(PglsBase.dMy)<<m_strUnitMoment<<endl
        <<_T("      V,Ed    = ")<<Get_String_Format(PglsBase.dv_Ed)<<m_strUnitMoment<<endl
        <<endl;

    // EN 1994-2, 6.6 Shear connection, 46page
    int iNum = SconPosi.iNum;
    double dhsc    = SconPosi.dhsc;
    double dfu_sc  = SconPosi.dfu;
    double dDia_sc = SconPosi.dDia;
    //double dSt_sc  = SconPosi.dSt;
    double dSpace  = SconPosi.dSpace;
    double dAlpha = 0.0;
    double dRat_sc = dDia_sc==0.0 ? 0.0 : dhsc/dDia_sc;
    //
    CString strNumSC; strNumSC.Format(_T("%d"),iNum);
    fout<<_T("    - Calc. P,Rd")<<endl
        <<_T("      Num.  = ")<<strNumSC<<endl
        <<_T("      h,sc  = ")<<Get_String_Format(dhsc)<<m_strUnitMM<<endl
        <<_T("      fu    = ")<<Get_String_Format(dfu_sc)<<m_strUnitStress<<endl
        <<_T("      d     = ")<<Get_String_Format(dDia_sc)<<m_strUnitMM<<endl
        <<_T("      Space = ")<<Get_String_Format(dSpace)<<m_strUnitMM<<endl
        <<endl;

    fout<<_T("      P,Rd1  = (0.8*fu*PI*d^2/4)/gamma,V            = ")<<Get_String_Format(PglsBase.dP_Rd1)<<m_strUnitForce<<endl;

    if(3.0<=dRat_sc && dRat_sc<=4.0)
    {
        //dAlpha = 0.2*(dRat_sc+1.0);
        fout<<_T("      3 < h,sc/d < 4")<<endl
            <<_T("      Alpha = 0.2*(h,sc/d+1) = ")<<Get_String_Format(PglsBase.dAlpha)<<endl;
    }
    else if(dRat_sc>4.0)
    {
        //dAlpha = 1.0;
        fout<<_T("      h,sc/d > 4")<<endl
            <<_T("      Alpha = ")<<Get_String_Format(PglsBase.dAlpha)<<endl;
    }
    else
    {
        fout<<_T("      3 > h,sc/d")<<endl
            <<_T("      Alpha = 0.2*(h,sc/d+1) = ")<<Get_String_Format(PglsBase.dAlpha)<<endl;   
        /*
        dAlpha = 0.2*(dRat_sc+1.0);
        if(dAlpha>1.0)
        dAlpha = 1.0;
        */
    }
    //double dP_Rd = dgam_V==0.0 ? 0.0 : 0.29*dAlpha*pow(dDia_sc,2)*sqrt(dfck*dEcm)/dgam_V;

    fout<<_T("      P,Rd2  = 0.29*Alpha*d^2*Root(fck*Ecm)/gamma,V = ")<<Get_String_Format(PglsBase.dP_Rd2)<<m_strUnitForce<<endl;
    fout<<_T("      P,Rd   = min(P,Rd1 , P,Rd2) = ")<<Get_String_Format(PglsBase.dP_Rd)<<m_strUnitForce<<endl;

    /*
    double dAz_Slab  = 0.0;
    double dAz_Hunch = 0.0;
    double dErat = dEcm == 0.0 ? 0.0 : dEs/dEcm;   // Es/Ec, Section에 있는 값.
    double dZbar = 0.0; // 철근 고려 안한. Original Section Stiffeness.  
    double dIyy_Org = 0.0;

    if(!bTapered)
    {
    dZbar = SectD.SectAfter.Stiffness.Czm;
    dIyy_Org = SectD.SectAfter.Stiffness.Ryy;
    }
    else
    {
    dZbar = bPosiI ? SectD.SectAfter.SectI.Stiffness.Czm : SectD.SectAfter.SectJ.Stiffness.Czm;
    dIyy_Org = bPosiI ? SectD.SectAfter.SectI.Stiffness.Ryy : SectD.SectAfter.SectJ.Stiffness.Ryy;
    }

    dAz_Slab  = dErat==0.0 ? 0.0 : (dBc*dtc/dErat)*dtc*((dHt-0.5*dtc)-dZbar);
    dAz_Hunch = dErat==0.0 ? 0.0 : (dB1*dHh/dErat)*((dHt-dtc-0.5*dHh)-dZbar);
    double dAzI = dIyy_Org==0.0 ? 0.0 : (dAz_Slab+dAz_Hunch)/dIyy_Org;
    */

    //
    //double dv_L_Ed = fabs(dV_Ed)*dAzI; // Shear flow
    //double dv_L_Rd = dSpace==0.0 ? 0.0 : dP_Rd*((double)iNum)/dSpace;
    //

    fout<<endl
        <<_T("    - Calc. v,L,Ed , v,L,Rd , V,L,Ed")<<endl
        <<_T("      A*z/I  = ")<<Get_String_Format(PglsBase.dAzI)<<_T(" mm^-1")<<endl
        <<_T("      v,L,Ed = V,Ed*(A*z/I) = ")<<Get_String_Format(PglsBase.dv_L_Ed)<<m_strUnitForce2<<endl
        <<_T("      v,L,Rd = P,Rd*Num/Space = ")<<Get_String_Format(PglsBase.dv_L_Rd)<<m_strUnitForce2<<endl
        <<endl;

    double dV_L_Ed = 0.0;  
    if(PglsBase.dMy>=0.0)
    {
        //dV_L_Ed = dMpl_Rd-dMel_Rd==0.0 ? 0.0 : (dNc_f-dNc_el)*(fabs(dM_Ed)-dMel_Rd)/(dMpl_Rd-dMel_Rd);
        fout<<_T("      M,Ed > 0 ")<<endl
            <<_T("      V,L,Ed = (Nc,f-Nc,el)*(M,Ed-Mel,Rd)/(Mpl,Rd-Mel,Rd) = ")<<Get_String_Format(PglsBase.dV_L_Ed)<<m_strUnitForce<<endl;
    }
    else
    {
        //dV_L_Ed = 0.0;
        fout<<_T("      M,Ed < 0 ")<<endl
            <<_T("      V,L,Ed = ")<<Get_String_Format(PglsBase.dV_L_Ed)<<m_strUnitForce<<endl;    
    }

    /*
    double de_d = 0.0; // e_d = 0, b_eff = Bc defined in Section Data, 07.06.** 답변메일 내용으로 반영 
    double dLength_v = de_d + dBc; // EN 1994-2, 6.6.2.3, Figure 6.12, 49p~51p 참고.
    double dv_L_Ed2 = dLength_v==0.0 ? 0.0 : dV_L_Ed / dLength_v;
    //  
    double dv_Ed=dv_L_Ed/(2*dtc);
    double dnu = 0.6*(1.0-dfck/250.0);
    double dTheta_f = m_dPi/180.0*45.0; // 45로 가정
    double dv_Ed_lim =  dnu*dfcd*sin(dTheta_f)*cos(dTheta_f);// Designers' guide to EN 1994-2, 128p
    */
    fout<<_T("      v,Ed = v,L,Ed/(2*tc) = ")<<Get_String_Format(PglsBase.dv_Ed)<<m_strUnitForce2<<endl
        <<endl;

    return TRUE;
}

BOOL CDgnPlateGirder_IRC::Detail_FatigueResistance(ElemPairK ElemK, BOOL bPosiI)
{
    rptwofstream fout(m_strTestDgnFile, ios::app);

    T_PGRF_D PgrfD;
    if(!m_pAnalysisResult->GetPgrfResultData(ElemK, PgrfD)) return FALSE;

    T_PGRF_BASE PgrfBase;
    PgrfBase= bPosiI ? PgrfD.PgrfBase[0] : PgrfD.PgrfBase[1];

    BOOL bSect=TRUE, bMatl=TRUE, bScon=TRUE, bFrdt=TRUE;
    CPG_SECT_ELEM SectElem;  
    if(!Get_CpgSectElem(ElemK, SectElem)) bSect=FALSE;

    CPG_MATL_ELEM MatlElem;  
    if(!Get_CpgMatlElem(ElemK, MatlElem)) bMatl=FALSE;

    CPG_SCON_ELEM SconElem;
    SconElem.Initialize();
    if(!Get_CpgSconElem(ElemK, SconElem)) bScon=FALSE;
    // Shear Connector.
    CPG_SCON_POSI SconPosi = bPosiI ? SconElem.SconPosi[0] : SconElem.SconPosi[1];

    CPG_FRDT_ELEM FrdtElem;
    FrdtElem.Initialize();
    if(!Get_CpgFrdtElem(ElemK, FrdtElem)) bFrdt=FALSE;
    // Lamda_v2, Lamda_v4
    CPG_FRDT_POSI FrdtPosi;
    FrdtPosi = bPosiI ? FrdtElem.FrdtPosi[0] : FrdtElem.FrdtPosi[1];

    // 1. Section.  
    T_SECT_D& SectD = SectElem.SectD;
    BOOL bTapered = m_pDgnSectUtil->Is_TaperedSect(&SectD);
    double dH		= m_pDgnSectUtil->Get_Size_H(&SectD, bPosiI);
    double dTw	= m_pDgnSectUtil->Get_Size_tw(&SectD, CDgnSectUtil::EN_WEB_THK_LEFT, bPosiI);
    double dB1	= m_pDgnSectUtil->Get_Size_bft(&SectD, bPosiI);
    double dTf1	= m_pDgnSectUtil->Get_Size_tft(&SectD, bPosiI);
    double dB2	= m_pDgnSectUtil->Get_Size_bfb(&SectD, bPosiI);
    double dTf2	= m_pDgnSectUtil->Get_Size_tfb(&SectD, bPosiI);
    double dh		= m_pDgnSectUtil->Get_Size_hw(&SectD, bPosiI);
    double dBc	= m_pDgnSectUtil->Get_Size_Bc(&SectD);
    double dtc	= m_pDgnSectUtil->Get_Size_tc(&SectD);
    double dHh	= m_pDgnSectUtil->Get_Size_Hh(&SectD);
    double dHt	= m_pDgnSectUtil->Get_Size_Ht(&SectD, bPosiI);

    // 2. Material
    T_MATD_D& MatdD = MatlElem.MatdD;
    int iMatClass = MatlElem.iMatClass;
    double dfy  = MatdD.Data1.Design.S_Fy1;
    double dfy1 = MatdD.Data1.Design.S_Fy2;
    double dfu  = MatdD.Data1.Design.S_Fu;
    double dEs  = MatdD.Data1.Analysis.Elast;
    double dPoissonS = MatdD.Data1.Analysis.Poisson;
    double dGs  = dEs/(1.0+dPoissonS); 
    double dfck  = MatlElem.dfck;
    double dfcm  = MatlElem.dfcm;
    double dfctm = MatlElem.dfctm;
    double dEcm  = MatlElem.dEcm;
    double dfsk = MatlElem.MatdD.MainRebarData.B_fy;
    double dErs = dEs; 
    double dgam_M0 = m_dGamma_M0;
    double dgam_M1 = m_dGamma_M1;
    double dgam_sr = m_dGamma_S;
    double dgam_c  = m_dGamma_C;  
    double dfcd =  0.67* dfck/dgam_c;
    double dfsd = fabs(dgam_sr) < m_dZero ? dfsk : dfsk/dgam_sr;
    double dfyt = MatdD.Data1.DesignTopFlange.S_Fy1;//dTF1 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyb = MatdD.Data1.DesignBotFlange.S_Fy1; //dTF2 < 40.0 + m_dZero ? dfy : dfy1;
    double dfyw = MatdD.Data1.DesignWeb.S_Fy1 ; //dTw < 40.0 ? dfy : dfy1;;
    double dfyd_t = fabs(dgam_M0) < m_dZero ? dfyt : dfyt/dgam_M0;
    double dfyd_b = fabs(dgam_M0) < m_dZero ? dfyb : dfyb/dgam_M0;
    double dfyd_w = fabs(dgam_M0) < m_dZero ? dfyw : dfyw/dgam_M0;

    // Load combination
    LPCTSTR aType[] = {_T("-"), _LS(IDS_TB_PSCD_FX_MAX), _LS(IDS_TB_PSCD_FX_MIN), _LS(IDS_TB_PSCD_FY_MAX), _LS(IDS_TB_PSCD_FY_MIN), 
        _LS(IDS_TB_PSCD_FZ_MAX), _LS(IDS_TB_PSCD_FZ_MIN), _LS(IDS_TB_PSCD_MX_MAX), _LS(IDS_TB_PSCD_MX_MIN),
        _LS(IDS_TB_PSCD_MY_MAX), _LS(IDS_TB_PSCD_MY_MIN), _LS(IDS_TB_PSCD_MZ_MAX), _LS(IDS_TB_PSCD_MZ_MIN)};

    // Lcom
    CString strLoadComb;    
    T_LCOM_D LcomData;      
    if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, PgrfBase.OrgLcomK, LcomData)) 
        strLoadComb.Format(_T("%s(%s)"),LcomData.LoadCombName, aType[PgrfBase.iMax]);
    else 
        strLoadComb = _T("-");

    ///////////////////////////////////////////////////

    //
    T_PGLS_D PglsD;
    PglsD.Initialize();
    int iDgnLcomNo = PgrfBase.LcomK;
    int iMaxMin=0;
    if(!m_pAnalysisResult->GetCpgLongitudinalShearResistanceResult(ElemK,iDgnLcomNo,iMaxMin,PglsD)) 
    {
    }

    T_PGLS_BASE PglsBase;
    PglsBase = bPosiI ? PglsD.PglsBase[0] : PglsD.PglsBase[1];

    //
    fout<<endl
        <<_T("--------------------------------------------------------------------------------------")<<endl
        <<endl
        <<_T("  *. Resistance to Fatigue")<<endl
        <<endl
        <<_T("    - Load Combination     = ")<<strLoadComb<<endl   
        <<_T("      My   = ")<<Get_String_Format(PgrfBase.dMy)<<m_strUnitMoment<<endl
        <<endl;

    /*
    // EN 1994-2, 6.6 Shear connection, 46page
    int iNum = SconPosi.iNum;
    double dhsc    = SconPosi.dhsc;
    double dfu_sc  = SconPosi.dfu;
    double dDia_sc = SconPosi.dDia;
    double dSt_sc  = SconPosi.dSt;
    double dSpace  = SconPosi.dSpace;
    double dAlpha = 0.0;

    double dv_L_Ed = PglsBase.dV_L_Ed;
    double dDel_Tau=0.0;
    double dLamda_v=0.0;
    double dLamda_v1=0.0;
    double dLamda_v2=FrdtPosi.dLamda_v2; // 입력 받는것으로.
    double dLamda_v3=0.0;
    double dLamda_v4=FrdtPosi.dLamda_v4;
    double dDel_Tau_E_2=0.0;
    double dDel_Tau_c=0.0;
    double dRatio=0.0;

    // The range of shear force per connector
    double dFsc = iNum==0.0 ? 0.0 : dv_L_Ed*dSpace/((double)iNum);
    double dA_sc = m_dPi*pow(dDia_sc,2)/4.0;
    dDel_Tau = dA_sc==0.0 ? 0.0 : dFsc/dA_sc;
    // EN 1994-2, 6.8.6.2 Shear connection, 76page
    // EN 1993-2, 9.5.2 Damage equivalence factors λ for road bridges, 42page
    dLamda_v1 = 1.55; // // 6.8.6.2(4), 76page
    double dQ0 = 480.0; // 480 kN
    double dN0 = 0.5*10E6;
    double dN_Obs = 0.5*10E6; // The UK's National Annex to EN 1991-2 may modify this value
    double dSigma_niQi5 = 8.051*10E18;
    double dSigma_ni = 1.0*10E6;
    double dQm1 = pow(dSigma_niQi5/dSigma_ni, 1./5.); //kN  
    //dLamda_v2 = dQ0==0.0 || dN0==0.0 ? 0.0 : dQm1/dQ0*pow((dN_Obs/dN0),1./5.);
    //    
    dLamda_v3 = pow(dt_Ld/100.0, 1./5.);
    //
    //dLamda_v4 = 1.0; // ?? EN 1993-2, 9.5.2(6), 44page
    //
    dLamda_v = dLamda_v1*dLamda_v2*dLamda_v3*dLamda_v4;
    // 
    dDel_Tau_E_2 = dLamda_v*dDel_Tau;
    // EN 1994-2, 6.8.3(3), 72page.
    dDel_Tau_c = 90.0; // is the reference value at Nc=2*10^6 cycles with Δτ_c equal to 90 N/mm^2
    //
    dRatio = dgam_Mfs==0.0 || dDel_Tau_c==0.0 ? dgam_Ff*dDel_Tau_E_2/m_dZero : dgam_Ff*dDel_Tau_E_2/(dDel_Tau_c/dgam_Mfs);
    dRatio = fabs(dRatio);
    if(dRatio<=1.0)
    PgrfBase.bOK = TRUE;
    else 
    PgrfBase.bOK = FALSE;
    */
    //
    fout<<_T("    - Calc. Delta,tau,E,2")<<endl
        <<_T("      F,sc         = ")<<Get_String_Format(PgrfBase.dFsc)<<m_strUnitForce<<endl
        <<_T("      A,sc         = ")<<Get_String_Format(PgrfBase.dAsc)<<m_strUnitMM2<<endl
        <<_T("      Delta,tau    = F,sc/A,sc = ")<<Get_String_Format(PgrfBase.dDel_Tau)<<m_strUnitStress<<endl
        <<_T("      Lamda,v1     = ")<<Get_String_Format(PgrfBase.dLamda_v1)<<endl
        <<_T("      Lamda,v2     = ")<<Get_String_Format(PgrfBase.dLamda_v2)<<endl
        <<_T("      Lamda,v3     = ")<<Get_String_Format(PgrfBase.dLamda_v3)<<endl
        <<_T("      Lamda,v4     = ")<<Get_String_Format(PgrfBase.dLamda_v4)<<endl
        <<_T("      Lamda,v      = Lamda,v1*Lamda,v2*Lamda,v3*Lamda,v4 = ")<<Get_String_Format(PgrfBase.dLamda_v)<<endl
        <<_T("      Delta,tau,E2 = Lamda,v*Delta,tau = ")<<Get_String_Format(PgrfBase.dDel_Tau_E_2)<<m_strUnitStress<<endl
        <<_T("      Delta,tau,c  = ")<<Get_String_Format(PgrfBase.dDel_Tau_c)<<m_strUnitStress<<endl
        <<endl;

    fout<<_T("      (gamma,Ff*Delta,tau,E2)/(Delta,tau,c/gamma,Mf,s) = ")<<Get_String_Format(PgrfBase.dRatio)<<endl;

    return TRUE;
}

CString CDgnPlateGirder_IRC::Get_String_Format(double dValue, int iNum)
{
    CString strFormat;

    switch(iNum)
    {
    case 16:     {strFormat.Format(_T("%16g"), dValue);      break;}
    case 17:     {strFormat.Format(_T("%17g"), dValue);      break;}
    case 18:     {strFormat.Format(_T("%18g"), dValue);      break;}
    default:     {strFormat.Format(_T("%16g"), dValue);      break;}
    }

    return strFormat;
}

void CDgnPlateGirder_IRC::Print_EffectivePolyAndLineData(T_GSEC_POLYGON& PolyD, CArray<T_GSEC_LINE, T_GSEC_LINE&>& aEffLine)
{
    int nLineSize = aEffLine.GetSize();
    for(int i=0; i<nLineSize; i++)
    {
        T_GSEC_LINE EffLine = aEffLine[i];
        int v1 = EffLine.v1;
        int v2 = EffLine.v2;
        double dx1 = PolyD.aVertex[v1].dX;
        double dy1 = PolyD.aVertex[v1].dY;
        double dx2 = PolyD.aVertex[v2].dX;
        double dy2 = PolyD.aVertex[v2].dY;
        TRACE(_T("%lf  %lf\n"), dx1, dy1);
        TRACE(_T("%lf  %lf\n"), dx2, dy2);
    }
}

int CDgnPlateGirder_IRC::Get_SubPannelCalcType(int nLongStfnSize)
{
    // 0=종방향 보강재 1 또는 2개 포함하는 패널, 1 = 없거나 3개이상 
    int nSubCalcType=0;
    if(nLongStfnSize==1 || nLongStfnSize==2) {nSubCalcType=0;}
    else                                     {nSubCalcType=1;}

    return nSubCalcType;
}

void CDgnPlateGirder_IRC::Calc_r1r2(double& dr1, double& dr2)
{
    //PROFILING(_T("CDgnPlateGirder_IRC::Calc_r1r2"));
    // IS:800-2007 Table 2. p.18
    // Reference ; BS 5950-1:2000 Clause 3.5.5, p.34
    double dpyf = m_dfy;
    double dpyw = m_dfy ;


    if(m_iType == 2) // H(I).
    {
        double dd = 0.0;
        if(fabs(m_dB1-m_dB2) < m_dZero || fabs(m_dtf1-m_dtf2) < m_dZero) // a) for I- or H-sections with equal flenges
        {
            dd = m_dtf2 < m_dZero ? m_dH-2*m_dtf1 : m_dH-m_dtf1-m_dtf2;

            dr1 = -m_dPu / (dd * m_dHw * dpyw);
            if(dr1 < -1.0)      dr1 = -1.0; // -1<r1<1
            else if(dr1 > 1.0)  dr1 = 1.0;

            dr2 = -m_dPu / (m_dArea * dpyw);
            //Print_IS800_07_014_7(dr1, dr2);
        }
        else  // b) for I- ro H-section with unequal flenges
        {
            double dtf2=0.0, df1 = 0.0, df2 = 0.0, dAf;
            Calc_StressWeb_H(df1, df2);  // get web stress

            dd   = m_dtf2 < m_dZero ? m_dH-2*m_dtf1 : m_dH-m_dtf1-m_dtf2; // depth of web
            dtf2 = m_dtf2 < m_dZero ? m_dtf1 : m_dtf2;
            dAf  = df1 > df2 ? (dtf2*m_dB2-m_dtf1*m_dB1) : (m_dtf1*m_dB1-dtf2*m_dB2); // (BtTt-BcTc)

            dr1 = (-m_dPu + dAf * dpyf)/ (dd * m_dHw * dpyw);
            if(dr1 < -1.0)      dr1 = -1.0; // -1<r1<1
            else if(dr1 > 1.0)  dr1 = 1.0;

            dr2 = (df1 + df2) / (m_dArea * dpyw);
            //Print_IS800_07_014_8(df1, df2, dr1, dr2);;
        }
    }
    else if(m_iType == 4 || m_iType == 7) // c) for RHS or welded box sections with equal flanges
    {
        double dd = 0.0;
        dd = m_dtf2 < m_dZero ? m_dH-2*m_dtf1 : m_dH-m_dtf1-m_dtf2;

        dr1 = -m_dPu / (2.0 * dd * m_dHw * dpyw);
        if(dr1 < -1.0)      dr1 = -1.0; // -1<r1<1
        else if(dr1 > 1.0)  dr1 = 1.0;

        dr2 = -m_dPu / (m_dArea * dpyw);  
        //Print_IS800_07_014_9(dr1, dr2);
    }
    else if (m_iType == 45)
    {
        double dd = 0.0;
        if(fabs(m_dB1-m_dB2) < m_dZero || fabs(m_dtf1-m_dtf2) < m_dZero) // a) for I- or H-sections with equal flenges
        {
            dd = m_dtf2 < m_dZero ? m_dH-2*m_dtf1 : m_dH-m_dtf1-m_dtf2;

            dr1 = -m_dPu / (dd * m_dHw * dpyw);
            if(dr1 < -1.0)      dr1 = -1.0; // -1<r1<1
            else if(dr1 > 1.0)  dr1 = 1.0;

            dr2 = -m_dPu / (m_dArea * dpyw);
            //Print_IS800_07_014_7(dr1, dr2);
        }
        else  // b) for I- ro H-section with unequal flenges
        {
            ASSERT(0);//Not Possible
            // 		  double dtf2=0.0, df1 = 0.0, df2 = 0.0, dAf;
            // 		  Calc_StressWeb_H(df1, df2);  // get web stress
            // 
            // 		  dd   = m_dtf2 < m_dZero ? m_dH-2*m_dtf1 : m_dH-m_dtf1-m_dtf2-2.0*m_dFillet; // depth of web
            // 		  dtf2 = m_dtf2 < m_dZero ? m_dtf1 : m_dtf2;
            // 		  dAf  = df1 > df2 ? (dtf2*m_dB2-m_dtf1*m_dB1) : (m_dtf1*m_dB1-dtf2*m_dB2); // (BtTt-BcTc)
            // 
            // 		  dr1 = (-m_dPu + dAf * dpyf)/ (dd * m_dHw * dpyw);
            // 		  if(dr1 < -1.0)      dr1 = -1.0; // -1<r1<1
            // 		  else if(dr1 > 1.0)  dr1 = 1.0;
            // 
            // 		  dr2 = (df1 + df2) / (m_dArea * dpyw);
            // 		  Print_IS800_07_014_8(df1, df2, dr1, dr2);;
        }
    }
    else ASSERT(0);
}

// calculate Wbe stress - These souces got from Eurocode3. Get_StressWeb Line.1781
void CDgnPlateGirder_IRC::Calc_StressWeb_H(double& dStrTop, double& dStrBot)
{
    //PROFILING(_T("CDgnPlateGirder_IRC::Calc_StressWeb_H"));
    // Remember Tension stress(-), Compression stress(+).
    dStrTop = 0.0, dStrBot = 0.0;
    double dStrTop01 = 0.0, dStrBot01 = 0.0;
    double dStrTop02 = 0.0, dStrBot02 = 0.0;

    // Compression(+), Tension(-).
    double dMinH1=0.0, dMinH2=0.0, dMinB1=0.0, dMinB2=0.0;
    double dMaxH1=0.0, dMaxH2=0.0, dMaxB1=0.0, dMaxB2=0.0;

    //if(m_iType!=2 || m_iType!=45)  ASSERT(0);

    dMinH1 = min(m_dH-m_dZbar, m_dZbar);
    dMinB1 = min(m_dYbar, m_dB1-m_dYbar);
    dMaxH1 = max(m_dH-m_dZbar, m_dZbar);
    dMaxB1 = max(m_dYbar, m_dB1-m_dYbar);

    dMinH2 = min(m_dH-m_dZbar-m_dtf1, m_dZbar-m_dtf1);
    dMinB2 = min(m_dYbar, m_dB1-m_dYbar);
    dMaxH2 = max(m_dH-m_dZbar-m_dtf1, m_dZbar-m_dtf1);
    dMaxB2 = max(m_dYbar, m_dB1-m_dYbar);

    dStrTop01 = (-1.)*m_dPu/m_dArea +       m_dMuy*dMinH1/m_dRyy + (-1.)*m_dMuz*(m_dHw/2.)/m_dRzz;
    dStrBot01 = (-1.)*m_dPu/m_dArea + (-1.)*m_dMuy*dMaxH1/m_dRyy + (-1.)*m_dMuz*(m_dHw/2.)/m_dRzz;
    dStrTop02 = (-1.)*m_dPu/m_dArea +       m_dMuy*dMinH1/m_dRyy +       m_dMuz*(m_dHw/2.)/m_dRzz;
    dStrBot02 = (-1.)*m_dPu/m_dArea + (-1.)*m_dMuy*dMaxH1/m_dRyy +       m_dMuz*(m_dHw/2.)/m_dRzz;

    if(max(dStrTop01,dStrBot01) > max(dStrTop02,dStrBot02))
    {
        dStrTop	= dStrTop01;
        dStrBot	= dStrBot01;
    }
    else
    {
        dStrTop	= dStrTop02;
        dStrBot	= dStrBot02;
    }
}

double CDgnPlateGirder_IRC::Calc_HTR()  // for web
{
    // Get HTR
    double dHTR=0.0;

    if(m_iType==0)			dHTR = (m_dH-m_dtf1)/m_dHw;				  	// L.
    else if(m_iType==1)	dHTR = (m_dH-m_dtf1-m_dtf2)/m_dHw;		// C.
    else if(m_iType==2)	dHTR = (m_dH-m_dtf1-m_dtf2)/m_dHw;		// H.
    else if(m_iType==3)	dHTR = (m_dH-m_dtf1)/m_dHw;				  	// T.
    else if(m_iType==4)	dHTR = (m_dH-m_dtf1-m_dtf2)/m_dHw;		// B.
    else if(m_iType==5)	dHTR = m_dH/m_dB1;										// P.
    else if(m_iType==6)	dHTR = 1.0;												  	// SR.
    else if(m_iType==7)	dHTR = m_dH/m_dB1;										// SB.
    else if(m_iType==8)	dHTR = (m_dH-m_dtf1)/(2.*m_dHw);			// 2L.
    else if(m_iType==9)	dHTR = (m_dH-2.*m_dtf1)/(2.*m_dHw);	  // 2C.
    else if(m_iType==45)dHTR = (m_dH-m_dtf1-m_dtf2)/m_dHw;		// HC.
    else ASSERT(0);

    return dHTR;
}

double CDgnPlateGirder_IRC::Calc_BTR()  // for flenge
{
    // Get BTR
    double dBTR=0.0;
    // Same Get_BTR() at CACSCode_EuroCode3.
    if(m_iType==0)			dBTR = m_dB1/m_dtf1;													      		// L.
    else if(m_iType==1)	dBTR = max(m_dB1/m_dtf1, m_dB2/m_dtf2);					      	// C.
    else if(m_iType==2)	dBTR = max(m_dB1/(2.*m_dtf1), m_dB2/(2.*m_dtf2));	      // H.
    else if(m_iType==3)	dBTR = m_dB1/(2.*m_dtf1);												      	// T.
    else if(m_iType==4)	dBTR = max((m_dB2-m_dHw)/m_dtf1, (m_dB2-m_dHw)/m_dtf2);	// B.
    else if(m_iType==5)	dBTR = m_dH/m_dB1;		                                  // P.
    else if(m_iType==6)	dBTR = 1.0;				                                  		// SR.
    else if(m_iType==7)	dBTR = m_dH/m_dB1;	                                  	// SB.
    else if(m_iType==8)	dBTR = m_dB1/m_dtf1;	                                  // 2L.
    else if(m_iType==9)	dBTR = m_dB1/m_dtf1;	                                  // 2C.
    else if(m_iType==45)	dBTR = max(m_dB1/(2.*m_dtf1), m_dB2/(2.*m_dtf2));	          // HC.
    else ASSERT(0);

    return dBTR;
}

double CDgnPlateGirder_IRC::Calc_Class_General()
{
    double dBeta2f,  dBeta2w,  dBeta3f,  dBeta3w;
    //PROFILING(_T("CDgnPlateGirder_IRC::Calc_Class_General"));
    // C, H, B, SB, 2C
    // IS:800-2007 Table 2. p.18
    // Plastic(1), Compact(2), Semi-compact(3), Slender(4).  
    int iClassF=0, iClassW=0;

    double dHTR = Calc_HTR();
    double dBTR = Calc_BTR();
    double Eps = Get_Epsilon(m_dfy);

    BOOL bPureBend = (fabs(m_dPu) < m_dZero ? TRUE : FALSE);
    BOOL bPureComp = (max(fabs(m_dMuy),fabs(m_dMuz)) < m_dZero && m_dPu < 0.0 ? TRUE : FALSE);
    //double m_iBuiltUp = 2.0;
    // flange
    // Outstand element of compression flange
    if(bPureComp) // pure compression
    {
        if (m_iType!=45)
        {
            if(dBTR <= 42.*Eps)	  	iClassF = 3;
            else							      iClassF = 4;

            dBeta2f = 42.*Eps; dBeta3f = 42.*Eps; 
            //Print_IS800_07_015_2(iClassF, dBTR, Eps, m_dfyf);
        }
        else
        {
            //For compound flange 3 width to thickness ratios need to be considered and worst of them to be used for design
            double iClassF_1=0.0;
            double BTR1=Calc_BTR_HC1();
            int iFlClass=0;//0-BTR1 Highest,1-BTR2 Highest,2-BTR3 Highest
            if(BTR1 <= 9.4*Eps)		  	iClassF_1 = 1;
            else if(BTR1 <= 10.5*Eps) iClassF_1 = 2;
            else if(BTR1 <= 15.7*Eps)	iClassF_1 = 3;
            else							        iClassF_1 = 4;

            double  dBeta2f_1 = 10.5*Eps; 
            double dBeta3f_1  = 15.7*Eps; 

            //------------------
            double iClassF_2=0.0;
            double BTR2=Calc_BTR_HC2();
            if(BTR2 <= 29.3*Eps)	  	iClassF_2 = 1;
            else if(BTR2 <= 33.5*Eps) iClassF_2 = 2;
            else if(BTR2 <= 42.0*Eps)	iClassF_2 = 3;
            else							        iClassF_2 = 4;

            double dBeta2f_2 = 33.5*Eps;
            double dBeta3f_2 = 42.0*Eps; 

            //Get Worst of Both
            dBTR=max(BTR1,BTR2);
            if (dBTR==BTR1)
            {
                iClassF=iClassF_1;
                dBeta2f=dBeta2f_1;
                dBeta3f=dBeta3f_1;
                iFlClass=0;
            }
            else
            {
                iClassF=iClassF_2;
                dBeta2f=dBeta2f_2;
                dBeta3f=dBeta3f_2;
                iFlClass=1;
            }


            //-----------------------------
            double iClassF_3=0.0;
            double BTR3=Calc_BTR_HC3();
            if(BTR3 <= 8.4*Eps)		  	iClassF_3 = 1;
            else if(BTR3 <= 9.4*Eps)  iClassF_3 = 2;
            else if(BTR3 <= 13.6*Eps)	iClassF_3 = 3;
            else							        iClassF_3 = 4;

            double dBeta2f_3 = 9.4*Eps;
            double dBeta3f_3 = 13.6*Eps;

            //Get Worst of All
            dBTR=max(dBTR,BTR3);
            if (dBTR==BTR3)
            {
                iClassF=iClassF_3;
                dBeta2f=dBeta2f_3;
                dBeta3f=dBeta3f_3;
                iFlClass=2;
            }



        }

    }
    else
    {
        if(m_iType==4) // if B. (Internal element)
        {
            if(dBTR <= 29.3*Eps)	  	iClassF = 1;
            else if(dBTR <= 33.5*Eps) iClassF = 2;
            else if(dBTR <= 42.0*Eps)	iClassF = 3;
            else							        iClassF = 4;

            dBeta2f = 33.5*Eps; dBeta3f = 42.0*Eps;     

        }
        else  // if C, H, SB, 2C  (Outstand element)
        {
            if(m_bIsRolledSection)	// Rolled.
            {
                if(dBTR <= 9.4*Eps)		  	iClassF = 1;
                else if(dBTR <= 10.5*Eps) iClassF = 2;
                else if(dBTR <= 15.7*Eps)	iClassF = 3;
                else							        iClassF = 4;

                dBeta2f = 10.5*Eps; dBeta3f = 15.7*Eps; 

            }
            else              // Built-up
            {
                if (m_iType==45)
                {
                    //For compound flange 3 width to thickness ratios need to be considered and worst of them to be used for design
                    double iClassF_1=0.0;
                    double BTR1=Calc_BTR_HC1();
                    int iFlClass=0;//0-BTR1 Highest,1-BTR2 Highest,2-BTR3 Highest
                    if(BTR1 <= 9.4*Eps)		  	iClassF_1 = 1;
                    else if(BTR1 <= 10.5*Eps) iClassF_1 = 2;
                    else if(BTR1 <= 15.7*Eps)	iClassF_1 = 3;
                    else							        iClassF_1 = 4;

                    double  dBeta2f_1 = 10.5*Eps; 
                    double dBeta3f_1  = 15.7*Eps; 

                    //------------------
                    double iClassF_2=0.0;
                    double BTR2=Calc_BTR_HC2();
                    if(BTR2 <= 29.3*Eps)	  	iClassF_2 = 1;
                    else if(BTR2 <= 33.5*Eps) iClassF_2 = 2;
                    else if(BTR2 <= 42.0*Eps)	iClassF_2 = 3;
                    else							        iClassF_2 = 4;

                    double dBeta2f_2 = 33.5*Eps;
                    double dBeta3f_2 = 42.0*Eps; 

                    //Get Worst of Both
                    dBTR=max(BTR1,BTR2);
                    if (dBTR==BTR1)
                    {
                        iClassF=iClassF_1;
                        dBeta2f=dBeta2f_1;
                        dBeta3f=dBeta3f_1;
                        iFlClass=0;
                    }
                    else
                    {
                        iClassF=iClassF_2;
                        dBeta2f=dBeta2f_2;
                        dBeta3f=dBeta3f_2;
                        iFlClass=1;
                    }

                    //-----------------------------
                    double iClassF_3=0.0;
                    double BTR3=Calc_BTR_HC3();
                    if(BTR3 <= 8.4*Eps)		  	iClassF_3 = 1;
                    else if(BTR3 <= 9.4*Eps)  iClassF_3 = 2;
                    else if(BTR3 <= 13.6*Eps)	iClassF_3 = 3;
                    else							        iClassF_3 = 4;

                    double dBeta2f_3 = 33.5*Eps;
                    double dBeta3f_3 = 42.0*Eps;

                    //Get Worst of All
                    dBTR=max(dBTR,BTR3);
                    if (dBTR==BTR3)
                    {
                        iClassF=iClassF_3;
                        dBeta2f=dBeta2f_3;
                        dBeta3f=dBeta3f_3;
                        iFlClass=2;
                    }


                }
                else
                {
                    if(dBTR <= 8.4*Eps)		  	iClassF = 1;
                    else if(dBTR <= 9.4*Eps)  iClassF = 2;
                    else if(dBTR <= 13.6*Eps)	iClassF = 3;
                    else							        iClassF = 4;
                    dBeta2f = 9.4*Eps; dBeta3f = 13.6*Eps; 

                }


            }
        }
    }

    // web
    // Web of a channel
    if(m_iType==1 || m_iType==9)  // C, 2C
    {
        if(dHTR <= 42.*Eps)	  	iClassW = 3;
        else							      iClassW = 4; 

    }
    else  // Web of an I-, H- or box section
    {
        double dr1 = 0.0, dr2 = 0.0;

        if(m_iType==4)  Eps = Get_Epsilon(m_dfy);

        if(bPureBend && !bPureComp) // pure bending
        {
            // Neutral axis at mid-depth
            if(dHTR <= 84.*Eps)		  	iClassW = 1;
            else if(dHTR <= 105.*Eps) iClassW = 2;
            else if(dHTR <= 126.*Eps)	iClassW = 3;
            else							        iClassW = 4; 

            dBeta2w = 105.*Eps; dBeta3w = 126.*Eps; 

        }
        else if(!bPureBend && bPureComp) // pure compression
        {

            Calc_r1r2(dr1, dr2);
            // Axial compression
            if(dHTR <= 42.*Eps) iClassW = 3;
            else                iClassW = 4;  

            dBeta2w = 42.*Eps;
            dBeta3w = 42.*Eps;

        }
        else
        {

            Calc_r1r2(dr1, dr2);
            // Generally
            if(dr1 < 0.0) // if r1 is negative
            {
                if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)         iClassW = 1;
                else if(dHTR <= (105.*Eps/(1+dr1)) && dHTR <= 42.*Eps)   iClassW = 2;
                else if(dHTR <= (126.*Eps/(1+2*dr2)) && dHTR <= 42.*Eps) iClassW = 3;
                else                                                     iClassW = 4;

                dBeta2w = min(105.*Eps/(1+dr1), 42.*Eps);
                dBeta3w = min(126.*Eps/(1+2*dr2), 42.*Eps);

            }
            else  // if r1 is positive
            {
                if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)           iClassW = 1;
                else if(dHTR <= (105.*Eps/(1+1.5*dr1)) && dHTR <= 42.*Eps) iClassW = 2;
                else if(dHTR <= (126.*Eps/(1+2.0*dr2)) && dHTR <= 42.*Eps) iClassW = 3;
                else                                                     iClassW = 4;

                dBeta2w = min(105.*Eps/(1+dr1), 42.*Eps);
                dBeta3w = min(126.*Eps/(1+2*dr2), 42.*Eps);

            }    
        }  
    }
    double m_iClass  = max(iClassW, iClassF);
    return m_iClass;
}

double CDgnPlateGirder_IRC::Calc_Class_General(int &iClassF,int &iClassW)
{
    double dBeta2f,  dBeta2w,  dBeta3f,  dBeta3w;
    //PROFILING(_T("CDgnPlateGirder_IRC::Calc_Class_General"));
    // C, H, B, SB, 2C
    // IS:800-2007 Table 2. p.18
    double dHTR = Calc_HTR();
    double dBTR = Calc_BTR();
    double Eps = Get_Epsilon(m_dfy);

    BOOL bPureBend = (fabs(m_dPu) < m_dZero ? TRUE : FALSE);
    BOOL bPureComp = (max(fabs(m_dMuy),fabs(m_dMuz)) < m_dZero && m_dPu < 0.0 ? TRUE : FALSE);
    double m_iBuiltUp = 2.0;
    // flange
    // Outstand element of compression flange
    if(bPureComp) // pure compression
    {
        if (m_iType!=45)
        {
            if(dBTR <= 42.*Eps)	  	iClassF = 3;
            else							      iClassF = 4;

            dBeta2f = 42.*Eps; dBeta3f = 42.*Eps; 
            //Print_IS800_07_015_2(iClassF, dBTR, Eps, m_dfyf);
        }
        else
        {
            //For compound flange 3 width to thickness ratios need to be considered and worst of them to be used for design
            double iClassF_1=0.0;
            double BTR1=Calc_BTR_HC1();
            int iFlClass=0;//0-BTR1 Highest,1-BTR2 Highest,2-BTR3 Highest
            if(BTR1 <= 9.4*Eps)		  	iClassF_1 = 1;
            else if(BTR1 <= 10.5*Eps) iClassF_1 = 2;
            else if(BTR1 <= 15.7*Eps)	iClassF_1 = 3;
            else							        iClassF_1 = 4;

            double  dBeta2f_1 = 10.5*Eps; 
            double dBeta3f_1  = 15.7*Eps; 

            //------------------
            double iClassF_2=0.0;
            double BTR2=Calc_BTR_HC2();
            if(BTR2 <= 29.3*Eps)	  	iClassF_2 = 1;
            else if(BTR2 <= 33.5*Eps) iClassF_2 = 2;
            else if(BTR2 <= 42.0*Eps)	iClassF_2 = 3;
            else							        iClassF_2 = 4;

            double dBeta2f_2 = 33.5*Eps;
            double dBeta3f_2 = 42.0*Eps; 

            //Get Worst of Both
            dBTR=max(BTR1,BTR2);
            if (dBTR==BTR1)
            {
                iClassF=iClassF_1;
                dBeta2f=dBeta2f_1;
                dBeta3f=dBeta3f_1;
                iFlClass=0;
            }
            else
            {
                iClassF=iClassF_2;
                dBeta2f=dBeta2f_2;
                dBeta3f=dBeta3f_2;
                iFlClass=1;
            }


            //-----------------------------
            double iClassF_3=0.0;
            double BTR3=Calc_BTR_HC3();
            if(BTR3 <= 8.4*Eps)		  	iClassF_3 = 1;
            else if(BTR3 <= 9.4*Eps)  iClassF_3 = 2;
            else if(BTR3 <= 13.6*Eps)	iClassF_3 = 3;
            else							        iClassF_3 = 4;

            double dBeta2f_3 = 9.4*Eps;
            double dBeta3f_3 = 13.6*Eps;

            //Get Worst of All
            dBTR=max(dBTR,BTR3);
            if (dBTR==BTR3)
            {
                iClassF=iClassF_3;
                dBeta2f=dBeta2f_3;
                dBeta3f=dBeta3f_3;
                iFlClass=2;
            }



        }

    }
    else
    {
        if(m_iType==4) // if B. (Internal element)
        {
            if(dBTR <= 29.3*Eps)	  	iClassF = 1;
            else if(dBTR <= 33.5*Eps) iClassF = 2;
            else if(dBTR <= 42.0*Eps)	iClassF = 3;
            else							        iClassF = 4;

            dBeta2f = 33.5*Eps; dBeta3f = 42.0*Eps;     

        }
        else  // if C, H, SB, 2C  (Outstand element)
        {
            if(m_iBuiltUp==2)	// Rolled.
            {
                if(dBTR <= 9.4*Eps)		  	iClassF = 1;
                else if(dBTR <= 10.5*Eps) iClassF = 2;
                else if(dBTR <= 15.7*Eps)	iClassF = 3;
                else							        iClassF = 4;

                dBeta2f = 10.5*Eps; dBeta3f = 15.7*Eps; 

            }
            else              // Built-up
            {
                if (m_iType==45)
                {
                    //For compound flange 3 width to thickness ratios need to be considered and worst of them to be used for design
                    double iClassF_1=0.0;
                    double BTR1=Calc_BTR_HC1();
                    int iFlClass=0;//0-BTR1 Highest,1-BTR2 Highest,2-BTR3 Highest
                    if(BTR1 <= 9.4*Eps)		  	iClassF_1 = 1;
                    else if(BTR1 <= 10.5*Eps) iClassF_1 = 2;
                    else if(BTR1 <= 15.7*Eps)	iClassF_1 = 3;
                    else							        iClassF_1 = 4;

                    double  dBeta2f_1 = 10.5*Eps; 
                    double dBeta3f_1  = 15.7*Eps; 

                    //------------------
                    double iClassF_2=0.0;
                    double BTR2=Calc_BTR_HC2();
                    if(BTR2 <= 29.3*Eps)	  	iClassF_2 = 1;
                    else if(BTR2 <= 33.5*Eps) iClassF_2 = 2;
                    else if(BTR2 <= 42.0*Eps)	iClassF_2 = 3;
                    else							        iClassF_2 = 4;

                    double dBeta2f_2 = 33.5*Eps;
                    double dBeta3f_2 = 42.0*Eps; 

                    //Get Worst of Both
                    dBTR=max(BTR1,BTR2);
                    if (dBTR==BTR1)
                    {
                        iClassF=iClassF_1;
                        dBeta2f=dBeta2f_1;
                        dBeta3f=dBeta3f_1;
                        iFlClass=0;
                    }
                    else
                    {
                        iClassF=iClassF_2;
                        dBeta2f=dBeta2f_2;
                        dBeta3f=dBeta3f_2;
                        iFlClass=1;
                    }

                    //-----------------------------
                    double iClassF_3=0.0;
                    double BTR3=Calc_BTR_HC3();
                    if(BTR3 <= 8.4*Eps)		  	iClassF_3 = 1;
                    else if(BTR3 <= 9.4*Eps)  iClassF_3 = 2;
                    else if(BTR3 <= 13.6*Eps)	iClassF_3 = 3;
                    else							        iClassF_3 = 4;

                    double dBeta2f_3 = 33.5*Eps;
                    double dBeta3f_3 = 42.0*Eps;

                    //Get Worst of All
                    dBTR=max(dBTR,BTR3);
                    if (dBTR==BTR3)
                    {
                        iClassF=iClassF_3;
                        dBeta2f=dBeta2f_3;
                        dBeta3f=dBeta3f_3;
                        iFlClass=2;
                    }


                }
                else
                {
                    if(dBTR <= 8.4*Eps)		  	iClassF = 1;
                    else if(dBTR <= 9.4*Eps)  iClassF = 2;
                    else if(dBTR <= 13.6*Eps)	iClassF = 3;
                    else							        iClassF = 4;
                    dBeta2f = 9.4*Eps; dBeta3f = 13.6*Eps; 

                }


            }
        }
    }

    // web
    // Web of a channel
    if(m_iType==1 || m_iType==9)  // C, 2C
    {
        if(dHTR <= 42.*Eps)	  	iClassW = 3;
        else							      iClassW = 4; 

    }
    else  // Web of an I-, H- or box section
    {
        double dr1 = 0.0, dr2 = 0.0;

        if(m_iType==4)  Eps = Get_Epsilon(m_dfy);

        if(bPureBend && !bPureComp) // pure bending
        {
            // Neutral axis at mid-depth
            if(dHTR <= 84.*Eps)		  	iClassW = 1;
            else if(dHTR <= 105.*Eps) iClassW = 2;
            else if(dHTR <= 126.*Eps)	iClassW = 3;
            else							        iClassW = 4; 

            dBeta2w = 105.*Eps; dBeta3w = 126.*Eps; 

        }
        else if(!bPureBend && bPureComp) // pure compression
        {

            Calc_r1r2(dr1, dr2);
            // Axial compression
            if(dHTR <= 42.*Eps) iClassW = 3;
            else                iClassW = 4;  

            dBeta2w = 42.*Eps;
            dBeta3w = 42.*Eps;

        }
        else
        {

            Calc_r1r2(dr1, dr2);
            // Generally
            if(dr1 < 0.0) // if r1 is negative
            {
                if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)         iClassW = 1;
                else if(dHTR <= (105.*Eps/(1+dr1)) && dHTR <= 42.*Eps)   iClassW = 2;
                else if(dHTR <= (126.*Eps/(1+2*dr2)) && dHTR <= 42.*Eps) iClassW = 3;
                else                                                     iClassW = 4;

                dBeta2w = min(105.*Eps/(1+dr1), 42.*Eps);
                dBeta3w = min(126.*Eps/(1+2*dr2), 42.*Eps);

            }
            else  // if r1 is positive
            {
                if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)           iClassW = 1;
                else if(dHTR <= (105.*Eps/(1+1.5*dr1)) && dHTR <= 42.*Eps) iClassW = 2;
                else if(dHTR <= (126.*Eps/(1+2.0*dr2)) && dHTR <= 42.*Eps) iClassW = 3;
                else                                                     iClassW = 4;

                dBeta2w = min(105.*Eps/(1+dr1), 42.*Eps);
                dBeta3w = min(126.*Eps/(1+2*dr2), 42.*Eps);

            }    
        }  
    }
    double m_iClass  = max(iClassW, iClassF);
    return m_iClass;
}

double CDgnPlateGirder_IRC::Calc_Class_General(int &iClassF,int &iClassW,double &dfy,double &dBTR_calg)
{
    double dBeta2f,  dBeta2w,  dBeta3f,  dBeta3w;
    //PROFILING(_T("CDgnPlateGirder_IRC::Calc_Class_General"));
    // C, H, B, SB, 2C
    // IS:800-2007 Table 2. p.18
    double dHTR = Calc_HTR();
    double dBTR = dBTR_calg;//Calc_BTR();
    double Eps = Get_Epsilon(dfy);

    BOOL bPureBend = (fabs(m_dPu) < m_dZero ? TRUE : FALSE);
    BOOL bPureComp = (max(fabs(m_dMuy),fabs(m_dMuz)) < m_dZero && m_dPu < 0.0 ? TRUE : FALSE);
    //double m_iBuiltUp = 2.0;
    // flange
    // Outstand element of compression flange
    if(bPureComp) // pure compression
    {
        if (m_iType!=45)
        {
            if(dBTR <= 42.*Eps)	  	iClassF = 3;
            else							      iClassF = 4;

            dBeta2f = 42.*Eps; dBeta3f = 42.*Eps; 
            //Print_IS800_07_015_2(iClassF, dBTR, Eps, m_dfyf);
        }
        else
        {
            //For compound flange 3 width to thickness ratios need to be considered and worst of them to be used for design
            double iClassF_1=0.0;
            double BTR1=Calc_BTR_HC1();
            int iFlClass=0;//0-BTR1 Highest,1-BTR2 Highest,2-BTR3 Highest
            if(BTR1 <= 9.4*Eps)		  	iClassF_1 = 1;
            else if(BTR1 <= 10.5*Eps) iClassF_1 = 2;
            else if(BTR1 <= 15.7*Eps)	iClassF_1 = 3;
            else							        iClassF_1 = 4;

            double  dBeta2f_1 = 10.5*Eps; 
            double dBeta3f_1  = 15.7*Eps; 

            //------------------
            double iClassF_2=0.0;
            double BTR2=Calc_BTR_HC2();
            if(BTR2 <= 29.3*Eps)	  	iClassF_2 = 1;
            else if(BTR2 <= 33.5*Eps) iClassF_2 = 2;
            else if(BTR2 <= 42.0*Eps)	iClassF_2 = 3;
            else							        iClassF_2 = 4;

            double dBeta2f_2 = 33.5*Eps;
            double dBeta3f_2 = 42.0*Eps; 

            //Get Worst of Both
            dBTR=max(BTR1,BTR2);
            if (dBTR==BTR1)
            {
                iClassF=iClassF_1;
                dBeta2f=dBeta2f_1;
                dBeta3f=dBeta3f_1;
                iFlClass=0;
            }
            else
            {
                iClassF=iClassF_2;
                dBeta2f=dBeta2f_2;
                dBeta3f=dBeta3f_2;
                iFlClass=1;
            }


            //-----------------------------
            double iClassF_3=0.0;
            double BTR3=Calc_BTR_HC3();
            if(BTR3 <= 8.4*Eps)		  	iClassF_3 = 1;
            else if(BTR3 <= 9.4*Eps)  iClassF_3 = 2;
            else if(BTR3 <= 13.6*Eps)	iClassF_3 = 3;
            else							        iClassF_3 = 4;

            double dBeta2f_3 = 9.4*Eps;
            double dBeta3f_3 = 13.6*Eps;

            //Get Worst of All
            dBTR=max(dBTR,BTR3);
            if (dBTR==BTR3)
            {
                iClassF=iClassF_3;
                dBeta2f=dBeta2f_3;
                dBeta3f=dBeta3f_3;
                iFlClass=2;
            }



        }

    }
    else
    {
        if(m_iType==4) // if B. (Internal element)
        {
            if(dBTR <= 29.3*Eps)	  	iClassF = 1;
            else if(dBTR <= 33.5*Eps) iClassF = 2;
            else if(dBTR <= 42.0*Eps)	iClassF = 3;
            else							        iClassF = 4;

            dBeta2f = 33.5*Eps; dBeta3f = 42.0*Eps;     

        }
        else  // if C, H, SB, 2C  (Outstand element)
        {
            if(m_bIsRolledSection)	// Rolled.
            {
                if(dBTR <= 9.4*Eps)		  	iClassF = 1;
                else if(dBTR <= 10.5*Eps) iClassF = 2;
                else if(dBTR <= 15.7*Eps)	iClassF = 3;
                else							        iClassF = 4;

                dBeta2f = 10.5*Eps; dBeta3f = 15.7*Eps; 

            }
            else              // Built-up
            {
                if (m_iType==45)
                {
                    //For compound flange 3 width to thickness ratios need to be considered and worst of them to be used for design
                    double iClassF_1=0.0;
                    double BTR1=Calc_BTR_HC1();
                    int iFlClass=0;//0-BTR1 Highest,1-BTR2 Highest,2-BTR3 Highest
                    if(BTR1 <= 9.4*Eps)		  	iClassF_1 = 1;
                    else if(BTR1 <= 10.5*Eps) iClassF_1 = 2;
                    else if(BTR1 <= 15.7*Eps)	iClassF_1 = 3;
                    else							        iClassF_1 = 4;

                    double  dBeta2f_1 = 10.5*Eps; 
                    double dBeta3f_1  = 15.7*Eps; 

                    //------------------
                    double iClassF_2=0.0;
                    double BTR2=Calc_BTR_HC2();
                    if(BTR2 <= 29.3*Eps)	  	iClassF_2 = 1;
                    else if(BTR2 <= 33.5*Eps) iClassF_2 = 2;
                    else if(BTR2 <= 42.0*Eps)	iClassF_2 = 3;
                    else							        iClassF_2 = 4;

                    double dBeta2f_2 = 33.5*Eps;
                    double dBeta3f_2 = 42.0*Eps; 

                    //Get Worst of Both
                    dBTR=max(BTR1,BTR2);
                    if (dBTR==BTR1)
                    {
                        iClassF=iClassF_1;
                        dBeta2f=dBeta2f_1;
                        dBeta3f=dBeta3f_1;
                        iFlClass=0;
                    }
                    else
                    {
                        iClassF=iClassF_2;
                        dBeta2f=dBeta2f_2;
                        dBeta3f=dBeta3f_2;
                        iFlClass=1;
                    }

                    //-----------------------------
                    double iClassF_3=0.0;
                    double BTR3=Calc_BTR_HC3();
                    if(BTR3 <= 8.4*Eps)		  	iClassF_3 = 1;
                    else if(BTR3 <= 9.4*Eps)  iClassF_3 = 2;
                    else if(BTR3 <= 13.6*Eps)	iClassF_3 = 3;
                    else							        iClassF_3 = 4;

                    double dBeta2f_3 = 33.5*Eps;
                    double dBeta3f_3 = 42.0*Eps;

                    //Get Worst of All
                    dBTR=max(dBTR,BTR3);
                    if (dBTR==BTR3)
                    {
                        iClassF=iClassF_3;
                        dBeta2f=dBeta2f_3;
                        dBeta3f=dBeta3f_3;
                        iFlClass=2;
                    }


                }
                else
                {
                    if(dBTR <= 8.4*Eps)		  	iClassF = 1;
                    else if(dBTR <= 9.4*Eps)  iClassF = 2;
                    else if(dBTR <= 13.6*Eps)	iClassF = 3;
                    else							        iClassF = 4;
                    dBeta2f = 9.4*Eps; dBeta3f = 13.6*Eps; 

                }


            }
        }
    }

    // web
    // Web of a channel
    if(m_iType==1 || m_iType==9)  // C, 2C
    {
        if(dHTR <= 42.*Eps)	  	iClassW = 3;
        else							      iClassW = 4; 

    }
    else  // Web of an I-, H- or box section
    {
        double dr1 = 0.0, dr2 = 0.0;

        if(m_iType==4)  Eps = Get_Epsilon(m_dfy);

        if(bPureBend && !bPureComp) // pure bending
        {
            // Neutral axis at mid-depth
            if(dHTR <= 84.*Eps)		  	iClassW = 1;
            else if(dHTR <= 105.*Eps) iClassW = 2;
            else if(dHTR <= 126.*Eps)	iClassW = 3;
            else							        iClassW = 4; 

            dBeta2w = 105.*Eps; dBeta3w = 126.*Eps; 

        }
        else if(!bPureBend && bPureComp) // pure compression
        {

            Calc_r1r2(dr1, dr2);
            // Axial compression
            if(dHTR <= 42.*Eps) iClassW = 3;
            else                iClassW = 4;  

            dBeta2w = 42.*Eps;
            dBeta3w = 42.*Eps;

        }
        else
        {

            Calc_r1r2(dr1, dr2);
            // Generally
            if(dr1 < 0.0) // if r1 is negative
            {
                if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)         iClassW = 1;
                else if(dHTR <= (105.*Eps/(1+dr1)) && dHTR <= 42.*Eps)   iClassW = 2;
                else if(dHTR <= (126.*Eps/(1+2*dr2)) && dHTR <= 42.*Eps) iClassW = 3;
                else                                                     iClassW = 4;

                dBeta2w = min(105.*Eps/(1+dr1), 42.*Eps);
                dBeta3w = min(126.*Eps/(1+2*dr2), 42.*Eps);

            }
            else  // if r1 is positive
            {
                if(dHTR <= (84.*Eps/(1+dr1)) && dHTR <= 42.*Eps)           iClassW = 1;
                else if(dHTR <= (105.*Eps/(1+1.5*dr1)) && dHTR <= 42.*Eps) iClassW = 2;
                else if(dHTR <= (126.*Eps/(1+2.0*dr2)) && dHTR <= 42.*Eps) iClassW = 3;
                else                                                     iClassW = 4;

                dBeta2w = min(105.*Eps/(1+dr1), 42.*Eps);
                dBeta3w = min(126.*Eps/(1+2*dr2), 42.*Eps);

            }    
        }  
    }
    double m_iClass  = max(iClassW, iClassF);
    return m_iClass;
}

double CDgnPlateGirder_IRC::Calc_Class_P_SR()
{
    //PROFILING(_T("CACSCode_IS800_07::Calc_Class_P_SR"));
    // IS:800-2007 Table 2. p.18
    // Plastic(1), Compact(2), Semi-compact(3), Slender(4).
    int iClassF=0, iClassW=0;
    double dHTR = Calc_HTR();
    double dBTR = Calc_BTR();
    double Eps = Get_Epsilon(m_dfy);

    BOOL bPureComp = (max(fabs(m_dMuy),fabs(m_dMuz)) < m_dZero && m_dPu < 0.0 ? TRUE : FALSE);

    if(bPureComp) // pure compression
    {
        // CHS, Axial compression
        if(dBTR <= 88*pow(Eps,2))		  	iClassF = 3;
        else                            iClassF = 4;

    }
    else
    {
        // CHS, Compression due to bending
        if(dBTR <= 42*pow(Eps,2))		  	iClassF = 1;
        else if(dBTR <= 52*pow(Eps,2))  iClassF = 2;
        else if(dBTR <= 146*pow(Eps,2))	iClassF = 3;
        else							        		 	iClassF = 4;

    }

    double m_iClass  = iClassW = iClassF;
    return m_iClass;
}


double CDgnPlateGirder_IRC::Calc_BTR_HC1()  // for Flange
{
    // Get BTR
    double dBTR=0.0;
    // Same Get_BTR() at CACSCode_EuroCode3.
    //Ratio of the out stand of compound flange to thickness of original flange
    //Classified under _T("Out stand element of compression flange-Rolled section")
    dBTR =m_dB1/(2*m_dtf2);	          // HC.


    return dBTR;
}

double CDgnPlateGirder_IRC::Calc_BTR_HC2()  // for Flange
{
    // Get BTR
    double dBTR=0.0;
    // Same Get_BTR() at CACSCode_EuroCode3.
    //Ratio of Internal width of plate between lines of weld connecting to original flange,to the thickness of plate
    //Classified under _T("Internal Element of compression flange-Compression due to bending")
    dBTR =m_dB1/(m_dHw);	          // HC.


    return dBTR;
}

double CDgnPlateGirder_IRC::Calc_BTR_HC3()  // for Flange
{
    // Get BTR
    double dBTR=0.0;
    // Same Get_BTR() at CACSCode_EuroCode3.
    //Ratio of outstand of the plate beyond lines of weld connecting to original flange to thickness of plate
    //Classified under _T("Outstand Element of compression flange-Welded Section")
    if (m_dH>m_dB1)
    {
        dBTR =(m_dH-m_dB1)/(2*m_dHw);	          // HC.
    }
    else
    {
        dBTR =(m_dB1-m_dH)/(2*m_dHw);	          // HC.
    }



    return dBTR;
}

void CDgnPlateGirder_IRC::Get_StaticForceMoment(int ElemNo, int LcaseNo, T_LCOM_D_UL& LcomUlData) 
{
    //////////////////////////////////////////////
    // <Remember> Data를 받아올때는 Code Unit기준
    T_UNIT_INDEX CurIndex, CngIndex;
    m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
    CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
    CngIndex.nBase_Force  = D_UNITSYS_FORCE_INDEX_TON;
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
    // <Remember> Data를 받아올때는 Code Unit기준
    //////////////////////////////////////////////

    for(int i=0; i<5; i++)
    {
        m_Fxx[i]  = 0.0; m_Fyy[i]  = 0.0; m_Fzz[i]  = 0.0; m_BMx[i]  = 0.0; m_BMy[i]  = 0.0; m_BMz[i]  = 0.0; 
        m_FxxD[i] = 0.0; m_FyyD[i] = 0.0; m_FzzD[i] = 0.0; m_BMxD[i] = 0.0; m_BMyD[i] = 0.0; m_BMzD[i] = 0.0; 
        m_FxxL[i] = 0.0; m_FyyL[i] = 0.0; m_FzzL[i] = 0.0; m_BMxL[i] = 0.0; m_BMyL[i] = 0.0; m_BMzL[i] = 0.0;
        m_FxxR[i] = 0.0; m_FyyR[i] = 0.0; m_FzzR[i] = 0.0; m_BMxR[i] = 0.0; m_BMyR[i] = 0.0; m_BMzR[i] = 0.0;
    }

    // Get Force Data by Load Case.
    T_LCOM_D LcaseData;
    LcaseData.Initialize();
    // Remember LoadCase Type.(LcomUlData.Combination[LcaseNo].AnalType -> D_LOADCASE_STATIC.)
    m_pDoc->m_pPostCtrl->MakeLoadComb(LcomUlData.Combination[LcaseNo].AnalType, 
        LcomUlData.Combination[LcaseNo].LoadCaseKey, LcaseData);
    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcaseData);
    // Get Load Case Type(D,L,W,E,...).
    T_STLD_D Lcase;
    Lcase.Initialize();
    m_pDoc->m_pPostCtrl->GetStld(LcomUlData.Combination[LcaseNo].LoadCaseKey, Lcase);
    // Get Element Data.
    T_ELEM_D Edata;
    Edata.Initialize();
    BOOL bCheck = m_pDoc->m_pAttrCtrl->GetElem(ElemNo, Edata);
    ASSERT(bCheck);
    int iElemType = Edata.eltyp;

    double factor = LcomUlData.Combination[LcaseNo].Factor;



    if(Lcase.LoadCaseType == _T("D")) // Dead Load.
    {	
        // Save Forces and Moments by Position.
        for(int i=0; i<5; i++)
        {
            if(m_pDoc->m_pAttrCtrl->IsBeam(iElemType))	// Beam Type.
            {
                // Change by ZINU.('01.11.27).
                T_STRB_D StrMaxData, StrMinData, StrAbsData;
                StrMaxData.Initialize();
                StrMinData.Initialize();
                StrAbsData.Initialize();
                m_pDoc->m_pPostCtrl->GetStrbNew(ElemNo, &StrMaxData, &StrMinData, &StrAbsData, TRUE);
                if(i < 4) 
                {
                    m_FxxD[i] = m_FxxD[i] + StrMaxData.dblForce[i][0] * factor;
                    m_FyyD[i] = m_FyyD[i] + StrMaxData.dblForce[i][1] * factor;
                    m_FzzD[i] = m_FzzD[i] + StrMaxData.dblForce[i][2] * factor;
                    m_BMxD[i] = m_BMxD[i] + StrMaxData.dblForce[i][4] * factor;
                    m_BMyD[i] = m_BMyD[i] + StrMaxData.dblForce[i][5] * factor;
                    m_BMzD[i] = m_BMzD[i] + StrMaxData.dblForce[i][6] * factor;
                }
                else if(i == 4)
                {
                    m_FxxD[i] = m_FxxD[i] + StrMaxData.dblForce[i-1][6] * factor;
                    m_FyyD[i] = m_FyyD[i] + StrMaxData.dblForce[i-1][7] * factor;
                    m_FzzD[i] = m_FzzD[i] + StrMaxData.dblForce[i-1][8] * factor;
                    m_BMxD[i] = m_BMxD[i] + StrMaxData.dblForce[i-1][9] * factor;
                    m_BMyD[i] = m_BMyD[i] + StrMaxData.dblForce[i-1][10] * factor;
                    m_BMzD[i] = m_BMzD[i] + StrMaxData.dblForce[i-1][11] * factor;
                }
            }
            else if(m_pDoc->m_pAttrCtrl->IsTruss(iElemType))	// Truss Type.
            {
                // Change by ZINU.('01.11.27).
                T_STRT_D StrMaxData, StrMinData, StrAbsData;
                StrMaxData.Initialize();
                StrMinData.Initialize();
                StrAbsData.Initialize();
                m_pDoc->m_pPostCtrl->GetStrtNew(ElemNo, &StrMaxData, &StrMinData, &StrAbsData);
                if(i < 4) 			m_FxxD[i] = m_FxxD[i] + StrMaxData.dblForce[0] * factor;
                else if(i == 4)	m_FxxD[i] = m_FxxD[i] + StrMaxData.dblForce[1] * factor;
            }
        }
    }
    else if(Lcase.LoadCaseType == _T("L") || Lcase.LoadCaseType == _T("LR")) // Live Load.
    {
        // Save Forces and Moments by Position.
        for(int i=0; i<5; i++)
        {
            if(m_pDoc->m_pAttrCtrl->IsBeam(iElemType))	// Beam Type.
            {
                // Change by ZINU.('01.11.27).
                T_STRB_D StrMaxData, StrMinData, StrAbsData;
                StrMaxData.Initialize();
                StrMinData.Initialize();
                StrAbsData.Initialize();
                m_pDoc->m_pPostCtrl->GetStrbNew(ElemNo, &StrMaxData, &StrMinData, &StrAbsData, TRUE);
                if(i < 4) 
                {
                    m_FxxL[i] = m_FxxL[i] + StrMaxData.dblForce[i][0] * factor;
                    m_FyyL[i] = m_FyyL[i] + StrMaxData.dblForce[i][1] * factor;
                    m_FzzL[i] = m_FzzL[i] + StrMaxData.dblForce[i][2] * factor;
                    m_BMxL[i] = m_BMxL[i] + StrMaxData.dblForce[i][4] * factor;
                    m_BMyL[i] = m_BMyL[i] + StrMaxData.dblForce[i][5] * factor;
                    m_BMzL[i] = m_BMzL[i] + StrMaxData.dblForce[i][6] * factor;
                }
                else if(i == 4)
                {
                    m_FxxL[i] = m_FxxL[i] + StrMaxData.dblForce[i-1][6] * factor;
                    m_FyyL[i] = m_FyyL[i] + StrMaxData.dblForce[i-1][7] * factor;
                    m_FzzL[i] = m_FzzL[i] + StrMaxData.dblForce[i-1][8] * factor;
                    m_BMxL[i] = m_BMxL[i] + StrMaxData.dblForce[i-1][9] * factor;
                    m_BMyL[i] = m_BMyL[i] + StrMaxData.dblForce[i-1][10] * factor;
                    m_BMzL[i] = m_BMzL[i] + StrMaxData.dblForce[i-1][11] * factor;
                }
            }	
            else if(m_pDoc->m_pAttrCtrl->IsTruss(iElemType))	// Truss Type.
            {
                // Change by ZINU.('01.11.27).
                T_STRT_D StrMaxData, StrMinData, StrAbsData;
                StrMaxData.Initialize();
                StrMinData.Initialize();
                StrAbsData.Initialize();
                m_pDoc->m_pPostCtrl->GetStrtNew(ElemNo, &StrMaxData, &StrMinData, &StrAbsData);
                if(i < 4) 			m_FxxL[i] = m_FxxL[i] + StrMaxData.dblForce[0] * factor;
                else if(i == 4)	m_FxxL[i] = m_FxxL[i] + StrMaxData.dblForce[1] * factor;
            }
        }
    }
    else // etc. LoadCase.
    {	
        // Save Forces and Moments by Position.
        for(int i=0; i<5; i++)
        {
            if(m_pDoc->m_pAttrCtrl->IsBeam(iElemType))	// Beam Type.
            {
                // Change by ZINU.('01.11.27).
                T_STRB_D StrMaxData, StrMinData, StrAbsData;
                StrMaxData.Initialize();
                StrMinData.Initialize();
                StrAbsData.Initialize();
                m_pDoc->m_pPostCtrl->GetStrbNew(ElemNo, &StrMaxData, &StrMinData, &StrAbsData, TRUE);
                if(i < 4) 
                {
                    m_FxxR[i] = m_FxxR[i] + StrMaxData.dblForce[i][0] * factor;
                    m_FyyR[i] = m_FyyR[i] + StrMaxData.dblForce[i][1] * factor;
                    m_FzzR[i] = m_FzzR[i] + StrMaxData.dblForce[i][2] * factor;
                    m_BMyR[i] = m_BMyR[i] + StrMaxData.dblForce[i][4] * factor;
                    m_BMyR[i] = m_BMyR[i] + StrMaxData.dblForce[i][5] * factor;
                    m_BMzR[i] = m_BMzR[i] + StrMaxData.dblForce[i][6] * factor;
                }
                else if(i == 4)
                {
                    m_FxxR[i] = m_FxxR[i] + StrMaxData.dblForce[i-1][6] * factor;
                    m_FyyR[i] = m_FyyR[i] + StrMaxData.dblForce[i-1][7] * factor;
                    m_FzzR[i] = m_FzzR[i] + StrMaxData.dblForce[i-1][8] * factor;
                    m_BMxR[i] = m_BMxR[i] + StrMaxData.dblForce[i-1][9] * factor;
                    m_BMyR[i] = m_BMyR[i] + StrMaxData.dblForce[i-1][10] * factor;
                    m_BMzR[i] = m_BMzR[i] + StrMaxData.dblForce[i-1][11] * factor;
                }
            }	
            else if(m_pDoc->m_pAttrCtrl->IsTruss(iElemType))	// Truss Type.
            {
                // Change by ZINU.('01.11.27).
                T_STRT_D StrMaxData, StrMinData, StrAbsData;
                StrMaxData.Initialize();
                StrMinData.Initialize();
                StrAbsData.Initialize();
                m_pDoc->m_pPostCtrl->GetStrtNew(ElemNo, &StrMaxData, &StrMinData, &StrAbsData);
                if(i < 4) 			m_FxxR[i] = m_FxxR[i] + StrMaxData.dblForce[0] * factor;
                else if(i == 4)	m_FxxR[i] = m_FxxR[i] + StrMaxData.dblForce[1] * factor;
            }
        }
    }
    //////////////////////////////////////////////
    // <Remember> Data를 받고나서는 User Unit기준
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);
    // <Remember> Data를 받고나서는 User Unit기준
    //////////////////////////////////////////////
}