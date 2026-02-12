#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldEarthPressureStaticDlg.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\ExportFunc.h"
#include "..\wg_cmd\CMLoadGroupDlg.h"
#include "..\wg_cmd\CMLdarGrupDefDlg.h"
#include "..\wg_cmd\SoilPropListDlg.h"
#include "..\wg_cmd\CMStaticEarthPressureProfileDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\ClosedPathFinder.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_gr\GDisplaySetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldEarthPressureStaticDlg dialog

#define ELEM_FRAME  1
#define ELEM_PLANAR 2

CStldEarthPressureStaticDlg::CStldEarthPressureStaticDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldEarthPressureStaticDlg::IDD)
{
// 	m_pMgr = NULL;
	m_Data.Initialize();
	m_LagrK = 0;


	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitEpstOut(m_Data);

	m_nSelectionType = SELECT_GRUP;
	m_nEarthPressureType = 0;
	m_bInitProfile = FALSE;

	m_aCtrlOption.RemoveAll();
	m_aCtrlOption.Add(IDC_TM_EP_TYPE_ADD_RDO);
	m_aCtrlOption.Add(IDC_TM_EP_TYPE_DEL_RDO);

	m_aCtrlAddDelete.RemoveAll();
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC0);
	m_aCtrlAddDelete.Add(IDC_TM_DIRECTION_COMBO);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC1);
	m_aCtrlAddDelete.Add(IDC_TM_ANGLE_EDT);
	m_aCtrlAddDelete.Add(IDC_TM_ANGLE_SPIN);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC2);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC3);
	m_aCtrlAddDelete.Add(IDC_TM_PLANE_PT1_EDIT);
	m_aCtrlAddDelete.Add(IDC_TM_PLANE_PT1_UNIT);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_STATIC4);
	m_aCtrlAddDelete.Add(IDC_TM_SCALEFACTOR);
	m_aCtrlAddDelete.Add(IDC_TM_EP_TYPE_GRP);
	m_aCtrlAddDelete.Add(IDC_TM_EP_TYPE_AT_REST_RDO);
	m_aCtrlAddDelete.Add(IDC_TM_EP_TYPE_ACTIVE_RDO);
	m_aCtrlAddDelete.Add(IDC_TM_EP_TYPE_PARAM_GRP);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_SURCHARGE_STC);
	m_aCtrlAddDelete.Add(IDC_TM_SURCHARGE_EDIT);
	m_aCtrlAddDelete.Add(IDC_TM_SURCHARGE_UNIT);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_WATER_STC);
	m_aCtrlAddDelete.Add(IDC_TM_WATER_EDIT);
	m_aCtrlAddDelete.Add(IDC_TM_WATER_UNIT);
	m_aCtrlAddDelete.Add(IDC_WG_TREEMENU_POSP_STC);
	m_aCtrlAddDelete.Add(IDC_TM_POSP_NAME_CMB);
	m_aCtrlAddDelete.Add(IDC_TM_POSP_BTN);
}

CStldEarthPressureStaticDlg::~CStldEarthPressureStaticDlg()
{
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.nTypeLaplLagr = 0;
	dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	ExportDisplaySetting::SET(&dspOpt);
}

void CStldEarthPressureStaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TM_LOADCASE, m_cobxLoadCase);
	DDX_Control(pDX, IDC_TM_DIRECTION_COMBO, m_cobxDirection);
	DDX_Control(pDX, IDC_TM_POSP_NAME_CMB, m_cobxSoilProp);
	DDX_Control(pDX, IDC_TM_LAGR_NAME, m_cobxLagr);
	DDX_Control(pDX, IDC_TM_ELEM_DEFINE, m_edElemList);
	DDX_Control(pDX, IDC_TM_ANGLE_EDT, m_edtAngle);
	DDX_Control(pDX, IDC_TM_SCALEFACTOR, m_edtScaleFactor);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_EDIT, m_wndCoordXYZ);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_UNIT, m_wndCoordUnit);
	DDX_Radio(pDX, IDC_TM_EP_TYPE_AT_REST_RDO, m_nEarthPressureType);
	DDX_Control(pDX, IDC_TM_SURCHARGE_EDIT, m_edtSurchargeLoad);
	DDX_Control(pDX, IDC_TM_SURCHARGE_UNIT, m_SurchargeLoadUnit);
	DDX_Control(pDX, IDC_TM_WATER_EDIT, m_edtWaterLevel);
	DDX_Control(pDX, IDC_TM_WATER_UNIT, m_WaterLevelUnit);
}

BEGIN_MESSAGE_MAP(CStldEarthPressureStaticDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_LOADCASE_BTN, OnCmdBtnLoadCase)
	ON_BN_CLICKED(IDC_TM_TARGET_RDO1, OnCmdTargetChanged)
	ON_BN_CLICKED(IDC_TM_TARGET_RDO2, OnCmdTargetChanged)
	ON_BN_CLICKED(IDC_TM_BTN_PROFILE, OnCmdPressureProfile)
	ON_BN_CLICKED(IDC_TM_LAGR_BTN, OnCmdBtnLoadAreaGroup)
	ON_BN_CLICKED(IDC_TM_POSP_BTN, OnCmdSoilPropBtn)
	ON_BN_CLICKED(IDC_TM_EP_TYPE_ADD_RDO, OnCmdOptionRdo)
	ON_BN_CLICKED(IDC_TM_EP_TYPE_DEL_RDO, OnCmdOptionRdo)
	ON_CBN_SELCHANGE(IDC_TM_LAGR_NAME, OnSelChangeLagr)
	ON_CBN_SELCHANGE(IDC_TM_DIRECTION_COMBO, OnSelChangeDir)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ANGLE_SPIN, OnDeltaposCmdAngle)     //Span Num
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldEarthPressureStaticDlg message handlers

void CStldEarthPressureStaticDlg::OnTmExecute()
{
	if (!Dlg2Data())
		return;

	int nOption = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	CArray<T_EPST_D, T_EPST_D&> DataSet;
	if (GetEpstAry(DataSet))
	{
		if (!CheckEpst(DataSet, TRUE))
			return;

		CArray<T_EPST_K, T_EPST_K> aEpstK;
		if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetEpstKeyListByData(DataSet, aEpstK) == 0) { ASSERT(0); return; }

		BOOL bResult = FALSE;
		if (nOption == 0) // Add/replace
		{
			bResult = CDBDoc::GetDocPoint()->m_pDataCtrl->AddEpst(aEpstK, DataSet);
		}
		else
		{
			for (int i = aEpstK.GetSize() - 1; i > -1; i--)
			{
				if (aEpstK[i] == 0) aEpstK.RemoveAt(i);
			}
			bResult = CDBDoc::GetDocPoint()->m_pDataCtrl->DelEpst(aEpstK);
		}

		if (bResult) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
	IUsageCounter::Use(_T("EARTH_P"), FALSE);
}

void CStldEarthPressureStaticDlg::OnTmClose()
{
	CloseDlg();
}


void CStldEarthPressureStaticDlg::OnCmdOptionRdo()
{
	int nOption = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlAddDelete, nOption==0);
	if (nOption == 0) // Add Mode
	{
		OnCmdTargetChanged();
		OnSelChangeDir();
		OnSelChangeLagr();
	}
}

BOOL CStldEarthPressureStaticDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);

	InitCobxDirection();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, 0);

	m_edElemList.Connect(SC_ID_ELEM, &m_edElemList);
	m_cobxLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_edtAngle.SetUnitType(D_UNITSYS_BASE_DEGREE);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	m_wndCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCoordXYZ.SetAttUcsPos();
	m_wndCoordXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_wndCoordXYZ.SetNextLink(NULL);

	m_SurchargeLoadUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtSurchargeLoad.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_WaterLevelUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtWaterLevel.SetUnitType(D_UNITSYS_BASE_LENGTH);

	GetElementType();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStldEarthPressureStaticDlg::GetHolderRect(CRect* pRect)
{
	return FALSE;
}

void CStldEarthPressureStaticDlg::GetMoveCtrlHolderID(CArray<UINT, UINT>& caCtrls, UINT& uiHolderID)
{
	caCtrls.RemoveAll();
}

void CStldEarthPressureStaticDlg::OnCmdBtnLoadAreaGroup()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLdarGrupDefDlg::IDD);
}

void CStldEarthPressureStaticDlg::OnCmdBtnLoadCase()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE, 0));
}

void CStldEarthPressureStaticDlg::InitCobxDirection()
{
	m_cobxDirection.ResetContent();

	int nIndex;
	CStringArray strItem;
	strItem.Add(_T("X-Y"));
	strItem.Add(_T("Normal"));
	int nNum = strItem.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		nIndex = m_cobxDirection.AddString(strItem[i]);
		m_cobxDirection.SetItemData(nIndex, i + 1);
	}
	m_cobxDirection.SetCurSel(0);
}

void CStldEarthPressureStaticDlg::OnDeltaposCmdAngle(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_TM_ANGLE_EDT, -180, 180, pNMHDR, pResult);

	*pResult = 0;
}

void CStldEarthPressureStaticDlg::OnCmdTargetChanged()
{
	m_nSelectionType = GetSelectionType();
	if (m_nSelectionType == SELECT_GRUP) //Group
	{
		GetDlgItem(IDC_TM_LAGR_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAGR_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->EnableWindow(FALSE);
	}
	else if (m_nSelectionType == SELECT_ELEM) //Elements
	{
		GetDlgItem(IDC_TM_LAGR_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAGR_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->EnableWindow(TRUE);
	}
	else ASSERT(0);

	OnSelChangeLagr();
}

void CStldEarthPressureStaticDlg::OnCmdPressureProfile()
{
	if (!Dlg2Data())
		return;

	double dHeight = 0.0;
	BOOL bRetVal = CWindLoadGeneratorCtrl::GetModelHeight4EarthPressure(m_Data.KeyPosp, dHeight);
	if (dHeight <= 1E-5 || bRetVal == FALSE)
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_NO_NODEINFO));
		return;
	}

	CCMStaticEarthPressureProfileDlg dlg;
	dlg.InitProfile(m_Data, m_bInitProfile);
	dlg.DoModal();

	m_bInitProfile = FALSE;
}

int CStldEarthPressureStaticDlg::GetSelectionType()
{
	int nSelType = SELECT_GRUP;
	if (((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
	{
		nSelType = SELECT_ELEM;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
	}
	return nSelType;
}

BOOL CStldEarthPressureStaticDlg::Data2Dlg(BOOL bWarning)
{
	if (m_Data.KeyStld)
		m_cobxLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Data.KeyStld);

	if (m_Data.nDirection == 2) m_cobxDirection.SetCurSel(1);
	else                       m_cobxDirection.SetCurSel(0);

	CString strPos;
	strPos.Format(_T("%g, %g, %g"), m_Data.dInnerPnt[0], m_Data.dInnerPnt[1], m_Data.dInnerPnt[2]);
	m_wndCoordXYZ.SetWindowText(strPos);

	m_nEarthPressureType = m_Data.nEarthPressureType;
	m_edtAngle.SetEditUnit(m_Data.dAngle);
	m_edtScaleFactor.SetEditUnit(m_Data.dScaleFactor);
	if (m_Data.KeyPosp > 0)
		m_cobxSoilProp.ChangeSelect(m_Data.KeyPosp);

	m_edtSurchargeLoad.SetEditUnit(m_Data.dSurchargeLoad);
	double dBaseLevel = 0.;
	T_BLDC_D Bldc;
	if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBldc(Bldc))
		dBaseLevel = Bldc.dBaseLevel;

	m_edtWaterLevel.SetEditUnit(dBaseLevel);

	if (m_Data.nSelection == SELECT_GRUP)
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
		m_cobxLagr.ChangeSelect(m_LagrK);
	}
	else if (m_Data.nSelection == SELECT_ELEM)
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(TRUE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem(NULL, m_Data.arKeysDefineElem, FALSE);
	}

	//Water Level/Param of Soil Prop이 변경되면 Profile 데이터가 초기화 되어야함.
	m_dScaleFactor = m_Data.dScaleFactor;
	m_dSurchargeLoad = m_Data.dSurchargeLoad;
	m_dWaterLevel = dBaseLevel;
	m_PospK = m_Data.KeyPosp;
	m_bInitProfile = TRUE;

	OnCmdTargetChanged();
	OnSelChangeDir();
	OnSelChangeLagr();

	UpdateData(FALSE);
	return TRUE;
}

BOOL CStldEarthPressureStaticDlg::Dlg2Data(BOOL bWarning)
{
	int nOldmEarthPressureType = m_nEarthPressureType;
	UpdateData();
	unsigned int nLoadCaseType;
	m_cobxLoadCase.GetSelectedLoad(nLoadCaseType, m_Data.KeyStld);
	if (nLoadCaseType != D_LOADCASE_STATIC || m_Data.KeyStld <= 0)
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_NO_SELECTED_STLD));
		return FALSE;
	}

	CString strPos;
	MyParser Parser;

	m_Data.nDirection = GetDirection();
	if (m_Data.nDirection == 1)
		m_Data.dAngle = m_edtAngle.GetEditValue();
	else if (m_Data.nDirection == 2)
	{
		m_wndCoordXYZ.GetWindowText(strPos);
		if (!Parser.ParsingPositionArg(strPos)) return FALSE;
		if (!Parser.GetPositionData(m_Data.dInnerPnt[0], m_Data.dInnerPnt[1], m_Data.dInnerPnt[2])) return FALSE;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
	m_Data.nEarthPressureType = m_nEarthPressureType;

	m_cobxSoilProp.GetSelectedPosp(m_Data.KeyPosp);
	if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->ExistPosp(m_Data.KeyPosp))
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_NO_SELECTED_POSP));
		return FALSE;
	}

	double dSurchargeLoad = m_edtSurchargeLoad.GetEditValue();
	if (dSurchargeLoad < 0.0)
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_SURCHARGE_LOAD_INVALID_SIZE));
		return FALSE;
	}
	m_Data.dSurchargeLoad = m_edtSurchargeLoad.GetEditValue();
	m_Data.dWaterLevel = m_edtWaterLevel.GetEditValue();

	m_Data.dScaleFactor = m_edtScaleFactor.GetEditValue();
	m_cobxSoilProp.GetSelectedPosp(m_Data.KeyPosp);

	if (((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->GetCheck())
	{
		m_Data.nSelection = SELECT_GRUP;
		m_Data.arKeysDefineElem.RemoveAll();
		m_cobxLagr.GetSelectedLagr(m_LagrK);
	}
	else if (((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
	{
		m_Data.nSelection = SELECT_ELEM;
		m_LagrK = 0;
		// [PMS 5011-6] 요소 많이 선택 시 문자열->요소번호 파싱하다 프로그램 다운
		CArray<T_ELEM_K, T_ELEM_K> aElem;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pViewCtrl->GetAllSelectedElem(aElem);
		if (!aElem.IsEmpty())
		{
			int i, nSize;
			nSize = aElem.GetSize();
			m_Data.arKeysDefineElem.SetSize(nSize);
			for(i=0; i < nSize; i++)
			{
				m_Data.arKeysDefineElem[i] = aElem[i];
			}
		}
	}

	//Water Level/Param of Soil Prop이 변경되면 Profile 데이터가 초기화 되어야함.
	if (m_dWaterLevel != m_Data.dWaterLevel		|| m_PospK != m_Data.KeyPosp ||
		m_dScaleFactor != m_Data.dScaleFactor	|| m_dSurchargeLoad != m_Data.dSurchargeLoad ||
		nOldmEarthPressureType != m_nEarthPressureType)
	{
		m_dScaleFactor = m_Data.dScaleFactor;
		m_dSurchargeLoad = m_Data.dSurchargeLoad;
		m_dWaterLevel = m_Data.dWaterLevel;
		m_PospK = m_Data.KeyPosp;
		m_bInitProfile = TRUE;
	}

	return TRUE;
}

void CStldEarthPressureStaticDlg::OnSelChangeLagr()
{
	GDisplaySetting dspOpt;
	m_nSelectionType = GetSelectionType();
	if (m_nSelectionType == SELECT_GRUP) //Group
	{
		T_LAGR_K LagrK;
		m_cobxLagr.GetSelectedLagr(LagrK);
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 2;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		dspOpt.m_Load.m_LoadingAreaKeys.Add((unsigned int)LagrK);
		ExportDisplaySetting::SET(&dspOpt);
	}
	else if (m_nSelectionType == SELECT_ELEM) //Elements
	{
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 0;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		ExportDisplaySetting::SET(&dspOpt);
	}
}

void CStldEarthPressureStaticDlg::OnSelChangeDir()
{
	if (GetDirection() == 2) //Normal
	{
		GetDlgItem(IDC_TM_ANGLE_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_ANGLE_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_PLANE_PT1_EDIT)->EnableWindow(TRUE);
		m_wndCoordXYZ.SetFocus();
	}
	else //X-Y
	{
		GetDlgItem(IDC_TM_ANGLE_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_ANGLE_SPIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_PLANE_PT1_EDIT)->EnableWindow(FALSE);
	}
}

int CStldEarthPressureStaticDlg::GetDirection()
{
	int nSel = m_cobxDirection.GetCurSel();
	if (nSel == 0)
		return 1; //X-Y
	else if (nSel == 1)
		return 2; //Normal

	m_cobxDirection.SetCurSel(0);
	return 1;
}

void CStldEarthPressureStaticDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		OnUnitChange();
		break;
	default:
		break;
	}
}

void CStldEarthPressureStaticDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_POSP_K CurPospK = NULL;
	if(m_Data.KeyPosp !=0) m_cobxSoilProp.GetSelectedPosp(CurPospK);

	T_POSP_K PospK = NULL;
	T_POSP_D PospD;

	BOOL bPospChanged = FALSE;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_POSP_ADD):
			pViewBuff->GetPosp(nKey, PospK, PospD);
			if(PospK != CurPospK) continue;
			bPospChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if(bPospChanged)
	{
		m_bInitProfile = TRUE;
	}
}

void CStldEarthPressureStaticDlg::OnUnitChange()
{
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitEpstPrevious(m_Data);
}

BOOL CStldEarthPressureStaticDlg::CheckEpst(CArray<T_EPST_D, T_EPST_D&>& rDataSet, BOOL bMsg)
{
	if (m_Data.nSelection != SELECT_GRUP && GetElementType() == ELEM_PLANAR)
	{
		for (int i = 0; i < rDataSet.GetSize(); i++)
		{
			CMap<T_ELEM_K, T_ELEM_K, T_PRES_D, T_PRES_D&> mapLoad;
			if (!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcEarthPressure_Face(rDataSet[i], mapLoad, TRUE))
				return FALSE;
		}
	}
	else
	{
		CMap<T_NODE_K, T_NODE_K, T_CNLD_D, T_CNLD_D&> mapLoad;
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcEarthPressure_Nodal(rDataSet, mapLoad, NULL, TRUE))
			return FALSE;
	}

	return TRUE;
}

BOOL CStldEarthPressureStaticDlg::GetEpstAry(CArray<T_EPST_D, T_EPST_D&>& rDataSet)
{
	int nSelection = m_Data.nSelection;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_LAPL_K, T_LAPL_K> KeyLapl;
	int i, nSize;

	int nElemType = GetElementType();
	if (m_Data.nSelection == SELECT_GRUP)
	{
		CArray<T_LAGR_K, T_LAGR_K> KeyLagr;
		KeyLagr.SetSize(1);
		KeyLagr[0] = m_LagrK;
		pDoc->m_pAttrCtrl2->GetKeyListLaplByLagr(KeyLagr, KeyLapl);

		nSize = KeyLapl.GetSize();
		rDataSet.SetSize(nSize);
		for(int i=0; i < nSize; i++)
		{
			m_Data.KeyLapl = KeyLapl[i];
			rDataSet[i] = m_Data;
		}
		if (nSize <= 0)
		{
			AfxMessageBox(_LS(IDS_TM_EARTHP_NO_LDPL));
			return FALSE;
		}
	}
	else
	{
		if (nElemType == ELEM_PLANAR)
		{
			T_ELEM_D ElemD; ElemD.Initialize();
			for (int i = m_Data.arKeysDefineElem.GetSize() - 1; i >= 0; i--)
			{
				if (!pDoc->m_pAttrCtrl->GetElem(m_Data.arKeysDefineElem[i], ElemD)) { ASSERT(0); return FALSE; }
				if (!pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp) && !pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp)) m_Data.arKeysDefineElem.RemoveAt(i);
			}

			if (m_Data.arKeysDefineElem.GetSize() < 1) return FALSE;

			T_EPST_D EpstD;
			rDataSet.SetSize(m_Data.arKeysDefineElem.GetSize());
			for(int i=0; i < m_Data.arKeysDefineElem.GetSize(); i++)
			{
				EpstD = m_Data;
				EpstD.arKeysDefineElem.RemoveAll();
				EpstD.arKeysDefineElem.Add(m_Data.arKeysDefineElem[i]);
				rDataSet.SetAt(i, EpstD);
			}
		}
		else
		{
// 			if (TRUE)
// 			{
				CArray<T_CPF_CYCLE_PATH3, T_CPF_CYCLE_PATH3&> aClosedPath;
				if (!IClosedPathFinder::Find4AutoGen(m_Data.arKeysDefineElem, aClosedPath) || aClosedPath.GetSize() <= 0)
				{
					ASSERT(0);
					return FALSE;
				}

				nSize = aClosedPath.GetSize();
				rDataSet.SetSize(nSize);
				for(int i=0; i < nSize; i++)
				{
					rDataSet[i] = m_Data;
					rDataSet[i].arKeysDefineElem.Copy(aClosedPath[i].aElemId);
					rDataSet[i].arKeysDefineNode.Copy(aClosedPath[i].aNodeId);
				}
// 			}
// 			else
// 			{
// 				CWindLoadGeneratorCtrl myCtrl;
// 				if (!myCtrl.RunFindClosedArea(m_Data.arKeysDefineElem, TRUE))
// 					return FALSE;
// 
// 				CArray<UINT, UINT> arSurfKey;
// 				CArray<T_NODE_K, T_NODE_K> caNodeKey;
// 				CArray<T_ELEM_K, T_ELEM_K> caElemKey;
// 				nSize = myCtrl.GetSurfKeyList(arSurfKey);
// 
// 				rDataSet.SetSize(nSize);
// 				for(int i=0; i < nSize; i++)
// 				{
// 					rDataSet[i] = m_Data;
// 					if (!myCtrl.GetSurfInfo(arSurfKey[i], rDataSet[i].arKeysDefineNode, rDataSet[i].arKeysDefineElem))
// 					{
// 						ASSERT(0);
// 						return FALSE;
// 					}
// 				}
// 			}
		}
	}

	int nSizeEpstSet = rDataSet.GetSize();
	if (nSizeEpstSet <= 0) return FALSE;

	CLateralLoad laterLoad;
	for (i = nSizeEpstSet - 1; i >= 0; i--)
	{
		if (!laterLoad.MakeStaticEarthPressure(rDataSet[i], m_bInitProfile))
		{
			rDataSet.RemoveAt(i); //일단 실패하는 상황을 만들면 안되지만. 실패하면 지우자.
		}
	}

	return TRUE;
}

void CStldEarthPressureStaticDlg::OnCmdSoilPropBtn()
{
	CSoilPropListDlg dlg;
	dlg.DoModal();
}

int CStldEarthPressureStaticDlg::GetElementType()
{
	int nElemType = ELEM_FRAME;
	if (((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->GetCheck())
		nElemType = ELEM_FRAME;
	else if (((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->GetCheck())
		nElemType = ELEM_PLANAR;
	else
	{
		nElemType = ELEM_FRAME;
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->SetCheck(FALSE);
	}
	return nElemType;
}