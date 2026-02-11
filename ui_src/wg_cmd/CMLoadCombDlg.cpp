#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombDlg.h"

#include <locale>

#include "..\mit_lib\UrlReader.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\ServiceProvider.h"
#include "..\wg_base\TextFileOut.h"

#include "../wg_common/MGridExportToExcel.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\IUsageCounter.h"

#include "..\wg_cmd2\CMD2_ServiceDef.h"

#include "CmdAutoLoadCombDlg.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadComb_SE_CVL_Dlg.h"
#include "CmdAutoLoadComb_SE_Dlg.h"
#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LOADCOMB_EXCEL
{
	constexpr int startRow = 3; // data start row
	constexpr int headerRow = startRow-1; // data start row
	constexpr int loadCombNameCol = 2;
	constexpr int loadCombActiveCol = 3;
	constexpr int loadCombTypeCol = 4;
	constexpr int elasticStageCol = 5;
	constexpr int descriptionCol = 6;
	constexpr int loadCaseStartCol = 7;
	constexpr int colNumOfLoadCaseInfo = 3;

	constexpr int designTypeCol = 1;

	constexpr int loadCaseColWidth = 15;
	constexpr int factorColWidth = 5;
	constexpr int typeColWidth = 5;

	constexpr wchar_t sheetName[] = L"LISTS";
	constexpr wchar_t factor[] = L"Factor";
	constexpr wchar_t type[] = L"Type";

	class MString
	{
	private:
		CString m_str;

	public:
		MString(const CString& str) : m_str(str) { m_str.Trim(); }

		bool operator==(const CString& other) const {
			return m_str.CompareNoCase(const_cast<CString&>(other).Trim()) == 0;
		}

		CString& Raw() { return m_str; }

		MString& operator=(const CString& str) { m_str = str; return *this; }
	};

}

CCMLoadCombDlg::CCMLoadCombDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLoadCombDlg::IDD, pParent)
{
	// *^^*  여기를 바꾸시오.
	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_bSpreadSheetStyle = FALSE;
	m_nSeismicType = LCOM_GENERAL;

	//{{AFX_DATA_INIT(CCMLoadCombDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlNormalStyle.Add(IDC_CMD_FRAME1);
	m_aCtrlNormalStyle.Add(IDC_CMD_LCBD_GRID1);
	m_aCtrlNormalStyle.Add(IDC_CMD_FRAME2);
	m_aCtrlNormalStyle.Add(IDC_CMD_LCBD_GRID2);

	m_aCtrlSpreadStyle.Add(IDC_CMD_FRAME3);
	m_aCtrlSpreadStyle.Add(IDC_CMD_LCBD_GRID3);

	m_pLoadCombList = std::make_shared<CCMLoadCombMainGrid>();
	m_pContentList = std::make_shared<CCMLoadCombSubGrid>();
	m_pContentList->SetSeismicType(m_nSeismicType);
	m_pCombList = std::make_shared<CCMLoadCombGrid>();
	m_pCombList->m_pParentDlg = this;

	m_bPosSet = FALSE;

#if defined(_CIVIL)
	CString csErrMsg(_T(""));
	m_bTB10002_1_05 = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLCBConcrete,
																														 _LSX(TB 10002.1-05), csErrMsg);
#else
	m_bTB10002_1_05 = FALSE;
#endif
	
}

CCMLoadCombDlg::~CCMLoadCombDlg()
{
}

void CCMLoadCombDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCombDlg)
	DDX_Control(pDX, IDC_CMD_COPY_DGN_COMB, m_cmbLcomType);
	DDX_Control(pDX, IDC_CMD_BTN_AUTOGEN, m_wndAutoGenBtn);
	DDX_Control(pDX, IDC_CMD_TAB_CTRL, m_wndTab);
	DDX_Control(pDX, IDC_CMD_LCBD_GRID1, *m_pLoadCombList);
	DDX_Control(pDX, IDC_CMD_LCBD_GRID2, *m_pContentList);
	DDX_Control(pDX, IDC_CMD_LCBD_GRID3, *m_pCombList);
	//}}AFX_DATA_MAP
}

void CCMLoadCombDlg::AlignControl()
{
	CRect rRef1, rRef2;
	CRect rToMove;

	GetDlgItem(IDC_CMD_FRAME1)->GetWindowRect(rRef1);
	GetDlgItem(IDC_CMD_FRAME2)->GetWindowRect(rRef2);
	CWnd* pWnd1 = GetDlgItem(IDC_CMD_FRAME3);
	ScreenToClient(rRef1);
	ScreenToClient(rRef2);
	rToMove.top = rRef1.top;
	rToMove.left = rRef1.left;
	rToMove.bottom = rRef1.bottom;
	rToMove.right = rRef2.right;
	pWnd1->MoveWindow(rToMove);

	GetDlgItem(IDC_CMD_LCBD_GRID1)->GetWindowRect(rRef1);
	GetDlgItem(IDC_CMD_LCBD_GRID2)->GetWindowRect(rRef2);
	CWnd* pWnd2 = GetDlgItem(IDC_CMD_LCBD_GRID3);
	ScreenToClient(rRef1);
	ScreenToClient(rRef2);
	rToMove.top = rRef1.top;
	rToMove.left = rRef1.left;
	rToMove.bottom = rRef1.bottom;
	rToMove.right = rRef2.right;
	pWnd2->MoveWindow(rToMove);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_BTN_CLOSE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);

	m_bPosSet = FALSE;
	MoveWindow(r);

	arLcomType.RemoveAll();
	arLcomType.Add(IDC_CMD_BTN_COPY_DGN);
	arLcomType.Add(IDC_CMD_COPY_DGN_COMB);

	int nSelectedLcomType = m_aTabLcomType[m_wndTab.GetCurSel()];  
	CDlgUtil::CtrlShowHide(this, arLcomType, m_nSeismicType == LCOM_GENERAL && nSelectedLcomType == D_LCOMTYPE_GENERAL);
}

#define CDialog CCMDlgBase
BEGIN_MESSAGE_MAP(CCMLoadCombDlg, CDialog)
#undef CDialog
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnDlgClose)
	ON_WM_EXITSIZEMOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CMD_BTN_MAKE_SHEET, OnCmdBtnExport)
	ON_BN_CLICKED(IDC_CMD_BTN_AUTOGEN, OnCmdBtnAutogen)
	ON_BN_CLICKED(IDC_CMD_BTN_IMPORT, OnCmdBtnImport)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB_CTRL, OnSelchangeCmdTabCtrl)
	ON_BN_CLICKED(IDC_CMD_BTN_CHANGE_STYLE, OnCmdBtnChangeStyle)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY, OnCmdBtnCopy)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_CMD_BTN_COPY_DGN, OnCmdBtnCopyIntoDgn)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombDlg message handlers
BOOL CCMLoadCombDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	// TODO: Add extra initialization here  
	SetTabWndText();
	AlignControl();

	m_pLoadCombList->SetLcomType(m_nLcomType);
	m_pContentList->SetLcomType(m_nLcomType);
	m_pCombList->SetLcomType(m_nLcomType);

	T_UNIT_SYSTEM UnitSystem;
	//m_pDoc->m_pUnitCtrl->GetUnitSystemLcom(UnitSystem);
	m_pLoadCombList->Initialize(m_pDoc, UnitSystem);
	m_pContentList->Initialize(m_pDoc);
	m_pCombList->Initialize(m_pDoc);

	m_pLoadCombList->m_pSubGrid = m_pContentList.get();
	m_pContentList->m_pMainGrid = m_pLoadCombList.get();

	m_pLoadCombList->CheckCurrentRecordChanged();

	CDlgUtil::CtrlShowHide(this, m_aCtrlNormalStyle, !m_bSpreadSheetStyle);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSpreadStyle, m_bSpreadSheetStyle);

	InitLcomTypeCombo();

//#if defined(_TOWER) || defined(_CIVIL)  // modified by KYM
#if defined(_TOWER)   // modified by KYM
	m_wndAutoGenBtn.ShowWindow(SW_HIDE);
#endif
	//m_wndAutoGenBtn.EnableWindow(m_nLcomType!=D_LCOMTYPE_GENERAL);
	GetWindowRect(m_defaultRect);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadCombDlg::OnExitSizeMove()
{
	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CCMLoadCombDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = m_defaultRect.Width();
	lpMMI->ptMinTrackSize.y = m_defaultRect.Height();
}

CString CCMLoadCombDlg::GetDefaultFileName() const
{
	// FileName;
	TCHAR *lpBuffer;
	lpBuffer = new TCHAR[255];

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();

	CString strFileName = pDoc->GetPathName();
	if (strFileName == _T(""))
	{
		GetCurrentDirectory(255, lpBuffer);
		strFileName = lpBuffer;
		strFileName += _T("\\Untitled");
	}
	else
	{
		const auto lastDotPos = strFileName.ReverseFind('.');
		if (lastDotPos != std::string::npos)
			strFileName = strFileName.Left(lastDotPos);
	}
	delete[] lpBuffer;

	return strFileName;
}

void CCMLoadCombDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	if (m_bSpreadSheetStyle) m_pCombList->OnTerminate(D_TB_SAVE_ALWAYS);
	else m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);
	DestroyWindow();	
}

void CCMLoadCombDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		m_pCombList->UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		m_pCombList->UpdateBuffer();
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
		m_pCombList->OnStageChanged();
	default:
		break;
	}
}

void CCMLoadCombDlg::OnStageChanged()
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

void CCMLoadCombDlg::UpdateBuffer()
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

	T_LCOM_K    Key, KeyBak;
	T_LCOM_D    Data, DataBak;

	BOOL bOldLock = m_pLoadCombList->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		if (m_nLcomType == D_LCOMTYPE_GENERAL)
		{
			if (nCmd != UR_LCOM_ADD && nCmd != UR_LCOM_DEL &&
					nCmd != UR_LCOM_MFD && nCmd != UR_LCOM_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_STEEL)
		{
			if (nCmd != UR_LCST_ADD && nCmd != UR_LCST_DEL &&
					nCmd != UR_LCST_MFD && nCmd != UR_LCST_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if (nCmd != UR_LCCO_ADD && nCmd != UR_LCCO_DEL &&
					nCmd != UR_LCCO_MFD && nCmd != UR_LCCO_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_SRC)
		{
			if (nCmd != UR_LCSC_ADD && nCmd != UR_LCSC_DEL &&
					nCmd != UR_LCSC_MFD && nCmd != UR_LCSC_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_FDN)
		{
			if (nCmd != UR_LCFT_ADD && nCmd != UR_LCFT_DEL &&
					nCmd != UR_LCFT_MFD && nCmd != UR_LCFT_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_ALUMINUM)
		{
			if (nCmd != UR_LCAL_ADD && nCmd != UR_LCAL_DEL &&
					nCmd != UR_LCAL_MFD && nCmd != UR_LCAL_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_STLCOMP)
		{
			if (nCmd != UR_LCSCM_ADD && nCmd != UR_LCSCM_DEL &&
				  nCmd != UR_LCSCM_MFD && nCmd != UR_LCSCM_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_CFSTEEL)
		{
			if (nCmd != UR_LCCF_ADD && nCmd != UR_LCCF_DEL &&
					nCmd != UR_LCCF_MFD && nCmd != UR_LCCF_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_SEISMIC)
		{
			if (nCmd != UR_LCSM_ADD && nCmd != UR_LCSM_DEL &&
				nCmd != UR_LCSM_MFD && nCmd != UR_LCSM_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_LINEAR_EVAL)
		{
			if (nCmd != UR_LCLN_ADD && nCmd != UR_LCLN_DEL &&
				nCmd != UR_LCLN_MFD && nCmd != UR_LCLN_MFS) continue;
		}	

		switch(nCmd)
		{
		case(UR_LCOM_ADD):
		case(UR_LCST_ADD):
		case(UR_LCCO_ADD):
		case(UR_LCSC_ADD):
		case(UR_LCFT_ADD):
		case(UR_LCAL_ADD):
		case(UR_LCSCM_ADD):
		case(UR_LCCF_ADD):
		case(UR_LCSM_ADD):
		case(UR_LCLN_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				m_pLoadCombList->InsertRecordByKey(Key, Data);
				break;
		case(UR_LCOM_DEL):
		case(UR_LCST_DEL):
		case(UR_LCCO_DEL):
		case(UR_LCSC_DEL):
		case(UR_LCFT_DEL):
		case(UR_LCAL_DEL):
		case(UR_LCSCM_DEL):	
		case(UR_LCCF_DEL):
		case(UR_LCSM_DEL):
		case(UR_LCLN_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				m_pLoadCombList->DeleteRecordByKey(Key, Data);
				break;
		case(UR_LCOM_MFD):
		case(UR_LCST_MFD):
		case(UR_LCCO_MFD):
		case(UR_LCSC_MFD):
		case(UR_LCFT_MFD):
		case(UR_LCAL_MFD):
		case(UR_LCSCM_MFD):
		case(UR_LCCF_MFD):
		case(UR_LCSM_MFD):
		case(UR_LCLN_MFD):
				// 여기서는 저장만 하고 UR_LCOM_MFS에서 처리한다.
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_LCOM_MFS):
		case(UR_LCST_MFS):
		case(UR_LCCO_MFS):
		case(UR_LCSC_MFS):
		case(UR_LCFT_MFS):
		case(UR_LCAL_MFS):
		case(UR_LCSCM_MFS):
		case(UR_LCCF_MFS):
		case(UR_LCSM_MFS):
		case(UR_LCLN_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				m_pLoadCombList->DeleteRecordByKey(Key, Data);
				m_pLoadCombList->InsertRecordByKey(KeyBak, DataBak);
				if (Key == cKey) bCurrentRecordChanged = TRUE;
				// 플래그 수정
				bMFD = FALSE;
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

void CCMLoadCombDlg::UpdateStld()
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
		case(UR_LCSM_ADD):
		case(UR_LCSM_DEL):
		case(UR_LCSM_MFS):
		case(UR_LCSM_MFD):
		case(UR_LCLN_ADD):
		case(UR_LCLN_DEL):
		case(UR_LCLN_MFS):
		case(UR_LCLN_MFD):
			bStldChanged = TRUE;
			break;
		}
	}

	if (bStldChanged) 
		m_pContentList->OnChangeStld();
}


void CCMLoadCombDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	BOOL bOldLock = m_pLoadCombList->LockUpdate(FALSE);
	m_pLoadCombList->Redraw();
	m_pLoadCombList->LockUpdate(bOldLock);

	bOldLock = m_pContentList->LockUpdate(FALSE);
	m_pContentList->Redraw();
	m_pContentList->LockUpdate(bOldLock);

	bOldLock = m_pCombList->LockUpdate(FALSE);
	m_pContentList->RedrawAll();
	m_pContentList->LockUpdate(bOldLock);

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CCMLoadCombDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);

	return CCMDlgBase::DestroyWindow();
}

//////////////////////////////////////////////////////////////
void CCMLoadCombDlg::OnCmdBtnAutogen() 
{
	// TODO: Add your control notification handler code here

	int nAutoGenType = LCOM_TYPE::LCOM_GENERAL;
	switch (m_nSeismicType)
	{
	case LCOM_TYPE::LCOM_GENERAL:
		{
			if(m_nLcomType == D_LCOMTYPE_SEISMIC)
			{
				nAutoGenType = LCOM_TYPE::LCOM_SEISMIC;
			}
			else if(m_nLcomType == D_LCOMTYPE_LINEAR_EVAL)
			{
				nAutoGenType = LCOM_TYPE::LCOM_LINEAR;
			}
			else
			{
				nAutoGenType = LCOM_TYPE::LCOM_GENERAL;
			}
		}
		break;
	case LCOM_TYPE::LCOM_SEISMIC:
		{
			nAutoGenType = LCOM_TYPE::LCOM_SEISMIC;
		}
		break;
	case LCOM_TYPE::LCOM_LINEAR:
		{
			nAutoGenType = LCOM_TYPE::LCOM_LINEAR;
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	if(nAutoGenType == LCOM_TYPE::LCOM_GENERAL)
	{
#if defined(_CIVIL)
		CCmdAutoLoadCombCvlDlg dlg;
#else
		CCmdAutoLoadCombDlg dlg;
#endif
		dlg.SetLcomType(m_nLcomType);
		dlg.DoModal();
	}
	else if(nAutoGenType == LCOM_TYPE::LCOM_SEISMIC)
	{
#if defined(_CIVIL)
		CCmdAutoLoadComb_SE_CVL_Dlg seis_dlg;
		seis_dlg.SetLcomType(m_nLcomType);
		seis_dlg.DoModal();
#else
		CCmdAutoLoadComb_SE_Dlg seis_dlg;
		seis_dlg.SetLcomType(m_nLcomType);
		seis_dlg.DoModal();
#endif
	}
	else if(nAutoGenType == LCOM_TYPE::LCOM_LINEAR)
	{
		T_AGEN_D AgenD;
		AgenD.nDlgID = D_CMD2_DO_MODAL_AUTO_GEN_LINEAR_EVAL;

		CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
		pHeadOffice->ReqService(_ULS(cmd2), D_CMD2_DO_MODAL, (void*)&AgenD);
	}
}
//////////////////////////////////////////////////////////////

void CCMLoadCombDlg::OnCmdBtnImport()
{
	CString strFilter = _T("Excel File (*.xlsx)|*.xlsx|\0");
	strFilter += _T("LCB Files (*.lcb)|*.lcb|\0");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES, strFilter);
	fileDlg.m_ofn.nFilterIndex = 1;

	if (fileDlg.DoModal() != IDOK)
		return;

	// 우선 Excel 만... lcb, lcp 는 포멧을 정확히 모르겠어서 
	// 사용자 문의가 들어오면 비슷한 패턴으로 수정 해야 할듯....ㅠㅠ
	BOOL bExcel = FALSE;
	const auto extension = fileDlg.GetFileExt();
	if (extension.CompareNoCase(_T("xlsx")) == 0)
	{
		bExcel = TRUE;
	}

	if (bExcel)
	{
		if (m_nLcomType == D_LCOMTYPE_GENERAL)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_General)), CMDTYPE_FREE)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_STEEL)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_Steel_Design)), CMDTYPE_REMOVE_STEEL)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_Concrete_Desig)), CMDTYPE_REMOVE_CONC_AND_COLKR)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_FDN)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_BasePlate_Desi)), CMDTYPE_REMOVE_FDN)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_SRC)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_SRC_Design)), CMDTYPE_REMOVE_SRC)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_ALUMINUM)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_Aluminum_Design)), CMDTYPE_REMOVE_ALUMINUM)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_STLCOMP)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_Steel_Composite_Design)), CMDTYPE_REMOVE_PLATEGIRDER)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_CFSTEEL)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_CFSteel_Design)), CMDTYPE_REMOVE_CFSTEEL)) { ASSERT(0); return; }
		}
		else if (m_nLcomType == D_LCOMTYPE_SEISMIC)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_Seismic)), CMDTYPE_FREE)) { ASSERT(0); return; }
			IUsageCounter::Use(_T("LCOMSE"));
		}
		else if (m_nLcomType == D_LCOMTYPE_LINEAR_EVAL)
		{
			if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Load_Combination_Linear_Eval)), CMDTYPE_FREE)) { ASSERT(0); return; }
		}
		else
		{
			ASSERT(FALSE);
			return;
		}
	}	

	const auto ImportLoadCombList = [&, strFilePathName = fileDlg.GetPathName()] () -> bool
	{
		if (::PathFileExists(strFilePathName) == FALSE)
			return false;
		const auto extension = fileDlg.GetFileExt();
		if (extension.CompareNoCase(_T("xlsx")) == 0)
			return ImportLoadCombListFromExcel(strFilePathName);

		if (extension.CompareNoCase(_T("lcb")) == 0)
			return ImportLoadCombListFromLCB(strFilePathName);

		return false;
	};

	if(ImportLoadCombList())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Import_Completed));

		if (bExcel) m_pDoc->m_pUndoCtrl->CloseEditDB(TRUE);
	}
	else
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Import_Failed));

		if (bExcel) m_pDoc->m_pUndoCtrl->CancelEditDB();
	}		
}

void CCMLoadCombDlg::OnSelchangeCmdTabCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndTab.GetCurSel();

	int nSelectedLcomType = m_aTabLcomType[nIndex];  
	CDlgUtil::CtrlShowHide(this, arLcomType, m_nSeismicType == LCOM_GENERAL && nSelectedLcomType == D_LCOMTYPE_GENERAL);

	if (m_nLcomType == nSelectedLcomType) return;

	if (m_bSpreadSheetStyle)
		m_pCombList->OnTerminate(D_TB_SAVE_ALWAYS);
	else m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);

	m_nLcomType = nSelectedLcomType;
	m_pLoadCombList->ChangeLcomType(m_nLcomType);
	m_pContentList->ChangeLcomType(m_nLcomType);
	m_pCombList->ChangeLcomType(m_nLcomType);

	m_pLoadCombList->DeleteAllRecords();
	m_pLoadCombList->MakeItemEx();
	m_pContentList->DeleteAllRecords();
	m_pContentList->m_sKey = _T("");

	m_pCombList->DeleteAllRecords();
	m_pCombList->SetColInfo();
	m_pCombList->MakeItemEx();

	m_pLoadCombList->CheckCurrentRecordChanged();

	//m_wndAutoGenBtn.EnableWindow(m_nLcomType!=D_LCOMTYPE_GENERAL);

	*pResult = 0;
}

void CCMLoadCombDlg::OnCmdBtnChangeStyle() 
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetDlgItem(IDC_CMD_BTN_CHANGE_STYLE);
	if (m_bSpreadSheetStyle)
	{
		m_bSpreadSheetStyle = FALSE;
		pWnd->SetWindowText(_LS(IDS_WG_CMD__ADDD__Spread_Sheet_Form));
		m_pCombList->OnTerminate(D_TB_SAVE_ALWAYS);
		m_pCombList->DeleteAllRecords();
		CDlgUtil::CtrlShowHide(this, m_aCtrlNormalStyle, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSpreadStyle, FALSE);
	}
	else
	{
		pWnd->SetWindowText(_LS(IDS_WG_CMD__ADDD__Normal_Form));
		m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS);

		m_bSpreadSheetStyle = TRUE;
		//m_pCombList->DeleteAllRecords();
		//m_pCombList->MakeItemEx();
		m_pCombList->OnStageChanged();	// Stage 변경효과를 이용해서 Grid Update
		CDlgUtil::CtrlShowHide(this, m_aCtrlNormalStyle, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrlSpreadStyle, TRUE);
	}
}

void CCMLoadCombDlg::OnCmdBtnCopy() 
{
	// TODO: Add your control notification handler code here
	T_LCOM_D LcomData, tlcom;
	LcomData.Initialize();
	ROWCOL ncRow, ncCol;
	CString csKey;
	if (m_bSpreadSheetStyle)
	{
		if (!m_pCombList->GetCurrentCell(ncRow, ncCol)) return;
		if (m_pCombList->IsAppendRow(ncRow))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_specified_load_combina));
			return;
		}
		if (m_pCombList->IsEdit())
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_copy_while_editing_));
			return;
		}
		csKey = m_pCombList->GetKeyString(ncRow);
	}
	else
	{
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
		csKey = m_pLoadCombList->GetKeyString(ncRow);
	}
	T_LCOM_K key = _ttol(csKey);
	if (!m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, key, LcomData)) return;
	// 하중조합조건 이름을 변경한다.
	int nNo=m_pDoc->m_pAttrCtrl->GetCountLcom(m_nLcomType)+1;
	do
	{
		LcomData.LoadCombName.Format(_LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_default) + _T("%d"), nNo++);
	}
	while (m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LcomData.LoadCombName, tlcom));
	m_pDoc->m_pDataCtrl->AddLcom(m_nLcomType, LcomData);
}

void CCMLoadCombDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CInternationalDlg::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	if ((lpwndpos->flags & SWP_NOSIZE) == 0)  // Size가 변경된 경우
	{
		if(isMaximized)
			isMaximized = false;
		else
			SetRedraw(FALSE);

		if (!m_bPosSet)
		{
			GetWindowRect(m_rectDlg);
			UINT aID[] = { IDC_CMD_TAB_CTRL, 
				IDC_CMD_FRAME1, IDC_CMD_FRAME2, IDC_CMD_FRAME3, 
				IDC_CMD_LCBD_GRID1, IDC_CMD_LCBD_GRID2, IDC_CMD_LCBD_GRID3,
				IDC_CMD_BTN_COPY, IDC_CMD_BTN_CLOSE };

			for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
				GetDlgItem(aID[i])->GetWindowRect(m_rectItem[i]);
			
			m_bPosSet = TRUE;
		}
		else
		{
			CRect r, rDlg;
			GetWindowRect(rDlg);

			r.left = rDlg.left + (m_rectItem[0].left - m_rectDlg.left);
			r.top = rDlg.top + (m_rectItem[0].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectItem[0].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectItem[0].bottom);
			ScreenToClient(r);
			GetDlgItem(IDC_CMD_TAB_CTRL)->MoveWindow(r, FALSE);

			int dx, dy;
			CArray<UINT, UINT> aBtn;
			aBtn.Add(IDC_CMD_BTN_CLOSE);
			GetDlgItem(aBtn[0])->GetWindowRect(r);
			dx = (rDlg.right - r.right) - (m_rectDlg.right - m_rectItem[8].right);
			dy = (rDlg.bottom - r.bottom) - (m_rectDlg.bottom - m_rectItem[8].bottom);
      CDlgUtil::CtrlMoveDistXY(this, aBtn, dx, dy, FALSE);

			aBtn.RemoveAll();
			aBtn.Add(IDC_CMD_BTN_COPY);
			aBtn.Add(IDC_CMD_BTN_IMPORT);
			aBtn.Add(IDC_CMD_BTN_AUTOGEN);
			aBtn.Add(IDC_CMD_BTN_CHANGE_STYLE);
			aBtn.Add(IDC_CMD_FRM_LOADCOMB);
			aBtn.Add(IDC_CMD_BTN_MAKE_SHEET);
			aBtn.Add(IDC_CMD_BTN_COPY_DGN);
			aBtn.Add(IDC_CMD_COPY_DGN_COMB);
			CDlgUtil::CtrlMoveDistY(this, aBtn, dy, FALSE);

			r.left = rDlg.left + (m_rectItem[1].left - m_rectDlg.left);
			r.top = rDlg.top + (m_rectItem[1].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectItem[1].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectItem[1].bottom);
			ScreenToClient(r);
			GetDlgItem(IDC_CMD_FRAME1)->MoveWindow(r, FALSE);

			r.left = rDlg.left + (m_rectItem[4].left - m_rectDlg.left);
			r.top = rDlg.top + (m_rectItem[4].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectItem[4].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectItem[4].bottom);
			ScreenToClient(r);
			GetDlgItem(IDC_CMD_LCBD_GRID1)->MoveWindow(r, FALSE);
			
			r.left = rDlg.right - (m_rectDlg.right - m_rectItem[2].left);
			r.top = rDlg.top + (m_rectItem[2].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectItem[2].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectItem[2].bottom);
			ScreenToClient(r);
			GetDlgItem(IDC_CMD_FRAME2)->MoveWindow(r, FALSE);

			r.left = rDlg.right - (m_rectDlg.right - m_rectItem[5].left);
			r.top = rDlg.top + (m_rectItem[5].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectItem[5].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectItem[5].bottom);
			ScreenToClient(r);
			GetDlgItem(IDC_CMD_LCBD_GRID2)->MoveWindow(r, FALSE);

			r.left = rDlg.left + (m_rectItem[3].left - m_rectDlg.left);
			r.top = rDlg.top + (m_rectItem[3].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectItem[3].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectItem[3].bottom);
			ScreenToClient(r);
			GetDlgItem(IDC_CMD_FRAME3)->MoveWindow(r, FALSE);

			r.left = rDlg.left + (m_rectItem[6].left - m_rectDlg.left);
			r.top = rDlg.top + (m_rectItem[6].top - m_rectDlg.top);
			r.right = rDlg.right - (m_rectDlg.right - m_rectItem[6].right);
			r.bottom = rDlg.bottom - (m_rectDlg.bottom - m_rectItem[6].bottom);
			ScreenToClient(r);
			GetDlgItem(IDC_CMD_LCBD_GRID3)->MoveWindow(r, FALSE);

			Invalidate();
		}
	}
}


void CCMLoadCombDlg::OnCmdBtnCopyIntoDgn() 
{
	T_LCOM_K LcomKey;
	CRowColArray awRows;
	
	CString csKey;
	CStringArray caKey; caKey.RemoveAll();
	int nRSize;
	if (m_bSpreadSheetStyle)
	{
		if (!m_pCombList->GetSelectedRows(awRows, TRUE, FALSE)) return;
		nRSize = awRows.GetSize();
		if (m_pCombList->IsAppendRow(nRSize))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_specified_load_combina));
			return;
		}
		if (m_pCombList->IsEdit())
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_copy_while_editing_));
			return;
		}
		for(int i=0; i<nRSize; i++)
		{
			csKey = m_pCombList->GetKeyString(awRows[i]);
			caKey.Add(csKey);
		}
	}
	else
	{
		if (!m_pLoadCombList->GetSelectedRows(awRows, TRUE, FALSE)) return;
		nRSize = awRows.GetSize();
		if (m_pLoadCombList->IsAppendRow(nRSize))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_specified_load_combina));
			return;
		}
		if (m_pLoadCombList->IsEdit())
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_copy_while_editing_));
			return;
		}
		for(int i=0; i<nRSize; i++)
		{
			csKey = m_pLoadCombList->GetKeyString(awRows[i]);
			caKey.Add(csKey);
		}
	}

	if(nRSize > 0)
	{
		CArray<T_LCOM_K, T_LCOM_K> aLcomKey;
		aLcomKey.RemoveAll();
		int nCopyLcomType = CDlgUtil::CobxGetCurSelItemData(m_cmbLcomType, m_cmbLcomType.GetCurSel());;
		for(int i=0; i<nRSize; i++) 
		{
			LcomKey = _ttol(caKey[i]);
			aLcomKey.Add(LcomKey);
		}
		m_pDoc->m_pDataCtrl->CopyIntoLcom(nCopyLcomType, aLcomKey);
	}
}

void CCMLoadCombDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ( nID == SC_CLOSE )
	{
		DestroyWindow();
		return;
	}

	if ((nID & 0xFFF0) == SC_MAXIMIZE || (nID & 0xFFF0) == SC_RESTORE)
		isMaximized = true;

	CDialog::OnSysCommand(nID, lParam);
}

void CCMLoadCombDlg::InitLcomTypeCombo()
{
	m_cmbLcomType.ResetContent();

	ASSERT(m_aTabTitle.GetSize());
	ASSERT(m_aTabLcomType.GetSize());

	for (int i = 0; i < m_aTabTitle.GetSize(); i++) 
	{
		if(m_aTabLcomType[i] == D_LCOMTYPE_GENERAL) continue;
		CDlgUtil::CobxAddItem(m_cmbLcomType, m_aTabTitle[i], m_aTabLcomType[i]);
	}

	m_cmbLcomType.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbLcomType);
	UpdateData(FALSE);
}

void CCMLoadCombDlg::OnCmdBtnExport()
{
	CString strFilter = _T("Excel File (*.xlsx)|*.xlsx|\0");
	strFilter += _T("LCP File (*.lcp)|*.lcp|\0");

	CFileDialog fileDlg(FALSE, NULL, GetDefaultFileName(), OFN_FILEMUSTEXIST | OFN_LONGNAMES, strFilter);
	fileDlg.m_ofn.nFilterIndex = 1;

	if (fileDlg.DoModal() != IDOK)
		return;

	const auto ExportLoadCombList = [&]()
	{
		const int nFilterIndex = fileDlg.m_ofn.nFilterIndex;
		CString strSelectedFilter;
		if (nFilterIndex == fileDlg.m_ofn.nMaxCustFilter + 1)
			strSelectedFilter = _T(".xlsx");
		else if (nFilterIndex == fileDlg.m_ofn.nMaxCustFilter + 2)
			strSelectedFilter = _T(".lcp");

		const auto fileName = fileDlg.GetPathName();
		if(fileName.IsEmpty()) return false;

		const auto& strFilePathName = fileName + strSelectedFilter;

		if (strSelectedFilter == _T(".xlsx"))
			return ExportLoadCombListFromExcel(strFilePathName);

		if (strSelectedFilter == _T(".lcp"))
			return ExportLoadCombListFromLCP(strFilePathName);
		return false;
	};

	if (ExportLoadCombList())
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Export_Completed));
	else
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Export_Failed));
}

bool CCMLoadCombDlg::ExportLoadCombListFromExcel(const CString& strFile) const
{
	CFileStatus FStatus;
	if (CFile::GetStatus(strFile, FStatus)) // file exist
	{
		if (AfxMessageBox(_LS(IDS_CMD_File_Already_Exist), MB_YESNO) == IDNO)  // _T("The file already exists, overwrite?")    
			return false;
	}

	const auto pExcel = std::make_unique<CMSExcel>();
	if (pExcel->IsExcel() == false)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Can_t_launch_Excel));
		return false;
	}

	CString strDesignType;
	if (!ConvertLcomTypeToString(m_nLcomType, strDesignType)) return false;

	const auto res = MGridExportToExcel()
		.SetFileName(strFile)
		.SetPreface({ strDesignType }, LOADCOMB_EXCEL::designTypeCol)
		.RegisterTable(m_pLoadCombList, LOADCOMB_EXCEL::sheetName)
		.SetVisible(false)
		.ExportExcel();

	if (!res) return false;

	// load data
	pExcel->Open(strFile);
	pExcel->SetActiveSheet(LOADCOMB_EXCEL::sheetName);
	pExcel->SetWindowState(XlWindowState::xlNormal);

	CString sValue;
	const int startCol = m_pLoadCombList->GetColCount();

	CArray<T_LCOM_K, T_LCOM_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_nLcomType, aKeyList);
	auto nRecCount = aKeyList.GetSize();
	int maxLoadCaseCount = 0;
	for (int row = 0; row < nRecCount; ++row)
	{
		T_LCOM_D data;
		// load data and fill each row
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, aKeyList[row], data);
		int nCount = m_pDoc->m_pAttrCtrl->GetCountLcomCase(m_nLcomType, data.LoadCombName);
		if (nCount == 0) continue;

		CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
		m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

		for (int col = 0; col < nCount; col++)
		{
#if defined(_CIVIL)
			if (data.aCombination[col].AnalType == D_LCOM_STAGE)
			{
				unsigned int SgldKey = data.aCombination[col].LoadCaseKey;
				// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
				if (SgldKey >= D_SGLD_ER1_CVL && SgldKey < D_SGLD_SUM_CVL)
				{
					int nTemp = 0;
					if (!mapErect.Lookup(SgldKey, nTemp)) continue;
				}
			}
#else
			if (data.aCombination[col].AnalType == D_LCOM_STAGE)
			{
				unsigned int SgldKey = data.aCombination[col].LoadCaseKey;
				// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
				if (SgldKey >= D_SGLD_ER1_GEN && SgldKey < D_SGLD_SUM_GEN)
				{
					int nTemp = 0;
					if (!mapErect.Lookup(SgldKey, nTemp)) continue;
				}
			}
#endif

			if (data.aCombination[col].AnalType==D_LCOM_ECCENSPEC)
			{
				T_SPLC_D SplcD;
				if (!m_pDoc->m_pAttrCtrl->GetSplc(data.aCombination[col].LoadCaseKey, SplcD)) { ASSERT(0); continue; }
				if (!SplcD.bConsiderEccen) continue;
			}
			else if (data.aCombination[col].AnalType==D_LCOM_SPECTRUM_ND)
			{
				T_SPLC_D SplcD;
				if (!m_pDoc->m_pAttrCtrl->GetSplc(data.aCombination[col].LoadCaseKey, SplcD)) { ASSERT(0); continue; }
				if (!SplcD.bNonDissipative) continue;
			}
			else if (data.aCombination[col].AnalType==D_LCOM_ECCENSPEC_ND)
			{
				T_SPLC_D SplcD;
				if (!m_pDoc->m_pAttrCtrl->GetSplc(data.aCombination[col].LoadCaseKey, SplcD)) { ASSERT(0); continue; }
				if (!SplcD.bConsiderEccen || !SplcD.bNonDissipative) continue;
			}

			int loadCaseType;
			CDBLib::GetLoadCaseTypeByLoadCombType(data.aCombination[col].AnalType, loadCaseType);
			sValue = CDBLib::GetLoadCaseNameByKey_ExceptType(loadCaseType, data.aCombination[col].LoadCaseKey, 2); // param 2 means getting the name without minmax info.

			pExcel->SetValue(row + LOADCOMB_EXCEL::startRow, startCol +(col* LOADCOMB_EXCEL::colNumOfLoadCaseInfo), sValue);
			pExcel->SetValue(row + LOADCOMB_EXCEL::startRow, startCol +(col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo)+1, CDBLib::GetCaseTypeName2(loadCaseType));
			pExcel->SetValue(row + LOADCOMB_EXCEL::startRow, startCol +(col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo)+2, data.aCombination[col].Factor);

			maxLoadCaseCount = max(maxLoadCaseCount, col+1);
		}
	}

	CString val;
	for (int col = 0; col < maxLoadCaseCount; ++col) // set scheme: Load Case, Factor
	{
		val.Format(_T("Case %d"), col+1);
		pExcel->SetValue(LOADCOMB_EXCEL::headerRow, startCol + (col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo), val);
		pExcel->SetValue(LOADCOMB_EXCEL::headerRow, startCol + (col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo)+1, LOADCOMB_EXCEL::type);
		pExcel->SetValue(LOADCOMB_EXCEL::headerRow, startCol + (col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo)+2, LOADCOMB_EXCEL::factor);
		pExcel->SetColumnWidth(LOADCOMB_EXCEL::headerRow, startCol + (col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo), LOADCOMB_EXCEL::headerRow, startCol + (col*3), LOADCOMB_EXCEL::loadCaseColWidth);
		pExcel->SetColumnWidth(LOADCOMB_EXCEL::headerRow, startCol + (col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo)+1, LOADCOMB_EXCEL::headerRow, startCol + (col*3)+1, LOADCOMB_EXCEL::typeColWidth);
		pExcel->SetColumnWidth(LOADCOMB_EXCEL::headerRow, startCol + (col*LOADCOMB_EXCEL::colNumOfLoadCaseInfo)+2, LOADCOMB_EXCEL::headerRow, startCol + (col*3)+2, LOADCOMB_EXCEL::factorColWidth);
	}
	const int lastCol = (maxLoadCaseCount-1)*LOADCOMB_EXCEL::colNumOfLoadCaseInfo+LOADCOMB_EXCEL::colNumOfLoadCaseInfo-1;
	for (int i = 0; i < nRecCount; ++i)
	{
		const auto rowColor = (LOADCOMB_EXCEL::startRow + i) % 2 ? EXCEL_STYLE::D_COLOR_ROW_ODD : EXCEL_STYLE::D_COLOR_ROW_EVEN;
		pExcel->SetBackColor(LOADCOMB_EXCEL::startRow + i, startCol, LOADCOMB_EXCEL::startRow + i, startCol +lastCol, rowColor);
	}
	// scheme style
	pExcel->SetBackColor(LOADCOMB_EXCEL::headerRow, startCol, LOADCOMB_EXCEL::headerRow, startCol + lastCol, EXCEL_STYLE::D_COLOR_HEADER);
	pExcel->SetFontBold(LOADCOMB_EXCEL::headerRow, startCol, LOADCOMB_EXCEL::headerRow, startCol + lastCol, TRUE);
	pExcel->SetHorAlign(LOADCOMB_EXCEL::headerRow, startCol, LOADCOMB_EXCEL::headerRow, startCol + lastCol, 3);

	pExcel->SetVisible(TRUE);
	pExcel->Save(pExcel->GetBookName());
	return true;
}

bool CCMLoadCombDlg::ExportLoadCombListFromLCP(const CString& strFile) const
{
	CFileStatus FStatus;
	FILE* fout;

	if (strFile ==_T("")) return FALSE;

	if (!CFile::GetStatus(strFile, FStatus)) // file doesn't exist
	{
		if ((fout = _tfopen(strFile, _T("w"))) == NULL) return FALSE;
	}
	else                                     // file exist
	{
		if (AfxMessageBox(_LS(IDS_CMD_File_Already_Exist), MB_YESNO) == IDYES)  // _T("The file already exists, overwrite?\nSelect 'No' to add")    
		{
			if ((fout = _tfopen(strFile, _T("w"))) == NULL) return FALSE;
		}
		else
		{
			if ((fout = _tfopen(strFile, _T("a"))) == NULL) return FALSE;
		}
	}

	T_PREFERENCE rPreference;
	m_pDoc->m_pInitCtrl->GetPreference(rPreference);

	CString strFullNa = D_MIDAS_FULL_NAME;
	CString strAddTxt = _LS(IDS_CMD_LOADCOMB_Load_Combinations);
	CString strProdNa = D_PRODUCT_NAME;
	CString strCpRgYr = D_PRODUCT_COPYRIGHT_YEAR;
	CString strCoFuNa = D_COMPANY_FULL_NAME;
	CString strCoAbNa = D_COMPANY_ABB_NAME;
	CString strUrlNa = D_MIDAS_URL;
	CString strVerStr = D_PRODUCT_VERSION_STR;

#if defined(_EDU)
	CString strTxt1 = _T(""); strTxt1.Format(_T("%-56s"), _T("                         MIDAS                          "));
#else
	CString strTxt1 = _T(""); strTxt1.Format(_T("%-56s"), _T("MIDAS(") + strFullNa + _T(")"));
#endif
	CString strTxt2 = _T(""); strTxt2.Format(_T("%-56s"), strProdNa + strAddTxt);
	CString strTxt3 = _T(""); strTxt3.Format(_T("%56s"), _T("(c)") + strCpRgYr);
	CString strTxt4 = _T(""); strTxt4.Format(_T("%-40s%16s"), strCoFuNa, _T("(") + strCoAbNa + _T(")"));
	CString strTxt5 = _T(""); strTxt5.Format(_T("%-56s"), strProdNa + _T("  Version ") + strVerStr);

	CString strProNaYear = D_PRODUCT_NAME_YEAR;
#if defined(_MGEN) && ( defined (_US) || defined (_RUS) || defined (_ORG)) // PMS:4075-JWKWON-20090918 // MNET:XXXX-JWKWON-20141010 : 국문도 영문처럼 버전 표시
	strTxt5.Format(_T("%-56s"), strProNaYear);
#endif


if (CProduct::IsRusLocal())
{
	CTextFileOut::WriteMbcs(fout, _T("\n"));
	CTextFileOut::WriteMbcs(fout, _T("               +============================================================+\n"));	//  +============================================================+
	CTextFileOut::WriteMbcs(fout, _T("               |                                                            |\n"));
	CTextFileOut::WriteMbcs(fout, _T("               |  %s  |\n"), (LPCTSTR)strTxt2);                                      //  |  MIDAS/Gen - Load Combinations      |
	CTextFileOut::WriteMbcs(fout, _T("               |                                                            |\n")); 
	CTextFileOut::WriteMbcs(fout, _T("               +============================================================+\n"));	//  +============================================================+
	CTextFileOut::WriteMbcs(fout, _T("\n"));
}
else
{
	CTextFileOut::WriteMbcs(fout, _T("\n"));
	CTextFileOut::WriteMbcs(fout, _T("               +============================================================+\n"));	//  +============================================================+
	CTextFileOut::WriteMbcs(fout, _T("               |  %s  |\n"), (LPCTSTR)strTxt1);	                                              //  |  MIDAS(Modeling, Integrated Design & Analysis Software)    |
	CTextFileOut::WriteMbcs(fout, _T("               |  %s  |\n"), (LPCTSTR)strTxt2);	                                              //  |  MIDAS/Gen - Design & checking system for windows        |
	CTextFileOut::WriteMbcs(fout, _T("               |  %s  |\n"), (LPCTSTR)strTxt3);	                                              //  |                                            (c)1990 - 2001  |
	CTextFileOut::WriteMbcs(fout, _T("               +============================================================+\n"));	//  +============================================================+
	CTextFileOut::WriteMbcs(fout, _T("               |  %s  |\n"), (LPCTSTR)strTxt4);                                             	//  |  MIDAS Information Technology Co., Ltd.        (MIDAS IT)  |
	CTextFileOut::WriteMbcs(fout, _T("               |  %s  |\n"), (LPCTSTR)strTxt5);                                               //  |  MIDAS/TOWER  Version 1.1.0                                |
	CTextFileOut::WriteMbcs(fout, _T("               +============================================================+\n"));	//  +============================================================+
	CTextFileOut::WriteMbcs(fout, _T("\n"));
}

	CString strDesignType;
	switch (m_nLcomType)
	{
	case D_LCOMTYPE_GENERAL: strDesignType = _LS(IDS_WG_CMD__ADDD__General); break;
	case D_LCOMTYPE_STEEL: strDesignType = _LS(IDS_WG_CMD__ADDD__Steel_Design); break;
	case D_LCOMTYPE_CONCRETE: strDesignType = _LS(IDS_WG_CMD__ADDD__Concrete_Design); break;
	case D_LCOMTYPE_SRC: strDesignType = _LS(IDS_WG_CMD__ADDD__SRC_Design); break;
	case D_LCOMTYPE_FDN: strDesignType = _LS(IDS_WG_CMD__ADDD__Footing_Design); break;
	case D_LCOMTYPE_ALUMINUM: strDesignType = _LS(IDS_WG_CMD__ADDD__Aluminum_Design); break;
	case D_LCOMTYPE_STLCOMP: strDesignType = _LS(IDS_WG_CMD__ADDD__Steel_Composite_Design); break;
	case D_LCOMTYPE_CFSTEEL: strDesignType = _LS(IDS_WG_CMD__ADDD__CFSteel_Design); break;
	case D_LCOMTYPE_SEISMIC: strDesignType = _LS(IDS_WG_CMD__ADDD__Seismic); break;
	case D_LCOMTYPE_LINEAR_EVAL: strDesignType = _LSX(Linear Eval.); break;
	}

	CString strDesignType2 = _LS(IDS_CMD_LOADCOMB_DESIGN_TYPE)              + _T("\n");
	CString strDesignType3 = _LS(IDS_CMD_LOADCOMB_LIST_OF_LOAD_COMBINATIONS)+ _T("\n");
	CString strDesignType4 = _LS(IDS_CMD_LOADCOMB_NUM_NAME_ACTIVE_TYPE)     + _T("\n");
	CString strDesignType5 = _LS(IDS_CMD_LOADCOMB_LOADCASE_FACTOR)          + _T("\n");

	CTextFileOut::WriteMbcs(fout, _T("-----------------------------\n"));
	CTextFileOut::WriteMbcs(fout, strDesignType2, (LPCTSTR)strDesignType);
	CTextFileOut::WriteMbcs(fout, _T("-----------------------------\n\n"));
	CTextFileOut::WriteMbcs(fout, strDesignType3);
	CTextFileOut::WriteMbcs(fout, _T("=============================================================================================\n"));
	CTextFileOut::WriteMbcs(fout, strDesignType4);
	CTextFileOut::WriteMbcs(fout, strDesignType5);
	CTextFileOut::WriteMbcs(fout, _T("=============================================================================================\n"));

	const auto GetLoadCaseName = [&](const T_LCOM_BASE& rLcomBase) -> CString
		{
			CString strLDCNa = _T("");
			switch (rLcomBase.AnalType)
			{
			case D_LCOM_STATIC:
			{
				T_STLD_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetStld(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_SPECTRUM:
			{
				T_SPLC_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetSplc(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_SPECTRUM_ND:
			{
				T_NSPC_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetNspc(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_HISTORY:
			{
				T_THIS_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetThis(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_MOVING:
			{
				// 코드에 따른 방식으로 변경 
				T_MVCD_D DataMvcd;
				if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
				switch (DataMvcd.nCodeType)
				{
				case D_MOVE_CODE_NONE:
					break;
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
				case D_MOVE_CODE_JAPAN_RAIL:
				case D_MOVE_CODE_NEWZEALAND:
				case D_MOVE_CODE_BRAZIL:
				{
					T_MVLD_D LcaseData;
					LcaseData.Initialize();
					m_pDoc->m_pAttrCtrl->GetMvld(rLcomBase.LoadCaseKey, LcaseData);
					strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
				}
				break;
				case D_MOVE_CODE_CHINA:
				{
					T_MVLDch_D LcaseData;
					LcaseData.Initialize();
					m_pDoc->m_pAttrCtrl->GetMvldch(rLcomBase.LoadCaseKey, LcaseData);
					strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
				}
				break;
				case D_MOVE_CODE_JAPAN:
				{
					T_MVLDjp_D LcaseData;
					m_pDoc->m_pAttrCtrl->InitializeMvldjp(LcaseData);
					m_pDoc->m_pAttrCtrl->GetMvldjp(rLcomBase.LoadCaseKey, LcaseData);
					strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
				}
				break;
				case D_MOVE_CODE_INDIA:
				{
					T_MVLDid_D LcaseData;
					LcaseData.Initialize();
					m_pDoc->m_pAttrCtrl->GetMvldid(rLcomBase.LoadCaseKey, LcaseData);
					strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
				}
				break;
				case D_MOVE_CODE_BS:
				{
					T_MVLDbs_D LcaseData;
					LcaseData.Initialize();
					m_pDoc->m_pAttrCtrl->GetMvldbs(rLcomBase.LoadCaseKey, LcaseData);
					strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
				}
				break;
				case D_MOVE_CODE_FRANCE:
				{
					T_MVLDfr_D LcaseData;
					LcaseData.Initialize();
					m_pDoc->m_pAttrCtrl->GetMvldfr(rLcomBase.LoadCaseKey, LcaseData);
					strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
				}
				break;
				case D_MOVE_CODE_TRANS:
				{
					T_MVLDtr_D LcaseData;
					LcaseData.Initialize();
					m_pDoc->m_pAttrCtrl->GetMvldtr(rLcomBase.LoadCaseKey, LcaseData);
					strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
				}
				break;
				default:
					ASSERT(0);
					break;
				}
			}
			break;
			case D_LCOM_SETTLEMENT:
			{
				T_SMLC_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetSmlc(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_STAGE:
			{
				T_SGLD_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetSgld(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));

			}
			break;
			case D_LCOM_ECCENSPEC:
			{
				T_ESPC_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetEspc(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_ECCENSPEC_ND:
			{
				T_NSPC_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetNspc(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_GRIDMODEL:
			{
				T_GILC_D LcaseData;
				LcaseData.Initialize();
				m_pDoc->m_pAttrCtrl->GetGilc(rLcomBase.LoadCaseKey, LcaseData);
				strLDCNa.Format(_T("%20s"), LcaseData.LoadCaseName.Left(20));
			}
			break;
			case D_LCOM_ENVELOPE:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_GENERAL:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_STEEL:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_CFSTEEL:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CFSTEEL, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_CONCRETE:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_FOOTING:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_FDN, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_ALUMINUM:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_ALUMINUM, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_STLCOMP:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STLCOMP, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_SRC:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SRC, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			// Coded by Seungjun ('20070507) MNet:No.2778. Crane Load
			case D_LCOM_CRANE:
			{
				CString sLoadName = _T("CN");
				strLDCNa.Format(_T("%20s"), sLoadName.Left(20));
			}
			break;
			case D_LCOM_CB_SEISMIC:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			case D_LCOM_CB_LINEAR_EVAL:
			{
				T_LCOM_D LcomData1;
				LcomData1.Initialize();
				m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_LINEAR_EVAL, rLcomBase.LoadCaseKey, LcomData1);
				strLDCNa.Format(_T("%20s"), LcomData1.LoadCombName.Left(20));
			}
			break;
			default:
				ASSERT(0);
			}
			return strLDCNa;
		};

	CArray<T_LCOM_K, T_LCOM_K> arLcombList;
	arLcombList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_nLcomType, arLcombList);
	int nLCombList = arLcombList.GetSize();
	for (int j = 0; j < nLCombList; j++)
	{
		T_LCOM_D LcomD;
		LcomD.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, arLcombList[j], LcomD);

		// Get Loadcase Number of current LcomD
		int nLcaseNum = LcomD.aCombination.GetSize();

		const auto& strLCBName = LcomD.LoadCombName;
		const auto strLCBActive = GetCombActiveStr(m_nLcomType, LcomD.nActive);
		const auto strLCBType = GetCombTypeStr(LcomD.LoadCombType);
		if (j!=0) CTextFileOut::WriteMbcs(fout, _T("\n"));
		CTextFileOut::WriteMbcs(fout, _T("%-3d  %-12s  %-17s  %-10s"), j+1, (LPCTSTR)strLCBName, (LPCTSTR)strLCBActive, (LPCTSTR)strLCBType);

		int iLDCCount = 0;
		BOOL bFirstGoodFactor = TRUE;
		BOOL bPrintLine = FALSE;
		CString strWriteData = _T("");

		for (int k = 0; k < nLcaseNum; k++)
		{
			T_LCOM_BASE& rLcomBase = LcomD.aCombination[k];
			const auto strLDCNa = GetLoadCaseName(rLcomBase);
			// LDC Factor.
			double temp_dFactor = rLcomBase.Factor;
			// LDC Factor value.
			CString strLDCFactor = _T("");
			strLDCFactor.Format(_T("%.3f"), temp_dFactor);
			if (rLcomBase.Factor != 0.0)
			{
				iLDCCount++;
				if (bFirstGoodFactor==TRUE)	strWriteData = _T("   ");  // 하중조합의 첫번째 Load Case
				else							          strWriteData = strWriteData + _T(" + ");  // 하중조합의 첫번째 이후의 Load Case
				bFirstGoodFactor = FALSE;
				CString temp_space = _T("");
				if (rLcomBase.Factor >= 0)	temp_space = _T(" ");
				strWriteData = strWriteData + strLDCNa + _T("(") + temp_space + strLDCFactor + _T(")");
			}
			if (iLDCCount == 3 && (nLcaseNum - 1) != k)
			{
				if (bPrintLine) CTextFileOut::WriteMbcs(fout, _T("\n"));
				else CTextFileOut::WriteMbcs(fout, _T("\n"));
				CTextFileOut::WriteMbcs(fout, strWriteData);
				iLDCCount = 0;
				strWriteData = _T("");
				bPrintLine = TRUE;
			}
			if ((nLcaseNum - 1) == k)
			{
				if (bPrintLine) CTextFileOut::WriteMbcs(fout, _T("\n"));
				else CTextFileOut::WriteMbcs(fout, _T("\n"));
				CTextFileOut::WriteMbcs(fout, strWriteData);
			}
		} // end of k(num of current LcomD)
		CTextFileOut::WriteMbcs(fout, _T("\n---------------------------------------------------------------------------------------------"));
	} // end of j(num of Load combination)	

	if (fout!=NULL) fclose(fout);
	return TRUE;
}

bool CCMLoadCombDlg::ImportLoadCombListFromExcel(const CString& strFile) const
{
	const auto pExcel = std::make_unique<CMSExcel>();

	if (!pExcel->IsExcel())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Can_t_launch_Excel));
		return false;
	}
	CArray<unsigned int, unsigned int> arProcID;
	pExcel->GetExistedExcelProcessID(arProcID);

	pExcel->Open(strFile);
	pExcel->SetVisible(FALSE);

	const auto strBookName = pExcel->GetBookName();
	if (!pExcel->IsExistSheetName(LOADCOMB_EXCEL::sheetName))
	{
		pExcel->Quit();
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_sheet_name), LOADCOMB_EXCEL::sheetName);
		AfxMessageBox(msg);
		return false;
	}
	pExcel->SetActiveSheet(LOADCOMB_EXCEL::sheetName);

	int nLcomType = D_LCOMTYPE_GENERAL;

	try // for handling error caused by getting value from excel 
	{
		const CString strLcomType = pExcel->GetValueString(0, LOADCOMB_EXCEL::designTypeCol);

		if (!ConvertToDesignType(strLcomType, nLcomType))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Design_Type), pExcel->GetCell(0, LOADCOMB_EXCEL::designTypeCol));
			throw std::move(msg);
		}

		int row = LOADCOMB_EXCEL::startRow;

		for (auto key = pExcel->GetValue(row, 1); key > 0; key = pExcel->GetValue(++row, 1))
		{
			if (std::fpclassify(key) == FP_SUBNORMAL)
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Key_num), pExcel->GetCell(row, 1));
				throw std::move(msg);
			}
			// get name
			auto loadCombName = pExcel->GetValueString(row, LOADCOMB_EXCEL::loadCombNameCol);
			if (loadCombName.IsEmpty())
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Empty_String), pExcel->GetCell(row, LOADCOMB_EXCEL::loadCombNameCol)); // Please enter a value in [%s]
				throw std::move(msg);
			}
			T_LCOM_D loadComb;
			const auto isExisted = m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, loadCombName, loadComb);

			const auto active = ConvertToLoadCombActive(nLcomType, pExcel->GetValueString(row, LOADCOMB_EXCEL::loadCombActiveCol));
			if (active == -1)
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Active_Type), pExcel->GetCell(row, LOADCOMB_EXCEL::loadCombActiveCol));
				throw std::move(msg);
			}
			loadComb.nActive = active;

			const auto type = ConvertToLoadCombType(pExcel->GetValueString(row, LOADCOMB_EXCEL::loadCombTypeCol));
			if (type == -1)
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Load_Combination_Type), pExcel->GetCell(row, LOADCOMB_EXCEL::loadCombTypeCol));
				throw std::move(msg);
			}
			loadComb.LoadCombType = type;

			if(nLcomType==D_LCOMTYPE_CONCRETE && loadComb.nActive == 2)// Serviceability
			{
				const auto elasticStage = pExcel->GetValue(row, LOADCOMB_EXCEL::elasticStageCol);
				if (elasticStage != 0 && elasticStage != 1)
				{
					CString msg; // todo 
					msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Elastic_Stage_Value), pExcel->GetCell(row, LOADCOMB_EXCEL::elasticStageCol));
					throw std::move(msg);
				}
				loadComb.bElasticStage = elasticStage ? true : false;
			}

			loadComb.Description = pExcel->GetValueString(row, LOADCOMB_EXCEL::descriptionCol);

			// load cases info
			int col = LOADCOMB_EXCEL::loadCaseStartCol;
			loadComb.aCombination.RemoveAll();
			for (auto loadCaseName = pExcel->GetValueString(row, col); loadCaseName.IsEmpty() == false; loadCaseName = pExcel->GetValueString(row, col))
			{
				loadCaseName.Trim();
				if(loadCaseName.IsEmpty())
				{
					CString msg;
					msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Empty_String), pExcel->GetCell(row, col));
					throw std::move(msg);
				}

				T_LCOM_BASE loadCase;
				UINT key;
				auto strLoadCaseType = pExcel->GetValueString(row, col+1);
				auto loadCaseType = CDBLib::GetCaseTypeByName2(strLoadCaseType);
				int nAnalType;
				CDBLib::GetLoadCombTypeByLoadCaseType(loadCaseType, nAnalType);
				CDBLib::GetLoadCaseKeyByName2(loadCaseType, loadCaseName, key);

				if (key <=0)
				{
					// load case does not exist.
					CString msg;
					msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Load_Case_Name_Type), pExcel->GetCell(row, col+1));
					throw std::move(msg);
				}

				loadCase.LoadCaseKey = key;
				loadCase.AnalType = nAnalType;
				const auto factor = pExcel->GetValue(row, col+2);

				if (std::fpclassify(factor) == FP_SUBNORMAL)
				{
					CString msg;
					msg.Format(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Factor), pExcel->GetCell(row, col+2));
					throw std::move(msg);
				}
				loadCase.Factor = factor;

				loadComb.aCombination.Add(loadCase);

				col += LOADCOMB_EXCEL::colNumOfLoadCaseInfo;
			}

			if (!isExisted)
			{
				loadComb.LoadCombName = loadCombName;
				if (!m_pDoc->m_pEditData->AddLcom(nLcomType, loadComb))
				{
					ASSERT(0);
					return false;
				}
			}
			else
			{
				if (!m_pDoc->m_pEditData->ModifyLcom(nLcomType, m_pDoc->m_pAttrCtrl->GetLcomKey(nLcomType, loadComb.LoadCombName), loadComb))
				{
					ASSERT(0);
					return false;
				}
			}
		}
	}
	catch(const CString& msg)
	{
		AfxMessageBox(msg);
		pExcel->Quit();
		pExcel->KillExcelProcess(arProcID);
		return false;
	}
	catch (...)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Invalid_Import_Form));
		pExcel->Quit();
		pExcel->KillExcelProcess(arProcID);
		return false;
	}
	
	pExcel->Quit();
	pExcel->KillExcelProcess(arProcID);

	return true;
}

bool CCMLoadCombDlg::ImportLoadCombListFromLCB(const CString& strFile) const
{
	//---- Open file and read LCB information ----
	FILE* fp = _tfopen(strFile, _T("rt"));
	if (fp == NULL)
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Can_t_open___s__file_), strFile);
		AfxMessageBox(msg);
		return false;
	}

	int nStldNum = m_pDoc->m_pAttrCtrl->GetCountStld();
	int LCid;
	CArray<T_STLD_K, T_STLD_K> aStldKey;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldKey);

	CLoadCombCtrl LoadCombCtrl;

	CString strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_default);
	switch (m_nLcomType)
	{
	case D_LCOMTYPE_GENERAL:		strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_GENERAL);	break;
	case D_LCOMTYPE_STEEL:			strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_STEEL); break;
	case D_LCOMTYPE_CONCRETE:		strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_CONCRETE); break;
	case D_LCOMTYPE_FDN:			strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_FDN); break;
	case D_LCOMTYPE_SRC:			strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_SRC); break;
	case D_LCOMTYPE_ALUMINUM:		strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_ALUMINUM); break;
	case D_LCOMTYPE_CFSTEEL:		strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_CFSTEEL);	break;
	case D_LCOMTYPE_SEISMIC:		strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_SEISMIC);	break;
	case D_LCOMTYPE_LINEAR_EVAL:	strPreLCB = _LS(IDS_CMD_LOAD_COMB_IMPORT_PRE_NAME_LINEAR_EVAL);	break;
	default: ASSERT(0); break;
	}

	CString msg;
	CArray<T_LCOM_D, T_LCOM_D&> aLCOM;
	T_LCOM_D lcom;
	CStringArray aToken;
	int nLineNo = 0;
	int nNum;
	char buf[1024];
	aToken.SetSize(20);
	char* token;
	CString csToken;
	while (fgets(buf, 1024, fp))
	{
		nLineNo++;
		if (strlen(buf) >= 1020)
		{
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Invalid_LCB_file_format_));
			AfxMessageBox(msg);
			fclose(fp);
			return false;
		}
		nNum = 0;
		token = strtok(buf, ",\r\n");
		while (token)
		{
			csToken = token;
			csToken.TrimLeft();
			csToken.TrimRight();
			if (csToken == "**") break;
			aToken.SetAtGrow(nNum++, csToken);
			token = strtok(NULL, ",\r\n");
		}
		if (nNum > 0 && aToken[nNum-1] == _T("")) nNum--;

		//---- Build LCB from token ----
		if (nNum == 0) continue;  // comment line
		lcom.Initialize();

		if (nNum < 4)
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___No_load_case_and_factor_i), nLineNo);
		else if (nNum % 2 != 0)
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Mismatched_load_case_and_), nLineNo);
		else if ((nNum-2)/2 > D_LCOM_NUMCOMB)
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_case__d__e), (nNum-2)/2, D_LCOM_NUMCOMB);
		else if (aToken[0] == _T("") || !CFormulaEdit::GetFormulaValue(aToken[0], (int&)lcom.LoadCombId))
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_combination_), aToken[0], nLineNo);
		else msg = _T("");
		if (msg != _T(""))
		{
			AfxMessageBox(msg);
			fclose(fp);
			return false;
		}

		lcom.LoadCombName.Format(_T("%s%d"), strPreLCB, lcom.LoadCombId);

		if (aToken[1] == _T("")) lcom.LoadCombType = 0;
		else CFormulaEdit::GetFormulaValue(aToken[1], (int&)lcom.LoadCombType);

		T_LCOM_BASE* pLCB;
		int nLCB = 0;
		T_LCOM_BASE aCombination[D_LCOM_NUMCOMB];
		for (int ii = 0; ii<D_LCOM_NUMCOMB; ii++)
		{
			aCombination[ii].AnalType = 0;  // D_LCOM_???
			aCombination[ii].LoadCaseKey = 0;
			aCombination[ii].Factor = 0.;
		}
		int nIndex = m_wndTab.GetCurSel();
		int nSelectedLcomType = m_aTabLcomType[nIndex];

		int nAnalType = 0;
		T_LCOM_K LcomK = 0;
		for (int n = 2; n < nNum; n += 2)
		{
			pLCB = &aCombination[nLCB++];
			nAnalType = D_LCOM_STATIC;
			if (aToken[n] == _T(""))
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_case__s__in_), aToken[n], nLineNo);
			else if (!CFormulaEdit::GetFormulaValue(aToken[n], LCid))
			{
				LcomK = m_pDoc->m_pAttrCtrl->GetLcomKey(nSelectedLcomType, aToken[n]);
				if (LcomK==0)
				{
					msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_case__s__in_), aToken[n], nLineNo);
				}
				else
				{
					nAnalType = LoadCombCtrl.Get_AnalType(m_nLcomType);

					if (aToken[n + 1] == _T("") || !CFormulaEdit::GetFormulaValue(aToken[n + 1], pLCB->Factor))
						msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_case_factor_), aToken[n + 1], nLineNo);
				}
			}
			else if (LCid < 1 || LCid > nStldNum)
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Load_case__d_in_line__d_d), LCid, nLineNo);
			else if (aToken[n+1] == _T("") || !CFormulaEdit::GetFormulaValue(aToken[n+1], pLCB->Factor))
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_case_factor_), aToken[n+1], nLineNo);

			if (msg != _T(""))
			{
				AfxMessageBox(msg);
				fclose(fp);
				return false;
			}
			if (pLCB->Factor == 0.0) nLCB--;
			else
			{
				pLCB->LoadCaseKey = (nAnalType==D_LCOM_STATIC) ? aStldKey[LCid-1] : LcomK;
				pLCB->AnalType = nAnalType;
			}
		}
		for (int ii = 0; ii<D_LCOM_NUMCOMB; ii++)
		{
			if (aCombination[ii].LoadCaseKey == 0) break;
			lcom.aCombination.Add(aCombination[ii]);
		}
		// Find and merge if found else add
		int lcomNUM = aLCOM.GetSize();
		int k = 0;
		for (k = 0; k < lcomNUM; k++)
			if (aLCOM[k].LoadCombId == lcom.LoadCombId) break;
		if (k == lcomNUM) aLCOM.Add(lcom);  // not found
		else // found so merge
		{
			if (!CDBLib::MergeLCOM(aLCOM[k], lcom))
			{
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_case_excee), D_LCOM_NUMCOMB);
				AfxMessageBox(msg);
				fclose(fp);
				return false;
			}
		}
	}
	fclose(fp);

	if (aLCOM.GetSize() > 0)
		m_pDoc->m_pDataCtrl->AddLcom(m_nLcomType, aLCOM);
	return true;
}


CString CCMLoadCombDlg::GetCombActiveStr(int nLcomType, int nActive) const
{
	CString strActive;
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_SEISMIC)  
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
		else if(nActive == 5) strActive = _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC);
		else if(nActive == EN_DGN_UG_STRN) strActive = _LS(IDS_CMD_LCOM_UG_STRENGTH);
		else if(nActive == EN_DGN_UG_SERV) strActive = _LS(IDS_CMD_LCOM_UG_SERVICE);
		else if(nActive == EN_DGN_UG_SPEC) strActive = _LS(IDS_CMD_LCOM_UG_SPECIAL);
	}
	else if(nLcomType == D_LCOMTYPE_STEEL)  // add by Seungjun (`06.06.16) No.2079 KSSC-ASD03.
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_STRENGTH);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_SERVICE);
		else if(nActive == 3) strActive = _LS(IDS_CMD_LCOM_SPECIAL);     
		else if(nActive == 4) strActive = _LS(IDS_CMD_LCOM_VERTICAL);
		else if(nActive == 5) strActive = _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC);
		else if(nActive == EN_DGN_UG_STRN) strActive = _LS(IDS_CMD_LCOM_UG_STRENGTH);
		else if(nActive == EN_DGN_UG_SERV) strActive = _LS(IDS_CMD_LCOM_UG_SERVICE);
		else if(nActive == EN_DGN_UG_SPEC) strActive = _LS(IDS_CMD_LCOM_UG_SPECIAL);
	}
	else if(nLcomType == D_LCOMTYPE_CFSTEEL)
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_STRENGTH);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_SERVICE);
	}
	else if(nLcomType == D_LCOMTYPE_SEISMIC)
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_GENERAL);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_VERTICAL);
// 		else if(nActive == 3) strActive = _LS(IDS_CMD_LCOM_VDISP);
// 		else if(nActive == 4) strActive = _LS(IDS_CMD_LCOM_VFORCE);
	}
	else if(nLcomType == D_LCOMTYPE_LINEAR_EVAL)
	{
		if(nActive == 0)      strActive = _LS(IDS_CMD_LCOM_INACTIVE);
		else if(nActive == 1) strActive = _LS(IDS_CMD_LCOM_GRAVITY);
		else if(nActive == 2) strActive = _LS(IDS_CMD_LCOM_SEISMIC);
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

std::vector<CString> CCMLoadCombDlg::GetCombActiveStringLists(int nLcomType) const
{
		std::vector<CString> vecActive;
#if defined(_CIVIL)
		if (nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_SEISMIC)
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_ACTIVE));
		}
		else if (nLcomType == D_LCOMTYPE_CONCRETE && CProduct::IsMovingType(D_PRODUCT_MOVING_CH) && m_bTB10002_1_05)
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_STRENGTH));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SERVICE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_TB10002_1_MAIN));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL));
		}
		else // (nLcomType == D_LCOMTYPE_CONCRETE || nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_SRC)
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_STRENGTH));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SERVICE));
		}
#elif defined(_MGEN)
		if (nLcomType == D_LCOMTYPE_CONCRETE)
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_STRENGTH));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SERVICE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SPECIAL));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_VERTICAL));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_STRENGTH_ELASTIC));
			vecActive.emplace_back( _LS(IDS_CMD_LCOM_UG_STRENGTH));
			vecActive.emplace_back( _LS(IDS_CMD_LCOM_UG_SERVICE));
			vecActive.emplace_back( _LS(IDS_CMD_LCOM_UG_SPECIAL));
		}
		else if (nLcomType == D_LCOMTYPE_STEEL)  // add by Seungjun (`06.06.16) No.2079 KSSC-ASD03.
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_STRENGTH));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SERVICE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SPECIAL));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_VERTICAL));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_STRENGTH_ELASTIC));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_UG_STRENGTH));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_UG_SERVICE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_UG_SPECIAL));
		}
		else if (nLcomType == D_LCOMTYPE_CFSTEEL)
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_STRENGTH));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SERVICE));
		}
		else if (nLcomType == D_LCOMTYPE_SEISMIC)
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_GENERAL));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_VERTICAL));
		}
		else if (nLcomType == D_LCOMTYPE_LINEAR_EVAL)
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_GRAVITY));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SEISMIC));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_SPECIAL));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_VERTICAL));
		}
		else
		{
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
			vecActive.emplace_back(_LS(IDS_CMD_LCOM_ACTIVE));
		}
#else
		vecActive.emplace_back(_LS(IDS_CMD_LCOM_INACTIVE));
		vecActive.emplace_back(_LS(IDS_CMD_LCOM_ACTIVE));
#endif
		return vecActive;
}

CString CCMLoadCombDlg::GetCombTypeStr(int nCombType) const
{
	switch (nCombType)
	{
	case 0: return D_LCOM_CT_ADD;
	case 1: return D_LCOM_CT_ENVELOPE;
	case 2: return D_LCOM_CT_ABS;
	case 3: return D_LCOM_CT_SRSS;
	default:
		return _LS(IDS_WG_CMD__ADDD__Error);
	}
}

CGXGridWnd* CCMLoadCombDlg::MakeVirtualTB(UINT UtblK)
{
	ASSERT(UtblK>0);
	
	// Show Wait Cursor.
	CWaitCursor WaitCursor;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	T_UTBL_D  UtblD;
	if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
	{ ASSERT(0); return FALSE; }

	// Smart report 생성 시의 탭으로 전환
	if(!UtblD.active.aCurData.GetSize())
	{ ASSERT(0); return FALSE; }
	m_nLcomType = UtblD.active.aCurData[0];
	m_pLoadCombList->ChangeLcomType(m_nLcomType);
	m_pLoadCombList->DeleteAllRecords();
	m_pLoadCombList->MakeItemEx();
	
	// 일단 모든 열을 hide시키고, 정렬정보에 있는 열만 해제
	m_pLoadCombList->HideColsByIntend(1,7,TRUE);
	for(int i=0; i<UtblD.header.nCurColCount; i++)
	{
		int nCol = UtblD.header.aCurIndex[i] + 1;
		m_pLoadCombList->HideColsByIntend(nCol,nCol,FALSE);
	}
	
	return m_pLoadCombList.get();
}

void CCMLoadCombDlg::GetTabTitleAndType(CArray<CString, CString&>& aTabTitle, CArray<int, int>& aTabLcomType)
{
	aTabTitle.RemoveAll();
	aTabLcomType.RemoveAll();

	if(m_nSeismicType == LCOM_TYPE::LCOM_GENERAL)
	{
#if defined(_MGEN)
	#if defined(_CH)
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General));
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__CFSteel_Design));
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Aluminum_Design));
	
			aTabLcomType.Add(D_LCOMTYPE_GENERAL);
			aTabLcomType.Add(D_LCOMTYPE_STEEL);
			aTabLcomType.Add(D_LCOMTYPE_CONCRETE);
			aTabLcomType.Add(D_LCOMTYPE_SRC);
			aTabLcomType.Add(D_LCOMTYPE_CFSTEEL);
			aTabLcomType.Add(D_LCOMTYPE_ALUMINUM);
	
	#elif defined(_JP) /*|| defined(_US)*/ || defined(_RUS)
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__CFSteel_Design));
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Footing_Design));
	
			aTabLcomType.Add(D_LCOMTYPE_GENERAL);
			aTabLcomType.Add(D_LCOMTYPE_STEEL);
			aTabLcomType.Add(D_LCOMTYPE_CONCRETE);
			aTabLcomType.Add(D_LCOMTYPE_SRC);
			aTabLcomType.Add(D_LCOMTYPE_CFSTEEL);
			aTabLcomType.Add(D_LCOMTYPE_FDN);
	
	#else //KR ); US
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design)); 
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__CFSteel_Design));
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Footing_Design));
			aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Aluminum_Design));
	
			aTabLcomType.Add(D_LCOMTYPE_GENERAL);
			aTabLcomType.Add(D_LCOMTYPE_STEEL);
			aTabLcomType.Add(D_LCOMTYPE_CONCRETE);
			aTabLcomType.Add(D_LCOMTYPE_SRC);
			aTabLcomType.Add(D_LCOMTYPE_CFSTEEL);
			aTabLcomType.Add(D_LCOMTYPE_FDN);
			aTabLcomType.Add(D_LCOMTYPE_ALUMINUM);
	#endif
#elif defined(_CIVIL)
	#if defined(_ORG)
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Composite_Design));  
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Seismic));    

		aTabLcomType.Add(D_LCOMTYPE_GENERAL);
		aTabLcomType.Add(D_LCOMTYPE_STEEL);
		aTabLcomType.Add(D_LCOMTYPE_CONCRETE);
		aTabLcomType.Add(D_LCOMTYPE_SRC);
		aTabLcomType.Add(D_LCOMTYPE_STLCOMP);    
		aTabLcomType.Add(D_LCOMTYPE_SEISMIC);
	#elif  defined(_US) || defined(_RUS) || defined(_JP) 
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Composite_Design));  

		aTabLcomType.Add(D_LCOMTYPE_GENERAL);
		aTabLcomType.Add(D_LCOMTYPE_STEEL);
		aTabLcomType.Add(D_LCOMTYPE_CONCRETE);
		aTabLcomType.Add(D_LCOMTYPE_SRC);
		aTabLcomType.Add(D_LCOMTYPE_STLCOMP);    
	#elif defined(_CH)
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Composite_Design));  
		
		aTabLcomType.Add(D_LCOMTYPE_GENERAL);
		aTabLcomType.Add(D_LCOMTYPE_STEEL);
		aTabLcomType.Add(D_LCOMTYPE_CONCRETE);
		aTabLcomType.Add(D_LCOMTYPE_SRC);
		aTabLcomType.Add(D_LCOMTYPE_STLCOMP);  
	#else
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Concrete_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__SRC_Design));  

		aTabLcomType.Add(D_LCOMTYPE_GENERAL);
		aTabLcomType.Add(D_LCOMTYPE_STEEL);
		aTabLcomType.Add(D_LCOMTYPE_CONCRETE);
		aTabLcomType.Add(D_LCOMTYPE_SRC);
	#endif
#elif defined(_TOWER)
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__General));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Steel_Design));
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Footing_Design));

		aTabLcomType.Add(D_LCOMTYPE_GENERAL);
		aTabLcomType.Add(D_LCOMTYPE_STEEL);
		aTabLcomType.Add(D_LCOMTYPE_FDN);
#endif
	}
	else if(m_nSeismicType == LCOM_TYPE::LCOM_SEISMIC)
	{
		aTabTitle.Add(_LS(IDS_WG_CMD__ADDD__Seismic));    

		aTabLcomType.Add(D_LCOMTYPE_SEISMIC);
	}
	else if(m_nSeismicType == LCOM_TYPE::LCOM_LINEAR)
	{
		aTabTitle.Add(CString(_LSX(Linear Eval.)));

		aTabLcomType.Add(D_LCOMTYPE_LINEAR_EVAL);
	}
	else
	{
		ASSERT(0);
	}
}

bool CCMLoadCombDlg::ConvertLcomTypeToString(const int nLcomType, CString& strDesignType) const
{
	switch (nLcomType)
	{
	case D_LCOMTYPE_GENERAL: strDesignType = _LS(IDS_WG_CMD__ADDD__General); break;
	case D_LCOMTYPE_STEEL: strDesignType = _LS(IDS_WG_CMD__ADDD__Steel_Design); break;
	case D_LCOMTYPE_CONCRETE: strDesignType = _LS(IDS_WG_CMD__ADDD__Concrete_Design); break;
	case D_LCOMTYPE_SRC: strDesignType = _LS(IDS_WG_CMD__ADDD__SRC_Design); break;
	case D_LCOMTYPE_FDN: strDesignType = _LS(IDS_WG_CMD__ADDD__Footing_Design); break;
	case D_LCOMTYPE_ALUMINUM: strDesignType = _LS(IDS_WG_CMD__ADDD__Aluminum_Design); break;
	case D_LCOMTYPE_STLCOMP: strDesignType = _LS(IDS_WG_CMD__ADDD__Steel_Composite_Design); break;
	case D_LCOMTYPE_CFSTEEL: strDesignType = _LS(IDS_WG_CMD__ADDD__CFSteel_Design); break;
	case D_LCOMTYPE_SEISMIC: strDesignType = _LS(IDS_WG_CMD__ADDD__Seismic); break;
	case D_LCOMTYPE_LINEAR_EVAL: strDesignType = _LSX(Linear Eval.); break;
	default: 
	{
		ASSERT(false);
		return false;
	}
	}
	return true;
}

bool CCMLoadCombDlg::ConvertToDesignType(const LOADCOMB_EXCEL::MString& strLcomType, int& nLcomType) const
{
	if (strLcomType == _LS(IDS_WG_CMD__ADDD__General)) nLcomType = D_LCOMTYPE_GENERAL;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__Steel_Design)) nLcomType = D_LCOMTYPE_STEEL;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__Concrete_Design)) nLcomType = D_LCOMTYPE_CONCRETE;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__SRC_Design)) nLcomType = D_LCOMTYPE_SRC;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__Footing_Design)) nLcomType = D_LCOMTYPE_FDN;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__Aluminum_Design)) nLcomType = D_LCOMTYPE_ALUMINUM;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__Steel_Composite_Design)) nLcomType = D_LCOMTYPE_STLCOMP;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__CFSteel_Design)) nLcomType = D_LCOMTYPE_CFSTEEL;
	else if (strLcomType == _LS(IDS_WG_CMD__ADDD__Seismic)) nLcomType = D_LCOMTYPE_SEISMIC;
	else if (strLcomType == _LSX(Linear Eval.)) nLcomType = D_LCOMTYPE_LINEAR_EVAL;
	else
	{
		ASSERT(false);
		return false;
	}
	return true;
}

int CCMLoadCombDlg::ConvertToLoadCombActive(const int nLcomType, const  LOADCOMB_EXCEL::MString& type) const
{
	const auto activeList = GetCombActiveStringLists(nLcomType);
	for(int idx = 0 ; idx < activeList.size(); ++idx)
	{
		if (type == activeList[idx])
			return idx;
	}
	return -1;
}

int CCMLoadCombDlg::ConvertToLoadCombType(const LOADCOMB_EXCEL::MString& type) const
{
	if (type == D_LCOM_CT_ADD)			return 0;
	if (type == D_LCOM_CT_ENVELOPE)		return 1;
	if (type == D_LCOM_CT_ABS)			return 2;
	if (type == D_LCOM_CT_SRSS)			return 3;
	return -1;
}

void CCMLoadCombDlg::SetTabWndText()
{  
	TC_ITEM item;
	memset(&item, 0, sizeof(TC_ITEM));
	item.mask = TCIF_TEXT;

	GetTabTitleAndType(m_aTabTitle, m_aTabLcomType);
	ASSERT(m_aTabTitle.GetSize() == m_aTabLcomType.GetSize());

	int nNum = m_aTabTitle.GetSize();  
	for (int i = 0; i < nNum; i++)
	{ 
		item.pszText = LPTSTR(LPCTSTR(m_aTabTitle[i]));
		m_wndTab.InsertItem(i, &item);
	}
	if(nNum > 0)
	{
		m_wndTab.SetCurSel(0);
	}
}
