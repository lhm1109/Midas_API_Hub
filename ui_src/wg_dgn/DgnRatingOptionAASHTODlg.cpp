// DgnRationOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRatingOptionAASHTODlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
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
// CDgnRatingOptionAASHTODlg dialog

constexpr int ELEM_TYPE_ELEMENT = 0;
constexpr int ELEM_TYPE_VBEAM = 1;

CDgnRatingOptionAASHTODlg::CDgnRatingOptionAASHTODlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRatingOptionAASHTODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRatingOptionAASHTODlg)
	m_nOption = -1;	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_nElemType = ELEM_TYPE_ELEMENT;

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

	// Controls ID
	m_aAutoUser.RemoveAll();
	m_aAutoUser.Add(IDC_DGN_AUTO_RDO);
	m_aAutoUser.Add(IDC_DGN_USER_RDO);
	
	m_aCtrl_I; m_aCtrl_I.RemoveAll();
	m_aCtrl_I.Add(IDC_DGN_DROOP_I_STATIC);
	m_aCtrl_I.Add(IDC_DGN_DROOP_I_EDIT);
	m_aCtrl_I.Add(IDC_DGN_DROOP_I_UNIT);
	m_aCtrl_I.Add(IDC_DGN_STRAIN_I_ST);
	m_aCtrl_I.Add(IDC_DGN_STRAIN_I_EDT);
	m_aCtrl_I.Add(IDC_DGN_STRAIN_I_UNT);


	m_aCtrl_J; m_aCtrl_J.RemoveAll();
	m_aCtrl_J.Add(IDC_DGN_DROOP_J_STATIC);
	m_aCtrl_J.Add(IDC_DGN_DROOP_J_EDIT);
	m_aCtrl_J.Add(IDC_DGN_DROOP_J_UNIT);
	m_aCtrl_J.Add(IDC_DGN_STRAIN_J_ST);
	m_aCtrl_J.Add(IDC_DGN_STRAIN_J_EDT);
	m_aCtrl_J.Add(IDC_DGN_STRAIN_J_UNT);

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
	m_aDispl_I.Add(IDC_DGN_DROOP_I_EDIT);
	m_aDispl_I.Add(IDC_DGN_DROOP_I_UNIT);

	m_aDispl_J;m_aDispl_J.RemoveAll();
	m_aDispl_J.Add(IDC_DGN_DROOP_J_STATIC);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_EDIT);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_UNIT);

	m_aDynamic;m_aDynamic.RemoveAll();
	m_aDynamic.Add(IDC_DGN_DYNAMIC_FACTOR_STATIC);
	m_aDynamic.Add(IDC_DGN_DYNAMIC_FACTOR_EDT);

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_VELEM_RDO);

	m_aMoveCtrl.RemoveAll();
	m_aMoveCtrl.Add(IDC_DEFINE_GROUP);
	m_aMoveCtrl.Add(IDC_POSITION_GROUP);
	m_aMoveCtrl.Add(IDC_DGN_I_RADIO);
	m_aMoveCtrl.Add(IDC_DGN_J_RADIO);
	m_aMoveCtrl.Add(IDC_DGN_IJ_RADIO);
	m_aMoveCtrl.Add(IDC_FACTOR_GROUP);
	m_aMoveCtrl.Add(IDC_DGN_AUTO_RDO);
	m_aMoveCtrl.Add(IDC_DGN_USER_RDO);
	m_aMoveCtrl.Add(IDC_DGN_DROOP_I_STATIC);
	m_aMoveCtrl.Add(IDC_DGN_DROOP_I_EDIT);
	m_aMoveCtrl.Add(IDC_DGN_DROOP_I_UNIT);
	m_aMoveCtrl.Add(IDC_DGN_DROOP_J_STATIC);
	m_aMoveCtrl.Add(IDC_DGN_DROOP_J_EDIT);
	m_aMoveCtrl.Add(IDC_DGN_DROOP_J_UNIT);
	m_aMoveCtrl.Add(IDC_DGN_DYNAMIC_FACTOR_STATIC);
	m_aMoveCtrl.Add(IDC_DGN_DYNAMIC_FACTOR_EDT);
	m_aMoveCtrl.Add(IDC_DGN_K_FACTOR_STC);
	m_aMoveCtrl.Add(IDC_DGN_K_FACTOR_EDT);

	m_aK.RemoveAll();
	m_aK.Add(IDC_DGN_K_FACTOR_STC);
	m_aK.Add(IDC_DGN_K_FACTOR_EDT);


	m_aEtc;m_aEtc.RemoveAll();
	m_aEtc.Add(IDC_DGN_EXECUTE);
	m_aEtc.Add(IDC_DGN_CLOSE);
}


void CDgnRatingOptionAASHTODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRatingOptionAASHTODlg)
	DDX_Radio(pDX, IDC_DGN_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_I_RADIO, m_nPosition);
 // DDX_Control(pDX, IDC_DGN_I_RADIO, m_ratioI);
	//DDX_Control(pDX, IDC_DGN_J_RADIO, m_ratioJ);
	//DDX_Control(pDX, IDC_DGN_IJ_RADIO, m_ratioIJ);
	DDX_Radio(pDX, IDC_DGN_AUTO_RDO, m_nCalcType);
	//DDX_Control(pDX, IDC_DGN_USER_RDO, m_bUser);
	DDX_Control(pDX, IDC_DGN_DROOP_I_EDIT, m_editDroopI);
	DDX_Control(pDX, IDC_DGN_DROOP_J_EDIT, m_editDroopJ);
	DDX_Control(pDX, IDC_DGN_STRAIN_I_EDT, m_editStrainI);
	DDX_Control(pDX, IDC_DGN_STRAIN_J_EDT, m_editStrainJ);
	DDX_Control(pDX, IDC_DGN_DYNAMIC_FACTOR_EDT, m_editDynamic);
	DDX_Control(pDX, IDC_DGN_K_FACTOR_EDT, m_editKFactor);
	DDX_Control(pDX, IDC_DGN_DROOP_I_UNIT, m_unitDroopI);	
	DDX_Control(pDX, IDC_DGN_DROOP_J_UNIT, m_unitDroopJ);
	//DDX_Control(pDX, IDC_DGN_STRAIN_I_UNT, m_unitStrainI);	
	//DDX_Control(pDX, IDC_DGN_STRAIN_J_UNT, m_unitStrainJ);	
	DDX_Radio(pDX, IDC_DGN_PSC_ELEM_RDO, m_nElemType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnRatingOptionAASHTODlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnRatingOptionAASHTODlg)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_AR, OnDgnAr)
	ON_BN_CLICKED(IDC_DGN_DEL, OnDgnDel)	
	ON_BN_CLICKED(IDC_DGN_I_RADIO,	SetEnableByCtrl)	
	ON_BN_CLICKED(IDC_DGN_J_RADIO,	SetEnableByCtrl)	
	ON_BN_CLICKED(IDC_DGN_IJ_RADIO, SetEnableByCtrl)
	ON_BN_CLICKED(IDC_DGN_AUTO_RDO, SetEnableByCtrl)
	ON_BN_CLICKED(IDC_DGN_USER_RDO, SetEnableByCtrl)
	ON_BN_CLICKED(IDC_DGN_PSC_ELEM_RDO, OnDgnPSCElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PSC_VELEM_RDO, OnDgnPSCElemTypeRdo)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRatingOptionAASHTODlg message handlers

// void CDgnRatingOptionAASHTODlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnRatingOptionAASHTODlg::OnDgnPSCElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}

void CDgnRatingOptionAASHTODlg::Execute()
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			T_RKCE_D rData;
			rData.Initialize();
			if (m_nOption == 0)	// add/replace
			{
				if (m_nPosition == 0)
				{
					rData.nChkPosition = 0;
					rData.nCalcType = m_nCalcType;
					rData.dRealDeflection[0] = m_editDroopI.GetEditValue();
					rData.dRealDeflection[1] = m_editDroopJ.GetEditValue();
					rData.dStrainI = m_editStrainI.GetEditValue();
					rData.dStrainJ = m_editStrainJ.GetEditValue();
					rData.dImpactFactor = m_editDynamic.GetEditValue();
					rData.dAdjustmentFactor = m_editKFactor.GetEditValue();
				}
				else if (m_nPosition == 1)
				{
					rData.nChkPosition = 1;
					rData.nCalcType = m_nCalcType;
					rData.dRealDeflection[0] = m_editDroopI.GetEditValue();
					rData.dRealDeflection[1] = m_editDroopJ.GetEditValue();
					rData.dStrainI = m_editStrainI.GetEditValue();
					rData.dStrainJ = m_editStrainJ.GetEditValue();
					rData.dImpactFactor = m_editDynamic.GetEditValue();
					rData.dAdjustmentFactor = m_editKFactor.GetEditValue();
				}
				else if (m_nPosition == 2)
				{
					rData.nChkPosition = 2;
					rData.nCalcType = m_nCalcType;
					rData.dRealDeflection[0] = m_editDroopI.GetEditValue();
					rData.dRealDeflection[1] = m_editDroopJ.GetEditValue();
					rData.dStrainI = m_editStrainI.GetEditValue();
					rData.dStrainJ = m_editStrainJ.GetEditValue();
					rData.dImpactFactor = m_editDynamic.GetEditValue();
					rData.dAdjustmentFactor = m_editKFactor.GetEditValue();
				}
				else ASSERT(0);

				if (aSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddRkce(aSelKey, rData))	Initial_SelectItem();
				}
				//else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelRkce(aSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	else
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);
		if (aSelKey.GetSize() > 0)
		{
			T_RKCV_D rData;
			rData.Initialize();
			if (m_nOption == 0)	// add/replace
			{
				if (m_nPosition == 0)
				{
					rData.nChkPosition = 0;
					rData.nCalcType = m_nCalcType;
					rData.dRealDeflection[0] = m_editDroopI.GetEditValue();
					rData.dRealDeflection[1] = m_editDroopJ.GetEditValue();
					rData.dStrainI = m_editStrainI.GetEditValue();
					rData.dStrainJ = m_editStrainJ.GetEditValue();
					rData.dImpactFactor = m_editDynamic.GetEditValue();
					rData.dAdjustmentFactor = m_editKFactor.GetEditValue();
				}
				else if (m_nPosition == 1)
				{
					rData.nChkPosition = 1;
					rData.nCalcType = m_nCalcType;
					rData.dRealDeflection[0] = m_editDroopI.GetEditValue();
					rData.dRealDeflection[1] = m_editDroopJ.GetEditValue();
					rData.dStrainI = m_editStrainI.GetEditValue();
					rData.dStrainJ = m_editStrainJ.GetEditValue();
					rData.dImpactFactor = m_editDynamic.GetEditValue();
					rData.dAdjustmentFactor = m_editKFactor.GetEditValue();
				}
				else if (m_nPosition == 2)
				{
					rData.nChkPosition = 2;
					rData.nCalcType = m_nCalcType;
					rData.dRealDeflection[0] = m_editDroopI.GetEditValue();
					rData.dRealDeflection[1] = m_editDroopJ.GetEditValue();
					rData.dStrainI = m_editStrainI.GetEditValue();
					rData.dStrainJ = m_editStrainJ.GetEditValue();
					rData.dImpactFactor = m_editDynamic.GetEditValue();
					rData.dAdjustmentFactor = m_editKFactor.GetEditValue();
				}
				else ASSERT(0);

				if (aSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddRkcv(aSelKey, rData))	Initial_SelectItem();
				}
				//else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelRkcv(aSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}

void CDgnRatingOptionAASHTODlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
	SetEnableByCtrl();
}

BOOL CDgnRatingOptionAASHTODlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	SetControl();
	SetControlText();
	/*if(SetControlEnableByDB())
	{
	Initial_Data();
	SetEnableByCtrl();
	}	*/
	Initial_Data();
	SetEnableByCtrl();
	HideAndMoveControlByDB();
	
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRatingOptionAASHTODlg::SetControl()
{	
	m_editDroopI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_editDroopJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_unitDroopI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_unitDroopJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_editStrainI.SetUnitType(D_UNITSYS_NONE);	
	m_editStrainJ.SetUnitType(D_UNITSYS_NONE);
	m_editDynamic.SetUnitType(D_UNITSYS_NONE);	
	m_editKFactor.SetUnitType(D_UNITSYS_NONE);	


	CString szTemp;
	szTemp.Format(_T("%s%s"), _LS(IDS_DGN_CHAR_MU), _LS(IDS_DGN_CHAR_EPSILON));
	GetDlgItem(IDC_DGN_STRAIN_I_UNT)->SetWindowText(szTemp);
	GetDlgItem(IDC_DGN_STRAIN_J_UNT)->SetWindowText(szTemp);
	
	GetDlgItem(IDC_DEFINE_GROUP)->GetWindowRect(m_rcGrpDefineBig);
	GetDlgItem(IDC_FACTOR_GROUP)->GetWindowRect(m_rcGrpFactorBig);
	
	ScreenToClient(m_rcGrpDefineBig);
	ScreenToClient(m_rcGrpFactorBig);
	
	m_rcGrpDefineSmall = m_rcGrpDefineBig;
	m_rcGrpDefineSmall.bottom -= 100;

	m_rcGrpFactorSmall = m_rcGrpFactorBig;
	m_rcGrpFactorSmall.bottom -= 100;

}

void CDgnRatingOptionAASHTODlg::SetControlText()
{
	GetDlgItem(IDC_DEFINE_GROUP       )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_DEFINE_GROUP));  //Define Rating
	GetDlgItem(IDC_DGN_DROOP_I_STATIC )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_DISPL_I));       //Displacement(I)
	GetDlgItem(IDC_DGN_DROOP_J_STATIC )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_DISPL_J));       //Displacement(J)
	GetDlgItem(IDC_DGN_STRAIN_I_ST    )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_STRAIN_I));      //Strain(I)
	GetDlgItem(IDC_DGN_STRAIN_J_ST    )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_STRAIN_J));      //Strain(J)

	GetDlgItem(IDC_POSITION_GROUP     )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_POSITION));      //Position
	GetDlgItem(IDC_FACTOR_GROUP       )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_FACTOR_GROUP));  //Load Test Measurements
	GetDlgItem(IDC_DGN_AUTO_RDO       )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_AUTO_RDO));      //Auto Calculation
	GetDlgItem(IDC_DGN_USER_RDO       )->SetWindowText(_LS(IDS_DGN_AASHTO_PSC_RATING_OPTION_USER_RDO));      //User Input

}


void CDgnRatingOptionAASHTODlg::CtrlMoveToSamePos(int nTargetCtrlID, int nMoveingCtrlID)
{
	CRect rcTarget, rcMoving;
	GetDlgItem(nTargetCtrlID)->GetWindowRect(rcTarget);
	ScreenToClient(rcTarget);

	GetDlgItem(nMoveingCtrlID)->GetWindowRect(rcMoving);

	GetDlgItem(nMoveingCtrlID)->SetWindowPos(NULL, rcTarget.left, rcTarget.top, rcMoving.Width(), rcMoving.Height(), SWP_SHOWWINDOW);
}

void CDgnRatingOptionAASHTODlg::HideAndMoveControlByDB()
{
	CRect rcTemp;

	T_RACD_D Data;
	Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);

	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_DGN_PSC_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DEFINE_GROUP)->GetWindowRect(rToMove);

	BOOL bPSCDgnVbem = CDBLib::ActivatePscVBEM();
	int nDistY;

	if(Data.nRatingCode == AASHTO_LRFD12_PSC_RATING || Data.nRatingCode == AASHTO_LRFD19_PSC_RATING )
	{
		T_BLRS_D BlrsD;	
		if(!m_pDoc->m_pAttrCtrl->GetBlrs(BlrsD))
		{
			BlrsD.Initialize();
		}
		BOOL bShowDeflection = BlrsD.iDiagnostic_Method == 1;

		CDlgUtil::CtrlShowHide(this, m_aDispl_I, bShowDeflection);
		CDlgUtil::CtrlShowHide(this, m_aDispl_J, bShowDeflection);

		CDlgUtil::CtrlShowHide(this, m_aStrain_I, !bShowDeflection);
		CDlgUtil::CtrlShowHide(this, m_aStrain_J, !bShowDeflection);
	
		if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !bPSCDgnVbem)
		{
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
			m_nElemType = ELEM_TYPE_ELEMENT;
		}
		else
		{
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
			CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, TRUE);
		}

	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aDispl_I, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aDispl_J, TRUE);

		CDlgUtil::CtrlShowHide(this, m_aStrain_I, !FALSE);
		CDlgUtil::CtrlShowHide(this, m_aStrain_J, !FALSE);

		nDistY = rRef.top - rToMove.top;
		m_nElemType = ELEM_TYPE_ELEMENT;
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
	}

	CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl, nDistY);

	CtrlMoveToSamePos(IDC_DGN_DROOP_I_STATIC, IDC_DGN_STRAIN_I_ST);
	CtrlMoveToSamePos(IDC_DGN_DROOP_I_EDIT, IDC_DGN_STRAIN_I_EDT);
	CtrlMoveToSamePos(IDC_DGN_DROOP_I_UNIT, IDC_DGN_STRAIN_I_UNT);
	CtrlMoveToSamePos(IDC_DGN_DROOP_J_STATIC, IDC_DGN_STRAIN_J_ST);
	CtrlMoveToSamePos(IDC_DGN_DROOP_J_EDIT, IDC_DGN_STRAIN_J_EDT);
	CtrlMoveToSamePos(IDC_DGN_DROOP_J_UNIT, IDC_DGN_STRAIN_J_UNT);

	// 변형률 단위 무조건 숨김.
	GetDlgItem(IDC_DGN_STRAIN_I_UNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STRAIN_J_UNT)->ShowWindow(SW_HIDE);
	
}

void CDgnRatingOptionAASHTODlg::Initial_Data()
{
	const double dDefVal = 0.001;

	T_RKCE_D RkceD; RkceD.Initialize();	

	m_nOption	= 0;      // add/replace
	m_nPosition = 0;
	m_nCalcType = 0;

	//m_ratioI.SetCheck(RkceD.nChkPosition == 0);
	//m_ratioJ.SetCheck(RkceD.nChkPosition == 1);
	//m_ratioIJ.SetCheck(RkceD.nChkPosition == 2);

	m_editDroopI.SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_editDroopJ.SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_editStrainI.SetEditUnit(RkceD.dStrainI);	
	m_editStrainJ.SetEditUnit(RkceD.dStrainJ);	
	m_editDynamic.SetEditUnit(RkceD.dImpactFactor);
	m_editKFactor.SetEditUnit(RkceD.dAdjustmentFactor);


	UpdateData(FALSE);
}

//BOOL CDgnRatingOptionAASHTODlg::SetControlEnableByDB()
//{		
//	T_RACD_D Data;
//	Data.Initialize();
//	m_pDoc->m_pAttrCtrl2->GetDgnRacd(Data);
//
//  if(Data.nRatingCode == KSCE_USD05_RATING || Data.nRatingCode == KSCE_USD10_RATING || Data.nRatingCode == KSCE_RAIL_USD11_RATING)
//  {
//
//    BOOL bKSCE_USD05 = Data.nRatingCode == KSCE_USD05_RATING ? TRUE : FALSE;
//
//    CDlgUtil::CtrlEnableDisable(this, m_aGroupBox, TRUE);
//    CDlgUtil::CtrlEnableDisable(this, m_aRadioBtn, TRUE);
//
//    CDlgUtil::CtrlEnableDisable(this, m_aStrain_I, TRUE);
//    CDlgUtil::CtrlEnableDisable(this, m_aStrain_J, TRUE);
//
//    CDlgUtil::CtrlEnableDisable(this, m_aDispl_I, TRUE);
//    CDlgUtil::CtrlEnableDisable(this, m_aDispl_J, TRUE);
//
//
//		return TRUE;
//	}
//	else //BRIDGE_LOAD_RATING
//	{
//    CDlgUtil::CtrlEnableDisable(this, m_aGroupBox, FALSE);
//    CDlgUtil::CtrlEnableDisable(this, m_aRadioBtn, FALSE);
//
//    CDlgUtil::CtrlEnableDisable(this, m_aStrain_I, FALSE);
//    CDlgUtil::CtrlEnableDisable(this, m_aStrain_J, FALSE);
//
//    CDlgUtil::CtrlEnableDisable(this, m_aDispl_I, FALSE);
//    CDlgUtil::CtrlEnableDisable(this, m_aDispl_J, FALSE);
//
//    CDlgUtil::CtrlEnableDisable(this, m_aImpact_I, FALSE);
//    CDlgUtil::CtrlEnableDisable(this, m_aImpact_J, FALSE);
//
//    CDlgUtil::CtrlEnableDisable(this, m_aAddReponce_I, FALSE);
//    CDlgUtil::CtrlEnableDisable(this, m_aAddReponce_I, FALSE);
//
//    CDlgUtil::CtrlEnableDisable(this, m_aKSCE_USD05, FALSE);
//    CDlgUtil::CtrlEnableDisable(this, m_aEtc, FALSE);
//
//		return FALSE;
//	}
//}

void CDgnRatingOptionAASHTODlg::OnDgnAr() 
{
	m_nOption  = 0;			//	add/replace	
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRatingOptionAASHTODlg::OnDgnDel() 
{
	m_nOption  = 1;			//	Delete	
	UpdateData(FALSE);

	SetEnableByCtrl();
}

void CDgnRatingOptionAASHTODlg::SetEnableByCtrl() 
{
	UpdateData(TRUE);

	BOOL bStateAddMod = m_nOption == 0;
	BOOL bAUTOType = m_nCalcType ==0;
	if(bStateAddMod==FALSE)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aGroupBox,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aAutoUser,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aDynamic,FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aK,FALSE);
	}
	else
	{
		if (bAUTOType==FALSE)
		{
			CDlgUtil::CtrlEnableDisable(this, m_aK,TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aAutoUser,TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aDynamic,FALSE);
		}
		else if (bAUTOType==TRUE)
		{
			if(m_nPosition==0)
			{
				CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,FALSE);
				CDlgUtil::CtrlEnableDisable(this, m_aAutoUser,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aDynamic,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aK,FALSE);
			}
			else if(m_nPosition==1)
			{
				CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,FALSE);
				CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aAutoUser,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aDynamic,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aK,FALSE);
			}
			else if(m_nPosition==2)
			{
				CDlgUtil::CtrlEnableDisable(this, m_aCtrl_I,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aCtrl_J,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aAutoUser,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aDynamic,TRUE);
				CDlgUtil::CtrlEnableDisable(this, m_aK,FALSE);
			}
			else{ASSERT(0);}
		}
		else ASSERT(0);

	}
}

void CDgnRatingOptionAASHTODlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnRatingOptionAASHTODlg::UpdateBuffer()
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
				//if(!SetControlEnableByDB())
				//	return;
				SetEnableByCtrl();
				return;
			}		

		case (UR_BLRS_ADD):
		case (UR_BLRS_DEL):
			{
				SetControl();
				SetControlText();
				Initial_Data();
				SetEnableByCtrl();
				HideAndMoveControlByDB();
				return;
			}
		default:
		  break;
		}
	}
}