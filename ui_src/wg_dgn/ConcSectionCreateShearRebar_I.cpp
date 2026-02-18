// ConcSectionCreateRebar.cpp : implementation file
// (060123) sshan Create

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionCreateShearRebar_I.h"
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
// CConcSectionCreateShearRebar_I dialog


CConcSectionCreateShearRebar_I::CConcSectionCreateShearRebar_I(CWnd* pParent /*=NULL*/)
	: CChildDialog(CConcSectionCreateShearRebar_I::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	//{{AFX_DATA_INIT(CConcSectionCreateShearRebar_I)
	m_strShearRebarDia = _T("");	
	//}}AFX_DATA_INIT

	m_pParent = (CConcSectionShearRebar*)pParent;

	m_ShbrData.Initialize();
}


CConcSectionCreateShearRebar_I::~CConcSectionCreateShearRebar_I()
{
	
}


void CConcSectionCreateShearRebar_I::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionCreateShearRebar_I)
	DDX_Control(pDX, IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_I, m_CobxShearRebarDia);
	DDX_CBString(pDX, IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_I, m_strShearRebarDia);
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_I_SPACE_EDT, m_dShearRebarXStep);
	DDX_Control(pDX, IDC_DGN_LENGTH_UNIT_SHEAR_REBAR_I_STATIC6, m_wndLengthUnit);	
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_I_Y_STEP_EDIT, m_edtShearRebarYNum);	
	DDX_Control(pDX, IDC_DGN_SHEAR_REBAR_I_Z_STEP_EDIT, m_edtShearRebarZNum);
	
	//}}AFX_DATA_MAP
}

BOOL CConcSectionCreateShearRebar_I::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	T_UNIT_INDEX CurUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurUnit.nBase_Length,CurUnit.nBase_Force,TRUE);

	SetInitUnit();
	CDBLib::GetRebarNameAtComboBox(&m_CobxShearRebarDia, TRUE);
	//Data2Dlg();
	SetInitData(m_pParent->m_DataI);	

	//(2010.02.05) Add by Unsang :: Set String!
	const CString& strCode = m_pParent->m_strDgnCode;
	if(strCode == CONCODE_EC2_2_05 || strCode == CONCODE_AASHTO_LRFD12 ||
	   strCode == CONCODE_AASHTO_LRFD07 || strCode == CONCODE_AASHTO_LRFD16 ||
	   strCode == CONCODE_AASHTO_LRFD17 || strCode == CONCODE_AASHTO_LRFD20 ||
	   strCode == CONCODE_AASHTO_LRFD24)
	{
		GetDlgItem(IDC_DGN_SHEAR_REBAR_I_STATIC4)->SetWindowText(_LS(IDS_DGN_CON_SECT_REBAR_SHEAR_SPACE_STC));	// 종방향(X)간격
		GetDlgItem(IDC_DGN_SHEAR_REBAR_I_STATIC3)->SetWindowText(_LS(IDS_DGN_CON_SECT_REBAR_SHEAR_NUM_Y_STC));	// Y방향 단수
		GetDlgItem(IDC_DGN_SHEAR_REBAR_I_STATIC2)->SetWindowText(_LS(IDS_DGN_CON_SECT_REBAR_SHEAR_NUM_Z_STC));	// Z방향 단수
	}
	
	m_edtShearRebarYNum.SetRange(2, 100);	
	m_edtShearRebarZNum.SetRange(2, 100);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CConcSectionCreateShearRebar_I, CChildDialog)
	//{{AFX_MSG_MAP(CConcSectionCreateShearRebar_I)	
	ON_CBN_EDITCHANGE(IDC_DGN_REBARDB_COMBO_SHEAR_REBAR_I, OnEditchangeDgnRebardbComboShear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearRebar_I message handlers


BOOL CConcSectionCreateShearRebar_I::DestroyWindow() 
{
	Dlg2Data();
	m_pParent->m_DataI = m_ShbrData;

	return CChildDialog::DestroyWindow();
}

void CConcSectionCreateShearRebar_I::SetInitUnit()
{
	m_dShearRebarXStep.SetUnitType(D_UNITSYS_BASE_LENGTH);
}



void CConcSectionCreateShearRebar_I::OnUpdateUI(CCmdUI*pCmdUI)
{
	
}

void CConcSectionCreateShearRebar_I::OnEditchangeDgnRebardbComboShear() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CConcSectionCreateShearRebar_I::Data2Dlg()
{
	//Share Rebar 정보를 Dlg에 보여줌
	m_dShearRebarXStep.SetEditUnit(m_ShbrData.dSubRebarSpace);
	m_edtShearRebarYNum.SetValue(m_ShbrData.dSubRebarNum[0]);
	m_edtShearRebarZNum.SetValue(m_ShbrData.dSubRebarNum[1]);

	m_CobxShearRebarDia.SetCurSel(GetComboRebarIndex(m_ShbrData.strSubRebarName));

	UpdateData(FALSE);

	return TRUE;
}

BOOL CConcSectionCreateShearRebar_I::Dlg2Data()
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

void CConcSectionCreateShearRebar_I::UpdateBuffer()
{
	
}

void CConcSectionCreateShearRebar_I::SetInitData(T_REBT_SHBR ShearRebarData)
{	
	m_ShbrData.Initialize();
	m_ShbrData.dSubRebarSpace = ShearRebarData.dSubRebarSpace;
	m_ShbrData.dSubRebarNum[0] = ShearRebarData.dSubRebarNum[0];
	m_ShbrData.dSubRebarNum[1] = ShearRebarData.dSubRebarNum[1];
	m_ShbrData.strSubRebarName = ShearRebarData.strSubRebarName;

	Data2Dlg();
}

void CConcSectionCreateShearRebar_I::SaveDlgData()
{
	Dlg2Data();
}

int CConcSectionCreateShearRebar_I::GetComboRebarIndex(CString strRebarname)
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


