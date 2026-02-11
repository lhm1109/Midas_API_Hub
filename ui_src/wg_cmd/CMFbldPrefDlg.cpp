// CMFbldPrefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMFbldPrefDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\INITCTRL.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMFbldPrefDlg dialog


CCMFbldPrefDlg::CCMFbldPrefDlg(CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMFbldPrefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMFbldPrefDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CRect TRect;
	AfxGetMainWnd()->GetClientRect(&TRect);

	m_nX = (TRect.left + TRect.right)  / 2;
	m_nY = (TRect.top  + TRect.bottom) / 2; 

	CDBDoc::GetDocPoint()->m_pInitCtrl->GetPreference(m_Preference);
	//현재 단위계로 단위계변환 
	int nLoad = m_Preference.FLoadDB.LoadList.GetSize();
	for(int i = 0; i < nLoad; i++)
	{
		m_Preference.FLoadDB.LoadList[i].dValue = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitDataOut(
																			CUnitCtrl::m_FBLD_UNIT.FloorLoad, m_Preference.FLoadDB.LoadList[i].dValue);
	}
}


void CCMFbldPrefDlg::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMFbldPrefDlg)
	DDX_Control(pDX, IDC_CMD_FLOAD_LIST, m_wndFLoadList);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CCMFbldPrefDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMFbldPrefDlg)
	ON_NOTIFY(NM_CLICK, IDC_CMD_FLOAD_LIST, OnClickCmdFloadList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_FLOAD_LIST, OnItemchangedCmdFloadList)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_FLOAD_LIST, OnDblclkCmdFloadList)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CNotifyListCtrl::CNOTIFY_LCTRL_KEYUP,OnListCtrlKeyUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMFbldPrefDlg message handlers
//void CCMFbldPrefDlg::

BOOL CCMFbldPrefDlg::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	MakeListHeader();
	ShowLoadData();
	SetWindowPos(NULL,m_nX,m_nY,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CCMFbldPrefDlg::MyDoModal(int XPos, int YPos)
{
	if(0 == m_Preference.FLoadDB.LoadList.GetSize())
	{
		AfxMessageBox(_LS(IDS_CMD_FBLDPREF_NotExistFLoadDB));
		return IDCANCEL;
	}

	m_nX = XPos     ; 
	m_nY = YPos     ;
	return DoModal();
}

void CCMFbldPrefDlg::MakeListHeader()
{
	CStringArray Contents;
	CArray<float, float> HRatio;

	Contents.Add(_LS(IDS_CMD_FBLDPREF_Name)); HRatio.Add(0.68f);
	Contents.Add(_LS(IDS_CMD_FBLDPREF_Load_Value)); HRatio.Add(0.32f);

	CDlgUtil::_SetListCtrlHeader(&m_wndFLoadList,Contents,&HRatio,NULL);
}

void CCMFbldPrefDlg::ShowLoadData()
{
	//m_wndFLoadList.DeleteAllItems();
	CDlgUtil::ClearAllListItemDbl(&m_wndFLoadList);
	CStringArray Contents;
	CString TStr;
	int nFLoad = m_Preference.FLoadDB.LoadList.GetSize();
	for(int i = 0; i< nFLoad; i++)
	{
		Contents.RemoveAll();
		Contents.Add(m_Preference.FLoadDB.LoadList[i].strName);
		TStr.Format(_T("%g"),m_Preference.FLoadDB.LoadList[i].dValue);
		Contents.Add(TStr);
		CDlgUtil::SetListItemDbl(&m_wndFLoadList,i,Contents,m_Preference.FLoadDB.LoadList[i].dValue);
	}
}

double CCMFbldPrefDlg::GetCurLoadValue()
{
	return m_CurLoadValue;
}

void CCMFbldPrefDlg::SetCurLoadValue()
{
	CArray<double, double> arRet;
	CDlgUtil::GetSelectedListItemDataDbl(&m_wndFLoadList,arRet);

	if(arRet.GetSize() != 1)
	{
		m_CurLoadValue = 0;
		return;
	}

	m_CurLoadValue = arRet[0];
}

LRESULT CCMFbldPrefDlg::OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam)
{
	SetCurLoadValue();
	return 0L;
}

void CCMFbldPrefDlg::OnClickCmdFloadList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetCurLoadValue();
	*pResult = 0;
}

void CCMFbldPrefDlg::OnItemchangedCmdFloadList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	SetCurLoadValue();
	
	*pResult = 0;
}

BOOL CCMFbldPrefDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDlgUtil::ClearAllListItemDbl(&m_wndFLoadList);
	
	return CInternationalDlg::DestroyWindow();
}

void CCMFbldPrefDlg::OnDblclkCmdFloadList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetCurLoadValue();
	OnOK();
	*pResult = 0;
	
}
