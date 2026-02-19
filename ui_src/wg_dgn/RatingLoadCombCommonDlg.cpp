// RatingLoadCombCommonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingLoadCombCommonDlg.h"

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombCommonDlg dialog
CRatingLoadCombCommonDlg::CRatingLoadCombCommonDlg(int nIDD, CWnd* pParent)
	: CDgnDlgBase(nIDD, pParent)
{
	// *^^*  여기를 바꾸시오.

	//{{AFX_DATA_INIT(CRatingLoadCombCommonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
		
	m_pDoc = CDBDoc::GetDocPoint();

	m_pLoadCombList = nullptr;
	m_pContentList = nullptr;
	m_pContentListMV = nullptr;
#if defined(_CIVIL)
 // CString csErrMsg(_T(""));
 // m_bTB10002_1_05 = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLCBConcrete,
	//                                                           _LSX(TB 10002.1-05), csErrMsg);
#else
	m_bTB10002_1_05 = FALSE;
#endif
}

CRatingLoadCombCommonDlg::~CRatingLoadCombCommonDlg()
{

}

void CRatingLoadCombCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingLoadCombCommonDlg)

	if (m_pLoadCombList != nullptr)
		DDX_Control(pDX, IDC_CMD_LCBD_GRID1, *m_pLoadCombList);
	if (m_pContentList != nullptr)
		DDX_Control(pDX, IDC_CMD_LCBD_GRID2, *m_pContentList);
	if (m_pContentListMV != nullptr)
		DDX_Control(pDX, IDC_CMD_LCBD_GRID3, *m_pContentListMV);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRatingLoadCombCommonDlg, CDgnDlgBase)
	//{{AFX_MSG_MAP(CRatingLoadCombCommonDlg)

	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnDlgClose)
	ON_WM_PAINT()	
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_UPDATE_MODELESS_DLG, OnUpdateDlg)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombCommonDlg message handlers
BOOL CRatingLoadCombCommonDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();

	// TODO: Add extra initialization here
	
	SetControl();

//#if defined(_TOWER) || defined(_CIVIL)  // modified by KYM
#if defined(_TOWER)   // modified by KYM
	m_wndAutoGenBtn.ShowWindow(SW_HIDE);
#endif
	//m_wndAutoGenBtn.EnableWindow(m_nLcomType!=D_LCOMTYPE_GENERAL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRatingLoadCombCommonDlg::SetControl()
{
	m_aIDResize.RemoveAll();
	m_aIDResize.Add(IDC_CMD_FRAME1);
	m_aIDResize.Add(IDC_CMD_LCBD_GRID1);
	
	m_aIDVectMoveHolResize.RemoveAll();
	m_aIDVectMoveHolResize.Add(IDC_CMD_FRAME2);
	m_aIDVectMoveHolResize.Add(IDC_CMD_LCBD_GRID2);

	m_aIDHolMoveVertResize.RemoveAll();
	m_aIDHolMoveVertResize.Add(IDC_BTN_COPY_TO_LCOM);

	GetWindowRect(m_rectDlg);      
	
	for (int i = 0; i < m_aIDResize.GetSize(); i++)
	{
		GetDlgItem(m_aIDResize[i])->GetWindowRect(m_rectResize[i]);
	}
	
	for (int i = 0; i < m_aIDVectMoveHolResize.GetSize(); i++)
	{
		GetDlgItem(m_aIDVectMoveHolResize[i])->GetWindowRect(m_rectVertMoveHorResize[i]);
	}
	
	for(int i = 0; i < m_aIDHolMoveVertResize.GetSize(); i++)
	{
		GetDlgItem(m_aIDHolMoveVertResize[i])->GetWindowRect(m_rectHorMoveVertResize[i]);
	}
	
	GetDlgItem(IDC_CMD_BTN_CLOSE)->GetWindowRect(m_rectClose);			

	T_UNIT_SYSTEM UnitSystem;
	//m_pDoc->m_pUnitCtrl->GetUnitSystemLcom(UnitSystem);
	m_pLoadCombList->Initialize(m_pDoc, UnitSystem);
	m_pContentList->Initialize(m_pDoc);
	m_pContentListMV->Initialize(m_pDoc);
	
	m_pLoadCombList->m_pSubGrid = m_pContentList;
	m_pLoadCombList->m_pSubGridMV = m_pContentListMV;
	m_pContentList->m_pMainGrid = m_pLoadCombList;
	m_pContentListMV->m_pMainGrid = m_pLoadCombList;
	
	m_pLoadCombList->CheckCurrentRecordChanged();	
}

void CRatingLoadCombCommonDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here  
	m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);
	EndDialog(IDOK);
}

void CRatingLoadCombCommonDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
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

void CRatingLoadCombCommonDlg::OnStageChanged()
{
	BOOL bOldLock1 = m_pLoadCombList->LockUpdate(TRUE);
	BOOL bOldLock2 = m_pContentList->LockUpdate(TRUE);
	BOOL bOldLock3 = m_pContentListMV->LockUpdate(TRUE);

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
	if (m_pContentListMV->m_nEditMode != CTBBrowserWnd::noMode)
	{
		// Active 상태이면 deactive시켜야 CancelEdit내에서 RemoveRow할 때 
		// Validation Check하지 않는다.
		ROWCOL nRow, nCol;
		m_pContentListMV->GetCurrentCell(nRow, nCol);
		CGXControl* pControl = m_pContentListMV->GetControl(nRow, nCol);
		if (pControl->IsActive()) pControl->SetActive(FALSE);
		m_pContentListMV->CancelEdit();  
	}


	m_pContentList->SetLoadCaseColumnStyle(1);
	m_pContentListMV->SetLoadCaseColumnStyle(1);

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
	m_pContentListMV->LockUpdate(bOldLock3);
	if (!bOldLock3) m_pContentListMV->Redraw();
}

void CRatingLoadCombCommonDlg::UpdateStld()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bStldChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
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
		m_pContentListMV->OnChangeStld();				
	}
}


void CRatingLoadCombCommonDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	BOOL bOldLock = m_pLoadCombList->LockUpdate(FALSE);
	m_pLoadCombList->Redraw();
	m_pLoadCombList->LockUpdate(bOldLock);

	bOldLock = m_pContentList->LockUpdate(FALSE);
	m_pContentList->Redraw();
	m_pContentList->LockUpdate(bOldLock);
 
	bOldLock = m_pContentListMV->LockUpdate(FALSE);
	m_pContentListMV->Redraw();
	m_pContentListMV->LockUpdate(bOldLock);

	// Do not call CDialog::OnPaint() for painting messages
}

CString CRatingLoadCombCommonDlg::GetCombActiveStr(int nLcomType, int nActive)
{
	CString strActive;
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL)  
	{
		if(nActive == 0)       strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1)  strActive = _LS(IDS_CMD_LCOM_ACTIVE);
	}  
	else if(nLcomType == D_LCOMTYPE_CONCRETE && CProduct::IsMovingType(D_PRODUCT_MOVING_CH) && m_bTB10002_1_05)
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_STRENGTH);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_SERVICE);
		else if(nActive == 3) strActive = _LS(IDS_CMD_LCOM_TB10002_1_MAIN);
		else if(nActive == 4) strActive = _LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION);
		else if(nActive == 5) strActive = _LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL);
	}
	else // (nLcomType == D_LCOMTYPE_CONCRETE || nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_SRC)
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_STRENGTH);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_SERVICE);     
	}  
#elif defined(_MGEN)
	if(nLcomType == D_LCOMTYPE_CONCRETE)
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_STRENGTH);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_SERVICE);     
		else if(nActive == 3) strActive = _LS(IDS_CMD_LCOM_SPECIAL);     
		else if(nActive == 4) strActive = _LS(IDS_CMD_LCOM_VERTICAL);     
	}
	else if(nLcomType == D_LCOMTYPE_STEEL)  // add by Seungjun (`06.06.16) No.2079 KSSC-ASD03.
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_STRENGTH);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_SERVICE);
		else if(nActive == 3) strActive = _LS(IDS_CMD_LCOM_SPECIAL);     
		else if(nActive == 4) strActive = _LS(IDS_CMD_LCOM_VERTICAL);
	}
	else
	{
		if(nActive == 0)       strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1)  strActive = _LS(IDS_CMD_LCOM_ACTIVE);
	}
#else
		if(nActive == 0)       strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1)  strActive = _LS(IDS_CMD_LCOM_ACTIVE);
#endif
	return strActive;
}

CString CRatingLoadCombCommonDlg::GetCombTypeStr(int nCombType)
{
	CString aCombTypeStr[] = {D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_ABS, D_LCOM_CT_SRSS};
	if (nCombType >= 0 && nCombType <= 3) return aCombTypeStr[nCombType];
	else return _LS(IDS_WG_CMD__ADDD__Error);
}

void CRatingLoadCombCommonDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
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
		CDlgUtil::CtrlMoveDistXY(this, aBtn, dx, dy, FALSE);

		// 가로 크기가 변하고 세로로 움직임
		for (int i = 0; i < m_aIDHolMoveVertResize.GetSize(); i++)
		{
			r.left = rDlg.left + (m_rectHorMoveVertResize[i].left - m_rectDlg.left);
			r.top = rDlg.bottom - (m_rectDlg.bottom - m_rectHorMoveVertResize[i].top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectHorMoveVertResize[i].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectHorMoveVertResize[i].bottom);
			ScreenToClient(r);
			GetDlgItem(m_aIDHolMoveVertResize[i])->MoveWindow(r, FALSE);
		}

		// 가로 세로 크기가 변하고 움직이지 않음
		for (int i = 0; i < m_aIDResize.GetSize(); i++)
		{
			r.left = rDlg.left + (m_rectResize[i].left - m_rectDlg.left);
			r.top = rDlg.top + (m_rectResize[i].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectResize[i].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectResize[i].bottom);
			ScreenToClient(r);
			GetDlgItem(m_aIDResize[i])->MoveWindow(r, FALSE);
		}
		
		// 세로 크기가 변하고 가로로 움직임
		for (int i = 0; i < m_aIDVectMoveHolResize.GetSize(); i++)
		{
			r.left = rDlg.right - (m_rectDlg.right - m_rectVertMoveHorResize[i].left);
			r.top = rDlg.top + (m_rectVertMoveHorResize[i].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectVertMoveHorResize[i].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectVertMoveHorResize[i].bottom);
			ScreenToClient(r);
			GetDlgItem(m_aIDVectMoveHolResize[i])->MoveWindow(r, FALSE);
		}

		Invalidate();
	}
}

void CRatingLoadCombCommonDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);
	CDgnDlgBase::OnClose();
}

void CRatingLoadCombCommonDlg::PostNcDestroy() 
{	
	CChildDialogMove::PostNcDestroy();
}

void CRatingLoadCombCommonDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	
	CChildDialogMove::OnSysCommand(nID, lParam);
}


BOOL CRatingLoadCombCommonDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		// When an item is being edited make sure the edit control
		// receives certain important key strokes
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);
			EndDialog(IDOK);
			return TRUE;
		}
	}
	
	return CDgnDlgBase::PreTranslateMessage(pMsg);
}
