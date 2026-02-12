// CMMvhlItemStdBSCS458Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSCS458Page.h"
#include "CMMvhlItemStdBSDlg.h"
#include "CMMvhlStdEuroBSNewTempForRefactor.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS458Page dialog

CCMMvhlItemStdBSCS458Page::CCMMvhlItemStdBSCS458Page(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSCS458Page::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSCS458Page)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_bDynamic = TRUE;
	m_nDynaAutoUser = 0;
	m_bOverload = TRUE;
	m_nOverAutoUser = 0;
	m_nDistVehl = 0;
	m_bPatch = FALSE;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_nLoadType = 0;
}

void CCMMvhlItemStdBSCS458Page::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSCS458Page)
	DDX_Control(pDX, IDC_CMD_FIRST_GRID, m_wndGrid1);
	DDX_Control(pDX, IDC_CMD_SECOND_GRID, m_wndGrid2);

	DDX_Check  (pDX, IDC_CMD_MVHL_DYNAMIC_CHK, m_bDynamic);
	DDX_Radio  (pDX, IDC_CMD_MVHL_AUTO_RDO, m_nDynaAutoUser);
	DDX_Control(pDX, IDC_CMD_MVHL_PHI_EDT, m_edtDynaPhi);
	
	DDX_Check  (pDX, IDC_OVERLOAD_FACTOR_CHK, m_bOverload);
	DDX_Radio  (pDX, IDC_OVERLOAD_FACTOR_AUTO_RDO, m_nOverAutoUser);
	DDX_Control(pDX, IDC_OVERLOAD_FACTOR_CRI_AXLE_EDT, m_edtOverCriAxle);
	DDX_Control(pDX, IDC_OVERLOAD_FACTOR_OTHER_AXLE_EDT, m_edtOverOtherAxle);

	DDX_Radio  (pDX, IDC_VEHICLE_SPEED_NORMAL_RDO, m_nDistVehl);	

	DDX_Check  (pDX, IDC_CMD_MVHL_PATCH_CHK, m_bPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSCS458Page::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSCS458Page::SetData2Dlg()
{
	T_MVHL_D MvhlD = *m_pData;
	m_wndGrid1.SetDataSource(&MvhlD,this,0,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(MvhlD,0), m_nLoadType);
	m_wndGrid2.SetDataSource(&MvhlD,this,1,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(MvhlD,1), m_nLoadType);

	m_bDynamic = m_pData->bDynamicFactor;
	m_nDynaAutoUser = (m_pData->bUserInput) ? 1 : 0;
	m_edtDynaPhi.SetEditUnit(m_pData->dAmplification);

	m_bOverload = m_pData->bOverLoad;
	m_nOverAutoUser = m_pData->nOverLoad;
	m_edtOverCriAxle.SetEditUnit(m_pData->dCriticalAxle);
	m_edtOverOtherAxle.SetEditUnit(m_pData->dOtherAxle);
	
	m_nDistVehl = m_pData->nVehiSpeed;

	if (m_pData->PatchLoad.aPatchLoad.GetSize() > 0)
	{
		m_bPatch = m_pData->PatchLoad.bPatchLoad;
		m_edtPatchW.SetEditUnit(m_pData->PatchLoad.aPatchLoad[0].dWidth);
		m_edtPatchL.SetEditUnit(m_pData->PatchLoad.aPatchLoad[0].dLength);
	}
	else
	{
		m_bPatch = FALSE;
		m_edtPatchW.SetEditUnit(0.0);
		m_edtPatchL.SetEditUnit(0.0);
	}

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdBSCS458Page::SetDlg2Data()
{
	UpdateData(TRUE);

	m_wndGrid1.SaveData();
	m_wndGrid2.SaveData();

	m_pData->bDynamicFactor = m_bDynamic;
	m_pData->bUserInput = (m_nDynaAutoUser == 0) ? FALSE : TRUE;
	m_pData->dAmplification = m_edtDynaPhi.GetEditValue();

	m_pData->bOverLoad = m_bOverload;
	m_pData->nOverLoad = m_nOverAutoUser;
	m_pData->dCriticalAxle = m_edtOverCriAxle.GetEditValue();
	m_pData->dOtherAxle = m_edtOverOtherAxle.GetEditValue();

	m_pData->nVehiSpeed = m_nDistVehl;
	
	m_pData->PatchLoad.bPatchLoad = m_bPatch;

	T_MVHL_PATCH_ITEM Patch;
	Patch.dWidth = m_edtPatchW.GetEditValue();
	Patch.dLength = m_edtPatchL.GetEditValue();

	m_pData->PatchLoad.aPatchLoad.RemoveAll();
	m_pData->PatchLoad.aPatchLoad.Add(Patch);

	return TRUE;
}

void CCMMvhlItemStdBSCS458Page::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_AUTO_RDO)->EnableWindow(m_bDynamic);
	GetDlgItem(IDC_CMD_MVHL_USER_RDO)->EnableWindow(m_bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->EnableWindow(m_bDynamic && m_nDynaAutoUser == 1);
	GetDlgItem(IDC_CMD_MVHL_PHI_EDT)->EnableWindow(m_bDynamic && m_nDynaAutoUser == 1);

	GetDlgItem(IDC_OVERLOAD_FACTOR_AUTO_RDO)->EnableWindow(m_bOverload);
	GetDlgItem(IDC_OVERLOAD_FACTOR_USER_RDO)->EnableWindow(m_bOverload);
	GetDlgItem(IDC_OVERLOAD_FACTOR_CRI_AXLE_STC)->EnableWindow(m_bOverload && m_nOverAutoUser == 1);
	GetDlgItem(IDC_OVERLOAD_FACTOR_CRI_AXLE_EDT)->EnableWindow(m_bOverload && m_nOverAutoUser == 1);
	GetDlgItem(IDC_OVERLOAD_FACTOR_OTHER_AXLE_STC)->EnableWindow(m_bOverload && m_nOverAutoUser == 1);
	GetDlgItem(IDC_OVERLOAD_FACTOR_OTHER_AXLE_EDT)->EnableWindow(m_bOverload && m_nOverAutoUser == 1);

	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(m_bPatch);
}

void CCMMvhlItemStdBSCS458Page::ChangeText()
{
	T_MVHL_D MvhlD = *m_pData;
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(MvhlD, 0));
	GetDlgItem(IDC_CMD_MVHL_SECOND_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(MvhlD, 1));
	
	if (m_nLoadType == 0 || m_nLoadType == 1 || m_nLoadType == 2 || m_nLoadType == 3 || m_nLoadType == 4)
	{
		GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_SOSOV_TEXT_1));
	}
	else if (m_nLoadType == 6 || m_nLoadType == 7 || m_nLoadType == 8 || m_nLoadType == 9)
	{
		GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_SOSOV_TEXT_2));
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->SetWindowText(_T(""));
	}
}

void CCMMvhlItemStdBSCS458Page::SetLoadType(int nLoadType)
{
	m_nLoadType = nLoadType;
}

void CCMMvhlItemStdBSCS458Page::InitUnit()
{
	m_edtPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdBSCS458Page::InitGrid()
{
	m_wndGrid1.InitGrid(153);
	m_wndGrid2.InitGrid(153);
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSCS458Page, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSCS458Page)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNAMIC_CHK, OnDynamicChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_AUTO_RDO, OnDynaAutoUserRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_USER_RDO, OnDynaAutoUserRdo)

	ON_BN_CLICKED(IDC_OVERLOAD_FACTOR_CHK, OnOverloadChk)
	ON_BN_CLICKED(IDC_OVERLOAD_FACTOR_AUTO_RDO, OnOverAutoUserRdo)
	ON_BN_CLICKED(IDC_OVERLOAD_FACTOR_USER_RDO, OnOverAutoUserRdo)

	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK, OnPatchBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS458Page message handlers
BOOL CCMMvhlItemStdBSCS458Page::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();
	InitGrid();

	SetData2Dlg();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBSCS458Page::OnDynamicChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSCS458Page::OnDynaAutoUserRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSCS458Page::OnOverloadChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSCS458Page::OnOverAutoUserRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdBSCS458Page::OnPatchBtn()
{
	EnableDisableControls();
}