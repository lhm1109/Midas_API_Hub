// BndrSprsDlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_treemenu.h"


#include "BndrSprsDlg.h"

#include "..\wg_common\wg_commonAll.h"
//#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_Dialog.h"
 
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

//#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\ExportFunc.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrSprsDlg dialog


CBndrSprsDlg::CBndrSprsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrSprsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrSprsDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_arSpring.Add(IDC_TM_SPRS_SDX_EDIT);
	m_arSpring.Add(IDC_TM_SPRS_SDY_EDIT);
	m_arSpring.Add(IDC_TM_SPRS_SDZ_EDIT);
	m_arSpring.Add(IDC_TM_SPRS_SRX_EDIT);
	m_arSpring.Add(IDC_TM_SPRS_SRY_EDIT);
	m_arSpring.Add(IDC_TM_SPRS_SRZ_EDIT);

	m_arSpringUnit.Add(IDC_TM_SPRS_SDX_UNIT);
	m_arSpringUnit.Add(IDC_TM_SPRS_SDY_UNIT);
	m_arSpringUnit.Add(IDC_TM_SPRS_SDZ_UNIT);
	m_arSpringUnit.Add(IDC_TM_SPRS_SRX_UNIT);
	m_arSpringUnit.Add(IDC_TM_SPRS_SRY_UNIT);
	m_arSpringUnit.Add(IDC_TM_SPRS_SRZ_UNIT);
}


void CBndrSprsDlg::Execute()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	CArray<T_NODE_K, T_NODE_K> aNodeKey;
	pIGM->GetSelectedNodeKeyList(aNodeKey);

	if (aNodeKey.GetSize() <= 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}

	CString strTypeName;
	if (m_cboSprsKey.GetCurSel() < 0)
		return;   //m_cboSprsKey.GetWindowText(strTypeName);
	else
		m_cboSprsKey.GetLBText(m_cboSprsKey.GetCurSel(), strTypeName);    // Read Only 일때 적용

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);

	m_GstpKey = m_pDoc->m_pAttrCtrl->GetGstpKey(strTypeName);

	if (m_cboOption.GetCheck())  // Add/Replace
	{
		m_GsprData.Initialize();

		m_GsprData.SpringTypeKey = m_GstpKey;
		m_GsprData.GroupKey = BngrK;

		if (m_pDoc->m_pDataCtrl->AddGspr(aNodeKey, m_GsprData))
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else  //Delete
	{
		if (m_pDoc->m_pDataCtrl->DelGspr(aNodeKey, BngrK))
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CBndrSprsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrSprsDlg)
	DDX_Control(pDX, IDC_TM_SPRS_OPT_ADD, m_cboOption);
	DDX_Control(pDX, IDC_TM_SPRS_STYPE_CBO, m_cboSprsKey);
	DDX_Control(pDX, IDC_TM_SPRS_SDX_EDIT, m_SDxEdit);
	DDX_Control(pDX, IDC_TM_SPRS_SDY_EDIT, m_SDyEdit);
	DDX_Control(pDX, IDC_TM_SPRS_SDZ_EDIT, m_SDzEdit);
	DDX_Control(pDX, IDC_TM_SPRS_SRX_EDIT, m_SRxEdit);
	DDX_Control(pDX, IDC_TM_SPRS_SRY_EDIT, m_SRyEdit);
	DDX_Control(pDX, IDC_TM_SPRS_SRZ_EDIT, m_SRzEdit);
	DDX_Control(pDX, IDC_TM_SPRS_SRZ_UNIT, m_SRzUnit);
	DDX_Control(pDX, IDC_TM_SPRS_SRY_UNIT, m_SRyUnit);
	DDX_Control(pDX, IDC_TM_SPRS_SRX_UNIT, m_SRxUnit);
	DDX_Control(pDX, IDC_TM_SPRS_SDZ_UNIT, m_SDzUnit);
	DDX_Control(pDX, IDC_TM_SPRS_SDY_UNIT, m_SDyUnit);
	DDX_Control(pDX, IDC_TM_SPRS_SDX_UNIT, m_SDxUnit);
	DDX_Control(pDX, IDC_TM_SPRS_MASS_EDT, m_MassEdit);
	DDX_Control(pDX, IDC_TM_SPRS_DAMP_EDT, m_DampEdit);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Radio(pDX, IDC_TM_SPRS_OPT_ADD, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrSprsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrSprsDlg)
	ON_CBN_SELCHANGE(IDC_TM_SPRS_STYPE_CBO, OnSelchangeTmSprsStypeCbo)
	ON_BN_CLICKED(IDC_TM_SPRS_STYPE_BTN, OnTmSprsStypeBtn)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_SPRS_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_SPRS_OPT_DELETE, OnChangeOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrSprsDlg message handlers
BOOL CBndrSprsDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	m_nOption = 0;
	// TODO: Add extra initialization here
	// Option 초기화
	//m_cboOption.SetCheck(TRUE);

	// 에디트 컨트롤 초기화 작업. Unit설정 및 Clear
	for(int i=0; i<6 ;i++)
	{
		((CEditUnit *)GetDlgItem(m_arSpring.GetAt(i)))->SetUnitType(CUnitCtrl::m_GSTP_UNIT.Spring[i]);
		((CTextUnit *)GetDlgItem(m_arSpringUnit.GetAt(i)))->SetUnitType(CUnitCtrl::m_GSTP_UNIT.Spring[i]);
		((CEditUnit *)GetDlgItem(m_arSpring.GetAt(i)))->SetWindowText(_T(""));
	}

	GetDlgItem(IDC_TM_SPRS_MASS_EDT)->SetWindowText(_LS(IDS_TM_BNDR_SPRS_NONE));
	GetDlgItem(IDC_TM_SPRS_DAMP_EDT)->SetWindowText(_LS(IDS_TM_BNDR_SPRS_NONE));

	// DB 에서 Gspr 관련 데이타를 확인한 후 데이타 적용.
	m_pDoc->m_pAttrCtrl->GetGstpKeyList(m_arGstpKey);
	if(m_arGstpKey.GetSize() > 0)
	{
		m_GstpData.Initialize();
		for(int i=0; i<m_arGstpKey.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetGstp(m_arGstpKey.GetAt(i), m_GstpData);
			m_cboSprsKey.AddString(m_GstpData.TypeName);
		}
		m_cboSprsKey.SetCurSel(0);
		OnSelchangeTmSprsStypeCbo(); 
	}
	else
	{
		for(int i=0; i<m_arSpring.GetSize(); i++)
			((CEditUnit *)GetDlgItem(m_arSpring.GetAt(i)))->SetWindowText(_T(""));
	}  

	GetDlgItem(IDC_TM_APPLY)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrSprsDlg::OnSelchangeTmSprsStypeCbo() 
{
	// TODO: Add your control notification handler code here
	CString strTypeName;
	if(m_cboSprsKey.GetCount() > 0)
	{
		m_cboSprsKey.GetLBText(m_cboSprsKey.GetCurSel(), strTypeName);

		m_pDoc->m_pAttrCtrl->GetGstp(strTypeName, m_GstpData);
		m_SDxEdit.SetEditUnit(m_GstpData.Spring[0]);
		m_SDyEdit.SetEditUnit(m_GstpData.Spring[1]);
		m_SDzEdit.SetEditUnit(m_GstpData.Spring[2]);
		m_SRxEdit.SetEditUnit(m_GstpData.Spring[3]);
		m_SRyEdit.SetEditUnit(m_GstpData.Spring[4]);
		m_SRzEdit.SetEditUnit(m_GstpData.Spring[5]);
	}
	else
		for(int i=0; i<m_arSpring.GetSize(); i++)
			((CEditUnit *)GetDlgItem(m_arSpring.GetAt(i)))->SetWindowText(_T("0"));

	CString strMass;
	if(m_GstpData.bMass) strMass = _LS(IDS_TM_BNDR_SPRS_CONSIDERED);
	else                 strMass = _LS(IDS_TM_BNDR_SPRS_NONE);
	GetDlgItem(IDC_TM_SPRS_MASS_EDT)->SetWindowText(strMass);

	CString strDamp;
	if(m_GstpData.bDamping) strDamp = _LS(IDS_TM_BNDR_SPRS_CONSIDERED);
	else                    strDamp = _LS(IDS_TM_BNDR_SPRS_NONE);
	GetDlgItem(IDC_TM_SPRS_DAMP_EDT)->SetWindowText(strDamp);
}

void CBndrSprsDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CBndrSprsDlg::UpdateBuffer()
{
	CString strNewTypeName;

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_GSTP_K Key;
	T_GSTP_D Data;

	BOOL bDrlsChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case UR_GSTP_ADD:
			 pViewBuff->GetGstp(nKey, Key, Data);
			 InsertItem(Key, Data);
			break;

		case UR_GSTP_DEL:
			pViewBuff->GetGstp(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;

		case UR_GSTP_MFD:
			pViewBuff->GetGstp(nKey, Key, Data);
			strNewTypeName = Data.TypeName;
			break;
		case UR_GSTP_MFS:
			pViewBuff->GetGstp(nKey, Key, Data);
			ModifyItem(strNewTypeName, Data.TypeName);
			break;

		default:
			break;
		}
	} // end of while 
}

void CBndrSprsDlg::InsertItem(T_GSTP_K Key, T_GSTP_D Data) 
{
	m_cboSprsKey.AddString(Data.TypeName);
	if(m_cboSprsKey.GetCount() == 1)
	{
		m_cboSprsKey.SetCurSel(0);
		OnSelchangeTmSprsStypeCbo(); 
	}
}

void CBndrSprsDlg::DeleteItem(T_GSTP_K Key, T_GSTP_D Data) 
{
	int nIndex;
	CString strTemp;
	BOOL bMoveRow=FALSE;

	for(int i=0; i<m_cboSprsKey.GetCount(); i++)
	{
		m_cboSprsKey.GetLBText(i, strTemp);
		if(strTemp == Data.TypeName)
		{
			nIndex = i;
			break;
		}
	}
	
	if(m_cboSprsKey.GetCurSel() == nIndex)
		bMoveRow = TRUE;
	
	m_cboSprsKey.DeleteString(nIndex);

	if(bMoveRow)
	{
		if(m_cboSprsKey.GetCount() > 0)
		{
			m_cboSprsKey.SetCurSel(0);
			OnSelchangeTmSprsStypeCbo();
		}
		else
		{
			m_cboSprsKey.SetWindowText(_T(""));
			for(int i=0; i<m_arSpring.GetSize(); i++)
				((CEditUnit *)GetDlgItem(m_arSpring.GetAt(i)))->SetWindowText(_T(""));			
//			m_cboSprsKey.UpdateWindow();
			m_cboSprsKey.RedrawWindow();
		}
	}
}

void CBndrSprsDlg::ModifyItem(CString NewTypeName, CString OldTypeName) 
{
	int nIndex;
	CString strTemp;
	BOOL bMoveRow=FALSE;

	for(int i=0; i<m_cboSprsKey.GetCount(); i++)
	{
		m_cboSprsKey.GetLBText(i, strTemp);
		if(strTemp == OldTypeName)
		{
			nIndex = i;
			break;
		}
	}
	
	if(m_cboSprsKey.GetCurSel() == nIndex)
		bMoveRow = TRUE;
	
	m_cboSprsKey.InsertString(nIndex, NewTypeName);
	m_cboSprsKey.DeleteString(nIndex+1);

	if(bMoveRow)
	{
		m_cboSprsKey.SetCurSel(nIndex);
		OnSelchangeTmSprsStypeCbo();
	}
}

void CBndrSprsDlg::OnTmSprsStypeBtn() 
{
	CCMBndrSprtDlg dlg;
	dlg.DoModal();
}

BOOL CBndrSprsDlg::ExternalInit(UINT key)
{
	
	if(key == 0) return FALSE;

	T_GSPR_D data;
	T_GSPR_K Key;
	Key.keymap = key;
	m_pDoc->m_pAttrCtrl->GetGspr(Key, data);

	T_GSTP_K stpkey = data.SpringTypeKey;
	T_GSTP_D stpdata;
	m_pDoc->m_pAttrCtrl->GetGstp(stpkey, stpdata);

	int nNum = m_cboSprsKey.FindString(-1, stpdata.TypeName);
	m_cboSprsKey.SetCurSel(nNum);

	m_SDxEdit.SetEditUnit(stpdata.Spring[0]);
	m_SDyEdit.SetEditUnit(stpdata.Spring[1]);
	m_SDzEdit.SetEditUnit(stpdata.Spring[2]);
	m_SRxEdit.SetEditUnit(stpdata.Spring[3]);
	m_SRyEdit.SetEditUnit(stpdata.Spring[4]);
	m_SRzEdit.SetEditUnit(stpdata.Spring[5]);

	// Group도 Setting
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);
	
	for(int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if(str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}

	m_cboOption.SetCheck(TRUE);
	UpdateData(FALSE);
	return TRUE;
	
}

void CBndrSprsDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMBndrGrupDefDlg::IDD);
}

void CBndrSprsDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_nOption == 1) //Delete
	{
		this->m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		this->m_wndGroupCombo.ShowHideAll(FALSE);
	}
	
}
