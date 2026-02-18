// RatingCaseKRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingCaseKRDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "RatingLiveFactorDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingCaseKRDlg dialog


CRatingCaseKRDlg::CRatingCaseKRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRatingCaseKRDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	//{{AFX_DATA_INIT(CRatingCaseKRDlg)
	//}}AFX_DATA_INIT

	m_pGrid = new CRatingCaseKRGrid;
}

CRatingCaseKRDlg::~CRatingCaseKRDlg()
{
	if(m_pGrid != NULL)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}  
}


void CRatingCaseKRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRatingCaseKRDlg)
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_LIST, m_List);
	DDX_Control(pDX, IDC_DGN_LOAD_CASE_COMBO, m_comboLoadCase);
	DDX_Control(pDX, IDC_DGN_LOAD_CASE_EDIT, m_editLoadCase);	
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_SERVICE_RADIO, m_ratioState1Service);	
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_STRENGTH_RADIO, m_ratioState2Strength);	
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_NAME_EDIT, m_editName);	
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_DESCRIPTION_EDIT, m_editDescription);	
	DDX_Control(pDX, IDC_DGN_IMPACT_FACTOR_EDIT, m_editImpactFactor);	
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_SUB_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRatingCaseKRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CRatingCaseKRDlg)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_ADD_BTN, OnDgnDefineRatingCaseAddBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_DELETE_BTN, OnDgnDefineRatingCaseDeleteBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_DEFINE_RATING_CASE_LIST, OnOnItemchangedDgnRatingCaseList)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_MODIFY_BTN, OnDgnDefineRatingCaseModifyBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_CLOSE, OnDgnDefineRatingCaseClose)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRatingCaseKRDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	// TODO: Add extra initialization here
	T_UNIT_SYSTEM UnitSystem;
	m_editLoadCase.SetUnitType(D_UNITSYS_NONE);	
	m_editImpactFactor.SetUnitType(D_UNITSYS_NONE);	

	SetListCtrlHeader();

	InitKey();

	InitData();
	
	m_pGrid->Initialize(m_pDoc);  
	
	InitLoadCombo();

	m_pGrid->SetData(m_Data);
	LoadDlgMoveLoad();
	LoadDlgLimitState();
	LoadDlgNameDescription();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CRatingCaseKRDlg::TestDataInitialize()
{
	/*
	m_Data.iLimitState = 0;
	m_Data.RatingCaseName = _T("TestCase1");
	m_Data.Description = _T("Test...Test...Test...Test");

	m_Data.PrimaryVeh.AnalType = D_LCOM_MOVING;
	m_Data.PrimaryVeh.LoadCaseKey = 0;
	m_Data.PrimaryVeh.Factor = 1.5;

	m_Data.AdjacentVeh.AnalType = D_LCOM_MOVING;
	m_Data.AdjacentVeh.LoadCaseKey = 0;
	m_Data.AdjacentVeh.Factor = 2.5;

	T_RTLD_D data;

	data.LoadType = D_RCTYPE_DC;
	data.FactorType = 0;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_Data.RateLoad[0] = data;
	
	data.LoadType = D_RCTYPE_DW;
	data.FactorType = 0;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_Data.RateLoad[1] = data;

	data.LoadType = D_RCTYPE_TEMPERATURE;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_Data.RateLoad[2] = data;

	data.LoadType = D_RCTYPE_SECONDARY;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_Data.RateLoad[3] = data;

	data.LoadType = D_RCTYPE_PERMANENT;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_Data.RateLoad[4] = data;

	data.LoadType = D_RCTYPE_USER;
	data.FactorType = 1;
	data.dFactor[0] = 1.0;
	data.dFactor[1] = 1.0;	
	m_Data.RateLoad[5] = data;*/
}

void CRatingCaseKRDlg::InitData()
{
	CArray<T_RKLC_K, T_RKLC_K> arRklcKey;

	m_pDoc->m_pAttrCtrl->GetRklcKeyList(arRklcKey);

	if(arRklcKey.GetSize() > 0)
	{
		for(int i=0 ; i<arRklcKey.GetSize() ; i++)
		{
			InsertListItem(arRklcKey[i]);
		}

		m_pDoc->m_pAttrCtrl->GetRklc(arRklcKey[0], m_Data);
	}
	else
	{
		m_Data.Initialize();
		m_Data.MoveLoadCase.Factor = 1.;
	}
}

void CRatingCaseKRDlg::InitKey()
{
	CArray<T_RKLC_K, T_RKLC_K> arKey;

	m_pDoc->m_pAttrCtrl->GetRklcKeyList(arKey);

	if(arKey.GetSize() > 0)
	{
		int temp = 0;
		for(int i=0 ; i<arKey.GetSize() ; i++)
		{
			if(temp < arKey[i])
				temp = arKey[i];
		}
		m_nNewkey = temp+1;
	}
	else
	{
		m_nNewkey = 1;
	}
}

void CRatingCaseKRDlg::InitLoadCombo()
{
	m_comboLoadCase.ResetContent();
	m_arLoadKey.RemoveAll();
	m_arAnalType.RemoveAll();

	CString item;
	int nRangeStart;

	int nNameLen = 0, nDescLen = 0;
	CSize nTempLen;
	CDC* pDC = GetDC();
	ASSERT(pDC);  

	//Static Load Case
	CArray<T_STLD_K,T_STLD_K> StldKeyList;
	T_STLD_D StldData;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(StldKeyList);
	int nSize = StldKeyList.GetSize();
	for(int i=0 ; i<nSize ; ++i)
	{
		StldData.Initialize();
		m_pDoc->m_pAttrCtrl->GetStld(StldKeyList[i], StldData);
		m_comboLoadCase.AddString(StldData.LoadCaseName);
		m_arLoadKey.Add(StldKeyList[i]);
		m_arAnalType.Add(D_LCOM_STATIC);
	}

	//Moving Load Case
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
		case D_MOVE_CODE_NONE:
			break;
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
			{
				T_MVLD_K MvldKey;
				T_MVLD_D MvldData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvld();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvld(pos,MvldKey,MvldData);
					item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
					m_comboLoadCase.AddString(item);
					m_arLoadKey.Add(MvldKey);
					m_arAnalType.Add(D_LCOM_MOVING);
				}
			}
			break;
		case D_MOVE_CODE_CHINA:
			{
				T_MVLDch_K MvldchKey;
				T_MVLDch_D MvldchData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldch();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldch(pos,MvldchKey,MvldchData);
					item.Format(_T("%s(MV)"), MvldchData.LoadCaseName);
					m_comboLoadCase.AddString(item);
					m_arLoadKey.Add(MvldchKey);
					m_arAnalType.Add(D_LCOM_MOVING);
				}
			}
			break;
		case D_MOVE_CODE_JAPAN:
			{
				// japan version
				T_MVLDjp_K MvldjpKey;
				T_MVLDjp_D MvldjpData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldjp();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldjp(pos,MvldjpKey,MvldjpData);
					item.Format(_T("%s(MV)"), MvldjpData.LoadCaseName);
					m_comboLoadCase.AddString(item);
					m_arLoadKey.Add(MvldjpKey);
					m_arAnalType.Add(D_LCOM_MOVING);
					nTempLen = pDC->GetTextExtent(MvldjpData.LoadCaseName+_T("(MV)"));
					if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
					nTempLen = pDC->GetTextExtent(MvldjpData.Description);
					if (nTempLen.cx > nDescLen) nDescLen = nTempLen.cx;    
				}      
			}
			break;
		case D_MOVE_CODE_INDIA:
			{
				T_MVLDid_K MvldidKey;
				T_MVLDid_D MvldidData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldid();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldid(pos,MvldidKey,MvldidData);
					item.Format(_T("%s(MV)"), MvldidData.LoadCaseName);
					m_comboLoadCase.AddString(item);
					m_arLoadKey.Add(MvldidKey);
					m_arAnalType.Add(D_LCOM_MOVING);
				}
			}
			break;
		case D_MOVE_CODE_BS:
			{
				T_MVLDbs_K MvldbsKey;
				T_MVLDbs_D MvldbsData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldbs();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldbs(pos,MvldbsKey,MvldbsData);
					item.Format(_T("%s(MV)"), MvldbsData.LoadCaseName);
					m_comboLoadCase.AddString(item); 
					m_arLoadKey.Add(MvldbsKey);
					m_arAnalType.Add(D_LCOM_MOVING);
				}
			}
			break;
		case D_MOVE_CODE_FRANCE:
			{
				T_MVLDfr_K MvldfrKey;
				T_MVLDfr_D MvldfrData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldfr();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldfr(pos, MvldfrKey, MvldfrData);
					item.Format(_T("%s(MV)"), MvldfrData.LoadCaseName);
					m_comboLoadCase.AddString(item); 
					m_arLoadKey.Add(MvldfrKey);
					m_arAnalType.Add(D_LCOM_MOVING);
				}
			}
			break;
		case D_MOVE_CODE_TRANS:
			{
				T_MVLDtr_K MvldKey;
				T_MVLDtr_D MvldData;
				nRangeStart = D_STLD_MAXNUM*D_LCOM_MOVING;
				POSITION pos = m_pDoc->m_pAttrCtrl->GetStartMvldtr();
				while(pos != NULL)
				{
					m_pDoc->m_pAttrCtrl->GetNextMvldtr(pos,MvldKey,MvldData);
					item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
					m_comboLoadCase.AddString(item);
					m_arLoadKey.Add(MvldKey);
					m_arAnalType.Add(D_LCOM_MOVING);
				}
			}
			break;
		default:
			ASSERT(0);
			break;
	} 
	
	m_comboLoadCase.SetCurSel(0);	
}

void CRatingCaseKRDlg::LoadDlgMoveLoad()
{
	m_comboLoadCase.SetCurSel(GetLoadCaseKeyIndex(m_Data.MoveLoadCase.LoadCaseKey, m_Data.MoveLoadCase.AnalType));
	m_editLoadCase.SetEditUnit(m_Data.MoveLoadCase.Factor);	
	m_editImpactFactor.SetEditUnit(m_Data.dImpFactor);	
}

int CRatingCaseKRDlg::GetLoadCaseKeyIndex(int Key, int AnalType)
{
	int nSize = m_arLoadKey.GetSize();
	for(int i=0 ; i<nSize ; i++)
	{
		if(m_arLoadKey[i] == Key && m_arAnalType[i] == AnalType)
			return i;
	}
	return 0;
}

void CRatingCaseKRDlg::SaveDlgMoveLoad()
{
	m_Data.MoveLoadCase.AnalType =  m_arAnalType[m_comboLoadCase.GetCurSel()];
	m_Data.MoveLoadCase.LoadCaseKey =  m_arLoadKey[m_comboLoadCase.GetCurSel()];
	m_Data.MoveLoadCase.Factor = m_editLoadCase.GetEditValue();
	m_Data.dImpFactor = m_editImpactFactor.GetEditValue();
}

void CRatingCaseKRDlg::LoadDlgLimitState()
{
	if(m_Data.iRatingCase == 0)
	{//Service rating case
		m_ratioState1Service.SetCheck(TRUE);
		m_ratioState2Strength.SetCheck(FALSE);
	}
	else
	{////Strength rating case
		m_ratioState1Service.SetCheck(FALSE);
		m_ratioState2Strength.SetCheck(TRUE);
	}
}

void CRatingCaseKRDlg::SaveDlgLimitState()
{
	if(m_ratioState1Service.GetCheck())
		m_Data.iRatingCase = 0;//Service rating case
	else
		m_Data.iRatingCase = 1;//Strength rating case
}

void CRatingCaseKRDlg::LoadDlgNameDescription()
{
	m_editName.SetWindowText(m_Data.strCaseName);
	m_editDescription.SetWindowText(m_Data.strDescription);
}

void CRatingCaseKRDlg::SaveDlgNameDescription()
{
	CString str = _T("");
	m_editName.GetWindowText(str);
	m_Data.strCaseName = str;

	m_editDescription.GetWindowText(str);
	m_Data.strDescription = str;
}


////////////////////////////////////////////////////////////////////////////
// CRatingCaseKRDlg message handlers

void CRatingCaseKRDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CRatingCaseKRDlg::OnDgnDefineRatingCaseAddBtn() 
{
	m_pGrid->GetCombItem(m_Data);
	SaveDlgMoveLoad();
	SaveDlgLimitState();
	SaveDlgNameDescription();

	if(RatingCaseNameCheck(m_Data.strCaseName))
	{
		AfxMessageBox(_LS(IDS_DGN_RATING_ERROR_MESSAGE_SAMENAME), MB_OK);
		return;
	}

	if(m_pDoc->m_pDataCtrl->AddRklc(m_nNewkey, m_Data))
	{
		InsertListItem(m_nNewkey);	
		m_nNewkey++;
	}
}



//ListCtrl
void CRatingCaseKRDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_RATING_FACTOR_NAME), _LS(IDS_DGN_RATING_LIMITE_STATE), _LS(IDS_DGN_RATING_DESCRIPTION)};  // _T("Index"), _T("Element List")
	int nColWidth[] = {60, 80, 105};
	int nAlign[] = {LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

//ListCtrl
int CRatingCaseKRDlg::FindInsertionPos(T_RKLC_K nID)
{
	int nCount = m_List.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_RKLC_K key = m_List.GetItemData(i);
		if (nID < key) { return i; }
	}
	return nCount;
}

//ListCtrl
BOOL CRatingCaseKRDlg::InsertListItem(T_RKLC_K Key)
{
	//DB에서 Data를 가져온다

	T_RKLC_D Data;
	Data.Initialize();
	
	m_pDoc->m_pAttrCtrl->GetRklc(Key, Data);
	//Data = m_Data;
	
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 3; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_List.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

//ListCtrl
CString CRatingCaseKRDlg::DataToStr(int i, T_RKLC_K Key, T_RKLC_D &Data)
{  
	CString str;

	if(i == 0) 
		str = Data.strCaseName;
	else if(i == 1)
	{
		if(Data.iRatingCase == 0)
			str = _LS(IDS_DGN_RATING_ALLOWFORCE);
		else
			str = _LS(IDS_DGN_RATING_FORCEDGN);
	}
	else if(i == 2)
		str = Data.strDescription;
	else ASSERT(0);

	return str;
}

//ListCtrl
BOOL CRatingCaseKRDlg::DeleteListItem(T_RKLC_K key)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_List.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;

	m_List.DeleteItem(nItem);

	return TRUE;
}

//ListCtrl
BOOL CRatingCaseKRDlg::ModifyListItem(T_RKLC_K key)
{
	T_RKLC_D Data;

	Data = m_Data;

	if(!m_pDoc->m_pDataCtrl->ModifyRklc(key, key, Data))
		return FALSE;

	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_List.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;

	for(int i=0; i<3; i++)
	{
		CString str = DataToStr(i, key, Data);
		m_List.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem    = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask     = LVIF_PARAM;
	lvitem.lParam   = (LPARAM)key;
	m_List.SetItem(&lvitem);

	return TRUE;
}


void CRatingCaseKRDlg::OnDgnDefineRatingCaseDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aSelItem;
	CArray<T_RKLC_K, T_RKLC_K> arKey;
	aSelItem.RemoveAll();
	arKey.RemoveAll();
	
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i = 0; i < nSize; i++)
	{
		DWORD(key) = m_List.GetItemData(aSelItem[i]);
		arKey.Add(key);	
	}
	for(int i=0 ; i<arKey.GetSize() ; i++)
	{
		//DB에서도 지워야함
		if(m_pDoc->m_pDataCtrl->DelRklc(arKey[i]))
			DeleteListItem(arKey[i]);
	}
}

void CRatingCaseKRDlg::OnOnItemchangedDgnRatingCaseList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}

void CRatingCaseKRDlg::ChangeItem(int nIndex)
{
	// get data
	m_Key = 0;
		
	if (nIndex != -1)
	{
	  m_Key = m_List.GetItemData(nIndex);
		//DB에서 데이터 가져옴
		m_pDoc->m_pAttrCtrl->GetRklc(m_Key, m_Data);

		//TestDataInitialize();//임시
	}

	m_pGrid->SetData(m_Data);
	LoadDlgMoveLoad();
	LoadDlgLimitState();
	LoadDlgNameDescription();
}

void CRatingCaseKRDlg::OnDgnDefineRatingCaseModifyBtn() 
{
	m_pGrid->GetCombItem(m_Data);
	SaveDlgMoveLoad();
	SaveDlgLimitState();
	SaveDlgNameDescription();

	CArray<int, int> aSelItem;
	CArray<T_RKLC_K, T_RKLC_K> arKey;
	aSelItem.RemoveAll();
	arKey.RemoveAll();
	
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i=0 ; i < nSize ; i++)
	{
		DWORD(key) = m_List.GetItemData(aSelItem[i]);
		arKey.Add(key);	
	}

	//선택한것 중에서 가장 처음것에만 Modify한다
	ModifyListItem(arKey[0]);	
}

void CRatingCaseKRDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for(UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}



void CRatingCaseKRDlg::OnDgnDefineRatingCaseClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

BOOL CRatingCaseKRDlg::RatingCaseNameCheck(CString strName) 
{
	BOOL bSame = FALSE;

	CArray<T_RKLC_K, T_RKLC_K> arRklcKey;
	m_pDoc->m_pAttrCtrl->GetRklcKeyList(arRklcKey);
	
	for(int i=0 ; i<arRklcKey.GetSize() ; i++)
	{
		T_RKLC_D RklcData;
		if(m_pDoc->m_pAttrCtrl->GetRklc(arRklcKey[i], RklcData))
		{
			if(RklcData.strCaseName == strName)
			{
				bSame = TRUE;
				return bSame;
			}
		}
	}

	return bSame;
}
