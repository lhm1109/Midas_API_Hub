// CMStageItemBndrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageItemBndrDlg.h"

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
// CCMStageItemBndrDlg dialog
#define CDialog CCMStageItemBase


CCMStageItemBndrDlg::CCMStageItemBndrDlg(CWnd* pParent /*=NULL*/)
	: CCMStageItemBase(CCMStageItemBndrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageItemBndrDlg)
	m_nPosOption = 1;
	//}}AFX_DATA_INIT
}


void CCMStageItemBndrDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMStageItemBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageItemBndrDlg)
	DDX_Control(pDX, IDC_CMD_GROUP_LIST, m_wndGroupList);
	DDX_Control(pDX, IDC_CMD_DEACTIVE_GROUP_LIST, m_DeactiveGroupList);
	DDX_Control(pDX, IDC_CMD_ACTIVE_GROUP_LIST, m_ActiveGroupList);
	DDX_Radio(pDX, IDC_CMD_ORI_POS_RADIO, m_nPosOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMStageItemBndrDlg, CDialog)

	//{{AFX_MSG_MAP(CCMStageItemBndrDlg)
	ON_BN_CLICKED(IDC_CMD_DEFGROUP_BUTTON, OnCmdDefineGroupButton)
	ON_BN_CLICKED(IDC_ACT_ADD_BUTTON, OnActAddButton)
	ON_BN_CLICKED(IDC_ACT_DELETE_BUTTON, OnActDeleteButton)
	ON_BN_CLICKED(IDC_ACT_MODIFY_BUTTON, OnActModifyButton)
	ON_BN_CLICKED(IDC_DEACT_ADD, OnDeactAdd)
	ON_BN_CLICKED(IDC_DEACT_DELETE_BUTTON, OnDeactDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageItemBndrDlg message handlers

void CCMStageItemBndrDlg::SetListCtrlHeader()
{
	CStringArray Htitles;
	CArray<float,float> HRatios;
	Htitles.Add(_LS(IDS_CMD0417__Name));
	Htitles.Add(_LS(IDS_CMD0417__Position));
	HRatios.Add(0.5f);
	HRatios.Add(0.5f);
	CDlgUtil::_SetListCtrlHeader(&m_ActiveGroupList,Htitles,&HRatios,NULL);
	Htitles.RemoveAll();
	HRatios.RemoveAll();
	Htitles.Add(_LS(IDS_CMD0417__Name));
	HRatios.Add(1.f);
	CDlgUtil::_SetListCtrlHeader(&m_DeactiveGroupList,Htitles,&HRatios,NULL);
}

BOOL CCMStageItemBndrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetListCtrlHeader();
	//UpdateGroupList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageItemBndrDlg::UpdateGroupList()
{

	CArray<CString,CString&> strGrupList;
	CArray<T_BNGR_K,T_BNGR_K> rKeyList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrList(strGrupList);
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKeyList(rKeyList);

	m_wndGroupList.ResetContent();
	int nGrup = strGrupList.GetSize();
	for(int i = 0 ; i < nGrup ; i++)
	{
		m_wndGroupList.AddString(strGrupList[i]);  
		m_wndGroupList.SetItemData(i,rKeyList[i]);
	}
}

void CCMStageItemBndrDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(GetSafeHwnd())
	{
		switch(lHint)
		{
		case D_UPDATE_DBALL:
		case D_UPDATE_DEFAULT:
			UpdateBuffer();
			break;
		case D_UPDATE_BUFFER_BEFORE:
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;
		case D_UPDATE_UNIT:
			break;
		case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
			break;
		case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
			break;
		default:
				//ASSERT(FALSE);
			break;
		}
	}
}

void CCMStageItemBndrDlg::UpdateBuffer()
{
	/*
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsBngrCommand = FALSE;	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		 
		case(UR_BNGR_ADD):
		case(UR_BNGR_DEL):
		case(UR_BNGR_MFD): 
		case(UR_BNGR_MFS):
					bIsBngrCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	if(bIsBngrCommand) 
	{
		this->SetCurStageData(m_pCurStageD);
		//UpdateGroupList();
	}
	*/
	
}

void CCMStageItemBndrDlg::OnCmdDefineGroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD,this);
}

void CCMStageItemBndrDlg::SetCurStageData(T_STAG_D *pStageD)
{
	m_pCurStageD = pStageD;

	m_ActiveGroupList  .DeleteAllItems();
	m_DeactiveGroupList.DeleteAllItems();

	CMap<T_BNGR_K, T_BNGR_K,int,int> ActBndrDatas  ;
	CArray<T_BNGR_K, T_BNGR_K> DeActBndrDatas;
	
	T_BNGR_D GroupD;
	

	int nGr = pStageD->aActiveBndr.GetSize();
	CStringArray strContents;
	CString      StrTemp;
	int Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(pStageD->aActiveBndr[i].BngrKey,GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);
			if(pStageD->aActiveBndr[i].nPosition == 1)
				StrTemp.Format(_LS(IDS_CMD0417__Original));
			else
				StrTemp.Format(_LS(IDS_CMD0417__Deformed));
			strContents.Add(StrTemp);

			CDlgUtil::SetListItem(&m_ActiveGroupList,Index,strContents,pStageD->aActiveBndr[i].BngrKey); // ItemData 동시 설정..
			Index++;
			ActBndrDatas.SetAt(pStageD->aActiveBndr[i].BngrKey,pStageD->aActiveBndr[i].nPosition);
		}
	}

	if(nGr != ActBndrDatas.GetCount())
	{
		POSITION Pos = ActBndrDatas.GetStartPosition();
		pStageD->aActiveBndr.RemoveAll();
		T_STAG_BNDR_BASE BndrBase;
		while(Pos)
		{
			ActBndrDatas.GetNextAssoc(Pos,BndrBase.BngrKey,BndrBase.nPosition);
			pStageD->aActiveBndr.Add(BndrBase);
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
		for(int i = 0 ; i < nAc ; i++)
		{
			pStageD->aDeactiveBndr.Add(DeActBndrDatas[i]);
		}
	}
	
	UpdateGroupListByCurState();
	
}


void CCMStageItemBndrDlg::UpdateGroupListByCurState()  // Active List Deactive List 설정후에 호출할것...
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
	for(int i = 0; i < nDeAct ;i ++)
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
	for(int i = 0 ; i < nGr ; i++)
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



void CCMStageItemBndrDlg::_AddFunc(int nWhat) // (0) Act Add (1) Deact Add
{
	
	CArray<T_BNGR_K,T_BNGR_K> arDActGKeys;
	CArray<T_BNGR_K,T_BNGR_K> arActGKeys ;

	int nPosOption = m_nPosOption +1;

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
	T_STAG_BNDR_BASE BndrBaseD;
	
	int nGr;
	CStringArray strContents;
	CString      StrTemp;
	 
	if(nWhat == 0)
	{
		nGr = rResultKeys2.GetSize();
		int nAct = m_ActiveGroupList.GetItemCount();
		
		if(nPosOption == 1)
			StrTemp.Format(_LS(IDS_CMD0417__Original));
		else if(nPosOption == 2)
			StrTemp.Format(_LS(IDS_CMD0417__Deformed));
		else
			ASSERT(FALSE);

		for(int  i = 0 ; i < nGr ; i++)
		{
			strContents.RemoveAll();
			
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(rResultKeys2[i],GroupD))
			{
				strContents.Add(GroupD.GroupName);
				strContents.Add(StrTemp);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i+nAct,strContents,rResultKeys2[i]); 
				BndrBaseD.BngrKey = rResultKeys2[i];
				BndrBaseD.nPosition = nPosOption;
				m_pCurStageD->aActiveBndr.Add(BndrBaseD);
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


void CCMStageItemBndrDlg::OnActAddButton() 
{
	UpdateData();
	_AddFunc(0);	
}

void CCMStageItemBndrDlg::OnActDeleteButton() 
{

	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_BNGR_K,T_BNGR_K> arSelGrupKey;
	CArray<T_BNGR_K,T_BNGR_K> arAllGrupKey;
	CMap<T_BNGR_K,T_BNGR_K,int,int> arGrupMap;
	
	CDlgUtil::GetSelectedListItemData(&m_ActiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aActiveBndr.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aActiveBndr[i].BngrKey);
		arGrupMap.SetAt(m_pCurStageD->aActiveBndr[i].BngrKey,m_pCurStageD->aActiveBndr[i].nPosition);
	}

	int nSel = arSelItemData.GetSize();

	for(int  i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_BNGR_K)arSelItemData[i]);
	}
	
	CArray<T_BNGR_K,T_BNGR_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);
	
	CString TStr;
	CStringArray Contents;
	T_BNGR_D GrupD;
	T_STAG_BNDR_BASE BndrBase;


	m_pCurStageD->aActiveBndr.RemoveAll();

	int nRes = aResultKey.GetSize();

	m_ActiveGroupList.DeleteAllItems();

	for(int  i = 0 ; i < nRes ; i++)
	{
		BndrBase.BngrKey = aResultKey[i];
		if(arGrupMap.Lookup(BndrBase.BngrKey,BndrBase.nPosition))
		{
			
			m_pCurStageD->aActiveBndr.Add(BndrBase);
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(BndrBase.BngrKey,GrupD))
			{
				Contents.RemoveAll();
				Contents.Add(GrupD.GroupName);
				if(BndrBase.nPosition == 1)
				{
					TStr.Format(_LS(IDS_CMD0417__Original));
				}
				else if(BndrBase.nPosition == 2)
				{
					TStr.Format(_LS(IDS_CMD0417__Deformed));
				}
				else
					ASSERT(FALSE);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,BndrBase.BngrKey);
			}
		}
	}

	UpdateGroupListByCurState();
	

	/*
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_BNGR_K,T_BNGR_K> arSelGrupKey;
	CArray<T_BNGR_K,T_BNGR_K> arAllGrupKey;
		
	CDlgUtil::GetSelectedListItemData(&m_ActiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aActiveBndr.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aActiveBndr[i]);
	}

	int nSel = arSelItemData.GetSize();

	for( i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_BNGR_K)arSelItemData[i]);
	}
	
	CArray<T_BNGR_K,T_BNGR_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);

	m_pCurStageD->aActiveBndr.RemoveAll();
	
	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_BNGR_D GrupD;

	m_ActiveGroupList.DeleteAllItems();

	for( i = 0 ; i < nRes ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(aResultKey[i],GrupD))
		{
			m_pCurStageD->aActiveBndr.Add(aResultKey[i]);
			Contents.RemoveAll();
			Contents.Add(GrupD.GroupName);
			CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,aResultKey[i]);
		}
	}
	UpdateGroupListByCurState();
	*/
}

void CCMStageItemBndrDlg::OnActModifyButton()
{
	UpdateData();
	
	CString      StrPosOpt;
	int nPosOpt = m_nPosOption + 1;

	
	if(nPosOpt == 1)
	{
		StrPosOpt.Format(_LS(IDS_CMD0417__Original));
	
	}
	else if(nPosOpt == 2)
	{
		StrPosOpt.Format(_LS(IDS_CMD0417__Deformed));
	}
	else
		ASSERT(FALSE);


	CArray<int,int> SelectedIndex;
	CDlgUtil::GetSelectedListItemIndex(&m_ActiveGroupList,SelectedIndex);
	T_BNGR_D TGrupD;
	T_BNGR_K TGrupK;
	//T_STAG_BNDR_BASE BndrBase;
	
	int nSel = SelectedIndex.GetSize();
	for( int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aActiveBndr[SelectedIndex[i]].BngrKey;
		
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(TGrupK,TGrupD))
		{
			m_pCurStageD->aActiveBndr[SelectedIndex[i]].nPosition = nPosOpt;
		}
	}

	CStringArray Contents;
	nSel = m_pCurStageD->aActiveBndr.GetSize();
	
	m_ActiveGroupList.DeleteAllItems();

	
	for(int  i = 0 ; i < nSel ; i++)
	{
		TGrupK  = m_pCurStageD->aActiveBndr[i].BngrKey;
		nPosOpt = m_pCurStageD->aActiveBndr[i].nPosition;
		if(nPosOpt == 1)
			StrPosOpt.Format(_LS(IDS_CMD0417__Original));
		else if(nPosOpt == 2)
			StrPosOpt.Format(_LS(IDS_CMD0417__Deformed));
		else
			ASSERT(FALSE);

		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(TGrupK,TGrupD))
		{
			Contents.RemoveAll();
			Contents.Add(TGrupD.GroupName);
			Contents.Add(StrPosOpt);
			CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,TGrupK); 
		}
	}
}

void CCMStageItemBndrDlg::OnDeactAdd() 
{
	this->_AddFunc(1);	
}

void CCMStageItemBndrDlg::OnDeactDeleteButton() 
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

void CCMStageItemBndrDlg::ResetParam()
{
	m_nPosOption = 0; 
	UpdateData(FALSE);
}
