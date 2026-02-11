// CMBridgeTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMBridgeTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMBridgeTypeDlg dialog


CCMBridgeTypeDlg::CCMBridgeTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMBridgeTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMBridgeTypeDlg)
	m_nKindBridge = 0;
	m_nTypeBridge = 0;
	m_nDirectLoad = 0;
	//}}AFX_DATA_INIT
}


void CCMBridgeTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMBridgeTypeDlg)
	DDX_Radio(pDX, IDC_RADIO_STEEL, m_nKindBridge);
	DDX_Radio(pDX, IDC_SCG, m_nTypeBridge);
	DDX_Radio(pDX, IDC_RADIO_DIRECT_LOAD, m_nDirectLoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMBridgeTypeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMBridgeTypeDlg)
	ON_BN_CLICKED(IDC_BBRIDGE_TYPE_DATA, OnRemoveBridgeTypeData)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMBridgeTypeDlg message handlers
/*
struct T_BRGT_D
{
	int nKindOfBridge;  // 0:Steel, 1:PC, 2:RC
	int nTypeOfBridge;  // 0:Simple, Continuous Girder, 1:Gerber, 2:Rahmen
	int nDirectLoad;    // 0:Direct Load, 1:Indirect Load
	void Initialize()
	{
		nKindOfBridge = 0;
		nTypeOfBridge = 0;
		nDirectLoad = 0;
	}
};
*/
void CCMBridgeTypeDlg::OnRemoveBridgeTypeData() // remove bridge data type 
{
	if (!m_pDoc->m_pDataCtrl->DelBrgt())
	{ ASSERT(0); return; }
	CDialogMove::OnOK();
}

void CCMBridgeTypeDlg::OnOk() 
{
	if(!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}

void CCMBridgeTypeDlg::OnCancel() 
{	
	CDialogMove::OnCancel();
}

BOOL CCMBridgeTypeDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();

	CDialogMove::OnInitDialog();
	Data2Dlg();

	GetDlgItem(IDC_BBRIDGE_TYPE_DATA)->EnableWindow(m_pDoc->m_pAttrCtrl->ExistBrgt());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMBridgeTypeDlg::Dlg2Data()
{
	UpdateData(TRUE); // Dlg ->Value 형 변수로 
	
	m_Data.nKindOfBridge = m_nKindBridge;
	m_Data.nTypeOfBridge = m_nTypeBridge;
	m_Data.nDirectLoad = m_nDirectLoad;

	if(!m_pDoc->m_pDataCtrl->AddBrgt(m_Data)) { ASSERT(0); return FALSE; }

	return TRUE;
}
void CCMBridgeTypeDlg::Data2Dlg()
{
	T_BRGT_D rData; rData.Initialize();

	if(m_pDoc->m_pAttrCtrl->ExistBrgt()) m_pDoc->m_pAttrCtrl->GetBrgt(rData);

	this->m_nKindBridge = rData.nKindOfBridge;
	this->m_nTypeBridge = rData.nTypeOfBridge;
	this->m_nDirectLoad = rData.nDirectLoad;
		
	UpdateData(FALSE);
}

