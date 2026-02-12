// CMMvctCHArchPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHArchPage.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMMvctCHArchPage dialog

CCMMvctCHArchPage::CCMMvctCHArchPage(T_FREQ_ARCH* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMMvctCHArchPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHArchPage)
	//}}AFX_DATA_INIT

	m_pData = NULL;
	m_pData = pData;

	m_nArchType = 0;
	m_bImpactFactor = TRUE;
}

void CCMMvctCHArchPage::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHArchPage)
	DDX_Control(pDX, IDC_CMD_MVCT_N_EDT,            m_edtn);
	DDX_Control(pDX, IDC_CMD_MVCT_F_EDT,            m_edtf);
	DDX_Control(pDX, IDC_CMD_MVCT_L_EDT,            m_edtL);
	DDX_Control(pDX, IDC_CMD_MVCT_E_EDT,            m_edtE);
	DDX_Control(pDX, IDC_CMD_MVCT_FC_EDT,           m_edtIc);
	DDX_Control(pDX, IDC_CMD_MVCT_MC_EDT,           m_edtmc);
	DDX_Control(pDX, IDC_CMD_MVCT_BMP1,             m_ImgViewer1);
	DDX_Control(pDX, IDC_CMD_MVCT_BMP2,             m_ImgViewer2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvctCHArchPage, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMMvctCHArchPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvctCHArchPage::SetData2Dlg()
{
	m_edtn.SetEditUnit(m_pData->dn);
	m_edtf.SetEditUnit(m_pData->df);
	m_edtL.SetEditUnit(m_pData->dL);
	m_edtE.SetEditUnit(m_pData->dE);
	m_edtIc.SetEditUnit(m_pData->dIc);
	m_edtmc.SetEditUnit(m_pData->dmc);
	
	UpdateData(FALSE);
}

BOOL CCMMvctCHArchPage::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->dn = m_edtn.GetEditValue();
	m_pData->df = m_edtf.GetEditValue();
	m_pData->dL = m_edtL.GetEditValue();
	m_pData->dE = m_edtE.GetEditValue();
	m_pData->dIc = m_edtIc.GetEditValue();
	m_pData->dmc = m_edtmc.GetEditValue();
	
	return TRUE;
}

void CCMMvctCHArchPage::InitUnit()
{
	m_edtn.SetUnitType(D_UNITSYS_NONE);
	m_edtf.SetUnitType(D_UNITSYS_NONE);
	m_edtL.SetUnitType(D_UNITSYS_NONE);
	m_edtE.SetUnitType(D_UNITSYS_NONE);
	m_edtIc.SetUnitType(D_UNITSYS_NONE);
	m_edtmc.SetUnitType(D_UNITSYS_NONE);
}

void CCMMvctCHArchPage::AlignControl()
{
	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVCT_BMP2);

	CRect rTo, rMove;
	int nDistX = 0;
	int nDistY = 0;

	GetDlgItem(IDC_CMD_MVCT_BMP1)->GetWindowRect(rTo);
	GetDlgItem(IDC_CMD_MVCT_BMP2)->GetWindowRect(rMove);
	nDistX = rTo.left-rMove.left;
	nDistY = rTo.top-rMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);
}

void CCMMvctCHArchPage::ControlsShowHide(int nArchType)
{
	m_nArchType = nArchType;

	GetDlgItem(IDC_CMD_MVCT_BMP1)->ShowWindow(m_nArchType == 0);
	GetDlgItem(IDC_CMD_MVCT_BMP2)->ShowWindow(m_nArchType == 1);
}

void CCMMvctCHArchPage::EnableDisableControls(BOOL bImpactFactor, BOOL bArchType/* = FALSE*/)
{
	m_bImpactFactor = bImpactFactor;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	CDlgUtil::GetCtrlAllIDbyDlg(this, aControls);
	CDlgUtil::CtrlEnableDisable(this, aControls, m_bImpactFactor);

	if (bArchType)
	{
		// 삽도 동작을 위해 추가
		SetRedraw(TRUE); // 다시 그릴 수 있도록 설정
		RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW); // 즉시 다시 그리기
	}
}

void CCMMvctCHArchPage::EnableDisableComponent(BOOL bEnable)
{
	m_edtn .EnableWindow(bEnable);
	m_edtf .EnableWindow(bEnable);
	m_edtL .EnableWindow(bEnable);
	m_edtE .EnableWindow(bEnable);
	m_edtIc.EnableWindow(bEnable);
	m_edtmc.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHArchPage message handlers
BOOL CCMMvctCHArchPage::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	InitUnit();
	AlignControl();

	CString strImgPath1, strImgPath2;
	
	strImgPath1 = _T("SVG\\illustration\\Dialog\\cmd_mvctch5.svg");
	strImgPath2 = _T("SVG\\illustration\\Dialog\\cmd_mvctch6.svg");

	m_ImgViewer1.SetImage(strImgPath1);
	m_ImgViewer2.SetImage(strImgPath2);
	
	SetData2Dlg();

	ControlsShowHide(m_nArchType);
	EnableDisableControls(m_bImpactFactor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
