// DgnConBeamBarSectDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamBarSectDlg.h"


#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamBarSectDlg dialog


CDgnConBeamBarSectDlg::CDgnConBeamBarSectDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnConBeamBarSectDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	//m_CurData 를 꼭 초기화한다

	//{{AFX_DATA_INIT(CDgnConBeamBarSectDlg)	
	m_bApplyAIJ = FALSE;
	m_nAIJ = 0;
	m_nOption = 0;
	m_nAddReplace = 0;
	m_bDoublyRein = FALSE;
	m_dDoublyRein = 0.0;
	m_nDgnClass = 0;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_bConsiderSpacingLimitBeam = TRUE;
	//}}AFX_DATA_INIT

	//m_pParent = (CDgnConBeamBarSectDlg*)pParent;
}


CDgnConBeamBarSectDlg::~CDgnConBeamBarSectDlg()
{
}

void CDgnConBeamBarSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamBarSectDlg)
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_MAIN_COMBO, m_ctrMainCombo);
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_STIR_COMBO, m_ctrStirCombo);
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_ARRA_COMBO, m_ctrArraCombo);
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_SIDE_COMBO, m_ctrSideCombo);	
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_DT_EDIT, m_ctrDT);
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_DB_EDIT, m_ctrDB);
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_DT_UNIT, m_ctrDTUnit);
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_DB_UNIT, m_ctrDBUnit);
	DDX_Check(pDX, IDC_DGN_CON_BEAM_BAR_AIJ_CHK, m_bApplyAIJ);
	DDX_Radio(pDX, IDC_DGN_CON_BEAM_BAR_AIJSIDE_RADIO, m_nAIJ);
	DDX_Radio(pDX, IDC_DGN_CON_BEAM_BAR_OPTION0_RADIO, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CON_BEAM_BAR_ADD_RADIO, m_nAddReplace);
	DDX_Check(pDX, IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_CHK, m_bDoublyRein);
	DDX_Control(pDX, IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_EDIT, m_DoublyReinEdit);
	DDX_Text(pDX, IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_EDIT, m_dDoublyRein);
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	DDX_Check(pDX, IDC_DGN_CON_BSPACE_LIMIT, m_bConsiderSpacingLimitBeam);
	DDX_Radio(pDX, IDC_DGN_CON_BEAM_BAR_DCH_RADIO, m_nDgnClass);
	//}}AFX_DATA_MAP
}

BOOL CDgnConBeamBarSectDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	SetInitUnit();	

	ControlMan(TRUE);
	InitData();	
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CDgnConBeamBarSectDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnConBeamBarSectDlg)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_BAR_AIJ_CHK, OnDgnUseAIJCheck)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_CHK, OnDgnDoublyReinCheck)
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	ON_BN_CLICKED(IDC_DGN_CON_BSPACE_LIMIT,OnDgnConsiderSpacingLimitCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearRebar_I message handlers


BOOL CDgnConBeamBarSectDlg::DestroyWindow() 
{
	return CChildDialog::DestroyWindow();
}

// Coded by SeungJun MNET:3466  '20080529
void CDgnConBeamBarSectDlg::InitData() 
{
	m_ctrArraCombo.ResetContent();

	CStringArray arStirrupNum;
	CDBLib::GetSubRebarNumList(arStirrupNum, TRUE);
	for(int i=0; i<arStirrupNum.GetSize(); i++)
	{
		m_ctrArraCombo.AddString(arStirrupNum[i]);	
	}
	
	SetRebarCombo();  
}

void CDgnConBeamBarSectDlg::SetRebarCombo() 
{
	if(m_ctrMainCombo.GetCount() > 0)	m_ctrMainCombo.ResetContent();
	if(m_ctrStirCombo.GetCount() > 0)	m_ctrStirCombo.ResetContent();
	if(m_ctrSideCombo.GetCount() > 0)	m_ctrSideCombo.ResetContent();

	CDBLib::GetRebarNameAtComboBox(&m_ctrMainCombo);
	CDBLib::GetRebarNameAtComboBox(&m_ctrStirCombo);
	CDBLib::GetRebarNameAtComboBox(&m_ctrSideCombo);

	InitDlgData();
}

void CDgnConBeamBarSectDlg::SetInitUnit() 
{
	m_ctrDT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrDB.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrDTUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_ctrDBUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}

void CDgnConBeamBarSectDlg::InitDlgData()
{
	m_CurData.Initialize();
	// Modify by sshan. 김민정.('20070402)
	// 디폴트 값 0.5에서 0.0으로 수정
	// Coded by Seungjun ('20071113) MNet:No.3087. Default 1.0으로.
	m_CurData.dDoublyBeam = 1.0;// Modify by sshan('20070130) 김승준 요청 Dlg Default 0.5

	//080401 sshan DCRB(전체철근정보)데이터로 초기값을 셋팅 (from 이진우)
	T_DCRB_D DataDcrb; DataDcrb.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetDcrb(DataDcrb))
	{
		m_CurData.MainRebarBeam						= DataDcrb.MainRebarBeam[0];
		m_CurData.StirrupBarBeam					= DataDcrb.StirrupBarBeam;
		m_CurData.SideBarBeam							= DataDcrb.SideBarBeam;
		m_CurData.dT_Beam									= DataDcrb.dT_Beam;
		m_CurData.dB_Beam									= DataDcrb.dB_Beam;
		m_CurData.iSubRebarNum_Beam				= DataDcrb.iSubRebarNum_Beam;
		m_CurData.bSpliceBeamJP						= DataDcrb.bSpliceBeamJP;
		m_CurData.iSpliceBeam							= DataDcrb.iSpliceBeam;
		m_CurData.iSpliceBeamSubType			= DataDcrb.iSpliceBeamSubType;
		m_CurData.bDoublyBeam							= DataDcrb.bDoublyBeam;
		m_CurData.dDoublyBeam							= DataDcrb.dDoublyBeam;
		m_CurData.bCheckRebarSpacing_Beam = DataDcrb.bCheckRebarSpacing_Beam;		
	}

	T_DCON_D DataDcon; DataDcon.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetDcon(DataDcon))
		m_CurData.iDgnClass = DataDcon.nClass;		

	Data2Dlg();
}

BOOL CDgnConBeamBarSectDlg::Data2Dlg()
{
	m_nAddReplace = 0;

	int Index = 0;
	CString strStirrupNum=_T("");

	// Main Rebar.
	if(m_CurData.MainRebarBeam==_T(""))	
		CDBLib::GetDefaultMainRebarSize(1, m_CurData.MainRebarBeam);
	Index = m_ctrMainCombo.FindStringExact(-1, m_CurData.MainRebarBeam);
	m_ctrMainCombo.SetCurSel(Index);
	// Stirrup.
	if(m_CurData.StirrupBarBeam==_T(""))	
		CDBLib::GetDefaultSubRebarSize(1, m_CurData.StirrupBarBeam);
	Index = m_ctrStirCombo.FindStringExact(-1, m_CurData.StirrupBarBeam);
	m_ctrStirCombo.SetCurSel(Index);
	// Stirrup Arrangement.
	if(m_CurData.iSubRebarNum_Beam==0)	
		CDBLib::GetDefaultSubRebarNum(m_CurData.iSubRebarNum_Beam);
	strStirrupNum.Format(_T("%d"), m_CurData.iSubRebarNum_Beam);
	Index = m_ctrArraCombo.FindStringExact(-1, strStirrupNum);
	m_ctrArraCombo.SetCurSel(Index<-1 ? 0 : Index);
	// Side Rebar.
	if(m_CurData.SideBarBeam==_T(""))	
		CDBLib::GetDefaultSideRebarSize(m_CurData.SideBarBeam);
	Index = m_ctrSideCombo.FindStringExact(-1, m_CurData.SideBarBeam);
	m_ctrSideCombo.SetCurSel(Index);
	// Covering.
	m_ctrDT.SetEditUnit(m_CurData.dT_Beam);
	m_ctrDB.SetEditUnit(m_CurData.dB_Beam);

	m_bApplyAIJ = m_CurData.bSpliceBeamJP;
	m_nAIJ = m_CurData.iSpliceBeamSubType;
	m_nOption = m_CurData.iSpliceBeam;

	m_bDoublyRein = m_CurData.bDoublyBeam;
	m_dDoublyRein = m_CurData.dDoublyBeam;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_bConsiderSpacingLimitBeam = m_CurData.bCheckRebarSpacing_Beam;

	m_nDgnClass = m_CurData.iDgnClass;

	UpdateData(FALSE);

	SetAIJControl();

	return TRUE;
}

BOOL CDgnConBeamBarSectDlg::Dlg2Data()
{
	UpdateData(TRUE);

	int Index = 0;
	CString strStirrupNum = _T("");

	// Beam Main Rebar Data.
	Index = m_ctrMainCombo.GetCurSel();
	m_ctrMainCombo.GetLBText(Index, m_CurData.MainRebarBeam);	
	// Beam Stirrup Rebar Data.
	Index = m_ctrStirCombo.GetCurSel();
	m_ctrStirCombo.GetLBText(Index, m_CurData.StirrupBarBeam);
	// Beam Stirrup Rebar Arrangement.
	Index = m_ctrArraCombo.GetCurSel();
	m_ctrArraCombo.GetLBText(Index, strStirrupNum);
	m_CurData.iSubRebarNum_Beam = _ttoi(strStirrupNum);
	// Beam Side Rebar Data.
	Index = m_ctrSideCombo.GetCurSel();
	m_ctrSideCombo.GetLBText(Index, m_CurData.SideBarBeam);
	// Beam Covering.
	m_CurData.dT_Beam = m_ctrDT.GetEditValue();
	m_CurData.dB_Beam = m_ctrDB.GetEditValue();
	
	m_CurData.bSpliceBeamJP = m_bApplyAIJ;
	m_CurData.iSpliceBeamSubType = m_nAIJ;
	m_CurData.iSpliceBeam = m_nOption;

	m_CurData.bDoublyBeam = m_bDoublyRein;
	m_CurData.dDoublyBeam = m_dDoublyRein;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_CurData.bCheckRebarSpacing_Beam = m_bConsiderSpacingLimitBeam;

	m_CurData.iDgnClass = m_nDgnClass;

	return TRUE;
}

void CDgnConBeamBarSectDlg::OnDgnUseAIJCheck()
{
	UpdateData(TRUE);
	if(m_bApplyAIJ) 
	{
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJSIDE_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJU_RADIO)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION0_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION50_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION100_RADIO)->EnableWindow(FALSE);    
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_SPLICE_TITLE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJSIDE_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJU_RADIO)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION0_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION50_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION100_RADIO)->EnableWindow(TRUE);   
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_SPLICE_TITLE)->EnableWindow(TRUE);
	}  
}

void CDgnConBeamBarSectDlg::OnDgnDoublyReinCheck()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_EDIT)->EnableWindow(m_bDoublyRein);    
	GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_STATIC)->EnableWindow(m_bDoublyRein);
	GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_Rhob)->EnableWindow(m_bDoublyRein);
}

void CDgnConBeamBarSectDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	InitDlgData();	
}

void CDgnConBeamBarSectDlg::ApplyData()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_D ElemData; ElemData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(aSelKey[i], ElemData))	ASSERT(0);

			T_MATD_D MatdData; MatdData.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;

			if(MatdData.Type == _T("C"))
			{
				// 1=column, 2=beam, 3=brace, 0=기타
				int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);
				
				if(nMembType == 2)
					rSelKey.Add(aSelKey[i]);
			}
		}
		
		UpdateData(TRUE);
		if(m_nAddReplace==0)	// add/replace
	  {
		  Dlg2Data();
		  
			if(rSelKey.GetSize() > 0)
			{
				// Initialize selected Element.
				if(m_pDoc->m_pDataCtrl->AddDcbb(rSelKey, m_CurData))	
					Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_BEAM),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelDcbb(rSelKey))	
				Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_BEAM),MB_OK);
}

void CDgnConBeamBarSectDlg::SetAIJControl()
{
	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	CString strCode = rData.DesignCode;
	if(strCode!=_T("AIJ-WSD99"))
	{
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJSIDE_RADIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJU_RADIO)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_GROUP)->ShowWindow(SW_HIDE);    
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_CHK)->ShowWindow(SW_HIDE);
		// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
		OnDgnConsiderSpacingLimitCheck();
		GetDlgItem(IDC_DGN_CON_BSPACE_LIMIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJSIDE_RADIO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJU_RADIO)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_CHK)->ShowWindow(SW_SHOW);
		OnDgnUseAIJCheck();
		// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
		GetDlgItem(IDC_DGN_CON_BSPACE_LIMIT)->EnableWindow(FALSE);
	}

	//if(strCode!=_T("TWN-USD92"))
	if(!CDgnCodeCtrl::IsConCodeUSD(strCode))
	{
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_EDIT)->ShowWindow(SW_HIDE);    		
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_STATIC)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_GROUP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_Rhob)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_EDIT)->ShowWindow(SW_SHOW); 
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_STATIC)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_CHK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_Rhob)->ShowWindow(SW_SHOW);
		OnDgnDoublyReinCheck();
	}

	// Change by ZINU.('08.03.21). KCI-USD07 -> KCI-USD07, ACI318-02, ACI318-05.
	if(CDBLib::IsConCodeRhomax075(strCode))
	{
		SetDlgItemText(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_Rhob, _T("k*0.75*Rhob"));
	}
	else
	{
		SetDlgItemText(IDC_DGN_CON_BEAM_BAR_DOUBLY_REIN_Rhob, _T("k*Rhomax"));
	}

	if(strCode==_T("AIJ-WSD99") || CDgnCodeCtrl::IsConCodeUSD(strCode))
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_GROUP)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_DGN_CON_BEAM_BAR_AIJ_GROUP)->ShowWindow(SW_HIDE);
}

void CDgnConBeamBarSectDlg::OnDgnConsiderSpacingLimitCheck()
{
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION0_RADIO)->EnableWindow(m_bConsiderSpacingLimitBeam);
	GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION50_RADIO)->EnableWindow(m_bConsiderSpacingLimitBeam);
	GetDlgItem(IDC_DGN_CON_BEAM_BAR_OPTION100_RADIO)->EnableWindow(m_bConsiderSpacingLimitBeam);
	GetDlgItem(IDC_DGN_CON_BEAM_BAR_SPLICE_TITLE)->EnableWindow(m_bConsiderSpacingLimitBeam);
}

void CDgnConBeamBarSectDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
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
			ControlMan(FALSE);
			SetAIJControl();
			break;
		case D_UPDATE_PREF_CHANGED:
			// do something...      
			ControlMan(FALSE);
			InitData();
			SetAIJControl();
			break;
		default:
			//ASSERT(FALSE);
			break;
	}
}

void CDgnConBeamBarSectDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bMFD   = FALSE;
	BOOL bDCON  = FALSE;
	BOOL bOther = FALSE;
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SECD_ADD):
		case(UR_SECD_DEL):
		case(UR_SECD_MFS):
		case(UR_SECD_MFD):	
		case(UR_DCBB_ADD):
		case(UR_DCBB_DEL):  {bMFD  = TRUE; break;}
		case(UR_DCON_ADD): 
		case(UR_DCON_DEL):  {bDCON = TRUE; break;}
		default:						{bOther=TRUE;	break;}
		}
	}
	
	if(bDCON) InitData();
	
}


void CDgnConBeamBarSectDlg::ControlMan(BOOL bInit)
{
	static CArray<UINT, UINT> arCtrlAType;
	static CArray<UINT, UINT> arCtrlBType;
	static BOOL bFirst = TRUE;

	if(bFirst)
	{
		CDlgUtil::GetCtrlIDByIncRect(this,arCtrlAType,IDC_DGN_A_HOLDER,FALSE);
		CDlgUtil::GetCtrlIDByIncRect(this,arCtrlBType,IDC_DGN_B_HOLDER,FALSE);
		bFirst = FALSE;
	}
		
	if(bInit)
	{
		CWnd* pAHolder = GetDlgItem(IDC_DGN_A_HOLDER);
		CWnd* pBHolder = GetDlgItem(IDC_DGN_B_HOLDER);
		CRect ARect,BRect;
		pAHolder->GetWindowRect(ARect);
		pBHolder->GetWindowRect(BRect);
		
		int DY = ARect.top - BRect.top;
		
		CDlgUtil::CtrlMoveDistY(this,arCtrlBType,DY,TRUE);    
	}

	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	CString strCode = rData.DesignCode;
	if(strCode!=_T("Eurocode2:04") || strCode == CONCODE_SP_63_13330_2018)
	{
		CDlgUtil::CtrlShowHide(this,arCtrlAType,TRUE);
		CDlgUtil::CtrlShowHide(this,arCtrlBType,FALSE);    
	}
	else
	{
		CDlgUtil::CtrlShowHide(this,arCtrlAType,FALSE);
		CDlgUtil::CtrlShowHide(this,arCtrlBType,TRUE);    
	}  
}