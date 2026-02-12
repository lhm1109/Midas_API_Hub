// StldWindPressureStructDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureStructDlg.h"


// CStldWindPressureStructDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStldWindPressureStructDlg, CDialogMove)

CStldWindPressureStructDlg::CStldWindPressureStructDlg(CWnd* pParent/*=NULL*/, int nStructType/*=-1*/, int nCodeType/* = -1*/)
	: CDialogMove(CStldWindPressureStructDlg::IDD, pParent)
{
	m_nStructType = nStructType;
	m_nCodeType = nCodeType;
}

CStldWindPressureStructDlg::~CStldWindPressureStructDlg()
{
}

void CStldWindPressureStructDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_STLD_WINDP_STRUCT_CMB,	m_cmbStructureType);
}

BEGIN_MESSAGE_MAP(CStldWindPressureStructDlg, CDialogMove)
	ON_BN_CLICKED(IDOK, OnCmdOk)
END_MESSAGE_MAP()


// CStldWindPressureStructDlg 메시지 처리기입니다.

BOOL CStldWindPressureStructDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitStructureTypeCombo();

	int nStructType = m_cmbStructureType.GetCount();
	for(int i=0; i<nStructType; i++)
	{
		if(m_cmbStructureType.GetItemData(i)!=m_nStructType) continue;
		m_cmbStructureType.SetCurSel(i);
		break;
	}

	return TRUE;
}

void CStldWindPressureStructDlg::OnCmdOk() 
{
	m_nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	CDialogMove::OnOK();
}

void CStldWindPressureStructDlg::InitStructureTypeCombo()
{
	m_cmbStructureType.ResetContent();
	int nIndex = 0;
	if (m_nCodeType == ASCE7_W_2016 || m_nCodeType == ASCE7_W_2022 || m_nCodeType == NSCP_W_2024)
	{
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CIRCLE_PLAN)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CIRCLE_PLAN);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_TRUSSED_TOWERS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_LATTICE_TOWERS);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_SINGLEPLANE)); m_cmbStructureType.SetItemData(nIndex, ENM_T_OPENSIGNSLF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CHIMNEYSTSS);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_SOLID_FREESTANDING)); m_cmbStructureType.SetItemData(nIndex, ENM_T_SOLID_FREE);

	}
	else
	{
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CIRCLE_PLAN)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CIRCLE_PLAN);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_LATTICE_TOWERS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_LATTICE_TOWERS);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_LATTICE_FRAMEWORKS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_OPENSIGNSLF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CHIMNEYSTSS);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_FENCES_ON_GROUND)); m_cmbStructureType.SetItemData(nIndex, ENM_T_FENCES_ON_GRND);
	}
	m_cmbStructureType.SetCurSel(0);
}

int CStldWindPressureStructDlg::GetStructureType()
{
	return m_nStructType;
}
