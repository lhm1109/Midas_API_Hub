// CMSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSelectDlg.h"

#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\AutomationDataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning ( disable : 4838 )

#define THIK_DATA_FMT   _T("%g")

/////////////////////////////////////////////////////////////////////////////
// CCMSelectDlg dialog


CCMSelectDlg::CCMSelectDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSelectDlg)
	m_bNode = FALSE;
	m_bElem = FALSE;
	m_bMultiple = FALSE;
	m_nStoryOpt = 1;
	//}}AFX_DATA_INIT

	arStoryOption.Add(IDC_STORY_FLOOR);
	arStoryOption.Add(IDC_STORY_ABOVE);
	arStoryOption.Add(IDC_STORY_BELOW);
	arStoryOption.Add(IDC_STORY_Both);
}


void CCMSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSelectDlg)
	DDX_Control(pDX, IDC_CMD_SD_REPLACE_SEL, m_wndMultiple);
	DDX_Control(pDX, IDC_CMD_SD_THIK_NO, m_wndPickThikNo);
	DDX_Control(pDX, IDC_CMD_SD_SECT_NO, m_wndPickSectNo);
	DDX_Control(pDX, IDC_CMD_SD_MATL_NO, m_wndPickMatlNo);
	DDX_Control(pDX, IDC_CMD_SD_ELEM_TYPE, m_wndPickElemType);
	DDX_Control(pDX, IDC_CMD_SD_ELEM_CHECK, m_wndElemCheck);
	DDX_Control(pDX, IDC_CMD_SD_NODE_CHECK, m_wndNodeCheck);
	DDX_Control(pDX, IDC_CMD_SD_ITEM_LIST, m_lbItemList);
	DDX_Control(pDX, IDC_CMD_SD_CATEGORY, m_cboCategory);
	DDX_Check(pDX, IDC_CMD_SD_NODE_CHECK, m_bNode);
	DDX_Check(pDX, IDC_CMD_SD_ELEM_CHECK, m_bElem);
	DDX_Check(pDX, IDC_CMD_SD_REPLACE_SEL, m_bMultiple);
	DDX_Radio(pDX, IDC_STORY_FLOOR, m_nStoryOpt);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_SD_SELECT_NODE, m_editSelectNode);
	DDX_Control(pDX, IDC_CMD_SD_SELECT_ELEM, m_editSelectElem);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSelectDlg public member functions
void CCMSelectDlg::SelectKind(int nKind, int nCategory)
{
	if (nKind == SC_ID_NODE) m_bNode = TRUE;
	else if (nKind == SC_ID_ELEM) m_bElem = TRUE;
	else if (nKind == SC_ID_NODE+SC_ID_ELEM) m_bNode = m_bElem = TRUE; // 이 코드는 들어오질 않는데?

	m_wndNodeCheck.SetCheck(m_bNode); m_editSelectNode.EnableWindow(m_bNode);
	m_wndElemCheck.SetCheck(m_bElem); m_editSelectElem.EnableWindow(m_bElem);


	FillCategory();

	CDlgUtil::CobxSetCurSelItemData(m_cboCategory, nCategory);
	ChangeCategory();
}

/////////////////////////////////////////////////////////////////////////////
// CCMSelectDlg implementations
void CCMSelectDlg::AlignControls()
{
	CRect rRef;
	m_wndPickElemType.GetWindowRect(rRef);
	ScreenToClient(rRef);
	m_wndPickMatlNo.MoveWindow(rRef);
	m_wndPickSectNo.MoveWindow(rRef);
	m_wndPickThikNo.MoveWindow(rRef);

	CRect rMove;
	GetDlgItem(IDC_STORY_FLOOR)->GetWindowRect(rMove);
	ScreenToClient(rMove);
	int nDistY = rRef.bottom - rMove.bottom;
	CDlgUtil::CtrlMoveDistY(this, arStoryOption, nDistY);

	CRect rectLast, r;
	GetDlgItem(IDC_CMD_SD_CLOSE)->GetWindowRect(rectLast);
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);
	MoveWindow(r);
};

void CCMSelectDlg::FillCategory()
{
	// 이전 Category 저장
	int nCategory = CDlgUtil::CobxGetCurSelItemData(m_cboCategory, m_cboCategory.GetCurSel());

	m_cboCategory.ResetContent();
	m_lbItemList.ResetContent();

	CString aName[] = {
		CM_SD_ELEM_TYPE, CM_SD_MATERIAL, CM_SD_SECTION,
		CM_SD_THICKNESS, CM_SD_NAMED_P, CM_SD_STORY,
		CM_SD_GROUP, CM_SD_BNDR_GROUP, CM_SD_LOAD_GROUP,
		CM_SD_CONS, CM_SD_FRLS, CM_SD_WALL, CM_SD_WMAK, CM_SD_HNGT,
		CM_SD_MEMB
		};
	int aCat[] = {
		CAT_ELEM_TYPE, CAT_MATL, CAT_SECT,
		CAT_THIK, CAT_NAPL, CAT_STORY,
		CAT_GROUP, CAT_BNDR_GROUP, CAT_LOAD_GROUP,
		CAT_CONS, CAT_FRLS, CAT_WALL, CAT_WMAK, CAT_HNGT,
		CAT_MEMB
	};
	int aMask[] = {
		D_PROD_ALL, D_PROD_ALL, D_PROD_ALL, 
		D_PROD_GEN+D_PROD_CVL, D_PROD_ALL, D_PROD_GEN,
		D_PROD_ALL, D_PROD_ALL, D_PROD_ALL,
		D_PROD_ALL, D_PROD_GEN+D_PROD_CVL, D_PROD_GEN, D_PROD_GEN, D_PROD_GEN,
		D_PROD_ALL
	};
	int nMask = CProduct::GetProductID();

	// 같은 Category 띄운다
	if (m_bNode || m_bElem) // select both
	{
		int nNum = sizeof(aMask)/sizeof(int);
		for (int i = 0; i < nNum; i++)
		{
			if ((aMask[i] & nMask) == 0) continue;
			CDlgUtil::CobxAddItem(m_cboCategory, aName[i], aCat[i]);
		}
	}
	// 이전 Category 복구 또는 초기화
	if (nCategory == CB_ERR) m_cboCategory.SetCurSel(0);
	else CDlgUtil::CobxSetCurSelItemData(m_cboCategory, nCategory);
	ChangeCategory();
}

void CCMSelectDlg::ChangeCategory()
{
	int nCategory;
	nCategory = GetCurCategory();
	switch(nCategory)
	{
	case CAT_NONE:        MakeListNone();      break;
	case CAT_ELEM_TYPE:   MakeListElemType();  break;
	case CAT_MATL:        MakeListMatl();      break;
	case CAT_SECT:        MakeListSect();      break;
	case CAT_THIK:        MakeListThik();      break;
	case CAT_NAPL:        MakeListNpln();      break;
	case CAT_STORY:       MakeListStory();     break;
	case CAT_GROUP:       MakeListGroup();     break;
	case CAT_BNDR_GROUP:  MakeListBndrGroup(); break;
	case CAT_LOAD_GROUP:  MakeListLoadGroup(); break;
	case CAT_CONS:        MakeListCons();      break;
	case CAT_FRLS:        MakeListFrls();      break;
	case CAT_WALL:        MakeListWall();      break;
	case CAT_WMAK:        MakeListWmak();      break;
	case CAT_HNGT:        MakeListHngt();      break;
	case CAT_MEMB:        MakeListMemb();      break;
	//case CAT_LINK_TYPE:   MakeListLink();      break; jslee
	}
	ShowHidePickCtrl(nCategory);

#if defined( _CIVIL_RUS )
	CDlgUtil::AdjustListBoxWidth( m_lbItemList );
#endif
}

int CCMSelectDlg::GetCurCategory()
{
	int nCategory = CDlgUtil::CobxGetCurSelItemData(m_cboCategory, m_cboCategory.GetCurSel());
	if (nCategory == CB_ERR) return CAT_NONE;
	return nCategory;
}

void CCMSelectDlg::ShowHidePickCtrl(int nCategory)
{
	m_wndPickElemType.ShowWindow(SW_HIDE);
	m_wndPickMatlNo.ShowWindow(SW_HIDE);
	m_wndPickSectNo.ShowWindow(SW_HIDE);
	m_wndPickThikNo.ShowWindow(SW_HIDE);
	
	CDlgUtil::CtrlShowHide(this, arStoryOption, FALSE);
	switch(nCategory)
	{
	case CAT_NONE: 
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_ELEM_TYPE: 
		m_wndPickElemType.ShowWindow(SW_SHOW); 
		m_wndPickElemType.SetFocus();
		m_wndMultiple.ShowWindow(SW_SHOW);
		break;
	case CAT_MATL: 
		m_wndPickMatlNo.ShowWindow(SW_SHOW); 
		m_wndPickMatlNo.SetFocus();
		m_wndMultiple.ShowWindow(SW_SHOW);
		break;
	case CAT_SECT: 
		m_wndPickSectNo.ShowWindow(SW_SHOW); 
		m_wndPickSectNo.SetFocus();
		m_wndMultiple.ShowWindow(SW_SHOW);
		break;
	case CAT_THIK: 
		m_wndPickThikNo.ShowWindow(SW_SHOW); 
		m_wndPickThikNo.SetFocus();
		m_wndMultiple.ShowWindow(SW_SHOW);
		break;
	case CAT_NAPL: 
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_STORY: 
		m_wndMultiple.ShowWindow(SW_HIDE);
		CDlgUtil::CtrlShowHide(this, arStoryOption, TRUE);
		break;
	case CAT_BNDR_GROUP:
	case CAT_LOAD_GROUP:
	case CAT_GROUP: 
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_CONS: 
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_FRLS: 
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_WALL:
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_WMAK:
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_HNGT:
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	case CAT_MEMB:
		m_wndMultiple.ShowWindow(SW_HIDE);
		break;
	}
}

void CCMSelectDlg::SelectItemByKey(UINT key)
{
	int nSize = m_lbItemList.GetCount();
	int i = 0;
	for ( i = 0; i < nSize; i++)
	{
		if (key == m_lbItemList.GetItemData(i))
			break;
	}
	if (i != nSize) m_lbItemList.SetSel(i, TRUE);
}

void CCMSelectDlg::SelectListItemByPick(UINT nID)
{
	if (!m_bMultiple) 
	{
		int nSize = m_lbItemList.GetCount();
		for (int i = 0; i < nSize; i++) m_lbItemList.SetSel(i, FALSE);
	}

	switch(nID)
	{
	case IDC_CMD_SD_ELEM_TYPE:
		{
			int nElemType;
			if (!m_wndPickElemType.GetKey(nElemType)) return;
			SelectItemByKey((UINT)nElemType);
		}
		break;
	case IDC_CMD_SD_MATL_NO:
		{
			T_MATL_K key;
			if (!m_wndPickMatlNo.GetKey(key)) return;
			SelectItemByKey(key);
		}
		break;
	case IDC_CMD_SD_SECT_NO:
		{
			T_SECT_K key;
			m_wndPickSectNo.GetKey(key);
			SelectItemByKey(key);
		}
		break;
	case IDC_CMD_SD_THIK_NO:
		{
			T_THIK_K key;
			m_wndPickThikNo.GetKey(key);
			SelectItemByKey(key);
		}
		break;
	}
}

void CCMSelectDlg::MakeListNone()
{
	m_lbItemList.ResetContent();
}

void CCMSelectDlg::MakeListElemType()
{
	m_lbItemList.ResetContent();

	CArray<int, int> aType;
	CArray<CString, CString&> aName;
	int nCount = m_pDoc->m_pAttrCtrl->GetElemTypeAndName(aType, aName);
	int nIndex;
	for (int i = 0; i < nCount; i++)
	{
		nIndex = m_lbItemList.AddString(aName[i]);
		m_lbItemList.SetItemData(nIndex, (DWORD)aType[i]);
	}
}

void CCMSelectDlg::MakeListMatl()
{
	m_lbItemList.ResetContent();
	T_MATL_K Key;
	T_MATL_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_MATL_K, T_MATL_K> aKey;
	//m_pDoc->m_pAttrCtrl->GetMatlKeyList(aKey);
	m_pDoc->m_pAttrCtrl->GetMatlKeyListUsedInElem(aKey);
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		Key = aKey[i];
	  if (m_pDoc->m_pAttrCtrl->GetMatl(Key, Data))
			csItem.Format(_T("%-4d : %s"), Key, Data.Name);
		else csItem.Format(_T("%-4d : "), Key);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListSect()
{
	m_lbItemList.ResetContent();
	T_SECT_K Key;
	T_SECT_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_SECT_K, T_SECT_K> aKey;
	//m_pDoc->m_pAttrCtrl->GetSectKeyList(aKey);
	m_pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aKey);
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		Key = aKey[i];
	  if (m_pDoc->m_pAttrCtrl->GetSect(Key,Data))
			csItem.Format(_T("%-4d : %s"), Key, Data.SName);
		else csItem.Format(_T("%-4d : "), Key);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListThik()
{
	m_lbItemList.ResetContent();
	T_THIK_K Key;
	T_THIK_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_THIK_K, T_THIK_K> aKey;
	//m_pDoc->m_pAttrCtrl->GetThikKeyList(aKey);
	m_pDoc->m_pAttrCtrl->GetThikKeyListUsedInElem(aKey);
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		Key = aKey[i];
		double dThickness;
	  //if (m_pDoc->m_pAttrCtrl->GetThik(Key,Data))
		if (m_pDoc->m_pAttrCtrl->GetThikForThikName(Key,dThickness))
		{
			csItem.Format(_T("%-4d : %g"), Key, dThickness);
			//if (Data.nThickType == 0) // Thick
			//  csItem.Format(_T("%-4d : %g"), Key, Data.ThickIn);
			//else
			//  csItem.Format(_T("%-4d : %g(%g)"), Key, Data.ThickIn, Data.ThickOut);
		}
		else csItem.Format(_T("%-4d : "), Key);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListNpln()
{
	m_lbItemList.ResetContent();
	T_NPLN_K Key;
	T_NPLN_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_NPLN_K, T_NPLN_K> aKey;
	m_pDoc->m_pAttrCtrl->GetNplnKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = aKey[i];
	  m_pDoc->m_pAttrCtrl->GetNpln(Key,Data);
		csItem.Format(_T("%s"), Data.PlaneName);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListStory()
{
	m_lbItemList.ResetContent();
	T_STOR_K Key;
	T_STOR_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_STOR_K, T_STOR_K> aKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aKey);
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		Key = aKey[i];
	  m_pDoc->m_pAttrCtrl->GetStor(Key,Data);
		csItem.Format(_T("%s"), Data.StoryName);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListGroup()
{
	m_lbItemList.ResetContent();
	T_GRUP_K Key;
	T_GRUP_D Data;

	CString csItem;
	int nIndex = 0;
	CArray<T_GRUP_K, T_GRUP_K> aKey;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = aKey[i];
	  m_pDoc->m_pAttrCtrl->GetGrup(Key,Data);
		csItem.Format(_T("%s"), Data.GroupName);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListBndrGroup()
{
	T_BNGR_K Key;
	m_lbItemList.ResetContent();
	CArray<CString,CString&> strBngrList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrList(strBngrList);
	int nSize = strBngrList.GetSize();

	CString csItem;
	int nIndex = 0;
	
	nIndex = m_lbItemList.AddString(_LS(IDS_DB_GROUP_NAME_DEFAULT));
	m_lbItemList.SetItemData(nIndex, (DWORD)0);
	for (int i = 0 ; i < nSize ; i++)
	{
		Key = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(strBngrList[i]);
		csItem.Format(_T("%s"), strBngrList[i]);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListLoadGroup()
{
	T_LDGR_K Key;
	m_lbItemList.ResetContent();
	CArray<CString,CString&> strLdgrList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrList(strLdgrList);
	int nSize = strLdgrList.GetSize();

	CString csItem;
	int nIndex = 0;
	
	nIndex = m_lbItemList.AddString(_LS(IDS_DB_GROUP_NAME_DEFAULT));
	m_lbItemList.SetItemData(nIndex, (DWORD)0);
	for (int i = 0 ; i < nSize ; i++)
	{               
		Key = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgrKey(strLdgrList[i]);
		csItem.Format(_T("%s"), strLdgrList[i]);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListExistOrNot()
{
	m_lbItemList.ResetContent();

	int nIndex = 0;
	nIndex = m_lbItemList.AddString(_LS(IDS_WG_CMD__ADD2__Exist));
	m_lbItemList.SetItemData(nIndex, (DWORD)1);
	nIndex = m_lbItemList.AddString(_LS(IDS_WG_CMD__ADD2__Not_Exist));
	m_lbItemList.SetItemData(nIndex, (DWORD)0);
}

void CCMSelectDlg::MakeListCons()
{
	MakeListExistOrNot();
}

void CCMSelectDlg::MakeListFrls()
{
	MakeListExistOrNot();
}

void CCMSelectDlg::MakeListWall()
{
	m_lbItemList.ResetContent();
	int Key;

	CString csItem;
	int nIndex = 0;
	CArray<int,int> aKey;
	m_pDoc->m_pAttrCtrl->GetWallIdList(aKey);
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = aKey[i];
		csItem.Format(_T("%d"), Key);
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListWmak()
{
	m_lbItemList.ResetContent();
	int Key;

	CString csItem;
	int nIndex = 0;
	
	T_WMAK_D data;
	CArray<T_WMAK_K, T_WMAK_K> aKey;
	m_pDoc->m_pAttrCtrl->GetWmakKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetWmak(Key, data);
		csItem = data.MarkName;
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListHngt()
{
	m_lbItemList.ResetContent();
	int Key;

	CString csItem;
	T_HNGT_D data;
	int nIndex = 0;
	CArray<T_HNGT_K, T_HNGT_K> aKey;
	m_pDoc->m_pAttrCtrl->GetHngtKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetHngt(aKey[i], data);
		csItem = data.strTypeName;
		nIndex = m_lbItemList.AddString(csItem);
		m_lbItemList.SetItemData(nIndex, (DWORD)Key);
	}
}

void CCMSelectDlg::MakeListMemb()
{
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

void CCMSelectDlg::DoSelectOp(int nOpType)
{
	if (!ValidQueryCondition()) return;

	CArray<UINT, UINT> aNodeKey;
	CArray<UINT, UINT> aElemKey;

	CQueryCtrl* pQueryCtrl = GetQueryCtrl();
	if (m_bNode)
	{
		CArray<UINT, UINT> aOldKey, aNewKey;
		CQueryCondition QCond;
		GetQueryCondition(QCond);

		m_pDoc->m_pViewCtrl->GetAllSelectedNode(aOldKey);
		pQueryCtrl->SelectKeyFromNode(aNewKey, QCond);
		switch(nOpType)
		{
		case SET_OP_UNION:
			pQueryCtrl->UnionKey(aOldKey, aNewKey, aNodeKey);
			break;
		case SET_OP_INTERSECT:
			pQueryCtrl->IntersectKey(aOldKey, aNewKey, aNodeKey);
			break;
		case SET_OP_SUBTRACT:
			pQueryCtrl->SubtractKey(aOldKey, aNewKey, aNodeKey);
			break;
		case SET_OP_REPLACE:
			pQueryCtrl->ReplaceKey(aNewKey, aNodeKey);
			break;
		}
	}
	if (m_bElem)
	{
		CArray<UINT, UINT> aOldKey, aNewKey;
		CQueryCondition QCond;
		GetQueryCondition(QCond);

		m_pDoc->m_pViewCtrl->GetAllSelectedElem(aOldKey);
		pQueryCtrl->SelectKeyFromElem(aNewKey, QCond);
		switch(nOpType)
		{
		case SET_OP_UNION:
			pQueryCtrl->UnionKey(aOldKey, aNewKey, aElemKey);
			break;
		case SET_OP_INTERSECT:
			pQueryCtrl->IntersectKey(aOldKey, aNewKey, aElemKey);
			break;
		case SET_OP_SUBTRACT:
			pQueryCtrl->SubtractKey(aOldKey, aNewKey, aElemKey);
			break;
		case SET_OP_REPLACE:
			pQueryCtrl->ReplaceKey(aNewKey, aElemKey);
			break;
		}
	}
	
	if (m_bNode && m_bElem)
		m_pDoc->m_pViewCtrl->Select(NULL, aNodeKey, aElemKey, FALSE, TRUE);
	else if (m_bNode)
		m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodeKey, TRUE, TRUE);
	else if (m_bElem)
		m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemKey, FALSE, TRUE, TRUE);
//   else if (m_bLink)
//     m_pDoc->m_pViewCtrl->SelectLink(NULL, aLinkKey, FALSE, TRUE, TRUE);
}

// 아래 두개의 함수(ReplaceSelectNode, ReplaceSelectElem)은 구 버전의 함수이다.
// 이 함수들은 DoSelectOp에 통합되었다.
void CCMSelectDlg::ReplaceSelectNode(CView* pSender, CArray<T_NODE_K, T_NODE_K>&rNode)
{
	// Unselect list와 Select Add List를 분리.
	CArray<T_NODE_K, T_NODE_K> aAdd, aDel, aCur;
	m_pDoc->m_pViewCtrl->GetAllSelectedNode(aCur);
	qsort((void*)aCur.GetData(), aAdd.GetSize(), sizeof(T_NODE_K), CCompFunc::UINTAsc);
	qsort((void*)rNode.GetData(), rNode.GetSize(), sizeof(T_NODE_K), CCompFunc::UINTAsc);
	int n1, n2;
	for (n1=n2=0; n1 < aCur.GetSize() && n2 < rNode.GetSize(); )
	{
		if (aCur.GetAt(n1) < rNode.GetAt(n2)) aDel.Add(aCur.GetAt(n1++));
		else if (aCur.GetAt(n1) > rNode.GetAt(n2)) aAdd.Add(rNode.GetAt(n2++));
		else {n1++; n2++;}
	}
	for (; n1 < aCur.GetSize(); n1++) aDel.Add(aCur.GetAt(n1));
	for (; n2 < rNode.GetSize(); n2++) aAdd.Add(rNode.GetAt(n2));

	// 뺄건 빼고, 추가할 건 추가한다.
	m_pDoc->m_pViewCtrl->UnSelectNode(pSender, aDel);
	m_pDoc->m_pViewCtrl->SelectNode(pSender, aAdd);
}

// ViewCtrl에 추가되어야 할 내용
void CCMSelectDlg::ReplaceSelectElem(CView* pSender, CArray<T_ELEM_K, T_ELEM_K>&rElem, BOOL bIncludeNode)
{
	// Unselect list와 Select Add List를 분리.
	CArray<T_ELEM_K, T_ELEM_K> aAdd, aDel, aCur;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aCur);
	qsort((void*)aCur.GetData(), aAdd.GetSize(), sizeof(T_ELEM_K), CCompFunc::UINTAsc);
	qsort((void*)rElem.GetData(), rElem.GetSize(), sizeof(T_ELEM_K), CCompFunc::UINTAsc);
	int n1, n2;
	for (n1=n2=0; n1 < aCur.GetSize() && n2 < rElem.GetSize(); )
	{
		if (aCur.GetAt(n1) < rElem.GetAt(n2)) aDel.Add(aCur.GetAt(n1++));
		else if (aCur.GetAt(n1) > rElem.GetAt(n2)) aAdd.Add(rElem.GetAt(n2++));
		else {n1++; n2++;}
	}
	for (; n1 < aCur.GetSize(); n1++) aDel.Add(aCur.GetAt(n1));
	for (; n2 < rElem.GetSize(); n2++) aAdd.Add(rElem.GetAt(n2));

	// 뺄건 빼고, 추가할 건 추가한다.
	m_pDoc->m_pViewCtrl->UnSelectElem(pSender, aDel, bIncludeNode);
	m_pDoc->m_pViewCtrl->SelectElem(pSender, aAdd, bIncludeNode);
}

BOOL CCMSelectDlg::ValidQueryCondition()
{
	if (GetCurCategory() == CAT_NONE) return FALSE;
	if (m_lbItemList.GetSelCount() == 0) return FALSE;
	return TRUE;
}

CQueryCtrl* CCMSelectDlg::GetQueryCtrl()
{
	return &m_QueryCtrl;
}

void CCMSelectDlg::GetQueryCondition(CQueryCondition& QCond)
{
	int nCategory = GetCurCategory();
	QCond.m_nCategory = nCategory;
	QCond.m_nOperation = QC_OP_OR;
	int nCount = m_lbItemList.GetSelCount();
	if (nCount == 0) return;

	CString sValue;
	DWORD nValue;
	int *aIndex = new int[nCount];
	m_lbItemList.GetSelItems(nCount, aIndex);
	for (int i = 0; i < nCount; i++)
	{
		switch(nCategory)
		{
		case CAT_STORY:
			UpdateData(TRUE);
			QCond.m_nStoryOption = m_nStoryOpt;   // Story Option
		case CAT_ELEM_TYPE:
		case CAT_MATL:
		case CAT_SECT:
		case CAT_THIK:
		case CAT_NAPL:
		case CAT_GROUP:
		case CAT_LOAD_GROUP:
		case CAT_BNDR_GROUP:
		case CAT_CONS:
		case CAT_FRLS:
		case CAT_WALL:
		case CAT_WMAK:
		case CAT_HNGT:
		case CAT_MEMB:
			nValue = m_lbItemList.GetItemData(aIndex[i]);
			sValue.Format(_T("%ld"), nValue);
			QCond.m_aValue.Add(sValue);
			break;
		}
	}
	delete[] aIndex;
	return;
}


#define CDialog CCMDlgBase
BEGIN_MESSAGE_MAP(CCMSelectDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMSelectDlg)
	ON_BN_CLICKED(IDC_CMD_SD_CLOSE, OnDlgClose)
	ON_BN_CLICKED(IDC_CMD_SD_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_CMD_SD_NONE, OnSelectNone)
	ON_BN_CLICKED(IDC_CMD_SD_INV, OnSelectInversion)
	ON_BN_CLICKED(IDC_CMD_SD_PREV, OnSelectPrevious)
	ON_CBN_SELCHANGE(IDC_CMD_SD_CATEGORY, OnChangeCategory)
	ON_BN_CLICKED(IDC_CMD_SD_ADD, OnClickBtnAdd)
	ON_BN_CLICKED(IDC_CMD_SD_DEL, OnClickBtnDel)
	ON_BN_CLICKED(IDC_CMD_SD_REPLACE, OnClickBtnReplace)
	ON_BN_CLICKED(IDC_CMD_SD_INTERSECT, OnClickBtnIntersect)
	ON_BN_CLICKED(IDC_CMD_SD_NODE_CHECK, OnToggleNodeCheck)
	ON_BN_CLICKED(IDC_CMD_SD_ELEM_CHECK, OnToggleElemCheck)
	ON_BN_CLICKED(IDC_CMD_SD_REPLACE_SEL, OnToggleReplaceSel)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER, OnMouseEditEnter)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSelectDlg message handlers

void CCMSelectDlg::OnDlgClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();	

	CAutomationDataMgr::m_bDlgAlreadyDestroy = TRUE;// Automation 할 때 잘 동작하게 하기 위해 넣은 코드
}

BOOL CCMSelectDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	
	// TODO: Add extra initialization here

	AlignControls();
	FillCategory();

	m_wndPickElemType.SetLButtonDownNotifyWindow(this);
	m_wndPickElemType.SetEnterNotifyWindow(this);
	m_wndPickMatlNo.SetLButtonDownNotifyWindow(this);
	m_wndPickMatlNo.SetEnterNotifyWindow(this);
	m_wndPickSectNo.SetLButtonDownNotifyWindow(this);
	m_wndPickSectNo.SetEnterNotifyWindow(this);
	m_wndPickThikNo.SetLButtonDownNotifyWindow(this);
	m_wndPickThikNo.SetEnterNotifyWindow(this);
	
	//if( CDBLib::Is64BitOS() )
	//{
	//  m_editSelectNode.LimitText(42765);
	//  m_editSelectElem.LimitText(42765);
	//}

	m_editSelectNode.Connect(SC_ID_NODE, &m_editSelectNode);
	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);

	AUTOMATION_SET_TIMER_CPP()

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

AUTOMATION_TIMER_CPP(CCMSelectDlg)

void CCMSelectDlg::OnSelectAll() 
{
	// TODO: Add your control notification handler code here
	if (m_bNode)
		m_pDoc->m_pViewCtrl->SelectAllNode(NULL);
	if (m_bElem)
		m_pDoc->m_pViewCtrl->SelectAllElem(NULL, FALSE);
}

void CCMSelectDlg::OnSelectNone() 
{
	// TODO: Add your control notification handler code here
	if (m_bNode)
		m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	if (m_bElem)
		m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CCMSelectDlg::OnSelectInversion() 
{
	CArray<T_NODE_K, T_NODE_K> aNodeSel;
	CArray<T_ELEM_K, T_ELEM_K> aElemSel;

	// TODO: Add your control notification handler code here
	if (m_bNode)
	{
		m_pDoc->m_pViewCtrl->GetAllUnSelectedNode(aNodeSel);
		m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
		m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodeSel);
	}
	if (m_bElem)
	{
		m_pDoc->m_pViewCtrl->GetAllUnSelectedElem(aElemSel);
		m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
		m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemSel, FALSE);
	}
}

void CCMSelectDlg::OnSelectPrevious() 
{
	// TODO: Add your control notification handler code here
	CString sSelectText;
	if (m_bNode)
	{
		sSelectText = m_editSelectNode.GetPreviousSelectText();
		m_editSelectNode.SelectByStr(sSelectText);
	}
	if (m_bElem)
	{
		sSelectText = m_editSelectElem.GetPreviousSelectText();
		m_editSelectElem.SelectByStr(sSelectText);
	}	
}

void CCMSelectDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
		UpdateUnit();
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

void CCMSelectDlg::UpdateBuffer()
{
	BOOL bMatlChanged = FALSE;
	BOOL bSectChanged = FALSE;
	BOOL bThikChanged = FALSE;
	BOOL bStorChanged = FALSE;
	BOOL bNplnChanged = FALSE;
	BOOL bGrupChanged = FALSE;
	BOOL bBngrChanged = FALSE;
	BOOL bLdgrChanged = FALSE;

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
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
		case(UR_ELEM_ADD): case(UR_ELEM_DEL): case(UR_ELEM_MFD): case(UR_ELEM_MFS):
			bMatlChanged = bSectChanged = bThikChanged = TRUE;
			break;
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
		case(UR_LDGR_ADD): case(UR_LDGR_DEL): case(UR_LDGR_MFD): case(UR_LDGR_MFS):
			bLdgrChanged = TRUE;
			break;
		case(UR_BNGR_ADD): case(UR_BNGR_DEL): case(UR_BNGR_MFD): case(UR_BNGR_MFS):
			bBngrChanged = TRUE;
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
		if (bMatlChanged) MakeListMatl();
		break;
	case CAT_SECT:
		if (bSectChanged) MakeListSect();
		break;
	case CAT_THIK:
		if (bThikChanged) MakeListThik();
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
	case CAT_LOAD_GROUP:
		if (bLdgrChanged) MakeListLoadGroup();
		break;
	case CAT_BNDR_GROUP:
		if (bBngrChanged) MakeListBndrGroup();
		break;
	}
}

void CCMSelectDlg::UpdateUnit()
{
	int nCategory;
	nCategory = GetCurCategory();
	if (nCategory == CAT_THIK) MakeListThik();
}

void CCMSelectDlg::OnChangeCategory() 
{
	// TODO: Add your control notification handler code here
	ChangeCategory();

	// Category에 따라 Node, Element On/Off를 제어한다.
	int nCategory;
	nCategory = GetCurCategory();

	m_bNode = m_bElem = FALSE;
	switch(nCategory)
	{
	case CAT_NONE: break;
	case CAT_ELEM_TYPE: m_bNode = FALSE; m_bElem = TRUE; break;
	case CAT_MATL:      m_bNode = FALSE; m_bElem = TRUE; break;
	case CAT_SECT:      m_bNode = FALSE; m_bElem = TRUE; break;
	case CAT_THIK:      m_bNode = FALSE; m_bElem = TRUE; break;
	case CAT_NAPL:      m_bNode = TRUE;  m_bElem = TRUE; break;
	case CAT_STORY:     m_bNode = TRUE;  m_bElem = TRUE; break;
	case CAT_BNDR_GROUP:
	case CAT_LOAD_GROUP:
	case CAT_GROUP:     m_bNode = TRUE;  m_bElem = TRUE; break;
	case CAT_CONS:      m_bNode = TRUE;  m_bElem = FALSE;break;
	case CAT_FRLS:      m_bNode = FALSE; m_bElem = TRUE; break;
	case CAT_WALL:      m_bNode = TRUE;  m_bElem = TRUE; break;
	case CAT_WMAK:      m_bNode = TRUE;  m_bElem = TRUE; break;
	case CAT_HNGT:      m_bNode = FALSE; m_bElem = TRUE; break;
	case CAT_MEMB:      m_bNode = TRUE;  m_bElem = TRUE; break;
	
	}
	m_wndNodeCheck.SetCheck(m_bNode); m_editSelectNode.EnableWindow(m_bNode);
	m_wndElemCheck.SetCheck(m_bElem); m_editSelectElem.EnableWindow(m_bElem);
}

void CCMSelectDlg::OnClickBtnAdd() 
{
	// TODO: Add your control notification handler code here
	DoSelectOp(SET_OP_UNION);  
}

void CCMSelectDlg::OnClickBtnDel() 
{
	// TODO: Add your control notification handler code here
	DoSelectOp(SET_OP_SUBTRACT);
}

void CCMSelectDlg::OnClickBtnReplace() 
{
	// TODO: Add your control notification handler code here
	DoSelectOp(SET_OP_REPLACE);
}

void CCMSelectDlg::OnClickBtnIntersect() 
{
	// TODO: Add your control notification handler code here
	DoSelectOp(SET_OP_INTERSECT);
}

void CCMSelectDlg::OnToggleNodeCheck() 
{
	// TODO: Add your control notification handler code here
	m_bNode = m_wndNodeCheck.GetCheck();
	m_editSelectNode.EnableWindow(m_bNode);
	FillCategory();
}

void CCMSelectDlg::OnToggleElemCheck() 
{
	// TODO: Add your control notification handler code here
	m_bElem = m_wndElemCheck.GetCheck();
	m_editSelectElem.EnableWindow(m_bElem);
	FillCategory();
}


LRESULT CCMSelectDlg::OnMouseEditEnter(WPARAM wParam, LPARAM lParam)
{
	// lParam == HWND ==> Message를 보낸 CMouseEdit의 HWND ...
	SelectListItemByPick(wParam);
	return 0L;
}

LRESULT CCMSelectDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	SelectListItemByPick(wParam);
	return 0L;
}

void CCMSelectDlg::OnToggleReplaceSel() 
{
	// TODO: Add your control notification handler code here
	m_bMultiple = m_wndMultiple.GetCheck();
}
