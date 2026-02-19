///////////////////////////////////////////////////////////////////////////
// DgnConBeamDlgCIVIL.cpp : implementation file
//
// 2002. 4. 25
// CIVIL Concrete Design Checking을 위한 Dialog   by TAE
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamDlgCIVIL.h"
#include "DgnConBeamBarDlgCIVIL.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"
#include "DgnConBeamBarDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDlgCIVIL dialog


CDgnConBeamDlgCIVIL::CDgnConBeamDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConBeamDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamDlgCIVIL)
	m_nOption = -1;
	m_bCrack = -1;
	//}}AFX_DATA_INIT

	m_aTobeMovedCtrl.RemoveAll();
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_RebarFrm);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_BGRID);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_INODE);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_MIDDLE);
	m_aTobeMovedCtrl.Add(IDC_DGN_CONC_JNODE);
	m_aTobeMovedCtrl.Add(IDC_DGN_EXECUTE);
	m_aTobeMovedCtrl.Add(IDC_DGN_CLOSE);

	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
}

void CDgnConBeamDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_RC_BHFUNIT, m_HfUnit);
	DDX_Control(pDX, IDC_DGN_RC_BHF, m_Hf);
	DDX_Control(pDX, IDC_DGN_RC_BHCUNIT, m_HcUnit);
	DDX_Control(pDX, IDC_DGN_RC_BHC, m_Hc);
	DDX_Control(pDX, IDC_DGN_RC_BBFUNIT, m_BfUnit);
	DDX_Control(pDX, IDC_DGN_RC_BBF, m_Bf);
	DDX_Control(pDX, IDC_DGN_RC_BBCUNIT, m_BcUnit);
	DDX_Control(pDX, IDC_DGN_RC_BBC, m_Bc);
	DDX_Radio(pDX, IDC_DGN_BEAM_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CON_BEAM_CRACK1, m_bCrack);

	DDX_Control(pDX, IDC_DGN_CONC_SELECTGRID, m_SelectGrid.m_Grid);
	DDX_Control(pDX, IDC_DGN_CONC_BGRID, m_InfoGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConBeamDlgCIVIL, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConBeamDlgCIVIL)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CONC_INODE, OnDgnConcInode)
	ON_BN_CLICKED(IDC_DGN_CONC_JNODE, OnDgnConcJnode)
	ON_BN_CLICKED(IDC_DGN_CONC_MIDDLE, OnDgnConcMiddle)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_DGN_BEAM_AR, OnDgnBeamAr)
	ON_BN_CLICKED(IDC_DGN_BEAM_DEL, OnDgnBeamDel)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_SELECTALL, OnDgnBeamSelectAll)
	ON_BN_CLICKED(IDC_DGN_CON_BEAM_UNSELECTALL, OnDgnBeamUnSelectAll)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CLIK, OnChangeFocusSectionGrid)
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CHEK, OnSelectCheckSectionGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamDlgCIVIL message handlers


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Initialize 관련 
//

//---------------------------------------------------------------------------
// Initialize Unit
//---------------------------------------------------------------------------
void CDgnConBeamDlgCIVIL::SetInitUnit()
{
	m_Hf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Hc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bf.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_HfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_HcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnConBeamDlgCIVIL::ShowRECTBitmap()
{
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_HIDE);
}

void CDgnConBeamDlgCIVIL::ShowTEEBitmap()
{
	GetDlgItem(IDC_DGN_BEAMRECT_BIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BEAMTEE_BIT)->ShowWindow(SW_SHOW);
}

//---------------------------------------------------------------------------
// SetData2Dlg 
//---------------------------------------------------------------------------
void CDgnConBeamDlgCIVIL::SetData2Dlg()
{
	// UnSelect All Elem.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	ResetSectGridData();
	m_nOption = 0;
	m_bCrack  = 0;
		
	m_Hf.SetEditUnit(0.);
	m_Hc.SetEditUnit(0.);
	m_Bf.SetEditUnit(0.);
	m_Bc.SetEditUnit(0.);

	m_Hf.EnableWindow(FALSE);
	m_Hc.EnableWindow(FALSE);
	m_Bf.EnableWindow(FALSE);
	m_Bc.EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_RC_BSHAPE)->EnableWindow(FALSE);
		
	ShowRECTBitmap();
	ShowHideSectCtrl(FALSE);
 
	UpdateData(FALSE);

	// PropNo가 선택이 안되었으므로..
	if(m_SelectGrid.m_arKey.GetCount() <= 0)
	{
		GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(FALSE);
	}

	CString title=_T("");
	CGXStyle style;
	style.SetValue(title);
	style.SetInterior(GetSysColor(COLOR_3DFACE));
	style.SetControl(GX_IDS_CTRL_STATIC);
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
	for(int i=1; i<11; i++)for(int j=1; j<4; j++) m_InfoGrid.SetStyleRange(CGXRange(i,j),style);
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));
}


//---------------------------------------------------------------------------
// OnInitDialog 
//---------------------------------------------------------------------------
BOOL CDgnConBeamDlgCIVIL::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	InitSectGrid();
	InitInfoGrid();
	SetInitUnit();

	SetData2Dlg();

	ChangeDlgItemInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//

//---------------------------------------------------------------------------
// OnDgnClose 
//---------------------------------------------------------------------------
void CDgnConBeamDlgCIVIL::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

//---------------------------------------------------------------------------
// CheckInputData 
//---------------------------------------------------------------------------

void CDgnConBeamDlgCIVIL::CopyRebarData(int FromNode, int ToNode)
{
	for(int i=0; i<5; i++)
	{
		// Top
		m_RchkData.BEAM.iTopBarNum[ToNode][i]  = m_RchkData.BEAM.iTopBarNum[FromNode][i];
		m_RchkData.BEAM.strTopBarNa1[ToNode][i] = m_RchkData.BEAM.strTopBarNa1[FromNode][i];
		m_RchkData.BEAM.strTopBarNa2[ToNode][i] = m_RchkData.BEAM.strTopBarNa2[FromNode][i];
		m_RchkData.BEAM.dDt[ToNode][i]         = m_RchkData.BEAM.dDt[FromNode][i];

		// Bottom
		m_RchkData.BEAM.iBotBarNum[ToNode][i]  = m_RchkData.BEAM.iBotBarNum[FromNode][i];
		m_RchkData.BEAM.strBotBarNa1[ToNode][i] = m_RchkData.BEAM.strBotBarNa1[FromNode][i];
		m_RchkData.BEAM.strBotBarNa2[ToNode][i] = m_RchkData.BEAM.strBotBarNa2[FromNode][i];
		m_RchkData.BEAM.dDb[ToNode][i]         = m_RchkData.BEAM.dDb[FromNode][i];

		// Stirrup
		m_RchkData.BEAM.strSubBarNa[ToNode] = m_RchkData.BEAM.strSubBarNa[FromNode];
		m_RchkData.BEAM.dSubBarDist[ToNode] = m_RchkData.BEAM.dSubBarDist[FromNode];
		m_RchkData.BEAM.dSubBarNum[ToNode]  = m_RchkData.BEAM.dSubBarNum[FromNode];
	}
}

BOOL CDgnConBeamDlgCIVIL::CheckInputData()
{

	// PropNo Check
	T_SECT_D sData;
	sData.Initialize();
	BOOL bGetOK = m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey, sData);
	
	if(!bGetOK){
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Property_Num_Err));
//    AfxMessageBox(_T("[오류] Property Number 선택이 적당하지 않습니다."));
		return FALSE;
	}

	// Rebar Check
	int  nInput = 0;
	BOOL bInput[3];
	for(int i=0; i<3; i++) bInput[i] = TRUE;
	if(m_RchkData.BEAM.iTopBarNum[0][0] == 0) bInput[0] = FALSE; // I_Node
	if(m_RchkData.BEAM.iTopBarNum[1][0] == 0) bInput[1] = FALSE; // Middle
	if(m_RchkData.BEAM.iTopBarNum[2][0] == 0) bInput[2] = FALSE; // J_Node

	for(int i=0; i<3; i++) if(bInput[i] == TRUE) nInput = nInput + 1;

	if(nInput == 0)          // 입력이 하나도 안됨
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Rebar_Info_Err));
//    AfxMessageBox(_T("[오류] Rebar 정보가 입력되지 않았습니다."));
		return FALSE;
	}
	else if(nInput == 3)     // 셋다 입력    
	{
		return TRUE;
	}
	else if(nInput == 1)    // 하나만 입력
	{
		if(bInput[0] == TRUE){        // I_Node 
			CopyRebarData(0, 1);
			CopyRebarData(0, 2);
			WriteNodeInfoOnGrid(1);
			WriteNodeInfoOnGrid(2);
		}
		else if(bInput[1] == TRUE){    // Middle
			CopyRebarData(1, 0);
			CopyRebarData(1, 2);
			WriteNodeInfoOnGrid(0);
			WriteNodeInfoOnGrid(2);
		}
		else if(bInput[2] == TRUE){    // J_Node
			CopyRebarData(2, 0);
			CopyRebarData(2, 1);
			WriteNodeInfoOnGrid(0);
			WriteNodeInfoOnGrid(1);
		}
		return TRUE;
	}
	else if(nInput == 2)    // 두개 입력
	{
		if(bInput[0] == FALSE)        // I_Node X
		{
			CopyRebarData(2, 0);
			WriteNodeInfoOnGrid(0);
		}
		else if(bInput[1] == FALSE)   // Middle X
		{
			CopyRebarData(0, 1);
			WriteNodeInfoOnGrid(1);
		}
		else if(bInput[2] == FALSE)   // J_Node X
		{
			CopyRebarData(0, 2);
			WriteNodeInfoOnGrid(2);
		}

		return TRUE;
	}

	return TRUE;
}

BOOL CDgnConBeamDlgCIVIL::SetDlg2Data()
{
	BOOL bCheck = TRUE;
	UpdateData(TRUE);

	m_RchkData.nEnvironType = m_bCrack;
	m_RchkData.nType = 0;

	CArray<T_RCHK_K, T_RCHK_K> arRchKey;
	arRchKey.RemoveAll();

	int Index = 0;
	T_RCHK_K RchKey;
	POSITION Pos = m_SelectGrid.m_arKey.GetStartPosition();
	while(Pos)
	{
		RchKey=0;
		m_SelectGrid.m_arKey.GetNextAssoc(Pos, Index, RchKey);
		arRchKey.Add(RchKey);
	}

	if(arRchKey.GetSize() > 0)
	{
		if(!m_pDoc->m_pDataCtrl->AddRchk(arRchKey, m_RchkData))
			bCheck = FALSE;
	}

	return bCheck;
}

//---------------------------------------------------------------------------
// OnDgnExecute 
//---------------------------------------------------------------------------
void CDgnConBeamDlgCIVIL::OnDgnExecute() 
{

	UpdateData(TRUE);

	if(!m_SelectGrid.IsExistEnabledSect())
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_SECT));
		return;
	}

	if(m_nOption == 0)   // Add/Replace
	{
		if(CheckInputData() == FALSE) return;   
		if(SetDlg2Data()) SetData2Dlg();
	}
	else                  // Del
	{
		BOOL bCheck = TRUE;
		CArray<T_RCHK_K, T_RCHK_K> arRchKey;
		arRchKey.RemoveAll();

		T_RCHK_K RchKey;
		int Index = 0;
	  POSITION PosSect = m_SelectGrid.m_arKey.GetStartPosition();
	  while(PosSect)
	  {
		  RchKey=0;
		  m_SelectGrid.m_arKey.GetNextAssoc(PosSect, Index, RchKey);
			arRchKey.Add(RchKey);
		}
		if(arRchKey.GetSize() > 0)
		{
			if(!m_pDoc->m_pDataCtrl->DelRchk(arRchKey)) bCheck = FALSE;
		}
		if(bCheck) SetData2Dlg();
	}

}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Change Prop. CB 관련 
//

//---------------------------------------------------------------------------
// SelectElemOnView 
//---------------------------------------------------------------------------
void CDgnConBeamDlgCIVIL::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	arElemKeyList.RemoveAll();

	T_ELEM_K ElemKey;
	T_ELEM_D ElemData;
	CDgnDataCtrl DataCtrl;

	T_RCHK_K RchKey;
	int Index = 0;
	POSITION PosSect = m_SelectGrid.m_arKey.GetStartPosition();
	while(PosSect)
	{
		RchKey=0;
		m_SelectGrid.m_arKey.GetNextAssoc(PosSect, Index, RchKey);

		POSITION PosElem = m_pDoc->m_pAttrCtrl->GetStartElem();
		while(PosElem)
		{
			ElemData.Initialize();
			m_pDoc->m_pAttrCtrl->GetNextElem(PosElem, ElemKey, ElemData);
		
			if(ElemData.elpro == RchKey) 
				if(ElemData.eltyp == TRUSS_EL || ElemData.eltyp == BEAM_EL)
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_BEAM)  arElemKeyList.Add(ElemKey);
		}
	}
	
	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

//---------------------------------------------------------------------------
// ShowHideSectCtrl 
//---------------------------------------------------------------------------
void CDgnConBeamDlgCIVIL::ShowHideSectCtrl(BOOL bShow)
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
// ChangeSectionInfo 
//---------------------------------------------------------------------------
void CDgnConBeamDlgCIVIL::ChangeSectionInfo()
{

	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_SelectGrid.GetCurKeyData(m_RchkKey, SectData))	return;

	// Set TEE Section Data
	if(SectData.SectBefore.Shape==D_SECT_SHAPE_REG_T)
	{
		ShowTEEBitmap();
		ShowHideSectCtrl(TRUE);

		GetDlgItem(IDC_DGN_RC_BSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_TEE));  
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bf.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[2]);   //  Tw
		m_Hf.SetEditUnit(SectData.SectBefore.SectI.Size[3]);   //  Tf1
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

	// Set Rebar Data
	WriteInfoGridTitle();

	m_RchkData.Initialize();
	m_pDoc->m_pAttrCtrl->GetRchk(m_RchkKey, m_RchkData);

	for(int i=0; i<3; i++) WriteNodeInfoOnGrid(i);

	m_bCrack  = m_RchkData.nEnvironType;
	UpdateData(FALSE);

	UpdateData(TRUE);
	if(m_SelectGrid.IsExistEnabledSect())  
	{
		GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(FALSE);
	}

}

void CDgnConBeamDlgCIVIL::ChangeDlgItemInfo()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->ShowWindow(SW_HIDE);  
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK4)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_HIDE);

	CStringArray aCrackEnv;
	aCrackEnv.RemoveAll();

	int iCrackEnvNum = CDBLib::GetCrackEnvList(aCrackEnv);

	AlignControls(iCrackEnvNum);

	if(iCrackEnvNum ==1)
	{
		m_bCrack = 0;
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
	}
	else if(iCrackEnvNum ==2)
	{
		if(m_bCrack > 1) m_bCrack = 0;
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->SetWindowText(aCrackEnv[1]);
	}
	else if(iCrackEnvNum == 3)
	{
		if(m_bCrack > 2) m_bCrack = 0;
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->SetWindowText(aCrackEnv[1]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->SetWindowText(aCrackEnv[2]);
	}
	else if(iCrackEnvNum == 4)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->SetWindowText(aCrackEnv[0]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->SetWindowText(aCrackEnv[1]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK3)->SetWindowText(aCrackEnv[2]);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CON_BEAM_CRACK4)->SetWindowText(aCrackEnv[3]);
	}

	UpdateData(FALSE);
	Invalidate();
}

void CDgnConBeamDlgCIVIL::AlignControls(int iCrackEnvNum)
{
	CRect RectDlg, rRef, rToMove;
	CRect rRect1, rRect2;

	GetWindowRect(&RectDlg);
	GetDlgItem(IDC_DGN_CONC_CrackFrm)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CONC_RebarFrm)->GetWindowRect(rToMove);
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK1)->GetWindowRect(rRect1);
	GetDlgItem(IDC_DGN_CON_BEAM_CRACK2)->GetWindowRect(rRect2);

	CRect rCrack;
	rCrack.left = rRef.left;
	rCrack.top = rRef.top;
	rCrack.right = rRef.right;
	rCrack.bottom = rRef.top + iCrackEnvNum*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top);
	ScreenToClient(rCrack);
	GetDlgItem(IDC_DGN_CONC_CrackFrm)->SetWindowPos(NULL, rCrack.left, rCrack.top, rCrack.Width(), rCrack.Height(), SWP_DRAWFRAME);

	int nDistY = 0;
	if(iCrackEnvNum == 0)
		nDistY = rRef.top - rToMove.top;
	else
		nDistY = rRef.top - rToMove.top + iCrackEnvNum*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top) + globalUtils.ScaleByDPI(8);
	
	CDlgUtil::CtrlMoveDistY(this, m_aTobeMovedCtrl, nDistY);

}

void CDgnConBeamDlgCIVIL::OnDgnBeamAr() 
{
	if(m_SelectGrid.m_arKey.GetCount() <= 0) return;
		
	GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(TRUE);

	GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(TRUE);
	GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(TRUE);
}

void CDgnConBeamDlgCIVIL::OnDgnBeamDel() 
{
	if(m_SelectGrid.m_arKey.GetCount() <= 0) return;

	GetDlgItem(IDC_DGN_CONC_INODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CONC_MIDDLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CONC_JNODE)->EnableWindow(FALSE);
}

//---------------------------------------------------------------------------
// Change Prop. CB
//---------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// SubDialog - Rebar Info 관련 
//

void CDgnConBeamDlgCIVIL::OnDgnConcInode() 
{
	DgnConBeamBarDlgCIVIL dlg;
	dlg.SetRchkData(0, m_RchkKey, m_SelectGrid.m_arKey, &(m_RchkData.BEAM));
	if(dlg.DoModal() == IDOK)
	{
		WriteInfoGridTitle();
		WriteNodeInfoOnGrid(0);
		m_InfoGrid.Redraw();
	}
}

void CDgnConBeamDlgCIVIL::OnDgnConcMiddle() 
{
	DgnConBeamBarDlgCIVIL dlg;
	dlg.SetRchkData(1, m_RchkKey, m_SelectGrid.m_arKey, &m_RchkData.BEAM);
	if(dlg.DoModal() == IDOK)
	{
		WriteInfoGridTitle();
		WriteNodeInfoOnGrid(1);
		m_InfoGrid.Redraw();
	}
}

void CDgnConBeamDlgCIVIL::OnDgnConcJnode() 
{
	DgnConBeamBarDlgCIVIL dlg;
	dlg.SetRchkData(2, m_RchkKey, m_SelectGrid.m_arKey, &m_RchkData.BEAM);
	if(dlg.DoModal() == IDOK)
	{
		WriteInfoGridTitle();
		WriteNodeInfoOnGrid(2);
		m_InfoGrid.Redraw();
	}
}

void CDgnConBeamDlgCIVIL::OnDgnDesignSection() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));	
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// DB 관련 
//

void CDgnConBeamDlgCIVIL::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			UpdateUnit();
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
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnConBeamDlgCIVIL::UpdateUnit()
{
	m_pDoc->m_pUnitCtrl->ConvertUnitRchkPrevious(m_RchkData);    
}

void CDgnConBeamDlgCIVIL::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	BOOL bSECD = FALSE;
	BOOL bDCON = FALSE;
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
			case(UR_RCHK_ADD): 
			case(UR_RCHK_DEL):  { bSECD = TRUE; break; }
			case(UR_DCON_ADD): 
			case(UR_DCON_DEL):  { bDCON = TRUE; break; }
			default:						{ bDEFT = TRUE;	break; }
		}
	}
	if(bSECD) ResetSectGridData(TRUE);
	if(bDCON) ChangeDlgItemInfo();
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Grid 관련
//
void CDgnConBeamDlgCIVIL::InitSectGrid()
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

	m_SelectGrid.InitSectGrid(m_arBeamData);

}

void CDgnConBeamDlgCIVIL::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	m_SelectGrid.ResetSectGridData(bKeepChk);
	ChangeSectionInfo();
}

void CDgnConBeamDlgCIVIL::WriteInfoGridTitle()
{
	CGXStyle style;

	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(GetSysColor(COLOR_3DFACE));

	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
	//////////////////////////////////////////////////////////////
	//  가로 Title를 찍어줍니다.
	CString title=_T("");
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,0),style);
	m_InfoGrid.SetColWidth(0,0,globalUtils.ScaleByDPI(35), nullptr,GX_UPDATENOW);
	
	title=_LS(IDS_DGN_GRID_I_NODE);
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,1),style);
	if(Is2TypeRebar(0)) 
		m_InfoGrid.SetColWidth(1,1,globalUtils.ScaleByDPI(65), nullptr,GX_UPDATENOW);
	else               
		m_InfoGrid.SetColWidth(1,1,globalUtils.ScaleByDPI(53), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_MIDDLE);
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,2),style);
	if(Is2TypeRebar(1)) 
		m_InfoGrid.SetColWidth(2,2,globalUtils.ScaleByDPI(65), nullptr,GX_UPDATENOW);
	else               
		m_InfoGrid.SetColWidth(2,2,globalUtils.ScaleByDPI(53), nullptr,GX_UPDATENOW);

	title=_LS(IDS_DGN_GRID_J_NODE);
	style.SetValue(title);
	m_InfoGrid.SetStyleRange(CGXRange(0,3),style);
	if(Is2TypeRebar(2)) 
		m_InfoGrid.SetColWidth(3,3,globalUtils.ScaleByDPI(65), nullptr,GX_UPDATENOW);
	else               
		m_InfoGrid.SetColWidth(3,3,globalUtils.ScaleByDPI(53), nullptr,GX_UPDATENOW);
	//////////////////////////////////////////////////////////////
	//  세로 Title를 찍어줍니다.
	title=_LS(IDS_DGN_GRID_TOP);
	style.SetValue(title);
	m_InfoGrid.SetCoveredCellsRowCol(1,0,5,0);
	m_InfoGrid.SetStyleRange(CGXRange(1,0),style);

	title=_LS(IDS_DGN_GRID_BOT);
	style.SetValue(title);
	m_InfoGrid.SetCoveredCellsRowCol(6,0,10,0);
	m_InfoGrid.SetStyleRange(CGXRange(6,0),style);

	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));
}

void CDgnConBeamDlgCIVIL::InitInfoGrid()
{
	m_InfoGrid.Initialize();

	m_InfoGrid.GetParam()->EnableUndo(FALSE);

	m_InfoGrid.SetRowCount(10);
	m_InfoGrid.SetColCount(3);

	m_InfoGrid.SetRowHeight(0,0, globalUtils.ScaleByDPI(20),NULL,GX_UPDATENOW);
	m_InfoGrid.SetRowHeight(1,10, globalUtils.ScaleByDPI(18),NULL,GX_UPDATENOW);

	WriteInfoGridTitle();

	m_InfoGrid.GetParam()->EnableSelection(FALSE);
	m_InfoGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_InfoGrid.GetParam()->EnableTrackColWidth(FALSE);
	m_InfoGrid.GetParam()->EnableMoveCols(FALSE);
	m_InfoGrid.GetParam()->EnableMoveRows(FALSE);

	m_InfoGrid.GetParam()->EnableUndo(TRUE);
	m_InfoGrid.SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
}

// nNode = 0 : i Node
// nNode = 1 : Middle
// nNode = 2 : j Node
void CDgnConBeamDlgCIVIL::WriteNodeInfoOnGrid(int nNode)
{
	if(nNode < 0 || nNode > 2) return;
	
	CString strRebar;
	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
 
	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(FALSE));
 
	for(int i=0; i<5; i++)
	{
		// Top Rebar
		strRebar = _T("");
	  style.SetValue(strRebar);
	  m_InfoGrid.SetStyleRange(CGXRange(i+1, nNode+1), style);
		if(m_RchkData.BEAM.iTopBarNum[nNode][i]>0)
		{
			if(m_RchkData.BEAM.strTopBarNa2[nNode][i]!=_T("")){
				if(m_RchkData.BEAM.strTopBarNa2[nNode][i].GetLength() >= 1)
				{
					int nLength = m_RchkData.BEAM.strTopBarNa2[nNode][i].GetLength()-1;
					strRebar.Format(_T("%d-%s,%s"), m_RchkData.BEAM.iTopBarNum[nNode][i], m_RchkData.BEAM.strTopBarNa1[nNode][i], m_RchkData.BEAM.strTopBarNa2[nNode][i].Mid(1, nLength));
				}
			}
			else  strRebar.Format(_T("%d-%s")   , m_RchkData.BEAM.iTopBarNum[nNode][i], m_RchkData.BEAM.strTopBarNa1[nNode][i]);
		}
	  style.SetValue(strRebar);
	  m_InfoGrid.SetStyleRange(CGXRange(i+1, nNode+1), style);

		// Bot Rebar
		strRebar = _T("");
	  style.SetValue(strRebar);
	  m_InfoGrid.SetStyleRange(CGXRange(10-i, nNode+1), style);

		if(m_RchkData.BEAM.iBotBarNum[nNode][i]>0)
		{
			if(m_RchkData.BEAM.strBotBarNa2[nNode][i]!=_T("")) {
				if(m_RchkData.BEAM.strBotBarNa2[nNode][i].GetLength() >= 1)
				{
					int nLength = m_RchkData.BEAM.strBotBarNa2[nNode][i].GetLength()-1;
					strRebar.Format(_T("%d-%s,%s"), m_RchkData.BEAM.iBotBarNum[nNode][i], m_RchkData.BEAM.strBotBarNa1[nNode][i], m_RchkData.BEAM.strBotBarNa2[nNode][i].Mid(1, nLength));
				}
			}
			else strRebar.Format(_T("%d-%s")   , m_RchkData.BEAM.iBotBarNum[nNode][i], m_RchkData.BEAM.strBotBarNa1[nNode][i]);
		}
	  style.SetValue(strRebar);
	  m_InfoGrid.SetStyleRange(CGXRange(10-i, nNode+1), style);

		m_InfoGrid.GetParam()->EnableUndo(TRUE);
	}

	m_InfoGrid.SetStyleRange(CGXRange().SetTable(), CGXStyle().SetReadOnly(TRUE));

}

BOOL CDgnConBeamDlgCIVIL::Is2TypeRebar(int nNode)
{
	for(int i=0; i<5; i++)
	{
		if(m_RchkData.BEAM.strTopBarNa2[nNode][i]!=_T("")) return TRUE;
		if(m_RchkData.BEAM.strBotBarNa2[nNode][i]!=_T("")) return TRUE;
	}
	return FALSE;
}

LRESULT CDgnConBeamDlgCIVIL::OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam)
{

	ChangeSectionInfo();

	return 0L;
}

LRESULT CDgnConBeamDlgCIVIL::OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam)
{
	if(m_SelectGrid.SelectSection())
		ChangeSectionInfo();
	else
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));

	return 0L;
}

void CDgnConBeamDlgCIVIL::OnDgnBeamSelectAll()
{
	if(!m_SelectGrid.SelectAll())
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	ChangeSectionInfo();

	UpdateData(FALSE);
}

void CDgnConBeamDlgCIVIL::OnDgnBeamUnSelectAll()
{
	ResetSectGridData();

	UpdateData(FALSE);
}

