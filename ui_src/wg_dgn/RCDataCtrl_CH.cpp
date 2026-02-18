// RCDataCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "RCDataCtrl_CH.h"
#include <math.h>
#include "DgnPscCommon.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\TendonGrup.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DBCodeDef.h"

#include "DgnMomentCurvatureDesign.h"
#include "DgnToolMPhi_SeismicDesign.h"
#include "Dgn_RCManager.h"
#include "CH_DGN_INTERFACE_UNICODE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRCDataCtrl_CH::CRCDataCtrl_CH(){
    m_dZero=1.0E-07;
	m_dPi=4.*atan(1.);
    m_pDoc = CDBDoc::GetDocPoint();
    ASSERT(m_pDoc);
    m_chrp.Initialize();
//   m_ForceCtrl->Set_LcomDataForDesign();
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
    Set_UnitIndex(CodeUnit);	
    // 
    m_pDesignForceResult  = new CDesignForceResult(&m_DataCtrl);

    m_pDoc->m_pAttrCtrl->GetChrp(m_chrp);
}

CRCDataCtrl_CH::~CRCDataCtrl_CH(){
    if(!m_pDesignForceResult)
    {
        delete m_pDesignForceResult;
        m_pDesignForceResult = NULL;
    }
}

void CRCDataCtrl_CH::Initialize(){  
	m_DataCtrl.Initialize();
	m_DataCtrl.Cal_LcomDataForDesignAll();	
	CCRCForceCtrl* pForceCtrl = m_DataCtrl.Get_ForceCtrlPointer();
	m_ForceCtrl = pForceCtrl;
}

/************************************************************************/
/* get RC Design parameters.                                            */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_Rcd(T_RC_D_CH& rcd){
    rcd.Initialize();
    
    rcd.iDimType = m_chrp.iDimType;
	rcd.iDgnCode = m_chrp.iDgnCode;
    if (rcd.iDgnCode == RC_JTG_D62_04) {
        if (m_chrp.iSafeType == 0 ) rcd.iSafeType = 1.1;
        if (m_chrp.iSafeType == 1 ) rcd.iSafeType = 1.0;
        if (m_chrp.iSafeType == 2 ) rcd.iSafeType = 0.9;
        if (m_chrp.iCrackCode == 0) rcd.iCrackCode = 0.2;
        if (m_chrp.iCrackCode == 1) rcd.iCrackCode = 0.15;
    }else if (rcd.iDgnCode == RC_TB_10002_3_05) {
        if (m_chrp.iCrackCode == 0) rcd.iCrackCode = 0.25;
        if (m_chrp.iCrackCode == 1) rcd.iCrackCode = 0.20;
        if (m_chrp.iCrackCode == 2) rcd.iCrackCode = 0.25;
        if (m_chrp.iCrackCode == 3) rcd.iCrackCode = 0.20;
        if (m_chrp.iCrackCode == 4) rcd.iCrackCode = 0.20;
        if (m_chrp.iCrackCode == 5) rcd.iCrackCode = 0.15;
    }
    else if (rcd.iDgnCode == RC_CJJ_11_2011) {
        if(m_chrp.bExistSpecialLoad)
        {
            if (m_chrp.iSafeType == 0 ) rcd.iSafeType = 1.0;
            if (m_chrp.iSafeType == 1 ) rcd.iSafeType = 1.0;
            if (m_chrp.iSafeType == 2 ) rcd.iSafeType = 1.0;
            if (m_chrp.iCrackCode == 0) rcd.iCrackCode = 0.25;
            if (m_chrp.iCrackCode == 1) rcd.iCrackCode = 0.15;
        }
        else
        {
            if (m_chrp.iSafeType == 0 ) rcd.iSafeType = 1.1;
            if (m_chrp.iSafeType == 1 ) rcd.iSafeType = 1.0;
            if (m_chrp.iSafeType == 2 ) rcd.iSafeType = 0.9;
            if (m_chrp.iCrackCode == 0) rcd.iCrackCode = 0.2;
            if (m_chrp.iCrackCode == 1) rcd.iCrackCode = 0.15;
        }
    }
    else
    {

    }
	rcd.iCrackType = m_chrp.iCrackType;
	for (int i=0; i<12; i++) {
		rcd.bDesignPrint[i] = m_chrp.bDesignPrint[i];
	}
    return TRUE;
}

/************************************************************************/
/* get element's material (concrete, rebars)                            */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_Matl(int Elemi,_DGN_RC_MATL_CH_W& matl)
{
    _DGN_RC_MATL_CH matlA;
    if(!Get_Matl(Elemi, matlA))
        return FALSE;
    matl = WA_Convert::Convert_A2W(matlA);
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_Matl(int Elemi,_DGN_RC_MATL_CH& matl){
    matl.Initialize();
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(Elemi,ElemD))	{ASSERT(0); return FALSE;}
	// Get Matl Data.
	T_MATD_D MatdD;	MatdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat,MatdD))	{ASSERT(0); return FALSE;}
	double dfck=0.0, dftk=0.0, dfcd=0.0, dftd=0.0, dfcuk=0.0, dBeta=0.0, dEc=0.0, dEpsilon=0.0;
	CString strConcName = MatdD.Data1.CodeMatlName ;
	double dfsd=0.0, dfsd1=0.0, dfsk=0.0, dEs=0.0;
	double dsib = 0.0;
	if(MatdD.Data1.CodeName==MATLCODE_CON_JTG04){
		// See JTG D62-04 Table 3.1.3 fck,ftk, 3.1.4 fcd,ftd.
		if(strConcName==_T("C15"))	{dfck=10.0; dftk=1.27; dfcd= 6.9; dftd=0.88; dfcuk=15.0; dBeta=0.8;  dEc =22000.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C20")) {dfck=13.4; dftk=1.54; dfcd= 9.2; dftd=1.06; dfcuk=20.0; dBeta=0.8;  dEc =25500.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C25")) {dfck=16.7; dftk=1.78; dfcd=11.5; dftd=1.23; dfcuk=25.0; dBeta=0.8;  dEc =28000.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C30")) {dfck=20.1; dftk=2.01; dfcd=13.8; dftd=1.39; dfcuk=30.0; dBeta=0.8;  dEc =30000.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C35")) {dfck=23.4; dftk=2.20; dfcd=16.1; dftd=1.52; dfcuk=35.0; dBeta=0.8;  dEc =31500.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C40")) {dfck=26.8; dftk=2.40; dfcd=18.4; dftd=1.65; dfcuk=40.0; dBeta=0.8;  dEc =32500.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C45")) {dfck=29.6; dftk=2.51; dfcd=20.5; dftd=1.74; dfcuk=45.0; dBeta=0.8;  dEc =33500.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C50")) {dfck=32.4; dftk=2.65; dfcd=22.4; dftd=1.83; dfcuk=50.0; dBeta=0.8;  dEc =34500.0; dEpsilon=0.0033;}
		else if(strConcName==_T("C55")) {dfck=35.5; dftk=2.74; dfcd=24.4; dftd=1.89; dfcuk=55.0; dBeta=0.79; dEc =35500.0; dEpsilon=0.00325;}
		else if(strConcName==_T("C60")) {dfck=38.5; dftk=2.85; dfcd=26.5; dftd=1.96; dfcuk=60.0; dBeta=0.78; dEc =36000.0; dEpsilon=0.0032;}
		else if(strConcName==_T("C65")) {dfck=41.5; dftk=2.93; dfcd=28.5; dftd=2.02; dfcuk=65.0; dBeta=0.77; dEc =36500.0; dEpsilon=0.00315;}
		else if(strConcName==_T("C70")) {dfck=44.5; dftk=3.00; dfcd=30.5; dftd=2.07; dfcuk=70.0; dBeta=0.76; dEc =37000.0; dEpsilon=0.0031;}
		else if(strConcName==_T("C75")) {dfck=47.4; dftk=3.05; dfcd=32.4; dftd=2.10; dfcuk=75.0; dBeta=0.75; dEc =37500.0; dEpsilon=0.00305;}
		else if(strConcName==_T("C80")) {dfck=50.2; dftk=3.10; dfcd=34.6; dftd=2.14; dfcuk=80.0; dBeta=0.74; dEc =38000.0; dEpsilon=0.003;}
		else {ASSERT(0);	return FALSE;}
		for(int i=0; i<2; i++)	// 0=M-bar, 1=S-bar.
		{
			CString strRbarNa = (i==0 ? MatdD.MainRebar_RebarName : MatdD.SubRebar_RebarName);
			// See JTG D62-04 Table 3.2.3-1 fsd,f'sd, 3.2.2-1 fsk, 3.2.4 Es. 
			     if(strRbarNa==_T("R235"))	{dfsk=235.0; dfsd=195.0; dfsd1=195.0; dEs=210000.0;}
			else if(strRbarNa==_T("HRB335")) {dfsk=335.0; dfsd=280.0; dfsd1=280.0; dEs=200000.0;}
			else if(strRbarNa==_T("HRB400")) {dfsk=400.0; dfsd=330.0; dfsd1=330.0; dEs=200000.0;}
			else if(strRbarNa==_T("KL400"))	 {dfsk=400.0; dfsd=330.0; dfsd1=330.0; dEs=200000.0;}
			else {ASSERT(0);	return FALSE;}
			if (i==0) {matl.dfsd  = dfsd;	matl.dfsd1 = dfsd1; matl.dEs = dEs;   matl.dfsk = dfsk;}
			if (i==1) {matl.dfsyd = dfsd; matl.dEsy = dEs; matl.dfsky = dfsk;}
		}
		//get dsib by table5.2.1 JTG D62-2004 p25 
		if (strConcName==_T("C75") || strConcName==_T("C80")) {
			ASSERT(0); return FALSE; //提示 混凝土材料选择错误~! 
		}
		if (MatdD.MainRebar_RebarName==_T("R235")) {
			if (strConcName==_T("C65") || strConcName==_T("C70")) dsib=0.58;
			else if (strConcName==_T("C55") || strConcName==_T("C60")) dsib=0.60;
			else dsib=0.62;
		}else if (MatdD.MainRebar_RebarName==_T("HRB335")) {
			if (strConcName==_T("C65") || strConcName==_T("C70")) dsib=0.52;
			else if (strConcName==_T("C55") || strConcName==_T("C60")) dsib=0.54;
			else dsib=0.56;
		}else if (MatdD.MainRebar_RebarName==_T("HRB400") || MatdD.MainRebar_RebarName==_T("KL400")) {
			if (strConcName==_T("C65") || strConcName==_T("C70")) dsib=0.49;
			else if (strConcName==_T("C55") || strConcName==_T("C60")) dsib=0.51;
			else dsib=0.53;
		}
	}else if (MatdD.Data1.CodeName==MATLCODE_CON_TB05) {
                 if(strConcName==_T("C20")) {dfck=13.5; dftk=1.70; dfcd=13.5; dftd=1.70; dEc=28000.0;}    //
        else if(strConcName==_T("C25")) {dfck=17.0; dftk=2.00; dfcd=17.0; dftd=2.00; dEc=30000.0;}    //
        else if(strConcName==_T("C30")) {dfck=20.0; dftk=2.20; dfcd=20.0; dftd=2.20; dEc=32000.0;}    // 
        else if(strConcName==_T("C35")) {dfck=23.5; dftk=2.50; dfcd=23.5; dftd=2.50; dEc=33000.0;}    // 
        else if(strConcName==_T("C40")) {dfck=27.0; dftk=2.70; dfcd=27.0; dftd=2.70; dEc=34000.0;}    //
        else if(strConcName==_T("C45")) {dfck=30.0; dftk=2.90; dfcd=30.0; dftd=2.90; dEc=34500.0;}    //
        else if(strConcName==_T("C50")) {dfck=33.5; dftk=3.10; dfcd=33.5; dftd=3.10; dEc=35500.0;}    //
        else if(strConcName==_T("C55")) {dfck=37.0; dftk=3.30; dfcd=37.0; dftd=3.30; dEc=36000.0;}    //
        else if(strConcName==_T("C60")) {dfck=40.0; dftk=3.50; dfcd=40.0; dftd=3.50; dEc=36500.0;}    //
		for(int i=0; i<2; i++)	// 0=M-bar, 1=S-bar.
		{
			CString strRbarNa = (i==0 ? MatdD.MainRebar_RebarName : MatdD.SubRebar_RebarName);
			     if(strRbarNa==_T("Q235"))   {dfsd=235; dfsd1=235; dEs=210000.0;}    			 //
			else if(strRbarNa==_T("HRB335")) {dfsd=335; dfsd1=335; dEs=200000.0;}                         //
			else {ASSERT(0);	return FALSE;}
			if (i==0) {matl.dfsd  = dfsd;	matl.dfsd1 = dfsd1; matl.dEs = dEs;   matl.dfsk = dfsk;}
			if (i==1) {matl.dfsyd = dfsd; matl.dEsy = dEs; matl.dfsky = dfsk;}
		}		
	}else{
		ASSERT(0); return FALSE;
	}
    matl.dfcd  = dfcd;   matl.dftd  = dftd;  matl.dfcuk = dfcuk;
    matl.dEc = dEc; 	matl.dfck1 = dfck;	 matl.dftk1 = dftk;
    matl.dsib  = dsib;  matl.dBeta = dBeta; matl.dEpsilon = dEpsilon;    
    return TRUE;
}

/************************************************************************/
/* get RC design elemlist. isBeam: TRUE is beam; FALSE is column        */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CrcElemListForDgn(BOOL isPsc, CArray<UINT,UINT>& aElemK, BOOL isBeam){
	aElemK.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> aSelElemK;
    int iTotElemNum = m_pDoc->m_pAttrCtrl->GetCountElem();
    for(int i=0; i<iTotElemNum; i++)
    {
        T_ELEM_K ElemK = m_pDoc->m_pPostCtrl->GetOriginElem(i+1); 
		int iNum=0; BOOL bGetOK = FALSE;
		if (isPsc) {
			T_POSC_D PoscD; PoscD.Initialize();
			iNum = m_pDoc->m_pAttrCtrl->GetCountPosc();
			bGetOK = m_pDoc->m_pAttrCtrl->GetPosc(ElemK, PoscD);
		}else{
			T_DORC_D DorcD; DorcD.Initialize();
            iNum = m_pDoc->m_pAttrCtrl->GetCountDorc();
            bGetOK = m_pDoc->m_pAttrCtrl->GetDorc(ElemK,DorcD);
		}
		BOOL bIncludeElem = TRUE;
        if (!bGetOK && iNum>0) bIncludeElem = FALSE;
        if (bIncludeElem) aSelElemK.Add(ElemK);
	}
    for(int i=0; i<aSelElemK.GetSize(); i++)
	{
		T_ELEM_K ElemK = aSelElemK.GetAt(i);
		ElemPairK EPairK(ElemK, EN_EL_BEAM);
        if (isBeam) {
            if(!CDBLib::IsPscDgnFor1st(EPairK,TRUE))	continue;
			if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_BEAM)	continue;
        }else{
                if(!CDBLib::IsRcColumnDgn(ElemK,TRUE))	continue;    
				if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_COLUMN)	continue;
        }
		aElemK.Add(ElemK);
	}  
    if(aElemK.GetSize() > 0)	return TRUE;
	else											return FALSE;
}

/************************************************************************/
/* get beam's section data                                              */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_BeamSection(int Elemi,int iPosi,_DGN_RC_SECTION_CH_W& section)
{
    _DGN_RC_SECTION_CH sectionA;
    if(!Get_BeamSection(Elemi, iPosi, sectionA))
        return FALSE;
    section = WA_Convert::Convert_A2W(sectionA);
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_BeamSection(int Elemi,int iPosi,_DGN_RC_SECTION_CH& section){
	section.Initialize(); 
	if(!m_pDoc->IsPostMode())	ASSERT(0);
	// Get Elem.
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetElemPost(Elemi,ElemD)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少单元数据."),Elemi);
        return FALSE;
    }

	ElemPairK EPairK(Elemi, EN_EL_BEAM);
    
    //get sect
    BOOL bPosiI = (iPosi==0 ? TRUE : FALSE);
    T_SPSC_SECT SpscSect;  SpscSect.Initialize();
    BOOL bClosedSect=FALSE;
    int  nDgnSectType = -1;
    double dAg=0.0, dAoh=0.0, dPh=0.0, dKt=0.0, dWt=0.0, dTorThk=0.0;
    double aKt[6]={0.0};
    double dpMomPos[4][2]={0.,0.,0.,0.,0.,0.,0.,0.};
    double dpShrPos[3]={0.,0.,0.}, dpShrThk[3]={0.,0.,0.}, dFlgThk[2]={0.,0.};
    double dPcp=0.0, dAcp=0.0;
    CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
    CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arInner;
    if(!CSectUtil::GetPscSectCPR(TRUE, EPairK, bPosiI, FALSE, TRUE, SpscSect))	return FALSE;
	if(!CSectUtil::GetPscCoordList(TRUE, ElemD.elpro, bPosiI, arOuter, arInner))	return FALSE;
	// Change Unit (CurrUnit -> CodeUnit).
	SpscSect.dyDim	= Cng_LengthFromCurrToCode(SpscSect.dyDim, 1);
	SpscSect.dzDim	= Cng_LengthFromCurrToCode(SpscSect.dzDim, 1);
	SpscSect.dyCen	= Cng_LengthFromCurrToCode(SpscSect.dyCen, 1);
	SpscSect.dzCen	= Cng_LengthFromCurrToCode(SpscSect.dzCen, 1);
	SpscSect.dArea	= Cng_LengthFromCurrToCode(SpscSect.dArea, 2);
    //sure the 单位换算
	SpscSect.dAsy  = Cng_LengthFromCurrToCode(SpscSect.dAsy,2);
	SpscSect.dAsz	 = Cng_LengthFromCurrToCode(SpscSect.dAsz,2);
    SpscSect.dIyy  = Cng_LengthFromCurrToCode(SpscSect.dIyy,4);
    SpscSect.dIzz  = Cng_LengthFromCurrToCode(SpscSect.dIzz,4);

	// Get Aoh, Ph, Kt, FlgThk.
	double dPyKt[6]={0.,0.,0.,0.,0.,0.}, dPzKt[6]={0.,0.,0.,0.,0.,0.};
    BOOL bDivSect4Tor=FALSE;
    int nDivPart=0;
    double dCutPos[3]={0.};  
    double dLbarThk=0.0;
	int nWebSize = 1;
	if(!CSectUtil::GetPscKtAmPoint(TRUE, ElemD.elpro, bPosiI, bClosedSect, nDgnSectType, dKt, aKt, dWt, dPyKt, dPzKt, dFlgThk, bDivSect4Tor, nDivPart, dCutPos, nWebSize))	return FALSE;
	if(!CSectUtil::GetPscPeriAcor(TRUE, EPairK, bPosiI, dPh, dAoh, dLbarThk))	return FALSE;
	dAoh = Cng_LengthFromCurrToCode(dAoh,2);
	dPh	 = Cng_LengthFromCurrToCode(dPh ,1);
	dKt	 = Cng_LengthFromCurrToCode(dKt ,3);
	dWt	 = Cng_LengthFromCurrToCode(dWt ,3);
	dFlgThk[0] = Cng_LengthFromCurrToCode(dFlgThk[0],1);
	dFlgThk[1] = Cng_LengthFromCurrToCode(dFlgThk[1],1);
	// Get Position & Thickness for Shear/Torsion.
	T_SECT_D SectD;	SectD.Initialize();
	BOOL bGetSect=FALSE;
	if(!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro,SectD))	return FALSE;
    BOOL bGetI = CDBLib::IsSectI(bPosiI,SectD.nStype);
	iPosi = (bGetI ? 0 : 1);
	T_SPSC_DSHR SpscDshr;  SpscDshr.Initialize();
	CSectUtil::GetPscShearPosi(TRUE, ElemD.elpro, bPosiI, SpscDshr, TRUE);
	dAg					= Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.Area : SectD.SectBefore.SectJ.Stiffness.Area), 1);
	dpShrPos[0] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][0],1);	// Left, Top.
	dpShrPos[1] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][2],1);	// Left, Center.
	dpShrPos[2] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][4],1);	// Left, Bot.
	dpShrThk[0] = Cng_LengthFromCurrToCode(SpscDshr.db[0],1);
	dpShrThk[1] = Cng_LengthFromCurrToCode(SpscDshr.db[1],1);
	dpShrThk[2] = Cng_LengthFromCurrToCode(SpscDshr.db[2],1);
	dTorThk			= Cng_LengthFromCurrToCode(SpscDshr.dKtThik,1);
	// Get Position for Moment.
	dpMomPos[0][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx1 : SectD.SectBefore.SectJ.Stiffness.dx1), 1);
	dpMomPos[0][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy1 : SectD.SectBefore.SectJ.Stiffness.dy1), 1);
	dpMomPos[1][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx2 : SectD.SectBefore.SectJ.Stiffness.dx2), 1);
	dpMomPos[1][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy2 : SectD.SectBefore.SectJ.Stiffness.dy2), 1);
	dpMomPos[2][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx3 : SectD.SectBefore.SectJ.Stiffness.dx3), 1);
	dpMomPos[2][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy3 : SectD.SectBefore.SectJ.Stiffness.dy3), 1);
	dpMomPos[3][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx4 : SectD.SectBefore.SectJ.Stiffness.dx4), 1);
	dpMomPos[3][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy4 : SectD.SectBefore.SectJ.Stiffness.dy4), 1);

    double dbw = min(min(dpShrThk[0],dpShrThk[1]),dpShrThk[2]);
        
    // Section
    double dyDim = SpscSect.dyDim;    double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;    double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;     double dCpz  = dzDim - dzCen;

    double dCzp	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Czp : SectD.SectBefore.SectJ.Stiffness.Czp);
    double dCzm	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Czm : SectD.SectBefore.SectJ.Stiffness.Czm);
    double dCyp	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Cyp : SectD.SectBefore.SectJ.Stiffness.Cyp);
    double dCym	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Cym : SectD.SectBefore.SectJ.Stiffness.Cym);
	dCzp = Cng_LengthFromCurrToCode(dCzp,1);    dCzm = Cng_LengthFromCurrToCode(dCzm,1);
    dCyp = Cng_LengthFromCurrToCode(dCyp,1);    dCym = Cng_LengthFromCurrToCode(dCym,1);

    //section
    double dh  = dzDim;        double dbf1 = dyDim;
    double dbf2 = fabs(dpMomPos[2][0] - dpMomPos[3][0]); // 滴痢狼 辨捞啊 酒丛 x谅钎 狼 瞒捞肺窃.
    double dhf1 =dFlgThk[0];   double dhf2 =dFlgThk[1];
        
    //Rbar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;	arRbarPosi.RemoveAll();
 	// Create Rbar by CurrUnit.
	m_pDoc->m_pMatlDB->Create_RebarData(m_CurrUnit.nBase_Length, m_CurrUnit.nBase_Force, TRUE);	// Concrete=TRUE.
    if(!m_pDoc->m_pMatlDB->Exist_RebarData())	return FALSE;
	// Get Rbar.
	_DGN_RBAR_CRC RbarCrc;
    T_RPSC_D RpscD;	RpscD.Initialize();
	T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(ElemD.elpro);
//	if(!m_pDoc->m_pAttrCtrl->GetRpscByConsiderRbar(TRUE,OrgSectK,RpscD))	return FALSE;
//	if(!m_pDoc->m_pAttrCtrl->GetRpsc(OrgSectK,RpscD))	return FALSE;//by maxiao 2006.12.22
    T_SECT_D orgSECTD;
    orgSECTD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetSect(OrgSectK,orgSECTD))	return FALSE;//by maxiao 2007.02.15 Rpsd:设计钢筋
    double dOrgSectH = bPosiI ? Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectI.Stiffness.Czm + orgSECTD.SectBefore.SectI.Stiffness.Czp,1)
                                                         :Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectJ.Stiffness.Czm + orgSECTD.SectBefore.SectJ.Stiffness.Czp,1);
	if(!m_pDoc->m_pAttrCtrl->GetRpsd(OrgSectK,RpscD))	return FALSE;//by maxiao 2007.02.15 Rpsd:设计钢筋
    //Be careful:when the org sect is tapper,RpscD's  dStaZ is tapper group sections H;
	int iPosiM =bPosiI ? 0 : 1;
	if(!RpscD.bMbarJ && !bPosiI) iPosiM = 0;

	ConvertRPSCByPosition(RpscD,bPosiI,section.rbar);    //add by juhonghua(MITC) 2012.03.05
	//////////////////////////////////////////////////////////////////////////
	//add by gongxing at 2016-05-09 变截面钢筋计算优化，原来存在问题
	double dAs = 0.0, dAs1 = 0.0, das = 0.0, das1 = 0.0;
	double dSumBarD=0.0, dSumBarD2=0.0, dSumBarD1=0.0, dSumBarD21=0.0;

	if(!GetTaperSectRebarInfo(RpscD,orgSECTD,das,das1,dAs,dAs1,dSumBarD,dSumBarD2,dSumBarD1,dSumBarD21)) return FALSE;

	double dd = (dSumBarD==0.0 ? 0.0 : dSumBarD2/dSumBarD);//面积之和/直径之和，求直径
	double dd1 = (dSumBarD1==0.0 ? 0.0 : dSumBarD21/dSumBarD1);//面积之和/直径之和，求直径
	double dt1 = dTorThk;
	double dt2 = min(dFlgThk[0],dFlgThk[1]);
	double dh0 = dh - das;
	//////////////////////////////////////////////////////////////////////////
	// #if 0
	//   int iMbarSize = RpscD.Mbar[iPosiM].GetSize();
	//   ConvertRPSCByPosition(RpscD,bPosiI,section.rbar);    //add by juhonghua(MITC) 2012.03.05
	//   for(int i=0; i<iMbarSize; i++)
	//   {
	//     T_RPSC_MBAR RpscMbar = RpscD.Mbar[iPosiM].GetAt(i);
	//     //section.rbar.Add(RpscMbar);//Add by maxiao 2006.12.05
	//     int iStaYRef		= RpscMbar.iStaYRef;	// 0=档缴俊辑 谅快措莫, 1=Left俊辑 Right肺.
	//     int iStaZRef		= RpscMbar.iStaZRef;	// 0=Top, 1=Bottom.
	//     int iBarNum			= RpscMbar.iBarNum;
	//     CString strBarNa= RpscMbar.strBarNa;
	//     double dStaY		= Cng_LengthFromCurrToCode(RpscMbar.dStaY,1);
	//     double dStaZ		= Cng_LengthFromCurrToCode(RpscMbar.dStaZ,1);
	//     double dPitch		= Cng_LengthFromCurrToCode(RpscMbar.dPitch,1);
	//     double dtemp =0.0;
	//     for(int j=0; j<iBarNum; j++)
	//     {
	//       // Set Rbar. Change Unit (CurrUnit -> CodeUnit).
	//       /*
	//       RbarCrc.strRbarNa = strBarNa;
	//       RbarCrc.dArea	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(strBarNa), 2);
	//       RbarCrc.dDia	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(strBarNa) , 1);
	//       dtemp = (iStaZRef==0 ?dStaZ : -dStaZ);
	//       RbarCrc.dy		= Cng_LengthFromCurrToCode(dStaY,1);
	//       RbarCrc.dz		= Cng_LengthFromCurrToCode(dtemp,1);
	//       */
	//       double dy1st = (iStaYRef==0 ? dStaY-dPitch*(iBarNum-1)/2. : dStaY-dyCen);
	//       double dz1st = (iStaZRef==0 ? (dzDim-dStaZ)-dzCen  : dStaZ-dzCen);
	//       // Set Rbar. Change Unit (CurrUnit -> CodeUnit).
	//       RbarCrc.strRbarNa = strBarNa;
	//       RbarCrc.dArea	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(strBarNa), 2);
	//       RbarCrc.dDia	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(strBarNa) , 1);
	//       //      dtemp = (iStaZRef==0 ?dStaZ : -dStaZ);
	//       //      RbarCrc.dy		= Cng_LengthFromCurrToCode(dStaY,1);
	//       //      RbarCrc.dz		= Cng_LengthFromCurrToCode(dtemp,1);
	//       //			RbarCrc.dy		= Cng_LengthFromCurrToCode(dy1st + j*dPitch,1);
	//       //			RbarCrc.dz		= Cng_LengthFromCurrToCode(dz1st,1);
	//       RbarCrc.dy		= /*Cng_LengthFromCurrToCode*/(dy1st + j*dPitch);
	//       RbarCrc.dz		= /*Cng_LengthFromCurrToCode*/(dz1st);
	//       
	//       
	//       
	//       arRbarPosi.Add(RbarCrc);
	//     }
	//   }
	//   if(arRbarPosi.GetSize() <= 0)	return FALSE;
	//   
	//   double dAs = 0.0, dAs1 = 0.0, das = 0.0, das1 = 0.0;
	//   int inumb = 0;
	//   double dSumBarD=0.0, dSumBarD2=0.0, dSumBarD1=0.0, dSumBarD21=0.0;
	//   for(i=0; i<arRbarPosi.GetSize(); i++)
	//   {
	//     _DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
	//     /*
	//     if((Rbar.dz>0.0 && Rbar.dz <dh/2.0) || (Rbar.dz<0 && fabs(Rbar.dz)>dh/2.0 )){
	//     dAs1 += Rbar.dArea;        
	//     dSumBarD1 += Rbar.dDia;
	//     dSumBarD21 += pow(Rbar.dDia,2);
	//     if (Rbar.dz >0.0 ) das1 += Rbar.dz;
	//     else das1 += dh - fabs(Rbar.dz);
	//     inumb ++;      
	//     }else if((Rbar.dz<0.0 && fabs(Rbar.dz)< dh/2.0) ||  Rbar.dz >dh/2.0 ){
	//     dAs += Rbar.dArea;
	//     dSumBarD += Rbar.dDia;
	//     dSumBarD2 += pow(Rbar.dDia,2);
	//     if (Rbar.dz <0.0 ) 	das +=fabs(Rbar.dz);
	//     else   das += dh -Rbar.dz;
	//     }
	//     */
	//     double dzTobot = Rbar.dz + dzCen;
	//     if(dzTobot < 0.0)
	//       dzTobot = 0.0;
	//     if(dzTobot > dh)
	//       dzTobot = dh;
	//     if(dzTobot >= dh/2.0){
	//       dAs1 += Rbar.dArea;        
	//       dSumBarD1 += Rbar.dDia;
	//       dSumBarD21 += pow(Rbar.dDia,2);
	//       double dtemp=.0;
	//       
	//       dtemp = dh - dzTobot;
	//       
	//       das1 += dtemp;
	//       inumb ++;      
	//       
	//     }else if(dzTobot < dh/2.0){
	//       dAs += Rbar.dArea;
	//       dSumBarD += Rbar.dDia;
	//       dSumBarD2 += pow(Rbar.dDia,2);
	//       double dtemp = .0;
	//       
	//       dtemp= dzTobot;
	//       das += dtemp;
	//       
	//     }
	//   }  
	//   if (inumb == 0) das1 = 0.0;
	//   else  das1 = das1/(1.0*inumb);
	//   if (inumb == arRbarPosi.GetSize())  das = 0.0;
	//   else  das = das/(1.0*(arRbarPosi.GetSize()-inumb));
	//   double dd = (dSumBarD==0.0 ? 0.0 : dSumBarD2/dSumBarD);
	//   double dd1 = (dSumBarD1==0.0 ? 0.0 : dSumBarD21/dSumBarD1);
	//   double dt1 = dTorThk;
	//   double dt2 = min(dFlgThk[0],dFlgThk[1]);
	//   double dh0 = dh - das;
	// #endif 

    //shear
 	T_RPSC_SBAR RpscSbar;	RpscSbar.Initialize();
 	iPosiM = (RpscD.bSbarJ && !bPosiI ? 1 : 0);      ////add by juhonghua 2012-3-6.
	RpscSbar = RpscD.Sbar[iPosiM];
	RpscSbar.dSbarTheta /= (180./m_dPi);	// Deg -> Rad.
	RpscSbar.dTndnTheta /= (180./m_dPi);	// Deg -> Rad.  
    //////////////////////////////////////////////////////////////////////////
    double dAsh    = (RpscSbar.bStirExist ? Cng_LengthFromCurrToCode(RpscSbar.dStirAw,2) : 0.0);    //Stirrup 
    double dPith   = (RpscSbar.bStirExist ? Cng_LengthFromCurrToCode(RpscSbar.dStirPitch,1): 0.0);
    double dAsb    = (RpscSbar.bSbarExist ? Cng_LengthFromCurrToCode(RpscSbar.dSbarAw,2): 0.0);     //弯起钢筋.
    double dSv_b   = (RpscSbar.bSbarExist ? Cng_LengthFromCurrToCode(RpscSbar.dSbarPitch,1): 0.0);  //版荤枚辟 埃拜.
    double dAnglb  = (RpscSbar.bSbarExist ? RpscSbar.dSbarTheta: 0.0);
	
	double dAsv1   = (RpscSbar.bLbarExist ? Cng_LengthFromCurrToCode(RpscSbar.dLbarAwt,2): 0.0);
    double dSv_t   = (RpscSbar.bLbarExist ? Cng_LengthFromCurrToCode(RpscSbar.dLbarPitch,1): 0.0); 
    double dAslt   = (RpscSbar.bLbarExist ? Cng_LengthFromCurrToCode(RpscSbar.dLbarAlt,2): 0.0);
    double dat     = (RpscSbar.bLbarExist ? Cng_LengthFromCurrToCode(RpscSbar.dLbarThick,1): 0.0);
    
	double dThetas = (RpscSbar.bTndnExist ? RpscSbar.dTndnTheta: 0.0);
    double dApbar  = (RpscSbar.bTndnExist ? Cng_LengthFromCurrToCode(RpscSbar.dTndnAp,2): 0.0);
    double dSv_p   = (RpscSbar.bTndnExist ? Cng_LengthFromCurrToCode(RpscSbar.dTndnDist,1): 0.0);
    //////////////////////////////////////////////////////////////////////////
    section.db     = dbw;                    section.dh     = dh;
	section.dbf1   = dbf1;                   section.dhf1   = dhf1;
	section.das    = das;                    section.das1   = das1;
	section.dAs    = dAs;                    section.dAs1   = dAs1;
	section.dh0    = dh0;
    section.dAsh   = dAsh;	                 section.dPith  = dPith;
    section.dbf2   = dbf2;                   section.dhf2   = dhf2;
	section.dAsb   = dAsb;                   section.dPitb  = dSv_b;
	section.dAnglb = dAnglb;
	section.dat    = dat;                    
    section.dAst   = dAslt;                  section.dAsv1  = dAsv1;
    section.dPitt  = dSv_t;                  section.dAnglt = dThetas;
    section.dWt    = dWt;
    section.dAoh   = dAoh;
    section.dPh    = dPh;
    section.dA     = SpscSect.dArea;
    section.dd     = dd;                     section.dd1    = dd1;
    section.dIyy   = SpscSect.dIyy;          section.dIzz   = SpscSect.dIzz;
    section.dCym   = dCym;                   section.dCyp   = dCyp;
    section.dCzm   = dCzm;                   section.dCzp   = dCzp;

    double dBetaa  = 1.0;//not closed section always = 1.0
	if (bClosedSect)
	{
		if (dt1>1.0E-07 && dt2>1.0E-07) 
		{
			double db=min(dbf1, dbf2); //dyDim;
			if((0.1*db<=dt2 && dt2<=0.25*db) || (0.1*dh<=dt1 && dt1<=0.25*dh))
			{
				double dba1 = (db==0.0 ? 0.0 : 4.0*dt2/db);
				double dba2 = (dh==0.0 ? 0.0 : 4.0*dt1/dh);
				dBetaa = min(dba1, dba2);
			}
			else if(dt2>0.25*db && dt1>0.25*dh) { dBetaa = 1.0; }
			else if(0.1*db>dt2 && 0.1*dh>dt1)  // { dBetaa = 1.0; }//2010-6-10 By maxiao (LiuMeiLan)
            {
                double dba1 = (db==0.0 ? 0.0 : 4.0*dt2/db);
                double dba2 = (dh==0.0 ? 0.0 : 4.0*dt1/dh);
				dBetaa = min(dba1, dba2);
            }
		}
    }
    section.dBetaa = dBetaa;

	if (fabs(section.dhf2 - section.dh)<1.0E-07) section.dhf2 = .0;
	if (fabs(section.dhf1 - section.dh)<1.0E-07) section.dhf1 = .0;

    if(section.dh <=0.0 || section.db <= 0.0 ||section.dA <=0.0 || section.dh0 <=0.0 ) return FALSE;
	return TRUE;
}

/************************************************************************/
/* get PSC beam's Tapersection Rebar data           
/*add by gongxing at 2016-05-09 插值获取变截面钢筋                                   */
/************************************************************************/
BOOL CRCDataCtrl_CH::GetTaperSectRebarInfo(const T_RPSC_D& RpscD,const T_SECT_D& orgSECTD,double& das,double& das1,double& dAs,double& dAs1,
										   double& dSumBarD,double& dSumBarD2,double& dSumBarD1,double& dSumBarD21)
{
	//////////////////////////////////////////////////////////////////////////
	/*add by gongxing at 2016-05-09 变截面钢筋插值方法优化，原来算法存在问题*/
	_DGN_RBAR_CRC RbarCrc;
	RbarCrc.Initialize();
	//计算I端
	int iMbarSizeI = RpscD.Mbar[0].GetSize();
	double SectHI = Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectI.Stiffness.Czm + orgSECTD.SectBefore.SectI.Stiffness.Czp,1);//截面高度
	double dyDimI = Cng_LengthFromCurrToCode(fabs(orgSECTD.SectBefore.SectI.Stiffness.dx1 - orgSECTD.SectBefore.SectI.Stiffness.dx2),1);  //顶部宽度  
	double dzDimI = Cng_LengthFromCurrToCode(fabs(orgSECTD.SectBefore.SectI.Stiffness.dy1 - orgSECTD.SectBefore.SectI.Stiffness.dy3),1);//截面高度
	double dyCenI = Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectI.Stiffness.Cym,1);    
	double dzCenI = Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectI.Stiffness.Czm,1);//中性轴以上高度

	_DGN_RC_SECTION_CH tempSectionI;  tempSectionI.Initialize();
	ConvertRPSCByPosition(RpscD,1,tempSectionI.rbar);//取I端钢筋
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosiI;arRbarPosiI.RemoveAll();
	for(int i=0; i<iMbarSizeI; i++)
	{
		T_RPSC_MBAR RpscMbarI= RpscD.Mbar[0].GetAt(i);
		int iStaYRefI		= RpscMbarI.iStaYRef;	// 0=质心, 1=Left 2=Right.
		int iStaZRefI		= RpscMbarI.iStaZRef;	// 0=Top, 1=Bottom.
		int iBarNumI			= RpscMbarI.iBarNum;
		CString strBarNaI= RpscMbarI.strBarNa;
		double dStaYI		= Cng_LengthFromCurrToCode(RpscMbarI.dStaY,1);
		double dStaZI		= Cng_LengthFromCurrToCode(RpscMbarI.dStaZ,1);
		double dPitchI		= Cng_LengthFromCurrToCode(RpscMbarI.dPitch,1);
		for(int j=0; j<iBarNumI; j++)
		{
			double dy1stI = (iStaYRefI==0 ? dStaYI-dPitchI*(iBarNumI-1)/2. : dStaYI-dyCenI);
			double dz1stI = (iStaZRefI==0 ? (dzDimI-dStaZI)-dzCenI  : dStaZI-dzCenI);
			RbarCrc.strRbarNa = strBarNaI;
			RbarCrc.dArea	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(strBarNaI), 2);
			RbarCrc.dDia	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(strBarNaI) , 1);
			RbarCrc.dy		= (dy1stI + j*dPitchI);
			RbarCrc.dz		= (dz1stI);

			arRbarPosiI.Add(RbarCrc);
		}
	}

	double dIAs = 0.0, dIAs1 = 0.0, dIas = 0.0, dIas1 = 0.0;
	double dISumAreaas = 0.0,  dISumAreaas1 = 0.0; 
	double dISumBarD=0.0, dISumBarD2=0.0, dISumBarD1=0.0, dISumBarD21=0.0;
	for(int i=0; i<arRbarPosiI.GetSize(); i++)
	{
		_DGN_RBAR_CRC RbarI = arRbarPosiI.GetAt(i);
		double dzToBotI = RbarI.dz + dzCenI;//距离顶部的距离
		if(dzToBotI < 0.0)
		{
			dzToBotI = 0.0;
		}

		if(dzToBotI > dzDimI)
		{
			dzToBotI = dzDimI;
		}

		if(dzToBotI >= dzCenI)//顶部钢筋
		{
			dIAs1 += RbarI.dArea;        
			dISumBarD1 += RbarI.dDia;
			dISumBarD21 += pow(RbarI.dDia,2);
			dISumAreaas1 += RbarI.dArea*(SectHI - dzCenI - RbarI.dz);    
		}
		else if(dzToBotI < dzCenI)//底部钢筋
		{
			dIAs += RbarI.dArea;
			dISumBarD += RbarI.dDia;
			dISumBarD2 += pow(RbarI.dDia,2);
			dISumAreaas += RbarI.dArea * dzToBotI; 
		}
	}  
	dIas = dIAs >0 ? dISumAreaas/dIAs : 0.0;
	dIas1 = dIAs1 >0 ? dISumAreaas1/dIAs1 : 0.0;//modified this line by gongxing at 2016-08-25

	//计算J端
	int iMbarSizeJ = RpscD.Mbar[1].GetSize();
	double SectHJ = Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectJ.Stiffness.Czm + orgSECTD.SectBefore.SectJ.Stiffness.Czp,1);
	double dyDimJ = Cng_LengthFromCurrToCode(fabs(orgSECTD.SectBefore.SectJ.Stiffness.dx1 - orgSECTD.SectBefore.SectJ.Stiffness.dx2),1);    
	double dzDimJ = Cng_LengthFromCurrToCode(fabs(orgSECTD.SectBefore.SectJ.Stiffness.dy1 - orgSECTD.SectBefore.SectJ.Stiffness.dy3),1);
	double dyCenJ = Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectJ.Stiffness.Cym,1);    
	double dzCenJ = Cng_LengthFromCurrToCode(orgSECTD.SectBefore.SectJ.Stiffness.Czm,1);

	if (orgSECTD.SectBefore.nStype == 0)//等截面时，需特殊处理 add this statement by gongxing at 2016-06-26
	{
		SectHJ = SectHI;
		dyDimJ = dyDimI;
		dzDimJ = dzDimI;
		dyCenJ = dyCenI;
		dzCenJ = dzCenI;
	}

	_DGN_RC_SECTION_CH tempSectionJ;  tempSectionJ.Initialize();
	ConvertRPSCByPosition(RpscD,0,tempSectionJ.rbar);//取J端钢筋
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosiJ;arRbarPosiJ.RemoveAll();

	for(int ii=0; ii<iMbarSizeJ; ii++)
	{
		T_RPSC_MBAR RpscMbarJ= RpscD.Mbar[1].GetAt(ii);
		int iStaYRefJ		= RpscMbarJ.iStaYRef;	// 0=质心, 1=Left 2=Right.
		int iStaZRefJ		= RpscMbarJ.iStaZRef;	// 0=Top, 1=Bottom.
		int iBarNumJ			= RpscMbarJ.iBarNum;
		CString strBarNaJ= RpscMbarJ.strBarNa;
		double dStaYJ		= Cng_LengthFromCurrToCode(RpscMbarJ.dStaY,1);
		double dStaZJ		= Cng_LengthFromCurrToCode(RpscMbarJ.dStaZ,1);
		double dPitchJ		= Cng_LengthFromCurrToCode(RpscMbarJ.dPitch,1);
		for(int jj=0; jj<iBarNumJ; jj++)
		{
			double dy1stJ = (iStaYRefJ==0 ? dStaYJ-dPitchJ*(iBarNumJ-1)/2. : dStaYJ-dyCenJ);
			double dz1stJ = (iStaZRefJ==0 ? (dzDimJ-dStaZJ)-dzCenJ  : dStaZJ-dzCenJ);
			RbarCrc.strRbarNa = strBarNaJ;
			RbarCrc.dArea	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(strBarNaJ), 2);
			RbarCrc.dDia	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(strBarNaJ) , 1);
			RbarCrc.dy		= (dy1stJ + jj*dPitchJ);
			RbarCrc.dz		= (dz1stJ);

			arRbarPosiJ.Add(RbarCrc);
		}
	}

	double dJAs = 0.0, dJAs1 = 0.0, dJas = 0.0, dJas1 = 0.0;
	double dJSumAreaas = 0.0,  dJSumAreaas1 = 0.0; 
	double dJSumBarD=0.0, dJSumBarD2=0.0, dJSumBarD1=0.0, dJSumBarD21=0.0;
	for(int i=0; i<arRbarPosiJ.GetSize(); i++)
	{
		_DGN_RBAR_CRC RbarJ = arRbarPosiJ.GetAt(i);
		double dzToBotJ = dzCenJ + RbarJ.dz ;//距离底边的距离
		if(dzToBotJ < 0.0)
		{
			dzToBotJ = 0.0;
		}

		if(dzToBotJ > dzDimJ)
		{
			dzToBotJ = dzDimJ;
		}

		if(dzToBotJ >= dzCenJ)//顶部钢筋
		{
			dJAs1 += RbarJ.dArea;        
			dJSumBarD1 += RbarJ.dDia;
			dJSumBarD21 += pow(RbarJ.dDia,2);
			dJSumAreaas1 += RbarJ.dArea*(SectHJ - dzCenJ - RbarJ.dz);     
		}
		else if(dzToBotJ < dzCenJ)//底部钢筋
		{
			dJAs += RbarJ.dArea;
			dJSumBarD += RbarJ.dDia;
			dJSumBarD2 += pow(RbarJ.dDia,2);
			dJSumAreaas += RbarJ.dArea * (dzToBotJ); 
		}
	}  
	dJas = dJAs >0 ? dJSumAreaas/dJAs : 0.0;
	dJas1 = dJAs1 >0 ? dJSumAreaas1/dJAs1 : 0.0;//modified this line by gongxing at 2016-08-25

	if(arRbarPosiI.GetSize() <= 0 && arRbarPosiJ.GetSize() <= 0)	return FALSE;

	das = (dIas+dJas)/2;
	das1 = (dIas1+dJas1)/2;
	dAs = (dIAs+dJAs)/2;
	dAs1 = (dIAs1+dJAs1)/2;
	dSumBarD = (dISumBarD+dJSumBarD)/2;
	dSumBarD2 = (dISumBarD2+dJSumBarD2)/2;
	dSumBarD1 = (dISumBarD1+dJSumBarD1)/2;
	dSumBarD21 = (dISumBarD21+dJSumBarD21)/2;

	return TRUE;
}

/************************************************************************/
/* get PSC beam's section data                                              */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_PSCBeamSection(int Elemi,int iPosi,_DGN_RC_SECTION_CH& section){
	section.Initialize(); 
	if(!m_pDoc->IsPostMode())	ASSERT(0);
	// Get Elem.
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetElemPost(Elemi,ElemD)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少单元数据."),Elemi);
        return FALSE;
    }  
	
	ElemPairK EPairK(Elemi, EN_EL_BEAM);
    //get sect
    BOOL bPosiI = (iPosi==0 ? TRUE : FALSE);
    T_SPSC_SECT SpscSect;  SpscSect.Initialize();
    BOOL bClosedSect=FALSE;
    int  nDgnSectType = -1;
    double dAg=0.0, dAoh=0.0, dPh=0.0, dKt=0.0, dWt=0.0, dTorThk=0.0;
    double aKt[6]={0.0};
    double dpMomPos[4][2]={0.,0.,0.,0.,0.,0.,0.,0.};
    double dpShrPos[3]={0.,0.,0.}, dpShrThk[3]={0.,0.,0.}, dFlgThk[2]={0.,0.};
    double dPcp=0.0, dAcp=0.0;
    CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
    CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arInner;
    if(!CSectUtil::GetPscSectCPR(TRUE, EPairK, bPosiI, FALSE, TRUE, SpscSect))	return FALSE;
	if(!CSectUtil::GetPscCoordList(TRUE, ElemD.elpro, bPosiI, arOuter, arInner))	return FALSE;
	// Change Unit (CurrUnit -> CodeUnit).
	SpscSect.dyDim	= Cng_LengthFromCurrToCode(SpscSect.dyDim, 1);
	SpscSect.dzDim	= Cng_LengthFromCurrToCode(SpscSect.dzDim, 1);
	SpscSect.dyCen	= Cng_LengthFromCurrToCode(SpscSect.dyCen, 1);
	SpscSect.dzCen	= Cng_LengthFromCurrToCode(SpscSect.dzCen, 1);
	SpscSect.dArea	= Cng_LengthFromCurrToCode(SpscSect.dArea, 2);
    //sure the 单位换算
	SpscSect.dAsy  = Cng_LengthFromCurrToCode(SpscSect.dAsy,2);
	SpscSect.dAsz	 = Cng_LengthFromCurrToCode(SpscSect.dAsz,2);
    SpscSect.dIyy  = Cng_LengthFromCurrToCode(SpscSect.dIyy,4);
    SpscSect.dIzz  = Cng_LengthFromCurrToCode(SpscSect.dIzz,4);

	// Get Aoh, Ph, Kt, FlgThk.
	double dPyKt[6]={0.,0.,0.,0.,0.,0.}, dPzKt[6]={0.,0.,0.,0.,0.,0.};
    BOOL bDivSect4Tor=FALSE;
    int nDivPart=0;
    double dCutPos[3]={0.};  
    double dLbarThk=0.0;
	int nWebSize = 1;
	if(!CSectUtil::GetPscKtAmPoint(TRUE, ElemD.elpro, bPosiI, bClosedSect, nDgnSectType, dKt, aKt, dWt, dPyKt, dPzKt, dFlgThk, bDivSect4Tor, nDivPart, dCutPos, nWebSize))	return FALSE;
	if(!CSectUtil::GetPscPeriAcor(TRUE, EPairK, bPosiI, dPh, dAoh, dLbarThk))	return FALSE;
	dAoh = Cng_LengthFromCurrToCode(dAoh,2);
	dPh	 = Cng_LengthFromCurrToCode(dPh ,1);
	dKt	 = Cng_LengthFromCurrToCode(dKt ,3);
	dWt	 = Cng_LengthFromCurrToCode(dWt ,3);
	dFlgThk[0] = Cng_LengthFromCurrToCode(dFlgThk[0],1);
	dFlgThk[1] = Cng_LengthFromCurrToCode(dFlgThk[1],1);
	// Get Position & Thickness for Shear/Torsion.
	T_SECT_D SectD;	SectD.Initialize();
	BOOL bGetSect=FALSE;
	if(!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro,SectD))	return FALSE;
    BOOL bGetI = CDBLib::IsSectI(bPosiI,SectD.nStype);
	iPosi = (bGetI ? 0 : 1);
	T_SPSC_DSHR SpscDshr;  SpscDshr.Initialize();
	CSectUtil::GetPscShearPosi(TRUE, ElemD.elpro, bPosiI, SpscDshr, TRUE);
	dAg					= Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.Area : SectD.SectBefore.SectJ.Stiffness.Area), 1);
	dpShrPos[0] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][0],1);	// Left, Top.
	dpShrPos[1] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][2],1);	// Left, Center.
	dpShrPos[2] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][4],1);	// Left, Bot.
	dpShrThk[0] = Cng_LengthFromCurrToCode(SpscDshr.db[0],1);
	dpShrThk[1] = Cng_LengthFromCurrToCode(SpscDshr.db[1],1);
	dpShrThk[2] = Cng_LengthFromCurrToCode(SpscDshr.db[2],1);
	dTorThk			= Cng_LengthFromCurrToCode(SpscDshr.dKtThik,1);

	// Get Position for Moment.
	dpMomPos[0][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx1 : SectD.SectBefore.SectJ.Stiffness.dx1), 1);
	dpMomPos[0][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy1 : SectD.SectBefore.SectJ.Stiffness.dy1), 1);
	dpMomPos[1][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx2 : SectD.SectBefore.SectJ.Stiffness.dx2), 1);
	dpMomPos[1][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy2 : SectD.SectBefore.SectJ.Stiffness.dy2), 1);
	dpMomPos[2][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx3 : SectD.SectBefore.SectJ.Stiffness.dx3), 1);
	dpMomPos[2][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy3 : SectD.SectBefore.SectJ.Stiffness.dy3), 1);
	dpMomPos[3][0] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dx4 : SectD.SectBefore.SectJ.Stiffness.dx4), 1);
	dpMomPos[3][1] = Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.dy4 : SectD.SectBefore.SectJ.Stiffness.dy4), 1);

    double dbw = min(min(dpShrThk[0],dpShrThk[1]),dpShrThk[2]);
        
    // Section
    double dyDim = SpscSect.dyDim;  double dzDim = SpscSect.dzDim;
    double dyCen = SpscSect.dyCen;  double dzCen = SpscSect.dzCen;
    double dCpy  = dyDim - dyCen;   double dCpz  = dzDim - dzCen;

    double dCzp	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Czp : SectD.SectBefore.SectJ.Stiffness.Czp);
    double dCzm	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Czm : SectD.SectBefore.SectJ.Stiffness.Czm);
    double dCyp	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Cyp : SectD.SectBefore.SectJ.Stiffness.Cyp);
    double dCym	 = (bGetI ? SectD.SectBefore.SectI.Stiffness.Cym : SectD.SectBefore.SectJ.Stiffness.Cym);
	dCzp = Cng_LengthFromCurrToCode(dCzp,1);   dCzm = Cng_LengthFromCurrToCode(dCzm,1);
    dCyp = Cng_LengthFromCurrToCode(dCyp,1);   dCym = Cng_LengthFromCurrToCode(dCym,1);

    //section
    double dh  = dzDim;       double dbf1 = dyDim;
    double dbf2 = fabs(dpMomPos[2][0] - dpMomPos[3][0]); // 滴痢狼 辨捞啊 酒丛 x谅钎 狼 瞒捞肺窃.
    double dhf1 =dFlgThk[0];  double dhf2 =dFlgThk[1];    
    //////////////////////////////////////////////////////////////////////////
    section.db     = dbw;                section.dh     = dh;
    section.dbf1   = dbf1;               section.dhf1   = dhf1;
    section.dbf2   = dbf2;               section.dhf2   = dhf2;
    section.dA     = SpscSect.dArea;
    section.dIyy   = SpscSect.dIyy;      section.dIzz   = SpscSect.dIzz;
    section.dCym   = dCym;               section.dCyp   = dCyp;
    section.dCzm   = dCzm;               section.dCzp   = dCzp;

	if (fabs(section.dhf2 - section.dh)<1.0E-07) section.dhf2 = .0;
	if (fabs(section.dhf1 - section.dh)<1.0E-07) section.dhf1 = .0;

    if(section.dh <=0.0 || section.db <= 0.0 ||section.dA <=0.0 ) return FALSE;
	return TRUE;
}

/************************************************************************/
/* get column's section data                                            */
/************************************************************************/
    BOOL CRCDataCtrl_CH::Get_ColumnSection(int Elemi,int iPosi,_DGN_RC_COLUMN_SECTION_CH_W& section) //iPosi(0:I, 1:J)
    {
         _DGN_RC_COLUMN_SECTION_CH sectD;
         if(!Get_ColumnSection(Elemi, iPosi, sectD))
             return FALSE;
         section = WA_Convert::Convert_A2W(sectD);
         return TRUE;
    }
BOOL CRCDataCtrl_CH::Get_ColumnSection(int Elemi,int iPosi,_DGN_RC_COLUMN_SECTION_CH& section){
    section.Initialize(); 
	if(!m_pDoc->IsPostMode())	ASSERT(0);
	// Get Elem.
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetElemPost(Elemi,ElemD)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少单元数据."),Elemi);
        return FALSE;
    }
//		if(!m_pDoc->m_pAttrCtrl->GetElem(iElem, eData))	ASSERT(0);
//		iSect = eData.elpro;  
    int iSect = ElemD.elpro;

 	T_SECT_D sData;	sData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetSectDesign(iSect, sData)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少截面数据."),Elemi);
        return FALSE;
    }
    int iSectShp; double m_dD[8];
    double m_dArea =0.0,m_dAsy=0.0,m_dAsz=0.0,m_dYbar=0.0,m_dZbar=0.0;
    double m_dIxx =0.0,m_dIyy =0.0, m_dIzz=0.0,m_dRoy=0.0,m_dRoz=0.0;

	if(sData.nStype==D_SECT_TYPE_REGULAR)
	{
		CString strShp = sData.SectBefore.Shape;    
		if(strShp==     D_SECT_SHAPE_REG_L   )	iSectShp =  0;
		else if(strShp==D_SECT_SHAPE_REG_C   )	iSectShp =  1;
		else if(strShp==D_SECT_SHAPE_REG_H   )	iSectShp =  2;
		else if(strShp==D_SECT_SHAPE_REG_T   )	iSectShp =  3;
		else if(strShp==D_SECT_SHAPE_REG_B   )	iSectShp =  4;
		else if(strShp==D_SECT_SHAPE_REG_P   )	iSectShp =  5;
		else if(strShp==D_SECT_SHAPE_REG_SR  )	iSectShp =  6;
		else if(strShp==D_SECT_SHAPE_REG_SB  )	iSectShp =  7;
		else if(strShp==D_SECT_SHAPE_REG_2L  )	iSectShp =  8;
		else if(strShp==D_SECT_SHAPE_REG_2C  )	iSectShp =  9;
		else if(strShp==D_SECT_SHAPE_REG_CC  )	iSectShp = 10;
		else if(strShp==D_SECT_SHAPE_REG_URIB)	iSectShp = 11;
		else if(strShp==D_SECT_SHAPE_REG_OCT )	iSectShp = 12;
		else if(strShp==D_SECT_SHAPE_REG_SOCT)	iSectShp = 13;
		else if(strShp==D_SECT_SHAPE_REG_TRK )	iSectShp = 14;
		else if(strShp==D_SECT_SHAPE_REG_STRK)	iSectShp = 15;
		else if(strShp==D_SECT_SHAPE_REG_HTRK)	iSectShp = 16;
		else if(strShp==D_SECT_SHAPE_REG_CL  )	iSectShp = 17;
		else if(strShp==D_SECT_SHAPE_REG_ROCT)  iSectShp = 18;  //add by maxiao 2007.12.26
		else	ASSERT(0);
		m_dD[0] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[0],1);
		m_dD[1] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[1],1);
		m_dD[2] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[2],1);
		m_dD[3] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[3],1);
		m_dD[4] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[4],1);
		m_dD[5] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[5],1);
		m_dD[6] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[6],1);
		m_dD[7] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Size[7],1);
        //sure the 单位换算
		m_dArea = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Area,2);
		m_dAsy	= Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Asy,2);
		m_dAsz	= Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Asz,2);
        m_dIxx  = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Rxx,4);
        m_dIyy  = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Ryy,4);
        m_dIzz  = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Rzz,4);
		m_dYbar = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Design.YBar,1);
		m_dZbar = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Design.ZBar,1);
		m_dRoy	= Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Design.Roy,1);
		m_dRoz	= Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Design.Roz,1);

        //get czp\czp//add by maxiao 2006.12.19
        section.dCzp = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Czp,1);
        section.dCyp = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Cyp,1);
        section.dCzm = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Czm,1);
        section.dCym = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Cym,1);
        section.dIyy = m_dIyy;     section.dIzz = m_dIzz;

		if(iSectShp==4)	// B.
		{
			if(m_dD[4]==0.0)	m_dD[4] = m_dD[1]-m_dD[2];	// Cw = B1-tw.
			if(m_dD[5]==0.0)	m_dD[5] = m_dD[2];					// Tf2 = Tf1.
            if(fabs(m_dD[4]-(m_dD[1]-m_dD[2])) > 1.0E-07){
                GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 的箱形截面数据有误."),Elemi);
                return  FALSE;
            }
		}
        if(m_dArea*m_dAsy*m_dAsz*m_dYbar*m_dZbar==0.0){
                GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 的截面数据有误."),Elemi);
                return  FALSE;
        }
	}
	else	return FALSE;
    if (m_dD[1] < 1.0E-07 )      section.dr = m_dD[0]/2.0;
    else { section.dh = m_dD[0]; section.db = m_dD[1]; }
    if (iSectShp == 2) {
        section.dh =  m_dD[0];  section.db = m_dD[2];
        section.dbf1 = m_dD[1]; section.dbf2 = m_dD[4];
        section.dhf1 = m_dD[3]; section.dhf2 = m_dD[5];
    }
    section.dA =  m_dArea; 
    //////////////////////////////////////////////////////////////////////////
    int CONST_RCSC_iPOS_DIV		=  5;
    int CONST_RCSC_iGRUPNUM		=  3;
    int CONST_RCSC_iBAR_LAY		=  5;
 	// Create Rbar by CurrUnit.
	m_pDoc->m_pMatlDB->Create_RebarData(m_CurrUnit.nBase_Length, m_CurrUnit.nBase_Force, TRUE);	// Concrete=TRUE.
    if(!m_pDoc->m_pMatlDB->Exist_RebarData()){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少配筋数据."),Elemi);
        return FALSE;
    }

    T_RCHK_D RchkData;		RchkData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetRchk(iSect, RchkData)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 的配筋数据有误."),Elemi);
        return FALSE;
    }
    double dDc =0.0, dAreatb = 0.0, dArealr =0.0, ddtb =0.0, ddlr = 0.0 ; 
    int    iDc = 0,  iAreatb = 0,   iArealr =0;
	for(int j=0; j<CONST_RCSC_iBAR_LAY; j++)//柱钢筋层数
    {
		// Before_Release_v825_Check - Rebar 备炼眉 函版登搁辑 荐沥登绢具 窍绰 何盒 
		// 老窜 0 栏肺 绊沥秦滴菌促~	

        if(RchkData.COLM.dDc[0][j] > 0.0){ iDc ++; dDc += Cng_LengthFromCurrToCode(RchkData.COLM.dDc[0][j],1);}
        for(int k=0; k<CONST_RCSC_iGRUPNUM; k++)
        {
            if(RchkData.COLM.iBarNum[0][k][j] > 0)
            {
                BOOL bHasDiffD = !(RchkData.COLM.strBarNa2[0][k][j].IsEmpty() || RchkData.COLM.strBarNa2[0][k][j].GetLength() == 0 || RchkData.COLM.strBarNa2[0][k][j] == _T(""));
		  double dBarNum1 = 0.0;
		  double dBarNum2 = 0.0;
		  if(bHasDiffD)
		  {
			   dBarNum1 = ceil(RchkData.COLM.iBarNum[0][k][j]/2.);
			   dBarNum2 = RchkData.COLM.iBarNum[0][k][j] - dBarNum1;
		  }
		  else
		  {
			   dBarNum1 = RchkData.COLM.iBarNum[0][k][j];
			   dBarNum2 = 0;

		  }

                if (k ==0) {
                    iAreatb++; 
                    dAreatb += dBarNum1*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[0][k][j]),2); 
                    ddtb += Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(RchkData.COLM.strBarNa1[0][k][j]),1);

		  if(bHasDiffD)
		  {
			  iAreatb++; 
			  dAreatb += dBarNum2*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa2[0][k][j]),2); 
			  ddtb += Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(RchkData.COLM.strBarNa2[0][k][j]),1);
		  }

                }
                if (k ==1) {
                    iArealr++;
                    dArealr += dBarNum1*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[0][k][j]),2); 
                    ddlr += Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(RchkData.COLM.strBarNa1[0][k][j]),1);

		  if(bHasDiffD)
		  {
			  iArealr++;
			  dArealr += dBarNum2*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa2[0][k][j]),2); 
			  ddlr += Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(RchkData.COLM.strBarNa2[0][k][j]),1);
		  }

                }
            }
        }
    }
    section.das = section.das1 = dDc/iDc;
    section.dAs = section.dAs1 = dAreatb;
    section.dAsl = section.dAsr = dArealr;
    if (iAreatb==0) {section.dd =0;}else{ section.dd = ddtb/iAreatb;}
    if (iArealr==0) {section.dd1=0;}else{ section.dd1 = ddlr/iArealr;}
    if (iSectShp == 7) { section.dh0 = section.dh- section.das; }//矩形
    if (iSectShp == 6) { //round
        section.dC = section.das; section.drs = section.dr-section.das;
        section.dh0 = section.dr+ section.drs; 
        section.dh = section.db = 2.0 * section.dr;
    }
    if (iSectShp == 2) {//H
        section.dAs1 = section.dAsl;
        section.dh0 = section.dh - section.das;
    }  //add by maxiao 2007.12.26
    else if (iSectShp ==  4) {//箱形
	  section.dbf1 = section.dbf2 = m_dD[1];
	  section.dhf1 = m_dD[3];
	  section.dhf2 = m_dD[5];
	  section.dh = m_dD[0];
	  section.db = m_dD[2]*2;
	  section.dh0 = section.dh - section.das;
    }else if (iSectShp == 12) {//8角形
	  section.dbf1 = section.dbf2 = m_dD[1];
	  section.dhf1 = section.dhf2 = m_dD[4];
	  section.dh = m_dD[0];
	  section.db = m_dD[4]*2;
	  section.dh0 = section.dh - section.das;
    }else if (iSectShp == 18) {//内8角形
	  int tempn = sData.SectBefore.nCellShape;
	  section.dbf1 = section.dbf2 = m_dD[1];
	  section.dhf1 = section.dhf2 = m_dD[5];
	  section.dh = m_dD[0];
	  section.db = m_dD[4]*2 + (tempn-1)*m_dD[6];
	  section.dh0 = section.dh - section.das;
    }else if (iSectShp == 14) {//轨道型
	  section.dbf1 = section.dbf2 = m_dD[1] - m_dD[0];
	  section.dhf1 = section.dhf2 = m_dD[2];
	  section.dh = m_dD[0];
	  section.db = m_dD[2]*2;
	  section.dh0 = section.dh - section.das;
    }else if (iSectShp == 13) {//实腹8角形
	  section.dbf1 = section.dbf2 = m_dD[1] - 2*m_dD[2];
	  section.dhf1 = section.dhf2 = m_dD[3];
	  section.dh = m_dD[0];
	  section.db = m_dD[1];
	  section.dh0 = section.dh - section.das;
    }else if (iSectShp == 15) {//实腹轨道型
	  section.dbf1 = section.dbf2 = 0.0;
	  section.dhf1 = section.dhf2 = 0.0;
	  section.dh = m_dD[0];
	  if (m_dD[0]>1.0E-7)
	  section.db = 12*m_dIyy/m_dD[0]/m_dD[0]/m_dD[0];
	  section.dh0 = section.dh - section.das;
    }else if (iSectShp == 16) {//半轨道型
	  section.dbf1 = section.dbf2 = 0.0;
	  section.dhf1 = section.dhf2 = 0.0;
	  section.dh = m_dD[0];
	  if (m_dD[0]>1.0E-7)
	  section.db = 12*m_dIyy/m_dD[0]/m_dD[0]/m_dD[0];
	  section.dh0 = section.dh - section.das;
    }
	//end add
    if(section.dh0<=0.0 || section.dh*section.db ==0.0){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 的截面数据有误."),Elemi);
        return FALSE;
    }
	double dSubBarNum = min(RchkData.COLM.dSubBarNum_y[0], RchkData.COLM.dSubBarNum_z[0]);
    if(dSubBarNum > 0.0)
    {
        double dSubBarArea = Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strSubBarNa[0]),2);
        section.dAsh = dSubBarArea* dSubBarNum;
        section.dPith = Cng_LengthFromCurrToCode(RchkData.COLM.dSubBarDist[0],1);
    }      

    //得到计算长度
     T_LENG_D Leng; Leng.Initialize(); 
     T_KFAC_D Kfac; Kfac.Initialize();
     m_pDoc->m_pAttrCtrl->GetLeng(Elemi,Leng);
     m_pDoc->m_pAttrCtrl->GetKfac(Elemi,Kfac);
     section.dl0 = Cng_LengthFromCurrToCode(Leng.BLy*Kfac.BKy,1);
     if(section.dl0 <=0.0){
         GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少计算长度数据."),Elemi);
         return FALSE;
     }
    //////////////////////////////////////////////////////////////////////////
    //get dPhi by [JTG D62--2004] Table 5.3.1 P33  
	 if (section.dr >1.0E-07){
		 section.dPhi = Get_Phi(2, section.dl0, 2*section.dr);
	 } else if (section.dbf1>1.0E-07 || section.dbf2 >1.0E-07 || section.dhf1>1.0E-07 || section.dhf2 >1.0E-07){
		 if (section.dA <1.0E-07) return FALSE;
 		 section.dPhi = Get_Phi(1, section.dl0, sqrt(min(m_dIyy,m_dIzz)/section.dA));// i=(I/A)^0.5，I=min(Iyy，Izz);
	 } else{
 		 section.dPhi = Get_Phi(0, section.dl0, min(section.db,section.dh));
	 }
    return TRUE; 
}

/************************************************************************/
/* get bending check's struct data                                      */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcCbcs(int Elemi,T_CBCS_CH& cbcs){
	cbcs.Initialize();
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	//Check Calculating Item.
    if(!Is_CalcItem(Elemi,0))  return FALSE;

	ElemPairK EPairK(Elemi, EN_EL_BEAM);

    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
	if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
	double dTol=1./pow(m_dZero,40);
	double dMyMax[2]={-dTol,-dTol}, dMyMin[2]={+dTol,+dTol};
	int iMaxDgnLcom[2]={0,0}, iMinDgnLcom[2]={0,0};
	int i=0, j=0;
	for(i=0; i<aForceLcom.GetSize(); i++)	{
		if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
		_DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
		for(j=0; j<2; j++){	// I,J.
			if(ForceCrc.dMuy[j] > dMyMax[j])	{dMyMax[j]=ForceCrc.dMuy[j]; iMaxDgnLcom[j]=i+1;}
			if(ForceCrc.dMuy[j] < dMyMin[j])	{dMyMin[j]=ForceCrc.dMuy[j]; iMinDgnLcom[j]=i+1;}
		}
	}
 	for(i=0; i<4; i++){		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		int iDgnLcomNo = (i%2==0 ? iMaxDgnLcom[i/2] : iMinDgnLcom[i/2]);
		CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
		double dMuy = (i%2==0 ? dMyMax[i/2] : dMyMin[i/2]);
		cbcs.cbcsBase[i].bCHK		= Get_ChkFlag(Elemi, i/2, 0);//0:moment check
		cbcs.cbcsBase[i].LcomK  = iDgnLcomNo;
        cbcs.cbcsBase[i].iMax   = (i%2 == 0 ? 9:10);//9:My-Max;10:My-Min
        cbcs.cbcsBase[i].dMuy   = dMuy;
        //Change CurrToCode...
        Get_CHRCCbcsBaseCurrToCode(cbcs.cbcsBase[i]);//Change UNIT
    }
    return TRUE;
}

/************************************************************************/
/* get shear check's struct data                                        */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcCsis(int Elemi,T_CSIS_CH& csis){
	csis.Initialize();
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	//Check Calculating Item.
    if(!Is_CalcItem(Elemi,1))  return FALSE;
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;

	ElemPairK EPairK(Elemi, EN_EL_BEAM);

	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
	if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
/*  ADGNFORCE aForceTnpr;
	if(!m_ForceCtrl->Get_LcomDataForDesign(Elemi, TRUE, FALSE, aForceLcom, aForceTnpr, iLcomNum)) return FALSE;
    if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
    if(aForceLcom.GetSize()!=aForceTnpr.GetSize()) return FALSE;
*/	double dTol=1./pow(m_dZero,40);
	double dFzMax[2]={-dTol,-dTol}, dFzMin[2]={+dTol,+dTol};
    double dFzMaxMy[2]={0,0}, dFzMinMy[2]={0,0};
    double dFzMaxMyTnpr[2]={0.,0.}, dFzMinMyTnpr[2]={0.,0.};
	int iMaxDgnLcom[2]={0,0}, iMinDgnLcom[2]={0,0};
	int i=0, j=0;  BOOL bis =TRUE;
	for(i=0; i<aForceLcom.GetSize(); i++)	{
		if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
		_DGN_FORC_CRC ForceCrc  = aForceLcom.GetAt(i);
//    _DGN_FORC_CRC ForceTnpr = aForceTnpr.GetAt(i);
		for(j=0; j<2; j++){	// I,J.
			if(ForceCrc.dFzz[j] > dFzMax[j]) {
                dFzMax[j]=ForceCrc.dFzz[j];        dFzMaxMy[j]=ForceCrc.dMuy[j];
                iMaxDgnLcom[j]=i+1;               // dFzMaxMyTnpr[j]=ForceTnpr.dMuy[j];
            }
			if(ForceCrc.dFzz[j] < dFzMin[j]) {
                dFzMin[j]=ForceCrc.dFzz[j];        dFzMinMy[j]=ForceCrc.dMuy[j];        
                iMinDgnLcom[j]=i+1;               // dFzMinMyTnpr[j]=ForceTnpr.dMuy[j];
            }
		}
		bis = FALSE;
	}
	if (bis) {	csis.Initialize(); return FALSE;	}

	for(i=0; i<4; i++){		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		int iDgnLcomNo = (i%2==0 ? iMaxDgnLcom[i/2] : iMinDgnLcom[i/2]);
		CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
//		if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
		double dFzz = (i%2==0 ? dFzMax[i/2] : dFzMin[i/2]);
        double dMuy = (i%2==0 ? dFzMaxMy[i/2] : dFzMinMy[i/2]);
        double dMuyTnpr = .0;//(i%2==0 ? dFzMaxMyTnpr[i/2] : dFzMinMyTnpr[i/2]);
        
		csis.CsisBase[i].bCHK	 = Get_ChkFlag(Elemi, i/2, 1);//shear check
		csis.CsisBase[i].LcomK = iDgnLcomNo;
		csis.CsisBase[i].iMax  = (i%2 == 0 ? 5:6);//1:Fz-Max;2:Fz-Min
        csis.CsisBase[i].dMu   = dMuy;
        csis.CsisBase[i].dMu_P = dMuyTnpr;
        csis.CsisBase[i].dVu   = dFzz;
        //Change CurrToCode...
        Get_CHRCCsisBaseCurrToCode(csis.CsisBase[i]);//Change UNIT
	}
    return TRUE;
}

/************************************************************************/
/* get torsion check's struct data                                      */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcCtss(int Elemi,T_CTSS_CH& ctss){
	ctss.Initialize();
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	//Check Calculating Item.
    if(!Is_CalcItem(Elemi,0))  return FALSE;

	ElemPairK EPairK(Elemi, EN_EL_BEAM);

 	ADGNFORCE aForceLcom;
    ADGNFORCE aForceTnpr;
    BOOL bDimType = TRUE;
    T_RC_D_CH temprcd;  
    if (Get_Rcd(temprcd)){ if (temprcd.iDimType ==0 ) bDimType = FALSE; }
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, TRUE, bDimType, aForceLcom, aForceTnpr, iLcomNum)) return FALSE;
    if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
    if(aForceLcom.GetSize()!=aForceTnpr.GetSize()) return FALSE;
	double dTol=1./pow(m_dZero,40);
	double dMxMaxT[2]={0.,0.}, dMxMaxV[2]={0.,0.};
	double dFzMaxT[2]={0.,0.}, dFzMaxV[2]={-dTol,-dTol};
	double dFzMinT[2]={0.,0.}, dFzMinV[2]={+dTol,+dTol};
    double dFzMaxMy[2]={0,0}, dFzMinMy[2]={0,0}, dMxMaxMy[2]={0.0};
    double dFzMaxMyTnpr[2]={0,0}, dFzMinMyTnpr[2]={0,0}, dMxMaxMyTnpr[2]={0.0};
	int iMxMaxDgnLcom[2]={0,0}, iFzMaxDgnLcom[2]={0,0}, iFzMinDgnLcom[2]={0,0};
	int i=0, j=0;
	for(i=0; i<aForceLcom.GetSize(); i++)	{
		if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
		_DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
        _DGN_FORC_CRC ForceTnpr = aForceTnpr.GetAt(i);
		for(j=0; j<2; j++){	// I,J.
			if(ForceCrc.dFzz[j] >= dFzMaxV[j]){
				dFzMaxT[j]=ForceCrc.dMux[j];				dFzMaxV[j]=ForceCrc.dFzz[j];
                dFzMaxMy[j]=ForceCrc.dMuy[j];				iFzMaxDgnLcom[j]=i+1;        
                dFzMaxMyTnpr[j]=ForceTnpr.dMuy[j];
			}
			if(ForceCrc.dFzz[j] <= dFzMinV[j])	{
				dFzMinT[j]=ForceCrc.dMux[j];				dFzMinV[j]=ForceCrc.dFzz[j];
                dFzMinMy[j]=ForceCrc.dMuy[j];				iFzMinDgnLcom[j]=i+1;
                dFzMinMyTnpr[j]=ForceTnpr.dMuy[j];
			}
			if(fabs(ForceCrc.dMux[j]) >= fabs(dMxMaxT[j]))	{
				dMxMaxT[j]=ForceCrc.dMux[j];				dMxMaxV[j]=ForceCrc.dFzz[j];
                dMxMaxMy[j]=ForceCrc.dMuy[j];				iMxMaxDgnLcom[j]=i+1;
                dMxMaxMyTnpr[j]=ForceTnpr.dMuy[j];
			}
		}
	}

    for(i=0; i<6; i++)	{		// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
		int iDgnLcomNo=0;
		double dMux=0.0, dFzz=0.0, dMuy=0.0, dMuyTnpr=0.0;
		if(i%3==0)			{iDgnLcomNo=iFzMaxDgnLcom[i/3]; dMux=dFzMaxT[i/3]; dFzz=dFzMaxV[i/3]; dMuy=dFzMaxMy[i/3]; dMuyTnpr=dFzMaxMyTnpr[i/3];}
		else if(i%3==1)	{iDgnLcomNo=iFzMinDgnLcom[i/3]; dMux=dFzMinT[i/3]; dFzz=dFzMinV[i/3]; dMuy=dFzMinMy[i/3]; dMuyTnpr=dFzMinMyTnpr[i/3];}
		else if(i%3==2)	{iDgnLcomNo=iMxMaxDgnLcom[i/3]; dMux=dMxMaxT[i/3]; dFzz=dMxMaxV[i/3]; dMuy=dMxMaxMy[i/3]; dMuyTnpr=dMxMaxMyTnpr[i/3];}
		else	ASSERT(0);
        CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
//		if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
		ctss.CtssBase[i].bCHK	= Get_ChkFlag(Elemi, i/3, 0);
		ctss.CtssBase[i].LcomK	= iDgnLcomNo;
		//ctss.CtssBase[i].iMax	= iMaxMinType;
        if     (i%3 ==0)  ctss.CtssBase[i].iMax = 5; /*5:Fz-Max(V-MAX)*/
        else if(i%3 ==1)	ctss.CtssBase[i].iMax = 6; /*5:Fz-Min(V-MIN)*/
        else if(i%3 ==2)	ctss.CtssBase[i].iMax = 7; /*5:Mx-Max(T-MAX)*/
		ctss.CtssBase[i].dTu	= dMux; 
		ctss.CtssBase[i].dVu	= dFzz;	
        ctss.CtssBase[i].dMu  = dMuy;
        //Change CurrToCode...
        Get_CHRCCtssBaseCurrToCode(ctss.CtssBase[i]);//Change UNIT
	}
    return TRUE;
}

/************************************************************************/
/* get crack width check's struct data. bisBeam: TRUE beam;FALSE column */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcCcws(int ElemK,BOOL bisBeam,T_CCWS_CH& ccws){
    //dFT放短期效应组合内力  dFb放对应的长期效应组合内力
    //dMsy-对应短期效应组合Ns的Msy(柱) dMsz-对应短期效应组合Ns的Msz(柱) 
    ccws.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	//Check Calculating Item.
    if(!Is_CalcItem(ElemK,0))  return FALSE;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

    // Set Data.
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
    ADGNFORCE aForceLcom;
    ADGNFORCE aForceTnpr;
    ADGNFORCE aForceTnsc;  
    if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, FALSE, FALSE, aForceLcom, aForceTnpr, aForceTnsc, iLcomNum)) return FALSE;
    if(m_ForceCtrl->Get_LcomNumForStrs()==0)	return FALSE;
    if(aForceLcom.GetSize()!=aForceTnpr.GetSize())	{ASSERT(0); return FALSE;}
    int i=0, j=0, k=0;

    double dFmaxI = -1./pow(m_dZero,40);  double dFminI = 1./pow(m_dZero,40);
    double dFmaxJ = -1./pow(m_dZero,40);  double dFminJ = 1./pow(m_dZero,40);
    double dMymaxI = -1./pow(m_dZero,40);  double dMyminI = 1./pow(m_dZero,40);
    double dMymaxJ = -1./pow(m_dZero,40);  double dMyminJ = 1./pow(m_dZero,40);
    double dMzmaxI = -1./pow(m_dZero,40);  double dMzminI = 1./pow(m_dZero,40);
    double dMzmaxJ = -1./pow(m_dZero,40);  double dMzminJ = 1./pow(m_dZero,40);
    double dF_IL[2] = {-1./pow(m_dZero,40), 1./pow(m_dZero,40)}; // Max,Min
	double dF_JL[2] = {-1./pow(m_dZero,40), 1./pow(m_dZero,40)}; // Max,Min

    int iFsMaxDgnLcom[2]={0,0}, iFsMinDgnLcom[2]={0,0};
    BOOL biIsLongTerm = FALSE, bjIsLongTerm = FALSE;
    //Long Term first...
    for(i=0; i<aForceLcom.GetSize(); i++)
    {
		int iDgnLcomNo = i+1;
		if(!m_ForceCtrl->Is_LcomForStrs(iDgnLcomNo))	continue;	// Only 沥惑荤侩(览仿八配矫.)
        if(m_ForceCtrl->Is_LcomForElst(iDgnLcomNo)) continue;
        if(!m_ForceCtrl->Is_LongTerm(iDgnLcomNo)) continue;  // Only Long Term.

        _DGN_FORC_CRC ForcLcom; ForcLcom.Initialize();
        ForcLcom = aForceLcom.GetAt(i);
        for(j=0; j<2; j++)
        {
            double dFtemp = 0.0;
            if (bisBeam) dFtemp = ForcLcom.dMuy[j];  //Except Tendon Primary,  Ms+Mp2
            else dFtemp = ForcLcom.dFxx[j];
            // End-I
            if(j==0)
            {
                if(dFtemp> dF_IL[0]) { dF_IL[0]=dFtemp; }//Bot
                if(dFtemp< dF_IL[1]) { dF_IL[1]=dFtemp; }//Top
				biIsLongTerm = TRUE;
            }
            // End-J
            else if(j==1)
            {
                if(dFtemp>dF_JL[0]) { dF_JL[0]=dFtemp; }//Bot
                if(dFtemp<dF_JL[1]) { dF_JL[1]=dFtemp; }//Top
				bjIsLongTerm = TRUE;
            }
        }  
    }
	     if (!biIsLongTerm){		dF_IL[0] = .0; dF_IL[1] = .0; }// Max,Min
	     if (!bjIsLongTerm){		dF_JL[0] = .0; dF_JL[1] = .0;	}// Max,Min

    //Not Long Term
	BOOL biIsShortTerm = FALSE,bjIsShortTerm = FALSE;
    for(i=0; i<aForceLcom.GetSize(); i++) {
        int iDgnLcomNo = i+1;
        if(!m_ForceCtrl->Is_LcomForStrs(iDgnLcomNo))	continue;	// Only 沥惑荤侩(览仿八配矫.)
        if(m_ForceCtrl->Is_LcomForElst(iDgnLcomNo)) continue;
        if(m_ForceCtrl->Is_LongTerm(iDgnLcomNo)) continue;  // not Long Term.
        _DGN_FORC_CRC Force; Force.Initialize();
        Force = aForceLcom.GetAt(i);
        for(j=0; j<2; j++)
        {
            double dMom = 0.0;
            if (bisBeam) dMom = Force.dMuy[j];  //Except Tendon Primary,Ms+Mp2
            else dMom = Force.dFxx[j];
            if(j==0)      {// End-I
                if(dMom>dFmaxI) { dFmaxI=dMom; iFsMaxDgnLcom[0] = i+1;dMymaxI=Force.dMuy[j];dMzmaxI=Force.dMuz[j];}
                if(dMom<dFminI) { dFminI=dMom; iFsMinDgnLcom[0] = i+1;dMyminI=Force.dMuy[j];dMzminI=Force.dMuz[j];}
				biIsShortTerm = TRUE;
            } else if(j==1)      {// End-J
                if(dMom>dFmaxJ) { dFmaxJ=dMom; iFsMaxDgnLcom[1] = i+1;dMymaxJ=Force.dMuy[j];dMzmaxJ=Force.dMuz[j];}
                if(dMom<dFminJ) { dFminJ=dMom; iFsMinDgnLcom[1] = i+1;dMyminJ=Force.dMuy[j];dMzminJ=Force.dMuz[j];}
				bjIsShortTerm = TRUE;
            }
        }  
    }
	if (!biIsShortTerm) {
		dFmaxI  = 0.0;  dFminI  = 0.0;	
		dMymaxI = 0.0;  dMyminI = 0.0;	
		dMzmaxI = 0.0;  dMzminI = 0.0;	
	}
    if (!bjIsShortTerm) {
		dFmaxJ  = 0.0;  dFminJ  = 0.0;
		dMymaxJ = 0.0;  dMyminJ = 0.0;
		dMzmaxJ = 0.0;  dMzminJ = 0.0;
	}
	
    for(i=0; i<4; i++)	{		// // 0=I-TOP_MAX, 1=I-BOT_MAX, 2=J-TOP_MAX, 3=J-BOT_MAX.
		int iDgnLcomNo=0;	double dFs=0.0, dFl=0.0;
//    if (i%2==0){  ccws.ccwsBase[i].dMsy = dMymaxI;   ccws.ccwsBase[i].dMsz = dMzmaxI;   }
//    else {        ccws.ccwsBase[i].dMsy = dMymaxJ;   ccws.ccwsBase[i].dMsz = dMzmaxJ;   }
        if (i==0 || i==2) {
            //if (bIsLongTerm)  dFl = dF_IL[1-i%2]; // Nega, Posi
            iDgnLcomNo=iFsMaxDgnLcom[i/2];
            if (i/2 ==0) dFs = dFmaxI; else  dFs = dFmaxJ;
            if (i/2 ==0) dFl = dF_IL[0]; else  dFl = dF_JL[0];
            if (i/2 ==0) ccws.ccwsBase[i].dMsy = dMymaxI; else  ccws.ccwsBase[i].dMsy = dMymaxJ;
        }
        else if(i==1 || i==3)	{
//      if (bIsLongTerm)  dFl = dF_JL[1-i%2]; 
            iDgnLcomNo=iFsMinDgnLcom[i/2];
            if (i/2 ==0) dFs = dFminI; else  dFs = dFminJ;
            if (i/2 ==0) dFl = dF_IL[1]; else  dFl = dF_JL[1];
            if (i/2 ==0) ccws.ccwsBase[i].dMsy = dMyminI; else  ccws.ccwsBase[i].dMsy = dMyminJ;
        }
        CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
//		if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
        ccws.ccwsBase[i].bCHK	= Get_ChkFlag(ElemK, i/2, 0);
		ccws.ccwsBase[i].LcomK	= iDgnLcomNo;
        ccws.ccwsBase[i].dFT = dFs;//短期荷载
        ccws.ccwsBase[i].dFB = dFl;//长期荷载

//		ccws.ccwsBase[i].iMax	= iMaxMinType;
        if(bisBeam){
            ccws.ccwsBase[i].iMax = (i%2 == 0 ? 9:10);//9:My-Max;10:My-Min
        }else{
            ccws.ccwsBase[i].iMax = (i%2 == 0 ? 1:2);//1:Fx-Max;2:Fx-Min
        }
        //Change CurrToCode...
        Get_CHRCCcwsBaseCurrToCode(ccws.ccwsBase[i],bisBeam);//Change UNIT
	}  
    return TRUE;
}

//get column ccws Force
BOOL CRCDataCtrl_CH::Get_CHRcColumnCcws(int ElemK,T_CCWS_CH_BASE ccwsBase[6])
{
    //dFT放短期效应组合内力 dFb放对应的长期效应组合内力
    //dMsy-对应短期效应组合Ns的Msy(柱) dMsz-对应短期效应组合Ns的Msz(柱)
 for(int i=0; i<6; i++)	ccwsBase[i].Initialize();
 CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
 //Check Calculating Item.
    if(!Is_CalcItem(ElemK,TRUE)) return FALSE;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

    // Set Data.
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
    ADGNFORCE aForceLcom;
    ADGNFORCE aForceTnpr;
    ADGNFORCE aForceTnsc;
    if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, FALSE, FALSE, aForceLcom, aForceTnpr, aForceTnsc, iLcomNum)) return FALSE;
    if(m_ForceCtrl->Get_LcomNumForStrs()==0)	return FALSE;
    if(aForceLcom.GetSize()!=aForceTnpr.GetSize())	{ASSERT(0); return FALSE;}
    int j=0, k=0;
 //not long term
    double dFminI = 1./pow(m_dZero,40); double dFminJ = 1./pow(m_dZero,40);//轴力最大时-Fx-min
    double dMymaxI = -1./pow(m_dZero,40); double dMyminI = 1./pow(m_dZero,40);
    double dMymaxJ = -1./pow(m_dZero,40); double dMyminJ = 1./pow(m_dZero,40);
    double dMymaxFI = .0; double dMyminFI = .0;
    double dMymaxFJ = .0; double dMyminFJ = .0;
    double dMyI = .0, dMyJ = .0;
    double dMyMax_F[2] = { 0,0 };
    double dMyMin_F[2] = { 0,0 };
    //Long term
    double dMy_IL[2] = { -1./pow(m_dZero,40), 1./pow(m_dZero,40) }; // My-Max,Min
    double dMy_JL[2] = { -1./pow(m_dZero,40), 1./pow(m_dZero,40) }; // My-Max,Min
    double dFx_IL = 1./pow(m_dZero, 40), dFx_JL = 1./pow(m_dZero, 40);//轴力最大时-Fx-min
    double dMy_IL_F[2] = { 0,0 };
    double dMy_JL_F[2] = { 0,0 };
    int iFsMinDgnLcom[2]={0,0}, iMyMaxDgnLcom[2]={0,0}, iMyMinDgnLcom[2]={0,0};
 
    BOOL bIsLongTerm = FALSE;
    //Long Term first...
    for(int i=0; i<aForceLcom.GetSize(); i++)
    {
    int iDgnLcomNo = i+1;
    if(!m_ForceCtrl->Is_LcomForStrs(iDgnLcomNo))	continue;	// Only 沥惑荤侩(览仿八配矫.)
        if(m_ForceCtrl->Is_LcomForElst(iDgnLcomNo)) continue;
        if(!m_ForceCtrl->Is_LongTerm(iDgnLcomNo)) continue; // Only Long Term.
        _DGN_FORC_CRC ForcLcom; ForcLcom.Initialize();
        ForcLcom = aForceLcom.GetAt(i);
        for(j=0; j<2; j++)
        {
            double dMtemp = ForcLcom.dMuy[j];
     double dFtemp = ForcLcom.dFxx[j];
            // End-I
            if(j==0)
            {
                if(dMtemp> dMy_IL[0]) { dMy_IL[0]=dMtemp;dMy_IL_F[0] =dFtemp; }//Bot
                if(dMtemp< dMy_IL[1]) { dMy_IL[1]=dMtemp;dMy_IL_F[1] =dFtemp; }//Top
        if(dFtemp< dFx_IL ) { dFx_IL=dFtemp; }//Top
            }
            // End-J
            else if(j==1)
            {
                if(dMtemp>dMy_JL[0]) { dMy_JL[0]=dMtemp;dMy_JL_F[0] =dFtemp; }//Bot
                if(dMtemp<dMy_JL[1]) { dMy_JL[1]=dMtemp;dMy_JL_F[1] =dFtemp; }//Top
                if(dFtemp<dFx_JL ) { dFx_JL=dFtemp; }//Top
            }
        }
        bIsLongTerm = TRUE;
    }
 if (!bIsLongTerm) {
    dMy_IL[0] = .0; dMy_IL[1] = .0;// Max,Min
    dMy_JL[0] = .0; dMy_JL[1] = .0;// Max,Min
    dFx_IL = dFx_JL = .0;
 }
    //Not Long Term
 BOOL bIsShortTerm = FALSE;
    for(int i=0; i<aForceLcom.GetSize(); i++) {
        int iDgnLcomNo = i+1;
        if(!m_ForceCtrl->Is_LcomForStrs(iDgnLcomNo))	continue;	// Only 沥惑荤侩(览仿八配矫.)
        if(m_ForceCtrl->Is_LcomForElst(iDgnLcomNo)) continue;
     // if(m_ForceCtrl->Is_LongTerm(iDgnLcomNo)) continue; // not Long Term. cancel this line by gongxing at 2016-08-25
        _DGN_FORC_CRC Force; Force.Initialize();
        Force = aForceLcom.GetAt(i);
        for(j=0; j<2; j++)
        {
            double dMom = Force.dMuy[j]; //Except Tendon Primary,Ms+Mp2
     double dFx = Force.dFxx[j];
            if(j==0) {// End-I
                if(dMom>dMymaxI) { dMymaxFI=Force.dMuy[j]; iMyMaxDgnLcom[0] = i+1;dMymaxI=Force.dMuy[j];dMyMax_F[0] = dFx;}
		if(dMom<dMyminI) { dMyminFI=Force.dMuy[j]; iMyMinDgnLcom[0] = i+1;dMyminI=Force.dMuy[j];dMyMin_F[0] = dFx;}
		if(dFx <= dFminI) //modified this statement by gongxing at 2016-08-31
		{ 
			dFminI=Force.dFxx[j]; 
			if(fabs(dMom) >= fabs(dMyI))
			{
				iFsMinDgnLcom[0] = i+1;
				dMyI=dMom;  
			} 
		}
            } else if(j==1) {// End-J
                if(dMom>dMymaxJ) { dMymaxFJ=Force.dMuy[j]; iMyMaxDgnLcom[1] = i+1;dMymaxJ=Force.dMuy[j];dMyMax_F[1] = dFx;}
                if(dMom<dMyminJ) { dMyminFJ=Force.dMuy[j]; iMyMinDgnLcom[1] = i+1;dMyminJ=Force.dMuy[j];dMyMin_F[1] = dFx;}
		if(dFx <= dFminJ) //modified this statement by gongxing at 2016-08-31
		{ 
			dFminJ=Force.dFxx[j]; 
			if(fabs(dMom) >= fabs(dMyJ))
			{
				iFsMinDgnLcom[1] = i+1;
				dMyJ=dMom; 
			}
		}
            }
        }
    bIsShortTerm = TRUE;
    }
 if (!bIsShortTerm) {
    dFminI = dFminJ = .0;
    dMymaxI = dMyminI = .0;
    dMymaxJ = dMyminJ = .0;
 }
    for(int i=0; i<6; i++)	{	 // // 0=I-TOP_MAX, 1=I-BOT_MAX, 2=J-TOP_MAX, 3=J-BOT_MAX.
        if (i==0 || i==3) {//Fx-min
            ccwsBase[i].LcomK=iFsMinDgnLcom[i/3];
            if (i/3 ==0) ccwsBase[i].dFT = dFminI; else ccwsBase[i].dFT = dFminJ;
            if (i/3 ==0) ccwsBase[i].dFB = dFx_IL; else ccwsBase[i].dFB = dFx_JL;
            if (i/3 ==0) ccwsBase[i].dMsy = dMyI; else ccwsBase[i].dMsy = dMyJ;;
            ccwsBase[i].iMax = 2;//2:Fx-Min
        }
        else if(i==1 || i==4)	{//My-max
            ccwsBase[i].LcomK=iMyMaxDgnLcom[i/3];
            if (i/3 ==0) ccwsBase[i].dFT = dMyMax_F[0]; else ccwsBase[i].dFT = dMyMax_F[1];
            if (i/3 ==0) ccwsBase[i].dFB = dMy_IL_F[0];else ccwsBase[i].dFB = dMy_JL_F[0];
            if (i/3 ==0) ccwsBase[i].dMsy = dMymaxI; else ccwsBase[i].dMsy = dMymaxJ;
            ccwsBase[i].iMax = 9;//9:My-Max
        }
        else if(i==2 || i==5)	{//My-min
            ccwsBase[i].LcomK=iMyMinDgnLcom[i/3];
            if (i/3 ==0) ccwsBase[i].dFT = dMyMin_F[0]; else ccwsBase[i].dFT = dMyMin_F[1];
            if (i/3 ==0) ccwsBase[i].dFB = dMy_IL_F[1]; else ccwsBase[i].dFB = dMy_JL_F[1];
            if (i/3 ==0) ccwsBase[i].dMsy = dMyminI; else ccwsBase[i].dMsy = dMyminJ;
            ccwsBase[i].iMax = 10;//10:My-Min
        }
    //Change CurrToCode...
    Get_CHRCCcwsBaseCurrToCode(ccwsBase[i],FALSE);//Change UNIT
        ccwsBase[i].bCHK	= Get_ChkFlag(ElemK, i/3, TRUE);
 }
    CString strout = _T("");
    //strout.Format(_T("FTMin_i:%g-FBMin_i:%g-FTMax_j:%g-FtMax_j:%g"),dMyminFI,dMy_IL[1],dMyminFJ,dMy_JL[1]);
 return TRUE;
}

/************************************************************************/
/* get compressive/tensile check's struct data. 
     isCscs: TRUE compressive; FALSE tensile(TRUE-抗压;FALSE-抗拉)        */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcCstcs(int Elemi,BOOL isCscs,T_CSTCS_CH& cstcs){
	cstcs.Initialize();
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	//Check Calculating Item.
    if(!Is_CalcItem(Elemi,2))  return FALSE;

	ElemPairK EPairK(Elemi, EN_EL_BEAM);

    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
	if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
	double dTol=1./pow(m_dZero,40);
	double dMyMax[2]={-dTol,-dTol}, dMyMin[2]={+dTol,+dTol},dMyMaxN[2]={0.0,0.0}, dMyMinN[2]={0.0,0.0};
    double dNxMax[2]={-dTol,-dTol},dNxMaxMy[2]={0.0,0.0},dNxMaxMz[2]={0.0,0.0};
    double dNxMin[2]={+dTol,+dTol},dNxMinMy[2]={0.0,0.0},dNxMinMz[2]={0.0,0.0};
    int iMyMaxDgnLcom[2]={0,0},iMyMinDgnLcom[2]={0,0},iNxMaxDgnLcom[2]={0,0}, iNxMinDgnLcom[2]={0,0};
    int i=0, j=0;
	for(i=0; i<aForceLcom.GetSize(); i++)	{
		if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
		_DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
		for(j=0; j<2; j++){	// I,J.
			if(ForceCrc.dMuy[j] > dMyMax[j])	{
                dMyMax[j]=ForceCrc.dMuy[j]; dMyMaxN[j] = ForceCrc.dFxx[j];
                iMyMaxDgnLcom[j]=i+1;
            }
			if(ForceCrc.dMuy[j] < dMyMin[j])	{
                dMyMin[j]=ForceCrc.dMuy[j]; dMyMinN[j] = ForceCrc.dFxx[j];
                iMyMinDgnLcom[j]=i+1;
            }
            if (ForceCrc.dFxx[j] > dNxMax[j]) {
                dNxMax[j] =  ForceCrc.dFxx[j]; dNxMaxMy[j] = ForceCrc.dMuy[j];
                dNxMaxMz[j] = ForceCrc.dMuz[j];  iNxMaxDgnLcom[j]=i+1;
            }
            if (ForceCrc.dFxx[j] < dNxMin[j]) {
                dNxMin[j] =  ForceCrc.dFxx[j]; dNxMinMy[j] = ForceCrc.dMuy[j];
                dNxMinMz[j] = ForceCrc.dMuz[j];  iNxMinDgnLcom[j]=i+1;
            }
		}
	}
 	for(i=0; i<10; i++){		//0=轴心,1/2-Fx组内力的偏心,3/4-My组内力的偏心.(0-4:i截面; 5-9:j截面)
        if (i%5==0) {
            if (isCscs) {
                cstcs.cstcsBase[i].dNd = dNxMin[i/5]; cstcs.cstcsBase[i].LcomK =iNxMinDgnLcom[i/5];
                cstcs.cstcsBase[i].iMax = 2;//2:Fx-Min; 
            }
            else{
                cstcs.cstcsBase[i].dNd = dNxMax[i/5]; cstcs.cstcsBase[i].LcomK =iNxMaxDgnLcom[i/5];
                cstcs.cstcsBase[i].iMax = 1;//1:Fx-Max
            }
        }
        if (i%5==1) {
            if (isCscs) { 
                cstcs.cstcsBase[i].dNd = dNxMin[i/5]; cstcs.cstcsBase[i].LcomK =iNxMinDgnLcom[i/5];
                cstcs.cstcsBase[i].dMd = dNxMinMy[i/5];  cstcs.cstcsBase[i].iMax = 2;//2:Fx-Min; 
            }
            else{ 
                cstcs.cstcsBase[i].dNd = dNxMax[i/5]; cstcs.cstcsBase[i].LcomK =iNxMaxDgnLcom[i/5];
                cstcs.cstcsBase[i].dMd = dNxMaxMy[i/5]; cstcs.cstcsBase[i].iMax = 1;//1:Fx-Max
            }
        }
        if (i%5==2) {
            if (isCscs) { 
                cstcs.cstcsBase[i].dNd = dNxMin[i/5]; cstcs.cstcsBase[i].LcomK =iNxMinDgnLcom[i/5];
                cstcs.cstcsBase[i].dMd = dNxMinMz[i/5];   cstcs.cstcsBase[i].iMax = 2;//2:Fx-Min; 
            }
            else{ 
                cstcs.cstcsBase[i].dNd = dNxMax[i/5]; cstcs.cstcsBase[i].LcomK =iNxMaxDgnLcom[i/5];
                cstcs.cstcsBase[i].dMd =  dNxMaxMz[i/5];  cstcs.cstcsBase[i].iMax = 1;//1:Fx-Max
            }
        }
        if (i%5==3) {
            cstcs.cstcsBase[i].dMd = dMyMax[i/5];cstcs.cstcsBase[i].LcomK =iMyMaxDgnLcom[i/5];
            cstcs.cstcsBase[i].dNd = dMyMaxN[i/5];  cstcs.cstcsBase[i].iMax = 9;//9:My-max
        }
        if (i%5==4) {      
            cstcs.cstcsBase[i].dMd = dMyMin[i/5];cstcs.cstcsBase[i].LcomK =iMyMinDgnLcom[i/5];
            cstcs.cstcsBase[i].dNd = dMyMinN[i/5];  cstcs.cstcsBase[i].iMax = 10;//10:My-min
        }
        cstcs.cstcsBase[i].bCHK	= Get_ChkFlag(Elemi, i/5, 2);//2:轴力
        if (isCscs) { cstcs.cstcsBase[i].dNd = -cstcs.cstcsBase[i].dNd; }
        //Change CurrToCode...
        Get_CHRCCstcsBaseCurrToCode(cstcs.cstcsBase[i]);//Change UNIT
	}
    return TRUE;
}

/************************************************************************/
/* get cross section stress check's struct data.(施工阶段正截面法向应力)*/
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CrcCscc(int ElemK, T_CSCC_CH& cscc){
    cscc.Initialize();
    if(!Is_CalcItem(ElemK,0))  return FALSE;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	_DGN_STRS_CRC StrsMax; StrsMax.Initialize();
	_DGN_STRS_CRC StrsMin; StrsMin.Initialize();
	CArray<T_STAG_K,T_STAG_K> aStagK;
    CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
	if(aStagK.GetSize()==0)	return FALSE;
	int i=0;
	for(i=0; i<aStagK.GetSize(); i++)
	{
        T_STAG_K StagK = aStagK.GetAt(i);
		int iFinalStepNo=0;
		// Get Last StepNo at Current Stage.
		Get_LastStepInCurStage(StagK, iFinalStepNo);
		CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
		T_SGLD_K SgldK = pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
		if(!m_ForceCtrl->Get_StrsBySgld(EPairK, SgldK, StagK, iFinalStepNo, StrsMax, StrsMin))	continue;
	}
    // Set Data.
	for(i=0; i<4; i++)
	{
		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
 		_DGN_STRS_CRC StrsCrc = (i%2==0 ? StrsMax : StrsMin);
		if(StrsCrc.dScom[i/2]==0.0)	continue;
// 		cscc.CsccBase[i].bCHK	= TRUE;//Get_ChkFlag(ElemK, i/2, TRUE);
        cscc.CsccBase[i].bCHK	= Get_ChkFlag(ElemK, i/2, 0);
		cscc.CsccBase[i].StagK	= StrsCrc.iRefNo[i/2];
		cscc.CsccBase[i].dFT		= StrsCrc.dStop[i/2];
		cscc.CsccBase[i].dFB		= StrsCrc.dSbot[i/2];
		cscc.CsccBase[i].dFTL	= StrsCrc.dStl[i/2];
		cscc.CsccBase[i].dFBL	= StrsCrc.dSbl[i/2];
		cscc.CsccBase[i].dFTR	= StrsCrc.dStr[i/2];
		cscc.CsccBase[i].dFBR	= StrsCrc.dSbr[i/2];
        cscc.CsccBase[i].dFMAX = StrsCrc.dScom[i/2];
        cscc.CsccBase[i].dALW	= 0.0;	// From CRC.
        //Change CurrToCode...
        Get_CHRCCsccBaseCurrToCode(cscc.CsccBase[i]);//Change UNIT
	}
	return TRUE;
}

/************************************************************************/
/* get rebars stress check's struct data.(受拉钢筋应力验算)             */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CrcCtst(int ElemK,int sectioni,int iStaZref ,double dh,double dStaz, double dEs,double dEc,T_CTST_CH& ctst){
    ctst.Initialize();
    if(!Is_CalcItem(ElemK,0))  return FALSE;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	_DGN_STRS_CRC StrsMax; StrsMax.Initialize();
	_DGN_STRS_CRC StrsMin; StrsMin.Initialize();
	CArray<T_STAG_K,T_STAG_K> aStagK;
    CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
	if(aStagK.GetSize()==0)	return FALSE;
	for(int i=0; i<aStagK.GetSize(); i++)
	{
        T_STAG_K StagK = aStagK.GetAt(i);
		int iFinalStepNo=0;
		// Get Last StepNo at Current Stage.
		Get_LastStepInCurStage(StagK, iFinalStepNo);
		CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
		T_SGLD_K SgldK = pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
		if(!m_ForceCtrl->Get_StrsBySgld(EPairK, SgldK, StagK, iFinalStepNo, StrsMax, StrsMin))	continue;
	}
    // Set Data.  内插得到钢筋的力
    double dtop = (StrsMin.dStl[sectioni] + StrsMin.dStr[sectioni])/2.0;
    double dbot = (StrsMin.dSbl[sectioni] + StrsMin.dSbr[sectioni])/2.0;
    //if (rbar.iStaZRef ==0) continue;//0:top; 1:bot
    if (iStaZref == 1) 
        ctst.dDLL = (dh - fabs(Cng_LengthFromCurrToCode(dStaz,1)))*(dbot-dtop)/dh+dtop;
    else
        ctst.dDLL = (dh - fabs(Cng_LengthFromCurrToCode(dStaz,1)))*(dtop-dbot)/dh+dbot;
    ctst.dDLL = -ctst.dDLL*dEs/dEc;

    //弹性阶段验算组合下混凝土应力反算钢筋应力
// 	ADGNFORCE aForceLcom;
//	if(!m_ForceCtrl->Get_LcomDataForDesign(ElemK, TRUE, FALSE, aForceLcom))	return FALSE;
//	if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
	ADGNFORCE aForceLcom;
	ADGNSTRSADD aStressAdd;
    ADGNSHRSADD aStssLcom;
	if(!m_ForceCtrl->Get_LcomDataForStress(EPairK, FALSE, aForceLcom, aStressAdd, aStssLcom, iLcomNum))	return FALSE;
    double dFll = 0.0;
	for(int i=0; i<aForceLcom.GetSize(); i++)	{
		if(!m_ForceCtrl->Is_LcomForElst(i+1))  continue; // is Serviceability and Elasticity.
        _DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
        _DGN_STRS_ADD StrsAdd /*; StrsAdd.Initialize();*/ = aStressAdd.GetAt(i);//温度应力
        StrsMax.Initialize(); StrsMin.Initialize();
        if (m_ForceCtrl->Get_StrsByForc(EPairK, i+1, ForceCrc, StrsAdd, StrsMax, StrsMin)){
            double dftop = -StrsMax.dStop[sectioni];
            double dfbot = -StrsMax.dSbot[sectioni];
            double dfll = 0.0;
            if (iStaZref ==1) 
                dfll = (dh - fabs(Cng_LengthFromCurrToCode(dStaz,1)))*(dfbot-dftop)/dh+dftop;
            else
                dfll = (dh - fabs(Cng_LengthFromCurrToCode(dStaz,1)))*(dftop-dfbot)/dh+dfbot;
            dfll = dfll*dEs/dEc;
            if ((fabs(dfll) > fabs(dFll)) && (fabs(dfll) > 1.0E-07) ) {dFll = dfll;}    
        }
	}
    ctst.dFLL = dFll;
    //Change CurrToCode...
    Get_CHRCCtstBaseCurrToCode(ctst);//Change UNIT
	return TRUE;
}

/************************************************************************/
/* get axis tensile stress check's struct data.(施工阶段中性轴主拉应力) */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CrcCsnc(int ElemK, T_CSNC_CH& csnc){
    csnc.Initialize(); 
	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
    if(!Is_CalcItem(ElemK,0))  return FALSE;
	//+++++++++++++++++++++++++++++++++
	CArray<T_STAG_K,T_STAG_K> aStagK;
	pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
	if(aStagK.GetSize()==0)	return FALSE;
	for (int i=0;i<4;i++) {
		int j=(i==0||i==1)?0:1;
		double dSig_Max = -1./pow(m_dZero,40);
		double dSig_Min = 1./pow(m_dZero,40);
		int iStagK = 0; T_STSS_D stss;
		for(int il=0; il<aStagK.GetSize(); il++){
	    T_STAG_K StagK = aStagK.GetAt(il);
			int iFinalStepNo=0;
			// Get Last StepNo at Current Stage.
			Get_LastStepInCurStage(StagK, iFinalStepNo);
			CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
			T_SGLD_K SgldK = pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);

		// Get Force Data by Load Case.
		T_LCOM_D LcasD;
		LcasD.Initialize();
		if(!pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, SgldK, LcasD))	{ASSERT(0); return FALSE;}
		if(iFinalStepNo > 0)	pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD,iFinalStepNo,2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
		else						pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);

			T_STSS_D t_stss;
			m_pDoc->m_pPostCtrl->GetStssNew(ElemK, 0, 0, &t_stss, 2);//2:主应力
			for (int kj=6;kj<8;kj++) {
				if (i==1 || i==3) {
					if (fabs(t_stss.dblStress[4*j][kj][6] + t_stss.dblStress[4*j][kj][7]) > dSig_Max) {
						dSig_Max = (t_stss.dblStress[4*j][kj][6] + t_stss.dblStress[4*j][kj][7]);
						iStagK = StagK; stss = t_stss;
					}
				}else{//i=1;i=3
					if (fabs(t_stss.dblStress[4*j][kj][6] + t_stss.dblStress[4*j][kj][7]) < dSig_Min) {
						dSig_Min = (t_stss.dblStress[4*j][kj][6] + t_stss.dblStress[4*j][kj][7]);
						iStagK = StagK; stss = t_stss;
					}
				}
			}//kj
		}
 		csnc.CsncBase[i].bCHK	= Get_ChkFlag(ElemK, i/2, 0);
		csnc.CsncBase[i].StagK= iStagK;//LcomK!!!!
        csnc.CsncBase[i].dFML = stss.dblStress[4*j][6][6] + stss.dblStress[4*j][6][7];//9/10位置点改为 7/8位置点	
        csnc.CsncBase[i].dFMR	= stss.dblStress[4*j][7][6] + stss.dblStress[4*j][7][7];	
        csnc.CsncBase[i].dFM	= (csnc.CsncBase[i].dFML +csnc.CsncBase[i].dFMR)/2.0;	
        csnc.CsncBase[i].dFMAX= fabs(csnc.CsncBase[i].dFML)>fabs(csnc.CsncBase[i].dFMR)? csnc.CsncBase[i].dFML: csnc.CsncBase[i].dFMR;//max(csnc.CsncBase[i].dFML,csnc.CsncBase[i].dFMR);// ShrsJtg.dSspAbs[i/2];	//Max,Min   
        //Change CurrToCode...
        Get_CHRCCsncBaseCurrToCode(csnc.CsncBase[i]);//Change UNIT
	}
	return TRUE;
}

/************************************************************************/
/* get beam's main-rebar estimate struct data.(梁纵向钢筋量估算)        */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcBrlr(int Elemi,T_BRLR_CH& brlr){
	brlr.Initialize();
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	//Check Calculating Item.
    if(!Is_CalcItem(Elemi,0))  return FALSE;

	ElemPairK EPairK(Elemi, EN_EL_BEAM);

    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
	if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
	double dTol=1./pow(m_dZero,40);
	double dMyMax[2]={-dTol,-dTol}, dMyMin[2]={+dTol,+dTol};
	int iMaxDgnLcom[2]={0,0}, iMinDgnLcom[2]={0,0};
	int i=0, j=0;
	for(i=0; i<aForceLcom.GetSize(); i++)	{
		if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
		_DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
		for(j=0; j<2; j++){	// I,J.
			if(ForceCrc.dMuy[j] > dMyMax[j])	{dMyMax[j]=ForceCrc.dMuy[j]; iMaxDgnLcom[j]=i+1;}
			if(ForceCrc.dMuy[j] < dMyMin[j])	{dMyMin[j]=ForceCrc.dMuy[j]; iMinDgnLcom[j]=i+1;}
		}
	}
 	for(i=0; i<4; i++){		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		int iDgnLcomNo = (i%2==0 ? iMaxDgnLcom[i/2] : iMinDgnLcom[i/2]);
		CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
//		if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
		double dMuy = (i%2==0 ? dMyMax[i/2] : dMyMin[i/2]);
		brlr.BrlrBase[i].bCHK		= Get_ChkFlag(Elemi, i/2, 0);
		brlr.BrlrBase[i].LcomK	  = iDgnLcomNo;
//		brlr.BrlrBase[i].iMax		= iMaxMinType;
        brlr.BrlrBase[i].iMax =(i%2 == 0 ? 9:10);//9:My-Max;10:My-Min
		brlr.BrlrBase[i].dMd		= dMuy;
        //Change CurrToCode...
        Get_CHRCBrlrBaseCurrToCode(brlr.BrlrBase[i]);//Change UNIT
	}
    return TRUE;
}

/************************************************************************/
/* get beam's shear-rebar estimate struct data.(梁箍筋钢筋用量估算)     */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcBrcr(int Elemi,T_BRCR_CH& brcr){
	brcr.Initialize();
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	//Check Calculating Item.
    if(!Is_CalcItem(Elemi,0))  return FALSE;

	ElemPairK EPairK(Elemi, EN_EL_BEAM);

    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
    ADGNFORCE aForceTnpr;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, TRUE, FALSE, aForceLcom, aForceTnpr, iLcomNum)) return FALSE;
    if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
    if(aForceLcom.GetSize()!=aForceTnpr.GetSize()) return FALSE;
	double dTol=1./pow(m_dZero,40);
	double dFzMax[2]={-dTol,-dTol}, dFzMin[2]={+dTol,+dTol};
 	int iMaxDgnLcom[2]={0,0}, iMinDgnLcom[2]={0,0};
	int i=0, j=0;
	for(i=0; i<aForceLcom.GetSize(); i++)	{
		if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
		_DGN_FORC_CRC ForceCrc  = aForceLcom.GetAt(i);
        _DGN_FORC_CRC ForceTnpr = aForceTnpr.GetAt(i);
		for(j=0; j<2; j++){	// I,J.
			if(ForceCrc.dFzz[j] > dFzMax[j]) { dFzMax[j]=ForceCrc.dFzz[j]; iMaxDgnLcom[j]=i+1; }
            if(ForceCrc.dFzz[j] < dFzMin[j]) { dFzMin[j]=ForceCrc.dFzz[j]; iMinDgnLcom[j]=i+1; }
		}
	}
	for(i=0; i<4; i++){		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		int iDgnLcomNo = (i%2==0 ? iMaxDgnLcom[i/2] : iMinDgnLcom[i/2]);
		CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
//		if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
		double dFzz = (i%2==0 ? dFzMax[i/2] : dFzMin[i/2]);
 		brcr.BrcrBase[i].bCHK	= Get_ChkFlag(Elemi, i/2, 0);
		brcr.BrcrBase[i].LcomK	= iDgnLcomNo;
        brcr.BrcrBase[i].iMax = (i%2 == 0 ? 5:6);//5:Fz-Max;6:Fz-Min
		brcr.BrcrBase[i].dVd		= dFzz;
        //Change CurrToCode...
        Get_CHRCBrcrBaseCurrToCode(brcr.BrcrBase[i]);//Change UNIT
 	}
    return TRUE;
}

/************************************************************************/
/* get column's main-rebar estimate struct data.(柱纵向钢筋量估算)      */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CHRcCrlr(int Elemi,T_CRLR_CH& crlr){
	crlr.Initialize();
    CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
 //Check Calculating Item.
    if(!Is_CalcItem(Elemi,0)) return FALSE;

	ElemPairK EPairK(Elemi, EN_EL_BEAM);

    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
    ADGNFORCE aForceLcom;
    ADGNFORCE aForceTnpr;
 if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, TRUE, FALSE, aForceLcom, aForceTnpr, iLcomNum))	return FALSE;
 if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
 double dTol=1./pow(m_dZero,40);
 double dNdMax[2]={0,0}, dNdMin[2]={+dTol,+dTol};
    double dNdMy_Max[2]={0.0,0.0}, dNdMz_Max[2]={0.0,0.0};
    double dNdMy_Min[2]={0.0,0.0}, dNdMz_Min[2]={0.0,0.0};
 int iMaxDgnLcom[2]={0,0}, iMinDgnLcom[2]={0,0};
 int i=0, j=0;
 for(i=0; i<aForceLcom.GetSize(); i++)	{
    if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
    _DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
    for(j=0; j<2; j++){	// I,J.
     if(fabs(ForceCrc.dFxx[j]) > fabs(dNdMax[j]))	{
                dNdMax[j]=ForceCrc.dFxx[j]; iMaxDgnLcom[j]=i+1;
                dNdMy_Max[j] =ForceCrc.dMuy[j]; dNdMz_Max[j] = ForceCrc.dMuz[j];
            }
     if(fabs(ForceCrc.dFxx[j]) < fabs(dNdMin[j]))	{
                dNdMin[j]=ForceCrc.dFxx[j]; iMinDgnLcom[j]=i+1;
                dNdMy_Min[j] =ForceCrc.dMuy[j]; dNdMz_Min[j] = ForceCrc.dMuz[j];
            }
    }
 }
    for(i=0; i<4; i++){	 // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
int iDgnLcomNo = (i%2==0 ? iMaxDgnLcom[i/2] : iMinDgnLcom[i/2]);
		CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
		//		if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
		//    double dmaxNd = fabs(dNdMax[i/2]) >= fabs(dNdMin[i/2]) ? dNdMax[i/2] : dNdMin[i/2];
		//    double dminNd = fabs(dNdMax[i/2]) < fabs(dNdMin[i/2]) ? dNdMax[i/2] : dNdMin[i/2];
		
		double dNd = (i%2==0 ? dNdMax[i/2] : dNdMin[i/2]);
		crlr.CrlrBase[i].bCHK = Get_ChkFlag(Elemi, i/2, 0);
		crlr.CrlrBase[i].LcomK = iDgnLcomNo;
		crlr.CrlrBase[i].dNd = dNd ;
		crlr.CrlrBase[i].dMyd = (i%2==0 ? dNdMy_Max[i/2] : dNdMy_Min[i/2]);
		crlr.CrlrBase[i].dMzd = (i%2==0 ? dNdMz_Max[i/2] : dNdMz_Min[i/2]);
		//Change CurrToCode...
		Get_CHRCCrlrBaseCurrToCode(crlr.CrlrBase[i]);//Change UNIT
    }
    return TRUE;
}

/************************************************************************/
/* Solve Quadratic which as ax^2+bx+c=0.
     if the Quadratic have no solution,return 0.0;else return Max(x1,x2). */
/************************************************************************/
double CRCDataCtrl_CH::Solve_Quadratic(double a,double b,double c){
    double dx1 = 0.0, dx2 = 0.0;
    if (a==0.0){
        if (b==0.0) return 0.0;
        else return -1.0*c/b;
    }
    if (b*b-4*a*c < 0.0) return 0.0;
    dx1 = (-1.0*b+sqrt(b*b-4*a*c))/2.0/a;
    dx2 = (-1.0*b-sqrt(b*b-4*a*c))/2.0/a;
    if (dx1>0.0 && dx2 >0.0) return min(dx1,dx2);
    else  return max(dx1,dx2);
}


BOOL CRCDataCtrl_CH::Get_LcomDataMap(CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K>& aLcomK)
{
	return m_ForceCtrl->Get_LcomDataMap(aLcomK);
}

double CRCDataCtrl_CH::Cng_LengthFromCurrToCode(double dLength, int iUp)
{
	double dLenCode=0.0;
	switch(m_CodeUnit.nBase_Length)
	{
		case D_UNITSYS_LENGTH_INDEX_M:	{dLenCode=1.0;		break;}
		case D_UNITSYS_LENGTH_INDEX_CM:	{dLenCode=0.01;		break;}
		case D_UNITSYS_LENGTH_INDEX_MM:	{dLenCode=0.001;	break;}
		case D_UNITSYS_LENGTH_INDEX_IN:	{dLenCode=0.0254;	break;}
		case D_UNITSYS_LENGTH_INDEX_FT:	{dLenCode=0.3048;	break;}
		default:	ASSERT(0);
	}
	double dLenCurr=0.0;
	switch(m_CurrUnit.nBase_Length)
	{
		case D_UNITSYS_LENGTH_INDEX_M:	{dLenCurr=1.0;		break;}
		case D_UNITSYS_LENGTH_INDEX_CM:	{dLenCurr=0.01;		break;}
		case D_UNITSYS_LENGTH_INDEX_MM:	{dLenCurr=0.001;	break;}
		case D_UNITSYS_LENGTH_INDEX_IN:	{dLenCurr=0.0254;	break;}
		case D_UNITSYS_LENGTH_INDEX_FT:	{dLenCurr=0.3048;	break;}
		default:	ASSERT(0);
	}
	return dLength * pow(dLenCurr/dLenCode, iUp);
}

double CRCDataCtrl_CH::Cng_ForceFromCurrToCode(double dForce)
{
	double dForCode=0.0;
	switch(m_CodeUnit.nBase_Force)
	{
		case D_UNITSYS_FORCE_INDEX_KG:	{dForCode=9.80665;	break;}
		case D_UNITSYS_FORCE_INDEX_TON:	{dForCode=9806.65;	break;}
		case D_UNITSYS_FORCE_INDEX_N:		{dForCode=1.0;			break;}
		case D_UNITSYS_FORCE_INDEX_KN:	{dForCode=1000.0;		break;}
		case D_UNITSYS_FORCE_INDEX_LBF:	{dForCode=4.448223;	break;}
		case D_UNITSYS_FORCE_INDEX_KIP:	{dForCode=4448.223;	break;}
		default:	ASSERT(0);
	}
	double dForCurr=0.0;
	switch(m_CurrUnit.nBase_Force)
	{
		case D_UNITSYS_FORCE_INDEX_KG:	{dForCurr=9.80665;	break;}
		case D_UNITSYS_FORCE_INDEX_TON:	{dForCurr=9806.65;	break;}
		case D_UNITSYS_FORCE_INDEX_N:		{dForCurr=1.0;			break;}
		case D_UNITSYS_FORCE_INDEX_KN:	{dForCurr=1000.0;		break;}
		case D_UNITSYS_FORCE_INDEX_LBF:	{dForCurr=4.448223;	break;}
		case D_UNITSYS_FORCE_INDEX_KIP:	{dForCurr=4448.223;	break;}
		default:	ASSERT(0);
	}
	return dForce * (dForCurr/dForCode);
}

double CRCDataCtrl_CH::Cng_MomentFromCurrToCode(double dMoment)
{
	double dLength=1.0;
	dLength = Cng_LengthFromCurrToCode(dLength,1);
	double dForce=1.0;
	dForce = Cng_ForceFromCurrToCode(dForce);
	return dMoment * (dForce*dLength);
}

double CRCDataCtrl_CH::Cng_LengthFromCodeToCurr(double dLength, int iUp)
{
	double dLenUnit=1.0;
	dLenUnit = Cng_LengthFromCurrToCode(dLenUnit,iUp);
	return dLength / dLenUnit;
}

double CRCDataCtrl_CH::Cng_ForceFromCodeToCurr(double dForce)
{
	double dForUnit=1.0;
	dForUnit = Cng_ForceFromCurrToCode(dForUnit);
	return dForce / dForUnit;
}

double CRCDataCtrl_CH::Cng_MomentFromCodeToCurr(double dMoment)
{
	double dLength=1.0;
	dLength = Cng_LengthFromCodeToCurr(dLength,1);
	double dForce=1.0;
	dForce = Cng_ForceFromCodeToCurr(dForce);
	return dMoment * (dForce*dLength);
}

double CRCDataCtrl_CH::Cng_StressFromCodeToCurr(double dStress)
{
	double dLength=1.0;
	dLength = Cng_LengthFromCodeToCurr(dLength,1);
	double dForce=1.0;
	dForce = Cng_ForceFromCodeToCurr(dForce);
	return dStress * (dForce/pow(dLength,2));
}

double CRCDataCtrl_CH::Cng_StressFromCurrToCode(double dStress)
{
	double dLength=1.0;
	dLength = Cng_LengthFromCurrToCode(dLength,1);
	double dForce=1.0;
	dForce = Cng_ForceFromCurrToCode(dForce);
	return dStress * (dForce/pow(dLength,2));
}

BOOL CRCDataCtrl_CH::Get_LastStepInCurStage(T_STAG_K StagK, int& iFinalStepNo)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	// Get Last StepNo at Current Stage.
	T_STAG_STEP_INFO StageInfo;
	pDoc->m_pAttrCtrl->GetStagStepInfo(StagK,StageInfo);
	int iStep = StageInfo.aInfo.GetSize();
	pDoc->m_pPostCtrl->GetStageInfo()->GetStepSerialKey(StagK,iStep,iFinalStepNo);

	return TRUE;
}

BOOL CRCDataCtrl_CH::Get_ConMatd(T_MATD_D& MatdD)
{
	// Get_DgnConMatd() at CDgnDataCtrl.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// Add by ZINU.('05.03.07). If SRC, Data2 -> Data1 (Src -> Con).
	if(MatdD.Type==_T("SRC"))	MatdD.Data1 = MatdD.Data2;

	// Concrete.
	if(MatdD.Data1.CodeName==_T("") || MatdD.Data1.CodeName==_T("None"))
	{
		T_MATL_CONCRETE ConcD;
		CString strCode=_T(""), strSubCode=_T(""), strMatName=_T("");
		CDBLib::GetDefaultConMatl(strCode, strSubCode, strMatName);
		pDoc->m_pMatlDB->GetConcreteDataNew(strCode,strSubCode,strMatName,ConcD);
		// If not exist, Set Default Data (Design).
		if(MatdD.Data1.Design.C_fc  <= 0.0)	MatdD.Data1.Design.C_fc	 = ConcD.C_fc;
		if(MatdD.Data1.Design.C_fci <= 0.0)	MatdD.Data1.Design.C_fci = ConcD.C_fci;
		// If not exist, Set Default Data (Analysis).
		if(MatdD.Data1.Analysis.Density <= 0.0)	MatdD.Data1.Analysis.Density = ConcD.Density;
		if(MatdD.Data1.Analysis.Elast   <= 0.0)	MatdD.Data1.Analysis.Elast   = ConcD.Elast;
		if(MatdD.Data1.Analysis.Poisson <= 0.0)	MatdD.Data1.Analysis.Poisson = ConcD.Poisson;
		if(MatdD.Data1.Analysis.Thermal <= 0.0)	MatdD.Data1.Analysis.Thermal = ConcD.Thermal;
	}
    // Rebar.
    if(MatdD.Rebar_CodeName==_T("") || MatdD.Rebar_CodeName==_T("None"))
	{
		CString strCode=_T(""), strRebarName=_T("");
		CDBLib::GetDefaultConRbar(strCode, strRebarName);

        T_MATL_REBAR RbarD;
        RbarD.Initialize();
        if(!pDoc->m_pMatlDB->GetRebarData(strCode, strRebarName, RbarD))	return FALSE;
        MatdD.Rebar_CodeName = strCode;
        // Save Main Rebar Data.
        if(MatdD.MainRebar_RebarName==_T(""))	        MatdD.MainRebar_RebarName     = strRebarName;	// SD40.
        if(MatdD.MainRebarData.B_Density <= 0.0)	MatdD.MainRebarData.B_Density = RbarD.B_Density;
        if(MatdD.MainRebarData.B_Elast   <= 0.0)	MatdD.MainRebarData.B_Elast   = RbarD.B_Elast;
        if(MatdD.MainRebarData.B_fy      <= 0.0)	MatdD.MainRebarData.B_fy      = RbarD.B_fy;

        // Save Sub Rebar Data.
        if(MatdD.SubRebar_RebarName==_T(""))					MatdD.SubRebar_RebarName     = strRebarName;	// SD40.
        if(MatdD.SubRebarData.B_Density <= 0.0)		MatdD.SubRebarData.B_Density = RbarD.B_Density;
        if(MatdD.SubRebarData.B_Elast   <= 0.0)		MatdD.SubRebarData.B_Elast   = RbarD.B_Elast;
        if(MatdD.SubRebarData.B_fy      <= 0.0)		MatdD.SubRebarData.B_fy      = RbarD.B_fy;
	}
	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
	MatdD.Data1.Design.C_fci = 0.7 * MatdD.Data1.Design.C_fc;

	// Change by ZINU.('05.03.07). Con -> Con,Src.
	if(MatdD.Type==_T("C") || MatdD.Type==_T("SRC"))	return TRUE;
	else																			return FALSE;
}

BOOL CRCDataCtrl_CH::Get_ConMatd(T_ELEM_K ElemK, T_MATD_D& MatdD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if(!pDoc->IsPostMode())	ASSERT(0);
	MatdD.Initialize();
	// Get Elem.
    T_ELEM_D ElemD;
    ElemD.Initialize();
    if(!pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return FALSE;
	// Get Matl.
    if(!pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat,MatdD))	return FALSE;
	// Change by ZINU.('05.03.07). Con -> Con, Src(颇屈碍魄).
	if(!(MatdD.Type==_T("C") || MatdD.Type==_T("SRC")))	return FALSE;
	// Set Default.
	Get_ConMatd(MatdD);
	// Change Unit (CurrUnit -> CodeUnit).
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
	pDoc->m_pUnitCtrl->ConvertUnitMatdPrevious(MatdD);
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CurrUnit);
	return TRUE;
}

void CRCDataCtrl_CH::Set_UnitIndex(T_UNIT_INDEX& CodeUnit)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	// Set CodeUnit, CurrUnit.
	m_CodeUnit = CodeUnit;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(m_CurrUnit);
}
T_UNIT_INDEX CRCDataCtrl_CH::Get_CodeUnitIndex()
{
    return m_CodeUnit;
}
/************************************************************************/
/* sure whether need print the PrintItem's data
     PrintItem:0-M+;1-M-;2-shear;3-torsion;4-tensile;
     5-compressive;6-rebarsM+;7-rebarsM-                                  */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_PrintOption(int Elemi,int iPosi,UINT PrintItem){
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int iPorcNum = pDoc->m_pAttrCtrl->GetCountPorc();
	T_PORC_D PorcD;
	PorcD.Initialize();
 	if(!pDoc->m_pAttrCtrl->GetPorc(Elemi, PorcD))	return  FALSE;	// Default.
//	if(!pDoc->m_pAttrCtrl->GetPorc(Elemi, PorcD))	return (iPorcNum==0 ? TRUE : FALSE);	// Default.
	if(PrintItem == 0)	// M+
	{
		if(PorcD.iMomChk[0]==0)									return FALSE;
		else if(PorcD.iMomChk[0]==1 && iPosi==1)	return FALSE;
		else if(PorcD.iMomChk[0]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	else if(PrintItem == 1)	// M-
	{
		if(PorcD.iMomChk[1]==0)									return FALSE;
		else if(PorcD.iMomChk[1]==1 && iPosi==1)	return FALSE;
		else if(PorcD.iMomChk[1]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 2) {//shear
		if(PorcD.iShrChk==0)									return FALSE;
		else if(PorcD.iShrChk==1 && iPosi==1)	return FALSE;
		else if(PorcD.iShrChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 3) {//torsion
		if(PorcD.iTorChk==0)									return FALSE;
		else if(PorcD.iTorChk==1 && iPosi==1)	return FALSE;
		else if(PorcD.iTorChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 4) {//tensile
		if(PorcD.iTenChk==0)									return FALSE;
		else if(PorcD.iTenChk==1 && iPosi==1)	return FALSE;
		else if(PorcD.iTenChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 5) {//compressive
		if(PorcD.iComChk==0)									return FALSE;
		else if(PorcD.iComChk==1 && iPosi==1)	return FALSE;
		else if(PorcD.iComChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 6) {//rebarsM+
		if(PorcD.iBarChk[0]==0)				  					return FALSE;
		else if(PorcD.iBarChk[0]==1 && iPosi==1)	return FALSE;
		else if(PorcD.iBarChk[0]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 7) {//rebarsM-
		if(PorcD.iBarChk[1]==0)					  				return FALSE;
		else if(PorcD.iBarChk[1]==1 && iPosi==1)	return FALSE;
		else if(PorcD.iBarChk[1]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/* sure whether need print the PrintItem's data.   For TB10002.3
     PrintItem:0-M+;1-M-;2-shear;3-PSC;4-tensile;5-compressive            */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_PSCPrintOption(int Elemi,int iPosi,UINT PrintItem){
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int iPorcNum = pDoc->m_pAttrCtrl->GetCountPosx();
	T_POSX_D PosxD;
	PosxD.Initialize();
 	if(!pDoc->m_pAttrCtrl->GetPosx(Elemi, PosxD))	return  FALSE;	// Default.
	if(PrintItem == 0)	// M+
	{
		if(PosxD.iMomChk[0]==0)			  						return FALSE;
		else if(PosxD.iMomChk[0]==1 && iPosi==1)	return FALSE;
		else if(PosxD.iMomChk[0]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	else if(PrintItem == 1)	// M-
	{
		if(PosxD.iMomChk[1]==0)						  			return FALSE;
		else if(PosxD.iMomChk[1]==1 && iPosi==1)	return FALSE;
		else if(PosxD.iMomChk[1]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 2) {//shear
		if(PosxD.iShrChk==0)									return FALSE;
		else if(PosxD.iShrChk==1 && iPosi==1)	return FALSE;
		else if(PosxD.iShrChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 3) {//PSC
		if(PosxD.iTorChk==0)									return FALSE;
		else if(PosxD.iTorChk==1 && iPosi==1)	return FALSE;
		else if(PosxD.iTorChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 4) {//tensile
		if(PosxD.iBarChk[0]==0)								  	return FALSE;
		else if(PosxD.iBarChk[0]==1 && iPosi==1)	return FALSE;
		else if(PosxD.iBarChk[0]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (PrintItem == 5) {//compressive
		if(PosxD.iBarChk[1]==0)							  		return FALSE;
		else if(PosxD.iBarChk[1]==1 && iPosi==1)	return FALSE;
		else if(PosxD.iBarChk[1]==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	return FALSE;
}

BOOL CRCDataCtrl_CH::Is_CalcItem(T_ELEM_K ElemK, UINT CheckItem)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    if(m_ForceCtrl->Get_Code()==JSCE02) return TRUE;  // Always TRUE if JSCE02.

    BOOL bCalcI = Get_ChkFlag(ElemK, 0, CheckItem);
    BOOL bCalcJ = Get_ChkFlag(ElemK, 1, CheckItem);
    if(bCalcI || bCalcJ)  return TRUE;
    return FALSE;
}

/************************************************************************/
/* sure whether need check the Elem's section   
     CheckItem:0-moment; 1-shear; 2-axis force ;                          */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_ChkFlag(T_ELEM_K ElemK, int iPosi, UINT CheckItem){
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int iDorcNum = pDoc->m_pAttrCtrl->GetCountDorc();
	T_DORC_D DorcD;
	DorcD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetDorc(ElemK, DorcD))	return (iDorcNum==0 ? TRUE : FALSE);	// Default.
	if(CheckItem == 0)	// Mom.
	{
		if(DorcD.iMomChk==0)									return FALSE;
		else if(DorcD.iMomChk==1 && iPosi==1)	return FALSE;
		else if(DorcD.iMomChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	else if(CheckItem == 1)	// Shear.
	{
		if(DorcD.iShrChk==0)									return FALSE;
		else if(DorcD.iShrChk==1 && iPosi==1)	return FALSE;
		else if(DorcD.iShrChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}else if (CheckItem == 2) {//axis
		if(DorcD.iAxisChk==0)									return FALSE;
		else if(DorcD.iAxisChk==1 && iPosi==1)	return FALSE;
		else if(DorcD.iAxisChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	return FALSE;
}

//add by maxiao2006.12.29
BOOL CRCDataCtrl_CH::Get_CHRCCbcsBaseCurrToCode(T_CBCS_CH_BASE& cbcsBase){
    cbcsBase.dMuy   = Cng_MomentFromCurrToCode(cbcsBase.dMuy);
    cbcsBase.drMuy  = Cng_MomentFromCurrToCode(cbcsBase.drMuy);
    cbcsBase.dMny   = Cng_MomentFromCurrToCode(cbcsBase.dMny);

    cbcsBase.dx            = Cng_LengthFromCurrToCode(cbcsBase.dx,1);
    cbcsBase.dEb           = Cng_LengthFromCurrToCode(cbcsBase.dEb,1);
    cbcsBase.d2a           = Cng_LengthFromCurrToCode(cbcsBase.d2a,1);
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCbcsBaseCodeToCurr(T_CBCS_CH_BASE& cbcsBase){
    cbcsBase.dMuy   = Cng_MomentFromCodeToCurr(cbcsBase.dMuy);
    cbcsBase.drMuy  = Cng_MomentFromCodeToCurr(cbcsBase.drMuy);
    cbcsBase.dMny   = Cng_MomentFromCodeToCurr(cbcsBase.dMny);

    cbcsBase.dx             = Cng_LengthFromCodeToCurr(cbcsBase.dx           ,1 );
    cbcsBase.dEb            = Cng_LengthFromCodeToCurr(cbcsBase.dEb          ,1 );
    cbcsBase.d2a            = Cng_LengthFromCodeToCurr(cbcsBase.d2a          ,1 );

    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCsisBaseCurrToCode(T_CSIS_CH_BASE& rData){
    rData.dMu       = Cng_MomentFromCurrToCode(rData.dMu        );
    rData.dMu_P     = Cng_MomentFromCurrToCode(rData.dMu_P      );
    rData.dVu       = Cng_ForceFromCurrToCode(rData.dVu        )*pow(10,-3);//change to kN
    rData.drVu      = Cng_ForceFromCurrToCode(rData.drVu       )*pow(10,-3);//change to kN
//  rData.dVu       = Cng_ForceFromCurrToCode(rData.dVu        )*pow(10,-3);//change to kN
    rData.dVn       = Cng_ForceFromCurrToCode(rData.dVn        )*pow(10,-3);//change to kN
    rData.dShrSect  = Cng_ForceFromCurrToCode(rData.dShrSect   )*pow(10,-3);//change to kN
    rData.dShrLoad  = Cng_ForceFromCurrToCode(rData.dShrLoad   )*pow(10,-3);//change to kN
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCsisBaseCodeToCurr(T_CSIS_CH_BASE& rData){
    rData.dMu       = Cng_MomentFromCodeToCurr(rData.dMu               );
    rData.dMu_P     = Cng_MomentFromCodeToCurr(rData.dMu_P             );
    rData.dVu       = Cng_ForceFromCodeToCurr(rData.dVu*pow(10,3)      );
    rData.drVu      = Cng_ForceFromCodeToCurr(rData.drVu*pow(10,3)     );
//  rData.dVu       = Cng_ForceFromCodeToCurr(rData.dVu*pow(10,3)      );
    rData.dVn       = Cng_ForceFromCodeToCurr(rData.dVn*pow(10,3)      );
    rData.dShrSect  = Cng_ForceFromCodeToCurr(rData.dShrSect*pow(10,3) );
    rData.dShrLoad  = Cng_ForceFromCodeToCurr(rData.dShrLoad*pow(10,3) );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCtssBaseCurrToCode(T_CTSS_CH_BASE& rData){
    rData.dTu 	  	= Cng_MomentFromCurrToCode(rData.dTu 	    );
    rData.drTu      = Cng_MomentFromCurrToCode(rData.drTu      );
    rData.dTn       = Cng_MomentFromCurrToCode(rData.dTn       );
    rData.dMu       = Cng_MomentFromCurrToCode(rData.dMu       );
    rData.dVu       = Cng_ForceFromCurrToCode(rData.dVu       );
    rData.drVu      = Cng_ForceFromCurrToCode(rData.drVu      );
    rData.dVn       = Cng_ForceFromCurrToCode(rData.dVn       );
    rData.dShrTorVal= Cng_StressFromCurrToCode(rData.dShrTorVal);
    rData.dTorSect  = Cng_StressFromCurrToCode(rData.dTorSect  );
    rData.dTorLoad  = Cng_StressFromCurrToCode(rData.dTorLoad  );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCtssBaseCodeToCurr(T_CTSS_CH_BASE& rData){
    rData.dTu 	  	= Cng_MomentFromCodeToCurr(rData.dTu 	    );
    rData.drTu      = Cng_MomentFromCodeToCurr(rData.drTu      );
    rData.dTn       = Cng_MomentFromCodeToCurr(rData.dTn       );
    rData.dMu       = Cng_MomentFromCodeToCurr(rData.dMu       );
    rData.dVu       = Cng_ForceFromCodeToCurr(rData.dVu       );
    rData.drVu      = Cng_ForceFromCodeToCurr(rData.drVu      );
    rData.dVn       = Cng_ForceFromCodeToCurr(rData.dVn       );
    rData.dShrTorVal= Cng_StressFromCodeToCurr(rData.dShrTorVal);
    rData.dTorSect  = Cng_StressFromCodeToCurr(rData.dTorSect  );
    rData.dTorLoad  = Cng_StressFromCodeToCurr(rData.dTorLoad  );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCcwsBaseCurrToCode(T_CCWS_CH_BASE& rData,BOOL isBeam){
    if (isBeam) {
        rData.dFT 	  = Cng_MomentFromCurrToCode(rData.dFT   );
        rData.dFB     = Cng_MomentFromCurrToCode(rData.dFB   );
    }else{
        rData.dFT 	  = Cng_ForceFromCurrToCode(rData.dFT    );
        rData.dFB     = Cng_ForceFromCurrToCode(rData.dFB    );
    }
    rData.dMsy 	  	= Cng_MomentFromCurrToCode(rData.dMsy    );
    rData.dMsz      = Cng_MomentFromCurrToCode(rData.dMsz    );
    rData.dFRT      = Cng_StressFromCurrToCode(rData.dFRT    );
    rData.dWC	      = Cng_LengthFromCurrToCode(rData.dWC,1   );
    rData.dAWC      = Cng_LengthFromCurrToCode(rData.dAWC,1  );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCcwsBaseCodeToCurr(T_CCWS_CH_BASE& rData,BOOL isBeam){
    if (isBeam) {
        rData.dFT 	  = Cng_MomentFromCodeToCurr(rData.dFT   );
        rData.dFB     = Cng_MomentFromCodeToCurr(rData.dFB   );
    }else{
        rData.dFT 	  = Cng_ForceFromCodeToCurr(rData.dFT    );
        rData.dFB     = Cng_ForceFromCodeToCurr(rData.dFB    );
    }
    rData.dMsy 	  	= Cng_MomentFromCodeToCurr(rData.dMsy    );
    rData.dMsz      = Cng_MomentFromCodeToCurr(rData.dMsz    );
    rData.dFRT      = Cng_StressFromCodeToCurr(rData.dFRT    );
    rData.dWC	      = Cng_LengthFromCodeToCurr(rData.dWC,1   );
    rData.dAWC      = Cng_LengthFromCodeToCurr(rData.dAWC,1  );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCsccBaseCurrToCode(T_CSCC_CH_BASE& rData){
    rData.dFT 	  = Cng_StressFromCurrToCode(rData.dFT 	 );
    rData.dFB 	  = Cng_StressFromCurrToCode(rData.dFB 	 );
    rData.dFTL    = Cng_StressFromCurrToCode(rData.dFTL   );
    rData.dFBL    = Cng_StressFromCurrToCode(rData.dFBL   );
    rData.dFTR    = Cng_StressFromCurrToCode(rData.dFTR   );
    rData.dFBR    = Cng_StressFromCurrToCode(rData.dFBR   );
    rData.dFMAX   = Cng_StressFromCurrToCode(rData.dFMAX  );
    rData.dALW    = Cng_StressFromCurrToCode(rData.dALW   );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCsccBaseCodeToCurr(T_CSCC_CH_BASE& rData){
    rData.dFT 	  = Cng_StressFromCodeToCurr(rData.dFT 	 );
    rData.dFB 	  = Cng_StressFromCodeToCurr(rData.dFB 	 );
    rData.dFTL    = Cng_StressFromCodeToCurr(rData.dFTL   );
    rData.dFBL    = Cng_StressFromCodeToCurr(rData.dFBL   );
    rData.dFTR    = Cng_StressFromCodeToCurr(rData.dFTR   );
    rData.dFBR    = Cng_StressFromCodeToCurr(rData.dFBR   );
    rData.dFMAX   = Cng_StressFromCodeToCurr(rData.dFMAX  );
    rData.dALW    = Cng_StressFromCodeToCurr(rData.dALW   );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCtstBaseCurrToCode(T_CTST_CH& rData){
    rData.dFLL 	  = Cng_StressFromCurrToCode(rData.dFLL 	 );
    rData.dAFLL 	= Cng_StressFromCurrToCode(rData.dAFLL 	 );
    rData.dDLL 	  = Cng_StressFromCurrToCode(rData.dDLL 	 );
    rData.dADLL 	= Cng_StressFromCurrToCode(rData.dADLL 	 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCtstBaseCodeToCurr(T_CTST_CH& rData){
    rData.dFLL 	  = Cng_StressFromCodeToCurr(rData.dFLL 	 );
    rData.dAFLL 	= Cng_StressFromCodeToCurr(rData.dAFLL 	 );
    rData.dDLL 	  = Cng_StressFromCodeToCurr(rData.dDLL 	 );
    rData.dADLL 	= Cng_StressFromCodeToCurr(rData.dADLL 	 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCsncBaseCurrToCode(T_CSNC_CH_BASE& rData){
    rData.dFM    = Cng_StressFromCurrToCode(rData.dFM   	 );
    rData.dFML   = Cng_StressFromCurrToCode(rData.dFML  	 );
    rData.dFMR   = Cng_StressFromCurrToCode(rData.dFMR  	 );
    rData.dFMAX  = Cng_StressFromCurrToCode(rData.dFMAX 	 );
    rData.dALW   = Cng_StressFromCurrToCode(rData.dALW  	 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCsncBaseCodeToCurr(T_CSNC_CH_BASE& rData){
    rData.dFM    = Cng_StressFromCodeToCurr(rData.dFM   	 );
    rData.dFML   = Cng_StressFromCodeToCurr(rData.dFML  	 );
    rData.dFMR   = Cng_StressFromCodeToCurr(rData.dFMR  	 );
    rData.dFMAX  = Cng_StressFromCodeToCurr(rData.dFMAX 	 );
    rData.dALW   = Cng_StressFromCodeToCurr(rData.dALW  	 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCstcsBaseCurrToCode(T_CSTCS_CH_BASE& rData){
    rData.dNd     = Cng_ForceFromCurrToCode(rData.dNd    	 );
    rData.dMd     = Cng_MomentFromCurrToCode(rData.dMd    	 );
    rData.drNd    = Cng_ForceFromCurrToCode(rData.drNd   	 );
    rData.drNde   = Cng_MomentFromCurrToCode(rData.drNde  	 );
    rData.drNde1  = Cng_MomentFromCurrToCode(rData.drNde1 	 );
    rData.dNn     = Cng_ForceFromCurrToCode(rData.dNn    	 );
    rData.dNne    = Cng_MomentFromCurrToCode(rData.dNne   	 );
    rData.dNne1   = Cng_MomentFromCurrToCode(rData.dNne1  	 );
    rData.dx      = Cng_LengthFromCurrToCode(rData.dx,1    	 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCstcsBaseCodeToCurr(T_CSTCS_CH_BASE& rData){
    rData.dNd     = Cng_ForceFromCodeToCurr(rData.dNd    	 );
    rData.dMd     = Cng_MomentFromCodeToCurr(rData.dMd    	 );
    rData.drNd    = Cng_ForceFromCodeToCurr(rData.drNd   	 );
    rData.drNde   = Cng_MomentFromCodeToCurr(rData.drNde  	 );
    rData.drNde1  = Cng_MomentFromCodeToCurr(rData.drNde1 	 );
    rData.dNn     = Cng_ForceFromCodeToCurr(rData.dNn    	 );
    rData.dNne    = Cng_MomentFromCodeToCurr(rData.dNne   	 );
    rData.dNne1   = Cng_MomentFromCodeToCurr(rData.dNne1  	 );
    rData.dx      = Cng_LengthFromCodeToCurr(rData.dx,1    	 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCstcsCurrToCode(T_CSTCS_CH& rData){
    for (int i=0;i<10;i++) Get_CHRCCstcsBaseCurrToCode(rData.cstcsBase[i]);
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCstcsCodeToCurr(T_CSTCS_CH& rData){
	for (int i=0;i<10;i++) Get_CHRCCstcsBaseCodeToCurr(rData.cstcsBase[i]);
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCBrlrBaseCurrToCode(T_BRLR_CH_BASE& rData){
    rData.dMd      = Cng_MomentFromCurrToCode(rData.dMd     	 );
    rData.dREQTop  = Cng_LengthFromCurrToCode(rData.dREQTop ,2 );
    rData.dREQBot  = Cng_LengthFromCurrToCode(rData.dREQBot ,2 );
    rData.dUSETop  = Cng_LengthFromCurrToCode(rData.dUSETop ,2 );
    rData.dUSEBot  = Cng_LengthFromCurrToCode(rData.dUSEBot ,2 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCBrlrBaseCodeToCurr(T_BRLR_CH_BASE& rData){
    rData.dMd      = Cng_MomentFromCodeToCurr(rData.dMd     	 );
    rData.dREQTop  = Cng_LengthFromCodeToCurr(rData.dREQTop ,2 );
    rData.dREQBot  = Cng_LengthFromCodeToCurr(rData.dREQBot ,2 );
    rData.dUSETop  = Cng_LengthFromCodeToCurr(rData.dUSETop ,2 );
    rData.dUSEBot  = Cng_LengthFromCodeToCurr(rData.dUSEBot ,2 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCBrcrBaseCurrToCode(T_BRCR_CH_BASE& rData){
    rData.dVd = Cng_ForceFromCurrToCode(rData.dVd );
    rData.dREQ= Cng_LengthFromCurrToCode(rData.dREQ,2);
    rData.dUSE= Cng_LengthFromCurrToCode(rData.dUSE,2);
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCBrcrBaseCodeToCurr(T_BRCR_CH_BASE& rData){
    rData.dVd = Cng_ForceFromCodeToCurr(rData.dVd );
    rData.dREQ= Cng_LengthFromCodeToCurr(rData.dREQ,2);
    rData.dUSE= Cng_LengthFromCodeToCurr(rData.dUSE,2);
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCrlrBaseCurrToCode(T_CRLR_CH_BASE& rData){
    rData.dNd       = Cng_ForceFromCurrToCode(rData.dNd      	 );
    rData.dMyd      = Cng_MomentFromCurrToCode(rData.dMyd     	 );
    rData.dMzd      = Cng_MomentFromCurrToCode(rData.dMzd     	 );
    rData.dREQTop   = Cng_LengthFromCurrToCode(rData.dREQTop,2 	 );
    rData.dREQBot   = Cng_LengthFromCurrToCode(rData.dREQBot,2 	 );
    rData.dREQLeft  = Cng_LengthFromCurrToCode(rData.dREQLeft,2	 );
    rData.dREQRight = Cng_LengthFromCurrToCode(rData.dREQRight,2 );
    
    rData.dUSETop   = Cng_LengthFromCurrToCode(rData.dUSETop,2 	 );
    rData.dUSEBot   = Cng_LengthFromCurrToCode(rData.dUSEBot,2 	 );
    rData.dUSELeft  = Cng_LengthFromCurrToCode(rData.dUSELeft,2	 );
    rData.dUSERight = Cng_LengthFromCurrToCode(rData.dUSERight,2 );
    return TRUE;
}
BOOL CRCDataCtrl_CH::Get_CHRCCrlrBaseCodeToCurr(T_CRLR_CH_BASE& rData){
    rData.dNd       = Cng_ForceFromCodeToCurr(rData.dNd      	 );
    rData.dMyd      = Cng_MomentFromCodeToCurr(rData.dMyd     	 );
    rData.dMzd      = Cng_MomentFromCodeToCurr(rData.dMzd     	 );
    rData.dREQTop   = Cng_LengthFromCodeToCurr(rData.dREQTop,2 	 );
    rData.dREQBot   = Cng_LengthFromCodeToCurr(rData.dREQBot,2 	 );
    rData.dREQLeft  = Cng_LengthFromCodeToCurr(rData.dREQLeft,2	 );
    rData.dREQRight = Cng_LengthFromCodeToCurr(rData.dREQRight,2 );
    rData.dUSETop   = Cng_LengthFromCodeToCurr(rData.dUSETop,2 	 );
    rData.dUSEBot   = Cng_LengthFromCodeToCurr(rData.dUSEBot,2 	 );
    rData.dUSELeft  = Cng_LengthFromCodeToCurr(rData.dUSELeft,2	 );
    rData.dUSERight = Cng_LengthFromCodeToCurr(rData.dUSERight,2 );
    return TRUE;
}

/************************************************************************/
/* get RC design elemlist. Beam and column                              */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CrcElemListForDgn(BOOL isPsc, CArray<UINT,UINT>& aElemK){
	aElemK.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> aSelElemK;
    int iTotElemNum = m_pDoc->m_pAttrCtrl->GetCountElem();
    for(int i=0; i<iTotElemNum; i++)
    {
        T_ELEM_K ElemK = m_pDoc->m_pPostCtrl->GetOriginElem(i+1);   
		int iNum=0; BOOL bGetOK = FALSE;
		if (isPsc) {
			T_POSC_D PoscD; PoscD.Initialize();
			iNum = m_pDoc->m_pAttrCtrl->GetCountPosc();
			bGetOK = m_pDoc->m_pAttrCtrl->GetPosc(ElemK, PoscD);
		}else{
			T_DORC_D DorcD; DorcD.Initialize();
            iNum = m_pDoc->m_pAttrCtrl->GetCountDorc();
            bGetOK = m_pDoc->m_pAttrCtrl->GetDorc(ElemK,DorcD);
		}
        BOOL bIncludeElem = TRUE;
        if (!bGetOK && iNum>0) bIncludeElem = FALSE;
        if (bIncludeElem) aSelElemK.Add(ElemK);
	}
    for(int i=0; i<aSelElemK.GetSize(); i++)
	{
		T_ELEM_K ElemK = aSelElemK.GetAt(i);
		ElemPairK EPairK(ElemK, EN_EL_BEAM);
		BOOL bCheck = FALSE;
        if (CDBLib::IsPscDgnFor1st(EPairK,TRUE) ){//beam
			if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_BEAM)	continue;
			bCheck = TRUE;
        }			
		//column
		if (!bCheck){
			if(CDBLib::IsRcColumnDgn(ElemK,TRUE)){
			if(m_pDoc->m_pAttrCtrl->GetMemberType(ElemK)!=D_MBTP_COLUMN)	continue;
				bCheck = TRUE;
			}
		}
		if (bCheck)		aElemK.Add(ElemK);
	}  
    if(aElemK.GetSize() > 0)	return TRUE;
	else											return FALSE;
}

BOOL CRCDataCtrl_CH::Get_ForcDataMap(BOOL isPsc,CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&>& aForcD, BOOL bSeismic)
{
	aForcD.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	if(!bSeismic)
    { if (!Get_CrcElemListForDgn(isPsc,aElemK)) return FALSE; }
	else 
	{ if (!Get_ElemListForSeismicDgn(aElemK)) return FALSE; }
    _CRC_COMF_K ComfK;
    _CRC_COMF_D ComfD;
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
	int i=0, j=0, k=0;
	for(i=0; i<aElemK.GetSize(); i++)
	{
		T_ELEM_K ElemK = aElemK.GetAt(i);
		ElemPairK EPairK(ElemK, EN_EL_BEAM);

		ADGNFORCE aForceLcom;
		if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE,FALSE,aForceLcom,iLcomNum))	continue;	// TRUE=Include Tendon Primary.
        for(j=0; j<aForceLcom.GetSize(); j++)
        {
            _DGN_FORC_CRC ForceLcom = aForceLcom.GetAt(j);
			int iDgnLcomNo = j+1;
            CString strOrgLcomNa=_T("");
			int iOrgLcomNo=0, iOrgLccomCount=0;
            int iMaxMinType=0;
            if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iOrgLcomNo, iOrgLccomCount, iMaxMinType)) continue;
            int iDgnLcomKind = m_ForceCtrl->Get_LcomKind(iDgnLcomNo);
            for(k=0; k<2; k++)	// I,J.
            {
                ComfK.Initialize();
                ComfK.ElemK		 = ElemK;
                ComfK.DgnLcomK = iDgnLcomNo;
                ComfK.nPos		 = k;
                ComfD.Initialize();
                ComfD.OrgLcomK = iOrgLcomNo;
                ComfD.iKind		 = iDgnLcomKind;
                ComfD.iMax		 = iMaxMinType;
                ComfD.dFxx		 = ForceLcom.dFxx[k];
                ComfD.dFyy		 = ForceLcom.dFyy[k];
                ComfD.dFzz		 = ForceLcom.dFzz[k];
                ComfD.dMux		 = ForceLcom.dMux[k];
                ComfD.dMuy		 = ForceLcom.dMuy[k];
                ComfD.dMuz		 = ForceLcom.dMuz[k];
                aForcD.SetAt(ComfK,ComfD);
            }
        }
    }
    return TRUE;
}

/************************************************************************/
/* 得到短暂状况(施工荷载)的正截面压应力  
                                                     钢筋拉应力                                   */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_CrcCscc(int ElemK,/*_DGN_RC_SECTION_CH& section,*/double fsd,double fsd1,double fcd,double dEs,double dEc,
																				T_CSCC_CH& cscc,CArray<T_CTST_CH,T_CTST_CH>& ctstList){//得到短暂状况(施工荷载)的正截面压应力 add by maxiao 2007.03.28
    cscc.Initialize(); 	ctstList.RemoveAll();

    if(!Is_CalcItem(ElemK,0))  return FALSE;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	_DGN_FORC_CRC ForcMax; ForcMax.Initialize(TRUE);
	_DGN_FORC_CRC ForcMin; ForcMin.Initialize(FALSE);
	CArray<T_STAG_K,T_STAG_K> aStagK;
    CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
	if(aStagK.GetSize()==0)	return FALSE;
	int iStagk =0;
	for(int i=0; i<aStagK.GetSize(); i++)
	{
        T_STAG_K StagK = aStagK.GetAt(i);    iStagk = StagK;
		int iFinalStepNo=0;
		// Get Last StepNo at Current Stage.
		Get_LastStepInCurStage(StagK, iFinalStepNo);
		CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
		T_SGLD_K SgldK = pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
		if(!m_ForceCtrl->Get_ForceBySgld(EPairK, SgldK, StagK, iFinalStepNo, ForcMax, ForcMin))	continue;
	}
    for(int i=0; i<4; i++)
    {		// // 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
 		_DGN_FORC_CRC ForcCrc = (i%2==0 ? ForcMax : ForcMin);
//		if(ForcCrc.dMuy[i/2]==0.0)	continue;
 		cscc.CsccBase[i].bCHK	= TRUE;//Get_ChkFlag(ElemK, i/2, TRUE);
		cscc.CsccBase[i].StagK	= iStagk;//ForcCrc.iRefNo[i/2];
		cscc.CsccBase[i].iMax = (i%2==0 ? 9:10);//0,2-Mymax   1,3- Mymin

		double dM =0.0, dx = 0.0, dJcr=0.0;
		_DGN_RC_SECTION_CH section; section.Initialize();
    	if (!Get_BeamSection(ElemK,i%2,section)) {continue;}
		dM = ForcCrc.dMuy[i/2];
		dM = Cng_MomentFromCurrToCode(dM);

		if (dM <0.0) {
			dM = -dM;
			double dtemp = section.dhf1; section.dhf1 = section.dhf2; section.dhf2 = dtemp;
			dtemp = section.dbf1; section.dbf1 = section.dbf2; section.dbf2 = dtemp;
			dtemp = section.dAs; section.dAs = section.dAs1; section.dAs1 = dtemp;
			dtemp = section.das; section.das = section.das1; section.das1 = dtemp;
			dtemp = fsd; fsd = fsd1; fsd1 = dtemp;
			section.dh0 = section.dh -section.das;
		}
		//get x  设计原理243页 T
		double da = section.db;
		double db = 2*section.dhf1*(section.dbf1-section.db)+2*dEs/dEc*(section.dAs+section.dAs1);
		double dc = -section.dhf1*section.dhf1*(section.dbf1-section.db)-2*dEs/dEc*(section.das1*section.dAs1+section.dAs*section.dh0);
		dx = Solve_Quadratic(da,db,dc);

		if (dx <= section.dhf1) {
			section.db =section.dbf1;
			da = section.db;
			db = 2*section.dhf1*(section.dbf1-section.db)+2*dEs/dEc*(section.dAs+section.dAs1);
			dc = -section.dhf1*section.dhf1*(section.dbf1-section.db)-2*dEs/dEc*(section.das1*section.dAs1+section.dAs*section.dh0);
			dx = Solve_Quadratic(da,db,dc);
		}
		//get Jcr
		dJcr = section.dbf1*dx*dx*dx/3.0-(section.dbf1-section.db)*(dx-section.dhf1)*(dx-section.dhf1)*(dx-section.dhf1)/3.0
			+dEs/dEc*section.dAs1*(dx-section.das1)*(dx-section.das1)+dEs/dEc*section.dAs*(section.dh0-dx)*(section.dh0-dx);
		//get σcc
        cscc.CsccBase[i].dFMAX = (fabs(dJcr)<1.0e-6) ? 0.0 : dM*dx/dJcr;
 		cscc.CsccBase[i].bCHK	= TRUE;//Get_ChkFlag(ElemK, i/2, TRUE);
//    Get_CHRCCsccBaseCurrToCode(cscc.CsccBase[i]);//Change UNIT
		//////////////////////////////////////////////////////////////////////////
		if (i==1 || i==3) continue;
        if (section.rbar.GetSize() <=0) continue;  
		int barnum =1;
		for (int k=0; k< section.rbar.GetSize();k++) {
                T_CTST_CH ctst ; ctst.Initialize();
                T_RPSC_MBAR rbar = section.rbar.GetAt(k);
				ctst.iposi = (i==0 ? 0:1);//i,J end
				rbar.dStaZ = Cng_LengthFromCurrToCode(rbar.dStaZ,1);
				ctst.dDLL = dEs/dEc*dM*(section.dh - rbar.dStaZ- dx)/dJcr;//DLL
				ctst.dDLL = Cng_StressFromCodeToCurr(ctst.dDLL);
			//////////////////////////////////////////////////////////////////////////
		  //弹性阶段验算组合下混凝土应力反算钢筋应力
			_DGN_STRS_CRC StrsMax; StrsMax.Initialize();
			_DGN_STRS_CRC StrsMin; StrsMin.Initialize();
                int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
				ADGNFORCE aForceLcom;
				ADGNSTRSADD aStressAdd;
				ADGNSHRSADD aStssLcom;
				if(!m_ForceCtrl->Get_LcomDataForStress(EPairK, FALSE, aForceLcom, aStressAdd, aStssLcom, iLcomNum))	return FALSE;
				double dFll = 0.0;
				for(int j=0; j<aForceLcom.GetSize(); j++)	{
					if(!m_ForceCtrl->Is_LcomForElst(j+1))  continue; // is Serviceability and Elasticity.
					_DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(j);
					_DGN_STRS_ADD StrsAdd /*; StrsAdd.Initialize();*/ = aStressAdd.GetAt(j);//温度应力
					StrsMax.Initialize(); StrsMin.Initialize();
					if (m_ForceCtrl->Get_StrsByForc(EPairK, j+1, ForceCrc, StrsAdd, StrsMax, StrsMin)){
						double dftop = -StrsMax.dStop[i%2];
						double dfbot = -StrsMax.dSbot[i%2];
						double dfll = 0.0;
						if (rbar.iStaZRef ==1) 
							dfll = (section.dh - fabs(rbar.dStaZ))*(dfbot-dftop)/section.dh+dftop;
						else
							dfll = (section.dh - fabs(rbar.dStaZ))*(dftop-dfbot)/section.dh+dfbot;
						dfll = dfll*dEs/dEc;
						if ((fabs(dfll) > fabs(dFll)) && (fabs(dfll) > 1.0E-07) ) {dFll = dfll;}    
					}
				}
				ctst.dFLL = dFll;
				ctst.iRbarNa = barnum++;
				//Change CurrToCode...
				Get_CHRCCtstBaseCurrToCode(ctst);//Change UNIT
				ctstList.Add(ctst);
		}		
	}
	return TRUE;
}
void CRCDataCtrl_CH::GetFactorGS_Sub(T_KEY LcombK, T_KEY SgldKTp, T_KEY SgldKTs, T_KEY SgldSumK, double dMutiRatio, double &dRatio1, double &dRatio2)
{
        T_LCOM_D rData;
        if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcombK, rData))
                return;
        for (int i=0; i<rData.aCombination.GetCount(); i++)
        {
                const T_LCOM_BASE &data = rData.aCombination[i];
                if (data.AnalType == D_LCOM_STAGE)
                {
                        if(data.LoadCaseKey == SgldKTp || data.LoadCaseKey == SgldSumK)
                                dRatio1 += data.Factor*dMutiRatio;
                        if(data.LoadCaseKey == SgldKTs || data.LoadCaseKey == SgldSumK)
                                dRatio2 += data.Factor*dMutiRatio;
                }
                else if(data.AnalType == D_LCOM_CB_CONCRETE)
                {
                        GetFactorGS_Sub(data.LoadCaseKey, SgldKTp, SgldKTs, SgldSumK, data.Factor, dRatio1, dRatio2);
                }
        }
}
void CRCDataCtrl_CH::GetFactorGS(int iDgnLcomK, double &dRatio1, double &dRatio2, CMapEx<T_KEY, T_KEY, double, double> &mapExistRatio1, CMapEx<T_KEY, T_KEY, double, double> &mapExistRatio2)
{
        dRatio1 = 0;
        dRatio2 = 0;
        T_LCOM_K LcomK = m_ForceCtrl->Get_OriginalLcomKey(iDgnLcomK);
        if(LcomK < 1)
                return;
        if(mapExistRatio1.Lookup(LcomK, dRatio1) && mapExistRatio2.Lookup(LcomK, dRatio2))
                return;  
        CString strSgldName = _LS(IDS_DB_SGLD_TP);	// Tp.
        T_SGLD_K SgldKTp = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
        strSgldName = _LS(IDS_DB_SGLD_TS);	// Ts.
        T_SGLD_K SgldKTs = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
        strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
        T_SGLD_K SgldSumK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
        //
        GetFactorGS_Sub(LcomK, SgldKTp, SgldKTs, SgldSumK, 1.0, dRatio1, dRatio2);
        mapExistRatio1.SetAt(LcomK, dRatio1);
        mapExistRatio2.SetAt(LcomK, dRatio2);
}

//////////////////////////////////////////////////////////////////////////
//railway
/************************************************************************/
/* 得到铁路-BEAM-验算需要的内力                                               */
/* 0－My最大值、Vz最大值   1－My最小值、Vz最小值                        */         
/* My1－My中的活载弯矩值   My2－My中的恒载弯矩值                        */ 
//BOOL bCSTp=TRUE-include CS(Tendon primary) FALSE-not include CS(Tendon primary) :PSC tend and shear not include~!
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_RailBeamForce(T_ELEM_K ElemK,int iposi,T_RAIL_BEAM_FORCE& beam_mv,BOOL bCSTp, BOOL bRemoveGS)
{
	    ElemPairK EPairK(ElemK, EN_EL_BEAM);

        beam_mv.Initialize();
        CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
        int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
        ADGNFORCE aForceLcom;
        if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))
                return FALSE;
        double dTol=1./pow(m_dZero,40);

        double dMyMax[2]={-dTol,-dTol}, dMyMin[2]={+dTol,+dTol};
        int iMyMaxLcom[2]={0,0}, iMyMinLcom[2]={0,0};
        double dVzMax[2]={-dTol,-dTol}, dVzMin[2]={+dTol,+dTol};
        int iVzMaxLcom[2]={0,0}, iVzMinLcom[2]={0,0};
        double dVzMaxMy[2]={-dTol,-dTol}, dVzMinMy[2]={+dTol,+dTol};
        int i=0, j=0;
        _DGN_FORC_CRC ForceDeadMax, ForceDeadMin, ForceLiveMax,ForceLiveMin,ForceTemp;
        ForceDeadMax.Initialize();	ForceLiveMax.Initialize();
        ForceDeadMin.Initialize(); 	 ForceLiveMin.Initialize();
        //
        _DGN_FORC_CRC ForceTP,ForceTS; ForceTP.Initialize(); ForceTS.Initialize();
        if(bRemoveGS)
                Get_ForceOfTpTs(ElemK,0,ForceTP,ForceTS);

        CMapEx<T_KEY, T_KEY, double, double> mapExistRatio1;
        CMapEx<T_KEY, T_KEY, double, double> mapExistRatio2;
        for(i=0; i<aForceLcom.GetSize(); i++)	
        {
                if ((!m_ForceCtrl->Is_LcomForMain(i+1)) && (!m_ForceCtrl->Is_LcomForMainAdd(i+1))) 
                        continue;
                _DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
                _DGN_FORC_CRC ForceCSTp; ForceCSTp.Initialize();
                double dRatio_GS1 = 0;
                double dRatio_GS2 = 0;
                if(bRemoveGS)
                {
                        GetFactorGS(i+1, dRatio_GS1, dRatio_GS2, mapExistRatio1, mapExistRatio2);
                        ForceCrc.dMuy[0] -= (dRatio_GS1*ForceTP.dMuy[0] + dRatio_GS2*ForceTS.dMuy[0]);
                        ForceCrc.dMuy[1] -= (dRatio_GS1*ForceTP.dMuy[1] + dRatio_GS2*ForceTS.dMuy[1]);
                }
                if(!bCSTp && !bRemoveGS)
                {
                        m_pDesignForceResult->GetForceCSTP_TB05Result(EPairK,i+1,ForceCSTp);//bend and shear of PSC
                }
                for(j=0; j<2; j++)
                {	// I,J.			  
                        if(ForceCrc.dMuy[j] - ForceCSTp.dMuy[j] > dMyMax[j])
                        {	
                                ForceTemp.Initialize();
                                dMyMax[j]=ForceCrc.dMuy[j] - ForceCSTp.dMuy[j]; iMyMaxLcom[j]=i+1;
                                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceTemp);
                                ForceDeadMax.dMuy[j] = ForceTemp.dMuy[j] - ForceCSTp.dMuy[j];
                                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceTemp);
                                ForceLiveMax.dMuy[j] = ForceTemp.dMuy[j];
                        }
                        if(ForceCrc.dMuy[j] - ForceCSTp.dMuy[j] < dMyMin[j])	
                        {				
                                ForceTemp.Initialize();
                                dMyMin[j]=ForceCrc.dMuy[j] - ForceCSTp.dMuy[j]; iMyMinLcom[j]=i+1;
                                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceTemp);//not run will
                                ForceDeadMin.dMuy[j] = ForceTemp.dMuy[j] - ForceCSTp.dMuy[j];
                                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceTemp);
                                ForceLiveMin.dMuy[j] = ForceTemp.dMuy[j];
                        }
                        if(ForceCrc.dFzz[j] - ForceCSTp.dFzz[j] > dVzMax[j])	
                        {
                                dVzMax[j]=ForceCrc.dFzz[j] - ForceCSTp.dFzz[j]; iVzMaxLcom[j]=i+1;
                                dVzMaxMy[j]=ForceCrc.dMuy[j] - ForceCSTp.dMuy[j];
                        }//Fz
                        if(ForceCrc.dFzz[j] - ForceCSTp.dFzz[j] < dVzMin[j])	
                        {
                                dVzMin[j]=ForceCrc.dFzz[j] - ForceCSTp.dFzz[j]; iVzMinLcom[j]=i+1;
                                dVzMinMy[j]=ForceCrc.dMuy[j] - ForceCSTp.dMuy[j];
                        }
                }
        }
        beam_mv.dMy[0] = Cng_MomentFromCurrToCode(dMyMax[iposi]); beam_mv.iMy[0] = iMyMaxLcom[iposi];
        beam_mv.dMy[1] = Cng_MomentFromCurrToCode(dMyMin[iposi]); beam_mv.iMy[1] = iMyMinLcom[iposi];
        beam_mv.dVz[2] = Cng_ForceFromCurrToCode(dVzMax[iposi]); beam_mv.iVz[2] = iVzMaxLcom[iposi];
        beam_mv.dMy[2] = Cng_ForceFromCurrToCode(dVzMaxMy[iposi]);
        beam_mv.dVz[3] = Cng_ForceFromCurrToCode(dVzMin[iposi]); beam_mv.iVz[3] = iVzMinLcom[iposi];
        beam_mv.dMy[3] = Cng_ForceFromCurrToCode(dVzMinMy[iposi]);
        //  beam_mv.dNx[0] = dNxMax[iposi];  beam_mv.iNx[0] = iNxMaxLcom[iposi];
        //	beam_mv.dNx[1] = dNxMin[iposi];  beam_mv.iNx[1] = iNxMinLcom[iposi];
        beam_mv.dMy1[0]= Cng_MomentFromCurrToCode(ForceLiveMax.dMuy[iposi]); 
        beam_mv.dMy1[1]= Cng_MomentFromCurrToCode(ForceLiveMin.dMuy[iposi]);
        beam_mv.dMy2[0]= Cng_MomentFromCurrToCode(ForceDeadMax.dMuy[iposi]); 
        beam_mv.dMy2[1]= Cng_MomentFromCurrToCode(ForceDeadMin.dMuy[iposi]);
        if (m_ForceCtrl->Is_LcomForMainAdd(iMyMaxLcom[iposi])) beam_mv.isAddForce[0] = TRUE;
        if (m_ForceCtrl->Is_LcomForMainAdd(iMyMinLcom[iposi])) beam_mv.isAddForce[1] = TRUE;
        if (m_ForceCtrl->Is_LcomForMainAdd(iVzMaxLcom[iposi])) beam_mv.isAddForce[2] = TRUE;
        if (m_ForceCtrl->Is_LcomForMainAdd(iVzMinLcom[iposi])) beam_mv.isAddForce[3] = TRUE;
        return TRUE;
}

/************************************************************************/
/* 得到铁路-COLUMN-验算需要的内力                                       */
/* 0－Nx最大时的内力组合	2－My最大时的内力组合  4－Vz最大时的内力组合  */
/* 1－Nx最小时的内力组合  3－My最小时的内力组合  5－Vz最小时的内力组合  */
/* 6－Mz最大时的内力组合  7－Mz最小时的内力组合                         */
/* My1－My中的活载弯矩值                         My2－My中的恒载弯矩值  */ 
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_RailColumnForce(T_ELEM_K ElemK,int iposi,T_RAIL_COLUMN_FORCE& column_mnv,BOOL isSeismic){
	column_mnv.Initialize();
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);
	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();

	ElemPairK EPairK(ElemK, EN_EL_BEAM);
 	ADGNFORCE aForceLcom;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
	double dTol=1./pow(m_dZero,40);
    double dNxMax[2]={-dTol,-dTol}, dNxMin[2]={+dTol,+dTol};
	int iNxMaxLcom[2]={0,0}, iNxMinLcom[2]={0,0};  
	double dMyMax[2]={-dTol,-dTol}, dMyMin[2]={+dTol,+dTol};
	int iMyMaxLcom[2]={0,0}, iMyMinLcom[2]={0,0};
	double dVzMax[2]={-dTol,-dTol}, dVzMin[2]={+dTol,+dTol};
	int iVzMaxLcom[2]={0,0}, iVzMinLcom[2]={0,0};
	double dMzMax[2]={-dTol,-dTol}, dMzMin[2]={+dTol,+dTol};
	int iMzMaxLcom[2]={0,0}, iMzMinLcom[2]={0,0};
    double dMyMax_Nx[2]={0.0,0.0},dMyMax_Vz[2]={0.0,0.0},dMyMax_Mz[2]={0.0,0.0};
    double dMyMin_Nx[2]={0.0,0.0},dMyMin_Vz[2]={0.0,0.0},dMyMin_Mz[2]={0.0,0.0};
    double dVzMax_Nx[2]={0.0,0.0},dVzMax_My[2]={0.0,0.0},dVzMax_Mz[2]={0.0,0.0};
    double dVzMin_Nx[2]={0.0,0.0},dVzMin_My[2]={0.0,0.0},dVzMin_Mz[2]={0.0,0.0};
    double dNxMax_Vz[2]={0.0,0.0},dNxMax_My[2]={0.0,0.0},dNxMax_Mz[2]={0.0,0.0};
    double dNxMin_Vz[2]={0.0,0.0},dNxMin_My[2]={0.0,0.0},dNxMin_Mz[2]={0.0,0.0};
    double dMzMax_Nx[2]={0.0,0.0},dMzMax_Vz[2]={0.0,0.0},dMzMax_My[2]={0.0,0.0};
    double dMzMin_Nx[2]={0.0,0.0},dMzMin_Vz[2]={0.0,0.0},dMzMin_My[2]={0.0,0.0};
    
	int i=0, j=0, inum = 0;
    _DGN_FORC_CRC ForceDeadMyMax, ForceDeadMyMin, ForceLiveMyMax,ForceLiveMyMin;
    _DGN_FORC_CRC ForceDeadNxMax, ForceDeadNxMin, ForceLiveNxMax,ForceLiveNxMin;
    _DGN_FORC_CRC ForceDeadVzMax, ForceDeadVzMin, ForceLiveVzMax,ForceLiveVzMin;
    _DGN_FORC_CRC ForceDeadMzMax, ForceDeadMzMin, ForceLiveMzMax,ForceLiveMzMin;
    ForceDeadMyMax.Initialize(); ForceDeadMyMin.Initialize(); ForceLiveMyMax.Initialize(); ForceLiveMyMin.Initialize();
    ForceDeadNxMax.Initialize(); ForceDeadNxMin.Initialize(); ForceLiveNxMax.Initialize(); ForceLiveNxMin.Initialize();
    ForceDeadVzMax.Initialize(); ForceDeadVzMin.Initialize(); ForceLiveVzMax.Initialize(); ForceLiveVzMin.Initialize();
    ForceDeadMzMax.Initialize(); ForceDeadMzMin.Initialize(); ForceLiveMzMax.Initialize(); ForceLiveMzMin.Initialize();

    for(i=0; i<aForceLcom.GetSize(); i++)	{
//    if ((!m_ForceCtrl->Is_LcomForMain(i+1)) && (!m_ForceCtrl->Is_LcomForMainAdd(i+1))) continue;
        if (isSeismic)
		{ if (!m_ForceCtrl->Is_LcomForMainSpecial(i+1)) continue; }
		else
		{ if ((!m_ForceCtrl->Is_LcomForMain(i+1)) && (!m_ForceCtrl->Is_LcomForMainAdd(i+1))) continue; }
		inum ++;
		_DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
		for(j=0; j<2; j++){	// I,J.
			if(ForceCrc.dMuy[j] > dMyMax[j])	{//My
                dMyMax[j]=ForceCrc.dMuy[j]; iMyMaxLcom[j]=i+1;
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadMyMax);
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveMyMax);
                dMyMax_Nx[j]=ForceCrc.dFxx[j]; dMyMax_Vz[j]=ForceCrc.dFzz[j]; dMyMax_Mz[j]=ForceCrc.dMuz[j];
            }
			if(ForceCrc.dMuy[j] < dMyMin[j])	{
                dMyMin[j]=ForceCrc.dMuy[j]; iMyMinLcom[j]=i+1;
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadMyMin);//not run will
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveMyMin);
                dMyMin_Nx[j]=ForceCrc.dFxx[j]; dMyMin_Vz[j]=ForceCrc.dFzz[j]; dMyMin_Mz[j]=ForceCrc.dMuz[j];
            }
			if(ForceCrc.dFzz[j] > dVzMax[j])	{//Fz
                dVzMax[j]=ForceCrc.dFzz[j]; iVzMaxLcom[j]=i+1;
                dVzMax_My[j]=ForceCrc.dMuy[j]; dVzMax_Nx[j]=ForceCrc.dFxx[j]; dVzMax_Mz[j]=ForceCrc.dMuz[j];
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadVzMax);//not run will
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveVzMax);
            }
			if(ForceCrc.dFzz[j] < dVzMin[j])	{
                dVzMin[j]=ForceCrc.dFzz[j]; iVzMinLcom[j]=i+1;
                dVzMin_My[j]=ForceCrc.dMuy[j]; dVzMin_Nx[j]=ForceCrc.dFxx[j]; dVzMin_Mz[j]=ForceCrc.dMuz[j];
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadVzMin);//not run will
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveVzMin);
            }
			if(ForceCrc.dFxx[j] > dNxMax[j])	{//Nx
                dNxMax[j]=ForceCrc.dFxx[j]; iNxMaxLcom[j]=i+1;
                dNxMax_My[j]=ForceCrc.dMuy[j]; dNxMax_Vz[j]=ForceCrc.dFzz[j]; dNxMax_Mz[j]=ForceCrc.dMuz[j];
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadNxMax);//not run will
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveNxMax);
            }
			if(ForceCrc.dFxx[j] < dNxMin[j])	{
                dNxMin[j]=ForceCrc.dFxx[j]; iNxMinLcom[j]=i+1;
                dNxMin_My[j]=ForceCrc.dMuy[j]; dNxMin_Vz[j]=ForceCrc.dFzz[j]; dNxMin_Mz[j]=ForceCrc.dMuz[j];
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadNxMin);//not run will
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveNxMin);
            }

			if(ForceCrc.dMuz[j] > dMzMax[j])	{//Mz
                dMzMax[j]=ForceCrc.dMuz[j]; iMzMaxLcom[j]=i+1;
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadMzMax);
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveMzMax);
                dMzMax_Nx[j]=ForceCrc.dFxx[j]; dMzMax_Vz[j]=ForceCrc.dFzz[j]; dMzMax_My[j]=ForceCrc.dMuy[j];
            }
			if(ForceCrc.dMuz[j] < dMzMin[j])	{
                dMzMin[j]=ForceCrc.dMuz[j]; iMzMinLcom[j]=i+1;
                m_pDesignForceResult->GetForceDL_TB05Result(EPairK,i+1,ForceDeadMzMin);//not run will
                m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,ForceLiveMzMin);
                dMzMin_Nx[j]=ForceCrc.dFxx[j]; dMzMin_Vz[j]=ForceCrc.dFzz[j]; dMzMax_My[j]=ForceCrc.dMuy[j];
            }
        }
	}

	if (inum <=0) return FALSE; 
//  0－Nx最大时的内力组合	2－My最大时的内力组合  4－Vz最大时的内力组合  6-Mz最大时的内力组合
//  1－Nx最小时的内力组合 3－My最小时的内力组合  5－Vz最小时的内力组合  7-Mz最小时的内力组合
    column_mnv.dMy[0]  =Cng_MomentFromCurrToCode(dNxMax_My[iposi]);   
    column_mnv.dVz[0]  =Cng_ForceFromCurrToCode(dNxMax_Vz[iposi]); 
    column_mnv.dNx[0]  =Cng_ForceFromCurrToCode(dNxMax[iposi]);  
    column_mnv.dMz[0]  =Cng_MomentFromCurrToCode(dNxMax_Mz[iposi]); //  
    column_mnv.iMy[0]  =column_mnv.iMz[0] =column_mnv.iVz[0] =column_mnv.iNx[0] =iNxMaxLcom[iposi];
    column_mnv.dMy1[0] =Cng_MomentFromCurrToCode(ForceLiveNxMax.dMuy[iposi]); 
    column_mnv.dMy2[0] =Cng_MomentFromCurrToCode(ForceDeadNxMax.dMuy[iposi]);   
    if (m_ForceCtrl->Is_LcomForMainAdd(iNxMaxLcom[iposi])) column_mnv.isAddForce[0] = TRUE;

    column_mnv.dMy[1]  =Cng_MomentFromCurrToCode(dNxMin_My[iposi]);   
    column_mnv.dVz[1]  =Cng_ForceFromCurrToCode(dNxMin_Vz[iposi]); 
    column_mnv.dNx[1]  =Cng_ForceFromCurrToCode(dNxMin[iposi]);  
    column_mnv.dMz[1]  =Cng_MomentFromCurrToCode(dNxMin_Mz[iposi]); //  
    column_mnv.iMy[1]  =column_mnv.iMz[1] =column_mnv.iVz[1] = column_mnv.iNx[1] =iNxMinLcom[iposi];
    column_mnv.dMy1[1] =Cng_MomentFromCurrToCode(ForceLiveNxMin.dMuy[iposi]); 
    column_mnv.dMy2[1] =Cng_MomentFromCurrToCode(ForceDeadNxMin.dMuy[iposi]);   
    if (m_ForceCtrl->Is_LcomForMainAdd(iNxMinLcom[iposi])) column_mnv.isAddForce[1] = TRUE;

    column_mnv.dMy[2]  =Cng_MomentFromCurrToCode(dMyMax[iposi]);
    column_mnv.dVz[2]  =Cng_ForceFromCurrToCode(dMyMax_Vz[iposi]); 
    column_mnv.dNx[2]  =Cng_ForceFromCurrToCode(dMyMax_Nx[iposi]);  
    column_mnv.dMz[2]  =Cng_MomentFromCurrToCode(dMyMax_Mz[iposi]);
    column_mnv.iMy[2]  =column_mnv.iMz[2] =column_mnv.iVz[2] = column_mnv.iNx[2] =iMyMaxLcom[iposi];
    column_mnv.dMy1[2] =Cng_MomentFromCurrToCode(ForceLiveMyMax.dMuy[iposi]); 
    column_mnv.dMy2[2] =Cng_MomentFromCurrToCode(ForceDeadMyMax.dMuy[iposi]);
    if (m_ForceCtrl->Is_LcomForMainAdd(iMyMaxLcom[iposi])) column_mnv.isAddForce[2] = TRUE;

    column_mnv.dMy[3]  =Cng_MomentFromCurrToCode(dMyMin[iposi]);
    column_mnv.dVz[3]  =Cng_ForceFromCurrToCode(dMyMin_Vz[iposi]); 
    column_mnv.dNx[3]  =Cng_ForceFromCurrToCode(dMyMin_Nx[iposi]);  
    column_mnv.dMz[3]  =Cng_MomentFromCurrToCode(dMyMin_Mz[iposi]);
    column_mnv.iMy[3]  =column_mnv.iMz[3] =column_mnv.iVz[3] = column_mnv.iNx[3] =iMyMinLcom[iposi];
    column_mnv.dMy1[3] =Cng_MomentFromCurrToCode(ForceLiveMyMin.dMuy[iposi]); 
    column_mnv.dMy2[3] =Cng_MomentFromCurrToCode(ForceDeadMyMin.dMuy[iposi]);
    if (m_ForceCtrl->Is_LcomForMainAdd(iMyMinLcom[iposi])) column_mnv.isAddForce[3] = TRUE;

    column_mnv.dMy[4]  =Cng_MomentFromCurrToCode(dVzMax_My[iposi]);
    column_mnv.dVz[4]  =Cng_ForceFromCurrToCode(dVzMax[iposi]); 
    column_mnv.dNx[4]  =Cng_ForceFromCurrToCode(dVzMax_Nx[iposi]);  
    column_mnv.dMz[4]  =Cng_MomentFromCurrToCode(dVzMax_Mz[iposi]);
    column_mnv.iMy[4]  =column_mnv.iMz[4] =column_mnv.iVz[4] = column_mnv.iNx[4] =iVzMaxLcom[iposi];
    column_mnv.dMy1[4] =Cng_MomentFromCurrToCode(ForceLiveVzMax.dMuy[iposi]); 
    column_mnv.dMy2[4] =Cng_MomentFromCurrToCode(ForceDeadVzMax.dMuy[iposi]);
    if (m_ForceCtrl->Is_LcomForMainAdd(iVzMaxLcom[iposi])) column_mnv.isAddForce[4] = TRUE;

    column_mnv.dMy[5]  =Cng_MomentFromCurrToCode(dVzMin_My[iposi]);
    column_mnv.dVz[5]  =Cng_ForceFromCurrToCode(dVzMin[iposi]); 
    column_mnv.dNx[5]  =Cng_ForceFromCurrToCode(dVzMin_Nx[iposi]);  
    column_mnv.dMz[5]  =Cng_MomentFromCurrToCode(dVzMin_Mz[iposi]);
    column_mnv.iMy[5]  =column_mnv.iMz[5] =column_mnv.iVz[5] = column_mnv.iNx[5] =iVzMinLcom[iposi];
    column_mnv.dMy1[5] =Cng_MomentFromCurrToCode(ForceLiveVzMin.dMuy[iposi]); 
    column_mnv.dMy2[5] =Cng_MomentFromCurrToCode(ForceDeadVzMin.dMuy[iposi]);
    if (m_ForceCtrl->Is_LcomForMainAdd(iVzMinLcom[iposi])) column_mnv.isAddForce[5] = TRUE;

    column_mnv.dMy[6]  =Cng_MomentFromCurrToCode(dMzMax_My[iposi]);
    column_mnv.dVz[6]  =Cng_ForceFromCurrToCode(dMzMax_Vz[iposi]); 
    column_mnv.dNx[6]  =Cng_ForceFromCurrToCode(dMzMax_Nx[iposi]);  
    column_mnv.dMz[6]  =Cng_MomentFromCurrToCode(dMzMax[iposi]);
    column_mnv.iMy[6]  =column_mnv.iMz[6] =column_mnv.iVz[6] = column_mnv.iNx[6] =iMzMaxLcom[iposi];
    column_mnv.dMy1[6] =Cng_MomentFromCurrToCode(ForceLiveVzMin.dMuy[iposi]); 
    column_mnv.dMy2[6] =Cng_MomentFromCurrToCode(ForceDeadVzMin.dMuy[iposi]);
    if (m_ForceCtrl->Is_LcomForMainAdd(iMzMaxLcom[iposi])) column_mnv.isAddForce[6] = TRUE;

    column_mnv.dMy[7]  =Cng_MomentFromCurrToCode(dMzMin_My[iposi]);
    column_mnv.dVz[7]  =Cng_ForceFromCurrToCode(dMzMin[iposi]); 
    column_mnv.dNx[7]  =Cng_ForceFromCurrToCode(dMzMin_Nx[iposi]);  
    column_mnv.dMz[7]  =Cng_MomentFromCurrToCode(dMzMin[iposi]);
    column_mnv.iMy[7]  =column_mnv.iVz[7] = column_mnv.iNx[7] =iMzMinLcom[iposi];
    column_mnv.dMy1[7] =Cng_MomentFromCurrToCode(ForceLiveVzMin.dMuy[iposi]); 
    column_mnv.dMy2[7] =Cng_MomentFromCurrToCode(ForceDeadVzMin.dMuy[iposi]);
    if (m_ForceCtrl->Is_LcomForMainAdd(iMzMinLcom[iposi])) column_mnv.isAddForce[7] = TRUE;

    return TRUE;
}

/************************************************************************/
/* 得到梁截面最外层钢筋到混凝土边缘距离                                 */
/************************************************************************/
BOOL CRCDataCtrl_CH::GetBeamOutRebarDistance(int Elemi,double dh,double& m_das_d,double& m_da_s_d){
	m_das_d = 0.0; m_da_s_d = 0.0;
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(Elemi,ElemD))	{ASSERT(0); return FALSE;}
    //Rbar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;	arRbarPosi.RemoveAll();
 	// Create Rbar by CurrUnit.
	m_pDoc->m_pMatlDB->Create_RebarData(m_CurrUnit.nBase_Length, m_CurrUnit.nBase_Force, TRUE);	// Concrete=TRUE.
    if(!m_pDoc->m_pMatlDB->Exist_RebarData())	return FALSE;
	// Get Rbar.
	_DGN_RBAR_CRC RbarCrc;
    T_RPSC_D RpscD;	RpscD.Initialize();
	T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(ElemD.elpro);
	if(!m_pDoc->m_pAttrCtrl->GetRpsd(OrgSectK,RpscD))	return FALSE;//by maxiao 2007.02.15 Rpsd:设计钢筋
//	int iPosiM = (RpscD.bMbarJ && !bPosiI ? 1 : 0);
	int iPosiM = 0;
	int iMbarSize = RpscD.Mbar[iPosiM].GetSize();
	double dTop = dh, dBot = dh;
	for(int i=0; i<iMbarSize; i++)
	{
		T_RPSC_MBAR RpscMbar = RpscD.Mbar[iPosiM].GetAt(i);
		int iStaZRef		= RpscMbar.iStaZRef;	// 0=Top, 1=Bottom.
		if (iStaZRef==0) {
			dTop = min(RpscMbar.dStaZ, dTop);
			dBot = min(dh - RpscMbar.dStaZ, dBot);
		}else if (iStaZRef ==1) {
			dBot = min( RpscMbar.dStaZ, dBot);
			dTop = min(dh -RpscMbar.dStaZ, dTop);
		}
	}
	dBot		= Cng_LengthFromCurrToCode(dBot,1);
	dTop		= Cng_LengthFromCurrToCode(dTop,1);
	m_das_d  =   (dBot>=dh/2.0) ? 0.0:dBot ; 
	m_da_s_d =   (dTop>=dh/2.0) ? 0.0:dTop ;

	return TRUE;	
}
/************************************************************************/
/* 得到柱截面最外层钢筋到混凝土边缘距离（上下）                         */
/************************************************************************/
BOOL CRCDataCtrl_CH::GetColumnOutRebarDistance(int Elemi,double dh,double& m_das_d,double& m_da_s_d){
	m_das_d = 0.0; m_da_s_d = 0.0;
	T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(Elemi,ElemD))	{ASSERT(0); return FALSE;}

	// Before_Release_v825_Check - Rebar 备炼眉 函版登搁辑 荐沥登绢具 窍绰 何盒 
	// 老窜 0 栏肺 绊沥秦滴菌促~	

    T_RCHK_D RchkData;		RchkData.Initialize();

    if(!m_pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro, RchkData)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 的配筋数据有误."),Elemi);
        return FALSE;
    }
	double dDis=dh;
    int CONST_RCSC_iBAR_LAY		=  5;
	for(int j=0; j<CONST_RCSC_iBAR_LAY; j++)//柱钢筋层数
    {
        if(RchkData.COLM.dDc[0][j] > m_dZero)		dDis = min(dDis,RchkData.COLM.dDc[0][j] );
    }
	m_das_d  =   dDis ; 
	m_da_s_d =   dDis ;

	return TRUE;	
}

BOOL CRCDataCtrl_CH::Is_CalcPscItem(T_ELEM_K ElemK, UINT CheckItem)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    BOOL bCalcI = Get_ChkPscFlag(ElemK, 0, CheckItem);
    BOOL bCalcJ = Get_ChkPscFlag(ElemK, 1, CheckItem);
    if(bCalcI || bCalcJ)  return TRUE;
    return FALSE;
}

/************************************************************************/
/* sure whether need check the Elem's section   
     CheckItem:0-moment; 1-shear;                                         */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_ChkPscFlag(T_ELEM_K ElemK, int iPosi, UINT CheckItem){
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int iPoscNum = pDoc->m_pAttrCtrl->GetCountPosc();
	T_POSC_D PoscD;
	PoscD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetPosc(ElemK, PoscD))	return (iPoscNum==0 ? TRUE : FALSE);	// Default.
	if(CheckItem ==0)	// Mom.
	{
		if(PoscD.iMomChk==0)									return FALSE;
		else if(PoscD.iMomChk==1 && iPosi==1)	return FALSE;
		else if(PoscD.iMomChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	else if(CheckItem ==1)	// Shear.
	{
		if(PoscD.iShrChk==0)									return FALSE;
		else if(PoscD.iShrChk==1 && iPosi==1)	return FALSE;
		else if(PoscD.iShrChk==2 && iPosi==0)	return FALSE;
		else	return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/*get PSC degree                                                        */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_PscDegree(T_ELEM_K ElemK, _DGN_STRS_CRC& StrLcomMax, _DGN_STRS_CRC& StrSgld,int& iSerialLcom){
	StrLcomMax.Initialize(); StrSgld.Initialize();iSerialLcom=0;

 	CDBDoc* pDoc = CDBDoc::GetDocPoint();	ASSERT(pDoc);

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
	_DGN_STRS_CRC StrTnpr, StrTnsc;
	StrTnpr.Initialize();StrTnsc.Initialize();
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
	int i=0, j=0;
	double dMaxmax=0.0, dmax=0.0;
	for(i=0; i<aForceLcom.GetSize(); i++)	{
		_DGN_STRS_CRC StrLcom; StrLcom.Initialize();
		if(m_pDesignForceResult->GetStressLcomResult(EPairK,i+1,StrLcom)){
			for (j=0;j<2;j++) {
				dmax = max(max(max(StrLcom.dSbl[j],StrLcom.dSbr[j]),max(StrLcom.dStl[j],StrLcom.dStr[j])),max(StrLcom.dSbot[j],StrLcom.dStop[j]));
				dMaxmax = max(max(max(StrLcomMax.dSbl[j],StrLcomMax.dSbr[j]),max(StrLcomMax.dStl[j],StrLcomMax.dStr[j])),max(StrLcomMax.dSbot[j],StrLcomMax.dStop[j]));				
				if (dmax >= dMaxmax) {
					StrLcomMax = StrLcom;
					iSerialLcom = i+1;		
				}
			}
		}else{
			return FALSE;
		}		
	}
	if (iSerialLcom==0)return FALSE;
//////////////////////////////////////////////////////////////////////////
	int iDgnStatus = pDoc->GetDesignStatus();
	pDoc->SetDesignStatus(5);
	_DGN_STRS_CRC StrsMax1; StrsMax1.Initialize();
	_DGN_STRS_CRC StrsMin1; StrsMin1.Initialize();
	_DGN_STRS_CRC StrsMax2; StrsMax2.Initialize();
	_DGN_STRS_CRC StrsMin2; StrsMin2.Initialize();
	
	T_STAG_K StagK = pDoc->m_pPostCtrl->GetStageInfo()->GetFinalStage();			int iFinalStepNo=0;

	// Get Last StepNo at Current Stage.
	m_DataCtrl.Get_LastStepInCurStage(StagK, iFinalStepNo);
	CString strSgldName = _LS(IDS_DB_SGLD_TP);	//刚束一次.
	T_SGLD_K SgldK = pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);			
	if(!m_ForceCtrl->Get_StrsBySgld(EPairK, SgldK, StagK, iFinalStepNo, StrsMax1, StrsMin1))	return FALSE;

	strSgldName = _LS(IDS_DB_SGLD_TS);	//刚束二次.			
	SgldK = pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
	if(!m_ForceCtrl->Get_StrsBySgld(EPairK, SgldK, StagK, iFinalStepNo, StrsMax2, StrsMin2))	return FALSE;
	for (j=0;j<2;j++) {
		StrSgld.dSbl[j]      = StrsMax1.dSbl[j] + StrsMax2.dSbl[j];
		StrSgld.dSbot[j]     = StrsMax1.dSbot[j] + StrsMax2.dSbot[j];
		StrSgld.dSbr[j]      = StrsMax1.dSbr[j] + StrsMax2.dSbr[j];
		StrSgld.dStl[j]      = StrsMax1.dStl[j] + StrsMax2.dStl[j];
		StrSgld.dStop[j]     = StrsMax1.dStop[j] + StrsMax2.dStop[j];
		StrSgld.dStr[j]      = StrsMax1.dStr[j] + StrsMax2.dStr[j];
		StrSgld.dExtraRat[j] = StrsMax1.dExtraRat[j] + StrsMax2.dExtraRat[j];
		StrSgld.dScom[j]     = StrsMax1.dScom[j] + StrsMax2.dScom[j];
	}
	return TRUE;
}

/************************************************************************/
/*get PSC stress by stage                                               */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_Con_CS(T_ELEM_K ElemK, T_CON_CS_CH& concs){
	concs.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	int iDgnStatus = pDoc->GetDesignStatus();
	pDoc->SetDesignStatus(5);

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	_DGN_STRS_CRC StrsMax; StrsMax.Initialize();
	_DGN_STRS_CRC StrsMin; StrsMin.Initialize();
	CArray<T_STAG_K,T_STAG_K> aStagK;
	pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
	if(aStagK.GetSize()==0)	return FALSE;
	int i=0;
	for(i=0; i<aStagK.GetSize(); i++)
	{
        T_STAG_K StagK = aStagK.GetAt(i);
		int iFinalStepNo=0;
		// Get Last StepNo at Current Stage.
		Get_LastStepInCurStage(StagK, iFinalStepNo);
		CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
		T_SGLD_K SgldK = pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
		if(!m_ForceCtrl->Get_StrsBySgld(EPairK, SgldK, StagK, iFinalStepNo, StrsMax, StrsMin))	continue;
	}
	// Set Data.
	for(i=0; i<4; i++)
	{
		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		_DGN_STRS_CRC StrsCrc = (i%2==0 ? StrsMax : StrsMin);
		if(StrsCrc.dScom[i/2]==0.0)	continue;
		concs.dataBase[i].StagK	= StrsCrc.iRefNo[i/2];
		concs.dataBase[i].dSig_T  	= StrsCrc.dStop[i/2];
		concs.dataBase[i].dSig_B  	= StrsCrc.dSbot[i/2];
		concs.dataBase[i].dSig_TL 	= StrsCrc.dStl[i/2];
		concs.dataBase[i].dSig_BL 	= StrsCrc.dSbl[i/2];
		concs.dataBase[i].dSig_TR 	= StrsCrc.dStr[i/2];
		concs.dataBase[i].dSig_BR 	= StrsCrc.dSbr[i/2];
		concs.dataBase[i].dSig_MAX  = StrsCrc.dScom[i/2];
		concs.dataBase[i].dSig_ALW	= 0.0;	// From CRC.
	}	
	return TRUE;
}

/************************************************************************/
/*get tendon matl.      iLoadType:0=Pre-tension, 1=Post-tension.        */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_TendonMatl(T_TDNA_K TdnaK,int& iLoadType ,double& dfpk){
	// Tendon Profile.
	T_TDNA_D TdnaD;
	TdnaD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD))	{ASSERT(0); return FALSE;}
	// Tendon Property.
	T_TDNT_D TdntD;
	TdntD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetTdnt(TdnaD.TendonTypeKey,TdntD))	{ASSERT(0); return FALSE;}
	T_MATD_D MatdD;
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(TdntD.MatlKey,MatdD))	{ASSERT(0);return FALSE;}
	if (MatdD.Data1.CodeName== MATLCODE_STL_TB05) {
		CString  tendon   = MatdD.Data1.CodeMatlName;  //预应力的标号 
		if     (tendon==_T("PSB830"))   {dfpk=830;    }         //
		else if(tendon==_T("Wire1470")) {dfpk=1470;   }      //
		else if(tendon==_T("Wire1570")) {dfpk=1570;   }      //
		else if(tendon==_T("Wire1670")) {dfpk=1670;   }      //
		else if(tendon==_T("Wire1770")) {dfpk=1770;   }      //
		else if(tendon==_T("Wire1860")) {dfpk=1860;   }      //
		else if(tendon==_T("Strand1470")) {dfpk=1470; }      //
		else if(tendon==_T("Strand1570")) {dfpk=1570;  }      //
		else if(tendon==_T("Strand1670")) {dfpk=1670; }      //
		else if(tendon==_T("Strand1720")) {dfpk=1720; }      //
		else if(tendon==_T("Strand1770")) {dfpk=1770; }      //
		else if(tendon==_T("Strand1820")) {dfpk=1820; }      //
		else if(tendon==_T("Strand1860")) {dfpk=1860; }      //
    }else {/*ASSERT(0);	*/return FALSE;}
	iLoadType = TdntD.nLoadType;// 0=Pre-tension, 1=Post-tension.	
	return TRUE;
}

/************************************************************************/
/* get psc section                                                      */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_PscSect(T_ELEM_K ElemK,int iPosi, BOOL isBeam, _TB_PSC_SECT& sect){
	sect.Initialize(); 
	if(!m_pDoc->IsPostMode())	ASSERT(0);
	// Get Elem.
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK,ElemD)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少单元数据."),ElemK);
        return FALSE;
    }  

	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	BOOL bPosiI = (iPosi==0 ? TRUE : FALSE);
	double dCzp =.0,dCzm=.0, dU =.0;
	if (isBeam) {//beam  
 		T_SPSC_SECT SpscSect;  SpscSect.Initialize();
		BOOL bClosedSect=FALSE;//not closed section.
        int  nDgnSectType = -1;
		double dAg=0.0, dAoh=0.0, dPh=0.0, dKt=0.0, dWt=0.0, dTorThk=0.0;
        double aKt[6]={0.0};
//		double dpMomPos[4][2]={0.,0.,0.,0.,0.,0.,0.,0.};
		double dpShrPos[3]={0.,0.,0.}, dpShrThk[3]={0.,0.,0.}, dFlgThk[2]={0.,0.};
		double dPcp=0.0, dAcp=0.0;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arInner;
		if(!CSectUtil::GetPscSectCPR(TRUE, EPairK, bPosiI, FALSE, TRUE, SpscSect))	return FALSE;
		if(!CSectUtil::GetPscCoordList(TRUE, ElemD.elpro, bPosiI, arOuter, arInner))	return FALSE;
		// Change Unit (CurrUnit -> CodeUnit).
		SpscSect.dyDim	= Cng_LengthFromCurrToCode(SpscSect.dyDim, 1);
		SpscSect.dzDim	= Cng_LengthFromCurrToCode(SpscSect.dzDim, 1);
		SpscSect.dyCen	= Cng_LengthFromCurrToCode(SpscSect.dyCen, 1);
		SpscSect.dzCen	= Cng_LengthFromCurrToCode(SpscSect.dzCen, 1);
		SpscSect.dArea	= Cng_LengthFromCurrToCode(SpscSect.dArea, 2);
		//sure the 单位换算
		SpscSect.dAsy  = Cng_LengthFromCurrToCode(SpscSect.dAsy,2);
		SpscSect.dAsz	 = Cng_LengthFromCurrToCode(SpscSect.dAsz,2);

		// Get Aoh, Ph, Kt, FlgThk.
		double dPyKt[6]={0.,0.,0.,0.,0.,0.}, dPzKt[6]={0.,0.,0.,0.,0.,0.};
        BOOL bDivSect4Tor=FALSE;
        int nDivPart=0;
        double dCutPos[3]={0.};  
        double dLbarThk=0.0;
		int nWebSize = 1;
		if(!CSectUtil::GetPscKtAmPoint(TRUE, ElemD.elpro, bPosiI, bClosedSect, nDgnSectType, dKt, aKt, dWt, dPyKt, dPzKt, dFlgThk, bDivSect4Tor, nDivPart, dCutPos, nWebSize))	return FALSE;
		if(!CSectUtil::GetPscPeriAcor(TRUE, EPairK, bPosiI, dPh, dAoh, dLbarThk))	return FALSE;
		dAoh = Cng_LengthFromCurrToCode(dAoh,2);
		dPh	 = Cng_LengthFromCurrToCode(dPh ,1);
		dKt	 = Cng_LengthFromCurrToCode(dKt ,3);
		dWt	 = Cng_LengthFromCurrToCode(dWt ,3);
		dFlgThk[0] = Cng_LengthFromCurrToCode(dFlgThk[0],1);
		dFlgThk[1] = Cng_LengthFromCurrToCode(dFlgThk[1],1);
		// Get Position & Thickness for Shear/Torsion.
		T_SECT_D SectD;	SectD.Initialize();
		BOOL bGetSect=FALSE;
		if(!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro,SectD))	return FALSE;
		BOOL bGetI = CDBLib::IsSectI(bPosiI,SectD.nStype);
		T_SPSC_DSHR SpscDshr;  SpscDshr.Initialize();
		CSectUtil::GetPscShearPosi(TRUE, ElemD.elpro, bPosiI, SpscDshr, TRUE);
		dAg					= Cng_LengthFromCurrToCode((bGetI ? SectD.SectBefore.SectI.Stiffness.Area : SectD.SectBefore.SectJ.Stiffness.Area), 1);
		dpShrPos[0] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][0],1);	// Left, Top.
		dpShrPos[1] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][2],1);	// Left, Center.
		dpShrPos[2] = Cng_LengthFromCurrToCode(SpscDshr.dyz[0][4],1);	// Left, Bot.
		dpShrThk[0] = Cng_LengthFromCurrToCode(SpscDshr.db[0],1);
		dpShrThk[1] = Cng_LengthFromCurrToCode(SpscDshr.db[1],1);
		dpShrThk[2] = Cng_LengthFromCurrToCode(SpscDshr.db[2],1);
		dTorThk			= Cng_LengthFromCurrToCode(SpscDshr.dKtThik,1);

		double dbw = min(min(dpShrThk[0],dpShrThk[1]),dpShrThk[2]);
		// Section
		double dyDim = SpscSect.dyDim;    double dzDim = SpscSect.dzDim;
		double dyCen = SpscSect.dyCen;    double dzCen = SpscSect.dzCen;
		double dCpy  = dyDim - dyCen;     double dCpz  = dzDim - dzCen;

		//add 07.17
		double dYbar = (bGetI ? SectD.SectBefore.SectI.Design.YBar : SectD.SectBefore.SectJ.Design.YBar);
		double dZbar = (bGetI ? SectD.SectBefore.SectI.Design.ZBar : SectD.SectBefore.SectJ.Design.ZBar);
		double dArea = (bGetI ? SectD.SectBefore.SectI.Stiffness.Area : SectD.SectBefore.SectJ.Stiffness.Area);

		// Get Matl Data.
		T_MATL_D MatlD;
		MatlD.Initialize();
		BOOL bGetMatl=FALSE;		double dEc=0.0; 
		if(m_pDoc->m_pPostCtrl->GetMatlPost(ElemD.elmat,MatlD)){
			if(MatlD.Type==_T("C"))					dEc = MatlD.Data1.Analysis.Elast;
			else if(MatlD.Type==_T("SRC"))	dEc = MatlD.Data2.Analysis.Elast;
			else												dEc = 0.0;
		}
		// Get Rbar Data.
		double dAss=0.0, dEs=0.0;
		double dyzR[2]={0.,0.};
		T_SPSC_RBAR SpscRbar;
		SpscRbar.Initialize();
		if(CSectUtil::GetPscRebarPosi(TRUE,ElemD.elpro,bPosiI,TRUE,SpscRbar))
		{
			dAss	= SpscRbar.dAs;       dEs = SpscRbar.dEs;
			dyzR[0] = SpscRbar.dyz[0];  dyzR[1] = SpscRbar.dyz[1];
		}
		double dRatEs = (dEc==0.0 ? 0.0 : (dEs/dEc));
		////////////////////////////
		// Get Tndn Data.
		double dAp=0.0, dEp=0.0;
		double dyzP[2]={0.,0.};
		T_SPSC_RBAR SpscTndn;
		SpscTndn.Initialize();
		if(CSectUtil::GetPscTndnPosi(TRUE,EPairK,bPosiI,SpscTndn))
		{
			dAp	= SpscTndn.dAs;         dEp	= SpscTndn.dEs;
			dyzP[0] = SpscTndn.dyz[0];  dyzP[1] = SpscTndn.dyz[1];
		}
		double dRatEp = (dEc==0.0 ? 0.0 : dEp/dEc);	// Applied dEp=(dEp-dEc) at GetPscTndnPosi().
		// Calculate Combined Data.
		double dACom = dArea + dAss*dRatEs + dAp*dRatEp;
		double dyCom = (dArea*dYbar + dAss*dRatEs*(dyzR[0]+dYbar) + dAp*dRatEp*(dyzP[0]+dYbar)) / dACom;
		double dzCom = (dArea*dZbar + dAss*dRatEs*(dyzR[1]+dZbar) + dAp*dRatEp*(dyzP[1]+dZbar)) / dACom;

		// Get Position for Moment.
		double dx1,dx2,dx3,dx4,dy1,dy2,dy3,dy4;
		dx1 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dx1 : SectD.SectBefore.SectJ.Stiffness.dx1);
		dy1 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dy1 : SectD.SectBefore.SectJ.Stiffness.dy1);
		dx2 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dx2 : SectD.SectBefore.SectJ.Stiffness.dx2);
		dy2 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dy2 : SectD.SectBefore.SectJ.Stiffness.dy2);
		dx3 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dx3 : SectD.SectBefore.SectJ.Stiffness.dx3);
		dy3 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dy3 : SectD.SectBefore.SectJ.Stiffness.dy3);
		dx4 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dx4 : SectD.SectBefore.SectJ.Stiffness.dx4);
		dy4 = (bGetI ? SectD.SectBefore.SectI.Stiffness.dy4 : SectD.SectBefore.SectJ.Stiffness.dy4);

		sect.dpMomPos[0][0] = Cng_LengthFromCurrToCode(dx1 + (dYbar-dyCom) , 1);
		sect.dpMomPos[0][1] = Cng_LengthFromCurrToCode(dy1 + (dZbar-dzCom) , 1);
		sect.dpMomPos[1][0] = Cng_LengthFromCurrToCode(dx2 + (dYbar-dyCom) , 1);
		sect.dpMomPos[1][1] = Cng_LengthFromCurrToCode(dy2 + (dZbar-dzCom) , 1);
		sect.dpMomPos[2][0] = Cng_LengthFromCurrToCode(dx3 + (dYbar-dyCom) , 1);
		sect.dpMomPos[2][1] = Cng_LengthFromCurrToCode(dy3 + (dZbar-dzCom) , 1);
		sect.dpMomPos[3][0] = Cng_LengthFromCurrToCode(dx4 + (dYbar-dyCom) , 1);
		sect.dpMomPos[3][1] = Cng_LengthFromCurrToCode(dy4 + (dZbar-dzCom) , 1);

		T_BMPD_D SectData;
		if (!m_pDoc->m_pPostCtrl->GetBmpd(ElemK, SectData))return FALSE;
		sect.dCzp   = Cng_LengthFromCurrToCode(SectData.dStiffness[iPosi][6], 1);// [i,j] [Area, Ixx, Iyy, Izz, Cyp, Cym, Czp, Czm, WArea] : 窜搁 漂己 
		sect.dCzm   = Cng_LengthFromCurrToCode(SectData.dStiffness[iPosi][7], 1);
		sect.dIyy   = Cng_LengthFromCurrToCode(SectData.dStiffness[iPosi][2], 4);
		sect.dIzz   = Cng_LengthFromCurrToCode(SectData.dStiffness[iPosi][3], 4);
		sect.dArea  = Cng_LengthFromCurrToCode(SectData.dStiffness[iPosi][0], 2);

		dCzp = sect.dCzp;   dCzm = sect.dCzm;

		//section
		double dh  = dzDim;       double dbf1 = dyDim;
	  double dbf2 = fabs(sect.dpMomPos[2][0] - sect.dpMomPos[3][0]); // 滴痢狼 辨捞啊 酒丛 x谅钎 狼 瞒捞肺窃.
        double dhf1 =dFlgThk[0];  double dhf2 =dFlgThk[1];
	
		//get S0
		double dQzb  = (bGetI ? SectD.SectBefore.SectI.Stiffness.Qzb : SectD.SectBefore.SectJ.Stiffness.Qzb);
		double dQyb  = (bGetI ? SectD.SectBefore.SectI.Stiffness.Qyb : SectD.SectBefore.SectJ.Stiffness.Qyb);
		dQzb = Cng_LengthFromCurrToCode(dQzb,2);
		dQyb = Cng_LengthFromCurrToCode(dQyb,2);
		sect.dS0 = dQyb*dpShrThk[1];	
        
		/////////读取普通钢筋的面积,保护层厚度以及钢筋总的周边长度(P63)/////////////////////
		double dat=0.0,dab=0.0,dAs=.02,dAs1=.0,das=.0,das1=.0;
		Get_rebarData(ElemK,iPosi,dh,dAs,dAs1,das,das1,dU,dat,dab);	
		sect.db     = dbw;   sect.dh     = dh;
		sect.db_f   = dbf1;  sect.dbf    = dbf2;
		sect.dh_f   = dhf1;  sect.dhf    = dhf2;
		sect.dAs    = dAs;   sect.dA_s   = dAs1;
		sect.das    = das;   sect.da_s   = das1;
		sect.dU     = dU;
	}
	else{	//column
		_DGN_RC_COLUMN_SECTION_CH ColumnSection;
    	if (!Get_ColumnSection(ElemK,iPosi,ColumnSection)) return FALSE;
		sect.db     = ColumnSection.db;    sect.dh     = ColumnSection.dh;   
        sect.db_f   = ColumnSection.dbf1;  sect.dbf    = ColumnSection.dbf2; 
        sect.dh_f   = ColumnSection.dhf1;  sect.dhf    = ColumnSection.dhf2; 
        sect.dL0    = ColumnSection.dl0;  
		sect.dArea  = ColumnSection.dA;
		sect.dIyy		= ColumnSection.dIyy;  sect.dIzz   = ColumnSection.dIzz;
		sect.dCzp   = ColumnSection.dCzp;  sect.dCzm   = ColumnSection.dCzm;
		sect.dAs    = ColumnSection.dAs;   sect.dA_s   = ColumnSection.dAs1;
		sect.das    = ColumnSection.das;   sect.da_s   = ColumnSection.das1;
		T_SECT_D sData;	sData.Initialize();
	  int iSect = ElemD.elpro;
		m_pDoc->m_pAttrCtrl->GetSectDesign(iSect, sData);
		sect.dpMomPos[0][0] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dx1 , 1);
		sect.dpMomPos[0][1] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dy1 , 1);
		sect.dpMomPos[1][0] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dx2 , 1);
		sect.dpMomPos[1][1] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dy2 , 1);
		sect.dpMomPos[2][0] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dx3 , 1);
		sect.dpMomPos[2][1] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dy3 , 1);
		sect.dpMomPos[3][0] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dx4 , 1);
		sect.dpMomPos[3][1] = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.dy4 , 1);
		double dQzb  = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Qzb , 2);
		double dQyb  = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Qyb , 2);
		dCzp	       = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Czp , 1);
		dCzm	       = Cng_LengthFromCurrToCode(sData.SectBefore.SectI.Stiffness.Czm , 1);
		sect.dS0    = dQyb*sect.db;
		dU =4* (2*sqrt(sect.dAs*PI));
	}
////////////////////////////////////////////////////////////////////////////////////	 
	double dAp=.0;	double dA_p=.0;	double dApb=.0; 	double dA_pb=.0;  
	double dAv=.0; double dap=.0;   double da_p=.0;	double dapb=.0;  
	double da_pb=.0;	double dApbsina=.0;	double dA_pbsina=.0;  
	double dApbcosa=.0; 	double dA_pbcosa=.0; 	double dsv=.0;
/////////读取预应力钢筋的预应力,预应力损失,面积,保护层厚度以及钢筋总的周边长度//////
	 BOOL bIsGrup = FALSE;	// Grup (NOT Sngl).                                       //
	 double dMuyMax=0.0, dMuyMin=0.0;                                               //
	 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;                               //
	 m_DataCtrl.Get_ConTndn(EPairK,bPosiI,bIsGrup,dMuyMax,dMuyMin, arTndnPosi);     //
	 double Tolap=0,Tola_p=0,Tolapb=0,Tola_pb=0;                                    //
	 int Tolapnum=0,Tola_pnum=0,Tolapbnum=0,Tola_pbnum=0;                           //
	 double dStreX,dStreZ,dStreXZ,dSina,dCosa;    
	 T_MATD_D MatdD;	MatdD.Initialize();                                           
	 for( int k=0; k<arTndnPosi.GetSize(); k++)                                          //
	 {                                                                              //
			_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(k);                                   //
            if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(Tndn.MatlK,MatdD))	{ASSERT(0); return FALSE;}
//			iLoadType=Tndn.iLoadType;                                                   //
			dStreX  = Tndn.dStress[0]; // x应力                                         //                     
			dStreZ  = Tndn.dStress[2]; // z应力                                         //
			dStreXZ = sqrt(dStreX*dStreX + dStreZ*dStreZ);                              //
			dSina   = (dStreXZ==0.0 ? 0.0 : dStreZ/dStreXZ);                            //
			dCosa   = (dStreXZ==0.0 ? 0.0 : dStreX/dStreXZ);                            //
			dU+=2*sqrt(Tndn.dArea*PI);                                                //
	    if(dSina==0)                                                                //
			{                                                                           //
				if(Tndn.dz>0.0) { dA_p+=Tndn.dArea; Tola_p+=dCzp-Tndn.dz; Tola_pnum++;}   //
			  else            { dAp+=Tndn.dArea;  Tolap+=dCzm-fabs(Tndn.dz);Tolapnum++;}//
			  dU+=2*sqrt(Tndn.dArea*PI);                                                //
			}                                                                           //
			else                                                                        //
			{ 	                                                                        //
				if(Tndn.dz>0.0)                                                           //
				{                 
                    dA_p+=Tndn.dArea; Tola_p+=dCzp-Tndn.dz; Tola_pnum++;//add
					dA_pbsina+=Tndn.dArea*fabs(dSina); dA_pbcosa+=Tndn.dArea*fabs(dCosa);   //
				  dA_pb+=Tndn.dArea; Tola_pb+=dCzp-Tndn.dz; Tola_pbnum++;	                //
				}                                                                         //
			  else                                                                      //
				{                                                                         //
                    dAp+=Tndn.dArea;  Tolap+=dCzm-fabs(Tndn.dz);Tolapnum++;//add
					dApbsina+=Tndn.dArea*fabs(dSina);  dApbcosa+=Tndn.dArea*fabs(dCosa);    //
				  dApb+=Tndn.dArea;  Tolapb+=dCzm-fabs(Tndn.dz); Tolapbnum++;             //
				}                                                                         //
			}                                                                           //
	 }                                                                              //
	 dap=Tolapnum==0?0:Tolap/Tolapnum;                                              // 
	 da_p=Tola_pnum==0?0:Tola_p/Tola_pnum;                                          //
	 dapb=Tolapbnum==0?0:Tolapb/Tolapbnum;                                          //
	 da_pb=Tola_pbnum==0?0:Tola_pb/Tola_pbnum;                                      //
///////////////读取斜截面抗剪的相关数据/////////////////////////////////////////////
	 T_RPSC_SBAR RpscSbar;                                                          //
	 RpscSbar.Initialize();                                                         //
	 double dfdt=0.0, dfdb=0.0, dVd=0.0, dMd=0.0;                                   //
	 m_DataCtrl.Get_ConRshr(EPairK, bPosiI, dfdt, dfdb, dVd, dMd, RpscSbar);//
         //Modify by Juhonghua 20151126
	 dAv=Cng_LengthFromCurrToCode(RpscSbar.dStirAw , 2);                                                          //
	 dsv=Cng_LengthFromCurrToCode(RpscSbar.dStirPitch , 1);                                                       //
////////////////////////////////////////////////////////////////////////////////////
	sect.dAp    = dAp;               sect.dA_p   = dA_p;              
	sect.dApb   = dApb;              sect.dA_pb  = dA_pb;             
	sect.dAv    = dAv;               
	sect.dap    = dap;               sect.da_p   = da_p;              
	sect.dapb   = dapb;              sect.da_pb  = da_pb;             
	sect.dApbsina=dApbsina;          sect.dA_pbsina= dA_pbsina;       
	sect.dApbcosa = dApbcosa;        sect.dA_pbcosa= dA_pbcosa;       
	sect.dsv      = dsv;       
 	sect.dU       = dU;
	//矩形: //    bf' = bf = b   //hf' = h   //hf = 0   //T 形：//    bf = b
    if(sect.dbf <1.0E-07) sect.dbf = sect.db; if(sect.db_f <1.0E-07) sect.db_f = sect.db;    if(sect.dh_f <1.0E-07) sect.dh_f = sect.dh;    
    if(sect.dh <=0.0 || sect.db <= 0.0 ||sect.dArea <=0.0 ) return FALSE;

	if(fabs(sect.dhf-sect.dh) <1.0E-07) sect.dhf = .0;
	if(fabs(sect.dh_f-sect.dh) <1.0E-07) sect.dh_f = .0;
	return TRUE;
}

BOOL CRCDataCtrl_CH::Get_TSelfStressFordgn(T_ELEM_K ElemK,int iLCom,UINT iDimType,BOOL isBeam,_DGN_STRS_CRC& aStress)
{
	aStress.Initialize();
	_DGN_LCOM LcomD;
	if(!m_ForceCtrl->Get_DgnLcomData(iLCom,LcomD))
		return FALSE;

	_TB_PSC_SECT sect[2];
	if(!Get_PscSect(ElemK,0,isBeam,sect[0]))
		return FALSE;
	if(!Get_PscSect(ElemK,1,isBeam,sect[1]))
		return FALSE;


	for (int k=0;k<LcomD.LcomUlData.Combination.GetSize();k++)
	{
		T_LCOM_BASE caseinfo = LcomD.LcomUlData.Combination.GetAt(k);
		if(caseinfo.AnalType == D_LOADCASE_STATIC)
		{
			T_STLD_D stldD;
			if(!m_pDoc->m_pAttrCtrl->GetStld(caseinfo.LoadCaseKey, stldD))
			{
				continue;
			}
			if(stldD.LoadCaseType!=_T("TPG"))
				continue;

			T_LCOM_D LcasD;
			LcasD.Initialize();
			if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STATIC, caseinfo.LoadCaseKey, LcasD))	{continue;}
			else						m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);

			T_STRB_D t_strb;
			m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, 0, 0, &t_strb, TRUE);


			double dStop[cPOSI_CRC], dSbot[cPOSI_CRC];	// 惑窜, 窍窜 览仿档.
			double dStl[cPOSI_CRC], dSbl[cPOSI_CRC];		// 惑窜谅窜, 窍窜谅窜 览仿档.
			double dStr[cPOSI_CRC], dSbr[cPOSI_CRC];		// 惑窜快窜, 窍窜快窜 览仿档.
			double dSxx[cPOSI_CRC];  
			int iPart,iKind1,iKind2;
			for(int j=0; j<cPOSI_CRC; j++)
			{	// I,J.				
				//计算次应力
				if(j==0)
				{
					iPart	 = 0;
					iKind2 = 0;      
				}
				else
				{
					iPart	 = 3;
					iKind2 = 1;
				}
				//modify by juhonghua 20160405 (T SELF STRESS UNIT)
				double dN = Cng_ForceFromCurrToCode(t_strb.dblForce[iPart][j==0 ? 0 : 6]);
				double dMy = Cng_MomentFromCurrToCode(t_strb.dblForce[iPart][j==0 ? 4 : 10]);
				double dMz = Cng_MomentFromCurrToCode(t_strb.dblForce[iPart][j==0 ? 5 : 11]);
				TRACE(_T("\n//////////*****Elem:%u ,IJ=%d ,LC=%s,T Self Stress:******///////\n"),ElemK,j,stldD.LoadCaseName);
				TRACE(_T("N=%g,My=%g,Mz=%g\n"),dN,dMy,dMz);
				TRACE(_T("Area=%g,Iyy=%g,Izz=%g\n"),sect[j].dArea,sect[j].dIyy,sect[j].dIzz);
				TRACE(_T("Pos1=%g,%g\n"),sect[j].dpMomPos[0][1],sect[j].dpMomPos[0][0]);
				TRACE(_T("Pos2=%g,%g\n"),sect[j].dpMomPos[1][1],sect[j].dpMomPos[1][0]);
				TRACE(_T("Pos3=%g,%g\n"),sect[j].dpMomPos[2][1],sect[j].dpMomPos[2][0]);
				TRACE(_T("Pos4=%g,%g\n"),sect[j].dpMomPos[3][1],sect[j].dpMomPos[3][0]);

				TRACE(_T("SigMy=%g,%g\n"),(dMy) * sect[j].dpMomPos[0][1] / sect[j].dIyy,(dMy) * sect[j].dpMomPos[3][1] / sect[j].dIyy);
				TRACE(_T("SigMz_top=%g,%g\n"),(dMz) * sect[j].dpMomPos[0][0] / sect[j].dIzz,(dMz) * sect[j].dpMomPos[1][0] / sect[j].dIzz);
				TRACE(_T("SigMz_bot=%g,%g\n"),(dMz) * sect[j].dpMomPos[2][0] / sect[j].dIzz,(dMz) * sect[j].dpMomPos[3][0] / sect[j].dIzz);


				dSxx[j] = (dN) / sect[j].dArea;
				TRACE(_T("dSxx[%d] = %g\n"),j,dSxx[j]);
				dStl[j] = dSxx[j] + (dMy) * sect[j].dpMomPos[0][1] / sect[j].dIyy+ (dMz) * sect[j].dpMomPos[0][0] / sect[j].dIzz;
				TRACE(_T("dStl[%d] = %g\n"),j,dStl[j]);
				dStr[j] = dSxx[j] + (dMy) * sect[j].dpMomPos[1][1] / sect[j].dIyy+ (dMz) * sect[j].dpMomPos[1][0] / sect[j].dIzz;
				TRACE(_T("dStr[%d] = %g\n"),j,dStr[j]);
				dSbr[j] = dSxx[j] + (dMy) * sect[j].dpMomPos[2][1] / sect[j].dIyy+ (dMz) * sect[j].dpMomPos[2][0] / sect[j].dIzz;
				TRACE(_T("dSbr[%d] = %g\n"),j,dSbr[j]);
				dSbl[j] = dSxx[j] + (dMy) * sect[j].dpMomPos[3][1] / sect[j].dIyy+ (dMz) * sect[j].dpMomPos[3][0] / sect[j].dIzz;
				TRACE(_T("dSbl[%d] = %g\n"),j,dSbl[j]);

				dStop[j] = dSxx[j] + (dMy) * sect[j].dCzp / sect[j].dIyy;
				TRACE(_T("dStop[%d] = %g\n"),j,dStop[j]);
				dSbot[j] = dSxx[j] + (dMy) * -1.0*sect[j].dCzm / sect[j].dIyy;
				TRACE(_T("dSbot[%d] = %g\n"),j,dSbot[j]);
			}


			for(int j=0; j<cPOSI_CRC; j++)
			{	// I,J.				
				//计算次应力

				if(j==0)
				{
					iPart  = 0;
					iKind1 = 0;
					iKind2 = 0;      
				}
				else
				{
					iPart  = 3;
					iKind1 = 7;
					iKind2 = 1;
				}

				aStress.dStl[j]  += caseinfo.Factor * (Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+16]) - dStl[j]);
				aStress.dStr[j]  += caseinfo.Factor * (Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+18]) - dStr[j]);
				aStress.dSbl[j]  += caseinfo.Factor * (Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+22]) - dSbl[j]);
				aStress.dSbr[j]	 += caseinfo.Factor * (Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+20]) - dSbr[j]);

				aStress.dStop[j]  += caseinfo.Factor * (Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind1+0] + t_strb.dblStress[iPart][iKind1+5]) - dStop[j]);
				aStress.dSbot[j]  += caseinfo.Factor * (Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind1+0] + t_strb.dblStress[iPart][iKind1+6]) - dSbot[j]);
				//aStress.dSxx[j]  += caseinfo.Factor * (t_strb.dblStress[iPart][iKind1+0] - dSxx[j]);

				TRACE(_T("\n//////////*****Elem:%u ,IJ=%d ,LC=%s,Anlysis Stress Result:******///////\n"),ElemK,j,stldD.LoadCaseName);

				TRACE(_T("dStl_A[%d] = %g\n"),j,Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+16]));
				TRACE(_T("dStr_A[%d] = %g\n"),j,Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+18]));
				TRACE(_T("dSbl_A[%d] = %g\n"),j,Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+22]));
				TRACE(_T("dSbr_A[%d] = %g\n"),j,Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind2+20]));
				TRACE(_T("dStop_A[%d] = %g\n"),j,Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind1+0] + t_strb.dblStress[iPart][iKind1+5]));
				TRACE(_T("dSbot_A[%d] = %g\n"),j,Cng_StressFromCurrToCode(t_strb.dblStress[iPart][iKind1+0] + t_strb.dblStress[iPart][iKind1+6]));

			}
			aStress *= -1.0;
			TRACE(_T("//////////*****End Elem:%u,LC=%s,T Self Stress:******///////\n"),ElemK,stldD.LoadCaseName);

		}
	}

	return TRUE;
}

/************************************************************************/
/* 0~5:Fx-max ~Mz-max. 6~11:Fx-min ~Mz-min     _DGN_FORC_CRC:0-i \ 1-j. 
BOOL bCSTp: TRUE-include CS(tendon primary)  FALSE-remove CS(tendon primary)  */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_ForceFordgn(T_ELEM_K ElemK,UINT iDimType,_DGN_FORC_CRC aForce[12],BOOL bCSTp){
	for (int i=0;i<6;i++) aForce[i].Initialize(TRUE);
	for (int i=6;i<12;i++) aForce[i].Initialize(FALSE);
	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

 	ADGNFORCE aForceLcom;
	BOOL bDim = (iDimType== 0)? FALSE:TRUE;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, bDim, aForceLcom, iLcomNum))	return FALSE;
	for(int i=0; i<aForceLcom.GetSize(); i++)	
	{
        if ((!m_ForceCtrl->Is_LcomForMain(i+1)) && (!m_ForceCtrl->Is_LcomForMainAdd(i+1))) continue;
		_DGN_FORC_CRC ForceCrc = aForceLcom.GetAt(i);
		if (m_ForceCtrl->Is_LcomForMain(i+1)) ForceCrc.strType = _T("Main");
		if (m_ForceCtrl->Is_LcomForMainAdd(i+1)) ForceCrc.strType = _T("MainADD");
		_DGN_FORC_CRC ForceCSTp; ForceCSTp.Initialize();

		if(!bCSTp)
		{
			m_pDesignForceResult->GetForceCSTP_TB05Result(EPairK,i+1,ForceCSTp);//bend and shear of PSC
		}
		for(int j=0; j<2; j++)	// I,J.				
		{
			ForceCrc.iRefNo[j] = i+1;
			if(!bCSTp)
			{
			  ForceCrc.dFxx[j] = ForceCrc.dFxx[j] -ForceCSTp.dFxx[j] ;        
 			  ForceCrc.dFyy[j] = ForceCrc.dFyy[j] -ForceCSTp.dFyy[j] ;
			  ForceCrc.dFzz[j] = ForceCrc.dFzz[j] -ForceCSTp.dFzz[j] ;
			  ForceCrc.dMux[j] = ForceCrc.dMux[j] -ForceCSTp.dMux[j] ;
			  ForceCrc.dMuy[j] = ForceCrc.dMuy[j] -ForceCSTp.dMuy[j] ;
			  ForceCrc.dMuz[j] = ForceCrc.dMuz[j] -ForceCSTp.dMuz[j] ;
			}
			if(ForceCrc.dFxx[j] >= aForce[0].dFxx[j]){ aForce[0] = ForceCrc; aForce[0].strType = ForceCrc.strType;}
			if(ForceCrc.dFyy[j] >= aForce[1].dFyy[j]){ aForce[1] = ForceCrc; aForce[1].strType = ForceCrc.strType;}
			if(ForceCrc.dFzz[j] >= aForce[2].dFzz[j]){ aForce[2] = ForceCrc; aForce[2].strType = ForceCrc.strType;}
			if(ForceCrc.dMux[j] >= aForce[3].dMux[j]){ aForce[3] = ForceCrc; aForce[3].strType = ForceCrc.strType;}
			if(ForceCrc.dMuy[j] >= aForce[4].dMuy[j]){ aForce[4] = ForceCrc; aForce[4].strType = ForceCrc.strType;}
			if(ForceCrc.dMuz[j] >= aForce[5].dMuz[j]){ aForce[5] = ForceCrc; aForce[5].strType = ForceCrc.strType;}

			if(ForceCrc.dFxx[j] <= aForce[6].dFxx[j]){ aForce[6] = ForceCrc; aForce[6].strType = ForceCrc.strType;}
			if(ForceCrc.dFyy[j] <= aForce[7].dFyy[j]){ aForce[7] = ForceCrc; aForce[7].strType = ForceCrc.strType;}
			if(ForceCrc.dFzz[j] <= aForce[8].dFzz[j]){ aForce[8] = ForceCrc; aForce[8].strType = ForceCrc.strType;}
			if(ForceCrc.dMux[j] <= aForce[9].dMux[j]){ aForce[9] = ForceCrc; aForce[9].strType = ForceCrc.strType;}
			if(ForceCrc.dMuy[j] <= aForce[10].dMuy[j]){ aForce[10] = ForceCrc; aForce[10].strType = ForceCrc.strType;}
			if(ForceCrc.dMuz[j] <= aForce[11].dMuz[j]){ aForce[11] = ForceCrc; aForce[11].strType = ForceCrc.strType;}
		}
	}
	//change unit
	for (int i=0;i<12;i++) 
	{
		for (int j=0; j<2; j++) 
		{
            aForce[i].dFxx[j] = fabs(aForce[i].dFxx[j])>pow(10,57)? 0.0 : Cng_ForceFromCurrToCode(aForce[i].dFxx[j]);
			aForce[i].dFyy[j] = fabs(aForce[i].dFyy[j])>pow(10,57)? 0.0 : Cng_ForceFromCurrToCode(aForce[i].dFyy[j]);
			aForce[i].dFzz[j] = fabs(aForce[i].dFzz[j])>pow(10,57)? 0.0 : Cng_ForceFromCurrToCode(aForce[i].dFzz[j]);
			aForce[i].dMux[j] = fabs(aForce[i].dMux[j])>pow(10,57)? 0.0 : Cng_MomentFromCurrToCode(aForce[i].dMux[j]);
			aForce[i].dMuy[j] = fabs(aForce[i].dMuy[j])>pow(10,57)? 0.0 : Cng_MomentFromCurrToCode(aForce[i].dMuy[j]);
			aForce[i].dMuz[j] = fabs(aForce[i].dMuz[j])>pow(10,57)? 0.0 : Cng_MomentFromCurrToCode(aForce[i].dMuz[j]);
			if (iDimType == 0) {aForce[i].dMux[j]=0.0; aForce[i].dMuz[j]=0.0; }
		}
	}
	return TRUE;
}

/************************************************************************/
/* get Np Np*ey Np*ez;      The last stage all tendon's sum value.      */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_PscNpe(T_ELEM_K ElemK,int iPosi,double& dNp,double& dNpey,double& dNpez){
    dNp =.0; dNpey =.0; dNpez =.0;
	int nPart = (iPosi==0 ? 0:7);
	T_STAG_K StagK = m_pDoc->m_pPostCtrl->GetStageInfo()->GetFinalStage();	
	CArray<T_TDNA_K,T_TDNA_K> arTdnaK;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(arTdnaK);
	if (arTdnaK.GetSize()<=0) return FALSE;
	for(int i=0; i<arTdnaK.GetSize(); i++)
	{
		T_TDNA_K TdnaK = arTdnaK[i];
		T_TENDON_ARRANGE data;
		CTendonGrup TendonGrup;
		CArray<T_ELEM_K, T_ELEM_K> raElemK;
		TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
		BOOL bFind = FALSE;
		for (int i=0;i<raElemK.GetSize();i++) {
			if (ElemK == raElemK.GetAt(i)) bFind = TRUE;
		}
		if (!bFind) continue;
        if (!TendonGrup.GetTendonArrange_Tdna(TdnaK, StagK, ElemK, nPart, data)) continue;
        //modify by juhonghua 2012-06-25 _Zhufeng's suggestion
		dNp += Cng_ForceFromCurrToCode(data.dForce*data.dNumTendon);	//change unit
		dNpey += Cng_ForceFromCurrToCode(data.dForce*data.dNumTendon)*Cng_LengthFromCurrToCode(data.dYp,1);
		dNpez += Cng_ForceFromCurrToCode(data.dForce*data.dNumTendon)*Cng_LengthFromCurrToCode(data.dZp,1);
	}
	return TRUE;
}

/************************************************************************/
/* 0~5:Fx-max ~Mz-max. 6~11:Fx-min ~Mz-min     _DGN_FORC_CRC:0-i \ 1-j. */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_ForceStageCS(T_ELEM_K ElemK,UINT iDimType,CArray<_DGN_FORC_CRC,_DGN_FORC_CRC>& aForce){
	aForce.RemoveAll();

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	_DGN_FORC_CRC ForcMax; ForcMax.Initialize();
	_DGN_FORC_CRC ForcMin; ForcMin.Initialize();
	CArray<T_STAG_K,T_STAG_K> aStagK;
	m_pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
	if(aStagK.GetSize()==0)	return FALSE;
	for(int i=0; i<aStagK.GetSize(); i++)
	{
        T_STAG_K StagK = aStagK.GetAt(i);
		int iFinalStepNo=0;
		// Get Last StepNo at Current Stage.
		Get_LastStepInCurStage(StagK, iFinalStepNo);
		CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// SUM.
		T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
		if(!m_ForceCtrl->Get_ForceBySgld(EPairK, SgldK, StagK, iFinalStepNo, ForcMax, ForcMin))	continue;
		//////////////////////////////////////////////////////////////////////////
		// Normal
		T_STRB_D StrMaxData; StrMaxData.Initialize();
		T_STRB_D StrMinData; StrMinData.Initialize();
		T_STRB_D StrAbsData; StrAbsData.Initialize();
		// Get Force Data by Load Case.
		T_LCOM_D LcasD;
		LcasD.Initialize();
		if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, SgldK, LcasD))	{ASSERT(0); return FALSE;}
		if(iFinalStepNo > 0)	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD,iFinalStepNo,2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
		else						m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
		//
		int nPartId = 1;
		m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrMaxData, &StrMinData, &StrAbsData, FALSE);    
		// Get Load Case Name (SUM).
		// Set Force.
		for(int i=0; i<cPOSI_CRC; i++)
		{
			int iPart = (i==0 ? 0 : 3);    			
			int iKind = (i==0 ? 0 : 6);
			ForcMax.iRefNo[i]= StagK;
			ForcMax.dFxx[i] = Cng_ForceFromCurrToCode(StrMaxData.dblForce[iPart][iKind+0]);
			ForcMax.dFzz[i] = Cng_ForceFromCurrToCode(StrMaxData.dblForce[iPart][iKind+1]);
			ForcMax.dFyy[i] = Cng_ForceFromCurrToCode(StrMaxData.dblForce[iPart][iKind+2]);
			ForcMax.dMux[i] = Cng_MomentFromCurrToCode(StrMaxData.dblForce[iPart][iKind+3]);
			ForcMax.dMuy[i] = Cng_MomentFromCurrToCode(StrMaxData.dblForce[iPart][iKind+4]);
			ForcMax.dMuz[i] = Cng_MomentFromCurrToCode(StrMaxData.dblForce[iPart][iKind+5]);
		}
		//////////////////////////////////////////////////////////////////////////
		aForce.Add(ForcMax);
	}
	if (aForce.GetSize()<=0) return FALSE;
	return TRUE;
}

/************************************************************************/
/* 6.4.4-1                                                                     */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_TBConMatl(T_ELEM_K Elemi,double& dalpha, double& dfc,double& dfct){
    T_ELEM_D ElemD;  ElemD.Initialize();                                                   
    if(!m_pDoc->m_pAttrCtrl->GetElem(Elemi,ElemD))	{ASSERT(0); return FALSE;}                  
	T_MATD_D MatdD;	MatdD.Initialize();                                                    
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat,MatdD))	{ASSERT(0); return FALSE;}      
    if (MatdD.Data1.CodeName==MATLCODE_CON_TB05) {
        CString  concrete = MatdD.Data1.CodeMatlName;   //混凝土的标号                                 
                 if(concrete==_T("C20")) {dfc=13.5; dfct=1.70; dalpha=0.70;}                         
        else if(concrete==_T("C25")) {dfc=17.0; dfct=2.00; dalpha=0.70;}                         
        else if(concrete==_T("C30")) {dfc=20.0; dfct=2.20; dalpha=0.70;}                          
        else if(concrete==_T("C35")) {dfc=23.5; dfct=2.50; dalpha=0.70;}                          
        else if(concrete==_T("C40")) {dfc=27.0; dfct=2.70; dalpha=0.70;}                         
        else if(concrete==_T("C45")) {dfc=30.0; dfct=2.90; dalpha=0.70;}                         
        else if(concrete==_T("C50")) {dfc=33.5; dfct=3.10; dalpha=0.75;}                         
        else if(concrete==_T("C55")) {dfc=37.0; dfct=3.30; dalpha=0.75;}                         
        else if(concrete==_T("C60")) {dfc=40.0; dfct=3.50; dalpha=0.75;}                         
    }else {ASSERT(0);	return FALSE;}
	return TRUE;
}

/************************************************************************/
/* get Yp   Zp;      The last stage tendon's Yp Zp.                     */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_YpZp(T_TDNA_K TdnaK,T_ELEM_K ElemK,int iPosi,double& dYp,double& dZp){
    dYp =.0; dZp =.0;
	int nPart = (iPosi==0 ? 0:7);
	T_STAG_K StagK = m_pDoc->m_pPostCtrl->GetStageInfo()->GetFinalStage();	
	T_TENDON_ARRANGE data;
	CTendonGrup TendonGrup;
	CArray<T_ELEM_K, T_ELEM_K> raElemK;
	TendonGrup.GetElemKeyList_Tdna(TdnaK, raElemK);
	BOOL bFind = FALSE;
	for (int i=0;i<raElemK.GetSize();i++) {
		if (ElemK == raElemK.GetAt(i)) bFind = TRUE;
	}
	if (!bFind) return FALSE;

	if (!TendonGrup.GetTendonArrange_Tdna(TdnaK, StagK, ElemK, nPart, data)) return FALSE;
	dYp = Cng_LengthFromCurrToCode(data.dYp,1);
	dZp = Cng_LengthFromCurrToCode(data.dZp,1);
	return TRUE;
}
/************************************************************************/
/* get dn                                                               */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_TBdn(T_ELEM_K ElemK,T_TDNA_K TdnaK,double& dn)
{
    dn=.0; 
    double dEp=.0, dEc=.0;
    // Tendon Profile.
    T_TDNA_D TdnaD;	TdnaD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD))	{ASSERT(0); return FALSE;}
    // Tendon Property.
    T_TDNT_D TdntD;	TdntD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetTdnt(TdnaD.TendonTypeKey,TdntD))	{ASSERT(0); return FALSE;}
    T_MATD_D MatdD;
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(TdntD.MatlKey,MatdD))	{ASSERT(0);return FALSE;}
    if (MatdD.Data1.CodeName== MATLCODE_STL_TB05) {
        CString  tendon   = MatdD.Data1.CodeMatlName;  //预应力的标号 
        if     (tendon==_T("PSB830"))   {dEp=2.0E5;    }         //
        else if(tendon==_T("Wire1470") || tendon==_T("Wire1570") || tendon==_T("Wire1670")
            || tendon==_T("Wire1770") || tendon==_T("Wire1860")){ dEp=2.05E5;   }     
        else if(tendon==_T("Strand1470") || tendon==_T("Strand1570") || tendon==_T("Strand1670")
            || tendon==_T("Strand1720") || tendon==_T("Strand1770") || tendon==_T("Strand1820")
            || tendon==_T("Strand1860")) {dEp=1.95E5; }      //
    }else {ASSERT(0);	return FALSE;}
    
    // Get Matl Data.
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	{ASSERT(0); return FALSE;}
    // Get Matl Data.
    MatdD.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat,MatdD))	{ASSERT(0); return FALSE;}
    CString strConcName = MatdD.Data1.CodeMatlName ;
    if (MatdD.Data1.CodeName==MATLCODE_CON_TB05) {
        if(strConcName==_T("C20")) {dEc=28000.0;}    //
        else if(strConcName==_T("C25")) {dEc=30000.0;}    //
        else if(strConcName==_T("C30")) {dEc=32000.0;}    // 
        else if(strConcName==_T("C35")) {dEc=33000.0;}    // 
        else if(strConcName==_T("C40")) {dEc=34000.0;}    //
        else if(strConcName==_T("C45")) {dEc=34500.0;}    //
        else if(strConcName==_T("C50")) {dEc=35500.0;}    //
        else if(strConcName==_T("C55")) {dEc=36000.0;}    //
        else if(strConcName==_T("C60")) {dEc=36500.0;}    //
        else  {ASSERT(0);	return FALSE;}
    }
    else  {ASSERT(0);	return FALSE;}
    
    dn = dEp/dEc;
    return TRUE;
}
/************************************************************************/
/* 获取钢束1次2次内力                                                   */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_ForceOfTpTs(T_ELEM_K ElemK,int iDimType,_DGN_FORC_CRC& ForceTP,_DGN_FORC_CRC& ForceTS){

	ForceTP.Initialize(TRUE); ForceTS.Initialize(TRUE);
	_DGN_FORC_CRC ForcMin;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	T_STAG_K StagK = m_pDoc->m_pPostCtrl->GetStageInfo()->GetFinalStage();
	int iFinalStepNo=0;
	// Get Last StepNo at Current Stage.
	Get_LastStepInCurStage(StagK, iFinalStepNo);
	CString strSgldName = _LS(IDS_DB_SGLD_TP);	// Tp.
	T_SGLD_K SgldKTp = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
	m_ForceCtrl->Get_ForceBySgld(EPairK, SgldKTp, StagK, iFinalStepNo, ForceTP, ForcMin);

	strSgldName = _LS(IDS_DB_SGLD_TS);	// Ts.
	T_SGLD_K SgldKTs = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
	m_ForceCtrl->Get_ForceBySgld(EPairK, SgldKTs, StagK, iFinalStepNo, ForceTS, ForcMin);
	
	//change unit
	for (int j=0;j<2;j++) {
		ForceTP.dFxx[j] = Cng_ForceFromCurrToCode(ForceTP.dFxx[j]);
		ForceTP.dFyy[j] = Cng_ForceFromCurrToCode(ForceTP.dFyy[j]);
		ForceTP.dFzz[j] = Cng_ForceFromCurrToCode(ForceTP.dFzz[j]);
		ForceTP.dMux[j] = Cng_MomentFromCurrToCode(ForceTP.dMux[j]);
		ForceTP.dMuy[j] = Cng_MomentFromCurrToCode(ForceTP.dMuy[j]);
		ForceTP.dMuz[j] = Cng_MomentFromCurrToCode(ForceTP.dMuz[j]);
		ForceTP.iRefNo[j] = SgldKTp;

		ForceTS.dFxx[j] = Cng_ForceFromCurrToCode(ForceTS.dFxx[j]);
		ForceTS.dFyy[j] = Cng_ForceFromCurrToCode(ForceTS.dFyy[j]);
		ForceTS.dFzz[j] = Cng_ForceFromCurrToCode(ForceTS.dFzz[j]);
		ForceTS.dMux[j] = Cng_MomentFromCurrToCode(ForceTS.dMux[j]);
		ForceTS.dMuy[j] = Cng_MomentFromCurrToCode(ForceTS.dMuy[j]);
		ForceTS.dMuz[j] = Cng_MomentFromCurrToCode(ForceTS.dMuz[j]);
		ForceTS.iRefNo[j] = SgldKTs;

		if (iDimType == 0) {
			ForceTP.dMux[j]=0.0; ForceTP.dMuz[j]=0.0; 
			ForceTS.dMux[j]=0.0; ForceTS.dMuz[j]=0.0; 
		}
	}
	return TRUE;
}

/************************************************************************/
/* 获取cs合计内力                                                       */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_ForceOfCSALL(T_ELEM_K ElemK,int iDimType,_DGN_FORC_CRC& ForceCSALL)
{
    ForceCSALL.Initialize(); 
    _DGN_FORC_CRC ForcMin;

	ElemPairK EPairK(ElemK, EN_EL_BEAM);
    
    T_STAG_K StagK = m_pDoc->m_pPostCtrl->GetStageInfo()->GetFinalStage();
    int iFinalStepNo=0;
    // Get Last StepNo at Current Stage.
    Get_LastStepInCurStage(StagK, iFinalStepNo);
    CString strSgldName = _LS(IDS_DB_SGLD_SUM);	// ALL.
    T_SGLD_K SgldKTp = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
    m_ForceCtrl->Get_ForceBySgld(EPairK, SgldKTp, StagK, iFinalStepNo, ForceCSALL, ForcMin);
    
    //change unit
    for (int j=0;j<2;j++) {
        ForceCSALL.dFxx[j] = Cng_ForceFromCurrToCode(ForceCSALL.dFxx[j]);
        ForceCSALL.dFyy[j] = Cng_ForceFromCurrToCode(ForceCSALL.dFyy[j]);
        ForceCSALL.dFzz[j] = Cng_ForceFromCurrToCode(ForceCSALL.dFzz[j]);
        ForceCSALL.dMux[j] = Cng_MomentFromCurrToCode(ForceCSALL.dMux[j]);
        ForceCSALL.dMuy[j] = Cng_MomentFromCurrToCode(ForceCSALL.dMuy[j]);
        ForceCSALL.dMuz[j] = Cng_MomentFromCurrToCode(ForceCSALL.dMuz[j]);
        ForceCSALL.iRefNo[j] = SgldKTp;
        
        if (iDimType == 0) {
            ForceCSALL.dMux[j]=0.0; ForceCSALL.dMuz[j]=0.0; 
        }
    }
    return TRUE;
}

/************************************************************************/
/* 0~5:Fx-max ~Mz-max. 6~11:Fx-min ~Mz-min     _DGN_FORC_CRC:0-i \ 1-j. */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_MVForceFordgn(T_ELEM_K ElemK,UINT iDimType,_DGN_FORC_CRC aForce[12]){
	for (int i=0;i<6;i++) aForce[i].Initialize(TRUE);
	for (int i=6;i<12;i++) aForce[i].Initialize(FALSE);
	
	ElemPairK EPairK(ElemK, EN_EL_BEAM);
	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
	for(int i=0; i<aForceLcom.GetSize(); i++)	{
        if ((!m_ForceCtrl->Is_LcomForMain(i+1)) && (!m_ForceCtrl->Is_LcomForMainAdd(i+1))) continue;
		_DGN_FORC_CRC MvForce;		
		m_pDesignForceResult->GetForceLL_TB05Result(EPairK,i+1,MvForce);//not run will
		for(int j=0; j<2; j++){	// I,J.				
			MvForce.iRefNo[j] = i+1;
			if(MvForce.dFxx[j] >= aForce[0].dFxx[j]){ aForce[0] = MvForce; }
			if(MvForce.dFyy[j] >= aForce[1].dFyy[j]){ aForce[1] = MvForce; }
			if(MvForce.dFzz[j] >= aForce[2].dFzz[j]){ aForce[2] = MvForce; }
			if(MvForce.dMux[j] >= aForce[3].dMux[j]){ aForce[3] = MvForce; }
			if(MvForce.dMuy[j] >= aForce[4].dMuy[j]){ aForce[4] = MvForce; }
			if(MvForce.dMuz[j] >= aForce[5].dMuz[j]){ aForce[5] = MvForce; }

			if(MvForce.dFxx[j] <= aForce[6].dFxx[j]){ aForce[6] = MvForce; }
			if(MvForce.dFyy[j] <= aForce[7].dFyy[j]){ aForce[7] = MvForce; }
			if(MvForce.dFzz[j] <= aForce[8].dFzz[j]){ aForce[8] = MvForce; }
			if(MvForce.dMux[j] <= aForce[9].dMux[j]){ aForce[9] = MvForce; }
			if(MvForce.dMuy[j] <= aForce[10].dMuy[j]){ aForce[10] = MvForce; }
			if(MvForce.dMuz[j] <= aForce[11].dMuz[j]){ aForce[11] = MvForce; }
		}
	}
	//change unit
	for (int i=0;i<12;i++) {
		for (int j=0; j<2; j++) {
			aForce[i].dFxx[j] = Cng_ForceFromCurrToCode(aForce[i].dFxx[j]);
			aForce[i].dFyy[j] = Cng_ForceFromCurrToCode(aForce[i].dFyy[j]);
			aForce[i].dFzz[j] = Cng_ForceFromCurrToCode(aForce[i].dFzz[j]);
			aForce[i].dMux[j] = Cng_MomentFromCurrToCode(aForce[i].dMux[j]);
			aForce[i].dMuy[j] = Cng_MomentFromCurrToCode(aForce[i].dMuy[j]);
			aForce[i].dMuz[j] = Cng_MomentFromCurrToCode(aForce[i].dMuz[j]);
			if (iDimType == 0) {aForce[i].dMux[j]=0.0; aForce[i].dMuz[j]=0.0; }
		}
	}
	return TRUE;
}

BOOL CRCDataCtrl_CH::Get_fAezData(T_ELEM_K elemK,int iPosi,double dMy,double dN,double dh,double& dfpApep,double& dfp_Ap_ep,double& dfsAses,double& dfs_As_es,double& dfpApZp,double& dfp_Ap_Zp,double& dfsAsZs,double& dfs_As_Zs){
	dfpApep = dfp_Ap_ep = dfsAses = dfs_As_es = dfpApZp = dfp_Ap_Zp = dfsAsZs = dfs_As_Zs	=.0;
	BOOL bPosiI = (iPosi==0 ? TRUE : FALSE);
	//////////////////////////////////////////////////////////////////////////
	//get dfsAsZs dfsAses
	_DGN_RC_MATL_CH matl;	Get_Matl(elemK,matl);
    int CONST_RCSC_iPOS_DIV		=  5;  int CONST_RCSC_iGRUPNUM		=  3;  int CONST_RCSC_iBAR_LAY		=  5;
 	// Create Rbar by CurrUnit.
	m_pDoc->m_pMatlDB->Create_RebarData(m_CurrUnit.nBase_Length, m_CurrUnit.nBase_Force, TRUE);	// Concrete=TRUE.

	ElemPairK EPairK(elemK, EN_EL_BEAM);

	// Before_Release_v825_Check - Rebar 备炼眉 函版登搁辑 荐沥登绢具 窍绰 何盒 
	// 老窜 0 栏肺 绊沥秦滴菌促~	

    T_RCHK_D RchkData;		RchkData.Initialize();
    T_ELEM_D ElemD;  ElemD.Initialize();
	if (m_pDoc->m_pPostCtrl->GetElemPost(elemK,ElemD)){
		int iSect = ElemD.elpro;
		m_pDoc->m_pAttrCtrl->GetRchk(iSect, RchkData);
		double dDc =0.0, dAreatb = 0.0, dArealr =0.0, ddtb =0.0, ddlr = 0.0 ; 
		for(int j=0; j<CONST_RCSC_iBAR_LAY; j++)//柱钢筋层数
		{
			if(RchkData.COLM.dDc[0][j] <= 0.0)	continue;
			dDc= Cng_LengthFromCurrToCode(RchkData.COLM.dDc[0][j],1);
			for(int k=0; k<CONST_RCSC_iGRUPNUM; k++)
			{
				if(RchkData.COLM.iBarNum[0][k][j] > 0)
				{
					if (k ==0) {
						dAreatb += RchkData.COLM.iBarNum[0][k][j]*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[0][k][j]),2); 
						dfsAsZs = dfsAsZs + matl.dfsd*dAreatb*(dh/2.0-dDc);
						if (fabs(dN)>1.0E-07) dfsAses = dfsAses + matl.dfsd*dAreatb*(fabs(dh/2.0-dDc)+fabs(dMy/dN));
					}
					//        if (k ==1) {
					//          dArealr += RchkData.COLM.iBarNum[k][j]*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[k][j]),2); 
					//        }
				}
			}
		}
    }
	dfsAses = dfs_As_es; dfsAsZs = dfs_As_Zs;
	//PSC
	 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;                               //
	 m_DataCtrl.Get_ConTndn(EPairK,bPosiI,TRUE,dMy,dMy, arTndnPosi);     //
	 T_MATD_D MatdD;	MatdD.Initialize();                                           
	 for( int k=0; k<arTndnPosi.GetSize(); k++)                                          //
	 { 
			_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(k);                                   //
            if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(Tndn.MatlK,MatdD))	{ASSERT(0); return FALSE;}
			if(Tndn.dz>0.0) {
				 dfp_Ap_Zp += Tndn.dfpk*Tndn.dArea*Tndn.dz;
				 if (fabs(dN)>1.0E-07)  dfs_As_es += Tndn.dfpk*Tndn.dArea*(fabs(Tndn.dz)+fabs(dMy/dN));
			}   //
		  else            {
				 dfpApZp += Tndn.dfpk*Tndn.dArea*Tndn.dz;
				 if (fabs(dN)>1.0E-07)  dfsAses += Tndn.dfpk*Tndn.dArea*(fabs(Tndn.dz)+fabs(dMy/dN));
			}//
	 }                                                                              //
	return TRUE;
}
BOOL CRCDataCtrl_CH::Get_fAaData(T_ELEM_K elemK,int iPosi,double dCzp, double dCzm, double& dfpApap,double& dfp_Ap_ap,double& dfsAsas,double& dfs_As_as,double& dfpAp,double& dfp_Ap,double& dfsAs,double& dfs_As){
	dfpApap = dfp_Ap_ap = dfsAsas = dfs_As_as = dfpAp = dfp_Ap = dfsAs = dfs_As =.0;
	BOOL bPosiI = (iPosi==0 ? TRUE : FALSE);

	ElemPairK EPairK(elemK, EN_EL_BEAM);
	//////////////////////////////////////////////////////////////////////////
	//get dfsAsZs dfsAses
	_DGN_RC_MATL_CH matl;	Get_Matl(elemK,matl);
    int CONST_RCSC_iPOS_DIV		=  5;  int CONST_RCSC_iGRUPNUM		=  3;  int CONST_RCSC_iBAR_LAY		=  5;
 	// Create Rbar by CurrUnit.
	m_pDoc->m_pMatlDB->Create_RebarData(m_CurrUnit.nBase_Length, m_CurrUnit.nBase_Force, TRUE);	// Concrete=TRUE.
    T_RCHK_D RchkData;		RchkData.Initialize();
    T_ELEM_D ElemD;  ElemD.Initialize();
	if (m_pDoc->m_pPostCtrl->GetElemPost(elemK,ElemD)){
		int iSect = ElemD.elpro;
		m_pDoc->m_pAttrCtrl->GetRchk(iSect, RchkData);
		double dDc =0.0, dAreatb = 0.0, dArealr =0.0, ddtb =0.0, ddlr = 0.0 ; 
		for(int j=0; j<CONST_RCSC_iBAR_LAY; j++)//柱钢筋层数
		{
			// Before_Release_v825_Check - Rebar 备炼眉 函版登搁辑 荐沥登绢具 窍绰 何盒 
    		// 老窜 0 栏肺 绊沥秦滴菌促~	

			if(RchkData.COLM.dDc[0][j] <= 0.0)	continue;
			dDc= Cng_LengthFromCurrToCode(RchkData.COLM.dDc[0][j],1);
			for(int k=0; k<CONST_RCSC_iGRUPNUM; k++)
			{
				if(RchkData.COLM.iBarNum[0][k][j] > 0)
				{
					if (k ==0) {
						dAreatb += RchkData.COLM.iBarNum[0][k][j]*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[0][k][j]),2); 
						dfsAsas = dfsAsas + matl.dfsd*dAreatb*dDc;
						dfsAs   = dfsAs   + matl.dfsd*dAreatb;
					}
					//        if (k ==1) {
					//          dArealr += RchkData.COLM.iBarNum[k][j]*Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[k][j]),2); 
					//        }
				}
			}
		}
    }
	dfs_As_as = dfsAsas; dfs_As = dfsAs;
	//PSC
	 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;                               //
	 m_DataCtrl.Get_ConTndn(EPairK,bPosiI,TRUE,0,0, arTndnPosi);     //
	 T_MATD_D MatdD;	MatdD.Initialize();                                           
	 for( int k=0; k<arTndnPosi.GetSize(); k++)                                          //
	 { 
			_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(k);                                   //
            if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(Tndn.MatlK,MatdD))	{ASSERT(0); return FALSE;}
			if(Tndn.dz>0.0) {
				 dfp_Ap_ap += Tndn.dfpk*Tndn.dArea*(dCzp-Tndn.dz);
				 dfp_Ap    += Tndn.dfpk*Tndn.dArea;
			}   //
		  else            {
				 dfpApap += Tndn.dfpk*Tndn.dArea*(dCzm-Tndn.dz);
				 dfpAp   += Tndn.dfpk*Tndn.dArea;
			}//
	 }          
	 return TRUE;
}


BOOL CRCDataCtrl_CH::Get_con_fracture(int ElemK, int iSafeType, T_BEAM_CON_INCLINE_FRACTURE_CH& con_incline_fracture_elem)
{
		con_incline_fracture_elem.Initialize();
//	//get force
//	_DGN_FORC_CRC aForce[12];
//	if (!Get_ForceFordgn(ElemK,iDimType,aForce)) return FALSE;

	_DGN_SHRS_CRC ShrsMax,ShrsMin;
	ShrsMax.Initialize(); ShrsMin.Initialize();
	//new method to get shear. by maxiao 2010-11-16
// 	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
// 	ADGNFORCE aForceLcom;
// 	if(!m_ForceCtrl->Get_LcomDataForDesign(ElemK, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
// 	if(m_ForceCtrl->Get_LcomNumForStrn()==0)	return FALSE;
	T_LCOM_D LcomData;	
	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
    //Modify by Juhonghua 20151126
    int queryType = 1;  // Beam Stress(PSC)

    //2.Tendon Psc
		T_STSS_D TP_stss; TP_stss.Initialize();
		CArray<T_STAG_K,T_STAG_K> aStagK;
		m_pDoc->m_pPostCtrl->GetStageInfo()->GetResultSavedStagKeyList(aStagK);
		if(aStagK.GetSize()>0)
		{
			CString strSgldName = _LS(IDS_DB_SGLD_TP);	// TP.
			T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);
			// Get Force Data by Load Case.
			T_LCOM_D LcasD;  LcasD.Initialize();
			if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, SgldK, LcasD))	{ASSERT(0); return FALSE;}
			int iFinalStepNo=0;
			Get_LastStepInCurStage(aStagK[aStagK.GetCount()-1], iFinalStepNo);

			m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD, iFinalStepNo,2);

// 			if(iFinalStepNo > 0)	
// 				m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD,iFinalStepNo,2);	// 0=Nonlinear, 1=Pushover, 2=Consruction.
// 			else						
// 				m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);

			//T_STSS_D t_stss;
			m_pDoc->m_pPostCtrl->GetStssNew(ElemK, 0, 0, &TP_stss, queryType);//// Beam Stress(PSC)
		}

	for(int i=0; i<arLcomKey.GetSize(); i++)	
	{ //1.
		T_STSS_D t_stss; t_stss.Initialize();
// 		if(!m_ForceCtrl->Is_LcomForStrn(i+1))	continue;	// Only 辆惫矫.
		
		LcomData.Initialize(); 
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey[i], LcomData);
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
		//m_pDoc->m_pPostCtrl->SelectLoadCombNew(aForceLcom.GetAt(i), 0, 2);
        m_pDoc->m_pPostCtrl->GetStssNew(ElemK, 0, 0, &t_stss, queryType);//荷载组合产生的主应力
		
		//0,2-SigmaTp;  1,3-SigmaCp
		double Kf = 1.2;
                if(iSafeType == 1)
                        Kf = 1.0;
		for (int j=0; j<4; j++)//ij
		{
			double SigmaTp[10],SigmaCp[10];
			double AbsTp=.0, AbsCp=.0;
			for (int k=0; k<10; k++)//10个位置点
			{
				double SigmaCx = t_stss.dblStress[j/2][k][0] + t_stss.dblStress[j/2][k][3] //Sig-xx轴向+Sig-xx竖筋
					+ Kf*(t_stss.dblStress[j/2][k][1]+t_stss.dblStress[j/2][k][2]);//Sig-xx弯矩y+Sig-xx弯矩z
				double SigmaCy = t_stss.dblStress[j/2][k][5];//Sig-zz
				double tauC = Kf*t_stss.dblStress[j/2][k][6]//Sig-xz(剪切)
					            -(Kf+1)*TP_stss.dblStress[j/2][k][6]//Sig-xz(剪切)-Tendon Sig-xz(剪切)
				              +t_stss.dblStress[j/2][k][7]//Sig-xz(扭转)
											+t_stss.dblStress[j/2][k][8];//Sig-xz(竖筋)
				SigmaCx = -SigmaCx;//分析结果是拉+压-，设计时转化为拉-压+
				SigmaCy = -SigmaCy;
				tauC = -tauC;
				
				SigmaTp[k]= (SigmaCx+SigmaCy)/2 - sqrt((SigmaCx-SigmaCy)*(SigmaCx-SigmaCy)/4+tauC*tauC);
				SigmaCp[k]= (SigmaCx+SigmaCy)/2 + sqrt((SigmaCx-SigmaCy)*(SigmaCx-SigmaCy)/4+tauC*tauC);
				if (SigmaTp[k]<AbsTp)	{ AbsTp = SigmaTp[k];}
				if (SigmaCp[k]>AbsCp)	{ AbsCp = SigmaCp[k];}
			}
			if ((j%2==0 && AbsTp < con_incline_fracture_elem.dataBase[j].dSig_MAX))
			{
				con_incline_fracture_elem.dataBase[j].itype	= 1;
				con_incline_fracture_elem.dataBase[j].LcomK	= m_ForceCtrl->Get_SerialLcomKey(arLcomKey[i], 1);
				con_incline_fracture_elem.dataBase[j].dSig_P1 = Cng_StressFromCurrToCode(SigmaTp[0]);
				con_incline_fracture_elem.dataBase[j].dSig_P2 = Cng_StressFromCurrToCode(SigmaTp[1]);
				con_incline_fracture_elem.dataBase[j].dSig_P3 = Cng_StressFromCurrToCode(SigmaTp[2]);
				con_incline_fracture_elem.dataBase[j].dSig_P4 = Cng_StressFromCurrToCode(SigmaTp[3]);
				con_incline_fracture_elem.dataBase[j].dSig_P5 = Cng_StressFromCurrToCode(SigmaTp[4]);
				con_incline_fracture_elem.dataBase[j].dSig_P6 = Cng_StressFromCurrToCode(SigmaTp[5]);
				con_incline_fracture_elem.dataBase[j].dSig_P7 = Cng_StressFromCurrToCode(SigmaTp[6]);
				con_incline_fracture_elem.dataBase[j].dSig_P8 = Cng_StressFromCurrToCode(SigmaTp[7]);
				con_incline_fracture_elem.dataBase[j].dSig_P9 = Cng_StressFromCurrToCode(SigmaTp[8]);
				con_incline_fracture_elem.dataBase[j].dSig_P10= Cng_StressFromCurrToCode(SigmaTp[9]);
				con_incline_fracture_elem.dataBase[j].dSig_MAX= Cng_StressFromCurrToCode(AbsTp);
			}
			else if(j%2==1 && AbsCp > con_incline_fracture_elem.dataBase[j].dSig_MAX)
			{
				con_incline_fracture_elem.dataBase[j].itype	= 2;
				con_incline_fracture_elem.dataBase[j].LcomK	= m_ForceCtrl->Get_SerialLcomKey(arLcomKey[i], 2);
				con_incline_fracture_elem.dataBase[j].dSig_P1 = Cng_StressFromCurrToCode(SigmaCp[0]);
				con_incline_fracture_elem.dataBase[j].dSig_P2 = Cng_StressFromCurrToCode(SigmaCp[1]);
				con_incline_fracture_elem.dataBase[j].dSig_P3 = Cng_StressFromCurrToCode(SigmaCp[2]);
				con_incline_fracture_elem.dataBase[j].dSig_P4 = Cng_StressFromCurrToCode(SigmaCp[3]);
				con_incline_fracture_elem.dataBase[j].dSig_P5 = Cng_StressFromCurrToCode(SigmaCp[4]);
				con_incline_fracture_elem.dataBase[j].dSig_P6 = Cng_StressFromCurrToCode(SigmaCp[5]);
				con_incline_fracture_elem.dataBase[j].dSig_P7 = Cng_StressFromCurrToCode(SigmaCp[6]);
				con_incline_fracture_elem.dataBase[j].dSig_P8 = Cng_StressFromCurrToCode(SigmaCp[7]);
				con_incline_fracture_elem.dataBase[j].dSig_P9 = Cng_StressFromCurrToCode(SigmaCp[8]);
				con_incline_fracture_elem.dataBase[j].dSig_P10= Cng_StressFromCurrToCode(SigmaCp[9]);
				con_incline_fracture_elem.dataBase[j].dSig_MAX= Cng_StressFromCurrToCode(AbsCp);
			}
		}	
	}
	/*
	con_incline_fracture_elem.Initialize();
//	//get force
//	_DGN_FORC_CRC aForce[12];
//	if (!Get_ForceFordgn(ElemK,iDimType,aForce)) return FALSE;

	_DGN_SHRS_CRC ShrsMax,ShrsMin;
	ShrsMax.Initialize(); ShrsMin.Initialize();
	if (!m_ForceCtrl->Get_ShrsByLcom(ElemK, ShrsMax,ShrsMin, 3, FALSE)) return FALSE;
	//TB: -ShraMax is min; -ShrsMin is max.

	for(int i=0; i<4; i++)
	{
		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		_DGN_SHRS_CRC ShrsTB= (i%2==0 ? ShrsMin : ShrsMax);
		if(ShrsTB.dSspAbs[i/2]==0.0)	continue;

		int iDgnLcomNo = ShrsTB.iRefNo[i/2];
		CString strOrgLcomNa=_T("");
		int iMaxMinType=0;
		if(!m_ForceCtrl->Get_LcomType(iDgnLcomNo, strOrgLcomNa, iMaxMinType))	ASSERT(0);
		con_incline_fracture_elem.dataBase[i].LcomK	= iDgnLcomNo;
		//FpmsJtg.FpmsBase[i].iKind	= m_ForceCtrl.Get_LcomKind(iDgnLcomNo);
		con_incline_fracture_elem.dataBase[i].itype	= iMaxMinType;
		con_incline_fracture_elem.dataBase[i].dSig_P1 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][0]);
		con_incline_fracture_elem.dataBase[i].dSig_P2 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][1]);
		con_incline_fracture_elem.dataBase[i].dSig_P3 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][2]);
		con_incline_fracture_elem.dataBase[i].dSig_P4 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][3]);
		con_incline_fracture_elem.dataBase[i].dSig_P5 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][4]);
		con_incline_fracture_elem.dataBase[i].dSig_P6 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][5]);
		con_incline_fracture_elem.dataBase[i].dSig_P7 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][6]);
		con_incline_fracture_elem.dataBase[i].dSig_P8 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][7]);	
        con_incline_fracture_elem.dataBase[i].dSig_P9 	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][8]);	
        con_incline_fracture_elem.dataBase[i].dSig_P10	= Cng_StressFromCurrToCode(ShrsTB.dSsp[i/2][9]);	
        con_incline_fracture_elem.dataBase[i].dSig_MAX	= Cng_StressFromCurrToCode(ShrsTB.dSspAbs[i/2]);	//Max,Min
	}
	*/
	return TRUE;
}

BOOL CRCDataCtrl_CH::Get_rebarData(T_ELEM_K elemK,int iPosi,double dh,double& dAs, double& dAs1,double& das,double& das1,double& dU,double& dat,double& dab){
    dAs=0.0, dAs1=0.0, das=0.0, das1=0.0; dU=0.0; dat=0.0; dab=0.0;

	// Get Elem.
    T_ELEM_D ElemD;  ElemD.Initialize();
    if(!m_pDoc->m_pPostCtrl->GetElemPost(elemK,ElemD)){
        GSaveHistoryFormatNF(_T("[错误] 设计单元 %d 缺少单元数据."),elemK);
        return FALSE;
    }
    //get sect
    BOOL bPosiI = (iPosi==0 ? TRUE : FALSE);

	//Rbar
    CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;	arRbarPosi.RemoveAll();
 	// Create Rbar by CurrUnit.
	m_pDoc->m_pMatlDB->Create_RebarData(m_CurrUnit.nBase_Length, m_CurrUnit.nBase_Force, TRUE);	// Concrete=TRUE.
    if(!m_pDoc->m_pMatlDB->Exist_RebarData())	return FALSE;
	// Get Rbar.
	_DGN_RBAR_CRC RbarCrc;
    T_RPSC_D RpscD;	RpscD.Initialize();
	T_SECT_K OrgSectK = m_pDoc->m_pPostCtrl->GetOrgSectKeyPost(ElemD.elpro);
//	if(!m_pDoc->m_pAttrCtrl->GetRpscByConsiderRbar(TRUE,OrgSectK,RpscD))	return FALSE;
//	if(!m_pDoc->m_pAttrCtrl->GetRpsc(OrgSectK,RpscD))	return FALSE;//by maxiao 2006.12.22
	if(!m_pDoc->m_pAttrCtrl->GetRpsd(OrgSectK,RpscD))	return FALSE;//by maxiao 2007.02.15 Rpsd:设计钢筋
	int iPosiM = (RpscD.bMbarJ && !bPosiI ? 1 : 0);
	int iMbarSize = RpscD.Mbar[iPosiM].GetSize();
	for(int i=0; i<iMbarSize; i++)
	{
		T_RPSC_MBAR RpscMbar = RpscD.Mbar[iPosiM].GetAt(i);
		int iStaYRef		= RpscMbar.iStaYRef;	// 0=档缴俊辑 谅快措莫, 1=Left俊辑 Right肺.
		int iStaZRef		= RpscMbar.iStaZRef;	// 0=Top, 1=Bottom.
		int iBarNum			= RpscMbar.iBarNum;
		CString strBarNa= RpscMbar.strBarNa;
		double dStaY		= RpscMbar.dStaY;
		double dStaZ		= RpscMbar.dStaZ;
		double dPitch		= RpscMbar.dPitch;
        double dtemp =0.0;
		for(int j=0; j<iBarNum; j++)
		{
			// Set Rbar. Change Unit (CurrUnit -> CodeUnit).
			RbarCrc.strRbarNa = strBarNa;
			RbarCrc.dArea	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarArea(strBarNa), 2);
			RbarCrc.dDia	= Cng_LengthFromCurrToCode(m_pDoc->m_pMatlDB->Get_RebarDia(strBarNa) , 1);
            dtemp = (iStaZRef==0 ?dStaZ : -dStaZ);
            RbarCrc.dy		= Cng_LengthFromCurrToCode(dStaY,1);
            RbarCrc.dz		= Cng_LengthFromCurrToCode(dtemp,1);
			arRbarPosi.Add(RbarCrc);
		}
	}
	if(arRbarPosi.GetSize() <= 0)	return FALSE;

    int inumb = 0;
    double dSumBarD=0.0, dSumBarD2=0.0, dSumBarD1=0.0, dSumBarD21=0.0;
    for(int i=0; i<arRbarPosi.GetSize(); i++)
    {
        _DGN_RBAR_CRC Rbar = arRbarPosi.GetAt(i);
        if((Rbar.dz>0.0 && Rbar.dz <dh/2.0) || (Rbar.dz<0 && fabs(Rbar.dz)>dh/2.0 )){
            dAs1 += Rbar.dArea;        
            dSumBarD1 += Rbar.dDia;
            dSumBarD21 += pow(Rbar.dDia,2);
            double dtemp=.0;
			if (Rbar.dz >0.0 ) dtemp = Rbar.dz;
            else dtemp = dh - fabs(Rbar.dz);
			das1 += dtemp;
            inumb ++;      
			if (i==0) dat = dtemp; else dat = min(dat,dtemp);
        }else if((Rbar.dz<0.0 && fabs(Rbar.dz)< dh/2.0) ||  Rbar.dz >dh/2.0 ){
            dAs += Rbar.dArea;
            dSumBarD += Rbar.dDia;
            dSumBarD2 += pow(Rbar.dDia,2);
			double dtemp = .0;
			if (Rbar.dz <0.0 ) 	dtemp=fabs(Rbar.dz);
			else   dtemp= dh -Rbar.dz;
			das += dtemp;
			if (i==0) dab = dtemp; else dab = min(dab,dtemp);
        }
		dU+=2*sqrt(Rbar.dArea*PI);
    }  
    if (inumb == 0) das1 = 0.0;
    else  das1 = das1/(1.0*inumb);
    if (inumb == arRbarPosi.GetSize())  das = 0.0;
    else  das = das/(1.0*(arRbarPosi.GetSize()-inumb));
	return TRUE;
}

/************************************************************************/
/* //JTG D62-04 Table 5.3.1        itype: 0(db=b);1(db=i);2(db=2r)      */
/************************************************************************/
double CRCDataCtrl_CH::Get_Phi(INT itype, double dl0, double db)
{
	double dPhi = 1.0;
	if (itype == 2){
		 if (db<1.0E-07) return dPhi;
		      if(dl0/db<=7   )  dPhi = 1.0;
		 else if(dl0/db<=8.5 )  dPhi = 0.98;
		 else if(dl0/db<=10.5)  dPhi = 0.95;
		 else if(dl0/db<=12  )  dPhi = 0.92;
		 else if(dl0/db<=14  )  dPhi = 0.87;
		 else if(dl0/db<=15.5)  dPhi = 0.81;
		 else if(dl0/db<=17  )  dPhi = 0.75;
		 else if(dl0/db<=19  )  dPhi = 0.70;
		 else if(dl0/db<=21  )  dPhi = 0.65;
		 else if(dl0/db<=22.5)  dPhi = 0.60;
		 else if(dl0/db<=24  )  dPhi = 0.56;
		 else if(dl0/db<=26  )  dPhi = 0.52;
		 else if(dl0/db<=28  )  dPhi = 0.48;
		 else if(dl0/db<=29.5)  dPhi = 0.44;
		 else if(dl0/db<=31  )  dPhi = 0.40;
		 else if(dl0/db<=33  )  dPhi = 0.36;
		 else if(dl0/db<=34.5)  dPhi = 0.32;
		 else if(dl0/db<=36.5)  dPhi = 0.29;
		 else if(dl0/db<=38  )  dPhi = 0.26;
		 else if(dl0/db<=40  )  dPhi = 0.23;
		 else if(dl0/db<=41.5)  dPhi = 0.21;
		 else dPhi = 0.19;
	 } else if (itype == 1){
		 if (db<1.0E-07) return dPhi;
		      if(dl0/db<=28 ) dPhi = 1.0;
		 else if(dl0/db<=35 ) dPhi = 0.98;
		 else if(dl0/db<=42 ) dPhi = 0.95;
		 else if(dl0/db<=48 ) dPhi = 0.92;
		 else if(dl0/db<=55 ) dPhi = 0.87;
		 else if(dl0/db<=62 ) dPhi = 0.81;
		 else if(dl0/db<=69 ) dPhi = 0.75;
		 else if(dl0/db<=76 ) dPhi = 0.70;
		 else if(dl0/db<=83 ) dPhi = 0.65;
		 else if(dl0/db<=90 ) dPhi = 0.60;
		 else if(dl0/db<=97 ) dPhi = 0.56;
		 else if(dl0/db<=104) dPhi = 0.52;
		 else if(dl0/db<=111) dPhi = 0.48;
		 else if(dl0/db<=118) dPhi = 0.44;
		 else if(dl0/db<=125) dPhi = 0.40;
		 else if(dl0/db<=132) dPhi = 0.36;
		 else if(dl0/db<=139) dPhi = 0.32;
		 else if(dl0/db<=146) dPhi = 0.29;
		 else if(dl0/db<=153) dPhi = 0.26;
		 else if(dl0/db<=160) dPhi = 0.23;
		 else if(dl0/db<=167) dPhi = 0.21;
		 else dPhi = 0.19;
	 } else if (itype ==0) {
		 if (db<1.0E-07) return dPhi;
		      if(dl0/db<=8)  dPhi = 1.0;
		 else if(dl0/db<=10)  dPhi = 0.98;
		 else if(dl0/db<=12)  dPhi = 0.95;
		 else if(dl0/db<=14)  dPhi = 0.92;
		 else if(dl0/db<=16)  dPhi = 0.87;
		 else if(dl0/db<=18)  dPhi = 0.81;
		 else if(dl0/db<=20)  dPhi = 0.75;
		 else if(dl0/db<=22)  dPhi = 0.70;
		 else if(dl0/db<=24)  dPhi = 0.65;
		 else if(dl0/db<=26)  dPhi = 0.60;
		 else if(dl0/db<=28)  dPhi = 0.56;
		 else if(dl0/db<=30)  dPhi = 0.52;
		 else if(dl0/db<=32)  dPhi = 0.48;
		 else if(dl0/db<=34)  dPhi = 0.44;
		 else if(dl0/db<=36)  dPhi = 0.40;
		 else if(dl0/db<=38)  dPhi = 0.36;
		 else if(dl0/db<=40)  dPhi = 0.32;
		 else if(dl0/db<=42)  dPhi = 0.29;
		 else if(dl0/db<=44)  dPhi = 0.26;
		 else if(dl0/db<=46)  dPhi = 0.23;
		 else if(dl0/db<=48)  dPhi = 0.21;
		 else dPhi = 0.19;
	 }
	 return dPhi;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//seiemic design
//---------------------------------------------------------------------------------------------------------
/************************************************************************/
/* 拉(+)压(-),故isCscs为True时,取Fx-min,否则取Fx-max                    */
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_SeismicComp(BOOL bE1,int Elemi,BOOL isCscs,T_CSTCS_CH& cstcs)//E1作用下拉力及压力
{
    cstcs.Initialize();
    _DGN_FORC_CRC aForce[12];
    if (Get_SeismicForce(Elemi, aForce))
    {
        for (int i=0; i<10; i++)
        {
            cstcs.cstcsBase[i].bCHK = TRUE;
            cstcs.cstcsBase[i].dx = 0.0;
	  int isEedI = i/5;//[i/5]: _DGN_FORC_CRC:0-i \ 1-j. add this line for replace [i/5] by gongxing at 2015-12-18
            if (i%5 == 0) {//0=轴心
                int iKey = 1;
                if (!isCscs) iKey = 0;//modify this line by gongxing at 2015-12-18
                cstcs.cstcsBase[i].iMax = iKey+1; //1:Fx-Max; 2:Fx-Min; 		
                cstcs.cstcsBase[i].dNd = aForce[iKey].dFxx[isEedI];
                cstcs.cstcsBase[i].LcomK = aForce[iKey].iRefNo[isEedI];
            }else if (i%5 == 1)//Fx-My
            {
                int iKey = 1;
                if (!isCscs) iKey = 0;//modify this line by gongxing at 2015-12-18
                cstcs.cstcsBase[i].iMax = iKey+1; //1:Fx-Max; 2:Fx-Min;
                cstcs.cstcsBase[i].dNd = aForce[iKey].dFxx[isEedI];//[i/5]: _DGN_FORC_CRC:0-i \ 1-j. 
                cstcs.cstcsBase[i].LcomK = aForce[iKey].iRefNo[isEedI];
                cstcs.cstcsBase[i].dMd = aForce[iKey].dMuy[isEedI];
            } else if (i%5 == 2)//Fx-Mz
            {
                int iKey = 1;
                if (!isCscs) iKey = 0;//modify this line by gongxing at 2015-12-18
                cstcs.cstcsBase[i].iMax = iKey+1; //1:Fx-Max; 2:Fx-Min;
                cstcs.cstcsBase[i].dNd = aForce[iKey].dFxx[isEedI];//[i/5]: _DGN_FORC_CRC:0-i \ 1-j. 
                cstcs.cstcsBase[i].LcomK = aForce[iKey].iRefNo[isEedI];
                cstcs.cstcsBase[i].dMd = aForce[iKey].dMuz[isEedI];
            }
            if (i%5==3) {//9:My-max
                int iKey = 8;
                cstcs.cstcsBase[i].iMax = iKey + 1; 
                cstcs.cstcsBase[i].dNd = aForce[iKey].dFxx[isEedI];//[i/5]: _DGN_FORC_CRC:0-i \ 1-j. 
                cstcs.cstcsBase[i].LcomK = aForce[iKey].iRefNo[isEedI];
                cstcs.cstcsBase[i].dMd = aForce[iKey].dMuy[isEedI];
            }
            if (i%5==4) {//10:My-min      
                int iKey = 9;
                cstcs.cstcsBase[i].iMax = iKey + 1; 
                cstcs.cstcsBase[i].dNd = aForce[iKey].dFxx[isEedI];//[i/5]: _DGN_FORC_CRC:0-i \ 1-j. 
                cstcs.cstcsBase[i].LcomK = aForce[iKey].iRefNo[isEedI];
                cstcs.cstcsBase[i].dMd = aForce[iKey].dMuy[isEedI];
            }
            cstcs.cstcsBase[i].dNd = -cstcs.cstcsBase[i].dNd;
            cstcs.cstcsBase[i].drNd = cstcs.cstcsBase[i].dNd;
        }
    }	
    return TRUE;
}

//E1作用下，取抗震荷载组合的弯矩；-----------不适用于盖梁**
BOOL CRCDataCtrl_CH::Get_SeismicCbcs(BOOL bE1,int ElemK,T_CBCS_CH& cbcs)//弯矩
{
    cbcs.Initialize();
    _DGN_FORC_CRC aForce[12];
    //取抗震荷载组合的弯矩；
    if(!Get_SeismicForce(ElemK, aForce)) return FALSE;
    for (int i=0; i<4; i++)
    {
        cbcs.cbcsBase[i].bCHK = TRUE;
        int iKey = (i%2 == 0 ? 9:10);//9:My-Max;10:My-Min
        cbcs.cbcsBase[i].iMax   = iKey;
        cbcs.cbcsBase[i].LcomK  = aForce[iKey-1].iRefNo[i/2];
        cbcs.cbcsBase[i].dMuy = aForce[iKey-1].dMuy[i/2];
        cbcs.cbcsBase[i].drMuy = cbcs.cbcsBase[i].dMuy;
    }
    return TRUE;
}

//E1作用下，取抗震荷载组合的剪力；-----------不适用于盖梁**
BOOL CRCDataCtrl_CH::Get_SeismicShear(BOOL bE1,int ElemK,T_CSIS_CH& csis)//剪力	
{
    csis.Initialize();
    _DGN_FORC_CRC aForce[12];
    //取抗震荷载组合的剪力；
    if(!Get_SeismicForce(ElemK, aForce)) return FALSE;
    for (int i=0; i<4; i++)
    {
        int iKey = (i%2 == 0 ? 5:6);//5:Fz-Max;6:Fz-Min
        csis.CsisBase[i].bCHK = TRUE;
        csis.CsisBase[i].iMax = iKey;
        csis.CsisBase[i].dVu = aForce[iKey-1].dFzz[i/2];
        csis.CsisBase[i].drVu = csis.CsisBase[i].dVu;
        csis.CsisBase[i].dMu = aForce[iKey-1].dMuy[i/2];
        csis.CsisBase[i].LcomK = aForce[iKey-1].iRefNo[i/2];
    }
    return TRUE;
}


//弹性作用下，取抗震荷载组合的弯矩；弹塑性作用下，取永久作用效应的弯矩，然后与Mp0组合
BOOL CRCDataCtrl_CH::Get_SeismicCapBeamCbcs(BOOL bElastic,int ElemK,T_CBCS_CH& cbcs)
{
    cbcs.Initialize();
    _DGN_FORC_CRC aForce[12];
    if(bElastic)//弹性作用下，取抗震荷载组合的弯矩；
    {
        if(!Get_SeismicForce(ElemK, aForce)) return FALSE;
    }
    else//弹塑性作用下，取永久作用效应的弯矩，然后在CDgnJTG_D62_04_Seismic与Mp0组合
    {
        if(!Get_PermanentSeismicForce(ElemK, aForce)) return FALSE;
    }
    for (int i=0; i<4; i++)
    {
        cbcs.cbcsBase[i].bCHK = TRUE;
        int iKey = (i%2 == 0 ? 9:10);//9:My-Max;10:My-Min
        cbcs.cbcsBase[i].iMax   = iKey;
        cbcs.cbcsBase[i].LcomK  = aForce[iKey-1].iRefNo[i/2];
        cbcs.cbcsBase[i].dMuy = aForce[iKey-1].dMuy[i/2];
    }
    return TRUE;
}

//弹性作用下，取抗震荷载组合的弯矩；弹塑性作用下，取永久作用效应的弯矩，然后与Vc0组合
BOOL CRCDataCtrl_CH::Get_SeismicCapBeamShear(BOOL bElastic,int ElemK,T_CSIS_CH& csis)
{
    csis.Initialize();
    _DGN_FORC_CRC aForce[12];
    if(bElastic)//弹性作用下，取抗震荷载组合的弯矩；
    {
        if(!Get_SeismicForce(ElemK, aForce)) return FALSE;
    }
    else//弹塑性作用下，取永久作用效应的弯矩，然后在CDgnJTG_D62_04_Seismic与Vc0组合
    {
        if(!Get_PermanentSeismicForce(ElemK, aForce)) return FALSE;
    }
    for (int i=0; i<4; i++)
    {
        int iKey = (i%2 == 0 ? 5:6);//5:Fz-Max;6:Fz-Min
        csis.CsisBase[i].bCHK = TRUE;
        csis.CsisBase[i].iMax = iKey;
        csis.CsisBase[i].dVu = aForce[iKey-1].dFzz[i/2];
        csis.CsisBase[i].dMu = aForce[iKey-1].dMuy[i/2];
        csis.CsisBase[i].LcomK = aForce[iKey-1].iRefNo[i/2];
    }
    return TRUE;
}

//change name by maxiao 2009.11.11.    E1 is same to E2
//BOOL CRCDataCtrl_CH::Get_SeismicE1FounD(int ElemK,T_FOUND_CH& funddata)//E1作用下基础受力	
BOOL CRCDataCtrl_CH::Get_SeismicFounD(int ElemK, T_FOUND_CH& funddata)
{
	funddata.Initialize();	
    funddata.FoundBase[0].dPu1 = funddata.FoundBase[0].dPu2 = (-1)/pow(cDgn_Zero,40);
    funddata.FoundBase[1].dPu1 = funddata.FoundBase[1].dPu2 = (-1)/pow(cDgn_Zero,40);
    ///////////////////////////////////////////////////////////////////////////////
    BOOL bEqLcom = FALSE;
    T_LCOM_D LcomData;  int i, j;
    CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
    for(i=0; i<arLcomKey.GetSize(); i++)
    {
        LcomData.Initialize(); 
        m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey[i], LcomData);
        if(LcomData.LoadCombType != 0 && LcomData.LoadCombType != 3) continue;  // 0=Add, 1=Envelope, 2=ABS, 3=SRSS.
        if(LcomData.nActive!=1)	continue;
        BOOL bEQ = FALSE;
        // Execute Nomalize for Lcase Lists.
        T_LCOM_D_UL	LcomUlData;
        LcomUlData.Initialize();
        if(LcomData.LoadCombType == 3)
        {
            if(m_ForceCtrl->IsEqLoad_JTG_B02(D_LCOM_CB_CONCRETE, LcomData.LoadCombId)) { bEQ = TRUE; }
        }
        else
        {
            if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData,LcomUlData))	continue;//{ASSERT(0); return FALSE;}
            for(j=0; j<LcomUlData.Combination.GetSize(); j++)
            {
                T_LCOM_BASE  LcomBase = LcomUlData.Combination.GetAt(j);
                unsigned int iLcaseKey = LcomBase.LoadCaseKey;									
                if(m_ForceCtrl->IsEqLoad_JTG_B02(LcomBase.AnalType, iLcaseKey)) { bEQ = TRUE;  break; }
            }
        }    
        if(bEQ)
        {
            T_STRB_D strb;
            m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
            if(m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &strb, 0, 0, FALSE))
            {
                //double dAxial = (-1.0)*strb.dblStress[3][7];
                double dEccentricI = (-1.0)*(min(min(strb.dblStress[0][16], strb.dblStress[0][18]), min(strb.dblStress[0][20], strb.dblStress[0][22])));
                double dEccentricJ = (-1.0)*(min(min(strb.dblStress[3][17], strb.dblStress[3][19]), min(strb.dblStress[3][21], strb.dblStress[3][23])));
                if(funddata.FoundBase[0].dPu2 < dEccentricI)
                {
                    funddata.FoundBase[0].LcomK = LcomData.LoadCombId;
                    funddata.FoundBase[0].dPu1 = (-1.0)*strb.dblStress[0][0];
                    funddata.FoundBase[0].dPu2 = dEccentricI;
                }
                if(funddata.FoundBase[1].dPu2 < dEccentricJ)
                {
                    funddata.FoundBase[1].LcomK = LcomData.LoadCombId;
                    funddata.FoundBase[1].dPu1 = (-1.0)*strb.dblStress[3][7];
                    funddata.FoundBase[1].dPu2 = dEccentricJ;
                }
                bEqLcom = TRUE;
            }
        }
    }
    ///////////////////////////////////////////////////////////////////////////////
/*	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
	if(!m_ForceCtrl->Get_LcomDataForDesign(ElemK, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;

	funddata.FoundBase[0].dPu1 = funddata.FoundBase[0].dPu1 = (-1)/pow(cDgn_Zero,40);
	funddata.FoundBase[1].dPu1 = funddata.FoundBase[1].dPu1 = (-1)/pow(cDgn_Zero,40);

//	int i, j;
	double dAxial, dEccentric;
	BOOL bEqLcom = FALSE;
	for(i=0; i<aForceLcom.GetSize(); i++)	
	{
        if (!m_ForceCtrl->IsEqLcom_JTG_B02(i+1)) continue;
		bEqLcom = TRUE;
		_DGN_STRS_CRC EqStress;		
		m_pDesignForceResult->GetStressLcomResult(ElemK,i+1,EqStress);

		for(j=0 ; j<2 ; j++)
		{
			dAxial = (-1.0)*EqStress.dSxx[j];
			dEccentric = (-1.0)*(min(min(EqStress.dStl[j], EqStress.dSbl[j]), min(EqStress.dStr[j], EqStress.dSbr[j])));
			if(funddata.FoundBase[j].dPu2 < dEccentric)
			{
				funddata.FoundBase[j].LcomK = EqStress.iRefNo[j];
				funddata.FoundBase[j].dPu1 = dAxial;
				funddata.FoundBase[j].dPu2 = dEccentric;
			}
		}
	}
    */
	if(!bEqLcom)
	{
		funddata.Initialize();
	}
	else 
	{			//change unit
		for (int i=0; i<2; i++) 
		{
			funddata.FoundBase[i].dPu1 = Cng_StressFromCurrToCode(funddata.FoundBase[i].dPu1);
			funddata.FoundBase[i].dPu2 = Cng_StressFromCurrToCode(funddata.FoundBase[i].dPu2);
		}
	}
	return TRUE;
}
/************************************************************************/
//  FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11
//  FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12
//  _DGN_FORC_CRC:0-i \ 1-j. 
/************************************************************************/
BOOL CRCDataCtrl_CH::Get_SeismicForce(T_ELEM_K ElemK, BOOL bE1, _DGN_FORC_CRC aForce[12])
{
	for (int i=0;i<12;i++) aForce[i].Initialize(i%2==0);
	
	BOOL aIsChk[12];
    for(int i=0 ; i<12 ; i++) aIsChk[i] = FALSE;
	
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	BOOL bEqLcom = FALSE;
	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
	if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;

	for(int i=0; i<aForceLcom.GetSize(); i++)	
	{
        if (!m_ForceCtrl->IsEqLcom_JTG_B02(i+1)) continue;
		bEqLcom = TRUE;
		_DGN_FORC_CRC EqForce;		
		if(bE1) m_pDesignForceResult->GetForceLcomResult(EPairK,i+1,EqForce);
		else    m_pDesignForceResult->GetForceEQE2_JTGResult(EPairK,i+1,EqForce);

        GetEqForceExTempForce(ElemK,i+1,EqForce);
		for(int j=0; j<2; j++){	// I,J.				
			EqForce.iRefNo[j] = i+1;
			if(EqForce.dFxx[j] >= aForce[ 0].dFxx[j]){ aForce[ 0] = EqForce;  aIsChk[ 0] = TRUE; }
			if(EqForce.dFxx[j] <= aForce[ 1].dFxx[j]){ aForce[ 1] = EqForce;  aIsChk[ 1] = TRUE; }
			if(EqForce.dFyy[j] >= aForce[ 2].dFyy[j]){ aForce[ 2] = EqForce;  aIsChk[ 2] = TRUE; }
			if(EqForce.dFyy[j] <= aForce[ 3].dFyy[j]){ aForce[ 3] = EqForce;  aIsChk[ 3] = TRUE; }
			if(EqForce.dFzz[j] >= aForce[ 4].dFzz[j]){ aForce[ 4] = EqForce;  aIsChk[ 4] = TRUE; }
			if(EqForce.dFzz[j] <= aForce[ 5].dFzz[j]){ aForce[ 5] = EqForce;  aIsChk[ 5] = TRUE; }
			if(EqForce.dMux[j] >= aForce[ 6].dMux[j]){ aForce[ 6] = EqForce;  aIsChk[ 6] = TRUE; }
			if(EqForce.dMux[j] <= aForce[ 7].dMux[j]){ aForce[ 7] = EqForce;  aIsChk[ 7] = TRUE; }
			if(EqForce.dMuy[j] >= aForce[ 8].dMuy[j]){ aForce[ 8] = EqForce;  aIsChk[ 8] = TRUE; }
			if(EqForce.dMuy[j] <= aForce[ 9].dMuy[j]){ aForce[ 9] = EqForce;  aIsChk[ 9] = TRUE; }
			if(EqForce.dMuz[j] >= aForce[10].dMuz[j]){ aForce[10] = EqForce;  aIsChk[10] = TRUE; }
			if(EqForce.dMuz[j] <= aForce[11].dMuz[j]){ aForce[11] = EqForce;  aIsChk[11] = TRUE; }
		}
	}
	
	for (int i=0;i<12;i++) 
	{ if(!aIsChk[i]) aForce[i].Initialize(); }
	//change unit
	for (int i=0;i<12;i++) 
	{
		for (int j=0; j<2; j++) 
		{
			aForce[i].dFxx[j] = Cng_ForceFromCurrToCode(aForce[i].dFxx[j]);
			aForce[i].dFyy[j] = Cng_ForceFromCurrToCode(aForce[i].dFyy[j]);
			aForce[i].dFzz[j] = Cng_ForceFromCurrToCode(aForce[i].dFzz[j]);
			aForce[i].dMux[j] = Cng_MomentFromCurrToCode(aForce[i].dMux[j]);
			aForce[i].dMuy[j] = Cng_MomentFromCurrToCode(aForce[i].dMuy[j]);
			aForce[i].dMuz[j] = Cng_MomentFromCurrToCode(aForce[i].dMuz[j]);
		}
	}	
	return TRUE;
}
BOOL CRCDataCtrl_CH::Get_SeisFoundBaseCodeToCurr(T_FOUND_CH_BASE& rData)
{
    rData.dPn1 = Cng_StressFromCodeToCurr(rData.dPn1);
    rData.dPn2 = Cng_StressFromCodeToCurr(rData.dPn2);
    rData.dPu1 = Cng_StressFromCodeToCurr(rData.dPu1);
    rData.dPu2 = Cng_StressFromCodeToCurr(rData.dPu2);
	return TRUE;
}
BOOL CRCDataCtrl_CH::Get_SeisSupportDispCodeToCurr(T_SUPPORTDISP_CH& rData)
{
    rData.dHoriDispX = Cng_LengthFromCodeToCurr(rData.dHoriDispX, 1);
    rData.dHoriDispY = Cng_LengthFromCodeToCurr(rData.dHoriDispY, 1);
    rData.dThick     = Cng_LengthFromCodeToCurr(rData.dThick    , 1);
	return TRUE;
}
BOOL CRCDataCtrl_CH::Get_SeisSupportForceCodeToCurr(T_SUPPORTFORCE_CH& rData)
{
    rData.dHoriForceX = Cng_ForceFromCodeToCurr(rData.dHoriForceX);
    rData.dHoriForceY = Cng_ForceFromCodeToCurr(rData.dHoriForceY);
    rData.dAllowForce = Cng_ForceFromCodeToCurr(rData.dAllowForce);
	return TRUE;
}
BOOL CRCDataCtrl_CH::Get_SeismicFrustaTopBaseCodeToCurr(T_FRUSTATOPDISP_CH_BASE& rData)
{
    rData.dHoriDisp = Cng_LengthFromCodeToCurr(rData.dHoriDisp, 1);
    rData.dAlwDisp  = Cng_LengthFromCodeToCurr(rData.dAlwDisp , 1);
	return TRUE;
}

BOOL CRCDataCtrl_CH::Get_ElemListForSeismicDgn(CArray<UINT,UINT>& ElemKList)
{
    //ElemK
	CArray<T_RSDM_K, T_RSDM_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRsdmKeyList(1, KeyList);
	int nSize = KeyList.GetSize();
	for(int i=0 ; i<nSize ; i++)
    {
		ElemKList.Add(KeyList[i].key.OKey);
    }
    //Member
    CArray<T_RSDL_K, T_RSDL_K> MKeyList;
    m_pDoc->m_pAttrCtrl->GetRsdlKeyList(MKeyList);
    int nMSize = MKeyList.GetSize();
    for(int i=0 ; i<nMSize ; i++)
    {
        T_RSDL_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdl(MKeyList[i], dataType)) return FALSE;
        ElemKList.Append(dataType.aElemList);
    }  
	return (ElemKList.GetSize() > 0);  
}
BOOL CRCDataCtrl_CH::Get_GeneralLinkListForSeismicDgn(CArray<UINT,UINT>& GeneralLinkList)
{
	CArray<T_RSDM_K, T_RSDM_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRsdmKeyList(3, KeyList);
	int nSize = KeyList.GetSize();
	GeneralLinkList.SetSize(nSize);
	for(int i=0 ; i<nSize ; i++)
		GeneralLinkList.SetAt(i, KeyList[i].key.OKey);
	return (GeneralLinkList.GetSize() > 0);
}
BOOL CRCDataCtrl_CH::Get_ElasticLinkListForSeismicDgn(CArray<UINT,UINT>& ElasticLinkList)
{
	CArray<T_RSDM_K, T_RSDM_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRsdmKeyList(2, KeyList);
	int nSize = KeyList.GetSize();
	ElasticLinkList.SetSize(nSize);
	for(int i=0 ; i<nSize ; i++)
		ElasticLinkList.SetAt(i, KeyList[i].key.OKey);
	return (ElasticLinkList.GetSize() > 0);
}
	//取得弹性连接位移 x-顺桥向  y-横桥向	
BOOL CRCDataCtrl_CH::GetElasticLinkDisp(T_ELNK_K ElnkK,BOOL bE1,CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP>& aLinkDispList)
{	
	T_ELNK_D ElnkD; ElnkD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElnk(ElnkK, ElnkD)) return FALSE;
	
	T_SEIS_LINK_DISP data;
	T_LCOM_D LcomData, LcasD;
	int i, j;
	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
	for(i=0; i<arLcomKey.GetSize(); i++)
	{
		LcomData.Initialize(); 
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey[i], LcomData);
        
        if(LcomData.LoadCombType != 0 && LcomData.LoadCombType != 3) continue;  // 0=Add, 1=Envelope, 2=ABS, 3=SRSS.

		if(LcomData.nActive!=1)	continue;
        BOOL bEQ = FALSE;			
		// Execute Nomalize for Lcase Lists.
		T_LCOM_D_UL	LcomUlData;
		LcomUlData.Initialize();
        if(LcomData.LoadCombType == 3)
        {
        	LcomUlData = LcomData;
            if(m_ForceCtrl->IsEqLoad_JTG_B02(D_LCOM_CB_CONCRETE, LcomData.LoadCombId)) { bEQ = TRUE; }
        }
        else
        {
            if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData,LcomUlData))	continue;//{ASSERT(0); return FALSE;}
            for(j=0; j<LcomUlData.Combination.GetSize(); j++)
            {
                T_LCOM_BASE  LcomBase = LcomUlData.Combination.GetAt(j);
                unsigned int iLcaseKey = LcomBase.LoadCaseKey;									
                if(m_ForceCtrl->IsEqLoad_JTG_B02(LcomBase.AnalType, iLcaseKey)) { bEQ = TRUE;  break; }
            }
        }		
		if(bEQ)
		{
			double dTotalDy, dTotalDz, dDLDy, dDLDz; 

			T_DISP_D DispIMax, DispIMin, DispJMax, DispJMin;
			T_DISP_D DispIMax_DL, DispIMin_DL, DispJMax_DL, DispJMin_DL;
//			m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
//			m_pDoc->m_pPostCtrl->GetDispNew(ElnkD.Node1, &DispIMax, &DispIMin, NULL);
//			m_pDoc->m_pPostCtrl->GetDispNew(ElnkD.Node2, &DispJMax, &DispJMin, NULL);

//			if(!GetLocalDisplacementOnGlobalPlane(ElnkK, DispIMax, DispIMin, DispJMax, DispJMin, dTotalDy, dTotalDz, 2)) return FALSE;
					
			DispIMax_DL.Initialize();  DispIMin_DL.Initialize();  DispJMax_DL.Initialize();  DispJMin_DL.Initialize(); 
			for(j=0; j<LcomUlData.Combination.GetSize(); j++)
			{
				T_LCOM_BASE  LcomBase = LcomUlData.Combination.GetAt(j);
				unsigned int iLcaseKey = LcomBase.LoadCaseKey;	
				int nLoadCaseType;				
				if(!CDBLib::GetLoadCaseTypeByLoadCombType(LcomBase.AnalType, nLoadCaseType)) return FALSE;
				
                if(m_ForceCtrl->IsPermanentLoad_JTG_B02(LcomBase.AnalType, iLcaseKey))
				{
					m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType,iLcaseKey,LcasD);
					m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
					m_pDoc->m_pPostCtrl->GetDispNew(ElnkD.Node1, &DispIMax, &DispIMin, NULL);
					m_pDoc->m_pPostCtrl->GetDispNew(ElnkD.Node2, &DispJMax, &DispJMin, NULL);
					DispIMax.Mult(LcomBase.Factor);  DispIMin.Mult(LcomBase.Factor);
					DispJMax.Mult(LcomBase.Factor);  DispJMin.Mult(LcomBase.Factor);						
					DispIMax_DL.Add(DispIMax);  DispIMin_DL.Add(DispIMin);  
					DispJMax_DL.Add(DispJMax);  DispJMin_DL.Add(DispJMin);  
				}
                else
                {
                    m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType,iLcaseKey,LcasD);
                    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
                    m_pDoc->m_pPostCtrl->GetDispNew(ElnkD.Node1, &DispIMax, &DispIMin, NULL);
                    m_pDoc->m_pPostCtrl->GetDispNew(ElnkD.Node2, &DispJMax, &DispJMin, NULL);          
                    if(!GetLocalDisplacementOnGlobalPlane(ElnkK, DispIMax, DispIMin, DispJMax, DispJMin, dTotalDy, dTotalDz, 2)) return FALSE;			
                }				
			}
			if(!GetLocalDisplacementOnGlobalPlane(ElnkK, DispIMax_DL, DispIMin_DL, DispJMax_DL, DispJMin_DL, dDLDy, dDLDz, 2)) return FALSE;
			
			data.Initialize();
			data.LcomK = arLcomKey[i];
			data.LcomName = LcomData.LoadCombName;
            data.dXDx = Cng_LengthFromCurrToCode(dTotalDz,1);
            data.dXDy = Cng_LengthFromCurrToCode(dTotalDy,1);
            data.dXHx = Cng_LengthFromCurrToCode(dDLDz,1);
            data.dXHy = Cng_LengthFromCurrToCode(dDLDy,1);
			aLinkDispList.Add(data);
		}
	}
	return TRUE;
}
	//取得弹性链接水平内力
BOOL CRCDataCtrl_CH::GetElasticLinkForce(T_ELNK_K ElnkK,BOOL bE1,CArray<T_SEIS_LINK_FORCE,T_SEIS_LINK_FORCE>& aLinkForceList)
{
	T_ELNK_D ElnkD; ElnkD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElnk(ElnkK, ElnkD)) return FALSE;
	
	T_SEIS_LINK_FORCE data;
	T_LCOM_D LcomData, LcasD;
	int i, j;
	CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
	for(i=0; i<arLcomKey.GetSize(); i++)
	{
		LcomData.Initialize(); 
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey[i], LcomData);
        
        if(LcomData.LoadCombType != 0 && LcomData.LoadCombType != 3) continue;  // 0=Add, 1=Envelope, 2=ABS, 3=SRSS.

		if(LcomData.nActive!=1)	continue;
		// Execute Nomalize for Lcase Lists.
		T_LCOM_D_UL	LcomUlData;
		LcomUlData.Initialize();
        BOOL bEQ = FALSE;
        if(LcomData.LoadCombType == 3)
        {
        	LcomUlData = LcomData;
            if(m_ForceCtrl->IsEqLoad_JTG_B02(D_LCOM_CB_CONCRETE, LcomData.LoadCombId)) { bEQ = TRUE; }
        }
        else
        {
            if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData,LcomUlData))	continue;//{ASSERT(0); return FALSE;}
            for(j=0; j<LcomUlData.Combination.GetSize(); j++)
            {
                T_LCOM_BASE  LcomBase = LcomUlData.Combination.GetAt(j);
                unsigned int iLcaseKey = LcomBase.LoadCaseKey;									
                if(m_ForceCtrl->IsEqLoad_JTG_B02(LcomBase.AnalType, iLcaseKey)) { bEQ = TRUE;  break; }
            }
        }		
		if(bEQ)
		{
			T_STEK_D StrTotalMax, StrTotalMin, StrDLMax, StrDLMin, StrWLMax, StrWLMin;

			T_STEK_D StrMaxData; StrMaxData.Initialize();// dblForce  : local force , 2 node       , component : fx, fy, fz, mx, my, mz
			T_STEK_D StrMinData; StrMinData.Initialize();
//			m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
//			m_pDoc->m_pPostCtrl->GetStekNew(ElnkK, &StrMaxData, &StrMinData, NULL);  // elastic link force/moment
//			StrTotalMax = StrMaxData;   StrTotalMin = StrMinData;

			StrDLMax.Initialize();  StrDLMin.Initialize(); 
			StrWLMax.Initialize();  StrWLMin.Initialize();
			for(j=0; j<LcomUlData.Combination.GetSize(); j++)
			{
				T_LCOM_BASE  LcomBase = LcomUlData.Combination.GetAt(j);
				unsigned int iLcaseKey = LcomBase.LoadCaseKey;	
				int nLoadCaseType;				
				if(!CDBLib::GetLoadCaseTypeByLoadCombType(LcomBase.AnalType, nLoadCaseType)) return FALSE;
				if(m_ForceCtrl->IsPermanentLoad_JTG_B02(LcomBase.AnalType, iLcaseKey))
				{
					m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType,iLcaseKey,LcasD);
					m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
					m_pDoc->m_pPostCtrl->GetStekNew(ElnkK, &StrMaxData, &StrMinData, NULL);
					StrMaxData.Mult(LcomBase.Factor);  StrMinData.Mult(LcomBase.Factor);			
					StrDLMax.Add(StrMaxData);          StrDLMin.Add(StrMinData);  
				}
                else
                {
                    m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType,iLcaseKey,LcasD);
                    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
                    m_pDoc->m_pPostCtrl->GetStekNew(ElnkK, &StrMaxData, &StrMinData, NULL);  // elastic link force/moment
                    StrTotalMax = StrMaxData;   StrTotalMin = StrMinData;
                }
                if(m_ForceCtrl->IsSelfWeightLoad_JTG_B02(LcomBase.AnalType, iLcaseKey))
                {
                    m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType,iLcaseKey,LcasD);
                    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
                    m_pDoc->m_pPostCtrl->GetStekNew(ElnkK, &StrMaxData, &StrMinData, NULL);
                    StrMaxData.Mult(LcomBase.Factor);  StrMinData.Mult(LcomBase.Factor);			
                    StrWLMax.Add(StrMaxData);          StrWLMin.Add(StrMinData);  
                }        
			}

			data.Initialize();
			data.LcomK = arLcomKey[i];
			data.LcomName = LcomData.LoadCombName;
            data.dRb    = max(fabs(StrWLMax.dblForce[0][0] + StrWLMax.dblForce[1][0]), fabs(StrWLMin.dblForce[0][0] + StrWLMin.dblForce[1][0]))/2.0;
			data.dEhzdx = max(fabs(StrDLMax.dblForce[0][2] + StrDLMax.dblForce[1][2]), fabs(StrDLMin.dblForce[0][2] + StrDLMin.dblForce[1][2]))/2.0;
            data.dEhzdy = max(fabs(StrDLMax.dblForce[0][1] + StrDLMax.dblForce[1][1]), fabs(StrDLMin.dblForce[0][1] + StrDLMin.dblForce[1][1]))/2.0;
			data.dEhzex = max(fabs(StrTotalMax.dblForce[0][2] + StrTotalMax.dblForce[1][2]), fabs(StrTotalMin.dblForce[0][2] + StrTotalMin.dblForce[1][2]))/2.0;
			data.dEhzey = max(fabs(StrTotalMax.dblForce[0][1] + StrTotalMax.dblForce[1][1]), fabs(StrTotalMin.dblForce[0][1] + StrTotalMin.dblForce[1][1]))/2.0;
            data.dRb    = Cng_ForceFromCurrToCode(data.dRb   );
            data.dEhzdx = Cng_ForceFromCurrToCode(data.dEhzdx);
            data.dEhzdy = Cng_ForceFromCurrToCode(data.dEhzdy);
            data.dEhzex = Cng_ForceFromCurrToCode(data.dEhzex);
            data.dEhzey = Cng_ForceFromCurrToCode(data.dEhzey);
			aLinkForceList.Add(data);
		}
	}
	return TRUE;
}
	//取得一般连接位移 x-顺桥向  y-横桥向	-时称作用
BOOL CRCDataCtrl_CH::GetGeneralLinkDisp(T_NLNK_K NlnkK,BOOL bE1,CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP>& aLinkDispList)
{	
	T_NLNK_D NlnkD; NlnkD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetNlnk(NlnkK, NlnkD)) return FALSE;
	
    T_SEIS_LINK_DISP data;  data.Initialize();
	T_LCOM_D LcomData, LcasD;
	int i, j;	
    CArray<T_LCOM_K, T_LCOM_K> arLcomKey;
    double dTotalDy=.0, dTotalDz=.0; 
    //时称位移
    BOOL bHistory = FALSE;  
    CArray<T_THIS_K, T_THIS_K> rKeyList;  //serial 锅龋肺 沥纺等 府胶飘  
    m_pDoc->m_pAttrCtrl->GetThisKeyList(rKeyList);
    for(i=0; i<rKeyList.GetSize(); i++) 
    {
        //m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType,iLcaseKey,LcasD);
        m_pDoc->m_pPostCtrl->SelectThis(rKeyList.GetAt(i));
        
        T_DISP_D dataMin0, dataMin1, dataMax0, dataMax1;    
        //Min
        m_pDoc->m_pPostCtrl->GetNldfHistoryMaxMinAll(NlnkK, 0, &dataMin0, 0, 0, &dataMin1, 0);
        //Max
        m_pDoc->m_pPostCtrl->GetNldfHistoryMaxMinAll(NlnkK, &dataMax0, 0, 0, &dataMax1, 0, 0);
        
        dTotalDy = max(max(fabs(dataMin0.dblDisp[1]),fabs(dataMax0.dblDisp[1])),dTotalDy);
        dTotalDz = max(max(fabs(dataMin0.dblDisp[2]),fabs(dataMax0.dblDisp[2])),dTotalDz);
        bHistory = TRUE; break;
    } 
    if(!bHistory) return FALSE;
 
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, arLcomKey);
	for(i=0; i<arLcomKey.GetSize(); i++)
	{
		LcomData.Initialize(); 
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, arLcomKey[i], LcomData);
        
        if(LcomData.LoadCombType != 0) continue;  // 0=Add, 1=Envelope, 2=ABS, 3=SRSS.

		if(LcomData.nActive!=1)	continue;
		// Execute Nomalize for Lcase Lists.
		T_LCOM_D_UL	LcomUlData;
		LcomUlData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->NormalizeLcom(LcomData,LcomUlData))	continue;//{ASSERT(0); return FALSE;}
	
        double dDLDy, dDLDz; 
        T_DISP_D DispIMax, DispIMin, DispJMax, DispJMin;
        T_DISP_D DispIMax_DL, DispIMin_DL, DispJMax_DL, DispJMin_DL;    
        //-永久作用
        DispIMax_DL.Initialize();  DispIMin_DL.Initialize();  DispJMax_DL.Initialize();  DispJMin_DL.Initialize(); 
        for(j=0; j<LcomUlData.Combination.GetSize(); j++)
        {
            T_LCOM_BASE  LcomBase = LcomUlData.Combination.GetAt(j);
            unsigned int iLcaseKey = LcomBase.LoadCaseKey;	
            int nLoadCaseType;				
            if(!CDBLib::GetLoadCaseTypeByLoadCombType(LcomBase.AnalType, nLoadCaseType)) continue;	
            
            if(m_ForceCtrl->IsPermanentLoad_JTG_B02(LcomBase.AnalType, iLcaseKey))
            {
                m_pDoc->m_pPostCtrl->MakeLoadComb(nLoadCaseType,iLcaseKey,LcasD);
                m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
                m_pDoc->m_pPostCtrl->GetDispNew(NlnkD.Node1, &DispIMax, &DispIMin, NULL);
                m_pDoc->m_pPostCtrl->GetDispNew(NlnkD.Node2, &DispJMax, &DispJMin, NULL);
                DispIMax.Mult(LcomBase.Factor);  DispIMin.Mult(LcomBase.Factor);
                DispJMax.Mult(LcomBase.Factor);  DispJMin.Mult(LcomBase.Factor);						
                DispIMax_DL.Add(DispIMax);  DispIMin_DL.Add(DispIMin);  
                DispJMax_DL.Add(DispJMax);  DispJMin_DL.Add(DispJMin);  
            }
        }
//    if(!GetLocalDisplacementOnGlobalPlane(NlnkK, DispIMax_DL, DispIMin_DL, DispJMax_DL, DispJMin_DL, dDLDy, dDLDz, 2)) continue;
        dDLDz = max(fabs(DispJMax_DL.dblDisp[0]-DispIMax_DL.dblDisp[0]),fabs(DispJMin_DL.dblDisp[0]-DispIMin_DL.dblDisp[0]));
        dDLDy = max(fabs(DispJMax_DL.dblDisp[1]-DispIMax_DL.dblDisp[1]),fabs(DispJMin_DL.dblDisp[1]-DispIMin_DL.dblDisp[1]));
        data.dXHx = max(data.dXHx,dDLDz);
        data.dXHy = max(data.dXHy,dDLDy); 
    }  
    data.LcomK = 0;
    data.LcomName = _T("时称作用");
    data.dXDx = Cng_LengthFromCurrToCode(dTotalDz,1);
    data.dXDy = Cng_LengthFromCurrToCode(dTotalDy,1);
    data.dXHx = Cng_LengthFromCurrToCode(data.dXHx,1);
    data.dXHy = Cng_LengthFromCurrToCode(data.dXHy,1);
    aLinkDispList.Add(data);  
	return TRUE;
}

//取得箍筋信息 iElk-单元id
BOOL CRCDataCtrl_CH::Get_SeismicStirrup(T_ELEM_K iElk, double& dAx, double& dAy, double& dDx, double& dDy)
{	
	dAx = dAy = dDx = dDy = 0.0;
	// Get Elem.
	T_ELEM_D ElemD;  ElemD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetElemPost(iElk,ElemD)) return FALSE;

	T_RCHK_D RchkD;
	if(!m_pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro, RchkD)) return FALSE;

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurrUnit.nBase_Length, CurrUnit.nBase_Force, TRUE);	
	
	T_RC_D_CH rcd;  rcd.Initialize();
	Get_Rcd(rcd);

	// Before_Release_v825_Check - Rebar 备炼眉 函版登搁辑 荐沥登绢具 窍绰 何盒 
	// 老窜 0 栏肺 绊沥秦滴菌促~	

	if(rcd.iDgnCode == RC_JTG_B02_01_2008||rcd.iDgnCode == RC_CJJ_166_2011)
	{
		dAx = m_pDoc->m_pMatlDB->Get_RebarArea(RchkD.COLM.strSubBarNa_z[0]) * RchkD.COLM.dSubBarNum_z[0];
		dAy = m_pDoc->m_pMatlDB->Get_RebarArea(RchkD.COLM.strSubBarNa_y[0]) * RchkD.COLM.dSubBarNum_y[0];
	}
	else 
	{
		dAx = m_pDoc->m_pMatlDB->Get_RebarArea(RchkD.COLM.strSubBarNa[0]) * RchkD.COLM.dSubBarNum_z[0];
		dAy = m_pDoc->m_pMatlDB->Get_RebarArea(RchkD.COLM.strSubBarNa[0]) * RchkD.COLM.dSubBarNum_y[0];
	}
	
	dDx = dDy = RchkD.COLM.dSubBarDist[0];

	return TRUE;
}

BOOL CRCDataCtrl_CH::Get_SeismicD(T_JTG_SEISMIC_D& dgnD)
{	
    T_CHRP_D rData; rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetChrp(rData);

	dgnD.iLoadType  = rData.iEarthquakeType;
	dgnD.iCheckType = rData.iBridgeType;
	for (int i=0; i<12; i++)
	{
		dgnD.bDesignTpye[i] = rData.bDesignPrint[i];
	}	
	return TRUE;
}
BOOL CRCDataCtrl_CH::GetElemSeisChkType(T_ELEM_K ElemK, T_RSDM_D& SeisType)
{
	T_RSDM_K RsdmK;
	RsdmK.key.OType = 1;  RsdmK.key.OKey = ElemK;
	BOOL bGet = m_pDoc->m_pAttrCtrl->GetRsdm(RsdmK, SeisType);
    SeisType.dSupportAllowHorizonDis = Cng_LengthFromCurrToCode(SeisType.dSupportAllowHorizonDis,1);
    SeisType.dSupportRubberThick = Cng_LengthFromCurrToCode(SeisType.dSupportRubberThick,1);
    SeisType.dCappingBeamSpan = Cng_LengthFromCurrToCode(SeisType.dCappingBeamSpan,1);
    SeisType.dFdnAllowGround = Cng_StressFromCurrToCode(SeisType.dFdnAllowGround);
    SeisType.dSupportMaxHorizonLoad = Cng_ForceFromCurrToCode(SeisType.dSupportMaxHorizonLoad);
    return bGet;
}
BOOL CRCDataCtrl_CH::GetGeneralLinkSeisChkType(T_NLNK_K NlnkK, T_RSDM_D& SeisType)
{
	T_RSDM_K RsdmK;
	RsdmK.key.OType = 3;  RsdmK.key.OKey = NlnkK;
	BOOL bGet = m_pDoc->m_pAttrCtrl->GetRsdm(RsdmK, SeisType);
    SeisType.dSupportAllowHorizonDis = Cng_LengthFromCurrToCode(SeisType.dSupportAllowHorizonDis,1);
    SeisType.dSupportRubberThick = Cng_LengthFromCurrToCode(SeisType.dSupportRubberThick,1);
    SeisType.dCappingBeamSpan = Cng_LengthFromCurrToCode(SeisType.dCappingBeamSpan,1);
    SeisType.dFdnAllowGround = Cng_StressFromCurrToCode(SeisType.dFdnAllowGround);
    SeisType.dSupportMaxHorizonLoad = Cng_ForceFromCurrToCode(SeisType.dSupportMaxHorizonLoad);
    return bGet;
}
BOOL CRCDataCtrl_CH::GetElasticLinkSeisChkType(T_ELNK_K ElnkK, T_RSDM_D& SeisType)
{
	T_RSDM_K RsdmK;
	RsdmK.key.OType = 2;  RsdmK.key.OKey = ElnkK;
	BOOL bGet = m_pDoc->m_pAttrCtrl->GetRsdm(RsdmK, SeisType);
    SeisType.dSupportAllowHorizonDis = Cng_LengthFromCurrToCode(SeisType.dSupportAllowHorizonDis,1);
    SeisType.dSupportRubberThick = Cng_LengthFromCurrToCode(SeisType.dSupportRubberThick,1);
    SeisType.dCappingBeamSpan = Cng_LengthFromCurrToCode(SeisType.dCappingBeamSpan,1);
    SeisType.dFdnAllowGround = Cng_StressFromCurrToCode(SeisType.dFdnAllowGround);
    SeisType.dSupportMaxHorizonLoad = Cng_ForceFromCurrToCode(SeisType.dSupportMaxHorizonLoad);
    return bGet;
}


//取得截面位置处最大MG
double CRCDataCtrl_CH::Get_MGofElem (int iElemK, int ij){
    double dMg = 0.0;
    _DGN_FORC_CRC aForce[12];
    Get_WeightSeismicForce(iElemK, aForce);
    dMg = aForce[9].dMuy[ij]; // MY-MAX=9  //  _DGN_FORC_CRC:0-i \ 1-j. 
    return dMg;
}
//Get parameters data from pier;
BOOL CRCDataCtrl_CH::GetParaFromPier(int iMember,BOOL& bOnlyBot, double& dHn, BOOL& bDoublePier, double & dDisp, int& iOutput)
{
    T_RSDL_D PierType;
    if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, PierType)) return FALSE;
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    if(rcd.iDgnCode == RC_JTG_B02_01_2008)
    {
        if(PierType.nMemberType!=0) return FALSE;
    }
    else if(rcd.iDgnCode == RC_CJJ_166_2011)
    {
        if(PierType.nMemberType!=10 && PierType.nMemberType!=12) return FALSE;
    }
    else
        ASSERT(0);
    bOnlyBot =  (PierType.nPierPHingeRegionType ==0) ? TRUE : FALSE;//0-bottom; 1-Top&bottom
    dHn = Cng_LengthFromCurrToCode(PierType.dPierHn,1);
    bDoublePier = PierType.bPierDColumnBent;
    dDisp = Cng_LengthFromCurrToCode(PierType.dPierAllowDisp,1);
    iOutput = PierType.nOutputType;
    return TRUE;
}
//Get parameters data from cap Beam;  
BOOL CRCDataCtrl_CH::GetParaFromCapBeam(int iMember,double& dL0, int& iOutput)
{
    T_RSDL_D CapBeamType;
    if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, CapBeamType)) return FALSE;
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    if(rcd.iDgnCode == RC_JTG_B02_01_2008)
    {
        if(CapBeamType.nMemberType!=3) return FALSE;
    }
    else if(rcd.iDgnCode == RC_CJJ_166_2011)
    {
        if(CapBeamType.nMemberType!=13) return FALSE;
    }
    else
        ASSERT(0);
    dL0 = Cng_LengthFromCurrToCode(CapBeamType.dCappingBeamSpan,1);
    iOutput = CapBeamType.nOutputType;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL CRCDataCtrl_CH::GetLocalDisplacementOnGlobalPlane(UINT elemK, T_DISP_D& DispIMax, T_DISP_D& DispIMin, T_DISP_D& DispJMax, T_DISP_D& DispJMin, double& dDy, double& dDz, int nDataType)
{
	double dLocal_yOnGP[2] = {0.0, 0.0};
	double dLocal_zOnGP[2] = {0.0, 0.0};
	
	if(!GetLocalDirectionOnGlobalPlane(elemK, 2, dLocal_yOnGP, nDataType)) return FALSE;
	if(!GetLocalDirectionOnGlobalPlane(elemK, 3, dLocal_zOnGP, nDataType)) return FALSE;

	double dMaxDy = GetLocalDirectionValue(dLocal_yOnGP, DispIMax.dblDisp[0], DispIMax.dblDisp[1]) - GetLocalDirectionValue(dLocal_yOnGP, DispJMax.dblDisp[0], DispJMax.dblDisp[1]);
	double dMaxDz = GetLocalDirectionValue(dLocal_zOnGP, DispIMax.dblDisp[0], DispIMax.dblDisp[1]) - GetLocalDirectionValue(dLocal_zOnGP, DispJMax.dblDisp[0], DispJMax.dblDisp[1]);
	double dMinDy = GetLocalDirectionValue(dLocal_yOnGP, DispIMin.dblDisp[0], DispIMin.dblDisp[1]) - GetLocalDirectionValue(dLocal_yOnGP, DispJMin.dblDisp[0], DispJMin.dblDisp[1]);
	double dMinDz = GetLocalDirectionValue(dLocal_zOnGP, DispIMin.dblDisp[0], DispIMin.dblDisp[1]) - GetLocalDirectionValue(dLocal_zOnGP, DispJMin.dblDisp[0], DispJMin.dblDisp[1]);

	dDy = max(fabs(dMaxDy), fabs(dMinDy));
	dDz = max(fabs(dMaxDz), fabs(dMinDz));
	return TRUE;
}
BOOL CRCDataCtrl_CH::GetLocalDirectionOnGlobalPlane(UINT elemK, int nAxis, double dirVectorN[2], int nDataType)
{
	double localVector[3][3];
	if(nDataType == 1)
    { m_pDoc->calcFrameLocalVector(elemK, localVector); }
	else if(nDataType == 2 || nDataType == 3)
	{
		T_NODE_K Node1K, Node2K;
		double   dBetaAngle;
		if(nDataType == 2)
		{
			T_ELNK_D ElnkD;
			m_pDoc->m_pAttrCtrl->GetElnk(elemK, ElnkD);
			Node1K = ElnkD.Node1;  Node2K = ElnkD.Node2;
			dBetaAngle = ElnkD.dBetaAngle;
		}
		else
		{ 
			T_NLNK_D NlnkD;
			m_pDoc->m_pAttrCtrl->GetNlnk(elemK, NlnkD);
			Node1K = NlnkD.Node1;  Node2K = NlnkD.Node2;
			dBetaAngle = NlnkD.dBetaAngle;			
		}	
		T_NODE_D Node1D, Node2D;		
		m_pDoc->m_pAttrCtrl->GetNode(Node1K, Node1D);
		m_pDoc->m_pAttrCtrl->GetNode(Node2K, Node2D);
		m_pDoc->calcFrameLocalVector(Node1D, Node2D, dBetaAngle, localVector);
	}
	else ASSERT(0);

	double projLocal[3]  = {0.0, 0.0, 0.0};
	double projLocalN[3] = {0.0, 0.0, 0.0};
    // Global X-Y 乞搁俊 捧康等 local 谅钎拌狼 窜困 Vector 己盒.
	switch(nAxis)
	{
		case 1:
			projLocal[0] = localVector[0][0];  
			projLocal[1] = localVector[0][1];
			break;
		case 2:
			projLocal[0] = localVector[1][0];  
			projLocal[1] = localVector[1][1];
			break;
		case 3:
			projLocal[0] = localVector[2][0];  
			projLocal[1] = localVector[2][1];
			break;
		default:
			ASSERT(0);
			return FALSE;
	}
	
    if(CMathFunc::mathNormalize(projLocal, projLocalN))
	{
		dirVectorN[0] = projLocalN[0];
		dirVectorN[1] = projLocalN[1];
	}
	else
	{
		dirVectorN[0] = dirVectorN[1] = 0.0;
		return FALSE;
	}
	return TRUE;
}

double CRCDataCtrl_CH::GetLocalDirectionValue(double dirVectorN[2], double dX, double dY)
{
	double dLength = CMathFunc::mathLength(dX, dY);
	
	double dDisVector[2] = {dX, dY};
	double dDisVectorN[2] = {0.0, 0.0};
	CMathFunc::mathNormalize2D(dDisVector, dDisVectorN);

	if((dirVectorN[0]==0.0 && dirVectorN[1]==0.0) || (dDisVectorN[0]==0.0 && dDisVectorN[1]==0.0))
		return 0.0;

	double dAngle = CMathFunc::mathCrossAngle2DSign(dirVectorN[0], dirVectorN[1], dDisVectorN[0], dDisVectorN[1]) * CMathFunc::m_trrad;

	return dLength * cos(dAngle);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Need Korea to do 
//--------------------------Get check Member or Element List-------------------------------------------------------
//Get Pier Member List
BOOL CRCDataCtrl_CH::Get_PierMemberListForSeismicDgn(CArray<T_RSDL_K,T_RSDL_K>& MemberList)
{// Member List
    MemberList.RemoveAll();  
    CArray<T_RSDL_K, T_RSDL_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdlKeyList(KeyList);
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        T_RSDL_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdl(KeyList[i], dataType)) return FALSE;
        
        if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==10:dataType.nMemberType==0))
        {
            MemberList.Add(KeyList[i]);
        }
        if((rcd.iDgnCode == RC_CJJ_166_2011) && (dataType.nMemberType==12))
        {
            MemberList.Add(KeyList[i]);
        }
    }
    return (MemberList.GetSize() > 0);
}
//Get Abutment Element List
BOOL CRCDataCtrl_CH::Get_AbutmentElemListForSeismicDgn(CArray<T_ELEM_K,T_ELEM_K>& ElemKList)
{  //ElemK List
    ElemKList.RemoveAll();
    CArray<T_RSDM_K, T_RSDM_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdmKeyList(1, KeyList);
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        T_RSDM_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdm(KeyList[i], dataType)) return FALSE;
        if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==11:dataType.nMemberType==1))//1-Abutment
        {
            ElemKList.Add(KeyList[i].key.OKey);
        }
    }
    return (ElemKList.GetSize() > 0);
}
//Get Foundation Element List
BOOL CRCDataCtrl_CH::Get_FoundationElemListForSeismicDgn(CArray<T_ELEM_K,T_ELEM_K>& ElemKList)
{  //ElemK List
    ElemKList.RemoveAll();
    CArray<T_RSDM_K, T_RSDM_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdmKeyList(1, KeyList);//Element
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        T_RSDM_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdm(KeyList[i], dataType)) return FALSE;
        if(rcd.iDgnCode == RC_CJJ_166_2011 ? FALSE : dataType.nMemberType==2)//2-Foundition
        {
            ElemKList.Add(KeyList[i].key.OKey);
        }
    }
    return (ElemKList.GetSize() > 0);
}
//Get Cap beam Member List
BOOL CRCDataCtrl_CH::Get_CapBeamMemberListForSeismicDgn(CArray<T_RSDL_K,T_RSDL_K>& MemberList)
{// Member List
    MemberList.RemoveAll();  
    CArray<T_RSDL_K, T_RSDL_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdlKeyList(KeyList);
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        T_RSDL_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdl(KeyList[i], dataType)) return FALSE;
        if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==13:dataType.nMemberType==3))
        {
            MemberList.Add(KeyList[i]);
        }
    }
    return (MemberList.GetSize() > 0);
}
//Get Rubber Link List
BOOL CRCDataCtrl_CH::Get_RubberLinkListForSeismicDgn(CArray<T_ELNK_K,T_ELNK_K>& LinkKList)
{// Link List
    LinkKList.RemoveAll();
    CArray<T_RSDM_K, T_RSDM_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdmKeyList(2, KeyList);//2.Elastic Link
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        T_RSDM_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdm(KeyList[i], dataType)) return FALSE;
        if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==14:dataType.nMemberType==4))//4-Rubber Link
        {
            LinkKList.Add(KeyList[i].key.OKey);
        }
    }
    return (LinkKList.GetSize() > 0);
}
//Get Active Link List-Only general link
BOOL CRCDataCtrl_CH::Get_ActiveLinkListForSeismicDgn(CArray<T_NLNK_K,T_NLNK_K>& LinkKList)
{// Link List
    LinkKList.RemoveAll();
    CArray<T_RSDM_K, T_RSDM_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdmKeyList(3, KeyList);//3.general Link
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        LinkKList.Add(KeyList[i].key.OKey);
//         T_RSDM_D dataType;
//         if(!m_pDoc->m_pAttrCtrl->GetRsdm(KeyList[i], dataType)) return FALSE;
//          if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==16:dataType.nMemberType==6))//4-Active Link
//         {
//           LinkKList.Add(KeyList[i].key.OKey);
//         }
    }
    return (LinkKList.GetSize() > 0);
}
//Get fixed Link List-Only Elastic link
BOOL CRCDataCtrl_CH::Get_FixedLinkListForSeismicDgn(CArray<T_ELNK_K,T_ELNK_K>& LinkKList)
{// Link List
    LinkKList.RemoveAll();
    CArray<T_RSDM_K, T_RSDM_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdmKeyList(2, KeyList);//2.Elastic Link
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        T_RSDM_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdm(KeyList[i], dataType)) return FALSE;
        if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==16:dataType.nMemberType==6))//4-fixed Link
        {
            LinkKList.Add(KeyList[i].key.OKey);
        }
    }
    return (LinkKList.GetSize() > 0);
}
//Get Main Arch Element List
BOOL CRCDataCtrl_CH::Get_MainArchElemListForSeismicDgn(CArray<T_ELEM_K,T_ELEM_K>& ElemKList)
{
    ElemKList.RemoveAll();
    CArray<T_RSDM_K, T_RSDM_K> KeyList;
    m_pDoc->m_pAttrCtrl->GetRsdmKeyList(1, KeyList);
    int nSize = KeyList.GetSize();
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    for(int i=0 ; i<nSize ; i++)
    {
        T_RSDM_D dataType;
        if(!m_pDoc->m_pAttrCtrl->GetRsdm(KeyList[i], dataType)) return FALSE;
        if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==17:dataType.nMemberType==7))//7-Main arch
        {
            ElemKList.Add(KeyList[i].key.OKey);
        }
    }
    return (ElemKList.GetSize() > 0);
}
//Get Pier's element list by pierID
BOOL CRCDataCtrl_CH::Get_PierElemListByPierID (T_RSDL_K iMember, CArray<T_ELEM_K,T_ELEM_K>& ElemKList)
{//ElemK List
    ElemKList.RemoveAll();
    T_RSDL_D dataType;
    if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, dataType)) return FALSE;
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
 if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==10 || dataType.nMemberType==12:dataType.nMemberType==0))//1:Pier
    {
        ElemKList.Copy(dataType.aElemList);
    }
 
    return (ElemKList.GetSize() > 0);
}
//Get CapBeam's element list by CapBeamID
BOOL CRCDataCtrl_CH::Get_CapBeamElemListByCapBeamId(T_RSDL_K iMember, CArray<T_ELEM_K,T_ELEM_K>& ElemKList)
{//ElemK List
    ElemKList.RemoveAll();
    T_RSDL_D dataType;
    if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, dataType)) return FALSE;
    T_RC_D_CH rcd;
    rcd.Initialize();
    Get_Rcd(rcd);
    if((rcd.iDgnCode == RC_CJJ_166_2011 ? dataType.nMemberType==13:dataType.nMemberType==3))//3:Capping Beam
    {
        ElemKList.Copy(dataType.aElemList);
    }
    return (ElemKList.GetSize() > 0);
}  

//--------------------------Get Stirrup data ------------------------------------------------------------------------
//获取盖梁对应的关联桥墩构件编号。
//根据盖梁选择的节点求得。如果选择了节点，并节点在某个墩上，则添加到结果列表。
BOOL CRCDataCtrl_CH::Get_PierListofCapBeam(T_RSDL_K iCapBeamMember, CArray<T_RSDL_K,T_RSDL_K>&PierMemberList)
{
	PierMemberList.RemoveAll();
	T_RSDL_D RsdlD;
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iCapBeamMember, RsdlD)) return FALSE;
	int nNodeSize = RsdlD.aNodeList.GetSize();
	if(nNodeSize == 0) return FALSE;

	if(!Get_PierMemberListForSeismicDgn(PierMemberList)) { PierMemberList.RemoveAll();  return FALSE; }
	int nSize = PierMemberList.GetSize();
	if(nSize == 0) return FALSE;

	int i, j, nPos;
	T_ELEM_K elemk[2];
	int ij[2];
	T_ELEM_D elemD;
	BOOL bFind;
	for(i=nSize-1 ; i>=0 ; i--)
	{
		if(!GetElemkijFromPierMemberID(PierMemberList[i], elemk, ij)) { PierMemberList.RemoveAt(i);  continue; }
		if(!m_pDoc->m_pAttrCtrl->GetElem(elemk[0], elemD)) { PierMemberList.RemoveAt(i);  continue; }
		nPos = ij[0];

		bFind = FALSE;
		for(j=0 ; j<nNodeSize ; j++)
		{
			if(RsdlD.aNodeList[j] == elemD.elnod[nPos])
			{ bFind = TRUE;  break; }
		}

		if(!bFind) PierMemberList.RemoveAt(i);
	}

	return (PierMemberList.GetSize() > 0);
}
//Get Pier's End Elemk and ij. elemk: 0-Top, 1-bottom;  ij:Position(0:I 1:J) (x,y,z), small z is bottom
BOOL CRCDataCtrl_CH::GetElemkijFromPierMemberID(T_RSDL_K iMember, T_ELEM_K elemk[2], int ij[2])
{
	T_RSDL_D RsdlData;
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, RsdlData)) return FALSE;

	if(!CDBLib::GetSortElem_Position(RsdlData.aElemList)) return FALSE;
	T_ELEM_D ElemD_I, ElemD_J;
	T_NODE_D NodeD_I, NodeD_J;
	int nSize = RsdlData.aElemList.GetSize();
	if(nSize == 0) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetElem(RsdlData.aElemList[0],       ElemD_I)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetElem(RsdlData.aElemList[nSize-1], ElemD_J)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD_I.elnod[0], NodeD_I)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD_J.elnod[1], NodeD_J)) return FALSE;

	if(NodeD_I.z >= NodeD_J.z)
	{// I窜捞 Top牢 版快
		elemk[0] = RsdlData.aElemList[0];
		elemk[1] = RsdlData.aElemList[nSize-1];
		ij[0] = 0;
		ij[1] = 1;
	}
	else 
	{// J窜捞 Top牢 版快
		elemk[0] = RsdlData.aElemList[nSize-1];
		elemk[1] = RsdlData.aElemList[0];
		ij[0] = 1;
		ij[1] = 0;
	}

    return TRUE;
}
//获取盖梁对应的两端的单元,ij截面信息0: 一端, 1: 另一端 
BOOL CRCDataCtrl_CH::GetElemkijFromCapBeamMemberID(T_RSDL_K iMember, int elemk[2], int ij[2])
{
	T_RSDL_D RsdlData;
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, RsdlData)) return FALSE;

	if(!CDBLib::GetSortElem_Position(RsdlData.aElemList)) return FALSE;

	int nSize = RsdlData.aElemList.GetSize();
	elemk[0] = RsdlData.aElemList[0];
	elemk[1] = RsdlData.aElemList[nSize-1];
	ij[0] = 0;
	ij[1] = 1;

    return TRUE;
}

//--------------------------Get Load combination data ------------------------------------------------------------------------
//Get Element's seismic Load combination Force (E1 same to E2)
BOOL CRCDataCtrl_CH::Get_SeismicForce(T_ELEM_K ElemK, _DGN_FORC_CRC aForce[12])
{
    for (int i=0;i<12;i++) aForce[i].Initialize(i%2==0);
    
    BOOL aIsChk[12];
    for(int i=0 ; i<12 ; i++) aIsChk[i] = FALSE;
    
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

    BOOL bEqLcom = FALSE;
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
    if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;

    for(int i=0; i<aForceLcom.GetSize(); i++)	
    {
        if (!m_ForceCtrl->IsEqLcom_JTG_B02(i+1)) continue;
        bEqLcom = TRUE;
        _DGN_FORC_CRC EqForce;		
        m_pDesignForceResult->GetForceLcomResult(EPairK,i+1,EqForce);


        GetEqForceExTempForce(ElemK,i+1,EqForce);

        
        for(int j=0; j<2; j++){	// I,J.				
            EqForce.iRefNo[j] = i+1;
            if(EqForce.dFxx[j] >= aForce[ 0].dFxx[j]){ aForce[ 0] = EqForce;  aIsChk[ 0] = TRUE; }
            if(EqForce.dFxx[j] <= aForce[ 1].dFxx[j]){ aForce[ 1] = EqForce;  aIsChk[ 1] = TRUE; }
            if(EqForce.dFyy[j] >= aForce[ 2].dFyy[j]){ aForce[ 2] = EqForce;  aIsChk[ 2] = TRUE; }
            if(EqForce.dFyy[j] <= aForce[ 3].dFyy[j]){ aForce[ 3] = EqForce;  aIsChk[ 3] = TRUE; }
            if(EqForce.dFzz[j] >= aForce[ 4].dFzz[j]){ aForce[ 4] = EqForce;  aIsChk[ 4] = TRUE; }
            if(EqForce.dFzz[j] <= aForce[ 5].dFzz[j]){ aForce[ 5] = EqForce;  aIsChk[ 5] = TRUE; }
            if(EqForce.dMux[j] >= aForce[ 6].dMux[j]){ aForce[ 6] = EqForce;  aIsChk[ 6] = TRUE; }
            if(EqForce.dMux[j] <= aForce[ 7].dMux[j]){ aForce[ 7] = EqForce;  aIsChk[ 7] = TRUE; }
            if(EqForce.dMuy[j] >= aForce[ 8].dMuy[j]){ aForce[ 8] = EqForce;  aIsChk[ 8] = TRUE; }
            if(EqForce.dMuy[j] <= aForce[ 9].dMuy[j]){ aForce[ 9] = EqForce;  aIsChk[ 9] = TRUE; }
            if(EqForce.dMuz[j] >= aForce[10].dMuz[j]){ aForce[10] = EqForce;  aIsChk[10] = TRUE; }
            if(EqForce.dMuz[j] <= aForce[11].dMuz[j]){ aForce[11] = EqForce;  aIsChk[11] = TRUE; }
        }
    }
    
    for (int i=0;i<12;i++) 
    { if(!aIsChk[i]) aForce[i].Initialize(); }
    //change unit
    for (int i=0;i<12;i++) 
    {
        for (int j=0; j<2; j++) 
        {
            aForce[i].dFxx[j] = Cng_ForceFromCurrToCode(aForce[i].dFxx[j]);
            aForce[i].dFyy[j] = Cng_ForceFromCurrToCode(aForce[i].dFyy[j]);
            aForce[i].dFzz[j] = Cng_ForceFromCurrToCode(aForce[i].dFzz[j]);
            aForce[i].dMux[j] = Cng_MomentFromCurrToCode(aForce[i].dMux[j]);
            aForce[i].dMuy[j] = Cng_MomentFromCurrToCode(aForce[i].dMuy[j]);
            aForce[i].dMuz[j] = Cng_MomentFromCurrToCode(aForce[i].dMuz[j]);
        }
    }	
	return TRUE;
}
//Get Element's Permanent Load Force (E1 same to E2) [D+tendon(CS)+EH+EV+WP+B]
BOOL CRCDataCtrl_CH::Get_PermanentSeismicForce (T_ELEM_K ElemK, _DGN_FORC_CRC aForce[12])
{
    for (int i=0;i<12;i++) aForce[i].Initialize(i%2==0);
    
    BOOL aIsChk[12];
    for(int i=0 ; i<12 ; i++) aIsChk[i] = FALSE;
    
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

    BOOL bEqLcom = FALSE;
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
    if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;

    for(int i=0; i<aForceLcom.GetSize(); i++)	
    {
        if (!m_ForceCtrl->IsEqLcom_JTG_B02(i+1)) continue;
        bEqLcom = TRUE;
        _DGN_FORC_CRC EqForce;		
        m_pDesignForceResult->GetForceEQE2_JTGResult(EPairK,i+1,EqForce);

        GetEqForceExTempForce(ElemK,i+1,EqForce);
        for(int j=0; j<2; j++){	// I,J.				
            EqForce.iRefNo[j] = i+1;
            if(EqForce.dFxx[j] >= aForce[ 0].dFxx[j]){ aForce[ 0] = EqForce;  aIsChk[ 0] = TRUE; }
            if(EqForce.dFxx[j] <= aForce[ 1].dFxx[j]){ aForce[ 1] = EqForce;  aIsChk[ 1] = TRUE; }
            if(EqForce.dFyy[j] >= aForce[ 2].dFyy[j]){ aForce[ 2] = EqForce;  aIsChk[ 2] = TRUE; }
            if(EqForce.dFyy[j] <= aForce[ 3].dFyy[j]){ aForce[ 3] = EqForce;  aIsChk[ 3] = TRUE; }
            if(EqForce.dFzz[j] >= aForce[ 4].dFzz[j]){ aForce[ 4] = EqForce;  aIsChk[ 4] = TRUE; }
            if(EqForce.dFzz[j] <= aForce[ 5].dFzz[j]){ aForce[ 5] = EqForce;  aIsChk[ 5] = TRUE; }
            if(EqForce.dMux[j] >= aForce[ 6].dMux[j]){ aForce[ 6] = EqForce;  aIsChk[ 6] = TRUE; }
            if(EqForce.dMux[j] <= aForce[ 7].dMux[j]){ aForce[ 7] = EqForce;  aIsChk[ 7] = TRUE; }
            if(EqForce.dMuy[j] >= aForce[ 8].dMuy[j]){ aForce[ 8] = EqForce;  aIsChk[ 8] = TRUE; }
            if(EqForce.dMuy[j] <= aForce[ 9].dMuy[j]){ aForce[ 9] = EqForce;  aIsChk[ 9] = TRUE; }
            if(EqForce.dMuz[j] >= aForce[10].dMuz[j]){ aForce[10] = EqForce;  aIsChk[10] = TRUE; }
            if(EqForce.dMuz[j] <= aForce[11].dMuz[j]){ aForce[11] = EqForce;  aIsChk[11] = TRUE; }
        }
    }
    
    for (int i=0;i<12;i++) 
    { if(!aIsChk[i]) aForce[i].Initialize(); }
    //change unit
    for (int i=0;i<12;i++) 
    {
        for (int j=0; j<2; j++) 
        {
            aForce[i].dFxx[j] = Cng_ForceFromCurrToCode(aForce[i].dFxx[j]);
            aForce[i].dFyy[j] = Cng_ForceFromCurrToCode(aForce[i].dFyy[j]);
            aForce[i].dFzz[j] = Cng_ForceFromCurrToCode(aForce[i].dFzz[j]);
            aForce[i].dMux[j] = Cng_MomentFromCurrToCode(aForce[i].dMux[j]);
            aForce[i].dMuy[j] = Cng_MomentFromCurrToCode(aForce[i].dMuy[j]);
            aForce[i].dMuz[j] = Cng_MomentFromCurrToCode(aForce[i].dMuz[j]);
        }
    }	
	return TRUE;
}
//Get Element's self weight Load Force (E1 same to E2) 
BOOL CRCDataCtrl_CH::Get_WeightSeismicForce (T_ELEM_K ElemK, _DGN_FORC_CRC aForce[12])
{
    for (int i=0;i<12;i++) aForce[i].Initialize(i%2==0);
    
    BOOL aIsChk[12];
    for(int i=0 ; i<12 ; i++) aIsChk[i] = FALSE;
    
	ElemPairK EPairK(ElemK, EN_EL_BEAM);

    BOOL bEqLcom = FALSE;
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcom;
    if(!m_ForceCtrl->Get_LcomDataForDesign(EPairK, 0, TRUE, FALSE, aForceLcom, iLcomNum))	return FALSE;
    for(int i=0; i<aForceLcom.GetSize(); i++)	
    {
        if (!m_ForceCtrl->IsEqLcom_JTG_B02(i+1)) continue;
        bEqLcom = TRUE;
        _DGN_FORC_CRC EqForce;		
        m_pDesignForceResult->GetForceWeight_JTGResult(EPairK,i+1,EqForce);
        for(int j=0; j<2; j++){	// I,J.				
            EqForce.iRefNo[j] = i+1;
            if(EqForce.dFxx[j] >= aForce[ 0].dFxx[j]){ aForce[ 0] = EqForce;  aIsChk[ 0] = TRUE; }
            if(EqForce.dFxx[j] <= aForce[ 1].dFxx[j]){ aForce[ 1] = EqForce;  aIsChk[ 1] = TRUE; }
            if(EqForce.dFyy[j] >= aForce[ 2].dFyy[j]){ aForce[ 2] = EqForce;  aIsChk[ 2] = TRUE; }
            if(EqForce.dFyy[j] <= aForce[ 3].dFyy[j]){ aForce[ 3] = EqForce;  aIsChk[ 3] = TRUE; }
            if(EqForce.dFzz[j] >= aForce[ 4].dFzz[j]){ aForce[ 4] = EqForce;  aIsChk[ 4] = TRUE; }
            if(EqForce.dFzz[j] <= aForce[ 5].dFzz[j]){ aForce[ 5] = EqForce;  aIsChk[ 5] = TRUE; }
            if(EqForce.dMux[j] >= aForce[ 6].dMux[j]){ aForce[ 6] = EqForce;  aIsChk[ 6] = TRUE; }
            if(EqForce.dMux[j] <= aForce[ 7].dMux[j]){ aForce[ 7] = EqForce;  aIsChk[ 7] = TRUE; }
            if(EqForce.dMuy[j] >= aForce[ 8].dMuy[j]){ aForce[ 8] = EqForce;  aIsChk[ 8] = TRUE; }
            if(EqForce.dMuy[j] <= aForce[ 9].dMuy[j]){ aForce[ 9] = EqForce;  aIsChk[ 9] = TRUE; }
            if(EqForce.dMuz[j] >= aForce[10].dMuz[j]){ aForce[10] = EqForce;  aIsChk[10] = TRUE; }
            if(EqForce.dMuz[j] <= aForce[11].dMuz[j]){ aForce[11] = EqForce;  aIsChk[11] = TRUE; }
        }
    }
    
    for (int i=0;i<12;i++) 
    { if(!aIsChk[i]) aForce[i].Initialize(); }
    //change unit
    for (int i=0;i<12;i++) 
    {
        for (int j=0; j<2; j++) 
        {
            aForce[i].dFxx[j] = Cng_ForceFromCurrToCode(aForce[i].dFxx[j]);
            aForce[i].dFyy[j] = Cng_ForceFromCurrToCode(aForce[i].dFyy[j]);
            aForce[i].dFzz[j] = Cng_ForceFromCurrToCode(aForce[i].dFzz[j]);
            aForce[i].dMux[j] = Cng_MomentFromCurrToCode(aForce[i].dMux[j]);
            aForce[i].dMuy[j] = Cng_MomentFromCurrToCode(aForce[i].dMuy[j]);
            aForce[i].dMuz[j] = Cng_MomentFromCurrToCode(aForce[i].dMuz[j]);
        }
    }	
	return TRUE;
}

//------------------------------------------------------------------------------------------------------
//Get Maxθp and it's elements. 0-Longitudinal;1-Transverse.
//data[0]:Maxθp of Longitudinal direction. and LongitudinalElemList is the Maxθp's elements.
//data[1]:Maxθp of Transverse direction. and TransverseElemList is the Maxθp's elements.
BOOL CRCDataCtrl_CH::Get_SeismicE2Theta(T_RSDL_K iMember,T_CORNERDISP_CH& data)

{
	data.Initialize();

	CArray<T_THIS_K,T_THIS_K> ThisKeyList;
    m_pDoc->m_pAttrCtrl->GetThisKeyList(ThisKeyList);
	int nThisSize = ThisKeyList.GetSize();
	if(nThisSize == 0) return FALSE;

	// 腮瘤备康狼 Node困摹 魂沥
	T_RSDL_D RsdlD;
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, RsdlD)) return FALSE;

	if(!CDBLib::GetSortElem_Position(RsdlD.aElemList)) return FALSE;

	T_ELEM_D ElemD_I, ElemD_J;
	T_NODE_D NodeD_I, NodeD_J;
	int nElemSize = RsdlD.aElemList.GetSize();
	if(nElemSize == 0) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetElem(RsdlD.aElemList[0],           ElemD_I)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetElem(RsdlD.aElemList[nElemSize-1], ElemD_J)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD_I.elnod[0], NodeD_I)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD_J.elnod[1], NodeD_J)) return FALSE;

	BOOL bI_Top = (NodeD_I.z >= NodeD_J.z);
	T_ELEM_K ElemK_Top, ElemK_Top2, ElemK_Bottom, ElemK_Bottom2;
	T_NODE_K NodeKI1_Top, NodeKJ1_Top, NodeKI2_Top, NodeKJ2_Top, NodeKI1_Bot, NodeKJ1_Bot, NodeKI2_Bot, NodeKJ2_Bot;
	double dRateS_Top, dRateE_Top, dRateS_Bot, dRateE_Bot;

	double dLp_Top, dLp_Bottom;	
	if(!Get_Lp_JTG(iMember, TRUE,  dLp_Top))    return FALSE;
	if(!Get_Lp_JTG(iMember, FALSE, dLp_Bottom)) return FALSE;
	if(bI_Top)
	{
		ElemK_Top    = RsdlD.aElemList[0];            dRateS_Top = 1.0;
		ElemK_Bottom = RsdlD.aElemList[nElemSize-1];  dRateS_Bot = 0.0;
		if(!Get_ElemPosition(RsdlD.aElemList, dLp_Top,    TRUE,  ElemK_Top2,    dRateE_Top)) return FALSE;
		if(!Get_ElemPosition(RsdlD.aElemList, dLp_Bottom, FALSE, ElemK_Bottom2, dRateE_Bot)) return FALSE;
	}
	else 
	{
		ElemK_Top    = RsdlD.aElemList[nElemSize-1];  dRateS_Top = 0.0;
		ElemK_Bottom = RsdlD.aElemList[0];            dRateS_Bot = 1.0;
		if(!Get_ElemPosition(RsdlD.aElemList, dLp_Top,    FALSE, ElemK_Top2,    dRateE_Top)) return FALSE;
		if(!Get_ElemPosition(RsdlD.aElemList, dLp_Bottom, TRUE,  ElemK_Bottom2, dRateE_Bot)) return FALSE;
	}
	
	T_ELEM_D ElemD;
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK_Top, ElemD)) return FALSE;
	NodeKI1_Top = ElemD.elnod[0];  NodeKJ1_Top = ElemD.elnod[1];
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK_Top2, ElemD)) return FALSE;
	NodeKI2_Top = ElemD.elnod[0];  NodeKJ2_Top = ElemD.elnod[1];

	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK_Bottom, ElemD)) return FALSE;
	NodeKI1_Bot = ElemD.elnod[0];  NodeKJ1_Bot = ElemD.elnod[1];
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK_Bottom2, ElemD)) return FALSE;
	NodeKI2_Bot = ElemD.elnod[0];  NodeKJ2_Bot = ElemD.elnod[1];

	int i;
	nElemSize = RsdlD.aElemList.GetSize();
	T_ELEM_K_LIST arElemListTop, arElemListBot;
	arElemListTop.RemoveAll();  arElemListBot.RemoveAll();
	BOOL bTopFind, bBotFind;
	bTopFind = bBotFind = FALSE;
	for(i=0 ; i<nElemSize ; i++)
	{
		if(RsdlD.aElemList[i] == ElemK_Top || RsdlD.aElemList[i] == ElemK_Top2)
		{
			arElemListTop.Add(RsdlD.aElemList[i]);
			bTopFind= bTopFind ? FALSE : TRUE;
		}
		else if(bTopFind)
		{ arElemListTop.Add(RsdlD.aElemList[i]); }

		
		if(RsdlD.aElemList[i] == ElemK_Bottom || RsdlD.aElemList[i] == ElemK_Bottom2)
		{
			arElemListBot.Add(RsdlD.aElemList[i]);
			bBotFind= bBotFind ? FALSE : TRUE;
		}
		else if(bBotFind)
		{ arElemListBot.Add(RsdlD.aElemList[i]); }
	}

	int nThetaCalcType = 2; // 1:矫累Node客 场Node狼 雀傈阿 瞒捞肺 魂免,  2:矫累Elem何磐 场Elem鳖瘤狼 Ry, Rz狼 穿利蔼栏肺 魂免
	// 雀傈阿 阂矾坷扁
	T_THIS_K ThisK;
	double dMaxTheta[2];
	int nMaxTimeIndex[2];
	int nDataMaxTimeIndex[2];
	nDataMaxTimeIndex[0] = nDataMaxTimeIndex[1] = 0;
	for(i=0 ; i<nThisSize ; i++)
	{
		ThisK = ThisKeyList[i];
		if(nThetaCalcType==1) { if(!Get_Theta(ThisK, NodeKI1_Bot, NodeKJ1_Bot, dRateS_Bot, NodeKI2_Bot, NodeKJ2_Bot, dRateE_Bot, dMaxTheta, nMaxTimeIndex)) continue; }
		else                  { if(!Get_Theta(ThisK, arElemListBot, dRateS_Bot, dRateE_Bot, dMaxTheta, nMaxTimeIndex)) continue; }
		
		if(fabs(data.DataBase[0].dHoriDisp) < fabs(dMaxTheta[0]))
		{
			data.DataBase[0].dHoriDisp = dMaxTheta[0];
			data.DataBase[0].LcomK     = ThisK;
			nDataMaxTimeIndex[0]       = nMaxTimeIndex[0];
		}
		if(fabs(data.DataBase[1].dHoriDisp) < fabs(dMaxTheta[1]))
		{
			data.DataBase[1].dHoriDisp = dMaxTheta[1];
			data.DataBase[1].LcomK     = ThisK;
			nDataMaxTimeIndex[1]       = nMaxTimeIndex[1];
		}
		
		if(RsdlD.nPierPHingeRegionType==1)
		{
			if(nThetaCalcType==1) { if(!Get_Theta(ThisK, NodeKI1_Top, NodeKJ1_Top, dRateS_Top, NodeKI2_Top, NodeKJ2_Top, dRateE_Top, dMaxTheta, nMaxTimeIndex)) continue; }
			else                  { if(!Get_Theta(ThisK, arElemListTop, dRateS_Top, dRateE_Top, dMaxTheta, nMaxTimeIndex)) continue; }
			if(fabs(data.DataBase[0].dHoriDisp) < fabs(dMaxTheta[0]))
			{
				data.DataBase[0].dHoriDisp = dMaxTheta[0];
				data.DataBase[0].LcomK     = ThisK;
				nDataMaxTimeIndex[0]       = nMaxTimeIndex[0];
			}
			if(fabs(data.DataBase[1].dHoriDisp) < fabs(dMaxTheta[1]))
			{
				data.DataBase[1].dHoriDisp = dMaxTheta[1];
				data.DataBase[1].LcomK     = ThisK;
				nDataMaxTimeIndex[1]       = nMaxTimeIndex[1];
			}
		}
	}

	data.DataBase[0].iKey = data.DataBase[1].iKey = iMember;
	data.DataBase[0].iDirect = 0;
	data.DataBase[1].iDirect = 1;
	
    return TRUE;
}

BOOL CRCDataCtrl_CH::Get_Lp_JTG(T_RSDL_K iMember, BOOL bTop, double& dLp)
{
	dLp = 0.0;

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	BOOL bUnitChanged = (CurrUnit.nBase_Force != D_UNITSYS_FORCE_INDEX_N || CurrUnit.nBase_Length != D_UNITSYS_LENGTH_INDEX_MM);
	if(bUnitChanged)
	{
		T_UNIT_INDEX CodeUnit;
		CodeUnit = CurrUnit;
		CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
		CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
		m_pDoc->m_pMatlDB->Create_RebarData(CodeUnit.nBase_Length, CodeUnit.nBase_Force, TRUE);
	}
	BOOL bChk = TRUE;

	T_RSDL_D RsdlD;
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, RsdlD)) { bChk = FALSE; }

	T_ELEM_K trgElemK = 0;
	int      trgPos   = 0;
	if(bChk)
	{
		T_ELEM_K elemk[2];
		int ij[2];
		if(!GetElemkijFromPierMemberID(iMember, elemk, ij)) { bChk = FALSE; }
		else 
		{
			trgElemK = bTop ? elemk[0] : elemk[1];
			trgPos   = bTop ? ij[0] : ij[1];
			trgPos = trgPos==0 ? 0 : 2;
		}
	}
	
	T_ELEM_D ElemD;	
	_DGN_RC_MATL_CH MatdD;
	_DGN_RC_COLUMN_SECTION_CH sectioni;
	if(bChk)
	{ if(!m_pDoc->m_pAttrCtrl->GetElem(trgElemK, ElemD)) { bChk = FALSE; } }		
	if(bChk)
	{ if(!Get_Matl(trgElemK, MatdD)) { bChk = FALSE; } }  
	if(bChk)
	{ if(!Get_ColumnSection(trgElemK,(trgPos==0?0:1),sectioni)) { bChk = FALSE; } }

	if(bChk)
	{
		double dHn = RsdlD.dPierHn * 10.0; // cm
		double dfy = MatdD.dfsk; // MPa
		double db  = min(sectioni.dCym+sectioni.dCyp, sectioni.dCzm+sectioni.dCzp) * 10.0; // cm
		double dds = 0.0; // cm

		// Before_Release_v825_Check - Rebar 备炼眉 函版登搁辑 荐沥登绢具 窍绰 何盒 
		// 老窜 0 栏肺 绊沥秦滴菌促~	

		T_RCHK_D RchkD;
		if(m_pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro, RchkD))
		{
			if(RchkD.COLM.dDc[0][0] > 0.0)
			{
				dds = m_pDoc->m_pMatlDB->Get_RebarDia(RchkD.COLM.strBarNa1[0][0][0]);
				if(RchkD.COLM.strBarNa2[0][0][0] != _T(""))
					dds = max(dds, m_pDoc->m_pMatlDB->Get_RebarDia(RchkD.COLM.strBarNa2[0][0][0]));
			}
		}	

		dLp = min(max(0.08*dHn+0.022*dfy*dds, 0.044*dfy*dds), 2.0/3.0*db); // cm
		dLp /= 10.0; // mm窜困肺 函券
	}

	if(bUnitChanged)
	{
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit); 
		m_pDoc->m_pMatlDB->Create_RebarData(CurrUnit.nBase_Length, CurrUnit.nBase_Force, TRUE);
	}
	return bChk;
}

//Get Pier's top Disp..//取得规则桥墩墩顶位移
BOOL CRCDataCtrl_CH::Get_SeismicE2FrustaTopDisp(T_RSDL_K iMember,T_FRUSTATOPDISP_CH& data)
{
	data.Initialize();

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);

	T_RSDL_D RsdlData;	
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, RsdlData))  { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
	if(!CDBLib::GetSortElem_Position(RsdlData.aElemList)) { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
	int nSizeElem = RsdlData.aElemList.GetSize();
	if(nSizeElem == 0)                                    { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }

	// 窍窜 Node锅龋 棺 Position搬沥
	int nBasicPos = 1;
	T_NODE_K NodeK_Bot = 0;	
	T_ELEM_D ElemD1, ElemD2;
	T_NODE_D NodeD1, NodeD2;
	if(!m_pDoc->m_pAttrCtrl->GetElem(RsdlData.aElemList[0],ElemD1))           { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD1.elnod[0], NodeD1))                { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
	if(!m_pDoc->m_pAttrCtrl->GetElem(RsdlData.aElemList[nSizeElem-1],ElemD2)) { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD2.elnod[1], NodeD2))                { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
	if(NodeD1.z > NodeD2.z) { nBasicPos = 2;  NodeK_Bot = ElemD2.elnod[1]; }
	else                    { nBasicPos = 1;  NodeK_Bot = ElemD1.elnod[0]; }
		
	// M-Phi 沥焊 积己
	int i, j;
	_DGN_MPHI_STRN_D MPhiStrnData;  
	MPhiStrnData.Initialize();
	MPhiStrnData.bUserCurve = TRUE;
	if(RsdlData.PierMpccKey == 0)
	{
		MPhiStrnData.arUserMPhi[0].Copy(RsdlData.arPierUserMPhi[0]);
		MPhiStrnData.arUserMPhi[1].Copy(RsdlData.arPierUserMPhi[1]);		
	}
	else 
	{		
		T_MPCC_D MpccData;
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(RsdlData.PierMpccKey, MpccData)) { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
		CDgnMomentCurvatureDesign CalcMPhi;
		DGN_CALC_MPHI_RESULT ResultD[2];
		MpccData.dAxisAngle = 0.0;
		if(0 != CalcMPhi.CalcMomentCurvature(MpccData, ResultD[0])) { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }
		MpccData.dAxisAngle = -90.0;
		if(0 != CalcMPhi.CalcMomentCurvature(MpccData, ResultD[1])) { m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);  return FALSE; }

		int nSize_Pos, nSize_Neg;
		T_RSDL_MPHI_D MPhiD;

 if(MpccData.bIdealize)
        {
            MPhiStrnData.dpMom_I[3][0] = ResultD[0].MPhi_yI[0].dMoment;
            MPhiStrnData.dpMom_I[3][1] = ResultD[0].MPhi_yI[1].dMoment;
            MPhiStrnData.dpMom_J[3][0] = ResultD[1].MPhi_yI[0].dMoment;
            MPhiStrnData.dpMom_J[3][1] = ResultD[1].MPhi_yI[1].dMoment;
            
            MPhiStrnData.dpPhi_I[3][0] = ResultD[0].MPhi_yI[0].dCurvature;
            MPhiStrnData.dpPhi_I[3][1] = ResultD[0].MPhi_yI[1].dCurvature;
            MPhiStrnData.dpPhi_J[3][0] = ResultD[1].MPhi_yI[0].dCurvature;
            MPhiStrnData.dpPhi_J[3][1] = ResultD[1].MPhi_yI[1].dCurvature;

        }
        else
        {
            MPhiStrnData.dpMom_I[3][0] = ResultD[0].MPhi_u[0].dMoment;
            MPhiStrnData.dpMom_I[3][1] = ResultD[0].MPhi_u[1].dMoment;
            MPhiStrnData.dpMom_J[3][0] = ResultD[1].MPhi_u[0].dMoment;
            MPhiStrnData.dpMom_J[3][1] = ResultD[1].MPhi_u[1].dMoment;
            
            MPhiStrnData.dpPhi_I[3][0] = ResultD[0].MPhi_u[0].dCurvature;
            MPhiStrnData.dpPhi_I[3][1] = ResultD[0].MPhi_u[1].dCurvature;
            MPhiStrnData.dpPhi_J[3][0] = ResultD[1].MPhi_u[0].dCurvature;
            MPhiStrnData.dpPhi_J[3][1] = ResultD[1].MPhi_u[1].dCurvature;

        }


		for(i=0 ; i<2 ; i++)
		{
			nSize_Pos = ResultD[i].arCurvePos.GetSize();
			nSize_Neg = ResultD[i].arCurvePos.GetSize();

			if(!ResultD[i].bSymmetry || nSize_Neg < 2)
			{
				MPhiStrnData.arUserMPhi[i].SetSize(nSize_Pos+nSize_Neg-1);
				for(j=0 ; j<nSize_Neg-1 ; j++)
				{
					MPhiD.dCurvature = ResultD[i].arCurvePos[nSize_Neg-j-1].dCurvature;
					MPhiD.dMoment    = ResultD[i].arCurvePos[nSize_Neg-j-1].dMoment;
					MPhiStrnData.arUserMPhi[i].SetAt(j, MPhiD);
				}				
				for(j=0 ; j<nSize_Pos ; j++)
				{
					MPhiD.dCurvature = ResultD[i].arCurvePos[j].dCurvature;
					MPhiD.dMoment    = ResultD[i].arCurvePos[j].dMoment;
					MPhiStrnData.arUserMPhi[i].SetAt(nSize_Neg+j-1, MPhiD);
				}	
			}
			else 
			{
				MPhiStrnData.arUserMPhi[i].SetSize(nSize_Pos);
				for(j=0 ; j<nSize_Pos ; j++)
				{
					MPhiD.dCurvature = ResultD[i].arCurvePos[j].dCurvature;
					MPhiD.dMoment    = ResultD[i].arCurvePos[j].dMoment;
					MPhiStrnData.arUserMPhi[i].SetAt(j, MPhiD);
				}				
			}
		}
	}

	CArray<_DGN_MPHI_STRN_D, _DGN_MPHI_STRN_D&> arMPhiStrn;	
	arMPhiStrn.SetSize(nSizeElem);
	for(i=0 ; i<nSizeElem ; i++) arMPhiStrn.SetAt(i, MPhiStrnData);

	// Result 檬扁拳
	data.DataBase[0].iKey = data.DataBase[1].iKey = iMember;
	data.DataBase[0].iDirect = 0;
	data.DataBase[1].iDirect = 1;
	data.DataBase[0].LcomK = data.DataBase[1].LcomK = 0;
	data.DataBase[0].dHoriDisp = data.DataBase[1].dHoriDisp = 0.0;
	// 何犁仿 魂免
	
	CDgnToolMPhi_SeismicDesign CalcDisp;
	CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D> arMoment_yy;
	CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D> arMoment_zz;
	arMoment_yy.SetSize(nSizeElem);
	arMoment_zz.SetSize(nSizeElem);

	int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
	int iSerialLcom;
	_DGN_FORC_CRC ForceLcom;
	double dDispx, dDispy, dDispz, dRx, dRy, dRz;
	double dDis;
	BOOL bFirst_yy = TRUE, bFirst_zz = TRUE;
	BOOL bChk;
    T_RC_D_CH rcd;
    rcd.Initialize();
    if(!Get_Rcd(rcd))return FALSE;

	for(i=0 ; i<iLcomNum ; i++)
	{
		iSerialLcom = i+1;
		if(!m_ForceCtrl->IsEqLcom_JTG_B02(iSerialLcom)) continue;
        if(rcd.iDgnCode == RC_JTG_B02_01_2008)
        {
            bChk = TRUE;
            for(j=0 ; j<nSizeElem ; j++)
            {		
				auto EPairK = m_DataCtrl.GetElemPairKey4Elem(RsdlData.aElemList[j]);
                if(!m_pDesignForceResult->GetForceLcomResult(EPairK, iSerialLcom, ForceLcom)) { bChk = FALSE;  break; }
                GetEqForceExTempForce(RsdlData.aElemList[j], iSerialLcom, ForceLcom);
                arMoment_yy[j].dM[0] = ForceLcom.dMuy[0];
                arMoment_yy[j].dM[1] = (ForceLcom.dMuy[0] + ForceLcom.dMuy[1]) / 2.0;
                arMoment_yy[j].dM[2] = ForceLcom.dMuy[1];				
                arMoment_zz[j].dM[0] = ForceLcom.dMuz[0];
                arMoment_zz[j].dM[1] = (ForceLcom.dMuz[0] + ForceLcom.dMuz[1]) / 2.0;
                arMoment_zz[j].dM[2] = ForceLcom.dMuz[1];			
            }
            if(!bChk) continue;
            
            dRx = dRy = dRz = 0.0;
            if(!Get_DispQE2(NodeK_Bot, iSerialLcom, dDispx, dDispy, dDispz, dRx, dRy, dRz))
            { dRx = dRy = dRz = 0.0; }
            if(CalcDisp.CalcPhiDis(TRUE, RsdlData.aElemList, arMoment_yy, arMPhiStrn, dDis, nBasicPos, dRx, dRy, dRz))
            {
                if(bFirst_yy || (fabs(dDis) > fabs(data.DataBase[0].dHoriDisp))) 
                {
                    data.DataBase[0].LcomK     = iSerialLcom;
                    data.DataBase[0].dHoriDisp = dDis;
                    bFirst_yy = FALSE;
                }
            }
            
            if(CalcDisp.CalcPhiDis(FALSE, RsdlData.aElemList, arMoment_zz, arMPhiStrn, dDis, nBasicPos, dRx, dRy, dRz))
            {
                if(bFirst_zz || (fabs(dDis) > fabs(data.DataBase[1].dHoriDisp))) 
                {
                    data.DataBase[1].LcomK     = iSerialLcom;
                    data.DataBase[1].dHoriDisp = dDis;
                    bFirst_zz = FALSE;
                }
		}
        }
        else if(rcd.iDgnCode == RC_CJJ_166_2011)
        {
            _DGN_LCOM LcomD;
            if(!m_ForceCtrl->Get_DgnLcomData(iSerialLcom,LcomD))
                return FALSE;
            T_NODE_K PiertopNodeKey = 0;
            GetPierTopNodeID(RsdlData.aElemList,PiertopNodeKey);
            double dMajorAxisDisp = 0.0;
            double dUnMajorAxisDisp = 0.0;
            double dLocalVector[3][3];
            double dHorizonFactor = 1.0;

            dRx = dRy = dRz = 0.0;
            if(!Get_DispQE2(PiertopNodeKey, iSerialLcom, dDispx, dDispy, dDispz, dRx, dRy, dRz))
            { dRx = dRy = dRz = 0.0; }

            if(!CDBLib::GetSortElem_Position(RsdlData.aElemList)) return FALSE;

            m_pDoc->calcElemLocalVector(RsdlData.aElemList[RsdlData.aElemList.GetSize()-1], dLocalVector);

            CMathFunc::mathGCS2UCS(dRx, dRy, dRz, dLocalVector);
            
            dMajorAxisDisp= dDispx;
            
            dHorizonFactor = sqrt(dLocalVector[2][0]*dLocalVector[2][0] + dLocalVector[2][1]*dLocalVector[2][1]); 
            dMajorAxisDisp *= dHorizonFactor; 

            
            dUnMajorAxisDisp=dDispy ;
            
            dHorizonFactor = sqrt(dLocalVector[1][0]*dLocalVector[1][0] + dLocalVector[1][1]*dLocalVector[1][1]); 
            dUnMajorAxisDisp *= dHorizonFactor; 
            
            if(fabs(dMajorAxisDisp) > fabs(data.DataBase[0].dHoriDisp))
            {
                data.DataBase[0].dHoriDisp = fabs(dMajorAxisDisp);
                bFirst_yy = FALSE;
            }
            if(fabs(dUnMajorAxisDisp) > fabs(data.DataBase[1].dHoriDisp))
            {
                data.DataBase[1].dHoriDisp = fabs(dUnMajorAxisDisp);
                bFirst_zz = FALSE;
            }
        }
		
	}
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);

	CUnitCtrl::ConvValueTgtUnit2CurUnit(m_CodeUnit.nBase_Force, m_CodeUnit.nBase_Length, D_UNITSYS_BASE_LENGTH, data.DataBase[0].dHoriDisp);
	CUnitCtrl::ConvValueTgtUnit2CurUnit(m_CodeUnit.nBase_Force, m_CodeUnit.nBase_Length, D_UNITSYS_BASE_LENGTH, data.DataBase[1].dHoriDisp);

    return ((!bFirst_yy) && (!bFirst_zz));
}

//--------------------------Get Pier's other data ------------------------------------------------------------------------
BOOL CRCDataCtrl_CH::GetMfromPierPM(T_RSDL_K iMember, BOOL bTop, BOOL bLongitudinal, double dP, double& dM)
{
	dM = 0.0;
	_CVL_CH_RC_COLUMN_JTG_W InData;   InData.Initialize();
	_CVL_CH_RC_COL_RES_W    OutData;  OutData.Initialize();

	T_ELEM_K elemk[2];
	int ij[2];
	if(!GetElemkijFromPierMemberID(iMember, elemk, ij)) return FALSE;
	T_ELEM_K trgElemK = bTop ? elemk[0] : elemk[1];
	int nIMJ = ( bTop ? ij[0]==0 :  ij[1]==0) ? 0 : 2;
	T_ELEM_D ElemD;  ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(trgElemK, ElemD)) return FALSE;
 	T_SECT_D sData;	sData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetSectDesign(ElemD.elpro, sData)) return FALSE;
	T_RCHK_D RchkD;  RchkD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetRchk(ElemD.elpro, RchkD)) return FALSE;
	T_RSDL_D RsdlD;  RsdlD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, RsdlD)) return FALSE;
	if(!CDBLib::GetSortElem_Position(RsdlD.aElemList)) return FALSE;
	int nElemSize = RsdlD.aElemList.GetSize();
	if(nElemSize ==0) return FALSE;
	
	CDgn_RCManager DllManager;

	// 
	InData.iDgnCode = DllManager.ConvertToDLL_Code(RC_JTG_B02_01_2008);
	InData.iCheck_PnMn_Type = 1;
	InData.bReduceRbar = TRUE;
	InData.iTypeStressStrain = 0;
	InData.iSliceNum = 50;
	InData.iPmDivNum = 24;             // Pm惑包档 积己矫 谅钎狼 荐(积己矫俊父 利侩)
	InData.iDivisionNumber_90deg = 6; // Pm惑包档 积己矫 90狼 盒且荐(积己矫俊父 利侩)
	InData.dTolAngle = 0.1;             // Tolerance for Angle (沥剐 PM-Curve 魂免矫 荐放坷瞒 default=0.1ⅹ)
	InData.dTolM0 = 0.001;                // 弥措 葛膏飘 魂免矫狼 牢立 吝赋绵困摹 魂免棺 葛膏飘 坷瞒厚(葛膏飘 坷瞒 = m_dTolM0*弥措葛膏飘). (default=0.001)  
	InData.dTolP0 = 0.001;                // 绵窍吝 坷瞒厚(绵窍吝 坷瞒 = m_dTolP0*弥措绵窍吝). (default=0.001)  
	InData.bFindMmax = FALSE;             // 闭屈颇鲍矫甫 Mmax瘤痢(惑技拌魂)阑 荤侩且瘤 Get_BalancedXb肺 拌魂等 吝赋绵困摹甫 荤侩且瘤 咯何(DB窜搁篮 PM谅钎吝 弥措蔼(埃帆拌魂)阑 扁霖栏肺 Phi蔼 魂免, Gen窜搁篮 m_bFindMmax俊 狼秦 搬沥等 闭屈碍档蔼栏肺 Phi蔼 魂免)
	InData.iIterNum  = 30;              // 矫青馒坷过 拌魂矫 馆汗茄拌蔼
	InData.dDcDgn = 0.0; // Design矫 乔汗滴膊

	if(!DllManager.ConvertToDLL_SectData(sData, nIMJ, InData.Sect)) return FALSE;
	if(!DllManager.ConvertToDLL_RebarData(sData, nIMJ, RchkD.COLM, InData.Rebar)) return FALSE;

	// Check!!! 咯扁辑 何磐 09.12.08

	_DGN_RC_MATL_CH matl;		matl.Initialize();
	if (!Get_Matl(trgElemK,matl)) return FALSE;
	InData.MatlD.dFck = matl.dfck1;
	InData.MatlD.dFy  = matl.dfsk;
	InData.MatlD.dFys = matl.dfsky;
	InData.MatlD.dEc  = matl.dEc;
	InData.MatlD.dEs  = matl.dEs;
	InData.MatlD.dEsv = matl.dEsy;
	InData.MatlD.dGc  = 1.0;
	InData.MatlD.dGs  = 1.0;
	InData.MatlD.dGsv = 1.0;

	
	InData.dfcd       = 0.0; // Auto Caluration
	InData.dfsd_Tens  = 0.0; // Auto Caluration
	InData.dfsd_Comp  = 0.0; // Auto Caluration
	InData.dPhi_JTG   = 0.0; // Auto Caluration
	
	int nSType;
	T_SECT_SECTBASE_D SectBaseD = DllManager.GetSectBaseD(sData, nSType, nIMJ, TRUE);
	
	InData.dKy = 2.0; // Cantilever
	InData.dKz = 2.0; // Cantilever
	InData.dLy = Get_ElemListLength(RsdlD.aElemList);
	InData.dLz = Get_ElemListLength(RsdlD.aElemList);
	InData.dRoy = SectBaseD.Design.Roy;
	InData.dRoz = SectBaseD.Design.Roz;
	
	InData.arLoad.SetSize(1);
	InData.arLoad[0].Initialize();
	InData.arLoad[0].bUse = TRUE;
	InData.arLoad[0].dP = (-1.0)*dP;
	InData.arLoad[0].dM = 1.0;
	if(bLongitudinal)	InData.arLoad[0].dMy = 1.0;
	else              InData.arLoad[0].dMz = 1.0;


	// P-M Calculation
	if(!DllManager.Calc_RC_Column_Execute_Chk(InData, OutData)) return FALSE;
	if(OutData.arPmmRes.GetSize() != 1) return FALSE;

	if(bLongitudinal)	dM = OutData.arPmmRes[0].dPhiMny;
	else              dM = OutData.arPmmRes[0].dPhiMnz;

    return TRUE;
}

BOOL CRCDataCtrl_CH::GetEndForcefromPierMember(T_RSDL_K iMember, CArray <_DGN_FORC_CRC,_DGN_FORC_CRC>& aForceList)
{
    aForceList.RemoveAll();
    T_ELEM_K ElemK[2];
	int IJ[2];
    if(!GetElemkijFromPierMemberID(iMember, ElemK, IJ)) return FALSE;
    if(IJ[0]>1 || IJ[1]>1 || IJ[0]<0 || IJ[1]<0) return FALSE;

	auto ElemIPairK = m_DataCtrl.GetElemPairKey4Elem(ElemK[0]);
	auto ElemJPairK = m_DataCtrl.GetElemPairKey4Elem(ElemK[1]);
    
    BOOL bEqLcom = FALSE;
    int iLcomNum=m_DataCtrl.Get_DgnLcomNum();
 	ADGNFORCE aForceLcomI;
    ADGNFORCE aForceLcomJ;
    if(!m_ForceCtrl->Get_LcomDataForDesign(ElemIPairK, 0,TRUE, FALSE, aForceLcomI, iLcomNum))	return FALSE;
    if(!m_ForceCtrl->Get_LcomDataForDesign(ElemJPairK, 0,TRUE, FALSE, aForceLcomJ, iLcomNum))	return FALSE;
    int iMax = min(aForceLcomI.GetSize(),aForceLcomJ.GetSize());
    for(int i=0; i<iMax; i++)	
    {
        if (!m_ForceCtrl->IsEqLcom_JTG_B02(i+1)) continue;
        bEqLcom = TRUE;
        _DGN_FORC_CRC EqForceI,EqForceJ,EqForce;
        m_pDesignForceResult->GetForceLcomResult(ElemIPairK,i+1,EqForceI);
        m_pDesignForceResult->GetForceLcomResult(ElemJPairK,i+1,EqForceJ);
        GetEqForceExTempForce(ElemK[0],i+1,EqForceI);
        GetEqForceExTempForce(ElemK[1],i+1,EqForceJ);
        EqForce.Initialize();    
        EqForce.iRefNo[0] = EqForce.iRefNo[1] = i+1;
        EqForce.dFxx[0] = Cng_ForceFromCurrToCode( EqForceI.dFxx[IJ[0]]); 
        EqForce.dFyy[0] = Cng_ForceFromCurrToCode( EqForceI.dFyy[IJ[0]]); 
        EqForce.dFzz[0] = Cng_ForceFromCurrToCode( EqForceI.dFzz[IJ[0]]); 
        EqForce.dMux[0] = Cng_MomentFromCurrToCode(EqForceI.dMux[IJ[0]]); 
        EqForce.dMuy[0] = Cng_MomentFromCurrToCode(EqForceI.dMuy[IJ[0]]); 
        EqForce.dMuz[0] = Cng_MomentFromCurrToCode(EqForceI.dMuz[IJ[0]]); 
        EqForce.dFxx[1] = Cng_ForceFromCurrToCode( EqForceJ.dFxx[IJ[1]]);
        EqForce.dFyy[1] = Cng_ForceFromCurrToCode( EqForceJ.dFyy[IJ[1]]);
        EqForce.dFzz[1] = Cng_ForceFromCurrToCode( EqForceJ.dFzz[IJ[1]]);
        EqForce.dMux[1] = Cng_MomentFromCurrToCode(EqForceJ.dMux[IJ[1]]);
        EqForce.dMuy[1] = Cng_MomentFromCurrToCode(EqForceJ.dMuy[IJ[1]]);
        EqForce.dMuz[1] = Cng_MomentFromCurrToCode(EqForceJ.dMuz[IJ[1]]);
        aForceList.Add(EqForce);
    }
    return TRUE;
}

BOOL CRCDataCtrl_CH::Get_RsdlCurvature(T_RSDL_K iMember, double dPhiy[2], double dPhiu[2])
{
	dPhiy[0] = dPhiy[1] = dPhiu[0] = dPhiy[1] = 0.0;

	T_RSDL_D RsdlData;	
	if(!m_pDoc->m_pAttrCtrl->GetRsdl(iMember, RsdlData))  { return FALSE; }
	
	if(RsdlData.PierMpccKey == 0)
	{
		dPhiy[0] = RsdlData.dPierYieldCurvature[0];
		dPhiy[1] = RsdlData.dPierYieldCurvature[1];
		dPhiu[0] = RsdlData.dPierUltimateCurvature[0];
		dPhiu[1] = RsdlData.dPierUltimateCurvature[1];
	}
	else 
	{		
		T_MPCC_D MpccData;
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(RsdlData.PierMpccKey, MpccData)) { return FALSE; }
		CDgnMomentCurvatureDesign CalcMPhi;
		DGN_CALC_MPHI_RESULT ResultD[2];
		if(!MpccData.bIdealize)
		{
			MpccData.bIdealize = TRUE;
			MpccData.bCurvature = FALSE;
		}
		MpccData.dAxisAngle = 0.0;
		if(0 != CalcMPhi.CalcMomentCurvature(MpccData, ResultD[0])) { return FALSE; }
		MpccData.dAxisAngle = -90.0;
		if(0 != CalcMPhi.CalcMomentCurvature(MpccData, ResultD[1])) { return FALSE; }
		
		dPhiy[0] = (ResultD[0].bSymmetry ? ResultD[0].MPhi_yI[0].dCurvature : max(fabs(ResultD[0].MPhi_yI[0].dCurvature), fabs(ResultD[0].MPhi_yI[1].dCurvature)));
		dPhiy[1] = (ResultD[1].bSymmetry ? ResultD[1].MPhi_yI[0].dCurvature : max(fabs(ResultD[1].MPhi_yI[0].dCurvature), fabs(ResultD[1].MPhi_yI[1].dCurvature)));
		dPhiu[0] = (ResultD[0].bSymmetry ? ResultD[0].MPhi_u[0].dCurvature  : max(fabs(ResultD[0].MPhi_u[0].dCurvature),  fabs(ResultD[0].MPhi_u[1].dCurvature) ));
		dPhiu[1] = (ResultD[1].bSymmetry ? ResultD[1].MPhi_u[0].dCurvature  : max(fabs(ResultD[1].MPhi_u[0].dCurvature),  fabs(ResultD[1].MPhi_u[1].dCurvature) ));
	}

	return TRUE;
}

double CRCDataCtrl_CH::Get_ElemLength(T_ELEM_K ElemK)
{	
	double dLength=0.0;
    T_ELEM_D ElemD;
    ElemD.Initialize();
	m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD);
	// 1st node.
	T_NODE_D NodeD1; NodeD1.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0],NodeD1);
	// 2nd node.
	T_NODE_D NodeD2; NodeD2.Initialize();
	m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1],NodeD2);
	
	dLength = sqrt(pow(NodeD1.x-NodeD2.x,2)+pow(NodeD1.y-NodeD2.y,2)+pow(NodeD1.z-NodeD2.z,2));
	
	return dLength;
}

double CRCDataCtrl_CH::Get_ElemListLength(CArray<T_ELEM_K, T_ELEM_K>& arElemK)
{
	double dLength=0.0;
	int nSize = arElemK.GetSize();
	for(int i=0 ; i<nSize ; i++)
		dLength += Get_ElemLength(arElemK[0]);

	return dLength;
}

BOOL CRCDataCtrl_CH::Get_ElemPosition(CArray<T_ELEM_K, T_ELEM_K>& arSortElemK, double dL, BOOL bStartI, T_ELEM_K& PosElemK, double& dRate)
{
	PosElemK = 0;
	int i, nSize;
	nSize = arSortElemK.GetSize();
	if(nSize == 0) return FALSE;
	double dLength;
	if(bStartI)
	{
		for(i=0 ; i<nSize ; i++)
		{
			dLength = Get_ElemLength(arSortElemK[i]);
			if(dLength >= dL)
			{
				PosElemK = arSortElemK[i];
				dRate = dLength==0.0 ? 1.0 : (dLength-dL)/dLength;
			}
			else if(i==nSize-1)
			{
				PosElemK = arSortElemK[i];
				dRate = 0.0;
			}
			else 
				dL -= dLength;
		}
	}
	else 
	{
		for(i=nSize-1 ; i>=0 ; i--)
		{
			dLength = Get_ElemLength(arSortElemK[i]);
			if(dLength >= dL)
			{
				PosElemK = arSortElemK[i];
				dRate = dLength==0.0 ? 0.0 : 1.0-(dLength-dL)/dLength;
			}
			else if(i==0)
			{
				PosElemK = arSortElemK[i];
				dRate = 1.0;
			}
			else 
				dL -= dLength;
		}
	}
	return (PosElemK != 0);
}

BOOL CRCDataCtrl_CH::Get_Theta(T_THIS_K ThisK, T_NODE_K NodeKI1, T_NODE_K NodeKJ1, double dRate1, T_NODE_K NodeKI2, T_NODE_K NodeKJ2, double dRate2, double dMaxTheta[2], int nMaxTimeIndex[2])
{
	dMaxTheta[0] = dMaxTheta[1] = 0.0;
	nMaxTimeIndex[0] = nMaxTimeIndex[1] = 0;

	if(!m_pDoc->m_pAttrCtrl->ExistThis(ThisK)) return FALSE;
	CArray<double, double> arTimeStep;
	m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, arTimeStep);
	int nStepSize = arTimeStep.GetSize();
	if(nStepSize == 0) return FALSE;

	//T_LCOM_D rLoadComb;
	//m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, ThisK, rLoadComb);
	//m_pDoc->m_pPostCtrl->SelectLoadCombNew(rLoadComb,0,0);

	m_pDoc->m_pPostCtrl->SelectThis(ThisK);
	T_DISP_D DispDI1,DispDI2,DispDJ1,DispDJ2;
	double dTheta[2];
	for(int i=0 ; i<nStepSize ; i++)
	{
		DispDI1.Initialize();  DispDI2.Initialize();  DispDJ1.Initialize();  DispDJ2.Initialize();
		if(dRate1 != 0.0) { if(!m_pDoc->m_pPostCtrl->GetDispHistory(i, NodeKI1, DispDI1,3)) continue; }
		if(dRate1 != 1.0) { if(!m_pDoc->m_pPostCtrl->GetDispHistory(i, NodeKJ1, DispDJ1,3)) continue; }
		if(dRate2 != 0.0) { if(!m_pDoc->m_pPostCtrl->GetDispHistory(i, NodeKI2, DispDI2,3)) continue; }
		if(dRate2 != 1.0) { if(!m_pDoc->m_pPostCtrl->GetDispHistory(i, NodeKJ2, DispDJ2,3)) continue; }

		dTheta[0] = (DispDI2.dblDisp[4]*dRate2 + DispDJ2.dblDisp[4]*(1.0-dRate2)) - (DispDI1.dblDisp[4]*dRate1 + DispDJ1.dblDisp[4]*(1.0-dRate1));
		dTheta[1] = (DispDI2.dblDisp[5]*dRate2 + DispDJ2.dblDisp[5]*(1.0-dRate2)) - (DispDI1.dblDisp[5]*dRate1 + DispDJ1.dblDisp[5]*(1.0-dRate1));
		if(fabs(dMaxTheta[0]) < fabs(dTheta[0]))
		{ dMaxTheta[0] = dTheta[0];  nMaxTimeIndex[0] = i; }
		if(fabs(dMaxTheta[1]) < fabs(dTheta[1]))
		{ dMaxTheta[1] = dTheta[1];  nMaxTimeIndex[1] = i; }
	}

	return TRUE;
}

BOOL CRCDataCtrl_CH::Get_Theta(T_THIS_K ThisK, CArray<T_ELEM_K, T_ELEM_K>& arElemK, double dRateS, double dRateE, double dMaxTheta[2], int nMaxTimeIndex[2])
{
	dMaxTheta[0] = dMaxTheta[1] = 0.0;
	nMaxTimeIndex[0] = nMaxTimeIndex[1] = 0;

	if(!m_pDoc->m_pAttrCtrl->ExistThis(ThisK)) return FALSE;
	CArray<double, double> arTimeStep;
	m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, arTimeStep);
	int nStepSize = arTimeStep.GetSize();
	if(nStepSize == 0) return FALSE;
	int nElemSize = arElemK.GetSize();
	if(nElemSize == 0) return FALSE;

	m_pDoc->m_pPostCtrl->SelectThis(ThisK);
	double dTheta[2];
	T_ELEM_K ElemK;	
	int i, j, nIndex;
    T_INELASTICDYN_HINGE_INFO_BASE *pHngeInfoBase_Ry;
	T_INELASTICDYN_HINGE_INFO_BASE *pHngeInfoBase_Rz;
    T_INELASTICDYN_HINGE_INFO InelHngeInfo;
	T_DGN_DUTL_D DutlD_Ry, DutlD_Rz;
	pHngeInfoBase_Ry = pHngeInfoBase_Rz = NULL;
	for(i=0 ; i<nStepSize ; i++)
	{
		dTheta[0] = dTheta[1] = 0.0;
		for(j=0 ; j<nElemSize ; j++)
		{
			ElemK = arElemK.GetAt(j);
			if(!m_pDoc->m_pPostCtrl->GetThisInfo()->GetInelasticDynHngtDataElem(ElemK,InelHngeInfo)) continue; 
			if(!GetHngeInfoBaseDataByComp(4/*RY*/,nIndex,InelHngeInfo.aInelasticDynHingeInfo)) continue; 
			if(nIndex<0) continue; 			
			pHngeInfoBase_Ry = &(InelHngeInfo.aInelasticDynHingeInfo[nIndex]);
			if(!GetHngeInfoBaseDataByComp(5/*RZ*/,nIndex,InelHngeInfo.aInelasticDynHingeInfo)) continue; 
			if(nIndex<0) continue; 			
			pHngeInfoBase_Rz = &(InelHngeInfo.aInelasticDynHingeInfo[nIndex]);

			if(!GetInElasticData(DutlD_Ry,pHngeInfoBase_Ry, i, 4/*RY*/)) continue; 
			if(!GetInElasticData(DutlD_Rz,pHngeInfoBase_Rz, i, 5/*RY*/)) continue; 

			if(nElemSize == 1)
			{// Elem啊 1俺牢 版快
				dTheta[0] += GetInElasticTheta(DutlD_Ry, dRateS, dRateE);
				dTheta[1] += GetInElasticTheta(DutlD_Rz, dRateS, dRateE);
			}
			else if(j==0)
			{// I窜牢 版快				
				dTheta[0] += GetInElasticTheta(DutlD_Ry, dRateS, 0.0);
				dTheta[1] += GetInElasticTheta(DutlD_Rz, dRateS, 0.0);
			}
			else if(j==nElemSize-1)
			{// J窜牢 版快			
				dTheta[0] += GetInElasticTheta(DutlD_Ry, 1.0, dRateE);
				dTheta[1] += GetInElasticTheta(DutlD_Rz, 1.0, dRateE);
			}
			else 
			{
				dTheta[0] += GetInElasticTheta(DutlD_Ry, 1.0, 0.0);
				dTheta[1] += GetInElasticTheta(DutlD_Rz, 1.0, 0.0);
			}
		}

		if(fabs(dMaxTheta[0]) < fabs(dTheta[0]))
		{ dMaxTheta[0] = dTheta[0];  nMaxTimeIndex[0] = i; }
		if(fabs(dMaxTheta[1]) < fabs(dTheta[1]))
		{ dMaxTheta[1] = dTheta[1];  nMaxTimeIndex[1] = i; }
	}

	return TRUE;
}

BOOL CRCDataCtrl_CH::GetHngeInfoBaseDataByComp(int nComp,int &nIndex,CArray<T_INELASTICDYN_HINGE_INFO_BASE,T_INELASTICDYN_HINGE_INFO_BASE &> &HngeInfo)
{
    int i;
    nIndex = -1;
    BOOL bExist=FALSE; 
    for(i=0 ; i<HngeInfo.GetSize(); i++)
    if(nComp==HngeInfo[i].nHingeDirection)
    {
        bExist=TRUE; 
        nIndex = i;
        break; 
    }
    return bExist;
}

BOOL CRCDataCtrl_CH::GetInElasticData(T_DGN_DUTL_D &DutlD,T_INELASTICDYN_HINGE_INFO_BASE *pHngeInfoBase, int nTimeIndex, int nComp)
{
    DutlD.Initialize(); 

	if(pHngeInfoBase->nHingeType != 0) return FALSE;

	DutlD.nPosType = pHngeInfoBase->nHingeLocation;
    switch(pHngeInfoBase->nHingeLocation)
    {
    case 0:// I pos  
    case 1:// J pos   
    case 3:// Center
        DutlD.arValue.SetSize(1);
        DutlD.arValue[0] = GetInElasticValue(1,nTimeIndex,pHngeInfoBase->nStartHingeId,nComp,0);
        break; 
    case 2:// I-J pos  
        DutlD.arValue.SetSize(2);
        DutlD.arValue[0] = GetInElasticValue(1,nTimeIndex,pHngeInfoBase->nStartHingeId,nComp,0);
        DutlD.arValue[1] = GetInElasticValue(1,nTimeIndex,pHngeInfoBase->nStartHingeId+1,nComp,0);
        break;   
    }
	return TRUE;
}


double CRCDataCtrl_CH::GetInElasticValue(int nHingeType,int nTimeIndex,int nInElasticSeqId, int nComp,int nHngeValType)
{
    BOOL    bRotateDof;
    double  dValue;
    T_NLHG_D NlhgD; 

    if(3/*RX*/<=nComp) bRotateDof=TRUE; 
    else                           bRotateDof=FALSE; 

	if(!m_pDoc->m_pPostCtrl->GetNlhgHistory(nTimeIndex,nInElasticSeqId,nHingeType,bRotateDof,NlhgD,nComp))
	{  
		ASSERT(0);  
		return 0.0; 
	}
	//--------------------------------------------
    //Output Type : INELAST_TYPE_DEFORM
    //--------------------------------------------
    double dPosVal = NlhgD.dDisplacement;
    double dNegVal = NlhgD.dDisplacement;
    
    //--------------------------------------------
    // Value Type
    //--------------------------------------------
    switch(nHngeValType)
    {
    case 0: return dPosVal;
    case 1: return dNegVal;
    case 2: (fabs(dPosVal)>fabs(dNegVal)) ? dValue=dPosVal: dValue=dNegVal;   return dValue;
    }
    return 0.0; 
}

double CRCDataCtrl_CH::GetInElasticTheta(T_DGN_DUTL_D &DutlD, double dRateS, double dRateE)
{
	int nSize = DutlD.arValue.GetSize();	
	if((DutlD.nPosType==0 || DutlD.nPosType==1 || DutlD.nPosType==3) && nSize==1)
	{
		double dRateL = max(0.0, dRateS-dRateE);
		return dRateL*DutlD.arValue[0];
	}
	else if(DutlD.nPosType==2 && nSize==2)
	{
		double dRateL_I = max(0.0, max(0.0, 2.0*(dRateS-0.5)) - max(0.0, 2.0*(dRateE-0.5)));
		double dRateL_J = max(0.0, min(1.0, 2.0*dRateS) - min(1.0, 2.0*dRateE));		
		return dRateL_I*DutlD.arValue[0] + dRateL_J*DutlD.arValue[1];
	}
	return 0.0;
}

BOOL CRCDataCtrl_CH::Get_DispQE2(T_NODE_K NodeK, UINT iSerialLcom, double& dDispX, double& dDispY, double& dDispZ, double& dRX, double& dRY, double& dRZ)
{
	dDispX = dDispY = dDispZ = dRX = dRY = dRZ = 0.0;
	_DGN_LCOM LcomDesign;
	if(!m_ForceCtrl->Get_DgnLcomData(iSerialLcom, LcomDesign)) return FALSE;

	T_LCOM_D LcomData;  LcomData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomDesign.OriginalLcomKey, LcomData)) return FALSE;
	
	T_DISP_D DispIMax, DispIMin;	
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);
	m_pDoc->m_pPostCtrl->GetDispNew(NodeK, &DispIMax, &DispIMin, NULL);
	dDispX = DispIMax.dblDisp[0];
	dDispY = DispIMax.dblDisp[1];
	dDispZ = DispIMax.dblDisp[2];
	dRX    = DispIMax.dblDisp[3];
	dRY    = DispIMax.dblDisp[4];
	dRZ    = DispIMax.dblDisp[5];

	return TRUE;
}
//Get T and Tg
BOOL CRCDataCtrl_CH::Get_TTg(double& dT, double& dTg)
{  
	dT = dTg = 0.0;

	// T魂免
	BOOL bChk = TRUE;
	T_FREQ_D data;
	if(!m_pDoc->m_pPostCtrl->GetEiFreq(1, data)) bChk = FALSE;
	else                                         dT = data.dblFreq[2];

	// Tg魂免 : 荤侩登绰 SPFC 吝 JTJXXX_2008扁霖阑 荤侩茄 霉锅掳 SPFC狼 Tg
	T_SPFC_D SpfcD;
	CArray<T_SPFC_K, T_SPFC_K> arSpfcK;    arSpfcK.RemoveAll();
	CArray<T_SPFC_K, T_SPFC_K> arSpfcKOk;  arSpfcKOk.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSpfcKeyList(arSpfcK);
	int i, j, k, nSize, nSpfcSize, nSpclSize;

	nSpfcSize = arSpfcK.GetSize();
	for(i=0 ; i<nSpfcSize ; i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetSpfc(arSpfcK[i], SpfcD)) continue;
		if(SpfcD.nSpecCodeType == D_SPFC_CODE_JTJXXX_2008||SpfcD.nSpecCodeType == D_SPFC_CODE_CJJ166_2011)
            arSpfcKOk.Add(arSpfcK[i]);
	}
	
	T_SPLC_D SplcD;
	CArray<T_SPLC_K, T_SPLC_K> arSplcK;    arSplcK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(arSplcK);

	nSpfcSize = arSpfcKOk.GetSize();
	nSpclSize = arSplcK.GetSize();
	for(i=0 ; i<nSpfcSize ; i++)
	{
		for(j=0 ; j<nSpclSize ; j++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSplc(arSplcK[j], SplcD)) continue;
			nSize = SplcD.aSpfcKey.GetSize();
			for(k=0 ; k<nSize ; k++)
			{
				if(arSpfcKOk[i] == SplcD.aSpfcKey[k])
				{ 
					if(!m_pDoc->m_pAttrCtrl->GetSpfc(arSpfcKOk[i], SpfcD)) continue;
                    if(SpfcD.nSpecCodeType == D_SPFC_CODE_JTJXXX_2008)
                    {
					   dTg = SpfcD.CodeParam.JTJXXX_2008.dTg;
                    }
                    else  if(SpfcD.nSpecCodeType == D_SPFC_CODE_CJJ166_2011)
                    {
                         dTg = SpfcD.CodeParam.CJJ166_2011.dTg;
                    }
                    else
                        ASSERT(0);

					return bChk;
				}
			}
		}
	}
	return FALSE;
}

BOOL CRCDataCtrl_CH::ConvertRPSCByPosition( const T_RPSC_D& RpscD,BOOL bEndI,CArray<T_RPSC_MBAR,T_RPSC_MBAR>& rBars )
{
    rBars.RemoveAll();
    std::map<_RBAR_KEY,T_RPSC_MBAR> posRebars;
    int iPosiM = (RpscD.bMbarJ && !bEndI ? 1 : 0);
    int iMbarSize = RpscD.Mbar[iPosiM].GetSize();
    _RBAR_KEY rbarkey;
    for(int i=0; i<iMbarSize; i++)
    {
		T_RPSC_MBAR RpscMbar = RpscD.Mbar[iPosiM].GetAt(i);
        rbarkey.m_strBarName = RpscMbar.strBarNa;
        rbarkey.m_iStaZRef = RpscMbar.iStaZRef;
        rbarkey.m_dStaZ = RpscMbar.dStaZ;

        std::map<_RBAR_KEY,T_RPSC_MBAR>::iterator pfind = posRebars.find(rbarkey);
        if(pfind !=posRebars.end())
        {
             pfind->second.iBarNum +=  RpscMbar.iBarNum;
        }
        else
        {
            posRebars.insert(std::make_pair(rbarkey,RpscMbar));
        }

    }

    for (std::map<_RBAR_KEY,T_RPSC_MBAR>::iterator pit = posRebars.begin();pit != posRebars.end();++pit )
    {
        rBars.Add(pit->second);
    }

    return TRUE;
}

void CRCDataCtrl_CH::GetEqForceExTempForce(int iElemK,int iLCombID,_DGN_FORC_CRC& EqForce)
{
    if (!m_ForceCtrl->IsEqLcom_JTG_B02(iLCombID)) return;
	
    T_RC_D_CH rcd;  rcd.Initialize();
    Get_Rcd(rcd);
    if(rcd.iDgnCode == RC_CJJ_166_2011)//扣除偶然组合中的温度效应荷载
    {
        _DGN_LCOM LcomD;
        if(!m_ForceCtrl->Get_DgnLcomData(iLCombID,LcomD))
            return;
        for (int j=0;j<LcomD.LcomUlData.Combination.GetSize();j++)
        {
            T_LCOM_BASE caseinfo = LcomD.LcomUlData.Combination.GetAt(j);
            if(caseinfo.AnalType == D_LOADCASE_STATIC)
            {
                T_STLD_D stldD;
                if(!m_pDoc->m_pAttrCtrl->GetStld(caseinfo.LoadCaseKey, stldD))
                {
                    continue;
                }
                if(!CDBLib::IsStldTL(stldD.LoadCaseType))
                    continue;
                
                T_LCOM_D LcasD;
                LcasD.Initialize();
                if(!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STATIC, caseinfo.LoadCaseKey, LcasD))	{ASSERT(0); return ;}
                else						m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcasD);
                
                T_STRB_D t_strb;
                m_pDoc->m_pPostCtrl->GetStrbNew(iElemK, 0, 0, &t_strb, TRUE);
                
                for(int j=0; j<2; j++)
                {	// I,J.				
                    
                    EqForce.dFxx[j] -=  caseinfo.Factor * (t_strb.dblForce[j==0 ? 0 : 3][j==0 ? 0 : 6]);
                    EqForce.dFyy[j] -=  caseinfo.Factor * (t_strb.dblForce[j==0 ? 0 : 3][j==0 ? 1 : 7]);
                    EqForce.dFzz[j] -=  caseinfo.Factor * (t_strb.dblForce[j==0 ? 0 : 3][j==0 ? 2 : 8]);
                    EqForce.dMux[j] -=  caseinfo.Factor * (t_strb.dblForce[j==0 ? 0 : 3][j==0 ? 3 : 9]);
                    EqForce.dMuy[j] -=  caseinfo.Factor * (t_strb.dblForce[j==0 ? 0 : 3][j==0 ? 4 : 10]);
                    EqForce.dMuz[j] -=  caseinfo.Factor * (t_strb.dblForce[j==0 ? 0 : 3][j==0 ? 5 : 11]);
                    
                }
            }
        }
    }
}

BOOL CRCDataCtrl_CH::GetPierTopNodeID(const CArray<T_ELEM_K, T_ELEM_K>& aElemList,T_NODE_K& nodeKey )
{
    nodeKey = 0;
    if(aElemList.GetSize() == 0)
        return FALSE;
    double dPierTopZ = -1.0E+50;
    T_ELEM_D ElemD1;
    ElemD1.Initialize();
    T_NODE_D NodeD1;
    NodeD1.Initialize();
    T_NODE_D NodeD2;
    NodeD2.Initialize();

    for(int i=0;i<aElemList.GetSize();i++)
    {
        if(!m_pDoc->m_pAttrCtrl->GetElem(aElemList.GetAt(i),ElemD1))           { return FALSE; }
        if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD1.elnod[0], NodeD1))                { return FALSE; }
        if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD1.elnod[1], NodeD2))                { return FALSE; }
        
        if(NodeD1.z > dPierTopZ)
        {
             nodeKey = ElemD1.elnod[0];
             dPierTopZ = NodeD1.z;
        }
        if(NodeD2.z > dPierTopZ)
        {
            nodeKey = ElemD1.elnod[1];
            dPierTopZ = NodeD2.z;
        }
    }
    return TRUE;
}
