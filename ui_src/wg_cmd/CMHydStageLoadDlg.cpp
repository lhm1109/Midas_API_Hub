// CMHydStageLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHydStageLoadDlg.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "CMLoadGroupDlg.h"
#include "ExportFunc.h"
	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageLoadDlg dialog


CCMHydStageLoadDlg::CCMHydStageLoadDlg(CWnd* pParent /*=NULL*/)
	: CCMHydStageItemBase(CCMHydStageLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMHydStageLoadDlg)
	//}}AFX_DATA_INIT
}


void CCMHydStageLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMHydStageItemBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHydStageLoadDlg)
	DDX_Control(pDX, IDC_CMD_DEACTDAY_COMBO, m_wndDeactDayCombo);
	DDX_Control(pDX, IDC_CMD_ACTDAY_COMBO, m_wndActDayCombo);
	DDX_Control(pDX, IDC_CMD_GROUP_LIST, m_wndGroupList);
	DDX_Control(pDX, IDC_CMD_DEACTIVE_GROUP_LIST, m_DeactiveGroupList);
	DDX_Control(pDX, IDC_CMD_ACTIVE_GROUP_LIST, m_ActiveGroupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMHydStageLoadDlg, CCMHydStageItemBase)
	//{{AFX_MSG_MAP(CCMHydStageLoadDlg)
	ON_BN_CLICKED(IDC_CMD_DEFGROUP_BUTTON, OnCmdDefgroupButton)
	ON_BN_CLICKED(IDC_ACT_ADD_BUTTON, OnActAddButton)
	ON_BN_CLICKED(IDC_ACT_DELETE_BUTTON, OnActDeleteButton)
	ON_BN_CLICKED(IDC_ACT_MODIFY_BUTTON, OnActModifyButton)
	ON_BN_CLICKED(IDC_DEACT_ADD, OnDeactAdd)
	ON_BN_CLICKED(IDC_DEACT_DELETE_BUTTON, OnDeactDeleteButton)
	ON_BN_CLICKED(IDC_DEACT_MODIFY_BUTTON, OnDeactModifyButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageLoadDlg message handlers

void CCMHydStageLoadDlg::SetListCtrlHeader()
{
	CStringArray Htitles;
	CArray<float,float> HRatios;
	Htitles.Add(_LS(IDS_CMD0417__Name));
	Htitles.Add(_LS(IDS_CMD0417__Time_hr_));
	HRatios.Add(0.5);
	HRatios.Add(0.5);
	CDlgUtil::_SetListCtrlHeader(&m_ActiveGroupList  ,Htitles,&HRatios,NULL);
	CDlgUtil::_SetListCtrlHeader(&m_DeactiveGroupList,Htitles,&HRatios,NULL);
}

BOOL CCMHydStageLoadDlg::OnInitDialog() 
{
	CCMHydStageItemBase::OnInitDialog();
	
	SetListCtrlHeader();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMHydStageLoadDlg::UpdateGroupList()
{
	CArray<CString,CString&> strGrupList;
	CArray<T_LDGR_K,T_LDGR_K> rKeyList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrList(strGrupList);
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKeyList(rKeyList);
	
	m_wndGroupList.ResetContent();
	int nGrup = strGrupList.GetSize();
	for(int i = 0 ; i < nGrup ; i++)
	{
		m_wndGroupList.AddString(strGrupList[i]);  
		m_wndGroupList.SetItemData(i,rKeyList[i]);
	}
}

void CCMHydStageLoadDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCMHydStageLoadDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsGroupCommand = FALSE;	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		 
		case(UR_LDGR_ADD):
		case(UR_LDGR_DEL):
		case(UR_LDGR_MFD): 
		case(UR_LDGR_MFS):
					bIsGroupCommand = TRUE;
				break;
		default:
			break;
		}
	} 

	if(bIsGroupCommand) 
	{
		//UpdateGroupList();
		this->SetCurStageData(m_pCurStageD);
	}
}

void CCMHydStageLoadDlg::OnCmdDefgroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD,this);
}

/*
void CCMHydStageLoadDlg::SetCurStageDataOnly(T_STAG_D *pStageD)
{





}
*/




void CCMHydStageLoadDlg::SetCurStageData(T_HSTG_D* pStageD)
{

	m_pCurStageD = pStageD;
	
	// Dialog 가 Window로 존재한다는 가정...
	CArray<T_LDGR_K,T_LDGR_K> rGroupKeys;
	CArray<T_LDGR_K,T_LDGR_K> ActiveGrupKeys;
	CArray<T_LDGR_K,T_LDGR_K> DeactiveGrupKeys;

	CMap<T_LDGR_K,T_LDGR_K, double , double > ActLoadData  ;
	CMap<T_LDGR_K,T_LDGR_K, double , double > DeActLoadData;
	CMap<double,double,double,double>               DayMap;

	int nStep = pStageD->aStep.GetSize();
	double dDay;
	
	DayMap.SetAt(0,0);
	DayMap.SetAt(-1,-1);

	for(int i = 0 ; i < nStep ; i++)
	{
		dDay = pStageD->aStep[i];
		DayMap.SetAt(dDay,dDay);
	}
		
	m_ActiveGroupList  .DeleteAllItems();
	m_DeactiveGroupList.DeleteAllItems();
	T_LDGR_D GroupD;
	
	CStringArray strContents;
	CString TempStr;
	
	int nGr = pStageD->aActiveLoad.GetSize();
	int Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(pStageD->aActiveLoad[i].LdgrKey,GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);

			if(!DayMap.Lookup(pStageD->aActiveLoad[i].dDay,dDay))
				pStageD->aActiveLoad[i].dDay = 0.;
			
			if(pStageD->aActiveLoad[i].dDay == 0.)
				TempStr.Format(_LS(IDS_CMD0417__First));
			else if(pStageD->aActiveLoad[i].dDay == -1.)
				TempStr.Format(_LS(IDS_CMD0417__Last));
			else
				TempStr.Format(_T("%.8g"),pStageD->aActiveLoad[i].dDay);
			strContents.Add(TempStr);
			CDlgUtil::SetListItem(&m_ActiveGroupList,Index,strContents,pStageD->aActiveLoad[i].LdgrKey); // ItemData 동시 설정..
			Index++;
			ActLoadData.SetAt(pStageD->aActiveLoad[i].LdgrKey,pStageD->aActiveLoad[i].dDay);
		}
	}

	if(nGr != ActLoadData.GetCount())
	{
		// Current Stage Data를 Update한다. 
		T_HSTG_LOAD_BASE LoadBaseD;
		POSITION Pos = ActLoadData.GetStartPosition();
		pStageD->aActiveLoad.RemoveAll();
		while(Pos)
		{
			ActLoadData.GetNextAssoc(Pos,LoadBaseD.LdgrKey,LoadBaseD.dDay);
			pStageD->aActiveLoad.Add(LoadBaseD);
		}
	}

	nGr = pStageD->aDeactiveLoad.GetSize();
	Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(pStageD->aDeactiveLoad[i].LdgrKey,GroupD))
		{
			strContents.RemoveAll();
			strContents.Add(GroupD.GroupName);

			if(!DayMap.Lookup(pStageD->aDeactiveLoad[i].dDay,dDay))
				pStageD->aDeactiveLoad[i].dDay = 0.;
	
			if(pStageD->aDeactiveLoad[i].dDay == 0.)
				TempStr.Format(_LS(IDS_CMD0417__First));
			else if(pStageD->aDeactiveLoad[i].dDay == -1.)
				TempStr.Format(_LS(IDS_CMD0417__Last));
			else
				TempStr.Format(_T("%.8g"),pStageD->aDeactiveLoad[i].dDay);
			strContents.Add(TempStr);
			CDlgUtil::SetListItem(&m_DeactiveGroupList,Index,strContents,
														pStageD->aDeactiveLoad[i].LdgrKey); // ItemData 동시 설정..
			Index++;
			DeActLoadData.SetAt(pStageD->aDeactiveLoad[i].LdgrKey,
														pStageD->aDeactiveLoad[i].dDay);
		}
	}

	if(nGr != DeActLoadData.GetCount())
	{
		// Current Stage Data를 Update한다. 
		T_HSTG_LOAD_BASE LoadBaseD;
		POSITION Pos = DeActLoadData.GetStartPosition();
		pStageD->aDeactiveLoad.RemoveAll();
		while(Pos)
		{
			DeActLoadData.GetNextAssoc(Pos,LoadBaseD.LdgrKey,LoadBaseD.dDay);
			pStageD->aDeactiveLoad.Add(LoadBaseD);
		}
	}
	SetDayCombo();
	UpdateGroupListByCurState();

}



void CCMHydStageLoadDlg::SetDayCombo()
{
	static double FirstLast[2] = {0.,-1.};
	int nSize = m_pCurStageD->aStep.GetSize();

	m_wndActDayCombo  .ResetContent();
	m_wndDeactDayCombo.ResetContent();

//  m_wndActDayCombo.AddString(_LS(IDS_CMD0417__First));
//  m_wndActDayCombo.SetItemData(0,(DWORD)&FirstLast[0]);
//  m_wndActDayCombo.AddString(_LS(IDS_CMD0417__Last));
//  m_wndActDayCombo.SetItemData(1,(DWORD)&FirstLast[1]);
	

	CString StrTemp;

	for(int i = 0 ; i < nSize ; i++)
	{
		StrTemp.Format(_T("%.8g"),m_pCurStageD->aStep[i]);
		m_wndActDayCombo.AddString(StrTemp);
		m_wndActDayCombo.SetItemData(i,(DWORD_PTR)(&m_pCurStageD->aStep[i]));
	}

//  m_wndDeactDayCombo.AddString(_LS(IDS_CMD0417__First));
//  m_wndDeactDayCombo.SetItemData(0,(DWORD)&FirstLast[0]);
//  m_wndDeactDayCombo.AddString(_LS(IDS_CMD0417__Last));
//  m_wndDeactDayCombo.SetItemData(1,(DWORD)&FirstLast[1]);

	for(int i = 0 ; i < nSize ; i++)
	{
		StrTemp.Format(_T("%.8g"),m_pCurStageD->aStep[i]);
		m_wndDeactDayCombo.AddString(StrTemp);
		m_wndDeactDayCombo.SetItemData(i,(DWORD_PTR)(&m_pCurStageD->aStep[i]));
	}

	m_wndActDayCombo.SetCurSel(0);
	m_wndDeactDayCombo.SetCurSel(0);
}

BOOL CCMHydStageLoadDlg::GetActAge(double& dAge,CString& StrAge)
{
	int nCurSel = m_wndActDayCombo.GetCurSel();
	if(CB_ERR == nCurSel) return FALSE;
	
	dAge = *(double*)m_wndActDayCombo.GetItemData(nCurSel);
	if(dAge == 0.) StrAge.Format(_LS(IDS_CMD0417__First));
	else if(dAge == -1.) StrAge.Format(_LS(IDS_CMD0417__Last));
	else StrAge.Format(_T("%.8g"),dAge);
	return TRUE;
}

BOOL CCMHydStageLoadDlg::GetDeactAge(double& dAge, CString& StrAge)
{
	int nCurSel = m_wndDeactDayCombo.GetCurSel();
	if(CB_ERR == nCurSel) return FALSE;
	
	dAge = *(double*)m_wndDeactDayCombo.GetItemData(nCurSel);
	if(dAge == 0.) StrAge.Format(_LS(IDS_CMD0417__First));
	else if(dAge == -1.) StrAge.Format(_LS(IDS_CMD0417__Last));
	else StrAge.Format(_T("%.8g"),dAge);
	return TRUE;
}

void CCMHydStageLoadDlg::UpdateGroupListByCurState()  // Active List Deactive List 설정후에 호출할것...
{
	
	CMap<T_LDGR_K,T_LDGR_K,T_LDGR_K,T_LDGR_K> m_ActGrups;
	CMap<T_LDGR_K,T_LDGR_K,T_LDGR_K,T_LDGR_K> m_DeActGrups;
	m_ActGrups.InitHashTable(256);
	m_DeActGrups.InitHashTable(256);
	
	T_LDGR_K DummyK = 0;

	int nAct = m_ActiveGroupList.GetItemCount();
	for( int i = 0; i < nAct ; i++)
	{
		m_ActGrups.SetAt((T_LDGR_K)m_ActiveGroupList.GetItemData(i),DummyK);
	}
	
	int nDeAct = m_DeactiveGroupList.GetItemCount();
	for(int i = 0; i < nDeAct ;i ++)
	{
		m_DeActGrups.SetAt((T_LDGR_K)m_DeactiveGroupList.GetItemData(i),DummyK);
	}


	// Dialog 가 Window로 존재한다는 가정...
	CArray<T_LDGR_K,T_LDGR_K> rGroupKeys;
	m_wndGroupList     .ResetContent();
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKeyList(rGroupKeys);

	int nGr = rGroupKeys.GetSize();
	T_LDGR_D GroupD;
	T_LDGR_K GroupK;
	int Index = 0;
	for(int i = 0 ; i < nGr ; i++)
	{
		if(!m_ActGrups.Lookup(rGroupKeys[i],GroupK) && !m_DeActGrups.Lookup(rGroupKeys[i],GroupK))
		{
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(rGroupKeys[i],GroupD))
			{
				m_wndGroupList.AddString(GroupD.GroupName);
				m_wndGroupList.SetItemData(Index, rGroupKeys[i]);
				Index++;
			}
		}
	}
	
}


void CCMHydStageLoadDlg::_AddFunc(int nWhat) // (0) Act Add (1) Deact Add
{

	CArray<T_LDGR_K,T_LDGR_K> arDActGKeys;
	CArray<T_LDGR_K,T_LDGR_K> arActGKeys ;

	int nGN = m_DeactiveGroupList.GetItemCount();
	 
	T_LDGR_K TempK;

	for( int i = 0 ; i < nGN ; i++)
	{

		TempK = (T_LDGR_K) m_DeactiveGroupList.GetItemData(i);
		arDActGKeys.Add(TempK);
	}

	nGN = m_ActiveGroupList.GetItemCount();

	for(int  i = 0 ;i < nGN ; i++)
	{
		TempK = (T_LDGR_K) m_ActiveGroupList.GetItemData(i);
		arActGKeys.Add(TempK);
	}
	
	CArray<T_LDGR_K, T_LDGR_K> SelGrupKeys;
	CArray<int, int> arSelIndex;

	int nSel = this->m_wndGroupList.GetSelCount();
	arSelIndex.SetSize(nSel);
	m_wndGroupList.GetSelItems(nSel,arSelIndex.GetData());

	for(int i = 0 ; i < nSel ; i++)
	{
		TempK = (T_LDGR_K) m_wndGroupList.GetItemData(arSelIndex[i]);
		SelGrupKeys.Add(TempK);
	}
 

	CArray<T_LDGR_K,T_LDGR_K> rResultKeys1;
	CArray<T_LDGR_K,T_LDGR_K> rResultKeys2;
	I_GENModelBase::GetCurMySelfST()->SubtractKey(SelGrupKeys ,arActGKeys ,rResultKeys1); // RList = Sel List - Active List 
	I_GENModelBase::GetCurMySelfST()->SubtractKey(rResultKeys1,arDActGKeys,rResultKeys2); // RList - Deactive List

	T_LDGR_D GroupD;
	T_HSTG_LOAD_BASE StageLoadD;
	int nGr;
	CStringArray strContents;
	CString      StrAge , TempStr ;
	
	if(nWhat == 0)
	{
		nGr = rResultKeys2.GetSize();
		if(!GetActAge(m_Age,StrAge))
		{
			m_Age = 0.;
			StrAge.Format(_LS(IDS_CMD0417__First));
		}
				
		int nAct = m_ActiveGroupList.GetItemCount();

		for(int  i = 0 ; i < nGr ; i++)
		{
			strContents.RemoveAll();
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(rResultKeys2[i],GroupD))
			{
				TempStr.Format(_T("%s"),GroupD.GroupName);
				strContents.Add(TempStr);
				strContents.Add(StrAge);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i+nAct,strContents,rResultKeys2[i]); 
				StageLoadD.LdgrKey         = rResultKeys2[i];
				StageLoadD.dDay    = m_Age          ;
				m_pCurStageD->aActiveLoad.Add(StageLoadD);
			}
		}
	}
	else
	{
		
		nGr = rResultKeys2.GetSize();
		if(!GetDeactAge(m_Age,StrAge))
		{
			m_Age = 0.; 
			StrAge.Format(_LS(IDS_CMD0417__First));
		}

		int nDeAct = m_DeactiveGroupList.GetItemCount();
		
		for(int i = 0 ; i < nGr ; i++)
		{
			strContents.RemoveAll();
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(rResultKeys2[i],GroupD))
			{
				TempStr.Format(_T("%s"),GroupD.GroupName);
				strContents.Add(TempStr);
				strContents.Add(StrAge);
				CDlgUtil::SetListItem(&m_DeactiveGroupList,i+nDeAct,strContents,rResultKeys2[i]); 
				StageLoadD.LdgrKey      = rResultKeys2[i];
				StageLoadD.dDay = m_Age;
				m_pCurStageD->aDeactiveLoad.Add(StageLoadD);
			}
		}
	}
	UpdateGroupListByCurState();

}



void CCMHydStageLoadDlg::OnActAddButton() 
{
	_AddFunc(0);
}

void CCMHydStageLoadDlg::OnActDeleteButton() 
{

	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_LDGR_K,T_LDGR_K> arSelGrupKey;
	CArray<T_LDGR_K,T_LDGR_K> arAllGrupKey;
	CMap<T_LDGR_K,T_LDGR_K,double,double> arGrupMap;
	
	CDlgUtil::GetSelectedListItemData(&m_ActiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aActiveLoad.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aActiveLoad[i].LdgrKey);
		arGrupMap .SetAt(m_pCurStageD->aActiveLoad[i].LdgrKey,m_pCurStageD->aActiveLoad[i].dDay);
	}

	int nSel = arSelItemData.GetSize();

	for(int i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_LDGR_K)arSelItemData[i]);
	}
	
	CArray<T_LDGR_K,T_LDGR_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);

	m_pCurStageD->aActiveLoad.RemoveAll();
	T_HSTG_LOAD_BASE TLoadBase;

	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_LDGR_D GrupD;

	m_ActiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nRes ; i++)
	{
		TLoadBase.LdgrKey = aResultKey[i];
		if(arGrupMap.Lookup(TLoadBase.LdgrKey,TLoadBase.dDay))
		{
			m_pCurStageD->aActiveLoad.Add(TLoadBase);
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(TLoadBase.LdgrKey,GrupD))
			{
				Contents.RemoveAll();
				Contents.Add(GrupD.GroupName);
				if(TLoadBase.dDay == 0.)
					TStr.Format(_LS(IDS_CMD0417__First));
				else if(TLoadBase.dDay == -1.)
					TStr.Format(_LS(IDS_CMD0417__Last));
				else
					TStr.Format(_T("%.8g"),TLoadBase.dDay);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,TLoadBase.LdgrKey);
			}
		}
	}
	UpdateGroupListByCurState();

}

void CCMHydStageLoadDlg::OnActModifyButton() 
{
	
	CString      StrAge;

	if(!GetActAge(m_Age,StrAge))
	{
		m_Age = 0.; 
		StrAge.Format(_LS(IDS_CMD0417__First));
	}
	
	CArray<int,int> SelectedIndex;

	CDlgUtil::GetSelectedListItemIndex(&m_ActiveGroupList,SelectedIndex);
	T_LDGR_D TGrupD;
	T_LDGR_K TGrupK;
	

	int nSel = SelectedIndex.GetSize();
	for( int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aActiveLoad[SelectedIndex[i]].LdgrKey;
		
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(TGrupK,TGrupD))
		{
			m_pCurStageD->aActiveLoad[SelectedIndex[i]].dDay = m_Age;
		}
	}

	CStringArray Contents;
	nSel = m_pCurStageD->aActiveLoad.GetSize();
	
	m_ActiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aActiveLoad[i].LdgrKey;
		m_Age  = m_pCurStageD->aActiveLoad[i].dDay;
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(TGrupK,TGrupD))
		{
			Contents.RemoveAll();
			Contents.Add(TGrupD.GroupName);
			if(m_Age == 0.)
				StrAge.Format(_LS(IDS_CMD0417__First));
			else if(m_Age == -1.)
				StrAge.Format(_LS(IDS_CMD0417__Last));
			else
				StrAge.Format(_T("%.8g"),m_Age);

			Contents.Add(StrAge);
			CDlgUtil::SetListItem(&m_ActiveGroupList,i,Contents,TGrupK); 
		}
	}
	
}

void CCMHydStageLoadDlg::OnDeactAdd() 
{
	// TODO: Add your control notification handler code here
	_AddFunc(1);
}

void CCMHydStageLoadDlg::OnDeactDeleteButton() 
{
	
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_LDGR_K,T_LDGR_K> arSelGrupKey;
	CArray<T_LDGR_K,T_LDGR_K> arAllGrupKey;
	CMap<T_LDGR_K,T_LDGR_K,double,double> arGrupMap;
	
	CDlgUtil::GetSelectedListItemData(&m_DeactiveGroupList,arSelItemData);

	int nItem = this->m_pCurStageD->aDeactiveLoad.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		arAllGrupKey.Add(m_pCurStageD->aDeactiveLoad[i].LdgrKey);
		arGrupMap .SetAt(m_pCurStageD->aDeactiveLoad[i].LdgrKey,
										 m_pCurStageD->aDeactiveLoad[i].dDay);
	}

	int nSel = arSelItemData.GetSize();

	for(int i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_LDGR_K)arSelItemData[i]);
	}
	
	CArray<T_LDGR_K,T_LDGR_K>aResultKey;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(arAllGrupKey,arSelGrupKey,aResultKey);

	m_pCurStageD->aDeactiveLoad.RemoveAll();
	T_HSTG_LOAD_BASE TLoadBase;

	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_LDGR_D GrupD;

	m_DeactiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nRes ; i++)
	{
		TLoadBase.LdgrKey = aResultKey[i];
		if(arGrupMap.Lookup(TLoadBase.LdgrKey,TLoadBase.dDay))
		{
			m_pCurStageD->aDeactiveLoad.Add(TLoadBase);
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(TLoadBase.LdgrKey,GrupD))
			{
				Contents.RemoveAll();
				Contents.Add(GrupD.GroupName);
				if(TLoadBase.dDay == 0.) 
					TStr.Format(_LS(IDS_CMD0417__First));
				else if(TLoadBase.dDay == -1.)
					TStr.Format(_LS(IDS_CMD0417__Last));
				else
					TStr.Format(_T("%.8g"),TLoadBase.dDay);
				
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_DeactiveGroupList,i,Contents,TLoadBase.LdgrKey);
			}
		}
	}
	UpdateGroupListByCurState();

	

	/*
	CArray<DWORD, DWORD> arSelItemData;
	CArray<T_LDGR_K,T_LDGR_K> arSelGrupKey;
	CArray<T_LDGR_K,T_LDGR_K> arAllGrupKey;
		
	CDlgUtil::GetSelectedListItemData(&m_DeactiveGroupList,arSelItemData);
	
	int nSel = arSelItemData.GetSize();

	for(int i = 0 ;i < nSel ; i++)
	{
		arSelGrupKey.Add((T_LDGR_K)arSelItemData[i]);
	}
	
	CArray<T_LDGR_K,T_LDGR_K>aResultKey;
	
 
	
	I_GENModelBase::GetCurMySelfST()->SubtractKey(m_pCurStageD->aDeactiveLoad,arSelGrupKey,aResultKey);

	m_pCurStageD->aDeactiveLoad.RemoveAll();
	
	int nRes = aResultKey.GetSize();
	
	CString TStr;
	CStringArray Contents;
	T_LDGR_D GrupD;

	m_DeactiveGroupList.DeleteAllItems();

	for( i = 0 ; i < nRes ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(aResultKey[i],GrupD))
		{
			m_pCurStageD->aDeactiveLoad.Add(aResultKey[i]);
			Contents.RemoveAll();
			Contents.Add(GrupD.GroupName);
			CDlgUtil::SetListItem(&m_DeactiveGroupList,i,Contents,aResultKey[i]);
		}
	}
	UpdateGroupListByCurState();
	*/
	
}

/*
void CCMHydStageLoadDlg::OnDeltaposCmdDelaySpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_CMD_AGE_EDIT,0,-1,pNMHDR,pResult);
}

*/

void CCMHydStageLoadDlg::ResetParam()
{
	//m_wndAgeEdit.SetWindowText(_T("0"));
}

void CCMHydStageLoadDlg::OnDeactModifyButton() 
{

	CString      StrAge;

	if(!GetDeactAge(m_Age,StrAge))
	{
		m_Age = 0; 
		StrAge.Format(_LS(IDS_CMD0417__First));
	}
	
	CArray<int,int> SelectedIndex;

	CDlgUtil::GetSelectedListItemIndex(&m_DeactiveGroupList,SelectedIndex);
	T_LDGR_D TGrupD;
	T_LDGR_K TGrupK;
	

	int nSel = SelectedIndex.GetSize();
	for( int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aDeactiveLoad[SelectedIndex[i]].LdgrKey;
		
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(TGrupK,TGrupD))
		{
			m_pCurStageD->aDeactiveLoad[SelectedIndex[i]].dDay = m_Age;
		}
	}

	CStringArray Contents;
	nSel = m_pCurStageD->aDeactiveLoad.GetSize();
	
	m_DeactiveGroupList.DeleteAllItems();

	for(int i = 0 ; i < nSel ; i++)
	{
		TGrupK = m_pCurStageD->aDeactiveLoad[i].LdgrKey;
		m_Age  = m_pCurStageD->aDeactiveLoad[i].dDay;
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(TGrupK,TGrupD))
		{
			Contents.RemoveAll();
			Contents.Add(TGrupD.GroupName);
			if(m_Age ==0.) 
				StrAge.Format(_LS(IDS_CMD0417__First));
			else if(m_Age == -1.)
				StrAge.Format(_LS(IDS_CMD0417__Last));
			else
				StrAge.Format(_T("%.8g"),m_Age);

			Contents.Add(StrAge);
			CDlgUtil::SetListItem(&m_DeactiveGroupList,i,Contents,TGrupK); 
		}
	}

}
