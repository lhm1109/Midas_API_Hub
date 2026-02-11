// WindGustNbc95Dlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "wg_cmd.h"
#include "WindGustNbc95Dlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindGustNbc95Dlg dialog
CWindGustNbc95Dlg::CWindGustNbc95Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindGustNbc95Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindGustNbc95Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWindGustNbc95Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindGustNbc95Dlg)
	DDX_Control(pDX, IDC_CMD_NBC95_TOPOGRAPHY_EDIT, m_wndTopography);
	DDX_Control(pDX, IDC_CMD_NBC95_WIDTH_Y_UNIT, m_wndBreadthYUnit);
	DDX_Control(pDX, IDC_CMD_NBC95_WIDTH_X_UNIT, m_wndBreadthXUnit);
	DDX_Control(pDX, IDC_CMD_NBC95_BUILDING_HEIGHT_UNIT, m_wndBldgHeightUnit);
	DDX_Control(pDX, IDC_CMD_NBC95_WIND_SPEED_EDIT, m_wndWindSpeed);
	DDX_Control(pDX, IDC_CMD_NBC95_WIDTH_Y_EDIT, m_wndBreadthY);
	DDX_Control(pDX, IDC_CMD_NBC95_WIDTH_X_EDIT, m_wndBreadthX);
	DDX_Control(pDX, IDC_CMD_NBC95_GUST_Y_EDIT, m_wndGustY);
	DDX_Control(pDX, IDC_CMD_NBC95_GUST_X_EDIT, m_wndGustX);
	DDX_Control(pDX, IDC_CMD_NBC95_FREQUENCY_Y_EDIT, m_wndFrequencyY);
	DDX_Control(pDX, IDC_CMD_NBC95_FREQUENCY_X_EDIT, m_wndFrequencyX);
	DDX_Control(pDX, IDC_CMD_NBC95_EXPOSURE_EDIT, m_wndExposure);
	DDX_Control(pDX, IDC_CMD_NBC95_DAMPING_EDIT, m_wndDamping);
	DDX_Control(pDX, IDC_CMD_NBC95_BUILDING_HEIGHT_EDIT, m_wndBldgHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindGustNbc95Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindGustNbc95Dlg)
	ON_BN_CLICKED(IDC_CMD_NBC95_CALCULATE_BTN, OnCmdNbc95CalculateBtn)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_OK, OnCmdWindgustOk)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CANCEL, OnCmdWindgustCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindGustNbc95Dlg message handlers

BOOL CWindGustNbc95Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CString strExposure, strTopography;
	switch(m_nExposure)
	{
		case 0: strExposure = _T("A"); break;
		case 1: strExposure = _T("B"); break;
		case 2: strExposure = _T("C"); break;		
	}
	if(m_bTopography) strTopography = _LS(IDS_CMD_WIND_NBC95_INCLUDED);
	else              strTopography = _LS(IDS_CMD_WIND_NBC95_NOT_INCLUDED);

	m_wndBreadthYUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);  
	m_wndBreadthXUnit.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_wndBldgHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);   

	m_wndWindSpeed.SetEditUnit(m_dWindSpeed);
	m_wndExposure.SetWindowText(strExposure);
	m_wndTopography.SetWindowText(strTopography);	
	m_wndBldgHeight.SetEditUnit(m_dBldgHeight);  
	m_wndBreadthX.SetEditUnit(m_dBreadthX); 
	m_wndBreadthY.SetEditUnit(m_dBreadthY);    
	m_wndFrequencyY.SetEditUnit(m_dFrequencyX);
	m_wndFrequencyX.SetEditUnit(m_dFrequencyY);
	m_wndDamping.SetEditUnit(m_dDamping);  
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindGustNbc95Dlg::OnCmdNbc95CalculateBtn() 
{  
	double dBldgHeight, dBreadthX, dBreadthY, dHillHeight, dHillLength, dHillDistance, nX, nY, dBeta;
	dBldgHeight   = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_wndBldgHeight.GetEditValue()); // dialog input
	dBreadthX     = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_wndBreadthX.GetEditValue()); // dialog input
	dBreadthY     = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_wndBreadthY.GetEditValue()); // dialog input
	dHillHeight   = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_dHillHeight); 
	dHillLength   = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_dHillLength); 
	dHillDistance = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, m_dHillDistance); 
	nX		        = m_wndFrequencyX.GetEditValue();
	nY		        = m_wndFrequencyY.GetEditValue();
	dBeta         = m_wndDamping.GetEditValue();

	if(dBldgHeight <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei)); return;
	}
	if(dBreadthX  <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_X_B)); return;
	}
	if(dBreadthY <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Y_B)); return;
	}	
	if(nX <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_X_Natural_Fr)); return;
	}
	if(nY <= 0) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Y_Natural_Fr)); return;
	}	
	if(dBeta <= 0)
	{		
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Damping_Rati)); return;
	}

	double CeH = 0.;  // Exposure Factor at the top of the Building
	CalcExposureFactor(m_bTopography, dBldgHeight, m_nExposure, m_nHillShape, m_nBldgLocation, 
										 dHillHeight, dHillLength, dHillDistance, CeH);
	
	double GfactorX, GfactorY;
	CalcGustFactor(CeH, m_nExposure, m_dWindSpeed, dBldgHeight, dBreadthX, nX, dBeta, GfactorX);
	CalcGustFactor(CeH, m_nExposure, m_dWindSpeed, dBldgHeight, dBreadthY, nY, dBeta, GfactorY);

	m_wndGustX.SetEditUnit(GfactorX);	
	m_wndGustY.SetEditUnit(GfactorY);	
}

double CWindGustNbc95Dlg::CalcExposureFactor(BOOL bTopography, double dBldgHeight, int nExposure, 
				int nHillShape, int nBldgLocation, double dHillHeight, double dHillLength, double dHillDistance, 
				double& dCeH)
{  
	switch(nExposure)
	{
		case 0: dCeH = pow(dBldgHeight/10., 0.28);         dCeH = __max(dCeH, 1.0); break;
		case 1: dCeH = 0.5 * pow(dBldgHeight/12.7, 0.50);  dCeH = __max(dCeH, 0.5); break;
		case 2: dCeH = 0.4 * pow(dBldgHeight/30.0, 0.72);  dCeH = __max(dCeH, 0.4); break;
	}

	if(bTopography)
	{
		double alpha, Smax, HoverL;  
		if(dHillLength == 0 || dHillHeight == 0) return 1;    
		if(dHillLength < 2.0 * dHillHeight) dHillLength = 2.0 * dHillHeight;
		HoverL = dHillHeight / dHillLength;    
	
		switch(nHillShape)
		{
			case 0 : alpha = 3.0, Smax = 2.2 * HoverL; break;
			case 1 : alpha = 2.5, Smax = 1.3 * HoverL; break;
			case 2 : alpha = 4.0, Smax = 1.6 * HoverL; break;
		}

		double k = 1.5;  
		if(nHillShape == 1 && nBldgLocation == 1) k = 4.0;

		double dCeHFactor = 1.0;
		if(fabs(dHillDistance) < k * dHillLength)
		{
			dCeHFactor = 1 + Smax * (1 - fabs(dHillDistance) / (k * dHillLength)) * 
									 pow(2.71828, - alpha * dBldgHeight / dHillLength); 
			dCeHFactor = dCeHFactor * dCeHFactor;
		}
		dCeH = dCeH * dCeHFactor;
	}
	return dCeH;
}

double CWindGustNbc95Dlg::CalcGustFactor(double dCeH, int nExposure, double dWindSpeed, 
					 double dBldgHeight, double dBldgWidth, double dno, double beta, double& Gfactor) 
{
	double K = 0.;    // a factor related to the surface roughness coefficient of terrain  
	switch(nExposure)
	{
		case 0: K = 0.08;  break;
		case 1: K = 0.10;  break;
		case 2: K = 0.14;  break;
	}
		 
	double F = 0.;     // gust enengy ratio at the natural frequency of the structure
	double xo, Vh;
	Vh = dWindSpeed * sqrt(dCeH);   // mean wind speed[m/sec] at the top of structure
	if(Vh == 0) return 0;
	xo = 1220. * dno / Vh;
	F = xo * xo / pow(1 + xo * xo, 4./3.);

	double s;     // size effect factor
	s = (3.141593 / 3.) / (1 + 8 * dno * dBldgHeight / (3 * Vh)) / (1 + 10 * dno * dBldgWidth / Vh);
	
	double B = 0; // background turbulence factor
	double delta = 914.0 / dBldgHeight / 1000.;  
	double x = 0;
	double f = 0;
	for(int i = 0; i < 1000; i++)
	{
		x = delta * i;
		f = (x / pow(1 + x * x, 4. / 3.)) / (1 + x * dBldgHeight / 457.) / (1 + x * dBldgWidth / 122.);  
		B = B + delta * f;
	}
	B = B * (4. / 3.);
	double gp;    // statistical peak factor for the loading effect
	double mu;
	mu = dno * sqrt(s * F / (s * F + beta * B));
	gp = sqrt(2 * log(mu * 3600)) + 0.577 / sqrt(2 * log(mu * 3600));
	
	Gfactor = 1 + gp * sqrt((K / dCeH)  * (B + s * F / beta)); 
	return Gfactor;
}

double CWindGustNbc95Dlg::CvtUnit(int nForce, int nLength, int nUnitType, double dblData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX OrgUnit, TgtUnit;

	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(OrgUnit);

	TgtUnit.nBase_Force = nForce;
	TgtUnit.nBase_Length = nLength;
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(TgtUnit, FALSE);
	double dTgtVal = pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitType, dblData);
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(OrgUnit, FALSE);
	return dTgtVal;
}
	
void CWindGustNbc95Dlg::OnCmdWindgustOk() 
{
	UpdateData();
	m_dBreadthX    = m_wndBreadthX.GetEditValue();   
	m_dBreadthY    = m_wndBreadthY.GetEditValue();  
	m_dFrequencyX  = m_wndFrequencyY.GetEditValue();    
	m_dFrequencyY  = m_wndFrequencyX.GetEditValue();
	m_dDamping     = m_wndDamping.GetEditValue();  
	m_dGustFactorX = m_wndGustX.GetEditValue();
	m_dGustFactorY = m_wndGustY.GetEditValue();

	CDialogMove::OnOK();	
}

void CWindGustNbc95Dlg::OnCmdWindgustCancel() 
{
	CDialogMove::OnCancel();			
}
