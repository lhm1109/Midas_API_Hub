// CMTendonDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonDefDlg.h"

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
// CCMTendonDefDlg dialog


CCMTendonDefDlg::CCMTendonDefDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMTendonDefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTendonDefDlg)
	m_strGroupName = _T("");
	m_strSuffix = _T("");
	//}}AFX_DATA_INIT
}

/* IDD  수정  */
void CCMTendonDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTendonDefDlg)
	DDX_Control(pDX, IDC_CMD_TENDON_GROUP_EDIT, m_wndGroupName);
	DDX_Control(pDX, IDC_CMD_GROUP_NAME_LIST, m_wndGrupNameList);
	DDX_Text(pDX, IDC_CMD_TENDON_GROUP_EDIT, m_strGroupName);
	DDX_Text(pDX, IDC_CMD_TENDON_SUFFIX_EDIT, m_strSuffix);
	DDX_Control(pDX, IDC_CMD_GRP_SORT_ASC_BTN, m_btnSortAsc);
	DDX_Control(pDX, IDC_CMD_GRP_SORT_DSC_BTN, m_btnSortDsc);
	DDX_Control(pDX, IDC_CMD_GRP_UP_BTN, m_btnUp);
	DDX_Control(pDX, IDC_CMD_GRP_DW_BTN, m_btnDown);
	//}}AFX_DATA_MAP
}

/* IDD  수정  */
BEGIN_MESSAGE_MAP(CCMTendonDefDlg, CDialog)
	//{{AFX_MSG_MAP(CCMTendonDefDlg)
	ON_BN_CLICKED(IDC_CMD_TENDON_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_TENDON_ADD_BUTTON, OnCmdAddButton)
	ON_BN_CLICKED(IDC_CMD_TENDON_DELETE_BUTTON, OnCmdDeleteButton)
	ON_BN_CLICKED(IDC_CMD_TENDON_MODIFY_BUTTON, OnCmdModifyButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_GROUP_NAME_LIST, OnSelchangeCmdGroupNameList)
	ON_BN_CLICKED(IDC_CMD_TENDON_INV_DELETE_BUTTON, OnCmdInvDeleteButton)
	ON_BN_CLICKED(IDC_CMD_GRP_SORT_ASC_BTN, OnCmdASCButton)
	ON_BN_CLICKED(IDC_CMD_GRP_SORT_DSC_BTN, OnCmdDESCButton)
	ON_BN_CLICKED(IDC_CMD_GRP_UP_BTN, OnCmdUpButton)
	ON_BN_CLICKED(IDC_CMD_GRP_DW_BTN, OnCmdDownButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTendonDefDlg message handlers

BOOL CCMTendonDefDlg::OnInitDialog() 
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
	m_wndGrupNameList.SetTndnType();

	UpdateTendonList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CCMTendonDefDlg::UpdateTendonList()
{
	CArray<CString,CString&> strTendonList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdgrList(strTendonList);

	m_wndGrupNameList.DeleteAllItems();
	int nTdgr = strTendonList.GetSize();
	for(int i = 0 ; i < nTdgr ; i++)
	{
		m_wndGrupNameList.InsertItem(i, strTendonList[i]);  
		m_wndGrupNameList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER); 
	}

	m_wndGrupNameList.SetFocus();
}

void CCMTendonDefDlg::OnCmdBtnClose() 
{
	DestroyWindow();
}

void CCMTendonDefDlg::GenBysuffixButton() 
{
	UpdateData();
	CArray<int,int> SuffixList;
	CMap<int,int,int,int> SuffixMap;
	SuffixMap.InitHashTable(500);
	CStringArray    GroupNameArray;
	int TempInt;
	T_TDGR_D TdgrD; //T_BNGR_D BngrD;
	CArray<T_TDGR_D,T_TDGR_D&> Tdgrs;

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
				TdgrD.GroupName = TempStr;
				Tdgrs.Add(TdgrD);
				
				//CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr
			}
		}
		//CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr(BngrD);
		CDBDoc::GetDocPoint()->m_pDataCtrl->AddTdgr(Tdgrs);
	}

}

void CCMTendonDefDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strSuffix.IsEmpty())
	{
		T_TDGR_D TdgrD;
		TdgrD.GroupName = m_strGroupName;
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddTdgr(TdgrD))
		{
		}
	}
	else
	{
		this->GenBysuffixButton();
	}
}

void CCMTendonDefDlg::OnCmdDeleteButton() 
{
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	CArray<T_TDGR_K,T_TDGR_K> TdgrKeys;
	CString TStr;
	T_TDGR_K TdgrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		TdgrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdgrKey(TStr);
		if(TdgrK)
			TdgrKeys.Add(TdgrK);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->DelTdgr(TdgrKeys);
}

void CCMTendonDefDlg::OnCmdInvDeleteButton() 
{
	// TODO: Add your control notification handler code here
	CMap  <T_TDGR_K,T_TDGR_K,T_TDGR_K,T_TDGR_K> SelectedTdgrKeys;
	SelectedTdgrKeys.InitHashTable( 500 );

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses); 
	CArray<T_TDGR_K,T_TDGR_K> TdgrKeys;
	CString TStr;
	T_TDGR_K TdgrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		TdgrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdgrKey(TStr);
		if(TdgrK)
		{
			if(!SelectedTdgrKeys.Lookup(TdgrK,TdgrK))
				SelectedTdgrKeys.SetAt(TdgrK,TdgrK);
		}
	}
	
	CArray<T_TDGR_K,T_TDGR_K> rKeyList;
	CArray<T_TDGR_K,T_TDGR_K> TargetKeys;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdgrKeyList(rKeyList);

	int nKeys = rKeyList.GetSize();
	
	for(int  i = 0 ; i < nKeys; i++)
	{
		if(!SelectedTdgrKeys.Lookup(rKeyList[i],TdgrK))
			TargetKeys.Add(rKeyList[i]);
	}

	CDBDoc::GetDocPoint()->m_pDataCtrl->DelTdgr(TargetKeys);
}



void CCMTendonDefDlg::OnCmdModifyButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	T_TDGR_D TTDgrD;
	T_TDGR_K TDgrK;

	//BngrD.

	CString Str;
	
	int nSelCount = m_wndGrupNameList.GetSelectedCount();
	
	if(nSelCount == 1)
	{
		POSITION  pos = m_wndGrupNameList.GetFirstSelectedItemPosition();      
		int nCurSel = m_wndGrupNameList.GetNextSelectedItem(pos);
		Str = m_wndGrupNameList.GetItemText(nCurSel, 0);
	}

	//Str = this->m_strCurGroupName;
	if(!Str.IsEmpty())
	{
		BOOL RetVal = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdgr(Str,TTDgrD);
		TDgrK       = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdgrKey(Str);

		if(TDgrK && RetVal)
		{
			CArray<T_TDGR_K,T_TDGR_K>  TdgrKeys;
			CArray<T_TDGR_D,T_TDGR_D&> TdgrDatas;
			TdgrKeys.Add(TDgrK);
			TTDgrD.GroupName = this->m_strGroupName;
			TdgrDatas.Add(TTDgrD);
			if(CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdgr(TdgrKeys,TdgrDatas))
			{

			}
		}
	}
}

void CCMTendonDefDlg::OnCmdASCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);

	CArray<CString, CString &> strTdgrList;
	CArray<T_TDGR_K, T_TDGR_K> rKeyList;
	CArray<T_TDGR_D, T_TDGR_D&> rDataList;  
	pAttrCtrl->GetTdgrList(strTdgrList);
	
	int nCount = strTdgrList.GetSize();  
	if (nCount>0)
	{
		qsort(strTdgrList.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);

		T_TDGR_K  TdgrK;
		T_TDGR_D  TdgrD;
		for (int i=0; i<nCount; i++)
		{
			TdgrD.Initialize();
			if (!pAttrCtrl->GetTdgr(strTdgrList[i], TdgrD)) continue;
			if (TdgrD.nGroupId == i+1) continue;

			TdgrD.nGroupId = i+1;
			TdgrK = pAttrCtrl->GetTdgrKey(strTdgrList[i]);

			rKeyList.Add(TdgrK);
			rDataList.Add(TdgrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdgr(rKeyList, rDataList);
	}  
}

void CCMTendonDefDlg::OnCmdDESCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<CString, CString &> strTdgrList;
	CArray<T_TDGR_K, T_TDGR_K> rKeyList;
	CArray<T_TDGR_D, T_TDGR_D&> rDataList;  
	pAttrCtrl->GetTdgrList(strTdgrList);
	
	int nCount = strTdgrList.GetSize();  
	if (nCount>0)
	{
		qsort(strTdgrList.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		
		T_TDGR_K  TdgrK;
		T_TDGR_D  TdgrD;
		for (int i=0; i<nCount; i++)
		{
			TdgrD.Initialize();
			if (!pAttrCtrl->GetTdgr(strTdgrList[i], TdgrD)) continue;
			if (TdgrD.nGroupId == i+1) continue;

			TdgrD.nGroupId = i+1;
			TdgrK = pAttrCtrl->GetTdgrKey(strTdgrList[i]);
			
			rKeyList.Add(TdgrK);
			rDataList.Add(TdgrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdgr(rKeyList, rDataList);
	}
}

void CCMTendonDefDlg::OnCmdUpButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;

	CArray<BOOL, BOOL> aSelList;
	CStringArray       aSelString;
	CArray<CString, CString &> strTdgrList; 
	CArray<CString, CString &> strNewTdgrList; 
	pAttrCtrl->GetTdgrList(strTdgrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE;  aSelString.Add(strTdgrList[SelIPoses[i]]); }

	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[0]==0);
	BOOL bFirstGrup = TRUE;

	strNewTdgrList.SetSize(nTotal);
	strNewTdgrList.InsertAt(0, strTdgrList[0]);
	for (int i=1; i<nTotal; i++)
	{
		bSel = aSelList[i];

		if (!bSel && bFirstGrup) bFirstGrup=FALSE;

		if (bFirstGrup)
		{
			if (!bFirst && bSel) strNewTdgrList.InsertAt(i-1, strTdgrList[i]);      
			else                 strNewTdgrList.InsertAt(i, strTdgrList[i]);
		}
		else
		{
			if (bSel) strNewTdgrList.InsertAt(i-1, strTdgrList[i]);
			else      strNewTdgrList.InsertAt(i, strTdgrList[i]);
		}
	}

	CArray<T_TDGR_K, T_TDGR_K> rKeyList;
	CArray<T_TDGR_D, T_TDGR_D&> rDataList;  
	T_TDGR_K  TdgrK;
	T_TDGR_D  TdgrD;
	for (int i=0; i<nTotal; i++)
	{
		TdgrD.Initialize();
		if (!pAttrCtrl->GetTdgr(strNewTdgrList[i], TdgrD)) continue;
		if (TdgrD.nGroupId == i+1) continue;

		TdgrD.nGroupId = i+1;
		TdgrK = pAttrCtrl->GetTdgrKey(strNewTdgrList[i]);
		
		rKeyList.Add(TdgrK);
		rDataList.Add(TdgrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdgr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMTendonDefDlg::OnCmdDownButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;
		
	CArray<BOOL, BOOL> aSelList;
	CStringArray       aSelString;
	CArray<CString, CString &> strTdgrList; 
	CArray<CString, CString &> strNewTdgrist; 
	pAttrCtrl->GetTdgrList(strTdgrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE; aSelString.Add(strTdgrList[SelIPoses[i]]); }
	
	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[nCount-1]==nTotal-1);
	BOOL bFirstGrup = TRUE;
	
	strNewTdgrist.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) strNewTdgrist[i] = _T("");

	strNewTdgrist.SetAt(nTotal-1, strTdgrList[nTotal-1]);
	for (int i=nTotal-2; i>=0; i--)
	{
		bSel = aSelList[i];
		
		if (!bSel && bFirstGrup) bFirstGrup=FALSE;
		
		if (bFirstGrup)
		{
			if (!bFirst && bSel)
			{
				if (strNewTdgrist[i+1] != _T(""))
				{
					CString temp = strNewTdgrist[i+1];
					strNewTdgrist.SetAt(i, temp);
				}
				strNewTdgrist.SetAt(i+1, strTdgrList[i]);
			}
			else
			{
				strNewTdgrist.SetAt(i, strTdgrList[i]);
			}
		}
		else
		{
			if (bSel)
			{
				if (strNewTdgrist[i+1] != _T(""))
				{
					CString temp = strNewTdgrist[i+1];
					strNewTdgrist.SetAt(i, temp);
				}
				strNewTdgrist.SetAt(i+1, strTdgrList[i]);
			}
			else
			{
				strNewTdgrist.SetAt(i, strTdgrList[i]);
			}
		}
	}
	
	CArray<T_TDGR_K, T_TDGR_K> rKeyList;
	CArray<T_TDGR_D, T_TDGR_D&> rDataList;  
	T_TDGR_K  TdgrK;
	T_TDGR_D  TdgrD;
	for (int i=0; i<nTotal; i++)
	{
		TdgrD.Initialize();
		if (!pAttrCtrl->GetTdgr(strNewTdgrist[i], TdgrD)) continue;
		if (TdgrD.nGroupId == i+1) continue;

		TdgrD.nGroupId = i+1;
		TdgrK = pAttrCtrl->GetTdgrKey(strNewTdgrist[i]);
		
		rKeyList.Add(TdgrK);
		rDataList.Add(TdgrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdgr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMTendonDefDlg::UpdateSelectMark(CStringArray& aSelSZ)
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
void CCMTendonDefDlg::UpdateListData()
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
	
	CArray<T_TDGR_K, T_TDGR_K> rKeyList;
	CArray<T_TDGR_D, T_TDGR_D&> rDataList;  
	T_TDGR_K  TdgrK;
	T_TDGR_D  TdgrD;
	int nTotal = m_wndGrupNameList.GetItemCount();

	for (int i=0; i<nTotal; i++)
	{
		TdgrD.Initialize();
		szList = m_wndGrupNameList.GetItemText(i, 0);

		if (!pAttrCtrl->GetTdgr(szList, TdgrD)) continue;
		if (TdgrD.nGroupId == i+1) continue;

		TdgrD.nGroupId = i+1;
		TdgrK = pAttrCtrl->GetTdgrKey(szList);
		
		rKeyList.Add(TdgrK);
		rDataList.Add(TdgrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyTdgr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMTendonDefDlg::OnSelchangeCmdGroupNameList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCMTendonDefDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CCMTendonDefDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdgrCommand = FALSE;	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_TDGR_ADD):
		case(UR_TDGR_DEL):
		case(UR_TDGR_MFD): 
		case(UR_TDGR_MFS):
				bIsTdgrCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	if(bIsTdgrCommand) 
		UpdateTendonList();
}