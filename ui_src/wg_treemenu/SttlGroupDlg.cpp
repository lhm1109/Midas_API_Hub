// SttlGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SttlGroupDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSttlGroupDlg dialog


CSttlGroupDlg::CSttlGroupDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSttlGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSttlGroupDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CSttlGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSttlGroupDlg)
	DDX_Control(pDX, IDC_TM_STTL_GROUP_DISP_UNIT, m_SetlDispUnit);
	DDX_Control(pDX, IDC_TM_STTL_GROUP_NAME_EDIT, m_edtGroupName);
	DDX_Control(pDX, IDC_TM_STTL_GROUP_DISP_EDIT, m_edtSetlDisp);
	DDX_Control(pDX, IDC_TM_STTL_GROUP_LOADLIST_EDIT, m_edtNodeList);
	DDX_Control(pDX, IDC_TM_STTL_GROUP_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSttlGroupDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSttlGroupDlg)
	ON_BN_CLICKED(IDC_TM_STTL_GROUP_CLOSE, OnTmSttlGroupClose)
	ON_BN_CLICKED(IDC_TM_STTL_GROUP_ADD, OnTmSttlGroupAdd)
	ON_BN_CLICKED(IDC_TM_STTL_GROUP_MODIFY, OnTmSttlGroupModify)
	ON_BN_CLICKED(IDC_TM_STTL_GROUP_DELETE, OnTmSttlGroupDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_STTL_GROUP_LIST, OnItemchangedTmSttlGroupList)
	ON_EN_CHANGE(IDC_TM_STTL_GROUP_LOADLIST_EDIT, OnChangeTmSttlGroupLoadlistEdit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSttlGroupDlg message handlers

void CSttlGroupDlg::OnTmSttlGroupClose() 
{
	CloseDlg();
}

BOOL CSttlGroupDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SetlDispUnit.SetUnitType(m_SmptUnit.dSettle);
	m_edtNodeList.Connect(SC_ID_NODE, &m_edtNodeList);
	m_edtSetlDisp.SetAttUcsDistance();
	m_edtSetlDisp.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_edtSetlDisp.SetDistValueMode(TRUE);

	m_edtSetlDisp.SetWindowText(_LS(IDS_WG_TREEMENU_0_0));

	InitListTitle();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define COLCOUNT 3
void CSttlGroupDlg::InitListTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(m_SmptUnit.dSettle, csUnit); 

	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Name),_LS(IDS_WG_TREEMENU_Disp___)+csUnit+_T(")"),_LS(IDS_WG_TREEMENU_Node_List)};
	int nColWidth[COLCOUNT]={60,80,120};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}

	if(m_pDoc->m_pAttrCtrl->GetCountSmpt())
	{
		InitSmptData();
//	  m_List.SetItemState(0,  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CSttlGroupDlg::InitSmptData()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSmpt();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	CString strDist;
	int nCount;

	if(nItemCount)
	{
		T_SMPT_K Key;
		T_SMPT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSmpt();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextSmpt(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_SMPT_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSmpt(Key,Data);

			int nIndex = m_List.InsertItem(nCount, Data.GroupName);
			strDist.Format(_LS(IDS_WG_TREEMENU__g), Data.dSettle);
			m_List.SetItemText(nIndex,1, strDist);

//		qsort((void*)m_SmptData.aNodeData.GetData(), m_SmptData.aNodeData.GetSize(), sizeof(T_NODE_K), CCompFunc::UINTAsc);
			CString strNodeList;
			CNumericOptimizer optimizer;
			strNodeList = optimizer.Optimize((long*)Data.aNodeData.GetData(),Data.aNodeData.GetSize());
			m_List.SetItemText(nIndex, 2, strNodeList);		
		}
		delete []KeyBuf;
	}
}

void CSttlGroupDlg::OnTmSttlGroupAdd() 
{
	CString strDist;
	CString strGroupName;	
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	m_edtGroupName.GetWindowText(strGroupName);

	if(strGroupName == _T(""))
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_defined_));
		return;
	}

	m_edtSetlDisp.GetWindowText(strDist);
	
	m_SmptData.Initialize();

	CString strNodeList;
	m_edtNodeList.GetWindowText(strNodeList);
	m_edtNodeList.SelectByStr(strNodeList);

	pIGM->GetSelectedNodeKeyList(m_SmptData.aNodeData);
	if(!m_SmptData.aNodeData.GetSize())
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}
	
	m_SmptData.GroupName = strGroupName;
	if(!GetFloatNumber(strDist, m_SmptData.dSettle)) return;
		
	if(m_pDoc->m_pDataCtrl->AddSmpt(m_SmptData))
	{
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		m_edtGroupName.SetWindowText(_T(""));
//		m_edtSetlDisp.SetWindowText(_LS(IDS_WG_TREEMENU_0_0));
		m_edtNodeList.SetWindowText(_T(""));
		m_edtGroupName.SetFocus();
//		m_List.SetItemState(m_List.GetItemCount()-1,  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CSttlGroupDlg::OnTmSttlGroupModify() 
{
	if(GetCurrentRow() < 0)
	{
	  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_selected));
		return;
	}

	CString strDist;
	CString strGroupName;
	CString strOldGroupName;
	CString strNodeList;
	T_SMPT_D SmptOldData;

	m_edtGroupName.GetWindowText(strGroupName);
	m_edtSetlDisp.GetWindowText(strDist);
	m_edtNodeList.GetWindowText(strNodeList);

	m_SmptData.Initialize();

	m_edtNodeList.SelectByStr(strNodeList);

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(m_SmptData.aNodeData);

	if(!m_SmptData.aNodeData.GetSize())
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}
	
	m_SmptData.GroupName = strGroupName;
	if(!GetFloatNumber(strDist, m_SmptData.dSettle)) return;

	m_pDoc->m_pAttrCtrl->GetSmpt(GetCurrentSmptKey(), SmptOldData);

	m_SmptData.GroupId = SmptOldData.GroupId;

	strOldGroupName = GetCurrentSmptKeyName();

	if(m_pDoc->m_pDataCtrl->ModifySmpt(strOldGroupName, m_SmptData))
	{
		m_edtGroupName.SetFocus();
		m_List.SetItemState(GetCurrentRow(),  LVIS_FOCUSED|LVIS_SELECTED , LVIS_FOCUSED|LVIS_SELECTED); 
	}
}

void CSttlGroupDlg::OnTmSttlGroupDelete() 
{
	if(GetCurrentRow() < 0)
	{
	  GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Group_Name_was_not_selected));
		return;
	}

	int nSelect = GetCurrentRow();
 
	m_pDoc->m_pDataCtrl->DelSmpt(m_List.GetItemText(nSelect, 0));

	if(!m_List.SetItemState(nSelect, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
		if(!m_List.SetItemState(nSelect-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED))
		{
			m_edtSetlDisp.SetWindowText(_LS(IDS_WG_TREEMENU_0_0));
			m_edtGroupName.Clear();
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
			m_List.SetItemState(-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			m_edtSetlDisp.SetFocus();
		}
}

void CSttlGroupDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	 	InitSmptData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		InitSmptData();
		break; 
	case D_UPDATE_UNIT:
		UpdateUnit();
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

void CSttlGroupDlg::UpdateUnit() 
{
	ModifyTitle();
	
	int nUnitType = m_SmptUnit.dSettle;
	double dblDistance;                                 

	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		dblDistance = _tstof(m_List.GetItemText(i,1));
		dblDistance = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitType, dblDistance);
		CString strTemp;
		strTemp.Format(_LS(IDS_WG_TREEMENU___7g),dblDistance);
		m_List.SetItemText(i,1,strTemp);
	}
}

void CSttlGroupDlg::ModifyTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(m_SmptUnit.dSettle, csUnit); 

	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Disp___)+csUnit+_T(")")};
	int nColWidth[1]={80};

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	title = aTitle[0];
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.iSubItem = 1;
	lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[0]);
	lvcolumn.pszText = title.GetBuffer(0);
	m_List.SetColumn(1,&lvcolumn);
	title.ReleaseBuffer();
}

void CSttlGroupDlg::OnItemchangedTmSttlGroupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CString strTemp;

	int nSelect = GetCurrentRow();

	m_SmptKey = m_pDoc->m_pAttrCtrl->GetSmptKey(m_List.GetItemText(nSelect, 0));
	if(m_pDoc->m_pAttrCtrl->GetSmpt(m_SmptKey, m_SmptData))
	{
		m_edtGroupName.SetWindowText(m_SmptData.GroupName);
		strTemp.Format(_LS(IDS_WG_TREEMENU__g), m_SmptData.dSettle);
		m_edtSetlDisp.SetWindowText(strTemp);	
		
		//m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	  CNumericOptimizer optimizer;
	  strTemp = optimizer.Optimize((long*)m_SmptData.aNodeData.GetData(),m_SmptData.aNodeData.GetSize());
	  m_edtNodeList.SelectByStr(strTemp);	
	}	

	*pResult = 0;
}

void CSttlGroupDlg::OnChangeTmSttlGroupLoadlistEdit() 
{
/*	CString strTemp;
	m_edtNodeList.GetWindowText(strTemp);
	m_edtNodeList.SelectByStr(strTemp);  */
}

int CSttlGroupDlg::GetCurrentSmptKey()
{
	int nSelect;

	CString strGroupName;

	nSelect = GetCurrentRow();

	strGroupName = m_List.GetItemText(nSelect, 0);

	return m_pDoc->m_pAttrCtrl->GetSmptKey(strGroupName);		
}

CString CSttlGroupDlg::GetCurrentSmptKeyName()
{
	int nSelect;

	CString strGroupName;

	nSelect = GetCurrentRow();

	strGroupName = m_List.GetItemText(nSelect, 0);

	return strGroupName;		
}

int CSttlGroupDlg::GetCurrentRow()
{
	int nSelect=-1;
	LV_ITEM lvitem;
	int count = m_List.GetItemCount();
	lvitem.mask = LVIF_STATE;

	for(int i=count-1 ; i>=0; i--)
	{
		lvitem.iItem = i;
		if(m_List.GetItemState(i, LVIS_SELECTED) != 0)
		{
			nSelect = i;
			break;
		}
	}
	return nSelect;
}

void CSttlGroupDlg::OnDestroy() 
{
	CMenuBarChildDlg::OnDestroy();
	
	// TODO: Add your message handler code here
}

BOOL CSttlGroupDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_SMPT_D data;
	CString strTemp;

	m_pDoc->m_pAttrCtrl->GetSmpt(key, data);

	m_edtGroupName.SetWindowText(data.GroupName);
	strTemp.Format(_LS(IDS_WG_TREEMENU__g), data.dSettle);
	m_edtSetlDisp.SetWindowText(strTemp);	
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);

	CNumericOptimizer optimizer;
	strTemp = optimizer.Optimize((long*)data.aNodeData.GetData(), data.aNodeData.GetSize());
	m_edtNodeList.SelectByStr(strTemp);

	return TRUE;
}
