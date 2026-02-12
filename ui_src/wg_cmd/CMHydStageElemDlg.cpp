// CMHydStageElemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydStageElemDlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CMStrtGrupDefDlg.h"
#include "ExportFunc.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageElemDlg dialog
#define CDialog CCMHydStageItemBase


CCMHydStageElemDlg::CCMHydStageElemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMHydStageElemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydStageElemDlg)
	//}}AFX_DATA_INIT
}


void CCMHydStageElemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydStageElemDlg)
	DDX_Control(pDX, IDC_CMD_GROUP_LIST, m_wndGroupList);
	DDX_Control(pDX, IDC_CMD_ACTIVE_GROUP_LIST, m_ActiveGroupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMHydStageElemDlg, CDialog)
	//{{AFX_MSG_MAP(CCMHydStageElemDlg)
	ON_BN_CLICKED(IDC_CMD_DEFGROUP_BUTTON, OnCmdDefgroupButton)
	ON_BN_CLICKED(IDC_ACT_ADD_BUTTON, OnActAddButton)
	ON_BN_CLICKED(IDC_ACT_DELETE_BUTTON, OnActDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageElemDlg message handlers
void CCMHydStageElemDlg::SetListCtrlHeader()
{
	CStringArray HTitles;
	CArray<float, float> HRatio;

	HTitles.Add(_LS(IDS_CMD0417__Name)); HRatio.Add(1.0f);
	CDlgUtil::_SetListCtrlHeader(&m_ActiveGroupList,HTitles,&HRatio,NULL); 
}

BOOL CCMHydStageElemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetListCtrlHeader();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMHydStageElemDlg::SetCurStageData(T_HSTG_D* pStageD)
{
	
	this->m_pCurStageD = pStageD;
	
	// Dialog 가 Window로 존재한다는 가정...
	CArray<T_GRUP_K,T_GRUP_K> rGroupKeys;
	CArray<T_GRUP_K,T_GRUP_K> ActiveGrupKeys;
	
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> ActElemData;
 
	m_ActiveGroupList  .DeleteAllItems();
	
	int nGr;
	
	
	T_GRUP_D GroupD;

	nGr = pStageD->aActiveElem.GetSize();
	CStringArray strContents;
	CString TempStr;
	int Index = 0;
	for(int   i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(pStageD->aActiveElem[i],GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);
			CDlgUtil::SetListItem(&m_ActiveGroupList,Index,strContents,pStageD->aActiveElem[i]); // ItemData 동시 설정..
			Index++;
			//ActElemData.SetAt(pStageD->aActiveElem[i].GrupKey,pStageD->aActiveElem[i].nAge);
			ActElemData.SetAt(pStageD->aActiveElem[i],pStageD->aActiveElem[i]);
		}
	}

	if(nGr != ActElemData.GetCount())
	{
		// StageData를 Update한다. 
		T_GRUP_K GrupK;
		T_GRUP_K TGrupK;
		pStageD->aActiveElem.RemoveAll();
		POSITION Pos = ActElemData.GetStartPosition();
		while(Pos)
		{
			ActElemData.GetNextAssoc(Pos,GrupK,TGrupK);
			pStageD->aActiveElem.Add(GrupK);
		}
	}
	UpdateGroupListByCurState();
}

void CCMHydStageElemDlg::UpdateGroupListByCurState()
{
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> m_ActGrups;
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> m_DeActGrups;
	m_ActGrups.InitHashTable(256);
		
	T_GRUP_K DummyK = 0;

	int nAct = m_ActiveGroupList.GetItemCount();
	for( int i = 0; i < nAct ; i++)
	{
		m_ActGrups.SetAt((T_GRUP_K)m_ActiveGroupList.GetItemData(i),DummyK);
	}
	
	// Dialog 가 Window로 존재한다는 가정...
	CArray<T_GRUP_K,T_GRUP_K> rGroupKeys;
	m_wndGroupList     .ResetContent();
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKeyList(rGroupKeys);

	int nGr = rGroupKeys.GetSize();
	T_GRUP_D GroupD;
	T_GRUP_K GroupK;
	int Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(!m_ActGrups.Lookup(rGroupKeys[i],GroupK))
		{
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(rGroupKeys[i],GroupD))
			{
				m_wndGroupList.AddString(GroupD.GroupName);
				m_wndGroupList.SetItemData(Index, rGroupKeys[i]);
				Index++;
			}
		}
	}
}

void CCMHydStageElemDlg::OnCmdDefgroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMStrtGrupDefDlg::IDD,this);
}

void CCMHydStageElemDlg::OnActAddButton() 
{
	
	CArray<T_GRUP_K,T_GRUP_K> arActGKeys ;
	 
	T_GRUP_K TempK;
	
	int nGN = m_ActiveGroupList.GetItemCount();

	for( int   i = 0 ;i < nGN ; i++)
	{
		TempK = (T_GRUP_K) m_ActiveGroupList.GetItemData(i);
		arActGKeys.Add(TempK);
	}

	CArray<T_GRUP_K, T_GRUP_K> SelGrupKeys;
	CArray<int, int> arSelIndex;

	int nSel = this->m_wndGroupList.GetSelCount();
	arSelIndex.SetSize(nSel);
	m_wndGroupList.GetSelItems(nSel,arSelIndex.GetData());

	for(int  i = 0 ; i < nSel ; i++)
	{
		TempK = (T_GRUP_K) m_wndGroupList.GetItemData(arSelIndex[i]);
		SelGrupKeys.Add(TempK);
	}
 

	CArray<T_GRUP_K,T_GRUP_K> rResultKeys1;
	
	I_GENModelBase::GetCurMySelfST()->SubtractKey(SelGrupKeys ,arActGKeys ,rResultKeys1); // RList = Sel List - Active List 
	
	T_GRUP_D GroupD;
	int nGr;
	CStringArray strContents;
		
	nGr = rResultKeys1.GetSize();
	int nAct = m_ActiveGroupList.GetItemCount();
	for(int i = 0 ; i < nGr ; i++)
	{
		strContents.RemoveAll();
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(rResultKeys1[i],GroupD))
		{
			strContents.Add(GroupD.GroupName);
			CDlgUtil::SetListItem(&m_ActiveGroupList,i+nAct,strContents,rResultKeys1[i]); 
			m_pCurStageD->aActiveElem.Add(rResultKeys1[i]);
		}
	}
	UpdateGroupListByCurState();
}

void CCMHydStageElemDlg::OnActDeleteButton() 
{
 
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_GRUP_K,T_GRUP_K> arSelGrupKey;
	CArray<T_GRUP_K,T_GRUP_K> arAllGrupKey;
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> arGrupMap;
	
	CDlgUtil::GetSelectedListItemData(&m_ActiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aActiveElem.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aActiveElem[i]);
		arGrupMap.SetAt(m_pCurStageD->aActiveElem[i],m_pCurStageD->aActiveElem[i]);
	}

	int nSel = arSelItemData.GetSize();

	for(int  i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_GRUP_K)arSelItemData[i]);
	}
	
	CArray<T_GRUP_K,T_GRUP_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);

	m_pCurStageD->aActiveElem.RemoveAll();
	
	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_GRUP_D GrupD;
	T_GRUP_K GrupK,TGrupK;

	m_ActiveGroupList.DeleteAllItems();

	for(int  i = 0 ; i < nRes ; i++)
	{
		GrupK = aResultKey[i];
		if(arGrupMap.Lookup(GrupK,TGrupK))
		{
			
			m_pCurStageD->aActiveElem.Add(GrupK);
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(GrupK,GrupD))
			{
				Contents.RemoveAll();
				Contents.Add(GrupD.GroupName);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,GrupK);
			}
		}
	}

	UpdateGroupListByCurState();
}
