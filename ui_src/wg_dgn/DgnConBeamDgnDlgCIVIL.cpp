// DgnConBeamDgnDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamDgnDlgCIVIL.h"
#include "DgnDataCtrl.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDgnDlgCIVIL dialog


CDgnConBeamDgnDlgCIVIL::CDgnConBeamDgnDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConBeamDgnDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamDgnDlgCIVIL)
	m_AddorDel = -1;
	//}}AFX_DATA_INIT

	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
}


void CDgnConBeamDgnDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamDgnDlgCIVIL)
	DDX_Control(pDX, IDC_STIRRUP_NUM_CB, m_SubNum);
	DDX_Control(pDX, IDC_STIRRUP_CB, m_SubCB);
	DDX_Control(pDX, IDC_dT_UNIT, m_dTunit);
	DDX_Control(pDX, IDC_dT_EDIT, m_dT);
	DDX_Control(pDX, IDC_DGN_RC_BHFUNIT, m_hfunit);
	DDX_Control(pDX, IDC_DGN_RC_BHF, m_hf);
	DDX_Control(pDX, IDC_DGN_RC_BHCUNIT, m_Hcunit);
	DDX_Control(pDX, IDC_DGN_RC_BHC, m_Hc);
	DDX_Control(pDX, IDC_DGN_RC_BBFUNIT, m_bfunit);
	DDX_Control(pDX, IDC_DGN_RC_BBF, m_bf);
	DDX_Control(pDX, IDC_DGN_RC_BBCUNIT, m_Bcunit);
	DDX_Control(pDX, IDC_DGN_RC_BBC, m_Bc);
	DDX_Control(pDX, IDC_dB_UNIT, m_dBunit);
	DDX_Control(pDX, IDC_dB_EDIT, m_dB);
	DDX_Radio(pDX, IDC_DGN_BEAM_AR, m_AddorDel);

	DDX_Control(pDX, IDC_DGN_CONC_SELECTGRID, m_SelectGrid.m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBeamDgnDlgCIVIL, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConBeamDgnDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_SELECTALL, OnDgnBeamSelectAll)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_UNSELECTALL, OnDgnBeamUnSelectAll)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CLIK, OnChangeFocusSectionGrid)
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CHEK, OnSelectCheckSectionGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDgnDlgCIVIL message handlers

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Initialize 관련 
//
void CDgnConBeamDgnDlgCIVIL::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	m_SelectGrid.ResetSectGridData(bKeepChk);
	ChangeSectionInfo();
}

void CDgnConBeamDgnDlgCIVIL::SetInitStirCB()
{
	if(m_SubCB.GetCount()>0) m_SubCB.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SubCB);

}

void CDgnConBeamDgnDlgCIVIL::SetInitUnit()
{
	m_dT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_hf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dB.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dTunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_hfunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hcunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_bfunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bcunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dBunit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_SubNum.SetUnitType(D_UNITSYS_NONE);

}

void CDgnConBeamDgnDlgCIVIL::SetData2Dlg()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	ResetSectGridData();

	m_AddorDel = 0;
		
	m_hf.SetEditUnit(0.);
	m_Hc.SetEditUnit(0.);
	m_bf.SetEditUnit(0.);
	m_Bc.SetEditUnit(0.);

	m_hf.EnableWindow(FALSE);
	m_Hc.EnableWindow(FALSE);
	m_bf.EnableWindow(FALSE);
	m_Bc.EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_RC_BSHAPE)->EnableWindow(FALSE);
		
	ShowRECTBitmap();
	ShowHideSectCtrl(FALSE);

	m_SubCB.SetCurSel(-1);
	m_SubNum.SetEditUnit(0.);
	m_dB.SetEditUnit(0);
	m_dT.SetEditUnit(0);

	UpdateData(FALSE);
}

BOOL CDgnConBeamDgnDlgCIVIL::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitSectGrid();
	SetInitStirCB();
	SetInitUnit();
	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnConBeamDgnDlgCIVIL::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
	
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//

BOOL CDgnConBeamDgnDlgCIVIL::CheckInputData()
{
	int nIndex;

	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(m_RdgnKey, sData);
	
	if(!bGetOK){
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Property_Num_Err));
//    AfxMessageBox(_T("[오류] Property Number 선택이 적당하지 않습니다."));
		return FALSE;
	}

	nIndex = m_SubCB.GetCurSel();
	if(nIndex < 0)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Size_Err));
		return FALSE;
	}

	double dH = m_Hc.GetEditValue()/2.0;
	if(m_dT.GetEditValue()<=0 || m_dT.GetEditValue()>=dH)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Dt_Input_Err));
		//AfxMessageBox(_T("[오류] dT 입력이 잘못되었습니다."));
		return FALSE;
	}

	if(m_dB.GetEditValue()<=0 || m_dB.GetEditValue()>=dH)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Db_Input_Err));
		//AfxMessageBox(_T("[오류] dB 입력이 잘못되었습니다."));
		return FALSE;
	}

	if(m_SubNum.GetEditValue()<=0)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Stirrup_Bar_Num_Err));
//    AfxMessageBox(_T("[오류] Stirrup(Spacing) 입력이 적당하지 않습니다."));
		return FALSE;
	}

	return TRUE;
}

void CDgnConBeamDgnDlgCIVIL::SetDlg2Data()
{
	CString str;
	m_RdgnData.nType = 0;

	m_RdgnData.BEAM.dDt[0] = m_dT.GetEditValue();
	m_RdgnData.BEAM.dDb[0] = m_dB.GetEditValue();  
	
/*
	int nIndex = m_SubNum.GetCurSel();
	if(nIndex>=0) m_SubNum.GetLBText(nIndex, m_RdgnData.BEAM.strSubBarNum);
*/
	m_RdgnData.BEAM.dSubBarNum[0] = m_SubNum.GetEditValue();

	int nIndex = m_SubCB.GetCurSel();
	if(nIndex>=0) m_SubCB.GetLBText(nIndex, m_RdgnData.BEAM.strSubBarNa[0]);

	CArray<T_RDGN_K, T_RDGN_K> arRdgnKey;
	arRdgnKey.RemoveAll();

	int Index = 0;
	T_RDGN_K RdgnKey;
	POSITION Pos = m_SelectGrid.m_arKey.GetStartPosition();
	while(Pos)
	{
		RdgnKey=0;
		m_SelectGrid.m_arKey.GetNextAssoc(Pos, Index, RdgnKey);
		arRdgnKey.Add(RdgnKey);
	}

	if(arRdgnKey.GetSize() > 0)
		m_pDoc->m_pDataCtrl->AddRdgn(arRdgnKey, m_RdgnData);
}

void CDgnConBeamDgnDlgCIVIL::OnDgnExecute() 
{
	UpdateData(TRUE);

	if(!m_SelectGrid.IsExistEnabledSect())
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_SECT));
		return;
	}

	if(m_AddorDel == 0)   // Add/Replace
	{
		if(CheckInputData() == FALSE) return;
		SetDlg2Data();	   // Del
		SetData2Dlg();
	}
	else
	{
		BOOL bCheck = TRUE;
		CArray<T_RDGN_K, T_RDGN_K> arRdgnKey;
		arRdgnKey.RemoveAll();

		T_RDGN_K RdgnKey;
		int Index = 0;
	  POSITION PosSect = m_SelectGrid.m_arKey.GetStartPosition();
	  while(PosSect)
	  {
		  RdgnKey=0;
		  m_SelectGrid.m_arKey.GetNextAssoc(PosSect, Index, RdgnKey);
			arRdgnKey.Add(RdgnKey);
		}
		if(arRdgnKey.GetSize() > 0)
		{
			if(!m_pDoc->m_pDataCtrl->DelRdgn(arRdgnKey)) bCheck = FALSE;
		}
		if(bCheck)  SetData2Dlg();
	}		
	
}

void CDgnConBeamDgnDlgCIVIL::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
	
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Change PropNo 관련 
//

void CDgnConBeamDgnDlgCIVIL::ShowRECTBitmap()
{
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_HIDE);
}

void CDgnConBeamDgnDlgCIVIL::ShowTEEBitmap()
{
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_SHOW);
}

//---------------------------------------------------------------------------
// ShowHideSectCtrl 
//---------------------------------------------------------------------------
void CDgnConBeamDgnDlgCIVIL::ShowHideSectCtrl(BOOL bShow)
{
	int i;
	CWnd* pWnd[6];
	pWnd[0] = GetDlgItem(IDC_DGN_BF_STATIC);  ASSERT(pWnd[0]);
	pWnd[1] = GetDlgItem(IDC_DGN_RC_BBF);     ASSERT(pWnd[1]);
	pWnd[2] = GetDlgItem(IDC_DGN_RC_BBFUNIT); ASSERT(pWnd[2]);
	pWnd[3] = GetDlgItem(IDC_DGN_HF_STATIC);  ASSERT(pWnd[3]);
	pWnd[4] = GetDlgItem(IDC_DGN_RC_BHF);     ASSERT(pWnd[4]);
	pWnd[5] = GetDlgItem(IDC_DGN_RC_BHFUNIT); ASSERT(pWnd[5]);

	if(bShow) for(i=0; i<6; i++) pWnd[i]->EnableWindow(TRUE);
	else      for(i=0; i<6; i++) pWnd[i]->EnableWindow(FALSE);
}

//---------------------------------------------------------------------------
// SelectElemOnView 
//---------------------------------------------------------------------------
void CDgnConBeamDgnDlgCIVIL::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	arElemKeyList.RemoveAll();

	T_ELEM_K ElemKey;
	T_ELEM_D ElemData;
	CDgnDataCtrl DataCtrl;

	T_RDGN_K RdgnKey;
	int Index = 0;
	POSITION PosSect = m_SelectGrid.m_arKey.GetStartPosition();
	while(PosSect)
	{
		RdgnKey=0;
		m_SelectGrid.m_arKey.GetNextAssoc(PosSect, Index, RdgnKey);

		POSITION PosElem = m_pDoc->m_pAttrCtrl->GetStartElem();
		while(PosElem)
		{
			ElemData.Initialize();
			m_pDoc->m_pAttrCtrl->GetNextElem(PosElem, ElemKey, ElemData);
		
			if(ElemData.elpro == RdgnKey) 
				if(ElemData.eltyp == TRUSS_EL || ElemData.eltyp == BEAM_EL)
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_BEAM)  arElemKeyList.Add(ElemKey);
		}
	}

	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

void CDgnConBeamDgnDlgCIVIL::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_SelectGrid.GetCurKeyData(m_RdgnKey, SectData))  return;

	// Set TEE Section Data
	if(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_T)
	{
		ShowTEEBitmap();
		ShowHideSectCtrl(TRUE);

		GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_TEE));  
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_bf.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[2]);   //  Tw
		m_hf.SetEditUnit(SectData.SectBefore.SectI.Size[3]);   //  Tf1
	}
	// SEt RECT Section Data
	else if(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SB)
	{
		ShowRECTBitmap();
		ShowHideSectCtrl(FALSE);

		GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_RECT));
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
	}

	m_RdgnData.Initialize();
	m_pDoc->m_pAttrCtrl->GetRdgn(m_RdgnKey, m_RdgnData);

	// Init Data
	m_dT.SetEditUnit(m_RdgnData.BEAM.dDt[0]);
	m_dB.SetEditUnit(m_RdgnData.BEAM.dDb[0]);
//	m_SubNum.SelectString(-1, m_RdgnData.BEAM.strSubBarNum);
	m_SubNum.SetEditUnit(m_RdgnData.BEAM.dSubBarNum[0]);
//  m_SubNum.SetEditUnit(m_RdgnData.BEAM.iSubBarNum);

	if(m_SubCB.SelectString(-1, m_RdgnData.BEAM.strSubBarNa[0])==LB_ERR) 
		m_SubCB.SetCurSel(-1);;
}

void CDgnConBeamDgnDlgCIVIL::InitSectGrid()
{

	// Data Sort & Store
	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	CMap<int,int,int,int> ColmSecMap; ColmSecMap.RemoveAll();

	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);
	int nCount = arElemKeyList.GetSize();

	for(int i=0; i<nCount; i++)
	{
		T_ELEM_D ElemData; ElemData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[i], ElemData))	ASSERT(0);

		T_MATD_D MatdData; MatdData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemData.elmat, MatdData))	continue;

		if(MatdData.Type == _T("C"))
		{
			// 1=column, 2=beam, 3=brace, 0=기타
	    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);

			T_SECT_D SectData; SectData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);

			if(bCheck == TRUE) 
			{
				if(nMembType == D_MBTP_COLUMN && 
					(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_B    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SB   || 
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_P    ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SR   ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_OCT  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_SOCT ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_TRK  ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_STRK ||
					 SectData.SectBefore.Shape==D_SECT_SHAPE_REG_HTRK )) ColmSecMap.SetAt(ElemData.elpro, ElemData.elpro);

				if(nMembType == D_MBTP_BEAM && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("T")) &&
					 SectData.nStype==D_SECT_TYPE_REGULAR) 
					 m_arBeamData.SetAt(ElemData.elpro,ElemData.elpro);
			}
		}
	}

	POSITION Pos = ColmSecMap.GetStartPosition();
	while(Pos)
	{
		int SectK=0;
		int rSectK=0;
		ColmSecMap.GetNextAssoc(Pos, SectK, rSectK);
		if(m_arBeamData.Lookup(SectK, rSectK))
		{
			m_arBeamData.RemoveKey(SectK);
		}
	}  

	m_SelectGrid.InitSectGrid(m_arBeamData, 1); // 1 : Dgn Mode

}

LRESULT CDgnConBeamDgnDlgCIVIL::OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam)
{

	ChangeSectionInfo();

	return 0L;
}

LRESULT CDgnConBeamDgnDlgCIVIL::OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam)
{
	if(m_SelectGrid.SelectSection())
		ChangeSectionInfo();
	else
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));

	return 0L;
}

void CDgnConBeamDgnDlgCIVIL::OnDgnBeamSelectAll()
{
	if(!m_SelectGrid.SelectAll())
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	ChangeSectionInfo();

	UpdateData(FALSE);
}

void CDgnConBeamDgnDlgCIVIL::OnDgnBeamUnSelectAll()
{
	ResetSectGridData();

	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// DB 관련 
//
void CDgnConBeamDgnDlgCIVIL::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			UpdateBuffer();
			break;
		case D_UPDATE_PREF_CHANGED:
			// Add by ZINU.('02.10.16). Update changed rebar kind.
			if(m_SubCB.GetCount()>0) m_SubCB.ResetContent();
			CDBLib::GetRebarNameAtComboBox(&m_SubCB);
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnConBeamDgnDlgCIVIL::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bSECD = FALSE;
	BOOL bDEFT = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			// 다른 case도 추가해주어야 한다.
			case(UR_SECD_ADD): 
			case(UR_SECD_DEL): 
			case(UR_SECD_MFS): 
			case(UR_SECD_MFD):
			case(UR_RDGN_ADD): 
			case(UR_RDGN_DEL):  { bSECD = TRUE; break; }
			default:						{ bDEFT = TRUE;	break; }
		}
	}

	if(bSECD) ResetSectGridData(TRUE);
}

