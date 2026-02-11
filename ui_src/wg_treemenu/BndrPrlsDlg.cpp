// BndrPrlsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrPrlsDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrPrlsDlg dialog


CBndrPrlsDlg::CBndrPrlsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrPrlsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrPrlsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aCtrlOption.Add(IDC_TM_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE);

	m_aCtrlDisable.Add(IDC_TM_CHECK_FX1);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FX2);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FX3);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FX4);

	m_aCtrlDisable.Add(IDC_TM_CHECK_FY1);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FY2);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FY3);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FY4);

	m_aCtrlDisable.Add(IDC_TM_CHECK_FZ1);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FZ2);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FZ3);
	m_aCtrlDisable.Add(IDC_TM_CHECK_FZ4);

	m_aCtrlDisable.Add(IDC_TM_CHECK_MY1);
	m_aCtrlDisable.Add(IDC_TM_CHECK_MY2);
	m_aCtrlDisable.Add(IDC_TM_CHECK_MY3);
	m_aCtrlDisable.Add(IDC_TM_CHECK_MY4);

	m_aCtrlDisable.Add(IDC_TM_CHECK_MZ1);
	m_aCtrlDisable.Add(IDC_TM_CHECK_MZ2);
	m_aCtrlDisable.Add(IDC_TM_CHECK_MZ3);
	m_aCtrlDisable.Add(IDC_TM_CHECK_MZ4);

	m_aCtrlDisable.Add(IDC_TM_ALLPIN);
	m_aCtrlDisable.Add(IDC_TM_ALLFIX);
}


void CBndrPrlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrPrlsDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC1, m_wndPicture);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrPrlsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrPrlsDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_ALLPIN, OnTmAllpin)
	ON_BN_CLICKED(IDC_TM_ALLFIX, OnTmAllfix)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnTmOptDelete)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnTmOptAdd)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrPrlsDlg message handlers

BOOL CBndrPrlsDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, 0);
	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_prls 1.svg"));

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrPrlsDlg::OnTmClose() 
{
	CloseDlg();
}

void CBndrPrlsDlg::OnTmExecute() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	//CArray<T_PRLS_K, T_PRLS_K> aSelKey;
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_PRLS_D data;
	T_ELEM_D elem;
	int nOption;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	int nSize = aSelKey.GetSize();
	for (int i = nSize-1; i >= 0; i--)
	{
		pDoc->m_pAttrCtrl->GetElem(aSelKey[i], elem);
		if (!pDoc->m_pAttrCtrl->IsPlate(elem.eltyp))
			aSelKey.RemoveAt(i);
	}

	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_plate_element_));
		return;
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	BOOL bSuccess;
	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	data.GroupKey = BngrK;
	
	if (nOption == 0)
	{
		UINT nID[] = {
			IDC_TM_CHECK_FX1, IDC_TM_CHECK_FX2, IDC_TM_CHECK_FX3, IDC_TM_CHECK_FX4,
			IDC_TM_CHECK_FY1, IDC_TM_CHECK_FY2, IDC_TM_CHECK_FY3, IDC_TM_CHECK_FY4,
			IDC_TM_CHECK_FZ1, IDC_TM_CHECK_FZ2, IDC_TM_CHECK_FZ3, IDC_TM_CHECK_FZ4,
			IDC_TM_CHECK_MY1, IDC_TM_CHECK_MY2, IDC_TM_CHECK_MY3, IDC_TM_CHECK_MY4,
			IDC_TM_CHECK_MZ1, IDC_TM_CHECK_MZ2, IDC_TM_CHECK_MZ3, IDC_TM_CHECK_MZ4,
		};

		for(int i=0 ; i<4 ; i++)  // N1,N2,N3,N4
			for(int j =0 ; j<5 ; j++)  // Direction
			{
				data.Release[i][j] = this->IsDlgButtonChecked(nID[i+j*4]) ? '1' : '0';
			}   

		bSuccess = pDoc->m_pDataCtrl->AddPrls(aSelKey, data);
	}
	else
		bSuccess = pDoc->m_pDataCtrl->DelPrls(aSelKey,BngrK);

	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CBndrPrlsDlg::OnTmAllpin() 
{
	CtrlSetCheck(this, m_aCtrlDisable, TRUE);  //모두 체크
}

void CBndrPrlsDlg::OnTmAllfix() 
{
	CtrlSetCheck(this, m_aCtrlDisable, FALSE); //모두 해제
}

void CBndrPrlsDlg::OnTmOptDelete() 
{
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, FALSE); // 체크박스 Disable 시킴 
	this->m_wndGroupCombo.ShowHideAll(TRUE);
}

void CBndrPrlsDlg::OnTmOptAdd() 
{
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, TRUE); // 체크박스 Enable 시킴
	this->m_wndGroupCombo.ShowHideAll(FALSE);
}

void CBndrPrlsDlg::CtrlSetCheck(CWnd* pParent, CArray<UINT,UINT> &aCtrlID, int nCheck)
{
	CButton * pWnd;
	int nNum = aCtrlID.GetSize();
	for(int i = 0 ; i < nNum; i++)
	{
		pWnd = (CButton*)pParent->GetDlgItem(aCtrlID[i]);
		if (pWnd == 0 || pWnd->GetSafeHwnd() == 0) 
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Program_Error___No_dialog_item_id__), aCtrlID[i]);
			continue;
		}
		pWnd->SetCheck(nCheck);
	}
}

BOOL CBndrPrlsDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_PRLS_D data;
	T_PRLS_K Key;
	Key.keymap = key;
	pDoc->m_pAttrCtrl->GetPrls(Key, data);

	UINT nID[] = {
		IDC_TM_CHECK_FX1, IDC_TM_CHECK_FX2, IDC_TM_CHECK_FX3, IDC_TM_CHECK_FX4,
		IDC_TM_CHECK_FY1, IDC_TM_CHECK_FY2, IDC_TM_CHECK_FY3, IDC_TM_CHECK_FY4,
		IDC_TM_CHECK_FZ1, IDC_TM_CHECK_FZ2, IDC_TM_CHECK_FZ3, IDC_TM_CHECK_FZ4,
		IDC_TM_CHECK_MY1, IDC_TM_CHECK_MY2, IDC_TM_CHECK_MY3, IDC_TM_CHECK_MY4,
		IDC_TM_CHECK_MZ1, IDC_TM_CHECK_MZ2, IDC_TM_CHECK_MZ3, IDC_TM_CHECK_MZ4,
	};

	for(int i=0 ; i < 4 ; i++)  // N1,N2,N3,N4
	{
		for(int j=0 ; j < 5 ; j++)  // Direction
		{
			CButton* bt = (CButton*)GetDlgItem(nID[i+j*4]);
			if(data.Release[i][j] == '1')	bt->SetCheck(TRUE);
			else bt->SetCheck(FALSE);
		}
	}
	// Group도 Setting
	T_BNGR_D  bngrD;
	pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, 0);
	UpdateData(FALSE);
	return TRUE;
}

void CBndrPrlsDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMBndrGrupDefDlg::IDD);
}

