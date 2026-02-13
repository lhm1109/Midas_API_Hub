// ConcSectionCreateRebar.cpp : implementation file
// (060123) sshan Create

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionCreateShearRebar_J.h"
#include "ConcSectionShearRebar.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearRebar_J dialog


CConcSectionCreateShearRebar_J::CConcSectionCreateShearRebar_J(CWnd* pParent /*=NULL*/)
	: CChildDialog(CConcSectionCreateShearRebar_J::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	//{{AFX_DATA_INIT(CConcSectionCreateShearRebar_J)
	m_strShearRebarDia = _T("");	
	//}}AFX_DATA_INIT

	m_pParent = (CConcSectionShearRebar*)pParent;

	m_ShbrData.Initialize();
}


CConcSectionCreateShearRebar_J::~CConcSectionCreateShearRebar_J()
{
	
}


void CConcSectionCreateShearRebar_J::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionCreateShearRebar_J)
	DDX_Control(pDX, IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_J, m_CobxShearRebarDia);
	DDX_CBString(pDX, IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_J, m_strShearRebarDia);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_J_SPACE_EDT, m_dShearRebarXStep);	
	//DDX_Control(pDX, IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_J, m_CobxShearRebarDia);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_J_Y_STEP_EDIT, m_edtShearRebarYNum);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_J_Z_STEP_EDIT, m_edtShearRebarZNum);
	DDX_Control(pDX, IDC_DGN_LENGTH_UNIT_SHEAR_REBAR_J_STATIC6, m_wndLengthUnit);
	//}}AFX_DATA_MAP
}

BOOL CConcSectionCreateShearRebar_J::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	T_UNIT_INDEX CurUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurUnit.nBase_Length,CurUnit.nBase_Force,TRUE);

	SetInitUnit();
	CDBLib::GetRebarNameAtComboBox(&m_CobxShearRebarDia, TRUE);
	//Data2Dlg();
	SetInitData(m_pParent->m_DataJ);

	AllControlEnableWindow(!m_pParent->bSameIJ);

	m_edtShearRebarYNum.SetRange(2, 100);
	m_edtShearRebarZNum.SetRange(2, 100);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CConcSectionCreateShearRebar_J, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionCreateShearRebar_J)
	ON_CBN_EDITCHANGE(IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_J, OnEditchangeDgnRebardbComboShear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearRebar_J message handlers


BOOL CConcSectionCreateShearRebar_J::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_DataJ = m_ShbrData;

	return CChildDialog::DestroyWindow();
}

void CConcSectionCreateShearRebar_J::SetInitUnit()
{
	m_dShearRebarXStep.SetUnitType(D_UNITSYS_BASE_LENGTH);
}



void CConcSectionCreateShearRebar_J::OnUpdateUI(CCmdUI*pCmdUI)
{
	
}

void CConcSectionCreateShearRebar_J::OnEditchangeDgnRebardbComboShear() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CConcSectionCreateShearRebar_J::Data2Dlg()
{
	//Share Rebar 정보를 Dlg에 보여줌
	m_dShearRebarXStep.SetEditUnit(m_ShbrData.dSubRebarSpace);
	m_edtShearRebarYNum.SetValue(m_ShbrData.dSubRebarNum[0]);
	m_edtShearRebarZNum.SetValue(m_ShbrData.dSubRebarNum[1]);

	m_CobxShearRebarDia.SetCurSel(GetComboRebarIndex(m_ShbrData.strSubRebarName));

	UpdateData(FALSE);

	return TRUE;
}

BOOL CConcSectionCreateShearRebar_J::Dlg2Data()
{
	
	UpdateData(TRUE);

	//전단철근 데이타 Dlg로부터 저장
	m_ShbrData.Initialize();
	m_ShbrData.dSubRebarSpace = m_dShearRebarXStep.GetEditValue();
	m_ShbrData.dSubRebarNum[0] = m_edtShearRebarYNum.GetEditValue();
	m_ShbrData.dSubRebarNum[1] = m_edtShearRebarZNum.GetEditValue();
	int Index = m_CobxShearRebarDia.GetCurSel();
	if(Index!=-1) m_CobxShearRebarDia.GetLBText(Index,m_ShbrData.strSubRebarName);

	return TRUE;
}

void CConcSectionCreateShearRebar_J::UpdateBuffer()
{
	
}

void CConcSectionCreateShearRebar_J::SetInitData(T_REBT_SHBR ShearRebarData)
{
	m_ShbrData.Initialize();
	m_ShbrData.dSubRebarSpace = ShearRebarData.dSubRebarSpace;
	m_ShbrData.dSubRebarNum[0] = ShearRebarData.dSubRebarNum[0];
	m_ShbrData.dSubRebarNum[1] = ShearRebarData.dSubRebarNum[1];
	m_ShbrData.strSubRebarName = ShearRebarData.strSubRebarName;

	Data2Dlg();
}

void CConcSectionCreateShearRebar_J::SaveDlgData()
{
	Dlg2Data();
}

int CConcSectionCreateShearRebar_J::GetComboRebarIndex(CString strRebarname)
{
	CStringArray arRebarNa;
	CDBLib::GetRebarNameListByCode(arRebarNa, TRUE);

	int i = 0;
	for(i=0 ; i<arRebarNa.GetSize() ; i++)
	{
		if(strRebarname == arRebarNa[i])
			break;
	}

	return i;
}

void CConcSectionCreateShearRebar_J::AllControlEnableWindow(BOOL bEnable)
{
	std::vector<int> vEnableID;
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_STATIC);
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_STATIC4);
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_STATIC3);
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_STATIC2);
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_STATIC5);
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_SPACE_EDT);
	vEnableID.emplace_back(IDC_DGN_LENGTH_UNIT_SHEAR_REBAR_J_STATIC6);
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_Y_STEP_EDIT);
	vEnableID.emplace_back(IDC_DGN_SHEAR_REBAR_J_Z_STEP_EDIT);
	vEnableID.emplace_back(IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_J);
	
	for ( auto nID : vEnableID )
	{
		GetDlgItem(nID)->EnableWindow(bEnable);
	}	
}



