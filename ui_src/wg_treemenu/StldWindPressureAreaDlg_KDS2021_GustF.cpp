// WindGustKDS2021Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_KDS2021_GustF.h"


#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_db\Wind_KDS41_10_15_2021.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KDS2021_GustF dialog
CStldWindPressureAreaDlg_KDS2021_GustF::CStldWindPressureAreaDlg_KDS2021_GustF(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureAreaDlg_KDS2021_GustF::IDD, pParent)
{
	m_dGustFactor = 0.0;
	m_bKBC2016 = FALSE;
}


void CStldWindPressureAreaDlg_KDS2021_GustF::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_EXPOSURE_CATE, m_edtExposureCate);
	DDX_Control(pDX, IDC_TM_BWS_EDIT, m_edtBasicWindSpeed);
// 	DDX_Control(pDX, IDC_TM_BWS_UNIT, m_stcBasicWindSpeed);	
	DDX_Control(pDX, IDC_TM_IF_EDIT,  m_edtImportanceFactor);	
	DDX_Control(pDX, IDC_TM_DIRECTIONAL_EDIT, m_edtDirectionalFactor);
	DDX_Control(pDX, IDC_TM_TOPOGRAPHY_EDIT, m_edtTopoEffects);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT1,   m_edtAverageRoofH);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT1,   m_stcAverageRoofH);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT2,   m_edtBreadth);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT2,   m_stcBreadth);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT4,   m_edtNaturalFreq);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT5,   m_edtDampingRatio);
	DDX_Control(pDX, IDC_TM_WINDGUST_GUST_EDIT, m_edtGustFactor);

	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT7, m_edtTotalMass);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT7, m_stcTotalMass);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT8, m_edt1stMx);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT8, m_stc1stMx);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT9, m_editVibration);

}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_KDS2021_GustF, CDialogMove)
	ON_BN_CLICKED(IDC_TM_WINDGUST_CAL,    OnCmdWindgustCal)
	ON_BN_CLICKED(IDC_TM_WINDGUST_OK,     OnCmdWindgustOk)
	ON_BN_CLICKED(IDC_TM_WINDGUST_CANCEL, OnCmdWindgustCancel)
	ON_BN_CLICKED(IDC_TM_RIGID_RADIO1, OnCmdRdoChanged)
	ON_BN_CLICKED(IDC_TM_FLEX_RADIO2,  OnCmdRdoChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KDS2021_GustF message handlers

BOOL CStldWindPressureAreaDlg_KDS2021_GustF::OnInitDialog() 
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

void CStldWindPressureAreaDlg_KDS2021_GustF::OnCmdWindgustCal() 
{
	Dlg2Data();

	// 단위를 m로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	const double dToM = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);
	const double dH_UnitM  = m_Data.dAverageRoofH * dToM;
	const double dBx_UnitM = m_Data.dBreadth * dToM;

	const double dToKG = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_MASS, 1.0);
	const double dMassT = m_Data.dTotalMass* dToKG;
	const double dMassx = m_Data.d1stMx* dToKG;
	
	if( dH_UnitM <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei)); return;
	}
	if( dBx_UnitM <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_X_B)); return;
	}

	if(m_Data.nStructureType == 2) // Flexible Structure
	{
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
		if (m_Data.dTotalMass <= 0.0)
		{
			AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_TotalMass)); return;
		}
		if (m_Data.d1stMx <= 0.0)
		{
			AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_MassX)); return;
		}
		if (m_Data.dVibration < 0.5 || m_Data.dVibration > 2.0)
		{
			AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Viboration)); return;
		}
	}
	
	CWind_KDS41_10_15_2021 WindCalc;
	if (m_Data.nStructureType == 1) // Rigid Structure
	{
		m_dGustFactor =  WindCalc.CalcGustEffectFactor_Rigid(m_Data.nExposureCategory, dBx_UnitM, dH_UnitM, m_Data.dNaturalFreq);
	}
	else // Flexible Structure
	{
		m_dGustFactor = WindCalc.CalcGustEffectFactor_Flexible(m_Data.nExposureCategory, dBx_UnitM, dH_UnitM, m_Data.dNaturalFreq,
			m_Data.dBasicWindSpeed, dMassT, dMassx, m_Data.dDampingRatio, m_Data.dVibration, m_Data.bTopoEffects, m_Data.dKzt, m_Data.dImportanceFactor, m_Data.dKd);
	}

	DisplayGustFactor();
}

void CStldWindPressureAreaDlg_KDS2021_GustF::OnCmdWindgustOk() 
{
	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_KDS2021_GustF::OnCmdWindgustCancel() 
{
	CDialogMove::OnCancel();		
}

void CStldWindPressureAreaDlg_KDS2021_GustF::InitControl()
{
	m_edtBasicWindSpeed.SetUnitType(D_UNITSYS_BASE_VELOCITY); // m/sec
// 	m_stcBasicWindSpeed.SetUnitType(D_UNITSYS_BASE_VELOCITY); // m/sec
	m_edtImportanceFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtDirectionalFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtAverageRoofH.SetUnitType(D_UNITSYS_BASE_LENGTH); // m
	m_stcAverageRoofH.SetUnitType(D_UNITSYS_BASE_LENGTH); // m
	m_edtBreadth.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_stcBreadth.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_edtNaturalFreq.SetUnitType(D_UNITSYS_NONE);  // Hz
	m_edtDampingRatio.SetUnitType(D_UNITSYS_NONE);

	m_edtTotalMass.SetUnitType(D_UNITSYS_BASE_MASS);
	m_stcTotalMass.SetUnitType(D_UNITSYS_BASE_MASS);
	m_edt1stMx.SetUnitType(D_UNITSYS_BASE_MASS);  //
	m_stc1stMx.SetUnitType(D_UNITSYS_BASE_MASS);  //
	m_editVibration.SetUnitType(D_UNITSYS_NONE);

	m_edtGustFactor.SetUnitType(D_UNITSYS_NONE);
}

void CStldWindPressureAreaDlg_KDS2021_GustF::ShowHideControls()
{
	int nShow = SW_SHOW;
	if (m_Data.nStructureType == 1)  nShow = SW_HIDE;// Rigid Structure

	GetDlgItem(IDC_TM_WINDGUST_TITLE4)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT4)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_UNIT4)->ShowWindow(nShow);

	GetDlgItem(IDC_TM_WINDGUST_TITLE5)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT5)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_TITLE7)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT7)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_UNIT7)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_TITLE8)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT8)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_UNIT8)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_TITLE9)->ShowWindow(nShow);
	GetDlgItem(IDC_TM_WINDGUST_EDIT9)->ShowWindow(nShow);
}

void CStldWindPressureAreaDlg_KDS2021_GustF::OnCmdRdoChanged()
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

BOOL CStldWindPressureAreaDlg_KDS2021_GustF::Dlg2Data()
{
	m_Data.dBreadth       = m_edtBreadth.GetEditValue();
	m_Data.dNaturalFreq   = m_edtNaturalFreq.GetEditValue();
	m_Data.dDampingRatio  = m_edtDampingRatio.GetEditValue();

	m_Data.dTotalMass	 = m_edtTotalMass.GetEditValue();
	m_Data.d1stMx		 = m_edt1stMx.GetEditValue();
	m_Data.dVibration	 = m_editVibration.GetEditValue();
	
	return TRUE;
}

BOOL CStldWindPressureAreaDlg_KDS2021_GustF::Data2Dlg()
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
	m_edtImportanceFactor.SetEditUnit(m_Data.dImportanceFactor);
	m_edtDirectionalFactor.SetEditUnit(m_Data.dKd);
	CString strTopography;
	if(m_Data.bTopoEffects) strTopography = _LS(IDS_CMD_WIND_NBC95_INCLUDED);
	else                    strTopography = _LS(IDS_CMD_WIND_NBC95_NOT_INCLUDED);
	m_edtTopoEffects.SetWindowText(strTopography);

	m_edtAverageRoofH.SetEditUnit(m_Data.dAverageRoofH);
	m_edtBreadth.SetEditUnit(m_Data.dBreadth);
	m_edtNaturalFreq.SetEditUnit(m_Data.dNaturalFreq);
	m_edtDampingRatio.SetEditUnit(m_Data.dDampingRatio);
	m_edtTotalMass.SetEditUnit(m_Data.dTotalMass);
	m_edt1stMx.SetEditUnit(m_Data.d1stMx);
	m_editVibration.SetEditUnit(m_Data.dVibration);

	DisplayGustFactor();

	return TRUE;  
}

void CStldWindPressureAreaDlg_KDS2021_GustF::DisplayGustFactor()
{
	if(m_dGustFactor > 0.0) m_edtGustFactor.SetEditUnit(m_dGustFactor, 4);
	else                    m_edtGustFactor.SetWindowText(_T(""));
}


BOOL CStldWindPressureAreaDlg_KDS2021_GustF::SetWindData( const T_WVEP_KDS2021& KDS2021, T_WDPR_D* pData )
{
	m_bKBC2016 = FALSE;

	m_Data.nExposureCategory = KDS2021.nExposureCategory;
	m_Data.dImportanceFactor = KDS2021.dImportanceFactor;
	m_Data.bTopoEffects      = KDS2021.bTopographicEffect;
	m_Data.dNaturalFreq		 = pData->CodeParam.KDS2021.dNaturalFreq;
	m_Data.dDampingRatio	 = pData->CodeParam.KDS2021.dDampingRatio;

	if (KDS2021.nMethod == 0)
	{
		m_Data.dBasicWindSpeed = KDS2021.dSimpleBasicWindSpeed;
		m_Data.dAverageRoofH = KDS2021.dSimpleRoofHeight;
	}
	else
	{
		m_Data.dBasicWindSpeed = KDS2021.dBasicWindSpeed;
		m_Data.dTotalMass = KDS2021.dVortexMass;
		m_Data.d1stMx = KDS2021.dVortexMass / 3.;

		if (KDS2021.bVortexShedding)
		{
			m_Data.dAverageRoofH = KDS2021.dVortexAverageRoof;
			m_Data.dNaturalFreq  = KDS2021.dVortexN;
			m_Data.dDampingRatio = KDS2021.dVortexDamp;
		}
		else m_Data.dAverageRoofH = m_Data.dAverageRoofH = KDS2021.dRoofHeight;
	}

	m_Data.dKzt = KDS2021.dKzt;
	m_Data.dHzt = KDS2021.dVzt;

	m_Data.nStructureType = pData->CodeParam.KDS2021.nRigidity;
// 	m_Data.dTotalMass	  = pData->CodeParam.KDS2021.dTotalMass;
// 	m_Data.d1stMx		  = pData->CodeParam.KDS2021.dMx1st;
	m_Data.dVibration	  = pData->CodeParam.KDS2021.dVibrationMode;
	m_Data.dKd			  = pData->CodeParam.KDS2021.dKd;
	m_dGustFactor         = pData->CodeParam.KDS2021.dGfExternal;
	int    nStore;
	double dHeight;
	double dWidth;
	if(CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(pData, dHeight, nStore, dWidth))
	{
	  m_Data.dBreadth = dWidth;
	  return TRUE;
	}
	m_Data.dBreadth = 0.0;
	return FALSE;
}

void CStldWindPressureAreaDlg_KDS2021_GustF::GetDlgInfo(T_WDPR_KDS2021& KDS2021)
{
	KDS2021.nRigidity		= m_Data.nStructureType;
	KDS2021.dBreath			= m_Data.dBreadth;
	KDS2021.dNaturalFreq	= m_Data.dNaturalFreq;
	KDS2021.dDampingRatio	= m_Data.dDampingRatio;
	KDS2021.dTotalMass		= m_Data.dTotalMass;
	KDS2021.dMx1st			= m_Data.d1stMx;
	KDS2021.dVibrationMode	= m_Data.dVibration;
	KDS2021.dGfExternal		= m_dGustFactor;
}