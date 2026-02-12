// CMSectItemPSC_CMP.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC_CMP.h"

#include "CMSectItemPSC.h"
#include "CMSectItemPSC_CMPLeft.h"
#include "CMSectItemPSC_CMPRight.h"
#include "CMSectOffsetDlg.h"
#include "CMConcrSteelMatlDlg.h"
#include "CMSectItemPSCCMPGridDlg.h"
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
// CCMSectItemPSC_CMP dialog
CCMSectItemPSC_CMP::CCMSectItemPSC_CMP(CWnd* pParent /*=NULL*/)
	: CCMSectItemPSCBase(CCMSectItemPSC_CMP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC_CMP)
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CCMSectItemPSC *)pParent;

	m_pLeftDlg  = NULL;
	m_pRightDlg = NULL;
	m_bModify   = FALSE;
	m_nWarpingCheck= 0;
}

void CCMSectItemPSC_CMP::DoDataExchange(CDataExchange* pDX)
{
	CCMSectItemPSCBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC_CMP)
	DDX_Control(pDX, IDC_CMD_CMP_EFD_EDIT , m_editEFD);
	DDX_Control(pDX, IDC_CMD_CMP_LRF_EDIT , m_editLRF);
	DDX_Control(pDX, IDC_CMD_MATL_MVALUE4 , m_editMValue4);
	DDX_Control(pDX, IDC_CMD_MATL_MVALUE3 , m_editMValue3);
	DDX_Control(pDX, IDC_CMD_MATL_MVALUE2 , m_editMValue2);
	DDX_Control(pDX, IDC_CMD_MATL_MVALUE1 , m_editMValue1);
	DDX_Control(pDX, IDC_CMD_CMP_T_UNIT   , m_unitCMPt);
	DDX_Control(pDX, IDC_CMD_CMP_T_EDIT   , m_editCMPt);
	DDX_Control(pDX, IDC_CMD_CMP_H_UNIT   , m_unitCMPh);
	DDX_Control(pDX, IDC_CMD_CMP_H_EDIT   , m_editCMPh);
	DDX_Control(pDX, IDC_CMD_CMP_B_UNIT   , m_unitCMPb);
	DDX_Control(pDX, IDC_CMD_CMP_B_EDIT   , m_editCMPb);
	DDX_Control(pDX, IDC_CMD_CMP_A_UNIT   , m_unitCMPa);
	DDX_Control(pDX, IDC_CMD_CMP_A_EDIT   , m_editCMPa);
	DDX_Control(pDX, IDC_CMD_SHEAR3_UNIT  , m_unitShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR3_EDIT  , m_editShear3);
	DDX_Control(pDX, IDC_CMD_SHEAR2_UNIT  , m_unitShear2);
	DDX_Control(pDX, IDC_CMD_SHEAR2_EDIT  , m_editShear2);
	DDX_Control(pDX, IDC_CMD_SHEAR1_UNIT  , m_unitShear1);
	DDX_Control(pDX, IDC_CMD_SHEAR1_EDIT  , m_editShear1);
	DDX_Control(pDX, IDC_CMD_SHEAR_CHECK  , m_chkShearCheck);
	DDX_Control(pDX, IDC_CMD_SHEAR_DEFORM , m_chkShearDeform);
	DDX_Control(pDX, IDC_CMD_WARPING_EFFECT_CHK, m_chkWarpingEffect);
	DDX_Control(pDX, IDC_CMD_HUNCH_CHECK  , m_chkHunch);
	DDX_Control(pDX, IDC_CMD_SYMMETRIC_CHECK, m_chkSymmetic);
	DDX_Control(pDX, IDC_CMD_TAB          , m_ctrlTab);
	DDX_Radio(pDX, IDC_CMD_WARPING_EFFECT_AUTO_RDO, m_nWarpingCheck);
	DDX_Control(pDX, IDC_CMD_CMP_FRM, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSectItemPSC_CMP, CCMSectItemPSCBase)
	//{{AFX_MSG_MAP(CCMSectItemPSC_CMP)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB    , OnSelchangeCmdTab)
	ON_BN_CLICKED(IDC_CMD_CMP_LRF_BTN       , OnCmdCMPLRFBtn)
	ON_BN_CLICKED(IDC_CMD_CHANGE_OFFSET_BTN , OnCmdChangeOffsetBtn)
	ON_BN_CLICKED(IDC_CMD_SECT_CENTROID_BTN , OnCmdSectCentroidBtn)
	ON_BN_CLICKED(IDC_CMD_MATL_BTN          , OnCmdMaterialBtn)
	ON_BN_CLICKED(IDC_CMD_HUNCH_CHECK       , OnCmdHunchCheck)
	ON_BN_CLICKED(IDC_CMD_SYMMETRIC_CHECK   , OnCmdSymmetricCheck)
	ON_BN_CLICKED(IDC_CMD_SHEAR_CHECK       , OnCmdShearCheck)
	ON_EN_CHANGE(IDC_CMD_SHEAR1_EDIT        , OnChangeCmdShearEdit)
	ON_EN_CHANGE(IDC_CMD_CMP_A_EDIT         , OnChangeCmdAEdit)
	ON_EN_CHANGE(IDC_CMD_CMP_B_EDIT         , OnChangeCmdBEdit)
	ON_EN_CHANGE(IDC_CMD_CMP_H_EDIT         , OnChangeCmdHEdit)
	ON_EN_CHANGE(IDC_CMD_CMP_T_EDIT         , OnChangeCmdTEdit)
	ON_EN_CHANGE(IDC_CMD_MATL_MVALUE1       , OnChangeCmdMatlEdit)
	ON_EN_CHANGE(IDC_CMD_SHEAR2_EDIT        , OnChangeCmdShearEdit)
	ON_EN_CHANGE(IDC_CMD_SHEAR3_EDIT        , OnChangeCmdShearEdit)
	ON_BN_CLICKED(IDC_CMD_SECT_TABLEINPUT_BTN, OnCmdSectTableinputBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_BTN,       OnCmdSectWapingCheckBtn)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_AUTO_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_USER_RDO,  OnCmdSectWapingCheckUser)
	ON_BN_CLICKED(IDC_CMD_WARPING_EFFECT_CHK,       OnCmdSectWapingCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// External Data Control Function
BOOL CCMSectItemPSC_CMP::GetDlgData(void* pData)
{
	T_SECT_D* pSect = (T_SECT_D*)pData;
	pSect->SectBefore.Shape  = D_SECT_SHAPE_PSC_CMPWEB;
	pSect->SectBefore.SectI = m_SectData.SectBefore.SectI;

	// symmetric
	pSect->SectBefore.bSymmetric = (m_chkSymmetic.GetCheck()==1) ? TRUE : FALSE;

	// hunch
	pSect->SectBefore.bSmallHole = (m_chkHunch.GetCheck()==1) ? TRUE : FALSE;

	// shear check
	pSect->SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	if (pSect->SectBefore.bShearCheck)
	{
		pSect->SectBefore.bAutoShearCheckPos[0][0] = TRUE;
		pSect->SectBefore.bAutoShearCheckPos[0][1] = TRUE;
		pSect->SectBefore.bAutoShearCheckPos[0][2] = TRUE;
		pSect->SectBefore.dShearCheckPos[0][0] = m_editShear1.GetEditValue();
		pSect->SectBefore.dShearCheckPos[0][2] = m_editShear3.GetEditValue();
	}

	// CMP size
	pSect->SectBefore.SectI.Size[56] = m_editCMPa.GetEditValue();
	pSect->SectBefore.SectI.Size[57] = m_editCMPb.GetEditValue();
	pSect->SectBefore.SectI.Size[58] = m_editCMPh.GetEditValue();
	pSect->SectBefore.SectI.Size[59] = m_editCMPt.GetEditValue();

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

	// 재단고정도, 길이감소율
	pSect->SectBefore.SectI.Design.TanA = m_editEFD.GetEditValue();
	pSect->SectBefore.SectI.Design.Beta = m_editLRF.GetEditValue();

	// shear deform
	pSect->SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	pSect->SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	// warping stress point
	pSect->SectBefore.nWarpingCheck[0] = pSect->SectBefore.nWarpingCheck[1] =m_nWarpingCheck;
	double dWarpingCheckPos[2][6]={0.0};
	memcpy(pSect->SectBefore.dWarpingCheckPosI, m_SectData.SectBefore.dWarpingCheckPosI, sizeof(dWarpingCheckPos)); // I/J같음.
	memcpy(pSect->SectBefore.dWarpingCheckPosJ, m_SectData.SectBefore.dWarpingCheckPosI, sizeof(dWarpingCheckPos));

	// size - Child Dialog
 	if(m_pLeftDlg != NULL && m_pLeftDlg->GetSafeHwnd()) m_pLeftDlg->GetDlgData(pSect); 
	if(m_pRightDlg != NULL && m_pRightDlg->GetSafeHwnd()) m_pRightDlg->GetDlgData(pSect);
	if (pSect->SectBefore.bSymmetric)
	{
		pSect->SectBefore.SectI.Size[32] = pSect->SectBefore.SectI.Size[7];
		pSect->SectBefore.SectI.Size[33] = pSect->SectBefore.SectI.Size[8];
		pSect->SectBefore.SectI.Size[34] = pSect->SectBefore.SectI.Size[17];
	}

	// material
	GetMatlSectData(pSect);

	return TRUE;
}

void  CCMSectItemPSC_CMP::SetCurDlgData(void* pData, BOOL bModify)
{
	m_bModify = bModify;
	m_SectData.Initialize();
	m_SectData = *((T_SECT_D *)pData);
	m_SectData.nStype = D_SECT_TYPE_PSC;
	m_SectData.SectBefore.Shape = D_SECT_SHAPE_PSC_CMPWEB;
	if (!m_bModify && CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
}

void CCMSectItemPSC_CMP::DisplayOffsetPoint()
{
	if (m_SectData.SectBefore.nOffsetPoint == 5 && 
			m_SectData.SectBefore.nOffsetCenter == 1) RedrawSection();
	else OnCmdSectCentroidBtn();
}

void CCMSectItemPSC_CMP::ChangeMatlData(T_MATL_CONCRETE &ConcrData, T_MATL_STEEL &SteelData)
{
	// add code here
	T_SECT_SECTION_D* pData = &(m_SectData.SectBefore);
	pData->Matl_Elast = SteelData.Elast/ConcrData.Elast;
	pData->Matl_Density = SteelData.Density/ConcrData.Density;
	pData->Matl_Poisson_S = SteelData.Poisson;
	pData->Matl_Poisson_C = ConcrData.Poisson;
	SetMatlSectData();
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_CMP message handlers

BOOL CCMSectItemPSC_CMP::OnInitDialog() 
{
	CCMSectItemPSCBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDataSource(&m_SectData);
	m_pParent->DisplayShearCheck();

	LayoutChildDlgs();

	InitControls();

	SetData2Dlg();

	RedrawSection();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemPSC_CMP::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
/*
	if (m_chkSymmetic.GetCheck() == 1)
	{
		T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
		for (int i=1; i<=29; i++) pData->SectI.Size[i+29] = pData->SectI.Size[i];
	}
*/

	if (m_chkSymmetic.GetCheck() == 1)
	{
		T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
		pData->SectI.Size[32] = m_SectData.SectBefore.SectI.Size[7];
		pData->SectI.Size[33] = m_SectData.SectBefore.SectI.Size[8];
		pData->SectI.Size[34] = m_SectData.SectBefore.SectI.Size[17];
	}

	ChangeStatus();

	*pResult = 0;
}

void CCMSectItemPSC_CMP::OnCmdCMPLRFBtn()
{
	double dLRF = 0.0;
	double a, b, h, temp;

	a = m_editCMPa.GetEditValue();
	b = m_editCMPb.GetEditValue();
	h = m_editCMPh.GetEditValue();
	
	temp = a + sqrt(pow(b,2)+pow(h,2));
	if (temp == 0.) return;

	dLRF = (a+b) / temp;
	m_editLRF.SetEditUnit(dLRF);
}

void CCMSectItemPSC_CMP::OnCmdChangeOffsetBtn() 
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
			GetDlgItem(IDC_CMD_OFFSET_TEXT)->SetWindowText(csOffset);
		}
	}
	else m_SectData.SectBefore = OldData;

	RedrawSection();
}

void CCMSectItemPSC_CMP::OnCmdSectCentroidBtn() 
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

void CCMSectItemPSC_CMP::OnCmdMaterialBtn() 
{
	// TODO: Add your control notification handler code here
	CRect rect;
	GetDlgItem(IDC_CMD_MATL_BTN)->GetWindowRect(rect);
	//rect.top += 20;

	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	CCMConcrSteelMatlDlg dlg(pDoc, rect, IDD_CMD_SECT_ITEM_PSC_CMP, this);
	CStringArray aData;
	UINT aID[] = {
		IDC_CMD_MATL_MVALUE1, IDC_CMD_MATL_MVALUE2,
		IDC_CMD_MATL_MVALUE3, IDC_CMD_MATL_MVALUE4,
	};

	// save current data
	CString data;
	for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
	{
		GetDlgItem(aID[i])->GetWindowText(data);
		aData.Add(data);
	}
	if (dlg.DoModal() != IDOK)
	{
		// restore old data
		for (int i = 0; i < sizeof(aID)/sizeof(UINT); i++)
			GetDlgItem(aID[i])->SetWindowText(aData[i]);
	}		
}

void CCMSectItemPSC_CMP::OnCmdHunchCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bSmallHole = (m_chkHunch.GetCheck()==1) ? TRUE : FALSE;
	ChangeStatus();	
}

void CCMSectItemPSC_CMP::OnCmdSymmetricCheck() 
{
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.bSymmetric = m_chkSymmetic.GetCheck();
/*
	if (m_SectData.SectBefore.bSymmetric == 1)
	{
		T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
		for (int i=1; i<=29; i++) pData->SectI.Size[i+29] = pData->SectI.Size[i];
	}
	*/

	if (m_SectData.SectBefore.bSymmetric == 1)
	{
		m_SectData.SectBefore.SectI.Size[32] = m_SectData.SectBefore.SectI.Size[7];
		m_SectData.SectBefore.SectI.Size[33] = m_SectData.SectBefore.SectI.Size[8];
		m_SectData.SectBefore.SectI.Size[34] = m_SectData.SectBefore.SectI.Size[17];
	}

	ChangeStatus();
}

void CCMSectItemPSC_CMP::OnCmdShearCheck() 
{
	// TODO: Add your control notification handler code here
	BOOL bSym = (m_chkShearCheck.GetCheck()==1) ? TRUE : FALSE;
	m_SectData.SectBefore.bShearCheck = bSym;
	m_editShear1.EnableWindow(FALSE);   // 항상 자동계산 
	m_editShear2.EnableWindow(FALSE);   // 항상 자동계산 
	m_editShear3.EnableWindow(FALSE);   // 항상 자동계산 
	RedrawSection();
}

void CCMSectItemPSC_CMP::OnChangeCmdShearEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CCMSectItemPSCBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_SectData.SectBefore.dShearCheckPos[0][0] = m_editShear1.GetEditValue();
	m_SectData.SectBefore.dShearCheckPos[0][2] = m_editShear3.GetEditValue();
	RedrawSection();
}

void CCMSectItemPSC_CMP::OnChangeCmdAEdit()
{
	m_SectData.SectBefore.SectI.Size[56] = m_editCMPa.GetEditValue();
	RedrawSection();
}

void CCMSectItemPSC_CMP::OnChangeCmdBEdit()
{
	m_SectData.SectBefore.SectI.Size[57] = m_editCMPb.GetEditValue();
	RedrawSection();
}

void CCMSectItemPSC_CMP::OnChangeCmdHEdit()
{
	m_SectData.SectBefore.SectI.Size[58] = m_editCMPh.GetEditValue();
	RedrawSection();
}

void CCMSectItemPSC_CMP::OnChangeCmdTEdit()
{
	m_SectData.SectBefore.SectI.Size[59] = m_editCMPt.GetEditValue();
	RedrawSection();
}

void CCMSectItemPSC_CMP::OnChangeCmdMatlEdit()
{
	m_SectData.SectBefore.Matl_Elast = m_editMValue1.GetEditValue();
	RedrawSection();
}

BOOL CCMSectItemPSC_CMP::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_SectData.Initialize();
	if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP) 
		m_SectData.SectBefore.bConsiderShearDeform = FALSE;
	return CCMSectItemPSCBase::DestroyWindow();
}

void CCMSectItemPSC_CMP::SetAutotShearCheckPos(double dShearPos[3])
{
	if (m_chkShearCheck.GetCheck() != 1) return;

	m_SectData.SectBefore.dShearCheckPos[0][0] = dShearPos[0];
	m_editShear1.SetEditUnit(dShearPos[0]);
	
	m_SectData.SectBefore.dShearCheckPos[0][1] = dShearPos[1];
	
	m_SectData.SectBefore.dShearCheckPos[0][2] = dShearPos[2];
	m_editShear3.SetEditUnit(dShearPos[2]);

	RedrawSection();
}

void CCMSectItemPSC_CMP::SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])
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
// User Defeind Function
void CCMSectItemPSC_CMP::InitControls()
{
	m_unitCMPa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCMPa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCMPb.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCMPb.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCMPh.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCMPh.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCMPt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCMPt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitShear1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editShear1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitShear2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editShear2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitShear3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editShear3.SetUnitType(D_UNITSYS_BASE_LENGTH);
 
	m_editMValue1.SetUnitType(D_UNITSYS_NONE);
	m_editMValue2.SetUnitType(D_UNITSYS_NONE);
	m_editMValue3.SetUnitType(D_UNITSYS_NONE);
	m_editMValue4.SetUnitType(D_UNITSYS_NONE);

	m_editEFD.SetUnitType(D_UNITSYS_NONE);
	m_editLRF.SetUnitType(D_UNITSYS_NONE);

	// 재단고정도, 길이감소율 - 일본버전에서만
	BOOL bJapan = (CProduct::IsMovingType(D_PRODUCT_MOVING_JP));
	GetDlgItem(IDC_CMD_CMP_EFD_TEXT)->ShowWindow(bJapan);
	GetDlgItem(IDC_CMD_CMP_EFD_EDIT)->ShowWindow(bJapan);
	GetDlgItem(IDC_CMD_CMP_LRF_TEXT)->ShowWindow(bJapan);
	GetDlgItem(IDC_CMD_CMP_LRF_EDIT)->ShowWindow(bJapan);
	GetDlgItem(IDC_CMD_CMP_LRF_BTN)->ShowWindow(bJapan);

#if defined(_CH)
	GetDlgItem(IDC_WG_CMD_STATIC14)->ShowWindow(SW_HIDE);
#endif

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\psc_cmd_sub.svg"));
}

void CCMSectItemPSC_CMP::LayoutChildDlgs()
{
	// PlaceHolder Setting
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);

	// TabWinddow Create & Setting 
	m_pLeftDlg = new CCMSectItemPSC_CMPLeft(this);
	m_ctrlTab.AddTab(m_pLeftDlg, _LS(IDS_CMD_PSC_NCELL_LEFT), CCMSectItemPSC_CMPLeft::IDD, TRUE);

	m_pRightDlg = new CCMSectItemPSC_CMPRight(this);
	m_ctrlTab.AddTab(m_pRightDlg, _LS(IDS_CMD_PSC_NCELL_RIGHT), CCMSectItemPSC_CMPRight::IDD, TRUE);

	m_ctrlTab.ShowTab(0);
}

void CCMSectItemPSC_CMP::SetData2Dlg()
{
	// symmetric
	m_chkSymmetic.SetCheck((int)m_SectData.SectBefore.bSymmetric);

	// hunch
	m_chkHunch.SetCheck((int)m_SectData.SectBefore.bSmallHole);

	// shear check
	int nCheck = (int)m_SectData.SectBefore.bShearCheck;
	m_chkShearCheck.SetCheck(nCheck);
	m_editShear1.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][0]);
	m_editShear2.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][1]);
	m_editShear3.SetEditUnit(m_SectData.SectBefore.dShearCheckPos[0][2]);
	m_editShear1.EnableWindow(FALSE);
	m_editShear3.EnableWindow(FALSE);

	// CMP size
	m_editCMPa.SetEditUnit(m_SectData.SectBefore.SectI.Size[56]);
	m_editCMPb.SetEditUnit(m_SectData.SectBefore.SectI.Size[57]);
	m_editCMPh.SetEditUnit(m_SectData.SectBefore.SectI.Size[58]);
	m_editCMPt.SetEditUnit(m_SectData.SectBefore.SectI.Size[59]);

	// offset 
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	if (m_SectData.SectBefore.nOffsetPoint >= 1)
	{
		CString csOffset = arOffsetNameList[m_SectData.SectBefore.nOffsetPoint-1];
		GetDlgItem(IDC_CMD_OFFSET_TEXT)->SetWindowText(csOffset);
	}

	// 재단고정도, 길이감소율
	m_editEFD.SetEditUnit(m_SectData.SectBefore.SectI.Design.TanA);
	m_editLRF.SetEditUnit(m_SectData.SectBefore.SectI.Design.Beta);

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
	// enable/disable
	T_SECT_SECTION_D* pData = &m_SectData.SectBefore;
	int nCurSel = m_ctrlTab.GetCurSel();
	if (nCurSel == 0)
	{
		CCMSectItemPSC_CMPLeft* pDlg = (CCMSectItemPSC_CMPLeft*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg) 
		{
			pDlg->SetData2Dlg();
			pDlg->ChangeStatus(pData->bSymmetric, pData->bSmallHole);
		}
	}
	else if (nCurSel == 1)
	{
		CCMSectItemPSC_CMPRight* pDlg = (CCMSectItemPSC_CMPRight*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg)
		{
			pDlg->SetData2Dlg();
			pDlg->ChangeStatus(pData->bSymmetric, pData->bSmallHole);
		}
	}
	else ASSERT(0);
	
	// material
	SetMatlSectData();

	// change bitmap
	ChangeBitmap();
}

void CCMSectItemPSC_CMP::SetDlg2Data()
{
	// 여기서는 자신의 것만 저장한다.
	// Child Dialog들은 각각 알아서 저장된다.
	m_SectData.nStype = D_SECT_TYPE_PSC;

	// symmetric
	m_SectData.SectBefore.bSymmetric = (m_chkSymmetic.GetCheck()==1) ? TRUE : FALSE;

	// hunch
	m_SectData.SectBefore.bSmallHole = (m_chkHunch.GetCheck()==1) ? TRUE : FALSE;

	// shear check
	m_SectData.SectBefore.bShearCheck = m_chkShearCheck.GetCheck();
	if (m_SectData.SectBefore.bShearCheck)
	{
		m_SectData.SectBefore.bAutoShearCheckPos[0][0] = TRUE;
		m_SectData.SectBefore.bAutoShearCheckPos[0][1] = TRUE;
		m_SectData.SectBefore.bAutoShearCheckPos[0][2] = TRUE;
		m_SectData.SectBefore.dShearCheckPos[0][0] = m_editShear1.GetEditValue();
		m_SectData.SectBefore.dShearCheckPos[0][2] = m_editShear3.GetEditValue();
	}

	// CMP size
	m_SectData.SectBefore.SectI.Size[56] = m_editCMPa.GetEditValue();
	m_SectData.SectBefore.SectI.Size[57] = m_editCMPb.GetEditValue();
	m_SectData.SectBefore.SectI.Size[58] = m_editCMPh.GetEditValue();
	m_SectData.SectBefore.SectI.Size[59] = m_editCMPt.GetEditValue();

	// offset = nothing

	// 재단고정도, 길이감소율
	if(CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
	{
		m_SectData.SectBefore.SectI.Design.TanA = m_editEFD.GetEditValue();
		m_SectData.SectBefore.SectI.Design.Beta = m_editLRF.GetEditValue();
	}
	else
	{
		m_SectData.SectBefore.SectI.Design.TanA = 1.0;
		m_SectData.SectBefore.SectI.Design.Beta = 1.0;
	}

	// shear deform
	m_SectData.SectBefore.bConsiderShearDeform = m_chkShearDeform.GetCheck();
	m_SectData.SectBefore.bConsiderWarpingEffect = m_chkWarpingEffect.GetCheck();

	// warping stress point
	m_SectData.SectBefore.nWarpingCheck[0] =m_nWarpingCheck;
	m_SectData.SectBefore.nWarpingCheck[1] =m_nWarpingCheck;

	// size - symmetric
	if (m_SectData.SectBefore.bSymmetric)
	{
		m_SectData.SectBefore.SectI.Size[32] = m_SectData.SectBefore.SectI.Size[7];
		m_SectData.SectBefore.SectI.Size[33] = m_SectData.SectBefore.SectI.Size[8];
		m_SectData.SectBefore.SectI.Size[34] = m_SectData.SectBefore.SectI.Size[17];
	}

	// material
	GetMatlSectData(&m_SectData);
}

void CCMSectItemPSC_CMP::SetMatlSectData()
{
	m_editMValue1.SetEditUnit(m_SectData.SectBefore.Matl_Elast);
	m_editMValue2.SetEditUnit(m_SectData.SectBefore.Matl_Density);
	m_editMValue3.SetEditUnit(m_SectData.SectBefore.Matl_Poisson_S);
	m_editMValue4.SetEditUnit(m_SectData.SectBefore.Matl_Poisson_C);  
}

void CCMSectItemPSC_CMP::GetMatlSectData(T_SECT_D* pData)
{
	pData->SectBefore.Matl_Elast     = m_editMValue1.GetEditValue();
	pData->SectBefore.Matl_Density   = m_editMValue2.GetEditValue();
	pData->SectBefore.Matl_Poisson_S = m_editMValue3.GetEditValue();
	pData->SectBefore.Matl_Poisson_C = m_editMValue4.GetEditValue();
}

void CCMSectItemPSC_CMP::ChangeStatus()
{
	T_SECT_SECTION_D* pData = &m_SectData.SectBefore;

	int nCurSel = m_ctrlTab.GetCurSel();
	if (nCurSel == 0)
	{
		CCMSectItemPSC_CMPLeft* pDlg = (CCMSectItemPSC_CMPLeft*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg) pDlg->ChangeStatus(pData->bSymmetric, pData->bSmallHole);
	}
	else if (nCurSel == 1)
	{
		CCMSectItemPSC_CMPRight* pDlg = (CCMSectItemPSC_CMPRight*)m_ctrlTab.GetTab(nCurSel);
		if (pDlg) pDlg->ChangeStatus(pData->bSymmetric, pData->bSmallHole);
	}
	else ASSERT(0);

	RedrawSection();
}

////////////////////////////////////////////////////////////////////////////
//
// Update Viewer
BOOL CCMSectItemPSC_CMP::ChangeBitmap()
{
	ASSERT(m_pParent);
	return m_pParent->ChangeBitmap(__SECT_PSC_CMPWEB__);
}

void CCMSectItemPSC_CMP::SetDataSource(T_SECT_D* pDataSrc)
{
	ASSERT(m_pParent);
	m_pParent->SetDataSource(pDataSrc);
}

void CCMSectItemPSC_CMP::RedrawSection()
{
	ASSERT(m_pParent);
	m_pParent->RedrawSection();
}

int CCMSectItemPSC_CMP::GetSelectedTab()
{
	return m_ctrlTab.GetCurSel();
}

void CCMSectItemPSC_CMP::ChangeTab(int index)
{
	m_ctrlTab.ShowTab(index);

}

void CCMSectItemPSC_CMP::OnCmdSectTableinputBtn() 
{
	CCMSectItemPSCCMPGridDlg m_GridDlg(this);
	T_SECT_D* sect=&m_SectData;

	T_SECT_D_PSC_CMPWEB Cell;
	Cell.Set(*sect,TRUE,TRUE);
	m_GridDlg.SetData(sect,this);
	if(m_GridDlg.DoModal()!=IDOK)
	{
		Cell.Get(*sect,TRUE,TRUE);
		SetData2Dlg();
		
	}	

}

void CCMSectItemPSC_CMP::OnCmdSectWapingCheckBtn() 
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

void CCMSectItemPSC_CMP::OnCmdSectWapingCheckUser()
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

void CCMSectItemPSC_CMP::OnCmdSectWapingCheck()
{
	UpdateData(TRUE);

	BOOL bChkWE = m_chkWarpingEffect.GetCheck();
	GetDlgItem(IDC_CMD_WARPING_EFFECT_STC     )->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_AUTO_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_USER_RDO)->EnableWindow(bChkWE);
	GetDlgItem(IDC_CMD_WARPING_EFFECT_BTN     )->EnableWindow(bChkWE);
	if(bChkWE) OnCmdSectWapingCheckUser();

}