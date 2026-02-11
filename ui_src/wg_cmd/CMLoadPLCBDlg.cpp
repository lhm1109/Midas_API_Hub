// CMLoadPLCBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadPLCBDlg.h"

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
// CCMLoadPLCBDlg dialog


CCMLoadPLCBDlg::CCMLoadPLCBDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMLoadPLCBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadPLCBDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CCMLoadPLCBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadPLCBDlg)
	DDX_Control(pDX, IDC_CMD_PLCB_SELLOAD_LIST, m_lstSelLCase);
	DDX_Control(pDX, IDC_CMD_PLCB_LOAD_LIST, m_lstLCase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMLoadPLCBDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMLoadPLCBDlg)
	ON_LBN_DBLCLK(IDC_CMD_PLCB_LOAD_LIST, OnDblclkCmdPlcbLoadList)
	ON_LBN_DBLCLK(IDC_CMD_PLCB_SELLOAD_LIST, OnDblclkCmdPlcbSelloadList)
	ON_BN_CLICKED(IDC_CMD_PLCB_SELECT_BTN, OnCmdPlcbSelectBtn)
	ON_BN_CLICKED(IDC_CMD_PLCB_REMOVE_BTN, OnCmdPlcbRemoveBtn)
	ON_BN_CLICKED(IDC_CMD_PLCB_REM_BTN, OnCmdPlcbRemBtn)
	ON_BN_CLICKED(IDC_CMD_PLCB_OK, OnCmdPlcbOk)
	ON_BN_CLICKED(IDC_CMD_PLCB_CANCEL, OnCmdPlcbCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPLCBDlg message handlers

BOOL CCMLoadPLCBDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitStldData();

	if(m_pDoc->m_pAttrCtrl->ExistPlcb())
	{
		((CButton*)GetDlgItem(IDC_CMD_PLCB_REM_BTN))->EnableWindow(TRUE);
		CString strTemp;
		m_PlcbData.Initialize();
		m_StldData.Initialize();

		m_pDoc->m_pAttrCtrl->GetPlcb(m_PlcbData);

		for(int i=0; i<m_PlcbData.arLoad.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetStld(m_PlcbData.arLoad.GetAt(i), m_StldData);
			m_lstSelLCase.AddString(m_StldData.LoadCaseName);

			for(int j=0; j<m_lstLCase.GetCount(); j++)
			{
				m_lstLCase.GetText(j, strTemp);
				if(m_StldData.LoadCaseName == strTemp)
					m_lstLCase.DeleteString(j);
			}
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CMD_PLCB_REM_BTN))->EnableWindow(FALSE);
	}

	CtrlButtonOK();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadPLCBDlg::InitStldData()
{
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	if(nItemCount == 0) return;

	m_lstLCase.ResetContent();

	POSITION pos;
	CString str;
	CString strDist;
	int nCount;

	if(nItemCount)
	{
		T_STLD_K Key;
		T_STLD_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartStld();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextStld(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_STLD_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetStld(Key,Data);

			int nIndex = m_lstLCase.InsertString(nCount, Data.LoadCaseName);
		}
		delete []KeyBuf;
	}
}

void CCMLoadPLCBDlg::CtrlButtonOK()
{
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_CMD_PLCB_OK);
	if(pWnd == NULL) return;

	BOOL bEnable = m_lstSelLCase.GetCount()>0 ? TRUE : FALSE;
	pWnd->EnableWindow(bEnable);
}

void CCMLoadPLCBDlg::OnDblclkCmdPlcbLoadList() 
{
	OnCmdPlcbSelectBtn(); 	
}

void CCMLoadPLCBDlg::OnDblclkCmdPlcbSelloadList() 
{
	OnCmdPlcbRemoveBtn(); 
}

void CCMLoadPLCBDlg::OnCmdPlcbSelectBtn() 
{
	if(m_lstLCase.GetSelCount() == 0)
		return;

	int* lIndex = new int[m_lstLCase.GetSelCount()];	
	m_lstLCase.GetSelItems(m_lstLCase.GetSelCount(), lIndex);

	CString strSelString;

	for(int i=0; i<m_lstLCase.GetSelCount(); i++)
	{
		m_lstLCase.GetText(lIndex[i], strSelString);
		m_lstSelLCase.AddString(strSelString);
	}

	for(int i=m_lstLCase.GetSelCount()-1; i>=0; i--)
		m_lstLCase.DeleteString(lIndex[i]);

	m_lstLCase.SetCurSel(m_lstLCase.GetCount()-1);

	delete[] lIndex;
	
	CtrlButtonOK();
}

void CCMLoadPLCBDlg::OnCmdPlcbRemoveBtn() 
{
	if(m_lstSelLCase.GetSelCount() == 0)
		return;

	int* lIndex = new int[m_lstSelLCase.GetSelCount()];	
	m_lstSelLCase.GetSelItems(m_lstSelLCase.GetSelCount(), lIndex);

	for(int i=m_lstSelLCase.GetSelCount()-1; i >= 0; i--)
		m_lstSelLCase.DeleteString(lIndex[i]);

	delete[] lIndex;

	CString strSelTemp;
	CString strTemp;

	InitStldData();
	for(int i=0; i<m_lstSelLCase.GetCount(); i++)
	{
		m_lstSelLCase.GetText(i, strSelTemp);		
		for(int j=0; j<m_lstLCase.GetCount(); j++)
		{
			m_lstLCase.GetText(j, strTemp);
			if(strTemp == strSelTemp)
			{
				m_lstLCase.DeleteString(j);
				break;
			}
		}
	}

	if(m_lstLCase.GetCount() > 0)
		m_lstLCase.SetCurSel(m_lstLCase.GetCount()-1);

	CtrlButtonOK();
}

void CCMLoadPLCBDlg::OnCmdPlcbRemBtn() 
{
	if(m_pDoc->m_pDataCtrl->DelPlcb())
		OnOK();
}

void CCMLoadPLCBDlg::OnCmdPlcbOk() 
{
	if(!m_lstSelLCase.GetCount())
	{
		OnOK();
		return;
	}

	CString strLCaseName;
	m_PlcbData.Initialize();

	for(int i=0; i<m_lstSelLCase.GetCount(); i++)
	{
		m_lstSelLCase.GetText(i, strLCaseName);
		m_StldKey = m_pDoc->m_pAttrCtrl->GetStldKey(strLCaseName);
		m_PlcbData.arLoad.Add(m_StldKey);
	}
	
	if(m_pDoc->m_pDataCtrl->AddPlcb(m_PlcbData))
		OnOK();	
}

void CCMLoadPLCBDlg::OnCmdPlcbCancel() 
{
	OnCancel();	
}

