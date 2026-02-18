// DgnSodMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSodMatDlg.h"

#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSodMatDlg dialog
CDgnSodMatDlg::CDgnSodMatDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSodMatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSodMatDlg)
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
	m_Gamma_m= 0.0;

	m_arCtrlCType.Add(IDC_DGN_STATIC_MATFY_DRF);
	m_arCtrlCType.Add(IDC_DGN_STEEL_MATFY_DRF);
	m_nOldItem = -1;

	//}}AFX_DATA_INIT
}


void CDgnSodMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSodMatDlg)
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
	DDX_Text(pDX, IDC_DGN_STEEL_MATFY_DRF, m_Gamma_m);

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnSodMatDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnSodMatDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_STEELMAT_LIST, OnItemchangedDgnSteelmatList)
	ON_CBN_SELCHANGE(IDC_DGN_STEELMAT_CODE,           OnSelchangeDgnSteelmatCode)
	ON_CBN_SELCHANGE(IDC_DGN_STEEL_MAT,               OnSelchangeDgnSteelMat)
	ON_BN_CLICKED(IDC_DGN_STEELMAT_MODIFY,            OnDgnSteelmatModify)
	ON_NOTIFY(NM_CLICK, IDC_DGN_STEELMAT_LIST,        OnClickDgnSteelmatList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSodMatDlg message handlers

BOOL CDgnSodMatDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	ControlMan(TRUE);

	Initial_ListCtrl();
	Initial_Data();
	Initial_Unit();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSodMatDlg::Initial_Unit()
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
}

void CDgnSodMatDlg::Initial_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR *list[14]	= {_T("ID"),  (LPTSTR)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), _T("Es"), _T("Fu"), _T("Fy|Fy1"), _T("Fy2"), _T("Fy3"), _T("Fy4"), _T("Fy5"), _T("Fy6"), _T("Fy7"), _T("Fy8"), _T("Fy9"), _T("Fy10")};
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

void CDgnSodMatDlg::Initial_Data()
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
			CString strMatCode = mData.Data1.CodeName;
			ShowEditBox(GetChkKind(strMatCode));
			
			CString strEs=_T("");  strEs.Format( _T("%g"),mData.Data1.Analysis.Elast);
			CString strFu=_T("");  strFu.Format( _T("%g"),mData.Data1.Design.S_Fu);
			CString strFy1=_T(""); strFy1.Format(_T("%g"),mData.Data1.Design.S_Fy1);
			CString strFy2=_T(""); strFy2.Format(_T("%g"),mData.Data1.Design.S_Fy2);
			CString strFy3=_T(""); strFy3.Format(_T("%g"),mData.Data1.Design.S_Fy3);
			CString strFy4=_T(""); strFy4.Format(_T("%g"),mData.Data1.Design.S_Fy4);
			CString strFy5=_T(""); strFy5.Format(_T("%g"),mData.Data1.Design.S_Fy5);
			CString strFy6=_T(""); strFy6.Format(_T("%g"),mData.Data1.Design.S_Fy6);
			CString strFy7=_T(""); strFy7.Format(_T("%g"), mData.Data1.Design.S_Fy7);
			CString strFy8=_T(""); strFy8.Format(_T("%g"), mData.Data1.Design.S_Fy8);
			CString strFy9=_T(""); strFy9.Format(_T("%g"), mData.Data1.Design.S_Fy9);
			CString strFy10=_T(""); strFy10.Format(_T("%g"), mData.Data1.Design.S_Fy10);
			int ListLineNo = m_ListCtrl.GetItemCount();

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
				strFy10= _T("-");
			}
			////////////////////////////////////////////////////////////////////

			Write_ListCtrlBox(ListLineNo,strID,strName,strEs,strFu,strFy1,strFy2,strFy3,strFy4,strFy5,strFy6,strFy7,strFy8,strFy9,strFy10);
		}
	}
	EnableEditBox(TRUE);
	GetDlgItem(IDC_DGN_STEEL_MAT2)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void CDgnSodMatDlg::Write_ListCtrlBox(int Index, CString strID, CString strName, CString strEs, CString strFu,
																		 CString strFy1, CString strFy2, CString strFy3, CString strFy4, CString strFy5, CString strFy6, CString strFy7, CString strFy8, CString strFy9, CString strFy10)
{
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

void CDgnSodMatDlg::OnClickDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
// 	OnItemchangedDgnSteelmatList(pNMHDR,pResult);
// 	*pResult = 0;
}

void CDgnSodMatDlg::OnItemchangedDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SRCMAT_LIST, OnItemchangedDgnSrcmatList)
	//  ON_NOTIFY(NM_CLICK, IDC_DGN_SRCMAT_LIST, OnClickDgnSrcmatList)
	//  1. NM_CLICK (�������� ���� ��Ʈ���� ���� �޽���) - Ű������ ���õ� ������
	//  2. LVN_ITEMCHANGED (����Ʈ ��Ʈ���� ������ �׸� ���濡 ���� ���� �޽���)  - Ű������ ���õ� ������
	//
	// ���⼭�� Ű������ ���õ� ����ϱ����� LVN_ITEMCHANGED�� �����..
	// ��� LVN_ITEMCHANGED�� 3�� ���� ȣ��ǹǷ� m_OldItem�� ���� ��Ʈ�� ��.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	if(m_nOldItem == nItem)
		return;
	else  
		m_nOldItem = nItem;

	CString strID = m_ListCtrl.GetItemText(nItem,0);

	T_MATD_D mData;
	mData.Initialize();
	int key = _ttoi(strID);
	BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(key, mData);
	if(bCheck)
	{
		m_Es	= mData.Data1.Analysis.Elast;
		m_Ps	= mData.Data1.Analysis.Poisson;
		m_Fu  = mData.Data1.Design.S_Fu;
		m_Fy1 = mData.Data1.Design.S_Fy1;
		m_Fy2 = mData.Data1.Design.S_Fy2;
		m_Fy3 = mData.Data1.Design.S_Fy3;
		m_Fy4 = mData.Data1.Design.S_Fy4;
		m_Fy5 = mData.Data1.Design.S_Fy5;
		m_Fy6 = mData.Data1.Design.S_Fy6;
		m_Fy7 = mData.Data1.Design.S_Fy7;
		m_Fy8 = mData.Data1.Design.S_Fy8;
		m_Fy9 = mData.Data1.Design.S_Fy9;
		m_Fy10 = mData.Data1.Design.S_Fy10;
		m_Gamma_m= mData.Data1.Design.dGamma_m;

		// Change by ZINU.('02.9.17).
		CString strMatCode = mData.Data1.CodeName;
		ShowEditBox(GetChkKind(strMatCode));
		if(strMatCode==_T("") || strMatCode==_LS(IDS_DGN_NONE))	EnableEditBox(TRUE);
		else																			EnableEditBox(FALSE);
		// Material Code�� Name ComboBox�� �ʱ�ȭ�մϴ�.
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

}

void CDgnSodMatDlg::EnableEditBox(BOOL bCheck)
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
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY6)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY7)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY8)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY9)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10)->EnableWindow(TRUE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlCType,FALSE);
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
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY9)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10)->EnableWindow(FALSE);
		CDlgUtil::CtrlShowHide(this,m_arCtrlCType,FALSE);
	}
}

void CDgnSodMatDlg::ShowEditBox(int nCheck)
{
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
		GetDlgItem(IDC_DGN_STEEL_MATFY_DRF)->ShowWindow(SW_HIDE);

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
		GetDlgItem(IDC_DGN_STATIC_MATFY_DRF)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STEEL_MATFY2UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY3UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY4UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY5UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY6UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY7UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY8UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY9UNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STEEL_MATFY10UNIT)->ShowWindow(SW_HIDE);
	}

	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	if(Index != -1)
		m_Code.GetLBText(Index,strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	CString strDgnCode = GetDesignCodeName();
	if(strDgnCode==_T("KSCE-ASD05") && (strCode== MATLCODE_STL_KS_CIVIL || strCode== MATLCODE_STL_KS08_CIVIL)) // Modify, Jaeoh.(08.10.30)
	{
		return;   
	}

	if(strCode== _LS(IDS_DGN_NONE))
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY_DRF)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY_DRF)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY));
		
	}
	else if(nCheck==1)	// ASTM(S), None.
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
				
		GetDlgItem(IDC_DGN_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STATIC_MATFY1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY));
	}
	else if(nCheck==2)	// KS(S), JIS(S), DIN(S), EN05(S), EN(S), UNI(S), CNS(S). CNS06(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);

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
	else if(nCheck==5)	// BC1:12-ASTM(S), BC1:12-GB(S).
	{
		GetDlgItem(IDC_DGN_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_STEEL_MATFY5)->ShowWindow(SW_SHOW);
		
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

void CDgnSodMatDlg::Initial_MaterialCombo(CString strCode, CString strName)
{
	m_Code.ResetContent();
	m_MatName.ResetContent();

	CArray<CString, CString&> DesignCodeList;
	CString strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, DesignCodeList);
	m_Code.AddString(_LS(IDS_DGN_NONE));
	for (int i = 0; i < DesignCodeList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(DesignCodeList[i]);
#endif
		m_Code.AddString(DesignCodeList[i]);
	}

	if(strCode==_T(""))	strCode = _LS(IDS_DGN_NONE);

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

void CDgnSodMatDlg::OnSelchangeDgnSteelmatCode() 
{
	ControlMan(FALSE);

	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	m_MatName.ResetContent();
	// Change Material Name by Code.
	if(strCode== _LS(IDS_DGN_NONE) || strCode==_T(""))
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

	if(strCode!= _LS(IDS_DGN_NONE))
	{
		m_Es	= 0.0;
		m_Ps	= 0.0;
		m_Fu	= 0.0;
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
		m_Gamma_m= 0.0;
	}
	m_MatName2 = _T("");

	UpdateData(FALSE);

}

void CDgnSodMatDlg::OnSelchangeDgnSteelMat() 
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
	if(bCheck)
	{
		m_Es	= sData.Elast;
		m_Ps	= sData.Poisson;
		m_Fu	= sData.S_Fu;
		m_Fy1 = sData.S_Fy1;
		m_Fy2 = sData.S_Fy2;
		m_Fy3 = sData.S_Fy3;
		m_Fy4 = sData.S_Fy4;
		m_Fy5 = sData.S_Fy5;
		m_Fy6 = sData.S_Fy6;
		m_Fy7 = sData.S_Fy7;
		m_Fy8 = sData.S_Fy8;
		m_Fy9 = sData.S_Fy9;
		m_Fy10 = sData.S_Fy10;

		UpdateData(FALSE);
	}
}

int CDgnSodMatDlg::GetChkKind(CString strMatlCode)
{
	CDgnDataCtrl DataCtrl;
	return DataCtrl.GetChkKindStlMatl(strMatlCode);
}

BOOL CDgnSodMatDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	if(m_Code.GetCurSel()==0)	
	{
		if(m_MatName2 == _T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);}
	}
	if(m_Es <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ES_ERROR);}
	if(m_Ps <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_PS_ERROR);}
	// Change, Jaeoh.(08.03.14) Pre���� �Է��� Material�� None�� ��� KR, Steel Design, KSCE-ASD05�� �� 
	// �Է��� �ȵ�. Skip��.
	if(GetDesignCodeName()!=_T("KSCE-ASD05"))
		if(m_Fu <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FU_ERROR);}

	// Code�� ���� Error Checking.
	int Index = m_Code.GetCurSel();
	CString strCode=_T("");
	m_Code.GetLBText(Index, strCode);  
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	int iChkKind = GetChkKind(strCode);
	if(iChkKind==1)
	{
		if(GetDesignCodeName()!=_T("KSCE-ASD05"))
			if(m_Fy1 <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
		m_Fy2 = m_Fy1;
		m_Fy3 = m_Fy1;
		m_Fy4 = m_Fy1;
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
		m_Fy5 = m_Fy4;
		m_Fy6 = m_Fy4;
		m_Fy7 = m_Fy4;
		m_Fy8 = m_Fy4;
		m_Fy9 = m_Fy4;
		m_Fy10 = m_Fy4;
	}
	else if(iChkKind==5) 
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
	else if(iChkKind==6) 
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
		if (m_Fy1 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR); }
		if (m_Fy2 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR); }
		if (m_Fy3 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY3_ERROR); }
		if (m_Fy4 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY4_ERROR); }
		if (m_Fy5 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY5_ERROR); }
		if (m_Fy6 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY6_ERROR); }
		if (m_Fy7 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY7_ERROR); }
		if (m_Fy8 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY8_ERROR); }
		if (m_Fy9 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY9_ERROR); }
		if (m_Fy10 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY10_ERROR); }
	}
	else	ASSERT(0);


	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CDgnSodMatDlg::OnDgnSteelmatModify() 
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
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
			int nName = m_MatName.GetCurSel();
			m_MatName.GetLBText(nName, strMatName);
			strName = strMatName;

#if defined(_RUS)
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
		mData.Data1.Design.S_Fy1	= m_Fy1;
		mData.Data1.Design.S_Fy2	= m_Fy2;
		mData.Data1.Design.S_Fy3	= m_Fy3;
		mData.Data1.Design.S_Fy4	= m_Fy4;
		mData.Data1.Design.S_Fy5	= m_Fy5;
		mData.Data1.Design.S_Fy6	= m_Fy6;
		mData.Data1.Design.S_Fy7	= m_Fy7;
		mData.Data1.Design.S_Fy8	= m_Fy8;
		mData.Data1.Design.S_Fy9	= m_Fy9;
		mData.Data1.Design.S_Fy10	= m_Fy10;
		mData.Data1.Design.dGamma_m	= m_Gamma_m;

		

		BOOL bCheck = m_pDoc->m_pDataCtrl->AddMatd(nID, mData);
		if(bCheck)
		{
			CString strEs=_T("");  strEs.Format( _T("%g"),m_Es);
			CString strFu=_T("");  strFu.Format( _T("%g"),m_Fu);
			CString strFy1=_T(""); strFy1.Format(_T("%g"),m_Fy1);
			CString strFy2=_T(""); strFy2.Format(_T("%g"),m_Fy2);
			CString strFy3=_T(""); strFy3.Format(_T("%g"),m_Fy3);
			CString strFy4=_T(""); strFy4.Format(_T("%g"),m_Fy4);
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

void CDgnSodMatDlg::OnOK() 
{
	CDialogMove::OnOK();
}

void CDgnSodMatDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnSodMatDlg::ControlMan(BOOL bInit)
{
	CArray<UINT, UINT> arCtrlAType;
	CDlgUtil::GetCtrlIDByIncRect(this,arCtrlAType,IDC_DGN_TYPE_A_HOLDER, FALSE);
		
	if(bInit)
	{
		CDlgUtil::CtrlShowHide(this,m_arCtrlCType,FALSE);
	}

	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	if(Index != -1)
		m_Code.GetLBText(Index,strCode);
	
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	CString strDgnCode = GetDesignCodeName();
	
	if(strCode== _LS(IDS_DGN_NONE))
	{
		CDlgUtil::CtrlShowHide(this,arCtrlAType,FALSE);
		
		CRect AHRect, BHRect;
		GetDlgItem(IDC_DGN_STATIC_MATFY2)->GetWindowRect(AHRect);
		GetDlgItem(IDC_DGN_STATIC_MATFY_DRF)->GetWindowRect(BHRect);
		int DY_B = AHRect.top - BHRect.top - globalUtils.ScaleByDPI(2);	
		CDlgUtil::CtrlMoveDistY(this, m_arCtrlCType, DY_B, TRUE);    
	}
	if(strDgnCode==_T("KSCE-ASD05") && (strCode== MATLCODE_STL_KS_CIVIL || strCode== MATLCODE_STL_KS08_CIVIL))
	{
		CDlgUtil::CtrlShowHide(this,arCtrlAType,FALSE);
	}
	else
	{
		// TODO : ���� �ʿ�
		CDlgUtil::CtrlShowHide(this,arCtrlAType,TRUE);
	}		

	UpdateData(FALSE);
}


CString CDgnSodMatDlg::GetDesignCodeName()
{	
	CString strDesingCode = _T("");
	if(m_pDoc->m_pAttrCtrl->ExistDstl())
	{
		T_DSTL_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDstl(rData);
		strDesingCode = rData.DesignCode;			
	}
	else
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		strDesingCode = rPref.DgnCode.SteelCode;				
	}

	return strDesingCode;
}

