// CMBuckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMBuckDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
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
// CCMBuckDlg

//IMPLEMENT_DYNCREATE(CCMBuckDlg, CDialogMove)

CCMBuckDlg::CCMBuckDlg(CWnd* pParent)
	: CDialogMove(CCMBuckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMBuckDlg)
		m_nLoadType = 0;
	//}}AFX_DATA_INIT
	m_aLoadFactor.SetSize(2);
	m_aLoadFactor[0] = IDC_CMD_POSITIVE_RDO;
	m_aLoadFactor[1] = IDC_CMD_SEARCH_RDO;

	m_aBuckComb.SetSize(2);
	m_aBuckComb[0] = IDC_CMD_BUCK_LOADTYPE_RDO;
	m_aBuckComb[1] = IDC_CMD_BUCK_LOADTYPE_RDO2;
}

CCMBuckDlg::~CCMBuckDlg()
{
}

void CCMBuckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMBuckDlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE, m_LoadCase);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SCALE_FACTOR, m_wndScaleFactor);
	//DDX_Control(pDX, IDC_CMD_CONV_TOL_EDIT, m_wndConvTol);
	//DDX_Control(pDX, IDC_CMD_ITER_EDIT, m_wndIterEdit);
	DDX_Control(pDX, IDC_CMD_BUCK_NUM_EDIT, m_wndBuckNumEdit);
	DDX_Control(pDX, IDC_CMD_SEARCH_FROM_EDT, m_wndSearchFrom);
	DDX_Control(pDX, IDC_CMD_SEARCH_TO_EDT,   m_wndSearchTo);
	DDX_Radio(pDX,IDC_CMD_BUCK_LOADTYPE_RDO, m_nLoadType);
	//DDX_Check(pDX, IDC_CMD_POSITIVE_CHK, m_bPositive);
	DDX_Radio(pDX, IDC_CMD_POSITIVE_RDO, m_bPositive);
	DDX_Control(pDX, IDC_CMD_STURM_CHK, m_bSturm);
	DDX_Control(pDX, IDC_CMD_BUCK_STIFFOPT_CHK, m_bAxialForce);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMBuckDlg Implementation

void CCMBuckDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_CMD_BUCK_loadcase), _LS(IDS_CMD_BUCK_scale), _LS(IDS_CMD_BUCK_LoadType)};
//18japan  CString aTitle[] = {_T("LoadCase"), _T("Scale")};
	CRect rtList;
	m_List.GetWindowRect(rtList);
	double dWidth = (rtList.Width() * 1.0) - globalUtils.GetSystemMetricsForDPI(&m_List, SM_CXVSCROLL) - 1.0;
	int nColWidth[] = { dWidth * 0.36, dWidth * 0.28, dWidth * 0.36};
	int nColNum = 3;
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

BOOL CCMBuckDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMBuckDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_Data.nNumBuckCase;
	if(nItemCount == 0) return;

	for (int nCount = 0; nCount < nItemCount; nCount++)
	{
		m_List.InsertItem(nCount, _T(""));
		SetItem(nCount, m_Data.BuckCase[nCount]);
	}
	m_List.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
}

CString CCMBuckDlg::DataToStr(int i, T_BUCK_BASE &data)
{
	CString aLoadType[] = {_LS(IDS_CMD_BUCK_Variable), _LS(IDS_CMD_BUCK_Constant)};
	CString value;
	if (i == 0) 
	{
		T_STLD_K key = data.LoadCaseKey;
		T_STLD_D data;
		m_pDoc->m_pAttrCtrl->GetStld(key, data);
		value = data.LoadCaseName;
	}
	else if (i == 1) value.Format(_T("%g"), data.dblFactor);
	else if (i == 2) value = aLoadType[data.nLoadType];
	return value;
}

void CCMBuckDlg::SetItem(int nIndex, T_BUCK_BASE &data)
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

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
}

BOOL CCMBuckDlg::ValidItem(T_BUCK_BASE& data)
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

BOOL CCMBuckDlg::Dlg2Item(T_BUCK_BASE &data)
{
	UINT nLoadCaseType;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Loadcase_was_not_selected_));
		return FALSE;
	}
	CString csScaleFactor;
	m_wndScaleFactor.GetWindowText(csScaleFactor);
	if(!CStrParser::GetFloatNumber(csScaleFactor,data.dblFactor)) return FALSE;
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aBuckComb, m_nLoadType);
	data.nLoadType = m_nLoadType;

	return TRUE;
}

BOOL CCMBuckDlg::Item2Dlg(T_BUCK_BASE &data)
{
	m_LoadCase.ChangeSelect(D_LOADCASE_STATIC, data.LoadCaseKey);
	CString csScaleFactor;
	csScaleFactor.Format(_T("%g"), data.dblFactor);
	m_wndScaleFactor.SetWindowText(csScaleFactor);
		
	CDlgUtil::CtrlRadioSetCheck(this, m_aBuckComb, m_nLoadType);
	data.nLoadType = m_nLoadType;

	int nLoadFactor;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadFactor, nLoadFactor);
	if (nLoadFactor == 0) m_bPositive = TRUE;
	else                  m_bPositive = FALSE; 

	m_Data.bPositive = m_bPositive;

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMBuckDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMBuckDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnChangedCurrentItem)
	ON_BN_CLICKED(IDC_CMD_REMOVE_DATA, OnCmdRemoveData)
	ON_BN_CLICKED(IDC_CMD_POSITIVE_RDO, OnCmdLoadFactor)
	ON_BN_CLICKED(IDC_CMD_SEARCH_RDO, OnCmdLoadFactor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMBuckDlg message handlers

BOOL CCMBuckDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add your specialized code here and/or call the base class
	if (!m_pDoc->m_pAttrCtrl->GetBuck(m_Data))
	{
		GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
		m_Data.Initialize();
	}
	
	Data2Dlg();
	
	EnableDisableCtrl();

	SetListCtrlHeader();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMBuckDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();
	
	if (m_pDoc->m_pDataCtrl->AddBuck(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CCMBuckDlg::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CCMBuckDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));	
}

void CCMBuckDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_BUCK_BASE data;
	
	if (m_Data.nNumBuckCase >= D_BUCK_NUMBUCK) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_add_loadcase_any_more_));
		return;
	}
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumBuckCase; i++)
	{
		if (m_Data.BuckCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.BuckCase[m_Data.nNumBuckCase] = data;
	m_List.InsertItem(m_Data.nNumBuckCase, _T(""));
	SetItem(m_Data.nNumBuckCase, data);
	m_List.SetItemState(m_Data.nNumBuckCase++, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMBuckDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}

	T_BUCK_BASE data;
	if (!Dlg2Item(data)) return;
	if (!ValidItem(data)) return;
	for (int i = 0; i < m_Data.nNumBuckCase; i++)
	{
		if (i == nIndex) continue;  // 변경 대상은 비교 제외
		if (m_Data.BuckCase[i].LoadCaseKey == data.LoadCaseKey)
		{
			CString csLoadCase;
			m_LoadCase.GetWindowText(csLoadCase);
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), csLoadCase);
			AfxMessageBox(msg);
			return;
		}
	}
	
	m_Data.BuckCase[nIndex] = data;
	SetItem(nIndex, data);
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMBuckDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nIndex;
	if (!GetSelectedItem(nIndex))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
		return;
	}
	m_List.DeleteItem(nIndex);
	for (int i = nIndex; i < m_Data.nNumBuckCase-1; i++)
		m_Data.BuckCase[i] = m_Data.BuckCase[i+1];
	m_Data.nNumBuckCase--;
	if (m_Data.nNumBuckCase > 0)
	{
		if (nIndex >= m_Data.nNumBuckCase) nIndex--;
		m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}	
}

void CCMBuckDlg::OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex;
	T_BUCK_BASE data;
	if (!GetSelectedItem(nIndex))
	{
		data.Initialize();
		data.dblFactor = 1.0;
		data.nLoadType = 0;
	}
	else data = m_Data.BuckCase[nIndex];

	// 값을 설정한다.
	Item2Dlg(data);

	*pResult = 0;
}

void CCMBuckDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMBuckDlg::UpdateBuffer()
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
				for (i = m_Data.nNumBuckCase-1; i >= 0; i--)
				{
					if (m_Data.BuckCase[i].LoadCaseKey == Key)
					{
						m_List.DeleteItem(i);
						for (int j = i; j < m_Data.nNumBuckCase-1; j++)
							m_Data.BuckCase[j] = m_Data.BuckCase[j+1];
						m_Data.nNumBuckCase--;
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
				for (i = 0; i < m_Data.nNumBuckCase; i++)
				{
					if (m_Data.BuckCase[i].LoadCaseKey == Key)
					{
						m_Data.BuckCase[i].LoadCaseKey = KeyBak;
						SetItem(i, m_Data.BuckCase[i]);
					}
				}
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// BUCK 의 변화를 반영한다.
	BOOL bUpdate = FALSE;
	pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_BUCK_ADD):
			pViewBuff->GetBuck(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_BUCK_DEL):
			break;  // do nothing
		case(UR_BUCK_MFD):
			pViewBuff->GetBuck(nKey, m_Data);
			bUpdate = TRUE;
			break;
		case(UR_BUCK_MFS):
			break;  // do nothing
		}
	}
	if (bUpdate)
	{
		CString csNumMode, csIter, csConvTol;
		csNumMode.Format(_T("%d"), m_Data.nNumMode);
		csIter.Format(_T("%d"), m_Data.nNumItr);
		csConvTol.Format(_T("%g"), m_Data.ConvTol);
		m_wndBuckNumEdit.SetWindowText(csNumMode);
		//m_wndIterEdit.SetWindowText(csIter);
		//m_wndConvTol.SetWindowText(csConvTol);
		MakeItemEx();
	}
}

void CCMBuckDlg::OnCmdRemoveData() 
{
	// TODO: Add your control notification handler code here
	if (m_pDoc->m_pDataCtrl->DelBuck())
	{
		// *^^* Remove하면 Dialog 닫는다.
		//GetDlgItem(IDC_CMD_REMOVE_DATA)->EnableWindow(FALSE);
		//m_Data.Initialize();
		//MakeItemEx();
		CDialogMove::OnOK();
	}
}

void CCMBuckDlg::OnCmdLoadFactor()
{
	EnableDisableCtrl();
}

void CCMBuckDlg::EnableDisableCtrl()
{
	int nLoadFactor;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadFactor, nLoadFactor);

	if(nLoadFactor == 0)
	{
		GetDlgItem(IDC_CMD_SEARCH_FROM_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SEARCH_TO_EDT)  ->EnableWindow(FALSE);
	}
	else 
	{
		GetDlgItem(IDC_CMD_SEARCH_FROM_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SEARCH_TO_EDT)  ->EnableWindow(TRUE);
	}
}

void CCMBuckDlg::Data2Dlg()
{
	CString csNumMode,/* csIter, csConvTol,*/ csSearchFrom, csSearchTo;
	 
	csNumMode.Format(_T("%d"), m_Data.nNumMode);
	m_wndBuckNumEdit.SetWindowText(csNumMode);
	m_wndBuckNumEdit.SetValue(m_Data.nNumMode);
	m_wndBuckNumEdit.SetRange(0, 1000);
	m_wndBuckNumEdit.SetInteger(TRUE);
	//csIter.Format(_T("%d"), m_Data.nNumItr);
	//csConvTol.Format(_T("%g"), m_Data.ConvTol);
	//m_wndIterEdit.SetWindowText(csIter);
	//m_wndIterEdit.SetRange(1, 1000);
	//m_wndConvTol.SetWindowText(csConvTol);

	csSearchFrom.Format(_T("%g"), m_Data.dLoadFactorFrom);
	m_wndSearchFrom.SetWindowText(csSearchFrom);

	csSearchTo.Format(_T("%g"), m_Data.dLoadFactorTo);
	m_wndSearchTo.SetWindowText(csSearchTo);

	m_bPositive=m_Data.bPositive;
	if(m_bPositive) CDlgUtil::CtrlRadioSetCheck(this, m_aLoadFactor, 0);
	else            CDlgUtil::CtrlRadioSetCheck(this, m_aLoadFactor, 1);

	m_bAxialForce.SetCheck(m_Data.bConsiderAxialOnly);
		
	m_bSturm.SetCheck(m_Data.bSturmSeq);
	
	m_wndScaleFactor.SetWindowText(_T("1"));

	m_nLoadType = 0;

	m_LoadCase.SetLoadType(D_LOADCASE_STATIC);
}

void CCMBuckDlg::Dlg2Data()
{
	CString csNumMode,/* csIter, csConvTol,*/ csSearchFrom, csSearchTo;
	m_wndBuckNumEdit.GetWindowText(csNumMode);
	m_wndSearchFrom.GetWindowText(csSearchFrom);
	m_wndSearchTo.GetWindowText(csSearchTo);
	//m_wndIterEdit.GetWindowText(csIter);
	//m_wndConvTol.GetWindowText(csConvTol);

	if(!CStrParser::GetINumber(csNumMode, m_Data.nNumMode)) return;
	if(!CStrParser::GetFloatNumber(csSearchFrom, m_Data.dLoadFactorFrom)) return;
	if(!CStrParser::GetFloatNumber(csSearchTo, m_Data.dLoadFactorTo)) return;
// 	if(!CStrParser::GetINumber(csIter, m_Data.nNumItr)) return;
// 	if(!CStrParser::GetFloatNumber(csConvTol, m_Data.ConvTol)) return;
	
	int nLoadFactor;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLoadFactor, nLoadFactor);
	if (nLoadFactor == 0) m_bPositive = TRUE;
	else                  m_bPositive = FALSE; 

	m_Data.bPositive = m_bPositive;
	m_Data.bSturmSeq = m_bSturm.GetCheck();
	m_Data.bConsiderAxialOnly = m_bAxialForce.GetCheck();

	// 입력되지 않는 PdelCase는 초기화한다.
	for (int i = m_Data.nNumBuckCase; i < D_BUCK_NUMBUCK; i++)
		m_Data.BuckCase[i].Initialize();
}

