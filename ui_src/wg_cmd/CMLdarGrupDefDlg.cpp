// CMBndrGrupDefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLdarGrupDefDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
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
// CCMLdarGrupDefDlg dialog

CCMLdarGrupDefDlg::CCMLdarGrupDefDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMLdarGrupDefDlg::IDD, pParent)
{
	m_strGroupName = _T("");
	m_strSuffix = _T("");
}


void CCMLdarGrupDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_LDAR_GROUP_EDIT, m_wndGroupName);
	DDX_Control(pDX, IDC_CMD_GROUP_NAME_LIST, m_wndGrupNameList);
	DDX_Text(pDX, IDC_CMD_LDAR_GROUP_EDIT, m_strGroupName);
	DDX_Text(pDX, IDC_CMD_SUFFIX_EDIT, m_strSuffix);
	DDX_Control(pDX, IDC_CMD_GRP_SORT_ASC_BTN, m_btnSortAsc);
	DDX_Control(pDX, IDC_CMD_GRP_SORT_DSC_BTN, m_btnSortDsc);
	DDX_Control(pDX, IDC_CMD_GRP_UP_BTN, m_btnUp);
	DDX_Control(pDX, IDC_CMD_GRP_DW_BTN, m_btnDown);
}

BEGIN_MESSAGE_MAP(CCMLdarGrupDefDlg, CDialog)
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLdarGrupDefDlg message handlers
BOOL CCMLdarGrupDefDlg::OnInitDialog() 
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

	UpdateLagrList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMLdarGrupDefDlg::UpdateLagrList()
{
	CArray<CString,CString&> strLagrList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetLagrList(strLagrList);

	m_wndGrupNameList.DeleteAllItems();
	int nLagr = strLagrList.GetSize();
	for(int i = 0 ; i < nLagr ; i++)
	{    
		m_wndGrupNameList.InsertItem(i, strLagrList[i]);  
		m_wndGrupNameList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER); 
	}

	m_wndGrupNameList.SetFocus();
}


void CCMLdarGrupDefDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMLdarGrupDefDlg::GenBysuffixButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CArray<int,int> SuffixList;
	CMap<int,int,int,int> SuffixMap;
	SuffixMap.InitHashTable(500);
	CStringArray    GroupNameArray;
	int TempInt;
	T_LAGR_D LagrD;
	CArray<T_LAGR_D,T_LAGR_D&> LagrDs;

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
				LagrD.GroupName = TempStr;
				LagrDs.Add(LagrD);
				
			}
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->AddLagr(LagrDs);
	}

}

void CCMLdarGrupDefDlg::OnCmdAddButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_strSuffix.IsEmpty())
	{
		T_LAGR_D LagrD;
		LagrD.GroupName = m_strGroupName;
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddLagr(LagrD))
		{

		}
	}
	else
	{
		this->GenBysuffixButton();
	}
}


void CCMLdarGrupDefDlg::OnCmdDeleteButton() 
{
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	CArray<T_LAGR_K,T_LAGR_K> LagrKeys;
	CString TStr;
	T_LAGR_K LagrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		LagrK = CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetLagrKey(TStr);
		if(LagrK)
			LagrKeys.Add(LagrK);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->DelLagr(LagrKeys);
}

void CCMLdarGrupDefDlg::OnCmdInvDeleteButton() 
{
	// TODO: Add your control notification handler code here
	CMap  <T_LAGR_K,T_LAGR_K,T_LAGR_K,T_LAGR_K> SelectedLagrKeys;
	SelectedLagrKeys.InitHashTable( 500 );

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses); 
	CArray<T_LAGR_K,T_LAGR_K> LagrKeys;
	CString TStr;
	T_LAGR_K LagrK;
	
	for( int i = 0 ; i < nCount ; i++)
	{
		TStr = m_wndGrupNameList.GetItemText(SelIPoses[i], 0);
		LagrK = CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetLagrKey(TStr);
		if(LagrK)
		{
			if(!SelectedLagrKeys.Lookup(LagrK,LagrK))
				SelectedLagrKeys.SetAt(LagrK,LagrK);
		}
	}
	
	CArray<T_LAGR_K,T_LAGR_K> rKeyList;
	CArray<T_LAGR_K,T_LAGR_K> TargetKeys;
	CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetLagrKeyList(rKeyList);

	int nKeys = rKeyList.GetSize();
	
	for(int  i = 0 ; i < nKeys; i++)
	{
		if(!SelectedLagrKeys.Lookup(rKeyList[i], LagrK))
			TargetKeys.Add(rKeyList[i]);
	}

	CDBDoc::GetDocPoint()->m_pDataCtrl->DelLagr(TargetKeys);
}



void CCMLdarGrupDefDlg::OnCmdModifyButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	T_LAGR_D TLagrD;
	T_LAGR_K LagrK;

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
		BOOL RetVal = CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetLagr(Str, TLagrD);
		LagrK       = CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetLagrKey(Str);

		if(LagrK && RetVal)
		{
			CArray<T_LAGR_K,T_LAGR_K>  LagrKeys;
			CArray<T_LAGR_D,T_LAGR_D&> LagrDatas;
			LagrKeys.Add(LagrK);
			TLagrD.GroupName = this->m_strGroupName;
			LagrDatas.Add(TLagrD);
			if(CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLagr(LagrKeys, LagrDatas))
			{

			}
		}
	}
}

void CCMLdarGrupDefDlg::OnCmdASCButton()
{
	CAttrCtrl2* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl2;
	ASSERT(pAttrCtrl);

	CArray<CString, CString &> strLagrList;
	CArray<T_LAGR_K, T_LAGR_K> rKeyList;
	CArray<T_LAGR_D, T_LAGR_D&> rDataList;  
	pAttrCtrl->GetLagrList(strLagrList);
	
	int nCount = strLagrList.GetSize();  
	if (nCount>0)
	{
		qsort(strLagrList.GetData(), nCount, sizeof(CString), CCompFunc::stringAsc);

		T_LAGR_K  LagrK;
		T_LAGR_D  LagrD;
		for (int i=0; i<nCount; i++)
		{
			LagrD.Initialize();
			if (!pAttrCtrl->GetLagr(strLagrList[i], LagrD)) continue;
			if (LagrD.nGroupId == i+1) continue;

			LagrD.nGroupId = i+1;
			LagrK = pAttrCtrl->GetLagrKey(strLagrList[i]);

			rKeyList.Add(LagrK);
			rDataList.Add(LagrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLagr(rKeyList, rDataList);
	}  
}

void CCMLdarGrupDefDlg::OnCmdDESCButton()
{
	CAttrCtrl2* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl2;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<CString, CString &> strLagrList;
	CArray<T_LAGR_K, T_LAGR_K> rKeyList;
	CArray<T_LAGR_D, T_LAGR_D&> rDataList;  
	pAttrCtrl->GetLagrList(strLagrList);
	
	int nCount = strLagrList.GetSize();  
	if (nCount>0)
	{
		qsort(strLagrList.GetData(), nCount, sizeof(CString), CCompFunc::stringDesc);
		
		T_LAGR_K  LagrK;
		T_LAGR_D  LagrD;
		for (int i=0; i<nCount; i++)
		{
			LagrD.Initialize();
			if (!pAttrCtrl->GetLagr(strLagrList[i], LagrD)) continue;
			if (LagrD.nGroupId == i+1) continue;

			LagrD.nGroupId = i+1;
			LagrK = pAttrCtrl->GetLagrKey(strLagrList[i]);
			
			rKeyList.Add(LagrK);
			rDataList.Add(LagrD);
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLagr(rKeyList, rDataList);
	}
}

void CCMLdarGrupDefDlg::OnCmdUpButton()
{
	CAttrCtrl2* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl2;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;

	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;

	CArray<BOOL, BOOL> aSelList; 
	CStringArray       aSelString;
	CArray<CString, CString &> strLagrList; 
	CArray<CString, CString &> strNewLagrList; 
	pAttrCtrl->GetLagrList(strLagrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE; aSelString.Add(strLagrList[SelIPoses[i]]); }

	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[0]==0);
	BOOL bFirstGrup = TRUE;

	strNewLagrList.SetSize(nTotal);
	strNewLagrList.InsertAt(0, strLagrList[0]);
	for (int i=1; i<nTotal; i++)
	{
		bSel = aSelList[i];

		if (!bSel && bFirstGrup) bFirstGrup=FALSE;

		if (bFirstGrup)
		{
			if (!bFirst && bSel) strNewLagrList.InsertAt(i-1, strLagrList[i]);      
			else                 strNewLagrList.InsertAt(i, strLagrList[i]);
		}
		else
		{
			if (bSel) strNewLagrList.InsertAt(i-1, strLagrList[i]);
			else      strNewLagrList.InsertAt(i, strLagrList[i]);
		}
	}

	CArray<T_LAGR_K, T_LAGR_K> rKeyList;
	CArray<T_LAGR_D, T_LAGR_D&> rDataList;  
	T_LAGR_K  LagrK;
	T_LAGR_D  LagrD;
	for (int i=0; i<nTotal; i++)
	{
		LagrD.Initialize();
		if (!pAttrCtrl->GetLagr(strNewLagrList[i], LagrD)) continue;
		if (LagrD.nGroupId == i+1) continue;

		LagrD.nGroupId = i+1;
		LagrK = pAttrCtrl->GetLagrKey(strNewLagrList[i]);
		
		rKeyList.Add(LagrK);
		rDataList.Add(LagrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLagr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMLdarGrupDefDlg::OnCmdDownButton()
{
	CAttrCtrl2* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl2;
	ASSERT(pAttrCtrl);
	if (pAttrCtrl==NULL) return;
	
	CArray<int,int> SelIPoses;
	int nCount = m_wndGrupNameList.GetSelItems(SelIPoses);
	if (nCount<1) return;
		
	CArray<BOOL, BOOL> aSelList;
	CStringArray       aSelString;
	CArray<CString, CString &> strLagrList; 
	CArray<CString, CString &> strNewLagrList; 
	pAttrCtrl->GetLagrList(strLagrList);

	int nTotal = m_wndGrupNameList.GetItemCount();
	aSelList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) { aSelList[i]=FALSE; }
	for (int i=0; i<nCount; i++) { aSelList[SelIPoses[i]]=TRUE;  aSelString.Add(strLagrList[SelIPoses[i]]); }
	
	BOOL bSel = FALSE;
	BOOL bFirst = (SelIPoses[nCount-1]==nTotal-1);
	BOOL bFirstGrup = TRUE;
	
	strNewLagrList.SetSize(nTotal);
	for (int i=0; i<nTotal; i++) strNewLagrList[i] = _T("");

	strNewLagrList.SetAt(nTotal-1, strLagrList[nTotal-1]);
	for (int i=nTotal-2; i>=0; i--)
	{
		bSel = aSelList[i];
		
		if (!bSel && bFirstGrup) bFirstGrup=FALSE;
		
		if (bFirstGrup)
		{
			if (!bFirst && bSel)
			{
				if (strNewLagrList[i+1] != _T(""))
				{
					CString temp = strNewLagrList[i+1];
					strNewLagrList.SetAt(i, temp);
				}
				strNewLagrList.SetAt(i+1, strLagrList[i]);
			}
			else
			{
				strNewLagrList.SetAt(i, strLagrList[i]);
			}
		}
		else
		{
			if (bSel)
			{
				if (strNewLagrList[i+1] != _T(""))
				{
					CString temp = strNewLagrList[i+1];
					strNewLagrList.SetAt(i, temp);
				}
				strNewLagrList.SetAt(i+1, strLagrList[i]);
			}
			else
			{
				strNewLagrList.SetAt(i, strLagrList[i]);
			}
		}
	}
	
	CArray<T_LAGR_K, T_LAGR_K> rKeyList;
	CArray<T_LAGR_D, T_LAGR_D&> rDataList;  
	T_LAGR_K  LagrK;
	T_LAGR_D  LagrD;
	for (int i=0; i<nTotal; i++)
	{
		LagrD.Initialize();
		if (!pAttrCtrl->GetLagr(strNewLagrList[i], LagrD)) continue;
		if (LagrD.nGroupId == i+1) continue;

		LagrD.nGroupId = i+1;
		LagrK = pAttrCtrl->GetLagrKey(strNewLagrList[i]);
		
		rKeyList.Add(LagrK);
		rDataList.Add(LagrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLagr(rKeyList, rDataList);
	
	UpdateSelectMark(aSelString);
}

void CCMLdarGrupDefDlg::UpdateSelectMark(CStringArray& aSelSZ)
{
	int nSize = aSelSZ.GetSize();
	if (nSize<1) return;
	
	for (int i=0; i<m_wndGrupNameList.GetItemCount(); i++)
	{
		CString szList = _T("");
		szList = m_wndGrupNameList.GetItemText(i, 0);
		int j = 0;
		for ( j=0; j<nSize; j++)
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
void CCMLdarGrupDefDlg::UpdateListData()
{
	CAttrCtrl2* pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl2;
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
	
	CArray<T_LAGR_K, T_LAGR_K> rKeyList;
	CArray<T_LAGR_D, T_LAGR_D&> rDataList;  
	T_LAGR_K  LagrK;
	T_LAGR_D  LagrD;
	int nTotal = m_wndGrupNameList.GetItemCount();

	for (int i=0; i<nTotal; i++)
	{
		LagrD.Initialize();
		szList = m_wndGrupNameList.GetItemText(i, 0);

		if (!pAttrCtrl->GetLagr(szList, LagrD)) continue;
		if (LagrD.nGroupId == i+1) continue;

		LagrD.nGroupId = i+1;
		LagrK = pAttrCtrl->GetLagrKey(szList);
		
		rKeyList.Add(LagrK);
		rDataList.Add(LagrD);
	}
	CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyLagr(rKeyList, rDataList);
	UpdateSelectMark(aSelString);
}

void CCMLdarGrupDefDlg::OnSelchangeCmdGroupNameList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CCMLdarGrupDefDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CCMLdarGrupDefDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsLagrCommand = FALSE;	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_LAGR_ADD):
		case(UR_LAGR_DEL):
		case(UR_LAGR_MFD): 
		case(UR_LAGR_MFS):
				bIsLagrCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	if(bIsLagrCommand) 
		UpdateLagrList();
}

