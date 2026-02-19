// DgnBridgeLoadRating.cpp: implementation of the CDgnBridgeLoadRating class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnBridgeLoadRating.h"

#include "CRCForceCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"

// For Debugging.
#include "..\MIT_Lib\rptostream.h"
using namespace std;
#include "..\wg_db\FileCtrl.h"


//////////////////////////////////////////////////////////////////////
// _DBLR_FLEX_DATA
//////////////////////////////////////////////////////////////////////

void _DBLR_FLEX_DATA::Initialize()
{
	ElemKey.first   = -1;
	iPosi     = 0;
	bPositive = TRUE;

	bStrainCompatibility=TRUE;        
	dMn       = dphiMn      = dphi        = 0.0;                      
	dTp       = dTp_bond    = dTp_unbond  = 0.0;  
	dTst      = dCsc        = dCc         = 0.0;             
	dfps      = dfps_bond   = dfps_unbond = 0.0;
	dfst      = dfsc        = dfcc        = 0.0;
	ddc       = ddp         = ddp_bond    = ddp_unbond = ddst = ddsc = 0.0;   
	dAp       = dAp_bond    = dAp_unbond  = 0.0;   
	dAst      = dAsc        = dAc         = 0.0;             
	dNeuC     = dde         = ddv         = 0.0;
	iNum_bond = iNum_unbond = 0;      
	bOneType  = TRUE;  
	bBondType = TRUE;    
	iCount    = 0;
	dH        = 0.0;

	dMcr      = 0.0;	    
	dfr       = dfcpe       = 0.0; 
	dSc       = 0.0;


	dfck      = dfy         = dfpy        = dfpu          = 0.0;
	dB_flange = dB_web      = dThick_Top  = dThick_Bottom = 0.0;
	dfst      = dfsc        = 0.0;         
}   
_DBLR_FLEX_DATA& _DBLR_FLEX_DATA::operator = (_DBLR_FLEX_DATA& rData)
{
	Initialize();
	ElemKey       = rData.ElemKey    ;
	iPosi         = rData.iPosi      ;
	bPositive     = rData.bPositive  ;

	bStrainCompatibility = rData.bStrainCompatibility;
	dMn           = rData.dMn        ;
	dphiMn        = rData.dphiMn     ;
	dphi          = rData.dphi       ;
	dTp           = rData.dTp        ;
	dTp_bond      = rData.dTp_bond   ;
	dTp_unbond    = rData.dTp_unbond ;
	dTst          = rData.dTst       ;
	dCsc          = rData.dCsc       ;
	dCc           = rData.dCc        ;
	dfps          = rData.dfps       ;
	dfps_bond     = rData.dfps_bond  ;
	dfps_unbond   = rData.dfps_unbond;
	dfst          = rData.dfst       ;
	dfsc          = rData.dfsc       ;
	dfcc          = rData.dfcc       ;
	ddc           = rData.ddc        ;
	ddp           = rData.ddp        ;
	ddp_bond      = rData.ddp_bond   ;
	ddp_unbond    = rData.ddp_unbond ;
	ddst          = rData.ddst       ;
	ddsc          = rData.ddsc       ;
	dAp           = rData.dAp        ;
	dAp_bond      = rData.dAp_bond   ;
	dAp_unbond    = rData.dAp_unbond ;
	dAst          = rData.dAst       ;
	dAsc          = rData.dAsc       ;
	dAc           = rData.dAc        ;
	dNeuC         = rData.dNeuC      ;
	dH            = rData.dH         ;
	da            = rData.da         ;
	dde           = rData.dde        ;
	ddv           = rData.ddv        ;
	iNum_bond     = rData.iNum_bond  ;
	iNum_unbond   = rData.iNum_unbond;
	bOneType      = rData.bOneType   ;
	bBondType     = rData.bBondType  ;
	iCount        = rData.iCount     ;

	dMcr          = rData.dMcr       ;	    
	dfr           = rData.dfr        ;
	dfcpe         = rData.dfcpe      ; 
	dSc           = rData.dSc        ;
	
	dfck          = rData.dfck         ;         
	dfy           = rData.dfy          ;          
	dfpy          = rData.dfpy         ;         
	dfpu          = rData.dfpu         ;         
	dB_flange     = rData.dB_flange    ;    
	dB_web        = rData.dB_web       ;       
	dThick_Top    = rData.dThick_Top   ;   
	dThick_Bottom = rData.dThick_Bottom;
	dfst          = rData.dfst         ;         
	dfsc          = rData.dfsc         ;         
	return *this;
}

//////////////////////////////////////////////////////////////////////
// _DBLR_SHEAR_DATA
//////////////////////////////////////////////////////////////////////

void _DBLR_SHEAR_DATA::Initialize()
{
	ElemKey.first   = -1;
	iPosi     = 0;
	bPositive = TRUE;

	bStrainCompatibility = TRUE;
	bSegmentalBoxGirder  = TRUE;
	dMu = dVu = 0.0;
	dVn = dVc = dVs = dVp = 0.0;  
	dphiVn = 0.0;      
	
	strRatingCase = _T("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	dde = ddv   = dAv_s  = 0.0;  
	dex = dbeta = dtheta = 0.0;
}   
_DBLR_SHEAR_DATA& _DBLR_SHEAR_DATA::operator = (_DBLR_SHEAR_DATA& rData)
{
	Initialize();
	ElemKey   = rData.ElemKey  ;
	iPosi     = rData.iPosi    ;
	bPositive = rData.bPositive;

	bStrainCompatibility = rData.bStrainCompatibility;
	bSegmentalBoxGirder  = rData.bSegmentalBoxGirder; 
	dMu    = rData.dMu   ;
	dVu    = rData.dVu   ;
	dVn    = rData.dVn   ;
	dVc    = rData.dVc   ;
	dVs    = rData.dVs   ;
	dVp    = rData.dVp   ;  
	dphiVn = rData.dphiVn;    
	
	strRatingCase = rData.strRatingCase;
	dde    = rData.dde   ;
	ddv    = rData.ddv   ;
	dAv_s  = rData.dAv_s ;
	dex    = rData.dex   ;
	dbeta  = rData.dbeta ;
	dtheta = rData.dtheta;
	return *this;
}

//////////////////////////////////////////////////////////////////////
// CDgnBridgeLoadRating
//////////////////////////////////////////////////////////////////////

CDgnBridgeLoadRating::CDgnBridgeLoadRating()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_dZero=1.0E-07;
	m_PSC_iITER = 100;  

	// Coded by Seungjun for Test. ('20060719)
	CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
	m_bTestMode = csDesignOutput==_ULS(yes) ? TRUE : FALSE;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	m_iCivilCode = pDoc->GetCivilCodeDgn();
}

CDgnBridgeLoadRating::~CDgnBridgeLoadRating()
{
}

///////////////////////////////////////////
// For Connection to CRCDataCtrl.

void CDgnBridgeLoadRating::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit; 
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
	CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;
	m_pDataCtrl->Set_UnitIndex(CodeUnit);	

	Make_BridgeLoadRatingDgnData();

	// For test_DBLR.txt -----------------
	Prt_Setting(TRUE);
	Prt_ResultData(TRUE, _T("UNIT : Force=kip  Length=in"));
	//------------------------------------
}

BOOL CDgnBridgeLoadRating::Get_RatingDgnFlexuralStrength(ElemPairK ElemK, int iCheckPosi, BLR_FLEX_DATA& FlexD)
{
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	//Phi
	double dPhi_I_Posi, dPhi_I_Nega, dPhi_J_Posi, dPhi_J_Nega;
	dPhi_I_Posi = dPhi_I_Nega = dPhi_J_Posi = dPhi_J_Nega = 0.0;//초기화

	FlexD.Initialize();
	/*
	arMn.SetSize(4);
	arPhi.SetSize(4);
	for(int n=0 ; n<4 ; n++)
		arMn.SetAt(n, 0.0);//초기화
	*/

	T_BLRS_D BlrsD; BlrsD.Initialize();
	pDoc->m_pAttrCtrl->GetBlrs(BlrsD);

	BOOL bIter=FALSE; 
	if(BlrsD.iMn_Method==1) bIter = TRUE; //0=설계기준, 1=Iteration

	//+++++++++++++++++++++++++++++++++++++++
	// CurrUnit -> CodeUnit.
	//
	//+++++++++++++++++++++++++++++++++++++++
	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		// CodeUnit. -> CurrUnit
		//m_pDataCtrl->Get_KsceBdcyCodeToCurr(BdcrKsce);// 0.0 이므로 수정하지 않음
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		// CodeUnit. -> CurrUnit
		//m_pDataCtrl->Get_KsceBdcyCodeToCurr(BdcrKsce);// 0.0 이므로 수정하지 않음
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

	// Value of k  
	int iTndnKind = BlrsD.iTendonType;
	double dk = 0.0;    
	Get_AASHTOTndnValueOfk(iTndnKind, dk);
	
	BOOL bI=FALSE, bJ=FALSE;
	if(iCheckPosi==0) bI= TRUE;
	else if(iCheckPosi==1) bJ= TRUE;
	else if(iCheckPosi==2) { bI=TRUE, bJ=TRUE; }
	
	// For test_DBLR.txt -----------------
	m_bNewElemNum_Flex = (m_ElemKey_Flex != ElemK);
	if(m_bNewElemNum_Flex) m_ElemKey_Flex = ElemK;
	//------------------------------------
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=iSta; i<iEnd; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);        
			
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		arOuter.Copy(SectElem.SectPosi[i].arOuter);
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
		int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
		for(int m=0; m<iInnerCellNum; m++)
		{
			CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
			arInner.Copy(SectElem.SectPosi[i].arInner[m]);
			arpInner.Add(&arInner);
		}
			
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);    
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		BOOL bRebarExist=TRUE;
		BOOL bTndnExist=TRUE;
		if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
		{
			bRebarExist=FALSE;
			bTndnExist=FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}

		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    
				

		for(int j=0 ; j<2 ; j++) //Positive, Negative
		{
			int k = i*2+j;
			if(!bRebarExist && !bTndnExist) continue;      
			BOOL bPositive = (j==0 ? TRUE : FALSE);
			_DBLR_FLEX_DATA FlexData; FlexData.Initialize();      
			if(!Get_FlexureStrength(TRUE, bIter, bPositive, MatlElem, SpscSect, SpscgSect, arOuter, arpInner, 
															dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, FlexData)) continue;
			// For test_DBLR.txt -----------------
			FlexData.ElemKey = ElemK;
			FlexData.iPosi = i;
			FlexData.bPositive = bPositive;
			Prt_FlexData(TRUE, FlexData);
			if(m_bNewElemNum_Flex) m_bNewElemNum_Flex = FALSE;//첫번째 이후부터는 같은 요소의 I(-), J(+), J(-) 등을 출력하므로

			// 0=I-Positive, 1=I-Negative, 2=J-Positive, 3=J-Negative
			FlexD.dPhi[k] = FlexData.dphi; 
			FlexD.dMn[k]  = FlexData.dMn;
			FlexD.dArea_bar[k] = FlexData.dAst; // 인장측 철근면적
			FlexD.dArea_ten[k] = FlexData.dAp;  // 인장측 Tendon면적
			FlexD.bBarRat_Min[k] = (FlexData.dphiMn >= FlexData.dMcr); // 최소철근비 Check : AASHTO LRFD(2006) 5.7.3.3.2 참조
			if(Is_CaltransCode())
				FlexD.bBarRat_Max[k] = (FlexData.dde==0.0) ? FALSE :  FlexData.dNeuC/FlexData.dde <= 0.42; // Caltrans 요구사항 : 3359_Rating Result Enhancements 추가.xls 참조
			else
				FlexD.bBarRat_Max[k] = TRUE; // Provision Deleted in 2005
			//------------------------------------
		}
		// Delete because of New Operator.
		for(int k=0; k<arpInner.GetSize(); k++)
		{
			if(arpInner[k])
			{
				arpInner[k]->RemoveAll();
				delete arpInner[k];
				arpInner[k] = NULL; 
			}
		}
		arpInner.RemoveAll();  

	} 

	return bCalcOK;
}

BOOL CDgnBridgeLoadRating::Get_RatingDgnShearStrength(ElemPairK ElemK, int iCheckPosi, BOOL bFlexure, BOOL bSegBox, T_BLRC_K blrcK, BOOL bDc, BOOL bDw, BOOL bT, int iLine, double dNu, double dMu, double dVu, double dVp_TP, double da_g, double& dPhi_I, double& dPhi_J, CArray<double,double>& arVn)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	arVn.SetSize(2);
	arVn.SetAt(0, 0.0);  arVn.SetAt(1, 0.0);//초기화

	T_BLRS_D BlrsD; BlrsD.Initialize();
	pDoc->m_pAttrCtrl->GetBlrs(BlrsD);

	BOOL bIter=FALSE; 
	if(BlrsD.iMn_Method==1) bIter = TRUE; //0=설계기준, 1=Iteration

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		// CodeUnit. -> CurrUnit
		// arVn는 0.0으로 초기화 된 상태이므로 단위 변환 할 필요 없음
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		// CodeUnit. -> CurrUnit
		// arVn는 0.0으로 초기화 된 상태이므로 단위 변환 할 필요 없음
		return FALSE;
	}
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;  
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	// Value of k  
	int iTndnKind = BlrsD.iTendonType;
	double dk = 0.0;    
	Get_AASHTOTndnValueOfk(iTndnKind, dk);
	
	BOOL bI=FALSE, bJ=FALSE;
	if(iCheckPosi==0) bI= TRUE;
	else if(iCheckPosi==1) bJ= TRUE;
	else if(iCheckPosi==2) { bI=TRUE, bJ=TRUE; }
	
	// For test_DBLR.txt -----------------
	m_bNewElemNum_Shear = (m_ElemKey_Shear != ElemK);
	if(m_bNewElemNum_Shear) m_ElemKey_Shear = ElemK;
	//------------------------------------
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=iSta; i<iEnd; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE); 
			
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		arOuter.Copy(SectElem.SectPosi[i].arOuter);
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
		int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
		for(int m=0; m<iInnerCellNum; m++)
		{
			CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
			arInner.Copy(SectElem.SectPosi[i].arInner[m]);
			arpInner.Add(&arInner);
		}
			
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);    
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		// Sbar
		T_RPSC_SBAR RpscSbar = RshrElem.RshrPosi[i].RpscSbar;

		BOOL bRebarExist=TRUE;
		BOOL bTndnExist=TRUE;
		if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
		{
			bRebarExist=FALSE;
			bTndnExist=FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}

		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    
		
		if(!bRebarExist && !bTndnExist) continue;      
		_DBLR_SHEAR_DATA ShearData; ShearData.Initialize();      
		if(!Get_ShearDesign(bSegBox, bFlexure, TRUE, bIter, dMu, dVu, dVp_TP, da_g, MatlElem, SpscSect, SpscgSect, arOuter, arpInner, 
															dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, RpscSbar, ShearData)) continue;
		// For test_DBLR.txt -----------------
		CString strCaseName = GetRatingCaseString(blrcK, bDc, bDw, bT, iLine);
		strCaseName = GetStringReSize(43, strCaseName);

		ShearData.ElemKey = ElemK;
		ShearData.iPosi = i;
		ShearData.bPositive = (dMu>= -m_dZero);   
		ShearData.strRatingCase = strCaseName;
		
		Prt_ShearData(TRUE, ShearData);
		//------------------------------------
		//Phi    
		if(i==0)      dPhi_I = ShearData.dphi;
		else if(i==1) dPhi_J = ShearData.dphi;
		//Vn
		arVn.SetAt(i, ShearData.dVn);
		
		// Delete because of New Operator.
		for(int k=0; k<arpInner.GetSize(); k++)
		{
			if(arpInner[k])
			{
				arpInner[k]->RemoveAll();
				delete arpInner[k];
				arpInner[k] = NULL; 
			}
		}
		arpInner.RemoveAll();  

	}
	return bCalcOK;
}

BOOL CDgnBridgeLoadRating::Get_FlexureStrength(BOOL bYaxis, BOOL bIter, BOOL bPositive, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, 
																							 CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner,
																							 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, 
																							 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, _DBLR_FLEX_DATA& FlexData)

{
	FlexData.Initialize();// 초기화
	// 압축측 Tendon은 미고려
	// 모멘트 계산은 변형률접합해석시 중립축을 기준으로, Code해석시 복부 응력 블럭의 중심점을 기준으로 되었으나 값이 틸수 있으므로 중립축을 기준으로 수행
	//
	T_MATD_D MatD = MatlElem.MatdD;
	//Material
	double dfck= MatD.Data1.Design.C_fc;     
	double dEc = MatD.Data1.Analysis.Elast;  
	double dfy = MatD.MainRebarData.B_fy;    
	double dEs = MatD.MainRebarData.B_Elast; 
	//Section - 철근이 고려된 한산단면적
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dArea = SpscSect.dArea;
	double dIyy  = SpscSect.dIyy;
	double dIzz  = SpscSect.dIzz;
	double dIner   =(bYaxis==TRUE ? dIyy : dIzz);

	double dyDim_g = SpscgSect.dyDim;
	double dzDim_g = SpscgSect.dzDim;
	double dyCen_g = SpscgSect.dyCen;
	double dzCen_g = SpscgSect.dzCen;
	double dCpy_g  = dyDim_g - dyCen_g;
	double dCpz_g  = dzDim_g - dzCen_g;
	double dArea_g = SpscgSect.dArea;
	double dIyy_g  = SpscgSect.dIyy;
	double dIzz_g  = SpscgSect.dIzz;
	double dIner_g =(bYaxis==TRUE ? dIyy_g : dIzz_g);

	BOOL   bDirec  = (bPositive ? TRUE : FALSE);
	double dh      = (bYaxis==TRUE ? dzDim : dyDim);
	double dLength = (bYaxis==TRUE ? dyDim : dzDim);
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db      = (bPositive ? dbf_top : dbf_bot);   
	double dhf     = (bPositive ? dFlgThk[0] : dFlgThk[1]); // Flange Height. 
			
	double dphi    = 0.75; // 이후 긴장재의 변형률에 따라 재계산함
	double dalpha1 = 0.85; // phi와는 다른 값.
	double dBeta1 = Get_Beta1(dfck);
	 
	double dEpsi_cu = Get_MaxStrain_Concrete();            //콘크리트의 극한 변형률
	double dEpsi_st=0.0, dEpsi_sc=0.0;                     // st:인장철근의 변형율                           sc:압축철근의 변형룰
	double dEpsi_cp=0.0, dEpsi_ce=0.0;                     // cp:파괴시의 긴장재위치의 콘크리트 변형률(ε3)  ce:유효PS력만이 작용시의 긴장재위치의 콘크리트 변형률(ε2)
	double dEpsi_pe=0.0, dEpsi_ps=0.0;                     // pe:유효PS력만이 작용시의 긴장재의 변형률(ε1)  ps:파괴시의 긴장재의 총변형률
 
	//    
	double ddp=0.0,      ddc=0.0;                          // 긴장재의 힘의 도심의 압축단에서의 거리, 콘크리트의 힘의 도심에서 입축단에서의 거리
	double ddp_bond=0.0, ddp_unbond  =0.0;
	double dfst_Ave=0.0, dfsc_Ave=0.0;                     // 인장철근의 평균인장응력,  압축철근의 평균압축응력(Caltrans 수정안 용)
	double ddst=0.0,     ddsc=0.0;                         // 인장철근의 힘의 도심의 압축단에서의 거리, 압축철근의 힘의 도심의 압축단에서의 거리
	double dApt=0.0;                                       // 인장측 긴장재의 단면적
	double dAp_bond=0.0, dAp_unbond  =0.0;
	double dAst=0.0,     dAsc=0.0;                         // 인장측철근의 단면적                   , 압축측 철근의 단면적    
	double dQp_bond=0.0, dQp_unbond  =0.0;

	double dNeuC=0.0;                                      // 압축연단에서 중립축까지의 거리
	double dfst=0.0,     dfsc=0.0;                         // 인장철근의 응력, 압축철근의 응력 (변형율 접합공식용)
	double dfst_cal=0.0, dfsc_cal=0.0;                     // 인장철근의 계산상응력, 압축철근의 계산상응력(E*ε)
	
	double daArea=0.0,   daQ=0.0;                          // 응력블럭이 차지하는 면적, 응력블럭 하단면에서 면적의 1차모멘트
	double da=0.0,       daCent=0.0;                       // 응력블럭의 길이, 응력블럭하단면에서 응력블럭 중심까지의 거리
	double dCc=0.0,      dTp=0.0;                          // 콘크리트의 압축력,   인장긴장재의 총모멘트
	double dTp_bond=0.0, dTp_unbond=0.0; 
	double dTst=0.0,     dCsc=0.0;                         // 인장철근의 총힘, 압축철근의 총힘
	double dMst=0.0,     dMsc=0.0;                         // 인장철근의 총모멘트, 압축철근의 총모멘트
	
	double dfps=0.0,     dfps_bond=0.0,  dfps_unbond =0.0; // 긴장재의 응력or평균응력,   
	double dMps=0.0,     dMps_bond=0.0,  dMps_unbond=0.0;  //긴장재의 모멘트
	double dMn=0.0,      dphiMn=0.0;
	double dMn_Cal=0.0,  dphiMn_Cal=0.0;
	double dCcDel=0.0;                                     // 축력(인장+, 여기서는 재거 되어야 할 힘임)
	double dTOL2 = 0.001;
	double dToler=0.0;


	// 긴장재의 평균 변형률로 Φ값 산출을 위한것이므로 항복이후에는 정밀한 값은 필요 없음(0.002~0.005까지의 변형률에 대해서만 필요하며 항복은 일반적으로 0.005이후에 생김)
	double dEpsi_pt=0.0;                                   // 인장측 긴장재의 평균변형률 // (SHIN 06.12.18) 인장측의 평균변형률로 하기로 합의함(김정인DL)
	double dEpsi_pt_bond=0.0, dEpsi_pt_unbond=0.0;         // bonded, unbonded 별 인장측 긴장재의 평균변형률
	

	// Code의 공식을 사용할 때.
	BOOL bOneType   =TRUE;
	BOOL bBondType  =TRUE;
	UINT iBondNum   =0;
	UINT iUnbondNum =0;
	double dfpy_bond=0.0;   //bonded중 최소 항복응력
	double dfpy_unbond=0.0; //unbonded중 최소 항복응력
	double dfpu_bond=0.0;   //bonded중 최소 극한응력
	double dfpu_unbond=0.0; //unbonded중 최소 극한응력
	double dEps_bond=0.0;   //bonded중 최대 탄성계수
	double dEps_unbond=0.0; //bonded중 최대 탄성계수

	int i=0,j=0;

	//Assume C.  
	double dCp = 0.0;
	if(bPositive) dCp = (bYaxis==TRUE ? dCpz_g : dCpy_g);
	else          dCp = (bYaxis==TRUE ? dzCen_g : dyCen_g);
	double dCtemp=0.0;
	int NumberofTendon = arTndnPosi.GetSize();
	int NumberofRebar  = arRbarPosi.GetSize();
	UINT iCount = 1;  
	

	// 중립축 결정을 위해서 최외각 인장범 Tendon위치 검토(측위는 압축연단에서 인장연단 또는 최 외각 인장Tendon까지)  
	BOOL bPosiForce=TRUE;
	double dNeuC1 = (bYaxis==TRUE ? dzDim : dyDim);//전체 압축으로 초기화
	double dNeuC2 = 0.0;                           //전체 인장으로 초기화
	double dTotalTpe = 0.0;                        //손실후 초기 프리스트레스력의 총합
	for(i=0 ; i<NumberofTendon ; i++)
	{
		_DGN_TNDN_CRC TnInf  = arTndnPosi.GetAt(i);    
		double ddpi=0.0; // 압축연단에서의 거리
		if(bPositive)
			ddpi = (bYaxis==TRUE ? dCpz_g - TnInf.dz : dCpy_g - TnInf.dy); 
		else
			ddpi = (bYaxis==TRUE ? dzCen_g + TnInf.dz : dyCen_g + TnInf.dy); 
		dNeuC1 = max(dNeuC1, ddpi);
		dTotalTpe += TnInf.dArea*TnInf.dStress[0];
	}
	dNeuC = (dNeuC1 + dNeuC2) / 2.0;
	// 단면의 압축강도(콘크리트+철근)가 손실후 유효스트레스력보다 작은지 검토 
	double dTotalRebarArea = 0.0;//전체 철근의 면적
	for(i=0 ; i<NumberofRebar ; i++)
	{
		_DGN_RBAR_CRC ReInf = arRbarPosi.GetAt(i);
		dTotalRebarArea += ReInf.dArea;
	}
	double dCompStrength = dalpha1*dfck*dArea + dTotalRebarArea*dfy;
	if(dCompStrength < dTotalTpe) 
		return TRUE; // 긴장력에 단면이 견디지 못하므로 종료.(FlexData은 초기화된 값임) 단 전단계산시 dv값 산출을 위해 de, dv값이 사용되나 전단 계산시 dv의 한계값을 0.72*H로 규정하므로 괜찮음
			
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi; // 중립축을 중심으로 좌표 변환된 긴장재의 정보
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi; // 중립축을 중심으로 좌표 변환된 철근의 정보


	double dPe=0.0, dMe=0.0, dep=0.0; //전체 프리스트레스력, 전체 프리스트레스력에 의한 균열전도심에서의 모멘트, 편심거리 
	for(i=0 ; i<NumberofTendon ; i++)
	{// 손실후 유효프리스트레스력만 작용시의 전체 프리스트레스력과 균열전도심(환산단면도심)에 대한 편심거리 산출 
		_DGN_TNDN_CRC rTnInf = arTndnPosi.GetAt(i);
		if(rTnInf.dArea < m_dZero) continue; // 단면이 음수일 경우 검토 대상에서 제외
		double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);        
		dPe += rTnInf.dArea * rTnInf.dStress[0];
		dMe += rTnInf.dArea * rTnInf.dStress[0] * (-1.0)*dposi;//긴장재가 인장이고 위치가 중립축 이하일때가 +모멘트 
	}
	dep = (dPe==0 ? 0.0: dMe/dPe);// 부호값을 가짐(+:상or우, -:하or좌)

	if(bIter)
	{// 변형율 접합공식 적용시 : 축하중이 0이 되는 중립축을 찾음 //////////////////////////////////////////////////////////////////////
		do
		{
			//초기화
			dCc = 0.0;
			dMst = dMsc = dTst = dCsc = dAsc = dAst = 0.0;   
			dfps = dMps = dTp = dApt = 0.0;
			dMps_bond = dMps_unbond = 0.0;
			dTp_bond  = dTp_unbond  = 0.0;   
			dAp_bond  = dAp_unbond  = 0.0;
			dEpsi_pt  = dEpsi_pt_bond = dEpsi_pt_unbond = 0.0;

			double dTp_Approximation = 0.0;// 수렴하지 못하는 경우에 단면이 긴장력에 버티는지 여부를 학인하기 위한 근사 긴장력(중립축 근처의 긴장재를 포함한 인장영역의 전체 긴장력)

			da = dBeta1*dNeuC;

			//1. Concrete
			double daTemp = (bPositive ? dh-da : da);
			if(CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, daTemp, dLength, daArea, daQ))
			{
				daCent = (daArea==0.0 ? 0.0 : daQ/daArea);
				dCc    = dalpha1*dfck*daArea; //
			}      
			else
			{ ASSERT(0); }

			Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);
			//2. Rebar
			for(i=0 ; i<NumberofRebar ; i++)
			{
				_DGN_RBAR_CRC rReInf = rarRbarPosi.GetAt(i);
				double dposi = (bYaxis==TRUE ? rReInf.dz : rReInf.dy); 
				if((bPositive && dposi<0.0) || (!bPositive && dposi>0.0)) // 인장측
				{
					double dds_t = dNeuC + fabs(dposi);
					dEpsi_st = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dds_t-dNeuC)/dNeuC);
					dfst_cal = dEpsi_st*dEs; 
					dfst = min(dfst_cal, dfy);
					dTst += rReInf.dArea*dfst;
					dMst += rReInf.dArea*dfst * fabs(dposi);  
					dAst += rReInf.dArea;
				}
				else
				{
					double dds_c = dNeuC + fabs(dposi);
					dEpsi_sc = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dds_c-dNeuC)/dNeuC);
					dfsc_cal = dEpsi_sc*dEs; 
					dfsc = min(dfsc_cal, dfy);
					dCsc += rReInf.dArea*dfsc;
					dMsc += rReInf.dArea*dfsc * fabs(dposi);      
					dAsc += rReInf.dArea;
				}
			}

			//3. Tendon      
			dfpu_bond   = 1.0/m_dZero;  dfpu_unbond = 1.0/m_dZero; //변형율 접합공식 적용시에는 필요없는 값이나 text출력을 위해 산출함
			dfpy_bond   = 1.0/m_dZero;  dfpy_unbond = 1.0/m_dZero; //변형율 접합공식 적용시에는 필요없는 값이나 text출력을 위해 산출함
			
			iBondNum = iUnbondNum = 0;
			for(i=0 ; i<NumberofTendon ; i++)
			{// 파괴시의 값 계산
				_DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
				_DGN_TNDN_CRC TnInf  = arTndnPosi.GetAt(i);
				double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);// 파괴시의 중림축에서 편심거리
				double dei     = (bYaxis==TRUE ? TnInf.dz : TnInf.dy);  // 균열전 도심에서의 편심거리
			
				if(rTnInf.dArea < m_dZero) continue; // 단면이 음수일 경우 검토 대상에서 제외

				if((bPositive && dposi<0.0) || (!bPositive && dposi>0.0)) // 인장측만 고려
				{          
					double ddpi = dNeuC + fabs(dposi);          

					if(rTnInf.iBondType==0)   
					{            
						iBondNum++;      
						dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
						dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
						dEpsi_ce = (dArea_g==0.0 || dEc==0.0 || dIner_g==0.0) ? 0.0 : dPe/(dArea_g*dEc)*(1.0+(dep*dei)/(dIner_g/dArea_g));  
						dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
						dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps); 
						dTp_bond  += dfps*rTnInf.dArea;
						dMps_bond += dfps*rTnInf.dArea*fabs(dposi);  
						dAp_bond  += rTnInf.dArea;
						//
						dEpsi_pt_bond += dEpsi_ps*rTnInf.dArea;
						//
						dfpu_bond  = min(dfpu_bond, rTnInf.dfpu);  // 한요소에 Tendon Material 여러가지 이면 최소.
						dfpy_bond  = min(dfpy_bond, rTnInf.dfpy);
					}
					else if(rTnInf.iBondType==1) 
					{
						iUnbondNum++;      
						double dle = Get_EffectiveTendonLength(rTnInf);
						//dfps = (dle==0.0 ? 0.0 : min(rTnInf.dStress[0]+900.0*fabs(dposi)/dle, rTnInf.dfpy));
						dfps = min(rTnInf.dStress[0]+15.0, rTnInf.dfpy);// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망
						dTp_unbond  += dfps*rTnInf.dArea;
						dMps_unbond += dfps*rTnInf.dArea*fabs(dposi);  
						dAp_unbond  += rTnInf.dArea;
						//
						dEpsi_pt_unbond += dfps/rTnInf.dEps * rTnInf.dArea;// (SHIN 06.12.30) unbond의 경우 변형률을 산출하기 어려워 간략식형식으로 대처하였음
						//
						dfpu_unbond  = min(dfpu_bond, rTnInf.dfpu);  // 한요소에 Tendon Material 여러가지 이면 최소.
						dfpy_unbond  = min(dfpy_bond, rTnInf.dfpy);
					}          
				}
				dEpsi_pt        = (dAp_bond+dAp_unbond ==0.0)? 0.0 : (dEpsi_pt_bond+dEpsi_pt_unbond)/(dAp_bond+dAp_unbond);
				dEpsi_pt_bond   = (dAp_bond   == 0.0)? 0.0 : dEpsi_pt_bond/dAp_bond;
				dEpsi_pt_unbond = (dAp_unbond == 0.0)? 0.0 : dEpsi_pt_unbond/dAp_unbond;

				// 중립축에 근접한 긴장재도 dTp_Approximation값에 포함시킴(이 값은 수렴하지 못하는 경우 긴장력에 단면이 견디는지 여부를 확인하기 위함임) 
				if((bPositive && dposi<fabs(dNeuC-dNeuC1)) || (!bPositive && dposi>(-1)*fabs(dNeuC-dNeuC1))) 
				{
					double ddpi = dNeuC + fabs(dposi);   
					double dfps_App = 0.0;
					if(rTnInf.iBondType==0)   
					{            
						dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
						dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
						dEpsi_ce = (dArea_g==0.0 || dEc==0.0 || dIner_g==0.0) ? 0.0 : dPe/(dArea_g*dEc)*(1.0+(dep*dei)/(dIner_g/dArea_g));  
						dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
						dfps_App = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);             
					}
					else if(rTnInf.iBondType==1) 
					{
						double dle = Get_EffectiveTendonLength(rTnInf);
						//dfps_App = (dle==0.0 ? 0.0 : min(rTnInf.dStress[0]+900.0*fabs(dposi)/dle, rTnInf.dfpy));
						dfps_App = min(rTnInf.dStress[0]+15.0, rTnInf.dfpy);// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망            
					}      
					dTp_Approximation += dfps_App*rTnInf.dArea;        
				}
			}      
			
			//4. 총 압축력 검토
			double dComp = dCc;
			dTp  = dTp_bond + dTp_unbond;      
		  dCcDel = dTp + dTst - dCsc - dCc;
			dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

			if(fabs(dToler)<dTOL2) break;
			if(iCount>m_PSC_iITER) 
			{/*ASSERT(0);*/ 
				if(dCompStrength < dTp_Approximation)return TRUE; //수렵하지 못하였을때 산출된 긴장력에 단면이 견디지 못하는 경우 종료.(FlexData은 초기화된 값임)
				break;
			}

			if(dCcDel>=0.0) dNeuC2 = dNeuC; // 인장력이 클경우
			else            dNeuC1 = dNeuC; // 압축력이 클경우
						
			dNeuC = 0.5*(dNeuC1 + dNeuC2);      

			++iCount;

		}while(fabs(dToler)>dTOL2);
		
		dMps = dMps_bond + dMps_unbond;     

		if(Is_CaltransCode())// Caltrans Code
			Get_Phi_PrestressedConcrete_Flexure(dEpsi_pt, arTndnPosi, dphi);
		else                 // AASTO Code (현재(06.12.30) 사용하지 않음)
		{
			double dTemp_Phiv;
			Get_Phi_PrestressedConcrete_AASHTO(MatlElem.MatdD.MainRebarData.B_fy, arRbarPosi, arTndnPosi, dphi, dTemp_Phiv);
		}


		//dNeuC에서 dMn..
		if(fabs(dNeuC) < m_dZero)
		{
			dMn = dMn_Cal = dphiMn = dphiMn_Cal = 0.0;
		}
		else
		{
			dMn = dCc*(dNeuC-(da-daCent)) + dMps + dMst + dMsc; 
			dMn_Cal = dMn;   
			dphiMn = dphi*dMn;    
			dphiMn_Cal = dphiMn;
		}
		// 최대철근량에 대한 규정은 없어 졌으므로(2005년 부터) 고려하지 않음
		// 기타
		dApt       = dAp_bond + dAp_unbond;
		ddp        = (dTp==0.0 ? 0.0 : dNeuC + dMps/dTp);
		ddc        = da-daCent;
		dfps       = (dApt==0.0 ? 0.0 : dTp/dApt);    
		ddp_bond   = (dTp_bond==0.0 ? 0.0 : dNeuC + dMps_bond/dTp_bond);
		ddp_unbond = (dTp_unbond==0.0 ? 0.0 : dNeuC + dMps_unbond/dTp_unbond);    
		ddst       = (dTst==0.0 ? 0.0 : dNeuC + dMst/dTst);
		ddsc       = (dCsc==0.0 ? 0.0 : dNeuC - dMsc/dCsc);
		dfps_bond  = (dAp_bond==0.0   ? 0.0 : dTp_bond/dAp_bond);
		dfps_unbond= (dAp_unbond==0.0 ? 0.0 : dTp_unbond/dAp_unbond);      
		
		if(iBondNum>=0 && iUnbondNum==0)
		{ bBondType = TRUE;   bOneType  = TRUE; }// 긴장재가 없는 경우에도 포함함
		else if(iBondNum==0 && iUnbondNum>0)
		{ bBondType = FALSE;  bOneType  = TRUE; }
		else if(iBondNum>0 && iUnbondNum>0)
		{ bBondType = FALSE;  bOneType  = FALSE;}  
	}
	else
	{// AASHTO 공식 적용시 : AASHTO기준상의 중립축의 위치와 우리가 가정한 중립축의 위치가 같은 위치를 찾음 ////////////////////////////////////////////
	 // Caltrans 수정안으로 변경함 (SHIN 06.12.30  변경내용: Φ값 산출, 철근의 응력을 실제 변형율 반영)
		BOOL bCalcOK;    
		// 긴장재 조건이 부착인지, 비부착인지, 또는 조합형인지 검토
		da=0.0;
		double dle_ave = 0.0;
		double dTpe_bond=0.0, dTpe_unbond=0.0;// 손실후 유효프리스트레스력
		// fps 산출
		double dNeuC_Code;// AASHTO에 기재된 방법으로 중립축을 계산한값
		
		// 중립축을 찾음      
		do
		{         
			dAsc = dAst = 0.0; 

			da = dBeta1*dNeuC;      

			Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);        
			
			// 1. 철근의 값은 아래 조건으로 찾음 
			if(Is_CaltransCode())
			{// Caltrans Code
				dMst = dMsc = dTst = dCsc = 0.0;
				for(i=0 ; i<NumberofRebar ; i++)
				{
					_DGN_RBAR_CRC rReInf = rarRbarPosi.GetAt(i);
					double dposi = (bYaxis==TRUE ? rReInf.dz : rReInf.dy); 
					if((bPositive && dposi<0.0) || (!bPositive && dposi>0.0)) // 인장측
					{
						double dds_t = dNeuC + fabs(dposi);
						if(dNeuC/dds_t > 0.6)
						{
							dEpsi_st = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dds_t-dNeuC)/dNeuC);
							dfst_cal = dEpsi_st*dEs; 
							dfst = min(dfst_cal, dfy);
						}
						else 
							dfst = dfy;
						dTst += rReInf.dArea*dfst;
						dMst += rReInf.dArea*dfst * fabs(dposi);  
						dAst += rReInf.dArea;
					}
					else
					{
						double dds_c = dNeuC + fabs(dposi);
						if(dNeuC < 3.0*(dNeuC-fabs(dposi)))
						{
							dEpsi_sc = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dds_c-dNeuC)/dNeuC);
							dfsc_cal = dEpsi_sc*dEs; 
							dfsc = min(dfsc_cal, dfy);
						}
						else
							dfsc = dfy;
						dCsc += rReInf.dArea*dfsc;
						dMsc += rReInf.dArea*dfsc * fabs(dposi);      
						dAsc += rReInf.dArea;
					}
				}
				ddst = (dTst==0.0 ? 0.0 : dNeuC + dMst/dTst);
				ddsc = (dCsc==0.0 ? 0.0 : dNeuC - dMsc/dCsc);
				dfst_Ave = (dAst==0.0 ? 0.0 : dTst/dAst);
				dfsc_Ave = (dAsc==0.0 ? 0.0 : dCsc/dAsc);
			}
			else
			{// AASTHO Code 
				dCtemp = (bPositive ? dNeuC : dh-dNeuC);
				_DGN_TNDN_RBAR rTdRb;
				rTdRb.Initialize();      
				bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
				if(bPositive)  // 정모멘트 
				{
					dAst=rTdRb.dAsttol; ddst = (dAst==0.0 ? 0.0 : rTdRb.ddst);
					dAsc=rTdRb.dAsctol; ddsc = (dAsc==0.0 ? 0.0 : rTdRb.ddsc);
				}
				else
				{
					dAst=rTdRb.dAsctol; ddst = (dAst==0.0 ? 0.0 : dh - rTdRb.ddsc);
					dAsc=rTdRb.dAsttol; ddsc = (dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
				}   
				// AASTHO는 모든 철근이 항복한것으로 계산함
				dfst_Ave = dfy;
				dfsc_Ave = dfy;
			}


			// 2. 긴장재값 산출      

			dfpu_bond   = 1.0/m_dZero;  dfpu_unbond = 1.0/m_dZero; // fpu는 최소값을 찾음
			dfpy_bond   = 1.0/m_dZero;  dfpy_unbond = 1.0/m_dZero; // fpy는 최소값을 찾음
			dEps_bond   = 0.0;          dEps_unbond = 0.0;         // Eps는 최대값을 찾음
			dTp_bond=0.0, dTp_unbond=0.0;
			dQp_bond=0.0, dQp_unbond=0.0;
			dAp_bond=0.0, dAp_unbond=0.0;
			ddp_bond   =0.0, ddp_unbond   =0.0;
			dle_ave = 0.0;
			iBondNum = iUnbondNum = 0;
			dTpe_bond= dTpe_unbond=0.0;
			
			double dTp_Approximation = 0.0;// 수렴하지 못하는 경우에 단면이 긴장력에 버티는지 여부를 학인하기 위한 근사 긴장력(중립축 근처의 긴장재를 포함한 인장영역의 전체 긴장력)
			BOOL   bOneType_App=TRUE, bBondType_App=TRUE;
			double dfpu_App_bond = 1.0/m_dZero,  dfpu_App_unbond = 1.0/m_dZero;
			double dfpy_App_bond = 1.0/m_dZero,  dfpy_App_unbond = 1.0/m_dZero;
			double dfps_App_bond = 0.0,          dfps_App_unbond = 0.0;
			double dQp_App_bond=0.0, dQp_App_unbond=0.0;
			double dAp_App_bond=0.0, dAp_App_unbond=0.0;
			double ddp_App_bond=0.0, ddp_App_unbond=0.0;
			double dle_App_ave = 0.0;
			double iBondNum_App=0, iUnbondNum_App = 0;
			double dTpe_App_bond=0.0, dTpe_App_unbond=0.0;
			
			for(i=0 ; i<NumberofTendon ; i++)
			{
				_DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);

				double dposi = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
				double ddpi  = dNeuC + fabs(dposi);
		
				if(rTnInf.dArea < m_dZero) continue; // 단면이 음수일 경우 검토 대상에서 제외
				if((bPositive && dposi<0.0) || (!bPositive && dposi>0.0)) // 인장측만 고려
				{         
					if(rTnInf.iBondType==0)   
					{        
						iBondNum++;         
						dfpu_bond  = min(dfpu_bond, rTnInf.dfpu);  // 한요소에 Tendon Material 여러가지 이면 최소.
						dfpy_bond  = min(dfpy_bond, rTnInf.dfpy);
						dEps_bond += rTnInf.dArea*rTnInf.dEps;
						dTpe_bond += rTnInf.dArea*rTnInf.dStress[0];
						dQp_bond  += rTnInf.dArea*ddpi;
						dAp_bond  += rTnInf.dArea;
					}
					else if(rTnInf.iBondType==1) 
					{
						iUnbondNum++;         
						dfpu_unbond  = min(dfpu_unbond, rTnInf.dfpu);
						dfpy_unbond  = min(dfpy_unbond, rTnInf.dfpy);        
						dEps_unbond += rTnInf.dArea*rTnInf.dEps;
						dTpe_unbond += rTnInf.dArea*rTnInf.dStress[0];
						dQp_unbond  += rTnInf.dArea*ddpi;
						dAp_unbond  += rTnInf.dArea;
						dle_ave     += Get_EffectiveTendonLength(rTnInf);              
					}
					else ASSERT(0);
				} 
				// 중립축에 근접한 긴장재도 dTp_Approximation값에 포함시킴(이 값은 수렴하지 못하는 경우 긴장력에 단면이 견디는지 여부를 확인하기 위함임) 
				if((bPositive && dposi<fabs(dNeuC-dNeuC1)) || (!bPositive && dposi>(-1)*fabs(dNeuC-dNeuC1))) 
				{
					if(rTnInf.iBondType==0)  
					{
						iBondNum_App++;         
						dfpu_App_bond  = min(dfpu_App_bond, rTnInf.dfpu);  // 한요소에 Tendon Material 여러가지 이면 최소.
						dfpy_App_bond  = min(dfpy_App_bond, rTnInf.dfpy);
						dTpe_App_bond += rTnInf.dArea*rTnInf.dStress[0];
						dQp_App_bond  += rTnInf.dArea*ddpi;
						dAp_App_bond  += rTnInf.dArea;
					}
					else if(rTnInf.iBondType==1) 
					{
						iUnbondNum_App++;         
						dfpu_App_unbond  = min(dfpu_App_unbond, rTnInf.dfpu);
						dfpy_App_unbond  = min(dfpy_App_unbond, rTnInf.dfpy);        
						dTpe_App_unbond += rTnInf.dArea*rTnInf.dStress[0];
						dQp_App_unbond  += rTnInf.dArea*ddpi;
						dAp_App_unbond  += rTnInf.dArea;
						dle_App_ave     += Get_EffectiveTendonLength(rTnInf);     
					}
					else ASSERT(0);
				}
			}
			ddp_bond   = (dAp_bond==0.0   ? 0.0 : dQp_bond/dAp_bond);
			ddp_unbond = (dAp_unbond==0.0 ? 0.0 : dQp_unbond/dAp_unbond);
			dle_ave    = (iUnbondNum==0     ? 0.0 : dle_ave /= (double)iUnbondNum);

			dEps_bond   = (dAp_bond==0.0   ? 0.0 : dEps_bond/dAp_bond);
			dEps_unbond = (dAp_unbond==0.0 ? 0.0 : dEps_unbond/dAp_unbond);


			if(iBondNum>0 && iUnbondNum==0)
			{ bBondType = TRUE;   bOneType  = TRUE; }
			else if(iBondNum==0 && iUnbondNum>0)
			{ bBondType = FALSE;  bOneType  = TRUE; }
			else if(iBondNum>0 && iUnbondNum>0)
			{ bBondType = FALSE;  bOneType  = FALSE;}         

			if(da > dh)
			{
				// 이때에는 dNeuC_Code값은 실재 중립축을 뜻하는 것이 아니며 음수냐 양수냐가 중요함 
				// 음수일경우 압축력이 크므로 중립축을 압축측으로 이동하도록 dNeuC_Code값을 dNeuC보다 작게 만들어 주며
				// 양수일 경우 인장력이 크므로 더이상 힘의 평형조건에 의한 수렴을 할 수 없으며 계속 인장Tendon의 도심을 향하도록 dNeuC_Code값을 dNeuC보다 크게 만들어 줌
				dfps_unbond = (dAp_unbond==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond + 15.0, dfpy_unbond));// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망
				dCcDel = dAp_bond*dfpu_bond + dAp_unbond*dfps_unbond + dAst*dfy - dAsc*dfy - dalpha1*dfck*(db-dbw)*dhf - dalpha1*dfck*dbw*dh; 
				if(dCcDel > m_dZero)                                                                            
					dNeuC_Code = dNeuC + 1.0;
				else
					dNeuC_Code = dNeuC - 1.0;
			}
			else
			{
				if(bOneType)
				{
					if(bBondType)
					{ // 긴장재 조건이 부착만일경우           
						double dk = (dfpu_bond==0.0 ? 0.08 : 2.0*(1.04-dfpy_bond/dfpu_bond));
						double dfpu_Over_dp = (ddp_bond==0.0 ? 0.0 : dfpu_bond/ddp_bond);
						if(dhf>=da) dNeuC_Code = (dAp_bond*dfpu_bond + dAst*dfst_Ave - dAsc*dfsc_Ave) / (dalpha1*dfck*dBeta1*db + dk*dAp_bond*dfpu_Over_dp);                             // 사각형
						else        dNeuC_Code = (dAp_bond*dfpu_bond + dAst*dfst_Ave - dAsc*dfsc_Ave - dalpha1*dfck*(db-dbw)*dhf) / (dalpha1*dfck*dBeta1*dbw + dk*dAp_bond*dfpu_Over_dp);// T형          
					}
					else
					{ // 긴장재 조건이 비부착만일경우        
						//dfps_unbond = (dAp_unbond==0.0 || dle_ave==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond + 900.0*(ddp_unbond-dNeuC)/dle_ave, dfpy_unbond));
						dfps_unbond = (dAp_unbond==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond + 15.0, dfpy_unbond));// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망
						if(dhf>=da) dNeuC_Code = (db==0.0  ? 0.0 : (dAp_unbond*dfps_unbond + dAst*dfst_Ave - dAsc*dfsc_Ave) / (dalpha1*dfck*dBeta1*db));                             // 사각형
						else        dNeuC_Code = (dbw==0.0 ? 0.0 : (dAp_unbond*dfps_unbond + dAst*dfst_Ave - dAsc*dfsc_Ave - dalpha1*dfck*(db-dbw)*dhf) / (dalpha1*dfck*dBeta1*dbw));// T형          
					}
				}
				else
				{   // 긴장재 조건이 부착, 비부착 조합일 경우
					//dfps_unbond = (dAp_unbond==0.0 || dle_ave==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond + 900.0*(ddp_unbond-dNeuC)/dle_ave, dfpy_unbond));
					dfps_unbond = (dAp_unbond==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond + 15.0, dfpy_unbond));// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망
					double dk = (dfpu_bond==0.0 ? 0.08 : 2.0*(1.04-dfpy_bond/dfpu_bond));
					double dfpu_Over_dp = (ddp_bond==0.0 ? 0.0 : dfpu_bond/ddp_bond);
					if(dhf>=da) dNeuC_Code = (dAp_bond*dfpu_bond + dAp_unbond*dfps_unbond + dAst*dfst_Ave - dAsc*dfsc_Ave) / (dalpha1*dfck*dBeta1*db + dk*dAp_bond*dfpu_Over_dp);                             // 사각형
					else        dNeuC_Code = (dAp_bond*dfpu_bond + dAp_unbond*dfps_unbond + dAst*dfst_Ave - dAsc*dfsc_Ave - dalpha1*dfck*(db-dbw)*dhf) / (dalpha1*dfck*dBeta1*dbw + dk*dAp_bond*dfpu_Over_dp);// T형
				}
			}     

			dToler = dNeuC-dNeuC_Code;
			if(fabs(dToler) < dTOL2) break;
			if(iCount>m_PSC_iITER) 
			{/*ASSERT(0);*/ //수렵하지 못하였을때 산출된 긴장력에 단면이 견디지 못하는 검토
				ddp_App_bond   = (dAp_App_bond==0.0   ? 0.0 : dQp_App_bond/dAp_App_bond);
				ddp_App_unbond = (dAp_App_unbond==0.0 ? 0.0 : dQp_App_unbond/dAp_App_unbond);
				dle_App_ave    = (iUnbondNum_App==0     ? 0.0 : dle_App_ave /= (double)iUnbondNum_App);
				if(iBondNum_App>0 && iUnbondNum_App==0)
				{ bBondType_App = TRUE;   bOneType_App  = TRUE; }
				else if(iBondNum_App==0 && iUnbondNum_App>0)
				{ bBondType_App = FALSE;  bOneType_App  = TRUE; }
				else if(iBondNum_App>0 && iUnbondNum_App>0)
				{ bBondType_App = FALSE;  bOneType_App  = FALSE;}      
				
				if(bOneType_App)
				{
					if(bBondType_App)
					{ // 긴장재 조건이 부착만일경우    
						double dk = 2.0*(1.04-dfpy_App_bond/dfpu_App_bond);
						dfps_App_bond = (ddp_App_bond==0 ? 0.0 : dfpu_App_bond*(1-dk*dNeuC/ddp_App_bond));
						dfps_App_unbond = 0.0;
					}
					else
					{ // 긴장재 조건이 비부착만일경우  
						//dfps_App_unbond = (dAp_App_unbond==0.0 || dle_App_ave==0.0 ? 0.0 : min(dTpe_App_unbond/dAp_App_unbond+900.0*fabs(ddp_App_unbond-dNeuC)/dle_App_ave, dfpy_App_unbond)); 
						dfps_App_unbond = (dAp_App_unbond==0.0 ? 0.0 : min(dTpe_App_unbond/dAp_App_unbond+15.0, dfpy_App_unbond));// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망
						dfps_App_bond = 0.0;
					}   
				}
				else
				{   // 긴장재 조건이 부착, 비부착 조합일 경우
					double dk = 2.0*(1.04-dfpy_App_bond/dfpu_App_bond);
					dfps_App_bond   = (ddp_App_bond==0.0 ? 0.0 : dfpu_App_bond*(1-dk*dNeuC/ddp_App_bond));
					//dfps_App_unbond = (dAp_App_unbond==0.0 || dle_App_ave==0.0 ? 0.0 : min(dTpe_App_unbond/dAp_App_unbond + 900.0*(ddp_App_unbond-dNeuC)/dle_App_ave, dfpy_App_unbond));
					dfps_App_unbond = (dAp_App_unbond==0.0 ? 0.0 : min(dTpe_App_unbond/dAp_App_unbond + 15.0, dfpy_App_unbond));// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망          
				}
				dTp_Approximation = dAp_App_bond*dfps_App_bond + dAp_App_unbond*dfps_App_unbond;

				if(dCompStrength < dTp_Approximation)return TRUE; //수렵하지 못하였을때 산출된 긴장력에 단면이 견디지 못하는 경우 종료.(FlexData은 초기화된 값임)
				break;
			}

			if(dNeuC_Code > dNeuC) dNeuC2 = dNeuC; // Code중립축이 큰 경우 
			else                   dNeuC1 = dNeuC; //
						
			dNeuC = 0.5*(dNeuC1 + dNeuC2);     

			++iCount;
		}while(fabs(dToler)>dTOL2);

		// 평균 fps를 산출
		if(bOneType)
		{
			if(bBondType)
			{ // 긴장재 조건이 부착만일경우    
				double dk = 2.0*(1.04-dfpy_bond/dfpu_bond);
				dfps = dfps_bond = (ddp_bond==0 ? 0.0 : dfpu_bond*(1-dk*dNeuC/ddp_bond));
				dfps_unbond = 0.0;
				dApt = dAp_bond;
				ddp  = ddp_bond;

				dEpsi_pt_bond   = (dEps_bond==0.0 ? 0.0 : dfps_bond/dEps_bond);// dEps_bond가 0인경우는 긴장재가 없거나 탄성계수 입력을 0으로 한 경우이므로 실제는 무한대 값이나 Φ값을 낮추기 위해 0.0으로 만듬       
				dEpsi_pt_unbond = 0.0;
				dEpsi_pt        = dEpsi_pt_bond;        
			}
			else
			{ // 긴장재 조건이 비부착만일경우  
				//dfps = dfps_unbond = (dAp_unbond==0.0 || dle_ave==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond+900.0*fabs(ddp_unbond-dNeuC)/dle_ave, dfpy_unbond)); 
				dfps = dfps_unbond = (dAp_unbond==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond+15.0, dfpy_unbond));// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망
				dfps_bond = 0.0;
				dApt = dAp_unbond;
				ddp  = ddp_unbond;

				dEpsi_pt_bond   = 0.0;        
				dEpsi_pt_unbond = (dEps_unbond==0.0 ? 0.0 : dfps_unbond/dEps_unbond);// dEps_unbond가 0인경우는 긴장재가 없거나 탄성계수 입력을 0으로 한 경우이므로 실제는 무한대 값이나 Φ값을 낮추기 위해 0.0으로 만듬
				dEpsi_pt        = dEpsi_pt_unbond;        
			}   
		}
		else
		{   // 긴장재 조건이 부착, 비부착 조합일 경우
			double dk = 2.0*(1.04-dfpy_bond/dfpu_bond);
			dfps_bond   = (ddp_bond==0.0 ? 0.0 : dfpu_bond*(1-dk*dNeuC/ddp_bond));
			//dfps_unbond = (dAp_unbond==0.0 || dle_ave==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond + 900.0*(ddp_unbond-dNeuC)/dle_ave, dfpy_unbond));
			dfps_unbond = (dAp_unbond==0.0 ? 0.0 : min(dTpe_unbond/dAp_unbond + 15.0, dfpy_unbond));// SHIN ('06.06.01) : 현재 le값 산출에 문제가 있어 간략식을 대처하나 이후 수정요망
			dApt = dAp_bond + dAp_unbond;
			dfps = (dApt==0.0 ? 0.0 : (dfps_bond*dAp_bond + dfps_unbond*dAp_unbond)/dApt);      
			ddp  = (dApt==0.0 ? 0.0 : (ddp_bond*dAp_bond + ddp_unbond*dAp_unbond)/dApt); 
			
			dEpsi_pt_bond   = (dEps_bond==0.0   ? 0.0 : dfps_bond/dEps_bond);    // dEps_bond가 0인경우는 긴장재가 없거나 탄성계수 입력을 0으로 한 경우이므로 실제는 무한대 값이나 Φ값을 낮추기 위해 0.0으로 만듬      
			dEpsi_pt_unbond = (dEps_unbond==0.0 ? 0.0 : dfps_unbond/dEps_unbond);// dEps_unbond가 0인경우는 긴장재가 없거나 탄성계수 입력을 0으로 한 경우이므로 실제는 무한대 값이나 Φ값을 낮추기 위해 0.0으로 만듬
			dEpsi_pt        = ((dAp_bond + dAp_unbond)==0.0 ? 0.0 : (dEpsi_pt_bond*dAp_bond + dEpsi_pt_unbond*dAp_unbond)/(dAp_bond + dAp_unbond));  
		}
		
		da = min(da, dh);// 콘크리트 압축 면적 계산시 a값은 단면의 높이값을 초과 할 수 없다.
		if(dhf>=da) 
		{
			//dMn = dApt*dfps*(ddp-da/2.0) + dAst*dfy*(ddst-da/2.0) - dAsc*dfy*(ddsc-da/2.0);
			dMn = dApt*dfps*(ddp-dNeuC) + dAst*dfst_Ave*(ddst-dNeuC) - dAsc*dfsc_Ave*(ddsc-dNeuC) - dalpha1*dfck*db*da*(da/2.0-dNeuC); 
			daArea = da*db;
			ddc    = da/2.0;
		}
		else
		{
			//dMn = dApt*dfps*(ddp-da/2.0) + dAst*dfy*(ddst-da/2.0) - dAsc*dfy*(ddsc-da/2.0) + dalpha1*dfck*(db-dbw)*dhf*(da/2.0-dhf/2.0);
			dMn = dApt*dfps*(ddp-dNeuC) + dAst*dfst_Ave*(ddst-dNeuC) - dAsc*dfsc_Ave*(ddsc-dNeuC) - dalpha1*dfck*(db-dbw)*dhf*(dhf/2.0-dNeuC) - dalpha1*dfck*dbw*da*(da/2.0-dNeuC);
			daArea = (db-dbw)*dhf + da*dbw;
			ddc    = (daArea==0.0 ? 0.0 : ((db-dbw)*dhf*dhf/2.0 + dbw*da*da/2.0)/daArea);
		}   
		
		if(Is_CaltransCode())// Caltrans Code
			Get_Phi_PrestressedConcrete_Flexure(dEpsi_pt, arTndnPosi, dphi);
		else                 // AASTO Code (현재(06.12.30) 사용하지 않음)
		{
			double dTemp_Phiv;
			Get_Phi_PrestressedConcrete_AASHTO(MatlElem.MatdD.MainRebarData.B_fy, arRbarPosi, arTndnPosi, dphi, dTemp_Phiv);
		}

		dMn_Cal = dMn;   
		dphiMn = dphi*dMn;    
		dphiMn_Cal = dphiMn;
		// 기타    
		dCc  = dalpha1*dfck*daArea;  
		dTp_bond = dfps_bond*dAp_bond;
		dTp_unbond = dfps_unbond*dAp_unbond;
		dTp  = dTp_bond + dTp_unbond;
		dTst = dAst * dfst_Ave;
		dCsc = dAsc * dfsc_Ave;
	}  

	//균열검토
	double dSc  ; // Sc  탄성상태(비균열)에서 합성단면의 단면계수(콘크리트+철근+긴장재의 단면계수)
	double dSrc ;
	if(bPositive)
	{
		if(bYaxis)
		{	dSc  = dzCen==0.0   ? 0.0 : dIner/dzCen;
			dSrc = dzCen_g==0.0 ? 0.0 : dIner_g/dzCen_g;	}
		else
		{	dSc  = dyCen==0.0   ? 0.0 : dIner/dyCen;
			dSrc = dyCen_g==0.0 ? 0.0 : dIner_g/dyCen_g;	}
	}
	else
	{
		if(bYaxis)
		{	dSc  = dCpz==0.0   ? 0.0 : dIner/dCpz;
			dSrc = dCpz_g==0.0 ? 0.0 : dIner_g/dCpz_g; }
		else
		{	dSc  = dCpy==0.0   ? 0.0 : dIner/dCpy;
			dSrc = dCpy_g==0.0 ? 0.0 : dIner_g/dCpy_g; }
	}
	double dfr  = 0.37*sqrt(dfck); // 콘크리트의 균열강도 : AASHTO LRFD 5.4.2.6
	double dfcpe = (dArea_g*dSrc == 0.0) ? 0.0 : dPe/dArea_g + (bPositive ? 1.0 : -1.0)*dMe/dSrc; // 탄성상태(비균열)에서 유효PS력에 의한 콘크리트의 응력
	double dMcr = max(dSc*(dfr+dfcpe), dSc*dfr);                                       


	// 계산값 정리 ( SHIN ('06.05.31) : dMn, dde, ddv 이외에는 값산출Test목적임)
	FlexData.bStrainCompatibility = bIter;
	FlexData.dphi        = dphi;
	FlexData.dMn         = dMn_Cal;
	FlexData.dphiMn      = dphiMn_Cal;
	FlexData.dTp         = dTp;
	FlexData.dTp_bond    = dTp_bond;
	FlexData.dTp_unbond  = dTp_unbond;
	FlexData.dTst        = dTst;
	FlexData.dCsc        = dCsc;
	FlexData.dCc         = dCc;
	FlexData.dfps        = dfps;
	FlexData.dfps_bond   = dfps_bond;
	FlexData.dfps_unbond = dfps_unbond;
	FlexData.dfst        = (dAst==0.0 ? 0.0 : dTst/dAst);
	FlexData.dfsc        = (dAsc==0.0 ? 0.0 : dCsc/dAsc);
	FlexData.dfcc        = (daArea==0.0 ? 0.0 : dCc/daArea);
	FlexData.ddc         = ddc;
	FlexData.ddp         = ddp;
	FlexData.ddp_bond    = ddp_bond;
	FlexData.ddp_unbond  = ddp_unbond;
	FlexData.ddst        = ddst;
	FlexData.ddsc        = ddsc;
	FlexData.dAp         = dApt;
	FlexData.dAp_bond    = dAp_bond;
	FlexData.dAp_unbond  = dAp_unbond;
	FlexData.dAst        = dAst;
	FlexData.dAsc        = dAsc;
	FlexData.dAc         = daArea;
	FlexData.dNeuC       = dNeuC;
	FlexData.dH          = dh;
	FlexData.da          = da;
	FlexData.dde         = min(dh, ((dTp+dTst)==0.0 ? 0.0 : (dTp*ddp + dTst*ddst)/(dTp+dTst)));
	FlexData.ddv         = min(dh, ((dCc+dCsc)==0.0 ? 0.0 : FlexData.dde - (dCc*ddc + dCsc*ddsc)/(dCc+dCsc)));
	FlexData.iNum_bond   = iBondNum;
	FlexData.iNum_unbond = iUnbondNum;
	FlexData.bOneType    = bOneType;
	FlexData.bBondType   = bBondType;
	FlexData.iCount      = iCount;
	//
	FlexData.dMcr        = dMcr;	
	FlexData.dfr         = dfr;
	FlexData.dfcpe       = dfcpe;  
	FlexData.dSc         = dSc;   

	//Caltrans요구로 추가한 변수(06.12.15 mail)
	FlexData.dfck        = dfck;                                       
	FlexData.dfy         = dfy;                                        
	if(bOneType)
	{
		if(bBondType)
		{ FlexData.dfpu = dfpu_bond;     FlexData.dfpy = dfpy_bond; }
		else
		{ FlexData.dfpu = dfpu_unbond;   FlexData.dfpy = dfpy_unbond; }
	}
	else
	{
		FlexData.dfpu = min(dfpu_bond, dfpu_unbond);   
		FlexData.dfpy = min(dfpy_bond, dfpy_unbond); 
	}
	FlexData.dB_flange = db;                       
	FlexData.dB_web    = dbw;                      
	FlexData.dThick_Top = dFlgThk[0];              
	FlexData.dThick_Bottom = dFlgThk[1];           

	return TRUE;
}

BOOL CDgnBridgeLoadRating::Get_ShearDesign(BOOL bSegmBoxGirder, BOOL bFlexure, BOOL bYaxis, BOOL bIter, double dMu, double dVu, double dVp_TP, double da_g, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, 
											 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, _DBLR_SHEAR_DATA& ShearData)
{
	if(bSegmBoxGirder)
	{
		if(!Get_ShearDesign_WithSegmentalBoxGirderBridge(bFlexure, bYaxis, bIter, dMu, dVu, dVp_TP, MatlElem, SpscSect, SpscgSect, arOuter, arInner, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, RpscSbar, ShearData))
			return FALSE;
	}
	else
	{// (SHIN, 07.03.09) : β, θ값 계산시에 반복계산을 통해서 산출하도록 bIterBT=TRUE적용
		if(!Get_ShearDesign_WithoutSegmentalBoxGirderBridge(bFlexure, bYaxis, bIter, TRUE, dMu, dVu, dVp_TP, da_g, MatlElem, SpscSect, SpscgSect, arOuter, arInner, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, RpscSbar, ShearData))
			return FALSE;
	}
	return TRUE;
}
BOOL CDgnBridgeLoadRating::Get_ShearDesign_WithoutSegmentalBoxGirderBridge(BOOL bFlexure, BOOL bYaxis, BOOL bIter, BOOL bIterBT, double dMu, double dVu, double dVp_TP, double da_g, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, 
											 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, _DBLR_SHEAR_DATA& ShearData)
{
	ShearData.Initialize(); //초기화
	BOOL bPositive = (dMu >= -m_dZero);
	//Material
	double dfck = MatlElem.MatdD.Data1.Design.C_fc;     
	double dEc  = MatlElem.MatdD.Data1.Analysis.Elast; 
	double dfy  = MatlElem.MatdD.MainRebarData.B_fy;  
	double dEs  = MatlElem.MatdD.MainRebarData.B_Elast; 
	double dfys = MatlElem.MatdD.SubRebarData.B_fy;
	double dfyp = dfys;
	//Section - 철근이 고려된 한산단면적
	double dArea = SpscSect.dArea;
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCen  = (bYaxis==TRUE ? dzCen : dyCen);
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dIyy  = SpscSect.dIyy;
	double dIzz  = SpscSect.dIzz;
	double dIner = (bYaxis==TRUE ? dIyy : dIzz);

	double dArea_g = SpscgSect.dArea;
	double dyDim_g = SpscgSect.dyDim;
	double dzDim_g = SpscgSect.dzDim;
	double dyCen_g = SpscgSect.dyCen;
	double dzCen_g = SpscgSect.dzCen;
	double dCen_g  = (bYaxis==TRUE ? dzCen_g : dyCen_g);
	double dCpy_g  = dyDim_g - dyCen_g;
	double dCpz_g  = dzDim_g - dzCen_g;
	double dIyy_g  = SpscgSect.dIyy;
	double dIzz_g  = SpscgSect.dIzz;
	double dIner_g = (bYaxis==TRUE ? dIyy_g : dIzz_g);

	double dh    = (bYaxis==TRUE ? dzDim : dyDim);
	double dCp;
	if(bPositive) dCp = (bYaxis==TRUE ? dCpz_g : dCpy_g);
	else          dCp = (bYaxis==TRUE ? dzCen_g: dyCen_g);
	double dhf   = (bPositive ? dFlgThk[0] : dFlgThk[1]); 
	double dLength = (bYaxis==TRUE ? dyDim : dzDim);

	int NumberofTendon = arTndnPosi.GetSize();
	int NumberofRebar  = arRbarPosi.GetSize();
	int i=0;

	//강도계수 산출
	double dphiv = 0.9;
	Get_Phi_PrestressedConcrete_Shear(dphiv);


	// 전단철근
	double dsv = RpscSbar.dSbarPitch;  
	double dAv = (dsv==0.0 ? 0.0 : RpscSbar.dSbarAw);
	double dAlpha = RpscSbar.dSbarTheta;// 수직이 90도(단위:radian)
	
	//최소철근량 계산
	double dAvmin = (dfy==0.0 ? 0.0 : 0.0316*sqrt(dfck)*dbw*dsv/dfys);

	//유효프리스트레스력 수직분력 및 Flexural tensin side(h/2 아래)에 위치한 긴장재와 철근의 면적, 콘크리트면적 산출 산출
	double dVp = fabs(dVp_TP); 
	double dVu_dVp   = fabs(dVu)-dVp;
	double dVu_phiVp = fabs(dVu)-dphiv*dVp;
	double dEp=0.0, dAps=0.0, dAst=0.0;
	double dfpu = 1.0/m_dZero;
	double dAc=0.0;
	for(i=0 ; i<NumberofTendon ; i++)
	{
		_DGN_TNDN_CRC rTnInf = arTndnPosi.GetAt(i);   
		if(rTnInf.dArea < m_dZero) continue; // 단면이 음수일 경우 검토 대상에서 제외
		double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);     
		//dVp += rTnInf.dArea * rTnInf.dStress[2];

		if((bPositive && dposi<=dh/2.0-dCen_g) || (!bPositive && dposi>=dh/2.0-dCen_g)) // h/2 지점을 기준으로 인장측만 고려
		{
			dEp += rTnInf.dArea * rTnInf.dEps;
			dAps += rTnInf.dArea;
			dfpu = min(rTnInf.dfpu, dfpu);
		}
	}
	dEp = (dAps==0.0 ? 0.0 : dEp/dAps);
	for(i=0 ; i<NumberofRebar ; i++)
	{
		_DGN_RBAR_CRC rReInf = arRbarPosi.GetAt(i);
		double dposi = (bYaxis==TRUE ? rReInf.dz : rReInf.dy); 
		if((bPositive && dposi<=dh/2.0-dCen_g) || (!bPositive && dposi>=dh/2.0-dCen_g)) // h/2 지점을 기준으로 인장측만 고려
			dAst += rReInf.dArea;    
	}
	BOOL   bDirec = (bPositive ? FALSE : TRUE);// 인장측을 찾으므로
	double daArea=0.0, daQ=0.0;
	if(CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, dh/2.0, dLength, daArea, daQ))
		dAc = daArea;
	else
	{ ASSERT(0); }
	

	//dv산출
	double ddv =  0.0;
	double dde = -1.0;
	if(bFlexure)
	{//휨강도 계산시 산출된 값 사용
		_DBLR_FLEX_DATA FlexData;
		Get_FlexureStrength(bYaxis, bIter, bPositive, MatlElem, SpscSect, SpscgSect, arOuter, arInner, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, FlexData);
		dde = FlexData.dde;
		ddv = max(FlexData.ddv, max(0.9*dde, 0.72*dh));
	}
	else
		ddv = 0.72*dh;
		
	
	//β,θ값 산출
	//  초기화
	double dBeta=2.0;
	double dTheta=45.0/180.0 * DBLR_PI;
	double dEpsi_x=0.0;
	if(dAv >= dAvmin)
		dEpsi_x = ((dEs*dAst+dEp*dAps)==0.0 ? 0.0 : (fabs(dMu)/ddv + (0.5/tan(dTheta))*fabs(dVu_dVp)-dAps*0.7*dfpu)/2.0/(dEs*dAst+dEp*dAps));
	else
		dEpsi_x = ((dEs*dAst+dEp*dAps)==0.0 ? 0.0 : (fabs(dMu)/ddv + (0.5/tan(dTheta))*fabs(dVu_dVp)-dAps*0.7*dfpu)/(dEs*dAst+dEp*dAps));

	if(bIterBT)
	{// 반복계산을 통한 β,θ값 산출		
		double bCheck = FALSE; // 반복계산으로 수렴하였는지 여부
		double dCotTheta05 = 1.0;// SHIN('06.06.01) 간략식으로 0.5 x cotθ = 1.0 을 초기값으로 사용 : C5.8.3.4.2 참조
		dTheta = 0.463648; // 약26.565˚: 0.5 x cotθ = 1.0 을 만족하기 위한 θ값
		double dBeta_before, dTheta_before, dEpsi_x_before;
		for(int k=0 ; k<20 ; ++k)
		{
			double bCheckAv = FALSE;
			dBeta_before = dBeta;  
			dTheta_before = dTheta;
			dEpsi_x_before = dEpsi_x;
			if(dTheta == 0.0) {	bCheck = FALSE;		break; }//계산불가
			dCotTheta05 = 0.5/tan(dTheta);
			if(dAv >= dAvmin)
			{
				dEpsi_x = ((dEs*dAst+dEp*dAps)==0.0 ? 0.0 : (fabs(dMu)/ddv + dCotTheta05*fabs(dVu_dVp)-dAps*0.7*dfpu)/2.0/(dEs*dAst+dEp*dAps));
				if(dEpsi_x <= 0.001+m_dZero)
				{
					bCheckAv = TRUE;
					double dv_u = (dphiv*dbw*ddv==0.0 ? 0.0 : fabs(dVu_phiVp)/(dphiv*dbw*ddv));
					Get_ThetaAndBeta_TranverseReinforcement(dv_u, dfck, dEpsi_x, dTheta, dBeta);					
				}
			}
			else
			{    
				dEpsi_x = ((dEs*dAst+dEp*dAps)==0.0 ? 0.0 : (fabs(dMu)/ddv + dCotTheta05*fabs(dVu_dVp)-dAps*0.7*dfpu)/(dEs*dAst+dEp*dAps));
				if(dEpsi_x <= 0.002+m_dZero)
				{
					bCheckAv = TRUE;
					double dSxe = min(ddv*1.38/(da_g+0.63), 80.0);      
					Get_ThetaAndBeta_LessThanMinimumTranverseReinforcement(dSxe, dEpsi_x, dTheta, dBeta);
				}
			}
			if(!bCheckAv)
			{
				dEpsi_x = ((dEc*dAc+dEs*dAst+dEp*dAps)==0.0 ? 0.0 : (fabs(dMu)/ddv + dCotTheta05*fabs(dVu_dVp)-dAps*0.7*dfpu)/2.0/(dEc*dAc+dEs*dAst+dEp*dAps));
				double dv_u = (dphiv*dbw*ddv==0.0 ? 0.0 : fabs(dVu_phiVp)/(dphiv*dbw*ddv));
				Get_ThetaAndBeta_TranverseReinforcement(dv_u, dfck, dEpsi_x, dTheta, dBeta);    
			}
			// 검토
			if(fabs(dBeta_before-dBeta) < m_dZero && fabs(dTheta_before-dTheta) < m_dZero)
			{	bCheck = TRUE;		break; }			
		}
		if(!bCheck)
		{//수렴하지 못한 경우
			//ASSERT(0);
			dBeta=(dBeta_before+dBeta)/2.0;   dTheta=(dTheta_before+dTheta)/2.0;   dEpsi_x=dTheta=(dEpsi_x_before+dEpsi_x)/2.0;
		}
	}
	//
	double dVc = 0.0316*dBeta*sqrt(dfck)*dbw*ddv;
	double dCotTheta = (fabs(dTheta-DBLR_PI/2.0) <= m_dZero ? 0.0 : (fabs(tan(dTheta))<=m_dZero ? 1.0/m_dZero : 1.0/tan(dTheta)));
	double dCotAlpha = (fabs(dAlpha-DBLR_PI/2.0) <= m_dZero ? 0.0 : (fabs(tan(dAlpha))<=m_dZero ? 1.0/m_dZero : 1.0/tan(dAlpha)));
	double dSinAlpha = (fabs(dAlpha-DBLR_PI/2.0) <= m_dZero ? 1.0 : (fabs(dAlpha)<=m_dZero ? 0.0 : sin(dAlpha)));
	double dVs = (dsv==0.0 ? 0.0 : fabs(dAv*dfy*ddv*(dCotTheta+dCotAlpha)*dSinAlpha/dsv));
	double dVn = min(dVc+dVs+dVp, 0.25*dfck*dbw*ddv + dVp);

	double dphi = 0.9;
	Get_Phi_PrestressedConcrete_Shear(dphi);
	// 계산값 정리
	ShearData.bSegmentalBoxGirder  = FALSE;
	ShearData.bStrainCompatibility = bIter;
	ShearData.dphi   = dphi;
	ShearData.dMu    = dMu;
	ShearData.dVu    = dVu;
	ShearData.dVn    = dVn;
	ShearData.dVc    = dVc;
	ShearData.dVs    = dVs;
	ShearData.dVp    = dVp;
	ShearData.dphiVn = dphi*dVn;  

	ShearData.dde    = dde; //휨강도 계산시 값을 사용하지 않았을 경우 -1.0이 들어 가게 됨
	ShearData.ddv    = ddv;  
	ShearData.dAv_s  = (dsv==0.0 ? 0.0 : fabs(dAv/dsv));
	ShearData.dex    = dEpsi_x;
	ShearData.dbeta  = dBeta;
	ShearData.dtheta = dTheta;
	return TRUE;
}
	

BOOL CDgnBridgeLoadRating::Get_ShearDesign_WithSegmentalBoxGirderBridge(BOOL bFlexure, BOOL bYaxis, BOOL bIter, double dMu, double dVu, double dVp_TP, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, 
													 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, _DBLR_SHEAR_DATA& ShearData)
{  
	ShearData.Initialize(); //초기화
	BOOL bPositive = (dMu >= -m_dZero);
	//Material
	double dfck= MatlElem.MatdD.Data1.Design.C_fc;     
	double dEc = MatlElem.MatdD.Data1.Analysis.Elast; 
	double dfy = MatlElem.MatdD.MainRebarData.B_fy;    
	//Section - 철근이 고려된 한산단면적
	double dArea = SpscSect.dArea;
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dIyy  = SpscSect.dIyy;
	double dIzz  = SpscSect.dIzz;
	double dIner = (bYaxis==TRUE ? dIyy : dIzz);

	double dArea_g = SpscgSect.dArea;
	double dyDim_g = SpscgSect.dyDim;
	double dzDim_g = SpscgSect.dzDim;
	double dyCen_g = SpscgSect.dyCen;
	double dzCen_g = SpscgSect.dzCen;
	double dCpy_g  = dyDim_g - dyCen_g;
	double dCpz_g  = dzDim_g - dzCen_g;
	double dIyy_g  = SpscgSect.dIyy;
	double dIzz_g  = SpscgSect.dIzz;
	double dIner_g = (bYaxis==TRUE ? dIyy_g : dIzz_g);

	double dh    = (bYaxis==TRUE ? dzDim : dyDim);
	double dCp;
	if(bPositive) dCp = (bYaxis==TRUE ? dCpz_g : dCpy_g);
	else          dCp = (bYaxis==TRUE ? dzCen_g: dyCen_g);
	double dhf   = (bPositive ? dFlgThk[0] : dFlgThk[1]); 

	int NumberofTendon = arTndnPosi.GetSize();
	int i=0;

	double dK = 0.0;
	double dPe=0.0, dMe=0.0, dep=0.0; //전체 프리스트레스력, 전체 프리스트레스력에 의한 균열전도심에서의 모멘트, 편심거리 
	double dfpc = 0.0;
	double dei = 0.0;// fpc를 검토할 위치
	//if(dhf > dCp) (bPositive ? dCp-dhf : dhf-dCp);
	
	//강도계수 산출
	double dphiv = 0.9;
	Get_Phi_PrestressedConcrete_Shear(dphiv);

	double dVn=0.0, dVc=0.0, dVs=0.0;
	
	for(i=0 ; i<NumberofTendon ; i++)
	{// 손실후 유효프리스트레스력만 작용시의 전체 프리스트레스력과 균열전도심(환산단면도심)에 대한 편심거리 산출 
		_DGN_TNDN_CRC rTnInf = arTndnPosi.GetAt(i);
		if(rTnInf.dArea < m_dZero) continue; // 단면이 음수일 경우 검토 대상에서 제외
		double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);        
		dPe += rTnInf.dArea * rTnInf.dStress[0];
		dMe += rTnInf.dArea * rTnInf.dStress[0] * dposi;
	}
	dep = (dPe==0 ? 0.0: dMe/dPe);// 부호값을 가짐(+:상or우, -:하or좌)  
	dfpc = (dArea_g==0.0 || dEc==0.0 || dIner_g==0.0) ? 0.0 : dPe/dArea_g*(1.0+(dep*dei)/(dIner_g/dArea_g));
	dK = sqrt(1.0 + dfpc/0.125/sqrt(dfck));
	//Mu상태에서의 인장단의 응력 검토  
	double dfct1 = ((dIner_g/dArea_g)==0.0 ? 0.0 : dPe/dArea+(dMu+dMe)*(dCp-dh)/(dIner_g/dArea_g));
	double dfct2 = ((dIner_g/dArea_g)==0.0 ? 0.0 : dPe/dArea+(dMu+dMe)*(dh-dCp)/(dIner_g/dArea_g));
	double dfct  = min(dfct1, dfct2);//균열전의 도심에서 인장 연단에서의 최대 인장응력
	if(dfct < -0.19*sqrt(dfck)) dK = min(dK, 1.0);
	else                        dK = min(dK, 2.0);


	//dv산출
	double ddv=  0.0;
	double dde= -1.0; 
	if(bFlexure)
	{//휨강도 계산시 산출된 값 사용
		_DBLR_FLEX_DATA FlexData;
		Get_FlexureStrength(bYaxis, bIter, bPositive, MatlElem, SpscSect, SpscgSect, arOuter, arInner, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, FlexData);
		dde = FlexData.dde;
		ddv = max(FlexData.ddv, max(0.9*dde, 0.72*dh));
	}
	else
		ddv = 0.72*dh;  
	//콘크리트의 전단강도 계산    
	dVc = 0.0316*dK*sqrt(dfck)*dbw*ddv;
	//철근의 전단강도 계산
	double dsv = RpscSbar.dSbarPitch;  
	double dAv = (dsv==0.0 ? 0.0 : RpscSbar.dSbarAw);
	double dAlpha = RpscSbar.dSbarTheta;// 수직이 90도(단위:radian)
	double dCotAlpha = (fabs(dAlpha-DBLR_PI/2.0) <= m_dZero ? 0.0 : (fabs(tan(dAlpha))<=m_dZero ? 1.0/m_dZero : 1.0/tan(dAlpha)));
	double dSinAlpha = (fabs(dAlpha-DBLR_PI/2.0) <= m_dZero ? 1.0 : (fabs(dAlpha)<=m_dZero ? 0.0 : sin(dAlpha)));
	dVs = (dsv==0.0 ? 0.0 : fabs(dAv*dfy*ddv*(1.0+dCotAlpha)*dSinAlpha/dsv));  
	//공칭 전단강도 계산
	dVn = min(dVc+dVs, 0.379*sqrt(dfck)*dbw*ddv);
	
	double dphi = 0.9;
	Get_Phi_PrestressedConcrete_Shear(dphi);

	// 계산값 정리
	ShearData.bSegmentalBoxGirder  = TRUE;
	ShearData.bStrainCompatibility = bIter;
	ShearData.dphi   = dphi;
	ShearData.dMu    = dMu;
	ShearData.dVu    = dVu;
	ShearData.dVn    = dVn;
	ShearData.dVc    = dVc;
	ShearData.dVs    = dVs;
	ShearData.dphiVn = dphiv*dVn;  

	ShearData.dde    = dde; //휨강도 계산시 값을 사용하지 않았을 경우 -1.0이 들어 가게 됨
	ShearData.ddv    = ddv;  
	ShearData.dAv_s  = (dsv==0.0 ? 0.0 : fabs(dAv/dsv));  
 

	return TRUE;
}

void CDgnBridgeLoadRating::Get_Phi_PrestressedConcrete_AASHTO(double dfy, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dPhib, double& dPhiv)
{
	int i = 0;  
	double dApfpy  = 0.0;
	double dAsfsy  = 0.0;
	int iNumRebar  = arRbarPosi.GetSize();
	int iNumTendon = arTndnPosi.GetSize();
	for(i=0 ; i<iNumRebar ; i++)
		dAsfsy += arRbarPosi[i].dArea*dfy;
	for(i=0 ; i<iNumTendon ; i++)
	{
		if(arTndnPosi[i].dArea < m_dZero) continue; // 단면이 음수일 경우 검토 대상에서 제외
		dApfpy += arTndnPosi[i].dArea*arTndnPosi[i].dfpy;
	}

	double PPR = ((dAsfsy+dApfpy) == 0.0 ? 0.0 : dApfpy/(dAsfsy+dApfpy));
	dPhib = 0.9 + 0.1*PPR;//간장재가 없는 경우 PPR=0.0이므로 RC(Φ=0.9)로 봄
	dPhiv = 0.9;    
}

void CDgnBridgeLoadRating::Get_Phi_PrestressedConcrete_Flexure(double dEpsi_pt, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dPhib)
{
	int i = 0;  
	double dApfpy  = 0.0;
	int iNumTendon = arTndnPosi.GetSize();
	for(i=0 ; i<iNumTendon ; i++)
	{
		if(arTndnPosi[i].dArea < m_dZero) continue; // 단면이 음수일 경우 검토 대상에서 제외
		dApfpy += arTndnPosi[i].dArea*arTndnPosi[i].dfpy;
	}
	
	if(dApfpy > 0.0)
	{
		if(dEpsi_pt <= 0.002) 
			dPhib = 0.75;
		else if(dEpsi_pt >= 0.005)
			dPhib = 0.95;
		else
			dPhib = 0.75+200/3.0*(dEpsi_pt - 0.002);  
	}
	else
		dPhib = 0.9;//간장재가 없는 경우 RC로 봄
}
void CDgnBridgeLoadRating::Get_Phi_PrestressedConcrete_Shear(double& dPhiv)
{
	dPhiv = 0.9;
}

double CDgnBridgeLoadRating::Get_Beta1(double dfck)
{//α = β1*c에서 4.0ksi까지는 0.85, 4.0ksi이상에서는 1.0ksi마다 0.05씩 감소하며, 단 0.65 이상 이어야 함
	// Factor for stress block width.
	double dBeta=0.0;

	if(dfck <= 4.0)	dBeta = 0.85;
	else						dBeta = max(0.85-0.05*(dfck-4.0), 0.65);
	
	return dBeta;
}

double CDgnBridgeLoadRating::Get_StressOfTendon(double df_py, double df_pu, double dEp, double dEpsi_ps)
{
	double dfp=0.0;  
	if(df_pu<=(250.0-m_dZero)) // SWPC 7A
	{
		if(dEpsi_ps<=(0.0076+m_dZero)) dfp = dEpsi_ps*dEp;
		else                           dfp = (250.0-0.04/(dEpsi_ps-0.0064));       
	}
	else // SWPC 7B
	{
		if(dEpsi_ps<=(0.0086+m_dZero)) dfp = dEpsi_ps*dEp;
		else                           dfp = (270.0-0.04/(dEpsi_ps-0.007));
	}
	if(dfp>df_pu) dfp = df_pu;
	return dfp;  
}

double CDgnBridgeLoadRating::Get_EffectiveTendonLength(_DGN_TNDN_CRC& TendonData)
{
	double dNs = 0.0; // 긴장재 고정점 사이의 지지점이 개수 SHIN ('06.05.27) : 현재 값을 받아오기 힘드므로 
	double dLi = 3000;// 임시값 SHIN ('06.05.27)
	return 2*dLi/(2.0+dNs);
}

BOOL CDgnBridgeLoadRating::Get_ThetaAndBeta_TranverseReinforcement(double dvu, double dfck, double dEpsilon_x, double& dTheta, double& dBeta)
{
	if(dfck <= 0) return FALSE;
	double dStressRatio = dvu/dfck; // vu/fck

	if     (dEpsilon_x <= -0.0002)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 22.3;   dBeta = 6.32; }
		else if(dStressRatio <= 0.100)    { dTheta = 18.1;   dBeta = 3.79; }
		else if(dStressRatio <= 0.125)    { dTheta = 19.9;   dBeta = 3.18; }
		else if(dStressRatio <= 0.150)    { dTheta = 21.6;   dBeta = 2.88; }
		else if(dStressRatio <= 0.175)    { dTheta = 23.2;   dBeta = 2.73; }
		else if(dStressRatio <= 0.200)    { dTheta = 24.7;   dBeta = 2.63; }
		else if(dStressRatio <= 0.225)    { dTheta = 26.1;   dBeta = 2.53; }
		else/* 0.225 보다 큰경우 */       { dTheta = 27.5;   dBeta = 2.39; }
	}
	else if(dEpsilon_x <= -0.0001)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 20.4;   dBeta = 4.75; }
		else if(dStressRatio <= 0.100)    { dTheta = 20.4;   dBeta = 3.38; }
		else if(dStressRatio <= 0.125)    { dTheta = 21.9;   dBeta = 2.99; }
		else if(dStressRatio <= 0.150)    { dTheta = 23.3;   dBeta = 2.79; }
		else if(dStressRatio <= 0.175)    { dTheta = 24.7;   dBeta = 2.66; }
		else if(dStressRatio <= 0.200)    { dTheta = 26.1;   dBeta = 2.59; }
		else if(dStressRatio <= 0.225)    { dTheta = 27.3;   dBeta = 2.45; }
		else/* 0.225 보다 큰경우 */       { dTheta = 28.6;   dBeta = 2.39; }
	}
	else if(dEpsilon_x <= -0.00005)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 21.0;   dBeta = 4.10; }
		else if(dStressRatio <= 0.100)    { dTheta = 21.4;   dBeta = 3.24; }
		else if(dStressRatio <= 0.125)    { dTheta = 22.8;   dBeta = 2.94; }
		else if(dStressRatio <= 0.150)    { dTheta = 24.2;   dBeta = 2.78; }
		else if(dStressRatio <= 0.175)    { dTheta = 25.5;   dBeta = 2.65; }
		else if(dStressRatio <= 0.200)    { dTheta = 26.7;   dBeta = 2.52; }
		else if(dStressRatio <= 0.225)    { dTheta = 27.9;   dBeta = 2.42; }
		else/* 0.225 보다 큰경우 */       { dTheta = 29.1;   dBeta = 2.33; }
	}
	else if(dEpsilon_x <= 0.0)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 21.8;   dBeta = 3.75; }
		else if(dStressRatio <= 0.100)    { dTheta = 22.5;   dBeta = 3.14; }
		else if(dStressRatio <= 0.125)    { dTheta = 23.7;   dBeta = 2.87; }
		else if(dStressRatio <= 0.150)    { dTheta = 25.0;   dBeta = 2.72; }
		else if(dStressRatio <= 0.175)    { dTheta = 26.2;   dBeta = 2.60; }
		else if(dStressRatio <= 0.200)    { dTheta = 27.4;   dBeta = 2.51; }
		else if(dStressRatio <= 0.225)    { dTheta = 28.5;   dBeta = 2.40; }
		else/* 0.225 보다 큰경우 */       { dTheta = 29.7;   dBeta = 2.33; }
	}
	else if(dEpsilon_x <= 0.000125)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 24.3;   dBeta = 3.24; }
		else if(dStressRatio <= 0.100)    { dTheta = 24.9;   dBeta = 2.91; }
		else if(dStressRatio <= 0.125)    { dTheta = 25.9;   dBeta = 2.74; }
		else if(dStressRatio <= 0.150)    { dTheta = 26.9;   dBeta = 2.60; }
		else if(dStressRatio <= 0.175)    { dTheta = 28.0;   dBeta = 2.52; }
		else if(dStressRatio <= 0.200)    { dTheta = 29.0;   dBeta = 2.43; }
		else if(dStressRatio <= 0.225)    { dTheta = 30.0;   dBeta = 2.34; }
		else/* 0.225 보다 큰경우 */       { dTheta = 30.6;   dBeta = 2.12; }
	}
	else if(dEpsilon_x <= 0.00025)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 26.6;   dBeta = 2.94; }
		else if(dStressRatio <= 0.100)    { dTheta = 27.1;   dBeta = 2.75; }
		else if(dStressRatio <= 0.125)    { dTheta = 27.9;   dBeta = 2.62; }
		else if(dStressRatio <= 0.150)    { dTheta = 28.8;   dBeta = 2.52; }
		else if(dStressRatio <= 0.175)    { dTheta = 29.7;   dBeta = 2.44; }
		else if(dStressRatio <= 0.200)    { dTheta = 30.6;   dBeta = 2.37; }
		else if(dStressRatio <= 0.225)    { dTheta = 30.8;   dBeta = 2.14; }
		else/* 0.225 보다 큰경우 */       { dTheta = 31.3;   dBeta = 1.93; }
	}
	else if(dEpsilon_x <= 0.0005)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 30.5;   dBeta = 2.59; }
		else if(dStressRatio <= 0.100)    { dTheta = 30.8;   dBeta = 2.50; }
		else if(dStressRatio <= 0.125)    { dTheta = 31.4;   dBeta = 2.42; }
		else if(dStressRatio <= 0.150)    { dTheta = 32.1;   dBeta = 2.36; }
		else if(dStressRatio <= 0.175)    { dTheta = 32.7;   dBeta = 2.28; }
		else if(dStressRatio <= 0.200)    { dTheta = 32.8;   dBeta = 2.14; }
		else if(dStressRatio <= 0.225)    { dTheta = 32.3;   dBeta = 1.86; }
		else/* 0.225 보다 큰경우 */       { dTheta = 32.8;   dBeta = 1.70; }
	}
	else if(dEpsilon_x <= 0.00075)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 33.7;   dBeta = 2.38; }
		else if(dStressRatio <= 0.100)    { dTheta = 34.0;   dBeta = 2.32; }
		else if(dStressRatio <= 0.125)    { dTheta = 34.4;   dBeta = 2.26; }
		else if(dStressRatio <= 0.150)    { dTheta = 34.9;   dBeta = 2.21; }
		else if(dStressRatio <= 0.175)    { dTheta = 35.2;   dBeta = 2.14; }
		else if(dStressRatio <= 0.200)    { dTheta = 34.5;   dBeta = 1.94; }
		else if(dStressRatio <= 0.225)    { dTheta = 34.0;   dBeta = 1.73; }
		else/* 0.225 보다 큰경우 */       { dTheta = 34.3;   dBeta = 1.58; }
	}
	else if(dEpsilon_x <= 0.001+m_dZero)
	{
		if     (dStressRatio <= 0.075)    { dTheta = 36.4;   dBeta = 2.23; }
		else if(dStressRatio <= 0.100)    { dTheta = 36.7;   dBeta = 2.18; }
		else if(dStressRatio <= 0.125)    { dTheta = 37.0;   dBeta = 2.13; }
		else if(dStressRatio <= 0.150)    { dTheta = 37.3;   dBeta = 2.08; }
		else if(dStressRatio <= 0.175)    { dTheta = 36.8;   dBeta = 1.96; }
		else if(dStressRatio <= 0.200)    { dTheta = 36.1;   dBeta = 1.79; }
		else if(dStressRatio <= 0.225)    { dTheta = 35.7;   dBeta = 1.64; }
		else/* 0.225 보다 큰경우 */       { dTheta = 35.8;   dBeta = 1.50; }
	}
	else 
		return FALSE;// 0.001보다 큰경우
	
	dTheta = dTheta/180.0 * DBLR_PI;// Radian단위로 변환

	return TRUE;
}

BOOL CDgnBridgeLoadRating::Get_ThetaAndBeta_LessThanMinimumTranverseReinforcement(double dSxe, double dEpsilon_x, double& dTheta, double& dBeta)
{ 
	if     (dEpsilon_x <= -0.0002)
	{
		if     (dSxe <=  5.0)    { dTheta = 25.4;   dBeta = 6.36; }
		else if(dSxe <= 10.0)    { dTheta = 27.6;   dBeta = 5.78; }
		else if(dSxe <= 15.0)    { dTheta = 29.5;   dBeta = 5.34; }
		else if(dSxe <= 20.0)    { dTheta = 31.2;   dBeta = 4.99; }
		else if(dSxe <= 30.0)    { dTheta = 34.1;   dBeta = 4.46; }
		else if(dSxe <= 40.0)    { dTheta = 36.6;   dBeta = 4.06; }
		else if(dSxe <= 60.0)    { dTheta = 40.8;   dBeta = 3.50; }
		else/* 60보다 큰경우 */  { dTheta = 44.3;   dBeta = 3.10; }
	}
	else if(dEpsilon_x <= -0.0001)
	{
		if     (dSxe <=  5.0)    { dTheta = 25.5;   dBeta = 6.06; }
		else if(dSxe <= 10.0)    { dTheta = 27.6;   dBeta = 5.78; }
		else if(dSxe <= 15.0)    { dTheta = 29.5;   dBeta = 5.34; }
		else if(dSxe <= 20.0)    { dTheta = 31.2;   dBeta = 4.99; }
		else if(dSxe <= 30.0)    { dTheta = 34.1;   dBeta = 4.46; }
		else if(dSxe <= 40.0)    { dTheta = 36.6;   dBeta = 4.06; }
		else if(dSxe <= 60.0)    { dTheta = 40.8;   dBeta = 3.50; }
		else/* 60보다 큰경우 */  { dTheta = 44.3;   dBeta = 3.10; }
	}
	else if(dEpsilon_x <= -0.00005)
	{
		if     (dSxe <=  5.0)    { dTheta = 25.9;   dBeta = 5.56; }
		else if(dSxe <= 10.0)    { dTheta = 28.3;   dBeta = 5.38; }
		else if(dSxe <= 15.0)    { dTheta = 29.7;   dBeta = 5.27; }
		else if(dSxe <= 20.0)    { dTheta = 31.2;   dBeta = 4.99; }
		else if(dSxe <= 30.0)    { dTheta = 34.1;   dBeta = 4.46; }
		else if(dSxe <= 40.0)    { dTheta = 36.6;   dBeta = 4.06; }
		else if(dSxe <= 60.0)    { dTheta = 40.8;   dBeta = 3.50; }
		else/* 60보다 큰경우 */  { dTheta = 44.3;   dBeta = 3.10; }
	}
	else if(dEpsilon_x <= 0.0)
	{
		if     (dSxe <=  5.0)    { dTheta = 26.4;   dBeta = 5.15; }
		else if(dSxe <= 10.0)    { dTheta = 29.3;   dBeta = 4.89; }
		else if(dSxe <= 15.0)    { dTheta = 31.1;   dBeta = 4.73; }
		else if(dSxe <= 20.0)    { dTheta = 32.3;   dBeta = 4.61; }
		else if(dSxe <= 30.0)    { dTheta = 34.2;   dBeta = 4.43; }
		else if(dSxe <= 40.0)    { dTheta = 36.6;   dBeta = 4.06; }
		else if(dSxe <= 60.0)    { dTheta = 40.8;   dBeta = 3.50; }
		else/* 60보다 큰경우 */  { dTheta = 44.3;   dBeta = 3.10; }
	}
	else if(dEpsilon_x <= 0.000125)
	{
		if     (dSxe <=  5.0)    { dTheta = 27.7;   dBeta = 4.41; }
		else if(dSxe <= 10.0)    { dTheta = 31.6;   dBeta = 4.05; }
		else if(dSxe <= 15.0)    { dTheta = 34.1;   dBeta = 3.82; }
		else if(dSxe <= 20.0)    { dTheta = 36.0;   dBeta = 3.65; }
		else if(dSxe <= 30.0)    { dTheta = 38.9;   dBeta = 3.39; }
		else if(dSxe <= 40.0)    { dTheta = 41.2;   dBeta = 3.20; }
		else if(dSxe <= 60.0)    { dTheta = 44.5;   dBeta = 2.92; }
		else/* 60보다 큰경우 */  { dTheta = 47.1;   dBeta = 2.71; }
	}
	else if(dEpsilon_x <= 0.00025)
	{
		if     (dSxe <=  5.0)    { dTheta = 28.9;   dBeta = 3.91; }
		else if(dSxe <= 10.0)    { dTheta = 33.5;   dBeta = 3.52; }
		else if(dSxe <= 15.0)    { dTheta = 36.5;   dBeta = 3.28; }
		else if(dSxe <= 20.0)    { dTheta = 38.8;   dBeta = 3.09; }
		else if(dSxe <= 30.0)    { dTheta = 42.3;   dBeta = 2.82; }
		else if(dSxe <= 40.0)    { dTheta = 45.0;   dBeta = 2.62; }
		else if(dSxe <= 60.0)    { dTheta = 49.2;   dBeta = 2.32; }
		else/* 60보다 큰경우 */  { dTheta = 52.3;   dBeta = 2.11; }
	}
	else if(dEpsilon_x <= 0.0005)
	{
		if     (dSxe <=  5.0)    { dTheta = 30.9;   dBeta = 3.26; }
		else if(dSxe <= 10.0)    { dTheta = 36.3;   dBeta = 2.88; }
		else if(dSxe <= 15.0)    { dTheta = 39.9;   dBeta = 2.64; }
		else if(dSxe <= 20.0)    { dTheta = 42.7;   dBeta = 2.46; }
		else if(dSxe <= 30.0)    { dTheta = 46.9;   dBeta = 2.19; }
		else if(dSxe <= 40.0)    { dTheta = 50.2;   dBeta = 2.00; }
		else if(dSxe <= 60.0)    { dTheta = 55.1;   dBeta = 1.72; }
		else/* 60보다 큰경우 */  { dTheta = 58.7;   dBeta = 1.52; }
	}
	else if(dEpsilon_x <= 0.00075)
	{
		if     (dSxe <=  5.0)    { dTheta = 32.4;   dBeta = 2.86; }
		else if(dSxe <= 10.0)    { dTheta = 38.4;   dBeta = 2.50; }
		else if(dSxe <= 15.0)    { dTheta = 42.4;   dBeta = 2.26; }
		else if(dSxe <= 20.0)    { dTheta = 45.5;   dBeta = 2.09; }
		else if(dSxe <= 30.0)    { dTheta = 50.1;   dBeta = 1.84; }
		else if(dSxe <= 40.0)    { dTheta = 53.7;   dBeta = 1.66; }
		else if(dSxe <= 60.0)    { dTheta = 58.9;   dBeta = 1.40; }
		else/* 60보다 큰경우 */  { dTheta = 62.8;   dBeta = 1.21; }
	}
	else if(dEpsilon_x <= 0.001)
	{
		if     (dSxe <=  5.0)    { dTheta = 33.7;   dBeta = 2.58; }
		else if(dSxe <= 10.0)    { dTheta = 40.1;   dBeta = 2.23; }
		else if(dSxe <= 15.0)    { dTheta = 44.4;   dBeta = 2.01; }
		else if(dSxe <= 20.0)    { dTheta = 47.6;   dBeta = 1.85; }
		else if(dSxe <= 30.0)    { dTheta = 52.6;   dBeta = 1.60; }
		else if(dSxe <= 40.0)    { dTheta = 56.3;   dBeta = 1.43; }
		else if(dSxe <= 60.0)    { dTheta = 61.8;   dBeta = 1.18; }
		else/* 60보다 큰경우 */  { dTheta = 65.7;   dBeta = 1.01; }
	}
	else if(dEpsilon_x <= 0.0015)
	{
		if     (dSxe <=  5.0)    { dTheta = 35.6;   dBeta = 2.21; }
		else if(dSxe <= 10.0)    { dTheta = 42.7;   dBeta = 1.88; }
		else if(dSxe <= 15.0)    { dTheta = 47.4;   dBeta = 1.68; }
		else if(dSxe <= 20.0)    { dTheta = 50.9;   dBeta = 1.52; }
		else if(dSxe <= 30.0)    { dTheta = 56.3;   dBeta = 1.30; }
		else if(dSxe <= 40.0)    { dTheta = 60.2;   dBeta = 1.14; }
		else if(dSxe <= 60.0)    { dTheta = 65.8;   dBeta = 0.92; }
		else/* 60보다 큰경우 */  { dTheta = 69.7;   dBeta = 0.76; }
	}
	else if(dEpsilon_x <= 0.002+m_dZero)
	{
		if     (dSxe <=  5.0)    { dTheta = 37.2;   dBeta = 1.96; }
		else if(dSxe <= 10.0)    { dTheta = 44.7;   dBeta = 1.65; }
		else if(dSxe <= 15.0)    { dTheta = 49.7;   dBeta = 1.46; }
		else if(dSxe <= 20.0)    { dTheta = 53.4;   dBeta = 1.31; }
		else if(dSxe <= 30.0)    { dTheta = 59.0;   dBeta = 1.10; }
		else if(dSxe <= 40.0)    { dTheta = 63.0;   dBeta = 0.95; }
		else if(dSxe <= 60.0)    { dTheta = 68.6;   dBeta = 0.75; }
		else/* 60보다 큰경우 */  { dTheta = 72.4;   dBeta = 0.62; }
	}
	else
		return FALSE;// 0.002보다 큰경우

	dTheta = dTheta/180.0 * DBLR_PI;// Radian단위로 변환
	
	return TRUE; 
}

BOOL CDgnBridgeLoadRating::CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ)
{
	// Outer=CCW, Inn=CW.
	dQ = 0.0;
	dCutArea = 0.0;
	//*************************************
	// Get Polygon by Cutting Line.
	I_PolyMaker* pPolyMaker;
	pPolyMaker = I_PolyMakerFactory::GetInstance()->New_PMaker();
	int i=0, j=0, k=0;    
	// Outer.
	CArray<XGL_3dp,XGL_3dp&> arOutCut;
	for(i=0; i<arOuter.GetSize(); i++)
	{
		T_SPSC_COOR SpscCoor = arOuter[i];
		for(j=0; j<SpscCoor.arSpscPont[0].GetSize(); j++)
		{
			// x,y Coordinate (from Left, Bottom).
			double dx = SpscCoor.arSpscPont[0].GetAt(j);
			double dy = SpscCoor.arSpscPont[1].GetAt(j);
			XGL_3dp OutXGL;
			OutXGL.Set(dx,dy,0.0);
			arOutCut.Add(OutXGL);
		}
	}
	pPolyMaker->SetOutterPoly(arOutCut);
		
	// Inner.
	for(i=0; i<arInner.GetSize(); i++)
	{
		CArray<XGL_3dp,XGL_3dp&> arInnCut;
		T_SPSC_COOR_AR* apSpscCoor = arInner[i];
		for(j=0; j<apSpscCoor->GetSize(); j++)
		{
			arInnCut.RemoveAll();
			T_SPSC_COOR SpscCoor = apSpscCoor->GetAt(j);
			for(k=0; k<SpscCoor.arSpscPont[0].GetSize(); k++)
			{
				// x,y Coordinate (from Left, Bottom).
				double dx = SpscCoor.arSpscPont[0].GetAt(k);
				double dy = SpscCoor.arSpscPont[1].GetAt(k);
				XGL_3dp InnXGL;
				InnXGL.Set(dx,dy,0.0);
				arInnCut.Add(InnXGL);
			}
			pPolyMaker->AddInnerPoly(arInnCut);
		}
	}

	double dZero=1.0E-07;
	// Make Polygon by Cutting Line.
	// Get Left Part of Cutting Line (Sta->End).
	XGL_3dp StaXGL, EndXGL;
	double dxSta=0.0, dxEnd=0.0, dySta=0.0, dyEnd=0.0;
	if(bDir==1)  // 1: Left->Right , Top->Bot  0: Right->Left, Bot->Top
	{
		 dxSta = (baxis==1 ? (-1)*dZero : dpos);  //1: y-axis 0: z-axis
	   dxEnd = (baxis==1 ? dlen+dZero : dpos);
	   dySta = (baxis==1 ? dpos : dlen+dZero);
	   dyEnd = (baxis==1 ? dpos : (-1)*dZero);
	}
	else
	{
		 dxSta = (baxis==1 ? dlen+dZero : dpos);  //1: y-axis 0: z-axis
	   dxEnd = (baxis==1 ? (-1)*dZero : dpos);
	   dySta = (baxis==1 ? dpos : (-1)*dZero);
	   dyEnd = (baxis==1 ? dpos : dlen+dZero);
	}

	StaXGL.Set(dxSta,dySta,0.0);
	EndXGL.Set(dxEnd,dyEnd,0.0);
	pPolyMaker->SetCuttingLine(StaXGL,EndXGL);
	if(!pPolyMaker->MakeResult())	ASSERT(0);
	// Result Polygon.
	for(j=0; j<pPolyMaker->GetResultPolyCount(); j++)
	{
		CArray<XGL_3dp,XGL_3dp&> arResPoly;
		pPolyMaker->GetResultPoly(j,arResPoly);
		int iResNum = arResPoly.GetSize();
		//---------------------------------
		// Calculate Center(x,y), Area.
		double* dpx = new double[iResNum];
		double* dpy = new double[iResNum];
		for(k=0; k<iResNum; k++)
		{
			dpx[k] = arResPoly[k].x();
			dpy[k] = arResPoly[k].y();
		}
		double dxCen=0.0, dyCen=0.0, dArea=0.0;
		if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
		delete []dpx;
		delete []dpy;
		//---------------------------------
		dCutArea += dArea;
		if(baxis==1)			dQ += dArea*fabs(dyCen-dpos);
		else if(baxis==0)	dQ += dArea*fabs(dxCen-dpos);
		else	ASSERT(0);
	}
	//------------------------------------------------------------------------------
	// Hole 을 얻어 오는것이 별도 함수로 분리 되면서 추가된 부분 
	for(j=0; j<pPolyMaker->GetResultHolePolyCount(); j++)
	{
		CArray<XGL_3dp,XGL_3dp&> arResPoly;
		pPolyMaker->GetResultHolePoly(j,arResPoly);
		int iResNum = arResPoly.GetSize();
		//---------------------------------
		// Calculate Center(x,y), Area.
		double* dpx = new double[iResNum];
		double* dpy = new double[iResNum];
		for(k=0; k<iResNum; k++)
		{
			dpx[k] = arResPoly[k].x();
			dpy[k] = arResPoly[k].y();
		}
		double dxCen=0.0, dyCen=0.0, dArea=0.0;
		if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
		delete []dpx;
		delete []dpy;
		//---------------------------------
		dCutArea -= dArea;
		if(baxis==1)			dQ -= dArea*fabs(dyCen-dpos);
		else if(baxis==0)	dQ -= dArea*fabs(dxCen-dpos);
		else	ASSERT(0);
	}

	I_PolyMakerFactory::GetInstance()->Del_PMaker(pPolyMaker);  
	
	return TRUE;
}


////////////

void CDgnBridgeLoadRating::Prt_Setting(BOOL bPrint)
{
	//++++++++++++++++++++++++++
	// For zzzDataForMphi.txt
	m_bTestDBLR=FALSE;
	m_strTestDBLRFile=_T("");


	m_ElemKey_Flex.first  = -1;
	m_ElemKey_Shear.first = -1;
	m_bNewElemNum_Flex  = TRUE;
	m_bNewElemNum_Shear = TRUE;


	m_bTestDBLR_Release=TRUE;//Caltrans의 요구로 중간값을 text파일로 찍어 주기로 함(06.12.15 mail)

	if(m_bTestMode)
	{
		m_bTestDBLR=TRUE;
	}

	if(m_bTestDBLR || m_bTestDBLR_Release)
	{
		CString strPathName = m_pDoc->GetPathName();
		CFileCtrl FileCtrl(strPathName);
		m_strTestDBLRFile = FileCtrl.GetFilePathAndNameWithoutExtension();  
		if (CDBLib::ActivateGenNXAnal())
			m_strTestDBLRFile.Format(_T("%s_DBLR.xout"), m_strTestDBLRFile);
		else
			m_strTestDBLRFile.Format(_T("%s_DBLR.out"), m_strTestDBLRFile);

		rptwofstream fout(m_strTestDBLRFile, ios::trunc);		
		Prt_ResultData(bPrint, _T("====================================\n   Brigde Load Rating - Design  \n===================================="));
	}  	
	//++++++++++++++++++++++++++
}

void CDgnBridgeLoadRating::Prt_ResultData(BOOL bPrint, CString strTxt)
{
	if((m_bTestDBLR || m_bTestDBLR_Release) && bPrint)
	{
		rptwofstream fout(m_strTestDBLRFile, ios::app);
		CString str1=_T(""); str1.Format(_T("%s")  , strTxt);    
		fout<<str1<<endl;
	}
}

void CDgnBridgeLoadRating::Prt_Debug(BOOL bPrint, CString strTxt)
{
	if(m_bTestDBLR && bPrint)
	{
		rptwofstream fout(m_strTestDBLRFile, ios::app);
		CString str1=_T(""); str1.Format(_T("%s")  , strTxt);    
		fout<<str1<<endl;
	}
}

void CDgnBridgeLoadRating::Prt_FlexData(BOOL bPrint, _DBLR_FLEX_DATA FlexData)
{
	CString strTest = _T("");    
	if(m_bNewElemNum_Flex)
	{
		Prt_ResultData(bPrint, _T(""));
		Prt_ElementNumber(bPrint, FlexData.ElemKey);
		Prt_ResultData(bPrint, _T("[FlexuralStrength-Design]"));
		strTest.Format(_T("Analysis Type : %s"), (FlexData.bStrainCompatibility ? _T("StrainCompatibility") : _T("Code"))); Prt_ResultData(bPrint,strTest);
	}
	Prt_ResultData(bPrint, _T(""));
	Prt_PositionData(bPrint, FlexData.iPosi, FlexData.bPositive);  
		
	Prt_ResultData(bPrint,_T(" <Input data>"));
	Prt_ResultData(bPrint,_T(" *Material strength"));
	strTest.Format(_T("   fck      = %10g   fy       = %10g"), FlexData.dfck, FlexData.dfy);                                                Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   fpy      = %10g   fpu      = %10g"), FlexData.dfpy, FlexData.dfpu);                                               Prt_ResultData(bPrint,strTest);
	Prt_ResultData(bPrint,_T(" *Section data"));
	strTest.Format(_T("   Tension Type : %s"), (FlexData.bOneType ? (FlexData.bBondType ? _T("OneType(Bonded)") : _T("OneType(UnBonded)")) : _T("Bond & Unbond Type"))); Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   Num      = %10d   Num_bond = %10d   Num_unbond= %10d"), FlexData.iNum_bond+FlexData.iNum_unbond, FlexData.iNum_bond, FlexData.iNum_unbond); Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   H        = %10g   t(top)   = %10g   t(bottom) = %10g"), FlexData.dH, FlexData.dThick_Top, FlexData.dThick_Bottom);                 Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   B(flange)= %10g   B(web)   = %10g   Sc        = %10g"), FlexData.dB_flange, FlexData.dB_web, FlexData.dSc);                        Prt_ResultData(bPrint,strTest);
	Prt_ResultData(bPrint,_T(" <Result data>"));
	strTest.Format(_T("   Mn       = %10g   Phi      = %10g   Phi*Mn    = %10g"), FlexData.dMn, FlexData.dphi, FlexData.dphiMn);            Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   Mcr      = %10g")                                     , FlexData.dMcr);                                           Prt_ResultData(bPrint,strTest);
	Prt_ResultData(bPrint,_T(" *Neutral axis"));
	strTest.Format(_T("   a        = %10g   NeuC     = %10g"), FlexData.da,  FlexData.dNeuC);                                               Prt_ResultData(bPrint,strTest); 
	strTest.Format(_T("   de       = %10g   dv       = %10g"), FlexData.dde, FlexData.ddv);                                               Prt_ResultData(bPrint,strTest);

	Prt_ResultData(bPrint,_T(" *Force"));
	strTest.Format(_T("   Tp       = %10g   Tp_bond  = %10g   Tp_unbond = %10g"), FlexData.dTp, FlexData.dTp_bond, FlexData.dTp_unbond);    Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   Cc       = %10g   Tst      = %10g   Csc       = %10g"), FlexData.dCc, FlexData.dTst, FlexData.dCsc);              Prt_ResultData(bPrint,strTest);
	Prt_ResultData(bPrint,_T(" *Area"));
	strTest.Format(_T("   Ap       = %10g   Ap_bond  = %10g   Ap_unbond = %10g"), FlexData.dAp, FlexData.dAp_bond, FlexData.dAp_unbond);    Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   Ac       = %10g   Ast      = %10g   Asc       = %10g"), FlexData.dAc, FlexData.dAst, FlexData.dAsc);              Prt_ResultData(bPrint,strTest);
	Prt_ResultData(bPrint,_T(" *Stress"));
	strTest.Format(_T("   fps      = %10g   fps_bond = %10g   fps_unbond= %10g"), FlexData.dfps, FlexData.dfps_bond, FlexData.dfps_unbond); Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   fcc      = %10g   fst      = %10g   fsc       = %10g"), FlexData.dfcc, FlexData.dfst, FlexData.dfsc);             Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   fcpe     = %10g   fr       = %10g")                   , FlexData.dfcpe, FlexData.dfr);                            Prt_ResultData(bPrint,strTest);
	Prt_ResultData(bPrint,_T(" *Centroid depth"));
	strTest.Format(_T("   dp       = %10g   dp_bond  = %10g   dp_unbond = %10g"), FlexData.ddp, FlexData.ddp_bond, FlexData.ddp_unbond);    Prt_ResultData(bPrint,strTest);
	strTest.Format(_T("   dc       = %10g   dst      = %10g   dsc       = %10g"), FlexData.ddc, FlexData.ddst, FlexData.ddsc);              Prt_ResultData(bPrint,strTest);
		
	strTest.Format(_T("Count = %4d"), FlexData.iCount); Prt_Debug(bPrint,strTest);  //Debug시에만 출력  
}

void CDgnBridgeLoadRating::Prt_ShearData(BOOL bPrint, _DBLR_SHEAR_DATA ShearData)
{
	CString strTest = _T("");  
	if(m_bNewElemNum_Shear)
	{
		Prt_ResultData(bPrint, _T(""));
		Prt_ElementNumber(bPrint, ShearData.ElemKey);
		Prt_ResultData(bPrint, _T("[ShearStrength-Design]"));
		if(ShearData.bSegmentalBoxGirder)
		{
			Prt_ResultData(bPrint,_T("PSC Type : SegmentalBoxGirder"));
			Prt_ResultData(bPrint,_T("Rating Case                                  Part        Mu        Vu        Vn    Phi*Vn        Vc        Vs        Vp     de     dv   Av/s"));
			Prt_ResultData(bPrint,_T("----------------------------------------------------------------------------------------------------------------------------------------------"));
		}
		else
		{
			Prt_ResultData(bPrint,_T("PSC Type : Without SegmentalBoxGirder"));
			Prt_ResultData(bPrint,_T("Rating Case                                  Part        Mu        Vu        Vn    Phi*Vn        Vc        Vs        Vp     de     dv   Av/s     ex   beta  theta(Degree)"));
			Prt_ResultData(bPrint,_T("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------"));
		}    
	}  

	CString strPosi = Get_strPositionData(ShearData.iPosi, ShearData.bPositive);
	if(ShearData.bSegmentalBoxGirder)
	{
		if(ShearData.dde >= 0.0)
		{
			strTest.Format(_T("%s  %s %9g %9g %9g %9g %9g %9g %9g %6g %6g %6g"), 
											ShearData.strRatingCase, strPosi, ShearData.dMu, ShearData.dVu, ShearData.dVn, ShearData.dphiVn, ShearData.dVc, ShearData.dVs, ShearData.dVp, ShearData.dde, ShearData.ddv, ShearData.dAv_s); 
		}
		else
		{
			strTest.Format(_T("%s  %s %9g %9g %9g %9g %9g %9g %9g      - %6g %6g"), 
											ShearData.strRatingCase, strPosi, ShearData.dMu, ShearData.dVu, ShearData.dVn, ShearData.dphiVn, ShearData.dVc, ShearData.dVs, ShearData.dVp, ShearData.ddv, ShearData.dAv_s); 
		}
		Prt_ResultData(bPrint,strTest);
	}
	else
	{
		if(ShearData.dde >= 0.0)
		{
			strTest.Format(_T("%s  %s  %9g %9g %9g %9g %9g %9g %9g %6g %6g %6g %6g %6g %6g"),
											ShearData.strRatingCase, strPosi, ShearData.dMu, ShearData.dVu, ShearData.dVn, ShearData.dphiVn, ShearData.dVc, ShearData.dVs, ShearData.dVp, ShearData.dde, ShearData.ddv, ShearData.dAv_s, ShearData.dex, ShearData.dbeta, ShearData.dtheta*180.0/DBLR_PI);
		}
		else
		{
			strTest.Format(_T("%s  %s  %9g %9g %9g %9g %9g %9g %9g      - %6g %6g %6g %6g %6g"),
											ShearData.strRatingCase, strPosi, ShearData.dMu, ShearData.dVu, ShearData.dVn, ShearData.dphiVn, ShearData.dVc, ShearData.dVs, ShearData.dVp, ShearData.ddv, ShearData.dAv_s, ShearData.dex, ShearData.dbeta, ShearData.dtheta*180.0/DBLR_PI);
		}
		Prt_ResultData(bPrint,strTest);
	}  
}
void CDgnBridgeLoadRating::Prt_ElementNumber(BOOL bPrint, ElemPairK ElemK)
{
	CString strElemK=_T(""); strElemK.Format(_T("+++ %4d Element ++++++++++++++++++++++++++++++++++++++++++++++++++++"), ElemK.first);
	Prt_ResultData(bPrint,strElemK);	
}
void CDgnBridgeLoadRating::Prt_PositionData(BOOL bPrint, int iPosi, BOOL bPositive)
{
	CString strTest = _T("");
	strTest.Format(_T("Position : %s (%s)"), (iPosi==0 ? _T("I") : _T("J")), (bPositive ? _T("Positive") : _T("Negative"))); Prt_ResultData(bPrint,strTest);
}
CString CDgnBridgeLoadRating::Get_strPositionData(int iPosi, BOOL bPositive)
{
	CString strTest = _T("");
	strTest.Format(_T("%s(%s)"), (iPosi==0 ? _T("I") : _T("J")), (bPositive ? _T("+") : _T("-"))); 
	return strTest;
}


CString CDgnBridgeLoadRating::GetRatingCaseString(T_BLRC_K blrcK, BOOL bDc, BOOL bDw, BOOL bT, int iLine)
{
	CString strResult = _T("_");

	if(bDc)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_DC_MAX);
	else
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_DC_MIN);

	if(bDw)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_DW_MAX);
	else
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_DW_MIN);

	if(bT)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_TEMP_MAX);
	else
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_TEMP_MIN);

	if(iLine == 1)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Fxx_Max);
	else if(iLine == 2)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Fxx_Min);
	else if(iLine == 3)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Fyy_Max);
	else if(iLine == 4)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Fyy_Min);
	else if(iLine == 5)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Fzz_Max);
	else if(iLine == 6)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Fzz_Min);
	else if(iLine == 7)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Mxx_Max);
	else if(iLine == 8)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Mxx_Min);
	else if(iLine == 9)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Myy_Max);
	else if(iLine == 10)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Myy_Min);
	else if(iLine == 11)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Mzz_Max);
	else if(iLine == 12)
		strResult += _LS(IDS_DGN_RESULTVIEW_RATING_COLHEAD_Mzz_Min);	
	

	T_BLRC_D BlrcData;
	BlrcData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetBlrc(blrcK, BlrcData))
		return strResult;
	strResult = BlrcData.RatingCaseName + strResult;

	return strResult;
}

CString CDgnBridgeLoadRating::GetStringReSize(int nSize, CString strText)
{
	CString strResult = _T("");
	int i=0;  

	for(i=0 ; i<nSize ; ++i)
		strResult += _T(" ");
	
	int nInSize = strText.GetLength();
	int minSize = min(nInSize, nSize);
	
	for(i=0 ; i<minSize ; ++i)
		strResult.SetAt(i, strText.GetAt(i));
	 
	return strResult;
}