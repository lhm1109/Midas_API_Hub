// CMSdisDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdisHorSLDDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSdisHorSLDDlg dialog

CCMSdisHorSLDDlg::CCMSdisHorSLDDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdisHorSLDDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdisHorSLDDlg)
	m_key = 0;
	m_data.Initialize();
	//}}AFX_DATA_INIT
}

CCMSdisHorSLDDlg::~CCMSdisHorSLDDlg()
{
}

void CCMSdisHorSLDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdisHorSLDDlg)
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_SLD_K0_EDT, m_edtK0);
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_SLD_K0_UNT, m_untK0);
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_SLD_MU0_EDT, m_edtMu0);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdisHorSLDDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdisHorSLDDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdisHorSLDDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	UpdateCtrls();

	return FALSE;
}

void CCMSdisHorSLDDlg::OnOK()
{
	Dlg2Data();
	return CDialogMove::OnOK();
}

void CCMSdisHorSLDDlg::Data2Dlg()
{
	m_edtK0 .SetEditUnit(m_data.dK0 );
	m_edtMu0.SetEditUnit(m_data.dMu0);

	UpdateData(FALSE);
}

BOOL CCMSdisHorSLDDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.dK0  = m_edtK0 .GetEditValue();
	m_data.dMu0 = m_edtMu0.GetEditValue();

	return TRUE;
}

void CCMSdisHorSLDDlg::InitCtrl()
{
	m_edtK0 .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dK0 );
	m_untK0 .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dK0 );
}

void CCMSdisHorSLDDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	if(m_bDBData)
	{
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_HOR_SLD_K0_EDT))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_HOR_SLD_MU0_EDT))->SetReadOnly(TRUE);
	}
}