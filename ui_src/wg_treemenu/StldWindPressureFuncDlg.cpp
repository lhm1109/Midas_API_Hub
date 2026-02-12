// CMGridImpactFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureFuncDlg.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\ExportFunc.h"
#include "..\wg_cmd\CMLoadGroupDlg.h"
#include "..\wg_cmd\CMLdarGrupDefDlg.h"
#include "..\wg_cmd\CMWindPressureVelocityDefDlg.h"
#include "..\wg_cmd\CMWindPressureFunctionDefDlg.h"
#include "..\wg_cmd\CMWindPressureProfileDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
//#include "..\wg_base\PL_Parser.h"

#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_gr\GDisplaySetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureFuncDlg dialog

#define ELEM_FRAME  1
#define ELEM_PLANAR 2

CStldWindPressureFuncDlg::CStldWindPressureFuncDlg(CWnd* pParent /*=NULL*/)
	: CCMWindPressureBaseDlg(CStldWindPressureFuncDlg::IDD)
{
	m_pRootDlg = (CCMWindPressureRootDlg*)pParent;

	m_pMgr = NULL;
	//m_pMgr = new CStldWindPressureAreaMgr(this, (void*)&m_Data);
	m_Data.Initialize();
	m_LagrK = 0;

	
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprOut(m_Data);

	m_nSelectionType = 1;

	m_aCtrlOption.RemoveAll();
	m_aCtrlOption.Add(IDC_TM_FCWD_OPT_ADD_REP_RDO);
	m_aCtrlOption.Add(IDC_TM_FCWD_OPT_DEL_RDO);
}

CStldWindPressureFuncDlg::~CStldWindPressureFuncDlg()
{
//   if (m_pMgr != NULL)
//   {
//     delete m_pMgr;
//     m_pMgr = NULL;
//   }  

	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.nTypeLaplLagr = 0;
	dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	ExportDisplaySetting::SET(&dspOpt); 
}

void CStldWindPressureFuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TM_LOADCASE,        m_cobxLoadCase);
	DDX_Control(pDX, IDC_TM_DIRECTION_COMBO, m_cobxDirection);
	DDX_Control(pDX, IDC_TM_FUNC_CBX,        m_cobxFunction);
	DDX_Control(pDX, IDC_TM_LAGR_NAME,       m_cobxLagr); 
	DDX_Control(pDX, IDC_TM_ELEM_DEFINE,     m_edElemList);  
	DDX_Control(pDX, IDC_TM_ANGLE_EDT,       m_edtAngle);
	DDX_Control(pDX, IDC_TM_SCALEFACTOR,     m_edtScaleFactor);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_EDIT,  m_wndCoordXYZ);
	DDX_Control(pDX, IDC_TM_PLANE_PT1_UNIT,  m_wndCoordUnit);
	DDX_Control(pDX, IDC_TM_PLANE_PT2_EDIT,  m_wndCenterXYZ);
	DDX_Control(pDX, IDC_TM_PLANE_PT2_UNIT,  m_wndCenterUnit);
}


BEGIN_MESSAGE_MAP(CStldWindPressureFuncDlg, CCMChildBarBase)
	ON_BN_CLICKED(IDC_TM_LOADCASE_BTN,  OnCmdBtnLoadCase)
	ON_BN_CLICKED(IDC_TM_TARGET_RDO1,   OnCmdTargetChanged)  
	ON_BN_CLICKED(IDC_TM_TARGET_RDO2,   OnCmdTargetChanged)  
	ON_BN_CLICKED(IDC_TM_LAGR_BTN,      OnCmdBtnLoadAreaGroup)
	ON_BN_CLICKED(IDC_TM_FUNC_BTN,      OnCmdFunctionBtn)
	ON_CBN_SELCHANGE(IDC_TM_LAGR_NAME,  OnSelChangeLagr)
	ON_CBN_SELCHANGE(IDC_TM_FUNC_CBX,   OnSelChangeFunc)
	ON_CBN_SELCHANGE(IDC_TM_DIRECTION_COMBO,   OnSelChangeDir)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ANGLE_SPIN, OnDeltaposCmdAngle)     //Span Num
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureFuncDlg message handlers

BOOL CStldWindPressureFuncDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);

	//m_pMgr->SetInit(TRUE);

	if(CWindLoadGeneratorCtrl::m_bSaved[WDPRTYPE_FUNC-1])
	{
	  m_Data = CWindLoadGeneratorCtrl::m_WdprLast[WDPRTYPE_FUNC-1];
	}

	InitCobxDirection();

	m_edElemList.Connect(SC_ID_ELEM, &m_edElemList);	
	m_cobxLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_edtAngle.SetUnitType(D_UNITSYS_NONE);
	m_edtScaleFactor.SetUnitType(D_UNITSYS_NONE);

	m_wndCoordUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCoordXYZ.SetAttUcsPos();
	m_wndCoordXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);  
	m_wndCoordXYZ.SetNextLink(NULL);

	m_wndCenterUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndCenterXYZ.SetAttUcsPos();
	m_wndCenterXYZ.SetModeToUse(MOUSEEDIT_USE_SET_POS);  
	m_wndCenterXYZ.SetNextLink(NULL);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, 0);

	GetElementType();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStldWindPressureFuncDlg::GetHolderRect( CRect* pRect )
{
	return FALSE;
}

void CStldWindPressureFuncDlg::Execute()
{
	if (!Dlg2Data())
		return;

	int nOption = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	CArray<T_WDPR_D, T_WDPR_D&> DataSet;
	if (GetWdprAry(DataSet))
	{
		if (!CheckWdpr(DataSet, TRUE))
			return;

		CArray<T_WDPR_K, T_WDPR_K> aWdprK;
		if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWdprKeyListByData(DataSet, aWdprK) == 0) { ASSERT(0); return; }

		BOOL bResult = FALSE;

		if (nOption == 0)
		{
			bResult = CDBDoc::GetDocPoint()->m_pDataCtrl->AddWdpr(aWdprK, DataSet);
		}
		else
		{
			for (int i = aWdprK.GetSize() - 1; i > -1; i--)
			{
				if (aWdprK[i] == 0) aWdprK.RemoveAt(i);
			}
			bResult = CDBDoc::GetDocPoint()->m_pDataCtrl->DelWdpr(aWdprK);
		}

		if (bResult)
		{
			CWindLoadGeneratorCtrl::SetWdprSave(WDPRTYPE_FUNC, m_Data);
			CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
		}
	}
}

void CStldWindPressureFuncDlg::GetMoveCtrlHolderID( CArray<UINT, UINT>& caCtrls, UINT& uiHolderID )
{
	caCtrls.RemoveAll();
}

// void CStldWindPressureFuncDlg::OnCmdBtnLoadGroup()
// {
//   CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLoadGroupDlg::IDD);		  
// }

void CStldWindPressureFuncDlg::OnCmdBtnLoadAreaGroup()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLdarGrupDefDlg::IDD);		
}

void CStldWindPressureFuncDlg::OnCmdBtnLoadCase()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldWindPressureFuncDlg::InitCobxDirection()
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

void CStldWindPressureFuncDlg::OnDeltaposCmdAngle( NMHDR* pNMHDR, LRESULT* pResult )
{    
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_TM_ANGLE_EDT,-180,180,pNMHDR,pResult);
	
	*pResult = 0;  
}

void CStldWindPressureFuncDlg::OnCmdTargetChanged()
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

int CStldWindPressureFuncDlg::GetSelectionType()
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

BOOL CStldWindPressureFuncDlg::Data2Dlg(BOOL bWarning)
{
	//m_cobxLdgr.ChangeSelect(m_Data.KeyLdgr);
	if(m_Data.KeyStld)
		m_cobxLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Data.KeyStld);

	if(m_Data.nDirection == 2) m_cobxDirection.SetCurSel(1);
	else                       m_cobxDirection.SetCurSel(0);
	
	CString strPos;
	strPos.Format(_T("%g, %g, %g"), m_Data.dInnerPnt[0], m_Data.dInnerPnt[1], m_Data.dInnerPnt[2]);
	m_wndCoordXYZ.SetWindowText(strPos);
	strPos.Format(_T("%g, %g, %g"), m_Data.dCenterPnt[0], m_Data.dCenterPnt[1], m_Data.dCenterPnt[2]);
	m_wndCenterXYZ.SetWindowText(strPos);

	m_edtAngle.SetEditUnit(m_Data.dAngle);
	m_edtScaleFactor.SetEditUnit(m_Data.dScaleFactor); 
	if(m_Data.KeyWdfc > 0)
		m_cobxFunction.ChangeSelect(m_Data.KeyWdfc);
	
	//Code Dlg -> Data
	//m_pMgr->Data2CodeDlg((void*) &m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL);

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
	OnSelChangeLagr();
	OnSelChangeFunc();

	return TRUE;
}

BOOL CStldWindPressureFuncDlg::Dlg2Data(BOOL bWarning)
{
	//m_cobxLdgr.GetSelectedLdgr(m_Data.KeyLdgr);
	unsigned int nLoadCaseType;
	m_cobxLoadCase.GetSelectedLoad(nLoadCaseType, m_Data.KeyStld);
	if(nLoadCaseType != D_LOADCASE_STATIC || m_Data.KeyStld <= 0)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_STLD));
		return FALSE;
	}
	
	CString strPos;
	MyParser Parser;

	m_Data.nType = WDPRTYPE_FUNC; //Function...
	m_Data.nDirection = GetDirection();
	if(m_Data.nDirection == 1)
		m_Data.dAngle = m_edtAngle.GetEditValue();
	else if(m_Data.nDirection == 2)
	{
		m_wndCoordXYZ.GetWindowText(strPos);
		if(!Parser.ParsingPositionArg(strPos)) return FALSE;
		if(!Parser.GetPositionData(m_Data.dInnerPnt[0],m_Data.dInnerPnt[1],m_Data.dInnerPnt[2])) return FALSE;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
	
	m_wndCenterXYZ.GetWindowText(strPos);
	if(!Parser.ParsingPositionArg(strPos)) return FALSE;
	if(!Parser.GetPositionData(m_Data.dCenterPnt[0],m_Data.dCenterPnt[1],m_Data.dCenterPnt[2])) return FALSE;

	m_Data.dScaleFactor = m_edtScaleFactor.GetEditValue(); 
	m_cobxFunction.GetSelectedWdfc(m_Data.KeyWdfc);
	
	//Code Dlg -> Data
//   if(!m_pMgr->CodeDlg2Data((void*) &m_Data, EN_WIND_PRESSURE_CODE_TYPE_ALL))
//     return FALSE;

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

	return TRUE;
}

void CStldWindPressureFuncDlg::OnSelChangeLagr()
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

void CStldWindPressureFuncDlg::OnSelChangeDir()
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
}

int CStldWindPressureFuncDlg::GetDirection()
{
	int nSel = m_cobxDirection.GetCurSel();
	if(nSel == 0)
		return 1; //X-Y
	else if(nSel == 1)
		return 2; //Normal

	m_cobxDirection.SetCurSel(0);
	return 1;
}

void CStldWindPressureFuncDlg::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
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

void CStldWindPressureFuncDlg::OnUnitChange()
{
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitWdprPrevious(m_Data);
}

BOOL CStldWindPressureFuncDlg::CheckWdpr( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet, BOOL bMsg )
{
	if(m_Data.nSelection != 1 && GetElementType()==ELEM_PLANAR)
	{
		for(int i=0; i<rDataSet.GetSize(); i++)
		{
			CMap<T_ELEM_K,T_ELEM_K,T_PRES_D,T_PRES_D&> mapLoad;
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcWindPressure_Face(rDataSet[i], mapLoad))
				return FALSE;
		}
	}
	else
	{
		CMap<T_NODE_K,T_NODE_K,T_CNLD_D,T_CNLD_D&> mapLoad;
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->CalcWindPressure_Nodal(rDataSet, mapLoad, NULL, TRUE))
			return FALSE;
	}

	return TRUE;
}

BOOL CStldWindPressureFuncDlg::GetWdprAry( CArray<T_WDPR_D, T_WDPR_D&>& rDataSet )
{
	int nSelection = m_Data.nSelection;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_LAPL_K, T_LAPL_K> KeyLapl;
	int i, nSize;
	
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
		int nElemType = GetElementType();
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
	return TRUE;
}

void CStldWindPressureFuncDlg::OnCmdFunctionBtn()
{
	CCMWindPressureFunctionDefDlg dlg;
	dlg.DoModal();  
}

void CStldWindPressureFuncDlg::OnSelChangeFunc()
{
	T_WDFC_K WdfcK;
	T_WDFC_D WdfcD;
	if(m_cobxFunction.GetSelectedWdfc(WdfcK))
	{    
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetWdfc(WdfcK, WdfcD))
		{
			if(WdfcD.nCoordType == 1) //직교
			{
				m_wndCenterXYZ.EnableWindow(FALSE);
				m_wndCenterUnit.EnableWindow(FALSE);
			}
			else if(WdfcD.nCoordType == 2) //원통
			{
				m_wndCenterXYZ.EnableWindow(TRUE);
				m_wndCenterUnit.EnableWindow(TRUE);        
			}
		}
	}
	else
	{
		m_wndCenterXYZ.EnableWindow(FALSE);
		m_wndCenterUnit.EnableWindow(FALSE);
	}
}

int CStldWindPressureFuncDlg::GetElementType()
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