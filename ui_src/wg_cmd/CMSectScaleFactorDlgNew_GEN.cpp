// CMSectScaleFactorDlgNew_GEN.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectScaleFactorDlgNew_GEN.h"

#include "CMBndrGrupDefDlg.h"
#include "CMSectScaleFactorGrid_GEN.h"
#include "CMSecPageStiffDlg.h"
#include "ExportFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

//#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorDlgNew_GEN dialog


CCMSectScaleFactorDlgNew_GEN::CCMSectScaleFactorDlgNew_GEN(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectScaleFactorDlgNew_GEN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectScaleFactorDlgNew_GEN)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_pGrid = new CCMSectScaleFactorGrid_GEN();

	EnableLayout();
}

CCMSectScaleFactorDlgNew_GEN::~CCMSectScaleFactorDlgNew_GEN()
{
	if (m_pGrid != 0)
	{
		delete m_pGrid;
		m_pGrid = 0;
	}
}

void CCMSectScaleFactorDlgNew_GEN::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectScaleFactorDlgNew_GEN)
	DDX_Control(pDX, IDC_CMD_BTN_STIFF, m_btnStif);
	DDX_Control(pDX, IDC_CMD_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_CMD_LIST_SECTION, m_listSect);
	DDX_Control(pDX, IDC_CMD_COBX_BDGR, m_cobxBdgr);
	//}}AFX_DATA_MAP
	UINT aCtrlEdit[7] = { IDC_CMD_EDIT_AREA, IDC_CMD_EDIT_ASY, IDC_CMD_EDIT_ASZ,
												IDC_CMD_EDIT_LXX , IDC_CMD_EDIT_LYY, IDC_CMD_EDIT_LZZ,
												IDC_CMD_EDIT_WEIGHT };

	for (int i = 0; i < 7; ++i) 
	{
		DDX_Control(pDX, aCtrlEdit[i], m_editStif[i]);
	}
	DDX_Control(pDX, IDC_CMD_BEFORE_CHK, m_chkBefore);
	DDX_Control(pDX, IDC_CMD_AFTER_CHK, m_chkAfter);
	DDX_Control(pDX, IDC_CMD_GRID_FRM, *m_pGrid);
}


BEGIN_MESSAGE_MAP(CCMSectScaleFactorDlgNew_GEN, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectScaleFactorDlgNew_GEN)
	ON_WM_SIZING()
	ON_WM_SIZE()
	//ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_STIFF, OnCmdBtnStiff)
	ON_BN_CLICKED(IDC_CMD_BTN_BDGR, OnCmdBtnBdgr)
	//ON_NOTIFY(NM_CLICK, IDC_CMD_LIST_SECTION, OnClickCmdListSection)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST_SECTION, OnClickCmdListSection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorDlgNew_GEN message handlers

BOOL CCMSectScaleFactorDlgNew_GEN::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SaveWindowSizeData();
	// TODO: Add extra initialization here
	SetBngrCombo();

	m_pGrid->Initialize(this);
	m_pGrid->MakeItemEx();

	SetHeaderTitle();
	MakeItemEx();

	T_SECF_D Data;
	Data.Initialize();
	for (int i = 0; i < 7; ++i) 
	{
		m_editStif[i].SetUnitType(D_UNITSYS_NONE);
		m_editStif[i].SetEditUnit(Data.dScaleFactor[i]);
	}

	m_btnAdd.EnableWindow(FALSE);
	m_btnStif.EnableWindow(FALSE);

	m_chkBefore.SetCheck(0);
	m_chkAfter.SetCheck(1); // default on
	m_chkBefore.EnableWindow(FALSE);
	m_chkAfter.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectScaleFactorDlgNew_GEN::OnCmdBtnAdd() 
{
	const int nCount = m_listSect.GetSelectedCount();
	if (nCount < 1) return;
	
	T_SECT_K key;
	T_SECF_D Data;
	Data.Initialize();

	// 공통 입력값을 Data 에 세팅한다.
	Data.GroupKey = GetSelectedBngr(); 
	for (int i = 0; i < 7; ++i) 
		Data.dScaleFactor[i] = m_editStif[i].GetEditValue();

	POSITION pos = m_listSect.GetFirstSelectedItemPosition();
	for (int i = 0; i < nCount; ++i)
	{
		int ix = m_listSect.GetNextSelectedItem(pos);
		key = m_listSect.GetItemData(ix); // 키값을 받아온다
		
		// Before, After 값을 세팅한다 - 3 가지 경우
		if (TypeByDefaultSecfValue(key)==0) // Composite Box, I
		{
			if (!m_chkBefore.IsWindowEnabled()) 
			{ 
				Data.bBefore = FALSE;  
				Data.bAfter = TRUE; 
			}
			else 
			{ 
				Data.bBefore = (m_chkBefore.GetCheck()==1); 
				Data.bAfter = (m_chkAfter.GetCheck()==1); 
				if (!Data.bBefore && !Data.bAfter) continue;
			}
		}
		else if (TypeByDefaultSecfValue(key)==1) // Other Composite
		{ 
			Data.bBefore = TRUE;  
			Data.bAfter = TRUE; 
		} 
		else  // Not Composite
		{ 
			Data.bBefore = TRUE;  
			Data.bAfter = FALSE; 
		} 

		if (!m_pDoc->m_pDataCtrl->AddSecf(key, Data)) continue;
	}
}

void CCMSectScaleFactorDlgNew_GEN::OnCmdBtnStiff() 
{
	T_SECF_K CurKey;
	if (!m_pGrid->GetCurrentSecf(CurKey))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Error___No_selected_section_scale_fact));
		return;
	}

	T_SECT_D SectData;
	T_SECF_D SecfData;
	if (!m_pDoc->m_pAttrCtrl->GetSect(CurKey.key.entity, SectData)) { ASSERT(0); return; }
	if (!m_pDoc->m_pAttrCtrl->GetSecf(CurKey, SecfData)) { ASSERT(0); return; }
	
	// CCMStiffDlg 창에서 Longterm, Shrinkage를 표시하지 않기 위하여
	SectData.SectBefore.bMultipleElastic = FALSE;
	SectData.SectAfter.bMultipleElastic = FALSE;
	
	if (SecfData.bBefore == TRUE)
	{
		ApplyScaleFactor(SectData.SectBefore.Stiffness, SecfData);
		ApplyScaleFactor(SectData.SectBefore.SectI.Stiffness, SecfData);
		ApplyScaleFactor(SectData.SectBefore.SectJ.Stiffness, SecfData);
	}
	if (SecfData.bAfter == TRUE)
	{
		ApplyScaleFactor(SectData.SectAfter.Stiffness, SecfData);
		ApplyScaleFactor(SectData.SectAfter.SectI.Stiffness, SecfData);
		ApplyScaleFactor(SectData.SectAfter.SectJ.Stiffness, SecfData);
	}

	CCMStiffDlg dlg(m_pDoc, &SectData, NULL, this);
	dlg.DoModal();	
}

void CCMSectScaleFactorDlgNew_GEN::OnCmdBtnBdgr() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD,this);
}

void CCMSectScaleFactorDlgNew_GEN::OnClickCmdListSection(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_btnAdd.EnableWindow(FALSE);
	m_chkBefore.EnableWindow(FALSE); 
	m_chkAfter.EnableWindow(FALSE);
	
	T_SECT_K SectKey;
	BOOL bAllCompositeBoxOrI = TRUE;
	BOOL bAllNonComposite = TRUE;
	POSITION pos = m_listSect.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	else 
	{ 
		while (pos)
		{
			SectKey = m_listSect.GetItemData(m_listSect.GetNextSelectedItem(pos));
			if (!SectKey) return; 
			else 
			{
				bAllCompositeBoxOrI = bAllCompositeBoxOrI && (TypeByDefaultSecfValue(SectKey)==0);
				bAllNonComposite = bAllNonComposite && (TypeByDefaultSecfValue(SectKey)==2);
			}
		}
	}
	// 전부 Composite Box 나 I 인 경우만 Enable
	if (bAllCompositeBoxOrI)
	{
		m_chkBefore.EnableWindow(TRUE);   
		m_chkAfter.EnableWindow(TRUE); 
		m_chkBefore.SetCheck(FALSE); 
		m_chkAfter.SetCheck(TRUE);
	}
	else if (bAllNonComposite)
	{
		m_chkBefore.SetCheck(TRUE); m_chkAfter.SetCheck(FALSE);
	}
	else
	{
		m_chkBefore.SetCheck(TRUE); m_chkAfter.SetCheck(TRUE);
	}
	m_btnAdd.EnableWindow(TRUE);

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
void CCMSectScaleFactorDlgNew_GEN::OnChangeCurrentSecf(T_SECF_K key)
{
	// Enable/disable show stiffness button
	BOOL bEnable = (key.keymap != 0);
	m_btnStif.EnableWindow(bEnable);
	if (!bEnable) return;

	// Show data
	T_SECF_D data;
	if (!m_pDoc->m_pAttrCtrl->GetSecf(key, data)) { ASSERT(0); return; }
	ChangeSelect(data.GroupKey);
	for (int i = 0; i < 7; ++i)
		m_editStif[i].SetEditUnit(data.dScaleFactor[i]);

	// Select section list
	int nCount = m_listSect.GetItemCount();
	T_SECT_K SectKey;
	for (int i = 0; i < nCount; ++i)
	{
		SectKey = m_listSect.GetItemData(i);
		if (SectKey == key.key.entity)
			m_listSect.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		else
			m_listSect.SetItemState(i, 0,  LVIS_SELECTED | LVIS_FOCUSED);
	}

	// Enable/Disable Buttons - OnClickCmdListSection 함수와 같은 동작 하도록
	if (TypeByDefaultSecfValue(key)==0)
	{
		m_chkBefore.EnableWindow(TRUE);     m_chkAfter.EnableWindow(TRUE);
		m_chkBefore.SetCheck(data.bBefore); m_chkAfter.SetCheck(data.bAfter);
	}
	else if (TypeByDefaultSecfValue(key)==1)
	{
		m_chkBefore.EnableWindow(FALSE);      m_chkAfter.EnableWindow(FALSE);
		m_chkBefore.SetCheck(TRUE);           m_chkAfter.SetCheck(TRUE);
	}
	else // (TypeByDefaultSecfValue(key)==2)
	{
		m_chkBefore.EnableWindow(FALSE);      m_chkAfter.EnableWindow(FALSE);
		m_chkBefore.SetCheck(TRUE);           m_chkAfter.SetCheck(FALSE);
	}
	m_btnAdd.EnableWindow(TRUE);
}

void CCMSectScaleFactorDlgNew_GEN::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Name)};
	int nColWidth[2];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_listSect.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_listSect.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 30;
	nColWidth[1] = 279;

	// Set Title
	for(i=0; i < 2; ++i)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_listSect.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMSectScaleFactorDlgNew_GEN::MakeItemEx(CArray<T_SECT_K, T_SECT_K>* paKey/*=NULL*/)
{
	CWaitCursor Cursor;
	m_listSect.DeleteAllItems();

	CArray<T_SECT_K, T_SECT_K> aKey;
	if (paKey == 0)
	{
		m_pDoc->m_pAttrCtrl->GetSectKeyList(aKey);
	}
	else aKey.Copy(*paKey);
	int nCount = aKey.GetSize();
	if (nCount < 1) return;

	T_SECT_D Data;

	for (int i = 0; i < nCount; ++i)
	{
		if (!m_pDoc->m_pAttrCtrl->GetSect(aKey[i], Data)) continue;
		InsertItem(aKey[i], Data, NULL);
	}
}

BOOL CCMSectScaleFactorDlgNew_GEN::InsertItem(T_SECT_K Key, T_SECT_D& Data, T_SECF_D* WorstSecfData)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_listSect.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 2; ++i)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data);

		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_listSect.InsertItem(&lvitem);
		}
		else m_listSect.SetItem(&lvitem);

		str.ReleaseBuffer();
	}
	m_listSect.SetItemData(nItem, (DWORD)Key);
	
	return TRUE;
}

int CCMSectScaleFactorDlgNew_GEN::FindInsertionPos(T_SECT_K nID)
{
	T_SECT_K key;

	int nCount = m_listSect.GetItemCount();
	int i = 0;
	for (i=0; i < nCount; ++i)
	{
		key = m_listSect.GetItemData(i);
		if (nID < key) break;
	}
	return i;
}

CString CCMSectScaleFactorDlgNew_GEN::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data)
{
	CString str;
	if (i == 0) str.Format(_T("%d"), Key);
	else str = Data.SName;

	return str;
}

void CCMSectScaleFactorDlgNew_GEN::ApplyScaleFactor(T_SECT_STIFFNESS& Stiff, T_SECF_D& Secf)
{
	Stiff.Area *= Secf.dScaleFactor[0];
	Stiff.Asy  *= Secf.dScaleFactor[1];
	Stiff.Asz  *= Secf.dScaleFactor[2];
	Stiff.Rxx  *= Secf.dScaleFactor[3];
	Stiff.Ryy  *= Secf.dScaleFactor[4];
	Stiff.Rzz  *= Secf.dScaleFactor[5];
	Stiff.WArea*= Secf.dScaleFactor[6];
}

void CCMSectScaleFactorDlgNew_GEN::SetBngrCombo()
{
	m_cobxBdgr.ResetContent();
	m_cobxBdgr.SetItemData(m_cobxBdgr.AddString(_LS(IDS_DB_GROUP_NAME_DEFAULT)), D_BNGR_KEY_DEFAULT);
	m_cobxBdgr.SetCurSel(0);

	CArray<T_BNGR_K, T_BNGR_K> aKey;
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(aKey);
	int nCount = aKey.GetSize();
	if (nCount < 1) return;

	T_BNGR_D Data;
	for (int i = 0; i < nCount; ++i)
	{
		if (!m_pDoc->m_pAttrCtrl->GetBngr(aKey[i], Data)) continue;
		int ix = m_cobxBdgr.AddString(Data.GroupName);
		m_cobxBdgr.SetItemData(ix, aKey[i]);
	}
}

T_BNGR_K CCMSectScaleFactorDlgNew_GEN::GetSelectedBngr()
{
	int ix = m_cobxBdgr.GetCurSel();
	if (ix == CB_ERR) return 0;
	return m_cobxBdgr.GetItemData(ix);
}

void CCMSectScaleFactorDlgNew_GEN::ChangeSelect(T_BNGR_K Key)
{
	int nCount = m_cobxBdgr.GetCount();
	for (int i = 0; i < nCount; ++i)
		if (Key == m_cobxBdgr.GetItemData(i))
			m_cobxBdgr.SetCurSel(i);
}

int CCMSectScaleFactorDlgNew_GEN::TypeByDefaultSecfValue(T_SECF_K key)
{
	return TypeByDefaultSecfValue(key.key.entity);
}

// Composite Box, I / 기타 Composite / Non Composite의 세 타입으로 나눔
int CCMSectScaleFactorDlgNew_GEN::TypeByDefaultSecfValue(T_SECT_K key)
{
	T_SECT_D SectData;
	if (!m_pDoc->m_pAttrCtrl->GetSect(key, SectData)) { ASSERT(0); return FALSE; }
	int nStype = SectData.nStype;
	if (nStype==D_SECT_TYPE_COMPO_B      || nStype==D_SECT_TYPE_COMPO_I      || nStype==D_SECT_TYPE_COMPO_TUB ||
		  nStype==D_SECT_TYPE_COMPO_STLG_B || nStype==D_SECT_TYPE_COMPO_STLG_I || nStype==D_SECT_TYPE_COMPO_STLG_TUB) 
		return 0; // Composite Box, I
	else if (nStype==D_SECT_TYPE_COMPO_CI || nStype==D_SECT_TYPE_COMPO_CT || nStype==D_SECT_TYPE_COMPO_PC) 
		return 1; // Composite Others
	else 
		return 2; // Not Composite
}

void CCMSectScaleFactorDlgNew_GEN::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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

void CCMSectScaleFactorDlgNew_GEN::UpdateBuffer()
{
	int nModified = 0;

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bOldLock = m_pGrid->LockUpdate(TRUE);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChangeBngr = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_BNGR_ADD):
		case(UR_BNGR_DEL):
		case(UR_BNGR_MFD):
		case(UR_BNGR_MFS):
			bChangeBngr = TRUE;
			break;
		}
	}
	if (bChangeBngr)
	{
		ROWCOL nCol;
		m_pGrid->GetColumnNoFor(11, nCol);
		m_pGrid->SetBngrColumn(nCol);

		SetBngrCombo();
	}

	pos = pViewBuff->GetStartBuffer();

	T_SECF_K key; 
	T_SECF_D data;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SECF_ADD):
				pViewBuff->GetSecf(nKey, key, data);
				// 삽입될 위치(nRow)를 찾아 넣는다.
				m_pGrid->InsertRecordByKey(key, data);
				nModified |= 0x01;
				break;
		case(UR_SECF_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetSecf(nKey, key, data);
				m_pGrid->DeleteRecordByKey(key, data);
				nModified |= 0x02;
				break;
		default:
			break;
		}
	} // end of while

	if (nModified) m_pGrid->ShowSelected();
	m_pGrid->AdjustAppendRowNo();

	if (bChangeBngr) 
		m_pGrid->MakeItemEx(0, -2, FALSE);

	m_pGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_pGrid->Redraw();
}


// Size 조절
void CCMSectScaleFactorDlgNew_GEN::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CRect r(pRect);
	if (r.Width() != m_MinimumRect.Width())
	{
		if (fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_BOTTOMLEFT)
			pRect->left = pRect->right - m_MinimumRect.Width();
		else
			pRect->right = pRect->left + m_MinimumRect.Width();
	}
	if (r.Height() < m_MinimumRect.Height())
	{
		if (fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT)
			pRect->top = pRect->bottom - m_MinimumRect.Height();
		else
			pRect->bottom = pRect->top + m_MinimumRect.Height();
	}
	CDialogMove::OnSizing(fwSide, pRect);
}

void CCMSectScaleFactorDlgNew_GEN::OnSize(UINT nType, int cx, int cy) 
{
	CDialogMove::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	CButton* pBtn = (CButton*)GetDlgItem(IDCANCEL);
	if (pBtn == 0 || pBtn->GetSafeHwnd() == 0 ||
			!IsWindow(pBtn->GetSafeHwnd())) return;
	CRect r;
	GetWindowRect(r);
	ArrangeControls(r);
}

void CCMSectScaleFactorDlgNew_GEN::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);

	rc.left = rc.right - ::GetSystemMetrics(SM_CXHSCROLL);
	rc.top = rc.bottom - ::GetSystemMetrics(SM_CYVSCROLL);

	dc.DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	// Do not call CDialogMove::OnPaint() for painting messages
}

void CCMSectScaleFactorDlgNew_GEN::SaveWindowSizeData()
{
	// Save default width as minimum rectangle
	GetWindowRect(m_MinimumRect);
	m_nDlgHeight = m_MinimumRect.Height();

	CRect r;
	GetDlgItem(IDC_CMD_GRID_FRM)->GetWindowRect(r);
	m_nGridFrmHeight = r.Height();
}

void CCMSectScaleFactorDlgNew_GEN::ArrangeControls(LPRECT pRect)
{
	int nNewHeight = pRect->bottom - pRect->top;
	int nHeightDiff = m_nDlgHeight - nNewHeight;

	CRect r;
	// Tree
	CWnd* pWnd = GetDlgItem(IDC_CMD_GRID_FRM);
	pWnd->GetWindowRect(r);
	r.bottom = (r.top + m_nGridFrmHeight) - nHeightDiff;
	ScreenToClient(r);
	pWnd->MoveWindow(r, FALSE);

	pWnd = GetDlgItem(IDC_CMD_BTN_STIFF);
	pWnd->GetWindowRect(r);
	int nHeight = r.Height();
	r.bottom = pRect->bottom - 15;
	r.top = r.bottom - nHeight;
	ScreenToClient(r);
	pWnd->MoveWindow(r, FALSE);

	pWnd = GetDlgItem(IDCANCEL);
	pWnd->GetWindowRect(r);
	nHeight = r.Height();
	r.bottom = pRect->bottom - 15;
	r.top = r.bottom - nHeight;
	ScreenToClient(r);
	pWnd->MoveWindow(r, FALSE);

	Invalidate();
	UpdateWindow();
}
