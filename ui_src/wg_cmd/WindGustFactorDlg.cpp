// WindGustFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindGustFactorDlg.h"

#include <math.h>
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindGustFactorDlg dialog
CWindGustFactorDlg::CWindGustFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindGustFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindGustFactorDlg)
	// m_GustFactorDirction = 0;
	m_dWidth = 0.0;
	m_dBreadth = 0.0;	
	m_dFrequencyX = 0.0;
	m_dFrequencyY = 0.0;
	m_dDamping = 0.0;

	//}}AFX_DATA_INIT

	m_bDirection = TRUE;
	m_dGustFactorX = 0.0;
	m_dGustFactorY = 0.0;
	
	// m_bDirection = TRUE;  // 1 for X, 0 for Y
}


void CWindGustFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindGustFactorDlg)
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT3, m_wndBreadthYUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT2, m_wndBreadthXUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT1, m_wndBldgHeightUnit);	
	DDX_Control(pDX, IDC_CMD_KOR2000_TOPOGRAPHY_EDIT, m_wndTopography);	
	DDX_Control(pDX, IDC_CMD_KOR2000_IF_EDIT, m_wndKor2000If);
	DDX_Control(pDX, IDC_CMD_KOR2000_BWS_EDIT, m_wndKor2000Bws);
	DDX_Control(pDX, IDC_CMD_KOR2000_EC_COMBO, m_wndKor2000ECcbo);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT6, m_edtFrequencyY);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT5, m_edtDampingRatio);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT4, m_edtFrequencyX);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT3, m_edtBreadthY);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT2, m_edtBreadthX);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT1, m_edtHeight);
	// DDX_Radio(pDX, IDC_CMD_WINDGUST_GUST_RDO1, m_GustFactorDirction);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindGustFactorDlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindGustFactorDlg)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CAL, OnCmdWindgustCal)
	// ON_BN_CLICKED(IDC_CMD_WINDGUST_GUST_RDO1, OnCmdWindgustGustRdo)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_OK, OnCmdWindgustOk)
	// ON_BN_CLICKED(IDC_CMD_WINDGUST_GUST_RDO2, OnCmdWindgustGustRdo)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CANCEL, OnCmdWindgustCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindGustFactorDlg message handlers

BOOL CWindGustFactorDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	// ((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->EnableWindow(m_GustFactorDirction == 1);
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT1))->SetWindowText(_T(""));
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->SetWindowText(_T(""));

	InitialData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindGustFactorDlg::InitialData()
{
	m_edtHeight.SetEditUnit(m_dHeight);
	m_edtBreadthX.SetEditUnit(m_dWidth);
	m_edtBreadthY.SetEditUnit(m_dBreadth);
	m_edtFrequencyX.SetEditUnit(m_dFrequencyX);
	m_edtFrequencyY.SetEditUnit(m_dFrequencyY);
	m_edtDampingRatio.SetEditUnit(m_dDamping);	

	m_wndBreadthYUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);  
	m_wndBreadthXUnit.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_wndBldgHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);   

	CString strTemp;

	switch(m_nExposure)
	{
		case 0: strTemp = _T("A"); break;
		case 1: strTemp = _T("B"); break;
		case 2: strTemp = _T("C"); break;
		case 3: strTemp = _T("D"); break;
	}
	m_wndKor2000ECcbo.SetWindowText(strTemp);

	strTemp.Format(_T("%g"), m_dSpeed);
	m_wndKor2000Bws.SetWindowText(strTemp);

	strTemp.Format(_T("%g"), m_dImportance);
	m_wndKor2000If.SetWindowText(strTemp);

	CString strTopography;
	if(m_bTopography) strTopography = _LS(IDS_CMD_WIND_NBC95_INCLUDED);
	else              strTopography = _LS(IDS_CMD_WIND_NBC95_NOT_INCLUDED);

	m_wndTopography.SetWindowText(strTopography);	

	UpdateData(FALSE); 

}

double CWindGustFactorDlg::GetrfValue()
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

double CWindGustFactorDlg::GetBfValue(BOOL bDirection)
{
	double dBf = 0.;  //확인 사살용.

	double routine1 = 0.;
	double routine2 = 0.;

	double dk = GetBfFactor(bDirection);

	if(bDirection)
	{
		routine1 = pow(( 100 * sqrt(m_dHeight/30) / sqrt(m_dHeight* m_dWidth ) ) , (1.3) );
		routine2 = pow((m_dWidth / m_dHeight) , dk);
	}
	else
	{
		routine1 = pow(( 100 * sqrt(m_dHeight/30) / sqrt(m_dHeight* m_dBreadth ) ) , (1.3) );
		routine2 = pow((m_dBreadth / m_dHeight) , dk);
	}

	dBf = 1 - ( 1 / ( pow( ( 1 + 5.1*routine1*routine2 ), ( 1./3. ) )  ) );

	return dBf;
}

#define M_PI 3.14159265358979323846
double CWindGustFactorDlg::GetRfValue(BOOL bDirection)
{
	double dRf = 0.;
	double dSf = 0.;
	double dF = 0.;

	dSf = GetSfValue(bDirection);
	dF = GetFValue(bDirection);

	dRf = M_PI / ( 4 * m_dDamping) * dSf * dF;

	return dRf;
}

double CWindGustFactorDlg::GetSfValue(BOOL bDirection)
{
	double dSf = 0.;
	double routine1 = 0.;
	double routine2 = 0.;
	double dVh = GetVhValue();

	if(bDirection)
	{
		routine1 = 1 + 2.1 * ( m_dFrequencyX * m_dHeight / dVh );
		routine2 = 1 + 2.1 * ( m_dFrequencyX * m_dWidth / dVh );
	}
	else
	{
		routine1 = 1 + 2.1 * ( m_dFrequencyY * m_dHeight / dVh );
		routine2 = 1 + 2.1 * ( m_dFrequencyY * m_dBreadth / dVh );
	}

	dSf = 0.84 / ( routine1 * routine2 );

	return dSf;
}

double CWindGustFactorDlg::GetFValue(BOOL bDirection)
{
	double dF = 0.;
	double routine1 = 0.;
	double dVh = GetVhValue();
	double dLh = 100 * sqrt(m_dHeight / 30);

	if(bDirection)
		routine1 = m_dFrequencyX * dLh / dVh;
	else
		routine1 = m_dFrequencyY * dLh / dVh;

	dF = 4*routine1 / pow( (1+71*pow(routine1,2)) , (5./6.) );

	return dF;
}

double CWindGustFactorDlg::GetVhValue()
{
	double dVh = 0.;
	double Kzr = 0.;
	double Kzt = 0.;   // 우선 1로 가정.

	double dAlpha, dZb, dZg;
	double Kzr_low, Kzr_high;
	
	GetrfFactor(dAlpha, dZb, dZg);
	GetKzrFactor(Kzr_low,Kzr_high);

	if(m_dHeight <= dZb)
		Kzr = Kzr_low;
	else if((m_dHeight > dZb) && (m_dHeight <= dZg))
		Kzr = Kzr_high * pow(m_dHeight, dAlpha);
	else if(m_dHeight > dZg)
		Kzr = Kzr_high * pow(dZg, dAlpha);

	GetKztFactor(Kzt);

	dVh = m_dSpeed * Kzr * Kzt * m_dImportance;

	return dVh;
}

void CWindGustFactorDlg::GetKztFactor(double& dKzt)
{
	if(m_dKzt <= 1.0) m_dKzt = 1.0;   // 풍속할증계수의 최소값은 항상 1.0이어야 한다.
	
	if(m_bTopography)
	{
		if(m_dHeight >= m_dHzt) dKzt = 1.0;
		else  dKzt = 1.0 + (m_dKzt - 1.0) * (m_dHzt - m_dHeight) / m_dHzt;    
	}
	else dKzt = 1.0;
}

void CWindGustFactorDlg::GetKzrFactor(double& Kzr_low, double& Kzr_high)
{
	switch(m_nExposure + 65)
	{
		case 'A':	Kzr_low = 0.58;		Kzr_high = 0.22;	break;
		case 'B':	Kzr_low = 0.81;		Kzr_high = 0.45;	break;
		case 'C':	Kzr_low = 1.0;		Kzr_high = 0.71;	break;
		case 'D':	Kzr_low = 1.13;		Kzr_high = 0.97;	break;
	}
}

double CWindGustFactorDlg::GetgfValue(BOOL bDirection)
{
	double dgf = 0.;
	double dVf = 0.;
	double dIh = 0.;
	double dRf = 0.;
	double dBf = 0.;

	dRf = GetRfValue(bDirection);
	dBf = GetBfValue(bDirection);
		
	if(bDirection) dVf = m_dFrequencyX * sqrt ( dRf / ( dBf + dRf ) );
	else		       dVf = m_dFrequencyY * sqrt ( dRf / ( dBf + dRf ) );
	
	dgf = sqrt( 2 * log(600 * dVf) + 1.2);

	return dgf;
}

void CWindGustFactorDlg::GetrfFactor(double& dAlpha, double& dZb, double& dZg)
{
	switch(m_nExposure + 65)
	{
		case 'A': dAlpha = 0.33, dZb	= 20.0,	dZg  = 500.0; break;
		case 'B': dAlpha = 0.22,	dZb	= 15.0,	dZg  = 400.0;	break;
		case 'C': dAlpha = 0.15,	dZb	= 10.0,	dZg  = 300.0;	break;
		case 'D': dAlpha = 0.10,	dZb	= 5.0,	dZg  = 250.0;	break;	
	}
}

double CWindGustFactorDlg::GetBfFactor(BOOL bDirection)
{
	double dk = 0.;  //확인 사살용.

	if(bDirection)
	{
		if(m_dHeight >= m_dWidth)	dk = 0.33;
		else	dk = -0.33;
	}
	else
	{
		if(m_dHeight >= m_dBreadth)	dk = 0.33;
		else	dk = -0.33;
	}
	return dk;
}

void CWindGustFactorDlg::OnCmdWindgustCal() 
{
	double dHeight, dWidth, dBreadth;
	dHeight		    = m_edtHeight.GetEditValue();
	dWidth			  = m_edtBreadthX.GetEditValue();
	dBreadth		  = m_edtBreadthY.GetEditValue();
	m_dFrequencyX = m_edtFrequencyX.GetEditValue();
	m_dFrequencyY = m_edtFrequencyY.GetEditValue();
	m_dDamping		= m_edtDampingRatio.GetEditValue();

	// 단위를 m로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;	
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);

	switch(UnitIndex.nBase_Length)
	{
		case D_UNITSYS_LENGTH_INDEX_MM:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_MM;
			m_dWidth  = dWidth  * D_UNITSYS_LENGTH_FACTOR_MM;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_MM;
			break;
		case D_UNITSYS_LENGTH_INDEX_CM:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_CM;
			m_dWidth  = dWidth  * D_UNITSYS_LENGTH_FACTOR_CM;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_CM;
			break;
		case D_UNITSYS_LENGTH_INDEX_M:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_M;
			m_dWidth  = dWidth  * D_UNITSYS_LENGTH_FACTOR_M;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_M;
			break;
		case D_UNITSYS_LENGTH_INDEX_IN:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_IN;
			m_dWidth  = dWidth  * D_UNITSYS_LENGTH_FACTOR_IN;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_IN;
			break;
		case D_UNITSYS_LENGTH_INDEX_FT:
			m_dHeight = dHeight * D_UNITSYS_LENGTH_FACTOR_FT;
			m_dWidth  = dWidth  * D_UNITSYS_LENGTH_FACTOR_FT;
			m_dBreadth= dBreadth* D_UNITSYS_LENGTH_FACTOR_FT;
			break;
	}
	
	if(m_dHeight <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei)); return;
	}
	if(m_dWidth  <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_X_B)); return;
	}
	if(m_dBreadth <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Y_B)); return;
	}
	if(m_dFrequencyX <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_X_Natural_Fr)); return;
	}
	if(m_dFrequencyY <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Y_Natural_Fr)); return;
	}
	if(m_dDamping <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Damping_Rati)); return;
	}
	
	double drf=0.;
	double dBfX=0.,dBfY=0.;
	double dgfX=0.,dgfY=0.;
	double dRfX=0.,dRfY=0.;

	drf = GetrfValue();
	dgfX = GetgfValue(TRUE);
	dgfY = GetgfValue(FALSE);
	dBfX = GetBfValue(TRUE);
	dBfY = GetBfValue(FALSE);
	dRfX = GetRfValue(TRUE);
	dRfY = GetRfValue(FALSE);

	m_dGustFactorX = 1 + dgfX * drf * sqrt(dBfX + dRfX);
	m_dGustFactorY = 1 + dgfY * drf * sqrt(dBfY + dRfY);

	CString strTemp;
	strTemp.Format(_T("%f"), m_dGustFactorX);
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT1))->SetWindowText(strTemp);
	strTemp.Format(_T("%f"), m_dGustFactorY);
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->SetWindowText(strTemp);
}

/*
void CWindGustFactorDlg::OnCmdWindgustGustRdo() 
{
	UpdateData(TRUE);
	
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT1))->EnableWindow(m_GustFactorDirction == 0);
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->EnableWindow(m_GustFactorDirction == 1);
}
*/

void CWindGustFactorDlg::OnCmdWindgustOk() 
{
	UpdateData(TRUE);

	// m_bDirection	= m_GustFactorDirction;
	m_dWidth			= m_edtBreadthX.GetEditValue();
	m_dBreadth		= m_edtBreadthY.GetEditValue();	
	m_dFrequencyX	= m_edtFrequencyX.GetEditValue();
	m_dFrequencyY	= m_edtFrequencyY.GetEditValue();
	m_dDamping		= m_edtDampingRatio.GetEditValue();
/*	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;
	
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);

	switch(UnitIndex.nBase_Length)
	{
		case D_UNITSYS_LENGTH_INDEX_MM:			
			m_dWidth  = m_dWidth  / D_UNITSYS_LENGTH_FACTOR_MM;
			m_dBreadth= m_dBreadth/ D_UNITSYS_LENGTH_FACTOR_MM;
			break;
		case D_UNITSYS_LENGTH_INDEX_CM:			
			m_dWidth  = m_dWidth  / D_UNITSYS_LENGTH_FACTOR_CM;
			m_dBreadth= m_dBreadth/ D_UNITSYS_LENGTH_FACTOR_CM;
			break;
		case D_UNITSYS_LENGTH_INDEX_M:			
			m_dWidth  = m_dWidth  / D_UNITSYS_LENGTH_FACTOR_M;
			m_dBreadth= m_dBreadth/ D_UNITSYS_LENGTH_FACTOR_M;
			break;
		case D_UNITSYS_LENGTH_INDEX_IN:			
			m_dWidth  = m_dWidth  / D_UNITSYS_LENGTH_FACTOR_IN;
			m_dBreadth= m_dBreadth/ D_UNITSYS_LENGTH_FACTOR_IN;
			break;
		case D_UNITSYS_LENGTH_INDEX_FT:			
			m_dWidth  = m_dWidth  / D_UNITSYS_LENGTH_FACTOR_FT;
			m_dBreadth= m_dBreadth/ D_UNITSYS_LENGTH_FACTOR_FT;
			break;
	}
*/	
	CDialogMove::OnOK();
}

void CWindGustFactorDlg::OnCmdWindgustCancel() 
{
	CDialogMove::OnCancel();		
}
