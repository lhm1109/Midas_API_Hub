// DgnConWallBarSectDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWallBarSectDlg.h"


#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallBarSectDlg dialog


CDgnConWallBarSectDlg::CDgnConWallBarSectDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnConWallBarSectDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	//m_CurData 를 꼭 초기화한다


	//{{AFX_DATA_INIT(CDgnConWallBarSectDlg)	
	m_bApplyAIJ = FALSE;
	m_nAddReplace = 0;
	m_bVerEndWall = FALSE;
	m_nDgnClass = 0;
	//}}AFX_DATA_INIT

	//m_pParent = (CDgnConWallBarSectDlg*)pParent;


}


CDgnConWallBarSectDlg::~CDgnConWallBarSectDlg()
{
	
}


void CDgnConWallBarSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConWallBarSectDlg)
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_VER_COMBO, m_ctrVerCombo);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_HOR_COMBO, m_ctrHorCombo);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_END_COMBO, m_ctrEndCombo);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_BEHOR_COMBO, m_ctrBEHorCombo);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_BEHOR_SPACE_EDT, m_ctrBEHorSpaceEdt);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_BEVER_SPACE_EDT, m_ctrBEVerSpaceEdt);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_DE_EDIT, m_ctrDE);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_DW_EDIT, m_ctrDW);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_BEHOR_SPACE_UNIT, m_ctrBEHorSpaceUnit);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_BEVER_SPACE_UNIT, m_ctrBEVerSpaceUnit);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_DE_UNIT, m_ctrDEUnit);
	DDX_Control(pDX, IDC_DGN_CON_WALL_BAR_DW_UNIT, m_ctrDWUnit);
	DDX_Check(pDX, IDC_DGN_CON_WALL_BAR_AIJ_CHK, m_bApplyAIJ);
	DDX_Radio(pDX, IDC_DGN_CON_WALL_BAR_ADD_RADIO, m_nAddReplace);
	// Add by sshan (090222) NO:3909
	DDX_Check(pDX, IDC_DGN_CON_VER_END_CHECK, m_bVerEndWall);
	DDX_Control(pDX, IDC_DGN_RC_VER_END_WALL, m_ComboVerEndWall);
	DDX_Control(pDX, IDC_DGN_RC_VER_END_FY_WALL, m_ComboVerEndFyWall);
	DDX_Radio(pDX, IDC_DGN_CON_WALL_BAR_DCH_RADIO, m_nDgnClass);
	//}}AFX_DATA_MAP
}

BOOL CDgnConWallBarSectDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	if(Is_KCIUSD07_KS_KS01RC())
	{
		GetDlgItem(IDC_DGN_CON_VER_END_CHECK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_VER_END_STA1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_VER_END_WALL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_VER_END_STA2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_RC_VER_END_FY_WALL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_VER_END_STA3)->ShowWindow(SW_SHOW);		
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_VER_END_CHECK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_VER_END_STA1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_VER_END_WALL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_VER_END_STA2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RC_VER_END_FY_WALL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_VER_END_STA3)->ShowWindow(SW_HIDE);
	}

	SetControlEnable();
	if(!m_pDoc->m_pPostCtrl->IsPostEnable())
	{
		SetShowHideControl();
		return TRUE;
	}

	AlignCtls();
	SetInitUnit();
	
	SetRebarCombo();

	if(Is_KCIUSD07_KS_KS01RC())	OnDgnRcVerEndWall();  

	UpdateData(FALSE);  

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CDgnConWallBarSectDlg, CChildDialog)
	//{{AFX_MSG_MAP(CDgnConWallBarSectDlg)
	ON_BN_CLICKED(IDC_DGN_CON_VER_END_CHECK, OnDgnRcVerEndWall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionCreateShearRebar_I message handlers


BOOL CDgnConWallBarSectDlg::DestroyWindow() 
{
	

	return CChildDialog::DestroyWindow();
}

void CDgnConWallBarSectDlg::SetRebarCombo() 
{
	if(m_ctrVerCombo.GetCount() > 0)	  m_ctrVerCombo.ResetContent();
	if(m_ctrHorCombo.GetCount() > 0)	  m_ctrHorCombo.ResetContent();
	if(m_ctrEndCombo.GetCount() > 0)	  m_ctrEndCombo.ResetContent();
	if(m_ctrBEHorCombo.GetCount() > 0)	m_ctrBEHorCombo.ResetContent();

	CDBLib::GetRebarNameAtComboBox(&m_ctrVerCombo);
	CDBLib::GetRebarNameAtComboBox(&m_ctrHorCombo);
	CDBLib::GetRebarNameAtComboBox(&m_ctrEndCombo);
	CDBLib::GetRebarNameAtComboBox(&m_ctrBEHorCombo);

	CDBLib::GetRebarNameAtComboBox(&m_ComboVerEndWall);
	CArray<CString, CString&> arRebar;
	CString strRebarMatl = MATLCODE_CON_KS19;
	m_pDoc->m_pMatlDB->GetRebarNameList(strRebarMatl, arRebar);
	for(int i=0; i<arRebar.GetSize(); i++)  
	{
		m_ComboVerEndFyWall.AddString(arRebar[i]);    
	}

	InitDlgData();
}

void CDgnConWallBarSectDlg::SetInitUnit() 
{
	m_ctrBEHorSpaceEdt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrBEVerSpaceEdt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrDE.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrDW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrBEHorSpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrBEVerSpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ctrDEUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_ctrDWUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}

BOOL CDgnConWallBarSectDlg::Data2Dlg()
{
	m_nAddReplace = 0;

	int Index = 0;
	CString strStirrupNum=_T("");

	// Vertical Rebar.
	if(m_CurData.VerticalRebarWall==_T(""))	
		CDBLib::GetDefaultMainRebarSize(4, m_CurData.VerticalRebarWall);
	Index = m_ctrVerCombo.FindStringExact(-1, m_CurData.VerticalRebarWall);
	m_ctrVerCombo.SetCurSel(Index);
	// Horizontal Rebar.
	if(m_CurData.HorizontalRebarWall==_T(""))	
		CDBLib::GetDefaultSubRebarSize(4, m_CurData.HorizontalRebarWall);
	Index = m_ctrHorCombo.FindStringExact(-1, m_CurData.HorizontalRebarWall);
	m_ctrHorCombo.SetCurSel(Index);
	// End Rebar	
	if(m_CurData.EndRebarWall==_T(""))	
		CDBLib::GetDefaultSubRebarSize(4, m_CurData.EndRebarWall);
	Index = m_ctrEndCombo.FindStringExact(-1, m_CurData.EndRebarWall);
	m_ctrEndCombo.SetCurSel(Index);
	// Boundary Element Horizontal Rebar.
	if(m_CurData.BEHorizontalRebarWall==_T(""))	
		CDBLib::GetDefaultSubRebarSize(4, m_CurData.BEHorizontalRebarWall);
	Index = m_ctrBEHorCombo.FindStringExact(-1, m_CurData.BEHorizontalRebarWall);
	m_ctrBEHorCombo.SetCurSel(Index);

	// Boundary Element Horizontal Rebar Space.
	m_ctrBEHorSpaceEdt.SetEditUnit(m_CurData.dBEHorizontalRebarSpace);
	m_ctrBEVerSpaceEdt.SetEditUnit(m_CurData.dBEVerticalRebarSpace);

	// Covering.
	m_ctrDE.SetEditUnit(m_CurData.dDe_Wall);
	m_ctrDW.SetEditUnit(m_CurData.dDw_Wall);

	// if vertical and end Rebar >= ...
	m_bVerEndWall = m_CurData.bVerEndBar_Wall;

	if(m_CurData.strVerEndBar_Wall==_T(""))	
		CDBLib::GetDefaultVerEndWallRebarSize(m_CurData.strVerEndBar_Wall);
	Index = m_ComboVerEndWall.FindStringExact(-1, m_CurData.strVerEndBar_Wall);
	m_ComboVerEndWall.SetCurSel(Index);
	if(m_CurData.strVerEndBarFy_Wall==_T(""))	
		m_CurData.strVerEndBarFy_Wall = _T("SD500");
	Index = m_ComboVerEndFyWall.FindStringExact(-1, m_CurData.strVerEndBarFy_Wall);
	m_ComboVerEndFyWall.SetCurSel(Index);

	m_bApplyAIJ = m_CurData.bSpliceWallJP;	
	m_nDgnClass = m_CurData.iDgnClass;
	
	UpdateData(FALSE);

	SetAIJControl();

	SetShowHideControl();

	SetBEControlByRCSpecialWall();
	
	return TRUE;
}

BOOL CDgnConWallBarSectDlg::Dlg2Data()
{
	int Index = 0;
	CString strStirrupNum = _T("");
	
	UpdateData(TRUE);

	// Vertical Rebar.
	Index = m_ctrVerCombo.GetCurSel();
	m_ctrVerCombo.GetLBText(Index, m_CurData.VerticalRebarWall);	
	// Horizontal Rebar.
	Index = m_ctrHorCombo.GetCurSel();
	m_ctrHorCombo.GetLBText(Index, m_CurData.HorizontalRebarWall);	
	// End Rebar
	Index = m_ctrEndCombo.GetCurSel();
	m_ctrEndCombo.GetLBText(Index, m_CurData.EndRebarWall);
	// Boundary Element Horizontal Rebar.
	Index = m_ctrBEHorCombo.GetCurSel();
	m_ctrBEHorCombo.GetLBText(Index, m_CurData.BEHorizontalRebarWall);
	// Boundary Element Horizontal Rebar Space.
	m_CurData.dBEHorizontalRebarSpace = m_ctrBEHorSpaceEdt.GetEditValue();
	m_CurData.dBEVerticalRebarSpace = m_ctrBEVerSpaceEdt.GetEditValue();
	// Wall Covering.
	m_CurData.dDe_Wall = m_ctrDE.GetEditValue();
	m_CurData.dDw_Wall = m_ctrDW.GetEditValue();
	
	m_CurData.bSpliceWallJP = m_bApplyAIJ;	

	m_CurData.bVerEndBar_Wall = m_bVerEndWall;
	Index = m_ComboVerEndWall.GetCurSel();
	m_ComboVerEndWall.GetLBText(Index, m_CurData.strVerEndBar_Wall);	
	Index = m_ComboVerEndFyWall.GetCurSel();
	if (Index >= 0)
	{
		m_ComboVerEndFyWall.GetLBText(Index, m_CurData.strVerEndBarFy_Wall);
	}
	else
	{
		m_CurData.strVerEndBarFy_Wall = _T("SD500");
	}
	m_CurData.iDgnClass = m_nDgnClass;
	
	return TRUE;
}

void CDgnConWallBarSectDlg::InitDlgData()
{
	m_CurData.Initialize();

	//080401 sshan DCRB(전체철근정보)데이터로 초기값을 셋팅 (from 이진우)
	T_DCRB_D DataDcrb; DataDcrb.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetDcrb(DataDcrb))
	{
		// MNET:4076-SHJUNG-20100310 - BEHorizontalRebarWall 데이타 세팅
		if(DataDcrb.BEHorizontalRebarWall==_T(""))	CDBLib::GetDefaultSubRebarSize(4, DataDcrb.BEHorizontalRebarWall);
		
		m_CurData.VerticalRebarWall			= DataDcrb.VerticalRebarWall[0];
		m_CurData.HorizontalRebarWall		= DataDcrb.HorizontalRebarWall;
		m_CurData.EndRebarWall					= DataDcrb.EndRebarWall;		

		m_CurData.BEHorizontalRebarWall = DataDcrb.BEHorizontalRebarWall;
		m_CurData.dBEHorizontalRebarSpace = DataDcrb.dBEHorizontalRebarSpace;

		m_CurData.dBEVerticalRebarSpace = DataDcrb.dBEVerticalRebarSpace;

		m_CurData.dDe_Wall							= DataDcrb.dDe_Wall;
		m_CurData.dDw_Wall							= DataDcrb.dDw_Wall;		

		m_CurData.bVerEndBar_Wall				= DataDcrb.bVerEndBar_Wall;		
		m_CurData.strVerEndBar_Wall			= DataDcrb.strVerEndBar_Wall;		
		m_CurData.strVerEndBarFy_Wall		= DataDcrb.strVerEndBarFy_Wall;		
	}

	if(m_CurData.dBEHorizontalRebarSpace<=0.)
	{
		//Boundary Element Horizontal Rebar Space
		double dSpace = 0.2;
		T_UNIT_INDEX UnitIndex;
		CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
		if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
			UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
		{
			dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
		}
		else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
		{
			dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
		}
		else ASSERT(0);
		m_CurData.dBEHorizontalRebarSpace = dSpace;
	}

	if (m_CurData.dBEVerticalRebarSpace <= 0.)
	{
		//Boundary Element Vertical Rebar Space
		double dSpace = 0.1;
		T_UNIT_INDEX UnitIndex;
		CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
		if (UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
			UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
		{
			dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
		}
		else if (UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
		{
			dSpace = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dSpace);
		}
		else ASSERT(0);
		m_CurData.dBEVerticalRebarSpace = dSpace;
	}

	T_DCON_D DataDcon; DataDcon.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetDcon(DataDcon))
		m_CurData.iDgnClass = DataDcon.nClass;

	Data2Dlg();	
}

void CDgnConWallBarSectDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	
	SetAIJControl();        
	SetShowHideControl();        
	SetBEControlByRCSpecialWall();

	// Initialize Data.
	//InitDlgData();
}

void CDgnConWallBarSectDlg::ApplyData() 
{
	if(!m_pDoc->m_pPostCtrl->IsPostEnable()) return;
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_DCBW_K,T_DCBW_K> rSelKey;
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
				// MQC:9308-JWKWON-20141216 : Design Criteria for Rebar by Member 에서 Wall 선택 안되는 오류 수정(m_pAttrCtrl->GetMemberType 함수에서 835까지는 0을 return 했는제 Renewal로 되면서 4를 return 하고 있음)
				// 1=column, 2=beam, 3=brace, 4=wall
				int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(aSelKey[i]);
				
				if(nMembType == D_MBTP_WALL)
				{
					T_DCBW_K dcbwKey;

					unsigned int nInternalWallId = 0;
					m_pDoc->m_pPostCtrl->GetInternalWallId(aSelKey[i], nInternalWallId);
					T_WALL_K iWallKey;
					m_pDoc->m_pPostCtrl->GetWallKey(nInternalWallId, iWallKey.keymap);
					dcbwKey.key.wallid = iWallKey.key.wallid;
					dcbwKey.key.storid  = iWallKey.key.storid;					
					
					rSelKey.Add(dcbwKey);
				}
			}
		}
		
		UpdateData(TRUE);
		if(m_nAddReplace==0)	// add/replace
	  {
		  Dlg2Data();
		  
			if(rSelKey.GetSize() > 0)
			{
				/*
				for(i=0; i<rSelKey.GetSize(); i++)
				{
					if(m_pDoc->m_pAttrCtrl->ExistDcbw(rSelKey[i]))
					{
						m_pDoc->m_pDataCtrl->DelDcbw(rSelKey[i]);
					}
					// Initialize selected Element.
					if(!m_pDoc->m_pDataCtrl->AddDcbw(rSelKey[i], m_CurData)) {ASSERT(0);}            
				}
				*/
				if(!m_pDoc->m_pDataCtrl->AddDcbw(rSelKey, m_CurData)) {ASSERT(0);} 
				Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL),MB_OK);
		}
	  else	// Delete
		{
			// Initialize selected Element.
			if(!m_pDoc->m_pDataCtrl->DelDcbw(rSelKey)) {ASSERT(0);}
			Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_CON_ELEM_WALL),MB_OK);
}

void CDgnConWallBarSectDlg::SetAIJControl()
{
	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	CString strCode = rData.DesignCode;

	//080402 sshan 기준과 상관없이 AIJ Recommendation Option 사용하지 않음 (from 승준)
	if(strCode!=_T("AIJ-WSD99"))
	{
		GetDlgItem(IDC_DGN_CON_WALL_BAR_AIJ_CHK)->ShowWindow(SW_HIDE);
	}
	else
		GetDlgItem(IDC_DGN_CON_WALL_BAR_AIJ_CHK)->ShowWindow(SW_HIDE);
}

void CDgnConWallBarSectDlg::SetControlEnable()
{  
	BOOL bPostEnable = m_pDoc->m_pPostCtrl->IsPostEnable();
	GetDlgItem(IDC_DGN_CON_WALL_GROUP)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_ADD_RADIO)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DEL_RADIO)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_WALL_BE_GROUP)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC2)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC3)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC4)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC13)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_VER_COMBO)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_HOR_COMBO)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_END_COMBO)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEHOR_COMBO)->EnableWindow(bPostEnable);

	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC8)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEHOR_SPACE_EDT)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEHOR_SPACE_UNIT)->EnableWindow(bPostEnable);

	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEVER_SPACE_EDT)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEVER_SPACE_UNIT)->EnableWindow(bPostEnable);

	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC9)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC10)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DE_EDIT)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DW_EDIT)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DE_UNIT)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DW_UNIT)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_AIJ_CHK)->EnableWindow(bPostEnable);  
	GetDlgItem(IDC_DGN_CON_VER_END_CHECK)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_VER_END_STA1)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_VER_END_STA3)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_RC_VER_END_WALL)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_VER_END_STA2)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_RC_VER_END_FY_WALL)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_WALL_DUCTY_GROUP)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DCH_RADIO)->EnableWindow(bPostEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DCM_RADIO)->EnableWindow(bPostEnable);
}

BOOL CDgnConWallBarSectDlg::Is_KCIUSD07_KS_KS01RC()
{
	return FALSE;

	bool bKCI_USD07 = false;	
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

    auto L_IsCodeKCI07Series = [] (const CString& strCode) -> bool
    {
        if ( strCode == CONCODE_KCI_USD07 ) return true;
        if ( strCode == CONCODE_KCI_USD12 ) return true;
        if ( strCode == CONCODE_KDS_41_30_2018 ) return true;
        if ( strCode == CONCODE_KDS_41_20_2022 ) return true;

        return false;
    };

	if(m_pDoc->m_pAttrCtrl->ExistDcon())
	{
		T_DCON_D rData;
		rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetDcon(rData);

        bKCI_USD07 = L_IsCodeKCI07Series(rData.DesignCode);		
	}
	else
	{		
		rPref.DgnCode.ConcCode = CDBLib::ViewConvertCodeName(rPref.DgnCode.ConcCode);

        bKCI_USD07 = L_IsCodeKCI07Series(rPref.DgnCode.ConcCode);
	}

    const bool bKS = [] (const CString& strMatlCode) -> bool
    {
        if ( strMatlCode == MATLCODE_CON_KS01_CIVIL ) return true;
        if ( strMatlCode == MATLCODE_CON_KS_CIVIL ) return true;
        if ( strMatlCode == MATLCODE_CON_KS19 ) return true;
        if ( strMatlCode == MATLCODE_CON_KS01 ) return true;
        if ( strMatlCode == MATLCODE_CON_KS ) return true;
        return false;
    }(rPref.DgnCode.ConcRebarMatlCode);

    auto l_Is_KS01_Series = [] (const CString& strMatlCode)
    {
        if ( strMatlCode == MATLCODE_CON_KS01 ) return true;
        if ( strMatlCode == MATLCODE_CON_KS19 ) return true;
        return false;
    };

    bool bKS01_RC = false;
	T_MATD_D MatlD;
	CArray<T_MATL_K, T_MATL_K> arMatlK; arMatlK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(arMatlK);
	int nSize = arMatlK.GetSize();
	for(int i=0 ; i<nSize ; ++i)
	{
		T_MATL_K MatlK = arMatlK[i];
		MatlD.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(MatlK, MatlD);		
        if ( l_Is_KS01_Series(MatlD.Rebar_CodeName) )
		{
			bKS01_RC = true;
			break;
		}
		else if(MatlD.Rebar_CodeName == _T(""))
		{
            if ( l_Is_KS01_Series(rPref.DgnCode.ConcRebarMatlCode) )
            {
                bKS01_RC = true;
                break;
			}
		}
	}

	if(bKCI_USD07 && bKS && bKS01_RC)
		return TRUE;
	else
		return FALSE;	
}

void CDgnConWallBarSectDlg::OnDgnRcVerEndWall()
{
	UpdateData(TRUE);
	if(m_bVerEndWall == FALSE) 
	{
		GetDlgItem(IDC_DGN_CON_VER_END_STA1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RC_VER_END_WALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CON_VER_END_STA2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RC_VER_END_FY_WALL)->EnableWindow(FALSE);    
		GetDlgItem(IDC_DGN_CON_VER_END_STA3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CON_VER_END_STA1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RC_VER_END_WALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CON_VER_END_STA2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RC_VER_END_FY_WALL)->EnableWindow(TRUE);    
		GetDlgItem(IDC_DGN_CON_VER_END_STA3)->EnableWindow(TRUE);
	}
}

void CDgnConWallBarSectDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			AlignCtls();
			break;
		case D_UPDATE_PREF_CHANGED:
			// do something...
			SetRebarCombo();	
			AlignCtls();
			break;
		default:
			//ASSERT(FALSE);
			break;
	}
}

void CDgnConWallBarSectDlg::UpdateBuffer()
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
	
	if(bDCON) 
	{
		OnInitDialog();
		SetShowHideControl();
		SetBEControlByRCSpecialWall();
	}
		
	SetControlEnable();
}

void CDgnConWallBarSectDlg::SetBEControlByRCSpecialWall()
{
	if(!m_pDoc->m_pPostCtrl->IsPostEnable()) return;
	
	T_DCON_D DataDcon;
	if(!m_pDoc->m_pAttrCtrl->GetDcon(DataDcon))
		DataDcon.Initialize();  

	BOOL bEnable = m_pDoc->m_pAttrCtrl->IsWallEndHorBarCondition();
	GetDlgItem(IDC_DGN_WALL_BE_GROUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC4)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC13)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEHOR_COMBO)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC8)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEHOR_SPACE_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEHOR_SPACE_UNIT)->EnableWindow(bEnable);

	bEnable = m_pDoc->m_pAttrCtrl->IsWallEndVerBarCondition();

	GetDlgItem(IDC_DGN_CON_WALL_BAR_STATIC13)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEVER_SPACE_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_BEVER_SPACE_UNIT)->EnableWindow(bEnable);
}

void CDgnConWallBarSectDlg::SetShowHideControl()
{
	T_DCON_D DataDcon;
	if(!m_pDoc->m_pAttrCtrl->GetDcon(DataDcon))
		DataDcon.Initialize();

	BOOL bAIJ = FALSE;
	BOOL bEURO_04 = FALSE;

	GetDlgItem(IDC_DGN_CON_WALL_BAR_AIJ_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_WALL_DUCTY_GROUP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DCH_RADIO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_DCM_RADIO)->ShowWindow(SW_HIDE);

	if(DataDcon.DesignCode == _T("AIJ-WSD99"))
		bAIJ = TRUE;
	if(DataDcon.DesignCode == _T("Eurocode2:04") || DataDcon.DesignCode == CONCODE_SP_63_13330_2018)
		bEURO_04 = TRUE;

	if(bAIJ)
		GetDlgItem(IDC_DGN_CON_WALL_BAR_AIJ_CHK)->ShowWindow(SW_SHOW);

	if(bEURO_04)
	{
		GetDlgItem(IDC_DGN_WALL_DUCTY_GROUP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_WALL_BAR_DCH_RADIO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_WALL_BAR_DCM_RADIO)->ShowWindow(SW_SHOW);
	}
}

void CDgnConWallBarSectDlg::AlignCtls()
{
	T_DCON_D DataDcon;
	if(!m_pDoc->m_pAttrCtrl->GetDcon(DataDcon)) DataDcon.Initialize();
	BOOL bEURO_04 = FALSE;
	if(DataDcon.DesignCode == _T("Eurocode2:04") || DataDcon.DesignCode == CONCODE_SP_63_13330_2018)
		bEURO_04 = TRUE;
	
	CRect refRect, moveRect;
	GetDlgItem(IDC_DGN_CON_VER_END_STA1)->GetWindowRect(refRect);
	GetDlgItem(IDC_DGN_WALL_DUCTY_GROUP)->GetWindowRect(moveRect);
	
	CArray<UINT, UINT> arMoveCtrl; arMoveCtrl.RemoveAll();
	arMoveCtrl.Add(IDC_DGN_WALL_DUCTY_GROUP);
	arMoveCtrl.Add(IDC_DGN_CON_WALL_BAR_DCH_RADIO);
	arMoveCtrl.Add(IDC_DGN_CON_WALL_BAR_DCM_RADIO);
	
	int nDistY = (bEURO_04) ? refRect.top - moveRect.top : refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(4);
	CDlgUtil::CtrlMoveDistY(this, arMoveCtrl, nDistY);

	BOOL bAIJ = FALSE;
	if (DataDcon.DesignCode == CONCODE_AIJ_WSD99)
		bAIJ = TRUE;
	GetDlgItem(IDC_DGN_CON_VER_END_STA1)->GetWindowRect(refRect);
	GetDlgItem(IDC_DGN_CON_WALL_BAR_AIJ_CHK)->GetWindowRect(moveRect);

	arMoveCtrl.RemoveAll();
	arMoveCtrl.Add(IDC_DGN_CON_WALL_BAR_AIJ_CHK);

	nDistY = (bAIJ) ? refRect.top - moveRect.top : refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(4);
	CDlgUtil::CtrlMoveDistY(this, arMoveCtrl, nDistY);

	RedrawWindow();
}