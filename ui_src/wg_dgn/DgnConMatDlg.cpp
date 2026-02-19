// DgnConMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConMatDlg.h"

#include "DgnConMatSLTEDlg.h"
#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_EditData.h"

#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog


CDgnConMatDlg::CDgnConMatDlg(BOOL bPscDgn/*=FALSE*/,CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConMatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMatDlg)
	m_bLambda = FALSE;
	m_bChkTransfer = FALSE;

	m_Fc = 0.0;
	m_MatName2 = _T("");
	m_SubBarName2 = _T("");
	m_MainBarName2 = _T("");
	m_Fy = 0.0;
	m_Fys = 0.0;
	//}}AFX_DATA_INIT

	m_strCurrentItem = _T("");

	m_pDoc = CDBDoc::GetDocPoint();

	m_bTransfer = FALSE;
	// 현재 m_bTransfer 는 PSC 설계 AS 기준만 적용됨. 
	if(bPscDgn) 
	{
		T_PSCD_D PscdD; PscdD.Initialize();
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		if(PscdD.iDgnCode == AS_5100_5_17_PSC) m_bTransfer = TRUE;
	}
}

void CDgnConMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatDlg)
	DDX_Control(pDX, IDC_DGN_CONC_REBARCODE,  m_RebarCode);
	DDX_Control(pDX, IDC_DGN_CONC_SUBNAME,    m_SubBarName);
	DDX_Control(pDX, IDC_DGN_CONC_MAINNAME,   m_MainBarName);
	DDX_Control(pDX, IDC_DGN_CONCMAT_CODE,    m_Code);
	DDX_Control(pDX, IDC_DGN_CONC_MAT,        m_MatName);
	DDX_Control(pDX, IDC_DGN_CONC_MATFYUNIT,  m_FyUnit);
	DDX_Control(pDX, IDC_DGN_CONC_MATFYSUNIT, m_FysUnit);
	DDX_Control(pDX, IDC_DGN_CONC_MATFCUNIT,  m_FcUnit);
	DDX_Control(pDX, IDC_DGN_TRANSFER_UNIT,   m_TransferUnit);
	DDX_Check  (pDX, IDC_DGN_TRANSFER_CHK,    m_bChkTransfer);
	DDX_Check  (pDX, IDC_DGN_LAMBDA_CHK,      m_bLambda);
	DDX_Control(pDX, IDC_DGN_LAMBDA_EDT,      m_edtLambda);
	DDX_Control(pDX, IDC_DGN_TRANSFER_EDT,    m_edtTransfer);
	DDX_Control(pDX, IDC_DGN_CONCMAT_LIST,    m_ListCtrl);
	DDX_Text(pDX, IDC_DGN_CONC_MATFC, m_Fc);
	DDX_Text(pDX, IDC_DGN_CONC_MAT2, m_MatName2);
	DDX_Text(pDX, IDC_DGN_CONC_SUBNAME2, m_SubBarName2);
	DDX_Text(pDX, IDC_DGN_CONC_MAINNAME2, m_MainBarName2);
	DDX_Text(pDX, IDC_DGN_CONC_MATFY, m_Fy);
	DDX_Text(pDX, IDC_DGN_CONC_MATFYS, m_Fys);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConMatDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConMatDlg)
	ON_NOTIFY(NM_CLICK, IDC_DGN_CONCMAT_LIST, OnClickDgnConcmatList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_CONCMAT_LIST, OnItemchangedDgnConcmatList)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_MAT, OnSelchangeDgnConcMat)
	ON_CBN_SELCHANGE(IDC_DGN_CONCMAT_CODE, OnSelchangeDgnConcmatCode)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_MAINNAME, OnSelchangeDgnConcMainname)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_SUBNAME, OnSelchangeDgnConcSubname)
	ON_BN_CLICKED(IDC_DGN_LAMBDA_CHK, OnClickLambdaChk)
	ON_BN_CLICKED(IDC_DGN_TRANSFER_CHK, OnClickTransferChk)
	ON_BN_CLICKED(IDC_DGN_CONCMAT_MODIFY, OnDgnConcmatModify)
	ON_BN_CLICKED(IDC_DGN_CONCMAT_CLOSE, OnDgnConcmatClose)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_REBARCODE, OnSelchangeDgnConcRebarcode)
	ON_BN_CLICKED(IDC_DGN_CONC_EL, OnDgnConcElasticity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg message handlers

BOOL CDgnConMatDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	AlignControl();
	// Initialize Grid, Unit, Matl Code.
	Init_GridData();
	Init_Unit();
	Init_MatlCode();
	Init_Items();
	
	UpdateData(FALSE);

	OnClickLambdaChk();
	
	OnClickTransferChk();
	
	return TRUE;
}

void CDgnConMatDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;
	CArray<UINT, UINT> aControls;

	// control move
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_LAMBDA_EDT);
	aControls.Add(IDC_DGN_LAMBDA_CHK);

	GetDlgItem(IDC_DGN_CONC_MATFC)->GetWindowRect(rRef);
	if(m_bTransfer)
	{
		GetDlgItem(IDC_DGN_TRANSFER_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	}
	else
	{
		GetDlgItem(IDC_DGN_LAMBDA_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	}
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
 
	// Concrete Selection Group resize
	CRect rectResize;
	GetDlgItem(IDC_DGN_CONC_GRP)->GetWindowRect(rectResize);
	GetDlgItem(IDC_DGN_LAMBDA_EDT)->GetWindowRect(rRef);
	rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);

	ScreenToClient(rectResize);
	GetDlgItem(IDC_DGN_CONC_GRP)->MoveWindow(rectResize);

	// Rebar Selection Group(GR) move
	aControls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this,aControls,IDC_DGN_REBAR_GRP, TRUE);

	GetDlgItem(IDC_DGN_CONC_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_REBAR_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

    // control move
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_CONCMAT_MODIFY);
	aControls.Add(IDC_DGN_CONCMAT_CLOSE);

	GetDlgItem(IDC_DGN_REBAR_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CONCMAT_MODIFY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// ResizeDialog
	UINT nLastCtrlID = IDC_DGN_CONCMAT_MODIFY;
	CWnd* pWndLast = GetDlgItem(nLastCtrlID);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

// Coded by Seungjun MNet:No.2393 ('20061110)
void CDgnConMatDlg::Init_Items()
{
	GetDlgItem(IDC_DGN_TRANSFER_CHK)->ShowWindow(m_bTransfer);	
	GetDlgItem(IDC_DGN_TRANSFER_EDT)->ShowWindow(m_bTransfer);	
	GetDlgItem(IDC_DGN_TRANSFER_UNIT)->ShowWindow(m_bTransfer);	

	if(!(CDBLib::GetConCodeName()==_T("Eurocode2") || CDBLib::GetConCodeName()==_T("Eurocode2:04") ||
			 CDBLib::GetConCodeName()==_T("Eurocode2-2:05"))) // Coded by Seungjun ('20070613) MNet:No.2313.
	{
		GetDlgItem(IDC_DGN_CONC_EL)->ShowWindow(SW_HIDE);
		return;
	}
	
	GetDlgItem(IDC_DGN_CONC_EL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_CONC_EL)->EnableWindow(FALSE);
}

BOOL CDgnConMatDlg::IsEnableLambdaCode_Civil()
{
	const CString& strCode = CDBLib::GetConCodeName();
	if (strCode == CONCODE_AREMA_2023) { return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD20) { return TRUE; }
	if (strCode == CONCODE_AASHTO_LRFD24) { return TRUE; }
	return FALSE;		 
}

void CDgnConMatDlg::Init_GridData()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;

	TCHAR* list[7] = {_T("ID"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME), _T("fc|fck|R"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_CHECK), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_CHAR_LAMBDA), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MAIN_BAR), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_SUB_BAR) };
#ifdef _MGEN_CH
	list[2] = { _T("fcu,k") };
#endif
	int width[7]	= {40,85,70,50,55,65,75};
	for(int i=0; i<7; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}
	// Set Design Material Data using Analysis Material Data.
	CArray<T_MATL_K, T_MATL_K> arKey;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(arKey);
	// Arrange Data for initializing Material Data.
	T_MATD_D mData;
	int iListNo = 0;
	for(int i=0; i<arKey.GetSize(); i++)
	{
		int iID = arKey.GetAt(i);
		mData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(iID, mData))	continue;
		if(mData.Type==_T("C"))
		{
			CString strID=_T(""), strFc=_T("");
			strID.Format(_T("%7d"), iID);
			strFc.Format(_T("%g"),  mData.Data1.Design.C_fc);
			CString strNa		= mData.Name;
			CString strMbar	= mData.MainRebar_RebarName;
			CString strSbar	= mData.SubRebar_RebarName;

			CString strChk;
			if(mData.Data1.Design.bLambda) strChk = _T("O");
			else                           strChk = _T("X");

			CString strLambda;
			strLambda.Format(_T("%g"), mData.Data1.Design.dLambda);

			Write_ListBox(iListNo,strID,strNa,strFc,strChk,strLambda,strMbar,strSbar);
			iListNo++;
		}
	}
	
	GetDlgItem(IDC_DGN_CONC_MAT2)->ShowWindow(SW_HIDE);
}

void CDgnConMatDlg::Init_Unit()
{
	m_FcUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FyUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FysUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtLambda.SetUnitType(D_UNITSYS_NONE);
	m_edtTransfer.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_TransferUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
}

void CDgnConMatDlg::Init_MatlCode()
{
	// Reset Contents.
	m_Code.ResetContent();
	m_RebarCode.ResetContent();

	CArray<CString, CString&> arDesignCode;
	CString strMatlType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arDesignCode);
	// Initialize MatlCode, RebarCode.
	m_Code.AddString(_LS(IDS_DGN_NONE));
	m_RebarCode.AddString(_LS(IDS_DGN_NONE));
	for(int i=0; i<arDesignCode.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arDesignCode[i]);
#endif
		m_Code.AddString(arDesignCode[i]);
	}
	for(int i=0; i<arDesignCode.GetSize(); i++)	m_RebarCode.AddString(arDesignCode[i]);

	EnableEditBox(FALSE);
	EnableRebarEditBox(FALSE);
}

void CDgnConMatDlg::ChangeDlgItemInfo()
{
	CString ConcCode = _T("");
	CString RBarCode = _T("");
	int ConcIdx = m_Code.GetCurSel();
	if(ConcIdx >= 0)
		m_Code.GetLBText(ConcIdx, ConcCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(ConcCode);
#endif

	BOOL bChinaEN04 = FALSE;
#ifdef _MGEN_CH
	bChinaEN04 = TRUE;
#endif


	if(ConcCode == MATLCODE_CON_GB_CIVIL )
		GetDlgItem(IDC_DGN_STATIC_FC)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_JTJ));
	else if(!bChinaEN04 && ConcCode == MATLCODE_CON_EN04)
		GetDlgItem(IDC_DGN_STATIC_FC)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_EN04));
	else
		GetDlgItem(IDC_DGN_STATIC_FC)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_ETC));

	int RBarIdx = m_RebarCode.GetCurSel();
	if(RBarIdx >= 0)
		m_RebarCode.GetLBText(RBarIdx, RBarCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(RBarCode);
#endif
	if(RBarCode == MATLCODE_CON_GB_CIVIL)
	{
		GetDlgItem(IDC_DGN_STATIC_FY)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_JTJ));
		GetDlgItem(IDC_DGN_STATIC_FYS)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_JTJ));
	}
	else
	{
		GetDlgItem(IDC_DGN_STATIC_FY)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_ETC));
		GetDlgItem(IDC_DGN_STATIC_FYS)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_ETC));
	}

	UpdateData(FALSE);
}

void CDgnConMatDlg::Write_ListBox(int iIndex, CString strID, CString strNa, CString strFc, CString strChk, CString strLambda, CString strMbar, CString strSbar)
{
	LV_ITEM lvitem;
	LPTSTR szText[7];
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)strNa;
	szText[2] = (LPTSTR)(LPCTSTR)strFc;
	szText[3] = (LPTSTR)(LPCTSTR)strChk;
	szText[4] = (LPTSTR)(LPCTSTR)strLambda;
	szText[5] = (LPTSTR)(LPCTSTR)strMbar;
	szText[6] = (LPTSTR)(LPCTSTR)strSbar;
	// Input ListBox.
	for(int i=0; i<7; i++)
	{
		lvitem.mask		  = LVIF_TEXT;
		lvitem.iItem  	= iIndex;
		lvitem.iSubItem = i;
		lvitem.pszText	= szText[i];
		if(i==0)	m_ListCtrl.InsertItem(&lvitem);
		else			m_ListCtrl.SetItem(&lvitem);
	}
}

void CDgnConMatDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);

// MQC_ab_5526-2 Civil의 경우 light weight concrete 고려 하지 않음.
#ifdef _CIVIL	
	BOOL bEnableLambda = IsEnableLambdaCode_Civil();
	GetDlgItem(IDC_DGN_LAMBDA_CHK)->EnableWindow(bEnableLambda);	
	//else {
	//	m_bLambda = FALSE;
	//}
#endif // _CIVIL	
}

void CDgnConMatDlg::OnDgnConcmatClose() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConMatDlg::OnClickDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnItemchangedDgnConcmatList(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnConMatDlg::OnItemchangedDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem,0);
	
	T_MATD_D mData;
	mData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(_ttoi(strID),mData))	return;

	// Coded by Seungjun MNet:No.2393 ('20061110)
	m_strCurrentItem = strID;
	GetDlgItem(IDC_DGN_CONC_EL)->EnableWindow(TRUE);

	// If Code is None, Material Name is mData.Name.
	m_MatName2 = mData.Name;
	m_Fc = mData.Data1.Design.C_fc;
	m_bLambda = mData.Data1.Design.bLambda;
	// MQC_ab_5526-2 Civil의 경우 light weight concrete 고려 하지 않음.
#ifdef _CIVIL
	m_bLambda = IsEnableLambdaCode_Civil();	
#endif // _CIVIL

	m_bChkTransfer = mData.Data1.Design.bTransfer;
	m_edtTransfer.SetEditUnit(mData.Data1.Design.dTransferFci);
	if(!m_bTransfer) m_bChkTransfer = FALSE;

	m_edtLambda.SetEditUnit(mData.Data1.Design.dLambda);
	m_MainBarName2 = mData.MainRebar_RebarName;
	m_Fy = mData.MainRebarData.B_fy;
	m_SubBarName2 = mData.SubRebar_RebarName;
	m_Fys = mData.SubRebarData.B_fy;

	CString strMatlCode=_T("");
	if(mData.Data1.CodeName==_T("None") || mData.Data1.CodeName==_T(""))	// None.
	{
		strMatlCode = _T("None");
		EnableEditBox(TRUE);
	}
	else	// DB.
	{
		strMatlCode = mData.Data1.CodeName;
		EnableEditBox(FALSE);
	}

	CString strRebarCode=_T("");
	if(mData.Rebar_CodeName==_T("None") || mData.Rebar_CodeName==_T(""))	// None.
	{
		strRebarCode = _T("None");
		EnableRebarEditBox(TRUE);
	}
	else	// DB.
	{
		strRebarCode = mData.Rebar_CodeName;
		EnableRebarEditBox(FALSE);
	}

	CString CodeName;
	CodeName = strRebarCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif

	int iIndex = m_RebarCode.FindStringExact(-1, CodeName);
	m_RebarCode.SetCurSel(iIndex);

	CodeName = strMatlCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	iIndex = m_Code.FindStringExact(-1, CodeName);
	m_Code.SetCurSel(iIndex);
	// Initialize Material Name ComboBox by Material Code.
	Init_MatlNameCombo(strMatlCode, mData.Data1.CodeMatlName);
	// Initialize RebarCode and Name ComboBox.
	Init_RebarCombo(strRebarCode, mData);

	ChangeDlgItemInfo();

	UpdateData(FALSE);

	OnClickLambdaChk();

	*pResult = 0;
}

void CDgnConMatDlg::Init_RebarCombo(CString strRebarCode, T_MATD_D rData)
{
	m_MainBarName.ResetContent();
	m_SubBarName.ResetContent();

	T_MATL_REBAR RebarData;
	if(strRebarCode!=_T("None") && strRebarCode!=_T(""))
	{
		// Initialize Rebar Name ComboBox by Code.
		CArray<CString,CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, arRebar);
		for(int i=0; i<arRebar.GetSize(); i++)  
		{
			m_MainBarName.AddString(arRebar[i]);
			m_SubBarName.AddString(arRebar[i]);
		}
		int Index = m_MainBarName.FindStringExact(-1, rData.MainRebar_RebarName);
		if(Index!=-1)
		{
			m_MainBarName.SetCurSel(Index);
			if(rData.MainRebarData.B_fy==0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.MainRebar_RebarName, RebarData);
				m_Fy = RebarData.B_fy;
			}
			else	m_Fy = rData.MainRebarData.B_fy;
		}
		else	m_Fy = 0.0;

		Index = m_SubBarName.FindStringExact(-1, rData.SubRebar_RebarName);
		if(Index!=-1)
		{
			m_SubBarName.SetCurSel(Index);
			if(rData.SubRebarData.B_fy==0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.SubRebar_RebarName, RebarData);
				m_Fys = RebarData.B_fy;
			}
	    else	m_Fys = rData.SubRebarData.B_fy;
		}
		else	m_Fys = 0.0;
	}
	UpdateData(FALSE);
}

void CDgnConMatDlg::EnableEditBox(BOOL bCheck)
{
	if(bCheck)	// None.
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
		GetDlgItem(IDC_DGN_CONC_MAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MAT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MATFC)->EnableWindow(TRUE);
	}
	else	// DB.
	{
		GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
		GetDlgItem(IDC_DGN_CONC_MAT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MAT2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MATFC)->EnableWindow(FALSE);
	}
}

void CDgnConMatDlg::EnableRebarEditBox(BOOL bCheck)
{
	if(bCheck)	// None.
	{
		GetDlgItem(IDC_DGN_STATIC_MATEFY)->SetWindowText(_LS(IDS_DGN_TEXT_FY_NAME));
		GetDlgItem(IDC_DGN_STATIC_MATAFV)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_NAME));
		GetDlgItem(IDC_DGN_CONC_MAINNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MAINNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MATFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_SUBNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SUBNAME2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MATFYS)->EnableWindow(TRUE);
	}
	else	// DB.
	{
		GetDlgItem(IDC_DGN_STATIC_MATEFY)->SetWindowText(_LS(IDS_DGN_TEXT_FY_DB_NAME));
		GetDlgItem(IDC_DGN_STATIC_MATAFV)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_DB_NAME));
		GetDlgItem(IDC_DGN_CONC_MAINNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MAINNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MATFY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_SUBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_SUBNAME2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MATFYS)->EnableWindow(FALSE);
	}
}

void CDgnConMatDlg::Init_MatlNameCombo(CString strCode, CString strNa)
{
	if(strCode==_T("None") || strCode==_T(""))	return;	// Skip if None.

	m_MatName.ResetContent();
	CArray<CString,CString&> arName;
	m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arName);
	for (int i = 0; i < arName.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arName[i]);
#endif
		m_MatName.AddString(arName[i]);
	}

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, strNa);
#endif
	int Index = m_MatName.FindStringExact(-1, strNa);
	if(Index!=-1)	m_MatName.SetCurSel(Index);
}

void CDgnConMatDlg::OnSelchangeDgnConcMat() 
{
	int Index = m_Code.GetCurSel();
	CString strCode=_T("");
	if(Index != -1) m_Code.GetLBText(Index,strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	Index = m_MatName.GetCurSel();
	CString strName=_T("");
	if(Index != -1) m_MatName.GetLBText(Index, strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode, strName, cData);
	if(bCheck)
	{
		m_Fc = cData.C_fc;
		UpdateData(FALSE);
	}
}

void CDgnConMatDlg::OnSelchangeDgnConcmatCode() 
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int iIndex = m_Code.GetCurSel();
	CString strCode=_T("");
	if(iIndex != -1)	m_Code.GetLBText(iIndex, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if(strCode==_T("None") || strCode==_T(""))	EnableEditBox(TRUE);
	else
	{
		EnableEditBox(FALSE);
		// Change Material Name by Code.
		m_MatName.ResetContent();
		CArray<CString, CString&> arMatlName;
		m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arMatlName);
		for (int i = 0; i < arMatlName.GetSize(); i++)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arMatlName[i]);
#endif
			m_MatName.AddString(arMatlName[i]);
		}
	}
	if(strCode!=_T("None"))	m_Fc = 0.0;
	m_MatName2=_T("");
	
	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CDgnConMatDlg::OnSelchangeDgnConcRebarcode() 
{
	CString strCode=_T("");
	int Index = m_RebarCode.GetCurSel();
	if(Index != -1)	m_RebarCode.GetLBText(Index, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if(strCode==_T("None") || strCode==_T(""))	EnableRebarEditBox(TRUE);
	else
	{
		EnableRebarEditBox(FALSE);
		m_MainBarName.ResetContent();
		m_SubBarName.ResetContent();

		CArray<CString, CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arRebar);
		for(int i=0; i<arRebar.GetSize(); i++)  
		{
			m_MainBarName.AddString(arRebar[i]);
			m_SubBarName.AddString(arRebar[i]);
		}	
	}

	if(strCode!=_T("None"))
	{
		m_Fy = 0.0;
		m_Fys = 0.0;
	}
	m_MainBarName2=_T("");
	m_SubBarName2=_T("");

	ChangeDlgItemInfo();

	UpdateData(FALSE);
}

void CDgnConMatDlg::OnSelchangeDgnConcMainname() 
{
	CString strRebarName=_T(""), strRebarCode=_T("");
	int Index = m_MainBarName.GetCurSel();
	if(Index != -1)
	{
		UpdateData(TRUE);
		m_MainBarName.GetLBText(Index,strRebarName);

		Index = m_RebarCode.GetCurSel();
		if(Index != -1) m_RebarCode.GetLBText(Index,strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE),MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if(m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,rData))
		{
			m_Fy = rData.B_fy;
		}
		UpdateData(FALSE);
	}
}

void CDgnConMatDlg::OnSelchangeDgnConcSubname() 
{
	CString strRebarName=_T(""), strRebarCode=_T("");
	int Index = m_SubBarName.GetCurSel();
	if(Index != -1)
	{
		UpdateData(TRUE);
		m_SubBarName.GetLBText(Index,strRebarName);

		Index = m_RebarCode.GetCurSel();
		if(Index != -1) m_RebarCode.GetLBText(Index,strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE),MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if(m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,rData))
		{
		    m_Fys = rData.B_fy;
		}
		UpdateData(FALSE);
	}
}

void CDgnConMatDlg::OnClickLambdaChk()
{
	UpdateData(TRUE);
	ControlsEnableDisable();
	if (m_bLambda) {
		GetDlgItem(IDC_DGN_LAMBDA_EDT)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_DGN_LAMBDA_EDT)->EnableWindow(FALSE);
		m_edtLambda.SetEditUnit(1.0);
	}
}

void CDgnConMatDlg::OnClickTransferChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_TRANSFER_EDT)->EnableWindow(m_bChkTransfer);
}

void CDgnConMatDlg::OnDgnConcmatModify() 
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}

	CString strID=_T(""), strName=_T(""), strCode=_T(""), strRebarCode=_T("");
	CString strFc=_T(""), strChk=_T(""), strLambda=_T(""), strMain=_T("");
	CString strSub =_T(""), strMatName=_T("");

	UpdateData(TRUE);
	if(ErrorCheck())
	{
		strID = m_ListCtrl.GetItemText(nItem,0);
		// Modify Material ID
		int nID = _ttoi(strID);
		T_MATD_D mData;
		mData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);

		// Get changed Code Name.
		int Index   = m_Code.GetCurSel();
		m_Code.GetLBText(Index,strCode);							// Get Material Code.
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
		mData.Data1.CodeName = strCode;
		int reIndex = m_RebarCode.GetCurSel();
		m_RebarCode.GetLBText(reIndex,strRebarCode);	// Get Rebar Code.
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif
		mData.Rebar_CodeName = strRebarCode;

		if(Index != 0)	// if Code exists.
		{
			Index = m_MatName.GetCurSel();
			if(Index != -1) m_MatName.GetLBText(Index,strMatName);
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatName);
#endif
			mData.Data1.CodeMatlName = strMatName;	// Get Material Code Name.
			
			strName	= strMatName;
			mData.Name = strName;
			// Get Analysis Data & Design Data.
			T_MATL_CONCRETE cData;
			BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode,strMatName,cData);
			if(bCheck)
			{
				mData.Data1.Design.C_fc	 = m_Fc;
				mData.Data1.Design.C_fci = 0.7*m_Fc;	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
				strFc.Format(_T("%g"),mData.Data1.Design.C_fc);

				if(m_bTransfer)
				{
					mData.Data1.Design.bTransfer    = m_bChkTransfer;
					mData.Data1.Design.dTransferFci = (m_bChkTransfer)? m_edtTransfer.GetEditValue() : 0.0;
				}
				else 
				{
					mData.Data1.Design.bTransfer = FALSE;
					mData.Data1.Design.dTransferFci = 0.0;
				}

				mData.Data1.Design.bLambda = m_bLambda;
				mData.Data1.Design.dLambda = m_edtLambda.GetEditValue();

				if(m_bLambda) strChk = _T("O");
				else          strChk = _T("X");

				strLambda.Format(_T("%g"), mData.Data1.Design.dLambda);
			}
		}
		else	// if Code not exists.
		{
			mData.Data1.CodeMatlName	= strCode;

			strName = m_MatName2;
			mData.Name = strName;

			mData.Data1.Design.C_fc	 = m_Fc;
			mData.Data1.Design.C_fci = 0.7*m_Fc;	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
			strFc.Format(_T("%g"),mData.Data1.Design.C_fc);

			if(m_bTransfer)
			{
				mData.Data1.Design.bTransfer    = m_bChkTransfer;
				mData.Data1.Design.dTransferFci = (m_bChkTransfer)? m_edtTransfer.GetEditValue() : 0.0;
			}
			else 
			{
				mData.Data1.Design.bTransfer    = FALSE;
				mData.Data1.Design.dTransferFci =  0.0;
			}

			mData.Data1.Design.bLambda = m_bLambda;
			mData.Data1.Design.dLambda = m_edtLambda.GetEditValue();

			if(m_bLambda) strChk = _T("O");
			else          strChk = _T("X");
			
			strLambda.Format(_T("%g"), mData.Data1.Design.dLambda);
		}

		if(reIndex != 0)	// if Code exists.
		{
			// Rebar Data를 얻는다.
			T_MATL_REBAR MainData,SubData;
			Index = m_MainBarName.GetCurSel();
			if(Index != -1) m_MainBarName.GetLBText(Index,strMain);
			mData.MainRebar_RebarName = strMain;
			MainData.Initialize();
			bCheck = m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strMain,MainData);
			mData.MainRebarData.B_Density = MainData.B_Density;
			mData.MainRebarData.B_Elast   = MainData.B_Elast;
			mData.MainRebarData.B_fy      = MainData.B_fy;

			Index = m_SubBarName.GetCurSel();
			if(Index != -1) m_SubBarName.GetLBText(Index,strSub);
			mData.SubRebar_RebarName = strSub;
			SubData.Initialize();
			bCheck = m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strSub,SubData);
			mData.SubRebarData.B_Density = SubData.B_Density;
			mData.SubRebarData.B_Elast   = SubData.B_Elast;
			mData.SubRebarData.B_fy      = SubData.B_fy;
		}
		else	// if Code not exists.
		{
			mData.Rebar_CodeName			= strRebarCode;  //  Rebar Code를 얻는다.

			strMain = m_MainBarName2;
			strSub = m_SubBarName2;

			mData.MainRebar_RebarName     = m_MainBarName2;
			mData.MainRebarData.B_Density = 0.0;
			mData.MainRebarData.B_Elast   = 0.0;
			mData.MainRebarData.B_fy      = m_Fy;

			mData.SubRebar_RebarName      = m_SubBarName2;
			mData.SubRebarData.B_Density  = 0.0;
			mData.SubRebarData.B_Elast    = 0.0;
			mData.SubRebarData.B_fy       = m_Fys;

		}

		bCheck = m_pDoc->m_pDataCtrl->AddMatd(nID,mData);
		if(bCheck)
		{
			m_ListCtrl.SetItemText(nItem,1,strName);	// Name.
			m_ListCtrl.SetItemText(nItem,2,strFc);		// fc'/fck.
			m_ListCtrl.SetItemText(nItem,3,strChk);   // Chk
			m_ListCtrl.SetItemText(nItem,4,strLambda);// Lambda
			m_ListCtrl.SetItemText(nItem,5,strMain);	// Main-Rebar.
			m_ListCtrl.SetItemText(nItem,6,strSub);		// Sub-Rebar.
			
			m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

BOOL CDgnConMatDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	if(m_Code.GetCurSel()==0)	
	{
		if(m_MatName2==_T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_CON_MATL_NAME_ERROR);}
	}
	if(m_RebarCode.GetCurSel()==0)
	{
		if(m_MainBarName2==_T("") || m_SubBarName2==_T(""))
		{
		  bCheck = FALSE;
		  strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
		}
	}

	double dLambda = m_edtLambda.GetEditValue();
	if(m_bLambda)
	{
		if(dLambda < 0.75 || dLambda > 1.0) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_LAMBDA_ERROR);}
	}

	if(m_Fc <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FC_ERROR);}
	if(m_Fy <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
	if(m_Fys<= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FYS_ERROR);}

	double dTransferFci = m_edtTransfer.GetEditValue();
	if(dTransferFci < 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FCI_ERROR);}

	//CString strcode = ConCode

	CString strCode = CDBLib::GetConCodeName();
	double Aremafylim = CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 420.0);

	double Aremafyslim = CUnitCtrl::ConvValueTgtUnit2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 420.0);
	if (strCode == CONCODE_AREMA_2023) {
		if (m_Fy > Aremafylim || m_Fys > Aremafylim) {
			bCheck = FALSE;
			strMsg = strMsg + _T("\n") + _LS(IDS_FY_LIM_ERROR);
	}
	
	}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	

	return bCheck;
}

// Coded by Seungjun MNet:No.2393 ('20061110)
void CDgnConMatDlg::OnDgnConcElasticity()
{
	CDgnConMatSLTEDlg dlg;

	dlg.m_iMatdKey = _ttoi(m_strCurrentItem);
	dlg.DoModal();
}
