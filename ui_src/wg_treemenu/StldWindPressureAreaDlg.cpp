// CMGridImpactFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg.h"
#include "StldWindPressureAreaMgr.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\ExportFunc.h"
#include "..\wg_cmd\CMLoadGroupDlg.h"
#include "..\wg_cmd\CMLdarGrupDefDlg.h"
#include "..\wg_cmd\CMWindPressureVelocityDefDlg.h"
#include "..\wg_cmd\CMWindPressureProfileDlg.h"
#include "..\wg_cmd\CMWindPressureProfileKBC2016Dlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

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
#include "..\wg_db\Wind_IS875_2015.h"

#include "..\wg_gr\GDisplaySetting.h"
#include "..\wg_common\TBUtilFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg dialog

#define ELEM_FRAME  1
#define ELEM_PLANAR 2

CStldWindPressureAreaDlg::CStldWindPressureAreaDlg(CWnd* pParent /*=NULL*/)
	: CCMWindPressureBaseDlg(CStldWindPressureAreaDlg::IDD)
{
	m_pRootDlg = (CCMWindPressureRootDlg*)pParent;

	m_pMgr = NULL;
	m_pMgr = new CStldWindPressureAreaMgr(this, (void*)&m_Data);
	m_Data.Initialize();
	m_LagrK = 0;
#ifdef _CH
	m_Data.CodeParam.CH2012.Initialize();
	m_Data.CodeParam.CH2001.Initialize();
	m_Data.CodeParam.CH2019.Initialize();
#endif

	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprOut(m_Data);

	m_nSelectionType = 1;
}

CStldWindPressureAreaDlg::~CStldWindPressureAreaDlg()
{
	if (m_pMgr != NULL)
	{
		delete m_pMgr;
		m_pMgr = NULL;
	}  

	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.nTypeLaplLagr = 0;
	dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	ExportDisplaySetting::SET(&dspOpt); 
}

void CStldWindPressureAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TM_PLACE_HOLDER,    m_wndHolder);
	DDX_Control(pDX, IDC_TM_CODE_NAME,       m_cobxCodeName);
	DDX_Control(pDX, IDC_TM_VELOCITY_NAME,   m_cobxWvep);  
	//DDX_Control(pDX, IDC_TM_LOADGROUP_COMBO, m_cobxLdgr);  
	DDX_Control(pDX, IDC_TM_LOADCASE,        m_cobxLoadCase);
	DDX_Control(pDX, IDC_TM_DIRECTION_COMBO, m_cobxDirection);
	DDX_Control(pDX, IDC_TM_LAGR_NAME,       m_cobxLagr); 
	DDX_Control(pDX, IDC_TM_ELEM_DEFINE,     m_edElemList);  
	DDX_Control(pDX, IDC_TM_ANGLE_EDT,       m_edtAngle);    
	DDX_Control(pDX, IDC_TM_SCALEFACTOR,     m_edtScaleFactor);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_EDIT,  m_wndCoordXYZ);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_UNIT,  m_wndCoordUnit);
}


BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg, CCMChildBarBase)
	ON_BN_CLICKED(IDC_TM_VELOCITY_BTN,  OnVelocityBtn)  
	ON_BN_CLICKED(IDC_TM_LOADCASE_BTN,  OnCmdBtnLoadCase)
	//ON_BN_CLICKED(IDC_TM_LOADGROUP_BTN, OnCmdBtnLoadGroup)  
	ON_BN_CLICKED(IDC_TM_TARGET_RDO1,   OnCmdTargetChanged)  
	ON_BN_CLICKED(IDC_TM_TARGET_RDO2,   OnCmdTargetChanged)  
	ON_BN_CLICKED(IDC_TM_BTN_PROFILE,   OnCmdPressureProfile)  
	ON_BN_CLICKED(IDC_TM_LAGR_BTN,      OnCmdBtnLoadAreaGroup)
	ON_CBN_SELCHANGE(IDC_TM_CODE_NAME,  OnSelchangeCmdCodeName)
	ON_CBN_SELCHANGE(IDC_TM_LAGR_NAME,  OnSelChangeLagr)
	ON_EN_CHANGE(IDC_TM_ANGLE_EDT,	  	OnChangeAngle)
	ON_CBN_SELCHANGE(IDC_TM_DIRECTION_COMBO,   OnSelChangeDir)
	ON_CBN_SELCHANGE(IDC_TM_VELOCITY_NAME,   OnSelChangeVelocity)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ANGLE_SPIN, OnDeltaposCmdAngle)     //Span Num
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg message handlers

BOOL CStldWindPressureAreaDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

#ifdef _CH
	GetDlgItem(IDC_CHECK_CONVERT_TO_LOAD)->ShowWindow(SW_SHOW);
#endif

	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);

	m_pMgr->SetInit(TRUE);
	InitCobxCodeName();
	if(CWindLoadGeneratorCtrl::m_bSaved[WDPRTYPE_AREA-1])
	{
		m_Data = CWindLoadGeneratorCtrl::m_WdprLast[WDPRTYPE_AREA-1];
	}
	
	InitCobxDirection();

	m_wndHolder.ShowWindow(SW_HIDE);
	m_edElemList.Connect(SC_ID_ELEM, &m_edElemList);	
	m_cobxLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_edtAngle.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	m_wndCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCoordXYZ.SetAttUcsPos();
	m_wndCoordXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);  
	m_wndCoordXYZ.SetNextLink(NULL);

	int nCurSel = m_cobxCodeName.GetCurSel();
	int nCodeType = (int)m_cobxCodeName.GetItemData(nCurSel);
	m_cobxWvep.SetCodeType(nCodeType);

	GetElementType();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStldWindPressureAreaDlg::GetHolderRect( CRect* pRect )
{
	if(m_wndHolder.GetSafeHwnd() && ::IsWindow(m_wndHolder.GetSafeHwnd()))
	{
		m_wndHolder.GetWindowRect(pRect);
		return TRUE;
	}
	return FALSE;
}

void CStldWindPressureAreaDlg::Execute()
{
	if (!Dlg2Data())
		return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pEditData->CheckWdprKBC2022KdValue(m_Data)) return;

	//double start = clock();
	CArray<T_WDPR_D, T_WDPR_D&> DataSet;
	if (GetWdprAry(DataSet, FALSE))
	{
		//double cost1=clock()-start;	//Bugfree No.20987
		//start = clock();
		if (!CheckWdpr(DataSet, TRUE))
			return;

		if (MITC_CommonTool::AddWdpr_Area(DataSet, CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD)))
		{
			CWindLoadGeneratorCtrl::m_WdprLast[WDPRTYPE_AREA - 1] = m_Data;
			CWindLoadGeneratorCtrl::m_bSaved[WDPRTYPE_AREA - 1] = TRUE;
			CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
		}
		//double cost2=clock()-start;
		//GSaveHistoryFormat("충루箕뵐潼警속痰珂:%f - %f", cost1, cost2);
	}
}

void CStldWindPressureAreaDlg::ChangeCodeDlg()
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

void CStldWindPressureAreaDlg::OnSelchangeCmdCodeName() 
{
	int nSel = m_cobxCodeName.GetCurSel();
	int nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, nSel);

	m_Data.nCodeType = nCodeIndex;
	m_Data.CodeParam.Initialize(m_Data.nCodeType);
	Data2Dlg();

	m_cobxWvep.SetCodeType(nCodeIndex);

	ChangeCodeDlg();
}

void CStldWindPressureAreaDlg::InitCobxCodeName()
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
			m_nOldSel = -1;
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

void CStldWindPressureAreaDlg::GetMoveCtrlHolderID( CArray<UINT, UINT>& caCtrls, UINT& uiHolderID )
{
	caCtrls.RemoveAll();
	caCtrls.Add(IDC_TM_BTN_PROFILE);
	caCtrls.Add(IDC_TM_LAPL_ELEM_TYPE_GRB);
	caCtrls.Add(IDC_TM_LAPL_ELEM_TYPE1_RDO);
	caCtrls.Add(IDC_TM_LAPL_ELEM_TYPE2_RDO);
	caCtrls.Add(IDC_TM_TARGET_STATIC);
	caCtrls.Add(IDC_TM_TARGET_RDO1);
	caCtrls.Add(IDC_TM_TARGET_RDO2);
	caCtrls.Add(IDC_TM_LAGR_NAME_STATIC);
	caCtrls.Add(IDC_TM_LAGR_NAME);
	caCtrls.Add(IDC_TM_LAGR_BTN);
	caCtrls.Add(IDC_TM_ELEM_DEFINE_STATIC);
	caCtrls.Add(IDC_TM_ELEM_DEFINE);
	caCtrls.Add(IDC_CHECK_CONVERT_TO_LOAD);
	caCtrls.Add(IDC_WINDP_TM_EXECUTE);
	caCtrls.Add(IDC_WINDP_TM_CLOSE);  
	uiHolderID = IDC_TM_PLACE_HOLDER;
}

void CStldWindPressureAreaDlg::OnVelocityBtn()
{
	CCMWindPressureVelocityDefDlg dlg;
	dlg.DoModal();
}

// void CStldWindPressureAreaDlg::OnCmdBtnLoadGroup()
// {
//   CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLoadGroupDlg::IDD);		  
// }

void CStldWindPressureAreaDlg::OnCmdBtnLoadAreaGroup()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLdarGrupDefDlg::IDD);		
}

void CStldWindPressureAreaDlg::OnCmdBtnLoadCase()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldWindPressureAreaDlg::InitCobxDirection()
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
		m_cobxDirection.SetItemData(nIndex , i + 1);
	}
	m_cobxDirection.SetCurSel(0);  
}

void CStldWindPressureAreaDlg::OnDeltaposCmdAngle( NMHDR* pNMHDR, LRESULT* pResult )
{    
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_TM_ANGLE_EDT,-180,180,pNMHDR,pResult);
	m_Data.arPresProfile.RemoveAll();

	*pResult = 0;  
}

void CStldWindPressureAreaDlg::OnCmdTargetChanged()
{  
	m_nSelectionType = GetSelectionType();
	if(m_nSelectionType == 1) //Group
	{
		GetDlgItem(IDC_TM_LAGR_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAGR_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->EnableWindow(FALSE);
	}
	else if(m_nSelectionType == 2) //Elements
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

void CStldWindPressureAreaDlg::OnCmdPressureProfile()
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
		if(nCodeType==KBC_W_2016 || nCodeType==KDS_W_2019 || nCodeType==KDS_W_2022)
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
		BOOL bRetVal = GetWdprAry(DataSet, TRUE);

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

			if(nCodeType==KBC_W_2016 || nCodeType==KDS_W_2019 || nCodeType==KDS_W_2022)
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

int CStldWindPressureAreaDlg::GetSelectionType()
{
	int nSelType = 1;
	if(((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->GetCheck())
		nSelType = 1;
	else if(((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
		nSelType = 2;
	else 
	{
		nSelType = 1;
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
	}
	return nSelType;  
}

BOOL CStldWindPressureAreaDlg::Data2Dlg(BOOL bWarning)
{
	//m_cobxLdgr.ChangeSelect(m_Data.KeyLdgr);
	if(m_Data.KeyStld)
		m_cobxLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Data.KeyStld);

	if(m_Data.nDirection == 2) m_cobxDirection.SetCurSel(1);
	else                       m_cobxDirection.SetCurSel(0);

	CString strPos;
	strPos.Format(_T("%g, %g, %g"), m_Data.dInnerPnt[0], m_Data.dInnerPnt[1], m_Data.dInnerPnt[2]);
	m_wndCoordXYZ.SetWindowText(strPos);

	m_edtAngle.SetEditUnit(m_Data.dAngle);
	m_edtScaleFactor.SetEditUnit(m_Data.dScaleFactor); 

	if(m_Data.nCodeType)
	{
		CDlgUtil::CobxSetCurSelItemData(m_cobxCodeName, m_Data.nCodeType);
		m_cobxWvep.SetCodeType(m_Data.nCodeType);
	}
	if(m_Data.KeyWvep)   m_cobxWvep.ChangeSelect(m_Data.KeyWvep);

	if(m_Data.nSelection == 1)
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(FALSE);
		m_cobxLagr.ChangeSelect(m_LagrK);
	}
	else if(m_Data.nSelection == 2)
	{
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->SetCheck(TRUE);

		CDBDoc::GetDocPoint()->m_pViewCtrl->SelectElem(NULL, m_Data.arKeysDefineElem, FALSE);
	}
	OnCmdTargetChanged();
	OnSelChangeDir();
	ChangeCodeDlg();
	OnSelChangeLagr();
	//Code Dlg -> Data

	EN_WIND_PRESSURE_CODE_TYPE enCodeEnum = m_pMgr->GetCodeIdxbyDBCode(m_Data.nCodeType);

	m_pMgr->Data2CodeDlg((void*)&m_Data, enCodeEnum);
	return TRUE;
}

BOOL CStldWindPressureAreaDlg::Dlg2Data(BOOL bWarning)
{
	//m_cobxLdgr.GetSelectedLdgr(m_Data.KeyLdgr);
	unsigned int nLoadCaseType;
	m_cobxLoadCase.GetSelectedLoad(nLoadCaseType, m_Data.KeyStld);
	if(nLoadCaseType != D_LOADCASE_STATIC || m_Data.KeyStld <= 0)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_STLD));
		return FALSE;
	}

	m_Data.nType = WDPRTYPE_AREA; //Area...
	m_Data.nDirection = GetDirection();
	if(m_Data.nDirection == 1)
		m_Data.dAngle = m_edtAngle.GetEditValue();
	else if(m_Data.nDirection == 2)
	{
		CString strPos;
		m_wndCoordXYZ.GetWindowText(strPos);
		MyParser Parser;
		if(!Parser.ParsingPositionArg(strPos)) return FALSE;
		if(!Parser.GetPositionData(m_Data.dInnerPnt[0],m_Data.dInnerPnt[1],m_Data.dInnerPnt[2])) return FALSE;
	}
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
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return FALSE;
	}

	//Code Dlg -> Data
	EN_WIND_PRESSURE_CODE_TYPE enCodeEnum = m_pMgr->GetCodeIdxbyDBCode(m_Data.nCodeType);
	if(!m_pMgr->CodeDlg2Data((void*) &m_Data, enCodeEnum, TRUE))
		return FALSE;

	if(((CButton*)GetDlgItem(IDC_TM_TARGET_RDO1))->GetCheck())
	{
		m_Data.nSelection = 1;
		m_Data.arKeysDefineElem.RemoveAll();
		m_cobxLagr.GetSelectedLagr(m_LagrK);
	}
	else if(((CButton*)GetDlgItem(IDC_TM_TARGET_RDO2))->GetCheck())
	{
		m_Data.nSelection = 2;
		m_LagrK = 0;
		//     CString strTemp;
		//     m_edElemList.GetWindowText(strTemp);
		//     CArray<int, int> aElem;  
		//     if(CStrParser::ParsingListByTo(strTemp, aElem))
		// [PMS 5011-6] 요소 많이 선택 시 문자열->요소번호 파싱하다 프로그램 다운
		CArray<T_ELEM_K, T_ELEM_K> aElem;  
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pViewCtrl->GetAllSelectedElem(aElem);
		if(!aElem.IsEmpty())
		{
			int i, nSize;
			nSize = aElem.GetSize();
			m_Data.arKeysDefineElem.SetSize(nSize);
			for(i = 0 ; i < nSize; i++)
			{
				m_Data.arKeysDefineElem[i] = aElem[i];
			}
		}
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
		if(	 (m_Data.CodeParam.KBC2016.bAutoForceCoef && !m_Data.CodeParam.KBC2016.bAlongWind) ||
			(!m_Data.CodeParam.KBC2016.bAutoForceCoef && !m_Data.CodeParam.KBC2016.bAlongWind && !m_Data.CodeParam.KBC2016.bAcrossWind && !m_Data.CodeParam.KBC2016.bTorsionalWind))
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_DIR));
			return FALSE;
		}

		if(!CDBDoc::GetDocPoint()->m_pEditData->CheckWdprKBC2016VibrValue(m_Data))
			return FALSE;
	}
	else if(m_Data.nCodeType==KDS_W_2019)
	{
		if(	 (m_Data.CodeParam.KDS2019.bAutoForceCoef && !m_Data.CodeParam.KDS2019.bAlongWind) ||
			(!m_Data.CodeParam.KDS2019.bAutoForceCoef && !m_Data.CodeParam.KDS2019.bAlongWind && !m_Data.CodeParam.KDS2019.bAcrossWind && !m_Data.CodeParam.KDS2019.bTorsionalWind))
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_DIR));
			return FALSE;
		}

		if(!CDBDoc::GetDocPoint()->m_pEditData->CheckWdprKBC2016VibrValue(m_Data))
			return FALSE;
	}
	else if (m_Data.nCodeType == KDS_W_2022)
	{
		if  ((m_Data.CodeParam.KDS2021.bAutoForceCoef && !m_Data.CodeParam.KDS2021.bAlongWind) ||
			(!m_Data.CodeParam.KDS2021.bAutoForceCoef && !m_Data.CodeParam.KDS2021.bAlongWind && !m_Data.CodeParam.KDS2021.bAcrossWind && !m_Data.CodeParam.KDS2021.bTorsionalWind))
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_DIR));
			return FALSE;
		}

		if (!CDBDoc::GetDocPoint()->m_pEditData->CheckWdprKBC2016VibrValue(m_Data))
			return FALSE;
	}
	return TRUE;
}

void CStldWindPressureAreaDlg::OnSelChangeLagr()
{
	GDisplaySetting dspOpt;
	m_nSelectionType = GetSelectionType();
	if(m_nSelectionType == 1) //Group
	{
		T_LAGR_K LagrK;
		m_cobxLagr.GetSelectedLagr(LagrK);
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 2;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		dspOpt.m_Load.m_LoadingAreaKeys.Add((unsigned int)LagrK);
		ExportDisplaySetting::SET(&dspOpt);
	}
	else if(m_nSelectionType == 2) //Elements
	{
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 0;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		ExportDisplaySetting::SET(&dspOpt);
	}
}

void CStldWindPressureAreaDlg::OnSelChangeDir()
{
	if(GetDirection() == 2) //Normal
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
	m_Data.arPresProfile.RemoveAll();
}

void CStldWindPressureAreaDlg::OnSelChangeVelocity()
{
	int nCurSel = 0;
	int nCodeType = (int)m_cobxCodeName.GetItemData(nCurSel);
	if(nCodeType != KBC_W_2016 && nCodeType != KDS_W_2019 && nCodeType != KDS_W_2022) return;
	m_pMgr->Data2CodeDlg((void*) &m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);
}

int CStldWindPressureAreaDlg::GetDirection()
{
	int nSel = m_cobxDirection.GetCurSel();
	if(nSel == 0)
		return 1; //X-Y
	else if(nSel == 1)
		return 2; //Normal

	m_cobxDirection.SetCurSel(0);
	return 1;
}

void CStldWindPressureAreaDlg::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
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

void CStldWindPressureAreaDlg::OnUnitChange()
{
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprPrevious(m_Data);
}

BOOL CStldWindPressureAreaDlg::CheckWdpr( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet, BOOL bMsg )
{
#ifdef _MGEN_CH
	if(CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD))
		return TRUE;
#endif
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl2->IsWdprSelectionBelowBaseLevel(rDataSet))
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_WARNING_BELOW_BASELEVEL));
		return FALSE;
	}

	if(m_Data.nSelection != 1 && GetElementType()==ELEM_PLANAR)
	{
		if(m_Data.nCodeType == IS_W_875_2015)
		{
			CWind_IS875_2015 WindIS875;
			WindIS875.GetKaFactorArrPlate(rDataSet);
		}
		for(int i=0; i<rDataSet.GetSize(); i++)
		{
			CMap<T_ELEM_K,T_ELEM_K,T_PRES_D,T_PRES_D&> mapLoad;
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcWindPressure_Face(rDataSet[i], mapLoad))
				return FALSE;
		}
	}
	else
	{
		if(m_Data.nSelection == 1 && m_Data.nCodeType == IS_W_875_2015)
		{
			CWind_IS875_2015 WindIS875;
			WindIS875.GetKaFactorArrGroup(rDataSet);
		}
		else if(m_Data.nSelection == 2 && GetElementType() == ELEM_FRAME && m_Data.nCodeType == IS_W_875_2015)
		{
			CWind_IS875_2015 WindIS875;
			WindIS875.GetKaFactorArrFrame(rDataSet);
		}
		CMap<T_NODE_K,T_NODE_K,T_CNLD_D,T_CNLD_D&> mapLoad;
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcWindPressure_Nodal(rDataSet, mapLoad, NULL, TRUE))
			return FALSE;
	}

	return TRUE;
}

BOOL CStldWindPressureAreaDlg::GetWdprAry( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet , BOOL bProfile)
{
	int i, nSize;
	int nSelection = m_Data.nSelection;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_LAPL_K, T_LAPL_K> KeyLapl;

	int nElemType = GetElementType();
	if(m_Data.nSelection == 1)
	{
		CArray<T_LAGR_K, T_LAGR_K> KeyLagr;
		KeyLagr.SetSize(1);
		KeyLagr[0] = m_LagrK;
		pDoc->m_pAttrCtrl2->GetKeyListLaplByLagr(KeyLagr, KeyLapl);
		nSize = KeyLapl.GetSize();
		rDataSet.SetSize(nSize);
		for(i = 0 ; i < nSize; i++)
		{
			m_Data.KeyLapl = KeyLapl[i];
			rDataSet[i] = m_Data;
		}
		if(nSize <= 0)
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_LDPL));
			return FALSE;
		}
	}
	else
	{
		if(nElemType==ELEM_PLANAR)
		{
			T_ELEM_D ElemD; ElemD.Initialize();
			for(int i=m_Data.arKeysDefineElem.GetSize()-1; i>=0; i--)
			{
				if(!pDoc->m_pAttrCtrl->GetElem(m_Data.arKeysDefineElem[i],ElemD)) { ASSERT(0); return FALSE; }
				if(!pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp)&&!pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp)) m_Data.arKeysDefineElem.RemoveAt(i);
			}

			if(m_Data.arKeysDefineElem.GetSize()<1) return FALSE;

			T_WDPR_D WdprD;
			rDataSet.SetSize(m_Data.arKeysDefineElem.GetSize());
			for(int i=0; i<m_Data.arKeysDefineElem.GetSize(); i++)
			{
				WdprD = m_Data;
				WdprD.arKeysDefineElem.RemoveAll();
				WdprD.arKeysDefineElem.Add(m_Data.arKeysDefineElem[i]);
				rDataSet.SetAt(i, WdprD);
			}

		}
		else
		{
			CWindLoadGeneratorCtrl myCtrl;
			if(!myCtrl.RunFindClosedArea(m_Data.arKeysDefineElem, TRUE))
				return FALSE;

			CArray<UINT, UINT> arSurfKey;
			CArray<T_NODE_K, T_NODE_K> caNodeKey;
			CArray<T_ELEM_K, T_ELEM_K> caElemKey;
			nSize = myCtrl.GetSurfKeyList(arSurfKey);

			rDataSet.SetSize(nSize);
			for(i = 0 ; i < nSize; i++)
			{
				rDataSet[i] = m_Data;
				if(!myCtrl.GetSurfInfo(arSurfKey[i], rDataSet[i].arKeysDefineNode, rDataSet[i].arKeysDefineElem))
				{
					ASSERT(0);
					return FALSE;
				}
			}
		}
	}

	int nSizeWdprSet = rDataSet.GetSize();
	if(nSizeWdprSet <= 0) return FALSE;
	BOOL bCheck = TRUE;
#ifdef _MGEN_CH
	bCheck = CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD) == FALSE;
#endif
	if(bCheck || bProfile)
	{
		CLateralLoad laterLoad;
		for(int i = nSizeWdprSet - 1 ; i >= 0; i--)
		{
			if(!laterLoad.MakeWindPressure(rDataSet[i]))
				rDataSet.RemoveAt(i); //일단 실패하는 상황을 만들면 안되지만. 실패하면 지우자.
		}
	}
	return TRUE;
}

void CStldWindPressureAreaDlg::OnChangeAngle()
{
	m_Data.arPresProfile.RemoveAll();
}

int CStldWindPressureAreaDlg::GetElementType()
{
	int nElemType = ELEM_FRAME;
	if(((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->GetCheck())
		nElemType = ELEM_FRAME;
	else if(((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->GetCheck())
		nElemType = ELEM_PLANAR;
	else 
	{
		nElemType = ELEM_FRAME;
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO))->SetCheck(FALSE);
	}
	return nElemType;  
}