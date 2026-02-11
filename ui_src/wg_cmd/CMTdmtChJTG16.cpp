// CMTdmtChJTG16.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtChJTG16.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtChJTG16 dialog


CCMTdmtChJTG16::CCMTdmtChJTG16(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtChJTG16::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtChJTG16)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bEqiv = FALSE;
}


void CCMTdmtChJTG16::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtChJTG16)
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_COMP,        m_edtCompStrength);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_COMP_UNIT,   m_untCompStrength);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_RELA,        m_edtRelativeHumidity);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_NOTA,        m_edtNotationSize);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_NOTA_UNIT,   m_untNotationSize);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_CEMENTTYPE,  m_edtCementTypeCoef);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_AGE,         m_edtConcreteAge);	
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_AGE_UNIT,    m_untConcreteAge);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_FLY_ASH_EDT, m_edtAddFlyash);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_ELAST_RATIO_EDT, m_edtR_Elast);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_AREA_RATIO_EDT, m_edtR_Area);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_EQUIV_ELAST_EDT, m_edtEqvElast);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_EQUIV_ELAST_UNIT, m_untEqvElast);

	DDX_Check(pDX, IDC_CMD_HCRP_CHINA_EQUIV_CHK, m_bEqiv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtChJTG16, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtChJTG16)
	ON_BN_CLICKED(IDC_CMD_HCRP_CHINA_EQUIV_CHK, OnEqivChk)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtChJTG16 message handlers

void CCMTdmtChJTG16::SetUnitType()
{ 
	m_edtCompStrength.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_untCompStrength.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_edtNotationSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_untNotationSize.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_untConcreteAge.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_edtAddFlyash.SetUnitType(D_UNITSYS_NONE);
	m_edtEqvElast.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dEqv_Elast);
	m_untEqvElast.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dEqv_Elast);
}

void CCMTdmtChJTG16::SetData2Dlg()
{ 
	m_edtCompStrength.SetEditUnit(m_pData->CHJTG04.dCompStrength);
	m_edtRelativeHumidity.SetValue(m_pData->CHJTG04.dRelativeHumidity);
	m_edtNotationSize.SetEditUnit(m_pData->CHJTG04.dMemberSize);
	m_edtCementTypeCoef.SetEditUnit(m_pData->CHJTG04.dCementTypeCoef);	
	m_edtConcreteAge.SetValue(m_pData->CHJTG04.dConcreteAge);
	m_edtAddFlyash.SetEditUnit(m_pData->CHJTG04.dAddFlyAsh);
	m_edtR_Elast.SetEditUnit(m_pData->CHJTG04.dR_Elast);
	m_edtR_Area.SetEditUnit(m_pData->CHJTG04.dR_Area);
	m_edtEqvElast.SetEditUnit(m_pData->CHJTG04.dEqv_Elast);
	m_bEqiv = m_pData->CHJTG04.bEqv;

	UpdateData(FALSE);
}

void CCMTdmtChJTG16::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->CHJTG04.dCompStrength     = m_edtCompStrength.GetEditValue();
	m_pData->CHJTG04.dRelativeHumidity = m_edtRelativeHumidity.GetEditValue();
	m_pData->CHJTG04.dMemberSize       = m_edtNotationSize.GetEditValue();
	m_pData->CHJTG04.dCementTypeCoef   = m_edtCementTypeCoef.GetEditValue();
	m_pData->CHJTG04.dConcreteAge      = m_edtConcreteAge.GetEditValue();
	m_pData->CHJTG04.dAddFlyAsh        = m_edtAddFlyash.GetEditValue();
	m_pData->CHJTG04.dR_Elast          = m_edtR_Elast.GetEditValue();
	m_pData->CHJTG04.dR_Area           = m_edtR_Area.GetEditValue();
	m_pData->CHJTG04.dEqv_Elast        = m_edtEqvElast.GetEditValue();
	m_pData->CHJTG04.bEqv              = m_bEqiv;
}
void CCMTdmtChJTG16::SetData2DlgByCodeChange(int nCode)
{
	CArray<UINT, UINT> aCtrlList01;  aCtrlList01.RemoveAll();
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_CHK);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_GRP);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_ELAST_RATIO_STC);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_ELAST_RATIO_EDT);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_AREA_RATIO_STC );
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_AREA_RATIO_EDT );
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_ELAST_STC);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_ELAST_EDT);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_ELAST_UNIT);

	CArray<UINT, UINT> aCtrlList02;  aCtrlList02.RemoveAll();
	aCtrlList02.Add(IDC_WG_CMD_STATIC8);   // Cement Type Coefficient (Bsc)
	aCtrlList02.Add(IDC_CMD_HCRP_CHINA_CEMENTTYPE); 
	aCtrlList02.Add(IDC_WG_CMD_STATIC5);   // Age of concrete at the beginning of shrinkage
	aCtrlList02.Add(IDC_CMD_HCRP_CHINA_AGE);
	aCtrlList02.Add(IDC_CMD_HCRP_CHINA_AGE_UNIT);	

	CArray<UINT, UINT> aCtrlList03;  aCtrlList03.RemoveAll();
	aCtrlList03.Add(IDC_CMD_HCRP_CHINA_FLY_ASH_STC);
	aCtrlList03.Add(IDC_CMD_HCRP_CHINA_FLY_ASH_EDT);
	aCtrlList03.Add(IDC_CMD_HCRP_CHINA_FLY_ASH_UNT);
		
	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_WG_CMD_STATIC8)->GetWindowRect(rRef);    // Cement Type Coefficient (Bsc)
	GetDlgItem(IDC_WG_CMD_STATIC5)->GetWindowRect(rToMove); // Age of concrete at the beginning of shrinkage
	int nStdY = rRef.top - rToMove.top;

	if (nCode == TDMT_CODE_CHINA_JTG_16)
	{
		GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowText(_LS(IDS_CMD_TDMT_CHJTG3362_18));
		CDlgUtil::CtrlShowHide(this, aCtrlList01, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, aCtrlList02, SW_SHOW);

		GetDlgItem(IDC_WG_CMD_STATIC5)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_HCRP_CHINA_FLY_ASH_STC)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top - nStdY;
		CDlgUtil::CtrlMoveDistY(this, aCtrlList03, nDistY);
	}
	else if (nCode == TDMT_CODE_CHINA_CHJTG_T_D65_2015)
	{
		GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowText(_LS(IDS_CMD_TDMT_CHJTG_T_D65_2015));
		CDlgUtil::CtrlShowHide(this, aCtrlList01, SW_SHOW);
		CDlgUtil::CtrlShowHide(this, aCtrlList02, SW_HIDE);

		GetDlgItem(IDC_WG_CMD_STATIC8)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_HCRP_CHINA_FLY_ASH_STC)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, aCtrlList03, nDistY);
	}
	else
		ASSERT(0);

	GetDlgItem(IDC_CMD_HCRP_CHINA_FLY_ASH_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_HCRP_CHINA_EQUIV_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top - nStdY;
	CDlgUtil::CtrlMoveDistY(this, aCtrlList01, nDistY);

    OnEqivChk();
}

BOOL CCMTdmtChJTG16::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
		
	ASSERT(m_pData != 0); // Please Set m_pData;

	SetUnitType();

	m_edtRelativeHumidity.SetRange(40, 99);
	m_edtRelativeHumidity.SetValue(70);

	m_edtConcreteAge.SetRange(0, 10000);
	m_edtConcreteAge.SetValue(3);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTdmtChJTG16::OnEqivChk()
{
	UpdateData(TRUE);

	CArray<UINT, UINT> aCtrlList01;  aCtrlList01.RemoveAll();
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_ELAST_RATIO_STC);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_ELAST_RATIO_EDT);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_AREA_RATIO_STC);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_AREA_RATIO_EDT);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_ELAST_STC);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_ELAST_EDT);
	aCtrlList01.Add(IDC_CMD_HCRP_CHINA_EQUIV_ELAST_UNIT);
	CDlgUtil::CtrlEnableDisable(this, aCtrlList01, m_bEqiv);

}