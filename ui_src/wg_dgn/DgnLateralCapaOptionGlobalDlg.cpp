// DgnLateralCapaOptionGlobalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnLateralCapaOptionGlobalDlg.h"

#include "DgnLateralCapaOptionDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if defined (_CIVIL)
#define D_SGLD_SUM_KEY  9
#else
#define D_SGLD_SUM_KEY  7
#endif


/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionGlobalDlg dialog
///


CDgnLateralCapaOptionGlobalDlg::CDgnLateralCapaOptionGlobalDlg(LateralCapaDataAll* pDataAll, CWnd* pParent /*=NULL*/)
	: CDlgChild(CDgnLateralCapaOptionGlobalDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CDgnLateralCapaOptionDlg *) pParent;
	m_pDataAll = pDataAll;

	//{{AFX_DATA_INIT(CDgnLateralCapaOptionGlobalDlg)
	m_nInitForceType = 0;
	m_nYieldPoint = 0;
	//}}AFX_DATA_INIT

	// set flag childHasBtn Flag....
	SetChildHasBtnFlag(TRUE);
}


void CDgnLateralCapaOptionGlobalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnLateralCapaOptionGlobalDlg)
	//DDX_Control(pDX, IDC_ITERATION_SPIN,     m_IterationSpin);
	DDX_Control(pDX, IDC_LOADCASE_COBX,      m_cLoadCaseCobx);
	DDX_Control(pDX, IDC_TYPE_I_CHK,         m_EarthquakeTypeIChk);
	DDX_Control(pDX, IDC_TYPE_II_CHK,        m_EarthquakeTypeIIChk);
	DDX_Radio  (pDX, IDC_RESIONAL_A_RDO,     m_nRegionalKind);
	DDX_Radio  (pDX, IDC_RESIONAL2_A1_RDO,   m_nRegionalKind2);
	DDX_Radio  (pDX, IDC_GROUND_I_RDO,       m_nGroundKind);	
	DDX_Radio  (pDX, IDC_EQUILIBRIUM_RDO,    m_nInitForceType);
	DDX_Radio  (pDX, IDC_INIT_YIELD_RDO,     m_nYieldPoint);	
	DDX_Control(pDX, IDC_NUMBER_SECTION_EDT, m_edtNoSectDivision);
	DDX_Control(pDX, IDC_MAX_ITERATION_EDT,  m_IterationSpin);
	DDX_Control(pDX, IDC_TOLERANCE_EDT,      m_edtTolerance);	
	DDX_Control(pDX, IDC_CODE_COBX, m_cCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnLateralCapaOptionGlobalDlg, CDlgChild)
	//{{AFX_MSG_MAP(CDgnLateralCapaOptionGlobalDlg)	
	ON_BN_CLICKED(IDC_EQUILIBRIUM_RDO, OnEquilibriumRdo)
	ON_BN_CLICKED(IDC_LOADCACE_RDO,    OnLoadcaceRdo)
	ON_BN_CLICKED(IDC_USERDEFINED_RDO, OnUserDefinedRdo)
	ON_BN_CLICKED(IDC_TYPE_I_CHK,      OnTypeIChk)
	ON_BN_CLICKED(IDC_TYPE_II_CHK,     OnTypeIIChk)
	ON_CBN_SELCHANGE(IDC_CODE_COBX, OnSelchangeModelCobx)
	ON_WM_DESTROY()
	//ON_NOTIFY(UDN_DELTAPOS, IDC_ITERATION_SPIN, OnDeltaposIterationSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionGlobalDlg message handlers
BOOL CDgnLateralCapaOptionGlobalDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// Spin을 Initialize 한다..
	m_IterationSpin.SetRange(0, 1000);
	m_IterationSpin.SetInteger(TRUE);
	m_IterationSpin.SetValue(20);

	m_aCtrlRsnCategory2.RemoveAll();
	m_aCtrlRsnCategory2.Add(IDC_RESIONAL2_A1_RDO);
	m_aCtrlRsnCategory2.Add(IDC_RESIONAL2_A2_RDO);
	m_aCtrlRsnCategory2.Add(IDC_RESIONAL2_B1_RDO);
	m_aCtrlRsnCategory2.Add(IDC_RESIONAL2_B2_RDO);
	m_aCtrlRsnCategory2.Add(IDC_RESIONAL2_C_RDO);
	SetCtrlPos();
	// Init combo....
	InitComboBox();

	if(!Data2Dlg()) return FALSE;
	
	EnableCtrl();	
	ShowHideCtrl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnLateralCapaOptionGlobalDlg::SetCtrlPos()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// 1st point 
	GetDlgItem(IDC_RESIONAL_A_RDO)->GetWindowRect(rRef);
	GetDlgItem(IDC_RESIONAL2_A1_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;

	CDlgUtil::CtrlMoveDistY(this, m_aCtrlRsnCategory2, nDistY);
}

void CDgnLateralCapaOptionGlobalDlg::InitComboBox()
{
	int nIndex = 0;

	m_cLoadCaseCobx.ResetContent();

	CString tempS;

	// Load Case Cbx
	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	for(int i=0; i<aStldK.GetSize(); i++)
	{
		T_STLD_D rStldD;
		m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], rStldD);						
		tempS.Format(_T("ST: %s"), rStldD.LoadCaseName);
		nIndex = m_cLoadCaseCobx.AddString(tempS);
		m_cLoadCaseCobx.SetItemData(nIndex, i);	
	}

    int i = aStldK.GetSize();
	if(m_pDoc->m_pAttrCtrl->GetCountStag() > 0)
	{
		CString strConstructionType[] = {_LS(IDS_CMD_THIS_STAG_TYPE1), _LS(IDS_CMD_THIS_STAG_TYPE2)};
		for(int j = 0; j < sizeof(strConstructionType) / sizeof(CString); j++)
		{    
			tempS.Format(_T("CS: %s"), strConstructionType[j]);
			nIndex = m_cLoadCaseCobx.AddString(tempS);
		  m_cLoadCaseCobx.SetItemData(nIndex, i++);      
		}
	}  
	AdjustComboListBox(m_cLoadCaseCobx);


	// Code Cbx
	m_cCode.ResetContent();
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_V24_DEFAULT)); // 순서랑 변수에 저장되는 값이 다름! 주의 하기 바람..!!
	m_cCode.SetItemData(nIndex, 4);
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_CONC_CODE_BOOK));
	m_cCode.SetItemData(nIndex, 0);	
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_V));
	m_cCode.SetItemData(nIndex, 1);	
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_III_SIG_CK));
	m_cCode.SetItemData(nIndex, 2);	
	nIndex = m_cCode.AddString(_LS(IDS_CMD_MPHI_III_85_SIG_CK));
	m_cCode.SetItemData(nIndex, 3);  
	m_cCode.SetCurSel(4);
	AdjustComboListBox(m_cCode);
}

void CDgnLateralCapaOptionGlobalDlg::AdjustComboListBox(CComboBox& Cbx)
	/*
	콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에 
	맞추어 변경한다.
	*/
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = Cbx.GetDC();
	for (int i=0;i < Cbx.GetCount();i++)
	{
		Cbx.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	Cbx.ReleaseDC(pDC);

	if (Cbx.GetDroppedWidth() < dx)
	{
		Cbx.SetDroppedWidth(dx);
		ASSERT(Cbx.GetDroppedWidth() == dx);
	}
}

void CDgnLateralCapaOptionGlobalDlg::OnEquilibriumRdo() 
{
	UpdateData();
	EnableCtrl();
}

void CDgnLateralCapaOptionGlobalDlg::OnLoadcaceRdo() 
{
	UpdateData();
	EnableCtrl();
}

void CDgnLateralCapaOptionGlobalDlg::OnUserDefinedRdo() 
{
	UpdateData();
	EnableCtrl();
}

void CDgnLateralCapaOptionGlobalDlg::EnableCtrl() 
{		
	UpdateData();

	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_cCode, nCode); // 각 숫자가 의미하는 것은 MpgbD.nCode 의 주석 참고

	GetDlgItem(IDC_LOADCASE_COBX)->EnableWindow(m_nInitForceType == 1);  
	GetDlgItem(IDC_INIT_YIELD_RDO)->EnableWindow(nCode != 4);   // 도시V (평성24년)
	GetDlgItem(IDC_NORMAL_YIELD_RDO)->EnableWindow(nCode != 4); // 도시V (평성24년)
}

void CDgnLateralCapaOptionGlobalDlg::ShowHideCtrl() 
{		
	UpdateData();

	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_cCode, nCode); // 각 숫자가 의미하는 것은 MpgbD.nCode 의 주석 참고

	GetDlgItem(IDC_RESIONAL_A_RDO)->ShowWindow(nCode != 4);   // 도시V (평성24년) 이 아닌경우
	GetDlgItem(IDC_RESIONAL_B_RDO)->ShowWindow(nCode != 4);   // 도시V (평성24년) 이 아닌경우
	GetDlgItem(IDC_RESIONAL_C_RDO)->ShowWindow(nCode != 4);   // 도시V (평성24년) 이 아닌경우
	GetDlgItem(IDC_RESIONAL2_A1_RDO)->ShowWindow(nCode == 4);   // 도시V (평성24년)
	GetDlgItem(IDC_RESIONAL2_A2_RDO)->ShowWindow(nCode == 4);   // 도시V (평성24년)
	GetDlgItem(IDC_RESIONAL2_B1_RDO)->ShowWindow(nCode == 4);   // 도시V (평성24년)
	GetDlgItem(IDC_RESIONAL2_B2_RDO)->ShowWindow(nCode == 4);   // 도시V (평성24년)
	GetDlgItem(IDC_RESIONAL2_C_RDO)->ShowWindow(nCode == 4);   // 도시V (평성24년)
}

void CDgnLateralCapaOptionGlobalDlg::OnTypeIChk() 
{
	//if(!m_EarthquakeTypeIChk.GetCheck() && !m_EarthquakeTypeIIChk.GetCheck())
	//{ m_EarthquakeTypeIChk.SetCheck(TRUE); } // Check!!! 두개중 1개는 무조건 선택
}
void CDgnLateralCapaOptionGlobalDlg::OnTypeIIChk() 
{
	//if(!m_EarthquakeTypeIChk.GetCheck() && !m_EarthquakeTypeIIChk.GetCheck())
	//{ m_EarthquakeTypeIIChk.SetCheck(TRUE); } // Check!!! 두개중 1개는 무조건 선택
}

void CDgnLateralCapaOptionGlobalDlg::OnSelchangeModelCobx()
{
	EnableCtrl();
	ShowHideCtrl();
}

/////////////////////////////////////////////////////////////////////
// Data Control.....
BOOL CDgnLateralCapaOptionGlobalDlg::Apply()
{
	//if(!Dlg2Data()) return FALSE;
	//
	//if(!m_pDoc->m_pDataCtrl->AddMpgb(m_pMpgbData)) return FALSE;
	//if(!m_pDoc->m_pDataCtrl->AddRlco(m_pRlcoData)) return FALSE;
	
	return TRUE;
}

BOOL CDgnLateralCapaOptionGlobalDlg::Data2Dlg()
{
	T_MPGB_D& MpgbD = m_pDataAll->MpgbD;
	T_RLCO_D& RlcoD = m_pDataAll->RlcoD;	

	m_EarthquakeTypeIChk.SetCheck(RlcoD.bEarthquakeTypeI);
	m_EarthquakeTypeIIChk.SetCheck(RlcoD.bEarthquakeTypeII);
	if(MpgbD.nCode == 4)
	{
		m_nRegionalKind = 0;
		m_nRegionalKind2 = RlcoD.nRegionalKind;
	}
	else
	{    
		m_nRegionalKind   = RlcoD.nRegionalKind;
		m_nRegionalKind2 = 0;
	}

	m_nGroundKind     = RlcoD.nGroundKind;
	m_nInitForceType  = RlcoD.nInitialForce;
	m_nYieldPoint     = MpgbD.nYieldPoint;

	if(m_nInitForceType==1)
	{
		CArray<T_STLD_K, T_STLD_K> aStldK;
		m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
		int nIndex;
		for(int i=0; i<aStldK.GetSize(); i++)
		{
			if(MpgbD.LoadCaseKey==aStldK[i])	
			{
				nIndex=i;
				break;
			}
		}
		if(MpgbD.nLoadCaseType==0)			
			m_cLoadCaseCobx.SetCurSel(nIndex);
		else
		{
			if(MpgbD.LoadCaseKey == 1) m_cLoadCaseCobx.SetCurSel(aStldK.GetSize());
			else if(MpgbD.LoadCaseKey == D_SGLD_SUM_KEY) m_cLoadCaseCobx.SetCurSel(aStldK.GetSize() + 1);
			else ASSERT(FALSE);
		}
	}
	else	m_cLoadCaseCobx.SetCurSel(0);

	CDlgUtil::CobxSetCurSelItemData(m_cCode, MpgbD.nCode);

	CString tempS;
	tempS.Format(_T("%d"), MpgbD.nSectionDivision);
	m_edtNoSectDivision.SetWindowText(tempS);
	tempS.Format(_T("%d"), MpgbD.nMaxIteration);
	m_IterationSpin.SetWindowText(tempS);
	tempS.Format(_T("%.4g"), MpgbD.dTolerance);
	m_edtTolerance.SetWindowText(tempS);

	UpdateData(FALSE);
	
	return TRUE;
}

BOOL CDgnLateralCapaOptionGlobalDlg::Dlg2Data()
{
	UpdateData();	

	T_MPGB_D& MpgbD = m_pDataAll->MpgbD;
	T_RLCO_D& RlcoD = m_pDataAll->RlcoD;	

	RlcoD.bEarthquakeTypeI  = m_EarthquakeTypeIChk.GetCheck();
	RlcoD.bEarthquakeTypeII = m_EarthquakeTypeIIChk.GetCheck();
	
	DWORD nItemData;
	CDlgUtil::CobxGetItemDataByCurSel(m_cCode, nItemData);
	MpgbD.nCode = nItemData;
	if(MpgbD.nCode == 4)
	{
		RlcoD.nRegionalKind   = m_nRegionalKind2  ;
	}
	else
	{
		RlcoD.nRegionalKind   = m_nRegionalKind  ;
	}

	RlcoD.nGroundKind     = m_nGroundKind    ;	

	RlcoD.nInitialForce = m_nInitForceType;
	if(m_nInitForceType < 2) MpgbD.nInitialForce = m_nInitForceType;
	MpgbD.nYieldPoint = m_nYieldPoint ;
	
	/*
	Prefix를 검사하여 Load Case의 종류를 판별...
	_T("ST")이면 Static Load Case이고,
	그 이외의 것이면 Time History Load Case임....
	*/
	CString strLoadName;
	m_cLoadCaseCobx.GetWindowText(strLoadName);
	if(strLoadName.Left(2) == _T("ST"))
	{
		CString strLoadCaseName;
		int nStrLength = strLoadName.GetLength();
		strLoadCaseName = strLoadName.Right(nStrLength - 4);

		MpgbD.LoadCaseKey = m_pDoc->m_pAttrCtrl->GetStldKey(strLoadCaseName);
		MpgbD.nLoadCaseType = 0;
	}
	else
	{
		CString strLoadCaseName;
		int nStrLength = strLoadName.GetLength();
		strLoadCaseName = strLoadName.Right(nStrLength - 4);
		//m_pMpgbData.LoadCaseKey = m_pDoc->m_pAttrCtrl->GetSgldKey(strLoadCaseName);
		if(strLoadCaseName.CompareNoCase(_LS(IDS_CMD_THIS_STAG_TYPE1)) == 0)	MpgbD.LoadCaseKey = 1;
		else if(strLoadCaseName.CompareNoCase(_LS(IDS_CMD_THIS_STAG_TYPE2)) == 0) MpgbD.LoadCaseKey = D_SGLD_SUM_KEY;
		MpgbD.nLoadCaseType = 1;
	}
	
	CFormulaEdit::GetEditValue(&m_edtNoSectDivision, MpgbD.nSectionDivision);
	CFormulaEdit::GetEditValue(&m_IterationSpin, MpgbD.nMaxIteration);
	CFormulaEdit::GetEditValue(&m_edtTolerance, MpgbD.dTolerance);
	
	return TRUE;
}

void CDgnLateralCapaOptionGlobalDlg::OnOK()
{
	return;
}

void CDgnLateralCapaOptionGlobalDlg::OnCancel() 
{
	return;
}

void CDgnLateralCapaOptionGlobalDlg::OnDestroy() 
{
	Dlg2Data();

	CDlgChild::OnDestroy();
}
