// DgnConBraceBarSectDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBraceBarSectDlg.h"


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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBraceBarSectDlg dialog


CDgnConBraceBarSectDlg::CDgnConBraceBarSectDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnConBraceBarSectDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	//m_CurData 를 꼭 초기화한다

	//{{AFX_DATA_INIT(CDgnConBraceBarSectDlg)	
	m_bApplyAIJ = FALSE;
	m_nAddReplace = 0;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_bConsiderSpacingLimitBrace = TRUE;
	m_nDgnClass = 0;
	//}}AFX_DATA_INIT

	//m_pParent = (CDgnConBraceBarSectDlg*)pParent;
}


CDgnConBraceBarSectDlg::~CDgnConBraceBarSectDlg()
{
}


void CDgnConBraceBarSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBraceBarSectDlg)
	DDX_Control(pDX, IDC_DGN_CON_BRACE_BAR_MAIN_COMBO, m_ctrMainCombo);
	DDX_Control(pDX, IDC_DGN_CON_BRACE_BAR_TIES_COMBO, m_ctrTiesCombo);
	DDX_Control(pDX, IDC_DGN_CON_BRACE_BAR_Y_COMBO, m_ctrYCombo);
	DDX_Control(pDX, IDC_DGN_CON_BRACE_BAR_Z_COMBO, m_ctrZCombo);	
	DDX_Control(pDX, IDC_DGN_CON_BRACE_BAR_DO_EDIT, m_ctrDo);
	DDX_Control(pDX, IDC_DGN_CON_BRACE_BAR_DO_UNIT, m_ctrDoUnit);	
	DDX_Check(pDX, IDC_DGN_CON_BRACE_BAR_AIJ_CHK, m_bApplyAIJ);
	DDX_Radio(pDX, IDC_DGN_CON_BRACE_BAR_OPTION0_RADIO, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CON_BRACE_BAR_ADD_RADIO, m_nAddReplace);
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	DDX_Check(pDX, IDC_DGN_CON_RSPACE_LIMIT, m_bConsiderSpacingLimitBrace);
	DDX_Radio(pDX, IDC_DGN_CON_BRACE_BAR_DCH_RADIO, m_nDgnClass);
	//}}AFX_DATA_MAP
}

BOOL CDgnConBraceBarSectDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	SetInitUnit();	
	
	CStringArray arStirrupNum;
	CDBLib::GetSubRebarNumList(arStirrupNum);
	for(int i=0; i<arStirrupNum.GetSize(); i++)
	{
		m_ctrYCombo.AddString(arStirrupNum[i]);	
		m_ctrZCombo.AddString(arStirrupNum[i]);	
	}

	SetRebarCombo();
			
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CDgnConBraceBarSectDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnConBraceBarSectDlg)
	ON_BN_CLICKED(IDC_DGN_CON_BRACE_BAR_AIJ_CHK, OnDgnUseAIJCheck)
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	ON_BN_CLICKED(IDC_DGN_CON_RSPACE_LIMIT,OnDgnConsiderSpacingLimitCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBraceBarSectDlg message handlers


BOOL CDgnConBraceBarSectDlg::DestroyWindow() 
{
	

	return CChildDialog::DestroyWindow();
}

void CDgnConBraceBarSectDlg::SetRebarCombo() 
{
	if(m_ctrMainCombo.GetCount() > 0)	m_ctrMainCombo.ResetContent();
	if(m_ctrTiesCombo.GetCount() > 0)	m_ctrTiesCombo.ResetContent();

	CDBLib::GetRebarNameAtComboBox(&m_ctrMainCombo);
	CDBLib::GetRebarNameAtComboBox(&m_ctrTiesCombo);	

	InitDlgData();
}

void CDgnConBraceBarSectDlg::SetInitUnit() 
{
	m_ctrDo.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrDoUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);		
}

BOOL CDgnConBraceBarSectDlg::Data2Dlg()
{
	m_nAddReplace = 0;

	int Index = 0;
	CString strStirrupNum=_T("");

	// Main Rebar.
	if(m_CurData.MainRebarBrace==_T(""))	
		CDBLib::GetDefaultMainRebarSize(3, m_CurData.MainRebarBrace);
	Index = m_ctrMainCombo.FindStringExact(-1, m_CurData.MainRebarBrace);
	m_ctrMainCombo.SetCurSel(Index);
	// Ties.
	if(m_CurData.StirrupBarBrace==_T(""))	
		CDBLib::GetDefaultSubRebarSize(3, m_CurData.StirrupBarBrace);
	Index = m_ctrTiesCombo.FindStringExact(-1, m_CurData.StirrupBarBrace);
	m_ctrTiesCombo.SetCurSel(Index);
	// Stirrup Arrangement.
	if(m_CurData.iSubRebarNum_Brace[0]==0)	
		CDBLib::GetDefaultSubRebarNum(m_CurData.iSubRebarNum_Brace[0]);
	strStirrupNum.Format(_T("%d"), m_CurData.iSubRebarNum_Brace[0]);
	Index = m_ctrYCombo.FindStringExact(-1, strStirrupNum);
	m_ctrYCombo.SetCurSel(Index);

	if(m_CurData.iSubRebarNum_Brace[1]==0)	
		CDBLib::GetDefaultSubRebarNum(m_CurData.iSubRebarNum_Brace[1]);
	strStirrupNum.Format(_T("%d"), m_CurData.iSubRebarNum_Brace[1]);
	Index = m_ctrZCombo.FindStringExact(-1, strStirrupNum);
	m_ctrZCombo.SetCurSel(Index);
	
	// Covering.
	m_ctrDo.SetEditUnit(m_CurData.d0_Brace);	

	m_bApplyAIJ = m_CurData.bSpliceBraceJP;
	m_nOption = m_CurData.iSpliceBrace;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_bConsiderSpacingLimitBrace = m_CurData.bCheckRebarSpacing_Brace;
	m_nDgnClass = m_CurData.iDgnClass;

	UpdateData(FALSE);

	SetAIJControl();
	SetShowHideControl();

	return TRUE;
}

BOOL CDgnConBraceBarSectDlg::Dlg2Data()
{
	int Index = 0;
	CString strStirrupNum = _T("");

	UpdateData(TRUE);

	// Beam Main Rebar Data.
	Index = m_ctrMainCombo.GetCurSel();
	m_ctrMainCombo.GetLBText(Index, m_CurData.MainRebarBrace);	
	// Beam Stirrup Rebar Data.
	Index = m_ctrTiesCombo.GetCurSel();
	m_ctrTiesCombo.GetLBText(Index, m_CurData.StirrupBarBrace);
	// Beam Stirrup Rebar Arrangement.
	Index = m_ctrYCombo.GetCurSel();
	m_ctrYCombo.GetLBText(Index, strStirrupNum);
	m_CurData.iSubRebarNum_Brace[0] = _ttoi(strStirrupNum);

	Index = m_ctrZCombo.GetCurSel();
	m_ctrZCombo.GetLBText(Index, strStirrupNum);
	m_CurData.iSubRebarNum_Brace[1] = _ttoi(strStirrupNum);
	
	// Beam Covering.
	m_CurData.d0_Brace = m_ctrDo.GetEditValue();	
	
	m_CurData.bSpliceBraceJP = m_bApplyAIJ;
	m_CurData.iSpliceBrace = m_nOption;
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	m_CurData.bCheckRebarSpacing_Brace = m_bConsiderSpacingLimitBrace;
	m_CurData.iDgnClass = m_nDgnClass;

	return TRUE;
}


void CDgnConBraceBarSectDlg::OnDgnUseAIJCheck()
{
	UpdateData(TRUE);
	if(m_bApplyAIJ) 
	{    
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION0_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION50_RADIO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION100_RADIO)->EnableWindow(FALSE);    
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_SPLICE_TITLE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION0_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION50_RADIO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION100_RADIO)->EnableWindow(TRUE);   
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_SPLICE_TITLE)->EnableWindow(TRUE);
	}  
}

void CDgnConBraceBarSectDlg::InitDlgData()
{
	m_CurData.Initialize();

	//080401 sshan DCRB(전체철근정보)데이터로 초기값을 셋팅 (from 이진우)
	T_DCRB_D DataDcrb; DataDcrb.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetDcrb(DataDcrb))
	{
		m_CurData.MainRebarBrace							= DataDcrb.MainRebarBrace[0];
		m_CurData.StirrupBarBrace							= DataDcrb.StirrupBarBrace;
		m_CurData.d0_Brace										= DataDcrb.d0_Brace;
		m_CurData.iSubRebarNum_Brace[0]				= DataDcrb.iSubRebarNum_Brace[0];
		m_CurData.iSubRebarNum_Brace[1]				= DataDcrb.iSubRebarNum_Brace[1];
		m_CurData.bSpliceBraceJP							= DataDcrb.bSpliceBraceJP;
		m_CurData.iSpliceBrace								= DataDcrb.iSpliceBrace;
		m_CurData.bCheckRebarSpacing_Brace		= DataDcrb.bCheckRebarSpacing_Brace;		
	}

	T_DCON_D DataDcon; DataDcon.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetDcon(DataDcon))
		m_CurData.iDgnClass = DataDcon.nClass;

	Data2Dlg();
}

void CDgnConBraceBarSectDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	InitDlgData();
}

void CDgnConBraceBarSectDlg::ApplyData() 
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
				
				if(nMembType == 3)
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
				if(m_pDoc->m_pDataCtrl->AddDcbr(rSelKey, m_CurData))	
					Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_BRACE),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
	  	if(m_pDoc->m_pDataCtrl->DelDcbr(rSelKey))	
				Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_BRACE),MB_OK);
}

void CDgnConBraceBarSectDlg::SetAIJControl()
{
	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	CString strCode = rData.DesignCode;
	if(strCode!=_T("AIJ-WSD99"))
	{
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_AIJ_CHK)->ShowWindow(SW_HIDE);    
		// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
		OnDgnConsiderSpacingLimitCheck();
		GetDlgItem(IDC_DGN_CON_RSPACE_LIMIT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_AIJ_CHK)->ShowWindow(SW_SHOW);    
		OnDgnUseAIJCheck();
		// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
		GetDlgItem(IDC_DGN_CON_RSPACE_LIMIT)->EnableWindow(FALSE);
	}
}

void CDgnConBraceBarSectDlg::OnDgnConsiderSpacingLimitCheck()
{
	// Add by ZINU.('08.03.21). NO:3292, Option to Apply Spacing Limit.
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION0_RADIO)->EnableWindow(m_bConsiderSpacingLimitBrace);
	GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION50_RADIO)->EnableWindow(m_bConsiderSpacingLimitBrace);
	GetDlgItem(IDC_DGN_CON_BRACE_BAR_OPTION100_RADIO)->EnableWindow(m_bConsiderSpacingLimitBrace);
	GetDlgItem(IDC_DGN_CON_BRACE_BAR_SPLICE_TITLE)->EnableWindow(m_bConsiderSpacingLimitBrace);
}

void CDgnConBraceBarSectDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			break;
		case D_UPDATE_PREF_CHANGED:
			// do something...
			SetRebarCombo();			
			break;
		default:
			//ASSERT(FALSE);
			break;
	}
}

void CDgnConBraceBarSectDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bDCON  = FALSE;  
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_DCON_ADD): 
		case(UR_DCON_DEL):  {bDCON = TRUE; break;}    
		}
	}
	
	if(bDCON) SetShowHideControl();
	
}

void CDgnConBraceBarSectDlg::SetShowHideControl()
{
	T_DCON_D DataDcon;
	if(!m_pDoc->m_pAttrCtrl->GetDcon(DataDcon))
		DataDcon.Initialize();

	BOOL bAIJ = FALSE;
	BOOL bEURO_04 = FALSE;

	GetDlgItem(IDC_DGN_CON_BRACE_BAR_AIJ_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BRACE_DUCTY_GROUP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_BRACE_BAR_DCH_RADIO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_BRACE_BAR_DCM_RADIO)->ShowWindow(SW_HIDE);

	if(DataDcon.DesignCode == _T("AIJ-WSD99"))
		bAIJ = TRUE;
	if(DataDcon.DesignCode == _T("Eurocode2:04") || DataDcon.DesignCode == CONCODE_SP_63_13330_2018)
		bEURO_04 = TRUE;

	if(bAIJ)
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_AIJ_CHK)->ShowWindow(SW_SHOW);

	if(bEURO_04)
	{
		GetDlgItem(IDC_DGN_BRACE_DUCTY_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_DCH_RADIO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BRACE_BAR_DCM_RADIO)->ShowWindow(SW_SHOW);
	}    
}




