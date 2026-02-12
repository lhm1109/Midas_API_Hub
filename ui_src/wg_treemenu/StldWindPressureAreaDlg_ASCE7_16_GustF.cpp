// StldWindPressureAreaDlg_ASCE7_16_GustF.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_ASCE7_16_GustF.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_db\Wind_ASCE7_16.h"
#include "..\wg_db\Wind_NSCP2024.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_ASCE7_GustF dialog
CStldWindPressureAreaDlg_ASCE7_GustF::CStldWindPressureAreaDlg_ASCE7_GustF(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureAreaDlg_ASCE7_GustF::IDD, pParent)
{
	m_dGustFactor = 0.0;
	m_Data.Initialize();
	m_nWindCode = 0;
}


void CStldWindPressureAreaDlg_ASCE7_GustF::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_EXPOSURE_CATE, m_edtExposureCate);
	DDX_Control(pDX, IDC_TM_BWS_EDIT, m_edtBasicWindSpeed);
// 	DDX_Control(pDX, IDC_TM_BWS_UNIT, m_stcBasicWindSpeed);	
	DDX_Control(pDX, IDC_TM_DIRECTIONAL_EDIT, m_edtDirectionalFactor);
	DDX_Control(pDX, IDC_TM_TOPOGRAPHY_EDIT,  m_edtTopoEffects);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT1,   m_edtAverageRoofH);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT1,   m_stcAverageRoofH);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT2,   m_edtBreadth);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT2,   m_stcBreadth);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT3,   m_edtBreadth_L);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT3,   m_stcBreadth_L);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT4,   m_edtNaturalFreq);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT5,   m_edtDampingRatio);
	DDX_Control(pDX, IDC_TM_WINDGUST_GUST_EDIT, m_edtGustFactor);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_ASCE7_GustF, CDialogMove)
	ON_BN_CLICKED(IDC_TM_WINDGUST_CAL,    OnCmdWindgustCal)
	ON_BN_CLICKED(IDC_TM_WINDGUST_OK,     OnCmdWindgustOk)
	ON_BN_CLICKED(IDC_TM_WINDGUST_CANCEL, OnCmdWindgustCancel)
	ON_BN_CLICKED(IDC_TM_RIGID_RADIO1, OnCmdRdoChanged)
	ON_BN_CLICKED(IDC_TM_FLEX_RADIO2,  OnCmdRdoChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_ASCE7_GustF message handlers

BOOL CStldWindPressureAreaDlg_ASCE7_GustF::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	if(m_Data.nStructureType == 2)
	{
		((CButton*)GetDlgItem(IDC_TM_RIGID_RADIO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_FLEX_RADIO2))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_TM_RIGID_RADIO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_FLEX_RADIO2))->SetCheck(FALSE);
	}

	InitControl();
	Data2Dlg();
	OnCmdRdoChanged();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureAreaDlg_ASCE7_GustF::OnCmdWindgustCal() 
{
	Dlg2Data();

	// 단위를 m로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	const double dH = m_Data.dAverageRoofH;
	const double dBx = m_Data.dBreadth;
	const double dBL = m_Data.dBreadth_L;
	if(dH <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei)); return;
	}
	if(dBx <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Breath)); return;
	}
	
	if(m_Data.nStructureType == 2) // Flexible Structure
	{
		if (dBL <= 0)
		{
			AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Length)); return;
		}
		if (m_Data.dKd < 0.85 || m_Data.dKd > 1.0)
		{
			AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_KDX)); return;
		}
		if(m_Data.dNaturalFreq <= 1E-5) //X방향일때
		{
			AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_X_Natural_Fr)); return;
		}
		if(m_Data.dDampingRatio <= 1E-5) 
		{
			AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Damping_Rati)); return;
		}

	}

	switch (m_nWindCode)
	{
	case ASCE7_W_2016:
	case ASCE7_W_2022:
		{
			CWind_ASCE7_16 WindCalc(ASCE7_W_2022, CWind_ASCE7_16::EN_UNIT_US);
			if (m_Data.nStructureType == 1) // Rigid Structure
			{
				m_dGustFactor = WindCalc.CalcGustFactorRigid(m_Data.nExposureCategory, dBx, dH);
			}
			else // Flexible Structure
			{
				m_dGustFactor = WindCalc.CalcGustFactorFlexible(m_Data.nExposureCategory, m_Data.dBasicWindSpeed, dH, dBx, dBL, m_Data.dNaturalFreq, m_Data.dDampingRatio);
			}
		}
		break;
	case NSCP_W_2024:
		{
			CWind_NSCP2024 WindCalc;
			if (m_Data.nStructureType == 1) // Rigid Structure
			{
				m_dGustFactor = WindCalc.CalcGustFactorRigid(m_Data.nExposureCategory, dBx, dH);
			}
			else // Flexible Structure
			{
				m_dGustFactor = WindCalc.CalcGustFactorFlexible(m_Data.nExposureCategory, m_Data.dBasicWindSpeed, dH, dBx, dBL, m_Data.dNaturalFreq, m_Data.dDampingRatio);
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}	

	DisplayGustFactor();
}

void CStldWindPressureAreaDlg_ASCE7_GustF::OnCmdWindgustOk() 
{
	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_ASCE7_GustF::OnCmdWindgustCancel() 
{
	CDialogMove::OnCancel();		
}

void CStldWindPressureAreaDlg_ASCE7_GustF::InitControl()
{
	m_edtBasicWindSpeed.SetUnitType(D_UNITSYS_NONE); //  mile/h
// 	m_stcBasicWindSpeed.SetUnitType(D_UNITSYS_BASE_VELOCITY); // m/sec
	m_edtDirectionalFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtAverageRoofH.SetUnitType(D_UNITSYS_BASE_LENGTH); // m
	m_stcAverageRoofH.SetUnitType(D_UNITSYS_BASE_LENGTH); // m
	m_edtBreadth.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_stcBreadth.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_edtBreadth_L.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_stcBreadth_L.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_edtNaturalFreq.SetUnitType(D_UNITSYS_NONE);  // Hz
	m_edtDampingRatio.SetUnitType(D_UNITSYS_NONE);

	m_edtGustFactor.SetUnitType(D_UNITSYS_NONE);
}

void CStldWindPressureAreaDlg_ASCE7_GustF::ShowHideControls()
{
	int nShow = SW_SHOW;
	if (m_Data.nStructureType == 1)  nShow = SW_HIDE;// Rigid Structure

	GetDlgItem(IDC_TM_WINDGUST_TITLE3)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT3)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_UNIT3)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_TITLE4)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT4)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_UNIT4)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_TITLE5)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT5)->ShowWindow(nShow);
	
}

void CStldWindPressureAreaDlg_ASCE7_GustF::OnCmdRdoChanged()
{
	Dlg2Data();

	if(((CButton*)GetDlgItem(IDC_TM_FLEX_RADIO2))->GetCheck())
		m_Data.nStructureType = 2;
	else
		m_Data.nStructureType = 1; //기본 1 (Rigid) 로 선택...
	
	DisplayGustFactor();
	ShowHideControls();  
	Data2Dlg(); //방향이 바뀌면 내용도 바뀔수...
}

BOOL CStldWindPressureAreaDlg_ASCE7_GustF::Dlg2Data()
{
	m_Data.dBreadth       = m_edtBreadth.GetEditValue();
	m_Data.dBreadth_L	  = m_edtBreadth_L.GetEditValue();

	m_Data.dNaturalFreq   = m_edtNaturalFreq.GetEditValue();
	m_Data.dDampingRatio  = m_edtDampingRatio.GetEditValue();
	
	return TRUE;
}

BOOL CStldWindPressureAreaDlg_ASCE7_GustF::Data2Dlg()
{
	if(((CButton*)GetDlgItem(IDC_TM_FLEX_RADIO2))->GetCheck())
		m_Data.nStructureType = 2;
	else
		m_Data.nStructureType = 1;

	if(m_Data.nExposureCategory == 1)      m_edtExposureCate.SetWindowText(_T("A"));
	else if(m_Data.nExposureCategory == 2) m_edtExposureCate.SetWindowText(_T("B"));
	else if(m_Data.nExposureCategory == 3) m_edtExposureCate.SetWindowText(_T("C"));
	else if(m_Data.nExposureCategory == 4) m_edtExposureCate.SetWindowText(_T("D"));
	else ASSERT(0);

	m_edtBasicWindSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_edtDirectionalFactor.SetEditUnit(m_Data.dKd);
	CString strTopography;
	if(m_Data.bTopoEffects) strTopography = _LS(IDS_CMD_WIND_NBC95_INCLUDED);
	else                    strTopography = _LS(IDS_CMD_WIND_NBC95_NOT_INCLUDED);
	m_edtTopoEffects.SetWindowText(strTopography);

	m_edtAverageRoofH.SetEditUnit(m_Data.dAverageRoofH);
	m_edtBreadth.SetEditUnit(m_Data.dBreadth);
	m_edtBreadth_L.SetEditUnit(m_Data.dBreadth_L);
	m_edtNaturalFreq.SetEditUnit(m_Data.dNaturalFreq);
	m_edtDampingRatio.SetEditUnit(m_Data.dDampingRatio);

	DisplayGustFactor();

	return TRUE;  
}

void CStldWindPressureAreaDlg_ASCE7_GustF::DisplayGustFactor()
{
	if(m_dGustFactor > 0.0) m_edtGustFactor.SetEditUnit(m_dGustFactor, 4);
	else                    m_edtGustFactor.SetWindowText(_T(""));
}


BOOL CStldWindPressureAreaDlg_ASCE7_GustF::SetWindData( const T_WVEP_ASCE7_16* pASCE7, T_WDPR_D* pData )
{
	if (pASCE7 == NULL) return FALSE;

	T_WDPR_ASCE7_16* pWdprCode = pData->GetASCE7Series();

	m_Data.Initialize();
	m_Data.nExposureCategory = pASCE7->nExposureCategory;
	m_Data.dBasicWindSpeed   = pASCE7->dBasicWindSpeed;
	m_Data.bTopoEffects      = pASCE7->bTopographicEffect;
	m_Data.dAverageRoofH     = pASCE7->dMeanRoofHeight;
	m_Data.dKzt              = pASCE7->dKzt;
	m_Data.nStructureType	= pWdprCode->nRigidity;
	m_Data.dBreadth			= pWdprCode->dBreath;
	m_Data.dNaturalFreq		= pWdprCode->dNaturalFreq;
	m_Data.dDampingRatio	= pWdprCode->dDampingRatio;
	m_Data.dKd				= pWdprCode->dKd;
	m_dGustFactor			= pWdprCode->dGfExternal;

	m_nWindCode = pData->nCodeType;

	int    nStore;
	double dHeight;
	double dWidth;
	if (CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(pData, dHeight, nStore, dWidth))
	{
		m_Data.dBreadth = dWidth;
		return TRUE;
	}
	m_Data.dBreadth = 0.0;
	return FALSE;
}

void CStldWindPressureAreaDlg_ASCE7_GustF::GetDlgInfo(T_WDPR_ASCE7_16& ASCE7)
{
	ASCE7.nRigidity		= m_Data.nStructureType;
	ASCE7.dBreath		= m_Data.dBreadth;
	ASCE7.dNaturalFreq	= m_Data.dNaturalFreq;
	ASCE7.dDampingRatio	= m_Data.dDampingRatio;
	ASCE7.dGfExternal	= m_dGustFactor;
}