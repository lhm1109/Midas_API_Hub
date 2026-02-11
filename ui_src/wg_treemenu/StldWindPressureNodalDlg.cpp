// CMGridImpactFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureNodalDlg.h"
#include "StldWindPressureNodalMgr.h"
#include "StldWindPressureNodalDlg_StructureType.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\CMLoadGroupDlg.h"
#include "..\wg_cmd\CMLdarGrupDefDlg.h"
#include "..\wg_cmd\CMWindPressureVelocityDefDlg.h"
#include "..\wg_cmd\CMWindPressureProfileDlg.h"
#include "..\wg_cmd\CMWindPressureProfileKBC2016Dlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
// #include "..\wg_base\StringParser.h"
// 
// #include "..\wg_base\Parser.h"
#include "..\wg_base\PL_Parser.h"

#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\MITC_CommonTool.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_common\TBUtilFunc.h"
#include "..\wg_gr\GDisplaySetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg dialog


CStldWindPressureNodalDlg::CStldWindPressureNodalDlg(CWnd* pParent /*=NULL*/)
	: CCMWindPressureBaseDlg(CStldWindPressureNodalDlg::IDD)
{
	m_pRootDlg = (CCMWindPressureRootDlg*)pParent;

	m_pMgr = NULL;
	m_pMgr = new CStldWindPressureNodalMgr(this, (void*)&m_Data);
	m_Data.Initialize();
	
	m_Data.nodalStruct.Initialize();
	m_Data.nodalStruct.dB1 = 6.0;
	m_Data.nodalStruct.dB2 = 4.5;
	m_Data.nodalStruct.dB3 = 3.0;
	m_Data.nodalStruct.dB4 = 2.5;
	m_Data.nodalStruct.dR1 = 2.7;
	m_Data.nodalStruct.dR2 = 1.5;
	m_Data.nodalStruct.dH  = 4.2;
	m_Data.nodalStruct.dAngleOfB1 = 0.0;

	m_bExecute = FALSE;
	m_LagrK = 0;
	
#ifdef _CH
	m_Data.CodeParam.CH2012.Initialize();
	m_Data.CodeParam.CH2001.Initialize();
	m_Data.CodeParam.CH2019.Initialize();
#endif

	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprOut(m_Data);

	m_nSelectionType = 1;
}

CStldWindPressureNodalDlg::~CStldWindPressureNodalDlg()
{
	if (m_pMgr != NULL)
	{
		delete m_pMgr;
		m_pMgr = NULL;
	}
	DrawPreview(FALSE);
}

void CStldWindPressureNodalDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TM_PLACE_HOLDER,    m_wndHolder);
	DDX_Control(pDX, IDC_TM_CODE_NAME,       m_cobxCodeName);
	DDX_Control(pDX, IDC_TM_VELOCITY_NAME,   m_cobxWvep);  
	DDX_Control(pDX, IDC_TM_LOADCASE,        m_cobxLoadCase);
	DDX_Control(pDX, IDC_TM_DIRECTION_COMBO, m_cobxDirection);
	DDX_Control(pDX, IDC_TM_ANGLE_EDT,       m_edtAngle);    
	DDX_Control(pDX, IDC_TM_SCALEFACTOR,     m_edtScaleFactor);
	DDX_Control(pDX, IDC_TM_NODE_DEFINE,     m_edNodeList);
	DDX_Control(pDX, IDC_TM_WINDP_DIRAREA,       m_edtWindArea);
	DDX_Control(pDX, IDC_TM_WINDP_DIRAREA_UNIT,  m_stcWindArea);  
	DDX_Control(pDX, IDC_TM_WINDP_CONCENPT,      m_wndConcenCoordXYZ);
	DDX_Control(pDX, IDC_TM_WINDP_CONCENPT_UNIT, m_wndConcenCoordUnit);
}


BEGIN_MESSAGE_MAP(CStldWindPressureNodalDlg, CCMChildBarBase)
	ON_BN_CLICKED(IDC_TM_VELOCITY_BTN,  OnVelocityBtn)  
	ON_BN_CLICKED(IDC_TM_LOADCASE_BTN,  OnCmdBtnLoadCase)
	ON_BN_CLICKED(IDC_TM_BTN_PROFILE,   OnCmdPressureProfile)  
	ON_BN_CLICKED(IDC_TM_WINDP_NODAL_DETAIL, OnCmdBtnStructure)  
	ON_CBN_SELCHANGE(IDC_TM_CODE_NAME,  OnSelchangeCmdCodeName)
	ON_CBN_SELCHANGE(IDC_TM_VELOCITY_NAME,   OnSelChangeVelocity)
	ON_EN_CHANGE(IDC_TM_ANGLE_EDT,	  	OnChange_ClearProfile)
	ON_EN_CHANGE(IDC_TM_WINDP_CONCENPT,	OnChange_ClearProfile)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ANGLE_SPIN, OnDeltaposCmdAngle)     //Span Num
	ON_BN_CLICKED(IDC_TM_AUTO_CALC_RD,  OnCmdRdoChange)  
	ON_BN_CLICKED(IDC_TM_USER_DEF_RD,   OnCmdRdoChange)  
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureNodalDlg message handlers

BOOL CStldWindPressureNodalDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
#ifdef _CH
	GetDlgItem(IDC_CHECK_CONVERT_TO_LOAD)->ShowWindow(SW_SHOW);
#endif
	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);

	m_pMgr->SetInit(TRUE);

	if(CWindLoadGeneratorCtrl::m_bSaved[WDPRTYPE_NODAL-1])
	{
	  m_Data = CWindLoadGeneratorCtrl::m_WdprLast[WDPRTYPE_NODAL-1];
	}

	InitCobxCodeName();
	InitCobxDirection();

	m_wndHolder.ShowWindow(SW_HIDE);
	m_edElemList.Connect(SC_ID_ELEM, &m_edElemList);	
	m_cobxLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_edtAngle.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	m_edtWindArea.SetUnitType(D_UNITSYS_BASE_AREA);
	m_stcWindArea.SetUnitType(D_UNITSYS_BASE_AREA);
	
	m_edNodeList.Connect(SC_ID_NODE, &m_edNodeList);	

	m_wndConcenCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndConcenCoordXYZ.SetAttUcsPos();
	m_wndConcenCoordXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);  
	m_wndConcenCoordXYZ.SetNextLink(NULL);

	int nCurSel = m_cobxCodeName.GetCurSel();
	int nCodeType = (int)m_cobxCodeName.GetItemData(nCurSel);
	if(m_Data.nCodeType == 0) m_Data.nCodeType = nCodeType;
	m_cobxWvep.SetCodeType(nCodeType);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStldWindPressureNodalDlg::GetHolderRect( CRect* pRect )
{
	if(m_wndHolder.GetSafeHwnd() && ::IsWindow(m_wndHolder.GetSafeHwnd()))
	{
		m_wndHolder.GetWindowRect(pRect);
		return TRUE;
	}
	return FALSE;
}

void CStldWindPressureNodalDlg::Execute()
{
	m_bExecute = TRUE;

	if (!Dlg2Data())
	{
		m_bExecute = FALSE;
		return;
	}
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pEditData->CheckWdprKBC2022KdValue(m_Data))
	{
		m_bExecute = FALSE;
		return;
	}
	m_bExecute = FALSE;

	CArray<T_WDPR_D, T_WDPR_D&> DataSet;
	if (GetWdprAry(DataSet))
	{
		if (!CheckWdpr(DataSet, TRUE))
			return;
		if (MITC_CommonTool::AddWdpr_Node(DataSet, CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD)))
		{
			CWindLoadGeneratorCtrl::SetWdprSave(WDPRTYPE_NODAL, m_Data);
			CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
			m_wndConcenCoordXYZ.SetWindowText(_T(""));
			m_edtWindArea.SetWindowText(_T("0"));
			DrawPreview(FALSE);
		}
	}
}

void CStldWindPressureNodalDlg::ChangeCodeDlg()
{
	m_Data.arPresProfile.RemoveAll();
	m_Data.arKBC2009ForceCoef.RemoveAll();
	// TODO: Add your control notification handler code here
	int nSel = m_cobxCodeName.GetCurSel();
	int nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, nSel);
#if defined(_CH) && defined(_MGEN)
	if (m_aWlCodeEnable[nSel]==FALSE)
	{
		CString strCode = _T("");
		m_cobxCodeName.GetWindowText(strCode);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCode);
		GSaveHistoryFormatNF(strErrMsg);
		m_cobxCodeName.SetCurSel(m_nOldSel);
		nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, m_nOldSel);
		return;
	}
	m_nOldSel = nSel;
#endif
	if (nCodeIndex >= 0)
	{
		m_cobxWvep.SetCodeType(nCodeIndex);
		m_pMgr->ChangeCodeDlg(nCodeIndex);
	}
}

void CStldWindPressureNodalDlg::OnSelchangeCmdCodeName() 
{
	int nSel = m_cobxCodeName.GetCurSel();
	int nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, nSel);
	m_Data.nCodeType = nCodeIndex;
	m_Data.CodeParam.Initialize(m_Data.nCodeType);
	Data2Dlg();

	m_cobxWvep.SetCodeType(nCodeIndex);

	ChangeCodeDlg();
}

void CStldWindPressureNodalDlg::OnSelChangeVelocity()
{
	int nCurSel = 0;
	int nCodeType = (int)m_cobxCodeName.GetItemData(nCurSel);
	if(nCodeType != KBC_W_2016 && nCodeType != KDS_W_2019 && nCodeType != KDS_W_2022) return;
	m_pMgr->Data2CodeDlg((void*) &m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);
}

void CStldWindPressureNodalDlg::InitCobxCodeName()
{
	m_cobxCodeName.ResetContent();
	
	CStringArray aCodeName;
	CArray<int, int> aCodeIndex;
	int nNum = m_pMgr->GetCodeNameList(aCodeName, aCodeIndex, &m_aWlCodeEnable);
	
	for(int i = 0; i < nNum; i++) 
	{
		CDlgUtil::CobxAddItem(m_cobxCodeName, aCodeName[i], aCodeIndex[i]);  
	}
	
	if(nNum > 0)
	{
		m_cobxCodeName.SetCurSel(0);
		m_nOldSel = 0;
#if defined(_CH) && defined(_MGEN)
		if (m_aWlCodeEnable[0] == FALSE)
		{
			m_nOldSel = -1;
		}
		else
			m_Data.nCodeType = aCodeIndex[0];
		if (m_Data.CodeParam.CH2012.nAlongType > 1)
		{
			m_Data.CodeParam.CH2012.nAlongType = 0;
		}
		if (m_Data.CodeParam.CH2019.nAlongType > 1)
		{
			m_Data.CodeParam.CH2019.nAlongType = 0;
		}
#else
		m_Data.nCodeType = aCodeIndex[0];
		m_Data.CodeParam.Initialize(m_Data.nCodeType);
#endif
	}
}

void CStldWindPressureNodalDlg::GetMoveCtrlHolderID( CArray<UINT, UINT>& caCtrls, UINT& uiHolderID )
{
	caCtrls.RemoveAll();
	caCtrls.Add(IDC_TM_ELEM_DEFINE_STATIC);
	caCtrls.Add(IDC_TM_NODE_DEFINE);
	caCtrls.Add(IDC_TM_WINDP_DIRAREA_STC);
	caCtrls.Add(IDC_TM_WINDP_DIRAREA);
	caCtrls.Add(IDC_TM_WINDP_DIRAREA_UNIT);
	caCtrls.Add(IDC_TM_WINDP_NODAL_DETAIL);
	caCtrls.Add(IDC_TM_WINDP_CONCENPT_STC);
	caCtrls.Add(IDC_TM_WINDP_CONCENPT);
	caCtrls.Add(IDC_TM_WINDP_CONCENPT_UNIT);
	caCtrls.Add(IDC_TM_TARGET_STATIC);
	caCtrls.Add(IDC_TM_AUTO_CALC_RD);
	caCtrls.Add(IDC_TM_USER_DEF_RD);
	caCtrls.Add(IDC_TM_BTN_PROFILE);
	caCtrls.Add(IDC_CHECK_CONVERT_TO_LOAD);
	uiHolderID = IDC_TM_PLACE_HOLDER;
}

void CStldWindPressureNodalDlg::OnVelocityBtn()
{
	CCMWindPressureVelocityDefDlg dlg;
	dlg.DoModal();
}

void CStldWindPressureNodalDlg::OnCmdBtnLoadCase()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldWindPressureNodalDlg::InitCobxDirection()
{
	m_cobxDirection.ResetContent();
	
	int nIndex;
	CStringArray strItem;
	strItem.Add(_T("X-Y"));
	int nNum = strItem.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		nIndex = m_cobxDirection.AddString(strItem[i]);
		m_cobxDirection.SetItemData(nIndex , i + 1);
	}
	m_cobxDirection.SetCurSel(0);  
}

void CStldWindPressureNodalDlg::OnDeltaposCmdAngle( NMHDR* pNMHDR, LRESULT* pResult )
{    
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_TM_ANGLE_EDT,-180,180,pNMHDR,pResult);
	m_Data.arPresProfile.RemoveAll();
	
	*pResult = 0;  
}

void CStldWindPressureNodalDlg::OnCmdPressureProfile()
{
	if(!Dlg2Data())
		return;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pEditData->CheckWdprKBC2022KdValue(m_Data)) return;

	int nStore;
	double dHeight, dWidth;
	BOOL bRetVal = CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(NULL, dHeight, nStore, dWidth);
	if(dHeight <= 1E-5 || bRetVal == FALSE)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_NODEINFO));
		return;
	}

	int nCurSel = m_cobxCodeName.GetCurSel();
	int nCodeType = (int)m_cobxCodeName.GetItemData(nCurSel);
	if(m_Data.nDirection == 1) //방향이 XY일때는 그냥 보여주면 된다.
	{
		if(nCodeType==KBC_W_2016 || nCodeType==KDS_W_2019 || nCodeType == KDS_W_2022)
		{
			CCMWindPressureProfileKBC2016Dlg dlg;
			dlg.InitWindProfile(m_Data);
			dlg.DoModal();
		}
		else
		{
			CCMWindPressureProfileDlg dlg;
			dlg.InitWindProfile(m_Data);
			dlg.DoModal();
		}
	}
	else//Normal일때....
	{
		CArray<T_WDPR_D, T_WDPR_D&> DataSet; 
		BOOL bRetVal = GetWdprAry(DataSet);
		
		if(bRetVal)
		{
			double dWindVec[3] = { 0.0, };
			double dWindVecTemp[3];
			int i;
			int nSize = DataSet.GetSize();
			for(i = 0 ; i < nSize; i++)
			{
				if(!pDoc->m_pAttrCtrl2->GetWdprAverageWindVec(DataSet[i], dWindVecTemp, TRUE))
					return;
				dWindVec[0] += dWindVecTemp[0];
				dWindVec[1] += dWindVecTemp[1];
				dWindVec[2] += dWindVecTemp[2];
			}
			if(!CMathFunc::mathNormalize(dWindVec[0], dWindVec[1], dWindVec[2], dWindVec[0], dWindVec[1], dWindVec[2]))
			{ //우연히 평균이 0이 되어버릴때.... 그냥 마지막것을 사용한다.
				dWindVec[0] = dWindVecTemp[0];
				dWindVec[1] = dWindVecTemp[1];
				dWindVec[2] = dWindVecTemp[2];
			}
			
			if(nCodeType==KBC_W_2016 || nCodeType==KDS_W_2019 || nCodeType == KDS_W_2022)
			{
				CCMWindPressureProfileKBC2016Dlg dlg;
				dlg.InitWindProfile(m_Data, dWindVec);
				dlg.DoModal();
			}
			else
			{
				CCMWindPressureProfileDlg dlg;
				dlg.InitWindProfile(m_Data, dWindVec);
				dlg.DoModal();
			}
		}
	}
}

BOOL CStldWindPressureNodalDlg::Data2Dlg(BOOL bWarning)
{
	//m_cobxLdgr.ChangeSelect(m_Data.KeyLdgr);
	if(m_Data.KeyStld)
		m_cobxLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Data.KeyStld);

	if(m_Data.nDirection == 2) m_cobxDirection.SetCurSel(1);
	else                       m_cobxDirection.SetCurSel(0);

	m_edtAngle.SetEditUnit(m_Data.dAngle);
	m_edtScaleFactor.SetEditUnit(m_Data.dScaleFactor); 

	if(m_Data.nCodeType) CDlgUtil::CobxSetCurSelItemData(m_cobxCodeName, m_Data.nCodeType);
	if(m_Data.KeyWvep)   m_cobxWvep.ChangeSelect(m_Data.KeyWvep);

	SetStructureType(m_Data.nodalStruct.nType);
	CString strPos;
	strPos.Format(_T("%g, %g, %g"), m_Data.dConcenPt[0], m_Data.dConcenPt[1], m_Data.dConcenPt[2]);
	m_wndConcenCoordXYZ.SetWindowText(strPos);
	m_edtWindArea.SetEditUnit(m_Data.dWindArea);

	ChangeCodeDlg();
	OnCmdRdoChange();
	//Code Dlg -> Data
	m_pMgr->Data2CodeDlg((void*)&m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);
	return TRUE;
}

BOOL CStldWindPressureNodalDlg::Dlg2Data(BOOL bWarning)
{
	//m_cobxLdgr.GetSelectedLdgr(m_Data.KeyLdgr);
	unsigned int nLoadCaseType;
	m_cobxLoadCase.GetSelectedLoad(nLoadCaseType, m_Data.KeyStld);
	if(nLoadCaseType != D_LOADCASE_STATIC || m_Data.KeyStld <= 0)
	{
		if(bWarning) AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_STLD));
		return FALSE;
	}
	
	MyParser Parser;
	CString strPos;

	m_Data.nType = WDPRTYPE_NODAL; //Nodal...
	m_Data.nSelection = 3; //Node....
	m_Data.nDirection = GetDirection();
	if(m_Data.nDirection == 1)
		m_Data.dAngle = m_edtAngle.GetEditValue();
	else
	{
		ASSERT(0);
		return FALSE;
	}

	m_Data.dScaleFactor = m_edtScaleFactor.GetEditValue(); 
	m_Data.nCodeType = (int)CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName,m_cobxCodeName.GetCurSel());
	m_cobxWvep.GetSelectedWvep(m_Data.KeyWvep);
	if(m_Data.KeyWvep <= 0)
	{
		if(bWarning) AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return FALSE;
	}
	
	//Code Dlg -> Data
	if(!m_pMgr->CodeDlg2Data((void*) &m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL, TRUE))
		return FALSE;
	
	CDBDoc::GetDocPoint()->m_pViewCtrl->GetAllSelectedNode(m_Data.arKeysDefineNode);
	
	m_wndConcenCoordXYZ.GetWindowText(strPos);
	if(!Parser.ParsingPositionArg(strPos)) return FALSE;
	if(!Parser.GetPositionData(m_Data.dConcenPt[0],m_Data.dConcenPt[1],m_Data.dConcenPt[2])) return FALSE;
	m_Data.dWindArea = m_edtWindArea.GetEditValue();
	if(m_Data.dWindArea <= 1E-5 && m_bExecute)
	{
		if(bWarning) AfxMessageBox(_LS(IDS_TM_WINDP_NODAL_INVALID_AREA));
		return FALSE;
	}
	
	if(m_Data.arKeysDefineNode.GetSize() < 1 || m_Data.arKeysDefineNode.GetSize() == 2)
	{
		if(bWarning) AfxMessageBox(_LS(IDS_TM_WINDP_NODAL_INVALID_INPUTNODE));
		return FALSE;
	}

	//끝으로 Wvep의 CodeType과 WDPR의 CodeType을 비교한다.
	T_WVEP_D WvepD;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWvep(m_Data.KeyWvep, WvepD))
	{
		ASSERT(0);
		return FALSE;
	}
	if(WvepD.nCodeType != m_Data.nCodeType)
	{
		ASSERT(0);
		return FALSE;
	}

	if(m_Data.nCodeType==KBC_W_2016)
	{
		if(!m_Data.CodeParam.KBC2016.bAlongWind && !m_Data.CodeParam.KBC2016.bAcrossWind && !m_Data.CodeParam.KBC2016.bTorsionalWind)
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_DIR));
			return FALSE;
		}

		if(!CDBDoc::GetDocPoint()->m_pEditData->CheckWdprKBC2016VibrValue(m_Data))
			return FALSE;
	}
	else if(m_Data.nCodeType==KDS_W_2019)
	{
		if(!m_Data.CodeParam.KDS2019.bAlongWind && !m_Data.CodeParam.KDS2019.bAcrossWind && !m_Data.CodeParam.KDS2019.bTorsionalWind)
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_DIR));
			return FALSE;
		}

		if(!CDBDoc::GetDocPoint()->m_pEditData->CheckWdprKBC2016VibrValue(m_Data))
			return FALSE;
	}
	else if (m_Data.nCodeType == KDS_W_2022)
	{
		if (!m_Data.CodeParam.KDS2021.bAlongWind && !m_Data.CodeParam.KDS2021.bAcrossWind && !m_Data.CodeParam.KDS2021.bTorsionalWind)
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_DIR));
			return FALSE;
		}

		if (!CDBDoc::GetDocPoint()->m_pEditData->CheckWdprKBC2016VibrValue(m_Data))
			return FALSE;
	}
	return TRUE;
}

int CStldWindPressureNodalDlg::GetDirection()
{
	int nSel = m_cobxDirection.GetCurSel();
	if(nSel == 0)
		return 1; //X-Y
	else if(nSel == 1)
		return 2; //Normal

	m_cobxDirection.SetCurSel(0);
	return 1;
}

void CStldWindPressureNodalDlg::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_BUFFER_AFTER:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_UNIT:
		OnUnitChange();
		break;
	default:
		break;
	}	  
}

void CStldWindPressureNodalDlg::OnUnitChange()
{
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprPrevious(m_Data);
}

BOOL CStldWindPressureNodalDlg::CheckWdpr( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet, BOOL bMsg )
{
	if(m_Data.arKeysDefineNode.GetSize() < 1 || m_Data.arKeysDefineNode.GetSize() == 2)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NODAL_INVALID_INPUTNODE));
		return FALSE;
	}
#ifdef _MGEN_CH
	if(CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD))
			return TRUE;
#endif
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl2->IsWdprSelectionBelowBaseLevel(rDataSet))
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_WARNING_BELOW_BASELEVEL));
		return FALSE;
	}
	CMap<T_NODE_K,T_NODE_K,T_CNLD_D,T_CNLD_D&> mapLoad;
	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcWindPressure_Nodal(rDataSet, mapLoad, NULL, TRUE))
		return FALSE;

	return TRUE;
}

BOOL CStldWindPressureNodalDlg::GetWdprAry( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet )
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CLateralLoad lateralLoad;
	
	int i;

	int nSize = 1;
	rDataSet.SetSize(1);
	for(i = 0 ; i < nSize; i++)
	{
		rDataSet[0] = m_Data;
	}
	
	CLateralLoad laterLoad;
	nSize = rDataSet.GetSize();
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		if(!laterLoad.MakeWindPressure(rDataSet[i]))
		rDataSet.RemoveAt(i);
	}

	nSize = rDataSet.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
	  if(!CWindLoadGeneratorCtrl::IsValidPressure_Nodal(rDataSet[i]))
	  {
		  AfxMessageBox(_LS(IDS_TM_WINDP_NO_PRESSURE));
		  return FALSE;
	  }
	} 

	return TRUE;
}

void CStldWindPressureNodalDlg::OnChange_ClearProfile()
{
	m_Data.arPresProfile.RemoveAll();
}

void CStldWindPressureNodalDlg::OnCmdBtnStructure()
{
	if(!Dlg2Data())
		return;

	CStldWindPressureNodalDlg_StructureType dlg;
	dlg.SetWdpr(&m_Data);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetNodalStructure(&m_Data);
		double dArea;
		double dConcenPt[3];
		if(CWindLoadGeneratorCtrl::GetNodal_WindAreaConcenPt(m_Data, dArea, dConcenPt))
		{
			m_Data.dWindArea = dArea;
			m_Data.dConcenPt[0] = dConcenPt[0];
			m_Data.dConcenPt[1] = dConcenPt[1];
			m_Data.dConcenPt[2] = dConcenPt[2];

			CString strPos;
			strPos.Format(_T("%g, %g, %g"), m_Data.dConcenPt[0], m_Data.dConcenPt[1], m_Data.dConcenPt[2]);
			m_wndConcenCoordXYZ.SetWindowText(strPos);
			m_edtWindArea.SetEditUnit(m_Data.dWindArea);
			m_Data.arPresProfile.RemoveAll();
			DrawPreview(TRUE);
		}
	}
}

void CStldWindPressureNodalDlg::DrawPreview( BOOL bShow )
{
	if(bShow)
	{
		CWindLoadGeneratorCtrl::m_Preview.bShow = bShow;
		CWindLoadGeneratorCtrl::m_Preview.SetData(m_Data.nodalStruct);
		double* pOrigin = CWindLoadGeneratorCtrl::m_Preview.dOrigin;
		CWindLoadGeneratorCtrl::GetNodal_StructureAverageMaxZ(m_Data.arKeysDefineNode, pOrigin);
	}
	else
	{
		CWindLoadGeneratorCtrl::m_Preview.bShow = bShow;
	}

	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	ExportDisplaySetting::SET(&dspOpt); 
}

void CStldWindPressureNodalDlg::SetStructureType( int nType )
{
	CButton* pBtnAuto = (CButton*)GetDlgItem(IDC_TM_AUTO_CALC_RD);
	CButton* pBtnUser = (CButton*)GetDlgItem(IDC_TM_USER_DEF_RD);

	if(pBtnAuto == NULL || pBtnUser == NULL)         return;
	if(::IsWindow(pBtnAuto->GetSafeHwnd()) == FALSE) return;
	if(::IsWindow(pBtnUser->GetSafeHwnd()) == FALSE) return;

	if(nType == 0)
	{
		pBtnAuto->SetCheck(FALSE);
		pBtnUser->SetCheck(TRUE);
	}
	else
	{
		pBtnAuto->SetCheck(TRUE);
		pBtnUser->SetCheck(FALSE);
	}  
}

void CStldWindPressureNodalDlg::OnCmdRdoChange()
{
	CButton* pBtnAuto = (CButton*)GetDlgItem(IDC_TM_AUTO_CALC_RD);
	CButton* pBtnUser = (CButton*)GetDlgItem(IDC_TM_USER_DEF_RD);
	CButton* pBtnStru = (CButton*)GetDlgItem(IDC_TM_WINDP_NODAL_DETAIL);

	if(pBtnAuto == NULL || pBtnUser == NULL || pBtnStru == NULL) return;
	if(::IsWindow(pBtnAuto->GetSafeHwnd()) == FALSE) return;
	if(::IsWindow(pBtnUser->GetSafeHwnd()) == FALSE) return;
	if(::IsWindow(pBtnStru->GetSafeHwnd()) == FALSE) return;

	m_Data.nodalStruct.nType = 0;

	if(pBtnAuto->GetCheck())
	{
		m_Data.nodalStruct.nType = 0;
		m_edtWindArea.EnableWindow(FALSE);
		m_wndConcenCoordXYZ.EnableWindow(FALSE);
		pBtnStru->EnableWindow(TRUE);
	}
	else
	{
		m_edtWindArea.EnableWindow(TRUE);
		m_wndConcenCoordXYZ.EnableWindow(TRUE);
		pBtnStru->EnableWindow(FALSE);
	}  
}
