// DgnSteelRatingOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCRatingOptionDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRCRatingOptionDlg dialog

CDgnRCRatingOptionDlg::CDgnRCRatingOptionDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnRCRatingOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRCRatingOptionDlg)
	m_nOption   = 0;	
	m_nPosition = 0;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_nRatingCode = 0;
	m_bShowDispl  = FALSE;
}

CDgnRCRatingOptionDlg::~CDgnRCRatingOptionDlg()
{
}

void CDgnRCRatingOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio  (pDX, IDC_DGN_AR,            m_nOption);
	DDX_Radio  (pDX, IDC_DGN_I_RADIO,       m_nPosition);
	DDX_Control(pDX, IDC_DGN_DROOP_I_EDT,    m_edtDisplI);
	DDX_Control(pDX, IDC_DGN_DROOP_I_UNIT,    m_untDisplI);	
	DDX_Control(pDX, IDC_DGN_DROOP_J_EDT,  m_edtDisplJ);
	DDX_Control(pDX, IDC_DGN_DROOP_J_UNIT,  m_untDisplJ);		
	DDX_Control(pDX, IDC_DGN_STRAIN_I_EDT,   m_edtStrainI);
	DDX_Control(pDX, IDC_DGN_STRAIN_J_EDT, m_edtStrainJ);
	DDX_Control(pDX, IDC_DGN_IMPACT_I_EDT,   m_edtImpactI);
	DDX_Control(pDX, IDC_DGN_IMPACT_J_EDT,   m_edtImpactJ);
	DDX_Control(pDX, IDC_DGN_K_I_EDT,   m_edtK_I);
	DDX_Control(pDX, IDC_DGN_K_J_EDT,   m_edtK_J);
}

void CDgnRCRatingOptionDlg::InitUnit()
{
	m_edtDisplI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_edtDisplJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_untDisplI.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_untDisplJ.SetUnitType(D_UNITSYS_BASE_LENGTH);	

	m_edtStrainI.SetUnitType(D_UNITSYS_NONE);	
	m_edtStrainJ.SetUnitType(D_UNITSYS_NONE);	

	m_edtImpactI.SetUnitType(D_UNITSYS_NONE);	
	m_edtImpactJ.SetUnitType(D_UNITSYS_NONE);	
	m_edtK_I.SetUnitType(D_UNITSYS_NONE);	
	m_edtK_J.SetUnitType(D_UNITSYS_NONE);	

	CString szTemp;
	szTemp.Format(_T("%s%s"), _LS(IDS_DGN_CHAR_MU), _LS(IDS_DGN_CHAR_EPSILON));
	GetDlgItem(IDC_DGN_STRAIN_I_UNIT)->SetWindowText(szTemp);
	GetDlgItem(IDC_DGN_STRAIN_J_UNIT)->SetWindowText(szTemp);
}

void CDgnRCRatingOptionDlg::InitSetiing()
{
	// RC Rating Code
	T_RCDC_D RcdcD; RcdcD.Initialize();	
	m_pDoc->m_pAttrCtrl2->GetDgnRcdc(RcdcD);
	m_nRatingCode = RcdcD.nRatingCode;

	// Strain or Displacement
	T_RPAC_D RpacD;
	if(!m_pDoc->m_pAttrCtrl2->GetRpac(RpacD))
	{
		RpacD.Initialize(m_nRatingCode);
	}		

	if(m_nRatingCode==KSCE_USD10_RC_RATING || m_nRatingCode==KSCE_RAIL_USD11_RC_RATING || m_nRatingCode==KSCE_LSD15_RC_RATING)
	{
		if(RpacD.nSurveyMeth == 0) m_bShowDispl = TRUE; // Strain
		else                        m_bShowDispl = FALSE;  // Displacement
	}
	else  { ASSERT(FALSE); }

	// Controls ID
	m_aStrain_I.RemoveAll();
	m_aStrain_I.Add(IDC_DGN_STRAIN_I_STATIC);
	m_aStrain_I.Add(IDC_DGN_STRAIN_I_EDT);
	m_aStrain_I.Add(IDC_DGN_STRAIN_I_UNIT);

	m_aStrain_J.RemoveAll();
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_STATIC);
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_EDT);
	m_aStrain_J.Add(IDC_DGN_STRAIN_J_UNIT);

	m_aDispl_I.RemoveAll();
	m_aDispl_I.Add(IDC_DGN_DROOP_I_STATIC);
	m_aDispl_I.Add(IDC_DGN_DROOP_I_EDT);
	m_aDispl_I.Add(IDC_DGN_DROOP_I_UNIT);

	m_aDispl_J.RemoveAll();
	m_aDispl_J.Add(IDC_DGN_DROOP_J_STATIC);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_EDT);
	m_aDispl_J.Add(IDC_DGN_DROOP_J_UNIT);

	m_aImpact_I.RemoveAll();
	m_aImpact_I.Add(IDC_DGN_FACTOR_I_STATIC);
	m_aImpact_I.Add(IDC_DGN_IMPACT_I_EDT);

	m_aImpact_J.RemoveAll();
	m_aImpact_J.Add(IDC_DGN_FACTOR_J_STATIC);
	m_aImpact_J.Add(IDC_DGN_IMPACT_J_EDT);

	m_aAddResponceK_I.RemoveAll();
	m_aAddResponceK_I.Add(IDC_DGN_K_I_STC);
	m_aAddResponceK_I.Add(IDC_DGN_K_I_EDT);

	m_aAddResponceK_J.RemoveAll();
	m_aAddResponceK_J.Add(IDC_DGN_K_J_STC);
	m_aAddResponceK_J.Add(IDC_DGN_K_J_EDT);

	m_aEtc.RemoveAll();
	m_aEtc.Add(IDC_DGN_EXECUTE);
	m_aEtc.Add(IDC_DGN_CLOSE);
}

void CDgnRCRatingOptionDlg::InitText()
{
	CString strCtrlName; 
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_FACTOR_GROUP));        GetDlgItem(IDC_FACTOR_GROUP        )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_FACTOR_GROUP));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_DISPL_I));             GetDlgItem(IDC_DGN_DROOP_I_STATIC  )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_DISPL_I));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_DISPL_J));             GetDlgItem(IDC_DGN_DROOP_J_STATIC  )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_DISPL_J));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_I));            GetDlgItem(IDC_DGN_STRAIN_I_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_I));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_J));            GetDlgItem(IDC_DGN_STRAIN_J_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_J));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_I));     GetDlgItem(IDC_DGN_FACTOR_I_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_I));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_J));     GetDlgItem(IDC_DGN_FACTOR_J_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_J));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_I));GetDlgItem(IDC_DGN_K_I_STC         )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_I));
	strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_J));GetDlgItem(IDC_DGN_K_J_STC         )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_J));

	if(m_nRatingCode==KSCE_USD10_RC_RATING || m_nRatingCode==KSCE_RAIL_USD11_RC_RATING || m_nRatingCode==KSCE_LSD15_RC_RATING)
	{
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_FACTOR_GROUP));        GetDlgItem(IDC_FACTOR_GROUP        )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_FACTOR_GROUP));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_DISPL_I));             GetDlgItem(IDC_DGN_DROOP_I_STATIC  )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_DISPL_I));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_DISPL_J));             GetDlgItem(IDC_DGN_DROOP_J_STATIC  )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_DISPL_J));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_I));            GetDlgItem(IDC_DGN_STRAIN_I_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_I));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_J));            GetDlgItem(IDC_DGN_STRAIN_J_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_STRAIN_J));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_I));     GetDlgItem(IDC_DGN_FACTOR_I_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_I));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_J));     GetDlgItem(IDC_DGN_FACTOR_J_STATIC )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_IMPACT_FACTOR_J));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_I));GetDlgItem(IDC_DGN_K_I_STC         )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_I));
		strCtrlName.Format(_T("%s"),_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_J));GetDlgItem(IDC_DGN_K_J_STC         )->SetWindowText(_LS(IDS_DGN_RC_RATING_OPTION_ADD_RESPONCE_FATOR_J));
	}
	else
	{
		ASSERT(FALSE);
	}

		RedrawWindow();
}

void CDgnRCRatingOptionDlg::InitDefaultValue()
{
	T_RROC_D RrocD; RrocD.Initialize();

	const double dDefVal = 0.001;
	m_edtDisplI .SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_edtDisplJ .SetEditUnit(m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dDefVal));
	m_edtStrainI.SetEditUnit(RrocD.dStrainI);
	m_edtStrainJ.SetEditUnit(RrocD.dStrainJ);
	m_edtImpactI .SetEditUnit(RrocD.dRealCrashCoeffI);
	m_edtImpactJ .SetEditUnit(RrocD.dRealCrashCoeffJ);
	m_edtK_I.SetEditUnit(RrocD.dAdjustmentFactorI);
	m_edtK_J.SetEditUnit(RrocD.dAdjustmentFactorJ);
}

void CDgnRCRatingOptionDlg::AlignControl()
{
	CRect rRef;
	CRect rDist;
	CRect rStrain;
	int nDistY = 0;

	if(m_nRatingCode==KSCE_USD10_RC_RATING || m_nRatingCode==KSCE_RAIL_USD11_RC_RATING || m_nRatingCode==KSCE_LSD15_RC_RATING)
	{
		GetDlgItem(IDC_FACTOR_GROUP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DROOP_I_EDT)->GetWindowRect(rDist);
		GetDlgItem(IDC_DGN_STRAIN_I_EDT)->GetWindowRect(rStrain);
		ScreenToClient(rRef);
		ScreenToClient(rDist);
		ScreenToClient(rStrain);
		nDistY = rDist.top - rStrain.top;

		if(nDistY != 0)
		{
			CDlgUtil::CtrlMoveDistY(this, m_aStrain_I, nDistY);
			CDlgUtil::CtrlMoveDistY(this, m_aStrain_J, nDistY);
		}
	}
	else
	{
			ASSERT(FALSE);
	}
}

void CDgnRCRatingOptionDlg::EnableDisableControls()
{
	if(m_nOption == 0) // Add/Replace
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DEFINE_GROUP, TRUE, TRUE);

		GetDlgItem(IDC_DGN_DROOP_I_STATIC)->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_I_EDT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_I_UNIT  )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_J_STATIC)->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_J_EDT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_DROOP_J_UNIT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);

		GetDlgItem(IDC_DGN_STRAIN_I_STATIC)->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_I_EDT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_I_UNIT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_J_STATIC)->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_J_EDT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_STRAIN_J_UNIT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);

		GetDlgItem(IDC_DGN_FACTOR_I_STATIC)->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_IMPACT_I_EDT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_FACTOR_J_STATIC)->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_IMPACT_J_EDT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);

		GetDlgItem(IDC_DGN_K_I_STC)->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_K_I_EDT    )->EnableWindow(m_nPosition == 0 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_K_J_STC)->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
		GetDlgItem(IDC_DGN_K_J_EDT  )->EnableWindow(m_nPosition == 1 || m_nPosition == 2);
	}
	else
	{
		CDlgUtil::CtrlEnableDisableByRect(this, IDC_DEFINE_GROUP, FALSE, TRUE);
	}	

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CLOSE)->EnableWindow(TRUE);
}

void CDgnRCRatingOptionDlg::ControlsShowHide()
{
	if(m_nRatingCode==KSCE_USD10_RC_RATING || m_nRatingCode==KSCE_RAIL_USD11_RC_RATING || m_nRatingCode==KSCE_LSD15_RC_RATING)
	{
		CDlgUtil::CtrlShowHide(this, m_aDispl_I,  m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aDispl_J,  m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aStrain_I,!m_bShowDispl);
		CDlgUtil::CtrlShowHide(this, m_aStrain_J,!m_bShowDispl);
	}
	else
	{
		ASSERT(FALSE);
	}

}

BEGIN_MESSAGE_MAP(CDgnRCRatingOptionDlg, CMenuBarChildDlg)
// 	ON_BN_CLICKED(IDC_DGN_EXECUTE,  OnDgnExecute)
// 	ON_BN_CLICKED(IDC_DGN_CLOSE,    OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_AR,       OnDgnArDelRdo)
	ON_BN_CLICKED(IDC_DGN_DEL,      OnDgnArDelRdo)	
	ON_BN_CLICKED(IDC_DGN_I_RADIO,	OnDgnPositionRdo)	
	ON_BN_CLICKED(IDC_DGN_J_RADIO,	OnDgnPositionRdo)	
	ON_BN_CLICKED(IDC_DGN_IJ_RADIO, OnDgnPositionRdo)	
END_MESSAGE_MAP()


// CDgnRCRatingOptionDlg 메시지 처리기입니다.

BOOL CDgnRCRatingOptionDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitUnit();
	InitSetiing();
	InitText();
	InitDefaultValue();
	AlignControl();
	EnableDisableControls();
	ControlsShowHide();	

	UpdateData(FALSE);

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRCRatingOptionDlg::Execute() 
{
	UpdateData(TRUE);

	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		T_RROC_D rData; rData.Initialize();

		if(m_nOption==0)	// add/replace
		{
			rData.nPosition = m_nPosition;
			rData.dRealDeflectionI     = m_edtDisplI.GetEditValue();
			rData.dRealDeflectionJ     = m_edtDisplJ.GetEditValue();
			rData.dStrainI  = m_edtStrainI.GetEditValue();
			rData.dStrainJ  = m_edtStrainJ.GetEditValue();
			rData.dRealCrashCoeffI     = m_edtImpactI.GetEditValue();
			rData.dRealCrashCoeffJ     = m_edtImpactJ.GetEditValue();
			rData.dAdjustmentFactorI = m_edtK_I.GetEditValue();
			rData.dAdjustmentFactorJ = m_edtK_J.GetEditValue();

			if(m_pDoc->m_pDataCtrl->AddRroc(aSelKey,rData))
			{
				m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelRroc(aSelKey))
			{
				m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
			}
		}    
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

// void CDgnRCRatingOptionDlg::OnDgnClose() 
// {
// 	// TODO: Add your control notification handler code here
// 	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
// }

void CDgnRCRatingOptionDlg::OnDgnArDelRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnRCRatingOptionDlg::OnDgnPositionRdo() 
{
	UpdateData(TRUE);
	EnableDisableControls();
}

void CDgnRCRatingOptionDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnRCRatingOptionDlg::UpdateBuffer()
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
		case(UR_RCDC_ADD):
		case(UR_RCDC_DEL): // Code 변경시
		case(UR_RPAC_ADD):
		case(UR_RPAC_DEL): // Option변경시
			InitSetiing();
			InitText();
			AlignControl();
			EnableDisableControls();
			ControlsShowHide();	
			break;		
		default:
			break;
		}
	}
}