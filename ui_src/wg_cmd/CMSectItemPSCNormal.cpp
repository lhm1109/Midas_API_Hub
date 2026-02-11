// CMSectItemPSCNormal.cpp : implementation file
// 2002. 7. 2 by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCNormal.h"

#include "CMSectItemPSC.h"
#include "CMSectOffsetDlg.h"
#include "CMSectItemPSCNormalGridDlg.h"
#include "CMSectItemPSC_WarpingCheckDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCNormal dialog
void CCMSectItemPSCNormal::InitCtrlsData()
{
	m_bJI1 = FALSE;
	m_bJI2 = FALSE;
	m_bJI3 = FALSE;
	m_bJI4 = FALSE;
	m_bJI5 = FALSE;
	m_bJO1 = FALSE;
	m_bJO2 = FALSE;
	m_bJO3 = FALSE;
	m_nCellType = 0;
	m_nHalfType = 0;
	m_nWarpingCheck= 0;
}

CCMSectItemPSCNormal::CCMSectItemPSCNormal(CWnd* pParent /*=NULL*/, int nType /*=0*/)
	: CCMSectItemPSCBase(CCMSectItemPSCNormal::IDD, pParent)
{
	//  InitCtrlsData();

	//{{AFX_DATA_INIT(CCMSectItemPSCNormal)
	m_bJI1 = FALSE;
	m_bJI2 = FALSE;
	m_bJI3 = FALSE;
	m_bJI4 = FALSE;
	m_bJI5 = FALSE;
	m_bJO1 = FALSE;
	m_bJO2 = FALSE;
	m_bJO3 = FALSE;
	m_nCellType = 0;
	m_nHalfType = 0;
	m_nWarpingCheck    = 0;
	//}}AFX_DATA_INIT

	m_bHalfType = nType;

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	// Joint Group
	arJO1Group.Add(IDC_CMD_HO21_EDIT);  arJO1Group.Add(IDC_CMD_BO11_EDIT);
	arJO2Group.Add(IDC_CMD_HO22_EDIT);  arJO2Group.Add(IDC_CMD_BO12_EDIT);
	arJO3Group.Add(IDC_CMD_HO31_EDIT);  arJO3Group.Add(IDC_CMD_BO21_EDIT);
	arJI1Group.Add(IDC_CMD_HI21_EDIT);  arJI1Group.Add(IDC_CMD_BI11_EDIT2);
	arJI2Group.Add(IDC_CMD_HI22_EDIT);  arJI2Group.Add(IDC_CMD_BI12_EDIT2);
	arJI3Group.Add(IDC_CMD_HI31_EDIT);  arJI3Group.Add(IDC_CMD_BI21_EDIT2);
	arJI4Group.Add(IDC_CMD_HI42_EDIT2); arJI4Group.Add(IDC_CMD_BI32_EDIT2);
	arJI5Group.Add(IDC_CMD_HI41_EDIT2); arJI5Group.Add(IDC_CMD_BI31_EDIT2);

	// Half Group
	arComnGroup.Add(IDC_CMD_JI1_CHECK2);  arComnGroup.Add(IDC_CMD_JI2_CHECK);
	arComnGroup.Add(IDC_CMD_JI3_CHECK2);  arComnGroup.Add(IDC_CMD_JI4_CHECK);
	arComnGroup.Add(IDC_CMD_JI5_CHECK);
	arNoneGroup.Add(IDC_CMD_HI2_EDIT);    arNoneGroup.Add(IDC_CMD_HI3_EDIT);
	arNoneGroup.Add(IDC_CMD_HI4_EDIT2);   arNoneGroup.Add(IDC_CMD_HI5_EDIT2);
	arNoneGroup.Add(IDC_CMD_BI1_EDIT);    arNoneGroup.Add(IDC_CMD_BI3_EDIT2);
	arCircGroup.Add(IDC_CMD_HI2_EDIT);    arCircGroup.Add(IDC_CMD_HI4_EDIT2);
	arCircGroup.Add(IDC_CMD_BI1_EDIT);    arCircGroup.Add(IDC_CMD_BI3_EDIT2);

	// Normal & Half Group
	arNormGroup.Add(IDC_SECTYPE_STATIC);
	arNormGroup.Add(IDC_CMD_SECT_1CELL_RADIO);
	arNormGroup.Add(IDC_CMD_SECT_2CELL_RADIO);
	arNormGroup.Add(IDC_BI4_STATIC);
	arNormGroup.Add(IDC_CMD_BI4_EDIT2);
	arNormGroup.Add(IDC_CMD_BI4_UNIT);
	arHalfGroup.Add(IDC_CMD_PSC_SECTYPE_CB);
	arHalfGroup.Add(IDC_CELLTYPE_STATIC);
	arHalfGroup.Add(IDC_CMD_PSC_NONE_RADIO);
	arHalfGroup.Add(IDC_CMD_PSC_CIRC_RADIO);
	arHalfGroup.Add(IDC_CMD_PSC_POLY_RADIO);

	// Shear Group
	arShearGroup.Add(IDC_WG_CMD_STATIC0);
	arShearGroup.Add(IDC_WG_CMD_STATIC38);
	arShearGroup.Add(IDC_WG_CMD_STATIC35);
	arShearGroup.Add(IDC_CMD_SHEAR1_EDIT);
	arShearGroup.Add(IDC_CMD_SHEAR1_UNIT);
	arShearGroup.Add(IDC_CMD_SHEAR1_CHECK);
	arShearGroup.Add(IDC_WG_CMD_STATIC37);
	arShearGroup.Add(IDC_WG_CMD_STATIC36);
	arShearGroup.Add(IDC_CMD_SHEAR3_EDIT);
	arShearGroup.Add(IDC_CMD_SHEAR3_UNIT);
	arShearGroup.Add(IDC_CMD_SHEAR3_CHECK);

	// Web Group
	arWebGroup.Add(IDC_WG_CMD_STATIC39);
	arWebGroup.Add(IDC_WG_CMD_STATIC41);
	arWebGroup.Add(IDC_WG_CMD_STATIC43);
	arWebGroup.Add(IDC_WG_CMD_STATIC44);
	arWebGroup.Add(IDC_CMD_WEB_I_EDIT2);
	arWebGroup.Add(IDC_CMD_WEB_I_UNIT2);
	arWebGroup.Add(IDC_CMD_WEB_I_CHECK2);
	arWebGroup.Add(IDC_WG_CMD_STATIC45);
	arWebGroup.Add(IDC_CMD_WEB_I_EDIT3);
	arWebGroup.Add(IDC_CMD_WEB_I_UNIT3);
	arWebGroup.Add(IDC_CMD_WEB_I_CHECK3);
	arWebGroup.Add(IDC_WG_CMD_STATIC46);
	arWebGroup.Add(IDC_CMD_WEB_I_EDIT4);
	arWebGroup.Add(IDC_CMD_WEB_I_UNIT4);
	arWebGroup.Add(IDC_CMD_WEB_I_CHECK4);
	arWebGroup.Add(IDC_WG_CMD_STATIC42);
	arWebGroup.Add(IDC_CMD_WEB_I_EDIT);
	arWebGroup.Add(IDC_CMD_WEB_I_UNIT);
	arWebGroup.Add(IDC_CMD_WEB_I_CHECK);
}


void CCMSectItemPSCNormal::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCNormal)
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
	DDX_Control(pDX, IDC_CMD_PSC_SECTYPE_CB, m_cSecTypeCB);
	DDX_Check(pDX, IDC_CMD_JI1_CHECK2, m_bJI1);
	DDX_Check(pDX, IDC_CMD_JI2_CHECK, m_bJI2);
	DDX_Check(pDX, IDC_CMD_JI3_CHECK2, m_bJI3);
	DDX_Check(pDX, IDC_CMD_JI4_CHECK, m_bJI4);
	DDX_Check(pDX, IDC_CMD_JI5_CHECK, m_bJI5);
	DDX_Check(pDX, IDC_CMD_JO1_CHECK, m_bJO1);
	DDX_Check(pDX, IDC_CMD_JO2_CHECK, m_bJO2);
	DDX_Check(pDX, IDC_CMD_JO3_CHECK, m_bJO3);
	DDX_Radio(pDX, IDC_CMD_SECT_1CELL_RADIO, m_nCellType);
	DDX_Radio(pDX, IDC_CMD_PSC_NONE_RADIO, m_nHalfType);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);

	//}}AFX_DATA_MAP

	CArray<UINT , UINT > UnitIDs;
	UnitIDs.Add(IDC_CMD_HO1_UNIT);  UnitIDs.Add(IDC_CMD_HO2_UNIT);
	UnitIDs.Add(IDC_CMD_HO21_UNIT); UnitIDs.Add(IDC_CMD_HO22_UNIT);
	UnitIDs.Add(IDC_CMD_HO3_UNIT);  UnitIDs.Add(IDC_CMD_HO31_UNIT);
	UnitIDs.Add(IDC_CMD_BO1_UNIT);  UnitIDs.Add(IDC_CMD_BO11_UNIT);
	UnitIDs.Add(IDC_CMD_BO12_UNIT); UnitIDs.Add(IDC_CMD_BO2_UNIT);
	UnitIDs.Add(IDC_CMD_BO21_UNIT); UnitIDs.Add(IDC_CMD_BO3_UNIT);
	UnitIDs.Add(IDC_CMD_HI1_UNIT);  UnitIDs.Add(IDC_CMD_HI2_UNIT);
	UnitIDs.Add(IDC_CMD_HI21_UNIT); UnitIDs.Add(IDC_CMD_HI22_UNIT);
	UnitIDs.Add(IDC_CMD_HI3_UNIT);  UnitIDs.Add(IDC_CMD_HI31_UNIT);
	UnitIDs.Add(IDC_CMD_HI4_UNIT);  UnitIDs.Add(IDC_CMD_HI41_UNIT);
	UnitIDs.Add(IDC_CMD_HI42_UNIT); UnitIDs.Add(IDC_CMD_HI5_UNIT);
	UnitIDs.Add(IDC_CMD_BI1_UNIT);  UnitIDs.Add(IDC_CMD_BI11_UNIT);
	UnitIDs.Add(IDC_CMD_BI12_UNIT); UnitIDs.Add(IDC_CMD_BI21_UNIT);
	UnitIDs.Add(IDC_CMD_BI3_UNIT);  UnitIDs.Add(IDC_CMD_BI31_UNIT);
	UnitIDs.Add(IDC_CMD_BI32_UNIT); UnitIDs.Add(IDC_CMD_BI4_UNIT);
	for(int i = 0; i < 30 ; i++)
	{
		DDX_Control(pDX,UnitIDs[i],m_Units[i]);
	}

	CArray<UINT , UINT> EditIDs;
	EditIDs.Add(IDC_CMD_HO1_EDIT);  EditIDs.Add(IDC_CMD_HO2_EDIT);
	EditIDs.Add(IDC_CMD_HO21_EDIT); EditIDs.Add(IDC_CMD_HO22_EDIT);
	EditIDs.Add(IDC_CMD_HO3_EDIT);  EditIDs.Add(IDC_CMD_HO31_EDIT);
	EditIDs.Add(IDC_CMD_BO1_EDIT);  EditIDs.Add(IDC_CMD_BO11_EDIT);
	EditIDs.Add(IDC_CMD_BO12_EDIT); EditIDs.Add(IDC_CMD_BO2_EDIT);
	EditIDs.Add(IDC_CMD_BO21_EDIT); EditIDs.Add(IDC_CMD_BO3_EDIT);
	EditIDs.Add(IDC_CMD_HI1_EDIT);  EditIDs.Add(IDC_CMD_HI2_EDIT);
	EditIDs.Add(IDC_CMD_HI21_EDIT); EditIDs.Add(IDC_CMD_HI22_EDIT);
	EditIDs.Add(IDC_CMD_HI3_EDIT);  EditIDs.Add(IDC_CMD_HI31_EDIT);
	EditIDs.Add(IDC_CMD_HI4_EDIT2); EditIDs.Add(IDC_CMD_HI41_EDIT2);
	EditIDs.Add(IDC_CMD_HI42_EDIT2);EditIDs.Add(IDC_CMD_HI5_EDIT2);
	EditIDs.Add(IDC_CMD_BI1_EDIT);  EditIDs.Add(IDC_CMD_BI11_EDIT2);
	EditIDs.Add(IDC_CMD_BI12_EDIT2);EditIDs.Add(IDC_CMD_BI21_EDIT2);
	EditIDs.Add(IDC_CMD_BI3_EDIT2); EditIDs.Add(IDC_CMD_BI31_EDIT2);
	EditIDs.Add(IDC_CMD_BI32_EDIT2);EditIDs.Add(IDC_CMD_BI4_EDIT2);
	for(int i = 0; i < 30; i++)
	{
		DDX_Control(pDX,EditIDs[i],m_Edits[i]);
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
}

BEGIN_MESSAGE_MAP(CCMSectItemPSCNormal, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSCNormal)
	ON_BN_CLICKED(IDC_CMD_JI1_CHECK2, OnCmdCheckCtrl)
	ON_CBN_SELCHANGE(IDC_CMD_PSC_SECTYPE_CB, OnSelchangeCmdPscSectypeCb)
	ON_BN_CLICKED(IDC_CMD_PSC_NONE_RADIO, OnCmdCheckCtrls2)
	ON_BN_CLICKED(IDC_CMD_SECT_1CELL_RADIO, OnCmdSectCellRadio)
	ON_EN_CHANGE(IDC_CMD_BI1_EDIT, OnChangeCmdSize)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK, OnCmdShearCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_CHECK, OnCmdShear1Check)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_CHECK, OnCmdShear3Check)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK, OnCmdWebICheck)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK2, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_JI2_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI3_CHECK2, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI4_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI5_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JO1_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JO2_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JO3_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_PSC_CIRC_RADIO, OnCmdCheckCtrls2)
	ON_BN_CLICKED(IDC_CMD_PSC_POLY_RADIO, OnCmdCheckCtrls2)
	ON_BN_CLICKED(IDC_CMD_SECT_2CELL_RADIO, OnCmdSectCellRadio)
	ON_EN_CHANGE(IDC_CMD_BI11_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BI12_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BI21_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BI3_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BI31_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BI32_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BI4_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BO1_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BO11_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BO12_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BO2_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BO21_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_BO3_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI1_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI2_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI21_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI22_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI3_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI31_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI4_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI41_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI42_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HI5_EDIT2, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HO1_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HO2_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HO21_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HO22_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HO3_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_HO31_EDIT, OnChangeCmdSize)
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
// CCMSectItemPSCNormal message handlers

////////////////////////////////////////////////////////////////////////////
//
// External Data Coontrol Functions
//

BOOL CCMSectItemPSCNormal::GetDlgData(void* pData)
{
	UpdateData(TRUE);  

	// Window가 죽기전에 불린다.
	T_SECT_D * pSect = (T_SECT_D*) pData;  

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

	// offset
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
	
	// Section Type
	pSect->SectBefore.nJoint = 0;

	if(m_bHalfType)
	{
		pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_HALF;
		pSect->SectBefore.nCellType = m_cSecTypeCB.GetCurSel();
		pSect->SectBefore.nCellShape = m_nHalfType;

		// Joint
		if(m_bJO1) pSect->SectBefore.nJoint |= 0x00000001 ;
		if(m_bJO2) pSect->SectBefore.nJoint |= 0x00000002 ;
		if(m_bJO3) pSect->SectBefore.nJoint |= 0x00000004 ;
		if(m_bJI1) pSect->SectBefore.nJoint |= 0x00000008 ;
		if(m_bJI2) pSect->SectBefore.nJoint |= 0x00000010 ;
		if(m_bJI3) pSect->SectBefore.nJoint |= 0x00000020 ;
		if(m_bJI4) pSect->SectBefore.nJoint |= 0x00000040 ;
		if(m_bJI5) pSect->SectBefore.nJoint |= 0x00000080 ;
	}
	else
	{
		if(m_nCellType == 0)      pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_1CELL;
		else if(m_nCellType == 1) pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_2CELL;
		else ASSERT(0);

		// Joint
		if(m_bJO1) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000001 ;
		if(m_bJO2) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000002 ;
		if(m_bJO3) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000004 ;
		if(m_bJI1) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000008 ;
		if(m_bJI2) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000010 ;
		if(m_bJI3) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000020 ;
		if(m_bJI4) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000040 ;
		if(m_bJI5) pSect->SectBefore.SectI.BuiltUpFlag |= 0x00000080 ;
	}

	for(int i=0; i<30; i++)
		pSect->SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();

	return TRUE;
}

void  CCMSectItemPSCNormal::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D *)pData);
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

void CCMSectItemPSCNormal::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
}

void CCMSectItemPSCNormal::SetAutotShearCheckPos(double dShearPos[3])
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

void CCMSectItemPSCNormal::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_chkWebShear[i].GetCheck() == 1) 
			m_editWebShear[i].SetEditUnit(dShear[i]);
	}
	if (m_chkWebI.GetCheck() == 1) m_editWebI.SetEditUnit(dTorsion);
}

void CCMSectItemPSCNormal::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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

////////////////////////////////////////////////////////////////////////////
//
// Initilaize
//

void CCMSectItemPSCNormal::AlignControls()
{
	CRect rRef, ToMove;
	GetDlgItem(IDC_SECTYPE_STATIC)->GetWindowRect(&rRef);
	GetDlgItem(IDC_CELLTYPE_STATIC)->GetWindowRect(&ToMove);
	int nDistY = rRef.top - ToMove.top;
	int nDistX = rRef.left - ToMove.left;
	CDlgUtil::CtrlMoveDistXY(this, arHalfGroup, nDistX, nDistY);
	
	if(m_bHalfType)
	{
		CDlgUtil::CtrlShowHide(this, arNormGroup, FALSE);
		CDlgUtil::CtrlShowHide(this, arHalfGroup, TRUE);

		GetDlgItem(IDC_CELLTYPE_STATIC)->GetWindowRect(&rRef);
		GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&ToMove);
		nDistY = rRef.bottom - ToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, arShearGroup, nDistY);
		CDlgUtil::CtrlMoveDistY(this, arWebGroup, nDistY);
	}
	else 
	{
		CDlgUtil::CtrlShowHide(this, arNormGroup, TRUE);
		CDlgUtil::CtrlShowHide(this, arHalfGroup, FALSE);

		GetDlgItem(IDC_SECTYPE_STATIC)->GetWindowRect(&rRef);
		GetDlgItem(IDC_WG_CMD_STATIC0)->GetWindowRect(&ToMove);
		nDistY = rRef.bottom - ToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, arShearGroup, nDistY);
		CDlgUtil::CtrlMoveDistY(this, arWebGroup, nDistY);
	}
}

void CCMSectItemPSCNormal::UpdateCtrlsByJoint()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, arJO1Group, m_bJO1);
	CDlgUtil::CtrlEnableDisable(this, arJO2Group, m_bJO2);
	CDlgUtil::CtrlEnableDisable(this, arJO3Group, m_bJO3);
	CDlgUtil::CtrlEnableDisable(this, arJI1Group, m_bJI1);
	CDlgUtil::CtrlEnableDisable(this, arJI2Group, m_bJI2);
	CDlgUtil::CtrlEnableDisable(this, arJI3Group, m_bJI3);
	CDlgUtil::CtrlEnableDisable(this, arJI4Group, m_bJI4);
	CDlgUtil::CtrlEnableDisable(this, arJI5Group, m_bJI5);
}

void CCMSectItemPSCNormal::UpdateCtrlsByCell()
{
	UpdateData(TRUE);

	CString strTemp;
	int nIndex = m_cSecTypeCB.GetCurSel();
	m_cSecTypeCB.GetLBText(nIndex, strTemp);

	CDlgUtil::CtrlEnableDisable(this, arComnGroup, TRUE);
	CDlgUtil::CtrlEnableDisable(this, arNoneGroup, TRUE);
	CDlgUtil::CtrlEnableDisable(this, arCircGroup, TRUE);

	if(m_nHalfType == 0)  // None
	{
		m_bJI1 = m_bJI2 = m_bJI3 = m_bJI4 = m_bJI5 = FALSE;
		UpdateData(FALSE);
		CDlgUtil::CtrlEnableDisable(this, arComnGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, arNoneGroup, FALSE);

		if(strTemp == _LS(IDS_CMD_PSC_HALF__Left)) this->ChangeBitmap(__SECT_PSC_HLF_LEFT_NONE__);
		else if(strTemp == _LS(IDS_CMD_PSC_HALF__Right)) this->ChangeBitmap(__SECT_PSC_HLF_RGHT_NONE__);
	}
	else if(m_nHalfType == 1) // Circ
	{
		m_bJI1 = m_bJI2 = m_bJI3 = m_bJI4 = m_bJI5 = FALSE;
		UpdateData(FALSE);
		CDlgUtil::CtrlEnableDisable(this, arComnGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, arCircGroup, FALSE);

		if(strTemp == _LS(IDS_CMD_PSC_HALF__Left)) this->ChangeBitmap(__SECT_PSC_HLF_LEFT_CIRC__);
		else if(strTemp == _LS(IDS_CMD_PSC_HALF__Right)) this->ChangeBitmap(__SECT_PSC_HLF_RGHT_CIRC__);
	}
	else if(m_nHalfType == 2) // Poly
	{
		if(strTemp == _LS(IDS_CMD_PSC_HALF__Left)) this->ChangeBitmap(__SECT_PSC_HLF_LEFT_POLY__);
		else if(strTemp == _LS(IDS_CMD_PSC_HALF__Right)) this->ChangeBitmap(__SECT_PSC_HLF_RGHT_POLY__);
	}
	else ASSERT(0);

}

void CCMSectItemPSCNormal::UpdateCtrlsBySect()
{
	// Normal 일때만
	UpdateData(TRUE);
	if(m_bHalfType) return;

	CWnd* pWnd = GetDlgItem(IDC_CMD_BI4_EDIT2);
	if(m_nCellType == 0) 
	{
		pWnd->EnableWindow(FALSE); 
		this->ChangeBitmap(__SECT_PSC_NML_1_CELL__);
	}
	else if(m_nCellType == 1)
	{
		pWnd->EnableWindow(TRUE);
		this->ChangeBitmap(__SECT_PSC_NML_2_CELL__);
	}
}

void CCMSectItemPSCNormal::UpdateCtrlsByType()
{
	UpdateData(TRUE);

	if(m_bHalfType) UpdateCtrlsByCell();
	else            UpdateCtrlsBySect();
	
	UpdateCtrlsByJoint();
}

void CCMSectItemPSCNormal::UpdateCurDatas()
{
	InitCtrlsData();

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

	// Section Type
	if(m_bHalfType)
	{
		if(m_SectData.SectBefore.nCellType >= 0)
			m_cSecTypeCB.SetCurSel(m_SectData.SectBefore.nCellType);
		m_nHalfType = m_SectData.SectBefore.nCellShape;

		// Joint
		if(m_SectData.SectBefore.nJoint & 0x00000001) m_bJO1 = TRUE ;
		if(m_SectData.SectBefore.nJoint & 0x00000002) m_bJO2 = TRUE ;
		if(m_SectData.SectBefore.nJoint & 0x00000004) m_bJO3 = TRUE ;
		if(m_SectData.SectBefore.nJoint & 0x00000008) m_bJI1 = TRUE ;
		if(m_SectData.SectBefore.nJoint & 0x00000010) m_bJI2 = TRUE ;
		if(m_SectData.SectBefore.nJoint & 0x00000020) m_bJI3 = TRUE ;
		if(m_SectData.SectBefore.nJoint & 0x00000040) m_bJI4 = TRUE ;
		if(m_SectData.SectBefore.nJoint & 0x00000080) m_bJI5 = TRUE ;
	}
	else
	{
		if(m_SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL)      m_nCellType = 0;
		else if(m_SectData.SectBefore.Shape == D_SECT_SHAPE_PSC_2CELL) m_nCellType = 1;
		else m_nCellType = 0; // 1Cell을 default로..

		// Joint
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000001) m_bJO1 = TRUE ;
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000002) m_bJO2 = TRUE ;
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000004) m_bJO3 = TRUE ;
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000008) m_bJI1 = TRUE ;
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000010) m_bJI2 = TRUE ;
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000020) m_bJI3 = TRUE ;
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000040) m_bJI4 = TRUE ;
		if(m_SectData.SectBefore.SectI.BuiltUpFlag & 0x00000080) m_bJI5 = TRUE ;
	}

	// Size
	for(int i = 0; i < 30 ; i++)
		m_Edits[i].SetEditUnit(m_SectData.SectBefore.SectI.Size[i]);

	UpdateData(FALSE);
	UpdateCtrlsByType();
	RedrawSection();
}

void CCMSectItemPSCNormal::InitSecTypeCombo()
{
	m_cSecTypeCB.ResetContent();
	m_cSecTypeCB.AddString(_LS(IDS_CMD_PSC_HALF__Left));
	m_cSecTypeCB.AddString(_LS(IDS_CMD_PSC_HALF__Right));
	m_cSecTypeCB.SetCurSel(0);
}


BOOL CCMSectItemPSCNormal::OnInitDialog() 
{ 
	// 일부 데이터 초기화 
	if (m_bHalfType)
	{
		if (!m_bModify)
		{
			m_SectData.SectBefore.nCellType  = 0;
			m_SectData.SectBefore.nCellShape = 2;
		}
		//else m_bModify = FALSE;
	}

	CCMSectItemPSCBase::OnInitDialog();
	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();

	for (int i = 0 ;  i < 30 ; i++)
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

	AlignControls();
	InitSecTypeCombo();
	UpdateCurDatas();	
	UpdateCtrlsByType();

	// draw section on viewer
	RedrawSection();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
BOOL CCMSectItemPSCNormal::DestroyWindow() 
{
	// Type이 Change될때 Data Initialize
	m_SectData.Initialize();
	if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
	return CCMSectItemPSCBase::DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////
//
// Eanble/Disable 
//
void CCMSectItemPSCNormal::OnCmdCheckCtrl() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByJoint();

	RedrawSection();
}

void CCMSectItemPSCNormal::OnSelchangeCmdPscSectypeCb() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByCell();

	RedrawSection();
}

void CCMSectItemPSCNormal::OnCmdCheckCtrls2() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByCell();

	RedrawSection();
}

void CCMSectItemPSCNormal::OnCmdSectCellRadio() 
{
	UpdateCtrlsBySect();

	RedrawSection();
}


void CCMSectItemPSCNormal::OnChangeCmdSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	RedrawSection();		
}

void CCMSectItemPSCNormal::OnCmdSectCentroidBtn() 
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


void CCMSectItemPSCNormal::OnCmdChangeOffsetBtn() 
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

void CCMSectItemPSCNormal::OnChangeCmdShearEdit() 
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

void CCMSectItemPSCNormal::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	ChangeShearCheck(m_SectData.SectBefore.bShearCheck);
	RedrawSection();
}

void CCMSectItemPSCNormal::ChangeShearCheck(int nCheck)
{
	m_chkShear1.EnableWindow(nCheck == 1);
	m_chkShear3.EnableWindow(nCheck == 1);
	//m_editShear1.EnableWindow(nCheck == 1);
	//m_editShear2.EnableWindow(nCheck == 1);
	//m_editShear3.EnableWindow(nCheck == 1);
	ChangeAutoShearCheck1();
	ChangeAutoShearCheck3();
}

void CCMSectItemPSCNormal::OnCmdShear1Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck1();
	RedrawSection();
}

void CCMSectItemPSCNormal::ChangeAutoShearCheck1()
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

void CCMSectItemPSCNormal::OnCmdShear3Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck3();
	RedrawSection();
}

void CCMSectItemPSCNormal::ChangeAutoShearCheck3()
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

void CCMSectItemPSCNormal::OnCmdWebICheck() 
{
	// TODO: Add your control notification handler code here]
	m_SectData.SectBefore.bAutoWebThk[0] = m_chkWebI.GetCheck();
	m_editWebI.EnableWindow(!m_SectData.SectBefore.bAutoWebThk[0]);
}

void CCMSectItemPSCNormal::OnCmdWebICheck2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = m_chkWebShear[i].GetCheck();
		m_editWebShear[i].EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][i]);
	}
}

void CCMSectItemPSCNormal::SaveDataForDrawSection()
{
	UpdateData(TRUE);

	m_SectData.nStype = D_SECT_TYPE_PSC;

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
	m_SectData.SectBefore.nJoint = 0;

	if(m_bHalfType)
	{
		m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_HALF;

		nIndex = m_cSecTypeCB.GetCurSel();
		if(nIndex>=0) m_SectData.SectBefore.nCellType = nIndex;
		m_SectData.SectBefore.nCellShape = m_nHalfType;

		m_SectData.SectBefore.nJoint = 0;
		if(m_bJO1) m_SectData.SectBefore.nJoint |= 0x00000001 ;
		if(m_bJO2) m_SectData.SectBefore.nJoint |= 0x00000002 ;
		if(m_bJO3) m_SectData.SectBefore.nJoint |= 0x00000004 ;
		if(m_bJI1) m_SectData.SectBefore.nJoint |= 0x00000008 ;
		if(m_bJI2) m_SectData.SectBefore.nJoint |= 0x00000010 ;
		if(m_bJI3) m_SectData.SectBefore.nJoint |= 0x00000020 ;
		if(m_bJI4) m_SectData.SectBefore.nJoint |= 0x00000040 ;
		if(m_bJI5) m_SectData.SectBefore.nJoint |= 0x00000080 ;
	}
	else
	{
		if(m_nCellType == 0) m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_1CELL;
		else if(m_nCellType == 1) m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_2CELL;

		m_SectData.SectBefore.SectI.BuiltUpFlag = 0;
		if(m_bJO1) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000001 ;
		if(m_bJO2) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000002 ;
		if(m_bJO3) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000004 ;
		if(m_bJI1) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000008 ;
		if(m_bJI2) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000010 ;
		if(m_bJI3) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000020 ;
		if(m_bJI4) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000040 ;
		if(m_bJI5) m_SectData.SectBefore.SectI.BuiltUpFlag |= 0x00000080 ;
	}

	for(int i=0; i<30; i++)
		m_SectData.SectBefore.SectI.Size[i] = m_Edits[i].GetEditValue();

}


////////////////////////////////////////////////////////////////////////////
//
// Update Viewer
//
BOOL CCMSectItemPSCNormal::ChangeBitmap(int nIndex)
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(nIndex);
}

void CCMSectItemPSCNormal::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSCNormal::RedrawSection()
{
	ASSERT(m_pParent);

	SaveDataForDrawSection();
	m_pParent->RedrawSection();
}


void CCMSectItemPSCNormal::OnCmdSectTableinputBtn() 
{
	CCMSectItemPSCNormalGridDlg m_GridDlg(this);
	T_SECT_D* sect=&m_SectData;
	if(IsHalfType())
	{
		T_SECT_D_PSC_HALF Cell2;
		Cell2.Set(*sect,TRUE,TRUE);
		m_GridDlg.SetData(sect,this);
	
		if(m_GridDlg.DoModal()!=IDOK)
		{
		
			Cell2.Get(*sect,TRUE,TRUE);
			UpdateCurDatas();
		
		}	
	}
	else
	{

		T_SECT_D_PSC_2CELL Cell2;
		Cell2.Set(*sect,TRUE,TRUE);
		m_GridDlg.SetData(sect,this);
	
		if(m_GridDlg.DoModal()!=IDOK)
		{
		
			Cell2.Get(*sect,TRUE,TRUE);
			UpdateCurDatas();
		
		}	
	}
	
}

void CCMSectItemPSCNormal::OnCmdSectWapingCheckBtn() 
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

void CCMSectItemPSCNormal::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSCNormal::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}

int CCMSectItemPSCNormal::GetSellType()
{
	return m_nCellType;
}

int CCMSectItemPSCNormal::GetHalfType()
{
	return m_nHalfType;
}

BOOL CCMSectItemPSCNormal::IsHalfType()
{
	return m_bHalfType;
}