// DgnRationOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRatingOptionDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRationOptionDlg dialog


CDgnRationOptionDlg::CDgnRationOptionDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRationOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRationOptionDlg)
	m_nOption = -1;	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_aGroupBox; m_aGroupBox.RemoveAll();
	m_aGroupBox.Add(IDC_DGN_OPTION_GROUP);
	m_aGroupBox.Add(IDC_DEFINE_GROUP);
	m_aGroupBox.Add(IDC_POSITION_GROUP);
	m_aGroupBox.Add(IDC_FACTOR_GROUP);

	m_aRadioBtn; m_aRadioBtn.RemoveAll();
	m_aRadioBtn.Add(IDC_DGN_AR);
	m_aRadioBtn.Add(IDC_DGN_DEL);
	m_aRadioBtn.Add(IDC_DGN_I_RADIO);
	m_aRadioBtn.Add(IDC_DGN_J_RADIO);
	m_aRadioBtn.Add(IDC_DGN_IJ_RADIO);

	m_aCtrl_I; m_aCtrl_I.RemoveAll();
	m_aCtrl_I.Add(IDC_DGN_STRAIN_I_ST);
	m_aCtrl_I.Add(IDC_DGN_STRAIN_I_EDT);
	m_aCtrl_I.Add(IDC_DGN_STRAIN_I_UNT);
	m_aCtrl_I.Add(IDC_DGN_DROOP_I_STATIC);
	m_aCtrl_I.Add(IDC_DGN_DROOP_EDIT);
	m_aCtrl_I.Add(IDC_DGN_DROOP_I_UNIT);
	m_aCtrl_I.Add(IDC_DGN_IMPACT_FACTOR_I);
	m_aCtrl_I.Add(IDC_DGN_IMPACT_EDIT_I);
	m_aCtrl_I.Add(IDC_DGN_ADD_RESPONSE_FACTOR_I);
	m_aCtrl_I.Add(IDC_DGN_ADD_REPONCE_EDIT_I);

	m_aCtrl_J; m_aCtrl_J.RemoveAll();
	m_aCtrl_J.Add(IDC_DGN_STRAIN_J_ST);
	m_aCtrl_J.Add(IDC_DGN_STRAIN_J_EDT);
	m_aCtrl_J.Add(IDC_DGN_STRAIN_J_UNT);
	m_aCtrl_J.Add(IDC_DGN_DROOP_J_STATIC);
	m_aCtrl_J.Add(IDC_DGN_DROOP_J_EDIT);
	m_aCtrl_J.Add(IDC_DGN_DROOP_J_UNIT);
	m_aCtrl_J.Add(IDC_DGN_IMPACT_FACTOR_J);
	m_aCtrl_J.Add(IDC_DGN_IMPACT_EDIT_J);
	m_aCtrl_J.Add(IDC_DGN_ADD_RESPONSE_FACTOR_J);
	m_aCtrl_J.Add(IDC_DGN_ADD_REPONCE_EDIT_J);

	m_aStrain_I;m_aStrain_I.RemoveAll();
	m_aStrain_I.Add(IDC_DGN_STRAIN_I_ST);
	m_aStrain_I.Add(IDC_DGN_STRAIN_I_EDT);
	m_aStrain_I.Add(IDC_DGN_STRAIN_I_UNT);

	m_aStrain_J;m_aStrain_J.RemoveAll();
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_ST);
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_EDT);
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_UNT);

	m_aDispl_I;m_aDispl_I.RemoveAll();
	m_aDispl_I.Add(IDC_DGN_DROOP_I_STATIC);
	m_aDispl_I.Add(IDC_DGN_DROOP_EDIT);
	m_aDispl_I.Add(IDC_DGN_DROOP_I_UNIT);

	m_aDispl_J;m_aDispl_J.RemoveAll();
	m_aDispl_J.Add(IDC_DGN_DROOP_J_STATIC);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_EDIT);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_UNIT);

	m_aImpact_I;m_aImpact_I.RemoveAll();
	m_aImpact_I.Add(IDC_DGN_IMPACT_FACTOR_I);
	m_aImpact_I.Add(IDC_DGN_IMPACT_EDIT_I);

	m_aImpact_J;m_aImpact_J.RemoveAll();
	m_aImpact_J.Add(IDC_DGN_IMPACT_FACTOR_J);
	m_aImpact_J.Add(IDC_DGN_IMPACT_EDIT_J);

	m_aAddReponce_I;m_aAddReponce_I.RemoveAll();
	m_aAddReponce_I.Add(IDC_DGN_ADD_RESPONSE_FACTOR_I);
	m_aAddReponce_I.Add(IDC_DGN_ADD_REPONCE_EDIT_I);

	m_aAddReponce_J;m_aAddReponce_J.RemoveAll();
	m_aAddReponce_J.Add(IDC_DGN_ADD_RESPONSE_FACTOR_J);
	m_aAddReponce_J.Add(IDC_DGN_ADD_REPONCE_EDIT_J);

	m_aKSCE_USD05;m_aKSCE_USD05.RemoveAll();
	m_aKSCE_USD05.Add(IDC_DGN_FACTOR_STATIC2);
	m_aKSCE_USD05.Add(IDC_DGN_FACTOR_STATIC3);
	m_aKSCE_USD05.Add(IDC_DGN_FACTOR_STATIC4);
	m_aKSCE_USD05.Add(IDC_DGN_TRAFFIT_EDIT);
	m_aKSCE_USD05.Add(IDC_DGN_LOAD_EDIT);
	m_aKSCE_USD05.Add(IDC_DGN_COMPEN_EDIT);

	m_aEtc;m_aEtc.RemoveAll();
	m_aEtc.Add(IDC_DGN_EXECUTE);
	m_aEtc.Add(IDC_DGN_CLOSE);
}


void CDgnRationOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRationOptionDlg)
	DDX_Radio(pDX, IDC_DGN_AR, m_nOption);
	DDX_Control(pDX, IDC_DGN_I_RADIO, m_ratioI);
	DDX_Control(pDX, IDC_DGN_J_RADIO, m_ratioJ);
	DDX_Control(pDX, IDC_DGN_IJ_RADIO, m_ratioIJ);
	DDX_Control(pDX, IDC_DGN_DROOP_EDIT, m_editDroopI);
	DDX_Control(pDX, IDC_DGN_DROOP_J_EDIT, m_editDroopJ);
	DDX_Control(pDX, IDC_DGN_STRAIN_I_EDT, m_editStrainI);
	DDX_Control(pDX, IDC_DGN_STRAIN_J_EDT, m_editStrainJ);
	DDX_Control(pDX, IDC_DGN_IMPACT_EDIT_I, m_editImpact_I);
	DDX_Control(pDX, IDC_DGN_IMPACT_EDIT_J, m_editImpact_J);
	DDX_Control(pDX, IDC_DGN_ADD_REPONCE_EDIT_I, m_editAddReponceF_I);
	DDX_Control(pDX, IDC_DGN_ADD_REPONCE_EDIT_J, m_editAddReponceF_J);
	DDX_Control(pDX, IDC_DGN_TRAFFIT_EDIT, m_editTraffit);
	DDX_Control(pDX, IDC_DGN_LOAD_EDIT, m_editLoad);
	DDX_Control(pDX, IDC_DGN_DROOP_I_UNIT, m_unitDroopI);	
	DDX_Control(pDX, IDC_DGN_DROOP_J_UNIT, m_unitDroopJ);	
	DDX_Control(pDX, IDC_DGN_COMPEN_EDIT, m_editCompen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnRationOptionDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRationOptionDlg)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_AR, OnDgnAr)
	ON_BN_CLICKED(IDC_DGN_DEL, OnDgnDel)	
	ON_BN_CLICKED(IDC_DGN_I_RADIO,	SetEnableByCtrl)	
	ON_BN_CLICKED(IDC_DGN_J_RADIO,	SetEnableByCtrl)	
	ON_BN_CLICKED(IDC_DGN_IJ_RADIO, SetEnableByCtrl)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRationOptionDlg message handlers

// void CDgnRationOptionDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnRationOptionDlg::Execute()
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		/*
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
			if(CDBLib::IsPscDgnFor1st(key,TRUE))	rSelKey.Add(key);
		}
		*/

		T_RKCE_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			if(m_ratioI.GetCheck())
				rData.nChkPosition = 0;
			else if(m_ratioJ.GetCheck())
				rData.nChkPosition = 1;
			else //if(m_ratioIJ.GetCheck())
				rData.nChkPosition = 2;
			rData.dRealDeflection[0] = m_editDroopI.GetEditValue();
			rData.dRealDeflection[1] = m_editDroopJ.GetEditValue();
			rData.dStrainI = m_editStrainI.GetEditValue();
			rData.dStrainJ = m_editStrainJ.GetEditValue();			
			rData.dRealCrashCoeff = m_editImpact_I.GetEditValue();
			rData.dRealCrashCoeff_J = m_editImpact_J.GetEditValue();
			rData.dAddResponce_I = m_editAddReponceF_I.GetEditValue();
			rData.dAddResponce_J = m_editAddReponceF_J.GetEditValue();
			rData.dTrafficStateCoeff = m_editTraffit.GetEditValue();
			rData.dRoadStateCoeff = m_editLoad.GetEditValue();
			rData.dCompenCoeff = m_editCompen.GetEditValue();

			if(aSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddRkce(aSelKey,rData))	Initial_SelectItem();
			}      
			//else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelRkce(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnRationOptionDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
	SetEnableByCtrl();
}

BOOL CDgnRationOptionDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	SetControl();
	SetControlText();
	if(SetControlEnableByDB())
	{
		Initial_Data();
		SetEnableByCtrl();
	}	

	HideAndMoveControlByDB();

	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRationOptionDlg::SetControl()
{	
	m_editDroopI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_editDroopJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_unitDroopI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_unitDroopJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_editStrainI.SetUnitType(D_UNITSYS_NONE);	
	m_editStrainJ.SetUnitType(D_UNITSYS_NONE);	
	m_editImpact_I.SetUnitType(D_UNITSYS_NONE);	
	m_editImpact_J.SetUnitType(D_UNITSYS_NONE);	
	m_editAddReponceF_I.SetUnitType(D_UNITSYS_NONE);	
	m_editAddReponceF_J.SetUnitType(D_UNITSYS_NONE);	
	m_editTraffit.SetUnitType(D_UNITSYS_NONE);	
	m_editLoad.SetUnitType(D_UNITSYS_NONE);	
	m_editCompen.SetUnitType(D_UNITSYS_NONE);	

	CString szTemp;
	szTemp.Format(_T("%s%s"), _LS(IDS_DGN_CHAR_MU), _LS(IDS_DGN_CHAR_EPSILON));
	GetDlgItem(IDC_DGN_STRAIN_I_UNT)->SetWindowText(szTemp);
	GetDlgItem(IDC_DGN_STRAIN_J_UNT)->SetWindowText(szTemp);
	
	CtrlMoveToSamePos(IDC_DGN_DROOP_I_STATIC, IDC_DGN_STRAIN_I_ST);
	CtrlMoveToSamePos(IDC_DGN_DROOP_EDIT, IDC_DGN_STRAIN_I_EDT);
	CtrlMoveToSamePos(IDC_DGN_DROOP_I_UNIT, IDC_DGN_STRAIN_I_UNT);
	CtrlMoveToSamePos(IDC_DGN_DROOP_J_STATIC, IDC_DGN_STRAIN_J_ST);
	CtrlMoveToSamePos(IDC_DGN_DROOP_J_EDIT, IDC_DGN_STRAIN_J_EDT);
	CtrlMoveToSamePos(IDC_DGN_DROOP_J_UNIT, IDC_DGN_STRAIN_J_UNT);

	GetDlgItem(IDC_DEFINE_GROUP)->GetWindowRect(m_rcGrpDefineBig);
	GetDlgItem(IDC_FACTOR_GROUP)->GetWindowRect(m_rcGrpFactorBig);
	
	ScreenToClient(m_rcGrpDefineBig);
	ScreenToClient(m_rcGrpFactorBig);
	
	m_rcGrpDefineSmall = m_rcGrpDefineBig;
	m_rcGrpDefineSmall.bottom -= 100;

	m_rcGrpFactorSmall = m_rcGrpFactorBig;
	m_rcGrpFactorSmall.bottom -= 100;
}

void CDgnRationOptionDlg::SetControlText()
{
	GetDlgItem(IDC_DEFINE_GROUP        )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_FACTOR_GROUP));

	GetDlgItem(IDC_DGN_DROOP_I_STATIC  )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_DISPL_I));  // 실측 처짐량(I)
	GetDlgItem(IDC_DGN_DROOP_J_STATIC  )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_DISPL_J));  // 실측 처짐량(J)

	GetDlgItem(IDC_DGN_STRAIN_I_ST     )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_I));   // 실측 변형률(I)
	GetDlgItem(IDC_DGN_STRAIN_J_ST     )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_J));   // 실측 변형률(J)

	GetDlgItem(IDC_DGN_IMPACT_FACTOR_I )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_I)); // 실측 충격계수(I)
	GetDlgItem(IDC_DGN_IMPACT_FACTOR_J )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_J)); // 실측 충격계수(J)

	GetDlgItem(IDC_DGN_ADD_RESPONSE_FACTOR_I )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_I)); // 추가응답보정계수(I)
	GetDlgItem(IDC_DGN_ADD_RESPONSE_FACTOR_J )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_J)); // 추가응답보정계수(J)

}


void CDgnRationOptionDlg::CtrlMoveToSamePos(int nTargetCtrlID, int nMoveingCtrlID)
{
	CRect rcTarget, rcMoving;
	GetDlgItem(nTargetCtrlID)->GetWindowRect(rcTarget);
	ScreenToClient(rcTarget);

	GetDlgItem(nMoveingCtrlID)->GetWindowRect(rcMoving);

	GetDlgItem(nMoveingCtrlID)->SetWindowPos(NULL, rcTarget.left, rcTarget.top, rcMoving.Width(), rcMoving.Height(), SWP_SHOWWINDOW);
}

void CDgnRationOptionDlg::HideAndMoveControlByDB()
{
	CRect rcTemp;

	T_RACD_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

	if(CDBLib::IsPscRatingCodeKRSince10(Data.nRatingCode))
	{
		T_RAKR_D tRakr;	
		if(!m_pDoc->m_pAttrCtrl2->GetRakr(tRakr))
		{
			tRakr.Initialize();

			// KSCE_USD10_RATING와 KSCE_RAIL_USD11_RATING일 경우만 Init을 다르게 하는 처리..
			tRakr.dDgnLiveLoad = 24.0;
			tRakr.bRefRebar = TRUE;
		}
			
		BOOL bShowDeflection = tRakr.nSurveyMeth == 0;

		CDlgUtil::CtrlShowHide(this, m_aDispl_I, bShowDeflection);
		CDlgUtil::CtrlShowHide(this, m_aDispl_J, bShowDeflection);

		CDlgUtil::CtrlShowHide(this, m_aStrain_I, !bShowDeflection);
		CDlgUtil::CtrlShowHide(this, m_aStrain_J, !bShowDeflection);

		CDlgUtil::CtrlShowHide(this, m_aKSCE_USD05, FALSE);

		GetDlgItem(IDC_DEFINE_GROUP)->SetWindowPos(NULL, m_rcGrpDefineSmall.left, m_rcGrpDefineSmall.top, m_rcGrpDefineSmall.Width(), m_rcGrpDefineSmall.Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_FACTOR_GROUP)->SetWindowPos(NULL, m_rcGrpFactorSmall.left, m_rcGrpFactorSmall.top, m_rcGrpFactorSmall.Width(), m_rcGrpFactorSmall.Height(), SWP_SHOWWINDOW);
		
		GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(&rcTemp);
		ScreenToClient(rcTemp);
		GetDlgItem(IDC_DGN_EXECUTE)->SetWindowPos(NULL, rcTemp.left, m_rcGrpDefineSmall.bottom + 8, rcTemp.Width(), rcTemp.Height(), SWP_SHOWWINDOW);
		
		GetDlgItem(IDC_DGN_CLOSE)->GetWindowRect(&rcTemp);
		ScreenToClient(rcTemp);
		GetDlgItem(IDC_DGN_CLOSE)->SetWindowPos(NULL, rcTemp.left, m_rcGrpDefineSmall.bottom + 8, rcTemp.Width(), rcTemp.Height(), SWP_SHOWWINDOW);		
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aDispl_I, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDispl_J, TRUE);

		CDlgUtil::CtrlShowHide(this, m_aStrain_I, !FALSE);
		CDlgUtil::CtrlShowHide(this, m_aStrain_J, !FALSE);
		CDlgUtil::CtrlShowHide(this, m_aKSCE_USD05, TRUE);

		GetDlgItem(IDC_DEFINE_GROUP)->SetWindowPos(NULL, m_rcGrpDefineBig.left, m_rcGrpDefineBig.top, m_rcGrpDefineBig.Width(), m_rcGrpDefineBig.Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_FACTOR_GROUP)->SetWindowPos(NULL, m_rcGrpFactorBig.left, m_rcGrpFactorBig.top, m_rcGrpFactorBig.Width(), m_rcGrpFactorBig.Height(), SWP_SHOWWINDOW);

		GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(&rcTemp);
		ScreenToClient(rcTemp);
		GetDlgItem(IDC_DGN_EXECUTE)->SetWindowPos(NULL, rcTemp.left, m_rcGrpDefineBig.bottom + 8, rcTemp.Width(), rcTemp.Height(), SWP_SHOWWINDOW);
		
		GetDlgItem(IDC_DGN_CLOSE)->GetWindowRect(&rcTemp);
		ScreenToClient(rcTemp);
		GetDlgItem(IDC_DGN_CLOSE)->SetWindowPos(NULL, rcTemp.left, m_rcGrpDefineBig.bottom + 8, rcTemp.Width(), rcTemp.Height(), SWP_SHOWWINDOW);
	}
}

void CDgnRationOptionDlg::Initial_Data()
{
	const double dDefVal = 0.001;

	T_RKCE_D RkceD; RkceD.Initialize();	

	m_nOption	= 0;      // add/replace
	m_ratioI.SetCheck(RkceD.nChkPosition == 0);
	m_ratioJ.SetCheck(RkceD.nChkPosition == 1);
	m_ratioIJ.SetCheck(RkceD.nChkPosition == 2);
	m_editDroopI.SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_editDroopJ.SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_editStrainI.SetEditUnit(RkceD.dStrainI);	
	m_editStrainJ.SetEditUnit(RkceD.dStrainJ);	
	m_editImpact_I.SetEditUnit(RkceD.dRealCrashCoeff);
	m_editImpact_J.SetEditUnit(RkceD.dRealCrashCoeff_J);
	m_editAddReponceF_I.SetEditUnit(RkceD.dAddResponce_I);
	m_editAddReponceF_J.SetEditUnit(RkceD.dAddResponce_J);
	m_editTraffit.SetEditUnit(RkceD.dTrafficStateCoeff);
	m_editLoad.SetEditUnit(RkceD.dRoadStateCoeff);
	m_editCompen.SetEditUnit(RkceD.dCompenCoeff);

	UpdateData(FALSE);
}

BOOL CDgnRationOptionDlg::SetControlEnableByDB()
{		
	T_RACD_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

	if(Data.nRatingCode == KSCE_USD05_RATING || CDBLib::IsPscRatingCodeKRSince10(Data.nRatingCode))
	{

		BOOL bKSCE_USD05 = Data.nRatingCode == KSCE_USD05_RATING ? TRUE : FALSE;

		CDlgUtil::CtrlEnableDisable(this, m_aGroupBox, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aRadioBtn, TRUE);

		CDlgUtil::CtrlEnableDisable(this, m_aStrain_I, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aStrain_J, TRUE);

		CDlgUtil::CtrlEnableDisable(this, m_aDispl_I, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aDispl_J, TRUE);

		CDlgUtil::CtrlEnableDisable(this, m_aImpact_I, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aImpact_J, TRUE);

		CDlgUtil::CtrlEnableDisable(this, m_aAddReponce_I, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aAddReponce_I, TRUE);

		CDlgUtil::CtrlEnableDisable(this, m_aKSCE_USD05, bKSCE_USD05);
		CDlgUtil::CtrlEnableDisable(this, m_aEtc, TRUE);

		return TRUE;
	}
	else //BRIDGE_LOAD_RATING
	{
		CDlgUtil::CtrlEnableDisable(this, m_aGroupBox, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aRadioBtn, FALSE);

		CDlgUtil::CtrlEnableDisable(this, m_aStrain_I, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aStrain_J, FALSE);

		CDlgUtil::CtrlEnableDisable(this, m_aDispl_I, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aDispl_J, FALSE);

		CDlgUtil::CtrlEnableDisable(this, m_aImpact_I, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aImpact_J, FALSE);

		CDlgUtil::CtrlEnableDisable(this, m_aAddReponce_I, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aAddReponce_I, FALSE);

		CDlgUtil::CtrlEnableDisable(this, m_aKSCE_USD05, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aEtc, FALSE);

		return FALSE;
	}
}

void CDgnRationOptionDlg::OnDgnAr() 
{
	m_nOption  = 0;			//	add/replace	
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRationOptionDlg::OnDgnDel() 
{
	m_nOption  = 1;			//	Delete	
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRationOptionDlg::SetEnableByCtrl() 
{
	UpdateData(TRUE);

	BOOL bStateAddMod = m_nOption == 0;
 
	T_RACD_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

	BOOL bShowDeflection = TRUE;
	if(CDBLib::IsPscRatingCodeKRSince10(Data.nRatingCode))
	{
		T_RAKR_D tRakr;	
		if(!m_pDoc->m_pAttrCtrl2->GetRakr(tRakr))
		{
			tRakr.Initialize();
		}
		bShowDeflection = tRakr.nSurveyMeth == 0;
	}
	
	if(!bStateAddMod)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aGroupBox,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aKSCE_USD05,FALSE);
	}
	else
	{
		if(m_ratioI.GetCheck())
		{
			 CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,TRUE);
			 CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,FALSE);
		}
		else if(m_ratioJ.GetCheck())
		{
			CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,TRUE);
		}
		else if(m_ratioIJ.GetCheck())
		{
			CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,TRUE);
		}
		else{ASSERT(0);}

		if(CDBLib::IsPscRatingCodeKRSince10(Data.nRatingCode))
		{
			CDlgUtil::CtrlEnableDisable(this, m_aKSCE_USD05,FALSE);
		}
		else if(Data.nRatingCode == KSCE_USD05_RATING)
		{
			CDlgUtil::CtrlEnableDisable(this, m_aKSCE_USD05,TRUE);
		}
		else    { ASSERT(0);}
	}

}

void CDgnRationOptionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				UpdateBuffer();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CDgnRationOptionDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bSpfcChanged = FALSE;
	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		switch(nCmd)
		{
		case(UR_RACD_ADD)://코드 변경시
		case(UR_RACD_DEL): 
			{				
				HideAndMoveControlByDB();
				if(!SetControlEnableByDB())
					return;
				SetEnableByCtrl();
				return;
			}		

		case (UR_RAKR_ADD):
		case (UR_RAKR_DEL):
			{
				HideAndMoveControlByDB();
				return;
			}
		default:
		  break;
		}
	}
}