// DgnGenCmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenCmDlg_CH.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_CMFT

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenCmDlg_CH dialog


CDgnGenCmDlg_CH::CDgnGenCmDlg_CH(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenCmDlg_CH::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenCmDlg_CH)
	m_nOption = 0;
	m_Cmy = 0.0;
	m_Cmz = 0.0;
	m_Cty = 0.0;
	m_Ctz = 0.0;
	m_bFormulaCmy = FALSE;
	m_bFormulaCmz = FALSE;
	m_bFormulaCty = FALSE;
	m_bFormulaCtz = FALSE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnGenCmDlg_CH::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenCmDlg_CH)
	DDX_Radio(pDX, IDC_DGN_CM_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CM_CMY, m_Cmy);
	DDX_Text(pDX, IDC_DGN_CM_CMZ, m_Cmz);
	DDX_Text(pDX, IDC_DGN_CM_CTY, m_Cty);
	DDX_Text(pDX, IDC_DGN_CM_CTZ, m_Ctz);
	DDX_Check(pDX, IDC_DGN_CM_AUTOCM_Y, m_bFormulaCmy);
	DDX_Check(pDX, IDC_DGN_CM_AUTOCM_Z, m_bFormulaCmz);
	DDX_Check(pDX, IDC_DGN_CM_AUTOCT_Y, m_bFormulaCty);
	DDX_Check(pDX, IDC_DGN_CM_AUTOCT_Z, m_bFormulaCtz);
	DDX_Control(pDX, IDC_DGN_CM_AUTOCMY_COMBO, m_wndCombxCmy);
	DDX_Control(pDX, IDC_DGN_CM_AUTOCMZ_COMBO, m_wndCombxCmz);
	DDX_Control(pDX, IDC_DGN_CM_AUTOCTY_COMBO, m_wndCombxCty);
	DDX_Control(pDX, IDC_DGN_CM_AUTOCTZ_COMBO, m_wndCombxCtz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenCmDlg_CH, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenCmDlg_CH)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CM_AR, OnDgnCmAr)
	ON_BN_CLICKED(IDC_DGN_CM_DEL, OnDgnCmDel)
	ON_BN_CLICKED(IDC_DGN_CM_AUTOCM_Y, OnDgnCmAutoCmy)
	ON_BN_CLICKED(IDC_DGN_CM_AUTOCM_Z, OnDgnCmAutoCmz)
	ON_BN_CLICKED(IDC_DGN_CM_AUTOCT_Y, OnDgnCmAutoCty)
	ON_BN_CLICKED(IDC_DGN_CM_AUTOCT_Z, OnDgnCmAutoCtz)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenCmDlg_CH message handlers

void CDgnGenCmDlg_CH::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenCmDlg_CH::OnDgnExecute() 
{
	UpdateData(TRUE);
	if (!ErrorCheck())
		return;
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key,eData);
			// Beam, Truss, Wall.
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp) || m_pDoc->m_pAttrCtrl->IsWall(eData.eltyp))	rSelKey.Add(key);
		}
		T_CMFT_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			rData.Cmy	  = m_Cmy;
			rData.Cmz	  = m_Cmz;
			rData.Cty	  = m_Cty;
			rData.Ctz	  = m_Ctz;
			rData.bFormulaCmy = m_bFormulaCmy;
			rData.bFormulaCmz = m_bFormulaCmz;
			rData.bFormulaCty = m_bFormulaCty;
			rData.bFormulaCtz = m_bFormulaCtz;
			rData.nFormulaCmy = m_wndCombxCmy.GetCurSel();
			rData.nFormulaCmz = m_wndCombxCmz.GetCurSel();
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddCmft(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelCmft(rSelKey))	Initial_SelectItem();
	  }
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnGenCmDlg_CH::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

BOOL CDgnGenCmDlg_CH::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	// 선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	Initial_SelectItem();
	InitialFormulaCombx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenCmDlg_CH::InitialFormulaCombx()
{
	m_wndCombxCmy.ResetContent();
	m_wndCombxCmz.ResetContent();
	m_wndCombxCty.ResetContent();
	m_wndCombxCtz.ResetContent();

	CString strFormula[] = { _T("0.6+0.4M2/M1"), _T("1-0.36N/Ncr"), _T("1-0.18N/Ncr"), _T("1-0.36(1-m)N/Ncr") };
	CString strCtFormula = _T("0.65+0.35M2/M1");
	for (int i=0; i<4; i++)
	{
		m_wndCombxCmy.InsertString(i,strFormula[i]);
		m_wndCombxCmz.InsertString(i, strFormula[i]);
	}
	m_wndCombxCty.AddString(strCtFormula);
	m_wndCombxCtz.AddString(strCtFormula);
	m_wndCombxCmy.SetCurSel(0);
	m_wndCombxCmz.SetCurSel(0);
	m_wndCombxCty.SetCurSel(0);
	m_wndCombxCtz.SetCurSel(0);
}

void CDgnGenCmDlg_CH::Initial_Data()
{
	m_nOption	= 0;	// add/replace
	m_Cmy = 1.0;
	m_Cmz = 1.0;
	m_Cty = 1.0;
	m_Ctz = 1.0;
	m_bFormulaCmy = TRUE;
	m_bFormulaCmz = TRUE;
	m_bFormulaCty = TRUE;
	m_bFormulaCtz = TRUE;
	GetDlgItem(IDC_DGN_CM_CMY)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CM_AUTOCMY_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CM_CMZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CM_AUTOCMZ_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CM_CTY)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CM_AUTOCTY_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CM_CTZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CM_AUTOCTZ_COMBO)->EnableWindow(TRUE);
	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

BOOL CDgnGenCmDlg_CH::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(!m_bFormulaCmy)		//	Not Automation
	{
		if(m_Cmy <= 0 || m_Cmy > 1.)	bCheck = FALSE;
	}
	if (!m_bFormulaCmz)		//	Not Automation
	{
		if (m_Cmz <= 0 || m_Cmz > 1.)	bCheck = FALSE;
	}
	if (!m_bFormulaCty)		//	Not Automation
	{
		if (m_Cty <= 0 || m_Cty > 1.)	bCheck = FALSE;
	}
	if (!m_bFormulaCtz)		//	Not Automation
	{
		if (m_Ctz <= 0 || m_Ctz > 1.)	bCheck = FALSE;
	}
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_MOM_FACTOR),MB_OK);

	return bCheck;
}

void CDgnGenCmDlg_CH::OnDgnCmAutoCmy()
{
	UpdateData(TRUE);
	if (m_bFormulaCmy)
	{
		m_Cmy = 1.0;
		GetDlgItem(IDC_DGN_CM_CMY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_AUTOCMY_COMBO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CM_CMY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_AUTOCMY_COMBO)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CDgnGenCmDlg_CH::OnDgnCmAutoCmz()
{
	UpdateData(TRUE);
	if (m_bFormulaCmz)
	{
		m_Cmz = 1.0;
		GetDlgItem(IDC_DGN_CM_CMZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_AUTOCMZ_COMBO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CM_CMZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_AUTOCMZ_COMBO)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CDgnGenCmDlg_CH::OnDgnCmAutoCty()
{
	UpdateData(TRUE);
	if (m_bFormulaCty)
	{
		m_Cty = 1.0;
		GetDlgItem(IDC_DGN_CM_CTY)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_AUTOCTY_COMBO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CM_CTY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_AUTOCTY_COMBO)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CDgnGenCmDlg_CH::OnDgnCmAutoCtz()
{
	UpdateData(TRUE);
	if (m_bFormulaCtz)
	{
		m_Ctz = 1.0;
		GetDlgItem(IDC_DGN_CM_CTZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CM_AUTOCTZ_COMBO)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CM_CTZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CM_AUTOCTZ_COMBO)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

void CDgnGenCmDlg_CH::OnDgnCmAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnGenCmDlg_CH::OnDgnCmDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('01.1.3).
void CDgnGenCmDlg_CH::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				// Add by ZINU.('01.1.3).
				Update_InitDataByCode();
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

// Add by ZINU.('01.1.3).
void CDgnGenCmDlg_CH::Update_InitDataByCode()
{
	CString strInitCmy = _T("Cmy"), strInitCmz = _T("Cmz");
	CString strCmy = m_pDoc->GetDesignParamNa(strInitCmy);
	CString strCmz = m_pDoc->GetDesignParamNa(strInitCmz);
	GetDlgItem(IDC_DGN_CMY_STATIC)->SetWindowText(strCmy + _T(" :"));
	GetDlgItem(IDC_DGN_CMZ_STATIC)->SetWindowText(strCmz + _T(" :"));

	CString strInitCty = _T("Cty"), strInitCtz = _T("Ctz");
	CString strCty = m_pDoc->GetDesignParamNa(strInitCty);
	CString strCtz = m_pDoc->GetDesignParamNa(strInitCtz);
	GetDlgItem(IDC_DGN_CTY_STATIC)->SetWindowText(strCty + _T(" :"));
	GetDlgItem(IDC_DGN_CTZ_STATIC)->SetWindowText(strCtz + _T(" :"));

	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);

}