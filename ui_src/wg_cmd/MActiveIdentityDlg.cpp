// MActiveIdentityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MActiveIdentityDlg.h"

#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_common\ActiveObserver.h"  // for Active Observer
#include "..\wg_base\AutomationDataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMActiveIdentityDlg dialog


CMActiveIdentityDlg::CMActiveIdentityDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CMActiveIdentityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMActiveIdentityDlg)
	m_nCategory = 0;
	m_bIsActiveLayer = FALSE;
	m_nStoryOpt = 3;
	//}}AFX_DATA_INIT

	arStoryOption.Add(IDC_STORY_FLOOR);
	arStoryOption.Add(IDC_STORY_ABOVE);
	arStoryOption.Add(IDC_STORY_BELOW);
	arStoryOption.Add(IDC_STORY_Both);
}


void CMActiveIdentityDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMActiveIdentityDlg)
	DDX_Control(pDX, IDC_CMD_SD_ITEM_LIST, m_lbItemList);
	DDX_Radio(pDX, IDC_CMD_CUR_UCSPLANE, m_nCategory);
	DDX_Check(pDX, IDC_CMD_ACTIVE_ONLY, m_bIsActiveLayer);
	DDX_Radio(pDX, IDC_STORY_FLOOR, m_nStoryOpt);
	//}}AFX_DATA_MAP
}

#define CDialog CCMDlgBase
BEGIN_MESSAGE_MAP(CMActiveIdentityDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CMActiveIdentityDlg)
	ON_BN_CLICKED(IDC_CMD_CUR_UCSPLANE, OnCmdCurUcsplane)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_ACTIVEALL_BUTTON, OnCmdActiveallButton)
	ON_BN_CLICKED(IDC_CMD_ACTIVE_ACCUM, OnCmdActiveAccum)
	ON_BN_CLICKED(IDC_CMD_INACTIVE_ACCUM, OnCmdInactiveAccum)
	ON_BN_CLICKED(IDC_CMD_BNGR, OnCmdBngr)
	ON_BN_CLICKED(IDC_CMD_LDGR, OnCmdLdgr)
	ON_BN_CLICKED(IDC_CMD_GROUP, OnCmdCurUcsplane)
	ON_BN_CLICKED(IDC_CMD_NAMED_PLANE, OnCmdCurUcsplane)
	ON_BN_CLICKED(IDC_CMD_STORY, OnCmdCurUcsplane)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_CMD_MEMBER, OnCmdMember)
	ON_BN_CLICKED(IDC_CMD_LOADINGAREA_GRP, OnCmdLagr)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMActiveIdentityDlg message handlers

BOOL CMActiveIdentityDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here
//  m_nStoryOpt = 3;
 // UpdateData(FALSE);
	CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);

	// MNET:8108-HSSHIM-20140716
#if !defined(_CIVIL)
	GetDlgItem(IDC_CMD_LOADINGAREA_GRP)->GetWindowRect(m_rectLoadingPlane);
	GetDlgItem(IDC_CMD_MEMBER)->GetWindowRect(m_rectMember);
	ScreenToClient(m_rectLoadingPlane);
	ScreenToClient(m_rectMember);

	BOOL bShow = CDBDoc::IsEnableWindPressure();
	ShowHideLoadingAreaPlane(bShow);
#endif

	AUTOMATION_SET_TIMER_CPP()

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


AUTOMATION_TIMER_CPP(CMActiveIdentityDlg)

/*********************************************************************
	 *
	 *
 *****
	***
	 */
void CMActiveIdentityDlg::MakeListNone()
{
	m_lbItemList.ResetContent();
}

void CMActiveIdentityDlg::MakeListGroup()
{
	m_lbItemList.ResetContent();
	T_GRUP_K Key;
	T_GRUP_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_GRUP_K, T_GRUP_K> aKey;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = aKey[i];
	  CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrup(Key,Data);
		csItem.Format(_T("%s"), Data.GroupName);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CMActiveIdentityDlg::MakeListBngr()
{
	T_BNGR_K Key;
	m_lbItemList.ResetContent();
	CArray<CString,CString&> strBngrList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrList(strBngrList);
	int nSize = strBngrList.GetSize();

	CString csItem;
	int nIndex = 0;
	
	for (int i = 0 ; i < nSize ; i++)
	{
		Key = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(strBngrList[i]);
		csItem.Format(_T("%s"), strBngrList[i]);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}  
}

void CMActiveIdentityDlg::MakeListLdgr()
{
	T_LDGR_K Key;
	m_lbItemList.ResetContent();
	CArray<CString,CString&> strLdgrList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrList(strLdgrList);
	int nSize = strLdgrList.GetSize();

	CString csItem;
	int nIndex = 0;
	
	for (int i = 0 ; i < nSize ; i++)
	{               
		Key = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKey(strLdgrList[i]);
		csItem.Format(_T("%s"), strLdgrList[i]);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CMActiveIdentityDlg::MakeListNpln()
{
	m_lbItemList.ResetContent();
	T_NPLN_K Key;
	T_NPLN_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_NPLN_K, T_NPLN_K> aKey;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNplnKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = aKey[i];
	  CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNpln(Key,Data);
		csItem.Format(_T("%s"), Data.PlaneName);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CMActiveIdentityDlg::MakeListStory()
{
	m_lbItemList.ResetContent();
	T_STOR_K Key;
	T_STOR_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_STOR_K, T_STOR_K> aKey;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStorKeyList(aKey);
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		Key = aKey[i];
	  CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStor(Key,Data);
		csItem.Format(_T("%s"), Data.StoryName);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CMActiveIdentityDlg::MakeListLagr()
{
	T_LAGR_D LagrD;
	int i, nIndex, nSize;

	CArray<T_LAGR_K, T_LAGR_K> rKeyList;

	m_lbItemList.ResetContent();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->GetLagrKeyList(rKeyList);
	nSize = rKeyList.GetSize();  
	
	for (i = 0 ; i < nSize ; i++)
	{
		if(pDoc->m_pAttrCtrl2->GetLagr(rKeyList[i], LagrD))
		{
			nIndex = m_lbItemList.AddString(LagrD.GroupName);
			m_lbItemList.SetItemData(nIndex, (DWORD)rKeyList[i]);
		}
		else ASSERT(0);
	} 
}

void CMActiveIdentityDlg::MakeListMember()
{
	/*
	m_lbItemList.ResetContent();
	T_MEMB_K Key;
	T_MEMB_D Data;
	
	CString csItem;
	int nIndex = 0;
	CArray<T_MEMB_K, T_MEMB_K> aKey;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMembKeyList(aKey);
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		Key = aKey[i];
	  CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMemb(Key,Data);
		if(Data.aElemList.GetSize())
		{
			csItem.Format(_T("%d"), Data.aElemList[0]);
			nIndex = m_lbItemList.AddString(csItem);
			m_lbItemList.SetItemData(nIndex, (DWORD)Key);
		}
	}
	*/

	m_lbItemList.ResetContent();

	CString csItem;
	T_MEMB_D data;
	int nIndex = 0;
	int nIndexMemb;
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMemb();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount;  

	if(nItemCount)
	{   
		T_MEMB_K Key;
		T_MEMB_D Data;
		T_ELEM_K KeyElem;

		CArray<T_MEMB_K, T_MEMB_K> aKey;
		int* KeyBuf = new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMemb();
		while(pos)
		{
			m_pDoc->m_pAttrCtrl->GetNextMemb(pos, Key, Data);
		  KeyBuf[nCount++] = Data.aElemList[0];
		}  
		qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);
		for(nCount = 0; nCount < nItemCount; nCount++)
		{    
			KeyElem = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, Key);
			m_pDoc->m_pAttrCtrl->GetMemb(Key,Data);      
			nIndexMemb = Data.aElemList[0];
			CString strElemList = _T("");
			CString csItem;      
			int i = 0;
			for(i = 0; i < Data.aElemList.GetSize() - 1; i++)
			{
				csItem.Format(_T("%d"), Data.aElemList[i]);
				strElemList += csItem + _T(", ");
			}
			if(Data.aElemList.GetSize() > 0)
			{
				csItem.Format(_T("%d"), Data.aElemList[i]);
				strElemList += csItem;
			}
			csItem.Format(_T("%-5d: %s"), nIndexMemb, strElemList);
			nIndex = m_lbItemList.AddString(csItem);
			m_lbItemList.SetItemData(nIndex, (DWORD)Key);
		}
		delete []KeyBuf;
	}
}

// radio button event
void CMActiveIdentityDlg::ChangeCategory()
{
	int nCategory;
	nCategory = GetCurCategory();
	switch(nCategory)
	{
	case CAT_NONE:      
		MakeListNone();     
		CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
		break;
	case CAT_ELEM_TYPE: //keListElemType(); break;
	case CAT_MATL:      //keListMatl();     break;
	case CAT_SECT:      //keListSect();     break;
	case CAT_THIK:      //keListThik();     break;
	case CAT_CONS:      //keListCons();     break;
	case CAT_FRLS:      //keListFrls();     break;
	case CAT_WALL:      //keListWall();     break;
	case CAT_NAPL:  
		MakeListNpln();    
		CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
		break;
	case CAT_STORY:   
		MakeListStory();    
		CDlgUtil::CtrlShowHide(this, arStoryOption, TRUE);
		break;
	case CAT_GROUP:    
		MakeListGroup(); 
		CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
		break;
	case CAT_LOAD_GROUP:
		MakeListLdgr(); 
		CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
		break;
	case CAT_BNDR_GROUP: 
		MakeListBngr(); 
		CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
		break;
	case CAT_MEMB:
		MakeListMember();
		CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
		break;
	case CAT_LAGR:
		MakeListLagr();
		CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
		break;
	default:
		ASSERT(0);
		break;
	}
}
// radio ... ID...s
int CMActiveIdentityDlg::GetCurCategory()
{
	/*
	CString sCategory;
	m_cboCategory.GetWindowText(sCategory);
	if (sCategory == CM_SD_ELEM_TYPE) return CAT_ELEM_TYPE;
	if (sCategory == CM_SD_MATERIAL)  return CAT_MATL;
	if (sCategory == CM_SD_SECTION)   return CAT_SECT;
	if (sCategory == CM_SD_THICKNESS) return CAT_THIK;
	if (sCategory == CM_SD_NAMED_P)   return CAT_NAPL;
	if (sCategory == CM_SD_STORY)     return CAT_STORY;
	if (sCategory == CM_SD_GROUP)     return CAT_GROUP;
	if (sCategory == CM_SD_CONS)      return CAT_CONS;
	if (sCategory == CM_SD_FRLS)      return CAT_FRLS;
	if (sCategory == CM_SD_WALL)      return CAT_WALL;
	
	*/
	UpdateData(TRUE);
	//switch(m_nCategory)
	switch(GetCategory())
	{
#ifdef _TOWER
	case 0:  // Current Ucs Plane
		return CAT_NONE;
	case 1:  // Named Plane 
		return CAT_NAPL;
	case 2:  // Story
		return CAT_BNDR_GROUP;
	case 3:  // Group
		return CAT_GROUP;
	case 4:
		return CAT_LOAD_GROUP;
	case 5:
		ASSERT(FALSE);
	default:
		break;
#elif _CIVIL
	case 0:  // Current Ucs Plane
		return CAT_NONE;
	case 1:  // Named Plane 
		return CAT_NAPL;
	//case 2:  // Story
	//  return CAT_STORY;
	case 2:
		return CAT_BNDR_GROUP;
	case 3:  // Group
		return CAT_GROUP;
	case 4:
		return CAT_LOAD_GROUP;
	case 5:
		return CAT_MEMB;
	default:
		break;
#else 
	case 0:  // Current Ucs Plane
		return CAT_NONE;
	case 1:  // Named Plane 
		return CAT_NAPL;
	case 2:  // Story
		return CAT_STORY;
	case 3:  // Group
		return CAT_GROUP;
	case 4:
		return CAT_BNDR_GROUP;
	case 5:
		return CAT_LOAD_GROUP;
	case 6:
		return CAT_MEMB;
	case 7:
		return CAT_LAGR;
	default:
		break;
#endif
	}
	return CAT_NONE;
}

void CMActiveIdentityDlg::GetQueryCondition(CQueryCondition& QCond)
{
	int nCategory = GetCurCategory();
	QCond.m_nCategory = nCategory;
	QCond.m_nOperation = QC_OP_OR;
	int nCount = m_lbItemList.GetSelCount();
	if (nCount == 0) return;
	
	CString sValue;
	DWORD nValue;
	CArray<int,int> aIndex;  
	aIndex.SetSize(nCount);
	m_lbItemList.GetSelItems(nCount, aIndex.GetData());
	for (int i = 0; i < nCount; i++)
	{
		switch(nCategory)
		{
		case CAT_STORY:
			UpdateData(TRUE);
			QCond.m_nStoryOption = m_nStoryOpt;   // Story Option
		case CAT_MEMB:
		case CAT_ELEM_TYPE:
		case CAT_MATL:
		case CAT_SECT:
		case CAT_THIK:
		case CAT_NAPL:
		case CAT_GROUP:
		case CAT_CONS:
		case CAT_FRLS:
		case CAT_WALL:
		case CAT_LOAD_GROUP:
		case CAT_BNDR_GROUP:
		case CAT_LAGR:
			nValue = m_lbItemList.GetItemData(aIndex[i]);
			sValue.Format(_T("%ld"), nValue);
			QCond.m_aValue.Add(sValue);
			break;
		}
	}
	return;
}

void CMActiveIdentityDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CMActiveIdentityDlg::UpdateBuffer()
{
	BOOL bMatlChanged = FALSE;
	BOOL bSectChanged = FALSE;
	BOOL bThikChanged = FALSE;
	BOOL bStorChanged = FALSE;
	BOOL bNplnChanged = FALSE;
	BOOL bGrupChanged = FALSE;
	BOOL bBngrChanged = FALSE;
	BOOL bLdgrChanged = FALSE;
	BOOL bMembChanged = FALSE;
	BOOL bLagrChanged = FALSE;

	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MATL_ADD): case(UR_MATL_DEL): case(UR_MATL_MFD): case(UR_MATL_MFS):
			bMatlChanged = TRUE;
			break;
		case(UR_SECT_ADD): case(UR_SECT_DEL): case(UR_SECT_MFD): case(UR_SECT_MFS):
			bSectChanged = TRUE;
			break;
		case(UR_THIK_ADD): case(UR_THIK_DEL): case(UR_THIK_MFD): case(UR_THIK_MFS):
			bThikChanged = TRUE;
			break;
		case(UR_STOR_ADD): case(UR_STOR_DEL): case(UR_STOR_MFD): case(UR_STOR_MFS):
			bStorChanged = TRUE;
			break;
		case(UR_GRUP_ADD): case(UR_GRUP_DEL): case(UR_GRUP_MFD): case(UR_GRUP_MFS):
			bGrupChanged = TRUE;
			break;
		case(UR_NPLN_ADD): case(UR_NPLN_DEL): //case(UR_STOR_MFD): case(UR_STOR_MFS):
			bNplnChanged = TRUE;
			break;
		case(UR_BNGR_ADD): case(UR_BNGR_DEL): case(UR_BNGR_MFD): case(UR_BNGR_MFS):
			bBngrChanged = TRUE;
			break;
		case(UR_LDGR_ADD): case(UR_LDGR_DEL): case(UR_LDGR_MFD): case(UR_LDGR_MFS):
			bLdgrChanged = TRUE;
			break;
		case(UR_MEMB_ADD): case(UR_MEMB_DEL): case(UR_MEMB_MFD): case(UR_MEMB_MFS):
			bMembChanged = TRUE;
			break;
		case(UR_LAGR_ADD): case(UR_LAGR_DEL): case(UR_LAGR_MFD): case(UR_LAGR_MFS):
			bLagrChanged = TRUE;
			break;
		}
	}
	int nCategory = GetCurCategory();
	switch(nCategory)
	{
	case CAT_NONE:
		break;
	case CAT_ELEM_TYPE:
		break;
	case CAT_MATL:
		//if (bMatlChanged) MakeListMatl();
		break;
	case CAT_SECT:
		//if (bSectChanged) MakeListSect();
		break;
	case CAT_THIK:
		//if (bThikChanged) MakeListThik();
		break;
	case CAT_NAPL:
		if (bNplnChanged) MakeListNpln();
		break;
	case CAT_STORY:
		if (bStorChanged) MakeListStory();
		break;
	case CAT_GROUP:
		if (bGrupChanged) MakeListGroup();
		break;
	case CAT_BNDR_GROUP:
		if (bBngrChanged) MakeListBngr();
		break;
	case CAT_LOAD_GROUP:
		if (bLdgrChanged) MakeListLdgr();
		break;
	case CAT_MEMB:
		if (bMembChanged) MakeListMember();
		break;
	case CAT_LAGR:
		if (bLagrChanged) MakeListLagr();
	default:
		break;
	}
}

void CMActiveIdentityDlg::DoActiveOp()
{
	//if (!ValidQueryCondition()) return;
	CArray<UINT, UINT> aNodeKey;
	CArray<UINT, UINT> aElemKey;

	CQueryCtrl QueryCtrl;
	CQueryCondition  QCond;
	GetQueryCondition(QCond);
		
	QueryCtrl.SelectKeyFromNode(aNodeKey, QCond);
	QueryCtrl.SelectKeyFromElem(aElemKey, QCond);

	if(aNodeKey.GetSize() || aElemKey.GetSize())
		I_GENModelBase::GetCurMySelfST()->ActiveThisObjects(aNodeKey,aElemKey,m_bIsActiveLayer,FALSE);

	// Active Observer에 등록.
	int nActiveCategory=0, nOption=0;
	CMapEx<UINT,UINT,UINT,UINT> mKey;
	mKey.InitHashTable(D_MAP_MAX_SIZE);
	mKey.RemoveAll();

	SetActiveObserver(&nActiveCategory, mKey, &nOption);
	CActiveObserver::GetInstance()->SetActive(mKey, nActiveCategory, nOption);
}

void CMActiveIdentityDlg::SetActiveObserver(int* pActiveCategory, CMapEx<UINT,UINT,UINT,UINT>& mKey, int* pOption)
{
	int nCount = m_lbItemList.GetSelCount();
	if (nCount == 0) 
	{ ASSERT(0); return; }
	CArray<int,int> aIndex;  
	aIndex.SetSize(nCount);
	int nCategory = GetCurCategory();
	switch(nCategory)
	{
	case CAT_NONE:
	case CAT_ELEM_TYPE:
	case CAT_MATL:
	case CAT_SECT:
	case CAT_THIK:
		*pActiveCategory = EN_ACTIVE_CAT_NONE;
		break;
	case CAT_NAPL:
		*pActiveCategory = EN_ACTIVE_CAT_NAPL;
		break;
	case CAT_STORY:
		*pActiveCategory = EN_ACTIVE_CAT_STOR;
		break;
	case CAT_GROUP:
		*pActiveCategory = EN_ACTIVE_CAT_GRUP;
		break;
	case CAT_BNDR_GROUP:
		*pActiveCategory = EN_ACTIVE_CAT_BNGR;
		break;
	case CAT_LOAD_GROUP:
		*pActiveCategory = EN_ACTIVE_CAT_LDGR;
		break;
	case CAT_MEMB:
		*pActiveCategory = EN_ACTIVE_CAT_MEMB;
		break;
	case CAT_LAGR:
		*pActiveCategory = EN_ACTIVE_CAT_LAGR;
		break;    
	default:
		ASSERT(0);
		break;
	}
	
	if(nCategory==CAT_STORY)
		*pOption = m_nStoryOpt;
	
	DWORD nValue=0;
	UINT  nKey=0;
	mKey.RemoveAll();
	
	m_lbItemList.GetSelItems(nCount, aIndex.GetData());
	for(int i=0; i<nCount; i++)
	{
		nValue = m_lbItemList.GetItemData(aIndex[i]);
		nKey = nValue;
		
		mKey.SetAt(nKey, 1);
	}
}

/*
void CMActiveIdentityDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_nCategory == 0)
		I_GENModelBase::GetCurMySelfST()->ActiveOnTheCurUCSPlane();
	else
		DoActiveOp();
	CDialog::OnOK();
}
*/

void CMActiveIdentityDlg::OnCmdCurUcsplane() 
{
	// TODO: Add your control notification handler code here
	ChangeCategory();	
	m_lbItemList.SetFocus();
	m_lbItemList.SetCurSel(0);  

}

void CMActiveIdentityDlg::OnCmdBngr() 
{
	// TODO: Add your control notification handler code here
	ChangeCategory();	
	m_lbItemList.SetFocus();
	m_lbItemList.SetCurSel(0);  

}

void CMActiveIdentityDlg::OnCmdLdgr() 
{
	// TODO: Add your control notification handler code here
	ChangeCategory();	
	m_lbItemList.SetFocus();
	m_lbItemList.SetCurSel(0);  

}

void CMActiveIdentityDlg::OnCmdMember() 
{
	ChangeCategory();
	m_lbItemList.SetFocus();
	m_lbItemList.SetCurSel(0);  
}

void CMActiveIdentityDlg::OnCmdLagr()
{
	ChangeCategory();
	m_lbItemList.SetFocus();
	m_lbItemList.SetCurSel(0);    
}

void CMActiveIdentityDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CMActiveIdentityDlg::OnCmdApply() 
{
	UpdateData(TRUE);
	if(m_nCategory == 0)
		I_GENModelBase::GetCurMySelfST()->ActiveOnTheCurUCSPlane();
	else
		DoActiveOp();
}

void CMActiveIdentityDlg::OnCmdActiveallButton() 
{
	I_GENModelBase::GetCurMySelfST()->ActiveAll();	
}

void CMActiveIdentityDlg::OnCmdActiveAccum() 
{
	// TODO: Add your control notification handler code here
	CArray<UINT, UINT> aNodeKey;
	CArray<UINT, UINT> aElemKey;

	CQueryCtrl QueryCtrl;
	CQueryCondition QCond;
	GetQueryCondition(QCond);
		
	QueryCtrl.SelectKeyFromNode(aNodeKey, QCond);
	QueryCtrl.SelectKeyFromElem(aElemKey, QCond);
	
	if(aNodeKey.GetSize() || aElemKey.GetSize())
		I_GENModelBase::GetCurMySelfST()->ManagementObject_ActOrInAct(aNodeKey,aElemKey,m_bIsActiveLayer,1,FALSE);
	
	// Active Observer에 등록.
	int nActiveCategory=0, nOption=0;
	CMapEx<UINT,UINT,UINT,UINT> mKey;
	mKey.InitHashTable(D_MAP_MAX_SIZE);
	mKey.RemoveAll();
	
	SetActiveObserver(&nActiveCategory, mKey, &nOption);
	CActiveObserver::GetInstance()->SetActivePlus(mKey, nActiveCategory, nOption);
}

void CMActiveIdentityDlg::OnCmdInactiveAccum() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CArray<UINT, UINT> aNodeKey;
	CArray<UINT, UINT> aElemKey;

	CQueryCtrl QueryCtrl;
	CQueryCondition QCond;
	GetQueryCondition(QCond);
	
	QueryCtrl.SelectKeyFromNode(aNodeKey, QCond);
	QueryCtrl.SelectKeyFromElem(aElemKey, QCond);
	
	if(aNodeKey.GetSize() || aElemKey.GetSize())
		I_GENModelBase::GetCurMySelfST()->ManagementObject_ActOrInAct(aNodeKey,aElemKey,m_bIsActiveLayer,2,FALSE);

	// Active Observer에 등록.
	int nActiveCategory=0, nOption=0;
	CMapEx<UINT,UINT,UINT,UINT> mKey;
	mKey.InitHashTable(D_MAP_MAX_SIZE);
	mKey.RemoveAll();
	
	SetActiveObserver(&nActiveCategory, mKey, &nOption);
	CActiveObserver::GetInstance()->SetInactivePlus(mKey, nActiveCategory, nOption);
}

int CMActiveIdentityDlg::GetCategory()
{
	return m_nCategory;
}

void CMActiveIdentityDlg::ShowHideLoadingAreaPlane(BOOL bShow)
{
	// MNET:8108-HSSHIM-20140716
#if defined(_CIVIL)
	ASSERT(0);
	return;
#endif

	CWnd* pWndLoadingArea = GetDlgItem(IDC_CMD_LOADINGAREA_GRP);
	CWnd* pWndMember      = GetDlgItem(IDC_CMD_MEMBER);

	if(bShow)
	{
		pWndLoadingArea->ShowWindow(SW_SHOW);
		pWndMember->MoveWindow(&m_rectMember);
	}  
	else
	{
		pWndLoadingArea->ShowWindow(SW_HIDE);
		pWndMember->MoveWindow(&m_rectLoadingPlane);
	}
}
