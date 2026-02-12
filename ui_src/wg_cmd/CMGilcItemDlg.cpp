// CMGilcItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGilcItemDlg.h"
#include "CMGilcListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMGilcItemDlg dialog


CCMGilcItemDlg::CCMGilcItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGilcItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGilcItemDlg)
	//}}AFX_DATA_INIT
	// 데이타초기화
	
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_bModify = FALSE;
	m_csOldName.Empty();
	m_Data.Initialize();
}

void CCMGilcItemDlg::SetParamData(LPCTSTR name) // 수정 루틴 (cf. 클래스 외부에서 사용됨)
{ 
	m_csOldName = name; // 이름이 키가 된다.
	m_pDoc->m_pAttrCtrl->GetGilc(m_csOldName, m_Data); // 수정할 데이타를 이름으로 찾아서 
	m_bModify = TRUE;
}

void CCMGilcItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGilcItemDlg)
	DDX_Control(pDX, IDC_CMD_NAME_EDIT, m_ctrlName);
	DDX_Control(pDX, IDC_CMD_TYPE_COBX, m_ctrlType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGilcItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGilcItemDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGilcItemDlg message handlers

void CCMGilcItemDlg::InitComboBox()
{
	/*
	CString aItem[5] = { _T("Dead Load"), _T("Other Dead Load"), 
		_T("Vehicle Live Load"),_T("Crowded Live Load"), _T("Other Live Load")};
	DWORD aData[5] = { 0, 1, 2, 3, 4 };
	*/
	/*
	REG_STR_RES(IDS_CMD_GILC_DEAD_LOAD , _T("Dead Load"));
	REG_STR_RES(IDS_CMD_GILC_ODEAD_LOAD , _T("Other Dead Load"));
	REG_STR_RES(IDS_CMD_GILC_VLIVE_LOAD , _T("Vehicle Live Load"));
	REG_STR_RES(IDS_CMD_GILC_CLIVE_LOAD , _T("Crowded Live Load"));
	REG_STR_RES(IDS_CMD_GILC_OLIVE_LOAD , _T("Other Live Load"));
	*/
	CStringArray aName;
	CArray<UINT, UINT> aData;
#define M_NAME_DATA_ADD(NAME, DATA)  aName.Add(NAME); aData.Add(DATA)
	M_NAME_DATA_ADD(_LS(IDS_CMD_GILC_DEAD_LOAD),  0);
	//M_NAME_DATA_ADD(_LS(IDS_CMD_GILC_ODEAD_LOAD), 1); // Other Dead Load 뺀다. 상반 응력 고려 하중 조건을 이용하는 방법
	//#if defined (_JP)
	//if (CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
	//{
	M_NAME_DATA_ADD(_LS(IDS_CMD_GILC_VLIVE_LOAD), 2);
	M_NAME_DATA_ADD(_LS(IDS_CMD_GILC_CLIVE_LOAD), 3);
	M_NAME_DATA_ADD(_LS(IDS_CMD_GILC_OLIVE_LOAD), 4);
	//}
	//#endif
#undef M_NAME_DATA_ADD
	
	int ix;
	m_ctrlType.ResetContent(); // 콤보 박스  초기화 
	for (int i=0; i<aName.GetSize(); i++)
	{
		ix = m_ctrlType.AddString(aName[i]);
		m_ctrlType.SetItemData(ix, aData[i]);
	}
	m_ctrlType.SetCurSel(0);
}

BOOL CCMGilcItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitComboBox();
	/*
	m_ctrlType.AddString(_T("Dead Load"));// Dead Load
	m_ctrlType.AddString(_T("Other Dead Load"));// Other Dead Load
	
#if defined (_JP)
	m_ctrlType.AddString(_T("Vehicle Live Load"));// Vehicle Live Load
	m_ctrlType.AddString(_T("Crowded Live Load"));// Crowded Live Load
	m_ctrlType.AddString(_T("Other Live Load"));// Other Live Load
#endif
	*/
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(!m_bModify);
	//데이타 세팅 
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGilcItemDlg::OnOK() 
{
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}

void CCMGilcItemDlg::OnCancel() 
{	
	CDialogMove::OnCancel();
}

void CCMGilcItemDlg::OnCmdApply() 
{
	Dlg2Data();
}

BOOL CCMGilcItemDlg::Dlg2Data() // 다이얼로그에서 db 로 
{
	T_GILC_D OldData;
	OldData = m_Data; //수정될 값을 백업 

	m_Data.Initialize();
/*
	BOOL ModifyGilc(CString& rstrOldLoadCaseName, T_GILC_D& rDataNew);  //serial 번호도 변경 가능
	T_GILC_D m_Data;
	CString   m_csOldName;
	UINT      m_nOldID;
*/
	//m_Data.nLoadCaseType = m_ctrlType.GetCurSel();

	/*새로 수정될 데이타를 다이얼로그로부터 얻어와서*/
	m_ctrlName.GetWindowText(m_Data.LoadCaseName); //new data
	int nIndex = m_ctrlType.GetCurSel();
	if (nIndex == CB_ERR) return FALSE;
	m_Data.nLoadCaseType = m_ctrlType.GetItemData(nIndex); //new data
	/*************************************************/

	if( m_bModify )
	{ 
		m_Data.LoadCaseId = OldData.LoadCaseId; // 새로운 데이타에 수정될 아이디만 세팅후 
		if( !m_pDoc->m_pDataCtrl->ModifyGilc(m_csOldName, m_Data) ) return FALSE; // 수정해준다.//아이디는 동일 
	}
	else
	{
		m_bModify = FALSE; //m_Key = 0; m_Data.Initialize();
		if( !m_pDoc->m_pDataCtrl->AddGilc(m_Data) ) return FALSE;
	}

	/*
	int nIndex = m_ctrlType.GetCurSel();
	if (nIndex == CB_ERR) return FALSE;
	m_Data.nLoadCaseType = m_ctrlType.GetItemData(nIndex); 
	*/
	return TRUE;
}

void CCMGilcItemDlg::Data2Dlg()
{  
	// SetWindowText
	//m_pDoc->m_pAttrCtrl->GetGilc(m_Data.LoadCaseName,m_Data);
	
	m_ctrlName.SetWindowText(m_Data.LoadCaseName);

	int nCount = m_ctrlType.GetCount();
	int i = 0;
	for (i=0; i<nCount; i++)
	{
		if (m_Data.nLoadCaseType == m_ctrlType.GetItemData(i)) break;
	}
	ASSERT(i != nCount);
	m_ctrlType.SetCurSel(i);

	//m_ctrlType.SetItemData(IDC_CMD_TYPE_COBX,m_Data.nLoadCaseType);
	// CComboBox : SetItemData(Init)/ GetItemData
}

