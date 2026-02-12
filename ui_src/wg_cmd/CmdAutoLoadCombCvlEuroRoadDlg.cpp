// CmdAutoLoadCombCvlEuroRoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlEuroRoadDlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlEuroRoadDlg dialog
CCmdAutoLoadCombCvlEuroRoadDlg::CCmdAutoLoadCombCvlEuroRoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadCombCvlEuroRoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlEuroRoadDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_aMvldChar.RemoveAll();
	m_aMvldFreq.RemoveAll();
}

CCmdAutoLoadCombCvlEuroRoadDlg::~CCmdAutoLoadCombCvlEuroRoadDlg()
{
}

void CCmdAutoLoadCombCvlEuroRoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectioanagerMainDlg)
	DDX_Control(pDX, IDC_CMD_LCOMB_EURO_ROAD_CHRA_LST,   m_lstChra);
	DDX_Control(pDX, IDC_CMD_LCOMB_EURO_ROAD_FREQ_LST,   m_lstFreq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlEuroRoadDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlEuroRoadDlg)
	ON_BN_CLICKED(IDC_CMD_LCOMB_EURO_ROAD_ADD_BTN,      OnCmdEuroRoadAddBtn)
	ON_BN_CLICKED(IDC_CMD_LCOMB_EURO_ROAD_DEL_BTN,      OnCmdEuroRoadDelBtn)
	ON_LBN_DBLCLK(IDC_CMD_LCOMB_EURO_ROAD_FREQ_LST,     OnCmdEuroRoadDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlEuroRoadDlg message handlers

BOOL CCmdAutoLoadCombCvlEuroRoadDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CArray<T_MVLD_K, T_MVLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetMvldKeyList(arKeyList);
	int nAllSize = arKeyList.GetSize();

	int nFreqSize = m_aMvldFreq.GetSize();
	if(nFreqSize==0)
	{
		// 초기 : 전체 arKeyList의 값 적용
		for(int i=0; i<nAllSize; i++)
		{
			T_MVLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMvld(arKeyList[i], rData)) { ASSERT(0); return FALSE; }
			
			m_aMvldChar.Add(arKeyList[i]);
			m_aSelFlag.Add(FALSE);
		}
	}
	else
	{
		// 저장된 m_aMvldChar, m_aMvldFreq 적용
		for(int i=0; i<nAllSize; i++)
		{
			m_aSelFlag.Add(FALSE);
		}
			
		for(int j=0; j<m_aMvldFreq.GetSize(); j++)
		{
			for(int  i=0; i<nAllSize; i++)
			{
				if(m_aMvldFreq[j]==arKeyList[i]) m_aSelFlag[i] = TRUE;
			}
		}
	}

	int nCharSize = m_aMvldChar.GetSize();
	if(nCharSize <= 0) return FALSE;
	
	m_lstChra.ResetContent();
	for (int i=0; i<nCharSize; i++)
	{
		T_MVLD_D rData; rData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMvld(m_aMvldChar[i], rData)) continue;
		m_lstChra.SetItemData(m_lstChra.AddString(rData.LoadCaseName), i);
	}

	m_lstFreq.ResetContent();
	
	for (int i=0; i<nFreqSize; i++)
	{
		T_MVLD_D rData; rData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMvld(m_aMvldFreq[i], rData)) continue;
		m_lstFreq.SetItemData(m_lstFreq.AddString(rData.LoadCaseName), i);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdAutoLoadCombCvlEuroRoadDlg::OnOK() 
{

	CArray<T_MVLD_K, T_MVLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetMvldKeyList(arKeyList);

	m_aMvldChar.RemoveAll();
	int nChrSize = m_lstChra.GetCount();
	for(int j=0; j<nChrSize; j++)
	{
		m_aMvldChar.Add(arKeyList[m_lstChra.GetItemData(j)]);
	}


	m_aMvldFreq.RemoveAll();
	int nSize = m_lstFreq.GetCount();
	for(int i=0; i<nSize; i++)
	{
		m_aMvldFreq.Add(arKeyList[m_lstFreq.GetItemData(i)]);
	}

	CDialogMove::OnOK();
}

void CCmdAutoLoadCombCvlEuroRoadDlg::OnCmdEuroRoadAddBtn() 
{
	int nAllSize = m_aMvldChar.GetSize();
	if(nAllSize <= 0) return;

	CArray<int, int> aIndex; aIndex.RemoveAll();
	CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
	aSelTmpFlag.Copy(m_aSelFlag);
	aIndex.SetSize(nAllSize);
	int nCount = m_lstChra.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstChra.GetItemData(aIndex[i])] = TRUE;
		m_lstChra.DeleteString(aIndex[i]);
	}
	
	for (int i=0; i<nAllSize; i++)
	{
		for(int j=0; j<m_aMvldFreq.GetSize(); j++)
		{
			if(m_aMvldChar[i]==m_aMvldFreq[j]) aSelTmpFlag[i] = FALSE;
		}
	}
	
	m_lstFreq.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
			T_MVLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMvld(m_aMvldChar[i], rData)) continue;
			if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
			m_lstFreq.SetItemData(m_lstFreq.AddString(rData.LoadCaseName), i);
		}
	}
	
	m_aMvldFreq.RemoveAll();
	for (int i=0; i<nAllSize; i++)
	{
		if(!aSelTmpFlag[i]) m_aMvldFreq.Add(m_aMvldChar[i]);
	}
}

void CCmdAutoLoadCombCvlEuroRoadDlg::OnCmdEuroRoadDelBtn() 
{
	int nAllSize = m_aMvldChar.GetSize();
	if (nAllSize <= 0) return;
	CArray<int, int> aIndex; aIndex.RemoveAll();
	aIndex.SetSize(nAllSize);
	int nCount = m_lstFreq.GetSelItems(nAllSize, aIndex.GetData());
	if (nCount == 0) return;
	for (int i=nCount-1; i>=0; i--)
	{
		m_aSelFlag[m_lstFreq.GetItemData(aIndex[i])] = FALSE;
		m_lstFreq.DeleteString(aIndex[i]);
	}
	
	m_lstChra.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (!m_aSelFlag[i]) 
		{
			T_MVLD_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMvld(m_aMvldChar[i], rData)) continue;
			m_lstChra.SetItemData(m_lstChra.AddString(rData.LoadCaseName), i);
		}
	}
}

void CCmdAutoLoadCombCvlEuroRoadDlg::OnCmdEuroRoadDblclk() 
{
	OnCmdEuroRoadDelBtn();	
}
