// CMLoadCaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCaseDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_LoadDB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseDlg dialog


CCMLoadCaseDlg::CCMLoadCaseDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLoadCaseDlg::IDD, pParent)
{
	m_wndGrid = new CCMLoadCaseGrid;
	//{{AFX_DATA_INIT(CCMLoadCaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMLoadCaseDlg::~CCMLoadCaseDlg()
{
	if (m_wndGrid) delete m_wndGrid;
}

void CCMLoadCaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCaseDlg)
	DDX_Control(pDX, IDC_CMD_LCD_DESC,     m_wndDesc);
	DDX_Control(pDX, IDC_CMD_LCD_CASE_CMB, m_cmbCase);
	DDX_Control(pDX, IDC_CMD_LCD_TYPE,     m_wndType);
	DDX_Control(pDX, IDC_CMD_LCD_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_LCD_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}

void CCMLoadCaseDlg::InitCaseCombo()
{
	m_cmbCase.ResetContent();
	m_cmbCase.SetItemData(m_cmbCase.AddString(_LS(IDS_CMD_LOAD_CASE_ALL_CIVIL)),          0);
	m_cmbCase.SetItemData(m_cmbCase.AddString(_LS(IDS_CMD_LOAD_CASE_PERMANENT_CIVIL)),    1);
	m_cmbCase.SetItemData(m_cmbCase.AddString(_LS(IDS_CMD_LOAD_CASE_TRANSIENT_CIVIL)),    2);
	m_cmbCase.SetCurSel(0);
}

void CCMLoadCaseDlg::InitTypeCombo()
{
	CArray<CString, CString&> LoadType;
	m_pDoc->m_pLoadDB->GetLoadTypeList(&LoadType);
	VERIFY(LoadType.GetSize() > 0);

	m_wndType.ResetContent();
	m_aStldType.RemoveAll();

	int nSize = LoadType.GetSize();
	int nCase = m_cmbCase.GetItemData(m_cmbCase.GetCurSel());
	int nLoadCaseType = 0;

	CDC* pDC = GetDC();  
	CSize max, cur;
	max = pDC->GetTextExtent(m_pDoc->m_pLoadDB->GetLoadTypeName(LoadType.GetAt(0)));
	for(int i = 0; i < nSize; i++)
	{
#if defined(_CIVIL)
		if(!m_pDoc->m_pLoadDB->IsPossibleGetLoadType(nCase, LoadType.GetAt(i))) continue;
#endif
		m_aStldType.Add(LoadType.GetAt(i));
		m_wndType.AddString(m_pDoc->m_pLoadDB->GetLoadTypeName(LoadType.GetAt(i)));
		cur = pDC->GetTextExtent(m_pDoc->m_pLoadDB->GetLoadTypeName(LoadType.GetAt(i))); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if (max.cx > m_wndType.GetDroppedWidth()) m_wndType.SetDroppedWidth(max.cx);

	ReleaseDC(pDC);	
}

void CCMLoadCaseDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;

	CArray<UINT, UINT> aMoveCtrl; aMoveCtrl.RemoveAll();
	aMoveCtrl.Add(IDC_CMD_LCD_GEN_GRP);

	GetDlgItem(IDC_CMD_LCD_CVL_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LCD_GEN_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);
#if defined(_CIVIL)
	// Nothing
#else
	aMoveCtrl.RemoveAll();
	aMoveCtrl.Add(IDC_CMD_LCD_TYPE_STC);
	aMoveCtrl.Add(IDC_CMD_LCD_TYPE);
	aMoveCtrl.Add(IDC_CMD_LCD_DESC_STC);
	aMoveCtrl.Add(IDC_CMD_LCD_DESC);
	aMoveCtrl.Add(IDC_CMD_LCD_GRID);
	aMoveCtrl.Add(IDC_CMD_BTN_CLOSE);

	GetDlgItem(IDC_CMD_LCD_CASE_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_LCD_TYPE_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aMoveCtrl, nDistY);
#endif
	// 윈도우 사이즈를 맞춘다.
	CWnd* pWndLast = GetDlgItem(IDC_CMD_BTN_CLOSE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMLoadCaseDlg::ShowHideControls()
{
#if defined(_CIVIL)
	GetDlgItem(IDC_CMD_LCD_GEN_GRP)->ShowWindow(SW_HIDE);
#else
	GetDlgItem(IDC_CMD_LCD_CVL_GRP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_LCD_CASE_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_LCD_CASE_CMB)->ShowWindow(SW_HIDE);
#endif
}

BOOL CCMLoadCaseDlg::Data2Dlg()
{
	int nSize = m_aStldType.GetSize();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		if (m_Data.LoadCaseType == m_aStldType[i]) break;
	}

#if defined(_CIVIL)
	int j=0;
	BOOL bUseJ = FALSE;
	if (i == nSize)
	{
		int nCase = m_cmbCase.GetItemData(m_cmbCase.GetCurSel());
		if      (nCase == 1) nCase = 2;
		else if (nCase == 2) nCase = 1;  
		m_cmbCase.SetCurSel(nCase);
		InitTypeCombo();
		int nSize2 = m_aStldType.GetSize();
		for (j = 0; j < nSize2; j++)
		{
			bUseJ = TRUE;
			if (m_Data.LoadCaseType == m_aStldType[j]) break;
		}
		if (j == nSize2)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_case_type__s)
				, m_pDoc->m_pLoadDB->GetLoadTypeNameAbbreviation(m_Data.LoadCaseType));
			return FALSE;
		}
	}

	if(bUseJ) m_wndType.SetCurSel(j);
	else      m_wndType.SetCurSel(i);
#else
	if (i == nSize)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_case_type__s)
			, m_pDoc->m_pLoadDB->GetLoadTypeNameAbbreviation(m_Data.LoadCaseType));
		return FALSE;
	}

	m_wndType.SetCurSel(i);
#endif

	m_wndName.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);

	return TRUE;
}

BOOL CCMLoadCaseDlg::Dlg2Data()
{
	m_Data.Initialize();
	m_wndName.GetWindowText(m_Data.LoadCaseName);
	int nSel = m_wndType.GetCurSel();
	if (nSel < 0) return FALSE;
	m_Data.LoadCaseType = m_aStldType.GetAt(nSel);
	m_wndDesc.GetWindowText(m_Data.Description);
	return TRUE;
}

#define CDialog CCMDlgBase
BEGIN_MESSAGE_MAP(CCMLoadCaseDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMLoadCaseDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnDlgClose)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CMD_LCD_ADD,    OnCmdLcdAdd)
	ON_BN_CLICKED(IDC_CMD_LCD_MODIFY, OnCmdLcdModify)
	ON_BN_CLICKED(IDC_CMD_LCD_DELETE, OnCmdLcdDelete)
	ON_CBN_SELCHANGE(IDC_CMD_LCD_CASE_CMB, OnCmdLcdCaseCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseDlg message handlers

BOOL CCMLoadCaseDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid->Initialize(m_pDoc, this);
	
	InitCaseCombo();
	InitTypeCombo();

	AlignControl();
	ShowHideControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadCaseDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMLoadCaseDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMLoadCaseDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_STLD_K cKey;
	m_wndGrid->GetCurrentCell(ncRow, ncCol);
	m_wndGrid->GetColumnNoFor(m_wndGrid->GetKeyColID(), nkCol);
	cKey = _tstol(m_wndGrid->GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K    Key, KeyBak;
	T_STLD_D    Data, DataBak;

	BOOL bOldLock = m_wndGrid->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetStld(nKey, Key, Data);
				m_wndGrid->InsertRecordByKey(Key, Data);
				break;
		case(UR_STLD_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetStld(nKey, Key, Data);
				m_wndGrid->DeleteRecordByKey(Key, Data);
				break;
		case(UR_STLD_MFD):
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetStld(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_STLD_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetStld(nKey, Key, Data);
				m_wndGrid->DeleteRecordByKey(Key, Data);
				m_wndGrid->InsertRecordByKey(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_wndGrid->m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_wndGrid->m_nEditMode = CTBBrowserWnd::noMode;
		if (m_wndGrid->GetRowCount() < ncRow) ncRow = m_wndGrid->GetRowCount();
		BOOL bSet = m_wndGrid->SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = m_wndGrid->SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = m_wndGrid->SetLeftValidCell(ncRow, ncCol);
		m_wndGrid->m_nEditMode = nOldMode;
	}

	m_wndGrid->GetCurrentCell(ncRow, ncCol);
	m_wndGrid->GetColumnNoFor(m_wndGrid->GetKeyColID(), nkCol);
	T_STLD_K newKey = _tstol(m_wndGrid->GetValueRowCol(ncRow, nkCol));
	if (newKey != cKey) m_wndGrid->OnChangeCurrentRecord(ncRow);

	m_wndGrid->AdjustAppendRowNo();
	m_wndGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid->Redraw();
}

// Add, Modify, Del시 에러 메세지가 떴을 때 이동하면 화면이 
// 다시 그려지도록 하기 위해 LockUpdate를 끄고 다시 그린 후
// LockUpdate를 원상복구한다.
void CCMLoadCaseDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	BOOL bOldLock = m_wndGrid->LockUpdate(FALSE);
	m_wndGrid->Redraw();
	m_wndGrid->LockUpdate(bOldLock);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CCMLoadCaseDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS);
	return CCMDlgBase::DestroyWindow();
}

void CCMLoadCaseDlg::OnCmdLcdAdd() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Input_data_error_));
		return;
	}
	CString csName = m_Data.LoadCaseName;

	CStringArray aNewNames;
	if(IsMultiInput(csName, aNewNames)) // 여러개 한꺼번에 입력 ( /1-10/ 의 형태 )
	{
		CArray<T_STLD_D, T_STLD_D&> rData;
		for(int i=0 ; i<aNewNames.GetSize() ; ++i)
		{
			rData.Add(m_Data);
			rData[i].LoadCaseName = aNewNames[i];
		}
		m_pDoc->m_pDataCtrl->AddStld(rData);
	}
	else // 기존
	{

	if (m_pDoc->m_pDataCtrl->AddStld(m_Data))
	{
		T_STLD_K key;
		T_STLD_D data;
		key = m_pDoc->m_pAttrCtrl->GetStldKey(csName);
		m_pDoc->m_pAttrCtrl->GetStld(key, data);
		m_wndGrid->SetCurrentStld(key, data);
	}
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_CMD_LCD_NAME);
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);

	}
}

void CCMLoadCaseDlg::OnCmdLcdModify() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data()) return;
	CString csNewName = m_Data.LoadCaseName;
	CStringArray aNewNames;
	T_STLD_D NewStldD, OldStld;
	if(IsMultiInput(csNewName, aNewNames)) // 여러개 한꺼번에 입력 ( %1-10% 의 형태 )
	{
		CArray<CString, CString&> rstrOldLoadCaseName;
		CArray<T_STLD_D, T_STLD_D&> rDataNew;
		for(int i=0 ; i<aNewNames.GetSize() ; ++i)
		{
			if(m_pDoc->m_pAttrCtrl->GetStld(aNewNames[i], OldStld) && OldStld.LoadCaseName==aNewNames[i])
			{
				NewStldD = m_Data;
				NewStldD.LoadCaseName = aNewNames[i];
				NewStldD.LoadCaseId = OldStld.LoadCaseId;

				rstrOldLoadCaseName.Add(aNewNames[i]);
				rDataNew.Add(NewStldD);
				//m_pDoc->m_pDataCtrl->ModifyStld(aNewNames[i], NewStldD);
			}
		}
		if(rstrOldLoadCaseName.GetSize() > 0)
			m_pDoc->m_pDataCtrl->ModifyStld(rstrOldLoadCaseName, rDataNew);
	}
	else // 기존
	{


	T_STLD_K key;
	T_STLD_D data;
	if (!m_wndGrid->GetCurrentStld(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_static_load_c));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetStld(key, data));
	CString csOldName = data.LoadCaseName;
	if (!Dlg2Data()) return;
	m_Data.LoadCaseId = data.LoadCaseId;
	m_pDoc->m_pDataCtrl->ModifyStld(csOldName, m_Data);	

	}
}

void CCMLoadCaseDlg::OnCmdLcdDelete() 
{
	// TODO: Add your control notification handler code here
	CRowColArray awRows;
	m_wndGrid->GetSelectedRows(awRows, TRUE, FALSE);
	int nSize = awRows.GetSize();
	if (nSize > 0)
	{
		m_wndGrid->DeleteFromDB(awRows);
		return;
	}
	ROWCOL nRow, nCol;
	m_wndGrid->GetCurrentCell(nRow, nCol);
	if (m_wndGrid->IsAppendRow(nRow)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_static_load_c));
		return;
	}
	m_wndGrid->DeleteFromDB(nRow);
}

void CCMLoadCaseDlg::OnChangeCurrentStld(T_STLD_K key)
{
	m_pDoc->m_pAttrCtrl->GetStld(key, m_Data);
	Data2Dlg();
}

// /2-5/ 의 형태가 존재하는지 판별해서 존재하면 생성된 이름들을 rNames 에 넣어주는 함수
BOOL CCMLoadCaseDlg::IsMultiInput(const CString &csName, CStringArray &rNames)
{
	rNames.RemoveAll();

	int nIdx = 0;
	CArray<int, int> aIdx;
	for(int i=0 ; i<100 ; ++i)
	{
		nIdx = csName.Find(_T("/"), nIdx);
		if(nIdx==-1) break;
		else
		{
			aIdx.Add(nIdx);
			++nIdx;
		}
	}
	if(aIdx.GetSize() <=1) return FALSE;

	for(int i=0 ; i<aIdx.GetSize()-1 ; ++i)
	{
		int nStart = aIdx[i];
		int nEnd = aIdx[i+1];
		if(nEnd-nStart <= 3) continue;
		int nSepIdx = csName.Find(_T("-"), nStart+1);
		if(nSepIdx <= nStart+1 || nSepIdx >= nEnd-1) continue;

		CString s1 = csName.Mid(nStart+1, nSepIdx-nStart-1);
		CString s2 = csName.Mid(nSepIdx+1, nEnd-nSepIdx-1);

		int n1 = _ttoi(s1);
		int n2 = _ttoi(s2);
		if(n1==n2) continue;
		if(n1 > n2) {int nTmp = n1; n1=n2; n2=nTmp;}
		ASSERT(n2>n1);
		CString str;
		for(int j=n1 ; j<=n2 ; ++j)
		{
			str.Format(_T("%s%d%s"), csName.Left(nStart), j, csName.Mid(nEnd+1));
			rNames.Add(str);
		}
		if(rNames.GetSize() > 0) return TRUE;
	}

	return FALSE;
}

void CCMLoadCaseDlg::OnCmdLcdCaseCmb()
{
	InitTypeCombo();
}

CGXGridWnd* CCMLoadCaseDlg::MakeVirtualTB(UINT UtblK)
{
	ASSERT(UtblK>0);
	
	// Show Wait Cursor.
	CWaitCursor WaitCursor;
	
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	
	T_UTBL_D  UtblD;
	if(!m_pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
	{ ASSERT(0); return FALSE; }
	
	// 일단 모든 열을 hide시키고, 정렬정보에 있는 열만 해제
	m_wndGrid->HideColsByIntend(1,5,TRUE);
	for(int i=0; i<UtblD.header.nCurColCount; i++)
	{
		int nCol = UtblD.header.aCurIndex[i] + 1;
		m_wndGrid->HideColsByIntend(nCol, nCol, FALSE);
	}
	
	return m_wndGrid;
}