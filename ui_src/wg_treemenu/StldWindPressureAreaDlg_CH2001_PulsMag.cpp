// WindGustKBC2009Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_CH2001_PulsMag.h"
#include "StldWindPressureAreaDlg_CH_Period.h"

#include <math.h>

#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF dialog
CStldWindPressureAreaDlg_CH2001_PulsMag::CStldWindPressureAreaDlg_CH2001_PulsMag(CWnd* pParent /*=NULL*/)
	: CDialogMove(CStldWindPressureAreaDlg_CH2001_PulsMag::IDD, pParent)
{
	m_pWdpr = NULL;
	m_nPeriodType = 1;
	m_dPeriod = 0.0;
	m_dMagnifyFactor = 0.0;
	m_dDampingRatio = 1.0;
}


void CStldWindPressureAreaDlg_CH2001_PulsMag::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_MATERIAL,         m_cobxMaterial);
	DDX_Control(pDX, IDC_TM_DAMPING_CFT_EDIT, m_edtDampingRatio);
	
	DDX_Control(pDX, IDC_TM_FUND_PERIOD,      m_edtFundamentalPeriod);	
	DDX_Control(pDX, IDC_TM_PULSE_MAGNIFYF,   m_edtPulsMagFactor);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_CH2001_PulsMag, CDialogMove)
	ON_BN_CLICKED(IDC_TM_PERIOD_BTN,      OnBtnFundamentalPeriod)  
	ON_CBN_SELCHANGE(IDC_TM_MATERIAL,     OnSelChangeMatl)
	ON_EN_CHANGE(IDC_TM_DAMPING_CFT_EDIT, OnCalcMagFactor)
	ON_EN_CHANGE(IDC_TM_FUND_PERIOD,      OnCalcMagFactor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF message handlers

BOOL CStldWindPressureAreaDlg_CH2001_PulsMag::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitControl();
	OnSelChangeMatl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::OnOK() 
{
	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::OnCancel() 
{
	CDialogMove::OnCancel();		
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::InitMaterialCombo()
{
	m_cobxMaterial.ResetContent();
	
	CString aItem[] = {_LS(IDS_WG_CMD__ADDD__Steel), _LS(IDS_CMD_STEEL_INFILLED_WALL), _LS(IDS_CMD_RC)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum ; i++)
		m_cobxMaterial.AddString(aItem[i]);
	m_cobxMaterial.SetCurSel(0);
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::InitControl()
{
	InitMaterialCombo();
	m_edtDampingRatio.SetUnitType(D_UNITSYS_NONE);
	m_edtFundamentalPeriod.SetUnitType(D_UNITSYS_NONE);
	m_edtPulsMagFactor.SetUnitType(D_UNITSYS_NONE);

	m_edtDampingRatio.SetEditUnit(1.0);
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::OnBtnFundamentalPeriod()
{
	double dHeight;
	double dWidth;
	int    nStore;
	if(!CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(m_pWdpr, dHeight, nStore, dWidth))
	{
		dHeight = 0.0;
		dWidth = 0.0;
		nStore = 0;
	}

	CStldWindPressureAreaDlg_CH_Period dlg;
	dlg.SetInit(dWidth, dHeight, nStore);
	dlg.SetDlgType(m_nPeriodType);


	if(dlg.DoModal() == IDOK)
	{
		dlg.GetDlgPeriod(m_dPeriod);
		m_nPeriodType = dlg.GetDlgType();
		OnCalcMagFactor();
		Data2Dlg();
	}
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::OnCalcMagFactor()
{
	if(m_WvepK == 0)
		return;

	m_dDampingRatio = m_edtDampingRatio.GetEditValue();

	T_WIND_CH2002 data;
	data.Initialize();
	data.nRoughCategory     = m_WvepD.CodeParam.CH2001.nRoughCategory;
	data.dDampingRatio      = m_dDampingRatio;
	data.dBasicWindPressure = m_WvepD.CodeParam.CH2001.dBasicWindPressure;
	data.dPeriod_X          = m_dPeriod;
	data.dPeriod_Y          = m_dPeriod;

	if(data.dPeriod_X < 0 || data.dPeriod_Y < 0)
	{
		CString str;  
		CString strError = _LS(IDS_CMD_HSFC_error);  // Error
		CString strErrorR = strError + _T("! ");         // Error!	
		AfxMessageBox(strErrorR + _LS(IDS_WG_CMD__ADD2__Period));
		return;
	}
	CDBDoc::GetDocPoint()->m_pEditData->CalcWindChinaPulseMagnifyFactor(data);
	if(fabs(data.dPulseMagnifyFactorX - data.dPulseMagnifyFactorY) < 1E-5)
	{
		m_dMagnifyFactor = data.dPulseMagnifyFactorX;
		Data2Dlg();
	}
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::SetWvepKey( T_WVEP_K key )
{
	m_WvepK = key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc->m_pAttrCtrl2->ExistWvep(m_WvepK))
	{
		if(!pDoc->m_pAttrCtrl2->GetWvep(m_WvepK, m_WvepD))
		{
			ASSERT(0);
			m_WvepD.Initialize();
		}
	} 
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::Data2Dlg()
{
	m_edtPulsMagFactor.SetEditUnit(m_dMagnifyFactor);
	m_edtFundamentalPeriod.SetEditUnit(m_dPeriod);
}


void CStldWindPressureAreaDlg_CH2001_PulsMag::Dlg2Data()
{
	m_dPeriod = m_edtFundamentalPeriod.GetEditValue();
	m_dMagnifyFactor = m_edtPulsMagFactor.GetEditValue();
}

void CStldWindPressureAreaDlg_CH2001_PulsMag::OnSelChangeMatl()
{
	int nMaterial = m_cobxMaterial.GetCurSel();
	double dDamping[] = { 0.01, 0.02, 0.05 };
	m_edtDampingRatio.SetEditUnit(dDamping[nMaterial]);
	Dlg2Data();
	OnCalcMagFactor();
}
