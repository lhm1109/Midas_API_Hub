// WindGustKDS2021Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindGustKDS2021Dlg.h"

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
// CWindGustKDS2021Dlg dialog
CWindGustKDS2021Dlg::CWindGustKDS2021Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindGustKDS2021Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWindGustKDS2021Dlg)
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
	
	m_dTotalMass = 0.0;
	m_dMx = 0.0;
	m_dMy = 0.0;
	m_dVibration = 0.0;

	m_dStoryHMax = 0.0;
	m_dKdx = 1.0;
	m_dKdy = 1.0;
}


void CWindGustKDS2021Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWindGustKDS2021Dlg)
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT3, m_wndBreadthYUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT2, m_wndBreadthXUnit);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT1, m_wndBldgHeightUnit);	
	DDX_Control(pDX, IDC_CMD_KOR2000_TOPOGRAPHY_EDIT, m_wndTopography);	
	DDX_Control(pDX, IDC_CMD_KDS2021_KDX_EDIT, m_wndKdx);
	DDX_Control(pDX, IDC_CMD_KDS2021_KDY_EDIT, m_wndKdy);
	DDX_Control(pDX, IDC_CMD_KOR2000_IF_EDIT, m_wndKor2000If);
	DDX_Control(pDX, IDC_CMD_KOR2000_BWS_EDIT, m_wndKor2000Bws);
	DDX_Control(pDX, IDC_CMD_KOR2000_EC_COMBO, m_wndKor2000ECcbo);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT6, m_edtFrequencyY);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT5, m_edtDampingRatio);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT4, m_edtFrequencyX);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT3, m_edtBreadthY);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT2, m_edtBreadthX);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT1, m_edtHeight);

	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT7, m_edtTotalMass);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT7, m_untTotalMass);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT8, m_edtMx);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT9, m_edtMy);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT8, m_untMx);
	DDX_Control(pDX, IDC_CMD_WINDGUST_UNIT9, m_untMy);
	DDX_Control(pDX, IDC_CMD_WINDGUST_EDIT10, m_editVibration);
	// DDX_Radio(pDX, IDC_CMD_WINDGUST_GUST_RDO1, m_GustFactorDirction);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindGustKDS2021Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CWindGustKDS2021Dlg)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CAL, OnCmdWindgustCal)
	// ON_BN_CLICKED(IDC_CMD_WINDGUST_GUST_RDO1, OnCmdWindgustGustRdo)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_OK, OnCmdWindgustOk)
	// ON_BN_CLICKED(IDC_CMD_WINDGUST_GUST_RDO2, OnCmdWindgustGustRdo)
	ON_BN_CLICKED(IDC_CMD_WINDGUST_CANCEL, OnCmdWindgustCancel)

	ON_BN_CLICKED(IDC_CMD_WINDGUST_BETA_UPDATE, OnChangeVibration)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindGustKDS2021Dlg message handlers

BOOL CWindGustKDS2021Dlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	// ((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->EnableWindow(m_GustFactorDirction == 1);
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT1))->SetWindowText(_T(""));
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->SetWindowText(_T(""));

	InitialData();

	int nShow = SW_SHOW;
	if (m_nRigidity == 0)  nShow = SW_HIDE;// Rigid Structure
	
	GetDlgItem(IDC_CMD_WINDGUST_TITLE4)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_EDIT4)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_UNIT4)->ShowWindow(nShow);

	GetDlgItem(IDC_CMD_WINDGUST_TITLE6)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_EDIT6)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_UNIT5)->ShowWindow(nShow);

	GetDlgItem(IDC_CMD_WINDGUST_TITLE5)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_EDIT5)->ShowWindow(nShow);

	GetDlgItem(IDC_CMD_WINDGUST_TITLE7)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_EDIT7)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_UNIT7)->ShowWindow(nShow);

	GetDlgItem(IDC_CMD_WINDGUST_TITLE8)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_EDIT8)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_UNIT8)->ShowWindow(nShow);

	GetDlgItem(IDC_CMD_WINDGUST_TITLE9)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_EDIT9)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_UNIT9)->ShowWindow(nShow);

	GetDlgItem(IDC_CMD_WINDGUST_TITLE10)->ShowWindow(nShow);
	GetDlgItem(IDC_CMD_WINDGUST_EDIT10)->ShowWindow(nShow);
	
	GetDlgItem(IDC_CMD_WINDGUST_BETA_UPDATE)->ShowWindow(nShow);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWindGustKDS2021Dlg::InitialData()
{
	m_edtHeight.SetEditUnit(m_dHeight);
	m_edtBreadthX.SetEditUnit(m_dWidth);
	m_edtBreadthY.SetEditUnit(m_dBreadth);
	m_edtFrequencyX.SetEditUnit(m_dFrequencyX);
	m_edtFrequencyY.SetEditUnit(m_dFrequencyY);
	m_edtDampingRatio.SetEditUnit(m_dDamping);	

	m_edtTotalMass.SetEditUnit(m_dTotalMass);
	m_edtMx.SetEditUnit(m_dMx);
	m_edtMy.SetEditUnit(m_dMy);
	m_editVibration.SetEditUnit(m_dVibration);

	m_wndBreadthYUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);  
	m_wndBreadthXUnit.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_wndBldgHeightUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);   
	
	m_untTotalMass.SetUnitType(D_UNITSYS_BASE_MASS);
	m_untMx.SetUnitType(D_UNITSYS_BASE_MASS);
	m_untMy.SetUnitType(D_UNITSYS_BASE_MASS);

	//m_edtHeightMax.SetEditUnit(m_dStoryHMax);

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

	m_wndKdx.SetEditUnit(m_dKdx);
	m_wndKdy.SetEditUnit(m_dKdy);

	UpdateData(FALSE); 
}

void CWindGustKDS2021Dlg::OnCmdWindgustCal() 
{	
	m_dHeight     = m_edtHeight.GetEditValue();
	m_dWidth      = m_edtBreadthX.GetEditValue();
	m_dBreadth	  = m_edtBreadthY.GetEditValue();
	m_dFrequencyX = m_edtFrequencyX.GetEditValue();
	m_dFrequencyY = m_edtFrequencyY.GetEditValue();
	m_dDamping	  = m_edtDampingRatio.GetEditValue();
	m_dTotalMass  = m_edtTotalMass.GetEditValue();
	m_dMx = m_edtMx.GetEditValue();
	m_dMy = m_edtMy.GetEditValue();
	m_dVibration = m_editVibration.GetEditValue();

	//m_dStoryHMax = m_edtHeightMax.GetEditValue();

	// 단위를 m로 바꾼다.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();	
    const double dToM = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N,  D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);    
    const double dH_UnitM  = m_dHeight  * dToM;
	const double dH_StoryUnitM = m_dStoryHMax * dToM;
    const double dBx_UnitM = m_dWidth   * dToM;
    const double dBy_UnitM = m_dBreadth * dToM;
	
    const double dToKG = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_MASS, 1.0);
    const double dMassT = m_dTotalMass * dToKG;
	const double dMassx = m_dMx * dToKG;
	const double dMassy = m_dMy * dToKG;

    if ( m_dHeight <= 0.0 )
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei)); return;
	}
    if ( m_dWidth  <= 0.0 )
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_X_B)); return;
	}
    if ( m_dBreadth <= 0.0 )
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Y_B)); return;
	}
    if ( m_nRigidity == 1 ) // Flexible Structure
    {
		if (m_dKdx < 0.85 || m_dKdx > 1.0)
		{
			AfxMessageBox(_LS(IDS_DB_ERROR_WIND_KDS2021_KDX)); return;
		}
		if (m_dKdy < 0.85 || m_dKdy > 1.0)
		{
			AfxMessageBox(_LS(IDS_DB_ERROR_WIND_KDS2021_KDY)); return;
		}

        if ( m_dFrequencyX <= 0.0 )
        {
            AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_X_Natural_Fr)); return;
        }
        if ( m_dFrequencyY <= 0.0 )
        {
            AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Y_Natural_Fr)); return;
        }
        if ( m_dDamping <= 0.0 )
        {
            AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Damping_Rati)); return;
        }
        if ( m_dTotalMass <= 0.0 )
        {
            AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_TotalMass)); return;
        }
        if ( m_dMx <= 0.0 )
        {
            AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_MassX)); return;
        }
        if ( m_dMy <= 0.0 )
        {
            AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_MassY)); return;
        }
        if ( m_dVibration < 0.5 || m_dVibration > 2.0 )
        {
            AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Viboration)); return;
		}
	}
	
    CWind_KDS41_10_15_2021 WindCalc;
    const int nExposureCalc = m_nExposure + 1;
    switch ( m_nRigidity )
    {
    case 0:
        m_dGustFactorX = WindCalc.CalcGustEffectFactor_Rigid(nExposureCalc, dBx_UnitM, dH_UnitM, m_dFrequencyX);
        m_dGustFactorY = WindCalc.CalcGustEffectFactor_Rigid(nExposureCalc, dBy_UnitM, dH_UnitM, m_dFrequencyY);
        break;
    case 1:
        m_dGustFactorX = WindCalc.CalcGustEffectFactor_Flexible(nExposureCalc, dBx_UnitM, dH_StoryUnitM, m_dFrequencyX,
            m_dSpeed, dMassT, dMassx, m_dDamping, m_dVibration, m_bTopography, m_dKzt, m_dImportance, m_dKdx);
        m_dGustFactorY = WindCalc.CalcGustEffectFactor_Flexible(nExposureCalc, dBy_UnitM, dH_StoryUnitM, m_dFrequencyY,
            m_dSpeed, dMassT, dMassy, m_dDamping, m_dVibration, m_bTopography, m_dKzt, m_dImportance, m_dKdy);
        break;
    default:
        ASSERT(0);
        break;
    }

	CString strTemp;
	strTemp.Format(_T("%f"), m_dGustFactorX);
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT1))->SetWindowText(strTemp);
	strTemp.Format(_T("%f"), m_dGustFactorY);
	((CButton *)GetDlgItem(IDC_CMD_WINDGUST_GUST_EDIT2))->SetWindowText(strTemp);
}

void CWindGustKDS2021Dlg::OnCmdWindgustOk() 
{
	UpdateData(TRUE);

	m_dWidth		= m_edtBreadthX.GetEditValue();
	m_dBreadth		= m_edtBreadthY.GetEditValue();	
	m_dFrequencyX	= m_edtFrequencyX.GetEditValue();
	m_dFrequencyY	= m_edtFrequencyY.GetEditValue();
	m_dDamping		= m_edtDampingRatio.GetEditValue();

	CDialogMove::OnOK();
}

void CWindGustKDS2021Dlg::OnCmdWindgustCancel() 
{
	CDialogMove::OnCancel();		
}

void CWindGustKDS2021Dlg::OnChangeVibration()
{
	const double dVibVal = m_editVibration.GetEditValue();
	//const double dHmax   = m_edtHeightMax.GetEditValue();
	const double dHmax = m_dStoryHMax;

	if (dVibVal < 0.5 || dVibVal > 2.0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Viboration)); return;
	}

	double dGmass = 0.; 
	if(m_StoryHeight.GetSize() > 0)
	{
		double dHLevel,dMuz;
		double dTol = 1.e-10;
		for (int i= 0; i < m_StoryHeight.GetSize(); i++)
		{
			dHLevel = m_StoryHeight[i];
			if(dHLevel < -dTol) continue;

			if(abs(dHmax) > 0. && dHLevel/dHmax >= 0.)
			{
				dMuz = pow(dHLevel/dHmax,dVibVal);
				dGmass += m_StoryMass[i]*pow(dMuz,2.);
			}
			else
			{
				ASSERT(0);
			}
		}
	}

	m_edtMx.SetEditUnit(dGmass);
	m_edtMy.SetEditUnit(dGmass);
}
