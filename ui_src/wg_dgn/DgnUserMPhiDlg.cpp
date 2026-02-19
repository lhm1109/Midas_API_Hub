// DgnUserMPhiDlg.cpp: implementation of the CDgnUserMPhiDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnUserMPhiDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnUserMPhiDlg::CDgnUserMPhiDlg(CWnd* pParent)
	: CDialogMove(CDgnUserMPhiDlg::IDD, pParent)
{
	m_pY_Grid = new CDgnUserMPhiGrid(this);
	m_pZ_Grid = new CDgnUserMPhiGrid(this);
}

CDgnUserMPhiDlg::~CDgnUserMPhiDlg()
{	
	if(m_pY_Grid != NULL)
	{
		delete m_pY_Grid;
		m_pY_Grid = NULL;
	}

	if(m_pZ_Grid != NULL)
	{
		delete m_pZ_Grid;
		m_pZ_Grid = NULL;
	}
}


void CDgnUserMPhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnUserMPhiDlg)	
	DDX_Control(pDX, IDC_DGN_USER_MPHI_Y_SIZE_EDT,      m_Size_Y_Edit);	
	DDX_Control(pDX, IDC_DGN_USER_MPHI_Z_SIZE_EDT,      m_Size_Z_Edit);	
	DDX_Control(pDX, IDC_DGN_USER_MPHI_Y_YIELD_CUR_EDT, m_YieldCur_Y_Edit);	
	DDX_Control(pDX, IDC_DGN_USER_MPHI_Z_YIELD_CUR_EDT, m_YieldCur_Z_Edit);	
	DDX_Control(pDX, IDC_DGN_USER_MPHI_Y_ULTI_CUR_EDT,  m_UltiCur_Y_Edit);	
	DDX_Control(pDX, IDC_DGN_USER_MPHI_Z_ULTI_CUR_EDT,  m_UltiCur_Z_Edit);	

	DDX_Control(pDX, IDC_DGN_USER_MPHI_Y_GRID, *m_pY_Grid);
	DDX_Control(pDX, IDC_DGN_USER_MPHI_Z_GRID, *m_pZ_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnUserMPhiDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnUserMPhiDlg)
	ON_EN_KILLFOCUS(IDC_DGN_USER_MPHI_Y_SIZE_EDT,       OnKillFocusYSize)
	ON_EN_KILLFOCUS(IDC_DGN_USER_MPHI_Z_SIZE_EDT,       OnKillFocusZSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDgnUserMPhiDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCtrl();
	Data2Dlg();

	return TRUE;
}

void CDgnUserMPhiDlg::InitUnit()
{	
	m_YieldCur_Y_Edit.SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_YieldCur_Z_Edit.SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_UltiCur_Y_Edit.SetUnitType(D_UNITSYS_BASE_PERLENGTH);
	m_UltiCur_Z_Edit.SetUnitType(D_UNITSYS_BASE_PERLENGTH);
}

void CDgnUserMPhiDlg::InitCtrl()
{	
	m_pY_Grid->Initialize(&(m_arPierUserMPhi[0]));
	m_pZ_Grid->Initialize(&(m_arPierUserMPhi[1]));  
}

void CDgnUserMPhiDlg::Data2Dlg()
{
	int nSize;
	CString strSize;
	
	nSize = m_arPierUserMPhi[0].GetSize();
	strSize.Format(_T("%d"), nSize);
	m_Size_Y_Edit.SetWindowText(strSize);
	m_YieldCur_Y_Edit.SetEditUnit(m_dPierYieldCurvature[0]);
	m_UltiCur_Y_Edit.SetEditUnit(m_dPierUltimateCurvature[0]);
	m_pY_Grid->Data2Grid();
		
	nSize = m_arPierUserMPhi[1].GetSize();
	strSize.Format(_T("%d"), nSize);
	m_Size_Z_Edit.SetWindowText(strSize);
	m_YieldCur_Z_Edit.SetEditUnit(m_dPierYieldCurvature[1]);
	m_UltiCur_Z_Edit.SetEditUnit(m_dPierUltimateCurvature[1]);
	m_pZ_Grid->Data2Grid();
}

BOOL CDgnUserMPhiDlg::Dlg2Data()
{
	m_dPierYieldCurvature[0]    = m_YieldCur_Y_Edit.GetEditValue();
	m_dPierYieldCurvature[1]    = m_YieldCur_Z_Edit.GetEditValue();
	m_dPierUltimateCurvature[0] = m_UltiCur_Y_Edit.GetEditValue();
	m_dPierUltimateCurvature[1] = m_UltiCur_Z_Edit.GetEditValue();
	m_pY_Grid->Grid2Data();
	m_pZ_Grid->Grid2Data();

	return TRUE;
}
void CDgnUserMPhiDlg::OnOK()
{
	Dlg2Data();
	CDialogMove::OnOK();
}

void CDgnUserMPhiDlg::OnKillFocusYSize()
{
	CString strSize;	
	m_Size_Y_Edit.GetWindowText(strSize);
	int nSize =_ttoi(strSize);
	m_pY_Grid->SetRowSize(nSize);
}

void CDgnUserMPhiDlg::OnKillFocusZSize()
{
	CString strSize;	
	m_Size_Z_Edit.GetWindowText(strSize);
	int nSize =_ttoi(strSize);
	m_pZ_Grid->SetRowSize(nSize);
}