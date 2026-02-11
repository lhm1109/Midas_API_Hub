// CMSectItemPSC3Cell.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC3Cell.h"
#include "CMSectItemPSC3CellDef.h"
#include "CMSectItemPSC3CellOpt.h"
#include "CMSectItemPSC3CellGridDlg.h"
#include "CMSectItemPSC.h"
#include "CMSectItemPSC_WarpingCheckDlg.h"

#include "CMSectOffsetDlg.h"

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
// CCMSectItemPSC3Cell dialog

void CCMSectItemPSC3Cell::InitCtrlsData()
{
	m_bJI1 = FALSE;
	m_bJI2 = FALSE;
	m_bJI3 = FALSE;
	m_bJI4 = FALSE;
	m_bJI5 = FALSE;
	m_bJI6 = FALSE;
	m_bJI7 = FALSE;
	m_bJI8 = FALSE;
	m_bJI9 = FALSE;
	m_bJI10 = FALSE;
	m_bJI11 = FALSE;
	m_bJO1 = FALSE;
	m_bJO2 = FALSE;
	m_nWarpingCheck= 0;
}

CCMSectItemPSC3Cell::CCMSectItemPSC3Cell(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSC3Cell::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC3Cell)
	m_bJI1 = FALSE;
	m_bJI10 = FALSE;
	m_bJI11 = FALSE;
	m_bJI2 = FALSE;
	m_bJI3 = FALSE;
	m_bJI4 = FALSE;
	m_bJI5 = FALSE;
	m_bJI6 = FALSE;
	m_bJI7 = FALSE;
	m_bJI8 = FALSE;
	m_bJI9 = FALSE;
	m_bJO1 = FALSE;
	m_bJO2 = FALSE;
	//}}AFX_DATA_INIT

	m_pDefDlg = NULL;
	m_pOptDlg = NULL;
	m_pParent = NULL;
	m_nWarpingCheck= 0;

	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	m_bModify = FALSE;
	m_SectData.Initialize();
}

CCMSectItemPSC3Cell::~CCMSectItemPSC3Cell()
{
	//---------------------------------------------------------
	// CTabCtrl의 ChildDialog를 Delete하면 안된다.
	// CTabCtrl내부에서 자체 삭제를 하기 때문.
	/*
	if(m_pDefDlg != NULL){ delete m_pDefDlg; m_pDefDlg = NULL; }
	if(m_pOptDlg != NULL){ delete m_pOptDlg; m_pOptDlg = NULL; }
	*/
}

void CCMSectItemPSC3Cell::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC3Cell)
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
	DDX_Control(pDX, IDC_CMD_TAB, m_ctrlTab);
	DDX_Check(pDX, IDC_CMD_JI1_CHECK, m_bJI1);
	DDX_Check(pDX, IDC_CMD_JI10_CHECK, m_bJI10);
	DDX_Check(pDX, IDC_CMD_JI11_CHECK, m_bJI11);
	DDX_Check(pDX, IDC_CMD_JI2_CHECK, m_bJI2);
	DDX_Check(pDX, IDC_CMD_JI3_CHECK, m_bJI3);
	DDX_Check(pDX, IDC_CMD_JI4_CHECK, m_bJI4);
	DDX_Check(pDX, IDC_CMD_JI5_CHECK, m_bJI5);
	DDX_Check(pDX, IDC_CMD_JI6_CHECK, m_bJI6);
	DDX_Check(pDX, IDC_CMD_JI7_CHECK, m_bJI7);
	DDX_Check(pDX, IDC_CMD_JI8_CHECK, m_bJI8);
	DDX_Check(pDX, IDC_CMD_JI9_CHECK, m_bJI9);
	DDX_Check(pDX, IDC_CMD_JO1_CHECK, m_bJO1);
	DDX_Check(pDX, IDC_CMD_JO2_CHECK, m_bJO2);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	//}}AFX_DATA_MAP

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


BEGIN_MESSAGE_MAP(CCMSectItemPSC3Cell, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSC3Cell)
	ON_BN_CLICKED(IDC_CMD_JI1_CHECK, OnCmdCheckCtrl)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK, OnCmdShearCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_CHECK, OnCmdShear1Check)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_CHECK, OnCmdShear3Check)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK, OnCmdWebICheck)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK2, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_JI10_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI11_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI2_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI3_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI4_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI5_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI6_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI7_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI8_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JI9_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JO1_CHECK, OnCmdCheckCtrl)
	ON_BN_CLICKED(IDC_CMD_JO2_CHECK, OnCmdCheckCtrl)
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
// CCMSectItemPSC3Cell message handlers


/////////////////////////////////////////////////////////////////////////////
//
// External Data Control Function
//
T_SECT_D* CCMSectItemPSC3Cell::GetDataPointer()
{
	return (&m_SectData);
}

BOOL CCMSectItemPSC3Cell::GetDlgData(void* pData)
{
	UpdateData(TRUE);

	T_SECT_D * pSect = (T_SECT_D*) pData; 
	pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_3CELL;
	pSect->SectBefore.SectI = m_SectData.SectBefore.SectI;

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

	// Joint
	pSect->SectBefore.nJoint = 0;
	if(m_bJI1 ) pSect->SectBefore.nJoint |= 0x0001;
	if(m_bJI2 ) pSect->SectBefore.nJoint |= 0x0002;
	if(m_bJI3 ) pSect->SectBefore.nJoint |= 0x0004;
	if(m_bJI4 ) pSect->SectBefore.nJoint |= 0x0008;
	if(m_bJI5 ) pSect->SectBefore.nJoint |= 0x0010;
	if(m_bJI6 ) pSect->SectBefore.nJoint |= 0x0020;
	if(m_bJI7 ) pSect->SectBefore.nJoint |= 0x0040;
	if(m_bJI8 ) pSect->SectBefore.nJoint |= 0x0080;
	if(m_bJI9 ) pSect->SectBefore.nJoint |= 0x0100;
	if(m_bJI10) pSect->SectBefore.nJoint |= 0x0200;
	if(m_bJI11) pSect->SectBefore.nJoint |= 0x0400;
	if(m_bJO1 ) pSect->SectBefore.nJoint |= 0x0800;
	if(m_bJO2 ) pSect->SectBefore.nJoint |= 0x1000;

	// Size - Child Dialog
 	if(m_pDefDlg != NULL && m_pDefDlg->GetSafeHwnd()) m_pDefDlg->GetDlgData(pSect); // 구현 !!!
	if(m_pOptDlg != NULL && m_pOptDlg->GetSafeHwnd()) m_pOptDlg->GetDlgData(pSect); // 구현 !!!
 
	return TRUE;
}

void  CCMSectItemPSC3Cell::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D *)pData);
	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_3CELL;
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

void CCMSectItemPSC3Cell::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
	/*
	if (m_SectData.SectBefore.nOffsetCenter == 0) 
		OnCmdSectCentroidBtn();
	else RedrawSection();
	*/
}

void CCMSectItemPSC3Cell::SetAutotShearCheckPos(double dShearPos[3])
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

void CCMSectItemPSC3Cell::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_chkWebShear[i].GetCheck() == 1) 
			m_editWebShear[i].SetEditUnit(dShear[i]);
	}
	if (m_chkWebI.GetCheck() == 1) m_editWebI.SetEditUnit(dTorsion);
}

void CCMSectItemPSC3Cell::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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

void CCMSectItemPSC3Cell::LayoutChildDlgs(void)
{
	// PlaceHolder Setting
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	// TabWinddow Create & Setting 
	m_pDefDlg = new CCMSectItemPSC3CellDef(this);
	m_ctrlTab.AddTab(m_pDefDlg, _LS(IDS_CMD_SECT_ITEM_PSC_DEF), CCMSectItemPSC3CellDef::IDD, TRUE);

	m_pOptDlg = new CCMSectItemPSC3CellOpt(this);
	m_ctrlTab.AddTab(m_pOptDlg, _LS(IDS_CMD_SECT_ITEM_PSC_OPT), CCMSectItemPSC3CellOpt::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CCMSectItemPSC3Cell::SetData2Dlg()
{
	InitCtrlsData();

	// shear check
	m_chkShearCheck.SetCheck((int)m_SectData.SectBefore.bShearCheck);
	m_chkShear1.SetCheck((int)m_SectData.SectBefore.bAutoShearCheckPos[0][0]);
	m_chkShear3.SetCheck((int)m_SectData.SectBefore.bAutoShearCheckPos[0][2]);
	m_editShear1.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][0]);
	m_editShear2.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][1]);
	m_editShear3.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][2]);
	OnCmdShearCheck();

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

	// offset 
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_SectData.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_SectData.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

	// Joint
	if(m_SectData.SectBefore.nJoint & 0x0001) m_bJI1 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0002) m_bJI2 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0004) m_bJI3 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0008) m_bJI4 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0010) m_bJI5 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0020) m_bJI6 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0040) m_bJI7 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0080) m_bJI8 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0100) m_bJI9 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0200) m_bJI10= TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0400) m_bJI11= TRUE;
	if(m_SectData.SectBefore.nJoint & 0x0800) m_bJO1 = TRUE;
	if(m_SectData.SectBefore.nJoint & 0x1000) m_bJO2 = TRUE;

	UpdateData(FALSE);
	m_pDefDlg->SetData2Dlg();
	m_pOptDlg->SetData2Dlg();
}

void CCMSectItemPSC3Cell::SetDlg2Data()
{
	// 여기서는 자신의 것만 저장한다.
	// Child Dialog들은 각각 알아서 저장된다.
	UpdateData(TRUE);

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

	// Joint
	m_SectData.SectBefore.nJoint = 0;  // 초기화 

	if(m_bJI1 ) m_SectData.SectBefore.nJoint |= 0x0001;
	if(m_bJI2 ) m_SectData.SectBefore.nJoint |= 0x0002;
	if(m_bJI3 ) m_SectData.SectBefore.nJoint |= 0x0004;
	if(m_bJI4 ) m_SectData.SectBefore.nJoint |= 0x0008;
	if(m_bJI5 ) m_SectData.SectBefore.nJoint |= 0x0010;
	if(m_bJI6 ) m_SectData.SectBefore.nJoint |= 0x0020;
	if(m_bJI7 ) m_SectData.SectBefore.nJoint |= 0x0040;
	if(m_bJI8 ) m_SectData.SectBefore.nJoint |= 0x0080;
	if(m_bJI9 ) m_SectData.SectBefore.nJoint |= 0x0100;
	if(m_bJI10) m_SectData.SectBefore.nJoint |= 0x0200;
	if(m_bJI11) m_SectData.SectBefore.nJoint |= 0x0400;
	if(m_bJO1 ) m_SectData.SectBefore.nJoint |= 0x0800;
	if(m_bJO2 ) m_SectData.SectBefore.nJoint |= 0x1000;
	
}

void CCMSectItemPSC3Cell::OnCmdCheckCtrl() 
{
	// TODO: Add your control notification handler code here
	SetDlg2Data();

/* 그리는 문제 때문인 거 같은디... 
	m_pDefDlg->SetDlg2Data();
	m_pOptDlg->SetDlg2Data();
*/
	int nCurSel = m_ctrlTab.GetCurSel();
	UpdateCtrls();
	RedrawSection();
}

BOOL CCMSectItemPSC3Cell::OnInitDialog() 
{
	CCMSectItemPSCBase::OnInitDialog();
	
	// TODO: Add extra initialization here	
	//InitOffsetCombo();
	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();

	this -> LayoutChildDlgs();

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

	SetData2Dlg();

	// Update Viewer
	ChangeBitmap(__SECT_PSC_NML_3_CELL__);
	OnCmdCheckCtrl(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
BOOL CCMSectItemPSC3Cell::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_SectData.Initialize();	
	if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
	return CCMSectItemPSCBase::DestroyWindow();
}

void CCMSectItemPSC3Cell::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ctrlTab.GetCurSel();
	if (nCurSel < 0) return;

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// 
// Update Viewer
//

BOOL CCMSectItemPSC3Cell::ChangeBitmap(int nIndex)
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(nIndex);
}

void CCMSectItemPSC3Cell::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSC3Cell::RedrawSection()
{
	ASSERT(m_pParent);
	m_pParent->RedrawSection();
}

void CCMSectItemPSC3Cell::OnCmdSectCentroidBtn() 
{
	// TODO: Add your control notification handler code here
	SetDlg2Data();

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

void CCMSectItemPSC3Cell::OnCmdChangeOffsetBtn() 
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

void CCMSectItemPSC3Cell::OnChangeCmdShearEdit() 
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

void CCMSectItemPSC3Cell::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	m_chkShear1.EnableWindow(m_SectData.SectBefore.bShearCheck == 1);
	m_chkShear3.EnableWindow(m_SectData.SectBefore.bShearCheck == 1);
	OnCmdShear1Check();
	OnCmdShear3Check();
	//RedrawSection();
}

void CCMSectItemPSC3Cell::OnCmdShear1Check() 
{
	// TODO: Add your control notification handler code here
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
	RedrawSection();
}

void CCMSectItemPSC3Cell::OnCmdShear3Check() 
{
	// TODO: Add your control notification handler code here
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
	RedrawSection();
}

void CCMSectItemPSC3Cell::OnCmdWebICheck() 
{
	// TODO: Add your control notification handler code here]
	m_SectData.SectBefore.bAutoWebThk[0] = m_chkWebI.GetCheck();
	m_editWebI.EnableWindow(!m_SectData.SectBefore.bAutoWebThk[0]);
}

void CCMSectItemPSC3Cell::OnCmdWebICheck2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = m_chkWebShear[i].GetCheck();
		m_editWebShear[i].EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][i]);
	}
}

void CCMSectItemPSC3Cell::OnCmdSectTableinputBtn() 
{
	CCMSectItemPSC3CellGridDlg m_GridDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_3CELL Cell;
	Cell.Set(*sect,TRUE,TRUE);
	m_GridDlg.SetData(sect,this);
	
	if(m_GridDlg.DoModal()!=IDOK)
	{
		Cell.Get(*sect,TRUE,TRUE);
		SetData2Dlg();
		
	}	
	
}
void CCMSectItemPSC3Cell::OnCmdSectWapingCheckBtn() 
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

void CCMSectItemPSC3Cell::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSC3Cell::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}

void CCMSectItemPSC3Cell::ChangeTab(int index)
{
	m_ctrlTab.ShowTab(index);

}
int CCMSectItemPSC3Cell::GetSelectedTab()
{
	return m_ctrlTab.GetCurSel();
}
void CCMSectItemPSC3Cell::UpdateCtrls()
{
	int nCurSel = m_ctrlTab.GetCurSel();
	if(nCurSel == 1)
	{
		CCMSectItemPSC3CellOpt * pDlg = (CCMSectItemPSC3CellOpt*)m_ctrlTab.GetTab(nCurSel);
		if(pDlg)
			pDlg->UpdateCtrls();
	}
}


