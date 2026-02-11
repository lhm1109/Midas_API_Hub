// CMTdmtJSCE07.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtJSCE07.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_base\wg_base_StrParser.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtJSCE07 dialog


CCMTdmtJSCE07::CCMTdmtJSCE07(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtJSCE07::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtJSCE07)
	//}}AFX_DATA_INIT
}


void CCMTdmtJSCE07::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtJSCE07)

	DDX_Control(pDX, IDC_CMD_JSCE07_RH_EDT          , m_jsceRh);  
	DDX_Control(pDX, IDC_CMD_JSCE07_VS_EDT          , m_jsceVs);
	DDX_Control(pDX, IDC_CMD_JSCE07_CC_EDT          , m_jsceCc);
	DDX_Control(pDX, IDC_CMD_JSCE07_WC_EDT          , m_jsceWc);
	DDX_Control(pDX, IDC_CMD_JSCE07_AC_EDT          , m_jsceAc);
	DDX_Control(pDX, IDC_CMD_JSCE07_COMP_EDT        , m_jsceComp );
	DDX_Control(pDX, IDC_CMD_JSCE07_ALPHA_EDT       , m_jsceAlpha);
	DDX_Control(pDX, IDC_CMD_JSCE07_AUTOSHR_GAMM_EDT, m_jsceGamma);
	DDX_Control(pDX, IDC_CMD_JSCE07_AUTOSHR_AFAC_EDT, m_jsceAFact);
	DDX_Control(pDX, IDC_CMD_JSCE07_AUTOSHR_BFAC_EDT, m_jsceBFact);

	DDX_Control(pDX, IDC_CMD_CC_UNIT         , m_jsceCcUnit);
	DDX_Control(pDX, IDC_CMD_WC_UNIT         , m_jsceWcUnit);
	DDX_Control(pDX, IDC_CMD_VS_UNIT         , m_jsceVsUnit);
	DDX_Control(pDX, IDC_CMD_JSCE07_COMP_UNIT, m_jsceCompUnit);

	DDX_Control(pDX, IDC_CMD_JSCE07_AUTOSHR_CHK, m_chkAutoShr);
	DDX_Control(pDX, IDC_CMD_JSCE07_GENSHR_CHK,  m_chkGenShr);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtJSCE07, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtJSCE07)	
	ON_BN_CLICKED(IDC_CMD_JSCE07_AUTOSHR_CHK, OnCmdAutoShrankageChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtJSCE07 message handlers
BOOL CCMTdmtJSCE07::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	ASSERT(m_pData != 0); // Please Set m_pData;

	SetUnitType();

	m_jsceRh.SetRange(40, 90);
	m_jsceRh.SetValue(70);

	m_jsceAc.SetRange(0, 10000);
	m_jsceAc.SetValue(3);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMTdmtJSCE07::OnCmdAutoShrankageChk()
{
	BOOL bAuto = m_chkAutoShr.GetCheck();

	GetDlgItem(IDC_CMD_JSCE07_AUTOSHR_GAMM_STC)->EnableWindow(bAuto);
	GetDlgItem(IDC_CMD_JSCE07_AUTOSHR_GAMM_EDT)->EnableWindow(bAuto);
	GetDlgItem(IDC_CMD_JSCE07_AUTOSHR_AFAC_STC)->EnableWindow(bAuto);
	GetDlgItem(IDC_CMD_JSCE07_AUTOSHR_AFAC_EDT)->EnableWindow(bAuto);
	GetDlgItem(IDC_CMD_JSCE07_AUTOSHR_BFAC_STC)->EnableWindow(bAuto);
	GetDlgItem(IDC_CMD_JSCE07_AUTOSHR_BFAC_EDT)->EnableWindow(bAuto);
}

//--------------------------------------------------------------------
//
void CCMTdmtJSCE07::SetUnitType()
{
	// Value
	m_jsceRh   .SetValue(D_UNITSYS_NONE);
	m_jsceVs   .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_jsceCc   .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);
	m_jsceWc   .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWaterContent);
	m_jsceAc   .SetValue(D_UNITSYS_NONE);
	m_jsceComp .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_jsceAlpha.SetUnitType(D_UNITSYS_NONE);
	m_jsceGamma.SetUnitType(D_UNITSYS_NONE);
	m_jsceAFact.SetUnitType(D_UNITSYS_NONE);
	m_jsceBFact.SetUnitType(D_UNITSYS_NONE);

	// Text
	m_jsceVsUnit  .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);  
	m_jsceCcUnit  .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);
	m_jsceWcUnit  .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWaterContent);
	m_jsceCompUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);


}

void CCMTdmtJSCE07::SetData2Dlg()
{ 
	m_jsceRh   .SetValue(m_pData->COMMON.dRelativeHumidity    );  // 
	m_jsceVs   .SetEditUnit(m_pData->JPJSCE07.dVolumeSurfaceRatio);
	m_jsceCc   .SetEditUnit(m_pData->JPJSCE07.dCementContent     );
	m_jsceWc   .SetEditUnit(m_pData->JPJSCE07.dWaterContent      );
	m_jsceAc   .SetValue(m_pData->COMMON.dConcreteAge         );   //
	m_jsceComp .SetEditUnit(m_pData->COMMON.dCompStrength        );  //
	m_jsceAlpha.SetEditUnit(m_pData->JPJSCE07.dAlpaFactor        );
	m_jsceGamma.SetEditUnit(m_pData->JPJSCE07.dGammaFactor       );
	m_jsceAFact.SetEditUnit(m_pData->JPJSCE07.dAFactor           );
	m_jsceBFact.SetEditUnit(m_pData->JPJSCE07.dBFactor           );

	m_chkAutoShr.SetCheck(m_pData->JPJSCE07.bAutoShrankage);
	m_chkGenShr.SetCheck(m_pData->JPJSCE07.bGeneralShrankage);

	OnCmdAutoShrankageChk();

	UpdateData(FALSE);
}

void CCMTdmtJSCE07::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->COMMON.dRelativeHumidity     = m_jsceRh   .GetEditValue();
	m_pData->JPJSCE07.dVolumeSurfaceRatio = m_jsceVs   .GetEditValue();
	m_pData->JPJSCE07.dCementContent      = m_jsceCc   .GetEditValue();
	m_pData->JPJSCE07.dWaterContent       = m_jsceWc   .GetEditValue();
	m_pData->COMMON.dConcreteAge          = m_jsceAc   .GetEditValue();
	m_pData->COMMON.dCompStrength         = m_jsceComp .GetEditValue();
	m_pData->JPJSCE07.dAlpaFactor         = m_jsceAlpha.GetEditValue();
	m_pData->JPJSCE07.dGammaFactor        = m_jsceGamma.GetEditValue();
	m_pData->JPJSCE07.dAFactor            = m_jsceAFact.GetEditValue();
	m_pData->JPJSCE07.dBFactor            = m_jsceBFact.GetEditValue();
	
	m_pData->JPJSCE07.bAutoShrankage    =  m_chkAutoShr.GetCheck();
	m_pData->JPJSCE07.bGeneralShrankage =  m_chkGenShr.GetCheck();
}
