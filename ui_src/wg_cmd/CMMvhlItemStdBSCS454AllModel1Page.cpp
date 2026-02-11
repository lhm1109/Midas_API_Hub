// CMMvhlItemStdBSCS454AllModel1Page.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdBSCS454AllModel1Page.h"
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
// CCMMvhlItemStdBSCS454AllModel1Page dialog

CCMMvhlItemStdBSCS454AllModel1Page::CCMMvhlItemStdBSCS454AllModel1Page(T_MVHL_D* pData, CWnd* pParent /*=NULL*/)
	: CDialog(CCMMvhlItemStdBSCS454AllModel1Page::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdBSCS454AllModel1Page)
	m_pParent = (CCMMvhlItemStdBSDlg*) pParent;

	m_nLoadCase = 0;
	m_nRoadSuf = 0;
	m_nTrafficFlow = 0;
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_nLoadType = 0;
	m_nSubType = 0;
}

void CCMMvhlItemStdBSCS454AllModel1Page::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdBSCS454AllModel1Page)
 	DDX_Control(pDX, IDC_CMD_ALLMODEL1_GRID, m_wndGrid);
 	DDX_Control(pDX, IDC_CMD_MVHL_ALLMODEL1_O1_EDIT, m_edtQ1);
	DDX_Control(pDX, IDC_CMD_MVHL_ALLMODEL1_O1_UNIT, m_untQ1);
	DDX_Control(pDX, IDC_CMD_MVHL_ALLMODEL1_O2_EDIT, m_edtQ2);
	DDX_Control(pDX, IDC_CMD_MVHL_ALLMODEL1_O2_UNIT, m_untQ2);
	DDX_Radio  (pDX, IDC_ALLMODEL1_CASE_SINGLE_RDO, m_nLoadCase);
	DDX_Radio  (pDX, IDC_ALLMODEL1_SURFACE_GOOD_RDO, m_nRoadSuf);
	DDX_Radio  (pDX, IDC_ALLMODEL1_TRAFFIC_HIGH_RDO, m_nTrafficFlow);
	DDX_Check  (pDX, IDC_CMD_MVHL_PATCH_CHK, m_bPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdBSCS454AllModel1Page::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 13)
		pMsg->wParam = 9;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = VK_TAB;

	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemStdBSCS454AllModel1Page::SetData2Dlg()
{
	T_MVHL_D MvhlD = *m_pData;
	m_wndGrid.SetDataSource(&MvhlD, this, 0, CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(MvhlD, 0), m_nLoadType);
		
	int nD = 0;
	for (int i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_pData->dPointLoad3[0][i] < 1.0E-7) { nD = i;  break; }
	}

	m_edtQ1.SetEditUnit(m_pData->dPointDistance3[0][nD - 1]);
	m_edtQ2.SetEditUnit(m_pData->dPointDistance3[0][nD]);

	m_nLoadCase = m_pData->nLM1Case;
	m_nRoadSuf = m_pData->nLM1Surface;
	m_nTrafficFlow = m_pData->nLM1Category;

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

BOOL CCMMvhlItemStdBSCS454AllModel1Page::SetDlg2Data()
{
	UpdateData(TRUE);

	m_wndGrid.SaveData();

	int nD = 0;
	for (int i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_pData->dPointLoad3[0][i] < 1.0E-7) { nD = i;  break; }
	}

	m_pData->dPointDistance3[0][nD - 1] = m_edtQ1.GetEditValue();
	m_pData->dPointDistance3[0][nD] = m_edtQ2.GetEditValue();

	m_pData->nLM1Case = m_nLoadCase;
	m_pData->nLM1Surface = m_nRoadSuf;
	m_pData->nLM1Category = m_nTrafficFlow;

	m_pData->PatchLoad.bPatchLoad = m_bPatch;

	T_MVHL_PATCH_ITEM Patch;
	Patch.dWidth = m_edtPatchW.GetEditValue();
	Patch.dLength = m_edtPatchL.GetEditValue();

	m_pData->PatchLoad.aPatchLoad.RemoveAll();
	m_pData->PatchLoad.aPatchLoad.Add(Patch);

	return TRUE;
}

void CCMMvhlItemStdBSCS454AllModel1Page::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(m_bPatch);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(m_bPatch);
}

void CCMMvhlItemStdBSCS454AllModel1Page::ChangeText()
{
	T_MVHL_D MvhlD = *m_pData;
	GetDlgItem(IDC_CMD_MVHL_ALLMODEL1_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(MvhlD, 0));
}

void CCMMvhlItemStdBSCS454AllModel1Page::SetLoadandSubType(int nLoadType, int nSubType)
{
	m_nLoadType = nLoadType;
	m_nSubType = nSubType;
}

void CCMMvhlItemStdBSCS454AllModel1Page::InitUnit()
{
	m_edtQ1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untQ1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtQ2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untQ2.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdBSCS454AllModel1Page::InitGrid()
{
	m_wndGrid.InitGrid(160);
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdBSCS454AllModel1Page, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemStdBSCS454AllModel1Page)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK, OnPatchBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS454AllModel1Page message handlers
BOOL CCMMvhlItemStdBSCS454AllModel1Page::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUnit();
	InitGrid();

	SetData2Dlg();

	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdBSCS454AllModel1Page::OnPatchBtn()
{
	EnableDisableControls();
}