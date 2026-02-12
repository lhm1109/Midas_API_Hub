// TpsgConvertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "TpsgConvertDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpsgConvertDlg dialog


CTpsgConvertDlg::CTpsgConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CTpsgConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTpsgConvertDlg)
	m_nStartNumber = 1;
	//}}AFX_DATA_INIT
}


void CTpsgConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTpsgConvertDlg)
	DDX_Control(pDX, IDC_TREEMENU_TPSG_USE_SUFFIX, m_chkSuffix);
	DDX_Control(pDX, IDC_TREEMENU_TPSG_SUFFIX, m_editSuffix);
	DDX_Text(pDX, IDC_TREEMENU_TPSG_NEWNUMBER, m_nStartNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTpsgConvertDlg, CDialogMove)
	//{{AFX_MSG_MAP(CTpsgConvertDlg)
	ON_BN_CLICKED(IDC_TREEMENU_TPSG_USE_SUFFIX, OnTreemenuTpsgUseSuffix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpsgConvertDlg message handlers

void CTpsgConvertDlg::OnOK() 
{
	UpdateData(TRUE);
	if(m_nStartNumber < 1) return;
	
	CString csSuffix(_T(""));
	int nCheck = m_chkSuffix.GetCheck();
	if (nCheck) 
	{
		m_editSuffix.GetWindowText(csSuffix);
		TCHAR ch;
		for (int i=0; i<csSuffix.GetLength(); i++)
		{
			ch = csSuffix.GetAt(i);
			if (ch < '0' || ch > '9') 
			{
				AfxMessageBox(_LS(IDS_TM_TPSG_ERR_Start_Suffix_Number));
				return;
			}
		}
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 주의 메세지 출력
	BOOL bExistRpsc = FALSE;
	T_TSGR_K TsgrK;
	T_TSGR_D TsgrD;
	T_ELEM_D ElemD;
	for (int i = 0; i < m_aSelStrings.GetSize(); i++)
	{
		TsgrK = pDoc->m_pAttrCtrl->GetTsgrKey(m_aSelStrings[i]);
		if (!pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD)) continue;
		if (TsgrD.aElemList.GetSize() == 0) continue;
		if (!pDoc->m_pAttrCtrl->GetElem(TsgrD.aElemList[0], ElemD)) continue;
		if (!pDoc->m_pAttrCtrl->ExistRpsc(ElemD.elpro)) continue;
		bExistRpsc = TRUE;
	}

	if(!pDoc->m_pDataCtrl->ConvertTsgr(m_aSelStrings, m_nStartNumber, csSuffix))return;
	//REG_STR_RES(IDS_DB_TAPERED_CONV_WARN, "[뭾댰] 긡?긬?뭚뽋긐깑?긵귩뭚뽋궸빾듂궥귡렄PC뭚뽋궸궇귡밪뗗륃뺪궼렔벍맯맟궠귢귏궧귪갃");
	if (bExistRpsc)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_TSGR_CONVERT_WARNING));
	}

	CDialogMove::OnOK();
}

BOOL CTpsgConvertDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_chkSuffix.SetCheck(1);
	m_editSuffix.SetWindowText(_T("1"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTpsgConvertDlg::SetSelStrings(CArray<CString,CString&>& aSelStrings)
{
	m_aSelStrings.Copy(aSelStrings);
}

void CTpsgConvertDlg::OnTreemenuTpsgUseSuffix() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkSuffix.GetCheck();
	GetDlgItem(IDC_TREEMENU_TPSG_SUFFIX_STATIC)->EnableWindow(nCheck == 1);
	GetDlgItem(IDC_TREEMENU_TPSG_SUFFIX)->EnableWindow(nCheck == 1);
}
