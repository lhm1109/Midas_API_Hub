// StldWindPressureArea_ASCE7_16_Topography.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindPressureVelo_ASCE7_16_Topography.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_db\Wind_ASCE7_16.h"
#include "..\wg_db\Wind_NSCP2024.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMWindPressureVelo_ASCE7_16Topography dialog
CCMWindPressureVelo_ASCE7_16Topography::CCMWindPressureVelo_ASCE7_16Topography(int nCode, CWnd* pParent/* = NULL*/)   // standard constructor
	: CDialogMove(CCMWindPressureVelo_ASCE7_16Topography::IDD, pParent)
{
	m_nCode = nCode;
	m_nExposure = 2;
	m_nHillShape= EN_2D_RIDGE;
	m_nBldgLocation = EN_UPWIND_CREST;
	m_dHillHeight = 0.0;
	m_dHillLength = 0.0;
	m_dHillDistance = 0.0;
	m_dKzt = 0.0;
	m_dZ = 0.0;
}

void CCMWindPressureVelo_ASCE7_16Topography::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMWindPressureVelo_ASCE7_16Topography)
	DDX_Control(pDX, IDC_CMD_ANAL_HLENGTH_X_EDIT    , m_editHillLeng_X  );
	DDX_Control(pDX, IDC_CMD_ANAL_HILLSHAPE_X_COMBO , m_cobxHillShape_X );
	DDX_Control(pDX, IDC_CMD_ANAL_HHEIGHT_X_EDIT    , m_editHHeight_X   );
	DDX_Control(pDX, IDC_CMD_ANAL_CBDIST_X_EDIT     , m_editCBDistance_X);
	DDX_Control(pDX, IDC_CMD_ANAL_BLOCATION_X_COMBO , m_cobxBLocation_X );
	DDX_Control(pDX, IDC_CMD_ANAL_HLENGTH_UNIT      , m_unitHillLeng    );  
	DDX_Control(pDX, IDC_CMD_ANAL_CBDIST_UNIT       , m_unitCBDistance  );
	DDX_Control(pDX, IDC_CMD_ANAL_HHEIGHT_UNIT      , m_unitHHeight     );  
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindPressureVelo_ASCE7_16Topography, CInternationalDlg)
	//{{AFX_MSG_MAP(CMWindPressureVelo_ASCE7_16Topography)
	ON_CBN_SELCHANGE(IDC_CMD_ANAL_HILLSHAPE_X_COMBO, OnSelchangeCmdAnalHillshapeCombo_X)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMWindPressureVelo_ASCE7_16Topography message handlers

BOOL CCMWindPressureVelo_ASCE7_16Topography::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindPressureVelo_ASCE7_16Topography::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	CInternationalDlg::OnOK();
}

void CCMWindPressureVelo_ASCE7_16Topography::InitHillShapeCombo()
{
	m_cobxHillShape_X.ResetContent();
	
	CString aItem[] = {_LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_2DRIDGE), 
		_LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_2DESCARP),
		_LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_3DAXISYM)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxHillShape_X.AddString(aItem[i]);  
	}  
	m_cobxHillShape_X.SetCurSel(0);  

	ChangeComboWidth();
}

void CCMWindPressureVelo_ASCE7_16Topography::InitBLocationCombo()
{
	m_cobxBLocation_X.ResetContent();
	
	CString aItem[] = {_LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_UP), _LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_DN)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxBLocation_X.AddString(aItem[i]);
	}  
	m_cobxBLocation_X.SetCurSel(0);
}

//--------------------------------------------------------------------------------
void CCMWindPressureVelo_ASCE7_16Topography::InitControls()
{
	m_editHillLeng_X.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillLength);  
	m_editHHeight_X.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillHeight);
	m_editCBDistance_X.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillDistance);	
	m_unitHHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillHeight);  
	m_unitHillLeng.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillLength);
	m_unitCBDistance.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillDistance);	  

	InitHillShapeCombo();
	InitBLocationCombo();
}

void CCMWindPressureVelo_ASCE7_16Topography::Data2Dlg()
{
	m_cobxHillShape_X.SetCurSel(m_nHillShape);
	m_cobxBLocation_X.SetCurSel(m_nBldgLocation);
	m_editHHeight_X.SetEditUnit(m_dHillHeight);
	m_editHillLeng_X.SetEditUnit(m_dHillLength);
	m_editCBDistance_X.SetEditUnit(m_dHillDistance);

	OnSelchangeCmdAnalHillshapeCombo_X();
}

BOOL CCMWindPressureVelo_ASCE7_16Topography::Dlg2Data()
{
	m_nHillShape    = m_cobxHillShape_X.GetCurSel();
	m_nBldgLocation = m_cobxBLocation_X.GetCurSel();
	m_dHillHeight   = m_editHHeight_X.GetEditValue();
	m_dHillLength   = m_editHillLeng_X.GetEditValue();
	m_dHillDistance = m_editCBDistance_X.GetEditValue();

	switch (m_nCode)
	{
	case ASCE7_W_2016:
	case ASCE7_W_2022:
		{
			CWind_ASCE7_16 CodeCalc(m_nCode, CWind_ASCE7_16::EN_UNIT_US);
			m_dKzt = CodeCalc.CalKzt(m_nExposure, m_nHillShape, m_nBldgLocation, m_dHillHeight, m_dHillLength, m_dHillDistance, m_dZ, false);
		}
		break;
	case NSCP_W_2024:
		{
			CWind_NSCP2024 CodeCalc;
			m_dKzt = CodeCalc.CalKzt(m_nExposure, m_nHillShape, m_nBldgLocation, m_dHillHeight, m_dHillLength, m_dHillDistance, m_dZ, false);
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	return TRUE;
}

void CCMWindPressureVelo_ASCE7_16Topography::ChangeComboWidth()
{
	int nCount = m_cobxHillShape_X.GetCount();
	if(nCount<1) return;
	
	CString data = _T("");
	m_cobxHillShape_X.GetLBText(0, data);
	
	CDC* pDC = GetDC();  
	CSize max, cur;
	max = pDC->GetTextExtent(data);
	m_cobxHillShape_X.SetDroppedWidth(max.cx);
	
	for(int i=1; i<nCount; i++)
	{
		m_cobxHillShape_X.GetLBText(i, data);
		cur = pDC->GetTextExtent(data); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if(max.cx > m_cobxHillShape_X.GetDroppedWidth())
	{
		m_cobxHillShape_X.SetDroppedWidth(max.cx);
	}
	
	ReleaseDC(pDC);
}

void CCMWindPressureVelo_ASCE7_16Topography::OnSelchangeCmdAnalHillshapeCombo_X()
{
	int nHillShape = m_cobxHillShape_X.GetCurSel();
	m_cobxBLocation_X.EnableWindow(nHillShape == 1);
}
