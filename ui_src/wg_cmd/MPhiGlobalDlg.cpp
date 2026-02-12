// MPhiGlobalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiGlobalDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\DialogActionReactor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if defined (_CIVIL)
#define D_SGLD_SUM_KEY  D_SGLD_SUM_CVL/*9*/
#else
#define D_SGLD_SUM_KEY  D_SGLD_SUM_GEN/*7*/
#endif


/////////////////////////////////////////////////////////////////////////////
// CMPhiGlobalDlg dialog
///


CMPhiGlobalDlg::CMPhiGlobalDlg(MPhiDataAll* pMPhiDataAll, CWnd* pParent /*=NULL*/)
	: CDlgChild(CMPhiGlobalDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pMPhiDataAll = pMPhiDataAll;

	//{{AFX_DATA_INIT(CMPhiGlobalDlg)
	m_nInitForceType = 0;
	m_nYieldPoint = 0;
	//}}AFX_DATA_INIT

	// set flag childHasBtn Flag....
	SetChildHasBtnFlag(TRUE);
}


void CMPhiGlobalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPhiGlobalDlg)
	DDX_Control(pDX, IDC_LOADCASE_COBX, m_cLoadCaseCobx);		
	DDX_Radio(pDX, IDC_EQUILIBRIUM_RDO, m_nInitForceType);
	DDX_Radio(pDX, IDC_INIT_YIELD_RDO, m_nYieldPoint);	
	DDX_Control(pDX, IDC_NUMBER_SECTION_EDT, m_edtNoSectDivision);
	DDX_Control(pDX, IDC_MAX_ITERATION_EDT, m_edtMaxIteration);
	DDX_Control(pDX, IDC_TOLERANCE_EDT, m_edtTolerance);	
	DDX_Control(pDX, IDC_INCLUDE_TENDON_CHK, m_Tendonchk);
	DDX_Control(pDX, IDC_SHIFT_CURVE_CHK, m_Shiftchk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMPhiGlobalDlg, CDlgChild)
	//{{AFX_MSG_MAP(CMPhiGlobalDlg)
	ON_BN_CLICKED(IDC_EQUILIBRIUM_RDO, OnEquilibriumRdo)
	ON_BN_CLICKED(IDC_LOADCACE_RDO, OnLoadcaceRdo)	
	ON_BN_CLICKED(IDC_TYPE_II_RDO, OnTypeIiRdo)
	ON_BN_CLICKED(IDC_SHIFT_CURVE_CHK, OnChkCurve)
	
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPhiGlobalDlg message handlers
BOOL CMPhiGlobalDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// Spin을 Initialize 한다..
	m_edtMaxIteration.SetRange(0, 1000);
	m_edtMaxIteration.SetInteger(TRUE);
	m_edtMaxIteration.SetValue(20);

	// Init combo....
	InitComboBox();

	// 호출
	DialogActionReactor::DoDlgReact(DialogActionReactor::kOnInitAfter, CMPhiGlobalDlg::IDD, this);

	if(!Data2Dlg()) return FALSE;
	
	EnableCtrl();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMPhiGlobalDlg::InitComboBox()
{
	int nIndex = 0;

	// Load Case Cbx
	m_cLoadCaseCobx.ResetContent();

	CString tempS;

	CArray<T_STLD_K, T_STLD_K> aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	int i = 0;
	for(i=0; i<aStldK.GetSize(); i++)
	{
		T_STLD_D rStldD;
		m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], rStldD);						
		tempS.Format(_T("ST: %s"), rStldD.LoadCaseName);
		nIndex = m_cLoadCaseCobx.AddString(tempS);
		m_cLoadCaseCobx.SetItemData(nIndex, i);	
	}

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

	
}

void CMPhiGlobalDlg::AdjustComboListBox(CComboBox& Cbx)
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

void CMPhiGlobalDlg::OnEquilibriumRdo() 
{
	UpdateData();
	EnableCtrl();
}

void CMPhiGlobalDlg::OnLoadcaceRdo() 
{
	UpdateData();
	EnableCtrl();
}

void CMPhiGlobalDlg::OnTypeIiRdo() 
{		
}

void CMPhiGlobalDlg::OnChkCurve()
{
	UpdateData();
	EnableCtrl(); 
	
	UpdateData(FALSE);
}

void CMPhiGlobalDlg::EnableCtrl() 
{		
	UpdateData();
	
	GetDlgItem(IDC_LOADCASE_COBX)->EnableWindow(m_nInitForceType);  

	BOOL bShiftcurve= m_Shiftchk.GetCheck();
	GetDlgItem(IDC_INCLUDE_TENDON_CHK)->EnableWindow(bShiftcurve);  
}

/////////////////////////////////////////////////////////////////////
// Data Control.....
BOOL CMPhiGlobalDlg::Apply()
{
	//if(!Dlg2Data()) return FALSE;
	//
	//if(!m_pDoc->m_pDataCtrl->AddMpgb(m_pMPhiDataAll)) return FALSE;
	
	return TRUE;
}

BOOL CMPhiGlobalDlg::Data2Dlg()
{
	//if(!m_pDoc->m_pAttrCtrl->GetMpgb(m_pMPhiDataAll)) m_pMPhiDataAll.Initialize();

	T_MPGB_D& MpgbD = m_pMPhiDataAll->MpgbD;
		
	m_nInitForceType = MpgbD.nInitialForce;
	m_nYieldPoint = MpgbD.nYieldPoint;

	if(m_nInitForceType==1)
	{
		CArray<T_STLD_K, T_STLD_K> aStldK;
		m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
		int nIndex = -1;
		for(int i=0; i<aStldK.GetSize(); i++)
		{
			if(MpgbD.LoadCaseKey==aStldK[i])	
			{
				nIndex=i;
				break;
			}
		}
		
		if(MpgbD.nLoadCaseType==0)			
		{
			if(nIndex == -1)
			{
				m_cLoadCaseCobx.SetCurSel(0);
			}
			else
			{
				m_cLoadCaseCobx.SetCurSel(nIndex);
			}			
		}
		else
		{
			if(MpgbD.LoadCaseKey == 1) m_cLoadCaseCobx.SetCurSel(aStldK.GetSize());
			else if(MpgbD.LoadCaseKey == D_SGLD_SUM_KEY) m_cLoadCaseCobx.SetCurSel(aStldK.GetSize() + 1);
			else ASSERT(FALSE);
		}
	}
	else	m_cLoadCaseCobx.SetCurSel(0);


	CString tempS;
	tempS.Format(_T("%d"), MpgbD.nSectionDivision);
	m_edtNoSectDivision.SetWindowText(tempS);
	tempS.Format(_T("%d"), MpgbD.nMaxIteration);
	m_edtMaxIteration.SetWindowText(tempS);
	tempS.Format(_T("%.4g"), MpgbD.dTolerance);
	m_edtTolerance.SetWindowText(tempS);
	m_Tendonchk.SetCheck(MpgbD.bIncludeTendon);
	m_Shiftchk.SetCheck(MpgbD.bShiftcurve);

	UpdateData(FALSE);
	
	return TRUE;
}

BOOL CMPhiGlobalDlg::Dlg2Data()
{
	UpdateData();
	T_MPGB_D& MpgbD = m_pMPhiDataAll->MpgbD;
	MpgbD.Initialize();
		
	MpgbD.nInitialForce = m_nInitForceType;
	MpgbD.nYieldPoint = m_nYieldPoint ;
	
	/*
	Prefix를 검사하여 Load Case의 종류를 판별...
	_T("ST")이면 Static Load Case이고,
	그 이외의 것이면 Time History Load Case임....
	*/
	CString strLoadName;
	m_cLoadCaseCobx.GetWindowText(strLoadName);
	if(strLoadName.Left(2) == _T("ST") || strLoadName.IsEmpty())
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
		//m_pData.LoadCaseKey = m_pDoc->m_pAttrCtrl->GetSgldKey(strLoadCaseName);
		if(strLoadCaseName.CompareNoCase(_LS(IDS_CMD_THIS_STAG_TYPE1)) == 0)	MpgbD.LoadCaseKey = 1;
		else if(strLoadCaseName.CompareNoCase(_LS(IDS_CMD_THIS_STAG_TYPE2)) == 0) MpgbD.LoadCaseKey = D_SGLD_SUM_KEY;
		MpgbD.nLoadCaseType = 1;
	}
	
	CFormulaEdit::GetEditValue(&m_edtNoSectDivision, MpgbD.nSectionDivision);
	CFormulaEdit::GetEditValue(&m_edtMaxIteration, MpgbD.nMaxIteration);
	CFormulaEdit::GetEditValue(&m_edtTolerance, MpgbD.dTolerance);
	MpgbD.bIncludeTendon= m_Tendonchk.GetCheck();
	MpgbD.bShiftcurve= m_Shiftchk.GetCheck();
	if(!MpgbD.bShiftcurve) MpgbD.bIncludeTendon = FALSE;
	
	return TRUE;
}

void CMPhiGlobalDlg::OnDestroy() 
{
	Dlg2Data();

	CDlgChild::OnDestroy();
}

void CMPhiGlobalDlg::OnOK()
{
	return;
}

void CMPhiGlobalDlg::OnCancel() 
{
	return;
}
