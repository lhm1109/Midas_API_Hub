/////////////////////////////////////////////////////////////////////////////////////////////////
//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
/////////////////////////////////////////////////////////////////////////////////////////////////

// DgnConCvlUlsParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCvlUlsParamDlg.h"

#include "DgnStruct.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCvlUlsParamDlg dialog


CDgnConCvlUlsParamDlg::CDgnConCvlUlsParamDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConCvlUlsParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConCvlUlsParamDlg)
	m_nOption     = -1;
	m_dUlsParam1  = 0.0;  
	m_dUlsParam2  = 0.0;  
	m_dUlsParam3  = 0.0;  
	m_dUlsParam4  = 0.0;  
	m_dUlsParam5  = 0.0;  
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConCvlUlsParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCvlUlsParamDlg)
	DDX_Radio(pDX, IDC_DGN_CON_ULEC_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_CONC_ULS_PARAM1, m_dUlsParam1);
	DDX_Text(pDX, IDC_DGN_CONC_ULS_PARAM2, m_dUlsParam2);
	DDX_Text(pDX, IDC_DGN_CONC_ULS_PARAM3, m_dUlsParam3);
	DDX_Text(pDX, IDC_DGN_CONC_ULS_PARAM4, m_dUlsParam4);
	DDX_Text(pDX, IDC_DGN_CONC_ULS_PARAM5, m_dUlsParam5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConCvlUlsParamDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConCvlUlsParamDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,          OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE,        OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_ULEC_AR,    OnDgnCtcAr)
	ON_BN_CLICKED(IDC_DGN_CON_ULEC_DEL,   OnDgnCtcDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCvlUlsParamDlg message handlers

void CDgnConCvlUlsParamDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConCvlUlsParamDlg::OnDgnExecute() 
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
			// Beam.
			if(m_pDoc->m_pAttrCtrl->IsBeam(eData.eltyp))	rSelKey.Add(key);
		}
		T_ULEC_D rData;
		rData.Initialize();
		if(m_nOption==0)	// add/replace
	  {
		  UpdateData(TRUE);

			//(2009.06.15) Add by Unsang :: Data Checking
			if(!ErrorCheck()) return;

		  rData.dCRdc     = m_dUlsParam1;
			rData.dNumin    = m_dUlsParam2;
			rData.dk1       = m_dUlsParam3;
			rData.dNu1      = m_dUlsParam4;
			rData.dAlphacw  = m_dUlsParam5;
			
			if(rSelKey.GetSize() > 0)
			{
		    // Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddUlec(rSelKey,rData))	Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK); //TEST// Static 변경.
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelUlec(rSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnConCvlUlsParamDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnConCvlUlsParamDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	double dFlen = 0.0;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
		// do something...
			break;
		case D_UPDATE_UNIT:
		// do something...
			break;
		case D_UPDATE_SEL_ADD:
		// do something...
			break;
		case D_UPDATE_SEL_DEL:
		// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			// do something...
			UpdateBuffer();
			break;
		case D_UPDATE_PREF_CHANGED:
			break;
		default:
			//ASSERT(FALSE);
			break;
	}
}

BOOL CDgnConCvlUlsParamDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Set Current Design Code.
	GetDesignCodeNo(CDBLib::GetConCodeName());

	// Default 값 초기화
	Initial_Data();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCvlUlsParamDlg::Initial_Data()
{
	m_nOption     = 0;    //	add/replace
	m_dUlsParam1  = 0.0;	//	National Annex확인후 SET!!
	m_dUlsParam2  = 0.0;	//	National Annex확인후 SET!!
	m_dUlsParam3  = 0.0;	//	National Annex확인후 SET!!
	m_dUlsParam4  = 0.0;	//	National Annex확인후 SET!!
	m_dUlsParam5  = 0.0;	//	National Annex확인후 SET!!

	//
	Update_InitDlgItems();
	UpdateData(FALSE);
}

BOOL CDgnConCvlUlsParamDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;
	CString strMsg = _T("");

	//TEST// Message 생성할 것!!!
	if(m_dUlsParam1 <= 0.0)
	{
		bCheck = FALSE;
		if(strMsg.GetLength()!=0) strMsg += _T("\n");
		strMsg += _LS(IDS_DGN_CHK_CRDC_FACTOR);
	}
	
	if(m_dUlsParam2 <= 0.0)
	{
		bCheck = FALSE;
		if(strMsg.GetLength()!=0) strMsg += _T("\n");
		strMsg += _LS(IDS_DGN_CHK_NUMIN_FACTOR);
	}
	
	if(m_dUlsParam3 <= 0.0)
	{
		bCheck = FALSE;
		if(strMsg.GetLength()!=0) strMsg += _T("\n");
		strMsg += _LS(IDS_DGN_CHK_K1_FACTOR);
	}

	if(m_dUlsParam4 <= 0.0)
	{
		bCheck = FALSE;
		if(strMsg.GetLength()!=0) strMsg += _T("\n");
		strMsg += _LS(IDS_DGN_CHK_NU1_FACTOR);
	}

	if(m_dUlsParam5 <= 0.0)
	{
		bCheck = FALSE;
		if(strMsg.GetLength()!=0) strMsg += _T("\n");
		strMsg += _LS(IDS_DGN_CHK_ALPHACW_FACTOR);
	}  

	if(!bCheck)
		AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}


void CDgnConCvlUlsParamDlg::OnDgnCtcAr() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConCvlUlsParamDlg::OnDgnCtcDel() 
{
	// TODO: Add your control notification handler code here
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnConCvlUlsParamDlg::EnableItems(BOOL bEnable)
{
	// GroupBox
	GetDlgItem(IDC_STATIC_OPTION)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_ULS_PARAM)->EnableWindow(bEnable);

	// STATIC
	GetDlgItem(IDC_DGN_CONC_ULS_STC1)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_STC2)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_STC3)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_STC4)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_STC5)->EnableWindow(bEnable);

	// EDITBOX
	GetDlgItem(IDC_DGN_CONC_ULS_PARAM1)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_PARAM2)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_PARAM3)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_PARAM4)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_ULS_PARAM5)->EnableWindow(bEnable);

	// RADIOBUTTON
	GetDlgItem(IDC_DGN_CON_ULEC_AR)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_ULEC_DEL)->EnableWindow(bEnable);

	// BUTTON
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bEnable);
}

// 아이템들을 enable / disable 시킨다.
void CDgnConCvlUlsParamDlg::Update_InitDlgItems()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCon(ID_DESIGN_CONC_CVL_ULS_FACTOR, strTitle, bShow, m_pDoc->IsPostMode());

	EnableItems(bShow);    
}


void CDgnConCvlUlsParamDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;		
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			case(UR_DCON_ADD):  // Design Code 변경시.
			case(UR_DCON_DEL):
			{
				GetDesignCodeNo(CDBLib::GetConCodeName());  // Set Design Code to Enum.
				Initial_Data();                             // Initialize the data.
				break;      
			}
			default:	break;
		}
	}
}

void CDgnConCvlUlsParamDlg::GetDesignCodeNo(CString strCode) 
{
	if     (strCode==CONCODE_KCI_USD99)	    m_iCode = KCI_USD99;
	else if(strCode==CONCODE_KSCE_USD96)	m_iCode = KSCE_USD96;
	else if(strCode==CONCODE_AASHTO_LRFD02) m_iCode = AASHTO_LC02;
	else if(strCode==CONCODE_AASHTO_LFD96)  m_iCode = AASHTO_LC96;
	else if(strCode==CONCODE_AASHTO_LRFD12) m_iCode = AASHTO_LC12;
	else if(strCode==CONCODE_AASHTO_LRFD16) m_iCode = AASHTO_LC16;
    else if(strCode==CONCODE_AASHTO_LRFD17) m_iCode = AASHTO_LC17;
    else if(strCode==CONCODE_AASHTO_LRFD20) m_iCode = AASHTO_LC20;
	else if(strCode==CONCODE_AASHTO_LRFD24) m_iCode = AASHTO_LC24;
	else if(strCode==CONCODE_ACI318_02)	    m_iCode = ACI318_02;
	else if(strCode==CONCODE_JTJ023_85)     m_iCode = JTJ023_85;
	else if(strCode==CONCODE_CSA_S6_00)     m_iCode = CSA_S6_00;
    else if(strCode==CONCODE_CSA_S6_14)     m_iCode = CSA_S6_14_RC;
	else if(strCode==CONCODE_CSA_S6_19)     m_iCode = CSA_S6_19_RC;
	else if(strCode==CONCODE_IRC21_2000)    m_iCode = IRC_21_00;
	else if(strCode==CONCODE_IRC112_2011)   m_iCode = IRC_112_11;
	else if(strCode==CONCODE_IRC112_2020)   m_iCode = IRC_112_20;
	else if(strCode==CONCODE_TWN_BRG_LSD90) m_iCode = TWN_BRG_LC90;
	else if(strCode==CONCODE_EC2_2_05)		m_iCode = Eurocode2_2_05;
	else ASSERT(0);
}