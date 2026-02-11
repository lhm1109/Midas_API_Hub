// WindGustKBC2009Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_KBC2009_GustF.h"

#include <math.h>


#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009_GustF dialog
CStldWindPressureAreaDlg_KBC2009_GustF::CStldWindPressureAreaDlg_KBC2009_GustF(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureAreaDlg_KBC2009_GustF::IDD, pParent)
{
	m_dGustFactor = 0.0;
	m_bKBC2016 = FALSE;
}


void CStldWindPressureAreaDlg_KBC2009_GustF::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_EXPOSURE_CATE, m_edtExposureCate);
	DDX_Control(pDX, IDC_TM_BWS_EDIT, m_edtBasicWindSpeed);
// 	DDX_Control(pDX, IDC_TM_BWS_UNIT, m_stcBasicWindSpeed);	
	DDX_Control(pDX, IDC_TM_IF_EDIT,  m_edtImportanceFactor);	
	DDX_Control(pDX, IDC_TM_TOPOGRAPHY_EDIT, m_edtTopoEffects);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT1,   m_edtAverageRoofH);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT1,   m_stcAverageRoofH);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT2,   m_edtBreadth);
	DDX_Control(pDX, IDC_TM_WINDGUST_UNIT2,   m_stcBreadth);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT4,   m_edtNaturalFreq);
	DDX_Control(pDX, IDC_TM_WINDGUST_EDIT5,   m_edtDampingRatio);
	DDX_Control(pDX, IDC_TM_WINDGUST_GUST_EDIT, m_edtGustFactor);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_KBC2009_GustF, CDialogMove)
	ON_BN_CLICKED(IDC_TM_WINDGUST_CAL,    OnCmdWindgustCal)
	ON_BN_CLICKED(IDC_TM_WINDGUST_OK,     OnCmdWindgustOk)
	ON_BN_CLICKED(IDC_TM_WINDGUST_CANCEL, OnCmdWindgustCancel)
	ON_BN_CLICKED(IDC_TM_RIGID_RADIO1, OnCmdRdoChanged)
	ON_BN_CLICKED(IDC_TM_FLEX_RADIO2,  OnCmdRdoChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009_GustF message handlers

BOOL CStldWindPressureAreaDlg_KBC2009_GustF::OnInitDialog() 
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

double CStldWindPressureAreaDlg_KBC2009_GustF::GetrfValue()
{
	double drf = 0.; // Value 확인용.
	double dIh = 0.;
	double dAlpha = 0.;
	double dZb = 0.;
	double dZg = 0.;

	GetrfFactor(dAlpha, dZb, dZg);
	
	dIh = 0.1 * pow(( m_dHeight / dZg ) , ( -dAlpha - 0.05 ));
	drf	= ((( 3 + 3 * dAlpha) / ( dAlpha + 2 ))) * dIh;

	return drf;
}

double CStldWindPressureAreaDlg_KBC2009_GustF::GetBfValue()
{
	double dBf = 0.;  //확인 사살용.

	double routine1 = 0.;
	double routine2 = 0.;

	double dk = GetBfFactor();
	
	if(m_dBreadth <= 1E-5)
		return 0.0;
	
	routine1 = pow(( 100 * sqrt(m_dHeight / 30) / sqrt(m_dHeight * m_dBreadth ) ) , (1.3) );
	routine2 = pow((m_dBreadth / m_dHeight) , dk);
		
	dBf = 1 - ( 1 / ( pow( ( 1 + 5.1 * routine1 * routine2 ), ( 1.0 / 3.0 ) ) ) );
	
	return dBf;
}

#define M_PI 3.14159265358979323846
double CStldWindPressureAreaDlg_KBC2009_GustF::GetRfValue()
{
	double dRf = 0.;
	double dSf = 0.;
	double dF = 0.;

	dSf = GetSfValue();
	dF  = GetFValue();

	dRf = M_PI / ( 4 * m_Data.dDampingRatio) * dSf * dF;

	return dRf;
}

double CStldWindPressureAreaDlg_KBC2009_GustF::GetSfValue()
{
	double dSf = 0.;
	double routine1 = 0.;
	double routine2 = 0.;
	double dVh = GetVhValue();
	
	routine1 = 1 + 2.1 * ( m_Data.dNaturalFreq * m_dHeight  / dVh );
	routine2 = 1 + 2.1 * ( m_Data.dNaturalFreq * m_dBreadth / dVh );

	dSf = 0.84 / ( routine1 * routine2 );

	return dSf;
}

double CStldWindPressureAreaDlg_KBC2009_GustF::GetFValue()
{
	double dF = 0.;
	double routine1 = 0.;
	double dVh = GetVhValue();
	double dLh = 100 * sqrt(m_dHeight / 30);

	routine1 = m_Data.dNaturalFreq * dLh / dVh;
		
	dF = 4 * routine1 / pow( (1 + 71 * pow(routine1, 2)) , (5.0 / 6.0) );

	return dF;
}

double CStldWindPressureAreaDlg_KBC2009_GustF::GetVhValue()
{
	double dVh = 0.;
	double Kzr = 0.;
	double Kzt = 0.;   // 우선 1로 가정.

	double dAlpha, dZb, dZg;
	double Kzr_low, Kzr_high;
	
	GetrfFactor(dAlpha, dZb, dZg);
	GetKzrFactor(Kzr_low, Kzr_high);

	if(m_dHeight <= dZb)
		Kzr = Kzr_low;
	else if((m_dHeight > dZb) && (m_dHeight <= dZg))
		Kzr = Kzr_high * pow(m_dHeight, dAlpha);
	else if(m_dHeight > dZg)
		Kzr = Kzr_high * pow(dZg, dAlpha);

	GetKztFactor(Kzt);

	dVh = m_Data.dBasicWindSpeed * Kzr * Kzt * m_Data.dImportanceFactor;

	return dVh;
}

void CStldWindPressureAreaDlg_KBC2009_GustF::GetKztFactor(double& dKzt)
{
	if(m_Data.dKzt <= 1.0) m_Data.dKzt = 1.0;   // 풍속할증계수의 최소값은 항상 1.0이어야 한다.
	
	if(m_Data.bTopoEffects)
	{
// 		if(m_dHeight >= m_Data.dHzt) dKzt = 1.0;
// 		else  dKzt = 1.0 + (m_Data.dKzt - 1.0) * (m_Data.dHzt - m_dHeight) / m_Data.dHzt;    
		dKzt = m_Data.dKzt;
	}
	else dKzt = 1.0;
}

void CStldWindPressureAreaDlg_KBC2009_GustF::GetKzrFactor(double& Kzr_low, double& Kzr_high)
{
	if(m_bKBC2016)
	{
		switch(m_Data.nExposureCategory)
		{
		case 1:	Kzr_low = 0.58;		Kzr_high = 0.22;	break;
		case 2:	Kzr_low = 0.81;		Kzr_high = 0.45;	break;
		case 3:	Kzr_low = 1.0;		Kzr_high = 0.71;	break;
		case 4:	Kzr_low = 1.13;		Kzr_high = 0.98;	break;
		}
	}
	else
	{
		switch(m_Data.nExposureCategory)
		{
		case 1:	Kzr_low = 0.58;		Kzr_high = 0.22;	break;
		case 2:	Kzr_low = 0.81;		Kzr_high = 0.45;	break;
		case 3:	Kzr_low = 1.0;		Kzr_high = 0.71;	break;
		case 4:	Kzr_low = 1.13;		Kzr_high = 0.97;	break;
		}
	}
}

double CStldWindPressureAreaDlg_KBC2009_GustF::GetgfValue()
{
	double dgf = 0.;
	double dVf = 0.;
	double dIh = 0.;
	double dRf = 0.;
	double dBf = 0.;

	if(m_bKBC2016)
	{
		dVf = m_Data.dNaturalFreq;
	}
	else
	{
		dRf = GetRfValue();
		dBf = GetBfValue();

		dVf = m_Data.dNaturalFreq * sqrt ( dRf / ( dBf + dRf ) );
	}
	
	dgf = sqrt( 2 * log(600 * dVf ) + 1.2);

	return dgf;
}

void CStldWindPressureAreaDlg_KBC2009_GustF::GetrfFactor(double& dAlpha, double& dZb, double& dZg)
{
	if(m_bKBC2016)
	{
		switch(m_Data.nExposureCategory)
		{
		case 1: dAlpha = 0.33,  dZb	= 20.0,	dZg  = 550.0; break;
		case 2: dAlpha = 0.22,	dZb	= 15.0,	dZg  = 450.0;	break;
		case 3: dAlpha = 0.15,	dZb	= 10.0,	dZg  = 350.0;	break;
		case 4: dAlpha = 0.10,	dZb	= 5.0,	dZg  = 250.0;	break;	
		}
	}
	else
	{
		switch(m_Data.nExposureCategory)
		{
		case 1: dAlpha = 0.33,  dZb	= 20.0,	dZg  = 500.0; break;
		case 2: dAlpha = 0.22,	dZb	= 15.0,	dZg  = 400.0;	break;
		case 3: dAlpha = 0.15,	dZb	= 10.0,	dZg  = 300.0;	break;
		case 4: dAlpha = 0.10,	dZb	= 5.0,	dZg  = 250.0;	break;	
		}
	}
}

double CStldWindPressureAreaDlg_KBC2009_GustF::GetBfFactor()
{
	double dk = 0.;  //확인 사살용.
	
	if(m_dHeight >= m_dBreadth)	dk = 0.33;
	else	dk = -0.33;

	return dk;
}

void CStldWindPressureAreaDlg_KBC2009_GustF::OnCmdWindgustCal() 
{
	Dlg2Data();

	double dHeight, dBreadth;
	dHeight		    = m_Data.dAverageRoofH;
	dBreadth		  = m_Data.dBreadth;

	// 단위를 m로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;	
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);

	switch(UnitIndex.nBase_Length)
	{
		case D_UNITSYS_LENGTH_INDEX_MM:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_MM;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_MM;
			break;
		case D_UNITSYS_LENGTH_INDEX_CM:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_CM;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_CM;
			break;
		case D_UNITSYS_LENGTH_INDEX_M:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_M;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_M;
			break;
		case D_UNITSYS_LENGTH_INDEX_IN:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_IN;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_IN;
			break;
		case D_UNITSYS_LENGTH_INDEX_FT:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_FT;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_FT;
			break;
	}
	
	if(m_dHeight <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei)); return;
	}
	if(m_dBreadth  <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_X_B)); return;
	}

	if(m_Data.nStructureType == 2) // Flexible Structure
	{
		if(m_Data.dNaturalFreq <= 1E-5) //X방향일때
	  {
		  AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_X_Natural_Fr)); return;
	  }
	  if(m_Data.dDampingRatio <= 1E-5) 
	  {
		  AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Damping_Rati)); return;
	  }
	}
	
	
	double drf = 0.0;
	double dBf = 0.0;
	double dgf = 0.0;
	double dRf = 0.0;

	drf = GetrfValue();
	dBf = GetBfValue();
	
	if(m_Data.nStructureType == 2) // Flexible Structure
	{
		dgf = GetgfValue();
		dRf = GetRfValue();
	}

	if (m_Data.nStructureType == 1) // Rigid Structure
	{
		m_dGustFactor = 1 + 4 * drf * sqrt(dBf);
	}
	else // Flexible Structure
	{
		m_dGustFactor = 1 + dgf * drf * sqrt(dBf + dRf);
	}

	DisplayGustFactor();
}

void CStldWindPressureAreaDlg_KBC2009_GustF::OnCmdWindgustOk() 
{
	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_KBC2009_GustF::OnCmdWindgustCancel() 
{
	CDialogMove::OnCancel();		
}

void CStldWindPressureAreaDlg_KBC2009_GustF::InitControl()
{
	m_edtBasicWindSpeed.SetUnitType(D_UNITSYS_BASE_VELOCITY); // m/sec
// 	m_stcBasicWindSpeed.SetUnitType(D_UNITSYS_BASE_VELOCITY); // m/sec
	m_edtImportanceFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtAverageRoofH.SetUnitType(D_UNITSYS_BASE_LENGTH); // m
	m_stcAverageRoofH.SetUnitType(D_UNITSYS_BASE_LENGTH); // m
	m_edtBreadth.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_stcBreadth.SetUnitType(D_UNITSYS_BASE_LENGTH);      // m
	m_edtNaturalFreq.SetUnitType(D_UNITSYS_NONE);  // Hz
	m_edtDampingRatio.SetUnitType(D_UNITSYS_NONE);

	m_edtGustFactor.SetUnitType(D_UNITSYS_NONE);
}

void CStldWindPressureAreaDlg_KBC2009_GustF::ShowHideControls()
{
	if (m_Data.nStructureType == 1) // Rigid Structure
	{
		GetDlgItem(IDC_TM_WINDGUST_TITLE4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_WINDGUST_EDIT4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_WINDGUST_UNIT4)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_TM_WINDGUST_TITLE5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TM_WINDGUST_EDIT5)->ShowWindow(SW_HIDE);
	}
	else if (m_Data.nStructureType == 2) // Flexible Structure
	{
		GetDlgItem(IDC_TM_WINDGUST_TITLE4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_WINDGUST_EDIT4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_WINDGUST_UNIT4)->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_TM_WINDGUST_TITLE5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TM_WINDGUST_EDIT5)->ShowWindow(SW_SHOW);
	}
	else
		ASSERT(0);
}

void CStldWindPressureAreaDlg_KBC2009_GustF::OnCmdRdoChanged()
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

BOOL CStldWindPressureAreaDlg_KBC2009_GustF::Dlg2Data()
{
	m_Data.dBreadth       = m_edtBreadth.GetEditValue();
	m_Data.dNaturalFreq   = m_edtNaturalFreq.GetEditValue();
	m_Data.dDampingRatio = m_edtDampingRatio.GetEditValue();

	return TRUE;
}

BOOL CStldWindPressureAreaDlg_KBC2009_GustF::Data2Dlg()
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

	CString strTopography;
	if(m_Data.bTopoEffects) strTopography = _LS(IDS_CMD_WIND_NBC95_INCLUDED);
	else                    strTopography = _LS(IDS_CMD_WIND_NBC95_NOT_INCLUDED);
	m_edtTopoEffects.SetWindowText(strTopography);

	m_edtAverageRoofH.SetEditUnit(m_Data.dAverageRoofH);
	m_edtBreadth.SetEditUnit(m_Data.dBreadth);
	m_edtNaturalFreq.SetEditUnit(m_Data.dNaturalFreq);
	m_edtDampingRatio.SetEditUnit(m_Data.dDampingRatio);
	DisplayGustFactor();

	return TRUE;  
}

void CStldWindPressureAreaDlg_KBC2009_GustF::DisplayGustFactor()
{
	if(m_dGustFactor > 0.0) m_edtGustFactor.SetEditUnit(m_dGustFactor, 4);
	else                    m_edtGustFactor.SetWindowText(_T(""));
}

BOOL CStldWindPressureAreaDlg_KBC2009_GustF::SetWindData( const T_WVEP_KBC2016& KBC2016, T_WDPR_D* pData )
{
	m_bKBC2016 = TRUE;

	m_Data.nExposureCategory = KBC2016.nExposureCategory;
	m_Data.dBasicWindSpeed   = KBC2016.dBasicWindSpeed;
	m_Data.dImportanceFactor = KBC2016.dImportanceFactor;
	m_Data.bTopoEffects      = KBC2016.bTopographicEffect;
	m_Data.dAverageRoofH     = KBC2016.dRoofHeight;
	m_Data.dKzt = KBC2016.dKzt;
	m_Data.dHzt = KBC2016.dVzt;

	m_Data.dDampingRatio  = pData->CodeParam.KBC2016.dDampingRatio;
	m_Data.nStructureType = pData->CodeParam.KBC2016.nRigidity;
	m_Data.dNaturalFreq   = pData->CodeParam.KBC2016.dNaturalFreq;
	m_dGustFactor         = pData->CodeParam.KBC2016.dGfExternal;
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

BOOL CStldWindPressureAreaDlg_KBC2009_GustF::SetWindData( const T_WVEP_KBC2009& KBC2009, T_WDPR_D* pData )
{
	m_bKBC2016 = FALSE;

	m_Data.nExposureCategory = KBC2009.nExposureCategory;
	m_Data.dBasicWindSpeed   = KBC2009.dBasicWindSpeed;
	m_Data.dImportanceFactor = KBC2009.dImportanceFactor;
	m_Data.bTopoEffects      = KBC2009.bTopographicEffect;
	m_Data.dAverageRoofH     = KBC2009.dRoofHeight;
	m_Data.dKzt = KBC2009.dKzt;
	m_Data.dHzt = KBC2009.dVzt;

	m_Data.dDampingRatio  = pData->CodeParam.KBC2009.dDampingRatio;
	m_Data.nStructureType = pData->CodeParam.KBC2009.nRigidity;
	m_Data.dNaturalFreq   = pData->CodeParam.KBC2009.dNaturalFreq;
	m_dGustFactor         = pData->CodeParam.KBC2009.dGfExternal;
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

void CStldWindPressureAreaDlg_KBC2009_GustF::GetDlgInfo(int& nRigidity, double& dBreath, double& dNatFreq, double& dDampingRatio, double& dGustFac)
{
	nRigidity      = m_Data.nStructureType;
	dBreath        = m_Data.dBreadth;
	dNatFreq       = m_Data.dNaturalFreq;
	dDampingRatio = m_Data.dDampingRatio;
	dGustFac       = m_dGustFactor;
}