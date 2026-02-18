// DgnRUSStlCompDataCtrl.h: interface for the CDgnRUSStlCompDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNRUSStlCompDataCtrl_H__INCLUDED_)
#define AFX_DGNRUSStlCompDataCtrl_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnStruct.h"
#include "CRCDataCtrl.h"
#include "CRCForceCtrl.h"
#include "DgnPscCommon.h"
#include "..\dgnengine\src\DGNe_RUS\DgnCalc_RUS_STL_OD_Struct.h"
#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_SOD_Struct.h"

#include "HeaderPre.h"

class CRUSStlCompGirderDesign;
class CDgnRUSStCompManager;

struct T_STL_COMP_STFN_INX_MAP
{
    CMap<CString,LPCTSTR,int,int&> mStfnIdx;
    void Initialize()
    {
        mStfnIdx.RemoveAll();
    }
    T_STL_COMP_STFN_INX_MAP() {Initialize();}
    T_STL_COMP_STFN_INX_MAP(const T_STL_COMP_STFN_INX_MAP& rData) { *this = rData; }
    T_STL_COMP_STFN_INX_MAP& operator = (const T_STL_COMP_STFN_INX_MAP &rData)
    {
        POSITION Pos = rData.mStfnIdx.GetStartPosition();
        while(Pos)
        {
            CString sStfnName=_T("");
            int nStfnIdx=0;      
            rData.mStfnIdx.GetNextAssoc(Pos, sStfnName, nStfnIdx);

            mStfnIdx.SetAt(sStfnName, nStfnIdx);
        }
        return *this;
    }  
};

struct T_STL_COMP_STFN_INX_D
{
    T_STL_COMP_STFN_INX_MAP StfnTL;
    T_STL_COMP_STFN_INX_MAP StfnTM;
    T_STL_COMP_STFN_INX_MAP StfnTR;
    T_STL_COMP_STFN_INX_MAP StfnWL;
    T_STL_COMP_STFN_INX_MAP StfnWR;
    T_STL_COMP_STFN_INX_MAP StfnBL;
    T_STL_COMP_STFN_INX_MAP StfnBM;
    T_STL_COMP_STFN_INX_MAP StfnBR;

    void Initialize()
    {
        StfnTL.Initialize();
        StfnTM.Initialize();
        StfnTR.Initialize();
        StfnWL.Initialize();
        StfnWR.Initialize();
        StfnBL.Initialize();
        StfnBM.Initialize();
        StfnBR.Initialize();
    }

    T_STL_COMP_STFN_INX_D() {Initialize();}
    T_STL_COMP_STFN_INX_D(const T_STL_COMP_STFN_INX_D& rData) { *this = rData; }
    T_STL_COMP_STFN_INX_D& operator = (const T_STL_COMP_STFN_INX_D &rData)
    {
        StfnTL = rData.StfnTL;
        StfnTM = rData.StfnTM;
        StfnTR = rData.StfnTR;
        StfnWL = rData.StfnWL;
        StfnWR = rData.StfnWR;
        StfnBL = rData.StfnBL;
        StfnBM = rData.StfnBM;
        StfnBR = rData.StfnBR;
        return *this;
    }  
};


struct T_STL_COMP_SECT_STFN_INX
{
    T_STL_COMP_STFN_INX_D Stiff[2]; // 0=I, 1=J

    void Initialize()
    {
        for(int i=0; i<2; ++i) Stiff[i].Initialize();
    }
    T_STL_COMP_SECT_STFN_INX() {Initialize();}
    T_STL_COMP_SECT_STFN_INX(const T_STL_COMP_SECT_STFN_INX& rData) { *this = rData; }
    T_STL_COMP_SECT_STFN_INX& operator = (const T_STL_COMP_SECT_STFN_INX &rData)
    {
        for(int i=0; i<2; ++i) Stiff[i] = rData.Stiff[i];
        return *this;
    }  

};

struct T_FATI_LCOM_K_LIST
{
    T_LCOM_K_LIST aFatiList;

    void Initialize()
    {
        aFatiList.RemoveAll();
    }
    T_FATI_LCOM_K_LIST() {Initialize();}
    T_FATI_LCOM_K_LIST(const T_FATI_LCOM_K_LIST& rData) { *this = rData; }
    T_FATI_LCOM_K_LIST& operator = (const T_FATI_LCOM_K_LIST &rData)
    {
        aFatiList.Copy(rData.aFatiList);
        return *this;
    }  
};

typedef CMap<int, int,  STL_COMP_FORCE_D, STL_COMP_FORCE_D&> Force4Elem;  // DgnLcom

struct ShearConnGForD
{
    Force4Elem For4ElemD;
    void Initialize()
    {
        For4ElemD.RemoveAll();
    }
    ShearConnGForD() {Initialize();}
    ShearConnGForD(const ShearConnGForD& rData) { *this = rData; }
    ShearConnGForD& operator = (const ShearConnGForD &rData)
    {
        POSITION pos = rData.For4ElemD.GetStartPosition();
        while (pos)
        {
            int i = 0;
            STL_COMP_FORCE_D Converter;
            rData.For4ElemD.GetNextAssoc(pos, i, Converter);
            For4ElemD.SetAt(i, Converter);
        }
        return *this;
    }  
};

class __MY_EXT_CLASS__ CDgnRUSStlCompDataCtrl : public CDgnPscCommon
{
public:
    CDgnRUSStlCompDataCtrl();
    virtual ~CDgnRUSStlCompDataCtrl();

    void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);
    BOOL InitialData(int nDgnCode);

    //////////////////////////////////////////////////////////////////////////
    // Execute Design Functions
    //////////////////////////////////////////////////////////////////////////
    // Design
    BOOL ExcuteStlCompGirderDesign();

    // Construction Stage Design
    BOOL Check_ConstructionStage(T_ELEM_K ElemK, int nLcomK4CS, STL_COMP_MEMB_POSD_SNiP &MembInD); 
    BOOL Check_ConstructionNoStage(T_ELEM_K ElemK, int nLcomK4CS, STL_COMP_MEMB_POSD_SNiP &MembInD);

    //////////////////////////////////////////////////////////////////////////
    // Base Data Make Functions
    //////////////////////////////////////////////////////////////////////////

    // Make Design Data
    BOOL Make_DesignBaseData();

    // Make Design Data - Sub
    BOOL Make_ElemSpanMapKey();
    BOOL Make_ElemSpanData();
    BOOL Make_DgnSectD();
    BOOL Make_DgnSectKeyList();
    BOOL Make_Matl2SectD();
    BOOL Make_KappaFactorChkPosData(); // Kappa Factor Check Position
    BOOL Make_FatiChkPosData(); // Fatigue Check Position
    BOOL Make_FloorBeamData();  // Transverse Stiff(floor beam) and global deck stability data
    BOOL Make_AddWebPropData(); // Additional web property data
    BOOL Make_LimitStateGroupParamData(); // Limit State Group Param
    BOOL Make_DesignPosData(); // Design Position Data for Sub Part (Top , Bot , Web, Slab, Rebar...)
    BOOL Make_ShearConnectorGroupData(); // Shear Connector Group Data
    BOOL Make_ShearConnectorGroupData_Unit(const T_SCGD_D& crScgdD, STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP& rData);
    BOOL Make_ConvertSectD4ShearConnGroup(T_SECT_K SectK, T_ELEM_K ElemK, STL_COMP_SECT_ELEMD_SNiP& rData, double dCCF_1st, double dCCF_2nd);
    BOOL Make_ShearConnGroupForceD(const STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP& crShearConnInD, CArray<int, int>& aDgnLcomList);

    // Make or Get Load Combination Data Functions
    BOOL GetDeadLoadCSMaxFactor(int &nLcomK, double &dFactor);
    BOOL Get_DgnLcomKeyList4ShearConnDesign(const ADGNFORCE& craDgnForceLcom, T_LCOM_K_LIST& aStrnLcomKList);
    BOOL Get_DgnLcomKeyList4StlCompDesign(const ADGNFORCE& craDgnForceLcom,  T_LCOM_K_LIST& aKapaChkLcomKList, T_LCOM_K_LIST& aStrnLcomKList,  T_LCOM_K_LIST& aFatigueOrtLcomKList,  
        CArray<T_FATI_LCOM_K_LIST, T_FATI_LCOM_K_LIST&>& aFatigueLcomKList, T_LCOM_K_LIST& aServLcomKeyList);
    BOOL Get_LocalForceD(T_ELEM_K ElemK, int nIJ, int nLcomK, STL_COMP_LFORCE_SNIP &rLForce, BOOL bIsAfter = TRUE);
    BOOL Get_RebarData(T_ELEM_K ElemK, BOOL bPosIi, T_SECT_D& SectD, STL_COMP_SECT_POSD& rData);
    BOOL Get_StagName(int nStageK, CStringW& strStageName);
    BOOL Get_LcomName(int nLcom, CStringW& strLcomName);

    //
    void MakeSectStfnIndexInfo(int nStype, T_SECT_SECTBASE_D *pSectD, T_STL_COMP_STFN_INX_D &rData);
    void MakeSectStfnPropileIndex(int nStype, T_SECT_SECTBASE_D *pSectD, T_SECT_SOD_STIFF_SUB_D &PartStfn, T_STL_COMP_STFN_INX_D &rData);  

    //////////////////////////////////////////////////////////////////////////
    //  Get Design Base Data Functions
    //////////////////////////////////////////////////////////////////////////
    BOOL Get_StlCompUlmSection(T_ELEM_K ElemI, T_ELEM_K ElemM, T_ELEM_K ElemJ, int nPosI, int nPosM, int nPosJ, STL_COMP_ULMS_D &rData);
    BOOL GetDgnBaseD4Elem_IJ(T_ELEM_K ElemK, const T_ELEM_D& crElemD, int nPosIJ, STL_COMP_MEMB_POSD_SNiP& rData);

    BOOL GetDgnLengD();
    BOOL GetDgnLengDFromMemb(T_ELEM_K ElemK, const T_MEMB_D &MembD, T_EDLK_D &DgnLengD);
    BOOL GetDgnLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, T_EDLK_D &DgnLengD);
    BOOL GetDgnSuptLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, double &dSpanLength);
    BOOL GetDgnSuptTypeDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, int nSpanEndSupType1YZ[2], int nSpanEndSupType2YZ[2]);
    BOOL GetDgnElemLocDInSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, const double dSpanLength, const int nStartElemIndex, const int nEndElemIndex, int nElemLocaD[2]); // I & J
    BOOL Get_UnbracedLengthData(T_ELEM_K ElemK, BOOL bKfac, T_KFAC_D &KFacD, STL_MEMB_ULEN &rData);
    BOOL Get_UnbracedLengthData(T_ELEM_K ElemK, STL_MEMB_ULEN &rData, BOOL &bUnLeng, BOOL &bEffLengF);

    //////////////////////////////////////////////////////////////////////////
    // Get Critical Result Functions
    //////////////////////////////////////////////////////////////////////////
    // Before Strength
    void GetCrFlgStrnRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNRibRat, double dBFSNRibRat, double &dTFlgSNRibMaxR, double &dBFlgSNRibMaxR, T_SDCR_BASE &rTRib, T_SDCR_BASE &rBRib);
    void GetCrFlgStrnDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNDeckRat, double dBFSNDeckRat, double &dTFlgSNDeckMaxR, double &dBFlgSNDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck);
    void GetCrFlgStrnFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNFBeamRat, double dBFSNFBeamRat, double &dTFlgSNFBeamMaxR, double &dBFlgSNFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam);
    void GetCrWebStrnRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNRibRat, double &dWebSNRibMaxR, T_SDCR_BASE &rWRib);
    void GetCrWebStrnDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNDeckRat, double &dWebSNDeckMaxR, T_SDCR_BASE &rWDeck);
    //  Before Stability
    void GetCrFlgStabRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBRibRat, double dBFSBRibRat, double &dTFlgSBRibMaxR, double &dBFlgSBRibMaxR, T_SDCR_BASE &rTRib,  T_SDCR_BASE &rBRib);
    void GetCrFlgStabDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBDeckRat, double dBFSBDeckRat, double &dTFlgSBDeckMaxR, double &dBFlgSBDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck);
    void GetCrFlgStabFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBFBeamRat, double dBFSBFBeamRat, double &dTFlgSBFBeamMaxR, double &dBFlgSBFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam);
    void GetCrWebStabRibRatio  (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBRibRat, double &dWebSBRibMaxR, T_SDCR_BASE &rWRib);
    void GetCrWebStabDeckRatio (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBDeckRat, double &dWebSBDeckMaxR, T_SDCR_BASE &rWDeck);
    void GetCrGDStabRatio      (int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dStbGDRat,     double &dSBGDMaxR,      T_SDCR_BASE &rGDCr);

    // After Strength
    void Get_AFCr_PlasticLimitRatio(int nPlimCt, const T_PLASTIC_LIM_BASE& LcomKeyD, double dPlasticLimitRat, double& dPlasticLimitRatMax, T_PLASTIC_LIM_BASE& rPlasticLim);

    void Get_AFCrStrnConcreteRatio  (int nStrnCt, const T_ACST_CONC_BASE &LcomKeyD, double dConcRat,  double &dConcMaxRat,T_ACST_CONC_BASE &rConc);
    void Get_AFCrStrnRebarRatio  (int nStrnCt, const T_ACST_REBAR_BASE &LcomKeyD, double dRbarRat, double &dRbarMaxRat, T_ACST_REBAR_BASE &rRebar);
    void Get_AFCrStrnStlGirderRatio  (int nStrnCt, const T_ACST_SGFR_BASE &LcomKeyD, double dTFlgRat, double dBFlgRat, double &dTFlgMaxRat, double &dBFlgMaxRat, 
        T_ACST_SGFR_BASE &rTDeck, T_ACST_SGFR_BASE &rBDeck);

    // After Fatigue
    void Get_AFCr_Fati_ConcreteRatio(int nFatiCt, const T_FATI_CONC_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR, T_FATI_CONC_BASE &rData);
    void Get_AFCr_Fati_RebarRatio(int nFatiCt, const T_FATI_REBAR_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR, T_FATI_REBAR_BASE &rData);
    void Get_AFCr_Fati_StlGirderRatio(int nFatiCt, const T_FATI_STL_GIRDER_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR, T_FATI_STL_GIRDER_BASE &rData);

    // After Serviceability
    void Get_AFCr_ServRatio(int nServCt, const T_CRACK_CONC_BASE &LcomKeyD, double dServRat, double &dServMaxR, T_CRACK_CONC_BASE &rData);

    // Shear Connector
    void Get_AFCr_ShearConnectorRatio(int nShearConnCt, const T_STRD_SHEAR_CONN_BASE &LcomKeyD, double dGRat, double &dGMaxR, T_STRD_SHEAR_CONN_BASE &rData);


    //////////////////////////////////////////////////////////////////////////
    // Convert Functions (Civil to Design Engine)
    //////////////////////////////////////////////////////////////////////////

    // Convert Functions - General Data
    void Convert_StlComp_SectData(int nPos, T_SECT_D *pSectD, T_STPO_D &StpoD, T_STL_COMP_STFN_INX_D &rStfnId, T_SECT_STIFFNESS SpscSect[5],  STL_COMP_SECT_POSD &rData);
    void Convert_StlComp_SectBaseData(int nStype, T_SECT_SECTBASE_D *pSectAfter, T_SECT_SECTBASE_D *pSectBefore,T_STPO_D &StpoD, T_STL_COMP_STFN_INX_D &rStfnId, STL_COMP_SECT_INFO &rData);
    void Convert_StlComp_SectSize(int nStype,  T_SECT_SECTBASE_D *pSectBefore,T_SECT_SECTBASE_D *pSectAfter, STL_COMP_SECT_INFO &rData);	
    void Convert_SectStiffnessData(const T_SECT_STIFFNESS &crStiff, DGN_SECT_STIFFNESS &rData);
    void Convert_RebarData(_DGN_RBAR_CRC& rInD, DGN_RBAR_RC& rData);

    void Convert_Stl_Comp_StiffenerData(int nStype, T_SECT_SECTBASE_D *pSectD, T_STPO_D &StpoD, T_SECT_SOD_STIFF_D *pStfn, ST_SECT_STIFF_D &rData);
    void Convert_StlComp_StfnShapeData(T_STIFF_SHAPE_D *pShape, ST_STIFF_SHAPE_D &rData);
    void Convert_StlComp_StfnProfileData(int nStype, T_SECT_SECTBASE_D *pSectD, T_STPO_D &StpoD, T_SECT_SOD_STIFF_SUB_D &PartStfn, ST_SECT_STIFF_D &rData);
    void Convert_StlComp_StfnProfileData(int nStype, T_STPO_D &StpoD, double dWidth, T_SECT_SOD_STIFF_SUB_D &PartStfn, CArray<ST_STIFF_PROFILE_D,ST_STIFF_PROFILE_D&> &arData);
    void Convert_StlComp_StfnProfileData(T_STPO_D &StpoD, int nRefPos, int nDeckPos, int nDeckPart, double dWidth, T_SECT_SOD_STIFF_DATA_D *pStfnProf, ST_STIFF_PROFILE_D &rData);

    BOOL ConvertStlCompSectPolygonData(int nPos, T_SECT_D *pSectD, ST_SECT_SECTBASE_D &rData);

    // Convert Force Data
    BOOL Convert_Lcom(T_ELEM_K ElemK, INT_PTR nPart, INT_PTR nLcomOrgK);
    void ConvertStlCompCalcUlmCSForce(int nPosI, int nPosM, int nPosJ, _DGN_FORC_CRC &ForI, _DGN_FORC_CRC &ForM, _DGN_FORC_CRC &ForJ, STL_COMP_ULMF_D &rData);
    void ConvertStlCompCalcForce4CSStage(int nPos, _DGN_FORC_CRC &For, STL_COMP_FORCE_D &rData);

    void ConvertStlCompCalcForce(int nPos, _DGN_FORC_CRC &ForBefore, _DGN_FORC_CRC &ForAfter, _DGN_FORC_CRC &ForAfter_DL,  _DGN_FORC_CRC &ForD_Crp,  
        _DGN_FORC_CRC &ForD_Shr,  _DGN_FORC_CRC &ForD_BST,  _DGN_SCFP_PART_LCASE &Str_Crp , _DGN_SCFP_PART_LCASE &Str_Shr,
        _DGN_SCFP_PART_LCASE &Str_BST , STL_COMP_FORCE_D &rData);
    void ConvertStlCompCalcForceUnit(int nPos, _DGN_FORC_CRC &For, STL_FORCE_D &rData);
    void ConvertStlCompCalcForceUnit(int nPos, _DGN_FORC_CRC &For, T_CSGD_FORCE &rData);
    void ConvertStlCompCalcStressUnit(int nPos, _DGN_SCFP_PART_LCASE &For, STL_COMP_INTERNAL_STRESS_2D &rData);
    void ConvertStlCompCalcStressUnit(int nPos, _DGN_SCFP_PART_LCASE &For, T_SCGD_INT_STRESS &rData);
    void ConvertStlCompForce(int nPos, _DGN_FORC_CRC &For, T_CSGD_FORCE &rData);
    void ConvertStlCompForce4SaveD(int nPos, _DGN_FORC_CRC &ForBefore, _DGN_FORC_CRC &ForAfter, _DGN_FORC_CRC &ForAfter_DL,  _DGN_FORC_CRC &ForD_Crp,  
        _DGN_FORC_CRC &ForD_Shr,  _DGN_FORC_CRC &ForD_BST,  _DGN_SCFP_PART_LCASE &Str_Crp , _DGN_SCFP_PART_LCASE &Str_Shr,
        _DGN_SCFP_PART_LCASE &Str_BST ,T_SCGD_DGN_FORCE& rData);


    // Convert  Functions- Design Data
    BOOL Convert_GlobalDgnParamD(STL_COMP_GLOBAL_D &rData);
    BOOL Convert_Stl_CompDgnLengthData(const T_EDLK_D &DgnLengD, STL_COMP_RES_BEFORE_BASE_SNiP &rData);
    BOOL Convert_Stl_CompDgnLengthData(const T_EDLK_D &DgnLengD, STL_COMP_RES_AFTER_BASE_SNiP &rData);

    // Material Converting
    BOOL Convert_Sect2MatlD(T_MATD_D& MatD, STL_MATL_SNiP_PROP& rData);
    BOOL Convert_Stl2StlCompMatl(const T_MATD_D& crMatlD,const SLT_MATL_SNiP_POSD& crStlMatlD, STL_COMP_MAT_SNiP& rStlCompD);
    BOOL Convert_Stl2StlCompMatl(const T_MATD_D& crMatlD, STL_COMP_MAT_SNiP& rStlCompD);
    void Convert_MatlStlPart(const T_MATL_ANALYSIS &AnalMatl, const T_MATL_DESIGN &DgnMatl, STL_MATL_SNiP &rData);

    // Limit Plastic strain Converting
    void ConvertStlCompKappaFactor(T_SDPS_SUB_D &InD, STL_KAPPA_INPUT_D &rData);
    void ConvertStlCompKappaFactor(const STL_KAPPA_INPUT_D& crInD, STL_COMP_KAPPA_INPUT_D& rData);
    void ConvertStlCompKappaFactorChkPos(T_SDPS_SUB_D &InD, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData);
    void ConvertStlCompKappaFactorChkPosOne(T_POINT_SSM &InD, STL_MEMB_POSD_SNiP_CHK_KAPPA &rData);
    void ConvertStlCompKappaFactorChkPosDefault(CArray<T_POINT_SSM, T_POINT_SSM&>& aStressPoint, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData);

    // Fatigue Check Position Converting
    void ConvertStlCompFatiCheckPos(int nPos, T_SDFC_D &InD, STL_COMP_FATI_CHK_POS_DGND_SNiP &rData, double dDelta_y = 0.0, double dDelta_z = 0.0); 
    void ConvertStlCompFatiCheckPosOne(T_SDFC_SUB_D &InD, STL_COMP_MEMB_POSD_SNiP_FAT &rData, double dDelta_y = 0.0, double dDelta_z = 0.0);      // Fatigue Check Position  for steel girder and shear connector

    // Floor Beam Data Converting
    void ConvertStlCompFloorBeam(int nPos, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D &rData);
    void ConvertStlCompFloorBeamPart(int nPos, CString &strFBPartName, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_D &rData);

    // Additional Girder Web Prop. Converting
    void ConvertStlCompWebVerticalForce(int nStype, int nPos, T_SDAP_D &InD, STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM &rForWeb, STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM &rForWebP, BOOL bBeforeComp = FALSE); 
    void ConvertStlCompWebPlateForce(int nStype, double dBeta, int nConnectType, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_PLATE_CHKUNIT_D &rData); 
    void CovertStlCompWebPlateForceBase(T_SDAP_PROP_D &InD, STL_LFORCE_WEB_PLATE_D &rData);
    void ConvertStlCompWebVerticalLcomForce(int nStype, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_SIGY_CHKUNIT_D &rData);
    void ConvertStlCompWebVerticalForceBase(int nWeb, T_SDAP_VERT_D &InD, STL_LFORCE_WEB_SIGY_D &rData);

    // Limit State Group 2 Param Data Converting
    void ConvertStlCompLimitGroupParam(const T_SERV_D&  crServD,  STL_COMP_LIMIT_CRACK_STATE_INPUT_D &rData);

    // Design Sub Position Data Converting
    BOOL ConvertStlCompSectCheckPart(int nStype, int nPos, T_SDDP_D &InD, STL_COMP_CHK_POSD_SNiP &rData);

    // Shear Connector Data Converting
    void ConvertStlCompShearConnector(const T_SCGD_D&  crScgdD,  STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP &rData);

    // Convert Result Data - Before Composite
    void Convert_StlComp_BF_Str_Flg(int nStype, BOOL bIsTop, STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &DgnResD, T_SDSN_FLG &rData, BOOL &bStlCompCheck);
    void Convert_StlComp_BF_Str_Web(int nStype, STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &DgnResD, T_SDSN_WEB &rData, BOOL &bStlCompCheck);
    void Convert_StlComp_BF_Sta_Flg(int nStype, BOOL bIsTop, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &DgnResD, T_SDSB_FLG &rData, BOOL &bStlCompCheck);
    void Convert_StlComp_BF_Sta_Web(int nStype,  STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &DgnResD, T_SDSB_WEB &rData, BOOL &bStlCompCheck);
    void Convert_StlComp_BF_Sta_GD(int nStype, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &DgnResD, T_SDSB_GD &rData, BOOL &bStlCompCheck);

    void Convert_BF_Str_FlgPartEngToCVL(STL_MEMB_RES_SNiP_STR_DP_CHK_UNIT &StrnPartR, T_SDPT_STRN_FLG &rData);
    void Convert_StrnLRibEngToCVL(STL_MEMB_RES_SNiP_STR_RIB &RibR, STL_MEMB_RES_SNiP_STR_DP &DeckR, T_SDLS_STRN_FLG &rData);
    void Convert_BF_Str_WebPartEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_CHK_UNIT &StrnWebR, T_SDPT_STRN_WEB &rData);
    void Convert_StrnWebPlateEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_RIB &WebP, T_SDPS_STRN_WEB &rData);
    void Convert_StabFlgPartEngToCVL(STL_MEMB_RES_SNiP_STA_DP_CHK_UNIT &StabPartR, T_SDPT_STAB_FLG &rData);
    void Convert_StabLRibEngToCVL(STL_MEMB_RES_SNiP_STA_RIB_CHK_UNIT &RibR, T_SDSB_SECT_NOLS &rData, int &nRibPart_cr, double &dRat_Rib_Temp, BOOL &bChk_Rib);
    void Convert_StabDeckEngToCVL(STL_MEMB_RES_SNiP_STA_LOCAL_BTR &DeckR, T_SDSB_PAN &rData);
    void Convert_StabWebPartEngToCVL(STL_MEMB_RES_SNiP_STA_WEB &StabWebR, T_SDPT_STAB_WEB &rData);
    void Convert_StabWebPanEngToCVL(STL_MEMB_RES_SNiP_STA_WEB_PLATE &WebP, T_SDPS_STAB_WEB_PAN &rData);

    // Convert Result Data - After Composite

    void Convert_StlComp_AF_Str_Concrete(STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_ACST_CONC &rData, BOOL &bStlCompCheck);
    void Convert_StlComp_AF_Str_Rebar(STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_ACST_REBAR &rData, BOOL &bStlCompCheck);
    void Convert_StlComp_AF_Str_Flg(int nStype, BOOL bIsTop, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_ACST_SGFR_PART &rData, BOOL &bStlCompCheck);
    void Convert_StlComp_AF_Str_Web(int nStype, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &DgnResD, T_SDSN_WEB &rData, BOOL &bStlCompCheck);

    void Convert_StlComp_Fati_Conc(STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &DgnResD,T_FATI_CONC&rData, BOOL &bStlCompChecks);
    void Convert_StlComp_Fati_Rebar(STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &DgnResD, T_FATI_REBAR &rData, BOOL &bStlCompChecks);
    void Convert_StlComp_Fati_StlGirder(STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &DgnResD, T_FATI_STL_GIRDER &rData, BOOL &bStlCompChecks);

    void Convert_StlComp_Serv_Conc(STL_COMP_MEMB_RESD_SNiP_AFTER_SERV &DgnResD,T_CRACK_CONC&rData, BOOL &bStlCompChecks);

    void Convert_ShearConnResD(STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN& rInD, T_STRD_SHEAR_CONN& rData, BOOL &bStlCompChecks);

    void Convert_AF_StrnFlgPartEngToCVL(STL_COMP_MEMB_RES_SNiP_AFTER_STR_DP_CHK_UNIT &StrnPartR, STL_COMP_FLXURE_CHK_CASE_SNiP_AFTER_STR& CaseTypeD, T_ACST_SGFR_ELEM &rData);
    void Convert_AF_StrnFlgEngToCVL(STL_COMP_STL_GIRDER_RESD_SNiP_AFTER_STR &RibR,  STL_COMP_FLXURE_CHK_CASE_SNiP_AFTER_STR& CaseTypeD, T_ACST_SGFR &rData);
    void Convert_AF_StrnLRibEngToCVL(STL_MEMB_RES_SNiP_STR_RIB &RibR, T_SDLS_STRN_FLG &rData);
    void Convert_AF_StrnWebPartEngToCVL(STL_COMP_MEMB_RES_SNiP_STR_WEB_CHK_UNIT& DgnResD,T_SDPT_STRN_WEB& rData);

    // Is..?
    BOOL IsNeedBeforeChk(const STL_COMP_CHK_POSD_SNiP& crDgnSubPosD);
    BOOL IsReplaceStfnIndex(int nStype, int nRefPos, int nDeckPos, int nDeckPart);
    BOOL IsLcomExist(T_LCOM_K LcomK, CArray<T_LCOM_K, T_LCOM_K&> & aLcomList);
    BOOL IsValidSectType(T_ELEM_K ElemK);
    BOOL IsNAinSteelGirder(const STL_COMP_SECT_POSD& crSectD);
    BOOL IsExistScfp4Lcom(int nLcomType, int nOrgLcom, bool& bExisCrp, bool& bExisShr, bool& bExisBST);

    // Sub Calculation
    BOOL CalcStiffnessOfStiffener(T_SECT_STIFF_D &Stfn, ST_SECT_STIFF_D &rData);

    //////////////////////////////////////////////////////////////////////////
    //  Execute Report
    //////////////////////////////////////////////////////////////////////////
    BOOL PrintStlCompGirderReport();
    CString GetSaveFileNameEx(BOOL bWithoutExtension  = FALSE);

    // Get Before Composite Check Result
    BOOL GetRptD_BeforeResD(T_ELEM_K ElemK,  int &nChkPosIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD);
    BOOL GetRptD_BeforeStrResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &pMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &pMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD);
    BOOL GetRptD_BeforeStrFlgResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD);
    BOOL GetRptD_BeforeStrWebResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &rDgnStrResD);
    BOOL GetRptD_BeforeStaResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &pMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &pMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD);
    BOOL GetRptD_BeforeStaLTBResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &pMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &pMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD);
    BOOL GetRptD_BeforeStaFlgResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &pMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &pMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD);
    BOOL GetRptD_BeforeStaWebResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &pMemb,  STL_COMP_RES_BEFORE_BASE_SNiP &pMembBase,
        STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD);
    BOOL GetRptD_KappaChkResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD);
    // Get After Composite Check Result
    BOOL GetRptD_AfterResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &pMemb,  STL_COMP_RES_AFTER_BASE_SNiP &pMembBase,
        STL_COMP_RPT_ELEM_RES_UNIT &rDgnResD);
    BOOL GetRptD_AfterStrResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnResD);
    BOOL GetRptD_AfterStrConcResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD);
    BOOL GetRptD_AfterStrRebarResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD,  STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD);
    BOOL GetRptD_AfterStrFlgResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD);
    BOOL GetRptD_AfterStrWebResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &rDgnStrResD);

    BOOL GetRptD_AfterStaResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnResD);
    BOOL GetRptD_AfterStaFlgResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD,  STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD);
    BOOL GetRptD_AfterStaWebResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD,  STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &rDgnStaResD);

    BOOL GetRptD_AfterFaitResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD);
    BOOL GetRptD_AfterFaitConcResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD);
    BOOL GetRptD_AfterFaitRebarResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD);
    BOOL GetRptD_AfterFaitStlGirderResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &rDgnResD);
    BOOL GetRptD_AfterServResD(T_ELEM_K ElemK,  int &nIJ, STL_COMP_MEMB_POSD_SNiP &rMemb,  STL_COMP_RES_AFTER_BASE_SNiP &rMembBase,
        STL_COMP_MEMB_RESD_SNiP_KAPPA &rDgnKappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_SERV &rDgnResD);
    BOOL GetRptD_ShearConnGroupResD(T_SCGD_K ScgdK, STL_COMP_MEMB_POSD_SNiP& rMembD, STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP& rShearConnBaseD,  
        STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN& rDgnResD);

    // Get Before Composite Check Result - Sub
    BOOL Get_RptForceD_BeforeStrRes_Flg(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD, 
        T_SDPT_STRN_FLG &SDSN_Result);   

    BOOL Get_RptForceD_BeforeStrRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDPT_STRN_WEB &SDSN_Result);

    BOOL Get_RptForceD_BeforeStaRes_LTB(T_ELEM_K ElemKey, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDSB_GD &SDSB_GDR);
    BOOL Set_RptForceD_BeforeStaRes_Flange(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDPT_STAB_FLG &SDSB_Result); 
    BOOL Set_RptForceD_BeforeStaRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDPT_STAB_WEB &SDSB_Result);  

    // Get After Composite Check Result - Sub
    BOOL Get_RptForceD_AfterStrRes_Conc(T_ELEM_K ElemKey, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_ACST_CONC &rSTR_Conc);
    BOOL Get_RptForceD_AfterStrRes_Rebar(T_ELEM_K ElemKey, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_ACST_REBAR &rSTR_Rbar);
    BOOL Get_RptForceD_AfterStrRes_Flg(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD, 
        T_ACST_SGFR_ELEM &rStr_Flg_Result);   
    BOOL Get_RptForceD_AfterStrRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDPT_STRN_WEB &SDSN_Result);

    BOOL Get_RptForceD_AfterStaRes_Flange(T_ELEM_K ElemK, int nIJ, BOOL bTop, int nFlangePart, int nDeckRibFB, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDPT_STAB_FLG &SDSB_Result); 
    BOOL Set_RptForceD_AfterStaRes_Web(T_ELEM_K ElemK, int nIJ, BOOL bLeft, int nWebChkType, STL_COMP_MEMB_POSD_SNiP &MembLcomD, T_SDPT_STAB_WEB &SDSB_Result);  

    BOOL Get_RptForceD_AfterFatiRes_Conc(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD);
    BOOL Get_RptForceD_AfterFatiRes_Rebar(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD);
    BOOL Get_RptForceD_AfterFatiRes_StlGirder(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD);

    BOOL Get_RptForceD_AfterServRes_Conc(T_ELEM_K ElemK, int nIJ, STL_COMP_MEMB_POSD_SNiP &MembLcomD,  T_CRACK_CONC &rSTR_Conc);

    BOOL GetRptForceD_ShearConnGroupResD(T_SCGD_K ScgdK, STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP& rShearConnInD, SHEAR_CONN_GROUP_FORCE_SNIP& rShearConnGForceD);

    CString Get_LcomType(int nConcurrent);

    // Report Convert
    void ConvertStlCompForce2Engine(T_ELEM_K ElemK, int nIJ,  int nChkPos, int nDgnLcomK, STL_COMP_MEMB_POSD_SNiP &MembLcomD, BOOL bIsAfter = TRUE);
    void ConvertStlCompGForce2Engine(T_SCGD_DGN_FORCE_UNIT &Elem, STL_COMP_FORCE_D &ChkForD);



public:
    CRUSStlCompGirderDesign *m_pStlCompDesign;
    CDgnRUSStCompManager *m_pStlCompManager;
    int m_nDgnCode;

    CArray<T_ELEM_K,T_ELEM_K> m_arElemK;
    CMap<T_SECT_K,T_SECT_K, T_SECT_K, T_SECT_K> m_mChkSectKey;
    CMap<T_SECT_K,T_SECT_K,T_STL_COMP_SECT_STFN_INX,T_STL_COMP_SECT_STFN_INX&> m_SectStfnIdx;
    CMap<T_ELEM_K,T_ELEM_K, T_SECT_K, T_SECT_K&>      m_mElemKtoDgnSectK;  // ElemK,Serial DgnSectK, Effective Width가 적용되면, Elememt 별로 다름
    CMap<T_SECT_K,T_SECT_K, STL_COMP_SECT_ELEMD_SNiP, STL_COMP_SECT_ELEMD_SNiP&>      m_mStlCompSect;     // Serial DgnSectK
    CMap<T_ELEM_K,T_ELEM_K, STL_COMP_SECT_ELEMD_SNiP, STL_COMP_SECT_ELEMD_SNiP&>      m_mStlCompSectElem; // Tapered Section Group 경우 요소별 사용하기 위해 

    // Design Input Data Storage
    CMap<T_ELEM_K, T_ELEM_K, STL_COMP_MAT_SNiP, STL_COMP_MAT_SNiP&>  m_mStlCompMatl2Sect;
    CMap<T_ELEM_K,T_ELEM_K,STL_KAPPA_INPUT_D,STL_KAPPA_INPUT_D&>    m_mStlCompKappaFactor_I;
    CMap<T_ELEM_K,T_ELEM_K,STL_KAPPA_INPUT_D,STL_KAPPA_INPUT_D&>    m_mStlCompKappaFactor_J;
    CMap<T_ELEM_K,T_ELEM_K,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD&>     m_mStlCompKappaChkPos_I;
    CMap<T_ELEM_K,T_ELEM_K,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD,STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD&>     m_mStlCompKappaChkPos_J;
    CMap<T_ELEM_K,T_ELEM_K,STL_COMP_FATI_CHK_POS_DGND_SNiP,STL_COMP_FATI_CHK_POS_DGND_SNiP&>    m_mStlCompFatiChkPos_I;
    CMap<T_ELEM_K,T_ELEM_K,STL_COMP_FATI_CHK_POS_DGND_SNiP,STL_COMP_FATI_CHK_POS_DGND_SNiP&>    m_mStlCompFatiChkPos_J;
    CMap<T_ELEM_K,T_ELEM_K,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D&> m_mStlCompFloorBeam_I;
    CMap<T_ELEM_K,T_ELEM_K,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D,STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D&> m_mStlCompFloorBeam_J;
    CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM&>   m_mStlCompLForWeb_I[2]; // 0 : Before Composite,  1 : After Composite
    CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM,STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM&>   m_mStlCompLForWeb_J[2]; // 0 : Before Composite,  1 : After Composite
    CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM&> m_mStlCompLForWebPlate_I[2]; // 0 : Before Composite,  1 : After Composite
    CMap<T_ELEM_K,T_ELEM_K,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM,STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM&> m_mStlCompLForWebPlate_J[2]; // 0 : Before Composite,  1 : After Composite
    CMap<T_ELEM_K,T_ELEM_K,STL_COMP_LIMIT_CRACK_STATE_INPUT_D,STL_COMP_LIMIT_CRACK_STATE_INPUT_D&>     m_mStlCompLimitGroupParam;  //  I & J  구분 없음
    CMap<T_ELEM_K,T_ELEM_K,STL_COMP_CHK_POSD_SNiP,STL_COMP_CHK_POSD_SNiP&>     m_mStlCompDgnPosD_I;
    CMap<T_ELEM_K,T_ELEM_K,STL_COMP_CHK_POSD_SNiP,STL_COMP_CHK_POSD_SNiP&>     m_mStlCompDgnPosD_J;  
    CMap<T_SCGD_K,T_SCGD_K,STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP,STL_COMP_SHEAR_CONN_INPUT_DATA_SNiP&>     m_mStlCompShearConnectorD;  
    CMap<int, int, SHEAR_CONN_GROUP_FORCE_SNIP, SHEAR_CONN_GROUP_FORCE_SNIP&> m_mShearConnGroupForceD; // T_LCOM_K (DgnLcom) , DgnLcom별 Shear Connector Data
    CMap<CString,LPCTSTR,int,int&> m_mStfnNameIndex;


    // Design Length Info.
    CMap<T_ELEM_K, T_ELEM_K, T_EDLK_D, T_EDLK_D&> m_mDgnLengD;

    // Span Info.
    CMap<T_ELEM_K,T_ELEM_K, T_SPAN_K,T_SPAN_K&> m_mElemSpanK; 
    CMap<T_ELEM_K,T_ELEM_K, T_SPAN_BASE,T_SPAN_BASE&> m_mElemSpanD; 

protected:
    T_RSCD_ITEM m_ChkItemD;

private:
    CDBDoc* m_pDoc;
    CCRCDataCtrl*  m_pDataCtrl;
    CCRCForceCtrl* m_pForcCtrl;

};


#include "HeaderPost.h"

#endif // !defined(AFX_DGNRUSStlCompDataCtrl_H__INCLUDED_)