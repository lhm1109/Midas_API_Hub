// CMGridSpanLengthDlg.cpp: implementation of the CCMGridSpanLengthDlg class.

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridSpanLengthDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "CMBridgeTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 3
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_DYNCREATE(CCMGridSpanLengthDlg, CCMDlgBarBase)

CCMGridSpanLengthDlg::CCMGridSpanLengthDlg()
	: CCMChildBarBase(CCMGridSpanLengthDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridSpanLengthDlg)
	m_nSpanType=0;
	m_nConnectionType=0;
	//}}AFX_DATA_INIT
	m_Data.Initialize();
	m_nTypeOfBridge = 0;
}

void CCMGridSpanLengthDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridSpanLengthDlg)
	DDX_Radio(pDX, IDC_CMD_GRID_SPAN_TYPE_GIRDER_RDO, m_nSpanType);
	DDX_Radio(pDX, IDC_CMD_GRID_CON_BOTH_FIX_RDO, m_nConnectionType);
	DDX_Control(pDX, IDC_CMD_GRID_SPAN_LENGH_EDT, m_edtLength);
	DDX_Control(pDX, IDC_CMD_GRID_SPAN_LENGH_UNT, m_untLength);
	DDX_Control(pDX, IDC_CMD_GRID_SPAN_LENGH_LOAD_EDT, m_edtLengthLoad);
	DDX_Control(pDX, IDC_CMD_GRID_SPAN_LENGH_LOAD_UNT, m_untLengthLoad);
	DDX_Control(pDX, IDC_CMD_GRID_SPAN_RANGE_START_CMB, m_cmbStart);
	DDX_Control(pDX, IDC_CMD_GRID_SPAN_RANGE_END_CMB, m_cmbEnd);
	DDX_Control(pDX, IDC_CMD_GRID_SPAN_LENGTH_LIST, m_GridList);
	DDX_Text(pDX, IDC_CMD_GRID_BRG_TYPE_STATIC, m_strTypeBrg);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridSpanLengthDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMGridSpanLengthDlg)
	ON_BN_CLICKED(IDC_CMD_GRID_SPAN_TYPE_GIRDER_RDO, OnChangeSpanType)
	ON_BN_CLICKED(IDC_CMD_GRID_SPAN_TYPE_RAHMEN_RDO, OnChangeSpanType)
	ON_BN_CLICKED(IDC_CMD_GRID_SPAN_TYPE_CANTILEVER_RDO, OnChangeSpanType)
	ON_BN_CLICKED(IDC_CMD_GRID_SPAN_TYPE_HANGER_RDO, OnChangeSpanType)
	ON_BN_CLICKED(IDC_CMD_GRID_OPER_ADD_BTN, OnAddBtn)
	ON_BN_CLICKED(IDC_CMD_GRID_OPER_MOD_BTN, OnModBtn)
	ON_BN_CLICKED(IDC_CMD_GRID_OPER_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(IDC_CMD_GRID_TYPE_BRDG_BTN, OnCmdBridgeType)
	ON_EN_CHANGE(IDC_CMD_GRID_SPAN_LENGH_EDT, OnCmdSpanLengthEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_GRID_SPAN_LENGTH_LIST, OnItemchangedSpanLengthList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMGridSpanLengthDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	int nNum = m_GridList.GetItemCount();
	int i = 0;
	for (i=0; i<nNum; i++)
		if (m_GridList.GetItemData(i) == key) break;
	if (i == nNum) return FALSE;

	if (m_pDoc->m_pAttrCtrl->GetBspn(key, m_Data)) 
	{
		m_GridList.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
		Data2Dlg();
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CPushOverHingeAssignDlg message handlers

BOOL CCMGridSpanLengthDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CCMChildBarBase::OnInitDialog();

	GetDlgItem(IDC_CMD_GRID_CLOSE_BTN)->ShowWindow(FALSE);

	// List
	SetHeaderTitle();

	if(m_nTypeOfBridge!=0)
	{
		if(m_nSpanType!=3) m_nConnectionType=0;
		else               m_nConnectionType=1;
	}
	UpdateData(FALSE);
	// Data setting
	Data2Dlg();
	

	MakeItemEx();

	// 단위
	m_edtLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLength.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLengthLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untLengthLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridSpanLengthDlg::OnChangeSpanType()
{
	//if(!m_pDoc->m_pAttrCtrl->ExistBrgt()) return;
	GetDlgItem(IDC_CMD_GRID_SPAN_TYPE_GIRDER_RDO)    ->EnableWindow(m_nTypeOfBridge==1);
	GetDlgItem(IDC_CMD_GRID_SPAN_TYPE_RAHMEN_RDO)    ->EnableWindow(m_nTypeOfBridge==2);
	GetDlgItem(IDC_CMD_GRID_SPAN_TYPE_CANTILEVER_RDO)->EnableWindow(m_nTypeOfBridge!=0);
	GetDlgItem(IDC_CMD_GRID_SPAN_TYPE_HANGER_RDO)    ->EnableWindow(m_nTypeOfBridge!=0);
	GetDlgItem(IDC_CMD_GRID_CON_BOTH_FIX_RDO)        ->EnableWindow(m_nTypeOfBridge!=0);
	GetDlgItem(IDC_CMD_GRID_CON_LEFT_PIN_RDO)        ->EnableWindow(m_nTypeOfBridge!=0);
	GetDlgItem(IDC_CMD_GRID_CON_RIGHT_PIN_RDO)       ->EnableWindow(m_nTypeOfBridge!=0);
	GetDlgItem(IDC_CMD_GRID_CON_BOTH_PIN_RDO)        ->EnableWindow(m_nTypeOfBridge!=0);

	UpdateData(TRUE);
	if(m_nTypeOfBridge==1) // Gerber
	{
		GetDlgItem(IDC_CMD_GRID_CON_BOTH_FIX_RDO) ->EnableWindow(m_nSpanType!=3);
		GetDlgItem(IDC_CMD_GRID_CON_LEFT_PIN_RDO) ->EnableWindow(m_nSpanType!=0);
		GetDlgItem(IDC_CMD_GRID_CON_RIGHT_PIN_RDO)->EnableWindow(m_nSpanType!=0);
		GetDlgItem(IDC_CMD_GRID_CON_BOTH_PIN_RDO) ->EnableWindow(m_nSpanType==1 || m_nSpanType==3);
	}
	else if(m_nTypeOfBridge==2) // Rahmen
	{
		GetDlgItem(IDC_CMD_GRID_CON_BOTH_FIX_RDO) ->EnableWindow(m_nSpanType!=3);
		GetDlgItem(IDC_CMD_GRID_CON_LEFT_PIN_RDO) ->EnableWindow(m_nSpanType>=2);
		GetDlgItem(IDC_CMD_GRID_CON_RIGHT_PIN_RDO)->EnableWindow(m_nSpanType>=2);
		GetDlgItem(IDC_CMD_GRID_CON_BOTH_PIN_RDO) ->EnableWindow(m_nSpanType==3);
	}

	if(m_nTypeOfBridge!=0)
	{
		if(m_nSpanType!=3) m_nConnectionType=0;
		else               m_nConnectionType=1;
		UpdateData(FALSE);
	}
}

void CCMGridSpanLengthDlg::OnAddBtn()
{
	if(!Dlg2Data()) return;
	
	if(!m_pDoc->m_pDataCtrl->AddBspn(m_Data)){ ASSERT(0); return; } 

	int nSel = m_GridList.GetItemCount()-1;
	m_GridList.SetItemState(nSel, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	RefreshRangeCombo();
}

void CCMGridSpanLengthDlg::OnModBtn()
{
	int nItem = m_GridList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	if(!Dlg2Data()) return;
	T_BSPN_K key = m_GridList.GetItemData(nItem);
	if(!m_pDoc->m_pDataCtrl->ModifyBspn(key, m_Data)) { ASSERT(0); return; }

	m_GridList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	RefreshRangeCombo();
}

void CCMGridSpanLengthDlg::OnDelBtn()
{
	int nItem = m_GridList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	T_BSPN_K key = m_GridList.GetItemData(nItem);
	if(!m_pDoc->m_pDataCtrl->DelBspn(key)) ASSERT(0);
	
	int nSel = m_GridList.GetItemCount()>nItem ? nItem:nItem-1;
	m_GridList.SetItemState(nSel, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

}

void CCMGridSpanLengthDlg::OnItemchangedSpanLengthList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nItem = pNMListView->iItem;
	if(nItem<0) return;
	T_BSPN_K BspnK = m_GridList.GetItemData(nItem);

	if(m_pDoc->m_pAttrCtrl->GetBspn(BspnK, m_Data)) Data2Dlg();
	//else ASSERT(0);

	*pResult = 0;
}

void CCMGridSpanLengthDlg::OnCmdBridgeType()
{
	CCMBridgeTypeDlg dlg;
	dlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////////
// Data Change
void CCMGridSpanLengthDlg::Data2Dlg()
{
	m_edtLength.SetEditUnit(m_Data.dSpanLength);
	m_edtLengthLoad.SetEditUnit(m_Data.dLoadingLength);

	m_nSpanType = m_Data.nSpanType;
	m_nConnectionType = m_Data.nConnectionType;

	m_cmbStart.ChangeSelect(m_Data.StartCrossK); // Cross Beam Key
	m_cmbEnd.ChangeSelect(m_Data.EndCrossK);     // Cross Beam Key

	//if(m_pDoc->m_pAttrCtrl->ExistBrgt())
	{
		T_BRGT_D BrgtD; BrgtD.Initialize();
		m_pDoc->m_pAttrCtrl->GetBrgt(BrgtD);
		m_nTypeOfBridge = BrgtD.nTypeOfBridge;

		if     (m_nTypeOfBridge==0) m_strTypeBrg = _LS(IDS_CMD_GRID_GIRDER);
		else if(m_nTypeOfBridge==1) m_strTypeBrg = _LS(IDS_CMD_GRID_GERBER);
		else if(m_nTypeOfBridge==2) m_strTypeBrg = _LS(IDS_CMD_GRID_RAHMEN);
	}
	m_nSpanType = m_Data.nSpanType;
	UpdateData(FALSE);


	OnChangeSpanType();
	m_nConnectionType = m_Data.nConnectionType;
	UpdateData(FALSE);
}

BOOL CCMGridSpanLengthDlg::Dlg2Data()
{
	//if(!m_pDoc->m_pAttrCtrl->ExistBrgt()) return FALSE;

	UpdateData(TRUE);

	m_Data.dSpanLength = m_edtLength.GetEditValue();
	m_Data.dLoadingLength = m_edtLengthLoad.GetEditValue();

	m_Data.nSpanType = m_nSpanType;
	m_Data.nConnectionType = m_nConnectionType;

	m_cmbStart.GetSelectedCbem(m_Data.StartCrossK); // Cross Beam Key
	m_cmbEnd.GetSelectedCbem(m_Data.EndCrossK);     // Cross Beam Key
	 
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// List Control
void CCMGridSpanLengthDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_GRID_START), _LS(IDS_CMD_GRID_END), _LS(IDS_CMD_GRID_LENGTH)};
	int nColWidth[] = {50,50,70};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_GridList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_GridList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i=0; i<3; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_GridList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMGridSpanLengthDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_GridList.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountBspn();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_BSPN_K Key;
		T_BSPN_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartBspn();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextBspn(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetBspn(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

///////////////////////////////////////////////////////////////////////
int CCMGridSpanLengthDlg::FindInsertionPos(int nID)
{
	T_BSPN_K key;
	T_BSPN_D data;
	int nCount = m_GridList.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		key = m_GridList.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetBspn(key, data);
		if (nID < data.nSeq) break;
	}
	return i;    
}

BOOL CCMGridSpanLengthDlg::InsertItem(T_BSPN_K Key, T_BSPN_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_GridList.GetItemCount();

	lvitem.iItem = FindInsertionPos(Data.nSeq);
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i==0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_GridList.InsertItem(&lvitem);
		}
		else m_GridList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMGridSpanLengthDlg::DeleteItem(T_BSPN_K Key, T_BSPN_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_GridList.FindItem(&FindInfo);
	if (nItem != -1) m_GridList.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMGridSpanLengthDlg::ModifyItem(T_BSPN_K KeyOld, T_BSPN_K Key, T_BSPN_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_GridList.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Data);
			m_GridList.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_GridList.SetItem(&lvitem);
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////
CString CCMGridSpanLengthDlg::DataToStr(int i, T_BSPN_D &data)
{
	CString value;
	T_CBEM_D CbemD; CbemD.Initialize();
	if(i==0)
	{
		m_pDoc->m_pAttrCtrl->GetCbem(data.StartCrossK, CbemD);
		value = CbemD.Name;
	}
	else if(i==1)
	{
		m_pDoc->m_pAttrCtrl->GetCbem(data.EndCrossK, CbemD);
		value = CbemD.Name;
	}
	else if(i==2) value.Format(_T("%g"), data.dSpanLength);

	return value;
}

void CCMGridSpanLengthDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		ChangeUnit();
		break;
	default:
		break;
	}
}
void CCMGridSpanLengthDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bMFD=FALSE, bBSPNChange=FALSE, bBRGTChange=FALSE;
	T_UDRD_BUFFER buffer_ur;
	T_BSPN_K Key, KeyBack;
	T_BSPN_D Data, DataBack;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_BRGT_ADD): case(UR_BRGT_DEL):
			bBRGTChange = TRUE;
			break;
		case UR_BSPN_ADD:
			pViewBuff->GetBspn(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case UR_BSPN_DEL:
			pViewBuff->GetBspn(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case UR_BSPN_MFD:
			pViewBuff->GetBspn(nKey, KeyBack, DataBack);
			bMFD=TRUE;
			break;
		case UR_BSPN_MFS:
			ASSERT(bMFD);
			pViewBuff->GetBspn(nKey, Key, Data);
			ModifyItem(Key, KeyBack, DataBack);
			bMFD=FALSE;
			break;
		default:
			break;
		}
	}
	if(bBRGTChange)
	{
		T_BRGT_D BrgtD; BrgtD.Initialize();
		if(m_pDoc->m_pAttrCtrl->ExistBrgt())
		{
			m_pDoc->m_pAttrCtrl->GetBrgt(BrgtD);
		}
		m_nTypeOfBridge = BrgtD.nTypeOfBridge;
		if     (m_nTypeOfBridge==0) m_strTypeBrg = _LS(IDS_CMD_GRID_GIRDER);
		else if(m_nTypeOfBridge==1) { m_strTypeBrg = _LS(IDS_CMD_GRID_GERBER); m_nSpanType = 0; }
		else if(m_nTypeOfBridge==2) { m_strTypeBrg = _LS(IDS_CMD_GRID_RAHMEN); m_nSpanType = 1; }
		UpdateData(FALSE);

		OnChangeSpanType();
	}
}

void CCMGridSpanLengthDlg::ChangeUnit()
{
	int nCount = m_GridList.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString str = m_GridList.GetItemText(i, 2);
		double dLength = _tstof(str);
		/*m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M,
																									D_UNITSYS_BASE_LENGTH, dLength);
		*/
		dLength = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_LENGTH, dLength);

		str.Format(_T("%g"), dLength);
		m_GridList.SetItemText(i, 2, str);
	}
}

void CCMGridSpanLengthDlg::RefreshRangeCombo()
{
	int nIndex = m_cmbEnd.GetCurSel();
	if (nIndex == CB_ERR) nIndex = 0;

	if (nIndex >= m_cmbStart.GetCount()) return;
	if (nIndex >= m_cmbEnd.GetCount()) return;

	m_cmbStart.SetCurSel(nIndex);
	m_cmbEnd.SetCurSel(nIndex);
}

void CCMGridSpanLengthDlg::OnCmdSpanLengthEdit()
{
	double SpanLength = m_edtLength.GetEditValue();
	m_edtLengthLoad.SetEditUnit(SpanLength);
}