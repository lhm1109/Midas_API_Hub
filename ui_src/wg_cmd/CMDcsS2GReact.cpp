// CMDcsS2GReact.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDcsS2GReact.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDcsS2GReact dialog


CCMDcsS2GReact::CCMDcsS2GReact(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDcsS2GReact::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	//{{AFX_DATA_INIT(CCMDcsS2GReact)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMDcsS2GReact::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDcsS2GReact)
	DDX_Control(pDX, IDC_CMD_G2SREACT_SELSTORY_LIST, m_lstSelStory);
	DDX_Control(pDX, IDC_CMD_G2SREACT_STORY_LIST, m_lstStory);
	DDX_Control(pDX, IDC_CMD_G2SREACT_FILE_EDIT, m_edtFileName);
	//}}AFX_DATA_MAP
}

void CCMDcsS2GReact::InitCtrlData()
{
	m_edtFileName.SetWindowText(_T(""));
	InitStorData();
}

void CCMDcsS2GReact::InitStorData()
{
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountStor();
	if(nItemCount == 0) return;

	m_lstStory.ResetContent();

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

			int nIndex = m_lstStory.InsertString(nCount, Data.StoryName);
		}
		delete []KeyBuf;
	}
}

BEGIN_MESSAGE_MAP(CCMDcsS2GReact, CDialogMove)
	//{{AFX_MSG_MAP(CCMDcsS2GReact)
	ON_BN_CLICKED(IDC_CMD_G2SREACT_STORY_SELBTN, OnCmdG2sreactStorySelbtn)
	ON_BN_CLICKED(IDC_CMD_G2SREACT_STORY_REMOVEBTN, OnCmdG2sreactStoryRemovebtn)
	ON_LBN_DBLCLK(IDC_CMD_G2SREACT_STORY_LIST, OnDblclkCmdG2sreactStoryList)
	ON_LBN_DBLCLK(IDC_CMD_G2SREACT_SELSTORY_LIST, OnDblclkCmdG2sreactSelstoryList)
	ON_BN_CLICKED(IDC_CMD_G2SREACT_FILE_BTN, OnCmdG2sreactFileBtn)
	ON_BN_CLICKED(IDC_CMD_G2SREACT_CANCEL, OnCmdG2sreactCancel)
	ON_BN_CLICKED(IDC_CMD_G2SREACT_OK, OnCmdG2sreactOk)
	ON_BN_CLICKED(IDC_CMD_G2SREACT_STORY_SELECTALL, OnCmdG2sreactStorySelectall)
	ON_BN_CLICKED(IDC_CMD_G2SREACT_STORY_REMOVEALL, OnCmdG2sreactStoryRemoveall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDcsS2GReact message handlers

BOOL CCMDcsS2GReact::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCtrlData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDcsS2GReact::OnCmdG2sreactStorySelbtn() 
{
	if(m_lstStory.GetSelCount() == 0)
		return;

	int* lIndex = new int[m_lstStory.GetSelCount()];	
	m_lstStory.GetSelItems(m_lstStory.GetSelCount(), lIndex);

	CString strSelString;

	for(int i=0; i<m_lstStory.GetSelCount(); i++)
	{
		m_lstStory.GetText(lIndex[i], strSelString);
		m_lstSelStory.AddString(strSelString);
	}

	for(int i=m_lstStory.GetSelCount()-1; i>=0; i--)
		m_lstStory.DeleteString(lIndex[i]);

	m_lstStory.SetCurSel(m_lstStory.GetCount()-1);

	delete[] lIndex;	
}

void CCMDcsS2GReact::OnCmdG2sreactStoryRemovebtn() 
{
	if(m_lstSelStory.GetSelCount() == 0)
		return;

	int* lIndex = new int[m_lstSelStory.GetSelCount()];	
	m_lstSelStory.GetSelItems(m_lstSelStory.GetSelCount(), lIndex);

	for(int i=m_lstSelStory.GetSelCount()-1; i >= 0; i--)
		m_lstSelStory.DeleteString(lIndex[i]);

	delete[] lIndex;

	CString strSelTemp;
	CString strTemp;

	InitStorData();
	for(int i=0; i<m_lstSelStory.GetCount(); i++)
	{
		m_lstSelStory.GetText(i, strSelTemp);		
		for(int j=0; j<m_lstStory.GetCount(); j++)
		{
			m_lstStory.GetText(j, strTemp);
			if(strTemp == strSelTemp)
			{
				m_lstStory.DeleteString(j);
				break;
			}
		}
	}

	if(m_lstStory.GetCount() > 0)
		m_lstStory.SetCurSel(m_lstStory.GetCount()-1);
}

void CCMDcsS2GReact::OnDblclkCmdG2sreactStoryList() 
{
	OnCmdG2sreactStorySelbtn(); 	
}

void CCMDcsS2GReact::OnDblclkCmdG2sreactSelstoryList() 
{
	OnCmdG2sreactStoryRemovebtn(); 	
}

void CCMDcsS2GReact::OnCmdG2sreactFileBtn() 
{
	CFileDialog fd(TRUE, _T(".BN1"), NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
								 _LS(IDS_WG_CMD__ADDD__SDS_File___BN1___BN2____BN1___BN2), NULL);
	if (fd.DoModal() == IDOK)
	{
		CString strFileName = fd.GetPathName();
		m_edtFileName.SetWindowText(strFileName);
	}			
}

void CCMDcsS2GReact::OnCmdG2sreactCancel() 
{
	CDialogMove::OnCancel();
}

void CCMDcsS2GReact::OnCmdG2sreactOk() 
{
	BOOL bFileCheck=TRUE;
	CString strFileName;
	CString strNoExtFileName;
	m_edtFileName.GetWindowText(strFileName);

	if(strFileName.ReverseFind('.') != -1)
	{
		strNoExtFileName = strFileName.Left(strFileName.GetLength() - 4);
	}

	CFileFind* pFileFind = new CFileFind();

	bFileCheck = pFileFind->FindFile(strNoExtFileName+_T(".BN1"), 0);
	bFileCheck = pFileFind->FindFile(strNoExtFileName+_T(".BN2"), 0);

	if(!bFileCheck)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_SDS_Files_BN1_BN2_),MB_OK);
		delete pFileFind;
		return;
	}
	delete pFileFind;

	if(m_lstSelStory.GetCount() == 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_StoryName));
		return;
	}

	CArray<T_STOR_K, T_STOR_K> arStoryKeyList;
	CString strStoryName;

	for(int i=0; i<m_lstSelStory.GetCount(); i++)
	{
		m_lstSelStory.GetText(i, strStoryName);
		arStoryKeyList.Add(m_pDoc->m_pAttrCtrl->GetStorKey(strStoryName));
	}

	// Display Wait Cursor
	CWaitCursor wait;

	// DB랑 연결하는 함수.
	if(m_pDoc->m_pDataCtrl->Sds2Gen_Load(strFileName, arStoryKeyList))	
		CDialogMove::OnOK();
}


void CCMDcsS2GReact::OnCmdG2sreactStorySelectall() 
{
	CString strStoryName;
	for(int i=0; i < m_lstStory.GetCount(); i++)
	{
		m_lstStory.GetText(i, strStoryName);
		m_lstSelStory.AddString(strStoryName);
	}

	m_lstStory.ResetContent();	
}

void CCMDcsS2GReact::OnCmdG2sreactStoryRemoveall() 
{
	m_lstSelStory.ResetContent();
	InitStorData();	
}
