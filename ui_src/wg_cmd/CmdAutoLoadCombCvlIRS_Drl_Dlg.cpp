// CmdAutoLoadCombCvlIRS_Drl_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlIRS_Drl_Dlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRS_DRL_Dlg dialog
CCmdAutoLoadCombCvlIRS_DRL_Dlg::CCmdAutoLoadCombCvlIRS_DRL_Dlg(CWnd* pParent /*=NULL*/)
  : CDialogMove(CCmdAutoLoadCombCvlIRS_DRL_Dlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlIRS_DRL_Dlg)	
  //}}AFX_DATA_INIT
  m_pDoc = CDBDoc::GetDocPoint();
  m_aMvldChar.RemoveAll();
  m_aMvldFreq.RemoveAll();
}

CCmdAutoLoadCombCvlIRS_DRL_Dlg::~CCmdAutoLoadCombCvlIRS_DRL_Dlg()
{
}

void CCmdAutoLoadCombCvlIRS_DRL_Dlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogMove::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCMSectioanagerMainDlg)
  DDX_Control(pDX, IDC_CMD_LCOMB_EURO_ROAD_CHRA_LST,   m_lstChra);
  DDX_Control(pDX, IDC_CMD_LCOMB_EURO_ROAD_FREQ_LST,   m_lstFreq);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlIRS_DRL_Dlg, CDialogMove)
  //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlIRS_DRL_Dlg)
  ON_BN_CLICKED(IDC_CMD_LCOMB_EURO_ROAD_ADD_BTN,      OnCmdEuroRoadAddBtn)
  ON_BN_CLICKED(IDC_CMD_LCOMB_EURO_ROAD_DEL_BTN,      OnCmdEuroRoadDelBtn)
  ON_LBN_DBLCLK(IDC_CMD_LCOMB_EURO_ROAD_FREQ_LST,     OnCmdEuroRoadDblclk)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRS_DRL_Dlg message handlers

BOOL CCmdAutoLoadCombCvlIRS_DRL_Dlg::OnInitDialog() 
{
  CDialogMove::OnInitDialog();

  CArray<T_STLD_K, T_STLD_K> arKeyList;
  m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
  int nAllSize = arKeyList.GetSize();

  int nFreqSize = m_aMvldFreq.GetSize();
  int nCharSize = m_aMvldChar.GetSize();
  if(nCharSize+nFreqSize==0)
  {
    // 초기 : 전체 arKeyList의 값 적용
    for(int i=0; i<nAllSize; i++)
    {
      T_STLD_D rData; rData.Initialize();
      if(!m_pDoc->m_pAttrCtrl->GetStld(arKeyList[i], rData)) { ASSERT(0); return FALSE; }
       if(rData.LoadCaseType==_T("DRL")) 
       {
         m_aMvldChar.Add(arKeyList[i]);
         m_aSelFlag.Add(FALSE);
       }
      
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
      for(int i=0; i<nAllSize; i++)
      {
        if(m_aMvldFreq[j]==arKeyList[i]) m_aSelFlag[i] = TRUE;
      }
    }
  }

  
  

  m_lstChra.ResetContent();
  for (int i=0; i<m_aMvldChar.GetSize(); i++)
  {
    T_STLD_D rData; rData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetStld(m_aMvldChar[i], rData)) continue;
    m_lstChra.SetItemData(m_lstChra.AddString(rData.LoadCaseName),m_pDoc->m_pAttrCtrl->GetStldKey(rData.LoadCaseName));
  }
  //if(nCharSize <= 0) return FALSE;
  m_lstFreq.ResetContent();

  for (int i=0; i<m_aMvldFreq.GetSize(); i++)
  {
    T_STLD_D rData; rData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetStld(m_aMvldFreq[i], rData)) continue;
    m_lstFreq.SetItemData(m_lstFreq.AddString(rData.LoadCaseName), m_pDoc->m_pAttrCtrl->GetStldKey(rData.LoadCaseName) );
  }


  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdAutoLoadCombCvlIRS_DRL_Dlg::OnOK() 
{

  CArray<T_STLD_K, T_STLD_K> arKeyList;
  m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);

  m_aMvldChar.RemoveAll();
  int nChrSize = m_lstChra.GetCount();
  for(int j=0; j<nChrSize; j++)
  {
    T_STLD_D rData; rData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetStld(m_lstChra.GetItemData(j), rData)) continue;
    m_aMvldChar.Add(m_pDoc->m_pAttrCtrl->GetStldKey(rData.LoadCaseName));
  }


  m_aMvldFreq.RemoveAll();
  int nSize = m_lstFreq.GetCount();
  for(int i=0; i<nSize; i++)
  {
    T_STLD_D rData; rData.Initialize();
    if(!m_pDoc->m_pAttrCtrl->GetStld(m_lstFreq.GetItemData(i), rData)) continue;
    m_aMvldFreq.Add(m_pDoc->m_pAttrCtrl->GetStldKey(rData.LoadCaseName));
  }

  CDialogMove::OnOK();
}

void CCmdAutoLoadCombCvlIRS_DRL_Dlg::OnCmdEuroRoadAddBtn() 
{
  int nAllSize = m_aMvldChar.GetSize();
  if(nAllSize <= 0) return;

  CArray<T_STLD_K, T_STLD_K> arKeyList;
  m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);

  CArray<int, int> aIndex; aIndex.RemoveAll();
  CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
  aSelTmpFlag.Copy(m_aSelFlag);
  aIndex.SetSize(nAllSize);
  int nCount = m_lstChra.GetSelItems(nAllSize, aIndex.GetData());
  if (nCount == 0) return;
  for (int i=nCount-1; i>=0; i--)
  {
    
    m_aSelFlag[aIndex[i]] = TRUE;
     //m_aSelFlag.Add([arKeyList(m_lstChra.GetItemData(i))],i) = TRUE;
    m_lstChra.DeleteString(aIndex[i]);
  }

  for (int i=0; i<nAllSize; i++)
  {
    for(int j=0; j<m_aMvldFreq.GetSize(); j++)
    {
      if(m_aMvldChar[i]==m_aMvldFreq[j]) aSelTmpFlag[i] = FALSE;
    }
  }

  if (m_aMvldFreq.GetSize() == 0) m_lstFreq.ResetContent();
  
  for (int i=0; i<nAllSize; i++)
  {
    if (m_aSelFlag[i]) 
    {
      T_STLD_D rData; rData.Initialize();
      if(!m_pDoc->m_pAttrCtrl->GetStld(m_aMvldChar[i], rData)) continue;
      if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
      m_lstFreq.SetItemData(m_lstFreq.AddString(rData.LoadCaseName),m_pDoc->m_pAttrCtrl->GetStldKey(rData.LoadCaseName));
    }
  }

  if (m_aMvldFreq.GetSize() == 0)  m_aMvldFreq.RemoveAll();
  for (int i=0; i<nAllSize; i++)
  {
    if(!aSelTmpFlag[i]) m_aMvldFreq.Add(m_aMvldChar[i]);
  }
}

void CCmdAutoLoadCombCvlIRS_DRL_Dlg::OnCmdEuroRoadDelBtn() 
{
//   int nAllSize = m_aMvldChar.GetSize();
//   if (nAllSize <= 0) return;
//   CArray<int, int> aIndex; aIndex.RemoveAll();
//   CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
//   aIndex.SetSize(nAllSize);
//   int nCount = m_lstFreq.GetSelItems(nAllSize, aIndex.GetData());
//   if (nCount == 0) return;
//   for (int i=nCount-1; i>=0; i--)
//   {
//     //m_aSelFlag[m_lstFreq.GetItemData(aIndex[i])] = FALSE;
//     m_aSelFlag[aIndex[i]] = TRUE;
//     m_lstFreq.DeleteString(aIndex[i]);
//   }
// 
// //   for (i=0; i<nAllSize; i++)
// //   {
// //     for(int j=0; j<m_aMvldChar.GetSize(); j++)
// //     {
// //       if(m_aMvldChar[j]==m_aMvldFreq[i]) aSelTmpFlag[i] = FALSE;
// //     }
// //   }
// 
//   m_lstChra.ResetContent();
//   for (i=0; i<nAllSize; i++)
//   {
//     if (m_aSelFlag[i]) 
//     {
//       T_STLD_D rData; rData.Initialize();
//       if(!m_pDoc->m_pAttrCtrl->GetStld(m_aMvldChar[i], rData)) continue;
//       /*if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;*/
//       m_lstChra.SetItemData(m_lstChra.AddString(rData.LoadCaseName), m_pDoc->m_pAttrCtrl->GetStldKey(rData.LoadCaseName));
//     }
//   }

  int nAllSize = m_aMvldFreq.GetSize();
  if (nAllSize <= 0) return;
  CArray<int, int> aIndex; aIndex.RemoveAll();
  aIndex.SetSize(nAllSize);
  CArray<BOOL, BOOL> aSelTmpFlag; aSelTmpFlag.RemoveAll();
  aSelTmpFlag.Copy(m_aSelFlag);
  int nCount = m_lstFreq.GetSelItems(nAllSize, aIndex.GetData());
  if (nCount == 0) return;
  for (int i=nCount-1; i>=0; i--)
  {
    m_aSelFlag[aIndex[i]] = TRUE;
    m_lstFreq.DeleteString(aIndex[i]);
  }

  for (int i=0; i<nAllSize; i++)
  {
    for(int j=0; j<m_aMvldChar.GetSize(); j++)
    {
      if(m_aMvldChar[j]==m_aMvldFreq[i]) aSelTmpFlag[j] = FALSE;
    }
  }

  if (m_aMvldChar.GetSize() == 0) m_lstChra.ResetContent();
  for (int i=0; i<nAllSize; i++)
  {
    if (m_aSelFlag[i]) 
    {
      T_STLD_D rData; rData.Initialize();
      if(!m_pDoc->m_pAttrCtrl->GetStld(m_aMvldFreq[i], rData)) continue;
      if(m_aSelFlag[i]==aSelTmpFlag[i]) continue;
      m_lstChra.SetItemData(m_lstChra.AddString(rData.LoadCaseName),m_pDoc->m_pAttrCtrl->GetStldKey(rData.LoadCaseName));
    }
  }
  if (m_aMvldChar.GetSize() == 0) m_aMvldChar.RemoveAll();
  for (int i=0; i<nAllSize; i++)
  {
    if(aSelTmpFlag[i]) m_aMvldChar.Add(m_aMvldFreq[i]);
  }
}

void CCmdAutoLoadCombCvlIRS_DRL_Dlg::OnCmdEuroRoadDblclk() 
{
  OnCmdEuroRoadDelBtn();	
}

