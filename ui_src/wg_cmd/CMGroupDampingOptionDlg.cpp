// CMGroupDampingOptionDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGroupDampingOptionDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingOptionDlg dialog


CCMGroupDampingOptionDlg::CCMGroupDampingOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGroupDampingOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGroupDampingOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMGroupDampingOptionDlg::CCMGroupDampingOptionDlg(int nType, T_GRDP_D* pGrdpD, CWnd* pParent /*=NULL*/)
: CDialogMove(CCMGroupDampingOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGroupDampingOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nType = nType;
	
	m_pData = pGrdpD;
	
	m_aGroupRdo.Add(IDC_CMD_GRDP_MATL_RDO);
	m_aGroupRdo.Add(IDC_CMD_GRDP_STRUCTURE_RDO);
	m_aValueRdo.Add(IDC_CMD_GRDP_SMALL_RDO);
	m_aValueRdo.Add(IDC_CMD_GRDP_LARGE_RDO);
}

void CCMGroupDampingOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGroupDampingOptionDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGroupDampingOptionDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGroupDampingOptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingOptionDlg message handlers

BOOL CCMGroupDampingOptionDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(m_nType == 1)  SetWindowText(_LS(IDS_CMD_DAMP_SELECT_OPTION));
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGroupDampingOptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
//  if (!m_pDoc->m_pDataCtrl->AddGrdp(m_Data)) return;

	CDialogMove::OnOK();
}

void CCMGroupDampingOptionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMGroupDampingOptionDlg::Data2Dlg()
{
	if(m_nType == 0)      // Element Mass & Stiffness Proportiona
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aGroupRdo, m_pData->nElemGroupPriority);
		CDlgUtil::CtrlRadioSetCheck(this, m_aValueRdo, m_pData->nElemValuePriority);
	}
	else
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aGroupRdo, m_pData->nStrainGroupPriority);
		CDlgUtil::CtrlRadioSetCheck(this, m_aValueRdo, m_pData->nStrainValuePriority);
	}
}

BOOL CCMGroupDampingOptionDlg::Dlg2Data()
{
	if(m_nType == 0)      // Element Mass & Stiffness Proportiona
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aGroupRdo, m_pData->nElemGroupPriority);
		CDlgUtil::CtrlRadioGetCheck(this, m_aValueRdo, m_pData->nElemValuePriority);
	}
	else
	{
		CDlgUtil::CtrlRadioGetCheck(this, m_aGroupRdo, m_pData->nStrainGroupPriority);
		CDlgUtil::CtrlRadioGetCheck(this, m_aValueRdo, m_pData->nStrainValuePriority);
	}
	
	return TRUE;
}


