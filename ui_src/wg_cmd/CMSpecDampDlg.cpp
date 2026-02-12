// CMSpecDampDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSpecDampDlg.h"

#include "ThisCalcDamping.h"
#include "DampGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditData.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSpecDampDlg dialog


CCMSpecDampDlg::CCMSpecDampDlg(T_SPLC_D* pData, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSpecDampDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSpecDampDlg)
	//}}AFX_DATA_INIT
	m_pAllData = 0;
	ASSERT(pData);
	m_pAllData = pData;

	m_CurData.Initialize();

	m_nFreqPeriodRdoDlg = 0;
	m_dFrequencyDlg = 1.0;
	m_dPeriodDlg = 1.0;  

	m_aCtrlDampType.Add(IDC_CMD_TYPE_RD1);
	m_aCtrlDampType.Add(IDC_CMD_TYPE_RD2);
	m_aCtrlDampType.Add(IDC_CMD_TYPE_RD3);

	m_aCtrlCoefDefn.Add(IDC_CMD_STIFF_TYPE_RD1);
	m_aCtrlCoefDefn.Add(IDC_CMD_STIFF_TYPE_RD2);

	m_aCtrlFreqPerd.Add(IDC_CMD_STIFF_MODE_RD1);
	m_aCtrlFreqPerd.Add(IDC_CMD_STIFF_MODE_RD2);

	m_pGrid = new CDampGrid;
}

CCMSpecDampDlg::~CCMSpecDampDlg()
{
	if (m_pGrid) delete m_pGrid;
}

void CCMSpecDampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSpecDampDlg)
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT52, m_editStiffDM2);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT51, m_editStiffDM1);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT42, m_editStiffPM2);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT41, m_editStiffPM1);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT32, m_editStiffFM2);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT31, m_editStiffFM1);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT22, m_editStiffSC);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT21, m_editStiffMC);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT12, m_editStiffSD);
	DDX_Control(pDX, IDC_CMD_STIFF_EDIT11, m_editStiffMD);
	DDX_Control(pDX, IDC_CMD_STIFF_MASS_CHK, m_chkStiffMass);
	DDX_Control(pDX, IDC_CMD_STIFF_STIFF_CHK, m_chkStiffStiff);
	//DDX_Control(pDX, IDC_CMD_DIRT_LIST, m_wndDampList);
	DDX_Control(pDX, IDC_CMD_DIRT_RATIO_EDIT, m_editDampAll);
	DDX_Control(pDX, IDC_CMD_TYPE_RD3, m_rdoBtnGroup);
	//DDX_Control(pDX, IDC_CMD_MODE_EDIT, m_editDampMode);
	//DDX_Control(pDX, IDC_CMD_DAMP_EDIT, m_editDampRatio);
	DDX_Control(pDX, IDC_CMD_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSpecDampDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSpecDampDlg)
	ON_BN_CLICKED(IDC_CMD_TYPE_RD1, OnCmdTypeRadio)
	ON_BN_CLICKED(IDC_CMD_STIFF_TYPE_RD1, OnCmdStiffTypeRadio)
	ON_BN_CLICKED(IDC_CMD_STIFF_MODE_RD1, OnCmdStiffModeRadio)
	ON_BN_CLICKED(IDC_CMD_STIFF_MASS_CHK, OnCmdStiffMassChk)
	ON_BN_CLICKED(IDC_CMD_STIFF_STIFF_CHK, OnCmdStiffStiffChk)
	ON_BN_CLICKED(IDC_CMD_STIFF_BTN, OnCmdStiffBtn)
	ON_BN_CLICKED(IDC_CMD_TYPE_RD2, OnCmdTypeRadio)
	ON_BN_CLICKED(IDC_CMD_TYPE_RD3, OnCmdTypeRadio)
	ON_BN_CLICKED(IDC_CMD_STIFF_TYPE_RD2, OnCmdStiffTypeRadio)
	ON_BN_CLICKED(IDC_CMD_STIFF_MODE_RD2, OnCmdStiffModeRadio)
	ON_EN_CHANGE(IDC_CMD_STIFF_EDIT31, OnChangeCmdCoefCalcEdit)
	ON_EN_CHANGE(IDC_CMD_STIFF_EDIT32, OnChangeCmdCoefCalcEdit)
	ON_EN_CHANGE(IDC_CMD_STIFF_EDIT41, OnChangeCmdCoefCalcEdit)
	ON_EN_CHANGE(IDC_CMD_STIFF_EDIT42, OnChangeCmdCoefCalcEdit)
	ON_EN_CHANGE(IDC_CMD_STIFF_EDIT51, OnChangeCmdCoefCalcEdit)
	ON_EN_CHANGE(IDC_CMD_STIFF_EDIT52, OnChangeCmdCoefCalcEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSpecDampDlg message handlers

BOOL CCMSpecDampDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	// init controls
	InitControls();

	// align controls
	AlignControl();

	// set current data to dialog
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSpecDampDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CCMSpecDampDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMSpecDampDlg::OnCmdTypeRadio() 
{
	// TODO: Add your control notification handler code here
	int nDampType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDampType, nDampType);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDirect,   nDampType == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMassSiff, nDampType == 1);	  
}

void CCMSpecDampDlg::OnCmdStiffTypeRadio() 
{
	// TODO: Add your control notification handler code here
	DoCtrlManager();
}

void CCMSpecDampDlg::OnCmdStiffModeRadio() 
{
	// TODO: Add your control notification handler code here
	DoCtrlManager();
	AutoCalcRmRkCoef();
}

void CCMSpecDampDlg::OnCmdStiffMassChk() 
{
	// TODO: Add your control notification handler code here
	DoCtrlManager();
	AutoCalcRmRkCoef();	
}

void CCMSpecDampDlg::OnCmdStiffStiffChk() 
{
	// TODO: Add your control notification handler code here
	DoCtrlManager();
	AutoCalcRmRkCoef();	
}

void CCMSpecDampDlg::OnCmdStiffBtn() 
{
	// TODO: Add your control notification handler code here
	ShowDampingRatio();
}

void CCMSpecDampDlg::OnChangeCmdCoefCalcEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	AutoCalcRmRkCoef();
}

/////////////////////////////////////////////////////////////////////////////
// user defined functions

void CCMSpecDampDlg::InitControls()
{
	// set unit type
	m_editStiffDM2.SetUnitType(0);
	m_editStiffDM1.SetUnitType(0);
	m_editStiffPM2.SetUnitType(0);
	m_editStiffPM1.SetUnitType(0);
	m_editStiffFM2.SetUnitType(0);
	m_editStiffFM1.SetUnitType(0);
	m_editStiffSC.SetUnitType(0);
	m_editStiffMC.SetUnitType(0);
	m_editStiffSD.SetUnitType(0);
	m_editStiffMD.SetUnitType(0);
	m_editDampAll.SetUnitType(0);
	//m_editDampMode.SetUnitType(0);
	//m_editDampRatio.SetUnitType(0);

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	m_pGrid->Initialize(pDoc);
	m_pGrid->SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
}

void CCMSpecDampDlg::AlignControl()
{
	// make control group
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDirect,   IDC_CMD_DIRT_FRM,  TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlMassSiff, IDC_CMD_STIFF_FRM, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlCoefCalc, IDC_CMD_STIFF_FRM_SUB, FALSE);

	// align controls
	CRect rFromMove, rToMove;  
	GetDlgItem(IDC_CMD_STIFF_FRM)->GetWindowRect(rToMove);
	GetDlgItem(IDC_CMD_DIRT_FRM)->GetWindowRect(rFromMove);
	int nDistY = rToMove.top - rFromMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlDirect, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

/*
CString CCMSpecDampDlg::DataToStr(int i, const T_THIS_BASE &data)
{
	CString value;
	if (i == 0) value.Format(_T("%d"), data.nModeId);
	else if (i == 1) value.Format(_T("%g"), data.dDamping);
	else value = _LS(IDS_WG_CMD__ADDD__Error);
	return value;
}
*/

void CCMSpecDampDlg::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlDampType, m_pAllData->nDampingType-1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDirect,   m_pAllData->nDampingType == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMassSiff, m_pAllData->nDampingType == 2);

	// [direct modal]-------------------------------
	m_editDampAll.SetEditUnit(m_pAllData->dDampAll);
	
	// set current data
	m_pGrid->SetData(m_pAllData->aDampData);
	
	// [mass and stiffness proportional]-------------
	PropData2Dlg();
}

BOOL CCMSpecDampDlg::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlDampType, m_pAllData->nDampingType);
	m_pAllData->nDampingType++;

	if (m_pAllData->nDampingType == 1)    // direct modal
	{
		m_pAllData->dDampAll = m_editDampAll.GetEditValue();
		m_pAllData->PropDamping.Initialize();
		m_pGrid->GetData(m_pAllData->aDampData);
	}
	else if (m_pAllData->nDampingType == 2)   // mass and stiffness proportional
	{
		if (!Dlg2PropData()) return FALSE;
		m_pAllData->dDampAll = 0.05;
		m_pAllData->aDampData.RemoveAll();
	}
	else if (m_pAllData->nDampingType == 3)  // group
	{
		m_pAllData->aDampData.RemoveAll();
		m_pAllData->PropDamping.Initialize();
	}    
	else ASSERT(0);

	return TRUE;
}

void CCMSpecDampDlg::PropData2Dlg()
{
	m_chkStiffMass.SetCheck(m_pAllData->PropDamping.bMassProp);
	m_chkStiffStiff.SetCheck(m_pAllData->PropDamping.bStiffProp);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCoefDefn, m_pAllData->PropDamping.nCoefDefine-1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlFreqPerd, m_pAllData->PropDamping.nFreqPeriod-1);

	if (m_pAllData->PropDamping.nCoefDefine == 1)
	{
		m_editStiffMD.SetEditUnit(m_pAllData->PropDamping.dRm);
		m_editStiffSD.SetEditUnit(m_pAllData->PropDamping.dRk);
		m_editStiffMC.SetWindowText(_T("--"));
		m_editStiffSC.SetWindowText(_T("--"));
	}
	else if (m_pAllData->PropDamping.nCoefDefine == 2)
	{
		m_editStiffMD.SetEditUnit(0);
		m_editStiffSD.SetEditUnit(0);
		m_editStiffMC.SetEditUnit(m_pAllData->PropDamping.dRm);
		m_editStiffSC.SetEditUnit(m_pAllData->PropDamping.dRk);
	}
	else ASSERT(0);

	m_editStiffFM1.SetEditUnit(m_pAllData->PropDamping.dFreqOne);
	m_editStiffFM2.SetEditUnit(m_pAllData->PropDamping.dFreqTwo);
	m_editStiffPM1.SetEditUnit(m_pAllData->PropDamping.dPeriodOne);
	m_editStiffPM2.SetEditUnit(m_pAllData->PropDamping.dPeriodTwo);
	m_editStiffDM1.SetEditUnit(m_pAllData->PropDamping.dDampingOne);
	m_editStiffDM2.SetEditUnit(m_pAllData->PropDamping.dDampingTwo);

	DoCtrlManager();
}

BOOL CCMSpecDampDlg::Dlg2PropData()
{
	m_pAllData->PropDamping.Initialize();

	m_pAllData->PropDamping.bMassProp = m_chkStiffMass.GetCheck()==1 ? TRUE : FALSE;
	m_pAllData->PropDamping.bStiffProp = m_chkStiffStiff.GetCheck()==1 ? TRUE : FALSE;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCoefDefn, m_pAllData->PropDamping.nCoefDefine);
	m_pAllData->PropDamping.nCoefDefine++;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlFreqPerd, m_pAllData->PropDamping.nFreqPeriod);
	m_pAllData->PropDamping.nFreqPeriod++;

	if (m_pAllData->PropDamping.nCoefDefine == 1)
	{
		m_pAllData->PropDamping.dRm = m_editStiffMD.GetEditValue();
		m_pAllData->PropDamping.dRk = m_editStiffSD.GetEditValue();
	}
	else if (m_pAllData->PropDamping.nCoefDefine == 2)
	{
		m_pAllData->PropDamping.dRm = m_editStiffMC.GetEditValue();
		m_pAllData->PropDamping.dRk = m_editStiffSC.GetEditValue();
		m_pAllData->PropDamping.dFreqOne = m_editStiffFM1.GetEditValue();
		m_pAllData->PropDamping.dFreqTwo = m_editStiffFM2.GetEditValue();
		m_pAllData->PropDamping.dPeriodOne = m_editStiffPM1.GetEditValue();
		m_pAllData->PropDamping.dPeriodTwo = m_editStiffPM2.GetEditValue();
		m_pAllData->PropDamping.dDampingOne = m_editStiffDM1.GetEditValue();
		m_pAllData->PropDamping.dDampingTwo = m_editStiffDM2.GetEditValue();
	}
	else return FALSE;

	return TRUE;
}

void CCMSpecDampDlg::AutoCalcRmRkCoef()
{
	if (!Dlg2PropData()) return;
	if (m_pAllData->PropDamping.nCoefDefine != 2) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc->m_pEditData->AutoCalcThisCoef(m_pAllData->PropDamping, 
																					m_pAllData->PropDamping.dRm, m_pAllData->PropDamping.dRk))
	{
		m_editStiffMC.SetEditUnit(m_pAllData->PropDamping.dRm);
		m_editStiffSC.SetEditUnit(m_pAllData->PropDamping.dRk);
	}
	else
	{
		if (m_pAllData->PropDamping.bMassProp)  m_editStiffMC.SetWindowText(_T("--"));
		if( m_pAllData->PropDamping.bStiffProp) m_editStiffSC.SetWindowText(_T("--"));
	}
}

void CCMSpecDampDlg::DoCtrlManager()
{
	int nCoefDefefine, nFreqPeriod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCoefDefn, nCoefDefefine);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlFreqPerd, nFreqPeriod);
	
	BOOL bMass  = m_chkStiffMass.GetCheck();
	BOOL bStiff = m_chkStiffStiff.GetCheck();

	GetDlgItem(IDC_CMD_STIFF_FRM_SUB)->EnableWindow(bMass || bStiff);
	GetDlgItem(IDC_CMD_STIFF_TYPE_RD1)->EnableWindow(bMass || bStiff);
	GetDlgItem(IDC_CMD_STIFF_TYPE_RD2)->EnableWindow(bMass || bStiff);  
	GetDlgItem(IDC_CMD_STIFF_EDIT11)->EnableWindow(nCoefDefefine == 0 && bMass);
	GetDlgItem(IDC_CMD_STIFF_EDIT12)->EnableWindow(nCoefDefefine == 0 && bStiff);
	GetDlgItem(IDC_CMD_STIFF_EDIT21)->EnableWindow(nCoefDefefine == 1 && bMass);
	GetDlgItem(IDC_CMD_STIFF_EDIT22)->EnableWindow(nCoefDefefine == 1 && bStiff);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCoefCalc, nCoefDefefine == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_STIFF_EDIT31)->EnableWindow(nCoefDefefine == 1 && nFreqPeriod == 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_STIFF_EDIT32)->EnableWindow(nCoefDefefine == 1 && nFreqPeriod == 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_STIFF_EDIT41)->EnableWindow(nCoefDefefine == 1 && nFreqPeriod == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_STIFF_EDIT42)->EnableWindow(nCoefDefefine == 1 && nFreqPeriod == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_STIFF_EDIT51)->EnableWindow(nCoefDefefine == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_STIFF_EDIT52)->EnableWindow(nCoefDefefine == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_STIFF_BTN)->EnableWindow(bMass || bStiff);
}

void CCMSpecDampDlg::ShowDampingRatio()
{	
	CRect rShowBtn, rDialog;  
	GetDlgItem(IDC_CMD_STIFF_BTN)->GetWindowRect(rShowBtn);
	GetWindowRect(rDialog);
	int nDistY = rDialog.bottom - rShowBtn.top;

	int nCoefDefn;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCoefDefn, nCoefDefn);

	double dRm = 0., dRk = 0.;
	if (nCoefDefn == 0)
	{
		dRm = m_editStiffMD.GetEditValue();
		dRk = m_editStiffSD.GetEditValue();
	}
	else
	{
		dRm = m_editStiffMC.GetEditValue();
		dRk = m_editStiffSC.GetEditValue();
	}

	CThisCalcDamping dlg;
	dlg.SetInitPos(D_INIT_POS_RT, 0, 0);
//  dlg.SetInitPos(D_INIT_POS_RB, 0, -nDistY);
	dlg.m_bMass  = m_chkStiffMass.GetCheck();
	dlg.m_bStiff = m_chkStiffStiff.GetCheck();
	dlg.m_dRm		 = dRm;
	dlg.m_dRk	   = dRk;
	dlg.m_nFreqPeriodRdo = m_nFreqPeriodRdoDlg;
	dlg.m_dFrequency =  m_dFrequencyDlg;
	dlg.m_dPeriod = m_dPeriodDlg;  
	if (dlg.DoModal() == IDOK)
	{
		m_nFreqPeriodRdoDlg = dlg.m_nFreqPeriodRdo;
		m_dFrequencyDlg = dlg.m_dFrequency;
		m_dPeriodDlg = dlg.m_dPeriod;
	}
}

