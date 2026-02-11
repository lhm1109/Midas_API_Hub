// CMNlacItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNlacItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "CMLoadCaseDlg.h"
#include "ExportFunc.h"

#include "CMNlacDlg.h"
#include "CMNlacItemGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMNlacItemDlg dialog

CCMNlacItemDlg::CCMNlacItemDlg(CWnd* pParent /*=NULL*/) : CCMDlgBase(CCMNlacItemDlg::IDD, pParent)
{
	m_pParentWnd = reinterpret_cast<CCMNlacDlg*>(pParent);

	m_pDoc = 0;
	m_bModify = FALSE;  
	m_nMethod = 0;
	m_nInterMethodRdo = 0;
	m_OriginKeyStld = 0;
	//{{AFX_DATA_INIT(CCMNlacItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT  
	m_wndGrid = NULL;
	m_wndGrid = new CCMNlacItemGrid();  

	m_DataNewton.Initialize();
	m_DataArclen.Initialize();
	m_DataDispct.Initialize();
}

CCMNlacItemDlg::~CCMNlacItemDlg()
{
	if (m_wndGrid != NULL)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CCMNlacItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMNlacItemDlg)
	DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_LOAD_CASE_CMB, m_NewtonLoadCaseCmb);
	DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_MAX_ED, m_maximum);
	DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_NUM_ED, m_number);	
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_LOAD_CASE_CMB, m_ArclenLoadCaseCmb);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXITER_ED, m_maxiter);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXDISP_ED, m_maxdisp);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXDISP_UNIT, m_maxdispunit);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_MAXINCR_ED, m_maxincr);
	DDX_Control(pDX, IDC_CMD_NLAC_ARCLENG_INIT_ED, m_initforce);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_LOAD_CASE_CMB, m_DispctLoadCaseCmb);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXITER_ED, m_DispCtrlMaxIter);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXDISP_ED, m_DispCtrlMaxDisp);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MAXDISP_UNIT, m_DispCtrlMaxDispUnit);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_DISPSTEP_ED, m_DispCtrlDispStep);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_MASTER_NODE_ED, m_DispCtrlMasterNode);
	DDX_Control(pDX, IDC_CMD_NLAC_DISPCTRL_DIRECTION_CMB, m_DispCtrlDirectionCmb);
	DDX_Radio  (pDX, IDC_CMD_NLAC_NEWTON_RDO, m_nInterMethodRdo);  
	DDX_Control(pDX, IDC_CMD_NLAC_NEWTON_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMNlacItemDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMNlacItemDlg)
	ON_BN_CLICKED(IDC_CMD_NLAC_NEWTON_GEN_FACTOR_BTN, OnNewtonGenBtn)
	ON_BN_CLICKED(IDC_CMD_NLAC_DISPCTRL_GEN_DISP_BTN, OnDispCtrlGenBtn)
	ON_BN_CLICKED(IDC_CMD_NLAC_NEWTON_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_EN_CHANGE(IDC_CMD_NLAC_NEWTON_NUM_ED, OnChangeCmdNlacNewtonNumEd)
	ON_BN_CLICKED(IDC_CMD_NLAC_DISPCTRL_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_NLAC_ARCLENG_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_EN_CHANGE(IDC_CMD_NLAC_DISPCTRL_DISPSTEP_ED, OnChangeCmdNlacDispctrlDispstepEd)
	ON_EN_CHANGE(IDC_CMD_NLAC_DISPCTRL_MAXDISP_ED, OnChangeCmdNlacDispctrlMaxdispEd)
	ON_BN_CLICKED(IDC_CMD_NLAC_NEWTON_RDO, OnMethodRdo) 
	ON_BN_CLICKED(IDC_CMD_NLAC_DISPL_RDO , OnMethodRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMNlacItemDlg message handlers

BOOL CCMNlacItemDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CCMDlgBase::OnInitDialog(); 

	switch(m_nMethod)
	{
	case 0:
		m_nInterMethodRdo = 0;
		break;
	case 2:
		m_nInterMethodRdo = 1;
		break;
	default:
		m_nInterMethodRdo = 0;
		break;
	}

	if (m_nMethod == 0) m_wndGrid->SetOption(0);
	else if(m_nMethod == 2) m_wndGrid->SetOption(1);
	m_wndGrid->Initialize();

	m_aCtrlNewton.RemoveAll();
	m_aCtrlArcLeng.RemoveAll();
	m_aCtrlDispCtrl.RemoveAll();  
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlNewton,   IDC_CMD_NLAC_NEWTON_FRM, TRUE);  
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlArcLeng,  IDC_CMD_NLAC_ARCLENG_FRM, TRUE);	
	CDlgUtil::GetCtrlIDByIncRect(this, m_aCtrlDispCtrl, IDC_CMD_NLAC_DISPCTRL_FRM, TRUE);

	// initialize /////////////////
	m_maximum.SetRange(1, 100);
	m_maximum.SetInteger(TRUE);
	m_number.SetRange(1, 100);
	m_number.SetInteger(TRUE);
	m_maxiter.SetRange(1, 100);
	m_maxiter.SetInteger(TRUE);
	m_maxincr.SetRange(1, 100);
	m_maxincr.SetInteger(TRUE);
	m_DispCtrlDispStep.SetRange(1, 100);
	m_DispCtrlDispStep.SetInteger(TRUE);
	m_DispCtrlMaxIter.SetRange(1, 100);
	m_DispCtrlMaxIter.SetInteger(TRUE);

	m_DispCtrlMasterNode.SetAttNodeList();
	m_DispCtrlMasterNode.SetLButtonDownNotifyWindow(this);
	m_DispCtrlMasterNode.SetEnterNotifyWindow(this);
	m_DispCtrlMasterNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_DispCtrlMasterNode.SetMaxNodeKeyNum(1);

	SetInitData();
	Data2Dlg();
	m_maxdispunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DispCtrlMaxDispUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	///////////////////////////////
	UpdateData(FALSE);
	
	AlignControls();
	CtrlManager();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMNlacItemDlg::AlignControls()
{  
	CRect rRef;
	CRect rToMove;
	int nDistX=0,nDistY=0;

	if(m_nMethod==0 || m_nMethod==2)
	{
		CArray<UINT, UINT> aCtrlMethod;
		aCtrlMethod.Add(IDC_CMD_NLAC_INTERMETHOD_FRM);
		aCtrlMethod.Add(IDC_CMD_NLAC_NEWTON_RDO);
		aCtrlMethod.Add(IDC_CMD_NLAC_DISPL_RDO);

		GetDlgItem(IDC_CMD_NLAC_NEWTON_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_NLAC_INTERMETHOD_FRM)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, aCtrlMethod, nDistX, nDistY);

		GetDlgItem(IDC_CMD_NLAC_INTERMETHOD_FRM)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_NLAC_NEWTON_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlNewton, nDistY);

		CArray<UINT, UINT> aCtrlOkCancel;
		aCtrlOkCancel.Add(IDOK);
		aCtrlOkCancel.Add(IDCANCEL);
		CDlgUtil::CtrlMoveDistY(this, aCtrlOkCancel, nDistY);
	}
	
	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlArcLeng, &m_aCtrlDispCtrl};
	UINT aFirstCtrlID[] = {IDC_CMD_NLAC_ARCLENG_FRM, IDC_CMD_NLAC_DISPCTRL_FRM};
	
	GetDlgItem(IDC_CMD_NLAC_NEWTON_FRM)->GetWindowRect(rRef);
	for(int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);    
	}

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);

	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CCMNlacItemDlg::SetInitData()
{  
	m_DispCtrlDirectionCmb.ResetContent();
	m_DispCtrlDirectionCmb.AddString(_T("Dx"));
	m_DispCtrlDirectionCmb.AddString(_T("Dy"));
	m_DispCtrlDirectionCmb.AddString(_T("Dz"));
	m_DispCtrlDirectionCmb.SetCurSel(0);

	m_NewtonLoadCaseCmb.SetIgnoreStldCSLoad(TRUE);
	m_NewtonLoadCaseCmb.SetLoadType(D_LOADCASE_STATIC, TRUE);

	m_ArclenLoadCaseCmb.SetIgnoreStldCSLoad(TRUE);
	m_ArclenLoadCaseCmb.SetLoadType(D_LOADCASE_STATIC, TRUE);

	m_DispctLoadCaseCmb.SetIgnoreStldCSLoad(TRUE);
	m_DispctLoadCaseCmb.SetLoadType(D_LOADCASE_STATIC, TRUE);
}

void CCMNlacItemDlg::CtrlManager()
{
	CDlgUtil::CtrlShowHide(this, m_aCtrlNewton, m_nMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlArcLeng, m_nMethod == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlDispCtrl, m_nMethod == 2);

	CWnd* pWnd = GetDlgItem(IDC_CMD_NLAC_NEWTON_GRID);
	ASSERT(pWnd);
	if (m_nMethod == 0 || m_nMethod == 2) 
		pWnd->ShowWindow(SW_SHOW);
	else 
		pWnd->ShowWindow(SW_HIDE);
}

void CCMNlacItemDlg::Data2Dlg()
{
	// Newton-Raphson
	m_NewtonLoadCaseCmb.ChangeSelect(D_LOADCASE_STATIC, m_DataNewton.StldKey);
	m_number.SetValue(m_DataNewton.nLoadStep);
	m_maximum.SetValue(m_DataNewton.nMaxIterPerLoadStep);

	// Arc-Length
	m_ArclenLoadCaseCmb.ChangeSelect(D_LOADCASE_STATIC, m_DataArclen.StldKey);
	m_maxincr.SetValue(m_DataArclen.nMaxIncStep);
	m_maxiter.SetValue(m_DataArclen.nMaxIterIncStep);
	m_initforce.SetEditUnit(m_DataArclen.dIniForceRatio);
	m_maxdisp.SetEditUnit(m_DataArclen.dMaxDisplacement);

	// Displacement-Control
	m_DispctLoadCaseCmb.ChangeSelect(D_LOADCASE_STATIC, m_DataDispct.StldKey);
	m_DispCtrlDispStep.SetValue(m_DataDispct.nMumDispStep);
	m_DispCtrlMaxIter.SetValue(m_DataDispct.nMaxIterIncStep);
	m_DispCtrlMasterNode.SetKeyPressedFlag(FALSE);
	m_DispCtrlMasterNode.AddNodeKey(m_DataDispct.MasterNodeKey);
	m_DispCtrlDirectionCmb.SetCurSel(m_DataDispct.nDirection);
	m_DispCtrlMaxDisp.SetEditUnit(m_DataDispct.dMaxDisplacement);	  

	if(m_nMethod == 0)
	{
		m_OriginKeyStld = m_DataNewton.StldKey;
		
		m_wndGrid->SetMaxDispValue(1);
		m_wndGrid->SetRowCountUser(m_DataNewton.nLoadStep);
		
		if (m_bModify)
		{
			int nCount = m_DataNewton.arLoadFactor.GetSize();
			ASSERT(nCount == m_DataNewton.nLoadStep);
			for (int i=0; i<nCount; i++)
				m_wndGrid->SetValueRow( i+1, m_DataNewton.arLoadFactor[i].dValue);
		}
	}
	else if(m_nMethod == 1)
	{
		m_OriginKeyStld = m_DataArclen.StldKey;
	}
	else if(m_nMethod == 2)
	{
		m_OriginKeyStld = m_DataDispct.StldKey;

		m_wndGrid->SetMaxDispValue(m_DataDispct.dMaxDisplacement);
		m_wndGrid->SetRowCountUser(m_DataDispct.nMumDispStep);

		if (m_bModify)
		{
			int nCount = m_DataDispct.arNodeDisp.GetSize();
			ASSERT(nCount == m_DataDispct.nMumDispStep);

			for (int i=0; i<nCount; i++)
				m_wndGrid->SetValueRow(i+1, m_DataDispct.arNodeDisp[i].dValue);
		}
	}
}

void CCMNlacItemDlg::OnNewtonGenBtn()
{
	int nStep = m_number.GetEditValueInt();
	double dValue = 1. / nStep;
	
	m_wndGrid->GenerateValue(dValue);
}

void CCMNlacItemDlg::OnDispCtrlGenBtn()
{
	int nStep = m_DispCtrlDispStep.GetEditValueInt();
	double dMaxDisp = m_DispCtrlMaxDisp.GetEditValue();
	double dValue = dMaxDisp / nStep;

	m_wndGrid->GenerateValue(dValue);
}

BOOL CCMNlacItemDlg::Dlg2Data()
{
	UpdateData();    
	CString strLoadCaseName;
	UINT nLoadCaseType;
	T_STLD_K KeyStld;
	T_NLCT_VALUE_BASE BaseValue;
	BaseValue.Initialize();
		
	if(m_nMethod == 0)  // Newton-Raphson
	{
		m_DataNewton.Initialize();    
		if(!m_NewtonLoadCaseCmb.GetSelectedLoad(nLoadCaseType, KeyStld))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
			return FALSE;
		}    
		m_DataNewton.StldKey = KeyStld;
		m_DataNewton.nLoadStep = m_number.GetEditValueInt();
		m_DataNewton.nMaxIterPerLoadStep = m_maximum.GetEditValueInt();
		for(int i = 0; i < m_DataNewton.nLoadStep; i++)   
		{
			BaseValue.dValue = m_wndGrid->GetValueRow(i+1);
			m_DataNewton.arLoadFactor.Add(BaseValue);
		}
		if(!CheckDataNewton(m_DataNewton)) return FALSE;
	}
	else if(m_nMethod == 1)  // Arc-Length
	{
	  m_DataArclen.Initialize();          
		if(!m_NewtonLoadCaseCmb.GetSelectedLoad(nLoadCaseType, KeyStld))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
			return FALSE;
		}    
		m_DataArclen.StldKey = KeyStld;
		m_DataArclen.nMaxIncStep      = m_maxincr.GetEditValueInt();
		m_DataArclen.nMaxIterIncStep  = m_maxiter.GetEditValueInt();
		m_DataArclen.dIniForceRatio   = m_initforce.GetEditValue();
		m_DataArclen.dMaxDisplacement = m_maxdisp.GetEditValue();
		if(!CheckDataArclen(m_DataArclen)) return FALSE;
	}
	else if(m_nMethod == 2)  // Displacement-Control
	{
		m_DataDispct.Initialize();
	  if(!m_NewtonLoadCaseCmb.GetSelectedLoad(nLoadCaseType, KeyStld))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
			return FALSE;
		}    
		m_DataDispct.StldKey = KeyStld;
		m_DataDispct.nMumDispStep = m_DispCtrlDispStep.GetEditValueInt();
		m_DataDispct.nMaxIterIncStep = m_DispCtrlMaxIter.GetEditValueInt();

		int nMasterNodeKey = 0;
		CFormulaEdit::GetEditValue(&m_DispCtrlMasterNode, nMasterNodeKey);
		m_DataDispct.MasterNodeKey = nMasterNodeKey;

		m_DataDispct.nDirection = m_DispCtrlDirectionCmb.GetCurSel();
		m_DataDispct.dMaxDisplacement = m_DispCtrlMaxDisp.GetEditValue();
		for(int i = 0; i < m_DataDispct.nMumDispStep; i++)   // Table의 값으로 수정요망
		{
			BaseValue.dValue = m_wndGrid->GetValueRow(i+1);
			m_DataDispct.arNodeDisp.Add(BaseValue);
		}
		if(!CheckDataDispct(m_DataDispct)) return FALSE;
	}

	return TRUE;
}

BOOL CCMNlacItemDlg::CheckDataNewton(T_NLCT_NEWTON DataNewton)
{
	for(int i = 0; i < m_arNewton.GetSize(); i++)
	{// Stld Key가 변경되었고, 이 변경된 Key에 해당하는 Data가 있는 경우 Error 처리
		if(DataNewton.StldKey != m_OriginKeyStld && DataNewton.StldKey == m_arNewton[i].StldKey)
		{
			GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DUPLICATE_LOAD_CASE)); // _T("[오류] 중복된 Load Case 입니다.")
			return FALSE;
		}
	}
	for(int i = 0; i < m_arDispct.GetSize(); i++)
	{
		if(DataNewton.StldKey == m_arDispct[i].StldKey)
		{
			if(!m_bModify)
			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DUPLICATE_LOAD_CASE_IN_DISPL));
				return FALSE;
			}
			if(DataNewton.StldKey != m_OriginKeyStld)
			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DUPLICATE_LOAD_CASE)); // _T("[오류] 중복된 Load Case 입니다.")
				return FALSE;
			}
		}
	}
	if(DataNewton.nLoadStep < 1) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_LOAD_STEP)); // _T("[오류] Load Step은 1보다 크거나 같아야 합니다.")
		return FALSE;
	}   
	if(DataNewton.nMaxIterPerLoadStep < 1 || DataNewton.nMaxIterPerLoadStep > 1000) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_ITER_PER_LOADSTEP)); // _T("[오류] Maximum Number of Iterations/Load Step은 1보다 크거나 같고, 1000보다 작거나 같아야 합니다.")
		return FALSE;
	}
	if(DataNewton.nLoadStep && DataNewton.arLoadFactor[0].dValue == 0.0)
	{
		CString strMsg;
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_NONE_ZERO), _LS(IDS_CMD_LOAD_FACTOR_1ST));  // Error! Load Factor of first step cannot be zero.
		GSaveHistoryFormatNF(strMsg);
		return FALSE;
	}
	for(int i = 0; i < DataNewton.nLoadStep - 1; i++)
	{
		if(DataNewton.arLoadFactor[i].dValue == DataNewton.arLoadFactor[i+1].dValue) 
		{
			GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_LOAD_FACTOR));  // Error! Load Factors of neighboring steps must not be same value.
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCMNlacItemDlg::CheckDataArclen(T_NLCT_ARCLEN DataArclen)
{
	for(int i = 0; i < m_arArclen.GetSize(); i++)
	{// Stld Key가 변경되었고, 이 변경된 Key에 해당하는 Data가 있는 경우 Error 처리
		if(DataArclen.StldKey != m_OriginKeyStld && DataArclen.StldKey == m_arArclen[i].StldKey)
		{
			GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DUPLICATE_LOAD_CASE)); // _T("[오류] 중복된 Load Case 입니다.")
			return FALSE;
		}
	}
	if(DataArclen.nMaxIncStep < 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_INCREMENT_STEP)); // _T("[오류] Increment Step은 0보다 크거나 같아야 합니다.")
		return FALSE;
	}   
	if(DataArclen.nMaxIterIncStep < 0) 
	{    
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_ITER_PER_INCSTEP));  // _T("Error! Maximum Number of Iterations/Increment Step은 0보다 크거나 같아야 합니다.")
		return FALSE;
	}   
	if(DataArclen.dIniForceRatio < 0.0 || DataArclen.dIniForceRatio > 100.0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_INIT_FORCE_RATIO)); // _T("[오류] Initial Force Ratio for Unit Arc-Length는 0보다 크거나 같고, 100보다 작거나 같아야 합니다.")
		return FALSE;
	}
	if(DataArclen.dMaxDisplacement < 0) 
	{    
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_MAX_DISP_BOUNDARY));  // _T("Error! Maximum Displacement Boundary는 0보다 크거나 같아야 합니다.")
		return FALSE;
	}   
	return TRUE;
}

BOOL CCMNlacItemDlg::CheckDataDispct(T_NLCT_DISPCT DataDispct)
{
	for(int i = 0; i < m_arDispct.GetSize(); i++)
	{// Stld Key가 변경되었고, 이 변경된 Key에 해당하는 Data가 있는 경우 Error 처리
		if(DataDispct.StldKey != m_OriginKeyStld && DataDispct.StldKey == m_arDispct[i].StldKey)
		{
			GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DUPLICATE_LOAD_CASE)); // _T("[오류] 중복된 Load Case 입니다.")
			return FALSE;
		}
	}
	for(int i = 0; i < m_arNewton.GetSize(); i++)
	{
		if(DataDispct.StldKey==m_arNewton[i].StldKey)
		{
			if(!m_bModify)
			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DUPLICATE_LOAD_CASE_IN_NEWTON));
				return FALSE;
			}
			if(DataDispct.StldKey != m_OriginKeyStld)
			{
				GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DUPLICATE_LOAD_CASE)); // _T("[오류] 중복된 Load Case 입니다.")
				return FALSE;
			}
		}
	}
	if(DataDispct.nMumDispStep < 1) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DISPLACEMENT_STEP));  // _T("[오류] Displacement Step은 1보다 크거나 같아야 합니다.")); // 
		return FALSE;
	}   
	if(DataDispct.nMaxIterIncStep < 1) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_ITER_PER_INCSTEP2)); // _T("[오류] Maximum Number of Iterations/Increment Step은 1보다 크거나 같아야 합니다.")); // 
		return FALSE;
	}
	if(!m_pDoc->m_pAttrCtrl->ExistNode(DataDispct.MasterNodeKey)) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_NON_EXIST_NODE)); // _T("[오류] 입력된 Master Node는 존재하지 않는 절점입니다.")); // 
		return FALSE;
	}
	if(DataDispct.nDirection < 0 || DataDispct.nDirection > 2) 
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_DIRECTION)); // _T("[오류] Direction")); // 
		return FALSE;
	}
	if(DataDispct.dMaxDisplacement == 0.0) 
	{
		CString strMsg;
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_NONE_ZERO), _LS(IDS_CMD_MAX_DISPLACEMENT));  // Error! Maximum Displacement cannot be zero.
		GSaveHistoryFormatNF(strMsg);    
		return FALSE;
	}
	if(DataDispct.nMumDispStep && DataDispct.arNodeDisp[0].dValue == 0.0)
	{
		CString strMsg;
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_NONE_ZERO), _LS(IDS_CMD_MASTER_NODE_DISPLACEMENT_1ST));  // Error! Master Node Displacement of first step cannot be zero.
		GSaveHistoryFormatNF(strMsg);          
		return FALSE;
	}
	for(int i = 0; i < DataDispct.nMumDispStep - 1; i++)
	{
		if(DataDispct.arNodeDisp[i].dValue == DataDispct.arNodeDisp[i+1].dValue) 
		{      
			GSaveHistoryFormatNF(_LS(IDS_CMD_ERROR_MASTER_NODE_DISPLACEMENT));   // Error! Master Node Displacements of neighboring steps must not be same value.
			return FALSE;
		}
	}
	return TRUE;
}

void CCMNlacItemDlg::SetParamData(bool isModify, int nMethod, const T_NLCT_NEWTON& dataNewton, const T_NLCT_DISPCT& dataDispct, const CArray<T_NLCT_NEWTON, T_NLCT_NEWTON&>& arNewton, const CArray<T_NLCT_DISPCT, T_NLCT_DISPCT&>& arDispct)
{
	m_bModify = isModify;
	m_nMethod = nMethod;

	switch(m_nMethod)
	{
	case 0:
		m_nInterMethodRdo = 0;
		break;
	case 2:
		m_nInterMethodRdo = 1;
		break;
	default:
		m_nInterMethodRdo = 0;
		break;
	}

	m_DataNewton = dataNewton;
	m_DataDispct = dataDispct;
	m_arNewton.Copy(arNewton);
	m_arDispct.Copy(arDispct);

	Data2Dlg();

	UpdateData(FALSE);
	CtrlManager();
}

void CCMNlacItemDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	CCMDlgBase::DestroyWindow();
}

void CCMNlacItemDlg::OnCancel()
{
	CCMDlgBase::DestroyWindow();
}

BOOL CCMNlacItemDlg::ApplyOrOK() 
{   
	if(!Dlg2Data()) return FALSE;

	if(m_nMethod == 0) // Newton-Raphson
	{
		if(m_bModify)
		{
			m_pParentWnd->UpdateNewtonList(m_DataNewton);
		}
		m_pParentWnd->SetItemNewton(m_DataNewton);
		m_pParentWnd->SetDataNewton(m_DataNewton);
	}
	else if(m_nMethod == 1) // Arc-Length
	{
		if(m_bModify)
		{
			m_pParentWnd->UpdateArclenList(m_DataArclen);
		}
		m_pParentWnd->SetItemArclen(m_DataArclen);
		m_pParentWnd->SetDataArclen(m_DataArclen);
	}
	else if(m_nMethod == 2) // Displacement-Control
	{
		if(m_bModify)
		{
			m_pParentWnd->UpdateDispctList(m_DataDispct);
		}
		m_pParentWnd->SetItemDispct(m_DataDispct);
		m_pParentWnd->SetDataDispct(m_DataDispct);
	}
	return TRUE;
}

void CCMNlacItemDlg::OnCmdLoadcaseBtn() 
{
	CreateOrActivateDlg((CDBDoc*)CDBDoc::GetDocPoint(),CCMLoadCaseDlg::IDD,NULL);	
}


void CCMNlacItemDlg::OnChangeCmdNlacNewtonNumEd() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int nNum = m_number.GetEditValueInt();
	if (nNum > 0) m_wndGrid->SetRowCountUser(nNum);

}

void CCMNlacItemDlg::OnChangeCmdNlacDispctrlDispstepEd() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int nNum = m_DispCtrlDispStep.GetEditValueInt();
	if (nNum > 0) m_wndGrid->SetRowCountUser(nNum);
	
}

void CCMNlacItemDlg::OnChangeCmdNlacDispctrlMaxdispEd() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	double dValue = m_DispCtrlMaxDisp.GetEditValue();
	m_wndGrid->SetMaxDispValue(dValue);
}

void CCMNlacItemDlg::OnMethodRdo()
{
	UpdateData();
	GridDlg2Data();

	if(m_nInterMethodRdo==0)
	{
		m_wndGrid->SetOption(0);  
		OnChangeCmdNlacNewtonNumEd();
		OnNewtonGenBtn();
		m_nMethod=0;
	}
	else if(m_nInterMethodRdo==1)
	{
		m_wndGrid->SetOption(1);
		OnChangeCmdNlacDispctrlDispstepEd();
		OnDispCtrlGenBtn();
		m_nMethod=2;
	}
	else
	{
		ASSERT(0);
	}

	GridData2Dlg();

	CtrlManager();  
}

void CCMNlacItemDlg::GridData2Dlg()
{
	if(m_nMethod == 0)
	{
		int nCount = m_DataNewton.arLoadFactor.GetSize();
		for (int i = 0; i < nCount; i++)
			m_wndGrid->SetValueRow( i+1, m_DataNewton.arLoadFactor[i].dValue);
	}
	else if(m_nMethod == 1)
	{
		// do nothing
	}
	else if(m_nMethod == 2)
	{
		int nCount = m_DataDispct.arNodeDisp.GetSize();

		for (int i=0; i<nCount; i++)
			m_wndGrid->SetValueRow(i+1, m_DataDispct.arNodeDisp[i].dValue);
	}
}

void CCMNlacItemDlg::GridDlg2Data()
{
	if(m_nMethod == 0)  // Newton-Raphson
	{
		const int nCount = m_wndGrid->GetRowCount();
		m_DataNewton.arLoadFactor.SetSize(nCount);
		for(int i = 0; i < nCount; i++)   
		{
			m_DataNewton.arLoadFactor[i].dValue = m_wndGrid->GetValueRow(i+1);
		}
	}
	else if(m_nMethod == 1)  // Arc-Length
	{
		// do nothing
	}
	else if(m_nMethod == 2)  // Displacement-Control
	{
		const int nCount = m_wndGrid->GetRowCount();
		m_DataDispct.arNodeDisp.SetSize(nCount);
		for(int i = 0; i < nCount; i++)   // Table의 값으로 수정요망
		{
			m_DataDispct.arNodeDisp[i].dValue = m_wndGrid->GetValueRow(i+1);
		}
	}
}