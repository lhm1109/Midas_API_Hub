// CMIehcShearRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMIehcShearRDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMIehcShearRDlg
CCMIehcShearRDlg::CCMIehcShearRDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMIehcShearRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMIehcShearRDlg)
	//}}AFX_DATA_INIT
	m_bGlobalMode=FALSE;
	m_nUseOption=D_IEHC_R_GLOBAL;
	m_pBitmap =NULL;

	m_aCtrlIDUseOptRdo.Add(IDD_CMD_IEHC_SHEAR_PROP_R_IEHC_RDO);
	m_aCtrlIDUseOptRdo.Add(IDD_CMD_IEHC_SHEAR_PROP_R_USER_RDO);
	m_aCtrlIDMoveUp.Add(IDD_CMD_IEHC_SHEAR_PROP_R_USER_STC);
	m_aCtrlIDMoveUp.Add(IDD_CMD_IEHC_SHEAR_PROP_R_USER_EDT);
	m_aCtrlIDMoveUp.Add(IDD_CMD_IEHC_SHEAR_PROP_R_USER2_STC);
	m_aCtrlIDMoveUp.Add(IDD_CMD_IEHC_SHEAR_PROP_R_USER3_STC);
	m_aCtrlIDMoveUp.Add(IDD_CMD_IEHC_SHEAR_PROP_IMG_GRP);
	m_aCtrlIDMoveUp.Add(IDD_CMD_IEHC_SHEAR_PROP_IMG_PIC);
	m_aCtrlIDMoveUp.Add(IDOK);
	m_aCtrlIDMoveUp.Add(IDCANCEL);
}

CCMIehcShearRDlg::~CCMIehcShearRDlg()
{
	if (m_pBitmap ) delete m_pBitmap ;
}

void CCMIehcShearRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMIehcShearRDlg)
	DDX_Control(pDX, IDD_CMD_IEHC_SHEAR_PROP_R_USER_EDT, m_edtR);
	DDX_Radio(pDX, IDD_CMD_IEHC_SHEAR_PROP_R_IEHC_RDO, m_nUseOption);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMIehcShearRDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMIehcShearRDlg)
	ON_BN_CLICKED(IDD_CMD_IEHC_SHEAR_PROP_R_IEHC_RDO, CtrlManager)
	ON_BN_CLICKED(IDD_CMD_IEHC_SHEAR_PROP_R_USER_RDO, CtrlManager)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMIehcShearRDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitCtrl();
	Data2Dlg();
	CtrlManager();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMIehcShearRDlg::InitCtrl()
{
	if(m_bGlobalMode)
	{
		// Global일 때 Hide
		CDlgUtil::CtrlShowHide(this, m_aCtrlIDUseOptRdo, FALSE);

		// 끌어올릴 간격
		CRect rectIehcRdo, rectUserStc;
		GetDlgItem(IDD_CMD_IEHC_SHEAR_PROP_R_IEHC_RDO)->GetWindowRect(rectIehcRdo);
		GetDlgItem(IDD_CMD_IEHC_SHEAR_PROP_R_USER_STC)->GetWindowRect(rectUserStc);  
		const int nYDist = rectUserStc.top - rectIehcRdo.top;

		// 컨트롤들 끌어올림
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlIDMoveUp, -1*nYDist);

		// 프레임 크기 조정
		CRect rect;
		GetDlgItem(IDD_CMD_IEHC_SHEAR_PROP_R_GRP)->GetWindowRect(rect);
		rect.bottom -= nYDist;
		ScreenToClient(rect);
		GetDlgItem(IDD_CMD_IEHC_SHEAR_PROP_R_GRP)->MoveWindow(rect);

		// 대화상자 사이즈 조정
		GetWindowRect(rect);
		rect.bottom -= nYDist;
		MoveWindow(rect);
	}

	if (m_pBitmap!= 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDD_CMD_IEHC_SHEAR_PROP_IMG_PIC);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(IDB_IEHC_SHEAR_R);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CCMIehcShearRDlg::Data2Dlg()
{
	UpdateData(TRUE);
	m_edtR.SetEditUnit(m_dR);
}

void CCMIehcShearRDlg::Dlg2Data()
{
	m_dR = m_edtR.GetEditValue();
	UpdateData(FALSE);
}

void CCMIehcShearRDlg::OnOK() 
{	
	Dlg2Data();

	if(m_dR<0 || m_dR>1)
	{
		AfxMessageBox(_LS(IDS_CMD_IEHC_SHEAR_R_INVALID_VALUE));
		return;
	}

	CDialogMove::OnOK();
}

void CCMIehcShearRDlg::CtrlManager()
{
	UpdateData(TRUE);

	const BOOL bAuto = m_nUseOption==0 && !m_bGlobalMode;
	m_edtR.SetReadOnly(bAuto);
	if(bAuto)
	{
		T_IEHC_D IehcD;
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetIehc(IehcD)) IehcD.Initialize();
		m_edtR.SetEditUnit(IehcD.dR);        
	}  
}