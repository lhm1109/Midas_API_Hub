#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdReinforceDesignDlg.h"
#include "../wg_db/wg_db_DataCtrl.h"
#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_base/wg_base_DlgUtil.h"
#include "../wg_db/PostCtrl.h"
#include "../wg_db/DesignResult.h"
#include "../wg_db/FileDesignMgrBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCmdReinforceDesignDlg::CCmdReinforceDesignDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdReinforceDesignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdReinforceDesignDlg)
	m_nReinforceMethod = 0;
	//m_nVerificationProcess = 0;
	//}}AFX_DATA_INIT
}

void CCmdReinforceDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_CMD_STATIC2, m_nReinforceMethod);
	DDX_Control(pDX, IDC_CMD_COMB_STATIC1, m_StructureType);
}

BEGIN_MESSAGE_MAP(CCmdReinforceDesignDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_OK_BTN, OnOK)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_CANCEL_BTN, OnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_BROWSE, OnDetail)
	
	ON_BN_CLICKED(IDC_CMD_STATIC2, OnUpdateMethod)
	ON_BN_CLICKED(IDC_CMD_STATIC3, OnUpdateMethod)
	
END_MESSAGE_MAP()

BOOL CCmdReinforceDesignDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	m_StructureType.AddString(_LS(IDS_CMD_REINFORCE_STRUCTURE_1));
	m_StructureType.AddString(_LS(IDS_CMD_REINFORCE_STRUCTURE_2));
	m_StructureType.AddString(_LS(IDS_CMD_REINFORCE_STRUCTURE_3));
	m_StructureType.AddString(_LS(IDS_CMD_REINFORCE_STRUCTURE_4));
	m_StructureType.SetCurSel(2);
	//m_ImportanceFactor.AddString(_T("1.0"));
	//m_ImportanceFactor.SetCurSel(0);

	Data2Dlg();
	OnUpdateMethod();
	return TRUE;
}

void CCmdReinforceDesignDlg::Data2Dlg()
{
	m_Data.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetRdpa(m_Data))
	{
		m_nReinforceMethod = m_Data.nReinForceType;
		m_StructureType.SetCurSel(m_Data.nStructureCategory);
	}
	
	UpdateData(FALSE);
}

BOOL CCmdReinforceDesignDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.nReinForceType = m_nReinforceMethod;
	m_Data.nStructureCategory = m_StructureType.GetCurSel();
	return TRUE;
}

void CCmdReinforceDesignDlg::OnOK()
{
	if (!Dlg2Data())	
		return;
	BOOL bReinforceDgnEnable = FALSE;
	CFileDesignMgrBase* pFileDgnMgr = NULL;
	if (CDBDoc::GetDocPoint()->m_pPostCtrl->GetDesignResult())
	{
		pFileDgnMgr = CDBDoc::GetDocPoint()->m_pPostCtrl->GetDesignResult()->GetStlDgnMgr()->GetReinforceDgnMgr();
		bReinforceDgnEnable = pFileDgnMgr->IsDesignResultEnable() && pFileDgnMgr->ReadDesignType() == 1;
	}
	else
	{
		ASSERT(FALSE);
	}
	
	if (bReinforceDgnEnable)
	{
		if (AfxMessageBox(_LS(IDS_DB_REMOVE_REINFORCE_DESIGN), MB_YESNO) != IDYES)
			return;
	}
	if (!CDBDoc::GetDocPoint()->m_pDataCtrl->AddRdpa(m_Data)) 
		return;
	if (bReinforceDgnEnable)
	{
		pFileDgnMgr->ClearDesignDataByType(0);
	}
	CDialogMove::OnOK();
}

void CCmdReinforceDesignDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCmdReinforceDesignDlg::OnDetail()
{
	#define WM_RPC_STLCODEDLG 1001
	unsigned int WM_RPC_MESSAGE = RegisterWindowMessage(_T("WM_RPC_MESSAGE"));
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_RPC_MESSAGE, WM_RPC_STLCODEDLG, (LPARAM)&m_Data.stlData);
}

void CCmdReinforceDesignDlg::OnUpdateMethod()
{
	UpdateData(TRUE);
	if (m_nReinforceMethod == 0)
	{
		GetDlgItem(IDC_CMD_STATIC4)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_COMB_STATIC1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_STATIC4)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_COMB_STATIC1)->EnableWindow(TRUE);
	}
}

