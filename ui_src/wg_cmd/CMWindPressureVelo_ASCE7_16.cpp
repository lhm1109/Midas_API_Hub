// CMWindPressureVelo_ASCE7_16.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemDlg.h"
#include "CMWindPressureVelo_ASCE7_16.h"
#include "CMWindPressureVelo_ASCE7_16_Topography.h"
#include "WindAutoCalcDlg.h"
#include "CMWindPressureVelocityAddMod.h"

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
// CCMWindPressureVelo_ASCE7_16 dialog


CCMWindPressureVelo_ASCE7_16::CCMWindPressureVelo_ASCE7_16(CWnd* pParent /*=NULL*/)
: CChildDialog(CCMWindPressureVelo_ASCE7_16::IDD, pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindPressureVelocityAddMod*)pParent;
	//{{AFX_DATA_INIT(CCMWindPressureVelo_ASCE7_16)
	//}}AFX_DATA_INIT

	m_bInit = FALSE;
	m_Data.Initialize();
}

void CCMWindPressureVelo_ASCE7_16::SetData2Dlg(T_WVEP_ASCE7_16& data)
{
	m_Data = data;
	Data2Dlg();
}

BOOL CCMWindPressureVelo_ASCE7_16::SetDlg2Data(T_WVEP_ASCE7_16& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindPressureVelo_ASCE7_16::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindPressureVelo_ASCE7_16)
	DDX_Control(pDX, IDC_CMD_TOPO_EFFECT_CHECK, m_chkTopoEffect);
	DDX_Control(pDX, IDC_CMD_SPEED_EDIT, m_editSpeed);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_UNIT, m_unitRoofHeight);
	DDX_Control(pDX, IDC_CMD_ROOF_HEIGHT_EDIT, m_editRoofHeight);
	DDX_Control(pDX, IDC_CMD_KZT_EDIT, m_editKZT);
	DDX_Control(pDX, IDC_CMD_CATEGORY_COMBO, m_cobxCategory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindPressureVelo_ASCE7_16, CChildDialog)
//{{AFX_MSG_MAP(CCMWindPressureVelo_ASCE7_16)
	ON_BN_CLICKED(IDC_CMD_AUTO_CALC_BTN, OnCmdAutoCalcBtn)
	ON_BN_CLICKED(IDC_CMD_TOPO_EFFECT_CHECK, OnCmdTopoEffectCheck)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureVelo_ASCE7_16 message handlers

BOOL CCMWindPressureVelo_ASCE7_16::OnInitDialog() 
{
	CChildDialog::OnInitDialog();


// 	m_unitSpeed2.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);
// 	m_unitSpeed.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);
	m_editSpeed.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dBasicWindSpeed);

	m_unitRoofHeight.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	m_editRoofHeight.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dRoofHeight);
	
	m_editKZT.SetUnitType(CUnitCtrl::m_WVEP_UNIT.KBC2009_dKzt);


	if (!m_bInit)
	{
		InitDefaultData();
		m_bInit = TRUE;
	}
	
	InitCategoryCombo();
	OnCmdTopoEffectCheck();
	
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CCMWindPressureVelo_ASCE7_16::DestroyWindow() 
{
	Dlg2Data();
	
	return CChildDialog::DestroyWindow();
}

void CCMWindPressureVelo_ASCE7_16::OnCmdAutoCalcBtn() 
{
	const int nCode = m_pParent->m_Data.nCodeType;

	CCMWindPressureVelo_ASCE7_16Topography dlg(nCode);
	dlg.SetInitPos(D_INIT_POS_RT);

	if (dlg.DoModal() == IDOK)
	{
		m_editKZT.SetEditUnit(dlg.GetKzt());
	}
}

void CCMWindPressureVelo_ASCE7_16::OnCmdTopoEffectCheck() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopoEffect.GetCheck();
	
	m_editKZT.EnableWindow(nCheck == 1);
	GetDlgItem(IDC_CMD_AUTO_CALC_BTN)->EnableWindow(nCheck == 1);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMWindPressureVelo_ASCE7_16::InitDefaultData()
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
	// 초기화
	m_nClassifyStructure = 0;
	m_nClassifyPlane = 0;
	m_dClassifyBX = dBx;
	m_dClassifyBY = dBy;
	m_dAreadKBC2015 = dBx*dBy;
}

void CCMWindPressureVelo_ASCE7_16::InitCategoryCombo()
{
	m_cobxCategory.ResetContent();
	
	CString aItem[] = { _T("B"), _T("C"), _T("D") };
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		CDlgUtil::CobxAddItem(m_cobxCategory, aItem[i], EN_ASCE7_EXPO_B + i);
	}
	m_cobxCategory.SetCurSel(0);
}


void CCMWindPressureVelo_ASCE7_16::Data2Dlg()
{
	CDlgUtil::CobxSetCurSelItemData(m_cobxCategory, m_Data.nExposureCategory);
	m_editSpeed.SetEditUnit(m_Data.dBasicWindSpeed);
	m_editRoofHeight.SetEditUnit(m_Data.dMeanRoofHeight);
	m_chkTopoEffect.SetCheck(m_Data.bTopographicEffect);
	OnCmdTopoEffectCheck();
	m_editKZT.SetEditUnit(m_Data.dKzt);
		
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
	}(m_pParent->m_Data.nCodeType);
	
	GetDlgItem(IDC_CMD_SPEED_UNIT)->SetWindowText(strUnitSpeed);
}

BOOL CCMWindPressureVelo_ASCE7_16::Dlg2Data()
{
	m_Data.Initialize();

	m_Data.nExposureCategory = CDlgUtil::CobxGetCurSelItemData(m_cobxCategory, m_cobxCategory.GetCurSel());
	m_Data.dBasicWindSpeed = m_editSpeed.GetEditValue();

	m_Data.dMeanRoofHeight = m_editRoofHeight.GetEditValue();
	m_Data.bTopographicEffect = m_chkTopoEffect.GetCheck();
	if (m_Data.bTopographicEffect)
	{
		m_Data.dKzt = m_editKZT.GetEditValue();
	}
	
	return TRUE;
}