// CMPdelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMPdelDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"


using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMPdelDlg
//IMPLEMENT_DYNCREATE(CCMPdelDlg, CDialogMove)

CCMPdelDlg::CCMPdelDlg(CWnd* pParent)
	: CDialogMove(CCMPdelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMPdelDlg)
	//}}AFX_DATA_INIT
}

CCMPdelDlg::~CCMPdelDlg()
{
}

void CCMPdelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMPdelDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_LOADCASE, m_LoadCase);
	DDX_Control(pDX, IDC_CMD_CONV_TOL_EDIT, m_wndConvTol);
	DDX_Control(pDX, IDC_CMD_ITER_EDIT, m_wndIterEdit);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMPdelDlg Implementations
void CCMPdelDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_CMD_PDEL_loadcase), _LS(IDS_CMD_PDEL_scale) };
//18japan  CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__LoadCase), _LS(IDS_WG_CMD__ADDD__ScaleFactor)};
	CRect rtList;
	m_List.GetWindowRect(rtList);
	int nWidth = rtList.Width() - globalUtils.GetSystemMetricsForDPI(&m_List, SM_CXVSCROLL) - 1;
	int nColWidth[] = { nWidth * 2.0 / 3, nWidth * 1.0 / 3 }/*{90, 45}*/;
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
		lvcolumn.fmt = LVCFMT_CENTER;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMPdelDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMPdelDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.nNumPdelCase;
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.PdelCase[nCount]);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCMPdelDlg::DataToStr(int i, T_PDEL_BASE &data)
{
	CString value;
	if (i == 0) 
	{
		T_STLD_K key = data.LoadCaseKey;
		T_STLD_D data;
		m_pDoc->m_pAttrCtrl->GetStld(key, data);
		value = data.LoadCaseName;
	}
	else if (i == 1) value.Format(_T("%g"), data.dblFactor);
	return value;
}

void CCMPdelDlg::SetItem(int nIndex, T_PDEL_BASE &data)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 2; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CCMPdelDlg::ValidItem(T_PDEL_BASE& data)
{
	if (data.LoadCaseKey <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_loadcase_key_));
		return FALSE;
	}

	if (data.dblFactor <= 0.0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_scale_factor_));
		return FALSE;
	}
		
	return TRUE;
}

BOOL CCMPdelDlg::Dlg2Item(T_PDEL_BASE &data)
{
	UINT nLoadCaseType;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}
	CString csScaleFactor;
	m_wndScaleFactor.GetWindowText(csScaleFactor);
	if(!CStrParser::GetFloatNumber(csScaleFactor, data.dblFactor)) return FALSE;
//  data.dblFactor = _tstof(csScaleFactor);

	return TRUE;
}

BOOL CCMPdelDlg::Item2Dlg(T_PDEL_BASE &data)
{
	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, data.LoadCaseKey);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dblFactor);
	m_wndScaleFactor.SetWindowText(csScaleFactor);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMPdelDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMPdelDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_REMOVE_DATA, OnCmdRemoveData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMPdelDlg message handlers

BOOL CCMPdelDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class
	if (!m_pDoc->m_pAttrCtrl->GetPdel(m_Data))
	{
		GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
		m_Data.Initialize();
	}
	
	// Set Default Value
	CString csIter, csConvTol;
	csIter.Format(_T("%d"), m_Data.nNumItr);
	csConvTol.Format(_T("%g"), m_Data.ConvTol);
	m_wndIterEdit.SetWindowText(csIter);
	m_wndIterEdit.SetRange(1, 100);
	m_wndIterEdit.SetInteger(TRUE);
	m_wndConvTol.SetWindowText(csConvTol);
	m_wndScaleFactor.SetWindowText(_T("1"));

	m_LoadCase.SetLoadType(D_LOADCASE_STATIC);

	SetListCtrlHeader();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMPdelDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString csIter, csConvTol;
	m_wndIterEdit.GetWindowText(csIter);
	m_wndConvTol.GetWindowText(csConvTol);

//  m_Data.nNumItr = _ttoi(csIter);
//  m_Data.ConvTol = _tstof(csConvTol);
	if(!CStrParser::GetINumber(csIter, m_Data.nNumItr)) return;
	if(!CStrParser::GetFloatNumber(csConvTol, m_Data.ConvTol)) return;

	// 입력되지 않는 PdelCase는 초기화한다.
	//for (int i = m_Data.nNumPdelCase; i < D_PDEL_NUMPDEL_OLD; i++)
	//	m_Data.PdelCase[i].Initialize();
	if (m_pDoc->m_pDataCtrl->AddPdel(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CCMPdelDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CCMPdelDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));	
}

void CCMPdelDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_PDEL_BASE data;
	
	if (m_Data.nNumPdelCase > D_PDEL_NUMPDEL_MAX/*D_PDEL_NUMPDEL_OLD*/) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
		return;
	}
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumPdelCase; i++)
	{
		if (m_Data.PdelCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	//m_Data.PdelCase[m_Data.nNumPdelCase] = data;
	m_Data.PdelCase.Add(data);
	m_List.InsertItem(m_Data.nNumPdelCase, _T(""));
	SetItem(m_Data.nNumPdelCase, data);
	m_List.SetItemState(m_Data.nNumPdelCase++, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMPdelDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_PDEL_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumPdelCase; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (m_Data.PdelCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.PdelCase[nIndex] = data;
	SetItem(nIndex, data);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMPdelDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_List.DeleteItem(nIndex);
	for (int i = nIndex; i < m_Data.nNumPdelCase-1; i++)
		m_Data.PdelCase[i] = m_Data.PdelCase[i+1];

	m_Data.PdelCase.RemoveAt(m_Data.nNumPdelCase-1);
	m_Data.nNumPdelCase--;
	if (m_Data.nNumPdelCase > 0)
	{
		if (nIndex >= m_Data.nNumPdelCase) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CCMPdelDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	T_PDEL_BASE data;
	if (!GetSelectedItem(nIndex))
		data.Initialize();
	else data = m_Data.PdelCase[nIndex];

	// 값을 설정한다.
	Item2Dlg(data);

	*pResult = 0;
}

void CCMPdelDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMPdelDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key, KeyBak;
	T_STLD_D Data, DataBak;

	int i;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
				// do nothing
				break;
		case(UR_STLD_DEL):
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetStld(nKey, Key, Data);
				for (i = m_Data.nNumPdelCase-1; i >= 0; i--)
				{
					if (m_Data.PdelCase[i].LoadCaseKey == Key)
					{
						m_List.DeleteItem(i);
						for (int j = i; j < m_Data.nNumPdelCase-1; j++)
							m_Data.PdelCase[j] = m_Data.PdelCase[j+1];

						m_Data.PdelCase.RemoveAt(m_Data.nNumPdelCase-1);
						m_Data.nNumPdelCase--;
					}
				}
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
				for (i = 0; i < m_Data.nNumPdelCase; i++)
				{
					if (m_Data.PdelCase[i].LoadCaseKey == Key)
					{
						m_Data.PdelCase[i].LoadCaseKey = KeyBak;
						SetItem(i, m_Data.PdelCase[i]);
					}
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// PDEL 의 변화를 반영한다.
	BOOL bUpdate = FALSE;
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_PDEL_ADD):
			pViewBuff->GetPdel(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_PDEL_DEL):
			break;  // do nothing
		case(UR_PDEL_MFD):
			pViewBuff->GetPdel(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_PDEL_MFS):
			break;  // do nothing
		}
	}
	if (bUpdate)
	{
		CString csIter, csConvTol;
		csIter.Format(_T("%d"), m_Data.nNumItr);
		csConvTol.Format(_T("%g"), m_Data.ConvTol);
		m_wndIterEdit.SetWindowText(csIter);
		m_wndConvTol.SetWindowText(csConvTol);
		MakeItemEx();
	}
}

void CCMPdelDlg::OnCmdRemoveData() 
{
	// TODO: Add your control notification handler code here
	if (m_pDoc->m_pDataCtrl->DelPdel())
	{
		// *^^* Remove하면 Dialog 닫는다.
		//GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
		//m_Data.Initialize();
		//MakeItemEx();
		CDialogMove::OnOK();
	}
}
