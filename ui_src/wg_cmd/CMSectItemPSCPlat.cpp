// CMSectItemPSCPlat.cpp : implementation file
// 2002. 8. 14.    by TAE
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCPlat.h"

#include "CMSectItemPSC.h"
#include "CMSectOffsetDlg.h"
#include "CMSectItemPSCPlatDB.h"
#include "CMSectItemPSCPlatGridDlg.h"
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
// CCMSectItemPSCPlat dialog

CCMSectItemPSCPlat::CCMSectItemPSCPlat(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSCPlat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCPlat)
	m_bJL1 = FALSE;
	m_bJR1 = FALSE;
	m_nSecType = 1;
	m_bSymmetry = TRUE;
	//}}AFX_DATA_INIT

	m_nWarpingCheck= 0;

	// Save Parent Pointer
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	m_pSectDB = NULL;
	m_pSectDB = new CCMSectItemPSCPlatDB();

	// Make Enable/Disable Group
	LeftDefaltGroup.Add(IDC_CMD_HOL1_EDIT);
	LeftDefaltGroup.Add(IDC_CMD_HOL2_EDIT);
	LeftDefaltGroup.Add(IDC_CMD_HOL3_EDIT);
	LeftDefaltGroup.Add(IDC_CMD_BOL1_EDIT);
	LeftDefaltGroup.Add(IDC_CMD_BOL2_EDIT);
	LeftDefaltGroup.Add(IDC_CMD_BOL3_EDIT);

	LeftOptionGroup.Add(IDC_CMD_HIL1_EDIT);
	LeftOptionGroup.Add(IDC_CMD_HIL2_EDIT);
	LeftOptionGroup.Add(IDC_CMD_BIL1_EDIT);
	LeftOptionGroup.Add(IDC_CMD_BIL2_EDIT);
	LeftOptionGroup.Add(IDC_CMD_BIL3_EDIT);

	RghtDefaltGroup.Add(IDC_CMD_HOR1_EDIT);
	RghtDefaltGroup.Add(IDC_CMD_HOR2_EDIT);
	RghtDefaltGroup.Add(IDC_CMD_HOR3_EDIT);
	RghtDefaltGroup.Add(IDC_CMD_BOR1_EDIT);
	RghtDefaltGroup.Add(IDC_CMD_BOR2_EDIT);
	RghtDefaltGroup.Add(IDC_CMD_BOR3_EDIT);

	RghtOptionGroup.Add(IDC_CMD_HIR1_EDIT);
	RghtOptionGroup.Add(IDC_CMD_HIR2_EDIT);
	RghtOptionGroup.Add(IDC_CMD_BIR1_EDIT);
	RghtOptionGroup.Add(IDC_CMD_BIR2_EDIT);
	RghtOptionGroup.Add(IDC_CMD_BIR3_EDIT);

	CellOptionGroup.Add(IDC_CMD_BIL4_EDIT);

	JL1Group.Add(IDC_CMD_HOL11_EDIT);
	JL1Group.Add(IDC_CMD_BOL11_EDIT);

	JR1Group.Add(IDC_CMD_HOR11_EDIT);
	JR1Group.Add(IDC_CMD_BOR11_EDIT);
}

CCMSectItemPSCPlat::~CCMSectItemPSCPlat()
{
	if (m_pSectDB != NULL)
	{
		delete m_pSectDB;
		m_pSectDB = NULL;
	}
}

void CCMSectItemPSCPlat::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCPlat)
	DDX_Control(pDX, IDC_CMD_WEB_I_CHECK, m_chkWebI);
	DDX_Control(pDX, IDC_CMD_SHEAR3_CHECK, m_chkShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR1_CHECK, m_chkShear1);
	DDX_Control(pDX, IDC_CMD_SECT_NAME_COMBO, m_cobxSectName);
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
	DDX_Control(pDX, IDC_CMD_HALF_CB, m_cboHalf);
	DDX_Control(pDX, IDC_CMD_1CELL_CB, m_cbo1Cell);
	DDX_Check(pDX, IDC_CMD_JL1_CHECK, m_bJL1);
	DDX_Check(pDX, IDC_CMD_JR1_CHECK, m_bJR1);
	DDX_Radio(pDX, IDC_CMD_SECTYPE_RD1, m_nSecType);
	DDX_Check(pDX, IDC_SYMMETRY_CHECK, m_bSymmetry);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	//}}AFX_DATA_MAP

	CArray<UINT, UINT> arCtrlIDs;
	arCtrlIDs.Add(IDC_CMD_H1_EDIT);
	arCtrlIDs.Add(IDC_CMD_H2_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOL1_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOL2_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOL3_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOL1_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOL2_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOL3_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOL11_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOL11_EDIT);
	arCtrlIDs.Add(IDC_CMD_HIL1_EDIT);
	arCtrlIDs.Add(IDC_CMD_HIL2_EDIT);
	arCtrlIDs.Add(IDC_CMD_BIL1_EDIT);
	arCtrlIDs.Add(IDC_CMD_BIL2_EDIT);
	arCtrlIDs.Add(IDC_CMD_BIL3_EDIT);
	arCtrlIDs.Add(IDC_CMD_BIL4_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOR1_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOR2_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOR3_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOR1_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOR2_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOR3_EDIT);
	arCtrlIDs.Add(IDC_CMD_HOR11_EDIT);
	arCtrlIDs.Add(IDC_CMD_BOR11_EDIT);
	arCtrlIDs.Add(IDC_CMD_HIR1_EDIT);
	arCtrlIDs.Add(IDC_CMD_HIR2_EDIT);
	arCtrlIDs.Add(IDC_CMD_BIR1_EDIT);
	arCtrlIDs.Add(IDC_CMD_BIR2_EDIT);
	arCtrlIDs.Add(IDC_CMD_BIR3_EDIT);
	for(int i=0; i<29; i++)
	{
	  DDX_Control(pDX, arCtrlIDs[i], m_Edits[i]);
	}

	CArray<UINT, UINT> arUnitIDs;
	arUnitIDs.Add(IDC_CMD_H1_UNIT);
	arUnitIDs.Add(IDC_CMD_H2_UNIT);
	arUnitIDs.Add(IDC_CMD_HOL1_UNIT);
	arUnitIDs.Add(IDC_CMD_HOL2_UNIT);
	arUnitIDs.Add(IDC_CMD_HOL3_UNIT);
	arUnitIDs.Add(IDC_CMD_BOL1_UNIT);
	arUnitIDs.Add(IDC_CMD_BOL2_UNIT);
	arUnitIDs.Add(IDC_CMD_BOL3_UNIT);
	arUnitIDs.Add(IDC_CMD_HOL11_UNIT);
	arUnitIDs.Add(IDC_CMD_BOL11_UNIT);
	arUnitIDs.Add(IDC_CMD_HIL1_UNIT);
	arUnitIDs.Add(IDC_CMD_HIL2_UNIT);
	arUnitIDs.Add(IDC_CMD_BIL1_UNIT);
	arUnitIDs.Add(IDC_CMD_BIL2_UNIT);
	arUnitIDs.Add(IDC_CMD_BIL3_UNIT);
	arUnitIDs.Add(IDC_CMD_BIL4_UNIT);
	arUnitIDs.Add(IDC_CMD_HOR1_UNIT);
	arUnitIDs.Add(IDC_CMD_HOR2_UNIT);
	arUnitIDs.Add(IDC_CMD_HOR3_UNIT);
	arUnitIDs.Add(IDC_CMD_BOR1_UNIT);
	arUnitIDs.Add(IDC_CMD_BOR2_UNIT);
	arUnitIDs.Add(IDC_CMD_BOR3_UNIT);
	arUnitIDs.Add(IDC_CMD_HOR11_UNIT);
	arUnitIDs.Add(IDC_CMD_BOR11_UNIT);
	arUnitIDs.Add(IDC_CMD_HIR1_UNIT);
	arUnitIDs.Add(IDC_CMD_HIR2_UNIT);
	arUnitIDs.Add(IDC_CMD_BIR1_UNIT);
	arUnitIDs.Add(IDC_CMD_BIR2_UNIT);
	arUnitIDs.Add(IDC_CMD_BIR3_UNIT);
	for(int i=0; i<29; i++)
	{
	  DDX_Control(pDX, arUnitIDs[i], m_Units[i]);
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
	UINT aCtrlJoinChek[] = { IDC_CMD_JL1_CHECK, IDC_CMD_JR1_CHECK,};
	for (int i = 0; i < 2; i++)
	{
	  DDX_Control(pDX, aCtrlJoinChek[i], m_jChk[i]);
	}
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCPlat, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSCPlat)
	ON_BN_CLICKED(IDC_CMD_SECTYPE_RD1, OnCmdSectype)
	ON_CBN_SELCHANGE(IDC_CMD_HALF_CB, OnSelchangeCmdCombo)
	ON_BN_CLICKED(IDC_CMD_JL1_CHECK, OnCmdJl1Check)
	ON_BN_CLICKED(IDC_CMD_JR1_CHECK, OnCmdJr1Check)
	ON_BN_CLICKED(IDC_SYMMETRY_CHECK, OnSymmetryCheck)
	ON_EN_CHANGE(IDC_CMD_BIL1_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BIR1_EDIT, OnChangeCmdEdit)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK, OnCmdShearCheck)
	ON_CBN_SELCHANGE(IDC_CMD_SECT_NAME_COMBO, OnSelchangeCmdSectNameCombo)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_CHECK, OnCmdShear1Check)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_CHECK, OnCmdShear3Check)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK, OnCmdWebICheck)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK2, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_SECTYPE_RD2, OnCmdSectype)
	ON_BN_CLICKED(IDC_CMD_SECTYPE_RD3, OnCmdSectype)
	ON_CBN_SELCHANGE(IDC_CMD_1CELL_CB, OnSelchangeCmdCombo)
	ON_EN_CHANGE(IDC_CMD_BIL2_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BIL3_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BIL4_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BIR2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BIR3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BOL1_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BOL11_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BOL2_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BOL3_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_BOR1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BOR11_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BOR2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_BOR3_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_H1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_H2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HIL1_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HIL2_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HIR1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HIR2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HOL1_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HOL11_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HOL2_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HOL3_EDIT, OnChangeCmdLeftEdit)
	ON_EN_CHANGE(IDC_CMD_HOR1_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HOR11_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HOR2_EDIT, OnChangeCmdEdit)
	ON_EN_CHANGE(IDC_CMD_HOR3_EDIT, OnChangeCmdEdit)
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
// CCMSectItemPSCPlat message handlers


/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function 
//
BOOL CCMSectItemPSCPlat::GetDlgData(void* pData)
{
	UpdateData(TRUE);  

	// Window가 죽기전에 불린다.
	T_SECT_D * pSect = (T_SECT_D*) pData;  
	pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_BOX;

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
	pSect->SectBefore.nCellType = m_nSecType;

	if(m_nSecType == 0)   // HALF
	{
		CString strText = _T("");
		int nIndex = m_cboHalf.GetCurSel();
		if(nIndex>=0) m_cboHalf.GetLBText(nIndex, strText);
		
		if(strText == _LS(IDS_CMD_SECT_PSC_Half_Type_Left))     pSect->SectBefore.nCellShape = 0;
		else if(strText == _LS(IDS_CMD_SECT_PSC_Half_Type_Rght))pSect->SectBefore.nCellShape = 1;
		else ASSERT(0);
	}
	else if(m_nSecType == 1)  // 1CELL
	{
		CString strText = _T("");
		int nIndex = m_cbo1Cell.GetCurSel();
		if(nIndex>=0) m_cbo1Cell.GetLBText(nIndex, strText);
	
		if(strText == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly))     pSect->SectBefore.nCellShape = 2;
		else if(strText == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ))pSect->SectBefore.nCellShape = 1;
		else ASSERT(0);
	}
	else if(m_nSecType == 2)  // 2CELL
	{
		// Nothing
	}
	else ASSERT(0);
	
	pSect->SectBefore.nJoint = 0;
	if(!pSect->SectBefore.bSymmetric)  // 대칭이 아닐때 
	{
		// Joint
		if(m_bJL1) pSect->SectBefore.nJoint |= 0x00000001 ;
		if(m_bJR1) pSect->SectBefore.nJoint |= 0x00000002 ;

		for(int i=0; i<29; i++) 
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();
	}
	else  // 대칭일때
	{
		// Joint
		if(m_bJL1) pSect->SectBefore.nJoint |= 0x00000001 ;
		if(m_bJL1) pSect->SectBefore.nJoint |= 0x00000002 ;

		for(int i=0; i<16; i++) 
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();

		for(int i=16; i<29; i++)
			pSect->SectBefore.SectI.Size[i] =  m_Edits[i-14].GetEditValue();
	}

	return TRUE;
}


void  CCMSectItemPSCPlat::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D *)pData);
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

void CCMSectItemPSCPlat::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
}

void CCMSectItemPSCPlat::SetAutotShearCheckPos(double dShearPos[3])
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

void CCMSectItemPSCPlat::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_chkWebShear[i].GetCheck() == 1) 
			m_editWebShear[i].SetEditUnit(dShear[i]);
	}
	if (m_chkWebI.GetCheck() == 1) m_editWebI.SetEditUnit(dTorsion);
}

void CCMSectItemPSCPlat::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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
void CCMSectItemPSCPlat::InitCtrlsData()
{
	m_bJL1 = FALSE;
	m_bJR1 = FALSE;
	m_nSecType = 1;
	m_nWarpingCheck= 0;
}

void CCMSectItemPSCPlat::InitCellTypeCombo()
{
	CStringArray strHalfType, str1CellType; 

	strHalfType.Add(_LS(IDS_CMD_SECT_PSC_Half_Type_Left));
	strHalfType.Add(_LS(IDS_CMD_SECT_PSC_Half_Type_Rght));
	str1CellType.Add(_LS(IDS_CMD_SECT_PSC_Cell_Type_Circ));
	str1CellType.Add(_LS(IDS_CMD_SECT_PSC_Cell_Type_Poly));

	m_cboHalf.ResetContent();
	for(int i=0; i<strHalfType.GetSize(); i++)
	{
		m_cboHalf.AddString(strHalfType[i]);
	}
	m_cboHalf.SetCurSel(0);

	m_cbo1Cell.ResetContent();
	for(int i=0; i<str1CellType.GetSize(); i++)
	{
		m_cbo1Cell.AddString(str1CellType[i]);
	}
	m_cbo1Cell.SetCurSel(0);
}

void CCMSectItemPSCPlat::UpdateCurDatas()
{
	InitCtrlsData();

	// symmetric
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
	m_nSecType = m_SectData.SectBefore.nCellType;

	if(m_nSecType == 0)   // HALF
	{
		CString strText = _T("");
		if(m_SectData.SectBefore.nCellShape == 0)     strText = _LS(IDS_CMD_SECT_PSC_Half_Type_Left);
		else if(m_SectData.SectBefore.nCellShape == 1)strText = _LS(IDS_CMD_SECT_PSC_Half_Type_Rght);
		else ASSERT(0);

		m_cboHalf.SelectString(-1, strText);
	}
	else if(m_nSecType == 1)  // 1CELL
	{
		CString strText = _T("");
		if(m_SectData.SectBefore.nCellShape == 2)     strText = _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly);
		else if(m_SectData.SectBefore.nCellShape == 1) strText = _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ);
		else ASSERT(0);

		m_cbo1Cell.SelectString(-1, strText);
	}
	else if(m_nSecType == 2)  // 2CELL
	{
		// Nothing
	}
	else ASSERT(0);  

	// Joint
	if(m_SectData.SectBefore.nJoint & 0x00000001) m_bJL1 = TRUE ;
	if(m_SectData.SectBefore.nJoint & 0x00000002) m_bJR1 = TRUE ;

	// size
	for(int i = 0; i < 29 ; i++)
		m_Edits[i].SetEditUnit(m_SectData.SectBefore.SectI.Size[i]);
 
	UpdateData(FALSE);
}

void CCMSectItemPSCPlat::InitSectNameCombo()
{
	m_cobxSectName.ResetContent();
	
	CStringArray aDBNameList;
	int nNum = m_pSectDB->GetDBNameList(aDBNameList);
	for (int i=0; i<nNum; i++)
		m_cobxSectName.AddString(aDBNameList[i]);
	m_cobxSectName.SetCurSel(0);
}

BOOL CCMSectItemPSCPlat::OnInitDialog() 
{
	// 일부 데이터 초기화 
	if(!m_bModify)
	{
		m_SectData.SectBefore.nCellType  = 1;
		m_SectData.SectBefore.nCellShape = 2;
	}
//  else m_bModify = FALSE;

	CCMSectItemPSCBase::OnInitDialog();
	
	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();

	for( int i = 0 ;  i < 29 ; i++)
	{
		m_Units[i].SetUnitType(D_UNITSYS_BASE_LENGTH);    
		m_Edits[i].SetEditUnit(D_UNITSYS_BASE_LENGTH);    
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

	InitSectNameCombo();
	InitCellTypeCombo();
	UpdateCurDatas();

	// Update Viewer
	UpdateCtrlsByType();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//

BOOL CCMSectItemPSCPlat::DestroyWindow() 
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

void CCMSectItemPSCPlat::UpdateCtrlsByType()
{
	UpdateData(TRUE);

	if(m_nSecType == 0) // HALF
	{
		if(m_bSymmetry) // HALF는 대칭이 없다.
		{
			m_bSymmetry = FALSE;
		}

		CString strText = _T("");
		int nIndex = m_cboHalf.GetCurSel();
		if(nIndex >= 0) m_cboHalf.GetLBText(nIndex, strText);
		
		GetDlgItem(IDC_SYMMETRY_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_HALF_CB)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_1CELL_CB)->EnableWindow(FALSE);

		CDlgUtil::CtrlEnableDisable(this, LeftOptionGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, RghtOptionGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, CellOptionGroup, FALSE);
		
		if(strText == _LS(IDS_CMD_SECT_PSC_Half_Type_Left))  // Left
		{
			if(m_bJR1)  m_bJR1 = FALSE;      
			GetDlgItem(IDC_CMD_JL1_CHECK)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(FALSE);

			CDlgUtil::CtrlEnableDisable(this, LeftDefaltGroup, TRUE);
			CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, FALSE);
			ChangeBitmap(__SECT_PSC_PLAT_HALF_LEFT__);
		}
		else if(strText == _LS(IDS_CMD_SECT_PSC_Half_Type_Rght)) // Right
		{
			if(m_bJL1)  m_bJL1 = FALSE;
			GetDlgItem(IDC_CMD_JL1_CHECK)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(TRUE);

			CDlgUtil::CtrlEnableDisable(this, LeftDefaltGroup, FALSE);
			CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, TRUE);
			ChangeBitmap(__SECT_PSC_PLAT_HALF_RGHT__);
		}
		else ASSERT(0);
	}
	else if(m_nSecType == 1) // 1CELL
	{
		CString strText = _T("");
		int nIndex = m_cbo1Cell.GetCurSel();
		if(nIndex >= 0) m_cbo1Cell.GetLBText(nIndex, strText);

		GetDlgItem(IDC_SYMMETRY_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_HALF_CB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_1CELL_CB)->EnableWindow(TRUE);

		GetDlgItem(IDC_CMD_JL1_CHECK)->EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this, LeftDefaltGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, CellOptionGroup, FALSE);

		if(strText == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly))  // Plygon
		{
			CDlgUtil::CtrlEnableDisable(this, LeftOptionGroup, TRUE);

			if(m_bSymmetry)
			{      
				if(m_bJR1)  m_bJR1 = FALSE;
				GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(FALSE);
				CDlgUtil::CtrlEnableDisable(this, RghtOptionGroup, FALSE);
				CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, FALSE);
			}
			else 
			{
				GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(TRUE);
				CDlgUtil::CtrlEnableDisable(this, RghtOptionGroup, TRUE);
				CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, TRUE);
			}
			ChangeBitmap(__SECT_PSC_PLAT_1CEL_POLY__);
		}
		else if(strText == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ))  // Circle
		{
			CDlgUtil::CtrlEnableDisable(this, LeftOptionGroup, FALSE);
			CDlgUtil::CtrlEnableDisable(this, RghtOptionGroup, FALSE);

			if(m_bSymmetry)
			{
				if(m_bJR1)  m_bJR1 = FALSE;
				GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(FALSE);
				CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, FALSE);
			}
			else 
			{
				GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(TRUE);
				CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, TRUE);
			}
			ChangeBitmap(__SECT_PSC_PLAT_1CEL_CIRC__);
		}
		else ASSERT(0);
	}
	else if(m_nSecType == 2) // 2CELL
	{
		GetDlgItem(IDC_SYMMETRY_CHECK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_HALF_CB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_1CELL_CB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_JL1_CHECK)->EnableWindow(TRUE);
		CDlgUtil::CtrlEnableDisable(this, LeftDefaltGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, CellOptionGroup, TRUE);
		CDlgUtil::CtrlEnableDisable(this, LeftOptionGroup, FALSE);
		CDlgUtil::CtrlEnableDisable(this, RghtOptionGroup, FALSE);

		if(m_bSymmetry)
		{
			if(m_bJR1)  m_bJR1 = FALSE;
			GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(FALSE);
			CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, FALSE);
		}
		else
		{
			GetDlgItem(IDC_CMD_JR1_CHECK)->EnableWindow(TRUE);
			CDlgUtil::CtrlEnableDisable(this, RghtDefaltGroup, TRUE);
		}
			
		ChangeBitmap(__SECT_PSC_PLAT_2CEL__);
	}
	else ASSERT(0);
	
	UpdateData(FALSE);

	UpdateCtrlsByJoint();
}

// Symmetry Check
void CCMSectItemPSCPlat::OnSymmetryCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByType();
	SyncEditData(m_bSymmetry);
	SyncCheckData(m_bSymmetry);
}

// Section Type - Half, 1Cell, 2Cell
void CCMSectItemPSCPlat::OnCmdSectype() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByType();
	
}

// Section Type ComboBox - HalfCombo, 1CellCombo
void CCMSectItemPSCPlat::OnSelchangeCmdCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByType();
	
}

void CCMSectItemPSCPlat::UpdateCtrlsByJoint()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, JL1Group, m_bJL1);
	CDlgUtil::CtrlEnableDisable(this, JR1Group, m_bJR1&&!m_bSymmetry);	

	RedrawSection();
}

// Joint On/Off 
void CCMSectItemPSCPlat::OnCmdJl1Check() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByJoint();
	SyncCheckData(m_bSymmetry);
}

void CCMSectItemPSCPlat::OnCmdJr1Check() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrlsByJoint();
}

void CCMSectItemPSCPlat::SyncCheckData(BOOL bSync)
{
	if(bSync)
	{
		if(m_jChk[0].GetCheck()!=m_jChk[1].GetCheck())
		{
			m_jChk[1].SetCheck(m_jChk[0].GetCheck());
		}
	}
}


void CCMSectItemPSCPlat::SyncEditData(BOOL bSync)
{
	if(bSync)
	{
		for(int i = 2; i < 16 ; i++)  
		{
			if(m_Edits[i].GetEditValue()!=m_Edits[i+14].GetEditValue())
			{
				m_Edits[i+14].SetEditUnit(m_Edits[i].GetEditValue());
			}
		}
	}
}

void CCMSectItemPSCPlat::OnChangeCmdLeftEdit()
{
	SyncEditData(m_bSymmetry);
	RedrawSection();
}

void CCMSectItemPSCPlat::OnChangeCmdEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	RedrawSection();	
}

void CCMSectItemPSCPlat::OnCmdSectCentroidBtn() 
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

void CCMSectItemPSCPlat::OnCmdChangeOffsetBtn() 
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

void CCMSectItemPSCPlat::OnChangeCmdShearEdit() 
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

void CCMSectItemPSCPlat::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	ChangeShearCheck(m_SectData.SectBefore.bShearCheck);
	RedrawSection();
}

void CCMSectItemPSCPlat::ChangeShearCheck(int nCheck)
{
	m_chkShear1.EnableWindow(nCheck == 1);
	m_chkShear3.EnableWindow(nCheck == 1);
	//m_editShear1.EnableWindow(nCheck == 1);
	//m_editShear2.EnableWindow(nCheck == 1);
	//m_editShear3.EnableWindow(nCheck == 1);
	ChangeAutoShearCheck1();
	ChangeAutoShearCheck3();
}

void CCMSectItemPSCPlat::OnSelchangeCmdSectNameCombo() 
{
	// TODO: Add your control notification handler code here
	CString csDBName;
	int nIndex = m_cobxSectName.GetCurSel();
	if (nIndex == CB_ERR) return;
	m_cobxSectName.GetLBText(nIndex, csDBName);

	ASSERT(m_pSectDB);

	int nOffset;
	CArray<double, double> aSize;
	if (!m_pSectDB->GetDBData(csDBName, nOffset, aSize)) return;

	m_bSymmetry = TRUE;   // symmetry
	m_nSecType = 1;       // 1cell
	m_cbo1Cell.SelectString(-1, _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly));
	
	// Joint
	m_bJL1 = (nOffset & 0x00000001);
	m_bJR1 = (nOffset & 0x00000002);

	// size
	for(int i = 0; i < 29 ; i++)
		m_Edits[i].SetEditUnit(aSize[i]);

	UpdateData(FALSE);

	UpdateCtrlsByType();
	RedrawSection();		
}

void CCMSectItemPSCPlat::OnCmdShear1Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck1();
	RedrawSection();
}

void CCMSectItemPSCPlat::ChangeAutoShearCheck1()
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

void CCMSectItemPSCPlat::OnCmdShear3Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck3();
	RedrawSection();
}

void CCMSectItemPSCPlat::ChangeAutoShearCheck3()
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

void CCMSectItemPSCPlat::OnCmdWebICheck() 
{
	// TODO: Add your control notification handler code here]
	m_SectData.SectBefore.bAutoWebThk[0] = m_chkWebI.GetCheck();
	m_editWebI.EnableWindow(!m_SectData.SectBefore.bAutoWebThk[0]);
}

void CCMSectItemPSCPlat::OnCmdWebICheck2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = m_chkWebShear[i].GetCheck();
		m_editWebShear[i].EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][i]);
	}
}

void CCMSectItemPSCPlat::SaveDataForDrawSection()
{
	UpdateData(TRUE);

	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_BOX;
		
	// symmetric
	m_SectData.SectBefore.bSymmetric = m_bSymmetry;

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
	
	// Section Type
	m_SectData.SectBefore.nCellType = m_nSecType;

	if(m_nSecType == 0)   // HALF
	{
		CString strText = _T("");
		int nIndex = m_cboHalf.GetCurSel();
		if(nIndex>=0) m_cboHalf.GetLBText(nIndex, strText);
		
		if(strText == _LS(IDS_CMD_SECT_PSC_Half_Type_Left))     m_SectData.SectBefore.nCellShape = 0;
		else if(strText == _LS(IDS_CMD_SECT_PSC_Half_Type_Rght))m_SectData.SectBefore.nCellShape = 1;
		else ASSERT(0);
	}
	else if(m_nSecType == 1)  // 1CELL
	{
		CString strText = _T("");
		int nIndex = m_cbo1Cell.GetCurSel();
		if(nIndex>=0) m_cbo1Cell.GetLBText(nIndex, strText);
	
		if(strText == _LS(IDS_CMD_SECT_PSC_Cell_Type_Poly))     m_SectData.SectBefore.nCellShape = 2;
		else if(strText == _LS(IDS_CMD_SECT_PSC_Cell_Type_Circ))m_SectData.SectBefore.nCellShape = 1;
		else ASSERT(0);
	}
	else if(m_nSecType == 2)  // 2CELL
	{
	}
	else ASSERT(0);
	
	m_SectData.SectBefore.nJoint = 0;
	if (!m_SectData.SectBefore.bSymmetric)  // 대칭이 아닐때 
	{
		// Joint
		if(m_bJL1) m_SectData.SectBefore.nJoint |= 0x00000001 ;
		if(m_bJR1) m_SectData.SectBefore.nJoint |= 0x00000002 ;

		for(int i=0; i<29; i++) 
			m_SectData.SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();
	}
	else  // 대칭일때
	{
		// Joint
		if(m_bJL1) m_SectData.SectBefore.nJoint |= 0x00000001 ;
		if(m_bJL1) m_SectData.SectBefore.nJoint |= 0x00000002 ;

		for(int i=0; i<16; i++) 
			m_SectData.SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();

		for(int i=16; i<29; i++)
			m_SectData.SectBefore.SectI.Size[i] =  m_Edits[i-14].GetEditValue();
	}
}

///////////////////////////////////////////////////////////////////////////////
// 
// Update Viewer
//

BOOL CCMSectItemPSCPlat::ChangeBitmap(int nIndex)
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(nIndex);
}

void CCMSectItemPSCPlat::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSCPlat::RedrawSection()
{
	ASSERT(m_pParent);

	SaveDataForDrawSection();
	m_pParent->RedrawSection();
}



void CCMSectItemPSCPlat::OnCmdSectTableinputBtn() 
{
	CCMSectItemPSCPlatGridDlg m_GridDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_PLAT Cell;
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

void CCMSectItemPSCPlat::OnCmdSectWapingCheckBtn() 
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

void CCMSectItemPSCPlat::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSCPlat::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}
