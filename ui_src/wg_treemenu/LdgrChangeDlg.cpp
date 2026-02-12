// LdgrChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "LdgrChangeDlg.h"

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
// CLdgrChangeDlg dialog


CLdgrChangeDlg::CLdgrChangeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CLdgrChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLdgrChangeDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	
}


void CLdgrChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLdgrChangeDlg)
	DDX_Control(pDX, IDC_TM_LOADTYPE_LIST, m_wndLoadTypeList);
	DDX_Control(pDX, IDC_TM_TARGET_COMBO, m_wndTarGroupComb);
	DDX_Control(pDX, IDC_TM_SOURCE_COMBO, m_wndSrcGroupComb);
	DDX_Radio(pDX, IDC_TM_COPY, m_nOption);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CLdgrChangeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CLdgrChangeDlg)
	ON_BN_CLICKED(IDC_TM_TARGET_BUTTON, OnDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_COPY, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_MOVE, OnChangeOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLdgrChangeDlg message handlers

BOOL CLdgrChangeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	m_nOption = 0;
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	SetLoadTypeList();
	OnChangeOption(); 

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLdgrChangeDlg::SetLoadTypeList()
{
	CArray<int, int> aItemChecked;
	DataExchange(aItemChecked, 0);
}

void CLdgrChangeDlg::OnDefineGroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);
}

unsigned int CLdgrChangeDlg::GetSourceGroupKey()
{
	/*
	CString StrGrupName;
	int nCurSel = this->m_wndSrcGroupComb.GetCurSel();
	m_wndSrcGroupComb.GetLBText(nCurSel,StrGrupName);

	T_BNGR_K BngrK;
	BngrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(StrGrupName);

	return BngrK;
	*/
	T_LDGR_K LdgrK;
	m_wndSrcGroupComb.GetSelectedLdgr(LdgrK);
	return LdgrK;
}

unsigned int CLdgrChangeDlg::GetTargetGroupKey()
{
	/*
	CString StrGrupName;
	int nCurSel = this->m_wndTarGroupComb.GetCurSel();
	m_wndTarGroupComb.GetLBText(nCurSel,StrGrupName);

	T_BNGR_K BngrK;
	BngrK = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngrKey(StrGrupName);

	return BngrK;
	*/
	T_LDGR_K LdgrK;
	m_wndTarGroupComb.GetSelectedLdgr(LdgrK);
	return LdgrK;
}


void CLdgrChangeDlg::OnTmExecute() 
{
	UpdateData(TRUE);
	T_LDGR_K SrcKey, DestKey;

	SrcKey  = GetSourceGroupKey();
	DestKey = GetTargetGroupKey();
	
	CArray<int, int> TypeArray;
	DataExchange(TypeArray, 1);

	CDBDoc::GetDocPoint()->m_pDataCtrl->ChangeLoadGroup(SrcKey,DestKey,TypeArray, m_nOption+1);
}

void CLdgrChangeDlg::OnTmClose() 
{
	CloseDlg();
}

void CLdgrChangeDlg::OnChangeOption() 
{
	UpdateData();
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

void CLdgrChangeDlg::DataExchange(CArray<int, int>& aItemChecked, int nOperation)
{
	struct T_LIST_BOX_ITEM
	{
		CString str;
		int nCmd;
		int nMask;
	};
	T_LIST_BOX_ITEM aListBoxItem[] = {
		{_LS(IDS_TM_DT_BODF), LT_BODF_CMD, D_PROD_GEN+D_PROD_CVL},    
		{_LS(IDS_TM_DT_CNLD), LT_CNLD_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_SDSP), LT_SDSP_CMD, D_PROD_GEN+D_PROD_CVL+D_PROD_TOW},
		{_LS(IDS_TM_DT_BMLD), LT_BMLD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_FBLA), LT_FBLA_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_FMLD), LT_FMLD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PRES), LT_PRES_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PNLA), LT_PNLA_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_STMP), LT_STMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_NTMP), LT_NTMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_ETMP), LT_ETMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_GTMP), LT_GTMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_BTMP), LT_BTMP_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PTMP), LT_PTMP_CMD, D_PROD_CVL},
		{_LS(IDS_TM_DT_PRST), LT_PRST_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_PTNS), LT_PTNS_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_TDPL), LT_TDPL_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_TMLD), LT_TMLD_CMD, D_PROD_GEN+D_PROD_CVL},
		{_LS(IDS_TM_DT_CRPC), LT_CRPC_CMD, D_PROD_GEN+D_PROD_CVL},
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
	CClientDC dc(&m_wndLoadTypeList);
	aItemChecked.RemoveAll();
	int cxExtent = 0;
	int i, nCount, ix=0;
	nCount = sizeof(aListBoxItem)/sizeof(T_LIST_BOX_ITEM);
	for(i=0; i < nCount; i++)
	{
		if ((nMask & aListBoxItem[i].nMask) == 0) continue;
		
		BOOL bFlag = TRUE;
		if(aListBoxItem[i].str == _LS(IDS_TM_DT_FMLD))
		{
			// RUS에서는 마감하중 제외
			#if defined(_RUS)
				bFlag = FALSE;
			#endif
		}
		
		if(bFlag)
		{
			if (nOperation == 0)
			{
				ix = m_wndLoadTypeList.AddString(aListBoxItem[i].str);
				m_wndLoadTypeList.SetCheck(ix, 0);
				CSize sz = dc.GetTextExtent(aListBoxItem[i].str);
				if (sz.cx > cxExtent)
					cxExtent = sz.cx;
			}
			else
			{
				if (m_wndLoadTypeList.GetCheck(ix++) != 0)
					aItemChecked.Add(aListBoxItem[i].nCmd);
			}
		}
	}

	// 수평 스크롤바 설정
	m_wndLoadTypeList.SetHorizontalExtent(cxExtent);

}
