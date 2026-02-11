// CMHydStageBndrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydStageBndrDlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "CMBndrGrupDefDlg.h" // Modeless Or Modal Dialog
#include "ExportFunc.h"
	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageBndrDlg dialog
#define CDialog CCMHydStageItemBase


CCMHydStageBndrDlg::CCMHydStageBndrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMHydStageBndrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydStageBndrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMHydStageBndrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydStageBndrDlg)
	DDX_Control(pDX, IDC_CMD_GROUP_LIST, m_wndGroupList);
	DDX_Control(pDX, IDC_CMD_DEACTIVE_GROUP_LIST, m_DeactiveGroupList);
	DDX_Control(pDX, IDC_CMD_ACTIVE_GROUP_LIST, m_ActiveGroupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMHydStageBndrDlg, CDialog)
	//{{AFX_MSG_MAP(CCMHydStageBndrDlg)
	ON_BN_CLICKED(IDC_CMD_DEFGROUP_BUTTON, OnCmdDefgroupButton)
	ON_BN_CLICKED(IDC_ACT_ADD_BUTTON, OnActAddButton)
	ON_BN_CLICKED(IDC_DEACT_ADD, OnDeactAdd)
	ON_BN_CLICKED(IDC_ACT_DELETE_BUTTON, OnActDeleteButton)
	ON_BN_CLICKED(IDC_DEACT_DELETE_BUTTON, OnDeactDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageBndrDlg message handlers
void CCMHydStageBndrDlg::SetListCtrlHeader()
{
	CStringArray Htitles;
	CArray<float,float> HRatios;
	Htitles.Add(_LS(IDS_CMD0417__Name));
	HRatios.Add(1.0f);
	CDlgUtil::_SetListCtrlHeader(&m_ActiveGroupList,Htitles,&HRatios,NULL);
	Htitles.RemoveAll();
	HRatios.RemoveAll();
	Htitles.Add(_LS(IDS_CMD0417__Name));
	HRatios.Add(1.f);
	CDlgUtil::_SetListCtrlHeader(&m_DeactiveGroupList,Htitles,&HRatios,NULL);
}


BOOL CCMHydStageBndrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetListCtrlHeader();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMHydStageBndrDlg::OnCmdDefgroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD,this);
}

void CCMHydStageBndrDlg::SetCurStageData(T_HSTG_D *pStageD)
{
	m_pCurStageD = pStageD;

	m_ActiveGroupList  .DeleteAllItems();
	m_DeactiveGroupList.DeleteAllItems();

	CMap<T_BNGR_K, T_BNGR_K,T_BNGR_K,T_BNGR_K> ActBndrDatas  ;
	CArray<T_BNGR_K, T_BNGR_K> DeActBndrDatas;
	
	T_BNGR_D GroupD;
	T_BNGR_K GroupK,TGroupK;
	

	int nGr = pStageD->aActiveBndr.GetSize();
	CStringArray strContents;
	CString      StrTemp;
	int Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(pStageD->aActiveBndr[i],GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);
			CDlgUtil::SetListItem(&m_ActiveGroupList,Index,strContents,pStageD->aActiveBndr[i]); // ItemData 동시 설정..
			Index++;
			ActBndrDatas.SetAt(pStageD->aActiveBndr[i],pStageD->aActiveBndr[i]);
		}
	}

	
	if(nGr != ActBndrDatas.GetCount())
	{
		POSITION Pos = ActBndrDatas.GetStartPosition();
		pStageD->aActiveBndr.RemoveAll();
		while(Pos)
		{
			ActBndrDatas.GetNextAssoc(Pos,GroupK,TGroupK);
			pStageD->aActiveBndr.Add(GroupK);
		}
	}
	
	nGr = pStageD->aDeactiveBndr.GetSize();
	Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(pStageD->aDeactiveBndr[i],GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);
			CDlgUtil::SetListItem(&m_DeactiveGroupList,Index,strContents,pStageD->aDeactiveBndr[i]); // ItemData 동시 설정..
			Index++;

			DeActBndrDatas.Add(pStageD->aDeactiveBndr[i]);
		}
	}

	if(nGr != DeActBndrDatas.GetSize())
	{
		int nAc = DeActBndrDatas.GetSize();
		pStageD->aDeactiveBndr.RemoveAll();
		for(int  i = 0 ; i < nAc ; i++)
		{
			pStageD->aDeactiveBndr.Add(DeActBndrDatas[i]);
		}
	}
	
	UpdateGroupListByCurState();
	
}

void CCMHydStageBndrDlg::UpdateGroupListByCurState()  // Active List Deactive List 설정후에 호출할것...
{
	CMap<T_BNGR_K,T_BNGR_K,T_BNGR_K,T_BNGR_K> m_ActGrups;
	CMap<T_BNGR_K,T_BNGR_K,T_BNGR_K,T_BNGR_K> m_DeActGrups;
	m_ActGrups.InitHashTable(256);
	m_DeActGrups.InitHashTable(256);
	
	T_BNGR_K DummyK = 0;

	int nAct = m_ActiveGroupList.GetItemCount();
	for( int i = 0; i < nAct ; i++)
	{
		m_ActGrups.SetAt((T_BNGR_K)m_ActiveGroupList.GetItemData(i),DummyK);
	}
	
	int nDeAct = m_DeactiveGroupList.GetItemCount();
	for(int  i = 0; i < nDeAct ;i ++)
	{
		m_DeActGrups.SetAt((T_BNGR_K)m_DeactiveGroupList.GetItemData(i),DummyK);
	}


	// Dialog 가 Window로 존재한다는 가정...
	CArray<T_BNGR_K,T_BNGR_K> rGroupKeys;
	m_wndGroupList     .ResetContent();
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKeyList(rGroupKeys);

	int nGr = rGroupKeys.GetSize();
	T_BNGR_D GroupD;
	T_BNGR_K GroupK;
	int Index = 0;
	for(int  i = 0 ; i < nGr ; i++)
	{
		if(!m_ActGrups.Lookup(rGroupKeys[i],GroupK) && !m_DeActGrups.Lookup(rGroupKeys[i],GroupK))
		{
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(rGroupKeys[i],GroupD))
			{
				m_wndGroupList.AddString(GroupD.GroupName);
				m_wndGroupList.SetItemData(Index, rGroupKeys[i]);
				Index++;
			}
		}
	}
}

void CCMHydStageBndrDlg::_AddFunc(int nWhat) // (0) Act Add (1) Deact Add
{
	
	CArray<T_BNGR_K,T_BNGR_K> arDActGKeys;
	CArray<T_BNGR_K,T_BNGR_K> arActGKeys ;

	
	int nGN = m_DeactiveGroupList.GetItemCount();
	 
	T_BNGR_K TempK;

	for( int i = 0 ; i < nGN ; i++)
	{

		TempK = (T_BNGR_K) m_DeactiveGroupList.GetItemData(i);
		arDActGKeys.Add(TempK);
	}

	nGN = m_ActiveGroupList.GetItemCount();

	for(int  i = 0 ;i < nGN ; i++)
	{
		TempK = (T_BNGR_K) m_ActiveGroupList.GetItemData(i);
		arActGKeys.Add(TempK);
	}
	
	CArray<T_BNGR_K, T_BNGR_K> SelGrupKeys;
	CArray<int, int> arSelIndex;

	int nSel = this->m_wndGroupList.GetSelCount();
	arSelIndex.SetSize(nSel);
	m_wndGroupList.GetSelItems(nSel,arSelIndex.GetData());

	for(int i = 0 ; i < nSel ; i++)
	{
		TempK = (T_BNGR_K) m_wndGroupList.GetItemData(arSelIndex[i]);
		SelGrupKeys.Add(TempK);
	}
 

	CArray<T_BNGR_K,T_BNGR_K> rResultKeys1;
	CArray<T_BNGR_K,T_BNGR_K> rResultKeys2;
	I_GENModelBase::GetCurMySelfST()->SubtractKey(SelGrupKeys ,arActGKeys ,rResultKeys1); // RList = Sel List - Active List 
	I_GENModelBase::GetCurMySelfST()->SubtractKey(rResultKeys1,arDActGKeys,rResultKeys2); // RList - Deactive List

	T_BNGR_D GroupD;
	//T_STAG_BNDR_BASE BndrBaseD;
	
	int nGr;
	CStringArray strContents;
	CString      StrTemp;
	 
	if(nWhat == 0)
	{
		nGr = rResultKeys2.GetSize();
		int nAct = m_ActiveGroupList.GetItemCount();
		
		for(int i = 0 ; i < nGr ; i++)
		{
			strContents.RemoveAll();
			
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(rResultKeys2[i],GroupD))
			{
				strContents.Add(GroupD.GroupName);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i+nAct,strContents,rResultKeys2[i]); 
				m_pCurStageD->aActiveBndr.Add(rResultKeys2[i]);
			}
		}
	}
	else
	{
		nGr = rResultKeys2.GetSize();
		int nAct = m_DeactiveGroupList.GetItemCount();
		for(int i = 0 ; i < nGr ; i++)
		{
			strContents.RemoveAll();
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(rResultKeys2[i],GroupD))
			{
				strContents.Add(GroupD.GroupName);
				CDlgUtil::SetListItem(&m_DeactiveGroupList,i+nAct,strContents,rResultKeys2[i]); 
				m_pCurStageD->aDeactiveBndr.Add(rResultKeys2[i]);
			}
		}
	}
	UpdateGroupListByCurState();
}

void CCMHydStageBndrDlg::OnActAddButton() 
{
	// TODO: Add your control notification handler code here
	_AddFunc(0);
}

void CCMHydStageBndrDlg::OnActDeleteButton() 
{
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_BNGR_K,T_BNGR_K> arSelGrupKey;
	CArray<T_BNGR_K,T_BNGR_K> arAllGrupKey;
	CMap<T_BNGR_K,T_BNGR_K,T_BNGR_K,T_BNGR_K> arGrupMap;
	
	CDlgUtil::GetSelectedListItemData(&m_ActiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aActiveBndr.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aActiveBndr[i]);
		arGrupMap.SetAt(m_pCurStageD->aActiveBndr[i],m_pCurStageD->aActiveBndr[i]);
	}

	int nSel = arSelItemData.GetSize();

	for(int i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_BNGR_K)arSelItemData[i]);
	}
	
	CArray<T_BNGR_K,T_BNGR_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);
	
	CString TStr;
	CStringArray Contents;
	T_BNGR_D GrupD;
	T_BNGR_K TBngrK;

	m_pCurStageD->aActiveBndr.RemoveAll();

	int nRes = aResultKey.GetSize();

	m_ActiveGroupList.DeleteAllItems();

	for(int  i = 0 ; i < nRes ; i++)
	{
		
		if(arGrupMap.Lookup(aResultKey[i],TBngrK))
		{
			
			m_pCurStageD->aActiveBndr.Add(aResultKey[i]);
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(aResultKey[i],GrupD))
			{
				Contents.RemoveAll();
				Contents.Add(GrupD.GroupName);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,aResultKey[i]);
			}
		}
	}

	UpdateGroupListByCurState();
}

void CCMHydStageBndrDlg::OnDeactAdd() 
{
	// TODO: Add your control notification handler code here
	_AddFunc(1);
}


void CCMHydStageBndrDlg::OnDeactDeleteButton() 
{
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_BNGR_K,T_BNGR_K> arSelGrupKey;
	CArray<T_BNGR_K,T_BNGR_K> arAllGrupKey;
		
	CDlgUtil::GetSelectedListItemData(&m_DeactiveGroupList,arSelItemData);
	
	int nSel = arSelItemData.GetSize();

	for(int i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_BNGR_K)arSelItemData[i]);
	}
	
	CArray<T_BNGR_K,T_BNGR_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(m_pCurStageD->aDeactiveBndr,arSelGrupKey,aResultKey);

	m_pCurStageD->aDeactiveBndr.RemoveAll();
	
	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_BNGR_D GrupD;

	m_DeactiveGroupList.DeleteAllItems();

	for(int  i = 0 ; i < nRes ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(aResultKey[i],GrupD))
		{
			m_pCurStageD->aDeactiveBndr.Add(aResultKey[i]);
			Contents.RemoveAll();
			Contents.Add(GrupD.GroupName);
			CDlgUtil::SetListItem(&m_DeactiveGroupList,i,Contents,aResultKey[i]);
		}
	}
	UpdateGroupListByCurState();
}
