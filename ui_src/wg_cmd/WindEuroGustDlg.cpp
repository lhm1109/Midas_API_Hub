// WindEuroGustDlg.cpp : implementation file
//
#include "stdafx.h"
#include <math.h>
#include "wg_cmd.h"
#include "WindEuroGustDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CWindEuroGustDlg dialog


CWindEuroGustDlg::CWindEuroGustDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindEuroGustDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindEuroGustDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWindEuroGustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindEuroGustDlg)
	DDX_Control(pDX, IDC_CMD_EURO_GUST_TOPO_EDIT, m_wndEuroGustTopography);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_UNIT5, m_wndEuroGustDepthUnit);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_UNIT4, m_wndEuroGustBreadthUnit);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_UNIT3, m_wndEuroGustHeightUnit);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT8, m_wndEuroGustFactor);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT7, m_wndEuroGustDamping);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT6, m_wndEuroGustFrequency);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT5, m_wndEuroGustDepth);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT4, m_wndEuroGustBreadth);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT3, m_wndEuroGustHeight);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT2, m_wndEuroGustVref);
	DDX_Control(pDX, IDC_CMD_EURO_GUST_EDIT1, m_wndEuroGustRoughCategory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindEuroGustDlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindEuroGustDlg)
	ON_BN_CLICKED(IDC_CMD_EURO_GUST_OK, OnCmdEuroGustOk)
	ON_BN_CLICKED(IDC_CMD_EURO_GUST_CANCEL, OnCmdEuroGustCancel)
	ON_BN_CLICKED(IDC_CMD_EURO_GUST_CAL, OnCmdEuroGustCal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindEuroGustDlg message handlers

BOOL CWindEuroGustDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strRoughCategory, strTopography;
	switch(m_nRoughCategory)
	{
		case 0: strRoughCategory = _T("I");   break;
		case 1: strRoughCategory = _T("II");  break;
		case 2: strRoughCategory = _T("III"); break;
		case 3: strRoughCategory = _T("IV");  break;
	}
	    
	if(m_bTopography) strTopography = _LS(IDS_CMD_WIND_NBC95_INCLUDED);
	else              strTopography = _LS(IDS_CMD_WIND_NBC95_NOT_INCLUDED);

	m_wndEuroGustHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);  
	m_wndEuroGustBreadth.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_wndEuroGustDepth.SetUnitType(D_UNITSYS_BASE_LENGTH);   

	m_wndEuroGustRoughCategory.SetWindowText(strRoughCategory);
	m_wndEuroGustVref.SetEditUnit(m_dVref);
	m_wndEuroGustHeight.SetEditUnit(m_dH);  
	m_wndEuroGustBreadth.SetEditUnit(m_dBB); 
	m_wndEuroGustDepth.SetEditUnit(m_dBD);    
	m_wndEuroGustFrequency.SetEditUnit(m_dFreq);
	m_wndEuroGustDamping.SetEditUnit(m_dDamping);  
	m_wndEuroGustTopography.SetWindowText(strTopography);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CWindEuroGustDlg::OnCmdEuroGustCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();		
}

void CWindEuroGustDlg::OnCmdEuroGustCal() 
{
	// TODO: Add your control notification handler code here
	double Gfactor=0.0;
	double Kr=0.0, Z0=0.0, Zmin=0.0, Epsilon=0.0;

	Gfactor = 0.0;
	switch(m_nRoughCategory)
	{
		case 0: Kr = 0.17, Z0 = 0.01, Zmin =  2, Epsilon = 0.13; break;
		case 1: Kr = 0.19, Z0 = 0.05, Zmin =  4, Epsilon = 0.26; break;
		case 2: Kr = 0.22, Z0 = 0.30, Zmin =  8, Epsilon = 0.37; break;
		case 3: Kr = 0.24, Z0 = 1.00, Zmin = 16, Epsilon = 0.46; break;
		default: AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Exposure_Category_Error_)); return;
	}

	double height=0.0, width=0.0, depth=0.0, n1=0.0, delta=0.0, Ze=0.0, Lv=0.0, Cr=0.0, Q0=0.0, Li=0.0, Rx=0.0, Rn=0.0, Vm=0.0, Nx=0.0; 
	double NxLsb=0.0, NxLsh=0.0, NxLsd=0.0, Rb=0.0, Rh=0.0, Rd=0.0, Lsb=0.0, Lsh=0.0, Lsd=0.0;

	height = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, m_wndEuroGustHeight.GetEditValue()); // dialog input
	width	 = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, m_wndEuroGustBreadth.GetEditValue()); // dialog input
	depth  = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, m_wndEuroGustDepth.GetEditValue()); // dialog input
	n1		 = m_wndEuroGustFrequency.GetEditValue();
	delta  = m_wndEuroGustDamping.GetEditValue();

	Ze = 0.6 * height;
	if(Ze < Zmin)	Ze = Zmin;
		
	if(delta <= 0)
	{		
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_value_of_Fundamental_Damping));
		return;
	}

	Cr = Kr * log(Ze/Z0);
	
	double dCt = GetTopographyFactor(Ze);
	
	Lv = 0.2 / (Cr * dCt);

	Li = 300 * pow( (Ze / 300), Epsilon);
	if (Li >300 ) Li = 300;

	Q0 = sqrt(1. / (1+ 0.9 * pow(((width + height) / Li), 0.63)));
		
	Vm = Cr * dCt * m_dVref;

	if(Vm == 0 || dCt ==0)   // Design wind velocity가 0 일때는 gust factor는 0 이 됨
	{													 // Topography Coefficient가 0 일때는 gust factor는 0 이 됨
		Gfactor = 0; 
		return;
	}
	
	Nx = n1 * Li / Vm;
	Rn = 33.9 * Nx / pow((1 + 10.3 * Nx), 5/3);
	Lsb = 11.5 * width / Li;
	Lsh = 11.5 * height / Li;
	Lsd = 38.5 * depth / Li;
	NxLsb = Nx * Lsb;
	NxLsh = Nx * Lsh;
	NxLsd = Nx * Lsd;
	if(NxLsb > 0) Rb = 2.5 / (NxLsb) - 3.12 * (1 - exp(-0.8*NxLsb)) / pow(NxLsb, 2);
	else if(NxLsb == 0) Rb = 1;
	
	if(NxLsh > 0) Rh = 2.5 / (NxLsh) - 3.12 * (1 - exp(-0.8*NxLsh)) / pow(NxLsh, 2);
	else if(NxLsh == 0) Rh = 1;
	
	if(NxLsd > 0) Rd = 2.5 / (NxLsd) - 3.12 * (1 - exp(-0.8*NxLsd)) / pow(NxLsd, 2);
	else if(NxLsd == 0) Rd = 1;

	Rx = sqrt((Rn * Rb * Rh / delta) * ( 0.53 + 0.47 * Rd));
	Gfactor = 1 + 7.0 * Lv * sqrt(Q0 * Q0 + Rx * Rx);

	m_wndEuroGustFactor.SetEditUnit(Gfactor);
}

double CWindEuroGustDlg::GetTopographyFactor(double Ze)
{
	double dCt;
	if(Ze <= 0) return 1.0;
	if(m_dCt <= 1.0) return 1.0;

	if(m_bTopography)
	{
		if(Ze >= m_dCtRange) dCt = 1.0;
		else dCt = 1.0 + (m_dCt - 1.0) * (m_dCtRange - Ze) / m_dCtRange;    
	}
	else dCt = 1.0;

	return dCt;
}

double CWindEuroGustDlg::CvtUnit(int nForce, int nLength, int nUnitType, double dblData)
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

void CWindEuroGustDlg::OnCmdEuroGustOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_dH					= m_wndEuroGustHeight.GetEditValue();   
	m_dBB					= m_wndEuroGustBreadth.GetEditValue();  
	m_dBD					= m_wndEuroGustDepth.GetEditValue();    
	m_dFreq				= m_wndEuroGustFrequency.GetEditValue();
	m_dDamping		= m_wndEuroGustDamping.GetEditValue();  
	m_dGustFactor	= m_wndEuroGustFactor.GetEditValue();   

	CDialogMove::OnOK();	
}
