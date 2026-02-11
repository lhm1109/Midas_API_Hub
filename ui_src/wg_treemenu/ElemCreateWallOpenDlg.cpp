// ElemCreateWallOpenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemCreateWallOpenDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"       // for ID_DB_MERGETOL

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemCreateWallOpenDlg dialog


CElemCreateWallOpenDlg::CElemCreateWallOpenDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemCreateWallOpenDlg::IDD, pParent)
{
	m_aDirCtrlsRdo.RemoveAll();
	m_aDirCtrlsRdo.Add(IDC_TM_DIR_I2J_RDO);
	m_aDirCtrlsRdo.Add(IDC_TM_DIR_J2I_RDO);
	m_aDirCtrlsRdo.Add(IDC_TM_DIR_MID_RDO);
	m_aDirCtrlsRdo.FreeExtra();

	m_nDirType = 0;

	m_bMergeDupNode   = TRUE;
	m_bSubDivideFrame = TRUE;
}

CElemCreateWallOpenDlg::~CElemCreateWallOpenDlg()
{
}


void CElemCreateWallOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemCreateWallOpenDlg)
	//DDX_Check(pDX, IDC_TM_FREE_CHECK, m_bFreeNode);
	DDX_Radio(pDX, IDC_TM_DIR_I2J_RDO, m_nDirType);
	DDX_Control(pDX, IDC_TM_DIST_DX_EDT,   m_edtDx);
	DDX_Control(pDX, IDC_TM_DIST_DY_EDT,   m_edtDy);
	DDX_Control(pDX, IDC_TM_SIZE_W_EDT ,   m_edtDw);
	DDX_Control(pDX, IDC_TM_SIZE_H_EDT ,   m_edtDh);
	DDX_Control(pDX, IDC_TM_DIST_DX_UNT,   m_untDx);
	DDX_Control(pDX, IDC_TM_DIST_DY_UNT,   m_untDy);
	DDX_Control(pDX, IDC_TM_SIZE_W_UNT ,   m_untDw);
	DDX_Control(pDX, IDC_TM_SIZE_H_UNT ,   m_untDh);
	DDX_Check(pDX, IDC_TM_SUB_DIVIDE_FRAME  , m_bSubDivideFrame);
	DDX_Check(pDX, IDC_TM_MERGE_DUP_NODE    , m_bMergeDupNode  );
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);

	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemCreateWallOpenDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemCreateWallOpenDlg)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	ON_BN_CLICKED(IDC_TM_DIR_I2J_RDO, OnTmWallOpenDir)
	ON_BN_CLICKED(IDC_TM_DIR_J2I_RDO, OnTmWallOpenDir)
	ON_BN_CLICKED(IDC_TM_DIR_MID_RDO, OnTmWallOpenDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemCreateWallOpenDlg message handlers

BOOL CElemCreateWallOpenDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

#if defined(_DEBUG)
	m_edtDx.SetWindowText(_T("1"));
	m_edtDy.SetWindowText(_T("0.5"));
	m_edtDw.SetWindowText(_T("2"));
	m_edtDh.SetWindowText(_T("1"));
#endif

	m_untDx.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDy.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDh.SetUnitType(D_UNITSYS_BASE_LENGTH);

	OnTmWallOpenDir();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemCreateWallOpenDlg::Execute()
{
	_ElemWallOpen ED;

	if(GetParameter(ED))
	{
		CreateWallOpenElem(ED);
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
	}
}

BOOL CElemCreateWallOpenDlg::GetParameter(_ElemWallOpen& ED)
{
	UpdateData(TRUE);
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirCtrlsRdo, m_nDirType);

	ED.m_nDir = m_nDirType;

	double dVal = 0.0;
	CString strFloat = _T("");

	m_edtDx.GetWindowText(strFloat);
	if (GetFloatNumber(strFloat, dVal)) ED.m_dx = dVal;
	else { ASSERT(0);  return FALSE; }

	m_edtDy.GetWindowText(strFloat);
	if (GetFloatNumber(strFloat, dVal)) ED.m_dy = dVal;
	else { ASSERT(0);  return FALSE; }

	m_edtDw.GetWindowText(strFloat);
	if (GetFloatNumber(strFloat, dVal)) ED.m_dw = dVal;
	else { ASSERT(0);  return FALSE; }

	m_edtDh.GetWindowText(strFloat);
	if (GetFloatNumber(strFloat, dVal)) ED.m_dh = dVal;
	else { ASSERT(0);  return FALSE; }

	ED.m_bSubDivideFrame = m_bSubDivideFrame;
	ED.m_bMergeDupNode = m_bMergeDupNode;

	return TRUE;
}

void CElemCreateWallOpenDlg::CreateWallOpenElem(_ElemWallOpen& ED)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	BOOL bSuccess = FALSE;

	// Get Selected Wall Elem
	CArray<unsigned int, unsigned int> aElemK_wall;
	pIGM->GetSelectedElemKeyList(aElemK_wall);

	T_ELEM_D ElemD;
	for (int i = aElemK_wall.GetSize()-1; i >= 0; --i)
	{
		if (!pDoc->m_pAttrCtrl->GetElem(aElemK_wall[i], ElemD))  { ASSERT(0);  return; }
		if (!pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) aElemK_wall.RemoveAt(i);
	}
	if (aElemK_wall.GetSize() == 0) 
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		ASSERT(0);    return;
	}

	bSuccess = pDoc->m_pDataCtrl->DivideWallByOpening(aElemK_wall, 
		ED.m_dw, ED.m_dh, ED.m_dy, ED.m_nDir, ED.m_dx, 
		ED.m_bMergeDupNode, ED.m_bSubDivideFrame);

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CElemCreateWallOpenDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

void CElemCreateWallOpenDlg::OnTmWallOpenDir()
{
	UpdateData(TRUE); // get m_nElemType, m_nOption
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirCtrlsRdo, m_nDirType);
	BOOL bEnableDx_new = TRUE;    if (m_nDirType == 2) bEnableDx_new = FALSE;

	CDlgUtil::CtrlEnableDisable(this, IDC_TM_DIST_DX_EDT, bEnableDx_new);

	// change bitmap
	ChangeBitmap(m_nDirType);

}

void CElemCreateWallOpenDlg::ChangeBitmap(int nDirType)
{
	CString strSVG = _T("");
	if (nDirType == 0)
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_opening_window_s.svg");
	else if (nDirType == 1)
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_opening_window_s1.svg");
	else if (nDirType == 2)
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_opening_window_s2.svg");
	else
	{
		ASSERT(false);
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_opening_window_s.svg");
	}

	m_wndPicture.SetImage(strSVG);
}