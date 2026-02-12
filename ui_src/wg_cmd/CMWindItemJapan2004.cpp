// CMWindItemJapan2004.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemJapan2004.h"
#include "CMWindItemDlg.h"

#include "CMWindItemJapan2004LoadF.h"
#include "CMWindItemJapan2004GustF.h"
#include "CMWindItemJapan2004Param.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004 dialog


CCMWindItemJapan2004::CCMWindItemJapan2004(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemJapan2004::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemJapan2004)
	//}}AFX_DATA_INIT
	m_Data.Initialize();
	m_DataBack.Initialize();
	m_bModify = FALSE;
	m_bInit = FALSE;
	m_dDgnWindSpeedX = 0.;
	m_dDgnWindSpeedY = 0.;

	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD1);
	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD2);

	m_aCtrlFcCoef.Add(IDC_CMD_FCOEF_AUTO);
	m_aCtrlFcCoef.Add(IDC_CMD_FCOEF_USER);

	m_aCtrlEFactor.Add(IDC_CMD_EFACTOR_TITLE2);
	m_aCtrlEFactor.Add(IDC_CMD_EFACTOR_EDIT2);

	m_aCtrlMethod1.Add(IDC_CMD_EFACTOR_TITLE2);
	m_aCtrlMethod1.Add(IDC_CMD_EFACTOR_EDIT2);
	m_aCtrlMethod1.Add(IDC_CMD_FCOEF_FRM);
	m_aCtrlMethod1.Add(IDC_CMD_FCOEF_AUTO);
	m_aCtrlMethod1.Add(IDC_CMD_FCOEF_USER);
	m_aCtrlMethod1.Add(IDC_CMD_FCOEF_CFX_TITLE);
	m_aCtrlMethod1.Add(IDC_CMD_FCOEF_CFX_EDIT);
	m_aCtrlMethod1.Add(IDC_CMD_FCOEF_CFY_TITLE);
	m_aCtrlMethod1.Add(IDC_CMD_FCOEF_CFY_EDIT);
		
	m_aCtrlMethod2.Add(IDC_CMD_MEAN_SPEED_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_MEAN_SPEED_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_MEAN_SPEED_UNIT);
	m_aCtrlMethod2.Add(IDC_CMD_CATEGORY_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_CATEGORY_COBX);
	m_aCtrlMethod2.Add(IDC_CMD_DIR_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_DIR_KDX_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_DIR_KDX_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_DIR_KDY_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_DIR_KDY_EDIT);  
	m_aCtrlMethod2.Add(IDC_CMD_PARAM_BTN);
	m_aCtrlMethod2.Add(IDC_CMD_GUST_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_GX_TITLE2);
	m_aCtrlMethod2.Add(IDC_CMD_GX_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_GY_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_GY_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_GFACTOR_BTN);
	m_aCtrlMethod2.Add(IDC_CMD_COMBLOAD_FRM);
	m_aCtrlMethod2.Add(IDC_CMD_WDIR_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_COMBX_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_COMBY_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_COMBZ_TITLE);
	m_aCtrlMethod2.Add(IDC_CMD_WDIR_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_COMBX_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_COMBY_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_COMBZ_EDIT);
	m_aCtrlMethod2.Add(IDC_CMD_COMB_BTN);

	m_aCtrlDirX.Add(IDC_CMD_HSHAPE_X_COBX);
	m_aCtrlDirX.Add(IDC_CMD_HHEIGHT_X_EDIT);
	m_aCtrlDirX.Add(IDC_CMD_HLEGNTH_X_EDIT);
	m_aCtrlDirX.Add(IDC_CMD_BLDGDIST_X_EDIT);

	m_aCtrlDirY.Add(IDC_CMD_HSHAPE_Y_COBX);
	m_aCtrlDirY.Add(IDC_CMD_HHEIGHT_Y_EDIT);
	m_aCtrlDirY.Add(IDC_CMD_HLEGNTH_Y_EDIT);
	m_aCtrlDirY.Add(IDC_CMD_BLDGDIST_Y_EDIT);
}

void CCMWindItemJapan2004::SetData2Dlg(T_WIND_JP2004& data, BOOL bModify)
{
	m_Data = data;
	m_DataBack = data;
	m_bModify = bModify;
}

BOOL CCMWindItemJapan2004::SetDlg2Data(T_WIND_JP2004& data)
{
	BOOL bOK = Dlg2Data(&m_Data);
	data = m_Data;
	return bOK;
}

void CCMWindItemJapan2004::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemJapan2004)
	DDX_Control(pDX, IDC_CMD_COMBZ_EDIT, m_editCombZ);
	DDX_Control(pDX, IDC_CMD_COMBY_EDIT, m_editCombY);
	DDX_Control(pDX, IDC_CMD_COMBX_EDIT, m_editCombX);
	DDX_Control(pDX, IDC_CMD_WDIR_EDIT, m_editWindDir);
	DDX_Control(pDX, IDC_CMD_GY_EDIT, m_editGy);
	DDX_Control(pDX, IDC_CMD_GX_EDIT, m_editGx);
	DDX_Control(pDX, IDC_CMD_FCOEF_CFY_EDIT, m_editCfy);
	DDX_Control(pDX, IDC_CMD_FCOEF_CFX_EDIT, m_editCfx);
	DDX_Control(pDX, IDC_CMD_BLDGDIST_UNIT, m_unitHDist);
	DDX_Control(pDX, IDC_CMD_BLDGDIST_Y_EDIT, m_editHDistY);
	DDX_Control(pDX, IDC_CMD_BLDGDIST_X_EDIT, m_editHDistX);
	DDX_Control(pDX, IDC_CMD_HLEGNTH_UNIT, m_unitHLeng);
	DDX_Control(pDX, IDC_CMD_HLEGNTH_Y_EDIT, m_editHLengY);
	DDX_Control(pDX, IDC_CMD_HLEGNTH_X_EDIT, m_editHLengX);
	DDX_Control(pDX, IDC_CMD_HHEIGHT_UNIT, m_unitHHeight);
	DDX_Control(pDX, IDC_CMD_HHEIGHT_Y_EDIT, m_editHHeightY);
	DDX_Control(pDX, IDC_CMD_HHEIGHT_X_EDIT, m_editHHeightX);
	DDX_Control(pDX, IDC_CMD_HSHAPE_Y_COBX, m_cobxHShapeY);
	DDX_Control(pDX, IDC_CMD_HSHAPE_X_COBX, m_cobxHShapeX);
	DDX_Control(pDX, IDC_CMD_DIR_Y_CHECK, m_chkDirY);
	DDX_Control(pDX, IDC_CMD_DIR_X_CHECK, m_chkDirX);
	DDX_Control(pDX, IDC_CMD_DIR_KDY_EDIT, m_editKdy);
	DDX_Control(pDX, IDC_CMD_DIR_KDX_EDIT, m_editKdx);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COBX, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_HEIGHT_UNIT, m_unitHeight);
	DDX_Control(pDX, IDC_CMD_HEIGHT_EDIT, m_editHeight);
	DDX_Control(pDX, IDC_CMD_EFACTOR_EDIT2, m_editEFactor);
	DDX_Control(pDX, IDC_CMD_MEAN_SPEED_EDIT, m_editMeanSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemJapan2004, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemJapan2004)
	ON_BN_CLICKED(IDC_CMD_COMB_BTN, OnCmdCombBtn)
	ON_BN_CLICKED(IDC_CMD_DIR_X_CHECK, OnCmdDirXCheck)
	ON_BN_CLICKED(IDC_CMD_DIR_Y_CHECK, OnCmdDirYCheck)
	ON_BN_CLICKED(IDC_CMD_GFACTOR_BTN, OnCmdGfactorBtn)
	ON_BN_CLICKED(IDC_CMD_FCOEF_AUTO, OnCmdForceCoefRadio)
	ON_BN_CLICKED(IDC_CMD_FCOEF_USER, OnCmdForceCoefRadio)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD1, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD2, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_PARAM_BTN, OnCmdParamBtn)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan2004 message handlers

BOOL CCMWindItemJapan2004::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	AlignControls();

	InitControls();

	if (!m_bInit) 
	{
		InitDefaultData();
		m_bInit = TRUE;
	}

	m_grpWindParam.InitControl(this, CCMWindItemJapan2004::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemJapan2004::DestroyWindow() 
{
	Dlg2Data(&m_Data);

	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

void CCMWindItemJapan2004::OnCmdMethodRadio() 
{
	// TODO: Add your control notification handler code here
	ChangeMethod();
}

void CCMWindItemJapan2004::OnCmdDirXCheck() 
{
	// TODO: Add your control notification handler code here
	ChangeTopoXY();
}

void CCMWindItemJapan2004::OnCmdDirYCheck() 
{
	// TODO: Add your control notification handler code here
	ChangeTopoXY();
}

void CCMWindItemJapan2004::OnCmdForceCoefRadio() 
{
	// TODO: Add your control notification handler code here
	ChangeFcCoef();
}

void CCMWindItemJapan2004::OnCmdCombBtn() 
{
	// TODO: Add your control notification handler code here
	T_WIND_JP2004 data;
	data.Initialize();
	if (!Dlg2Data(&data)) ASSERT(0);

	CCMWindItemJapan2004LoadF dlg(&data);
	if (dlg.DoModal() == IDOK)
	{
		m_DataBack.nWindDirection = data.nWindDirection;
		m_DataBack.nCombType = data.nCombType;

		CString strDir[] = { _LS(IDS_CMD_WIND_X_DIRECTION), _LS(IDS_CMD_WIND_Y_DIRECTION) };
		m_editWindDir.SetWindowText(strDir[data.nWindDirection]);
		m_editCombX.SetEditUnit(data.dLoadFactorX);
		m_editCombY.SetEditUnit(data.dLoadFactorY);

		if(data.nCombType == 3) data.dLoadFactorR = 0.;
		m_editCombZ.SetEditUnit(data.dLoadFactorR);
		m_editCombZ.EnableWindow(data.nCombType != 3);
	}
}

void CCMWindItemJapan2004::OnCmdGfactorBtn() 
{
	// TODO: Add your control notification handler code here
	T_WIND_JP2004 data;
	data.Initialize();
	if (!Dlg2Data(&data)) ASSERT(0);

	CCMWindItemJapan2004GustF dlg(&data);
	if (dlg.DoModal() == IDOK)
	{
		m_editGx.SetEditUnit(data.dGustFactorX);
		m_editGy.SetEditUnit(data.dGustFactorY);
	}
}

void CCMWindItemJapan2004::OnCmdParamBtn() 
{
	// TODO: Add your control notification handler code here
	CCMWindItemJapan2004Param dlg;

	dlg.m_dBreadthX    = m_DataBack.dBreadthX;
	dlg.m_dBreadthY    = m_DataBack.dBreadthY;
	dlg.m_dDampRatioX  = m_DataBack.dDampingX;
	dlg.m_dDampRatioY  = m_DataBack.dDampingY;
	dlg.m_dDampRatioR  = m_DataBack.dDampingR;
	dlg.m_dFrequencyX  = m_DataBack.dFrequencyX;
	dlg.m_dFrequencyY  = m_DataBack.dFrequencyY;
	dlg.m_dFrequencyR  = m_DataBack.dFrequencyR;
	dlg.m_dModeFactorX = m_DataBack.dModeFactorX;
	dlg.m_dModeFactorY = m_DataBack.dModeFactorY;
	dlg.m_dModeFactorR = m_DataBack.dModeFactorR;

	if (dlg.DoModal() == IDOK)
	{
		m_DataBack.dBreadthX    = dlg.m_dBreadthX   ;
		m_DataBack.dBreadthY    = dlg.m_dBreadthY   ;
		m_DataBack.dDampingX    = dlg.m_dDampRatioX ;
		m_DataBack.dDampingY    = dlg.m_dDampRatioY ;
		m_DataBack.dDampingR    = dlg.m_dDampRatioR ;
		m_DataBack.dFrequencyX  = dlg.m_dFrequencyX ;
		m_DataBack.dFrequencyY  = dlg.m_dFrequencyY ;
		m_DataBack.dFrequencyR  = dlg.m_dFrequencyR ;
		m_DataBack.dModeFactorX = dlg.m_dModeFactorX;
		m_DataBack.dModeFactorY = dlg.m_dModeFactorY;
		m_DataBack.dModeFactorR = dlg.m_dModeFactorR;
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemJapan2004::AlignControls()
{
	CRect fromRect, toRect;
	GetDlgItem(IDC_CMD_PARAM_BTN)->GetWindowRect(fromRect);
	GetDlgItem(IDC_CMD_FCOEF_FRM)->GetWindowRect(toRect);
	int nDistY = toRect.top - fromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod1, -nDistY+3);

	GetDlgItem(IDC_CMD_EFACTOR_EDIT2)->GetWindowRect(fromRect);
	GetDlgItem(IDC_CMD_MEAN_SPEED_EDIT)->GetWindowRect(toRect);
	nDistY = toRect.top - fromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlEFactor, nDistY);
}

void CCMWindItemJapan2004::InitControls()
{
	InitCategoryCobx();
	InitHillShapeCobx();

	m_editHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_editHHeightX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHHeightY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_editHLengX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHLengY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHLeng.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	m_editHDistX.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHDistY.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMWindItemJapan2004::ChangeMethod()
{
	int nMethod;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, nMethod==0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, nMethod==1);

//  CString strTitle[] = { _LS(IDS_CMD_WIND_ENVIRONMENTAL_FACTOR), _LS(IDS_CMD_WIND_MEAN_WIND_SPEED) };
//  GetDlgItem(IDC_CMD_EFACTOR_TITLE)->SetWindowText(strTitle[nMethod]);
}

void CCMWindItemJapan2004::ChangeTopoXY()
{
	int nCheck;
	nCheck = m_chkDirX.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDirX, nCheck == 1);
	nCheck = m_chkDirY.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDirY, nCheck == 1);
}

void CCMWindItemJapan2004::ChangeFcCoef()
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlFcCoef, nCheck);
	m_editCfx.EnableWindow(nCheck == 1);
	m_editCfy.EnableWindow(nCheck == 1);
}

void CCMWindItemJapan2004::InitCategoryCobx()
{
	m_cobxCategory.ResetContent();

	CString aItem[] = { _T("I"), _T("II"), _T("III"), _T("IV"), _T("V") };
	int nNum = sizeof (aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCategory.AddString(aItem[i]);
	m_cobxCategory.SetCurSel(0);
}

void CCMWindItemJapan2004::InitHillShapeCobx()
{
	m_cobxHShapeX.ResetContent();
	m_cobxHShapeY.ResetContent();

	CString aItem[] = { _LS(IDS_CMD_WIND_ESCARPMENT), _LS(IDS_CMD_WIND_HILL) };
	int nNum = sizeof(aItem)/sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxHShapeX.AddString(aItem[i]);
		m_cobxHShapeY.AddString(aItem[i]);
	}
	m_cobxHShapeX.SetCurSel(0);
	m_cobxHShapeY.SetCurSel(0);
}

void CCMWindItemJapan2004::InitDefaultData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	// 건물의 높이 자동계산
	CArray<T_STOR_K, T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nNumStor = aStorK.GetSize();
	T_STOR_D StorD;
	StorD.Initialize();
	if (nNumStor > 0) pDoc->m_pAttrCtrl->GetStor(aStorK[nNumStor-1], StorD);

	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	if (!m_bModify) 
	{
		m_Data.dBuildingHeight = StorD.dStoryLevel - dBaseLevel;	
	}

	// Bx, By 자동계산
	double dBx = 0.0;
	double dBy = 0.0;
	if (nNumStor > 0)
	{	
		for (int i=0; i<nNumStor; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);
			if (StorD.dStoryLevel > dBaseLevel) break;
		}
		if (StorD.dStoryLevel > dBaseLevel) 
		{
			dBx = StorD.WindWidthX;
			dBy = StorD.WindWidthY;
		}
		else
		{
			dBx = 0.0; dBy = 0.0;
		}
	}
	m_DataBack.dBreadthX = dBx;
	m_DataBack.dBreadthY = dBy;
}

void CCMWindItemJapan2004::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nProcedure-1);
	ChangeMethod();

	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editEFactor.SetEditUnit(m_Data.dEnvironmentFactor);
	m_editMeanSpeed.SetEditUnit(m_Data.dMeanWindSpeed);
	m_editHeight.SetEditUnit(m_Data.dBuildingHeight);
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory);
	m_editKdx.SetEditUnit(m_Data.dDirectionFactorX);
	m_editKdy.SetEditUnit(m_Data.dDirectionFactorY);

	m_chkDirX.SetCheck(m_Data.bTopographyX);
	m_cobxHShapeX.SetCurSel(m_Data.nHillShapeX);
	m_editHHeightX.SetEditUnit(m_Data.dHillHeightX);
	m_editHLengX.SetEditUnit(m_Data.dHillLengthX);
	m_editHDistX.SetEditUnit(m_Data.dBldgDistanceX);

	m_chkDirY.SetCheck(m_Data.bTopographyY);
	m_cobxHShapeY.SetCurSel(m_Data.nHillShapeY);
	m_editHHeightY.SetEditUnit(m_Data.dHillHeightY);
	m_editHLengY.SetEditUnit(m_Data.dHillLengthY);
	m_editHDistY.SetEditUnit(m_Data.dBldgDistanceY);

	ChangeTopoXY();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlFcCoef, m_Data.nForceCoefMethod);
	ChangeFcCoef();
	m_editCfx.SetEditUnit(m_Data.dForceCoefficientX);
	m_editCfy.SetEditUnit(m_Data.dForceCoefficientY);

	m_editGx.SetEditUnit(m_Data.dGustFactorX);
	m_editGy.SetEditUnit(m_Data.dGustFactorY);
	if (m_Data.nWindDirection == 0)
		m_editWindDir.SetWindowText(_LS(IDS_CMD_WIND_X_DIRECTION));
	else 
		m_editWindDir.SetWindowText(_LS(IDS_CMD_WIND_Y_DIRECTION));
	m_editCombX.SetEditUnit(m_Data.dLoadFactorX);
	m_editCombY.SetEditUnit(m_Data.dLoadFactorY);
	m_editCombZ.SetEditUnit(m_Data.dLoadFactorR);
	m_editCombZ.EnableWindow(m_Data.nCombType != 3);
}

BOOL CCMWindItemJapan2004::Dlg2Data(T_WIND_JP2004* pData)
{
	if (pData == 0) { ASSERT(0); return FALSE; }
	pData->Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, pData->nProcedure);
	pData->nProcedure++;
	
	pData->dBasicWindSpeed = m_editSpeed.GetEditValue();
	pData->dBuildingHeight = m_editHeight.GetEditValue();
	if (pData->nProcedure == 1)        // Simplified Method
	{
		pData->dEnvironmentFactor = m_editEFactor.GetEditValue();

		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlFcCoef, pData->nForceCoefMethod);
		if (pData->nForceCoefMethod == 1)    // User Defined
		{
			pData->dForceCoefficientX = m_editCfx.GetEditValue();
			pData->dForceCoefficientY = m_editCfy.GetEditValue();
		}
	}
	else if (pData->nProcedure == 2)   // General Method
	{
		pData->dMeanWindSpeed = m_editMeanSpeed.GetEditValue();
		pData->nExposureCategory = m_cobxCategory.GetCurSel();
		pData->dDirectionFactorX = m_editKdx.GetEditValue();
		pData->dDirectionFactorY = m_editKdy.GetEditValue();

		pData->dBreadthX    = m_DataBack.dBreadthX;
		pData->dBreadthY    = m_DataBack.dBreadthY;
		pData->dDampingX    = m_DataBack.dDampingX;
		pData->dDampingY    = m_DataBack.dDampingY;
		pData->dDampingR    = m_DataBack.dDampingR;
		pData->dFrequencyX  = m_DataBack.dFrequencyX;
		pData->dFrequencyY  = m_DataBack.dFrequencyY;
		pData->dFrequencyR  = m_DataBack.dFrequencyR;
		pData->dModeFactorX = m_DataBack.dModeFactorX;
		pData->dModeFactorY = m_DataBack.dModeFactorY;
		pData->dModeFactorR = m_DataBack.dModeFactorR;

		pData->dGustFactorX = m_editGx.GetEditValue();
		pData->dGustFactorY = m_editGy.GetEditValue();
		
		pData->nWindDirection = m_DataBack.nWindDirection;
		pData->nCombType = m_DataBack.nCombType;
		pData->dLoadFactorX = m_editCombX.GetEditValue();
		pData->dLoadFactorY = m_editCombY.GetEditValue();
		pData->dLoadFactorR = m_editCombZ.GetEditValue();
	}
	else { ASSERT(0); return FALSE; }

	pData->bTopographyX = m_chkDirX.GetCheck();
	if (pData->bTopographyX)
	{
		pData->nHillShapeX = m_cobxHShapeX.GetCurSel();
		pData->dHillHeightX = m_editHHeightX.GetEditValue();
		pData->dHillLengthX = m_editHLengX.GetEditValue(); 
		pData->dBldgDistanceX = m_editHDistX.GetEditValue();
	}

	pData->bTopographyY = m_chkDirY.GetCheck();
	if (pData->bTopographyY)
	{
		pData->nHillShapeY = m_cobxHShapeY.GetCurSel();
		pData->dHillHeightY = m_editHHeightY.GetEditValue();
		pData->dHillLengthY = m_editHLengY.GetEditValue();
		pData->dBldgDistanceY = m_editHDistY.GetEditValue();
	}

	return TRUE;
}

LRESULT CCMWindItemJapan2004::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		BOOL bFolded = m_grpWindParam.GetFoldState();
		if (!bFolded)
		{
				ChangeMethod();
		}

		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
