// DgnStlRatingDataCtrl.cpp: implementation of the CDgnStlRatingDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "DgnStlRatingDataCtrl.h"
#include "CRCForceCtrl.h"
//#include "Dgn_CSGManager.h"
#include "DgnDataCtrl.h"
#include "DgnCsgDataCtrl.h"
#include "DgnCsgDataCtrlAASHTO12.h"
#include "DgnCsgDataCtrlKSCE_LSD15.h"
#include "DgnCsgDataCtrlCSA_S6_14.h"
#include "DgnCsgDataCtrlKSSC_2014.h"
#include "DgnCsgDataCtrlCS457_R1.h"
#include "DgnCsgDataCtrlNR_GN_CIV_025_06.h"
#include "DgnForceCtrl.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\PostCtrl.h"
//#include "..\wg_db\PlateGirderDesign.h"
//#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
//#include "..\wg_base\wg_base_I_PolyMaker.h"
//#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\DesignLoadRating.h"
//#include "..\wg_umd\UMDC_DesignManagerBase.h"
#include "..\wg_db\SelfWgt.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const double cMaxRF = 1.0E+70;
#define DgnDivide(a,b) fabs(b)>cDGN_Zero ? (a/b) : cMaxRF
#define DgnAbsDivide(a,b) fabs(b)>cDGN_Zero ? fabs(a/b) : cMaxRF

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnStlRatingDataCtrl::CDgnStlRatingDataCtrl() : CDgnPscCommon()
{
    m_nDgnCode = m_iCivilCode;

    m_pRatingManager = new CDgn_RatingManager;
    m_pDgnDataCtrl = new CDgnDataCtrl;
    // MQC.20510 CSG Composite Rating
    //m_pCSGDataCtrl = new CDgnCsgDataCtrl;
    SetCSGDataCtrl(m_nDgnCode);
}

CDgnStlRatingDataCtrl::~CDgnStlRatingDataCtrl()
{
    if(m_pRatingManager!=NULL) 
    {
        delete m_pRatingManager;
        m_pRatingManager = NULL;
    }

    if(m_pDgnDataCtrl!=NULL) 
    {
        delete m_pDgnDataCtrl;
        m_pDgnDataCtrl = NULL;
    }
    if (m_pCSGDataCtrl!=NULL)
    {
        delete m_pCSGDataCtrl;
        m_pCSGDataCtrl = NULL;
    }
}

void CDgnStlRatingDataCtrl::SetCSGDataCtrl(int nDgnCode)
{
    switch (nDgnCode)
    {
    case STL_AASHTO_LRFD12:
    case STL_AASHTO_LRFD19:
        {
            m_pCSGDataCtrl = new CDgnCsgDataCtrlAASHTO12;
        }
        break;
    case STL_KSCE_LSD15:
        {
            T_RPAS_D RpasD;
            if (!m_pDoc->m_pAttrCtrl2->GetRpas(RpasD)) RpasD.Initialize();

            if (RpasD.nBridgeType==0)
            {
                m_pCSGDataCtrl = new CDgnCsgDataCtrlKSCE_LSD15;
            }
            else
            {// 곡선교
                m_pCSGDataCtrl = new CDgnCsgDataCtrlKSSC_2014;
            }
        }
        break;
    case STL_CS454_20:
        {
            m_pCSGDataCtrl = new CDgnCsgDataCtrlCS457_R1;
        }
        break;
    case STL_NR_GN_CIV_025_06:
        {
            m_pCSGDataCtrl = new CDgnCsgDataCtrlNR_GN_CIV_025_06;
        }
        break;
    case STL_KSCE_ASD10:
    case STL_KSCE_ASD05:
    case STL_KSCE_RAIL_ASD11:
    case STL_KSCE_RAIL_ASD04:
        {
            m_pCSGDataCtrl = new CDgnCsgDataCtrl;
        }
        break;
    default:
        ASSERT(0);
        break;
    }
}

void CDgnStlRatingDataCtrl::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
    CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	switch ( m_nDgnCode )
    {
    case STL_KSCE_ASD10:
    case STL_KSCE_ASD05:
    case STL_KSCE_RAIL_ASD11:
    case STL_KSCE_RAIL_ASD04:
    case STL_KSCE_LSD15:
        CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
        CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
        m_pDataCtrl->Set_UnitIndex(CodeUnit);
        if (m_pCSGDataCtrl!=nullptr)
        {
            int nCsgDgnCode=0;
            if      (m_nDgnCode==STL_KSCE_ASD10)      nCsgDgnCode = KSCE_ASD10_CSG;
            else if (m_nDgnCode==STL_KSCE_RAIL_ASD11) nCsgDgnCode = KSCE_RAIL_ASD11_CSG;
            else if (m_nDgnCode==STL_KSCE_LSD15)      nCsgDgnCode = KSCE_LSD15_CSG;
            else ASSERT(0);
            m_pCSGDataCtrl->SetCsgDgnCode(nCsgDgnCode);
            m_pCSGDataCtrl->SetDataCtrlPointer(pDataCtrl);
        }
        break;
    case STL_AASHTO_LRFD12:    
        CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
        CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;
        m_pDataCtrl->Set_UnitIndex(CodeUnit);
        if (m_pCSGDataCtrl!=nullptr)
        {
            m_pCSGDataCtrl->SetCsgDgnCode(AASHTO_LRFD12_CSG);
            m_pCSGDataCtrl->SetDataCtrlPointer(pDataCtrl);
        }
        break;
    case STL_AASHTO_LRFD19:
        CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
        CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;
        m_pDataCtrl->Set_UnitIndex(CodeUnit);
        if ( m_pCSGDataCtrl!=nullptr )
        {
            m_pCSGDataCtrl->SetCsgDgnCode(AASHTO_LRFD17_CSG);
            m_pCSGDataCtrl->SetDataCtrlPointer(pDataCtrl);
        }
        break;
    case STL_CS454_20:
        CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
        CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
        m_pDataCtrl->Set_UnitIndex(CodeUnit);
        if ( m_pCSGDataCtrl!=nullptr )
        {
            m_pCSGDataCtrl->SetCsgDgnCode(CS457_R1_CSG);
            m_pCSGDataCtrl->SetDataCtrlPointer(pDataCtrl);
        }
        break;
    case STL_NR_GN_CIV_025_06:
        CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
        CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
        m_pDataCtrl->Set_UnitIndex(CodeUnit);
        if ( m_pCSGDataCtrl!=nullptr )
        {
            m_pCSGDataCtrl->SetCsgDgnCode(NR_GN_CIV_025_06_CSG);
            m_pCSGDataCtrl->SetDataCtrlPointer(pDataCtrl);
        }
        break;

    default: ASSERT(0); break;
    }

    InitialData(m_nDgnCode);

}

BOOL CDgnStlRatingDataCtrl::InitialData(int nDgnCode)
{
    if(!m_pRatingManager->InitialData(DGNE_CODETYPE_STEEL, nDgnCode)) return FALSE;
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Make_RatingMatlDgnData()
{
    m_mRatingMatl.RemoveAll();
    CArray<T_MATL_K, T_MATL_K> aMatlKey;
    m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlKey);
    int nMatlKeySize = aMatlKey.GetSize();
    //T_MATD_D MatdD;  

    for(int i=0; i<nMatlKeySize; ++i)
    {
        T_MATD_D MatdD; MatdD.Initialize();	
        T_MATL_K MatlK = aMatlKey[i];
        STL_KR_MATL StlMatL;
        if (m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD))
        {
            StlMatL.StlMatL.CodeName       = CT2W(MatdD.Data1.CodeName); //_T("KS01-Civil(S)"); // 우선 고정.
            StlMatL.StlMatL.MatlName       = CT2W(MatdD.Data1.CodeMatlName);
            StlMatL.StlMatL.Steel.Elast    = MatdD.Data1.Analysis.Elast;//D_UNITSYS_BASE_ELAST;          
            StlMatL.StlMatL.Steel.Poisson  = MatdD.Data1.Analysis.Poisson;     //D_UNITSYS_NONE;  
            StlMatL.StlMatL.Steel.S_Fu     = MatdD.Data1.Design.S_Fu;        //D_UNITSYS_BASE_STRESS;
            StlMatL.StlMatL.Steel.S_Fy     = MatdD.Data1.Design.S_Fy;        //D_UNITSYS_BASE_STRESS
        }
        else
        {
            CDBLib::GetDefaultStlMatl(MatdD.Data1.CodeName, MatdD.Data1.CodeMatlName);
            if (m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatdD))
            {
                StlMatL.StlMatL.CodeName       = CT2W(MatdD.Data1.CodeName); //_T("KS01-Civil(S)"); // 우선 고정.
                StlMatL.StlMatL.MatlName       = CT2W(MatdD.Data1.CodeMatlName);
                StlMatL.StlMatL.Steel.Elast    = MatdD.Data1.Analysis.Elast;//D_UNITSYS_BASE_ELAST;          
                StlMatL.StlMatL.Steel.Poisson  = MatdD.Data1.Analysis.Poisson;     //D_UNITSYS_NONE;  
                StlMatL.StlMatL.Steel.S_Fu     = MatdD.Data1.Design.S_Fu;        //D_UNITSYS_BASE_STRESS;
                StlMatL.StlMatL.Steel.S_Fy     = MatdD.Data1.Design.S_Fy;        //D_UNITSYS_BASE_STRESS
            }
        }

        //CSG_MATL_ELEM CsgMatlD;
        //ConvertRatingCalcMatlData(&MatlElem, CsgMatlD);

        m_mRatingMatl.SetAt(MatlK, StlMatL/*CsgMatlD*/);
    }
    return TRUE;
}

int CDgnStlRatingDataCtrl::Get_ShapeNoBySectType(CString strSectType)
{
    int iSectType=0;
    if(strSectType==D_SECT_SHAPE_REG_L)		iSectType = 0;
    else if(strSectType==D_SECT_SHAPE_REG_C)		iSectType = 1;
    else if(strSectType==D_SECT_SHAPE_REG_H)		iSectType = 2;
    else if(strSectType==D_SECT_SHAPE_REG_T)		iSectType = 3;
    else if(strSectType==D_SECT_SHAPE_REG_B)		iSectType = 4;
    else if(strSectType==D_SECT_SHAPE_REG_P)		iSectType = 5;
    else if(strSectType==D_SECT_SHAPE_REG_SR)		iSectType = 6;
    else if(strSectType==D_SECT_SHAPE_REG_SB)		iSectType = 7;
    else if(strSectType==D_SECT_SHAPE_REG_2L)		iSectType = 8;
    else if(strSectType==D_SECT_SHAPE_REG_2C)		iSectType = 9;
    else if(strSectType==D_SECT_SHAPE_REG_CC)		iSectType = 10;
    else if(strSectType==D_SECT_SHAPE_REG_CL)		iSectType = 17;
    else if(strSectType==D_SECT_SHAPE_REG_GEN)	iSectType = 21;
    else if(strSectType==D_SECT_SHAPE_REG_CB)	  iSectType = 23;
    else if(strSectType==D_SECT_SHAPE_REG_UDT)  iSectType = 31;
    else if(strSectType==D_SECT_SHAPE_COMA_HT)	iSectType = 41;
    else if(strSectType==D_SECT_SHAPE_COMA_2T1)	iSectType = 42;
    else if(strSectType==D_SECT_SHAPE_COMA_H2T)	iSectType = 43;
    else if(strSectType==D_SECT_SHAPE_COMPO_B)  iSectType = 101;
    else if(strSectType==D_SECT_SHAPE_COMPO_I)  iSectType = 102;
    else if(strSectType==D_SECT_SHAPE_COMPO_TUB)iSectType = 103;
    else if(strSectType== D_SECT_SHAPE_STLG_I)iSectType = DGN_SECT_SHAPE_INDEX_STL_GIRDER_I;
    //else if(strSectType==D_SECT_SHAPE_STLG_B)iSectType = DGN_SECT_SHAPE_INDEX_STL_GIRDER_B;
    else	ASSERT(0);
    return iSectType;
}

BOOL CDgnStlRatingDataCtrl::Get_UnbracedLengthData(ElemPairK ElemK, BOOL bKfac, T_KFAC_D &KFacD, STL_KR_MEMBEROPTION &rData)
{
    T_LENG_D LengD; LengD.Initialize();
    BOOL bAutoCalc=FALSE;
    if (ElemK.second == EN_EL_BEAM)
    {
        if (m_pDoc->m_pAttrCtrl->GetLeng(ElemK.first, LengD))
        {
            rData.dLy = LengD.BLy;
            rData.dLz = LengD.BLz;
            rData.dLb = LengD.BLu;
            bAutoCalc = LengD.bAutoCalcLe;
        }
        else
        {
            auto itUblmK = m_pDataCtrl->m_mElemUblmKey.find(ElemK);
            if (itUblmK != m_pDataCtrl->m_mElemUblmKey.end())
            {
                T_ULEN_MEMB_D UblmD;
				auto itUblMemb = m_pDataCtrl->m_mUblenMemb.find(itUblmK->second);
                if (itUblMemb != m_pDataCtrl->m_mUblenMemb.end())
                {
                    const T_ULEN_MEMB_D& UblmD = itUblMemb->second;
                    rData.dLy = UblmD.dLength;
                    rData.dLz = UblmD.dLength;
                    rData.dLb = UblmD.dLength;
                }
            }
            else
            {
                double dElemLength = m_pDoc->calcLAVElem(ElemK.first);
                rData.dLy = dElemLength;
                rData.dLz = dElemLength;
                rData.dLb = dElemLength;
            }
        }
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
        T_LENV_D LenvD;
        if (m_pDoc->m_pAttrCtrl->GetLenv(ElemK.first, LenvD))
        {
            rData.dLy = LenvD.BLy;
            rData.dLz = LenvD.BLz;
            rData.dLb = LenvD.BLu;
            // bAutoCalc = LengD.bAutoCalcLe; auto 할 수 없는데...
        }
        else
        {
            double dElemLength = m_pDoc->calcLenthVbem(ElemK.first);
            rData.dLy = dElemLength;
            rData.dLz = dElemLength;
            rData.dLb = dElemLength;
        }
    }
    else ASSERT(0);

    if (bKfac)
    {
        rData.dKy = KFacD.BKy;
        rData.dKz = KFacD.BKz;
    }
    else
    {
        rData.dKy = rData.dKz = 1.0;
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Make_RatingSectDgnData()
{
    BOOL bPostMode = m_pDoc->IsPostMode();
    if(!bPostMode) return FALSE;

    m_mRatingSect.RemoveAll();
    CArray<T_ELEM_K,T_ELEM_K> aCheckElemK;

    ArrElemPairKey aCheckElemPairK;
    m_pDataCtrl->Get_ModiCheckElemKey(m_arElemK, m_pDataCtrl->m_aChkElemUblmKey, aCheckElemPairK);
    m_pDataCtrl->GetElemKeyList4ElemPair(EN_EL_BEAM, aCheckElemPairK, aCheckElemK);

    int nChkElemSize = aCheckElemK.GetSize();

    STL_KR_SECT SectElem;
    for(int nElem=0; nElem<nChkElemSize; ++nElem)
    {		
        T_ELEM_K ElemK = aCheckElemK[nElem];

        T_ELEM_D ElemD, ElemPreD;
        ElemD.Initialize();
        ElemPreD.Initialize();
        BOOL bGetElem = m_pDoc->m_pPostCtrl->GetElemPost(ElemK,ElemD);	  
        BOOL bGetElemPre = bGetElem = m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemPreD);	  
        if(!bGetElem || !bGetElemPre)	continue;

        T_SECT_K SectK = ElemD.elpro;
        T_SECT_K SectPreK = ElemPreD.elpro;

        //if (m_mRatingSect.Lookup(SectK, SectElem)) continue; // 있으면...
        SectElem.Initialize();

        T_TSGR_K TsgrK = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK);

        T_SECT_D SectOrgD;  

        if (TsgrK>0)
        {
            T_SECT_D TSectD;
            m_pDoc->m_pAttrCtrl->GetSect(SectPreK, TSectD);

            T_TSGR_D TsgrD;
            double dTotalLength = 0.0;
            double dPositionI   = 0.0;
            double dPositionJ   = 0.0;
            if (m_pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD))
            {
                double dTsgrTotLength = 0.0;
                m_pDoc->m_pAttrCtrl->CalcTsgrTotLength(TsgrD, ElemK, dTsgrTotLength, dPositionI);
                //         int nTsgrElemList = TsgrD.aElemList.GetSize();
                //         for(int i=0; i<nTsgrElemList; ++i)
                //         {
                //           if(TsgrD.aElemList[i]==ElemK) dPositionI = dTsgrTotLength;
                //           dTsgrTotLength += m_pDoc->calcLAVElem(TsgrD.aElemList[i]);
                //         }
                dPositionJ = dPositionI + m_pDoc->calcLAVElem(ElemD);        

                m_pDoc->m_pSectDB->CalcTaperedSection(TSectD, TsgrD, dTsgrTotLength, dPositionI, dPositionJ, 0, SectOrgD);
            }
        }
        else
        {
            //m_pDoc->m_pPostCtrl->GetSectPost(SectK, SectOrgD);
            m_pDgnDataCtrl->Get_DgnStlSect(ElemK, SectOrgD);
        }

        if (m_pDataCtrl->IsCsgSect(SectOrgD.nStype, SectOrgD.SectBefore.nStype))
        {
            m_mCompoElemK.SetAt(ElemK, ElemK);
        }

        STL_KR_SECT SectD;
        SectD.Initialize();
        SectD.Shape      = SectOrgD.SectBefore.Shape;
        SectD.iShapeType = Get_ShapeNoBySectType(SectOrgD.SectBefore.Shape); //21; // D_SECT_SHAPE_REG_GEN에 해당하는 wg_acs에서 사용하는 값이 21이어서 Setting, UMD-Steel도 같이 사용함. 
        SectD.iSectOpt   = 0;      // 0=User, 1=DB  

        // T_UMD_SECT_STIFFNESS Stiffness;
        SectD.Stiffness.Area = SectOrgD.SectBefore.SectI.Stiffness.Area;
        SectD.Stiffness.Asy  = SectOrgD.SectBefore.SectI.Stiffness.Asy;
        SectD.Stiffness.Asz  = SectOrgD.SectBefore.SectI.Stiffness.Asz;
        SectD.Stiffness.Rxx  = SectOrgD.SectBefore.SectI.Stiffness.Rxx;
        SectD.Stiffness.Ryy  = SectOrgD.SectBefore.SectI.Stiffness.Ryy;
        SectD.Stiffness.Rzz  = SectOrgD.SectBefore.SectI.Stiffness.Rzz;
        SectD.Stiffness.Cyp  = SectOrgD.SectBefore.SectI.Stiffness.Cyp;
        SectD.Stiffness.Cym  = SectOrgD.SectBefore.SectI.Stiffness.Cym;
        SectD.Stiffness.Czp  = SectOrgD.SectBefore.SectI.Stiffness.Czp;
        SectD.Stiffness.Czm  = SectOrgD.SectBefore.SectI.Stiffness.Czm;
        SectD.Stiffness.Qyb  = SectOrgD.SectBefore.SectI.Stiffness.Qyb;
        SectD.Stiffness.Qzb  = SectOrgD.SectBefore.SectI.Stiffness.Qzb;
        SectD.Stiffness.dx1  = SectOrgD.SectBefore.SectI.Stiffness.dx1;
        SectD.Stiffness.dy1  = SectOrgD.SectBefore.SectI.Stiffness.dy1;
        SectD.Stiffness.dx2  = SectOrgD.SectBefore.SectI.Stiffness.dx2;
        SectD.Stiffness.dy2  = SectOrgD.SectBefore.SectI.Stiffness.dy2;
        SectD.Stiffness.dx3  = SectOrgD.SectBefore.SectI.Stiffness.dx3;
        SectD.Stiffness.dy3  = SectOrgD.SectBefore.SectI.Stiffness.dy3;
        SectD.Stiffness.dx4  = SectOrgD.SectBefore.SectI.Stiffness.dx4;
        SectD.Stiffness.dy4  = SectOrgD.SectBefore.SectI.Stiffness.dy4;
        SectD.Stiffness.Roy  = (SectD.Stiffness.Area==0.0) ? 0.0 : sqrt(SectD.Stiffness.Ryy/SectD.Stiffness.Area);
        SectD.Stiffness.Roz  = (SectD.Stiffness.Area==0.0) ? 0.0 : sqrt(SectD.Stiffness.Rzz/SectD.Stiffness.Area);

        SectD.Size[0] = SectOrgD.SectBefore.SectI.Size[0];
        SectD.Size[1] = SectOrgD.SectBefore.SectI.Size[1];
        SectD.Size[2] = SectOrgD.SectBefore.SectI.Size[2];
        SectD.Size[3] = SectOrgD.SectBefore.SectI.Size[3];
        SectD.Size[4] = SectOrgD.SectBefore.SectI.Size[4];
        SectD.Size[5] = SectOrgD.SectBefore.SectI.Size[5];
        SectD.Size[6] = SectOrgD.SectBefore.SectI.Size[6];
        SectD.Size[7] = SectOrgD.SectBefore.SectI.Size[7];

        int nPosi = 0;
        if(SectD.Shape==D_SECT_SHAPE_REG_GEN)
        {
            T_ELEM_D ElemD; ElemD.Initialize();
            T_SECT_D SectOrgD; SectOrgD.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return FALSE;
            int iSect = ElemD.elpro;	
            if(!m_pDgnDataCtrl->Get_DgnStlSect(ElemK, SectOrgD))	return FALSE;

            T_SECT_SECTBASE_D SectBaseD;	
            SectBaseD = (nPosi>0 && SectOrgD.nStype==D_SECT_TYPE_TAPERED) ? SectOrgD.SectBefore.SectJ : SectOrgD.SectBefore.SectI; 

            DGN_GSEC_POLYGON PolygonData;
            int iOutPolyNum = SectBaseD.aOuterPolygon.GetSize();
            SectD.aOuterPolygon.SetSize(iOutPolyNum);
            for(int i=0; i<iOutPolyNum; i++)
            {
                T_GSEC_POLYGON GsecPolygonD = SectBaseD.aOuterPolygon[i];
                ConvertToWgUMD_Polygon(GsecPolygonD, PolygonData);
                SectD.aOuterPolygon.SetAt(i, PolygonData);      
            }
            //
            DGN_GSEC_LINE UmdGsecLine;
            int GsecLineNum = SectBaseD.aLine.GetSize();
            SectD.aLine.SetSize(GsecLineNum);
            for(int i=0; i<GsecLineNum; i++)
            {
                T_GSEC_LINE GsecLineD = SectBaseD.aLine[i];
                ConvertToWgUMD_GsecLine(GsecLineD, UmdGsecLine);
                SectD.aLine.SetAt(i, UmdGsecLine);
            }
            //
            SectD.aLineLoop.Copy(SectBaseD.aLineLoop);
            //
            STL_KR_GSEC_LSHAP UmdGsecLshapD;
            int iGsecLshapNum = SectBaseD.aShape.GetSize();
            SectD.aShape.SetSize(iGsecLshapNum);
            for(int i=0; i<iGsecLshapNum; i++)
            {
                T_GSEC_LSHAP GsecLshapD = SectBaseD.aShape[i];
                ConvertToWgUMD_GsecShape(GsecLshapD, UmdGsecLshapD);
                SectD.aShape.SetAt(i, UmdGsecLshapD);
            }
            //
            for(int i=0; i<4; i++)
            {
                SectD.ShapeIndex[i] = SectBaseD.ShapeIndex[i];
                SectD.LdgnIndex[i] = SectBaseD.LdgnIndex[i];
            }
        }

        // Rail-Code 시 추가변수임.
        if(m_nDgnCode == STL_KSCE_RAIL_ASD04 || m_nDgnCode == STL_KSCE_RAIL_ASD11)
        {
            // Default. Gen Sect Type :: U-Section.
            SectD.iGenSectType  = 1;
            SectD.iShapeInfo    = 1;

            SectD.bRatio_Afg_Afn= TRUE;
            for(int i=0; i<5; i++)  SectD.dAf_gn_rat[i] = 1.0;
        }

        m_mRatingSect.SetAt(SectK, SectD);
    }

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::ConvertToWgUMD_GsecShape(T_GSEC_LSHAP& LshapeD, STL_KR_GSEC_LSHAP& rData)
{
    rData.sName = LshapeD.sName;
    rData.iShape = LshapeD.iShape;   // 0=Top flange, 1=Bottom flange, 2=Left web, 3=Right web, 4=Center web
    int iLdgnNum = LshapeD.aLdgn.GetSize();
    rData.aLdgn.SetSize(iLdgnNum);
    for(int i=0; i<iLdgnNum; i++)
    {
        T_GSEC_LDGN LdgnD = LshapeD.aLdgn[i];
        STL_KR_GSEC_LDGN UmdLdgnD;
        ConvertToWgUMD_GsecLdgn(LdgnD, UmdLdgnD);
        rData.aLdgn.SetAt(i, UmdLdgnD);    
    }

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::ConvertToWgUMD_GsecLdgn(T_GSEC_LDGN& LdgnD, STL_KR_GSEC_LDGN& rData)
{
    rData.sName     = LdgnD.sName;
    rData.iBoundary = LdgnD.iBoundary;
    rData.db        = LdgnD.db;
    rData.dt        = LdgnD.dt;
    rData.iPanNum   = LdgnD.iPanNum;
    rData.dy1       = LdgnD.dy1;
    rData.dz1       = LdgnD.dz1;
    rData.dy2       = LdgnD.dy2;
    rData.dz2       = LdgnD.dz2;

    int iLineNum = LdgnD.aLine.GetSize();
    rData.aLine.SetSize(iLineNum);
    for(int i=0; i<iLineNum; i++)
    {
        T_GSEC_LINE LineD = LdgnD.aLine[i];
        DGN_GSEC_LINE UmdLineD;
        ConvertToWgUMD_GsecLine(LineD, UmdLineD);
        rData.aLine.SetAt(i,UmdLineD);
    }
    rData.bExcept = LdgnD.bExcept;
    //rData.bStiffener = LdgnD.bStiffener; //[Check before Release...!!] BSC-20140314 이거 변수가 없어서 일단 주석..

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::ConvertToWgUMD_GsecLine(T_GSEC_LINE& LineD, DGN_GSEC_LINE& rData)
{
    rData.Initialize();
    rData.v1     = LineD.v1    ;
    rData.v2     = LineD.v2    ;
    rData.dThik  = LineD.dThik ;
    rData.nAlign = LineD.nAlign;
    rData.dA     = LineD.dA    ;
    rData.dVF    = LineD.dVF   ;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::ConvertToWgUMD_Polygon(T_GSEC_POLYGON& InData, DGN_GSEC_POLYGON& OutData)
{
    OutData.Initialize();
    int nSize = InData.aVertex.GetSize();
    OutData.aVertex.SetSize(nSize);
    DGN_GSEC_VERTEX    VertexUnit;
    for(int i=0 ; i<nSize ; i++)
    {
        VertexUnit.Set(InData.aVertex[i].dX, InData.aVertex[i].dY);
        OutData.aVertex.SetAt(i, VertexUnit);
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Make_UstlSMStrpData()
{
    m_mRatingStrp.RemoveAll();
    RATING_STRP_ELEM StrpElem;

    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        StrpElem.Initialize();
        T_STRP_SSM_D StrpD;
        StrpD.Initialize();
        T_ELEM_K ElemK = m_arElemK.GetAt(i).first;
        T_ELEM_D ElemD;
        if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return FALSE;
        if(!m_pDoc->m_pAttrCtrl2->GetStrpSsm(ElemD.elpro, StrpD))	continue;
        // nPosiDgn == 0 : I 단 , nPosiDgn == 1 : J 단
        for(int j=0; j<2; j++)
        {
            int nSize = StrpD.aPoint[j].GetSize();
            StrpElem.Strp[j].aPoint.SetSize(nSize);
            for(int i=0; i<nSize; i++)
            {
                StrpElem.Strp[j].aPoint[i].dPointY = StrpD.aPoint[j][i].dPointY;
                StrpElem.Strp[j].aPoint[i].dPointZ = StrpD.aPoint[j][i].dPointZ;
            }
        }

        // Set Rbar.
        m_mRatingStrp.SetAt(ElemK, StrpElem);
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Make_UstlSMGrpdData()
{
    m_mRatingGrpd.RemoveAll();
    RATING_GRPD_ELEM GrpdElem;

    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        GrpdElem.Initialize();
        T_GRPD_SSM_D GrpdD;
        GrpdD.Initialize();
        T_ELEM_K ElemK = m_arElemK.GetAt(i).first;
        T_ELEM_D ElemD;
        if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return FALSE;
        if(!m_pDoc->m_pAttrCtrl2->GetGrpdSsm(ElemD.elpro, GrpdD))	continue;
        // nPosiDgn == 0 : I 단 , nPosiDgn == 1 : J 단
        for(int j=0; j<2; j++)
        {
            int nSize = GrpdD.aPart[j].GetSize();
            GrpdElem.Grpd[j].aPart.SetSize(nSize);
            for(int i=0; i<nSize; i++)
            {
                GrpdElem.Grpd[j].aPart[i].nType       = GrpdD.aPart[j][i].nType;
                GrpdElem.Grpd[j].aPart[i].nGroupID    = GrpdD.aPart[j][i].nGroupID;
                GrpdElem.Grpd[j].aPart[i].strPartName = GrpdD.aPart[j][i].strPartName;
                GrpdElem.Grpd[j].aPart[i].nPartShape  = GrpdD.aPart[j][i].nPartShape;
                GrpdElem.Grpd[j].aPart[i].bStiffener  = GrpdD.aPart[j][i].bStiffener;
                GrpdElem.Grpd[j].aPart[i].aLine.Copy(GrpdD.aPart[j][i].aLine);
            }

            nSize = GrpdD.aGrpd[j].GetSize();
            GrpdElem.Grpd[j].aGrpd.SetSize(nSize);
            for(int i=0; i<nSize; i++)
            {
                GrpdElem.Grpd[j].aGrpd[i].nGroupID     = GrpdD.aGrpd[j][i].nGroupID;
                GrpdElem.Grpd[j].aGrpd[i].strGroupName = GrpdD.aGrpd[j][i].strGroupName;
                GrpdElem.Grpd[j].aGrpd[i].nGroupType   = GrpdD.aGrpd[j][i].nGroupType;
            }
        }

        // Set Rbar.
        m_mRatingGrpd.SetAt(ElemK, GrpdElem);
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Make_UstlSMStfnData()
{
    m_mRatingStfn.RemoveAll();
    RATING_STFN_ELEM StfnElem;

    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        StfnElem.Initialize();
        T_STFN_SSM_D StfnD;
        StfnD.Initialize();
        T_ELEM_K ElemK = m_arElemK.GetAt(i).first;
        T_ELEM_D ElemD;
        if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))	return FALSE;
        if(!m_pDoc->m_pAttrCtrl2->GetStfnSsm(ElemD.elpro, StfnD))	continue;
        // nPosiDgn == 0 : I 단 , nPosiDgn == 1 : J 단
        for(int j=0; j<2; j++)
        {
            int nSize = StfnD.aStiffener[j].GetSize();
            StfnElem.Stfn[j].aStiffener.SetSize(nSize);
            for(int i=0; i<nSize; i++)
            {
                StfnElem.Stfn[j].aStiffener[i].nType      = StfnD.aStiffener[j][i].nType;
                StfnElem.Stfn[j].aStiffener[i].nStandRef  = StfnD.aStiffener[j][i].nStandRef;
                StfnElem.Stfn[j].aStiffener[i].nPosonLine = StfnD.aStiffener[j][i].nPosonLine;
                StfnElem.Stfn[j].aStiffener[i].dStandRefL = StfnD.aStiffener[j][i].dStandRefL;
                StfnElem.Stfn[j].aStiffener[i].nNum       = StfnD.aStiffener[j][i].nNum;
                StfnElem.Stfn[j].aStiffener[i].dCTC       = StfnD.aStiffener[j][i].dCTC;
                StfnElem.Stfn[j].aStiffener[i].aLine.Copy(StfnD.aStiffener[j][i].aLine);
                StfnElem.Stfn[j].aStiffener[i].Stiffener.nStiffenerType = StfnD.aStiffener[j][i].Stiffener.nStiffenerType;

                for(int j=0; j<20; j++)
                {
                    StfnElem.Stfn[j].aStiffener[i].Stiffener.dSize[j] = StfnD.aStiffener[j][i].Stiffener.dSize[j];
                }
            }
        }
        // Set Rbar.
        m_mRatingStfn.SetAt(ElemK, StfnElem);
    }
    return TRUE;
}

//PMS:RATING BSC-20140329
BOOL CDgnStlRatingDataCtrl::Make_RatingProsData()
{
    m_mRatingRRos.RemoveAll();
    RATING_RROS_ELEM RrosElem;
    for(int i=0; i<m_arElemK.GetSize(); i++)
    {
        RrosElem.Initialize();
        T_RROS_D RrosD;
        RrosD.Initialize();
        T_ELEM_K ElemK = m_arElemK.GetAt(i).first;
        if(!m_pDoc->m_pAttrCtrl2->GetRros(ElemK,RrosD))	continue;
        // Set Rbar (0=I, 1=J).
        RrosElem.dDefI         = RrosD.dDefI;        
        RrosElem.dDefJ         = RrosD.dDefJ;     
        RrosElem.dStrainI      = RrosD.dStrainI;    
        RrosElem.dStrainJ      = RrosD.dStrainJ;   
        RrosElem.dImpactFactor  = RrosD.dImpactFactor;
        RrosElem.dImpactFactor_J= RrosD.dImpactFactor_J;
        RrosElem.dAddReponceF_I = RrosD.dAddReponceF_I;
        RrosElem.dAddReponceF_J = RrosD.dAddReponceF_J;

        // Set Rbar.
        m_mRatingRRos.SetAt(ElemK, RrosElem);
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Make_RatingDgnData()
{
    int nReturn=0;	
    // Set arElemK	
    m_arElemK.RemoveAll();
    ArrElemPairKey aElemPairK;
    if(m_pDataCtrl->Get_ElemListForBlrDgn_STL(TRUE, aElemPairK))
    {        
        int nElemSize = aElemPairK.GetSize();
        for(int i=0; i<nElemSize; ++i)
        {
            auto ElemK = aElemPairK.GetAt(i);

            BOOL bSaveElemK = TRUE;  // 검토 가능한 Steel 전체에 대해 Rf 계산
            ////if(m_nDgnCode!=STL_KSCE_ASD10 && m_nDgnCode!=STL_KSCE_RAIL_ASD11)
            //{
            //	bSaveElemK = m_pDoc->m_pAttrCtrl2->ExistRros(ElemK);			
            //}
            // Save ElemK.
            if(bSaveElemK)	m_arElemK.Add(ElemK);
        }
        if(m_arElemK.GetSize()==0)  m_arElemK.Copy(aElemPairK);
    }

    m_pCSGDataCtrl->Make_ElemSpanMapKey();
    m_pCSGDataCtrl->Make_ElemSpanMapKey_Sub();
    m_pCSGDataCtrl->Make_ElemSpanData();

    if(nReturn==0)  nReturn = (Make_RatingMatlDgnData() ? 0 : 1);  
    if(nReturn==0)  nReturn = (Make_RatingSectDgnData() ? 0 : 2);
    if(nReturn==0)  nReturn = (Make_UstlSMStrpData()    ? 0 : 3); 
    if(nReturn==0)  nReturn = (Make_UstlSMGrpdData()    ? 0 : 4); 
    if(nReturn==0)  nReturn = (Make_UstlSMStfnData()    ? 0 : 5); 
    if(nReturn==0)  nReturn = (Make_RatingProsData()    ? 0 : 6);

    if(nReturn==0) 
    {
        m_pCSGDataCtrl->SetChkCsgDgnElem(m_arElemK);
        if(!m_pCSGDataCtrl->Make_CSGDgnData4Rating()) return FALSE;
    }

    if(nReturn!=0)  return FALSE;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_KSCE_ASD_Rating()
{  
    CDesignLoadRating *pCheckRating = m_pDoc->m_pPostCtrl->GetLoadRating();
    if(!Make_RatingDgnData()) return FALSE;

    //Get Design Parameters
    T_RPAS_D RpasD;
    if (!m_pDoc->m_pAttrCtrl2->GetRpas(RpasD)) RpasD.Initialize();

    T_RFKR_CASE TensMinRfkrD;
    T_RFKR_CASE CompMinRfkrD;
    TensMinRfkrD.Initialize();
    CompMinRfkrD.Initialize();

    CSG_MEMB_RES_KSCE_LSD12_D MembRes;

    CSG_MEMB_RESULT_D      MembR;
    CSG_CHECK_RESULT_CODES ChkResD;

    m_pCSGDataCtrl->ClassNewMembResult(MembR);
    m_pCSGDataCtrl->ClassNewDgnResult(ChkResD);


    // 내하율 산정.
    for (int j=0; j<m_arElemK.GetSize(); ++j)
    {
        CSG_MEMB_POSD_KSCE_LSD MembLcomD;
        ConvertRatingGlobalData(RpasD, MembLcomD.Memb.Csgd);

        auto ElemK = m_arElemK[j];
        T_ELEM_D ElemD; ElemD.Initialize();

        if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) continue;

        m_pCSGDataCtrl->InitMembResult(MembR);

        // Design Position Setting
        BOOL bChkIJ[2]={FALSE};
        bChkIJ[0] = m_pDataCtrl->Get_ChkElemFlag4RatingKSCEASD(ElemK, 1);
        bChkIJ[1] = m_pDataCtrl->Get_ChkElemFlag4RatingKSCEASD(ElemK, 2);

        BOOL bIsCompoElem = IsCompoElem(ElemK.first);
        // Material       
        if (bIsCompoElem==TRUE)
        {
            MembRes.Initialize();
            if (!m_pCSGDataCtrl->m_mCsgMatl.Lookup(ElemD.elmat, MembLcomD.Memb.Matl)) MembLcomD.Memb.Matl.Initialize();

            const auto& itSect4E = m_pCSGDataCtrl->m_mCsgElemSect.find(ElemK);
            if (itSect4E != m_pCSGDataCtrl->m_mCsgElemSect.end())
            {
                const auto& itSect = m_pCSGDataCtrl->m_mCsgSect.find(itSect4E->second);
                if (itSect != m_pCSGDataCtrl->m_mCsgSect.end())
                {
                    MembLcomD.Memb.PosD[0].Sect = itSect->second.SectPos[0];
                    MembLcomD.Memb.PosD[1].Sect = itSect->second.SectPos[1];
                    //MembLcomD.Memb.PosD[0].bSupprotNode = bSupprotNode[0];
                    //MembLcomD.Memb.PosD[1].bSupprotNode = bSupprotNode[1];
                }
            }

            m_pCSGDataCtrl->Get_CsgMembInData(ElemK, MembLcomD);
            m_pCSGDataCtrl->MakeMemberResultData(ElemK, MembLcomD, MembR);
        }

        RATING_RROS_ELEM Rros;
        if (!m_mRatingRRos.Lookup(ElemK.first, Rros)) Rros.Initialize();

        for (int IJ = 0; IJ<2; ++IJ)
        {
            BOOL bCheckIJ = TRUE;
            if (IJ == 0 && bChkIJ[0] == FALSE) { bCheckIJ = FALSE;/*continue;*/ }
            if (IJ == 1 && bChkIJ[1] == FALSE) { bCheckIJ = FALSE;/*continue;*/ }

            if (m_nDgnCode==STL_KSCE_LSD15) {if (bCheckIJ == FALSE) continue;}
            // Initial Input & Output
            UMD_STL_KR_D InD;
            STL_KR_RES_D resD;      
            S_KR_LSD_RES_D LSDres;

            // Get Input Data...
            InD.ElemID   = ElemK.first;
            InD.nPosiDgn = (IJ == 0) ? 0 : 2;
            RATING_STRP_ELEM STRP;
            RATING_GRPD_ELEM GRPD;
            RATING_STFN_ELEM STFN;
            if (!m_mRatingMatl.Lookup(ElemD.elmat, InD.Matl)) { ASSERT(0); InD.Matl.Initialize(); }
            if (!m_mRatingSect.Lookup(ElemD.elpro, InD.Sect)) { ASSERT(0); InD.Sect.Initialize(); }
            if (!m_mRatingStrp.Lookup(ElemK.first, STRP)) STRP.Initialize();
            if (!m_mRatingGrpd.Lookup(ElemK.first, GRPD)) GRPD.Initialize();
            if (!m_mRatingStfn.Lookup(ElemK.first, STFN)) STFN.Initialize();

            InD.StrpSM = STRP.Strp[IJ];
            InD.GrpdSM = GRPD.Grpd[IJ];
            InD.StfnSM = STFN.Stfn[IJ];

            FORCE_LRKR_ELEM  Force;
            FORCE_LRKR_ELEM  Force_Str;
            STRESS_LRKR_ELEM Stress;
			const auto& itForce = m_pForcCtrl->m_arBlrdForceElemKR.find(ElemK);
            if (itForce != m_pForcCtrl->m_arBlrdForceElemKR.end()) Force = itForce->second;
            const auto& itForceStr = m_pForcCtrl->m_arBlrdForceElemKR_Str.find(ElemK);
            if (itForceStr != m_pForcCtrl->m_arBlrdForceElemKR_Str.end()) Force_Str = itForceStr->second;
            const auto& itStress = m_pForcCtrl->m_arBlrdStressElemKR.find(ElemK);
            if (itStress != m_pForcCtrl->m_arBlrdStressElemKR.end()) Stress = itStress->second;

            // InData Option
            if (bIsCompoElem==TRUE)
            {
                InD.Option.iSelDgnLcom = 2; // 0=Envelope 동시발생 부재력, 1=Max/Min, 2=All 하중조합별,
                InD.Option.iChkMethod  = 0; // 0=휨검토, 1=축력검토, 2=휨&축력검토
                InD.Option.iChkAxial   = 0;  // 검토 주축 선택 (0=y, 1=z, 2=y&z)
            }
            else
            {
                InD.Option.iSelDgnLcom = 2; // 0=Envelope 동시발생 부재력, 1=Max/Min, 2=All 하중조합별,
                InD.Option.iChkMethod  = 2; // 0=휨검토, 1=축력검토, 2=휨&축력검토
                InD.Option.iChkAxial   = 2;  // 검토 주축 선택 (0=y, 1=z, 2=y&z)
            }

            // Code Matl Code
            STL_KR_UMDC_STLOPTION InStlOpt;
            InStlOpt.strDgnCode  = CDBLib::GetStlRatCodeName(m_nDgnCode);
            InStlOpt.strMatlCode = MATLCODE_STL_KS_CIVIL;

            // K & Lb
            T_KFAC_D KFacD; KFacD.Initialize();
            BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(ElemK, KFacD);

            MembLcomD.Memb.ChIf.nChkPos = IJ;
            m_pCSGDataCtrl->Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);
            if (bIsCompoElem==TRUE)
            {				
                ConvertForce2Engine(Force, InD.Load, IJ); // test

                ConvertCsgForce2Engine(ElemK, Force, MembLcomD, IJ);        
                Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, InD.Option);
            }
            else
            {
                ConvertForce2Engine(Force, InD.Load, IJ);
                Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, InD.Option);
            }      

            InD.Option.bReducingFac = FALSE; 


            switch (m_nDgnCode)
            {
            case STL_KSCE_ASD10:
            case STL_KSCE_ASD05:
            case STL_KSCE_RAIL_ASD11:
            case STL_KSCE_RAIL_ASD04:
                {
                    Check_KSCE_ASD(RpasD, 
                        ElemK.first, IJ, bCheckIJ, 
                        MembLcomD,
                        InD,
                        InStlOpt,
                        Force,
                        Force_Str,
                        MembRes,
                        resD,
                        TensMinRfkrD,
                        CompMinRfkrD);      
                    break;
                }
            case STL_KSCE_LSD15:
                {
                    Check_KSCE_LSD(RpasD, 
                        ElemK.first, IJ, bCheckIJ, 
                        Rros,
                        MembLcomD,
                        InD,
                        InStlOpt,
                        Force,
                        Force_Str,
                        MembR,
                        LSDres,
                        TensMinRfkrD,
                        CompMinRfkrD);
                    break;
                }
            default:
                ASSERT(0);
                break;
            }

        }

        if (pCheckRating != NULL)
        {
            pCheckRating->WriteFlexuralStress4KSCEASDResult(ElemK,0, 1, 0,TensMinRfkrD);
            //pCheckRating->WriteFlexuralStress4KSCEASDResult(ElemK,0, 2, 0,CompMinRfkrD);    
            TensMinRfkrD.Initialize();
            CompMinRfkrD.Initialize();
        }
    }
    // 결과 STL_KR_RES_D

    if (MembR.pAASHTO != NULL) delete MembR.pAASHTO;
    if (MembR.pLSD    != NULL) delete MembR.pLSD;
    if (MembR.pCSA    != NULL) delete MembR.pCSA;

    m_pCSGDataCtrl->ClassDelDgnResult(ChkResD);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_KSCE_ASD(T_RPAS_D &RpasD,
                                           T_ELEM_K ElemK, int IJ, BOOL bCheckIJ, 
                                           CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                           UMD_STL_KR_D &InD,
                                           STL_KR_UMDC_STLOPTION &InStlOpt,
                                           FORCE_LRKR_ELEM  &Force,
                                           FORCE_LRKR_ELEM  &Force_Str,
                                           CSG_MEMB_RES_KSCE_LSD12_D &MembRes, 
                                           STL_KR_RES_D &resD,
                                           T_RFKR_CASE &TensMinRfkrD,
                                           T_RFKR_CASE &CompMinRfkrD)
{
    // Live Load Stress
    double dMaxStresTT = 0.0;     double dMaxStresTC = 0.0    ; double dMaxStresBT = 0.0    ; double dMaxStresBC = 0.0;
    double dMaxRatioTT = DBL_MAX; double dMaxRatioTC = DBL_MAX; double dMaxRatioBT = DBL_MAX; double dMaxRatioBC = DBL_MAX;
    int    nMaxIndexTT = -1;      int    nMaxIndexTC = -1;      int    nMaxIndexBT = -1;      int    nMaxIndexBC = -1;
    int    nLCBKeyTT = -1;        int    nLCBKeyTC = -1;        int    nLCBKeyBT = -1;        int    nLCBKeyBC = -1;
    double dMaxFlTT = 0.0,  dMaxFlTC = 0.0, dMaxFlBT = 0.0, dMaxFlBC = 0.0;
    double dMaxFdTT = 0.0,  dMaxFdTC = 0.0, dMaxFdBT = 0.0, dMaxFdBC = 0.0;
    double dMaxFgTT = 0.0,  dMaxFgTC = 0.0, dMaxFgBT = 0.0, dMaxFgBC = 0.0;
    double dImpFaTT = 0.0,  dImpFaTC = 0.0, dImpFaBT = 0.0, dImpFaBC = 0.0;
    CString strLcomTT = _T(""), strLcomTC= _T(""), strLcomBT= _T(""), strLcomBC= _T("");

    BOOL bIsCompoElem = IsCompoElem(ElemK);

    ElemPairK EPairK(ElemK, EN_EL_BEAM);

    if (bIsCompoElem==TRUE)
    {
        if (!m_pCSGDataCtrl->Check_Strength_ASD(ElemK, IJ, MembLcomD, MembRes, resD)) return FALSE;
    }
    else
    {
        if (!Check_Rating(InD, InStlOpt, resD)) return FALSE;
    }


    int nResSize = resD.arStdrData.GetSize();
    BOOL bSectGen = resD.StedBaseD.strSectNa == _T("GEN");

    for (int j = 0; j<nResSize; ++j)
    {
        int iOrgRcaseNo=0, iRatingCase=0, iRcaseTypeDL=0, iRcaseTypeLL=0;
        if(!m_pForcCtrl->Get_RcaseType(j+1, iOrgRcaseNo, iRatingCase, iRcaseTypeDL, iRcaseTypeLL)) ASSERT(0);
        // Impact Factor
        T_RLCS_D RlcsD;
        m_pDoc->m_pAttrCtrl2->GetRlcs(iOrgRcaseNo, RlcsD);
        int nMVLType = Force.arForceRCase[j].ForceLL.iRefNo[0];

        FORCE_RCASE_KR &ForceD    = Force.arForceRCase.GetAt(j);
        FORCE_RCASE_KR &ForceStrD = Force_Str.arForceRCase.GetAt(j);
        _DGN_FORC_CRC &ForceValDL = ForceD.ForceDL;
        _DGN_FORC_CRC &ForceValLL = ForceD.ForceLL;
        _DGN_FORC_CRC &ForceValG  = ForceStrD.ForceG;
        double dMDy = ForceValDL.dMuy[IJ];
        double dMLy = ForceValLL.dMuy[IJ];
        double dMGy = ForceValG.dMuy[IJ]*(1.0+RlcsD.dImpFactor);

        double dFlTT = 0.0; //Stress.arStressRCase[j].StressLL.dStop[IJ];
        double dFlTC = 0.0; //Stress.arStressRCase[j].StressLL.dStop[IJ];
        double dFlBT = 0.0; //Stress.arStressRCase[j].StressLL.dSbot[IJ];
        double dFlBC = 0.0; //Stress.arStressRCase[j].StressLL.dSbot[IJ];

        double dFdTT = 0.0; //Stress.arStressRCase[j].StressDL.dStop[IJ];
        double dFdTC = 0.0; //Stress.arStressRCase[j].StressDL.dStop[IJ];
        double dFdBT = 0.0; //Stress.arStressRCase[j].StressDL.dSbot[IJ];
        double dFdBC = 0.0; //Stress.arStressRCase[j].StressDL.dSbot[IJ];

        double dFgTT = 0.0;
        double dFgTC = 0.0;
        double dFgBT = 0.0;
        double dFgBC = 0.0;

        if (bIsCompoElem==TRUE)
        {
            CSG_SECT_POSD &SectD = MembLcomD.Memb.PosD[IJ].Sect;

            CalcStressCompo(dMLy, dMDy, dMGy, SectD, dFlTT, dFlTC, dFlBT, dFlBC, dFdTT, dFdTC, dFdBT, dFdBC, dFgTT, dFgTC, dFgBT, dFgBC);
        }
        else
        {
            CalcStress(dMLy, dMDy, dMGy, InD.Sect, dFlTT, dFlTC, dFlBT, dFlBC, dFdTT, dFdTC, dFdBT, dFdBC, dFgTT, dFgTC, dFgBT, dFgBC);
        }

        dFlTT = dFlTT > 0.0 ? dFlTT : 0.0;
        dFlTC = dFlTC < 0.0 ? dFlTC : 0.0;
        dFlBT = dFlBT > 0.0 ? dFlBT : 0.0;
        dFlBC = dFlBC < 0.0 ? dFlBC : 0.0;

        dFdTT = dFdTT > 0.0 ? dFdTT : 0.0;
        dFdTC = dFdTC < 0.0 ? dFdTC : 0.0;
        dFdBT = dFdBT > 0.0 ? dFdBT : 0.0;
        dFdBC = dFdBC < 0.0 ? dFdBC : 0.0;

        dFgTT = dFgTT > 0.0 ? dFgTT : 0.0;
        dFgTC = dFgTC < 0.0 ? dFgTC : 0.0;
        dFgBT = dFgBT > 0.0 ? dFgBT : 0.0;
        dFgBC = dFgBC < 0.0 ? dFgBC : 0.0;

        double dAlloTT = 0.0;
        double dAlloTC = 0.0;
        double dAlloBT = 0.0;
        double dAlloBC = 0.0;


        STL_KR_RES_STDR_BASE &Data = resD.arStdrData[j];
        double dAlwCF_T = bSectGen ? resD.arStdrData[j].dAlwFlexCompYT  : MinWithoutZero(Data.dAlwCF_TL, Data.dAlwCF_TR, Data.dAlwCF_T);
        double dAlwCF_B = bSectGen ? resD.arStdrData[j].dAlwFlexCompYB  : MinWithoutZero(Data.dAlwCF_BL, Data.dAlwCF_BR, Data.dAlwCF_B);
        double dAlwTF_T = bSectGen ? resD.arStdrData[j].dAlwTF_T : MinWithoutZero(Data.dAlwTF_TL, Data.dAlwTF_TR, Data.dAlwTF_T);
        double dAlwTF_B = bSectGen ? resD.arStdrData[j].dAlwTF_B : MinWithoutZero(Data.dAlwTF_BL, Data.dAlwTF_BR, Data.dAlwTF_B);

        //*************************************************************
        // * 휨 응력에 따라. 사하중 응력을 기준으로 함.
        //*************************************************************
        //*********************************************************************
        //TOP
        //*********************************************************************
        double dStressTL = resD.arStdrData[j].dStressTL;
        if (dFdTC < 0.0)
        {
            double dRFUpperTC = (dFlTC>=0.0)? DBL_MAX : ( fabs(dAlwCF_T) - fabs(dFdTC) ) / ( fabs(dFlTC) * (1.0 + RlcsD.dImpFactor) );
            if (dMaxRatioTC > dRFUpperTC  ) // (-) dMaxStresTC > resD.arStdrData[j].dStressTL
            {
                dMaxStresTC = resD.arStdrData[j].dStressTL;
                dMaxRatioTC = dRFUpperTC;
                nMaxIndexTC = j;
                dMaxFlTC    = dFlTC;
                dMaxFdTC    = dFdTC;
                dMaxFgTC    = dFgTC;
                dImpFaTC    = RlcsD.dImpFactor;
                strLcomTC.Format(_T("%s(%s)"), RlcsD.strCaseName, m_pDataCtrl->GetLcomType(nMVLType));
                nLCBKeyTC   = iOrgRcaseNo;
            }
        }

        if (dFdTT > 0.0)
        {
            double dRFUpperTT = (dFlTT<=0.0)? DBL_MAX  : ( fabs(dAlwTF_T) - fabs(dFdTT) ) / ( fabs(dFlTT) * (1.0 + RlcsD.dImpFactor) );
            if (dMaxRatioTT > dRFUpperTT  ) // (+) dMaxStresTT < resD.arStdrData[j].dStressTL
            { 
                dMaxStresTT = resD.arStdrData[j].dStressTL;
                dMaxRatioTT = dRFUpperTT;
                nMaxIndexTT = j;
                dMaxFlTT    = dFlTT;
                dMaxFdTT    = dFdTT;
                dMaxFgTT    = dFgTT;
                dImpFaTT    = RlcsD.dImpFactor;
                strLcomTT.Format(_T("%s(%s)"), RlcsD.strCaseName, m_pDataCtrl->GetLcomType(nMVLType));
                nLCBKeyTT   = iOrgRcaseNo;
            }
        }

        //*********************************************************************
        //Bottom
        //*********************************************************************
        double dStressBL = resD.arStdrData[j].dStressBL;
        if (dFdBC < 0.0)
        {
            double dRFUpperBC = (dFlBC>=0.0)? DBL_MAX : ( fabs(dAlwCF_B) - fabs(dFdBC) ) / ( fabs(dFlBC) * (1.0 + RlcsD.dImpFactor) );
            if ( dMaxRatioBC > dRFUpperBC )  // (-)  dMaxStresBC > resD.arStdrData[j].dStressBL
            {
                dMaxStresBC = resD.arStdrData[j].dStressBL;
                dMaxRatioBC = dRFUpperBC;
                nMaxIndexBC = j;
                dMaxFlBC    = dFlBC;
                dMaxFdBC    = dFdBC;
                dMaxFgBC    = dFgBC;
                dImpFaBC    = RlcsD.dImpFactor;
                strLcomBC.Format(_T("%s(%s)"), RlcsD.strCaseName, m_pDataCtrl->GetLcomType(nMVLType));
                nLCBKeyBC   = iOrgRcaseNo;
            }
        }

        if (dFdBT > 0.0)
        {
            double dRFUpperBT = (dFlBT<=0.0)? DBL_MAX : ( fabs(dAlwTF_B) - fabs(dFdBT) ) / ( fabs(dFlBT) * (1.0 + RlcsD.dImpFactor) );
            if ( dMaxRatioBT > dRFUpperBT ) //(+) // dMaxStresBT < resD.arStdrData[j].dStressBL/
            {
                dMaxStresBT = resD.arStdrData[j].dStressBL;
                dMaxRatioBT = dRFUpperBT;
                nMaxIndexBT = j;
                dMaxFlBT    = dFlBT;
                dMaxFdBT    = dFdBT;
                dMaxFgBT    = dFgBT;
                dImpFaBT    = RlcsD.dImpFactor;
                strLcomBT.Format(_T("%s(%s)"), RlcsD.strCaseName, m_pDataCtrl->GetLcomType(nMVLType));
                nLCBKeyBT   = iOrgRcaseNo;
            }
        }
    } // for nRessize

    RATING_RROS_ELEM Rros;
    if (!m_mRatingRRos.Lookup(ElemK, Rros)) Rros.Initialize();
    for(int k=0; k<2; k++) // 상연/ 하연
    {
        int nIdxTen = (k==0) ? nMaxIndexTT : nMaxIndexBT;
        int nIdxCom = (k==0) ? nMaxIndexTC : nMaxIndexBC;
        double dIdispZ=0.0, dJdispZ=0.0; 
        int nPosi = IJ * 2 + k;
        if (nIdxTen != -1) 
        {
            STL_KR_RES_STDR_BASE &Data = resD.arStdrData[nIdxTen];
            int nLcom = nIdxTen + 1;
            if (!m_pDataCtrl->Get_Deflection4NodeOfElem(EPairK, nLcom, dIdispZ, dJdispZ, ENUM_KSCE_ASD)) { ASSERT(0); };
            TensMinRfkrD.RfkrBase[nPosi].strLiveLoad    = RpasD.strDgnLiveLoad;
            TensMinRfkrD.RfkrBase[nPosi].bCheck         = bCheckIJ; //Data.bCheck;
            TensMinRfkrD.RfkrBase[nPosi].bCheck2        = TRUE;   // 전체 요소에 대해 RF계산함.
            TensMinRfkrD.RfkrBase[nPosi].ElemK          = ElemK;
            TensMinRfkrD.RfkrBase[nPosi].nRCaseK        = k==0 ? nLCBKeyTT : nLCBKeyBT;
            TensMinRfkrD.RfkrBase[nPosi].nDgnRCaseK     = Data.nDgnLcomK;
            TensMinRfkrD.RfkrBase[nPosi].nConcurrent    = Force.arForceRCase[nIdxTen].ForceLL.iRefNo[0];
            TensMinRfkrD.RfkrBase[nPosi].dfd            = (k==0) ? dMaxFdTT : dMaxFdBT;//Data.dStressTL : Data.dStressBL;
            TensMinRfkrD.RfkrBase[nPosi].dfl            = (k==0) ? dMaxFlTT : dMaxFlBT;
            TensMinRfkrD.RfkrBase[nPosi].dfa            = bSectGen ? ( (k==0) ? Data.dAlwTF_T : Data.dAlwTF_B ) : ( (k==0) ? MinWithoutZero(Data.dAlwTF_TL, Data.dAlwTF_TR, Data.dAlwTF_T) : MinWithoutZero(Data.dAlwTF_BL, Data.dAlwTF_BR, Data.dAlwTF_B) );

            TensMinRfkrD.RfkrBase[nPosi].dCalcDisp      = (IJ==0) ? fabs(dIdispZ) : fabs(dJdispZ);
            TensMinRfkrD.RfkrBase[nPosi].dEstiDisp      = (IJ==0) ? Rros.dDefI : Rros.dDefJ;
            TensMinRfkrD.RfkrBase[nPosi].dRatioDisp     = (TensMinRfkrD.RfkrBase[nPosi].dEstiDisp==0.0)? 0.0 : fabs(TensMinRfkrD.RfkrBase[nPosi].dCalcDisp)/TensMinRfkrD.RfkrBase[nPosi].dEstiDisp;

            TensMinRfkrD.RfkrBase[nPosi].dCalcEwip      = (k==0) ? dMaxFlTT / InD.Matl.StlMatL.Steel.Elast  : dMaxFlBT / InD.Matl.StlMatL.Steel.Elast;
            TensMinRfkrD.RfkrBase[nPosi].dEstiEwip      = (IJ==0) ? Rros.dStrainI * 0.001 : Rros.dStrainJ * 0.001;
            TensMinRfkrD.RfkrBase[nPosi].dRatioEwip     = (TensMinRfkrD.RfkrBase[nPosi].dEstiEwip==0.0)? 0.0 : TensMinRfkrD.RfkrBase[nPosi].dCalcEwip/TensMinRfkrD.RfkrBase[nPosi].dEstiEwip;

            double dImpactFactor =  (IJ==0) ? Rros.dImpactFactor : Rros.dImpactFactor_J;
            double dAddReponceF  =  (IJ==0) ? Rros.dAddReponceF_I : Rros.dAddReponceF_J; 
            TensMinRfkrD.RfkrBase[nPosi].dCalcImpFactor = (k==0) ? dImpFaTT : dImpFaBT;
            TensMinRfkrD.RfkrBase[nPosi].dEstiImpFactor = dImpactFactor;
            TensMinRfkrD.RfkrBase[nPosi].dRatioImpFactor= (1.0 + TensMinRfkrD.RfkrBase[nPosi].dCalcImpFactor) / (1.0 + dImpactFactor);
            TensMinRfkrD.RfkrBase[nPosi].d1iFactor      = (1.0 + TensMinRfkrD.RfkrBase[nPosi].dCalcImpFactor);
            TensMinRfkrD.RfkrBase[nPosi].dRF            = (k==0) ? dMaxRatioTT : dMaxRatioBT;
            if(bCheckIJ==TRUE)
            {
                TensMinRfkrD.RfkrBase[nPosi].dKs          = RpasD.nSurveyMeth == 0 ? (fabs(TensMinRfkrD.RfkrBase[nPosi].dCalcDisp) / TensMinRfkrD.RfkrBase[nPosi].dEstiDisp) * ( (1 + TensMinRfkrD.RfkrBase[nPosi].dCalcImpFactor) / (1 + TensMinRfkrD.RfkrBase[nPosi].dEstiImpFactor) ) 
                    : (fabs(TensMinRfkrD.RfkrBase[nPosi].dCalcEwip) / TensMinRfkrD.RfkrBase[nPosi].dEstiEwip) * ( (1 + TensMinRfkrD.RfkrBase[nPosi].dCalcImpFactor) / (1 + TensMinRfkrD.RfkrBase[nPosi].dEstiImpFactor) );

                TensMinRfkrD.RfkrBase[nPosi].dKs          *= dAddReponceF;
            }
            else
            {
                TensMinRfkrD.RfkrBase[nPosi].dKs          = 1.0;
            }

            TensMinRfkrD.RfkrBase[nPosi].dPr            = RpasD.dDgnLiveLoad;
            TensMinRfkrD.RfkrBase[nPosi].dP0            = TensMinRfkrD.RfkrBase[nPosi].dRF * RpasD.dDgnLiveLoad;
            TensMinRfkrD.RfkrBase[nPosi].dP             = TensMinRfkrD.RfkrBase[nPosi].dKs * TensMinRfkrD.RfkrBase[nPosi].dP0;
            TensMinRfkrD.RfkrBase[nPosi].nOK            = TensMinRfkrD.RfkrBase[nPosi].dPr < TensMinRfkrD.RfkrBase[nPosi].dP ? 1 : 2;
            ConvertRatForceEngToCvl(InD.Load.LcomList[nIdxTen], TensMinRfkrD.RfkrBase[nPosi].Load); // TensMinRfkrD.RfkrBase[nPosi].Load           = InD.Load.LcomList[nIdxTen];          
            TensMinRfkrD.RfkrBase[nPosi].Load.strName   = (k==0) ? strLcomTT : strLcomBT;
            ConvertRatForceEngToCvl(InD.Load.Lcom2List[nIdxTen], TensMinRfkrD.RfkrBase[nPosi].Load2); // TensMinRfkrD.RfkrBase[nPosi].Load2          = InD.Load.Lcom2List[nIdxTen];          
            TensMinRfkrD.RfkrBase[nPosi].Load2.strName  = (k==0) ? strLcomTT : strLcomBT;
            TensMinRfkrD.RfkrBase[nPosi].dfdl           = (k==0) ? dMaxFgTT : dMaxFgBT;//TensMinRfkrD.RfkrBase[nPosi].dfd + TensMinRfkrD.RfkrBase[nPosi].dfl;
            TensMinRfkrD.RfkrBase[nPosi].dRatiodfa      = TensMinRfkrD.RfkrBase[nPosi].dfa / fabs(TensMinRfkrD.RfkrBase[nPosi].dfdl);
            TensMinRfkrD.RfkrBase[nPosi].nRankType      = m_pDataCtrl->GetBridgeLevel(TensMinRfkrD.RfkrBase[nPosi].dRatiodfa, TensMinRfkrD.RfkrBase[nPosi].nOK);

            if (bIsCompoElem==TRUE)
            {
                m_pCSGDataCtrl->ConvertCSGEngToUlmForce(MembLcomD.aUlmf[nIdxTen], TensMinRfkrD.RfkrBase[nPosi].Ulmf);
                m_pCSGDataCtrl->ConvertCSGDCOMF(MembLcomD.aChkFor[nIdxTen], TensMinRfkrD.RfkrBase[nPosi].ChkFor);
            }
        }

        if (nIdxCom != -1) 
        {
            STL_KR_RES_STDR_BASE &Data = resD.arStdrData[nIdxCom];
            int nLcom = nIdxCom +1;
            if (!m_pDataCtrl->Get_Deflection4NodeOfElem(EPairK, nLcom, dIdispZ, dJdispZ, ENUM_KSCE_ASD)) { ASSERT(0); };
            CompMinRfkrD.RfkrBase[nPosi].strLiveLoad    = RpasD.strDgnLiveLoad;
            CompMinRfkrD.RfkrBase[nPosi].bCheck         = bCheckIJ; //Data.bCheck;
            CompMinRfkrD.RfkrBase[nPosi].bCheck2        = TRUE;  // 전체 요소에 대해 RF계산함.
            CompMinRfkrD.RfkrBase[nPosi].ElemK          = ElemK;
            CompMinRfkrD.RfkrBase[nPosi].nRCaseK        = k==0 ? nLCBKeyTC : nLCBKeyBC;
            CompMinRfkrD.RfkrBase[nPosi].nDgnRCaseK     = Data.nDgnLcomK;
            CompMinRfkrD.RfkrBase[nPosi].nConcurrent    = Force.arForceRCase[nIdxCom].ForceLL.iRefNo[0];
            CompMinRfkrD.RfkrBase[nPosi].dfd            = (k==0) ? dMaxFdTC : dMaxFdBC;// Data.dStressTL : Data.dStressBL;
            CompMinRfkrD.RfkrBase[nPosi].dfl            = (k==0) ? dMaxFlTC : dMaxFlBC;
            CompMinRfkrD.RfkrBase[nPosi].dfa            = bSectGen ? ( (k==0) ? Data.dAlwFlexCompYT : Data.dAlwFlexCompYB ) : ( (k==0) ? MinWithoutZero(Data.dAlwCF_TL, Data.dAlwCF_TR, Data.dAlwCF_T) : MinWithoutZero(Data.dAlwCF_BL, Data.dAlwCF_BR, Data.dAlwCF_B) );

            CompMinRfkrD.RfkrBase[nPosi].dCalcDisp      = (IJ==0) ? fabs(dIdispZ) : fabs(dJdispZ);
            CompMinRfkrD.RfkrBase[nPosi].dEstiDisp      = (IJ==0) ? Rros.dDefI : Rros.dDefJ;
            CompMinRfkrD.RfkrBase[nPosi].dRatioDisp     = (CompMinRfkrD.RfkrBase[nPosi].dEstiDisp==0.0)? 0.0 : fabs(CompMinRfkrD.RfkrBase[nPosi].dCalcDisp) / CompMinRfkrD.RfkrBase[nPosi].dEstiDisp;

            CompMinRfkrD.RfkrBase[nPosi].dCalcEwip      = (k==0) ? dMaxFlTC / InD.Matl.StlMatL.Steel.Elast  : dMaxFlBC / InD.Matl.StlMatL.Steel.Elast;
            CompMinRfkrD.RfkrBase[nPosi].dEstiEwip      = (IJ==0) ? Rros.dStrainI * 0.001 : Rros.dStrainJ * 0.001;
            CompMinRfkrD.RfkrBase[nPosi].dRatioEwip     = (CompMinRfkrD.RfkrBase[nPosi].dEstiEwip==0.0)? 0.0 : CompMinRfkrD.RfkrBase[nPosi].dCalcEwip/CompMinRfkrD.RfkrBase[nPosi].dEstiEwip;

            double dImpactFactor =  (IJ==0) ? Rros.dImpactFactor : Rros.dImpactFactor_J;
            double dAddReponceF  =  (IJ==0) ? Rros.dAddReponceF_I : Rros.dAddReponceF_J; 
            CompMinRfkrD.RfkrBase[nPosi].dCalcImpFactor = (k==0) ? dImpFaTC : dImpFaBC;
            CompMinRfkrD.RfkrBase[nPosi].dEstiImpFactor = dImpactFactor;
            CompMinRfkrD.RfkrBase[nPosi].dRatioImpFactor= (1.0 + CompMinRfkrD.RfkrBase[nPosi].dCalcImpFactor) / (1.0 + Rros.dImpactFactor);
            CompMinRfkrD.RfkrBase[nPosi].d1iFactor      = (1.0 + CompMinRfkrD.RfkrBase[nPosi].dCalcImpFactor);
            CompMinRfkrD.RfkrBase[nPosi].dRF            = (k==0) ? dMaxRatioTC : dMaxRatioBC;

            if(bCheckIJ==TRUE)
            {
                CompMinRfkrD.RfkrBase[nPosi].dKs          = RpasD.nSurveyMeth == 0 ? (fabs(CompMinRfkrD.RfkrBase[nPosi].dCalcDisp) / CompMinRfkrD.RfkrBase[nPosi].dEstiDisp) * ( (1 + CompMinRfkrD.RfkrBase[nPosi].dCalcImpFactor) / (1 + CompMinRfkrD.RfkrBase[nPosi].dEstiImpFactor) )
                    : (fabs(CompMinRfkrD.RfkrBase[nPosi].dCalcEwip) / CompMinRfkrD.RfkrBase[nPosi].dEstiEwip) * ( (1 + CompMinRfkrD.RfkrBase[nPosi].dCalcImpFactor) / (1 + CompMinRfkrD.RfkrBase[nPosi].dEstiImpFactor) );
                CompMinRfkrD.RfkrBase[nPosi].dKs         *= dAddReponceF;
            }
            else
            {
                CompMinRfkrD.RfkrBase[nPosi].dKs          = 1.0;
            }
            CompMinRfkrD.RfkrBase[nPosi].dPr            = RpasD.dDgnLiveLoad;
            CompMinRfkrD.RfkrBase[nPosi].dP0            = CompMinRfkrD.RfkrBase[nPosi].dRF * RpasD.dDgnLiveLoad;
            CompMinRfkrD.RfkrBase[nPosi].dP             = CompMinRfkrD.RfkrBase[nPosi].dKs * CompMinRfkrD.RfkrBase[nPosi].dP0;
            CompMinRfkrD.RfkrBase[nPosi].nOK            = CompMinRfkrD.RfkrBase[nPosi].dPr < CompMinRfkrD.RfkrBase[nPosi].dP ? 1 : 2;
            ConvertRatForceEngToCvl(InD.Load.LcomList[nIdxCom], CompMinRfkrD.RfkrBase[nPosi].Load); //CompMinRfkrD.RfkrBase[nPosi].Load           = InD.Load.LcomList[nIdxCom];
            CompMinRfkrD.RfkrBase[nPosi].Load.strName   = (k==0) ? strLcomTC : strLcomBC;
            ConvertRatForceEngToCvl(InD.Load.Lcom2List[nIdxCom], CompMinRfkrD.RfkrBase[nPosi].Load2); //CompMinRfkrD.RfkrBase[nPosi].Load2          = InD.Load.Lcom2List[nIdxCom];
            CompMinRfkrD.RfkrBase[nPosi].Load2.strName  = (k==0) ? strLcomTC : strLcomBC;
            CompMinRfkrD.RfkrBase[nPosi].dfdl           = (k==0) ? dMaxFgTC : dMaxFgBC;; //CompMinRfkrD.RfkrBase[nPosi].dfd + CompMinRfkrD.RfkrBase[nPosi].dfl;
            CompMinRfkrD.RfkrBase[nPosi].dRatiodfa      = CompMinRfkrD.RfkrBase[nPosi].dfa / fabs(CompMinRfkrD.RfkrBase[nPosi].dfdl);
            CompMinRfkrD.RfkrBase[nPosi].nRankType      = m_pDataCtrl->GetBridgeLevel(CompMinRfkrD.RfkrBase[nPosi].dRatiodfa, CompMinRfkrD.RfkrBase[nPosi].nOK);

            if (bIsCompoElem==TRUE)
            {
                m_pCSGDataCtrl->ConvertCSGEngToUlmForce(MembLcomD.aUlmf[nIdxCom], CompMinRfkrD.RfkrBase[nPosi].Ulmf);
                m_pCSGDataCtrl->ConvertCSGDCOMF(MembLcomD.aChkFor[nIdxCom], CompMinRfkrD.RfkrBase[nPosi].ChkFor);
            }
        }
    }

    //부재력 주어 담기
    int nSize = Force.arForceRCase.GetSize();
    if (nSize != 0 )
    {
        TensMinRfkrD.nForceSize = nSize;
        TensMinRfkrD.arForceRCaseI.SetSize(nSize);
        TensMinRfkrD.arForceRCaseJ.SetSize(nSize);
        CompMinRfkrD.nForceSize = nSize;
        CompMinRfkrD.arForceRCaseI.SetSize(nSize);
        CompMinRfkrD.arForceRCaseJ.SetSize(nSize);
        for (int i = 0; i<nSize; ++i)
        {
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, TensMinRfkrD.arForceRCaseI[i].ForceDL, 0);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, TensMinRfkrD.arForceRCaseI[i].ForceLL, 0);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, TensMinRfkrD.arForceRCaseJ[i].ForceDL, 1);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, TensMinRfkrD.arForceRCaseJ[i].ForceLL, 1);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, CompMinRfkrD.arForceRCaseI[i].ForceDL, 0);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, CompMinRfkrD.arForceRCaseI[i].ForceLL, 0);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, CompMinRfkrD.arForceRCaseJ[i].ForceDL, 1);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, CompMinRfkrD.arForceRCaseJ[i].ForceLL, 1);
        }
    }

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_KSCE_LSD(T_RPAS_D &RpasD,
                                           T_ELEM_K ElemK, int IJ, BOOL bCheckIJ,
                                           RATING_RROS_ELEM &Rros,
                                           CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                           UMD_STL_KR_D &InD,
                                           STL_KR_UMDC_STLOPTION &InStlOpt,
                                           FORCE_LRKR_ELEM  &Force,
                                           FORCE_LRKR_ELEM  &Force_Str,
                                           CSG_MEMB_RESULT_D &MembRes, 
                                           S_KR_LSD_RES_D &resD,
                                           T_RFKR_CASE &TensMinRfkrD,
                                           T_RFKR_CASE &CompMinRfkrD)
{
    BOOL bCurved = MembLcomD.Memb.ChIf.bCurved;

    UINT nIdx = IJ*2;

    BOOL bIsCompoElem = IsCompoElem(ElemK);

    ElemPairK EPairK(ElemK, EN_EL_BEAM);

    double dRFPmin = DBL_MAX;
    double dRFNmin = DBL_MAX;
    CSG_MEMB_POSD_KSCE_LSD MembLC = MembLcomD;
    UMD_STL_KR_D NCInD = InD;
    for (INT_PTR i=0; i<Force.arForceRCase.GetSize(); ++i)
    {
        int nDgnRcase = i+1;
        int iOrgRcaseNo=0, iRatingCase=0, iRcaseTypeDL=0, iRcaseTypeLL=0;
        if(!m_pForcCtrl->Get_RcaseType(nDgnRcase, iOrgRcaseNo, iRatingCase, iRcaseTypeDL, iRcaseTypeLL)) ASSERT(0);
        // Impact Factor
        T_RLCS_D RlcsD;
        m_pDoc->m_pAttrCtrl2->GetRlcs(iOrgRcaseNo, RlcsD);
        int nMVLType = Force.arForceRCase[i].ForceLL.iRefNo[0];

        CString strLcom; 
        strLcom.Format(_T("%s(%s)"), RlcsD.strCaseName, m_pDataCtrl->GetLcomType(nMVLType));

        T_RFKR_BASE RFacB; RFacB.Initialize();
        RFacB.nRCaseK        = iOrgRcaseNo;
        RFacB.nDgnRCaseK     = nDgnRcase;
        ConvertRatForceEngToCvl(InD.Load.LcomList[i], RFacB.Load);    
        RFacB.Load.strName   = strLcom;
        ConvertRatForceEngToCvl(InD.Load.Lcom2List[i], RFacB.Load2);    
        RFacB.Load2.strName  = strLcom;

        MembLC.aUlmf.RemoveAll();
        MembLC.aChkFor.RemoveAll();

        FORCE_LRKR_ELEM ForLRT;
        ForLRT.arForceRCase.Add(Force.arForceRCase[i]);

        //int nMVLType = Force.arForceRCase[i].ForceLL.iRefNo[0];

        double dMu_mv = 0.0;
        if (bIsCompoElem) 
        {
            ConvertCsgForce2Engine(EPairK, ForLRT, MembLC, IJ);

            CheckStrengthRcaseLSD15(EPairK, RpasD, Rros, IJ, bCurved, MembLC, InD, MembRes, RlcsD, Force.arForceRCase[i], Force_Str.arForceRCase[i], RFacB);

            dMu_mv = MembLC.aChkFor[0].ForMV.dForce[4];
        }
        else
        {      
            NCInD.Load.Initialize();
            ConvertForce2Engine(ForLRT, NCInD.Load, IJ);
            CheckNonCptStrengthRcaseLSD15(EPairK, RpasD, Rros, IJ, bCurved, MembLC, NCInD, InStlOpt, MembRes, resD, RlcsD, Force.arForceRCase[i], Force_Str.arForceRCase[i], RFacB);

            dMu_mv = ForLRT.arForceRCase[0].ForceLL.dMuy[IJ];
        }

        BOOL   bPosiM = dMu_mv>=0.0;
        if (bCurved==FALSE) // KSCE-LSD15
        {
            if (bPosiM)
            {
                if (RFacB.dRF<dRFPmin)
                {
                    TensMinRfkrD.RfkrBase[nIdx] = RFacB;
                }
            }
            else
            {
                if (RFacB.dRF<dRFNmin)
                {
                    TensMinRfkrD.RfkrBase[nIdx+1] = RFacB;
                }
            }
        }
        else // KSSC
        {
            if (bPosiM)
            {
                if (RFacB.dRF<dRFPmin)
                {
                    TensMinRfkrD.RfkrBase[nIdx] = RFacB;
                }
            }
            else
            {
                if (RFacB.dRF<dRFNmin)
                {
                    TensMinRfkrD.RfkrBase[nIdx+1] = RFacB;
                }
            }
        }
    }

    for (int k=0; k<2; ++k)
    {
        Calc_RatingData(EPairK, RpasD, Rros, IJ, TensMinRfkrD.RfkrBase[nIdx+k]);
    }

    //부재력 주어 담기
    int nSize = Force.arForceRCase.GetSize();
    if (nSize != 0 )
    {
        TensMinRfkrD.nForceSize = nSize;
        TensMinRfkrD.arForceRCaseI.SetSize(nSize);
        TensMinRfkrD.arForceRCaseJ.SetSize(nSize);
        //     CompMinRfkrD.nForceSize = nSize;
        //     CompMinRfkrD.arForceRCaseI.SetSize(nSize);
        //     CompMinRfkrD.arForceRCaseJ.SetSize(nSize);
        for (int i = 0; i<nSize; ++i)
        {
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, TensMinRfkrD.arForceRCaseI[i].ForceDL, 0);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, TensMinRfkrD.arForceRCaseI[i].ForceLL, 0);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, TensMinRfkrD.arForceRCaseJ[i].ForceDL, 1);
            m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, TensMinRfkrD.arForceRCaseJ[i].ForceLL, 1);
            //       m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, CompMinRfkrD.arForceRCaseI[i].ForceDL, 0);
            //       m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, CompMinRfkrD.arForceRCaseI[i].ForceLL, 0);
            //       m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceDL, CompMinRfkrD.arForceRCaseJ[i].ForceDL, 1);
            //       m_pDataCtrl->ConvertForce2ResStr(Force.arForceRCase[i].ForceLL, CompMinRfkrD.arForceRCaseJ[i].ForceLL, 1);
        }
    }

    return TRUE;
}


BOOL CDgnStlRatingDataCtrl::CalcStressCompo(double dMLy, double dMDy,  double dMGy, CSG_SECT_POSD &SectD, double& dFlTT, double& dFlTC, double& dFlBT, double& dFlBC, double& dFdTT, double& dFdTC, double& dFdBT, double& dFdBC,
                                            double &dFgTT, double &dFgTC, double &dFgBT, double &dFgBC)
{


    DGN_SECT_STIFFNESS& Stiffness = SectD.RbarStiffn;

    double dAreaD = Stiffness.dArea;
    double dRyyD  = Stiffness.dIyy;

    double dz1D   = Stiffness.dy1;
    double dz2D   = Stiffness.dy2;
    double dz3D   = Stiffness.dy3;
    double dz4D   = Stiffness.dy4;

    // 압축 -, 인장 +
    double dFlTstress = (-1.)*dMLy * dz1D/dRyyD + (-1.)*dMLy * dz2D/dRyyD;
    dFlTstress /=2.0;

    double dFlBstress = (-1.)*dMLy * dz3D/dRyyD + (-1.)*dMLy * dz4D/dRyyD;
    dFlBstress /=2.0;

    double dFdTstress = (-1.)*dMDy * dz1D/dRyyD + (-1.)*dMDy * dz2D/dRyyD;
    dFdTstress /=2.0;

    double dFdBstress = (-1.)*dMDy * dz3D/dRyyD + (-1.)*dMDy * dz4D/dRyyD;
    dFdBstress /=2.0;

    double dFgTstress = (-1.)*dMGy * dz1D/dRyyD + (-1.)*dMGy * dz2D/dRyyD;
    dFgTstress /=2.0;

    double dFgBstress = (-1.)*dMGy * dz3D/dRyyD + (-1.)*dMGy * dz4D/dRyyD;
    dFgBstress /=2.0;

    dFlTT = dFlTstress;
    dFlTC = dFlTstress; 
    dFlBT = dFlBstress;
    dFlBC = dFlBstress;

    dFdTT = dFdTstress;
    dFdTC = dFdTstress;
    dFdBT = dFdBstress;
    dFdBC = dFdBstress;

    dFgTT = dFgTstress;
    dFgTC = dFgTstress;
    dFgBT = dFgBstress;
    dFgBC = dFgBstress;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CalcStress(double dMLy, double dMDy,  double dMGy, STL_KR_SECT& SectD, double& dFlTT, double& dFlTC, double& dFlBT, double& dFlBC, double& dFdTT, double& dFdTC, double& dFdBT, double& dFdBC,
                                       double &dFgTT, double &dFgTC, double &dFgBT, double &dFgBC)
{

    STL_KR_SECT_STIFFNESS& Stiffness = SectD.Stiffness;
    double dAreaD = Stiffness.Area;
    double dRyyD  = Stiffness.Ryy;
    double dz1D   = Stiffness.dy1;
    double dz2D   = Stiffness.dy2;
    double dz3D   = Stiffness.dy3;
    double dz4D   = Stiffness.dy4;

    // 압축 -, 인장 +
    double dFlTstress = (-1.)*dMLy * dz1D/dRyyD + (-1.)*dMLy * dz2D/dRyyD;
    dFlTstress /=2.0;

    double dFlBstress = (-1.)*dMLy * dz3D/dRyyD + (-1.)*dMLy * dz4D/dRyyD;
    dFlBstress /=2.0;

    double dFdTstress = (-1.)*dMDy * dz1D/dRyyD + (-1.)*dMDy * dz2D/dRyyD;
    dFdTstress /=2.0;

    double dFdBstress = (-1.)*dMDy * dz3D/dRyyD + (-1.)*dMDy * dz4D/dRyyD;
    dFdBstress /=2.0;

    double dFgTstress = (-1.)*dMGy * dz1D/dRyyD + (-1.)*dMGy * dz2D/dRyyD;
    dFgTstress /=2.0;

    double dFgBstress = (-1.)*dMGy * dz3D/dRyyD + (-1.)*dMGy * dz4D/dRyyD;
    dFgBstress /=2.0;

    dFlTT = dFlTstress;
    dFlTC = dFlTstress; 
    dFlBT = dFlBstress;
    dFlBC = dFlBstress;

    dFdTT = dFdTstress;
    dFdTC = dFdTstress;
    dFdBT = dFdBstress;
    dFdBC = dFdBstress;

    dFgTT = dFgTstress;
    dFgTC = dFgTstress;
    dFgBT = dFgBstress;
    dFgBC = dFgBstress;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_Rating(UMD_STL_KR_D &InD, STL_KR_UMDC_STLOPTION &OptD, STL_KR_RES_D &rData)
{ 
    if (!m_pRatingManager->Check_MemberRes_KSCE_ASD_Rating(InD, OptD, rData)) return FALSE;
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_Rating(UMD_STL_KR_D &InD, STL_KR_UMDC_STLOPTION &OptD, S_KR_LSD_RES_D &rData)
{ 
    if (!m_pRatingManager->Check_MemberRes_KSCE_LSD_Rating(InD, OptD, rData)) return FALSE;
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_AASHTO_LRFR()
{
    // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
    //int iDgnStatus = m_pDoc->GetDesignStatus();
    //m_pDoc->SetDesignStatus(5);

    Make_RatingDgnData();

    CArray<T_RLCS_K,T_RLCS_K> aRlcsKey;
    m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(aRlcsKey);
    int nRlcsKeySize = aRlcsKey.GetSize();

    //CArray<T_ELEM_K,T_ELEM_K> arCheckElemK; 
    //arCheckElemK.Copy(m_arElemK);
    int nTotalElement = m_arElemK.GetSize();

    T_RPAS_D RpasD; RpasD.Initialize();  
    m_pDoc->m_pAttrCtrl2->GetRpas(RpasD);
    m_pRpasD = &RpasD;

    double dPhi_SF = RpasD.dSystemFactor;

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);  

    int i=0, j=0, k=0, m=0, n=0;
    FORCE_BLRD_ELEM ForceBlrdElem;
    FORCE_BLRD_ELEM ForcePreCompo;
    BLR_SPAN_D BlrSpanD;

    //double dAlwCompStress = RpasD.dAlwCompStress;    
    //double dAlwTensStress = RpasD.dAlwTensStress;

    m_pDataCtrl->Calc_LengthPosition4Span(m_arElemK);

    // Pre-Combined Composite Bridge
    BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb(); 

    CSG_MEMB_RESULT_D      MembR;
    CSG_CHECK_RESULT_CODES ChkResD;

    MembR.pAASHTO = new CSG_MEMBPOS_RES_AASHTO_LRFD_D;

    ChkResD.pAASHTO = new CSG_MEMB_RES_AASHTO_LRFD_D;

    //CSG_MEMBPOS_RES_AASHTO_LRFD_D MembResAASHTO;

    DgnBeamPairK nCurUblmK(EN_EL_BEAM, 0);
    ArrElemPairKey aUblmElemK;
    BOOL bCantilever=FALSE;
    T_RROS_D RrosD;
    for(i=0; i<nTotalElement; ++i)
    {
        CSG_MEMB_POSD_KSCE_LSD MembLcomD;
        ConvertRatingGlobalData(RpasD, MembLcomD.Memb.Csgd);

        int nMaxMin=0;
        auto CheckElemK = m_arElemK[i];
        RrosD.Initialize();
        if (CheckElemK.second == EN_EL_BEAM)
        {
            m_bbAppAFK = m_pDoc->m_pAttrCtrl2->GetRros(CheckElemK.first, RrosD); // Adjustment Factor, K 적용 여부 
        }
        else if (CheckElemK.second == EN_EL_VBEAM)
        {
            m_bbAppAFK = m_pDoc->m_pAttrCtrl2->GetRrov(CheckElemK.first, RrosD);
        }
        else ASSERT(0);
        
        m_bAutoAFK = (RrosD.nCalcType==0);

        m_dKb = m_bbAppAFK ? RrosD.dImpactFactor : 1.0; 
        MembLcomD.Memb.ChIf.bCantilever = bCantilever;

        BlrSpanD.Initialize();
        BOOL bSpanInfo=FALSE;
        if(m_pDataCtrl->Get_ElemSpanInfo(CheckElemK, BlrSpanD))
        {
            bSpanInfo = TRUE;
        }

        T_STAG_K nGdLastStagK=0;
        int nLastStepNo=0;
        m_pForcCtrl->GetGirderLastStageKey(CheckElemK, nGdLastStagK, nLastStepNo);
        int nForceBeforeType = 1;

        T_ELEM_WEIGHT ElemWgt;
        CSelfWgt SelfWgt(m_pDoc);
        SelfWgt.GetElemWeight(CheckElemK.first, ElemWgt);
        m_dWeight = ElemWgt.dTotalWeight;

        T_GRUP_K CheckGroupK = 0; //rKeyList[0]; //arGrupList[0];
        if(!m_pDataCtrl->GetBlrCheckGroupKey(CheckElemK, CheckGroupK)) return FALSE;

        int nNode[2] ={0, 0};
        BOOL bSupprotNode[2] ={FALSE, FALSE};
        m_pCSGDataCtrl->CheckSupprotNodeElem(CheckElemK,  nNode);
        bSupprotNode[0]  = nNode[0];
        bSupprotNode[1]  = nNode[1];
        bool bInteriorSupport[2] = { false, false };
        m_pCSGDataCtrl->CheckInteriorSupportNodeElem(CheckElemK, bInteriorSupport);

        MembR.pAASHTO->Initialize();

        int nMatlK = CDBLib::GetMaterialKey4ElemPairK(CheckElemK);
        if (!m_pCSGDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl)) MembLcomD.Memb.Matl.Initialize();

        double dVuFactor = 1.0;
        m_pCSGDataCtrl->GetCsgSectionSupport(CheckElemK, MembLcomD);
        if (IsSectTypeWith2Web(MembLcomD.Memb.PosD[0].Sect.SectInfo.nStype))
        {
            dVuFactor = 0.5;
        }            

        m_pCSGDataCtrl->Get_CsgMembInData(CheckElemK, MembLcomD);

        MembLcomD.Memb.ChIf.nChkPos = 0;
        //MembLcomD.Memb.ChIf.bCurved = bCurved;
        m_pCSGDataCtrl->MakeMemberResultData(CheckElemK, MembLcomD, MembR);   

        T_SPAN_K CurSpanK=0;
        ElemPairK nElemI=CheckElemK, nElemM=CheckElemK, nElemJ=CheckElemK;
        int nPosI=0;
        int nPosM=0; // 임시, 찾아야함. 
        int nPosJ=1;

        BOOL bChkUblm=FALSE;
        BOOL bCantilever=FALSE;
        BOOL bMakeMembFor=FALSE;
        BOOL b2SideBgg=FALSE;
        DgnBeamPairK nUblmK(EN_EL_BEAM, 0);

        m_pDataCtrl->GetUbLengthElemInfo(CheckElemK, nUblmK, nCurUblmK, nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, bChkUblm, bCantilever, bMakeMembFor, b2SideBgg, aUblmElemK);
        MembLcomD.bChkUlmf = true;

        T_KFAC_D KFacD; KFacD.Initialize();
        BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(CheckElemK, KFacD);
        m_pCSGDataCtrl->Get_UnbracedLengthData(CheckElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);
        //
        m_pCSGDataCtrl->Get_CsgUlmSection(nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);

        //MembLcomD.Memb.ChIf.dFactorCS = dFactorDeadCS;
        MembLcomD.Memb.ChIf.bCantilever = bCantilever;


        // 시공중 거더 Force

        _DGN_FORC_CRC ForceNC;

        int nLcomNum = m_pForcCtrl->Get_LcomDataCount();

        ADGNFORCE aPreForceLcom;
        if (bIsPLCB==TRUE)
        {
            //m_pForcCtrl->Get_BeforeForce4PreCompo(CheckElemK, aPreForceLcom, nLcomNum);
            const auto& iter = m_pForcCtrl->m_arBlrdPreCompoForecElem.find(CheckElemK);
			if (iter != m_pForcCtrl->m_arBlrdPreCompoForecElem.end())
                ForcePreCompo = iter->second;
        }

        int nPreForceLcomSize = ForcePreCompo.arForceRCase.GetSize();

        T_RSGS_D RsgsD; RsgsD.Initialize();
        m_pDoc->m_pAttrCtrl2->GetRsgs(CheckGroupK,RsgsD);  
        int    nChkPosition = RsgsD.iChkPosition;         // 0=I 단, 1=J 단, 2=I,J 단
        double dPhi_CF      = RsgsD.dConditionFactor;    //   

        ForceBlrdElem.Initialize();
		const auto& itForceElem = m_pForcCtrl->m_arBlrdForceElem.find(CheckElemK);
        if(itForceElem == m_pForcCtrl->m_arBlrdForceElem.end()) continue;
		ForceBlrdElem = itForceElem->second;

        CArray<FORCE_BLRD_RCASE,FORCE_BLRD_RCASE> arForceRCase;
        arForceRCase.RemoveAll();
        arForceRCase.Copy(ForceBlrdElem.arForceRCase);

        // Condition factor, System factor.
        // Ref. Load and Resistance Factor Rating. 6.4.2 General Load-Rating Equation
        // Where thr following lower limit shall apply : phi_c*phi_s >= 0.85
        double dCapacityF=dPhi_CF*dPhi_SF;
        if(dCapacityF<0.85) dCapacityF = 0.85;

        double dCapacityFsh = 1.0 * dPhi_CF; // Shear system factor는 항상 1.0으로 적용, AASHTO LRFR 11,6A.4.2.4 PSC(14.07.10) by shishir 
        if(dCapacityFsh < 0.85) dCapacityFsh = 0.85;

        double dCapaN_Fati=0.0;

        double dMaxValue = 1.0/pow(m_dZero, 10);

        double dPositive_RF[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dNegative_RF[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dPositive_CADE[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dNegative_CADE[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        double dVn_RF[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dVn_CADE[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        double dCompI_RF = dMaxValue;
        double dCompJ_RF = dMaxValue;
        double dTensI_RF = dMaxValue;
        double dTensJ_RF = dMaxValue;
        double dCompI_CADE = dMaxValue;
        double dCompJ_CADE = dMaxValue;
        double dTensI_CADE = dMaxValue;
        double dTensJ_CADE = dMaxValue;

        double dFTopI_RF = dMaxValue;
        double dFBotI_RF = dMaxValue;
        double dFTopJ_RF = dMaxValue;
        double dFBotJ_RF = dMaxValue;
        double dFTopI_CADE = dMaxValue;
        double dFBotI_CADE = dMaxValue;
        double dFTopJ_CADE = dMaxValue;
        double dFBotJ_CADE = dMaxValue;

        // Critical Rating case
        T_RCFC_D RcfcD; // Flexure
        T_RCSC_D RcscD; // Shear
        T_RCGS_D RcgsD; // Service
        T_RCFA_D RcfaD; // Fatigue

        int nFatiRCaseCount=0;
        //T_RCST_CASE PosiMinRcstD;
        //T_RCST_CASE NegaMinRcstD;

        int nConcurr=0;
        BOOL bDC_Max, bDW_Max, bTemp_Max;
        for(j=0; j<nRlcsKeySize; ++j)
        {
            T_RLCS_K RlcsK = aRlcsKey[j];
            T_RLCS_D RlcsD; RlcsD.Initialize();
            m_pDoc->m_pAttrCtrl2->GetRlcs(RlcsK, RlcsD);
            //if(BlrcD.iLimitState==0) continue;
            //ChangeTemperatureFactor(BlrcD);

            T_LCOM_D LcomDe;
            double dDispI=0.0, dDispJ=0.0;
            if (m_bbAppAFK && m_bAutoAFK) 
            {
                //MakeLcomData4RCase(RlcsD, bDC_Max, bDW_Max, bTemp_Max, LcomDe);
                Get_DeflectionOfNode(CheckElemK.first, RlcsD, LcomDe, dDispI, dDispJ);
            }      

            FORCE_BLRD_RCASE &ForceBlrdRcase = arForceRCase[j];              
            if (RlcsD.nLimitState==1 || RlcsD.nLimitState==0)
            {
                T_RLCS_KEY auRlcskey[96];
                m_pDoc->m_pPostCtrl->GetStlRatingLcomList(RlcsK, RlcsD, RlcsD.nLimitState, auRlcskey);

                MembLcomD.aChkFor.RemoveAll();
                MembLcomD.aUlmf.RemoveAll();
                int nChkLcomSize=1;
                int nChkLcomSizeUlmf = 6 ;
                MembLcomD.aUlmf.SetSize(nChkLcomSizeUlmf);
                MembLcomD.aChkFor.SetSize(nChkLcomSize);

                for(k=0; k<cRAT_LCASE; k++)
                {
                    m_pDoc->Get_BlrcLoadTypeData(k, bDC_Max, bDW_Max, bTemp_Max);

                    if (IsSkipRatingCase(bDC_Max, bDW_Max, bTemp_Max, RlcsD)==FALSE) continue;

                    double dFactorDL = bDC_Max ? RlcsD.RateLoad[0].dFactor[0] : RlcsD.RateLoad[0].dFactor[1];
                    if (bIsPLCB==TRUE)
                    {
                        if (j<nPreForceLcomSize)            
                        {
                            int nInx = bDC_Max ? 0 : 1;
                            ForcePreCompo.arForceRCase[j].ForceDCBe[nInx].GetForce(ForceNC);
                        }
                    }

                    T_STRB_D MembForNC;
                    for(m=0; m<cFORC_CON_DGN; m++)
                    {
                        int iIdx = k*12 + m;
                        T_RLCS_KEY uRlcskey = auRlcskey[iIdx];
                        if(uRlcskey == 0) continue;

                        nConcurr=m+1;

                        MembLcomD.aChkFor[0].Initialize();
                        CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];
                        if      (RlcsD.nLimitState==1) ComLcomF.nLcomType = 0;
                        else if (RlcsD.nLimitState==0) // Service
                        {
                            ComLcomF.nLcomType = 1;
                            //ComLcomF.nLcomSub  = nServLcomType;
                        }      
                        ComLcomF.nMax = nConcurr;

                        int nRCDgnLcomNo = m_pForcCtrl->GetStlRCDgnLcomKey(cRAT_LCASE, cFORC_CON_DGN, j, k, m);
                        if (bIsPLCB==FALSE) m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo,  nRCDgnLcomNo, nForceBeforeType, ForceNC, dFactorDL);

                        MembForNC.Initialize();
                        m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, aUblmElemK, &MembForNC, dFactorDL);

                        // UlmForce
                        MakeConverMemberForce(StctD, j, nGdLastStagK, nLastStepNo, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurr, aUblmElemK, MembForNC, MembLcomD);

                        //T_RLCS_KEY uRlcskey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr);
                        T_RCST_CASE RcstC;
                        T_RCSS_CASE RcssC;
                        for(n=0; n<2; n++)
                        {

                            if     (n==0 && nChkPosition==1) continue;
                            else if(n==1 && nChkPosition==0) continue;

                            MembLcomD.Memb.ChIf.nChkPos = n;

                            ConvertRatStlCalcChkForce(n, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurr, ForceNC, ForceBlrdRcase, ComLcomF);

                            m_dEpsiC = 0.0;              
                            m_dEpsiT = (n==0) ? RrosD.dStrainI : RrosD.dStrainJ;

                            m_dDispC = (n==0) ? dDispI : dDispJ;
                            m_dDispT = (n==0) ? RrosD.dDefI : RrosD.dDefJ;

                            m_dKa = 0.0;
                            m_dK  = 1.0;              

                            int nCCId = nConcurr-1;
                            if (m_bbAppAFK && m_bAutoAFK)
                            {
                                CSG_MEMBPOS_RES_AASHTO_LRFD_D StressMembRes = *MembR.pAASHTO;
                                Calc_Stress4MVpr(CheckElemK, n, MembLcomD, StressMembRes, m_dEpsiC);
                                double dRepCalcVal = (m_pRpasD->nMeasurement==0) ? m_dEpsiC : m_dDispC;
                                double dRepTestVal = (m_pRpasD->nMeasurement==0) ? m_dEpsiT : m_dDispT;
                                Calc_AdjustmentFactorK(dRepCalcVal, dRepTestVal, m_dKb, RrosD.dAdjustmentFactor, m_dKa, m_dK);
                            }
                            else if (m_bbAppAFK && m_bAutoAFK==FALSE)
                            {
                                m_dK = RrosD.dAdjustmentFactor;
                            }

                            double dRel_Locat = (n==0) ? BlrSpanD.dElemI_Rat : BlrSpanD.dElemJ_Rat;

                            if (RpasD.bStrength==TRUE && RlcsD.nLimitState==1) //Strength
                            {              
                                Get_RatingStrengthLCaseForce(n, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, RlcsD, ForceNC, ForceBlrdRcase, RcstC.RcstB[n]);
                                Get_RatingStrengthData(n, nConcurr, RlcsD, ForceBlrdRcase, RcstC.RcstB[n]);
                                //

                                SetRcaseStrengthResult(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, bSpanInfo, 
                                    BlrSpanD.SpanK, BlrSpanD.iL_count, dRel_Locat, dPhi_CF, dPhi_SF, RcstC.RcstB[n]);

                                double *pPositive_RF = dPositive_RF;
                                double *pNegative_RF = dNegative_RF;
                                double *pPositive_CADE = dPositive_CADE;
                                double *pNegative_CADE = dNegative_CADE;

                                double *pVn_RF = dVn_RF;
                                double *pVn_CADE = dVn_RF;

                                // Calculation Rating Factor.
                                CheckStrengthRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                    MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF, dCapacityFsh,
                                    dVuFactor, RcstC.RcstB[n], pPositive_RF, pNegative_RF, pPositive_CADE, pNegative_CADE,
                                    RcfcD, pVn_RF, pVn_CADE, RcscD);  
                            }

                            if (RpasD.bService==TRUE && RlcsD.nLimitState==0) // Service
                            {
                                Get_RatingServiceLCaseForce(n, bDC_Max, bDW_Max, bTemp_Max, RlcsD, ForceBlrdRcase, RcssC.StreB[n]);
                                Get_RatingServiceData(n, nConcurr, RlcsD, ForceBlrdRcase, RcssC.StreB[n]);
                                //
                                SetRcaseServiceResult(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, bSpanInfo, 
                                    BlrSpanD.SpanK, BlrSpanD.iL_count, dRel_Locat, dPhi_CF, dPhi_SF, RcssC.StreB[n]);

                                CheckServicethRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                    MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF,
                                    RcssC.StreB[n], 
                                    dCompI_RF, dTensI_RF, dCompJ_RF, dTensJ_RF,
                                    dCompI_CADE,  dCompJ_CADE,  dTensI_CADE,  dTensJ_CADE,
                                    RcgsD);              
                            }           

                        }
                        if (RpasD.bStrength==TRUE && RlcsD.nLimitState==1) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlStrengthRatingResult(CheckElemK,uRlcskey,nConcurr,RcstC);
                        if (RpasD.bService==TRUE  && RlcsD.nLimitState==0) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlStressRatingResult(CheckElemK,uRlcskey,nConcurr,RcssC);

                    } // m
                }// k
            } 
            else if (RpasD.bFatigue==TRUE && RlcsD.nLimitState==2)
            {
                T_RLCS_KEY auRlcskey[8];
                m_pDoc->m_pPostCtrl->GetStlRatingLcomList(RlcsK, RlcsD, RlcsD.nLimitState, auRlcskey);

                nFatiRCaseCount++;
                int nFatiConcSize = 12;
                MembLcomD.aChkFor.RemoveAll();
                MembLcomD.aUlmf.RemoveAll();
                MembLcomD.aChkFor.SetSize(nFatiConcSize);
                MembLcomD.aUlmf.SetSize(nFatiConcSize);

                int nFatiLcomType = 0;

                CSGFORCOMP_ARRAY aChkForceI;
                aChkForceI.SetSize(nFatiConcSize);
                CSGFORCOMP_ARRAY aChkForceJ;
                aChkForceJ.SetSize(nFatiConcSize);

                CSG_FATI_CR_RSLT FatiCrResD;

                for (int jj=0; jj<nFatiConcSize; ++jj)
                {
                    int nMaxMinType=jj+1;
                    MembLcomD.aChkFor[jj].Initialize();
                    CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[jj];
                    aChkForceJ[jj].nLcomType = aChkForceI[jj].nLcomType = ComLcomF.nLcomType = 2;
                    aChkForceJ[jj].nLcomSub  = aChkForceI[jj].nLcomSub  = ComLcomF.nLcomSub  = 1;//m_pForcCtrl->Get_FatigueType(nDgnLcomId);
                    aChkForceJ[jj].nMax      = aChkForceI[jj].nMax      = ComLcomF.nMax      = nMaxMinType;
                    nFatiLcomType = ComLcomF.nLcomSub;

                    //ConvertRatStlCalcUlmForce(nPosI, nPosM, nPosJ, bDC_Max, bDW_Max, bTemp_Max, nConcurr, ForceRcaseI, ForceRcaseM, ForceRcaseJ, MembLcomD.aUlmf[0]);
                    ConvertRatStlCalcMVChkForce(0, 0, nMaxMinType, ForceBlrdRcase, aChkForceI[jj]);
                    ConvertRatStlCalcMVChkForce(1, 0, nMaxMinType, ForceBlrdRcase, aChkForceJ[jj]);
                }

                double dEpsiC[2] = {0.0, 0.0};              
                double dEpsiT[2] = {RrosD.dStrainI, RrosD.dStrainJ};

                double dDispC[2] = {dDispI, dDispJ};
                double dDispT[2] = {RrosD.dDefI, RrosD.dDefJ};

                double dKa_f[2] = {0.0, 0.0};
                double dK_f[2]  = {1.0, 1.0};              

                if (m_bbAppAFK && m_bAutoAFK)
                {
                    for (int jj=0; jj<2; ++jj )
                    {
                        CSG_MEMB_POSD_KSCE_LSD MembLcomMVpr = MembLcomD;
                        if ( jj==0) MembLcomMVpr.aChkFor.Copy(aChkForceI);
                        else      MembLcomMVpr.aChkFor.Copy(aChkForceJ);

                        CSG_MEMBPOS_RES_AASHTO_LRFD_D StressMembRes = *MembR.pAASHTO;
                        Calc_Stress4MVpr(CheckElemK, jj, MembLcomMVpr, StressMembRes, dEpsiC[jj]);

                        double dRepCalcVal = (m_pRpasD->nMeasurement==0) ? dEpsiC[jj] : dDispC[jj];
                        double dRepTestVal = (m_pRpasD->nMeasurement==0) ? dEpsiT[jj] : dDispT[jj];
                        Calc_AdjustmentFactorK(dRepCalcVal, dRepTestVal, m_dKb, RrosD.dAdjustmentFactor, dKa_f[jj], dK_f[jj]);
                    }
                }
                else if (m_bbAppAFK && m_bAutoAFK==FALSE)
                {
                    dK_f[0] = dK_f[1] = RrosD.dAdjustmentFactor;
                }


                CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D FatiResMV_PR[2];
                CSG_CHK_RES_DTR_PARAM  DtrR;

                // PR. MV 계산. 
                for (int jj=0; jj<2; ++jj )
                {
                    MembLcomD.Memb.ChIf.nChkPos = jj;

                    if ( jj==0) MembLcomD.aChkFor.Copy(aChkForceI);
                    else      MembLcomD.aChkFor.Copy(aChkForceJ);

                    m_pCSGDataCtrl->Check_Fatigue(CheckElemK, jj, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
                    FatiResMV_PR[jj] = ChkResD.pAASHTO->FatigueD;
                }
                double dStrDelMV_PR_I[2]={0.0};
                double dStrDelMV_PR_J[2]={0.0};
                dStrDelMV_PR_I[0] = FatiResMV_PR[0].Fat_LoadInduceD.df_fat;
                dStrDelMV_PR_I[1] = FatiResMV_PR[0].Fat_LoadInduceD.df_fat_B;
                dStrDelMV_PR_J[0] = FatiResMV_PR[1].Fat_LoadInduceD.df_fat;
                dStrDelMV_PR_J[1] = FatiResMV_PR[1].Fat_LoadInduceD.df_fat_B;

                double dDelF_TH = FatiResMV_PR[0].Fat_LoadInduceD.dF_TH;
                dCapaN_Fati = dDelF_TH;

                CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D FatiResMV_AD[2];

                // AD. MV 계산. 

                CSGFORCOMP_ARRAY aChkForceAdI;
                aChkForceAdI.SetSize(nFatiConcSize);
                CSGFORCOMP_ARRAY aChkForceAdJ;
                aChkForceAdJ.SetSize(nFatiConcSize);

                CSG_MEMB_POSD_KSCE_LSD MembLcomMVAd = MembLcomD;
                for (int jj=0; jj<nFatiConcSize; ++jj )
                {
                    int nMaxMinType=jj+1;
                    MembLcomMVAd.aChkFor[jj].Initialize();
                    CSG_FORCE_COMP &ComLcomF = MembLcomMVAd.aChkFor[jj];
                    aChkForceAdJ[jj].nLcomType = aChkForceAdI[jj].nLcomType = ComLcomF.nLcomType = 2;
                    aChkForceAdJ[jj].nLcomSub  = aChkForceAdI[jj].nLcomSub  = ComLcomF.nLcomSub  = 1;//m_pForcCtrl->Get_FatigueType(nDgnLcomId);
                    aChkForceAdJ[jj].nMax      = aChkForceAdI[jj].nMax      = ComLcomF.nMax      = nMaxMinType;

                    ConvertRatStlCalcMVChkForce(0, 1, nMaxMinType, ForceBlrdRcase, aChkForceAdI[jj]);
                    ConvertRatStlCalcMVChkForce(1, 1, nMaxMinType, ForceBlrdRcase, aChkForceAdJ[jj]);
                }

                CSG_MEMBPOS_RES_AASHTO_LRFD_D FatiAdMembRes = *MembR.pAASHTO;
                CSG_MEMB_RESULT_D FatiAdMembR;
                FatiAdMembR.pAASHTO = &FatiAdMembRes;
                for (int jj=0; jj<2; ++jj )
                {
                    MembLcomMVAd.Memb.ChIf.nChkPos = jj;

                    if ( jj==0) MembLcomMVAd.aChkFor.Copy(aChkForceAdI);
                    else      MembLcomMVAd.aChkFor.Copy(aChkForceAdJ);

                    m_pCSGDataCtrl->Check_Fatigue(CheckElemK, jj, MembLcomMVAd, FatiAdMembR, ChkResD, DtrR, FatiCrResD);
                    FatiResMV_AD[jj] = ChkResD.pAASHTO->FatigueD;
                }
                double dStrDelMV_AD_I[2]={0.0};
                double dStrDelMV_AD_J[2]={0.0};
                dStrDelMV_AD_I[0] = FatiResMV_AD[0].Fat_LoadInduceD.df_fat;
                dStrDelMV_AD_I[1] = FatiResMV_AD[0].Fat_LoadInduceD.df_fat_B;
                dStrDelMV_AD_J[0] = FatiResMV_AD[1].Fat_LoadInduceD.df_fat;
                dStrDelMV_AD_J[1] = FatiResMV_AD[1].Fat_LoadInduceD.df_fat_B;

                T_STRB_D MembForNC;

                int nChkLcomSize=1;
                MembLcomD.aChkFor.RemoveAll();
                MembLcomD.aChkFor.SetSize(nChkLcomSize);
                for(k=0; k<cRAT_LCASE; k++)
                {
                    m_pDoc->Get_BlrcLoadTypeData(k, bDC_Max, bDW_Max, bTemp_Max);

                    if (IsSkipRatingCase(bDC_Max, bDW_Max, bTemp_Max, RlcsD)==FALSE) continue;
                    T_RLCS_KEY uRlcskey = auRlcskey[k];
                    if(uRlcskey==0) continue;

                    MembLcomD.aChkFor[0].Initialize();

                    int nFatiConcurr = 1;
                    //T_RLCS_KEY uRlcskey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr);

                    double dFactorDL = bDC_Max ? RlcsD.RateLoad[0].dFactor[0] : RlcsD.RateLoad[0].dFactor[1];
                    if ( bIsPLCB==TRUE )
                    {
                        if ( j<nPreForceLcomSize )
                        {
                            int nInx = bDC_Max ? 0 : 1;
                            ForcePreCompo.arForceRCase[j].ForceDCBe[nInx].GetForce(ForceNC);
                        }
                    }

                    int nRCDgnLcomNo = m_pForcCtrl->GetStlRCDgnLcomKey(cRAT_LCASE, cFORC_CON_DGN, j, k, 0);
                    if ( bIsPLCB==FALSE ) m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC, dFactorDL);

                    MembForNC.Initialize();
                    m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, aUblmElemK, &MembForNC, dFactorDL);

                    // UlmForce
                    MakeConverMemberForce(StctD, j, nGdLastStagK, nLastStepNo, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurr, aUblmElemK, MembForNC, MembLcomD);

                    T_RCFA_CASE RcfaC;
                    for(int n=0; n<2; n++)
                    {

                        if     (n==0 && nChkPosition==1) continue;
                        else if(n==1 && nChkPosition==0) continue;

                        CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];

                        MembLcomD.Memb.ChIf.nChkPos = n;

                        ConvertRatStlCalcChkForce(n, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr, ForceNC, ForceBlrdRcase, ComLcomF);

                        double dRel_Locat = (n==0) ? BlrSpanD.dElemI_Rat : BlrSpanD.dElemJ_Rat;

                        Get_RatingFatigueLCaseForce(n, bDC_Max, bDW_Max, bTemp_Max, RlcsD, ForceBlrdRcase, RcfaC.FatiB[n]);

                        Get_RatingFatigueData(n, nFatiConcurr, RlcsD, ForceBlrdRcase, RcfaC.FatiB[n]);

                        //            
                        m_dEpsiC = dEpsiC[n];
                        m_dEpsiT = dEpsiT[n];
                        m_dDispC = dDispC[n];
                        m_dDispT = dDispT[n];
                        m_dKa    = dKa_f[n];
                        m_dK     = dK_f[n];

                        SetRcaseFatiResult(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr, bSpanInfo, 
                            BlrSpanD.SpanK, BlrSpanD.iL_count, dRel_Locat, dPhi_CF, dPhi_SF, RcfaC.FatiB[n]);

                        double *pStrDelPR = (n==0) ? dStrDelMV_PR_I : dStrDelMV_PR_J;
                        double *pStrDelAD = (n==0) ? dStrDelMV_AD_I : dStrDelMV_AD_J;            

                        CheckFatigueRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr,
                            MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF, dCapaN_Fati,
                            pStrDelPR, pStrDelAD,
                            RcfaC.FatiB[n],
                            dFTopI_RF, dFBotI_RF, dFTopJ_RF, dFBotJ_RF,
                            dFTopI_CADE, dFBotI_CADE, dFTopJ_CADE, dFBotJ_CADE,
                            RcfaD);
                    }

                    m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlFatigueRatingResult(CheckElemK,uRlcskey,nFatiConcurr,RcfaC);
                }

            }
            else ASSERT(0);
        } // j
        //         

        if(RpasD.bStrength==TRUE)
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcfcResult(CheckElemK, RcfcD);
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcscResult(CheckElemK, RcscD);
        }
        if(RpasD.bService==TRUE)
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcgsResult(CheckElemK, RcgsD);
        }
        if(RpasD.bFatigue==TRUE)
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcfaResult(CheckElemK, RcfaD);
        }

    }

    //
    Check_RFminbyRatingCase();

    if (MembR.pAASHTO != NULL) delete MembR.pAASHTO;

    if (ChkResD.pAASHTO != NULL) delete ChkResD.pAASHTO;

    return TRUE;
}

enum enResultType
{
	Design_Strength,
	Design_Service,
	Legar_Strength,
	Legar_Service,
	Permit_Strength,
	Permit_Service,
	Num,
};

BOOL CDgnStlRatingDataCtrl::Check_AASHTO_LRFR19()
{
    // Add by ZINU.('05.09.05).	PSC-Design/Strs(5), PSC-Design/Strn(6).
    //int iDgnStatus = m_pDoc->GetDesignStatus();
    //m_pDoc->SetDesignStatus(5);

    Make_RatingDgnData();

    CArray<T_RLCS_K, T_RLCS_K> aRlcsKey;
    m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(aRlcsKey);
    int nRlcsKeySize = aRlcsKey.GetSize();

    int nTotalElement = m_arElemK.GetSize();

    T_RPAS_D RpasD; RpasD.Initialize();
    m_pDoc->m_pAttrCtrl2->GetRpas(RpasD);
    m_pRpasD = &RpasD;

    double dPhi_SF = RpasD.dSystemFactor;

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);

    int i=0, j=0, k=0, m=0, n=0;
    FORCE_BLRD_ELEM ForceBlrdElem;
    FORCE_BLRD_ELEM ForcePreCompo;
    BLR_SPAN_D BlrSpanD;

    //double dAlwCompStress = RpasD.dAlwCompStress;    
    //double dAlwTensStress = RpasD.dAlwTensStress;

    m_pDataCtrl->Calc_LengthPosition4Span(m_arElemK);

    // Pre-Combined Composite Bridge
    BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();


    CSG_MEMB_RESULT_D      MembR;
    CSG_CHECK_RESULT_CODES ChkResD;

    MembR.pAASHTO = new CSG_MEMBPOS_RES_AASHTO_LRFD_D;

    ChkResD.pAASHTO = new CSG_MEMB_RES_AASHTO_LRFD_D;

    DgnBeamPairK nCurUblmK(EN_EL_BEAM, 0);
    ArrElemPairKey aUblmElemK;
    BOOL bCantilever=FALSE;
    T_RROS_D RrosD;
    for ( i=0; i<nTotalElement; ++i )
    {
        CSG_MEMB_POSD_KSCE_LSD MembLcomD;
        ConvertRatingGlobalData(RpasD, MembLcomD.Memb.Csgd);

        int nMaxMin=0;
        auto CheckElemK = m_arElemK[i];

        RrosD.Initialize();
        if (CheckElemK.second == EN_EL_BEAM)
        {
            m_bbAppAFK = m_pDoc->m_pAttrCtrl2->GetRros(CheckElemK.first, RrosD); // Adjustment Factor, K 적용 여부 
        }
        else if (CheckElemK.second == EN_EL_VBEAM)
        {
            m_bbAppAFK = m_pDoc->m_pAttrCtrl2->GetRrov(CheckElemK.first, RrosD);
        }
        else ASSERT(0);
        m_bAutoAFK = (RrosD.nCalcType==0);

        m_dKb = m_bbAppAFK ? RrosD.dImpactFactor : 1.0;
        MembLcomD.Memb.ChIf.bCantilever = bCantilever;
        MembLcomD.Memb.ChIf.nBeamType = CheckElemK.second;

        BlrSpanD.Initialize();
        BOOL bSpanInfo=FALSE;
        if ( m_pDataCtrl->Get_ElemSpanInfo(CheckElemK, BlrSpanD) )
        {
            bSpanInfo = TRUE;
        }

        T_STAG_K nGdLastStagK=0;
        int nLastStepNo=0;
        m_pForcCtrl->GetGirderLastStageKey(CheckElemK, nGdLastStagK, nLastStepNo);
        int nForceBeforeType = 1;

        T_ELEM_WEIGHT ElemWgt;
        CSelfWgt SelfWgt(m_pDoc);
        SelfWgt.GetElemWeight(CheckElemK.first, ElemWgt);
        m_dWeight = ElemWgt.dTotalWeight;

        T_GRUP_K CheckGroupK = 0; //rKeyList[0]; //arGrupList[0];
        if ( !m_pDataCtrl->GetBlrCheckGroupKey(CheckElemK, CheckGroupK) ) return FALSE;


        MembR.pAASHTO->Initialize();

        int nMatlK = CDBLib::GetMaterialKey4ElemPairK(CheckElemK);
        if ( !m_pCSGDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl) ) MembLcomD.Memb.Matl.Initialize();

        double dVuFactor = 1.0;
        m_pCSGDataCtrl->GetCsgSectionSupport(CheckElemK, MembLcomD);
        if (IsSectTypeWith2Web(MembLcomD.Memb.PosD[0].Sect.SectInfo.nStype))
        {
            dVuFactor = 0.5;
        }

        m_pCSGDataCtrl->Get_CsgMembInData(CheckElemK, MembLcomD);

        MembLcomD.Memb.ChIf.nChkPos = 0;
        //MembLcomD.Memb.ChIf.bCurved = bCurved;
        m_pCSGDataCtrl->MakeMemberResultData(CheckElemK, MembLcomD, MembR);

        T_SPAN_K CurSpanK=0;
        ElemPairK nElemI = CheckElemK, nElemM = CheckElemK, nElemJ = CheckElemK;
        int nPosI=0;
        int nPosM=0; // 임시, 찾아야함. 
        int nPosJ=1;

        BOOL bChkUblm=FALSE;
        BOOL bCantilever=FALSE;
        BOOL bMakeMembFor=FALSE;
        BOOL b2SideBgg=FALSE;
        DgnBeamPairK nUblmK(EN_EL_BEAM, 0);

        m_pDataCtrl->GetUbLengthElemInfo(CheckElemK, nUblmK, nCurUblmK, nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, bChkUblm, bCantilever, bMakeMembFor, b2SideBgg, aUblmElemK);
        MembLcomD.bChkUlmf = true;

        T_KFAC_D KFacD; KFacD.Initialize();
        BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(CheckElemK, KFacD);
        m_pCSGDataCtrl->Get_UnbracedLengthData(CheckElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);
        //
        m_pCSGDataCtrl->Get_CsgUlmSection(nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);

        //MembLcomD.Memb.ChIf.dFactorCS = dFactorDeadCS;
        MembLcomD.Memb.ChIf.bCantilever = bCantilever;


        // 시공중 거더 Force

        _DGN_FORC_CRC ForceNC;

        int nLcomNum = m_pForcCtrl->Get_LcomDataCount();

        ADGNFORCE aPreForceLcom;
        if ( bIsPLCB==TRUE )
        {
            //m_pForcCtrl->Get_BeforeForce4PreCompo(CheckElemK, aPreForceLcom, nLcomNum);
            const auto& iter = m_pForcCtrl->m_arBlrdPreCompoForecElem.find(CheckElemK);
            if (iter != m_pForcCtrl->m_arBlrdPreCompoForecElem.end())
                ForcePreCompo = iter->second;
        }

        int nPreForceLcomSize = ForcePreCompo.arForceRCase.GetSize();

        T_RSGS_D RsgsD; RsgsD.Initialize();
        m_pDoc->m_pAttrCtrl2->GetRsgs(CheckGroupK, RsgsD);
        int    nChkPosition = RsgsD.iChkPosition;         // 0=I 단, 1=J 단, 2=I,J 단
        double dPhi_CF      = RsgsD.dConditionFactor;    //   

        ForceBlrdElem.Initialize();
        const auto& itForceElem = m_pForcCtrl->m_arBlrdForceElem.find(CheckElemK);
        if (itForceElem == m_pForcCtrl->m_arBlrdForceElem.end()) continue;
        ForceBlrdElem = itForceElem->second;

        CArray<FORCE_BLRD_RCASE, FORCE_BLRD_RCASE> arForceRCase;
        arForceRCase.RemoveAll();
        arForceRCase.Copy(ForceBlrdElem.arForceRCase);

        // Condition factor, System factor.
        // Ref. Load and Resistance Factor Rating. 6.4.2 General Load-Rating Equation
        // Where thr following lower limit shall apply : phi_c*phi_s >= 0.85
        double dCapacityF=dPhi_CF*dPhi_SF;
        if ( dCapacityF<0.85 ) dCapacityF = 0.85;

        double dCapacityFsh = 1.0 * dPhi_CF; // Shear system factor는 항상 1.0으로 적용, AASHTO LRFR 11,6A.4.2.4 PSC(14.07.10) by shishir 
        if ( dCapacityFsh < 0.85 ) dCapacityFsh = 0.85;

        double dMaxValue = 1.0/pow(m_dZero, 10);

        double dPositive_RF_DGN[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dPositive_RF_LGL[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dPositive_RF_PER[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        double dNegative_RF_DGN[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dNegative_RF_LGL[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dNegative_RF_PER[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        double dPositive_CADE_DGN[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dPositive_CADE_LGL[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dPositive_CADE_PER[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        double dNegative_CADE_DGN[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dNegative_CADE_LGL[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dNegative_CADE_PER[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        double dVn_RF_DGN[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dVn_RF_LGL[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dVn_RF_PER[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        double dVn_CADE_DGN[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dVn_CADE_LGL[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J
        double dVn_CADE_PER[2] = { dMaxValue, dMaxValue }; //0 : I 1 : J

        //
        double dCompI_RF_Design = dMaxValue;
        double dCompJ_RF_Design = dMaxValue;
        double dTensI_RF_Design = dMaxValue;
        double dTensJ_RF_Design = dMaxValue;
        double dCompI_CADE_Design = dMaxValue;
        double dCompJ_CADE_Design = dMaxValue;
        double dTensI_CADE_Design = dMaxValue;
        double dTensJ_CADE_Design = dMaxValue;
        
        double dCompI_RF_Legal = dMaxValue;
        double dCompJ_RF_Legal = dMaxValue;
        double dTensI_RF_Legal = dMaxValue;
        double dTensJ_RF_Legal = dMaxValue;
        double dCompI_CADE_Legal = dMaxValue;
        double dCompJ_CADE_Legal = dMaxValue;
        double dTensI_CADE_Legal = dMaxValue;
        double dTensJ_CADE_Legal = dMaxValue;

        double dCompI_RF_Permit = dMaxValue;
        double dCompJ_RF_Permit = dMaxValue;
        double dTensI_RF_Permit = dMaxValue;
        double dTensJ_RF_Permit = dMaxValue;
        double dCompI_CADE_Permit = dMaxValue;
        double dCompJ_CADE_Permit = dMaxValue;
        double dTensI_CADE_Permit = dMaxValue;
        double dTensJ_CADE_Permit = dMaxValue;

        int nFatiRCaseCount=0;
        //T_RCST_CASE PosiMinRcstD;
        //T_RCST_CASE NegaMinRcstD;

        int nConcurr=0;
        BOOL bDC_Max, bDW_Max, bTemp_Max;

		// Critical Rating case
		T_RCFC_D RcfcD_Design; RcfcD_Design.Initialize(); // Flexure_Design
		T_RCFC_D RcfcD_Legal;  RcfcD_Legal.Initialize();// Flexure_Legal
		T_RCFC_D RcfcD_Permit; RcfcD_Permit.Initialize();// Flexure_Permit

		T_RCSC_D RcscD_Design; RcscD_Design.Initialize();// Shear_Design
		T_RCSC_D RcscD_Legal;  RcscD_Legal.Initialize();// Shear_Legal
		T_RCSC_D RcscD_Permit; RcscD_Permit.Initialize();// Shear_Permit

		T_RCGS_D RcgsD_Design; RcgsD_Design.Initialize();// Service_Design
		T_RCGS_D RcgsD_Legal;  RcgsD_Legal.Initialize();// Service_Legal
		T_RCGS_D RcgsD_Permit; RcgsD_Permit.Initialize();// Service_Permit

		T_RCFA_INF_LIFE_D RcfaInfD; RcfaInfD.Initialize();// Fatigue Infinite
		T_RCFA_FIN_LIFE_D RcfaFinD; RcfaFinD.Initialize();// Fatigue Finite

		std::vector<bool> vResult;
		vResult.resize(enResultType::Num);

        for ( j=0; j<nRlcsKeySize; ++j )
        {
            T_RLCS_K RlcsK = aRlcsKey[j];
            T_RLCS_D RlcsD; RlcsD.Initialize();
            m_pDoc->m_pAttrCtrl2->GetRlcs(RlcsK, RlcsD);
            //if(BlrcD.iLimitState==0) continue;
            //ChangeTemperatureFactor(BlrcD);

            T_LCOM_D LcomDe;
            double dDispI=0.0, dDispJ=0.0;
            if ( m_bbAppAFK && m_bAutoAFK )
            {
                //MakeLcomData4RCase(RlcsD, bDC_Max, bDW_Max, bTemp_Max, LcomDe);
                Get_DeflectionOfNode(CheckElemK.first, RlcsD, LcomDe, dDispI, dDispJ);
            }

            FORCE_BLRD_RCASE &ForceBlrdRcase = arForceRCase[j];
            if ( RlcsD.nLimitState==1 || RlcsD.nLimitState==0 )
            {
                T_RLCS_KEY auRlcskey[96];
                m_pDoc->m_pPostCtrl->GetStlRatingLcomList(RlcsK, RlcsD, RlcsD.nLimitState, auRlcskey);

                MembLcomD.aChkFor.RemoveAll();
                MembLcomD.aUlmf.RemoveAll();
                int nChkLcomSize=1;
                int nChkLcomSizeUlmf = 6;
                MembLcomD.aUlmf.SetSize(nChkLcomSizeUlmf);
                MembLcomD.aChkFor.SetSize(nChkLcomSize);

                for ( k=0; k<cRAT_LCASE; k++ )
                {
                    m_pDoc->Get_BlrcLoadTypeData(k, bDC_Max, bDW_Max, bTemp_Max);

                    if ( IsSkipRatingCase(bDC_Max, bDW_Max, bTemp_Max, RlcsD)==FALSE ) continue;

                    double dFactorDL = bDC_Max ? RlcsD.RateLoad[0].dFactor[0] : RlcsD.RateLoad[0].dFactor[1];
                    if ( bIsPLCB==TRUE )
                    {
                        if ( j<nPreForceLcomSize )
                        {
                            int nInx = bDC_Max ? 0 : 1;
                            ForcePreCompo.arForceRCase[j].ForceDCBe[nInx].GetForce(ForceNC);
                        }
                    }

                    T_STRB_D MembForNC;
                    for ( m=0; m<cFORC_CON_DGN; m++ )
                    {
                        int iIdx = k*12 + m;
                        T_RLCS_KEY uRlcskey = auRlcskey[iIdx];
                        if ( uRlcskey == 0 ) continue;

                        nConcurr=m+1;

                        MembLcomD.aChkFor[0].Initialize();
                        CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];
                        if ( RlcsD.nLimitState==1 ) ComLcomF.nLcomType = 0;
                        else if ( RlcsD.nLimitState==0 ) // Service
                        {
                            ComLcomF.nLcomType = 1;
                            //ComLcomF.nLcomSub  = nServLcomType;
                        }
                        ComLcomF.nMax = nConcurr;

                        int nRCDgnLcomNo = m_pForcCtrl->GetStlRCDgnLcomKey(cRAT_LCASE, cFORC_CON_DGN, j, k, m);
                        if ( bIsPLCB==FALSE ) m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC, dFactorDL);

                        MembForNC.Initialize();
                        m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, aUblmElemK, &MembForNC, dFactorDL);

                        // UlmForce
                        MakeConverMemberForce(StctD, j, nGdLastStagK, nLastStepNo, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurr, aUblmElemK, MembForNC, MembLcomD);

                        //T_RLCS_KEY uRlcskey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr);
                        T_RCST_CASE RcstC;
                        T_RCSS_CASE RcssC;
                        for ( n=0; n<2; n++ )
                        {

                            if ( n==0 && nChkPosition==1 ) continue;
                            else if ( n==1 && nChkPosition==0 ) continue;

                            MembLcomD.Memb.ChIf.nChkPos = n;

                            ConvertRatStlCalcChkForce(n, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurr, ForceNC, ForceBlrdRcase, ComLcomF);

                            m_dEpsiC = 0.0;
                            m_dEpsiT = (n==0) ? RrosD.dStrainI : RrosD.dStrainJ;

                            m_dDispC = (n==0) ? dDispI : dDispJ;
                            m_dDispT = (n==0) ? RrosD.dDefI : RrosD.dDefJ;

                            m_dKa = 0.0;
                            m_dK  = 1.0;

                            int nCCId = nConcurr-1;
                            if ( m_bbAppAFK && m_bAutoAFK )
                            {
                                CSG_MEMBPOS_RES_AASHTO_LRFD_D StressMembRes = *MembR.pAASHTO;
                                Calc_Stress4MVpr(CheckElemK, n, MembLcomD, StressMembRes, m_dEpsiC);
                                double dRepCalcVal = (m_pRpasD->nMeasurement==0) ? m_dEpsiC : m_dDispC;
                                double dRepTestVal = (m_pRpasD->nMeasurement==0) ? m_dEpsiT : m_dDispT;
                                Calc_AdjustmentFactorK(dRepCalcVal, dRepTestVal, m_dKb, RrosD.dAdjustmentFactor, m_dKa, m_dK);
                            }
                            else if ( m_bbAppAFK && m_bAutoAFK==FALSE )
                            {
                                m_dK = RrosD.dAdjustmentFactor;
                            }

                            double dRel_Locat = (n==0) ? BlrSpanD.dElemI_Rat : BlrSpanD.dElemJ_Rat;

                            if ( RpasD.bStrength==TRUE && RlcsD.nLimitState==1 ) //Strength
                            {
                                Get_RatingStrengthLCaseForce(n, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, RlcsD, ForceNC, ForceBlrdRcase, RcstC.RcstB[n]);
                                Get_RatingStrengthData(n, nConcurr, RlcsD, ForceBlrdRcase, RcstC.RcstB[n]);
                                //

                                SetRcaseStrengthResult(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, bSpanInfo,
                                    BlrSpanD.SpanK, BlrSpanD.iL_count, dRel_Locat, dPhi_CF, dPhi_SF, RcstC.RcstB[n]);

                                double *pPositive_RF_DGN = dPositive_RF_DGN;
                                double *pPositive_RF_LGL = dPositive_RF_LGL;
                                double *pPositive_RF_PER = dPositive_RF_PER;
                                double *pNegative_RF_DGN = dNegative_RF_DGN;
                                double *pNegative_RF_LGL = dNegative_RF_LGL;
                                double *pNegative_RF_PER = dNegative_RF_PER;

                                double *pPositive_CADE_DGN = dPositive_CADE_DGN;
                                double *pPositive_CADE_LGL = dPositive_CADE_LGL;
                                double *pPositive_CADE_PER = dPositive_CADE_PER;
                                double *pNegative_CADE_DGN = dNegative_CADE_DGN;
                                double *pNegative_CADE_LGL = dNegative_CADE_LGL;
                                double *pNegative_CADE_PER = dNegative_CADE_PER;

                                double *pVn_RF_DGN = dVn_RF_DGN;
                                double *pVn_RF_LGL = dVn_RF_LGL;
                                double *pVn_RF_PER = dVn_RF_PER;

                                double *pVn_CADE_DGN = dVn_RF_DGN;
                                double *pVn_CADE_LGL = dVn_RF_LGL;
                                double *pVn_CADE_PER = dVn_RF_PER;

                                switch ( RlcsD.nEvaluation )
                                {
                                case ENUM_DESIGN:
									vResult[enResultType::Design_Strength] = true;
                                    // Calculation Rating Factor.
                                    CheckStrengthRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                        MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF, dCapacityFsh,
                                        dVuFactor, RcstC.RcstB[n], pPositive_RF_DGN, pNegative_RF_DGN, pPositive_CADE_DGN, pNegative_CADE_DGN,
                                        RcfcD_Design, pVn_RF_DGN, pVn_CADE_DGN, RcscD_Design);
                                    break;
                                case ENUM_LEGAL:
									vResult[enResultType::Legar_Strength] = true;
                                    // Calculation Rating Factor.
                                    CheckStrengthRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                        MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF, dCapacityFsh,
                                        dVuFactor, RcstC.RcstB[n], pPositive_RF_LGL, pNegative_RF_LGL, pPositive_CADE_LGL, pNegative_CADE_LGL,
                                        RcfcD_Legal, pVn_RF_LGL, pVn_CADE_LGL, RcscD_Legal);
                                    break;
                                case ENUM_PERMIT:
									vResult[enResultType::Permit_Strength] = true;
                                    // Calculation Rating Factor.
                                    CheckStrengthRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                        MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF, dCapacityFsh,
                                        dVuFactor, RcstC.RcstB[n], pPositive_RF_PER, pNegative_RF_PER, pPositive_CADE_PER, pNegative_CADE_PER,
                                        RcfcD_Permit, pVn_RF_PER, pVn_CADE_PER, RcscD_Permit);
                                    break;
                                default:
                                    ASSERT(0);
                                    break;
                                }
                            }

                            if ( RpasD.bService==TRUE && RlcsD.nLimitState==0 ) // Service
                            {
                                Get_RatingServiceLCaseForce(n, bDC_Max, bDW_Max, bTemp_Max, RlcsD, ForceBlrdRcase, RcssC.StreB[n]);
                                Get_RatingServiceData(n, nConcurr, RlcsD, ForceBlrdRcase, RcssC.StreB[n]);
                                //
                                SetRcaseServiceResult(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, bSpanInfo,
                                    BlrSpanD.SpanK, BlrSpanD.iL_count, dRel_Locat, dPhi_CF, dPhi_SF, RcssC.StreB[n]);

                                switch ( RlcsD.nEvaluation )
                                {
                                case ENUM_DESIGN:
									vResult[enResultType::Design_Service] = true;
                                    CheckServicethRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                        MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF,
                                        RcssC.StreB[n],
                                        dCompI_RF_Design, dTensI_RF_Design, dCompJ_RF_Design, dTensJ_RF_Design,
                                        dCompI_CADE_Design, dCompJ_CADE_Design, dTensI_CADE_Design, dTensJ_CADE_Design,
                                        RcgsD_Design);
                                    break;
                                case ENUM_LEGAL:
									vResult[enResultType::Legar_Service] = true;
                                    CheckServicethRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                        MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF,
                                        RcssC.StreB[n],
                                        dCompI_RF_Legal, dTensI_RF_Legal, dCompJ_RF_Legal, dTensJ_RF_Legal,
                                        dCompI_CADE_Legal, dCompJ_CADE_Legal, dTensI_CADE_Legal, dTensJ_CADE_Legal,
                                        RcgsD_Legal);
                                    break;
                                case ENUM_PERMIT:
									vResult[enResultType::Permit_Service] = true;
                                    CheckServicethRatingcaseAASHTO12(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
                                        MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF,
                                        RcssC.StreB[n],
                                        dCompI_RF_Permit, dTensI_RF_Permit, dCompJ_RF_Permit, dTensJ_RF_Permit,
                                        dCompI_CADE_Permit, dCompJ_CADE_Permit, dTensI_CADE_Permit, dTensJ_CADE_Permit,
                                        RcgsD_Permit);
                                    break;
                                default:
                                    ASSERT(0);
                                    break;
                                }
                            }
                        }
                        if ( RpasD.bStrength==TRUE && RlcsD.nLimitState==1 ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlStrengthRatingResult(CheckElemK, uRlcskey, nConcurr, RcstC);
                        if ( RpasD.bService==TRUE  && RlcsD.nLimitState==0 ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlStressRatingResult(CheckElemK, uRlcskey, nConcurr, RcssC);

                    } // m
                }// k
            }
            else if ( RpasD.bFatigue==TRUE && RlcsD.nLimitState==2 )
            {
                double dCapaN_Fati[4] = { 0.0, };

                double dConstantA[4] = { 0.0, };

                double dStressI_RF[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };
                double dStressJ_RF[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };

                double dStressI_CADE[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };
                double dStressJ_CADE[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };

                double dStressFINI_RF[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };
                double dStressFINJ_RF[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };

                double dStressFINI_CADE[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };
                double dStressFINJ_CADE[4] = { dMaxValue, dMaxValue, dMaxValue, dMaxValue };

                double *pStressI_RF = dStressI_RF;
                double *pStressJ_RF = dStressJ_RF;

                double *pStressFINI_RF = dStressFINI_RF;
                double *pStressFINJ_RF = dStressFINJ_RF;

                double *pStressI_CADE = dStressI_CADE;
                double *pStressJ_CADE = dStressJ_CADE;

                double *pStressFINI_CADE = dStressFINI_CADE;
                double *pStressFINJ_CADE = dStressFINJ_CADE;

                T_RLCS_KEY auRlcskey[8];
                m_pDoc->m_pPostCtrl->GetStlRatingLcomList(RlcsK, RlcsD, RlcsD.nLimitState, auRlcskey);

                nFatiRCaseCount++;
                int nFatiConcSize = 12;
                MembLcomD.aChkFor.RemoveAll();
                MembLcomD.aUlmf.RemoveAll();
                MembLcomD.aChkFor.SetSize(nFatiConcSize);
                MembLcomD.aUlmf.SetSize(nFatiConcSize);

                int nFatiLcomType = 0;

                CSGFORCOMP_ARRAY aChkForceI;
                aChkForceI.SetSize(nFatiConcSize);
                CSGFORCOMP_ARRAY aChkForceJ;
                aChkForceJ.SetSize(nFatiConcSize);

                CSG_FATI_CR_RSLT FatiCrResD;

                for ( int jj=0; jj<nFatiConcSize; ++jj )
                {
                    int nMaxMinType=jj+1;
                    MembLcomD.aChkFor[jj].Initialize();
                    CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[jj];
                    aChkForceJ[jj].nLcomType = aChkForceI[jj].nLcomType = ComLcomF.nLcomType = 2;
                    aChkForceJ[jj].nLcomSub  = aChkForceI[jj].nLcomSub  = ComLcomF.nLcomSub  = 1;//m_pForcCtrl->Get_FatigueType(nDgnLcomId);
                    aChkForceJ[jj].nMax      = aChkForceI[jj].nMax      = ComLcomF.nMax      = nMaxMinType;
                    nFatiLcomType = ComLcomF.nLcomSub;

                    //ConvertRatStlCalcUlmForce(nPosI, nPosM, nPosJ, bDC_Max, bDW_Max, bTemp_Max, nConcurr, ForceRcaseI, ForceRcaseM, ForceRcaseJ, MembLcomD.aUlmf[0]);
                    ConvertRatStlCalcMVChkForce(0, 0, nMaxMinType, ForceBlrdRcase, aChkForceI[jj]);
                    ConvertRatStlCalcMVChkForce(1, 0, nMaxMinType, ForceBlrdRcase, aChkForceJ[jj]);
                }

                double dEpsiC[2] = { 0.0, 0.0 };
                double dEpsiT[2] = { RrosD.dStrainI, RrosD.dStrainJ };

                double dDispC[2] = { dDispI, dDispJ };
                double dDispT[2] = { RrosD.dDefI, RrosD.dDefJ };

                double dKa_f[2] = { 0.0, 0.0 };
                double dK_f[2]  = { 1.0, 1.0 };

                if ( m_bbAppAFK && m_bAutoAFK )
                {
                    for ( int jj=0; jj<2; ++jj )
                    {
                        CSG_MEMB_POSD_KSCE_LSD MembLcomMVpr = MembLcomD;
                        if ( jj==0 ) MembLcomMVpr.aChkFor.Copy(aChkForceI);
                        else      MembLcomMVpr.aChkFor.Copy(aChkForceJ);

                        CSG_MEMBPOS_RES_AASHTO_LRFD_D StressMembRes = *MembR.pAASHTO;
                        Calc_Stress4MVpr(CheckElemK, jj, MembLcomMVpr, StressMembRes, dEpsiC[jj]);

                        double dRepCalcVal = (m_pRpasD->nMeasurement==0) ? dEpsiC[jj] : dDispC[jj];
                        double dRepTestVal = (m_pRpasD->nMeasurement==0) ? dEpsiT[jj] : dDispT[jj];
                        Calc_AdjustmentFactorK(dRepCalcVal, dRepTestVal, m_dKb, RrosD.dAdjustmentFactor, dKa_f[jj], dK_f[jj]);
                    }
                }
                else if ( m_bbAppAFK && m_bAutoAFK==FALSE )
                {
                    dK_f[0] = dK_f[1] = RrosD.dAdjustmentFactor;
                }


                CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D FatiResMV_PR[2];
                CSG_CHK_RES_DTR_PARAM  DtrR;

                // PR. MV 계산. 
                for ( int jj=0; jj<2; ++jj )
                {
                    MembLcomD.Memb.ChIf.nChkPos = jj;

                    if ( jj==0 ) MembLcomD.aChkFor.Copy(aChkForceI);
                    else      MembLcomD.aChkFor.Copy(aChkForceJ);

                    m_pCSGDataCtrl->Check_Fatigue(CheckElemK, jj, MembLcomD, MembR, ChkResD, DtrR, FatiCrResD);
                    FatiResMV_PR[jj] = ChkResD.pAASHTO->FatigueD;
                }
                double dStrDelMV_PR_I[4]= { 0.0, };
                double dStrDelMV_PR_J[4]= { 0.0, };
                dStrDelMV_PR_I[0] = FatiResMV_PR[0].Fat_LoadInduceD.df_TopFlng_Top;
                dStrDelMV_PR_I[1] = FatiResMV_PR[0].Fat_LoadInduceD.df_TopFlng_Bot;
                dStrDelMV_PR_I[2] = FatiResMV_PR[0].Fat_LoadInduceD.df_BotFlng_Top;
                dStrDelMV_PR_I[3] = FatiResMV_PR[0].Fat_LoadInduceD.df_BotFlng_Bot;

                dStrDelMV_PR_J[0] = FatiResMV_PR[1].Fat_LoadInduceD.df_TopFlng_Top;
                dStrDelMV_PR_J[1] = FatiResMV_PR[1].Fat_LoadInduceD.df_TopFlng_Bot;
                dStrDelMV_PR_J[2] = FatiResMV_PR[1].Fat_LoadInduceD.df_BotFlng_Top;
                dStrDelMV_PR_J[3] = FatiResMV_PR[1].Fat_LoadInduceD.df_BotFlng_Bot;

                dCapaN_Fati[0] = FatiResMV_PR[0].Fat_LoadInduceD.dF_TH;
                dCapaN_Fati[1] = FatiResMV_PR[0].Fat_LoadInduceD.dF_TH2;
                dCapaN_Fati[2] = FatiResMV_PR[0].Fat_LoadInduceD.dF_TH3;
                dCapaN_Fati[3] = FatiResMV_PR[0].Fat_LoadInduceD.dF_TH4;

                dConstantA[0] = FatiResMV_PR[0].Fat_LoadInduceD.dA;
                dConstantA[1] = FatiResMV_PR[0].Fat_LoadInduceD.dA2;
                dConstantA[2] = FatiResMV_PR[0].Fat_LoadInduceD.dA3;
                dConstantA[3] = FatiResMV_PR[0].Fat_LoadInduceD.dA4;

                CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D FatiResMV_AD[2];

                // AD. MV 계산. 

                CSGFORCOMP_ARRAY aChkForceAdI;
                aChkForceAdI.SetSize(nFatiConcSize);
                CSGFORCOMP_ARRAY aChkForceAdJ;
                aChkForceAdJ.SetSize(nFatiConcSize);

                CSG_MEMB_POSD_KSCE_LSD MembLcomMVAd = MembLcomD;
                for ( int jj=0; jj<nFatiConcSize; ++jj )
                {
                    int nMaxMinType=jj+1;
                    MembLcomMVAd.aChkFor[jj].Initialize();
                    CSG_FORCE_COMP &ComLcomF = MembLcomMVAd.aChkFor[jj];
                    aChkForceAdJ[jj].nLcomType = aChkForceAdI[jj].nLcomType = ComLcomF.nLcomType = 2;
                    aChkForceAdJ[jj].nLcomSub  = aChkForceAdI[jj].nLcomSub  = ComLcomF.nLcomSub  = 1;//m_pForcCtrl->Get_FatigueType(nDgnLcomId);
                    aChkForceAdJ[jj].nMax      = aChkForceAdI[jj].nMax      = ComLcomF.nMax      = nMaxMinType;

                    ConvertRatStlCalcMVChkForce(0, 1, nMaxMinType, ForceBlrdRcase, aChkForceAdI[jj]);
                    ConvertRatStlCalcMVChkForce(1, 1, nMaxMinType, ForceBlrdRcase, aChkForceAdJ[jj]);
                }

                CSG_MEMBPOS_RES_AASHTO_LRFD_D FatiAdMembRes = *MembR.pAASHTO;
                CSG_MEMB_RESULT_D FatiAdMembR;
                FatiAdMembR.pAASHTO = &FatiAdMembRes;
                for ( int jj=0; jj<2; ++jj )
                {
                    MembLcomMVAd.Memb.ChIf.nChkPos = jj;

                    if ( jj==0 ) MembLcomMVAd.aChkFor.Copy(aChkForceAdI);
                    else      MembLcomMVAd.aChkFor.Copy(aChkForceAdJ);

                    m_pCSGDataCtrl->Check_Fatigue(CheckElemK, jj, MembLcomMVAd, FatiAdMembR, ChkResD, DtrR, FatiCrResD);
                    FatiResMV_AD[jj] = ChkResD.pAASHTO->FatigueD;
                }
                double dStrDelMV_AD_I[4]= { 0.0, };
                double dStrDelMV_AD_J[4]= { 0.0, };
                dStrDelMV_AD_I[0] = FatiResMV_AD[0].Fat_LoadInduceD.df_TopFlng_Top;
                dStrDelMV_AD_I[1] = FatiResMV_AD[0].Fat_LoadInduceD.df_TopFlng_Bot;
                dStrDelMV_AD_I[2] = FatiResMV_AD[0].Fat_LoadInduceD.df_BotFlng_Top;
                dStrDelMV_AD_I[3] = FatiResMV_AD[0].Fat_LoadInduceD.df_BotFlng_Bot;

                dStrDelMV_AD_J[0] = FatiResMV_AD[1].Fat_LoadInduceD.df_TopFlng_Top;
                dStrDelMV_AD_J[1] = FatiResMV_AD[1].Fat_LoadInduceD.df_TopFlng_Bot;
                dStrDelMV_AD_J[2] = FatiResMV_AD[1].Fat_LoadInduceD.df_BotFlng_Top;
                dStrDelMV_AD_J[3] = FatiResMV_AD[1].Fat_LoadInduceD.df_BotFlng_Bot;

                int nChkLcomSize=1;
                MembLcomD.aChkFor.RemoveAll();
                MembLcomD.aChkFor.SetSize(nChkLcomSize);

                T_CPFP_D rCPFP;
                rCPFP.Initialize();
                if ( !m_pDoc->m_pAttrCtrl2->ExistCpfp(CheckElemK.first) ) continue;

                T_STRB_D MembForNC;
                for ( int k=0; k<cRAT_LCASE; k++ )
                {
                    m_pDoc->Get_BlrcLoadTypeData(k, bDC_Max, bDW_Max, bTemp_Max);

                    if ( IsSkipRatingCase(bDC_Max, bDW_Max, bTemp_Max, RlcsD)==FALSE ) continue;
                    T_RLCS_KEY uRlcskey = auRlcskey[k];
                    if ( uRlcskey==0 ) continue;

                    MembLcomD.aChkFor[0].Initialize();

                    int nFatiConcurr = 1;
                    //T_RLCS_KEY uRlcskey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr);

                    double dFactorDL = bDC_Max ? RlcsD.RateLoad[0].dFactor[0] : RlcsD.RateLoad[0].dFactor[1];
                    if ( bIsPLCB==TRUE )
                    {
                        if ( j<nPreForceLcomSize )
                        {
                            int nInx = bDC_Max ? 0 : 1;
                            ForcePreCompo.arForceRCase[j].ForceDCBe[nInx].GetForce(ForceNC);
                        }
                    }

                    int nRCDgnLcomNo = m_pForcCtrl->GetStlRCDgnLcomKey(cRAT_LCASE, cFORC_CON_DGN, j, k, 0);
                    if ( bIsPLCB==FALSE ) m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC, dFactorDL);

                    MembForNC.Initialize();
                    m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(TRUE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, aUblmElemK, &MembForNC, dFactorDL);

                    // UlmForce
                    MakeConverMemberForce(StctD, j, nGdLastStagK, nLastStepNo, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurr, aUblmElemK, MembForNC, MembLcomD);


                    T_RCFA_INF_LIFE_CASE RcfaInfC;
                    T_RCFA_FIN_LIFE_CASE RcfaFinC;

                    for ( int n=0; n<2; n++ )
                    {

                        if ( n==0 && nChkPosition==1 ) continue;
                        else if ( n==1 && nChkPosition==0 ) continue;

                        CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];

                        MembLcomD.Memb.ChIf.nChkPos = n;

                        ConvertRatStlCalcChkForce(n, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr, ForceNC, ForceBlrdRcase, ComLcomF);

                        double dRel_Locat = (n==0) ? BlrSpanD.dElemI_Rat : BlrSpanD.dElemJ_Rat;

                        Get_RatingFatigueLCaseForce(n, bDC_Max, bDW_Max, bTemp_Max, RlcsD, ForceBlrdRcase, RcfaInfC.FatiB[n].DRCFA);

                        //            
                        m_dEpsiC = dEpsiC[n];
                        m_dEpsiT = dEpsiT[n];
                        m_dDispC = dDispC[n];
                        m_dDispT = dDispT[n];
                        m_dKa    = dKa_f[n];
                        m_dK     = dK_f[n];

                        SetRcaseFatiResult(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr, bSpanInfo,
                            BlrSpanD.SpanK, BlrSpanD.iL_count, dRel_Locat, dPhi_CF, dPhi_SF, RcfaInfC.FatiB[n].DRCFA);

                        RcfaInfC.FatiB[n].InfLife.DiagnoTest[0].dEpsiC = m_pRpasD->nMeasurement == 0 ? m_dEpsiC : m_dDispC;
                        RcfaInfC.FatiB[n].InfLife.DiagnoTest[0].dEpsiT = m_pRpasD->nMeasurement == 0 ? m_dEpsiT : m_dDispT;
                        RcfaInfC.FatiB[n].InfLife.DiagnoTest[0].dKa    = m_dKa;
                        RcfaInfC.FatiB[n].InfLife.DiagnoTest[0].dK     = m_dK;
                        RcfaInfC.FatiB[n].InfLife.DiagnoTest[0].dKb    = m_dKb;

                        RcfaInfC.FatiB[n].InfLife.DiagnoTest[3] = RcfaInfC.FatiB[n].InfLife.DiagnoTest[2] = RcfaInfC.FatiB[n].InfLife.DiagnoTest[1] = RcfaInfC.FatiB[n].InfLife.DiagnoTest[0];

                        double *pStrDelPR = (n==0) ? dStrDelMV_PR_I : dStrDelMV_PR_J;
                        double *pStrDelAD = (n==0) ? dStrDelMV_AD_I : dStrDelMV_AD_J;

                        double *pStrFTH = dCapaN_Fati;

                        CheckInfiniteFatigueRatingAASHTO(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr,
                            MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF, dCapaN_Fati,
                            pStrDelPR, pStrDelAD,
                            RcfaInfC.FatiB[n], pStressI_RF, pStressJ_RF, pStressI_CADE, pStressJ_CADE,
                            RcfaInfD);

                        if ( RpasD.nFatigueLife == 1 )
                        {
                            RcfaFinC.FatiB[n].dA[0] = dConstantA[0];
                            RcfaFinC.FatiB[n].dA[1] = dConstantA[1];
                            RcfaFinC.FatiB[n].dA[2] = dConstantA[2];
                            RcfaFinC.FatiB[n].dA[3] = dConstantA[3];

                            Get_RatingFatigueLCaseForce(n, bDC_Max, bDW_Max, bTemp_Max, RlcsD, ForceBlrdRcase, RcfaFinC.FatiB[n].DRCFA);
                            SetRcaseFatiResult(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr, bSpanInfo,
                                BlrSpanD.SpanK, BlrSpanD.iL_count, dRel_Locat, dPhi_CF, dPhi_SF, RcfaFinC.FatiB[n].DRCFA);

                            CheckFiniteFatigueRatingAASHTO(CheckElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nFatiConcurr,
                                MembLcomD, *MembR.pAASHTO, RlcsD, dCapacityF, dCapaN_Fati,
                                pStrDelPR, pStrDelAD,
                                RcfaFinC.FatiB[n], pStressFINI_RF, pStressFINJ_RF, pStressFINI_CADE, pStressFINJ_CADE,
                                RcfaFinD);
                        }
                    }

                    m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlFatigueRatingINFResult(CheckElemK, uRlcskey, nFatiConcurr, RcfaInfC);
                    m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlFatigueRatingFINResult(CheckElemK, uRlcskey, nFatiConcurr, RcfaFinC);
                }
            }
            else ASSERT(0);
        } // j

		if ( vResult[enResultType::Design_Strength] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcfcResult_Design(CheckElemK, RcfcD_Design);
		if ( vResult[enResultType::Design_Strength] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcscResult_Design(CheckElemK, RcscD_Design);
		if ( vResult[enResultType::Design_Service]  ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcgsResult_Design(CheckElemK, RcgsD_Design);
		if ( vResult[enResultType::Legar_Strength] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcfcResult_Legal(CheckElemK, RcfcD_Legal);
		if ( vResult[enResultType::Legar_Strength] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcscResult_Legal(CheckElemK, RcscD_Legal);
		if ( vResult[enResultType::Legar_Service] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcgsResult_Legal(CheckElemK, RcgsD_Legal);
		if ( vResult[enResultType::Permit_Strength] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcfcResult_Permit(CheckElemK, RcfcD_Permit);
		if ( vResult[enResultType::Permit_Strength] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcscResult_Permit(CheckElemK, RcscD_Permit);
		if ( vResult[enResultType::Permit_Service] ) m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcgsResult_Permit(CheckElemK, RcgsD_Permit);

		if ( RpasD.bFatigue==TRUE )
		{
			m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcfaInfiniteResult(CheckElemK, RcfaInfD);
			m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRcfaFiniteResult(CheckElemK, RcfaFinD);
		}
    }
    //
    Check_RFminbyRatingCase19();

    if ( MembR.pAASHTO != NULL ) delete MembR.pAASHTO;

    if ( ChkResD.pAASHTO != NULL ) delete ChkResD.pAASHTO;

    return TRUE;
}


template<typename T1>
BOOL CalcReserveFactor(int nMovType, T1& ResB)
{
    BOOL bPosVal = (ResB.dSA >= 0.0); // 하중조합 기준으로,, (이전 : Special vehicle 기준으로..)
    double dFCa = bPosVal ? 1.0 : -1.0;
    double dFacRA = dFCa * ResB.dRA;
    double dA = DgnAbsDivide(ResB.dRA, ResB.dSA);
    ResB.dA = dA;
    BOOL bPositive = bPosVal;
    if ( bPosVal )
    {
        bPositive = (ResB.dS>=0.0);
    }
    else
    {
        bPositive = (ResB.dS<0.0);
    }

    if ( nMovType == 1 ) //Special인 경우
    {
        if ( !bPositive )
        {
            ResB.dPsi_ast = 0.0;
            ResB.dRF = dA;
        }
        else
        {
            double dnumer = dFacRA - (ResB.dSd);
            double ddenom = ResB.dS;
            ResB.dPsi_ast = DgnDivide(dnumer, ddenom);
            ResB.dRF = min(ResB.dPsi_ast, ResB.dA);

        }
    }
    else //Combined 인 경우
    {
        if ( !bPositive )
        {
            ResB.dPsi_ast = 0.0;
            ResB.dPsi = 0.0;
            ResB.dRF = dA;
        }
        else
        {
            double dnumer = dFacRA - (ResB.dSd + ResB.dSst);
            double ddenom = ResB.dS;
            ResB.dPsi = DgnDivide(dnumer, ddenom);
            dnumer = dFacRA - ResB.dSd;
            ResB.dPsi_ast = DgnDivide(dnumer, ddenom);
            double dRF_min = min(ResB.dPsi, ResB.dPsi_ast);
            ResB.dRF = min(dRF_min, ResB.dA);
        }
    }
    return TRUE;
}

template<typename T1>
BOOL CalcReserveFactorCombined(int nMovType, T1& ResB)
{
    BOOL bPosVal = (ResB.dSA >= 0.0); // Special vehicle 기준으로..
    double dFCa = bPosVal ? 1.0 : -1.0;
    double dFacRA = dFCa * ResB.dRA;
    double dA = DgnAbsDivide(ResB.dRA, ResB.dSA);
    ResB.dA = dA;
    BOOL bPositive = bPosVal;
    if ( bPosVal )
    {
        bPositive = (ResB.dS>=0.0);
    }
    else
    {
        bPositive = (ResB.dS<0.0);
    }

    if ( nMovType == 1 ) //Special인 경우
    {
        if ( !bPositive )
        {
            ResB.dPsi_ast = 0.0;
            ResB.dRF = dA;
        }
        else
        {
            double dnumer = dFacRA - (ResB.dSd);
            double ddenom = ResB.dS;
            ResB.dPsi_ast = DgnDivide(dnumer, ddenom);
            ResB.dRF = min(ResB.dPsi_ast, ResB.dA);
        }
    }
    else //Combined 인 경우
    {
        if ( !bPositive )
        {
            ResB.dPsi_ast = 0.0;
            ResB.dPsi = 0.0;
            ResB.dRF = dA;
        }
        else
        {
            double dnumer = dFacRA - (ResB.dSd_Sst); // Sd + Sst
            double ddenom = ResB.dS;
            ResB.dPsi = DgnDivide(dnumer, ddenom);
            dnumer = dFacRA - ResB.dSd;
            ResB.dPsi_ast = DgnDivide(dnumer, ddenom);
            double dRF_min = min(ResB.dPsi, ResB.dPsi_ast);
            ResB.dRF = min(dRF_min, ResB.dA);
        }
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_CS454CSGBridgeAssessment()
{
    Make_RatingDgnData();

    int nTotalElement = m_arElemK.GetSize();

    T_RPSS_D RpssD; RpssD.Initialize();
    m_pDoc->m_pAttrCtrl2->GetRpss(RpssD);
    
    double dFc = RpssD.dConditionFactor; // condition factor - global parameter

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);

    FORCE_ASBD_ELEM ForceAsbdElem;
    FORCE_BLRD_ELEM ForcePreCompo;
    BLR_SPAN_D BlrSpanD;

    m_pDataCtrl->Calc_LengthPosition4Span(m_arElemK);

    // Pre-Combined Composite Bridge
    BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb();

    CSG_EFF_BREADTH_RATIO_SPAN_D EffBreadthR;
    CSG_C4_VALUE_TABLE_CS457 C4table;


    CSG_MEMB_RESULT_D      MembR;
    CSG_CHECK_RESULT_CODES ChkResD;

    switch (m_nDgnCode)
    {
    case STL_CS454_20:
    {
        MembR.pCS457 = new CSG_MEMBPOS_RES_CS457_D;

        ChkResD.pCS457 = new CSG_CHECK_RES_CS457;
        break;
    }
    case STL_NR_GN_CIV_025_06:
    {
        MembR.pNRGN = new CSG_MEMBPOS_RES_NR_GN_CIV_025_D;

        ChkResD.pNRGN = new CSG_CHECK_RES_NR_GN_CIV_025;
        break;
    }
    default:
        ASSERT(0);
        break;
    }

    auto _L_RF4NR = [&](const T_ASCA_RAT_RAIL_BS_B& rRFData) -> double
        {
            if (LT0(rRFData.dSBSU) && LT0(rRFData.dDBSU))
            {
                return max(rRFData.dSBSU, rRFData.dDBSU);
            }
            else
            {
                return min(rRFData.dSBSU, rRFData.dDBSU);
            }
            return min(rRFData.dSBSU, rRFData.dDBSU);
        };

    BOOL bULS = (RpssD.bUlsFlexure || RpssD.bUlsShear || RpssD.bUlsFST || RpssD.bUlsLgShear);
    BOOL bSLS = (RpssD.bSlsStress || RpssD.bSlsLgShear);

    DgnBeamPairK nCurUblmK(EN_EL_BEAM, 0);
    ArrElemPairKey aUblmElemK;

    BOOL bCantilever=FALSE;
    T_RROS_D RrosD;
	T_RROV_D RrovD;
    T_UFMT_D UfmtD;
    for ( int i=0; i<nTotalElement; ++i )
    {

        CSG_MEMB_POSD_KSCE_LSD MembLcomD;
        ConvertRatingGlobalData(RpssD, MembLcomD.Memb.Csgd);

        int nMaxMin=0;
        auto CheckElemK = m_arElemK[i];

        T_ELEM_D ElemD; ElemD.Initialize();

        UfmtD.Initialize();
        RrosD.Initialize();
        int nMatlK = 0;
        T_SRAS_D SrasD; SrasD.Initialize(); //검토위치
        BOOL bSrasKey = FALSE; 

        if (CheckElemK.second == EN_EL_BEAM)
        {
            m_pDoc->m_pAttrCtrl->GetUfmt(CheckElemK.first, UfmtD);
            m_bbAppAFK = m_pDoc->m_pAttrCtrl2->GetRros(CheckElemK.first, RrosD); // Adjustment Factor, K 적용 여부 

            if (!m_pDoc->m_pPostCtrl->GetElemPost(CheckElemK.first, ElemD)) continue;
            nMatlK = ElemD.elmat;			
            bSrasKey = m_pDoc->m_pAttrCtrl2->GetSras(CheckElemK.first, SrasD);
        }
        else if (CheckElemK.second == EN_EL_VBEAM)
        {
            RrovD.Initialize();
            m_bbAppAFK = m_pDoc->m_pAttrCtrl2->GetRrov(CheckElemK.first, RrovD);
            RrosD=RrovD;

            T_MATL_K SlabMatlK = 0, GirderMatlK = 0;
            m_pDoc->m_pAttrCtrl2->GetSecvMatlK(CheckElemK.first, SlabMatlK, GirderMatlK);
            nMatlK = GirderMatlK;

            T_SRCV_D SrcvD; SrcvD.Initialize();
            bSrasKey = m_pDoc->m_pAttrCtrl2->GetSrcv(CheckElemK.first, SrcvD);
            SrasD=SrcvD;

        }
        else ASSERT(0);

        m_bAutoAFK = (RrosD.nCalcType == 0);

        m_dKb = m_bbAppAFK ? RrosD.dImpactFactor : 1.0;
        MembLcomD.Memb.ChIf.bCantilever = bCantilever ? true : false;
		MembLcomD.Memb.ChIf.nBeamType = CheckElemK.second;

        BlrSpanD.Initialize();
        BOOL bSpanInfo=FALSE;
        if ( m_pDataCtrl->Get_ElemSpanInfo(CheckElemK, BlrSpanD) )
        {
            bSpanInfo = TRUE;
        }                

        T_STAG_K nGdLastStagK=0;
        int nLastStepNo=0;
        m_pForcCtrl->GetGirderLastStageKey(CheckElemK, nGdLastStagK, nLastStepNo);
        int nForceBeforeType = 1;
                      

        if (MembR.pCS457 != nullptr)
        {
            MembR.pCS457->Initialize(); // 계산 구조체~
            MembR.pCS457->MembR[0].pC4 = &C4table;
            MembR.pCS457->MembR[1].pC4 = &C4table;
        }
        if (MembR.pNRGN != nullptr)
        {
            MembR.pNRGN->Initialize(); // 계산 구조체~
            MembR.pNRGN->MembR[0].pC4 = &C4table;
            MembR.pNRGN->MembR[1].pC4 = &C4table;
        }

        if ( !m_pCSGDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl) ) MembLcomD.Memb.Matl.Initialize();

        double dVuFactor = 1.0;
        m_pCSGDataCtrl->GetCsgSectionSupport(CheckElemK, MembLcomD);
        if (IsSectTypeWith2Web(MembLcomD.Memb.PosD[0].Sect.SectInfo.nStype))
        {
            dVuFactor = 0.5;
        }

        m_pCSGDataCtrl->Get_CsgMembInData(CheckElemK, MembLcomD);

        MembLcomD.Memb.ChIf.nChkPos = 0;
        //MembLcomD.Memb.ChIf.bCurved = bCurved;

        switch (m_nDgnCode)
        {
        case STL_NR_GN_CIV_025_06:
        {
            switch (UfmtD.nMemberType)
            {
            case 0: MembLcomD.Memb.ChIf.enMembType = EN_Chk_Memb_Main_Girder; break;
            case 1: MembLcomD.Memb.ChIf.enMembType = EN_Chk_Memb_Cross_Girder; break;
            case 2: MembLcomD.Memb.ChIf.enMembType = EN_Chk_Memb_Rail_Bearer; break;
            default: ASSERT(0); break;
            }
			MembLcomD.Memb.ChIf.enSpan = EN_Simply_supported_beam; // ???  

            break;
        }
        default:
            break;
        }


        T_SPAN_K CurSpanK=0;
        ElemPairK nElemI = CheckElemK, nElemM = CheckElemK, nElemJ = CheckElemK;
        int nPosI=0;
        int nPosM=0; // 임시, 찾아야함. 
        int nPosJ=1;

        BOOL bChkUblm=FALSE;
        BOOL bCantilever=FALSE;
        BOOL bMakeMembFor=FALSE;
        BOOL b2SideBgg=FALSE;
        DgnBeamPairK nUblmK(EN_EL_BEAM, 0);

        m_pDataCtrl->GetUbLengthElemInfo(CheckElemK, nUblmK, nCurUblmK, nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, bChkUblm, bCantilever, bMakeMembFor, b2SideBgg, aUblmElemK);
        MembLcomD.bChkUlmf = bChkUblm;

        T_KFAC_D KFacD; KFacD.Initialize();
        BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(CheckElemK, KFacD);
        m_pCSGDataCtrl->Get_UnbracedLengthData(CheckElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);
        //
        m_pCSGDataCtrl->Get_CsgUlmSection(nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, MembLcomD.Memb.Ulms);

        MembLcomD.Memb.ChIf.bCantilever = bCantilever;

        m_pCSGDataCtrl->MakeMemberResultData(CheckElemK, MembLcomD, MembR);

        // 시공중 거더 Force
        _DGN_FORC_CRC ForceNC;
        _DGN_FORC_CRC_EX ForceNC_EX;

        int nLcomNum = m_pForcCtrl->Get_LcomDataCount();

        ADGNFORCE aPreForceLcom;
        if ( bIsPLCB==TRUE )
        {
            //m_pForcCtrl->Get_BeforeForce4PreCompo(CheckElemK, aPreForceLcom, nLcomNum);
            const auto& iter = m_pForcCtrl->m_arBlrdPreCompoForecElem.find(CheckElemK);
            if (iter != m_pForcCtrl->m_arBlrdPreCompoForecElem.end())
                ForcePreCompo = iter->second;
        }

        int nPreForceLcomSize = ForcePreCompo.arForceRCase.GetSize();

        
        ForceAsbdElem.Initialize();
		const auto& itForceAsbd = m_pForcCtrl->m_arBlrdForceElemBD.find(CheckElemK);
        if (itForceAsbd == m_pForcCtrl->m_arBlrdForceElemBD.end()) continue;
		ForceAsbdElem = itForceAsbd->second;

        CArray<FORCE_RCASE_BD, FORCE_RCASE_BD> arForceRCase;
        arForceRCase.RemoveAll();
        arForceRCase.Copy(ForceAsbdElem.arForceRCase);

        double dIendPosiM_RF = DBL_MAX;
        double dIendNegaM_RF = DBL_MAX;
        double dJendPosiM_RF = DBL_MAX;
        double dJendNegaM_RF = DBL_MAX;

        T_ASFC_CSG_BS_CASE PosiM_Asfc; // i, j
        T_ASFC_CSG_BS_CASE NegaM_Asfc;
        T_ASSC_CSG_BS_CASE VRFmin;
        T_ASCB_CSG_BS_CASE CombRFmax;
        T_ASFG_CSG_BS_CASE CompFlgMin;
        T_ASFG_CSG_BS_CASE TensFlgMin;
        T_ASFS_CSG_BS_CASE CompFlgStfn;
        T_ASWY_CSG_BS_D    WebYCrPos;
        T_ASWY_CSG_BS_D    WebYCrNeg;
        T_ASWB_CSG_BS_D    WebBCrPos;
        T_ASWB_CSG_BS_D    WebBCrNeg;
        T_ASWS_CSG_BS_D    WebSCrPos;
        T_ASWS_CSG_BS_D    WebSCrNeg;
        T_ASLS_CSG_BS_CASE LShrCr;
        T_ASSB_CSG_BS_D    SlabCr;
        T_ASBS_SG_BS_CASE  TsWebCr;
        T_ASBS_SG_BS_CASE  TsStiffCr;
        T_ASBS_SG_BS_CASE  TsBuckCr;
        T_ASSF_SG_BS_CASE  FastenerCr;

        T_ASSL_CSG_BS_CASE ServCStrCr;
        T_ASSL_CSG_BS_CASE ServTStrCr;
        T_ASLS_CSG_BS_CASE LShrSLSCr;

        T_ASRS_CSG_BS_D RFSumD;       // Reserve Factor/Verification Summary
        T_ASRS_LS_CSG_BS_D RFLSSumD;  // Reserve Factor Summary Table (Beams with L. Stiffeners)


        double dFC_Pmin[2] = { DBL_MAX, DBL_MAX };
        double dFC_Nmin[2] = { DBL_MAX, DBL_MAX };
        double dVRF_min[2] = { DBL_MAX, DBL_MAX };
        double dCombMV_max[2] = { DBL_MAX, DBL_MAX };
        double dCompFlg_RFmin[2] = { DBL_MAX, DBL_MAX };
        double dTensFlg_RFmin[2] = { DBL_MAX, DBL_MAX };
        double dWebYCrPos[2] = { DBL_MAX, DBL_MAX };
        double dWebYCrNeg[2] = { DBL_MAX, DBL_MAX };
        double dWebBCrPos[2] = { DBL_MAX, DBL_MAX };
        double dWebBCrNeg[2] = { DBL_MAX, DBL_MAX };
        double dWebSCrPos[2] = { DBL_MAX, DBL_MAX };
        double dWebSCrNeg[2] = { DBL_MAX, DBL_MAX };
        double dLShearCr[2] = { -DBL_MAX, -DBL_MAX };
        double dSlabCr[2] = { DBL_MAX, DBL_MAX };
        double dCompStrCr[2] = { DBL_MAX, DBL_MAX };
        double dTensStrCr[2] = { DBL_MAX, DBL_MAX };
        double dLShearSLSCr[2] = { -DBL_MAX, -DBL_MAX };
        double dTSWebCr[2] = { DBL_MAX, DBL_MAX };
        double dTSStiffCr[2] = { DBL_MAX, DBL_MAX };
        double dTSBuckCr[2] = { DBL_MAX, DBL_MAX };
        double dFastenerCr[2] = { DBL_MAX, DBL_MAX };

        T_STRB_D MembForNC;
        double dFactorDL = 1.0; // ??? 어떤 값으로 연결
        int nRcaseNum = arForceRCase.GetSize();

        for ( int j=0; j<nRcaseNum; ++j )
        {
            int nRcaseNo = j+1;
            int nOrgRcaseNo=0, nRatingCase=0, nRcaseTypeDL=0, nRcaseTypeLL=0;
            if ( !m_pForcCtrl->Get_RcaseType(nRcaseNo, nOrgRcaseNo, nRatingCase, nRcaseTypeDL, nRcaseTypeLL) ) ASSERT(0);
            //if ( nRatingCase==1 ) continue;

            T_ALCS_D AlcsD;
            m_pDoc->m_pAttrCtrl2->GetAlcs(nOrgRcaseNo, AlcsD); // Rating case

            BOOL bChkFlex[4]= { FALSE, FALSE, FALSE, FALSE };
            int nConcurrent = m_pDataCtrl->Get_ConCurrType(nRcaseTypeDL, nRcaseTypeLL);

            int nState = 0;

            FORCE_RCASE_BD ForceRcaseBD = arForceRCase[j];
            FORCE_BLRD_RCASE ForceBlrdRcase;
            ForceRcaseBD.ConvertBlrd(ForceBlrdRcase);

            MembLcomD.aChkFor.RemoveAll();
            MembLcomD.aUlmf.RemoveAll();
            int nChkLcomSize=1;
            int nChkLcomSizeUlmf = 6;
            MembLcomD.aUlmf.SetSize(nChkLcomSizeUlmf);
            MembLcomD.aChkFor.SetSize(nChkLcomSize);

            if ( bIsPLCB==TRUE )
            {
                if ( j<nPreForceLcomSize )
                {
                    int nInx = 0; // bDC_Max ? 0 : 1;
                    ForcePreCompo.arForceRCase[j].ForceDCBe[nInx].GetForce(ForceNC);                    
                }
            }

            MembLcomD.aChkFor[0].Initialize();
            CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];
            if ( AlcsD.nLimitState==0 ) ComLcomF.nLcomType = 0;
            else if ( AlcsD.nLimitState==1 ) // Service
            {
                ComLcomF.nLcomType = 1;
                //ComLcomF.nLcomSub  = nServLcomType;
            }
            ComLcomF.nMax = nConcurrent;
			ComLcomF.enLLType = (AlcsD.nLiveType == 0) ? EN_NR_LIVE_LOAD_RAI : EN_NR_LIVE_LOAD_Wagon;
            dFactorDL = GetDeadLoadFactor4CS454(AlcsD);

            int nRCDgnLcomNo = nRcaseNo; 
            if ( bIsPLCB==FALSE )
            {
                m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC);
                m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM_EX(FALSE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC_EX);
            }

            MembForNC.Initialize();
            m_pForcCtrl->Get_GirderMembForce4CSLoadOfLCOM(FALSE, StctD, CheckElemK, nGdLastStagK, nLastStepNo, nRCDgnLcomNo, nForceBeforeType, aUblmElemK, &MembForNC, dFactorDL);

            // UlmForce
            BOOL bDC_Max(TRUE), bDW_Max(TRUE), bTemp_Max(TRUE);
            MakeConverMemberForce(StctD, j, nGdLastStagK, nLastStepNo, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurrent, aUblmElemK, MembForNC, MembLcomD);
            MakePanelForce(StctD, nRcaseNo, CheckElemK, nGdLastStagK, nLastStepNo, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurrent, ForceNC_EX, MembLcomD);
            MakePanelForceMax(StctD, nRcaseNo, CheckElemK, nGdLastStagK, nLastStepNo, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurrent, MembLcomD);

            T_ASFC_CSG_BS_CASE AsfcCase;
            T_ASSC_CSG_BS_CASE AsscCase;
            T_ASCB_CSG_BS_CASE AscbCase;
            T_ASLS_CSG_BS_CASE AslsCase;
            T_ASFG_CSG_BS_CASE AsfgCase;  // Flange - Steel Flange Reserve Factor Table (Beams with L. Stiffeners)
            T_ASFG_CSG_BS_CASE AsfgTenCase;
            T_ASFS_CSG_BS_CASE AsfsCase;  // Web - Steel Flange Reserve Factor Table (Beams with L. Stiffeners)
            T_ASWY_CSG_BS_D    AswyCase;  // Web yielding - Web Panel Reserve Factor Table (Beams with L. Stiffeners)
            T_ASWB_CSG_BS_D    AswbCase;  // Web buckling - Web Panel Reserve Factor Table (Beams with L. Stiffeners)
            T_ASWS_CSG_BS_D    AswsCase;  // Web stiffener - Web Panel Reserve Factor Table (Beams with L. Stiffeners)
            T_ASSB_CSG_BS_D    AssbCase;  // Slab Reserve Factor Table (Beams with L. Stiffeners)
            T_ASBS_SG_BS_CASE  AsbsCase;
            T_ASSF_SG_BS_CASE  AssfCase;
             //
            T_ASSL_CSG_BS_CASE AsslCase;  // SLS
            T_ASLS_CSG_BS_CASE AslsSLSCase;
            //            

            for ( int k=0; k<2; ++k ) // i, j
            {
                BOOL bCheck = Get_ChkElemFlag4RatingCS454(bSrasKey, SrasD, k);
                T_ASFC_CSG_BS_BASE& AsfcB = AsfcCase.AsfcB[k];

                AsfcB.ComD.Set_Param(bCheck, CheckElemK.first, CheckElemK.second, k, nOrgRcaseNo, nConcurrent, dFc);
				const CSG_DATA_POSD& PosD = MembLcomD.Memb.PosD[k];
                BOOL bSupportNode = PosD.bSupprotNode;
                BOOL bInterSupportNode = PosD.bInteriorSupport;
				AsfcB.ComD.enSupport = m_pCSGDataCtrl->GetSupportType(bSupportNode, bInterSupportNode);

                // Sa* - Assessment Load Effect total Sa* = Sd* + Sha* + Ssv*
                // Sd* - Assessment Load Effect; Dead and superimposed dead loads
                // Sha* - Assessment Load Effect; Standard Vehicle
                // S* - Assessment Load Effect; Special Vehicle

                AsscCase.AsscB[k].ComD = AsfcB.ComD;
                AscbCase.AscbB[k].ComD = AsfcB.ComD; 
                AslsCase.AslsB[k].ComD = AsfcB.ComD;
                AsfgCase.AsfgB[k].ComD = AsfcB.ComD;
                AsfgTenCase.AsfgB[k].ComD = AsfcB.ComD;
                AsfsCase.AsfsB[k].ComD = AsfcB.ComD;
                AsfsCase.AsfsFlgB[k].ComD = AsfcB.ComD;
                AswyCase.AswyB[k].ComD = AsfcB.ComD;
                AswbCase.AswbB[k].ComD = AsfcB.ComD;
                AswsCase.AswsB[k].ComD = AsfcB.ComD;
                AssbCase.AssbB[k].RFData.ComD = AsfcB.ComD;
                 //
                AsslCase.AsslB[k].ComD = AsfcB.ComD;
                AslsSLSCase.AslsB[k].ComD = AsfcB.ComD;
				AsbsCase.AsbsB[k].ComD = AsfcB.ComD;
                AssfCase.AssfB[k].ComD = AsfcB.ComD;
                

                if ( bCheck )
                {
                    int nIdx = GetIndex4MomentRes(AsfcB.dSA, k);

                    bChkFlex[nIdx] = TRUE;
					AsfcCase.AsfcB[k].ComD.bChk = RpssD.bUlsFlexure ? true : false;
                    AsscCase.AsscB[k].ComD.bChk = RpssD.bUlsShear ? true : false;
					AsbsCase.AsbsB[k].ComD.bChk = RpssD.bUlsIntBearStiff ? true : false;
                    AssfCase.AssfB[k].ComD.bChk = RpssD.bUlsLgShear ? true : false;
                }

            }            

            if ( bULS==TRUE && AlcsD.nLimitState==0 ) // 
            {
                for ( int k=0; k<2; ++k )
                {
                    BOOL bCheck = Get_ChkElemFlag4RatingCS454(bSrasKey, SrasD, k);
                    if ( !bCheck ) continue;
                    
                    CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];

                    MembLcomD.Memb.ChIf.nChkPos = k;

                    int nConcurrentId = 1; // FORCE_BLRD_RCASE 의 ConvertBlrd를 하기 때문에 1로 고정
                    ConvertRatStlCalcChkForce(k, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurrentId, ForceNC, ForceBlrdRcase, ComLcomF);

                    double dMySA  = ComLcomF.ForLC.My();
                    double dMySd  = ComLcomF.ForNC.My() + ComLcomF.ForLT.My();
                    double dMySst = ComLcomF.ForST.My();
                    double dMyS   = ComLcomF.ForMV.My();
                    double dVzSA  = ComLcomF.ForLC.Fz();
                    double dVzSd  = ComLcomF.ForNC.Fz() + ComLcomF.ForLT.Fz();
                    double dVzSst = ComLcomF.ForST.Fz();
                    double dVzS   = ComLcomF.ForMV.Fz();
                    if (m_nDgnCode == STL_NR_GN_CIV_025_06)
                    {
                        dMySst = ComLcomF.ForSV.My();
                        dVzSst = ComLcomF.ForSV.Fz();
                    }

                    T_REAC_D DLre; DLre.Initialize();
                    T_REAC_D SLLre; SLLre.Initialize();
                    T_REAC_D DLLre; DLLre.Initialize();
                    MakeReactionRCase(AlcsD, CheckElemK, k, nConcurrent, DLre, SLLre, DLLre, MembLcomD);

                    T_CSGF_MEMB ComFor;
                    m_pCSGDataCtrl->ConvertCsgEngToDgnForce(MembLcomD, ComFor);

                    T_ASFC_CSG_BS_BASE& AsfcB = AsfcCase.AsfcB[k];
                    AsfcB.ComD.ForD = ComFor;
                    T_ASSC_CSG_BS_BASE& AsscB = AsscCase.AsscB[k];
                    AsscB.ComD.ForD = ComFor;
                    T_ASCB_CSG_BS_BASE& AscbB = AscbCase.AscbB[k]; 
                    AscbB.ComD.ForD = ComFor;
                    m_pCSGDataCtrl->ConvertCSGDCOMF(MembLcomD.PanelM, AscbB.ForcePanelM);
                    m_pCSGDataCtrl->ConvertCSGDCOMF(MembLcomD.PanelV, AscbB.ForcePanelV);

                    T_ASLS_CSG_BS_BASE& AslsB = AslsCase.AslsB[k];
                    AslsB.ComD.ForD = ComFor;
                    T_ASFG_CSG_BS_BASE& AsfgB = AsfgCase.AsfgB[k];
                    AsfgB.ComD.ForD = ComFor;
                    T_ASFG_CSG_BS_BASE& AsfgTenB = AsfgTenCase.AsfgB[k];
                    AsfgTenB.ComD.ForD = ComFor;
                    T_ASFS_CSG_BS_BASE& AsfsB = AsfsCase.AsfsB[k];
                    AsfsB.ComD.ForD = ComFor;
                    T_ASFS_CSG_BS_BASE& AsfsFlgB = AsfsCase.AsfsFlgB[k];
                    AsfsFlgB.ComD.ForD = ComFor;
                    T_ASWY_CSG_BS_BASE& AswyB = AswyCase.AswyB[k];
                    AswyB.ComD.ForD = ComFor;
                    for ( int pp=0; pp<2; ++pp )
                    {
                        m_pCSGDataCtrl->ConvertCSGDCOMF(MembLcomD.Panelij[pp], AswyB.Panelij[pp]);
                    }              

                    T_ASWB_CSG_BS_BASE& AswbB = AswbCase.AswbB[k];
                    AswbB.ComD.ForD = ComFor;
                    T_ASWS_CSG_BS_BASE& AswsB = AswsCase.AswsB[k];
                    AswsB.ComD.ForD = ComFor;
                    T_ASSB_CSG_BS_BASE& AssbB = AssbCase.AssbB[k];
                    AssbB.RFData.ComD.ForD = ComFor;
                    T_ASBS_SG_BS_BASE& AsbsB = AsbsCase.AsbsB[k];
					AsbsB.ComD.ForD = ComFor;
                    T_ASSF_SG_BS_BASE& AssfB = AssfCase.AssfB[k];
                    AssfB.ComD.ForD = ComFor;

                    AsfcB.SetForce(dMySA, dMySd, dMySst, dMyS);
                    AsscB.SetForce(dVzSA, dVzSd, dVzSst, dVzS);
                    AscbB.SetForce(dMySA, dMySd, dMySst, dMyS);
                    //AslsB.SetForce(dSA, dSd, dSst, dS);

                    T_ASSM_ULS_CSG_BS_BASE UlsB;
                    UlsB.SetPtr(&AsfcB, &AsscB, &AscbB, &AslsB, &AsfgB, &AsfgTenB,
                        &AsfsB, &AsfsFlgB, &AswyB, &AswbB, &AswsB, &AssbB, &AsbsB, &AssfB);

                    CSG_STRN_RES_CS457 StrnResD;
                    CSG_SCON_RES_CS457 SconResD;

                    CSG_CHECK_RES_CS457 RsltB;
                    RsltB.pStrnD = &StrnResD;
                    RsltB.pSconD = &SconResD;

                    CheckStrengthRCaseAssessment(CheckElemK, k, AlcsD.nMovType, MembLcomD, MembR, RsltB, UlsB);
                    //
                    CheckShearConnectorRCaseCS454(CheckElemK, k, AlcsD.nMovType, MembLcomD, *MembR.pCS457, RsltB, AslsB);
                    BOOL bPositiveM = IsPositiveM(dMyS);
                    
                    if (m_nDgnCode == STL_CS454_20) // 일단 이렇게...
                    {
                        if (bPositiveM)
                        {
                            if (AsfcB.dRF < dFC_Pmin[k])
                            {
                                if (AsfcB.dRF < 0.0 && dFC_Pmin[k] < 0.0)
                                {
                                    if (AsfcB.dRF > dFC_Pmin[k])
                                    {
                                        PosiM_Asfc.AsfcB[k] = AsfcB;
                                        dFC_Pmin[k] = AsfcB.dRF;
                                    }
                                }
                                else
                                {
                                    PosiM_Asfc.AsfcB[k] = AsfcB;
                                    dFC_Pmin[k] = AsfcB.dRF;
                                }
                            }
                        }
                        if (!bPositiveM)
                        {
                            if (AsfcB.dRF < dFC_Nmin[k])
                            {
                                if (AsfcB.dRF < 0.0 && dFC_Nmin[k] < 0.0)
                                {
                                    if (AsfcB.dRF > dFC_Nmin[k])
                                    {
                                        NegaM_Asfc.AsfcB[k] = AsfcB;
                                        dFC_Nmin[k] = AsfcB.dRF;
                                    }
                                }
                                else
                                {
                                    NegaM_Asfc.AsfcB[k] = AsfcB;
                                    dFC_Nmin[k] = AsfcB.dRF;
                                }
                            }
                        }
                        if (AsscB.dRF < dVRF_min[k])
                        {
                            if (AsscB.dRF < 0.0 && dVRF_min[k] < 0.0)
                            {
                                if (AsscB.dRF > dVRF_min[k])
                                {
                                    VRFmin.AsscB[k] = AsscB;
                                    dVRF_min[k] = AsscB.dRF;
                                }
                            }
                            else
                            {
                                VRFmin.AsscB[k] = AsscB;
                                dVRF_min[k] = AsscB.dRF;
                            }
                        }
                    }
                    else if (m_nDgnCode == STL_NR_GN_CIV_025_06)
                    {
                        double dBSUb = _L_RF4NR(AsfcB.RatPa);
                        if (bPositiveM)
                        {
                            if (IsCriticalValue(dBSUb, dFC_Pmin[k]))
                            {
                                if (dBSUb < 0.0 && dFC_Pmin[k] < 0.0)
                                {
                                    if (dBSUb > dFC_Pmin[k])
                                    {
                                        PosiM_Asfc.AsfcB[k] = AsfcB;
                                        dFC_Pmin[k] = dBSUb;
                                    }
                                }
                                else
                                {
                                    PosiM_Asfc.AsfcB[k] = AsfcB;
                                    dFC_Pmin[k] = dBSUb;
                                }
                            }
                        }
                        if (!bPositiveM)
                        {
                            if (IsCriticalValue(dBSUb, dFC_Nmin[k]))
                            {
                                if (dBSUb < 0.0 && dFC_Nmin[k] < 0.0)
                                {
                                    if (dBSUb > dFC_Nmin[k])
                                    {
                                        NegaM_Asfc.AsfcB[k] = AsfcB;
                                        dFC_Nmin[k] = dBSUb;
                                    }
                                }
                                else
                                {
                                    NegaM_Asfc.AsfcB[k] = AsfcB;
                                    dFC_Nmin[k] = dBSUb;
                                }
                            }
                        }

                        double dBSUs = _L_RF4NR(AsscB.RatPa);
                        if (IsCriticalValue(dBSUs, dVRF_min[k]))
                        {
                            if (dBSUs < 0.0 && dVRF_min[k] < 0.0)
                            {
                                if (dBSUs > dVRF_min[k])
                                {
                                    VRFmin.AsscB[k] = AsscB;
                                    dVRF_min[k] = dBSUs;
                                }
                            }
                            else
                            {
                                VRFmin.AsscB[k] = AsscB;
                                dVRF_min[k] = dBSUs;
                            }
                        }
                    }

                    if ( AscbB.dA < dCombMV_max[k] ) // dA가 작은 값으로.
                    {
                        CombRFmax.AscbB[k] = AscbB;
                        dCombMV_max[k] = AscbB.dA;
                    }
                    if ( AsfgB.dRF < dCompFlg_RFmin[k] )
                    {
                        if ( AsfgB.dRF<0.0 && dCompFlg_RFmin[k]<0.0 )
                        {
                            if ( AsfgB.dRF > dCompFlg_RFmin[k] )
                            {
                                CompFlgMin.AsfgB[k] = AsfgB;
                                CompFlgStfn.AsfsB[k] = AsfsB;
                                dCompFlg_RFmin[k] = AsfgB.dRF;
                            }
                        }
                        else
                        {
                            CompFlgMin.AsfgB[k] = AsfgB;
                            CompFlgStfn.AsfsB[k] = AsfsB;
                            dCompFlg_RFmin[k] = AsfgB.dRF;
                        }
                    }
                    if ( AsfgTenB.dRF < dTensFlg_RFmin[k] )
                    {
                        if ( AsfgTenB.dRF<0.0 && dTensFlg_RFmin[k]<0.0 )
                        {
                            if ( AsfgTenB.dRF > dTensFlg_RFmin[k] )
                            {
                                TensFlgMin.AsfgB[k] = AsfgTenB;
                                dTensFlg_RFmin[k] = AsfgTenB.dRF;
                            }
                        }
                        else
                        {
                            TensFlgMin.AsfgB[k] = AsfgTenB;
                            dTensFlg_RFmin[k] = AsfgTenB.dRF;
                        }
                    }

                    if ( bPositiveM )
                    {
                        if ( AswyB.dRF < dWebYCrPos[k] )
                        {
                            if ( AswyB.dRF<0.0 && dWebYCrPos[k]<0.0 )
                            {
                                if ( AswyB.dRF > dWebYCrPos[k] )
                                {
                                    WebYCrPos.AswyB[k] = AswyB;
                                    dWebYCrPos[k] = AswyB.dRF;
                                    // Web Cr과 같이.. 나중에 분리 되면 밑에 주석 다시 고려
                                    WebBCrPos.AswbB[k] = AswbB;
                                    WebSCrPos.AswsB[k] = AswsB;
                                }
                            }
                            else
                            {
                                WebYCrPos.AswyB[k] = AswyB;
                                dWebYCrPos[k] = AswyB.dRF;

                                WebBCrPos.AswbB[k] = AswbB;
                                WebSCrPos.AswsB[k] = AswsB;
                            }
                        }
                    }
                    else
                    {
                        if ( AswyB.dRF < dWebYCrNeg[k] )
                        {
                            if ( AswyB.dRF<0.0 && dWebYCrNeg[k]<0.0 )
                            {
                                if ( AswyB.dRF > dWebYCrNeg[k] )
                                {
                                    WebYCrNeg.AswyB[k] = AswyB;
                                    dWebYCrNeg[k] = AswyB.dRF;

                                    WebBCrNeg.AswbB[k] = AswbB;
                                    WebSCrNeg.AswsB[k] = AswsB;
                                }
                            }
                            else
                            {
                                WebYCrNeg.AswyB[k] = AswyB;
                                dWebYCrNeg[k] = AswyB.dRF;

                                WebBCrNeg.AswbB[k] = AswbB;
                                WebSCrNeg.AswsB[k] = AswsB;
                            }
                        }

                    }

//                     if ( AswbB.dRF < dWebBCr[k] )
//                     {
//                         if ( AswbB.dRF<0.0 && dWebBCr[k]<0.0 )
//                         {
//                             if ( AswbB.dRF > dWebBCr[k] )
//                             {
//                                 WebBCr.AswbB[k] = AswbB;
//                                 dWebBCr[k] = AswbB.dRF;
//                             }
//                         }
//                         else
//                         {
//                             WebBCr.AswbB[k] = AswbB;
//                             dWebBCr[k] = AswbB.dRF;
//                         }
//                     }
//                     if ( AswsB.dRF < dWebSCr[k] )
//                     {
//                         if ( AswsB.dRF<0.0 && dWebSCr[k]<0.0 )
//                         {
//                             if ( AswsB.dRF > dWebSCr[k] )
//                             {
//                                 WebSCr.AswsB[k] = AswsB;
//                                 dWebSCr[k] = AswsB.dRF;
//                             }
//                         }
//                         else
//                         {
//                             WebSCr.AswsB[k] = AswsB;
//                             dWebSCr[k] = AswsB.dRF;
//                         }
//                     }
                    if ( AslsB.dA > dLShearCr[k] )
                    {
                        LShrCr.AslsB[k] = AslsB;
                        dLShearCr[k] = AslsB.dA;
                    }
                    if ( AssbB.RFData.dRF < dSlabCr[k] )
                    {
                        if ( AssbB.RFData.dRF<0.0 && dSlabCr[k]<0.0 )
                        {
                            if ( AssbB.RFData.dRF > dSlabCr[k] )
                            {
                                SlabCr.AssbB[k] = AssbB;
                                dSlabCr[k] = AssbB.RFData.dRF;
                            }
                        }
                        else
                        {
                            SlabCr.AssbB[k] = AssbB;
                            dSlabCr[k] = AssbB.RFData.dRF;
                        }
                    }

                    double dBSUts_w = _L_RF4NR(AsbsB.YieldingWeb.RatPa);
					if (IsCriticalValue(dBSUts_w, dTSWebCr[k]))
					{
                        if (dBSUts_w < 0.0 && dTSWebCr[k] < 0.0)
                        {
                            if (dBSUts_w > dTSWebCr[k])
                            {
                                dTSWebCr[k] = dBSUts_w;
                                TsWebCr.AsbsB[k] = AsbsB;
                            }
                        }
                        else
                        {
                            dTSWebCr[k] = dBSUts_w;
                            TsWebCr.AsbsB[k] = AsbsB;
                        }
					}
                    //
                    double dBSUts_s = _L_RF4NR(AsbsB.YieldingStiffener.RatPa);
                    if (IsCriticalValue(dBSUts_s, dTSStiffCr[k]))
                    {
                        if (dBSUts_s < 0.0 && dTSStiffCr[k] < 0.0)
                        {
                            if (dBSUts_s > dTSStiffCr[k])
                            {
                                dTSStiffCr[k] = dBSUts_s;
                                TsStiffCr.AsbsB[k] = AsbsB;
                            }
                        }
                        else
                        {
                            dTSStiffCr[k] = dBSUts_s;
                            TsStiffCr.AsbsB[k] = AsbsB;
                        }
                    }
                    //
                    double dBSUts_b = _L_RF4NR(AsbsB.BucklingEffStiffener.RatPa);
					if (IsCriticalValue(dBSUts_b, dTSBuckCr[k]))
					{
						if (dBSUts_b < 0.0 && dTSBuckCr[k] < 0.0)
						{
							if (dBSUts_b > dTSBuckCr[k])
							{
								dTSBuckCr[k] = dBSUts_b;
								TsBuckCr.AsbsB[k] = AsbsB;
							}
						}
						else
						{
							dTSBuckCr[k] = dBSUts_b;
							TsBuckCr.AsbsB[k] = AsbsB;
						}
					}
					//
					double dBSUfs_f = _L_RF4NR(AssfB.Fastener.RatPa);
                    if (IsCriticalValue(dBSUfs_f, dFastenerCr[k]))
                    {
                        if (dBSUfs_f < 0.0 && dFastenerCr[k] < 0.0)
                        {
                            if (dBSUfs_f > dFastenerCr[k])
                            {
                                dFastenerCr[k] = dBSUfs_f;
                                FastenerCr.AssfB[k] = AssfB;
                            }
                        }
                        else
                        {
                            dFastenerCr[k] = dBSUfs_f;
                            FastenerCr.AssfB[k] = AssfB;
                        }
                    }
                }
                
                T_ASFG_CSG_BS_D AsfgD;
                AsfgD.AsfgB[0] = AsfgCase.AsfgB[0];
                AsfgD.AsfgB[1] = AsfgTenCase.AsfgB[0];
                AsfgD.AsfgB[2] = AsfgCase.AsfgB[1];
                AsfgD.AsfgB[3] = AsfgTenCase.AsfgB[1];

                int nPosNeg = 0;
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlexuralStrength4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nPosNeg, nConcurrent, AsfcCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteShearStrength4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nConcurrent, AsscCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteCombinedMVStrength4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nConcurrent, AscbCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteLongShear4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nConcurrent, AslsCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlangeWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nPosNeg, nConcurrent, AsfgD);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlangeStiffenerWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nConcurrent, AsfsCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebYieldingWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nPosNeg, nConcurrent, AswyCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebBucklingWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nPosNeg, nConcurrent, AswbCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebStiffenerWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nPosNeg, nConcurrent, AswsCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteSlabCheckWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nConcurrent, AssbCase);
                enNR_TS_Check enTsChk = EN_NR_TS_None;
				m_pDoc->m_pPostCtrl->GetLoadRating()->WriteBearingStiffener4NRRailResult(CheckElemK, nOrgRcaseNo, nConcurrent, enTsChk, AsbsCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStrengthFastener4NRRailResult(CheckElemK, nOrgRcaseNo, nConcurrent, AssfCase);
            }
            else if ( bSLS==TRUE && AlcsD.nLimitState==1 )
            {
                for ( int k=0; k<2; ++k )
                {
                    BOOL bCheck = Get_ChkElemFlag4RatingCS454(bSrasKey, SrasD, k);
                    if ( !bCheck ) continue;

                    CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];

                    MembLcomD.Memb.ChIf.nChkPos = k;

                    int nConcurrentId = 1; // FORCE_BLRD_RCASE 의 ConvertBlrd를 하기 때문에 1로 고정
                    ConvertRatStlCalcChkForce(k, bIsPLCB, bDC_Max, bDW_Max, bTemp_Max, nConcurrentId, ForceNC, ForceBlrdRcase, ComLcomF);

                    T_CSGF_MEMB ComFor;
                    m_pCSGDataCtrl->ConvertCsgEngToDgnForce(MembLcomD, ComFor);

                    T_ASSL_CSG_BS_BASE& AsslB   = AsslCase.AsslB[k];
                    AsslB.ComD.ForD = ComFor;
                    T_ASLS_CSG_BS_BASE& AslsSLS = AslsSLSCase.AslsB[k];
                    AslsSLS.ComD.ForD = ComFor;

                    CSG_SERV_RES_CS457 ServResD;
                    CSG_SCON_RES_CS457 SconSLSD;

                    CSG_CHECK_RES_CS457 RsltB;
                    RsltB.pServD = &ServResD;
                    RsltB.pServSconD = &SconSLSD;

                    CheckServiceRCaseAssessment(CheckElemK, k, AlcsD.nMovType, MembLcomD, MembR, RsltB, AsslB);
                    //
                    CheckShearConnectorRCaseCS454(CheckElemK, k, AlcsD.nMovType, MembLcomD, *MembR.pCS457, RsltB, AslsSLS);

                    if ( AsslB.RFcom.dRF < dCompStrCr[k] )
                    {
                        ServCStrCr.AsslB[k] = AsslB;
                        dCompStrCr[k] = AsslB.RFcom.dRF;
                    }
                    if ( AsslB.RFten.dRF < dTensStrCr[k] )
                    {
                        ServTStrCr.AsslB[k] = AsslB;
                        dTensStrCr[k] = AsslB.RFten.dRF;
                    }

                    if ( AslsSLS.dA > dLShearSLSCr[k] )
                    {
                        LShrSLSCr.AslsB[k] = AslsSLS;
                        dLShearSLSCr[k] = AslsSLS.dA;
                    }

                }
                int nPosNeg = 0; // ???
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteSLS4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nPosNeg, nConcurrent, AsslCase);
                m_pDoc->m_pPostCtrl->GetLoadRating()->WriteLongShearSLS4CS454_CSGResult(CheckElemK, nOrgRcaseNo, nConcurrent, AslsSLSCase);
            }

        }
        int nPosNeg = 1;
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlexuralStrength4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, PosiM_Asfc);
        nPosNeg = 2;
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlexuralStrength4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, NegaM_Asfc);

        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteShearStrength4CS454_CSGResult(CheckElemK, 0, 0, VRFmin);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteCombinedMVStrength4CS454_CSGResult(CheckElemK, 0, 0, CombRFmax);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteLongShear4CS454_CSGResult(CheckElemK, 0, 0, LShrCr);

        int nComTen = 1;
        T_ASFG_CSG_BS_D CompAsfgMin;
        CompAsfgMin.AsfgB[0] = CompFlgMin.AsfgB[0];
        CompAsfgMin.AsfgB[2] = CompFlgMin.AsfgB[1];
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlangeWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nComTen, 0, CompAsfgMin);
        nComTen = 2;
        T_ASFG_CSG_BS_D TensAsfgMin;
        TensAsfgMin.AsfgB[1] = CompFlgMin.AsfgB[0];
        TensAsfgMin.AsfgB[3] = CompFlgMin.AsfgB[1];
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlangeWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nComTen, 0, TensAsfgMin);

        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteFlangeStiffenerWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, 0, CompFlgStfn);
        nPosNeg = 1;
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebYieldingWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, WebYCrPos);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebBucklingWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, WebBCrPos);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebStiffenerWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, WebSCrPos);
        nPosNeg = 2;
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebYieldingWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, WebYCrNeg);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebBucklingWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, WebBCrNeg);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteWebStiffenerWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, nPosNeg, 0, WebSCrNeg);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteSlabCheckWithLStiffenerBoxShape4CS454_CSGResult(CheckElemK, 0, 0, SlabCr);

        // Transverse stiffener
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteBearingStiffener4NRRailResult(CheckElemK, 0, 0, EN_NR_TS_Web_Yielding, TsWebCr);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteBearingStiffener4NRRailResult(CheckElemK, 0, 0, EN_NR_TS_Yielding, TsStiffCr);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteBearingStiffener4NRRailResult(CheckElemK, 0, 0, EN_NR_TS_Buckling, TsBuckCr);
		// Fastener
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStrengthFastener4NRRailResult(CheckElemK, 0, 0, FastenerCr);

        // SLS
        nComTen = 1;
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteSLS4CS454_CSGResult(CheckElemK, 0, nComTen, 0, ServCStrCr);
        nComTen = 2;
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteSLS4CS454_CSGResult(CheckElemK, 0, nComTen, 0, ServTStrCr);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteLongShearSLS4CS454_CSGResult(CheckElemK, 0, 0, LShrSLSCr);

        RFSumD.AsrsB[0].bChk = PosiM_Asfc.AsfcB[0].ComD.bChk || NegaM_Asfc.AsfcB[0].ComD.bChk || CompAsfgMin.AsfgB[0].ComD.bChk;
        RFSumD.AsrsB[1].bChk = PosiM_Asfc.AsfcB[1].ComD.bChk || NegaM_Asfc.AsfcB[1].ComD.bChk || CompAsfgMin.AsfgB[1].ComD.bChk;
        RFLSSumD.AsrsB[0].bChk =  CompFlgMin.AsfgB[0].ComD.bChk ||  TensFlgMin.AsfgB[0].ComD.bChk;
        RFLSSumD.AsrsB[1].bChk =  CompFlgMin.AsfgB[1].ComD.bChk ||  TensFlgMin.AsfgB[1].ComD.bChk;
        for ( int kk=0; kk<2; ++kk )
        {
            RFSumD.AsrsB[kk].PBend.bChk = PosiM_Asfc.AsfcB[kk].ComD.bChk;
            RFSumD.AsrsB[kk].PBend.dRF = PosiM_Asfc.AsfcB[kk].dRF;
            RFSumD.AsrsB[kk].NBend.bChk = NegaM_Asfc.AsfcB[kk].ComD.bChk;
            RFSumD.AsrsB[kk].NBend.dRF = NegaM_Asfc.AsfcB[kk].dRF;
            RFSumD.AsrsB[kk].Shear.bChk = VRFmin.AsscB[kk].ComD.bChk;
            RFSumD.AsrsB[kk].Shear.dRF = VRFmin.AsscB[kk].dRF;
            RFSumD.AsrsB[kk].Comb.bChk = CombRFmax.AscbB[kk].ComD.bChk;
            RFSumD.AsrsB[kk].Comb.dRF = CombRFmax.AscbB[kk].dA;
            RFSumD.AsrsB[kk].LShear.bChk = LShrCr.AslsB[kk].ComD.bChk;
            RFSumD.AsrsB[kk].LShear.dRF = LShrCr.AslsB[kk].dA;
            RFSumD.AsrsB[kk].SLS.bChk = (ServCStrCr.AsslB[kk].ComD.bChk || ServTStrCr.AsslB[kk].ComD.bChk);
            RFSumD.AsrsB[kk].SLS.dRF = min(ServCStrCr.AsslB[kk].dA, ServTStrCr.AsslB[kk].dA);
            //
            RFLSSumD.AsrsB[kk].CFlg.bChk = CompFlgMin.AsfgB[kk].ComD.bChk;
            RFLSSumD.AsrsB[kk].CFlg.dRF  = CompFlgMin.AsfgB[kk].dRF;
            RFLSSumD.AsrsB[kk].TFlg.bChk = TensFlgMin.AsfgB[kk].ComD.bChk;
            RFLSSumD.AsrsB[kk].TFlg.dRF  = TensFlgMin.AsfgB[kk].dRF;
            RFLSSumD.AsrsB[kk].Web.bChk = WebYCrPos.AswyB[kk].ComD.bChk;
            RFLSSumD.AsrsB[kk].Web.dRF = WebYCrPos.AswyB[kk].dRF;
            RFLSSumD.AsrsB[kk].LShear.bChk = LShrCr.AslsB[kk].ComD.bChk;
            RFLSSumD.AsrsB[kk].LShear.dRF = LShrCr.AslsB[kk].dA;
            RFLSSumD.AsrsB[kk].SLS.bChk = (ServCStrCr.AsslB[kk].ComD.bChk || ServTStrCr.AsslB[kk].ComD.bChk);
            RFLSSumD.AsrsB[kk].SLS.dRF = min(ServCStrCr.AsslB[kk].dA, ServTStrCr.AsslB[kk].dA);
        }
        CheckSummaryReserveFactorCS454(RFSumD);
        CheckLSSummaryReserveFactorCS454(RFLSSumD);

        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteRFSummaryIShape4CS454_CSGResult(CheckElemK, RFSumD);
        m_pDoc->m_pPostCtrl->GetLoadRating()->WriteRFSummaryBoxShape4CS454_CSGResult(CheckElemK, RFLSSumD);
    }

    //Check_RFminbyRatingCase();

    if ( MembR.pCS457 != NULL ) delete MembR.pCS457;
    if ( ChkResD.pCS457 != NULL ) delete ChkResD.pCS457;

    if ( MembR.pNRGN != NULL) delete MembR.pNRGN;
    if ( ChkResD.pNRGN != NULL) delete ChkResD.pNRGN;

    return TRUE;
}

template<typename T1>
BOOL CheckReserveFactor(const T1& rData)
{
    BOOL bOK = (rData.bChk && rData.dRF < 1.0) ? FALSE : TRUE;
    return bOK;
}

BOOL CDgnStlRatingDataCtrl::CheckSummaryReserveFactorCS454(T_ASRS_CSG_BS_D& rData)
{
    for ( int i=0; i<2; ++i )
    {
        T_ASRS_CSG_BS_BASE& AsrsB = rData.AsrsB[i];
        BOOL bPBendOK = CheckReserveFactor(AsrsB.PBend);
        BOOL bNBendOK = CheckReserveFactor(AsrsB.NBend);
        BOOL bShearOK = CheckReserveFactor(AsrsB.Shear);
        BOOL bCombOK  = CheckReserveFactor(AsrsB.Comb);
        BOOL bSLSOK   = CheckReserveFactor(AsrsB.SLS);
        BOOL bLShearOK= CheckReserveFactor(AsrsB.LShear);
        AsrsB.nOK = (!bPBendOK || !bNBendOK || !bShearOK || !bCombOK || !bSLSOK || !bLShearOK) ? 0 : 1;
    }
    
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckLSSummaryReserveFactorCS454(T_ASRS_LS_CSG_BS_D& rData)
{
    for ( int i=0; i<2; ++i )
    {
        T_ASRS_LS_CSG_BS_BASE& AsrsB = rData.AsrsB[i];
        BOOL bCFlgOK    = CheckReserveFactor(AsrsB.CFlg);
        BOOL bTFlgOK    = CheckReserveFactor(AsrsB.TFlg);
        BOOL bWebOK     = CheckReserveFactor(AsrsB.Web);
        BOOL bSLSOK     = CheckReserveFactor(AsrsB.SLS);
        BOOL bLShearOK  = CheckReserveFactor(AsrsB.LShear);
        AsrsB.nOK = (!bCFlgOK || !bTFlgOK || !bWebOK || !bSLSOK || !bLShearOK) ? 0 : 1;
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckStrengthRatingcaseAASHTO12(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
                                               CSG_MEMB_POSD_KSCE_LSD &MembLcomD, 
                                               CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
                                               T_RLCS_D &RlcsD, double dCapacityF,  double dCapacityFsh, double dVuFactor, T_RCST_BASE &RcstB,
                                               double dPositive_RF[2], double dNegative_RF[2], double dPositive_CADE[2],  double dNegative_CADE[2], T_RCFC_D &RcfcD,
                                               double dVn_RF[2], double dVn_CADE[2], T_RCSC_D &RcscD)
{
    CSG_MEMBPOS_RES_AASHTO_LRFD_D TMembResAASHTO = MembResAASHTO;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pAASHTO = &TMembResAASHTO;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_MEMB_RES_AASHTO_LRFD_D TRes;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pAASHTO = &TRes;

    MembLcomD.aChkFor[0].nLcomType = 0;

    m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    CSG_MEMB_RES_AASHTO_LRFD_STR_D &StrnRes = TRes.StrengthD;

    //////////////////////////////////////////////////////////////////////////

    double dMn=0.0;
    double dMu_PR = RcstB.ForPR.dForce[4];

    BOOL bPositive = (dMu_PR>=0.0);
    int nPosNeg = 0;
    if (bPositive)
    {
        if (StrnRes.FlexD[0].nCase != 0) nPosNeg = 0;
        else                             nPosNeg = 1;
    }
    else
    {
        if (StrnRes.FlexD[1].nCase != 0) nPosNeg = 1;
        else                             nPosNeg = 0;
    }
    CSG_MEMB_RES_AASHTO_LRFD_STR_FLEX_D& FlexD = StrnRes.FlexD[nPosNeg];

    //  CSG_LCOM_RES_D &ForMVpr = MembResMVpr.MembR[nPos].ServiceD.ForceD;

    double dpMn_CA = 0.0;
    double dpFn_CA = 0.0;
    double dpResis = 0.0;
    double dDemand = 0.0;
    double dMVval  = 0.0;

    double dPhi=0.0;

    switch (FlexD.nCase)
    {
    case 1:
        dpMn_CA = dCapacityF * FlexD.PFCSD.dpMn;
        dPhi    = FlexD.PFCSD.dphif;
        dpResis = dpMn_CA;
        dDemand = RcstB.ForSUM.dForce[4];
        dMVval  = RcstB.drPR*RcstB.ForPR.dForce[4];
        break;
    case 2:  // tension stress로 비교
        dpFn_CA = dCapacityF * FlexD.PFNSD.dpFnt;
        dPhi    = FlexD.PFNSD.dphif;
        dpResis = dpFn_CA;
        dDemand = FlexD.PFNSD.dfbutSum;
        dMVval  = FlexD.ForceD.dfbut_fl3_mv; //FlexD.PFNSD.dfbutMV;
        RcstB.dfbu_SUM = dDemand;
        RcstB.dfbu_PR  = dMVval; 
        break;
    case 3:  //  compression stress로 비교
	{
		double dRF_t = CalcTensionRF(dCapacityF * FlexD.NFD.dpFnt, FlexD.NFD.dfbutSum, FlexD.ForceD.dfbut_mv);
		double dRF_c = CalcCompressionRF(dCapacityF * FlexD.NFD.dpFnc, FlexD.NFD.dfbucSum, FlexD.ForceD.dfbuc_fl3_mv);

		if (dRF_c < dRF_t + m_dZero)
		{
			dpFn_CA = dCapacityF * FlexD.NFD.dpFnc;
			dDemand = FlexD.NFD.dfbucSum;
			dMVval = FlexD.ForceD.dfbuc_fl3_mv; //-1.0 * FlexD.NFD.dfbucMV;
			RcstB.dfbu_SUM = dDemand;
			RcstB.dfbu_PR = dMVval;
		}
		else
		{
			dpFn_CA = dCapacityF * FlexD.NFD.dpFnt;
			dDemand = FlexD.NFD.dfbutSum;
			dMVval = FlexD.ForceD.dfbut_mv;
			RcstB.dfbu_SUM = dDemand;
			RcstB.dfbu_PR = dMVval;

		}
		dPhi = FlexD.NFD.dphif;
		dpResis = dpFn_CA;
	}
	break;
    case 4:
        {
            double dMr = 0.0;
            if ( FlexD.NFA6.enMnCase == EN_Mnc_FLB )
            {
                //dMu = FlexD.NFA6.dMuSum_F;
                dMr = FlexD.NFA6.dpMnc_cr;
            }
            else if ( FlexD.NFA6.enMnCase == EN_Mnc_LTB )
            {
                //dMu = FlexD.NFA6.dMuSum_L;
                dMr = FlexD.NFA6.dpMnc_cr;
            }
            else if ( FlexD.NFA6.enMnCase == EN_Mnt )
            {
                //dMu = FlexD.NFA6.dMuy;
                dMr = FlexD.NFA6.dpMnt;
            }

            dpMn_CA = dCapacityF * dMr;
            dPhi    = FlexD.NFA6.dphif;
            dpResis = dpMn_CA;
            dDemand = RcstB.ForSUM.dForce[4];
            dMVval  = RcstB.drPR*RcstB.ForPR.dForce[4];
            break;
        }
    default:
        break;
    }

    RcstB.dMn_CA  = (dPhi==0.0) ? 0.0 : dpMn_CA/dPhi;
    RcstB.dpMn_CA = dpMn_CA;
    RcstB.dFn_CA  = (dPhi==0.0) ? 0.0 : dpFn_CA/dPhi;
    RcstB.dpFn_CA = dpFn_CA;  

    if(FlexD.nCase == 1) // positive flexure <moment> term
    {
        RcstB.dCaDeM = fabs(dpMn_CA) - fabs(dDemand);

        if(RcstB.dCaDeM < 0.0 && dMVval > 0.0 )  // 강도로 인한 NG, 부호가 같은 경우
        {
            RcstB.nRFcase = 0;
            RcstB.dRF     = 0.0;
        }
        else if(RcstB.dCaDeM < 0.0 && dMVval <0.0)  // 강도로 인한 NG, 부호가 다른 경우
        {
            RcstB.nRFcase = 1;
            RcstB.dRF     = 0.0;
        }
        else if(dMVval >= 0.0)  // 부호가 같은 경우
        {
            RcstB.nRFcase = 2;
            RcstB.dRF = dMVval == 0.0 ? m_dRatMax : RcstB.dCaDeM / dMVval;      
        }
        else if(dMVval <= 0.0) // 부호가 다른 경우
        {
            RcstB.nRFcase = 3;
            RcstB.dRF = dMVval == 0.0 ? m_dRatMax : RcstB.dCaDeM / dMVval;
        }

    }
    else if(FlexD.nCase == 4) // negative flexure <moment> term
    {
        RcstB.dCaDeM = fabs(dpMn_CA) - fabs(dDemand);

        if(RcstB.dCaDeM < 0.0 && dMVval < 0.0) // 강도로 인한 NG,  부호가 같은 경우
        {
            RcstB.nRFcase = 0;
            RcstB.dRF     = 0.0;
        }
        else if(RcstB.dCaDeM < 0.0 && dMVval > 0.0) // 강도로 인한 NG, 부호가 다른 경우
        {
            RcstB.nRFcase = 1;
            RcstB.dRF     = 0.0;
        }
        else if(dMVval <= 0.0)  // 부호가 같은 경우
        {
            RcstB.nRFcase = 2;
            RcstB.dRF = dMVval == 0.0 ? m_dRatMax : -1.0 * RcstB.dCaDeM  / dMVval;
        }
        else if(dMVval >= 0.0)  // 부호가 다른 경우
        {
            RcstB.nRFcase = 3;
            RcstB.dRF = dMVval == 0.0 ? m_dRatMax : RcstB.dCaDeM  / dMVval;
        }
    }
    else if(FlexD.nCase == 2)  // check with tension <stress> term (+)
    {
        RcstB.dCaDeF = fabs(dpFn_CA) - dDemand;
        
		if(dMVval >= 0.0)  // 부호가 같은 경우
        {
            RcstB.nRFcase = 2;
            RcstB.dRF = dMVval == 0.0 ? m_dRatMax : RcstB.dCaDeF / dMVval;      
        }
        else if(dMVval <= 0.0) // 부호가 다른 경우
        {
            RcstB.nRFcase = 3;
            RcstB.dRF = dMVval == 0.0 ? m_dRatMax : -1.0 * RcstB.dCaDeF / dMVval;
        }
    }
    else if(FlexD.nCase == 3)  // check with compression <stress> term (-)
    {
		if (dMVval >= 0.0)
		{
			RcstB.dCaDeF = fabs(dpFn_CA) - dDemand;
			if (dDemand >= 0.0)  // 부호가 같은 경우
			{
				RcstB.nRFcase = 2;
				RcstB.dRF = dMVval == 0.0 ? m_dRatMax : RcstB.dCaDeF / dMVval;
			}
			else if (dDemand <= 0.0) // 부호가 다른 경우
			{
				RcstB.nRFcase = 3;
				RcstB.dRF = dMVval == 0.0 ? m_dRatMax : -1.0 * RcstB.dCaDeF / dMVval;
			}
		}
		else
		{
			if (dDemand < 0.0) dDemand = fabs(dDemand);
			else               dDemand = -1.0*dDemand;
			RcstB.dCaDeF = dpFn_CA - dDemand;

			if (dDemand <= 0.0)  // 부호가 같은 경우
			{
				RcstB.nRFcase = 2;
				RcstB.dRF = dMVval == 0.0 ? m_dRatMax : RcstB.dCaDeF / dMVval;
			}
			else if (dDemand > 0.0)  // 부호가 다른 경우
			{
				RcstB.nRFcase = 3;
				RcstB.dRF = dMVval == 0.0 ? m_dRatMax : -1.0 * RcstB.dCaDeF / dMVval;
			}
		}
    }
    else ASSERT(0);

    // Calculation Rating Factor.
    RcstB.nCaseRes = FlexD.nCase;
    RcstB.dPhi_RE = dPhi; 
    RcstB.dKRF    = m_dK*RcstB.dRF;
    RcstB.dRT     = m_dWeight; // 우선 여기에 할당
    RcstB.dSLCapa = m_dWeight * RcstB.dRF * m_dK;  


    BOOL bPositiveDC = TRUE;
    if(FlexD.nCase == 3 || FlexD.nCase == 4)
    {
        bPositiveDC = FALSE;
    }

    if(dMu_PR == 0.0)
    {
        RcstB.dRF = m_dRatMax;
    }
    else if(RcstB.dRF < 0.0)
    {
        // skip
    }
    else if(bPositiveDC)
    {
        RcstB.nMoment = 1; // 1=Positive, 2=Negative

        if(nPos==0) // I   
        {      
            if(dPositive_RF[0] > RcstB.dRF)
            {
                dPositive_RF[0] = RcstB.dRF;
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[0]);
            }
            else if(dPositive_RF[0] == 0.0 && RcstB.dRF == 0.0)
            {
                if(FlexD.nCase == 1 || FlexD.nCase == 4)
                {
                    if(dPositive_CADE[0] > RcstB.dCaDeM)
                    {
                        dPositive_CADE[0] = RcstB.dCaDeM;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[0]);
                    }               
                }
                else if(FlexD.nCase == 2 || FlexD.nCase == 3)
                {
                    if(dPositive_CADE[0] > RcstB.dCaDeF)
                    {
                        dPositive_CADE[0] = RcstB.dCaDeF;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[0]);
                    } 
                }
            }
        }
        else
        {
            if(dPositive_RF[1] > RcstB.dRF)
            {
                dPositive_RF[1] = RcstB.dRF;
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[2]);
            }
            else if(dPositive_RF[1] == 0.0 && RcstB.dRF == 0.0)
            {
                if(FlexD.nCase == 1 || FlexD.nCase == 4)
                {
                    if(dPositive_CADE[1] > RcstB.dCaDeM)
                    {
                        dPositive_CADE[1] = RcstB.dCaDeM;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[2]);
                    }               
                }
                else if(FlexD.nCase == 2 || FlexD.nCase == 3)
                {
                    if(dPositive_CADE[1] > RcstB.dCaDeF)
                    {
                        dPositive_CADE[1] = RcstB.dCaDeF;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[2]);
                    } 
                }
            }
        }    
    }
    else if(!bPositiveDC)
    {
        RcstB.nMoment = 2; // 1=Positive, 2=Negative
        if(nPos==0) // I      
        {
            if(dNegative_RF[0] > RcstB.dRF)
            {
                dNegative_RF[0] = RcstB.dRF;
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[1]);
            }
            else if(dNegative_RF[0] == 0.0 && RcstB.dRF == 0.0)
            {
                if(FlexD.nCase == 1 || FlexD.nCase == 4)
                {
                    if(dNegative_CADE[0] > RcstB.dCaDeM)
                    {
                        dNegative_CADE[0] = RcstB.dCaDeM;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[1]);
                    }               
                }
                else if(FlexD.nCase == 2 || FlexD.nCase == 3)
                {
                    if(dNegative_CADE[0] > RcstB.dCaDeF)
                    {
                        dNegative_CADE[0] = RcstB.dCaDeF;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[1]);
                    } 
                }
            }
        }
        else
        {
            if(dNegative_RF[1]>RcstB.dRF)
            {
                dNegative_RF[1] = RcstB.dRF;
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[3]);
            }
            else if(dNegative_RF[1] == 0.0 && RcstB.dRF == 0.0)
            {
                if(FlexD.nCase == 1 || FlexD.nCase == 4)
                {
                    if(dNegative_CADE[1] > RcstB.dCaDeM)
                    {
                        dNegative_CADE[1] = RcstB.dCaDeM;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[3]);
                    }               
                }
                else if(FlexD.nCase == 2 || FlexD.nCase == 3)
                {
                    if(dNegative_CADE[1] > RcstB.dCaDeF)
                    {
                        dNegative_CADE[1] = RcstB.dCaDeF;
                        SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfcD.FlexR[3]);
                    } 
                }
            }
        }              
    }  


    // RcstB.dRF     = (dMVval==0.0) ? 0.0 : (dCapacityF*dpResis-dDemand)/(dMVval);


    //////////////////////////////////////////////////////////////////////////
    // Shear
    // Calculation Rating Factor.
    double dVu_PR = dVuFactor * RcstB.ForPR.dForce[2];


    CSG_MEMB_RES_AASHTO_LRFD_STR_SHEAR_D &ShrR = StrnRes.ShearD;

    double dVuDemand = dVuFactor * RcstB.ForSUM.dForce[2];

    RcstB.dVn_CA = dCapacityFsh * ShrR.dVn;
    RcstB.dVr_CA = dCapacityFsh * ShrR.dpVn;  
    RcstB.dCaDeSh = fabs(RcstB.dVr_CA) - fabs(dVuDemand);

    if(RcstB.dCaDeSh < 0.0)
    {
        RcstB.nRFcasesh = 0;
        RcstB.dRFsh     = 0.0;
    }
    else // shear는 항상 부호가 같다고 판단!
    {
        RcstB.nRFcasesh = 1;
        RcstB.dRFsh  = (RcstB.drPR * dVu_PR==0.0) ? 0.0 : (dCapacityFsh * ShrR.dpVn-fabs(dVuDemand)) / fabs(RcstB.drPR * dVu_PR);

    }

    RcstB.dKRFsh = m_dK*RcstB.dRFsh;
    RcstB.dSLCapaSh = m_dWeight * RcstB.dRFsh * m_dK;

    if(dVu_PR==0.0)
    {              
        RcstB.dRFsh = m_dRatMax;
    }
    else if(RcstB.dRFsh < 0.0)
    {
        // skip
    }
    else
    {

        if(nPos==0) // I
        {
            if(dVn_RF[0]>RcstB.dRFsh)
            {
                dVn_RF[0] = RcstB.dRFsh;
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcscD.ShearR[0]);        
            }
            else if(dVn_RF[0]==0.0 && RcstB.dRFsh==0.0)
            {
                if(dVn_CADE[0] > RcstB.dCaDeSh)
                {
                    dVn_CADE[0] = RcstB.dCaDeSh;
                    SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcscD.ShearR[0]);        
                }
            }
        }
        else
        {
            if(dVn_RF[1] >RcstB.dRFsh)
            {
                dVn_RF[1] = RcstB.dRFsh;
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcscD.ShearR[1]);        
            }
            else if(dVn_RF[1]==0.0 && RcstB.dRFsh==0.0)
            {
                if(dVn_CADE[1] > RcstB.dCaDeSh)
                {
                    dVn_CADE[1] = RcstB.dCaDeSh;
                    SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcscD.ShearR[0]);        
                }
            }
        }
    }

    return TRUE;
}

double CDgnStlRatingDataCtrl::CalcTensionRF(double dCa, double dDe, double dMV)
{
	double dRF = 0.0;
	double dCaDe = dCa - dDe;
	if (dMV >= 0.0) // 부호가 같은 경우
	{
		dRF = (dMV == 0.0) ? m_dRatMax : dCaDe / dMV;
	}
	else // 부호가 다른 경우
	{
		dRF = (dMV == 0.0) ? m_dRatMax : -1.0*dCaDe / dMV;
	}
	return dRF;
}

double CDgnStlRatingDataCtrl::CalcCompressionRF(double dCa, double dDe, double dMV)
{
	double dStrCDe = dDe;
	if (dStrCDe < 0.0) dStrCDe = fabs(dStrCDe);
	else               dStrCDe = -1.0*dStrCDe;

	double dRF = 0.0;
	double dCaDe = dCa - dStrCDe;
	if (dMV <= 0.0) // 부호가 같은 경우
	{
		dRF = (dMV == 0.0) ? m_dRatMax : -1.0*dCaDe / dMV;
	}
	else // 부호가 다른 경우
	{
		dRF = (dMV == 0.0) ? m_dRatMax :dCaDe / dMV;
	}
	return dRF;

}

BOOL CDgnStlRatingDataCtrl::CheckServicethRatingcaseAASHTO12(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
                                                CSG_MEMB_POSD_KSCE_LSD &MembLcomD, 
                                                CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
                                                T_RLCS_D &RlcsD, double dCapacityF,
                                                T_RCSS_BASE &RcssB,
                                                double &dCompI_RF, double &dTensI_RF, double &dCompJ_RF, double &dTensJ_RF,    
                                                double &dCompI_CADE,  double &dCompJ_CADE,  double &dTensI_CADE,  double &dTensJ_CADE,
                                                T_RCGS_D &RcgsD)                                               

{
    CSG_MEMBPOS_RES_AASHTO_LRFD_D TMembResAASHTO = MembResAASHTO;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pAASHTO = &TMembResAASHTO;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_MEMB_RES_AASHTO_LRFD_D TRes;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pAASHTO = &TRes;

    MembLcomD.aChkFor[0].nLcomType = 1;

    m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    CSG_MEMB_RES_AASHTO_LRFD_SER_D &ServRes = TRes.ServiceD;


    //////////////////////////////////////////////////////////////////////////

    if (m_pRpasD->nServCalcType==0)
    {
        RcssB.dStress_AL[0] = ServRes.dFcrw;
        RcssB.dStress_AL[1] = ServRes.dffcn;
        RcssB.dStress_AL[2] = ServRes.dfftn;
    }
    else
    {
        if (RlcsD.nEvaluation==ENUM_DESIGN) // 0:Design Live Load
        {
            //RcssB.dStress_AL[0] = ServRes.dFcrw;
            RcssB.dStress_AL[1] = m_pRpasD->dDgnLoadComp;
            RcssB.dStress_AL[2] = m_pRpasD->dDgnLoadTens;
        }
        else if (RlcsD.nEvaluation==ENUM_LEGAL || RlcsD.nEvaluation==ENUM_PERMIT ) // 1:Legal Load/Permit Load
        {
            //RcssB.dStress_AL[0] = ServRes.dFcrw;
            RcssB.dStress_AL[1] = m_pRpasD->dLegalLoadComp;
            RcssB.dStress_AL[2] = m_pRpasD->dLegalLoadTens;
        }
    }

    CSG_MEMB_RES_AASHTO_LRFD_SER_D &MembServD = ServRes;

    CSG_LCOM_RES_D &ForD = MembServD.ForceD;

    BOOL bPositive = (ForD.dMu>=0.0);
    BOOL bIsTopC   = bPositive; 

    RcssB.StrDC_Be.bIsTopC = RcssB.StrST.bIsTopC = RcssB.StrLT.bIsTopC = bIsTopC;

    RcssB.StrSUM.dfcw = ServRes.dfc;
    RcssB.dRT[0]     = m_dWeight;  // 우선 여기에 할당

    if (ForD.dft_mv < 0.0) // Demand의 부호가 아니라 이동하중 기준으로 수정
    {
        RcssB.StrDC_Be.dfcf = ForD.dft_gr;  
        RcssB.StrLT.dfcf    = ForD.dft_lt;  
        RcssB.StrST.dfcf    = ForD.dft_st;
        RcssB.StrSUM.dfcf   = ForD.dft;
        //
        RcssB.StrDC_Be.dftf = ForD.dfb_gr;  
        RcssB.StrLT.dftf    = ForD.dfb_lt;  
        RcssB.StrST.dftf    = ForD.dfb_st;  
        RcssB.StrSUM.dftf   = MembServD.dfuSum; //ForD.dfb;
        //
    }
    else
    {
        RcssB.StrDC_Be.dfcf = ForD.dfb_gr;  
        RcssB.StrLT.dfcf    = ForD.dfb_lt;  
        RcssB.StrST.dfcf    = ForD.dfb_st;  
        RcssB.StrSUM.dfcf   = MembServD.dfuSum; //ForD.dfb;
        //
        RcssB.StrDC_Be.dftf = ForD.dft_gr;  
        RcssB.StrLT.dftf    = ForD.dft_lt;  
        RcssB.StrST.dftf    = ForD.dft_st;  
        RcssB.StrSUM.dftf   = ForD.dft;

        bIsTopC = FALSE;
        RcssB.StrDC_Be.bIsTopC = RcssB.StrST.bIsTopC = RcssB.StrLT.bIsTopC = bIsTopC;
    }


    //////////////////////////////////////////////////////////////////////////

    if(ForD.dft_mv < 0.0)
    {
        RcssB.StrPR.dfcf    = ForD.dft_mv;
        RcssB.StrPR.dftf    = ForD.dffb_mv2max; //dfb_mv; 
        RcssB.StrPR.bIsTopC = TRUE;
    }
    else if(ForD.dft_mv > 0.0)
    {
        RcssB.StrPR.dfcf    = ForD.dffb_mv2max;
        RcssB.StrPR.dftf    = ForD.dft_mv;
        RcssB.StrPR.bIsTopC = FALSE;
    }  

    //////////////////////////////////////////////////////////////////////////

    double dffcn = RcssB.dStress_AL[1];
    double dfftn = RcssB.dStress_AL[2];

    //double dffc_PR = RcssB.StrPR.dfcf;
    //double dfft_PR = RcssB.StrPR.dftf;

    // 이동하중 기준으로 수정
    double dfcom4MVpos = RcssB.StrPR.dfcf;
    double dften4MVpos = RcssB.StrPR.dftf;


    // compression flange
    double dStrCDe = RcssB.StrSUM.dfcf;
    //
    if (dStrCDe<0.0) dStrCDe = fabs(dStrCDe);
    else             dStrCDe = -1.0*dStrCDe;

    RcssB.dCaDe[1] = fabs(dffcn) - dStrCDe;

    // MQC-18527, Stress 경우 이기 때문에 Stress가 반대로 나오더라고 RF는 계산할 수 있기 때문에 error로 판단한 부분은 삭제..
    //   if(RcssB.dCaDe[1] < 0.0 && dfcom4MVpos < 0.0) //  strength error!, 부호가 같은 경우
    //   {
    //     RcssB.nRFcase[1] = 0;
    //     RcssB.dRF[1]     = 0.0;
    //   }
    //   else if(RcssB.dCaDe[1] < 0.0 && dfcom4MVpos > 0.0)  //  strength error!, 부호가 다른 경우
    //   {
    //     RcssB.nRFcase[1] = 1;
    //     RcssB.dRF[1]     = 0.0;
    //   }
    if(dfcom4MVpos <= 0.0) // 부호가 같은 경우
    {
        RcssB.nRFcase[1] = 2; 
        RcssB.dRF[1]  = (RcssB.drPR * dfcom4MVpos ==0.0) ? m_dRatMax : -1.0 * RcssB.dCaDe[1] / (dfcom4MVpos);
    }
    else if(dfcom4MVpos >= 0.0) // 부호가 다른 경우  
    {
        RcssB.nRFcase[1] = 3;   
        RcssB.dRF[1]  = (RcssB.drPR * dfcom4MVpos ==0.0) ? m_dRatMax : RcssB.dCaDe[1] / (dfcom4MVpos);
    }  

    // tension flange
    double dStrTDe = RcssB.StrSUM.dftf;
    //
    RcssB.dCaDe[2] = fabs(dfftn) - dStrTDe;
    //   if(RcssB.dCaDe[2] < 0.0 && dften4MVpos > 0.0) // strength error!, 부호가 같은 경우
    //   {
    //     RcssB.nRFcase[2] = 0;
    //     RcssB.dRF[2]     = 0.0;
    //   }
    //   else if(RcssB.dCaDe[2] < 0.0 && dften4MVpos < 0.0)  //   strength error!, 부호가 다른 경우
    //   {
    //     RcssB.nRFcase[2] = 1;
    //     RcssB.dRF[2]     = 0.0;
    //   }
    if(dften4MVpos >= 0.0) // 부호가 같은 경우
    {
        RcssB.nRFcase[2] = 2;
        RcssB.dRF[2]  = (RcssB.drPR * dften4MVpos==0.0) ? m_dRatMax : RcssB.dCaDe[2] / (dften4MVpos);
    }
    else if(dften4MVpos <= 0.0) // 부호가 다른 경우
    {
        RcssB.nRFcase[2] = 3;
        RcssB.dRF[2]  = (RcssB.drPR * dften4MVpos==0.0) ? m_dRatMax : -1.0 * RcssB.dCaDe[2] / (dften4MVpos);
    }

    RcssB.dKRF[1] = m_dK*RcssB.dRF[1];
    RcssB.dKRF[2] = m_dK*RcssB.dRF[2];
    RcssB.dSLCapa[1] = m_dWeight*RcssB.dKRF[1];
    RcssB.dSLCapa[2] = m_dWeight*RcssB.dKRF[2];

    RcssB.nPoint[0] = (bIsTopC==TRUE) ? 0 : 1;
    RcssB.nPoint[1] = (bIsTopC==TRUE) ? 1 : 0;

    if(RcssB.dRF[1] >= 0.0 && RcssB.dRF[2]>= 0.0)
    {
        RcssB.dRFmin = min(RcssB.dRF[1], RcssB.dRF[2]);
    }
    else if(RcssB.dRF[1] < 0.0 && RcssB.dRF[2] < 0.0)
    {
        RcssB.dRFmin = max(RcssB.dRF[1], RcssB.dRF[2]);
    }
    else if(RcssB.dRF[1] * RcssB.dRF[2] < 0.0)
    {
        RcssB.dRFmin = max(RcssB.dRF[1], RcssB.dRF[2]);
    }

    if (RcssB.dRF[1]>RcssB.dRF[2]) // 인장이 불리 
    {
        RcssB.nCrMinType = 2;    
    }
    else // 압축이 불리
    {
        RcssB.nCrMinType = 1;    
    }
    RcssB.dKRFmin = m_dK*RcssB.dRFmin;



    // Calculation Rating Factor.
    if(nPos==0)
    {
        if(fabs(m_dRatMax-RcssB.dRF[1])<m_dZero)
        {
            // Skip.
        }
        else if(RcssB.dRF[1] < 0.0)
        {
            // skip
        }
        else if(dCompI_RF>RcssB.dRF[1])
        {
            dCompI_RF = RcssB.dRF[1];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[1]);
        }
        else if(dCompI_RF == 0.0 && RcssB.dRF[1] == 0.0)
        {
            if(dCompI_CADE > RcssB.dCaDe[1])
            {
                dCompI_CADE = RcssB.dCaDe[1];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[1]);
            }
        }

        if(fabs(m_dRatMax-RcssB.dRF[2])<m_dZero)
        {
            // Skip.
        }
        else if(RcssB.dRF[2] < 0.0)
        {
            // skip.
        }
        else if(dTensI_RF>RcssB.dRF[2]) 
        {
            dTensI_RF = RcssB.dRF[2];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[2]);
        }   
        else if(dTensI_RF == 0.0 && RcssB.dRF[1] == 0.0)
        {
            if(dTensI_CADE > RcssB.dCaDe[2])
            {
                dTensI_CADE = RcssB.dCaDe[2];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[2]);
            }
        }   
    }
    else
    {
        if(fabs(m_dRatMax-RcssB.dRF[1])<m_dZero)
        {
            // Skip.
        }
        else if(RcssB.dRF[1] < 0.0)
        {
            //skip.
        }
        else if(dCompJ_RF>RcssB.dRF[1])
        {
            dCompJ_RF = RcssB.dRF[1];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[4]);
        }
        else if(dCompJ_RF == 0.0 && RcssB.dRF[1] == 0.0)
        {
            if(dCompJ_CADE > RcssB.dCaDe[1])
            {
                dCompJ_CADE = RcssB.dCaDe[1];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[4]);
            }
        }   

        if(fabs(m_dRatMax-RcssB.dRF[2])<m_dZero)
        {
            // Skip.
        }
        else if(RcssB.dRF[2] < 0.0)
        {
            // Skip.
        }
        else if(dTensJ_RF>RcssB.dRF[2]) 
        {
            dTensJ_RF = RcssB.dRF[2];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[5]);
        }
        else if(dTensJ_RF == 0.0 && RcssB.dRF[1] == 0.0)
        {
            if(dTensJ_CADE > RcssB.dCaDe[2])
            {
                dTensJ_CADE = RcssB.dCaDe[2];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcgsD.StressR[5]);
            }
        }     
    }                      


    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckFatigueRatingcaseAASHTO12(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
                                              CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
                                              T_RLCS_D &RlcsD, double dCapacityF, double dCapaN, 
                                              double dStrDelMV_PR[2], double dStrDelMV_AD[2],
                                              T_RCFA_BASE &RcfaB,
                                              double &dTopI_RF, double &dBotI_RF, double &dTopJ_RF, double &dBotJ_RF,    
                                              double &dFTopI_CADE,  double &dFBotI_CADE,  double &dFTopJ_CADE,    double &dFBotJ_CADE,
                                              T_RCFA_D &RcfaD)
{
    CSG_MEMBPOS_RES_AASHTO_LRFD_D TMembResAASHTO = MembResAASHTO;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pAASHTO = &TMembResAASHTO;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_MEMB_RES_AASHTO_LRFD_D TRes;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pAASHTO = &TRes;

    MembLcomD.aChkFor[0].nLcomType=1;
    m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);

    CSG_MEMB_RES_AASHTO_LRFD_SER_D &MembServD = TMembR.pAASHTO->MembR[nPos].ServiceD;

    CSG_LCOM_RES_D &ForD = MembServD.ForceD;

    BOOL bPositive = (ForD.dMu>=0.0);
    BOOL bIsTopC   = bPositive; 


    RcfaB.StrDC_Be.dStrT = 0.0; //ForD.dft_gr;  // MQC 28. In case of fatiuge, We ignore demand, just consider capacity and pll by shsihir(14.07.08) for steel
    RcfaB.StrLT.dStrT    = 0.0; //ForD.dft_lt;  
    RcfaB.StrST.dStrT    = 0.0; //ForD.dft_st;  
    //ForD.dft;      // sum
    RcfaB.StrDC_Be.dStrB = 0.0; //ForD.dfb_gr;  
    RcfaB.StrLT.dStrB    = 0.0; //ForD.dfb_lt;  
    RcfaB.StrST.dStrB    = 0.0; //ForD.dfb_st;  
    //ForD.dfb;      // sum
    RcfaB.StrPR.dStrDelT = dStrDelMV_PR[0];
    RcfaB.StrPR.dStrDelB = dStrDelMV_PR[1];

    RcfaB.StrAD.dStrDelT = 0.0; //dStrDelMV_AD[0];
    RcfaB.StrAD.dStrDelB = 0.0; //dStrDelMV_AD[1];

    RcfaB.dRT     = m_dWeight; // 우선 여기에 할당

    double dfst[2]={ForD.dft, ForD.dfb};


    double dffcn = MembServD.dffcn;
    double dfftn = MembServD.dfftn;


    RcfaB.LoadR[0].dpDelF_n = RcfaB.LoadR[1].dpDelF_n = dCapaN;

    for (int i=0; i<2; ++i)
    {
        RcfaB.dCaDe[i] = fabs(dCapaN);// - fabs(dfst[i]) - fabs(dStrDelMV_AD[i]);

        if(RcfaB.dCaDe[i] < 0.0)
        {
            RcfaB.nRFcase[i] = 0;
            RcfaB.dRF_Load[i] = 0.0; 
        }
        else
        {
            RcfaB.nRFcase[i] = 1;
            RcfaB.dRF_Load[i] = (RcfaB.drPR*dStrDelMV_PR[i]==0.0) ? m_dRatMax : RcfaB.dCaDe[i] / fabs(dStrDelMV_PR[i]); 
        }
        RcfaB.LoadR[i].dfbuSum    = fabs(dfst[i]) + fabs(dStrDelMV_AD[i]);
        RcfaB.LoadR[i].dGammaDelF = fabs(dStrDelMV_PR[i]);   

        RcfaB.dKRF_Load[i]  = m_dK*RcfaB.dRF_Load[i];
        RcfaB.dSLCapa[i]    = m_dWeight*RcfaB.dRF_Load[i] * m_dK;
    }

    if (RcfaB.dRF_Load[1]>RcfaB.dRF_Load[2])
    {
        RcfaB.nCrMinType = 1;
    }
    else
    {
        RcfaB.nCrMinType = 0;
    }

    RcfaB.dRFmin = min(RcfaB.dRF_Load[0], RcfaB.dRF_Load[1]);
    RcfaB.dKRFmin = m_dK*RcfaB.dRFmin;

    // Calculation Rating Factor.
    if(nPos==0)
    {
        if(fabs(m_dRatMax-RcfaB.dRF_Load[0])<m_dZero)
        {
            // Skip.
        }
        else if(RcfaB.dRF_Load[0] < 0.0)
        {
            // skip.
        }
        else if(dTopI_RF>RcfaB.dRF_Load[0])
        {
            dTopI_RF = RcfaB.dRF_Load[0];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[0]);
        }
        else if(dTopI_RF == 0.0 && RcfaB.dRF_Load[0] == 0.0)
        {
            if(dFTopI_CADE > RcfaB.dCaDe[0])
            {
                dFTopI_CADE = RcfaB.dCaDe[0];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[0]);
            }
        }


        if(fabs(m_dRatMax-RcfaB.dRF_Load[1])<m_dZero)
        {
            // Skip.
        }
        else if(RcfaB.dRF_Load[1] < 0.0)
        {
            // skip.
        }
        else if(dBotI_RF>RcfaB.dRF_Load[1]) 
        {
            dBotI_RF = RcfaB.dRF_Load[1];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[1]);
        } 
        else if(dBotI_RF == 0.0 && RcfaB.dRF_Load[1] == 0.0)
        {
            if(dFBotI_CADE > RcfaB.dCaDe[1])
            {
                dFBotI_CADE = RcfaB.dCaDe[1];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[1]);
            }
        }

    }
    else
    {
        if(fabs(m_dRatMax-RcfaB.dRF_Load[0])<m_dZero)
        {
            // Skip.
        }
        else if(RcfaB.dRF_Load[0] < 0.0)
        {
            // skip.
        }
        else if(dTopJ_RF>RcfaB.dRF_Load[0])
        {
            dTopJ_RF = RcfaB.dRF_Load[0];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[2]);
        }
        else if(dTopJ_RF == 0.0 && RcfaB.dRF_Load[1] == 0.0)
        {
            if(dFTopJ_CADE > RcfaB.dCaDe[0])
            {
                dFTopJ_CADE = RcfaB.dCaDe[0];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[2]);
            }
        }  

        if(fabs(m_dRatMax-RcfaB.dRF_Load[1])<m_dZero)
        {
            // Skip.
        }
        else if(RcfaB.dRF_Load[1] < 0.0)
        {
            // skip.
        }
        else if(dBotJ_RF>RcfaB.dRF_Load[1]) 
        {
            dBotJ_RF = RcfaB.dRF_Load[1];
            SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[3]);
        }
        else if(dBotJ_RF == 0.0 && RcfaB.dRF_Load[1] == 0.0)
        {
            if(dFBotJ_CADE > RcfaB.dCaDe[1])
            {
                dFBotJ_CADE = RcfaB.dCaDe[1];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiR[3]);
            }
        }     
    }               

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckInfiniteFatigueRatingAASHTO(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
    CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
    T_RLCS_D &RlcsD, double dCapacityF, double dCapaN[4],
    double dStrDelMV_PR[4], double dStrDelMV_AD[4],
    T_RCFA_INF_BASE &RcfaB, double dStressI_RF[4], double dStressJ_RF[4], double dStressI_CADE[4], double dStressJ_CADE[4],
    T_RCFA_INF_LIFE_D &RcfaD)
{
    CSG_MEMBPOS_RES_AASHTO_LRFD_D TMembResAASHTO = MembResAASHTO;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pAASHTO = &TMembResAASHTO;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_MEMB_RES_AASHTO_LRFD_D TRes;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pAASHTO = &TRes;

    MembLcomD.aChkFor[0].nLcomType=1;
    m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);

    CSG_MEMB_RES_AASHTO_LRFD_SER_D &MembServD = TMembR.pAASHTO->MembR[nPos].ServiceD;

    CSG_LCOM_RES_D &ForD = MembServD.ForceD;

    CSG_LCOM_FLANGE_STRESS_D& FlgStrs =  MembServD.ForceD.FlgStrs4Posi;

    BOOL bPositive = (ForD.dMu>=0.0);
    BOOL bIsTopC   = bPositive;

    T_RATING_INF_FATI_LIFE& InfLife = RcfaB.InfLife;

    RcfaB.DRCFA.StrDC_Be.dStrT_Top = FlgStrs.dft_gr_Top;
    RcfaB.DRCFA.StrDC_Be.dStrT_Bot = FlgStrs.dft_gr_Bot;
    RcfaB.DRCFA.StrLT.dStrT_Top    = FlgStrs.dft_lt_Top;
    RcfaB.DRCFA.StrLT.dStrT_Bot    = FlgStrs.dft_lt_Bot;
    RcfaB.DRCFA.StrST.dStrT_Top    = FlgStrs.dft_st_Top;
    RcfaB.DRCFA.StrST.dStrT_Bot    = FlgStrs.dft_st_Bot;
    RcfaB.DRCFA.StrDC_Be.dStrB_Top = FlgStrs.dfb_gr_Top;
    RcfaB.DRCFA.StrDC_Be.dStrB_Bot = FlgStrs.dfb_gr_Bot;
    RcfaB.DRCFA.StrLT.dStrB_Top    = FlgStrs.dfb_lt_Top;
    RcfaB.DRCFA.StrLT.dStrB_Bot    = FlgStrs.dfb_lt_Bot;
    RcfaB.DRCFA.StrST.dStrB_Top    = FlgStrs.dfb_st_Top;
    RcfaB.DRCFA.StrST.dStrB_Bot    = FlgStrs.dfb_st_Bot;

    RcfaB.DRCFA.dRT     = m_dWeight; // 우선 여기에 할당

    RcfaB.DRCFA.StrPR.dStrDelT_Top = dStrDelMV_PR[0];
    RcfaB.DRCFA.StrPR.dStrDelT_Bot = dStrDelMV_PR[1];
    RcfaB.DRCFA.StrPR.dStrDelB_Top = dStrDelMV_PR[2];
    RcfaB.DRCFA.StrPR.dStrDelB_Bot = dStrDelMV_PR[3];

    double dfst[4]= { FlgStrs.dft_Top, FlgStrs.dft_Bot, FlgStrs.dfb_Top, FlgStrs.dfb_Bot };

    double dffcn = MembServD.dffcn;
    double dfftn = MembServD.dfftn;

    GetFatigueParamINF(ElemK.first, nPos, RcfaB);

    double dRsFact = 1.0;
    RcfaB.dRp = CalcRpFact(RcfaB.dSpan, RcfaB.dADTT_Present, RcfaB.dnL);
    RcfaB.dRs = dRsFact;
    double dDeadComp[4] = { RcfaB.DRCFA.StrLT.dStrT_Top, RcfaB.DRCFA.StrLT.dStrT_Bot, RcfaB.DRCFA.StrLT.dStrB_Top, RcfaB.DRCFA.StrLT.dStrB_Bot };

    for ( int i=0; i<4; ++i )
    {
        InfLife.LoadCapa[i].dpDelF_n = dCapaN[i];
        InfLife.LoadCapa[i].dDelFeff = dRsFact * RcfaB.dRp * fabs(dStrDelMV_PR[i]);

        InfLife.LoadCapa[i].dDelFmax = CalcDelFmax(INF_FATI, RlcsD.nEvaluation, InfLife.LoadCapa[i].dDelFeff);

        RcfaB.dCapaDemand[i] = fabs(dCapaN[i]);// - fabs(dfst[i]) - fabs(dStrDelMV_AD[i]);

        RcfaB.dRatFact[i] = (InfLife.LoadCapa[i].dDelFmax==0.0) ? m_dRatMax : RcfaB.dCapaDemand[i] / InfLife.LoadCapa[i].dDelFmax;
        InfLife.LoadCapa[i].dfbuSum    = fabs(dfst[i]) + fabs(dStrDelMV_AD[i]);
        InfLife.LoadCapa[i].dGammaDelF = fabs(dStrDelMV_PR[i]);

        RcfaB.dKRatFact_Load[i]  = m_dK*RcfaB.dRatFact[i];
        RcfaB.dSafeLoadCapa[i]    = m_dWeight*RcfaB.dRatFact[i] * m_dK;

        if ( 2.2 * InfLife.LoadCapa[i].dDelFeff < -1.0 * dDeadComp[i] ) InfLife.bSkip[i] = TRUE;
        RcfaB.bSkip[i] = InfLife.bSkip[i];
    }

    std::vector<double> RatFact;
    for (int m = 0; m < 4; m++ )
    {
        RatFact.emplace_back(RcfaB.dRatFact[m]);
    }

    double dMinRatingFactor = *std::min_element(RatFact.begin(), RatFact.end());

    for (int n = 0; n < 4; n++ )
    {
        if ( dgn::EQ(RcfaB.dRatFact[n], dMinRatingFactor) )
        {
            RcfaB.nCrMinTypeDetail = n;
        }
    }

    RcfaB.DRCFA.dRFmin = dMinRatingFactor;
    RcfaB.DRCFA.dKRFmin = m_dK*dMinRatingFactor;

    // Calculation Rating Factor.
    if ( nPos==0 )
    {
        //0 Top of Top Flange
        //1 Bot of Top Flange
        //2 Top of Bot Flange
        //3 Bot of Bot Flange

        for ( int x = 0; x < 4; x++ )
        {
            if ( RcfaB.bChkPosi[x]==FALSE ) continue;

            if ( fabs(m_dRatMax-RcfaB.dRatFact[x])<m_dZero )
            {
                // Skip.
            }
            else if ( RcfaB.dRatFact[x] < 0.0 )
            {
                // skip.
            }
            else if ( dStressI_RF[x] > RcfaB.dRatFact[x] )
            {
                dStressI_RF[x] = RcfaB.dRatFact[x];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiInfR[x]);
            }
            else if ( dStressI_RF[x] == 0.0 && RcfaB.dRatFact[x] == 0.0 )
            {
                if ( dStressI_CADE[x] > RcfaB.dCapaDemand[x] )
                {
                    dStressI_CADE[x] = RcfaB.dCapaDemand[x];
                    SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiInfR[x]);
                }
            }
        }
    }
    else
    {
        //0 Top of Top Flange
        //1 Bot of Top Flange
        //2 Top of Bot Flange
        //3 Bot of Bot Flange
        for ( int y = 0; y < 4; y++ )
        {
            if ( RcfaB.bChkPosi[y]==FALSE ) continue;

            if ( fabs(m_dRatMax-RcfaB.dRatFact[y])<m_dZero )
            {
                // Skip.
            }
            else if ( RcfaB.dRatFact[y] < 0.0 )
            {
                // skip.
            }
            else if ( dStressJ_RF[y] > RcfaB.dRatFact[y] )
            {
                dStressJ_RF[y] = RcfaB.dRatFact[y];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiInfR[4+y]);
            }
            else if ( dStressJ_RF[y] == 0.0 && RcfaB.dRatFact[y] == 0.0 )
            {
                if ( dStressJ_CADE[y] > RcfaB.dCapaDemand[y] )
                {
                    dStressJ_CADE[y] = RcfaB.dCapaDemand[y];
                    SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiInfR[4+y]);
                }
            }
        }
    }

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckFiniteFatigueRatingAASHTO(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
    CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
    T_RLCS_D &RlcsD, double dCapacityF, double dCapaN[4],
    double dStrDelMV_PR[4], double dStrDelMV_AD[4],
    T_RCFA_FIN_BASE &RcfaB, double dStressI_RF[4], double dStressJ_RF[4], double dStressI_CADE[4], double dStressJ_CADE[4],
    T_RCFA_FIN_LIFE_D &RcfaD)
{
    CSG_MEMBPOS_RES_AASHTO_LRFD_D TMembResAASHTO = MembResAASHTO;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pAASHTO = &TMembResAASHTO;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_MEMB_RES_AASHTO_LRFD_D TRes;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pAASHTO = &TRes;

    MembLcomD.aChkFor[0].nLcomType=1;
    m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);

    CSG_MEMB_RES_AASHTO_LRFD_SER_D &MembServD = TMembR.pAASHTO->MembR[nPos].ServiceD;

    CSG_LCOM_RES_D &ForD = MembServD.ForceD;

    CSG_LCOM_FLANGE_STRESS_D& FlgStrs =  MembServD.ForceD.FlgStrs4Posi;

    BOOL bPositive = (ForD.dMu>=0.0);
    BOOL bIsTopC   = bPositive;

    RcfaB.DRCFA.StrDC_Be.dStrT_Top = FlgStrs.dft_gr_Top;
    RcfaB.DRCFA.StrDC_Be.dStrT_Bot = FlgStrs.dft_gr_Bot;
    RcfaB.DRCFA.StrLT.dStrT_Top    = FlgStrs.dft_lt_Top;
    RcfaB.DRCFA.StrLT.dStrT_Bot    = FlgStrs.dft_lt_Bot;
    RcfaB.DRCFA.StrST.dStrT_Top    = FlgStrs.dft_st_Top;
    RcfaB.DRCFA.StrST.dStrT_Bot    = FlgStrs.dft_st_Bot;
    RcfaB.DRCFA.StrDC_Be.dStrB_Top = FlgStrs.dfb_gr_Top;
    RcfaB.DRCFA.StrDC_Be.dStrB_Bot = FlgStrs.dfb_gr_Bot;
    RcfaB.DRCFA.StrLT.dStrB_Top    = FlgStrs.dfb_lt_Top;
    RcfaB.DRCFA.StrLT.dStrB_Bot    = FlgStrs.dfb_lt_Bot;
    RcfaB.DRCFA.StrST.dStrB_Top    = FlgStrs.dfb_st_Top;
    RcfaB.DRCFA.StrST.dStrB_Bot    = FlgStrs.dfb_st_Bot;

    RcfaB.DRCFA.dRT     = m_dWeight; // 우선 여기에 할당

    RcfaB.DRCFA.StrPR.dStrDelT_Top = dStrDelMV_PR[0];
    RcfaB.DRCFA.StrPR.dStrDelT_Bot = dStrDelMV_PR[1];
    RcfaB.DRCFA.StrPR.dStrDelB_Top = dStrDelMV_PR[2];
    RcfaB.DRCFA.StrPR.dStrDelB_Bot = dStrDelMV_PR[3];


    T_RPAS_D RpasD; RpasD.Initialize();
    m_pDoc->m_pAttrCtrl2->GetRpas(RpasD);

    double dDeadComp[4] = { RcfaB.DRCFA.StrLT.dStrT_Top, RcfaB.DRCFA.StrLT.dStrT_Bot, RcfaB.DRCFA.StrLT.dStrB_Top, RcfaB.DRCFA.StrLT.dStrB_Bot };

    double dRsFact = 1.0;

    GetFatigueParamFIN(ElemK.first, nPos, RcfaB);

    RcfaB.dRp = CalcRpFact(RcfaB.dSpan, RcfaB.dADTT_Present, RcfaB.dnL);

    for ( int i=0; i<4; ++i )
    {
        RcfaB.dDelF[i] = fabs(dStrDelMV_PR[i]);

        RcfaB.dDelFeff[i] = dRsFact *  RcfaB.dRp * fabs(dStrDelMV_PR[i]);

        RcfaB.dDelFmax[i] = CalcDelFmax(INF_FATI, RlcsD.nEvaluation, RcfaB.dDelFeff[i]);

        RcfaB.dCapaDemand[i] = fabs(dCapaN[i]);// - fabs(dfst[i]) - fabs(dStrDelMV_AD[i]);

        RcfaB.dRatFact[i] = (RcfaB.dDelFmax[i] == 0.0) ? m_dRatMax : RcfaB.dCapaDemand[i] / RcfaB.dDelFmax[i];

        if ( 2.2 *  RcfaB.dDelFeff[i] < -1.0 * dDeadComp[i] )  RcfaB.bSkip[i] = TRUE;
    }

    std::vector<double> RatFact;
    for ( int m = 0; m < 4; m++ )
    {
        RatFact.emplace_back(RcfaB.dRatFact[m]);
    }

    double dMinRatingFactor = *std::min_element(RatFact.begin(), RatFact.end());

    for ( int n = 0; n < 4; n++ )
    {
        if ( dgn::EQ(RcfaB.dRatFact[n], dMinRatingFactor) )
        {
            RcfaB.nCrMinTypeDetail = n;
        }
    }

    RcfaB.DRCFA.dRFmin = dMinRatingFactor;

    // Calculation Rating Factor.
    if ( nPos==0 )
    {
        //0 Top of Top Flange
        //1 Bot of Top Flange
        //2 Top of Bot Flange
        //3 Bot of Bot Flange

        for ( int x = 0; x < 4; x++ )
        {
            if ( RcfaB.bChkPosi[x]==FALSE ) continue;

            if ( fabs(m_dRatMax-RcfaB.dRatFact[x])<m_dZero )
            {
                // Skip.
            }
            else if ( RcfaB.dRatFact[x] < 0.0 )
            {
                // skip.
            }
            else if ( dStressI_RF[x] > RcfaB.dRatFact[x] )
            {
                dStressI_RF[x] = RcfaB.dRatFact[x];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiFinR[x]);
            }
            else if ( dStressI_RF[x] == 0.0 && RcfaB.dRatFact[x] == 0.0 )
            {
                if ( dStressI_CADE[x] > RcfaB.dCapaDemand[x] )
                {
                    dStressI_CADE[x] = RcfaB.dCapaDemand[x];
                    SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiFinR[x]);
                }
            }
        }
    }
    else
    {
        //0 Top of Top Flange
        //1 Bot of Top Flange
        //2 Top of Bot Flange
        //3 Bot of Bot Flange
        for ( int y = 0; y < 4; y++ )
        {
            if ( RcfaB.bChkPosi[y]==FALSE ) continue;

            if ( fabs(m_dRatMax-RcfaB.dRatFact[y])<m_dZero )
            {
                // Skip.
            }
            else if ( RcfaB.dRatFact[y] < 0.0 )
            {
                // skip.
            }
            else if ( dStressJ_RF[y] > RcfaB.dRatFact[y] )
            {
                dStressJ_RF[y] = RcfaB.dRatFact[y];
                SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiFinR[4+y]);
            }
            else if ( dStressJ_RF[y] == 0.0 && RcfaB.dRatFact[y] == 0.0 )
            {
                if ( dStressJ_CADE[y] > RcfaB.dCapaDemand[y] )
                {
                    dStressJ_CADE[y] = RcfaB.dCapaDemand[y];
                    SetRccrBaseData(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RcfaD.FatiFinR[4+y]);
                }
            }
        }
    }


    for ( int k=0; k<4; k++ ) //0 Minimum, 1 Eval1, 2 Eval2, 3 Mean
    {
        CheckFiniteFatigueRatingForEach(ElemK, nPos, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr,
            MembLcomD, RlcsD, dCapacityF, dCapaN,
            dStrDelMV_PR, dStrDelMV_AD,
            RcfaB, dStressI_RF, dStressJ_RF, dStressI_CADE, dStressJ_CADE,
            RcfaD, k, RcfaB.FinLife[k]);
    }
    
    return TRUE;
}


BOOL CDgnStlRatingDataCtrl::CheckFiniteFatigueRatingForEach(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
    CSG_MEMB_POSD_KSCE_LSD &MembLcomD, T_RLCS_D &RlcsD, double dCapacityF, double dCapaN[4],
    double dStrDelMV_PR[4], double dStrDelMV_AD[4],
    T_RCFA_FIN_BASE &RcfaB, double dStressI_RF[4], double dStressJ_RF[4], double dStressI_CADE[4], double dStressJ_CADE[4],
    T_RCFA_FIN_LIFE_D &RcfaD, int iLifeType, T_RATING_FIN_FATI_LIFE &FinLife)
{
    T_RPAS_D RpasD; RpasD.Initialize();
    m_pDoc->m_pAttrCtrl2->GetRpas(RpasD);

    for ( int k = 0; k < 4; k++ )
    {
        FinLife.LoadCapa[k].dpDelF_n = dCapaN[k];
    }

    double dRsFact = 1.0;

    switch ( RlcsD.nEvaluMethod )
    {
    case REFINED:
        switch ( RlcsD.nEvaluation )
        {
        case LRFD:
            dRsFact = 0.95;
            break;
        case TRUCK_SURVEY:
            dRsFact = 0.9;
            break;
        default:
            ASSERT(0);
            break;
        }
        break;
    case SIMPLIFIED:
        switch ( RlcsD.nEvaluation )
        {
        case LRFD:
            dRsFact = 1.0;
            break;
        case TRUCK_SURVEY:
            dRsFact = 0.95;
            break;
        default:
            ASSERT(0);
            break;
        }
        break;
    default:
        ASSERT(0);
        break;
    }

    if ( iLifeType == MEAN ) dRsFact = 1.0;

    GetFatigueParamFIN(ElemK.first, nPos, RcfaB);

    RcfaB.dRp = CalcRpFact(RcfaB.dSpan, RcfaB.dADTT_Present, RcfaB.dnL);

    FinLife.dRs = dRsFact;

    for ( int i=0; i<4; ++i )
    {
        FinLife.LoadCapa[i].dDelFeff = FinLife.dRs * RcfaB.dRp * fabs(dStrDelMV_PR[i]);
        FinLife.LoadCapa[i].dDelFmax = CalcDelFmax(FIN_FATI, RlcsD.nEvaluation, FinLife.LoadCapa[i].dDelFeff);
    }

    double dDeadComp[4] = { RcfaB.DRCFA.StrLT.dStrT_Top, RcfaB.DRCFA.StrLT.dStrT_Bot, RcfaB.DRCFA.StrLT.dStrB_Top, RcfaB.DRCFA.StrLT.dStrB_Bot };

    for ( int m = 0; m<4; m++ )
    {
        FinLife.dDel_Fact_f_Tens[m] = 2.2 * FinLife.LoadCapa[m].dDelFeff;
        if ( RcfaB.dRatFact[m] >= 1.0 ) FinLife.bOnlyInfinite[m] = TRUE;
        if ( FinLife.bOnlyInfinite[m] == TRUE || RcfaB.bSkip[m] == TRUE ) continue;

        int nDetailCategory = FinLife.nCategory[m];

        switch ( iLifeType )
        {
        case MINIMUM:
            FinLife.dRr[m] = 1.0;
            break;
        case EVALUATION1:
            switch ( nDetailCategory )
            {
            case CATE_A:
                FinLife.dRr[m] = 1.5;
                break;
            case CATE_E:
                FinLife.dRr[m] = 1.2;
                break;
            default:
                FinLife.dRr[m] = 1.3;
                break;
            }
            break;
        case EVALUATION2:
            switch ( nDetailCategory )
            {
            case CATE_A:
                FinLife.dRr[m] = 2.2;
                break;
            case CATE_B_P:
            case CATE_E_P:
                FinLife.dRr[m] = 1.6;
                break;
            case CATE_E:
                FinLife.dRr[m] = 1.4;
                break;
            default:
                FinLife.dRr[m] = 1.7;
                break;
            }
            break;
        case MEAN:
            switch ( nDetailCategory )
            {
            case CATE_A:
                FinLife.dRr[m] = 2.9;
                break;
            case CATE_B:
            case CATE_D:
                FinLife.dRr[m] = 2.0;
                break;
            case CATE_B_P:
            case CATE_E_P:
                FinLife.dRr[m] = 1.9;
                break;
            case CATE_C:
            case CATE_C_P:
                FinLife.dRr[m] = 2.1;
                break;
            case CATE_E:
                FinLife.dRr[m] = 1.6;
                break;
            default:
                ASSERT(0);
                break;
            }
            break;
        default:
            ASSERT(0);
            break;
        }

        Convert_Finite_Fatigue_Value(RpasD, RcfaB);

        FinLife.dNav[m] = FinLife.dRr[m] * RcfaB.dA[m] / pow( FinLife.LoadCapa[m].dDelFeff, 3.0 );

        if ( RcfaB.dADTT_SL_Present == RcfaB.dADTT_SL_0 )
        {
            RcfaB.dN1 = 365.0 * RcfaB.dn * (RcfaB.dCur_Age + 1.0) * RcfaB.dADTT_SL_Present;
        }
        else
        {
            double dN1part1 = 365.0 * RcfaB.dn * RcfaB.dADTT_SL_Present;
            double dN1part2 = 1.0 - RcfaB.dADTT_SL_0 / RcfaB.dADTT_SL_Present;
            double dN1part3 = pow(RcfaB.dADTT_SL_Present / RcfaB.dADTT_SL_0, 1.0/RcfaB.dCur_Age) - 1.0;

            RcfaB.dN1 = dN1part1 * (dN1part2 / dN1part3 + 1);
        }

        if ( FinLife.dNav[m] > RcfaB.dN1 ) FinLife.bRemainFatiLife[m] = TRUE;


        if ( FinLife.bRemainFatiLife[m] == TRUE )
        {
            double dRemainLifePart1 = log10(( RcfaB.dAnnualGrowth / (1.0 + RcfaB.dAnnualGrowth)) * ((FinLife.dNav[m] - RcfaB.dN1) / (365.0 * RcfaB.dn * RcfaB.dADTT_SL_Present)) + 1.0);
            double dRemainLifePart2 = log10(1.0 + RcfaB.dAnnualGrowth);
            double dRemainLife = dRemainLifePart1 / dRemainLifePart2;

            FinLife.dADTT_SL_Future[m] = RcfaB.dADTT_SL_Present * pow(1.0 + RcfaB.dAnnualGrowth, dRemainLife);

            double dY_ADTT_Limit = 0.0;
            double dRemainLifeModi = 0.0;

            FinLife.dY_Rem[m] = dRemainLife;

            if ( RcfaB.dADTT_SL_Limit > FinLife.dADTT_SL_Future[m] )
            {
                dRemainLifeModi = dRemainLife;
            }
            else
            {
                dY_ADTT_Limit = log10(RcfaB.dADTT_SL_Limit / RcfaB.dADTT_SL_Present) / log10(1.0 + RcfaB.dAnnualGrowth);
                double dRemainLifeModiPart1 = (FinLife.dNav[m] - RcfaB.dN1) / (365.0 * RcfaB.dn * RcfaB.dADTT_SL_Limit);
                double dRemainLifeModiPart2 = (pow(1.0 + RcfaB.dAnnualGrowth, dY_ADTT_Limit) - 1.0) / (RcfaB.dAnnualGrowth * pow(1.0 + RcfaB.dAnnualGrowth, dY_ADTT_Limit - 1.0));
                dRemainLifeModi = dRemainLifeModiPart1 - dRemainLifeModiPart2 + dY_ADTT_Limit;

            }

            FinLife.dY_ADTT_Limit[m] = dY_ADTT_Limit;
            FinLife.dY_Rem_Mod[m] = dRemainLifeModi;
            FinLife.dY[m] = dRemainLifeModi + RcfaB.dCur_Age;
        }
        else
        {
            FinLife.dY_Rem[m] = 0.0;
            FinLife.dY[m] = RcfaB.dCur_Age;
        }
        FinLife.dGreaterY[m] = max(FinLife.dY[m], 100.0);

        FinLife.dSerIndex[m] = RcfaB.dG * RcfaB.dI * RcfaB.dR * (FinLife.dY[m] - RcfaB.dCur_Age) / FinLife.dGreaterY[m];
    }
    return TRUE;
}

double CDgnStlRatingDataCtrl::CalcDelFmax(int iFatiType, int iTruckType, double dDelfeff)
{
    double dTrukFact = 0.0;
    double dFatiFact = 0.0;

    switch ( iFatiType )
    {
    case INF_FATI:
        if ( iTruckType == LRFD )
        {
            return 1.75 * dDelfeff;
        }
        else
        {
            return 2.2 * dDelfeff;
        }
        break;
    case FIN_FATI:
        if ( iTruckType == LRFD )
        {
            return 0.8 * dDelfeff;
        }
        else
        {
            return 1.0 * dDelfeff;
        }
        break;
    default:
        break;
    }

    return dDelfeff;
}

void CDgnStlRatingDataCtrl::GetFatigueParamINF(T_ELEM_K ElemK, int nPos, T_RCFA_INF_BASE &RcfaD)
{
    double dSpanLength = 0.0;

    T_SPLN_D rSPLN; rSPLN.Initialize();
    T_SPAN_D SpanD; SpanD.Initialize();
    BLR_SPAN_D BlrSpanD; BlrSpanD.Initialize();
    CArray<T_SPAN_K, T_SPAN_K> SpanKeyList;
    m_pDoc->m_pAttrCtrl->GetSpanKeyList(SpanKeyList);
    const int iSpanKeyNum = SpanKeyList.GetSize();

    ElemPairK EPairK(ElemK, EN_EL_BEAM);
    if ( m_pDataCtrl->Get_ElemSpanInfo(EPairK, BlrSpanD) )
    {
        double dTotalLength = 0.0;
        for ( int j = 0; j < iSpanKeyNum; j++ )
        {
            if ( dTotalLength > 0.0 ) break;
            T_SPAN_K SpanK = SpanKeyList[j];
            m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
            int iSpanElemNum = SpanD.aElemBase.GetSize();

            int iCount = 0;

            if ( SpanD.aSpan.GetSize() == 0 ) break;

            for ( int k=0; k<iSpanElemNum; ++k )
            {
				const auto& CurElemB = SpanD.aElemBase[k];
                if (k == 0)
                {
                    if (CurElemB.nSupport == 1) iCount++;
                }
                else
                {
                    if (CurElemB.nSupport == 1 || CurElemB.nSupport == 2) iCount++;
                }
                
                if ( CurElemB.ElemK == ElemK )
                {
                    if (iCount == 0)
                    {
                        dTotalLength = SpanD.aSpan.GetSize()>0 ? SpanD.aSpan[0] : 0.0;
                    }
                    else
                    {
                        dTotalLength = SpanD.aSpan[iCount - 1];
                    }                    
                    break;
                }
            }
        }
        dSpanLength = (dTotalLength) / 12.0 ;
    }
    else if ( m_pDoc->m_pAttrCtrl->ExistSpln(ElemK) )
    {
        m_pDoc->m_pAttrCtrl->GetSpln(ElemK, rSPLN);
        dSpanLength = rSPLN.dSpanL / 12.0; //inch가 아닌 feet로 변환
    }
    else
    {
        dSpanLength = m_pDoc->calcLAVElem(ElemK) / 12.0; //inch가 아닌 feet로 변환;
    }

    if ( EQ0(dSpanLength) ) dSpanLength = m_pDoc->calcLAVElem(ElemK) / 12.0; //inch가 아닌 feet로 변환;

    T_CPFP_D rCPFP;
    rCPFP.Initialize();
    if ( m_pDoc->m_pAttrCtrl2->ExistCpfp(ElemK) )
    {
        m_pDoc->m_pAttrCtrl2->GetCpfp(ElemK, rCPFP);
        RcfaD.bChkPosi[0] = rCPFP.TfType[nPos].bTopF_Top;
        RcfaD.bChkPosi[1] = rCPFP.TfType[nPos].bTopF_Bot;
        RcfaD.bChkPosi[2] = rCPFP.TfType[nPos].bBotF_Top;
        RcfaD.bChkPosi[3] = rCPFP.TfType[nPos].bBotF_Bot;

        RcfaD.dADTT_Present = rCPFP.TfType[nPos].dADTT_Pr;
        RcfaD.dSpan = dSpanLength;
        RcfaD.dnL = max(rCPFP.TfType[nPos].dNL, 1.0);
        RcfaD.dn = max(rCPFP.TfType[nPos].dNum, 1.0);
    }
    else
    {
        RcfaD.bChkPosi[0] = FALSE;
        RcfaD.bChkPosi[1] = FALSE;
        RcfaD.bChkPosi[2] = FALSE;
        RcfaD.bChkPosi[3] = FALSE;

        RcfaD.dADTT_Present = 0.0;
        RcfaD.dSpan = dSpanLength;
        RcfaD.dnL = 1.0;
        RcfaD.dn = 1.0;
    }
}


void CDgnStlRatingDataCtrl::GetFatigueParamFIN(T_ELEM_K ElemK, int nPos, T_RCFA_FIN_BASE &RcfaD)
{
    double dSpanLength = 0.0;

    T_SPLN_D rSPLN; rSPLN.Initialize();
    T_SPAN_D SpanD; SpanD.Initialize();
    BLR_SPAN_D BlrSpanD; BlrSpanD.Initialize();
    CArray<T_SPAN_K, T_SPAN_K> SpanKeyList;
    m_pDoc->m_pAttrCtrl->GetSpanKeyList(SpanKeyList);
    const int iSpanKeyNum = SpanKeyList.GetSize();

    ElemPairK EPairK(ElemK, EN_EL_BEAM);
    if ( m_pDataCtrl->Get_ElemSpanInfo(EPairK, BlrSpanD) )
    {
        double dTotalLength = 0.0;
        for ( int j = 0; j < iSpanKeyNum; j++ )
        {
            if ( dTotalLength > 0.0 ) break;
            T_SPAN_K SpanK = SpanKeyList[j];
            m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
            int iSpanElemNum = SpanD.aElemBase.GetSize();

            int iCount = 0;

            if ( SpanD.aSpan.GetSize() == 0 ) break;

            for ( int k=0; k<iSpanElemNum; ++k )
            {
                const auto& CurElemB = SpanD.aElemBase[k];
                if (k == 0)
                {
                    if (CurElemB.nSupport == 1) iCount++;
                }
                else
                {
                    if (CurElemB.nSupport == 1 || CurElemB.nSupport == 2) iCount++;
                }

                if (CurElemB.ElemK == ElemK)
                {
                    if (iCount == 0)
                    {
                        dTotalLength = SpanD.aSpan.GetSize() > 0 ? SpanD.aSpan[0] : 0.0;
                    }
                    else
                    {
                        dTotalLength = SpanD.aSpan[iCount - 1];
                    }
                    break;
                }
            }
        }
        dSpanLength = (dTotalLength) / 12.0;
    }
    else if ( m_pDoc->m_pAttrCtrl->ExistSpln(ElemK) )
    {
        m_pDoc->m_pAttrCtrl->GetSpln(ElemK, rSPLN);
        dSpanLength = rSPLN.dSpanL / 12.0; //inch가 아닌 feet로 변환;
    }
    else
    {
        dSpanLength = m_pDoc->calcLAVElem(ElemK) / 12.0; //inch가 아닌 feet로 변환;
    }

    if ( EQ0(dSpanLength) ) dSpanLength = m_pDoc->calcLAVElem(ElemK) / 12.0; //inch가 아닌 feet로 변환;

    T_CPFP_D rCPFP;
    rCPFP.Initialize();
    if ( m_pDoc->m_pAttrCtrl2->ExistCpfp(ElemK) )
    {
        m_pDoc->m_pAttrCtrl2->GetCpfp(ElemK, rCPFP);
        RcfaD.bChkPosi[0] = rCPFP.TfType[nPos].bTopF_Top;
        RcfaD.bChkPosi[1] = rCPFP.TfType[nPos].bTopF_Bot;
        RcfaD.bChkPosi[2] = rCPFP.TfType[nPos].bBotF_Top;
        RcfaD.bChkPosi[3] = rCPFP.TfType[nPos].bBotF_Bot;

        RcfaD.dADTT_Present = rCPFP.TfType[nPos].dADTT_Pr;
        RcfaD.dADTT_SL_Present = rCPFP.TfType[nPos].dADTT;
        RcfaD.dSpan = dSpanLength;
        RcfaD.dnL = max(rCPFP.TfType[nPos].dNL, 1.0);
        RcfaD.dn = max(rCPFP.TfType[nPos].dNum, 1.0);

        for ( int k=0; k<4; k++ )
        {
            RcfaD.FinLife[k].nCategory[0] = rCPFP.TfType[nPos].nDetailCategory;
            RcfaD.FinLife[k].nCategory[1] = rCPFP.TfType[nPos].nDetailCategory2;
            RcfaD.FinLife[k].nCategory[2] = rCPFP.TfType[nPos].nDetailCategory3;
            RcfaD.FinLife[k].nCategory[3] = rCPFP.TfType[nPos].nDetailCategory4;
        }
    }
    else
    {
        RcfaD.bChkPosi[0] = FALSE;
        RcfaD.bChkPosi[1] = FALSE;
        RcfaD.bChkPosi[2] = FALSE;
        RcfaD.bChkPosi[3] = FALSE;

        RcfaD.dADTT_Present = 0.0;
        RcfaD.dADTT_SL_Present = 0.0;
        RcfaD.dSpan = dSpanLength;
        RcfaD.dnL = 1.0;
        RcfaD.dn = 1.0;
    }
}

double CDgnStlRatingDataCtrl::CalcRpFact(double dSpanLength, double dADTT_P, double dNumberLane)
{
    return max(0.988 + 6.87*pow(10.0, -5.0) * dSpanLength + 4.01*pow(10.0, -6.0)*dADTT_P + 0.0107/dNumberLane, 1.0);
}

void CDgnStlRatingDataCtrl::Convert_Finite_Fatigue_Value(T_RPAS_D RpasD, T_RCFA_FIN_BASE &RcfaB)
{
    RcfaB.dADTT_SL_0 = RpasD.dAdTTSL_0;
    RcfaB.dADTT_SL_Limit = RpasD.dAdTTSL_lim;
    RcfaB.dAnnualGrowth = RpasD.dAdTTSL_g;
    RcfaB.dCur_Age = RpasD.dCurAge_a;

    switch ( RpasD.nFatServ_G )
    {
    case 0:
        RcfaB.dG = 0.8;
        break;
    case 1:
        RcfaB.dG = 0.9;
        break;
    case 2:
        RcfaB.dG = 1.0;
        break;
    default:
        ASSERT(0);
        break;
    }

    switch ( RpasD.nFatServ_R )
    {
    case 0:
        RcfaB.dR = 0.9;
        break;
    case 1:
        RcfaB.dR = 1.0;
        break;
    default:
        ASSERT(0);
        break;
    }

    switch ( RpasD.nFatServ_I )
    {
    case 0:
        RcfaB.dI = 0.9;
        break;
    case 1:
        RcfaB.dI = 0.95;
        break;
    case 2:
        RcfaB.dI = 1.0;
        break;
    default:
        ASSERT(0);
        break;
    }
}


BOOL CDgnStlRatingDataCtrl::Check_RFminbyRatingCase()
{
    CArray<T_RLCS_K,T_RLCS_K> aRlcsKey;
    m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(aRlcsKey);
    int nRlcsKeySize = aRlcsKey.GetSize();

    int nElemSize = m_arElemK.GetSize();

    T_RCST_CASE RcstC;
    T_RCSS_CASE RcssC;
    T_RCFA_CASE RcfaC;

    double dMaxLimit = 1.0/pow(m_dZero,10);
    double dMinLimit = pow(m_dZero, 5);

    for (int i=0; i<nRlcsKeySize; ++i)
    {
        T_RLCS_K RlcsK = aRlcsKey[i];
        T_RLCS_D RlcsD; RlcsD.Initialize();
        m_pDoc->m_pAttrCtrl2->GetRlcs(RlcsK, RlcsD);

        double dRFmin_Flex[2]		= {dMaxLimit, dMaxLimit}; 
        double dRFmin_Flex_CaDe[2]	= {dMaxLimit, dMaxLimit}; 
        double dRFmin_Shear[2]		= {dMaxLimit, dMaxLimit};
        double dRFmin_Shear_CaDe[2] = {dMaxLimit, dMaxLimit}; 
        double dRFmin_Str[2]		= {dMaxLimit, dMaxLimit}; 
        double dRFmin_Str_CaDe[2]	= {dMaxLimit, dMaxLimit}; 
        double dRFmin_Fati[2]		= {dMaxLimit, dMaxLimit}; 
        double dRFmin_Fati_CaDe[2]	= {dMaxLimit, dMaxLimit}; 

        T_RSCR_STRN RscrStrn;
        T_RSCR_SERV RscrServ;
        T_RSCR_FATI RscrFati;

        for (int j=0; j<nElemSize; ++j)
        {
            auto ElemK = m_arElemK[j];
            T_GRUP_K CheckGroupK = 0; //rKeyList[0]; //arGrupList[0];
            if(!m_pDataCtrl->GetBlrCheckGroupKey(ElemK, CheckGroupK)) continue;

            for(int k=0; k<cRAT_LCASE; k++)
            {
                BOOL bDC_Max, bDW_Max, bTemp_Max;
                m_pDoc->Get_BlrcLoadTypeData(k, bDC_Max, bDW_Max, bTemp_Max);        

                if (IsSkipRatingCase(bDC_Max, bDW_Max, bTemp_Max, RlcsD)==FALSE) continue;

                for(int m=0; m<cFORC_CON_DGN; m++)
                {
                    int nConcurr=m+1;

                    T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr);

                    if (RlcsD.nLimitState==1)  // strength
                    {
                        RcstC.Initialize();
                        m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetStlStrengthRating(ElemK, RlcsKey, RcstC);

                        for (int n=0; n<2; ++n) // flexure
                        {
                            if (RcstC.RcstB[n].bChk==FALSE) continue;
                            if (fabs(RcstC.RcstB[n].dRF-m_dRatMax)<m_dZero) continue;

                            if(RcstC.RcstB[n].dRF < m_dZero || RcstC.RcstB[n].dKRF > dRFmin_Flex[n])
                            {
                                continue;// skip
                            }
                            else if (RcstC.RcstB[n].dKRF < MIN(dRFmin_Flex[0],dRFmin_Flex[1]) && RcstC.RcstB[n].dKRF > dMinLimit)
                            {
                                dRFmin_Flex[n] = RcstC.RcstB[n].dKRF;
                                SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.FlexR);
                            }
                            else if(RcstC.RcstB[n].dRF == 0.0 && dRFmin_Flex[n] == 0.0)
                            {
                                if(RcstC.RcstB[n].nCaseRes == 1 || RcstC.RcstB[n].nCaseRes == 4)
                                {
                                    if(dRFmin_Flex_CaDe[n] > RcstC.RcstB[n].dCaDeM )
                                    {
                                        dRFmin_Flex_CaDe[n] = RcstC.RcstB[n].dCaDeM;
                                        SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.FlexR);                 
                                    }
                                }
                                else if(RcstC.RcstB[n].nCaseRes == 2 || RcstC.RcstB[n].nCaseRes == 3)
                                {
                                    dRFmin_Flex_CaDe[n] = RcstC.RcstB[n].dCaDeF;
                                    SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.FlexR);    
                                }
                            }              
                        }

                        for (int n=0; n<2; ++n) // shear
                        {
                            if (RcstC.RcstB[n].bChk==FALSE) continue;
                            if (fabs(RcstC.RcstB[n].dRFsh-m_dRatMax)<m_dZero) continue;

                            if(RcstC.RcstB[n].dRFsh < m_dZero || RcstC.RcstB[n].dKRFsh > dRFmin_Shear[n])
                            { 
                                continue;// skip.
                            }
                            else if (RcstC.RcstB[n].dKRFsh < MIN(dRFmin_Shear[0],dRFmin_Shear[1]) && RcstC.RcstB[n].dKRFsh > dMinLimit)
                            {
                                dRFmin_Shear[n] = RcstC.RcstB[n].dKRFsh;
                                SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.ShearR);
                            }
                            else if(RcstC.RcstB[n].dRFsh == 0.0 || dRFmin_Shear[n] == 0.0)
                            {
                                if(dRFmin_Shear_CaDe[n] > RcstC.RcstB[n].dCaDeSh)
                                {
                                    dRFmin_Shear_CaDe[n] = RcstC.RcstB[n].dCaDeSh;
                                    SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.ShearR);
                                }
                            }
                        }

                    }

                    if (RlcsD.nLimitState==0) // service
                    {
                        RcssC.Initialize();
                        m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetStlStressRating(ElemK, RlcsKey, RcssC);

                        for (int n=0; n<2; ++n)
                        {
                            if (RcssC.StreB[n].bChk==FALSE) continue;
                            if (fabs(RcssC.StreB[n].dRFmin-m_dRatMax)<m_dZero) continue;

                            if(RcssC.StreB[n].dRFmin < m_dZero || RcssC.StreB[n].dKRFmin > dRFmin_Str[n])
                            {
                                continue;// skip.
                            }
                            else if (RcssC.StreB[n].dKRFmin < MIN(dRFmin_Str[0],dRFmin_Str[1]) && RcssC.StreB[n].dKRFmin > dMinLimit)
                            {
                                dRFmin_Str[n] = RcssC.StreB[n].dKRFmin;
                                SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrServ.StressR);
                            }
                            else if(RcssC.StreB[n].dRFmin == 0.0 || dRFmin_Str[n] == 0.0)
                            {
                                if(dRFmin_Str_CaDe[n] > RcssC.StreB[n].dCaDe[RcssC.StreB[n].nCrMinType])
                                {
                                    dRFmin_Str_CaDe[n] = RcssC.StreB[n].dCaDe[RcssC.StreB[n].nCrMinType];
                                    SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrServ.StressR);
                                }
                            }
                        }

                    }

                    if (RlcsD.nLimitState==2)  // Fatigue 
                    {
                        RcfaC.Initialize();
                        m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetStlFatigueRating(ElemK, RlcsKey, RcfaC);

                        for (int n=0; n<2; ++n)
                        {
                            if (RcfaC.FatiB[n].bChk==FALSE) continue;
                            if (fabs(RcfaC.FatiB[n].dRFmin-m_dRatMax)<m_dZero) continue;

                            if(RcfaC.FatiB[n].dRFmin < 0.0 || RcfaC.FatiB[n].dKRFmin > dRFmin_Fati[n])
                            {
                                continue;// skip.
                            }
                            else if (RcfaC.FatiB[n].dKRFmin < MIN(dRFmin_Fati[0],dRFmin_Fati[1]) && RcfaC.FatiB[n].dKRFmin > dMinLimit)
                            {
                                dRFmin_Fati[n] = RcfaC.FatiB[n].dKRFmin;
                                SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrFati.FatiR);
                            }
                            else if(RcfaC.FatiB[n].dRFmin == 0.0 || dRFmin_Fati[n] == 0.0)
                            {
                                if( dRFmin_Fati_CaDe[n] > RcfaC.FatiB[n].dCaDe[RcfaC.FatiB[n].nCrMinType])
                                {
                                    dRFmin_Fati_CaDe[n] = RcfaC.FatiB[n].dCaDe[RcfaC.FatiB[n].nCrMinType];
                                    SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrFati.FatiR);
                                }
                            }
                        }            
                    }          
                }
            } //k
        } //j

        if (RlcsD.nLimitState==1)
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRCaseStrnElem(RlcsK, RscrStrn);
        }

        if (RlcsD.nLimitState==0)
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRCaseServElem(RlcsK, RscrServ);
        }

        if (RlcsD.nLimitState==2)
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRCaseFatiElem(RlcsK, RscrFati);
        }
    }

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Check_RFminbyRatingCase19()
{
    CArray<T_RLCS_K, T_RLCS_K> aRlcsKey;
    m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(aRlcsKey);
    int nRlcsKeySize = aRlcsKey.GetSize();

    T_RPAS_D RpasD; RpasD.Initialize();
    m_pDoc->m_pAttrCtrl2->GetRpas(RpasD);

    int nElemSize = m_arElemK.GetSize();

    T_RCST_CASE RcstC;
    T_RCSS_CASE RcssC;

    double dMaxLimit = 1.0/pow(m_dZero, 10);
    double dMinLimit = pow(m_dZero, 5);

    for ( int i=0; i<nRlcsKeySize; ++i )
    {
        T_RLCS_K RlcsK = aRlcsKey[i];
        T_RLCS_D RlcsD; RlcsD.Initialize();
        m_pDoc->m_pAttrCtrl2->GetRlcs(RlcsK, RlcsD);

        double dRFmin_Flex[2]		= { dMaxLimit, dMaxLimit };
        double dRFmin_Flex_CaDe[2]	= { dMaxLimit, dMaxLimit };
        double dRFmin_Shear[2]		= { dMaxLimit, dMaxLimit };
        double dRFmin_Shear_CaDe[2] = { dMaxLimit, dMaxLimit };
        double dRFmin_Str[2]		= { dMaxLimit, dMaxLimit };
        double dRFmin_Str_CaDe[2]	= { dMaxLimit, dMaxLimit };

        T_RSCR_STRN RscrStrn;
        T_RSCR_SERV RscrServ;

        for ( int j=0; j<nElemSize; ++j )
        {
            auto ElemK = m_arElemK[j];
            T_GRUP_K CheckGroupK = 0; //rKeyList[0]; //arGrupList[0];
            if ( !m_pDataCtrl->GetBlrCheckGroupKey(ElemK, CheckGroupK) ) continue;

            for ( int k=0; k<cRAT_LCASE; k++ )
            {
                BOOL bDC_Max, bDW_Max, bTemp_Max;
                m_pDoc->Get_BlrcLoadTypeData(k, bDC_Max, bDW_Max, bTemp_Max);

                if ( IsSkipRatingCase(bDC_Max, bDW_Max, bTemp_Max, RlcsD)==FALSE ) continue;

                for ( int m=0; m<cFORC_CON_DGN; m++ )
                {
                    int nConcurr=m+1;

                    T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr);

                    if ( RpasD.bStrength == TRUE && RlcsD.nLimitState==1 )  // strength
                    {
                        RcstC.Initialize();
                        m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetStlStrengthRating(ElemK, RlcsKey, RcstC);

                        for ( int n=0; n<2; ++n ) // flexure
                        {
                            if ( RcstC.RcstB[n].bChk==FALSE ) continue;
                            if ( fabs(RcstC.RcstB[n].dRF-m_dRatMax)<m_dZero ) continue;

                            if ( RcstC.RcstB[n].dRF < m_dZero || RcstC.RcstB[n].dKRF > dRFmin_Flex[n] )
                            {
                                continue;// skip
                            }
                            else if ( RcstC.RcstB[n].dKRF < MIN(dRFmin_Flex[0], dRFmin_Flex[1]) && RcstC.RcstB[n].dKRF > dMinLimit )
                            {
                                dRFmin_Flex[n] = RcstC.RcstB[n].dKRF;
                                SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.FlexR);
                            }
                            else if ( RcstC.RcstB[n].dRF == 0.0 && dRFmin_Flex[n] == 0.0 )
                            {
                                if ( RcstC.RcstB[n].nCaseRes == 1 || RcstC.RcstB[n].nCaseRes == 4 )
                                {
                                    if ( dRFmin_Flex_CaDe[n] > RcstC.RcstB[n].dCaDeM )
                                    {
                                        dRFmin_Flex_CaDe[n] = RcstC.RcstB[n].dCaDeM;
                                        SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.FlexR);
                                    }
                                }
                                else if ( RcstC.RcstB[n].nCaseRes == 2 || RcstC.RcstB[n].nCaseRes == 3 )
                                {
                                    dRFmin_Flex_CaDe[n] = RcstC.RcstB[n].dCaDeF;
                                    SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.FlexR);
                                }
                            }
                        }

                        for (int n=0; n<2; ++n ) // shear
                        {
                            if ( RcstC.RcstB[n].bChk==FALSE ) continue;
                            if ( fabs(RcstC.RcstB[n].dRFsh-m_dRatMax)<m_dZero ) continue;

                            if ( RcstC.RcstB[n].dRFsh < m_dZero || RcstC.RcstB[n].dKRFsh > dRFmin_Shear[n] )
                            {
                                continue;// skip.
                            }
                            else if ( RcstC.RcstB[n].dKRFsh < MIN(dRFmin_Shear[0], dRFmin_Shear[1]) && RcstC.RcstB[n].dKRFsh > dMinLimit )
                            {
                                dRFmin_Shear[n] = RcstC.RcstB[n].dKRFsh;
                                SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.ShearR);
                            }
                            else if ( RcstC.RcstB[n].dRFsh == 0.0 || dRFmin_Shear[n] == 0.0 )
                            {
                                if ( dRFmin_Shear_CaDe[n] > RcstC.RcstB[n].dCaDeSh )
                                {
                                    dRFmin_Shear_CaDe[n] = RcstC.RcstB[n].dCaDeSh;
                                    SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrStrn.ShearR);
                                }
                            }
                        }

                    }

                    if ( RpasD.bService == TRUE && RlcsD.nLimitState==0 ) // service
                    {
                        RcssC.Initialize();
                        m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetStlStressRating(ElemK, RlcsKey, RcssC);

                        for ( int n=0; n<2; ++n )
                        {
                            if ( RcssC.StreB[n].bChk==FALSE ) continue;
                            if ( fabs(RcssC.StreB[n].dRFmin-m_dRatMax)<m_dZero ) continue;

                            if ( RcssC.StreB[n].dRFmin < m_dZero || RcssC.StreB[n].dKRFmin > dRFmin_Str[n] )
                            {
                                continue;// skip.
                            }
                            else if ( RcssC.StreB[n].dKRFmin < MIN(dRFmin_Str[0], dRFmin_Str[1]) && RcssC.StreB[n].dKRFmin > dMinLimit )
                            {
                                dRFmin_Str[n] = RcssC.StreB[n].dKRFmin;
                                SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrServ.StressR);
                            }
                            else if ( RcssC.StreB[n].dRFmin == 0.0 || dRFmin_Str[n] == 0.0 )
                            {
                                if ( dRFmin_Str_CaDe[n] > RcssC.StreB[n].dCaDe[RcssC.StreB[n].nCrMinType] )
                                {
                                    dRFmin_Str_CaDe[n] = RcssC.StreB[n].dCaDe[RcssC.StreB[n].nCrMinType];
                                    SetRccrBaseData(ElemK, n, CheckGroupK, RlcsK, bDC_Max, bDW_Max, bTemp_Max, nConcurr, RscrServ.StressR);
                                }
                            }
                        }

                    }
                }
            } //k
        } //j

        if ( RpasD.bStrength == TRUE && RlcsD.nLimitState==1 )
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRCaseStrnElem(RlcsK, RscrStrn);
        }

        if ( RpasD.bService == TRUE && RlcsD.nLimitState==0 )
        {
            m_pDoc->m_pPostCtrl->GetLoadRating()->WriteStlRCaseServElem(RlcsK, RscrServ);
        }
    }

    return TRUE;
}


BOOL CDgnStlRatingDataCtrl::CheckStrengthRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
    T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if (!MembResD.pCS457) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = *MembResD.pCS457;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;

    MembLcomD.aChkFor[0].nLcomType = 0;

    m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    //CSG_MEMB_RES_AASHTO_LRFD_STR_D &StrnRes = TRes.StrengthD;

    CheckFlexuralStrengthRCaseCS454(ElemK, nPos, nMovType, MembLcomD, *MembResD.pCS457, TRes, UlsB);

    CheckShearStrengthRCaseCS454(ElemK, nPos, nMovType, MembLcomD, *MembResD.pCS457, TRes, UlsB);

    CheckCombinedMomentShearRCaseCS454(ElemK, nPos, nMovType, MembLcomD, *MembResD.pCS457, TRes, UlsB);

    CheckFlangeWithLStiffenerBoxShapeRCaseCS454(ElemK, nPos, nMovType, MembLcomD, *MembResD.pCS457, TRes, UlsB);

    CheckWebWithLStiffenerBoxShapeRCaseCS454(ElemK, nPos, nMovType, MembLcomD, *MembResD.pCS457, TRes, UlsB);
    
    CheckSlabWithLStiffenerBoxShapeRCaseCS454(ElemK, nPos, nMovType, MembLcomD, *MembResD.pCS457, TRes, UlsB);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckServiceRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
    T_ASSL_CSG_BS_BASE& AsslB)
{
    if (!MembResD.pCS457) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = *MembResD.pCS457;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_SERV_RES_CS457 ServR;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pServD = &ServR;

    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;

    MembLcomD.aChkFor[0].nLcomType = 1;

    m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    BOOL bPositiveM = IsPositiveM(MembLcomD.aChkFor[0].ForLC.My());
    int nPN = bPositiveM ? 0 : 1;

    AsslB.dsig_top[0] = ServR.SigTop[0].dSigma;
    AsslB.dsig_top[1] = ServR.SigTop[1].dSigma;
    AsslB.dsig_bot[0] = ServR.SigBottom[0].dSigma;
    AsslB.dsig_bot[1] = ServR.SigBottom[1].dSigma;
    AsslB.dsig_t = ServR.dSigma_t;
    AsslB.dsig_c = ServR.dSigma_c;
    AsslB.dsig_c_lim = ServR.dSig_c_lim;
    AsslB.dsig_t_lim = ServR.dSig_t_lim;

    AsslB.dfr = ServR.dfr;
    AsslB.dfr_lim = ServR.dfr_lim;
    AsslB.nChkRebar = ServR.nChkRebar;

    AsslB.dft_s = ServR.dft_s;
    AsslB.dft_s_lim = ServR.dft_s_lim;

    AsslB.RFcom.dRA = AsslB.ComD.dFc * ServR.dSig_c_lim;
    AsslB.RFten.dRA = AsslB.ComD.dFc * ServR.dSig_t_lim;
    if ( ServR.bTopComp )
    {
        const CSG_WEB_PANEL_STRESS_CS457& Sig = ServR.bIsCompLeftCr ? ServR.SigTop[0] : ServR.SigTop[1];
        AsslB.RFcom.dSA  = Sig.dSigma;
        AsslB.RFcom.dSd  = Sig.dSigma_gr + Sig.dSigma_lt;
        AsslB.RFcom.dSst = Sig.dSigma_sv;
        AsslB.RFcom.dS   = Sig.dSigma_mv;
        //
        const CSG_WEB_PANEL_STRESS_CS457& SigB = ServR.bIsTensLeftCr ? ServR.SigBottom[0] : ServR.SigBottom[1];
        AsslB.RFten.dSA  = SigB.dSigma;
        AsslB.RFten.dSd  = SigB.dSigma_gr + SigB.dSigma_lt;
        AsslB.RFten.dSst = SigB.dSigma_sv;
        AsslB.RFten.dS   = SigB.dSigma_mv;
    }
    else
    {
        const CSG_WEB_PANEL_STRESS_CS457& SigB = ServR.bIsCompLeftCr ? ServR.SigBottom[0] : ServR.SigBottom[1];
        AsslB.RFcom.dSA  = SigB.dSigma;
        AsslB.RFcom.dSd  = SigB.dSigma_gr + SigB.dSigma_lt;
        AsslB.RFcom.dSst = SigB.dSigma_sv;
        AsslB.RFcom.dS   = SigB.dSigma_mv;
        //
        const CSG_WEB_PANEL_STRESS_CS457& SigT = ServR.bIsTensLeftCr ? ServR.SigTop[0] : ServR.SigTop[1];
        AsslB.RFten.dSA  = SigT.dSigma;
        AsslB.RFten.dSd  = SigT.dSigma_gr + SigT.dSigma_lt;
        AsslB.RFten.dSst = SigT.dSigma_sv;
        AsslB.RFten.dS   = SigT.dSigma_mv;
    }
    AsslB.nSectClass = TMembRes.MembR[nPos].SClassD[nPN].enClass;

    
    CalcReserveFactor(nMovType, AsslB.RFcom);
    CalcReserveFactor(nMovType, AsslB.RFten);

    AsslB.dA = min(AsslB.RFcom.dA, AsslB.RFten.dA);

    AsslB.ComD.bTopFlangeWithLS    = ServR.bIsTopFlangeWithLS ? TRUE : FALSE;
    AsslB.ComD.bBottomFlangeWithLS = ServR.bIsBotFlangeWithLS ? TRUE : FALSE;
    AsslB.ComD.bWebWithLS          = ServR.bIsWebWithLS ? TRUE : FALSE;        
    
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckShearConnectorRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD,
    T_ASLS_CSG_BS_BASE& AslsB)
{
    //if ( ResD.pSconD==NULL ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_SCON_RES_CS457 SconRes;
    CSG_CHECK_RES_CS457 TRes;

    int nLcomType = MembLcomD.aChkFor[0].nLcomType;
    BOOL bPositiveM = IsPositiveM(MembLcomD.aChkFor[0].ForLC.My());
    int nPN = bPositiveM ? 0 : 1;

    if ( nLcomType==0 ) // ULS
    {
        TRes.pSconD = &SconRes;
    }
    else // SLS
    {
        TRes.pServSconD = &SconRes;
    }

    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;

    m_pCSGDataCtrl->Check_ShearConnector(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);

    AslsB.dq   = SconRes.dq;
    AslsB.dPim = MembLcomD.Memb.PosD[nPos].Scon.dFrc; //.MembResD.MembR[nPos].SConnD.dP
    AslsB.dPa  = SconRes.dPa;
    AslsB.dqr  = SconRes.dqr;
    AslsB.dq_qr = SconRes.dq_qr_ratio;
    AslsB.dq_qr_lim = MembLcomD.Memb.PosD[nPos].Scon.dq_qr_ratio;
    AslsB.dRA  = AslsB.dqr;
    AslsB.dSA  = AslsB.dq;
    AslsB.dSd  = AslsB.dq;
    AslsB.dA   = AslsB.dq_qr;
    AslsB.nSectClass = TMembRes.MembR[nPos].SClassD[nPN].enClass;
    AslsB.nOK  = SconRes.bOK ? 1 : 0;

    AslsB.ComD.bTopFlangeWithLS    = SconRes.bIsTopFlangeWithLS ? TRUE : FALSE;
    AslsB.ComD.bBottomFlangeWithLS = SconRes.bIsBotFlangeWithLS ? TRUE : FALSE;
    AslsB.ComD.bWebWithLS          = SconRes.bIsWebWithLS ? TRUE : FALSE;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckFlexuralStrengthRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if ( UlsB.pAsfcB==NULL ) return FALSE;
    if ( ResD.pStrnD==NULL ) return FALSE;

    T_ASFC_CSG_BS_BASE& AsfcB = *UlsB.pAsfcB;

    CSG_FLEX_RES_CS457& FlexD = ResD.pStrnD->FlexD;

    AsfcB.dR  = FlexD.dM_R;
    AsfcB.dRA = AsfcB.ComD.dFc * FlexD.dM_R;
    CalcReserveFactor(nMovType, AsfcB);

    AsfcB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckShearStrengthRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if ( UlsB.pAsscB==NULL ) return FALSE;
    if ( ResD.pStrnD==NULL ) return FALSE;

    T_ASSC_CSG_BS_BASE& AsscB = *UlsB.pAsscB;

    CSG_FLEX_RES_CS457& FlexD = ResD.pStrnD->FlexD;

    CSG_SHEAR_RES_CS457& ShearD = ResD.pStrnD->ShearD;
    AsscB.dR  = ShearD.dV_D;
    AsscB.dRA = AsscB.ComD.dFc * ShearD.dV_D;
    CalcReserveFactor(nMovType, AsscB);

    AsscB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckCombinedMomentShearRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if ( UlsB.pAscbB==NULL ) return FALSE;
    if ( ResD.pStrnD==NULL ) return FALSE;
    if ( !IsCheckPosition(nPos) ) return FALSE;

    T_ASCB_CSG_BS_BASE& AscbB = *UlsB.pAscbB;

    CSG_FLEX_RES_CS457& FlexD = ResD.pStrnD->FlexD;

    const CSG_COMBINED_MV_RES_CS457& Comb = ResD.pStrnD->CombD;
    const CSG_FORCE_COMP& ForceD = MembLcomD.aChkFor[0];
    
    AscbB.dMf  = ResD.pStrnD->FlexD.dMf;
    AscbB.dM_D = ResD.pStrnD->FlexD.dM_D;
    AscbB.dV_D = ResD.pStrnD->ShearD.dV_D;
    AscbB.dV_R = ResD.pStrnD->ShearD.dV_R;
    AscbB.dM_SV = Comb.ForStrD.dMu_mv;
    AscbB.dV_SV = Comb.ForStrD.dVu_mv;
    AscbB.dM_ST = ForceD.ForSV.My();//Comb.ForStrD.dMu_st;
    AscbB.dV_ST = ForceD.ForSV.Fz();//Comb.ForStrD.dVu_st;
    AscbB.dM_DL = Comb.ForStrD.dMu_lt + Comb.ForStrD.dMu_gr;
    AscbB.dV_DL = Comb.ForStrD.dVu_lt + Comb.ForStrD.dVu_gr;
    AscbB.dA   = Comb.dRatioMax;    

    AscbB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckTorsionalStrengthRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    return TRUE;
}



BOOL CDgnStlRatingDataCtrl::CheckLongitudinalShearRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if ( UlsB.pAslsB==NULL ) return FALSE;
    //if ( ResD.pStrnD==NULL ) return FALSE;

    ASSERT(0);    

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckFlangeWithLStiffenerBoxShapeRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if ( UlsB.pAsfgB==NULL ) return FALSE;
    if ( UlsB.pAsfgTenB==NULL ) return FALSE;
    if ( ResD.pStrnD==NULL ) return FALSE;
    if ( !IsCheckPosition(nPos) ) return FALSE;

    int nStype = MembLcomD.Memb.PosD[nPos].Sect.SectInfo.nStype;
        
    // Web 보강재가 있고 , Flange에 보강재가 있거나 없거나. 
    T_ASFG_CSG_BS_BASE& AsfgC = *UlsB.pAsfgB;
    T_ASFG_CSG_BS_BASE& AsfgT = *UlsB.pAsfgTenB;

    const CSG_LCOM_RES_D& ElemForceD = ResD.pStrnD->FlexD.ForStrD; // 여기는 압축(-), 인장(+)
    const CSG_FLEX_RES_CS457& FlexD = ResD.pStrnD->FlexD;
    const CSG_FLEX_FLG_W_LS_RES_CS457& FlgTop = FlexD.WithLS;
    const CSG_FLEX_FLG_W_LS_RES_CS457& FlgBot = FlexD.WithLSBot;
    const CSG_FLANGE_LS_RES_CS457& FlgD = ResD.pStrnD->FlangeD;

    BOOL bPositiveM = ElemForceD.dMu >= 0.0;
    BOOL bIsTopComp = TRUE;

    BOOL bApp9_10_2_1 = TRUE;
    if ( bApp9_10_2_1 )
    {
        if ( ElemForceD.dft<0.0 || fabs(ElemForceD.dft)<m_dZero)
        {
            bIsTopComp = TRUE;
            if ( FlexD.bIsCompFlangeWithLS )
            {
                AsfgC.dSig_yf   = FlexD.dSigma_ca;
                AsfgC.dSig_f_DL = -1.0*(ElemForceD.FlgStrs4Posi.dft_lt_mid + ElemForceD.FlgStrs4Posi.dft_gr_mid);
                AsfgC.dSig_f_ST = -1.0*ElemForceD.FlgStrs4Posi.dft_sd_mid;
                AsfgC.dSig_f_SV = -1.0*ElemForceD.FlgStrs4Posi.dft_mv_mid;
                AsfgC.dTau_DL   = FlgTop.DL.dtau;
                AsfgC.dTau_ST   = FlgTop.Std.dtau;
                AsfgC.dTau_SV   = FlgTop.SV.dtau;
                AsfgC.dRA       = AsfgC.ComD.dFc * FlgTop.dSigma_lim2;
                AsfgC.dSA       = FlgTop.LC.dDemand;
                AsfgC.dSd       = FlgTop.DL.dDemand;
                AsfgC.dSst      = FlgTop.Std.dDemand;
                AsfgC.dS        = FlgTop.SV.dDemand;
                AsfgC.dSd_Sst   = FlgTop.DL_Std.dDemand;
            }
            else
            {
                AsfgC.dSig_yf   = FlexD.dSigma_ca;
                AsfgC.dSig_f_DL = -1.0*(ElemForceD.dft_lt + ElemForceD.dft_gr);
                AsfgC.dSig_f_ST = -1.0*ElemForceD.dft_sv;
                AsfgC.dSig_f_SV = -1.0*ElemForceD.dft_mv;
                AsfgC.dTau_DL   = FlgTop.DL.dtau;
                AsfgC.dTau_ST   = FlgTop.Std.dtau;
                AsfgC.dTau_SV   = FlgTop.SV.dtau;
                //AsfgC.dR        = FlgTop.dSigma_lim2;
                AsfgC.dRA       = AsfgC.ComD.dFc * FlexD.dSigma_ca;
                AsfgC.dSA       = -1.0*ElemForceD.dft;
                AsfgC.dSd       = AsfgC.dSig_f_DL;
                AsfgC.dSst      = -1.0*ElemForceD.dft_sv;
                AsfgC.dS        = -1.0*ElemForceD.dft_mv;
            }
            //
            AsfgT.dSig_yf   = FlexD.dSigma_ta;
            AsfgT.dSig_f_DL = -1.0*(ElemForceD.dfb_lt + ElemForceD.dfb_gr);
            AsfgT.dSig_f_ST = -1.0*ElemForceD.dfb_sv;
            AsfgT.dSig_f_SV = -1.0*ElemForceD.dfb_mv;
            AsfgT.dTau_DL   = FlgBot.DL.dtau;
            AsfgT.dTau_ST   = FlgBot.Std.dtau;
            AsfgT.dTau_SV   = FlgBot.SV.dtau;
            //AsfgT.dR        = FlgBot.dSigma_lim2;
            AsfgT.dRA       = AsfgT.ComD.dFc * FlexD.dSigma_ta;
            AsfgT.dSA       = -1.0*ElemForceD.dfb;
            AsfgT.dSd       = AsfgT.dSig_f_DL;
            AsfgT.dSst      = -1.0*ElemForceD.dfb_sv;
            AsfgT.dS        = -1.0*ElemForceD.dfb_mv;
        }
        else
        {
            bIsTopComp = FALSE;
            if ( FlexD.bIsCompFlangeWithLS )
            {
                AsfgC.dSig_yf   = FlexD.dSigma_ca;
                AsfgC.dSig_f_DL = -1.0*(ElemForceD.FlgStrs4Posi.dfb_lt_mid + ElemForceD.FlgStrs4Posi.dfb_gr_mid);
                AsfgC.dSig_f_ST = -1.0*ElemForceD.FlgStrs4Posi.dfb_sd_mid;
                AsfgC.dSig_f_SV = -1.0*ElemForceD.FlgStrs4Posi.dfb_mv_mid;
                AsfgC.dTau_DL   = FlgBot.DL.dtau;
                AsfgC.dTau_ST   = FlgBot.Std.dtau;
                AsfgC.dTau_SV   = FlgBot.SV.dtau;
                AsfgC.dRA       = AsfgC.ComD.dFc * FlgBot.dSigma_lim2;
                AsfgC.dSA       = FlgBot.LC.dDemand;
                AsfgC.dSd       = FlgBot.DL.dDemand;
                AsfgC.dSst      = FlgBot.Std.dDemand;
                AsfgC.dS        = FlgBot.SV.dDemand;
                AsfgC.dSd_Sst   = FlgBot.DL_Std.dDemand;
            }
            else
            {
                AsfgC.dSig_yf   = FlexD.dSigma_ca;
                AsfgC.dSig_f_DL = -1.0*(ElemForceD.dfb_lt + ElemForceD.dfb_gr);
                AsfgC.dSig_f_ST = -1.0*ElemForceD.dfb_sv;
                AsfgC.dSig_f_SV = -1.0*ElemForceD.dfb_mv;
                AsfgC.dTau_DL   = FlgBot.DL.dtau;
                AsfgC.dTau_ST   = FlgBot.Std.dtau;
                AsfgC.dTau_SV   = FlgBot.SV.dtau;
                AsfgC.dRA       = AsfgC.ComD.dFc * FlexD.dSigma_ca;
                AsfgC.dSA       = -1.0*ElemForceD.dfb;
                AsfgC.dSd       = AsfgC.dSig_f_DL;
                AsfgC.dSst      = AsfgC.dSig_f_ST;
                AsfgC.dS        = AsfgC.dSig_f_SV;
            }
            //
            AsfgT.dSig_yf   = FlexD.dSigma_ta;
            AsfgT.dSig_f_DL = -1.0*(ElemForceD.dft_lt + ElemForceD.dft_gr);
            AsfgT.dSig_f_ST = -1.0*ElemForceD.dft_sv;
            AsfgT.dSig_f_SV = -1.0*ElemForceD.dft_mv;
            AsfgT.dTau_DL   = FlgTop.DL.dtau;
            AsfgT.dTau_ST   = FlgTop.Std.dtau;
            AsfgT.dTau_SV   = FlgTop.SV.dtau;
            AsfgT.dRA       = AsfgT.ComD.dFc * FlexD.dSigma_ta;
            AsfgT.dSA       = -1.0*ElemForceD.dft;
            AsfgT.dSd       = AsfgT.dSig_f_DL;
            AsfgT.dSst      = AsfgT.dSig_f_ST;
            AsfgT.dS        = AsfgT.dSig_f_SV;
        }
    }
    else
    {
        if ( ElemForceD.dft<0.0 )
        {
            AsfgC.dSig_yf   = FlexD.dSigma_ca;
            AsfgC.dSig_f_DL = -1.0*(ElemForceD.dft_lt + ElemForceD.dft_gr);
            AsfgC.dSig_f_ST = -1.0*ElemForceD.dft_sv;
            AsfgC.dSig_f_SV = -1.0*ElemForceD.dft_mv;
            AsfgC.dTau_DL   = FlgTop.LT.dtau + FlgTop.GR.dtau;
            AsfgC.dTau_ST   = FlgTop.Std.dtau;
            AsfgC.dTau_SV   = FlgTop.SV.dtau;
            //AsfgC.dR        = FlgTop.dSigma_lim2;
            AsfgC.dRA       = AsfgC.ComD.dFc * FlexD.dSigma_ca;
            AsfgC.dSA       = -1.0*ElemForceD.dft;
            AsfgC.dSd       = AsfgC.dSig_f_DL;
            AsfgC.dSst      = -1.0*ElemForceD.dft_sv;
            AsfgC.dS        = -1.0*ElemForceD.dft_mv;
            //
            AsfgT.dSig_yf   = FlexD.dSigma_ta;
            AsfgT.dSig_f_DL = -1.0*(ElemForceD.dfb_lt + ElemForceD.dfb_gr);
            AsfgT.dSig_f_ST = -1.0*ElemForceD.dfb_sv;
            AsfgT.dSig_f_SV = -1.0*ElemForceD.dfb_mv;
            AsfgT.dTau_DL   = FlgBot.LT.dtau + FlgBot.GR.dtau;
            AsfgT.dTau_ST   = FlgBot.Std.dtau;
            AsfgT.dTau_SV   = FlgBot.SV.dtau;
            //AsfgT.dR        = FlgBot.dSigma_lim2;
            AsfgT.dRA       = AsfgT.ComD.dFc * FlexD.dSigma_ta;
            AsfgT.dSA       = -1.0*ElemForceD.dfb;
            AsfgT.dSd       = AsfgT.dSig_f_DL;
            AsfgT.dSst      = -1.0*ElemForceD.dfb_sv;
            AsfgT.dS        = -1.0*ElemForceD.dfb_mv;
        }
        else
        {
            AsfgC.dSig_yf   = FlexD.dSigma_ca;
            AsfgC.dSig_f_DL = -1.0*(ElemForceD.dfb_lt + ElemForceD.dfb_gr);
            AsfgC.dSig_f_ST = -1.0*ElemForceD.dfb_sv;
            AsfgC.dSig_f_SV = -1.0*ElemForceD.dfb_mv;
            AsfgC.dTau_DL   = FlgBot.LT.dtau + FlgBot.GR.dtau;
            AsfgC.dTau_ST   = FlgBot.Std.dtau;
            AsfgC.dTau_SV   = FlgBot.SV.dtau;
            AsfgC.dRA       = AsfgC.ComD.dFc * FlexD.dSigma_ca;
            AsfgC.dSA       = -1.0*ElemForceD.dfb;
            AsfgC.dSd       = AsfgC.dSig_f_DL;
            AsfgC.dSst      = AsfgC.dSig_f_ST;
            AsfgC.dS        = AsfgC.dSig_f_SV;
            //
            AsfgT.dSig_yf   = FlexD.dSigma_ta;
            AsfgT.dSig_f_DL = -1.0*(ElemForceD.dft_lt + ElemForceD.dft_gr);
            AsfgT.dSig_f_ST = -1.0*ElemForceD.dft_sv;
            AsfgT.dSig_f_SV = -1.0*ElemForceD.dft_mv;
            AsfgT.dTau_DL   = FlgTop.LT.dtau + FlgTop.GR.dtau;
            AsfgT.dTau_ST   = FlgTop.Std.dtau;
            AsfgT.dTau_SV   = FlgTop.SV.dtau;
            AsfgT.dRA       = AsfgT.ComD.dFc * FlexD.dSigma_ta;
            AsfgT.dSA       = -1.0*ElemForceD.dft;
            AsfgT.dSd       = AsfgT.dSig_f_DL;
            AsfgT.dSst      = AsfgT.dSig_f_ST;
            AsfgT.dS        = AsfgT.dSig_f_SV;
        }

    }

    CalcReserveFactorCombined(nMovType, AsfgC);
    if ( FlexD.bIsCompFlangeWithLS )
    {
        AsfgC.dPsi = bIsTopComp ? FlgTop.dPsi : FlgBot.dPsi;
        AsfgC.dPsi_ast = bIsTopComp ? FlgTop.dPsi_ast : FlgBot.dPsi_ast;
    }
    CalcReserveFactor(nMovType, AsfgT);
    const CSG_FLEX_FLG_LS_RES_CS457& LStiffRes = bPositiveM ? FlgTop.LStfn : FlgBot.LStfn;

    T_ASFS_CSG_BS_BASE& AsfsB = *UlsB.pAsfsB;
    AsfsB.dk_l = LStiffRes.dk_l1;
    AsfsB.dk_s = LStiffRes.dk_s1;
    AsfsB.dSig_y    = LStiffRes.dSigma_ys;
    AsfsB.dSig_a_SV = LStiffRes.dSigma_sv;
    AsfsB.dTau_1_SV = LStiffRes.dtau_sv;
    AsfsB.dSig_a_DL = LStiffRes.dSigma_lt + LStiffRes.dSigma_gr;
    AsfsB.dTau_1_DL = LStiffRes.dtau_lt + LStiffRes.dtau_gr;
    AsfsB.dSig_a_ST = LStiffRes.dSigma_std;
    AsfsB.dTau_1_ST = LStiffRes.dtau_std;
    AsfsB.dRA       = AsfsB.ComD.dFc * LStiffRes.dAlwStress1; // dAlwStressCr;
    AsfsB.dSA       = LStiffRes.dDemand1; // dDemandCr;
    AsfsB.dSd       = LStiffRes.dDemand_lt;
    AsfsB.dSst      = LStiffRes.dDemand_std;
    AsfsB.dS        = LStiffRes.dDemand_sv;

    CalcReserveFactor(nMovType, AsfsB);

    T_ASFS_CSG_BS_BASE& AsfsFlgB = *UlsB.pAsfsFlgB;
    AsfsFlgB.dk_l = LStiffRes.dk_l2;
    AsfsFlgB.dk_s = LStiffRes.dk_s2;
    AsfsFlgB.dSig_y    = LStiffRes.dSigma_ye;
    AsfsFlgB.dSig_a_SV = LStiffRes.dSigma_sv;
    AsfsFlgB.dTau_1_SV = LStiffRes.dtau_sv;
    AsfsFlgB.dSig_a_DL = LStiffRes.dSigma_lt + LStiffRes.dSigma_gr;
    AsfsFlgB.dTau_1_DL = LStiffRes.dtau_lt + LStiffRes.dtau_gr;
    AsfsFlgB.dSig_a_ST = LStiffRes.dSigma_std;
    AsfsFlgB.dTau_1_ST = LStiffRes.dtau_std;
    AsfsFlgB.dRA       = AsfsB.ComD.dFc * LStiffRes.dAlwStress2; //dAlwStressCr;
    AsfsFlgB.dSA       = LStiffRes.dDemand2; // dDemandCr;
    AsfsFlgB.dSd       = LStiffRes.dDemand_lt;
    AsfsFlgB.dSst      = LStiffRes.dDemand_std;
    AsfsFlgB.dS        = LStiffRes.dDemand_sv;

    CalcReserveFactor(nMovType, AsfsFlgB);

    AsfgC.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);
    AsfgT.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);
    AsfsB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);
    AsfsFlgB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckWebWithLStiffenerBoxShapeRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if ( UlsB.pAsfgB==NULL ) return FALSE;
    if ( ResD.pStrnD==NULL ) return FALSE;
    if ( !IsCheckPosition(nPos) ) return FALSE;

    int nStype = MembLcomD.Memb.PosD[nPos].Sect.SectInfo.nStype;
    //if ( nStype != DGN_SECT_TYPE_COMPO_B || nStype != DGN_SECT_TYPE_COMPO_TUB ) return FALSE;

    // Web 보강재가 있고 , Flange에 보강재가 있거나 없거나. 
    const CSG_MEMB_RES_CS457_FLEX_D& MembFlexD = MembResD.MembR[nPos].FlexD[0];

    const CSG_LCOM_RES_D& ElemForceD = ResD.pStrnD->ForceD;
    const CSG_FLEX_RES_CS457& FlexD = ResD.pStrnD->FlexD;
    const CSG_FLANGE_LS_RES_CS457& FlgD = ResD.pStrnD->FlangeD;

    const CSG_FLEX_WEB_RES_CS457& WebD = FlexD.WebCrD;
    CSG_FLEX_WEB_PANEL_RES_CS457 PanelD;
    if ( WebD.bLeftWebCr )
    {
        if ( WebD.aWebPanel.GetSize() > WebD.nPanelCr )
        {
            PanelD = WebD.aWebPanel[WebD.nPanelCr];
        }
    }
    else
    {
        if ( WebD.aWebPanelR.GetSize() > WebD.nPanelCrR )
        {
            PanelD = WebD.aWebPanelR[WebD.nPanelCrR];
        }
    }

    T_ASWY_CSG_BS_BASE& AswyB = *UlsB.pAswyB;
    AswyB.dSig_yw    = MembFlexD.dSigma_yw;
    AswyB.dSig_1e_SV = PanelD.dSigma1e_sp;
    AswyB.dTau_SV    = PanelD.dTau_sp;
    AswyB.dSig_1e_DL = PanelD.dSigma1e_lt + PanelD.dSigma1e_gr;
    AswyB.dTau_DL    = PanelD.dTau_lt + PanelD.dTau_gr;
    AswyB.dSig_1e_ST = PanelD.dSigma1e_std;
    AswyB.dTau_ST    = PanelD.dTau_std;
    AswyB.dRA        = AswyB.ComD.dFc * PanelD.dSigma_alw2;
    AswyB.dSA        = PanelD.dDemand;
    AswyB.dSd        = PanelD.dDemand_DL;
    AswyB.dSst       = PanelD.dDemand_st;
    AswyB.dS         = PanelD.dDemand_sp;
    AswyB.dSd_Sst    = PanelD.dDemand_DL_Std;
    //CalcReserveFactor(nMovType, AswyB);
    CalcReserveFactorCombined(nMovType, AswyB);
    AswyB.dPsi = PanelD.dPsi;
    AswyB.dPsi_ast = PanelD.dPsi_ast;


    T_ASWB_CSG_BS_BASE& AswbB = *UlsB.pAswbB;
    AswbB.dSig_yw = MembFlexD.dSigma_yw;
    AswbB.dK1 = PanelD.dK1;
    AswbB.dKb = PanelD.dKb;
    AswbB.dKq = PanelD.dKq;
    AswbB.dSig_1_SV = PanelD.dSigma1_S_max;
    AswbB.dSig_b_SV = PanelD.dSigma_b_S_ave;
    AswbB.dTau_SV   = PanelD.dTau_S_ave;
    AswbB.dSig_1_DL = PanelD.dSigma1_DL_max;
    AswbB.dSig_b_DL = PanelD.dSigma_b_DL_ave;
    AswbB.dTau_DL   = PanelD.dTau_DL_ave;
    AswbB.dSig_1_ST = PanelD.dSigma1_Std_max;
    AswbB.dSig_b_ST = PanelD.dSigma_b_Std_ave;
    AswbB.dTau_ST   = PanelD.dTau_Std_ave;
    AswbB.dRA       = AswbB.ComD.dFc * PanelD.dInterLim;
    AswbB.dSA       = PanelD.dInterRatio;
    AswbB.dSd       = PanelD.dInterRatio_lt;
    AswbB.dSst      = PanelD.dInterRatio_std;
    AswbB.dS        = PanelD.dInterRatio_sp;
    CalcReserveFactor(nMovType, AswbB);
    if ( AswbB.dSA < 0.0 )
    {
        // 유태용 프로님 요청 인장이면 Psi 계산하지 않음
        AswbB.dPsi = cMaxRF;
        AswbB.dPsi_ast = cMaxRF;
    }
    else
    {
        AswbB.dPsi = PanelD.dPsi;
        AswbB.dPsi_ast = PanelD.dPsi_ast;
    }

    CSG_FLEX_WEB_L_STIFFENER_RES_CS457 StiffenerD;
    if ( WebD.bLeftWebCr )
    {
        if ( WebD.aStiff.GetSize() > WebD.nStiffCr )
        {
            StiffenerD = WebD.aStiff[WebD.nStiffCr];
        }
    }
    else
    {
        if ( WebD.aStiffR.GetSize() > WebD.nStiffCrR )
        {
            StiffenerD = WebD.aStiffR[WebD.nStiffCrR];
        }
    }

    T_ASWS_CSG_BS_BASE& AswsB = *UlsB.pAswsB;
    AswsB.dSig_ls    = StiffenerD.dSigma_ls;
    AswsB.dSig_se_SV = StiffenerD.dSigma_se_sv;
    AswsB.dSig_se_DL = StiffenerD.dSigma_se_lt + StiffenerD.dSigma_se_gr;
    AswsB.dSig_se_ST = StiffenerD.dSigma_se_std;
    AswsB.dRA        = AswbB.ComD.dFc * StiffenerD.dSigma_lim;
    AswsB.dSA        = StiffenerD.dSigma_se;
    AswsB.dSd        = StiffenerD.dSigma_se_DL;
    AswsB.dSst       = StiffenerD.dSigma_se_st;
    AswsB.dS         = StiffenerD.dSigma_se_sv;
    AswsB.dSd_Sst    = StiffenerD.dSigma_se_DL_Std;
    CalcReserveFactorCombined(nMovType, AswsB);

    AswyB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);
    AswbB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);
    AswsB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckSlabWithLStiffenerBoxShapeRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if ( UlsB.pAssbB==NULL ) return FALSE;
    if ( !IsCheckPosition(nPos) ) return FALSE;

    T_ASSB_CSG_BS_BASE& AssbB = *UlsB.pAssbB;

    const CSG_FLEX_RES_CS457& FlexD = ResD.pStrnD->FlexD;
    const CSG_LCOM_RES_D& ElemForceD = ResD.pStrnD->FlexD.ForStrD;
    const CSG_SLAB_LS_RES_CS457& Slab = ResD.pStrnD->SlabD;
    AssbB.bIsSlabCr = Slab.bIsSlabCr ? TRUE : FALSE;
    if ( Slab.bIsSlabCr )
    {
        AssbB.RFData.dRA  = AssbB.RFData.ComD.dFc * Slab.dft_s_lim;
        AssbB.RFData.dSA  = Slab.SlabStress.dSigma;
        AssbB.RFData.dSd  = Slab.SlabStress.dSigma_lt;
        AssbB.RFData.dSst = Slab.SlabStress.dSigma_sv;
        AssbB.RFData.dS   = Slab.SlabStress.dSigma_mv;
    }
    else
    {
        AssbB.RFData.dRA  = AssbB.RFData.ComD.dFc * Slab.dfr_lim;
        AssbB.RFData.dSA  = Slab.RebarStress.dSigma;
        AssbB.RFData.dSd  = Slab.RebarStress.dSigma_lt;
        AssbB.RFData.dSst = Slab.RebarStress.dSigma_sv;
        AssbB.RFData.dS   = Slab.RebarStress.dSigma_mv;
    }
    CalcReserveFactor(nMovType, AssbB.RFData);    

    AssbB.RFData.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);

    return TRUE;
}



BOOL CDgnStlRatingDataCtrl::CheckStrengthRcaseLSD15(ElemPairK ElemK,
                                                    T_RPAS_D &RpasD,       
                                                    RATING_RROS_ELEM &Rros,
                                                    int nPos, BOOL bCurved,
                                                    CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                                    UMD_STL_KR_D &InD,
                                                    CSG_MEMB_RESULT_D &MembRes,
                                                    T_RLCS_D &RlcsD,
                                                    FORCE_RCASE_KR  &Force,
                                                    FORCE_RCASE_KR  &Force_Str,
                                                    T_RFKR_BASE &RFB)
{
    CSG_MEMBPOS_RES_KSCE_LSD12_D TMembRes;
    if (MembRes.pLSD != NULL) TMembRes = *MembRes.pLSD;

    CSG_MEMBPOS_RES_AASHTO_LRFD_D TMembRKSSC;
    if (MembRes.pAASHTO != NULL) TMembRKSSC = *MembRes.pAASHTO;

    CSG_MEMB_RESULT_D TMembR;
    TMembR.pLSD = &TMembRes;
    TMembR.pAASHTO = &TMembRKSSC;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_CHECK_RES_KSCE_LSD12 TRes;
    CSG_MEMB_RES_AASHTO_LRFD_D TResKSSC;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pLSD = &TRes;
    TChkResD.pAASHTO = &TResKSSC;

    MembLcomD.aChkFor[0].nLcomType = 0;

    m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);  

    CSG_STRN_RES_KSCE_LSD12 &StrnRes = TRes.StrnD;
    //
    CSG_MEMB_RES_AASHTO_LRFD_STR_D &StrnResKSSC = TResKSSC.StrengthD;


    double dMr    = StrnRes.FlexD.dMr;
    double dMu_De = Force.ForceDL.dMuy[nPos];
    double dMu_MV = Force.ForceLL.dMuy[nPos];

    double dRF = 0.0;

    double dpMn_CA = 0.0;
    double dpFn_CA = 0.0;
    double dpResis = 0.0;
    double dDemand = 0.0;
    double dMVval  = 0.0;
    double dfmv    = 0.0;
    double dfg     = 0.0;

    BOOL bPositive = (dMu_MV>=0.0);

    int nRFca=0; // 0=Stress, 1=Force
    double d1iFactor = Calc_1PlusImpF(RlcsD.dImpFactor);
    if (bCurved==FALSE)
    {
        nRFca = 1;
        dpResis = dMr;
        dDemand = dMu_De;
        dMVval  = dMu_MV;
        if (fabs(dMu_MV)<m_dZero)
        {
            dRF = m_dRatMax;
        }
        else
        {
            if (dMu_MV>0.0)
            {
                dRF = (dMr - dMu_De) / (dMu_MV*d1iFactor);
                dfmv = StrnRes.FlexD.ForceInfo_F.dft_mv; // 우선 압축??
                dfg  = StrnRes.FlexD.ForceInfo_F.dft_lt;
            }
            else
            {
                dRF = (-dMr - dMu_De) / (dMu_MV*d1iFactor);
                dfmv = StrnRes.FlexD.ForceInfo_F.dfb_mv;
                dfg  = StrnRes.FlexD.ForceInfo_F.dfb_lt;
            }
        }
    }
    else
    {
        int nPosNeg = GetPosNegIndex(bPositive, StrnResKSSC);

        CSG_MEMB_RES_AASHTO_LRFD_STR_FLEX_D& FlexD = StrnResKSSC.FlexD[nPosNeg];

        BOOL bPosiM = FlexD.ForceD.dMu>=0.0;
        switch (FlexD.nCase)
        {
        case 1:
            nRFca = 1;
            dpMn_CA = FlexD.PFCSD.dpMn;
            dpResis = dpMn_CA;
            dDemand = dMu_De;
            dMVval  = dMu_MV;
            break;
        case 2:  // tension stress로 비교
            dpFn_CA = FlexD.PFNSD.dpFnt;
            dpResis = dpFn_CA;
            dDemand = FlexD.PFNSD.dfbutSum;
            dMVval  = FlexD.PFNSD.dfbutMV;
            dfmv    = dMVval;
            dfg     = bPosiM ? FlexD.ForceD.dfb_lt : FlexD.ForceD.dft_lt;
            //RcstB.dfbu_SUM = dDemand;
            //RcstB.dfbu_PR  = dMVval;
            break;
        case 3:  //  compression stress로 비교
            dpFn_CA = FlexD.NFD.dpFnc;
            dpResis = dpFn_CA;
            dDemand = -1.0 * FlexD.NFD.dfbucSum;
            dMVval  = -1.0 * FlexD.NFD.dfbucMV;
            dfmv    = dMVval;
            dfg     = bPosiM ? FlexD.ForceD.dft_lt : FlexD.ForceD.dfb_lt;
            //RcstB.dfbu_SUM = dDemand;
            //RcstB.dfbu_PR  = dMVval;
            break;
        case 4:
            nRFca = 1;
            dpMn_CA = FlexD.NFA6.dpMnt;
            dpResis = dpMn_CA;
            dDemand = dMu_De;
            dMVval  = dMu_MV;
            break;
        default:
            break;
        }
        //
        dfg *= d1iFactor;

        //RcstB.dMn_CA  = (dPhi==0.0) ? 0.0 : dpMn_CA/dPhi;
        //RcstB.dpMn_CA = dpMn_CA;
        //RcstB.dFn_CA  = (dPhi==0.0) ? 0.0 : dpFn_CA/dPhi;
        //RcstB.dpFn_CA = dpFn_CA;  

        double dCaDeM  = 0.0;
        double dCaDeF  = 0.0;
        int    nRFcase = 0;
        if(FlexD.nCase == 1) // positive flexure <moment> term
        {
            dCaDeM = fabs(dpMn_CA) - fabs(dDemand);

            if(dCaDeM < 0.0 && dMVval > 0.0 )  // 강도로 인한 NG, 부호가 같은 경우
            {
                nRFcase = 0;
                dRF     = 0.0;
            }
            else if(dCaDeM < 0.0 && dMVval <0.0)  // 강도로 인한 NG, 부호가 다른 경우
            {
                nRFcase = 1;
                dRF     = 0.0;
            }
            else if(dMVval >= 0.0)  // 부호가 같은 경우
            {
                nRFcase = 2;
                dRF = dMVval == 0.0 ? m_dRatMax : dCaDeM / (dMVval*d1iFactor);      
            }
            else if(dMVval <= 0.0) // 부호가 다른 경우
            {
                nRFcase = 3;
                dRF = dMVval == 0.0 ? m_dRatMax : dCaDeM / (dMVval*d1iFactor);
            }

        }
        else if(FlexD.nCase == 4) // negative flexure <moment> term
        {
            dCaDeM = fabs(dpMn_CA) - fabs(dDemand);

            if(dCaDeM < 0.0 && dMVval < 0.0) // 강도로 인한 NG,  부호가 같은 경우
            {
                nRFcase = 0;
                dRF     = 0.0;
            }
            else if(dCaDeM < 0.0 && dMVval > 0.0) // 강도로 인한 NG, 부호가 다른 경우
            {
                nRFcase = 1;
                dRF     = 0.0;
            }
            else if(dMVval <= 0.0)  // 부호가 같은 경우
            {
                nRFcase = 2;
                dRF = dMVval == 0.0 ? m_dRatMax : -1.0 * dCaDeM  / (dMVval*d1iFactor);
            }
            else if(dMVval >= 0.0)  // 부호가 다른 경우
            {
                nRFcase = 3;
                dRF = dMVval == 0.0 ? m_dRatMax : dCaDeM  / (dMVval*d1iFactor);
            }
        }
        else if(FlexD.nCase == 2)  // check with tension <stress> term (+)
        {
            dCaDeF = fabs(dpFn_CA) - fabs(dDemand);

            if(dCaDeF < 0.0 && dMVval > 0.0)  // 강도로 인한 NG,  부호가 같은 경우
            {
                nRFcase = 0;
                dRF     = 0.0;
            }
            else if(dCaDeF < 0.0 &&  dMVval < 0.0) // 강도로 인한 NG, 부호가 다른 경우
            {
                nRFcase = 1;
                dRF     = 0.0;
            }
            else if(dMVval >= 0.0)  // 부호가 다른 경우
            {
                nRFcase = 2;
                dRF = dMVval == 0.0 ? m_dRatMax : dCaDeF / (dMVval*d1iFactor);      
            }
            else if(dMVval <= 0.0) // 부호가 다른 경우
            {
                nRFcase = 3;
                dRF = dMVval == 0.0 ? m_dRatMax : dCaDeF / (dMVval*d1iFactor);
            }
        }
        else if(FlexD.nCase == 3)  // check with compression <stress> term (-)
        {
            dCaDeF = fabs(dpFn_CA) - fabs(dDemand);

            if(dCaDeF < 0.0 && dMVval < 0.0)  // 강도로 인한 NG,  부호가 같은 경우
            {
                nRFcase = 0;
                dRF     = 0.0;
            }
            else if(dCaDeF < 0.0 && dMVval > 0.0)// 강도로 인한 NG, 부호가 다른 경우
            {
                nRFcase = 1;
                dRF     = 0.0;
            }
            else if(dMVval <= 0.0)  // 부호가 같은 경우
            {
                nRFcase = 2;
                dRF = dMVval == 0.0 ? m_dRatMax : -1.0 * dCaDeF  / (dMVval*d1iFactor);
            }
            else if(dMVval >= 0.0)  // 부호가 다른 경우
            {
                nRFcase = 3;
                dRF = dMVval == 0.0 ? m_dRatMax : dCaDeF  / (dMVval*d1iFactor);
            }
        }
        else ASSERT(0);

    }

    double dEs = MembLcomD.Memb.Matl.MatSw.Steel.Elast;

    //   double dIdispZ = 0.0, dJdispZ = 0.0;
    //   if (!m_pDataCtrl->Get_Deflection4NodeOfElem(ElemK, RFB.nDgnRCaseK, dIdispZ, dJdispZ, ENUM_KSCE_ASD)) { ASSERT(0); };
    RFB.strLiveLoad = RpasD.strDgnLiveLoad;
    RFB.bCheck      = TRUE; // bCheckIJ
    RFB.bCheck2     = TRUE; // 전체 요소에 대해 RF계산함.
    RFB.ElemK       = ElemK.first;    
    RFB.nConcurrent = Force.ForceLL.iRefNo[0];
    RFB.nRFca       = nRFca;
    if (nRFca==0)
    {
        RFB.dfd       = dDemand;
        RFB.dfl       = dMVval;
        RFB.dfa       = dpResis;
    }
    else
    {
        RFB.dMd       = dDemand;
        RFB.dMl       = dMVval;
        RFB.dMc       = dpResis;
    }

    //   RFB.dCalcDisp   = (nPos==0) ? fabs(dIdispZ) : fabs(dJdispZ);
    //   RFB.dEstiDisp   = (nPos==0) ? Rros.dDefI : Rros.dDefJ;
    //   RFB.dRatioDisp  = (fabs(RFB.dEstiDisp)<m_dZero)? 0.0 : fabs(RFB.dCalcDisp)/RFB.dEstiDisp;

    RFB.dCalcEwip   = (fabs(dEs)<m_dZero) ? 0.0 : dfmv / dEs;
    RFB.dEstiEwip   = (nPos==0) ? Rros.dStrainI * 0.001 : Rros.dStrainJ * 0.001;
    RFB.dRatioEwip  = (fabs(RFB.dEstiEwip)<m_dZero) ? 0.0 : RFB.dCalcEwip/RFB.dEstiEwip;

    RFB.dCalcImpFactor = RlcsD.dImpFactor;
    RFB.d1iFactor      = d1iFactor;
    RFB.dRF            = dRF;

    RFB.dfdl           = dfg;
    RFB.dMdl           = Force_Str.ForceG.dMuy[nPos]*d1iFactor;
    if (nRFca==0)
    {
        RFB.dRatiodfa      = fabs(RFB.dfdl)<m_dZero ? 0.0 : RFB.dfa / fabs(RFB.dfdl);
    }
    else
    {
        RFB.dRatiodfa      = fabs(RFB.dMdl)<m_dZero ? 0.0 : RFB.dMc / fabs(RFB.dMdl);
    }  
    RFB.nRankType      = m_pDataCtrl->GetBridgeLevel(RFB.dRatiodfa, RFB.nOK);

    m_pCSGDataCtrl->ConvertCSGEngToUlmForce(MembLcomD.aUlmf[0], RFB.Ulmf);
    m_pCSGDataCtrl->ConvertCSGDCOMF(MembLcomD.aChkFor[0], RFB.ChkFor);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::CheckNonCptStrengthRcaseLSD15(ElemPairK ElemK,
                                                          T_RPAS_D &RpasD,    
                                                          RATING_RROS_ELEM &Rros,
                                                          int nPos, BOOL bCurved,
                                                          CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                                          UMD_STL_KR_D &InD,
                                                          STL_KR_UMDC_STLOPTION &InStlOpt,
                                                          CSG_MEMB_RESULT_D &MembRes,
                                                          S_KR_LSD_RES_D &resD,
                                                          T_RLCS_D &RlcsD,
                                                          FORCE_RCASE_KR  &Force,
                                                          FORCE_RCASE_KR  &Force_Str,
                                                          T_RFKR_BASE &RFB)
{

    if (!Check_Rating(InD, InStlOpt, resD)) return FALSE;
    if (resD.aStdrBaseD.GetSize()==0) return FALSE;

    S_KR_LSD_STDR_BASE &StrnB = resD.aStdrBaseD[0];
    double dMr    = StrnB.dMr[0];
    double dMu_De = Force.ForceDL.dMuy[nPos];
    double dMu_MV = Force.ForceLL.dMuy[nPos];

    double dRF = 0.0;

    double dfmv    = 0.0;
    double dfg     = 0.0;

    BOOL bPositive = (dMu_MV>=0.0);

    int nRFca=1; // 0=Stress, 1=Force
    double d1iFactor = Calc_1PlusImpF(RlcsD.dImpFactor);

    double dpResis = dMr;
    double dDemand = dMu_De;
    double dMVval  = dMu_MV;
    if (fabs(dMu_MV)<m_dZero)
    {
        dRF = m_dRatMax;
    }
    else
    {
        if (dMu_MV>0.0)
        {
            dRF = (dMr - dMu_De) / (dMu_MV*d1iFactor);
        }
        else
        {
            dRF = (-dMr - dMu_De) / (dMu_MV*d1iFactor);
        }
    }

    double dEs = MembLcomD.Memb.Matl.MatSw.Steel.Elast;

    RFB.strLiveLoad = RpasD.strDgnLiveLoad;
    RFB.bCheck      = TRUE; // bCheckIJ
    RFB.bCheck2     = TRUE; // 전체 요소에 대해 RF계산함.
    RFB.ElemK       = ElemK.first;    
    RFB.nConcurrent = Force.ForceLL.iRefNo[0];
    RFB.nRFca       = nRFca;
    RFB.dMd         = dDemand;
    RFB.dMl         = dMVval;
    RFB.dMc         = dpResis;

    RFB.dCalcEwip   = (fabs(dEs)<m_dZero) ? 0.0 : dfmv / dEs;
    RFB.dEstiEwip   = (nPos==0) ? Rros.dStrainI * 0.001 : Rros.dStrainJ * 0.001;
    RFB.dRatioEwip  = (fabs(RFB.dEstiEwip)<m_dZero) ? 0.0 : RFB.dCalcEwip/RFB.dEstiEwip;

    RFB.dCalcImpFactor = RlcsD.dImpFactor;
    RFB.d1iFactor      = d1iFactor;
    RFB.dRF            = dRF;

    RFB.dfdl           = dfg;
    RFB.dMdl           = Force_Str.ForceG.dMuy[nPos]*d1iFactor;
    RFB.dRatiodfa      = fabs(RFB.dMdl)<m_dZero ? 0.0 : RFB.dMc / fabs(RFB.dMdl);
    RFB.nRankType      = m_pDataCtrl->GetBridgeLevel(RFB.dRatiodfa, RFB.nOK);


    return TRUE;
}


BOOL CDgnStlRatingDataCtrl::Calc_RatingData(ElemPairK ElemK,
                                            T_RPAS_D &RpasD,       
                                            RATING_RROS_ELEM &Rros,
                                            int nPos,                                            
                                            T_RFKR_BASE &RFB)
{
    if (RFB.bCheck==FALSE) return FALSE;

    double dIdispZ = 0.0, dJdispZ = 0.0;
    if (!m_pDataCtrl->Get_Deflection4NodeOfElem(ElemK, RFB.nDgnRCaseK, dIdispZ, dJdispZ, ENUM_KSCE_ASD)) { ASSERT(0); };

    RFB.dCalcDisp   = (nPos==0) ? fabs(dIdispZ) : fabs(dJdispZ);
    RFB.dEstiDisp   = (nPos==0) ? Rros.dDefI : Rros.dDefJ;
    RFB.dRatioDisp  = (fabs(RFB.dEstiDisp)<m_dZero)? 0.0 : fabs(RFB.dCalcDisp)/RFB.dEstiDisp;

    double dImpactFactor =  (nPos==0) ? Rros.dImpactFactor : Rros.dImpactFactor_J;
    double dAddReponceF  =  (nPos==0) ? Rros.dAddReponceF_I : Rros.dAddReponceF_J; 
    //RFB.dCalcImpFactor = RlcsD.dImpFactor;
    RFB.dEstiImpFactor = dImpactFactor;
    RFB.dRatioImpFactor= RFB.d1iFactor / (1.0 + dImpactFactor);

    RFB.dKs            = RpasD.nSurveyMeth == 0 ? (fabs(RFB.dCalcDisp) / RFB.dEstiDisp) * ( (1 + RFB.dCalcImpFactor) / (1 + RFB.dEstiImpFactor) ) 
        : (fabs(RFB.dCalcEwip) / RFB.dEstiEwip) * ( (1 + RFB.dCalcImpFactor) / (1 + RFB.dEstiImpFactor) );

    RFB.dKs            *= dAddReponceF;

    RFB.dPr            = RpasD.dDgnLiveLoad;
    RFB.dP0            = RFB.dRF * RpasD.dDgnLiveLoad;
    RFB.dP             = RFB.dKs * RFB.dP0;
    RFB.nOK            = RFB.dPr < RFB.dP ? 1 : 2;
    //   RFB.Load           = InD.Load.LcomList[nIdxTen];
    //   RFB.Load.strName   = (k==0) ? strLcomTT : strLcomBT;
    //   RFB.Load2          = InD.Load.Lcom2List[nIdxTen];
    //   RFB.Load2.strName  = (k==0) ? strLcomTT : strLcomBT;

    return TRUE;
}


BOOL CDgnStlRatingDataCtrl::IsSkipRatingCase(BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD)
{
    int i,j,k;  
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, i, j, k);

    if (RlcsD.RateLoad[1].dFactor[i]==0.0 || 
        RlcsD.RateLoad[2].dFactor[j]==0.0 ||
        RlcsD.RateLoad[3].dFactor[k]==0.0   ) return FALSE;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Calc_Stress4MVpr(ElemPairK ElemK, int nPos,
                                             CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
                                             double &dEpsiCal)
{
    CSG_MEMBPOS_RES_AASHTO_LRFD_D TMembResAASHTO = MembResAASHTO;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pAASHTO = &TMembResAASHTO;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    double dEs = MembLcomD.Memb.Matl.MatStf.Steel.Elast;

    int nChkLcomSize = MembLcomD.aChkFor.GetSize();

    double dEpsiCalMax=0.0;
    for (int i=0; i<nChkLcomSize; ++i)
    {
        CSG_MEMB_RES_AASHTO_LRFD_D TRes;
        CSG_CHECK_RESULT_CODES TChkResD;
        TChkResD.pAASHTO = &TRes;

        MembLcomD.aChkFor[i].nLcomType = 1;
        m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);

        CSG_MEMB_RES_AASHTO_LRFD_SER_D &MembServD = TMembR.pAASHTO->MembR[nPos].ServiceD;

        CSG_LCOM_RES_D &ForD = MembServD.ForceD;

        double dEpsiT = (dEs==0.0) ? 0.0 : ForD.dft_st/dEs;
        double dEpsiB = (dEs==0.0) ? 0.0 : ForD.dfb_st/dEs;

        double dEpsiCalCur = max(fabs(dEpsiT), fabs(dEpsiB));

        if (i==0)
        {
            dEpsiCalMax = dEpsiCalCur;
        }
        else 
        {
            if (dEpsiCalCur>dEpsiCalMax)
            {
                dEpsiCalMax = dEpsiCalCur;
            }
        }
    }    

    dEpsiCal = dEpsiCalMax;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Calc_AdjustmentFactorK(double dEpsiC, double dEpsiT, double dKb, double dKuser, double &dKa, double &dK)
{
    dK = 1.0;
    if (m_bbAppAFK==TRUE)
    {
        if (m_bAutoAFK==TRUE)
        {
            dKa = (dEpsiT==0.0) ? 0.0 : dEpsiC/dEpsiT-1;
            dK  = 1.0 + dKa*dKb;
        }
        else
        {
            dK = dKuser;
        }
    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Get_DeflectionOfNode(T_ELEM_K ElemK, T_RLCS_D &RlcsD, T_LCOM_D &LcomDE, double &dIdisp, double &dJdisp)
{
    T_ELEM_D ElemD;
    m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
    T_NODE_K NodeK1 = ElemD.elnod[0];
    T_NODE_K NodeK2 = ElemD.elnod[1];    

    //   int nI, nJ, nK;
    //   Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, nI, nJ, nK);
    //   
    //   RcfaB.drDC_BE = RlcsD.RateLoad[0].dFactor[nI];
    //   RcfaB.drDC    = RlcsD.RateLoad[1].dFactor[nI];  
    //   RcfaB.drDW    = RlcsD.RateLoad[2].dFactor[nJ];  
    //   RcfaB.drTE    = RlcsD.RateLoad[3].dFactor[nK];
    //   RcfaB.drTG    = RlcsD.RateLoad[4].dFactor[nK];
    //   RcfaB.drSE    = RlcsD.RateLoad[5].dFactor[0]; 
    //   RcfaB.drP     = RlcsD.RateLoad[6].dFactor[0]; 
    //   RcfaB.drUS    = RlcsD.RateLoad[7].dFactor[0];
    //   RcfaB.drPR    = RlcsD.PrimaryVeh.Factor;
    //   RcfaB.drAD    = RlcsD.AdjacentVeh.Factor;

    //  

    double dIdisp_DL = 0.0;
    double dJdisp_DL = 0.0;

    //      
    T_DISP_D Node1MaxDisp; Node1MaxDisp.Initialize();
    T_DISP_D Node1MinDisp; Node1MinDisp.Initialize();
    T_DISP_D Node2MaxDisp; Node2MaxDisp.Initialize();
    T_DISP_D Node2MinDisp; Node2MinDisp.Initialize();

    //   m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomDE);
    //   m_pDoc->m_pPostCtrl->GetDispNew(NodeK1, &Node1MaxDisp, &Node1MinDisp, NULL);
    //   m_pDoc->m_pPostCtrl->GetDispNew(NodeK2, &Node2MaxDisp, &Node2MinDisp, NULL);
    //   //
    //   int nRcaseTypeDemand = 0;
    //   if(nRcaseTypeDemand==0) 
    //   {
    //     dIdisp_DL = Node1MaxDisp.dblDisp[2];
    //     dJdisp_DL = Node2MaxDisp.dblDisp[2];      
    //   }
    //   else // Max/Min Type
    //   {
    //     dIdisp_DL = Node1MinDisp.dblDisp[2]; // Max/Min Type이면 Min만...
    //     dJdisp_DL = Node2MinDisp.dblDisp[2];      
    //   }

    //++++++++++
    // Moving
    T_DISP_D Node1MaxDispMV; Node1MaxDispMV.Initialize();
    T_DISP_D Node1MinDispMV; Node1MinDispMV.Initialize();
    T_DISP_D Node2MaxDispMV; Node2MaxDispMV.Initialize();
    T_DISP_D Node2MinDispMV; Node2MinDispMV.Initialize();

    T_LCOM_D LcomMV;  LcomMV.Initialize();    
    LcomMV.aCombination.Add(RlcsD.PrimaryVeh);

    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomMV);
    m_pDoc->m_pPostCtrl->GetDispNew(NodeK1, &Node1MaxDispMV, &Node1MinDispMV, NULL);
    m_pDoc->m_pPostCtrl->GetDispNew(NodeK2, &Node2MaxDispMV, &Node2MinDispMV, NULL);

    double dIdisp_LL = max(fabs(Node1MaxDispMV.dblDisp[2]), fabs(Node1MinDispMV.dblDisp[2]));
    double dJdisp_LL = max(fabs(Node2MaxDispMV.dblDisp[2]), fabs(Node2MinDispMV.dblDisp[2]));

    // Moving만 적용한다. 
    dIdisp = /*dIdisp_DL + */dIdisp_LL;
    dJdisp = /*dJdisp_DL + */dJdisp_LL;
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::MakeLcomData4RCase(T_RLCS_D &RlcsD, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_LCOM_D &LcomD)
{
    int nI, nJ, nK;
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, nI, nJ, nK);

    LcomD.Initialize();

    //   RcfaB.drDC    = RlcsD.RateLoad[1].dFactor[nI];  
    //   RcfaB.drDW    = RlcsD.RateLoad[2].dFactor[nJ];  
    //   RcfaB.drTE    = RlcsD.RateLoad[3].dFactor[nK];
    //   RcfaB.drTG    = RlcsD.RateLoad[4].dFactor[nK];
    //   RcfaB.drSE    = RlcsD.RateLoad[5].dFactor[0]; 
    //   RcfaB.drP     = RlcsD.RateLoad[6].dFactor[0]; 
    //   RcfaB.drUS    = RlcsD.RateLoad[7].dFactor[0];

    int nLC=0;
    for (int i=1; i<cRAT_LCASE; ++i)
    {
        T_RTLD_D &RtlD = RlcsD.RateLoad[i];
        int nCombSize = RtlD.Combination.GetSize();
        for (int j=0; j<nCombSize; ++j)
        {
            T_LCOM_BASE &LcomB = RtlD.Combination[j];
            switch(i)
            {
            case 1:
                LcomB.Factor = RtlD.dFactor[nI];
                break;
            case 2:
                LcomB.Factor = RtlD.dFactor[nJ];
                break;
            case 3:
            case 4:
                LcomB.Factor = RtlD.dFactor[nK];
                break;
            default:
                LcomB.Factor = RtlD.dFactor[0];
                break;
            }     
            LcomD.aCombination.Add(LcomB);
        }
    }

    LcomD.aCombination.Add(RlcsD.AdjacentVeh);


    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::ConvertForce2Engine(FORCE_LRKR_ELEM &Force, STL_KR_LOAD &rForce, const int &nIJ)
{
    int nSize = Force.arForceRCase.GetSize();
    rForce.LcomList.SetSize(nSize);
    rForce.Lcom2List.SetSize(nSize);
    rForce.aCb.SetSize(nSize);
    for (int i = 0; i<nSize; ++i)
    {
        FORCE_RCASE_KR &ForceD = Force.arForceRCase.GetAt(i);
        _DGN_FORC_CRC &ForceValDL = ForceD.ForceDL;
        _DGN_FORC_CRC &ForceValLL = ForceD.ForceLL;
        rForce.LcomList[i].Initialize();
        rForce.LcomList[i].dForce[0] = ForceValDL.dFxx[nIJ==1] + ForceValLL.dFxx[nIJ==1];
        rForce.LcomList[i].dForce[1] = ForceValDL.dFyy[nIJ==1] + ForceValLL.dFyy[nIJ==1];
        rForce.LcomList[i].dForce[2] = ForceValDL.dFzz[nIJ==1] + ForceValLL.dFzz[nIJ==1];
        rForce.LcomList[i].dForce[3] = ForceValDL.dMux[nIJ==1] + ForceValLL.dMux[nIJ==1];
        rForce.LcomList[i].dForce[4] = ForceValDL.dMuy[nIJ==1] + ForceValLL.dMuy[nIJ==1];
        rForce.LcomList[i].dForce[5] = ForceValDL.dMuz[nIJ==1] + ForceValLL.dMuz[nIJ==1];
        rForce.LcomList[i].bUse      = TRUE;
        rForce.LcomList[i].nOrgLkey  = ForceValDL.iRefNo[nIJ==1];

        rForce.Lcom2List[i].Initialize();
        rForce.Lcom2List[i].dForce[0] = ForceValDL.dFxx[nIJ==0] + ForceValLL.dFxx[nIJ==0];
        rForce.Lcom2List[i].dForce[1] = ForceValDL.dFyy[nIJ==0] + ForceValLL.dFyy[nIJ==0];
        rForce.Lcom2List[i].dForce[2] = ForceValDL.dFzz[nIJ==0] + ForceValLL.dFzz[nIJ==0];
        rForce.Lcom2List[i].dForce[3] = ForceValDL.dMux[nIJ==0] + ForceValLL.dMux[nIJ==0];
        rForce.Lcom2List[i].dForce[4] = ForceValDL.dMuy[nIJ==0] + ForceValLL.dMuy[nIJ==0];
        rForce.Lcom2List[i].dForce[5] = ForceValDL.dMuz[nIJ==0] + ForceValLL.dMuz[nIJ==0];
        rForce.Lcom2List[i].bUse      = TRUE;
        rForce.Lcom2List[i].nOrgLkey  = ForceValDL.iRefNo[nIJ==0];

        rForce.aCb[i] = 1.0; // 임시.
    }

    return TRUE; 
}

BOOL CDgnStlRatingDataCtrl::ConvertCsgForce2Engine(ElemPairK ElemK, FORCE_LRKR_ELEM &Force, CSG_MEMB_POSD_KSCE_LSD &rMembLcomD, const int &nIJ)
{
    int nSize = Force.arForceRCase.GetSize();
    if (nSize<=0) return FALSE;

    int nPos = rMembLcomD.Memb.ChIf.nChkPos = nIJ;

    int nChkLcomSizeUlmf = nSize;
    if (m_nDgnCode==STL_KSCE_LSD15 && rMembLcomD.Memb.ChIf.bCurved==TRUE) nChkLcomSizeUlmf = 6;

    rMembLcomD.aUlmf.SetSize(nChkLcomSizeUlmf);
    rMembLcomD.aChkFor.SetSize(nSize);    

    // Pre-Combined Composite Bridge
    BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb(); 

    ElemPairK nElemI = ElemK, nElemM = ElemK, nElemJ = ElemK;
    int nPosI=0;
    int nPosM=0;
    int nPosJ=1;

    BOOL bCantilever=FALSE;
    DgnBeamPairK nUblmK(EN_EL_BEAM, 0);

    m_pDataCtrl->GetUbLengthElemInfo(ElemK, nUblmK, nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, bCantilever);
    //
    m_pCSGDataCtrl->Get_CsgUlmSection(nElemI, nElemM, nElemJ, nPosI, nPosM, nPosJ, rMembLcomD.Memb.Ulms);

    rMembLcomD.Memb.ChIf.bCantilever = bCantilever;

    // 시공중 거더 Force
    _DGN_FORC_CRC FcsDL;
    m_pForcCtrl->Get_CSGirderForce4CompoBefore(ElemK, FcsDL);

    // 2014.03.31 이정우 GJ님 요청 
    // Girder의 Force는 Dead(CS)만 포함 하기로 하고 하중조합 Dead(CS)의 factor를 적용한다. 
    _DGN_FORC_CRC ForceNC_org;
    _DGN_FORC_CRC ForceNC;
    if (bIsPLCB==FALSE) ForceNC_org += FcsDL;

    int nLcomNum = m_pForcCtrl->Get_LcomDataCount();

    ADGNFORCE aPreForceLcom;
    int nPreForceLcomSize = 0;
    FORCE_RCASE_PRECOMP_ELEM aPreforceElem; aPreforceElem.Initialize();
    if (bIsPLCB==TRUE)
    {
		const auto& itFor = m_pForcCtrl->m_aPreCompForceElem.find(ElemK);
		if (itFor != m_pForcCtrl->m_aPreCompForceElem.end())
		{
			aPreforceElem = itFor->second;
		}
        nPreForceLcomSize = aPreforceElem.aForceRCase.GetSize();
        //     m_pForcCtrl->Get_BeforeForce4PreCompo(ElemK, aPreForceLcom, nLcomNum);
        //     nPreForceLcomSize = aPreForceLcom.GetSize();
    }

    FORCE_LRKR_ELEM  ForceI;
    FORCE_LRKR_ELEM  ForceM;
    FORCE_LRKR_ELEM  ForceJ;

	const auto& itForI = m_pForcCtrl->m_arBlrdForceElemKR.find(nElemI);
    const auto& itForM = m_pForcCtrl->m_arBlrdForceElemKR.find(nElemM);
    const auto& itForJ = m_pForcCtrl->m_arBlrdForceElemKR.find(nElemJ);
    BOOL bIsSpanForI = FALSE;
    BOOL bIsSpanForM = FALSE;
    BOOL bIsSpanForJ = FALSE;
    if (itForI != m_pForcCtrl->m_arBlrdForceElemKR.end())
    { 
        bIsSpanForI = TRUE;
		ForceI = itForI->second;
    }
    if (itForM != m_pForcCtrl->m_arBlrdForceElemKR.end())
    {
        bIsSpanForM = TRUE;
        ForceM = itForM->second;
    }
    if (itForJ != m_pForcCtrl->m_arBlrdForceElemKR.end())
    {
        bIsSpanForI = TRUE;
        ForceJ = itForJ->second;
    }

    for (int i=0; i<nSize; ++i)
    {
        int nDgnLcomNo = i+1;
        if (bIsPLCB==TRUE)
        {
            if (i<nPreForceLcomSize) ForceNC = aPreforceElem.aForceRCase[i].Force;
            //if (i<nPreForceLcomSize) ForceNC = aPreForceLcom[i];
        }
        else
        {
            ForceNC = ForceNC_org;
            _DGN_LCOM DgnLcom;
            if (m_pForcCtrl->Get_DgnLcomData(nDgnLcomNo, DgnLcom))
            {
                ForceNC *= DgnLcom.dFactorDeadCS;
            }
        }

        CSG_FORCE_COMP &ComLcomF = rMembLcomD.aChkFor[i];
        ComLcomF.nLcomType = 0;
        //     if      (bULSLcom==TRUE) ComLcomF.nLcomType = 0;
        //     else if (bSLSLcom==TRUE) 
        //     {
        //       ComLcomF.nLcomType = 1;
        //       ComLcomF.nLcomSub  = nServLcomType;
        //     }      
        //     ComLcomF.nMax = nMaxMinType;


        if (bIsSpanForI || bIsSpanForJ) 
        {        
            _DGN_FORC_CRC &SpanForI = ForceI.arForceRCase[i].ForceDL;
            _DGN_FORC_CRC &SpanForM = ForceM.arForceRCase[i].ForceDL;
            _DGN_FORC_CRC &SpanForJ = ForceJ.arForceRCase[i].ForceDL;
            m_pCSGDataCtrl->ConvertCsgCalcUlmForcePos3(nPosI,nPosM,nPosJ, SpanForI, SpanForM, SpanForJ, rMembLcomD.aUlmf[i]);      
        }

        //     _DGN_FORC_CRC &ForceLT   =  Force.arForceRCase[i].ForceDL; // Rating에서 DL성분들은 ST로 본다. 그리고 전체 stress를 엔진에서 누적한다.  
        // 		               ForceLT  -= ForceNC;
        //     _DGN_FORC_CRC &ForceST   =  Force.arForceRCase[i].ForceLL;
        //     _DGN_FORC_CRC &ForceLcom = ForceNC;
        // 		              ForceLcom += ForceLT;
        // 									ForceLcom += ForceST;

        _DGN_FORC_CRC &ForceLT   =  Force.arForceRCase[i].ForceDL; // Rating에서 DL성분들은 ST로 본다. 그리고 전체 stress를 엔진에서 누적한다.  
        _DGN_FORC_CRC ForceST    =  Force.arForceRCase[i].ForceST;
        _DGN_FORC_CRC ForceMV    =  Force.arForceRCase[i].ForceLL;
        ForceST += ForceMV;
        _DGN_FORC_CRC ForceLcom;
        AddForceData(ForceNC, ForceLcom);
        AddForceData(ForceLT, ForceLcom);
        AddForceData(ForceST, ForceLcom);

        m_pCSGDataCtrl->ConvertCsgCalcChkForce(nPos, ForceNC, ForceLT, ForceST, ForceLcom, rMembLcomD.aChkFor[i]);
        m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForceMV, rMembLcomD.aChkFor[i].ForMV);

    }

    return TRUE;
}

void CDgnStlRatingDataCtrl::AddForceData(const _DGN_FORC_CRC& Force, _DGN_FORC_CRC& rData)
{
    for(int nPosi=0; nPosi<cPOSI_CRC; nPosi++)
    {
        rData.dFxx[nPosi] += Force.dFxx[nPosi];
        rData.dFyy[nPosi] += Force.dFyy[nPosi];
        rData.dFzz[nPosi] += Force.dFzz[nPosi];
        rData.dMux[nPosi] += Force.dMux[nPosi];
        rData.dMuy[nPosi] += Force.dMuy[nPosi];
        rData.dMuz[nPosi] += Force.dMuz[nPosi];
        rData.dMbi[nPosi] += Force.dMbi[nPosi];
        rData.dTst[nPosi] += Force.dTst[nPosi];
        rData.dTwp[nPosi] += Force.dTwp[nPosi];

    }

}


BOOL CDgnStlRatingDataCtrl::ConvertForce2EnginePRT(_DGN_FORC_CRC &Force, STL_KR_UNITLCOM &rData, const int &nIJ)
{
    if (&Force == NULL) return FALSE;  
    rData.strName = Force.strType;
    rData.dForce[0] = Force.dFxx[nIJ];
    rData.dForce[1] = Force.dFyy[nIJ];
    rData.dForce[2] = Force.dFzz[nIJ];
    rData.dForce[3] = Force.dMux[nIJ];
    rData.dForce[4] = Force.dMuy[nIJ];
    rData.dForce[5] = Force.dMuz[nIJ];
    rData.nType     = Force.iRefNo[0];//FX-MAX..를 일단 여기에다 담음.
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::IsCompoElem(T_ELEM_K ElemK)
{
    T_ELEM_K ValElemK=0;
    if (m_mCompoElemK.Lookup(ElemK,ValElemK)) return TRUE;
    return FALSE;
}

int CDgnStlRatingDataCtrl::GetConvertStlRatingCodeCvlToEng(int nDgnCode)
{
    int nStlRatingCode=0;

    switch ( nDgnCode )
    {
    case STL_AASHTO_LRFD12:    nStlRatingCode = DGN_AASHTO_LRFD12_RATING; break;
    case STL_AASHTO_LRFD19:    nStlRatingCode = DGN_AASHTO_LRFD19_RATING; break;
    case STL_CS454_20:         nStlRatingCode = DGN_CS_454_20_RATING; break;
    case STL_NR_GN_CIV_025_06: nStlRatingCode = DGN_NR_GN_CIV_025_06_RATING; break;
    default: ASSERT(0); break;
    }

    return nStlRatingCode;
}

int CDgnStlRatingDataCtrl::GetConvertStlRatingCodeEngToCvl(int nDgnCode)
{
    int nStlRatingCode=0;

    switch(nDgnCode)
    {
    case DGN_AASHTO_LRFD12_RATING:    nStlRatingCode = STL_AASHTO_LRFD12; break;
    case DGN_AASHTO_LRFD19_RATING:    nStlRatingCode = STL_AASHTO_LRFD19; break;
    case DGN_CS_454_20_RATING:        nStlRatingCode = STL_CS454_20; break;
    case DGN_NR_GN_CIV_025_06_RATING: nStlRatingCode = STL_NR_GN_CIV_025_06; break;
    default: ASSERT(0); break;
    }

    return nStlRatingCode;
}

BOOL CDgnStlRatingDataCtrl::Get_RatingStrengthLCaseForce(int iIJ, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD, _DGN_FORC_CRC &ForceNC, FORCE_BLRD_RCASE &ForceRcase, T_RCST_BASE &RcstB)
{
    int i,j,k;  
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, i, j, k);

    RcstB.drDC_BE = RlcsD.RateLoad[0].dFactor[i];
    RcstB.drDC    = RlcsD.RateLoad[1].dFactor[i];  
    RcstB.drDW    = RlcsD.RateLoad[2].dFactor[j];  
    RcstB.drTE    = RlcsD.RateLoad[3].dFactor[k];
    RcstB.drTG    = RlcsD.RateLoad[4].dFactor[k];
    RcstB.drSE    = RlcsD.RateLoad[5].dFactor[0]; 
    RcstB.drP     = RlcsD.RateLoad[6].dFactor[0]; 
    RcstB.drUS    = RlcsD.RateLoad[7].dFactor[0];
    RcstB.drPR    = RlcsD.PrimaryVeh.Factor;
    RcstB.drAD    = RlcsD.AdjacentVeh.Factor;

    // DC(Before)
    _DGN_FORC_CRC ForNC = ForceNC;
    ConvertForceDgnToCsgDB(iIJ, ForNC, RcstB.ComFor.ForNC);

    // DC.Before
    if (bIsPLCB==TRUE)
    {
        //RcstB.ForDCBe.dForce[2] = (RcstB.drDC_BE==0.0) ? 0.0 : ForceRcase.ForceDCBe[i].dFzz[iIJ] / RcstB.drDC_BE;
        //RcstB.ForDCBe.dForce[4] = (RcstB.drDC_BE==0.0) ? 0.0 : ForceRcase.ForceDCBe[i].dMuy[iIJ] / RcstB.drDC_BE;
        RcstB.ForDCBe.dForce[2] = (RcstB.drDC_BE==0.0) ? 0.0 : ForceNC.dFzz[iIJ] / RcstB.drDC_BE;    
        RcstB.ForDCBe.dForce[4] = (RcstB.drDC_BE==0.0) ? 0.0 : ForceNC.dMuy[iIJ] / RcstB.drDC_BE;
    }
    else
    {
        if ( RcstB.drDC != 0.0 )
        {
            ForNC / RcstB.drDC;
        }
        ConvertForceDgnToCsgDB(iIJ, ForNC, RcstB.ForDCBe);
    }

    FORCE_BLRD_RCASE TForRcase = ForceRcase;
    // DC.
    if ( RcstB.drDC != 0.0 )
    {
        TForRcase.ForceDC[i] / RcstB.drDC;
        _DGN_FORC_CRC TFor;
        TForRcase.ForceDC[i].GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForDC);
    }
    
    // DW.
    if ( RcstB.drDW != 0.0 )
    {
        TForRcase.ForceDW[j] / RcstB.drDW;
        _DGN_FORC_CRC TFor;
        TForRcase.ForceDW[j].GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForDW);
    }

    // Temperature.
    if ( RcstB.drTE != 0.0 )
    {
        TForRcase.ForceTemp[k] / RcstB.drTE;
        _DGN_FORC_CRC TFor;
        TForRcase.ForceTemp[k].GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForTE);
    }

    // Temperature gradient.
    if ( RcstB.drTG != 0.0 )
    {
        TForRcase.ForceTemp[k] / RcstB.drTG;
        _DGN_FORC_CRC TFor;
        TForRcase.ForceTempG[k].GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForTG);
    }

    // Secondary.
    if ( RcstB.drSE != 0.0 )
    {
        TForRcase.ForceSec / RcstB.drSE;
        _DGN_FORC_CRC TFor;
        TForRcase.ForceSec.GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForSE);
    }

    // Permanent.
    if ( RcstB.drP != 0.0 )
    {
        TForRcase.ForcePerm / RcstB.drP;
        _DGN_FORC_CRC TFor;
        TForRcase.ForcePerm.GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForP);
    }

    // User.
    if ( RcstB.drUS != 0.0 )
    {
        TForRcase.ForceUser / RcstB.drUS;
        _DGN_FORC_CRC TFor;
        TForRcase.ForceUser.GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForUS);
    }

    // Temperature(-) 일 때만 적용.
    if(k==1) 
    {
        for ( int nC=0; nC < D_CSG_FORCE_COMPONENT; ++nC )
        {
            RcstB.ForTE.dForce[nC] *= -1.0;
        }
    }  

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Get_RatingStrengthData(int iIJ, int nConcurrent, T_RLCS_D& RlcsD, FORCE_BLRD_RCASE& ForceRcase, T_RCST_BASE& RcstB)
{  
    FORCE_BLRD_RCASE TForRcase = ForceRcase;

    int nCCId = nConcurrent-1;
    // Primary Vehicle
    if ( RcstB.drPR != 0.0 )
    {
        TForRcase.ForcePrVehicle[nCCId] / RcstB.drPR;
        _DGN_FORC_CRC TFor;
        TForRcase.ForcePrVehicle[nCCId].GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForPR);
    }

    // Adjacent Vehicle
    if ( RcstB.drAD != 0.0 )
    {
        TForRcase.ForceAdVehicle[nCCId] / RcstB.drAD;
        _DGN_FORC_CRC TFor;
        TForRcase.ForceAdVehicle[nCCId].GetForce(TFor);
        ConvertForceDgnToCsgDB(iIJ, TFor, RcstB.ForAD);
    }

    for ( int nC=0; nC < D_CSG_FORCE_COMPONENT; ++nC )
    {
        RcstB.ForSUM.dForce[nC] = RcstB.ForDCBe.dForce[nC]  * RcstB.drDC_BE +
            RcstB.ForDC.dForce[nC]    * RcstB.drDC    +
            RcstB.ForDW.dForce[nC]    * RcstB.drDW    +
            RcstB.ForTE.dForce[nC]    * RcstB.drTE    +
            RcstB.ForTG.dForce[nC]    * RcstB.drTG    +
            RcstB.ForSE.dForce[nC]    * RcstB.drSE    +
            RcstB.ForP.dForce[nC]     * RcstB.drP     +
            RcstB.ForUS.dForce[nC]    * RcstB.drUS    +
            RcstB.ForAD.dForce[nC]    * RcstB.drAD;
    }

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Get_RatingServiceLCaseForce(int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCSS_BASE &RcssB)
{
    int i,j,k;  
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, i, j, k);

    RcssB.drDC_BE = RlcsD.RateLoad[0].dFactor[i];
    RcssB.drDC    = RlcsD.RateLoad[1].dFactor[i];  
    RcssB.drDW    = RlcsD.RateLoad[2].dFactor[j];  
    RcssB.drTE    = RlcsD.RateLoad[3].dFactor[k];
    RcssB.drTG    = RlcsD.RateLoad[4].dFactor[k];
    RcssB.drSE    = RlcsD.RateLoad[5].dFactor[0]; 
    RcssB.drP     = RlcsD.RateLoad[6].dFactor[0]; 
    RcssB.drUS    = RlcsD.RateLoad[7].dFactor[0];
    RcssB.drPR    = RlcsD.PrimaryVeh.Factor;
    RcssB.drAD    = RlcsD.AdjacentVeh.Factor;

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Get_RatingServiceData(int iIJ, int nConcurrent, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCSS_BASE &RcstB)
{
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::Get_RatingFatigueLCaseForce(int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCFA_BASE &RcfaB)
{
    int i,j,k;  
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, i, j, k);

    RcfaB.drDC_BE = RlcsD.RateLoad[0].dFactor[i];
    RcfaB.drDC    = RlcsD.RateLoad[1].dFactor[i];  
    RcfaB.drDW    = RlcsD.RateLoad[2].dFactor[j];  
    RcfaB.drTE    = RlcsD.RateLoad[3].dFactor[k];
    RcfaB.drTG    = RlcsD.RateLoad[4].dFactor[k];
    RcfaB.drSE    = RlcsD.RateLoad[5].dFactor[0]; 
    RcfaB.drP     = RlcsD.RateLoad[6].dFactor[0]; 
    RcfaB.drUS    = RlcsD.RateLoad[7].dFactor[0];
    RcfaB.drPR    = RlcsD.PrimaryVeh.Factor;
    RcfaB.drAD    = RlcsD.AdjacentVeh.Factor;

    return TRUE;

}

BOOL CDgnStlRatingDataCtrl::Get_RatingFatigueData(int iIJ, int nConcurrent, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCFA_BASE &RcfaB)
{
    return TRUE;
}


void CDgnStlRatingDataCtrl::ConvertRatingGlobalData(T_RPAS_D &RpasD, S_CSGD_D &rData)
{  
    rData.dPhi_y = RpasD.dPhi_y;
    rData.dPhi_u = RpasD.dPhi_u;
    rData.dPhi_c = RpasD.dPhi_c;
    rData.dPhi_f = RpasD.dPhi_f;
    rData.dPhi_v = RpasD.dPhi_v;
    rData.dPhi_se= RpasD.dPhi_se;
    rData.dPhi_b = RpasD.dPhi_b;

    rData.nGirderType = RpasD.nGirderType;
    //rData.nForceType  = RpasD.nForceType;
    //memcpy(rData.bDesignParameter,CpgdD.bDesignParameter,sizeof(rData.bDesignParameter));
    //rData.nConsiderPv = RpasD.nConsiderPv;

    rData.bDistortion =RpasD.bConsiderTorsion;       

    rData.bAppendix6  = RpasD.bStrengthOption[0];
    rData.b1p3RhMy    = RpasD.bStrengthOption[1];          
    rData.bPBTFA4SH   = RpasD.bStrengthOption[2];        

    int nDgnCode = 0;  

    switch ( m_nDgnCode )
    {
    case STL_KSCE_LSD15:
        nDgnCode = KSCE_LSD15_CSG;
        break;
    case STL_AASHTO_LRFD12:
        nDgnCode = AASHTO_LRFD12_CSG;
        break;
    case STL_AASHTO_LRFD19:
        nDgnCode = AASHTO_LRFD17_CSG;
        break;
    default:
        ASSERT(0);
        break;
    }

    rData.nDgnCode = GetConvertCSGCodeCvlToEng(nDgnCode);

    rData.nDgnRatCode		= GetConvertStlRatingCodeCvlToEng(m_nDgnCode);  
    rData.bIsRatingDgn	= rData.nDgnRatCode != 0 ? TRUE : FALSE;
}

void CDgnStlRatingDataCtrl::ConvertRatingGlobalData(T_RPSS_D &RpssD, S_CSGD_D &rData)
{
    rData.nGamma_m = RpssD.nGamma_m;
    rData.bPartialFactorUser = RpssD.bUserInput ? true : false;
	rData.dConditionFactor = RpssD.dConditionFactor;
	rData.dk2 = RpssD.dk2;
    rData.dk3 = RpssD.dk3;
    rData.dl_R = RpssD.dUframeS;
    rData.bApp9_6_4_1_1_2A = RpssD.bEqA4 ? true : false;
    if ( RpssD.bUserInput )
    {
        const T_RPSS_FT_D& FactorD = RpssD.Factors;
        if (m_nDgnCode == STL_CS454_20)
        {
            rData.Gamma_mCSU[0] = FactorD.dCsUlsReinforce;
            rData.Gamma_mCSU[1] = FactorD.dCsUlsConcrete;
            rData.Gamma_mCSU[2] = FactorD.dCsUlsStructStl;
            rData.Gamma_mCSU[3] = FactorD.dCsUlsStiffComp;
            rData.Gamma_mCSU[4] = FactorD.dCsUlsStiffTens;
            rData.Gamma_mCSU[5] = FactorD.dCsUlsShearConc;
            //
            rData.Gamma_mCSS[0] = FactorD.dCsSlsReinforce;
            rData.Gamma_mCSS[1] = FactorD.dCsSlsConcrete;
            rData.Gamma_mCSS[2] = FactorD.dCsSlsStructStl;
            rData.Gamma_mCSS[3] = FactorD.dCsSlsStiffComp;
            rData.Gamma_mCSS[4] = FactorD.dCsSlsStiffTens;
            rData.Gamma_mCSS[5] = FactorD.dCsSlsShearConc;
            //
            rData.Gamma_mWSU[0] = FactorD.dWsUlsReinforce;
            rData.Gamma_mWSU[1] = FactorD.dWsUlsConcrete;
            rData.Gamma_mWSU[2] = FactorD.dWsUlsStructStl;
            rData.Gamma_mWSU[3] = FactorD.dWsUlsStiffComp;
            rData.Gamma_mWSU[4] = FactorD.dWsUlsStiffTens;
            rData.Gamma_mWSU[5] = FactorD.dWsUlsShearConc;
            //
            rData.Gamma_mWSS[0] = FactorD.dWsSlsReinforce;
            rData.Gamma_mWSS[1] = FactorD.dWsSlsConcrete;
            rData.Gamma_mWSS[2] = FactorD.dWsSlsStructStl;
            rData.Gamma_mWSS[3] = FactorD.dWsSlsStiffComp;
            rData.Gamma_mWSS[4] = FactorD.dWsSlsStiffTens;
            rData.Gamma_mWSS[5] = FactorD.dWsSlsShearConc;
        }
		else if (m_nDgnCode == STL_NR_GN_CIV_025_06)
		{
			rData.Gamma_mCSU[0] = FactorD.dCsUlsSteel;
			rData.Gamma_mCSU[1] = FactorD.dCsUlsBuckStiff;
			rData.Gamma_mCSU[2] = FactorD.dCsUlsYieldStiff;
			rData.Gamma_mCSU[3] = FactorD.dCsUlsWFRivet;
			rData.Gamma_mCSU[4] = FactorD.dCsUlsOtherRivet;
			//
            rData.Gamma_mWSU[0] = FactorD.dWsUlsSteel;
            rData.Gamma_mWSU[1] = FactorD.dWsUlsBuckStiff;
            rData.Gamma_mWSU[2] = FactorD.dWsUlsYieldStiff;
            rData.Gamma_mWSU[3] = FactorD.dWsUlsWFRivet;
            rData.Gamma_mWSU[4] = FactorD.dWsUlsOtherRivet;
        }
		else
			ASSERT(0);

        rData.dGamma_f3ULS = FactorD.dUlsGamma_f3;
        rData.dGamma_f3SLS = FactorD.dSlsGamma_f3;
    }

    int nDgnCode = 0;

    switch ( m_nDgnCode )
    {
    case STL_CS454_20:
        nDgnCode = CS457_R1_CSG;
        break;
    case STL_NR_GN_CIV_025_06:
    {
        if (RpssD.nFormula == 0)
        {
            nDgnCode = NR_GN_CIV_025_06_CSG;
        }
        else if (RpssD.nFormula == 1)
        {
            nDgnCode = BS5400_3_00_CSG;
        }
        else
            ASSERT(0);

        int nGrade = RpssD.nReqRouteAvailNum+11;
        rData.enRailRequiredGrade = static_cast<enNRRailRouteGrade>(nGrade); // EN_NR_Rail_Route_Grade_10;
            
        break;
    }
    default:
        ASSERT(0);
        break;
    }
    rData.nDgnCode = GetConvertCSGCodeCvlToEng(nDgnCode);

    rData.nDgnRatCode	= GetConvertStlRatingCodeCvlToEng(m_nDgnCode);
    rData.bIsRatingDgn	= rData.nDgnRatCode != 0 ? TRUE : FALSE;

}

void CDgnStlRatingDataCtrl::ConvertRatStlCalcUlmForce(int nPosI, int nPosM, int nPosJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr,
                                                      FORCE_BLRD_RCASE &ForceRcaseI,
                                                      FORCE_BLRD_RCASE &ForceRcaseM,
                                                      FORCE_BLRD_RCASE &ForceRcaseJ,
                                                      CSG_ULMF_D &rData)
{

    _DGN_FORC_CRC ForI, ForM, ForJ;
    GetRCaseUlmForceSum(bDCmax, bDWmax, bTmax, nConcurr, ForceRcaseI, ForI);
    GetRCaseUlmForceSum(bDCmax, bDWmax, bTmax, nConcurr, ForceRcaseM, ForM);
    GetRCaseUlmForceSum(bDCmax, bDWmax, bTmax, nConcurr, ForceRcaseJ, ForJ);

    m_pCSGDataCtrl->ConvertCsgCalcForce(nPosI, ForI, rData.ForD[0]);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPosM, ForM, rData.ForD[1]);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPosJ, ForJ, rData.ForD[2]);

}

void CDgnStlRatingDataCtrl::ConvertRatStlCalcChkForce(int nPos, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, _DGN_FORC_CRC &ForceNC, FORCE_BLRD_RCASE &ForceRcase, CSG_FORCE_COMP &rData)
{
    int i,j,k;  
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, i, j, k);

    _DGN_FORC_CRC_EX ForLT, ForST, ForLC;
    ForLT += ForceRcase.ForceDCBe[i];
    ForLT += ForceRcase.ForceDC[i];
    ForLT += ForceRcase.ForceDW[j];
    ForLT += ForceRcase.ForcePerm;
    ForLT += ForceRcase.ForceSec;  // [6/30/2014 joahn], 이혜연 과장님 요청 
    //
    int nCCId = nConcurr-1;
    ForST += ForceRcase.ForceTemp[k];
    ForST += ForceRcase.ForceTempG[k];  
    ForST += ForceRcase.ForceUser;
    ForST += ForceRcase.ForceAdVehicle[nCCId];
    ForST += ForceRcase.ForcePrVehicle[nCCId];

    ForLC += ForLT;
    ForLC += ForST;

    _DGN_FORC_CRC ForNC = ForceNC;

    _DGN_FORC_CRC ForceLT, ForceST, ForceLC;
    ForLT.GetForce(ForceLT);
    ForST.GetForce(ForceST);
    ForLC.GetForce(ForceLC);
    ForceLT -= ForNC;

    _DGN_FORC_CRC ForcePV, ForceSV;
    ForceRcase.ForcePrVehicle[nCCId].GetForce(ForcePV);
    ForceRcase.ForceAdVehicle[nCCId].GetForce(ForceSV);

    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForNC, rData.ForNC);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForceLT, rData.ForLT);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForceST, rData.ForST);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForceLC, rData.ForLC);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForcePV, rData.ForMV);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForceSV, rData.ForSV);
}

void CDgnStlRatingDataCtrl::ConvertRatStlCalcChkForceEX(int nPos, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, _DGN_FORC_CRC_EX &ForceNC, FORCE_BLRD_RCASE &ForceRcase, CSG_FORCE_COMP &rData)
{
    _DGN_FORC_CRC_EX ForLT, ForST, ForLC, ForPV, ForSV;
    ConvertRatForceToDgnForce(bIsPLCB, bDCmax, bDWmax, bTmax, nConcurr, ForceRcase, ForLT, ForST, ForLC, ForPV, ForSV);
    
    _DGN_FORC_CRC_EX ForNC = ForceNC;
    ForLT -= ForNC;

    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForNC, rData.ForNC);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForLT, rData.ForLT);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForST, rData.ForST);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForLC, rData.ForLC);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForPV, rData.ForMV);
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForSV, rData.ForSV);
}

void CDgnStlRatingDataCtrl::ConvertRatStlCalcMVChkForce(int nPos, int nMvType, int nConcurr, FORCE_BLRD_RCASE &ForceRcase, CSG_FORCE_COMP &rData)
{
    int nCCId = nConcurr-1;
    _DGN_FORC_CRC ForcePV, ForceAV;
    ForceRcase.ForcePrVehicle[nCCId].GetForce(ForcePV);
    ForceRcase.ForceAdVehicle[nCCId].GetForce(ForceAV);

    _DGN_FORC_CRC &ForST = (nMvType==0) ? ForcePV : ForceAV;
    m_pCSGDataCtrl->ConvertCsgCalcForce(nPos, ForST, rData.ForST);
}

void CDgnStlRatingDataCtrl::ConvertRatForceToDgnForce(BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, FORCE_BLRD_RCASE &ForceRcase,
    _DGN_FORC_CRC_EX& ForLT, _DGN_FORC_CRC_EX& ForST, _DGN_FORC_CRC_EX& ForLC, _DGN_FORC_CRC_EX& ForPV, _DGN_FORC_CRC_EX& ForSV)
{
    ForLT.Initialize();
    ForST.Initialize();
    ForLC.Initialize();
    ForPV.Initialize();
    ForSV.Initialize();

    int i=0, j=0, k=0;
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, i, j, k);

    ForLT += ForceRcase.ForceDCBe[i];
    ForLT += ForceRcase.ForceDC[i];
    ForLT += ForceRcase.ForceDW[j];
    ForLT += ForceRcase.ForcePerm;
    ForLT += ForceRcase.ForceSec;  // [6/30/2014 joahn], 이혜연 과장님 요청 
    //
    int nCCId = nConcurr-1;
    ForST += ForceRcase.ForceTemp[k];
    ForST += ForceRcase.ForceTempG[k];
    ForST += ForceRcase.ForceUser;
    ForST += ForceRcase.ForceAdVehicle[nCCId];
    ForST += ForceRcase.ForcePrVehicle[nCCId];

    ForPV += ForceRcase.ForcePrVehicle[nCCId];//맘에 안들지만 로마법에 따라야지.. Rating일때는 여기다 넣었다가 ST에서 빼는짓을 계산단에서 하고 있다.-_-;
    ForSV += ForceRcase.ForceAdVehicle[nCCId];

    ForLC += ForLT;
    ForLC += ForST;
}

void CDgnStlRatingDataCtrl::ConvertForceDgnToCsgDB(int nPos, _DGN_FORC_CRC &DgnFor, T_CSGD_FORCE &rData)
{
    rData.dForce[0] = DgnFor.dFxx[nPos];
    rData.dForce[1] = DgnFor.dFyy[nPos];
    rData.dForce[2] = DgnFor.dFzz[nPos];
    rData.dForce[3] = DgnFor.dMux[nPos];
    rData.dForce[4] = DgnFor.dMuy[nPos];
    rData.dForce[5] = DgnFor.dMuz[nPos];
}

void CDgnStlRatingDataCtrl::ConvertRatForceCvlToEng(T_RATI_UNITLCOM &For, STL_KR_UNITLCOM &rData)
{
    rData.bUse     = For.bUse;
    rData.nType    = For.nType;
    rData.nOrgLkey = For.nOrgLkey;
    rData.strName  = For.strName;	 
    rData.strDesc  = For.strDesc;
    for(int i=0 ; i<6 ; i++) rData.dForce[i] = For.dForce[i];
}

void CDgnStlRatingDataCtrl::ConvertRatForceEngToCvl(STL_KR_UNITLCOM &For, T_RATI_UNITLCOM &rData)
{
    rData.bUse     = For.bUse;
    rData.nType    = For.nType;
    rData.nOrgLkey = For.nOrgLkey;
    rData.strName  = For.strName;	 
    rData.strDesc  = For.strDesc;
    for(int i=0 ; i<6 ; i++) rData.dForce[i] = For.dForce[i];
}

BOOL CDgnStlRatingDataCtrl::GetRCaseUlmForceSum(BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, FORCE_BLRD_RCASE &ForceRcase, _DGN_FORC_CRC &rData)
{
    int i,j,k;  
    Get_LoadCaseMaxMinId(bDCmax, bDWmax, bTmax, i, j, k);

    _DGN_FORC_CRC_EX ForD;

    int nCCId = nConcurr-1;
    ForD += ForceRcase.ForceDC[i];
    ForD += ForceRcase.ForceDW[j];
    ForD += ForceRcase.ForceTemp[k];
    ForD += ForceRcase.ForceTempG[k];
    ForD += ForceRcase.ForceSec;
    ForD += ForceRcase.ForcePerm;
    ForD += ForceRcase.ForceUser;
    ForD += ForceRcase.ForceAdVehicle[nCCId];

    ForD.GetForce(rData);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::MakeConverMemberForce(T_STCT_D& StctD, int nRCase, T_STAG_K GirderLastStagK, int nStepNo, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr,
    ArrElemPairKey& aUblmElemK,
    T_STRB_D& MembForNC,
    CSG_MEMB_POSD_KSCE_LSD& MembLcomD
)
{
    int nUblElemSize = aUblmElemK.GetSize();
    if ( nUblElemSize < 1 ) return FALSE;

    CArray<T_STRB_D, T_STRB_D&> aStrbElem;
    CArray<T_STRB_D, T_STRB_D&> aStrbElem_LT;
    CArray<T_STRB_D, T_STRB_D&> aStrbElem_ST;
    CArray<T_STRB_D, T_STRB_D&> aStrbElem_PV;
    CArray<T_STRB_D, T_STRB_D&> aStrbElem_SV;
    CArray<T_STRB_D, T_STRB_D&> aStrbElem_Pre;

    aStrbElem.SetSize(nUblElemSize);
    aStrbElem_LT.SetSize(nUblElemSize);
    aStrbElem_ST.SetSize(nUblElemSize);
    aStrbElem_PV.SetSize(nUblElemSize);
    aStrbElem_SV.SetSize(nUblElemSize);
    aStrbElem_Pre.SetSize(nUblElemSize);

    double dMmax = 0.0;
    T_STRB_D MembForD; MembForD.Initialize();
    T_STRB_D MembForD_LTerm; MembForD_LTerm.Initialize();
    T_STRB_D MembForD_STerm; MembForD_STerm.Initialize();
    T_STRB_D MembForD_Pre; MembForD_Pre.Initialize();
    T_STRB_D MembForD_PV; MembForD_PV.Initialize();
    T_STRB_D MembForD_SV; MembForD_SV.Initialize();

    for ( INT_PTR nEl=0; nEl<aUblmElemK.GetSize(); ++nEl )    {
        
        auto EPairK = aUblmElemK[nEl];
        if ( m_nDgnCode==STL_CS454_20 || m_nDgnCode== STL_NR_GN_CIV_025_06 )
        {
            FORCE_ASBD_ELEM ForceAsbdElem;
            const auto& itForceAsbd = m_pForcCtrl->m_arBlrdForceElemBD.find(EPairK);     
            if (itForceAsbd != m_pForcCtrl->m_arBlrdForceElemBD.end())
            {
                ForceAsbdElem = itForceAsbd->second;
                int nRcaseSize = ForceAsbdElem.arForceRCase.GetSize();
                if ( nRcaseSize<=0 ) continue;
                if ( nRcaseSize <= nRCase ) continue;
                FORCE_RCASE_BD ForceRcaseBD = ForceAsbdElem.arForceRCase[nRCase];
                FORCE_BLRD_RCASE ForRcase;
                ForceRcaseBD.ConvertBlrd(ForRcase);
                _DGN_FORC_CRC_EX ForLT, ForST, ForLC, ForPV, ForSV;
                ConvertRatForceToDgnForce(bIsPLCB, bDCmax, bDWmax, bTmax, nConcurr, ForRcase, ForLT, ForST, ForLC, ForPV, ForSV);
                m_pForcCtrl->ConvertDgnForceToStrb(ForLC, aStrbElem[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForLT, aStrbElem_LT[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForST, aStrbElem_ST[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForPV, aStrbElem_PV[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForSV, aStrbElem_SV[nEl]);
                //if ( bIsPLCB ) m_pForcCtrl->ConvertDgnForceToStrb(aDgnMembFor_Pre[nEl].ElemFor[nLcomId], aStrbElem_Pre[nEl]);
            }
        }
        else
        {
            FORCE_BLRD_ELEM ForceElem;
            const auto& itForceElem = m_pForcCtrl->m_arBlrdForceElem.find(EPairK);
            if (itForceElem != m_pForcCtrl->m_arBlrdForceElem.end())
            {
                ForceElem = itForceElem->second;
                int nRcaseSize = ForceElem.arForceRCase.GetSize();
                if ( nRcaseSize<=0 ) continue;
                if ( nRcaseSize <= nRCase ) continue;
                FORCE_BLRD_RCASE ForRcase = ForceElem.arForceRCase[nRCase];
                _DGN_FORC_CRC_EX ForLT, ForST, ForLC, ForPV, ForSV;
                ConvertRatForceToDgnForce(bIsPLCB, bDCmax, bDWmax, bTmax, nConcurr, ForRcase, ForLT, ForST, ForLC, ForPV, ForSV);
                m_pForcCtrl->ConvertDgnForceToStrb(ForLC, aStrbElem[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForLT, aStrbElem_LT[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForST, aStrbElem_ST[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForPV, aStrbElem_PV[nEl]);
                m_pForcCtrl->ConvertDgnForceToStrb(ForSV, aStrbElem_SV[nEl]);
                //if ( bIsPLCB ) m_pForcCtrl->ConvertDgnForceToStrb(aDgnMembFor_Pre[nEl].ElemFor[nLcomId], aStrbElem_Pre[nEl]);
            }
        }
    }
    DGN_MY_MAX_B MyMaxLcom;
    DGN_MY_MAX_B MyMaxLTerm;
    DGN_MY_MAX_B MyMaxSTerm;
    DGN_MY_MAX_B MyMaxPrV;
    DGN_MY_MAX_B MyMaxStV;
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem, &MembForD, MyMaxLcom);  //Unbraced 내 요소들에 대해서 I, 1/4, 1/2, 3/4, J 구간의 부재력을 가져옴(PartI, Part2, Part3, Part4)
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_LT, &MembForD_LTerm, MyMaxLTerm);
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_ST, &MembForD_STerm, MyMaxSTerm);
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_PV, &MembForD_PV, MyMaxPrV);
    m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_SV, &MembForD_SV, MyMaxStV);
    //if ( bIsPLCB ) m_pForcCtrl->GetMembForce(aUblmElemK, aStrbElem_Pre, &MembForD_Pre, FALSE);

    if (MyMaxLTerm.ElemK == 0)
		MyMaxLTerm = MyMaxLcom;
    ElemPairK EMymaxPairK(MyMaxLTerm.ElemK, EN_EL_BEAM);
    m_pCSGDataCtrl->Get_CsgUlmsData4MyMax(EMymaxPairK, MyMaxLTerm.nPosIJ, MembLcomD.Memb.Ulms);

    if ( MembForNC.nDOFType==2 )
    {
        MembForD.nDOFType       = MembForNC.nDOFType;
        MembForD_LTerm.nDOFType = MembForNC.nDOFType;
        MembForD_STerm.nDOFType = MembForNC.nDOFType;
        MembForD_Pre.nDOFType   = MembForNC.nDOFType;
        MembForD_PV.nDOFType    = MembForNC.nDOFType;
        MembForD_SV.nDOFType    = MembForNC.nDOFType;
    }

    T_STRB_D MembForNC_tp = MembForNC;
    MembForNC_tp.Mult(-1.0);
    MembForD_LTerm.Add(MembForNC_tp);  // 완성계 LT에서 NC 빼줌.

    T_STRB_D MembNCForD = bIsPLCB==TRUE ? MembForD_Pre : MembForNC;

    //m_pCSGDataCtrl->ConvertCsgCalcUblMForce(0, MembForD, MembLcomD.aUlmFor[0].ForD[1].ForLC);   //MembForD의 Part1의 J단을 Ma로 저장
    //m_pCSGDataCtrl->ConvertCsgCalcUblMForce(1, MembForD, MembLcomD.aUlmFor[0].ForD[2].ForLC);   //MembForD의 Part2의 J단을 Mb로 저장
    //m_pCSGDataCtrl->ConvertCsgCalcUblMForce(2, MembForD, MembLcomD.aUlmFor[0].ForD[3].ForLC);   //MembForD의 Part3의 J단을 Mc로 저장
    //MembLcomD.aUlmFor[0].dMmax = m_pForcCtrl->GetMaxMyByMembForce(&MembForD);
    int nMid = 0;
    int nCom = 0;
    m_pCSGDataCtrl->GetUlmMidIndex(MembForD, nMid, nCom);
    m_pCSGDataCtrl->ConvertCsgCalcUlmForce(MembForD,  MembLcomD.aUlmf[0]);      //LC (NC+LT+ST)
    m_pCSGDataCtrl->ConvertCsgCalcUlmForce(MembNCForD,  MembLcomD.aUlmf[1]);    //NC (_T("Dead Load")의한 부재력 + Erection Dead Before부재력)
    m_pCSGDataCtrl->ConvertCsgCalcUlmForce(MembForD_LTerm,  MembLcomD.aUlmf[2]);//LT
    m_pCSGDataCtrl->ConvertCsgCalcUlmForce(MembForD_STerm,  MembLcomD.aUlmf[3]);//ST
    m_pCSGDataCtrl->ConvertCsgCalcUlmForce(MembForD_PV,  MembLcomD.aUlmf[4]);   //PV
    m_pCSGDataCtrl->ConvertCsgCalcUlmForce(MembForD_SV,  MembLcomD.aUlmf[5]);   //SV

    m_pCSGDataCtrl->ConvertCsgCalcUlmForceCb(MembForD, nMid, nCom, MembLcomD.aUlmf[0]);      //LC (NC+LT+ST)
    m_pCSGDataCtrl->ConvertCsgCalcUlmForceCb(MembNCForD, nMid, nCom, MembLcomD.aUlmf[1]);    //NC (_T("Dead Load")의한 부재력 + Erection Dead Before부재력)
    m_pCSGDataCtrl->ConvertCsgCalcUlmForceCb(MembForD_LTerm, nMid, nCom, MembLcomD.aUlmf[2]);//LT
    m_pCSGDataCtrl->ConvertCsgCalcUlmForceCb(MembForD_STerm, nMid, nCom, MembLcomD.aUlmf[3]);//ST
    m_pCSGDataCtrl->ConvertCsgCalcUlmForceCb(MembForD_PV, nMid, nCom, MembLcomD.aUlmf[4]);   //PV
    m_pCSGDataCtrl->ConvertCsgCalcUlmForceCb(MembForD_SV, nMid, nCom, MembLcomD.aUlmf[5]);   //SV

    
    MembLcomD.Memb.Ulms.nElemK4Mymax = MyMaxLTerm.ElemK;
    MembLcomD.Memb.Ulms.nSectK4Mymax = MyMaxLTerm.SectK;
    MembLcomD.Memb.Ulms.nPos4Mymax = MyMaxLTerm.nPos5;

    int nMyMaxId = 0;
    for (int m = 0; m < aUblmElemK.GetSize(); m++)
    {
        if (aUblmElemK[m].first == MyMaxLTerm.ElemK)
        {
            break;
        }
        nMyMaxId++;
    }
        
    int nForceBeforeType = 1;
    _DGN_FORC_CRC_EX ForceNC;
    m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM_EX(FALSE, StctD, EMymaxPairK, GirderLastStagK, nStepNo, nRCase, nForceBeforeType, ForceNC);
    m_pCSGDataCtrl->ConvertCsgCalcUblMForce4Pos(MyMaxLTerm.nPos5, aStrbElem[nMyMaxId], MembLcomD.aUlmf[0].MyForce);
    m_pCSGDataCtrl->ConvertCsgCalcForce(MyMaxLTerm.nPos5, ForceNC, MembLcomD.aUlmf[1].MyForce);
    m_pCSGDataCtrl->ConvertCsgCalcUblMForce4Pos(MyMaxLTerm.nPos5, aStrbElem_LT[nMyMaxId], MembLcomD.aUlmf[2].MyForce);
    m_pCSGDataCtrl->ConvertCsgCalcUblMForce4Pos(MyMaxLTerm.nPos5, aStrbElem_ST[nMyMaxId], MembLcomD.aUlmf[3].MyForce);
    m_pCSGDataCtrl->ConvertCsgCalcUblMForce4Pos(MyMaxLTerm.nPos5, aStrbElem_PV[nMyMaxId], MembLcomD.aUlmf[4].MyForce);
    m_pCSGDataCtrl->ConvertCsgCalcUblMForce4Pos(MyMaxLTerm.nPos5, aStrbElem_SV[nMyMaxId], MembLcomD.aUlmf[5].MyForce);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::MakePanelForce(T_STCT_D &StctD, int nRCase, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr,
    _DGN_FORC_CRC_EX& ForceNC,
    CSG_MEMB_POSD_KSCE_LSD& MembLcomD)
{
    int nForceBeforeType = 1;
    int nRCaseIndex = nRCase - 1;
    int nConcurrentId = 1; // FORCE_BLRD_RCASE 의 ConvertBlrd를 하기 때문에 1로 고정

	BOOL bWebpAssigned = FALSE;
    T_WEBP_D WebpD;
	T_WEBV_D WebvD;
    BOOL bWebPanel = FALSE;
    ArrElemPairKey aWebPanelPairK;
    if (ElemK.second == EN_EL_BEAM)
    {
        T_WEBP_K WebpK = 0;
        bWebpAssigned = m_pDoc->m_pAttrCtrl2->GetWebpAssigned(ElemK.first, WebpK);
        bWebPanel = m_pDoc->m_pAttrCtrl2->GetWebp(WebpK, WebpD);
		if (bWebPanel)
		{
			CDBLib::ConvertToElemPairKey(EN_EL_BEAM, WebpD.aElemList, aWebPanelPairK);
		}
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
		T_WEBV_K WebvK = 0;
        //bWebpAssigned = m_pDoc->m_pAttrCtrl2->GetWebpAssigned(ElemK.first, WebvK);
        bWebPanel = m_pDoc->m_pAttrCtrl2->GetWebv(WebvK, WebvD);
        if (bWebPanel)
        {
            CDBLib::ConvertToElemPairKey(EN_EL_VBEAM, WebvD.aElemList, aWebPanelPairK);
        }
    }
    else ASSERT(0);

    
    if (bWebpAssigned)
    {
        if (bWebPanel)
        {
            double dMy_max = -DBL_MAX;
            double dVz_max = -DBL_MAX;
            T_ELEM_K MyElemK = 0;
            T_ELEM_K VzElemK = 0;
            int nMyPos = 0;
            int nVzPos = 0;
            BOOL bConvertM = FALSE;
            BOOL bConvertV = FALSE;
            for ( INT_PTR nEl=0; nEl< aWebPanelPairK.GetSize(); ++nEl )
            {
                auto CurEPairK = aWebPanelPairK[nEl];
                FORCE_ASBD_ELEM ForceAsbdElem;
                const auto& itForceAsbd = m_pForcCtrl->m_arBlrdForceElemBD.find(CurEPairK);
                if (itForceAsbd != m_pForcCtrl->m_arBlrdForceElemBD.end())
                {
					ForceAsbdElem = itForceAsbd->second;
                }                 
                int nRcaseSize = ForceAsbdElem.arForceRCase.GetSize();
                if ( nRcaseSize<=0 ) continue;
                if ( nRcaseSize <= nRCase ) continue;

                int nRCDgnLcomNo = nRCase;
                _DGN_FORC_CRC_EX ForceNC_EX;
                m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM_EX(FALSE, StctD, CurEPairK, GirderLastStagK, nStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC_EX);
                                
                FORCE_RCASE_BD ForceRcaseBD = ForceAsbdElem.arForceRCase[nRCaseIndex];
                FORCE_BLRD_RCASE ForceBlrdRcase;
                ForceRcaseBD.ConvertBlrd(ForceBlrdRcase);
                                
                if ( nEl==0 )
                {
                    ConvertRatStlCalcChkForceEX(0, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[0]);
                }
                if ( nEl==WebpD.aElemList.GetSize()-1 )
                {
                    ConvertRatStlCalcChkForceEX(4, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[1]);
                }
            }
        }
    }
    else
    {
        FORCE_ASBD_ELEM ForceAsbdElem;
        const auto& itForceAsbd = m_pForcCtrl->m_arBlrdForceElemBD.find(ElemK);
        if (itForceAsbd != m_pForcCtrl->m_arBlrdForceElemBD.end())
        {
            ForceAsbdElem = itForceAsbd->second;
        }
        int nRcaseSize = ForceAsbdElem.arForceRCase.GetSize();

        int nRCDgnLcomNo = nRCase;
        _DGN_FORC_CRC_EX ForceNC_EX;
        m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM_EX(FALSE, StctD, ElemK, GirderLastStagK, nStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC_EX);

        FORCE_RCASE_BD ForceRcaseBD = ForceAsbdElem.arForceRCase[nRCaseIndex];
        FORCE_BLRD_RCASE ForceBlrdRcase;
        ForceRcaseBD.ConvertBlrd(ForceBlrdRcase);

        ConvertRatStlCalcChkForceEX(0, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[0]);
        ConvertRatStlCalcChkForceEX(4, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[1]);
    }


    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::MakePanelForceMax(T_STCT_D &StctD, int nRCase, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr,
    CSG_MEMB_POSD_KSCE_LSD& MembLcomD)
{
    int nForceBeforeType = 1;
    int nOrgRcaseTarget=0, nRatingCase=0, nRcaseTypeDL=0, nRcaseTypeLL=0;
    if ( !m_pForcCtrl->Get_RcaseType(nRCase, nOrgRcaseTarget, nRatingCase, nRcaseTypeDL, nRcaseTypeLL) ) ASSERT(0);

    BOOL bWebpAssigned = FALSE;
    T_WEBP_D WebpD;
    T_WEBV_D WebvD;
    BOOL bWebPanel = FALSE;
    ArrElemPairKey aWebPanelPairK;
    if (ElemK.second == EN_EL_BEAM)
    {
        T_WEBP_K WebpK = 0;
        bWebpAssigned = m_pDoc->m_pAttrCtrl2->GetWebpAssigned(ElemK.first, WebpK);
        bWebPanel = m_pDoc->m_pAttrCtrl2->GetWebp(WebpK, WebpD);
        if (bWebPanel)
        {
            CDBLib::ConvertToElemPairKey(EN_EL_BEAM, WebpD.aElemList, aWebPanelPairK);
        }
    }
    else if (ElemK.second == EN_EL_VBEAM)
    {
        T_WEBV_K WebvK = 0;
        //bWebpAssigned = m_pDoc->m_pAttrCtrl2->GetWebpAssigned(ElemK.first, WebvK);
        bWebPanel = m_pDoc->m_pAttrCtrl2->GetWebv(WebvK, WebvD);
        if (bWebPanel)
        {
            CDBLib::ConvertToElemPairKey(EN_EL_VBEAM, WebvD.aElemList, aWebPanelPairK);
        }
    }
    else ASSERT(0);

    T_WEBP_K WebpK = 0;
    if (bWebpAssigned)
    {
        if (bWebPanel)
        {
            double dMy_max = -DBL_MAX;
            double dVz_max = -DBL_MAX;
            T_ELEM_K MyElemK = 0;
            T_ELEM_K VzElemK = 0;
            int nMyPos = 0;
            int nVzPos = 0;
            BOOL bConvertM = FALSE;
            BOOL bConvertV = FALSE;
            for ( INT_PTR nEl=0; nEl< aWebPanelPairK.GetSize(); ++nEl )
            {
                auto CurEPairK = aWebPanelPairK[nEl];
                FORCE_ASBD_ELEM ForceAsbdElem;
                const auto& itForceAsbd = m_pForcCtrl->m_arBlrdForceElemBD.find(CurEPairK);
                if (itForceAsbd != m_pForcCtrl->m_arBlrdForceElemBD.end())
                {
                    ForceAsbdElem = itForceAsbd->second;
                }
                int nRcaseSize = ForceAsbdElem.arForceRCase.GetSize();
                for ( int nR=0; nR<nRcaseSize; ++nR )
                {
                    int nRcaseNoT = nR+1;
                    int nCurrOrgRcase=0, nRatingCaseT=0, nRcaseTypeDLTemp=0, nRcaseTypeLLTemp=0;
                    if ( !m_pForcCtrl->Get_RcaseType(nRcaseNoT, nCurrOrgRcase, nRatingCaseT, nRcaseTypeDLTemp, nRcaseTypeLLTemp) ) ASSERT(0);
                    if ( nOrgRcaseTarget != nCurrOrgRcase ) continue;

                    int nRCDgnLcomNo = nRcaseNoT;
                    _DGN_FORC_CRC_EX ForceNC_EX;
                    m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM_EX(FALSE, StctD, CurEPairK, GirderLastStagK, nStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC_EX);

                    FORCE_RCASE_BD ForceRcaseBD = ForceAsbdElem.arForceRCase[nR];
                    FORCE_BLRD_RCASE ForceBlrdRcase;
                    ForceRcaseBD.ConvertBlrd(ForceBlrdRcase);

                    int nConcurrentId = 1; // FORCE_BLRD_RCASE 의 ConvertBlrd를 하기 때문에 1로 고정
                    for ( int p=0; p<5; ++p )
                    {
                        double dMyi = ForceRcaseBD.ForceSd.dMuy[p] + ForceRcaseBD.ForceSst.dMuy[p] + ForceRcaseBD.ForceS.dMuy[p];
                        double dVzi = ForceRcaseBD.ForceSd.dFzz[p] + ForceRcaseBD.ForceSst.dFzz[p] + ForceRcaseBD.ForceS.dFzz[p];
                        if ( fabs(dMyi) > dMy_max )
                        {
                            dMy_max = fabs(dMyi);
                            MyElemK = CurEPairK.first;
                            nMyPos  = p;
                            bConvertM = TRUE;
                        }
                        if ( fabs(dVzi) > dVz_max )
                        {
                            dVz_max = fabs(dVzi);
                            VzElemK = CurEPairK.first;
                            nVzPos  = p;
                            bConvertV = TRUE;
                        }
                    }

                    if ( bConvertM )
                    {
                        ConvertRatStlCalcChkForceEX(nMyPos, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.PanelM);
                        bConvertM = FALSE;
                    }
                    if ( bConvertV )
                    {
                        ConvertRatStlCalcChkForceEX(nVzPos, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.PanelV);
                        bConvertV = FALSE;
                    }

//                     if ( nEl==0 )
//                     {
//                         ConvertRatStlCalcChkForceEX(0, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[0]);
//                     }
//                     if ( nEl==WebpD.aElemList.GetSize()-1 )
//                     {
//                         ConvertRatStlCalcChkForceEX(4, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[1]);
//                     }
                }

            }
        }
    }
    else
    {
        FORCE_ASBD_ELEM ForceAsbdElem;
        const auto& itForceAsbd = m_pForcCtrl->m_arBlrdForceElemBD.find(ElemK);
        if (itForceAsbd != m_pForcCtrl->m_arBlrdForceElemBD.end())
        {
            ForceAsbdElem = itForceAsbd->second;
        }
        int nRcaseSize = ForceAsbdElem.arForceRCase.GetSize();

        double dMy_max = -DBL_MAX;
        double dVz_max = -DBL_MAX;
        int nMyPos = 0;
        int nVzPos = 0;
        BOOL bConvertM = FALSE;
        BOOL bConvertV = FALSE;

        for ( int nR=0; nR<nRcaseSize; ++nR )
        {
            int nRcaseNoT = nR+1;
            int nCurrOrgRcase=0, nRatingCaseT=0, nRcaseTypeDLTemp=0, nRcaseTypeLLTemp=0;
            if ( !m_pForcCtrl->Get_RcaseType(nRcaseNoT, nCurrOrgRcase, nRatingCaseT, nRcaseTypeDLTemp, nRcaseTypeLLTemp) ) ASSERT(0);
            if ( nOrgRcaseTarget != nCurrOrgRcase ) continue;

            int nRCDgnLcomNo = nRcaseNoT;
            _DGN_FORC_CRC_EX ForceNC_EX;
            m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM_EX(FALSE, StctD, ElemK, GirderLastStagK, nStepNo, nRCDgnLcomNo, nForceBeforeType, ForceNC_EX);

            FORCE_RCASE_BD ForceRcaseBD = ForceAsbdElem.arForceRCase[nR];
            FORCE_BLRD_RCASE ForceBlrdRcase;
            ForceRcaseBD.ConvertBlrd(ForceBlrdRcase);

            int nConcurrentId = 1; // FORCE_BLRD_RCASE 의 ConvertBlrd를 하기 때문에 1로 고정
            for ( int p=0; p<5; ++p )
            {
                double dMyi = ForceRcaseBD.ForceSd.dMuy[p] + ForceRcaseBD.ForceSst.dMuy[p] + ForceRcaseBD.ForceS.dMuy[p];
                double dVzi = ForceRcaseBD.ForceSd.dFzz[p] + ForceRcaseBD.ForceSst.dFzz[p] + ForceRcaseBD.ForceS.dFzz[p];
                if ( fabs(dMyi) > dMy_max )
                {
                    dMy_max = fabs(dMyi);
                    nMyPos  = p;
                    bConvertM = TRUE;
                }
                if ( fabs(dVzi) > dVz_max )
                {
                    dVz_max = fabs(dVzi);
                    nVzPos  = p;
                    bConvertV = TRUE;
                }
            }

            if ( bConvertM )
            {
                ConvertRatStlCalcChkForceEX(nMyPos, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.PanelM);
                bConvertM = FALSE;
            }
            if ( bConvertV )
            {
                ConvertRatStlCalcChkForceEX(nVzPos, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.PanelV);
                bConvertV = FALSE;
            }
            
            //ConvertRatStlCalcChkForceEX(0, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[0]);
            //ConvertRatStlCalcChkForceEX(4, bIsPLCB, bDCmax, bDWmax, bTmax, nConcurrentId, ForceNC_EX, ForceBlrdRcase, MembLcomD.Panelij[1]);
        }

    }
    return TRUE;
}

BOOL CDgnStlRatingDataCtrl::MakeReactionRCase(const T_ALCS_D& AlcsD, ElemPairK EPairK, int nPosIJ, int nMaxMinType,
    T_REAC_D& DLRe, T_REAC_D& SLLRe, T_REAC_D& DLLRe, CSG_MEMB_POSD_KSCE_LSD& MembLcomD)
{
    int nLoadCaseNum = AlcsD.arLoadComb.GetSize();
    if (nLoadCaseNum == 0) return FALSE;

    MembLcomD.aReac.RemoveAll();
    MembLcomD.aReacSLL.RemoveAll();
    MembLcomD.aReacDLL.RemoveAll();

    int nChkLcomSize = 1;
    MembLcomD.aReac.SetSize(nChkLcomSize);
    MembLcomD.aReacSLL.SetSize(nChkLcomSize);
    MembLcomD.aReacDLL.SetSize(nChkLcomSize);

	auto L_ConvertReac = [&](const T_REAC_D& Reac, CSGREACFOR_ARRAY& aReacD) -> void
    {
        if (aReacD.GetSize()==0) return;
        aReacD[0].dFz = Reac.dblReac[2];
		memcpy(aReacD[0].adReac, Reac.dblReac, sizeof(Reac.dblReac));       
	};

    // Dead Load
    T_LCOM_D DeadLcomD; DeadLcomD.Initialize();
	DeadLcomD.aCombination.Copy(AlcsD.arLoadComb);

    m_pCSGDataCtrl->GetReactionNodeOfElem(DeadLcomD, EPairK, nPosIJ, nMaxMinType, DLRe);
    L_ConvertReac(DLRe, MembLcomD.aReac);

    // Static
    if (AlcsD.bCombinedMV)
    {
        T_LCOM_D StaLcomD; StaLcomD.Initialize();
        T_LCOM_BASE StaLL = AlcsD.CombinedMVCase;
        StaLcomD.aCombination.Add(StaLL);
        m_pCSGDataCtrl->GetReactionNodeOfElem(StaLcomD, EPairK, nPosIJ, nMaxMinType, SLLRe);
        L_ConvertReac(SLLRe, MembLcomD.aReacSLL);
    }

	// Dynamic
    if (AlcsD.bSpecialMV)
    {
        T_LCOM_D DynLcomD; DynLcomD.Initialize();
        T_LCOM_BASE DynLL = AlcsD.SpecialMVCase;
        DynLcomD.aCombination.Add(DynLL);
        m_pCSGDataCtrl->GetReactionNodeOfElem(DynLcomD, EPairK, nPosIJ, nMaxMinType, DLLRe);
        L_ConvertReac(DLLRe, MembLcomD.aReacDLL);
    }

    return TRUE;
}

void CDgnStlRatingDataCtrl::SetRccrBaseData(ElemPairK ElemK, UINT nPos, T_GRUP_K nGrupKey, T_RLCS_K RlcsK, BOOL bDCmax, BOOL bDWmax, BOOL bTEmax, int nConcurr, T_RCCR_BASE &rData)
{
    rData.ElemK       = ElemK.first;
    rData.nBeamType   = ElemK.second;
    rData.nPos        = nPos;
    rData.nGrupKey    = nGrupKey;
    rData.RCaseK      = RlcsK;
    rData.bDCmax      = bDCmax;
    rData.bDWmax      = bDWmax;
    rData.bTEmax      = bTEmax;
    rData.nConcurrent = nConcurr;
}

BOOL CDgnStlRatingDataCtrl::Get_LoadCaseMaxMinId(BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int &nI,int &nJ, int &nK)
{  
    nI=nJ=nK=0;
    if(bDCmax && bDWmax && bTmax)        { nI=0, nJ=0, nK=0; }
    else if(bDCmax && bDWmax && !bTmax)  { nI=0, nJ=0, nK=1; }
    else if(bDCmax && !bDWmax && bTmax)  { nI=0, nJ=1, nK=0; }
    else if(bDCmax && !bDWmax && !bTmax) { nI=0, nJ=1, nK=1; }
    else if(!bDCmax && bDWmax && bTmax)  { nI=1, nJ=0, nK=0; }
    else if(!bDCmax && bDWmax && !bTmax) { nI=1, nJ=0, nK=1; }
    else if(!bDCmax && !bDWmax && bTmax) { nI=1, nJ=1, nK=0; }
    else if(!bDCmax && !bDWmax && !bTmax){ nI=1, nJ=1, nK=1; }
    else ASSERT(0);  
    return TRUE;
}

void CDgnStlRatingDataCtrl::SetRcaseStrengthResult(ElemPairK CheckElemK, UINT nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK,
                                                   BOOL bDCmax, BOOL bDWmax, BOOL bTmax, UINT nConCurr, BOOL bSpanInfo, 
                                                   T_SPAN_K SpanKey, UINT nSpanPos, double dRel_Locat, double dPhi_CF, double dPhi_SF,
                                                   T_RCST_BASE &RcstB)
{
    RcstB.bChk        = TRUE;
    RcstB.ElemK       = CheckElemK.first;
    RcstB.nPosi       = nPos; // nPosi : 0=I-End, 1=J-End              
    RcstB.nGrupKey    = CheckGroupK;
    RcstB.RCaseK      = RlcsK;
    RcstB.bDCmax      = bDCmax;
    RcstB.bDWmax      = bDWmax;
    RcstB.bTEmax      = bTmax;
    RcstB.nConcurrent = nConCurr;            
    RcstB.nIsLocation = bSpanInfo ? 1 : 0; // Is Location Data 0=Non, 1=Exist
    RcstB.SpanKey     = SpanKey;
    RcstB.nSpanPos    = nSpanPos;
    RcstB.dRel_Locat  = dRel_Locat;
    RcstB.dPhi_CF     = dPhi_CF;
    RcstB.dPhi_SF     = dPhi_SF;      

    RcstB.DtkF.dEpsiC = m_pRpasD->nMeasurement == 0 ? m_dEpsiC : m_dDispC;
    RcstB.DtkF.dEpsiT = m_pRpasD->nMeasurement == 0 ? m_dEpsiT : m_dDispT;
    RcstB.DtkF.dKa    = m_dKa;
    RcstB.DtkF.dK     = m_dK;
    RcstB.DtkF.dKb    = m_dKb;

    RcstB.DtkS = RcstB.DtkF;
}

void CDgnStlRatingDataCtrl::SetRcaseServiceResult(ElemPairK CheckElemK, UINT nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK,
                                                  BOOL bDCmax, BOOL bDWmax, BOOL bTmax, UINT nConCurr, BOOL bSpanInfo, 
                                                  T_SPAN_K SpanKey, UINT nSpanPos, double dRel_Locat, double dPhi_CF, double dPhi_SF,
                                                  T_RCSS_BASE &RcssB)
{
    RcssB.bChk        = TRUE;
    RcssB.ElemK       = CheckElemK.first;
    RcssB.nPosi       = nPos; // nPosi : 0=I-End, 1=J-End              
    RcssB.nGrupKey    = CheckGroupK;
    RcssB.RCaseK      = RlcsK;
    RcssB.bDCmax      = bDCmax;
    RcssB.bDWmax      = bDWmax;
    RcssB.bTEmax      = bTmax;
    RcssB.nConcurrent = nConCurr;            
    RcssB.nIsLocation = bSpanInfo ? 1 : 0; // Is Location Data 0=Non, 1=Exist
    RcssB.SpanKey     = SpanKey;
    RcssB.nSpanPos    = nSpanPos;
    RcssB.dRel_Locat  = dRel_Locat;
    RcssB.dPhi_CF     = dPhi_CF;
    RcssB.dPhi_SF     = dPhi_SF;          

    RcssB.DtkS[1].dEpsiC = m_pRpasD->nMeasurement == 0 ? m_dEpsiC : m_dDispC;
    RcssB.DtkS[1].dEpsiT = m_pRpasD->nMeasurement == 0 ? m_dEpsiT : m_dDispT;
    RcssB.DtkS[1].dKa    = m_dKa;
    RcssB.DtkS[1].dK     = m_dK;
    RcssB.DtkS[1].dKb    = m_dKb;

    RcssB.DtkS[2] = RcssB.DtkS[1];
}

void CDgnStlRatingDataCtrl::SetRcaseFatiResult(ElemPairK CheckElemK, UINT nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK,
                                               BOOL bDCmax, BOOL bDWmax, BOOL bTmax, UINT nConCurr, BOOL bSpanInfo,
                                               T_SPAN_K SpanKey, UINT nSpanPos, double dRel_Locat, double dPhi_CF, double dPhi_SF,
                                               T_RCFA_BASE &RcfaB)
{
    RcfaB.bChk        = TRUE;
    RcfaB.ElemK       = CheckElemK.first;
    RcfaB.nPosi       = nPos; // nPosi : 0=I-End, 1=J-End              
    RcfaB.nGrupKey    = CheckGroupK;
    RcfaB.RCaseK      = RlcsK;
    RcfaB.bDCmax      = bDCmax;
    RcfaB.bDWmax      = bDWmax;
    RcfaB.bTEmax      = bTmax;
    RcfaB.nConcurrent = nConCurr;            
    RcfaB.nIsLocation = bSpanInfo ? 1 : 0; // Is Location Data 0=Non, 1=Exist
    RcfaB.SpanKey     = SpanKey;
    RcfaB.nSpanPos    = nSpanPos;
    RcfaB.dRel_Locat  = dRel_Locat;
    RcfaB.dPhi_CF     = dPhi_CF;
    RcfaB.dPhi_SF     = dPhi_SF;            

    RcfaB.DtkS[0].dEpsiC = m_pRpasD->nMeasurement == 0 ? m_dEpsiC : m_dDispC;
    RcfaB.DtkS[0].dEpsiT = m_pRpasD->nMeasurement == 0 ? m_dEpsiT : m_dDispT;
    RcfaB.DtkS[0].dKa    = m_dKa;
    RcfaB.DtkS[0].dK     = m_dK;
    RcfaB.DtkS[0].dKb    = m_dKb;

    RcfaB.DtkS[1] = RcfaB.DtkS[0];
}

int CDgnStlRatingDataCtrl::GetPosNegIndex(BOOL bPositive, CSG_MEMB_RES_AASHTO_LRFD_STR_D &StrnR)
{
    int nPosNeg = 0;
    if (bPositive)
    {
        if (StrnR.FlexD[0].nCase != 0) nPosNeg = 0;
        else                           nPosNeg = 1;
    }
    else
    {
        if (StrnR.FlexD[1].nCase != 0) nPosNeg = 1;
        else                           nPosNeg = 0;
    }
    return nPosNeg;
}

double CDgnStlRatingDataCtrl::Calc_1PlusImpF(double dImpFactor)
{
    return 1.0 + dImpFactor;
}

BOOL CDgnStlRatingDataCtrl::Calc_CbFactor(T_SECT_D &SectD, double dMy1, double dMy2, double& dCb)
{
    dCb = 1.0; 

    bool bSingleCurve=FALSE;	
    double dCbUpLim = 2.3;
    if(dCb <= 0.0)
    {
        BOOL bCantilever = (m_pForcCtrl->m_pDgnForceCtrl->m_iCantilever > 0 ? TRUE : FALSE);
        double dM1	= fabs(m_pForcCtrl->m_pDgnForceCtrl->Get_EndMy1(1));
        double dM2	= fabs(m_pForcCtrl->m_pDgnForceCtrl->Get_EndMy2(1));
        double dM04 = m_pForcCtrl->m_pDgnForceCtrl->Get_EndMye(1,0);
        double dM14 = m_pForcCtrl->m_pDgnForceCtrl->Get_EndMye(1,1);
        double dM24 = m_pForcCtrl->m_pDgnForceCtrl->Get_EndMye(1,2);
        double dM34 = m_pForcCtrl->m_pDgnForceCtrl->Get_EndMye(1,3);
        double dM44 = m_pForcCtrl->m_pDgnForceCtrl->Get_EndMye(1,4);

        {
            // All Moment are Same Sign.
            bSingleCurve = m_pForcCtrl->m_pDgnForceCtrl->IsSingleCurve(dM04,dM14,dM24,dM34,dM44);
            if(dM1 < m_dZero)
            {
                if(bCantilever)			bSingleCurve = TRUE;	// Cantilever.
            }
            if(dM2 < m_dZero)		{dCb=1.0; }
            else if(bCantilever)	{dCb=1.0; }
            else
            {
                double dPl=0.0, dPh=0.0;
                Calc_Pl_Ph_SectForce(SectD, dMy1, dMy2, dPl, dPh);
                dCb = Cal_Cb(bSingleCurve, dPl, dPh);
            }
            // Mid Moment is larger than End Moments.(Compared Absolute Values).
            if(!bSingleCurve)	// Only double curve.
            {
                double dMidM = max(max(fabs(dM14),fabs(dM34)),fabs(dM24));
                if(dMidM > dM2)	{dCb=1.0; }
            }
        }
    }	
    return bSingleCurve;
}

BOOL CDgnStlRatingDataCtrl::Calc_Pl_Ph_SectForce(T_SECT_D &SectD, double dMy1, double dMy2, double& dPl, double& dPh)
{
    CString strShape = SectD.SectBefore.Shape;
    int nPos=0;
    double aSize[8]={0.0,};
    double dP1=0.0, dP2=0.0;

    const T_SECT_SECTBASE_D& SectI = SectD.SectBefore.SectI;
    const T_SECT_STIFFNESS& SectStiff = SectI.Stiffness;

    for(int i=0; i<2; i++)
    {
        nPos = i==0 ? 0 : 4;
        aSize[0] = SectI.Size[0];
        aSize[1] = SectI.Size[1];
        aSize[2] = SectI.Size[2];
        aSize[3] = SectI.Size[3];
        aSize[4] = SectI.Size[4];
        aSize[5] = SectI.Size[5];
        aSize[6] = SectI.Size[6];
        aSize[7] = SectI.Size[7];
        double dA   = SectStiff.Area;
        double dIy  = SectStiff.Ryy;
        double dIz  = SectStiff.Rzz;
        double dCzp = SectStiff.Czp;
        double dCzm = SectStiff.Czm;

        if(i==0) {dP1 = Get_SectForce(strShape, aSize, dA, dIy, dIz, dCzp, dCzm, dMy1);}
        else     {dP2 = Get_SectForce(strShape, aSize, dA, dIy, dIz, dCzp, dCzm, dMy2);}
    }
    if(dP1<dP2) { dPh = dP2, dPl = dP1; }
    else        { dPh = dP1, dPl = dP2; }

    return TRUE;
}

double CDgnStlRatingDataCtrl::Get_SectForce(CString strShape, double aSize[8], double dA, double dIy, double dIz, double dCzp, double dCzm, double dMy)
{
    double dP=0.0;
    double dy1=0.0, dz1=0.0;
    double dy2=0.0, dz2=0.0;
    BOOL bPositiveM = dMy>=0.0;

    double dFx=0.0, dMz=0.0;
    double dStr1=0.0, dStr2=0.0;
    double dHt = dCzp + dCzm;
    double dbf=0.0,dtf=0.0;
    if(strShape==D_SECT_SHAPE_REG_H)
    {
        if(bPositiveM)
        {
            dbf = aSize[1];
            dtf = aSize[3];
            dz1 = dHt-dCzm;
            dz2 = dz1-dtf;      
        }
        else
        {
            dbf = aSize[4];
            dtf = aSize[5];
            dz1 = -dCzm;
            dz2 = dtf-dCzm;      
        }
    }
    else if(strShape==D_SECT_SHAPE_REG_B)
    {
        if(bPositiveM)
        {
            dbf = aSize[1];
            dtf = aSize[3];
            dz1 = dHt-dCzm;
            dz2 = dz1-dtf;      
        }
        else
        {
            dbf = aSize[1];
            dtf = aSize[5];
            dz1 = -dCzm;
            dz2 = dtf-dCzm;      
        }
    }
    else
    {
        // ASSERT(0);
        // Nothing
    }


    dStr1 = Calc_StressOfPosition(dA, dIy, dIz, dy1, dz1, dFx, dMy, dMz);
    dStr2 = Calc_StressOfPosition(dA, dIy, dIz, dy2, dz2, dFx, dMy, dMz);
    dP = fabs(dStr1 + dStr2)*0.5*dtf * dbf;

    return dP;
}

double CDgnStlRatingDataCtrl::Calc_StressOfPosition(double dA, double dIy, double dIz, double dy, double dz, double dFx, double dMy, double dMz)
{
    // dy, dz 는 도심이 원점이 되며 사분면을 고려해서(부호를 고려) 떨어진 위치로 입력 
    double dStress=0.0;

    if(dA==0.0 || dIy==0.0 || dIz==0.0)
        return dStress;
    // Compression(-), Tension(+)

    dStress = dFx/dA + (-1.0)*dMy*dz/dIy+ (-1.0)*dMz*dy/dIz;  

    return dStress;
}

double CDgnStlRatingDataCtrl::Cal_Cb(bool bSingleCurve, double dPl, double dPh)
{
    if(dPh==0.0) {return 1.0;}

    double dPl_Ph = dPl/dPh;
    double dCb = 0.0;

    if(bSingleCurve) 
    {
        dCb = 1.75-1.05*(dPl_Ph)+0.3*pow(dPl_Ph,2);
        dCb = min(dCb, 1.75);
    }
    else
    {
        dCb = 1.75+1.05*(dPl_Ph)+0.3*pow(dPl_Ph,2);
        dCb = min(dCb, 2.3);
    }
    return dCb;
}

BOOL CDgnStlRatingDataCtrl::Get_ChkElemFlag4RatingCS454(BOOL bKey, T_SRAS_D& SrasD, int nPosi)
{
    if ( !bKey ) return FALSE;

    return Get_ChkElemFlag4RatingCS454(SrasD, nPosi);
}

BOOL CDgnStlRatingDataCtrl::Get_ChkElemFlag4RatingCS454(T_SRAS_D& SrasD, int nPosi)
{
    // 구조체 교체 필요함
    if ( SrasD.nPos==0 && nPosi==0 )      return TRUE;
    else if ( SrasD.nPos==1 && nPosi==1 ) return TRUE;
    else if ( SrasD.nPos==2 )	          return TRUE;

    return FALSE;
}

double CDgnStlRatingDataCtrl::GetDeadLoadFactor4CS454(const T_ALCS_D& rData)
{
    double dFactor = 1.0;
    for ( INT_PTR i=0; i<rData.arLoadComb.GetSize(); ++i )
    {
        const T_LCOM_BASE& LcomB = rData.arLoadComb[i];
        if ( LcomB.AnalType == D_LCOM_STAGE )
        {
            CString strSgldName = _LS(IDS_DB_SGLD_DL);	// DL.
            T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
            if ( LcomB.LoadCaseKey == SgldK )
            {
                dFactor = LcomB.Factor;
                break;
            }
        }       
    }
    return dFactor;
}

int  CDgnStlRatingDataCtrl::GetIndex4MomentRes(double dMy, int nPos)
{
    int nIdx = (dMy>=0.0) ? (nPos==0 ? 0 : 2) : (nPos==0 ? 1 : 3);
    return nIdx;
}

BOOL CDgnStlRatingDataCtrl::IsPositiveM(double dMy)
{
    return (dMy >= 0.0);
}

BOOL CDgnStlRatingDataCtrl::IsCriticalValue(double dCurV, double dCriV)
{
    if (LT0(dCurV) && LT0(dCriV))
    {
        return MT(dCurV, dCriV);
    }

    return LT(dCurV, dCriV);
}

