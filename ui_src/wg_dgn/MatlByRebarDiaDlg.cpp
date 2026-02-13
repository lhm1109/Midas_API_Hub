// MatlByRebarDiaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "MatlByRebarDiaDlg.h"
#include <limits>

#include "MatlByRebarDiaGrid.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\DBCodeDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseDlg dialog


CMatlByRebarDiaDlg::CMatlByRebarDiaDlg(T_DCRB_D* pData, CWnd* pParent /*=NULL*/)
	: CDialogMove(CMatlByRebarDiaDlg::IDD, pParent)
{
	m_wndVertGrid = new CMatlByRebarDiaGrid;
	m_wndHorzGrid = new CMatlByRebarDiaGrid;
	m_pDoc = CDBDoc::GetDocPoint();
	m_pMatlDB = m_pDoc->m_pMatlDB;
	m_pData = pData;
	m_pBufData = new T_DCRB_D();
	*m_pBufData = *m_pData;
	//{{AFX_DATA_INIT(CCMLoadCaseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMatlByRebarDiaDlg::~CMatlByRebarDiaDlg()
{
	if (m_wndVertGrid) delete m_wndVertGrid;
	if (m_wndHorzGrid) delete m_wndHorzGrid;
	if (m_pBufData) delete m_pBufData;
}

void CMatlByRebarDiaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMLoadCaseDlg)
	DDX_Control(pDX, IDC_CON_BAR_MATL_CODE_CMB, m_cmbConcMatCode);

	DDX_Control(pDX, IDC_CON_BAR_VERT_FROM_CMB, m_cmdVertFromCmb);
	DDX_Control(pDX, IDC_CON_BAR_VERT_TO___CMB, m_cmdVertToedCmb);
	DDX_Control(pDX, IDC_CON_BAR_VERT_MATL_CMB, m_cmdVertMatlCmb);

	DDX_Control(pDX, IDC_CON_BAR_HORZ_FROM_CMB, m_cmdHorzFromCmb);
	DDX_Control(pDX, IDC_CON_BAR_HORZ_TO___CMB, m_cmdHorzToedCmb);
	DDX_Control(pDX, IDC_CON_BAR_HORZ_MATL_CMB, m_cmdHorzMatlCmb);

	DDX_Control(pDX, IDC_CON_BAR_MATL_VERT_GRID, *m_wndVertGrid);
	DDX_Control(pDX, IDC_CON_BAR_MATL_HORZ_GRID, *m_wndHorzGrid);

	//}}AFX_DATA_MAP
}

void CMatlByRebarDiaDlg::InitCodeCombo()
{
	CArray<CString, CString&> MatCodeList;	
	//MQC 4752-5 Code List에 KS만 보이도록 요청받음
    MatCodeList.Add(CString(MATLCODE_CON_KS19));
	MatCodeList.Add(CString(MATLCODE_CON_KS01));
	MatCodeList.Add(CString(MATLCODE_CON_KS));
	MatCodeList.InsertAt(0, T_DCRB_D::GetNoneCode());
	
	//m_cmbPGDesignCode.AddString(_T("EN 1994-2"));
	
	const int nMatlCode = MatCodeList.GetSize();
	for( int nMatlCode_i = 0; nMatlCode_i<nMatlCode; nMatlCode_i++)
	{
		m_cmbConcMatCode.AddString(MatCodeList[nMatlCode_i]);
	}

	m_cmbConcMatCode.SetCurSel(0);
}

void CMatlByRebarDiaDlg::InitDiaCombo()
{
	CDBLib::GetRebarNameAtComboBox(&m_cmdVertFromCmb);
	m_cmdVertFromCmb.InsertString(0, T_MATLRBDIA_D::GetSmallBoundDia());
	m_cmdVertFromCmb.SetCurSel(0);
	CDBLib::GetRebarNameAtComboBox(&m_cmdVertToedCmb);
	m_cmdVertToedCmb.AddString(T_MATLRBDIA_D::GetBigBoundDia());
	m_cmdVertToedCmb.SetCurSel(0);

	CDBLib::GetRebarNameAtComboBox(&m_cmdHorzFromCmb);
	m_cmdHorzFromCmb.InsertString(0, T_MATLRBDIA_D::GetSmallBoundDia());
	m_cmdHorzFromCmb.SetCurSel(0);
	CDBLib::GetRebarNameAtComboBox(&m_cmdHorzToedCmb);
	m_cmdHorzToedCmb.AddString(T_MATLRBDIA_D::GetBigBoundDia());
	m_cmdHorzToedCmb.SetCurSel(0);
}

void CMatlByRebarDiaDlg::InitMatlCombo()
{
	CString strCode = _T("");
	if(!GetSelectedString(&m_cmbConcMatCode, &strCode)){ ASSERT(FALSE); return; }
	CArray<CString, CString&> arRebar; arRebar.RemoveAll();
	m_pMatlDB->GetRebarNameList(strCode, arRebar);
	
	arRebar.InsertAt(0, T_MATLRBDIA_D::GetNoneMatl());

	m_cmdVertMatlCmb.ResetContent();
	m_cmdHorzMatlCmb.ResetContent();
	const int nRbarSize = arRebar.GetSize();
	for(int nRbar_i = 0; nRbar_i < nRbarSize; ++nRbar_i)
	{
		m_cmdVertMatlCmb.AddString(arRebar[nRbar_i]);
		m_cmdHorzMatlCmb.AddString(arRebar[nRbar_i]);
	}

	m_cmdVertMatlCmb.SetCurSel(0);
	m_cmdHorzMatlCmb.SetCurSel(0);
}

BOOL CMatlByRebarDiaDlg::Data2Dlg()
{
	const int nCodeCount = m_cmbConcMatCode.GetCount();
	BOOL bExist = FALSE;
	for(int nCode_i = 0; nCode_i < nCodeCount && bExist == FALSE; ++nCode_i)
	{
		CString strCurLB = _T("");
		m_cmbConcMatCode.GetLBText(nCode_i, strCurLB);
		if(m_pBufData->strMatlCode.CompareNoCase(strCurLB) == 0)
		{
			m_cmbConcMatCode.SetCurSel(nCode_i);
			bExist = TRUE;
		}
	}

	m_wndVertGrid->Data2Grid(m_pBufData);
	m_wndHorzGrid->Data2Grid(m_pBufData);

	return TRUE;
}

BOOL CMatlByRebarDiaDlg::Dlg2Data()
{
	GetSelectedString(&m_cmbConcMatCode, &m_pBufData->strMatlCode);
	
	if(!m_wndVertGrid->Grid2Data(m_pBufData)) return FALSE;
	if(!m_wndHorzGrid->Grid2Data(m_pBufData)) return FALSE;
	
	*m_pData = *m_pBufData;
	return TRUE;
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CMatlByRebarDiaDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CCMLoadCaseDlg)
	ON_CBN_SELCHANGE(IDC_CON_BAR_MATL_CODE_CMB, OnCodeCmb)
	ON_CBN_SELCHANGE(IDC_CON_BAR_VERT_FROM_CMB, OnVertFromCmb)
	ON_CBN_SELCHANGE(IDC_CON_BAR_VERT_TO___CMB, OnVertToedCmb)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_VERT_FRTO_BTN, OnVertAddRplBtn)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_VERT_ADD_BTN, OnVertAddBtn)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_VERT_INS_BTN, OnVertInsertBtn)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_VERT_DEL_BTN, OnVertDelBtn)

	ON_CBN_SELCHANGE(IDC_CON_BAR_HORZ_FROM_CMB, OnHorzFromCmb)
	ON_CBN_SELCHANGE(IDC_CON_BAR_HORZ_TO___CMB, OnHorzToedCmb)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_HORZ_FRTO_BTN, OnHorzAddRplBtn)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_HORZ_ADD_BTN, OnHorzAddBtn)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_HORZ_INS_BTN, OnHorzInsertBtn)
	ON_BN_CLICKED(IDC_CON_BAR_MATL_HORZ_DEL_BTN, OnHorzDelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseDlg message handlers

BOOL CMatlByRebarDiaDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	InitCodeCombo();

	ASSERT(m_pDoc);
	// TODO: Add extra initialization here
	m_wndVertGrid->Initialize(this, m_pBufData, CMatlByRebarDiaGrid::VERT_RABAR);

	m_wndHorzGrid->Initialize(this, m_pBufData, CMatlByRebarDiaGrid::HORZ_REBAR);
	
	Data2Dlg();
	InitDiaCombo();
	InitMatlCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// BOOL CMatlByRebarDia::DestroyWindow() 
// {
// 	// TODO: Add your specialized code here and/or call the base class
//   m_wndGrid->OnTerminate(D_TB_SAVE_ALWAYS);
// 	return CCMDlgBase::DestroyWindow();
// }

// void CMatlByRebarDia::OnCmdLcdDelete() 
// {
// 	// TODO: Add your control notification handler code here
//   CRowColArray awRows;
//   m_wndGrid->GetSelectedRows(awRows, TRUE, FALSE);
//   int nSize = awRows.GetSize();
// 	if (nSize > 0)
//   {
//     m_wndGrid->DeleteFromDB(awRows);
//     return;
//   }
//   ROWCOL nRow, nCol;
//   m_wndGrid->GetCurrentCell(nRow, nCol);
//   if (m_wndGrid->IsAppendRow(nRow)) 
//   {
//     AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_static_load_c));
//     return;
//   }
//   m_wndGrid->DeleteFromDB(nRow);
// }

void CMatlByRebarDiaDlg::OnCodeCmb()
{
	CString strCurCode = _T("");
	GetSelectedString(&m_cmbConcMatCode, &strCurCode);
	BOOL bDiffrent = (m_pBufData->strMatlCode.CompareNoCase(strCurCode) != 0);

	BOOL bEnable = (strCurCode != T_DCRB_D::GetNoneCode());
	m_cmdVertFromCmb.EnableWindow(bEnable);
	m_cmdVertToedCmb.EnableWindow(bEnable);
	m_cmdVertMatlCmb.EnableWindow(bEnable);
	GetDlgItem(IDC_CON_BAR_MATL_VERT_FRTO_BTN)->EnableWindow(bEnable);

	m_cmdHorzFromCmb.EnableWindow(bEnable);
	m_cmdHorzToedCmb.EnableWindow(bEnable);
	m_cmdHorzMatlCmb.EnableWindow(bEnable);
	GetDlgItem(IDC_CON_BAR_MATL_HORZ_FRTO_BTN)->EnableWindow(bEnable);
	
	if(bDiffrent && bEnable)
	{
		//InitDiaCombo();
		InitMatlCombo();
		m_pBufData->strMatlCode = strCurCode;
		m_pBufData->aVertMatlByDia.RemoveAll();
		m_pBufData->aHorzMatlByDia.RemoveAll();
		m_wndVertGrid->Data2Grid(m_pBufData);
		m_wndHorzGrid->Data2Grid(m_pBufData);
	}
}

void CMatlByRebarDiaDlg::OnVertFromCmb()
{
	CString strFromDia = _T("");
	if(!GetSelectedString(&m_cmdVertFromCmb, &strFromDia)){ ASSERT(FALSE); return; }
	
	CString strToDia = _T("");
	if(!GetSelectedString(&m_cmdVertToedCmb, &strToDia)){ ASSERT(FALSE); return; }

	int nFromIndxOnToCmb = 0;
	if(strFromDia.CompareNoCase(T_MATLRBDIA_D::GetSmallBoundDia()) != 0)
	{
		nFromIndxOnToCmb = m_cmdVertToedCmb.FindStringExact(-1, strFromDia);
	}

	const int nToedCmbSize = m_cmdVertToedCmb.GetCount();
	int nToIndx = nToedCmbSize - 1;
	if(nToIndx < 0){ ASSERT(FALSE); return; }
	if(strToDia.CompareNoCase(T_MATLRBDIA_D::GetBigBoundDia()) != 0)
	{
		nToIndx = m_cmdVertToedCmb.FindStringExact(-1, strToDia);
	}
	
	if(nFromIndxOnToCmb > nToIndx)
	{
		m_cmdVertToedCmb.SetCurSel(nFromIndxOnToCmb);
	}
}

void CMatlByRebarDiaDlg::OnVertToedCmb()
{
	CString strFromDia = _T("");
	if(!GetSelectedString(&m_cmdVertFromCmb, &strFromDia)){ ASSERT(FALSE); return; }
	
	CString strToDia = _T("");
	if(!GetSelectedString(&m_cmdVertToedCmb, &strToDia)){ ASSERT(FALSE); return; }

	int nFromIndxOnToCmb = 0;
	if(strFromDia.CompareNoCase(T_MATLRBDIA_D::GetSmallBoundDia()) != 0)
	{
		nFromIndxOnToCmb = m_cmdVertToedCmb.FindStringExact(-1, strFromDia);
	}
	
	const int nToedCmbSize = m_cmdVertToedCmb.GetCount();
	int nToIndx = nToedCmbSize - 1;
	if(nToIndx < 0){ ASSERT(FALSE); return; }
	if(strToDia.CompareNoCase(T_MATLRBDIA_D::GetBigBoundDia()) != 0)
	{
		nToIndx = m_cmdVertToedCmb.FindStringExact(-1, strToDia);
	}
	
	if(nFromIndxOnToCmb > nToIndx)
	{
		m_cmdVertFromCmb.SetCurSel(m_cmdVertFromCmb.FindStringExact(-1, strToDia));
	}
}

void CMatlByRebarDiaDlg::OnHorzFromCmb()
{
	CString strFromDia = _T("");
	if(!GetSelectedString(&m_cmdHorzFromCmb, &strFromDia)){ ASSERT(FALSE); return; }
	
	CString strToDia = _T("");
	if(!GetSelectedString(&m_cmdHorzToedCmb, &strToDia)){ ASSERT(FALSE); return; }
	
	int nFromIndxOnToCmb = 0;
	if(strFromDia.CompareNoCase(T_MATLRBDIA_D::GetSmallBoundDia()) != 0)
	{
		nFromIndxOnToCmb = m_cmdHorzToedCmb.FindStringExact(-1, strFromDia);
	}
	
	const int nToedCmbSize = m_cmdHorzToedCmb.GetCount();
	int nToIndx = nToedCmbSize - 1;
	if(nToIndx < 0){ ASSERT(FALSE); return; }
	if(strToDia.CompareNoCase(T_MATLRBDIA_D::GetBigBoundDia()) != 0)
	{
		nToIndx = m_cmdHorzToedCmb.FindStringExact(-1, strToDia);
	}
	
	if(nFromIndxOnToCmb > nToIndx)
	{
		m_cmdHorzToedCmb.SetCurSel(nFromIndxOnToCmb);
	}
}

void CMatlByRebarDiaDlg::OnHorzToedCmb()
{
	CString strFromDia = _T("");
	if(!GetSelectedString(&m_cmdHorzFromCmb, &strFromDia)){ ASSERT(FALSE); return; }
	
	CString strToDia = _T("");
	if(!GetSelectedString(&m_cmdHorzToedCmb, &strToDia)){ ASSERT(FALSE); return; }
	
	int nFromIndxOnToCmb = 0;
	if(strFromDia.CompareNoCase(T_MATLRBDIA_D::GetSmallBoundDia()) != 0)
	{
		nFromIndxOnToCmb = m_cmdHorzToedCmb.FindStringExact(-1, strFromDia);
	}
	
	const int nToedCmbSize = m_cmdHorzToedCmb.GetCount();
	int nToIndx = nToedCmbSize - 1;
	if(nToIndx < 0){ ASSERT(FALSE); return; }
	if(strToDia.CompareNoCase(T_MATLRBDIA_D::GetBigBoundDia()) != 0)
	{
		nToIndx = m_cmdHorzToedCmb.FindStringExact(-1, strToDia);
	}
	
	if(nFromIndxOnToCmb > nToIndx)
	{
		m_cmdHorzFromCmb.SetCurSel(m_cmdHorzFromCmb.FindStringExact(-1, strToDia));
	}
}

void CMatlByRebarDiaDlg::OnVertAddRplBtn()
{
	CString strFromDia = _T("");
	if(!GetSelectedString(&m_cmdVertFromCmb, &strFromDia)){ ASSERT(FALSE); return; }

	CString strToDia = _T("");
	if(!GetSelectedString(&m_cmdVertToedCmb, &strToDia)){ ASSERT(FALSE); return; }

	int nFromIndxOnToCmb = 0;
	if(strFromDia.CompareNoCase(T_MATLRBDIA_D::GetSmallBoundDia()) != 0)
	{
		nFromIndxOnToCmb = m_cmdVertToedCmb.FindStringExact(-1, strFromDia);
	}

	const int nToedCmbSize = m_cmdVertToedCmb.GetCount();
	int nToIndx = nToedCmbSize - 1;
	if(nToIndx < 0){ ASSERT(FALSE); return; }
	if(strToDia.CompareNoCase(T_MATLRBDIA_D::GetBigBoundDia()) != 0)
	{
		nToIndx = m_cmdVertToedCmb.FindStringExact(-1, strToDia);
	}

	CString strCode = _T("");
	if(!GetSelectedString(&m_cmbConcMatCode, &strCode)){ ASSERT(FALSE); return; }

	CString strMatl = _T("");
	if(!GetSelectedString(&m_cmdVertMatlCmb, &strMatl)){ ASSERT(FALSE); return; }

	CString strMatlForCalcFy = _T("");
	if(strMatl == T_MATLRBDIA_D::GetNoneMatl())
	{
		CArray<CString, CString&> arRebar; arRebar.RemoveAll();
		m_pMatlDB->GetRebarNameList(strCode, arRebar);
		if(arRebar.GetSize() < 1){ ASSERT(FALSE); return; }

		strMatlForCalcFy = arRebar[0];
	}
	else
	{
		strMatlForCalcFy = strMatl;
	}

	T_MATL_REBAR MatlRebar; MatlRebar.Initialize();
	m_pMatlDB->GetRebarData(strCode, strMatlForCalcFy, MatlRebar);
	
	m_wndVertGrid->Grid2Data(m_pBufData);
	const int nVertSize = m_pBufData->aVertMatlByDia.GetSize();

	CMap<int, int, BOOL, BOOL> mapIndxExist; mapIndxExist.InitHashTable(nVertSize + 1);
	for(int i = 0; i < nVertSize; ++i)
	{
		int nToedCmbIndx = m_cmdVertToedCmb.FindStringExact(-1, m_pBufData->aVertMatlByDia[i].strDia);
		if(nFromIndxOnToCmb > nToedCmbIndx){ continue; }
		if(nToIndx < nToedCmbIndx){ continue; }
		m_pBufData->aVertMatlByDia[i].strMatl = strMatl;
		m_pBufData->aVertMatlByDia[i].dFy = MatlRebar.B_fy;
		
		mapIndxExist.SetAt(nToedCmbIndx, TRUE);
	}
	
	for(int k = nFromIndxOnToCmb; k < nToedCmbSize-1 && k <= nToIndx; ++k)
	{
		BOOL bMapRslt = FALSE;
		if(!mapIndxExist.Lookup(k, bMapRslt))
		{
			T_MATLRBDIA_D MatlRbar_Input; MatlRbar_Input.Initialize();
			m_cmdVertToedCmb.GetLBText(k, MatlRbar_Input.strDia);
			MatlRbar_Input.strMatl = strMatl;
			MatlRbar_Input.dFy = MatlRebar.B_fy;
			m_pBufData->aVertMatlByDia.Add(MatlRbar_Input);
		}
	}

	m_wndVertGrid->Data2Grid(m_pBufData);
}

void CMatlByRebarDiaDlg::OnVertAddBtn()
{
	m_wndVertGrid->AddRow(FALSE);
}

void CMatlByRebarDiaDlg::OnVertInsertBtn()
{
	m_wndVertGrid->AddRow(TRUE);
}

void CMatlByRebarDiaDlg::OnVertDelBtn()
{
	m_wndVertGrid->DelRow(TRUE);
}

void CMatlByRebarDiaDlg::OnHorzAddRplBtn()
{
	CString strFromDia = _T("");
	if(!GetSelectedString(&m_cmdHorzFromCmb, &strFromDia)){ ASSERT(FALSE); return; }
	
	CString strToDia = _T("");
	if(!GetSelectedString(&m_cmdHorzToedCmb, &strToDia)){ ASSERT(FALSE); return; }
	
	int nFromIndxOnToCmb = 0;
	if(strFromDia.CompareNoCase(T_MATLRBDIA_D::GetSmallBoundDia()) != 0)
	{
		nFromIndxOnToCmb = m_cmdHorzToedCmb.FindStringExact(-1, strFromDia);
	}
	
	const int nToedCmbSize = m_cmdHorzToedCmb.GetCount();
	int nToIndx = nToedCmbSize - 1;
	if(nToIndx < 0){ ASSERT(FALSE); return; }
	if(strToDia.CompareNoCase(T_MATLRBDIA_D::GetBigBoundDia()) != 0)
	{
		nToIndx = m_cmdHorzToedCmb.FindStringExact(-1, strToDia);
	}

	CString strCode = _T("");
	if(!GetSelectedString(&m_cmbConcMatCode, &strCode)){ ASSERT(FALSE); return; }
	
	CString strMatl = _T("");
	if(!GetSelectedString(&m_cmdHorzMatlCmb, &strMatl)){ ASSERT(FALSE); return; }
	
	CString strMatlForCalcFy = _T("");
	if(strMatl == T_MATLRBDIA_D::GetNoneMatl())
	{
		CArray<CString, CString&> arRebar; arRebar.RemoveAll();
		m_pMatlDB->GetRebarNameList(strCode, arRebar);
		if(arRebar.GetSize() < 1){ ASSERT(FALSE); return; }
		
		strMatlForCalcFy = arRebar[0];
	}
	else
	{
		strMatlForCalcFy = strMatl;
	}
	
	T_MATL_REBAR MatlRebar; MatlRebar.Initialize();
	m_pMatlDB->GetRebarData(strCode, strMatlForCalcFy, MatlRebar);
	
	m_wndHorzGrid->Grid2Data(m_pBufData);
	const int nHorzSize = m_pBufData->aHorzMatlByDia.GetSize();
	
	CMap<int, int, BOOL, BOOL> mapIndxExist; mapIndxExist.InitHashTable(nHorzSize + 1);
	for(int i = 0; i < nHorzSize; ++i)
	{
		int nToedCmbIndx = m_cmdHorzToedCmb.FindStringExact(-1, m_pBufData->aHorzMatlByDia[i].strDia);
		if(nFromIndxOnToCmb > nToedCmbIndx){ continue; }
		if(nToIndx < nToedCmbIndx){ continue; }
		m_pBufData->aHorzMatlByDia[i].strMatl = strMatl;
		m_pBufData->aHorzMatlByDia[i].dFy = MatlRebar.B_fy;
		mapIndxExist.SetAt(nToedCmbIndx, TRUE);
	}
	
	for(int k = nFromIndxOnToCmb; k < nToedCmbSize-1 && k <= nToIndx; ++k)
	{
		BOOL bMapRslt = FALSE;
		if(!mapIndxExist.Lookup(k, bMapRslt))
		{
			T_MATLRBDIA_D MatlRbar_Input; MatlRbar_Input.Initialize();
			m_cmdHorzToedCmb.GetLBText(k, MatlRbar_Input.strDia);
			MatlRbar_Input.strMatl = strMatl;
			MatlRbar_Input.dFy = MatlRebar.B_fy;
			m_pBufData->aHorzMatlByDia.Add(MatlRbar_Input);
		}
	}
	
	m_wndHorzGrid->Data2Grid(m_pBufData);
}

void CMatlByRebarDiaDlg::OnHorzAddBtn()
{
	m_wndHorzGrid->AddRow(FALSE);
}

void CMatlByRebarDiaDlg::OnHorzInsertBtn()
{
	m_wndHorzGrid->AddRow(TRUE);
}

void CMatlByRebarDiaDlg::OnHorzDelBtn()
{
	m_wndHorzGrid->DelRow(TRUE);
}

void CMatlByRebarDiaDlg::OnOK() 
{
	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}

BOOL CMatlByRebarDiaDlg::GetSelectedString(const CComboBox* pCombo, CString* const pStrCur)
{
	if(pCombo == NULL || pStrCur == NULL){ ASSERT(FALSE); return FALSE; }
	const int nIndx = pCombo->GetCurSel();
	if(nIndx < 0){ ASSERT(FALSE); return FALSE; }
	CString strSel = _T("");
	pCombo->GetLBText(nIndx, *pStrCur);
	return TRUE;
}