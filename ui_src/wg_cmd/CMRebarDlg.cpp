// CMRebarDlg.cpp : implementation file
//
// 2006. 11. 23
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"


#include "CMRebarDlg.h"
#include "CMRebarPscDlg.h"
#include "CMRebarCompositeDlg.h"

//#include "..\wg_db\wg_db_AttrCtrl.h"
//#include "..\wg_db\wg_db_DataCtrl.h"
//#include "..\wg_db\wg_db_SectUtil.h"
//#include "..\wg_base\wg_base_MsgDll.h"

//#include "CMRebarPscCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRebarDlg dialog


CCMRebarDlg::CCMRebarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMRebarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRebarPscDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_KeyPsc = 0;
	m_KeyComposite = 0;
	m_pPscDlg = NULL;
	m_pCompositeDlg = NULL;
}

void CCMRebarDlg::SetParamDataPsc(T_RPSC_K RpscK)
{
	m_KeyPsc = RpscK;
}
void CCMRebarDlg::SetParamDataComposite(T_RPSC_K RpscK)
{
	m_KeyComposite = RpscK;
}

void CCMRebarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarDlg)
	DDX_Control(pDX, IDC_CMD_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMRebarDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMRebarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMRebarDlg message handlers

BOOL CCMRebarDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->LayoutChildDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMRebarDlg::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_CMD_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	int nInitShow = 0;

	m_pPscDlg = new CCMRebarPscDlg(this);
	m_pCompositeDlg = new CCMRebarCompositeDlg(this);

	if (m_KeyPsc != 0) 
	{
		m_pPscDlg->SetParamData(m_KeyPsc);
	}
	if (m_KeyComposite != 0)
	{
		m_pCompositeDlg->SetParamData(m_KeyComposite);
		nInitShow = 1;
	}

// MQC:2272-JHYUN-20081107
#if defined (_CIVIL)
	m_ctrlTab.AddTab(m_pPscDlg, _LS(IDS_WG_CMD__ADDD__PSC), CCMRebarPscDlg::IDD, TRUE);
	m_ctrlTab.AddTab(m_pCompositeDlg, _LS(IDS_WG_CMD__ADDD__Composite), CCMRebarCompositeDlg::IDD, TRUE);
#else
	m_ctrlTab.AddTab(m_pCompositeDlg, _LS(IDS_WG_CMD__ADDD__Composite), CCMRebarCompositeDlg::IDD, TRUE);
#endif

	m_ctrlTab.ShowTab(nInitShow);
}
