// IehpYieldPropDlgNew.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpYieldPropDlgNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "IehpYieldPropGrid.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DbToolPmcv.h"
/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropDlgNew dialog


CIehpYieldPropDlgNew::CIehpYieldPropDlgNew(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpYieldPropDlgNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpYieldPropDlgNew)
	m_nPMInputType = 0;
	m_nYLDInputType = 0;
	m_nValueTypeSymmetry = 0;
	m_strAlpha = _T("");
	m_str1Gamma = _T("");
	m_str2Gamma = _T("");
	m_str3Gamma = _T("");
	m_nPmcv = 0;
	//}}AFX_DATA_INIT
	m_bAssignProperties = FALSE;
	m_bUnable1stCtrl = FALSE;
	m_MainData.Initialize();
	m_SubData.Initialize();

	m_wndGrid = NULL;
	m_wndGrid = new CIehpYieldPropGrid(this);

	m_nSectType = 1;
	m_bUseMz        = TRUE;
	m_bUse3rdCtrl = FALSE;
	m_bShowValueSet = FALSE;
}

CIehpYieldPropDlgNew::~CIehpYieldPropDlgNew()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CIehpYieldPropDlgNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpYieldPropDlgNew)
	DDX_Control(pDX, IDC_HINGE_TAB1, m_Tab);
	DDX_Control(pDX, IDC_PLOT_COMB, m_cmbPlot);
	DDX_Control(pDX, IDC_1ST_PM_CRACK_PC0T_EDT, m_edtPC0);
	DDX_Radio(pDX, IDC_INPUT_TYPE_USER_RDO, m_nPMInputType);
	DDX_Radio(pDX, IDC_YIELD_INPUT_TYPE_USER_RDO, m_nYLDInputType);
	DDX_Text(pDX, IDC_ALPHA_EDT, m_strAlpha);
	DDX_Text(pDX, IDC_1ST_GAMMA, m_str1Gamma);
	DDX_Text(pDX, IDC_2ND_GAMMA, m_str2Gamma);
	DDX_Text(pDX, IDC_3RD_GAMMA, m_str3Gamma);
	DDX_Radio(pDX, IDC_TYPE_SYM_RDO2, m_nValueTypeSymmetry);
	DDX_Radio(pDX, IDC_PMCV_1ST, m_nPmcv);
	DDX_Control(pDX, IDC_SHOWVALUE_CHK, m_chkShowValue);


	if (m_nPmcv == 2) m_nPmcv = 3;

	//}}AFX_DATA_MAP
	UINT nMC0[] = { IDC_1ST_PM_CRACK_MC0Y_EDT, IDC_1ST_PM_CRACK_MC0Z_EDT };
	UINT nPC[]  = { IDC_1ST_PM_PCT_EDT,        IDC_1ST_PM_PCC_EDT };
	UINT nPCB[] = { IDC_1ST_PM_PCBY_EDT,       IDC_1ST_PM_PCBZ_EDT };
	UINT nMC[]  = { IDC_1ST_PM_MCY_EDT,        IDC_1ST_PM_MCZ_EDT };
	UINT nPY[]  = { IDC_2ND_PM_PYT_EDT,        IDC_2ND_PM_PYC_EDT };
	UINT nPYB[] = { IDC_2ND_PM_PYBY_EDT,       IDC_2ND_PM_PYBZ_EDT };
	UINT nMY[]  = { IDC_2ND_PM_MYY_EDT,        IDC_2ND_PM_MYZ_EDT };
	UINT nPU[]  = { IDC_3RD_PM_PUT_EDT,        IDC_3RD_PM_PUC_EDT };
	UINT nPUB[] = { IDC_3RD_PM_PUBY_EDT,       IDC_3RD_PM_PUBZ_EDT };
	UINT nMU[]  = { IDC_3RD_PM_MUY_EDT,        IDC_3RD_PM_MUZ_EDT };
	UINT nBetay1st[] = { IDC_1ST_BETAY_T,      IDC_1ST_BETAY_C };     // 0: T, 1: C
	UINT nBetay2nd[] = { IDC_2ND_BETAY_T,      IDC_2ND_BETAY_C };     // 0: T, 1: C
	UINT nBetay3rd[] = { IDC_3RD_BETAY_T,      IDC_3RD_BETAY_C };     // 0: T, 1: C
	UINT nBetaz1st[] = { IDC_1ST_BETAZ_T,      IDC_1ST_BETAZ_C };     // 0: T, 1: C
	UINT nBetaz2nd[] = { IDC_2ND_BETAZ_T,      IDC_2ND_BETAZ_C };     // 0: T, 1: C
	UINT nBetaz3rd[] = { IDC_3RD_BETAZ_T,      IDC_3RD_BETAZ_C };     // 0: T, 1: C

	for(int i=0; i<2; i++)
	{
		DDX_Control(pDX, nMC0[i],   m_edtMC0[i]);
		DDX_Control(pDX, nPC[i],    m_edtPC[i]);
		DDX_Control(pDX, nPCB[i],   m_edtPCB[i]);
		DDX_Control(pDX, nMC[i],    m_edtMC[i]);
		DDX_Control(pDX, nPY[i],    m_edtPY[i]);
		DDX_Control(pDX, nPYB[i],   m_edtPYB[i]);
		DDX_Control(pDX, nMY[i],    m_edtMY[i]);
		DDX_Text(pDX, nBetay1st[i], m_strBetay1st[i]);
		DDX_Text(pDX, nBetay2nd[i], m_strBetay2nd[i]);
		DDX_Text(pDX, nBetaz1st[i], m_strBetaz1st[i]);
		DDX_Text(pDX, nBetaz2nd[i], m_strBetaz2nd[i]);
		DDX_Control(pDX, nPU[i], m_edtPU[i]);
		DDX_Control(pDX, nPUB[i], m_edtPUB[i]);
		DDX_Control(pDX, nMU[i], m_edtMU[i]);
		DDX_Text(pDX, nBetay3rd[i], m_strBetay3rd[i]);
		DDX_Text(pDX, nBetaz3rd[i], m_strBetaz3rd[i]);
	}

	DDX_Control(pDX, IDC_IEHP_TABLE, *m_wndGrid);
	DDX_Control(pDX, IDC_WG_CMD_STATIC27, m_wndFormulaPicture);
	DDX_Control(pDX, IDC_IMAGE, m_wndLegendPicture);
}

BEGIN_MESSAGE_MAP(CIehpYieldPropDlgNew, CDialogMove)
	//{{AFX_MSG_MAP(CIehpYieldPropDlgNew)
	ON_BN_CLICKED(IDC_INPUT_TYPE_USER_RDO, OnPMInputType)
	ON_BN_CLICKED(IDC_YIELD_INPUT_TYPE_AUTO_RDO, OnYLDInputType)
	ON_CBN_SELCHANGE(IDC_PLOT_COMB, OnSelchangePlotComb)
	ON_BN_CLICKED(IDC_INPUT_TYPE_AUTO_RDO, OnPMInputType)
	ON_BN_CLICKED(IDC_YIELD_INPUT_TYPE_USER_RDO, OnYLDInputType)

	ON_EN_CHANGE(IDC_1ST_BETAY_C, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_BETAY_T, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_BETAZ_C, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_BETAZ_T, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_GAMMA, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_CRACK_MC0Y_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_CRACK_MC0Z_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_CRACK_PC0T_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_MCY_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_MCZ_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_PCBY_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_PCBZ_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_PCC_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_1ST_PM_PCT_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_BETAY_C, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_BETAY_T, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_BETAZ_C, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_BETAZ_T, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_GAMMA, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_PM_MYY_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_PM_MYZ_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_PM_PYBY_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_PM_PYBZ_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_PM_PYC_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_2ND_PM_PYT_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_BETAY_C, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_BETAY_T, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_BETAZ_C, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_BETAZ_T, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_GAMMA, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_PM_MUY_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_PM_MUZ_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_PM_PUBY_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_PM_PUBZ_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_PM_PUC_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_3RD_PM_PUT_EDT, OnChangeEditBox)
	ON_EN_CHANGE(IDC_ALPHA_EDT, OnChangeEditBox)

	ON_EN_KILLFOCUS(IDC_1ST_BETAY_C, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_BETAY_T, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_BETAZ_C, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_BETAZ_T, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_GAMMA, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_CRACK_MC0Y_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_CRACK_MC0Z_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_CRACK_PC0T_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_MCY_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_MCZ_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_PCBY_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_PCBZ_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_PCC_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_1ST_PM_PCT_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_BETAY_C, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_BETAY_T, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_BETAZ_C, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_BETAZ_T, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_GAMMA, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_PM_MYY_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_PM_MYZ_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_PM_PYBY_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_PM_PYBZ_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_PM_PYC_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_2ND_PM_PYT_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_BETAY_C, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_BETAY_T, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_BETAZ_C, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_BETAZ_T, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_GAMMA, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_PM_MUY_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_PM_MUZ_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_PM_PUBY_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_PM_PUBZ_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_PM_PUC_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_3RD_PM_PUT_EDT, OnKillFocusEditBox)
	ON_EN_KILLFOCUS(IDC_ALPHA_EDT, OnKillFocusEditBox)

	ON_BN_CLICKED(IDC_PMCV_1ST, OnPmcvGrid)
	ON_BN_CLICKED(IDC_PMCV_2ND, OnPmcvGrid)
	ON_BN_CLICKED(IDC_PMCV_3RD, OnPmcvGrid)

	ON_NOTIFY(TCN_SELCHANGING, IDC_HINGE_TAB1, OnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE  , IDC_HINGE_TAB1, OnSelchangeTab)
	ON_BN_CLICKED(IDC_TYPE_SYM_RDO2,					OnValueTypeSymmetry)
	ON_BN_CLICKED(IDC_TYPE_ASYM_RDO2,					OnValueTypeSymmetry)
	ON_BN_CLICKED(IDC_SHOWVALUE_CHK, OnShowValueChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropDlgNew message handlers

BOOL CIehpYieldPropDlgNew::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_wndGrid->SetDefaultData(m_bConc);
	m_wndGrid->Initialize();

	if(!m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arUser,				TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arPMInput,		TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arYLDInput,		TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge,	TRUE);
	}

	if(m_bAssignProperties == TRUE) // Auto
	{
		if(m_ElemK < 1)	
		{
			ASSERT(0);
			return FALSE;		//Assign 시에는 해당 키값이 1이상이 무조건 들어와야함
		}
		CDBDoc::GetDocPoint()->m_pAttrCtrl->CalcIehpYield(IehpData, m_ElemK,m_MainData,m_SubData);

		// 2014-07-29. by Hsshim. MQC 4836-58의 3번. 자동 계산된 값으로 Update
//     m_MainData = IehpData.PROP[6].YLDSUR;
//     m_SubData  = IehpData.SubPROP[6].YLDSUR;
	}

	InitCtrl();
	InitTabCtrl();
	InitGraph();

	m_chkShowValue.SetCheck(FALSE); // not associate with data

	WndMessageCtrl();
	SetInitData();
	Data2Dlg();

	
	if(m_bConc || m_bUnable1stCtrl) { m_nPmcv = 1; UpdateData(FALSE); }
	OnPmcvGrid();

	OnPMInputType();
	OnYLDInputType();

	if(!CheckDataCtrl(m_Tab.GetCurSel())) return TRUE;
	UpdateGraph(m_cmbPlot.GetCurSel());
	if(m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arUser,				FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arPMInput,		FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arYLDInput,		FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge,	FALSE);
	}
	
	const T_IEHP_YLDSUR& Data = m_Tab.GetCurSel()==D_TAB_DEFALUT ? m_MainData : m_SubData;
	for (int ia=0; ia<2; ++ia)
	{
		dPC[ia]  = Data.dPC[ia];
		dPCB[ia] = Data.dPCB[ia];
		dMC[ia]  = Data.dMC[ia];
		dPY[ia]  = Data.dPY[ia];
		dPYB[ia] = Data.dPYB[ia];
		dMY[ia]  = Data.dMY[ia];
		dPU[ia]  = Data.dPU[ia];
		dPUB[ia] = Data.dPUB[ia];
		dMU[ia]  = Data.dMU[ia];
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIehpYieldPropDlgNew::OnOK() 
{
	// TODO: Add extra validation here
	Dlg2Data(TRUE);
	if(!CheckDataCtrl(m_Tab.GetCurSel())) return;

	if(IehpData.nHingeType == 0 && m_nPMInputType == 1)
	{
		TabDlg2Data(D_TAB_SUB    , m_SubData, TRUE);
		if(!CheckDataCtrl(D_TAB_SUB)) return;
	}

	int nTabCount = m_Tab.GetItemCount();
	if(nTabCount == 1)	
		m_bExistIJData = FALSE;	
	else if(nTabCount == 2)
		m_bExistIJData = TRUE;

	CDialogMove::OnOK();
}

void CIehpYieldPropDlgNew::OnPMInputType() 
{
	UpdateData();
	Dlg2Data(TRUE);

	T_IEHP_YLDSUR Data;

	if(m_Tab.GetCurSel()==D_TAB_DEFALUT)
		Data = m_MainData;
	else if(m_Tab.GetCurSel()==D_TAB_SUB)
		Data = m_SubData;

	if(IehpData.nHingeType == 0 && m_nPMInputType == 0)
	{
		GetDlgItem(IDC_STATIC_IEND_JEND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TYPE_SYM_RDO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TYPE_ASYM_RDO2)->ShowWindow(SW_SHOW);
		if(m_nValueTypeSymmetry == -1)
		{
			m_nValueTypeSymmetry = 0;
			UpdateData(FALSE);
			ChangeTabCtrl();
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_IEND_JEND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TYPE_SYM_RDO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TYPE_ASYM_RDO2)->ShowWindow(SW_HIDE);
		if(IehpData.nHingeType == 0 && m_nPMInputType == 1 && m_nValueTypeSymmetry == 1 && m_bAssignProperties == TRUE)
		{

		}
		else 
		{

			m_nValueTypeSymmetry = -1;
			m_bExistIJData = FALSE;
			UpdateData(FALSE);
			ChangeTabCtrl();
		}
	}
	
	//ChangeTabCtrl();
	
	if(m_nPMInputType==1 && m_bAssignProperties)
	{
		if(!CheckDataCtrl(m_Tab.GetCurSel())) goto GT_RETURN_FALSE;
	}
	PMData2Dlg(Data);

	if(!CheckDataCtrl(m_Tab.GetCurSel(),FALSE)) return;
	UpdateGraph(m_cmbPlot.GetCurSel());

	CDlgUtil::CtrlEnableDisable(this, m_arPMInput, m_nPMInputType==0);

	if(m_Tab.GetCurSel()==D_TAB_DEFALUT)
		m_MainData = Data;
	else if(m_Tab.GetCurSel()==D_TAB_SUB)
		m_SubData = Data;

	return;

GT_RETURN_FALSE:
	m_nPMInputType = 0;
	UpdateData(FALSE);
}

void CIehpYieldPropDlgNew::OnYLDInputType() 
{
	UpdateData();
	Dlg2Data(TRUE);

	T_IEHP_YLDSUR Data;
	if(m_Tab.GetCurSel()==D_TAB_DEFALUT)
		Data = m_MainData;
	else if(m_Tab.GetCurSel()==D_TAB_SUB)
		Data = m_SubData;

	
	
	if(m_nYLDInputType==1 && m_bAssignProperties)
	{
		if(!CheckDataCtrl(m_Tab.GetCurSel())) goto GT_RETURN_FALSE;

		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		int nMat = 0;
		if(m_bConc) nMat = 1;
		else
		{
			nMat = 0;
		}

		if(!pDoc->m_pEditData->GetIehpShapeAprxYieldSurf(Data, nMat)) goto GT_RETURN_FALSE;
	}
	
	
	CDlgUtil::CtrlEnableDisable(this, m_arYLDInput, m_nYLDInputType==0);
	YLDData2Dlg(Data);
	
	if(!CheckDataCtrl(m_Tab.GetCurSel(),FALSE)) return;
	UpdateGraph(m_cmbPlot.GetCurSel());

	if(m_Tab.GetCurSel()==D_TAB_DEFALUT)
		m_MainData = Data;
	else if(m_Tab.GetCurSel()==D_TAB_SUB)
		m_SubData = Data;

	return;

GT_RETURN_FALSE:
	m_nYLDInputType = 0;
	UpdateData(FALSE);
}

void CIehpYieldPropDlgNew::SetInitData()
{
	m_edtPC0.SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPC0);
	for(int i=0; i<2; i++)
	{
		m_edtMC0[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dMC0);
		m_edtPC[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPC);
		m_edtPCB[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPCB);
		m_edtMC[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dMC);
		m_edtPY[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPY);
		m_edtPYB[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPYB);
		m_edtMY[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dMY);
		m_edtPU[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPU);
		m_edtPUB[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dPUB);
		m_edtMU[i].SetUnitType(CUnitCtrl::m_IEHP_UNIT.PROP.dMU);
	}

	m_cmbPlot.ResetContent();
#if defined(_JP)
	m_cmbPlot.AddString(_T("N-My"));
	m_cmbPlot.AddString(_T("N-Mz"));
#else
	m_cmbPlot.AddString(_T("P-My"));
	m_cmbPlot.AddString(_T("P-Mz"));
#endif
	m_cmbPlot.AddString(_T("My-Mz"));
	m_cmbPlot.SetCurSel(0);
}

void CIehpYieldPropDlgNew::WndMessageCtrl()
{
	switch(m_nMessage)
	{
	case 0:
		this->SetWindowText(_LS(IDS_CMD_IEHP_CONC));
		break;
	case 1:
		this->SetWindowText(_LS(IDS_CMD_IEHP_STEL));
		break;
	default: ASSERT(0); break;
	}

	GetDlgItem(IDC_INPUT_TYPE_USER_RDO)->EnableWindow(TRUE);
	GetDlgItem(IDC_INPUT_TYPE_AUTO_RDO)->EnableWindow(TRUE);

	if(m_bConc) GetDlgItem(IDC_STATIC2)->SetWindowText(_LS(IDS_CMD_IEHP_RC_STC2));
	else        GetDlgItem(IDC_STATIC2)->SetWindowText(_LS(IDS_CMD_IEHP_STL_STC2));

}

void CIehpYieldPropDlgNew::Data2Dlg()
{
	if     (m_Tab.GetCurSel()==D_TAB_DEFALUT)  Data2TabDlg(D_TAB_DEFALUT, m_MainData);
	else if(m_Tab.GetCurSel()==D_TAB_SUB)      Data2TabDlg(D_TAB_SUB    , m_SubData);

	ChangeDlgCtrls();
}

void CIehpYieldPropDlgNew::PMData2Dlg(T_IEHP_YLDSUR& Data)
{
	m_nPMInputType = Data.nCurveUserAuto;
	m_edtPC0.SetEditUnit(Data.dPC0);
	for(int i=0; i<2; i++)
	{
		if(!m_bUseMz && i==1)
		{
			m_edtMC0[i].SetEditUnit(0.0);
			m_edtPCB[i].SetEditUnit(0.0);
			m_edtPYB[i].SetEditUnit(0.0);
			m_edtMC[i] .SetEditUnit(0.0);
			m_edtMY[i] .SetEditUnit(0.0);
		}
		else
		{
			m_edtMC0[i].SetEditUnit(Data.dMC0[i]);
			m_edtPCB[i].SetEditUnit(Data.dPCB[i]);
			m_edtPYB[i].SetEditUnit(Data.dPYB[i]);
			m_edtMC[i] .SetEditUnit(Data.dMC[i]);
			m_edtMY[i] .SetEditUnit(Data.dMY[i]);
		}
		
		m_edtPC[i].SetEditUnit(Data.dPC[i]);
		//m_edtPCB[i].SetEditUnit(Data.dPCB[i]);
		//m_edtMC[i].SetEditUnit(Data.dMC[i]);
		m_edtPY[i].SetEditUnit(Data.dPY[i]);
		//m_edtPYB[i].SetEditUnit(Data.dPYB[i]);
		//m_edtMY[i].SetEditUnit(Data.dMY[i]);

		m_edtPU[i].SetEditUnit(Data.dPU[i]);
		m_edtPUB[i].SetEditUnit(Data.dPUB[i]);
		m_edtMU[i].SetEditUnit(Data.dMU[i]);
	}

	GetDlgItem(IDC_INPUT_TYPE_AUTO_RDO)->EnableWindow(TRUE);
	if(m_nSectType==2) m_nPMInputType=0;
	UpdateData(FALSE);

	BOOL bAuto = (m_nPMInputType == 1) ? TRUE:FALSE;

	for(int i=0; i<2; i++) for(int j=0; j<3; j++)
	{
		if(!m_bUseMz && i==1)
		{
			Data.dP1st[i][j*5] = 0.;
			Data.dP2nd[i][j*5] = 0.;
			Data.dP3rd[i][j*5] = 0.;
			Data.dM1st[i][j*5] = 0.;
			Data.dM2nd[i][j*5] = 0.;
			Data.dM3rd[i][j*5] = 0.;
		}
		else
		{
			if(j==1)
			{
				Data.dP1st[i][j*5] = 0.;
				Data.dP2nd[i][j*5] = 0.;
				Data.dP3rd[i][j*5] = 0.;
				Data.dM1st[i][j*5] = 1.;
				Data.dM2nd[i][j*5] = 1.;
				Data.dM3rd[i][j*5] = 1.;
			}
			else
			{
				Data.dP1st[i][j*5] = 1.;
				Data.dP2nd[i][j*5] = 1.;
				Data.dP3rd[i][j*5] = 1.;
				Data.dM1st[i][j*5] = 0.;
				Data.dM2nd[i][j*5] = 0.;
				Data.dM3rd[i][j*5] = 0.;
			}
		}
		
	}

	if (m_bShowValueSet)
	{
		double dP1st[2][11];
		double dM1st[2][11];
		double dP2nd[2][11];
		double dM2nd[2][11];
		double dP3rd[2][11];
		double dM3rd[2][11];
		auto ScaleDataAccordingToShowValue = [] (double dPaxis[]/*[2]*/, double dPb[]/*[2]*/, double dMb[]/*[2]*/,
			double dPratio[][11]/*[2][11]*/, double dMratio[][11]/*[2][11]*/,
			double dP[][11]/*[2][11]*/, double dM[][11]/*[2][11]*/) {
			for (int ia=0; ia<2; ++ia) // axes loop
			{
				for (int ip=0; ip<11; ++ip) // points loop
				{
					// for points 1-6, use compression; otherwise, use tension (negate it because it is stored as an absolute value)
					const double dPref = ip<6 ? dPaxis[1] : -dPaxis[0];
					const double dPmax = dPref - dPb[ia];
					const double dMmax = dMb[ia];
					dP[ia][ip] = dPratio[ia][ip]*dPmax + dPb[ia];
					dM[ia][ip] = dMratio[ia][ip]*dMmax;
				}
			}
			};
		ScaleDataAccordingToShowValue(Data.dPC, Data.dPCB ,Data.dMC, Data.dP1st, Data.dM1st, dP1st, dM1st);
		ScaleDataAccordingToShowValue(Data.dPY, Data.dPYB ,Data.dMY, Data.dP2nd, Data.dM2nd, dP2nd, dM2nd);
		ScaleDataAccordingToShowValue(Data.dPU, Data.dPUB ,Data.dMU, Data.dP3rd, Data.dM3rd, dP3rd, dM3rd);

		m_wndGrid->SetDataToGrid(dP1st, dM1st, dP2nd, dM2nd, dP3rd, dM3rd, bAuto, m_bAssignProperties);
	}
	else
	{
		for (int ii=0; ii<2; ++ii)
			for (int jj = 0; jj < 11; ++jj)
			{
				this->dP1st[ii][jj] = Data.dP1st[ii][jj];
				this->dM1st[ii][jj] = Data.dM1st[ii][jj];
				this->dP2nd[ii][jj] = Data.dP2nd[ii][jj];
				this->dM2nd[ii][jj] = Data.dM2nd[ii][jj];
				this->dP3rd[ii][jj] = Data.dP3rd[ii][jj];
				this->dM3rd[ii][jj] = Data.dM3rd[ii][jj];
			}

		m_wndGrid->SetDataToGrid(Data.dP1st, Data.dM1st, Data.dP2nd, Data.dM2nd, Data.dP3rd, Data.dM3rd, bAuto, m_bAssignProperties);
	}
	UpdateData(FALSE);
}

void CIehpYieldPropDlgNew::YLDData2Dlg(T_IEHP_YLDSUR& Data)
{
	m_nYLDInputType = Data.nSurfaceUserAuto;
	for(int i=0; i<2; i++)
	{
		m_strBetay1st[i].Format(_T("%g"), Data.dBetay1st[i]);
		m_strBetay2nd[i].Format(_T("%g"), Data.dBetay2nd[i]);
		m_strBetay3rd[i].Format(_T("%g"), Data.dBetay3rd[i]);

		if(m_bUseMz)
		{
			m_strBetaz1st[i].Format(_T("%g"), Data.dBetaz1st[i]);
			m_strBetaz2nd[i].Format(_T("%g"), Data.dBetaz2nd[i]);
			m_strBetaz3rd[i].Format(_T("%g"), Data.dBetaz3rd[i]);
		}
		else
		{
			m_strBetaz1st[i].Format(_T("%g"), 0.0);
			m_strBetaz2nd[i].Format(_T("%g"), 0.0);
			m_strBetaz3rd[i].Format(_T("%g"), 0.0);
		}
	}

	m_str1Gamma.Format(_T("%g"), Data.dGamma1st);
	m_str2Gamma.Format(_T("%g"), Data.dGamma2nd);
	m_str3Gamma.Format(_T("%g"), Data.dGamma3rd);

	m_strAlpha.Format(_T("%g"), Data.dAlpha);

	//GetDlgItem(IDC_YIELD_INPUT_TYPE_AUTO_RDO)->EnableWindow(m_nSectType==1);
	if(m_nSectType==2) m_nYLDInputType=0;
	
	UpdateData(FALSE);
}

void CIehpYieldPropDlgNew::Dlg2Data(BOOL bUpdateRatios)
{
	UpdateData();
	if     (m_Tab.GetCurSel()==D_TAB_DEFALUT)  TabDlg2Data(D_TAB_DEFALUT, m_MainData, bUpdateRatios);
	else if(m_Tab.GetCurSel()==D_TAB_SUB)      TabDlg2Data(D_TAB_SUB    , m_SubData,  bUpdateRatios);

	if(m_nPMInputType == 0 && !m_bExistIJData)
	{
		// User Type && Symmetric 이면 같은 같이 되도록 채운다. 
		// [MQC 8564] 관련해서 CAttrCtrl::CalcIehpYield에서 J단 계산안하게 하는 거 풀고 여기서 같은 값으로 Setting 함 JWKWON-2015-01-08
		m_SubData = m_MainData;
	}
}

BOOL CIehpYieldPropDlgNew::CheckErrorNameDataVar(LPCTSTR lpszData, LPCTSTR lpszVar, BOOL bMsg)
{
	if(bMsg) GSaveHistoryFormatNF(_LS(IDS_DB_IREG_DT_VAR_ERROR), lpszData, lpszVar);
	return FALSE;
}
#define CheckError(x, bMsg) CheckErrorNameDataVar(_LS(IDS_CMD_IEHP_YIELD), x, bMsg)
BOOL CIehpYieldPropDlgNew::CheckDataCtrl(int nTabID, BOOL bMsg/*=TRUE*/)
{
	T_IEHP_YLDSUR Data;
	if(nTabID==D_TAB_DEFALUT)
		Data = m_MainData;
	else if(nTabID==D_TAB_SUB)      
		Data = m_SubData;

	if(m_bConc)
	{
		#if defined(_JP)
			if(Data.dPC0 <= 0) return CheckError(_T("NC0 > 0"), bMsg);
			if(Data.dMC0[0] <= 0) return CheckError(_T("MC0y > 0"), bMsg);
			if(m_bUseMz && Data.dMC0[1] <= 0) return CheckError(_T("MC0z > 0"), bMsg);
		#else
			if(Data.dPC0 <= 0) return CheckError(_T("PC0 > 0"), bMsg);
			if(Data.dMC0[0] <= 0) return CheckError(_T("MC0y > 0"), bMsg);
			if(m_bUseMz && Data.dMC0[1] <= 0) return CheckError(_T("MC0z > 0"), bMsg);
		#endif
	}
	#if defined(_JP)
		if(Data.dPC[0] <= 0) return CheckError(_T("NC(t) > 0"), bMsg);
		if(Data.dPC[1] <= 0) return CheckError(_T("NC(c) > 0"), bMsg);
		if(Data.dPCB[0] < 0 || Data.dPCB[0] >= Data.dPC[1]) return CheckError(_T("0 <= NCBy < NC(c)"), bMsg);
		if(m_bUseMz && (Data.dPCB[1] < 0 || Data.dPCB[1] >= Data.dPC[1])) return CheckError(_T("0 <= NCBz < NC(c)"), bMsg);
		if(Data.dMC[0] <= 0) return CheckError(_T("MCy,max > 0"), bMsg);
		if(m_bUseMz && Data.dMC[1] <= 0) return CheckError(_T("MCz,max > 0"), bMsg);
		if(Data.dPY[0] <= 0) return CheckError(_T("NY(t) > 0"), bMsg);
		if(Data.dPY[1] <= 0) return CheckError(_T("NY(c) > 0"), bMsg);
		if(Data.dPYB[0] < 0 || Data.dPYB[0] >= Data.dPY[1]) return CheckError(_T("0 <= NYBy < NY(c)"), bMsg);
		if(m_bUseMz && (Data.dPYB[1] < 0 || Data.dPYB[1] >= Data.dPY[1])) return CheckError(_T("0 <= NYBz < NY(c)"), bMsg);
		if(Data.dMY[0] <= 0) return CheckError(_T("MYy,max > 0"), bMsg);
		if(m_bUseMz && Data.dMY[1] <= 0) return CheckError(_T("MYz,max > 0"), bMsg);
	#else
		if(Data.dPC[0] <= 0) return CheckError(_T("PC(t) > 0"), bMsg);
		if(Data.dPC[1] <= 0) return CheckError(_T("PC(c) > 0"), bMsg);
		if(dgn::ME((-1.0)*Data.dPC[0], Data.dPCB[0]) || dgn::ME(Data.dPCB[0], Data.dPC[1])) return CheckError(_T("PC(t) < PCBy < PC(c)"), bMsg);
		if(m_bUseMz && (dgn::ME((-1.0) * Data.dPC[0], Data.dPCB[1]) || dgn::ME(Data.dPCB[1], Data.dPC[1]))) return CheckError(_T("PC(t) < PCBz < PC(c)"), bMsg);
		if(Data.dMC[0] <= 0) return CheckError(_T("MCy,max > 0"), bMsg);
		if(m_bUseMz && Data.dMC[1] <= 0) return CheckError(_T("MCz,max > 0"), bMsg);
		if(Data.dPY[0] <= 0) return CheckError(_T("PY(t) > 0"), bMsg);
		if(Data.dPY[1] <= 0) return CheckError(_T("PY(c) > 0"), bMsg);
		if(dgn::ME((-1.0)*Data.dPY[0], Data.dPYB[0]) || dgn::ME(Data.dPYB[0], Data.dPY[1])) return CheckError(_T("PY(t) < PYBy < PY(c)"), bMsg);
		if(m_bUseMz && (dgn::ME((-1.0) * Data.dPY[0], Data.dPYB[1]) || dgn::ME(Data.dPYB[1], Data.dPY[1]))) return CheckError(_T("PY(t) < PYBz < PY(c)"), bMsg);
		if(Data.dMY[0] <= 0) return CheckError(_T("MYy,max > 0"), bMsg);
		if(m_bUseMz && Data.dMY[1] <= 0) return CheckError(_T("MYz,max > 0"), bMsg);
	#endif
		
	for(int i=0; i<2; i++)
	{
		for(int j=1; j<5; j++)  
		{
			if(!m_bConc && (Data.dM1st[i][j] <= 0. || Data.dM1st[i][j] >= 1.))
				return CheckError(_T("0 < M / MC,max < 1"), bMsg);
			if(Data.dM2nd[i][j] <= 0. || Data.dM2nd[i][j] >= 1.) return CheckError(_T("0 < M / MY,max < 1"), bMsg);

		}
		for(int j = 6; j < 10; j++)
		{
			if(!m_bConc && (Data.dM1st[i][j] <= 0. || Data.dM1st[i][j] >= 1.))
				return CheckError(_T("0 < M / MC,max < 1"), bMsg);
			if(Data.dM2nd[i][j] <= 0. || Data.dM2nd[i][j] >= 1.) return CheckError(_T("0 < M / MY,max < 1"), bMsg);

		}
	}

	BOOL bJapan = FALSE;
	int nMovingType = CProduct::GetMovingType();
	if(nMovingType == D_PRODUCT_MOVING_JP) bJapan = TRUE;

	for(int i=0; i<2; i++)
	{
		for(int j=0; j<5; j++)
		{
			#if defined(_JP)
				if(!m_bConc && (Data.dP1st[i][j] <= Data.dP1st[i][j + 1]))
					return CheckError(_T("(N-PCB) / (NC-PCB)"), bMsg);
				if(Data.dP2nd[i][j] <= Data.dP2nd[i][j + 1]) return CheckError(_T("(N-NYB) / (NY-NYB)"), bMsg);
			#else
			if( ((i==1 && m_bUseMz) || i==0) && !bJapan) // MQC 9532 : Mz를 사용하지 않을 때는 검사하지 않는다.
			{ 
				if(!m_bConc && (Data.dP1st[i][j] <= Data.dP1st[i][j + 1]))
					return CheckError(_T("(P-PCB) / (PC-PCB)"), bMsg);
				if(Data.dP2nd[i][j] <= Data.dP2nd[i][j + 1]) return CheckError(_T("(P-PYB) / (PY-PYB)"), bMsg);
			}
			#endif
		}
		for(int j=6; j<10; j++)
		{
			#if defined(_JP)
				if(!m_bConc && (Data.dP1st[i][j] >= Data.dP1st[i][j + 1]))
			    	return CheckError(_T("(N-NCB) / (NC-NCB)"), bMsg);
				if(Data.dP2nd[i][j] >= Data.dP2nd[i][j + 1]) return CheckError(_T("(N-NYB) / (NY-NYB)"), bMsg);
			#else
			if( ((i==1 && m_bUseMz) || i==0) && !bJapan) // MQC 9532 : Mz를 사용하지 않을 때는 검사하지 않는다.
			{ 
				if(!m_bConc && (Data.dP1st[i][j] >= Data.dP1st[i][j + 1]))
					return CheckError(_T("(P-PCB) / (PC-PCB)"), bMsg);
				if(Data.dP2nd[i][j] >= Data.dP2nd[i][j + 1]) return CheckError(_T("(P-PYB) / (PY-PYB)"), bMsg);
			}
			#endif
		}
	}

	for(int i=0; i<2; i++)
	{
		if(Data.dBetay1st[i] < 1.) return CheckError(_T("Beta y 1st"), bMsg);
		if(Data.dBetay2nd[i] < 1.) return CheckError(_T("Beta y 2nd"), bMsg);
		if(m_bUseMz)
		{
			if(Data.dBetaz1st[i] < 1.) return CheckError(_T("Beta z 1st"), bMsg);
			if(Data.dBetaz2nd[i] < 1.) return CheckError(_T("Beta z 2nd"), bMsg);
		}
	}
	if(Data.dGamma1st < 1. || Data.dGamma1st > 3.) return CheckError(_T("Gamma1st"), bMsg);
	if(Data.dGamma2nd < 1. || Data.dGamma2nd > 3.) return CheckError(_T("Gamma2nd"), bMsg);

	if(Data.dAlpha < 1. || Data.dAlpha > 3.) return CheckError(_LS(IDS_DB_ALPHA), bMsg); // _T("Alpha")
	
	if(m_Tab.GetCurSel()==D_TAB_DEFALUT)
		m_MainData = Data;
	else if(m_Tab.GetCurSel()==D_TAB_SUB)
		m_SubData = Data;

	return TRUE;
#undef CheckError
}

void CIehpYieldPropDlgNew::OnSelchangePlotComb() 
{
	Dlg2Data(TRUE);
	if(!CheckDataCtrl(m_Tab.GetCurSel())) return;

	int nSel = m_cmbPlot.GetCurSel();
	//UpdateChart(nSel);
	UpdateGraph(nSel);
}

void CIehpYieldPropDlgNew::OnChangeEditBox() 
{
	Dlg2Data(FALSE);
	if(!CheckDataCtrl(m_Tab.GetCurSel(),FALSE)) return;

	int nSel = m_cmbPlot.GetCurSel();
	//UpdateChart(nSel);
	UpdateGraph(nSel);
}

void CIehpYieldPropDlgNew::OnKillFocusEditBox()
{
	//if(!CheckDataCtrl(m_Tab.GetCurSel(),FALSE)) return;
	Data2Dlg();
	UpdateData(TRUE);

	int nSel = m_cmbPlot.GetCurSel();
	UpdateGraph(nSel);
}

void CIehpYieldPropDlgNew::OnPmcvGrid() 
{
	UpdateData();
	m_wndGrid->SetFormatMode(m_nPmcv);
}

/////////////////////////////////////////////////////////////////////
// Graph operations
void CIehpYieldPropDlgNew::InitGraph()
{
	CWnd* pGraph=GetDlgItem(IDC_PICTURE);
	ASSERT(pGraph);
	
	CRect rect;
	pGraph->GetClientRect(&rect);
	m_Draw.Create(pGraph, rect, NULL);
	m_Draw.FitToParent(rect);
}

void CIehpYieldPropDlgNew::UpdateGraph(int nSel)
{
	//Dlg2Data();
	//if(!CheckDataCtrl(FALSE)) return;

	BOOL bJapan = FALSE;
	int nMovingType = CProduct::GetMovingType();
	if(nMovingType == D_PRODUCT_MOVING_JP) bJapan = TRUE;
#if defined(_JP)
	bJapan = TRUE;
#endif

	double dTMP1;
	double dTMP2;

	int nPlot = 50;
	double dPI = 3.14159265358979;

	double dSin90;
	double dSin00;

	double dV_Pcrst[2][11];  // Crack Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double dV_Mcrst[2][11];  // Crack Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)

	double dV_P1st[2][11];  // 1st Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double dV_M1st[2][11];  // 1st Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double dV_P2nd[2][11];  // 2nd Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double dV_M2nd[2][11];  // 2nd Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double dV_P3rd[2][11];  // 3rd Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double dV_M3rd[2][11];  // 3rd Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)

	double dV_P1st_aprx[2][101];  // 1st Approximated Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double dV_M1st_aprx[2][101];  // 1st Approximated Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double dV_P2nd_aprx[2][101];  // 2nd Approximated Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double dV_M2nd_aprx[2][101];  // 2nd Approximated Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)
	double dV_P3rd_aprx[2][101];  // 3rd Approximated Interaction Curve Coordinate on P-axis (0: y-axis,  1: z-axis)
	double dV_M3rd_aprx[2][101];  // 3rd Approximated Interaction Curve Coordinate on M-axis (0: y-axis,  1: z-axis)

	double dV_MM1st_aprx[2][51];  // 1st Approximated My - Mz Interaction Curve Coordinate (0: y-axis,  1: z-axis)
	double dV_MM2nd_aprx[2][51];  // 2nd Approximated My - Mz Interaction Curve Coordinate (0: y-axis,  1: z-axis)
	double dV_MM3rd_aprx[2][51];  // 3rd Approximated My - Mz Interaction Curve Coordinate (0: y-axis,  1: z-axis)

	//===========================================================================
	//
	//  O r i g i n a l    S u r f a c e 
	//
	//===========================================================================

	T_IEHP_YLDSUR Data;
	if(m_Tab.GetCurSel()==D_TAB_DEFALUT)
		Data = m_MainData;
	else if(m_Tab.GetCurSel()==D_TAB_SUB)      
		Data = m_SubData;

	if(m_bConc) // Concrete
	{
		if(Data.dMC0[0] == 0.) return;

		if(!bJapan)
		{
			// Moment Component
			for(int i = 0; i < 11; i++)
			{
				// 1st Yield Surface
				dV_M1st[0][i] = Data.dMY[0] * i / 10;  // y-axis
				dV_M1st[1][i] = Data.dMY[1] * i / 10;  // z-axis

				// 2nd Yield Surface
				dV_M2nd[0][i] = Data.dMY[0] * Data.dM2nd[0][i];  // y-axis
				dV_M2nd[1][i] = Data.dMY[1] * Data.dM2nd[1][i];  // z-axis

				// 3rd Yield Surface
				dV_M3rd[0][i] = Data.dMU[0] * Data.dM3rd[0][i];  // y-axis
				dV_M3rd[1][i] = Data.dMU[1] * Data.dM3rd[1][i];  // z-axis
			}

			// Axial Force Component  ------  1st Yield Surface
			for(int i = 0; i < 11; i++)
			{
				dV_P1st[0][i] = Data.dPC0 / Data.dMC0[0] * dV_M1st[0][i] - Data.dPC0;  // y-axis compression
				dV_P1st[1][i] = Data.dPC0 / Data.dMC0[1] * dV_M1st[1][i] - Data.dPC0;  // z-axis compression
			}
			// Axial Force Component  ------  2nd Yield Surface
			for(int i = 0; i < 5; i++)
			{
				dV_P2nd[0][i  ] = ( Data.dPY[1] - Data.dPYB[0]) * Data.dP2nd[0][i  ] + Data.dPYB[0];  // y-axis compression
				dV_P2nd[0][i+6] = (-Data.dPY[0] - Data.dPYB[0]) * Data.dP2nd[0][i+6] + Data.dPYB[0];  // y-axis tension
				dV_P2nd[1][i  ] = ( Data.dPY[1] - Data.dPYB[1]) * Data.dP2nd[1][i  ] + Data.dPYB[1];  // z-axis compression
				dV_P2nd[1][i+6] = (-Data.dPY[0] - Data.dPYB[1]) * Data.dP2nd[1][i+6] + Data.dPYB[1];  // z-axis tension
			}
			dV_P2nd[0][5] = (Data.dPY[1] - Data.dPYB[0]) * Data.dP2nd[0][5] + Data.dPYB[0];  // y-axis center
			dV_P2nd[1][5] = (Data.dPY[1] - Data.dPYB[1]) * Data.dP2nd[1][5] + Data.dPYB[1];  // z-axis center

			// Axial Force Component  ------  3rd Yield Surface
			for (int i = 0; i < 5; i++)
			{
				dV_P3rd[0][i  ] = ( Data.dPU[1] - Data.dPUB[0]) * Data.dP3rd[0][i  ] + Data.dPUB[0];  // y-axis compression
				dV_P3rd[0][i+6] = (-Data.dPU[0] - Data.dPUB[0]) * Data.dP3rd[0][i+6] + Data.dPUB[0];  // y-axis tension
				dV_P3rd[1][i  ] = ( Data.dPU[1] - Data.dPUB[1]) * Data.dP3rd[1][i  ] + Data.dPUB[1];  // z-axis compression
				dV_P3rd[1][i+6] = (-Data.dPU[0] - Data.dPUB[1]) * Data.dP3rd[1][i+6] + Data.dPUB[1];  // z-axis tension
			}
			dV_P3rd[0][5] = (Data.dPU[1] - Data.dPUB[0]) * Data.dP3rd[0][5] + Data.dPUB[0];  // y-axis center
			dV_P3rd[1][5] = (Data.dPU[1] - Data.dPUB[1]) * Data.dP3rd[1][5] + Data.dPUB[1];  // z-axis center

			for(int i = 0; i < 2; i++) if(dV_P1st[i][10] > dV_P2nd[i][0])
			{
				double dR = (dV_P2nd[i][0] + Data.dPC0) / (dV_P1st[i][10] + Data.dPC0);
				for(int j = 0; j < 11; j++)
				{
					dV_M1st[i][j] *= dR;
					dV_P1st[i][j] = -Data.dPC0 + (dV_P1st[i][j] + Data.dPC0) * dR;
				}
			}
		}
		else
		{
			// Moment Component
			for(int i = 0; i < 11; i++)
			{
				// Crack
				dV_Mcrst[0][i] = Data.dMY[0] * i / 10;  // y-axis
				dV_Mcrst[1][i] = Data.dMY[1] * i / 10;  // z-axis

				// 1st Yield Surface
				dV_M1st[0][i] = Data.dMC[0] * Data.dM1st[0][i];  // y-axis
				dV_M1st[1][i] = Data.dMC[1] * Data.dM1st[1][i];  // z-axis

				// 2nd Yield Surface
				dV_M2nd[0][i] = Data.dMY[0] * Data.dM2nd[0][i];  // y-axis
				dV_M2nd[1][i] = Data.dMY[1] * Data.dM2nd[1][i];  // z-axis

				// 2nd Yield Surface
				dV_M3rd[0][i] = Data.dMU[0] * Data.dM3rd[0][i];  // y-axis
				dV_M3rd[1][i] = Data.dMU[1] * Data.dM3rd[1][i];  // z-axis
			}

			// Axial Force Component  ------  Crack
			for(int i = 0; i < 11; i++)
			{
				dV_Pcrst[0][i] = Data.dPC0 / Data.dMC0[0] * dV_Mcrst[0][i] - Data.dPC0;  // y-axis compression
				dV_Pcrst[1][i] = Data.dPC0 / Data.dMC0[1] * dV_Mcrst[1][i] - Data.dPC0;  // z-axis compression
			}

			// Axial Force Component  ------  1st Yield Surface
			for(int i = 0; i < 5; i++)
			{
				dV_P1st[0][i] = (Data.dPC[1] - Data.dPCB[0]) * Data.dP1st[0][i] + Data.dPCB[0];  // y-axis compression
				dV_P1st[0][i + 6] = (-Data.dPC[0] - Data.dPCB[0]) * Data.dP1st[0][i + 6] + Data.dPCB[0];  // y-axis tension
				dV_P1st[1][i] = (Data.dPC[1] - Data.dPCB[1]) * Data.dP1st[1][i] + Data.dPCB[1];  // z-axis compression
				dV_P1st[1][i + 6] = (-Data.dPC[0] - Data.dPCB[1]) * Data.dP1st[1][i + 6] + Data.dPCB[1];  // z-axis tension
			}
			dV_P1st[0][5] = (Data.dPC[1] - Data.dPCB[0]) * Data.dP1st[0][5] + Data.dPCB[0];  // y-axis center
			dV_P1st[1][5] = (Data.dPC[1] - Data.dPCB[1]) * Data.dP1st[1][5] + Data.dPCB[1];  // z-axis center

			// Axial Force Component  ------  2nd Yield Surface
			for(int i = 0; i < 5; i++)
			{
				dV_P2nd[0][i] = (Data.dPY[1] - Data.dPYB[0]) * Data.dP2nd[0][i] + Data.dPYB[0];  // y-axis compression
				dV_P2nd[0][i + 6] = (-Data.dPY[0] - Data.dPYB[0]) * Data.dP2nd[0][i + 6] + Data.dPYB[0];  // y-axis tension
				dV_P2nd[1][i] = (Data.dPY[1] - Data.dPYB[1]) * Data.dP2nd[1][i] + Data.dPYB[1];  // z-axis compression
				dV_P2nd[1][i + 6] = (-Data.dPY[0] - Data.dPYB[1]) * Data.dP2nd[1][i + 6] + Data.dPYB[1];  // z-axis tension
			}
			dV_P2nd[0][5] = (Data.dPY[1] - Data.dPYB[0]) * Data.dP2nd[0][5] + Data.dPYB[0];  // y-axis center
			dV_P2nd[1][5] = (Data.dPY[1] - Data.dPYB[1]) * Data.dP2nd[1][5] + Data.dPYB[1];  // z-axis center

			// Axial Force Component  ------  3rd Yield Surface
			for (int i = 0; i < 5; i++)
			{
				dV_P3rd[0][i  ] = ( Data.dPU[1] - Data.dPUB[0]) * Data.dP3rd[0][i  ] + Data.dPUB[0];  // y-axis compression
				dV_P3rd[0][i+6] = (-Data.dPU[0] - Data.dPUB[0]) * Data.dP3rd[0][i+6] + Data.dPUB[0];  // y-axis tension
				dV_P3rd[1][i  ] = ( Data.dPU[1] - Data.dPUB[1]) * Data.dP3rd[1][i  ] + Data.dPUB[1];  // z-axis compression
				dV_P3rd[1][i+6] = (-Data.dPU[0] - Data.dPUB[1]) * Data.dP3rd[1][i+6] + Data.dPUB[1];  // z-axis tension
			}
			dV_P3rd[0][5] = (Data.dPU[1] - Data.dPUB[0]) * Data.dP3rd[0][5] + Data.dPUB[0];  // y-axis center
			dV_P3rd[1][5] = (Data.dPU[1] - Data.dPUB[1]) * Data.dP3rd[1][5] + Data.dPUB[1];  // z-axis center

			for(int i=0; i<2; i++) if(dV_Pcrst[i][10] > dV_P2nd[i][0])
			{
				double dR = (dV_P2nd[i][0] + Data.dPC0) / (dV_Pcrst[i][10] + Data.dPC0);
				for(int j=0; j<11; j++)
				{
					dV_Mcrst[i][j] *= dR;
					dV_Pcrst[i][j] = -Data.dPC0 + (dV_Pcrst[i][j] + Data.dPC0) * dR;
				}
			}
		}
	}
	else
	{
		 //  Moment Component
		for (int i = 0; i < 11 ; i++ )
		{
			// 1st Yield Surface
			dV_M1st[0][i] = Data.dMC[0] * Data.dM1st[0][i];  // y-axis
			dV_M1st[1][i] = Data.dMC[1] * Data.dM1st[1][i];  // z-axis

			// 2nd Yield Surface
			dV_M2nd[0][i] = Data.dMY[0] * Data.dM2nd[0][i];  // y-axis
			dV_M2nd[1][i] = Data.dMY[1] * Data.dM2nd[1][i];  // z-axis

			// 3rd Yield Surface
			dV_M3rd[0][i] = Data.dMU[0] * Data.dM3rd[0][i];  // y-axis
			dV_M3rd[1][i] = Data.dMU[1] * Data.dM3rd[1][i];  // z-axis
		}
		
		//  Axial Force Component
		for (int i = 0; i < 5 ; i++ )
		{
			//  1st Yield Surface
			dV_P1st[0][i  ] = ( Data.dPC[1] - Data.dPCB[0] ) * Data.dP1st[0][i  ] + Data.dPCB[0];  // y-axis compression
			dV_P1st[0][i+6] = (-Data.dPC[0] - Data.dPCB[0] ) * Data.dP1st[0][i+6] + Data.dPCB[0];  // y-axis tension
			dV_P1st[1][i  ] = ( Data.dPC[1] - Data.dPCB[1] ) * Data.dP1st[1][i  ] + Data.dPCB[1];  // z-axis compression
			dV_P1st[1][i+6] = (-Data.dPC[0] - Data.dPCB[1] ) * Data.dP1st[1][i+6] + Data.dPCB[1];  // z-axis tension
	 
			//  2nd Yield Surface
			dV_P2nd[0][i  ] = ( Data.dPY[1] - Data.dPYB[0] ) * Data.dP2nd[0][i  ] + Data.dPYB[0];  // y-axis compression
			dV_P2nd[0][i+6] = (-Data.dPY[0] - Data.dPYB[0] ) * Data.dP2nd[0][i+6] + Data.dPYB[0];  // y-axis tension
			dV_P2nd[1][i  ] = ( Data.dPY[1] - Data.dPYB[1] ) * Data.dP2nd[1][i  ] + Data.dPYB[1];  // z-axis compression
			dV_P2nd[1][i+6] = (-Data.dPY[0] - Data.dPYB[1] ) * Data.dP2nd[1][i+6] + Data.dPYB[1];  // z-axis tension

			//  3rd Yield Surface
			dV_P3rd[0][i  ] = ( Data.dPU[1] - Data.dPUB[0]) * Data.dP3rd[0][i  ] + Data.dPUB[0];  // y-axis compression
			dV_P3rd[0][i+6] = (-Data.dPU[0] - Data.dPUB[0]) * Data.dP3rd[0][i+6] + Data.dPUB[0];  // y-axis tension
			dV_P3rd[1][i  ] = ( Data.dPU[1] - Data.dPUB[1]) * Data.dP3rd[1][i  ] + Data.dPUB[1];  // z-axis compression
			dV_P3rd[1][i+6] = (-Data.dPU[0] - Data.dPUB[1]) * Data.dP3rd[1][i+6] + Data.dPUB[1];  // z-axis tension
		}

		dV_P1st[0][5] = ( Data.dPC[1] - Data.dPCB[0] ) * Data.dP1st[0][5] + Data.dPCB[0]; // y-axis center
		dV_P1st[1][5] = ( Data.dPC[1] - Data.dPCB[1] ) * Data.dP1st[1][5] + Data.dPCB[1]; // z-axis center
		dV_P2nd[0][5] = ( Data.dPY[1] - Data.dPYB[0] ) * Data.dP2nd[0][5] + Data.dPYB[0]; // y-axis center
		dV_P2nd[1][5] = ( Data.dPY[1] - Data.dPYB[1] ) * Data.dP2nd[1][5] + Data.dPYB[1]; // z-axis center
		dV_P3rd[0][5] = ( Data.dPU[1] - Data.dPUB[0] ) * Data.dP3rd[0][5] + Data.dPUB[0]; // y-axis center
		dV_P3rd[1][5] = ( Data.dPU[1] - Data.dPUB[1] ) * Data.dP3rd[1][5] + Data.dPUB[1]; // z-axis center
	}

	//===========================================================================
	//
	//  A p p r o x i m a t e d    S u r f a c e 
	//
	//===========================================================================

	//  Axial Force Component 
	for (int i = 0 ; i < nPlot ; i++ )
	{
		dSin90 = sin( 0.5*dPI* (nPlot-i)/nPlot );
		dSin00 = sin( 0.5*dPI* (    1+i)/nPlot );

		//  1st Yield Surface
		dV_P1st_aprx[0][i        ] = ( Data.dPC[1] - Data.dPCB[0] ) * dSin90 + Data.dPCB[0];  // y-axis compression
		dV_P1st_aprx[0][i+nPlot+1] = (-Data.dPC[0] - Data.dPCB[0] ) * dSin00 + Data.dPCB[0];  // y-axis tension
		dV_P1st_aprx[1][i        ] = ( Data.dPC[1] - Data.dPCB[1] ) * dSin90 + Data.dPCB[1];  // z-axis compression
		dV_P1st_aprx[1][i+nPlot+1] = (-Data.dPC[0] - Data.dPCB[1] ) * dSin00 + Data.dPCB[1];  // z-axis tension

		//  2nd Yield Surface
		dV_P2nd_aprx[0][i        ] = ( Data.dPY[1] - Data.dPYB[0] ) * dSin90 + Data.dPYB[0];  // y-axis compression
		dV_P2nd_aprx[0][i+nPlot+1] = (-Data.dPY[0] - Data.dPYB[0] ) * dSin00 + Data.dPYB[0];  // y-axis tension
		dV_P2nd_aprx[1][i        ] = ( Data.dPY[1] - Data.dPYB[1] ) * dSin90 + Data.dPYB[1];  // z-axis compression
		dV_P2nd_aprx[1][i+nPlot+1] = (-Data.dPY[0] - Data.dPYB[1] ) * dSin00 + Data.dPYB[1];  // z-axis tension


		//  3rd Yield Surface
		dV_P3rd_aprx[0][i        ] = ( Data.dPU[1] - Data.dPUB[0] ) * dSin90 + Data.dPUB[0];  // y-axis compression
		dV_P3rd_aprx[0][i+nPlot+1] = (-Data.dPU[0] - Data.dPUB[0] ) * dSin00 + Data.dPUB[0];  // y-axis tension
		dV_P3rd_aprx[1][i        ] = ( Data.dPU[1] - Data.dPUB[1] ) * dSin90 + Data.dPUB[1];  // z-axis compression
		dV_P3rd_aprx[1][i+nPlot+1] = (-Data.dPU[0] - Data.dPUB[1] ) * dSin00 + Data.dPUB[1];  // z-axis tension
	}

	dV_P1st_aprx[0][nPlot] = Data.dPCB[0];  // 1st surface y-axis balance
	dV_P1st_aprx[1][nPlot] = Data.dPCB[1];  // 1st surface z-axis balance
	dV_P2nd_aprx[0][nPlot] = Data.dPYB[0];  // 2nd surface y-axis balance
	dV_P2nd_aprx[1][nPlot] = Data.dPYB[1];  // 2nd surface z-axis balance
	dV_P3rd_aprx[0][nPlot] = Data.dPUB[0];  // 3rd surface y-axis balance
	dV_P3rd_aprx[1][nPlot] = Data.dPUB[1];  // 3rd surface z-axis balance

	//  Bending Moment Component
	for(int i = 0 ; i < nPlot ; i++ )
	{
		 //  1st Yield Surface
		dTMP1=(dV_P1st_aprx[0][i]-Data.dPCB[0])/(Data.dPC[1]-Data.dPCB[0]);  // y, C
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetay1st[1]);
		dV_M1st_aprx[0][i]=Data.dMC[0]*pow(fabs(dTMP2),(1.0/Data.dGamma1st));

		dTMP1=(dV_P1st_aprx[0][i+nPlot+1]-Data.dPCB[0])/(-Data.dPC[0]-Data.dPCB[0]);  // y, T
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetay1st[0]);
		dV_M1st_aprx[0][i+nPlot+1]=Data.dMC[0]*pow(fabs(dTMP2),(1.0/Data.dGamma1st));

		dTMP1=(dV_P1st_aprx[1][i]-Data.dPCB[1])/(Data.dPC[1]-Data.dPCB[1]);  // z, C
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetaz1st[1]);
		dV_M1st_aprx[1][i]=Data.dMC[1]*pow(fabs(dTMP2),(1.0/Data.dGamma1st));

		dTMP1=(dV_P1st_aprx[1][i+nPlot+1]-Data.dPCB[1])/(-Data.dPC[0]-Data.dPCB[1]);  // z, T
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetaz1st[0]);
		dV_M1st_aprx[1][i+nPlot+1]=Data.dMC[1]*pow(fabs(dTMP2),(1.0/Data.dGamma1st));

		//  2nd Yield Surface
		dTMP1=(dV_P2nd_aprx[0][i]-Data.dPYB[0])/(Data.dPY[1]-Data.dPYB[0]);  // y, C
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetay2nd[1]);
		dV_M2nd_aprx[0][i]=Data.dMY[0]*pow(fabs(dTMP2),(1.0/Data.dGamma2nd));

		dTMP1=(dV_P2nd_aprx[0][i+nPlot+1]-Data.dPYB[0])/(-Data.dPY[0]-Data.dPYB[0]);  // y, T
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetay2nd[0]);
		dV_M2nd_aprx[0][i+nPlot+1]=Data.dMY[0]*pow(fabs(dTMP2),(1.0/Data.dGamma2nd));

		dTMP1=(dV_P2nd_aprx[1][i]-Data.dPYB[1])/(Data.dPY[1]-Data.dPYB[1]);  // z, C
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetaz2nd[1]);
		dV_M2nd_aprx[1][i]=Data.dMY[1]*pow(fabs(dTMP2),(1.0/Data.dGamma2nd));

		dTMP1=(dV_P2nd_aprx[1][i+nPlot+1]-Data.dPYB[1])/(-Data.dPY[0]-Data.dPYB[1]);  // z, T
		dTMP2=1.0-pow(fabs(dTMP1), Data.dBetaz2nd[0]);
		dV_M2nd_aprx[1][i+nPlot+1]=Data.dMY[1]*pow(fabs(dTMP2),(1.0/Data.dGamma2nd));

		//  3rd Yield Surface
		dTMP1 = (dV_P3rd_aprx[0][i] - Data.dPUB[0]) / (Data.dPU[1] - Data.dPUB[0]);  // y, C
		dTMP2 = 1.0 - pow(fabs(dTMP1), Data.dBetay3rd[1]);
		dV_M3rd_aprx[0][i] = Data.dMU[0] * pow(fabs(dTMP2), (1.0 / Data.dGamma3rd));

		dTMP1 = (dV_P3rd_aprx[0][i+nPlot+1] - Data.dPUB[0]) / (-Data.dPU[0] - Data.dPUB[0]);  // y, T
		dTMP2 = 1.0 - pow(fabs(dTMP1), Data.dBetay3rd[0]);
		dV_M3rd_aprx[0][i+nPlot+1] = Data.dMU[0] * pow(fabs(dTMP2), (1.0 / Data.dGamma3rd));

		dTMP1 = (dV_P3rd_aprx[1][i] - Data.dPUB[1]) / (Data.dPU[1] - Data.dPUB[1]);  // z, C
		dTMP2 = 1.0 - pow(fabs(dTMP1), Data.dBetaz3rd[1]);
		dV_M3rd_aprx[1][i] = Data.dMU[1] * pow(fabs(dTMP2), (1.0 / Data.dGamma3rd));

		dTMP1 = (dV_P3rd_aprx[1][i+nPlot+1] - Data.dPUB[1]) / (-Data.dPU[0] - Data.dPUB[1]);  // z, T
		dTMP2 = 1.0 - pow(fabs(dTMP1), Data.dBetaz3rd[0]);
		dV_M3rd_aprx[1][i+nPlot+1] = Data.dMU[1] * pow(fabs(dTMP2), (1.0 / Data.dGamma3rd));
	}

	dV_M1st_aprx[0][nPlot] = Data.dMC[0];  // 1st surface y-axis max. moment
	dV_M1st_aprx[1][nPlot] = Data.dMC[1];  // 1st surface z-axis max. moment
	dV_M2nd_aprx[0][nPlot] = Data.dMY[0];  // 2nd surface y-axis max. moment
	dV_M2nd_aprx[1][nPlot] = Data.dMY[1];  // 2nd surface z-axis max. moment
	dV_M3rd_aprx[0][nPlot] = Data.dMU[0];  // 3rd surface y-axis max. moment
	dV_M3rd_aprx[1][nPlot] = Data.dMU[1];  // 3rd surface z-axis max. moment

	//  My-Mz interaction curve
	for(int i = 0 ; i < nPlot+1 ; i++ )
	{
		dSin00 = sin( 0.5*dPI* i/nPlot );

		// 1st Yield Surface
		dV_MM1st_aprx[0][i] = Data.dMC[0] * dSin00; // 1st surface y-axis
		dTMP1 = 1.0 - pow( dSin00, Data.dAlpha );   
		dV_MM1st_aprx[1][i] = Data.dMC[1] * pow(dTMP1, (1.0/Data.dAlpha)); // 1st surface z-axis
		
		// 2nd Yield Surface
		dV_MM2nd_aprx[0][i] = Data.dMY[0] * dSin00; // 2nd surface y-axis
		dTMP1 = 1.0 - pow( dSin00, Data.dAlpha );
		dV_MM2nd_aprx[1][i] = Data.dMY[1] * pow(dTMP1, (1.0/Data.dAlpha)); // 2nd surface z-axis

		// 3rd Yield Surface
		dV_MM3rd_aprx[0][i] = Data.dMU[0] * dSin00; // 2nd surface y-axis
		dTMP1 = 1.0 - pow( dSin00, Data.dAlpha );
		dV_MM3rd_aprx[1][i] = Data.dMU[1] * pow(dTMP1, (1.0/Data.dAlpha)); // 2nd surface z-axis
	}

	m_Draw.SetDefaultData(nSel+2, dV_P1st, dV_M1st, dV_P2nd, dV_M2nd, dV_P3rd, dV_M3rd,
						  dV_P1st_aprx,  dV_M1st_aprx,  dV_P2nd_aprx, dV_M2nd_aprx, dV_P3rd_aprx, dV_M3rd_aprx,
						  dV_MM1st_aprx, dV_MM2nd_aprx, dV_MM3rd_aprx);
	if(bJapan)
	{
		m_Draw.SetCrackData(dV_Pcrst, dV_Mcrst);
	}
	m_Draw.SetDraw1stCurve(m_bUnable1stCtrl==FALSE);
	m_Draw.SetDraw3rdCurve(m_bUse3rdCtrl);
	m_Draw.DrawSurfaceGraph();
}

BOOL CIehpYieldPropDlgNew::InitTabCtrl()
{
	UpdateData(TRUE);
	m_Tab.DeleteAllItems();
	if(m_bExistIJData == TRUE)		
	{
			m_nValueTypeSymmetry = 1;
			UpdateData(FALSE);
	}
	if(IehpData.nHingeType == 0 && m_nPMInputType == 1 && m_bAssignProperties == TRUE)
	{
		BOOL bEqual = TRUE;
		if(memcmp(&m_MainData, &m_SubData, sizeof(m_MainData)) != 0) bEqual = FALSE;
		
		if(bEqual)		m_nValueTypeSymmetry = 0;
		else					m_nValueTypeSymmetry = 1;
			
		UpdateData(FALSE);
	}
	m_MainData;
	m_SubData;
	ChangeTabCtrl();
	m_Tab.SetCurSel(0);

	return TRUE;
}

BOOL CIehpYieldPropDlgNew::ChangeDlgCtrls()
{
	m_wndGrid->SetValueMode (m_chkShowValue.GetCheck());

	if( m_chkShowValue.GetCheck()) m_chkShowValue.SetWindowText( _LS(IDS_RPS_Show_Ratio));
	else m_chkShowValue.SetWindowText( _LS(IDS_RPS_Show_Value));

	return TRUE;
}

void CIehpYieldPropDlgNew::ChangeTabCtrl()
{
	UpdateData(TRUE);

	CString aTitle[3]  = { _LS(IDS_RPS_PO_DOF_Properties),_LS(IDS_RPS_PO_DOF_PropertiesI), _LS(IDS_RPS_PO_DOF_PropertiesJ)};
	m_Tab.DeleteAllItems();

	if(IehpData.nHingeType == 0)
	{
		if(m_nValueTypeSymmetry == 1)
		{
			m_Tab.InsertItem(0, aTitle[1]);
			m_Tab.InsertItem(1, aTitle[2]);
			return;
		}
	}

	if(m_bExistIJData)
	{
		m_Tab.InsertItem(0, aTitle[1]);
		m_Tab.InsertItem(1, aTitle[2]);
		return;
	}

	m_Tab.InsertItem(0, aTitle[0]);
	Invalidate();

	return;
}
void CIehpYieldPropDlgNew::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 다음탭으로 이동하기 직전 처리하는 부분
	int nTabID = m_Tab.GetCurSel();

	if(nTabID == D_TAB_DEFALUT)	
		if(!TabDlg2Data(nTabID,m_MainData, TRUE)) *pResult = 0;
	else if(nTabID == D_TAB_SUB)
		if(!TabDlg2Data(nTabID,m_SubData, TRUE)) *pResult = 1;
	else {ASSERT(FALSE);}

	if(m_MainData.nCurveUserAuto != m_SubData.nCurveUserAuto)
	{
		m_MainData.nCurveUserAuto = m_nPMInputType;
		m_SubData.nCurveUserAuto = m_nPMInputType;
	}
	if(m_MainData.nSurfaceUserAuto != m_SubData.nSurfaceUserAuto)
	{
		m_MainData.nSurfaceUserAuto = m_nYLDInputType;
		m_SubData.nSurfaceUserAuto = m_nYLDInputType;
	}
}
void CIehpYieldPropDlgNew::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 이동한 탭을 보여주기 전에 처리하는 부분
	if(!m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arUser,				TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arPMInput,		TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_arYLDInput,		TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge,	TRUE);
		InitCtrl_Unable1stCtrl();
	}

	WndMessageCtrl();
	SetInitData();
	Data2Dlg();
	if(m_bConc) { m_nPmcv = 1; UpdateData(FALSE); }
	OnPmcvGrid();

	OnPMInputType();
	OnYLDInputType();

	if(!CheckDataCtrl(m_Tab.GetCurSel())) *pResult = 0;  
	UpdateGraph(m_cmbPlot.GetCurSel());

	if(m_bAssignProperties)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arUser,				FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arPMInput,		FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arYLDInput,		FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge,	FALSE);
	}

	*pResult = 0;  
}

void CIehpYieldPropDlgNew::Data2TabDlg(int nTabID, T_IEHP_YLDSUR& Data)
{
	PMData2Dlg(Data);
	YLDData2Dlg(Data);
}
BOOL CIehpYieldPropDlgNew::TabDlg2Data(int nTabID, T_IEHP_YLDSUR& Data, BOOL bUpdateRatios)
{
	Data.nCurveUserAuto = m_nPMInputType;
	Data.dPC0 = m_edtPC0.GetEditValue();
	for(int i=0; i<2; i++)
	{	
		Data.dMC0[i] = m_edtMC0[i].GetEditValue();
		Data.dPC[i]  = m_edtPC[i].GetEditValue();
		Data.dPCB[i] = m_edtPCB[i].GetEditValue();
		Data.dMC[i]  = m_edtMC[i].GetEditValue();
		Data.dPY[i]  = m_edtPY[i].GetEditValue();
		Data.dPYB[i] = m_edtPYB[i].GetEditValue();
		Data.dMY[i]  = m_edtMY[i].GetEditValue();
		Data.dPU[i]  = m_edtPU[i].GetEditValue();
		Data.dPUB[i] = m_edtPUB[i].GetEditValue();
		Data.dMU[i]  = m_edtMU[i].GetEditValue();
		Data.dBetay1st[i] = _tstof(m_strBetay1st[i]);
		Data.dBetay2nd[i] = _tstof(m_strBetay2nd[i]);
		Data.dBetay3rd[i] = _tstof(m_strBetay3rd[i]);
		Data.dBetaz1st[i] = _tstof(m_strBetaz1st[i]);
		Data.dBetaz2nd[i] = _tstof(m_strBetaz2nd[i]);
		Data.dBetaz3rd[i] = _tstof(m_strBetaz3rd[i]);
	}
	Data.nSurfaceUserAuto = m_nYLDInputType;
	Data.dGamma1st = _tstof(m_str1Gamma);
	Data.dGamma2nd = _tstof(m_str2Gamma);
	Data.dGamma3rd = _tstof(m_str3Gamma);
	Data.dAlpha = _tstof(m_strAlpha);
	Data.nCouplingType = 2;

	m_wndGrid->GetGridToData(Data.dP1st, Data.dM1st, Data.dP2nd, Data.dM2nd, Data.dP3rd, Data.dM3rd);

	if (bUpdateRatios && m_bShowValueSet)
	{
		auto ScaleDataAccordingToShowValue = [](double dPaxis[]/*[2]*/, double dPb[]/*[2]*/, double dMb[]/*[2]*/,
			                                      double dP[][11]/*[2][11]*/, double dM[][11]/*[2][11]*/) {
			for (int ia=0; ia<2; ++ia) // axes loop
			{
				for (int ip=0; ip<11; ++ip) // points loop
				{
					// for points 1-6, use compression; otherwise, use tension (negate it because it is stored as an absolute value)
					const double dPref = ip<6 ? dPaxis[1] : -dPaxis[0];
					const double dPmax = dPref - dPb[ia];
					const double dMmax = dMb[ia];
					dP[ia][ip] = (dP[ia][ip]-dPb[ia])/dPmax;
					dM[ia][ip] = dM[ia][ip]/dMmax;
				}
			}
			};

		ScaleDataAccordingToShowValue(dPC, dPCB, dMC, Data.dP1st, Data.dM1st);
		ScaleDataAccordingToShowValue(dPY, dPYB, dMY, Data.dP2nd, Data.dM2nd);
		ScaleDataAccordingToShowValue(dPU, dPUB, dMU, Data.dP3rd, Data.dM3rd);
	}
	else {
		for (int ii=0; ii<2; ++ii)
			for (int jj = 0; jj < 11; ++jj)
			{
				Data.dP1st[ii][jj] = this->dP1st[ii][jj];
				Data.dM1st[ii][jj] = this->dM1st[ii][jj];
				Data.dP2nd[ii][jj] = this->dP2nd[ii][jj];
				Data.dM2nd[ii][jj] = this->dM2nd[ii][jj];
				Data.dP3rd[ii][jj] = this->dP3rd[ii][jj];
				Data.dM3rd[ii][jj] = this->dM3rd[ii][jj];
			}
	}

	for (int ia=0; ia<2; ++ia)
	{
		dPC[ia]  = Data.dPC[ia];
		dPCB[ia] = Data.dPCB[ia];
		dMC[ia]  = Data.dMC[ia];
		dPY[ia]  = Data.dPY[ia];
		dPYB[ia] = Data.dPYB[ia];
		dMY[ia]  = Data.dMY[ia];
		dPU[ia]  = Data.dPU[ia];
		dPUB[ia] = Data.dPUB[ia];
		dMU[ia]  = Data.dMU[ia];
	}

	return TRUE;
}

void CIehpYieldPropDlgNew::OnValueTypeSymmetry()
{
	UpdateData(TRUE);

	if(m_nValueTypeSymmetry == 0)
		m_bExistIJData = FALSE; //Asym
	else
		m_bExistIJData = TRUE;	//Symm

	ChangeTabCtrl();
}

void CIehpYieldPropDlgNew::SetIehpPropData(T_IEHP_D iehpD, T_IEHP_YLDSUR data, T_IEHP_YLDSUR Subdata, 
																					 int nSectType,T_IEHG_K AssignedIehgKey, BOOL bAssignProperties, BOOL bUseMz/*=TRUE*/, BOOL bUnable1stCtrl/*=FALSE*/)
{
	IehpData    = iehpD;
	m_MainData  = data;
	m_SubData   = Subdata;
	m_strName   = iehpD.strName;
	m_nMessage  = iehpD.nMaterialType;
	if(m_nMessage==0) m_bConc = TRUE;
	else m_bConc = FALSE;
	m_nSectType = nSectType;
	m_ElemK     = AssignedIehgKey.key.objK;
	m_bAssignProperties = bAssignProperties;
	m_bUseMz    = bUseMz;
	m_bUnable1stCtrl = bUnable1stCtrl;
}

void CIehpYieldPropDlgNew::Set3rdControl(BOOL bUse3rdCtrl)
{
	m_bUse3rdCtrl = bUse3rdCtrl;
}

void CIehpYieldPropDlgNew::SetIJDataExist(BOOL bExistIJData, int CurType)
{
	m_bExistIJData = bExistIJData;
	m_nPMInputType = CurType;
}


void CIehpYieldPropDlgNew::BackupDisplayData()
{
	UpdateData(TRUE);
	if(m_aBackupArr.GetSize() != 0)
	{
		ASSERT(FALSE);
		return;
	}
	m_aBackupArr.RemoveAll();

	m_aBackupArr.Add(m_edtPC0.GetEditValue());
	m_aBackupArr.Add(m_edtMC0[0].GetEditValue());
	m_aBackupArr.Add(m_edtMC0[1].GetEditValue());
	m_aBackupArr.Add(m_edtPC[0].GetEditValue());
	m_aBackupArr.Add(m_edtPC[1].GetEditValue());
	m_aBackupArr.Add(m_edtPCB[0].GetEditValue());
	m_aBackupArr.Add(m_edtPCB[1].GetEditValue());
	m_aBackupArr.Add(m_edtMC[0].GetEditValue());
	m_aBackupArr.Add(m_edtMC[1].GetEditValue());
	m_aBackupArr.Add(m_edtPY[0].GetEditValue());
	m_aBackupArr.Add(m_edtPY[1].GetEditValue());
	m_aBackupArr.Add(m_edtPYB[0].GetEditValue());
	m_aBackupArr.Add(m_edtPYB[1].GetEditValue());
	m_aBackupArr.Add(m_edtMY[0].GetEditValue());
	m_aBackupArr.Add(m_edtMY[1].GetEditValue());
	m_aBackupArr.Add(m_edtPU[0].GetEditValue());
	m_aBackupArr.Add(m_edtPU[1].GetEditValue());
	m_aBackupArr.Add(m_edtPUB[0].GetEditValue());
	m_aBackupArr.Add(m_edtPUB[1].GetEditValue());
	m_aBackupArr.Add(m_edtMU[0].GetEditValue());
	m_aBackupArr.Add(m_edtMU[1].GetEditValue());

	
	m_edtPC0.SetEditUnit(0);
	m_edtMC0[0].SetEditUnit(0);
	m_edtMC0[1].SetEditUnit(0);
	m_edtPC[0].SetEditUnit(0);
	m_edtPC[1].SetEditUnit(0);
	m_edtPCB[0].SetEditUnit(0);
	m_edtPCB[1].SetEditUnit(0);
	m_edtMC[0].SetEditUnit(0);
	m_edtMC[1].SetEditUnit(0);
	m_edtPY[0].SetEditUnit(0);
	m_edtPY[1].SetEditUnit(0);
	m_edtPYB[0].SetEditUnit(0);
	m_edtPYB[1].SetEditUnit(0);
	m_edtMY[0].SetEditUnit(0);
	m_edtMY[1].SetEditUnit(0);
	m_edtPU[0].SetEditUnit(0);
	m_edtPU[1].SetEditUnit(0);
	m_edtPUB[0].SetEditUnit(0);
	m_edtPUB[1].SetEditUnit(0);
	m_edtMU[0].SetEditUnit(0);
	m_edtMU[1].SetEditUnit(0);
	
	UpdateData(FALSE);
}
void CIehpYieldPropDlgNew::LoadDisplayBackupData()
{
	if(m_aBackupArr.GetSize() == 0) 
	{
		ASSERT(FALSE);
		return;
	}
	UpdateData(TRUE);

	m_edtPC0.SetEditUnit(m_aBackupArr.GetAt(0));
	m_edtMC0[0].SetEditUnit(m_aBackupArr.GetAt(1));
	m_edtMC0[1].SetEditUnit(m_aBackupArr.GetAt(2));
	m_edtPC[0].SetEditUnit(m_aBackupArr.GetAt(3));
	m_edtPC[1].SetEditUnit(m_aBackupArr.GetAt(4));
	m_edtPCB[0].SetEditUnit(m_aBackupArr.GetAt(5));
	m_edtPCB[1].SetEditUnit(m_aBackupArr.GetAt(6));
	m_edtMC[0].SetEditUnit(m_aBackupArr.GetAt(7));
	m_edtMC[1].SetEditUnit(m_aBackupArr.GetAt(8));
	m_edtPY[0].SetEditUnit(m_aBackupArr.GetAt(9));
	m_edtPY[1].SetEditUnit(m_aBackupArr.GetAt(10));
	m_edtPYB[0].SetEditUnit(m_aBackupArr.GetAt(11));
	m_edtPYB[1].SetEditUnit(m_aBackupArr.GetAt(12));
	m_edtMY[0].SetEditUnit(m_aBackupArr.GetAt(13));
	m_edtMY[1].SetEditUnit(m_aBackupArr.GetAt(14));
	m_edtPU[0].SetEditUnit(m_aBackupArr.GetAt(15));
	m_edtPU[1].SetEditUnit(m_aBackupArr.GetAt(16));
	m_edtPUB[0].SetEditUnit(m_aBackupArr.GetAt(17));
	m_edtPUB[1].SetEditUnit(m_aBackupArr.GetAt(18));
	m_edtMU[0].SetEditUnit(m_aBackupArr.GetAt(19));
	m_edtMU[1].SetEditUnit(m_aBackupArr.GetAt(20));

	m_aBackupArr.RemoveAll();
	UpdateData(FALSE);
}

void CIehpYieldPropDlgNew::InitCtrl()
{
	m_arUser.Add(IDC_STATIC1);
	m_arUser.Add(IDC_PM_CRACK_STC1);
	m_arUser.Add(IDC_PM_CRACK_STC2);
	m_arUser.Add(IDC_PM_CRACK_STC3);
	m_arUser.Add(IDC_1ST_PM_CRACK_PC0T_EDT);
	m_arUser.Add(IDC_1ST_PM_CRACK_MC0Y_EDT);
	m_arUser.Add(IDC_1ST_PM_CRACK_MC0Z_EDT);
	// Steel이면 항상 Hide 시켜줌
	CDlgUtil::CtrlShowHide(this, m_arUser, m_bConc);
	if(!m_bUnable1stCtrl)
		m_arUser.RemoveAll();

	if(!m_bUnable1stCtrl)
	{
		m_arPMInput.Add(IDC_1ST_PM_CRACK_PC0T_EDT);
		m_arPMInput.Add(IDC_1ST_PM_CRACK_MC0Y_EDT);
		m_arPMInput.Add(IDC_1ST_PM_CRACK_MC0Z_EDT);
		m_arPMInput.Add(IDC_1ST_PM_PCT_EDT);
		m_arPMInput.Add(IDC_1ST_PM_PCC_EDT);
		m_arPMInput.Add(IDC_1ST_PM_PCBY_EDT);
		m_arPMInput.Add(IDC_1ST_PM_PCBZ_EDT);
		m_arPMInput.Add(IDC_1ST_PM_MCY_EDT);
		m_arPMInput.Add(IDC_1ST_PM_MCZ_EDT);
	}
	m_arPMInput.Add(IDC_2ND_PM_PYT_EDT);
	m_arPMInput.Add(IDC_2ND_PM_PYC_EDT);
	m_arPMInput.Add(IDC_2ND_PM_PYBY_EDT);
	m_arPMInput.Add(IDC_2ND_PM_PYBZ_EDT);
	m_arPMInput.Add(IDC_2ND_PM_MYY_EDT);
	m_arPMInput.Add(IDC_2ND_PM_MYZ_EDT);

	if(!m_bUnable1stCtrl)
	{
		m_arYLDInput.Add(IDC_1ST_BETAY_C);
		m_arYLDInput.Add(IDC_1ST_BETAY_T);
		m_arYLDInput.Add(IDC_1ST_BETAZ_C);
		m_arYLDInput.Add(IDC_1ST_BETAZ_T);
		m_arYLDInput.Add(IDC_1ST_GAMMA);
	}
	m_arYLDInput.Add(IDC_2ND_BETAY_C);
	m_arYLDInput.Add(IDC_2ND_BETAY_T);
	m_arYLDInput.Add(IDC_2ND_BETAZ_C);
	m_arYLDInput.Add(IDC_2ND_BETAZ_T);
	m_arYLDInput.Add(IDC_2ND_GAMMA);
	m_arYLDInput.Add(IDC_ALPHA_EDT);

	m_aAssignHinge.Add(IDC_INPUT_TYPE_USER_RDO);
	m_aAssignHinge.Add(IDC_INPUT_TYPE_AUTO_RDO);
	m_aAssignHinge.Add(IDC_TYPE_SYM_RDO2);
	m_aAssignHinge.Add(IDC_TYPE_ASYM_RDO2);
	m_aAssignHinge.Add(IDC_YIELD_INPUT_TYPE_USER_RDO);
	m_aAssignHinge.Add(IDC_YIELD_INPUT_TYPE_AUTO_RDO);
	m_aAssignHinge.Add(IDOK);
	m_aAssignHinge.Add(IDC_IEHP_TABLE);

	if (m_bUse3rdCtrl)
	{
		m_arPMInput.Add(IDC_3RD_PM_PUT_EDT);
		m_arPMInput.Add(IDC_3RD_PM_PUC_EDT);
		m_arPMInput.Add(IDC_3RD_PM_PUBY_EDT);
		m_arPMInput.Add(IDC_3RD_PM_PUBZ_EDT);
		m_arPMInput.Add(IDC_3RD_PM_MUY_EDT);
		m_arPMInput.Add(IDC_3RD_PM_MUZ_EDT);

		m_arYLDInput.Add(IDC_3RD_BETAY_C);
		m_arYLDInput.Add(IDC_3RD_BETAY_T);
		m_arYLDInput.Add(IDC_3RD_BETAZ_C);
		m_arYLDInput.Add(IDC_3RD_BETAZ_T);
		m_arYLDInput.Add(IDC_3RD_GAMMA);
	}
	else
	{
		CArray<UINT, UINT> aCtrl_3rd;
		aCtrl_3rd.Add(IDC_3RD_BETAY_C);
		aCtrl_3rd.Add(IDC_3RD_BETAY_T);
		aCtrl_3rd.Add(IDC_3RD_BETAZ_C);
		aCtrl_3rd.Add(IDC_3RD_BETAZ_T);
		aCtrl_3rd.Add(IDC_3RD_GAMMA);
		aCtrl_3rd.Add(IDC_3RD_PM_PUT_EDT);
		aCtrl_3rd.Add(IDC_3RD_PM_PUC_EDT);
		aCtrl_3rd.Add(IDC_3RD_PM_PUBY_EDT);
		aCtrl_3rd.Add(IDC_3RD_PM_PUBZ_EDT);
		aCtrl_3rd.Add(IDC_3RD_PM_MUY_EDT);
		aCtrl_3rd.Add(IDC_3RD_PM_MUZ_EDT);
		aCtrl_3rd.Add(IDC_PMCV_3RD);
		CDlgUtil::CtrlEnableDisable(this, aCtrl_3rd, FALSE);
	}

	InitCtrl_Unable1stCtrl();

	Init3rdYieldCtrl();


	// hide the show value button when the material is steel
	CArray<UINT, UINT> arShowValue;
	arShowValue.Add(IDC_SHOWVALUE_CHK);
	CDlgUtil::CtrlShowHide(this, arShowValue, m_bConc);

	m_wndFormulaPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_iehp_ex.svg"));
	m_wndLegendPicture.SetIgnoreLargeView(TRUE);
	m_wndLegendPicture.SetImage(_T("SVG\\Illustration\\Dialog\\Lgnd1.svg"));
}

void CIehpYieldPropDlgNew::InitCtrl_Unable1stCtrl()
{
	if(!m_bUnable1stCtrl) return;
	
	// FEMA일 때 1st는 비활성화 시킴
	CArray<UINT,UINT> aCtrl_1st;
	aCtrl_1st.Add(IDC_STATIC1);
	aCtrl_1st.Add(IDC_PM_CRACK_STC1);
	aCtrl_1st.Add(IDC_PM_CRACK_STC2);
	aCtrl_1st.Add(IDC_PM_CRACK_STC3);
	aCtrl_1st.Add(IDC_1ST_PM_CRACK_PC0T_EDT);
	aCtrl_1st.Add(IDC_1ST_PM_CRACK_MC0Y_EDT);
	aCtrl_1st.Add(IDC_1ST_PM_CRACK_MC0Z_EDT);
	aCtrl_1st.Add(IDC_WG_CMD_STATIC3);
	aCtrl_1st.Add(IDC_WG_CMD_STATIC4);
	aCtrl_1st.Add(IDC_WG_CMD_STATIC5);
	aCtrl_1st.Add(IDC_WG_CMD_STATIC6);
	aCtrl_1st.Add(IDC_WG_CMD_STATIC7);
	aCtrl_1st.Add(IDC_WG_CMD_STATIC8);
	aCtrl_1st.Add(IDC_1ST_PM_PCT_EDT);
	aCtrl_1st.Add(IDC_1ST_PM_PCC_EDT);
	aCtrl_1st.Add(IDC_1ST_PM_PCBY_EDT);
	aCtrl_1st.Add(IDC_1ST_PM_PCBZ_EDT);
	aCtrl_1st.Add(IDC_1ST_PM_MCY_EDT);
	aCtrl_1st.Add(IDC_1ST_PM_MCZ_EDT);
	aCtrl_1st.Add(IDC_1ST_BETAY_C);
	aCtrl_1st.Add(IDC_1ST_BETAY_T);
	aCtrl_1st.Add(IDC_1ST_BETAZ_C);
	aCtrl_1st.Add(IDC_1ST_BETAZ_T);
	aCtrl_1st.Add(IDC_1ST_GAMMA);
	aCtrl_1st.Add(IDC_PMCV_1ST);
	aCtrl_1st.Add(IDC_STATIC2);
	CDlgUtil::CtrlEnableDisable(this, aCtrl_1st, FALSE);
}

void CIehpYieldPropDlgNew::Init3rdYieldCtrl()
{
	if (m_bUse3rdCtrl) return;

	CArray<UINT, UINT> aCtrl;

	GetDlgItem(IDC_WG_CMD_STATIC2)->SetWindowText(_LS(IDS_WG_CMD_STATIC2_2ND_YIELD));

	int nDistX,nDistY;
	CRect rTo, rMove;
	GetDlgItem(IDC_STATIC4)->GetWindowRect(rTo);
	GetDlgItem(IDC_3RD_PM_PUT_EDT)->GetWindowRect(rMove);
	nDistY = rTo.top - rMove.bottom;

	aCtrl.Add(IDC_WG_CMD_STATIC31);
	aCtrl.Add(IDC_WG_CMD_STATIC2);
	aCtrl.Add(IDC_PMCV_1ST);
	aCtrl.Add(IDC_PMCV_2ND);
	aCtrl.Add(IDC_IEHP_TABLE);
	aCtrl.Add(IDC_SHOWVALUE_CHK);
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	GetDlgItem(IDC_WG_CMD_STATIC1)->GetWindowRect(rMove);
	rMove.bottom += nDistY;
	ScreenToClient(rMove);
	GetDlgItem(IDC_WG_CMD_STATIC1)->MoveWindow(rMove);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_CMD_STATIC27);

	GetDlgItem(IDC_WG_CMD_STATIC40)->GetWindowRect(rTo);
	nDistY = rTo.top - rTo.bottom;
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(rMove);
	rMove.bottom += nDistY;
	ScreenToClient(rMove);
	GetDlgItem(IDC_WG_CMD_STATIC0)->MoveWindow(rMove);

	GetDlgItem(IDC_WG_CMD_STATIC15)->GetWindowRect(rMove);
	rMove.bottom += nDistY;
	ScreenToClient(rMove);
	GetDlgItem(IDC_WG_CMD_STATIC15)->MoveWindow(rMove);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_WG_CMD_STATIC28);
	aCtrl.Add(IDC_WG_CMD_STATIC29);
	aCtrl.Add(IDC_PICTURE);
	aCtrl.Add(IDC_PLOT_COMB);
	aCtrl.Add(IDC_WG_CMD_STATIC30);
	aCtrl.Add(IDC_IMAGE);
	aCtrl.Add(IDOK);
	aCtrl.Add(IDCANCEL);

	GetDlgItem(IDC_WG_CMD_STATIC1)->GetWindowRect(rTo);
	GetDlgItem(IDC_WG_CMD_STATIC28)->GetWindowRect(rMove);
	nDistY = rTo.bottom - rMove.bottom;
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);


	GetDlgItem(IDC_PMCV_3RD)->GetWindowRect(rTo);
	GetDlgItem(IDC_PMCV_2ND)->GetWindowRect(rMove);
	aCtrl.RemoveAll();
	aCtrl.Add(IDC_PMCV_2ND);
	nDistX = (rTo.left - rMove.left) * 0.5;
	CDlgUtil::CtrlMoveDistX(this, aCtrl, nDistX);

	GetDlgItem(IDC_HINGE_TAB1)->GetWindowRect(rMove);
	rMove.bottom += nDistY;
	ScreenToClient(rMove);
	GetDlgItem(IDC_HINGE_TAB1)->MoveWindow(rMove);

	GetWindowRect(rMove);
	rMove.bottom = rMove.bottom + nDistY;
	MoveWindow(rMove);

	aCtrl.RemoveAll();
	aCtrl.Add(IDC_3RD_PM_PUT_EDT);
	aCtrl.Add(IDC_3RD_PM_PUC_EDT);
	aCtrl.Add(IDC_3RD_PM_PUBY_EDT);
	aCtrl.Add(IDC_3RD_PM_PUBZ_EDT);
	aCtrl.Add(IDC_3RD_PM_MUY_EDT);
	aCtrl.Add(IDC_3RD_PM_MUZ_EDT);
	aCtrl.Add(IDC_3RD_BETAY_T);
	aCtrl.Add(IDC_3RD_BETAY_C);
	aCtrl.Add(IDC_3RD_BETAZ_T);
	aCtrl.Add(IDC_3RD_BETAZ_C);
	aCtrl.Add(IDC_3RD_GAMMA);
	aCtrl.Add(IDC_STATIC4);
	aCtrl.Add(IDC_WG_CMD_STATIC34);
	aCtrl.Add(IDC_WG_CMD_STATIC35);
	aCtrl.Add(IDC_WG_CMD_STATIC36);
	aCtrl.Add(IDC_WG_CMD_STATIC37);
	aCtrl.Add(IDC_WG_CMD_STATIC38);
	aCtrl.Add(IDC_WG_CMD_STATIC39);
	aCtrl.Add(IDC_WG_CMD_STATIC40);
	aCtrl.Add(IDC_PMCV_3RD);

	CDlgUtil::CtrlShowHide(this, aCtrl, FALSE);
}

void CIehpYieldPropDlgNew::OnShowValueChk() 
{
	Dlg2Data(TRUE);
	m_bShowValueSet = m_chkShowValue.GetCheck();
	Data2Dlg();
}

///////////////////////////////////////////////////////////////////////////////


