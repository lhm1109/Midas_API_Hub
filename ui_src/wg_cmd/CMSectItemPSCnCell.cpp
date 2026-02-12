// CMSectItemPSCnCell.cpp : implementation file
//
// 2004. 02. 23    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCnCell.h"

#include "CMSectOffsetDlg.h"

#include "CMSectItemPSC.h"
#include "CMSectItemPSCnCellLeft.h"
#include "CMSectItemPSCnCellRight.h"
#include "CMSectItemPSC_WarpingCheckDlg.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"
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
// CCMSectItemPSCnCell dialog


CCMSectItemPSCnCell::CCMSectItemPSCnCell(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSCnCell::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCnCell)
	//}}AFX_DATA_INIT
	m_pLeftDlg  = NULL;
	m_pRightDlg = NULL;
	m_pParent = NULL;
	m_nWarpingCheck= 0;

	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	m_bModify = FALSE;
	//m_SectData.Initialize();

	m_aCtrlCellType.Add(IDC_CMD_CELL_TYPE_RD1);
	m_aCtrlCellType.Add(IDC_CMD_CELL_TYPE_RD2);
}


void CCMSectItemPSCnCell::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCnCell)
	DDX_Control(pDX, IDC_CMD_WEB_I_CHECK, m_chkWebI);
	DDX_Control(pDX, IDC_CMD_SHEAR3_CHECK, m_chkShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR1_CHECK, m_chkShear1);
	DDX_Control(pDX, IDC_CMD_OFFSET_TEXT, m_txtOffset);
	DDX_Control(pDX, IDC_CMD_WEB_I_UNIT, m_unitWebI);
	DDX_Control(pDX, IDC_CMD_WEB_I_EDIT, m_editWebI);
	DDX_Control(pDX, IDC_CMD_SHEAR3_UNIT, m_unitShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR3_EDIT, m_editShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR2_UNIT, m_unitShear2);
	DDX_Control(pDX, IDC_CMD_SHEAR2_EDIT, m_editShear2);
	DDX_Control(pDX, IDC_CMD_SHEAR1_UNIT, m_unitShear1);
	DDX_Control(pDX, IDC_CMD_SHEAR1_EDIT, m_editShear1);
	DDX_Control(pDX, IDC_CMD_SLAB_WIDTH_UNIT, m_unitWidth);
	DDX_Control(pDX, IDC_CMD_SLAB_WIDTH_EDIT, m_editWidth);
	DDX_Control(pDX, IDC_CMD_SHEAR_CHECK, m_chkShearCheck);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM, m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_CELL_NUM_EDIT, m_editCellNumSpin);
	DDX_Control(pDX, IDC_CMD_JO_CHECK, m_chkJO);
	DDX_Control(pDX, IDC_CMD_JI_CHECK, m_chkJI);
	DDX_Control(pDX, IDC_CMD_SYMMETRIC_CHECK, m_chkSymmetic);
	DDX_Control(pDX, IDC_CMD_SIDE_HOLE_CHECK, m_chkHole);
	DDX_Control(pDX, IDC_CMD_TAB, m_ctrlTab);
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


BEGIN_MESSAGE_MAP(CCMSectItemPSCnCell, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSCnCell)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_CELL_NUM_SPIN, OnDeltaposCmdCellNumSpin)
	ON_BN_CLICKED(IDC_CMD_CELL_TYPE_RD1, OnCmdCellTypeRadio)
	ON_BN_CLICKED(IDC_CMD_JI_CHECK, OnCmdJointCheck)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN, OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_SIDE_HOLE_CHECK, OnCmdSideHoleCheck)
	ON_BN_CLICKED(IDC_CMD_SYMMETRIC_CHECK, OnCmdSymmetricCheck)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	ON_EN_CHANGE(IDC_CMD_SLAB_WIDTH_EDIT, OnChangeCmdSlabWidthEdit)
	ON_EN_CHANGE(IDC_CMD_CELL_NUM_EDIT, OnChangeCmdCellNumEdit)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN, OnCmdChangeOffsetBtn)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT, OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK, OnCmdShearCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR1_CHECK, OnCmdShear1Check)
	ON_BN_CLICKED(IDC_CMD_SHEAR3_CHECK, OnCmdShear3Check)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK, OnCmdWebICheck)
	ON_BN_CLICKED(IDC_CMD_WEB_I_CHECK2, OnCmdWebICheck2)
	ON_BN_CLICKED(IDC_CMD_CELL_TYPE_RD2, OnCmdCellTypeRadio)
	ON_BN_CLICKED(IDC_CMD_JO_CHECK, OnCmdJointCheck)
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
// External Data Control Function
BOOL CCMSectItemPSCnCell::GetDlgData(void* pData)
{
	T_SECT_D* pSect = (T_SECT_D*)pData;
	pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_NCEL2;
	pSect->SectBefore.SectI = m_SectData.SectBefore.SectI;

	// cell number
	pSect->SectBefore.nCellShape = m_editCellNumSpin.GetEditValueInt();

	// symmetric
	pSect->SectBefore.bSymmetric = (m_chkSymmetic.GetCheck()==1) ? TRUE : FALSE;

	// cell type
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCellType, pSect->SectBefore.nCellType);

	// side hole
	if (pSect->SectBefore.nCellType == 0) pSect->SectBefore.bSmallHole = FALSE;
	else pSect->SectBefore.bSmallHole = (m_chkHole.GetCheck()==1) ? TRUE : FALSE;

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
	if(m_chkJO.GetCheck()==1) pSect->SectBefore.nJoint |= 0x0001;
	if(m_chkJI.GetCheck()==1) pSect->SectBefore.nJoint |= 0x0002;
	
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

	// slab width
	SaveSlabWidth(pSect, FALSE);

	// Size - Child Dialog
 	if(m_pLeftDlg != NULL && m_pLeftDlg->GetSafeHwnd()) m_pLeftDlg->GetDlgData(pSect); 
	if(m_pRightDlg != NULL && m_pRightDlg->GetSafeHwnd()) m_pRightDlg->GetDlgData(pSect);

	// size - symmetric
	if (pSect->SectBefore.bSymmetric)
	{
		for (int i=1; i<=29; i++)
			pSect->SectBefore.SectI.Size[i+29] = pSect->SectBefore.SectI.Size[i];
	}

	return TRUE;
}

void  CCMSectItemPSCnCell::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D *)pData);
	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_NCEL2;
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

void CCMSectItemPSCnCell::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
}

void CCMSectItemPSCnCell::SetAutotShearCheckPos(double dShearPos[3])
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

void CCMSectItemPSCnCell::SetAutoMinWebThick(double dShear[3], double dTorsion)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_chkWebShear[i].GetCheck() == 1) 
			m_editWebShear[i].SetEditUnit(dShear[i]);
	}
	if (m_chkWebI.GetCheck() == 1) m_editWebI.SetEditUnit(dTorsion);
}

void CCMSectItemPSCnCell::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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
// CCMSectItemPSCnCell message handlers

BOOL CCMSectItemPSCnCell::OnInitDialog() 
{
	CCMSectItemPSCBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();

	this->LayoutChildDlgs();

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

	m_unitWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_editCellNumSpin.SetRange(1, 20);
	m_editCellNumSpin.SetInteger(TRUE);
	SetData2Dlg();

	RedrawSection();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// void CCMSectItemPSCnCell::OnDeltaposCmdCellNumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
// {
// 	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
// 	// TODO: Add your control notification handler code here
// 	CDlgUtil::SetFloatSpinValue(this, IDC_CMD_CELL_NUM_EDIT,_T("1"),_T("20"),1,pNMHDR,pResult);
// 	
// 	CString csCellNum;
// 	m_editCellNum.GetWindowText(csCellNum);
// 	m_SectData.SectBefore.nCellShape = _ttoi(csCellNum);
// 	
// 	SaveSlabWidth(&m_SectData, TRUE);
// 
// 	ChangeBitmap();
// 	ChangeStatus();
// 	RedrawSection();
// 
// 	*pResult = 0;
// }

void CCMSectItemPSCnCell::OnChangeCmdCellNumEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	// cell number : 1 ~ 20
	int nCellNum = m_editCellNumSpin.GetEditValueInt();
	if (nCellNum > 20 || nCellNum < 1)
	{
		if (nCellNum > 20) nCellNum = 20;
		if (nCellNum < 1) nCellNum = 1;

		m_editCellNumSpin.SetValue(nCellNum);
	}
	m_SectData.SectBefore.nCellShape = nCellNum;

	SaveSlabWidth(&m_SectData, TRUE);

	ChangeBitmap();
	ChangeStatus();
	RedrawSection();
}

void CCMSectItemPSCnCell::ChangeStatus()
{
	// for redraw
	//SetDlg2Data();

	T_SECT_SECTION_D* pData = &m_SectData.SectBefore;

	int nCurSel = m_ctrlTab.GetCurSel();
	if (nCurSel == 0)
	{
		CCMSectItemPSCnCellLeft* pDlg = (CCMSectItemPSCnCellLeft*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg) pDlg->ChangeStatus(pData->bSymmetric, pData->nCellType, 
																 pData->nCellShape, pData->nJoint, pData->bSmallHole);
	}
	else if (nCurSel == 1)
	{
		CCMSectItemPSCnCellRight* pDlg = (CCMSectItemPSCnCellRight*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg) pDlg->SetData2Dlg();
	}
	else ASSERT(0);

	RedrawSection();
}

void CCMSectItemPSCnCell::OnCmdCellTypeRadio() 
{
	// TODO: Add your control notification handler code here

	// 해야할 일 
	// 1. 일단 데이터를 저장한 후
	// 2. 자기 자신 Enable/Disable
	// 3. 자식 다이얼로그 Enable/Disable
	// 4. Bitmap
	// 5. 다시 그리기 

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCellType, m_SectData.SectBefore.nCellType);

	if (m_SectData.SectBefore.nCellType == 1) 
	{
		m_chkJI.SetCheck(0);
		m_chkJI.EnableWindow(FALSE);
	}
	else m_chkJI.EnableWindow(TRUE);
	m_chkHole.ShowWindow(m_SectData.SectBefore.nCellType==1);

	SaveSlabWidth(&m_SectData, TRUE);

	// change bitmap
	ChangeBitmap();

	// change status
	ChangeStatus();
}

void CCMSectItemPSCnCell::OnCmdJointCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.nJoint = 0;
	if(m_chkJO.GetCheck()==1) m_SectData.SectBefore.nJoint |= 0x0001;
	if(m_chkJI.GetCheck()==1) m_SectData.SectBefore.nJoint |= 0x0002;
	ChangeStatus();
}

void CCMSectItemPSCnCell::OnCmdSideHoleCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bSmallHole = (m_chkHole.GetCheck() == 1) ? TRUE : FALSE;
	ChangeStatus();	
}

void CCMSectItemPSCnCell::OnCmdSymmetricCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bSymmetric = m_chkSymmetic.GetCheck();
	if (m_SectData.SectBefore.bSymmetric == 1)
	{
		T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
		for (int i=1; i<=29; i++)
		{
			if(i==26||i==27)
				continue;
			pData->SectI.Size[i+29] = pData->SectI.Size[i];
			
		}
	}
	ChangeStatus();
}

void CCMSectItemPSCnCell::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	// change bitmap
	int nCellType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCellType, nCellType);

	// change bitmap
	ChangeBitmap();

	// symmetric
	if (m_chkSymmetic.GetCheck() == 1)
	{
		T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
		for (int i=1; i<=29; i++) pData->SectI.Size[i+29] = pData->SectI.Size[i];
	}
	ChangeStatus();

	*pResult = 0;
}

void CCMSectItemPSCnCell::OnCmdSectCentroidBtn() 
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

void CCMSectItemPSCnCell::OnChangeCmdSlabWidthEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.SectI.Size[0] = m_editWidth.GetEditValue();
	RedrawSection();
}

void CCMSectItemPSCnCell::OnCmdChangeOffsetBtn() 
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

void CCMSectItemPSCnCell::OnChangeCmdShearEdit() 
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

void CCMSectItemPSCnCell::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	ChangeShearCheck(m_SectData.SectBefore.bShearCheck);
	RedrawSection();
}

void CCMSectItemPSCnCell::ChangeShearCheck(int nCheck)
{
	m_chkShear1.EnableWindow(nCheck == 1);
	m_chkShear3.EnableWindow(nCheck == 1);
	//m_editShear1.EnableWindow(nCheck == 1);
	//m_editShear2.EnableWindow(nCheck == 1);
	//m_editShear3.EnableWindow(nCheck == 1);
	ChangeAutoShearCheck1();
	ChangeAutoShearCheck3();
}

BOOL CCMSectItemPSCnCell::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_SectData.Initialize();
	if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
	return CCMSectItemPSCBase::DestroyWindow();
}

void CCMSectItemPSCnCell::OnCmdShear1Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck1();
	RedrawSection();
}

void CCMSectItemPSCnCell::ChangeAutoShearCheck1()
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

void CCMSectItemPSCnCell::OnCmdShear3Check() 
{
	// TODO: Add your control notification handler code here
	ChangeAutoShearCheck3();
	RedrawSection();
}

void CCMSectItemPSCnCell::ChangeAutoShearCheck3()
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

void CCMSectItemPSCnCell::OnCmdWebICheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bAutoWebThk[0] = m_chkWebI.GetCheck();
	m_editWebI.EnableWindow(!m_SectData.SectBefore.bAutoWebThk[0]);
}

void CCMSectItemPSCnCell::OnCmdWebICheck2() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 3; i++)
	{
		m_SectData.SectBefore.bAutoWebThkShear[0][i] = (m_chkWebShear[i].GetCheck()==1);
		m_editWebShear[i].EnableWindow(!m_SectData.SectBefore.bAutoWebThkShear[0][i]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defeind Function

void CCMSectItemPSCnCell::LayoutChildDlgs()
{
	// PlaceHolder Setting
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	// TabWinddow Create & Setting 
	m_pLeftDlg = new CCMSectItemPSCnCellLeft(this);
	m_ctrlTab.AddTab(m_pLeftDlg, _LS(IDS_CMD_PSC_NCELL_LEFT), CCMSectItemPSCnCellLeft::IDD, TRUE);

	m_pRightDlg = new CCMSectItemPSCnCellRight(this);
	m_ctrlTab.AddTab(m_pRightDlg, _LS(IDS_CMD_PSC_NCELL_RIGHT), CCMSectItemPSCnCellRight::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CCMSectItemPSCnCell::SetData2Dlg()
{
	double dSlabWidth = m_SectData.SectBefore.SectI.Size[0];

	// cell number
	CString csCellNum;
	if (m_SectData.SectBefore.nCellShape <= 0 || m_SectData.SectBefore.nCellShape > 20)  
		m_SectData.SectBefore.nCellShape = 1;
	m_editCellNumSpin.SetValue(m_SectData.SectBefore.nCellShape);

	// symmetric
	m_chkSymmetic.SetCheck((int)m_SectData.SectBefore.bSymmetric);

	// cell type
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCellType, m_SectData.SectBefore.nCellType);

	// side hole
	m_chkHole.SetCheck((int)m_SectData.SectBefore.bSmallHole);
	m_chkHole.ShowWindow(m_SectData.SectBefore.nCellType==1);

	// Offset 
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_SectData.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_SectData.SectBefore.nOffsetPoint-1];
		m_txtOffset.SetWindowText(csOffset);
	}

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

	// slab width
	m_editWidth.SetEditUnit(dSlabWidth);

	// joint
	//Check Control이 0,1만 받는 Style로 생성되어 있는데 0x0002로 비트연산을 하고 있어서
	//2가 나오는 케이스가 있음. Assert 자꾸 걸려서 고쳐놓음.  
	m_chkJO.SetCheck(m_SectData.SectBefore.nJoint & 0x0001 ? 1 : 0);
	m_chkJI.SetCheck(m_SectData.SectBefore.nJoint & 0x0002 ? 1 : 0);

	// enable/disable
	T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
	int nCurSel = m_ctrlTab.GetCurSel();
	if (nCurSel == 0)
	{
		CCMSectItemPSCnCellLeft* pDlg = (CCMSectItemPSCnCellLeft*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg) pDlg->ChangeStatus(pData->bSymmetric, pData->nCellType, 
										pData->nCellShape, pData->nJoint, pData->bSmallHole);
		pDlg->SetData2Dlg();
		

	}
	else if (nCurSel == 1)
	{
		CCMSectItemPSCnCellRight* pDlg = (CCMSectItemPSCnCellRight*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg) pDlg->ChangeStatus(pData->bSymmetric, pData->nCellType, 
										pData->nCellShape, pData->nJoint, pData->bSmallHole);
		pDlg->SetData2Dlg();
	}
	else
	{
		ASSERT(0);
	}
	
	// change bitmap
	ChangeBitmap();
}

void CCMSectItemPSCnCell::SetDlg2Data()
{
	// 여기서는 자신의 것만 저장한다.
	// Child Dialog들은 각각 알아서 저장된다.

	// cell number
	m_SectData.SectBefore.nCellShape = m_editCellNumSpin.GetEditValueInt();

	// symmetric
	m_SectData.SectBefore.bSymmetric = (m_chkSymmetic.GetCheck()==1) ? TRUE : FALSE;

	// cell type
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCellType, m_SectData.SectBefore.nCellType);

	// side hole
	if (m_SectData.SectBefore.nCellType == 0) m_SectData.SectBefore.bSmallHole = FALSE;
	else m_SectData.SectBefore.bSmallHole = (m_chkHole.GetCheck()==1) ? TRUE : FALSE;

	// offset = nothing

	// Joint
	m_SectData.SectBefore.nJoint = 0;
	if(m_chkJO.GetCheck()==1) m_SectData.SectBefore.nJoint |= 0x0001;
	if(m_chkJI.GetCheck()==1) m_SectData.SectBefore.nJoint |= 0x0002;
	
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

	// slab width
	SaveSlabWidth(&m_SectData, FALSE);

	// size - symmetric
	if (m_SectData.SectBefore.bSymmetric)
	{
		for (int i=1; i<=29; i++)
			m_SectData.SectBefore.SectI.Size[i+29] = m_SectData.SectBefore.SectI.Size[i];
	}
}


////////////////////////////////////////////////////////////////////////////
//
// Update Viewer
//

BOOL CCMSectItemPSCnCell::ChangeBitmap()
{
	int nCurSel = m_ctrlTab.GetCurSel();
	ASSERT(nCurSel == 0 || nCurSel == 1);

	UINT aBitmap[] = {
		__SECT_PSC_nCELL_POLY1_LEFT__, __SECT_PSC_nCELL_POLY1_RGHT__,
		__SECT_PSC_nCELL_POLY2_LEFT__, __SECT_PSC_nCELL_POLY2_RGHT__,
		__SECT_PSC_nCELL_CIRC_LEFT__ , __SECT_PSC_nCELL_CIRC_RGHT__,
	}; 
	int nIndex;
	switch(m_SectData.SectBefore.nCellType)
	{
	case 0: 
		if (m_SectData.SectBefore.nCellShape < 2) nIndex = nCurSel;
		else nIndex = 2 + nCurSel;
		break;
	case 1: nIndex = 4 + nCurSel;
		break;
	default: ASSERT(0);
		break;
	}

	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(aBitmap[nIndex]);
}

void CCMSectItemPSCnCell::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSCnCell::RedrawSection()
{
	SaveSlabWidth(&m_SectData, FALSE);

	ASSERT(m_pParent);
	m_pParent->RedrawSection();
}

void CCMSectItemPSCnCell::SaveSlabWidth(T_SECT_D* pSectData, BOOL bEnableCtrl)
{
	ASSERT(pSectData);

	BOOL bSlabWidth;
	if (pSectData->SectBefore.nCellType == 0)   // polygon
		bSlabWidth = (pSectData->SectBefore.nCellShape > 2);
	else                                        // circle
		bSlabWidth = (pSectData->SectBefore.nCellShape > 1);

	if (bEnableCtrl) m_editWidth.EnableWindow(bSlabWidth);
	
	if (bSlabWidth)
		pSectData->SectBefore.SectI.Size[0] = m_editWidth.GetEditValue();
	else
	{
		pSectData->SectBefore.SectI.Size[0] = pSectData->SectBefore.SectI.Size[6] +
																					pSectData->SectBefore.SectI.Size[8] +
																					pSectData->SectBefore.SectI.Size[9] +
																					pSectData->SectBefore.SectI.Size[10] +
																					pSectData->SectBefore.SectI.Size[11];
	}

}
void CCMSectItemPSCnCell::OnCmdSectTableinputBtn() 
{
	// TODO: Add your control notification handler code here
	CCMSectItemPSCnCellGridDlg m_GridDlg(this);
	T_SECT_D* sect=GetDataPointer();
	T_SECT_D_PSC_NCEL2 NCell2;
	NCell2.Set(*sect,TRUE,TRUE);
	m_GridDlg.SetData(sect,this);
	
	if(m_GridDlg.DoModal()!=IDOK)
	{
		NCell2.Get(*sect,TRUE,TRUE);
		SetData2Dlg();
	}
}
void CCMSectItemPSCnCell::ChangeTab(int index)
{
	m_ctrlTab.ShowTab(index);

	int nCellType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCellType, nCellType);

	// change bitmap
	ChangeBitmap();

	// symmetric
	if (m_chkSymmetic.GetCheck() == 1)
	{
		T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
		for (int i=1; i<=29; i++) pData->SectI.Size[i+29] = pData->SectI.Size[i];
	}
	ChangeStatus();

}
	
void CCMSectItemPSCnCell::OnCmdSectWapingCheckBtn() 
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

void CCMSectItemPSCnCell::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSCnCell::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}
