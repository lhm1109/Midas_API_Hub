// CMSectItemPSC_I.cpp : implementation file
// 2003. 02. 14   by TAE
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC_I.h"

#include "CMSectItemPSC.h"
#include "CMSectItemPSC_I_DB.h"
#include "CMSectOffsetDlg.h"
#include "CMSectItemPSCIGridDlg.h"
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

//#define CMD_SECT_NONE  _LS(IDS_CMD0417__None)

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_I dialog

CCMSectItemPSC_I::CCMSectItemPSC_I(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSC_I::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC_I)
	m_bJ1 = FALSE;
	m_bJL1 = FALSE;
	m_bJL2 = FALSE;
	m_bJL3 = FALSE;
	m_bJL4 = FALSE;
	m_bJR1 = FALSE;
	m_bJR2 = FALSE;
	m_bJR3 = FALSE;
	m_bJR4 = FALSE;
	m_bSymmetry = TRUE;
	//}}AFX_DATA_INIT
	m_nWarpingCheck= 0;

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	m_pSectDB = NULL;
	m_pSectDB = new CCMSectItemPSCIDB();

	// Joint Group
	m_aJ1Group.Add(IDC_CMD_H1_EDIT);
	m_aJL1Group.Add(IDC_CMD_HL21_EDIT); m_aJL1Group.Add(IDC_CMD_BL21_EDIT);
	m_aJL2Group.Add(IDC_CMD_HL22_EDIT); m_aJL2Group.Add(IDC_CMD_BL22_EDIT); 
	m_aJL3Group.Add(IDC_CMD_HL42_EDIT); m_aJL3Group.Add(IDC_CMD_BL42_EDIT);
	m_aJL4Group.Add(IDC_CMD_HL41_EDIT); m_aJL4Group.Add(IDC_CMD_BL41_EDIT);
	m_aJR1Group.Add(IDC_CMD_HR21_EDIT); m_aJR1Group.Add(IDC_CMD_BR21_EDIT); 
	m_aJR2Group.Add(IDC_CMD_HR22_EDIT); m_aJR2Group.Add(IDC_CMD_BR22_EDIT);
	m_aJR3Group.Add(IDC_CMD_HR42_EDIT); m_aJR3Group.Add(IDC_CMD_BR42_EDIT);
	m_aJR4Group.Add(IDC_CMD_HR41_EDIT); m_aJR4Group.Add(IDC_CMD_BR41_EDIT);

	// Symmetry Group
	m_aSymmetryGroup1.Add(IDC_CMD_JR1_CHECK); m_aSymmetryGroup1.Add(IDC_CMD_JR2_CHECK);
	m_aSymmetryGroup1.Add(IDC_CMD_JR3_CHECK); m_aSymmetryGroup1.Add(IDC_CMD_JR4_CHECK);
	m_aSymmetryGroup2.Add(IDC_CMD_HR1_EDIT);  m_aSymmetryGroup2.Add(IDC_CMD_HR2_EDIT);
	m_aSymmetryGroup2.Add(IDC_CMD_HR21_EDIT); m_aSymmetryGroup2.Add(IDC_CMD_HR22_EDIT);
	m_aSymmetryGroup2.Add(IDC_CMD_HR3_EDIT);  m_aSymmetryGroup2.Add(IDC_CMD_HR4_EDIT);
	m_aSymmetryGroup2.Add(IDC_CMD_HR41_EDIT); m_aSymmetryGroup2.Add(IDC_CMD_HR42_EDIT);
	m_aSymmetryGroup2.Add(IDC_CMD_HR5_EDIT);  m_aSymmetryGroup2.Add(IDC_CMD_BR1_EDIT);
	m_aSymmetryGroup2.Add(IDC_CMD_BR2_EDIT);  m_aSymmetryGroup2.Add(IDC_CMD_BR21_EDIT);
	m_aSymmetryGroup2.Add(IDC_CMD_BR22_EDIT); m_aSymmetryGroup2.Add(IDC_CMD_BR4_EDIT);
	m_aSymmetryGroup2.Add(IDC_CMD_BR41_EDIT); m_aSymmetryGroup2.Add(IDC_CMD_BR42_EDIT); 
}

CCMSectItemPSC_I::~CCMSectItemPSC_I()
{
	if (m_pSectDB != NULL)
	{
		delete m_pSectDB;
		m_pSectDB = NULL;
	}
}

void CCMSectItemPSC_I::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC_I)
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
	DDX_Control(pDX, IDC_CMD_SECT_TYPE_COMBO, m_cobxTypeName);
	DDX_Control(pDX, IDC_CMD_SECT_NAME_COMBO, m_cobxSectName);
	DDX_Check(pDX, IDC_CMD_J1_CHECK, m_bJ1);
	DDX_Check(pDX, IDC_CMD_JL1_CHECK, m_bJL1);
	DDX_Check(pDX, IDC_CMD_JL2_CHECK, m_bJL2);
	DDX_Check(pDX, IDC_CMD_JL3_CHECK, m_bJL3);
	DDX_Check(pDX, IDC_CMD_JL4_CHECK, m_bJL4);
	DDX_Check(pDX, IDC_CMD_JR1_CHECK, m_bJR1);
	DDX_Check(pDX, IDC_CMD_JR2_CHECK, m_bJR2);
	DDX_Check(pDX, IDC_CMD_JR3_CHECK, m_bJR3);
	DDX_Check(pDX, IDC_CMD_JR4_CHECK, m_bJR4);
	DDX_Check(pDX, IDC_SYMMETRY_CHECK, m_bSymmetry);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	//}}AFX_DATA_MAP

	// 주의 : 이 부분은 순서 바뀌면 안됩니다!!

	CArray<UINT, UINT> aUnitIDs;
	aUnitIDs.Add(IDC_CMD_H1_UNIT);   aUnitIDs.Add(IDC_CMD_HL1_UNIT);
	aUnitIDs.Add(IDC_CMD_HL2_UNIT);  aUnitIDs.Add(IDC_CMD_HL21_UNIT);
	aUnitIDs.Add(IDC_CMD_HL22_UNIT); aUnitIDs.Add(IDC_CMD_HL3_UNIT);
	aUnitIDs.Add(IDC_CMD_HL4_UNIT);  aUnitIDs.Add(IDC_CMD_HL41_UNIT);
	aUnitIDs.Add(IDC_CMD_HL42_UNIT); aUnitIDs.Add(IDC_CMD_HL5_UNIT);
	aUnitIDs.Add(IDC_CMD_BL1_UNIT);  aUnitIDs.Add(IDC_CMD_BL2_UNIT);  
	aUnitIDs.Add(IDC_CMD_BL21_UNIT); aUnitIDs.Add(IDC_CMD_BL22_UNIT);
	aUnitIDs.Add(IDC_CMD_BL4_UNIT);  aUnitIDs.Add(IDC_CMD_BL41_UNIT); 
	aUnitIDs.Add(IDC_CMD_BL42_UNIT); 
	aUnitIDs.Add(IDC_CMD_HR1_UNIT);  aUnitIDs.Add(IDC_CMD_HR2_UNIT);
	aUnitIDs.Add(IDC_CMD_HR21_UNIT); aUnitIDs.Add(IDC_CMD_HR22_UNIT);
	aUnitIDs.Add(IDC_CMD_HR3_UNIT);  aUnitIDs.Add(IDC_CMD_HR4_UNIT);
	aUnitIDs.Add(IDC_CMD_HR41_UNIT); aUnitIDs.Add(IDC_CMD_HR42_UNIT);
	aUnitIDs.Add(IDC_CMD_HR5_UNIT);  aUnitIDs.Add(IDC_CMD_BR1_UNIT);
	aUnitIDs.Add(IDC_CMD_BR2_UNIT);  aUnitIDs.Add(IDC_CMD_BR21_UNIT);
	aUnitIDs.Add(IDC_CMD_BR22_UNIT); aUnitIDs.Add(IDC_CMD_BR4_UNIT);
	aUnitIDs.Add(IDC_CMD_BR41_UNIT); aUnitIDs.Add(IDC_CMD_BR42_UNIT); 
	for(int i=0; i<aUnitIDs.GetSize(); i++)
	{
	  DDX_Control(pDX, aUnitIDs[i], m_Units[i]);
	}
	
	CArray<UINT, UINT> aEditIDs;
	aEditIDs.Add(IDC_CMD_H1_EDIT);   aEditIDs.Add(IDC_CMD_HL1_EDIT);
	aEditIDs.Add(IDC_CMD_HL2_EDIT);  aEditIDs.Add(IDC_CMD_HL21_EDIT);
	aEditIDs.Add(IDC_CMD_HL22_EDIT); aEditIDs.Add(IDC_CMD_HL3_EDIT);
	aEditIDs.Add(IDC_CMD_HL4_EDIT);  aEditIDs.Add(IDC_CMD_HL41_EDIT);
	aEditIDs.Add(IDC_CMD_HL42_EDIT); aEditIDs.Add(IDC_CMD_HL5_EDIT);
	aEditIDs.Add(IDC_CMD_BL1_EDIT);  aEditIDs.Add(IDC_CMD_BL2_EDIT);
	aEditIDs.Add(IDC_CMD_BL21_EDIT); aEditIDs.Add(IDC_CMD_BL22_EDIT);  
	aEditIDs.Add(IDC_CMD_BL4_EDIT);  aEditIDs.Add(IDC_CMD_BL41_EDIT); 
	aEditIDs.Add(IDC_CMD_BL42_EDIT); 
	aEditIDs.Add(IDC_CMD_HR1_EDIT);  aEditIDs.Add(IDC_CMD_HR2_EDIT);
	aEditIDs.Add(IDC_CMD_HR21_EDIT); aEditIDs.Add(IDC_CMD_HR22_EDIT);
	aEditIDs.Add(IDC_CMD_HR3_EDIT);  aEditIDs.Add(IDC_CMD_HR4_EDIT);
	aEditIDs.Add(IDC_CMD_HR41_EDIT); aEditIDs.Add(IDC_CMD_HR42_EDIT);
	aEditIDs.Add(IDC_CMD_HR5_EDIT);  aEditIDs.Add(IDC_CMD_BR1_EDIT);
	aEditIDs.Add(IDC_CMD_BR2_EDIT);  aEditIDs.Add(IDC_CMD_BR21_EDIT);
	aEditIDs.Add(IDC_CMD_BR22_EDIT); aEditIDs.Add(IDC_CMD_BR4_EDIT);
	aEditIDs.Add(IDC_CMD_BR41_EDIT); aEditIDs.Add(IDC_CMD_BR42_EDIT); 
	for(int i=0; i<aEditIDs.GetSize(); i++)
	{
	  DDX_Control(pDX, aEditIDs[i], m_Edits[i]);
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


BEGIN_MESSAGE_MAP(CCMSectItemPSC_I, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSC_I)
	ON_BN_CLICKED(IDC_CMD_JL1_CHECK, OnCmdCheckLeftCtrls)
	ON_EN_CHANGE(IDC_CMD_BL2_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BL1_EDIT, OnChangeCmdEdit)
	ON_BN_CLICKED(IDC_CMD_J1_CHECK, OnCmdCheckCtrls)
	ON_BN_CLICKED(IDC_SYMMETRY_CHECK, OnSymmetryCheck)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_TYPE_COMBO, OnSelchangeCmdSectTypeCombo)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_NAME_COMBO, OnSelchangeCmdSectNameCombo)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK, OnCmdShearCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_CHECK, OnCmdShear1Check)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_CHECK, OnCmdShear3Check)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK, OnCmdWebICheck)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK2, OnCmdWebICheck2)
	ON_EN_CHANGE(IDC_CMD_BL21_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BL22_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BL4_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BL41_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BL42_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BR1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BR2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BR21_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BR22_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BR4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BR41_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BR42_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_H1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HL1_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL2_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL21_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL22_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL3_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL4_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL41_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL42_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HL5_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HR1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR21_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR22_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR4_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR41_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR42_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HR5_EDIT, OnChangeCmdEdit)
	ON_BN_CLICKED(IDC_CMD_JL2_CHECK, OnCmdCheckLeftCtrls)
	ON_BN_CLICKED(IDC_CMD_JL3_CHECK, OnCmdCheckLeftCtrls)
	ON_BN_CLICKED(IDC_CMD_JL4_CHECK, OnCmdCheckLeftCtrls)
	ON_BN_CLICKED(IDC_CMD_JR1_CHECK, OnCmdCheckCtrls)
	ON_BN_CLICKED(IDC_CMD_JR2_CHECK, OnCmdCheckCtrls)
	ON_BN_CLICKED(IDC_CMD_JR3_CHECK, OnCmdCheckCtrls)
	ON_BN_CLICKED(IDC_CMD_JR4_CHECK, OnCmdCheckCtrls)
	ON_EN_CHANGE(IDC_CMD_SHEAR2_EDIT, OnChangeCmdShearEdit)
	ON_EN_CHANGE(IDC_CMD_SHEAR3_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK3, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK4, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_SECT_TABLEINPUT_BTN, OnCmdSectTableinputBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_BTN,  OnCmdSectWapingCheckBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_BTN,       OnCmdSectWapingCheckBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_AUTO_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_USER_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_CHK,       OnCmdSectWapingCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_I message handlers

/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//
void CCMSectItemPSC_I::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D*)pData);
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

BOOL CCMSectItemPSC_I::GetDlgData(void* pData)
{
	UpdateData(TRUE);

	T_SECT_D* pSect = (T_SECT_D*)pData;
	pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_I;

	// symmetry
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
	if(pSect->SectBefore.bSymmetric)
	{
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
		for(int i=0; i<17; i++)  
			pSect->SectBefore.SectI.Size[i] = m_Edits[i].GetEditValue();
		for(int i=1; i<17;i++)
			pSect->SectBefore.SectI.Size[i+16] = m_Edits[i].GetEditValue();
	}
	else
	{
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
		for(int i = 0; i < 33 ; i++)  
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();
	}
	return TRUE;
}

void CCMSectItemPSC_I::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
}

void CCMSectItemPSC_I::SetAutotShearCheckPos(double dShearPos[3])
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

void CCMSectItemPSC_I::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_chkWebShear[i].GetCheck() == 1) 
			m_editWebShear[i].SetEditUnit(dShear[i]);
	}
	if (m_chkWebI.GetCheck() == 1) m_editWebI.SetEditUnit(dTorsion);
}

void CCMSectItemPSC_I::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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

void CCMSectItemPSC_I::InitCtrlsData()
{
	m_bJ1 = FALSE;
	m_bJL1 = FALSE;
	m_bJL2 = FALSE;
	m_bJL3 = FALSE;
	m_bJL4 = FALSE;
	m_bJR1 = FALSE;
	m_bJR2 = FALSE;
	m_bJR3 = FALSE;
	m_bJR4 = FALSE;
	m_nWarpingCheck= 0;
}

void CCMSectItemPSC_I::UpdateCtrlsByJoint()
{ 
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aJ1Group , m_bJ1);
	CDlgUtil::CtrlEnableDisable(this, m_aJL1Group, m_bJL1);
	CDlgUtil::CtrlEnableDisable(this, m_aJL2Group, m_bJL2);
	CDlgUtil::CtrlEnableDisable(this, m_aJL3Group, m_bJL3);
	CDlgUtil::CtrlEnableDisable(this, m_aJL4Group, m_bJL4);

	CDlgUtil::CtrlEnableDisable(this, m_aJR1Group, m_bJR1&&!m_bSymmetry);
	CDlgUtil::CtrlEnableDisable(this, m_aJR2Group, m_bJR2&&!m_bSymmetry);
	CDlgUtil::CtrlEnableDisable(this, m_aJR3Group, m_bJR3&&!m_bSymmetry);
	CDlgUtil::CtrlEnableDisable(this, m_aJR4Group, m_bJR4&&!m_bSymmetry);

}
void CCMSectItemPSC_I::UpdateCtrls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aSymmetryGroup1, !m_bSymmetry);
	CDlgUtil::CtrlEnableDisable(this, m_aSymmetryGroup2, !m_bSymmetry);
	if(m_bSymmetry) 
	{
		m_bJR1 = m_bJR2 = m_bJR3 = m_bJR4 = FALSE;
		UpdateData(FALSE);
	}
	UpdateCtrlsByJoint();
}

void CCMSectItemPSC_I::UpdateCurDatas()
{
	InitCtrlsData();

	// symmteric
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
	for(int i = 0; i < 33 ; i++)
		m_Edits[i].SetEditUnit(m_SectData.SectBefore.SectI.Size[i]);
 
	UpdateData(FALSE);
}

void CCMSectItemPSC_I::InitSectTypeCombo()
{
	m_cobxTypeName.ResetContent();

	CStringArray aTypeNameList;
	int nNum = m_pSectDB->GetTypeNameList(aTypeNameList);
	for (int i=0; i<nNum; i++)
		m_cobxTypeName.AddString(aTypeNameList[i]);
	m_cobxTypeName.SetCurSel(0);
	
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_cobxTypeName.GetDC();
	for (int i=0;i < m_cobxTypeName.GetCount();i++)
	{
		m_cobxTypeName.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_cobxTypeName.ReleaseDC(pDC);

	//dx += ::GetSystemMetrics(SM_CXVSCROLL); + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_cobxTypeName.GetDroppedWidth() < dx)
	{
		m_cobxTypeName.SetDroppedWidth(dx);
		ASSERT(m_cobxTypeName.GetDroppedWidth() == dx);
	}
}

void CCMSectItemPSC_I::InitSectNameCombo()
{
	m_cobxSectName.ResetContent();

	CString strTypeName;
	int nIndex = m_cobxTypeName.GetCurSel();
	if (nIndex == CB_ERR) return;
	m_cobxTypeName.GetLBText(nIndex, strTypeName);
		  
	CStringArray aDBNameList;
	int nNum = m_pSectDB->GetDBNameList(strTypeName, aDBNameList);
	for (int i=0; i<nNum; i++)
		m_cobxSectName.AddString(aDBNameList[i]);
	m_cobxSectName.SetCurSel(0);
	
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_cobxSectName.GetDC();
	for (int i=0;i < m_cobxSectName.GetCount();i++)
	{
		m_cobxSectName.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_cobxSectName.ReleaseDC(pDC);

	//dx += ::GetSystemMetrics(SM_CXVSCROLL); + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_cobxSectName.GetDroppedWidth() < dx)
	{
		m_cobxSectName.SetDroppedWidth(dx);
		ASSERT(m_cobxSectName.GetDroppedWidth() == dx);
	}
}

BOOL CCMSectItemPSC_I::OnInitDialog() 
{
	CCMSectItemPSCBase::OnInitDialog();
	
	// TODO: Add extra initialization here	
	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();

	for( int i = 0 ;  i < 33 ; i++)
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

	InitSectTypeCombo();
	InitSectNameCombo();
	UpdateCurDatas();
	UpdateCtrls();

	// Update Viewer
	RedrawSection();
	ChangeBitmap(__SECT_PSC_I__);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
BOOL CCMSectItemPSC_I::DestroyWindow() 
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
void CCMSectItemPSC_I::SyncCheckData(BOOL bSync)
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

void CCMSectItemPSC_I::OnCmdCheckLeftCtrls() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByJoint();
	SyncCheckData(m_bSymmetry);
	RedrawSection();
}

void CCMSectItemPSC_I::OnCmdCheckCtrls() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByJoint();
	RedrawSection();
}


void CCMSectItemPSC_I::SyncEditData(BOOL bSync)
{
	if(bSync)
	{
		for(int i = 1; i < 17 ; i++)  
		{
			if(m_Edits[i].GetEditValue()!=m_Edits[i+16].GetEditValue())
			{
				m_Edits[i+16].SetEditUnit(m_Edits[i].GetEditValue());
			}
		}
	}
}

void CCMSectItemPSC_I::OnChangeCmdLeftEdit()
{
	SyncEditData(m_bSymmetry);
	RedrawSection();
}

void CCMSectItemPSC_I::OnChangeCmdEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	RedrawSection();
}

void CCMSectItemPSC_I::OnSymmetryCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrls();
	RedrawSection();
	SyncEditData(m_bSymmetry);
	SyncCheckData(m_bSymmetry);
}

void CCMSectItemPSC_I::OnSelchangeCmdSectTypeCombo() 
{
	InitSectNameCombo();
	OnSelchangeCmdSectNameCombo();
}

void CCMSectItemPSC_I::OnSelchangeCmdSectNameCombo() 
{
	// TODO: Add your control notification handler code here
	CString csDBName;
	int nIndex = m_cobxSectName.GetCurSel();
	if (nIndex == CB_ERR) return;
	m_cobxSectName.GetLBText(nIndex, csDBName);
	//if (csDBName == CMD_SECT_NONE) return;

	ASSERT(m_pSectDB);

	CArray<double, double> aSize;
	CArray<BOOL, BOOL> aOffet;
	if (!m_pSectDB->GetDBData(csDBName, aOffet, aSize)) return;
	m_pParent->SetIDName(csDBName);

	m_bJ1  = aOffet[0];
	m_bJL1 = aOffet[1];
	m_bJL2 = aOffet[2];
	m_bJL3 = aOffet[3];
	m_bJL4 = aOffet[4];
	m_bJR1 = aOffet[5];
	m_bJR2 = aOffet[6];
	m_bJR3 = aOffet[7];
	m_bJR4 = aOffet[8];

	for(int i = 0; i < 33 ; i++)
		m_Edits[i].SetEditUnit(aSize[i]);

	UpdateData(FALSE);

	UpdateCtrls();
	RedrawSection();
}

void CCMSectItemPSC_I::OnCmdSectCentroidBtn() 
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

void CCMSectItemPSC_I::OnCmdChangeOffsetBtn() 
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

void CCMSectItemPSC_I::OnChangeCmdShearEdit() 
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

void CCMSectItemPSC_I::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	ChangeShearCheck(m_SectData.SectBefore.bShearCheck);
	RedrawSection();
}

void CCMSectItemPSC_I::ChangeShearCheck(int nCheck)
{
	m_chkShear1.EnableWindow(nCheck == 1);
	m_chkShear3.EnableWindow(nCheck == 1);
	//m_editShear1.EnableWindow(nCheck == 1);
	//m_editShear2.EnableWindow(nCheck == 1);
	//m_editShear3.EnableWindow(nCheck == 1);
	ChangeAutoShearCheck1();
	ChangeAutoShearCheck3();
}

void CCMSectItemPSC_I::OnCmdShear1Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck1();
	RedrawSection();
}

void CCMSectItemPSC_I::ChangeAutoShearCheck1()
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

void CCMSectItemPSC_I::OnCmdShear3Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck3();
	RedrawSection();
}

void CCMSectItemPSC_I::ChangeAutoShearCheck3()
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

void CCMSectItemPSC_I::OnCmdWebICheck() 
{
	// TODO: Add your control notification handler code here]
	m_SectData.SectBefore.bAutoWebThk[0] = m_chkWebI.GetCheck();
	m_editWebI.EnableWindow(!m_SectData.SectBefore.bAutoWebThk[0]);
}

void CCMSectItemPSC_I::OnCmdWebICheck2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = (m_chkWebShear[i].GetCheck()==1);
		m_editWebShear[i].EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][i]);
	}
}

void CCMSectItemPSC_I::SaveDataForDrawSection()
{
	UpdateData(TRUE);

	// symmetric
	m_SectData.SectBefore.bSymmetric = m_bSymmetry;

	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_I;
		
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
	m_SectData.SectBefore.nJoint = 0; // 초기화 
	if(m_SectData.SectBefore.bSymmetric)
	{
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
		for(int i = 0; i < 17 ; i++)  
			m_SectData.SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();
		for(int i = 1; i < 17 ; i++)
			m_SectData.SectBefore.SectI.Size[i+16] =  m_Edits[i].GetEditValue();
	}
	else
	{
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

int CCMSectItemPSC_I::_GetHeightFromFont()
{
	 CFont * pNewFont  = GetFont();
	 CFont *pSysFont,*pOldFont,TempFont;
	 CDC   *pCdc;
	 TEXTMETRIC  tmNew,tmSys;
	 int     nTemp,nCorrectVal;
 
	 //get the DC for the edit control
	 pCdc = GetDC();
 
	 //get the metrics for the system font
	 pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	 pOldFont  = pCdc->SelectObject( pSysFont);
	 pCdc->GetTextMetrics(&tmSys);
 
	 //get the metrics for the new font
	 pCdc->SelectObject(pNewFont);
	 pCdc->GetTextMetrics(&tmNew);
 
	 //select the original font back into the DC and release the DC
	 pCdc->SelectObject(pOldFont);
	 ReleaseDC(pCdc);
	 
	 nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight)/2);
	 //calculate the new height for the edit control
	 nTemp = tmNew.tmHeight + nCorrectVal;
	 //nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	 return nTemp;
}

///////////////////////////////////////////////////////////////////////////////
// 
// Update Viewer
//
BOOL CCMSectItemPSC_I::ChangeBitmap(int nIndex)
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(nIndex);
}

void CCMSectItemPSC_I::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSC_I::RedrawSection()
{
	ASSERT(m_pParent);
	SaveDataForDrawSection();
	m_pParent->RedrawSection();
}


void CCMSectItemPSC_I::OnCmdSectTableinputBtn() 
{
	CCMSectItemPSCIGridDlg m_GridDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_I Cell;
	Cell.Set(*sect,TRUE,TRUE);
	m_GridDlg.SetData(sect,this);
	if(m_GridDlg.DoModal()!=IDOK)
	{
		Cell.Get(*sect,TRUE,TRUE);
		UpdateCurDatas(); 
		UpdateCtrls();
		RedrawSection();   
	}	
}


void CCMSectItemPSC_I::OnCmdSectWapingCheckBtn() 
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

void CCMSectItemPSC_I::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSC_I::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}
