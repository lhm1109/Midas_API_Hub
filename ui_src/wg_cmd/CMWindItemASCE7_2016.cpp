// CMWindItemASCE7_2016.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindItemASCE7_2016.h"
#include "WindAutoCalcDlg.h"
#include "CMWindItemIBC2012Topography.h"

#include "WindGustFIBC2000Dlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemASCE7_2016 dialog


CCMWindItemASCE7_2016::CCMWindItemASCE7_2016(CWnd* pParent /*=NULL*/, int nCode)
: CChildDialog(CCMWindItemASCE7_2016::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemASCE7_2016)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	

	m_aCtrlStructType.Add(IDC_CMD_ANAL_RIGID_RD);
	m_aCtrlStructType.Add(IDC_CMD_ANAL_FLEX_RD);
	
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_KDX_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_X_EDIT);  
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_KDY_TITLE);	
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_DFACTOR_Y_EDIT);	  
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_RIGID_RD);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FLEX_RD);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GX_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GX_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GY_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_GY_EDIT);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_GUST_BUTTON);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_LOADEVAL_FRAME);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_LOADEVAL_CHK);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FCOEF_TITLE);
	m_aCtrlAnalytic.Add(IDC_CMD_ANAL_FCOEF_EDIT); 
	

	m_aCtrlForceCoef.RemoveAll();
	m_aCtrlForceCoef.Add(IDC_CMD_ANAL_FCOEF_EDIT);
	m_aCtrlForceCoef.Add(IDC_CMD_ANAL_FCOEF_TITLE);
	m_aCtrlForceCoef.FreeExtra();
	
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_POS);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NEG);
	m_aCtrlExRadio.Add(IDC_CMD_XDIR_NONE);
	
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_POS);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NEG);
	m_aCtrlEyRadio.Add(IDC_CMD_YDIR_NONE);

	m_bInit = FALSE;
	m_bModify = FALSE;

	m_nCode = nCode;
	
	const bool bUnitUS = [nCode]()
	{
		switch (nCode)
		{
		case ASCE7_W_2016:
		case ASCE7_W_2022:
			return true;
		case NSCP_W_2024:
			return false;
		default:
			ASSERT(0);
			return true;
		}
	}();	

	m_Data.Initialize(bUnitUS);
	m_WnatD.Initialize();
}

void CCMWindItemASCE7_2016::SetData2Dlg(T_WIND_ASCE7_16& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;	
}

BOOL CCMWindItemASCE7_2016::SetDlg2Data(T_WIND_ASCE7_16& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}


void CCMWindItemASCE7_2016::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemASCE7_2016)
	DDX_Control(pDX, IDC_CMD_ANAL_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_ANAL_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ANAL_ROOFH_UNIT, m_unitRoofH);
	DDX_Control(pDX, IDC_CMD_ANAL_ROOFH_EDIT, m_editRoofH);
	DDX_Control(pDX, IDC_CMD_ANAL_LOADEVAL_CHK, m_chkLoadEval);
	DDX_Control(pDX, IDC_CMD_ANAL_GUST_GY_EDIT, m_editGustGy);
	DDX_Control(pDX, IDC_CMD_ANAL_GUST_GX_EDIT, m_editGustGx);
	DDX_Control(pDX, IDC_CMD_ANAL_FCOEF_EDIT, m_editForceCoef);
	DDX_Control(pDX, IDC_CMD_ANAL_GFACTOR_X_EDIT, m_editGFactorX);
	DDX_Control(pDX, IDC_CMD_ANAL_GFACTOR_Y_EDIT, m_editGFactorY);
	DDX_Control(pDX, IDC_CMD_ANAL_DFACTOR_X_EDIT, m_editDFactorX);
	DDX_Control(pDX, IDC_CMD_ANAL_DFACTOR_Y_EDIT, m_editDFactorY);	
	DDX_Control(pDX, IDC_CMD_ANAL_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemASCE7_2016, CChildDialog)
//{{AFX_MSG_MAP(CCMWindItemASCE7_2016)
	ON_BN_CLICKED(IDC_CMD_ANAL_FLEX_RD, OnCmdAnalRigidFlexRd)
	ON_BN_CLICKED(IDC_CMD_ANAL_RIGID_RD, OnCmdAnalRigidFlexRd)
	ON_BN_CLICKED(IDC_CMD_ANAL_GUST_BUTTON, OnCmdAnalGustButton)
	ON_BN_CLICKED(IDC_CMD_ANAL_LOADEVAL_CHK, OnCmdAnalLoadevalChk)
	ON_BN_CLICKED(IDC_CMD_TOPOGRAPHY_BUTTON, OnBtnTopography)
	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemASCE7_2016 message handlers

BOOL CCMWindItemASCE7_2016::OnInitDialog()
{
	CChildDialog::OnInitDialog();
	
	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	
	InitControls();
// 	AlignControls();

	m_grpWindParam.InitControl(this, CCMWindItemASCE7_2016::IDD, IDC_CMD_FRAME, TRUE, TRUE); // AlignControls 다음에
	m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemASCE7_2016::DestroyWindow()
{
	Dlg2Data();

	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

void CCMWindItemASCE7_2016::OnCmdAnalRigidFlexRd()
{
	// TODO: Add your control notification handler code here
	int nRigidFlex;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, nRigidFlex);
	if (nRigidFlex == 0)
	{
		GetDlgItem(IDC_CMD_ANAL_GUST_GX_TITLE)->SetWindowText(_T("Gx"));
		GetDlgItem(IDC_CMD_ANAL_GUST_GY_TITLE)->SetWindowText(_T("Gy"));	
	}
	else
	{
		GetDlgItem(IDC_CMD_ANAL_GUST_GX_TITLE)->SetWindowText(_T("Gfx"));
		GetDlgItem(IDC_CMD_ANAL_GUST_GY_TITLE)->SetWindowText(_T("Gfy"));
	}
}

void CCMWindItemASCE7_2016::OnCmdAnalGustButton()
{
	// TODO: Add your control notification handler code here
	int nRigidFlex;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, nRigidFlex);
		
	const int nCodeType = m_nCode;
	if (nRigidFlex == 0)
	{
		const CWindGustFIBC2000Dlg::CALCTYPE nCalcType = [nCodeType]()
		{
			switch (nCodeType)
			{
			case ASCE7_W_2016:
			case ASCE7_W_2022:
				return CWindGustFIBC2000Dlg::IBC;
			case NSCP_W_2024:
				return CWindGustFIBC2000Dlg::NSCP;
			default:
				ASSERT(0);
				return CWindGustFIBC2000Dlg::IBC;
			}
		}();

		CWindGustFIBC2000Dlg dlg;
		dlg.SetDlgType(nCalcType);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.m_nExposure = CDlgUtil::CobxGetCurSelItemData(m_cobxCategory, m_cobxCategory.GetCurSel()) - 1;
		dlg.m_dheight = m_editRoofH.GetEditValue();
		dlg.m_dBX = m_dBYIBC2012;
		dlg.m_dBY = m_dBXIBC2012;		
		
		if(dlg.DoModal() == IDOK)
		{
			m_editGustGx.SetEditUnit(dlg.m_dXFactor);
			m_editGustGy.SetEditUnit(dlg.m_dYFactor);
			m_dBXIBC2012 = dlg.m_dBY;
			m_dBYIBC2012 = dlg.m_dBX;
		}
	}
	else
	{
		const CWindGustF2IBC2000Dlg::CALCTYPE nCalcType = [nCodeType]()
		{
			switch (nCodeType)
			{
			case ASCE7_W_2016:
			case ASCE7_W_2022:
				return CWindGustF2IBC2000Dlg::IBC;
			case NSCP_W_2024:
				return CWindGustF2IBC2000Dlg::NSCP;
			default:
				ASSERT(0);
				return CWindGustF2IBC2000Dlg::IBC;
			}		
		}();

		CWindGustF2IBC2000Dlg dlg;
		dlg.SetDlgType(nCalcType);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.m_dBasicWind = m_editSpeed.GetEditValue();
		dlg.m_nExposure = CDlgUtil::CobxGetCurSelItemData(m_cobxCategory, m_cobxCategory.GetCurSel()) - 1;
		dlg.m_dheight = m_editRoofH.GetEditValue();
		dlg.m_dBX = m_dBYIBC2012;
		dlg.m_dBY = m_dBXIBC2012;
		dlg.m_dXNatural = m_dXNaturalIBC2012;
		dlg.m_dYNatural = m_dYNaturalIBC2012;
		dlg.m_dDamping	= m_dDampingIBC2012;
		
		if(dlg.DoModal() == IDOK)
		{
			m_editGustGx.SetEditUnit(dlg.m_dXFactor);
			m_editGustGy.SetEditUnit(dlg.m_dYFactor);
			m_dBXIBC2012 = dlg.m_dBY;
			m_dBYIBC2012 = dlg.m_dBX;
			m_dXNaturalIBC2012 = dlg.m_dXNatural;  
			m_dYNaturalIBC2012 = dlg.m_dYNatural; 
			m_dDampingIBC2012  = dlg.m_dDamping;	
		}
	}	
}

void CCMWindItemASCE7_2016::OnCmdAnalLoadevalChk()
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkLoadEval.GetCheck();
	BOOL bEnable = (nCheck == 1)? TRUE : FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlForceCoef, bEnable);
	
}

void CCMWindItemASCE7_2016::OnBtnTopography()
{
	CCMWindItemIBC2012Topography dlg(&m_Data);
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindItemASCE7_2016::InitDefaultData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	// 층개수와 Base Level 구하기 
	double dBaseLevel = pDoc->m_pAttrCtrl->GetGroundLevel();
	T_STOR_D StorD, StorD2;
	CArray<T_STOR_K,T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);  
	int nStorNum = aStorK.GetSize();
	if (nStorNum > 0)	pDoc->m_pAttrCtrl->GetStor(aStorK[nStorNum-1], StorD);
	else StorD.Initialize();
	
	// 폭 구하기 
	double dBx = 0.0;
	double dBy = 0.0;
	if (nStorNum > 0)
	{	
		for (int i=0; i<nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}
		if(StorD2.dStoryLevel > dBaseLevel) 
		{
			dBx = StorD2.WindWidthX;
			dBy = StorD2.WindWidthY;
		}
		else
		{
			dBx = 0.0; dBy = 0.0;
		}
	}
	// 초기화
	m_dBXIBC2012 = dBx;	
	m_dBYIBC2012 = dBy;	
	m_dXNaturalIBC2012 = 0.0;
	m_dYNaturalIBC2012 = 0.0;
	m_dDampingIBC2012  = 0.0;
	
	if (!m_bModify) 
		m_Data.dMeadRoofHeight = StorD.dStoryLevel - dBaseLevel;
}

void CCMWindItemASCE7_2016::InitControls()
{
	m_unitRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHRoof);
	m_editRoofH.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHRoof);
	
	InitCategoryCombo();
}


void CCMWindItemASCE7_2016::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	CString aItem[] = {_T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
	{
		CDlgUtil::CobxAddItem(m_cobxCategory, aItem[i], EN_ASCE7_EXPO_B + i);
	}
	m_cobxCategory.SetCurSel(1);
}

void CCMWindItemASCE7_2016::Data2Dlg()
{
	CDlgUtil::CobxSetCurSelItemData(m_cobxCategory, m_Data.nExposureCategory);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editGFactorX.SetEditUnit(m_Data.dGroundElevationFactorX);
	m_editGFactorY.SetEditUnit(m_Data.dGroundElevationFactorY);
	m_editDFactorX.SetEditUnit(m_Data.dDirectionFactorX);
	m_editDFactorY.SetEditUnit(m_Data.dDirectionFactorY);	
	m_editRoofH.SetEditUnit(m_Data.dMeadRoofHeight);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlStructType, m_Data.nRigidity);
	OnCmdAnalRigidFlexRd();
	m_editGustGx.SetEditUnit(m_Data.dGustFactorX);
	m_editGustGy.SetEditUnit(m_Data.dGustFactorY);
	
	m_chkLoadEval.SetCheck(m_Data.bUseForceCoef);
	OnCmdAnalLoadevalChk();
	m_editForceCoef.SetEditUnit(m_Data.dForceCoefficient);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);

	const CString& strUnitSpeed = [](int nCode)
	{
		switch (nCode)
		{
		case ASCE7_W_2016:
		case ASCE7_W_2022:
			return _T("mile/h");
		case NSCP_W_2024:
			return _T("m/sec");
		default:
			ASSERT(0);
			return _T("mile/h");
		}
	}(m_nCode);

	m_unitSpeed.SetWindowText(strUnitSpeed);
}

BOOL CCMWindItemASCE7_2016::Dlg2Data()
{
	m_Data.nExposureCategory = CDlgUtil::CobxGetCurSelItemData(m_cobxCategory, m_cobxCategory.GetCurSel());
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();		
	m_Data.dMeadRoofHeight = m_editRoofH.GetEditValue();
		
	m_Data.dGroundElevationFactorX = m_editGFactorX.GetEditValue();
	m_Data.dGroundElevationFactorY = m_editGFactorY.GetEditValue();

	m_Data.dDirectionFactorX = m_editDFactorX.GetEditValue();
	m_Data.dDirectionFactorY = m_editDFactorY.GetEditValue();		
		
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlStructType, m_Data.nRigidity);
	m_Data.dGustFactorX = m_editGustGx.GetEditValue();
	m_Data.dGustFactorY = m_editGustGy.GetEditValue();
				
	m_Data.bUseForceCoef = m_chkLoadEval.GetCheck();
	if (m_Data.bUseForceCoef)
		m_Data.dForceCoefficient = m_editForceCoef.GetEditValue();
		
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlExRadio, m_Data.nEccen_X);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEyRadio, m_Data.nEccen_Y);

	return TRUE;
}

LRESULT CCMWindItemASCE7_2016::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	CRect rcChild;
	GetWindowRect(rcChild);
	m_pParent->AlignCtrl2Param(rcChild.bottom);

	return 0l;
}
