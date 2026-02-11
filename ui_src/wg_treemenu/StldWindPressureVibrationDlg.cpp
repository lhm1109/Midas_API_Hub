// StldWindPressureVibrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StldWindPressureVibrationDlg.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\WindVibrationDlg.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureVibrationDlg dialog
CStldWindPressureVibrationDlg::CStldWindPressureVibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureVibrationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldWindPressureVibrationDlg)
	m_dBreadthX = 0.0;
	m_dBreadthY = 0.0;
	m_dFreqA = 0.0;
	m_dFreqT = 0.0;
	m_dZf = 0.0;
	//}}AFX_DATA_INIT
}


void CStldWindPressureVibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldWindPressureVibrationDlg)
	DDX_Control(pDX, IDC_ETC_WIND_EDIT1, m_edtBreadthX);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT2, m_edtBreadthY);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT4, m_edtFreqA);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT5, m_edtFreqT);
	DDX_Control(pDX, IDC_ETC_WIND_EDIT9, m_edtZf);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT1, m_wndBreadthXUnit);
	DDX_Control(pDX, IDC_ETC_WIND_UNIT2, m_wndBreadthYUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStldWindPressureVibrationDlg, CDialogMove)
	//{{AFX_MSG_MAP(CStldWindPressureVibrationDlg)
	ON_BN_CLICKED(IDC_ETC_WIND_ZF_BTN, OnCmdZfBtn)
	ON_BN_CLICKED(IDC_ETC_WIND_OK, OnCmdOk)
	ON_BN_CLICKED(IDC_ETC_WIND_CANCEL, OnCmdCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureVibrationDlg message handlers

BOOL CStldWindPressureVibrationDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC4, m_nAcrossCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_TITLE_STC5, m_nTorsionalCheck==1);
	
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT4, m_nAcrossCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_EDIT5, m_nTorsionalCheck==1);
	
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT4, m_nAcrossCheck==1);
	CDlgUtil::CtrlEnableDisable(this, IDC_ETC_WIND_UNIT5, m_nTorsionalCheck==1);

	InitialData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureVibrationDlg::InitialData()
{
	m_edtBreadthX.SetEditUnit(m_dBreadthX);
	m_edtBreadthY.SetEditUnit(m_dBreadthY);
	m_edtFreqA.SetEditUnit(m_dFreqA);
	m_edtFreqT.SetEditUnit(m_dFreqT);
	m_edtZf.SetEditUnit(m_dZf);
	
	m_wndBreadthXUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndBreadthYUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	UpdateData(FALSE);
}

void CStldWindPressureVibrationDlg::OnCmdZfBtn() 
{
	CWindVibrationZfDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CStldWindPressureVibrationDlg::OnCmdOk() 
{
	UpdateData(TRUE);
	
	m_dBreadthX	= m_edtBreadthX.GetEditValue();
	m_dBreadthY	= m_edtBreadthY.GetEditValue();
	m_dFreqA		= m_edtFreqA.GetEditValue();
	m_dFreqT		= m_edtFreqT.GetEditValue();
	m_dZf				= m_edtZf.GetEditValue();

	if(!CheckValue()) return;

	CDialogMove::OnOK();
}

void CStldWindPressureVibrationDlg::OnCmdCancel() 
{
	CDialogMove::OnCancel();		
}

BOOL CStldWindPressureVibrationDlg::SetWindData(T_WDPR_KBC2016* pData, int nDirection, double dAngle)
{
	m_dBreadthX = pData->dBLy;
	m_dBreadthY = pData->dBLx;
	m_dFreqA = pData->dNoa;
	m_dFreqT = pData->dNot;
	m_dZf = pData->dZf;
	int    nStore;
	double dHeight;
	double dThick;
	double dWidth;
	if(CWindLoadGeneratorCtrl::GetModelHeightWidthThickTotal(nDirection, dAngle, dHeight, nStore, dWidth, dThick))
	{
		if(m_dBreadthX<=0.0) m_dBreadthX = dWidth;
		if(m_dBreadthY<=0.0) m_dBreadthY = dThick;
		return TRUE;
	}
	return FALSE;
}

BOOL CStldWindPressureVibrationDlg::CheckValue()
{
	if(m_nBuildingType==1 && (m_nAcrossCheck || m_nTorsionalCheck))
	{
		if(m_dBreadthX <= 0) return CheckErrorDataVar(_LS(IDS_DB_DT_WIND), _LS(IDS_DB_DT_WIND_Breadth));
		if(m_dBreadthY <= 0) return CheckErrorDataVar(_LS(IDS_DB_DT_WIND), _LS(IDS_DB_DT_WIND_Depth));
	}
	if(m_nBuildingType==1 && m_nAcrossCheck)
	{
		if(m_dFreqA <= 0) return CheckErrorDataVar(_LS(IDS_DB_DT_WIND), _LS(IDS_DB_DT_WIND_NOA));
	}
	if(m_nTorsionalCheck)
	{
		if(m_dFreqT <= 0) return CheckErrorDataVar(_LS(IDS_DB_DT_WIND), _LS(IDS_DB_DT_WIND_NOT));
	}
	if(m_nBuildingType==1 && (m_nAcrossCheck || m_nTorsionalCheck))
	{
		if(m_dZf <= 0) return CheckErrorDataVar(_LS(IDS_DB_DT_WIND), _LS(IDS_DB_DT_WIND_ZF));
	}

	return TRUE;
}

BOOL CStldWindPressureVibrationDlg::CheckErrorDataVar(LPCTSTR lpszData, LPCTSTR lpszVar)
{
	CDBDoc::GetDocPoint()->DisplayHistoryMessage(_LS(IDS_DB_IREG_DT_VAR_ERROR), lpszData, lpszVar);
	return FALSE;
}