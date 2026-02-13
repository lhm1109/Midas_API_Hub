// DgnCfMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfMatDlg.h"

#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCfMatDlg dialog


CDgnCfMatDlg::CDgnCfMatDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCfMatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCfMatDlg)
	m_MatName2 = _T("");
	m_Ps = 0.0;
	m_Es = 0.0;
	m_Fu = 0.0;
	m_Fy1 = 0.0;
	m_Fy2 = 0.0;
	m_Fy3 = 0.0;
	m_Fy4 = 0.0;
	m_Fy5 = 0.0;
	m_Fy6 = 0.0;
	m_Fy7 = 0.0;
	m_Fy8 = 0.0;
	m_Fy9 = 0.0;
	m_Fy10 = 0.0;
	m_dGammaR = 0.0;
	m_dEta = 1.0;
	//}}AFX_DATA_INIT
}


void CDgnCfMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfMatDlg)
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY10UNIT, m_Fy10Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY9UNIT, m_Fy9Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY8UNIT, m_Fy8Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY7UNIT, m_Fy7Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY6UNIT, m_Fy6Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY5UNIT, m_Fy5Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY4UNIT, m_Fy4Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY3UNIT, m_Fy3Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY2UNIT, m_Fy2Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFY1UNIT, m_Fy1Unit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATFUUNIT, m_FuUnit);
	DDX_Control(pDX, IDC_DGN_STEEL_MATESUNIT, m_EsUnit);
	DDX_Control(pDX, IDC_DGN_STEELMAT_CODE, m_Code);
	DDX_Control(pDX, IDC_DGN_STEEL_MAT, m_MatName);
	DDX_Control(pDX, IDC_DGN_STEELMAT_LIST, m_ListCtrl);
	DDX_Text(pDX, IDC_DGN_STEEL_MAT2, m_MatName2);
	DDX_Text(pDX, IDC_DGN_STEEL_MATPS, m_Ps);
	DDX_Text(pDX, IDC_DGN_STEEL_MATES, m_Es);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFU, m_Fu);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY1, m_Fy1);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY2, m_Fy2);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY3, m_Fy3);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY4, m_Fy4);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY5, m_Fy5);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY6, m_Fy6);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY7, m_Fy7);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY8, m_Fy8);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY9, m_Fy9);
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY10, m_Fy10);
	DDX_Text(pDX, IDC_DGN_STEEL_GAMAR, m_dGammaR);
	DDX_Text(pDX, IDC_DGN_CF_MAT_ETA, m_dEta);
	DDX_Control(pDX, IDC_DGN_CF_MAT_ETA_CHECK, m_chkEta);

	DDX_Control(pDX, IDC_DGN_STEEL_ALWS10, m_KSCE05Name);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS1, m_editAlwS1);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS2, m_editAlwS2);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS3, m_editAlwS3);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS4, m_editAlwS4);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS5, m_editAlwS5);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS6, m_editAlwS6);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS7, m_editAlwS7);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS8, m_editAlwS8);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS9, m_editAlwS9);
	DDX_Control(pDX, IDC_DGN_STEEL_MINT1, m_editMinT1);
	DDX_Control(pDX, IDC_DGN_STEEL_MINT2, m_editMinT2);
	DDX_Control(pDX, IDC_DGN_STEEL_MINT3, m_editMinT3);

	DDX_Control(pDX, IDC_DGN_STEEL_ALWS1_UNIT, m_unitAlwS1);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS2_UNIT, m_unitAlwS2);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS3_UNIT, m_unitAlwS3);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS4_UNIT, m_unitAlwS4);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS5_UNIT, m_unitAlwS5);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS6_UNIT, m_unitAlwS6);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS7_UNIT, m_unitAlwS7);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS8_UNIT, m_unitAlwS8);
	DDX_Control(pDX, IDC_DGN_STEEL_ALWS9_UNIT, m_unitAlwS9);
	DDX_Control(pDX, IDC_DGN_STEEL_MINT1_UNIT, m_unitMinT1);
	DDX_Control(pDX, IDC_DGN_STEEL_MINT2_UNIT, m_unitMinT2);
	DDX_Control(pDX, IDC_DGN_STEEL_MINT3_UNIT, m_unitMinT3);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnCfMatDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnCfMatDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_STEELMAT_LIST, OnItemchangedDgnSteelmatList)
	ON_CBN_SELCHANGE(IDC_DGN_STEELMAT_CODE, OnSelchangeDgnSteelmatCode)
	ON_BN_CLICKED(IDC_DGN_STEELMAT_MODIFY, OnDgnSteelmatModify)
	ON_CBN_SELCHANGE(IDC_DGN_STEEL_MAT, OnSelchangeDgnSteelMat)
	ON_NOTIFY(NM_CLICK, IDC_DGN_STEELMAT_LIST, OnClickDgnSteelmatList)
	ON_CBN_SELCHANGE(IDC_DGN_STEEL_ALWS10, OnSelchangeDgnSteelKSCE05Name)
	ON_BN_CLICKED(IDC_DGN_CF_MAT_ETA_CHECK, OnChkEtaGB50018)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfMatDlg message handlers

BOOL CDgnCfMatDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	ControlMan(TRUE);
#ifdef _CH
	EnableMatlPara(FALSE);
	ShowEditBox(1);
#endif	
	Initial_ListCtrl();
	Initial_Data();
	Initial_Unit();
	Initial_KSCE05NameCombo();
	//
	GetDlgItem(IDC_DGN_STATIC_GAMAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_GAMAR)->ShowWindow(SW_HIDE);
	//ControlMan(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCfMatDlg::Initial_Unit()
{
	m_FuUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_EsUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy1Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy2Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy3Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy4Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy5Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy6Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy7Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy8Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy9Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy10Unit.SetUnitType(D_UNITSYS_BASE_ELAST);

	m_editAlwS1.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS2.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS3.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS4.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS5.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS6.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS7.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS8.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editAlwS9.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_editMinT1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editMinT2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editMinT3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitAlwS1.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS2.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS3.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS4.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS5.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS6.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS7.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS8.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitAlwS9.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_unitMinT1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitMinT2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitMinT3.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnCfMatDlg::Initial_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	// changed by Seungjun (`06.05.29) add Fy5, Fy6.
	LV_COLUMN lvcolumn;
	TCHAR *list[14]	= {_T("ID"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), _T("Es"), _T("Fu"), _T("Fy|Fy1"), _T("Fy2"), _T("Fy3"), _T("Fy4"), _T("Fy5"), _T("Fy6"), _T("Fy7"), _T("Fy8"), _T("Fy9"), _T("Fy10") };
	int width[14]	= {48,75,72,68,68,68,68,68,68,68,68,68,68,68};
	for(int i=0; i<14; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}
}

void CDgnCfMatDlg::Initial_Data()
{
	CArray<T_MATL_K,T_MATL_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(KeyList);
	int iCount = KeyList.GetSize();

	for(int i=0; i<iCount; i++)
	{
		T_MATD_D mData;
		int nID = KeyList.GetAt(i);
		mData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData))	continue;

		if(mData.Type==_T("S"))
		{
			CString strID=_T(""); strID.Format(_T("%7d"),nID);
			CString strName = mData.Name;
			// Change by ZINU.('02.9.17).
			CString strMatCode = mData.Data1.CodeName;
			ShowEditBox(GetChkKind(strMatCode));
			
			CString strEs=_T("");  strEs.Format( _T("%g"),mData.Data1.Analysis.Elast);
			CString strFu=_T("");  strFu.Format( _T("%g"),mData.Data1.Design.S_Fu);
			CString strFy1=_T(""); strFy1.Format(_T("%g"),mData.Data1.Design.S_Fy1);
			CString strFy2=_T(""); strFy2.Format(_T("%g"),mData.Data1.Design.S_Fy2);
			CString strFy3=_T(""); strFy3.Format(_T("%g"),mData.Data1.Design.S_Fy3);
			CString strFy4=_T(""); strFy4.Format(_T("%g"),mData.Data1.Design.S_Fy4);
			// add by Seungjun (`06.05.29).
			CString strFy5=_T(""); strFy5.Format(_T("%g"),mData.Data1.Design.S_Fy5);
			CString strFy6=_T(""); strFy6.Format(_T("%g"),mData.Data1.Design.S_Fy6);
			CString strFy7 = _T(""); strFy7.Format(_T("%g"), mData.Data1.Design.S_Fy7);
			CString strFy8 = _T(""); strFy8.Format(_T("%g"), mData.Data1.Design.S_Fy8);
			CString strFy9 = _T(""); strFy9.Format(_T("%g"), mData.Data1.Design.S_Fy9);
			CString strFy10 = _T(""); strFy10.Format(_T("%g"), mData.Data1.Design.S_Fy10);
			int ListLineNo = m_ListCtrl.GetItemCount();

			// Add by sshan. MNET:2729.('20070507)//////////////////////////
			CString strDesingCode = GetDesignCodeName();
			if(strDesingCode==_T("KSCE-ASD05") && mData.Data1.CodeName == _T(""))
			{
				strFu = _T("-");
				strFy1 = _T("-");
				strFy2 = _T("-");
				strFy3 = _T("-");
				strFy4 = _T("-");
				strFy5 = _T("-");
				strFy6 = _T("-");
				strFy7 = _T("-");
				strFy8 = _T("-");
				strFy9 = _T("-");
				strFy10 = _T("-");
			}
			////////////////////////////////////////////////////////////////////

			Write_ListCtrlBox(ListLineNo,strID,strName,strEs,strFu,strFy1,strFy2,strFy3,strFy4,strFy5,strFy6,strFy7,strFy8,strFy9,strFy10);
		}
	}
	EnableEditBox(TRUE);
	GetDlgItem(IDC_DGN_STEEL_MAT2)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void CDgnCfMatDlg::Write_ListCtrlBox(int Index, CString strID, CString strName, CString strEs, CString strFu, CString strFy1, CString strFy2, CString strFy3, CString strFy4, CString strFy5, CString strFy6, CString strFy7, CString strFy8, CString strFy9, CString strFy10)
{
	// change by Seungjun (`06.05.29).
	LV_ITEM lvitem;
	LPTSTR szText[14];
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)strName;
	szText[2] = (LPTSTR)(LPCTSTR)strEs;
	szText[3] = (LPTSTR)(LPCTSTR)strFu;
	szText[4] = (LPTSTR)(LPCTSTR)strFy1;
	szText[5] = (LPTSTR)(LPCTSTR)strFy2;
	szText[6] = (LPTSTR)(LPCTSTR)strFy3;
	szText[7] = (LPTSTR)(LPCTSTR)strFy4;
	szText[8] = (LPTSTR)(LPCTSTR)strFy5;
	szText[9] = (LPTSTR)(LPCTSTR)strFy6;
	szText[10] = (LPTSTR)(LPCTSTR)strFy7;
	szText[11] = (LPTSTR)(LPCTSTR)strFy8;
	szText[12] = (LPTSTR)(LPCTSTR)strFy9;
	szText[13] = (LPTSTR)(LPCTSTR)strFy10;

	// Input ListCtrl Box.
	for(int i=0; i<14; i++)
	{
		lvitem.mask		  = LVIF_TEXT;
		lvitem.iItem  	= Index;
		lvitem.iSubItem = i;
		lvitem.pszText	= szText[i];
		if(i==0)	m_ListCtrl.InsertItem(&lvitem);
		else			m_ListCtrl.SetItem(&lvitem);
	}
}

void CDgnCfMatDlg::OnClickDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_bDoOnce = FALSE;
	OnItemchangedDgnSteelmatList(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnCfMatDlg::OnItemchangedDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
#ifdef _MGEN_CH
	if(m_bDoOnce)
		return;
	else
		m_bDoOnce = TRUE;
	SetRedraw(FALSE);
#endif

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem,0);

	T_MATD_D mData;
	mData.Initialize();
	int key = _ttoi(strID);
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(key, mData);
	if(bCheck)
	{
		//add by tss 2022/10/14
		BOOL bNewGB50018 = IsGB50018(mData.Data1.CodeName);
		EnableMatlPara(bNewGB50018);
		m_dGammaR = bNewGB50018 ? GetGB50018_GammaR(mData.Data1.CodeName, mData.Data1.CodeMatlName) : 0;
		if (IsGB50018(mData.Data1.CodeName))
		{
			GetDlgItem(IDC_DGN_CF_MAT_ETA_CHECK)->ShowWindow(TRUE);
			GetDlgItem(IDC_DGN_CF_MAT_ETA)->ShowWindow(TRUE);
			mData.Data1.Design.bChkEta ? GetDlgItem(IDC_DGN_CF_MAT_ETA)->EnableWindow(SW_SHOW) : GetDlgItem(IDC_DGN_CF_MAT_ETA)->EnableWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_DGN_CF_MAT_ETA_CHECK)->ShowWindow(FALSE);
			GetDlgItem(IDC_DGN_CF_MAT_ETA)->ShowWindow(FALSE);
		}
		m_chkEta.SetCheck(mData.Data1.Design.bChkEta);
		m_dEta = mData.Data1.Design.dEta == 0 ? 1.0 : mData.Data1.Design.dEta;
		m_Es	= mData.Data1.Analysis.Elast;
		m_Ps	= mData.Data1.Analysis.Poisson;
		m_Fu  = mData.Data1.Design.S_Fu;
		m_Fy1 = mData.Data1.Design.S_Fy1;
		m_Fy2 = mData.Data1.Design.S_Fy2;
		m_Fy3 = mData.Data1.Design.S_Fy3;
		m_Fy4 = mData.Data1.Design.S_Fy4;
		// add by Seungjun (`06.05.29). 
		m_Fy5 = mData.Data1.Design.S_Fy5;
		m_Fy6 = mData.Data1.Design.S_Fy6;
		m_Fy7 = mData.Data1.Design.S_Fy7;
		m_Fy8 = mData.Data1.Design.S_Fy8;
		m_Fy9 = mData.Data1.Design.S_Fy9;
		m_Fy10 = mData.Data1.Design.S_Fy10;

		// Add by sshan. MNET:2729.('20070507)
		m_editAlwS1.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS1);
		m_editAlwS2.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS2);
		m_editAlwS3.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS3);
		m_editAlwS4.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS5);
		m_editAlwS5.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS9);
		m_editAlwS6.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS10);
		m_editAlwS7.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS6);
		m_editAlwS8.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS7);
		m_editAlwS9.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dAlwS8);
		m_editMinT1.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dMin_t1);
		m_editMinT2.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dMin_t3);		
		m_editMinT3.SetEditUnit(mData.Data1.Design.Matl_KSCE05.dMin_t2);		

		// Change by ZINU.('02.9.17).
		CString strMatCode = mData.Data1.CodeName;
		ShowEditBox(GetChkKind(strMatCode));
		if(strMatCode==_T("") || strMatCode==_T("None"))	EnableEditBox(TRUE);
		else																			EnableEditBox(FALSE);
		// Material Code와 Name ComboBox를 초기화합니다.
		Initial_MaterialCombo(mData.Data1.CodeName, mData.Data1.CodeMatlName);

		CString MatlName;
		MatlName = mData.Data1.CodeMatlName;
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(mData.Data1.CodeName, MatlName);
#endif
		int Index = m_MatName.FindStringExact(-1, MatlName);
		if(Index == -1) 
		{
			m_MatName2 = m_ListCtrl.GetItemText(nItem,1);	// strName2
			GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
			GetDlgItem(IDC_DGN_STEEL_MAT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_STEEL_MAT2)->ShowWindow(SW_SHOW);
		}
		else 
		{
			m_MatName.SetCurSel(Index);
			GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
			GetDlgItem(IDC_DGN_STEEL_MAT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MAT2)->ShowWindow(SW_HIDE);
		}
		UpdateData(FALSE);
	}
	*pResult = 0;

	ControlMan(FALSE);
	ShowEditBox(GetChkKind(mData.Data1.CodeName));

	Initial_KSCE05NameCombo();

#ifdef _MGEN_CH
	SetRedraw(TRUE);
	Invalidate();
#endif
}

void CDgnCfMatDlg::EnableEditBox(BOOL bCheck)
{
	if(bCheck)	// Enable.
	{
		GetDlgItem(IDC_DGN_STEEL_MATES)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATPS)->EnableWindow(TRUE);

		GetDlgItem(IDC_DGN_STEEL_MATFU)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->EnableWindow(TRUE);
		// add by Seungjun (`06.05.29).
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY6)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY7)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY9)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10)->EnableWindow(TRUE);
	}
	else	// Disable.
	{
		GetDlgItem(IDC_DGN_STEEL_MATES)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATPS)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_STEEL_MATFU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->EnableWindow(FALSE);
		// add by Seungjun (`06.05.29).
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY9)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10)->EnableWindow(FALSE);
		//add by tss 2022/10/14
		GetDlgItem(IDC_DGN_STATIC_GAMAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_GAMAR)->ShowWindow(SW_HIDE);
	}
#ifdef _MGEN_CH
	GetDlgItem(IDC_DGN_STATIC_MATFY7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_MATFY8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_MATFY9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_MATFY10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY7UNIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY8UNIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY9UNIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STEEL_MATFY10UNIT)->ShowWindow(SW_HIDE);

#endif 
}

void CDgnCfMatDlg::ShowEditBox(int nCheck)
{
	//071106 sshan
	if(GetControlType() == CONTROL_TYPE_B)
		return;

	// Set Hide all Control
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON10)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY10)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY5UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY6UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY7UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY8UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY9UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10UNIT)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_CF_MAT_ETA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CF_MAT_ETA_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_GAMAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_GAMAR)->ShowWindow(SW_HIDE);
	}

	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	if(Index != -1)
		m_Code.GetLBText(Index,strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	CString strDgnCode = GetDesignCodeName();
	
	//----------Changed by Hong,jiseon 2002.7.19  
	if(nCheck==1)	// ASTM(S), None.
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY));

		//add by tss 2022/10/14
		CString strMatlName;
		int nIndex = m_MatName.GetCurSel();
		if (nIndex != -1)
		m_MatName.GetLBText(nIndex, strMatlName);
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatlName);
#endif
		BOOL bNewGB50018 = IsGB50018(strCode);
		EnableMatlPara(bNewGB50018);
	}
	else if(nCheck==2)	// KS(S), JIS(S), DIN(S), EN05(S), EN(S), UNI(S), CNS(S). CNS06(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
	}
	else if(nCheck==3)	// KS08(S), KS-Civil(S), GB(S)
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
	}
	else if(nCheck==4)	// JGJ(S), JTJ(S), GB03(S), BS(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_SHOW);

		BOOL bNewGB50018 = IsGB50018(strCode);
		EnableMatlPara(bNewGB50018);
	}
	else if(nCheck==5)	// BC1:12-ASTM(S), BC1:12-GB(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON4)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY5)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));
		
		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY5UNIT)->ShowWindow(SW_SHOW);
	}
	else if(nCheck==6)	// BS04(S). add by Seungjun (`06.05.29). BC1:12-BS EN(S), BC1:12-JIS(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY6)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON5)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY6)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY5UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY6UNIT)->ShowWindow(SW_SHOW);
	}
	else if (nCheck == 10) // SP16.2017t.B3(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY10)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON10)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY10)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY5UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY6UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY7UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY8UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY9UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY10UNIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnCfMatDlg::Initial_MaterialCombo(CString strCode, CString strName)
{
	m_Code.ResetContent();
	m_MatName.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DesignCodeList);
	m_Code.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
		m_Code.AddString(DesignCodeList[i]);
	}

	if(strCode==_T(""))	strCode = _T("None");

	CString CodeName;
	CodeName = strCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	int Index = m_Code.FindStringExact(-1, CodeName);
	m_Code.SetCurSel(Index);

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(strCode,NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, NameList[i]);
#endif
		m_MatName.AddString(NameList[i]);
	}

	UpdateData(FALSE);
}

void CDgnCfMatDlg::OnSelchangeDgnSteelmatCode() 
{
	ControlMan(TRUE);

	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	m_MatName.ResetContent();
	// Change Material Name by Code.
	if(strCode==_T("None") || strCode==_T(""))
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
		GetDlgItem(IDC_DGN_STEEL_MAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MAT2)->ShowWindow(SW_SHOW);
		EnableEditBox(TRUE);
		ShowEditBox(1);
	}
	else
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
		GetDlgItem(IDC_DGN_STEEL_MAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MAT2)->ShowWindow(SW_HIDE);
		EnableEditBox(FALSE);
		
		// Change by ZINU.('02.9.17).
		ShowEditBox(GetChkKind(strCode));

		CArray<CString, CString&> NameList;
		m_pDoc->m_pMatlDB->GetSteelNameList(strCode, NameList);
		for (int i = 0; i < NameList.GetSize(); i++)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, NameList[i]);
#endif
			m_MatName.AddString(NameList[i]);
		}
	}

	if(strCode!=_T("None"))
	{
		m_Es	= 0.0;
		m_Ps	= 0.0;
		m_Fu	= 0.0;
		m_Fy1 = 0.0;
		m_Fy2 = 0.0;
		m_Fy3 = 0.0;
		m_Fy4 = 0.0;
		// add by Seungjun (`06.05.29).
		m_Fy5 = 0.0;
		m_Fy6 = 0.0;
		m_Fy7 = 0.0;
		m_Fy8 = 0.0;
		m_Fy9 = 0.0;
		m_Fy10 = 0.0;

		m_dEta = 1.0;
		m_dGammaR = 0.;
		m_chkEta.SetCheck(0);
	}
	m_MatName2 = _T("");

	Initial_KSCE05NameCombo();

	UpdateData(FALSE);

}

void CDgnCfMatDlg::OnSelchangeDgnSteelMat() 
{
	CString strName = _T("");
	int Index = m_MatName.GetCurSel();
	m_MatName.GetLBText(Index, strName);

	Index = m_Code.GetCurSel();
	CString strCode = _T("");
	m_Code.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode, strName, sData);

	BOOL bNewMatl = IsGB50018(strCode);
	EnableMatlPara(bNewMatl);
	
	if (bCheck)
	{
		m_Es = sData.Elast;
		m_Ps = sData.Poisson;
		m_Fu = sData.S_Fu;
		m_Fy1 = sData.S_Fy1;
		m_Fy2 = sData.S_Fy2;
		m_Fy3 = sData.S_Fy3;
		m_Fy4 = sData.S_Fy4;
		// add by Seungjun (`06.05.29).
		m_Fy5 = sData.S_Fy5;
		m_Fy6 = sData.S_Fy6;
		m_Fy7 = sData.S_Fy7;
		m_Fy8 = sData.S_Fy8;
		m_Fy9 = sData.S_Fy9;
		m_Fy10 = sData.S_Fy10;

		//Add by tss
		m_dGammaR = GetGB50018_GammaR(strCode, strName);
	}

	ControlMan(FALSE);
	ShowEditBox(GetChkKind(strCode));
	
	UpdateData(FALSE);
}

int CDgnCfMatDlg::GetChkKind(CString strMatlCode)
{
	CDgnDataCtrl DataCtrl;
	return DataCtrl.GetChkKindStlMatl(strMatlCode);
}

BOOL CDgnCfMatDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	if(m_Code.GetCurSel()==0)	
	{
		if(m_MatName2 == _T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);}
	}
	if(m_Es <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ES_ERROR);}
	if(m_Ps <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_PS_ERROR);}
	// Change, Jaeoh.(08.03.14) Pre에서 입력한 Material이 None일 경우 KR, Steel Design, KSCE-ASD05일 때 
	// 입력이 안돼. Skip함.
	if(GetDesignCodeName()!=_T("KSCE-ASD05"))
		if(m_Fu <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FU_ERROR);}
	// Code에 따른 Error Checking.
	int Index = m_Code.GetCurSel();
	CString strCode=_T("");
	m_Code.GetLBText(Index, strCode);  
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	// Change by ZINU.('02.9.17).
	int iChkKind = GetChkKind(strCode);
	if(iChkKind==1)
	{
		if(GetDesignCodeName()!=_T("KSCE-ASD05"))
			if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
		m_Fy2 = m_Fy1;
		m_Fy3 = m_Fy1;
		m_Fy4 = m_Fy1;
		// add by Seungjun (`06.05.29).
		m_Fy5 = m_Fy1;
		m_Fy6 = m_Fy1;
		m_Fy7 = m_Fy1;
		m_Fy8 = m_Fy1;
		m_Fy9 = m_Fy1;
		m_Fy10 = m_Fy1;
	}
	else if(iChkKind==2)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		m_Fy3 = m_Fy2;
		m_Fy4 = m_Fy2;
		// add by Seungjun (`06.05.29).
		m_Fy5 = m_Fy2;
		m_Fy6 = m_Fy2;
		m_Fy7 = m_Fy2;
		m_Fy8 = m_Fy2;
		m_Fy9 = m_Fy2;
		m_Fy10 = m_Fy2;
	}
	else if(iChkKind==3)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		m_Fy4 = m_Fy3;
		// add by Seungjun (`06.05.29).
		m_Fy5 = m_Fy3;
		m_Fy6 = m_Fy3;
		m_Fy7 = m_Fy3;
		m_Fy8 = m_Fy3;
		m_Fy9 = m_Fy3;
		m_Fy10 = m_Fy3;
	}
	else if(iChkKind==4)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		if(m_Fy4 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR);}
		// add by Seungjun (`06.05.29).
		m_Fy5 = m_Fy4;
		m_Fy6 = m_Fy4;
		m_Fy7 = m_Fy4;
		m_Fy8 = m_Fy4;
		m_Fy9 = m_Fy4;
		m_Fy10 = m_Fy4;
	}
	else if(iChkKind==5) // by GAY. ('14.03.11)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		if(m_Fy4 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR);}
		if(m_Fy5 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY5_ERROR);}
		m_Fy6 = m_Fy5;
		m_Fy7 = m_Fy5;
		m_Fy8 = m_Fy5;
		m_Fy9 = m_Fy5;
		m_Fy10 = m_Fy5;
	}
	else if(iChkKind==6) // add by Seungjun (`06.05.29).
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		if(m_Fy4 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR);}
		if(m_Fy5 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY5_ERROR);}
		if(m_Fy6 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY6_ERROR);}
		m_Fy7 = m_Fy6;
		m_Fy8 = m_Fy6;
		m_Fy9 = m_Fy6;
		m_Fy10 = m_Fy6;
	}
	else if (iChkKind == 10)
	{
		if (m_Fy1 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR); }
		if (m_Fy2 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR); }
		if (m_Fy3 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR); }
		if (m_Fy4 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR); }
		if (m_Fy5 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY5_ERROR); }
		if (m_Fy6 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY6_ERROR); }
		if (m_Fy7 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY7_ERROR); }
		if (m_Fy8 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY8_ERROR); }
		if (m_Fy9 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY9_ERROR); }
		if (m_Fy10 <= 0.0) { bCheck = FALSE; strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY10_ERROR); }
	}
	else	ASSERT(0);

	if(GetControlType() == CONTROL_TYPE_B)
	{
		double dAlwS1 = 0;
		double dAlwS2 = 0;
		double dAlwS3 = 0;
		double dAlwS4 = 0;
		double dAlwS5 = 0;
		double dAlwS6 = 0;
		double dAlwS7 = 0;
		double dAlwS8 = 0;
		double dAlwS9 = 0;
		double dMinT1 = 0;
		double dMinT2 = 0;
		double dMinT3 = 0;

		dAlwS1 = m_editAlwS1.GetEditValue();
		dAlwS2 = m_editAlwS2.GetEditValue();
		dAlwS3 = m_editAlwS3.GetEditValue();
		dAlwS4 = m_editAlwS4.GetEditValue();
		dAlwS5 = m_editAlwS5.GetEditValue();
		dAlwS6 = m_editAlwS6.GetEditValue();
		dAlwS7 = m_editAlwS7.GetEditValue();
		dAlwS8 = m_editAlwS8.GetEditValue();
		dAlwS9 = m_editAlwS9.GetEditValue();
		dMinT1 = m_editMinT1.GetEditValue();
		dMinT2 = m_editMinT2.GetEditValue();
		dMinT3 = m_editMinT3.GetEditValue();

		if(dAlwS1 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS1_ERROR);}
		if(dAlwS2 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS2_ERROR);}
		if(dAlwS3 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS3_ERROR);}
		if(dAlwS4 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS5_ERROR);}
		if(dAlwS5 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS9_ERROR);}
		if(dAlwS6 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS10_ERROR);}
		if(dAlwS7 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS6_ERROR);}
		if(dAlwS8 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS7_ERROR);}
		if(dAlwS9 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ALWS8_ERROR);}
		if(dMinT1 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_MINT1_ERROR);}
		if(dMinT2 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_MINT3_ERROR);}
		if(dMinT3 < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_MINT2_ERROR);}
	}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CDgnCfMatDlg::OnDgnSteelmatModify() 
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL),MB_OK);
		return;
	}

	CString strName=_T(""), strMatName=_T("");
	CString strID = m_ListCtrl.GetItemText(nItem, 0);
	// Modify Material ID
	int nID   = _ttoi(strID);
	int nCode = m_Code.GetCurSel();
	CString strCode=_T("");

	T_MATD_D mData;
	mData.Initialize();
	m_pDoc->m_pAttrCtrl->GetMatlDesign(nID, mData);

	UpdateData(TRUE);
	if(ErrorCheck())
	{
		if(nCode==0)	// Material Code = None
		{
			strMatName = m_MatName2;
			strName    = m_MatName2;
		}
		else	// Material Code = Code
		{
			m_Code.GetLBText(nCode, strCode);
			int nName = m_MatName.GetCurSel();
			m_MatName.GetLBText(nName, strMatName);
			strName = strMatName;

#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatName);
#endif

			T_MATL_STEEL sData;
			if(!m_pDoc->m_pMatlDB->GetSteelData(strCode, strMatName, sData))	ASSERT(0);
		}
		mData.Name = strName;
		mData.Data1.CodeName      = strCode;
		mData.Data1.CodeMatlName  = strMatName;
		mData.Data1.Analysis.Elast   = m_Es;
		mData.Data1.Analysis.Poisson = m_Ps;
		mData.Data1.Design.S_Fu		= m_Fu;
		mData.Data1.Design.S_Fy		= m_Fy1;
		mData.Data1.Design.S_Fy1	= m_Fy1;
		mData.Data1.Design.S_Fy2	= m_Fy2;
		mData.Data1.Design.S_Fy3	= m_Fy3;
		mData.Data1.Design.S_Fy4	= m_Fy4;
		// add by Seungjun (`06.05.29).
		mData.Data1.Design.S_Fy5	= m_Fy5;
		mData.Data1.Design.S_Fy6	= m_Fy6;
		mData.Data1.Design.S_Fy7	= m_Fy7;
		mData.Data1.Design.S_Fy8	= m_Fy8;
		mData.Data1.Design.S_Fy9	= m_Fy9;
		mData.Data1.Design.S_Fy10	= m_Fy10;

		//
		mData.Data1.Design.dEta = m_dEta;
		mData.Data1.Design.bChkEta = m_chkEta.GetCheck();
		// Add by sshan. MNET:2729.('20070507)
		mData.Data1.Design.Matl_KSCE05.dAlwS1 = m_editAlwS1.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS2 = m_editAlwS2.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS3 = m_editAlwS3.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS5 = m_editAlwS4.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS9 = m_editAlwS5.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS10= m_editAlwS6.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS6 = m_editAlwS7.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS7 = m_editAlwS8.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dAlwS8 = m_editAlwS9.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dMin_t1 = m_editMinT1.GetEditValue();
		mData.Data1.Design.Matl_KSCE05.dMin_t3 = m_editMinT2.GetEditValue();		
		mData.Data1.Design.Matl_KSCE05.dMin_t2 = m_editMinT3.GetEditValue();		

		BOOL bCheck = m_pDoc->m_pDataCtrl->AddMatd(nID, mData);
		if(bCheck)
		{
			CString strEs=_T("");  strEs.Format( _T("%g"),m_Es);
			CString strFu=_T("");  strFu.Format( _T("%g"),m_Fu);
			CString strFy1=_T(""); strFy1.Format(_T("%g"),m_Fy1);
			CString strFy2=_T(""); strFy2.Format(_T("%g"),m_Fy2);
			CString strFy3=_T(""); strFy3.Format(_T("%g"),m_Fy3);
			CString strFy4=_T(""); strFy4.Format(_T("%g"),m_Fy4);
			// add by Seungjun (`06.05.29).
			CString strFy5=_T(""); strFy5.Format(_T("%g"),m_Fy5);
			CString strFy6=_T(""); strFy6.Format(_T("%g"),m_Fy6);
			CString strFy7=_T(""); strFy7.Format(_T("%g"), m_Fy7);
			CString strFy8=_T(""); strFy8.Format(_T("%g"), m_Fy8);
			CString strFy9=_T(""); strFy9.Format(_T("%g"), m_Fy9);
			CString strFy10=_T(""); strFy10.Format(_T("%g"), m_Fy10);

			if(GetDesignCodeName()==_T("KSCE-ASD05") && mData.Data1.CodeName==_T(""))
			{
				strFu = _T("-");
				strFy1 = _T("-");
				strFy2 = _T("-");
				strFy3 = _T("-");
				strFy4 = _T("-");
				strFy5 = _T("-");
				strFy6 = _T("-");
				strFy7 = _T("-");
				strFy8 = _T("-");
				strFy9 = _T("-");
				strFy10 = _T("-");
			}

			m_ListCtrl.SetItemText(nItem,1,strName);	// Name.
			m_ListCtrl.SetItemText(nItem,2,strEs);		// Es.
			m_ListCtrl.SetItemText(nItem,3,strFu);		// Fu.
			m_ListCtrl.SetItemText(nItem,4,strFy1);		// Fy1.
			m_ListCtrl.SetItemText(nItem,5,strFy2);		// Fy2.
			m_ListCtrl.SetItemText(nItem,6,strFy3);		// Fy3.
			m_ListCtrl.SetItemText(nItem,7,strFy4);		// Fy4.
			// add by Seungjun (`06.05.29).
			m_ListCtrl.SetItemText(nItem,8,strFy5);		// Fy5.
			m_ListCtrl.SetItemText(nItem,9,strFy6);		// Fy6.
			m_ListCtrl.SetItemText(nItem,10,strFy7);	// Fy7.
			m_ListCtrl.SetItemText(nItem,11,strFy8);	// Fy8.
			m_ListCtrl.SetItemText(nItem,12,strFy9);	// Fy9.
			m_ListCtrl.SetItemText(nItem,13,strFy10);	// Fy10.
			m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CDgnCfMatDlg::OnOK() 
{
	CDialogMove::OnOK();
}

void CDgnCfMatDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnCfMatDlg::ControlMan(BOOL bInit)
{
	static CArray<UINT, UINT> arCtrlAType;
	static CArray<UINT, UINT> arCtrlBType;
	static BOOL bFirst = TRUE;

	if(bFirst)
	{
		CDlgUtil::GetCtrlIDByIncRect(this,arCtrlAType,IDC_DGN_TYPE_A_HOLDER, FALSE);
		CDlgUtil::GetCtrlIDByIncRect(this,arCtrlBType,IDC_DGN_TYPE_B_HOLDER, FALSE);    
		bFirst = FALSE;
	}
		
	if(bInit)
	{
		CWnd* pAHolder = GetDlgItem(IDC_DGN_TYPE_A_HOLDER);
		CWnd* pBHolder = GetDlgItem(IDC_DGN_TYPE_B_HOLDER);
		
		CRect AHRect, BHRect;
		pAHolder->GetWindowRect(AHRect);
		pBHolder->GetWindowRect(BHRect);    

		int DY_B = AHRect.top - BHRect.top;
		
		CDlgUtil::CtrlMoveDistY(this, arCtrlBType, DY_B, TRUE);    
	}

	CRect RectDlg, RectMo, RectCn, RectOrg, RectGroup;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDC_DGN_STEEL_GROUP)->GetWindowRect(&RectGroup);
	GetDlgItem(IDC_DGN_STEELMAT_MODIFY)->GetWindowRect(&RectMo);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	GetDlgItem(IDC_DGN_STEEL_MATFY10)->GetWindowRect(&RectOrg);

	if(GetControlType() == CONTROL_TYPE_A) // Type A
	{
		CString strCode = _T("");
		int Index = m_Code.GetCurSel();
		if (Index != -1)
			m_Code.GetLBText(Index, strCode);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

		CDlgUtil::CtrlShowHide(this, arCtrlAType, TRUE);
		CDlgUtil::CtrlShowHide(this, arCtrlBType, FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10)->GetWindowRect(&RectOrg);

		const std::unordered_map<unsigned short, unsigned int> kind2IDC = { {1, IDC_DGN_STEEL_MATFY1}, {2, IDC_DGN_STEEL_MATFY2}, {3, IDC_DGN_STEEL_MATFY3}, {4, IDC_DGN_STEEL_MATFY4}, {5, IDC_DGN_STEEL_MATFY5}, {6, IDC_DGN_STEEL_MATFY6}, {7, IDC_DGN_STEEL_MATFY7}, {10, IDC_DGN_STEEL_MATFY10} };

		if (strCode == _T(""))
			GetDlgItem(IDC_DGN_STEEL_MATFY10)->GetWindowRect(&RectOrg);
		else
		{
			CString strMatlName = _T("");
			int matIndex = m_MatName.GetCurSel();
			if (matIndex != -1)
				m_MatName.GetLBText(matIndex, strMatlName);

#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatlName);
#endif

			const auto nChkKind = GetChkKind(strCode);

			if (IsGB50018(strCode))
			{
				EnableMatlPara(TRUE);
			}
			else
			{
				auto foundIDC = kind2IDC.find(nChkKind);
				if (foundIDC != kind2IDC.cend())
					GetDlgItem(foundIDC->second)->GetWindowRect(&RectOrg);
				else
					GetDlgItem(IDC_DGN_STEEL_MATFY10)->GetWindowRect(&RectOrg);
			}
		}
	}
	else if(GetControlType() == CONTROL_TYPE_B) // Type B
	{
		CDlgUtil::CtrlShowHide(this,arCtrlAType,FALSE);
		CDlgUtil::CtrlShowHide(this,arCtrlBType,TRUE);
		GetDlgItem(IDC_DGN_STEEL_MINT3)->GetWindowRect(&RectOrg);		
	}

	CRect rDlg;
	rDlg.left		= RectDlg.left;
	rDlg.top		= RectDlg.top;
	rDlg.right	=	RectDlg.right;
	rDlg.bottom	= RectOrg.bottom + RectMo.Height() + globalUtils.ScaleByDPI(24); //(int)(2.5*RectMo.Height());
	
	CRect rGroup;
	rGroup.left		= RectGroup.left;
	rGroup.top		= RectGroup.top;
	rGroup.right	=	RectGroup.right;
	rGroup.bottom	= RectOrg.bottom + globalUtils.ScaleByDPI(8);
	
	CRect rMo;
	rMo.left		= RectMo.left;
	rMo.right		= RectMo.right;
	rMo.top			= rDlg.bottom - RectMo.Height() - globalUtils.ScaleByDPI(8);
	rMo.bottom	= rMo.top + RectMo.Height();

	CRect rCn;
	rCn.left		= RectCn.left;
	rCn.right		= RectCn.right;
	rCn.top			= rMo.top;
	rCn.bottom	= rMo.bottom;

	 
	
	ScreenToClient(rDlg);
	ScreenToClient(rGroup);
	ScreenToClient(rMo);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDC_DGN_STEEL_GROUP)->SetWindowPos(NULL, rGroup.left, rGroup.top, rGroup.Width(), rGroup.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDC_DGN_STEELMAT_MODIFY)->SetWindowPos(NULL, rMo.left, rMo.top, rMo.Width(), rMo.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	UpdateData(FALSE);
}

int CDgnCfMatDlg::GetControlType()
{	
	return CONTROL_TYPE_A;
}

CString CDgnCfMatDlg::GetDesignCodeName()
{	
	CString strDesingCode = _T("");
	if(m_pDoc->m_pAttrCtrl->ExistDcfs())
	{
		T_DCFS_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDcfs(rData);
		strDesingCode = rData.DesignCode;			
	}
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		strDesingCode = rPref.DgnCode.CFSteelCode;				
	}

	return strDesingCode;
}

BOOL CDgnCfMatDlg::GetMaterialData4KSCE05()
{
	//m_MatName.ResetContent();
	CArray<CString, CString&> NameList;
	CString strMatName[12] = {_T("SS400"), _T("SM400"), _T("SMA400"),
														_T("SM490"), _T("SM490TMC"),
														_T("SM490Y"), _T("SM520"), _T("SM520TMC"), _T("SMA490"),
														_T("SM570"), _T("SM570TMC"), _T("SMA570") };

	int iCodeNum = 12;
	NameList.SetSize(iCodeNum);  
	for(int i=0; i<iCodeNum; i++)
		NameList.SetAt(i,strMatName[i]);	

	CString strName = _T("");

	//int Index = m_MatName.GetCurSel();
	//m_MatName.GetLBText(Index, strName);

	double dAlwStress[9];
	double dThick[3];
	GetAlwStress4KSCE05(strName, dAlwStress, dThick);

	m_editAlwS1.SetEditUnit(dAlwStress[0]);
	m_editAlwS2.SetEditUnit(dAlwStress[1]);
	m_editAlwS3.SetEditUnit(dAlwStress[2]);
	m_editAlwS4.SetEditUnit(dAlwStress[3]);
	m_editAlwS5.SetEditUnit(dAlwStress[4]);
	m_editAlwS6.SetEditUnit(dAlwStress[5]);
	m_editAlwS7.SetEditUnit(dAlwStress[6]);
	m_editAlwS8.SetEditUnit(dAlwStress[7]);
	m_editAlwS9.SetEditUnit(dAlwStress[8]);
	m_editMinT1.SetEditUnit(dThick[0]);
	m_editMinT2.SetEditUnit(dThick[1]);		
	m_editMinT3.SetEditUnit(dThick[2]);		

	return true;
}

BOOL CDgnCfMatDlg::GetAlwStress4KSCE05(CString strMatName, double* dAlwStress, double* dThick)
{
	for(int i=0; i<9; i++)  dAlwStress[i] = 0.0;
	for(int i=0; i<3; i++)  dThick[i] = 0.0;

	T_UNIT_INDEX CodeUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;

	CDgnDataCtrl DgnDataCtrl;  

	if(strMatName==_T("SS400") || strMatName==_T("SM400") || strMatName==_T("SMA400"))
	{
		dAlwStress[0] = 140.0;   // 표 3.3.1 허용축방향인장응력 및 허용휨인장응력 
		dAlwStress[1] = 140.0;   // 표 3.3.2 국부좌굴을 고려하지 않은 허용축방향압축응력
		dAlwStress[2] = 140.0;   // 표 3.3.4(a) 허용휨압축응력
		dAlwStress[3] = 80.0;    // 표 3.3.5 허용전단응력
		dAlwStress[4] = 140.0;   // 표 3.3.2 국부좌굴을 고려하지 않은 허용축방향압축응력 상한값
		dAlwStress[5] = 140.0;   // 허용휨압축응력 상한값. 
		dAlwStress[6] = 140.0;   // 표 3.4.3 양연지지판의 국부좌굴에 대한 허용응력
		dAlwStress[7] = 140.0;   // 표 3.4.4 자유돌출판의 국부좌굴에 대한 허용응력
		dAlwStress[8] = 140.0;   // 표 3.4.6 보강된 판의 국부좌굴에 대한 허용응력
		//
		dThick[0] = 10.0;   // 표 3.4.2 압축응력을 받는 양연지지판의 최소판두께  
		dThick[1] = 10.0;   // 압축응력을 받는 자유돌출판의 최소두께
		dThick[2] = 10.0;   // 표 3.4.5 압축응력을 받는 보강된 판의 최소판두께

	}
	else if(strMatName==_T("SM490") || strMatName==_T("SM490TMC") )
	{
		dAlwStress[0] = 190.0;
		dAlwStress[1] = 190.0; 
		dAlwStress[2] = 190.0; 
		dAlwStress[3] = 110.0;  
		dAlwStress[4] = 190.0; 
		dAlwStress[5] = 190.0; 
		dAlwStress[6] = 190.0; 
		dAlwStress[7] = 190.0; 
		dAlwStress[8] = 190.0; 
		//
		dThick[0] = 10.0;   // 
		dThick[1] = 10.0;   // 
		dThick[2] = 10.0;   // 

	}
	else if(strMatName==_T("SM490Y") || strMatName==_T("SM520") || strMatName==_T("SM520TMC") || strMatName==_T("SMA490"))          
	{
		dAlwStress[0] = 210.0;
		dAlwStress[1] = 210.0; 
		dAlwStress[2] = 210.0; 
		dAlwStress[3] = 120.0;  
		dAlwStress[4] = 210.0; 
		dAlwStress[5] = 210.0; 
		dAlwStress[6] = 210.0; 
		dAlwStress[7] = 210.0; 
		dAlwStress[8] = 210.0; 
		//
		dThick[0] = 10.0;   // 
		dThick[1] = 10.0;   // 
		dThick[2] = 10.0;   // 

	}
	else if(strMatName==_T("SM570") || strMatName==_T("SM570TMC") || strMatName==_T("SM570-TMC")|| strMatName==_T("SMA570") )
	{
		dAlwStress[0] = 260.0;
		dAlwStress[1] = 260.0; 
		dAlwStress[2] = 260.0; 
		dAlwStress[3] = 150.0;  
		dAlwStress[4] = 260.0; 
		dAlwStress[5] = 260.0; 
		dAlwStress[6] = 260.0; 
		dAlwStress[7] = 260.0; 
		dAlwStress[8] = 260.0; 
		//
		dThick[0] = 10.0;   // 
		dThick[1] = 10.0;   // 
		dThick[2] = 10.0;   // 
	}
	else 
	{
		dThick[0] = 10.0;   // 
		dThick[1] = 10.0;   // 
		dThick[2] = 10.0;   //     
	}

	double dFlen = DgnDataCtrl.Change_Flen(CodeUnit.nBase_Length);
	double dFstr = DgnDataCtrl.Change_Fstr(CodeUnit.nBase_Length, CodeUnit.nBase_Force);
	for(int i=0; i<9; i++)
		dAlwStress[i] /= dFstr;

	for(int i=0; i<3; i++)
		dThick[i] /= dFlen;  

	return true;
}

void CDgnCfMatDlg::OnSelchangeDgnSteelKSCE05Name() 
{
	CString strName = _T("");
	int Index = m_KSCE05Name.GetCurSel();
	m_KSCE05Name.GetLBText(Index, strName);

	double dAlwStress[9];
	double dThick[3];
	GetAlwStress4KSCE05(strName, dAlwStress, dThick);

	m_editAlwS1.SetEditUnit(dAlwStress[0]);
	m_editAlwS2.SetEditUnit(dAlwStress[1]);
	m_editAlwS3.SetEditUnit(dAlwStress[2]);
	m_editAlwS4.SetEditUnit(dAlwStress[3]);
	m_editAlwS5.SetEditUnit(dAlwStress[4]);
	m_editAlwS6.SetEditUnit(dAlwStress[5]);
	m_editAlwS7.SetEditUnit(dAlwStress[6]);
	m_editAlwS8.SetEditUnit(dAlwStress[7]);
	m_editAlwS9.SetEditUnit(dAlwStress[8]);
	m_editMinT1.SetEditUnit(dThick[0]);
	m_editMinT2.SetEditUnit(dThick[1]);		
	m_editMinT3.SetEditUnit(dThick[2]);		
}

void CDgnCfMatDlg::Initial_KSCE05NameCombo()
{
	m_KSCE05Name.ResetContent();

	CArray<CString, CString&> NameList;
	CString strMatName[12] = {_T("SS400"), _T("SM400"), _T("SMA400"),
														_T("SM490"), _T("SM490TMC"),
														_T("SM490Y"), _T("SM520"), _T("SM520TMC"), _T("SMA490"),
														_T("SM570"), _T("SM570TMC"), _T("SMA570") };
	int iCodeNum = 12;
	NameList.SetSize(iCodeNum);  
	for(int i=0; i<iCodeNum; i++)
		m_KSCE05Name.AddString(strMatName[i]);  
}

BOOL CDgnCfMatDlg::IsGB50018_02()
{
	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	if (Index == -1)
		return FALSE;

	m_Code.GetLBText(Index, strCode);
	if (strCode == MATLCODE_STL_GB50018_02)
		return TRUE;
	else
		return FALSE;
}

BOOL CDgnCfMatDlg::IsGB50018_25()
{
	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	if (Index == -1)
		return FALSE;

	m_Code.GetLBText(Index, strCode);
	if (strCode == MATLCODE_STL_GB50018_25)
		return TRUE;
	else
		return FALSE;
}

BOOL CDgnCfMatDlg::IsGB50018(CString strCode)
{
	if (strCode == _T("None") || strCode == MATLCODE_STL_GB50018_02 || strCode == MATLCODE_STL_GB50018_25)
		return TRUE; /// none일 때도 보이게 한 것 같아서 우선 return TRUE;
	return FALSE;
}

void CDgnCfMatDlg::EnableMatlPara(BOOL bNewMatl)
{
	if (bNewMatl)
	{
		GetDlgItem(IDC_DGN_STATIC_GAMAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_GAMAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CF_MAT_ETA_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CF_MAT_ETA)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STEEL_MATES)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATPS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFU)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->EnableWindow(TRUE);

		if (IsGB50018_25())
		{
			GetDlgItem(IDC_DGN_STATIC_GAMAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_GAMAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CF_MAT_ETA_CHECK)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CF_MAT_ETA)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_DGN_STEEL_MATES)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATPS)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATFU)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATFY1)->EnableWindow(FALSE);

			GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_DGN_STATIC_MATFU)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STEEL_MATFUUNIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC2_FU)->EnableWindow(TRUE);

			GetDlgItem(IDC_DGN_STATIC_MATFY1)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_MATFY2)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_MATFY3)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_MATFY4)->EnableWindow(TRUE);

			GetDlgItem(IDC_DGN_STEEL_MATFY1UNIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->EnableWindow(TRUE);

			GetDlgItem(IDC_DGN_STATIC2_FY1)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_COLON1)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_COLON2)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_COLON3)->EnableWindow(TRUE);
		}
		else if (IsGB50018_02())
		{
			GetDlgItem(IDC_DGN_STATIC_GAMAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_GAMAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CF_MAT_ETA_CHECK)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CF_MAT_ETA)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_DGN_STEEL_MATES)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATPS)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATFU)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATFY1)->EnableWindow(FALSE);

			GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_DGN_STATIC_MATFU)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STEEL_MATFUUNIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC2_FU)->EnableWindow(TRUE);

			GetDlgItem(IDC_DGN_STATIC_MATFY1)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_MATFY2)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STATIC_MATFY3)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STATIC_MATFY4)->EnableWindow(FALSE);

			GetDlgItem(IDC_DGN_STEEL_MATFY1UNIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->EnableWindow(FALSE);

			GetDlgItem(IDC_DGN_STATIC2_FY1)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STATIC_COLON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STATIC_COLON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STATIC_COLON3)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_DGN_STATIC_GAMAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_GAMAR)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STEEL_MATES)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATPS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_CF_MAT_ETA_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CF_MAT_ETA)->ShowWindow(SW_HIDE);
	}
}

double CDgnCfMatDlg::GetGB50018_GammaR(CString strCode, CString strMatName)
{
	if (strCode == _T("GB50018-02(S)"))
	{
		if (strMatName == _T("Q390") || strMatName == _T("Q420") || strMatName == _T("Q460"))
		{
			m_dGammaR = 1.125;
		}
		else if (strMatName == _T("Q550") || strMatName == _T("Q235") || strMatName == _T("Q345"))
		{
			m_dGammaR = 1.165;
		}
		else
			m_dGammaR = 0.0;
	}
	else
	{
		if (strMatName == _T("Q390"))
			m_dGammaR = 1.125;
		else if (strMatName == _T("Q235") || strMatName == _T("Q355")
			|| strMatName == _T("S280") || strMatName == _T("S350") || strMatName == _T("S550"))
			m_dGammaR = 1.165;
		else
			m_dGammaR = 0.0;
	}
	return m_dGammaR;
}
void CDgnCfMatDlg::OnChkEtaGB50018()
{
	UpdateData(TRUE);
	CString strEta = _T("1.0");
	if (m_chkEta.GetCheck())
	{
		GetDlgItem(IDC_DGN_CF_MAT_ETA)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CF_MAT_ETA)->SetWindowText(strEta);
	}
	else
	{
		GetDlgItem(IDC_DGN_CF_MAT_ETA)->EnableWindow(FALSE);
	}
}