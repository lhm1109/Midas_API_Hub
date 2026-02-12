// BndrCldrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrCldrDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
//#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
// #include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_cmd\wg_cmdAll.h"
//#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrCldrDlg dialog


CBndrCldrDlg::CBndrCldrDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrCldrDlg::IDD, pParent)
{
	//m_pBitmap = 0;

	//{{AFX_DATA_INIT(CBndrCldrDlg)
	m_nOption   = 0;
	m_nLocalDir = 0;
	//}}AFX_DATA_INIT

	m_aCtrlOption.Add(IDC_TM_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE);

	m_aCtrlLocalDir.Add(IDC_TM_DIR_LOCAL_XP);
	m_aCtrlLocalDir.Add(IDC_TM_DIR_LOCAL_XN);
	m_aCtrlLocalDir.Add(IDC_TM_DIR_LOCAL_YP);
	m_aCtrlLocalDir.Add(IDC_TM_DIR_LOCAL_YN);
	m_aCtrlLocalDir.Add(IDC_TM_DIR_LOCAL_ZP);
	m_aCtrlLocalDir.Add(IDC_TM_DIR_LOCAL_ZN);
}

CBndrCldrDlg::~CBndrCldrDlg()
{
	//if (m_pBitmap) delete m_pBitmap;
}

void CBndrCldrDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrCldrDlg)
	DDX_Radio(pDX, IDC_TM_OPT_ADD     , m_nOption  );
	DDX_Radio(pDX, IDC_TM_DIR_LOCAL_XP, m_nLocalDir);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

void CBndrCldrDlg::ChangeBitmap()
{
	CString aBitmapPath[] = {
		_T("SVG\\illustration\\Dialog\\tm_cldr1.svg"),
		_T("SVG\\illustration\\Dialog\\tm_cldr2.svg"),
		_T("SVG\\illustration\\Dialog\\tm_cldr3.svg"),
		_T("SVG\\illustration\\Dialog\\tm_cldr4.svg"),
		_T("SVG\\illustration\\Dialog\\tm_cldr5.svg"),
		_T("SVG\\illustration\\Dialog\\tm_cldr6.svg")
	}; // need change - mylee

	int nLocalDir;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLocalDir, nLocalDir);


	m_wndPicture.SetImage(aBitmapPath[nLocalDir]);
}

BEGIN_MESSAGE_MAP(CBndrCldrDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrCldrDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE     , OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE       , OnTmClose)
	ON_BN_CLICKED(IDC_TM_OPT_ADD     , OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE  , OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DIR_LOCAL_XP, OnChangeLocalDir)
	ON_BN_CLICKED(IDC_TM_DIR_LOCAL_XN, OnChangeLocalDir)
	ON_BN_CLICKED(IDC_TM_DIR_LOCAL_YP, OnChangeLocalDir)
	ON_BN_CLICKED(IDC_TM_DIR_LOCAL_YN, OnChangeLocalDir)
	ON_BN_CLICKED(IDC_TM_DIR_LOCAL_ZP, OnChangeLocalDir)
	ON_BN_CLICKED(IDC_TM_DIR_LOCAL_ZN, OnChangeLocalDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrCldrDlg message handlers

BOOL CBndrCldrDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here

	int nOpt = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, nOpt);

	int nLocalDir = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLocalDir, nLocalDir);

	ChangeBitmap();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrCldrDlg::Execute()
{
	// TODO: Add your control notification handler code here
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}

	T_CLDR_D data;
	data.Initialize();

	int nOption = -1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);
	if (nOption == 0)
	{
		int nLocalDir = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLocalDir, nLocalDir);
		data.nDir = nLocalDir;
	}

	BOOL bSuccess = FALSE;
	if (nOption == 0) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddCldr(aSelKey, data);
	else if (nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelCldr(aSelKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
void CBndrCldrDlg::OnTmExecute() 
{
	Execute();
}

void CBndrCldrDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrCldrDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	int nOpt;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOpt);
}

void CBndrCldrDlg::OnChangeLocalDir() 
{
	// TODO: Add your control notification handler code here
	int nLocalDir=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLocalDir, nLocalDir);
	ChangeLocalDir();
}

void CBndrCldrDlg::ChangeLocalDir()
{
	ChangeBitmap();

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->SetModelingModeST(IG_STATE_INITALL);
}

BOOL CBndrCldrDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_CLDR_D data;
	m_pDoc->m_pAttrCtrl->GetCldr(key, data);
	int nLocalDir = data.nDir;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlLocalDir, nLocalDir);
	ChangeLocalDir();

	int nOpt = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, nOpt);
	//UpdateData(FALSE);
	return TRUE;
}
