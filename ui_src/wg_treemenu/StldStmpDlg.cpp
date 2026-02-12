// StldStmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldStmpDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STMP_DATA_FMT _LS(IDS_WG_TREEMENU__g)

/////////////////////////////////////////////////////////////////////////////
// CStldStmpDlg dialog


CStldStmpDlg::CStldStmpDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldStmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldStmpDlg)
	//}}AFX_DATA_INIT
}


void CStldStmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldStmpDlg)
	DDX_Control(pDX, IDC_TM_LIST, m_List);
	DDX_Control(pDX, IDC_TM_T1_TEMP_UNIT, m_wndT1Unit);
	DDX_Control(pDX, IDC_TM_T1_TEMP_EDIT, m_wndT1Edit);
	DDX_Control(pDX, IDC_TM_T0_TEMP_UNIT, m_wndT0Unit);
	DDX_Control(pDX, IDC_TM_T0_TEMP_EDIT, m_wndT0Edit);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CStldStmpDlg implementation

void CStldStmpDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Load_Case), _LS(IDS_WG_TREEMENU_Temperature),
											_LS(IDS_TMWK__MSG176)};
	int nColWidth[] = {70,81,80};
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < sizeof(nColWidth)/sizeof(int); i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CStldStmpDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountStmp();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;

	if(nItemCount)
	{
		T_STMP_K Key;
		T_STMP_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartStmp();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextStmp(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		//qsort(KeyBuf, nItemCount, sizeof(T_STMP_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nItemCount-1-nCount];
			m_pDoc->m_pAttrCtrl->GetStmp(Key,Data);
			m_List.InsertItem(nCount, _T(""));
			SetItem(nCount, Key, Data);
		}
		delete []KeyBuf;
	}
}

CString CStldStmpDlg::DataToStr(int i, T_STMP_K key, T_STMP_D &data)
{
	CString value;
	T_STLD_D LoadData;
	if (i == 0) 
	{
		//m_pDoc->m_pAttrCtrl->GetStld((T_STLD_K)key, LoadData);
		m_pDoc->m_pAttrCtrl->GetStld(data.LoadCaseKey, LoadData);
		value = LoadData.LoadCaseName;
	}
	else if (i == 1) value.Format(STMP_DATA_FMT, data.dblSysTemp);
	else if (i == 2)
	{
		T_LDGR_D LdgrD;
		m_pDoc->m_pAttrCtrl->GetLdgr(data.GroupKey,LdgrD);
		value = LdgrD.GroupName;
		//value = data.GroupKey
	}
	return value;
}

BOOL CStldStmpDlg::GetSelectedItem(T_STMP_K &key)
{
	/*
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	CString sLoadCaseName = m_List.GetItemText(iItem, 0);
	key = (T_STMP_K)m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);

	return TRUE;
	*/
	return GetSelectedSTMP(key);
}

void CStldStmpDlg::SetItem(int nIndex, T_STMP_K key, T_STMP_D &data)
{
	LVITEM lvitem;
	CString str;

	for (int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, key, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

	  m_List.SetItem(&lvitem);
		m_List.SetItemData(nIndex,(DWORD)key);
		str.ReleaseBuffer();
	}
}

BOOL CStldStmpDlg::ValidItem(T_STMP_K key, T_STMP_D& data)
{
	if (key <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Invalid_loadcase_));
		return FALSE;
	}

	return TRUE;
}

BOOL CStldStmpDlg::Dlg2Item(T_STMP_K &key, T_STMP_D &data)
{
	UINT nLoadCaseType;
	UINT nLoadCaseKey;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Loadcase_is_not_selected_));
		return FALSE;
	}
	//key = nLoadCaseKey;
	data.LoadCaseKey = nLoadCaseKey;

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	data.GroupKey = LdgrK;

	CString csTemperature;
	m_wndT1Edit.GetWindowText(csTemperature);
	if(!GetFloatNumber(csTemperature, data.dblSysTemp)) return FALSE;

	return TRUE;
}

BOOL CStldStmpDlg::Item2Dlg(T_STMP_K key, T_STMP_D &data)
{
	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, data.LoadCaseKey);
	CString csTemperature;
	csTemperature.Format(STMP_DATA_FMT, data.dblSysTemp);
	m_wndT1Edit.SetWindowText(csTemperature);

	UpdateData(FALSE);
	return TRUE;
}

BEGIN_MESSAGE_MAP(CStldStmpDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldStmpDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_PROJ_CONF_BTN, OnTmProjConfBtn)
	ON_BN_CLICKED(IDC_TM_ADD, OnTmAdd)
	ON_BN_CLICKED(IDC_TM_MODIFY, OnTmModify)
	ON_BN_CLICKED(IDC_TM_DELETE_2, OnTmDelete)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_LIST, OnItemchangedTmList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldStmpDlg message handlers

BOOL CStldStmpDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndT0Unit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	m_wndT1Edit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	m_wndT1Unit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	
	// Set Default Values
	m_wndT1Edit.SetWindowText(_LS(IDS_WG_TREEMENU_0_0));
	SetListCtrlHeader();  
	MakeItemEx();

	// Enable/Disable/Show/Hide/Align controls
	m_wndT0Edit.SetReadOnly(TRUE);  // Initial Temperature는 Read Only한다.

	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldStmpDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldStmpDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}


void CStldStmpDlg::OnTmProjConfBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_CONFIG,0));		
}

void CStldStmpDlg::OnTmAdd() 
{
	// TODO: Add your control notification handler code here
	T_STMP_K key=1;
	T_STMP_D data;
	data.Initialize();

	if (!Dlg2Item(key, data)) return;
	if (!ValidItem(key, data)) return;

	if (m_pDoc->m_pDataCtrl->AddStmp(data))
	{
		int nIndex = m_pDoc->m_pAttrCtrl->GetCountStmp();
		m_List.SetItemState(nIndex-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CStldStmpDlg::OnTmModify() 
{
	// TODO: Add your control notification handler code here
	T_STMP_K key=1, keyOld;
	T_STMP_D data;
	data.Initialize();

	// Get Old Key
	if (!GetSelectedItem(keyOld))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_No_selected_item_in_the_list__nChoo));
		return;
	}
	int nIndex = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	ASSERT(nIndex != -1);

	// Get new key and data
	if (!Dlg2Item(key, data)) return;
	if (!ValidItem(key, data)) return;
 
	
	if (m_pDoc->m_pDataCtrl->ModifyStmp(keyOld, data))
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CStldStmpDlg::OnTmDelete() 
{
	// TODO: Add your control notification handler code here
	T_STMP_K key;

	if (!GetSelectedItem(key))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_system_temperat));
		return;
	}
	int nIndex = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	ASSERT(nIndex != -1);
	if (m_pDoc->m_pDataCtrl->DelStmp(key))
	{
		if (nIndex >= m_pDoc->m_pAttrCtrl->GetCountStmp()) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CStldStmpDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	int nIndex;
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
		nIndex = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		MakeItemEx(); // Unit Change시 MakeItemEx를 한다.
		if (nIndex >= 0)
			m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		break;
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
		MakeItemEx();
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

void CStldStmpDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bUpdate = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case UR_STLD_ADD: case UR_STLD_DEL: case UR_STLD_MFD: case UR_STLD_MFS:
		case UR_STMP_ADD: case UR_STMP_DEL:
				bUpdate = TRUE;
		default:
			break;
		}
	} // end of while

	// STMP의 변화는 MakeItemEx로 다시 새로 만들어 버린다.
	if (bUpdate) MakeItemEx();
}

BOOL CStldStmpDlg::ExternalInit(UINT key)
{
/*
	if(key == 0) return FALSE;

	T_STMP_D data;

	m_pDoc->m_pAttrCtrl->GetStmp(key, data);
	Item2Dlg(key, data);
*/
	return TRUE;
}

void CStldStmpDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

void CStldStmpDlg::OnItemchangedTmList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	/*
	// TODO: Add your control notification handler code here

	T_BODF_K key;
	T_BODF_D data;
	if (!GetSelectedBodyForce(key))
	{
		data.SX = 0.0; data.SY = 0.0; data.SZ = 0.0;
	}
	else
	{
		if (!m_pDoc->m_pAttrCtrl->GetBodf(key, data)) return;
		// Load case 선택을 변경한다.
		int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
		CString sLoadCaseName = m_List.GetItemText(iItem, 0);
		//m_LoadCase.SelectString(-1, sLoadCaseName);
		T_BODF_K nLoadCaseKey = m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);
		m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, nLoadCaseKey);
		m_wndLdgrCobx.ChangeSelect(data.GroupKey);
	}
	// 값을 설정한다.
	CString sValue;
	sValue.Format(BF_DATA_FMT, data.SX);
	m_FactorX.SetWindowText(sValue);
	sValue.Format(BF_DATA_FMT, data.SY);
	m_FactorY.SetWindowText(sValue);
	sValue.Format(BF_DATA_FMT, data.SZ);
	m_FactorZ.SetWindowText(sValue);
	*/
	T_STMP_K Key;
	T_STMP_D Data;

	if (!this->GetSelectedSTMP(Key))
	{
		Data.Initialize();
	}
	else
	{
		if(!m_pDoc->m_pAttrCtrl->GetStmp(Key,Data)) return;
		int iItem = m_List.GetNextItem(-1,LVNI_ALL| LVNI_SELECTED);
		CString sLoadCaseName = m_List.GetItemText(iItem, 0);
		T_STLD_K nLoadCaseKey = m_pDoc->m_pAttrCtrl->GetStldKey(sLoadCaseName);
		this->m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, nLoadCaseKey);
		this->m_wndLdgrCobx.ChangeSelect(Data.GroupKey);
	}

	//m_wndT1Edit.SetEditText(Data.dblSysTemp);
	CString csTemperature;
	csTemperature.Format(STMP_DATA_FMT, Data.dblSysTemp);
	m_wndT1Edit.SetWindowText(csTemperature);

	*pResult = 0;
}

BOOL CStldStmpDlg::GetSelectedSTMP(T_STMP_K &key)
{
	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	key = (T_STMP_K)m_List.GetItemData(iItem);
	
	return TRUE;
}
