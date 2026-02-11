// CMStrtGrupDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStrtGrupDefDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CCMDlgBase
/////////////////////////////////////////////////////////////////////////////
// CCMStrtGrupDefDlg dialog


CCMStrtGrupDefDlg::CCMStrtGrupDefDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMStrtGrupDefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStrtGrupDefDlg)
	m_strGroupName = _T("");
	m_strSuffix = _T("");
	//}}AFX_DATA_INIT
}


void CCMStrtGrupDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStrtGrupDefDlg)
	DDX_Control(pDX, IDC_CMD_BNDR_GROUP_EDIT, m_wndGroupName);
	DDX_Control(pDX, IDC_CMD_GROUP_NAME_LIST, m_wndGrupNameList);
	DDX_Text(pDX, IDC_CMD_BNDR_GROUP_EDIT, m_strGroupName);
	DDX_Text(pDX, IDC_CMD_SUFFIX_EDIT, m_strSuffix);  
	DDX_Control(pDX, IDC_CMD_GRP_SORT_ASC_BTN, m_btnSortAsc);
	DDX_Control(pDX, IDC_CMD_GRP_SORT_DSC_BTN, m_btnSortDsc);
	DDX_Control(pDX, IDC_CMD_GRP_UP_BTN, m_btnUp);
	DDX_Control(pDX, IDC_CMD_GRP_DW_BTN, m_btnDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMStrtGrupDefDlg, CDialog)

	//{{AFX_MSG_MAP(CCMStrtGrupDefDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_DELETE_BUTTON, OnCmdDeleteButton)
	ON_BN_CLICKED(IDC_CMD_MODIFY_BUTTON, OnCmdModifyButton)	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_GROUP_NAME_LIST, OnSelchangeCmdGroupNameList)
	ON_BN_CLICKED(IDC_CMD_INV_DELETE_BUTTON, OnCmdInvDeleteButton)
	ON_BN_CLICKED(IDC_CMD_GRP_SORT_ASC_BTN, OnCmdASCButton)
	ON_BN_CLICKED(IDC_CMD_GRP_SORT_DSC_BTN, OnCmdDESCButton)
	ON_BN_CLICKED(IDC_CMD_GRP_UP_BTN, OnCmdUpButton)
	ON_BN_CLICKED(IDC_CMD_GRP_DW_BTN, OnCmdDownButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStrtGrupDefDlg message handlers
BOOL CCMStrtGrupDefDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_btnSortAsc.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_ascendlist.svg"));
	m_btnSortDsc.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_descendlist.svg"));
	m_btnUp.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_listup.svg"));
	m_btnDown.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_listdown.svg"));


	m_wndGrupNameList.InsertColumn(0, _T("Col0"), LVCFMT_LEFT, 100);  
	m_wndGrupNameList.SetExtendedStyle(m_wndGrupNameList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_wndGrupNameList.ModifyStyle(NULL, LVS_SHOWSELALWAYS);
	m_wndGrupNameList.SetStrtType();

	// TODO: Add extra initialization here
	UpdateStgrList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStrtGrupDefDlg::UpdateStgrList()
{
	CArray<CString,CString&> strGrupList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupList(strGrupList);
	
	m_wndGrupNameList.DeleteAllItems();
	int nGrup = strGrupList.GetSize();
	for(int i = 0 ; i < nGrup ; i++)
	{
		m_wndGrupNameList.InsertItem(i, strGrupList[i]);  
		m_wndGrupNameList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER); 
	}  

	m_wndGrupNameList.SetFocus();
}


void CCMStrtGrupDefDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMStrtGrupDefDlg::GenBysuffixButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CArray<int,int> SuffixList;
	CMap<int,int,int,int> SuffixMap;
	SuffixMap.InitHashTable(500);
	CStringArray    GroupNameArray;
	int TempInt;
	T_GRUP_D LdgrD;
	CArray<T_GRUP_D,T_GRUP_D&> LdgrDs;

	if(CStrParser::ParsingListByTo(m_strSuffix,SuffixList))
	{
		CString TempStr;
		int nSuffix = SuffixList.GetSize();
		for(int i = 0 ; i < nSuffix ; i++)
		{
			if(!SuffixMap.Lookup(i,TempInt))
			{
				SuffixMap.SetAt(i,i);
				TempStr.Format(_T("%d"),SuffixList[i]);
				TempStr = m_strGroupName + TempStr;
				LdgrD.GroupName = TempStr;
				LdgrDs.Add(LdgrD);
				
				//CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr
			}
		}
		//CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr(BngrD);
		int nGrupD = LdgrDs.GetSize();
		for(int i = 0 ; i < nGrupD ; i++)
		{
			CDBDoc::GetDocPoint()->m_pDataCtrl->AddGrup(LdgrDs[i]);
		}
	}

}

void CCMStrtGrupDefDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	/*
	UpdateData();
	T_GRUP_D LdgrD;
	LdgrD.GroupName = m_strGroupName;
	if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddLdgr(LdgrD))
	{

	}
	*/


	UpdateData();
	if(m_strSuffix.IsEmpty())
	{
		T_GRUP_D LdgrD;
		LdgrD.GroupName = m_strGroupName;
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddGrup(LdgrD))
		{

		}
	}
	else
	{
		this->GenBysuffixButton();
	}
}

void CCMStrtGrupDefDlg::OnCmdDeleteButton() 
{
	CArray<int,int> SelIPoses;  
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	CArray<T_GRUP_K,T_GRUP_K> LdgrKeys;
	CString TStr;
	T_GRUP_K LdgrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		LdgrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKey(TStr);
		if(LdgrK)
			LdgrKeys.Add(LdgrK);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->DelGrup(LdgrKeys);
}

void CCMStrtGrupDefDlg::OnCmdInvDeleteButton() 
{
	// TODO: Add your control notification handler code here
	CMap  <T_GRUP_K,T_GRUP_K,T_GRUP_K,T_GRUP_K> SelectedLdgrKeys;
	SelectedLdgrKeys.InitHashTable( 500 );

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);  
	CArray<T_GRUP_K,T_GRUP_K> LdgrKeys;
	CString TStr;
	T_GRUP_K LdgrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		LdgrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKey(TStr);
		if(LdgrK)
		{
			if(!SelectedLdgrKeys.Lookup(LdgrK,LdgrK))
				SelectedLdgrKeys.SetAt(LdgrK,LdgrK);
		}
	}
	
	CArray<T_GRUP_K,T_GRUP_K> rKeyList;
	CArray<T_GRUP_K,T_GRUP_K> TargetKeys;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKeyList(rKeyList);

	int nKeys = rKeyList.GetSize();
	
	for(int i = 0 ; i < nKeys; i++)
	{
		if(!SelectedLdgrKeys.Lookup(rKeyList[i],LdgrK))
			TargetKeys.Add(rKeyList[i]);
	}

	CDBDoc::GetDocPoint()->m_pDataCtrl->DelGrup(TargetKeys);
}



void CCMStrtGrupDefDlg::OnCmdModifyButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	T_GRUP_D TLdgrD;
	T_GRUP_K LdgrK;

	CString Str;
	
	int nSelCount = m_wndGrupNameList.GetSelectedCount();
	
	if(nSelCount == 1)
	{
		POSITION  pos = m_wndGrupNameList.GetFirstSelectedItemPosition();      
		int nCurSel = m_wndGrupNameList.GetNextSelectedItem(pos);
		Str = m_wndGrupNameList.GetItemText(nCurSel, 0);
	}

	if(!Str.IsEmpty())
	{
		BOOL RetVal = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(Str,TLdgrD);
		LdgrK       = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKey(Str);

		if(LdgrK && RetVal)
		{
			CArray<T_GRUP_K,T_GRUP_K>  LdgrKeys;
			CArray<T_GRUP_D,T_GRUP_D&> LdgrDatas;
			LdgrKeys.Add(LdgrK);
			TLdgrD.GroupName = this->m_strGroupName;
			LdgrDatas.Add(TLdgrD);
			if(CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyGrup(LdgrKeys,LdgrDatas))
			{

			}
		}
	}
}

void CCMStrtGrupDefDlg::OnCmdASCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);

	CArray<CString, CString &> strGrupList;
	CArray<T_GRUP_K, T_GRUP_K> rKeyList;
	CArray<T_GRUP_D, T_GRUP_D&> rDataList;  
	pAttrCtrl->GetGrupList(strGrupList);
	
	int nCount = strGrupList.GetSize();  
	if (nCount>0)
	{
		qsort(strGrupList.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);

		T_GRUP_K  GrupK;
		T_GRUP_D  GrupD;
		for (int i=0; i<nCount; i++)
		{
			GrupD.Initialize();
			if (!pAttrCtrl->GetGrup(strGrupList[i], GrupD)) continue;
			if (GrupD.nGroupId == i+1) continue;

			GrupD.nGroupId = i+1;
			GrupK = pAttrCtrl->GetGrupKey(strGrupList[i]);

			rKeyList.Add(GrupK);
			rDataList.Add(GrupD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyGrup(rKeyList, rDataList);
	}

}

void CCMStrtGrupDefDlg::OnCmdDESCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<CString, CString &> strGrupList;
	CArray<T_GRUP_K, T_GRUP_K> rKeyList;
	CArray<T_GRUP_D, T_GRUP_D&> rDataList;  
	pAttrCtrl->GetGrupList(strGrupList);
	
	int nCount = strGrupList.GetSize();  
	if (nCount>0)
	{
		qsort(strGrupList.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		
		T_GRUP_K  GrupK;
		T_GRUP_D  GrupD;
		for (int i=0; i<nCount; i++)
		{
			GrupD.Initialize();
			if (!pAttrCtrl->GetGrup(strGrupList[i], GrupD)) continue;
			if (GrupD.nGroupId == i+1) continue;

			GrupD.nGroupId = i+1;
			GrupK = pAttrCtrl->GetGrupKey(strGrupList[i]);
			
			rKeyList.Add(GrupK);
			rDataList.Add(GrupD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyGrup(rKeyList, rDataList);
	}
}

void CCMStrtGrupDefDlg::OnCmdUpButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;

	CArray<BOOL, BOOL> aSelList;     
	CStringArray       aSelString;
	CArray<CString, CString &> strGrupList; 
	CArray<CString, CString &> strNewGrupList; 
	int nTotal = m_wndGrupNameList.GetItemCount();
	pAttrCtrl->GetGrupList(strGrupList);  

	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE; aSelString.Add(strGrupList[SelIPoses[i]]); }

	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[0]==0);
	BOOL bFirstGrup = TRUE;

	strNewGrupList.SetSize(nTotal);
	strNewGrupList.InsertAt(0, strGrupList[0]);
	for (int i=1; i<nTotal; i++)
	{
		bSel = aSelList[i];

		if (!bSel && bFirstGrup) bFirstGrup=FALSE;

		if (bFirstGrup)
		{
			if (!bFirst && bSel) strNewGrupList.InsertAt(i-1, strGrupList[i]);      
			else                 strNewGrupList.InsertAt(i, strGrupList[i]);
		}
		else
		{
			if (bSel) strNewGrupList.InsertAt(i-1, strGrupList[i]);
			else      strNewGrupList.InsertAt(i, strGrupList[i]);
		}
	}

	CArray<T_GRUP_K, T_GRUP_K> rKeyList;
	CArray<T_GRUP_D, T_GRUP_D&> rDataList;  
	T_GRUP_K  GrupK;
	T_GRUP_D  GrupD;
	for (int i=0; i<nTotal; i++)
	{
		GrupD.Initialize();
		if (!pAttrCtrl->GetGrup(strNewGrupList[i], GrupD)) continue;
		if (GrupD.nGroupId == i+1) continue;

		GrupD.nGroupId = i+1;
		GrupK = pAttrCtrl->GetGrupKey(strNewGrupList[i]);
		
		rKeyList.Add(GrupK);
		rDataList.Add(GrupD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyGrup(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
} 

void CCMStrtGrupDefDlg::OnCmdDownButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);  
	if (nCount<1) return;  
		
	CArray<BOOL, BOOL> aSelList;     
	CStringArray       aSelString;
	CArray<CString, CString &> strGrupList; 
	CArray<CString, CString &> strNewGrupList; 
	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	pAttrCtrl->GetGrupList(strGrupList);

	for (int i=0; i<nTotal; i++)
	{
		aSelList[i]=FALSE;
	}
	for (int i=0; i<nCount; i++)
	{ 
		aSelList[SelIPoses[i]]=TRUE;
		aSelString.Add(strGrupList[SelIPoses[i]]);
	}
	
	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[nCount-1]==nTotal-1);
	BOOL bFirstGrup = TRUE;
	
	strNewGrupList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) strNewGrupList[i] = _T("");

	strNewGrupList.SetAt(nTotal-1, strGrupList[nTotal-1]);
	for (int i=nTotal-2; i>=0; i--)
	{
		bSel = aSelList[i];
		
		if (!bSel && bFirstGrup) bFirstGrup=FALSE;
		
		if (bFirstGrup)
		{
			if (!bFirst && bSel)
			{
				if (strNewGrupList[i+1] != _T(""))
				{
					CString temp = strNewGrupList[i+1];
					strNewGrupList.SetAt(i, temp);
				}
				strNewGrupList.SetAt(i+1, strGrupList[i]);
			}
			else
			{
				strNewGrupList.SetAt(i, strGrupList[i]);
			}
		}
		else
		{
			if (bSel)
			{
				if (strNewGrupList[i+1] != _T(""))
				{
					CString temp = strNewGrupList[i+1];
					strNewGrupList.SetAt(i, temp);
				}
				strNewGrupList.SetAt(i+1, strGrupList[i]);
			}
			else
			{
				strNewGrupList.SetAt(i, strGrupList[i]);
			}
		}
	}
	
	CArray<T_GRUP_K, T_GRUP_K> rKeyList;
	CArray<T_GRUP_D, T_GRUP_D&> rDataList;  
	T_GRUP_K  GrupK;
	T_GRUP_D  GrupD;
	for (int i=0; i<nTotal; i++)
	{
		GrupD.Initialize();
		if (!pAttrCtrl->GetGrup(strNewGrupList[i], GrupD)) continue;
		if (GrupD.nGroupId == i+1) continue;

		GrupD.nGroupId = i+1;
		GrupK = pAttrCtrl->GetGrupKey(strNewGrupList[i]);
		
		rKeyList.Add(GrupK);
		rDataList.Add(GrupD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyGrup(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMStrtGrupDefDlg::UpdateSelectMark(CStringArray& aSelSZ)
{
	int nSize = aSelSZ.GetSize();
	if (nSize<1) return;

	for (int i=0; i<m_wndGrupNameList.GetItemCount(); i++)
	{
		CString szList = _T("");
		szList = m_wndGrupNameList.GetItemText(i, 0);
		int j = 0;
		for (j=0; j<nSize; j++)
		{
			if (aSelSZ[j] == szList) break;
		}

		if (j != nSize)
		{      
			m_wndGrupNameList.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);      
			m_wndGrupNameList.EnsureVisible(i, TRUE);
		}
	}
}

// PMS:XXXX-CJJEONG-20090508 (Tel : 2124) 
// ListCtrl 로 부터 Update되어다는 신호를 받고 List 를 돌며 Update
void CCMStrtGrupDefDlg::UpdateListData()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;

	CArray<int,int> SelIPoses;
	CStringArray       aSelString;
	int nSelCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nSelCount<1) return;
	CString   szList = _T("");

	aSelString.SetSize(nSelCount);
	for (int i=0; i<nSelCount; i++)
	{
		szList = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		aSelString[i] = szList;
	}

	CArray<T_GRUP_K, T_GRUP_K> rKeyList;
	CArray<T_GRUP_D, T_GRUP_D&> rDataList;  
	T_GRUP_K  GrupK;
	T_GRUP_D  GrupD;
	
	int nTotal = m_wndGrupNameList.GetItemCount();

	for (int i=0; i<nTotal; i++)
	{
		GrupD.Initialize();
		szList = m_wndGrupNameList.GetItemText(i, 0);

		if (!pAttrCtrl->GetGrup(szList, GrupD)) continue;
		if (GrupD.nGroupId == i+1) continue;

		GrupD.nGroupId = i+1;
		GrupK = pAttrCtrl->GetGrupKey(szList);
		
		rKeyList.Add(GrupK);
		rDataList.Add(GrupD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyGrup(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMStrtGrupDefDlg::OnSelchangeCmdGroupNameList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int nSel = m_wndGrupNameList.GetSelectedCount();
	if(nSel  <= 1)
	{
		CString Str;
		POSITION  pos = m_wndGrupNameList.GetFirstSelectedItemPosition();      
		int nCurSel = m_wndGrupNameList.GetNextSelectedItem(pos);
		Str = m_wndGrupNameList.GetItemText(nCurSel, 0);
		m_wndGroupName.SetWindowText(Str);    
	}
	else
	{
		m_wndGroupName.SetWindowText(_T(""));
	}

	*pResult = 0;
}

void CCMStrtGrupDefDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CCMStrtGrupDefDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsLdgrCommand = FALSE;	
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
				bIsLdgrCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	if(bIsLdgrCommand) 
		UpdateStgrList();
}
