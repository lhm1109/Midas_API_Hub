// CMSdisDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdisHorNRBDlg.h"

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
// CCMSdisHorNRBDlg dialog

CCMSdisHorNRBDlg::CCMSdisHorNRBDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdisHorNRBDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdisHorNRBDlg)
	m_key = 0;
	m_data.Initialize();
	//}}AFX_DATA_INIT
}

CCMSdisHorNRBDlg::~CCMSdisHorNRBDlg()
{
}

void CCMSdisHorNRBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdisHorNRBDlg)
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_NRB_KH_EDT, m_edtKh);
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_NRB_KH_UNT, m_untKh);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdisHorNRBDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdisHorNRBDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdisHorNRBDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	UpdateCtrls();

	return FALSE;
}

void CCMSdisHorNRBDlg::OnOK()
{
	Dlg2Data();
	return CDialogMove::OnOK();
}

void CCMSdisHorNRBDlg::Data2Dlg()
{
	m_edtKh.SetEditUnit(m_data.dKh);

	UpdateData(FALSE);
}

BOOL CCMSdisHorNRBDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.dKh = m_edtKh.GetEditValue();

	return TRUE;
}

void CCMSdisHorNRBDlg::InitCtrl()
{
	m_edtKh  .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKh  );
	m_untKh  .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKh  );
}

void CCMSdisHorNRBDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	if(m_bDBData)
	{
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_HOR_NRB_KH_EDT))->SetReadOnly(TRUE);
	}
}