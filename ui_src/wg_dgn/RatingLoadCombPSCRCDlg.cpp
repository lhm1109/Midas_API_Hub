// RatingLoadCombPSCRCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"

#include "../wg_db/ViewBuff.h"
#include "../wg_db/DataCtrl.h"
#include "../wg_db/AttrCtrl2.h"
#include "../wg_db/AttrCtrl.h"

#include "RatingLoadCombPSCRCDlg.h"
#include "RatingLoadCombPSCRCMainGrid.h"
#include "RatingLoadCombPSCRCSubGrid.h"
#include "RatingLoadCombUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombPSCRCDlg dialog
CRatingLoadCombPSCRCDlg::CRatingLoadCombPSCRCDlg(CWnd* pParent )
	: CRatingLoadCombCommonDlg(CRatingLoadCombPSCRCDlg::IDD, pParent)
{
	// *^^*  여기를 바꾸시오.

	//{{AFX_DATA_INIT(CRatingLoadCombPSCRCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pLoadCombList = new CRatingLoadCombPSCRCMainGrid;

	m_pContentList = new CRatingLoadCombPSCRCSubGrid(0);
	m_pContentListMV = new CRatingLoadCombPSCRCSubGrid(1);
}

CRatingLoadCombPSCRCDlg::~CRatingLoadCombPSCRCDlg()
{
	if (m_pLoadCombList) delete m_pLoadCombList;
	if (m_pContentList) delete m_pContentList;
	if (m_pContentListMV) delete m_pContentListMV;	
}

void CRatingLoadCombPSCRCDlg::DoDataExchange(CDataExchange* pDX)
{
	CRatingLoadCombCommonDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingLoadCombPSCRCDlg)


	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRatingLoadCombPSCRCDlg, CRatingLoadCombCommonDlg)
	//{{AFX_MSG_MAP(CRatingLoadCombPSCRCDlg)
	ON_BN_CLICKED(IDC_BTN_COPY_TO_LCOM, OnCopyToLCom)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombPSCRCDlg message handlers
BOOL CRatingLoadCombPSCRCDlg::OnInitDialog() 
{
	CRatingLoadCombCommonDlg::OnInitDialog();

	if(m_nDlgType==1) // Rc rating
	{
		this->SetWindowText(_T("Define Rating Case(RC)"));	// 종방향(X)간격
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRatingLoadCombPSCRCDlg::UpdateBuffer()
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

	T_RKLC_K    Key;
	T_RKLC_D    Data;

	BOOL bOldLock = m_pLoadCombList->LockUpdate(TRUE);	
	
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		if (nCmd != UR_RKLC_ADD && nCmd != UR_RKLC_DEL) continue;

		switch(nCmd)
		{
		case(UR_RKLC_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				 pViewBuff->GetRklc(nKey, Key, Data);
				 ((CRatingLoadCombPSCRCMainGrid*)m_pLoadCombList)->InsertRecordByKey(Key, Data);
				break;
		case(UR_RKLC_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRklc(nKey, Key, Data);
				((CRatingLoadCombPSCRCMainGrid*)m_pLoadCombList)->DeleteRecordByKey(Key, Data);
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

void CRatingLoadCombPSCRCDlg::OnCopyToLCom()
{
	if(!m_pLoadCombList->OnTerminate(D_TB_SAVE_ALWAYS))
	{
		return;
	}

	CRatingLoadCombUtil::CopyPSCRatingToLCom(m_pDoc);
}