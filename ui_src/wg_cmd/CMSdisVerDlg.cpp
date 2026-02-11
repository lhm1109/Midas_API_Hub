// CMSdisDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdisVerDlg.h"

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
// CCMSdisVerDlg dialog

CCMSdisVerDlg::CCMSdisVerDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdisVerDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdisVerDlg)
	m_key = 0;
	m_data.Initialize();
	m_bConsVDN = FALSE;
	//}}AFX_DATA_INIT
}

CCMSdisVerDlg::~CCMSdisVerDlg()
{
}

void CCMSdisVerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdisVerDlg)
	DDX_Control(pDX, IDC_ETC_SDIS_VER_KV_EDT    , m_edtKv   );
	DDX_Control(pDX, IDC_ETC_SDIS_VER_KV_UNT    , m_untKv   );
	DDX_Check  (pDX, IDC_ETC_SDIS_VER_CONS_CHK  , m_bConsVDN);
	DDX_Control(pDX, IDC_ETC_SDIS_VER_BETA_EDT  , m_edtBeta );
	DDX_Control(pDX, IDC_ETC_SDIS_VER_ALPHA_EDT , m_edtAlpha);
	DDX_Control(pDX, IDC_ETC_SDIS_VER_SIGV_EDT  , m_edtSigv );
	DDX_Control(pDX, IDC_ETC_SDIS_VER_SIGV_UNT  , m_untSigv );
	DDX_Control(pDX, IDC_ETC_SDIS_VER_BMP, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdisVerDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdisVerDlg)
	ON_BN_CLICKED(IDC_ETC_SDIS_VER_CONS_CHK , OnConsVDNChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdisVerDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	UpdateCtrls();

	return FALSE;
}

void CCMSdisVerDlg::OnOK()
{
	Dlg2Data();
	return CDialogMove::OnOK();
}

void CCMSdisVerDlg::Data2Dlg()
{
	m_bConsVDN = m_data.bConsNonl;
	m_edtKv   .SetEditUnit(m_data.dKv    );
	m_edtBeta .SetEditUnit(m_data.dBeta  );
	m_edtAlpha.SetEditUnit(m_data.dAlpha );
	m_edtSigv .SetEditUnit(m_data.dSigmav);

	UpdateData(FALSE);
}

BOOL CCMSdisVerDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.bConsNonl = m_bConsVDN;
	m_data.dKv       = m_edtKv.GetEditValue();
	m_data.dBeta     = m_edtBeta .GetEditValue();
	m_data.dAlpha    = m_edtAlpha.GetEditValue();
	m_data.dSigmav   = m_edtSigv .GetEditValue();

	return TRUE;
}

void CCMSdisVerDlg::OnConsVDNChk()
{
	UpdateCtrls();
}

void CCMSdisVerDlg::SetBmpImage()
{
	UpdateData(TRUE);

	//UINT aBitmapID[] = { IDB_ETC_SDIS_VER1, IDB_ETC_SDIS_VER2 } ;
	CString aImgPath[] = {
	_T("SVG\\illustration\\Dialog\\Sdis_Ver1.svg") ,
	_T("SVG\\illustration\\Dialog\\Sdis_Ver2.svg") 
	};

	int nIdx = !m_bConsVDN? 0:1;
	m_ImgViewer.SetImage(aImgPath[nIdx]);
}

void CCMSdisVerDlg::InitCtrl()
{
	CString strTitle;
	switch(m_nDevType)
	{
	case 0: strTitle = _LS(IDS_ETC_SDIS_VER_LRB_TITLE); break;
	case 1: strTitle = _LS(IDS_ETC_SDIS_VER_NRB_TITLE); break;
	default: ASSERT(0); strTitle = _LS(IDS_ETC_SDIS_VER_LRB_TITLE); break;
	}
	this->SetWindowText(strTitle);

	m_edtKv   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKv    );
	m_untKv   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKv    );
	m_edtBeta .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dBeta  );
	m_edtAlpha.SetUnitType(CUnitCtrl::m_SDIS_UNIT.dAlpha );
	m_edtSigv .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dSigmav);
	m_untSigv .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dSigmav);

	SetBmpImage();
}

void CCMSdisVerDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	CArray<UINT,UINT> aCtrls_VDN;
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_BETA_STC);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_BETA_EDT);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_BETA_STC2);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_ALPHA_STC);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_ALPHA_EDT);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_ALPHA_STC2);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_SIGV_STC);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_SIGV_EDT);
	aCtrls_VDN.Add(IDC_ETC_SDIS_VER_SIGV_UNT);
	CDlgUtil::CtrlEnableDisable(this, aCtrls_VDN, m_bConsVDN);

	SetBmpImage();

	if(m_bDBData)
	{
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_VER_KV_EDT))->SetReadOnly(TRUE);
		if(m_nDevType==1)
		{
			((CEdit*)GetDlgItem(IDC_ETC_SDIS_VER_BETA_EDT))->SetReadOnly(TRUE);
			((CEdit*)GetDlgItem(IDC_ETC_SDIS_VER_ALPHA_EDT))->SetReadOnly(TRUE);
			((CEdit*)GetDlgItem(IDC_ETC_SDIS_VER_SIGV_EDT))->SetReadOnly(TRUE);
		}
	}
}