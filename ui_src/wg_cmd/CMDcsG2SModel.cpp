// CMDcsG2SModel.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDcsG2SModel.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDcsG2SModel dialog


CCMDcsG2SModel::CCMDcsG2SModel(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDcsG2SModel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMDcsG2SModel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CCMDcsG2SModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDcsG2SModel)
	DDX_Control(pDX, IDC_CMD_G2SMODEL_FACTOR_EDIT, m_edtStiffnessFactor);
	DDX_Control(pDX, IDC_CMD_G2SMODEL_STORY_CBO, m_cboStory);
	DDX_Control(pDX, IDC_CMD_G2SMODEL_FILE_EDIT, m_edtFileName);
	//}}AFX_DATA_MAP
}

void CCMDcsG2SModel::InitCtrlData()
{
	m_edtFileName.SetWindowText(_T(""));
	m_edtStiffnessFactor.SetWindowText(_T("6.0"));
	((CButton*)GetDlgItem(IDC_CMD_G2SMODEL_VIEW_CHK))->SetCheck(TRUE);
	InitStorData();
}

void CCMDcsG2SModel::InitStorData()
{
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountStor();
	if(nItemCount == 0) return;

	m_cboStory.ResetContent();

	POSITION pos;
	CString str;
	CString strDist;
	int nCount;

	if(nItemCount)
	{
		T_STOR_K Key;
		T_STOR_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartStor();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextStor(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_STOR_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetStor(Key,Data);

			int nIndex = m_cboStory.InsertString(nCount, Data.StoryName);
		}
		delete []KeyBuf;
	}
}

BEGIN_MESSAGE_MAP(CCMDcsG2SModel, CDialogMove)
	//{{AFX_MSG_MAP(CCMDcsG2SModel)
	ON_BN_CLICKED(IDC_CMD_G2SMODEL_FILE_BTN, OnCmdG2smodelFileBtn)
	ON_BN_CLICKED(IDC_CMD_G2SMODEL_OK, OnCmdG2smodelOk)
	ON_BN_CLICKED(IDC_CMD_G2SMODEL_CANCEL, OnCmdG2smodelCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDcsG2SModel message handlers

BOOL CCMDcsG2SModel::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCtrlData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDcsG2SModel::OnCmdG2smodelFileBtn() 
{
	CFileDialog fd(FALSE, _T(".SDI"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
								 _LS(IDS_WG_CMD__ADDD__SDS_Model_File___SDI____SDI__), NULL);
	if (fd.DoModal() == IDOK)
		m_edtFileName.SetWindowText(fd.GetPathName());
}

void CCMDcsG2SModel::OnCmdG2smodelOk() 
{
	CString strFileName;
//	CString strNoExtFileName;
//	BOOL bFileCheck=TRUE;
	m_edtFileName.GetWindowText(strFileName);


	if(strFileName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Filename),MB_OK);		
		return;
	}

/*	// SDI 파일이 있는지 검사.
	if(strFileName.ReverseFind('.') != -1)
		strNoExtFileName = strFileName.Left(strFileName.GetLength() - 4);
	else
		strNoExtFileName = strFileName;

	CFileFind* pFileFind = new CFileFind();

	bFileCheck = pFileFind->FindFile(strNoExtFileName+_T(".SDI"), 0);

	if(!bFileCheck)
	{
		AfxMessageBox(_T("Error : Check SDS Files(SDI)"),MB_OK);
		delete pFileFind;
		return;
	}
	delete pFileFind;     */


	CString strStoryName;
	int nStoryKey=0;

	m_cboStory.GetWindowText(strStoryName);
	if(strStoryName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Story_Name),MB_OK);		
		return;
	}
	else
		nStoryKey = m_pDoc->m_pAttrCtrl->GetStorKey(strStoryName);

	double dStiffnessFactor;
	CString strStiffnessFactor;
	m_edtStiffnessFactor.GetWindowText(strStiffnessFactor);
	if(strStiffnessFactor == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Bending_Stiffness),MB_OK);		
		return;
	}
	if(!CStrParser::GetFloatNumber(strStiffnessFactor, dStiffnessFactor)) return;

	// Display Wait Cursor
	CWaitCursor wait;
	
	// DB 함수 연결
	if(m_pDoc->m_pDataCtrl->Gen2Sds_Model(strFileName, nStoryKey, dStiffnessFactor))
	{
		BOOL bCheck = this->IsDlgButtonChecked(IDC_CMD_G2SMODEL_VIEW_CHK);
		if(bCheck)
		{
//			if(strFileName == strNoExtFileName)  // Extension 이 없는 파일이름.
//				strFileName.Insert(strFileName.GetLength(),_T(".SDI"));

			CDBLib::RunTextEditor(strFileName, _LS(IDS_WG_CMD__ADDD__SDS_Model_Data));
		}

		CDialogMove::OnOK();	
	}
}

void CCMDcsG2SModel::OnCmdG2smodelCancel() 
{
	CDialogMove::OnCancel();
}
