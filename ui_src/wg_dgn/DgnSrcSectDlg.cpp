// DgnSrcSectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSrcSectDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDgnSrcSectDlg dialog


CDgnSrcSectDlg::CDgnSrcSectDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnSrcSectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSrcSectDlg)
	m_nOption = -1;
	m_RebarNo = 0;
	m_RowNo = 0;
	m_nAutoCal = FALSE;
	//}}AFX_DATA_INIT

	if(m_arElemData.GetCount()>0) m_arElemData.RemoveAll();

	//m_SpinNo = 0;
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnSrcSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSrcSectDlg)
	DDX_Control(pDX, IDC_DGN_SRC_REBARUNIT, m_RebarSpaceUnit);
	DDX_Control(pDX, IDC_DGN_SRC_REBARSPACE, m_RebarSpace);
	DDX_Control(pDX, IDC_DGN_SRC_REBARSIZE_COMBO, m_RebarSize);
	DDX_Control(pDX, IDC_DGN_SRC_SUBBARUNIT, m_SubbarSpaceUnit);
	DDX_Control(pDX, IDC_DGN_SRC_SUBBARSPACE, m_SubbarSpace);
	DDX_Control(pDX, IDC_DGN_SRC_SUBBARSIZE_COMBO, m_SubbarSize);
	DDX_Control(pDX, IDC_DGN_SRC_REBARROW, m_RowSpin);
	DDX_Control(pDX, IDC_DGN_SRC_D1, m_D1Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_D2, m_D2Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_D3, m_D3Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_D4, m_D4Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_D5, m_D5Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_D6, m_D6Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_D7, m_D7Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_D8, m_D8Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_HC, m_HcCtrl);
	DDX_Control(pDX, IDC_DGN_SRC_BC, m_BcCtrl);
	DDX_Control(pDX, IDC_DGN_SRC_D0, m_D0Ctrl);
	DDX_Control(pDX, IDC_DGN_SRC_REBARNO, m_RebarSpin);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D1, m_D1Unit);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D2, m_D2Unit);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D3, m_D3Unit);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D4, m_D4Unit);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D5, m_D5Unit);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D6, m_D6Unit);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D7, m_D7Unit);
	DDX_Control(pDX, IDC_DGN_SRC_UNIT_D8, m_D8Unit);
	DDX_Control(pDX, IDC_DGN_SRC_HCUNIT, m_HcUnit);
	DDX_Control(pDX, IDC_DGN_SRC_BCUNIT, m_BcUnit);
	DDX_Control(pDX, IDC_DGN_SRC_D0UNIT, m_D0Unit);
	DDX_Control(pDX, IDC_DGN_SRC_PICTURE, m_wndPicture);
	DDX_Radio(pDX, IDC_DGN_SRC_AR, m_nOption);
	DDX_Text(pDX, IDC_DGN_SRC_REBARNO, m_RebarNo);
	DDX_Text(pDX, IDC_DGN_SRC_REBARROW, m_RowNo);
	DDX_Check(pDX, IDC_DGN_SRC_AUTOCALREBAR, m_nAutoCal);
	DDX_Control(pDX, IDC_DGN_SRC_SELECTGRID, m_SelectGrid.m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSrcSectDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnSrcSectDlg)
	ON_BN_CLICKED(IDC_DGN_SRC_AR, OnDgnSrcAr)
	ON_BN_CLICKED(IDC_DGN_SRC_DEL, OnDgnSrcDel)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_SRC_REBARNO, OnDeltaposDgnSrcRebarspin)
	ON_BN_CLICKED(IDC_DGN_SRC_SECT, OnDgnSrcSect)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_SRC_REBARROW, OnDeltaposDgnSrcRebarrowspin)
	ON_BN_CLICKED(IDC_DGN_SRC_AUTOCALREBAR, OnDgnSrcAutocalrebar)
	ON_BN_CLICKED(IDC_DGN_SRC_SELECTALL, OnDgnSRCSelectAll)
	ON_BN_CLICKED(IDC_DGN_SRC_UNSELECTALL, OnDgnSRCUnSelectAll)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CLIK, OnChangeFocusSectionGrid)
	ON_REGISTERED_MESSAGE(WM_DGN_SECT_GRID_CELL_CHEK, OnSelectCheckSectionGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcSectDlg message handlers

BOOL CDgnSrcSectDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	// Check Error by Shape.
	CString strCheck = m_SrcType.Left(1);
	if(strCheck!=_T("E"))
	{
		// Check Rebar Space.
		double Space = m_RebarSpace.GetEditValue();
		if(Space < 0.)
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_SPACE),MB_OK);
			return bCheck;
		}
		// Check d0.
		double d0  = m_D0Ctrl.GetEditValue();
		double minLen = Get_ShapeMinLength()/2.;
		if(d0 < 0.) 
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_PROTECT_DIST2),MB_OK);
			return bCheck;
		}
		if(minLen <= d0) 
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_D0_CHK2),MB_OK);
			return bCheck;
		}
		// Check Rebar Number.
		double Rem = fmod(m_RebarNo,4.);
		if(m_RebarNo <= 0) 
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_NUMBER1),MB_OK);
			return bCheck;
		}
		if(Rem!=0.) 
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_NUMBER2),MB_OK);
			return bCheck;
		}
		// Check Number of Row.
		if(strCheck!=_T("C"))
		{
			int nCheck = m_RebarNo/2;
			Rem = fmod(m_RowNo,2.);
			if(Rem!=0.)
			{
				bCheck=FALSE;
				AfxMessageBox(_LS(IDS_DGN_CHK_ROW_NUMBER1),MB_OK);
				return bCheck;
			}
			if(m_RowNo <= 0)
			{
				bCheck=FALSE;
				AfxMessageBox(_LS(IDS_DGN_CHK_ROW_NUMBER2),MB_OK);
				return bCheck;
			}
			if(nCheck < m_RowNo)
			{
				bCheck=FALSE;
				AfxMessageBox(_LS(IDS_DGN_CHK_ROW_NUMBER3),MB_OK);
				return bCheck;
			}
		}
	}
	// Check Error by Sub Rebar Space.
	double dSubbarSpace = m_SubbarSpace.GetEditValue();
	if(dSubbarSpace <= 0.)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SUBBAR_SPACE),MB_OK);
		return bCheck;
	}

	return bCheck;
}

double CDgnSrcSectDlg::Get_ShapeMinLength()
{
	double dLen=0.0;
	CString strShape = m_SrcType.Left(1);
	if(strShape==_T("R"))				dLen = min(m_HcCtrl.GetEditValue(), m_BcCtrl.GetEditValue());
	else if(strShape==_T("C"))	dLen = m_HcCtrl.GetEditValue();
	else if(strShape==_T("E"))	// EBC, EPC.
	{
		if(m_SrcType==_T("EBC"))			dLen = min(m_D1Ctrl.GetEditValue(), m_D2Ctrl.GetEditValue());
		else if(m_SrcType==_T("EPC"))	dLen = m_D1Ctrl.GetEditValue();
	}
	return dLen;
}

void CDgnSrcSectDlg::Execute() 
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
		if(arRchKey.GetSize() > 0)
		{
			if(!m_pDoc->m_pDataCtrl->DelRebs(arRchKey))
				bCheck = FALSE;
		}
		if(bCheck)	SetData2Dlg();

	}
}

void CDgnSrcSectDlg::OnDgnSrcSect() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));
}

void CDgnSrcSectDlg::OnDgnSrcAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnSrcSectDlg::OnDgnSrcDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

BOOL CDgnSrcSectDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// Set Main/Sub Rebar Sizes.
	if(m_RebarSize.GetCount() > 0)	m_RebarSize.ResetContent();
	if(m_SubbarSize.GetCount() > 0)	m_SubbarSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_RebarSize, FALSE);
	CDBLib::GetRebarNameAtComboBox(&m_SubbarSize, FALSE);
	// Delete Seleted Element.
	if(m_pDoc==NULL)	return TRUE;
//**/	if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// Initialize Property ComboBox (Search All SRC Section).
	Initial_Data();
	InitSectGrid();

	// Initialize Unit Data.
	Initial_UnitData();
	// Set Uneditable Edit Box.
	Initial_EditBoxStatus();

	SetData2Dlg();

	UpdateData(FALSE);

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(FALSE);

	return TRUE;
}

void CDgnSrcSectDlg::Initial_UnitData()
{
	// Set Unit Type.
	m_D1Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D2Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D3Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D4Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D5Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D6Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D7Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D8Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HcCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BcCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D0Ctrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_RebarSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SubbarSpace.SetUnitType(D_UNITSYS_BASE_LENGTH);
	// Set Dimension Unit Type.
	m_D1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D3Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D4Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D5Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D6Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D7Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D8Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BcUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D0Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_RebarSpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SubbarSpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnSrcSectDlg::Initial_EditBoxStatus()
{
	m_D1Ctrl.EnableWindow(FALSE);
	m_D2Ctrl.EnableWindow(FALSE);
	m_D3Ctrl.EnableWindow(FALSE);
	m_D4Ctrl.EnableWindow(FALSE);
	m_D5Ctrl.EnableWindow(FALSE);
	m_D6Ctrl.EnableWindow(FALSE);
	m_D7Ctrl.EnableWindow(FALSE);
	m_D8Ctrl.EnableWindow(FALSE);
	m_HcCtrl.EnableWindow(FALSE);
	m_BcCtrl.EnableWindow(FALSE);
}

void CDgnSrcSectDlg::Initial_RebarData()
{
	// Get Main Bar Size.
	CString strRebarSize=_T("");
	CDBLib::GetDefaultSrcRebarSize(strRebarSize);
	// Get Sub Bar Size.
	CString strSubbarSize=_T("");
	CDBLib::GetDefaultSrcSubbarSize(strSubbarSize);
	// Get Sub Bar Space.
	double dSubbarSpace=0.0;
	CDBLib::GetDefaultSrcSubbarSpace(dSubbarSpace);
	m_SubbarSpace.SetEditUnit(dSubbarSpace);
	// Set d0.
	m_D0Ctrl.SetEditUnit(0.0);
	// Set Sub Bar Size.
	int iIndex = m_SubbarSize.FindStringExact(-1, strSubbarSize);
	m_SubbarSize.SetCurSel(iIndex);

	if(m_SrcType.Left(1)==_T("R"))	// Rectangle(RHB, RBO, RBC, RPO, RPC, RH2T, RHT).
	{
		// Set Auto Calculate Rebar Space.
		m_nAutoCal = 1;
		Set_RebarSpaceStatus(m_nAutoCal);
		// Initialize Main Rebar Num, Row.
		m_SpinNo  = 4;
		m_RebarNo = 4;
		m_RowNo   = 2;
		m_SpinRebarNo = 2;
		// Initialize Main Rebar Size.
		int Index = m_RebarSize.FindStringExact(-1, strRebarSize);
		m_RebarSize.SetCurSel(Index);
	}
	else if(m_SrcType.Left(1)==_T("C"))	// Circular(CHB, CBO, CBC, CPO, CPC)
	{
		// Set Auto Calculate Rebar Space.
		m_nAutoCal = 1;
		Set_RebarSpaceStatus(m_nAutoCal);
		// Initialize Main Rebar Num, Row.
		m_SpinNo  = 4;
		m_RebarNo = 4;
		m_RowNo   = 0;
		m_SpinRebarNo = 0;
		// Initialize Main Rebar Size.
		int Index = m_RebarSize.FindStringExact(-1, strRebarSize);
		m_RebarSize.SetCurSel(Index);
	}
	else if(m_SrcType.Left(1)==_T("E"))	// Empty(EBC, EPC).
	{
		// Set Auto Calculate Rebar Space.
		m_nAutoCal = 0;
		Set_RebarSpaceStatus(m_nAutoCal);
		// Initialize Main Rebar Num, Row.
		m_SpinNo  = 0;
		m_RebarNo = 0;
		m_RowNo   = 0;
		m_SpinRebarNo = 0;
		// Initialize Main Rebar Size (Not used).
		m_RebarSize.SetCurSel(-1);
	}
	else	AfxMessageBox(_LS(IDS_DGN_CHK_SRC_TYPE),MB_OK);
}

void CDgnSrcSectDlg::Initial_Data()
{
	m_nOption	= 0;	// add/replace
	m_bUpdate = FALSE;
	// Initialize Spin Button.
	m_RebarSpin.SetRange(1,50);
	m_RowSpin.SetRange(1,50);
	// Set RHB at Bitmap.
	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_sect_RIB.svg"));
	// Initialize Steel Type, Concrete Type.
	m_StelType = _T("H");
	m_ConcType = _T("RECT");
	m_SrcType  = D_SECT_SHAPE_SRC_RIB;
	// Initialize Section Dimension, Rebar Space.
	m_D1Ctrl.SetEditUnit(0.);
	m_D2Ctrl.SetEditUnit(0.);
	m_D3Ctrl.SetEditUnit(0.);
	m_D4Ctrl.SetEditUnit(0.);
	m_D5Ctrl.SetEditUnit(0.);
	m_D6Ctrl.SetEditUnit(0.);
	m_D7Ctrl.SetEditUnit(0.);
	m_D8Ctrl.SetEditUnit(0.);
	m_HcCtrl.SetEditUnit(0.);
	m_BcCtrl.SetEditUnit(0.);
	m_RebarSpace.SetEditUnit(0.);
	m_SubbarSpace.SetEditUnit(0.);
}

void CDgnSrcSectDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if(!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
			// do something...
			break;
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
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
		case D_UPDATE_PREF_CHANGED:
			// Add by ZINU.('02.10.16). Update changed rebar kind.
			if(m_RebarSize.GetCount() > 0)	m_RebarSize.ResetContent();
			if(m_SubbarSize.GetCount() > 0)	m_SubbarSize.ResetContent();
			CDBLib::GetRebarNameAtComboBox(&m_RebarSize, FALSE);
			CDBLib::GetRebarNameAtComboBox(&m_SubbarSize, FALSE);
			break;
		default:
			break;
	}
}

BOOL CDgnSrcSectDlg::OnNotifyRowSpin(WPARAM wParam, LPARAM& lParam, LRESULT* pResult)
{
	OnDeltaposDgnSrcRebarrowspin((NMHDR*)lParam, pResult);
	return TRUE;
}

BOOL CDgnSrcSectDlg::OnNotifyRebarSpin(WPARAM wParam, LPARAM& lParam, LRESULT* pResult)
{
	OnDeltaposDgnSrcRebarspin((NMHDR*)lParam, pResult);
	return TRUE;
}

void CDgnSrcSectDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0) return; 
	ASSERT(nCount==1);

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
			case(UR_REBS_ADD):
			case(UR_REBS_DEL):  {bSECD = TRUE; break;}
			case(UR_DSRC_ADD):
			case(UR_DSRC_DEL): 	{bDCON = TRUE; break;}
			default:					  {bDEFT = TRUE; break;}
		}
	}
	if(bSECD) ResetSectGridData(TRUE);
}

void CDgnSrcSectDlg::OnDeltaposDgnSrcRebarrowspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str=_T("");
	GetDlgItem(IDC_DGN_SRC_REBARROW)->GetWindowText(str);

	int Factor = (int)(pNMUpDown->iDelta);
	m_SpinRebarNo = _ttoi(str)+2*Factor;

	if(m_SpinRebarNo < 2)
	{
		if(Factor==1)	m_SpinRebarNo = 2;
		else					m_SpinRebarNo = 100;
	}
	else if(m_SpinRebarNo > 100)  m_SpinRebarNo = 2;

	str.Format(_T("%d"),m_SpinRebarNo);
	GetDlgItem(IDC_DGN_SRC_REBARROW)->SetWindowText(str);

	*pResult = 0;
}

void CDgnSrcSectDlg::OnDeltaposDgnSrcRebarspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str=_T("");
	GetDlgItem(IDC_DGN_SRC_REBARNO)->GetWindowText(str);

	int Factor = (int)(pNMUpDown->iDelta);
	m_SpinNo = _ttoi(str)+4*Factor;
	if(m_ConcType==_T("RECT"))
	{
		if(m_SpinNo <= 2)
		{
			if(Factor==1)	m_SpinNo = 4;
			else					m_SpinNo = 100;
		}
		else if(m_SpinNo > 100)  m_SpinNo = 4;
	}
	else 
	{
		if(m_SpinNo <= 2)
		{
			if(Factor==1)	m_SpinNo = 4;
			else					m_SpinNo = 100;
		}
		else if(m_SpinNo > 100)  m_SpinNo = 4;
	}
	str.Format(_T("%d"),m_SpinNo);
	GetDlgItem(IDC_DGN_SRC_REBARNO)->SetWindowText(str);

	*pResult = 0;
}

void CDgnSrcSectDlg::Set_RebarType(CString Type)
{
	if(Type==_T("RECT"))
	{
		GetDlgItem(IDC_DGN_SRCROW_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_REBARROW)->ShowWindow(SW_SHOW);
	}
	else if(Type==_T("CIR"))
	{
		GetDlgItem(IDC_DGN_SRCROW_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_REBARROW)->ShowWindow(SW_HIDE);
	}
	else	AfxMessageBox(_LS(IDS_DGN_CHK_SECT_SHAPE),MB_OK);
}

CString CDgnSrcSectDlg::Set_SRCStlType(CString strSrcShp, int iBuiltUpFlag)
{
	// Static Box.
	GetDlgItem(IDC_DGN_STATIC_SRC_D1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRC_D2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRC_D3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRC_D4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRC_D5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRC_D6)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRC_D7)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRC_D8)->ShowWindow(SW_SHOW);
	// Edit Box.
	GetDlgItem(IDC_DGN_SRC_D1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_D2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_D3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_D4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_D5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_D6)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_D7)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_D8)->ShowWindow(SW_SHOW);
	// Unit.
	GetDlgItem(IDC_DGN_SRC_UNIT_D1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_UNIT_D2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_UNIT_D3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_UNIT_D4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_UNIT_D5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_UNIT_D6)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_UNIT_D7)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_UNIT_D8)->ShowWindow(SW_SHOW);
	
	CString strStlShp=_T("");
	if(strSrcShp==D_SECT_SHAPE_SRC_RBO)				strStlShp = _T("BOX");
	else if(strSrcShp==D_SECT_SHAPE_SRC_RBC)	strStlShp = _T("BOX");
	else if(strSrcShp==D_SECT_SHAPE_SRC_RPO)	strStlShp = _T("PIPE");
	else if(strSrcShp==D_SECT_SHAPE_SRC_RPC)	strStlShp = _T("PIPE");
	else if(strSrcShp==D_SECT_SHAPE_SRC_CBO)	strStlShp = _T("BOX");
	else if(strSrcShp==D_SECT_SHAPE_SRC_CBC)	strStlShp = _T("BOX");
	else if(strSrcShp==D_SECT_SHAPE_SRC_CPO)	strStlShp = _T("PIPE");
	else if(strSrcShp==D_SECT_SHAPE_SRC_CPC)	strStlShp = _T("PIPE");
	else if(strSrcShp==D_SECT_SHAPE_SRC_EBC)	strStlShp = _T("BOX");
	else if(strSrcShp==D_SECT_SHAPE_SRC_EPC)	strStlShp = _T("PIPE");
	else if(strSrcShp==D_SECT_SHAPE_SRC_RIB)	strStlShp = _T("H");
	else if(strSrcShp==D_SECT_SHAPE_SRC_CIB)	strStlShp = _T("H");
	else if(strSrcShp==D_SECT_SHAPE_SRC_RH2T)	strStlShp = _T("H+2T");
	else if(strSrcShp==D_SECT_SHAPE_SRC_RHT)	strStlShp = _T("H+T");
	else	ASSERT(0);

	if(strStlShp==_T("BOX"))				Show_Stl_B(iBuiltUpFlag);
	else if(strStlShp==_T("H"))			Show_Stl_H();
	else if(strStlShp==_T("PIPE"))	Show_Stl_P();
	else if(strStlShp==_T("H+2T"))	Show_Stl_H2T();
	else if(strStlShp==_T("H+T"))		Show_Stl_HT();
	else	ASSERT(0);

	return strStlShp;
}

void CDgnSrcSectDlg::Set_SRCShape4RebarCtrl()
{
	if(m_SrcType==_T("EBC") || m_SrcType==_T("EPC"))
	{
		// Size.
		GetDlgItem(IDC_STATIC_SRC_REBAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_REBARNO)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SRC_HIPON)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_REBARSIZE_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRCROW_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_REBARROW)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SRC_DO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SRC_DO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_D0)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_D0UNIT)->EnableWindow(FALSE);
		// Main Bar.
		GetDlgItem(IDC_DGN_SRC_AUTOCALREBAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SRC_REBARSPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_REBARSPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_REBARUNIT)->EnableWindow(FALSE);
		// Sub Bar.
		GetDlgItem(IDC_DGN_SRC_SUBBARUNIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_SUBBARSPACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_SRC_SUBBARSIZE_COMBO)->EnableWindow(FALSE);
	}
	else 
	{
		// Size.
		GetDlgItem(IDC_STATIC_SRC_REBAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_REBARNO)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SRC_HIPON)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_REBARSIZE_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRCROW_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_REBARROW)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SRC_DO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SRC_DO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_D0)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_D0UNIT)->EnableWindow(TRUE);
		// Main Bar.
		GetDlgItem(IDC_DGN_SRC_AUTOCALREBAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SRC_REBARSPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_REBARSPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_REBARUNIT)->EnableWindow(TRUE);
		// Sub Bar.
		GetDlgItem(IDC_DGN_SRC_SUBBARUNIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_SUBBARSPACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_SRC_SUBBARSIZE_COMBO)->EnableWindow(TRUE);
	}
}

CString CDgnSrcSectDlg::Set_SRCConType(CString strConShp)
{
	CString strConType=_T(""),strShape=_T("");
	if(strConShp==_T("SB"))
	{
		strConType = _T("RECT");
		if(m_SrcType==_T("EBC") || m_SrcType==_T("EPC"))	Show_Con_None();
		else																			Show_Con_Rect();
	}
	else if(strConShp==_T("SR"))
	{
		strConType = _T("CIR");
		if(m_SrcType==_T("EBC") || m_SrcType==_T("EPC"))	Show_Con_None();
		else																			Show_Con_Cir();
	}
	return strConType;
}

void CDgnSrcSectDlg::Set_SRCMaterialData(int nSrcType)
{
	CString strType=_T("");
	if(nSrcType==0 || nSrcType==1)	// Steel
	{
		strType = _T(" Steel");
	}
	else if(nSrcType==2)	// Concrete
	{
		strType = _T(" Concrete");
	}
	else
	{
		strType = _T("");
	}
	GetDlgItem(IDC_DGN_SRC_BASICMAT)->SetWindowText(strType);
}

void CDgnSrcSectDlg::Set_SRCBitmap(CString Shape)
{
	CString aImagePath[] = {
	_T("SVG\\Illustration\\Dialog\\dgn_sect_RBO.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_RBC.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_RPO.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_RPC.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_CBO.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_CBC.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_CPO.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_CPC.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_RIB.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_CIB.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_EBC.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_EPC.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_RH2T.svg"),
	_T("SVG\\Illustration\\Dialog\\dgn_sect_RHT.svg"),
	};

	CString sPath;
	//  Edit Box Control를 위해서 Shape을 받습니다.
	m_SrcType = Shape;

	if (Shape == D_SECT_SHAPE_SRC_RBO)            sPath = aImagePath[0];
	else if (Shape == D_SECT_SHAPE_SRC_RBC)    sPath = aImagePath[1];
	else if (Shape == D_SECT_SHAPE_SRC_RPO)    sPath = aImagePath[2];
	else if (Shape == D_SECT_SHAPE_SRC_RPC)    sPath = aImagePath[3];
	else if (Shape == D_SECT_SHAPE_SRC_CBO)    sPath = aImagePath[4];
	else if (Shape == D_SECT_SHAPE_SRC_CBC)    sPath = aImagePath[5];
	else if (Shape == D_SECT_SHAPE_SRC_CPO)    sPath = aImagePath[6];
	else if (Shape == D_SECT_SHAPE_SRC_CPC)    sPath = aImagePath[7];
	else if (Shape == D_SECT_SHAPE_SRC_RIB)    sPath = aImagePath[8];
	else if (Shape == D_SECT_SHAPE_SRC_CIB)    sPath = aImagePath[9];
	else if (Shape == D_SECT_SHAPE_SRC_EBC)    sPath = aImagePath[10];
	else if (Shape == D_SECT_SHAPE_SRC_EPC)    sPath = aImagePath[11];
	else if (Shape == D_SECT_SHAPE_SRC_RH2T)    sPath = aImagePath[12];
	else if (Shape == D_SECT_SHAPE_SRC_RHT)    sPath = aImagePath[13];
	else	ASSERT(0);

	m_wndPicture.SetImage(sPath);
}

void CDgnSrcSectDlg::Set_SRCSteelDimension(T_SECT_D rData)
{
	double dD1 = rData.SectBefore.SectI.Size[0];
	double dD2 = rData.SectBefore.SectI.Size[1];
	double dD3 = rData.SectBefore.SectI.Size[2];
	double dD4 = rData.SectBefore.SectI.Size[3];
	double dD5 = rData.SectBefore.SectI.Size[4];
	double dD6 = rData.SectBefore.SectI.Size[5];
	double dD7 = rData.SectBefore.SectI.Size[6];
	double dD8 = rData.SectBefore.SectI.Size[7];

	CString strShp = rData.SectBefore.Shape;
	if(strShp==D_SECT_SHAPE_SRC_RH2T || strShp==D_SECT_SHAPE_SRC_RHT)
	{
		m_D1Ctrl.SetEditUnit(dD1);
		m_D2Ctrl.SetEditUnit(dD2);
		m_D3Ctrl.SetEditUnit(dD3);
		m_D4Ctrl.SetEditUnit(dD4);
		m_D5Ctrl.SetEditUnit(dD5);
		m_D6Ctrl.SetEditUnit(dD6);
		m_D7Ctrl.SetEditUnit(dD7);
		m_D8Ctrl.SetEditUnit(dD8);
	}
	else
	{
		m_D1Ctrl.SetEditUnit(dD1);
		m_D2Ctrl.SetEditUnit(dD2);
		m_D3Ctrl.SetEditUnit(dD3);
		m_D4Ctrl.SetEditUnit(dD4);
		m_D5Ctrl.SetEditUnit(dD6);	// tf2.
		m_D6Ctrl.SetEditUnit(dD5);	// Cw.
		if(dD5==0.0)
		{
			if(m_StelType==_T("B"))	dD5 = dD2 - dD3;
			else								dD5 = dD2;
		}
		if(dD6==0.0)	dD6 = dD4;
	}
}

void CDgnSrcSectDlg::Set_SRCConcreteDimension(T_SECT_D rData)
{
	m_HcCtrl.SetEditUnit(rData.SectBefore.SectJ.Size[0]);
	m_BcCtrl.SetEditUnit(rData.SectBefore.SectJ.Size[1]);
}

void CDgnSrcSectDlg::Show_Stl_B(int iBuiltUpFlag)
{
	// Static Box.
	GetDlgItem(IDC_DGN_STATIC_SRC_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D8)->ShowWindow(SW_HIDE);
	// Edit Box.
	GetDlgItem(IDC_DGN_SRC_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D8)->ShowWindow(SW_HIDE);
	// Unit.
	GetDlgItem(IDC_DGN_SRC_UNIT_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D8)->ShowWindow(SW_HIDE);	

	GetDlgItem(IDC_DGN_STATIC_SRC_D1)->SetWindowText(_T("H"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D2)->SetWindowText(_T("B"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D3)->SetWindowText(_T("tw"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D4)->SetWindowText(_T("tf"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D5)->SetWindowText(_T("tf2"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D6)->SetWindowText(_T("Cw"));

	if(iBuiltUpFlag==2)	// Hide Cw.
	{
		GetDlgItem(IDC_DGN_STATIC_SRC_D6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_D6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_UNIT_D6)->ShowWindow(SW_HIDE);
	}
}

void CDgnSrcSectDlg::Show_Stl_H()
{
	// Static Box.
	GetDlgItem(IDC_DGN_STATIC_SRC_D6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D8)->ShowWindow(SW_HIDE);
	// Edit Box.
	GetDlgItem(IDC_DGN_SRC_D6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D8)->ShowWindow(SW_HIDE);
	// Unit.
	GetDlgItem(IDC_DGN_SRC_UNIT_D6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D8)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_DGN_STATIC_SRC_D1)->SetWindowText(_T("H"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D2)->SetWindowText(_T("B"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D3)->SetWindowText(_T("tw"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D4)->SetWindowText(_T("tf"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D5)->SetWindowText(_T("tf2"));
}

void CDgnSrcSectDlg::Show_Stl_P()
{
	// Static Box.
	GetDlgItem(IDC_DGN_STATIC_SRC_D3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRC_D8)->ShowWindow(SW_HIDE);
	// Edit Box.
	GetDlgItem(IDC_DGN_SRC_D3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_D8)->ShowWindow(SW_HIDE);
	// Unit.
	GetDlgItem(IDC_DGN_SRC_UNIT_D3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_UNIT_D8)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_DGN_STATIC_SRC_D1)->SetWindowText(_T("H"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D2)->SetWindowText(_T("tw"));
}

void CDgnSrcSectDlg::Show_Stl_H2T()
{
	GetDlgItem(IDC_DGN_STATIC_SRC_D1)->SetWindowText(_T("H"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D2)->SetWindowText(_T("B"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D3)->SetWindowText(_T("tw"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D4)->SetWindowText(_T("tf"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D5)->SetWindowText(_T("H'"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D6)->SetWindowText(_T("B'"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D7)->SetWindowText(_T("tw'"));
	GetDlgItem(IDC_DGN_STATIC_SRC_D8)->SetWindowText(_T("tf'"));
}

void CDgnSrcSectDlg::Show_Stl_HT()
{
	Show_Stl_H2T();
}

void CDgnSrcSectDlg::Show_Con_None()
{
	//  Static Box를 Show,Hide
	GetDlgItem(IDC_DGN_STATIC_SRCHC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_STATIC_SRCBC)->ShowWindow(SW_HIDE);
	//  Edit Box를 Show,Hide
	GetDlgItem(IDC_DGN_SRC_HC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_BC)->ShowWindow(SW_HIDE);
	//  Unit를 Show,Hide
	GetDlgItem(IDC_DGN_SRC_HCUNIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_SRC_BCUNIT)->ShowWindow(SW_HIDE);	
}

void CDgnSrcSectDlg::Show_Con_Rect()
{
	//  Static Box를 Show,Hide
	GetDlgItem(IDC_DGN_STATIC_SRCHC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRCBC)->ShowWindow(SW_SHOW);
	//  Edit Box를 Show,Hide
	GetDlgItem(IDC_DGN_SRC_HC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_BC)->ShowWindow(SW_SHOW);
	//  Unit를 Show,Hide
	GetDlgItem(IDC_DGN_SRC_HCUNIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_BCUNIT)->ShowWindow(SW_SHOW);	
}

void CDgnSrcSectDlg::Show_Con_Cir()
{
	//  Static Box를 Show,Hide
	GetDlgItem(IDC_DGN_STATIC_SRCHC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STATIC_SRCBC)->ShowWindow(SW_HIDE);
	//  Edit Box를 Show,Hide
	GetDlgItem(IDC_DGN_SRC_HC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_BC)->ShowWindow(SW_HIDE);
	//  Unit를 Show,Hide
	GetDlgItem(IDC_DGN_SRC_HCUNIT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_SRC_BCUNIT)->ShowWindow(SW_HIDE);	
}

void CDgnSrcSectDlg::OnDgnSrcAutocalrebar() 
{
	UpdateData(TRUE);
	Set_RebarSpaceStatus(m_nAutoCal);
}

void CDgnSrcSectDlg::Set_RebarSpaceStatus(int nCheck)
{
	if(nCheck)    //  Auto Calculate...
	{
		GetDlgItem(IDC_STATIC_SRC_REBARSPACE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_REBARSPACE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SRC_REBARUNIT)->ShowWindow(SW_HIDE);
	}
	else          //  User Define...
	{
		GetDlgItem(IDC_STATIC_SRC_REBARSPACE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_REBARSPACE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SRC_REBARUNIT)->ShowWindow(SW_SHOW);
	}
}

void CDgnSrcSectDlg::InitSectGrid()
{

	// Data Sort & Store
	if(m_arElemData.GetCount()>0) m_arElemData.RemoveAll();

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;

	m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);
	int nCount = arElemKeyList.GetSize();

	for(int i=0; i<nCount; i++)
	{
		T_ELEM_D ElemData; ElemData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[i], ElemData))	ASSERT(0);

		int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[i]);

		T_SECT_D SectData; SectData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetSectDesign(ElemData.elpro,SectData);

		if(bCheck == TRUE) 
		{
			if(SectData.nStype==D_SECT_TYPE_SRC && nMembType == D_MBTP_COLUMN) 
				m_arElemData.SetAt(ElemData.elpro,ElemData.elpro);
		}
	}
	m_SelectGrid.InitSectGrid(m_arElemData,2);
}

void CDgnSrcSectDlg::ResetSectGridData(BOOL bKeepChk/*FALSE*/)
{
	m_SelectGrid.ResetSectGridData(bKeepChk);
	ChangeSectionInfo();
}

void CDgnSrcSectDlg::SelectElemOnView()
{
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
	arElemKeyList.RemoveAll();

	T_ELEM_K ElemKey;
	T_ELEM_D ElemData;

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
			{
				if(ElemData.eltyp == TRUSS_EL || ElemData.eltyp == BEAM_EL)
					arElemKeyList.Add(ElemKey);
			}
		}
	}
	m_pDoc->m_pViewCtrl->SelectElem(NULL, arElemKeyList, FALSE);
}

void CDgnSrcSectDlg::ChangeSectionInfo()
{
	// Select Elem. On View
	SelectElemOnView();

	T_SECT_D SectData; SectData.Initialize();
	if(!m_SelectGrid.GetCurKeyData(m_RchkKey, SectData)) return;

	// Set Bitmap.
	Set_SRCBitmap(SectData.SectBefore.Shape);
	// Set Steel Dimension.
	Set_SRCSteelDimension(SectData);
	// Set Concrete Dimension.
	Set_SRCConcreteDimension(SectData);
	// Set Material Data.
	Set_SRCMaterialData(SectData.SectBefore.nSRCtype);
	// Set Steel Type.
	m_StelType = Set_SRCStlType(SectData.SectBefore.Shape, SectData.SectBefore.SectI.BuiltUpFlag);
	// Set Concrete Type.
	m_ConcType = Set_SRCConType(SectData.SectBefore.SectJ.Shape);
	// Set EnableWindow Rebar by Section Shape.
	Set_SRCShape4RebarCtrl();
	// Set EnableWindow Number of Rebar by Concrete Type.
	Set_RebarType(m_ConcType);
	//if(m_ConcType==_T("CIR") && m_SpinNo==4)				m_SpinNo = 4;
	//else if(m_ConcType==_T("RECT") && m_SpinNo==4)	m_SpinNo = 4;
	//CString str=_T("");
	//str.Format(_T("%d"),m_SpinNo);
	//GetDlgItem(IDC_DGN_SRC_REBARNO)->SetWindowText(str);
	// Set Rebar Data.

	m_RchkData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetRebs(m_RchkKey, m_RchkData))
	{
		Initial_RebarData();
		return;
	}
	// Set d0.
	m_D0Ctrl.SetEditUnit(m_RchkData.dConcCenter);
	// Set Main Rebar Size.
	int Index1 = m_RebarSize.FindStringExact(-1,m_RchkData.MainRebarName);
	if(Index1!=-1)	m_RebarSize.SetCurSel(Index1);
	else						m_RebarSize.SetCurSel(-1);
	// Set Main Rebar Space.
	m_nAutoCal = m_RchkData.bUseRebarSpace;
	Set_RebarSpaceStatus(m_nAutoCal);
	if(m_nAutoCal)	m_RebarSpace.SetEditUnit(0.0);
	else						m_RebarSpace.SetEditUnit(m_RchkData.dRebarSpace);
	// Set Main Rebar Numbers and Rows.
	m_RebarNo = m_RchkData.nQrb;
	m_RowNo   = m_RchkData.nRow;
	// Set Sub Rebar Size.
	int Index2 = m_SubbarSize.FindStringExact(-1,m_RchkData.SubRebarName);
	if(Index2!=-1)	m_SubbarSize.SetCurSel(Index2);
	else						m_SubbarSize.SetCurSel(-1);
	// Set Sub Rebar Space.
	m_SubbarSpace.SetEditUnit(m_RchkData.dSubRebarSpace);

	UpdateData(FALSE);
}

void CDgnSrcSectDlg::SetData2Dlg()
{
	// UnSelect All Elem.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	ResetSectGridData();
	Initial_Data();
	Initial_RebarData();
	Set_SRCMaterialData(5);

}

BOOL CDgnSrcSectDlg::SetDlg2Data()
{
	BOOL bCheck = TRUE;
	UpdateData(TRUE);

	m_RchkData.dConcCenter = m_D0Ctrl.GetEditValue();
	CString strRebarSize=_T("");
	int Index1 = m_RebarSize.GetCurSel();
	if(Index1!=-1)	m_RebarSize.GetLBText(Index1,strRebarSize);
	m_RchkData.MainRebarName	= strRebarSize;
	m_RchkData.nQrb					= m_RebarNo;
	m_RchkData.nRow					= m_RowNo;
	if(m_nAutoCal)
	{
		m_RchkData.bUseRebarSpace = TRUE;
		m_RchkData.dRebarSpace    = 0.;
	}
	else
	{
		m_RchkData.bUseRebarSpace = FALSE;
		m_RchkData.dRebarSpace		 = m_RebarSpace.GetEditValue();
	}
	// Get Sub Rebar Data.
	CString strSubbarSize=_T("");
	int Index2 = m_SubbarSize.GetCurSel();
	if(Index2!=-1)	m_SubbarSize.GetLBText(Index2,strSubbarSize);
	m_RchkData.SubRebarName		= strSubbarSize;
	m_RchkData.dSubRebarSpace	= m_SubbarSpace.GetEditValue();

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
		if(!m_pDoc->m_pDataCtrl->AddRebs(arRchKey, m_RchkData))
			bCheck = FALSE;
	}
	return bCheck;
}

LRESULT CDgnSrcSectDlg::OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam)
{

	ChangeSectionInfo();

	return 0L;
}

LRESULT CDgnSrcSectDlg::OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam)
{
	if(m_SelectGrid.SelectSection())
		ChangeSectionInfo();
	else
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));

	return 0L;
}

void CDgnSrcSectDlg::OnDgnSRCSelectAll()
{
	if(!m_SelectGrid.SelectAll())
	{
		AfxMessageBox(_LS(IDS_DGN_CON_SELECT_DiffShpe_Err));
		return;
	}

	ChangeSectionInfo();

	UpdateData(FALSE);
}

void CDgnSrcSectDlg::OnDgnSRCUnSelectAll()
{
	ResetSectGridData();

	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
MSRCSectRowSpin::MSRCSectRowSpin()
: CFormulaEditSpin()
{
}

MSRCSectRowSpin::~MSRCSectRowSpin()
{

}

BOOL MSRCSectRowSpin::DoScroll(int nScrollSteps)
{
	NM_UPDOWN dataNMUD;
	memset(&dataNMUD, 0, sizeof(NM_UPDOWN));
	dataNMUD.iDelta = nScrollSteps;
	LRESULT lResult;
	return OnNotify(NULL, (LPARAM)&dataNMUD, &lResult);
}

BOOL MSRCSectRowSpin::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CDgnSrcSectDlg* pParent = (CDgnSrcSectDlg*)GetParent();
	return pParent->OnNotifyRowSpin(wParam, lParam, pResult);
}

MSRCSectRebarSpin::MSRCSectRebarSpin()
: CFormulaEditSpin()
{
}

MSRCSectRebarSpin::~MSRCSectRebarSpin()
{

}

BOOL MSRCSectRebarSpin::DoScroll(int nScrollSteps)
{
	NM_UPDOWN dataNMUD;
	memset(&dataNMUD, 0, sizeof(NM_UPDOWN));
	dataNMUD.iDelta = nScrollSteps;
	LRESULT lResult;
	return OnNotify(NULL, (LPARAM)&dataNMUD, &lResult);
}

BOOL MSRCSectRebarSpin::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CDgnSrcSectDlg* pParent = (CDgnSrcSectDlg*)GetParent();
	return pParent->OnNotifyRebarSpin(wParam, lParam, pResult);
}
