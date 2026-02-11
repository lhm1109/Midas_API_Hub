// AutoWidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "AutoWidDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CMLoadCaseDlg.h"
#include "ExportFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoWidDlg dialog


CAutoWidDlg::CAutoWidDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CAutoWidDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoWidDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_aRdoSelectType.RemoveAll();
	m_aRdoSelectType.Add(IDC_CMD_SELTYPE_STOR_RDO);
	m_aRdoSelectType.Add(IDC_CMD_SELTYPE_ALL_RDO );
	m_aRdoSelectType.FreeExtra();

}


void CAutoWidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoWidDlg)
	DDX_Control(pDX, IDC_CMD_STORYNAME_COMBO, m_wndStoryNameCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoWidDlg, CDialogMove)
	//{{AFX_MSG_MAP(CAutoWidDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CMD_SELTYPE_STOR_RDO,    OnSelectTypeRdo)
	ON_BN_CLICKED(IDC_CMD_SELTYPE_ALL_RDO ,    OnSelectTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoWidDlg message handlers
BOOL CAutoWidDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	int nSelect=0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSelectType, nSelect);

	m_pDoc = CDBDoc::GetDocPoint();
	CArray<T_STOR_K, T_STOR_K> aStorKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKey);
	T_STOR_D Data;
	int ix;
	int nSize = aStorKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(aStorKey[i], Data);		
		ix = m_wndStoryNameCombo.AddString(Data.StoryName);
		m_wndStoryNameCombo.SetItemData(ix, aStorKey[i]);
	} 
	m_wndStoryNameCombo.SetCurSel(0);	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoWidDlg::OnOK() 
{
	OnApply();

	CDialogMove::OnOK();
}

void CAutoWidDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CAutoWidDlg::OnApply() 
{
	int nSelect = 0;    // nSelect: 0=Story, 1=All
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSelectType, nSelect);

	T_STOR_K Key = 0;
	if (nSelect == 0)   // nSelect: 0=Story, 1=All
	{
		int num = m_wndStoryNameCombo.GetCurSel();
		if(num < 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD_AUTO_WID_ERROR));
			return;
		}
		
		Key = m_wndStoryNameCombo.GetItemData(num);	
	}

	m_pDoc->m_pDataCtrl->AutoWallId(Key);
}

void CAutoWidDlg::OnSelectTypeRdo()
{ 
	int nType=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSelectType, nType);   // nType: 0=Story, 1=All

	// enable/disable control
	GetDlgItem(IDC_CMD_STORYNAME_COMBO)->EnableWindow(nType==0);
}
