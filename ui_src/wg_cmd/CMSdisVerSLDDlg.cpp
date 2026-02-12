// CMSdisDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdisVerSLDDlg.h"

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
// CCMSdisVerSLDDlg dialog

CCMSdisVerSLDDlg::CCMSdisVerSLDDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdisVerSLDDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdisVerSLDDlg)
	m_key = 0;
	m_data.Initialize();
	//}}AFX_DATA_INIT
}

CCMSdisVerSLDDlg::~CCMSdisVerSLDDlg()
{
}

void CCMSdisVerSLDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdisVerSLDDlg)
	DDX_Control(pDX, IDC_ETC_SDIS_VER_KV_EDT, m_edtKv);
	DDX_Control(pDX, IDC_ETC_SDIS_VER_KV_UNT, m_untKv);
	DDX_Control(pDX, IDC_ETC_SDIS_VER_BMP, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdisVerSLDDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdisVerSLDDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdisVerSLDDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	UpdateCtrls();

	return FALSE;
}

void CCMSdisVerSLDDlg::OnOK()
{
	Dlg2Data();
	return CDialogMove::OnOK();
}

void CCMSdisVerSLDDlg::Data2Dlg()
{
	m_edtKv   .SetEditUnit(m_data.dKv    );

	UpdateData(FALSE);
}

BOOL CCMSdisVerSLDDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.dKv       = m_edtKv.GetEditValue();

	return TRUE;
}

void CCMSdisVerSLDDlg::SetBmpImage()
{
	UpdateData(TRUE);

	//UINT aBitmapID[] = { IDB_ETC_SDIS_SLD_VER } ;
	CString csImgPath = _T("SVG\\illustration\\Dialog\\SDIS_SLD_Ver.svg");
	m_ImgViewer.SetImage(csImgPath);
}

void CCMSdisVerSLDDlg::InitCtrl()
{
	m_edtKv   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKv    );
	m_untKv   .SetUnitType(CUnitCtrl::m_SDIS_UNIT.dKv    );

	SetBmpImage();
}

void CCMSdisVerSLDDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	if(m_bDBData)
	{
		((CEdit*)GetDlgItem(IDC_ETC_SDIS_VER_KV_EDT))->SetReadOnly(TRUE);
	}
}