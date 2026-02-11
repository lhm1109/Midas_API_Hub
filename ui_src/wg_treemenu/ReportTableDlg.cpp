#include "stdafx.h"

#include "ReportTableDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
// #include "..\wg_db\FileCtrl.h"
// #include "..\wg_db\StagCtrl.h"
// #include "..\wg_db\DB_ST_DT_FIGR.h"


/////////////////////////////////////////////////////////////////////////////
// CReportTableDlg dialog

CReportTableDlg::CReportTableDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CReportTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportTableDlg)
	m_strName     = _T("");
	//}}AFX_DATA_INIT
	m_strFilePath = _T("");

	m_bExteriorSet = FALSE;
	m_strNameOld = _T("");
}

CReportTableDlg::~CReportTableDlg()
{
}

void CReportTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportTableDlg)
	DDX_Text (pDX, IDC_TM_REPORT_TABLE_NAME_EDT, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportTableDlg, CDialogMove)
	//{{AFX_MSG_MAP(CReportTableDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportTableDlg message handlers

BOOL CReportTableDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

#if defined(_ORG)
	this->SetWindowText(CString(_T("Smart Report Table")));
#else
	this->SetWindowText(_LS(IDS_DB_DT_UTBL));
#endif
	
	if(!m_bExteriorSet)
		Data2Dlg();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless y
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CReportTableDlg::OnOK() 
{
	UpdateData(TRUE);
	Dlg2Data();

	CDialogMove::OnOK();
}

BOOL CReportTableDlg::Data2Dlg()
{
	ASSERT(0);
//   CDBDoc* pDoc = CDBDoc::GetDocPoint();
//   ASSERT(pDoc);
// 
//   CString strName="", strTemp="";
//   int nIdx=1;
// 
//   m_strName = "Template";
//   strName = m_strName;
//   while(pDoc->m_pAttrCtrl->ExistUtbl(strName))
//   {
//     strTemp.Format("%d", nIdx++);
//     strName = m_strName + strTemp;
//   }
//   m_strName = strName;

	return TRUE;
}

void CReportTableDlg::Dlg2Data()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	UpdateData(TRUE);  

	if(m_bExteriorSet)  // workstree 에서 수정시.
	{
		if(_tcsicmp(m_strNameOld, m_strName)!=0)
		{
			T_UTBL_D UtblD, UtblDOld; 
			UtblD.Initialize();
			UtblDOld.Initialize();
			if(!pDoc->m_pAttrCtrl->GetUtbl(m_strNameOld, UtblDOld))
			{ ASSERT(0); return; }

			UtblD = UtblDOld;
			UtblD.strName = m_strName;
			if(!pDoc->m_pDataCtrl->ModifyUtbl(m_strNameOld, UtblD))
			{ ASSERT(0); return; }
		}
	}
	else
	{
		ASSERT(0);
//     T_UTBL_K  UtblK=0;
//     T_UTBL_D  UtblD;
//     
//     ASSERT(m_strFilePath!="");
//     UtblD.strName = m_strName;
//     UtblD.strFilePath = m_strFilePath;
//     
//     if(!pDoc->m_pDataCtrl->AddUtbl(UtblD))
//     { 
//       ASSERT(0);
//     }
	}
}

void CReportTableDlg::SetData(UINT UtblK)
{
	ASSERT(UtblK!=0);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	T_UTBL_D UtblD;
	if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD))
	{ ASSERT(0); return; }
	
	m_strName = UtblD.strName;
	m_strNameOld = UtblD.strName;
	
	m_bExteriorSet  = TRUE;
}

void CReportTableDlg::SetXmlFilePath(CString strFilePath)
{
	m_strFilePath = strFilePath;
	ASSERT(m_strFilePath!=_T(""));
}

BOOL CReportTableDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		// Figure 이름에 기호나 특수문자를 허용하지 않음.
		UpdateData(TRUE);
		if(IsSymbol(m_strName))
		{
			AfxMessageBox(_LS(IDS_TM_DYNA_REPORT_NAME_ERROR));
			m_strName = DeleteSymbol(m_strName);
			UpdateData(FALSE);
		}
	}

	return CDialogMove::PreTranslateMessage(pMsg);
}

BOOL CReportTableDlg::IsSymbol(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	int r=-1;
	
	r = strTemp.Find(_T("~"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("!"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("@"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("#"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("$"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("%"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("^"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("&"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("*"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("("));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(")"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("-"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("_"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("="));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("+"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("\\"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("|"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("["));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("{"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("]"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("}"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(";"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(":"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("\'"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("\""));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(","));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("<"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("."));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T(">"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("/"));
	if(r>=0) return TRUE;
	r = strTemp.Find(_T("?"));
	if(r>=0) return TRUE;
	
	return FALSE; 
}

CString CReportTableDlg::DeleteSymbol(LPCTSTR lpStr)
{
	CString strTemp = lpStr;
	strTemp.Replace(_T("_"), _T("")); 
	strTemp.Replace(_T("~"),_T(""));
	strTemp.Replace(_T("!"),_T(""));
	strTemp.Replace(_T("@"),_T(""));
	strTemp.Replace(_T("#"),_T(""));
	strTemp.Replace(_T("$"),_T(""));
	strTemp.Replace(_T("%"),_T(""));
	strTemp.Replace(_T("^"),_T(""));
	strTemp.Replace(_T("&"),_T(""));
	strTemp.Replace(_T("*"),_T(""));
	strTemp.Replace(_T("("),_T(""));
	strTemp.Replace(_T(")"),_T(""));
	strTemp.Replace(_T("-"),_T(""));
	strTemp.Replace(_T("+"),_T(""));
	strTemp.Replace(_T("="),_T(""));
	strTemp.Replace(_T("\\"),_T(""));
	strTemp.Replace(_T("|"),_T(""));
	strTemp.Replace(_T("["),_T(""));
	strTemp.Replace(_T("{"),_T(""));
	strTemp.Replace(_T("]"),_T(""));
	strTemp.Replace(_T("}"),_T(""));
	strTemp.Replace(_T(":"),_T(""));
	strTemp.Replace(_T(";"),_T(""));
	strTemp.Replace(_T("\'"),_T(""));
	strTemp.Replace(_T("\""),_T(""));
	strTemp.Replace(_T("<"),_T(""));
	strTemp.Replace(_T(","),_T(""));
	strTemp.Replace(_T("."),_T(""));
	strTemp.Replace(_T(">"),_T(""));
	strTemp.Replace(_T("/"),_T(""));
	strTemp.Replace(_T("?"),_T(""));
	
	return strTemp; 
}
