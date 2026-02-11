// CMBndrGrupDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMBndrGrupDefDlg.h"

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

#define CDialog CCMDlgBase
/////////////////////////////////////////////////////////////////////////////
// CCMBndrGrupDefDlg dialog


CCMBndrGrupDefDlg::CCMBndrGrupDefDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMBndrGrupDefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMBndrGrupDefDlg)
	m_strGroupName = _T("");
	m_strSuffix = _T("");
	//}}AFX_DATA_INIT
}


void CCMBndrGrupDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMBndrGrupDefDlg)
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

BEGIN_MESSAGE_MAP(CCMBndrGrupDefDlg, CDialog)

	//{{AFX_MSG_MAP(CCMBndrGrupDefDlg)
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
// CCMBndrGrupDefDlg message handlers
BOOL CCMBndrGrupDefDlg::OnInitDialog() 
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
	m_wndGrupNameList.SetBndrType();

	UpdateBngrList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMBndrGrupDefDlg::UpdateBngrList()
{
	CArray<CString,CString&> strBngrList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrList(strBngrList);

	m_wndGrupNameList.DeleteAllItems();
	int nBngr = strBngrList.GetSize();
	for(int i = 0 ; i < nBngr ; i++)
	{    
		m_wndGrupNameList.InsertItem(i, strBngrList[i]);  
		m_wndGrupNameList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER); 
	}

	m_wndGrupNameList.SetFocus();
}


void CCMBndrGrupDefDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMBndrGrupDefDlg::GenBysuffixButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CArray<int,int> SuffixList;
	CMap<int,int,int,int> SuffixMap;
	SuffixMap.InitHashTable(500);
	CStringArray    GroupNameArray;
	int TempInt;
	T_BNGR_D BngrD;
	CArray<T_BNGR_D,T_BNGR_D&> BngrDs;

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
				BngrD.GroupName = TempStr;
				BngrDs.Add(BngrD);
				
				//CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr
			}
		}
		//CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr(BngrD);
		CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr(BngrDs);
	}

}

void CCMBndrGrupDefDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strSuffix.IsEmpty())
	{
		T_BNGR_D BngrD;
		BngrD.GroupName = m_strGroupName;
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddBngr(BngrD))
		{

		}
	}
	else
	{
		this->GenBysuffixButton();
	}
}


void CCMBndrGrupDefDlg::OnCmdDeleteButton() 
{
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	CArray<T_BNGR_K,T_BNGR_K> BngrKeys;
	CString TStr;
	T_BNGR_K BngrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		BngrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(TStr);
		if(BngrK)
			BngrKeys.Add(BngrK);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->DelBngr(BngrKeys);
}

void CCMBndrGrupDefDlg::OnCmdInvDeleteButton() 
{
	// TODO: Add your control notification handler code here
	CMap  <T_BNGR_K,T_BNGR_K,T_BNGR_K,T_BNGR_K> SelectedBngrKeys;
	SelectedBngrKeys.InitHashTable( 500 );

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses); 
	CArray<T_BNGR_K,T_BNGR_K> BngrKeys;
	CString TStr;
	T_BNGR_K BngrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		BngrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(TStr);
		if(BngrK)
		{
			if(!SelectedBngrKeys.Lookup(BngrK,BngrK))
				SelectedBngrKeys.SetAt(BngrK,BngrK);
		}
	}
	
	CArray<T_BNGR_K,T_BNGR_K> rKeyList;
	CArray<T_BNGR_K,T_BNGR_K> TargetKeys;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKeyList(rKeyList);

	int nKeys = rKeyList.GetSize();
	
	for( int i = 0 ; i < nKeys; i++)
	{
		if(!SelectedBngrKeys.Lookup(rKeyList[i],BngrK))
			TargetKeys.Add(rKeyList[i]);
	}

	CDBDoc::GetDocPoint()->m_pDataCtrl->DelBngr(TargetKeys);
}



void CCMBndrGrupDefDlg::OnCmdModifyButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	T_BNGR_D TBngrD;
	T_BNGR_K BngrK;

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
		BOOL RetVal = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(Str,TBngrD);
		BngrK       = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(Str);

		if(BngrK && RetVal)
		{
			CArray<T_BNGR_K,T_BNGR_K>  BngrKeys;
			CArray<T_BNGR_D,T_BNGR_D&> BngrDatas;
			BngrKeys.Add(BngrK);
			TBngrD.GroupName = this->m_strGroupName;
			BngrDatas.Add(TBngrD);
			if(CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyBngr(BngrKeys,BngrDatas))
			{

			}
		}
	}
}

void CCMBndrGrupDefDlg::OnCmdASCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);

	CArray<CString, CString &> strBngrList;
	CArray<T_BNGR_K, T_BNGR_K> rKeyList;
	CArray<T_BNGR_D, T_BNGR_D&> rDataList;  
	pAttrCtrl->GetBngrList(strBngrList);
	
	int nCount = strBngrList.GetSize();  
	if (nCount>0)
	{
		qsort(strBngrList.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);

		T_BNGR_K  BngrK;
		T_BNGR_D  BngrD;
		for (int i=0; i<nCount; i++)
		{
			BngrD.Initialize();
			if (!pAttrCtrl->GetBngr(strBngrList[i], BngrD)) continue;
			if (BngrD.nGroupId == i+1) continue;

			BngrD.nGroupId = i+1;
			BngrK = pAttrCtrl->GetBngrKey(strBngrList[i]);

			rKeyList.Add(BngrK);
			rDataList.Add(BngrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyBngr(rKeyList, rDataList);
	}  
}

void CCMBndrGrupDefDlg::OnCmdDESCButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<CString, CString &> strBngrList;
	CArray<T_BNGR_K, T_BNGR_K> rKeyList;
	CArray<T_BNGR_D, T_BNGR_D&> rDataList;  
	pAttrCtrl->GetBngrList(strBngrList);
	
	int nCount = strBngrList.GetSize();  
	if (nCount>0)
	{
		qsort(strBngrList.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		
		T_BNGR_K  BngrK;
		T_BNGR_D  BngrD;
		for (int i=0; i<nCount; i++)
		{
			BngrD.Initialize();
			if (!pAttrCtrl->GetBngr(strBngrList[i], BngrD)) continue;
			if (BngrD.nGroupId == i+1) continue;

			BngrD.nGroupId = i+1;
			BngrK = pAttrCtrl->GetBngrKey(strBngrList[i]);
			
			rKeyList.Add(BngrK);
			rDataList.Add(BngrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyBngr(rKeyList, rDataList);
	}
}

void CCMBndrGrupDefDlg::OnCmdUpButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;

	CArray<BOOL, BOOL> aSelList; 
	CStringArray       aSelString;
	CArray<CString, CString &> strBngrList; 
	CArray<CString, CString &> strNewBngrList; 
	pAttrCtrl->GetBngrList(strBngrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE; aSelString.Add(strBngrList[SelIPoses[i]]); }

	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[0]==0);
	BOOL bFirstGrup = TRUE;

	strNewBngrList.SetSize(nTotal);
	strNewBngrList.InsertAt(0, strBngrList[0]);
	for (int i=1; i<nTotal; i++)
	{
		bSel = aSelList[i];

		if (!bSel && bFirstGrup) bFirstGrup=FALSE;

		if (bFirstGrup)
		{
			if (!bFirst && bSel) strNewBngrList.InsertAt(i-1, strBngrList[i]);      
			else                 strNewBngrList.InsertAt(i, strBngrList[i]);
		}
		else
		{
			if (bSel) strNewBngrList.InsertAt(i-1, strBngrList[i]);
			else      strNewBngrList.InsertAt(i, strBngrList[i]);
		}
	}

	CArray<T_BNGR_K, T_BNGR_K> rKeyList;
	CArray<T_BNGR_D, T_BNGR_D&> rDataList;  
	T_BNGR_K  BngrK;
	T_BNGR_D  BngrD;
	for (int i=0; i<nTotal; i++)
	{
		BngrD.Initialize();
		if (!pAttrCtrl->GetBngr(strNewBngrList[i], BngrD)) continue;
		if (BngrD.nGroupId == i+1) continue;

		BngrD.nGroupId = i+1;
		BngrK = pAttrCtrl->GetBngrKey(strNewBngrList[i]);
		
		rKeyList.Add(BngrK);
		rDataList.Add(BngrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyBngr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMBndrGrupDefDlg::OnCmdDownButton()
{
	CAttrCtrl* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;
		
	CArray<BOOL, BOOL> aSelList;
	CStringArray       aSelString;
	CArray<CString, CString &> strBngrList; 
	CArray<CString, CString &> strNewBngrList; 
	pAttrCtrl->GetBngrList(strBngrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE;  aSelString.Add(strBngrList[SelIPoses[i]]); }
	
	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[nCount-1]==nTotal-1);
	BOOL bFirstGrup = TRUE;
	
	strNewBngrList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) strNewBngrList[i] = _T("");

	strNewBngrList.SetAt(nTotal-1, strBngrList[nTotal-1]);
	for (int i=nTotal-2; i>=0; i--)
	{
		bSel = aSelList[i];
		
		if (!bSel && bFirstGrup) bFirstGrup=FALSE;
		
		if (bFirstGrup)
		{
			if (!bFirst && bSel)
			{
				if (strNewBngrList[i+1] != _T(""))
				{
					CString temp = strNewBngrList[i+1];
					strNewBngrList.SetAt(i, temp);
				}
				strNewBngrList.SetAt(i+1, strBngrList[i]);
			}
			else
			{
				strNewBngrList.SetAt(i, strBngrList[i]);
			}
		}
		else
		{
			if (bSel)
			{
				if (strNewBngrList[i+1] != _T(""))
				{
					CString temp = strNewBngrList[i+1];
					strNewBngrList.SetAt(i, temp);
				}
				strNewBngrList.SetAt(i+1, strBngrList[i]);
			}
			else
			{
				strNewBngrList.SetAt(i, strBngrList[i]);
			}
		}
	}
	
	CArray<T_BNGR_K, T_BNGR_K> rKeyList;
	CArray<T_BNGR_D, T_BNGR_D&> rDataList;  
	T_BNGR_K  BngrK;
	T_BNGR_D  BngrD;
	for (int i=0; i<nTotal; i++)
	{
		BngrD.Initialize();
		if (!pAttrCtrl->GetBngr(strNewBngrList[i], BngrD)) continue;
		if (BngrD.nGroupId == i+1) continue;

		BngrD.nGroupId = i+1;
		BngrK = pAttrCtrl->GetBngrKey(strNewBngrList[i]);
		
		rKeyList.Add(BngrK);
		rDataList.Add(BngrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyBngr(rKeyList, rDataList);
	
	UpdateSelectMark(aSelString);
}

void CCMBndrGrupDefDlg::UpdateSelectMark(CStringArray& aSelSZ)
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
void CCMBndrGrupDefDlg::UpdateListData()
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
	
	CArray<T_BNGR_K, T_BNGR_K> rKeyList;
	CArray<T_BNGR_D, T_BNGR_D&> rDataList;  
	T_BNGR_K  BngrK;
	T_BNGR_D  BngrD;
	int nTotal = m_wndGrupNameList.GetItemCount();

	for (int i=0; i<nTotal; i++)
	{
		BngrD.Initialize();
		szList = m_wndGrupNameList.GetItemText(i, 0);

		if (!pAttrCtrl->GetBngr(szList, BngrD)) continue;
		if (BngrD.nGroupId == i+1) continue;

		BngrD.nGroupId = i+1;
		BngrK = pAttrCtrl->GetBngrKey(szList);
		
		rKeyList.Add(BngrK);
		rDataList.Add(BngrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyBngr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMBndrGrupDefDlg::OnSelchangeCmdGroupNameList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCMBndrGrupDefDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CCMBndrGrupDefDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
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
		UpdateBngrList();
}

