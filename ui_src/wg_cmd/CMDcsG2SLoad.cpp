// CMDcsG2SLoad.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDcsG2SLoad.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDcsG2SLoad dialog


CCMDcsG2SLoad::CCMDcsG2SLoad(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDcsG2SLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMDcsG2SLoad)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CCMDcsG2SLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDcsG2SLoad)
	DDX_Control(pDX, IDC_CMD_G2SLOAD_LEVEL_UNIT, m_unitLevelReaction);
	DDX_Control(pDX, IDC_CMD_G2SLOAD_LEVEL_EDIT, m_edtLevelReaction);
	DDX_Control(pDX, IDC_CMD_G2SLOAD_FILE_EDIT, m_edtFileName);
	//}}AFX_DATA_MAP
}

void CCMDcsG2SLoad::InitCtrlData()
{
	m_edtFileName.SetWindowText(_T(""));
	m_unitLevelReaction.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtLevelReaction.SetWindowText(_T("0"));
	((CButton*)GetDlgItem(IDC_CMD_G2SLOAD_VIEW_CHK))->SetCheck(TRUE);
}

BEGIN_MESSAGE_MAP(CCMDcsG2SLoad, CDialogMove)
	//{{AFX_MSG_MAP(CCMDcsG2SLoad)
	ON_BN_CLICKED(IDC_CMD_G2SLOAD_FILE_BTN, OnCmdG2sloadFileBtn)
	ON_BN_CLICKED(IDC_CMD_G2SLOAD_OK, OnCmdG2sloadOk)
	ON_BN_CLICKED(IDC_CMD_G2SLOAD_CANCEL, OnCmdG2sloadCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDcsG2SLoad message handlers


BOOL CCMDcsG2SLoad::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCtrlData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDcsG2SLoad::OnCmdG2sloadFileBtn() 
{
	CFileDialog fd(FALSE, _T(".G2S"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
								 _LS(IDS_WG_CMD__ADDD__SDS_Load_File___G2S____G2S__), NULL);
	if (fd.DoModal() == IDOK)
		m_edtFileName.SetWindowText(fd.GetPathName());
}

void CCMDcsG2SLoad::OnCmdG2sloadOk() 
{
	CString strFileName;
	CString strNoExtFileName;
	BOOL bFileCheck=TRUE;
	m_edtFileName.GetWindowText(strFileName);

	if(strFileName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Filename),MB_OK);		
		return;
	}
	
	// BN1 파일이 있는지 검사.
	if(strFileName.ReverseFind('.') != -1)
		strNoExtFileName = strFileName.Left(strFileName.GetLength() - 4);
	else
		strNoExtFileName = strFileName;

	CFileFind* pFileFind = new CFileFind();

	bFileCheck = pFileFind->FindFile(strNoExtFileName+_T(".BN1"), 0);

	if(!bFileCheck)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_SDS_Files_BN1_),MB_OK);
		delete pFileFind;
		return;
	}
	delete pFileFind;

	double dLevelFactor;
	CString strLevelReaction;
	m_edtLevelReaction.GetWindowText(strLevelReaction);
	if(strLevelReaction == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Level_for_Reactio),MB_OK);		
		return;
	}
	if(!CStrParser::GetFloatNumber(strLevelReaction, dLevelFactor)) return;

	// Display Wait Cursor
	CWaitCursor wait;
	
	// DB 함수 연결
	if(m_pDoc->m_pDataCtrl->Gen2Sds_Reacf(strFileName, dLevelFactor))
	{
		BOOL bCheck = this->IsDlgButtonChecked(IDC_CMD_G2SLOAD_VIEW_CHK);
		if(bCheck)
		{
			if(strFileName == strNoExtFileName)  // Extension 이 없는 파일이름.
				strFileName.Insert(strFileName.GetLength(),_T(".G2S"));

			CDBLib::RunTextEditor(strFileName, _LS(IDS_WG_CMD__ADDD__SDS_Loads_Data));
		}

		CDialogMove::OnOK();
	}
}

void CCMDcsG2SLoad::OnCmdG2sloadCancel() 
{
	CDialogMove::OnCancel();
}

