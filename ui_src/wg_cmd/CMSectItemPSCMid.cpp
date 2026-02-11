// CMSectItemPSCMid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCMid.h"

#include "CMSectItemPSC.h"
#include "CMSectOffsetDlg.h"
#include "CMSectItemPSCMidGridDlg.h"
#include "CMSectItemPSC_WarpingCheckDlg.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMid dialog

CCMSectItemPSCMid::CCMSectItemPSCMid(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSCMid::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCMid)
	m_bJ1 = FALSE;
	m_bJL1 = FALSE;
	m_bJL2 = FALSE;
	m_bJL3 = FALSE;
	m_bJL4 = FALSE;
	m_bJR2 = FALSE;
	m_bJR1 = FALSE;
	m_bJR3 = FALSE;
	m_bJR4 = FALSE;
	m_bSymmetry = TRUE;
	//}}AFX_DATA_INIT
	m_nWarpingCheck= 0;

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	// Joint Group
	arJ1Group.Add(IDC_CMD_H1_EDIT);
	arJL1Group.Add(IDC_CMD_HL21_EDIT); arJL1Group.Add(IDC_CMD_BL21_EDIT);
	arJL2Group.Add(IDC_CMD_HL22_EDIT); arJL2Group.Add(IDC_CMD_BL22_EDIT); 
	arJL3Group.Add(IDC_CMD_HL42_EDIT); arJL3Group.Add(IDC_CMD_BL42_EDIT);
	arJL4Group.Add(IDC_CMD_HL41_EDIT); arJL4Group.Add(IDC_CMD_BL41_EDIT);
	arJR1Group.Add(IDC_CMD_HR21_EDIT); arJR1Group.Add(IDC_CMD_BR21_EDIT); 
	arJR2Group.Add(IDC_CMD_HR22_EDIT); arJR2Group.Add(IDC_CMD_BR22_EDIT);
	arJR3Group.Add(IDC_CMD_HR42_EDIT); arJR3Group.Add(IDC_CMD_BR42_EDIT);
	arJR4Group.Add(IDC_CMD_HR41_EDIT); arJR4Group.Add(IDC_CMD_BR41_EDIT);

	// Cell Type Group
	arRhtComnGroup.Add(IDC_CMD_JR1_CHECK); arRhtComnGroup.Add(IDC_CMD_JR2_CHECK);
	arRhtComnGroup.Add(IDC_CMD_JR3_CHECK); arRhtComnGroup.Add(IDC_CMD_JR4_CHECK);
	arRhtCircGroup.Add(IDC_CMD_HR2_EDIT);  arRhtCircGroup.Add(IDC_CMD_HR4_EDIT);
	arRhtCircGroup.Add(IDC_CMD_BR1_EDIT);  
	arRhtNoneGroup.Add(IDC_CMD_HR2_EDIT);
	arRhtNoneGroup.Add(IDC_CMD_HR3_EDIT);  arRhtNoneGroup.Add(IDC_CMD_HR4_EDIT);
	arRhtNoneGroup.Add(IDC_CMD_HR5_EDIT);  arRhtNoneGroup.Add(IDC_CMD_BR1_EDIT);
	arLftComnGroup.Add(IDC_CMD_JL1_CHECK); arLftComnGroup.Add(IDC_CMD_JL2_CHECK);
	arLftComnGroup.Add(IDC_CMD_JL3_CHECK); arLftComnGroup.Add(IDC_CMD_JL4_CHECK);
	arLftCircGroup.Add(IDC_CMD_HL2_EDIT);  arLftCircGroup.Add(IDC_CMD_HL4_EDIT);
	arLftCircGroup.Add(IDC_CMD_BL1_EDIT);  arLftNoneGroup.Add(IDC_CMD_HL2_EDIT);
	arLftNoneGroup.Add(IDC_CMD_HL3_EDIT);  arLftNoneGroup.Add(IDC_CMD_HL4_EDIT);
	arLftNoneGroup.Add(IDC_CMD_HL5_EDIT);  arLftNoneGroup.Add(IDC_CMD_BL1_EDIT);

	// Symmetry Group
	arSymmetryGroup1.Add(IDC_CMD_JR1_CHECK); arSymmetryGroup1.Add(IDC_CMD_JR2_CHECK);
	arSymmetryGroup1.Add(IDC_CMD_JR3_CHECK); arSymmetryGroup1.Add(IDC_CMD_JR4_CHECK);
	arSymmetryGroup2.Add(IDC_CMD_HR1_EDIT);  arSymmetryGroup2.Add(IDC_CMD_HR2_EDIT);
	arSymmetryGroup2.Add(IDC_CMD_HR21_EDIT); arSymmetryGroup2.Add(IDC_CMD_HR22_EDIT);
	arSymmetryGroup2.Add(IDC_CMD_HR3_EDIT);  arSymmetryGroup2.Add(IDC_CMD_HR4_EDIT);
	arSymmetryGroup2.Add(IDC_CMD_HR41_EDIT); arSymmetryGroup2.Add(IDC_CMD_HR42_EDIT);
	arSymmetryGroup2.Add(IDC_CMD_HR5_EDIT);  arSymmetryGroup2.Add(IDC_CMD_BR1_EDIT);
	arSymmetryGroup2.Add(IDC_CMD_BR2_EDIT);  arSymmetryGroup2.Add(IDC_CMD_BR21_EDIT);
	arSymmetryGroup2.Add(IDC_CMD_BR22_EDIT); arSymmetryGroup2.Add(IDC_CMD_BR41_EDIT);
	arSymmetryGroup2.Add(IDC_CMD_BR42_EDIT); //arSymmetryGroup1.Add(IDC_CMD_TYPE_RGHT_CB);
}


void CCMSectItemPSCMid::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCMid)
	DDX_Control(pDX, IDC_CMD_WEB_I_CHECK, m_chkWebI);
	DDX_Control(pDX, IDC_CMD_SHEAR3_CHECK, m_chkShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR1_CHECK, m_chkShear1);
	DDX_Control(pDX, IDC_CMD_WEB_I_UNIT, m_unitWebI);
	DDX_Control(pDX, IDC_CMD_WEB_I_EDIT, m_editWebI);
	DDX_Control(pDX, IDC_CMD_SHEAR3_UNIT, m_unitShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR3_EDIT, m_editShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR2_UNIT, m_unitShear2);
	DDX_Control(pDX, IDC_CMD_SHEAR2_EDIT, m_editShear2);
	DDX_Control(pDX, IDC_CMD_SHEAR1_UNIT, m_unitShear1);
	DDX_Control(pDX, IDC_CMD_SHEAR1_EDIT, m_editShear1);
	DDX_Control(pDX, IDC_CMD_SHEAR_CHECK, m_chkShearCheck);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_TYPE_RGHT_CB, m_cRghtCombo);
	DDX_Control(pDX, IDC_CMD_TYPE_LEFT_CB, m_cLeftCombo);
	DDX_Check(pDX, IDC_CMD_J1_CHECK, m_bJ1);
	DDX_Check(pDX, IDC_CMD_JL1_CHECK, m_bJL1);
	DDX_Check(pDX, IDC_CMD_JL2_CHECK, m_bJL2);
	DDX_Check(pDX, IDC_CMD_JL3_CHECK, m_bJL3);
	DDX_Check(pDX, IDC_CMD_JL4_CHECK, m_bJL4);
	DDX_Check(pDX, IDC_CMD_JR2_CHECK, m_bJR2);
	DDX_Check(pDX, IDC_CMD_JR1_CHECK, m_bJR1);
	DDX_Check(pDX, IDC_CMD_JR3_CHECK, m_bJR3);
	DDX_Check(pDX, IDC_CMD_JR4_CHECK, m_bJR4);
	DDX_Check(pDX, IDC_SYMMETRY_CHECK, m_bSymmetry);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	//}}AFX_DATA_MAP

	CArray<UINT, UINT> arUnitIDs;
	// Left
	arUnitIDs.Add(IDC_CMD_H1_UNIT);   arUnitIDs.Add(IDC_CMD_HL1_UNIT);
	arUnitIDs.Add(IDC_CMD_HL2_UNIT);  arUnitIDs.Add(IDC_CMD_HL21_UNIT);
	arUnitIDs.Add(IDC_CMD_HL22_UNIT); arUnitIDs.Add(IDC_CMD_HL3_UNIT);
	arUnitIDs.Add(IDC_CMD_HL4_UNIT);  arUnitIDs.Add(IDC_CMD_HL41_UNIT);
	arUnitIDs.Add(IDC_CMD_HL42_UNIT); arUnitIDs.Add(IDC_CMD_HL5_UNIT);
	arUnitIDs.Add(IDC_CMD_BL1_UNIT);  arUnitIDs.Add(IDC_CMD_BL2_UNIT);  
	arUnitIDs.Add(IDC_CMD_BL21_UNIT); arUnitIDs.Add(IDC_CMD_BL22_UNIT); 
	arUnitIDs.Add(IDC_CMD_BL41_UNIT); arUnitIDs.Add(IDC_CMD_BL42_UNIT); 
	// Right
	arUnitIDs.Add(IDC_CMD_HR1_UNIT);  arUnitIDs.Add(IDC_CMD_HR2_UNIT);
	arUnitIDs.Add(IDC_CMD_HR21_UNIT); arUnitIDs.Add(IDC_CMD_HR22_UNIT);
	arUnitIDs.Add(IDC_CMD_HR3_UNIT);  arUnitIDs.Add(IDC_CMD_HR4_UNIT);
	arUnitIDs.Add(IDC_CMD_HR41_UNIT); arUnitIDs.Add(IDC_CMD_HR42_UNIT);
	arUnitIDs.Add(IDC_CMD_HR5_UNIT);  arUnitIDs.Add(IDC_CMD_BR1_UNIT);
	arUnitIDs.Add(IDC_CMD_BR2_UNIT);  arUnitIDs.Add(IDC_CMD_BR21_UNIT);
	arUnitIDs.Add(IDC_CMD_BR22_UNIT); arUnitIDs.Add(IDC_CMD_BR41_UNIT);
	arUnitIDs.Add(IDC_CMD_BR42_UNIT); 

	for(int i=0; i<arUnitIDs.GetSize(); i++)
	{
		DDX_Control(pDX,arUnitIDs[i],m_Units[i]);
	}

	CArray<UINT, UINT> arEditIDs;
	// Left
	arEditIDs.Add(IDC_CMD_H1_EDIT);   arEditIDs.Add(IDC_CMD_HL1_EDIT);
	arEditIDs.Add(IDC_CMD_HL2_EDIT);  arEditIDs.Add(IDC_CMD_HL21_EDIT);
	arEditIDs.Add(IDC_CMD_HL22_EDIT); arEditIDs.Add(IDC_CMD_HL3_EDIT);
	arEditIDs.Add(IDC_CMD_HL4_EDIT);  arEditIDs.Add(IDC_CMD_HL41_EDIT);
	arEditIDs.Add(IDC_CMD_HL42_EDIT); arEditIDs.Add(IDC_CMD_HL5_EDIT);
	arEditIDs.Add(IDC_CMD_BL1_EDIT);  arEditIDs.Add(IDC_CMD_BL2_EDIT);
	arEditIDs.Add(IDC_CMD_BL21_EDIT); arEditIDs.Add(IDC_CMD_BL22_EDIT);   
	arEditIDs.Add(IDC_CMD_BL41_EDIT); arEditIDs.Add(IDC_CMD_BL42_EDIT); 
	// Right
	arEditIDs.Add(IDC_CMD_HR1_EDIT);  arEditIDs.Add(IDC_CMD_HR2_EDIT);
	arEditIDs.Add(IDC_CMD_HR21_EDIT); arEditIDs.Add(IDC_CMD_HR22_EDIT);
	arEditIDs.Add(IDC_CMD_HR3_EDIT);  arEditIDs.Add(IDC_CMD_HR4_EDIT);
	arEditIDs.Add(IDC_CMD_HR41_EDIT); arEditIDs.Add(IDC_CMD_HR42_EDIT);
	arEditIDs.Add(IDC_CMD_HR5_EDIT);  arEditIDs.Add(IDC_CMD_BR1_EDIT);
	arEditIDs.Add(IDC_CMD_BR2_EDIT);  arEditIDs.Add(IDC_CMD_BR21_EDIT);
	arEditIDs.Add(IDC_CMD_BR22_EDIT); arEditIDs.Add(IDC_CMD_BR41_EDIT);
	arEditIDs.Add(IDC_CMD_BR42_EDIT); 

	for(int i=0; i<arEditIDs.GetSize(); i++)
	{
		DDX_Control(pDX,arEditIDs[i],m_Edits[i]);
	}

	UINT aCtrlUnit[] = { IDC_CMD_WEB_I_UNIT2, IDC_CMD_WEB_I_UNIT3, IDC_CMD_WEB_I_UNIT4};
	UINT aCtrlEdit[] = { IDC_CMD_WEB_I_EDIT2, IDC_CMD_WEB_I_EDIT3, IDC_CMD_WEB_I_EDIT4};
	UINT aCtrlChek[] = { IDC_CMD_WEB_I_CHECK2, IDC_CMD_WEB_I_CHECK3, IDC_CMD_WEB_I_CHECK4};
	for (int i = 0; i < 3; i++)
	{
	  DDX_Control(pDX, aCtrlUnit[i], m_unitWebShear[i]);
	  DDX_Control(pDX, aCtrlEdit[i], m_editWebShear[i]);
	  DDX_Control(pDX, aCtrlChek[i], m_chkWebShear[i]);
	}

	UINT aCtrlJoinChek[] = { IDC_CMD_JL1_CHECK, IDC_CMD_JL2_CHECK,
													 IDC_CMD_JL3_CHECK,IDC_CMD_JL4_CHECK,
													 IDC_CMD_JR1_CHECK, IDC_CMD_JR2_CHECK,
													 IDC_CMD_JR3_CHECK,IDC_CMD_JR4_CHECK};
	for (int i = 0; i < 8; i++)
	{
	  DDX_Control(pDX, aCtrlJoinChek[i], m_jChk[i]);
	}
}

BEGIN_MESSAGE_MAP(CCMSectItemPSCMid, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSCMid)
	ON_BN_CLICKED(IDC_CMD_JL1_CHECK, OnCmdCheckLeftCtrls)
	ON_BN_CLICKED(IDC_CMD_J1_CHECK, OnCmdCheckCtrls)
	ON_CBN_SELCHANGE(IDC_CMD_TYPE_LEFT_CB, OnSelchangeCmdTypeCombo)
	ON_EN_CHANGE(IDC_CMD_BL1_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_BR1_EDIT, OnChangeCmdSize)
	ON_BN_CLICKED(IDC_SYMMETRY_CHECK, OnSymmetryCheck)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK, OnCmdShearCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_CHECK, OnCmdShear1Check)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_CHECK, OnCmdShear3Check)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK, OnCmdWebICheck)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK2, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_JL2_CHECK, OnCmdCheckLeftCtrls)
	ON_BN_CLICKED(IDC_CMD_JL3_CHECK, OnCmdCheckLeftCtrls)
	ON_BN_CLICKED(IDC_CMD_JL4_CHECK, OnCmdCheckLeftCtrls)
	ON_BN_CLICKED(IDC_CMD_JR1_CHECK, OnCmdCheckCtrls)
	ON_BN_CLICKED(IDC_CMD_JR2_CHECK, OnCmdCheckCtrls)
	ON_BN_CLICKED(IDC_CMD_JR3_CHECK, OnCmdCheckCtrls)
	ON_BN_CLICKED(IDC_CMD_JR4_CHECK, OnCmdCheckCtrls)
	ON_CBN_SELCHANGE(IDC_CMD_TYPE_RGHT_CB, OnSelchangeCmdTypeCombo)
	ON_EN_CHANGE(IDC_CMD_BL2_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_BL21_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_BL22_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_BL41_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_BL42_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_BR2_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BR21_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BR22_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BR41_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BR42_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_H1_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HL1_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL2_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL21_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL22_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL3_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL4_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL41_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL42_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HL5_EDIT, OnChangeCmdLeftSize)
	ON_EN_CHANGE(IDC_CMD_HR1_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR2_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR21_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR22_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR3_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR4_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR41_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR42_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HR5_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SHEAR2_EDIT, OnChangeCmdShearEdit)
	ON_EN_CHANGE(IDC_CMD_SHEAR3_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK3, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK4, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_SECT_TABLEINPUT_BTN, OnCmdSectTableinputBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_BTN,       OnCmdSectWapingCheckBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_AUTO_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_USER_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_CHK,       OnCmdSectWapingCheck)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMid message handlers

/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//
BOOL CCMSectItemPSCMid::GetDlgData(void* pData)
{
	UpdateData(TRUE);

	T_SECT_D* pSect = (T_SECT_D*)pData;
	pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_MID;

	// symmetric
	pSect->SectBefore.bSymmetric = m_bSymmetry;

	// shear check
	pSect->SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	if (pSect->SectBefore.bShearCheck)
	{
		pSect->SectBefore.bAutoShearCheckPos[0][0] = (m_chkShear1.GetCheck()==1) ? TRUE : FALSE;
		pSect->SectBefore.bAutoShearCheckPos[0][1] = TRUE;
		pSect->SectBefore.bAutoShearCheckPos[0][2] = (m_chkShear3.GetCheck()==1) ? TRUE : FALSE;
		pSect->SectBefore.dShearCheckPos[0][0] = m_editShear1.GetEditValue();
		pSect->SectBefore.dShearCheckPos[0][2] = m_editShear3.GetEditValue();
	}
	// shear deform
	pSect->SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	pSect->SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	// warping stress point
	pSect->SectBefore.nWarpingCheck[0] = pSect->SectBefore.nWarpingCheck[1] =m_nWarpingCheck;
	double dWarpingCheckPos[2][6]={0.0};
	memcpy(pSect->SectBefore.dWarpingCheckPosI, m_SectData.SectBefore.dWarpingCheckPosI, sizeof(dWarpingCheckPos)); // I/J같음.
	memcpy(pSect->SectBefore.dWarpingCheckPosJ, m_SectData.SectBefore.dWarpingCheckPosI, sizeof(dWarpingCheckPos));

	// web thickness
	pSect->SectBefore.bAutoWebThk[0] = (m_chkWebI.GetCheck()==1) ? TRUE : FALSE;
	pSect->SectBefore.dWebThk[0] = m_editWebI.GetEditValue();
	for (int i = 0; i < 3; i++)
	{
		pSect->SectBefore.bAutoWebThkShear[0][i] = (m_chkWebShear[i].GetCheck()==1);
		pSect->SectBefore.dWebThkShear[0][i] = m_editWebShear[i].GetEditValue();
	}

	// Offset
	pSect->SectBefore.nOffsetPoint = m_SectData.SectBefore.nOffsetPoint;
	pSect->SectBefore.nOffsetCenter = m_SectData.SectBefore.nOffsetCenter;
	pSect->SectBefore.nUserOffsetRef = m_SectData.SectBefore.nUserOffsetRef;
	pSect->SectBefore.nHorzOffsetOpt = m_SectData.SectBefore.nHorzOffsetOpt;
	if (pSect->SectBefore.nHorzOffsetOpt == 1)
	{
		pSect->SectBefore.dUserDefOffsetY[0] = m_SectData.SectBefore.dUserDefOffsetY[0];
		//pSect->SectBefore.dUserDefOffsetY[1] = m_SectData.SectBefore.dUserDefOffsetY[1];
	}
	pSect->SectBefore.nVertOffsetOpt = m_SectData.SectBefore.nVertOffsetOpt;
	if (pSect->SectBefore.nVertOffsetOpt == 1)
	{
		pSect->SectBefore.dUserDefOffsetZ[0] = m_SectData.SectBefore.dUserDefOffsetZ[0];
		//pSect->SectBefore.dUserDefOffsetZ[1] = m_SectData.SectBefore.dUserDefOffsetZ[1];
	}

	pSect->SectBefore.nJoint = 0; // 초기화 
	if (pSect->SectBefore.bSymmetric)
	{
		// Cell Type 
		pSect->SectBefore.nCellType = m_cLeftCombo.GetCurSel();
		pSect->SectBefore.nCellShape = m_cLeftCombo.GetCurSel();

		// Joint
		if(m_bJ1 ) pSect->SectBefore.nJoint |= 0x00000001 ;
		if(m_bJL1) pSect->SectBefore.nJoint |= 0x00000002 ;
		if(m_bJL2) pSect->SectBefore.nJoint |= 0x00000004 ;
		if(m_bJL3) pSect->SectBefore.nJoint |= 0x00000008 ;
		if(m_bJL4) pSect->SectBefore.nJoint |= 0x00000010 ;
		if(m_bJL1) pSect->SectBefore.nJoint |= 0x00000020 ;
		if(m_bJL2) pSect->SectBefore.nJoint |= 0x00000040 ;
		if(m_bJL3) pSect->SectBefore.nJoint |= 0x00000080 ;
		if(m_bJL4) pSect->SectBefore.nJoint |= 0x00000100 ;

		// Size
		for(int i=0; i<16; i++)  
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();

		for(int i=1; i<16;i++)
			pSect->SectBefore.SectI.Size[i+15] =  m_Edits[i].GetEditValue();
	}
	else
	{
		// Cell Type 
		pSect->SectBefore.nCellType = m_cLeftCombo.GetCurSel();
		pSect->SectBefore.nCellShape = m_cRghtCombo.GetCurSel();

		// Joint
		if(m_bJ1 ) pSect->SectBefore.nJoint |= 0x00000001 ;
		if(m_bJL1) pSect->SectBefore.nJoint |= 0x00000002 ;
		if(m_bJL2) pSect->SectBefore.nJoint |= 0x00000004 ;
		if(m_bJL3) pSect->SectBefore.nJoint |= 0x00000008 ;
		if(m_bJL4) pSect->SectBefore.nJoint |= 0x00000010 ;
		if(m_bJR1) pSect->SectBefore.nJoint |= 0x00000020 ;
		if(m_bJR2) pSect->SectBefore.nJoint |= 0x00000040 ;
		if(m_bJR3) pSect->SectBefore.nJoint |= 0x00000080 ;
		if(m_bJR4) pSect->SectBefore.nJoint |= 0x00000100 ;

		// Size
		for(int i = 0; i < 31 ; i++)  
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();
	}
	return TRUE;
}


void CCMSectItemPSCMid::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D*)pData);
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

void CCMSectItemPSCMid::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
}

void CCMSectItemPSCMid::SetAutotShearCheckPos(double dShearPos[3])
{
	if (m_chkShearCheck.GetCheck() != 1) return;

	if (m_chkShear1.GetCheck() == 1)
	{
		m_SectData.SectBefore.dShearCheckPos[0][0] = dShearPos[0];
		m_editShear1.SetEditUnit(dShearPos[0]);
	}
	m_SectData.SectBefore.dShearCheckPos[0][1] = dShearPos[1];
	if (m_chkShear3.GetCheck() == 1)
	{
		m_SectData.SectBefore.dShearCheckPos[0][2] = dShearPos[2];
		m_editShear3.SetEditUnit(dShearPos[2]);
	}

	RedrawSection();
}

void CCMSectItemPSCMid::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_chkWebShear[i].GetCheck() == 1) 
			m_editWebShear[i].SetEditUnit(dShear[i]);
	}
	if (m_chkWebI.GetCheck() == 1) m_editWebI.SetEditUnit(dTorsion);
}

void CCMSectItemPSCMid::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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

/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMSectItemPSCMid::InitCtrlsData()
{
	m_bJ1 = FALSE;
	m_bJL1 = FALSE;
	m_bJL2 = FALSE;
	m_bJL3 = FALSE;
	m_bJL4 = FALSE;
	m_bJR2 = FALSE;
	m_bJR1 = FALSE;
	m_bJR3 = FALSE;
	m_bJR4 = FALSE;
	m_nWarpingCheck= 0;
}

void CCMSectItemPSCMid::ChangeMidBitmap()
{
	UpdateData(TRUE);

	CString strLeft, strRight;
	int nIndex = m_cLeftCombo.GetCurSel();
	if(nIndex >= 0) m_cLeftCombo.GetLBText(nIndex, strLeft);

	if (m_bSymmetry)
	{
		strRight = strLeft;
	}
	else
	{
		nIndex = m_cRghtCombo.GetCurSel();
		if(nIndex >= 0) m_cRghtCombo.GetLBText(nIndex, strRight);
	}

	// Change Bitmap
	if(strLeft == _LS(IDS_CMD_SECT_PSC_Cell_Type_None))
	{
		if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_None)) ChangeBitmap(__SECT_PSC_MID_NONE_NONE__);
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ)) ChangeBitmap(__SECT_PSC_MID_NONE_CIRC__);
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly)) ChangeBitmap(__SECT_PSC_MID_NONE_POLY__);
	}
	else if(strLeft == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ))
	{
		if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_None)) ChangeBitmap(__SECT_PSC_MID_CIRC_NONE__);
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ)) ChangeBitmap(__SECT_PSC_MID_CIRC_CIRC__);
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly)) ChangeBitmap(__SECT_PSC_MID_CIRC_POLY__);
	}
	else if(strLeft == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly))
	{
		if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_None)) ChangeBitmap(__SECT_PSC_MID_POLY_NONE__);
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ)) ChangeBitmap(__SECT_PSC_MID_POLY_CIRC__);
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly)) ChangeBitmap(__SECT_PSC_MID_POLY_POLY__);
	}
}

void CCMSectItemPSCMid::UpdateCtrlsByType()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_TYPE_RGHT_CB)->EnableWindow(!m_bSymmetry);

	CString strLeft, strRight;
	int nIndex = m_cLeftCombo.GetCurSel();
	if(nIndex >= 0) m_cLeftCombo.GetLBText(nIndex, strLeft);

	CDlgUtil::CtrlEnableDisable(this, arRhtComnGroup, TRUE);
	CDlgUtil::CtrlEnableDisable(this, arRhtCircGroup, TRUE);
	CDlgUtil::CtrlEnableDisable(this, arRhtNoneGroup, TRUE);
	CDlgUtil::CtrlEnableDisable(this, arLftComnGroup, TRUE);
	CDlgUtil::CtrlEnableDisable(this, arLftCircGroup, TRUE);
	CDlgUtil::CtrlEnableDisable(this, arLftNoneGroup, TRUE);

	if(strLeft == _LS(IDS_CMD_SECT_PSC_Cell_Type_None))
	{
		m_bJL1 = m_bJL2 = m_bJL3 = m_bJL4 = FALSE;
		UpdateData(FALSE);
		CDlgUtil::CtrlEnableDisable(this, arLftComnGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, arLftNoneGroup, FALSE);
	}
	else if(strLeft == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ))
	{
		m_bJL1 = m_bJL2 = m_bJL3 = m_bJL4 = FALSE;
		UpdateData(FALSE);
		CDlgUtil::CtrlEnableDisable(this, arLftComnGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, arLftCircGroup, FALSE);
	}
	else if(strLeft == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly))
	{
		// Nothing
	}
	else ASSERT(0);

	if (m_bSymmetry)
	{
		strRight = strLeft;
		CDlgUtil::CtrlEnableDisable(this, arSymmetryGroup1, (!m_bSymmetry));
		CDlgUtil::CtrlEnableDisable(this, arSymmetryGroup2, (!m_bSymmetry));
		
		m_bJR1 = m_bJR2 = m_bJR3 = m_bJR4 = FALSE;
		UpdateData(FALSE);
	}
	else
	{
		nIndex = m_cRghtCombo.GetCurSel();
		if(nIndex >= 0) m_cRghtCombo.GetLBText(nIndex, strRight);
	
		if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_None))
		{
			m_bJR1 = m_bJR2 = m_bJR3 = m_bJR4 = FALSE;
			UpdateData(FALSE);
			CDlgUtil::CtrlEnableDisable(this, arRhtComnGroup, FALSE);
			CDlgUtil::CtrlEnableDisable(this, arRhtNoneGroup, FALSE);
		}
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ))
		{
			m_bJR1 = m_bJR2 = m_bJR3 = m_bJR4 = FALSE;
			UpdateData(FALSE);
			CDlgUtil::CtrlEnableDisable(this, arRhtComnGroup, FALSE);
			CDlgUtil::CtrlEnableDisable(this, arRhtCircGroup, FALSE);
		}
		else if(strRight == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly))
		{
			// Nothing
		}
		else ASSERT(0);
	}
		
	GetDlgItem(IDC_CMD_HR1_EDIT)->EnableWindow(!m_bSymmetry);
	GetDlgItem(IDC_CMD_BR2_EDIT)->EnableWindow(!m_bSymmetry);

	UpdateCtrlsByJoint();

}

void CCMSectItemPSCMid::UpdateCtrlsByJoint()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, arJ1Group , m_bJ1);
	CDlgUtil::CtrlEnableDisable(this, arJL1Group, m_bJL1);
	CDlgUtil::CtrlEnableDisable(this, arJL2Group, m_bJL2);
	CDlgUtil::CtrlEnableDisable(this, arJL3Group, m_bJL3);
	CDlgUtil::CtrlEnableDisable(this, arJL4Group, m_bJL4);

	CDlgUtil::CtrlEnableDisable(this, arJR1Group, m_bJR1&&!m_bSymmetry);
	CDlgUtil::CtrlEnableDisable(this, arJR2Group, m_bJR2&&!m_bSymmetry);
	CDlgUtil::CtrlEnableDisable(this, arJR3Group, m_bJR3&&!m_bSymmetry);
	CDlgUtil::CtrlEnableDisable(this, arJR4Group, m_bJR4&&!m_bSymmetry);
}

void CCMSectItemPSCMid::UpdateCurDatas()
{
	InitCtrlsData();

	// symmetic
	m_bSymmetry = m_SectData.SectBefore.bSymmetric;

	// shear check
	int nCheck = (int)m_SectData.SectBefore.bShearCheck;
	m_chkShearCheck.SetCheck(nCheck);
	m_chkShear1.SetCheck((int)m_SectData.SectBefore.bAutoShearCheckPos[0][0]);
	m_chkShear3.SetCheck((int)m_SectData.SectBefore.bAutoShearCheckPos[0][2]);
	m_editShear1.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][0]);
	m_editShear2.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][1]);
	m_editShear3.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][2]);
	ChangeShearCheck(nCheck);

	// shear deform
	m_chkShearDeform.SetCheck((int)m_SectData.SectBefore.bConsiderShearDeform);
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
	// web thick
	m_chkWebI.SetCheck((int)m_SectData.SectBefore.bAutoWebThk[0]);
	m_editWebI.SetEditUnit(m_SectData.SectBefore.dWebThk[0]);
	for (int i = 0; i < 3; i++)
	{
		m_chkWebShear[i].SetCheck((int)m_SectData.SectBefore.bAutoWebThkShear[0][i]);
		m_editWebShear[i].SetEditUnit(m_SectData.SectBefore.dWebThkShear[0][i]);
	}
	OnCmdWebICheck();
	OnCmdWebICheck2();

	// Offset
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_SectData.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_SectData.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// Cell Type
	if(m_SectData.SectBefore.nCellType >= 0) m_cLeftCombo.SetCurSel(m_SectData.SectBefore.nCellType);
	if(m_SectData.SectBefore.nCellShape >= 0) m_cRghtCombo.SetCurSel(m_SectData.SectBefore.nCellShape);

	// Joint
	if(m_SectData.SectBefore.nJoint & 0x00000001) m_bJ1  = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000002) m_bJL1 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000004) m_bJL2 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000008) m_bJL3 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000010) m_bJL4 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000020) m_bJR1 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000040) m_bJR2 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000080) m_bJR3 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000100) m_bJR4 = TRUE ;

	// size
	for(int i = 0; i < 31 ; i++)
		m_Edits[i].SetEditUnit(m_SectData.SectBefore.SectI.Size[i]);
 
	UpdateData(FALSE);
}

void CCMSectItemPSCMid::InitCellTypeCombo()
{
	CStringArray strCellType; 
	strCellType.Add(_LS(IDS_CMD_SECT_PSC_Cell_Type_None));
	strCellType.Add(_LS(IDS_CMD_SECT_PSC_Cell_Type_Circ));
	strCellType.Add(_LS(IDS_CMD_SECT_PSC_Cell_Type_Poly));

	m_cLeftCombo.ResetContent();
	m_cRghtCombo.ResetContent();

	for(int i=0; i<strCellType.GetSize(); i++)
	{
		m_cLeftCombo.AddString(strCellType[i]);
		m_cRghtCombo.AddString(strCellType[i]);
	}
	m_cLeftCombo.SetCurSel(2);
	m_cRghtCombo.SetCurSel(2);
}


BOOL CCMSectItemPSCMid::OnInitDialog() 
{
	// 일부 데이터 초기화 
	if(!m_bModify)
	{
		m_SectData.SectBefore.nCellType  = 2;
		m_SectData.SectBefore.nCellShape = 2;
	}
//  else m_bModify = FALSE;

	CCMSectItemPSCBase::OnInitDialog();

	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();
 
	for( int i = 0 ;  i < 31 ; i++)
	{
		m_Units[i].SetUnitType(D_UNITSYS_BASE_LENGTH);    
		m_Edits[i].SetUnitType(D_UNITSYS_BASE_LENGTH);    
	}
	m_unitShear3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editShear3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitShear2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editShear2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitShear1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editShear1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitWebI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editWebI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	for (int i = 0; i < 3; i++)
	{
		m_unitWebShear[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_editWebShear[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}

	InitCellTypeCombo();

	UpdateCurDatas();
	UpdateCtrlsByType();

	// Update Viewer
	RedrawSection();
	ChangeMidBitmap();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
BOOL CCMSectItemPSCMid::DestroyWindow() 
{
	m_SectData.Initialize();
	if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
	return CCMSectItemPSCBase::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
//
// Elable/ Disable 
//

void CCMSectItemPSCMid::SyncCheckData(BOOL bSync)
{
	if(bSync)
	{
		for(int i = 0; i < 4 ; i++)  
		{
			if(m_jChk[i].GetCheck()!=m_jChk[i+4].GetCheck())
			{
				m_jChk[i+4].SetCheck(m_jChk[i].GetCheck());
			}
		}
	}
}

void CCMSectItemPSCMid::OnCmdCheckLeftCtrls() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByJoint();
	SyncCheckData(m_bSymmetry);
	RedrawSection();
}

void CCMSectItemPSCMid::OnCmdCheckCtrls() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByJoint();
	RedrawSection();
}

void CCMSectItemPSCMid::SyncCmbkData(BOOL bSync)
{
	if(bSync)
	{
		m_cRghtCombo.SetCurSel(m_cLeftCombo.GetCurSel());
	}
}

void CCMSectItemPSCMid::OnSelchangeCmdTypeCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByType();
	ChangeMidBitmap();
	RedrawSection();
	SyncCmbkData(m_bSymmetry);
}


void CCMSectItemPSCMid::SyncEditData(BOOL bSync)
{
	if(bSync)
	{
		for(int i = 1; i < 16 ; i++)  
		{
			if(m_Edits[i].GetEditValue()!=m_Edits[i+15].GetEditValue())
			{
				m_Edits[i+15].SetEditUnit(m_Edits[i].GetEditValue());
			}
		}
	}
}

void CCMSectItemPSCMid::OnChangeCmdLeftSize()
{
	SyncEditData(m_bSymmetry);
	RedrawSection();
}

void CCMSectItemPSCMid::OnChangeCmdSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	RedrawSection();
}

void CCMSectItemPSCMid::OnSymmetryCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByType();
	ChangeMidBitmap();
	RedrawSection();
	SyncEditData(m_bSymmetry);
	SyncCheckData(m_bSymmetry);
	SyncCmbkData(m_bSymmetry);
}

void CCMSectItemPSCMid::OnCmdChangeOffsetBtn() 
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

void CCMSectItemPSCMid::OnChangeCmdShearEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.dShearCheckPos[0][0] = m_editShear1.GetEditValue();
	//m_SectData.SectBefore.dShearCheckPos[0][1] = m_editShear2.GetEditValue();
	m_SectData.SectBefore.dShearCheckPos[0][2] = m_editShear3.GetEditValue();
	RedrawSection();
}

void CCMSectItemPSCMid::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	ChangeShearCheck(m_SectData.SectBefore.bShearCheck);
	RedrawSection();
}

void CCMSectItemPSCMid::ChangeShearCheck(int nCheck)
{
	m_chkShear1.EnableWindow(nCheck == 1);
	m_chkShear3.EnableWindow(nCheck == 1);
	//m_editShear1.EnableWindow(nCheck == 1);
	//m_editShear2.EnableWindow(nCheck == 1);
	//m_editShear3.EnableWindow(nCheck == 1);
	ChageAutoShearCheck1();
	ChageAutoShearCheck3();
}

void CCMSectItemPSCMid::OnCmdShear1Check() 
{
	// TODO: Add your control notification handler code here
	ChageAutoShearCheck1();
	RedrawSection();
}

void CCMSectItemPSCMid::ChageAutoShearCheck1()
{
	BOOL bShearCheck = m_chkShearCheck.GetCheck();
	BOOL bAutoShearCheck = m_chkShear1.GetCheck();
	BOOL bEnable = (bShearCheck && !bAutoShearCheck);
	m_SectData.SectBefore.bAutoShearCheckPos[0][0] = bAutoShearCheck;
	m_editShear1.EnableWindow(bEnable);
	/*
	if (!bEnable)
	{
		m_SectData.SectBefore.dShearCheckPos[0][0] = 0.0;
		m_editShear1.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][0]);
	}
	*/
}

void CCMSectItemPSCMid::OnCmdShear3Check() 
{
	// TODO: Add your control notification handler code here
	ChageAutoShearCheck3();
	RedrawSection();
}

void CCMSectItemPSCMid::ChageAutoShearCheck3()
{
	BOOL bShearCheck = m_chkShearCheck.GetCheck();
	BOOL bAutoShearCheck = m_chkShear3.GetCheck();
	BOOL bEnable = (bShearCheck && !bAutoShearCheck);
	m_SectData.SectBefore.bAutoShearCheckPos[0][2] = bAutoShearCheck;
	m_editShear3.EnableWindow(bEnable);	
	/*
	if (!bEnable)
	{
		m_SectData.SectBefore.dShearCheckPos[0][2] = 0.0;
		m_editShear3.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][2]);
	}
	*/
}

void CCMSectItemPSCMid::OnCmdWebICheck() 
{
	// TODO: Add your control notification handler code here]
	m_SectData.SectBefore.bAutoWebThk[0] = m_chkWebI.GetCheck();
	m_editWebI.EnableWindow(!m_SectData.SectBefore.bAutoWebThk[0]);
}

void CCMSectItemPSCMid::OnCmdWebICheck2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = m_chkWebShear[i].GetCheck();
		m_editWebShear[i].EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][i]);
	}
}

void CCMSectItemPSCMid::SaveDataForDrawSection()
{
	UpdateData(TRUE);

	// symmetry
	m_SectData.SectBefore.bSymmetric = m_bSymmetry;

	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_MID;
		
	// shear check
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	if (m_SectData.SectBefore.bShearCheck)
	{
		m_SectData.SectBefore.bAutoShearCheckPos[0][0] = (m_chkShear1.GetCheck()==1) ? TRUE : FALSE;
		m_SectData.SectBefore.bAutoShearCheckPos[0][1] = TRUE;
		m_SectData.SectBefore.bAutoShearCheckPos[0][2] = (m_chkShear3.GetCheck()==1) ? TRUE : FALSE;
		m_SectData.SectBefore.dShearCheckPos[0][0] = m_editShear1.GetEditValue();
		m_SectData.SectBefore.dShearCheckPos[0][2] = m_editShear3.GetEditValue();
	}
	// shear deform
	m_SectData.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_SectData.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	// warping stress point
	m_SectData.SectBefore.nWarpingCheck[0] =m_nWarpingCheck;
	m_SectData.SectBefore.nWarpingCheck[1] =m_nWarpingCheck;

	// web thickness
	m_SectData.SectBefore.bAutoWebThk[0] = (m_chkWebI.GetCheck()==1) ? TRUE : FALSE;
	m_SectData.SectBefore.dWebThk[0] = m_editWebI.GetEditValue();
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = (m_chkWebShear[i].GetCheck()==1);
		m_SectData.SectBefore.dWebThkShear[0][i] = m_editWebShear[i].GetEditValue();
	}

	// offset = nothing

	int nIndex;
	m_SectData.SectBefore.nJoint = 0;  // 초기화

	if (m_SectData.SectBefore.bSymmetric)
	{
		// Cell Type 
		nIndex = m_cLeftCombo.GetCurSel();
		if(nIndex >= 0)
		{
			m_SectData.SectBefore.nCellType = nIndex;
			m_SectData.SectBefore.nCellShape = nIndex;
		}
		// Joint
		if(m_bJ1 ) m_SectData.SectBefore.nJoint |= 0x00000001 ;
		if(m_bJL1) m_SectData.SectBefore.nJoint |= 0x00000002 ;
		if(m_bJL2) m_SectData.SectBefore.nJoint |= 0x00000004 ;
		if(m_bJL3) m_SectData.SectBefore.nJoint |= 0x00000008 ;
		if(m_bJL4) m_SectData.SectBefore.nJoint |= 0x00000010 ;
		if(m_bJL1) m_SectData.SectBefore.nJoint |= 0x00000020 ;
		if(m_bJL2) m_SectData.SectBefore.nJoint |= 0x00000040 ;
		if(m_bJL3) m_SectData.SectBefore.nJoint |= 0x00000080 ;
		if(m_bJL4) m_SectData.SectBefore.nJoint |= 0x00000100 ;
		
		// Size
		for(int i = 0; i < 16 ; i++)  
			m_SectData.SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();

		for(int i = 1; i < 16 ; i++)
			m_SectData.SectBefore.SectI.Size[i+15] =  m_Edits[i].GetEditValue();
	}
	else
	{
		// Cell Type 
		nIndex = m_cLeftCombo.GetCurSel();
		if(nIndex >= 0) m_SectData.SectBefore.nCellType = nIndex;
		nIndex = m_cRghtCombo.GetCurSel();
		if(nIndex >= 0) m_SectData.SectBefore.nCellShape = nIndex;

		// Joint
		if(m_bJ1 ) m_SectData.SectBefore.nJoint |= 0x00000001 ;
		if(m_bJL1) m_SectData.SectBefore.nJoint |= 0x00000002 ;
		if(m_bJL2) m_SectData.SectBefore.nJoint |= 0x00000004 ;
		if(m_bJL3) m_SectData.SectBefore.nJoint |= 0x00000008 ;
		if(m_bJL4) m_SectData.SectBefore.nJoint |= 0x00000010 ;
		if(m_bJR1) m_SectData.SectBefore.nJoint |= 0x00000020 ;
		if(m_bJR2) m_SectData.SectBefore.nJoint |= 0x00000040 ;
		if(m_bJR3) m_SectData.SectBefore.nJoint |= 0x00000080 ;
		if(m_bJR4) m_SectData.SectBefore.nJoint |= 0x00000100 ;

		// Size
		for(int i = 0; i < 31 ; i++)  
			m_SectData.SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();
	}
}

///////////////////////////////////////////////////////////////////////////////
// 
// Update Viewer
//
BOOL CCMSectItemPSCMid::ChangeBitmap(int nIndex)
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(nIndex);
}


void CCMSectItemPSCMid::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSCMid::RedrawSection()
{
	ASSERT(m_pParent);

	SaveDataForDrawSection();
	m_pParent->RedrawSection();
}

void CCMSectItemPSCMid::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	SaveDataForDrawSection();
	if (!CSectUtil::CalcSectData(m_SectData))
	{
		AfxMessageBox(_LS(IDS_CMD_SECT_Impossible_Display_Centroid));
		return;
	}
	T_SECT_SECTION_D* pSectBefore = &m_SectData.SectBefore;
	pSectBefore->dShearCheckPos[0][1] = pSectBefore->SectI.Design.ZBar;
	SetAutotShearCheckPos(pSectBefore->dShearCheckPos[0]);
	SetAutoMinWebThick(pSectBefore->dWebThkShear[0], pSectBefore->dWebThk[0]);

	ASSERT(m_pParent);
	m_pParent->DisplayCentoid();
}


void CCMSectItemPSCMid::OnCmdSectTableinputBtn() 
{
	CCMSectItemPSCMidGridDlg m_GridDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_MID Cell;
	Cell.Set(*sect,TRUE,TRUE);
	m_GridDlg.SetData(sect,this);
	if(m_GridDlg.DoModal()!=IDOK)
	{
		Cell.Get(*sect,TRUE,TRUE);
		UpdateCurDatas(); 
		UpdateCtrlsByType();
		RedrawSection();
		
	}	
	
}

void CCMSectItemPSCMid::OnCmdSectWapingCheckBtn() 
{
	CCMSectItemPSC_WarpingCheckDlg WapingChkDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_I Cell;
	Cell.Set(*sect,TRUE,TRUE);
	WapingChkDlg.SetData(sect,this);
	if(WapingChkDlg.DoModal()!=IDOK)
	{
		//    Cell.Get(*sect,TRUE,TRUE);
		UpdateCurDatas(); 
	}	
}

void CCMSectItemPSCMid::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSCMid::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}