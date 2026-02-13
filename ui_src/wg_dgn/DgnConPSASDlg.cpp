// DgnConPSASDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConPSASDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\\wg_db\\wg_db_DataCtrl.h"
#include "..\\wg_db\\wg_db_UnitCtrl.h"
#include "..\\wg_db\\wg_db_AttrCtrl.h"
#include "..\\wg_db\\wg_db_ViewCtrl.h"
#include "..\\wg_db\\wg_db_InitCtrl.h"
#include "..\wg_main\wg_mainres2.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConPSASDlg dialog

CDgnConPSASDlg::CDgnConPSASDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConPSASDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConPSASDlg)
	m_nOption = -1;
	m_TranComp = 0.0;
	m_TranSupT = 0.0;
	m_TranMidT = 0.0;
	m_ServSusC = 0.0;
	m_ServTotC = 0.0;
	m_ServTens = 0.0;
	m_strCode = _T("");
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConPSASDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConPSASDlg)
	DDX_Radio(pDX, IDC_DGN_CON_PSAS_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CON_PSAS_TRANCOMP, m_TranComp);
	DDX_Text(pDX, IDC_DGN_CON_PSAS_TRANSUPT, m_TranSupT);
	DDX_Text(pDX, IDC_DGN_CON_PSAS_TRANMIDT, m_TranMidT);
	DDX_Text(pDX, IDC_DGN_CON_PSAS_SERVSUSC, m_ServSusC);
	DDX_Text(pDX, IDC_DGN_CON_PSAS_SERVTOTC, m_ServTotC);
	DDX_Text(pDX, IDC_DGN_CON_PSAS_SERVTENS, m_ServTens);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConPSASDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConPSASDlg)
	ON_BN_CLICKED(IDC_DGN_CON_PSAS_AR, OnDgnDfnstressAr)
	ON_BN_CLICKED(IDC_DGN_CON_PSAS_DEL, OnDgnDfnstressDel)
	ON_BN_CLICKED(IDC_DGN_CON_PSAS_UPDATE_BTN, OnDgnPSASUpdateBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConPSASDlg message handlers
void CDgnConPSASDlg::OnDgnExecute() 
{
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
			// Beam, Truss.
			if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rSelKey.Add(key);
		}
		T_PSAS_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);
		  rData.dTranComp	= m_TranComp;
			rData.dTranSupT	= m_TranSupT;
			rData.dTranMidT	= m_TranMidT;
	  	rData.dServSusC	= m_ServSusC;
			rData.dServTotC	= m_ServTotC;
		  rData.dServTens	= m_ServTens;
			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddPsas(rSelKey,rData))	Initial_SelectItem();//rsh
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelPsas(rSelKey))	Initial_SelectItem();//rsh
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnConPSASDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnConPSASDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

BOOL CDgnConPSASDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Default 값 초기화	
	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConPSASDlg::Initial_Data()
{
	m_nOption = 0;      //  add/replace

	Update_InitDataByCode();
	// Change by ZINU.('05.05.04). Local Coding -> Set_DefaultByCode().
	Set_DefaultByCode(m_strCode);
	UpdateData(FALSE);
}

void CDgnConPSASDlg::OnDgnDfnstressAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConPSASDlg::OnDgnDfnstressDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

// Add by ZINU.('01.1.3).
void CDgnConPSASDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnConPSASDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DGN_CON_PSAS, strTitle, bShow, m_pDoc->IsPostMode());
	if(bShow)
	{
		GetDlgItem(IDC_DGN_CON_PSAS_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_TRANCOMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_TRANSUPT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_TRANMIDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_SERVSUSC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_SERVTOTC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_SERVTENS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_PSAS_UPDATE_BTN)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_PSAS_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_TRANCOMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_TRANSUPT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_TRANMIDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_SERVSUSC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_SERVTOTC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_SERVTENS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_PSAS_UPDATE_BTN)->EnableWindow(FALSE);
	}

	m_strCode = _T("");
	T_DCON_D rData;
	rData.Initialize();
	// Exist Concrete Control Data at DB.
	m_strCode = CDBLib::GetConCodeName();

	ChangeDlgItemInfo();
}

void CDgnConPSASDlg::OnDgnPSASUpdateBtn() 
{
	Set_DefaultByCode(m_strCode);
	UpdateData(FALSE);	
}

BOOL CDgnConPSASDlg::Set_DefaultByCode(CString strCode)
{
	if(strCode==_T(""))	return FALSE;
	CDgnDataCtrl DgnDataCtrl;
	double dFtsc=0.0, dFtst_s=0.0, dFtst_m=0.0, dFssc_s=0.0, dFssc_t=0.0, dFsst=0.0;
	if(!DgnDataCtrl.Get_DgnConPsasDef(strCode,dFtsc,dFtst_s,dFtst_m,dFssc_s,dFssc_t,dFsst)) ASSERT(0);
	
	m_TranComp = dFtsc; 
	m_TranSupT = dFtst_s;
	m_TranMidT = dFtst_m;
	m_ServSusC = dFssc_s; 
	m_ServTotC = dFssc_t; 
	m_ServTens = dFsst; 

	return TRUE;
}

void CDgnConPSASDlg::ChangeDlgItemInfo()
{

	GetDlgItem(IDC_DGN_CON_PSAS_SERVSUSC)->EnableWindow(FALSE);

	CString strfck = _T("");
	CString strfck05 = _T("");

	if(m_strCode == _T("ACI318-99") || m_strCode == _T("ACI318-02"))
	{
		GetDlgItem(IDC_DGN_CON_PSAS_SERVSUSC)->EnableWindow(TRUE);
		strfck = _T("fc");
	}
	else
	{
		strfck = _T("fck");
	}
	
	strfck05.Format(_T("%s^0.5"),strfck);

	GetDlgItem(IDC_DGN_CON_PSAS_SERVSUSC_STR)->SetWindowText(strfck);
	GetDlgItem(IDC_DGN_CON_PSAS_SERVTOTC_STR)->SetWindowText(strfck);
	GetDlgItem(IDC_DGN_CON_PSAS_SERVTENS_STR)->SetWindowText(strfck05);

}