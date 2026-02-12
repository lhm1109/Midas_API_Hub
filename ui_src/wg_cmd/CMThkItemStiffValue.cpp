// CMThkItemStiffValue.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThkItemStiffValue.h"

#include "..\wg_db\wg_db_SectUtil.h"
#include "CMThkItemStiffDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMThkItemStiffDlg*	CCMThkItemStiffValue::m_pParent;
/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffValue dialog


CCMThkItemStiffValue::CCMThkItemStiffValue(int nType, CWnd* pParent /*=NULL*/)
	: CDlgChild(CCMThkItemStiffValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMThkItemStiffValue)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParent = (CCMThkItemStiffDlg*)pParent;
	m_nIdentity = nType;
	m_pImageList = 0;
}

CCMThkItemStiffValue::~CCMThkItemStiffValue()
{
	if (m_pImageList != 0) delete m_pImageList;
}

void CCMThkItemStiffValue::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThkItemStiffValue)
	DDX_Control(pDX, IDC_CMD_TP_ID_UNIT4, m_unit4);
	DDX_Control(pDX, IDC_CMD_TP_ID_UNIT3, m_unit3);
	DDX_Control(pDX, IDC_CMD_TP_ID_UNIT2, m_unit2);
	DDX_Control(pDX, IDC_CMD_TP_ID_UNIT1, m_unit1);
	DDX_Control(pDX, IDC_CMD_TP_ID_VALUE4, m_value4);
	DDX_Control(pDX, IDC_CMD_TP_ID_VALUE3, m_value3);
	DDX_Control(pDX, IDC_CMD_TP_ID_VALUE2, m_value2);
	DDX_Control(pDX, IDC_CMD_TP_ID_VALUE1, m_value1);
	DDX_Control(pDX, IDC_CMD_TP_STIFFVALUE_TYPE, m_cboType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThkItemStiffValue, CDlgChild)
	//{{AFX_MSG_MAP(CCMThkItemStiffValue)
	ON_CBN_SELCHANGE(IDC_CMD_TP_STIFFVALUE_TYPE, OnSelchangeCmdTpIdType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemStiffValue message handlers


/////////////////////////////////////////////////////////////////////////////
// Initialize Data
BOOL CCMThkItemStiffValue::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	m_unit1.SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.ThickIn);
	m_unit2.SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.ThickOut);
	m_unit3.SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.Hu);
	m_unit4.SetUnitType(CUnitCtrl::m_THIK_UNIT.Stiffened.Value.Hl);

	// TODO: Add extra initialization here
	InitControls();
	InitialDBData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThkItemStiffValue::InitialDBData()
{
	if(m_nIdentity == 0) // YZ Section
		YZInitialDBData();
	else // XZ Section
		XZInitialDBData();
}

void CCMThkItemStiffValue::YZInitialDBData()
{
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_pParent->m_sDlgData.YZValue_Shape);
	m_cboType.SetCurSel(nShapeIndex);
	m_value1.SetEditUnit(m_pParent->m_sDlgData.YZValue_ThickIn);
	m_value2.SetEditUnit(m_pParent->m_sDlgData.YZValue_ThickOut);
	m_value3.SetEditUnit(m_pParent->m_sDlgData.YZValue_Hu);
	m_value4.SetEditUnit(m_pParent->m_sDlgData.YZValue_Hl);
}

void CCMThkItemStiffValue::XZInitialDBData()
{
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_pParent->m_sDlgData.XZValue_Shape);
	m_cboType.SetCurSel(nShapeIndex);
	m_value1.SetEditUnit(m_pParent->m_sDlgData.XZValue_ThickIn);
	m_value2.SetEditUnit(m_pParent->m_sDlgData.XZValue_ThickOut);
	m_value3.SetEditUnit(m_pParent->m_sDlgData.XZValue_Hu);
	m_value4.SetEditUnit(m_pParent->m_sDlgData.XZValue_Hl);
}

void CCMThkItemStiffValue::InitControls()
{
	// Init shape combo
	SetSectionShapeCombo();
	m_cboType.SetCurSel(0);
}

void CCMThkItemStiffValue::SetSectionShapeCombo()
{
	std::vector<CString> aBitmapID;
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\ThkRegIcon01.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon02.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon03.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\ThkRegIcon04.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon05.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon06.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\ThkRegIcon07.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon08.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon09.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon10.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon11.svg"));
	aBitmapID.push_back(_T("SVG\\Illustration\\ComboBox\\SecRegIcon12.svg"));

	CString aItemName[] = {
		_LS(IDS_WG_CMD__ADDD__Angle), _LS(IDS_WG_CMD__ADDD__Channel), _LS(IDS_WG_CMD__ADDD__H_Section), _LS(IDS_WG_CMD__ADDD__T_Section), _LS(IDS_WG_CMD__ADDD__Box), _LS(IDS_WG_CMD__ADDD__Pipe), 
		_LS(IDS_WG_CMD__ADDD__Double_Angle), _LS(IDS_WG_CMD__ADDD__Double_Channel), _LS(IDS_WG_CMD__ADDD__Solid_Rectangle), _LS(IDS_WG_CMD__ADDD__Solid_Round),
		_LS(IDS_WG_CMD__ADDD__Cold_Formed_Channel), _LS(IDS_WG_CMD__ADD2__U_RIB)
	};
	//if (CLocaleLib::IsEnglish()) // 영어권만(일본은 H로 씀)
	#if defined(_US) || defined(_RUS)
	{
		aItemName[2] = _LS(IDS_WG_CMD__ADDD__I_Section);
	}
	#endif

	int nCount = 12;

	for (int i = 0; i < nCount; i++)
	{
		m_cboType.AddString(aItemName[i], 0);
	}
	m_cboType.SetImageList(aBitmapID);
	m_cboType.SetComboHeight(globalUtils.ScaleByDPI(24));
}

int CCMThkItemStiffValue::GetCurShapeIndex()
{
	int nShapeIndex = m_cboType.GetCurSel();
	if (nShapeIndex == LB_ERR) return LB_ERR;

	return nShapeIndex;
}
/////////////////////////////////////////////////////////////////////////////







void CCMThkItemStiffValue::OnSelchangeCmdTpIdType() 
{
/*
	// TODO: Add your control notification handler code here
	int nShapeIndex = GetCurShapeIndex();
	if (nShapeIndex < 0) return;
	int nCurShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex == nCurShapeIndex) return;

	// Save new shape in data
	m_Data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);

	T_SECT_SECTBASE_D *pSectF;
	pSectF = &m_Data.SectBefore.SectI;

	// Section Name, DB Name은 보존
	m_wndName.GetWindowText(m_Data.SName);
	CString csDBNameF = pSectF->DBName;

	// First 초기화
	pSectF->Initialize();
	pSectF->DBName = csDBNameF;

	// Dialog에 반영
	ShowData2Dlg();	
*/
}

BOOL CCMThkItemStiffValue::OnApplyPublic()
{
	if(m_nIdentity == 0) // YZ Section
	{
		int nShapeIndex = GetCurShapeIndex();
		CString shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
		m_pParent->m_sDlgData.YZValue_Shape = shape;
		m_pParent->m_sDlgData.YZValue_ThickIn = m_value1.GetEditValue();
		m_pParent->m_sDlgData.YZValue_ThickOut = m_value2.GetEditValue();
		m_pParent->m_sDlgData.YZValue_Hu = m_value3.GetEditValue();
		m_pParent->m_sDlgData.YZValue_Hl = m_value4.GetEditValue();
	}
	else // XZ Section
	{
		int nShapeIndex = GetCurShapeIndex();
		CString shape = CSectUtil::GetShapeNameFromIndexReg(nShapeIndex);
		m_pParent->m_sDlgData.XZValue_Shape = shape;
		m_pParent->m_sDlgData.XZValue_ThickIn = m_value1.GetEditValue();
		m_pParent->m_sDlgData.XZValue_ThickOut = m_value2.GetEditValue();
		m_pParent->m_sDlgData.XZValue_Hu = m_value3.GetEditValue();
		m_pParent->m_sDlgData.XZValue_Hl = m_value4.GetEditValue();
	}

	return TRUE;
}
