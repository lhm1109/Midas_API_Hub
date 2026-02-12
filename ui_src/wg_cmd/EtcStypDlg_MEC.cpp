// EtcStypDlg_MEC.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtcStypDlg_MEC.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtcStypDlg_MEC dialog


CEtcStypDlg_MEC::CEtcStypDlg_MEC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtcStypDlg_MEC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtcStypDlg_MEC)
	m_nStructType = 0;
	m_nMassType = 0;
	m_nStructureMass = 0;
	m_bConsiderOffset = TRUE;
	m_bConvertSelfWeight = FALSE;
	m_bAlignBeam = FALSE;
	m_bAlignSlab = FALSE;
	m_bConsiderRotMPF = FALSE;
	m_nMassAtNode = 0;
	//}}AFX_DATA_INIT
}


void CEtcStypDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcStypDlg_MEC)
	DDX_Control(pDX, IDC_ETC_PC_TEMP, m_ctrlIniTemp);
	DDX_Control(pDX, IDC_ETC_PC_GRAV, m_ctrlGravity);
	DDX_Control(pDX, IDC_ETC_PC_TEMP_UNIT, m_ctrlIniTempUnit);
	DDX_Control(pDX, IDC_ETC_PC_GRAV_UNIT, m_ctrlGravUnit);
	DDX_Radio(pDX, IDC_ETC_PC_ST_3D, m_nStructType);
	DDX_Radio(pDX, IDC_ETC_PC_MASS_TYPE2, m_nMassType);
	DDX_Radio(pDX, IDC_ETC_PC_STRUCT_MASS, m_nStructureMass);
	DDX_Check(pDX, IDC_ETC_PC_CONSIDER_OFFSET_CHK, m_bConsiderOffset);
	DDX_Check(pDX, IDC_ETC_PC_CONVERT_SELFWEIGHT_CHK, m_bConvertSelfWeight);
	DDX_Check(pDX, IDC_ETC_PC_ALIGN_BEAM_CHK, m_bAlignBeam);
	DDX_Check(pDX, IDC_ETC_PC_ALIGN_SLAB_CHK, m_bAlignSlab);
	DDX_Check(pDX, IDC_ETC_CONSIDER_ROT_RIGID_MPF_CHK, m_bConsiderRotMPF);
	DDX_Radio(pDX, IDC_CMD_STYP_CONSIDER_MASS_CENTROID, m_nMassAtNode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEtcStypDlg_MEC, CDialogMove)
	//{{AFX_MSG_MAP(CEtcStypDlg_MEC)
	//ON_BN_CLICKED(IDC_ETC_PC_MASS_TYPE,  OnEtcPcMassType)
	ON_BN_CLICKED(IDC_ETC_PC_MASS_TYPE2, OnEtcPcMassType)
	ON_BN_CLICKED(IDC_ETC_PC_MASS_TYPE3, OnEtcPcMassType)
	ON_BN_CLICKED(IDC_ETC_PC_STRUCT_MASS, OnEtcPcStructMass)
	ON_BN_CLICKED(IDC_ETC_PC_STRUCT_MASS2, OnEtcPcStructMass)
	ON_BN_CLICKED(IDC_ETC_PC_STRUCT_MASS3, OnEtcPcStructMass)

	ON_BN_CLICKED(IDC_ETC_PC_CONSIDER_OFFSET_CHK, OnEtcPcConsiderOffset)
	ON_BN_CLICKED(IDC_ETC_PC_CONVERT_SELFWEIGHT_CHK, OnEtcPcSelfWeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcStypDlg_MEC message handlers

BOOL CEtcStypDlg_MEC::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	// TODO: Add extra initialization here
	m_ctrlGravity.SetUnitType(CUnitCtrl::m_STYP_UNIT.dblGravity);
	m_ctrlGravUnit.SetUnitType(CUnitCtrl::m_STYP_UNIT.dblGravity);
	m_ctrlIniTemp.SetUnitType(CUnitCtrl::m_STYP_UNIT.dblInitTemp);
	m_ctrlIniTempUnit.SetUnitType(CUnitCtrl::m_STYP_UNIT.dblInitTemp);

	m_aCtrlConsiderOffset.RemoveAll();
	m_aCtrlLumpedMass.RemoveAll();
	m_aCtrlConsiderOffset.Add(IDC_ETC_PC_CONSIDER_OFFSET_CHK);
	m_aCtrlLumpedMass.Add(IDC_ETC_PC_STRUCT_MASS);
	m_aCtrlLumpedMass.Add(IDC_ETC_PC_STRUCT_MASS2);
	m_aCtrlLumpedMass.Add(IDC_ETC_PC_STRUCT_MASS3);

	// #if !defined(_CIVIL)
	//   GetDlgItem(IDC_ETC_PC_CONSIDER_OFFSET_CHK)->ShowWindow(FALSE);
	//   GetDlgItem(IDC_ETC_PC_MASS_TYPE3)->ShowWindow(FALSE);
	// #endif

	InitDialogData();
	CtrlManager();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEtcStypDlg_MEC::InitDialogData()
{
	T_STYP_D data;
	if (!m_pDoc->m_pAttrCtrl->GetStyp(data))
	{
		data.Initialize();
		data.dblGravity = m_pDoc->m_pInitCtrl->DefaultGravity();
	}

	CString csGravity;
	csGravity.Format(_T("%g"), data.dblGravity);
	m_ctrlGravity.SetWindowText(csGravity);

	CString csIniTemp;
	csIniTemp.Format(_T("%g"), data.dblInitTemp);
	m_ctrlIniTemp.SetWindowText(csIniTemp);

	m_nStructType = data.nStructureType;
	m_nMassType = data.nMassType - 1;
	m_nStructureMass = data.nStructureMass - 1;
	m_bConsiderOffset = data.bConsiderOffset;
	m_bConvertSelfWeight = data.bConvertSelfWeight;
	m_bAlignBeam = data.bFloorBeam;
	m_bAlignSlab = data.bFloorSlab;
	m_bConsiderRotMPF = data.bConsiderRotRigidMPF;
	m_nMassAtNode = data.nMassAtNode;
}

void CEtcStypDlg_MEC::OnOK()
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;

	T_STYP_D data;

	data.Initialize();
	data.dblGravity = m_ctrlGravity.GetEditValue();
	data.dblInitTemp = m_ctrlIniTemp.GetEditValue();
	data.nStructureType = m_nStructType;
	data.nStructureMass = m_nStructureMass + 1;
	data.nMassType = m_nMassType + 1;
	data.bConsiderOffset = m_bConsiderOffset;
	data.bConvertSelfWeight = m_bConvertSelfWeight;
	data.bFloorBeam = m_bAlignBeam;
	data.bFloorSlab = m_bAlignSlab;
	data.bConsiderRotRigidMPF = m_bConsiderRotMPF;
	data.nMassAtNode = m_nMassAtNode;

	if (!m_pDoc->m_pDataCtrl->AddStyp(data)) return;

	CDialogMove::OnOK();
}

void CEtcStypDlg_MEC::CtrlManager()
{
	//   if(m_nMassType != 1) m_bConsiderOffset = FALSE;
	//   if(m_nStructureMass == 1 ||m_nStructureMass == 2) m_bConsiderOffset = FALSE;
	// 
	//   CDlgUtil::CtrlEnableDisable(this, m_aCtrlLumpedMass, m_nMassType == 1);
	//   CDlgUtil::CtrlEnableDisable(this, m_aCtrlConsiderOffset, m_nMassType == 1 && m_nStructureMass == 0);

	if (m_bConvertSelfWeight)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLumpedMass, TRUE);
		if ((m_nMassType == 0 && m_bConsiderOffset))
		{
			GetDlgItem(IDC_ETC_PC_STRUCT_MASS2)->EnableWindow(TRUE);
			GetDlgItem(IDC_ETC_PC_STRUCT_MASS3)->EnableWindow(TRUE);
		}
		else if (m_nMassType == 1)
		{
			GetDlgItem(IDC_ETC_PC_STRUCT_MASS2)->EnableWindow(FALSE);
			GetDlgItem(IDC_ETC_PC_STRUCT_MASS3)->EnableWindow(FALSE);
		}
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLumpedMass, FALSE);
	}

	if (m_nMassType == 1)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConsiderOffset, FALSE);

		GetDlgItem(IDC_CMD_STYP_CONSIDER_MASS_CENTROID)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_STYP_CONSIDER_MASS_OFFSET)->EnableWindow(FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlConsiderOffset, TRUE);

		GetDlgItem(IDC_CMD_STYP_CONSIDER_MASS_CENTROID)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_STYP_CONSIDER_MASS_OFFSET)->EnableWindow(TRUE);
	}

	// _T("Lumped Mass") & Consider Off-diagonal Masses _T("OFF") 인 경우만 Option On/Off 선택가능
	BOOL bEnableConsiderRotMPF = FALSE;
	if (m_nMassType == 0 && !m_bConsiderOffset)   bEnableConsiderRotMPF = TRUE;
	GetDlgItem(IDC_ETC_CONSIDER_ROT_RIGID_MPF_CHK)->EnableWindow(bEnableConsiderRotMPF);

	//if(m_nStructureMass == 1 || m_nStructureMass == 2) m_bConsiderOffset = FALSE;
}

void CEtcStypDlg_MEC::OnEtcPcMassType()
{
	UpdateData(TRUE);
	CtrlManager();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLumpedMass, 0);

	//UpdateData(FALSE);
}

void CEtcStypDlg_MEC::OnEtcPcStructMass()
{
	UpdateData(TRUE);
	CtrlManager();
	UpdateData(FALSE);
}

void CEtcStypDlg_MEC::OnEtcPcConsiderOffset()
{
	UpdateData(TRUE);
	if (m_bConvertSelfWeight && m_bConsiderOffset)
	{
		m_nStructureMass = 0;
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLumpedMass, m_nStructureMass);
	}
	UpdateData(FALSE);
	CtrlManager();
}

void CEtcStypDlg_MEC::OnEtcPcSelfWeight()
{
	UpdateData(TRUE);
	CtrlManager();
	//UpdateData(FALSE);
}