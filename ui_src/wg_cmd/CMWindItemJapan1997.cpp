// CMWindItemJapan1997.cpp : implementation file
//
// 2003. 09. 23  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemJapan1997.h"
#include "CMWindItemDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan1997 dialog


CCMWindItemJapan1997::CCMWindItemJapan1997(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMWindItemJapan1997::IDD, pParent)
{
		m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMWindItemDlg*)pParent;

	//{{AFX_DATA_INIT(CCMWindItemJapan1997)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Data.Initialize();
}

void CCMWindItemJapan1997::SetData2Dlg(T_WIND_JP1987& data, BOOL bModify)
{
	m_Data = data;
	m_bModify = bModify;
}

BOOL CCMWindItemJapan1997::SetDlg2Data(T_WIND_JP1987& data)
{
	BOOL bOK = Dlg2Data();
	data = m_Data;
	return bOK;
}

void CCMWindItemJapan1997::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemJapan1997)
	DDX_Control(pDX, IDC_CMD_REDU_FACTOR_EDIT, m_editReduFactor);
	DDX_Control(pDX, IDC_CMD_PRES_COEF_EDIT, m_editPresCoef);
		DDX_Control(pDX, IDC_CMD_FRAME, m_grpWindParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemJapan1997, CChildDialog)
	//{{AFX_MSG_MAP(CCMWindItemJapan1997)
		ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemJapan1997 message handlers

BOOL CCMWindItemJapan1997::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_grpWindParam.InitControl(this, CCMWindItemJapan1997::IDD, IDC_CMD_FRAME, TRUE, TRUE);
		m_grpWindParam.SetFoldState(FALSE); // 무조건 펼친다

	m_editReduFactor.SetUnitType(D_UNITSYS_NONE);
	m_editPresCoef.SetUnitType(D_UNITSYS_NONE);

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMWindItemJapan1997::DestroyWindow() 
{
	Dlg2Data();

	if (m_pParent != NULL)
	{
		if (!m_pParent->IsSaveFoldedInfo())
		{
			m_grpWindParam.SetFoldState(FALSE, FALSE);
		}
	}
	
	return CChildDialog::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMWindItemJapan1997::Data2Dlg()
{
	m_editReduFactor.SetEditUnit(m_Data.dPressureReductionFactor);
	m_editPresCoef.SetEditUnit(m_Data.dWindPressureCoefficient);
}

BOOL CCMWindItemJapan1997::Dlg2Data()
{
	m_Data.Initialize();
	m_Data.dPressureReductionFactor = m_editReduFactor.GetEditValue();
	m_Data.dWindPressureCoefficient = m_editPresCoef.GetEditValue();
	return TRUE;
}

LRESULT CCMWindItemJapan1997::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
		CRect rcChild;
		GetWindowRect(rcChild);
		m_pParent->AlignCtrl2Param(rcChild.bottom);

		return 0l;
}
