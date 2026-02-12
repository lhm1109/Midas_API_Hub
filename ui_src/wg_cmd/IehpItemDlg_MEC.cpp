// IehpItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpItemDlg_MEC.h"
#include "IehpPropDlg.h"
#include "IehpPropAxialDlg.h"
#include "IehpPropStlDlg.h"
#include "IehpYieldPropDlgNew_MEC.h"
#include "IsolatorLrbBilDlg.h"
#include "IsolatorLrbTriDlg.h"
#include "IsolatorDmpRbrDlg.h"
#include "IsolatorLrbH16Dlg.h"
#include "IsolatorLrbH30Dlg.h"
#include "IsolatorDmpH16RbrDlg.h"
#include "IsolatorDmpH30RbrDlg.h"
#include "IehpPropROHDDlg.h"
#include "IehpProp_MLinDlg.h"
#include "IehpMphiLocDlg.h"
#include "IehpFiberOutplDlg.h"
#include "CMIehcShearRDlg.h"
#include "IehpProp_PMmcDlg.h"
#include "IehpProp_PMmcMLDlg.h"
#include "IehpStiffnessOptionDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_ServiceProvider.h"

#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_gr\GR_ServiceDef.h"
#include "..\wg_rps\Rps_ServiceDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIehpItemDlg_MEC dialog

CIehpItemDlg_MEC::CIehpItemDlg_MEC(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpItemDlg_MEC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpItemDlg_MEC)
	m_strDesc = _T("");
	m_nInteractionType = -1;
	m_nMatlType = -1;
	m_nMembType = -1;
	m_nAutoUserLocMemb = 0;
	m_nLocMemb = 0;
	m_nWallType = 0;
	m_nElementType = HT_BEAM_COLM;
	m_nDefinitionType = 0;
	m_nNewHingeType = 0;
	m_nFiberAutoUser = 0;

	m_bAddData4D2E = FALSE;
	m_nAutoUserFiberOutpl = 0;
	m_bConsOutpl = 0;
	m_nAutoUserShearR = 0;
	m_dShearR = 0.4;
	//}}AFX_DATA_INIT
	m_Data.Initialize();
	m_bModify = FALSE;

	m_bExistIJDataYield = FALSE;
	m_bAssignHingeMode = FALSE;
	for ( int i = 0; i < 7; i++ )
		m_bExistIJPropDlg[i] = FALSE;

	m_nFlexStiff = 0;
	m_bUseNonlinearShearY = TRUE;
	m_bUseNonlinearShearZ = TRUE;

	for ( int i = 0; i < 6; i++ )
	{
		m_Data.AllProp.PROP[i].KINEMA.Initialize();
		m_Data.AllProp.PROP[i].ORIGIN.Initialize();
		m_Data.AllProp.PROP[i].PEAK.Initialize();
		m_Data.AllProp.PROP[i].CLOUGH.Initialize();
		m_Data.AllProp.PROP[i].DEGRAD.Initialize();
		m_Data.AllProp.PROP[i].TAKEDA.Initialize();
		m_Data.AllProp.PROP[i].NORBIL.Initialize();
		m_Data.AllProp.PROP[i].ELABIL.Initialize();
		m_Data.AllProp.PROP[i].ELATRI.Initialize();
		m_Data.AllProp.PROP[i].ELATET.Initialize();
		m_Data.AllProp.PROP[i].TAKTET.Initialize();
		m_Data.AllProp.PROP[i].LRBBIL.Initialize();
		m_Data.AllProp.PROP[i].LRBTRI.Initialize();
		m_Data.AllProp.PROP[i].DMPRBR.Initialize();
		m_Data.AllProp.PROP[i].LRBH16.Initialize();
		m_Data.AllProp.PROP[i].LRBH30.Initialize();
		m_Data.AllProp.PROP[i].DMPRH16.Initialize();
		m_Data.AllProp.PROP[i].DMPRH30.Initialize();
		m_Data.AllProp.PROP[i].ROHD.Initialize();
		m_Data.AllProp.PROP[i].AXIAL.Initialize();
		m_Data.AllProp.PROP[i].BUCKL.Initialize();
		m_Data.AllProp.PROP[i].SRCTET.Initialize();
		m_Data.AllProp.PMMULT[i].Initialize();

		m_Data.AllSubProp.PROP[i].KINEMA.Initialize();
		m_Data.AllSubProp.PROP[i].ORIGIN.Initialize();
		m_Data.AllSubProp.PROP[i].PEAK.Initialize();
		m_Data.AllSubProp.PROP[i].CLOUGH.Initialize();
		m_Data.AllSubProp.PROP[i].DEGRAD.Initialize();
		m_Data.AllSubProp.PROP[i].TAKEDA.Initialize();
		m_Data.AllSubProp.PROP[i].NORBIL.Initialize();
		m_Data.AllSubProp.PROP[i].ELABIL.Initialize();
		m_Data.AllSubProp.PROP[i].ELATRI.Initialize();
		m_Data.AllSubProp.PROP[i].ELATET.Initialize();
		m_Data.AllSubProp.PROP[i].TAKTET.Initialize();
		m_Data.AllSubProp.PROP[i].LRBBIL.Initialize();
		m_Data.AllSubProp.PROP[i].LRBTRI.Initialize();
		m_Data.AllSubProp.PROP[i].DMPRBR.Initialize();
		m_Data.AllSubProp.PROP[i].LRBH16.Initialize();
		m_Data.AllSubProp.PROP[i].LRBH30.Initialize();
		m_Data.AllSubProp.PROP[i].DMPRH16.Initialize();
		m_Data.AllSubProp.PROP[i].DMPRH30.Initialize();
		m_Data.AllSubProp.PROP[i].ROHD.Initialize();
		m_Data.AllSubProp.PROP[i].AXIAL.Initialize();
		m_Data.AllSubProp.PROP[i].BUCKL.Initialize();
		m_Data.AllSubProp.PROP[i].SRCTET.Initialize();
		m_Data.AllSubProp.PMMULT[i].Initialize();

		m_bPropMod[i] = FALSE;
		if ( i < 6 ) m_nMdlCombSel[i] = 0;
	}

	m_Data.AllProp.PROP[6].YLDSUR.Initialize();
	m_Data.AllSubProp.PROP[6].YLDSUR.Initialize();

	m_aHingeLocation.RemoveAll();
	m_aHingeLocation.Add(IDC_CMD_HINGE_LOCATION_TEXT);
	m_aHingeLocation.Add(IDC_CMD_HINGE_LOC_DX_CMB);
	m_aHingeLocation.Add(IDC_CMD_HINGE_LOC_DY_CMB);
	m_aHingeLocation.Add(IDC_CMD_HINGE_LOC_DZ_CMB);
	m_aHingeLocation.Add(IDC_CMD_HINGE_LOC_RX_CMB);
	m_aHingeLocation.Add(IDC_CMD_HINGE_LOC_RY_CMB);
	m_aHingeLocation.Add(IDC_CMD_HINGE_LOC_RZ_CMB);

	m_aNumofSection.RemoveAll();
	m_aNumofSection.Add(IDC_CMD_HINGE_LOCATION_TEXT2);
	m_aNumofSection.Add(IDC_CMD_SECTION_NUM_DX_EDIT);
	m_aNumofSection.Add(IDC_CMD_SECTION_NUM_DY_EDIT);
	m_aNumofSection.Add(IDC_CMD_SECTION_NUM_DZ_EDIT);
	m_aNumofSection.Add(IDC_CMD_SECTION_NUM_RX_EDIT);
	m_aNumofSection.Add(IDC_CMD_SECTION_NUM_RY_EDIT);
	m_aNumofSection.Add(IDC_CMD_SECTION_NUM_RZ_EDIT);

	//   m_aMembPos.RemoveAll();
	//   m_aMembPos.Add(IDC_STATIC_LOCATION_GRP);
	//   m_aMembPos.Add(IDC_STATIC_CURVATURE_BEAM_TEXT);
	//   m_aMembPos.Add(IDC_LOC_MEMB_I_RDO);
	//   m_aMembPos.Add(IDC_LOC_MEMB_M_RDO);
	//   m_aMembPos.Add(IDC_LOC_MEMB_J_RDO);

	m_aAssignHinge.Add(IDC_CMD_PROPERTY_NAME);
	m_aAssignHinge.Add(IDC_CMD_DESCRIPTION);
	m_aAssignHinge.Add(IDC_IEHP_ADD_DYNA2E_CHK);
	m_aAssignHinge.Add(IDC_MATL_TYPE_RC_SRC_ENCASED_RDO);
	m_aAssignHinge.Add(IDC_MATL_TYPE_STEEL_SRC_FILLED_RDO);
	m_aAssignHinge.Add(IDC_HING_TYPE_SPRING_RDO);
	m_aAssignHinge.Add(IDC_HING_TYPE_BEAM_RDO);
	m_aAssignHinge.Add(IDC_HING_TYPE_TRUS_RDO);
	m_aAssignHinge.Add(IDC_HING_TYPE_LUMP_RDO);
	m_aAssignHinge.Add(IDC_HING_TYPE_DIST_RDO);
	// 	m_aAssignHinge.Add(IDC_LOC_MEMB_I_RDO);
	// 	m_aAssignHinge.Add(IDC_LOC_MEMB_M_RDO);
	// 	m_aAssignHinge.Add(IDC_LOC_MEMB_J_RDO);
	m_aAssignHinge.Add(IDC_HINGE_TYPE_DEFINE_RDO1);
	m_aAssignHinge.Add(IDC_HINGE_TYPE_DEFINE_RDO2);
	m_aAssignHinge.Add(IDC_SECT_CMB);
	m_aAssignHinge.Add(IDC_INTR_NONE_RDO);
	m_aAssignHinge.Add(IDC_INTR_PM_RDO);
	m_aAssignHinge.Add(IDC_INTR_PMM_RDO);
	m_aAssignHinge.Add(IDC_INTR_PMMC_RDO);
	m_aAssignHinge.Add(IDC_CMD_DOF_DX_CHK);
	m_aAssignHinge.Add(IDC_CMD_DOF_DY_CHK);
	m_aAssignHinge.Add(IDC_CMD_DOF_DZ_CHK);
	m_aAssignHinge.Add(IDC_CMD_DOF_RX_CHK);
	m_aAssignHinge.Add(IDC_CMD_DOF_RY_CHK);
	m_aAssignHinge.Add(IDC_CMD_DOF_RZ_CHK);
	m_aAssignHinge.Add(IDC_CMD_HINGE_LOC_DX_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_LOC_DY_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_LOC_DZ_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_LOC_RX_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_LOC_RY_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_LOC_RZ_CMB);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_DX_CMB);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_DY_CMB);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_DZ_CMB);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_RX_CMB);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_RY_CMB);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_RZ_CMB);
	m_aAssignHinge.Add(IDC_CMD_SECTION_NUM_DX_EDIT);
	m_aAssignHinge.Add(IDC_CMD_SECTION_NUM_DY_EDIT);
	m_aAssignHinge.Add(IDC_CMD_SECTION_NUM_DZ_EDIT);
	m_aAssignHinge.Add(IDC_CMD_SECTION_NUM_RX_EDIT);
	m_aAssignHinge.Add(IDC_CMD_SECTION_NUM_RY_EDIT);
	m_aAssignHinge.Add(IDC_CMD_SECTION_NUM_RZ_EDIT);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_DX_BTN);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_DY_BTN);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_DZ_BTN);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_RX_BTN);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_RY_BTN);
	m_aAssignHinge.Add(IDC_CMD_HYS_MODEL_RZ_BTN);
	m_aAssignHinge.Add(IDC_CMD_FIBER_NAME_CMB);
	m_aAssignHinge.Add(IDOK);
	m_aAssignHinge.Add(IDC_CMD_APPLY);
	m_aAssignHinge.Add(IDC_HINGE_TYPE_FIBER_AUTO_RDO);
	m_aAssignHinge.Add(IDC_HINGE_TYPE_FIBER_USER_RDO);
	m_aAssignHinge.Add(IDC_SECT_STC);
	m_aAssignHinge.Add(IDC_CMD_FIBER_NAME_STC);
	m_aAssignHinge.Add(IDC_CMD_FIBER_NAME_BTN);
	m_aAssignHinge.Add(IDC_HINGE_TYPE_FIBER_OUTPLANE_BTN);
	m_aAssignHinge.Add(IDC_HING_TYPE_SUPPORT_RDO);

	m_aAssignHinge.Add(IDC_HING_TYPE_FLEXIBILITY_RDO);
	m_aAssignHinge.Add(IDC_HING_TYPE_STIFFNESS_RDO);
	m_aAssignHinge.Add(IDC_HING_STIFFNESS_OPTION_BTN);
	m_aAssignHinge.Add(IDC_CMD_HINGE_LOCATION_TEXT3);
	m_aAssignHinge.Add(IDC_CMD_HINGE_STIFF_LOC_DX_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_STIFF_LOC_DY_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_STIFF_LOC_DZ_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_STIFF_LOC_RX_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_STIFF_LOC_RY_CMB);
	m_aAssignHinge.Add(IDC_CMD_HINGE_STIFF_LOC_RZ_CMB);
	// Civil은 벽체가 없기 때문에 wall 컨트롤들 비활성화(141216)
#if !defined(_CIVIL)
	m_aAssignHinge.Add(IDC_HING_TYPE_WALLCRB_RDO);
	m_aAssignHinge.Add(IDC_HINGE_TYPE_WALL_RDO1);
	m_aAssignHinge.Add(IDC_HINGE_TYPE_WALL_RDO2);
#endif

	m_aCtrlIDFiberShearR.Add(IDC_CMD_PROP_SHEAR_R_BTN);
	m_aCtrlIDFiberShearR.Add(IDC_CMD_PROP_SHEAR_R_EDT);
	m_aCtrlIDFiberShearR.Add(IDC_CMD_PROP_SHEAR_R_STC);

	m_aCtrlStiffnessCmb.RemoveAll();
	m_aCtrlStiffnessCmb.Add(IDC_CMD_HINGE_LOCATION_TEXT3);
	m_aCtrlStiffnessCmb.Add(IDC_CMD_HINGE_STIFF_LOC_DX_CMB);
	m_aCtrlStiffnessCmb.Add(IDC_CMD_HINGE_STIFF_LOC_DY_CMB);
	m_aCtrlStiffnessCmb.Add(IDC_CMD_HINGE_STIFF_LOC_DZ_CMB);
	m_aCtrlStiffnessCmb.Add(IDC_CMD_HINGE_STIFF_LOC_RX_CMB);
	m_aCtrlStiffnessCmb.Add(IDC_CMD_HINGE_STIFF_LOC_RY_CMB);
	m_aCtrlStiffnessCmb.Add(IDC_CMD_HINGE_STIFF_LOC_RZ_CMB);
}

void CIehpItemDlg_MEC::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpItemDlg_MEC)
	DDX_Control(pDX, IDC_SECT_CMB, m_cmbSect);
	DDX_Control(pDX, IDC_CMD_PROPERTY_NAME, m_edtName);
	DDX_Text(pDX, IDC_CMD_DESCRIPTION, m_strDesc);
	DDX_Radio(pDX, IDC_INTR_NONE_RDO, m_nInteractionType);
	DDX_Radio(pDX, IDC_MATL_TYPE_RC_SRC_ENCASED_RDO, m_nMatlType);
	DDX_Radio(pDX, IDC_HING_TYPE_BEAM_RDO, m_nElementType);
	DDX_Radio(pDX, IDC_HING_TYPE_LUMP_RDO, m_nDefinitionType);
	DDX_Radio(pDX, IDC_HINGE_TYPE_DEFINE_RDO1, m_nNewHingeType);

	DDX_Check(pDX, IDC_IEHP_ADD_DYNA2E_CHK, m_bAddData4D2E);
	DDX_Control(pDX, IDC_CMD_FIBER_NAME_CMB, m_cmbFibrName);
	DDX_Radio(pDX, IDC_HINGE_TYPE_FIBER_AUTO_RDO, m_nFiberAutoUser);

	DDX_Radio(pDX, IDC_HINGE_TYPE_WALL_RDO1, m_nWallType);
	DDX_Radio(pDX, IDC_HING_TYPE_FLEXIBILITY_RDO, m_nFlexStiff);

	DDX_Control(pDX, IDC_CMD_HINGE_STIFF_LOC_DX_CMB, m_cmbHngStifLoc[0]);
	DDX_Control(pDX, IDC_CMD_HINGE_STIFF_LOC_DY_CMB, m_cmbHngStifLoc[1]);
	DDX_Control(pDX, IDC_CMD_HINGE_STIFF_LOC_DZ_CMB, m_cmbHngStifLoc[2]);
	DDX_Control(pDX, IDC_CMD_HINGE_STIFF_LOC_RX_CMB, m_cmbHngStifLoc[3]);
	DDX_Control(pDX, IDC_CMD_HINGE_STIFF_LOC_RY_CMB, m_cmbHngStifLoc[4]);
	DDX_Control(pDX, IDC_CMD_HINGE_STIFF_LOC_RZ_CMB, m_cmbHngStifLoc[5]);

	//}}AFX_DATA_MAP
	UINT nDof[6] = { IDC_CMD_DOF_DX_CHK, IDC_CMD_DOF_DY_CHK, IDC_CMD_DOF_DZ_CHK,
									 IDC_CMD_DOF_RX_CHK, IDC_CMD_DOF_RY_CHK, IDC_CMD_DOF_RZ_CHK, };
	UINT nSec[6] = { IDC_CMD_SECTION_NUM_DX_EDIT, IDC_CMD_SECTION_NUM_DY_EDIT,
									 IDC_CMD_SECTION_NUM_DZ_EDIT, IDC_CMD_SECTION_NUM_RX_EDIT,
									 IDC_CMD_SECTION_NUM_RY_EDIT, IDC_CMD_SECTION_NUM_RZ_EDIT, };
	UINT nLoc[6] = { IDC_CMD_HINGE_LOC_DX_CMB, IDC_CMD_HINGE_LOC_DY_CMB, IDC_CMD_HINGE_LOC_DZ_CMB,
									 IDC_CMD_HINGE_LOC_RX_CMB, IDC_CMD_HINGE_LOC_RY_CMB, IDC_CMD_HINGE_LOC_RZ_CMB, };
	UINT nMod[6] = { IDC_CMD_HYS_MODEL_DX_CMB, IDC_CMD_HYS_MODEL_DY_CMB, IDC_CMD_HYS_MODEL_DZ_CMB,
									 IDC_CMD_HYS_MODEL_RX_CMB, IDC_CMD_HYS_MODEL_RY_CMB, IDC_CMD_HYS_MODEL_RZ_CMB, };
	UINT nModB[6] = { IDC_CMD_HYS_MODEL_DX_BTN, IDC_CMD_HYS_MODEL_DY_BTN, IDC_CMD_HYS_MODEL_DZ_BTN,
									 IDC_CMD_HYS_MODEL_RX_BTN, IDC_CMD_HYS_MODEL_RY_BTN, IDC_CMD_HYS_MODEL_RZ_BTN, };

	for ( int i = 0; i < 6; i++ )
	{
		DDX_Check(pDX, nDof[i], m_bDof[i]);
		DDX_Text(pDX, nSec[i], m_strSec[i]);
		DDX_Control(pDX, nLoc[i], m_cmbHngLoc[i]);
		DDX_Control(pDX, nMod[i], m_cmbModel[i]);
		DDX_Control(pDX, nModB[i], m_btnModel[i]);
	}
	DDX_Control(pDX, IDC_CMD_PROP_SHEAR_R_EDT, m_edtShearR);
	// 잠깐: 여기서 MenuBtn은 Combo를 대신해서 사용되나 사용자 인터페이스 부분만
	//       처리하고 나머지는 기존 Combo가 그 역할을 계속하는 방식이다.
}

BEGIN_MESSAGE_MAP(CIehpItemDlg_MEC, CDialogMove)
	//{{AFX_MSG_MAP(CIehpItemDlg_MEC)  
	ON_BN_CLICKED(IDC_HING_TYPE_BEAM_RDO, OnCmdElementType)
	ON_BN_CLICKED(IDC_HING_TYPE_SPRING_RDO, OnCmdElementType)
	ON_BN_CLICKED(IDC_HING_TYPE_TRUS_RDO, OnCmdElementType)
	ON_BN_CLICKED(IDC_HING_TYPE_WALLCRB_RDO, OnCmdElementType)
	ON_BN_CLICKED(IDC_HING_TYPE_SUPPORT_RDO, OnCmdElementType)

	ON_BN_CLICKED(IDC_HING_TYPE_LUMP_RDO, OnCmdDefinitionType)
	ON_BN_CLICKED(IDC_HING_TYPE_DIST_RDO, OnCmdDefinitionType)

	ON_BN_CLICKED(IDC_HINGE_TYPE_WALL_RDO1, OnCmdWallType)
	ON_BN_CLICKED(IDC_HINGE_TYPE_WALL_RDO2, OnCmdWallType)

	ON_BN_CLICKED(IDC_HINGE_TYPE_DEFINE_RDO1, OnCmdNewHingeType)
	ON_BN_CLICKED(IDC_HINGE_TYPE_DEFINE_RDO2, OnCmdNewHingeType)

	ON_BN_CLICKED(IDC_INTR_NONE_RDO, OnInteractionType)
	ON_BN_CLICKED(IDC_INTR_PM_RDO, OnInteractionType)
	ON_BN_CLICKED(IDC_INTR_PMM_RDO, OnInteractionType)
	ON_BN_CLICKED(IDC_INTR_PMMC_RDO, OnInteractionType)

	ON_BN_CLICKED(IDC_MATL_TYPE_RC_SRC_ENCASED_RDO, OnMaterialType)
	ON_BN_CLICKED(IDC_MATL_TYPE_STEEL_SRC_FILLED_RDO, OnMaterialType)

	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_DOF_DX_CHK, OnCmdDofDxChk)
	ON_BN_CLICKED(IDC_CMD_DOF_RY_CHK, OnCmdDofRyChk)
	ON_BN_CLICKED(IDC_CMD_DOF_RZ_CHK, OnCmdDofRzChk)
	ON_BN_CLICKED(IDC_CMD_PROP_DX_BTN, OnCmdPropDxBtn)
	ON_BN_CLICKED(IDC_CMD_PROP_DY_BTN, OnCmdPropDyBtn)
	ON_BN_CLICKED(IDC_CMD_PROP_DZ_BTN, OnCmdPropDzBtn)
	ON_BN_CLICKED(IDC_CMD_PROP_RX_BTN, OnCmdPropRxBtn)
	ON_BN_CLICKED(IDC_CMD_PROP_RY_BTN, OnCmdPropRyBtn)
	ON_BN_CLICKED(IDC_CMD_PROP_RZ_BTN, OnCmdPropRzBtn)
	ON_BN_CLICKED(IDC_CMD_PROP_YS_BTN, OnCmdPropYsBtn)
	ON_EN_CHANGE(IDC_CMD_SECTION_NUM_DX_EDIT, OnChangeSectDxEdit)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_LOC_DX_CMB, OnSelchangeLocDxCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HYS_MODEL_DX_CMB, OnSelchangeModelDxCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HYS_MODEL_DY_CMB, OnSelchangeModelDyCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HYS_MODEL_DZ_CMB, OnSelchangeModelDzCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HYS_MODEL_RX_CMB, OnSelchangeModelRxCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HYS_MODEL_RY_CMB, OnSelchangeModelRyCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HYS_MODEL_RZ_CMB, OnSelchangeModelRzCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_LOC_RY_CMB, OnLocationChangedRyCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_LOC_RZ_CMB, OnLocationChangedRzCmb)

	ON_BN_CLICKED(IDC_CMD_DOF_DY_CHK, OnCmdDofDyChk)
	ON_BN_CLICKED(IDC_CMD_DOF_DZ_CHK, OnCmdDofDzChk)
	ON_BN_CLICKED(IDC_CMD_DOF_RX_CHK, OnCmdDofRxChk)

	ON_CBN_SELCHANGE(IDC_SECT_CMB, OnSelchangeSectCmb)
	ON_BN_CLICKED(IDC_IEHP_ADD_DYNA2E_CHK, OnAddDyna2E)
	ON_BN_CLICKED(IDC_CMD_HYS_MODEL_DX_BTN, OnModelChangedDx)
	ON_BN_CLICKED(IDC_CMD_HYS_MODEL_DY_BTN, OnModelChangedDy)
	ON_BN_CLICKED(IDC_CMD_HYS_MODEL_DZ_BTN, OnModelChangedDz)
	ON_BN_CLICKED(IDC_CMD_HYS_MODEL_RX_BTN, OnModelChangedRx)
	ON_BN_CLICKED(IDC_CMD_HYS_MODEL_RY_BTN, OnModelChangedRy)
	ON_BN_CLICKED(IDC_CMD_HYS_MODEL_RZ_BTN, OnModelChangedRz)
	ON_BN_CLICKED(IDC_HING_TYPE_DIST_REF_LOC_BTN, OnMphiLocBtn)
	ON_BN_CLICKED(IDC_CMD_FIBER_NAME_BTN, OnFiberNameBtn)
	ON_BN_CLICKED(IDC_HINGE_TYPE_FIBER_OUTPLANE_BTN, OnFiberOutplBtn)
	ON_BN_CLICKED(IDC_CMD_PROP_SHEAR_R_BTN, OnShearRBtn)
	ON_BN_CLICKED(IDC_HINGE_TYPE_FIBER_AUTO_RDO, CtrlManager)
	ON_BN_CLICKED(IDC_HINGE_TYPE_FIBER_USER_RDO, CtrlManager)

	ON_BN_CLICKED(IDC_HING_TYPE_FLEXIBILITY_RDO, OnFlexStifType)
	ON_BN_CLICKED(IDC_HING_TYPE_STIFFNESS_RDO, OnFlexStifType)
	ON_BN_CLICKED(IDC_HING_STIFFNESS_OPTION_BTN, OnStiffnessOptionBtn)


	ON_CBN_SELCHANGE(IDC_CMD_HINGE_STIFF_LOC_DX_CMB, OnSelchangeStiffLocDxCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_STIFF_LOC_DY_CMB, OnSelchangeStiffLocDyCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_STIFF_LOC_DZ_CMB, OnSelchangeStiffLocDzCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_STIFF_LOC_RX_CMB, OnSelchangeStiffLocRxCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_STIFF_LOC_RY_CMB, OnSelchangeStiffLocRyCmb)
	ON_CBN_SELCHANGE(IDC_CMD_HINGE_STIFF_LOC_RZ_CMB, OnSelchangeStiffLocRzCmb)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpItemDlg_MEC Data & Window handlers
void CIehpItemDlg_MEC::SetParamData(T_IEHP_K Key, T_IEHP_D& data)
{
	m_Key = m_OldKey = Key;
	m_bModify = TRUE;
	m_Data = data;
	m_csOldName = m_Data.strName;
	for ( int i = 0; i < 7; i++ ) if ( m_Data.cDir[i] == '1' ) m_bPropMod[i] = TRUE;
}
void CIehpItemDlg_MEC::AlignCtrl()
{
	// MQC 4949-53. Civil에서는 Wall 안보이게 숨김.
#if defined(_CIVIL)
	GetDlgItem(IDC_HING_TYPE_WALLCRB_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_WALL_TYPE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO2)->ShowWindow(SW_HIDE);

// 	// resize Material Type Group Box
// 	{
// 		CWnd* pWndRef = GetDlgItem(IDC_WG_CMD_STATIC9); ASSERT(pWndRef);
// 		CWnd* pWndRDORef = GetDlgItem(IDC_HINGE_TYPE_DEFINE_RDO1); ASSERT(pWndRDORef);
// 		CWnd* pWndSrc = GetDlgItem(IDC_WG_CMD_STATIC6); ASSERT(pWndSrc);
// 
// 		CRect rcRef, rcSrc;
// 		pWndRef->GetWindowRect(rcRef);
// 		pWndSrc->GetWindowRect(rcSrc);
// 		rcSrc.right = rcRef.right;
// 		rcSrc.left = rcRef.left;
// 		this->ScreenToClient(rcSrc);
// 		pWndSrc->MoveWindow(rcSrc, TRUE);
// 
// 		CRect rcRDORef;
// 		pWndRDORef->GetWindowRect(rcRDORef);
// 
// 		CWnd* pRadio1 = GetDlgItem(IDC_MATL_TYPE_RC_SRC_ENCASED_RDO);
// 		CRect rcRadio1;
// 		pRadio1->GetWindowRect(rcRadio1);
// 		rcRadio1.right = rcRDORef.right;
// 		rcRadio1.left = rcRDORef.left;
// 		this->ScreenToClient(rcRadio1);
// 		pRadio1->MoveWindow(rcRadio1, TRUE);
// 
// 		auto* pRadio2 = GetDlgItem(IDC_MATL_TYPE_STEEL_SRC_FILLED_RDO);
// 		CRect rcRadio2;
// 		pRadio2->GetWindowRect(rcRadio2);
// 		rcRadio2.right = rcRDORef.right;
// 		rcRadio2.left = rcRDORef.left;
// 		this->ScreenToClient(rcRadio2);
// 		pRadio2->MoveWindow(rcRadio2, TRUE);
// 	}
// 	// resize Element Type Group Box
// 	{
// 		CWnd* pWndRef = GetDlgItem(IDC_WG_CMD_STATIC10); ASSERT(pWndRef);
// 		CWnd* pWndSrc = GetDlgItem(IDC_WG_CMD_STATIC7); ASSERT(pWndSrc);
// 
// 		CRect rcRef, rcSrc;
// 		pWndRef->GetWindowRect(rcRef);
// 		pWndSrc->GetWindowRect(rcSrc);
// 		rcSrc.right = rcRef.right;
// 		this->ScreenToClient(rcSrc);
// 		pWndSrc->MoveWindow(rcSrc, TRUE);
// 
// 		// Move Radio (Point Spring Support)
// 		CWnd* pWndRDORef = GetDlgItem(IDC_HING_TYPE_WALLCRB_RDO);
// 		CWnd* pRadio = GetDlgItem(IDC_HING_TYPE_SUPPORT_RDO);
// 
// 		CRect rcRDORef, rcRadio;
// 		pWndRDORef->GetWindowRect(rcRDORef);
// 		pRadio->GetWindowRect(rcRadio);
// 		const auto nWidth = rcRadio.right - rcRadio.left;
// 		rcRadio.left = rcRDORef.left;
// 		rcRadio.top = rcRDORef.top;
// 		rcRadio.bottom = rcRDORef.bottom;
// 		rcRadio.right = rcRadio.left + nWidth;
// 		this->ScreenToClient(rcRadio);
// 		pRadio->MoveWindow(rcRadio, TRUE);
// 	}
#endif

	CRect r1, rRef, rToMove;
	int nDistX;
	CWnd* pWnd[2];
	pWnd[0] = GetDlgItem(IDC_CMD_HINGE_LOC_DX_CMB);
	pWnd[1] = GetDlgItem(IDC_CMD_SECTION_NUM_DX_EDIT);

	pWnd[0]->GetWindowRect(rRef);
	pWnd[1]->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aNumofSection, nDistX);

	pWnd[1] = GetDlgItem(IDC_CMD_HINGE_STIFF_LOC_DX_CMB);
	pWnd[1]->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlStiffnessCmb, nDistX);


	// MenuBtn이 Combo 대체
	UINT nMod[6] = { IDC_CMD_HYS_MODEL_DX_CMB, IDC_CMD_HYS_MODEL_DY_CMB, IDC_CMD_HYS_MODEL_DZ_CMB,
		IDC_CMD_HYS_MODEL_RX_CMB, IDC_CMD_HYS_MODEL_RY_CMB, IDC_CMD_HYS_MODEL_RZ_CMB, };
	UINT nModB[6] = { IDC_CMD_HYS_MODEL_DX_BTN, IDC_CMD_HYS_MODEL_DY_BTN, IDC_CMD_HYS_MODEL_DZ_BTN,
									 IDC_CMD_HYS_MODEL_RX_BTN, IDC_CMD_HYS_MODEL_RY_BTN, IDC_CMD_HYS_MODEL_RZ_BTN, };
	for ( int i = 0; i < 6; i++ )
	{
		pWnd[0] = GetDlgItem(nMod[i]);
		pWnd[1] = GetDlgItem(nModB[i]);
		pWnd[0]->GetWindowRect(r1);
		ScreenToClient(r1);
		pWnd[1]->MoveWindow(r1, FALSE);
		pWnd[0]->ShowWindow(SW_HIDE);
	}

	BOOL bHideStiffMethod = TRUE;
#ifdef _JP
	bHideStiffMethod = FALSE;
#else
	bHideStiffMethod = !CTestEnvMgr::IsTestEnvST(_T("IehpMethod"), _T("yes"));
#endif
	if (bHideStiffMethod)
	{
		HideStiffnessMethodCtrl();
	}

	// resize self size  (대화상자 오른쪽에 있는 controls 숨기기)
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.right = rectLast.right + globalUtils.ScaleByDPI(8);

	MoveWindow(r, FALSE);
}

void CIehpItemDlg_MEC::HideStiffnessMethodCtrl()
{
	CRect rRef, rMov;
	int nDistY;

	GetDlgItem(IDC_HING_TYPE_DIST_RDO)->GetWindowRect(&rRef);
	GetDlgItem(IDC_WG_CMD_STATIC9)->GetWindowRect(&rMov);
	ScreenToClient(&rRef);
	ScreenToClient(&rMov);

	nDistY = rRef.bottom - rMov.bottom + globalUtils.ScaleByDPI(8);

	CDlgUtil::CtrlResizeBottom(this, IDC_WG_CMD_STATIC9, nDistY, FALSE);
	CDlgUtil::CtrlResizeBottom(this, IDC_WG_CMD_STATIC10, nDistY, FALSE);


	GetDlgItem(IDC_WG_CMD_STATIC9)->GetWindowRect(&rRef);
	ScreenToClient(&rRef);

	CArray<UINT, UINT> aCtrlID;
	for (CWnd* pChild = GetWindow(GW_CHILD); pChild != nullptr; pChild = pChild->GetWindow(GW_HWNDNEXT))
	{
		UINT nID = pChild->GetDlgCtrlID();
		if (nID <= 0) continue;

		pChild->GetWindowRect(&rMov);
		ScreenToClient(&rMov);

		if (rRef.bottom < rMov.top)
		{
			aCtrlID.Add(pChild->GetDlgCtrlID());
		}
	}

	if (aCtrlID.GetSize() > 0)
	{
		CDlgUtil::CtrlMoveDistY(this, aCtrlID, nDistY, FALSE);
	}

	aCtrlID.RemoveAll();
	aCtrlID.Add(IDC_HING_TYPE_FLEXIBILITY_RDO);
	aCtrlID.Add(IDC_HING_TYPE_STIFFNESS_RDO);
	aCtrlID.Add(IDC_HING_STIFFNESS_OPTION_BTN);
	aCtrlID.Add(IDC_WG_CMD_STATIC53);
	aCtrlID.Add(IDC_CMD_HINGE_LOCATION_TEXT3);
	aCtrlID.Add(IDC_CMD_HINGE_STIFF_LOC_DX_CMB);
	aCtrlID.Add(IDC_CMD_HINGE_STIFF_LOC_DY_CMB);
	aCtrlID.Add(IDC_CMD_HINGE_STIFF_LOC_DZ_CMB);
	aCtrlID.Add(IDC_CMD_HINGE_STIFF_LOC_RX_CMB);
	aCtrlID.Add(IDC_CMD_HINGE_STIFF_LOC_RY_CMB);
	aCtrlID.Add(IDC_CMD_HINGE_STIFF_LOC_RZ_CMB);

	CDlgUtil::CtrlShowHide(this, aCtrlID, FALSE);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CIehpItemDlg_MEC::InitSectComboCtrl()
{
	ASSERT(m_nNewHingeType == 1);

	if ( m_nElementType == HT_BEAM_COLM || m_nElementType == HT_TRUSS )
		InitSectComboCtrl_Sect();
	else if ( m_nElementType == HT_WALL_CRB )
		InitSectComboCtrl_Thik();
}

void CIehpItemDlg_MEC::InitSectComboCtrl_Sect()
{
	m_cmbSect.ResetContent();

	GetDlgItem(IDC_SECT_STC)->SetWindowText(_LS(IDS_CMD_CSCS__Section));

	CArray<T_SECT_K, T_SECT_K> aSectK;
	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	CArray<T_MATL_K, T_MATL_K> aMatlK;
	CArray<UINT, UINT> aSelSectK;

	aSectK.RemoveAll();
	aElemK.RemoveAll();
	aMatlK.RemoveAll();
	aSelSectK.RemoveAll();

	m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
	// 2014-11-05. by Hsshim. Fiber용 단면에서는 재질타입과 무관하게 단면 리스트가 나오도록 수정. (김종민 부장님과 협의)
	// 	for(int i=aMatlK.GetSize() -1 ; i>= 0; i--)
	// 	{
	// 		if(!m_pDoc->m_pEditData->CheckMatl4Iehp(aMatlK[i], m_nMatlType)) 
	// 			aMatlK.RemoveAt(i);
	// 	}

	if ( aMatlK.GetSize() == 0 )
	{
		T_MATL_K tempKey = -1;
		aMatlK.Add(tempKey);
	}

	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);
	m_pDoc->m_pAttrCtrl->GetElemKeyListMatlSectUser(&aMatlK, &aSectK, &aElemK);
	int nSize = aElemK.GetSize();
	if ( nSize < 1 ) return;

	T_ELEM_D ElemD;
	for ( int i = 0; i < nSize; i++ )
	{
		ElemD.Initialize();
		BOOL bAdd = TRUE;
		if ( !m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD) ) { ASSERT(0); return; }
		if ( (m_nElementType == HT_BEAM_COLM && !m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) || // beam
			(m_nElementType == HT_TRUSS && !m_pDoc->m_pAttrCtrl->IsTruss(ElemD.eltyp)) )  // truss
			continue;

		for ( int j = 0; j < aSelSectK.GetSize(); j++ )
			if ( ElemD.elpro == aSelSectK[j] ) { bAdd = FALSE; break; }

		if ( bAdd ) aSelSectK.Add(ElemD.elpro);
	}

	qsort(aSelSectK.GetData(), aSelSectK.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

	int nSectKeyIndex = 0;
	int nIndex = 0;
	T_SECT_D SectD;
	CString str;
	for ( int i = 0; i < aSelSectK.GetSize(); i++ )
	{
		SectD.Initialize();
		str.Empty();
		m_pDoc->m_pAttrCtrl->GetSect(aSelSectK[i], SectD);
		str.Format(_T("%d : %s"), aSelSectK[i], SectD.SName);
		nIndex = m_cmbSect.AddString(str);
		m_cmbSect.SetItemData(nIndex, aSelSectK[i]);

		if ( m_Data.SectKey == aSelSectK[i] ) nSectKeyIndex = nIndex;
	}

	if ( m_cmbSect.GetCount() != 0 || m_cmbSect.GetCount() != -1 )
		m_cmbSect.SetCurSel(0);

	if ( m_bModify )
	{
		T_SECT_K SectK = 0;
		if ( m_Data.nFiberAutoUser == 0 && m_bAssignHingeMode )
		{
			T_ELEM_D ElemD;
			m_pDoc->m_pAttrCtrl->GetElem(m_AssignedIehgKey.key.objK, ElemD);
			ASSERT(m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp));
			SectK = ElemD.elpro;
		}
		else
		{
			SectK = m_Data.SectKey;
		}
		CDlgUtil::CobxSetCurSelItemData(m_cmbSect, SectK);
	}

	OnSelchangeSectCmb();
}

void CIehpItemDlg_MEC::InitSectComboCtrl_Thik()
{
	GetDlgItem(IDC_SECT_STC)->SetWindowText(_LS(IDS_CMD_THICKNESS_thickness));

	m_cmbSect.ResetContent();
	CArray<T_THIK_K, T_THIK_K> aThikK;
	m_pDoc->m_pAttrCtrl->GetThikKeyList(aThikK);

	T_THIK_D ThikD;
	for ( int i = 0; i < aThikK.GetSize(); i++ )
	{
		CString str = m_pDoc->m_pAttrCtrl->GetThikNameAuto(aThikK[i]);
		CDlgUtil::CobxAddItem(m_cmbSect, str, aThikK[i]);
	}

	if ( m_bModify )
	{
		T_THIK_K ThikK = 0;
		if ( m_Data.nFiberAutoUser == 0 && m_bAssignHingeMode )
		{
			T_ELEM_D ElemD;
			m_pDoc->m_pAttrCtrl->GetElem(m_AssignedIehgKey.key.objK, ElemD);
			ASSERT(m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp));
			ThikK = ElemD.elpro;
		}
		else
		{
			ThikK = m_Data.SectKey;
		}
		CDlgUtil::CobxSetCurSelItemData(m_cmbSect, ThikK);
	}
	else
		m_cmbSect.SetCurSel(0);
}

void CIehpItemDlg_MEC::HingeComboCtrl()
{
	// Fx
	int nIndex = 0;
	if ( m_nInteractionType == 2 ) // P-M-M  // MQC-7682, by Hsshim
	{
		m_cmbHngLoc[D_DOF_FX].ResetContent();
		nIndex = m_cmbHngLoc[D_DOF_FX].AddString(_T("I"));    m_cmbHngLoc[D_DOF_FX].SetItemData(nIndex, 0);
		nIndex = m_cmbHngLoc[D_DOF_FX].AddString(_T("J"));    m_cmbHngLoc[D_DOF_FX].SetItemData(nIndex, 1);
		nIndex = m_cmbHngLoc[D_DOF_FX].AddString(_T("I&J"));    m_cmbHngLoc[D_DOF_FX].SetItemData(nIndex, 2);

		if ( m_Data.nHingeLocation[0] > 2 ) m_cmbHngLoc[D_DOF_FX].SetCurSel(0);
		else                             m_cmbHngLoc[D_DOF_FX].SetCurSel(m_Data.nHingeLocation[0]);
	}
	else
	{
		m_cmbHngLoc[D_DOF_FX].ResetContent();
		nIndex = m_cmbHngLoc[D_DOF_FX].AddString(_LS(IDS_CMD_IEHP_CENTER));    m_cmbHngLoc[D_DOF_FX].SetItemData(nIndex, 3);
		m_cmbHngLoc[D_DOF_FX].SetCurSel(0);
	}

	// Fy, Fz, Mx, My, Mz
	for ( int i = 1; i < 6; i++ )
	{
		m_cmbHngLoc[i].ResetContent();
		nIndex = m_cmbHngLoc[i].AddString(_T("I"));    m_cmbHngLoc[i].SetItemData(nIndex, 0);
		nIndex = m_cmbHngLoc[i].AddString(_T("J"));    m_cmbHngLoc[i].SetItemData(nIndex, 1);
		nIndex = m_cmbHngLoc[i].AddString(_T("I&J"));    m_cmbHngLoc[i].SetItemData(nIndex, 2);

		m_cmbHngLoc[i].SetCurSel(m_Data.nHingeLocation[i]);
	}

	for (int i = 0; i < 6; i++)
	{
		m_cmbHngStifLoc[i].ResetContent();
		nIndex = m_cmbHngStifLoc[i].AddString(_T("I"));         m_cmbHngStifLoc[i].SetItemData(nIndex, 0);
		nIndex = m_cmbHngStifLoc[i].AddString(_T("J"));         m_cmbHngStifLoc[i].SetItemData(nIndex, 1);
		nIndex = m_cmbHngStifLoc[i].AddString(_T("Center"));    m_cmbHngStifLoc[i].SetItemData(nIndex, 2);

		m_cmbHngStifLoc[i].SetCurSel(m_Data.nStifLoc[i]);
	}
}

void CIehpItemDlg_MEC::ModelComboCtrl()
{
	for (int i = 0; i < 6; i++)
	{
		m_cmbModel[i].SetRedraw(FALSE);
	}

	int nIndex = 0;
	for ( int i = 0; i < 6; i++ )
	{
		BOOL bDyna2EHysModelOnly = FALSE;
		BOOL bCivilHysModelOnly = FALSE;
		BOOL bCivilIsolatorOnly = FALSE;
		switch ( m_Data.nHysModel[i] )
		{
		case D_IEHP_NELS:
		case D_IEHP_ASYM:
		case D_IEHP_NELA:
			bDyna2EHysModelOnly = TRUE;
			break;
		case D_IEHP_CLOU:
		case D_IEHP_NBIL:
		case D_IEHP_EBIL:
		case D_IEHP_ETRI:
		case D_IEHP_ETET:
		case D_IEHP_TTET:
		case D_IEHP_MTTE:
		case D_IEHP_SRCT:
			bCivilHysModelOnly = TRUE;
			break;
		case D_IEHP_LRBB:
		case D_IEHP_LRBT:
		case D_IEHP_DMPR:
		case D_IEHP_LRBH16:
		case D_IEHP_LRBH30:
		case D_IEHP_DMPRH16:
		case D_IEHP_DMPRH30:
		case D_IEHP_RO:
		case D_IEHP_HD:
			bCivilIsolatorOnly = TRUE;
			break;
		}

		BOOL bNewIehpJP = TRUE;

		m_cmbModel[i].ResetContent();
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_KINEMA));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_KINE);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ORIGIN));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_ORIG);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_PEAK));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_PICK);

		if ( m_nInteractionType == 0 )
		{
			// Interaction Type : None
			if ( (m_nElementType != HT_SPRING && i == 0) || (m_nElementType == HT_SPRING && i <= 2) )
			{
				// Beam-Column, Truss or Wall : Fx, Material Type : RC/SRC(encased)
				// GL-LINK : Fx, Fy, Fz
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_AXIAL));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_AXIAL);
			}
		}
		if ( !m_bAddData4D2E )
		{
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_CLOUGH));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_CLOU);
		}
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_DEGRAD));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_DEGR);
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKEDA));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_TAKE);

		if ( m_nInteractionType == 0 || m_nInteractionType == 1 )
		{
			// Takeda Slip  : Interaction Type : None, P-M 인 경우만 선택 가능
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKS));     m_cmbModel[i].SetItemData(nIndex, D_IEHP_TAKS);
		}
		if ( !m_bAddData4D2E && m_nElementType != HT_WALL_CRB )
		{
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKTET));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_TTET);
			if (bNewIehpJP)
			{
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SRCTET));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_SRCT);
			}
		}
		nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MOD_TAKETA));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTAK);
		if ( !m_bAddData4D2E )
		{
			if ( m_nElementType != HT_WALL_CRB )
			{
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MTATET));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTTE);
			}

			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_NORBIL));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_NBIL);

			if ( m_nElementType != HT_WALL_CRB )
			{
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELABIL));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_EBIL);
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELATRI));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_ETRI);
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_ELATET));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_ETET);
			}
		}
		if ( m_bAddData4D2E )
		{
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_NONL_ELA_SYM));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_NELS);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_NONL_INV_DIR));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_ASYM);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_NONL_ELA_ASY));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_NELA);
		}
		if ( m_nElementType != HT_WALL_CRB )
		{
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLPB);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR_TENS));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLBT);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_BILINEAR_COMP));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLBC);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLPT);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR_TENS));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLTT);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_SLIP_TRILINEAR_COMP));    m_cmbModel[i].SetItemData(nIndex, D_IEHP_SLTC);
		}
#if defined(_CIVIL) || defined(_IGEN) 
		// MNET:4337-JWKWON-20110920 : Multi Linear type 추가
		if ( m_nNewHingeType == 0 && m_nInteractionType == 0 )
		{
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MULTI_ELASTIC));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_MLEL);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MULTI_PLASTIC_KINEMATIC));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_MLKI);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MULTI_PLASTIC_TAKEDA));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_MLTA);
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MULTI_PLASTIC_PIVOT));      m_cmbModel[i].SetItemData(nIndex, D_IEHP_MLPI);
		}
#endif
		// 면진 받침 (일본과 한국만 사용)
		//int nMovingType = CProduct::GetMovingType();    
		if ( m_nElementType == HT_SPRING && !m_bAddData4D2E )
		{
			// Civil JP만 되도록 변경 : by jbseon 05.04.12 : 강은경, 지규선
			//if(nMovingType == D_PRODUCT_MOVING_ORG || nMovingType == D_PRODUCT_MOVING_JP)
			BOOL bAllowLRB = FALSE;
#if defined(_CIVIL)
			// 20080104 mylee - Civil KOR 도 되도록 변경 : 김정인 (from 720)
			if ( CProduct::IsMovingType(D_PRODUCT_MOVING_JP) || CProduct::IsMovingType(D_PRODUCT_MOVING_ORG) ) bAllowLRB = TRUE;
#elif defined(_MGEN)
			// MNET:4167-SHJUNG-20100625 - Gen JP제외한 모든 국가에 적용.
			if ( !CProduct::IsMovingType(D_PRODUCT_MOVING_JP) ) bAllowLRB = TRUE;
#endif
			if ( bAllowLRB )
			{
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_LRBBIL));        m_cmbModel[i].SetItemData(nIndex, D_IEHP_LRBB);
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_LRBTRI));        m_cmbModel[i].SetItemData(nIndex, D_IEHP_LRBT);
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_DMPRBR));        m_cmbModel[i].SetItemData(nIndex, D_IEHP_DMPR);
				if ( i <= 2 ) // RO, HD는 Fx, Fy, Fz만 지원
				{
					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_PROP_RO));          m_cmbModel[i].SetItemData(nIndex, D_IEHP_RO);
					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_PROP_HD));          m_cmbModel[i].SetItemData(nIndex, D_IEHP_HD);
				}

				if (i > 0 && i <= 2 && bNewIehpJP) // LBR H16/H30, HDR H16/30 은 Fy, Fz만 지원
				{
					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_LRBH16));        m_cmbModel[i].SetItemData(nIndex, D_IEHP_LRBH16);
					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_LRBH30));        m_cmbModel[i].SetItemData(nIndex, D_IEHP_LRBH30);
					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_DMPRH16));        m_cmbModel[i].SetItemData(nIndex, D_IEHP_DMPRH16);
					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_DMPRH30));        m_cmbModel[i].SetItemData(nIndex, D_IEHP_DMPRH30);
				}
			}
		}

		// (Moment-Rotation && None/PM) 또는 Truss, General Link일 때 FEMA 입력
		if ( (m_nDefinitionType == MOMENT_ROTATION && (m_nInteractionType == 0 || m_nInteractionType == 1)) 
			|| m_nElementType == HT_TRUSS || m_nElementType == HT_SPRING || m_nElementType == HT_POINT_SPRING )
		{
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_FEMA_DEFORM_RATIO)); m_cmbModel[i].SetItemData(nIndex, D_IEHP_FBIL);
		}

		// Truss/G_LINK && Fx
		if ( i == 0 && (m_nElementType == HT_TRUSS || m_nElementType == HT_SPRING) )
		{
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_FEMA_INFILL_STRUT)); m_cmbModel[i].SetItemData(nIndex, D_IEHP_FINF);
		}

		if ( m_nInteractionType == 3 && (i == 4 || i == 5) ) // P-M ulti-Curve Type
		{
			// 이것만 포함됨.
			m_cmbModel[i].ResetContent();
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_PM_MULTI_CURVE)); m_cmbModel[i].SetItemData(nIndex, D_IEHP_PMMC);
		}

		if ( m_nElementType != HT_WALL_CRB && m_nInteractionType == 0 )
		{
			// Interaction Type : None
			if ( (m_nElementType != HT_SPRING && i == 0) || (m_nElementType == HT_SPRING && i <= 2) )
			{
				// Beam-Column, Truss : Fx, Material Type : Steel/SRC(filled), 
				// GL-LINK : Fx, Fy, Fz
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_BUCK));     m_cmbModel[i].SetItemData(nIndex, D_IEHP_BUCK);
			}
		}

		if ( m_nInteractionType == 3 && (i == 4 || i == 5) )
		{
			m_cmbModel[i].SetCurSel(0);
		}
		else if ( (bDyna2EHysModelOnly && !m_bAddData4D2E) || (bCivilHysModelOnly && m_bAddData4D2E) ||
			(bCivilIsolatorOnly && m_bAddData4D2E) || (bCivilIsolatorOnly && m_nElementType == HT_BEAM_COLM) )
		{
			m_cmbModel[i].SetCurSel(0);
			if ( m_Data.nHysModel[i] != 0 )
			{
				m_Data.AllProp.PROP[i].KINEMA.Initialize();
				m_Data.AllSubProp.PROP[i].KINEMA.Initialize();
			}
			OnSelchangeModelCmb(i);
		}
		else
		{
			int nSel = m_cmbModel[i].SelectString(0, GetHysModelName(m_Data.nHysModel[i]));
			if ( nSel == CB_ERR )
			{
				m_cmbModel[i].SetCurSel(0);
			}
		}
	}

	// Combo 내용을 MenuBtn에 만든다.
	for ( int i = 0; i < 6; i++ )
	{
		MakeBtnMenu(i);
		SetCurSelBtnMenu(i);
	}

	for (int i = 0; i < 6; i++)
	{
		m_cmbModel[i].SetRedraw(TRUE);
		m_cmbModel[i].Invalidate();
	}
}

CString CIehpItemDlg_MEC::GetHysModelName(int nHysModel)
{
	CString strModel = _T("");
	switch ( nHysModel )
	{
	case D_IEHP_KINE: strModel = _LS(IDS_CMD_IEHP_KINEMA);       break;
	case D_IEHP_ORIG: strModel = _LS(IDS_CMD_IEHP_ORIGIN);       break;
	case D_IEHP_PICK: strModel = _LS(IDS_CMD_IEHP_PEAK);         break;
	case D_IEHP_CLOU: strModel = _LS(IDS_CMD_IEHP_CLOUGH);       break;
	case D_IEHP_DEGR: strModel = _LS(IDS_CMD_IEHP_DEGRAD);       break;
	case D_IEHP_TAKE: strModel = _LS(IDS_CMD_IEHP_TAKEDA);       break;
	case D_IEHP_MTAK: strModel = _LS(IDS_CMD_IEHP_MOD_TAKETA);   break;
	case D_IEHP_NELS: strModel = _LS(IDS_CMD_IEHP_NONL_ELA_SYM); break;
	case D_IEHP_ASYM: strModel = _LS(IDS_CMD_IEHP_NONL_INV_DIR); break;
	case D_IEHP_NELA: strModel = _LS(IDS_CMD_IEHP_NONL_ELA_ASY); break;
	case D_IEHP_NBIL: strModel = _LS(IDS_CMD_IEHP_NORBIL);       break;
	case D_IEHP_EBIL: strModel = _LS(IDS_CMD_IEHP_ELABIL);       break;
	case D_IEHP_ETRI: strModel = _LS(IDS_CMD_IEHP_ELATRI);       break;
	case D_IEHP_ETET: strModel = _LS(IDS_CMD_IEHP_ELATET);       break;
	case D_IEHP_TTET: strModel = _LS(IDS_CMD_IEHP_TAKTET);       break;
	case D_IEHP_MTTE: strModel = _LS(IDS_CMD_IEHP_MTATET);       break;
	case D_IEHP_LRBB: strModel = _LS(IDS_CMD_IEHP_LRBBIL);       break;
	case D_IEHP_LRBT: strModel = _LS(IDS_CMD_IEHP_LRBTRI);       break;
	case D_IEHP_DMPR: strModel = _LS(IDS_CMD_IEHP_DMPRBR);       break;
	case D_IEHP_LRBH16: strModel = _LS(IDS_CMD_IEHP_LRBH16);       break;
	case D_IEHP_LRBH30: strModel = _LS(IDS_CMD_IEHP_LRBH30);       break;
	case D_IEHP_DMPRH16: strModel = _LS(IDS_CMD_IEHP_DMPRH16);       break;
	case D_IEHP_DMPRH30: strModel = _LS(IDS_CMD_IEHP_DMPRH30);       break;
	case D_IEHP_SLPB: strModel = _LS(IDS_CMD_IEHP_SLIP_BILINEAR);      break;
	case D_IEHP_SLBT: strModel = _LS(IDS_CMD_IEHP_SLIP_BILINEAR_TENS); break;
	case D_IEHP_SLBC: strModel = _LS(IDS_CMD_IEHP_SLIP_BILINEAR_COMP); break;
	case D_IEHP_SLPT: strModel = _LS(IDS_CMD_IEHP_SLIP_TRILINEAR);     break;
	case D_IEHP_SLTT: strModel = _LS(IDS_CMD_IEHP_SLIP_TRILINEAR_TENS); break;
	case D_IEHP_SLTC: strModel = _LS(IDS_CMD_IEHP_SLIP_TRILINEAR_COMP); break;
	case D_IEHP_RO:   strModel = _LS(IDS_CMD_IEHP_PROP_RO);      break;
	case D_IEHP_HD:   strModel = _LS(IDS_CMD_IEHP_PROP_HD);      break;

	case D_IEHP_MLEL: strModel = _LS(IDS_CMD_IEHP_MULTI_ELASTIC);          break;
	case D_IEHP_MLKI: strModel = _LS(IDS_CMD_IEHP_MULTI_PLASTIC_KINEMATIC); break;
	case D_IEHP_MLTA: strModel = _LS(IDS_CMD_IEHP_MULTI_PLASTIC_TAKEDA);   break;
	case D_IEHP_MLPI: strModel = _LS(IDS_CMD_IEHP_MULTI_PLASTIC_PIVOT);    break;

	case D_IEHP_TAKS: strModel = _LS(IDS_CMD_IEHP_TAKS);        break;
	case D_IEHP_AXIAL: strModel = _LS(IDS_CMD_IEHP_AXIAL);       break;
	case D_IEHP_BUCK: strModel = _LS(IDS_CMD_IEHP_BUCK);        break;
	case D_IEHP_FBIL: strModel = _LS(IDS_CMD_IEHP_FEMA_DEFORM_RATIO); break;
	case D_IEHP_FINF: strModel = _LS(IDS_CMD_IEHP_FEMA_INFILL_STRUT); break;
	case D_IEHP_PMMC: strModel = _LS(IDS_CMD_IEHP_PM_MULTI_CURVE); break;
	case D_IEHP_SRCT: strModel = _LS(IDS_CMD_IEHP_SRCTET);       break;
	case D_IEHP_FTRI: ASSERT(0);/*strModel = _LS();        */break;

	default: ASSERT(0); break;
	}

	return strModel;
}

int CIehpItemDlg_MEC::GetCategory(int nHysModel)
{
	enum {
		D_IEHP_CAT_SIMP = 0,
		D_IEHP_CAT_DEGR,
		D_IEHP_CAT_NONL,
		D_IEHP_CAT_SLIP,
		D_IEHP_CAT_SEIS,
		D_IEHP_CAT_SPEC,
		D_IEHP_CAT_ALL,
		D_IEHP_CAT_MLIN,
		D_IEHP_CAT_STLM,
		D_IEHP_CAT_FEMA,
		D_IEHP_CAT_PMMC,
	};

	switch ( nHysModel )
	{
	case D_IEHP_KINE: return D_IEHP_CAT_SIMP;
	case D_IEHP_ORIG: return D_IEHP_CAT_SIMP;
	case D_IEHP_PICK: return D_IEHP_CAT_SIMP;
	case D_IEHP_CLOU: return D_IEHP_CAT_DEGR;
	case D_IEHP_DEGR: return D_IEHP_CAT_DEGR;
	case D_IEHP_TAKE: return D_IEHP_CAT_DEGR;
	case D_IEHP_MTAK: return D_IEHP_CAT_DEGR;
	case D_IEHP_NELS: return D_IEHP_CAT_NONL;
	case D_IEHP_ASYM: return D_IEHP_CAT_NONL;
	case D_IEHP_NELA: return D_IEHP_CAT_NONL;
	case D_IEHP_NBIL: return D_IEHP_CAT_SIMP;
	case D_IEHP_EBIL: return D_IEHP_CAT_NONL;
	case D_IEHP_ETRI: return D_IEHP_CAT_NONL;
	case D_IEHP_ETET: return D_IEHP_CAT_NONL;
	case D_IEHP_TTET: return D_IEHP_CAT_DEGR;
	case D_IEHP_MTTE: return D_IEHP_CAT_DEGR;
	case D_IEHP_LRBB: return D_IEHP_CAT_SEIS;
	case D_IEHP_LRBT: return D_IEHP_CAT_SEIS;
	case D_IEHP_DMPR: return D_IEHP_CAT_SEIS;
	case D_IEHP_LRBH16: return D_IEHP_CAT_SEIS;
	case D_IEHP_LRBH30: return D_IEHP_CAT_SEIS;
	case D_IEHP_DMPRH16: return D_IEHP_CAT_SEIS;
	case D_IEHP_DMPRH30: return D_IEHP_CAT_SEIS;
	case D_IEHP_SLPB: return D_IEHP_CAT_SLIP;
	case D_IEHP_SLBT: return D_IEHP_CAT_SLIP;
	case D_IEHP_SLBC: return D_IEHP_CAT_SLIP;
	case D_IEHP_SLPT: return D_IEHP_CAT_SLIP;
	case D_IEHP_SLTT: return D_IEHP_CAT_SLIP;
	case D_IEHP_SLTC: return D_IEHP_CAT_SLIP;
	case D_IEHP_RO:   return D_IEHP_CAT_SPEC;
	case D_IEHP_HD:   return D_IEHP_CAT_SPEC;
	case D_IEHP_MLEL: return D_IEHP_CAT_MLIN;
	case D_IEHP_MLKI: return D_IEHP_CAT_MLIN;
	case D_IEHP_MLTA: return D_IEHP_CAT_MLIN;
	case D_IEHP_MLPI: return D_IEHP_CAT_MLIN;
	case D_IEHP_TAKS: return D_IEHP_CAT_DEGR;
	case D_IEHP_AXIAL: return D_IEHP_CAT_DEGR;
	case D_IEHP_BUCK: return D_IEHP_CAT_STLM;
	case D_IEHP_FBIL: return D_IEHP_CAT_FEMA;
	case D_IEHP_FINF: return D_IEHP_CAT_FEMA;
	case D_IEHP_FTRI: return D_IEHP_CAT_FEMA;
	case D_IEHP_PMMC: return D_IEHP_CAT_PMMC;
	case D_IEHP_SRCT: return D_IEHP_CAT_DEGR;
	default: ASSERT(0); break;
	}
	ASSERT(0);
	return 0;
}

void CIehpItemDlg_MEC::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_strDesc = m_Data.strDescription;
	m_nMatlType = m_Data.nMaterialType;

	m_nAutoUserLocMemb = m_Data.nUseIehcLocation;
	T_IEHC_D IehcD;
	if ( !CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetIehc(IehcD) ) IehcD.Initialize();
	if ( m_nAutoUserLocMemb == 0 ) m_nLocMemb = IehcD.nBeamLoc;
	else                      m_nLocMemb = m_Data.nLocation;

	m_nDefinitionType = 0;
	switch ( m_Data.nHingeType )
	{
	case D_IEHP_HTYP_BEAM_LUMP:
	case D_IEHP_HTYP_BEAM_DIST:
		{
			m_nElementType = HT_BEAM_COLM; // beam
			m_nDefinitionType = m_Data.nHingeType; // lumped, distribution
		}
		break;
	case D_IEHP_HTYP_SPRING:
		{
			m_nElementType = HT_SPRING;   // spring
		}
		break;
	case D_IEHP_HTYP_TRUSS:
		{
			m_nElementType = HT_TRUSS;    // truss
		}
		break;
	case D_IEHP_HTYP_WALL:
		{
			m_nElementType = HT_WALL_CRB; // Wall CRB
		}
		break;
	case D_IEHP_HTYP_POINTSPRING:
		{
			m_nElementType = HT_POINT_SPRING; // point spring support
		}
		break;
	default: ASSERT(0); break;
	}

	m_nNewHingeType = m_Data.nDefinition;
	m_nInteractionType = m_Data.nInteractionType;
	m_bAddData4D2E = m_Data.bAddData4D2E;
	m_nWallType = m_Data.nWallType;
	m_bExistIJDataYield = m_Data.bIJExistYield;

	for ( int i = 0; i < 7; i++ )
		m_bExistIJPropDlg[i] = m_Data.bExistIJProp[i];

	for ( int i = 0; i < 6; i++ )
	{
		if ( m_Data.cDir[i] == '1' ) m_bDof[i] = TRUE;
		else
		{
			m_bDof[i] = FALSE;
			if ( m_Data.nHysModel[i] == 0 )
			{
				m_Data.AllProp.PROP[i].KINEMA.Initialize();
				m_Data.AllSubProp.PROP[i].KINEMA.Initialize();
			}
		}
		m_strSec[i].Format(_T("%d"), m_Data.nSectionNum[i]);
		m_nMdlCombSel[i] = m_Data.nHysModel[i];
	}

	m_nFlexStiff = m_Data.nFlexSitf;
	m_bUseNonlinearShearY = m_Data.bUseNonlinearShearY;
	m_bUseNonlinearShearZ = m_Data.bUseNonlinearShearZ;

	m_nFiberAutoUser = m_Data.nFiberAutoUser;
	m_nAutoUserFiberOutpl = m_Data.nUseIehcConsOutpl;
	m_bConsOutpl = m_Data.bConsOutpl;
	m_nAutoUserShearR = m_Data.nUseIehcShearR;
	if ( m_Data.nUseIehcShearR == 0 ) m_edtShearR.SetEditUnit(IehcD.dR);
	else                         m_edtShearR.SetEditUnit(m_Data.dShearR);

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::Dlg2CommonData()
{
	UpdateData();
	m_Data.nMaterialType = m_nMatlType;
	//m_Data.MatlKey = m_cmbMatl.GetItemData(m_cmbMatl.GetCurSel());
	if ( m_nNewHingeType == 1 )
	{
		m_Data.SectKey = m_cmbSect.GetItemData(m_cmbSect.GetCurSel());
	}
	m_Data.nWallType = m_nWallType;
	m_Data.nDefinition = m_nNewHingeType;
	m_Data.nLocation = m_nLocMemb;
	m_Data.bIJExistYield = m_bExistIJDataYield;

	m_Data.nFiberAutoUser = m_nFiberAutoUser;
	m_Data.nUseIehcLocation = m_nAutoUserLocMemb;
	m_Data.nUseIehcShearR = m_nAutoUserShearR;
	m_Data.dShearR = m_edtShearR.GetEditValue();
	m_Data.nUseIehcConsOutpl = m_nAutoUserFiberOutpl;
	m_Data.bConsOutpl = m_bConsOutpl;

	m_Data.nFlexSitf = m_nFlexStiff;
	if (m_nFlexStiff == 1)
	{
		m_Data.bUseNonlinearShearY = m_bUseNonlinearShearY;
		m_Data.bUseNonlinearShearZ = m_bUseNonlinearShearZ;
	}

	for ( int i = 0; i < 7; i++ )
		m_Data.bExistIJProp[i] = m_bExistIJPropDlg[i];

	switch ( m_nElementType )
	{
	case HT_BEAM_COLM:
		{
			m_Data.nHingeType = m_nDefinitionType;
		}
		break;
	case HT_SPRING:
		{
			m_Data.nHingeType = D_IEHP_HTYP_SPRING; // spring
		}
		break;
	case HT_TRUSS:
		{
			m_Data.nHingeType = D_IEHP_HTYP_TRUSS; // truss
		}
		break;
	case HT_WALL_CRB:
		{
			m_Data.nHingeType = D_IEHP_HTYP_WALL; // wall
		}
		break;
	case HT_POINT_SPRING:
		{
			m_Data.nHingeType = D_IEHP_HTYP_POINTSPRING; // point spring support
		}
		break;
	default: ASSERT(0); break;
	}

	m_Data.nInteractionType = m_nInteractionType;
	if ( m_Data.nDefinition == 1 && m_Data.nFiberAutoUser == 1 )
	{
		m_Data.FibrKey = m_cmbFibrName.GetItemData(m_cmbFibrName.GetCurSel());
	}
	else
	{
		m_Data.FibrKey = 0;
	}
	m_Data.nCodeStl = 0;
	m_Data.nCodeConc = 0;

	m_Data.bAddData4D2E = m_bAddData4D2E;
	// end 650

	for ( int i = 0; i < 6; i++ )
	{
		int nHysModel = m_cmbModel[i].GetItemData(m_cmbModel[i].GetCurSel());
		if ( m_bDof[i] )
		{
			// MQC 4836-48. by Hsshim. Auto 계산할 수 없는 경우에 비활성화.
			{
				BOOL bDisableAutoCalc = FALSE;    // Auto 계산을 할 수 없는 경우
				if ( m_Data.nHingeType == 2 ) bDisableAutoCalc = TRUE;    // nHingeType;  // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB)
				if ( m_Data.nInteractionType != 0 )                       // nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
				{
					int nDof = i;
					if ( nDof > 5 ) nDof = i - 3;
					if ( nDof == 0 || nDof == 4 || nDof == 5 ) bDisableAutoCalc = TRUE;
				}

				// nYieldStrengthOpt를 가진 데이터에 대해서 Auto -> User로 변경.
				if ( bDisableAutoCalc )
				{
					// nYieldStrengthOpt // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
					if ( nHysModel == D_IEHP_KINE || nHysModel == D_IEHP_NELS )
					{
						m_Data.AllProp.PROP[i].KINEMA.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].KINEMA.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_ORIG )
					{
						m_Data.AllProp.PROP[i].ORIGIN.nYieldStrengthOpt = 0;  // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
						m_Data.AllSubProp.PROP[i].ORIGIN.nYieldStrengthOpt = 0;  // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
					}
					else if ( nHysModel == D_IEHP_PICK )
					{
						m_Data.AllProp.PROP[i].PEAK.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].PEAK.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_CLOU )
					{
						m_Data.AllProp.PROP[i].CLOUGH.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].CLOUGH.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_DEGR )
					{
						m_Data.AllProp.PROP[i].DEGRAD.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].DEGRAD.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_TAKE || nHysModel == D_IEHP_TAKS || nHysModel == D_IEHP_MTAK || nHysModel == D_IEHP_ASYM || nHysModel == D_IEHP_NELA )
					{
						m_Data.AllProp.PROP[i].TAKEDA.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].TAKEDA.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_NBIL )
					{
						m_Data.AllProp.PROP[i].NORBIL.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].NORBIL.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_EBIL )
					{
						m_Data.AllProp.PROP[i].ELABIL.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].ELABIL.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_ETRI )
					{
						m_Data.AllProp.PROP[i].ELATRI.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].ELATRI.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_ETET )
					{
						m_Data.AllProp.PROP[i].ELATET.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].ELATET.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_TTET || nHysModel == D_IEHP_MTTE )
					{
						m_Data.AllProp.PROP[i].TAKTET.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].TAKTET.nYieldStrengthOpt = 0;
					}
					else if (nHysModel == D_IEHP_SRCT)
					{
						m_Data.AllProp.PROP[i].SRCTET.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].SRCTET.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_SLPB || nHysModel == D_IEHP_SLBT || nHysModel == D_IEHP_SLBC
						|| nHysModel == D_IEHP_SLPT || nHysModel == D_IEHP_SLTT || nHysModel == D_IEHP_SLTC )
					{
						m_Data.AllProp.PROP[i].SLIP.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].SLIP.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_AXIAL )
					{
						m_Data.AllProp.PROP[i].AXIAL.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].AXIAL.nYieldStrengthOpt = 0;
					}
					else if ( nHysModel == D_IEHP_BUCK )
					{
						m_Data.AllProp.PROP[i].BUCKL.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].BUCKL.nYieldStrengthOpt = 0;
					}
					else if ( m_pDoc->IsTHHysModelFEMA(nHysModel) )
					{
						m_Data.AllProp.PROP[i].FEMA.nYieldStrengthOpt = 0;
						m_Data.AllSubProp.PROP[i].FEMA.nYieldStrengthOpt = 0;
					}
				}
			}
		}
	}
}

BOOL CIehpItemDlg_MEC::Dlg2Data()
{
	UpdateData();
	m_edtName.GetWindowText(m_Data.strName);
	m_Data.strDescription = m_strDesc;

	Dlg2CommonData();

	for ( int i = 0; i < 6; i++ )
	{
		m_Data.nSectionNum[i] = _ttoi(m_strSec[i]);
		m_Data.nHingeLocation[i] = m_cmbHngLoc[i].GetItemData(m_cmbHngLoc[i].GetCurSel());
		m_Data.nHysModel[i] = m_cmbModel[i].GetItemData(m_cmbModel[i].GetCurSel());
		m_Data.nStifLoc[i] = m_cmbHngStifLoc[i].GetItemData(m_cmbHngStifLoc[i].GetCurSel());
		if ( m_bDof[i] )
		{
			m_Data.cDir[i] = '1';
			if ( !m_bPropMod[i] )
			{
				if ( m_Data.nHysModel[i] == D_IEHP_KINE || m_Data.nHysModel[i] == D_IEHP_NELS )
				{
					m_Data.AllProp.PROP[i].KINEMA.Initialize();
					m_Data.AllSubProp.PROP[i].KINEMA.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_ORIG )
				{
					m_Data.AllProp.PROP[i].ORIGIN.Initialize();
					m_Data.AllSubProp.PROP[i].ORIGIN.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_PICK )
				{
					m_Data.AllProp.PROP[i].PEAK.Initialize();
					m_Data.AllSubProp.PROP[i].PEAK.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_CLOU )
				{
					m_Data.AllProp.PROP[i].CLOUGH.Initialize();
					m_Data.AllSubProp.PROP[i].CLOUGH.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_DEGR )
				{
					m_Data.AllProp.PROP[i].DEGRAD.Initialize();
					m_Data.AllSubProp.PROP[i].DEGRAD.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_TAKE || m_Data.nHysModel[i] == D_IEHP_TAKS || m_Data.nHysModel[i] == D_IEHP_MTAK || m_Data.nHysModel[i] == D_IEHP_ASYM || m_Data.nHysModel[i] == D_IEHP_NELA )
				{
					m_Data.AllProp.PROP[i].TAKEDA.Initialize();
					m_Data.AllSubProp.PROP[i].TAKEDA.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_NBIL )
				{
					m_Data.AllProp.PROP[i].NORBIL.Initialize();
					m_Data.AllSubProp.PROP[i].NORBIL.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_EBIL )
				{
					m_Data.AllProp.PROP[i].ELABIL.Initialize();
					m_Data.AllSubProp.PROP[i].ELABIL.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_ETRI )
				{
					m_Data.AllProp.PROP[i].ELATRI.Initialize();
					m_Data.AllSubProp.PROP[i].ELATRI.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_ETET )
				{
					m_Data.AllProp.PROP[i].ELATET.Initialize();
					m_Data.AllSubProp.PROP[i].ELATET.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_TTET || m_Data.nHysModel[i] == D_IEHP_MTTE )
				{
					m_Data.AllProp.PROP[i].TAKTET.Initialize();
					m_Data.AllSubProp.PROP[i].TAKTET.Initialize();
				}
				else if (m_Data.nHysModel[i] == D_IEHP_SRCT)
				{
					m_Data.AllProp.PROP[i].SRCTET.Initialize();
					m_Data.AllSubProp.PROP[i].SRCTET.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_LRBB )
				{
					m_Data.AllProp.PROP[i].LRBBIL.Initialize();
					m_Data.AllSubProp.PROP[i].LRBBIL.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_LRBT )
				{
					m_Data.AllProp.PROP[i].LRBTRI.Initialize();
					m_Data.AllSubProp.PROP[i].LRBTRI.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_DMPR )
				{
					m_Data.AllProp.PROP[i].DMPRBR.Initialize();
					m_Data.AllSubProp.PROP[i].DMPRBR.Initialize();
				}
				else if (m_Data.nHysModel[i] == D_IEHP_LRBH16)
				{
					m_Data.AllProp.PROP[i].LRBH16.Initialize();
					m_Data.AllSubProp.PROP[i].LRBH16.Initialize();
				}
				else if (m_Data.nHysModel[i] == D_IEHP_LRBH30)
				{
					m_Data.AllProp.PROP[i].LRBH30.Initialize();
					m_Data.AllSubProp.PROP[i].LRBH30.Initialize();
				}
				else if (m_Data.nHysModel[i] == D_IEHP_DMPRH16)
				{
					m_Data.AllProp.PROP[i].DMPRH16.Initialize();
					m_Data.AllSubProp.PROP[i].DMPRH16.Initialize();
				}
				else if (m_Data.nHysModel[i] == D_IEHP_DMPRH30)
				{
					m_Data.AllProp.PROP[i].DMPRH30.Initialize();
					m_Data.AllSubProp.PROP[i].DMPRH30.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_SLPB || m_Data.nHysModel[i] == D_IEHP_SLBT || m_Data.nHysModel[i] == D_IEHP_SLBC
					|| m_Data.nHysModel[i] == D_IEHP_SLPT || m_Data.nHysModel[i] == D_IEHP_SLTT || m_Data.nHysModel[i] == D_IEHP_SLTC )
				{
					m_Data.AllProp.PROP[i].SLIP.Initialize();
					m_Data.AllSubProp.PROP[i].SLIP.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_RO || m_Data.nHysModel[i] == D_IEHP_HD )
				{
					m_Data.AllProp.PROP[i].ROHD.Initialize();
					m_Data.AllSubProp.PROP[i].ROHD.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_AXIAL )
				{
					m_Data.AllProp.PROP[i].AXIAL.Initialize();
					m_Data.AllSubProp.PROP[i].AXIAL.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_BUCK )
				{
					m_Data.AllProp.PROP[i].BUCKL.Initialize();
					m_Data.AllSubProp.PROP[i].BUCKL.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_FBIL || m_Data.nHysModel[i] == D_IEHP_FTRI )
				{
					m_Data.AllProp.PROP[i].FEMA.Initialize();
					m_Data.AllSubProp.PROP[i].FEMA.Initialize();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_FINF )
				{
					m_Data.AllProp.PROP[i].FEMA.InitializeRatio_Infill();
					m_Data.AllSubProp.PROP[i].FEMA.InitializeRatio_Infill();
				}
				else if ( m_Data.nHysModel[i] == D_IEHP_PMMC )
				{
					m_Data.AllProp.PMMULT[i].Initialize();
					m_Data.AllSubProp.PMMULT[i].Initialize();
				}
			}
		}
		else
		{
			m_Data.cDir[i] = '0';
			m_Data.nStifLoc[i] = 0;
		}
	}
	// Yield
	if ( !m_bPropMod[6] )
	{
		BOOL bInitYLDSUR = TRUE;

		// User Input 인 경우는 초기화에서 제외.  // MNET:XXXX-HSSHIM-20100626
		// 초기화 시키면 사용자 입력값이 날아가 버린다. 
		if ( m_Data.AllProp.PROP[6].YLDSUR.nCurveUserAuto == 0 )
			bInitYLDSUR = FALSE;

		if ( bInitYLDSUR )
		{
			m_Data.AllProp.PROP[6].YLDSUR.Initialize();
			m_Data.AllSubProp.PROP[6].YLDSUR.Initialize();
		}
	}

	if ( m_nElementType == HT_BEAM_COLM ) m_Data.cDir[3] = '0';
	if ( m_nElementType == HT_TRUSS )
	{
		for ( int i = 1; i < 6; i++ ) m_Data.cDir[i] = '0';
	}

	m_Data.nUseIehcShearR = m_nAutoUserShearR;
	m_Data.dShearR = m_edtShearR.GetEditValue();
	m_Data.nUseIehcConsOutpl = m_nAutoUserFiberOutpl;
	m_Data.bConsOutpl = m_bConsOutpl;
	m_Data.nUseIehcLocation = m_nAutoUserLocMemb;

	BOOL bSuc = TRUE;
	if ( m_bModify )
	{
		T_IEHP_D data;
		m_pDoc->m_pAttrCtrl->GetIehp(m_OldKey, data);
		m_Data.nSeq = data.nSeq;   // old data의 seq를 현재 data의 seq로 넣어준다.
		bSuc = m_pDoc->m_pDataCtrl->ModifyIehp(m_csOldName, m_Data);
	}
	else bSuc = m_pDoc->m_pDataCtrl->AddIehp(m_Data);

	if ( bSuc ) m_OldKey = m_Key;

	return bSuc;
}

/////////////////////////////////////////////////////////////////////////////
// CIehpItemDlg_MEC Message handlers
BOOL CIehpItemDlg_MEC::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	InitCtrl();

	GetDlgItem(IDC_IEHP_ADD_DYNA2E_CHK)->ShowWindow(FALSE);
	if ( !m_bAssignHingeMode ) { CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge, TRUE); }

	// Civil은 벽체가 없기 때문에 wall 컨트롤들 비활성화(141216)
#if defined(_CIVIL)
	GetDlgItem(IDC_HING_TYPE_WALLCRB_RDO)->EnableWindow(FALSE);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO2)->EnableWindow(FALSE);
#endif

	// 2004.11.29 박종배 부장님 요청 : 일본에서 고정하중(Gen)과 사하중(Civil)의 구별
#if defined(_MGEN)
	GetDlgItem(IDC_INTR_PM_RDO)->SetWindowText(_LS(IDS_CMD_IEHP_P_M_GEN));
#elif defined(_CIVIL)
	GetDlgItem(IDC_INTR_PM_RDO)->SetWindowText(_LS(IDS_CMD_IEHP_P_M_CIVIL));
#endif  

	AlignCtrl();
	Data2Dlg();

	// IDC_IEHP_ADD_DYNA2E_CHK를 안 쓰는 거 같은데, OnAddDyna2E()를 조건에 따라 호출하도록 변경함.
	if ( GetDlgItem(IDC_IEHP_ADD_DYNA2E_CHK)->IsWindowVisible() )  OnAddDyna2E();

	CtrlTypeManager();
	MakeFibrCombo(TRUE);

	if ( m_bAssignHingeMode ) { CDlgUtil::CtrlEnableDisable(this, m_aAssignHinge, FALSE); }

#if defined(_CIVIL)
	GetDlgItem(IDC_HING_TYPE_WALLCRB_RDO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_WALL_TYPE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO2)->ShowWindow(SW_HIDE);
#endif

	InitCtrlTexts();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CIehpItemDlg_MEC::CtrlManager()
{
	UpdateData(TRUE);

	EnableDisableCtrls();
	CtrlInteractionType();
	MakeFibrCombo(FALSE);

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnCmdElementType()
{
	CtrlManager();
}

void CIehpItemDlg_MEC::OnCmdDefinitionType()
{
	CtrlManager();
}

void CIehpItemDlg_MEC::OnCmdNewHingeType()
{
	CtrlManager();
}

void CIehpItemDlg_MEC::OnFlexStifType()
{
	UpdateData(TRUE);

	if (m_nFlexStiff == 0)
	{
		for (int i = 0; i < 6; ++i)
		{
			m_cmbHngStifLoc[i].SetCurSel(0);
		}
	}
	else
	{
		for (int i = 0; i < 6; ++i)
		{
			m_strSec[i] = _T("1");
		}
	}

	UpdateData(FALSE);
	OnCmdDefinitionType();
}

void CIehpItemDlg_MEC::OnInteractionType()
{
	UpdateData(TRUE);
	if ( m_nInteractionType == 1 )
	{
		if ( m_bDof[D_DOF_MY] == FALSE )
		{
			m_bDof[D_DOF_MY] = TRUE;
			UpdateData(FALSE);
		}
	}
	else if ( m_nNewHingeType == 0 && m_nInteractionType == 2 )
	{
		if ( m_bDof[D_DOF_FX] == FALSE )
		{
			m_bDof[D_DOF_FX] = TRUE;
			UpdateData(FALSE);
		}
	}
	else if ( m_nInteractionType == 3 )
	{
		BOOL bChek = FALSE;
		if ( m_bDof[D_DOF_MY] == FALSE )
		{
			m_bDof[D_DOF_MY] = TRUE;
			bChek = TRUE;
		}
		if ( m_bDof[D_DOF_MZ] == FALSE )
		{
			m_bDof[D_DOF_MZ] = TRUE;
			bChek = TRUE;
		}
		if ( bChek ) UpdateData(FALSE);
	}
	CtrlManager();
}

void CIehpItemDlg_MEC::OnMaterialType()
{
	CtrlManager();
}

void CIehpItemDlg_MEC::OnCmdWallType()
{
	UpdateData(TRUE);
	CtrlManager();
}

void CIehpItemDlg_MEC::CtrlElementType()
{
	EnableDisableCtrls();
}

void CIehpItemDlg_MEC::CtrlNewHingeType()
{
	EnableDisableCtrls();
}

void CIehpItemDlg_MEC::CtrlInteractionType()
{
	HingeComboCtrl();  // Make Hinge combo
	ModelComboCtrl();  // Make Hysteresis Model

	if ( m_nNewHingeType == 0 || m_nElementType != HT_BEAM_COLM ) // Fiber Beam은 이력모델 Kinematic Hardening만 사용
	{
		//ModelComboCtrl();
		for ( int i = 0; i < 6; i++ )
		{
			BOOL bDyna2EHysModelOnly = FALSE;
			if ( m_Data.nHysModel[i] == 7 )  bDyna2EHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 8 )  bDyna2EHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 9 )  bDyna2EHysModelOnly = TRUE;

			BOOL bCivilHysModelOnly = FALSE;
			if ( m_Data.nHysModel[i] == 3 )  bCivilHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 10 )  bCivilHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 11 )  bCivilHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 12 )  bCivilHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 13 )  bCivilHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 14 )  bCivilHysModelOnly = TRUE;
			if ( m_Data.nHysModel[i] == 15 )  bCivilHysModelOnly = TRUE;

			BOOL bCivilIsolatorOnly = FALSE;
			if ( m_Data.nHysModel[i] == 51 )  bCivilIsolatorOnly = TRUE;
			if ( m_Data.nHysModel[i] == 52 )  bCivilIsolatorOnly = TRUE;
			if ( m_Data.nHysModel[i] == 53 )  bCivilIsolatorOnly = TRUE;

			if ( m_nInteractionType == 3 && (i == 4 || i == 5) )
			{
				m_cmbModel[i].SetCurSel(0);
			}
			else if ( (bDyna2EHysModelOnly && !m_bAddData4D2E) || (bCivilHysModelOnly && m_bAddData4D2E) ||
				(bCivilIsolatorOnly && m_bAddData4D2E) || (bCivilIsolatorOnly && m_nElementType == HT_BEAM_COLM) )
			{
				m_cmbModel[i].SetCurSel(0);
				//if(m_Data.nHysModel[i]!=0) m_Data.PROP[i].KINEMA.Initialize();
				// MQC:3442-Seungjun-20100621
				if ( m_Data.nHysModel[i] != 0 )       m_bPropMod[i] = FALSE;
			}
			else
			{
				int nSel = m_cmbModel[i].SelectString(0, GetHysModelName(m_Data.nHysModel[i]));
			}
			DWORD nItemType;
			CDlgUtil::CobxGetItemDataByCurSel(m_cmbModel[i], nItemType);
			m_btnModel[i].SetCurItem(nItemType);
		}

		if ( m_nInteractionType == 2 ) // P-M-M  // MQC-7682, byHsshim
		{
			int nIndex;
			BOOL bNewIehpJP = TRUE;
			for ( int i = 0; i < 6; i++ )
			{
				if ( i == 1 || i == 2 ) continue;   // Fy, Fz는 PMM인 경우에도 None처럼 작동(2004.12.29 지규선 요청)

				m_cmbModel[i].ResetContent();
				nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_KINEMA));
				m_cmbModel[i].SetItemData(nIndex, D_IEHP_KINE);

				if (m_nMatlType == RC_SRC_ENCASED && bNewIehpJP)
				{
					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKEDA));
					m_cmbModel[i].SetItemData(nIndex, D_IEHP_TAKE);

					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_TAKTET));
					m_cmbModel[i].SetItemData(nIndex, D_IEHP_TTET);

					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MOD_TAKETA));
					m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTAK);

					nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_MTATET));
					m_cmbModel[i].SetItemData(nIndex, D_IEHP_MTTE);
				}

				if (m_Data.nHysModel[i] == D_IEHP_KINE)
				{
					nIndex = m_cmbModel[i].SelectString(0, _LS(IDS_CMD_IEHP_KINEMA));
					m_cmbModel[i].SetCurSel(nIndex);
				}
				else if (m_Data.nHysModel[i] == D_IEHP_TAKE && m_nMatlType == RC_SRC_ENCASED && bNewIehpJP)
				{
					nIndex = m_cmbModel[i].SelectString(0, _LS(IDS_CMD_IEHP_TAKEDA));
					m_cmbModel[i].SetCurSel(nIndex);
				}
				else if (m_Data.nHysModel[i] == D_IEHP_TTET && m_nMatlType == RC_SRC_ENCASED && bNewIehpJP)
				{
					nIndex = m_cmbModel[i].SelectString(0, _LS(IDS_CMD_IEHP_TAKTET));
					m_cmbModel[i].SetCurSel(nIndex);
				}
				else if (m_Data.nHysModel[i] == D_IEHP_MTAK && m_nMatlType == RC_SRC_ENCASED && bNewIehpJP)
				{
					nIndex = m_cmbModel[i].SelectString(0, _LS(IDS_CMD_IEHP_MOD_TAKETA));
					m_cmbModel[i].SetCurSel(nIndex);
				}
				else if (m_Data.nHysModel[i] == D_IEHP_MTTE && m_nMatlType == RC_SRC_ENCASED && bNewIehpJP)
				{
					nIndex = m_cmbModel[i].SelectString(0, _LS(IDS_CMD_IEHP_MTATET));
					m_cmbModel[i].SetCurSel(nIndex);
				}
				else
				{
					m_cmbModel[i].SetCurSel(0);
					m_bPropMod[i] = FALSE;
				}

				MakeBtnMenu(i);
				SetCurSelBtnMenu(i);
			}
			ChangeNumofSectionDx();
			int nHngCmbSel = m_cmbHngLoc[D_DOF_FX].GetCurSel();
			if ( nHngCmbSel < 0 ) ASSERT(0);
			m_cmbHngLoc[D_DOF_FX].SetCurSel(nHngCmbSel); // Fx
			m_cmbHngLoc[D_DOF_MY].SetCurSel(nHngCmbSel); // Ry
			m_cmbHngLoc[D_DOF_MZ].SetCurSel(nHngCmbSel); // Rz    

			nHngCmbSel = m_cmbHngStifLoc[D_DOF_FX].GetCurSel();
			if (nHngCmbSel < 0) ASSERT(0);
			m_cmbHngStifLoc[D_DOF_FX].SetCurSel(nHngCmbSel); // Fx
			m_cmbHngStifLoc[D_DOF_MY].SetCurSel(nHngCmbSel); // Ry
			m_cmbHngStifLoc[D_DOF_MZ].SetCurSel(nHngCmbSel); // Rz    
		}
	}
	else if ( m_nNewHingeType == 1 ) // Fiber
	{
		int nIndex;
		for ( int i = 0; i < 6; i++ )
		{
			m_cmbModel[i].ResetContent();
			nIndex = m_cmbModel[i].AddString(_LS(IDS_CMD_IEHP_KINEMA));
			m_cmbModel[i].SetItemData(nIndex, D_IEHP_KINE);
			m_cmbModel[i].SetCurSel(0);
			//			if(m_Data.nHysModel[i]!=0) m_Data.PROP[i].KINEMA.Initialize();
			// MQC:3442-Seungjun-20100621
			if ( m_Data.nHysModel[i] != 0 )       m_bPropMod[i] = FALSE;

			MakeBtnMenu(i);
			SetCurSelBtnMenu(i);
		}
		ChangeNumofSectionDx();

		int nHngCmbSel = m_cmbHngStifLoc[D_DOF_FX].GetCurSel();
		if (nHngCmbSel < 0) ASSERT(0);
		m_cmbHngStifLoc[D_DOF_FX].SetCurSel(nHngCmbSel); // Fx
		m_cmbHngStifLoc[D_DOF_MY].SetCurSel(nHngCmbSel); // Ry
		m_cmbHngStifLoc[D_DOF_MZ].SetCurSel(nHngCmbSel); // Rz    
	}

	//EnableDisableCtrls_Comp();
	//EnableDisableCtrls_Interact();
	//EnableDisableCtrls_YldSurf();
}

void CIehpItemDlg_MEC::CtrlMaterialType()
{
	UpdateData(TRUE);
	EnableDisableCtrls_MatlType();
	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::CtrlDefinition()
{
	UpdateData(TRUE);
	EnableDisableCtrls_DefType();
	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::CtrlWallType()
{
	UpdateData(TRUE);
	EnableDisableCtrls_WallType();
	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::CtrlLocation()
{
	UpdateData(TRUE);
	EnableDisableCtrls_RefLoc();
	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnCmdApply()
{
	if ( !Dlg2Data() ) return;
}

void CIehpItemDlg_MEC::EnableDisableCtrlDofDx()
{
	// 체크온 된 상태로 Fix
	BOOL bFixed = m_nNewHingeType == 1;
	if ( bFixed )
	{
		m_bDof[D_DOF_FX] = TRUE;
		UpdateData(FALSE);
	}

	CArray<UINT, UINT> aDx;
	aDx.RemoveAll();
	aDx.Add(IDC_CMD_HINGE_LOC_DX_CMB);
	aDx.Add(IDC_CMD_SECTION_NUM_DX_EDIT);
	aDx.Add(IDC_CMD_HYS_MODEL_DX_CMB);
	aDx.Add(IDC_CMD_HINGE_STIFF_LOC_DX_CMB);
	aDx.Add(IDC_CMD_HYS_MODEL_DX_BTN);
	aDx.Add(IDC_CMD_PROP_DX_BTN);

	BOOL bEnable = GetDlgItem(IDC_CMD_DOF_DX_CHK)->IsWindowEnabled();
	CDlgUtil::CtrlEnableDisable(this, aDx, m_bDof[D_DOF_FX] && bEnable);

	if ( m_nNewHingeType == 1 )
	{
		GetDlgItem(IDC_CMD_HYS_MODEL_DX_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HYS_MODEL_DX_BTN)->EnableWindow(FALSE);
	}

	// Fx와 My/Mz를 연동 처리 
	BOOL bMyFollowsFx = (m_nNewHingeType == 0 && m_nInteractionType == 2) ||  // Skeleton && P-M-M
		(m_nNewHingeType == 1); // Fiber
	BOOL bMzFollowsFx = bMyFollowsFx && (m_nElementType != HT_WALL_CRB || (m_bConsOutpl && m_nWallType == 1)); // Fiber : 벽체&ConsOutpl이 꺼졌을 때는 Mz비활성화함

	if (m_bDof[D_DOF_FX] && m_nFlexStiff == 1)
	{
		m_strSec[D_DOF_FX] = _T("1");
		UpdateData(FALSE);
	}


	if ( bMyFollowsFx )
	{
		m_bDof[D_DOF_MY] = m_bDof[D_DOF_FX];
		if (m_nFlexStiff == 1)
		{
			m_strSec[D_DOF_MY] = _T("1");
		}
		GetDlgItem(IDC_CMD_SECTION_NUM_RY_EDIT)->EnableWindow(m_bDof[D_DOF_MY]);
		GetDlgItem(IDC_CMD_PROP_RY_BTN)->EnableWindow(m_bDof[D_DOF_MY]);
		UpdateData(FALSE);
	}
	if ( bMzFollowsFx )
	{
		m_bDof[D_DOF_MZ] = m_bDof[D_DOF_FX];
		if (m_nFlexStiff == 1)
		{
			m_strSec[D_DOF_MZ] = _T("1");
		}
		GetDlgItem(IDC_CMD_SECTION_NUM_RZ_EDIT)->EnableWindow(m_bDof[D_DOF_MZ]);
		GetDlgItem(IDC_CMD_PROP_RZ_BTN)->EnableWindow(m_bDof[D_DOF_MZ]);
		UpdateData(FALSE);
	}
}

void CIehpItemDlg_MEC::EnableDisableCtrlDofDy()
{
	CArray<UINT, UINT> aDy;
	aDy.RemoveAll();
	aDy.Add(IDC_CMD_HINGE_LOC_DY_CMB);
	aDy.Add(IDC_CMD_SECTION_NUM_DY_EDIT);
	aDy.Add(IDC_CMD_HYS_MODEL_DY_CMB);
	aDy.Add(IDC_CMD_HYS_MODEL_DY_BTN);
	aDy.Add(IDC_CMD_HINGE_STIFF_LOC_DY_CMB);
	aDy.Add(IDC_CMD_PROP_DY_BTN);

	BOOL bEnable = GetDlgItem(IDC_CMD_DOF_DY_CHK)->IsWindowEnabled();
	//if(bEnable) UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, aDy, m_bDof[D_DOF_FY] && bEnable);

	BOOL bFiberWall = m_nElementType == HT_WALL_CRB && m_nNewHingeType == 1;
	((CEdit*)GetDlgItem(IDC_CMD_SECTION_NUM_DY_EDIT))->SetReadOnly(bEnable && bFiberWall);
	if ( bEnable && bFiberWall )
	{
		m_strSec[D_DOF_FY] = _T("1");
		UpdateData(FALSE);
	}

	if (m_nFlexStiff == 1)
	{
		UpdateData(TRUE);
		// P-M-M or Fiber
		if (m_nInteractionType == 2 || m_nNewHingeType == 1)
		{
			m_cmbHngLoc[D_DOF_FY].GetItemData(m_cmbHngLoc[D_DOF_FX].GetCurSel());
		}
		if (m_bDof[D_DOF_FY])
		{
			m_strSec[D_DOF_FY] = _T("1");
			UpdateData(FALSE);
		}
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_HINGE_STIFF_LOC_DY_CMB, m_bDof[D_DOF_FY] && !(m_nInteractionType == 2 || m_nNewHingeType == 1));
		UpdateData(FALSE);
	}
}

void CIehpItemDlg_MEC::EnableDisableCtrlDofDz()
{
	CArray<UINT, UINT> aDz;
	aDz.RemoveAll();
	aDz.Add(IDC_CMD_HINGE_LOC_DZ_CMB);
	aDz.Add(IDC_CMD_SECTION_NUM_DZ_EDIT);
	aDz.Add(IDC_CMD_HYS_MODEL_DZ_CMB);
	aDz.Add(IDC_CMD_HYS_MODEL_DZ_BTN);
	aDz.Add(IDC_CMD_HINGE_STIFF_LOC_DZ_CMB);
	aDz.Add(IDC_CMD_PROP_DZ_BTN);

	BOOL bEnable = GetDlgItem(IDC_CMD_DOF_DZ_CHK)->IsWindowEnabled();

	//if(bEnable) UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, aDz, m_bDof[D_DOF_FZ] && bEnable);

	BOOL bFiberWall = m_nElementType == HT_WALL_CRB && m_nNewHingeType == 1;
	((CEdit*)GetDlgItem(IDC_CMD_SECTION_NUM_DZ_EDIT))->SetReadOnly(bEnable && bFiberWall);
	if ( bEnable && bFiberWall )
	{
		m_strSec[D_DOF_FZ] = _T("1");
		UpdateData(FALSE);
	}
	if (m_nFlexStiff == 1)
	{
		UpdateData(TRUE);
		// P-M-M or Fiber
		if (m_nInteractionType == 2 || m_nNewHingeType == 1)
		{
			m_cmbHngLoc[D_DOF_FZ].GetItemData(m_cmbHngLoc[D_DOF_FX].GetCurSel());
		}
		if (m_bDof[D_DOF_FZ])
		{
			m_strSec[D_DOF_FZ] = _T("1");
			UpdateData(FALSE);
		}
		CDlgUtil::CtrlEnableDisable(this, IDC_CMD_HINGE_STIFF_LOC_DZ_CMB, m_bDof[D_DOF_FZ] && !(m_nInteractionType == 2 || m_nNewHingeType == 1));
		UpdateData(FALSE);
	}
}

void CIehpItemDlg_MEC::EnableDisableCtrlDofRx()
{
	CArray<UINT, UINT> aRx;
	aRx.RemoveAll();
	aRx.Add(IDC_CMD_HINGE_LOC_RX_CMB);
	aRx.Add(IDC_CMD_SECTION_NUM_RX_EDIT);
	aRx.Add(IDC_CMD_HYS_MODEL_RX_CMB);
	aRx.Add(IDC_CMD_HYS_MODEL_RX_BTN);
	aRx.Add(IDC_CMD_HINGE_STIFF_LOC_RX_CMB);
	aRx.Add(IDC_CMD_PROP_RX_BTN);

	BOOL bEnable = GetDlgItem(IDC_CMD_DOF_RX_CHK)->IsWindowEnabled();
	//if(bEnable) UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, aRx, m_bDof[D_DOF_MX] && bEnable);

	if (m_bDof[D_DOF_MX] && m_nFlexStiff == 1)
	{
		m_strSec[D_DOF_MX] = _T("1");
		UpdateData(FALSE);
	}
}

void CIehpItemDlg_MEC::EnableDisableCtrlDofRy()
{
	// Fiber일 때는 Fx와 연동
	BOOL bMyFollowsFx = (m_nNewHingeType == 0 && m_nInteractionType == 2) ||  // Skeleton && P-M-M
		(m_nNewHingeType == 1); // Fiber
	if ( bMyFollowsFx )
	{
		UpdateData(TRUE);
		m_bDof[D_DOF_MY] = m_bDof[D_DOF_FX];

		if (m_bDof[D_DOF_MY] && m_nFlexStiff == 1)
		{
			m_strSec[D_DOF_MY] = _T("1");
		}

		UpdateData(FALSE);
	}
	((CEdit*)GetDlgItem(IDC_CMD_SECTION_NUM_RY_EDIT))->SetReadOnly(bMyFollowsFx);

	BOOL bEnable = GetDlgItem(IDC_CMD_DOF_RY_CHK)->IsWindowEnabled();

	GetDlgItem(IDC_CMD_HINGE_LOC_RY_CMB)->EnableWindow(bEnable && m_bDof[D_DOF_MY] && !bMyFollowsFx);
	GetDlgItem(IDC_CMD_SECTION_NUM_RY_EDIT)->EnableWindow(bEnable && m_bDof[D_DOF_MY]);
	GetDlgItem(IDC_CMD_HYS_MODEL_RY_CMB)->EnableWindow(bEnable && m_bDof[D_DOF_MY] && !bMyFollowsFx);
	GetDlgItem(IDC_CMD_HYS_MODEL_RY_BTN)->EnableWindow(bEnable && m_bDof[D_DOF_MY] && !bMyFollowsFx);
	GetDlgItem(IDC_CMD_HINGE_STIFF_LOC_RY_CMB)->EnableWindow(bEnable && m_bDof[D_DOF_MY] && !bMyFollowsFx);

	GetDlgItem(IDC_CMD_PROP_RY_BTN)->EnableWindow(m_bDof[D_DOF_MY]);

	if (m_bDof[D_DOF_MY] && m_nFlexStiff == 1)
	{
		m_strSec[D_DOF_MY] = _T("1");
		UpdateData(FALSE);
	}
}

void CIehpItemDlg_MEC::EnableDisableCtrlDofRz()
{
	// Fx와 My/Mz를 연동 처리 
	BOOL bMyFollowsFx = (m_nNewHingeType == 0 && m_nInteractionType == 2) ||  // Skeleton && P-M-M
		(m_nNewHingeType == 1); // Fiber
	BOOL bMzFollowsFx = bMyFollowsFx && (m_nElementType != HT_WALL_CRB || (m_bConsOutpl && m_nWallType == 1)); // Fiber : 벽체&ConsOutpl이 꺼졌을 때는 Mz비활성화함
	if ( bMzFollowsFx )
	{
		UpdateData(TRUE);
		m_bDof[D_DOF_MZ] = m_bDof[D_DOF_FX];
		if (m_bDof[D_DOF_MZ] && m_nFlexStiff == 1)
		{
			m_strSec[D_DOF_MZ] = _T("1");
		}
		UpdateData(FALSE);
	}
	((CEdit*)GetDlgItem(IDC_CMD_SECTION_NUM_RZ_EDIT))->SetReadOnly(bMzFollowsFx);

	BOOL bEnable = GetDlgItem(IDC_CMD_DOF_RZ_CHK)->IsWindowEnabled();

	GetDlgItem(IDC_CMD_HINGE_LOC_RZ_CMB)->EnableWindow(bEnable && m_bDof[D_DOF_MZ] && !bMzFollowsFx);
	GetDlgItem(IDC_CMD_SECTION_NUM_RZ_EDIT)->EnableWindow(bEnable && m_bDof[D_DOF_MZ]);
	GetDlgItem(IDC_CMD_HYS_MODEL_RZ_CMB)->EnableWindow(bEnable && m_bDof[D_DOF_MZ] && !bMzFollowsFx);
	GetDlgItem(IDC_CMD_HYS_MODEL_RZ_BTN)->EnableWindow(bEnable && m_bDof[D_DOF_MZ] && !bMzFollowsFx);
	GetDlgItem(IDC_CMD_HINGE_STIFF_LOC_RZ_CMB)->EnableWindow(bEnable && m_bDof[D_DOF_MZ] && !bMyFollowsFx);

	GetDlgItem(IDC_CMD_PROP_RZ_BTN)->EnableWindow(m_bDof[D_DOF_MZ]);

	if (m_bDof[D_DOF_MZ] && m_nFlexStiff == 1)
	{
		m_strSec[D_DOF_MZ] = _T("1");
		UpdateData(FALSE);
	}
}

void CIehpItemDlg_MEC::OnCmdDofDxChk() { UpdateData(TRUE);  EnableDisableCtrlDofDx();  UpdateData(FALSE); }
void CIehpItemDlg_MEC::OnCmdDofDyChk() { UpdateData(TRUE);  EnableDisableCtrlDofDy();  UpdateData(FALSE); }
void CIehpItemDlg_MEC::OnCmdDofDzChk() { UpdateData(TRUE);  EnableDisableCtrlDofDz();  UpdateData(FALSE); }
void CIehpItemDlg_MEC::OnCmdDofRxChk() { UpdateData(TRUE);  EnableDisableCtrlDofRx();  UpdateData(FALSE); }
void CIehpItemDlg_MEC::OnCmdDofRyChk() { UpdateData(TRUE);  EnableDisableCtrlDofRy();  ChangebyCtrlDofRy(); UpdateData(FALSE); }
void CIehpItemDlg_MEC::OnCmdDofRzChk() { UpdateData(TRUE);  EnableDisableCtrlDofRz();  ChangebyCtrlDofRz(); UpdateData(FALSE); }

void CIehpItemDlg_MEC::OnCmdPropDxBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_PROP_DX_BTN);
	CString str; str.Empty();
	pWnd->GetWindowText(str);
	pWnd->SetWindowText(str + _T(">>"));

	int nSel = m_cmbModel[D_DOF_FX].GetItemData(m_cmbModel[D_DOF_FX].GetCurSel());
	int nTabStyle = GetTabStyle(0);


	T_IEHP_D data; data.Initialize();

	Dlg2CommonData();
	if ( m_bPropMod[D_DOF_FX] ) data = m_Data;

	PropBtnProc(data, 0, nSel, 0, nTabStyle);

	pWnd->SetWindowText(str);
}

void CIehpItemDlg_MEC::OnCmdPropDyBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_PROP_DY_BTN);
	CString str; str.Empty();
	pWnd->GetWindowText(str);
	pWnd->SetWindowText(str + _T(">>"));

	int nSel = m_cmbModel[D_DOF_FY].GetItemData(m_cmbModel[D_DOF_FY].GetCurSel());
	int nTabStyle = GetTabStyle(1);

	T_IEHP_D data; data.Initialize();

	Dlg2CommonData();
	if ( m_bPropMod[D_DOF_FY] ) data = m_Data;

	PropBtnProc(data, 1, nSel, 1, nTabStyle);

	pWnd->SetWindowText(str);
}

void CIehpItemDlg_MEC::OnCmdPropDzBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_PROP_DZ_BTN);
	CString str; str.Empty();
	pWnd->GetWindowText(str);
	pWnd->SetWindowText(str + _T(">>"));

	int nSel = m_cmbModel[D_DOF_FZ].GetItemData(m_cmbModel[D_DOF_FZ].GetCurSel());
	int nTabStyle = GetTabStyle(2);

	T_IEHP_D data; data.Initialize();

	Dlg2CommonData();
	if ( m_bPropMod[D_DOF_FZ] ) data = m_Data;

	PropBtnProc(data, 2, nSel, 2, nTabStyle);

	pWnd->SetWindowText(str);
}

void CIehpItemDlg_MEC::OnCmdPropRxBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_PROP_RX_BTN);
	CString str; str.Empty();
	pWnd->GetWindowText(str);
	pWnd->SetWindowText(str + _T(">>"));

	int nSel = m_cmbModel[D_DOF_MX].GetItemData(m_cmbModel[D_DOF_MX].GetCurSel());
	int nTabStyle = GetTabStyle(3);

	T_IEHP_D data; data.Initialize();

	Dlg2CommonData();
	if ( m_bPropMod[D_DOF_MX] ) data = m_Data;

	int nFg = 0;
	if ( m_nInteractionType == 0 ) nFg = 3;
	else                        nFg = 6;

	PropBtnProc(data, 3, nSel, nFg, nTabStyle);

	pWnd->SetWindowText(str);
}

void CIehpItemDlg_MEC::OnCmdPropRyBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_PROP_RY_BTN);
	CString str; str.Empty();
	pWnd->GetWindowText(str);
	pWnd->SetWindowText(str + _T(">>"));

	int nSel = m_cmbModel[D_DOF_MY].GetItemData(m_cmbModel[D_DOF_MY].GetCurSel());
	int nTabStyle = GetTabStyle(4);

	T_IEHP_D data; data.Initialize();

	Dlg2CommonData();
	if ( m_bPropMod[D_DOF_MY] ) data = m_Data;

	int nFg = 0;
	if ( m_nInteractionType == 0 ) nFg = 4;
	else                        nFg = 7;

	PropBtnProc(data, 4, nSel, nFg, nTabStyle);

	pWnd->SetWindowText(str);
}

void CIehpItemDlg_MEC::OnCmdPropRzBtn()
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_PROP_RZ_BTN);
	CString str; str.Empty();
	pWnd->GetWindowText(str);
	pWnd->SetWindowText(str + _T(">>"));

	int nSel = m_cmbModel[D_DOF_MZ].GetItemData(m_cmbModel[D_DOF_MZ].GetCurSel());
	int nTabStyle = GetTabStyle(5);

	int nHingeLocationIJ = 0;
	if ( m_nDefinitionType == MOMENT_ROTATION )
		nHingeLocationIJ = m_cmbHngLoc[D_DOF_MZ].GetItemData(m_cmbHngLoc[D_DOF_MZ].GetCurSel());

	T_IEHP_D data; data.Initialize();

	Dlg2CommonData();
	if ( m_bPropMod[D_DOF_MZ] ) data = m_Data;

	int nFg = 0;
	if ( m_nInteractionType == 0 ) nFg = 5;
	else                        nFg = 8;

	PropBtnProc(data, 5, nSel, nFg, nTabStyle);

	pWnd->SetWindowText(str);
}

bool CIehpItemDlg_MEC::PropBtnProc(T_IEHP_D data, int n, int nSel, int nFg, int nTabStyle, bool bUpdateNeeded)
{
	UpdateData(TRUE);
	T_SECT_K SectK = m_cmbSect.GetItemData(m_cmbSect.GetCurSel());
	//T_MATL_K MatlK = m_cmbMatl.GetItemData(m_cmbMatl.GetCurSel());
	//int nSectType = m_pDoc->m_pEditData->CheckSect4Iehp(SectK, MatlK, m_nMembType, m_nMatlType);
	int nSectType = 0;
	int nDlgType = 0;
	switch ( nSel )
	{
	case D_IEHP_LRBB:
	case D_IEHP_LRBT:
	case D_IEHP_DMPR:
	case D_IEHP_LRBH16:
	case D_IEHP_LRBH30:
	case D_IEHP_DMPRH16:
	case D_IEHP_DMPRH30:
		nDlgType = 1;
		break;
	case D_IEHP_RO:
	case D_IEHP_HD:
		nDlgType = 2;
		break;
	case D_IEHP_MLEL:
	case D_IEHP_MLKI:
	case D_IEHP_MLTA:
	case D_IEHP_MLPI:
		nDlgType = 3;
		break;
	case D_IEHP_AXIAL:
		nDlgType = 4;
		break;
	case D_IEHP_BUCK:
		nDlgType = 5;
		break;
	case D_IEHP_FBIL:
	case D_IEHP_FINF:
	case D_IEHP_FTRI:
		nDlgType = 6;
		break;
	case D_IEHP_PMMC:
		nDlgType = 7;
		break;
	default:
		nDlgType = 0;
		break;
	}

	if ( nDlgType == 0 )
	{
		CIehpPropDlg* pDlg = new CIehpPropDlg(nTabStyle);
		pDlg->SetInitPos(D_INIT_POS_RT);
		switch ( nSel )
		{
		case D_IEHP_KINE:
		case D_IEHP_NELS:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].KINEMA.Initialize();
				data.AllSubProp.PROP[n].KINEMA.Initialize();

#if defined (_US)
				if ( m_Data.nDefinition == 1 )
				{
					data.AllProp.PROP[n].KINEMA.nYieldStrengthOpt = 0;
					data.AllSubProp.PROP[n].KINEMA.nYieldStrengthOpt = 0;
				}
#endif
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].KINEMA, data.AllSubProp.PROP[n].KINEMA, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].KINEMA.nYieldStrengthOpt);
			break;
		case D_IEHP_ORIG:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].ORIGIN.Initialize();
				data.AllSubProp.PROP[n].ORIGIN.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].ORIGIN, data.AllSubProp.PROP[n].ORIGIN, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].ORIGIN.nYieldStrengthOpt);
			break;
		case D_IEHP_PICK:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].PEAK.Initialize();
				data.AllSubProp.PROP[n].PEAK.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey,  data.AllProp.PROP[n].PEAK, data.AllSubProp.PROP[n].PEAK, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].PEAK.nYieldStrengthOpt);
			break;
		case D_IEHP_CLOU:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].CLOUGH.Initialize();
				data.AllSubProp.PROP[n].CLOUGH.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey,  data.AllProp.PROP[n].CLOUGH, data.AllSubProp.PROP[n].CLOUGH, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].CLOUGH.nYieldStrengthOpt);
			break;
		case D_IEHP_DEGR:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].DEGRAD.Initialize();
				data.AllSubProp.PROP[n].DEGRAD.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey,  data.AllProp.PROP[n].DEGRAD, data.AllSubProp.PROP[n].DEGRAD, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].DEGRAD.nYieldStrengthOpt);
			break;
		case D_IEHP_TAKE:
		case D_IEHP_TAKS:
		case D_IEHP_MTAK:
		case D_IEHP_ASYM:
		case D_IEHP_NELA:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].TAKEDA.Initialize();
				data.AllSubProp.PROP[n].TAKEDA.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey,  data.AllProp.PROP[n].TAKEDA, data.AllSubProp.PROP[n].TAKEDA, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].TAKEDA.nYieldStrengthOpt);
			break;
		case D_IEHP_NBIL:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].NORBIL.Initialize();
				data.AllSubProp.PROP[n].NORBIL.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey,  data.AllProp.PROP[n].NORBIL, data.AllSubProp.PROP[n].NORBIL, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].NORBIL.nYieldStrengthOpt);
			break;
		case D_IEHP_EBIL:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].ELABIL.Initialize();
				data.AllSubProp.PROP[n].ELABIL.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].ELABIL, data.AllSubProp.PROP[n].ELABIL, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].ELABIL.nYieldStrengthOpt);
			break;
		case D_IEHP_ETRI:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].ELATRI.Initialize();
				data.AllSubProp.PROP[n].ELATRI.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].ELATRI, data.AllSubProp.PROP[n].ELATRI, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].ELATRI.nYieldStrengthOpt);
			break;
		case D_IEHP_ETET:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].ELATET.Initialize();
				data.AllSubProp.PROP[n].ELATET.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].ELATET, data.AllSubProp.PROP[n].ELATET, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].ELATET.nYieldStrengthOpt);
			break;
		case D_IEHP_TTET:
		case D_IEHP_MTTE:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].TAKTET.Initialize();
				data.AllSubProp.PROP[n].TAKTET.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].TAKTET, data.AllSubProp.PROP[n].TAKTET, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].TAKTET.nYieldStrengthOpt);
			break;
		case D_IEHP_SRCT:
			if (!m_bPropMod[n])
			{
				data.AllProp.PROP[n].SRCTET.Initialize();
				data.AllSubProp.PROP[n].SRCTET.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].SRCTET, data.AllSubProp.PROP[n].SRCTET, nSel, nFg, nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].SRCTET.nYieldStrengthOpt);
			break;
		case D_IEHP_SLPB: case D_IEHP_SLBT:
		case D_IEHP_SLBC: case D_IEHP_SLPT:
		case D_IEHP_SLTT: case D_IEHP_SLTC:
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].SLIP.Initialize();
				data.AllSubProp.PROP[n].SLIP.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].SLIP, data.AllSubProp.PROP[n].SLIP, nSel, nFg,nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(m_bExistIJPropDlg[n], data.AllProp.PROP[n].SLIP.nYieldStrengthOpt);
			break;
		default: ASSERT(0); break;
		}
		if (bUpdateNeeded)		
		{
			pDlg->Create(pDlg->IDD);
			if (!pDlg->SyncDialogData()) return false;
			pDlg->DestroyWindow();

			m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
			m_Data.AllSubProp.PROP[n] = pDlg->m_Data.AllSubProp.PROP[n];
			m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
			m_bPropMod[n] = TRUE;
			m_bExistIJPropDlg[n] = pDlg->m_bExistIJData;
		}
		else
		{
			if (pDlg->DoModal() == IDOK)
			{
				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.AllSubProp.PROP[n] = pDlg->m_Data.AllSubProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
				m_bExistIJPropDlg[n] = pDlg->m_bExistIJData;
			}
		}
		delete pDlg;
	}
	else if ( nDlgType == 1 ) // Isolator 인 경우
	{
		if ( nSel == D_IEHP_LRBB )
		{
			CIsolatorLrbBilDlg* pDlg = new CIsolatorLrbBilDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].LRBBIL.Initialize();
				data.AllSubProp.PROP[n].LRBBIL.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].LRBBIL, nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else if ( nSel == D_IEHP_LRBT )
		{
			CIsolatorLrbTriDlg* pDlg = new CIsolatorLrbTriDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].LRBTRI.Initialize();
				data.AllSubProp.PROP[n].LRBTRI.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].LRBTRI, nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else if ( nSel == D_IEHP_DMPR )
		{
			CIsolatorDmpRbrDlg* pDlg = new CIsolatorDmpRbrDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].DMPRBR.Initialize();
				data.AllSubProp.PROP[n].DMPRBR.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].DMPRBR, nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else if (nSel == D_IEHP_LRBH16)
		{
			CIsolatorLrbH16Dlg* pDlg = new CIsolatorLrbH16Dlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if (!m_bPropMod[n])
			{
				data.AllProp.PROP[n].LRBH16.Initialize();
				data.AllSubProp.PROP[n].LRBH16.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].LRBH16, nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else if (nSel == D_IEHP_LRBH30)
		{
			CIsolatorLrbH30Dlg* pDlg = new CIsolatorLrbH30Dlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if (!m_bPropMod[n])
			{
				data.AllProp.PROP[n].LRBH30.Initialize();
				data.AllSubProp.PROP[n].LRBH30.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].LRBH30, nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else if (nSel == D_IEHP_DMPRH16)
		{
			CIsolatorDmpH16RbrDlg* pDlg = new CIsolatorDmpH16RbrDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if (!m_bPropMod[n])
			{
				data.AllProp.PROP[n].DMPRH16.Initialize();
				data.AllSubProp.PROP[n].DMPRH16.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].DMPRH16, nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else if (nSel == D_IEHP_DMPRH30)
		{
			CIsolatorDmpH30RbrDlg* pDlg = new CIsolatorDmpH30RbrDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if (!m_bPropMod[n])
			{
				data.AllProp.PROP[n].DMPRH30.Initialize();
				data.AllSubProp.PROP[n].DMPRH30.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].DMPRH30, nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else ASSERT(0);
	}
	else if ( nDlgType == 2 )
	{
		CIehpPropROHDDlg* pDlg = new CIehpPropROHDDlg(&data.AllProp.PROP[n].ROHD);
		pDlg->SetInitPos(D_INIT_POS_RT);
		if ( !m_bPropMod[n] )
		{
			data.AllProp.PROP[n].ROHD.Initialize();
			data.AllSubProp.PROP[n].ROHD.Initialize();
		}

		int nDof = (nFg < 3) ? 0 : 1;
		pDlg->SetROHDType(nSel, nDof);

		if (bUpdateNeeded)
		{
			pDlg->Create(pDlg->IDD);
			if (!pDlg->SyncDialogData()) return false;
			pDlg->DestroyWindow();

			m_Data.AllProp.PROP[n] = data.AllProp.PROP[n];
			m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
			m_bPropMod[n] = TRUE;
		}
		else
		{
			if (pDlg->DoModal() == IDOK)
			{
				m_Data.AllProp.PROP[n] = data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
		}
		delete pDlg;
	}
	else if ( nDlgType == 3 )
	{
		CIehpProp_MLinDlg* pDlg = new CIehpProp_MLinDlg();
		pDlg->SetInitPos(D_INIT_POS_RT);
		if ( !m_bPropMod[n] )
		{
			data.AllProp.PROP[n].MULTLIN.Initialize();
			data.AllSubProp.PROP[n].MULTLIN.Initialize();
			if ( nSel == D_IEHP_MLTA )
			{
				data.AllProp.PROP[n].MULTLIN.dHysParam_Beta1 = 0.4; // default value
				data.AllSubProp.PROP[n].MULTLIN.dHysParam_Beta1 = 0.4; // default value
			}
		}

		pDlg->SetIehpPropData(m_Data, data.AllProp.PROP[n].MULTLIN, nSel, nFg);

		if (bUpdateNeeded)
		{
			pDlg->Create(pDlg->IDD);
			if (!pDlg->SyncDialogData()) return false;
			pDlg->CDialog::DestroyWindow();

			m_Data.MultiData[n] = pDlg->m_MultiData;
			m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
			m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
			m_bPropMod[n] = TRUE;
		}
		else
		{
			if (pDlg->DoModal() == IDOK)
			{
				m_Data.MultiData[n] = pDlg->m_MultiData;
				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
		}
		delete pDlg;
	}
	else if ( nDlgType == 4 )
	{
		if ( nSel == D_IEHP_AXIAL )
		{
			CIehpPropAxialDlg* pDlg = new CIehpPropAxialDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].AXIAL.Initialize();
				data.AllSubProp.PROP[n].AXIAL.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].AXIAL, data.AllSubProp.PROP[n].AXIAL, nSel, nFg, nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(data.AllProp.PROP[n].AXIAL.nYieldStrengthOpt);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->CDialog::DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
	}
	else if ( nDlgType == 5 )
	{
		if ( nSel == D_IEHP_BUCK )
		{
			CIehpPropStlDlg* pDlg = new CIehpPropStlDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			if ( !m_bPropMod[n] )
			{
				data.AllProp.PROP[n].BUCKL.Initialize();
				data.AllSubProp.PROP[n].BUCKL.Initialize();
			}
			pDlg->SetIehpPropData(m_Data, m_AssignedIehgKey, data.AllProp.PROP[n].BUCKL, data.AllSubProp.PROP[n].BUCKL, nSel, nFg, nSectType, m_bAssignHingeMode);
			pDlg->SetControlStateData(data.AllProp.PROP[n].BUCKL.nYieldStrengthOpt);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->DestroyWindow();

				m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
				m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PROP[n] = pDlg->m_Data.AllProp.PROP[n];
					m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
	}
	else if ( nDlgType == 6 )
	{
		if ( !m_bPropMod[n] )
		{
			if ( nSel == D_IEHP_FINF )
			{
				m_Data.AllProp.PROP[n].FEMA.InitializeRatio_Infill();
				m_Data.AllSubProp.PROP[n].FEMA.InitializeRatio_Infill();
			}
			else
			{
				m_Data.AllProp.PROP[n].FEMA.Initialize();
				m_Data.AllSubProp.PROP[n].FEMA.Initialize();
			}
		}

		T_AGEN_D AgenD;
		Dlg2CommonData();
		AgenD.nDlgID = D_RPS_DO_MODAL_PO_FEMA;
		m_Data.nHysModel[n] = m_cmbModel[n].GetItemData(m_cmbModel[n].GetCurSel());
		m_Data.nHingeLocation[n] = m_cmbHngLoc[n].GetItemData(m_cmbHngLoc[n].GetCurSel());

		// MQC 23768 : 데이터를 통째로 넣었더니 안에서 초기화하면서 값 변경돼서, 
		// 해당 성분만 업데이트하도록 백업데이터로 처리
		T_IEHP_D IehpBak = m_Data;
		AgenD.pData = (void*)&IehpBak;

		// 대충 배열에 넣어 보내지만 노오-력 해서 실수하지 말고 쓰자
		AgenD.nSubData[0] = n; // nDOF
		AgenD.nSubData[1] = m_bAssignHingeMode; // 할당여부
		AgenD.nSubData[2] = m_AssignedIehgKey;
		AgenD.nSubData[3] = TRUE; // PMS 5721-23 : 시간이력은 현재 자동계산을 지원하지 않으므로 무조건 비활성화함(181123)
		AgenD.nSubData[4] = bUpdateNeeded; // Update 필요 여부

		CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
		if ( pHeadOffice->ReqService(_ULS(rps), D_RPS_DO_MODAL, (void*)&AgenD) )
		{
			m_bPropMod[n] = TRUE;
			m_Data.AllProp.PROP[n].FEMA = IehpBak.AllProp.PROP[n].FEMA;
			m_Data.AllSubProp.PROP[n].FEMA = IehpBak.AllSubProp.PROP[n].FEMA;
			m_bExistIJPropDlg[n] = IehpBak.bExistIJProp[n];
		}
	}
	else if ( nDlgType == 7 )
	{
		if ( nSel == D_IEHP_PMMC )
		{
			if ( !m_bPropMod[n] )
			{
				m_Data.AllProp.PMMULT[n].Initialize();
				m_Data.AllSubProp.PMMULT[n].Initialize();
			}
			//CIehpProp_PMmcDlg pDlg;
			CIehpProp_PMmcMLDlg* pDlg = new CIehpProp_PMmcMLDlg();
			pDlg->SetInitPos(D_INIT_POS_RT);
			pDlg->SetIehpPropData(m_Data, data.AllProp.PMMULT[n], nSel, nFg);

			if (bUpdateNeeded)
			{
				pDlg->Create(pDlg->IDD);
				if (!pDlg->SyncDialogData()) return false;
				pDlg->CDialog::DestroyWindow();

				m_Data.AllProp.PMMULT[n] = pDlg->m_PMMultiData;
				m_bPropMod[n] = TRUE;
			}
			else
			{
				if (pDlg->DoModal() == IDOK)
				{
					m_Data.AllProp.PMMULT[n] = pDlg->m_PMMultiData;
					m_bPropMod[n] = TRUE;
				}
			}
			delete pDlg;
		}
		else
		{
			ASSERT(0);
		}
	}
	else ASSERT(0);

	return true;
}

void CIehpItemDlg_MEC::OnCmdPropYsBtn()
{
	UpdateData();
	Dlg2CommonData();

	UpdateData(TRUE);
	//T_SECT_K SectK = m_cmbSect.GetItemData(m_cmbSect.GetCurSel());
	//T_MATL_K MatlK = m_cmbMatl.GetItemData(m_cmbMatl.GetCurSel());

	// nSectType == 0: 적용할수 없는 단면, 1: 적용할수 있는 단면, 2: value type
	int nSectType = 0;
	if ( m_nElementType == HT_BEAM_COLM )
	{
		//nSectType = m_pDoc->m_pEditData->CheckSect4Iehp(SectK, MatlK, m_nMembType, m_nMatlType);
	}
	else if ( m_nElementType == HT_WALL_CRB ) // by GAY. ('13.07.15). PMS:4697. Wall의 동적탄소성 힌지 속성.
		nSectType = 1;


	CIehpYieldPropDlgNew_MEC dlg;
	dlg.SetInitPos(D_INIT_POS_RT);

	T_IEHP_YLDSUR data; data.Initialize();
	T_IEHP_YLDSUR Subdata; Subdata.Initialize();

	BOOL bUseMz = FALSE;
	/*if(m_Data.cDir[5]=='1') */ bUseMz = TRUE;

	BOOL bHysModel_Unable1stCtrl = FALSE;
	int nmodel = m_cmbModel[D_DOF_MY].GetItemData(m_cmbModel[D_DOF_MY].GetCurSel());

	if ( m_bDof[D_DOF_MY] )      bHysModel_Unable1stCtrl = IsHysModel_Unable1stCtrl(m_cmbModel[D_DOF_MY].GetItemData(m_cmbModel[D_DOF_MY].GetCurSel()));
	else if ( m_bDof[D_DOF_MZ] ) bHysModel_Unable1stCtrl = IsHysModel_Unable1stCtrl(m_cmbModel[D_DOF_MZ].GetItemData(m_cmbModel[D_DOF_MZ].GetCurSel()));
	BOOL bPMInteraction = m_Data.nInteractionType == 1;
	BOOL bUnable1stCtrl = bHysModel_Unable1stCtrl && bPMInteraction;

	BOOL bUse3rdCtrl = (m_Data.nInteractionType == 2 &&
		(m_cmbModel[D_DOF_FX].GetItemData(m_cmbModel[D_DOF_FX].GetCurSel()) == D_IEHP_MTTE || m_cmbModel[D_DOF_FX].GetItemData(m_cmbModel[D_DOF_FX].GetCurSel()) == D_IEHP_TTET));
	dlg.Set3rdControl(bUse3rdCtrl);

	if ( m_bModify || m_bPropMod[6] )
	{
		data = m_Data.AllProp.PROP[6].YLDSUR;
		Subdata = m_Data.AllSubProp.PROP[6].YLDSUR;
	}

	dlg.SetIJDataExist(m_bExistIJDataYield, data.nCurveUserAuto);
	dlg.SetIehpPropData(m_Data, data, Subdata, nSectType, m_AssignedIehgKey, m_bAssignHingeMode, bUseMz, bUnable1stCtrl);
	if ( dlg.DoModal() == IDOK )
	{
		m_bExistIJDataYield = dlg.m_bExistIJData;
		m_Data.AllProp.PROP[6].YLDSUR = dlg.m_MainData;
		m_Data.AllSubProp.PROP[6].YLDSUR = dlg.m_SubData;
		m_Data.cDir[6] = '1';
		m_bPropMod[6] = TRUE;
	}
}

void CIehpItemDlg_MEC::ChangeNumofSectionDx()
{
	UpdateData();
	if ( (m_nNewHingeType == 0 && m_nInteractionType == 2) ||  // Skeleton && P-M-M
		(m_nNewHingeType == 1 && ((m_nElementType == HT_BEAM_COLM || m_nElementType == HT_WALL_CRB) && m_nDefinitionType == 1)) ) // Fiber && Distributed
	{
		m_strSec[D_DOF_MY] = m_strSec[D_DOF_FX];

		// Wall & ConsOutpl일 때는 Mz 사용 안 함
		if ( m_nElementType != HT_WALL_CRB || (m_bConsOutpl && m_nWallType == 1) )
			m_strSec[D_DOF_MZ] = m_strSec[D_DOF_FX];
	}
	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnChangeSectDxEdit()
{
	ChangeNumofSectionDx();
}

void CIehpItemDlg_MEC::OnSelchangeLocDxCmb()
{
	UpdateData();
	if ( m_nInteractionType == 2 ) // P-M-M
	{
		m_cmbHngLoc[D_DOF_MY].SetCurSel(m_cmbHngLoc[D_DOF_FX].GetCurSel());
		m_cmbHngLoc[D_DOF_MZ].SetCurSel(m_cmbHngLoc[D_DOF_FX].GetCurSel());
	}
}

void CIehpItemDlg_MEC::OnSelchangeStiffLocDxCmb()
{
	UpdateData(TRUE);

	m_strSec[D_DOF_FX] = _T("1");

	if ((m_nNewHingeType == 0 && m_nInteractionType == 2) || (m_nNewHingeType == 1))
	{
		m_strSec[D_DOF_FY] = _T("1");
		m_strSec[D_DOF_FZ] = _T("1");
		m_strSec[D_DOF_MY] = _T("1");
		m_strSec[D_DOF_MZ] = _T("1");

		m_cmbHngStifLoc[D_DOF_FY].SetCurSel(m_cmbHngStifLoc[D_DOF_FX].GetCurSel());
		m_cmbHngStifLoc[D_DOF_FZ].SetCurSel(m_cmbHngStifLoc[D_DOF_FX].GetCurSel());
		m_cmbHngStifLoc[D_DOF_MY].SetCurSel(m_cmbHngStifLoc[D_DOF_FX].GetCurSel());
		m_cmbHngStifLoc[D_DOF_MZ].SetCurSel(m_cmbHngStifLoc[D_DOF_FX].GetCurSel());
	}

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnSelchangeStiffLocDyCmb()
{
	UpdateData(TRUE);

	m_strSec[D_DOF_FY] = _T("1");

	m_cmbHngStifLoc[D_DOF_MZ].SetCurSel(m_cmbHngStifLoc[D_DOF_FY].GetCurSel());

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnSelchangeStiffLocDzCmb()
{
	UpdateData(TRUE);

	m_strSec[D_DOF_FZ] = _T("1");

	m_cmbHngStifLoc[D_DOF_MY].SetCurSel(m_cmbHngStifLoc[D_DOF_FZ].GetCurSel());

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnSelchangeStiffLocRxCmb()
{
	UpdateData(TRUE);

	m_strSec[D_DOF_MX] = _T("1");

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnSelchangeStiffLocRyCmb()
{
	UpdateData(TRUE);

	m_strSec[D_DOF_MY] = _T("1");

	m_cmbHngStifLoc[D_DOF_FZ].SetCurSel(m_cmbHngStifLoc[D_DOF_MY].GetCurSel());

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnSelchangeStiffLocRzCmb()
{
	UpdateData(TRUE);

	m_strSec[D_DOF_MZ] = _T("1");

	m_cmbHngStifLoc[D_DOF_FY].SetCurSel(m_cmbHngStifLoc[D_DOF_MZ].GetCurSel());

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnOK()
{
	// TODO: Add extra validation here
	for (int i = D_DOF_FX; i < D_DOF_WP; i++)
	{
		if (!m_bDof[i]) continue;
		if (!UpdatePropData(i)) return;
	}

	if(!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}
void CIehpItemDlg_MEC::OnSelchangeModelCmb(int nDof)
{
	if ( nDof == 0 ) OnSelchangeModelDxCmb();
	else if ( nDof == 1 ) OnSelchangeModelDyCmb();
	else if ( nDof == 2 ) OnSelchangeModelDzCmb();
	else if ( nDof == 3 ) OnSelchangeModelRxCmb();
	else if ( nDof == 4 ) OnSelchangeModelRyCmb();
	else if ( nDof == 5 ) OnSelchangeModelRzCmb();
}

void CIehpItemDlg_MEC::OnSelchangeModelDxCmb()
{
	int nSel = m_cmbModel[D_DOF_FX].GetCurSel();
	if ( m_Data.nHysModel[0] == m_cmbModel[D_DOF_FX].GetItemData(nSel) )
		m_bPropMod[D_DOF_FX] = TRUE;
	else
	{
		if ( m_nMdlCombSel[0] != m_cmbModel[D_DOF_FX].GetItemData(nSel) )
			m_bPropMod[D_DOF_FX] = FALSE;
	}

	m_nMdlCombSel[0] = m_cmbModel[D_DOF_FX].GetItemData(nSel);
	m_btnModel[0].SetCurItem(m_nMdlCombSel[0]);
}

void CIehpItemDlg_MEC::OnSelchangeModelDyCmb()
{
	int nSel = m_cmbModel[D_DOF_FY].GetCurSel();
	if ( m_Data.nHysModel[1] == m_cmbModel[D_DOF_FY].GetItemData(nSel) )
		m_bPropMod[D_DOF_FY] = TRUE;
	else
	{
		if ( m_nMdlCombSel[1] != m_cmbModel[D_DOF_FY].GetItemData(nSel) )
			m_bPropMod[D_DOF_FY] = FALSE;
	}

	m_nMdlCombSel[1] = m_cmbModel[D_DOF_FY].GetItemData(nSel);
	m_btnModel[1].SetCurItem(m_nMdlCombSel[1]);
}

void CIehpItemDlg_MEC::OnSelchangeModelDzCmb()
{
	int nSel = m_cmbModel[D_DOF_FZ].GetCurSel();
	if ( m_Data.nHysModel[2] == m_cmbModel[D_DOF_FZ].GetItemData(nSel) )
		m_bPropMod[D_DOF_FZ] = TRUE;
	else
	{
		if ( m_nMdlCombSel[2] != m_cmbModel[D_DOF_FZ].GetItemData(nSel) )
			m_bPropMod[D_DOF_FZ] = FALSE;
	}

	m_nMdlCombSel[2] = m_cmbModel[D_DOF_FZ].GetItemData(nSel);
	m_btnModel[2].SetCurItem(m_nMdlCombSel[2]);
}

void CIehpItemDlg_MEC::OnSelchangeModelRxCmb()
{
	int nSel = m_cmbModel[D_DOF_MX].GetCurSel();
	if ( m_Data.nHysModel[3] == m_cmbModel[D_DOF_MX].GetItemData(nSel) )
		m_bPropMod[D_DOF_MX] = TRUE;
	else
	{
		if ( m_nMdlCombSel[3] != m_cmbModel[D_DOF_MX].GetItemData(nSel) )
			m_bPropMod[D_DOF_MX] = FALSE;
	}

	m_nMdlCombSel[3] = m_cmbModel[D_DOF_MX].GetItemData(nSel);
	m_btnModel[3].SetCurItem(m_nMdlCombSel[3]);
}

void CIehpItemDlg_MEC::OnSelchangeModelRyCmb()
{
	int nSel = m_cmbModel[D_DOF_MY].GetCurSel();
	if ( m_Data.nHysModel[4] == m_cmbModel[D_DOF_MY].GetItemData(nSel) )
		m_bPropMod[D_DOF_MY] = TRUE;
	else
	{
		if ( m_nMdlCombSel[4] != m_cmbModel[D_DOF_MY].GetItemData(nSel) )
			m_bPropMod[D_DOF_MY] = FALSE;
	}

	m_nMdlCombSel[4] = m_cmbModel[D_DOF_MY].GetItemData(nSel);
	m_btnModel[4].SetCurItem(m_nMdlCombSel[4]);
}

void CIehpItemDlg_MEC::OnSelchangeModelRzCmb()
{
	int nSel = m_cmbModel[D_DOF_MZ].GetCurSel();
	if ( m_Data.nHysModel[5] == m_cmbModel[D_DOF_MZ].GetItemData(nSel) )
		m_bPropMod[D_DOF_MZ] = TRUE;
	else
	{
		if ( m_nMdlCombSel[5] != m_cmbModel[D_DOF_MZ].GetItemData(nSel) )
			m_bPropMod[D_DOF_MZ] = FALSE;
	}

	m_nMdlCombSel[5] = m_cmbModel[D_DOF_MZ].GetItemData(nSel);
	m_btnModel[5].SetCurItem(m_nMdlCombSel[5]);
}

void CIehpItemDlg_MEC::MakeFibrCombo(BOOL bModify)
{
	m_cmbFibrName.ResetContent();
	if ( m_nNewHingeType != 1 ) return;

	if ( m_nElementType == HT_BEAM_COLM )
	{
		CArray<T_FIBR_K, T_FIBR_K> rKeyList;
		m_pDoc->m_pAttrCtrl->GetFibrKeyList(rKeyList, D_FIBR_TYPE_IEHP);
		T_FIBR_D FibrD;
		int nIndex = 0;
		for ( int i = 0; i < rKeyList.GetSize(); i++ )
		{
			m_pDoc->m_pAttrCtrl->GetFibr(rKeyList[i], FibrD);
			nIndex = m_cmbFibrName.AddString(FibrD.strName);
			m_cmbFibrName.SetItemData(nIndex, rKeyList[i]);
		}
	}
	else if ( m_nElementType == HT_WALL_CRB )
	{
		// 대화상자에서 선택된 두께를 사용하는 Fiber Wall 항목만 보여줌
		T_THIK_K ThikK = static_cast<T_THIK_K>(CDlgUtil::CobxGetCurSelItemData(m_cmbSect, m_cmbSect.GetCurSel()));

		CArray<T_FIBW_K, T_FIBW_K> rKeyList;
		m_pDoc->m_pAttrCtrl->GetFibwKeyList(rKeyList, D_FIBR_TYPE_IEHP);

		for ( int i = 0; i < rKeyList.GetSize(); i++ )
		{
			T_FIBW_D FibwD;
			m_pDoc->m_pAttrCtrl->GetFibw(rKeyList[i], FibwD);

			T_ELEM_K ElemK = m_pDoc->m_pAttrCtrl->GetElemKByWallIDStorK(FibwD.nWallID, FibwD.StorK);
			T_ELEM_D ElemD;
			if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { ASSERT(0); continue; }
			if ( !m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp) ) { ASSERT(0); continue; }
			if ( ElemD.elpro != ThikK )
				continue;

			CDlgUtil::CobxAddItem(m_cmbFibrName, FibwD.strName, rKeyList[i]);
		}
	}

	if ( bModify )
	{
		T_FIBR_K FibrKey = 0;
		if ( m_Data.nFiberAutoUser == 0 && m_bAssignHingeMode )
		{
			T_IEHG_D IehgD;
			m_pDoc->m_pAttrCtrl->GetIehg(m_AssignedIehgKey, IehgD);
			FibrKey = IehgD.FiberKey;
		}
		else
		{
			FibrKey = m_Data.FibrKey;
		}

		for ( int i = 0; i < m_cmbFibrName.GetCount(); i++ )
		{
			if ( m_cmbFibrName.GetItemData(i) == FibrKey )
			{
				m_cmbFibrName.SetCurSel(i);
				break;
			}
		}
	}
	else
	{
		if ( m_cmbFibrName.GetCount() > 0 ) m_cmbFibrName.SetCurSel(0);
	}

	CDlgUtil::CobxAdjustListBoxWidth(m_cmbFibrName);
}

void CIehpItemDlg_MEC::OnSelchangeSectCmb()
{
	MakeFibrCombo(FALSE);
}

void CIehpItemDlg_MEC::CtrlTypeManager()
{
	UpdateData(TRUE);

	EnableDisableCtrls();
	CtrlInteractionType();

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnAddDyna2E()
{
	UpdateData(TRUE);

	ModelComboCtrl();

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnModelChangedDx()
{
	DWORD nItemType;
	nItemType = m_btnModel[0].GetSelMenuID();
	CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_FX], nItemType);
	OnSelchangeModelCmb(D_DOF_FX);
}

void CIehpItemDlg_MEC::OnModelChangedDy()
{
	DWORD nItemType;
	nItemType = m_btnModel[1].GetSelMenuID();
	CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_FY], nItemType);
	OnSelchangeModelCmb(D_DOF_FY);
}

void CIehpItemDlg_MEC::OnModelChangedDz()
{
	DWORD nItemType;
	nItemType = m_btnModel[2].GetSelMenuID();
	CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_FZ], nItemType);
	OnSelchangeModelCmb(D_DOF_FZ);
}

void CIehpItemDlg_MEC::OnModelChangedRx()
{
	DWORD nItemType;
	nItemType = m_btnModel[3].GetSelMenuID();
	CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_MX], nItemType);
	OnSelchangeModelCmb(D_DOF_MX);
}

void CIehpItemDlg_MEC::OnModelChangedRy()
{
	DWORD nItemType;
	nItemType = m_btnModel[4].GetSelMenuID();
	CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_MY], nItemType);
	OnSelchangeModelCmb(D_DOF_MY);

// 	if (m_nNewHingeType == 0 && m_nInteractionType == 1) //Moment-Rotation && P-M
// 	{
// 		CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_MZ], nItemType);
// 		OnSelchangeModelCmb(D_DOF_MZ);
// 	}
}

void CIehpItemDlg_MEC::OnModelChangedRz()
{
	DWORD nItemType;
	nItemType = m_btnModel[5].GetSelMenuID();
	CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_MZ], nItemType);
	OnSelchangeModelCmb(D_DOF_MZ);

// 	if (m_nNewHingeType == 0 && m_nInteractionType == 1) //Moment-Rotation && P-M
// 	{
// 		CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_MY], nItemType);
// 		OnSelchangeModelCmb(D_DOF_MY);
// 	}
}

void CIehpItemDlg_MEC::OnLocationChangedRyCmb()
{
// 	if (m_nNewHingeType == 0 && m_nInteractionType == 1) //Moment-Rotation && P-M
// 	{
// 		int nHngCmbSel = m_cmbHngLoc[D_DOF_MY].GetCurSel();
// 		m_cmbHngLoc[D_DOF_MZ].SetCurSel(nHngCmbSel);
// 	}
}

void CIehpItemDlg_MEC::OnLocationChangedRzCmb()
{
// 	if (m_nNewHingeType == 0 && m_nInteractionType == 1) //Moment-Rotation && P-M
// 	{
// 		int nHngCmbSel = m_cmbHngLoc[D_DOF_MZ].GetCurSel();
// 		m_cmbHngLoc[D_DOF_MY].SetCurSel(nHngCmbSel);
// 	}
}

void CIehpItemDlg_MEC::ChangebyCtrlDofRy()
{
// 	if (m_nNewHingeType == 0 && m_nInteractionType == 1) //Moment-Rotation && P-M
// 	{
// 		DWORD nItemType;
// 		//nItemType = m_btnModel[5].GetSelMenuID();
// 		CDlgUtil::CobxGetItemDataByCurSel(m_cmbModel[D_DOF_MZ], nItemType);
// 		CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_MY], nItemType);
// 		OnSelchangeModelCmb(D_DOF_MY);
// 
// 		int nHngCmbSel = m_cmbHngLoc[D_DOF_MZ].GetCurSel();
// 		m_cmbHngLoc[D_DOF_MY].SetCurSel(nHngCmbSel);
// 	}
}

void CIehpItemDlg_MEC::ChangebyCtrlDofRz()
{
// 	if (m_nNewHingeType == 0 && m_nInteractionType == 1) //Moment-Rotation && P-M
// 	{
// 		DWORD nItemType;
// 		//nItemType = m_btnModel[4].GetSelMenuID();
// 		CDlgUtil::CobxGetItemDataByCurSel(m_cmbModel[D_DOF_MY], nItemType);
// 		CDlgUtil::CobxSetCurSelItemData(m_cmbModel[D_DOF_MZ], nItemType);
// 		OnSelchangeModelCmb(D_DOF_MZ);
// 
// 		int nHngCmbSel = m_cmbHngLoc[D_DOF_MY].GetCurSel();
// 		m_cmbHngLoc[D_DOF_MZ].SetCurSel(nHngCmbSel);
// 	}
}

void CIehpItemDlg_MEC::MakeBtnMenu(int ix)
{
	CString aCat[] = {
		_LS(IDS_CMD_SIMPLIFIED_MODEL),
		_LS(IDS_CMD_DEGRADING_MODEL),
		_LS(IDS_CMD_NONLINEAR_ELASTIC_MODEL),
		_LS(IDS_CMD_SLIP_MODEL),
		_LS(IDS_CMD_SEISMIC_ISOLATION_MODEL),
		_LS(IDS_CMD_SPECIAL_MODEL),
		_LS(IDS_CMD_ALL),
		_LS(IDS_CMD_MULTILINEAR_MODEL),
		_LS(IDS_CMD_STEEL_MODEL),
		_LS(IDS_CMD_IEHP_FEMA_MODEL),
		_LS(IDS_CMD_IEHP_PM_MULTI_CURVE),
	};

	CString csName;
	DWORD nItemType;
	int nCategory;
	m_btnModel[ix].ResetContent();

	for ( int j = 0; j < m_cmbModel[ix].GetCount(); j++ )
	{
		m_cmbModel[ix].GetLBText(j, csName);
		nItemType = m_cmbModel[ix].GetItemData(j);
		nCategory = GetCategory(nItemType);
		m_btnModel[ix].AddItem(aCat[nCategory] + _T(">>") + csName, nItemType);
	}
	for ( int j = 0; j < m_cmbModel[ix].GetCount(); j++ )
	{
		m_cmbModel[ix].GetLBText(j, csName);
		nItemType = m_cmbModel[ix].GetItemData(j);
		m_btnModel[ix].AddItem(aCat[6] + _T(">>") + csName, nItemType);

		//     if (CTestEnvMgr::GetTestEnvST(_LSX(IEHP_MultiLinear)) == _LSX(yes))   // MNET:XXXX-HSSHIM-20120321: 솔버팀(김종민 부장님) 요청으로 사용할 수 있도록 처리.
		//     {
		//       m_btnModel[ix].AddItem(aCat[7] + _T(">>") + csName, nItemType);
		//     }
	}

	m_btnModel[ix].MakeMenu();
}

void CIehpItemDlg_MEC::SetCurSelBtnMenu(int ix)
{
	DWORD nItemType;
	if ( CDlgUtil::CobxGetItemDataByCurSel(m_cmbModel[ix], nItemType) )
		m_btnModel[ix].SetCurItem(nItemType);
	
	if (m_bDof[ix]) OnSelchangeModelCmb(ix);
}


int CIehpItemDlg_MEC::GetTabStyle(int nIndex)
{
	UpdateData(TRUE);
	if ( m_nInteractionType == 3 ) return 0;  // ASSERT 피하기 위함.

	int nHingeLocationIJ = -1;
	if ( m_nDefinitionType == MOMENT_ROTATION )
		nHingeLocationIJ = m_cmbHngLoc[nIndex].GetItemData(m_cmbHngLoc[nIndex].GetCurSel());

	int nTabStyle = LOCATION_NONE;
	switch ( nHingeLocationIJ )
	{
	case -1:
		if ( m_nDefinitionType == MOMENT_CURVATURE )
		{
			switch ( m_nLocMemb )
			{
			case 0:	//I
				nTabStyle = I_LOCATION;
				break;
			case 1: //Center
				nTabStyle = CENTER_LOCATION;
				break;
			case 2: //J
				nTabStyle = J_LOCATION;
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		else
		{
			nTabStyle = LOCATION_NONE;
		}
		break;
	case 0:	//I
		nTabStyle = I_LOCATION;
		break;
	case 1: //J
		nTabStyle = J_LOCATION;
		break;
	case 2: //I & J
		nTabStyle = I_J_LOCATION;
		break;
	case 3: // Center
		nTabStyle = CENTER_LOCATION;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return nTabStyle;
}

void CIehpItemDlg_MEC::SetAssignHingeMode(BOOL bAssignHingeMode, T_IEHG_K IehgKey)
{
	m_bAssignHingeMode = bAssignHingeMode;
	m_AssignedIehgKey = IehgKey;
}

void CIehpItemDlg_MEC::EnableDisableCtrls_WallType()
{
	BOOL bIsEnableWallType = FALSE;
	if ( m_nElementType == HT_WALL_CRB )  bIsEnableWallType = TRUE;

	GetDlgItem(IDC_STATIC_WALL_TYPE)->EnableWindow(bIsEnableWallType);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO1)->EnableWindow(bIsEnableWallType);
	GetDlgItem(IDC_HINGE_TYPE_WALL_RDO2)->EnableWindow(bIsEnableWallType);

	if ( m_nWallType == -1 ) m_nWallType = 1;
	if ( !bIsEnableWallType ) m_nWallType = -1;
}

void CIehpItemDlg_MEC::EnableDisableCtrls_MatlType()
{
	BOOL bIsEnableMatlRC = TRUE;
	if ( m_nElementType == HT_SPRING )  bIsEnableMatlRC = FALSE;
	if ( m_nElementType == HT_POINT_SPRING )  bIsEnableMatlRC = FALSE;

	BOOL bIsEnableMatlSteel = TRUE;
	if ( m_nElementType == HT_WALL_CRB )  bIsEnableMatlSteel = FALSE;
	if ( m_nElementType == HT_SPRING )  bIsEnableMatlSteel = FALSE;
	if ( m_nElementType == HT_POINT_SPRING )  bIsEnableMatlSteel = FALSE;

	GetDlgItem(IDC_MATL_TYPE_RC_SRC_ENCASED_RDO)->EnableWindow(bIsEnableMatlRC);
	GetDlgItem(IDC_MATL_TYPE_STEEL_SRC_FILLED_RDO)->EnableWindow(bIsEnableMatlSteel);

	if ( m_nMatlType == -1 ) m_nMatlType = 0;
	if ( !bIsEnableMatlSteel ) m_nMatlType = 0;
	if ( !bIsEnableMatlRC ) m_nMatlType = -1;
}

void CIehpItemDlg_MEC::EnableDisableCtrls_RefLoc()
{
	BOOL bEnable = FALSE;
	if ( m_nElementType == HT_BEAM_COLM )
	{
		if ( m_nMatlType == RC_SRC_ENCASED && m_nInteractionType == 0 )
			bEnable = TRUE;
	}

	if ( m_nDefinitionType != MOMENT_CURVATURE )  bEnable = FALSE;

	GetDlgItem(IDC_HING_TYPE_DIST_REF_LOC_BTN)->EnableWindow(bEnable);
	//CDlgUtil::CtrlEnableDisable(this, m_aMembPos, bEnable);
	if ( m_nLocMemb == -1 ) m_nLocMemb = 0;
}

void CIehpItemDlg_MEC::EnableDisableCtrls_DefType()
{
	if (m_nElementType != HT_BEAM_COLM || m_nDefinitionType != MOMENT_CURVATURE)
	{
		m_nFlexStiff = 0;
		GetDlgItem(IDC_HING_TYPE_FLEXIBILITY_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_HING_TYPE_STIFFNESS_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_HING_STIFFNESS_OPTION_BTN)->EnableWindow(FALSE);
	}

	BOOL bEnableMTheta = FALSE;
	if (m_nElementType == HT_BEAM_COLM) bEnableMTheta = TRUE;
	if (m_nElementType == HT_WALL_CRB && m_nNewHingeType == 0) bEnableMTheta = TRUE;

	BOOL bEnableMPhi = FALSE;
	if (m_nElementType == HT_BEAM_COLM) bEnableMPhi = TRUE;
	if (m_nElementType == HT_WALL_CRB && m_nNewHingeType == 1) bEnableMPhi = TRUE;

	GetDlgItem(IDC_HING_TYPE_LUMP_RDO)->EnableWindow(bEnableMTheta);
	GetDlgItem(IDC_HING_TYPE_DIST_RDO)->EnableWindow(bEnableMPhi);

	if (m_nDefinitionType == -1) m_nDefinitionType = 0;
	if (!bEnableMPhi) m_nDefinitionType = 0;
	if (!bEnableMTheta)
	{
		if (bEnableMPhi) m_nDefinitionType = 1;
		else            m_nDefinitionType = -1;
	}


	if (m_nElementType != HT_BEAM_COLM || m_nDefinitionType != MOMENT_CURVATURE)
	{
		m_nFlexStiff = 0;
	}

	GetDlgItem(IDC_HING_TYPE_FLEXIBILITY_RDO)->EnableWindow((m_nElementType == HT_BEAM_COLM && m_nDefinitionType == MOMENT_CURVATURE));
	GetDlgItem(IDC_HING_TYPE_STIFFNESS_RDO)->EnableWindow((m_nElementType == HT_BEAM_COLM && m_nDefinitionType == MOMENT_CURVATURE));
	GetDlgItem(IDC_HING_STIFFNESS_OPTION_BTN)->EnableWindow((m_nDefinitionType == MOMENT_CURVATURE && m_nFlexStiff == 1));
}

void CIehpItemDlg_MEC::EnableDisableCtrls_HingeType()
{
	if ( m_nDefinitionType <= 0 && m_nInteractionType == 3 ) m_nInteractionType = 0;

	BOOL bEnableFiber = FALSE;
	BOOL bWall = FALSE;

	if ( m_nElementType == HT_BEAM_COLM && m_nDefinitionType == 1 ) bEnableFiber = TRUE;
	if ( m_nElementType == HT_WALL_CRB )
	{
		bEnableFiber = TRUE; // Wall인 경우는 M-theta or M-phi 관계없이 활성화
		bWall = TRUE;
	}

	GetDlgItem(IDC_HINGE_TYPE_DEFINE_RDO2)->EnableWindow(bEnableFiber);

	if ( m_nNewHingeType == -1 ) m_nNewHingeType = 0;
	if ( !bEnableFiber ) m_nNewHingeType = 0;

	// sect combo
	BOOL bEnableSect = FALSE;
	if ( m_nNewHingeType == 1 && m_nFiberAutoUser == 1 ) bEnableSect = TRUE;
	GetDlgItem(IDC_SECT_STC)->EnableWindow(bEnableSect);
	GetDlgItem(IDC_SECT_CMB)->EnableWindow(bEnableSect);
	GetDlgItem(IDC_HINGE_TYPE_FIBER_AUTO_RDO)->EnableWindow(m_nNewHingeType == 1);
	GetDlgItem(IDC_HINGE_TYPE_FIBER_USER_RDO)->EnableWindow(m_nNewHingeType == 1);

	// Init conbo
	if ( m_nNewHingeType == 1 )	InitSectComboCtrl();
	else                    m_cmbSect.ResetContent();

	// 텍스트 변경
	GetDlgItem(IDC_HINGE_TYPE_DEFINE_RDO1)->SetWindowText(!bWall ? _LS(IDS_CMD_IEHP_HINGE_TYPE_DEFINE_RDO1_BEAM) : _LS(IDS_CMD_IEHP_HINGE_TYPE_DEFINE_RDO1_WALL));
	GetDlgItem(IDC_HINGE_TYPE_DEFINE_RDO2)->SetWindowText(!bWall ? _LS(IDS_CMD_IEHP_HINGE_TYPE_DEFINE_RDO2_BEAM) : _LS(IDS_CMD_IEHP_HINGE_TYPE_DEFINE_RDO2_WALL));
	GetDlgItem(IDC_SECT_STC)->SetWindowText(!bWall ? _LS(IDS_CMD_IEHP_SECT_TXT) : _LS(IDS_CMD_IEHP_THIK_TXT));
}

void CIehpItemDlg_MEC::EnableDisableCtrls_Interact()
{
	BOOL bEnablePMM = FALSE;
	BOOL bEnablePM = FALSE;
	if ( m_nElementType == HT_BEAM_COLM )
	{
		bEnablePMM = TRUE;    bEnablePM = TRUE;
		if ( m_nNewHingeType == 1 ) { bEnablePMM = FALSE;  bEnablePM = FALSE; }   // m_nNewHingeType : 0=Skeleton, 1=Fiber
	}
	if ( m_nElementType == HT_WALL_CRB )
	{
		bEnablePMM = TRUE;    bEnablePM = TRUE;
		if ( m_nNewHingeType == 1 ) { bEnablePMM = FALSE;  bEnablePM = FALSE; }   // m_nNewHingeType : 0=Skeleton, 1=Fiber
		if ( m_nWallType == 0 || m_nWallType == 1 ) { bEnablePMM = FALSE; }   // m_nWallType : 0=Membrane, 1=Plate
	}

	GetDlgItem(IDC_INTR_PM_RDO)->EnableWindow(bEnablePM);
	GetDlgItem(IDC_INTR_PMM_RDO)->EnableWindow(bEnablePMM);
	GetDlgItem(IDC_INTR_PMMC_RDO)->EnableWindow(bEnablePMM);

#if !defined(_CIVIL)
	// Gen 에서 기능이 막혀있어서 CReadMGB::RB_IEHP() 에서 데이터를 안불러 오도록 처리 되어 있습니다.
	// 기능을 열게 되면 데이터 불러오는 곳도 꼭 확인해야 합니다.
	GetDlgItem(IDC_INTR_PMMC_RDO)->ShowWindow(SW_HIDE);
// #else
// 	BOOL bEnableMPhi = FALSE;
// 	if(m_nDefinitionType==MOMENT_CURVATURE) bEnableMPhi = TRUE;
// 	GetDlgItem(IDC_INTR_PMMC_RDO)->EnableWindow(bEnableMPhi);  
// 	BOOL bEnablePMMC = bEnableMPhi; 
#endif
	//

	if ( m_nInteractionType == -1 ) m_nInteractionType = 0;
	if ( m_nInteractionType == 2 && !bEnablePMM ) m_nInteractionType = 1;
	if ( m_nInteractionType == 1 && !bEnablePM ) m_nInteractionType = 0;
}

// Component 성분 별로 활성/비활성화
// IDC_CMD_DOF_DX_CHK, IDC_CMD_DOF_DY_CHK, IDC_CMD_DOF_DZ_CHK로 활성/비활성 성분을 1차로 구분
void CIehpItemDlg_MEC::EnableDisableCtrls_Comp()
{
	BOOL bEnableComp[6];
	std::fill_n(bEnableComp, 6, TRUE);

	if ( m_nElementType != HT_SPRING && m_nElementType != HT_POINT_SPRING )
	{
		bEnableComp[3] = FALSE;
	}

	if ( m_nElementType == HT_TRUSS )
	{
		bEnableComp[1] = FALSE;
		bEnableComp[2] = FALSE;
		bEnableComp[4] = FALSE;
		bEnableComp[5] = FALSE;
	}
	if ( m_nElementType == HT_BEAM_COLM && m_nNewHingeType == 1 )
	{
		bEnableComp[1] = FALSE;
		bEnableComp[2] = FALSE;
	}
	if ( m_nElementType == HT_WALL_CRB )
	{
		if ( m_nWallType == 0 )
		{
			bEnableComp[1] = FALSE;
			bEnableComp[5] = FALSE;
		}
		if ( m_nWallType == 1 && m_nNewHingeType == 1 )
		{
			bEnableComp[1] = m_bConsOutpl;
			bEnableComp[2] = TRUE;  // PMS 5317-14 : Fiber Wall일 때 Fz는 항상 활성화
			bEnableComp[5] = m_bConsOutpl;
		}
	}

	for ( int i = 0; i < 6; i++ )
	{
		if ( !bEnableComp[i] )
			m_bDof[i] = FALSE;
	}
	UpdateData(FALSE);

	//   //Interaction이 2일때 비활성화지만 체크가 된 상태로 만들기 위해..
	//   if (m_nInteractionType == 2)  
	//   {
	//     bEnableComp[4] = FALSE;
	//     bEnableComp[5] = FALSE; 
	//   }

	GetDlgItem(IDC_CMD_DOF_DY_CHK)->EnableWindow(bEnableComp[1]);
	GetDlgItem(IDC_CMD_DOF_DZ_CHK)->EnableWindow(bEnableComp[2]);
	GetDlgItem(IDC_CMD_DOF_RX_CHK)->EnableWindow(bEnableComp[3]);
	GetDlgItem(IDC_CMD_DOF_RY_CHK)->EnableWindow(bEnableComp[4]);
	GetDlgItem(IDC_CMD_DOF_RZ_CHK)->EnableWindow(bEnableComp[5]);

	EnableDisableCtrlDofDx();
	EnableDisableCtrlDofDy();
	EnableDisableCtrlDofDz();
	EnableDisableCtrlDofRx();
	EnableDisableCtrlDofRy();
	EnableDisableCtrlDofRz();

	// show/hide controls
	if (m_nFlexStiff == 0)
	{
		BOOL bShowHingeLocation = FALSE;
		if (m_nElementType == HT_BEAM_COLM || m_nElementType == HT_WALL_CRB)
		{
			if (m_nDefinitionType == 0)    bShowHingeLocation = TRUE;
		}
		CDlgUtil::CtrlShowHide(this, m_aHingeLocation, bShowHingeLocation);

		BOOL bShowNumOfSect = FALSE;
		if (m_nElementType == HT_BEAM_COLM || m_nElementType == HT_WALL_CRB)
		{
			if (m_nDefinitionType == 1)        bShowNumOfSect = TRUE;
		}
		CDlgUtil::CtrlShowHide(this, m_aNumofSection, bShowNumOfSect);

		CDlgUtil::CtrlShowHide(this, m_aCtrlStiffnessCmb, FALSE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlStiffnessCmb, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aHingeLocation, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aNumofSection, FALSE);
	}
}

void CIehpItemDlg_MEC::EnableDisableCtrls_YldSurf()
{
	BOOL bEnable = FALSE;
	if ( m_nInteractionType == 1 ) bEnable = TRUE;
	if ( m_nInteractionType == 2 ) bEnable = TRUE;
	GetDlgItem(IDC_CMD_PROP_YS_BTN)->EnableWindow(bEnable);
}

void CIehpItemDlg_MEC::EnableDisableCtrls_FiberName()
{
	BOOL bEnable = TRUE;
	if ( m_nElementType != HT_BEAM_COLM && m_nElementType != HT_WALL_CRB ) bEnable = FALSE;
	if ( m_nDefinitionType != 1 ) bEnable = FALSE;
	if ( m_nNewHingeType != 1 ) bEnable = FALSE;
	if ( m_nFiberAutoUser != 1 ) bEnable = FALSE;

	GetDlgItem(IDC_CMD_FIBER_NAME_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_FIBER_NAME_CMB)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_FIBER_NAME_BTN)->EnableWindow(bEnable);
	GetDlgItem(IDC_HINGE_TYPE_FIBER_OUTPLANE_BTN)->EnableWindow(m_nWallType == 1);
	GetDlgItem(IDC_HINGE_TYPE_FIBER_OUTPLANE_BTN)->ShowWindow(m_nElementType == HT_WALL_CRB);

	// PMS 5317-50 : Shear R은 국내출시에서 제외함. 솔버에는 0.5를 넘겨줌.
	BOOL bShowFiberWallShearR = CTestEnvMgr::GetTestEnvST(_LSX(ShowFiberWallShearR)) == _LSX(yes);
	BOOL bWall = m_nElementType == HT_WALL_CRB;
	BOOL bFiber = m_nNewHingeType == 1;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlIDFiberShearR, bFiber);
	CDlgUtil::CtrlShowHide(this, m_aCtrlIDFiberShearR, bWall && bShowFiberWallShearR);

	// Init conbo
	if ( m_nNewHingeType != 1 )	m_cmbFibrName.ResetContent();    // m_nNewHingeType: 0=Skeleton, 1=Fiber
}

void CIehpItemDlg_MEC::EnableDisableCtrls()
{
	UpdateData(TRUE);

	EnableDisableCtrls_MatlType();
	EnableDisableCtrls_WallType();
	EnableDisableCtrls_DefType();
	EnableDisableCtrls_RefLoc();
	EnableDisableCtrls_HingeType();
	EnableDisableCtrls_Interact();
	EnableDisableCtrls_Comp();       // Component 성분 별로 활성/비활성화
	EnableDisableCtrls_YldSurf();
	EnableDisableCtrls_FiberName();

	UpdateData(FALSE);
}

void CIehpItemDlg_MEC::OnStiffnessOptionBtn()
{
	CWnd* pBtn = GetDlgItem(IDC_HING_STIFFNESS_OPTION_BTN); ASSERT(pBtn);
	CRect rectBtn; rectBtn.SetRectEmpty();
	if (pBtn != NULL)
	{
		pBtn->GetWindowRect(&rectBtn);
		rectBtn.left += rectBtn.Width();
	}

	CIehpStiffnessOptionDlg dlg(m_bUseNonlinearShearY, m_bUseNonlinearShearZ);
	dlg.m_rectPosition = rectBtn;
	if (dlg.DoModal() == IDOK)
	{
		m_bUseNonlinearShearY = dlg.m_bUseNonlinearShearY;
		m_bUseNonlinearShearZ = dlg.m_bUseNonlinearShearZ;
	}
}

void CIehpItemDlg_MEC::OnMphiLocBtn()
{
	CWnd* pBtn = GetDlgItem(IDC_HING_TYPE_DIST_REF_LOC_BTN); ASSERT(pBtn);
	CRect rectBtn; rectBtn.SetRectEmpty();
	if ( pBtn != NULL )
	{
		pBtn->GetWindowRect(&rectBtn);
		rectBtn.left += rectBtn.Width();
	}

	CIehpMphiLocDlg dlg(m_nAutoUserLocMemb, m_nLocMemb);
	dlg.m_rectPosition = rectBtn;
	if ( dlg.DoModal() == IDOK )
	{
		m_nAutoUserLocMemb = dlg.m_nAutoUserLocMemb;
		m_nLocMemb = dlg.m_nLocMemb;
	}
}

void CIehpItemDlg_MEC::OnFiberNameBtn()
{
	UpdateData(TRUE);

	T_AGEN_D agent_data;
	int nFiberDlgType = m_pDoc->m_pAttrCtrl2->GetFiberDlgType(D_FIBR_TYPE_IEHP, m_nElementType == HT_BEAM_COLM);
	agent_data.nSubData[0] = nFiberDlgType;
	agent_data.pData = this;
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance(); ASSERT(pServiceHeadOffice);
	pServiceHeadOffice->ReqService(_ULS(gr), D_GR_RUNASSUB_FIBRLISTDLG, (void*)&agent_data);
}

void CIehpItemDlg_MEC::OnFiberOutplBtn()
{
	CWnd* pBtn = GetDlgItem(IDC_HINGE_TYPE_FIBER_OUTPLANE_BTN); ASSERT(pBtn);
	CRect rectBtn; rectBtn.SetRectEmpty();
	if ( pBtn != NULL )
	{
		pBtn->GetWindowRect(&rectBtn);
		rectBtn.left += rectBtn.Width();
	}

	CIehpFiberOutplDlg dlg(m_nAutoUserFiberOutpl, m_bConsOutpl, FALSE);
	dlg.m_rectPosition = rectBtn;
	if ( dlg.DoModal() == IDOK )
	{
		m_nAutoUserFiberOutpl = dlg.m_nUseIehc;

		if ( m_bConsOutpl != dlg.m_bConsOutpl )
		{
			// Fx - Mz 적분점수 맞춰줌
			m_bConsOutpl = dlg.m_bConsOutpl;
			ChangeNumofSectionDx();
		}
		EnableDisableCtrls();
		CtrlInteractionType();
	}
}

void CIehpItemDlg_MEC::OnShearRBtn()
{
	CCMIehcShearRDlg dlg;
	dlg.SetData(m_nAutoUserShearR, m_edtShearR.GetEditValue());
	dlg.SetInitPos(D_INIT_POS_RT, 5, 0);
	if ( dlg.DoModal() == IDOK )
	{
		m_nAutoUserShearR = dlg.GetUseOption();
		m_edtShearR.SetEditUnit(dlg.GetShearRValue());
	}
}
void CIehpItemDlg_MEC::InitCtrl()
{
	m_edtShearR.SetUnitType(D_UNITSYS_NONE);
	m_edtShearR.SetReadOnly(TRUE);
}

void CIehpItemDlg_MEC::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ( (hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd) ) return;

	switch ( lHint )
	{
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	default:
		break;
	}
}

void CIehpItemDlg_MEC::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if ( nCount == 0 ) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while ( pos != NULL )
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch ( nCmd )
		{
		case(UR_FIBR_ADD):
		case(UR_FIBR_DEL):
		case(UR_FIBR_MFS):
		case(UR_FIBR_MFD):
		case(UR_FIBW_ADD):
		case(UR_FIBW_DEL):
		case(UR_FIBW_MFS):
		case(UR_FIBW_MFD):
			{
				// Fiber 콤보박스 업데이트
				T_FIBR_K FibrK_cursel = m_cmbFibrName.GetItemData(m_cmbFibrName.GetCurSel());
				MakeFibrCombo(FALSE);
				m_cmbFibrName.SetCurSel(0);
				for ( int i = 0; i < m_cmbFibrName.GetCount(); i++ )
				{
					if ( m_cmbFibrName.GetItemData(i) == FibrK_cursel )
						m_cmbFibrName.SetCurSel(i);
				}
			}
			break;
		default:
			break;
		}
	}
}

BOOL CIehpItemDlg_MEC::IsHysModel_Unable1stCtrl(int nHysModel)
{
	switch ( nHysModel )
	{
	case D_IEHP_CLOU:
	case D_IEHP_NBIL:
	case D_IEHP_EBIL:
	case D_IEHP_SLPB:
	case D_IEHP_SLBT:
	case D_IEHP_SLBC:
	case D_IEHP_FBIL:
	case D_IEHP_FINF:
	case D_IEHP_FTRI:
		return TRUE;
	}

	return FALSE;
}

void CIehpItemDlg_MEC::InitCtrlTexts()
{
	this->SetWindowText(_LS(IDS_WG_CMD_ADD_MODIFY_INELASTIC_HINGE_PROPERTIES));
	GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowText(_LS(IDS_WG_CMD_NAME));
	GetDlgItem(IDC_WG_CMD_STATIC1)->SetWindowText(_LS(IDS_WG_CMD_DESCRIPTION));
	GetDlgItem(IDC_WG_CMD_STATIC7)->SetWindowText(_LS(IDS_WG_CMD_Element_Type));
	GetDlgItem(IDC_HING_TYPE_BEAM_RDO)->SetWindowText(_LS(IDS_WG_CMD_BEAM_COLUMN));
	GetDlgItem(IDC_HING_TYPE_TRUS_RDO)->SetWindowText(_LS(IDS_WG_CMD_TRUSS));
	GetDlgItem(IDC_HING_TYPE_SUPPORT_RDO)->SetWindowText(_LS(IDS_WG_CMD_POINTSPRING_SUPPORT));
	GetDlgItem(IDC_WG_CMD_STATIC6)->SetWindowText(_LS(IDS_WG_CMD_MATERIAL_TYPE));
	GetDlgItem(IDC_MATL_TYPE_RC_SRC_ENCASED_RDO)->SetWindowText(_LS(IDS_WG_CMD_RC_SRC_ENCASED));
	GetDlgItem(IDC_MATL_TYPE_STEEL_SRC_FILLED_RDO)->SetWindowText(_LS(IDS_WG_CMD_STEEL_SRC_FILLED));
	GetDlgItem(IDC_WG_CMD_STATIC10)->SetWindowText(_LS(IDS_WG_CMD_DEFINITION));
	GetDlgItem(IDC_HING_TYPE_LUMP_RDO)->SetWindowText(_LS(IDS_WG_CMD_MOMENT_ROTATION));
	GetDlgItem(IDC_HING_TYPE_DIST_RDO)->SetWindowText(_LS(IDS_WG_CMD_MOMENT_CURVATURE));
	GetDlgItem(IDC_WG_CMD_STATIC9)->SetWindowText(_LS(IDS_WG_CMD_HINGE_TYPE));
	GetDlgItem(IDC_WG_CMD_STATIC8)->SetWindowText(_LS(IDS_WG_CMD_INTERACTION_TYPE));
	GetDlgItem(IDC_INTR_NONE_RDO)->SetWindowText(_LS(IDS_WG_CMD_NONE));
	GetDlgItem(IDC_INTR_PMM_RDO)->SetWindowText(_LS(IDS_WG_CMD_PM_STATUS_DETERMINE));
	GetDlgItem(IDC_INTR_PMMC_RDO)->SetWindowText(_LS(IDS_WG_CMD_PM_PARAMETRIC_MULTI_CURVE));
	GetDlgItem(IDC_HINGE_SECT_TXT)->SetWindowText(_LS(IDS_WG_CMD_FIBER_SECTION));
	GetDlgItem(IDC_HINGE_TYPE_FIBER_AUTO_RDO)->SetWindowText(_LS(IDS_WG_CMD_AUTO_GENERATION));
	GetDlgItem(IDC_HINGE_TYPE_FIBER_USER_RDO)->SetWindowText(_LS(IDS_WG_CMD_USER_DEFINED));
	GetDlgItem(IDC_SECT_STC)->SetWindowText(_LS(IDS_WG_CMD_SECTION));
	GetDlgItem(IDC_CMD_FIBER_NAME_STC)->SetWindowText(_LS(IDS_WG_CMD_FIBER_NAME));
	GetDlgItem(IDC_WG_CMD_STATIC2)->SetWindowText(_LS(IDS_WG_CMD_COMPONENT_PROPERTIES));
	GetDlgItem(IDC_WG_CMD_STATIC3)->SetWindowText(_LS(IDS_WG_CMD_COMPONENT));
	GetDlgItem(IDC_CMD_HINGE_LOCATION_TEXT)->SetWindowText(_LS(IDS_WG_CMD_HINGE_LOCATION));
	GetDlgItem(IDC_CMD_HINGE_LOCATION_TEXT2)->SetWindowText(_LS(IDS_WG_CMD_NO_OF_SECTIONS));
	GetDlgItem(IDC_CMD_HINGE_LOCATION_TEXT3)->SetWindowText(_LS(IDS_WG_CMD_HINGE_LOCATION));
	GetDlgItem(IDC_HING_TYPE_FLEXIBILITY_RDO)->SetWindowText(_LS(IDS_WG_CMD_HING_TYPE_FLEXIBILITY));
	GetDlgItem(IDC_HING_TYPE_STIFFNESS_RDO)->SetWindowText(_LS(IDS_WG_CMD_HING_TYPE_STIFFNESS));
	GetDlgItem(IDC_IEHP_HYSMODEL_STC)->SetWindowText(_LS(IDS_WG_CMD_HYSTERESIS_MODEL));
	GetDlgItem(IDC_CMD_PROP_DX_BTN)->SetWindowText(_LS(IDS_WG_CMD_PROPERTIES_DOT3));
	GetDlgItem(IDC_CMD_PROP_DY_BTN)->SetWindowText(_LS(IDS_WG_CMD_PROPERTIES_DOT3));
	GetDlgItem(IDC_CMD_PROP_DZ_BTN)->SetWindowText(_LS(IDS_WG_CMD_PROPERTIES_DOT3));
	GetDlgItem(IDC_CMD_PROP_RX_BTN)->SetWindowText(_LS(IDS_WG_CMD_PROPERTIES_DOT3));
	GetDlgItem(IDC_CMD_PROP_RY_BTN)->SetWindowText(_LS(IDS_WG_CMD_PROPERTIES_DOT3));
	GetDlgItem(IDC_CMD_PROP_RZ_BTN)->SetWindowText(_LS(IDS_WG_CMD_PROPERTIES_DOT3));
	GetDlgItem(IDC_CMD_PROP_YS_BTN)->SetWindowText(_LS(IDS_WG_CMD_YIELD_SURFACE_PROPERTIES_DOT3));
	GetDlgItem(IDC_WG_CMD_STATIC53)->SetWindowText(_LS(IDS_WG_CMD_STATIC53));
}

bool CIehpItemDlg_MEC::UpdatePropData(int nDof)
{
	int nSel = m_cmbModel[nDof].GetItemData(m_cmbModel[nDof].GetCurSel());
	int nTabStyle = GetTabStyle(nDof);

	T_IEHP_D data; data.Initialize();

	Dlg2CommonData();
	if (m_bPropMod[nDof]) data = m_Data;

	int nFg = 0;
	if (m_nInteractionType == 0) nFg = nDof;
	else                        nFg = nDof + 3;

	bool bUpdateNeeded = true;
	if (!PropBtnProc(data, nDof, nSel, nFg, nTabStyle, bUpdateNeeded)) return false;

	return true;
}
