#include "stdafx.h"

#include "TreeWorkDefine.h"
#include "ReportUnitCustomizeDlg.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\UndoCtrl.h"


#include "..\wg_base\wg_base_DlgUtil.h"

/*
	CArray 형태의 문자열 배열 Sorting용 함수.
*/
int MDCompareStringArray( const void *arg1, const void *arg2 )
{
	/* Compare all of both strings: */
	return _stricmp( * ( char** ) arg1, * ( char** ) arg2 );
}

const int TYPE_UTBL=0, TYPE_UCHT=1, TYPE_SECTSUMM=2, TYPE_REINFORC=3, TYPE_COMPSECT=4;

/////////////////////////////////////////////////////////////////////////////
// CReportUnitCustomizeDlg dialog

CReportUnitCustomizeDlg::CReportUnitCustomizeDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CReportUnitCustomizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportUnitCustomizeDlg)
	//}}AFX_DATA_INIT
	m_nMode = WORK_DYUN_FLD;
	m_nSystemRdo = 0;
	m_bApplyAll = FALSE;

	m_aCtrlID_SetUnit.RemoveAll();
	m_aCtrlID_SetUnit.Add(IDC_TM_REPORT_UNIT_LENG_CMB);
	m_aCtrlID_SetUnit.Add(IDC_TM_REPORT_UNIT_FORC_CMB);
	m_aCtrlID_SetUnit.Add(IDC_TM_REPORT_UNIT_HEAT_CMB);
	m_aCtrlID_SetUnit.FreeExtra();
}

CReportUnitCustomizeDlg::~CReportUnitCustomizeDlg()
{
}

void CReportUnitCustomizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportUnitCustomizeDlg)
	DDX_Control(pDX, IDC_TM_REPORT_UNIT_ITEM_TAB      , m_tabItem   );
	DDX_Control(pDX, IDC_TM_REPORT_UNIT_ITEM_LST      , m_List      );
	DDX_Radio  (pDX, IDC_TM_REPORT_UNIT_CURSYS_RDO    , m_nSystemRdo);
	DDX_Control(pDX, IDC_TM_REPORT_UNIT_LENG_CMB      , m_cmbLeng   );
	DDX_Control(pDX, IDC_TM_REPORT_UNIT_FORC_CMB      , m_cmbForc   );
	DDX_Control(pDX, IDC_TM_REPORT_UNIT_HEAT_CMB      , m_cmbHeat   );
	DDX_Check  (pDX, IDC_TM_REPORT_UNIT_APPLY_ALL_CHK , m_bApplyAll );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportUnitCustomizeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CReportUnitCustomizeDlg)
	ON_BN_CLICKED(IDC_TM_REPORT_UNIT_CURSYS_RDO, OnUnitSysRdo)
	ON_BN_CLICKED(IDC_TM_REPORT_UNIT_DEFSYS_RDO, OnUnitSysRdo)
	ON_BN_CLICKED(IDC_TM_REPORT_UNIT_DEFAULT_ALL_BTN, OnDefaultAllBtn)
	ON_BN_CLICKED(IDC_TM_REPORT_UNIT_APPLY_BTN, OnApplyBtn)
	ON_BN_CLICKED(IDC_TM_REPORT_UNIT_APPLY_ALL_CHK, OnApplyAllChk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TM_REPORT_UNIT_ITEM_TAB, OnSelchangeTab)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_REPORT_UNIT_ITEM_LST, OnItemchangeLst)
	ON_CBN_SELCHANGE(IDC_TM_REPORT_UNIT_LENG_CMB, OnComboBoxSelChange)	
	ON_CBN_SELCHANGE(IDC_TM_REPORT_UNIT_FORC_CMB, OnComboBoxSelChange)	
	ON_CBN_SELCHANGE(IDC_TM_REPORT_UNIT_HEAT_CMB, OnComboBoxSelChange)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportUnitCustomizeDlg message handlers

BOOL CReportUnitCustomizeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(m_UnitIdxCur);

	m_tabItem.InsertItem(0, _LS(IDS_TM_DYNA_REPORT_Tables));
	m_tabItem.InsertItem(1, _LS(IDS_TM_DYNA_REPORT_Charts));
	m_tabItem.SetCurSel((m_nMode==WORK_UCHT_FLD || m_nMode==WORK_UCHT_LST)? 1:0); // UCHT일 때만 Index 1
	InitComboBox();
	MakeData();
	SetListCtrlHeader();
	SetListCtrlItem();

	// 트리에서 선택한 아이템으로 대화상자 세팅
	int nIndex=0;
	if(m_nMode==WORK_UTBL_LST || m_nMode==WORK_UCHT_LST)
	{
		for(int i=0; i<m_aData.GetSize(); i++)
		{
			if(m_aData[i].nKey == m_Key) nIndex = i;
		}
	}
	else if(m_nMode==WORK_USSS_LST)
	{
		for(int i=0; i<m_aData.GetSize(); i++)
		{
			if(m_aData[i].nType == TYPE_SECTSUMM) nIndex = i;
		}
	}
	else if(m_nMode==WORK_USRS_LST)
	{
		for(int i=0; i<m_aData.GetSize(); i++)
		{
			if(m_aData[i].nType == TYPE_REINFORC) nIndex = i;
		}
	}
	else if(m_nMode==WORK_USCS_LST)
	{
		for(int i=0; i<m_aData.GetSize(); i++)
		{
			if(m_aData[i].nType == TYPE_COMPSECT) nIndex = i;
		}
	}

	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	Data2Dlg(nIndex);

	return TRUE;
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CReportUnitCustomizeDlg::OnOK() 
{
	Dlg2Data();
	Apply();

	CDialogMove::OnOK();
}

void CReportUnitCustomizeDlg::Dlg2Data()
{
	int nSelected;
	for(int i=-1; i<m_List.GetItemCount()-1; i++)
	{
		nSelected = m_List.GetNextItem(i, LVNI_ALL | LVNI_SELECTED);
		if(nSelected>-1) Dlg2Data(nSelected);
	}
}

void CReportUnitCustomizeDlg::Dlg2Data(int nIndex)
{
	UpdateData(TRUE);
	if(nIndex<0) return; 

	m_aData[nIndex].nUnitSys = m_nSystemRdo;
	//m_aData[nIndex].bApplyAll = m_bApplyAll;
	m_aData[nIndex].UnitIdx.nBase_Length = m_cmbLeng.GetCurSel();
	m_aData[nIndex].UnitIdx.nBase_Force  = m_cmbForc.GetCurSel();
	m_aData[nIndex].UnitIdx.nBase_Heat   = m_cmbHeat.GetCurSel();
//  m_aData[nIndex].UnitIdx.nBase_Temper = m_cmbTemp.GetCurSel();
}

void CReportUnitCustomizeDlg::Data2Dlg()
{
	Data2Dlg(m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED));
}

void CReportUnitCustomizeDlg::Data2Dlg(int nIndex)
{
	if(nIndex<0) return; 

	T_UNIT_CUSTOM_DATA Data = m_aData[nIndex];;
	
	m_cmbLeng.SetCurSel(Data.UnitIdx.nBase_Length);
	m_cmbForc.SetCurSel(Data.UnitIdx.nBase_Force );
	m_cmbHeat.SetCurSel(Data.UnitIdx.nBase_Heat  );
	m_nSystemRdo = Data.nUnitSys;
	
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlID_SetUnit, Data.nUnitSys==TRUE);

	UpdateData(FALSE);
}

void CReportUnitCustomizeDlg::InitComboBox()
{
	int idx = 0;
	m_cmbLeng.ResetContent();
	m_cmbLeng.AddString(_LS(IDS_MIT_UNIT_MM)); m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbLeng.AddString(_LS(IDS_MIT_UNIT_CM)); m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbLeng.AddString(_LS(IDS_MIT_UNIT_M));  m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbLeng.AddString(_LS(IDS_MIT_UNIT_IN)); m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbLeng.AddString(_LS(IDS_MIT_UNIT_FT)); m_cmbLeng.SetItemData(idx, idx);

	idx = 0;
	m_cmbForc.ResetContent();
	m_cmbForc.AddString(_LS(IDS_MIT_UNIT_KG));   m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbForc.AddString(_LS(IDS_MIT_UNIT_TON));  m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbForc.AddString(_LS(IDS_MIT_UNIT_N));    m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbForc.AddString(_LS(IDS_MIT_UNIT_KN));   m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbForc.AddString(_LS(IDS_MIT_UNIT_LBF));  m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbForc.AddString(_LS(IDS_MIT_UNIT_KIPS)); m_cmbLeng.SetItemData(idx, idx);

	idx = 0;
	m_cmbHeat.ResetContent();
	m_cmbHeat.AddString(_LS(IDS_MIT_UNIT_CAL)); m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbHeat.AddString(_LS(IDS_MIT_UNIT_KCAL));m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbHeat.AddString(_LS(IDS_MIT_UNIT_J));   m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbHeat.AddString(_LS(IDS_MIT_UNIT_KJ));  m_cmbLeng.SetItemData(idx, idx); idx++;
	m_cmbHeat.AddString(_LS(IDS_MIT_UNIT_BTU)); m_cmbLeng.SetItemData(idx, idx);
}

void CReportUnitCustomizeDlg::SetListCtrlHeader()
{
	m_List.DeleteAllItems();

	CString aTitle[] = { _LS(IDS_WG_TREEMENU_Name) };
	int nColWidth[] = { 190 };
	int nAlign[] = { LVCFMT_LEFT };
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;
	int i;
	LV_COLUMN lvcolumn;
	
	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	// Set Title
	for(i = 0; i < nColNum; i++)
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

// ListCtrl 데이터 생성
void CReportUnitCustomizeDlg::SetListCtrlItem()
{
	for(int i=0; i<m_aData.GetSize(); i++)
	{
		m_List.InsertItem(i, _T(""));
		m_List.SetItemText(i, 0, m_aData[i].strName);
	}
}

void CReportUnitCustomizeDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Dlg2Data();

	if(m_tabItem.GetCurSel()==0)
	{
		m_aData_Chart.Copy(m_aData);
		m_aData.Copy(m_aData_Table);
	}
	else
	{
		m_aData_Table.Copy(m_aData);
		m_aData.Copy(m_aData_Chart);
	}

	// 초기화
	m_bApplyAll = FALSE;
	m_nSystemRdo = 0;

	m_List.DeleteAllItems();
	SetListCtrlItem();
	Data2Dlg(-1);
	*pResult = 0;

	// Apply to all tables/charts 문자열 변경
	CString strApplyAll = (m_tabItem.GetCurSel()==0) ? _LS(IDS_TM_DYNA_APPLYALL_TABLE) : _LS(IDS_TM_DYNA_APPLYALL_CHART);
	GetDlgItem(IDC_TM_REPORT_UNIT_APPLY_ALL_CHK)->SetWindowText(strApplyAll);
}

void CReportUnitCustomizeDlg::OnItemchangeLst(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(m_bApplyAll==TRUE)
	{
		// ApplyAll 체크온된 경우 아무것도 안 바꿈 
	}
	else if(pNMListView->uOldState == LVIS_FOCUSED)
	{
		// 변경 전
	}
	else if(pNMListView->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
	{
		// 변경 후
		Data2Dlg(pNMListView->iItem);
	}

	*pResult = 0;
}

void CReportUnitCustomizeDlg::OnUnitSysRdo()
{
	UpdateData(TRUE);

	int nSelected;
	for(int i=-1; i<m_List.GetItemCount()-1; i++)
	{
		nSelected = m_List.GetNextItem(i, LVNI_ALL | LVNI_SELECTED);
		if(nSelected>-1) 
		{
			// Current Unit System 누른 경우 Defined값 저장
			if(m_nSystemRdo==0)
			{
				Dlg2Data(nSelected);
			}
			// Defined 누른 경우 현재 단위계로 세팅
			else
			{
				m_aData[nSelected].nUnitSys = m_nSystemRdo;
				m_aData[nSelected].UnitIdx  = m_UnitIdxCur;
			}
		}
	}

	Data2Dlg();
}

// 모든 항목의 단위계를 기본값(Current Unit System)으로 초기화
void CReportUnitCustomizeDlg::OnDefaultAllBtn()
{
	for(int i=0; i<m_aData.GetSize(); i++)
	{
		m_aData[i].UnitIdx  = m_UnitIdxCur;
		m_aData[i].nUnitSys = 0;
	}
	
	Data2Dlg();

	// Unsp DB 전부 삭제
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_TM_DYNA_REPORT_UNIT_ADDDB)), CMDTYPE_FREE)) return; // DB Transaction없이 작업
	if (!pDoc->m_pEditData->DelAllUnsp()) EndEdit(FALSE);
	EndEdit(TRUE);
}

void CReportUnitCustomizeDlg::OnApplyBtn()
{
	Dlg2Data();
	Apply();
	
	// Apply 이후 해줘야 되는 작업
	m_bApplyAll = FALSE;
	UpdateData(FALSE);
}

void CReportUnitCustomizeDlg::OnApplyAllChk()
{
	UpdateData(TRUE);

	// ApplyAll 체크오프한 경우 기존 데이터로 대화상자 갱신
	if(m_bApplyAll==FALSE)
	{
		Data2Dlg();
	}
}

void CReportUnitCustomizeDlg::Apply()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_UTBL_K, T_UTBL_K> aUtblK;  CArray<T_UTBL_D, T_UTBL_D&> aUtblD;
	CArray<T_UCHT_K, T_UCHT_K> aUchtK;  CArray<T_UCHT_D, T_UCHT_D&> aUchtD;
	CArray<T_UNSP_K, T_UNSP_K> aUnspK;  CArray<T_UNSP_D, T_UNSP_D&> aUnspD;

	if(m_tabItem.GetCurSel()==0)  m_aData_Table.Copy(m_aData);
	else                          m_aData_Chart.Copy(m_aData);
	
	T_UNSP_K UnspK = 1;
	T_UNSP_D UnspD;
	if(!pDoc->m_pAttrCtrl2->GetUnsp(UnspK, UnspD)) 
	{
		UnspD.Initialize();
		UnspD.SectSumm.UnitIdx = m_UnitIdxCur;
		UnspD.ReinForc.UnitIdx = m_UnitIdxCur;
		UnspD.CompSect.UnitIdx = m_UnitIdxCur;
	}

	T_UNIT_CUSTOM_DATA Data, Data_CurDlg;
	if(m_bApplyAll==TRUE)
	{
		Data_CurDlg = m_aData[m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED)];
	}

	for(int i=0; i<m_aData_Table.GetSize(); i++)
	{
		if(m_tabItem.GetCurSel()==0 && m_bApplyAll==TRUE)
		{
			// Apply All에 체크되어있으면 모든 유닛을 이걸로 세팅
			m_aData_Table[i].nUnitSys  = Data_CurDlg.nUnitSys;
			m_aData_Table[i].UnitIdx   = Data_CurDlg.UnitIdx;
		}
		Data = m_aData_Table[i];

		if(Data.nType == TYPE_UTBL)
		{
			T_UTBL_K UtblK = Data.nKey;
			T_UTBL_D UtblD;
			if(!pDoc->m_pAttrCtrl->GetUtbl(UtblK, UtblD)) continue;
			UtblD.bDefinedUnit = Data.nUnitSys;
			UtblD.nUnitLength  = Data.UnitIdx.nBase_Length;
			UtblD.nUnitForce   = Data.UnitIdx.nBase_Force;
			UtblD.nUnitHeat 	 = Data.UnitIdx.nBase_Heat;
			UtblD.nUnitTemper  = Data.UnitIdx.nBase_Temper;
			
			aUtblK.Add(UtblK);
			aUtblD.Add(UtblD);
		}
		else if(Data.nType == TYPE_SECTSUMM)
		{
			UnspD.SectSumm.nUnitSys = Data.nUnitSys;
			UnspD.SectSumm.UnitIdx  = Data.UnitIdx;
		}
		else if(Data.nType == TYPE_REINFORC)
		{
			UnspD.ReinForc.nUnitSys = Data.nUnitSys;
			UnspD.ReinForc.UnitIdx  = Data.UnitIdx;
		}
		else if(Data.nType == TYPE_COMPSECT)
		{
			UnspD.CompSect.nUnitSys = Data.nUnitSys;
			UnspD.CompSect.UnitIdx  = Data.UnitIdx;
		}
		else ASSERT(0);
	}
	for(int i=0; i<m_aData_Chart.GetSize(); i++)
	{
		if(m_tabItem.GetCurSel()==1 && m_bApplyAll==TRUE)
		{
			// Apply All에 체크되어있으면 모든 유닛을 이걸로 세팅
			m_aData_Chart[i].nUnitSys  = Data_CurDlg.nUnitSys;
			m_aData_Chart[i].UnitIdx   = Data_CurDlg.UnitIdx;
		}
		Data = m_aData_Chart[i];

		if(Data.nType == TYPE_UCHT)
		{
			T_UCHT_K UchtK = Data.nKey;
			T_UCHT_D UchtD;
			if(!pDoc->m_pAttrCtrl->GetUcht(UchtK, UchtD)) continue;
			UchtD.bDefinedUnit = Data.nUnitSys;
			UchtD.nUnitLength  = Data.UnitIdx.nBase_Length;
			UchtD.nUnitForce   = Data.UnitIdx.nBase_Force;
			UchtD.nUnitHeat 	 = Data.UnitIdx.nBase_Heat;
			UchtD.nUnitTemper  = Data.UnitIdx.nBase_Temper;
		
			aUchtK.Add(UchtK);
			aUchtD.Add(UchtD);
		}
	}

	if(!pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_TM_DYNA_REPORT_UNIT_ADDDB)), CMDTYPE_FREE)) return; // DB Transaction없이 작업
	for(int i=0; i<aUtblK.GetSize(); i++) if(!pDoc->m_pEditData->AddUtbl(aUtblK[i], aUtblD[i])) EndEdit(FALSE);
	for(int i=0; i<aUchtK.GetSize(); i++) if(!pDoc->m_pEditData->AddUcht(aUchtK[i], aUchtD[i])) EndEdit(FALSE);
	if(!pDoc->m_pEditData->AddUnsp(UnspK, UnspD)) EndEdit(FALSE);

	EndEdit(TRUE);

	if(m_tabItem.GetCurSel()==0)  m_aData.Copy(m_aData_Table);
	else                          m_aData.Copy(m_aData_Chart);
}

BOOL CReportUnitCustomizeDlg::EndEdit(BOOL bEnd)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	if(bEnd && pDoc->m_pDataCtrl->CheckModifiedModel())
	{
		BOOL bUpdateDefault=FALSE;
		pDoc->m_pUndoCtrl->CloseEditDB(bUpdateDefault, FALSE, 0);
		return TRUE;
	}
	else
	{
		pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	return FALSE;
}

// m_aData에 값 저장
void CReportUnitCustomizeDlg::MakeData()
{
	m_aData_Table.RemoveAll();
	m_aData_Chart.RemoveAll();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_CUSTOM_DATA Data;
	Data.Initialize();

	// UTBL
	CArray<CString, CString&> StrUtblList;
	pDoc->m_pAttrCtrl->GetUtblList(StrUtblList);
	qsort( (void *)StrUtblList.GetData(), (size_t)StrUtblList.GetSize(), sizeof( char * ), MDCompareStringArray );   // StrList를 이름순으로 정렬

	T_UTBL_D UtblD;
	for(int i=0; i<StrUtblList.GetSize(); i++)
	{
		Data.nType = TYPE_UTBL;
		Data.strName = StrUtblList[i];
		Data.nKey    = pDoc->m_pAttrCtrl->GetUtblKey(Data.strName);
		pDoc->m_pAttrCtrl->GetUtbl(Data.strName, UtblD);
		Data.UnitIdx.nBase_Length  = UtblD.nUnitLength;
		Data.UnitIdx.nBase_Force   = UtblD.nUnitForce ;
		Data.UnitIdx.nBase_Heat    = UtblD.nUnitHeat  ;
		Data.UnitIdx.nBase_Temper  = UtblD.nUnitTemper;
		Data.nUnitSys              = UtblD.bDefinedUnit;

		m_aData_Table.Add(Data);
	}

	// UNSP
	T_UNSP_K UnspK = 1;
	T_UNSP_D UnspD;
	if(!pDoc->m_pAttrCtrl2->GetUnsp(UnspK, UnspD)) 
	{
		UnspD.Initialize();
		UnspD.SectSumm.UnitIdx = m_UnitIdxCur;
		UnspD.ReinForc.UnitIdx = m_UnitIdxCur;
		UnspD.CompSect.UnitIdx = m_UnitIdxCur;
	}
	Data.Initialize();
	Data.nType = TYPE_SECTSUMM;
	Data.strName = CString(_T("["))+_LS(IDS_TM_DYNA_REPORT_Section_Summary)+CString(_T("]"));
	Data.nUnitSys  = UnspD.SectSumm.nUnitSys;
	Data.UnitIdx   = UnspD.SectSumm.UnitIdx;      
	m_aData_Table.Add(Data);
	
	Data.nType = TYPE_REINFORC;
	Data.strName = CString(_T("["))+_LS(IDS_TM_DYNA_REPORT_Reinforcement_of_Sections)+CString(_T("]"));
	Data.nUnitSys  = UnspD.ReinForc.nUnitSys;
	Data.UnitIdx   = UnspD.ReinForc.UnitIdx;      
	m_aData_Table.Add(Data);
	
	Data.nType = TYPE_COMPSECT;
	Data.strName = CString(_T("["))+_LS(IDS_TM_DYNA_REPORT_Composite_Section_for_CS)+CString(_T("]"));
	Data.nUnitSys  = UnspD.CompSect.nUnitSys;
	Data.UnitIdx   = UnspD.CompSect.UnitIdx;      
	m_aData_Table.Add(Data);
	
	// UCHT
	CArray<CString, CString&> StrUchtList;
	pDoc->m_pAttrCtrl->GetUchtList(StrUchtList);
	qsort( (void *)StrUchtList.GetData(), (size_t)StrUchtList.GetSize(), sizeof( char * ), MDCompareStringArray );   // StrList를 이름순으로 정렬
	
	T_UCHT_D UchtD;
	for(int i=0; i<StrUchtList.GetSize(); i++)
	{
		Data.nType = TYPE_UCHT;
		Data.strName = StrUchtList[i];
		Data.nKey    = pDoc->m_pAttrCtrl->GetUchtKey(Data.strName);
		pDoc->m_pAttrCtrl->GetUcht(Data.strName, UchtD);
		Data.UnitIdx.nBase_Length  = UchtD.nUnitLength;
		Data.UnitIdx.nBase_Force   = UchtD.nUnitForce ;
		Data.UnitIdx.nBase_Heat    = UchtD.nUnitHeat  ;
		Data.UnitIdx.nBase_Temper  = UchtD.nUnitTemper;
		Data.nUnitSys              = UchtD.bDefinedUnit;
		
		m_aData_Chart.Add(Data);
	}

	if(m_nMode == WORK_UCHT_LST)
	{
		m_aData.Copy(m_aData_Chart);
	}
	else
	{
		m_aData.Copy(m_aData_Table);
	}
}

void CReportUnitCustomizeDlg::OnComboBoxSelChange()
{
	Dlg2Data();
}
