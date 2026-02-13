// DgnSeisSetFactorH29Dlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisSetFactorH29Dlg.h"
#include "DgnSeisSetFactorH29AllGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetParamDlg
CDgnSeisSetFactorH29Dlg::CDgnSeisSetFactorH29Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisSetFactorH29Dlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_RcPierGrid = new CDgnSeisSetFactorH29_RcPierGrid();
	m_StlPierGrid = new CDgnSeisSetFactorH29_StlPierGrid();
	m_CurvatureGrid = new CDgnSeisSetFactorH29_CurvatureGrid();
	m_MomentMydGrid = new CDgnSeisSetFactorH29_MomentMydGrid();
	m_MomentMudGrid = new CDgnSeisSetFactorH29_MomentMudGrid();
	m_ShearSusdGrid = new CDgnSeisSetFactorH29_ShearSusdGrid();
	m_ShearSucdGrid = new CDgnSeisSetFactorH29_ShearSucdGrid();
}

CDgnSeisSetFactorH29Dlg::~CDgnSeisSetFactorH29Dlg()
{
	m_pDoc = NULL;
	DelResource();
}

BEGIN_MESSAGE_MAP(CDgnSeisSetFactorH29Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisSetFactorH29Dlg)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDgnSeisSetFactorH29Dlg::OnCancel()
{
	// TODO:
	CDialogMove::OnCancel();
}

void CDgnSeisSetFactorH29Dlg::OnOk()
{
	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Modify_SIFA_H29)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return;

	T_SIFA_H29_K key = 1;
	T_SIFA_H29_D data;
	GetGridData(data);

	if (m_pDoc->m_pAttrCtrl2->GetQSifaH29()->Exist(key) == TRUE) {
		//수정할 값 설정
		if (m_pDoc->m_pEditData->ModifySifaH29(key, data) == FALSE) {
			ASSERT(FALSE);
			m_pDoc->m_pUndoCtrl->CancelEditDB();
			return;
		}
	}
	else {
		if (m_pDoc->m_pEditData->AddSifaH29(key, data) == FALSE) {
			ASSERT(FALSE);
			m_pDoc->m_pUndoCtrl->CancelEditDB();
			return;
		}
	}

	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIFA_H29_CMD);
	CDialogMove::OnOK();
}

void CDgnSeisSetFactorH29Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEIS_FACTOR_H29_RC_PIER_K_EDT, m_RcPierK);
	DDX_Text(pDX, IDC_SEIS_FACTOR_H29_RC_PIER_K2_EDT, m_RcPierK2);
	DDX_Text(pDX, IDC_SEIS_FACTOR_H29_STL_PIER_K_EDT, m_StlPierK);
	//{{AFX_DATA_MAP(CDgnSeisSetFactorH29Dlg)
	
	//}}AFX_DATA_MAP
}

BOOL CDgnSeisSetFactorH29Dlg::OnInitDialog()
{

	CDialogMove::OnInitDialog();

	T_SIFA_H29_D data;  data.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSifaH29()->Get(data);
	InitEditBox(data);
	InitAllGrid(data);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisSetFactorH29Dlg::InitEditBox(const T_SIFA_H29_D& rData)
{
	UpdateData(TRUE);

	m_RcPierK = rData.RcPierVal.dFactorK;
	m_RcPierK2 = rData.RcPierVal.dFactorK2;
	m_StlPierK = rData.StPierVal.dFactorK;

	UpdateData(FALSE);
}

void CDgnSeisSetFactorH29Dlg::InitAllGrid(const T_SIFA_H29_D& rData)
{
	CRect rectDummy;
	CWnd* prGrid = GetDlgItem(IDC_SEIS_FACTOR_H29_RC_PIER_GRID);
	prGrid->GetClientRect(&rectDummy);
	prGrid->MapWindowPoints(this, rectDummy);
	prGrid->ShowWindow(FALSE);
	if (!m_RcPierGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_SEIS_FACTOR_H29_RC_PIER_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_RcPierGrid->Initialize(rData);

	prGrid = GetDlgItem(IDC_SEIS_FACTOR_H29_STL_PIER_GRID);
	prGrid->GetClientRect(&rectDummy);
	prGrid->MapWindowPoints(this, rectDummy);
	prGrid->ShowWindow(FALSE);
	if (!m_StlPierGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_SEIS_FACTOR_H29_STL_PIER_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_StlPierGrid->Initialize(rData);

	prGrid = GetDlgItem(IDC_SEIS_FACTOR_H29_CURVATURE_GRID);
	prGrid->GetClientRect(&rectDummy);
	prGrid->MapWindowPoints(this, rectDummy);
	prGrid->ShowWindow(FALSE);
	if (!m_CurvatureGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_SEIS_FACTOR_H29_CURVATURE_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_CurvatureGrid->Initialize(rData);

	prGrid = GetDlgItem(IDC_SEIS_FACTOR_H29_MOMENT_MYD_GRID);
	prGrid->GetClientRect(&rectDummy);
	prGrid->MapWindowPoints(this, rectDummy);
	prGrid->ShowWindow(FALSE);
	if (!m_MomentMydGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_SEIS_FACTOR_H29_MOMENT_MYD_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_MomentMydGrid->Initialize(rData);

	prGrid = GetDlgItem(IDC_SEIS_FACTOR_H29_MOMENT_MUD_GRID);
	prGrid->GetClientRect(&rectDummy);
	prGrid->MapWindowPoints(this, rectDummy);
	prGrid->ShowWindow(FALSE);
	if (!m_MomentMudGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_SEIS_FACTOR_H29_MOMENT_MUD_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_MomentMudGrid->Initialize(rData);

	prGrid = GetDlgItem(IDC_SEIS_FACTOR_H29_SHEAR_SUSD_GRID);
	prGrid->GetClientRect(&rectDummy);
	prGrid->MapWindowPoints(this, rectDummy);
	prGrid->ShowWindow(FALSE);
	if (!m_ShearSusdGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_SEIS_FACTOR_H29_SHEAR_SUSD_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_ShearSusdGrid->Initialize(rData);

	prGrid = GetDlgItem(IDC_SEIS_FACTOR_H29_SHEAR_SUCD_GRID);
	prGrid->GetClientRect(&rectDummy);
	prGrid->MapWindowPoints(this, rectDummy);
	prGrid->ShowWindow(FALSE);
	if (!m_ShearSucdGrid->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_SEIS_FACTOR_H29_SHEAR_SUCD_GRID))
	{
		//ASSERT(FALSE);
		return;
	}
	m_ShearSucdGrid->Initialize(rData);
}

void CDgnSeisSetFactorH29Dlg::GetGridData(T_SIFA_H29_D& rData)
{
	UpdateData(TRUE);

	m_RcPierGrid->GetData(rData);
	m_StlPierGrid->GetData(rData);
	m_CurvatureGrid->GetData(rData);
	m_MomentMydGrid->GetData(rData);
	m_MomentMudGrid->GetData(rData);
	m_ShearSusdGrid->GetData(rData);
	m_ShearSucdGrid->GetData(rData);

	rData.RcPierVal.dFactorK = m_RcPierK;
	rData.RcPierVal.dFactorK2 = m_RcPierK2;
	rData.StPierVal.dFactorK = m_StlPierK;

	UpdateData(FALSE);
}

void  CDgnSeisSetFactorH29Dlg::DelResource()
{
	if (m_RcPierGrid)
	{
		delete m_RcPierGrid;
		m_RcPierGrid = NULL;
	}
	if (m_StlPierGrid)
	{
		delete m_StlPierGrid;
		m_StlPierGrid = NULL;
	}
	if (m_CurvatureGrid)
	{
		delete m_CurvatureGrid;
		m_CurvatureGrid = NULL;
	}
	if (m_MomentMydGrid)
	{
		delete m_MomentMydGrid;
		m_MomentMydGrid = NULL;
	}
	if (m_MomentMudGrid)
	{
		delete m_MomentMudGrid;
		m_MomentMudGrid = NULL;
	}
	if (m_ShearSusdGrid)
	{
		delete m_ShearSusdGrid;
		m_ShearSusdGrid = NULL;
	}
	if (m_ShearSucdGrid)
	{
		delete m_ShearSucdGrid;
		m_ShearSucdGrid = NULL;
	}
}