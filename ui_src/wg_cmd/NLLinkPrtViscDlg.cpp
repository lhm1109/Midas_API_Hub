// NLLinkPrtViscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtViscDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAXWELL_MODEL 0
#define KELVIN_MODEL 1
#define DAMPER_BRACE_MODEL 2

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtViscDlg dialog


CNLLinkPrtViscDlg::CNLLinkPrtViscDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtViscDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtViscDlg)

	//}}AFX_DATA_INIT
	m_aCtrlDamperType.Add(IDC_CMD_NLLK_MAXWELL_RDO);
	m_aCtrlDamperType.Add(IDC_CMD_NLLK_KELVIN_RDO);
	m_aCtrlDamperType.Add(IDC_CMD_NLLK_DAM_RDO);
}

CNLLinkPrtViscDlg::~CNLLinkPrtViscDlg()
{
}
void CNLLinkPrtViscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtViscDlg)
	DDX_Control(pDX, IDC_CMD_NLLK_STIFBRAC_UNIT, m_wndStifBracUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFDAMP_UNIT, m_wndStifDampUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_DAMP_UNIT, m_wndDampingUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_REFVEL_UNIT, m_wndRefVelocityUnit);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFBRAC_EDIT, m_wndStifBrac);
	DDX_Control(pDX, IDC_CMD_NLLK_STIFDAMP_EDIT, m_wndStifDamp);
	DDX_Control(pDX, IDC_CMD_NLLK_DAMP_EDIT, m_wndDamping);
	DDX_Control(pDX, IDC_CMD_NLLK_EXPO_EDIT, m_wndDampExpo);
	DDX_Control(pDX, IDC_CMD_NLLK_REFVEL_EDIT, m_wndRefVelocity);
	DDX_Control(pDX, IDC_CMD_NLLK_VED_IMG01, m_pBitmap01);
	DDX_Control(pDX, IDC_CMD_NLLK_VED_IMG02, m_pBitmap02);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLLinkPrtViscDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtViscDlg)
	  ON_BN_CLICKED(IDC_CMD_NLLK_MAXWELL_RDO, OnChangeDamperType)
	  ON_BN_CLICKED(IDC_CMD_NLLK_KELVIN_RDO, OnChangeDamperType)
	  ON_BN_CLICKED(IDC_CMD_NLLK_DAM_RDO, OnChangeDamperType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtViscDlg message handlers
BOOL CNLLinkPrtViscDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_nDofType)
	{
		case 0: ShowData(0); break;
		case 1: ShowData(1); break;
		case 2: ShowData(2); break;
		case 3: ShowData(3); break;
		case 4: ShowData(4); break;
		case 5: ShowData(5); break;
	}
	switch(m_nDofType)
	{
		case 0: case 1: case 2:
			m_wndStifBracUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
			m_wndStifDampUnit.SetUnitType(D_UNITSYS_BASE_SPRING);
			m_wndDampingUnit.SetUnitType(D_UNITSYS_BASE_FORCE);
			m_wndRefVelocityUnit.SetUnitType(D_UNITSYS_BASE_VELOCITY);
			break;
		case 3: case 4: case 5:
			m_wndStifBracUnit.SetUnitType(D_UNITSYS_BASE_ROSPRING);
			m_wndStifDampUnit.SetUnitType(D_UNITSYS_BASE_ROSPRING);
			m_wndDampingUnit.SetUnitType(D_UNITSYS_BASE_MOMENT);
			m_wndRefVelocityUnit.SetUnitType(D_UNITSYS_BASE_FREQUENCY);
			break;
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtViscDlg::ShowData(int nFlag)
{
	m_wndStifDamp.SetEditUnit(m_Data.NDP_Prop[nFlag].VD.dDamperStiffness );
	m_wndDamping.SetEditUnit (m_Data.NDP_Prop[nFlag].VD.dDamping         );
	m_wndDampExpo.SetEditUnit(m_Data.NDP_Prop[nFlag].VD.dDampExp         );  
	m_wndStifBrac.SetEditUnit(m_Data.NDP_Prop[nFlag].VD.dBracingStiffness);      
	m_wndRefVelocity.SetEditUnit(m_Data.NDP_Prop[nFlag].VD.dRefVelocity  );

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDamperType, m_Data.NDP_Prop[nFlag].VD.nVEDType);
	ShowHideCtrl();
}

void CNLLinkPrtViscDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_wndStifDamp.GetEditValue() < 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_DamperStiffness)); 	return;
	}
	if(m_wndDamping.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_Damping)); 	return;
	}
	if(m_wndDampExpo.GetEditValue() <= 0)
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_DampingExponent)); 	return;
	}

	int nDamperType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDamperType, nDamperType);

	if((nDamperType!=KELVIN_MODEL)&&(m_wndStifBrac.GetEditValue() <= 0))
	{
	  AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_BraceStiffness)); 	return;
	}

	if (m_wndRefVelocity.GetEditValue() <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_ReferenceVelocity)); 	return;
	}

	m_Data.NDP_Prop[m_nDofType].VD.nVEDType = nDamperType;


	if(nDamperType==MAXWELL_MODEL)
	{
	  m_Data.NDP_Prop[m_nDofType].VD.dDamperStiffness  = 0;
	}
	else
	{
	  m_Data.NDP_Prop[m_nDofType].VD.dDamperStiffness  = m_wndStifDamp.GetEditValue();
	}

	m_Data.NDP_Prop[m_nDofType].VD.dDamping     = m_wndDamping.GetEditValue() ;
	m_Data.NDP_Prop[m_nDofType].VD.dRefVelocity = m_wndRefVelocity.GetEditValue();
	m_Data.NDP_Prop[m_nDofType].VD.dDampExp     = m_wndDampExpo.GetEditValue();

	if(nDamperType==KELVIN_MODEL)
	{
		m_Data.NDP_Prop[m_nDofType].VD.dBracingStiffness = 0;
	}
	else
	{
		m_Data.NDP_Prop[m_nDofType].VD.dBracingStiffness = m_wndStifBrac.GetEditValue();
	}

	CDialogMove::OnOK();
}

void CNLLinkPrtViscDlg::OnCancel() 
{
	// TODO: Add extra cleanup here	
	CDialogMove::OnCancel();
}



void CNLLinkPrtViscDlg::OnChangeDamperType() 
{
	ShowHideCtrl();
}

int CNLLinkPrtViscDlg::ShowHideCtrl()
{
	int nDamperType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDamperType, nDamperType);

	CArray<UINT, UINT> aShowExceptMaxWell;
	CArray<UINT, UINT> aShowExceptKelvin;
	CArray<UINT, UINT> aMoveMaxWell;

	aShowExceptMaxWell.Add(IDC_CMD_NLLK_STIFDAMP_EDIT);
	aShowExceptMaxWell.Add(IDC_CMD_NLLK_STIFDAMP_UNIT);
	aShowExceptMaxWell.Add(IDC_CMD_NLLK_STIFDAMP_STC);

	aShowExceptKelvin.Add(IDC_CMD_NLLK_STIFBRAC_EDIT);
	aShowExceptKelvin.Add(IDC_CMD_NLLK_STIFBRAC_UNIT);
	aShowExceptKelvin.Add(IDC_CMD_NLLK_STIFBRAC_STC);


	aMoveMaxWell.Add(IDC_CMD_NLLK_DAMP_EDIT);
	aMoveMaxWell.Add(IDC_CMD_NLLK_DAMP_UNIT);
	aMoveMaxWell.Add(IDC_CMD_NLLK_DAMP_STC);

	aMoveMaxWell.Add(IDC_CMD_NLLK_REFVEL_EDIT);
	aMoveMaxWell.Add(IDC_CMD_NLLK_REFVEL_UNIT);
	aMoveMaxWell.Add(IDC_CMD_NLLK_REFVEL_STC);

	aMoveMaxWell.Add(IDC_CMD_NLLK_EXPO_EDIT);
	aMoveMaxWell.Add(IDC_CMD_NLLK_EXPO_STC);

	aMoveMaxWell.Add(IDC_CMD_NLLK_STIFBRAC_EDIT);
	aMoveMaxWell.Add(IDC_CMD_NLLK_STIFBRAC_UNIT);
	aMoveMaxWell.Add(IDC_CMD_NLLK_STIFBRAC_STC);
	
	double nDistY;
	CRect toRect, FromRect;


	GetDlgItem(IDC_CMD_NLLK_STIFDAMP_EDIT)->GetWindowRect(toRect);
	GetDlgItem(IDC_CMD_NLLK_DAMP_EDIT)->GetWindowRect(FromRect);

	if(nDamperType == MAXWELL_MODEL)
	{
			nDistY = toRect.top - FromRect.top;
	}
	else
	{
			nDistY = (toRect.bottom - FromRect.top)+ globalUtils.ScaleByDPI(6);
	}

	CDlgUtil::CtrlMoveDistY(this, aMoveMaxWell, nDistY);


	CDlgUtil::CtrlShowHide(this, aShowExceptMaxWell, nDamperType != MAXWELL_MODEL);
	CDlgUtil::CtrlShowHide(this, aShowExceptKelvin, nDamperType != KELVIN_MODEL);
	Invalidate(TRUE);
	UpdateWindow();
//  CDlgUtil::CtrlMoveDistY(this,aShowExceptKelvin,
//  CDlgUtil::CtrlEnableDisable(this, aShowExceptMaxWell, nDamperType != MAXWELL_MODEL);
//  CDlgUtil::CtrlEnableDisable(this, aShowExceptKelvin, nDamperType != KELVIN_MODEL);

	ChangeImage(nDamperType);

	return nDamperType;
}

void CNLLinkPrtViscDlg::ChangeImage(int nItem)
{
	switch(nItem)
	{
		case 0:
			m_pBitmap01.SetImage(_T("SVG\\Illustration\\Dialog\\Maxwell01.svg"));
			m_pBitmap02.SetImage(_T("SVG\\Illustration\\Dialog\\Maxwell02.svg"));
			break;
		case 1:
			m_pBitmap01.SetImage(_T("SVG\\Illustration\\Dialog\\Kelvin01.svg"));
			m_pBitmap02.SetImage(_T("SVG\\Illustration\\Dialog\\Kelvin02.svg"));
			break;
		case 2:
			m_pBitmap01.SetImage(_T("SVG\\Illustration\\Dialog\\Damper01.svg"));
			m_pBitmap02.SetImage(_T("SVG\\Illustration\\Dialog\\Damper02.svg"));
		break;
	}
}