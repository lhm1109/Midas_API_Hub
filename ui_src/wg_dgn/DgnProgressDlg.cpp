// DgnProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnProgressDlg.h"
#include "DgnStruct.h"
#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_db\wg_db_DBLib.h"	// GetStlStatus(), GetSrcStatus().

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnProgressDlg dialog

CDgnProgressDlg::CDgnProgressDlg(CWnd* pParent /*=NULL*/)
    : CModelessMoveDlg(CDgnProgressDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDgnProgressDlg)
    m_strName = _T("");
    m_strPercent = _T("");
    //}}AFX_DATA_INIT
    m_iType = PROGRESS_TYPE_NONE;
    m_nPos = 0;
    m_bStopExecute = FALSE;
    m_bActOnUpdate = FALSE;
	m_nTotalSteps = 0;
	m_nCurrentStep = 0;
}

void CDgnProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CModelessMoveDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDgnProgressDlg)
    DDX_Control(pDX, IDC_DGN_PROGRESS_BAR, m_ProgressBar);
    DDX_Text(pDX, IDC_DGN_PROGRESS_STATIC, m_strName);
    DDX_Text(pDX, IDC_DGN_PERCENT_STATIC, m_strPercent);
    //}}AFX_DATA_MAP
}

#define CDialog CModelessMoveDlg
BEGIN_MESSAGE_MAP(CDgnProgressDlg, CInternationalDlg)
#undef CDialog

    //{{AFX_MSG_MAP(CDgnProgressDlg)
    ON_BN_CLICKED(IDC_DGN_STOP_BTN, OnDgnStopBtn)
    ON_MESSAGE(WM_USER_UPDATE_PROGRESS, OnUserFunc)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CDgnProgressDlg::OnUserFunc(WPARAM wParam, LPARAM lParam)
{
    AfxMessageBox(_T("test"));
    return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CDgnProgressDlg message handlers

BOOL CDgnProgressDlg::Create(CWnd* pParentWnd)
{
    // TODO: Add your specialized code here and/or call the base class
    return CModelessMoveDlg::Create(IDD, pParentWnd);
}

BOOL CDgnProgressDlg::OnInitDialog()
{
    CModelessMoveDlg::OnInitDialog();

    // TODO: Add extra initialization here
    // Set Progress Bar Range.
    m_ProgressBar.SetRange(0, 100);
    m_nPos = 0;
    // Set Progress Position.
    m_ProgressBar.SetPos(m_nPos);

	SetIgnoreBottomBtnArea();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnProgressDlg::PostNcDestroy()
{
    // TODO: Add your specialized code here and/or call the base class
    DestroyWindow();
    CModelessMoveDlg::PostNcDestroy();
}

#include "..\dgnengine\idesign\DGN_lib\DgnProgObserver.h"
void CDgnProgressDlg::OnDgnStopBtn()
{
    // TODO: Add your control notification handler code here
    m_bStopExecute = TRUE;

    dgn::lib::CDgnProgObserver::Instance()->SetStop(true);
}

void CDgnProgressDlg::Set_PosInc2(int iCur, int iTot)
{
	int iPos = 100 * iCur / iTot;
	if (iPos > 100)	iPos = 0;
	m_nPos = iPos;
	// Show Progress Position.
	TCHAR chPer[10]; //, chPerNa[2];
	CString sPerNa = _T(" %");
	wsprintf(chPer, _T("%d"), m_nPos);
	//wsprintf(chPerNa, _T("%s"), _T(" %"));
	//m_strPercent = CString(chPer) + CString(chPerNa);
	m_strPercent = CString(chPer) + sPerNa;
	// Set Position.
	DelayTreate(m_nPos);
	GetDlgItem(IDC_DGN_PROGRESS_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_PERCENT_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_PROGRESS_BAR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STOP_BTN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_PROGRESS_STATIC)->SetWindowText(m_strName);
	GetDlgItem(IDC_DGN_PERCENT_STATIC)->SetWindowText(m_strPercent);
	// UpdateData(FALSE);
}
void CDgnProgressDlg::Set_PosInc(int iCur, int iTot)
{
    int iPos = 100 * iCur / iTot;
    if (iPos > 100)	iPos = 0;
    m_nPos = iPos;
    // Show Progress Position.
    TCHAR chPer[10]; //, chPerNa[2];
    CString sPerNa = _T(" %");
    wsprintf(chPer, _T("%d"), m_nPos);
    //wsprintf(chPerNa, _T("%s"), _T(" %"));
    //m_strPercent = CString(chPer) + CString(chPerNa);
    m_strPercent = CString(chPer) + sPerNa;
    // Set Position.
    DelayTreate(m_nPos);
    GetDlgItem(IDC_DGN_PROGRESS_STATIC)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_DGN_PERCENT_STATIC)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_DGN_PROGRESS_BAR)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_DGN_STOP_BTN)->ShowWindow(SW_SHOW);

    UpdateData(FALSE);
}

void CDgnProgressDlg::Create_ProgressDlg(int iType, CWnd* pParentWnd)
{
    if (pParentWnd)
    {
        this->Create(pParentWnd);
    }
    else
    {
        CAppBase* pApp = (CAppBase*)AfxGetApp();
        CFrameWnd* pFrame = (CFrameWnd*)pApp->GetMainWnd();
        CView* pWnd = pFrame->GetActiveView();
        this->Create(pWnd);
    }
    m_pDlg = this;
    VERIFY(m_pDlg != NULL);

    m_iType = iType;
    switch (m_iType)
    {
    case PROGRESS_TYPE_STEEL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_STL_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_STL);
        break;
    case PROGRESS_TYPE_SRC:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_SRC_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SRC);
        break;
    case PROGRESS_TYPE_CFS:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CFS_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_CFS);
        break;
    case PROGRESS_TYPE_TOWER:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_TWR_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_TWR);
        break;
    case PROGRESS_TYPE_RC_BEAM_DGN:
    case PROGRESS_TYPE_RC_BEAM_DUCTILE_DGN:
    case PROGRESS_TYPE_RC_PSBEAM_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_BEAM_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_BEAM);
        break;
    case PROGRESS_TYPE_RC_BEAM_CHK:
    case PROGRESS_TYPE_RC_PSBEAM_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_BEAM_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_BEAM);
        break;
    case PROGRESS_TYPE_RC_COLUMN_DGN:
    case PROGRESS_TYPE_RC_COLUMN_DUCTILE_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_COLM_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_COLM);
        break;
    case PROGRESS_TYPE_RC_COLUMN_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_COLM_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_COLM);
        break;
    case PROGRESS_TYPE_RC_BRACE_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_BRCE_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_BRCE);
        break;
    case PROGRESS_TYPE_RC_BRACE_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_BRCE_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_BRCE);
        break;
    case PROGRESS_TYPE_RC_WALL_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_WALL_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_WALL);
        break;
    case PROGRESS_TYPE_RC_WALL_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_WALL_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_WALL);
        break;
    case PROGRESS_TYPE_RC_SRC_BEAM_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_SRC_BEAM_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SRCBEAM);
        break;
    case PROGRESS_TYPE_RC_HAUNCH_BEAM_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_HCBM_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_HCBM);
        break;
    case PROGRESS_TYPE_RC_SLAB_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_SLAB);
        break;
    case PROGRESS_TYPE_RC_SLAB_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SLAB);
        break;
    case PROGRESS_TYPE_RC_SLAB_SERV_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_SERV_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SLAB);
        break;
    case PROGRESS_TYPE_RC_SLAB_PUNCHING_FORCE:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_PUNCH_CHK_FORCE));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SLAB);
        break;
    case PROGRESS_TYPE_RC_SLAB_PUNCHING_STRESS:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_PUNCH_CHK_STRESS));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SLAB);
        break;
    case PROGRESS_TYPE_RC_SLAB_PUNCHING_MAKE_FORCE:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_PUNCH_CHK_MAKE_FORCE));
        m_strName = _LS(IDS_DGN_TEXT_MAKE_FORCE_SLAB);
        break;
    case PROGRESS_TYPE_RC_SLAB_PUNCHING_MAKE_STRESS:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_PUNCH_CHK_MAKE_STRESS));
        m_strName = _LS(IDS_DGN_TEXT_MAKE_FORCE_SLAB);
        break;
    case PROGRESS_TYPE_RC_MESHED_WALL_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_WALL_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_WALL);
        break;
    case PROGRESS_TYPE_RC_MESHED_WALL_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_WALL_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_WALL);
        break;
    case PROGRESS_TYPE_STEEL_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_ACS_STEEL_XL_REPORT));
        m_strName = _LS(IDS_DGN_ACS_STEEL_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_RC_BEAM_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RCS_BEAM_XL_REPORT));
        m_strName = _LS(IDS_DGN_RCS_BEAM_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_RC_COLUMN_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RCS_COLM_XL_REPORT));
        m_strName = _LS(IDS_DGN_RCS_COLM_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_RC_WALL_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RCS_WALL_XL_REPORT));
        m_strName = _LS(IDS_DGN_RCS_WALL_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_RC_PLATE_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RCS_PLATE_XL_REPORT));
        m_strName = _LS(IDS_DGN_RCS_PLATE_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_PSC_RATING_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_PSC_RATING_XL_REPORT));
        m_strName = _LS(IDS_DGN_PSC_RATING_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_RC_RATING_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RC_RATING_XL_REPORT));
        m_strName = _LS(IDS_DGN_RC_RATING_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_RC_INC_SLAB_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SLAB);
        break;
    case PROGRESS_TYPE_RC_INC_SLAB_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SLAB_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SLAB);
        break;
    case PROGRESS_TYPE_JFD_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_JFD_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_JFD);
        break;
    case PROGRESS_TYPE_JFD_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_JFD_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_JFD);
        break;
    case PROGRESS_TYPE_STRIP_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_STRIP_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_STRIP);
        break;
    case PROGRESS_TYPE_STRIP_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_STRIP_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_STRIP);
        break;
    case PROGRESS_TYPE_STRIP_CALC_FORCE:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_STRIP_FOR));
        m_strName = _LS(IDS_DGN_TEXT_CALC_FORCE_STRIP);
        break;
    case PROGRESS_TYPE_RLC_ANALYSIS_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RLC_ANALYSIS_CHK));
        m_strName = _LS(IDS_DGN_RLC_CHECK_READY);
        break;
    case PROGRESS_TYPE_RLC_ANALYSIS_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RLC_ANALYSIS_PRINT));
        m_strName = _LS(IDS_DGN_RLC_REPORT_READY);
        break;
    case PROGRESS_TYPE_RC_SHELL_FLEX_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SHELL_DGN));
        m_strName = _LS(IDS_DGN_TEXT_READ_DGN_SHELL);
        break;
    case PROGRESS_TYPE_RC_SHELL_FLEX_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SHELL_CHK));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SHELL);
        break;
    case PROGRESS_TYPE_RC_SHELL_SHEAR_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CON_SHELL_SHR));
        m_strName = _LS(IDS_DGN_TEXT_READ_CHK_SHELL);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_PRE_CONCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_PRE_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_PRE_CONWALL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_PRE_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_WALL_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_PRE_STLCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_PRE_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_PRE_STLBRAC:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_PRE_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_BRAC_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_LIN_CONBEAM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_LIN_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_BEAM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_LIN_CONCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_LIN_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_LIN_CONWALL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_LIN_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_WALL_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_LIN_STLBEAM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_LIN_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_BEAM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_LIN_STLCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_LIN_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_LIN_STLBRAC:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_LIN_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_BRAC_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_LIN_INFWALL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_LIN_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_INF_WALL_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_CONBEAM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_BEAM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_CONCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_CONWALL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_WALL_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_STLBEAM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_BEAM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_STLCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_STLPANZ:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_JONT_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_STLBRAC:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_BRAC_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_1ST_INFWALL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_1ST_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_INF_WALL_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_2ND_CONBEAM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_2ND_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_BEAM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_2ND_CONCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_2ND_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_2ND_CONWALL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_2ND_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_RC_WALL_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_2ND_STLBEAM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_2ND_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_BEAM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_2ND_STLCOLM:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_2ND_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_COLM_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_2ND_STLBRAC:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_2ND_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_STL_BRAC_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_2ND_INFWALL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_2ND_EVALUATION));
        m_strName = _LS(IDS_DGN_TEXT_INF_WALL_EVALUATION);
        break;
    case PROGRESS_TYPE_SEIS_EVAL_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_SEIS_EVAL_XL_REPORT));
        m_strName = _LS(IDS_DGN_SEIS_EVAL_XL_REPORT_READY);
        break;

    case PROGRESS_TYPE_BRDG_SEIS_EVAL:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_BRDG_SEIS_EVAL));
        m_strName = _LS(IDS_DGN_TEXT_CHK_BRDG_SEIS_EVAL);
        break;
    case PROGRESS_TYPE_BRDG_SEIS_EVAL_BEARING:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_BRDG_SEIS_EVAL_BEARING));
        m_strName = _LS(IDS_DGN_TEXT_CHK_BRDG_SEIS_EVAL_BEARING);
        break;
    case PROGRESS_TYPE_BRDG_SEIS_EVAL_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_BRDG_SEIS_EVAL_XL_REPORT));
        m_strName = _LS(IDS_DGN_BRDG_SEIS_EVAL_RATING_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_STEEL_PFDN_OMEGA:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_STL_CHK));
        m_strName = _LS(IDS_DGN_TEXT_PFDN_OMEGA);
        break;
	case PROGRESS_TYPE_CFS_PFDN_OMEGA:
		m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_CFS_CHK));
		m_strName = _LS(IDS_DGN_TEXT_PFDN_OMEGA);
		break;
    case PROGRESS_TYPE_STEEL_FIRE_PREVENTION:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_STL_CHK));
        m_strName = _LS(IDS_DGN_TEXT_FIRE_PREVENTION);
        break;
    case PROGRESS_TYPE_PSC_DGN_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_PSC_XL_REPORT));
        m_strName = _LS(IDS_DGN_PSC_XL_REPORT_READY);
        break;
    case PROGRESS_TYPE_PSC_RATING:
        m_pDlg->SetWindowText(_LS(IDS_DGN_PSC_RATING_DGN));
        m_strName = _LS(IDS_DGN_TEXT_PERFORM_MSG_PREPARE_DGN);
        break;
    case PROGRESS_TYPE_DGNPERFORM_THREAD:
        m_pDlg->SetWindowText(_LS(IDS_DGN_STOP_DGN_THREAD));
        m_strName = _LS(IDS_DGN_TEXT_PERFORM_MSG_PREPARE_DGN);
        break;
    case PROGRESS_TYPE_BRDG_SEIS_EVAL_JP:
		m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_BRDG_SEIS_EVAL));
		m_strName = _LS(IDS_DGN_TEXT_CHK_BRDG_SEIS_EVAL);
        break;
    case PROGRESS_TYPE_BRDG_SEIS_EVAL_XL_RPT_JP:
		m_pDlg->SetWindowText(_LS(IDS_DGN_BRDG_SEIS_EVAL_XL_REPORT));
		m_strName = _LS(IDS_DGN_BRDG_SEIS_EVAL_RATING_XL_REPORT_READY);
        break;
    default:
        ASSERT(0);
        break;
    }

	m_pDlg->CenterWindow();

    BOOL bShowDlg = TRUE;
    if (m_iType == PROGRESS_TYPE_STEEL)				    bShowDlg = (CDBLib::GetStlStatus() == 1 || CDBLib::GetTowerODS() ? TRUE : FALSE);
    else if (m_iType == PROGRESS_TYPE_SRC)	        	bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
    else if (m_iType == PROGRESS_TYPE_RC_SRC_BEAM_CHK)	bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
    else
    {
        bShowDlg = CDBLib::IsIncludeMessageSkipRange() ? FALSE : TRUE;
    }

    if (bShowDlg)	m_pDlg->ShowWindow(SW_SHOW);
    else			m_pDlg->ShowWindow(SW_HIDE);

    UpdateWindow();
    UpdateData(FALSE);
}

void CDgnProgressDlg::Create_ProgressDlgCVL(int iType, CWnd* pParentWnd)
{
    if (pParentWnd)
    {
        this->Create(pParentWnd);
    }
    else
    {
        CAppBase* pApp = (CAppBase*)AfxGetApp();
        CFrameWnd* pFrame = (CFrameWnd*)pApp->GetMainWnd();
        CView* pWnd = pFrame->GetActiveView();
        this->Create(pWnd);
    }
    m_pDlg = this;
    VERIFY(m_pDlg != NULL);

    m_iType = iType;
    switch (m_iType)
    {
    case PROGRESS_TYPE_RC_PLATE_BEAM_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_EXECUTE_PLATE_BEAM_DGN)); //!/ 임시.. 메세지 수정해야함
        m_strName = _LS(IDS_DGN_TEXT_READ_PLATE_BEAM);
        break;
    case PROGRESS_TYPE_RC_PLATE_BEAM_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_EXECUTE_PLATE_BEAM_CHK)); //!/ 임시.. 메세지 수정해야함
        m_strName = _LS(IDS_DGN_TEXT_READ_PLATE_BEAM);
        break;
    case PROGRESS_TYPE_RC_PLATE_COLUMN_DGN:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_EXECUTE_PLATE_COLM_DGN)); //!/ 임시.. 메세지 수정해야함
        m_strName = _LS(IDS_DGN_TEXT_READ_PLATE_COLM);
        break;
    case PROGRESS_TYPE_RC_PLATE_COLUMN_CHK:
        m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_EXECUTE_PLATE_COLM_CHK)); //!/ 임시.. 메세지 수정해야함
        m_strName = _LS(IDS_DGN_TEXT_READ_PLATE_COLM);
        break;
    case PROGRESS_TYPE_RC_PLATE_XL_RPT:
        m_pDlg->SetWindowText(_LS(IDS_DGN_RCS_PLATE_XL_REPORT));
        m_strName = _LS(IDS_DGN_RCS_PLATE_XL_REPORT_READY);
        break;
	case PROGRESS_TYPE_STEEL:
		m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_STL_CHK));
		m_strName = _LS(IDS_DGN_TEXT_READ_CHK_STL);
		break;
	case PROGRESS_TYPE_STEEL_COMPOSITE:
		m_pDlg->SetWindowText(_LS(IDS_DGN_TEXT_STOP_STL_COMPOSITE_CHK));
		m_strName = _LS(IDS_DGN_TEXT_READ_CHK_STL_COMPOSITE);
		break;
    default:
        ASSERT(0);
        break;
	}

	m_pDlg->CenterWindow();

    BOOL bShowDlg = TRUE;

    if (bShowDlg)	m_pDlg->ShowWindow(SW_SHOW);
    else			m_pDlg->ShowWindow(SW_HIDE);

    UpdateWindow();
    UpdateData(FALSE);
}
BOOL CDgnProgressDlg::Update_ProgressDlg2(int iCur, int iTot)
{
	BOOL bStop = FALSE;
	BOOL bShowDlg = TRUE;

#ifdef _CIVIL
	bShowDlg = TRUE;
#else
	if (m_iType == PROGRESS_TYPE_STEEL)				    bShowDlg = (CDBLib::GetStlStatus() == 1 || CDBLib::GetTowerODS() ? TRUE : FALSE);
	else if (m_iType == PROGRESS_TYPE_SRC)	   	        bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
	else if (m_iType == PROGRESS_TYPE_RC_SRC_BEAM_CHK)	bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
#endif

	if (bShowDlg)
	{
		if (m_pDlg->IsWindowVisible())	m_pDlg->Set_PosInc2(iCur, iTot);
		if (m_pDlg->m_bStopExecute)		bStop = TRUE;
	}
	PumpMessages();
	UpdateWindow();

	if (m_bActOnUpdate) SetActiveWindow();

	return bStop;
}

BOOL CDgnProgressDlg::NextStep()
{
    if (m_ProgressBar.GetState() == PBST_PAUSED) return TRUE;
    if (m_nCurrentStep >= m_nTotalSteps) return TRUE;
    return !Update_ProgressDlg(++m_nCurrentStep, m_nTotalSteps);
}

BOOL CDgnProgressDlg::Update_ProgressDlg(int iCur, int iTot)
{
    BOOL bStop = FALSE;
    BOOL bShowDlg = TRUE;

#ifdef _CIVIL
	bShowDlg = TRUE;
#else
    if (m_iType == PROGRESS_TYPE_STEEL)				    bShowDlg = (CDBLib::GetStlStatus() == 1 || CDBLib::GetTowerODS() ? TRUE : FALSE);
    else if (m_iType == PROGRESS_TYPE_SRC)	   	        bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
    else if (m_iType == PROGRESS_TYPE_RC_SRC_BEAM_CHK)	bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
#endif

    if (bShowDlg)
    {
        if (m_pDlg->IsWindowVisible())	m_pDlg->Set_PosInc(iCur, iTot);
        if (m_pDlg->m_bStopExecute)		bStop = TRUE;
    }
    PumpMessages();
    UpdateWindow();

    if (m_bActOnUpdate) SetActiveWindow();

    return bStop;
}

BOOL CDgnProgressDlg::Update_ProgressDlg(CString strMessage, int iCur, int iTot)
{
    BOOL bStop = FALSE;
    BOOL bShowDlg = TRUE;
    if (m_iType == PROGRESS_TYPE_STEEL)				    bShowDlg = (CDBLib::GetStlStatus() == 1 || CDBLib::GetTowerODS() ? TRUE : FALSE);
    else if (m_iType == PROGRESS_TYPE_SRC)	   	        bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
    else if (m_iType == PROGRESS_TYPE_RC_SRC_BEAM_CHK)	bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
    if (bShowDlg)
    {
        if (m_pDlg->IsWindowVisible())
        {
            if (m_strName != strMessage &&strMessage.GetLength() > 0)
            {
                m_strName = strMessage;
                GetDlgItem(IDC_DGN_PROGRESS_STATIC)->SetWindowText(m_strName);
            }
            m_pDlg->Set_PosInc(iCur, iTot);
        }
        if (m_pDlg->m_bStopExecute)		bStop = TRUE;
    }
    PumpMessages();
    UpdateWindow();

    if (m_bActOnUpdate) SetActiveWindow();

    return bStop;
}

void CDgnProgressDlg::Update_ProgressDlg_Subject(CString strMessage)
{
	BOOL bShowDlg = TRUE;
	if (m_iType == PROGRESS_TYPE_STEEL)				    bShowDlg = (CDBLib::GetStlStatus() == 1 || CDBLib::GetTowerODS() ? TRUE : FALSE);
	else if (m_iType == PROGRESS_TYPE_SRC)	   	        bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
	else if (m_iType == PROGRESS_TYPE_RC_SRC_BEAM_CHK)	bShowDlg = (CDBLib::GetSrcStatus() == 1 ? TRUE : FALSE);
	if (bShowDlg)
	{
		if (m_pDlg->IsWindowVisible())
		{
			if (m_strName != strMessage && strMessage.GetLength() > 0)
			{
				m_strName = strMessage;
				GetDlgItem(IDC_DGN_PROGRESS_STATIC)->SetWindowText(m_strName);
			}
		}
	}
	PumpMessages();
	UpdateWindow();

	if (m_bActOnUpdate) SetActiveWindow();
}

void CDgnProgressDlg::Delete_ProgressDlg()
{
    m_pDlg->ShowWindow(SW_HIDE);
    m_pDlg->DestroyWindow();
}

void CDgnProgressDlg::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd != NULL);
    MSG msg;
    // Handle dialog messages
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (!IsDialogMessage(&msg))
        {
            if (msg.message == WM_SYSCOMMAND)	return;
            if (msg.message == WM_CLOSE)				return;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    /*
        MSG msg;
        if(::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
        {
            VERIFY(::GetMessage(&msg, NULL, 0, 0));
            if(msg.message == WM_SYSCOMMAND)	return bStop;
            if(msg.message == WM_CLOSE)				return bStop;
            DispatchMessage(&msg);
        }
    */
}
//////////////////////////////////////////////////////////////////////////

void CDgnProgressDlg::DelayTreate(const int& iPercent)
{
    if (iPercent == 100)
    {
        m_ProgressBar.SetRange(0, 101);
        m_ProgressBar.SetPos(iPercent + 1);
        m_ProgressBar.SetPos(iPercent);
        m_ProgressBar.SetRange(0, 100);
    }
    else
    {
        m_ProgressBar.SetPos(iPercent + 1);
        m_ProgressBar.SetPos(iPercent);
    }
}

void CDgnProgressDlg::Set_PosInc(CString strMessage, int iPercent)
{
    if (iPercent >= 0)
    {
        if (iPercent > 100)	iPercent = 0;
        m_nPos = iPercent;
        // Show Progress Position.
        TCHAR chPer[3], chPerNa[2];
        wsprintf(chPer, _T("%d"), m_nPos);
        wsprintf(chPerNa, _T("%s"), _T(" %"));
        m_strPercent = CString(chPer) + CString(chPerNa);
    }
    if (strMessage.GetLength() > 0)
    {
        m_strName = strMessage;
    }
    // Set Position.
    DelayTreate(m_nPos);
    GetDlgItem(IDC_DGN_PROGRESS_STATIC)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_DGN_PERCENT_STATIC)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_DGN_PROGRESS_BAR)->ShowWindow(SW_SHOW);

    UpdateData(FALSE);
}

void CDgnProgressDlg::Set_DlgData(CString strTitle, CString strMessage, int iPercent)
{
    if (strTitle.GetLength() > 0)
    {
        m_pDlg->SetWindowText(strTitle);
    }
    if (iPercent >= 0)
    {
        if (iPercent > 100)	iPercent = 0;
        m_nPos = iPercent;
        // Show Progress Position.
        m_strPercent = _T("");
        m_strPercent.Format(_T("%d"), m_nPos);
        m_strPercent += _T("%");

        DelayTreate(m_nPos);
        GetDlgItem(IDC_DGN_PERCENT_STATIC)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_DGN_PERCENT_STATIC)->SetWindowText(m_strPercent);
        GetDlgItem(IDC_DGN_PROGRESS_BAR)->ShowWindow(SW_SHOW);
    }
    if (strMessage.GetLength() > 0)
    {
        m_strName = strMessage;
        GetDlgItem(IDC_DGN_PROGRESS_STATIC)->SetWindowText(m_strName);
        GetDlgItem(IDC_DGN_PROGRESS_STATIC)->ShowWindow(SW_SHOW);
    }

    UpdateData(FALSE);
}

void CDgnProgressDlg::Create_ProgressDlg(CString strWindow, CString strMessage, BOOL bUseStopBtn)
{
    CAppBase* pApp = (CAppBase*)AfxGetApp();
    CFrameWnd* pFrame = (CFrameWnd*)pApp->GetMainWnd();
    CView* pWnd = pFrame->GetActiveView();
    this->Create(pWnd);
    m_pDlg = this;
    VERIFY(m_pDlg != NULL);

    if (!bUseStopBtn)
    {
        m_pDlg->GetDlgItem(IDC_DGN_STOP_BTN)->ShowWindow(SW_HIDE);
        CRect ProgressRect, StopBtnRect;
        GetDlgItem(IDC_DGN_PROGRESS_BAR)->GetWindowRect(ProgressRect);
        GetDlgItem(IDC_DGN_STOP_BTN)->GetWindowRect(StopBtnRect);
        GetDlgItem(IDC_DGN_PROGRESS_BAR)->SetWindowPos(NULL, 0, 0, ProgressRect.Width() + StopBtnRect.Width(), ProgressRect.Height(), SWP_NOMOVE);
    }

    Set_DlgData(strWindow, strMessage, -1);

    BOOL bShowDlg = TRUE;

    if (bShowDlg)	m_pDlg->ShowWindow(SW_SHOW);
    else			m_pDlg->ShowWindow(SW_HIDE);

    UpdateWindow();
    UpdateData(FALSE);
}

BOOL CDgnProgressDlg::Update_ProgressDlg(CString strWindow, CString strMessage, int iPercent)
{
    BOOL bStop = FALSE;
    BOOL bShowDlg = TRUE;

    if (bShowDlg)
    {
        if (m_pDlg->IsWindowVisible())
        {
            if (strWindow.GetLength() > 0) m_pDlg->SetWindowText(strWindow);
            m_pDlg->Set_PosInc(strMessage, iPercent);
        }
        if (m_pDlg->m_bStopExecute)
            bStop = TRUE;
    }
    PumpMessages();
    UpdateWindow();

    return bStop;
}
BOOL CDgnProgressDlg::Update_ProgressDlg(CString strMessage, int iPercent)
{
    return Update_ProgressDlg(_T(""), strMessage, iPercent);
}

#pragma region // for DgnPerfom thread
// Test code
DWORD CDgnProgressDlg::ThreadProgress(LPVOID IpParam)
{
    auto* pSubject = dgn::lib::CDgnProgObserver::Instance();
    if (pSubject == nullptr) return 0;

    CDgnEngineThreadObserver Observer;
    CDgnProgressDlg ProDlg;
    int* iType = (int*)IpParam;
    ProDlg.Create_ProgressDlg(*iType);

	int iIdx = 0;
	auto dSt = ::clock();
    while (true)
    {
        //Sleep(50);
        if (Observer.IsUpdate())
        {
            Observer.SetUpdateFalse();
            CString strMessage = CW2A(pSubject->GetCurrentSubject());
            int nRat = static_cast<int>(pSubject->GetCurrentRat()*100.0);
            if (ProDlg.Update_ProgressDlg(strMessage, nRat, 100))
            {
                pSubject->SetStop(true);
                break;
            }
        }
		else
		{
			int nRat = static_cast<int>(pSubject->GetCurrentRat()*100.0);
			if(nRat == 0)
			{
				auto dEd = ::clock();
				auto dDiff = dEd - dSt;
				if(dDiff > 200.0)
				{
					CString strMessage = CW2A(pSubject->GetCurrentSubjectNoDot());
					strMessage += [&iIdx]()
					{
						switch (iIdx)
						{
							case 0: iIdx++; return _T(".");
							case 1: iIdx++; return _T("..");
							case 2: iIdx++; return _T("...");
							case 3: iIdx=0; break;
						}
						return _T("");//(TCHAR)0x0000;
					}();
					ProDlg.Update_ProgressDlg_Subject(strMessage);

					dSt = ::clock();
				}
			}
		}
        if (pSubject->IsEnd())
        {
            break;
        }
    }
    ProDlg.Delete_ProgressDlg();
    return 0;
};
#pragma endregion