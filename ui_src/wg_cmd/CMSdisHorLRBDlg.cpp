// CMSdisDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdisHorLRBDlg.h"

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
// CCMSdisHorLRBDlg dialog

CCMSdisHorLRBDlg::CCMSdisHorLRBDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdisHorLRBDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdisHorLRBDlg)
	m_key = 0;
	m_data.Initialize();
	m_bDBData=FALSE;
	//}}AFX_DATA_INIT
}

CCMSdisHorLRBDlg::~CCMSdisHorLRBDlg()
{
}

void CCMSdisHorLRBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdisHorLRBDlg)
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_LRB_K0_EDT, m_edtK0  );
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_LRB_K0_UNT, m_untK0  );
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_LRB_K2_EDT, m_edtK2  );
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_LRB_K2_UNT, m_untK2  );
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_LRB_QD_EDT, m_edtQd  );
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_LRB_QD_UNT, m_untQd  );
	DDX_Control(pDX, IDC_ETC_SDIS_HOR_LRB_BMP_STC, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdisHorLRBDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdisHorLRBDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdisHorLRBDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	UpdateCtrls();

	return FALSE;
}

void CCMSdisHorLRBDlg::OnOK()
{
	Dlg2Data();
	return CDialogMove::OnOK();
}

void CCMSdisHorLRBDlg::Data2Dlg()
{
	m_edtK0  .SetEditUnit(m_data.dK0    );
	m_edtK2  .SetEditUnit(m_data.dK2    );
	m_edtQd  .SetEditUnit(m_data.dQd    );

	UpdateData(FALSE);
}

BOOL CCMSdisHorLRBDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.dK0     = m_edtK0  .GetEditValue();
	m_data.dK2     = m_edtK2  .GetEditValue();
	m_data.dQd     = m_edtQd  .GetEditValue();

	return TRUE;
}

void CCMSdisHorLRBDlg::SetBmpImage()
{
	UpdateData(TRUE);
	
	//UINT aBitmapID = IDB_ETC_SDIS_HOR;
	CString csImgPath = _T("SVG\\illustration\\Dialog\\Sdis_Hor.svg");
	m_ImgViewer.SetImage(csImgPath);
}

void CCMSdisHorLRBDlg::InitCtrl()
{
	m_edtK0.SetUnitType(CUnitCtrl::m_SDIS_UNIT.dK0);
	m_untK0.SetUnitType(CUnitCtrl::m_SDIS_UNIT.dK0);
	m_edtK2.SetUnitType(CUnitCtrl::m_SDIS_UNIT.dK2);
	m_untK2.SetUnitType(CUnitCtrl::m_SDIS_UNIT.dK2);
	m_edtQd.SetUnitType(CUnitCtrl::m_SDIS_UNIT.dQd);
	m_untQd.SetUnitType(CUnitCtrl::m_SDIS_UNIT.dQd);

	SetBmpImage();
}

void CCMSdisHorLRBDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	if(m_bDBData)
	{
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_HOR_LRB_K0_EDT))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_HOR_LRB_K2_EDT))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_HOR_LRB_QD_EDT))->SetReadOnly(TRUE);
	}
}