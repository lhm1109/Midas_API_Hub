// CMGridP1LoadFactorTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridP1LoadFactorTypeDlg.h"


#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGridP1LoadFactorTypeDlg dialog


CCMGridP1LoadFactorTypeDlg::CCMGridP1LoadFactorTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGridP1LoadFactorTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGridP1LoadFactorTypeDlg)
	m_nLoadItem = 0;
	m_bDxBend = FALSE;
	m_bDyBend = FALSE;
	m_bDzBend = FALSE;
	m_bRxBend = FALSE;
	m_bRyBend = FALSE;
	m_bRzBend = FALSE;
	//}}AFX_DATA_INIT

	m_bModify = FALSE;
	m_Key = 0;
	m_Data.Initialize();

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
}

void CCMGridP1LoadFactorTypeDlg::SetParam(T_P1LT_K key)
{
	m_Key = key;
	if (!m_pDoc->m_pAttrCtrl->GetP1lt(m_Key, m_Data)) ASSERT(0);
	m_bModify = TRUE;
}

void CCMGridP1LoadFactorTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridP1LoadFactorTypeDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_GridList);
	DDX_Control(pDX, IDC_CMD_NAME_EDT, m_wndName);
	DDX_Radio(pDX, IDC_CMD_REACTION, m_nLoadItem);
	DDX_Check(pDX, IDC_CMD_DX1, m_bDxBend);
	DDX_Check(pDX, IDC_CMD_DY1, m_bDyBend);
	DDX_Check(pDX, IDC_CMD_DZ1, m_bDzBend);
	DDX_Check(pDX, IDC_CMD_RX1, m_bRxBend);
	DDX_Check(pDX, IDC_CMD_RY1, m_bRyBend);
	DDX_Check(pDX, IDC_CMD_RZ1, m_bRzBend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridP1LoadFactorTypeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGridP1LoadFactorTypeDlg)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_MODIFY, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_REACTION, OnCmdLoadItem)
	ON_BN_CLICKED(IDC_CMD_DISPLACEMENT, OnCmdLoadItem)
	ON_BN_CLICKED(IDC_CMD_MEMBER_FORCE, OnCmdLoadItem)
	ON_BN_CLICKED(IDC_CMD_DX1, OnCmdDx1)
	ON_BN_CLICKED(IDC_CMD_DY1, OnCmdDy1)
	ON_BN_CLICKED(IDC_CMD_DZ1, OnCmdDz1)
	ON_BN_CLICKED(IDC_CMD_RX1, OnCmdRx1)
	ON_BN_CLICKED(IDC_CMD_RY1, OnCmdRy1)
	ON_BN_CLICKED(IDC_CMD_RZ1, OnCmdRz1)
	ON_BN_CLICKED(IDC_CMD_DX2, OnCmdDx2)
	ON_BN_CLICKED(IDC_CMD_DY2, OnCmdDy2)
	ON_BN_CLICKED(IDC_CMD_DZ2, OnCmdDz2)
	ON_BN_CLICKED(IDC_CMD_RX2, OnCmdRx2)
	ON_BN_CLICKED(IDC_CMD_RY2, OnCmdRy2)
	ON_BN_CLICKED(IDC_CMD_RZ2, OnCmdRz2)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridP1LoadFactorTypeDlg message handlers

BOOL CCMGridP1LoadFactorTypeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitList();
	MakeItemEx();

	if (m_bModify)
	{
		int nItemCount = m_GridList.GetItemCount();
		int i = 0;
		for (i=0; i<nItemCount; i++)
			if (m_GridList.GetItemData(i) == m_Key) break;
		if (i != nItemCount) m_GridList.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	//T_P1LT_D data;
	//data.Initialize();
	Data2Dlg(m_Data);

	SetLabel(m_nLoadItem);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridP1LoadFactorTypeDlg::InitList()
{
	CString aTitle[] = {_LS(IDS_CMD_P1_NAME), _LS(IDS_CMD_P1_ITEM), _LS(IDS_CMD_P1_TYPE)};
	int nColWidth[] = {70,100,70};

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

void CCMGridP1LoadFactorTypeDlg::OnCmdAdd() 
{
	UpdateData();
	CString strName;
	m_wndName.GetWindowText(strName);
	if (strName == _T(""))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___IMPACT_FACTOR_NO_SELECTED));
		return;
	}
	
	T_P1LT_D P1ltD;
	Dlg2Data(P1ltD, m_GridList.GetItemCount() + 1);

	int nCount = m_GridList.GetItemCount();
	if (m_pDoc->m_pDataCtrl->AddP1lt(P1ltD))
		m_GridList.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdModify() 
{
	CString strName;
	int nIndex;
	if (!GetSelectedData(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___CROSS_REFERENCE_NO_SELECTED));
		return;
	}

	T_P1LT_D data;
	Dlg2Data(data, nIndex+1);
	
	strName = m_GridList.GetItemText(nIndex, 0);
	if (m_pDoc->m_pDataCtrl->ModifyP1lt(strName, data))
		m_GridList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMGridP1LoadFactorTypeDlg::OnCmdDelete() 
{
	int nIndex;
	CString strName;
	if (!GetSelectedData(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___CROSS_REFERENCE_NO_SELECTED));
		return;
	}
	int nCount = m_GridList.GetItemCount();
	strName = m_GridList.GetItemText(nIndex, 0);
	if (m_pDoc->m_pDataCtrl->DelP1lt(strName))
	{
		if (nIndex >= nCount-1) nIndex--;
		m_GridList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}

	SetDataSequence();
}

void CCMGridP1LoadFactorTypeDlg::SetDataSequence()
{
	T_P1LT_D data;
	CString strName;
	int nSize = m_GridList.GetItemCount();
	if (nSize < 1)
		return;

	for (int i=0; i<nSize; i++)
	{
		strName = m_GridList.GetItemText(i, 0);
		m_pDoc->m_pAttrCtrl->GetP1lt(strName, data);
		data.nSeq = i+1;
		m_pDoc->m_pDataCtrl->ModifyP1lt(strName, data);
	}
}

BOOL CCMGridP1LoadFactorTypeDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_GridList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMGridP1LoadFactorTypeDlg::Data2Dlg(T_P1LT_D& data)
{
	m_wndName.SetWindowText(data.Name);
	m_nLoadItem = data.nLoadItem;
	UpdateData(FALSE);

	BOOL bCheck[6];
	for (int i=0; i<6; i++)
		bCheck[i] = FALSE;

	for (int i=0; i<6; i++)
	{
		if (data.nLoadType[i] == 0)
			bCheck[i] = TRUE;
	}

	((CButton*)GetDlgItem(IDC_CMD_DX1))->SetCheck(bCheck[0]);
	((CButton*)GetDlgItem(IDC_CMD_DX2))->SetCheck(!bCheck[0]);
	
	((CButton*)GetDlgItem(IDC_CMD_DY1))->SetCheck(bCheck[1]);
	((CButton*)GetDlgItem(IDC_CMD_DY2))->SetCheck(!bCheck[1]);
	
	((CButton*)GetDlgItem(IDC_CMD_DZ1))->SetCheck(bCheck[2]);
	((CButton*)GetDlgItem(IDC_CMD_DZ2))->SetCheck(!bCheck[2]);
	
	((CButton*)GetDlgItem(IDC_CMD_RX1))->SetCheck(bCheck[3]);
	((CButton*)GetDlgItem(IDC_CMD_RX2))->SetCheck(!bCheck[3]);

	((CButton*)GetDlgItem(IDC_CMD_RY1))->SetCheck(bCheck[4]);
	((CButton*)GetDlgItem(IDC_CMD_RY2))->SetCheck(!bCheck[4]);

	((CButton*)GetDlgItem(IDC_CMD_RZ1))->SetCheck(bCheck[5]);
	((CButton*)GetDlgItem(IDC_CMD_RZ2))->SetCheck(!bCheck[5]);
}

void CCMGridP1LoadFactorTypeDlg::Dlg2Data(T_P1LT_D& P1ltD, int nSeq)
{
	UpdateData(TRUE);

	CString strName;
	m_wndName.GetWindowText(strName);
	P1ltD.Name = strName;
	P1ltD.nLoadItem = m_nLoadItem;

	for (int i=0; i<6; i++)
		P1ltD.nLoadType[i] = 1;

	if (m_bDxBend)
		P1ltD.nLoadType[0] = 0;
	if (m_bDyBend)
		P1ltD.nLoadType[1] = 0;
	if (m_bDzBend)
		P1ltD.nLoadType[2] = 0;
	if (m_bRxBend)
		P1ltD.nLoadType[3] = 0;
	if (m_bRyBend)
		P1ltD.nLoadType[4] = 0;
	if (m_bRzBend)
		P1ltD.nLoadType[5] = 0;
	P1ltD.nSeq = nSeq;
}

void CCMGridP1LoadFactorTypeDlg::OnCmdClose() 
{
	CDialogMove::OnCancel();
}

void CCMGridP1LoadFactorTypeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
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

void CCMGridP1LoadFactorTypeDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bMgdrChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_P1LT_ADD):
		case(UR_P1LT_DEL):
		case(UR_P1LT_MFS):
		case(UR_P1LT_MFD):
			bMgdrChanged = TRUE;
			break;
		default:
			break;
		}
		if (bMgdrChanged)
			break;
	} // end of while

	if (bMgdrChanged) MakeItemEx(); 
}

void CCMGridP1LoadFactorTypeDlg::MakeItemEx()
{
	m_GridList.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountP1lt();
	if(nItemCount == 0) return;

	T_P1LT_K P1ltK;
	T_P1LT_D P1ltD;

	CArray<T_P1LT_K, T_P1LT_K> arKey;
	m_pDoc->m_pAttrCtrl->GetP1ltKeyList(arKey);

	int nSize = arKey.GetSize();
	if (nSize == 0)
		return;

	for (int i=0; i<nSize; i++)
	{
		P1ltK = arKey[i];
		m_pDoc->m_pAttrCtrl->GetP1lt(P1ltK, P1ltD);
		m_GridList.InsertItem(i, _T(""));
		SetItem(i, P1ltK, P1ltD);
	}
	//m_GridList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

void CCMGridP1LoadFactorTypeDlg::SetItem(int nIndex, T_P1LT_K key, T_P1LT_D& data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 3; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_GridList.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_GridList.SetItemData(nIndex, (DWORD)key);
}

CString CCMGridP1LoadFactorTypeDlg::DataToStr(int nIndex, T_P1LT_D& data)
{
	CString strName;
	strName = _T("");
	switch(nIndex)
	{
	case 0:
		strName = data.Name;
		break;
	case 1:
		if (data.nLoadItem == 0)
			strName = _LS(IDS_CMD_IMPACTNP1_REACTION);
		else if (data.nLoadItem == 1)
			strName = _LS(IDS_CMD_IMPACTNP1_DISP);
		else if (data.nLoadItem == 2)
			strName = _LS(IDS_CMD_IMPACTNP1_MEMBFORCE);
		break;
	case 2:
		for (int i=0; i<6; i++)
		{
			if (data.nLoadType[i] == 0)
				strName += _T("B");
			else
				strName += _T("S");
		}
		break;
	}
	return strName;
}

void CCMGridP1LoadFactorTypeDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	T_P1LT_K key;
	T_P1LT_D data;
	int nIndex;

	if (!GetSelectedData(nIndex))
	{
		data.Initialize();
		return;
	}
	else 
	{
		key = m_GridList.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetP1lt(key, data);
	}

	// 값을 설정한다.
	Data2Dlg(data);
	SetLabel(data.nLoadItem);

	*pResult = 0;
}

void CCMGridP1LoadFactorTypeDlg::OnCmdLoadItem()
{
	UpdateData();
	SetLabel(m_nLoadItem);

	if (m_nLoadItem == 0)
	{
		m_bDxBend = FALSE;
	  m_bDyBend = FALSE;
	  m_bDzBend = FALSE;
	  m_bRxBend = FALSE;
	  m_bRyBend = FALSE;
	  m_bRzBend = FALSE;
	}
	else if (m_nLoadItem == 1)
	{
		m_bDxBend = TRUE;
	  m_bDyBend = TRUE;
	  m_bDzBend = TRUE;
	  m_bRxBend = TRUE;
	  m_bRyBend = TRUE;
	  m_bRzBend = TRUE;
	}
	else if (m_nLoadItem == 2)
	{
		m_bDxBend = TRUE;
	  m_bDyBend = FALSE;
	  m_bDzBend = FALSE;
	  m_bRxBend = FALSE;
	  m_bRyBend = TRUE;
	  m_bRzBend = TRUE;
	}
	((CButton*)GetDlgItem(IDC_CMD_DX1))->SetCheck(m_bDxBend);
	((CButton*)GetDlgItem(IDC_CMD_DX2))->SetCheck(!m_bDxBend);
	((CButton*)GetDlgItem(IDC_CMD_DY1))->SetCheck(m_bDyBend);
	((CButton*)GetDlgItem(IDC_CMD_DY2))->SetCheck(!m_bDyBend);
	((CButton*)GetDlgItem(IDC_CMD_DZ1))->SetCheck(m_bDzBend);
	((CButton*)GetDlgItem(IDC_CMD_DZ2))->SetCheck(!m_bDzBend);
	((CButton*)GetDlgItem(IDC_CMD_RX1))->SetCheck(m_bRxBend);
	((CButton*)GetDlgItem(IDC_CMD_RX2))->SetCheck(!m_bRxBend);
	((CButton*)GetDlgItem(IDC_CMD_RY1))->SetCheck(m_bRyBend);
	((CButton*)GetDlgItem(IDC_CMD_RY2))->SetCheck(!m_bRyBend);
	((CButton*)GetDlgItem(IDC_CMD_RZ1))->SetCheck(m_bRzBend);
	((CButton*)GetDlgItem(IDC_CMD_RZ2))->SetCheck(!m_bRzBend);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdDx1()
{
	((CButton*)GetDlgItem(IDC_CMD_DX1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CMD_DX2))->SetCheck(FALSE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdDy1()
{
	((CButton*)GetDlgItem(IDC_CMD_DY1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CMD_DY2))->SetCheck(FALSE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdDz1()
{
	((CButton*)GetDlgItem(IDC_CMD_DZ1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CMD_DZ2))->SetCheck(FALSE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdRx1()
{
	((CButton*)GetDlgItem(IDC_CMD_RX1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CMD_RX2))->SetCheck(FALSE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdRy1()
{
	((CButton*)GetDlgItem(IDC_CMD_RY1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CMD_RY2))->SetCheck(FALSE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdRz1()
{
	((CButton*)GetDlgItem(IDC_CMD_RZ1))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CMD_RZ2))->SetCheck(FALSE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdDx2()
{
	((CButton*)GetDlgItem(IDC_CMD_DX1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CMD_DX2))->SetCheck(TRUE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdDy2()
{
	((CButton*)GetDlgItem(IDC_CMD_DY1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CMD_DY2))->SetCheck(TRUE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdDz2()
{
	((CButton*)GetDlgItem(IDC_CMD_DZ1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CMD_DZ2))->SetCheck(TRUE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdRx2()
{
	((CButton*)GetDlgItem(IDC_CMD_RX1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CMD_RX2))->SetCheck(TRUE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdRy2()
{
	((CButton*)GetDlgItem(IDC_CMD_RY1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CMD_RY2))->SetCheck(TRUE);
}

void CCMGridP1LoadFactorTypeDlg::OnCmdRz2()
{
	((CButton*)GetDlgItem(IDC_CMD_RZ1))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CMD_RZ2))->SetCheck(TRUE);
}

void CCMGridP1LoadFactorTypeDlg::SetLabel(int nType)
{
	if (nType == 0 || nType == 2)
	{
		GetDlgItem(IDC_CMD_DX)->SetWindowText(_LS(IDS_CMD_P1LD_FX));
		GetDlgItem(IDC_CMD_DY)->SetWindowText(_LS(IDS_CMD_P1LD_FY));
		GetDlgItem(IDC_CMD_DZ)->SetWindowText(_LS(IDS_CMD_P1LD_FZ));
		GetDlgItem(IDC_CMD_RX)->SetWindowText(_LS(IDS_CMD_P1LD_MX));
		GetDlgItem(IDC_CMD_RY)->SetWindowText(_LS(IDS_CMD_P1LD_MY));
		GetDlgItem(IDC_CMD_RZ)->SetWindowText(_LS(IDS_CMD_P1LD_MZ));
	}
	else
	{
		GetDlgItem(IDC_CMD_DX)->SetWindowText(_LS(IDS_CMD_P1LD_DX));
		GetDlgItem(IDC_CMD_DY)->SetWindowText(_LS(IDS_CMD_P1LD_DY));
		GetDlgItem(IDC_CMD_DZ)->SetWindowText(_LS(IDS_CMD_P1LD_DZ));
		GetDlgItem(IDC_CMD_RX)->SetWindowText(_LS(IDS_CMD_P1LD_RX));
		GetDlgItem(IDC_CMD_RY)->SetWindowText(_LS(IDS_CMD_P1LD_RY));
		GetDlgItem(IDC_CMD_RZ)->SetWindowText(_LS(IDS_CMD_P1LD_RZ));
	}
}
