// DgnConDgnConBraceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBraceDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "DgnDataCtrl.h"
#include "DgnConColmBarDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBraceDlg dialog


CDgnConBraceDlg::CDgnConBraceDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConBraceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBraceDlg)
	m_nOption = -1;
	m_nTied = -1;
	//}}AFX_DATA_INIT
	m_iSubRebarYNum = 0;
	m_iSubRebarZNum = 0;

	if(m_arBrceData.GetCount()>0) m_arBrceData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConBraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBraceDlg)
	DDX_Control(pDX, IDC_DGN_RC_BRS, m_S);
	DDX_Control(pDX, IDC_DGN_SUBBARSIZE_COMBO, m_SubRebar);
	DDX_Control(pDX, IDC_DGN_RC_BRHC, m_Hc);
	DDX_Control(pDX, IDC_DGN_RC_BRD0, m_D0);
	DDX_Control(pDX, IDC_DGN_RC_BRBC, m_Bc);
	DDX_Control(pDX, IDC_DGN_RC_BRBCUNIT, m_BcUnit);
	DDX_Control(pDX, IDC_DGN_RC_BRD0UNIT, m_D0Unit);
	DDX_Control(pDX, IDC_DGN_RC_BRHCUNIT, m_HcUnit);
	DDX_Radio(pDX, IDC_DGN_BRACE_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_RC_BRTIED, m_nTied);
	DDX_Control(pDX, IDC_DGN_CONC_BRGRID, m_wndGrid);
	DDX_Control(pDX, IDC_DGN_CONC_SELECTGRID, m_SelectGrid.m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConBraceDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConBraceDlg)
	ON_BN_CLICKED(IDC_DGN_BRACE_AR, OnDgnBraceAr)
	ON_BN_CLICKED(IDC_DGN_BRACE_DEL, OnDgnBraceDel)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_RC_BRINPUTBAR, OnDgnRcBrinputbar)
	ON_BN_CLICKED(IDC_DGN_RC_BRTIED, OnDgnRcBrtied)
	ON_BN_CLICKED(IDC_DGN_RC_BRSPIRAL, OnDgnRcBrspiral)
	ON_BN_CLICKED(IDC_DGN_DESIGN_SECTION, OnDgnDesignSection)
	ON_BN_CLICKED(IDC_DGN_CON_BRCE_SELECTALL, OnDgnBrceSelectAll)
	ON_BN_CLICKED(IDC_DGN_CON_BRCE_UNSELECTALL, OnDgnBrceUnSelectAll)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CLIK, OnChangeFocusSectionGrid)
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CHEK, OnSelectCheckSectionGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBraceDlg message handlers

void CDgnConBraceDlg::OnDgnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConBraceDlg::OnDgnBraceAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnConBraceDlg::OnDgnBraceDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

BOOL CDgnConBraceDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(m_nTied < 0)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_HOOP_TYPE),MB_OK);
		return bCheck;
	}
	if(m_SubRebar.GetCurSel()==-1)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_TIE_SPIRAL_TYPE),MB_OK);
		return bCheck;
	}
	if(m_D0.GetEditValue() <= 0.)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_PROTECT_DIST1),MB_OK);
		return bCheck;
	}
	if(m_wndGrid.GetValueRowCol(2,0)==_T("") && m_wndGrid.GetValueRowCol(2,1)==_T("") &&
		 m_wndGrid.GetValueRowCol(2,2)==_T(""))
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_DATA),MB_OK);
		return bCheck;
	}
	return bCheck;
}

void CDgnConBraceDlg::OnDgnExecute() 
{
	UpdateData(TRUE);

	if(!m_SelectGrid.IsExistEnabledSect())
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_PROP));
		return;
	}

	if(m_nOption == 0)   // Add/Replace
	{
		if(ErrorCheck() == FALSE) return;   
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
		int nRebrSize = arRchKey.GetSize();
		if(nRebrSize > 0)
		{
			CArray<T_REBR_K,T_REBR_K> aRebrK;
			aRebrK.SetSize(nRebrSize);
			for(int i = 0; i < nRebrSize; i++)
			{
				aRebrK[i].keymap = arRchKey[i];
			}
			if(!m_pDoc->m_pDataCtrl->DelRebr(aRebrK))
				bCheck = FALSE;
		}
		if(bCheck)
			SetData2Dlg();
	}

}

BOOL CDgnConBraceDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_SubRebar.GetCount() > 0)	m_SubRebar.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SubRebar);

	InitSectGrid();
	Initial_Unit();
	Initial_GridWnd();
	ShowBitmap(_T("SB"));

	SetData2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConBraceDlg::Initial_Unit()
{
	m_Hc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Bc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D0.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_S.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_HcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D0Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnConBraceDlg::Initial_GridWnd()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	m_wndGrid.GetParam()->EnableUndo(FALSE);

	m_wndGrid.SetRowCount(2);
	m_wndGrid.SetColCount(2);

	m_wndGrid.SetRowHeight(0,1, globalUtils.ScaleByDPI(20),NULL,GX_UPDATENOW);
	m_wndGrid.SetRowHeight(2,2, globalUtils.ScaleByDPI(18),NULL,GX_UPDATENOW);

	Write_GridTitle();

	//  Select가 안되도록 하자 !!!
	m_wndGrid.GetParam()->EnableSelection(FALSE);
	//  줄간격 및 칸간격을 조정하지 못하게 하자 !!!
	m_wndGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_wndGrid.GetParam()->EnableTrackColWidth(FALSE);
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);

	m_wndGrid.GetParam()->EnableUndo(TRUE);
}

void CDgnConBraceDlg::Write_GridTitle()
{
	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(GetSysColor(COLOR_3DFACE));
	//////////////////////////////////////////////////////////////
	//  가로 Title를 찍어줍니다.
	CString title=_LS(IDS_DGN_GRID_MAIN_BAR_PATTERN);
	style.SetValue(title);
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetDraw3dFrame(gxFrameRaised);
	m_wndGrid.SetCoveredCellsRowCol(0,0,0,2);
	m_wndGrid.SetStyleRange(CGXRange(0,0),style);

	title=_T("Nqrb");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(1,0),style);
	m_wndGrid.SetColWidth(0,0,globalUtils.ScaleByDPI(61), nullptr,GX_UPDATENOW);
	m_wndGrid.SetStyleRange(CGXRange(1,0),CGXStyle().SetReadOnly(TRUE));

	title=_T("Nrow");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(1,1),style);
	m_wndGrid.SetColWidth(1,1,globalUtils.ScaleByDPI(62), nullptr,GX_UPDATENOW);
	m_wndGrid.SetStyleRange(CGXRange(1,1),CGXStyle().SetReadOnly(TRUE));

	title=_T("Norb");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(1,2),style);
	m_wndGrid.SetColWidth(2,2,globalUtils.ScaleByDPI(62), nullptr,GX_UPDATENOW);
	m_wndGrid.SetStyleRange(CGXRange(1,2),CGXStyle().SetReadOnly(TRUE));
	//////////////////////////////////////////////////////////////
	title=_T("");
	style.SetValue(title);
	style.SetDraw3dFrame(gxFrameNormal);
	style.SetInterior(RGB(255,255,255));
	style.SetControl(GX_IDS_CTRL_STATIC);
	m_wndGrid.SetStyleRange(CGXRange(2,0),style);

	title=_T("");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,1),style);

	title=_T("");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,2),style);
	style.SetChoiceList(_T(""));
}

void CDgnConBraceDlg::Write_GridItem(int Nqrb,int Nrow,CString Norb)
{
	m_wndGrid.GetParam()->EnableUndo(FALSE);

	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(RGB(255,255,255));
	//////////////////////////////////////////////////////////////
	//  가로 Title를 찍어줍니다.
	CString title=_T("");
	if(Nqrb != 0) title.Format(_T("%d"),Nqrb);
	else          title = _T("");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,0),style);

	if(Nrow != 0) title.Format(_T("%d"),Nrow);
	else          title = _T("");
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,1),style);

	title=Norb;
	style.SetValue(title);
	m_wndGrid.SetStyleRange(CGXRange(2,2),style);
	//////////////////////////////////////////////////////////////
	m_wndGrid.GetParam()->EnableUndo(TRUE);
}

void CDgnConBraceDlg::OnDgnRcBrinputbar() 
{
	CDgnConColmBarDlg dlg;
	dlg.m_ColShape = m_Shape;
	dlg.m_RchkKey  = m_RchkKey;
	dlg.m_ElemType = eDGN_BRCE;
	dlg.m_nD0 = m_D0.GetEditValue();
	dlg.m_Space = m_S.GetEditValue();
	if(dlg.DoModal()==IDOK)
	{
		m_D0.SetEditUnit(dlg.m_nD0);
		m_SubSpace = dlg.m_Space;
		m_S.SetEditUnit(m_SubSpace);
		m_iSubRebarYNum = dlg.m_iSpaceYNum;
		m_iSubRebarZNum = dlg.m_iSpaceZNum;
		Write_GridItem(dlg.m_nNqrb,dlg.m_nNrow,dlg.m_strBarSize);
	}
}

void CDgnConBraceDlg::ShowBitmap(CString str)
{
	if(str==_T("SB"))
	{
		GetDlgItem(IDC_DGN_BRARECT_BIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BRACIRCLE_BIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BRAPIPE_BIT)->ShowWindow(SW_HIDE);
	}
	else if(str==_T("SR"))
	{
		GetDlgItem(IDC_DGN_BRARECT_BIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BRACIRCLE_BIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_BRAPIPE_BIT)->ShowWindow(SW_HIDE);
	}
	else if(str==_T("P"))
	{
		GetDlgItem(IDC_DGN_BRARECT_BIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BRACIRCLE_BIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_BRAPIPE_BIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnConBraceDlg::Initial_ExistData()
{
	m_D0.SetEditUnit(m_RchkData.dConcCenter);
	m_nTied = m_RchkData.nHoopType-1;

	m_SubSpace = m_RchkData.dSubRebarSpace;
	m_S.SetEditUnit(m_SubSpace);
	m_iSubRebarYNum = m_RchkData.iSubRebarNum[0];
	m_iSubRebarZNum = m_RchkData.iSubRebarNum[1];

	int Index = m_SubRebar.FindString(-1,m_RchkData.SubRebarName);
	if(Index != -1) m_SubRebar.SetCurSel(Index);

	Write_GridItem(m_RchkData.nQrb,m_RchkData.nRow,m_RchkData.MainRebarName);

	UpdateData(FALSE);
}

void CDgnConBraceDlg::Set_SectionShape(CString strShape)
{
	m_Shape = strShape;
	CString strSectShp=_T("");
	if(strShape==_T("SB"))			GetDlgItem(IDC_DGN_RC_BRSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_RECT));
	else if(strShape==_T("SR"))	GetDlgItem(IDC_DGN_RC_BRSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_CIR));
	else if(strShape==_T("P"))	GetDlgItem(IDC_DGN_RC_BRSHAPE)->SetWindowText(_LS(IDS_DGN_GRID_TEXT_SECT_PIPE));
	else										GetDlgItem(IDC_DGN_RC_BRSHAPE)->SetWindowText(_T(""));
}

void CDgnConBraceDlg::Set_ShowSRSection()
{
	GetDlgItem(IDC_DGN_BRBC_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_BRCOLON_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BRBC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_BRBCUNIT)->ShowWindow(SW_HIDE);
}

void CDgnConBraceDlg::Set_ShowSBSection()
{
	GetDlgItem(IDC_DGN_BRBC_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BRCOLON_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_BRBC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_BRBCUNIT)->ShowWindow(SW_SHOW);
}

void CDgnConBraceDlg::Set_ShowPSection()
{
	GetDlgItem(IDC_DGN_BRBC_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_BRCOLON_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_BRBC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_BRBCUNIT)->ShowWindow(SW_SHOW);
}

void CDgnConBraceDlg::OnDgnRcBrtied() 
{
	m_nTied = 0;			//	Tied
	UpdateData(FALSE);
}

void CDgnConBraceDlg::OnDgnRcBrspiral() 
{
	m_nTied = 1;			//	Spiral
	UpdateData(FALSE);
}

void CDgnConBraceDlg::OnDgnDesignSection() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));
}

void CDgnConBraceDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			// Add by ZINU.('02.10.16). Update changed rebar kind.
			if(m_SubRebar.GetCount() > 0)	m_SubRebar.ResetContent();
			CDBLib::GetRebarNameAtComboBox(&m_SubRebar);
			break;
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnConBraceDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bOther=FALSE;
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
			case(UR_REBR_ADD):
			case(UR_REBR_DEL):  {bMFD  = TRUE; break;}
			default:						{bOther=TRUE;	break;}
		}
	}

	if(bMFD) ResetSectGridData(TRUE);
}

void CDgnConBraceDlg::InitSectGrid()
{
	// Data Sort & Store
	if(m_arBrceData.GetCount()>0) m_arBrceData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;

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
			// 1=column, 2=Colm, 3=brace, 0=기타
	    int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);

			T_SECT_D SectData; SectData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);

			if(bCheck == TRUE) 
			{
				if(nMembType == D_MBTP_BRACE && (SectData.SectBefore.Shape == _T("SB") || SectData.SectBefore.Shape == _T("SR") ||
															SectData.SectBefore.Shape == _T("P"))) 
					 m_arBrceData.SetAt(ElemData.elpro,ElemData.elpro);
			}
		}
	}

	m_SelectGrid.InitSectGrid(m_arBrceData,5);
}

void CDgnConBraceDlg::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	m_SelectGrid.ResetSectGridData(bKeepChk);
	ChangeSectionInfo();
}

void CDgnConBraceDlg::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();

	if(!m_SelectGrid.GetCurKeyData(m_RchkKey, SectData)) return;

	if(SectData.SectBefore.Shape==_T("SR"))
	{
		Set_SectionShape(_T("SR"));
		ShowBitmap(_T("SR"));
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		Set_ShowSRSection();
	}
	else if(SectData.SectBefore.Shape==_T("SB"))
	{
		Set_SectionShape(_T("SB"));
		ShowBitmap(_T("SB"));
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
		Set_ShowSBSection();
	}
	else if(SectData.SectBefore.Shape==_T("P"))
	{
		Set_SectionShape(_T("P"));
		ShowBitmap(_T("P"));
		m_Hc.SetEditUnit(SectData.SectBefore.SectI.Size[0]);   //  H
		m_Bc.SetEditUnit(SectData.SectBefore.SectI.Size[1]);   //  B
		Set_ShowPSection();
	}

	m_RchkData.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetRebr(m_RchkKey, m_RchkData))  Initial_ExistData();
	else        Initial_WithoutData();

	UpdateData(FALSE);

	UpdateData(TRUE);
	if(m_SelectGrid.IsExistEnabledSect())  
		GetDlgItem(IDC_DGN_RC_BRINPUTBAR)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DGN_RC_BRINPUTBAR)->EnableWindow(FALSE);

}

LRESULT CDgnConBraceDlg::OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam)
{
	ChangeSectionInfo();

	return 0L;
}

LRESULT CDgnConBraceDlg::OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam)
{
	if(m_SelectGrid.SelectSection())
		ChangeSectionInfo();
	else
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));

	return 0L;
}

void CDgnConBraceDlg::OnDgnBrceSelectAll()
{
	if(!m_SelectGrid.SelectAll())
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	ChangeSectionInfo();

	UpdateData(FALSE);
}

void CDgnConBraceDlg::OnDgnBrceUnSelectAll()
{
	ResetSectGridData();

	UpdateData(FALSE);
}

void CDgnConBraceDlg::SetData2Dlg()
{
	// UnSelect All Elem.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	ResetSectGridData();

	m_nOption	= 0;    //  add/replace
	m_nTied   = 0;    //  Type(Tied=0, Spiral=1)
	m_SubSpace= 0.0;
	m_S.SetEditUnit(m_SubSpace);

	m_Shape   = _T("");
	m_bUpdate = FALSE;

	//  Space에 대한 것입니다.
	GetDlgItem(IDC_DGN_RC_BRS)->ShowWindow(SW_HIDE);
	//  Sub Rebar를 초기화시킵니다.
	m_SubRebar.SetCurSel(-1);

	//  대화상자가 바뀌면 변수를 초기화합니다.
	m_Bc.SetEditUnit(0.0);
	m_D0.SetEditUnit(0.0);
	m_Hc.SetEditUnit(0.0);

	GetDlgItem(IDC_DGN_RC_BRSHAPE)->EnableWindow(FALSE);

	m_Hc.EnableWindow(FALSE);
	m_Bc.EnableWindow(FALSE);

	UpdateData(FALSE);

	CGXStyle style;
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	//////////////////////////////////////////////////////////////
	//  철근 데이터 그리드의 값들을 초기화합니다.
	CString title=_T("");
	style.SetValue(title);
	style.SetControl(GX_IDS_CTRL_STATIC);
	for(int j=0; j<3; j++)
		m_wndGrid.SetStyleRange(CGXRange(2,j),style);

	Initial_WithoutData();
}

void CDgnConBraceDlg::Initial_WithoutData()
{
	m_nTied = 0;
	m_D0.SetEditUnit(0.);
	Write_GridItem(0,0,_T(""));
	m_SubRebar.SetCurSel(-1);

	m_SubSpace = 0.0;
	m_S.SetEditUnit(m_SubSpace);
	m_iSubRebarYNum = 0;
	m_iSubRebarZNum = 0;

	UpdateData(FALSE);
}

BOOL CDgnConBraceDlg::SetDlg2Data()
{
	BOOL bCheck = TRUE;
	UpdateData(TRUE);

	m_RchkData.nHoopType   = m_nTied+1;
	m_RchkData.dConcCenter = m_D0.GetEditValue();

	CString strTemp=_T("");
	strTemp = m_wndGrid.GetValueRowCol(2,0);
	m_RchkData.nQrb = _ttoi(strTemp);
	strTemp = m_wndGrid.GetValueRowCol(2,1);
	m_RchkData.nRow = _ttoi(strTemp);
	m_RchkData.MainRebarName = m_wndGrid.GetValueRowCol(2,2);

	int nIndex = m_SubRebar.GetCurSel();
	if(nIndex != -1)  m_SubRebar.GetLBText(nIndex,m_RchkData.SubRebarName);
	m_RchkData.dSubRebarSpace = m_S.GetEditValue();
	m_RchkData.iSubRebarNum[0] = m_iSubRebarYNum;
	m_RchkData.iSubRebarNum[1] = m_iSubRebarZNum;

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
	int nRebrSize = arRchKey.GetSize();
	if(nRebrSize > 0)
	{
		CArray<T_REBR_K,T_REBR_K> aRebrK;
		aRebrK.SetSize(nRebrSize);
		for(int i = 0; i < nRebrSize; i++)
		{
			aRebrK[i].keymap = arRchKey[i];
		}
		if(!m_pDoc->m_pDataCtrl->AddRebr(aRebrK, m_RchkData))
			bCheck = FALSE;
	}

	return bCheck;
}

void CDgnConBraceDlg::SelectElemOnView()
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
					if(DataCtrl.Get_DgnGenMbtp(ElemKey) == D_MBTP_BRACE)  arElemKeyList.Add(ElemKey);
		}
	}
	
	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

