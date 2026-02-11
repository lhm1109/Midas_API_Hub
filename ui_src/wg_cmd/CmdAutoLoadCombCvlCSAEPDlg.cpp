// CMLoadCombTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlCSAEPDlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlCSAEPDlg dialog
CCmdAutoLoadCombCvlCSAEPDlg::CCmdAutoLoadCombCvlCSAEPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadCombCvlCSAEPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlCSAEPDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_aStldK.RemoveAll();
}

CCmdAutoLoadCombCvlCSAEPDlg::~CCmdAutoLoadCombCvlCSAEPDlg()
{
}

void CCmdAutoLoadCombCvlCSAEPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectioanagerMainDlg)
	DDX_Control(pDX, IDC_CMD_CSA_DEFINE_EP_LST,                 m_lstEp);
	DDX_Control(pDX, IDC_CMD_CSA_DEFINE_EP1_LST,                m_lstEp1);
	DDX_Control(pDX, IDC_CMD_CSA_DEFINE_EP2_LST,                m_lstEp2);
	DDX_Control(pDX, IDC_CMD_CSA_DEFINE_EP3_LST,                m_lstEp3);
	DDX_Control(pDX, IDC_CMD_CSA_DEFINE_EP4_LST,                m_lstEp4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlCSAEPDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlCSAEPDlg)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP1_ADD_BTN,      OnCmdCSAEP1AddBtn)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP1_DEL_BTN,      OnCmdCSAEP1DelBtn)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP2_ADD_BTN,      OnCmdCSAEP2AddBtn)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP2_DEL_BTN,      OnCmdCSAEP2DelBtn)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP3_ADD_BTN,      OnCmdCSAEP3AddBtn)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP3_DEL_BTN,      OnCmdCSAEP3DelBtn)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP4_ADD_BTN,      OnCmdCSAEP4AddBtn)
	ON_BN_CLICKED(IDC_CMD_CSA_DEFINE_EP4_DEL_BTN,      OnCmdCSAEP4DelBtn)
	ON_LBN_DBLCLK(IDC_CMD_CSA_DEFINE_EP1_LST,          OnCmdCSAEP1Dblclk)
	ON_LBN_DBLCLK(IDC_CMD_CSA_DEFINE_EP2_LST,          OnCmdCSAEP2Dblclk)
	ON_LBN_DBLCLK(IDC_CMD_CSA_DEFINE_EP3_LST,          OnCmdCSAEP3Dblclk)
	ON_LBN_DBLCLK(IDC_CMD_CSA_DEFINE_EP4_LST,          OnCmdCSAEP4Dblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlCSAEPDlg message handlers

BOOL CCmdAutoLoadCombCvlCSAEPDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int nSize = arKeyList.GetSize();

	for(int i=0; i<nSize; i++)
	{
		T_STLD_D rData; rData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetStld(arKeyList[i], rData)) { ASSERT(0); return FALSE; }

		if(rData.LoadCaseType==_T("EP")) // General earth pressure load
		{
			m_aStldK.Add(arKeyList[i]);
			m_aSelFlag.Add(FALSE);
		}
	}

	int nAllSize = m_aStldK.GetSize();
	if(nAllSize <= 0) return FALSE;
	
	m_lstEp.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		T_STLD_D rData; rData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
		m_lstEp.SetItemData(m_lstEp.AddString(rData.LoadCaseName), i);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnOK() 
{
	m_aStldEp1.RemoveAll();
	m_aStldEp2.RemoveAll();
	m_aStldEp3.RemoveAll();
	m_aStldEp4.RemoveAll();

	int nSize = m_lstEp1.GetCount();
	for(int i=0; i<nSize; i++)
	{
		m_aStldEp1.Add(m_aStldK[m_lstEp1.GetItemData(i)]);
	}

	nSize = m_lstEp2.GetCount();
	for(int i=0; i<nSize; i++)
	{
		m_aStldEp2.Add(m_aStldK[m_lstEp2.GetItemData(i)]);
	}

	nSize = m_lstEp3.GetCount();
	for(int i=0; i<nSize; i++)
	{
		m_aStldEp3.Add(m_aStldK[m_lstEp3.GetItemData(i)]);
	}

	nSize = m_lstEp4.GetCount();
	for(int i=0; i<nSize; i++)
	{
		m_aStldEp4.Add(m_aStldK[m_lstEp4.GetItemData(i)]);
	}

	CDialogMove::OnOK();
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP1AddBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if(nAllSize <= 0) return;

	CArray<int, int> aIndex; aIndex.RemoveAll();
	CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
	aSelTmpFlag.Copy(m_aSelFlag);
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp.GetItemData(aIndex[i])] = TRUE;
		m_lstEp.DeleteString(aIndex[i]);
	}
	
	for (int i=0; i<nAllSize; i++)
	{
		for(int j=0; j<m_aStldEp1.GetSize(); j++)
		{
			if(m_aStldK[i]==m_aStldEp1[j]) aSelTmpFlag[i] = FALSE;
		}
	}
	
	m_lstEp1.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
			m_lstEp1.SetItemData(m_lstEp1.AddString(rData.LoadCaseName), i);
		}
	}
	
	m_aStldEp1.RemoveAll();
	for (int i=0; i<nAllSize; i++)
	{
		if(!aSelTmpFlag[i]) m_aStldEp1.Add(m_aStldK[i]);
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP1DelBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if (nAllSize <= 0) return;
	CArray<int, int> aIndex; aIndex.RemoveAll();
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp1.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp1.GetItemData(aIndex[i])] = FALSE;
		m_lstEp1.DeleteString(aIndex[i]);
	}
	
	m_lstEp.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (!m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			m_lstEp.SetItemData(m_lstEp.AddString(rData.LoadCaseName), i);
		}
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP2AddBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if(nAllSize <= 0) return;
	
	CArray<int, int> aIndex; aIndex.RemoveAll();
	CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
	aSelTmpFlag.Copy(m_aSelFlag);
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp.GetItemData(aIndex[i])] = TRUE;
		m_lstEp.DeleteString(aIndex[i]);
	}
	
	for (int i=0; i<nAllSize; i++)
	{
		for(int j=0; j<m_aStldEp2.GetSize(); j++)
		{
			if(m_aStldK[i]==m_aStldEp2[j]) aSelTmpFlag[i] = FALSE;
		}
	}
	
	m_lstEp2.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
			m_lstEp2.SetItemData(m_lstEp2.AddString(rData.LoadCaseName), i);
		}
	}
	
	m_aStldEp2.RemoveAll();
	for (int i=0; i<nAllSize; i++)
	{
		if(!aSelTmpFlag[i]) m_aStldEp2.Add(m_aStldK[i]);
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP2DelBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if (nAllSize <= 0) return;
	CArray<int, int> aIndex; aIndex.RemoveAll();
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp2.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp2.GetItemData(aIndex[i])] = FALSE;
		m_lstEp2.DeleteString(aIndex[i]);
	}
	
	m_lstEp.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (!m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			m_lstEp.SetItemData(m_lstEp.AddString(rData.LoadCaseName), i);
		}
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP3AddBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if(nAllSize <= 0) return;
	
	CArray<int, int> aIndex; aIndex.RemoveAll();
	CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
	aSelTmpFlag.Copy(m_aSelFlag);
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp.GetItemData(aIndex[i])] = TRUE;
		m_lstEp.DeleteString(aIndex[i]);
	}
	
	for (int i=0; i<nAllSize; i++)
	{
		for(int j=0; j<m_aStldEp3.GetSize(); j++)
		{
			if(m_aStldK[i]==m_aStldEp3[j]) aSelTmpFlag[i] = FALSE;
		}
	}
	
	m_lstEp3.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
			m_lstEp3.SetItemData(m_lstEp3.AddString(rData.LoadCaseName), i);
		}
	}
	
	m_aStldEp3.RemoveAll();
	for (int i=0; i<nAllSize; i++)
	{
		if(!aSelTmpFlag[i]) m_aStldEp3.Add(m_aStldK[i]);
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP3DelBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if (nAllSize <= 0) return;
	CArray<int, int> aIndex; aIndex.RemoveAll();
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp3.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp3.GetItemData(aIndex[i])] = FALSE;
		m_lstEp3.DeleteString(aIndex[i]);
	}
	
	m_lstEp.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (!m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			m_lstEp.SetItemData(m_lstEp.AddString(rData.LoadCaseName), i);
		}
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP4AddBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if(nAllSize <= 0) return;
	
	CArray<int, int> aIndex; aIndex.RemoveAll();
	CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
	aSelTmpFlag.Copy(m_aSelFlag);
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp.GetItemData(aIndex[i])] = TRUE;
		m_lstEp.DeleteString(aIndex[i]);
	}
	
	for (int i=0; i<nAllSize; i++)
	{
		for(int j=0; j<m_aStldEp4.GetSize(); j++)
		{
			if(m_aStldK[i]==m_aStldEp4[j]) aSelTmpFlag[i] = FALSE;
		}
	}
	
	m_lstEp4.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
			m_lstEp4.SetItemData(m_lstEp4.AddString(rData.LoadCaseName), i);
		}
	}
	
	m_aStldEp4.RemoveAll();
	for (int i=0; i<nAllSize; i++)
	{
		if(!aSelTmpFlag[i]) m_aStldEp4.Add(m_aStldK[i]);
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP4DelBtn() 
{
	int nAllSize = m_aStldK.GetSize();
	if (nAllSize <= 0) return;
	CArray<int, int> aIndex; aIndex.RemoveAll();
	aIndex.SetSize(nAllSize);
	int nCount = m_lstEp4.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstEp4.GetItemData(aIndex[i])] = FALSE;
		m_lstEp4.DeleteString(aIndex[i]);
	}
	
	m_lstEp.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (!m_aSelFlag[i]) 
		{
			T_STLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetStld(m_aStldK[i], rData)) continue;
			m_lstEp.SetItemData(m_lstEp.AddString(rData.LoadCaseName), i);
		}
	}
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP1Dblclk() 
{
	OnCmdCSAEP1DelBtn();	
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP2Dblclk() 
{
	OnCmdCSAEP2DelBtn();	
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP3Dblclk() 
{
	OnCmdCSAEP3DelBtn();
}

void CCmdAutoLoadCombCvlCSAEPDlg::OnCmdCSAEP4Dblclk() 
{
	OnCmdCSAEP4DelBtn();
}

