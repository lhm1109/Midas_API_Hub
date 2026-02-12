// EtcPrjConf.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtcPrjConf.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcPrjConf dialog


CEtcPrjConf::CEtcPrjConf(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtcPrjConf::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtcPrjConf)
	m_csPrjName = _T("");
	m_csUsrName = _T("");
	m_csRevision = _T("");
	m_csEmail = _T("");
	m_csAddress = _T("");
	m_csTelephone = _T("");
	m_csFax = _T("");
	m_csClient = _T("");
	m_csTitle = _T("");
	m_csFileName = _T("");
	m_csCreated = _T("");
	m_csDirectory = _T("");
	m_csModified = _T("");
	m_csFileSize = _T("");
	m_csEngName = _T("");
	m_csEngDate = _T("");
	m_csChk1Name = _T("");
	m_csChk1Date = _T("");
	m_csChk2Name = _T("");
	m_csChk2Date = _T("");
	m_csChk3Name = _T("");
	m_csChk3Date = _T("");
	m_csAprName = _T("");
	m_csAprDate = _T("");
	m_csComment = _T("");
	//}}AFX_DATA_INIT
}


void CEtcPrjConf::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcPrjConf)
	DDX_Text(pDX, IDC_ETC_PC_PNAME, m_csPrjName);
	DDX_Text(pDX, IDC_ETC_PC_UNAME, m_csUsrName);
	DDX_Text(pDX, IDC_ETC_PC_REVISION, m_csRevision);
	DDX_Text(pDX, IDC_ETC_PC_EMAIL, m_csEmail);
	DDX_Text(pDX, IDC_ETC_PC_ADDRESS, m_csAddress);
	DDX_Text(pDX, IDC_ETC_PC_TELEPHONE, m_csTelephone);
	DDX_Text(pDX, IDC_ETC_PC_FAX, m_csFax);
	DDX_Text(pDX, IDC_ETC_PC_CLIENT, m_csClient);
	DDX_Text(pDX, IDC_ETC_PC_TITLE, m_csTitle);
	DDX_Text(pDX, IDC_ETC_PC_FILENAME, m_csFileName);
	DDX_Text(pDX, IDC_ETC_PC_CREATED, m_csCreated);
	DDX_Text(pDX, IDC_ETC_PC_DIRECTORY, m_csDirectory);
	DDX_Text(pDX, IDC_ETC_PC_MODIFIED, m_csModified);
	DDX_Text(pDX, IDC_ETC_PC_FILESIZE, m_csFileSize);
	DDX_Text(pDX, IDC_ETC_PC_EGN_NAME, m_csEngName);
	DDX_Text(pDX, IDC_ETC_PC_ENG_DATE, m_csEngDate);
	DDX_Text(pDX, IDC_ETC_PC_CHK1_NAME, m_csChk1Name);
	DDX_Text(pDX, IDC_ETC_PC_CHK1_DATE, m_csChk1Date);
	DDX_Text(pDX, IDC_ETC_PC_CHK2_NAME, m_csChk2Name);
	DDX_Text(pDX, IDC_ETC_PC_CHK2_DATE, m_csChk2Date);
	DDX_Text(pDX, IDC_ETC_PC_CHK3_NAME, m_csChk3Name);
	DDX_Text(pDX, IDC_ETC_PC_CHK3_DATE, m_csChk3Date);
	DDX_Text(pDX, IDC_ETC_PC_APR_NAME, m_csAprName);
	DDX_Text(pDX, IDC_ETC_PC_APR_DATE, m_csAprDate);
	DDX_Text(pDX, IDC_ETC_PC_COMMENT, m_csComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEtcPrjConf, CDialogMove)
	//{{AFX_MSG_MAP(CEtcPrjConf)
	ON_BN_CLICKED(IDC_ETC_PC_BTN_DETAIL, OnEtcPcBtnDetail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcPrjConf message handlers

BOOL CEtcPrjConf::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();
	
	InitDialogData();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcPrjConf::SetProjectName(const CString& csName)
{
	if (csName.IsEmpty()) return;

	T_PJCF_D data;
	if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPjcf(data))
		data.Initialize();

	data.strProject = csName;
	if (!CDBDoc::GetDocPoint()->m_pDataCtrl->AddPjcf(data)) return;
}

void CEtcPrjConf::InitDialogData()
{
	T_PJCF_D data;
	if (!m_pDoc->m_pAttrCtrl->GetPjcf(data)) 
		data.Initialize();
	m_csPrjName = data.strProject;
	m_csRevision = data.strRevision;
	m_csUsrName = data.strUser;
	m_csEmail = data.strMail;
	m_csAddress = data.strAddress;
	m_csTelephone = data.strPhone;
	m_csFax = data.strFax;
	m_csClient = data.strClient;
	m_csTitle = data.strTitle;

	CString csPath = m_pDoc->GetPathName();
	if (csPath.IsEmpty())
	{
		m_csFileName = _T("");
		m_csCreated = _T("");
		m_csDirectory = _T("");
		m_csModified = _T("");
		m_csFileSize = _T("");
	}
	else
	{
		CFileCtrl file(csPath);
		CFileStatus status;
		file.GetFileStatus(status);
		m_csFileName = file.GetFileNameWithoutExtension();
		m_csCreated = status.m_ctime.Format(_T("%d/%m/%Y  %H:%M"));
		m_csDirectory = file.GetFilePath();
		int nLast = m_csDirectory.GetLength()-1;
		if (m_csDirectory.GetAt(nLast) == '\\') 
			m_csDirectory = m_csDirectory.Left(nLast);
		m_csModified = status.m_mtime.Format(_T("%d/%m/%Y  %H:%M"));
		int nSize = status.m_size/1024;
		if (nSize <= 0) 
			m_csFileSize.Format(_T("%d %s"), status.m_size, (status.m_size > 1) ? _T("bytes") : _T("byte"));
		else m_csFileSize.Format(_T("%d %s"), nSize, (nSize > 1) ? _T("Kbytes") : _T("Kbyte"));
	}

	m_csEngName = data.strReviewName[0];
	m_csEngDate = data.strReviewDate[0];
	m_csChk1Name = data.strReviewName[1];
	m_csChk1Date = data.strReviewDate[1];
	m_csChk2Name = data.strReviewName[2];
	m_csChk2Date = data.strReviewDate[2];
	m_csChk3Name = data.strReviewName[3];
	m_csChk3Date = data.strReviewDate[3];
	m_csAprName = data.strReviewName[4];
	m_csAprDate = data.strReviewDate[4];
	m_csComment = data.strComment;
}

void CEtcPrjConf::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;

	T_PJCF_D data;

	data.Initialize();

	data.strProject = m_csPrjName;
	data.strRevision = m_csRevision;
	data.strUser = m_csUsrName;
	data.strMail = m_csEmail;
	data.strAddress = m_csAddress;
	data.strPhone = m_csTelephone;
	data.strFax = m_csFax;
	data.strClient = m_csClient;
	data.strTitle = m_csTitle;
	data.strReviewName[0] = m_csEngName;
	data.strReviewDate[0] = m_csEngDate;
	data.strReviewName[1] = m_csChk1Name;
	data.strReviewDate[1] = m_csChk1Date;
	data.strReviewName[2] = m_csChk2Name;
	data.strReviewDate[2] = m_csChk2Date;
	data.strReviewName[3] = m_csChk3Name;
	data.strReviewDate[3] = m_csChk3Date;
	data.strReviewName[4] = m_csAprName;
	data.strReviewDate[4] = m_csAprDate;
	data.strComment = m_csComment;

	if (!m_pDoc->m_pDataCtrl->AddPjcf(data)) return;
	
	CDialogMove::OnOK();
}

void CEtcPrjConf::OnEtcPcBtnDetail() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_QUERY_PROJECT,0));
}
