// DgnImportForceDlg .cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnImportForceDlg.h"

#include "DgnStruct.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\DesignForce.h"
#include "..\wg_db\FileCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnImportForceDlg ::CDgnImportForceDlg (CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnImportForceDlg ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMatDlg)
	m_bImport = FALSE;
	m_strExistFile = _T("");
	//}}AFX_DATA_INIT
}

void CDgnImportForceDlg ::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatDlg)
	DDX_Text(pDX, IDC_STATIC_USE, m_strExistFile);
	DDX_Control(pDX, IDC_IMP_FOR_BROWSE, m_ImportBtn);
	DDX_Check(pDX, IDC_IMP_FOR_CHECK, m_bImport);	
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnImportForceDlg , CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConMatDlg)
	ON_BN_CLICKED(IDC_IMP_FOR_BROWSE, OnImportBtn)
	ON_BN_CLICKED(IDC_IMP_FOR_CHECK, OnImportCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg message handlers

BOOL CDgnImportForceDlg ::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// T_IPFO_D : 만들 구조체.

	// DB에 저장된 데이터가 있으면 읽어옮.
	
	if(pDoc->m_pAttrCtrl->ExistIpfo())
	{
		T_IPFO_D IpfoD; IpfoD.Initialize();  
		pDoc->m_pAttrCtrl->GetIpfo(IpfoD);

		//m_strFileName = IpfoD.strPathName;
		//ConvertCharStr(IpfoD.strPathName, m_strFileName, sizeof(IpfoD.strPathName));
		m_bImport = IpfoD.bImport;
	}	
	
	if(ExistImportForceFile()) m_strExistFile = _LS(IDS_DGN_IMPORT_FORCE_Possible);
	else                       m_strExistFile = _LS(IDS_DGN_IMPORT_FORCE_Impossible);   

	UpdateData(FALSE);

	return TRUE;
}

void CDgnImportForceDlg ::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnImportForceDlg ::OnOK() 
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 여기서 DB에 저장.
	
	T_IPFO_D IpfoD; IpfoD.Initialize();  
	
	//ConvertStrChar(m_strFileName, IpfoD.strPathName, sizeof(IpfoD.strPathName));
	//IpfoD.strPathName = m_strFileName;
	IpfoD.bImport = m_bImport;  
	
	BOOL bCheck = pDoc->m_pDataCtrl->AddIpfo(IpfoD);

	if(bCheck) CDialogMove::OnOK();
	

	CDialogMove::OnOK();
}

void CDgnImportForceDlg::OnImportBtn()
{
	//TCHAR pathName[500];
	CDesignForce ImportForce;
	
	if(ImportForce.RunAnalysis())
		m_strExistFile = _LS(IDS_DGN_IMPORT_FORCE_Possible);
	else
		m_strExistFile = _LS(IDS_DGN_IMPORT_FORCE_Impossible);   

	UpdateData(FALSE);
}

void CDgnImportForceDlg::OnImportCheck()
{
	UpdateData(TRUE);  
}

BOOL CDgnImportForceDlg::ExistImportForceFile()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CString strPathName=CDBDoc::GetDocPoint()->GetPathName();
	CFileCtrl FileCtrl(strPathName);  
	CString strDesignFileName=FileCtrl.GetFilePathAndNameWithoutExtension()+_T(".cd14");  

	CFileStatus status;  
	if(CFile::GetStatus(strDesignFileName, status))return TRUE;

	return FALSE;
}
 