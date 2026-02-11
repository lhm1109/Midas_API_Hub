// DgnSrcMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiMaterialSteelDlg_JP.h"
#include "MPhiMaterialDlg_JP.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "MPhiParameterDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPhiMaterialSteelDlg_JP dialog


CMPhiMaterialSteelDlg_JP::CMPhiMaterialSteelDlg_JP(MPhiDataAll* pMPhiDataAll, CMPhiMaterialDlg_JP* pParent /*=NULL*/, int nType /*=0*/)
	: CDlgChild(CMPhiMaterialSteelDlg_JP::IDD, pParent)
{
	m_pParent = pParent;
	m_MatName2 = _T("");
	m_SheathSteelName2 = _T("");

	m_Ps = 0.0;
	m_Es = 0.0;
	m_Fu = 0.0;
	m_Fy1 = 0.0;
	m_Fy2 = 0.0;
	m_SteelSurrend = 0.0;
	m_SheathEc = 0.0;
	m_SheathAck = 0.0;
	m_SheathHeight = 0.0;
	m_SheathWeight = 0.0;
	m_pMPhiDataAll = pMPhiDataAll;
}


void CMPhiMaterialSteelDlg_JP::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMD_STEELMAT_LIST, m_ListCtrl);
	DDX_Check(pDX, IDC_CMD_SHEATH_GRP_CBX, m_bSheath);

	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_CODE_COB, m_SheathSteelCode);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_NAME_COB, m_SheathSteelName);
	DDX_Control(pDX, IDC_CMD_STEELMAT_CODE, m_Code);
	DDX_Control(pDX, IDC_CMD_STEEL_MAT, m_MatName);

	DDX_Text(pDX, IDC_CMD_STEEL_MAT2, m_MatName2);
	DDX_Text(pDX, IDC_CMD_SHEATH_STELL_NAME_EDT, m_SheathSteelName2);

	DDX_Text(pDX, IDC_CMD_STEEL_MATPS, m_Ps);
	DDX_Text(pDX, IDC_CMD_STEEL_MATES, m_Es);
	DDX_Text(pDX, IDC_CMD_STEEL_MATFU, m_Fu);
	DDX_Text(pDX, IDC_CMD_STEEL_MATFY1, m_Fy1);
	DDX_Text(pDX, IDC_CMD_STEEL_MATFY2, m_Fy2);
	DDX_Text(pDX, IDC_CMD_SHEATH_STELL_SURRENDER_EDT, m_SteelSurrend);
	DDX_Text(pDX, IDC_SHEATH_STEEL_EC_EDT, m_SheathEc);
	DDX_Text(pDX, IDC_SHEATH_STEEL_ACK_EDT, m_SheathAck);
	DDX_Text(pDX, IDC_SHEATH_STEEL_HEIGHT_EDT, m_SheathHeight);
	DDX_Text(pDX, IDC_SHEATH_STEEL_WEIGHT_EDT, m_SheathWeight);

	DDX_Control(pDX, IDC_CMD_STEEL_MATPSUNIT, m_PsUnit);
	DDX_Control(pDX, IDC_CMD_STEEL_MATESUNIT, m_EsUnit);
	DDX_Control(pDX, IDC_CMD_STEEL_MATFUUNIT, m_FuUnit);
	DDX_Control(pDX, IDC_CMD_STEEL_MATFY1UNIT, m_Fy1Unit);
	DDX_Control(pDX, IDC_CMD_STEEL_MATFY2UNIT, m_Fy2Unit);
	DDX_Control(pDX, IDC_CMD_SHEATH_STELL_SURRENDER_UNIT, m_SteelSurrendUnit);
	DDX_Control(pDX, IDC_SHEATH_STEEL_EC_UNIT, m_SheathEcUnit);
	DDX_Control(pDX, IDC_SHEATH_STEEL_ACK_UNIT, m_SheathAckUnit);
	DDX_Control(pDX, IDC_SHEATH_STEEL_HEIGHT_UNIT, m_SheathHeightUnit);
	DDX_Control(pDX, IDC_SHEATH_STEEL_WEIGHT_UNIT, m_SheathWeightUnit);
	DDX_Control(pDX, IDC_CMD_SHEATH_S_REIN_FOLD, m_FoldGrp);

}

BEGIN_MESSAGE_MAP(CMPhiMaterialSteelDlg_JP, CDlgChild)
	ON_NOTIFY(NM_CLICK, IDC_CMD_STEELMAT_LIST, OnClickDgnSteelmatList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_STEELMAT_LIST, OnItemchangedDgnSteelmatList)
	ON_CBN_SELCHANGE(IDC_CMD_STEELMAT_CODE, OnSelchangeDgnSteelmatCode)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_STELL_CODE_COB, OnChangeSheathSteelCode)
	ON_CBN_SELCHANGE(IDC_CMD_SHEATH_STELL_NAME_COB, OnChangeSheathSteelName)
	ON_CBN_SELCHANGE(IDC_CMD_STEEL_MAT, OnSelchangeDgnSteelMat)
	ON_BN_CLICKED(IDC_CMD_SHEATH_GRP_CBX, OnClickbSheath)
	ON_BN_CLICKED(IDC_BTN_MPHI_MATL_ST_ADD_MOD, OnAddModBtn)
	ON_BN_CLICKED(IDC_BTN_MPHI_MATL_ST_DELETE, OnDeleteBtn)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMPhiMaterialSteelDlg_JP::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDlgChild::OnInitDialog();

	Initial_ListCtrl();
	Initial_Data();
	Initial_Unit();

	// Sheath 
	m_bSheath = FALSE;
	GetDlgItem(IDC_CMD_SHEATH_GRP_CBX)->EnableWindow(FALSE);
	m_FoldGrp.SetButtonShowHide(FALSE);

	Init_SheathCase();
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(TRUE);

	UpdateData(FALSE);

	m_FoldGrp.InitControl(this, CMPhiMaterialSteelDlg_JP::IDD, IDC_CMD_SHEATH_S_REIN_FOLD, FALSE, FALSE);
	if (m_FoldGrp.GetFoldState()) m_FoldGrp.SetFoldState(FALSE, FALSE); // 접힌 상태로 시작
	m_FoldGrp.SetFoldState(TRUE, TRUE); // 접힌 상태로 시작
	AlignBtnMove();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMPhiMaterialSteelDlg_JP::AlignBtnMove()
{
	CArray <UINT, UINT> aControls;
	aControls.Add(IDC_BTN_MPHI_MATL_ST_ADD_MOD);
	aControls.Add(IDC_BTN_MPHI_MATL_ST_DELETE);

	CRect rRect, rBtnRect;
	GetDlgItem(IDC_CMD_SHEATH_S_REIN_FOLD)->GetWindowRect(rRect);
	GetDlgItem(IDC_BTN_MPHI_MATL_ST_ADD_MOD)->GetWindowRect(rBtnRect);
	int dYDist = (rRect.bottom - rBtnRect.top) + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, dYDist);
}

void CMPhiMaterialSteelDlg_JP::FoldResetVisible()
{
	if (m_FoldGrp.GetFoldState())
	{
		// 접혔을 때
		m_FoldGrp.SetFoldState(FALSE, FALSE);
		m_FoldGrp.SetFoldState(TRUE, TRUE);
	}
	else
	{
		m_FoldGrp.SetFoldState(FALSE);
	}
	AlignBtnMove();
}

#define COLUMN_SIZE 7
void CMPhiMaterialSteelDlg_JP::Initial_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR* list[COLUMN_SIZE] = { _T("ID"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), 
		_T("Es"), _T("Fu"), _T("Fy1"), _T("Fy2"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MATD_REIN) };
	int width[COLUMN_SIZE] = { 40,120,120,120,120,100, 80};
	for (int i = 0; i < COLUMN_SIZE; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i, &lvcolumn);
	}
}

void CMPhiMaterialSteelDlg_JP::Initial_Data()
{
	const CArray<T_MATL_K, T_MATL_K>& aKey = m_pMPhiDataAll->aMatdK;
	int iCount = aKey.GetSize();

	for (int i = 0; i < iCount; i++)
	{
		int nID = aKey.GetAt(i);

		T_MATD_D mData;
		mData.Initialize();
		if (!m_pMPhiDataAll->GetMatd(nID, mData)) continue;
		if (mData.Type != _T("S")) continue;
		
		CString strID = _T(""); strID.Format(_T("%7d"), nID);
		CString strName = mData.Name;

		int iChkKind = 2; // only civil nx jp
		ShowEditBox(iChkKind);

		CString strEs = _T("");  strEs.Format(_T("%g"), mData.Data1.Analysis.Elast);
		CString strFu = _T("");  strFu.Format(_T("%g"), mData.Data1.Design.S_Fu);
		CString strFy1 = _T(""); strFy1.Format(_T("%g"), mData.Data1.Design.S_Fy1);
		CString strFy2 = _T(""); strFy2.Format(_T("%g"), mData.Data1.Design.S_Fy2);
		BOOL bMatdRein = m_pMPhiDataAll->ExistMatdRein(nID);
		int ListLineNo = m_ListCtrl.GetItemCount();

		// Add by sshan. MNET:2729.('20070507)//////////////////////////
		CString strDesingCode = GetDesignCodeName();
		if (strDesingCode == _T("KSCE-ASD05") && mData.Data1.CodeName == _T(""))
		{
			strFu = _T("-");
			strFy1 = _T("-");
			strFy2 = _T("-");
		}
		////////////////////////////////////////////////////////////////////

		Write_ListCtrlBox(ListLineNo, strID, strName, strEs, strFu, strFy1, strFy2, bMatdRein);
		
	}
	EnableEditBox(TRUE);
	GetDlgItem(IDC_CMD_STEEL_MAT2)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void CMPhiMaterialSteelDlg_JP::ShowEditBox(int nCheck)
{
	// Set Hide all Control
	{
		GetDlgItem(IDC_CMD_STEEL_MATFU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_STEEL_MATFY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_STEEL_MATFY2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CMD_STATIC_MATFU)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_STATIC_MATFY1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_STATIC_MATFY2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CMD_STEEL_MATFY2UNIT)->ShowWindow(SW_HIDE);
	}

	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	if (Index != -1)
		m_Code.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	CString strDgnCode = GetDesignCodeName();
	int iChkKind = 2; // only civil nx jp
	if (iChkKind == 1)	// ASTM(S), None.
	{
		GetDlgItem(IDC_CMD_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_STEEL_MATFY1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_CMD_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_STATIC_MATFY1)->ShowWindow(SW_SHOW);

		//GetDlgItem(IDC_CMD_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY));
	}
	else if (iChkKind == 2)	// KS(S), JIS(S), DIN(S), EN05(S), EN(S), UNI(S), CNS(S). CNS06(S).
	{
		GetDlgItem(IDC_CMD_STEEL_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_STEEL_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_STEEL_MATFY2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_CMD_STATIC_MATFU)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_STATIC_MATFY1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_STATIC_MATFY2)->ShowWindow(SW_SHOW);

		//GetDlgItem(IDC_CMD_STATIC_MATFY1)->SetWindowText(_LS(IDS_DGN_TEXT_FY1));

		GetDlgItem(IDC_CMD_STEEL_MATFY2UNIT)->ShowWindow(SW_SHOW);
	}
}

CString CMPhiMaterialSteelDlg_JP::GetDesignCodeName()
{
	CString strDesingCode = _T("");
	if (m_pDoc->m_pAttrCtrl->ExistDstl())
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

void CMPhiMaterialSteelDlg_JP::Write_ListCtrlBox(int Index, CString strID, CString strName, CString strEs, CString strFu,
	CString strFy1, CString strFy2, BOOL bMatdRein)
{
	// change by Seungjun (`06.05.29).
	LV_ITEM lvitem;
	LPTSTR szText[COLUMN_SIZE];
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)strName;
	szText[2] = (LPTSTR)(LPCTSTR)strEs;
	szText[3] = (LPTSTR)(LPCTSTR)strFu;
	szText[4] = (LPTSTR)(LPCTSTR)strFy1;
	szText[5] = (LPTSTR)(LPCTSTR)strFy2;
	szText[6] = bMatdRein ? _T("O") : _T("X");

	// Input ListCtrl Box.
	for (int i = 0; i < COLUMN_SIZE; i++)
	{
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = Index;
		lvitem.iSubItem = i;
		lvitem.pszText = szText[i];
		if (i == 0)	m_ListCtrl.InsertItem(&lvitem);
		else		m_ListCtrl.SetItem(&lvitem);
	}
}

void CMPhiMaterialSteelDlg_JP::EnableEditBox(BOOL bCheck)
{
	GetDlgItem(IDC_CMD_STEEL_MATES)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_STEEL_MATPS)->EnableWindow(bCheck);

	GetDlgItem(IDC_CMD_STEEL_MATFU)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_STEEL_MATFY1)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_STEEL_MATFY2)->EnableWindow(bCheck);
}

void CMPhiMaterialSteelDlg_JP::Initial_Unit()
{
	m_PsUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_FuUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_EsUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy1Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_Fy2Unit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_SheathEcUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_SheathAckUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_SheathHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SheathWeightUnit.SetUnitType(D_UNITSYS_BASE_ELAST);

	// 고정 Unit
	T_UNIT_INDEX ChangeIndex, InitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(InitIndex);
	ChangeIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KN;
	ChangeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(ChangeIndex);
	m_SteelSurrendUnit.SetUnitType(D_UNITSYS_BASE_ELAST); // kN / m2 

	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(InitIndex);
}

void CMPhiMaterialSteelDlg_JP::Init_SheathCase()
{
	m_SheathSteelName2 = _T("");
	m_SteelSurrend = 0.0;
	m_SheathEc = 0.0;
	m_SheathAck = 0.0;
	m_SheathHeight = 0.0;
	m_SheathWeight = 0.0;

	m_SheathSteelCode.EnableWindow(m_bSheath);
	m_SheathSteelName.EnableWindow(m_bSheath);

	EnableSheathEditBox(m_bSheath);
	if (m_bSheath)
	{
		Init_SheathComboBox();
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(TRUE);
	}
}

void CMPhiMaterialSteelDlg_JP::Init_SheathComboBox()
{
	// Reset Contents.
	m_SheathSteelCode.ResetContent();

	CArray<CString, CString&> arSCodeList;
	CString strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, arSCodeList);

	m_SheathSteelCode.AddString(_T("None"));
	for (int i = 0; i < arSCodeList.GetSize(); i++)
	{
		if (arSCodeList[i] == MATLCODE_STL_JIS || arSCodeList[i] == MATLCODE_STL_JIS_CIVIL)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arSCodeList[i]);
#endif
			m_SheathSteelCode.AddString((arSCodeList[i]));
		}
	}
}

void CMPhiMaterialSteelDlg_JP::Set_SheathCase(const T_MATD_REIN_D& reinD)
{
	m_SheathSteelCode.EnableWindow(m_bSheath);
	m_SheathSteelName.EnableWindow(m_bSheath);

	Init_SheathComboBox();

	BOOL bUserSP = (reinD.CoverSP.strCodeName == _T("None") || reinD.CoverSP.strCodeName == _T(""));
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(!bUserSP);
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(bUserSP);

	//강판(Steel Plate) 피복
	SetCtrlAtReinSP(&reinD.CoverSP);
	//충진재료
	SetCtrlAtReinCF(&reinD.CoverFM);
}

void CMPhiMaterialSteelDlg_JP::SetCtrlAtReinSP(const T_MATD_REIN_SP* pSP)
{
	int iIndex = 0;
	iIndex = m_SheathSteelCode.FindStringExact(-1, pSP->strCodeName);
	if (iIndex >= 0) m_SheathSteelCode.SetCurSel(iIndex);
	else			 m_SheathSteelCode.SetCurSel(0);

	iIndex = m_SheathSteelCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1) {
		m_SheathSteelCode.GetLBText(iIndex, strCode);

		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetSteelNameList(strCode, arName);

		m_SheathSteelName.ResetContent();
		for (int i = 0; i < arName.GetSize(); i++) m_SheathSteelName.AddString(arName[i]);
	}

	iIndex = m_SheathSteelName.FindStringExact(-1, pSP->strCodeMatlName);
	if (iIndex >= 0) m_SheathSteelName.SetCurSel(iIndex);
	else			 m_SheathSteelName.SetCurSel(0);

	m_SheathSteelName2 = pSP->strCodeMatlName;
	m_SteelSurrend = pSP->dFy;
}

void CMPhiMaterialSteelDlg_JP::SetCtrlAtReinCF(const T_FILL_MATL_SP* pFM)
{
	m_SheathEc = pFM->dEc;
	m_SheathAck = pFM->dSigmaCk;
	m_SheathHeight = pFM->dThickness;
	m_SheathWeight = pFM->dWeight;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnStlMatDlg message handlers

void CMPhiMaterialSteelDlg_JP::OnClickDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	OnItemchangedDgnSteelmatList(pNMHDR, pResult);
	*pResult = 0;
}

void CMPhiMaterialSteelDlg_JP::OnItemchangedDgnSteelmatList(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetRedraw(FALSE);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem, 0);

	T_MATD_D mData;
	mData.Initialize();
	int key = _ttoi(strID);
	BOOL bCheck = m_pMPhiDataAll->GetMatd(key, mData);

	if (bCheck)
	{
		m_kMatl = key;
		m_Es = mData.Data1.Analysis.Elast;
		m_Ps = mData.Data1.Analysis.Poisson;
		m_Fu = mData.Data1.Design.S_Fu;
		m_Fy1 = mData.Data1.Design.S_Fy1;
		m_Fy2 = mData.Data1.Design.S_Fy2;

		// Change by ZINU.('02.9.17).
		CString strMatCode = mData.Data1.CodeName;

		if (strMatCode == _T("") || strMatCode == _T("None"))
			EnableEditBox(TRUE);
		else
			EnableEditBox(FALSE);

		// Material Code�� Name ComboBox�� �ʱ�ȭ�մϴ�.
		Initial_MaterialCombo(mData.Data1.CodeName, mData.Data1.CodeMatlName);

		CString MatName;
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(mData.Data1.CodeName, MatName);
#endif

		int Index = m_MatName.FindStringExact(-1, MatName);
		if (Index == -1)
		{
			m_MatName2 = m_ListCtrl.GetItemText(nItem, 1);	// strName2
			GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));
			GetDlgItem(IDC_CMD_STEEL_MAT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMD_STEEL_MAT2)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_MatName.SetCurSel(Index);
			GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_S_DLG_IDC_STATIC_MATNAME));
			GetDlgItem(IDC_CMD_STEEL_MAT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CMD_STEEL_MAT2)->ShowWindow(SW_HIDE);
		}

		T_MATD_REIN_D reinD;
		if (m_pMPhiDataAll->GetMatdRein(key, reinD) == FALSE) {
			m_bSheath = FALSE;
			Init_SheathCase();
		}
		else {
			if (reinD.kMatl == 0) {
				m_bSheath = FALSE;
				Init_SheathCase();
			}
			else {
				m_bSheath = TRUE;
				Set_SheathCase(reinD);
			}
		}

		FoldResetVisible();
		UpdateData(FALSE);
	}
	*pResult = 0;

	int iChkKind = 2; // only civil nx jp
	ShowEditBox(iChkKind);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

void CMPhiMaterialSteelDlg_JP::Initial_MaterialCombo(CString strCode, CString strName)
{
	MInitCombo initCombo;

	m_Code.ResetContent();
	m_MatName.ResetContent();

	CArray<CString, CString&> arDesignCode;
	CString strType = _T("S");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strType, arDesignCode);
	m_Code.AddString(_T("None"));
	for (int i = 0; i < arDesignCode.GetSize(); i++)
	{
		if (arDesignCode[i] == MATLCODE_STL_JIS || arDesignCode[i] == MATLCODE_STL_JIS_CIVIL)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arDesignCode[i]);
#endif
			m_Code.AddString(arDesignCode[i]);
		}
	}

	if (strCode == _T(""))	strCode = _T("None");

	CString CodeName;
	CodeName = strCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif

	int Index = m_Code.FindStringExact(-1, CodeName);
	m_Code.SetCurSel(Index);

	CArray <CString, CString&> NameList;
	m_pDoc->m_pMatlDB->GetSteelNameList(strCode, NameList);
	for (int i = 0; i < NameList.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, NameList[i]);
#endif
		m_MatName.AddString(NameList[i]);
	}

	UpdateData(FALSE);
}

void CMPhiMaterialSteelDlg_JP::OnSelchangeDgnSteelmatCode()
{
	MInitCombo initCombo;
	SetRedraw(FALSE);

	CString strCode = _T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	m_MatName.ResetContent();
	// Change Material Name by Code.
	if (strCode == _T("None") || strCode == _T(""))
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));
		GetDlgItem(IDC_CMD_STEEL_MAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_STEEL_MAT2)->ShowWindow(SW_SHOW);
		EnableEditBox(TRUE);
		ShowEditBox(1);
	}
	else
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_S_DLG_IDC_STATIC_MATNAME));
		GetDlgItem(IDC_CMD_STEEL_MAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_STEEL_MAT2)->ShowWindow(SW_HIDE);
		EnableEditBox(FALSE);

		// Change by ZINU.('02.9.17).
		int iChkKind = 2; // only civil nx jp
		ShowEditBox(iChkKind);

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

	if (strCode != _T("None"))
	{
		m_Es = 0.0;
		m_Ps = 0.0;
		m_Fu = 0.0;
		m_Fy1 = 0.0;
		m_Fy2 = 0.0;
	}
	m_MatName2 = _T("");

	UpdateData(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

void CMPhiMaterialSteelDlg_JP::OnSelchangeDgnSteelMat()
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

	if (bCheck)
	{
		m_Es = sData.Elast;
		m_Ps = sData.Poisson;
		m_Fu = sData.S_Fu;
		m_Fy1 = sData.S_Fy1;
		m_Fy2 = sData.S_Fy2;
		UpdateData(FALSE);
	}
}

void CMPhiMaterialSteelDlg_JP::OnClickbSheath()
{
	m_bSheath = m_bSheath == FALSE ? TRUE : FALSE;

	Init_SheathCase();
}

LRESULT CMPhiMaterialSteelDlg_JP::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	AlignBtnMove();
	if (m_FoldGrp.GetFoldState())
	{
		m_FoldGrp.SetWindowTextW(_LS(IDS_CMD_MPHI_MATL_SHEATH_UNFOLD));
	}
	else
	{
		m_FoldGrp.SetWindowTextW(_LS(IDS_CMD_MPHI_MATL_SHEATH_FOLD));
		Invalidate();
		UpdateWindow();
		m_pParent->Invalidate();
		m_pParent->UpdateWindow();
	}
	return 0L;
}

void CMPhiMaterialSteelDlg_JP::EnableSheathEditBox(BOOL bCheck)
{
	GetDlgItem(IDC_CMD_SHEATH_STELL_SURRENDER_EDT)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->EnableWindow(bCheck);
	GetDlgItem(IDC_SHEATH_STEEL_EC_EDT)->EnableWindow(m_bSheath);
	GetDlgItem(IDC_SHEATH_STEEL_ACK_EDT)->EnableWindow(m_bSheath);
	GetDlgItem(IDC_SHEATH_STEEL_HEIGHT_EDT)->EnableWindow(m_bSheath);
	GetDlgItem(IDC_SHEATH_STEEL_WEIGHT_EDT)->EnableWindow(m_bSheath);
}

void CMPhiMaterialSteelDlg_JP::OnChangeSheathSteelCode()
{
	int iIndex = m_SheathSteelCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathSteelCode.GetLBText(iIndex, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	m_SheathSteelName.ResetContent();
	if (strCode == _T("None") || strCode == _T(""))
	{
		EnableSheathEditBox(TRUE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_DEFAULT_NAME));
	}
	else
	{
		EnableSheathEditBox(FALSE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_COB)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_EDT)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_SHEATH_STELL_NAME_STC)->SetWindowText(_LS(IDS_IDD_CMD_MPHI_ELEMENT_S_DLG_IDC_CMD_SHEATH_STELL_NAME_STC));

		// Change Material Name by Code.
		CArray<CString, CString&> arName;
		m_pDoc->m_pMatlDB->GetSteelNameList(strCode, arName);
		for (int i = 0; i < arName.GetSize(); i++) m_SheathSteelName.AddString(arName[i]);
	}
	m_SteelSurrend = 0.0;
	m_SheathSteelName2 = _T("");

	UpdateData(FALSE);
}

void CMPhiMaterialSteelDlg_JP::OnChangeSheathSteelName()
{
	int iIndex = m_SheathSteelCode.GetCurSel();
	CString strCode = _T("");
	if (iIndex != -1)	m_SheathSteelCode.GetLBText(iIndex, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	int nameIdx = m_SheathSteelName.GetCurSel();
	CString strName = _T("");
	if (iIndex != -1)	m_SheathSteelName.GetLBText(nameIdx, strName);

	T_MATL_STEEL sData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetSteelData(strCode, strName, sData);
	if (bCheck)	m_SteelSurrend = sData.S_Fy1;
	else
	{
		int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		if (nItem != -1)
		{
			CString strID = m_ListCtrl.GetItemText(nItem, 0);
			int nID = _ttoi(strID);

			T_MATD_D mData;
			mData.Initialize();
			BOOL bCheck = m_pMPhiDataAll->GetMatd(nID, mData);
			if (bCheck)
			{
				double fy1 = mData.Data1.Design.S_Fy1;
				if (fy1 <17)  m_SteelSurrend = fy1;
				else m_SteelSurrend = 0.0;
			}
			else	m_SteelSurrend = 0.0;
		}
	}
	UpdateData(FALSE);
}

BOOL CMPhiMaterialSteelDlg_JP::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	if (m_Code.GetCurSel() == 0)
	{
		if (m_MatName2 == _T("")) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR); }
	}
	if (m_Es <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_ES_ERROR); }
	if (m_Ps <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_PS_ERROR); }
	// Change, Jaeoh.(08.03.14)
	if (GetDesignCodeName() != _T("KSCE-ASD05"))
		if (m_Fu <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FU_ERROR); }
	// Code Error Checking.
	int Index = m_Code.GetCurSel();
	CString strCode = _T("");
	m_Code.GetLBText(Index, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	// Change by ZINU.('02.9.17).

	// only civil nx jp
	int iChkKind = 2;
	if (iChkKind == 1)
	{
		if (GetDesignCodeName() != _T("KSCE-ASD05"))
			if (m_Fy1 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR); }
		m_Fy2 = m_Fy1;
	}
	else if (iChkKind == 2)
	{
		if (m_Fy1 <= 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY1_ERROR); }
		if (m_Fy2 < 0.0) { bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY2_ERROR); }
	}

	if (!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CMPhiMaterialSteelDlg_JP::OnAddModBtn()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL), MB_OK);
		return;
	}

	UpdateData(TRUE);
	if (ErrorCheck() == FALSE) return;

	CString strID = m_ListCtrl.GetItemText(nItem, 0);

	T_MATL_K key = _ttoi(strID);
	T_MATD_D matdD; matdD.Initialize();
	T_MATD_REIN_D reinD; reinD.Initialize();
	m_pMPhiDataAll->GetMatd(key, matdD);
	m_pMPhiDataAll->GetMatdRein(key, reinD);
	if (!DlgToData(key, matdD, reinD)) return;

	// 일단 CMPhiParameterDlg::m_pMPhiDataAll에 쌓아 놓음
	m_pMPhiDataAll->SetMatd(key, matdD);
	if (reinD.kMatl != 0)
		m_pMPhiDataAll->SetMatdRein(key, reinD);

	ModifyItem(nItem, key, matdD);

	SetFocus();
	RedrawWindow();
}

void CMPhiMaterialSteelDlg_JP::OnDeleteBtn()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}

	CString strID = m_ListCtrl.GetItemText(nItem, 0);

	T_MATL_K key = _ttoi(strID);
	T_MATD_D data; data.Initialize();
	if (m_pMPhiDataAll->GetMatd(key, data) == FALSE) {
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	}

	m_pMPhiDataAll->DeleteMatdRein(key);

	ModifyItem(nItem, key, data);
	if (m_bSheath == TRUE)
		OnClickbSheath();

	SetFocus();
	RedrawWindow();
}

BOOL CMPhiMaterialSteelDlg_JP::DlgToData(const T_MATL_K& key, OUT T_MATD_D& rMatd, OUT T_MATD_REIN_D& rRein)
{
	CString strName = _T(""), strCode = _T(""), strMatName = _T("");

	// Modify Material ID
	int nCode = m_Code.GetCurSel();
	if (nCode == 0)	// Material Code = None
	{
		strMatName = m_MatName2;
		strName = m_MatName2;
	}
	else	// Material Code = Code
	{
		m_Code.GetLBText(nCode, strCode);
		int nName = m_MatName.GetCurSel();
		m_MatName.GetLBText(nName, strMatName);
		strName = strMatName;
	}

	rMatd.Name = strName;
	rMatd.Data1.CodeName = strCode;
	rMatd.Data1.CodeMatlName = strMatName;
	rMatd.Data1.Analysis.Elast = m_Es;
	rMatd.Data1.Analysis.Poisson = m_Ps;
	rMatd.Data1.Design.S_Fu = m_Fu;
	rMatd.Data1.Design.S_Fy = m_Fy1;
	rMatd.Data1.Design.S_Fy1 = m_Fy1;
	rMatd.Data1.Design.S_Fy2 = m_Fy2;

	DlgToDataRein(key, rRein);

	return TRUE;
}

void CMPhiMaterialSteelDlg_JP::DlgToDataRein(const T_MATL_K& key, OUT T_MATD_REIN_D& rRein)
{
	if (m_bSheath == FALSE) { //보강재질 삭제
		m_pMPhiDataAll->DeleteMatdRein(key);
		return;
	}

	int nIndex = 0;
	CString strVal;

	rRein.Initialize();
	rRein.kMatl = key;

	nIndex = m_SheathSteelCode.GetCurSel();
	if (nIndex != -1) m_SheathSteelCode.GetLBText(nIndex, rRein.CoverSP.strCodeName);

	if (rRein.CoverSP.strCodeName == _T("None") || rRein.CoverSP.strCodeName == _T("")) {	// None.
		rRein.CoverSP.strCodeMatlName = m_SheathSteelName2;
	}
	else {
		nIndex = m_SheathSteelName.GetCurSel();
		if (nIndex != -1) m_SheathSteelName.GetLBText(nIndex, rRein.CoverSP.strCodeMatlName);
	}

	rRein.CoverSP.dFy = m_SteelSurrend;
	
	rRein.CoverFM.dEc = m_SheathEc;
	rRein.CoverFM.dSigmaCk = m_SheathAck;
	rRein.CoverFM.dThickness = m_SheathHeight;
	rRein.CoverFM.dWeight = m_SheathWeight;
}

void CMPhiMaterialSteelDlg_JP::ModifyItem(const int& nLineNo, const T_MATL_K& Key, const T_MATD_D& mData)
{
	CString strEs = _T("");  strEs.Format(_T("%g"), m_Es);
	CString strFu = _T("");  strFu.Format(_T("%g"), m_Fu);
	CString strFy1 = _T(""); strFy1.Format(_T("%g"), m_Fy1);
	CString strFy2 = _T(""); strFy2.Format(_T("%g"), m_Fy2);
	BOOL bMatdRein = m_pMPhiDataAll->ExistMatdRein(Key);
	CString strMatdRein = bMatdRein ? _T("O") : _T("X");

	if (GetDesignCodeName() == _T("KSCE-ASD05") && mData.Data1.CodeName == _T(""))
	{
		strFu = _T("-");
		strFy1 = _T("-");
		strFy2 = _T("-");
	}

	m_ListCtrl.SetItemText(nLineNo, 1, mData.Name);	// Name.
	m_ListCtrl.SetItemText(nLineNo, 2, strEs);		// Es.
	m_ListCtrl.SetItemText(nLineNo, 3, strFu);		// Fu.
	m_ListCtrl.SetItemText(nLineNo, 4, strFy1);		// Fy1.
	m_ListCtrl.SetItemText(nLineNo, 5, strFy2);		// Fy2.
	m_ListCtrl.SetItemText(nLineNo, 6, strMatdRein);// 보강재질 유무
	m_ListCtrl.SetItemState(nLineNo, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}