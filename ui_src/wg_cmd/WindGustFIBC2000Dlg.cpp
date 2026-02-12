// WindGustFIBC2000Dlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "wg_cmd.h"
#include "WindGustFIBC2000Dlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"  
#include "..\wg_db\Wind_NSCP2024.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindGustFIBC2000Dlg dialog
CWindGustFIBC2000Dlg::CWindGustFIBC2000Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindGustFIBC2000Dlg::IDD, pParent)
{
	m_nCalcType = IBC;
	//{{AFX_DATA_INIT(CWindGustFIBC2000Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWindGustFIBC2000Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindGustFIBC2000Dlg)
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_UNIT3, m_YBreadthUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_UNIT2, m_XBreadthUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_UNIT1, m_heightUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_FEDIT2, m_YFactor);
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_FEDIT1, m_XFactor);
	DDX_Control(pDX, IDC_CMD_IBC2000_EC_COMBO, m_Exposure);
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_EDIT2, m_XBreadth);
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_EDIT3, m_YBreadth);
	DDX_Control(pDX, IDC_CMD_WINDGUST_RIGID_EDIT1, m_height);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindGustFIBC2000Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindGustFIBC2000Dlg)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CANCEL, OnCmdWindgustCancel)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_OK, OnCmdWindgustOk)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_RIGID_CAL, OnCmdWindgustRigidCal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindGustFIBC2000Dlg message handlers
BOOL CWindGustFIBC2000Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_height.SetEditUnit(m_dheight);
	m_XBreadth.SetEditUnit(m_dBX);
	m_YBreadth.SetEditUnit(m_dBY);
	m_heightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_XBreadthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_YBreadthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	CString str;
	switch(m_nExposure)
	{
		case 0: str = _T("A"); break;
		case 1: str = _T("B"); break;
		case 2: str = _T("C"); break;
		case 3: str = _T("D"); break;
	}
	m_Exposure.SetWindowText(str);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindGustFIBC2000Dlg::OnCmdWindgustCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();	
}

void CWindGustFIBC2000Dlg::OnCmdWindgustRigidCal() 
{
	const double dDlgH = m_height.GetEditValue();
	const double dDlgX = m_XBreadth.GetEditValue();
	const double dDlgY = m_YBreadth.GetEditValue();

	double xfactor = 0.0;
	double yfactor = 0.0;

	switch (m_nCalcType)
	{
	case CWindGustFIBC2000Dlg::IBC:
	case CWindGustFIBC2000Dlg::NSR:
		{			
			const double height = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, dDlgH);
			const double xBreadth = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, dDlgX);
			const double yBreadth = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, dDlgY);

			double l, n, Zmin, c;						
			switch (m_nExposure)
			{
			case 0: l = 180, n = 1. / 2., Zmin = 60, c = 0.45; break;
			case 1: l = 320, n = 1. / 3., Zmin = 30, c = 0.30; break;
			case 2: l = 500, n = 1. / 5., Zmin = 15, c = 0.20; break;
			case 3: l = 650, n = 1. / 8., Zmin = 7, c = 0.15; break;
			default: AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Exposure_Category_Error_)); return;
			}
						
			double z = 0.6 * height;
			if (z < Zmin) z = Zmin;
			/*	if(z < Zmin)
				{
					CString str;
					str.Format(_LS(IDS_WG_CMD__ADDD__Input_Error___Height____g_), Zmin/0.6);
					AfxMessageBox(str);
					m_XFactor.SetEditUnit(xfactor);
					m_YFactor.SetEditUnit(yfactor);
					return;
				}
				*/
			double Iz = c * pow((33. / z), 1. / 6.);
			double Lz = l * pow(z / 33., n);

			double dFactor = 1.0;
			if (m_nCalcType == IBC) { dFactor = 0.63; }
			else { dFactor = 0.62; }

			double qx = sqrt(1. / (1 + dFactor * pow(((xBreadth + height) / Lz), 0.63)));
			double qy = sqrt(1. / (1 + dFactor * pow(((yBreadth + height) / Lz), 0.63)));

			xfactor = 0.925 * ((1 + 1.7 * 3.4 * Iz * qx) / (1 + 1.7 * 3.4 * Iz));
			yfactor = 0.925 * ((1 + 1.7 * 3.4 * Iz * qy) / (1 + 1.7 * 3.4 * Iz));
		}
		break;
	case CWindGustFIBC2000Dlg::NSCP:
		{
			const int nExposure = m_nExposure + 1;
			CWind_NSCP2024 CodeCalc;
			xfactor = CodeCalc.CalcGustFactorRigid(nExposure, dDlgX, dDlgH);
			yfactor = CodeCalc.CalcGustFactorRigid(nExposure, dDlgY, dDlgH);
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	
	m_XFactor.SetEditUnit(xfactor);
	m_YFactor.SetEditUnit(yfactor);
}

void CWindGustFIBC2000Dlg::OnCmdWindgustOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_dBX = m_XBreadth.GetEditValue();
	m_dBY = m_YBreadth.GetEditValue();
	m_dXFactor = m_XFactor.GetEditValue();
	m_dYFactor = m_YFactor.GetEditValue();

	CDialogMove::OnOK();
}

// nForce : force unit
// nLength : length unit
// nUnitType
// dblData : ¹Ù²î±â Àü µ¥ÀÌÅ¸
// return value : ¹Ù²ï µ¥ÀÌÅ¸
double CWindGustFIBC2000Dlg::CvtUnit(int nForce, int nLength, int nUnitType, double dblData)
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
/////////////////////////////////////////////////////////////////////////////
















/////////////////////////////////////////////////////////////////////////////
// CWindGustF2IBC2000Dlg dialog
CWindGustF2IBC2000Dlg::CWindGustF2IBC2000Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindGustF2IBC2000Dlg::IDD, pParent)
{
	m_nCalcType = IBC;
	//{{AFX_DATA_INIT(CWindGustF2IBC2000Dlg)
	//}}AFX_DATA_INIT
}


void CWindGustF2IBC2000Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindGustF2IBC2000Dlg)
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT3, m_YBreadthUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT2, m_XBreadthUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT1, m_heightUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_FEDIT2, m_YFactor);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_FEDIT1, m_XFactor);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_EDIT5, m_Damping);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_EDIT6, m_YNatural);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_EDIT4, m_XNatural);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_EDIT3, m_YBreadth);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_EDIT2, m_XBreadth);
	DDX_Control(pDX, IDC_CMD_WINDGUST_FLEXI_EDIT1, m_height);
	DDX_Control(pDX, IDC_CMD_IBC2000_EC_COMBO, m_Exposure);
	DDX_Control(pDX, IDC_CMD_IBC2000_BWS_EDIT, m_BasicWind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWindGustF2IBC2000Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindGustF2IBC2000Dlg)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_OK, OnCmdWindgustOk)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CANCEL, OnCmdWindgustCancel)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_FLEXI_CAL, OnCmdWindgustFlexiCal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindGustF2IBC2000Dlg message handlers
BOOL CWindGustF2IBC2000Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_height.SetEditUnit(m_dheight);
	m_XBreadth.SetEditUnit(m_dBX);
	m_YBreadth.SetEditUnit(m_dBY);
	m_XNatural.SetEditUnit(m_dXNatural);
	m_YNatural.SetEditUnit(m_dYNatural);
	m_Damping.SetEditUnit(m_dDamping);

	m_BasicWind.SetEditUnit(m_dBasicWind);
	m_heightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_XBreadthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_YBreadthUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	CString str;
	switch(m_nExposure)
	{
		case 0: str = _T("A"); break;
		case 1: str = _T("B"); break;
		case 2: str = _T("C"); break;
		case 3: str = _T("D"); break;
	}
	m_Exposure.SetWindowText(str);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindGustF2IBC2000Dlg::OnCmdWindgustCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();	
}

void CWindGustF2IBC2000Dlg::OnCmdWindgustFlexiCal() 
{	
	double xfactor = 0.0;
	double yfactor = 0.0;

	const double height = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, m_height.GetEditValue()); // dialog input
	const double xBreadth = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, m_XBreadth.GetEditValue());  // dialog input
	const double yBreadth = CvtUnit(NULL, D_UNITSYS_LENGTH_INDEX_FT, D_UNITSYS_BASE_LENGTH, m_YBreadth.GetEditValue());  // dialog input
	const double damp = m_Damping.GetEditValue();			// dialog input
	const double speed = m_BasicWind.GetEditValue();		// dialog input
	const double nx = m_XNatural.GetEditValue();				// dialog input
	const double ny = m_YNatural.GetEditValue();				// dialog input

	switch (m_nCalcType)
	{
	case CWindGustF2IBC2000Dlg::IBC:		
	case CWindGustF2IBC2000Dlg::NSR:
		{
			double l, n, Zmin, c, b, alpha;
			switch (m_nExposure)
			{
			case 0: l = 180, n = 1. / 2., Zmin = 60, c = 0.45, b = 0.30, alpha = 1. / 3.0; break;
			case 1: l = 320, n = 1. / 3., Zmin = 30, c = 0.30, b = 0.45, alpha = 1. / 4.0; break;
			case 2: l = 500, n = 1. / 5., Zmin = 15, c = 0.20, b = 0.65, alpha = 1. / 6.5; break;
			case 3: l = 650, n = 1. / 8., Zmin = 7, c = 0.15, b = 0.80, alpha = 1. / 9.0; break;
			default: AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Exposure_Category_Error_)); return;
			}

			double z, Iz, Lz, qx, qy, xLength, yLength;
			double Gx, Gy, Rnx, Rny, Rhx, Rhy, Rbx, Rby, Rlx, Rly, Rx, Ry;
					
			z = 0.6 * height;
			if (z < Zmin) z = Zmin;
			/*  if(z < Zmin)
				{
					CString str;
					str.Format(_LS(IDS_WG_CMD__ADDD__Input_Error___Height____g_), Zmin/0.6);
					AfxMessageBox(str);
					m_XFactor.SetEditUnit(xfactor);
					m_YFactor.SetEditUnit(yfactor);
					return;
				}
			*/
			Iz = c * pow((33. / z), 1. / 6.);
			Lz = l * pow(z / 33., n);

			qx = sqrt(1. / (1 + 0.63 * pow(((xBreadth + height) / Lz), 0.63)));
			qy = sqrt(1. / (1 + 0.63 * pow(((yBreadth + height) / Lz), 0.63)));
			
			if (nx <= 0)
			{
				AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_X_Natural_Fr));
				return;
			}
			if (ny <= 0)
			{
				AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Y_Natural_Fr));
				return;
			}

			Gx = sqrt(2 * log(3600 * nx)) + 0.577 / sqrt(2 * log(3600 * nx));
			Gy = sqrt(2 * log(3600 * ny)) + 0.577 / sqrt(2 * log(3600 * ny));
						
			if (damp <= 0)
			{
				AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Damping_Rati));
				return;
			}

			double tempNx, tempNy, tempV;
			if (m_nCalcType == IBC)
			{
				tempV = b * pow((z / 33.), alpha) * speed * 88. / 60.;
			}
			else
			{
				tempV = b * pow((z / 10.), alpha) * speed;
			}

			tempNx = nx * Lz / tempV;
			tempNy = ny * Lz / tempV;
			Rnx = (7.47 * tempNx) / pow((1 + 10.3 * tempNx), 5. / 3.);
			Rny = (7.47 * tempNy) / pow((1 + 10.3 * tempNy), 5. / 3.);

			tempNx = 4.6 * nx * height / tempV;
			tempNy = 4.6 * ny * height / tempV;
			if (tempNx == 0) Rhx = 1;
			else Rhx = 1. / tempNx - 1. / (2 * pow(tempNx, 2)) * (1 - exp(-2 * tempNx));
			if (tempNy == 0) Rhy = 1;
			else Rhy = 1. / tempNy - 1. / (2 * pow(tempNy, 2)) * (1 - exp(-2 * tempNy));

			tempNx = 4.6 * nx * xBreadth / tempV;
			tempNy = 4.6 * ny * yBreadth / tempV;
			if (tempNx == 0) Rbx = 1;
			else Rbx = 1. / tempNx - 1. / (2 * pow(tempNx, 2)) * (1 - exp(-2 * tempNx));
			if (tempNy == 0) Rby = 1;
			else Rby = 1. / tempNy - 1. / (2 * pow(tempNy, 2)) * (1 - exp(-2 * tempNy));
						
			xLength = yBreadth;
			yLength = xBreadth;
			tempNx = 15.4 * nx * xLength / tempV;
			tempNy = 15.4 * ny * yLength / tempV;
			if (tempNx == 0) Rlx = 1;
			else Rlx = 1. / tempNx - 1. / (2 * pow(tempNx, 2)) * (1 - exp(-2 * tempNx));
			if (tempNy == 0) Rly = 1;
			else Rly = 1. / tempNy - 1. / (2 * pow(tempNy, 2)) * (1 - exp(-2 * tempNy));

			Rx = sqrt((1. / damp) * Rnx * Rhx * Rbx * (0.53 + 0.47 * Rlx));
			Ry = sqrt((1. / damp) * Rny * Rhy * Rby * (0.53 + 0.47 * Rly));

			xfactor = 0.925 * ((1 + 1.7 * Iz * sqrt(pow(3.4, 2) * pow(qx, 2) + pow(Gx, 2) * pow(Rx, 2))) / (1 + 1.7 * 3.4 * Iz));
			yfactor = 0.925 * ((1 + 1.7 * Iz * sqrt(pow(3.4, 2) * pow(qy, 2) + pow(Gy, 2) * pow(Ry, 2))) / (1 + 1.7 * 3.4 * Iz));
		}
		break;
	case CWindGustF2IBC2000Dlg::NSCP:
		{
			const int nExposure = m_nExposure + 1;
			CWind_NSCP2024 CodeCalc;
			xfactor = CodeCalc.CalcGustFactorFlexible(nExposure, speed, height, xBreadth, yBreadth, nx, damp);
			yfactor = CodeCalc.CalcGustFactorFlexible(nExposure, speed, height, xBreadth, yBreadth, ny, damp);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	m_XFactor.SetEditUnit(xfactor);
	m_YFactor.SetEditUnit(yfactor);
}

void CWindGustF2IBC2000Dlg::OnCmdWindgustOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_dBX				= m_XBreadth.GetEditValue();
	m_dBY				= m_YBreadth.GetEditValue();
	m_dXNatural = m_XNatural.GetEditValue();
	m_dYNatural = m_YNatural.GetEditValue();
	m_dDamping  = m_Damping.GetEditValue();
	m_dXFactor	= m_XFactor.GetEditValue();
	m_dYFactor	= m_YFactor.GetEditValue();

	CDialogMove::OnOK();
}

// nForce : force unit
// nLength : length unit
// nUnitType
// dblData : ¹Ù²î±â Àü µ¥ÀÌÅ¸
// return value : ¹Ù²ï µ¥ÀÌÅ¸
double CWindGustF2IBC2000Dlg::CvtUnit(int nForce, int nLength, int nUnitType, double dblData)
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
/////////////////////////////////////////////////////////////////////////////
