// CMLoadGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadGroupDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_base\wg_base_CompFunc.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadGroupDlg dialog
#define CDialog CCMDlgBase


CCMLoadGroupDlg::CCMLoadGroupDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLoadGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMLoadGroupDlg)
	m_strGroupName = _T("");
	m_strSuffix = _T("");
	//}}AFX_DATA_INIT
}


void CCMLoadGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadGroupDlg)
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

BEGIN_MESSAGE_MAP(CCMLoadGroupDlg, CDialog)

	//{{AFX_MSG_MAP(CCMLoadGroupDlg)
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
// CCMLoadGroupDlg message handlers
BOOL CCMLoadGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_btnSortAsc.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_ascendlist.svg"));
	m_btnSortDsc.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_descendlist.svg"));
	m_btnUp.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_listup.svg"));
	m_btnDown.SetImage(_T("SVG\\Illustration\\Button\\ico16_dialog_definestructuregroup_listdown.svg"));


	m_wndGrupNameList.InsertColumn(0, _T("Col0"), LVCFMT_LEFT, 100);  
	m_wndGrupNameList.SetExtendedStyle(m_wndGrupNameList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_wndGrupNameList.ModifyStyle(NULL, LVS_SHOWSELALWAYS);
	m_wndGrupNameList.SetLoadType();

	UpdateLdgrList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLoadGroupDlg::UpdateLdgrList()
{
	CArray<CString,CString&> strLdgrList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrList(strLdgrList);
	
	m_wndGrupNameList.DeleteAllItems();
	int nLdgr = strLdgrList.GetSize();
	for(int i = 0 ; i < nLdgr ; i++)
	{    
		m_wndGrupNameList.InsertItem(i, strLdgrList[i]);  
		m_wndGrupNameList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER); 
	}

	m_wndGrupNameList.SetFocus();
}


void CCMLoadGroupDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMLoadGroupDlg::GenBysuffixButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CArray<int,int> SuffixList;
	CMap<int,int,int,int> SuffixMap;
	SuffixMap.InitHashTable(500);
	CStringArray    GroupNameArray;
	int TempInt;
	T_LDGR_D LdgrD;
	CArray<T_LDGR_D,T_LDGR_D&> LdgrDs;

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
		CDBDoc::GetDocPoint()->m_pDataCtrl->AddLdgr(LdgrDs);
	}

}

void CCMLoadGroupDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	/*
	UpdateData();
	T_LDGR_D LdgrD;
	LdgrD.GroupName = m_strGroupName;
	if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddLdgr(LdgrD))
	{

	}
	*/


	UpdateData();
	if(m_strSuffix.IsEmpty())
	{
		T_LDGR_D LdgrD;
		LdgrD.GroupName = m_strGroupName;
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddLdgr(LdgrD))
		{

		}
	}
	else
	{
		this->GenBysuffixButton();
	}
}

void CCMLoadGroupDlg::OnCmdDeleteButton() 
{
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	CArray<T_LDGR_K,T_LDGR_K> LdgrKeys;
	CString TStr;
	T_LDGR_K LdgrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		LdgrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKey(TStr);
		if(LdgrK)
			LdgrKeys.Add(LdgrK);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->DelLdgr(LdgrKeys);
}

void CCMLoadGroupDlg::OnCmdInvDeleteButton() 
{
	// TODO: Add your control notification handler code here
	CMap  <T_LDGR_K,T_LDGR_K,T_LDGR_K,T_LDGR_K> SelectedLdgrKeys;
	SelectedLdgrKeys.InitHashTable( 500 );

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses); 
	CArray<T_LDGR_K,T_LDGR_K> LdgrKeys;
	CString TStr;
	T_LDGR_K LdgrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		LdgrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKey(TStr);
		if(LdgrK)
		{
			if(!SelectedLdgrKeys.Lookup(LdgrK,LdgrK))
				SelectedLdgrKeys.SetAt(LdgrK,LdgrK);
		}
	}
	
	CArray<T_LDGR_K,T_LDGR_K> rKeyList;
	CArray<T_LDGR_K,T_LDGR_K> TargetKeys;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKeyList(rKeyList);

	int nKeys = rKeyList.GetSize();
	
	for(int i = 0 ; i < nKeys; i++)
	{
		if(!SelectedLdgrKeys.Lookup(rKeyList[i],LdgrK))
			TargetKeys.Add(rKeyList[i]);
	}

	CDBDoc::GetDocPoint()->m_pDataCtrl->DelLdgr(TargetKeys);
}



void CCMLoadGroupDlg::OnCmdModifyButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	T_LDGR_D TLdgrD;
	T_LDGR_K LdgrK;

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
		BOOL RetVal = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(Str,TLdgrD);
		LdgrK       = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKey(Str);

		if(LdgrK && RetVal)
		{
			CArray<T_LDGR_K,T_LDGR_K>  LdgrKeys;
			CArray<T_LDGR_D,T_LDGR_D&> LdgrDatas;
			LdgrKeys.Add(LdgrK);
			TLdgrD.GroupName = this->m_strGroupName;
			LdgrDatas.Add(TLdgrD);
			if(CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLdgr(LdgrKeys,LdgrDatas))
			{

			}
		}
	}
}

void CCMLoadGroupDlg::OnCmdASCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);

	CArray<CString, CString &> strLdgrList;
	CArray<T_LDGR_K, T_LDGR_K> rKeyList;
	CArray<T_LDGR_D, T_LDGR_D&> rDataList;  
	pAttrCtrl->GetLdgrList(strLdgrList);
	
	int nCount = strLdgrList.GetSize();  
	if (nCount>0)
	{
		qsort(strLdgrList.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);

		T_LDGR_K  LdgrK;
		T_LDGR_D  LdgrD;
		for (int i=0; i<nCount; i++)
		{
			LdgrD.Initialize();
			if (!pAttrCtrl->GetLdgr(strLdgrList[i], LdgrD)) continue;
			if (LdgrD.nGroupId == i+1) continue;

			LdgrD.nGroupId = i+1;
			LdgrK = pAttrCtrl->GetLdgrKey(strLdgrList[i]);

			rKeyList.Add(LdgrK);
			rDataList.Add(LdgrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLdgr(rKeyList, rDataList);
	}  
}

void CCMLoadGroupDlg::OnCmdDESCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<CString, CString &> strLdgrList;
	CArray<T_LDGR_K, T_LDGR_K> rKeyList;
	CArray<T_LDGR_D, T_LDGR_D&> rDataList;  
	pAttrCtrl->GetLdgrList(strLdgrList);
	
	int nCount = strLdgrList.GetSize();  
	if (nCount>0)
	{
		qsort(strLdgrList.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		
		T_LDGR_K  LdgrK;
		T_LDGR_D  LdgrD;
		for (int i=0; i<nCount; i++)
		{
			LdgrD.Initialize();
			if (!pAttrCtrl->GetLdgr(strLdgrList[i], LdgrD)) continue;
			if (LdgrD.nGroupId == i+1) continue;

			LdgrD.nGroupId = i+1;
			LdgrK = pAttrCtrl->GetLdgrKey(strLdgrList[i]);
			
			rKeyList.Add(LdgrK);
			rDataList.Add(LdgrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLdgr(rKeyList, rDataList);
	}
}

void CCMLoadGroupDlg::OnCmdUpButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;

	CArray<BOOL, BOOL> aSelList;     
	CStringArray       aSelString;
	CArray<CString, CString &> strLdgrList; 
	CArray<CString, CString &> strNewLdgrList; 
	pAttrCtrl->GetLdgrList(strLdgrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE; aSelString.Add(strLdgrList[SelIPoses[i]]); }

	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[0]==0);
	BOOL bFirstGrup = TRUE;

	strNewLdgrList.SetSize(nTotal);
	strNewLdgrList.InsertAt(0, strLdgrList[0]);
	for (int i=1; i<nTotal; i++)
	{
		bSel = aSelList[i];

		if (!bSel && bFirstGrup) bFirstGrup=FALSE;

		if (bFirstGrup)
		{
			if (!bFirst && bSel) strNewLdgrList.InsertAt(i-1, strLdgrList[i]);      
			else                 strNewLdgrList.InsertAt(i, strLdgrList[i]);
		}
		else
		{
			if (bSel) strNewLdgrList.InsertAt(i-1, strLdgrList[i]);
			else      strNewLdgrList.InsertAt(i, strLdgrList[i]);
		}
	}

	CArray<T_LDGR_K, T_LDGR_K> rKeyList;
	CArray<T_LDGR_D, T_LDGR_D&> rDataList;  
	T_LDGR_K  LdgrK;
	T_LDGR_D  LdgrD;
	for (int i=0; i<nTotal; i++)
	{
		LdgrD.Initialize();
		if (!pAttrCtrl->GetLdgr(strNewLdgrList[i], LdgrD)) continue;
		if (LdgrD.nGroupId == i+1) continue;

		LdgrD.nGroupId = i+1;
		LdgrK = pAttrCtrl->GetLdgrKey(strNewLdgrList[i]);
		
		rKeyList.Add(LdgrK);
		rDataList.Add(LdgrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLdgr(rKeyList, rDataList);
		
	UpdateSelectMark(aSelString);
}

void CCMLoadGroupDlg::OnCmdDownButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;
		
	CArray<BOOL, BOOL> aSelList;
	CStringArray       aSelString;
	CArray<CString, CString &> strLdgrList; 
	CArray<CString, CString &> strNewLdgrList; 
	pAttrCtrl->GetLdgrList(strLdgrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);  
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE; aSelString.Add(strLdgrList[SelIPoses[i]]);}
	
	
	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[nCount-1]==nTotal-1);
	BOOL bFirstGrup = TRUE;
	
	strNewLdgrList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) strNewLdgrList[i] = _T("");

	strNewLdgrList.SetAt(nTotal-1, strLdgrList[nTotal-1]);
	for (int i=nTotal-2; i>=0; i--)
	{
		bSel = aSelList[i];
		
		if (!bSel && bFirstGrup) bFirstGrup=FALSE;
		
		if (bFirstGrup)
		{
			if (!bFirst && bSel)
			{
				if (strNewLdgrList[i+1] != _T(""))
				{
					CString temp = strNewLdgrList[i+1];
					strNewLdgrList.SetAt(i, temp);
				}
				strNewLdgrList.SetAt(i+1, strLdgrList[i]);
			}
			else
			{
				strNewLdgrList.SetAt(i, strLdgrList[i]);
			}
		}
		else
		{
			if (bSel)
			{
				if (strNewLdgrList[i+1] != _T(""))
				{
					CString temp = strNewLdgrList[i+1];
					strNewLdgrList.SetAt(i, temp);
				}
				strNewLdgrList.SetAt(i+1, strLdgrList[i]);
			}
			else
			{
				strNewLdgrList.SetAt(i, strLdgrList[i]);
			}
		}
	}
	
	CArray<T_LDGR_K, T_LDGR_K> rKeyList;
	CArray<T_LDGR_D, T_LDGR_D&> rDataList;  
	T_LDGR_K  LdgrK;
	T_LDGR_D  LdgrD;
	for (int i=0; i<nTotal; i++)
	{
		LdgrD.Initialize();
		if (!pAttrCtrl->GetLdgr(strNewLdgrList[i], LdgrD)) continue;
		if (LdgrD.nGroupId == i+1) continue;

		LdgrD.nGroupId = i+1;
		LdgrK = pAttrCtrl->GetLdgrKey(strNewLdgrList[i]);
		
		rKeyList.Add(LdgrK);
		rDataList.Add(LdgrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLdgr(rKeyList, rDataList);
	
	UpdateSelectMark(aSelString);
}

void CCMLoadGroupDlg::UpdateSelectMark(CStringArray& aSelSZ)
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
void CCMLoadGroupDlg::UpdateListData()
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
	
	CArray<T_LDGR_K, T_LDGR_K> rKeyList;
	CArray<T_LDGR_D, T_LDGR_D&> rDataList;  
	T_LDGR_K  LdgrK;
	T_LDGR_D  LdgrD;
	int nTotal = m_wndGrupNameList.GetItemCount();

	for (int i=0; i<nTotal; i++)
	{
		LdgrD.Initialize();
		szList = m_wndGrupNameList.GetItemText(i, 0);

		if (!pAttrCtrl->GetLdgr(szList, LdgrD)) continue;
		if (LdgrD.nGroupId == i+1) continue;

		LdgrD.nGroupId = i+1;
		LdgrK = pAttrCtrl->GetLdgrKey(szList);  
		
		rKeyList.Add(LdgrK);
		rDataList.Add(LdgrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLdgr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMLoadGroupDlg::OnSelchangeCmdGroupNameList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCMLoadGroupDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CCMLoadGroupDlg::UpdateBuffer()
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
		case(UR_LDGR_ADD):
		case(UR_LDGR_DEL):
		case(UR_LDGR_MFD): 
		case(UR_LDGR_MFS):
				bIsLdgrCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	if(bIsLdgrCommand) 
		UpdateLdgrList();
}
