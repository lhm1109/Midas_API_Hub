// DgnTowerMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnTowerMatDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_db\wg_db_EditData.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnTowerMatDlg dialog


CDgnTowerMatDlg::CDgnTowerMatDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnTowerMatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnTowerMatDlg)
	m_MatName2 = _T("");
	m_Ps	= 0.0;
	m_Es	= 0.0;
	m_Fu	= 0.0;
	m_Fy1 = 0.0;
	m_Fy2 = 0.0;
	m_Fy3 = 0.0;
	m_Fy3 = 0.0;
	m_BoltMatName2 = _T("");
	m_BoltFu	= 0.0;
	m_BoltFy1 = 0.0;
	m_Fy4 = 0.0;
	//}}AFX_DATA_INIT
}


void CDgnTowerMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnTowerMatDlg)
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
	DDX_Control(pDX, IDC_DGN_BOLT_MATFY1UNIT, m_BoltFy1Unit);
	DDX_Control(pDX, IDC_DGN_BOLT_MATFUUNIT, m_BoltFuUnit);
	DDX_Control(pDX, IDC_DGN_BOLTMAT_CODE, m_BoltCode);
	DDX_Control(pDX, IDC_DGN_BOLT_MAT, m_BoltMatName);
	DDX_Text(pDX, IDC_DGN_BOLT_MAT2, m_BoltMatName2);
	DDX_Text(pDX, IDC_DGN_BOLT_MATFU, m_BoltFu);
	DDX_Text(pDX, IDC_DGN_BOLT_MATFY1, m_BoltFy1);
	//}}AFX_DATA_MAP
}


#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnTowerMatDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnTowerMatDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_STEELMAT_LIST, OnItemchangedDgnSteelmatList)
	ON_CBN_SELCHANGE(IDC_DGN_STEELMAT_CODE, OnSelchangeDgnSteelmatCode)
	ON_CBN_SELCHANGE(IDC_DGN_BOLTMAT_CODE, OnSelchangeDgnBoltmatCode)
	ON_BN_CLICKED(IDC_DGN_STEELMAT_MODIFY, OnDgnSteelmatModify)
	ON_CBN_SELCHANGE(IDC_DGN_STEEL_MAT, OnSelchangeDgnSteelMat)
	ON_CBN_SELCHANGE(IDC_DGN_BOLT_MAT, OnSelchangeDgnBoltMat)
	ON_NOTIFY(NM_CLICK, IDC_DGN_STEELMAT_LIST, OnClickDgnSteelmatList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnTowerMatDlg message handlers

BOOL CDgnTowerMatDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	Initial_ListCtrl();
	Initial_Data();
	Initial_Unit();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnTowerMatDlg::Initial_Unit()
{
	m_FuUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_EsUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy1Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy2Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy3Unit.SetUnitType(D_UNITSYS_BASE_ELAST);

	m_BoltFuUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_BoltFy1Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
}

void CDgnTowerMatDlg::Initial_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[7]	= {_T("ID"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), _T("Fu"), _T("Fy|Fy1"), _T("Bolt"), _T("Fu"), _T("Fy|Fy1")};
	int width[7]	= {40,72,66,66,72,66,66};
	for(int i=0; i<7; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}
}

void CDgnTowerMatDlg::Initial_Data()
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
			CString strID=_T(""), strStlNa=_T(""), strFu=_T(""), strFy1=_T("");
			CString strBoltNa=_T(""), strBoltFu=_T(""), strBoltFy1=_T("");

			strID.Format(_T("%7d"),nID);
			// Steel.
			strStlNa = mData.Name;
			double dFu  = mData.Data1.Design.S_Fu;
			double dFy1 = mData.Data1.Design.S_Fy1;
			CString strMatCode = mData.Data1.CodeName;
			ShowEditBox(GetChkKind(strMatCode));

			strFu.Format(_T("%g"),dFu);
			strFy1.Format(_T("%g"),dFy1);
			// Bolt.
			strBoltNa	 = mData.Data2.CodeMatlName;
			double dBoltFu  = mData.Data2.Design.S_Fu;
			double dBoltFy1 = mData.Data2.Design.S_Fy1;
			strBoltFu.Format(_T("%g"),dBoltFu);
			strBoltFy1.Format(_T("%g"),dBoltFy1);

			int iListLineNo = m_ListCtrl.GetItemCount();
			Write_ListCtrlBox(iListLineNo,strID,strStlNa,strFu,strFy1,strBoltNa,strBoltFu,strBoltFy1);
		}
	}
	EnableEditBox(TRUE);
	GetDlgItem(IDC_DGN_STEEL_MAT2)->ShowWindow(SW_HIDE);
	EnableBoltEditBox(TRUE);
	GetDlgItem(IDC_DGN_BOLT_MAT2)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void CDgnTowerMatDlg::Write_ListCtrlBox(int Index, CString strID, CString strStlNa, CString strFu,
																 CString strFy1, CString strBoltNa, CString strBoltFu, CString strBoltFy1)
{
	LV_ITEM lvitem;
	LPTSTR szText[7];
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)strStlNa;
	szText[2] = (LPTSTR)(LPCTSTR)strFu;
	szText[3] = (LPTSTR)(LPCTSTR)strFy1;
	szText[4] = (LPTSTR)(LPCTSTR)strBoltNa;
	szText[5] = (LPTSTR)(LPCTSTR)strBoltFu;
	szText[6] = (LPTSTR)(LPCTSTR)strBoltFy1;
	// Input ListCtrl Box.
	for(int i=0; i<7; i++)
	{
		lvitem.mask		  = LVIF_TEXT;
		lvitem.iItem  	= Index;
		lvitem.iSubItem = i;
		lvitem.pszText	= szText[i];
		if(i==0)	m_ListCtrl.InsertItem(&lvitem);
		else			m_ListCtrl.SetItem(&lvitem);
	}
}

void CDgnTowerMatDlg::OnClickDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnItemchangedDgnSteelmatList(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnTowerMatDlg::OnItemchangedDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem,0);

	T_MATD_D mData;
	mData.Initialize();
	int key = _ttoi(strID);
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(key, mData);
	if(bCheck)
	{
		///////////////////////
		// Steel.
		CString strName2	= m_ListCtrl.GetItemText(nItem,1);
		m_Es	= mData.Data1.Analysis.Elast;
		m_Ps	= mData.Data1.Analysis.Poisson;
		m_Fu  = mData.Data1.Design.S_Fu;
		m_Fy1 = mData.Data1.Design.S_Fy1;
		m_Fy2 = mData.Data1.Design.S_Fy2;
		m_Fy3 = mData.Data1.Design.S_Fy3;
		m_Fy4 = mData.Data1.Design.S_Fy4;

		CString strMatCode = mData.Data1.CodeName;
		ShowEditBox(GetChkKind(strMatCode));
		if(strMatCode==_T("") || strMatCode==_T("None"))	EnableEditBox(TRUE);
		else																			EnableEditBox(FALSE);
		// Initialize Steel Matl Code, Name ComboBox.
		Initial_StlMatlCombo(mData.Data1.CodeName);

		CString MatlName;
		MatlName = mData.Data1.CodeMatlName;
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(mData.Data1.CodeName, MatlName);
#endif
		int Index = m_MatName.FindStringExact(-1, MatlName);
		if(Index == -1) 
		{
			m_MatName2 = strName2;
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

		///////////////////////
		// Bolt.
		CString strBoltName2	= m_ListCtrl.GetItemText(nItem,4);
		m_BoltFu  = mData.Data2.Design.S_Fu;
		m_BoltFy1 = mData.Data2.Design.S_Fy1;
		CString strBoltCode	= mData.Data2.CodeName;
		if(strBoltCode==_T("") || strBoltCode==_T("None"))	EnableBoltEditBox(TRUE);
		else																				EnableBoltEditBox(FALSE);
		// Initialize Bolt Matl Code, Name ComboBox.
		Initial_BoltMatlCombo(mData.Data2.CodeName);

		MatlName = mData.Data2.CodeMatlName;
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(mData.Data2.CodeName, MatlName);
#endif
		Index = m_BoltMatName.FindStringExact(-1, MatlName);
		if(Index == -1) 
		{
			m_BoltMatName2 = strBoltName2;
			GetDlgItem(IDC_STATIC_BOLT_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
			GetDlgItem(IDC_DGN_BOLT_MAT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_BOLT_MAT2)->ShowWindow(SW_SHOW);
		}
		else 
		{
			m_BoltMatName.SetCurSel(Index);
			GetDlgItem(IDC_STATIC_BOLT_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
			GetDlgItem(IDC_DGN_BOLT_MAT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_BOLT_MAT2)->ShowWindow(SW_HIDE);
		}

		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CDgnTowerMatDlg::EnableEditBox(BOOL bCheck)
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
	}
}

void CDgnTowerMatDlg::EnableBoltEditBox(BOOL bCheck)
{
	if(bCheck)	// Enable.
	{
		GetDlgItem(IDC_DGN_BOLT_MATFU)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BOLT_MATFY1)->EnableWindow(TRUE);
	}
	else	// Disable.
	{
		GetDlgItem(IDC_DGN_BOLT_MATFU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BOLT_MATFY1)->EnableWindow(FALSE);
	}
}

void CDgnTowerMatDlg::ShowEditBox(int nCheck)
{
//----------Changed by Hong,jiseon 2002.7.19  
	if(nCheck==1)	// ASTM(S), BS(S), JTG04(S), None.
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY));

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_HIDE);
	}
	else if(nCheck==2)	// KS(S), JIS(S), DIN(S), EN(S), UNI(S), CNS(S). CNS06(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY));

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_HIDE);
	}
	else if(nCheck==3)	// KS-Civil(S), GB(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_COLON1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_COLON3)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));
		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_HIDE);
	}
	else if(nCheck==4)	// JGJ(S), JTJ(S), GB03(S).
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
	}
}

void CDgnTowerMatDlg::Initial_StlMatlCombo(CString strCode)
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

	CArray<CString, CString&> NameList;
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

void CDgnTowerMatDlg::Initial_BoltMatlCombo(CString strCode)
{
	m_BoltCode.ResetContent();
	m_BoltMatName.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DesignCodeList);
	m_BoltCode.AddString(_T("None"));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
		m_BoltCode.AddString(DesignCodeList[i]);
	}

	if(strCode==_T(""))	strCode = _T("None");

	CString CodeName;
	CodeName = strCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	int Index = m_BoltCode.FindStringExact(-1, CodeName);
	m_BoltCode.SetCurSel(Index);

	CArray<CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(strCode,NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, NameList[i]);
#endif
		m_BoltMatName.AddString(NameList[i]);
	}

	UpdateData(FALSE);
}

void CDgnTowerMatDlg::OnSelchangeDgnSteelmatCode() 
{
	CString strCode=_T("");
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
	}
	m_MatName2 = _T("");
	UpdateData(FALSE);
}

void CDgnTowerMatDlg::OnSelchangeDgnBoltmatCode() 
{
	CString strCode=_T("");
	int Index = m_BoltCode.GetCurSel();
	m_BoltCode.GetLBText(Index,strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	m_BoltMatName.ResetContent();
	// Change Material Name by Code.
	if(strCode==_T("None") || strCode==_T(""))
	{
		GetDlgItem(IDC_STATIC_BOLT_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
		GetDlgItem(IDC_DGN_BOLT_MAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BOLT_MAT2)->ShowWindow(SW_SHOW);
		EnableBoltEditBox(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_BOLT_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
		GetDlgItem(IDC_DGN_BOLT_MAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BOLT_MAT2)->ShowWindow(SW_HIDE);
		EnableBoltEditBox(FALSE);

		CArray<CString, CString&> NameList;
		m_pDoc->m_pMatlDB->GetSteelNameList(strCode, NameList);
		for (int i = 0; i < NameList.GetSize(); i++)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, NameList[i]);
#endif
			m_BoltMatName.AddString(NameList[i]);
		}
	}

	if(strCode!=_T("None"))
	{
		m_BoltFu	= 0.0;
		m_BoltFy1 = 0.0;
	}
	m_BoltMatName2 = _T("");
	UpdateData(FALSE);
}

void CDgnTowerMatDlg::OnSelchangeDgnSteelMat() 
{
	CString strNa=_T("");
	int Index = m_MatName.GetCurSel();
	m_MatName.GetLBText(Index, strNa);

	Index = m_Code.GetCurSel();
	CString strCode=_T("");
	m_Code.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strNa);
#endif

	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode,strNa,sData);
	if(bCheck)
	{
		m_Es	= sData.Elast;
		m_Ps	= sData.Poisson;
		m_Fu	= sData.S_Fu;
		m_Fy1 = sData.S_Fy1;
		m_Fy2 = sData.S_Fy2;
		m_Fy3 = sData.S_Fy3;
		m_Fy4 = sData.S_Fy4;

		UpdateData(FALSE);
	}
}

void CDgnTowerMatDlg::OnSelchangeDgnBoltMat() 
{
	CString strNa=_T("");
	int Index = m_BoltMatName.GetCurSel();
	m_BoltMatName.GetLBText(Index, strNa);

	Index = m_BoltCode.GetCurSel();
	CString strCode=_T("");
	m_BoltCode.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strNa);
#endif

	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode,strNa,sData);
	if(bCheck)
	{
		m_BoltFu	= sData.S_Fu;
		m_BoltFy1 = sData.S_Fy1;

		UpdateData(FALSE);
	}
}

int CDgnTowerMatDlg::GetChkKind(CString strMatlCode)
{
	CDgnDataCtrl DataCtrl;
	return DataCtrl.GetChkKindStlMatl(strMatlCode);
}

BOOL CDgnTowerMatDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	/////////////////////////
	// Steel.
	if(m_Code.GetCurSel()==0)	
	{
		if(m_MatName2==_T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);}
	}
	if(m_Es <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ES_ERROR);}
	if(m_Ps <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_PS_ERROR);}
	if(m_Fu <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FU_ERROR);}
	// Code¿¡ µû¸¥ Error Checking.
	int Index = m_Code.GetCurSel();
	CString strCode = _T("");
	m_Code.GetLBText(Index, strCode);  
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	int iChkKind = GetChkKind(strCode);
	if(iChkKind==1)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
		m_Fy2 = m_Fy1;
		m_Fy3 = m_Fy1;
		m_Fy4 = m_Fy1;
	}
	else if(iChkKind==2)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		m_Fy3 = m_Fy2;
		m_Fy4 = m_Fy2;
	}
	else if(iChkKind==3)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		m_Fy4 = m_Fy3;
	}
	else if(iChkKind==4)
	{
		if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR);}
		if(m_Fy2 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR);}
		if(m_Fy3 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR);}
		if(m_Fy4 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR);}
	}
	else	ASSERT(0);

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CDgnTowerMatDlg::OnDgnSteelmatModify() 
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL),MB_OK);
		return;
	}

	CString strID=_T(""), strStlNa=_T(""), strFu=_T(""), strFy1=_T("");
	CString	strBoltNa=_T(""), strBoltFu=_T(""), strBoltFy1=_T("");

	strID		= m_ListCtrl.GetItemText(nItem, 0);
	int nID	= _ttoi(strID);

	T_MATD_D mData;
	mData.Initialize();
	m_pDoc->m_pAttrCtrl->GetMatlDesign(nID, mData);

	UpdateData(TRUE);
	if(ErrorCheck())
	{
		/////////////////////////
		// Steel.
		CString strCode=_T(""), strMatName=_T("");
		int nCode = m_Code.GetCurSel();
		if(nCode==0)	// Material Code = None
		{
			strMatName	= m_MatName2;
			strStlNa		= m_MatName2;
		}
		else	// Material Code = Code
		{
			m_Code.GetLBText(nCode, strCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
			int nName = m_MatName.GetCurSel();
			m_MatName.GetLBText(nName, strMatName);
			strStlNa	= strMatName;

#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatName);
#endif

			T_MATL_STEEL sData;
			BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode, strMatName, sData);
			ASSERT(bCheck);
		}
		strFu.Format(_T("%g"),m_Fu);
		strFy1.Format(_T("%g"),m_Fy1);

		mData.Name										= strStlNa;
		mData.Data1.CodeName					= strCode;
		mData.Data1.CodeMatlName			= strMatName;
		mData.Data1.Analysis.Elast		= m_Es;
		mData.Data1.Analysis.Poisson	= m_Ps;
		mData.Data1.Design.S_Fu				= m_Fu;
		mData.Data1.Design.S_Fy1			= m_Fy1;
		mData.Data1.Design.S_Fy2			= m_Fy2;
		mData.Data1.Design.S_Fy3			= m_Fy3;
		mData.Data1.Design.S_Fy4			= m_Fy4;

		/////////////////////////
		// Bolt.
		CString strBoltCode=_T(""), strBoltMatName=_T("");
		int nBoltCode = m_BoltCode.GetCurSel();
		if(nBoltCode==0)	// Material Code = None
		{
			strBoltMatName	= m_BoltMatName2;
			strBoltNa				= m_BoltMatName2;
		}
		else	// Material Code = Code
		{
			m_BoltCode.GetLBText(nBoltCode, strBoltCode);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strBoltCode);
#endif
			int nName	= m_BoltMatName.GetCurSel();
			m_BoltMatName.GetLBText(nName, strBoltMatName);
			strBoltNa	= strBoltMatName;

#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strBoltCode, strBoltMatName);
#endif

			T_MATL_STEEL sData;
			BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strBoltCode,strBoltMatName,sData);
			ASSERT(bCheck);
		}
		strBoltFu.Format(_T("%g"),m_BoltFu);
		strBoltFy1.Format(_T("%g"),m_BoltFy1);

		mData.Data2.CodeName					= strBoltCode;
		mData.Data2.CodeMatlName			= strBoltMatName;
		mData.Data2.Design.S_Fu				= m_BoltFu;
		mData.Data2.Design.S_Fy1			= m_BoltFy1;

		// Add Material Data.
		BOOL bCheck = m_pDoc->m_pDataCtrl->AddMatd(nID, mData);
		if(bCheck)
		{
			m_ListCtrl.SetItemText(nItem,1,strStlNa);		// Steel Name.
			m_ListCtrl.SetItemText(nItem,2,strFu);			// Fu.
			m_ListCtrl.SetItemText(nItem,3,strFy1);			// Fy1.
			m_ListCtrl.SetItemText(nItem,4,strBoltNa);	// Bolt Name.
			m_ListCtrl.SetItemText(nItem,5,strBoltFu);	// Bolt Fu.
			m_ListCtrl.SetItemText(nItem,6,strBoltFy1);	// Bolt Fy1.
			m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CDgnTowerMatDlg::OnOK() 
{
	CDialogMove::OnOK();
}

void CDgnTowerMatDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}
