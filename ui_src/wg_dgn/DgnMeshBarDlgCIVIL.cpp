// DgnMeshBarDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnMeshBarDlgCIVIL.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DBCodeDef.h"

#include "DgnConBarInfoDlg.h"
#include "DgnConBarSpaceDlg.h"
#include "DgnSlabRebarPictureDlg.h"
#include "DgnMeshBarLThk4SandwichDlg.h"
#include "DgnDataCtrl.h"

enum
{
	EN_BARSERIES_KS = 0,
	EN_BARSERIES_ASTM,
	EN_BARSERIES_EN,
	EN_BARSERIES_GB,
	EN_BARSERIES_CSA,
	EN_BARSERIES_UNI,
	EN_BARSERIES_TW,
	EN_BARSERIES_SS 
};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned int CDgnMeshBarDlgCIVIL::WM_AFTERINITDIALOG = RegisterWindowMessage(_T("WM_AFTERINITDIALOG"));
/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarDlgCIVIL dialog


CDgnMeshBarDlgCIVIL::CDgnMeshBarDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnMeshBarDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnMeshBarDlgCIVIL)	
	m_strBarShell = _T("");
	m_strSpaceShell = _T("");
	//}}AFX_DATA_INIT

	m_Msrb.Initialize();
	m_pPictureDlg = NULL;

	m_bPrevUnitType = 0;
		
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DGN_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_REBAR_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_REBAR_EDIT);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_REBAR_BTN);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_SPACE_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_SPACE_EDIT);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_SPACE_BTN);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DT_DB_STC);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DIR1_STC);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_XDT_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_XDB_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_XDB_UNIT);
	m_aShellCtrls.Add(IDC_WG_DGN_SHELL_DIR2_STC);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_YDT_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_YDB_EDIT);
	m_aShellCtrls.Add(IDC_DGN_MESH_SHELL_YDB_UNIT);
	m_aShellCtrls.Add(IDC_WG_DGN_STATIC44);
	m_aShellCtrls.Add(IDC_WG_DGN_STATIC46);

	m_aOkCancelCtrls.Add(IDOK);
	m_aOkCancelCtrls.Add(IDCANCEL);
}

void CDgnMeshBarDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnMeshBarDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_XDB_UNIT, m_unitXDbShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_YDB_UNIT, m_unitYDbShell);  
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_XDT_EDIT, m_editXDtShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_XDB_EDIT, m_editXDbShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_YDT_EDIT, m_editYDtShell);
	DDX_Control(pDX, IDC_DGN_MESH_SHELL_YDB_EDIT, m_editYDbShell);  
	DDX_Text(pDX, IDC_WG_DGN_SHELL_REBAR_EDIT, m_strBarShell);
	DDX_Text(pDX, IDC_WG_DGN_SHELL_SPACE_EDIT, m_strSpaceShell);  

	DDX_Control(pDX, IDC_DGN_AUTO_CHK, m_chkAuto);
	DDX_Control(pDX, IDC_DGN_TOP_EDT, m_edtTop);
	DDX_Control(pDX, IDC_DGN_BOT_EDT, m_edtBot);	
	DDX_Control(pDX, IDC_DGN_NUM_OF_ITR_EDT, m_edtItr);
	DDX_Control(pDX, IDC_DGN_TOL_EDT, m_edtTol);
	//}}AFX_DATA_MAP
}

BOOL CDgnMeshBarDlgCIVIL::DestroyWindow() 
{
	if(IsAlivePictureDlg())
	{
		DestroyPictureDlg();
	}
	return CDialog::DestroyWindow();
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnMeshBarDlgCIVIL, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnMeshBarDlgCIVIL)	
	ON_BN_CLICKED(IDC_WG_DGN_SHELL_REBAR_BTN, OnDgnShellbarSize)  
	ON_BN_CLICKED(IDC_WG_DGN_SHELL_SPACE_BTN, OnDgnShellbarSpace)  
	ON_BN_CLICKED(IDC_DGN_AUTO_CHK, OnChkAuto)	

	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CDgnMeshBarDlgCIVIL::WM_AFTERINITDIALOG, OnAfterInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnMeshBarDlgCIVIL message handlers


void CDgnMeshBarDlgCIVIL::OnOK() 
{
	UpdateData(TRUE);
	if(Check_ErrorMsg())
	{
		m_Msrb.Initialize();
		// Set Slab Rebar Size and Spacing Data.
		CArray<CString,CString&> SlabRebarList, SlabSpaceList;
		
		// Set Shell Rebar Size and Spacing Data.
		CArray<CString,CString&> ShellRebarList, ShellSpaceList;
		Get_RebarByString(m_strBarShell, ShellRebarList);
		for(int i=0; i<ShellRebarList.GetSize(); i++)	m_Msrb.RebarShellSize[i] = ShellRebarList.GetAt(i);
		Get_RebarByString(m_strSpaceShell, ShellSpaceList);
		for(int i=0; i<ShellSpaceList.GetSize(); i++)	m_Msrb.RebarShellSpace[i] = ShellSpaceList.GetAt(i);

		// Set Protection Thickness.
		m_Msrb.dB_Shell_X	= m_editXDbShell.GetEditValue();
		m_Msrb.dT_Shell_X	= m_editXDtShell.GetEditValue();
		m_Msrb.dB_Shell_Y	= m_editYDbShell.GetEditValue();
		m_Msrb.dT_Shell_Y	= m_editYDtShell.GetEditValue();

		m_Msrb.bThickOptimal = m_chkAuto.GetCheck();
		m_Msrb.dTopThik = m_edtTop.GetEditValue();		
		m_Msrb.dBotThik = m_edtBot.GetEditValue();		
		m_Msrb.nIteration = m_edtItr.GetEditValueInt();
		m_Msrb.dConvTolerance = m_edtTol.GetEditValue();		

		m_Msrb.bBasicRebar = FALSE;
		BOOL bChangeBasicRebar = FALSE;

		//if(m_bSaveBasicRebar != m_bBasicRebar)
		//{
		//  if(m_pDoc->m_pAttrCtrl->GetCountRbms() > 0)
		//  {
		//    if(IDCANCEL==AfxMessageBox(_LS(IDS_DGN_WARN_REMOVE_SLAB_REBAR), MB_OKCANCEL)) return;
		//  }
		//  bChangeBasicRebar = TRUE;      
		//}
	
		// Save Data at DB.
	  if(m_pDoc->m_pDataCtrl->AddMsrb(m_Msrb, bChangeBasicRebar))	CDialogMove::OnOK();        
	}  
}

BOOL CDgnMeshBarDlgCIVIL::Check_ErrorMsg()
{
	const int nChkSize = 11;
	BOOL bErrorExist[nChkSize] = {FALSE};
	if(m_editXDtShell.GetEditValue() < 0.0)	bErrorExist[0] = TRUE;
	if(m_editXDbShell.GetEditValue() < 0.0)	bErrorExist[1] = TRUE;
	if(m_editYDtShell.GetEditValue() < 0.0)	bErrorExist[2] = TRUE;
	if(m_editYDbShell.GetEditValue() < 0.0)	bErrorExist[3] = TRUE;

	BOOL bExistMsg = FALSE;
	for(int i=0; i<nChkSize; i++)
	{
		if(bErrorExist[i])	bExistMsg = TRUE;
	}
	if(bExistMsg)
	{
		//AfxMessageBox(_LS(IDS_DGN_DLG_ERR_COVER));
		return FALSE;
	}
	else return TRUE;
}

BOOL CDgnMeshBarDlgCIVIL::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_M:   case D_UNITSYS_LENGTH_INDEX_MM:   case D_UNITSYS_LENGTH_INDEX_CM:
		m_bPrevUnitType = 0;
		break;
	case D_UNITSYS_LENGTH_INDEX_IN:   case D_UNITSYS_LENGTH_INDEX_FT:
		m_bPrevUnitType = 1;
		break;
	}
	
	m_edtItr.SetRange(1, 200);
	m_edtItr.SetValue(1);
	m_edtItr.SetInteger(TRUE);

	Initial_UnitData();
	
	// Initialize Data.	
	m_Msrb.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMsrb(m_Msrb))	
		m_pDoc->m_pMatlDB->InitializeMsrb(m_Msrb);
	Initial_ExistData();
	UpdateData(FALSE);	

	EnableDisableCtrls();

	this->PostMessage(WM_AFTERINITDIALOG);

	return TRUE;
}

void CDgnMeshBarDlgCIVIL::EnableDisableCtrls()
{
	BOOL bAuto = m_chkAuto.GetCheck();

	GetDlgItem(IDC_DGN_NUM_OF_ITR_STC)->EnableWindow(bAuto);	
	GetDlgItem(IDC_DGN_NUM_OF_ITR_EDT)->EnableWindow(bAuto);	
	GetDlgItem(IDC_DGN_TOL_STC)->EnableWindow(bAuto);	
	GetDlgItem(IDC_DGN_TOL_EDT)->EnableWindow(bAuto);
}

void CDgnMeshBarDlgCIVIL::Initial_ExistData()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	CString strRebarCodeNa = rPref.DgnCode.ConcRebarMatlCode;

	int iRebarSeries= -1;
	CString strS = m_Msrb.RebarShellSize[0].Left(1);
	if(m_Msrb.RebarShellSize[0].GetLength() == 3)
	{
		CString strCSA = m_Msrb.RebarShellSize[0].Left(3);
		strCSA = strCSA.Right(1);
		if(strCSA == _T("M"))
		{
			strS = _T("M");
		}
	}

	if(strS==_T("D"))
	{
		if (strRebarCodeNa == MATLCODE_CON_CNS || strRebarCodeNa == MATLCODE_CON_CNS560 || strRebarCodeNa == MATLCODE_CON_CNS560_18)
		{
			iRebarSeries = EN_BARSERIES_TW;
		}
		else
		{
			iRebarSeries = EN_BARSERIES_KS;
		}
	}
	else if (strS == _T("#"))
	{
		iRebarSeries = EN_BARSERIES_ASTM;
	}		
	else if(strS==_T("P"))
	{
		if (strRebarCodeNa == MATLCODE_CON_UNI)
		{
			iRebarSeries = EN_BARSERIES_UNI;
		}
		else
		{
			iRebarSeries = EN_BARSERIES_EN;
		}
	}
	else if(strS==_T("d"))	
	{
		iRebarSeries = EN_BARSERIES_GB;
	}
	else if(strS==_T("M"))	
	{    
		iRebarSeries = EN_BARSERIES_CSA;    
	}
	else if(strS==_T("H"))
	{
		iRebarSeries = EN_BARSERIES_SS;    
	}
	else ASSERT(FALSE);

	switch(iRebarSeries) 
	{
		//Rebar Series
		// 0 : _T("D") series
		// 1 : _T("#") series
		// 2 : _T("P") series
		// 3 : _T("d") series
		// 4 : _T("M") series
		// 5 : _T("P") series Itlay.
		// 6 : _T("D") series Taiwan.
		case EN_BARSERIES_KS:
			// Get Shell Rebar Size Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("D10,D13");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@100,@150,@200,@300,@400");
			else									m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		case EN_BARSERIES_ASTM:			
			// Get Shell Rebar Size Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("#4,#5");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@4,@6,@8,@12,@16");
			else									m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		case EN_BARSERIES_EN:
			// Get Shell Rebar Size Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("P10,P12");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@100,@150,@200,@300,@400");
			else									m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		case EN_BARSERIES_GB:
			// Get Shell Rebar Size Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("d10,d12");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@100,@150,@200");
			else									m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		case EN_BARSERIES_CSA:	
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("10M,15M");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@100,@150,@200,@300,@400");
			else									m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		case EN_BARSERIES_UNI:			
			// Get Shell Rebar Size Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("P8,P10");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@100,@150,@200,@300,@400");
			else														  m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		case EN_BARSERIES_TW:			
			// Get Shell Rebar Size Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("D10,D13");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);
			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@100,@150,@200,@300,@400");
			else									m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		case EN_BARSERIES_SS:			
			// Get Shell Rebar Size Data.
			if(m_Msrb.RebarShellSize[0]==_T(""))	m_strBarShell = _T("H10,H12");
			else									m_strBarShell = Add_RebarSizeList(m_Msrb.RebarShellSize);			// Get Shell Rebar Spacing Data.
			if(m_Msrb.RebarShellSpace[0]==_T(""))	m_strSpaceShell = _T("@100,@150,@200,@300,@400");
			else									m_strSpaceShell = Add_RebarSpaceList(m_Msrb.RebarShellSpace);
			break;
		default:
			ASSERT(FALSE);
			break;
	}/*END-switch*/
		
	m_editXDtShell.SetEditUnit(m_Msrb.dT_Shell_X);
	m_editXDbShell.SetEditUnit(m_Msrb.dB_Shell_X);
	m_editYDtShell.SetEditUnit(m_Msrb.dT_Shell_Y);
	m_editYDbShell.SetEditUnit(m_Msrb.dB_Shell_Y);
	
	m_chkAuto.SetCheck(m_Msrb.bThickOptimal);
	//	m_edtElvBot.SetEditUnit(m_Msrb.dElvBot);
	m_edtTop.SetEditUnit(m_Msrb.dTopThik);      
	m_edtBot.SetEditUnit(m_Msrb.dBotThik);    	
	m_edtItr.SetValue(m_Msrb.nIteration);
	m_edtTol.SetEditUnit(m_Msrb.dConvTolerance);

	UpdateData(FALSE);
}

void CDgnMeshBarDlgCIVIL::Initial_UnitData()
{
	m_unitXDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitYDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_editXDtShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editXDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDtShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editYDbShell.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
}

void CDgnMeshBarDlgCIVIL::Init_DefaultData(const int iRebarSeries)
{
	m_editXDtShell.SetEditUnit(0.0);
	m_editXDbShell.SetEditUnit(0.0);
	m_editYDtShell.SetEditUnit(0.0);
	m_editYDbShell.SetEditUnit(0.0);
	switch(iRebarSeries)
	{
		case EN_BARSERIES_KS:
			m_strBarShell	= _T("D10,D13");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_ASTM:
			m_strBarShell	= _T("#4,#5");
			m_strSpaceShell	= _T("@4,@6,@8,@12,@16");
			break;
		case EN_BARSERIES_EN:
			m_strBarShell	= _T("P10,P12");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_GB:
			m_strBarShell	= _T("d10,d12");
			m_strSpaceShell	= _T("@100,@150,@200");
			break;
		case EN_BARSERIES_CSA:
			m_strBarShell	= _T("10M,15M");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_UNI://UNI(S,RC), CNS(S,RC), EN05(S), EN04(RC).
			m_strBarShell	= _T("P8,P10");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		case EN_BARSERIES_TW://UNI(S,RC), CNS(S,RC), EN05(S), EN04(RC).
			m_strBarShell	= _T("D10,D13");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;		
		case EN_BARSERIES_SS:
			m_strBarShell	= _T("H10,H12");
			m_strSpaceShell	= _T("@100,@150,@200,@300,@400");
			break;
		default:
			ASSERT(FALSE);
			break;
	}

	UpdateData(FALSE);	
}

BOOL CDgnMeshBarDlgCIVIL::Get_RebarByString(CString strRebar, CArray<CString,CString&>& SelRebarList)
{
	TCHAR chTemp;
	CString strTemp = _T("");
	int iLen = strRebar.GetLength();
	for(int i=0; i<iLen; i++)
	{
		chTemp = strRebar.GetAt(i);
		if(chTemp==',' || i+1 == iLen)
		{
			if(chTemp==',') SelRebarList.Add(strTemp);
			else			SelRebarList.Add(strTemp + chTemp);
			strTemp = _T("");
		}
		else	strTemp = strTemp + chTemp;
	}
	if(SelRebarList.GetSize() > 0)	return TRUE;
	else							return FALSE;
}

CString CDgnMeshBarDlgCIVIL::Add_RebarList(int iMax, CString* strRebar)
{
	CString strRebarData=_T("");
	for(int i=0; i<iMax; i++)
	{
		CString strTemp = strRebar[i];
		if(strTemp!=_T(""))	strRebarData = strRebarData + strTemp + _T(",");
	}
	int iLen = strRebarData.GetLength();
	if(iLen > 0)	return strRebarData.Left(iLen-1);
	else			return _T("");
}

CString CDgnMeshBarDlgCIVIL::Add_RebarSizeList(CString* strRebar)
{
	const int iConstRebarSizeMax	= 5;

	return Add_RebarList(iConstRebarSizeMax, strRebar);
}

CString CDgnMeshBarDlgCIVIL::Add_RebarSpaceList(CString* strRebar)
{
	const int iConstRebarSpaceMax	= 50;

	return Add_RebarList(iConstRebarSpaceMax, strRebar);
}

void CDgnMeshBarDlgCIVIL::OnDgnShellbarSize() 
{
	UpdateData(TRUE);	
	CDgnConBarInfoDlg dlg;
	dlg.m_strTotalReBarSize = m_strBarShell;
	if(dlg.DoModal() == IDOK)
	{
		m_strBarShell = dlg.m_strTotalReBarSize;
		UpdateData(FALSE);
	}
}

void CDgnMeshBarDlgCIVIL::OnDgnShellbarSpace() 
{
	UpdateData(TRUE);
	CDgnConBarSpaceDlg dlg;
	dlg.m_TotalVSpace = m_strSpaceShell;
	if(dlg.DoModal() == IDOK)
	{
		m_strSpaceShell = dlg.m_TotalVSpace;
		UpdateData(FALSE);
	}
}

LRESULT CDgnMeshBarDlgCIVIL::OnAfterInitDialog(WPARAM wParam, LPARAM lParam)
{
	if(IsAlivePictureDlg()) return 0L;
	
	m_pPictureDlg = new CDgnSlabRebarPictureDlg();
	m_pPictureDlg->SetInitPos(D_INIT_POS_RT);
	m_pPictureDlg->Create(CDgnSlabRebarPictureDlg::IDD, this);
	m_pPictureDlg->ShowWindow(SW_SHOW);

	return 0L;
}

void CDgnMeshBarDlgCIVIL::DestroyPictureDlg()
{
	// Window가 있으면 없앤다.
	if(m_pPictureDlg != NULL)
	{
		m_pPictureDlg->DestroyWindow();
		delete m_pPictureDlg;
		m_pPictureDlg = NULL;    
	}
}

BOOL CDgnMeshBarDlgCIVIL::IsAlivePictureDlg()
{ 
	if(m_pPictureDlg == NULL) return FALSE;
	HWND hWnd = m_pPictureDlg->GetSafeHwnd();
	if(!hWnd) return FALSE;
	if(!(::IsWindow(hWnd))) return FALSE;
	
	return TRUE;
}

CString CDgnMeshBarDlgCIVIL::ConvertSpaceData2String(double dSpace)
{
	CString strSpace;
	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		int nSpace = (int)m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dSpace);
		strSpace.Format(_T("%d"), nSpace);
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		double dTempSpace = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, dSpace);
		strSpace.Format(_T("%f"), dTempSpace);
	}
	else      ASSERT(0);
	
	return strSpace;
}

double CDgnMeshBarDlgCIVIL::ConvertSpaceString2Data(CString& strSpace, BOOL bUnitChange/*=FALSE*/)
{
	double dSpace = 0.0;
	double dTempSpace = 0.0;
	
	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_M:
	case D_UNITSYS_LENGTH_INDEX_MM:
	case D_UNITSYS_LENGTH_INDEX_CM:
		if(m_bPrevUnitType == 1 && bUnitChange)
			dSpace = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0,D_UNITSYS_LENGTH_INDEX_MM,0,D_UNITSYS_LENGTH_INDEX_IN,
			D_UNITSYS_BASE_LENGTH,_tstof(strSpace));
		else
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, MM, D_UNITSYS_BASE_LENGTH);
		break;
	case D_UNITSYS_LENGTH_INDEX_IN:   case D_UNITSYS_LENGTH_INDEX_FT:
		strSpace.Remove('\\');
		if(m_bPrevUnitType == 0 && bUnitChange)
			dSpace = m_pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(0,D_UNITSYS_LENGTH_INDEX_IN,0,D_UNITSYS_LENGTH_INDEX_MM,
			D_UNITSYS_BASE_LENGTH,_tstof(strSpace));
		else
			dSpace = M_InitValueCurUnit(_tstof(strSpace), N, IN, D_UNITSYS_BASE_LENGTH);
		break;
	default:    ASSERT(0);    break;
	}
	
	return dSpace;
}

void CDgnMeshBarDlgCIVIL::OnChkAuto()
{
	EnableDisableCtrls();  
	ChangeCaption();
}

void CDgnMeshBarDlgCIVIL::ChangeCaption()
{
	BOOL bAuto = m_chkAuto.GetCheck();
	if(bAuto)
	{
		GetDlgItem(IDC_DGN_TOP_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_TOP_AUTO));
		GetDlgItem(IDC_DGN_BOT_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_BOT_AUTO));      
	}
	else
	{
		GetDlgItem(IDC_DGN_TOP_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_TOP_MANUAL));
		GetDlgItem(IDC_DGN_BOT_STC)->SetWindowText(_LS(IDS_DGN_LTHK_4_SAND_BOT_MANUAL));      
	}
}