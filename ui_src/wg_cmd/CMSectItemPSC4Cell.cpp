// CMSectItemPSC4Cell.cpp : implementation file
//
// 2003. 08. 29    21:00    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC4Cell.h"
#include "CMSectItemPSC4CellGridDlg.h"
#include "CMSectItemPSC_WarpingCheckDlg.h"

#include "CMSectItemPSC.h"
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
// CCMSectItemPSC4Cell dialog

CCMSectItemPSC4Cell::CCMSectItemPSC4Cell(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSC4Cell::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC4Cell)
	m_nType = 0;
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	m_aCtrlType.Add(IDC_CMD_SECT_PSC_TYPE1);
	m_aCtrlType.Add(IDC_CMD_SECT_PSC_TYPE2);
	m_aCtrlType.Add(IDC_CMD_SECT_PSC_TYPE3);

	m_bModify = FALSE;
	m_SectData.Initialize();

	m_nWarpingCheck    = 0;
}

CCMSectItemPSC4Cell::~CCMSectItemPSC4Cell()
{
}

void CCMSectItemPSC4Cell::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC4Cell)
	//DDX_Control(pDX, IDC_CMD_WEB_I_UNIT2, m_unitWebI2);
	//DDX_Control(pDX, IDC_CMD_WEB_I_EDIT2, m_editWebI2);
	//DDX_Control(pDX, IDC_CMD_WEB_I_CHECK2, m_chkWebI2);
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
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT,  m_txtOffset);
	DDX_Control(pDX, IDC_CMD_SECT_PSC_NUM, m_editGirderNum);
	DDX_Radio(pDX, IDC_CMD_SECT_PSC_TYPE1, m_nType);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	//}}AFX_DATA_MAP

	CArray<UINT , UINT > UnitIDs;
	UnitIDs.Add(IDC_CMD_SECT_PSC_H1_UNIT);   
	UnitIDs.Add(IDC_CMD_SECT_PSC_H2_UNIT);  
	UnitIDs.Add(IDC_CMD_SECT_PSC_H3_UNIT);  
	UnitIDs.Add(IDC_CMD_SECT_PSC_H4_UNIT);  
	UnitIDs.Add(IDC_CMD_SECT_PSC_H5_UNIT); 
	UnitIDs.Add(IDC_CMD_SECT_PSC_B1_UNIT); 
	UnitIDs.Add(IDC_CMD_SECT_PSC_B2_UNIT); 
	UnitIDs.Add(IDC_CMD_SECT_PSC_B3_UNIT); 
	UnitIDs.Add(IDC_CMD_SECT_PSC_B4_UNIT);  
	UnitIDs.Add(IDC_CMD_SECT_PSC_B5_UNIT);  
	UnitIDs.Add(IDC_CMD_SECT_PSC_B6_UNIT);  
	UnitIDs.Add(IDC_CMD_SECT_PSC_B7_UNIT);  
	UnitIDs.Add(IDC_CMD_SECT_PSC_B8_UNIT); 

	for(int i=0; i<13; i++)
	{
	  DDX_Control(pDX, UnitIDs[i], m_Units[i]);
	}

	CArray<UINT , UINT > EditIDs;
	EditIDs.Add(IDC_CMD_SECT_PSC_H1_EDIT);   
	EditIDs.Add(IDC_CMD_SECT_PSC_H2_EDIT);  
	EditIDs.Add(IDC_CMD_SECT_PSC_H3_EDIT);  
	EditIDs.Add(IDC_CMD_SECT_PSC_H4_EDIT);  
	EditIDs.Add(IDC_CMD_SECT_PSC_H5_EDIT); 
	EditIDs.Add(IDC_CMD_SECT_PSC_B1_EDIT); 
	EditIDs.Add(IDC_CMD_SECT_PSC_B2_EDIT); 
	EditIDs.Add(IDC_CMD_SECT_PSC_B3_EDIT); 
	EditIDs.Add(IDC_CMD_SECT_PSC_B4_EDIT);  
	EditIDs.Add(IDC_CMD_SECT_PSC_B5_EDIT);  
	EditIDs.Add(IDC_CMD_SECT_PSC_B6_EDIT);  
	EditIDs.Add(IDC_CMD_SECT_PSC_B7_EDIT);  
	EditIDs.Add(IDC_CMD_SECT_PSC_B8_EDIT); 

	for(int i = 0; i < 13; i++)
	{
	  DDX_Control(pDX, EditIDs[i], m_Edits[i]);
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

BEGIN_MESSAGE_MAP(CCMSectItemPSC4Cell, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSC4Cell)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_H1_EDIT, OnChangeCmdSize)
	ON_BN_CLICKED(IDC_CMD_SECT_PSC_TYPE1, OnCmdSectPscType)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK, OnCmdShearCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_CHECK, OnCmdShear1Check)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_CHECK, OnCmdShear3Check)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK, OnCmdWebICheck)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK2, OnCmdWebICheck2)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_H2_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_H3_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_H4_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_H5_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B1_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B2_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B3_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B4_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B5_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B6_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B7_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_B8_EDIT, OnChangeCmdSize)
	ON_EN_CHANGE(IDC_CMD_SECT_PSC_NUM, OnChangeCmdSize)
	ON_BN_CLICKED(IDC_CMD_SECT_PSC_TYPE2, OnCmdSectPscType)
	ON_BN_CLICKED(IDC_CMD_SECT_PSC_TYPE3, OnCmdSectPscType)
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
// CCMSectItemPSC4Cell message handlers


////////////////////////////////////////////////////////////////////////////
//
// External Data Coontrol Functions
//
BOOL CCMSectItemPSC4Cell::GetDlgData(void* pData)
{
	// Window가 죽기전에 불린다.
	T_SECT_D * pSect = (T_SECT_D*) pData;  
	pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_4CELL;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, pSect->SectBefore.nCellType);

	// Girder Number -> Cell Number로 바뀜. DB 저장은 원래대로.
	CString csGirderNum;
	m_editGirderNum.GetWindowText(csGirderNum);
	pSect->SectBefore.nCellShape = _ttoi(csGirderNum) - 1;

	for(int i = 0; i < 13 ; i++)  
		pSect->SectBefore.SectI.Size[i] =  m_Edits[i].GetEditValue();

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

	return TRUE;
}

void  CCMSectItemPSC4Cell::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D *)pData);
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

void CCMSectItemPSC4Cell::DisplayOffsetPoint()
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

void CCMSectItemPSC4Cell::SetAutotShearCheckPos(double dShearPos[3])
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

void CCMSectItemPSC4Cell::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_chkWebShear[i].GetCheck() == 1) 
			m_editWebShear[i].SetEditUnit(dShear[i]);
	}
	if (m_chkWebI.GetCheck() == 1) m_editWebI.SetEditUnit(dTorsion);
}

void CCMSectItemPSC4Cell::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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
// Initialize
//

void CCMSectItemPSC4Cell::UpdateCurDatas()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlType, m_SectData.SectBefore.nCellType);

	// size
	for (int i = 0; i < 13 ; i++)
		m_Edits[i].SetEditUnit(m_SectData.SectBefore.SectI.Size[i]);

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

	// girder number
	CString csGirderNum;
	csGirderNum.Format(_T("%d"), m_SectData.SectBefore.nCellShape+1);
	m_editGirderNum.SetWindowText(csGirderNum);
	EnableDisableControls();

	UpdateData(FALSE);
}

BOOL CCMSectItemPSC4Cell::OnInitDialog() 
{
	CCMSectItemPSCBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();

	for( int i = 0 ;  i < 13 ; i++)
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

	UpdateCurDatas();	
	EnableDisableControls();

	RedrawSection();
	ChangeBitmap(__SECT_PSC_NML_4_CELL__);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

////////////////////////////////////////////////////////////////////////////
//
// OnOK
//
BOOL CCMSectItemPSC4Cell::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_SectData.Initialize();
	if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
	return CCMSectItemPSCBase::DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////
//
// Enable/Disable
//
void CCMSectItemPSC4Cell::OnChangeCmdSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	RedrawSection();
}

void CCMSectItemPSC4Cell::SaveDataForDrawSection()
{
	UpdateData(TRUE);

	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_4CELL;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, m_SectData.SectBefore.nCellType);

	CString csGirderNum;
	m_editGirderNum.GetWindowText(csGirderNum);
	m_SectData.SectBefore.nCellShape = _ttoi(csGirderNum) - 1;

	for(int i = 0; i < 13 ; i++)  
		m_SectData.SectBefore.SectI.Size[i] = m_Edits[i].GetEditValue();

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
}

void CCMSectItemPSC4Cell::EnableDisableControls()
{
	int nType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlType, nType);

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_CMD_SECT_PSC_B7_EDIT); ASSERT(pWnd);
	pWnd->EnableWindow(nType == 1 || nType == 2);
	pWnd = GetDlgItem(IDC_CMD_SECT_PSC_B8_EDIT); ASSERT(pWnd);
	pWnd->EnableWindow(nType == 2);
}

void CCMSectItemPSC4Cell::OnCmdSectPscType() 
{
	// TODO: Add your control notification handler code here
	EnableDisableControls();
 	RedrawSection();
}

void CCMSectItemPSC4Cell::OnCmdSectCentroidBtn() 
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

void CCMSectItemPSC4Cell::OnCmdChangeOffsetBtn() 
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

void CCMSectItemPSC4Cell::OnChangeCmdShearEdit() 
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

void CCMSectItemPSC4Cell::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	ChangeShearCheck(m_SectData.SectBefore.bShearCheck);
	RedrawSection();
}

void CCMSectItemPSC4Cell::ChangeShearCheck(int nCheck)
{
	m_chkShear1.EnableWindow(nCheck);
	m_chkShear3.EnableWindow(nCheck);
	ChangeAutoShearCheck1();
	ChangeAutoShearCheck3();
}

void CCMSectItemPSC4Cell::OnCmdShear1Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck1();
	RedrawSection();
}

void CCMSectItemPSC4Cell::ChangeAutoShearCheck1()
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

void CCMSectItemPSC4Cell::OnCmdShear3Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck3();
	RedrawSection();
}

void CCMSectItemPSC4Cell::ChangeAutoShearCheck3()
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

void CCMSectItemPSC4Cell::OnCmdWebICheck() 
{
	// TODO: Add your control notification handler code here]
	m_SectData.SectBefore.bAutoWebThk[0] = m_chkWebI.GetCheck();
	m_editWebI.EnableWindow(!m_SectData.SectBefore.bAutoWebThk[0]);
}

void CCMSectItemPSC4Cell::OnCmdWebICheck2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = (m_chkWebShear[i].GetCheck() == 1);
		m_editWebShear[i].EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][i]);
	}
}


////////////////////////////////////////////////////////////////////////////
//
// Update Viewer
//
BOOL CCMSectItemPSC4Cell::ChangeBitmap(int nIndex)
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(nIndex);
}

void CCMSectItemPSC4Cell::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSC4Cell::RedrawSection()
{
	ASSERT(m_pParent);

	SaveDataForDrawSection();
	m_pParent->RedrawSection();
}


void CCMSectItemPSC4Cell::OnCmdSectTableinputBtn() 
{
	CCMSectItemPSC4CellGridDlg m_GridDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_NCELL Cell;
	Cell.Set(*sect,TRUE,TRUE);
	m_GridDlg.SetData(sect,this);
	if(m_GridDlg.DoModal()!=IDOK)
	{
		Cell.Get(*sect,TRUE,TRUE);
		UpdateCurDatas();
		
	}
	
}

void CCMSectItemPSC4Cell::OnCmdSectWapingCheckBtn() 
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

void CCMSectItemPSC4Cell::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSC4Cell::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}
