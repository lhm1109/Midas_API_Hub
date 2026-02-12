// CMThgaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThgaDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThgaDlg
#define CDialog CCMChildBarBase

//IMPLEMENT_DYNCREATE(CCMThgaDlg, CDialog)

CCMThgaDlg::CCMThgaDlg()
	: CDialog(CCMThgaDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThgaDlg)
	//}}AFX_DATA_INIT
	m_aCtrlDir[0].Add(IDC_CMD_SCALE_FACTOR);
	m_aCtrlDir[0].Add(IDC_CMD_ARRIVAL_TIME);
	m_aCtrlDir[1].Add(IDC_CMD_SCALE_FACTOR2);
	m_aCtrlDir[1].Add(IDC_CMD_ARRIVAL_TIME2);
	m_aCtrlDir[2].Add(IDC_CMD_SCALE_FACTOR3);
	m_aCtrlDir[2].Add(IDC_CMD_ARRIVAL_TIME3);
}

CCMThgaDlg::~CCMThgaDlg()
{
}

void CCMThgaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThgaDlg)
	DDX_Control(pDX, IDC_CMD_ANGLE_OF_HGA_EDIT, m_wndHGA);
	DDX_Control(pDX, IDC_THGA_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_LOADCASE_NAME, m_wndLoadCase);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR3, m_wndScaleFactor[2]);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR2, m_wndScaleFactor[1]);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor[0]);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME3, m_wndFuncName[2]);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME2, m_wndFuncName[1]);
	DDX_Control(pDX, IDC_CMD_FUNC_NAME, m_wndFuncName[0]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_UNIT3, m_wndArTimeUnit[2]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_UNIT2, m_wndArTimeUnit[1]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME_UNIT, m_wndArTimeUnit[0]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME3, m_wndArTime[2]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME2, m_wndArTime[1]);
	DDX_Control(pDX, IDC_CMD_ARRIVAL_TIME, m_wndArTime[0]);
}

/////////////////////////////////////////////////////////////////////////////
// CCMThgaDlg implementation

void CCMThgaDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Case_Name), _LS(IDS_CMD_ANGLE_OF_ACC)};
	int nColWidth[] = {92, 112};
	int nColNum = 2;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMThgaDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMThgaDlg::MakeItemEx()
{
	m_List.DeleteAllItems();


	T_THGA_K key;
	T_THGA_D data;
	CArray<T_THGA_K, T_THGA_K> aKey;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartThga();
	while (pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextThga(pos, key, data);
		aKey.Add(key);
	}
	int nItemCount = aKey.GetSize();
	if(nItemCount == 0) return;
	qsort((void*)aKey.GetData(), nItemCount, sizeof(T_THGA_K), CCompFunc::UINTAsc);

	for (int i = 0; i < nItemCount; i++)
	{
		key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetThga(key, data);
		m_List.InsertItem(i, _T(""));
		SetItem(i, key, data);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

CString CCMThgaDlg::DataToStr(int i, T_THGA_K key, T_THGA_D &data)
{
	CString value;
	T_THIS_D thload;
	T_THFC_D thfc;
	if (i == 0) 
	{
		if (m_pDoc->m_pAttrCtrl->GetThis(key, thload))
			value = thload.LoadCaseName;
		else value = _LS(IDS_WG_CMD__ADDD__Error);
	}
	else if (i == 1)
	{
		value.Format(_T("%g"), data.dAngleHGA);
	}
	else if (i == 2) value = _LS(IDS_WG_CMD__ADDD__Error);
	return value;
}

void CCMThgaDlg::SetItem(int nIndex, T_THGA_K key, T_THGA_D &data)
{
	LVITEM lvitem;
	CString str;
	lvitem.iItem=nIndex;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, key, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_List.SetItemData(nIndex, (DWORD)key);
}

BOOL CCMThgaDlg::FindItem(T_THGA_K key, int &nIndex)
{
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
		if (m_List.GetItemData(i) == (DWORD)key)
		{
			nIndex = i; return TRUE;
		}

	return FALSE;
}

BOOL CCMThgaDlg::ValidData(T_THGA_D &data)
{
	if (data.nThisFuncKey[0] <= 0 &&
			data.nThisFuncKey[1] <= 0 &&
			data.nThisFuncKey[2] <= 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Time_history_function_nam));
		return FALSE;
	}
		
	return TRUE;
}

BOOL CCMThgaDlg::Dlg2Data(T_THGA_K &key, T_THGA_D &data)
{
	data.Initialize();

	UINT nLoadCaseType;
	if (!m_wndLoadCase.GetSelectedLoad(nLoadCaseType, key))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
		return FALSE;
	}

	CString csDir[] = {_T("X"), _T("Y"), _T("Z")};
	for (int i = 0; i < 3; i++)
	{
		m_wndFuncName[i].GetSelectedThfc(data.nThisFuncKey[i]);
		if (data.nThisFuncKey[i] == 0) continue;
		
		if (!m_wndScaleFactor[i].GetEditValue(data.dFactor[i]))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_for_), csDir[i]);
			return FALSE;
		}

		data.dArrivalTime[i] = m_wndArTime[i].GetEditValue();
	}
	
	data.dAngleHGA = m_wndHGA.GetEditValue();

	return TRUE;
}

BOOL CCMThgaDlg::Data2Dlg(T_THGA_K key, T_THGA_D &data)
{
	m_wndLoadCase.ChangeSelect(D_LOADCASE_HISTORY, key);

	CString csVal;
	for (int i = 0; i < 3; i++)
	{
		m_wndFuncName[i].ChangeSelect(data.nThisFuncKey[i]);
		csVal.Format(_T("%g"), data.dFactor[i]);
		m_wndScaleFactor[i].SetWindowText(csVal);
		csVal.Format(_T("%g"), data.dArrivalTime[i]);
		m_wndArTime[i].SetWindowText(csVal);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDir[i], data.nThisFuncKey[i] != 0);
	}

	m_wndHGA.SetValue(data.dAngleHGA);

	return TRUE;
}

void CCMThgaDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		MakeItemEx();
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

void CCMThgaDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_THGA_K Key;
	T_THGA_D Data;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THGA_ADD):
			{
				pViewBuff->GetThga(nKey, Key, Data);
				int nCount = m_List.GetItemCount();
				m_List.InsertItem(nCount, _T(""));
				SetItem(nCount, Key, Data);
			}
			break;
		case(UR_THGA_DEL):
			{
				pViewBuff->GetThga(nKey, Key, Data);
				int nIndex;
				if (FindItem(Key, nIndex))
					m_List.DeleteItem(nIndex);
			}
			break;
		default:
			break;
		}
	} // end of while
}

BEGIN_MESSAGE_MAP(CCMThgaDlg, CDialog)
	//{{AFX_MSG_MAP(CCMThgaDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_FUNC_BTN, OnCmdFuncBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_THGA_LIST, OnItemchangedThgaList)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME, OnSelchangeCmdFuncName)
	ON_BN_CLICKED(IDC_CMD_FUNC_BTN2, OnCmdFuncBtn)
	ON_BN_CLICKED(IDC_CMD_FUNC_BTN3, OnCmdFuncBtn)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME2, OnSelchangeCmdFuncName)
	ON_CBN_SELCHANGE(IDC_CMD_FUNC_NAME3, OnSelchangeCmdFuncName)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_ANGLE_OF_HGA_SPIN, OnDeltaposCmdAngleOfHgaSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThgaDlg diagnostics

#ifdef _DEBUG
void CCMThgaDlg::AssertValid() const
{
	CDialog::AssertValid();
}

void CCMThgaDlg::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMThgaDlg message handlers

/*
void CCMThgaDlg::OnInitialUpdate() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetListCtrlHeader();
	m_wndLoadCase.SetLoadType(D_SELECTLOAD_HISTORY, TRUE);
	T_THFC_K nKey;
	for (int i = 0; i < 3; i++)
	{
		m_wndFuncName[i].ChangeType(TRUE, TRUE, FALSE, FALSE); // normal, accel만 허용
		m_wndArTime[i].SetUnitType(CUnitCtrl::m_THGA_UNIT.dArrivalTime);
		m_wndArTimeUnit[i].SetUnitType(CUnitCtrl::m_THGA_UNIT.dArrivalTime);
		m_wndFuncName[i].GetSelectedThfc(nKey);
		m_wndScaleFactor[i].SetWindowText(_T("1"));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDir[i], nKey != 0);
	}

	MakeItemEx();
}
*/

BOOL CCMThgaDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitDialog();
	 
	SetListCtrlHeader();
	m_wndLoadCase.SetIgnoreInitialLoad(TRUE);
	m_wndLoadCase.SetLoadType(D_SELECTLOAD_HISTORY, TRUE);
	T_THFC_K nKey;
	for (int i = 0; i < 3; i++)
	{
		m_wndFuncName[i].ChangeType(TRUE, TRUE, FALSE, FALSE, FALSE); // normalacc, accel만 허용
		m_wndArTime[i].SetUnitType(CUnitCtrl::m_THGA_UNIT.dArrivalTime);
		m_wndArTimeUnit[i].SetUnitType(CUnitCtrl::m_THGA_UNIT.dArrivalTime);
		m_wndFuncName[i].GetSelectedThfc(nKey);
		m_wndScaleFactor[i].SetWindowText(_T("1"));
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDir[i], nKey != 0);
	}

	MakeItemEx();

	m_wndHGA.SetRange(-360, 360);
	m_wndHGA.SetValue(0);
	
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CCMThgaDlg::OnCmdBtnAdd() 
{
	T_THGA_K key;
	T_THGA_D data;
	
	if (!Dlg2Data(key, data)) return;
	if (!ValidData(data)) return;

	int nCount = m_List.GetItemCount();
	if (m_pDoc->m_pDataCtrl->AddThga(key, data))
		m_List.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMThgaDlg::OnCmdBtnDelete() 
{
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_ground_accele));
		return;
	}
	int nCount = m_List.GetItemCount();
	T_THGA_K key = (T_THGA_K)m_List.GetItemData(nIndex);
	if (m_pDoc->m_pDataCtrl->DelThga(key))
	{
		if (nIndex >= nCount-1) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCMThgaDlg::OnCmdBtnModify() 
{
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_ground_accele));
		return;
	}

	T_THGA_K key;
	T_THGA_D data;
	if (!Dlg2Data(key, data)) return;
	if (!ValidData(data)) return;
	
	T_THGA_K KeyOld = (T_THGA_K)m_List.GetItemData(nIndex);
	if (m_pDoc->m_pDataCtrl->ModifyThga(KeyOld, key, data))
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMThgaDlg::OnItemchangedThgaList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	T_THGA_K key;
	T_THGA_D data;
	int nIndex;

	if (!GetSelectedData(nIndex))
	{
		UINT nLoadCaseType;
		m_wndLoadCase.GetSelectedLoad(nLoadCaseType, key);
		data.Initialize();
	}
	else 
	{
		key = m_List.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetThga(key, data);
	}

	// 값을 설정한다.
	Data2Dlg(key, data);

	*pResult = 0;
}

void CCMThgaDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMThgaDlg::OnCmdLoadcaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HISTORY_LOADCASE, 0));
}

void CCMThgaDlg::OnCmdFuncBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_TIME_TFUNCTION, 0));
}

void CCMThgaDlg::OnSelchangeCmdFuncName() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	int nDir=0;
	UINT nID = LOWORD(pMsg->wParam);
	if (nID == IDC_CMD_FUNC_NAME) nDir = 0;
	else if (nID == IDC_CMD_FUNC_NAME2) nDir = 1;
	else if (nID == IDC_CMD_FUNC_NAME3) nDir = 2;
	
	T_THFC_K nKey;
	m_wndFuncName[nDir].GetSelectedThfc(nKey);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDir[nDir], nKey != 0);
}

BOOL CCMThgaDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_THGA_D data;

	m_pDoc->m_pAttrCtrl->GetThga(key, data);
	Data2Dlg(key, data);

	return TRUE;
}


void CCMThgaDlg::OnDeltaposCmdAngleOfHgaSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_ANGLE_OF_HGA_EDIT,_T("-360"),_T("360"),1,pNMHDR,pResult);
	
	*pResult = 0;
}
