// AssessmentLoadCombSTLNRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "AssessmentLoadCombSTLNRDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_base\ModelessDlgCtrl.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "RatingLoadCombUtil.h"
#include "AssessmentLoadCombMainGridSTLNR.h"
#include "AssessmentLoadCombSubGridSTLNR.h"
#include "..\wg_db\AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombSTLNRDlg dialog
CAssessmentLoadCombSTLNRDlg::CAssessmentLoadCombSTLNRDlg(CWnd* pParent)
	: CDgnDlgBase(CAssessmentLoadCombSTLNRDlg::IDD, pParent)
{
	// *^^*  여기를 바꾸시오.

	//{{AFX_DATA_INIT(CAssessmentLoadCombSTLNRDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bStatic = TRUE;
	m_bDynamic = TRUE;

	m_pDoc = CDBDoc::GetDocPoint();

	m_pLoadCombList = new CAssessmentLoadCombMainGridSTLNR(this);

	m_pContentList = new CAssessmentLoadCombSubGridSTLNR();
	m_bSetGridEditMode = FALSE;
}

CAssessmentLoadCombSTLNRDlg::~CAssessmentLoadCombSTLNRDlg()
{
	if (m_pLoadCombList)
	{
		delete m_pLoadCombList;
		m_pLoadCombList = NULL;
	}
	if (m_pContentList)
	{
		delete m_pContentList;
		m_pContentList = NULL;
	}
}

void CAssessmentLoadCombSTLNRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssessmentLoadCombSTLNRDlg)
	DDX_Radio(pDX, IDC_DGN_RAI_RDO, m_nLiveType);
	DDX_Check(pDX, IDC_DGN_STATIC_VHL_CHK, m_bStatic);
	DDX_Control(pDX, IDC_DGN_STATIC_VHL_CMB, m_cmbStaticVhl);
	DDX_Control(pDX, IDC_DGN_STATIC_VHL_EDT, m_edtStaticVhl);
	DDX_Check(pDX, IDC_DGN_DYNAMIC_VHL_CHK, m_bDynamic);
	DDX_Control(pDX, IDC_DGN_DYNAMIC_VHL_CMB, m_cmbDynamicVhl);
	DDX_Control(pDX, IDC_DGN_DYNAMIC_VHL_EDT, m_edtDynamicVhl);

	DDX_Control(pDX, IDC_CMD_LCBD_GRID1, *m_pLoadCombList);
	DDX_Control(pDX, IDC_CMD_LCBD_GRID2, *m_pContentList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssessmentLoadCombSTLNRDlg, CDgnDlgBase)
	//{{AFX_MSG_MAP(CAssessmentLoadCombSTLNRDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnDlgClose)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_UPDATE_MODELESS_DLG, OnUpdateDlg)
	ON_BN_CLICKED(IDC_BTN_COPY_TO_LCOM, OnCopyToLCom)
	ON_BN_CLICKED(IDC_BTN_COPY_ASSES_LCOM, OnCopy)

	ON_BN_CLICKED(IDC_DGN_STATIC_VHL_CHK, OnStaticVhlChk)
	ON_CBN_SELCHANGE(IDC_DGN_STATIC_VHL_CMB, OnStaticVhlCmb)
	ON_EN_CHANGE(IDC_DGN_STATIC_VHL_EDT, OnStaticVhlEdt)
	ON_BN_CLICKED(IDC_DGN_DYNAMIC_VHL_CHK, OnDynamicVhlChk)
	ON_CBN_SELCHANGE(IDC_DGN_DYNAMIC_VHL_CMB, OnDynamicVhlCmb)
	ON_EN_CHANGE(IDC_DGN_DYNAMIC_VHL_EDT, OnDynamicVhlEdt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombSTLNRDlg message handlers
BOOL CAssessmentLoadCombSTLNRDlg::OnInitDialog()
{
	CDgnDlgBase::OnInitDialog();

	// TODO: Add extra initialization here	
	SetControl();
	
	m_pLoadCombList->OnChangeCurrentRecord(1);

	//#if defined(_TOWER) || defined(_CIVIL)  // modified by KYM
#if defined(_TOWER)   // modified by KYM
	m_wndAutoGenBtn.ShowWindow(SW_HIDE);
#endif
	//m_wndAutoGenBtn.EnableWindow(m_nLcomType!=D_LCOMTYPE_GENERAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAssessmentLoadCombSTLNRDlg::SetControl()
{
	m_aIDResize.RemoveAll();
	m_aIDResize.Add(IDC_CMD_FRAME1);
	m_aIDResize.Add(IDC_CMD_LCBD_GRID1);

	m_aIDVectMoveHolResize.RemoveAll();
	m_aIDVectMoveHolResize.Add(IDC_CMD_FRAME2);
	m_aIDVectMoveHolResize.Add(IDC_CMD_LCBD_GRID2);

	m_aIDHolMoveVertResize.RemoveAll();
	m_aIDHolMoveVertResize.Add(IDC_BTN_COPY_ASSES_LCOM);
	m_aIDHolMoveVertResize.Add(IDC_BTN_COPY_TO_LCOM);

	GetWindowRect(m_rectDlg);

	m_aRectResize.SetSize(m_aIDResize.GetSize());
	for (int i = 0; i < m_aIDResize.GetSize(); i++)
	{
		GetDlgItem(m_aIDResize[i])->GetWindowRect(m_aRectResize[i]);
	}

	m_aRectVertMoveHorResize.SetSize(m_aIDVectMoveHolResize.GetSize());
	for (int i = 0; i < m_aIDVectMoveHolResize.GetSize(); i++)
	{
		GetDlgItem(m_aIDVectMoveHolResize[i])->GetWindowRect(m_aRectVertMoveHorResize[i]);
	}

	m_aRectHorMoveVertResize.SetSize(m_aIDHolMoveVertResize.GetSize());
	for (int i = 0; i < m_aIDHolMoveVertResize.GetSize(); i++)
	{
		GetDlgItem(m_aIDHolMoveVertResize[i])->GetWindowRect(m_aRectHorMoveVertResize[i]);
	}

	GetDlgItem(IDC_CMD_BTN_CLOSE)->GetWindowRect(m_rectClose);

	
	T_UNIT_SYSTEM UnitSystem;
	//m_pDoc->m_pUnitCtrl->GetUnitSystemLcom(UnitSystem);
	m_pLoadCombList->Initialize(m_pDoc, UnitSystem);
	m_pContentList->Initialize(m_pDoc);

	m_pLoadCombList->m_pSubGrid = m_pContentList;
	m_pContentList->m_pMainGrid = m_pLoadCombList;

	m_pLoadCombList->CheckCurrentRecordChanged();
	SetStaticVhlCmb(m_cmbStaticVhl);
	SetDynamicVhlCmb(m_cmbDynamicVhl);
}

void CAssessmentLoadCombSTLNRDlg::OnDlgClose()
{
	// TODO: Add your control notification handler code here  
	m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);
	EndDialog(IDOK);
}

void CAssessmentLoadCombSTLNRDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_POST_STAGE_CHANGED:
		OnStageChanged();
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CAssessmentLoadCombSTLNRDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_LCOM_K cKey;
	m_pLoadCombList->GetCurrentCell(ncRow, ncCol);
	m_pLoadCombList->GetColumnNoFor(m_pLoadCombList->GetKeyColID(), nkCol);
	m_pLoadCombList->GetKeyForRow(ncRow, nkCol);
	cKey = _ttol(m_pLoadCombList->GetKeyForRow(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	// 하중 조건에 변경이 있는지 먼저 조사해서 처리하고 나서 
	// 하중 조합에 대해서 처리한다.
	UpdateStld();

	T_ALCS_K    Key;
	T_ALCS_D    Data;

	BOOL bOldLock = m_pLoadCombList->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		if (nCmd != UR_ALCS_ADD && nCmd != UR_ALCS_DEL) continue;

		switch (nCmd)
		{
		case(UR_ALCS_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetAlcs(nKey, Key, Data);
			((CAssessmentLoadCombMainGridSTLNR*)m_pLoadCombList)->InsertRecordByKey(Key, Data);
			break;
		case(UR_ALCS_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetAlcs(nKey, Key, Data);
			((CAssessmentLoadCombMainGridSTLNR*)m_pLoadCombList)->DeleteRecordByKey(Key, Data);
			break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_pLoadCombList->m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_pLoadCombList->m_nEditMode = CTBBrowserWnd::noMode;
		if (m_pLoadCombList->GetRowCount() < ncRow) ncRow = m_pLoadCombList->GetRowCount();
		BOOL bSet = m_pLoadCombList->SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = m_pLoadCombList->SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = m_pLoadCombList->SetLeftValidCell(ncRow, ncCol);
		m_pLoadCombList->m_nEditMode = nOldMode;
	}

	if (!m_pLoadCombList->CheckCurrentRecordChanged())
	{
		// 현재 레코드 위치 변경없이 내용이 변경된 경우
		if (bCurrentRecordChanged) m_pLoadCombList->OnChangeCurrentRecord(ncRow);
	}

	m_pLoadCombList->AdjustAppendRowNo();
	m_pLoadCombList->LockUpdate(bOldLock);
	if (!bOldLock) m_pLoadCombList->Redraw();
}

void CAssessmentLoadCombSTLNRDlg::OnStageChanged()
{
	BOOL bOldLock1 = m_pLoadCombList->LockUpdate(TRUE);
	BOOL bOldLock2 = m_pContentList->LockUpdate(TRUE);

	// 포커스 가진 윈도우 핸들 저장

	HWND hwnd = ::GetFocus();

	// 수정중이면 일단 취소한다.
	if (m_pLoadCombList->m_nEditMode != CTBBrowserWnd::noMode)
	{
		// Active 상태이면 deactive시켜야 CancelEdit내에서 RemoveRow할 때 
		// Validation Check하지 않는다.
		ROWCOL nRow, nCol;
		m_pLoadCombList->GetCurrentCell(nRow, nCol);
		CGXControl* pControl = m_pLoadCombList->GetControl(nRow, nCol);
		if (pControl->IsActive()) pControl->SetActive(FALSE);
		m_pLoadCombList->CancelEdit();
	}
	if (m_pContentList->m_nEditMode != CTBBrowserWnd::noMode)
	{
		// Active 상태이면 deactive시켜야 CancelEdit내에서 RemoveRow할 때 
		// Validation Check하지 않는다.
		ROWCOL nRow, nCol;
		m_pContentList->GetCurrentCell(nRow, nCol);
		CGXControl* pControl = m_pContentList->GetControl(nRow, nCol);
		if (pControl->IsActive()) pControl->SetActive(FALSE);
		m_pContentList->CancelEdit();
	}
	m_pContentList->SetLoadCaseColumnStyle(1);

	m_pLoadCombList->MakeItemEx();
	ROWCOL ncRow, ncCol;
	if (m_pLoadCombList->GetCurrentCell(ncRow, ncCol))
		m_pLoadCombList->OnChangeCurrentRecord(ncRow);
	else
		m_pLoadCombList->OnChangeCurrentRecord(1);

	// 이전 포커스 가진 윈도우 복구
	if (::IsWindow(hwnd)) ::SetFocus(hwnd);
	m_pLoadCombList->LockUpdate(bOldLock1);
	if (!bOldLock1) m_pLoadCombList->Redraw();
	m_pContentList->LockUpdate(bOldLock2);
	if (!bOldLock2) m_pContentList->Redraw();
}

void CAssessmentLoadCombSTLNRDlg::UpdateStld()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	BOOL bStldChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_STLD_ADD):
		case(UR_STLD_DEL):
		case(UR_STLD_MFS):
		case(UR_STLD_MFD):
		case(UR_SPLC_ADD):
		case(UR_SPLC_DEL):
		case(UR_SPLC_MFS):
		case(UR_SPLC_MFD):
		case(UR_THIS_ADD):
		case(UR_THIS_DEL):
		case(UR_THIS_MFS):
		case(UR_THIS_MFD):
		case(UR_MVLD_ADD):
		case(UR_MVLD_DEL):
		case(UR_MVLD_MFS):
		case(UR_MVLD_MFD):
		case(UR_MVLDjp_ADD):
		case(UR_MVLDjp_DEL):
		case(UR_MVLDjp_MFS):
		case(UR_MVLDjp_MFD):
		case(UR_MVLDch_ADD):
		case(UR_MVLDch_DEL):
		case(UR_MVLDch_MFS):
		case(UR_MVLDch_MFD):
		case(UR_MVLDid_ADD):
		case(UR_MVLDid_DEL):
		case(UR_MVLDid_MFS):
		case(UR_MVLDid_MFD):
		case(UR_MVLDbs_ADD):
		case(UR_MVLDbs_DEL):
		case(UR_MVLDbs_MFS):
		case(UR_MVLDbs_MFD):
		case(UR_SMLC_ADD):
		case(UR_SMLC_DEL):
		case(UR_SMLC_MFS):
		case(UR_SMLC_MFD):
		case(UR_STAG_ADD):  // sgld에 영향
		case(UR_STAG_DEL):  // sgld에 영향 
		case(UR_GILC_ADD):
		case(UR_GILC_DEL):
		case(UR_GILC_MFS):
		case(UR_GILC_MFD):
		case(UR_CRAN_ADD):
		case(UR_CRAN_DEL):
		case(UR_CRAN_MFS):
		case(UR_CRAN_MFD):
			bStldChanged = TRUE;
			break;
		case(UR_LCOM_ADD):
		case(UR_LCOM_DEL):
		case(UR_LCOM_MFS):
		case(UR_LCOM_MFD):
		case(UR_LCST_ADD):
		case(UR_LCST_DEL):
		case(UR_LCST_MFS):
		case(UR_LCST_MFD):
		case(UR_LCCO_ADD):
		case(UR_LCCO_DEL):
		case(UR_LCCO_MFS):
		case(UR_LCCO_MFD):
		case(UR_LCFT_ADD):
		case(UR_LCFT_DEL):
		case(UR_LCFT_MFS):
		case(UR_LCFT_MFD):
		case(UR_LCSC_ADD):
		case(UR_LCSC_DEL):
		case(UR_LCSC_MFS):
		case(UR_LCSC_MFD):
		case(UR_LCAL_ADD):
		case(UR_LCAL_DEL):
		case(UR_LCAL_MFS):
		case(UR_LCAL_MFD):
		case(UR_LCSCM_ADD):
		case(UR_LCSCM_DEL):
		case(UR_LCSCM_MFS):
		case(UR_LCSCM_MFD):
		case(UR_LCCF_ADD):
		case(UR_LCCF_DEL):
		case(UR_LCCF_MFS):
		case(UR_LCCF_MFD):
			bStldChanged = TRUE;
			break;
		case(UR_RLCS_ADD):
		case(UR_RLCS_DEL):
			bStldChanged = TRUE;
			break;
		}
	}

	if (bStldChanged)
	{
		m_pContentList->OnChangeStld();
	}
}

void CAssessmentLoadCombSTLNRDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	BOOL bOldLock = m_pLoadCombList->LockUpdate(FALSE);
	m_pLoadCombList->Redraw();
	m_pLoadCombList->LockUpdate(bOldLock);

	bOldLock = m_pContentList->LockUpdate(FALSE);
	m_pContentList->Redraw();
	m_pContentList->LockUpdate(bOldLock);

	// Do not call CDialog::OnPaint() for painting messages
}

void CAssessmentLoadCombSTLNRDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CDgnDlgBase::OnWindowPosChanged(lpwndpos);

	// TODO: Add your message handler code here
	if ((lpwndpos->flags & SWP_NOSIZE) == 0)  // Size가 변경된 경우
	{
		CRect r, rDlg;
		GetWindowRect(rDlg);

		// 크기변화 없이 가로 세로 움직임
		int dx, dy;
		CArray<UINT, UINT> aBtn;
		aBtn.Add(IDC_CMD_BTN_CLOSE);
		GetDlgItem(IDC_CMD_BTN_CLOSE)->GetWindowRect(r);
		dx = (rDlg.right - r.right) - (m_rectDlg.right - m_rectClose.right);
		dy = (rDlg.bottom - r.bottom) - (m_rectDlg.bottom - m_rectClose.bottom);
		aBtn.Add(IDC_CMD_LCBD_GRID3);
		aBtn.Add(IDC_CMD_FRAME3);

		aBtn.Add(IDC_DGN_LIVE_LOAD_GRP);
		aBtn.Add(IDC_DGN_RAI_RDO);
		aBtn.Add(IDC_DGN_WAGON_RDO);

		aBtn.Add(IDC_DGN_STATIC_VHL_CHK);
		aBtn.Add(IDC_DGN_STATIC_VHL_CMB);
		aBtn.Add(IDC_DGN_STATIC_VHL_EDT);
		aBtn.Add(IDC_DGN_DYNAMIC_VHL_CHK);
		aBtn.Add(IDC_DGN_DYNAMIC_VHL_CMB);
		aBtn.Add(IDC_DGN_DYNAMIC_VHL_EDT);

		CDlgUtil::CtrlMoveDistXY(this, aBtn, dx, dy, FALSE);

		// 가로 크기가 변하고 세로로 움직임
		for (int i = 0; i < m_aIDHolMoveVertResize.GetSize(); i++)
		{
			r.left = rDlg.left + (m_aRectHorMoveVertResize[i].left - m_rectDlg.left);
			r.top = rDlg.bottom - (m_rectDlg.bottom - m_aRectHorMoveVertResize[i].top);
			r.right = rDlg.right - (m_rectDlg.right - m_aRectHorMoveVertResize[i].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_aRectHorMoveVertResize[i].bottom);
			ScreenToClient(r);
			GetDlgItem(m_aIDHolMoveVertResize[i])->MoveWindow(r, FALSE);
		}

		// 가로 세로 크기가 변하고 움직이지 않음
		for (int i = 0; i < m_aIDResize.GetSize(); i++)
		{
			r.left = rDlg.left + (m_aRectResize[i].left - m_rectDlg.left);
			r.top = rDlg.top + (m_aRectResize[i].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_aRectResize[i].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_aRectResize[i].bottom);
			ScreenToClient(r);
			GetDlgItem(m_aIDResize[i])->MoveWindow(r, FALSE);
		}

		// 세로 크기가 변하고 가로로 움직임
		for (int i = 0; i < m_aIDVectMoveHolResize.GetSize(); i++)
		{
			r.left = rDlg.right - (m_rectDlg.right - m_aRectVertMoveHorResize[i].left);
			r.top = rDlg.top + (m_aRectVertMoveHorResize[i].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_aRectVertMoveHorResize[i].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_aRectVertMoveHorResize[i].bottom);
			ScreenToClient(r);
			GetDlgItem(m_aIDVectMoveHolResize[i])->MoveWindow(r, FALSE);
		}

		Invalidate();
	}
}

void CAssessmentLoadCombSTLNRDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);
	CDgnDlgBase::OnClose();
}

void CAssessmentLoadCombSTLNRDlg::PostNcDestroy()
{
	CChildDialogMove::PostNcDestroy();
}

void CAssessmentLoadCombSTLNRDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

	CChildDialogMove::OnSysCommand(nID, lParam);
}


BOOL CAssessmentLoadCombSTLNRDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// When an item is being edited make sure the edit control
		// receives certain important key strokes

		if (pMsg->wParam == VK_ESCAPE)
		{
			m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);
			EndDialog(IDOK);
			return TRUE;
		}
	}

	return CDgnDlgBase::PreTranslateMessage(pMsg);
}

void CAssessmentLoadCombSTLNRDlg::SetMainGridModeEdit()
{
	if (m_bSetGridEditMode && m_pLoadCombList)
	{
		m_pLoadCombList->SetModeEdit();
	}
}

void CAssessmentLoadCombSTLNRDlg::OnStaticVhlChk()
{
	EnableDisableControls();
}

void CAssessmentLoadCombSTLNRDlg::OnStaticVhlCmb()
{
	SetMainGridModeEdit();
}

void CAssessmentLoadCombSTLNRDlg::OnStaticVhlEdt()
{
	SetMainGridModeEdit();
}

void CAssessmentLoadCombSTLNRDlg::OnDynamicVhlChk()
{
	EnableDisableControls();
}

void CAssessmentLoadCombSTLNRDlg::OnDynamicVhlCmb()
{
	SetMainGridModeEdit();
}

void CAssessmentLoadCombSTLNRDlg::OnDynamicVhlEdt()
{
	SetMainGridModeEdit();
}

void CAssessmentLoadCombSTLNRDlg::OnCopy()
{
	ROWCOL ncRow, ncCol;

	if (!m_pLoadCombList->GetCurrentCell(ncRow, ncCol)) return;

	if (m_pLoadCombList->IsAppendRow(ncRow))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_specified_load_combina));
		return;
	}
	if (m_pLoadCombList->IsEdit())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_copy_while_editing_));
		return;
	}

	CString csKey;
	csKey = m_pLoadCombList->GetKeyString(ncRow);

	T_ALCS_K Key = _ttol(csKey);
	if (!m_pDoc->m_pAttrCtrl2->ExistAlcs(Key)) return;

	CArray<T_ALCS_K, T_ALCS_K> aAlcsK;
	m_pLoadCombList->GetCurSelKey(aAlcsK);

	CRatingLoadCombUtil::CopyPSCAssessmentLCom(m_pDoc, aAlcsK);
}


void CAssessmentLoadCombSTLNRDlg::OnCopyToLCom()
{
	if (!m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS))
	{
		return;
	}

	CRatingLoadCombUtil::CopySteelNRToLCom(m_pDoc);
}

void CAssessmentLoadCombSTLNRDlg::SetStaticVhlCmb(MComboBox& cobxMv)
{
	BOOL bOld = m_bSetGridEditMode;
	m_bSetGridEditMode = FALSE;

	cobxMv.ResetContent();

	POSITION pos;

	T_MVCD_D DataMvcd;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_CHINA:
	case D_MOVE_CODE_JAPAN:
	case D_MOVE_CODE_INDIA:
	case D_MOVE_CODE_FRANCE:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		break;
	case D_MOVE_CODE_BS:
	{
		T_MVLDbs_K MvldKey;
		T_MVLDbs_D MvldData;
		pos = m_pDoc->m_pAttrCtrl->GetStartMvldbs();
		while (pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextMvldbs(pos, MvldKey, MvldData);

			if (MvldData.nLoadModel == 4)
			{
				CDlgUtil::CobxAddItem(cobxMv, MvldData.LoadCaseName + _T("(MV)"), MvldKey);
			}
		}
	}
	break;
	case D_MOVE_CODE_TRANS:
		break;
	default:
		ASSERT(0);
		break;
	}

	cobxMv.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(cobxMv);

	m_bSetGridEditMode = bOld;
}

void CAssessmentLoadCombSTLNRDlg::SetDynamicVhlCmb(MComboBox& cobxMv)
{
	BOOL bOld = m_bSetGridEditMode;
	m_bSetGridEditMode = FALSE;

	cobxMv.ResetContent();

	POSITION pos;

	T_MVCD_D DataMvcd;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_CHINA:
	case D_MOVE_CODE_JAPAN:
	case D_MOVE_CODE_INDIA:
	case D_MOVE_CODE_FRANCE:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		break;
	case D_MOVE_CODE_BS:
	{
		T_MVLDbs_K MvldKey;
		T_MVLDbs_D MvldData;
		pos = m_pDoc->m_pAttrCtrl->GetStartMvldbs();
		while (pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextMvldbs(pos, MvldKey, MvldData);

			if (MvldData.nLoadModel == 4)
			{
				CDlgUtil::CobxAddItem(cobxMv, MvldData.LoadCaseName + _T("(MV)"), MvldKey);
			}			
		}
	}
	break;
	case D_MOVE_CODE_TRANS:
		break;
	default:
		ASSERT(0);
		break;
	}

	cobxMv.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(cobxMv);

	m_bSetGridEditMode = bOld;
}

BOOL CAssessmentLoadCombSTLNRDlg::Dlg2Data(T_ALCS_D &AlcsD)
{
	UpdateData(TRUE);

	CString strTmp;

	AlcsD.nLiveType = m_nLiveType;

	AlcsD.bCombinedMV = m_bStatic;
	AlcsD.CombinedMVCase.AnalType = D_LCOM_MOVING;
	AlcsD.CombinedMVCase.LoadCaseKey = CDlgUtil::CobxGetCurSelItemData(m_cmbStaticVhl, m_cmbStaticVhl.GetCurSel());
	m_edtStaticVhl.GetWindowText(strTmp);
	AlcsD.CombinedMVCase.Factor = _tstof(strTmp);

	AlcsD.bSpecialMV = m_bDynamic;
	AlcsD.SpecialMVCase.AnalType = D_LCOM_MOVING;
	AlcsD.SpecialMVCase.LoadCaseKey = CDlgUtil::CobxGetCurSelItemData(m_cmbDynamicVhl, m_cmbDynamicVhl.GetCurSel());
	m_edtDynamicVhl.GetWindowText(strTmp);
	AlcsD.SpecialMVCase.Factor = _tstof(strTmp);

	AlcsD.nRatingType = 0;

	return TRUE;
}

BOOL CAssessmentLoadCombSTLNRDlg::GetData(T_ALCS_D &AlcsD)
{
	return Dlg2Data(AlcsD);
}

void CAssessmentLoadCombSTLNRDlg::Data2Dlg(const T_ALCS_D &AlcsD)
{
	BOOL bOld = m_bSetGridEditMode;
	m_bSetGridEditMode = FALSE;

	CString strTmp;

	m_nLiveType = AlcsD.nLiveType;

	m_bStatic = AlcsD.bCombinedMV;
	CDlgUtil::CobxSetCurSelItemData(m_cmbStaticVhl, AlcsD.CombinedMVCase.LoadCaseKey);
	strTmp.Format(_T("%g"), AlcsD.CombinedMVCase.Factor);
	m_edtStaticVhl.SetWindowText(strTmp);

	m_bDynamic = AlcsD.bSpecialMV;
	CDlgUtil::CobxSetCurSelItemData(m_cmbDynamicVhl, AlcsD.SpecialMVCase.LoadCaseKey);
	strTmp.Format(_T("%g"), AlcsD.SpecialMVCase.Factor);
	m_edtDynamicVhl.SetWindowText(strTmp);

	UpdateData(FALSE);

	m_bSetGridEditMode = bOld;

	EnableDisableControls();
}

void CAssessmentLoadCombSTLNRDlg::SetData(T_ALCS_K AlcsK)
{
	m_bSetGridEditMode = FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_ALCS_D AlcsD;
	AlcsD.Initialize();
	if (AlcsK != NULL)
	{
		if (!pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD)) ASSERT(0);
	}

	Data2Dlg(AlcsD);
	m_bSetGridEditMode = TRUE;
}

void CAssessmentLoadCombSTLNRDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_STATIC_VHL_CMB)->EnableWindow(m_bStatic);
	GetDlgItem(IDC_DGN_STATIC_VHL_EDT)->EnableWindow(m_bStatic);

	GetDlgItem(IDC_DGN_DYNAMIC_VHL_CMB)->EnableWindow(m_bDynamic);
	GetDlgItem(IDC_DGN_DYNAMIC_VHL_EDT)->EnableWindow(m_bDynamic);
}



