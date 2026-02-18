// DgnCompositeMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCompositeMatDlg.h"

#include "DgnConMatSLTEDlg.h"

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
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCompositeMatDlg dialog


CDgnCompositeMatDlg::CDgnCompositeMatDlg(BOOL bPscDgn/*=FALSE*/, CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCompositeMatDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();

	m_bTransfer = FALSE;
    // 현재 m_bTransfer 는 PSC 설계 AS 기준만 적용됨. 
	if(bPscDgn) 
	{
		T_PSCD_D PscdD; PscdD.Initialize();
		m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		if(PscdD.iDgnCode == AS_5100_5_17_PSC) m_bTransfer = TRUE;
	}
}

void CDgnCompositeMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DGN_CONCMAT_COMP_LIST,       m_ListCtrl);

	DDX_Control(pDX, IDC_DGN_CONCMAT_GR_CODE_COMBO,   m_cmbConCodeGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_MAT_COMBO,       m_cmbConMatNameGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_REBARCODE_COMBO, m_cmbRebarCodeGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_MAINNAME_COMBO,  m_cmbMainBarNamGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_SUBNAME_COMBO,   m_cmbSubBarNameGr);

	DDX_Control(pDX, IDC_DGN_CONC_GR_MATFCUNIT,       m_FcUnitGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_MATFC_EDT,       m_edtFcGr);

	DDX_Control(pDX, IDC_DGN_CONC_GR_MATFYUNIT,      m_FyUnitGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_MATFY_EDT,      m_edtFyGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_MATFYSUNIT,     m_FysUnitGr);
	DDX_Control(pDX, IDC_DGN_CONC_GR_MATFYS_EDT,     m_edtFysGr);

	DDX_Control(pDX, IDC_DGN_TRANSFER_GR_CHK,        m_chkTransferGr);
	DDX_Control(pDX, IDC_DGN_TRANSFER_GR_EDT,        m_edtTransferGr);
	DDX_Control(pDX, IDC_DGN_TRANSFER_GR_UNIT,       m_TransferGrUnit);

	DDX_Control(pDX, IDC_DGN_LAMBDA_GR_CHK,          m_chkLambdaGr);
	DDX_Control(pDX, IDC_DGN_LAMBDA_GR_EDT,          m_edtLambdaGr);

	DDX_Control(pDX, IDC_DGN_CONC_MAT2,              m_edtMatNameGr);
	DDX_Control(pDX, IDC_DGN_CONC_SUBNAME2,          m_edtSubBarNameGr);
	DDX_Control(pDX, IDC_DGN_CONC_MAINNAME2,         m_edtMainBarNameGr);

	DDX_Control(pDX, IDC_DGN_CONCMAT_SL_CODE_COMBO,   m_cmbConCodeSl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_MAT_COMBO,       m_cmbConMatNameSl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_REBARCODE_COMBO, m_cmbRebarCodeSl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_MAINNAME_COMBO,  m_cmbMainBarNamSl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_SUBNAME_COMBO,   m_cmbSubBarNameSl);

	DDX_Control(pDX, IDC_DGN_CONC_SL_MATFCUNIT,      m_FcUnitSl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_MATFC_EDT,      m_edtFcSl);

	DDX_Control(pDX, IDC_DGN_CONC_SL_MATFYUNIT,      m_FyUnitSl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_MATFY_EDT,      m_edtFySl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_MATFYSUNIT,     m_FysUnitSl);
	DDX_Control(pDX, IDC_DGN_CONC_SL_MATFYS_EDT,     m_edtFysSl);

	DDX_Control(pDX, IDC_DGN_LAMBDA_SL_CHK,          m_chkLambdaSl);
	DDX_Control(pDX, IDC_DGN_LAMBDA_SL_EDT,          m_edtLambdaSl);

	DDX_Control(pDX, IDC_DGN_CONC_MAT3,       m_edtMatNameSl);
	DDX_Control(pDX, IDC_DGN_CONC_SUBNAME3,   m_edtSubBarNameSl);
	DDX_Control(pDX, IDC_DGN_CONC_MAINNAME3,  m_edtMainBarNameSl);
}

BEGIN_MESSAGE_MAP(CDgnCompositeMatDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnCompositeMatDlg)
	ON_NOTIFY(NM_CLICK,        IDC_DGN_CONCMAT_COMP_LIST, OnClickDgnConcmatList) 
 	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_CONCMAT_COMP_LIST, OnItemchangedDgnConcMatList)

	ON_CBN_SELCHANGE(IDC_DGN_CONCMAT_GR_CODE_COMBO,       OnSelchangeDgnConcMatCode)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_GR_MAT_COMBO,           OnSelchangeDgnConcMatName)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_GR_REBARCODE_COMBO,     OnSelchangeDgnConcRebarCode)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_GR_MAINNAME_COMBO,      OnSelchangeDgnConcRebarMainName)
 	ON_CBN_SELCHANGE(IDC_DGN_CONC_GR_SUBNAME_COMBO,       OnSelchangeDgnConcRebarSubName)
 	ON_BN_CLICKED   (IDC_DGN_LAMBDA_GR_CHK,               OnClickLambdaChk )
	ON_BN_CLICKED   (IDC_DGN_CONC_GR_EL,                  OnDgnConcElasticity)
	ON_BN_CLICKED   (IDC_DGN_TRANSFER_GR_CHK,             OnClickTransferChk )

	ON_CBN_SELCHANGE(IDC_DGN_CONCMAT_SL_CODE_COMBO,       OnSelchangeDgnConcMatCodeSlab)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_SL_MAT_COMBO,           OnSelchangeDgnConcMatNameSlab)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_SL_REBARCODE_COMBO,     OnSelchangeDgnConcRebarCodeSlab)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_SL_MAINNAME_COMBO,      OnSelchangeDgnConcRebarMainNameSlab)
 	ON_CBN_SELCHANGE(IDC_DGN_CONC_SL_SUBNAME_COMBO,       OnSelchangeDgnConcRebarSubNameSlab)
 	ON_BN_CLICKED   (IDC_DGN_LAMBDA_SL_CHK,               OnClickLambdaChkSlab )
	ON_BN_CLICKED   (IDC_DGN_CONC_SL_EL,                  OnDgnConcElasticitySlab)

	ON_BN_CLICKED(IDC_DGN_CONCMAT_MODIFY, OnDgnConcmatModify) 
	ON_BN_CLICKED(IDC_DGN_CONCMAT_CLOSE, OnDgnConcmatClose)  

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDgnCompositeMatDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;
	CArray<UINT, UINT> aControls;

	// control move
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_LAMBDA_GR_EDT);
	aControls.Add(IDC_DGN_LAMBDA_GR_CHK);

 	GetDlgItem(IDC_DGN_CONC_GR_MATFC_EDT)->GetWindowRect(rRef);
	if(m_bTransfer)
	{
		GetDlgItem(IDC_DGN_TRANSFER_GR_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(3);
	}
	else
	{
		GetDlgItem(IDC_DGN_LAMBDA_GR_EDT)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(3);
	}
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Concrete Selection Group(GR) resize
	CRect rectResize;
	GetDlgItem(IDC_DGN_CONC_GR_GRP)->GetWindowRect(rectResize);
	GetDlgItem(IDC_DGN_LAMBDA_GR_EDT)->GetWindowRect(rRef);
	rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(5);

	ScreenToClient(rectResize);
	GetDlgItem(IDC_DGN_CONC_GR_GRP)->MoveWindow(rectResize);

	// Rebar Selection Group(GR) move
	aControls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this,aControls,IDC_DGN_REBAR_GR_GRP, TRUE);
	
	GetDlgItem(IDC_DGN_CONC_GR_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_REBAR_GR_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	// Girder group resize
	GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rectResize);
	GetDlgItem(IDC_DGN_REBAR_GR_GRP)->GetWindowRect(rRef);
	rectResize.bottom = rRef.bottom + globalUtils.ScaleByDPI(5);

	ScreenToClient(rectResize);
	GetDlgItem(IDC_WG_DGN_STATIC1)->MoveWindow(rectResize);

	// Slab controls move
	aControls.RemoveAll();
	CDlgUtil::GetCtrlIDByIncRect(this,aControls,IDC_WG_DGN_STATIC2, TRUE);
	
	GetDlgItem(IDC_WG_DGN_STATIC1)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_DGN_STATIC2)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// control move
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_CONCMAT_MODIFY);
	aControls.Add(IDC_DGN_CONCMAT_CLOSE);

	GetDlgItem(IDC_WG_DGN_STATIC2)->GetWindowRect(rRef);
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

void CDgnCompositeMatDlg::InitCtrls()
{
	Init_Items();
	Init_ItemsSlab();

	Init_ListCtrl();   // Initialize list
	Init_Data();       // data 2 list

	EnableEditBox(FALSE);     
	EnableRebarEditBox(FALSE);
	EnableEditBoxSlab(FALSE);     
	EnableRebarEditBoxSlab(FALSE);
//   OnSelchangeDgnConcMatCode();
//   OnSelchangeDgnConcRebarCode();
//   OnSelchangeDgnConcMatCodeSlab();
//   OnSelchangeDgnConcRebarCodeSlab();
//   OnClickLambdaChk();
//   OnClickLambdaChkSlab();
//   UpdateData(FALSE);

	int nIndex=0;
	if(!GetSelectedItem(nIndex)) return;  // set default item

}

void CDgnCompositeMatDlg::Init_Items()
{
	GetDlgItem(IDC_DGN_TRANSFER_GR_CHK)->ShowWindow(m_bTransfer);	
	GetDlgItem(IDC_DGN_TRANSFER_GR_EDT)->ShowWindow(m_bTransfer);	
	GetDlgItem(IDC_DGN_TRANSFER_GR_UNIT)->ShowWindow(m_bTransfer);	

	if(!(CDBLib::GetConCodeName()==_T("Eurocode2") || CDBLib::GetConCodeName()==_T("Eurocode2:04") ||
		CDBLib::GetConCodeName()==_T("Eurocode2-2:05"))) // Coded by Seungjun ('20070613) MNet:No.2313.
	{
		GetDlgItem(IDC_DGN_CONC_GR_EL)->ShowWindow(SW_HIDE);
		return;
	}

	GetDlgItem(IDC_DGN_CONC_GR_EL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_CONC_GR_EL)->EnableWindow(FALSE);

}

void CDgnCompositeMatDlg::Init_ItemsSlab()
{
	if(!(CDBLib::GetConCodeName()==_T("Eurocode2") || CDBLib::GetConCodeName()==_T("Eurocode2:04") ||
		CDBLib::GetConCodeName()==_T("Eurocode2-2:05"))) // Coded by Seungjun ('20070613) MNet:No.2313.
	{
		GetDlgItem(IDC_DGN_CONC_SL_EL)->ShowWindow(SW_HIDE);
		return;
	}

	GetDlgItem(IDC_DGN_CONC_SL_EL)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_CONC_SL_EL)->EnableWindow(FALSE);
}

void CDgnCompositeMatDlg::Init_ListCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;

	TCHAR* list[8] = {_T("ID"), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_NAME),
		(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_CONCRETE_GR), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MAIN_BAR_GR), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_SUB_BAR_GR),
		(TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_CONCRETE_SL), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_MAIN_BAR_SL), (TCHAR*)(LPCTSTR)_LS(IDS_DGN_LIST_SUB_BAR_SL)}; 
	int width[8]	= {40,85,80,80,80,80,80,80};


	for(int i=0; i<8; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}

}

void CDgnCompositeMatDlg::Init_Data()
{
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
		//Set_DlgData(mData);
		if(mData.Type==_T("C"))
		{
			CString strID=_T(""), strFc=_T("");
			strID.Format(_T("%7d"), iID);
			CString strNa		= mData.Name;
			CString strConNameGr = mData.Data1.CodeMatlName;
			CString strMbarGr	   = mData.MainRebar_RebarName;
			CString strSbarGr	   = mData.SubRebar_RebarName;

			CString strConNameSl = mData.Data2.CodeMatlName;    //if(strConNameSl ==_T("None") ||strConNameSl ==_T("")) strConNameSl = strConNameGr;
			CString strMbarSl	   = mData.MainRebar_RebarName2;  //if(strMbarSl    ==_T("None") ||strMbarSl    ==_T("")) strMbarSl    = strMbarGr;
			CString strSbarSl	   = mData.SubRebar_RebarName2;   //if(strSbarSl    ==_T("None") ||strSbarSl    ==_T("")) strSbarSl    = strSbarGr;

			Write_ListBox(iListNo,strID,strNa,strConNameGr,strMbarGr,strSbarGr,strConNameSl,strMbarSl,strSbarSl);
			iListNo++;
		}
	}
}


void CDgnCompositeMatDlg::Set_DlgData(T_MATD_D& mData)
{

	if(mData.Rebar_CodeName2 ==_T("None") ||mData.Rebar_CodeName2 ==_T("")) 
	{
		mData.Rebar_CodeName2 = mData.Rebar_CodeName;
		mData.MainRebarData2  = mData.MainRebarData;
		mData.SubRebarData2   = mData.SubRebarData;

	}
	if(mData.MainRebar_RebarName2  ==_T("None") ||mData.MainRebar_RebarName2 ==_T("")) mData.MainRebar_RebarName2  = mData.MainRebar_RebarName ;
	if(mData.SubRebar_RebarName2   ==_T("None") ||mData.SubRebar_RebarName2  ==_T("")) mData.SubRebar_RebarName2   = mData.SubRebar_RebarName  ;


	if(mData.Data2.CodeMatlName  ==_T("None") ||mData.Data2.CodeMatlName ==_T("")) 
	{
		mData.Data2.CodeMatlName   = mData.Data1.CodeMatlName ;
		mData.Data2.Design.C_fc    = mData.Data1.Design.C_fc;
		mData.Data2.Design.bLambda = mData.Data1.Design.bLambda;
		mData.Data2.Design.dLambda = mData.Data1.Design.dLambda;

		mData.MainRebarData2.B_fy      = mData.MainRebarData.B_fy     ; 
		mData.MainRebarData2.B_Elast   = mData.MainRebarData.B_Elast  ; 
		mData.MainRebarData2.B_Density = mData.MainRebarData.B_Density; 

		mData.SubRebarData2.B_fy      = mData.SubRebarData.B_fy     ; 
		mData.SubRebarData2.B_Elast   = mData.SubRebarData.B_Elast  ; 
		mData.SubRebarData2.B_Density = mData.SubRebarData.B_Density; 
	}

}

void CDgnCompositeMatDlg::Write_ListBox(int iIndex, CString strID, CString strNa, CString strConNameGr, CString strMbarGr, CString strSbarGr, CString strConNameSl, CString strMbarSl, CString strSbarSl)
{
	LV_ITEM lvitem;
	LPTSTR szText[8];
	szText[0] = (LPTSTR)(LPCTSTR)strID    ;
	szText[1] = (LPTSTR)(LPCTSTR)strNa    ;
	szText[2] = (LPTSTR)(LPCTSTR)strConNameGr;
	szText[3] = (LPTSTR)(LPCTSTR)strMbarGr   ;
	szText[4] = (LPTSTR)(LPCTSTR)strSbarGr   ;
	szText[5] = (LPTSTR)(LPCTSTR)strConNameSl;
	szText[6] = (LPTSTR)(LPCTSTR)strMbarSl   ;
	szText[7] = (LPTSTR)(LPCTSTR)strSbarSl   ;

	// Input ListBox.
	for(int i=0; i<8; i++)
	{
		lvitem.mask		  = LVIF_TEXT;
		lvitem.iItem  	= iIndex;
		lvitem.iSubItem = i;
		lvitem.pszText	= szText[i];
		if(i==0)	m_ListCtrl.InsertItem(&lvitem);
		else			m_ListCtrl.SetItem(&lvitem);
	}
}

BOOL CDgnCompositeMatDlg::GetSelectedItem(int &nIndex)
{
	int iItem = m_ListCtrl.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CDgnCompositeMatDlg::Init_Unit()
{
	m_FcUnitGr.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FyUnitGr.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FysUnitGr.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_edtFcGr.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtFyGr.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtFysGr.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtLambdaGr.SetUnitType(D_UNITSYS_NONE);
	m_edtTransferGr.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_FcUnitSl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FyUnitSl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_FysUnitSl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_TransferGrUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtFcSl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtFySl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtFysSl.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtLambdaSl.SetUnitType(D_UNITSYS_NONE);

}


void CDgnCompositeMatDlg::Init_CodeCombo()
{
	// Reset Contents.
	m_cmbConCodeGr.ResetContent();
	m_cmbRebarCodeGr.ResetContent();

	CArray<CString, CString&> arDesignCode;
	CString strMatlType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arDesignCode);
	// Initialize MatlCode, RebarCode.
	m_cmbConCodeGr.AddString(_T("None"));
	m_cmbRebarCodeGr.AddString(_T("None"));
	for(int i=0; i<arDesignCode.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arDesignCode[i]);
#endif
		m_cmbConCodeGr.AddString(arDesignCode[i]);
	}
	for(int i=0; i<arDesignCode.GetSize(); i++)	
	{
		m_cmbRebarCodeGr.AddString(arDesignCode[i]);
	}

	Init_NameCombo();
}

void CDgnCompositeMatDlg::Init_NameCombo()
{
	CString strMatlCode;
	if(m_Data.Data1.CodeName==_T("None") || m_Data.Data1.CodeName==_T(""))	// None.
	{
		strMatlCode = _T("None");
		EnableEditBox(TRUE);
	}
	else	// DB.
	{
		strMatlCode = m_Data.Data1.CodeName;
		EnableEditBox(FALSE);
	}

	CString strRebarCode;
	if(m_Data.Rebar_CodeName==_T("None") || m_Data.Rebar_CodeName==_T(""))	// None.
	{
		strRebarCode = _T("None");
		EnableRebarEditBox(TRUE);
	}
	else	// DB.
	{
		strRebarCode = m_Data.Rebar_CodeName;
		EnableRebarEditBox(FALSE);
	}

	CString CodeName;
	CodeName = strRebarCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	int iIndex = 0;
	iIndex = m_cmbRebarCodeGr.FindStringExact(-1, CodeName);
	m_cmbRebarCodeGr.SetCurSel(iIndex);

	CodeName = strMatlCode;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	iIndex = m_cmbConCodeGr.FindStringExact(-1, strMatlCode);
	m_cmbConCodeGr.SetCurSel(iIndex);

	Init_MatlNameCombo(strMatlCode, m_Data.Data1.CodeMatlName);
	Init_RebarNameCombo(strRebarCode, m_Data);

	UpdateData(FALSE);
}

void CDgnCompositeMatDlg::Init_CodeComboSlab()
{ 
	// Reset Contents.
	m_cmbConCodeSl.ResetContent();
	m_cmbRebarCodeSl.ResetContent();

	CArray<CString, CString&> arDesignCode;
	CString strMatlType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arDesignCode);
	// Initialize MatlCode, RebarCode.
	m_cmbConCodeSl.AddString(_T("None"));
	m_cmbRebarCodeSl.AddString(_T("None"));
	for(int i=0; i<arDesignCode.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arDesignCode[i]);
#endif
		m_cmbConCodeSl.AddString(arDesignCode[i]);
	}
	for(int i=0; i<arDesignCode.GetSize(); i++)	
	{
		m_cmbRebarCodeSl.AddString(arDesignCode[i]);
	}

	Init_NameComboSlab();
	
	UpdateData(FALSE);
}

void CDgnCompositeMatDlg::Init_NameComboSlab()
{
	CString strMatlCode_Sl;
	if(m_Data.Data2.CodeName==_T("None") || m_Data.Data2.CodeName==_T(""))	// None.
	{
		strMatlCode_Sl = _T("None"); //strMatlCode;
		EnableEditBoxSlab(TRUE);
	}
	else	// DB.
	{
		strMatlCode_Sl = m_Data.Data2.CodeName;
		EnableEditBoxSlab(FALSE);
	}

	CString strRebarCode_Sl;
	if(m_Data.Rebar_CodeName2==_T("None") || m_Data.Rebar_CodeName2==_T(""))	// None.
	{
		strRebarCode_Sl = _T("None"); //strRebarCode;
		EnableRebarEditBoxSlab(TRUE);
	}
	else	// DB.
	{
		strRebarCode_Sl = m_Data.Rebar_CodeName2;
		EnableRebarEditBoxSlab(FALSE);
	}

	CString CodeName;
	CodeName = strRebarCode_Sl;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	int iIndex = 0;
	iIndex = m_cmbRebarCodeSl.FindStringExact(-1, CodeName);
	m_cmbRebarCodeSl.SetCurSel(iIndex);

	CodeName = strMatlCode_Sl;
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
	iIndex = m_cmbConCodeSl.FindStringExact(-1, CodeName);
	m_cmbConCodeSl.SetCurSel(iIndex);

	CString strCodeMatlName_Sl = m_Data.Data2.CodeMatlName;  
	if(strCodeMatlName_Sl==_T("None") || strCodeMatlName_Sl==_T("")) strCodeMatlName_Sl = m_Data.Data1.CodeMatlName;

	Init_MatlNameComboSlab(strMatlCode_Sl, strCodeMatlName_Sl);
	Init_RebarNameComboSlab(strRebarCode_Sl, m_Data);

	UpdateData(FALSE);
}


void CDgnCompositeMatDlg::Init_RebarNameCombo(CString strRebarCode, T_MATD_D rData)
{
	m_cmbMainBarNamGr.ResetContent();
	m_cmbSubBarNameGr.ResetContent();

	T_MATL_REBAR RebarData;
	if(strRebarCode!=_T("None") && strRebarCode!=_T(""))
	{
		// Initialize Rebar Name ComboBox by Code.
		CArray<CString,CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, arRebar);
		for(int i=0; i<arRebar.GetSize(); i++)  
		{
			m_cmbMainBarNamGr.AddString(arRebar[i]);
			m_cmbSubBarNameGr.AddString(arRebar[i]);
		}
		int Index = m_cmbMainBarNamGr.FindStringExact(-1, rData.MainRebar_RebarName);
		if(Index!=-1)
		{
			m_cmbMainBarNamGr.SetCurSel(Index);
			if(rData.MainRebarData.B_fy==0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.MainRebar_RebarName, RebarData);
				m_edtFyGr.SetEditUnit(RebarData.B_fy);
			}
			else	m_edtFyGr.SetEditUnit(rData.MainRebarData.B_fy);
		}
		else	m_edtFyGr.SetEditUnit(0.0);

		Index = m_cmbSubBarNameGr.FindStringExact(-1, rData.SubRebar_RebarName);
		if(Index!=-1)
		{
			m_cmbSubBarNameGr.SetCurSel(Index);
			if(rData.SubRebarData.B_fy==0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.SubRebar_RebarName, RebarData);
				m_edtFysGr.SetEditUnit(RebarData.B_fy);
			}
			else	m_edtFysGr.SetEditUnit(rData.SubRebarData.B_fy);
		}
		else	m_edtFysGr.SetEditUnit(0.0);
	}
	UpdateData(FALSE);
}

void CDgnCompositeMatDlg::Init_RebarNameComboSlab(CString strRebarCode, T_MATD_D rData)
{
	m_cmbMainBarNamSl.ResetContent();
	m_cmbSubBarNameSl.ResetContent();

	T_MATL_REBAR RebarData;
	if(strRebarCode!=_T("None") && strRebarCode!=_T(""))
	{
		// Initialize Rebar Name ComboBox by Code.
		CArray<CString,CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, arRebar);
		for(int i=0; i<arRebar.GetSize(); i++)  
		{
			m_cmbMainBarNamSl.AddString(arRebar[i]);
			m_cmbSubBarNameSl.AddString(arRebar[i]);
		}
		int Index = m_cmbMainBarNamSl.FindStringExact(-1, rData.MainRebar_RebarName2);
		if(Index!=-1)
		{
			m_cmbMainBarNamSl.SetCurSel(Index);
			if(rData.MainRebarData2.B_fy==0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.MainRebar_RebarName2, RebarData);
				m_edtFySl.SetEditUnit(RebarData.B_fy);
			}
			else	m_edtFySl.SetEditUnit(rData.MainRebarData2.B_fy);
		}
		else	m_edtFySl.SetEditUnit(0.0);

		Index = m_cmbSubBarNameSl.FindStringExact(-1, rData.SubRebar_RebarName2);
		if(Index!=-1)
		{
			m_cmbSubBarNameSl.SetCurSel(Index);
			if(rData.SubRebarData2.B_fy==0.0)
			{
				RebarData.Initialize();
				m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, rData.SubRebar_RebarName2, RebarData);
				m_edtFysSl.SetEditUnit(RebarData.B_fy);
			}
			else	m_edtFysSl.SetEditUnit(rData.SubRebarData2.B_fy);
		}
		else	m_edtFysSl.SetEditUnit(0.0);
	}
	UpdateData(FALSE);
}


void CDgnCompositeMatDlg::Init_MatlNameCombo(CString strCode, CString strNa)
{
	if(strCode==_T("None") || strCode==_T(""))	return;	// Skip if None.

	m_cmbConMatNameGr.ResetContent();
	CArray<CString,CString&> arName;
	m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arName);
	for (int i = 0; i < arName.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arName[i]);
#endif
		m_cmbConMatNameGr.AddString(arName[i]);
	}

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, strNa);
#endif
	int Index = m_cmbConMatNameGr.FindStringExact(-1, strNa);
	if(Index!=-1)	m_cmbConMatNameGr.SetCurSel(Index);

	OnSelchangeDgnConcMatName(); 
}

void CDgnCompositeMatDlg::Init_MatlNameComboSlab(CString strCode, CString strNa)
{
	if(strCode==_T("None") || strCode==_T(""))	return;	// Skip if None.

	m_cmbConMatNameSl.ResetContent();
	CArray<CString,CString&> arName;
	m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arName);
	for (int i = 0; i < arName.GetSize(); i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arName[i]);
#endif
		m_cmbConMatNameSl.AddString(arName[i]);
	}

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, strNa);
#endif

	int Index = m_cmbConMatNameSl.FindStringExact(-1, strNa);
	if(Index!=-1)	m_cmbConMatNameSl.SetCurSel(Index);

	OnSelchangeDgnConcMatNameSlab();
}


BOOL CDgnCompositeMatDlg::Data2Dlg()
{

	UpdateData(TRUE);
	return TRUE;
}



void CDgnCompositeMatDlg::ChangeDlgItemInfo()
{
	CString ConcCode = _T("");
	CString RBarCode = _T("");
	int ConcIdx = m_cmbConCodeGr.GetCurSel();
	if(ConcIdx >= 0)
		m_cmbConCodeGr.GetLBText(ConcIdx, ConcCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(ConcCode);
#endif

	if(ConcCode == MATLCODE_CON_GB_CIVIL)
		GetDlgItem(IDC_DGN_STATIC_FC_GR)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_JTJ));
	else if(ConcCode == MATLCODE_CON_EN04)
		GetDlgItem(IDC_DGN_STATIC_FC_GR)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_EN04));
	else
		GetDlgItem(IDC_DGN_STATIC_FC_GR)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_ETC));

	int RBarIdx = m_cmbRebarCodeGr.GetCurSel();
	if(RBarIdx >= 0)
		m_cmbRebarCodeGr.GetLBText(RBarIdx, RBarCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(RBarCode);
#endif
	if(RBarCode == MATLCODE_CON_GB_CIVIL )
	{
		GetDlgItem(IDC_DGN_STATIC_FY_GR)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_JTJ));
		GetDlgItem(IDC_DGN_STATIC_FYS_GR)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_JTJ));
	}
	else
	{
		GetDlgItem(IDC_DGN_STATIC_FY_GR)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_ETC));
		GetDlgItem(IDC_DGN_STATIC_FYS_GR)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_ETC));
	}

	//UpdateData(FALSE);
}

void CDgnCompositeMatDlg::ChangeDlgItemInfoSlab()
{
	CString ConcCode = _T("");
	CString RBarCode = _T("");
	int ConcIdx = m_cmbConCodeSl.GetCurSel();
	if(ConcIdx >= 0)
		m_cmbConCodeSl.GetLBText(ConcIdx, ConcCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(ConcCode);
#endif

	if(ConcCode == MATLCODE_CON_GB_CIVIL )
		GetDlgItem(IDC_DGN_STATIC_FC_SL)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_JTJ));
	else if(ConcCode == MATLCODE_CON_EN04)
		GetDlgItem(IDC_DGN_STATIC_FC_SL)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_EN04));
	else
		GetDlgItem(IDC_DGN_STATIC_FC_SL)->SetWindowText(_LS(IDS_DGN_TEXT_CONCSTRENGTH_ETC));

	int RBarIdx = m_cmbRebarCodeSl.GetCurSel();
	if(RBarIdx >= 0)
		m_cmbRebarCodeSl.GetLBText(RBarIdx, RBarCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(RBarCode);
#endif
	if(RBarCode == MATLCODE_CON_GB_CIVIL)
	{
		GetDlgItem(IDC_DGN_STATIC_FY_SL)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_JTJ));
		GetDlgItem(IDC_DGN_STATIC_FYS_SL)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_JTJ));
	}
	else
	{
		GetDlgItem(IDC_DGN_STATIC_FY_SL)->SetWindowText(_LS(IDS_DGN_TEXT_RBARSTRENGTH_ETC));
		GetDlgItem(IDC_DGN_STATIC_FYS_SL)->SetWindowText(_LS(IDS_DGN_TEXT_SUBBARSTRENGTH_ETC));
	}

	//UpdateData(FALSE);
}



void CDgnCompositeMatDlg::EnableEditBox(BOOL bCheck)
{
	if(bCheck)	// None.
	{
		//GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
		GetDlgItem(IDC_DGN_CONC_GR_MAT_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MAT2        )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_GR_MATFC_EDT)->EnableWindow(TRUE);
	}
	else	// DB.
	{
		//GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
		GetDlgItem(IDC_DGN_CONC_GR_MAT_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MAT2        )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_GR_MATFC_EDT)->EnableWindow(FALSE);
	}
}
void CDgnCompositeMatDlg::EnableEditBoxSlab(BOOL bCheck)
{
	if(bCheck)	// None.
	{
		//GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_NAME));
		GetDlgItem(IDC_DGN_CONC_SL_MAT_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MAT3        )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_SL_MATFC_EDT)->EnableWindow(TRUE);
	}
	else	// DB.
	{
		//GetDlgItem(IDC_STATIC_MATNAME)->SetWindowText(_LS(IDS_DGN_TEXT_DB_NAME));
		GetDlgItem(IDC_DGN_CONC_SL_MAT_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MAT3        )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SL_MATFC_EDT)->EnableWindow(FALSE);
	}
}



void CDgnCompositeMatDlg::EnableRebarEditBox(BOOL bCheck)
{
	if(bCheck)	// None.
	{
		GetDlgItem(IDC_DGN_STATIC_MATEFY_GR)->SetWindowText(_LS(IDS_DGN_TEXT_FY_NAME));
		GetDlgItem(IDC_DGN_STATIC_MATAFV_GR)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_NAME));
		GetDlgItem(IDC_DGN_CONC_GR_MAINNAME_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MAINNAME2        )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_GR_MATFY_EDT     )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_GR_SUBNAME_COMBO )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SUBNAME2         )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_GR_MATFYS_EDT    )->EnableWindow(TRUE);
	}
	else	// DB.
	{
		GetDlgItem(IDC_DGN_STATIC_MATEFY_GR)->SetWindowText(_LS(IDS_DGN_TEXT_FY_DB_NAME));
		GetDlgItem(IDC_DGN_STATIC_MATAFV_GR)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_DB_NAME));
		GetDlgItem(IDC_DGN_CONC_GR_MAINNAME_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MAINNAME2        )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_GR_MATFY_EDT     )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_GR_SUBNAME_COMBO )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_SUBNAME2         )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_GR_MATFYS_EDT    )->EnableWindow(FALSE);
	}
}
void CDgnCompositeMatDlg::EnableRebarEditBoxSlab(BOOL bCheck)
{
	if(bCheck)	// None.
	{
		GetDlgItem(IDC_DGN_STATIC_MATEFY_SL)->SetWindowText(_LS(IDS_DGN_TEXT_FY_NAME));
		GetDlgItem(IDC_DGN_STATIC_MATAFV_SL)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_NAME));
		GetDlgItem(IDC_DGN_CONC_SL_MAINNAME_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_MAINNAME3        )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_SL_MATFY_EDT     )->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_SL_SUBNAME_COMBO )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SUBNAME3         )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_SL_MATFYS_EDT    )->EnableWindow(TRUE);
	}
	else	// DB.
	{
		GetDlgItem(IDC_DGN_STATIC_MATEFY_SL)->SetWindowText(_LS(IDS_DGN_TEXT_FY_DB_NAME));
		GetDlgItem(IDC_DGN_STATIC_MATAFV_SL)->SetWindowText(_LS(IDS_DGN_TEXT_FYS_DB_NAME));
		GetDlgItem(IDC_DGN_CONC_SL_MAINNAME_COMBO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_MAINNAME3        )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SL_MATFY_EDT     )->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_SL_SUBNAME_COMBO )->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_SUBNAME3         )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SL_MATFYS_EDT    )->EnableWindow(FALSE);
	}
}


void CDgnCompositeMatDlg::DgnConcmatModify(int nItem)
{

	CString strID=_T(""), strName=_T(""), strCode=_T(""), strRebarCode=_T("");
	CString strFc=_T(""), strChk=_T(""), strLambda=_T(""), strMain=_T("");
	CString strSub =_T(""), strMatName=_T("");

	if(ErrorCheck())
	{
		strID = m_ListCtrl.GetItemText(nItem,0);
		// Modify Material ID
		int nID = _ttoi(strID);
		T_MATD_D mData;
		mData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);
		// Set_DlgData(mData);

		// Get changed Code Name.
		int Index   = m_cmbConCodeGr.GetCurSel();
		m_cmbConCodeGr.GetLBText(Index,strCode);							// Get Material Code.
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
		mData.Data1.CodeName = strCode;
		int reIndex = m_cmbRebarCodeGr.GetCurSel();
		m_cmbRebarCodeGr.GetLBText(reIndex,strRebarCode);	// Get Rebar Code.
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif
		mData.Rebar_CodeName = strRebarCode;

		if(Index != 0)	// if Code exists.
		{
			Index = m_cmbConMatNameGr.GetCurSel();
			if(Index != -1) m_cmbConMatNameGr.GetLBText(Index,strMatName);

#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatName);
#endif

			mData.Data1.CodeMatlName = strMatName;	// Get Material Code Name.

			//strName	= strMatName; //MQC4581-SALEE
			//mData.Name = strName;
			// Get Analysis Data & Design Data.
			T_MATL_CONCRETE cData;
			BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode,strMatName,cData);
			if(bCheck)
			{
				mData.Data1.Design.C_fc	 = m_edtFcGr.GetEditValue();
				mData.Data1.Design.C_fci = 0.7*m_edtFcGr.GetEditValue();	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
				strFc.Format(_T("%g"),mData.Data1.Design.C_fc);

				if(m_bTransfer)
				{
					mData.Data1.Design.bTransfer    = m_chkTransferGr.GetCheck();
					if(m_chkTransferGr.GetCheck())
					{
						mData.Data1.Design.dTransferFci = m_edtTransferGr.GetEditValue();
					}
					else
						mData.Data1.Design.dTransferFci = 0.0;
				}
				else 
				{
					mData.Data1.Design.bTransfer = FALSE;
					mData.Data1.Design.dTransferFci = 0.0;
				}

				mData.Data1.Design.bLambda = m_chkLambdaGr.GetCheck();
				mData.Data1.Design.dLambda = m_edtLambdaGr.GetEditValue();

				if(m_chkLambdaGr.GetCheck()) 
					strChk = _T("O");
				else          
					strChk = _T("X");

				strLambda.Format(_T("%g"), mData.Data1.Design.dLambda);
			}
		}
		else	// if Code not exists.
		{
			mData.Data1.CodeMatlName	= strCode;

			mData.Data1.Design.C_fc	 = m_edtFcGr.GetEditValue();
			mData.Data1.Design.C_fci = 0.7*m_edtFcGr.GetEditValue();	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
			strFc.Format(_T("%g"),mData.Data1.Design.C_fc);

			if(m_bTransfer)
			{
				mData.Data1.Design.bTransfer    = m_chkTransferGr.GetCheck();
				if(m_chkTransferGr.GetCheck())
				{
					mData.Data1.Design.dTransferFci = m_edtTransferGr.GetEditValue();
				}
				else
					mData.Data1.Design.dTransferFci = 0.0;			}
			else 
			{
				mData.Data1.Design.bTransfer = FALSE;
				mData.Data1.Design.dTransferFci = 0.0;
			}

			mData.Data1.Design.bLambda = m_chkLambdaGr.GetCheck();
			mData.Data1.Design.dLambda = m_edtLambdaGr.GetEditValue();

			if(m_chkLambdaGr.GetCheck()) 
				strChk = _T("O");
			else          
				strChk = _T("X");

			strLambda.Format(_T("%g"), mData.Data1.Design.dLambda);
		}

		if(reIndex != 0)	// if Code exists.
		{
			// Rebar Data를 얻는다.
			T_MATL_REBAR MainData,SubData;
			Index = m_cmbMainBarNamGr.GetCurSel();
			if(Index != -1) m_cmbMainBarNamGr.GetLBText(Index,strMain);
			mData.MainRebar_RebarName = strMain;
			MainData.Initialize();
			bCheck = m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strMain,MainData);
			mData.MainRebarData.B_Density = MainData.B_Density;
			mData.MainRebarData.B_Elast   = MainData.B_Elast;
			mData.MainRebarData.B_fy      = MainData.B_fy;

			Index = m_cmbSubBarNameGr.GetCurSel();
			if(Index != -1) m_cmbSubBarNameGr.GetLBText(Index,strSub);
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

			CString strMainBarNameGr; m_edtMainBarNameGr.GetWindowText(strMainBarNameGr);
			CString strSubBarNameGr; m_edtSubBarNameGr.GetWindowText(strSubBarNameGr);
			strMain = strMainBarNameGr;
			strSub  = strSubBarNameGr;

			mData.MainRebar_RebarName     = strMainBarNameGr;
			mData.MainRebarData.B_Density = 0.0;
			mData.MainRebarData.B_Elast   = 0.0;
			mData.MainRebarData.B_fy      = m_edtFyGr.GetEditValue();

			mData.SubRebar_RebarName      = strSubBarNameGr;
			mData.SubRebarData.B_Density  = 0.0;
			mData.SubRebarData.B_Elast    = 0.0;
			mData.SubRebarData.B_fy       = m_edtFysGr.GetEditValue();

		}

		strName = mData.Name; //MQC4581-SALEE
		bCheck = m_pDoc->m_pDataCtrl->AddMatd(nID,mData);
		if(bCheck)
		{
			CString strConNameGr = mData.Data1.CodeMatlName;
			CString strMbarGr	   = mData.MainRebar_RebarName;
			CString strSbarGr	   = mData.SubRebar_RebarName;
			//       CString strConNameSl = mData.Data2.CodeMatlName;
			//       CString strMbarSl	   = mData.MainRebar_RebarName2;
			//       CString strSbarSl	   = mData.SubRebar_RebarName2;

			m_ListCtrl.SetItemText(nItem,1,strName);	     // Name.
			m_ListCtrl.SetItemText(nItem,2,strConNameGr);	 // Name.
			m_ListCtrl.SetItemText(nItem,3,strMain);	/*strMbarGr*/
			m_ListCtrl.SetItemText(nItem,4,strSub);   /*strSbarGr*/
			// 			m_ListCtrl.SetItemText(nItem,4,strConNameSl);
			// 			m_ListCtrl.SetItemText(nItem,5,strMbarSl);   
			// 			m_ListCtrl.SetItemText(nItem,6,strSbarSl);	

			m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
} 

void CDgnCompositeMatDlg::DgnConcmatModifySlab(int nItem)
{

	CString strID=_T(""), strName=_T(""), strCode=_T(""), strRebarCode=_T("");
	CString strFc=_T(""), strChk=_T(""), strLambda=_T(""), strMain=_T("");
	CString strSub =_T(""), strMatName=_T("");

	if(ErrorCheckSlab())
	{
		strID = m_ListCtrl.GetItemText(nItem,0);
		// Modify Material ID
		int nID = _ttoi(strID);
		T_MATD_D mData;
		mData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);
		// Set_DlgData(mData);

		// Get changed Code Name.
		int Index   = m_cmbConCodeSl.GetCurSel();
		m_cmbConCodeSl.GetLBText(Index,strCode);							// Get Material Code.
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
		mData.Data2.CodeName = strCode;
		int reIndex = m_cmbRebarCodeSl.GetCurSel();
		m_cmbRebarCodeSl.GetLBText(reIndex,strRebarCode);	// Get Rebar Code.
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif
		mData.Rebar_CodeName2 = strRebarCode;

		if(Index != 0)	// if Code exists.
		{
			Index = m_cmbConMatNameSl.GetCurSel();
			if(Index != -1) m_cmbConMatNameSl.GetLBText(Index,strMatName);

#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strMatName);
#endif

			mData.Data2.CodeMatlName = strMatName;	// Get Material Code Name.

			// 			strName	= strMatName; //MQC4581-SALEE
			// 			mData.Name = strName;
			// Get Analysis Data & Design Data.
			T_MATL_CONCRETE cData;
			BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode,strMatName,cData);
			if(bCheck)
			{
				mData.Data2.Design.C_fc	 = m_edtFcSl.GetEditValue();
				mData.Data2.Design.C_fci = 0.7*m_edtFcSl.GetEditValue();	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
				strFc.Format(_T("%g"),mData.Data2.Design.C_fc);

				mData.Data2.Design.bLambda = m_chkLambdaSl.GetCheck();
				mData.Data2.Design.dLambda = m_edtLambdaSl.GetEditValue();

				if(m_chkLambdaSl.GetCheck()) 
					strChk = _T("O");
				else            
					strChk = _T("X");

				strLambda.Format(_T("%g"), mData.Data2.Design.dLambda);
			}
		}
		else	// if Code not exists.
		{
			mData.Data2.CodeMatlName	= strCode;

			//       strName = m_MatName2; //MQC4581-SALEE
			//       mData.Name = strName;

			mData.Data2.Design.C_fc	 = m_edtFcSl.GetEditValue();
			mData.Data2.Design.C_fci = 0.7*m_edtFcSl.GetEditValue();	// Add by ZINU.('04.09.09). Always fci=0.7*fc.
			strFc.Format(_T("%g"),mData.Data2.Design.C_fc);

			mData.Data2.Design.bLambda = m_chkLambdaSl.GetCheck();
			mData.Data2.Design.dLambda = m_edtLambdaSl.GetEditValue();

			if(m_chkLambdaSl.GetCheck()) 
				strChk = _T("O");
			else            
				strChk = _T("X");

			strLambda.Format(_T("%g"), mData.Data2.Design.dLambda);
		}

		if(reIndex != 0)	// if Code exists.
		{
			// Rebar Data를 얻는다.
			T_MATL_REBAR MainData,SubData;
			Index = m_cmbMainBarNamSl.GetCurSel();
			if(Index != -1) m_cmbMainBarNamSl.GetLBText(Index,strMain);
			mData.MainRebar_RebarName2 = strMain;
			MainData.Initialize();
			bCheck = m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strMain,MainData);
			mData.MainRebarData2.B_Density = MainData.B_Density;
			mData.MainRebarData2.B_Elast   = MainData.B_Elast;
			mData.MainRebarData2.B_fy      = MainData.B_fy;

			Index = m_cmbSubBarNameSl.GetCurSel();
			if(Index != -1) m_cmbSubBarNameSl.GetLBText(Index,strSub);
			mData.SubRebar_RebarName2 = strSub;
			SubData.Initialize();
			bCheck = m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strSub,SubData);
			mData.SubRebarData2.B_Density = SubData.B_Density;
			mData.SubRebarData2.B_Elast   = SubData.B_Elast;
			mData.SubRebarData2.B_fy      = SubData.B_fy;
		}
		else	// if Code not exists.
		{
			mData.Rebar_CodeName2			= strRebarCode;  //  Rebar Code를 얻는다.

			CString srtMainBarNameSl;   m_edtMainBarNameSl.GetWindowText(srtMainBarNameSl);
			CString srtSubBarNameSl;   m_edtSubBarNameSl.GetWindowText(srtSubBarNameSl);
			strMain = srtMainBarNameSl;
			strSub  = srtSubBarNameSl;

			mData.MainRebar_RebarName2     = srtMainBarNameSl;
			mData.MainRebarData2.B_Density = 0.0;
			mData.MainRebarData2.B_Elast   = 0.0;
			mData.MainRebarData2.B_fy      = m_edtFySl.GetEditValue();

			mData.SubRebar_RebarName2      = srtSubBarNameSl;
			mData.SubRebarData2.B_Density  = 0.0;
			mData.SubRebarData2.B_Elast    = 0.0;
			mData.SubRebarData2.B_fy       = m_edtFysSl.GetEditValue();
		}

		strName = mData.Name; //MQC4581-SALEE
		bCheck = m_pDoc->m_pDataCtrl->AddMatd(nID,mData);
		if(bCheck)
		{
			CString strConNameSl = mData.Data2.CodeMatlName;
			CString strMbarSl	   = mData.MainRebar_RebarName2;
			CString strSbarSl	   = mData.SubRebar_RebarName2;

			m_ListCtrl.SetItemText(nItem,5,strConNameSl);
			m_ListCtrl.SetItemText(nItem,6,strMain);   //strMbarSl
			m_ListCtrl.SetItemText(nItem,7,strSub);	   //strSbarSl

			m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
} 



BOOL CDgnCompositeMatDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	// 거더
	if(m_cmbConCodeGr.GetCurSel()==0)	
	{
		CString strMatNameGr; m_edtMatNameGr.GetWindowText(strMatNameGr);
		if(strMatNameGr ==_T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_CON_MATL_NAME_ERROR);}
	}
	if(m_cmbRebarCodeGr.GetCurSel()==0)
	{
		//     if(m_MainBarName2==_T("") || m_SubBarName2==_T(""))
		//     {
		// 		  bCheck = FALSE;
		// 		  strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
		//     }
	}

	double dLambda = m_edtLambdaGr.GetEditValue();
	if(m_chkLambdaGr.GetCheck())
	{
		if(dLambda < 0.75 || dLambda > 1.0) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_LAMBDA_ERROR);}
	}
	
	double dFcGr  = m_edtFcGr.GetEditValue();
	double dFyGr  = m_edtFyGr.GetEditValue();
	double dFysGr = m_edtFysGr.GetEditValue();
	double dTracsferFciGr = m_edtTransferGr.GetEditValue();

	if(dFcGr <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FC_ERROR);}
	if(dFyGr <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
	if(dFysGr<= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FYS_ERROR);}
	if(dTracsferFciGr< 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FCI_ERROR);}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

BOOL CDgnCompositeMatDlg::ErrorCheckSlab()
{
	BOOL bCheck = TRUE;
	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);

	// 슬래브
	if(m_cmbConCodeSl.GetCurSel()==0)	
	{
		CString strMatNameSl; m_edtMatNameSl.GetWindowText(strMatNameSl);
		if(strMatNameSl==_T("")) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_CON_MATL_NAME_ERROR);}
	}
	if(m_cmbRebarCodeSl.GetCurSel()==0)
	{
		//     if(m_MainBarName2==_T("") || m_SubBarName2==_T(""))
		//     {
		// 		  bCheck = FALSE;
		// 		  strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_STL_MATL_NAME_ERROR);
		//     }
	}

	double dLambda = m_edtLambdaSl.GetEditValue();
	if(m_chkLambdaSl.GetCheck())
	{
		if(dLambda < 0.75 || dLambda > 1.0) {bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_LAMBDA_ERROR);}
	}

	double dFcSl  = m_edtFcSl.GetEditValue();
	double dFySl  = m_edtFySl.GetEditValue();
	double dFysSl = m_edtFysSl.GetEditValue();
	if(dFcSl <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FC_ERROR);}
	if(dFySl <= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FY_ERROR);}
	if(dFysSl<= 0.0)	{bCheck = FALSE;	strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_MATL_FYS_ERROR);}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}



//////////////////////////////////////////////////////////////////////////
BOOL CDgnCompositeMatDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	AlignControl();
	InitCtrls();
	Init_Unit();
	Data2Dlg();

	return TRUE;
}

void CDgnCompositeMatDlg::OnClickDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//OnItemchangedDgnConcMatList(pNMHDR,pResult);
	//*pResult = 0;
}

void CDgnCompositeMatDlg::OnItemchangedDgnConcMatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nItem=0;
	if(!GetSelectedItem(nItem)) return; 

	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	CString strID = m_ListCtrl.GetItemText(nItem,0);
	
	m_Data.Initialize();//T_MATD_D mData; mData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(_ttoi(strID), m_Data))	return;
	// Set_DlgData(m_Data);

	m_strCurrentItem = strID;

	//**********************************
	// 거더
	//**********************************
	// If Code is None, Material Name is mData.Name.
	m_edtMatNameGr.SetWindowText(m_Data.Name);
	m_edtFcGr.SetEditUnit(m_Data.Data1.Design.C_fc);
	m_chkTransferGr.SetCheck(m_Data.Data1.Design.bTransfer);
	m_chkLambdaGr.SetCheck(m_Data.Data1.Design.bLambda);
	m_edtLambdaGr.SetEditUnit(m_Data.Data1.Design.dLambda);
	m_edtTransferGr.SetEditUnit(m_Data.Data1.Design.dTransferFci);
	m_edtFyGr.SetEditUnit(m_Data.MainRebarData.B_fy);
	m_edtFysGr.SetEditUnit(m_Data.SubRebarData.B_fy);
	m_edtMainBarNameGr.SetWindowText(m_Data.MainRebar_RebarName);
	m_edtSubBarNameGr.SetWindowText(m_Data.SubRebar_RebarName);
	Init_CodeCombo();

//   OnSelchangeDgnConcMatCode();
//   OnSelchangeDgnConcRebarCode();
	OnClickLambdaChk();
	OnClickTransferChk();

	//**********************************
	// 슬래브
	//**********************************
	// If Code is None, Material Name is mData.Name.
	m_edtMatNameSl.SetWindowText(m_Data.Name);
	m_edtFcSl.SetEditUnit(m_Data.Data2.Design.C_fc);                
	m_chkLambdaSl.SetCheck(m_Data.Data2.Design.bLambda);
	m_edtLambdaSl.SetEditUnit(m_Data.Data2.Design.dLambda);
	m_edtFySl.SetEditUnit(m_Data.MainRebarData2.B_fy);              
	m_edtFysSl.SetEditUnit(m_Data.SubRebarData2.B_fy);              
	m_edtMainBarNameSl.SetWindowText(m_Data.MainRebar_RebarName2);
	m_edtSubBarNameSl.SetWindowText(m_Data.SubRebar_RebarName2);
	Init_CodeComboSlab();  

//   OnSelchangeDgnConcMatCodeSlab();
//   OnSelchangeDgnConcRebarCodeSlab();
	OnClickLambdaChkSlab();

	//UpdateData(FALSE);

	*pResult = 0;
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcMatCode() 
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int iIndex = m_cmbConCodeGr.GetCurSel();
	CString strCode=_T("");
	if(iIndex != -1)	m_cmbConCodeGr.GetLBText(iIndex, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	if(strCode==_T("None") || strCode==_T(""))	EnableEditBox(TRUE);
	else
	{
		EnableEditBox(FALSE);
		// Change Material Name by Code.
		m_cmbConMatNameGr.ResetContent();
		CArray<CString, CString&> arMatlName;
		m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arMatlName);
		for (int i = 0; i < arMatlName.GetSize(); i++)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arMatlName[i]);
#endif
			m_cmbConMatNameGr.AddString(arMatlName[i]);
		}
	}
	if(strCode!=_T("None"))	m_edtFcGr.SetEditUnit(0.0);
	m_edtMatNameGr.SetWindowText(_T(""));

	ChangeDlgItemInfo();

	OnSelchangeDgnConcMatName();

	//UpdateData(FALSE);
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcRebarCode() 
{
	CString strCode=_T("");
	int Index = m_cmbRebarCodeGr.GetCurSel();
	if(Index != -1)	m_cmbRebarCodeGr.GetLBText(Index, strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	if(strCode==_T("None") || strCode==_T(""))	EnableRebarEditBox(TRUE);
	else
	{
		EnableRebarEditBox(FALSE);
		m_cmbMainBarNamGr.ResetContent();
		m_cmbSubBarNameGr.ResetContent();

		CArray<CString, CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arRebar);
		for(int i=0; i<arRebar.GetSize(); i++)  
		{
			m_cmbMainBarNamGr.AddString(arRebar[i]);
			m_cmbSubBarNameGr.AddString(arRebar[i]);
		}	
	}

	if(strCode!=_T("None"))
	{
		m_edtFyGr.SetEditUnit(0.0);
		m_edtFysGr.SetEditUnit(0.0);
	}
	m_edtMainBarNameGr.SetWindowText(_T("")); // m_MainBarNameGr =_T("");
	m_edtSubBarNameGr.SetWindowText(_T(""));  // m_SubBarNameGr =_T("");

	ChangeDlgItemInfo();

	OnSelchangeDgnConcRebarMainName();
	OnSelchangeDgnConcRebarSubName();

	//UpdateData(FALSE);
}


void CDgnCompositeMatDlg::OnSelchangeDgnConcMatCodeSlab() 
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int iIndex = m_cmbConCodeSl.GetCurSel();
	CString strCode=_T("");
	if(iIndex != -1)	m_cmbConCodeSl.GetLBText(iIndex, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if(strCode==_T("None") || strCode==_T(""))	EnableEditBoxSlab(TRUE);
	else
	{
		EnableEditBoxSlab(FALSE);
		// Change Material Name by Code.
		m_cmbConMatNameSl.ResetContent();
		CArray<CString, CString&> arMatlName;
		m_pDoc->m_pMatlDB->GetConcreteNameList(strCode, arMatlName);
		for (int i = 0; i < arMatlName.GetSize(); i++)
		{
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(strCode, arMatlName[i]);
#endif
			m_cmbConMatNameSl.AddString(arMatlName[i]);
		}
	}
	if(strCode!=_T("None"))	m_edtFcSl.SetEditUnit(0.0);
	m_edtMatNameSl.SetWindowText(_T(""));


	ChangeDlgItemInfoSlab();

	OnSelchangeDgnConcMatNameSlab();

	//UpdateData(FALSE);
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcRebarCodeSlab() 
{
	CString strCode=_T("");
	int Index = m_cmbRebarCodeSl.GetCurSel();
	if(Index != -1)	m_cmbRebarCodeSl.GetLBText(Index, strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	if(strCode==_T("None") || strCode==_T(""))	EnableRebarEditBoxSlab(TRUE);
	else
	{
		EnableRebarEditBoxSlab(FALSE);
		m_cmbMainBarNamSl.ResetContent();
		m_cmbSubBarNameSl.ResetContent();

		CArray<CString, CString&> arRebar;
		m_pDoc->m_pMatlDB->GetRebarNameList(strCode, arRebar);
		for(int i=0; i<arRebar.GetSize(); i++)  
		{
			m_cmbMainBarNamSl.AddString(arRebar[i]);
			m_cmbSubBarNameSl.AddString(arRebar[i]);
		}	
	}

	if(strCode!=_T("None"))
	{
		m_edtFySl.SetEditUnit(0.0);
		m_edtFysSl.SetEditUnit(0.0);
	}
	m_edtMainBarNameSl.SetWindowText(_T(""));
	m_edtSubBarNameSl.SetWindowText(_T(""));

	ChangeDlgItemInfoSlab();

	OnSelchangeDgnConcRebarMainNameSlab();
	OnSelchangeDgnConcRebarSubNameSlab();

	UpdateData(FALSE);
}


void CDgnCompositeMatDlg::OnSelchangeDgnConcMatName() 
{
	int Index = m_cmbConCodeGr.GetCurSel();
	CString strCode=_T("");
	if(Index != -1) m_cmbConCodeGr.GetLBText(Index,strCode);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif

	Index = m_cmbConMatNameGr.GetCurSel();
	CString strName=_T("");
	if(Index != -1) m_cmbConMatNameGr.GetLBText(Index, strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode, strName, cData);
	if(bCheck)
	{
		m_edtFcGr.SetEditUnit(cData.C_fc);
	}
	UpdateData(FALSE);
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcMatNameSlab() 
{
	int Index = m_cmbConCodeSl.GetCurSel();
	CString strCode=_T("");
	if(Index != -1) m_cmbConCodeSl.GetLBText(Index,strCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strCode);
#endif
	Index = m_cmbConMatNameSl.GetCurSel();
	CString strName=_T("");
	if(Index != -1) m_cmbConMatNameSl.GetLBText(Index, strName);

#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatlDBName(strCode, strName);
#endif

	T_MATL_CONCRETE cData;
	BOOL bCheck = m_pDoc->m_pMatlDB->GetConcreteData(strCode, strName, cData);
	if(bCheck)
	{
		m_edtFcSl.SetEditUnit(cData.C_fc);
	}
	
	UpdateData(FALSE);
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcRebarMainName() 
{
	CString strRebarName=_T(""), strRebarCode=_T("");
	int Index = m_cmbMainBarNamGr.GetCurSel();
	if(Index != -1)
	{
		//UpdateData(TRUE);
		m_cmbMainBarNamGr.GetLBText(Index,strRebarName);

		Index = m_cmbRebarCodeGr.GetCurSel();
		if(Index != -1) m_cmbRebarCodeGr.GetLBText(Index,strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE),MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if(m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,rData))
		{
			m_edtFyGr.SetEditUnit(rData.B_fy);
		}
		
		UpdateData(FALSE);
	}
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcRebarMainNameSlab() 
{
	CString strRebarName=_T(""), strRebarCode=_T("");
	int Index = m_cmbMainBarNamSl.GetCurSel();
	if(Index != -1)
	{
		//UpdateData(TRUE);
		m_cmbMainBarNamSl.GetLBText(Index,strRebarName);

		Index = m_cmbRebarCodeSl.GetCurSel();
		if(Index != -1) m_cmbRebarCodeSl.GetLBText(Index,strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE),MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if(m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,rData))
		{
			m_edtFySl.SetEditUnit(rData.B_fy);
		}

		UpdateData(FALSE);
	}
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcRebarSubName() 
{
	CString strRebarName=_T(""), strRebarCode=_T("");
	int Index = m_cmbSubBarNameGr.GetCurSel();
	if(Index != -1)
	{
		//UpdateData(TRUE);
		m_cmbSubBarNameGr.GetLBText(Index,strRebarName);

		Index = m_cmbRebarCodeGr.GetCurSel();
		if(Index != -1) m_cmbRebarCodeGr.GetLBText(Index,strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE),MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if(m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,rData))
		{
			m_edtFysGr.SetEditUnit(rData.B_fy);
		}
	 
		UpdateData(FALSE);
	}
}

void CDgnCompositeMatDlg::OnSelchangeDgnConcRebarSubNameSlab() 
{
	CString strRebarName=_T(""), strRebarCode=_T("");
	int Index = m_cmbSubBarNameSl.GetCurSel();
	if(Index != -1)
	{
		//UpdateData(TRUE);
		m_cmbSubBarNameSl.GetLBText(Index,strRebarName);

		Index = m_cmbRebarCodeSl.GetCurSel();
		if(Index != -1) m_cmbRebarCodeSl.GetLBText(Index,strRebarCode);
		else            AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_CODE),MB_OK);

#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

		T_MATL_REBAR rData;
		rData.Initialize();
		if(m_pDoc->m_pMatlDB->GetRebarData(strRebarCode,strRebarName,rData))
		{
			m_edtFysSl.SetEditUnit(rData.B_fy);
		}
		
		UpdateData(FALSE);
	}
}

void CDgnCompositeMatDlg::OnClickLambdaChk()
{
 // UpdateData(TRUE);
	BOOL bChk = m_chkLambdaGr.GetCheck();
	GetDlgItem(IDC_DGN_LAMBDA_GR_EDT)->EnableWindow(bChk);
}
void CDgnCompositeMatDlg::OnClickTransferChk()
{
	BOOL bChk = m_chkTransferGr.GetCheck();
	GetDlgItem(IDC_DGN_TRANSFER_GR_EDT)->EnableWindow(bChk);
}
void CDgnCompositeMatDlg::OnClickLambdaChkSlab()
{
	//UpdateData(TRUE);
	BOOL bChk = m_chkLambdaSl.GetCheck();
	GetDlgItem(IDC_DGN_LAMBDA_SL_EDT)->EnableWindow(bChk);
}


// Coded by Seungjun MNet:No.2393 ('20061110)
void CDgnCompositeMatDlg::OnDgnConcElasticity()
{
	CDgnConMatSLTEDlg dlg;

	dlg.m_iMatdKey = _ttoi(m_strCurrentItem);
	dlg.DoModal();
}

void CDgnCompositeMatDlg::OnDgnConcElasticitySlab()
{
	CDgnConMatSLTEDlg dlg;

	dlg.m_iMatdKey = _ttoi(m_strCurrentItem);
	dlg.DoModal();
}

void CDgnCompositeMatDlg::OnDgnConcmatModify() 
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}

	UpdateData(TRUE);

	DgnConcmatModify(nItem);      // 거더
	DgnConcmatModifySlab(nItem);  // 슬래브

	UpdateData(FALSE);

}

void CDgnCompositeMatDlg::OnDgnConcmatClose() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}







