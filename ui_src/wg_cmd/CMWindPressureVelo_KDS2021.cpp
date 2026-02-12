// CMWindPressureVelo_KDS2021.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindPressureVelo_KDS2021.h"
#include "WindClassifyKBC2015Dlg.h"
#include "WindVibrationDlg.h"
#include "WindGustKBC2008Dlg.h"
#include "WindAutoCalcDlg.h"

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
// CCMWindPressureVelo_KDS2021 dialog


CCMWindPressureVelo_KDS2021::CCMWindPressureVelo_KDS2021(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindPressureVelo_KDS2021::IDD, pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindPressureVelocityAddMod*)pParent;
	//{{AFX_DATA_INIT(CCMWindPressureVelo_KDS2021)
	//}}AFX_DATA_INIT

	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD1);
	m_aCtrlMethod.Add(IDC_CMD_METHOD_RD2);

	m_aCtrlVortex.Add(IDC_CMD_VORTEX_ROOF_HEIGHT_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_ROOF_HEIGHT_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_ROOF_HEIGHT_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DM_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DM_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DM_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DB_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DB_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DIA_DB_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_NATURAL_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_NATURAL_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_NATURAL_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_MASS_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_MASS_EDT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_MASS_UNT);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DAMPING_STC);
	m_aCtrlVortex.Add(IDC_CMD_VORTEX_DAMPING_EDT);

	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_EDIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_SPEED_UNIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_ROOF_HEIGHT_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_ROOF_HEIGHT_EDIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_ROOF_HEIGHT_UNIT);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_CE_TITLE);
	m_aCtrlSimple.Add(IDC_CMD_SIMPLE_CE_COMBO);

	m_aCtrlGeneral.Add(IDC_CMD_KOR_EC_TITLE);
	m_aCtrlGeneral.Add(IDC_CMD_CATEGORY_COMBO);
	m_aCtrlGeneral.Add(IDC_CMD_KOR_BWS_TITLE);
	m_aCtrlGeneral.Add(IDC_CMD_SPEED_EDIT);
	m_aCtrlGeneral.Add(IDC_CMD_SPEED_UNIT);
	m_aCtrlGeneral.Add(IDC_CMD_KOR_PC_TITLE);
	m_aCtrlGeneral.Add(IDC_CMD_IMPORTANCE_COMBO);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC0);
	m_aCtrlGeneral.Add(IDC_CMD_ROOF_HEIGHT_EDIT);
	m_aCtrlGeneral.Add(IDC_CMD_ROOF_HEIGHT_UNIT);

	m_aCtrlGeneral.Add(IDC_CMD_TOPO_EFFECT_CHECK);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC1);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC2);
	m_aCtrlGeneral.Add(IDC_WG_CMD_STATIC3);
	m_aCtrlGeneral.Add(IDC_CMD_KZT_EDIT);
// 	m_aCtrlGeneral.Add(IDC_CMD_VKZT_STATIC);
// 	m_aCtrlGeneral.Add(IDC_CMD_VKZT_EDIT);
// 	m_aCtrlGeneral.Add(IDC_CMD_VKZT_UNIT);
	
	m_bInit = FALSE;
	m_Data.Initialize();
}

void CCMWindPressureVelo_KDS2021::SetData2Dlg(T_WVEP_KDS2021& data)
{
	m_Data = data;
	Data2Dlg();
}

BOOL CCMWindPressureVelo_KDS2021::SetDlg2Data(T_WVEP_KDS2021& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindPressureVelo_KDS2021::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindPressureVelo_KDS2021)
// 	DDX_Control(pDX, IDC_CMD_SIMPLE_SPEED_UNIT, m_unitSpeed2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_SPEED_EDIT, m_editSpeed2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_ROOF_HEIGHT_UNIT, m_unitRoofHeight2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_ROOF_HEIGHT_EDIT, m_editRoofHeight2);
	DDX_Control(pDX, IDC_CMD_SIMPLE_CE_COMBO, m_cobxCeFactor);
	DDX_Control(pDX, IDC_CMD_TOPO_EFFECT_CHECK, m_chkTopoEffect);
	DDX_Control(pDX, IDC_CMD_VKZT_UNIT, m_unitVKZT);
	DDX_Control(pDX, IDC_CMD_VKZT_EDIT, m_editVKZT);
// 	DDX_Control(pDX, IDC_CMD_SPEED_UNIT, m_unitSpeed);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_UNIT, m_unitRoofHeight);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_EDIT, m_editRoofHeight);
	DDX_Control(pDX, IDC_CMD_KZT_EDIT, m_editKZT);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_COMBO, m_cobxImportFactor);

	DDX_Control(pDX, IDC_CMD_VORTEX_CHECK, m_chkVortex);
	DDX_Control(pDX, IDC_CMD_VORTEX_ROOF_HEIGHT_UNT, m_unitRoofHeight3);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DM_UNT, m_unitDiaDM);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DB_UNT, m_unitDiaDB);
	//DDX_Control(pDX, IDC_CMD_VORTEX_NATURAL_UNT, m_unitNatural);
	DDX_Control(pDX, IDC_CMD_VORTEX_MASS_UNT, m_unitMass);
	DDX_Control(pDX, IDC_CMD_VORTEX_ROOF_HEIGHT_EDT, m_editRoofHeight3);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DM_EDT, m_editDiaDM);
	DDX_Control(pDX, IDC_CMD_VORTEX_DIA_DB_EDT, m_editDiaDB);
	DDX_Control(pDX, IDC_CMD_VORTEX_NATURAL_EDT, m_editNatural);
	DDX_Control(pDX, IDC_CMD_VORTEX_MASS_EDT, m_editMass);
	DDX_Control(pDX, IDC_CMD_VORTEX_DAMPING_EDT, m_editDamping);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindPressureVelo_KDS2021, CChildDialog)
//{{AFX_MSG_MAP(CCMWindPressureVelo_KDS2021)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD1, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_METHOD_RD2, OnCmdMethodRadio)
	ON_BN_CLICKED(IDC_CMD_VORTEX_CHECK, OnCmdVortexSheddingBtn)
	ON_BN_CLICKED(IDC_CMD_METHOD_GENERAL_BTN, OnCmdGeneralBtn)
	ON_BN_CLICKED(IDC_CMD_TOPO_EFFECT_CHECK, OnCmdTopoEffectCheck)
	ON_EN_CHANGE(IDC_CMD_SPEED_EDIT							, OnChangeSpeedEdit)
	ON_EN_CHANGE(IDC_CMD_SIMPLE_SPEED_EDIT			, OnChangeSimpleSpeedEdit)
	ON_EN_CHANGE(IDC_CMD_ROOF_HEIGHT_EDIT				, OnChangeRoofHeightEdit)
	ON_EN_CHANGE(IDC_CMD_SIMPLE_ROOF_HEIGHT_EDIT, OnChangeSimpleRoofHeightEdit)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_KDS2021 message handlers

BOOL CCMWindPressureVelo_KDS2021::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	AlignControls();

// 	m_unitSpeed2.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);
	m_editSpeed2.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);
// 	m_unitSpeed.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);

	m_unitRoofHeight2.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight2.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_unitRoofHeight.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	
	m_editKZT.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dKzt);
	m_unitVKZT.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dVzt);
	m_editVKZT.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dVzt);
	
	m_unitRoofHeight3.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight3.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_unitDiaDM.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editDiaDM.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_unitDiaDB.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editDiaDB.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);

	m_unitMass.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KDS2021_dMass);
	m_editMass.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KDS2021_dMass);

	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	
	InitCategoryCombo();
	InitImportFactorCombo();
	InitCeFactorCombo();
	OnCmdTopoEffectCheck();
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindPressureVelo_KDS2021::DestroyWindow() 
{
	Dlg2Data();
	
	return CChildDialog::DestroyWindow();
}

void CCMWindPressureVelo_KDS2021::OnCmdMethodRadio()
{
	int nMethod=1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);

	int nCheck = m_chkVortex.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, IDC_CMD_VORTEX_CHECK, nMethod == 1);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSimple, nMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlGeneral, nMethod == 1 && nCheck == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlVortex, nMethod == 1 && nCheck == 1);
}

void CCMWindPressureVelo_KDS2021::OnCmdVortexSheddingBtn()
{
	OnCmdMethodRadio();
}
void CCMWindPressureVelo_KDS2021::OnCmdGeneralBtn() 
{
	CWindClassifyKBC2015Dlg dlg(FALSE);
	dlg.SetInitPos(D_INIT_POS_RT);

	int nCheck = m_chkVortex.GetCheck();

	int nMethod=1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, nMethod);
		
	if (nMethod == 0) dlg.m_dHeight = m_editRoofHeight2.GetEditValue();
	else
	{
		if (nCheck == 0) dlg.m_dHeight = m_editRoofHeight.GetEditValue();
		else dlg.m_dHeight = m_editRoofHeight3.GetEditValue();
	}
	dlg.m_dWidth			= m_dClassifyBY;
	dlg.m_dBreadth		= m_dClassifyBX;
	dlg.m_dArea				= m_dAreadKBC2015;
	dlg.SetWindLoadCode(KDS_W_2022);
	dlg.DoModal();
}

void CCMWindPressureVelo_KDS2021::OnCmdTopoEffectCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopoEffect.GetCheck();
	m_editKZT.EnableWindow(nCheck == 1);
	m_editVKZT.EnableWindow(nCheck == 1);
}

void CCMWindPressureVelo_KDS2021::OnChangeSpeedEdit()
{
	double dValue = m_editSpeed.GetEditValue();
	m_editSpeed2.SetEditUnit(dValue);
}

void CCMWindPressureVelo_KDS2021::OnChangeSimpleSpeedEdit()
{
	double dValue = m_editSpeed2.GetEditValue();
	m_editSpeed.SetEditUnit(dValue);
}

void CCMWindPressureVelo_KDS2021::OnChangeRoofHeightEdit()
{
	double dValue = m_editRoofHeight.GetEditValue();
	m_editRoofHeight2.SetEditUnit(dValue);
}

void CCMWindPressureVelo_KDS2021::OnChangeSimpleRoofHeightEdit()
{
	double dValue = m_editRoofHeight2.GetEditValue();
	m_editRoofHeight.SetEditUnit(dValue);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindPressureVelo_KDS2021::InitDefaultData()
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
	double dMinBx = 0.0;
	double dMinBy = 0.0;

	double dDmLevel = 2 * StorD.dStoryLevel / 3.;
	double dMaxUp4Dm = 0.0;	//Dm => Diameter of 2H/3 Level
	double dMaxDn4Dm = 0.0;

	double dMaxUp4DB = 0.0;	//DB=> Diameter of Bottom
	double dMaxDn4DB = 0.0;
	if (nStorNum > 0)
	{
		BOOL bFirst = TRUE, bFirstDmLevel = TRUE;
		for (int i = 0; i < nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);

			// MQC 15282 : Min.Breadth 값을 가져올 때 Story Data에서 Ground Level 아래쪽 Level의 값을 제외하고 
			// 최소값을 가지고 올 수 있도록 수정부탁드립니다. 
			if (StorD2.dStoryLevel < dBaseLevel - (1.e-10))
			{
				dMaxDn4DB = max(dMaxDn4DB, max(StorD2.WindWidthX, StorD2.WindWidthY));
				continue;
			}

			if (StorD2.dStoryLevel < dDmLevel - (1.e-10))
			{
				dMaxDn4Dm = max(dMaxDn4Dm, max(StorD2.WindWidthX, StorD2.WindWidthY));
			}
			else
			{
				if (bFirstDmLevel)
				{
					dMaxUp4Dm = max(StorD2.WindWidthX, StorD2.WindWidthY);
					bFirstDmLevel = FALSE;
				}
			}
			if (bFirst)
			{
				dMinBx = StorD2.WindWidthX;
				dMinBy = StorD2.WindWidthY;
				bFirst = FALSE;

				dMaxUp4DB = max(StorD2.WindWidthX, StorD2.WindWidthY);
				continue;
			}
			if (dMinBx > StorD2.WindWidthX) dMinBx = StorD2.WindWidthX;
			if (dMinBy > StorD2.WindWidthY) dMinBy = StorD2.WindWidthY;
		}
		for (int i = 0; i < nStorNum; i++)
		{
			pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD2);
			if (StorD2.dStoryLevel > dBaseLevel) break;
		}
		if (StorD2.dStoryLevel > dBaseLevel)
		{
			dBx = StorD2.WindWidthX;
			dBy = StorD2.WindWidthY;
		}
		else
		{
			dBx = 0.0; dBy = 0.0;
		}
	}
	m_Data.dVortexDM = max(dMaxDn4Dm, dMaxUp4Dm);
	m_Data.dVortexDB = max(dMaxUp4DB, dMaxDn4DB);
	// 초기화
	m_nClassifyStructure = 0;
	m_nClassifyPlane = 0;
	m_dClassifyBX = dBx;
	m_dClassifyBY = dBy;
	m_dAreadKBC2015 = dBx*dBy;
}

void CCMWindPressureVelo_KDS2021::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	int nIndex;
	CString aItem[] = {_T("A"), _T("B"), _T("C"), _T("D")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		nIndex = m_cobxCategory.AddString(aItem[i]);
		m_cobxCategory.SetItemData(nIndex , i+1);
	}
	m_cobxCategory.SetCurSel(0);
}

void CCMWindPressureVelo_KDS2021::InitImportFactorCombo()
{
	m_cobxImportFactor.ResetContent();
	
	CString aItem[] = {_T("0.9"), _T("0.95"), _T("1.0"), _T("1.05")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxImportFactor.AddString(aItem[i]);
	m_cobxImportFactor.SetCurSel(0); 
}

void CCMWindPressureVelo_KDS2021::InitCeFactorCombo()
{
	m_cobxCeFactor.ResetContent();

	CString aItem[] = {_T("1.0"), _T("1.5")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cobxCeFactor.AddString(aItem[i]);
	m_cobxCeFactor.SetCurSel(0); 
}

void CCMWindPressureVelo_KDS2021::AlignControls()
{
	CRect ToRect, FromRect;
	
	GetDlgItem(IDC_CMD_SIMPLE_SPEED_TITLE)->GetWindowRect(FromRect);
	GetDlgItem(IDC_CMD_KOR_EC_TITLE)->GetWindowRect(ToRect);
	int nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSimple, nDistY);

	// 와류진동 풍하중 ctrl들을 위로 올림
	GetDlgItem(IDC_CMD_VORTEX_ROOF_HEIGHT_STC)->GetWindowRect(FromRect);
	nDistY = ToRect.top - FromRect.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlVortex, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_FRAME);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+4;
	MoveWindow(r);
}

void CCMWindPressureVelo_KDS2021::Data2Dlg()
{
	CString csVal;

	m_chkVortex.SetCheck(m_Data.bVortexShedding);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nMethod);
	OnCmdMethodRadio();

	if (m_Data.nMethod == 0)
	{
		m_Data.dBasicWindSpeed = m_Data.dSimpleBasicWindSpeed;
		m_Data.dRoofHeight = m_Data.dSimpleRoofHeight;
	}
	else
	{
		if (m_Data.bVortexShedding)
		{
			m_Data.dSimpleRoofHeight = m_Data.dVortexAverageRoof;
			m_Data.dRoofHeight = m_Data.dVortexAverageRoof;
		}
		else
		{
			m_Data.dSimpleBasicWindSpeed = m_Data.dBasicWindSpeed;
			m_Data.dSimpleRoofHeight = m_Data.dRoofHeight;
			m_Data.dVortexAverageRoof = m_Data.dRoofHeight;
		}
	}

	//////////////////////////////////////////
	// 간편법
	m_editSpeed2.SetEditUnit(m_Data.dSimpleBasicWindSpeed);
	m_editRoofHeight2.SetEditUnit(m_Data.dSimpleRoofHeight);
	if (m_Data.dSimpleCe == 1.0  || m_Data.dSimpleCe == 1.5 )
	{
		csVal.Format(_T("%g"), m_Data.dSimpleCe);
		m_cobxCeFactor.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dSimpleCe);
		m_cobxCeFactor.SetWindowText(csVal);
	}

	//////////////////////////////////////////
	// 일반법
	m_cobxCategory.SetCurSel(m_Data.nExposureCategory-1);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	if (m_Data.dImportanceFactor == 0.9  || m_Data.dImportanceFactor == 0.95  ||
		  m_Data.dImportanceFactor == 1.0  || m_Data.dImportanceFactor == 1.1   || m_Data.dImportanceFactor == 1.13)
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SelectString(-1, csVal);
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dImportanceFactor);
		m_cobxImportFactor.SetWindowText(csVal);
	}
	
	m_editRoofHeight.SetEditUnit(m_Data.dRoofHeight);
	m_chkTopoEffect.SetCheck(m_Data.bTopographicEffect);
	OnCmdTopoEffectCheck();
	m_editKZT.SetEditUnit(m_Data.dKzt);
	m_editVKZT.SetEditUnit(m_Data.dVzt);

	m_editRoofHeight3.SetEditUnit(m_Data.dVortexAverageRoof);
	m_editDiaDM.SetEditUnit(m_Data.dVortexDM);
	m_editDiaDB.SetEditUnit(m_Data.dVortexDB);
	m_editNatural.SetEditUnit(m_Data.dVortexN);
	m_editMass.SetEditUnit(m_Data.dVortexMass);
	m_editDamping.SetEditUnit(m_Data.dVortexDamp);

}

BOOL CCMWindPressureVelo_KDS2021::Dlg2Data()
{
	CString csVal;

	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nMethod);
	m_Data.bVortexShedding = m_chkVortex.GetCheck() == 1 ? TRUE : FALSE;

	if(m_Data.nMethod==0)
	{
		m_Data.dBasicWindSpeed = m_Data.dSimpleBasicWindSpeed;
		m_Data.dRoofHeight = m_Data.dSimpleRoofHeight;
	}
	else
	{
		m_Data.dSimpleBasicWindSpeed = m_Data.dBasicWindSpeed;
		m_Data.dSimpleRoofHeight = m_Data.dRoofHeight;
	}

	if(m_Data.nMethod == 0)
	{
		//////////////////////////////////////////
		// 간편법
		m_Data.dSimpleBasicWindSpeed = m_editSpeed2.GetEditValue();
		m_Data.dSimpleRoofHeight = m_editRoofHeight2.GetEditValue();
		m_cobxCeFactor.GetWindowText(csVal);
		m_Data.dSimpleCe = _tstof(csVal);
	}
	else
	{
		if (m_Data.bVortexShedding)
		{
			m_Data.dVortexAverageRoof = m_editRoofHeight3.GetEditValue();
			m_Data.dVortexDM = m_editDiaDM.GetEditValue();
			m_Data.dVortexDB = m_editDiaDB.GetEditValue();
			m_Data.dVortexN = m_editNatural.GetEditValue();
			m_Data.dVortexMass = m_editMass.GetEditValue();
			m_Data.dVortexDamp = m_editDamping.GetEditValue();
		}
		else
		{
			//////////////////////////////////////////
			// 일반법
			m_Data.nExposureCategory = m_cobxCategory.GetCurSel() + 1;
			m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();

			m_cobxImportFactor.GetWindowText(csVal);
			m_Data.dImportanceFactor = _tstof(csVal);

			m_Data.dRoofHeight = m_editRoofHeight.GetEditValue();
			m_Data.bTopographicEffect = m_chkTopoEffect.GetCheck();
			if (m_Data.bTopographicEffect)
			{
				m_Data.dKzt = m_editKZT.GetEditValue();
				m_Data.dVzt = m_editVKZT.GetEditValue();
			}
		}
	}
	
	return TRUE;
}