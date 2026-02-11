// CMStageItemElemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageItemElemDlg.h"

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
// CCMStageItemElemDlg dialog
#define CDialog CCMStageItemBase


CCMStageItemElemDlg::CCMStageItemElemDlg(CWnd* pParent /*=NULL*/)
	: CCMStageItemBase(CCMStageItemElemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageItemElemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMStageItemElemDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMStageItemBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageItemElemDlg)
	DDX_Control(pDX, IDC_CMD_REDIST_EDIT, m_wndRedistEdit);
	DDX_Control(pDX, IDC_CMD_AGE_EDIT, m_wndAgeEdit);
	DDX_Control(pDX, IDC_CMD_GROUP_LIST, m_wndGroupList);
	DDX_Control(pDX, IDC_CMD_DEACTIVE_GROUP_LIST, m_DeactiveGroupList);
	DDX_Control(pDX, IDC_CMD_ACTIVE_GROUP_LIST, m_ActiveGroupList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMStageItemElemDlg, CDialog)

	//{{AFX_MSG_MAP(CCMStageItemElemDlg)
	ON_BN_CLICKED(IDC_CMD_DEFGROUP_BUTTON, OnCmdDefgroupButton)
	ON_BN_CLICKED(IDC_DEACT_DELETE_BUTTON, OnDeactDeleteButton)
	ON_BN_CLICKED(IDC_DEACT_ADD, OnDeactAdd)
	ON_BN_CLICKED(IDC_ACT_ADD_BUTTON, OnActAddButton)
	ON_BN_CLICKED(IDC_ACT_DELETE_BUTTON, OnActDeleteButton)
	ON_BN_CLICKED(IDC_ACT_MODIFY_BUTTON, OnActModifyButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_AGE_SPIN, OnDeltaposCmdAgeSpin)
	ON_BN_CLICKED(IDC_DEACT_MODIFY_BUTTON, OnDeactModifyButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_REDIST_SPIN, OnDeltaposCmdRedistSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define CDialog CCMStageItemBase
/////////////////////////////////////////////////////////////////////////////
// CCMStageItemElemDlg message handlers
void CCMStageItemElemDlg::SetListCtrlHeader()
{
	CStringArray Htitles;
	CArray<float,float> HRatios;
	Htitles.Add(_LS(IDS_CMD0417__Name));
	Htitles.Add(_LS(IDS_CMD0417__Age));
	HRatios.Add(0.5f);
	HRatios.Add(0.5f);
	CDlgUtil::_SetListCtrlHeader(&m_ActiveGroupList,Htitles,&HRatios,NULL);
	Htitles.RemoveAll();
	Htitles.Add(_LS(IDS_CMD0417__Name));
	Htitles.Add(_LS(IDS_CMD0417__Redist_));
	CDlgUtil::_SetListCtrlHeader(&m_DeactiveGroupList,Htitles,&HRatios,NULL);
}

BOOL CCMStageItemElemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetListCtrlHeader();
	//UpdateGroupList();
	m_wndAgeEdit.SetWindowText(_T("0"));
	m_wndRedistEdit.SetWindowText(_T("100"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageItemElemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMStageItemElemDlg::UpdateBuffer()
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
		case(UR_GRUP_ADD):
		case(UR_GRUP_DEL):
		case(UR_GRUP_MFD): 
		case(UR_GRUP_MFS):
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

void CCMStageItemElemDlg::UpdateGroupList()
{
	CArray<CString,CString&> strGrupList;
	CArray<T_GRUP_K,T_GRUP_K> rKeyList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupList(strGrupList);
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKeyList(rKeyList);
	m_wndGroupList.ResetContent();
	int nGrup = strGrupList.GetSize();
	for(int i = 0 ; i < nGrup ; i++)
	{
		m_wndGroupList.AddString(strGrupList[i]);  
		m_wndGroupList.SetItemData(i,rKeyList[i]);
	}
	
}

void CCMStageItemElemDlg::OnCmdDefgroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMStrtGrupDefDlg::IDD,this);
}

////////////////////////////////////////////////////////////
// 최초 설정을 전제로 한다. 
void CCMStageItemElemDlg::SetCurStageData(T_STAG_D *pStageD)
{
	
	this->m_pCurStageD = pStageD;
	
	// Dialog 가 Window로 존재한다는 가정...
	CArray<T_GRUP_K,T_GRUP_K> rGroupKeys;
	CArray<T_GRUP_K,T_GRUP_K> ActiveGrupKeys;
	
	CMap<T_GRUP_K,T_GRUP_K,double, double> ActElemData;
	CMap<T_GRUP_K,T_GRUP_K,double, double > DeactElemData;
	
	//CArray<T_GRUP_K,T_GRUP_K>       DeActElemData;
	
	
	m_ActiveGroupList  .DeleteAllItems();
	m_DeactiveGroupList.DeleteAllItems();
	int nGr;
	
	
	T_GRUP_D GroupD;

	nGr = pStageD->aActiveElem.GetSize();
	CStringArray strContents;
	CString TempStr;
	int Index = 0;
	for(int   i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(pStageD->aActiveElem[i].GrupKey,GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);
			TempStr.Format(_T("%.8g"),pStageD->aActiveElem[i].dAge);
			strContents.Add(TempStr);
			CDlgUtil::SetListItem(&m_ActiveGroupList,Index,strContents,pStageD->aActiveElem[i].GrupKey); // ItemData 동시 설정..
			Index++;
			//ActElemData.SetAt(pStageD->aActiveElem[i].GrupKey,pStageD->aActiveElem[i].nAge);
			ActElemData.SetAt(pStageD->aActiveElem[i].GrupKey,pStageD->aActiveElem[i].dAge);
		}
	}

	if(nGr != ActElemData.GetCount())
	{
		// StageData를 Update한다. 
		T_STAG_AELEM_BASE ElemBase;
		T_GRUP_K GrupK;
		double      Age;
		pStageD->aActiveElem.RemoveAll();
		POSITION Pos = ActElemData.GetStartPosition();
		while(Pos)
		{
			ActElemData.GetNextAssoc(Pos,GrupK,Age);
			ElemBase.GrupKey = GrupK;
			ElemBase.dAge    = Age;
			pStageD->aActiveElem.Add(ElemBase);
		}
	}



	nGr = pStageD->aDeactiveElem.GetSize();
	Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(pStageD->aDeactiveElem[i].GrupKey,GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);
			TempStr.Format(_T("%g"),pStageD->aDeactiveElem[i].dRedistribution);
			strContents.Add(TempStr);
			CDlgUtil::SetListItem(&m_DeactiveGroupList,Index,strContents,pStageD->aDeactiveElem[i].GrupKey); // ItemData 동시 설정..
			Index++;
			DeactElemData.SetAt(pStageD->aDeactiveElem[i].GrupKey,pStageD->aDeactiveElem[i].dRedistribution);
		}
	}

	if(nGr != DeactElemData.GetCount())
	{
		// StageData를 Update한다. 
		T_STAG_DELEM_BASE ElemBase;
		T_GRUP_K GrupK;
		double   Redist;
		pStageD->aDeactiveElem.RemoveAll();
		POSITION Pos = DeactElemData.GetStartPosition();
		while(Pos)
		{
			DeactElemData.GetNextAssoc(Pos,GrupK,Redist);
			ElemBase.GrupKey = GrupK;
			ElemBase.dRedistribution  = Redist;
			pStageD->aDeactiveElem.Add(ElemBase);
		}
	}
	UpdateGroupListByCurState();
}



void CCMStageItemElemDlg::UpdateGroupListByCurState()  // Active List Deactive List 설정후에 호출할것...
{
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> m_ActGrups;
	CMap<T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> m_DeActGrups;
	m_ActGrups.InitHashTable(256);
	m_DeActGrups.InitHashTable(256);
	
	T_GRUP_K DummyK = 0;

	int nAct = m_ActiveGroupList.GetItemCount();
	for( int i = 0; i < nAct ; i++)
	{
		m_ActGrups.SetAt((T_GRUP_K)m_ActiveGroupList.GetItemData(i),DummyK);
	}
	
	int nDeAct = m_DeactiveGroupList.GetItemCount();
	for(int i = 0; i < nDeAct ;i ++)
	{
		m_DeActGrups.SetAt((T_GRUP_K)m_DeactiveGroupList.GetItemData(i),DummyK);
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
		if(!m_ActGrups.Lookup(rGroupKeys[i],GroupK) && !m_DeActGrups.Lookup(rGroupKeys[i],GroupK))
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



void CCMStageItemElemDlg::OnActAddButton() 
{
	_AddFunc(0);
}






void CCMStageItemElemDlg::_AddFunc(int nWhat) // (0) Act Add (1) Deact Add
{
	
	CArray<T_GRUP_K,T_GRUP_K> arDActGKeys;
	CArray<T_GRUP_K,T_GRUP_K> arActGKeys ;

	int nGN = m_DeactiveGroupList.GetItemCount();
	 
	T_GRUP_K TempK;

	for( int i = 0 ; i < nGN ; i++)
	{

		TempK = (T_GRUP_K) m_DeactiveGroupList.GetItemData(i);
		arDActGKeys.Add(TempK);
	}

	nGN = m_ActiveGroupList.GetItemCount();

	for(int i = 0 ;i < nGN ; i++)
	{
		TempK = (T_GRUP_K) m_ActiveGroupList.GetItemData(i);
		arActGKeys.Add(TempK);
	}

	

	CArray<T_GRUP_K, T_GRUP_K> SelGrupKeys;
	CArray<int, int> arSelIndex;

	int nSel = this->m_wndGroupList.GetSelCount();
	arSelIndex.SetSize(nSel);
	m_wndGroupList.GetSelItems(nSel,arSelIndex.GetData());

	for(int i = 0 ; i < nSel ; i++)
	{
		TempK = (T_GRUP_K) m_wndGroupList.GetItemData(arSelIndex[i]);
		SelGrupKeys.Add(TempK);
	}
 

	CArray<T_GRUP_K,T_GRUP_K> rResultKeys1;
	CArray<T_GRUP_K,T_GRUP_K> rResultKeys2;
	I_GENModelBase::GetCurMySelfST()->SubtractKey(SelGrupKeys ,arActGKeys ,rResultKeys1); // RList = Sel List - Active List 
	I_GENModelBase::GetCurMySelfST()->SubtractKey(rResultKeys1,arDActGKeys,rResultKeys2); // RList - Deactive List

	T_GRUP_D GroupD;
	int nGr;
	CStringArray strContents;
	CString      StrAge , TempStr ;

	
	if(nWhat == 0)
	{
		T_STAG_AELEM_BASE StageElemD;

		nGr = rResultKeys2.GetSize();
		
		m_wndAgeEdit.GetWindowText(StrAge);

		if(!CStrParser::GetFloatNumber(StrAge,m_Age))
		{
			m_Age = 0;
		}
			
		StrAge.Format(_T("%.5g"),m_Age);
		
		int nAct = m_ActiveGroupList.GetItemCount();

		for(int i = 0 ; i < nGr ; i++)
		{
			strContents.RemoveAll();
			
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(rResultKeys2[i],GroupD))
			{
				TempStr.Format(_T("%s"),GroupD.GroupName);
				strContents.Add(TempStr);
				strContents.Add(StrAge);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i+nAct,strContents,rResultKeys2[i]); 
				StageElemD.GrupKey = rResultKeys2[i];
				StageElemD.dAge    = m_Age;
				m_pCurStageD->aActiveElem.Add(StageElemD);
			}
		}
	}
	else
	{
		CString StrRedist;
		T_STAG_DELEM_BASE StageElemD;

		m_wndRedistEdit.GetWindowText(StrRedist);
		if(!CStrParser::GetFloatNumber(StrRedist,m_Redist))
		{
			m_Redist = 0;
		}
		StrRedist.Format(_T("%g"),m_Redist);

		nGr = rResultKeys2.GetSize();
		int nDeAct = m_DeactiveGroupList.GetItemCount();
		for(int i = 0 ; i < nGr ; i++)
		{
			strContents.RemoveAll();
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(rResultKeys2[i],GroupD))
			{
				TempStr.Format(_T("%s"),GroupD.GroupName);
				strContents.Add(TempStr);
				strContents.Add(StrRedist);
				CDlgUtil::SetListItem(&m_DeactiveGroupList,i+nDeAct,strContents,rResultKeys2[i]); 
				StageElemD.GrupKey         = rResultKeys2[i];
				StageElemD.dRedistribution = m_Redist;
				m_pCurStageD->aDeactiveElem.Add(StageElemD);
			}
		}
	}

	UpdateGroupListByCurState();
	
}





void CCMStageItemElemDlg::OnActDeleteButton() 
{
 
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_GRUP_K,T_GRUP_K> arSelGrupKey;
	CArray<T_GRUP_K,T_GRUP_K> arAllGrupKey;
	CMap<T_GRUP_K,T_GRUP_K,double,double> arGrupMap;
	
	CDlgUtil::GetSelectedListItemData(&m_ActiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aActiveElem.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aActiveElem[i].GrupKey);
		arGrupMap.SetAt(m_pCurStageD->aActiveElem[i].GrupKey,m_pCurStageD->aActiveElem[i].dAge);
	}

	int nSel = arSelItemData.GetSize();

	for(int i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_GRUP_K)arSelItemData[i]);
	}
	
	CArray<T_GRUP_K,T_GRUP_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);

	m_pCurStageD->aActiveElem.RemoveAll();
	T_STAG_AELEM_BASE TElemBase;

	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_GRUP_D GrupD;

	m_ActiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nRes ; i++)
	{
		TElemBase.GrupKey = aResultKey[i];
		if(arGrupMap.Lookup(TElemBase.GrupKey,TElemBase.dAge))
		{
			
			m_pCurStageD->aActiveElem.Add(TElemBase);
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(TElemBase.GrupKey,GrupD))
			{
				Contents.RemoveAll();
				Contents.Add(GrupD.GroupName);
				TStr.Format(_T("%.8g"),TElemBase.dAge);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,TElemBase.GrupKey);
			}
		}
	}

	UpdateGroupListByCurState();
	
}

void CCMStageItemElemDlg::OnActModifyButton() 
{
	CString      StrAge;
	m_wndAgeEdit.GetWindowText(StrAge);
	if(!CStrParser::GetFloatNumber(StrAge,m_Age))
	{
		m_Age = 0.;
	}
	StrAge.Format(_T("%.8g"),m_Age);

	CArray<int,int> SelectedIndex;

	CDlgUtil::GetSelectedListItemIndex(&m_ActiveGroupList,SelectedIndex);
	T_GRUP_D TGrupD;
	T_GRUP_K TGrupK;
	

	int nSel = SelectedIndex.GetSize();
	for( int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aActiveElem[SelectedIndex[i]].GrupKey;
		
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(TGrupK,TGrupD))
		{
			m_pCurStageD->aActiveElem[SelectedIndex[i]].dAge = m_Age;
		}
	}

	CStringArray Contents;
	nSel = m_pCurStageD->aActiveElem.GetSize();
	
	m_ActiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aActiveElem[i].GrupKey;
		m_Age  = m_pCurStageD->aActiveElem[i].dAge;
		StrAge.Format(_T("%.8g"),m_Age);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(TGrupK,TGrupD))
		{
			Contents.RemoveAll();
			Contents.Add(TGrupD.GroupName);
			Contents.Add(StrAge);
			CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,TGrupK); 
		}
	}
}

void CCMStageItemElemDlg::OnDeactAdd() 
{
	_AddFunc(1);
}




void CCMStageItemElemDlg::OnDeactModifyButton() 
{
	CString      StrRedist;
	m_wndRedistEdit.GetWindowText(StrRedist);
	if(!CStrParser::GetFloatNumber(StrRedist,m_Redist))
	{
		m_Redist = 0;
	}
	StrRedist.Format(_T("%d"),m_Redist);
	
	CArray<int,int> SelectedIndex;

	CDlgUtil::GetSelectedListItemIndex(&m_DeactiveGroupList,SelectedIndex);
	T_GRUP_D TGrupD;
	T_GRUP_K TGrupK;
	

	int nSel = SelectedIndex.GetSize();
	for( int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aDeactiveElem[SelectedIndex[i]].GrupKey;
		
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(TGrupK,TGrupD))
		{
			m_pCurStageD->aDeactiveElem[SelectedIndex[i]].dRedistribution = m_Redist;
		}
	}

	CStringArray Contents;
	nSel = m_pCurStageD->aDeactiveElem.GetSize();
	
	m_DeactiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nSel ; i++)
	{
		TGrupK    = m_pCurStageD->aDeactiveElem[i].GrupKey;
		m_Redist  = m_pCurStageD->aDeactiveElem[i].dRedistribution;
		StrRedist.Format(_T("%g"),m_Redist);
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(TGrupK,TGrupD))
		{
			Contents.RemoveAll();
			Contents.Add(TGrupD.GroupName);
			Contents.Add(StrRedist);
			CDlgUtil::SetListItem(&m_DeactiveGroupList,i,Contents,TGrupK); 
		}
	}
	
}


void CCMStageItemElemDlg::OnDeactDeleteButton() 
{
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_GRUP_K,T_GRUP_K> arSelGrupKey;
	CArray<T_GRUP_K,T_GRUP_K> arAllGrupKey;
	CMap<T_GRUP_K,T_GRUP_K,double,double> arGrupMap;
	
	CDlgUtil::GetSelectedListItemData(&m_DeactiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aDeactiveElem.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aDeactiveElem[i].GrupKey);
		arGrupMap.SetAt(m_pCurStageD->aDeactiveElem[i].GrupKey,m_pCurStageD->aDeactiveElem[i].dRedistribution);
	}

	int nSel = arSelItemData.GetSize();

	for(int i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_GRUP_K)arSelItemData[i]);
	}
	
	CArray<T_GRUP_K,T_GRUP_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);

	m_pCurStageD->aDeactiveElem.RemoveAll();
	T_STAG_DELEM_BASE TElemBase;

	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_GRUP_D GrupD;

	m_DeactiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nRes ; i++)
	{
		TElemBase.GrupKey = aResultKey[i];
		if(arGrupMap.Lookup(TElemBase.GrupKey,TElemBase.dRedistribution))
		{
			m_pCurStageD->aDeactiveElem.Add(TElemBase);
			/// 여기 까지....
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(TElemBase.GrupKey,GrupD))
			{
				Contents.RemoveAll();
				Contents.Add(GrupD.GroupName);
				TStr.Format(_T("%g"),TElemBase.dRedistribution);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_DeactiveGroupList,i,Contents,TElemBase.GrupKey);
			}
		}
	}

	UpdateGroupListByCurState();
}



void CCMStageItemElemDlg::OnDeltaposCmdAgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//CDlgUtil::SetSpinValue(this,IDC_CMD_AGE_EDIT,0,-1,pNMHDR,pResult);
	CDlgUtil::SetFloatSpinValue(this,IDC_CMD_AGE_EDIT,_T("0"),_T("UMAX"),1.0,pNMHDR,pResult);
}


void CCMStageItemElemDlg::OnDeltaposCmdRedistSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_CMD_REDIST_EDIT,0,100,pNMHDR,pResult);
}

void CCMStageItemElemDlg::ResetParam()
{
	m_wndRedistEdit.SetWindowText(_T("100"));
	m_wndAgeEdit   .SetWindowText(_T("0"));
}
