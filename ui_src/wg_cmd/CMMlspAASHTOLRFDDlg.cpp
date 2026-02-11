#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMlspAASHTOLRFDDlg.h"
#include "CMStrtGrupDefDlg.h"
#include "ExportFunc.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMMlspAASHTODlg::CCMMlspAASHTODlg()
	: CCMChildBarBase(CCMMlspAASHTODlg::IDD)
{
	m_nEType = 0;
	m_nPosition = 0;
	m_bFoldState = FALSE;
	m_bFirstInit = TRUE;
	m_bFoldInitialize = FALSE;

	m_aCtrlGridGrp.RemoveAll();
	m_aCtrlUserInputID.RemoveAll();
	m_aCtrlElemType.RemoveAll();

	//User Input Mode Control ID
	m_aCtrlUserInputID.Add(IDC_CMD_SUPPORT_POSITION_STC);
	m_aCtrlUserInputID.Add(IDC_CMD_SUP_POS_BOTH);
	m_aCtrlUserInputID.Add(IDC_CMD_SUP_POS_ENDI);
	m_aCtrlUserInputID.Add(IDC_CMD_SUP_POS_ENDJ);
	m_aCtrlUserInputID.Add(IDC_CMD_LIST);
	m_aCtrlUserInputID.Add(IDC_CMD_USER_ADD);
	m_aCtrlUserInputID.Add(IDC_CMD_USER_DELETE);

	//Auto Mode Control ID
	m_aCtrlGridGrp.Add(IDC_CMD_GIRDER_GROUP_STC);
	m_aCtrlGridGrp.Add(IDC_CMD_GIRD_GRP_CMB);
	m_aCtrlGridGrp.Add(IDC_CMD_GIRD_GRP_BTN);

	//Element Type (Beam or Plate)
	m_aCtrlElemType.Add(IDC_CMD_ETYPE_BEAM);
	m_aCtrlElemType.Add(IDC_CMD_ETYPE_PLATE);
}

CCMMlspAASHTODlg::~CCMMlspAASHTODlg()
{
}

void CCMMlspAASHTODlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMlspAASHTODlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_AUTO_LST    , m_lstAuto);
	DDX_Control(pDX, IDC_USER_INPUT_FRM  , m_frmUserInput);
	DDX_Control(pDX, IDC_AUTO_INPUT_FRM  , m_frmAutoInput);
	DDX_Control(pDX, IDC_CMD_GIRD_GRP_CMB, m_cbxGirderGrp);
	DDX_Radio(pDX, IDC_CMD_ETYPE_BEAM    , m_nEType);
	DDX_Radio(pDX, IDC_CMD_SUP_POS_BOTH  , m_nPosition);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMlspAASHTODlg, CCMChildBarBase)
	ON_BN_CLICKED(IDC_CMD_USER_ADD         ,  OnCmdUserAdd)
	ON_BN_CLICKED(IDC_CMD_USER_DELETE      ,  OnCmdUserDel)
	ON_BN_CLICKED(IDC_CMD_AUTO_ADD_BTN     ,  OnCmdAutoAdd)
	ON_BN_CLICKED(IDC_CMD_AUTO_DEL_BTN     ,  OnCmdAutoDel)
	ON_BN_CLICKED(IDC_CMD_ETYPE_BEAM       ,  OnCmdEtype)
	ON_BN_CLICKED(IDC_CMD_ETYPE_PLATE      ,  OnCmdEtype)
	ON_BN_CLICKED(IDC_CMD_GIRD_GRP_BTN     ,  OnCmdGrpTableBtn)
	ON_CBN_SELCHANGE(IDC_CMD_GIRD_GRP_CMB  ,  OnCmdGridGrpCmb )
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST,  OnClicklist)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

void CCMMlspAASHTODlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == nullptr || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
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
		break;
	}
}

void CCMMlspAASHTODlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MLSP_K Key;
	BOOL bMlspChanged = FALSE;
	BOOL bMlspChangedAuto = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MLSP_ADD):
			{
				pViewBuff->GetMlsp(nKey, Key);
				InsertItem(Key);
				if(Key.key.serial==0) bMlspChanged = TRUE;
				else bMlspChangedAuto = TRUE;
			}
			break;
		case(UR_MLSP_DEL):
			{
				pViewBuff->GetMlsp(nKey, Key);
				DeleteItem(Key);
				if(Key.key.serial==0) bMlspChanged = TRUE;
				else bMlspChangedAuto = TRUE;
			}
			break;
		default:
			break;
		}
	} // end of while
	if (bMlspChanged)
	{
		CString csNo;
		int nCount = m_List.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			csNo.Format(_T("%d"), i+1);
			m_List.SetItemText(i, 0, csNo);
		}
	}
	if (bMlspChangedAuto)
	{
		CString csNo;
		int nCount = m_lstAuto.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			csNo.Format(_T("%d"), i+1);
			m_lstAuto.SetItemText(i, 0, csNo);
		}
	}
}

void CCMMlspAASHTODlg::CtrlMan()
{
	UpdateData();

	CArray<UINT, UINT> TypeCtrls;
	CArray<UINT, UINT> PosCtrls;

	TypeCtrls.RemoveAll();
	PosCtrls.RemoveAll();

	TypeCtrls.Add(IDC_CMD_ETYPE_BEAM );
	TypeCtrls.Add(IDC_CMD_ETYPE_PLATE);
	TypeCtrls.Add(IDC_CMD_SUPPORT_POSITION_STC);
	PosCtrls.Add(IDC_CMD_SUP_POS_BOTH);
	PosCtrls.Add(IDC_CMD_SUP_POS_ENDI);
	PosCtrls.Add(IDC_CMD_SUP_POS_ENDJ);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlElemType, m_nEType);
	CDlgUtil::CtrlEnableDisable(this, TypeCtrls, TRUE);
	if (m_nEType == 0) // Beam 
		CDlgUtil::CtrlEnableDisable(this, PosCtrls, TRUE);
	else              // Plate
		CDlgUtil::CtrlEnableDisable(this, PosCtrls, FALSE);
}

#define COLCOUNT 4
//--------------------------------------------------------------------------
// Implementations
void CCMMlspAASHTODlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_CMD_MLSP_no), _LS(IDS_CMD_MLSP_element_no), _LS(IDS_CMD_MLSP_type),
											 _LS(IDS_CMD_MLSP_position) };
	int nColWidth[] = {40, 45, 45, 60};
	int nAlign[] = {LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_CENTER};
	int nColNum = sizeof(nColWidth)/sizeof(int);
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
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}

	//Auto Mode의 리스트는 여기부터...
	CString aAutoTitle[] = { _LS(IDS_CMD_MLSP_no), _LS(IDS_CMD_PJST_Group)};
	int nAutoColWidth[] = {45, 110};
	int nAutoAlign[] = {LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_LEFT};
	nColNum = sizeof(nAutoColWidth)/sizeof(int);
	
	// set full row select mode
	dwStyle = ListView_GetExtendedListViewStyle(m_lstAuto.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstAuto.GetSafeHwnd(), dwStyle);
	
	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aAutoTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAutoAlign[i];
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nAutoColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstAuto.InsertColumn(i,&lvcolumn);
		m_lstAuto.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMlspAASHTODlg::SetItem(int nIndex, T_MLSP_K Key)
{
	ASSERT(Key.key.serial==0);
	CString str;

	for (int i = 0; i < COLCOUNT; i++)
	{
		str = DataToStr(i, nIndex+1, Key);
		m_List.SetItemText(nIndex, i, str);
	}
	m_List.SetItemData(nIndex, Key.keymap); 
}

void CCMMlspAASHTODlg::SetGirderItem(int nIndex, T_MLSP_K Key)
{
	ASSERT(Key.key.serial==1);
	CString str;

	T_GRUP_D GrupD;   GrupD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetGrup(Key.key.entity, GrupD)) {ASSERT(0); return;}
	
	for (int i = 0; i < 2; i++)
	{
		if (i==0)
		{
			str.Format(_T("%d"), nIndex+1);
			m_lstAuto.SetItemText(nIndex, i, str);
		}
		else m_lstAuto.SetItemText(nIndex, i, GrupD.GroupName);
	}
	m_lstAuto.SetItemData(nIndex, Key.keymap); 
}

void CCMMlspAASHTODlg::InsertItem(T_MLSP_K Key)
{
	int nSize = 0;
	int nCount =0;
	// 삽입 위치를 찾는다. 
	if(Key.key.serial==0) // User
	{
		nSize = m_List.GetItemCount();
		for (nCount = 0; nCount < nSize; nCount++)
		{
			if (m_List.GetItemData(nCount) > Key.keymap) break;
		}
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, Key);
	}
	else // Auto
	{
		nSize = m_lstAuto.GetItemCount();
		for (nCount = 0; nCount < nSize; nCount++)
		{
			if (m_lstAuto.GetItemData(nCount) > Key.keymap) break;
		}
		m_lstAuto.InsertItem(nCount, _T(""));
		SetGirderItem(nCount, Key);
	}
}

void CCMMlspAASHTODlg::DeleteItem(T_MLSP_K Key)
{
	int nSize=0, nCount=0;
	
	CListCtrl* pList = ( Key.key.serial==0 ? &m_List : &m_lstAuto);
	
	nSize = pList->GetItemCount();
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (pList->GetItemData(nCount) == Key.keymap) break;
	}
	if (nCount == nSize) GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Key__d__not_found_), Key);
	else pList->DeleteItem(nCount);
}

void CCMMlspAASHTODlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	m_lstAuto.DeleteAllItems();

	POSITION pos;
	int nCount=0;

	BOOL bState = m_frmAutoInput.GetFoldState();

	if (!bState) // AutoMode  MNET:3021-JHLEE-20071010
	{
		int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMlsp();
		if(nItemCount == 0) return;

		if(nItemCount)
		{
			T_MLSP_K Key;
			T_MLSP_D Data;
			
			int* KeyBuf=new int[nItemCount];
			pos = m_pDoc->m_pAttrCtrl->GetStartMlsp();
			while(pos !=NULL)
			{
				m_pDoc->m_pAttrCtrl->GetNextMlsp(pos, Key, Data);
				if(Key.key.serial != 1) {--nItemCount; continue;}
				KeyBuf[nCount++] = Key.keymap;
			}
			if(nItemCount == 0) {delete []KeyBuf; return;}
			qsort(KeyBuf, nItemCount, sizeof(T_MLSP_K), CCompFunc::UINTAsc);
			
			for(nCount = 0 ; nCount < nItemCount ; nCount++)
			{
				Key.keymap=KeyBuf[nCount];
				m_lstAuto.InsertItem(nCount, _T(""));
				SetGirderItem(nCount, Key);
			}    
			delete []KeyBuf;
		}
	}
	else  // UserMode
	{ 
		int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMlsp();
		if(nItemCount == 0) return;
		
		if(nItemCount)
		{
			T_MLSP_K Key;
			T_MLSP_D Data;
			
			int* KeyBuf=new int[nItemCount];
			pos = m_pDoc->m_pAttrCtrl->GetStartMlsp();
			while(pos !=NULL)
			{
				m_pDoc->m_pAttrCtrl->GetNextMlsp(pos, Key, Data);
				if(Key.key.serial != 0) {--nItemCount; continue;}
				KeyBuf[nCount++] = Key.keymap;
			}
			if(nItemCount == 0){delete []KeyBuf; return;}
			qsort(KeyBuf, nItemCount, sizeof(T_MLSP_K), CCompFunc::UINTAsc);
			
			for(nCount = 0 ; nCount < nItemCount ; nCount++)
			{
				Key.keymap=KeyBuf[nCount];
				m_List.InsertItem(nCount, _T(""));
				SetItem(nCount, Key);
			}   
			delete []KeyBuf;
		}
	}
}

CString CCMMlspAASHTODlg::DataToStr(int i, int nNo, T_MLSP_K key)
{
	ASSERT(key.key.serial==0);

	CString value;
	if (i == 0) 
	{
		value.Format(_T("%d"), nNo);
	}
	else if (i == 1) 
	{
		value.Format(_T("%d"), key.key.entity);
	}
	else if (i == 2)
	{
		T_ELEM_D elem;
		m_pDoc->m_pAttrCtrl->GetElem(key.key.entity, elem);
		if (m_pDoc->m_pAttrCtrl->IsBeam(elem.eltyp)) 
			value = _LS(IDS_CMD_MLSP_beam);
		else 
			value = _LS(IDS_CMD_MLSP_plate);
	}
	else if (i == 3)
	{
		T_MLSP_D MlspD;
		if(m_pDoc->m_pAttrCtrl->GetMlsp(key,MlspD))
		{
			if(0 == MlspD.nPosition)
			{
				T_ELEM_D elem;
				m_pDoc->m_pAttrCtrl->GetElem(key.key.entity, elem);
				if (m_pDoc->m_pAttrCtrl->IsBeam(elem.eltyp))
					value = _LS(IDS_CMD_MLSP_both);
				else
					value = _T("");
			}
			else if( 1 == MlspD.nPosition)
			{
				//value = _T("End-I");
				value = _LS(IDS_CMD_MLSP_End_I);
				
			}
			else if( 2 == MlspD.nPosition)
			{
				//value = _T("End-J");
				value = _LS(IDS_CMD_MLSP_End_J);
			}
			else
			{
				value = _LS(IDS_CMD_MLSP_error);
			}
		}
		else
		{
			value = _LS(IDS_CMD_MLSP_error);
		}
	}
	else 
	{
		value = _LS(IDS_CMD_MLSP_error);
	}
	return value;
}

void CCMMlspAASHTODlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;

	// Update all of the selected items.
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i=0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCMMlspAASHTODlg diagnostics

#ifdef _DEBUG
void CCMMlspAASHTODlg::AssertValid() const
{
	CCMChildBarBase::AssertValid();
}

void CCMMlspAASHTODlg::Dump(CDumpContext& dc) const
{
	CCMChildBarBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCMMlspAASHTODlg message handlers

BOOL CCMMlspAASHTODlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CCMChildBarBase::OnInitDialog();

	if (!m_bFirstInit) 
	{   
		m_frmAutoInput.ReInit();
		m_frmUserInput.ReInit();
	}
	m_bFoldInitialize = TRUE;

	m_frmUserInput.InitControl(this, CCMMlspAASHTODlg::IDD, IDC_USER_INPUT_FRM, TRUE, FALSE, 3, 4);
	m_frmAutoInput.InitControl(this, CCMMlspAASHTODlg::IDD, IDC_AUTO_INPUT_FRM, TRUE, FALSE, 3, 4);

	// ListCtrl의 헤더를 설정한다.
	SetListCtrlHeader();
	// ListCtrl의 값을 설정
	MakeItemEx();
	UpdateData(TRUE);

	CtrlMan();
	m_bFirstInit = FALSE;
	m_bFoldState = m_frmAutoInput.GetFoldState();
	m_frmUserInput.SetFoldState(!m_bFoldState);
	return TRUE;
}

void CCMMlspAASHTODlg::OnCmdClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMMlspAASHTODlg::OnClicklist(NMHDR* pNMHDR, LRESULT* pResult)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	
	CArray<T_ELEM_K, T_ELEM_K> aKey;
	CArray<int, int> aSelItem;
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	for (int i = 0; i < nSize; i++)
	{
		aKey.Add(m_List.GetItemData(aSelItem[i]));
	}
	
	pIGM->UnselectAllElem();
	pIGM->AddSelectedElemKeyList(aKey);
}

BOOL CCMMlspAASHTODlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE && GetFocus() == &m_List)
		{
			OnDelKeyPressedInList();
			return TRUE;
		}
		else if (pMsg->wParam == VK_DELETE && GetFocus() == &m_lstAuto)
		{
			OnDelKeyPressedInListAuto();
			return TRUE;
		}
	}
	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

void CCMMlspAASHTODlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CCMChildBarBase::PostNcDestroy();
}

void CCMMlspAASHTODlg::OnCmdEtype() 
{
	CtrlMan();
}

void CCMMlspAASHTODlg::OnDelKeyPressedInList()
{
	CArray<T_MLSP_K, T_MLSP_K> aKey;
	CArray<int, int> aSelItem;
	int nSize = 0;

	GetSelectedItemList(&m_List, aSelItem);
	nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	T_MLSP_K key;
	for (int i = 0; i < nSize; i++)
	{
		key.keymap = m_List.GetItemData(aSelItem[i]);
		aKey.Add(key);
	}
	
	m_pDoc->m_pDataCtrl->DelMlsp(aKey);
}

void CCMMlspAASHTODlg::OnDelKeyPressedInListAuto()
{
	CArray<T_MLSP_K, T_MLSP_K> aKey;
	CArray<int, int> aSelItem;
	int nSize = 0;

	GetSelectedItemList(&m_lstAuto, aSelItem);
	nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	T_MLSP_K key;
	for (int i = 0; i < nSize; i++)
	{
		key.keymap = m_lstAuto.GetItemData(aSelItem[i]);
		aKey.Add(key);
	}
	
	m_pDoc->m_pDataCtrl->DelMlsp(aKey);
}

void CCMMlspAASHTODlg::OnCmdGrpTableBtn()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMStrtGrupDefDlg::IDD);
}

void CCMMlspAASHTODlg::OnCmdGridGrpCmb()
{
	if (m_cbxGirderGrp.GetCount() == 0) { return; }
	
	int nOption = 0;
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlElemType, nOption);

	T_GRUP_K GrupK = NULL;
	if (!m_cbxGirderGrp.GetSelectedGrup(GrupK)) {ASSERT(0); return;}
	
	T_GRUP_D GrupD;   GrupD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD)) {ASSERT(0); return;}

	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE,FALSE);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, GrupD.arKeyElem, FALSE, TRUE, TRUE);
}

LRESULT CCMMlspAASHTODlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	if (!m_bFoldInitialize) return 0L;   // FoldGroupBox가 초기화 되기전(ParentWnd가 초기화 되지 않은 상태)에서
																			 // SetFoldState를 호출되는것을 막기위해 사용 MNET:3021-JHLEE-20071009
	static BOOL bLock = FALSE;   // SendMessage에 의한 무한 루프를 막기 위해서
	if (!bLock)
	{
		bLock = TRUE;

		m_bFoldState = !m_bFoldState;

		m_frmAutoInput.SetFoldState(m_bFoldState);
		m_frmUserInput.SetFoldState(!m_bFoldState);

		MakeItemEx();

		bLock = FALSE;
	}  

	return 0L;
}

void CCMMlspAASHTODlg::OnCmdAutoAdd()
{
	T_GRUP_K GrupK = NULL;
	if (!m_cbxGirderGrp.GetSelectedGrup(GrupK)) {return;}

	T_MLSP_D MlspD;
	MlspD.Initialize();

	T_MLSP_K MlspK;
	MlspK.key.serial = 1;
	MlspK.key.entity = GrupK;

	m_pDoc->m_pDataCtrl->AddMlsp(MlspK,MlspD);
} 

void CCMMlspAASHTODlg::OnCmdAutoDel()
{
	CArray<int, int> aSelItem;
	int nSize = 0;
	
	GetSelectedItemList(&m_lstAuto, aSelItem);
	nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	T_MLSP_K MlspK;
	for (int i = 0; i < nSize; i++)
	{
		MlspK.keymap = m_lstAuto.GetItemData(aSelItem[i]);
		m_pDoc->m_pDataCtrl->DelMlsp(MlspK);
	}
}

void CCMMlspAASHTODlg::OnCmdUserAdd()
{
	CtrlMan();

	UpdateData(TRUE);

	CArray<T_ELEM_K, T_ELEM_K> aKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(aKey);

	// Remove element which is not beam type
	T_ELEM_D ElemData; ElemData.Initialize();

	for (int i = aKey.GetSize()-1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aKey.GetAt(i), ElemData);
		if (!((m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)&& m_nEType == 0)||
			(m_pDoc->m_pAttrCtrl->IsPlate(ElemData.eltyp)&& m_nEType == 1))) aKey.RemoveAt(i);
	}

	if (aKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_beam_and_plat));
		return;
	}

	BOOL bSuccess = FALSE;
	T_MLSP_D MlspD; MlspD.Initialize();
	MlspD.nPosition = m_nPosition;
	T_MLSP_K MlspK;
	MlspK.key.serial = 0;
	for (int i = 0; i<aKey.GetSize(); ++i)
	{
		MlspK.key.entity = aKey[i];
		bSuccess = m_pDoc->m_pDataCtrl->AddMlsp(MlspK, MlspD);
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CCMMlspAASHTODlg::OnCmdUserDel()
{
	CtrlMan();
	UpdateData(TRUE);

	CArray<T_ELEM_K, T_ELEM_K> aKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyList(aKey);

	// Remove element which is not beam type
	T_ELEM_D ElemData; ElemData.Initialize();

	if (aKey.GetSize() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_beam_and_plat));
		return;
	}

	BOOL bSuccess = FALSE;
	T_MLSP_D MlspD; MlspD.Initialize();
	MlspD.nPosition = m_nPosition;

	T_MLSP_K MlspK;
	MlspK.key.serial = 0;
	for (int i = 0; i<aKey.GetSize(); ++i)
	{
		MlspK.key.entity = aKey[i];
		bSuccess = m_pDoc->m_pDataCtrl->DelMlsp(MlspK);
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
