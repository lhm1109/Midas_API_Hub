// StldTendonPrst.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldTendonPrst.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"


#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldTendonPrst dialog
#define CDialog CMenuBarChildDlg



static int CALLBACK 
MyListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	// The lParam of an item is just its index.
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
	CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

	return strItem1.Compare(strItem2);
}

class ListItem
{
public:
	ListItem();
	ListItem(ListItem&Src);
	ListItem& operator = (ListItem&Src);

	CString m_Txt ;
	DWORD   m_Data;
};

ListItem::ListItem()
{

}

ListItem::ListItem(ListItem&Src)
{
	m_Txt  = Src.m_Txt;
	m_Data = Src.m_Data;
}

ListItem& ListItem::operator =(ListItem&Src)
{
	m_Txt  = Src.m_Txt ;
	m_Data = Src.m_Data;
	return *this;
}

int  __ListItemCompare (const void* arg1, const void* arg2)
{
	return ((ListItem*)arg1)->m_Txt.Compare(((ListItem*)arg2)->m_Txt);
}


void SortCListCtrl_Items(CListCtrl* pListCtrl)
{
	//pListCtrl->SortItems(MyListCompareProc, (LPARAM) pListCtrl);
	//pListCtrl->

	ListItem Item;
	CArray<ListItem,ListItem&> Items;
	
	int nItem = pListCtrl->GetItemCount();
	for(int i= 0; i < nItem ; i++)
	{
		Item.m_Txt  = pListCtrl->GetItemText(i,0);
		Item.m_Data = pListCtrl->GetItemData(i);
		Items.Add(Item);
	}

	qsort(Items.GetData(),Items.GetSize(),sizeof(ListItem), __ListItemCompare);

	CStringArray Contents;

	nItem = Items.GetSize();

	pListCtrl->DeleteAllItems();

	for(int  i = 0; i < nItem; i++)
	{
		Contents.RemoveAll();
		Contents.Add(Items[i].m_Txt);
		CDlgUtil::SetListItem(pListCtrl,i,Contents,Items[i].m_Data);
	}
}



CStldTendonPrst::CStldTendonPrst(CWnd* pParent /*=NULL*/)
	: CDialog(CStldTendonPrst::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldTendonPrst)
	m_nStressOpt = 0;
	//}}AFX_DATA_INIT
	m_n1stJackEnd = 0;
}

CStldTendonPrst::~CStldTendonPrst()
{

}


void CStldTendonPrst::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldTendonPrst)
	DDX_Control(pDX, IDC_TM_1STJACK_COMBO, m_wnd1stJackCombo);
	DDX_Control(pDX, IDC_TM_TPRST_RLIST, m_wndRList);
	DDX_Control(pDX, IDC_TM_TPRST_LLIST, m_wndLList);
	DDX_Control(pDX, IDC_TM_TPRST_LG, m_CobxLoadGroup);
	DDX_Control(pDX, IDC_TM_TPRST_LC, m_CobxLoadCase);
	DDX_Control(pDX, IDC_TM_TPRST_VALUE_BEGIN_ED, m_wndStreeBegin);
	DDX_Control(pDX, IDC_TM_TPRST_VALUE_END_ED, m_wndStressEnd);
	DDX_Control(pDX, IDC_TM_TPRST_VALUE_BEGIN_UNIT, m_wndStressUnit1);
	DDX_Control(pDX, IDC_TM_TPRST_VALUE_END_UNIT, m_wndStressUnit2);
	DDX_Control(pDX, IDC_TM_TPRST_LIST, m_wndTdplList);
	DDX_Radio(pDX, IDC_TM_TPRST_STRESS_CHK, m_nStressOpt);
	DDX_Control(pDX, IDC_TM_TPRST_GROUTING_EDIT, m_editGrouting);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldTendonPrst, CDialog)
	//{{AFX_MSG_MAP(CStldTendonPrst)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnTmBtnLoadCase)
	ON_BN_CLICKED(IDC_TM_BTN_LG, OnTmBtnLoadGroup)
	ON_BN_CLICKED(IDC_TM_TPRST_LBTN, OnTmTprstLeftButton)
	ON_BN_CLICKED(IDC_TM_TPRST_RBTN, OnTmTprstRightButton)
	ON_BN_CLICKED(IDC_TM_TPRST_ADD_BTN, OnTmTprstAddBtn)
	ON_BN_CLICKED(IDC_TM_TPRST_DEL_BTN, OnTmTprstDelBtn)
	ON_BN_CLICKED(IDC_TM_TPRST_MODY_BTN, OnTmTprstModyBtn)
	ON_NOTIFY(NM_CLICK, IDC_TM_TPRST_LIST, OnClickTmTprstList)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_TPRST_STRESS_CHK, OnTmTprstStressChk)
	ON_BN_CLICKED(IDC_TM_TPRST_FORCE_CHK, OnTmTprstForceChk)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_TPRST_LLIST, OnDblclkTmTprstLlist)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_TPRST_RLIST, OnDblclkTmTprstRlist)
	ON_CBN_SELCHANGE(IDC_TM_1STJACK_COMBO, OnSelchangeTm1stjackCombo)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CNotifyListCtrl::CNOTIFY_LCTRL_KEYUP,OnListCtrlKeyUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldTendonPrst message handlers

BOOL CStldTendonPrst::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	MakeListHeader();

	m_wnd1stJackCombo.AddString(_LS(IDS_TENDON_PRST__Begin));
	m_wnd1stJackCombo.AddString(_LS(IDS_TENDON_PRST__End)  );
	m_wnd1stJackCombo.AddString(_LS(IDS_TENDON_PRST__Both) );  
	m_wnd1stJackCombo.SetCurSel(0);
	
	m_editGrouting.SetRange(0, 1000);
	m_editGrouting.SetInteger(TRUE);
	m_editGrouting.SetValue(0);

	StressOptMan(m_nStressOpt);

	UpdateTdplList();
	UpdateTendonList();
	UpdateRightTendonList();
	
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_TPRST_DIAGM_BTN)->ShowWindow(SW_HIDE); // 안쓰는 버튼

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldTendonPrst::StressOptMan(int nStressOpt)
{

	if(nStressOpt == 0) // Stress
	{
		m_wndStressUnit1.SetUnitType(CUnitCtrl::m_TDPL_UNIT.dStressValue);
		m_wndStressUnit2.SetUnitType(CUnitCtrl::m_TDPL_UNIT.dStressValue);
		m_wndStreeBegin .SetUnitType(CUnitCtrl::m_TDPL_UNIT.dStressValue);
		m_wndStressEnd  .SetUnitType(CUnitCtrl::m_TDPL_UNIT.dStressValue);
	}
	else  // Force 
	{
		m_wndStressUnit1.SetUnitType(CUnitCtrl::m_TDPL_UNIT.dForceValue);
		m_wndStressUnit2.SetUnitType(CUnitCtrl::m_TDPL_UNIT.dForceValue);
		m_wndStreeBegin .SetUnitType(CUnitCtrl::m_TDPL_UNIT.dForceValue);
		m_wndStressEnd  .SetUnitType(CUnitCtrl::m_TDPL_UNIT.dForceValue);
	}
}


void CStldTendonPrst::MakeListHeader()
{
	CStringArray HTitles;
	CArray<float,float> HRatio;
	HTitles.Add(_LS(IDS_TENDON_PRST__Tendon)   );
	HTitles.Add(_LS(IDS_TENDON_PRST__Type)     );
	HTitles.Add(_LS(IDS_TENDON_PRST__Load_Case));
	HTitles.Add(_LS(IDS_TENDON_PRST__Group)    );
	HRatio.Add(0.3f);
	HRatio.Add(0.3f);
	HRatio.Add(0.3f);
	HRatio.Add(0.3f);
	CDlgUtil::_SetListCtrlHeader(&m_wndTdplList,HTitles,&HRatio,NULL);

	HTitles.RemoveAll();
	HRatio .RemoveAll();

	HTitles.Add(_LS(IDS_TENDON_PRST__Name));
	HRatio.Add(2.0f);

	CDlgUtil::_SetListCtrlHeader(&m_wndLList,HTitles,&HRatio,NULL);
	CDlgUtil::_SetListCtrlHeader(&m_wndRList,HTitles,&HRatio,NULL);

}

/*
#define T_TDPL_KEY unsigned int
union T_TDPL_K
{
	T_TDPL_KEY keymap;
	struct 
	{
		unsigned int entity : 20; // T_TDNA_K
		unsigned int serial : 12;
	} key;
};

struct T_TDPL_D
{
	T_STLD_K LoadCaseKey;
	T_LDGR_K GroupKey;
	int nLoadType;  // 0=Prestress, 1=Post-stress
	double dAnchoSlipBegin;
	double dAnchoSlipEnd;
	int nValueType; // 0=Stress, 1=Force;
	double dValueBegin[2];  // 0 for Stress, 1 for Force
	double dValueEnd[2];    // 0 for Stress, 1 for Force
	
	void Initialize()
	{
		LoadCaseKey = 0;
		GroupKey = 0;
		nLoadType = 0;
		dAnchoSlipBegin = 0.0;
		dAnchoSlipEnd = 0.0;
		nValueType = 0;
		dValueBegin[0] = dValueBegin[1] = 0;
		dValueEnd[0] = dValueEnd[1] = 0;
	}
};
*/


void CStldTendonPrst::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	 	UpdateTdplList();
		UpdateTendonList();
		UpdateRightTendonList();
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


void CStldTendonPrst::UpdateBuffer()
{
	CViewBuff* pViewBuff=  CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount ==0 ) return;

	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdnaCommand = FALSE;
	BOOL bIsTdplCommand = FALSE;
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_TDNA_ADD):
		case(UR_TDNA_DEL):
			bIsTdnaCommand = TRUE;
			break;
		case(UR_TDPL_ADD):
		case(UR_TDPL_DEL):
			bIsTdplCommand = TRUE;
			break;
		default:
			break;
		}
	}
	
	if(bIsTdplCommand ||bIsTdnaCommand)
	{
		UpdateTendonList();
		UpdateRightTendonList();
		UpdateTdplList();
	}
}

void CStldTendonPrst::Data2Dlg(T_TDPL_K TdplK)
{
	T_TDPL_D TdplD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdpl(TdplK,TdplD)) return;

	m_wnd1stJackCombo.SetCurSel(TdplD.nOrder);

	m_CobxLoadCase .ChangeSelect(D_LOADCASE_STATIC, TdplD.LoadCaseKey);
	m_CobxLoadGroup.ChangeSelect(TdplD.GroupKey);

	m_nStressOpt    = TdplD.nValueType;
	StressOptMan(m_nStressOpt);
	
	m_wndStreeBegin.SetEditUnit(TdplD.dValueBegin[m_nStressOpt]);
	m_wndStressEnd  .SetEditUnit(TdplD.dValueEnd[m_nStressOpt] );
	
	T_TDNA_K TdnaK = TdplK.key.entity;
	T_TDNA_D TdnaD;
	m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD);
	CString	strTendonName  = TdnaD.TendonName;
	m_editGrouting.SetValue(TdplD.nGrouting);
	
	m_wndRList.DeleteAllItems();
	int ix = m_wndRList.InsertItem(0, strTendonName);
	m_wndRList.SetItemData(ix,TdnaK);
	UpdateData(FALSE);
	this->UpdateRightTendonList();
}

void CStldTendonPrst::Dlg2Data(T_TDPL_D& TdplD)
{
	UpdateData();

	UINT nLCType,nLCKey;

	m_CobxLoadCase.GetSelectedLoad(nLCType,nLCKey);
	
	TdplD.nOrder  = m_wnd1stJackCombo.GetCurSel();
	
	TdplD.LoadCaseKey = nLCKey;
	TdplD.nValueType  = this->m_nStressOpt;
	TdplD.dValueBegin[TdplD.nValueType] = m_wndStreeBegin.GetEditValue();
	TdplD.dValueEnd  [TdplD.nValueType] = m_wndStressEnd.GetEditValue();

	TdplD.nGrouting = m_editGrouting.GetEditValueInt();
	
	T_LDGR_K LdgrK;
	m_CobxLoadGroup.GetSelectedLdgr(LdgrK);
	TdplD.GroupKey = LdgrK;
}

void CStldTendonPrst::UpdateTendonList()
{
	/*
	m_wndLList.DeleteAllItems();
	CArray<T_TDNA_K, T_TDNA_K> TdnaKeys;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(TdnaKeys);
	int nTdna = TdnaKeys.GetSize();
	if(nTdna == 0)
	{
		return;
	}
	
	CStringArray Contents;
	T_TDNA_D TdnaD;
	for( int i = 0; i < nTdna; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetTdna(TdnaKeys[i],TdnaD))
		{
			Contents.RemoveAll();
			Contents.Add(TdnaD.TendonName);
			CDlgUtil::SetListItem(&m_wndLList,i,Contents,TdnaKeys[i]);
		}
	}
	*/
}

void CStldTendonPrst::UpdateRightTendonList()
{

	CArray<T_TDNA_K, T_TDNA_K> TdnaKeys;
	CMap<T_TDNA_K,T_TDNA_K,T_TDNA_K,T_TDNA_K> TdnaKMap;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(TdnaKeys);
	int nTdna = TdnaKeys.GetSize();

	for(int  i = 0 ; i < nTdna ; i++)
	{
		TdnaKMap.SetAt(TdnaKeys[i],TdnaKeys[i]);
	}
	
	CArray<T_TDNA_K, T_TDNA_K> CurTdnaKeys;
	int nItem = m_wndRList.GetItemCount();
	T_TDNA_K TdnaK;
	for(int i= 0; i < nItem ; i++)
	{
		TdnaK = (T_TDNA_K)m_wndRList.GetItemData(i);
		CurTdnaKeys.Add(TdnaK);
		TdnaKMap.RemoveKey(TdnaK);
	}
		
	CStringArray Contents;
	T_TDNA_D TdnaD;
	int nCurTdna = CurTdnaKeys.GetSize();
	m_wndRList.DeleteAllItems();
	for(int i = 0; i < nItem ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetTdna(CurTdnaKeys[i],TdnaD))
		{
			Contents.RemoveAll();
			Contents.Add(TdnaD.TendonName);
			CDlgUtil::SetListItem(&m_wndRList,i,Contents,CurTdnaKeys[i]);
		}
	}

	m_wndLList.DeleteAllItems();
	POSITION Pos;
	Pos = TdnaKMap.GetStartPosition();
	T_TDNA_K KTemp;
	int Index = 0;
	while(Pos)
	{
		TdnaKMap.GetNextAssoc(Pos,TdnaK,KTemp);
		if(m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD))
		{
			Contents.RemoveAll();
			Contents.Add(TdnaD.TendonName);
			CDlgUtil::SetListItem(&m_wndLList,Index,Contents,TdnaK);
			Index++;
		}      
	}

	SortCListCtrl_Items(&m_wndRList);
	SortCListCtrl_Items(&m_wndLList);
}


void CStldTendonPrst::GetTdplListFromDBDoc(CArray<T_TDPL_K,T_TDPL_K>*pTdplKeys,
																					 CArray<T_TDPL_D,T_TDPL_D>*pTdplDatas)
{
	if(pTdplKeys)
		pTdplKeys->RemoveAll();
	if(pTdplDatas)
		pTdplDatas->RemoveAll();
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountTdpl();
	if(nItemCount)
	{
		T_TDPL_K TdplK;
		T_TDPL_D TdplD;
		POSITION Pos;
		Pos = m_pDoc->m_pAttrCtrl->GetStartTdpl();
		while(Pos)
		{
			m_pDoc->m_pAttrCtrl->GetNextTdpl(Pos,TdplK,TdplD);
			if(pTdplKeys)
				pTdplKeys->Add(TdplK);
			if(pTdplDatas)
				pTdplDatas->Add(TdplD);
		}
	}
}

void  CStldTendonPrst::UpdateTdplList()
{
	m_wndTdplList.DeleteAllItems();
	CArray<T_TDPL_K,T_TDPL_K> TdplKeys;
	CArray<T_TDPL_D,T_TDPL_D> TdplDatas;

	GetTdplListFromDBDoc(&TdplKeys,&TdplDatas);

	int nTdpl = TdplKeys.GetSize();

	CStringArray Contents;
	CString StrTemp;

	T_TDNA_D TdnaD;
	T_LDGR_D LdgrD;
	T_STLD_D StldD; 
 
	for( int i = 0; i < nTdpl ; i++)
	{
		Contents.RemoveAll();
		if(!m_pDoc->m_pAttrCtrl->GetTdna(TdplKeys [i].key.entity,TdnaD)) return;
		Contents.Add(TdnaD.TendonName);

		if(TdplDatas[i].nValueType == 0)
			StrTemp.Format(_LS(IDS_TENDON_PRST__Stress));
		else
			StrTemp.Format(_LS(IDS_TENDON_PRST__Force));
		Contents.Add(StrTemp);

		m_pDoc->m_pAttrCtrl->GetStld(TdplDatas[i].LoadCaseKey,StldD);
		Contents.Add(StldD.LoadCaseName);
		
		if(m_pDoc->m_pAttrCtrl->GetLdgr(TdplDatas[i].GroupKey,LdgrD))
			Contents.Add(LdgrD.GroupName);
		else
			Contents.Add(_LS(IDS_TENDON_PRST__Default));
 
		CDlgUtil::SetListItem(&m_wndTdplList,i,Contents,(DWORD)TdplKeys[i].keymap);
	}
}

void CStldTendonPrst::OnTmBtnLoadCase() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldTendonPrst::OnTmBtnLoadGroup() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLoadGroupDlg::IDD);
}

void CStldTendonPrst::OnTmTprstLeftButton() 
{
	CArray<int, int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndRList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();
	if(nRet == 0) return;

	int nItem = m_wndRList.GetItemCount();
	CMap<T_TDNA_K, T_TDNA_K,T_TDNA_K,T_TDNA_K> TdnaKMap;
	CArray<T_TDNA_K, T_TDNA_K> TdnaKeys;
	T_TDNA_K TdnaK;
	
	for( int i = 0; i < nItem ; i++)
	{
		TdnaK   = (T_TDNA_K) m_wndRList.GetItemData(i); 
		TdnaKMap.SetAt(TdnaK,TdnaK);
		TdnaKeys.Add(TdnaK);
	}

	for(int i = 0; i < nRet; i++)
	{
		TdnaKMap.RemoveKey(TdnaKeys[arRet[i]]);
	}
	
	m_wndRList.DeleteAllItems();
	POSITION Pos;
	T_TDNA_D TdnaD;
	T_TDNA_K KTemp;
	CStringArray Contents;
	Pos = TdnaKMap.GetStartPosition();
	int Index = 0;
	while(Pos)
	{
		TdnaKMap.GetNextAssoc(Pos,TdnaK,KTemp);
		if(m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD))
		{
			Contents.RemoveAll();
			Contents.Add(TdnaD.TendonName);
			CDlgUtil::SetListItem(&m_wndRList,Index,Contents,TdnaK);
			Index++;
		}
	}
		
	//SortCListCtrl_Items(&m_wndRList);
	UpdateRightTendonList();
}

void CStldTendonPrst::OnTmTprstRightButton() 
{
	CArray<int, int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndLList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();
	if(nRet == 0) return;

	int nItem = m_wndRList.GetItemCount();
	CMap<T_TDNA_K, T_TDNA_K,T_TDNA_K,T_TDNA_K> TdnaKMap;
	T_TDNA_K TdnaK;
	
	for( int i = 0; i < nItem ; i++)
	{
		TdnaK   = (T_TDNA_K) m_wndRList.GetItemData(i); 
		TdnaKMap.SetAt(TdnaK,TdnaK);
	}

	for(int i = 0; i < nRet; i++)
	{
		TdnaK = (T_TDNA_K)m_wndLList.GetItemData(arRet[i]);
		TdnaKMap.SetAt(TdnaK,TdnaK);
	}
	
	if(TdnaKMap.GetCount() == 0) return;

	m_wndRList.DeleteAllItems();
	int nRList = m_wndRList.GetItemCount();
	POSITION Pos;
	T_TDNA_D TdnaD;
	T_TDNA_K KTemp;
	CStringArray Contents;
	Pos = TdnaKMap.GetStartPosition();
	int Index = 0;
	while(Pos)
	{
		TdnaKMap.GetNextAssoc(Pos,TdnaK,KTemp);
		if(m_pDoc->m_pAttrCtrl->GetTdna(TdnaK,TdnaD))
		{
			Contents.RemoveAll();
			Contents.Add(TdnaD.TendonName);
			CDlgUtil::SetListItem(&m_wndRList,Index,Contents,TdnaK);
			Index++;
		}
	}
	//SortCListCtrl_Items(&m_wndRList);
	UpdateRightTendonList();
}

void CStldTendonPrst::OnTmTprstAddBtn() 
{
	CArray<T_TDNA_K, T_TDNA_K> TdnaKeys;
	T_TDPL_D TdplD;
	Dlg2Data(TdplD);

	int nItem = m_wndRList.GetItemCount();
	T_TDNA_K TdnaK;
	
	for( int i = 0; i < nItem ; i++)
	{
		TdnaK   = (T_TDNA_K) m_wndRList.GetItemData(i); 
		TdnaKeys.Add(TdnaK);
	}
	m_pDoc->m_pDataCtrl->AddTdpl(TdnaKeys,TdplD);
}

void CStldTendonPrst::OnTmTprstDelBtn() 
{
	CArray<T_TDPL_K, T_TDPL_K> TdplKeys;
	T_TDPL_K TdplK;
	CArray<int,int> arRet;
 
	CDlgUtil::GetListItemByMask(&m_wndTdplList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();

	for( int i = 0 ; i< nRet ; i++)
	{
		TdplK.keymap = (T_TDPL_KEY)m_wndTdplList.GetItemData(arRet[i]);
		TdplKeys.Add(TdplK);
	}
	m_pDoc->m_pDataCtrl->DelTdpl(TdplKeys);
}

void CStldTendonPrst::OnTmTprstModyBtn() 
{
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTdplList,arRet,LVIS_SELECTED);
	if(arRet.GetSize()>1 || arRet.GetSize() == 0) return;
	
	int nRList = this->m_wndRList.GetItemCount();
	if(nRList > 1 || nRList == 0) return;

	T_TDNA_K TdnaK = (T_TDNA_K)m_wndRList.GetItemData(0);
	
	T_TDPL_D TdplD;
	T_TDPL_K TdplK;

	TdplK.keymap = (T_TDPL_KEY) m_wndTdplList.GetItemData(arRet[0]);
	Dlg2Data(TdplD);
	
	m_pDoc->m_pDataCtrl->ModifyTdpl(TdplK, TdplK, TdplD);
	
}


void CStldTendonPrst::DisplayCurSel()
{
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndTdplList,arRet,LVIS_SELECTED);
	int nRet = arRet.GetSize();
	if(nRet != 1) return;

	T_TDPL_K TdplK;

	TdplK.keymap = (T_TDPL_KEY)m_wndTdplList.GetItemData(arRet[0]);

	Data2Dlg(TdplK);
}

LRESULT CStldTendonPrst::OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam)
{
	DisplayCurSel();
	return 0L;
}

void CStldTendonPrst::OnClickTmTprstList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DisplayCurSel();
	*pResult = 0;
}

void CStldTendonPrst::OnTmClose() 
{
	CloseDlg();
}

void CStldTendonPrst::OnTmTprstStressChk() 
{
	UpdateData();
	this->StressOptMan(this->m_nStressOpt);
}

void CStldTendonPrst::OnTmTprstForceChk() 
{
	UpdateData();
	this->StressOptMan(this->m_nStressOpt);
}

void CStldTendonPrst::OnDblclkTmTprstLlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmTprstRightButton(); 
	*pResult = 0;
}

void CStldTendonPrst::OnDblclkTmTprstRlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmTprstLeftButton();
	
	*pResult = 0;
}

void CStldTendonPrst::OnSelchangeTm1stjackCombo() 
{
	UpdateData(TRUE);
	m_n1stJackEnd = m_wnd1stJackCombo.GetCurSel();
}
