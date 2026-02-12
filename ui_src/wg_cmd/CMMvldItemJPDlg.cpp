// CMMvldItemJPDlg.cpp : implementation file
// 2002. 7. 19  by TAE
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemJPDlg.h"

#include "CMMvldItemLLoadJPDlg.h"
#include "CMMvldItemYLoadJPDlg.h"
#include "CMMvldItemTLoadJPDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemJPDlg dialog
void CCMMvldItemJPDlg::CreateChildDlg()
{
	m_pChildDlgs.Add(new CCMMvldItemLLoadJPDlg(this));
	m_pChildDlgs.Add(new CCMMvldItemYLoadJPDlg(this));
	m_pChildDlgs.Add(new CCMMvldItemTLoadJPDlg(this));  
}

CCMMvldItemJPDlg::CCMMvldItemJPDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemJPDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvldItemJPDlg)
	m_bImpact = TRUE;
	m_nImpact = 0;
	m_nLoadType = 0;
	m_strLoadCase = _T("");
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

	// Get Document Pointer
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();

	// Init Data
	m_bModify = FALSE;
	m_nCurLoadType = 0;

	// Create Objects of Child Dialog
	CreateChildDlg();

	// Make Group
	arImpactGroup.Add(IDC_IMPACT_RADIO1);
	arImpactGroup.Add(IDC_IMPACT_RADIO2);
	arImpactGroup.Add(IDC_IMPACT_RADIO3);
	arImpactGroup.Add(IDC_IMPACT_RADIO4);
	arImpactGroup.Add(IDC_IMPACT_USER_EDIT1);
	arImpactGroup.Add(IDC_IMPACT_USER_EDIT2);
	arUserRdGroup.Add(IDC_IMPACT_USER_EDIT1);
	arUserRdGroup.Add(IDC_IMPACT_USER_EDIT2);
}

void CCMMvldItemJPDlg::DestroyChildDlg()
{
	int nSize = m_pChildDlgs.GetSize();
	for(int i=0; i<nSize; i++)
	{
		delete m_pChildDlgs[i];
	}
}

CCMMvldItemJPDlg::~CCMMvldItemJPDlg()
{
	// Destroy Objects of Child Dialog
	DestroyChildDlg();
}

void CCMMvldItemJPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemJPDlg)
	DDX_Control(pDX, IDC_IMPACT_USER_EDIT2, m_editUser2);
	DDX_Control(pDX, IDC_IMPACT_USER_EDIT1, m_editUser1);
	DDX_Control(pDX, IDC_CHILD_FRM, m_wndHolder);
	DDX_Check(pDX, IDC_IMPACT_CHECK, m_bImpact);
	DDX_Radio(pDX, IDC_IMPACT_RADIO1, m_nImpact);
	DDX_Text(pDX, IDC_LOAD_CASE_EDIT, m_strLoadCase);
	DDX_Radio(pDX, IDC_LOAD_TYPE_RADIO1, m_nLoadType);
	DDX_Text(pDX, IDC_DESCRIPTION_EDIT, m_strDescription);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldItemJPDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemJPDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_IMPACT_RADIO4, OnImpactRadio4)
	ON_BN_CLICKED(IDC_IMPACT_CHECK, OnImpactCheck)
	ON_BN_CLICKED(IDC_LOAD_TYPE_RADIO1, OnLoadTypeRadio)
	ON_BN_CLICKED(IDC_LOAD_TYPE_RADIO2, OnLoadTypeRadio)
	ON_BN_CLICKED(IDC_LOAD_TYPE_RADIO3, OnLoadTypeRadio)
	ON_BN_CLICKED(IDC_IMPACT_RADIO1, OnImpactRadio4)
	ON_BN_CLICKED(IDC_IMPACT_RADIO2, OnImpactRadio4)
	ON_BN_CLICKED(IDC_IMPACT_RADIO3, OnImpactRadio4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemJPDlg message handlers

/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMMvldItemJPDlg::SetData2ChildDlg()
{
	int nSize = m_pChildDlgs.GetSize();
	for(int i=0; i<nSize; i++)
	{
		if(i == 0)
			((CCMMvldItemLLoadJPDlg *)m_pChildDlgs[i])->SetCurDlgData(&m_Data);
		else if(i == 1)
			((CCMMvldItemYLoadJPDlg *)m_pChildDlgs[i])->SetCurDlgData(&m_Data);
		else if(i == 2)
			((CCMMvldItemTLoadJPDlg *)m_pChildDlgs[i])->SetCurDlgData(&m_Data);
		else ASSERT(0);
	}
}

void CCMMvldItemJPDlg::SetData2Dlg()
{
	m_strLoadCase = m_Data.LoadCaseName;
	m_strDescription = m_Data.Description; 
	m_nLoadType = m_Data.nLiveLoadType;
	m_nCurLoadType = m_nLoadType;
	m_bImpact = m_Data.bImpactCoeff;
	m_nImpact = m_Data.nBridgeType;
	m_editUser1.SetEditUnit(m_Data.nCoeffNumerator);
	m_editUser2.SetEditUnit(m_Data.nCoeffDenominator);

	UpdateData(FALSE);

	// Child들 왈.. - 엄마 나두 초기화해도.. (단, 윈도우는 아직 안만들어진 상태)
	SetData2ChildDlg();

}

BOOL CCMMvldItemJPDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// T하중은 막는다. - 아직 미완성
	GetDlgItem(IDC_LOAD_TYPE_RADIO3)->EnableWindow(FALSE);

	// Init Data - Add 일때만
	T_MVLDjp_D TempData;
	m_pDoc->m_pAttrCtrl->InitializeMvldjp(TempData);
	if (!m_bModify) 
		m_Data = TempData;
	else
	{
		if (m_Data.nLiveLoadType == 0) 
		{
			m_Data.MLoad = TempData.MLoad;
			m_Data.TLoad = TempData.TLoad;
		}
		else if (m_Data.nLiveLoadType == 1)
		{
			m_Data.LLoad = TempData.LLoad;
			m_Data.TLoad = TempData.TLoad;
		}
		else if (m_Data.nLiveLoadType == 2)
		{
			m_Data.MLoad = TempData.MLoad;
			m_Data.TLoad = TempData.TLoad;
		}
		else ASSERT(0);
	}

	// Set Unit
	m_editUser1.SetUnitType(0); 
	m_editUser2.SetUnitType(0);

	// Data Setting
	SetData2Dlg();

	// Create Child Dialog Window
	if(!ShowChildDialog())
	{
		GSaveHistoryFormatNF(_T("Child Dialog가 만들어지지 않았습니다."));
		return FALSE;
	}
	
	// Update Control
	UpdateCtrls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
//
// OnOK
//
BOOL CCMMvldItemJPDlg::SetChildDlg2Data()
{
	if(m_nCurLoadType<0 || m_nCurLoadType>=m_pChildDlgs.GetSize()) return FALSE;

	BOOL bResult;
	
	if(m_nCurLoadType == 0)
		bResult = ((CCMMvldItemLLoadJPDlg *)m_pChildDlgs[m_nCurLoadType])->GetCurDlgData(&m_Data);
	else if(m_nCurLoadType == 1)
		bResult = ((CCMMvldItemYLoadJPDlg *)m_pChildDlgs[m_nCurLoadType])->GetCurDlgData(&m_Data);
	else if(m_nCurLoadType == 2)
		bResult = ((CCMMvldItemTLoadJPDlg *)m_pChildDlgs[m_nCurLoadType])->GetCurDlgData(&m_Data);
	else ASSERT(0);

	return bResult;
}

BOOL CCMMvldItemJPDlg::SetDlg2Data()
{ 
	UpdateData(TRUE);

	m_Data.LoadCaseName = m_strLoadCase;
	m_Data.Description = m_strDescription;
	m_Data.nLiveLoadType = m_nLoadType;
	m_Data.bImpactCoeff = m_bImpact;
	if(m_bImpact)
	{
		m_Data.nBridgeType = m_nImpact;
		if(m_nImpact == 3)  // User
		{
			m_Data.nCoeffNumerator = m_editUser1.GetEditValue();
			m_Data.nCoeffDenominator = m_editUser2.GetEditValue();
		}
	}
	// Child Dialog
	return SetChildDlg2Data();
}

BOOL CCMMvldItemJPDlg::ApplyOrOK()
{
	if (!SetDlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMvldjp(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMvldjp(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

void CCMMvldItemJPDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!ApplyOrOK()) return;

	CDialogMove::OnOK();
}

void CCMMvldItemJPDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	if(!ApplyOrOK()) return;
}

void CCMMvldItemJPDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
//
// Update Child Dialog
//

BOOL CCMMvldItemJPDlg::ShowChildDialog()
{
	// Destroy Current Child Dialog
	if(m_nCurLoadType<0 || m_nCurLoadType>=m_pChildDlgs.GetSize()) return FALSE;

	if(m_pChildDlgs[m_nCurLoadType]->GetSafeHwnd()) 
		m_pChildDlgs[m_nCurLoadType]->End();

	// Create New Child Dialog
	UpdateData(TRUE);
	if(m_pChildDlgs[m_nLoadType] == NULL) return FALSE;
	
	CRect WRect;
	m_wndHolder.GetWindowRect(&WRect);
	ScreenToClient(WRect); 
	m_pChildDlgs[m_nLoadType]->CreateInit(this,WRect.left,WRect.top);
	m_nCurLoadType = m_nLoadType;

	return TRUE;
}

void CCMMvldItemJPDlg::OnLoadTypeRadio() 
{
	// TODO: Add your control notification handler code here
	ShowChildDialog();
}

/////////////////////////////////////////////////////////////////////////////
//
// Update Controls
//
void CCMMvldItemJPDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, arImpactGroup, m_bImpact);

	if(m_bImpact)
	{
		if(m_nImpact == 3) 
			CDlgUtil::CtrlEnableDisable(this, arUserRdGroup, TRUE);
		else 
			CDlgUtil::CtrlEnableDisable(this, arUserRdGroup, FALSE);
	}
}

void CCMMvldItemJPDlg::OnImpactRadio4() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrls();
	
}

void CCMMvldItemJPDlg::OnImpactCheck() 
{
	// TODO: Add your control notification handler code here
	UpdateCtrls();
}

