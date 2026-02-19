// DgnConPSLCOM.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConPSLCOM.h"

#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConPSLCOM dialog


CDgnConPSLCOM::CDgnConPSLCOM(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConPSLCOM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConPSLCOM)
	//}}AFX_DATA_INIT
}


void CDgnConPSLCOM::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConPSLCOM)
	DDX_Control(pDX, IDC_DGN_PSLCOM_TRANLCB , m_TranLcb);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConPSLCOM, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConPSLCOM)
	ON_BN_CLICKED(IDC_DGN_PSLCOM_LOADCOMBI_BTN, OnDgnLoadCombiBtn)

//  ON_BN_CLICKED(IDC_DGN_PSLCOM_OK, OnOK)
// ON_BN_CLICKED(IDC_DGN_PSLCOM_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConPSLCOM message handlers
BOOL CDgnConPSLCOM::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	InitLComData();

	UpdateData(FALSE);

	return TRUE;
}

void CDgnConPSLCOM::OnCancel() 
{
	// TODO: PSc extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConPSLCOM::OnOK() 
{
	UpdateData(TRUE);

	CString strLComName = _T("");
	CString strLoadName = _T("");

	m_TranLcb.GetLBText(m_TranLcb.GetCurSel(), strLComName);

	if(strLComName == _T(""))
		return;

	T_DCON_D ConcData;
	ConcData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetDcon(ConcData))
	{
		CDgnDataCtrl DataCtrl;
		DataCtrl.Get_DgnConDcon(ConcData);
	}
	ConcData.LcomKeyTran = m_pDoc->m_pAttrCtrl->GetLcomKey(D_LCOMTYPE_CONCRETE,strLComName);

	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDcon(ConcData);

	if(bCheck)
		CDialogMove::OnOK();
}

void CDgnConPSLCOM::InitLComData()
{

	int nLcomType = D_LCOMTYPE_CONCRETE; //Concrete
	CString strLcbName = _T("");

	m_TranLcb.ResetContent();

	T_DCON_D ConcData;
	ConcData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetDcon(ConcData))
	{
		CDgnDataCtrl DataCtrl;
		DataCtrl.Get_DgnConDcon(ConcData);
	}
	int iIdx=0, iSelIdx=-1;
	T_LCOM_K LcbKey;
	T_LCOM_D LcbData;
	POSITION LcbPos = m_pDoc->m_pAttrCtrl->GetStartLcom(nLcomType);
	while(LcbPos)
	{
		m_pDoc->m_pAttrCtrl->GetNextLcom(nLcomType, LcbPos, LcbKey, LcbData);
		if(LcbData.nActive==2) // Serviceability.
		{
			m_TranLcb.AddString(LcbData.LoadCombName);
			if(ConcData.LcomKeyTran == LcbKey)
				iSelIdx = iIdx;
			iIdx++;
		}
	}
	m_TranLcb.SetCurSel(iSelIdx);
	
}

void CDgnConPSLCOM::OnDgnLoadCombiBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_COMBINATION,0));
}
