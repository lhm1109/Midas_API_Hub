// CMGridImpactFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureBeamDlg.h"
#include "StldWindPressureBeamMgr.h"

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
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_db\wg_db_MathFunc.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_gr\GDisplaySetting.h"
#include "..\wg_db\MITC_CommonTool.h"
#include "..\wg_common\TBUtilFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureBeamDlg dialog


CStldWindPressureBeamDlg::CStldWindPressureBeamDlg(CWnd* pParent /*=NULL*/)
	: CCMWindPressureBaseDlg(CStldWindPressureBeamDlg::IDD)
{
	m_pRootDlg = (CCMWindPressureRootDlg*)pParent;

	m_pMgr = NULL;
	m_pMgr = new CStldWindPressureBeamMgr(this, (void*)&m_Data);
	m_Data.Initialize();
	m_LagrK = 0;

#ifdef _CH
	m_Data.CodeParam.CH2012.Initialize();
	m_Data.CodeParam.CH2001.Initialize();
	m_Data.CodeParam.CH2019.Initialize();
#endif
	
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprOut(m_Data);
}

CStldWindPressureBeamDlg::~CStldWindPressureBeamDlg()
{
	if (m_pMgr != NULL)
	{
		delete m_pMgr;
		m_pMgr = NULL;
	}
}

void CStldWindPressureBeamDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TM_PLACE_HOLDER,    m_wndHolder);
	DDX_Control(pDX, IDC_TM_CODE_NAME,       m_cobxCodeName);
	DDX_Control(pDX, IDC_TM_VELOCITY_NAME,   m_cobxWvep);
	DDX_Control(pDX, IDC_TM_LOADCASE,        m_cobxLoadCase);
	DDX_Control(pDX, IDC_TM_DIRECTION_COMBO, m_cobxDirection);
	DDX_Control(pDX, IDC_TM_ANGLE_EDT,       m_edtAngle);    
	DDX_Control(pDX, IDC_TM_SCALEFACTOR,     m_edtScaleFactor);
}


BEGIN_MESSAGE_MAP(CStldWindPressureBeamDlg, CCMChildBarBase)
	ON_BN_CLICKED(IDC_TM_VELOCITY_BTN,  OnVelocityBtn)  
	ON_BN_CLICKED(IDC_TM_LOADCASE_BTN,  OnCmdBtnLoadCase)
	ON_BN_CLICKED(IDC_TM_BTN_PROFILE,   OnCmdPressureProfile)  
	ON_CBN_SELCHANGE(IDC_TM_CODE_NAME,  OnSelchangeCmdCodeName)
	ON_EN_CHANGE(IDC_TM_ANGLE_EDT,	  	OnChangeAngle)
	ON_CBN_SELCHANGE(IDC_TM_DIRECTION_COMBO,   OnSelChangeDir)
	ON_CBN_SELCHANGE(IDC_TM_VELOCITY_NAME,   OnSelChangeVelocity)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ANGLE_SPIN, OnDeltaposCmdAngle)     //Span Num
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureBeamDlg message handlers

BOOL CStldWindPressureBeamDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
#ifdef _CH
	GetDlgItem(IDC_CHECK_CONVERT_TO_LOAD)->ShowWindow(SW_SHOW);
#endif
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.nTypeLaplLagr = 0;
	dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	ExportDisplaySetting::SET(&dspOpt); 

	m_pMgr->SetInit(TRUE);

	if(CWindLoadGeneratorCtrl::m_bSaved[WDPRTYPE_BEAM-1])
	{
	  m_Data = CWindLoadGeneratorCtrl::m_WdprLast[WDPRTYPE_BEAM-1];
	}

	InitCobxCodeName();
	InitCobxDirection();

	m_wndHolder.ShowWindow(SW_HIDE);
	m_cobxLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_edtAngle.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	int nCurSel = m_cobxCodeName.GetCurSel();
	int nCodeType = (int)m_cobxCodeName.GetItemData(nCurSel);
	m_cobxWvep.SetCodeType(nCodeType);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStldWindPressureBeamDlg::GetHolderRect( CRect* pRect )
{
	if(m_wndHolder.GetSafeHwnd() && ::IsWindow(m_wndHolder.GetSafeHwnd()))
	{
		m_wndHolder.GetWindowRect(pRect);
		return TRUE;
	}
	return FALSE;
}

void CStldWindPressureBeamDlg::Execute()
{
	if (!Dlg2Data())
		return;

	CArray<T_WDPR_D, T_WDPR_D&> DataSet;
	if (GetWdprAry(DataSet, FALSE))
	{
		if (!CheckWdpr(DataSet, TRUE))
			return;

		if (MITC_CommonTool::AddWdpr_Beam(DataSet, CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD)))
		{
			CWindLoadGeneratorCtrl::m_WdprLast[WDPRTYPE_BEAM - 1] = m_Data;
			CWindLoadGeneratorCtrl::m_bSaved[WDPRTYPE_BEAM - 1] = TRUE;
			CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
		}
	}
}

void CStldWindPressureBeamDlg::ChangeCodeDlg()
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

void CStldWindPressureBeamDlg::OnSelchangeCmdCodeName() 
{
	int nSel = m_cobxCodeName.GetCurSel();
	int nCodeIndex = CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName, nSel);

	m_Data.nCodeType = nCodeIndex;
	m_Data.CodeParam.Initialize(m_Data.nCodeType);
	Data2Dlg();

	m_cobxWvep.SetCodeType(nCodeIndex);

	ChangeCodeDlg();
}

void CStldWindPressureBeamDlg::InitCobxCodeName()
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
		if (m_aWlCodeEnable[0]==FALSE)
		{
			m_nOldSel = -1;
		}
		else
			m_Data.nCodeType = aCodeIndex[0];
#else
		m_Data.nCodeType = aCodeIndex[0];
		m_Data.CodeParam.Initialize(m_Data.nCodeType);
#endif
	}
}

void CStldWindPressureBeamDlg::GetMoveCtrlHolderID( CArray<UINT, UINT>& caCtrls, UINT& uiHolderID )
{
	caCtrls.RemoveAll();
	caCtrls.Add(IDC_TM_BTN_PROFILE);
		caCtrls.Add(IDC_CHECK_CONVERT_TO_LOAD);
	uiHolderID = IDC_TM_PLACE_HOLDER;
}

void CStldWindPressureBeamDlg::OnVelocityBtn()
{
	CCMWindPressureVelocityDefDlg dlg;
	dlg.DoModal();
}

void CStldWindPressureBeamDlg::OnCmdBtnLoadAreaGroup()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLdarGrupDefDlg::IDD);		
}

void CStldWindPressureBeamDlg::OnCmdBtnLoadCase()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldWindPressureBeamDlg::InitCobxDirection()
{
	m_cobxDirection.ResetContent();
	
//1=X-Y, 2=Normal, 3=local y+, 4=local y-, 5=local z+, 6=local z-
	int nIndex;
	CString strTemp;
	CStringArray strItem;
	strItem.Add(_T("X-Y"));
	strTemp.Format(_LS(IDS_TM_0524Local_y));
	strTemp += _T("+");
	strItem.Add(strTemp);
	strTemp.Format(_LS(IDS_TM_0524Local_y));
	strTemp += _T("-");
	strItem.Add(strTemp);
	strTemp.Format(_LS(IDS_TM_0524Local_z));
	strTemp += _T("+");
	strItem.Add(strTemp);
	strTemp.Format(_LS(IDS_TM_0524Local_z));
	strTemp += _T("-");
	strItem.Add(strTemp);
	int nNum = strItem.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		nIndex = m_cobxDirection.AddString(strItem[i]);
		m_cobxDirection.SetItemData(nIndex , i + 1);
	}
	m_cobxDirection.SetCurSel(0);  
}

void CStldWindPressureBeamDlg::OnDeltaposCmdAngle( NMHDR* pNMHDR, LRESULT* pResult )
{    
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_TM_ANGLE_EDT,-180,180,pNMHDR,pResult);
	
	m_Data.arPresProfile.RemoveAll();
	*pResult = 0;  
}

void CStldWindPressureBeamDlg::OnCmdPressureProfile()
{
	if(!Dlg2Data())
		return;
	
	int nStore;
	double dHeight, dWidth;
	BOOL bRetVal = CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(NULL, dHeight, nStore, dWidth);
	if(dHeight <= 1E-5 || bRetVal == FALSE)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_WINDP_NO_NODEINFO));
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
		BOOL bRetVal = GetWdprAry(DataSet, TRUE);
		
		if(bRetVal)
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
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

BOOL CStldWindPressureBeamDlg::Data2Dlg(BOOL bWarning)
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

	if(m_Data.nDirection == 1)
		m_cobxDirection.SetCurSel(0);
	else
	{
		m_cobxDirection.SetCurSel(m_Data.nDirection-2);
	}

	OnSelChangeDir();
	ChangeCodeDlg();
	//Code Dlg -> Data
	m_pMgr->Data2CodeDlg((void*)&m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);
	return TRUE;
}

BOOL CStldWindPressureBeamDlg::Dlg2Data(BOOL bWarning)
{
	//m_cobxLdgr.GetSelectedLdgr(m_Data.KeyLdgr);
	unsigned int nLoadCaseType;
	m_cobxLoadCase.GetSelectedLoad(nLoadCaseType, m_Data.KeyStld);
	if(nLoadCaseType != D_LOADCASE_STATIC || m_Data.KeyStld <= 0)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_STLD));
		return FALSE;
	}
	
	m_Data.nSelection = 2;
	m_Data.nType = WDPRTYPE_BEAM; //Beam
	m_Data.nDirection = GetDirection();
	
	m_Data.dAngle = m_edtAngle.GetEditValue();
	m_Data.dScaleFactor = m_edtScaleFactor.GetEditValue(); 
	m_Data.nCodeType = (int)CDlgUtil::CobxGetCurSelItemData(m_cobxCodeName,m_cobxCodeName.GetCurSel());
	m_cobxWvep.GetSelectedWvep(m_Data.KeyWvep);
	if(m_Data.KeyWvep <= 0)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_WVEP));
		return FALSE;
	}
	
	//Code Dlg -> Data
	if(!m_pMgr->CodeDlg2Data((void*) &m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL, TRUE))
	  return FALSE;

	CDBDoc::GetDocPoint()->m_pViewCtrl->GetAllSelectedElem(m_arElemK);

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
		if ((m_Data.CodeParam.KDS2021.bAutoForceCoef && !m_Data.CodeParam.KDS2021.bAlongWind) ||
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

void CStldWindPressureBeamDlg::OnSelChangeDir()
{
	if(GetDirection() == 1) //X-Y
	{
		GetDlgItem(IDC_TM_ANGLE_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_ANGLE_SPIN)->EnableWindow(TRUE);
	}
	else // xy가 아닌 local 좌표계 방향.
	{
		GetDlgItem(IDC_TM_ANGLE_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_ANGLE_SPIN)->EnableWindow(FALSE);
	}
	m_Data.arPresProfile.RemoveAll();
}

void CStldWindPressureBeamDlg::OnSelChangeVelocity()
{
	int nCurSel = 0;
	int nCodeType = (int)m_cobxCodeName.GetItemData(nCurSel);
	if(nCodeType != KBC_W_2016 && nCodeType != KDS_W_2019 && nCodeType != KDS_W_2022) return;
	m_pMgr->Data2CodeDlg((void*) &m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);
}

int CStldWindPressureBeamDlg::GetDirection()
{
	int nSel = m_cobxDirection.GetCurSel();
	if(nSel == 0)
		return 1; //X-Y
	else if(nSel == 1)
		return 3; //local y+
	else if(nSel == 2)
		return 4; //local y-
	else if(nSel == 3)
		return 5; //local z+
	else if(nSel == 4)
		return 6; //local z-

	m_cobxDirection.SetCurSel(0);
	return 1;
}

void CStldWindPressureBeamDlg::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
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

void CStldWindPressureBeamDlg::OnUnitChange()
{
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprPrevious(m_Data);
}

BOOL CStldWindPressureBeamDlg::CheckWdpr( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet, BOOL bMsg )
{
//   int i;
//   int nSize = rDataSet.GetSize();
//   T_LAPL_D LaplD;
//   double dWindVec[3];
// 
// 
//   for(i = 0 ; i < nSize; i++)
//   {
//     if(rDataSet[i].nSelction == 2) // 요소 직접 선택.
//     {
//       CWindLoadGeneratorCtrl myCtrl;
//       myCtrl.SetMode(WINDGEN_MODE_MANUAL);
// 
//       if(!myCtrl.SetSelectedElems2Valid(rDataSet[i].arKeysDefineElem))
//         return FALSE;
//       CArray<UINT, UINT> arSurfKey;
//       int nSurfCnt = myCtrl.GetSurfKeyList(arSurfKey);
// 
//       if(nSurfCnt != 1)
//         return FALSE;
//       LaplD.Initialize();
// 
//       if(!myCtrl.GetSurfInfo(arSurfKey[i], LaplD.arKeyNode, LaplD.arKeyElem))
//         return FALSE;
// 
//       if(!CWindLoadGeneratorCtrl::GetWindDirlVecByLapl(rDataSet[i], LaplD, dWindVec)) // 풍향을 구할 수 있는가?
//         return FALSE;
//     }
//     else if(rDataSet[i].nSelction == 1)
//     {
//       if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetLapl(rDataSet[i].KeyLapl, LaplD))
//         return FALSE;
//       
//       if(!CWindLoadGeneratorCtrl::GetWindDirlVecByLapl(rDataSet[i], LaplD, dWindVec)) // 풍향을 구할 수 있는가?
//         return FALSE;
//     }
//   }
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

BOOL CStldWindPressureBeamDlg::GetWdprAry( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet , BOOL bProfile)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CLateralLoad lateralLoad;
	
	int i;
	int nSize = m_arElemK.GetSize();
	rDataSet.SetSize(nSize);
	m_Data.arKeysDefineElem.SetSize(1);
	for(i = 0 ; i < nSize; i++)
	{
		m_Data.arKeysDefineElem[0] = m_arElemK[i];
		rDataSet[i] = m_Data;
	}
	if(nSize <= 0)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return FALSE;
	}

	BOOL bCheck = TRUE;
#ifdef _MGEN_CH
	bCheck = CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD) == FALSE;
#endif
	if(bCheck || bProfile)
	{
			CLateralLoad laterLoad;
			nSize = rDataSet.GetSize();
			for(int i = nSize - 1 ; i >= 0; i--)
			{
					if(!laterLoad.MakeWindPressure(rDataSet[i]))
							rDataSet.RemoveAt(i);

			}
	}
	return TRUE;
}

void CStldWindPressureBeamDlg::OnChangeAngle()
{
	m_Data.arPresProfile.RemoveAll();
}
