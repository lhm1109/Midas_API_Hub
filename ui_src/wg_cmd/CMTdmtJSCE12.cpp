// CMTdmtJSCE12.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmtJSCE12.h"

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
// CCMTdmtJSCE12 dialog


CCMTdmtJSCE12::CCMTdmtJSCE12(CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMTdmtJSCE12::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTdmtJSCE12)
	//}}AFX_DATA_INIT
}


void CCMTdmtJSCE12::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTdmtJSCE12)

	DDX_Control(pDX, IDC_CMD_JSCE12_RH_EDT          , m_jsceRh);  
	DDX_Control(pDX, IDC_CMD_JSCE12_VS_EDT          , m_jsceVs);
	DDX_Control(pDX, IDC_CMD_JSCE12_CC_EDT          , m_jsceCc);
	DDX_Control(pDX, IDC_CMD_JSCE12_WC_EDT          , m_jsceWc);
	DDX_Control(pDX, IDC_CMD_JSCE12_AC_EDT          , m_jsceAc);
	DDX_Control(pDX, IDC_CMD_JSCE12_COMP_EDT        , m_jsceComp );
	DDX_Control(pDX, IDC_CMD_JSCE12_IMPACTFAC_EDT   , m_jsceImpact);
	DDX_Control(pDX, IDC_CMD_JSCE12_AGESOL_EDT      , m_jsceAgeSol);
	DDX_Control(pDX, IDC_CMD_CC_UNIT                , m_jsceCcUnit);
	DDX_Control(pDX, IDC_CMD_WC_UNIT                , m_jsceWcUnit);
	DDX_Control(pDX, IDC_CMD_VS_UNIT                , m_jsceVsUnit);
	DDX_Control(pDX, IDC_CMD_JSCE12_COMP_UNIT       , m_jsceCompUnit);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMTdmtJSCE12, CDlgChild)
	//{{AFX_MSG_MAP(CCMTdmtJSCE12)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtJSCE12 message handlers
BOOL CCMTdmtJSCE12::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	ASSERT(m_pData != 0); // Please Set m_pData;

	SetUnitType();

	m_jsceRh.SetRange(40, 90);
	m_jsceRh.SetValue(70);

	m_jsceAc.SetRange(0, 10000);
	m_jsceAc.SetValue(3);

	m_jsceAgeSol.SetRange(0, 10000);
	m_jsceAgeSol.SetValue(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//--------------------------------------------------------------------
//
void CCMTdmtJSCE12::SetUnitType()
{
	// Value
	m_jsceVs   .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_jsceCc   .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);
	m_jsceWc   .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWaterContent);
	m_jsceComp .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_jsceImpact.SetUnitType(D_UNITSYS_NONE);

	// Text
	m_jsceVsUnit  .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);  
	m_jsceCcUnit  .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);
	m_jsceWcUnit  .SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWaterContent);
	m_jsceCompUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
}

void CCMTdmtJSCE12::SetData2Dlg()
{ 
	m_jsceRh    .SetValue(m_pData->COMMON.dRelativeHumidity    );  // 
	m_jsceVs    .SetEditUnit(m_pData->JPJSCE12.dVolumeSurfaceRatio);
	m_jsceCc    .SetEditUnit(m_pData->JPJSCE12.dCementContent     );
	m_jsceWc    .SetEditUnit(m_pData->JPJSCE12.dWaterContent      );
	m_jsceAc    .SetValue(m_pData->COMMON.dConcreteAge         );   //
	m_jsceComp  .SetEditUnit(m_pData->COMMON.dCompStrength        );  //
	m_jsceImpact.SetEditUnit(m_pData->JPJSCE12.dImpactFactor      );
	m_jsceAgeSol.SetValue(m_pData->JPJSCE12.dConcreteAgeSol    );

	UpdateData(FALSE);
}

void CCMTdmtJSCE12::SetDlg2Data()
{
	UpdateData(TRUE);

	m_pData->COMMON.dRelativeHumidity     = m_jsceRh   .GetEditValue();
	m_pData->JPJSCE12.dVolumeSurfaceRatio = m_jsceVs   .GetEditValue();
	m_pData->JPJSCE12.dCementContent      = m_jsceCc   .GetEditValue();
	m_pData->JPJSCE12.dWaterContent       = m_jsceWc   .GetEditValue();
	m_pData->COMMON.dConcreteAge          = m_jsceAc   .GetEditValue();
	m_pData->COMMON.dCompStrength         = m_jsceComp .GetEditValue();
	m_pData->JPJSCE12.dImpactFactor       = m_jsceImpact.GetEditValue();
	m_pData->JPJSCE12.dConcreteAgeSol     = m_jsceAgeSol.GetEditValue();
}
