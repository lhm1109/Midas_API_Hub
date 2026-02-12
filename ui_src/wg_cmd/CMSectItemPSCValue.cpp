// CMSectItemPSCValue.cpp : implementation file
// 2004. 12. 06   by TAE
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCValue.h"

#include "CMSectItemPSC.h"
#include "CMSectOffsetDlg.h"

#include "CMSectItemPSCImport.h"
#include "CMSectItemPSCValueDBDlg.h"
#include "CMSectItemPSC_WarpingCheckDlg.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\CMSectImportSPC.h"
#include "..\wg_base\wg_base_serviceprovider.h"
#include "..\wg_cmd2\CMD2_ServiceDef.h"

#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCValue dialog

CCMSectItemPSCValue::CCMSectItemPSCValue(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSCValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCValue)
	//}}AFX_DATA_INIT
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC*)pParent;

	m_CurSectID = 0;
	m_aImportID.RemoveAll();
	m_aImportSect.RemoveAll();
	m_nWarpingCheck= 0;
}

CCMSectItemPSCValue::~CCMSectItemPSCValue()
{
	//m_SectData.Initialize();
	//double dTest = 0.0;
}

void CCMSectItemPSCValue::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCValue)
	DDX_Control(pDX, IDC_CMD_SHEAR3_THK_AUTO, m_wndZ3Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR3_THK_UNIT, m_unitZ3Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR3_THK_EDIT, m_editZ3Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR3_Z_CHK, m_wndZ3Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR3_QY_UNIT, m_unitZ3Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR3_QY_EDIT, m_editZ3Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR3_Z_UNIT, m_unitZ3Pos);
	DDX_Control(pDX, IDC_CMD_SHEAR3_Z_EDIT, m_editZ3Pos);
	DDX_Control(pDX, IDC_CMD_SHEAR2_THK_AUTO, m_wndZ2Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR2_THK_UNIT, m_unitZ2Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR2_THK_EDIT, m_editZ2Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR2_Z_AUTO, m_wndZ2Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR2_QY_UNIT, m_unitZ2Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR2_QY_EDIT, m_editZ2Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR1_THK_AUTO, m_wndZ1Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR1_THK_UNIT, m_unitZ1Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR1_THK_EDIT, m_editZ1Thk);
	DDX_Control(pDX, IDC_CMD_SHEAR1_Z_AUTO, m_wndZ1Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR1_QY_UNIT, m_unitZ1Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR1_QY_EDIT, m_editZ1Qy);
	DDX_Control(pDX, IDC_CMD_SHEAR1_Z_UNIT, m_unitZ1Pos);
	DDX_Control(pDX, IDC_CMD_SHEAR1_Z_EDIT, m_editZ1Pos);
	DDX_Control(pDX, IDC_CMD_TORSION_CHK, m_wndTorsion);
	DDX_Control(pDX, IDC_CMD_TORSION_UNIT, m_unitTorsion);
	DDX_Control(pDX, IDC_CMD_TORSION_EDIT, m_editTorsion);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_wndShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_HT_UNIT, m_unitHT);
	DDX_Control(pDX, IDC_CMD_HT_EDIT, m_editHT);
	DDX_Control(pDX, IDC_CMD_BT_UNIT, m_unitBT);
	DDX_Control(pDX, IDC_CMD_BT_EDIT, m_editBT);
	DDX_Control(pDX, IDC_CMD_T2_UNIT2, m_unitT2);
	DDX_Control(pDX, IDC_CMD_T2_EDIT2, m_editT2);
	DDX_Control(pDX, IDC_CMD_T1_UNIT, m_unitT1);
	DDX_Control(pDX, IDC_CMD_T1_EDIT, m_editT1);
	DDX_Control(pDX, IDC_CMD_IMPORT_EDIT, m_editPath);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_IMPORT_BTN, m_btnImport);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	DDX_Control(pDX, IDC_CMD_SP_ID_GRID, m_wndGridStiff);
	//}}AFX_DATA_MAP
}

void CCMSectItemPSCValue::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D*)pData);
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

BOOL CCMSectItemPSCValue::GetDlgData(void* pData)
{
	T_SECT_D* pSect = (T_SECT_D*)pData;

	// peri and stiffness
	m_wndGridStiff.SaveData();    
	m_SectData.SectBefore.SectI.Stiffness.WArea = m_SectData.SectBefore.SectI.Stiffness.Area;
	
	//*pSect = m_SectData;    // Import한 값들을 그대로 저장
	pSect->SectBefore.SectI.SName       = m_SectData.SectBefore.SectI.SName;
	pSect->SectBefore.SectI.aOuterPolygon.Copy(m_SectData.SectBefore.SectI.aOuterPolygon);
	pSect->SectBefore.SectI.aInnerPolygon.Copy(m_SectData.SectBefore.SectI.aInnerPolygon);
	pSect->SectBefore.SectI.Stiffness   = m_SectData.SectBefore.SectI.Stiffness;
	pSect->SectBefore.SectI.PeriIn      = m_SectData.SectBefore.SectI.PeriIn;
	pSect->SectBefore.SectI.PeriOut     = m_SectData.SectBefore.SectI.PeriOut;
	pSect->SectBefore.SectI.Design.YBar = m_SectData.SectBefore.SectI.Design.YBar;
	pSect->SectBefore.SectI.Design.ZBar = m_SectData.SectBefore.SectI.Design.ZBar;

	pSect->nStype = D_SECT_TYPE_PSC;
	pSect->SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;

	// param. for design
	pSect->SectBefore.SectI.Size[0] = m_editHT.GetEditValue();
	pSect->SectBefore.SectI.Size[1] = m_editBT.GetEditValue();
	pSect->SectBefore.SectI.Size[2] = m_editT1.GetEditValue();
	pSect->SectBefore.SectI.Size[3] = m_editT2.GetEditValue();

	// consider shear deform.
	pSect->SectBefore.bConsiderShearDeform = (m_wndShearDeform.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	// warping stress point
	pSect->SectBefore.nWarpingCheck[0] = pSect->SectBefore.nWarpingCheck[1] =m_nWarpingCheck;
	double dWarpingCheckPos[2][6]={0.0};
	memcpy(pSect->SectBefore.dWarpingCheckPosI, m_SectData.SectBefore.dWarpingCheckPosI, sizeof(dWarpingCheckPos)); // I/J같음.
	memcpy(pSect->SectBefore.dWarpingCheckPosJ, m_SectData.SectBefore.dWarpingCheckPosI, sizeof(dWarpingCheckPos));

	// thk. for torsion
	pSect->SectBefore.bAutoWebThk[0] = FALSE;
	pSect->SectBefore.dWebThk[0] = m_editTorsion.GetEditValue();

	// for shear
	pSect->SectBefore.bShearCheck = TRUE;   // 무조건 입력받음 

	pSect->SectBefore.dShearCheckPos[0][0] = m_editZ1Pos.GetEditValue();
	pSect->SectBefore.bAutoShearCheckPos[0][0] = FALSE;
	pSect->SectBefore.dQy[0][0] = m_editZ1Qy.GetEditValue();
	pSect->SectBefore.bAutoQy[0][0] = (m_wndZ1Qy.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.dWebThkShear[0][0] = m_editZ1Thk.GetEditValue();
	pSect->SectBefore.bAutoWebThkShear[0][0] = (m_wndZ1Thk.GetCheck()==1) ? TRUE : FALSE;

	pSect->SectBefore.bAutoShearCheckPos[0][1] = TRUE;
	pSect->SectBefore.dQy[0][1] = m_editZ2Qy.GetEditValue();
	pSect->SectBefore.bAutoQy[0][1] = (m_wndZ2Qy.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.dWebThkShear[0][1] = m_editZ2Thk.GetEditValue();
	pSect->SectBefore.bAutoWebThkShear[0][1] = (m_wndZ2Thk.GetCheck()==1) ? TRUE : FALSE;

	pSect->SectBefore.dShearCheckPos[0][2] = m_editZ3Pos.GetEditValue();
	pSect->SectBefore.bAutoShearCheckPos[0][2] = FALSE;
	pSect->SectBefore.dQy[0][2] = m_editZ3Qy.GetEditValue();
	pSect->SectBefore.bAutoQy[0][2] = (m_wndZ3Qy.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.dWebThkShear[0][2] = m_editZ3Thk.GetEditValue();
	pSect->SectBefore.bAutoWebThkShear[0][2] = (m_wndZ3Thk.GetCheck()==1) ? TRUE : FALSE;

	// offset
	pSect->SectBefore.nOffsetPoint = m_SectData.SectBefore.nOffsetPoint;
	pSect->SectBefore.nOffsetCenter = m_SectData.SectBefore.nOffsetCenter;
	pSect->SectBefore.nUserOffsetRef = m_SectData.SectBefore.nUserOffsetRef;
	pSect->SectBefore.nHorzOffsetOpt = m_SectData.SectBefore.nHorzOffsetOpt;
	if (pSect->SectBefore.nHorzOffsetOpt == 1)
		pSect->SectBefore.dUserDefOffsetY[0] = m_SectData.SectBefore.dUserDefOffsetY[0];
	pSect->SectBefore.nVertOffsetOpt = m_SectData.SectBefore.nVertOffsetOpt;
	if (pSect->SectBefore.nVertOffsetOpt == 1)
		pSect->SectBefore.dUserDefOffsetZ[0] = m_SectData.SectBefore.dUserDefOffsetZ[0];

	return TRUE;
}

CCMSectItmePSCView* CCMSectItemPSCValue::GetViewPointer() 
{ 
	if (m_pParent == 0) return 0;
	return m_pParent->GetViewPointer(); 
}

void CCMSectItemPSCValue::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
}

void CCMSectItemPSCValue::SetAutotShearCheckPos(double dShearPos[3])
{
	m_SectData.SectBefore.dShearCheckPos[0][1] = dShearPos[1];
	RedrawSection();
}

void CCMSectItemPSCValue::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	if (m_wndZ1Thk.GetCheck() == 1) m_editZ1Thk.SetEditUnit(dShear[0]);
	if (m_wndZ2Thk.GetCheck() == 1) m_editZ2Thk.SetEditUnit(dShear[1]);
	if (m_wndZ3Thk.GetCheck() == 1) m_editZ3Thk.SetEditUnit(dShear[2]);
}

void CCMSectItemPSCValue::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_SectData.SectBefore.dWarpingCheckPosI[i][j] = dWarpingCheckPosI[i][j];
			m_SectData.SectBefore.dWarpingCheckPosJ[i][j] = dWarpingCheckPosJ[i][j];
		}
	}
}

void CCMSectItemPSCValue::OnAfterCalcPeriAndStiff(T_SECT_SECTION_D* pSectBefore)
{
	if (pSectBefore == 0) { ASSERT(0); return; }

	if (m_wndZ1Qy.GetCheck() == 1) m_editZ1Qy.SetEditUnit(pSectBefore->dQy[0][0]);
	if (m_wndZ2Qy.GetCheck() == 1) m_editZ2Qy.SetEditUnit(pSectBefore->dQy[0][1]);
	if (m_wndZ3Qy.GetCheck() == 1) m_editZ3Qy.SetEditUnit(pSectBefore->dQy[0][2]);
}

BEGIN_MESSAGE_MAP(CCMSectItemPSCValue, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSCValue)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_IMPORT_BTN, OnCmdImportBtn)
	ON_BN_CLICKED(IDC_CMD_DEFINE_BTN, OnCmdDefineBtn)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_Z_AUTO, OnCmdShear1QyAuto)
	ON_BN_CLICKED(IDC_CMD_SHEAR2_Z_AUTO, OnCmdShear2QyAuto)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_Z_CHK, OnCmdShear3QyAuto)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_THK_AUTO, OnCmdShear1ThkAuto)
	ON_BN_CLICKED(IDC_CMD_SHEAR2_THK_AUTO, OnCmdShear2ThkAuto)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_THK_AUTO, OnCmdShear3ThkAuto)
	ON_BN_CLICKED(IDC_CMD_SEL_BTN, OnCmdSelectBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_Z_EDIT, OnChangeCmdShearEdit)
	ON_EN_CHANGE(IDC_CMD_SHEAR3_Z_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_CHK,  OnChkWarpingEffect)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_BTN,       OnCmdSectWapingCheckBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_AUTO_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_USER_RDO,  OnCmdSectWapingCheckUser)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCValue message handlers
BOOL CCMSectItemPSCValue::OnInitDialog() 
{
	CCMSectItemPSCBase::OnInitDialog();
	// TODO: Add extra initialization here	

	InitControls();

	ShowData();

	// Update Viewer
	RedrawSection();
	ChangeBitmap(__SECT_PSC_VALUE__);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemPSCValue::InitControls()
{
	// stiff grid
	m_wndGridStiff.InitGrid();
	m_wndGridStiff.SetDataSource(this,&m_SectData.SectBefore.SectI);

	// section view
	if (m_pParent) 
	{
		m_pParent->SetDataSource(&m_SectData);
		m_pParent->DisplayShearCheck();
	}

	// editunit, textunit
	m_unitHT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitBT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editBT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitT2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editT2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitT1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editT1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitZ3Thk.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editZ3Thk.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitZ3Qy.SetUnitType(D_UNITSYS_BASE_VOLUME);
	m_editZ3Qy.SetUnitType(D_UNITSYS_BASE_VOLUME);
	m_unitZ3Pos.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editZ3Pos.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitZ2Thk.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editZ2Thk.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitZ2Qy.SetUnitType(D_UNITSYS_BASE_VOLUME);
	m_editZ2Qy.SetUnitType(D_UNITSYS_BASE_VOLUME);
	m_unitZ1Thk.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editZ1Thk.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitZ1Qy.SetUnitType(D_UNITSYS_BASE_VOLUME);
	m_editZ1Qy.SetUnitType(D_UNITSYS_BASE_VOLUME);
	m_unitZ1Pos.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editZ1Pos.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitTorsion.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editTorsion.SetUnitType(D_UNITSYS_BASE_LENGTH);

	// disable control - not used
	m_editPath.SetReadOnly(TRUE);
	GetDlgItem(IDC_CMD_SEL_BTN)->EnableWindow(FALSE);
	m_wndTorsion.EnableWindow(FALSE);

	if (m_pParent)
	{
		BOOL bShow = m_bModify && (m_SectData.SectBefore.SectI.aOuterPolygon.GetSize() > 0);
		m_pParent->ShowSectView(bShow);
	}

	m_btnImport.ResetContent();
	m_btnImport.AddItem(_LS(IDS_CMD_DB__), 0);
	m_btnImport.AddItem(_LS(IDS_CMD_IMPORT_SPC__), 1);//_T(">>") + csName, nItemType);
	m_btnImport.MakeMenu();

	m_btnImport.SetWindowText(_LS(IDS_CMD_SECTION_DATA));
}

void CCMSectItemPSCValue::ShowData()
{
	// stiffness
	m_wndGridStiff.SetWarpingEffect(m_SectData.SectBefore.bConsiderWarpingEffect);
	m_wndGridStiff.ShowData();

	// param. for design
	m_editHT.SetEditUnit(m_SectData.SectBefore.SectI.Size[0]);
	m_editBT.SetEditUnit(m_SectData.SectBefore.SectI.Size[1]);
	m_editT1.SetEditUnit(m_SectData.SectBefore.SectI.Size[2]);
	m_editT2.SetEditUnit(m_SectData.SectBefore.SectI.Size[3]);

	// consider shear deform.
	m_wndShearDeform.SetCheck((int)m_SectData.SectBefore.bConsiderShearDeform);
	m_chkWarpingEffect.SetCheck((int)m_SectData.SectBefore.bConsiderWarpingEffect);

	// warping stress point
	m_nWarpingCheck = m_SectData.SectBefore.nWarpingCheck[0];
	if (!CSectDB::IsEnable7thDOF())
	{
		m_chkWarpingEffect.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->ShowWindow(SW_HIDE);
	}
	else
	{
		BOOL bChkWE = m_chkWarpingEffect.GetCheck();
		GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);

		if(m_nWarpingCheck==0) // Auto
		{
			GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(TRUE);
		}
	}

	// thk. for torsion
	m_editTorsion.SetEditUnit(m_SectData.SectBefore.dWebThk[0]);

	// for shear
	m_editZ1Pos.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][0]);	
	m_wndZ1Qy.SetCheck(m_SectData.SectBefore.bAutoQy[0][0]);
	m_editZ1Qy.SetEditUnit(m_SectData.SectBefore.dQy[0][0]);
	m_editZ1Qy.EnableWindow(!m_SectData.SectBefore.bAutoQy[0][0]);
	m_wndZ1Thk.SetCheck(m_SectData.SectBefore.bAutoWebThkShear[0][0]);
	m_editZ1Thk.SetEditUnit(m_SectData.SectBefore.dWebThkShear[0][0]);
	m_editZ1Thk.EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][0]);
 
	m_wndZ2Qy.SetCheck(m_SectData.SectBefore.bAutoQy[0][1]);
	m_editZ2Qy.SetEditUnit(m_SectData.SectBefore.dQy[0][1]);
	m_editZ2Qy.EnableWindow(!m_SectData.SectBefore.bAutoQy[0][1]);
	m_wndZ2Thk.SetCheck(m_SectData.SectBefore.bAutoWebThkShear[0][1]);
	m_editZ2Thk.SetEditUnit(m_SectData.SectBefore.dWebThkShear[0][1]);
	m_editZ2Thk.EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][1]);

	m_editZ3Pos.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][2]);
	m_wndZ3Qy.SetCheck(m_SectData.SectBefore.bAutoQy[0][2]);
	m_editZ3Qy.SetEditUnit(m_SectData.SectBefore.dQy[0][2]);
	m_editZ3Qy.EnableWindow(!m_SectData.SectBefore.bAutoQy[0][2]);
	m_wndZ3Thk.SetCheck(m_SectData.SectBefore.bAutoWebThkShear[0][2]);
	m_editZ3Thk.SetEditUnit(m_SectData.SectBefore.dWebThkShear[0][2]);
	m_editZ3Thk.EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][2]);

	// offset
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_SectData.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_SectData.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}
}

BOOL CCMSectItemPSCValue::DestroyWindow() 
{
	m_SectData.Initialize();
	if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
	return CCMSectItemPSCBase::DestroyWindow();
}

void CCMSectItemPSCValue::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	SaveDataForDrawSection();
	/*
	if (!CSectUtil::CalcSectData(m_SectData))
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}
	*/
	T_SECT_SECTION_D* pSectBefore = &m_SectData.SectBefore;
	pSectBefore->dShearCheckPos[0][1] = pSectBefore->SectI.Design.ZBar;
	SetAutotShearCheckPos(pSectBefore->dShearCheckPos[0]);
	SetAutoMinWebThick(pSectBefore->dWebThkShear[0], pSectBefore->dWebThk[0]);

	ASSERT(m_pParent);
	m_pParent->DisplayCentoid();
}

void CCMSectItemPSCValue::OnCmdChangeOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	T_SECT_SECTION_D OldData;
	OldData = m_SectData.SectBefore;

	CCMSectOffsetDlg dlg(&m_SectData.SectBefore, FALSE, m_pParent);
	if (dlg.DoModal() == IDOK)
	{
		CStringArray arOffsetNameList;
		CSectUtil::GetOffsetNameList(arOffsetNameList);
		if (m_SectData.SectBefore.nOffsetPoint >= 1)
		{
			CString csOffset = arOffsetNameList[m_SectData.SectBefore.nOffsetPoint-1];
			m_txtOffset.SetWindowText(csOffset);
		}
	}	
	else m_SectData.SectBefore = OldData;

	RedrawSection();
}

void CCMSectItemPSCValue::SaveDataForDrawSection()
{
	T_SECT_D* pSect = &m_SectData;

	pSect->nStype = D_SECT_TYPE_PSC;
	pSect->SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;

	// peri and stiffness
	m_wndGridStiff.SaveData();
	pSect->SectBefore.SectI.Stiffness.WArea = pSect->SectBefore.SectI.Stiffness.Area;

	// param. for design
	pSect->SectBefore.SectI.Size[0] = m_editHT.GetEditValue();
	pSect->SectBefore.SectI.Size[1] = m_editBT.GetEditValue();
	pSect->SectBefore.SectI.Size[2] = m_editT1.GetEditValue();
	pSect->SectBefore.SectI.Size[3] = m_editT2.GetEditValue();

	// consider shear deform.
	pSect->SectBefore.bConsiderShearDeform = (m_wndShearDeform.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	// warping stress point
	pSect->SectBefore.nWarpingCheck[0] =m_nWarpingCheck;
	pSect->SectBefore.nWarpingCheck[1] =m_nWarpingCheck;

	// thk. for torsion
	pSect->SectBefore.bAutoWebThk[0] = FALSE;
	pSect->SectBefore.dWebThk[0] = m_editTorsion.GetEditValue();

	// for shear
	pSect->SectBefore.bShearCheck = TRUE;   // 무조건 입력받음 

	pSect->SectBefore.dShearCheckPos[0][0] = m_editZ1Pos.GetEditValue();
	pSect->SectBefore.bAutoShearCheckPos[0][0] = FALSE;
	pSect->SectBefore.dQy[0][0] = m_editZ1Qy.GetEditValue();
	pSect->SectBefore.bAutoQy[0][0] = (m_wndZ1Qy.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.dWebThkShear[0][0] = m_editZ1Thk.GetEditValue();
	pSect->SectBefore.bAutoWebThkShear[0][0] = (m_wndZ1Thk.GetCheck()==1) ? TRUE : FALSE;

	pSect->SectBefore.bAutoShearCheckPos[0][1] = TRUE;
	pSect->SectBefore.dQy[0][1] = m_editZ2Qy.GetEditValue();
	pSect->SectBefore.bAutoQy[0][1] = (m_wndZ2Qy.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.dWebThkShear[0][1] = m_editZ2Thk.GetEditValue();
	pSect->SectBefore.bAutoWebThkShear[0][1] = (m_wndZ2Thk.GetCheck()==1) ? TRUE : FALSE;

	pSect->SectBefore.dShearCheckPos[0][2] = m_editZ3Pos.GetEditValue();
	pSect->SectBefore.bAutoShearCheckPos[0][2] = FALSE;
	pSect->SectBefore.dQy[0][2] = m_editZ3Qy.GetEditValue();
	pSect->SectBefore.bAutoQy[0][2] = (m_wndZ3Qy.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.dWebThkShear[0][2] = m_editZ3Thk.GetEditValue();
	pSect->SectBefore.bAutoWebThkShear[0][2] = (m_wndZ3Thk.GetCheck()==1) ? TRUE : FALSE;

	// offset
	pSect->SectBefore.nOffsetPoint = m_SectData.SectBefore.nOffsetPoint;
	pSect->SectBefore.nOffsetCenter = m_SectData.SectBefore.nOffsetCenter;
	pSect->SectBefore.nUserOffsetRef = m_SectData.SectBefore.nUserOffsetRef;
	pSect->SectBefore.nHorzOffsetOpt = m_SectData.SectBefore.nHorzOffsetOpt;
	if (pSect->SectBefore.nHorzOffsetOpt == 1)
		pSect->SectBefore.dUserDefOffsetY[0] = m_SectData.SectBefore.dUserDefOffsetY[0];
	pSect->SectBefore.nVertOffsetOpt = m_SectData.SectBefore.nVertOffsetOpt;
	if (pSect->SectBefore.nVertOffsetOpt == 1)
		pSect->SectBefore.dUserDefOffsetZ[0] = m_SectData.SectBefore.dUserDefOffsetZ[0];
}

void CCMSectItemPSCValue::ResetData()
{
	m_SectData.SectBefore.bAutoWebThkShear[0][0] = TRUE;
	m_SectData.SectBefore.bAutoWebThkShear[0][1] = TRUE;
	m_SectData.SectBefore.bAutoWebThkShear[0][2] = TRUE;
	m_SectData.SectBefore.bAutoWebThkShear[1][0] = TRUE;
	m_SectData.SectBefore.bAutoWebThkShear[1][1] = TRUE;
	m_SectData.SectBefore.bAutoWebThkShear[1][2] = TRUE; 

	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;

	double dQy=0., dQz=0.;
	if (CSectUtil::GetPscValueDataQ(&m_SectData, dQy, dQz))
	{
		m_SectData.SectBefore.SectI.Stiffness.Qyb = dQy;
		m_SectData.SectBefore.SectI.Stiffness.Qzb = dQz;
	}
	m_SectData.SectBefore.bAutoWebThkShear[0][0] = FALSE;
	m_SectData.SectBefore.bAutoWebThkShear[0][1] = FALSE;
	m_SectData.SectBefore.bAutoWebThkShear[0][2] = FALSE;
	m_SectData.SectBefore.bAutoWebThkShear[1][0] = FALSE;
	m_SectData.SectBefore.bAutoWebThkShear[1][1] = FALSE;
	m_SectData.SectBefore.bAutoWebThkShear[1][2] = FALSE; 


	if (m_pParent) 
	{
		m_pParent->SetIDName(/*m_CurSectID, */m_SectData.SName);
		m_pParent->ShowSectView(TRUE);
	}

	ShowData();
	RedrawSection();
}
///////////////////////////////////////////////////////////////////////////////
// 
// Update Viewer
//
BOOL CCMSectItemPSCValue::ChangeBitmap(int nIndex)
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(nIndex);
	return TRUE;
}

void CCMSectItemPSCValue::RedrawSection()
{
	ASSERT(m_pParent);
	SaveDataForDrawSection();
	m_pParent->RedrawSection();
}

void CCMSectItemPSCValue::OnCmdImportBtn() 
{
	UINT nItemType = m_btnImport.GetSelMenuID();
	if(nItemType==0) // DB
	{
	  CCMSectItemPSCValueDBDlg DBDlg;
		if(DBDlg.DoModal()!=IDOK) return;
		
		if(DBDlg.m_nCodeForLog==0) IUsageCounter::Use(_T("SECTDBUK"));
		else if(DBDlg.m_nCodeForLog==1) IUsageCounter::Use(_T("SECTDBIT"));

		T_SECT_D tempSect; tempSect.Initialize();
		tempSect = DBDlg.GetData();

		// 전체 복사를 하니 중국에서 특정 변수(tempSect.SectAfter.SectI.DBName) 메모리가 깨져서 문제가 됨;;
		m_SectData.nStype     = tempSect.nStype;
		m_SectData.SName      = tempSect.SName;
		m_SectData.SectBefore = tempSect.SectBefore;
		m_SectData.bHSQType     = tempSect.bHSQType;
		m_SectData.dCoatingThik = tempSect.dCoatingThik;
		m_SectData.nFormingType = tempSect.nFormingType;

		ResetData();
	}
	else // import SPC
	{
		CString strSec;
		if (CProduct::IsRusLocal())
			strSec = _T("Section Files(*.sec)|*.sec|All File(*.*)|*.*||");
		else
			strSec = _T("MIDAS Section Files(*.sec)|*.sec|All File(*.*)|*.*||");

		CString csFileName;
		CFileDialog fd(TRUE, _T(".sec"), NULL,
									 OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
									 strSec, NULL);
		if (fd.DoModal() != IDOK) return;

		CWaitCursor wait;
		CCMSectImportSPC Importer;
		if (!Importer.Import(fd.GetPathName()))
		{
			AfxMessageBox(_LS(IDS_CMD_SECT_CAN_NOT_IMPORT));
			return;
		}
		Importer.GetList(m_aImportID, m_aImportSect);
		m_editPath.SetWindowText(fd.GetPathName());
		m_editPath.SetFocus();
		m_editPath.SetSel(0, -1);
		GetDlgItem(IDC_CMD_SEL_BTN)->EnableWindow(TRUE);
		OnCmdSelectBtn();

		if (CSectDB::IsEnable7thDOF())
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
			// [경고] SPC에서 import한 단면은 7자유도 단면 속성이 자동 계산되지 않습니다.
			pDoc->DisplayHistoryMessage(_LS(IDS_WG_CMD__ADDD__Warning_Cannot_auto_calc_7thDOF_because_from_SPC));
		}  
	}
}

void CCMSectItemPSCValue::OnCmdDefineBtn() 
{
	BOOL bDesignSect = FALSE;
	T_AGEN_D agen_data;
	T_SECT_D tempSect;
	tempSect.Initialize();
	GetDlgData(&tempSect);
	agen_data.nDlgID = D_CMD2_DO_MODAL_CMSECTDEFDLG;
	agen_data.pData = &tempSect;


	//WG_CMD2에 있는 CCSectPSCDef 콜.
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance(); 
	ASSERT(pServiceHeadOffice);
	int nResult = pServiceHeadOffice->ReqService(_ULS(cmd2), D_CMD2_DO_MODAL, (void*)&agen_data);
	if(nResult==1) 
	{
		BOOL   bAutoWebThkShear[2][3];
		memcpy(bAutoWebThkShear, m_SectData.SectBefore.bAutoWebThkShear, sizeof(bAutoWebThkShear));

		//////////////////////////////////////////////////////////////////////////
		// CCSectPSCDef::OnOK() 내용만 복사.
		// 전체 복사를 하니 중국에서 특정 변수(tempSect.SectAfter.SectI.DBName) 메모리가 깨져서 문제가 됨;;
		m_SectData.SectBefore.Shape            = tempSect.SectBefore.Shape;
		m_SectData.SectBefore.SectI.PeriIn     = tempSect.SectBefore.SectI.PeriIn;
		m_SectData.SectBefore.SectI.PeriOut    = tempSect.SectBefore.SectI.PeriOut;
		m_SectData.SectBefore.SectI.Stiffness  = tempSect.SectBefore.SectI.Stiffness;
		m_SectData.SectBefore.SectI.Design     = tempSect.SectBefore.SectI.Design;
		m_SectData.SectBefore.SectI.aOuterPolygon.Copy(tempSect.SectBefore.SectI.aOuterPolygon);
		m_SectData.SectBefore.SectI.aInnerPolygon.Copy(tempSect.SectBefore.SectI.aInnerPolygon);
		//////////////////////////////////////////////////////////////////////////

		m_SectData.SectBefore.bAutoWebThkShear[0][0] = TRUE;
		m_SectData.SectBefore.bAutoWebThkShear[0][1] = TRUE;
		m_SectData.SectBefore.bAutoWebThkShear[0][2] = TRUE;
		m_SectData.SectBefore.bAutoWebThkShear[1][0] = TRUE;
		m_SectData.SectBefore.bAutoWebThkShear[1][1] = TRUE;
		m_SectData.SectBefore.bAutoWebThkShear[1][2] = TRUE;

		double dQy, dQz;
		if (CSectUtil::GetPscValueDataQ(&m_SectData, dQy, dQz))
		{
			m_SectData.SectBefore.SectI.Stiffness.Qyb = dQy;
			m_SectData.SectBefore.SectI.Stiffness.Qzb = dQz;
		}

		memcpy(m_SectData.SectBefore.bAutoWebThkShear, bAutoWebThkShear, sizeof(bAutoWebThkShear));

		if (m_pParent) 
		{
			m_pParent->SetIDName(/*m_CurSectID, */m_SectData.SName);
			m_pParent->ShowSectView(TRUE);
		}
		ShowData();
		RedrawSection();
	}

}
void CCMSectItemPSCValue::OnCmdSelectBtn() 
{
	// TODO: Add your control notification handler code here  
	CCMSectItemPSCImport dlg;
	dlg.SetData(m_aImportID, m_aImportSect, m_CurSectID);
	if (dlg.DoModal() == IDOK)
	{
		T_SECT_D tempSect; tempSect.Initialize();

		dlg.GetData(m_CurSectID, tempSect);
		// 전체 복사를 하니 중국에서 특정 변수(tempSect.SectAfter.SectI.DBName) 메모리가 깨져서 문제가 됨;;
		m_SectData.nStype = tempSect.nStype;
		m_SectData.SName = tempSect.SName;
		m_SectData.SectBefore = tempSect.SectBefore;
		ResetData();

		T_SECT_D SectTmp = m_SectData;
		if(!CSectUtil::IsMeshOK(SectTmp))
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning_mesh_NG_ImportFromSPC));
	}
}

void CCMSectItemPSCValue::OnCmdShear1QyAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndZ1Qy.GetCheck();
	m_editZ1Qy.EnableWindow(nCheck == 0);
}

void CCMSectItemPSCValue::OnCmdShear2QyAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndZ2Qy.GetCheck();
	m_editZ2Qy.EnableWindow(nCheck == 0);
}

void CCMSectItemPSCValue::OnCmdShear3QyAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndZ3Qy.GetCheck();
	m_editZ3Qy.EnableWindow(nCheck == 0);
}

void CCMSectItemPSCValue::OnCmdShear1ThkAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndZ1Thk.GetCheck();
	m_editZ1Thk.EnableWindow(nCheck == 0);
}

void CCMSectItemPSCValue::OnCmdShear2ThkAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndZ2Thk.GetCheck();
	m_editZ2Thk.EnableWindow(nCheck == 0);
}

void CCMSectItemPSCValue::OnCmdShear3ThkAuto() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_wndZ3Thk.GetCheck();
	m_editZ3Thk.EnableWindow(nCheck == 0);
}

void CCMSectItemPSCValue::OnChangeCmdShearEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	RedrawSection();
}
void CCMSectItemPSCValue::OnCalculateButton()
{
	ASSERT(m_pParent);
	if(m_pParent==0)return;
	m_pParent->OnCalculateButton();
}
void CCMSectItemPSCValue::OnDisplayCentroldButton()
{
	OnCmdSectCentroidBtn();
}
void CCMSectItemPSCValue::ExternalShowData()
{
	ShowData();
}

void CCMSectItemPSCValue::OnChkWarpingEffect()
{
	BOOL bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();
	m_SectData.SectBefore.bConsiderWarpingEffect = bConsiderWarpingEffect;
	m_wndGridStiff.SetWarpingEffect(bConsiderWarpingEffect);

	OnCmdSectWapingCheck();

}


void CCMSectItemPSCValue::OnCmdSectWapingCheckBtn() 
{
	CCMSectItemPSC_WarpingCheckDlg WapingChkDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_I Cell;
	Cell.Set(*sect,TRUE,TRUE);
	WapingChkDlg.SetData(sect,this);
	if(WapingChkDlg.DoModal()!=IDOK)
	{
		//    Cell.Get(*sect,TRUE,TRUE);
	}	
}

void CCMSectItemPSCValue::OnCmdSectWapingCheckUser()
{
	UpdateData(TRUE);
	m_SectData.SectBefore.nWarpingCheck[0] =m_nWarpingCheck;
	m_SectData.SectBefore.nWarpingCheck[1] =m_nWarpingCheck;


	if(m_nWarpingCheck==0) // Auto  
	{
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN)->EnableWindow(TRUE);
	}

}

void CCMSectItemPSCValue::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}
