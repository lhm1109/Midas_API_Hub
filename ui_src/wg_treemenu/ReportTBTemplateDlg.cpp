#include "stdafx.h"

#include "ReportTBTemplateDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\FileCtrl.h"
// #include "..\wg_db\StagCtrl.h"
// #include "..\wg_db\DB_ST_DT_FIGR.h"


/////////////////////////////////////////////////////////////////////////////
// CReportTBTemplateDlg dialog

CReportTBTemplateDlg::CReportTBTemplateDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CReportTBTemplateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportTBTemplateDlg)
	m_strName     = _T("");
	//}}AFX_DATA_INIT
	m_strFilePath = _T("");

	m_bExteriorSet = FALSE;
	m_strNameOld = _T("");
}

CReportTBTemplateDlg::~CReportTBTemplateDlg()
{
}

void CReportTBTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportTBTemplateDlg)
	DDX_Text (pDX, IDC_TM_REPORT_TBTEMPLATE_NAME_EDT, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportTBTemplateDlg, CDialogMove)
	//{{AFX_MSG_MAP(CReportTBTemplateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportTBTemplateDlg message handlers

BOOL CReportTBTemplateDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

#if defined(_ORG)
	this->SetWindowText(CString(_T("Smart Report Table Template")));
#else
	this->SetWindowText(_LS(IDS_DB_DT_UTPL));
#endif

	if(!m_bExteriorSet)
		Data2Dlg();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless y
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CReportTBTemplateDlg::OnOK() 
{
	UpdateData(TRUE);
	Dlg2Data();

	CDialogMove::OnOK();
}

BOOL CReportTBTemplateDlg::Data2Dlg()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CString strName=_T(""), strTemp=_T("");
	int nIdx=1;

	m_strName = _LS(IDS_TM_DYNA_REPORT_Template);
	strName = m_strName;
	while(pDoc->m_pAttrCtrl->ExistUtpl(strName))
	{
		strTemp.Format(_T("%d"), nIdx++);
		strName = m_strName + strTemp;
	}
	m_strName = strName;

	return TRUE;
}

void CReportTBTemplateDlg::Dlg2Data()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	UpdateData(TRUE);  

	if(m_bExteriorSet)  // workstree 에서 수정시.
	{
		if(_tcsicmp(m_strNameOld, m_strName)!=0)
		{
			T_UTPL_D UtplD, UtplDOld; 
			UtplD.Initialize();
			UtplDOld.Initialize();
			if(!pDoc->m_pAttrCtrl->GetUtpl(m_strNameOld, UtplDOld))
			{ ASSERT(0); return; }

			UtplD = UtplDOld;
			UtplD.strName = m_strName;
			if(!pDoc->m_pDataCtrl->ModifyUtpl(m_strNameOld, UtplD))
			{ ASSERT(0); return; }
		}
	}
	else
	{
		T_UTPL_K  UtplK=0;
		T_UTPL_D  UtplD;
		
		ASSERT(m_strFilePath!=_T(""));
		UtplD.strName = m_strName;
		UtplD.strFilePath = m_strFilePath;
		
		if(!pDoc->m_pDataCtrl->AddUtpl(UtplD))
		{ 
			ASSERT(0);
		}
	}
}

void CReportTBTemplateDlg::SetData(UINT UtplK)
{
	ASSERT(UtplK!=0);
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	T_UTPL_D UtplD;
	if(!pDoc->m_pAttrCtrl->GetUtpl(UtplK, UtplD))
	{ ASSERT(0); return; }
	
	m_strName = UtplD.strName;
	m_strNameOld = UtplD.strName;
	
	m_bExteriorSet  = TRUE;
}

void CReportTBTemplateDlg::SetXmlFilePath(CString strFilePath)
{
	m_strFilePath = strFilePath;
	ASSERT(m_strFilePath!=_T(""));

	CString strModelPath=_T("");
	strModelPath = CDBDoc::GetDocPoint()->GetPathName();
	
	CFileCtrl FileCtrl(strModelPath);
	CString strTmp=_T("");
	strTmp = FileCtrl.GetFilePath();
	if(strTmp!=_T(""))
	{
		CString strFolder=_T("");
		strFolder.Format(_LS(IDS_TM_DYNA_REPORT__s_Table_Template_Files), strTmp);

		if(!FileCtrl.FileExists(strFolder)) // Table Template Files 폴더 없으면 생성
		{
			if(!::CreateDirectory(strFolder, 0)) 
			{
				CString csMsg;
				csMsg.Format(_LS(IDS_TM_DYNA_REPORT_FOLDER_ERROR), strFolder);
				AfxMessageBox(csMsg);
				return;
			}
		}

		// Table Template Files 폴더에 파일복사.
		CString strModFilePath=_T("");
		CString strName=_T(""), strTemp=_T("");
		int nIdx=1;
		strModFilePath = strFolder + _T("\\")+_LS(IDS_TM_DYNA_REPORT_Template);
		strName = strModFilePath;
		while(FileCtrl.FileExists(strName+_T(".xml")))
		{
			strTemp.Format(_T("%d"), nIdx++);
			strName = strModFilePath + strTemp;
		}
		strModFilePath = strName + _T(".xml");

		CopyFile(strFilePath, strModFilePath, FALSE);

		FileCtrl.SetFilePathAndName(strModFilePath);
		m_strFilePath = FileCtrl.GetFileName();
		ASSERT(m_strFilePath!=_T(""));
	}
}

BOOL CReportTBTemplateDlg::PreTranslateMessage(MSG* pMsg) 
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

BOOL CReportTBTemplateDlg::IsSymbol(LPCTSTR lpStr)
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

CString CReportTBTemplateDlg::DeleteSymbol(LPCTSTR lpStr)
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
