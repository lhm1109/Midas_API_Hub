m_nEvaluation// DgnStlRatingLcomAASHTODlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingLcomAASHTODlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "RatingLiveFactorDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingLcomAASHTODlg dialog


CDgnStlRatingLcomAASHTODlg::CDgnStlRatingLcomAASHTODlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlRatingLcomAASHTODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlRatingLcomAASHTODlg)
	m_nLimitState = 0;
	m_nEvaluation = 0;
	m_nEvaluation19 = 0;
	m_nEvaluation19Fat = 0;
	m_nEvalumethod= 0;
	m_strCaseName = _T("");
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_Key = 0;
	m_Data.Initialize();
	m_RcdsD.Initialize();

	m_pMainGrid = new CDgnStlRatingLcomAASHTOMainGrid;
	m_pSubGrid  = new CDgnStlRatingLcomAASHTOSubGrid;
}

CDgnStlRatingLcomAASHTODlg::~CDgnStlRatingLcomAASHTODlg()
{
	if(m_pMainGrid != NULL)
	{
		delete m_pMainGrid;
		m_pMainGrid = NULL;
	}
	if(m_pSubGrid != NULL)
	{
		delete m_pSubGrid;
		m_pSubGrid = NULL;
	}  
}

void CDgnStlRatingLcomAASHTODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlRatingLcomAASHTODlg)
	DDX_Radio  (pDX, IDC_DGN_DEFINE_RATING_CASE_SERVICE_RADIO,        m_nLimitState);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_PRIMARY_COMBO,        m_cmbPrimary);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_ADJACENT_COMBO,       m_cmbAdjacent);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_FATI_CASE_CMB     ,        m_cmbFatigue);
	
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_PRIMARY_FACTOR_EDIT,  m_edtPrimaryFactor);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_ADJACENT_FACTOR_EDIT, m_edtAdjacentFactor);
	DDX_Radio  (pDX, IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO,             m_nEvaluation);
	DDX_Radio  (pDX, IDC_DGN_DEFINE_RATING19_EVALU_DGN_RDO,           m_nEvaluation19);     // LRFD19 - Serv, Str
	DDX_Radio  (pDX, IDC_DGN_DEFINE_RATING_EVALU_FATI_LRFD_RDO,       m_nEvaluation19Fat);  // LRFD19 - Fatigue

	DDX_Radio  (pDX, IDC_DGN_DEFINE_RATING_EVALU_METHOD_FATI_REFN_RDO,m_nEvalumethod); // LRFD19

	DDX_Text   (pDX, IDC_DGN_DEFINE_RATING_CASE_NAME_EDIT,            m_strCaseName);
	DDX_Text   (pDX, IDC_DGN_DEFINE_RATING_CASE_DESCRIPTION_EDIT,     m_strDescription);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_LIST,                 m_List);

	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_MAIN_GRID, *m_pMainGrid);
	DDX_Control(pDX, IDC_DGN_DEFINE_RATING_CASE_SUB_GRID, *m_pSubGrid);
	//}}AFX_DATA_MAP
}

void CDgnStlRatingLcomAASHTODlg::InitCtrl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX, nDistY;
	CArray<UINT, UINT> aControls, aControls19;

	// Live Load Combination
	m_aLiveControls.RemoveAll();
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_CASE_STATIC);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_CASE_PRIMARY_COMBO);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_CASE_PRIMARY_FACTOR_EDIT);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_CASE_STATIC2);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_CASE_ADJACENT_COMBO);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_CASE_ADJACENT_FACTOR_EDIT);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_EVALU_GRP);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO);
	m_aLiveControls.Add(IDC_DGN_DEFINE_RATING_EVALU_LEGAL_RDO);

	m_aLiveControls19.RemoveAll();
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_FATI_CASE_STC);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_FATI_CASE_CMB);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_FATI_CASE_FACTOR_EDT);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_EVALU_FATI_GRP);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_EVALU_FATI_LRFD_RDO);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_EVALU_FATI_SURV_RDO);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_EVALU_METHOD_FATI_GRP);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_EVALU_METHOD_FATI_REFN_RDO);
	m_aLiveControls19.Add(IDC_DGN_DEFINE_RATING_EVALU_METHOD_FATI_SIMP_RDO);

	m_aEVLcontrols.RemoveAll();
	m_aEVLcontrols.Add(IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO);
	m_aEVLcontrols.Add(IDC_DGN_DEFINE_RATING_EVALU_LEGAL_RDO);

	m_aEVLcontrols19.RemoveAll();
	m_aEVLcontrols19.Add(IDC_DGN_DEFINE_RATING19_EVALU_DGN_RDO);
	m_aEVLcontrols19.Add(IDC_DGN_DEFINE_RATING19_EVALU_LEGAL_RDO);
	m_aEVLcontrols19.Add(IDC_DGN_DEFINE_RATING19_EVALU_PERMIT_RDO);

	m_RcdsD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnRcds(m_RcdsD);

	// evaluation load model
	if(m_RcdsD.nRatingCode == STL_AASHTO_LRFD19)
	{
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_DGN_RDO)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_DEFINE_RATING19_EVALU_DGN_RDO)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top - globalUtils.ScaleByDPI(1);
		CDlgUtil::CtrlMoveDistXY(this, m_aEVLcontrols19, nDistX, nDistY);
	}
}

void CDgnStlRatingLcomAASHTODlg::InitUnit()
{
	m_edtPrimaryFactor .SetUnitType(D_UNITSYS_NONE);
	m_edtAdjacentFactor.SetUnitType(D_UNITSYS_NONE);
}

void CDgnStlRatingLcomAASHTODlg::InitRlcsKey()
{
	CArray<T_RLCS_K, T_RLCS_K> arRlcsKey;
	m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(arRlcsKey);
	
	if(arRlcsKey.GetSize() > 0)
	{
		int temp = 0;
		for(int i=0; i<arRlcsKey.GetSize(); ++i)
		{
			if(temp < arRlcsKey[i])
				temp = arRlcsKey[i];
		}
		m_Key = temp+1;
	}
	else
	{
		m_Key = 1;
	}
}

void CDgnStlRatingLcomAASHTODlg::InitCurrentData()
{
	CArray<T_RLCS_K, T_RLCS_K> arRlcsKey;
	m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(arRlcsKey);
	m_Data.Initialize();
	
	if(arRlcsKey.GetSize() > 0)
	{
		for(int i=0; i<arRlcsKey.GetSize(); ++i)
		{
			InsertListItem(arRlcsKey[i]);
		}
		
		m_pDoc->m_pAttrCtrl2->GetRlcs(arRlcsKey[0], m_Data);
	}
}

void CDgnStlRatingLcomAASHTODlg::InitMoveLoadCombo()
{
	m_cmbPrimary.ResetContent();
	m_cmbAdjacent.ResetContent();
	m_cmbFatigue.RedrawWindow();
	m_arMoveLoadKey.RemoveAll();

	CString item;
	int nRangeStart;

	int nNameLen = 0, nDescLen = 0;
	CSize nTempLen;
	CDC* pDC = GetDC();
	ASSERT(pDC);  

	//D_LCOM_MOVING		  4 : 이동 하중 조건
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
				m_cmbPrimary.AddString(item);
				m_cmbAdjacent.AddString(item);
				m_cmbFatigue.AddString(item);
				m_arMoveLoadKey.Add(MvldKey);
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
				m_cmbPrimary.AddString(item);
				m_cmbAdjacent.AddString(item);
				m_cmbFatigue.AddString(item);
				m_arMoveLoadKey.Add(MvldchKey);
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
				m_cmbPrimary.AddString(item);
				m_cmbAdjacent.AddString(item);
				m_cmbFatigue.AddString(item);
				m_arMoveLoadKey.Add(MvldjpKey);
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
				m_cmbPrimary.AddString(item);
				m_cmbAdjacent.AddString(item);
				m_cmbFatigue.AddString(item);
				m_arMoveLoadKey.Add(MvldidKey);
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
				m_cmbPrimary.AddString(item);
				m_cmbAdjacent.AddString(item); 
				m_cmbFatigue.AddString(item);
				m_arMoveLoadKey.Add(MvldbsKey);
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
				m_cmbPrimary.AddString(item);
				m_cmbAdjacent.AddString(item); 
				m_cmbFatigue.AddString(item);
				m_arMoveLoadKey.Add(MvldfrKey);
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
				m_cmbPrimary.AddString(item);
				m_cmbAdjacent.AddString(item);
				m_cmbFatigue.AddString(item);
				m_arMoveLoadKey.Add(MvldKey);
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	} 

	m_cmbPrimary.SetCurSel(0);
	m_cmbAdjacent.SetCurSel(0);
	m_cmbFatigue.SetCurSel(0);
}

void CDgnStlRatingLcomAASHTODlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_RATING_FACTOR_NAME), _LS(IDS_DGN_RATING_LIMITE_STATE), _LS(IDS_DGN_RATING_DESCRIPTION)};  // _T("Index"), _T("Element List")
	int nColWidth[] = {70, 70, 118};
	int nAlign[] = {LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	for(int i=0; i<nColNum; ++i)
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

BOOL CDgnStlRatingLcomAASHTODlg::InsertListItem(T_RLCS_K Key)
{
	T_RLCS_D Data; Data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRlcs(Key, Data);
	
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();
	
	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i<3; ++i)
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

BOOL CDgnStlRatingLcomAASHTODlg::ModifyListItem(T_RLCS_K key)
{
	T_RLCS_D Data;
	
	Data = m_Data;
	
	if(!m_pDoc->m_pDataCtrl->ModifyRlcs(key, key, Data))
		return FALSE;
	
	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;
	
	int nItem = m_List.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;
	
	for(int i=0; i<3; ++i)
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

BOOL CDgnStlRatingLcomAASHTODlg::DeleteListItem(T_RLCS_K key)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;
	
	int nItem = m_List.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;
	
	m_List.DeleteItem(nItem);
	
	return TRUE;
}

int CDgnStlRatingLcomAASHTODlg::FindInsertionPos(T_RLCS_K nID)
{
	int nCount = m_List.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_RLCS_K key = m_List.GetItemData(i);
        if ( nID < key ) { return i; }
	}
	return nCount;
}

CString CDgnStlRatingLcomAASHTODlg::DataToStr(int i, T_RLCS_K Key, T_RLCS_D &Data)
{  
	CString str;
	
	if(i == 0) 
		str = Data.strCaseName;
	else if(i == 1)
	{
		if(Data.nLimitState == 0)
			str = _LS(IDS_DGN_RATING_SERVICE);
		else if(Data.nLimitState == 1)
			str = _LS(IDS_DGN_RATING_STRENGTH);
		else
			str = _LS(IDS_DGN_RATING_FATIGUE);
	}
	else if(i == 2)
		str = Data.strDescription;
	else ASSERT(0);
	
	return str;
}

void CDgnStlRatingLcomAASHTODlg::ChangeItem(int nIndex)
{
	// get data
	T_RLCS_K RlcsK;
	
	if (nIndex != -1)
	{
		RlcsK = m_List.GetItemData(nIndex);
		//DB에서 데이터 가져옴
		m_pDoc->m_pAttrCtrl2->GetRlcs(RlcsK, m_Data);
	}

	m_nLimitState = m_Data.nLimitState;
	ShowhideGrid();
	AlignControls();
	
	m_pMainGrid->SetRtldData(m_Data.RateLoad);

	Data2Dlg();

}

void CDgnStlRatingLcomAASHTODlg::ShowhideGrid()
{
	// 선택된 아이템에 따른 grid show / hide
	BOOL bEnableCtrl = TRUE;
	if(m_RcdsD.nRatingCode ==STL_AASHTO_LRFD19) bEnableCtrl = TRUE;
	else
	{
		if (m_nLimitState == 2) bEnableCtrl = FALSE;
	}
	if (m_pMainGrid) m_pMainGrid->ShowWindow(bEnableCtrl);
	if (m_pSubGrid) m_pSubGrid->ShowWindow(bEnableCtrl);
	//
}

void CDgnStlRatingLcomAASHTODlg::AlignControls()
{
	// Move Ctrl
	CRect rRef;
	CRect rToMove;
	int nDistX, nDistY;
	CArray<UINT, UINT> aControls;

	// Load Combination GROUP resize
	CRect rectResize;
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_STATIC_GRUP)->GetWindowRect(rectResize);
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_MAIN_GRID)->GetWindowRect(rRef);
	if(m_RcdsD.nRatingCode ==STL_AASHTO_LRFD19)
		rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(5);
	else
	{
		if (m_nLimitState == 2) rectResize.bottom = rRef.top;        // Fatigue
		else                    rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(5);
	}
	ScreenToClient(rectResize);
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_STATIC_GRUP)->MoveWindow(rectResize);

	// Live load Combination
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DEFINE_RATING_FATI_CASE_STC)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aLiveControls19, nDistX, nDistY);
	if(m_RcdsD.nRatingCode ==STL_AASHTO_LRFD19)
	{
		CDlgUtil::CtrlShowHide(this, m_aLiveControls  , m_nLimitState != 2);
		CDlgUtil::CtrlShowHide(this, m_aLiveControls19, m_nLimitState == 2);

		CDlgUtil::CtrlShowHide(this, m_aEVLcontrols,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aEVLcontrols19, m_nLimitState != 2);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aLiveControls  , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aLiveControls19, FALSE);

		CDlgUtil::CtrlShowHide(this, m_aEVLcontrols  , TRUE);
		CDlgUtil::CtrlShowHide(this, m_aEVLcontrols19, FALSE);

	}
	GetDlgItem(IDC_DGN_DEFINE_RATING_FATI_CASE_FACTOR_EDT)->ShowWindow(SW_HIDE); // 사용 안함.

	// Live Load Combination resize
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_LIVE_GRUP)->GetWindowRect(rectResize);
	if(m_RcdsD.nRatingCode ==STL_AASHTO_LRFD19)
	{
		if (m_nLimitState == 2) 
			GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_METHOD_FATI_GRP)->GetWindowRect(rRef);
		else 
			GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP)->GetWindowRect(rRef);
	}
	else
		GetDlgItem(IDC_DGN_DEFINE_RATING_EVALU_GRP)->GetWindowRect(rRef);

	rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rectResize);
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_LIVE_GRUP)->MoveWindow(rectResize);

	// Live Load Combination Move
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_LIVE_GRUP);
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_LIVE_FACTORS_BTN);
	aControls.Append(m_aLiveControls);
	aControls.Append(m_aEVLcontrols19);
	aControls.Append(m_aLiveControls19);
	//CDlgUtil::GetCtrlIDByIncRect(this, aControls,  IDC_DGN_DEFINE_RATING_CASE_LIVE_GRUP,  TRUE);
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_STATIC_GRUP )->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_LIVE_GRUP )->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	// 나머지
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_STATIC3);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_NAME_EDIT);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_STATIC4);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_DESCRIPTION_EDIT);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_LIST);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_ADD_BTN);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_MODIFY_BTN);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_DELETE_BTN);  	
		
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_LIVE_GRUP )->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_NAME_EDIT )->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	aControls.RemoveAll();
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_COPY_BTN);  
	aControls.Add(IDC_DGN_DEFINE_RATING_CASE_CLOSE);  

	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_LIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_CLOSE)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	//////////////////////////////////////////////////////////////////////////
	// resize dialog
	UINT nLastCtrlID = IDC_DGN_DEFINE_RATING_CASE_CLOSE;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
	//////////////////////////////////////////////////////////////////////////

	// list에 잔상이 남아서 한번더 그려준다
	this->RedrawWindow();
}

void CDgnStlRatingLcomAASHTODlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for(UINT i=0; i<uSelectedCount; ++i)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CDgnStlRatingLcomAASHTODlg::Data2Dlg()
{
	m_nLimitState = m_Data.nLimitState;
	
	m_cmbPrimary.SetCurSel(GetMoveLoadKeyIndex(m_Data.PrimaryVeh.LoadCaseKey));
	m_edtPrimaryFactor.SetEditUnit(m_Data.PrimaryVeh.Factor);
	
	m_cmbAdjacent.SetCurSel(GetMoveLoadKeyIndex(m_Data.AdjacentVeh.LoadCaseKey));
	m_edtAdjacentFactor.SetEditUnit(m_Data.AdjacentVeh.Factor);

	m_cmbFatigue.SetCurSel(GetMoveLoadKeyIndex(m_Data.FatigueVeh.LoadCaseKey));

	if(m_RcdsD.nRatingCode ==STL_AASHTO_LRFD19 && (m_nLimitState == 0 || m_nLimitState == 1))
	{
		m_nEvaluation19    = m_Data.nEvaluation;
	}
	else
	{
		m_nEvaluation      = (m_Data.nEvaluation==2)? 1 : m_Data.nEvaluation;
		m_nEvaluation19Fat = (m_Data.nEvaluation==2)? 1 : m_Data.nEvaluation;
	}
	m_nEvalumethod= m_Data.nEvaluMethod;

	m_strCaseName = m_Data.strCaseName;
	m_strDescription = m_Data.strDescription;
	
	UpdateData(FALSE);
}

void CDgnStlRatingLcomAASHTODlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();
	
	m_Data.nLimitState = m_nLimitState;
	
	m_Data.PrimaryVeh.AnalType = D_LCOM_MOVING;
	if(m_arMoveLoadKey.GetSize() > 0)
		m_Data.PrimaryVeh.LoadCaseKey = m_arMoveLoadKey[m_cmbPrimary.GetCurSel()];
	m_Data.PrimaryVeh.Factor = m_edtPrimaryFactor.GetEditValue();
	
	m_Data.AdjacentVeh.AnalType = D_LCOM_MOVING;
	if(m_arMoveLoadKey.GetSize() > 0)
		m_Data.AdjacentVeh.LoadCaseKey = m_arMoveLoadKey[m_cmbAdjacent.GetCurSel()];
	m_Data.AdjacentVeh.Factor = m_edtAdjacentFactor.GetEditValue();
	
	m_Data.FatigueVeh.AnalType = D_LCOM_MOVING;
	if(m_arMoveLoadKey.GetSize() > 0)
		m_Data.FatigueVeh.LoadCaseKey = m_arMoveLoadKey[m_cmbFatigue.GetCurSel()];

	if(m_RcdsD.nRatingCode ==STL_AASHTO_LRFD19)
	{
		if(m_nLimitState==2) //Fat
			m_Data.nEvaluation = m_nEvaluation19Fat;
		else
			m_Data.nEvaluation = m_nEvaluation19;
	}
	else
	{
		m_Data.nEvaluation = m_nEvaluation;
	}

	m_Data.nEvaluMethod = m_nEvalumethod;
	
	m_Data.strCaseName = m_strCaseName;
	m_Data.strDescription = m_strDescription;
}

int CDgnStlRatingLcomAASHTODlg::GetMoveLoadKeyIndex(T_MVLD_K MvKey)
{
	for(int i=0; i<m_arMoveLoadKey.GetSize(); ++i)
	{
		if(m_arMoveLoadKey[i] == MvKey)
			return i;
	}
	
	return 0;
}

BOOL CDgnStlRatingLcomAASHTODlg::RatingCaseNameCheck(CString strName) 
{
	BOOL bSame = FALSE;
	
	CArray<T_RLCS_K, T_RLCS_K> arRlcsKey;
	m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(arRlcsKey);
	
	for(int i=0; i<arRlcsKey.GetSize(); ++i)
	{
		T_RLCS_D RlcsData; RlcsData.Initialize();
		if(m_pDoc->m_pAttrCtrl2->GetRlcs(arRlcsKey[i], RlcsData))
		{
			if(RlcsData.strCaseName == strName)
			{
				bSame = TRUE;
				return bSame;
			}
		}
	}
	
	return bSame;
}


BEGIN_MESSAGE_MAP(CDgnStlRatingLcomAASHTODlg, CDialogMove)
//{{AFX_MSG_MAP(CDgnStlRatingLcomAASHTODlg)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_SERVICE_RADIO,     OnDgnStlRatingLcomLimitStateRdo)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_STRENGTH_RADIO,    OnDgnStlRatingLcomLimitStateRdo)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_FATIGUE_RADIO,     OnDgnStlRatingLcomLimitStateRdo)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_LIVE_FACTORS_BTN,  OnDgnStlRatingLcomLiveFactorBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_ADD_BTN,           OnDgnStlRatingLcomAddBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_MODIFY_BTN,        OnDgnStlRatingLcomModBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_DELETE_BTN,        OnDgnStlRatingLcomDelBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_COPY_BTN,          OnDgnStlRatingLcomCopyBtn)
	ON_BN_CLICKED(IDC_DGN_DEFINE_RATING_CASE_CLOSE,             OnDgnStlRatingLcomClose)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_DEFINE_RATING_CASE_LIST, OnOnItemchangedDgnStlRatingLcomList)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingLcomAASHTODlg message handlers
BOOL CDgnStlRatingLcomAASHTODlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CCheckDialogOpen::SetSoftwareRendering();

	// TODO: Add extra initialization here
	
	SetListCtrlHeader();

	InitCtrl();

	InitUnit();
	InitRlcsKey();
	InitCurrentData();
	InitMoveLoadCombo();
	
	m_pMainGrid->Initialize(m_Data.RateLoad, this);
	m_pSubGrid->Initialize(m_pDoc);
	m_pMainGrid->m_pSubGrid = m_pSubGrid;

	m_pMainGrid->LoadDlgData();
	m_pSubGrid->SetData(m_Data.RateLoad[0]);

	Data2Dlg();

	OnDgnStlRatingLcomLimitStateRdo();

	// Test
	GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_COPY_BTN)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Add by sshan MNET:XXXX  '20060818 
// Radio 버튼을 Strength, Service로 선택함에 따라 Factor의 기본값을 변경 시켜줌
void CDgnStlRatingLcomAASHTODlg::OnDgnStlRatingLcomLimitStateRdo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_nLimitState == 0)
	{
		m_Data.RateLoad[0].dFactor[0] = 1.0;
		m_Data.RateLoad[0].dFactor[1] = 1.0;
		
		m_Data.RateLoad[1].dFactor[0] = 1.0;
		m_Data.RateLoad[1].dFactor[1] = 1.0;

		m_Data.RateLoad[2].dFactor[0] = 1.0;
		m_Data.RateLoad[2].dFactor[1] = 1.0;
		
		m_Data.RateLoad[3].dFactor[0] = 1.0;
		m_Data.RateLoad[4].dFactor[0] = 1.0;
		m_Data.RateLoad[5].dFactor[0] = 1.0;
		m_Data.RateLoad[6].dFactor[0] = 1.0;
		m_Data.RateLoad[7].dFactor[0] = 1.0;
	}
	else if(m_nLimitState == 1)
	{
		m_Data.RateLoad[0].dFactor[0] = 1.25;
		m_Data.RateLoad[0].dFactor[1] = 0.9;
		
		m_Data.RateLoad[1].dFactor[0] = 1.25;
		m_Data.RateLoad[1].dFactor[1] = 0.9;

		m_Data.RateLoad[2].dFactor[0] = 1.5;
		m_Data.RateLoad[2].dFactor[1] = 0.9;
		
		m_Data.RateLoad[3].dFactor[0] = 1.0;
		m_Data.RateLoad[4].dFactor[0] = 1.0;
		m_Data.RateLoad[5].dFactor[0] = 1.25;
		m_Data.RateLoad[6].dFactor[0] = 1.25;
		m_Data.RateLoad[7].dFactor[0] = 1.0;
	}
	else
	{
		m_Data.RateLoad[0].dFactor[0] = 1.0;
		m_Data.RateLoad[0].dFactor[1] = 1.0;
		
		m_Data.RateLoad[1].dFactor[0] = 1.0;
		m_Data.RateLoad[1].dFactor[1] = 1.0;
		
		m_Data.RateLoad[2].dFactor[0] = 1.0;
		m_Data.RateLoad[2].dFactor[1] = 1.0;
		
		m_Data.RateLoad[3].dFactor[0] = 1.0;
		m_Data.RateLoad[4].dFactor[0] = 1.0;
		m_Data.RateLoad[5].dFactor[0] = 1.0;
		m_Data.RateLoad[6].dFactor[0] = 1.0;
		m_Data.RateLoad[7].dFactor[0] = 1.0;
	}
	
	// MNET:XXXX-HSSHIM-20140710. 4845-32. 
	// Disable 시켜달라는 요청이었으나, 
	// Grid Control의 Disable시 기능은 Disable되어도 Grid 표시가 Disable이 되지 않아서 그냥 Hide 시킴.
// 	BOOL bEnableCtrl = TRUE;
// 	if (m_nLimitState == 2) bEnableCtrl = FALSE;
// 	if (m_pMainGrid) m_pMainGrid->ShowWindow(bEnableCtrl);
// 	if (m_pSubGrid) m_pSubGrid->ShowWindow(bEnableCtrl);
	ShowhideGrid();
	AlignControls();

	m_pMainGrid->SetRtldData(m_Data.RateLoad);	
}

void CDgnStlRatingLcomAASHTODlg::OnDgnStlRatingLcomLiveFactorBtn() 
{
	UpdateData(TRUE);
	
	RatingLiveFactorDlg dlg;
	dlg.m_iLimitState = m_nLimitState;
	dlg.m_nRatingType = 0;
	
	if(dlg.DoModal() == IDOK)
	{
		m_edtPrimaryFactor.SetEditUnit(dlg.m_dPrimary);
		m_edtAdjacentFactor.SetEditUnit(dlg.m_dAdjacent);
		
		UpdateData(FALSE);
	}
}

void CDgnStlRatingLcomAASHTODlg::OnDgnStlRatingLcomAddBtn() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();
	
	m_pMainGrid->SaveCurrentGrid();
	for(int i=0; i<8; ++i)
	{
		m_Data.RateLoad[i] = m_pMainGrid->RateLoad[i];
	}
	
	if(RatingCaseNameCheck(m_Data.strCaseName))
	{
		AfxMessageBox(_LS(IDS_DGN_RATING_ERROR_MESSAGE_SAMENAME), MB_OK);
		return;
	}
	
	if(m_pDoc->m_pDataCtrl->AddRlcs(m_Key, m_Data))
	{
		InsertListItem(m_Key);	
		m_Key++;
	}
}

void CDgnStlRatingLcomAASHTODlg::OnDgnStlRatingLcomModBtn() 
{
	// TODO: Add your control notification handler code here
	Dlg2Data();
	
	m_pMainGrid->SaveCurrentGrid();
	for(int i=0; i<8; ++i)
	{
		m_Data.RateLoad[i] = m_pMainGrid->RateLoad[i];
	}
	
	CArray<int, int> aSelItem;
	CArray<T_RLCS_K, T_RLCS_K> arKey;
	aSelItem.RemoveAll();
	arKey.RemoveAll();
	
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	
	for(int i=0; i<nSize; ++i)
	{
		DWORD(key) = m_List.GetItemData(aSelItem[i]);
		arKey.Add(key);	
	}
	
	//선택한것 중에서 가장 처음것에만 Modify한다
	ModifyListItem(arKey[0]);
}

void CDgnStlRatingLcomAASHTODlg::OnDgnStlRatingLcomDelBtn() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aSelItem;
	CArray<T_RLCS_K, T_RLCS_K> arKey;
	aSelItem.RemoveAll();
	arKey.RemoveAll();
	
	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	
	for(int i=0; i<nSize; ++i)
	{
		DWORD(key) = m_List.GetItemData(aSelItem[i]);
		arKey.Add(key);	
	}
	for(int i=0; i<arKey.GetSize(); ++i)
	{
		//DB에서도 지워야함
		if(m_pDoc->m_pDataCtrl->DelRlcs(arKey[i]))
			DeleteListItem(arKey[i]);
	}
}

void CDgnStlRatingLcomAASHTODlg::OnDgnStlRatingLcomCopyBtn() 
{


	// Steel Composite 으로 복사 

}

void CDgnStlRatingLcomAASHTODlg::OnDgnStlRatingLcomClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CDgnStlRatingLcomAASHTODlg::OnOnItemchangedDgnStlRatingLcomList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}

void CDgnStlRatingLcomAASHTODlg::EnableDisableSubGrid(int nRow)
{
	//GetDlgItem(IDC_DGN_DEFINE_RATING_CASE_SUB_GRID)->EnableWindow(nRow!=1);
}
