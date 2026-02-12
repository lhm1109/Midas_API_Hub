#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdLcomSP20LinksDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "../wg_base/DlgUtil.h"
#include "../wg_common/wg_common.h"
#include "CmdLcomSP20RelationshipTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CArray<T_SP20_LINK_D, T_SP20_LINK_D&>* g_LinkData = 0;
CMap<CString, LPCTSTR, int, int>* g_mapName2Index = 0;

CCmdLcomSP20LinksDlg::CCmdLcomSP20LinksDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdLcomSP20LinksDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdLcomSP20LinksDlg)
	//}}AFX_DATA_INIT
	m_pLcomPsiFactorGrid = new CCmdLcomSP20LinksGrid;

	CLoadCombSP20Ctrl::Instance()->GenerateLoadCaseLink(&m_aLcaseSP20LinkData);
	for (int i = 0; i < m_aLcaseSP20LinkData.GetSize(); i++)
	{
		m_mapName2Index.SetAt(m_aLcaseSP20LinkData[i].LoadCaseName, m_aLcaseSP20LinkData[i].nIndex);
	}
	g_LinkData = &m_aLcaseSP20LinkData;
	g_mapName2Index = &m_mapName2Index;
	m_pDoc = NULL;
}

CCmdLcomSP20LinksDlg::~CCmdLcomSP20LinksDlg()
{
	if (m_pLcomPsiFactorGrid) delete m_pLcomPsiFactorGrid;
	g_LinkData = 0;
	g_mapName2Index = 0;
}

void CCmdLcomSP20LinksDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdLcomSP20LinksDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdLcomSP20LinksDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdLcomSP20LinksDlg)
	ON_BN_CLICKED(IDC_CMD_TEST_BUTTON, OnCmdTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCmdLcomSP20LinksDlg::OnCmdTest()
{
	CCmdLcomSP20TestPathDlg dlg;
	dlg.SetData(m_aLcaseSP20LinkData);
	dlg.DoModal();
}

BOOL CCmdLcomSP20LinksDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();

	CDialogMove::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);
	int nAugmentWidth = globalUtils.ScaleByDPI(400);
	int nAugmentHeight = globalUtils.ScaleByDPI(300);
	int newWidth = rect.Width() + nAugmentWidth;
	int newHeight = rect.Height() + nAugmentHeight;
	SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->GetWindowRect(&rect);
	newWidth = rect.Width() + nAugmentWidth;
	newHeight = rect.Height() + nAugmentHeight;
	GetDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID)->SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&rect);
	newWidth = rect.Width() + nAugmentWidth;
	newHeight = rect.Height() + nAugmentHeight;
	GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowPos(NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);

	CArray<UINT, UINT> aMoveCtrl;
	aMoveCtrl.Add(IDOK);
	aMoveCtrl.Add(IDCANCEL);
	aMoveCtrl.Add(IDC_CMD_TEST_BUTTON);
	CDlgUtil::CtrlMoveDistXY(this, aMoveCtrl, nAugmentWidth, nAugmentHeight);

	GetDlgItem(IDC_CMD_TEST_BUTTON)->ShowWindow(SW_HIDE);

	m_pLcomPsiFactorGrid->SubclassDlgItem(IDC_CMD_LCOM_PSI_FACTOR_GRID, this);
	T_UNIT_SYSTEM UnitSystem;
	m_pLcomPsiFactorGrid->SetData(&m_aLcaseSP20LinkData);
	m_pLcomPsiFactorGrid->Initialize(m_pDoc, UnitSystem);

	return TRUE;
}

void CCmdLcomSP20LinksDlg::OnOK() 
{
	m_pLcomPsiFactorGrid->OnTerminate(D_TB_SAVE_ALWAYS);

	CLoadCombSP20Ctrl::Instance()->SetLoadCaseLink(m_aLcaseSP20LinkData);
	CDialogMove::OnOK();
}

void CCmdLcomSP20LinksDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTBCheckLoadCaseListComboLBox, CGXCheckListComboLBox)
CTBCheckLoadCaseListComboLBox::CTBCheckLoadCaseListComboLBox(CWnd* pMsgWnd)
	: CGXCheckListComboLBox(pMsgWnd)
{
}
CTBCheckLoadCaseListComboLBox::~CTBCheckLoadCaseListComboLBox()
{
}
BEGIN_MESSAGE_MAP(CTBCheckLoadCaseListComboLBox, CGXCheckListComboLBox)
	//{{AFX_MSG_MAP(CTBCheckLoadCaseListComboLBox)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CTBCheckLoadCaseListComboLBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	int i;
	// Get value before OnLButtonDown
	CDWordArray aCheckBefore;
	int nCount = GetCount();
	for (i = 0; i < nCount; i++)
		aCheckBefore.Add(GetCheck(i));

	CGXCheckListComboLBox::OnLButtonDown(nFlags, point);

	// Get value after OnLButtonDown
	// Compare two value, if two value is different, 
	// send message to fire OnModifyCell
	CDWordArray aCheckAfter;
	for (i = 0; i < nCount; i++)
	{
		if (GetCheck(i) != aCheckBefore.GetAt(i))
		{
			//m_pMsgWnd->SendMessage(WM_GX_LBOXEND, 0, 1);
			((CTBCheckListComboBox*)m_pMsgWnd)->OnChangedValue();
			return;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTBCheckLoadCaseListComboBox, CGXCheckListComboBox)

BEGIN_MESSAGE_MAP(CTBCheckLoadCaseListComboBox, CGXCheckListComboBox)
	//{{AFX_MSG_MAP(CTBCheckLoadCaseListComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTBCheckLoadCaseListComboBox::OnChangedValue()
{
	// 捞 何盒捞 泪犁利栏肺 滚弊甫 郴器窍绰 巴 鞍促. 沥犬窍霸绰 舅 荐 绝促.
	// 茄 锅究 橇肺弊伐捞 促款登绰 泅惑捞 乐促. 肋 救蹬搁 酒贰 滴 巩厘栏肺
	// 官层林搁 等促.
	//SetModify(TRUE);
	//OnModifyCell();
	SetModify(TRUE);
	StoreChangedValue();
	OnModifyCell();
}

BOOL CTBCheckLoadCaseListComboBox::StoreChangedValue()
{
	CCheckListBox& pListBox = (CCheckListBox&)m_pDropDownWnd->GetLBox();

	CString csVal;
	CString strTemp;
	int nVal = 0;
	int nCount = pListBox.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (pListBox.GetCheck(i))
		{
			DWORD_PTR lResult = pListBox.GetItemData(i);
			strTemp.Format(_T("%d"), lResult);
			if (!csVal.IsEmpty())
				csVal += _T(",");
			csVal += strTemp;
		}
		
	}
	return Grid()->SetValueRange(CGXRange(m_nRow, m_nCol), csVal);
}

CWnd* CTBCheckLoadCaseListComboBox::CreateListBox(CWnd* pParentWnd, UINT nID)
{
	// Creates the listbox

	// if you want to create an owner-drawn listbox, you should
	// derive a class from CGXCheckListComboLBox, override CreateListBox
	// and return a pointer to it.

	CTBCheckLoadCaseListComboLBox* pListBox = new CTBCheckLoadCaseListComboLBox(this);

	CRect rect;
	pParentWnd->GetClientRect(&rect);
	rect.left++;
	rect.right--;

	if (m_dwListBoxStyle == 0)
		m_dwListBoxStyle = WS_VSCROLL | LBS_NOTIFY;

	pListBox->Create(m_dwListBoxStyle, rect, pParentWnd, nID);

	if (IsReadOnly()) pListBox->EnableWindow(FALSE);
	// CTBCheckLoadCaseListComboBox will delete the listbox object.
	return pListBox;
}

BOOL CTBCheckLoadCaseListComboBox::GetControlText(CString& strResult, ROWCOL nRow, ROWCOL nCol, LPCTSTR pszRawValue, const CGXStyle& style)
{
	BOOL bSuccess = CGXCheckListComboBox::GetControlText(strResult, nRow, nCol, pszRawValue, style);
	if (bSuccess)
	{
		if (strResult.IsEmpty()) strResult = style.GetValue();
		if (strResult.IsEmpty()) return bSuccess;

		// Choice List狼 亲格 肮荐 颇厩
		int nCount = 0;
		if (style.GetIncludeChoiceList())
		{
			CString s = style.GetChoiceListRef();
			CString sItem;
			int n = GXGetNextLine(s, sItem);
			while (!s.IsEmpty() && n != -1)
			{
				n = GXGetNextLine(s, sItem);
				nCount++;
			}
		}
		//// 臂磊荐啊 亲格荐焊促 累栏搁 哭率俊 '0'阑 盲款促.
		//if (strResult.GetLength() < nCount)
		//{
		//	CString szLeft;
		//	szLeft.Format(_T("%0*d"), nCount - strResult.GetLength(), 0);
		//	strResult = szLeft + strResult;
		//}
	}
	return bSuccess;
}

void CTBCheckLoadCaseListComboBox::OnFillDroppedList(CListBox* lbox)
{
	const CGXStyle& style = Grid()->LookupStyleRowCol(m_nRow, m_nCol);

	CGXCheckListComboLBox* checklbox = (CGXCheckListComboLBox*)lbox;
	ASSERT(checklbox->IsKindOf(RUNTIME_CLASS(CGXCheckListComboLBox)));

	// fill with Choices
	if (style.GetIncludeChoiceList())
	{
		CString s = style.GetChoiceListRef();
		CString sItem;
		// skip first entry in choice list because this is the text to be displayed in the cell
		int n = GXGetNextLine(s, sItem);
		Grid()->GetRowCount();
		int nRefIndex;
		const T_SP20_LINK_D& linkD = g_LinkData->GetAt(m_nRow - 1);
		while (!s.IsEmpty() && n != -1)
		{
			n = GXGetNextLine(s, sItem);
			int index = lbox->AddString(sItem);
			g_mapName2Index->Lookup(sItem, nRefIndex);
			const T_SP20_LINK_D& RefLinkD = g_LinkData->GetAt(nRefIndex - 1);
			lbox->SetItemData(index, RefLinkD.nIndex);
			if (linkD.Contain(m_nCol - 5, nRefIndex))
				checklbox->SetCheck(index, TRUE);
		}
	}
	//CString input = style.GetValue();
	//if (style.GetIncludeValue() && !input.IsEmpty())
	//{
	//	int start = 0;
	//	int end = 0;
	//	CStringArray aStrNo;
	//	while ((end = input.Find(',', start)) != -1)
	//	{
	//		aStrNo.Add(input.Mid(start, end - start));
	//		start = end + 1;
	//	}
	//	aStrNo.Add(input.Mid(start));
	//	for (int n = 0; n < aStrNo.GetSize(); n++)
	//	{
	//		int nRow = _ttoi(aStrNo[n]);
	//		for (int i = 0; i < checklbox->GetCount(); i++)
	//		{
	//			if (checklbox->GetItemData(i) == nRow)
	//			{
	//				checklbox->SetCheck(i, TRUE);
	//				//checklbox->Enable(i, FALSE);
	//				break;
	//			}
	//		}
	//	}
	//}
}

void CTBCheckLoadCaseListComboBox::OnStoreDroppedList(CListBox* lbox)
{
	CGXCheckListComboLBox* checklbox = (CGXCheckListComboLBox*)lbox;
	ASSERT(checklbox->IsKindOf(RUNTIME_CLASS(CGXCheckListComboLBox)));
}

/////////////////////////////////////////////////////////////////////////////

int UINTAsc(const void* v1, const void* v2)
{
	UINT u1 = *((UINT*)v1);
	UINT u2 = *((UINT*)v2);
	if (u1 > u2) return 1;
	else if (u1 < u2) return -1;
	return 0;
}

#define NAME_COL_ID         2
#define LCOM_KEY_FMT        _T("%d")

CString CCmdLcomSP20LinksGrid::m_aCurDefVal[] = { _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("") };

CCmdLcomSP20LinksGrid::CCmdLcomSP20LinksGrid()
{
	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),		t_integer,	0);
	AddColInfo(_STR(_T("Load Case")),			t_string,	0);
	AddColInfo(_LSX(+/-),						t_integer,	0);
	AddColInfo(_LSX(Semblance),					t_string,	0);
	AddColInfo(_LSX(Combination),				t_string,	0);
	AddColInfo(_LSX(Mutual),					t_string,	0);
	AddColInfo(_LSX(Companionship),				t_string,	0);
	m_aPrimaryKey.Add(0);
}

CCmdLcomSP20LinksGrid::~CCmdLcomSP20LinksGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCmdLcomSP20LinksGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCmdLcomSP20LinksGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CCmdLcomSP20LinksGrid message handlers
void CCmdLcomSP20LinksGrid::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
{
	CTBCommon::Initialize(pDoc, UnitSystem);
	RegisterControl(IDS_TB_CTRL_CHECKLIST_COMBOBOX, new CTBCheckLoadCaseListComboBox(this));

	LockUpdate(TRUE);

	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	SetFrozenCols(2, 2);
	HideColsByIntend(1, 1, TRUE);

	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));

	CArray<T_STLD_K, T_STLD_K> aStldKeyList;
	m_pDoc->m_pAttrCtrl2->GetStldKeyList4LcomPsiFactor(aStldKeyList);

	CString strStldList = GetLCaseNameList();
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(strStldList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
				  .SetControl(GX_IDS_CTRL_CHECKBOX3D)
				  .SetValueType(GX_VT_NUMERIC)
				  .SetHorizontalAlignment(DT_CENTER)
				  .SetVerticalAlignment(DT_VCENTER)
				  .SetValue(_T("")));

	CString szChoiceList;
	SetStyleRangeCheckListComboBox(CGXRange().SetCols(5,8), szChoiceList);

	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	//HideCols(1, 1, TRUE);

	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // Serial No
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	GetParam()->EnableMoveRows(FALSE);

	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetColWidth(1, 1, 0);
	SetColWidth(2, 2, 40);
	SetColWidth(3, 3, 120);
	SetColWidth(4, 4, 50);
	SetColWidth(5, 8, 120);
	
	MakeItemEx();
	LockUpdate(FALSE);
}

//void CCmdLcomSP20LinksGrid::InitBrowserSettings()
//{
//	// Initialize pointer to some objects
//	CGXGridParam* pParam = GetParam();
//	ASSERT_VALID(pParam);
//
//	// Disable direct copy and paste
//	pParam->m_bDirectCopyPaste = FALSE;
//	pParam->m_bDirectCutPaste = FALSE;
//	m_nClipboardFlags &= ~GX_DNDSTYLES;
//
//	CGXStylesMap* pStyMap = pParam->GetStylesMap();
//	ASSERT_VALID(pStyMap);
//
//	CGXProperties* pProp = pParam->GetProperties();
//	ASSERT_VALID(pProp);
//
//	CTBBrowseParam* pBrowseData = GetBrowseParam();
//
//	//// Register user-defined controls
//	//RegisterControl(
//	//	IDS_TB_CTRL_BROWSEROWHEADER,
//	//	new CTBBrowserArrowRowHeader(this)
//	//);
//
//	RegisterControl(IDS_TB_CTRL_CHECKLIST_COMBOBOX, new CTBCheckLoadCaseListComboBox(this));
//
//	if (!pBrowseData->m_bIsBrowseInit)
//	{
//		BOOL bUndo = GetParam()->IsEnableUndo();
//		GetParam()->EnableUndo(FALSE);
//
//		// Following settings should only be initialized the first
//		// time OnInitialUpdate is called. This is especially
//		// usefull if the parameter-object is shared among
//		// several views (MFC doc/view support) because OnInitialUpdate
//		// is called for every new view.
//
//		// Row and column headers
//		ColHeaderStyle()
//			.SetReadOnly(FALSE)
//			.SetEnabled(FALSE) // Current cell cannot be moved to header
//			//.SetControl(IDS_TB_CTRL_BROWSECOLHEADER)
//			;
//
//		RowHeaderStyle()
//			.SetEnabled(FALSE)
//			.SetControl(IDS_TB_CTRL_BROWSEROWHEADER); // use previously registered header control
//
//		// Special grid settings
//		pParam->EnableMoveRows(FALSE);                  // rows cannot be dragged
//		pParam->EnableMoveCols(TRUE);                   // columns can be dragged
//		pParam->EnableTrackRowHeight(GX_TRACK_ALL);     // all rows have the same height.
//		pParam->EnableThumbTrack(FALSE);                // thumb-tracking is disabled
//
//		pParam->SetNumberedRowHeaders(FALSE);
//		pParam->SetNumberedColHeaders(FALSE);
//
//		// Do not allow selecting range of cells in the table
//		// pParam->EnableSelection(GX_SELTABLE | GX_SELROW | GX_SELCOL);
//
//		// set focus to control when moving current cell
//		// pParam->SetActivateCellFlags(GX_CAFOCUS_SETCURRENT);
//
//		// removing rows is not undoable for this kind of grid
//		pParam->SetRemoveColsFlags(FALSE);
//		pParam->SetRemoveRowsFlags(FALSE);
//
//		pProp->SetMarkColHeader(FALSE);                 // Turn off pressed button effect for column headers
//		pProp->SetPrintRowHeaders(FALSE);               // Don't print column headers
//
//		// set row header width to 22 pixels
//		SetColWidth(0, 0, 22);
//
//		pBrowseData->m_bIsBrowseInit = TRUE;
//		GetParam()->EnableUndo(bUndo);
//	}
//}

// CCmdLcomSP20LinksGrid Overridable Functions
void CCmdLcomSP20LinksGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_STLD_K)keys[i]);
	}
}

long CCmdLcomSP20LinksGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count
	long nKeyCount;
	CArray<T_STLD_K, T_STLD_K> aSelKey;

	GetAllSelectedLcom(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CCmdLcomSP20LinksGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCombMainGrid__ValidateFie));
		return FALSE;
	}

	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 2; i < GetColCount() - 4; i++)
	{
		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

BOOL CCmdLcomSP20LinksGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_KEY &key, T_SP20_LINK_D &data)
{
	for (int i = 0; i < value.GetSize(); i++)
	{
		if (!ValidateField(value[aCols[i]-1], i)) 
		{
			SetWarningText(_T(""));
			return FALSE;
		}
	}
	if (!ValidateRecord(value, aCols))
	{
		SetWarningText(_T(""));
		return FALSE;
	}

	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i俊 秦寸窍绰 value啊 历厘等 困摹
		ConvStrToData(i, value[nPos], data);
	}

	key = data.nIndex;

	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::AddToDB(ROWCOL nRow)
{
	T_KEY key;
	T_SP20_LINK_D data;
 
	GetValue(nRow, key, data);

	//T_SP20_LINK_D TempD;
	//if(m_pMapLinkD->Lookup(key, TempD))
	//{
	//	AfxMessageBox(_LS(IDS_CMD_LPSI_EXIST_LOADCASE));
	//	return FALSE;
	//}

	m_paLcaseSP20LinkData->SetAt(nRow - 1, data);
	InsertRecord(nRow);
	SetValue(nRow, key, data);

	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::DeleteFromDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::DeleteFromDB(CRowColArray& awRows)
{
	ASSERT(FALSE);
	//if (awRows.GetSize() <= 0) return TRUE;

	//// _LS(IDS_WG_CMD__ADDD__Name) 漠烦阑 掘绰促.
	//ROWCOL nCol;
	//VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));

	//CString value;
	//CArray<CString, CString&> awKeys;
 //
	//for (int i = 0; i < awRows.GetSize(); i++)
	//{
	//	if(!DeleteFromDB(awRows.GetAt(i))) return FALSE;
	//}

	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));  // load case name

	T_KEY key = nRow;
	T_SP20_LINK_D data;
	data = m_paLcaseSP20LinkData->GetAt(key - 1);
	GetValue(nRow, key, data);
	ASSERT(nRow == key);

	T_SP20_LINK_D PreTemp;
	PreTemp = m_paLcaseSP20LinkData->GetAt(key - 1);

	SetValue(nRow, key, data);
	m_paLcaseSP20LinkData->SetAt(key - 1,data);

	CArray<T_KEY, T_KEY> aIncreaseRow, aDecreaseRow;
	CMap<T_KEY, T_KEY, T_KEY, T_KEY> mapChangeRow;
	//Semblance
	if (GetChangeList(PreTemp.aLinksCase[0], data.aLinksCase[0], aIncreaseRow, aDecreaseRow))
	{
		for (int i = 0; i < aIncreaseRow.GetSize(); i++)
		{
			IncreaseListNo(aIncreaseRow[i],nRow,5);
			mapChangeRow.SetAt(aIncreaseRow[i], 1);
		}
		for (int i = 0; i < aDecreaseRow.GetSize(); i++)
		{
			DecreaseListNo(aDecreaseRow[i],nRow,5);
			mapChangeRow.SetAt(aDecreaseRow[i], 1);
		}
	}
	//Combinatio
	if (GetChangeList(PreTemp.aLinksCase[1], data.aLinksCase[1], aIncreaseRow, aDecreaseRow))
	{
		//1.Combinatio 下荷载号要一起出现
		//2.找到勾选了的row,如果每个row处于不同的组,把这些组合并.当前row如果勾选了别的是一个组的,则把组内成员全部合并到aIndex
		CMapEx<T_KEY, T_KEY, T_KEY, T_KEY> mapIndex;
		for (int i = 0; i < data.aLinksCase[1].GetSize(); i++)
		{
			mapIndex.SetAt(data.aLinksCase[1][i], 1);
		}
		for (int i = 0; i < aIncreaseRow.GetSize(); i++)
		{
			T_KEY_LIST aIndex;
			CString strTemp = GetValueRowCol(aIncreaseRow[i], 6);
			GetStldListByString(strTemp, aIndex);
			for (int j = 0; j < aIndex.GetSize(); j++)
				mapIndex.SetAt(aIndex[j], 1);
		}
		mapIndex.SetAt(nRow, 1);
		//
		T_KEY_LIST aIndex;
		mapIndex.GetSortedKeyListforUintK(aIndex);
		T_KEY_LIST aTemp;
		for (int i = 0; i < aIndex.GetSize(); i++)
		{
			aTemp.Copy(aIndex);
			for (int j = 0; j < aTemp.GetSize(); j++)
			{
				if (aTemp[j] == aIndex[i])
				{
					aTemp.RemoveAt(j);
					break;
				}
			}
			//m_pMapLinkD->Lookup(aIndex[i], data);
			//data.aLinksCase[1].Copy(aTemp);
			//m_pMapLinkD->SetAt(aIndex[i], data);
			m_paLcaseSP20LinkData->GetAt(aIndex[i] - 1).aLinksCase[1].Copy(aTemp);
			mapChangeRow.SetAt(aIndex[i], 1);
		}
		for (int i = 0; i < aDecreaseRow.GetSize(); i++)
		{
			//m_pMapLinkD->Lookup(aDecreaseRow[i], data);
			//data.aLinksCase[1].RemoveAll();
			//m_pMapLinkD->SetAt(aDecreaseRow[i], data);
			m_paLcaseSP20LinkData->GetAt(aDecreaseRow[i] - 1).aLinksCase[1].RemoveAll();
			mapChangeRow.SetAt(aDecreaseRow[i], 1);
		}
		//3.这个操作的优先级高于Companionship.检查工况号,如果有companionship的组合在combination选中或者取消,需要修改companionship.

	}
	//Mutual
	if (GetChangeList(PreTemp.aLinksCase[2], data.aLinksCase[2], aIncreaseRow, aDecreaseRow))
	{
		for (int i = 0; i < aIncreaseRow.GetSize(); i++)
		{
			IncreaseListNo(aIncreaseRow[i], nRow, 7);
			mapChangeRow.SetAt(aIncreaseRow[i], 1);
		}
		for (int i = 0; i < aDecreaseRow.GetSize(); i++)
		{
			DecreaseListNo(aDecreaseRow[i], nRow, 7);
			mapChangeRow.SetAt(aDecreaseRow[i], 1);
		}
	}
	//Companion
	if (GetChangeList(PreTemp.aLinksCase[3], data.aLinksCase[3], aIncreaseRow, aDecreaseRow))
	{
		/* After comp is checked, check the parent. If comp in parent has this index checked, uncheck it */
		T_KEY_LIST aIndex;
		for (int i = 0; i < aIncreaseRow.GetSize(); i++)
		{
			CString strTemp = GetValueRowCol(aIncreaseRow[i], 8);
			GetStldListByString(strTemp, aIndex);
			if (ContainsIndex(key, aIndex))
			{
				RemoveIndex(aIncreaseRow[i], 3, key);
				mapChangeRow.SetAt(aIncreaseRow[i], 1);
			}
		}
	}
	POSITION pos = mapChangeRow.GetStartPosition();
	T_KEY keyChange;
	T_KEY nTemp;
	while (pos)
	{
		mapChangeRow.GetNextAssoc(pos, keyChange, nTemp);
		data.Initialize();
		//m_pMapLinkD->Lookup(keyChange, data);
		data = m_paLcaseSP20LinkData->GetAt(keyChange - 1);
		SetValue(keyChange, keyChange, data);
	}
	
	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);
	return TRUE;
}

CString CCmdLcomSP20LinksGrid::GetNewKey()
{
	return _T("");
}

CString CCmdLcomSP20LinksGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCmdLcomSP20LinksGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCmdLcomSP20LinksGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_SP20_LINK_D* pData;
	pKey = new T_STLD_K;
	pData = new T_SP20_LINK_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCmdLcomSP20LinksGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_SP20_LINK_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STLD_K*)aKey.GetAt(i);
		pData = (T_SP20_LINK_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCmdLcomSP20LinksGrid::GetCountData()
{
	return m_paLcaseSP20LinkData->GetCount();
}

void CCmdLcomSP20LinksGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_SP20_LINK_D Data;

	for (long nCount = 0; nCount < nFilteredNum; nCount++)
 	{
		Data = m_paLcaseSP20LinkData->GetAt(aFilteredKeys[nCount] - 1);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CCmdLcomSP20LinksGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	//T_KEY Key;
	T_SP20_LINK_D Data;

	
	if (nNumData != m_paLcaseSP20LinkData->GetSize()) { ASSERT(0); return; }

	for(long nCount=1 ; nCount <= nNumData; nCount++)
	{
		//if(!m_mIndex.Lookup(nCount, Key)) {ASSERT(0); continue;}
		//if(!m_pMapLinkD->Lookup(nCount, Data)) {ASSERT(0); continue;}
		Data = m_paLcaseSP20LinkData->GetAt(nCount - 1);
		SetValue(GetParam()->GetData(), nCount, nCount, Data, awCols);
	}
}

void CCmdLcomSP20LinksGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_KEY nIndex;
	T_SP20_LINK_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) nIndex = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	
	SetValue(ncRow, nIndex, Data);
}

// CCmdLcomSP20LinksGrid Implementation
BOOL CCmdLcomSP20LinksGrid::GetValue(ROWCOL nRow, T_KEY& nIndex, T_SP20_LINK_D& data)
{
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		VERIFY(ConvertFormulaToValue(str)); 

		ConvStrToData(i, str, data);
	}

	nIndex = data.nIndex;

	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::SetValue(ROWCOL nRow, const T_KEY& nIndex, const T_SP20_LINK_D& data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, nIndex, data, awCols);
}

BOOL CCmdLcomSP20LinksGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_KEY& nIndex, const T_SP20_LINK_D& data, CRowColArray& awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (nIndex == 0) str.Empty();
			else str.Format(LCOM_KEY_FMT, nIndex);
		}
		else if(i==1)
		{
			str.Format(_T("%d"), nRow);
		}
		else ConvDataToStr(i, data, str);

		if (i > 3)
		{
			CString strOtherIndex;
			CArray<UINT, UINT> aIndex;
			//If the load falls into the "combination" group, the "companion" group should not be assigned, as the firm condition of co-occurrence is already set.
			if (i == 7)
			{
				CArray<UINT, UINT> aExcludeIndex;
				aExcludeIndex.Copy(m_paLcaseSP20LinkData->GetAt(nIndex - 1).aLinksCase[1]);
				//TODO:exclude还需要排除这种情况.在companion中7勾选了8,8便不让勾选7;或者如果让勾选,则勾选后要把7勾选的8去掉勾选.
				GetOtherIndexList(nIndex, aExcludeIndex, strOtherIndex, aIndex);
			}
			else
			{
				GetOtherIndexList(nIndex, strOtherIndex, aIndex);
			}
			SetStyleRangeCheckListComboBox(CGXRange(nRow, i + 1), strOtherIndex);
		}
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}
	if(data.eLoadCaseType == e_SP20_Inactive)
	{
		SetStyleRange(CGXRange(nRow, 4, nRow, 8), CGXStyle().SetEnabled(FALSE));
	}
	return TRUE;
}

void CCmdLcomSP20LinksGrid::ConvStrToData(int i, CString& value, T_SP20_LINK_D& data)
{
	if (i == 1)
		data.nIndex = _tstoi(value);
	else if (i==2)
	{
		//data.StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(value);
		data.LoadCaseName = value; // name
	}
	else if (i==3) data.bSynchronicity = _tstoi(value);
	else if (i >= 4 && i <= 7)
	{
		GetStldListByString(value,data.aLinksCase[i - 4]);
	}
}

void CCmdLcomSP20LinksGrid::ConvDataToStr(int i, const T_SP20_LINK_D& data, CString& value)
{
	if (i==2) value = data.LoadCaseName;
	else if (i==3) value.Format(_T("%d"), data.bSynchronicity);
	else if (i >= 4 && i <= 7)
	{
		GetStldNoListString(data.aLinksCase[i - 4], value);
	}
}

void CCmdLcomSP20LinksGrid::MakeSearchKey(const T_KEY&key, const T_SP20_LINK_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(LCOM_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCmdLcomSP20LinksGrid::GetStldNoListString(IN const CArray<T_KEY, T_KEY>& aIndex, OUT CString& value)
{
	value.Empty();
	CString strTemp;
	for (int i = 0; i < aIndex.GetSize(); i++)
	{
		strTemp.Format(_T("%d"), aIndex[i]);
		if (i != 0)
			value += _T(",");
		value += strTemp;
	}
}

void CCmdLcomSP20LinksGrid::GetStldListByString(IN CString& value, OUT CArray<T_KEY, T_KEY>& aIndex)
{
	aIndex.RemoveAll();
	if (value.IsEmpty())
		return;
	CString input = value;
	int start = 0;
	int end = 0;
	CStringArray aStrNo;
	while ((end = input.Find(',', start)) != -1) 
	{
		aStrNo.Add(input.Mid(start, end - start));
		start = end + 1;
	}
	aStrNo.Add(input.Mid(start));
	aIndex.SetSize(aStrNo.GetSize());
	for (int i = 0; i < aStrNo.GetSize(); i++)
	{
		aIndex[i] = _ttoi(aStrNo[i]);
	}
}

BOOL CCmdLcomSP20LinksGrid::GetChangeList(IN const CArray<T_KEY, T_KEY>& aSrc, IN const CArray<T_KEY, T_KEY>& aChange, OUT CArray<T_KEY, T_KEY>& aIncreaseRow, OUT CArray<T_KEY, T_KEY>& aDecreaseRow)
{
	aIncreaseRow.RemoveAll();
	aDecreaseRow.RemoveAll();
	if (aSrc.GetSize() == aChange.GetSize())
	{
		BOOL bChange = FALSE;
		for (int i = 0; i < aChange.GetSize(); i++)
		{
			if (aSrc[i] != aChange[i])
			{
				bChange = TRUE;
				break;
			}
		}
		if (!bChange)
			return FALSE;
	}
	CMapEx<T_KEY, T_KEY, T_KEY, T_KEY> mapIndex;
	for (int i = 0; i < aSrc.GetSize(); i++)
	{
		mapIndex.SetAt(aSrc[i], 1);
	}
	for (int i = aChange.GetSize() - 1; i >= 0; i--)
	{
		if (mapIndex.Exist(aChange[i]))
		{
			mapIndex.RemoveKey(aChange[i]);
		}
		else
			aIncreaseRow.Add(aChange[i]);
	}
	mapIndex.GetSortedKeyListforUintK(aDecreaseRow);
	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::IncreaseListNo(IN ROWCOL nRow, IN T_KEY key,IN ROWCOL nCol)
{
	CString str;
	str = GetValueRowCol(nRow, nCol);
	CArray<T_KEY, T_KEY> aIndex;
	GetStldListByString(str, aIndex);
	T_SP20_LINK_D linkD;
	linkD = m_paLcaseSP20LinkData->GetAt(nRow - 1);
	bool success = false;
	for (int i = 0; i < aIndex.GetSize(); i++)
	{
		if (aIndex[i] > key)
		{
			linkD.aLinksCase[nCol - 5].InsertAt(i, key);
			success = true;
			break;
		}
	}
	if (!success)
		linkD.aLinksCase[nCol - 5].Add(key);
	m_paLcaseSP20LinkData->SetAt(nRow - 1, linkD);
	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::DecreaseListNo(IN ROWCOL nRow, IN T_KEY key,IN ROWCOL nCol)
{
	CString str;
	str = GetValueRowCol(nRow, nCol);
	CArray<T_KEY, T_KEY> aIndex;
	GetStldListByString(str, aIndex);
	T_SP20_LINK_D linkD;
	linkD = m_paLcaseSP20LinkData->GetAt(nRow - 1);
	bool success = false;
	for (int i = 0; i < aIndex.GetSize(); i++)
	{
		if (aIndex[i] == key)
		{
			linkD.aLinksCase[nCol - 5].RemoveAt(i);
			success = true;
			break;
		}
	}
	if (!success)
	{
		ASSERT(FALSE);
	}
	//m_pMapLinkD->SetAt(nRow, linkD);
	m_paLcaseSP20LinkData->SetAt(nRow - 1, linkD);
	return TRUE;
}

BOOL CCmdLcomSP20LinksGrid::ContainsIndex(IN T_KEY index, IN CArray<T_KEY, T_KEY>& aIndex)
{
	for (int i = 0; i < aIndex.GetSize(); i++)
	{
		if(aIndex[i] == index)
			return TRUE;
	}
	return FALSE;
}

void CCmdLcomSP20LinksGrid::GetAllSelectedLcom(CArray<T_KEY, T_KEY>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

CString CCmdLcomSP20LinksGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

BOOL CCmdLcomSP20LinksGrid::IsActiveCol(ROWCOL& nCol)
{
	ROWCOL nRow, nACol;

	GetCurrentCell(nRow, nCol);
	VERIFY(GetColumnNoFor(5, nACol));

	return nCol==nACol;
}

void CCmdLcomSP20LinksGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

BOOL CCmdLcomSP20LinksGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CTBCommon::GetStyleRowCol(nRow, nCol, style, mt, nType);

	if(IsAppendRow(nRow))
	{
		if (nCol > 4)
		{
			style.SetEnabled(FALSE).SetInterior(m_Color.DisabledBg);
		}
	}

	return TRUE;
}

CString CCmdLcomSP20LinksGrid::GetLCaseNameList()
{
	CString strStldList = _T("");

	T_SP20_LINK_D LinkD;
	int nSize = m_paLcaseSP20LinkData->GetCount();
	for (int i = 1; i <= nSize; i++)
	{
		//if (!m_pMapLinkD->Lookup(i, LinkD)) { ASSERT(FALSE); continue; }
		LinkD = m_paLcaseSP20LinkData->GetAt(i - 1);
		strStldList += LinkD.LoadCaseName;
		strStldList += _T("\n");
	}
	
	return strStldList;
}
void CCmdLcomSP20LinksGrid::GetOtherIndexList(IN T_KEY index,OUT CString& strValue,OUT CArray<T_KEY, T_KEY>& aIndex)
{
	strValue = _T("\n");
	aIndex.RemoveAll();

	
	int nSize = m_paLcaseSP20LinkData->GetCount();
	for (int i = 0; i < nSize; i++)
	{
		//if (!m_pMapLinkD->Lookup(i, LinkD)) { ASSERT(FALSE); continue; }
		const T_SP20_LINK_D& LinkD = m_paLcaseSP20LinkData->GetAt(i);
		if(LinkD.nIndex == index)
			continue;
		aIndex.Add(LinkD.nIndex);
		strValue += LinkD.LoadCaseName;
		strValue += _T("\n");
	}
}

void CCmdLcomSP20LinksGrid::GetOtherIndexList(IN T_KEY index, IN CArray<T_KEY, T_KEY>& aExcludeIndex, OUT CString& strValue, OUT CArray<T_KEY, T_KEY>& aIndex)
{
	strValue = _T("\n");
	aIndex.RemoveAll();

	T_SP20_LINK_D LinkD;
	int nSize = m_paLcaseSP20LinkData->GetCount();
	for (int i = 0; i < nSize; i++)
	{
		LinkD = m_paLcaseSP20LinkData->GetAt(i);
		if (LinkD.nIndex == index)
			continue;
		if (ContainsIndex(LinkD.nIndex, aExcludeIndex))
			continue;
		aIndex.Add(LinkD.nIndex);
		strValue += LinkD.LoadCaseName;
		strValue += _T("\n");
	}
}

BOOL CCmdLcomSP20LinksGrid::OnPasteFromClipboard(const CGXRange &range)
{
	AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	return FALSE;
}

BOOL CCmdLcomSP20LinksGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bResult = CTBCommon::OnInsertEmptyRecord(nRow);

	ROWCOL ncRow, ncCol;
	CString value;
		
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);

	if (pControl && ncCol == 3) // Load Case
	{
		CString strLoadCaseName=_T("");
		pControl->GetValue(strLoadCaseName);
		T_STLD_D StldD;
		m_pDoc->m_pAttrCtrl->GetStld(strLoadCaseName, StldD);
		CString strOtherStld;
		CArray<UINT, UINT> aStldNo;
		GetOtherIndexList(StldD.LoadCaseId, strOtherStld, aStldNo);
		SetStyleRangeCheckListComboBox(CGXRange().SetCols(5, 8), strOtherStld);
		//for (int i = 0; i < 4; i++)
		//{
		//	CGXControl* pBaseControl = GetControl(nRow, i + 5);
		//	if (pBaseControl->IsKindOf(RUNTIME_CLASS(CTBCheckLoadCaseListComboBox)))
		//	{
		//		CTBCheckLoadCaseListComboBox* pLoadCaseCheckListCombox = static_cast<CTBCheckLoadCaseListComboBox*>(pBaseControl);
		//		pLoadCaseCheckListCombox->SetStldArrayNo(aStldNo);
		//		//CCheckListBox& pListBox = (CCheckListBox&)(pLoadCaseCheckListCombox->GetGridComboBox()->GetLBox());
		//		//if (aStldNo.GetSize() != pListBox.GetCount())
		//		//{
		//		//	ASSERT(FALSE);
		//		//	continue;
		//		//}
		//		//for (int i = 0; i < pListBox.GetCount(); i++)
		//		//{
		//		//	pListBox.SetItemData(i, aStldNo[i]);
		//		//}
		//	}
		//}
	}

	return bResult;
}

void CCmdLcomSP20LinksGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnModifyCell(nRow, nCol);

	if(nCol == 3)
	{
		CGXControl* pControl = (CGXControl*)GetControl(nRow, nCol);
		CString strLoadCaseName=_T("");
		pControl->GetValue(strLoadCaseName);
		T_STLD_D StldD;
		m_pDoc->m_pAttrCtrl->GetStld(strLoadCaseName, StldD);
		SetExpressionRowCol(nRow, 4, StldD.LoadCaseType);
	}
}

void CCmdLcomSP20LinksGrid::RemoveIndex(int nSource, int nSlot, int nIndex)
{
	for (int i = 0; i < m_paLcaseSP20LinkData->GetAt(nSource - 1).aLinksCase[nSlot].GetSize(); i++)
	{
		if (nIndex == m_paLcaseSP20LinkData->GetAt(nSource - 1).aLinksCase[nSlot][i])
		{
			m_paLcaseSP20LinkData->GetAt(nSource - 1).aLinksCase[nSlot].RemoveAt(i);
			break;
		}
	}
}