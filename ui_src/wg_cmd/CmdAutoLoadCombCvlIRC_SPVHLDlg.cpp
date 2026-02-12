// CCmdAutoLoadCombCvlIRC_SPLVEHDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlIRC_SPVHLDlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRC_SPLVEHDlg dialog
CCmdAutoLoadCombCvlIRC_SPLVEHDlg::CCmdAutoLoadCombCvlIRC_SPLVEHDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadCombCvlIRC_SPLVEHDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlIRC_SPLVEHDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_aMvldSPV.RemoveAll();
	m_aMvldSPVSel.RemoveAll();
}

CCmdAutoLoadCombCvlIRC_SPLVEHDlg::~CCmdAutoLoadCombCvlIRC_SPLVEHDlg()
{
}

void CCmdAutoLoadCombCvlIRC_SPLVEHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectioanagerMainDlg)
	DDX_Control(pDX, IDC_CMD_LCOMB_EURO_ROAD_CHRA_LST,   m_lstChra);
	DDX_Control(pDX, IDC_CMD_LCOMB_EURO_ROAD_FREQ_LST,   m_lstFreq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlIRC_SPLVEHDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlIRC_SPLVEHDlg)
	ON_BN_CLICKED(IDC_CMD_LCOMB_EURO_ROAD_ADD_BTN,      OnCmdEuroRoadAddBtn)
	ON_BN_CLICKED(IDC_CMD_LCOMB_EURO_ROAD_DEL_BTN,      OnCmdEuroRoadDelBtn)
	ON_LBN_DBLCLK(IDC_CMD_LCOMB_EURO_ROAD_FREQ_LST,     OnCmdEuroRoadDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRC_SPLVEHDlg message handlers

BOOL CCmdAutoLoadCombCvlIRC_SPLVEHDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CArray<T_MVLDid_K, T_MVLDid_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arKeyList);
	int nAllSize = arKeyList.GetSize();

	int nFreqSize = m_aMvldSPVSel.GetSize();
	if(nFreqSize==0)
	{
		// 초기 : 전체 arKeyList의 값 적용
		for(int i=0; i<nAllSize; i++)
		{
			T_MVLDid_D rData; rData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMvldid(arKeyList.GetAt(i), rData)) { ASSERT(0); return FALSE; }
			
			m_aMvldSPV.Add(arKeyList.GetAt(i));
			m_aSelFlag.Add(FALSE);
		}
	}
	else
	{
		// 저장된 m_aMvldSPV, m_aMvldSPVSel 적용
		for(int i=0; i<nAllSize; i++)
		{
			m_aSelFlag.Add(FALSE);
		}
			
		for(int j=0; j<m_aMvldSPVSel.GetSize(); j++)
		{
			for(int  i=0; i<nAllSize; i++)
			{
				if(m_aMvldSPVSel[j]==arKeyList.GetAt(i)) m_aSelFlag[i] = TRUE;
			}
		}
	}

	int nCharSize = m_aMvldSPV.GetSize();
	if(nCharSize <= 0) return FALSE;
	
	m_lstChra.ResetContent();
	for (int i=0; i<nCharSize; i++)
	{
		T_MVLDid_D rData; rData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMvldid(m_aMvldSPV[i], rData)) continue;
		m_lstChra.SetItemData(m_lstChra.AddString(rData.LoadCaseName), i);
	}

	m_lstFreq.ResetContent();
	
	for (int i=0; i<nFreqSize; i++)
	{
        T_MVLDid_D rData; rData.Initialize();
        if(!m_pDoc->m_pAttrCtrl->GetMvldid(m_aMvldSPVSel[i], rData)) continue;
		m_lstFreq.SetItemData(m_lstFreq.AddString(rData.LoadCaseName), i);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdAutoLoadCombCvlIRC_SPLVEHDlg::OnOK() 
{

    CArray<T_MVLDid_K, T_MVLDid_K> arKeyList;
    m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arKeyList);

	m_aMvldSPV.RemoveAll();
	int nChrSize = m_lstChra.GetCount();
    int nSize = m_lstFreq.GetCount();
    if (abs(nChrSize-nSize)!=arKeyList.GetSize())
    {
        for(int j=0; j<nChrSize; j++)
        {
            int idta = -1;//m_lstChra.GetItemData(j);
            if (idta<0)
            {
                CString str;str.Format(_T(""));
                m_lstChra.GetText(j,str);
                for (int m=0;m<arKeyList.GetSize();m++)
                {
                    T_MVLDid_D rData; rData.Initialize();
                    if(m_pDoc->m_pAttrCtrl->GetMvldid(arKeyList[m], rData) && rData.LoadCaseName.Find(str)==0)
                    {
                        idta = m;
                    }
                }
            }


            m_aMvldSPV.Add(arKeyList[idta]);
        }


        m_aMvldSPVSel.RemoveAll();

        for(int i=0; i<nSize; i++)
        {
            int idta = -1;//m_lstFreq.GetItemData(j);
            if (idta<0)
            {
                CString str;str.Format(_T(""));
                m_lstFreq.GetText(i,str);
                for (int j=0;j<arKeyList.GetSize();j++)
                {
                    T_MVLDid_D rData; rData.Initialize();
                    if(m_pDoc->m_pAttrCtrl->GetMvldid(arKeyList[j], rData) && rData.LoadCaseName.Find(str)==0)
                    {
                        idta = j;
                    }
                }
            }
            m_aMvldSPVSel.Add(arKeyList[idta]);
        }

    }
	
	CDialogMove::OnOK();
}

void CCmdAutoLoadCombCvlIRC_SPLVEHDlg::OnCmdEuroRoadAddBtn() 
{
	int nAllSize = m_aMvldSPV.GetSize();
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
		for(int j=0; j<m_aMvldSPVSel.GetSize(); j++)
		{
			if(m_aMvldSPV[i]==m_aMvldSPVSel[j]) aSelTmpFlag[i] = FALSE;
		}
	}
	
	m_lstFreq.ResetContent();
	for (int i=0; i<nAllSize; i++)
	{
		if (m_aSelFlag[i]) 
		{
            T_MVLDid_D rData; rData.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvldid(m_aMvldSPV[i], rData)) continue;
			if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
			m_lstFreq.SetItemData(m_lstFreq.AddString(rData.LoadCaseName), i);
		}
	}
	
	m_aMvldSPVSel.RemoveAll();
	for (int i=0; i<nAllSize; i++)
	{
		if(!aSelTmpFlag[i]) m_aMvldSPVSel.Add(m_aMvldSPV[i]);
	}
}

void CCmdAutoLoadCombCvlIRC_SPLVEHDlg::OnCmdEuroRoadDelBtn() 
{
	int nAllSize = m_aMvldSPV.GetSize();
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
            T_MVLDid_D rData; rData.Initialize();
            if(!m_pDoc->m_pAttrCtrl->GetMvldid(m_aMvldSPVSel[i], rData)) continue;
			m_lstChra.SetItemData(m_lstChra.AddString(rData.LoadCaseName), i);
		}
	}
}

void CCmdAutoLoadCombCvlIRC_SPLVEHDlg::OnCmdEuroRoadDblclk() 
{
	OnCmdEuroRoadDelBtn();	
}
