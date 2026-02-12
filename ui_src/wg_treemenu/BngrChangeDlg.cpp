// BngrChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BngrChangeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBngrChangeDlg dialog


CBngrChangeDlg::CBngrChangeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBngrChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBngrChangeDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
}


void CBngrChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBngrChangeDlg)
	DDX_Control(pDX, IDC_TM_TARGET_COMBO, m_wndTarGroupComb);
	DDX_Control(pDX, IDC_TM_SOURCE_COMBO, m_wndSrcGroupComb);
	DDX_Control(pDX, IDC_TM_BNDRTYPE_LIST, m_wndBndrTypeList);
	DDX_Radio(pDX, IDC_TM_COPY, m_nOption);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CBngrChangeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CBngrChangeDlg)
	ON_BN_CLICKED(IDC_TM_TARGET_BUTTON, OnDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_COPY, OnTmOptionChange)
	ON_BN_CLICKED(IDC_TM_DELETE, OnTmOptionChange)
	ON_BN_CLICKED(IDC_TM_MOVE, OnTmOptionChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBngrChangeDlg message handlers

BOOL CBngrChangeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nOption = 0;
	UpdateData(FALSE);
	SetBndrTypeList();
	OptionCtrlMan();
	
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBngrChangeDlg::SetBndrTypeList()
{
	CArray<int, int> aItemChecked;
	DataExchange(aItemChecked, 0);
}

void CBngrChangeDlg::OnDefineGroupButton() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD);		
}

unsigned int CBngrChangeDlg::GetSourceGroupKey()
{
	/*
	CString StrGrupName;
	int nCurSel = this->m_wndSrcGroupComb.GetCurSel();
	m_wndSrcGroupComb.GetLBText(nCurSel,StrGrupName);

	T_BNGR_K BngrK;
	BngrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(StrGrupName);
	*/

	T_BNGR_K BngrK;
	m_wndSrcGroupComb.GetSelectedBngr(BngrK);
	return BngrK;
}

unsigned int CBngrChangeDlg::GetTargetGroupKey()
{
	/*
	CString StrGrupName;
	int nCurSel = this->m_wndTarGroupComb.GetCurSel();
	m_wndTarGroupComb.GetLBText(nCurSel,StrGrupName);

	T_BNGR_K BngrK;
	BngrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(StrGrupName);
	*/

	T_BNGR_K BngrK;
	m_wndTarGroupComb.GetSelectedBngr(BngrK);

	return BngrK;
}
void CBngrChangeDlg::Execute()
{
	UpdateData(TRUE);
	T_BNGR_K SrcKey, DestKey;

	SrcKey = this->GetSourceGroupKey();
	DestKey = this->GetTargetGroupKey();

	CArray<int, int> TypeArray;
	DataExchange(TypeArray, 1);

	CDBDoc::GetDocPoint()->m_pDataCtrl->ChangeBndrGroup(SrcKey, DestKey, TypeArray, m_nOption + 1);
}
void CBngrChangeDlg::OnTmExecute() 
{
	Execute();
}

void CBngrChangeDlg::OnTmClose() 
{
	CloseDlg();
}

void CBngrChangeDlg::OnTmOptionChange() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	OptionCtrlMan();
}

void CBngrChangeDlg::OptionCtrlMan()
{
	CArray<UINT,UINT>Ctrls;
	Ctrls.Add(IDC_TM_STATIC11);
	Ctrls.Add(IDC_TM_TARGET_COMBO);
	Ctrls.Add(IDC_TM_TARGET_BUTTON);
	if(m_nOption == 2) // Delete
	{
		this->CtrlEnableDisable(Ctrls,FALSE);  
	}
	else
	{
		this->CtrlEnableDisable(Ctrls,TRUE);  
	}
}

void CBngrChangeDlg::DataExchange(CArray<int, int>& aItemChecked, int nOperation)
{
	struct T_LIST_BOX_ITEM
	{
		CString str;
		int nCmd;
		int nMask;
	};
	T_LIST_BOX_ITEM aListBoxItem[] = {
		{_LS(IDS_TM_DT_CONS), LT_CONS_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_NSPR), LT_NSPR_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_GSPR), LT_GSPR_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_ELNK), LT_ELNK_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_NLNK), LT_NLNK_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_FRLS), LT_FRLS_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_OFFS), LT_OFFS_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PRLS), LT_PRLS_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_RIGD), LT_RIGD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_MCON), LT_MCON_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_STDG), LT_STDG_CMD, D_PROD_GEN},
		{_LS(IDS_TM_DT_HECB), LT_HECB_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_HSPT), LT_HSPT_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_EWSF), LT_EWSF_CMD,            D_PROD_CVL},
		{_LS(IDS_TM_DT_SECF), LT_SECF_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_ESSF), LT_ESSF_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_WSSF), LT_WSSF_CMD, D_PROD_GEN},
		{_LS(IDS_TM_DT_PSSF), LT_PSSF_CMD, D_PROD_GEN+D_PROD_CVL},
	};

	int nMask = 0;
#if defined(_MGEN)
	nMask = D_PROD_GEN;
#elif defined(_CIVIL)
	nMask = D_PROD_CVL;
#elif defined(_TOWER)
	nMask = D_PROD_TOW;
#else
	#error _T("Unknown Product Definition!")
#endif

	aItemChecked.RemoveAll();

	int i, nCount, ix=0;
	nCount = sizeof(aListBoxItem)/sizeof(T_LIST_BOX_ITEM);
	for(i=0; i < nCount; i++)
	{
		if ((nMask & aListBoxItem[i].nMask) == 0) continue;
		if (nOperation == 0)
		{
			ix = m_wndBndrTypeList.AddString(aListBoxItem[i].str);
			m_wndBndrTypeList.SetCheck(ix, 0);
		}
		else
		{
			if (m_wndBndrTypeList.GetCheck(ix++) != 0)
				aItemChecked.Add(aListBoxItem[i].nCmd);
		}
	}
}
