// CmdScaleUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdScaleUpDlg.h"

#include "..\\wg_db\\wg_db_DBDoc.h"
#include "..\\wg_db\\wg_db_DataCtrl.h"
#include "..\\wg_db\\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCmdScaleUpDlg dialog


CCmdScaleUpDlg::CCmdScaleUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdScaleUpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdScaleUpDlg)
	m_Factor = 0.0;
	//}}AFX_DATA_INIT
}


void CCmdScaleUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdScaleUpDlg)
	DDX_Control(pDX, IDC_CMD_SCALEUP_LIST, m_CtrlList);
	DDX_Control(pDX, IDC_CMD_SCALEUP_LCASE, m_SpecLcase);
	DDX_Text(pDX, IDC_CMD_SCALEUP_FACTOR, m_Factor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdScaleUpDlg, CDialog)
	//{{AFX_MSG_MAP(CCmdScaleUpDlg)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_ADD, OnCmdScaleupAdd)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_MODIFY, OnCmdScaleupModify)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_DEL, OnCmdScaleupDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdScaleUpDlg message handlers


BOOL CCmdScaleUpDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialog::OnInitDialog();

	//	Default 초기화 
	Initial_ComboBox();
	Initial_Data();
	Initial_ListCtrlBox();

	UpdateData(FALSE);
	return TRUE;
}

void CCmdScaleUpDlg::Show_EditComboBox(BOOL bCheck)
{
	if(bCheck)
	{
		m_SpecLcase.EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_SCALEUP_FACTOR)->EnableWindow(TRUE);
	}
	else
	{
		m_SpecLcase.EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_SCALEUP_FACTOR)->EnableWindow(FALSE);
	}
}

void CCmdScaleUpDlg::Initial_ListCtrlBox()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_CtrlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_CtrlList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[2] = {(TCHAR*)(LPCTSTR)_LS(IDS_WG_CMD__ADDD__Factor),(TCHAR*)(LPCTSTR)_LS(IDS_WG_CMD__ADDD__Load_Case_Name)};
	int width[2] = {70,110};
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_CtrlList.InsertColumn(i,&lvcolumn);
	}
}

void CCmdScaleUpDlg::Initial_Data()
{
	m_Factor = 1.;
	if(m_SpecLcase.GetCount() != 0)  m_SpecLcase.SetCurSel(0);
}

void CCmdScaleUpDlg::Initial_ComboBox()
{
	m_SpecLcase.ResetContent();

	CArray<T_SPLC_K, T_SPLC_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(KeyList);
	int Count = KeyList.GetSize();
	if(Count > 0)
	{
		CString str=_LS(IDS_WG_CMD__ADDD__None);
	  m_SpecLcase.AddString(str);
		//  Spectrum Load Case Data가 있음.
		for(int i=0; i<Count; i++)
		{
			T_SPLC_D rData;
			rData.Initialize();
			int key = KeyList.GetAt(i);

			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSplc(key,rData);
			ASSERT(bCheck);

			str = rData.LoadCaseName;
	    m_SpecLcase.AddString(str);
		}
		Show_EditComboBox(TRUE);
	}
	else
		Show_EditComboBox(FALSE);
}

BOOL CCmdScaleUpDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(m_Factor < 0. || m_Factor > 10.)  bCheck = FALSE;
	if(!bCheck)	
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Scale_Up_Factor____),MB_OK);
		return bCheck;
	}

	if(m_Factor < 1. && m_Factor > 0.)  bCheck = FALSE;
	if(!bCheck)	
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning___Check_Scale_Up_Factor__),MB_OK);

	return TRUE;
}

void CCmdScaleUpDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CCmdScaleUpDlg::Write_ListCtrlBox(int Index,double Factor,CString strLoadCase)
{
	LV_ITEM lvitem;

	CString strFactor=_T("");
	strFactor.Format(_T("%10.3f"),Factor);
	//  ListCtrl Box에 값을 입력합니다.
	lvitem.mask		  = LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 0;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
	m_CtrlList.InsertItem(&lvitem);

	lvitem.mask 		= LVIF_TEXT;
	lvitem.iItem  	= Index;
	lvitem.iSubItem = 1;
	lvitem.pszText	= (LPTSTR)(LPCTSTR)strLoadCase;
	m_CtrlList.SetItem(&lvitem);
}

void CCmdScaleUpDlg::OverWrite_ListCtrlBox(int Index,double Factor,CString strLoadCase)
{
	CString strFactor=_T("");
	strFactor.Format(_T("%10.3f"),Factor);
	//  ListCtrl Box에 값을 입력합니다.
	m_CtrlList.SetItemText(Index,0,strFactor);
	m_CtrlList.SetItemText(Index,1,strLoadCase);
}

void CCmdScaleUpDlg::OnCmdScaleupAdd() 
{
	int nKey;
	UpdateData(TRUE);
	if(ErrorCheck())
	{
		//  Spectrum Load Case Data의 순번을 얻어옵니다.
		int Index = m_SpecLcase.GetCurSel();
		if(Index != -1)
		{
			CString strName=_T("");
			m_SpecLcase.GetLBText(Index,strName);
			if(strName == _LS(IDS_WG_CMD__ADDD__None))
				nKey = 0;
			else
				nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strName);

			if(nKey != 0)  m_ScaleData.SetAt(nKey,m_Factor);
			//  List Control Box에 작성합니다.
			int Index = m_CtrlList.GetItemCount();
			Write_ListCtrlBox(Index,m_Factor,strName);
		}
		else
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Load_Case____),MB_OK);
	}
}

void CCmdScaleUpDlg::OnCmdScaleupModify() 
{
	int nItem = m_CtrlList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_data_));
		return;
	}

	int nKey;
	UpdateData(TRUE);
	if(ErrorCheck())
	{
		//  Spectrum Load Case Data의 순번을 얻어옵니다.
		int Index = m_SpecLcase.GetCurSel();
		if(Index != -1)
		{
			CString strName=_T("");
			m_SpecLcase.GetLBText(Index,strName);
			if(strName == _LS(IDS_WG_CMD__ADDD__None))
				nKey = 0;
			else
				nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strName);

			if(nKey != 0)  m_ScaleData.SetAt(nKey,m_Factor);
			//  List Control Box에 재작성합니다.
			OverWrite_ListCtrlBox(nItem,m_Factor,strName);
		}
		else
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Load_Case____),MB_OK);
	}
}

void CCmdScaleUpDlg::OnCmdScaleupDel() 
{
	int nItem = m_CtrlList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_data_));
		return;
	}
	CString strName=_T("");
	strName = m_CtrlList.GetItemText(nItem,1);
	int nKey = m_pDoc->m_pAttrCtrl->GetSplcKey(strName);
	m_ScaleData.RemoveKey(nKey);
	m_CtrlList.DeleteItem(nItem);
}
