// DgnConColmBarDlgCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConColmBarDlgCIVIL.h"
#include "DgnColumnGridWndCIVIL.h"
#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmBarDlgCIVIL dialog


CDgnConColmBarDlgCIVIL::CDgnConColmBarDlgCIVIL(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConColmBarDlgCIVIL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConColmBarDlgCIVIL)
	m_nType = 0;
	m_ColGrid = NULL;
	//}}AFX_DATA_INIT
}

void CDgnConColmBarDlgCIVIL::SetRchkData(T_RCHK_K RchkKey, CMap<int,int,T_RCHK_K,T_RCHK_K>& arRchkKey, T_RCHK_COLM* pColmData)
{
	m_RchkKey = RchkKey;

	m_pColmData = NULL;
	m_pColmData = pColmData;

	int Index=0;
	T_RCHK_K RchkKey1;
	m_arRchkKey.RemoveAll();
	POSITION Pos = arRchkKey.GetStartPosition();
	while(Pos)
	{
		RchkKey1 = 0;
		arRchkKey.GetNextAssoc(Pos, Index, RchkKey1);
		m_arRchkKey.SetAt(Index,RchkKey1);
	}

}

void CDgnConColmBarDlgCIVIL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConColmBarDlgCIVIL)
	DDX_Control(pDX, IDC_DGN_RC_As_UT, m_AsUT);
	DDX_Control(pDX, IDC_DGN_RC_As, m_As);
	DDX_Control(pDX, IDC_DGN_NUM_CB, m_Num);
	DDX_Control(pDX, IDC_DGN_SPACE_UNIT, m_SpaceUT);
	DDX_Control(pDX, IDC_DGN_SPACE, m_Space);
	DDX_Control(pDX, IDC_DGN_SIZE_CB, m_SizeCB);
	DDX_Control(pDX, IDC_DGN_LAYER_CB, m_LayerCB);
	DDX_Radio(pDX, IDC_DGN_HOOP_TYPE_RD1, m_nType);

	DDX_Control(pDX, IDC_DGN_GRID, *m_ColGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConColmBarDlgCIVIL, CDialogMove)
	//{{AFX_MSG_MAP(CDgnConColmBarDlgCIVIL)
	ON_CBN_SELCHANGE(IDC_DGN_LAYER_CB, OnSelchangeDgnLayerCb)
	ON_BN_CLICKED(IDC_REDRAW_BTN, OnRedrawBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmBarDlgCIVIL message handlers


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Initialize 관련 
//

//--------------------------------------------------------------------------
// Init ComboBox
//--------------------------------------------------------------------------
void CDgnConColmBarDlgCIVIL::SetInitRebarCB()
{
	if(m_SizeCB.GetCount()>0) m_SizeCB.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_SizeCB);
/*
	if(m_Num.GetCount()>0) m_Num.ResetContent();
	CStringArray aRebarNum;
	if(CDBLib::GetSubRebarNumList_Civil(aRebarNum))
	{
		for(int i=0; i<aRebarNum.GetSize(); i++)	m_Num.AddString(aRebarNum[i]);
	}
*/
}

void CDgnConColmBarDlgCIVIL::SetInitLayerCB()
{
	CString str;
	m_LayerCB.ResetContent();

	for(int i=0; i<5; i++)
	{
		str.Format(_T("%d"), i+1);
		m_LayerCB.AddString(str);  
	}

}

//--------------------------------------------------------------------------
// Init Data
//--------------------------------------------------------------------------
void CDgnConColmBarDlgCIVIL::SetData2Dlg()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	m_Space.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SpaceUT.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_As.SetUnitType(D_UNITSYS_BASE_AREA);
	m_AsUT.SetUnitType(D_UNITSYS_BASE_AREA);
	m_Num.SetUnitType(D_UNITSYS_NONE);

	// StirrupBar
	m_Num.SetEditUnit(m_pColmData->dSubBarNum[0]);
//  m_Num.SelectString(-1, m_pColmData->dSubBarNum);
	m_Space.SetEditUnit(m_pColmData->dSubBarDist[0]);
	int nIndex = m_SizeCB.SelectString(-1, m_pColmData->strSubBarNa[0]);

	// Rebar Layer
	CString str;
	m_nLayerNum = 0;
	for(int i=0; i<5; i++)
	{
		if(m_pColmData->iBarNum[0][0][i]>0 || 
			 m_pColmData->iBarNum[0][1][i]>0 || 
			 m_pColmData->iBarNum[0][2][i]>0) m_nLayerNum++;
	}

	if(m_nLayerNum == 0) m_nLayerNum = 3;
	str.Format(_T("%d"), m_nLayerNum);
	nIndex = m_LayerCB.SelectString(-1, str);
	ASSERT(nIndex != LB_ERR);

	// Grid
	SetDataToGrid();
	m_ColGrid->SetLayerNum(m_RchkKey, m_nLayerNum); 

	// As
	CDgnDataCtrl DataCtrl;
	T_UNIT_INDEX CurIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CDBDoc::GetDocPoint()->m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);
	double dAsc=0.0;
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(m_pColmData->iBarNum[0][j][i] > 0)
			{
				double dAs1=0.0, dAs2=0.0;
				if(m_pColmData->strBarNa1[0][j][i]!=_T(""))  dAs1 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pColmData->strBarNa1[0][j][i]);
				if(m_pColmData->strBarNa2[0][j][i]!=_T(""))  dAs2 = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RebarArea(m_pColmData->strBarNa2[0][j][i]);
				if(dAs2==0.0) dAs2 = dAs1;
				// Change by ZINU.('06.02.27). ex) 5(2.5EA + 2.5EA) -> 5(3EA + 2EA).
				int iBarNum = m_pColmData->iBarNum[0][j][i];
				double dBarNum1 = ceil(iBarNum/2.);
				double dBarNum2 = iBarNum - dBarNum1;
				dAsc += DataCtrl.Get_UsedAs(m_RchkKey, j+1, dAs1*dBarNum1 + dAs2*dBarNum2);
			}
		}
	}
	UpdateAsInfo(dAsc);

	UpdateData(FALSE);
}

//--------------------------------------------------------------------------
// Init Dialog
//--------------------------------------------------------------------------
BOOL CDgnConColmBarDlgCIVIL::OnInitDialog() 
{
	if (m_ColGrid == nullptr)
		m_ColGrid = new CDgnColumnGridWndCIVIL();

	CDialogMove::OnInitDialog();
 
	// Init RebarCB
	SetInitRebarCB();

	// Init LayerCB
	SetInitLayerCB();

	// Init Grid
	m_ColGrid->Initialize();  

	// Init Data
	SetData2Dlg();

	// Init View
	CWnd* PlaceHolder = GetDlgItem(IDC_STATIC_VIEW);
	ASSERT(PlaceHolder);
	m_SectWnd.Initial(PlaceHolder);
	UpdateViewWnd();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//

BOOL CDgnConColmBarDlgCIVIL::CheckInputData()
{

	int iChkResult = 0;
	_DGNC_RBAR RebarData;
	
	int nError;
	CString str, errMsg;
	CDgnDataCtrl DataCtrl;

	int Index=0;
	T_RCHK_K RchkKey; 
	POSITION Pos = m_arRchkKey.GetStartPosition();
	while(Pos)
	{
		RebarData.Initialize();
		RchkKey=0; 
		str=_T(""); errMsg=_T("");
		m_arRchkKey.GetNextAssoc(Pos, Index, RchkKey);

		RebarData.iSectNo = RchkKey;
		RebarData.iBarLayc = m_nLayerNum;

		for(int i=0; i<m_nLayerNum; i++)
		{
			// pos1
			str = m_ColGrid->GetValueRowCol(i+2, 1);
			RebarData.iBarNum[0][i] = _ttoi(str);
			str = m_ColGrid->GetValueRowCol(i+2, 2);
			RebarData.strBarNa1[0][i] = str;
			str = m_ColGrid->GetValueRowCol(i+2, 3);
			RebarData.strBarNa2[0][i] = str;
			// pos2
			str = m_ColGrid->GetValueRowCol(i+2, 4);
			RebarData.iBarNum[1][i] = _ttoi(str);
			str = m_ColGrid->GetValueRowCol(i+2, 5);
			RebarData.strBarNa1[1][i] = str;
			str = m_ColGrid->GetValueRowCol(i+2, 6);
			RebarData.strBarNa2[1][i] = str;

			// pos3
			str = m_ColGrid->GetValueRowCol(i+2, 7);
			RebarData.iBarNum[2][i] = _ttoi(str);
			str = m_ColGrid->GetValueRowCol(i+2, 8);
			RebarData.strBarNa1[2][i] = str;
			str = m_ColGrid->GetValueRowCol(i+2, 9);
			RebarData.strBarNa2[2][i] = str;

			str = m_ColGrid->GetValueRowCol(i+2, 10);
			RebarData.dDc[i] = _tstof(str);
			double CL = 0;
			if(i == 0) CL = 0;
			else       CL = RebarData.dDc[i-1]; 
		}

		// Check Rebar Data
		nError = DataCtrl.Check_RbarCivil(2, RebarData);

		if(nError == 0)
		{
			for(int pos=0; pos<3; pos++)
			{
				for(int i=0; i<m_nLayerNum; i++)
				{
					if(RebarData.bCheckRebar[pos][i] == FALSE)  iChkResult |= 2;

					if(RebarData.bCheckSpace[pos][i] == FALSE)  iChkResult |= 32;

					if(RebarData.bCheckCovc[i] == FALSE)        iChkResult |= 64;
				}
			}
		}
		else 
		{
			if(nError == 1)       iChkResult |= 4;
			else if(nError == 2)  iChkResult |= 8;
			else if(nError == 3)  iChkResult |= 16;
		}
	}

	if(iChkResult & 2)
	{
		//errMsg.Format(_T("[오류] %d Layer(pos%d)의 Rebar 정보가 올바르지 않습니다."), i+1, pos+1);
		AfxMessageBox(_LS(IDS_DGN_CON_COLM_Pos_Rebar_Info_Err));
		return FALSE;
	}
	else if(iChkResult & 4)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Code));      
		return FALSE;
	}
	else if(iChkResult & 8)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_Not_exist_Section)); 
		return FALSE;
	}
	else if(iChkResult & 16)
	{
		AfxMessageBox(_LS(IDS_DGN_CON_BEAM_No_good_Section));   
		return FALSE;
	}
	else if(iChkResult & 32)
	{
		//errMsg.Format(_T("[경고] %d Layer(pos%d)의 철근 간격이 적당하지 않습니다.\n계속 하시겠습니까?"), i+1, pos+1);
		if(AfxMessageBox(_LS(IDS_DGN_CON_COLM_Pos_Rebar_Space_Warning), MB_YESNO) == IDNO) return FALSE;
	}
	else if(iChkResult & 64)
	{
		//errMsg.Format(_T("[경고] %d Layer의 Dc 값이 적당하지 않습니다.\n계속 하시겠습니까?"), i+1);
		if(AfxMessageBox(_LS(IDS_DGN_CON_COLM_Pos_Rebar_Dc_Warning), MB_YESNO) == IDNO) return FALSE;
	}

	UpdateAsInfo(RebarData.dAsc);

	return TRUE;
}

void CDgnConColmBarDlgCIVIL::SetDlg2Data()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	UpdateData(TRUE);

	// Stirrup Bar
/*
	int nIndex = m_Num.GetCurSel();
	if(nIndex >= 0) m_Num.GetLBText(nIndex, m_pColmData->dSubBarNum);
	//m_pColmData->iSubBarNum = m_Num.GetEditValue();
*/
	m_pColmData->dSubBarNum[0] = m_Num.GetEditValue();
	m_pColmData->dSubBarDist[0] = m_Space.GetEditValue();

	int nIndex = m_SizeCB.GetCurSel();
	if(nIndex >= 0) m_SizeCB.GetLBText(nIndex, m_pColmData->strSubBarNa[0]);
	
	// Rebar
	GetDataFromGrid();
}

void CDgnConColmBarDlgCIVIL::OnCancel() 
{	
	CDialogMove::OnCancel();
}

void CDgnConColmBarDlgCIVIL::OnOK() 
{
	// Data Checking
	if(CheckInputData() == FALSE) return;

	// Save Data
	SetDlg2Data();
 
	CDialogMove::OnOK();
}

void CDgnConColmBarDlgCIVIL::PostNcDestroy() 
{
	if(m_ColGrid != NULL){ delete m_ColGrid; m_ColGrid = NULL; }

	CDialogMove::PostNcDestroy();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Grid 관련 
//

void CDgnConColmBarDlgCIVIL::OnSelchangeDgnLayerCb() 
{
	CString strSel = _T("");
	int nIndex, nLayer;

	nIndex = m_LayerCB.GetCurSel();
	if(nIndex>=0) m_LayerCB.GetLBText(nIndex, strSel);
	nLayer = _ttoi(strSel);

	if(nLayer<1 || nLayer>5) return;

	m_nLayerNum = nLayer;
	m_ColGrid->SetLayerNum(m_RchkKey, m_nLayerNum);

}

void CDgnConColmBarDlgCIVIL::SetDataToGrid()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	for(int i=0; i<5; i++)
	{
		m_ColGrid->SetValueRange(CGXRange(i+2,1), double(m_pColmData->iBarNum[0][0][i]));
		m_ColGrid->SetValueRange(CGXRange(i+2,2), m_pColmData->strBarNa1[0][0][i]);
		m_ColGrid->SetValueRange(CGXRange(i+2,3), m_pColmData->strBarNa2[0][0][i]);

		m_ColGrid->SetValueRange(CGXRange(i+2,4), double(m_pColmData->iBarNum[0][1][i]));
		m_ColGrid->SetValueRange(CGXRange(i+2,5), m_pColmData->strBarNa1[0][1][i]);
		m_ColGrid->SetValueRange(CGXRange(i+2,6), m_pColmData->strBarNa2[0][1][i]);

		m_ColGrid->SetValueRange(CGXRange(i+2,7), double(m_pColmData->iBarNum[0][2][i]));
		m_ColGrid->SetValueRange(CGXRange(i+2,8), m_pColmData->strBarNa1[0][2][i]);
		m_ColGrid->SetValueRange(CGXRange(i+2,9), m_pColmData->strBarNa2[0][2][i]);

		m_ColGrid->SetValueRange(CGXRange(i+2,10), m_pColmData->dDc[0][i]);
	}
}

void CDgnConColmBarDlgCIVIL::GetDataFromGrid()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	CString str = _T("");

	for(int i=0; i<5 ; i++)
	{
		str = m_ColGrid->GetValueRowCol(i+2, 1);
		m_pColmData->iBarNum[0][0][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 2);
		m_pColmData->strBarNa1[0][0][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 3);
		m_pColmData->strBarNa2[0][0][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 4);
		m_pColmData->iBarNum[0][1][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 5);
		m_pColmData->strBarNa1[0][1][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 6);
		m_pColmData->strBarNa2[0][1][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 7);
		m_pColmData->iBarNum[0][2][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 8);
		m_pColmData->strBarNa1[0][2][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 9);
		m_pColmData->strBarNa2[0][2][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 10);
		m_pColmData->dDc[0][i] = _tstof(str);
	}
}

void CDgnConColmBarDlgCIVIL::UpdateViewWnd()
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	// 일단 0 으로 고정해두었다~	

	T_RCHK_D RchkData; RchkData.Initialize();
	RchkData.nType = 1;

	CString str = _T("");

	for(int i=0; i<m_nLayerNum ; i++)
	{
		str = m_ColGrid->GetValueRowCol(i+2, 1);
		RchkData.COLM.iBarNum[0][0][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 2);
		RchkData.COLM.strBarNa1[0][0][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 3);
		RchkData.COLM.strBarNa2[0][0][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 4);
		RchkData.COLM.iBarNum[0][1][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 5);
		RchkData.COLM.strBarNa1[0][1][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 6);
		RchkData.COLM.strBarNa2[0][1][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 7);
		RchkData.COLM.iBarNum[0][2][i] = _ttoi(str);
		str = m_ColGrid->GetValueRowCol(i+2, 8);
		RchkData.COLM.strBarNa1[0][2][i] = str;
		str = m_ColGrid->GetValueRowCol(i+2, 9);
		RchkData.COLM.strBarNa2[0][2][i] = str;

		str = m_ColGrid->GetValueRowCol(i+2, 10);
		RchkData.COLM.dDc[0][i] = _tstof(str);
	}
	m_SectWnd.Draw_Shape(1, m_RchkKey, FALSE, TRUE);
	m_SectWnd.Draw_ChkRebar(m_RchkKey, 0, RchkData, FALSE, TRUE);
	m_SectWnd.Invalidate();
}


void CDgnConColmBarDlgCIVIL::OnRedrawBtn() 
{
	if(CheckInputData()==TRUE) UpdateViewWnd();	
}

void CDgnConColmBarDlgCIVIL::UpdateAsInfo(double dAsc)
{
	m_As.SetEditUnit(dAsc);
}
