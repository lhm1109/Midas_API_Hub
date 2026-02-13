// DgnLateralCapaResultDlg.cpp: implementation of the CDgnLateralCapaResultDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnLateralCapaResultDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_dbLock\LockCtrl.h"

#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\MSOffice.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "DGN_DgnPrint.h"
#include "DgnToolMPhi_SeismicDraw.h"
#include "DgnToolMPhi_SeismicReport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnLateralCapaResultDlg::CDgnLateralCapaResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnLateralCapaResultDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();	
	m_ResData.Initialize();
}

CDgnLateralCapaResultDlg::~CDgnLateralCapaResultDlg()
{
}

void CDgnLateralCapaResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnLateralCapaResultDlg)
	DDX_Control(pDX, IDC_RLCC_RES_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnLateralCapaResultDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnLateralCapaResultDlg)	
	ON_BN_CLICKED(IDC_DGN_SELECT_ALL_BTN,   OnSelectAll)
	ON_BN_CLICKED(IDC_DGN_UNSELECT_ALL_BTN, OnUnSelectAll)
	ON_BN_CLICKED(IDC_RLCC_RES_REPORT_BTN,  OnWordReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaResultDlg message handlers
BOOL CDgnLateralCapaResultDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	
	// TODO: Add extra initialization here	
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);
	m_wndGrid.EnableMouseWheel();
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	EnableCtrls();
	Write_ResultData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnLateralCapaResultDlg::EnableCtrls()
{
	GetDlgItem(IDC_RLCC_RES_REPORT_BTN)->EnableWindow(FALSE); // hslee : 재대로된 word 보고서 개발 전 까지 기능 자체를 막기로 함
}

void CDgnLateralCapaResultDlg::OnSelectAll()
{
	/////////////////////////////
	// Set Grid Editable Status
	m_wndGrid.GetParam()->EnableUndo(FALSE);
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.GetParam()->SetLockReadOnly(FALSE);
	/////////////////////////////
	
	int nGridCount = m_wndGrid.GetRowCount();
	int nPosiSize = m_arRowPosi.GetSize();
	
	for(int i=0 ; i<nPosiSize ; i++)
	{
		if(m_arRowPosi[i] < 0) continue;
		if(m_arRowPosi[i] <= nGridCount)
		{
			m_wndGrid.SetValueRange(CGXRange(m_arRowPosi[i],D_LCR_COL_SEL),_T("1"),gxOverride,0,GX_UPDATENOW);			
		}
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	m_wndGrid.GetParam()->SetLockReadOnly(TRUE);
	m_wndGrid.GetParam()->EnableUndo(TRUE);	
}

void CDgnLateralCapaResultDlg::OnUnSelectAll()
{	
	/////////////////////////////
	// Set Grid Editable Status
	m_wndGrid.GetParam()->EnableUndo(FALSE);
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.GetParam()->SetLockReadOnly(FALSE);
	/////////////////////////////
	
	int nGridCount = m_wndGrid.GetRowCount();
	int nPosiSize = m_arRowPosi.GetSize();
	
	for(int i=0 ; i<nPosiSize ; i++)
	{
		if(m_arRowPosi[i] < 0) continue;
		if(m_arRowPosi[i] < nGridCount)
		{
			m_wndGrid.SetValueRange(CGXRange(m_arRowPosi[i],D_LCR_COL_SEL),_T("0"),gxOverride,0,GX_UPDATENOW);			
		}
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	m_wndGrid.GetParam()->SetLockReadOnly(TRUE);
	m_wndGrid.GetParam()->EnableUndo(TRUE);	
}

void CDgnLateralCapaResultDlg::OnWordReport()
{
#if defined(_JP)
	if(!m_pDoc->AllowCommand(D_OPTN_ID_ULTILAT_FRC)) 
	{
		AfxMessageBox(_LS(IDS_DB_NO_LICENSE_ULTILAT_FRC));
		return;    
	}
#endif

	CArray<BOOL, BOOL> arChk;
	int nSize = m_ResData.List.GetSize();
	int nPosiSize = m_arRowPosi.GetSize();
	arChk.SetSize(nSize);	
	for(int i=0 ; i<nSize ; i++)
	{
		if(i >= nPosiSize)          arChk[i] = FALSE;
		else if(m_arRowPosi[i] < 0) arChk[i] = FALSE;
		else 
		{
			arChk[i] = (1==_ttoi(m_wndGrid.GetValueRowCol(m_arRowPosi[i],D_LCR_COL_SEL)));			
		}
	}

	BOOL bChk = FALSE;
	for(int i=0 ; i<nSize ; i++)
	{ 
		if(arChk[i]) { bChk = TRUE;  break; } 
	}

	if(!bChk) 
	{
		CString strMsg = _LS(IDS_DGN_RLC_WORDPRINT_CHECK);
		GClearHistory();
		GSaveHistoryFormatNF(strMsg);		
		return;
	}

    bool bRTF = CDBLib::IsWordRTF();
    CString strExt    = CMSOffice::GetExtensionName(bRTF ? EXT_WORDRTF : EXT_WORD);
	CString strDotExt = _T(".") + strExt;
	CString strFileFormat = _T("");
	strFileFormat.Format(_T("EXCEL File(*.%s)|*.%s|All File(*.*)|*.*||"), strExt, strExt);

	CString csFileName = CDBLib::MakeFileNameWithNewExt(strDotExt);
	LPCTSTR szFileName;
	if (csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;

	CFileDialog fd(FALSE, strDotExt, szFileName,
								 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
								 strFileFormat, NULL); 

	if (fd.DoModal() == IDOK)
	{    
		CString strFileFullName = fd.GetPathName();

		CDgnToolMPhi_SeismicDraw DgnDraw;
		CDgnToolMPhi_SeismicReport DgnReport;

		DgnDraw.MakeLateralCapaFig(m_ResData);		
	
		CDgnProgressDlg ProDlg;
		ProDlg.Create_ProgressDlg(PROGRESS_TYPE_RLC_ANALYSIS_RPT, this);

		DgnReport.PrintReport(strFileFullName, arChk, m_ResData, &ProDlg);
		DgnDraw.DeleteLateralCapaFig(m_ResData);

		ProDlg.Delete_ProgressDlg();

	}
}

void CDgnLateralCapaResultDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	//DestroyWindow();
	CDialogMove::OnCancel();
}

void CDgnLateralCapaResultDlg::SetResData(_DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	m_ResData = ResData;
}

void CDgnLateralCapaResultDlg::Write_ResultData()
{
	T_RLCO_D RlcoD;
	if(!m_pDoc->m_pAttrCtrl->GetRlco(RlcoD)) m_pDoc->m_pAttrCtrl->GetDefaultRlco(RlcoD);
	int i, j;
	int nSize = m_ResData.List.GetSize();
	int iTotalCol = D_LCR_COL_NUM;
	int iTotalRow = 0;

	int nEqTypeCount = 0;
	if(RlcoD.bEarthquakeTypeI)  nEqTypeCount++;
	if(RlcoD.bEarthquakeTypeII) nEqTypeCount++;

	m_arRowPosi.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		m_arRowPosi[i] = -1;
		if(!m_ResData.List[i].bIsRes) continue;
		if(m_ResData.List[i].bTraverse)  iTotalRow += nEqTypeCount;
		if(m_ResData.List[i].bLongitude) iTotalRow += nEqTypeCount;		
	}

	CString title = _T("");
	CDGN_DgnPrint PrintObject;

	/////////////////////////////
	// Set Grid Editable Status
	m_wndGrid.GetParam()->EnableUndo(FALSE);
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.GetParam()->SetLockReadOnly(FALSE);
	/////////////////////////////
	
	m_wndGrid.SetRowCount(iTotalRow);
	m_wndGrid.SetColCount(iTotalCol);
	
	Set_Title();
	
	for(i=1; i<=iTotalRow; i++)
	{
		m_wndGrid.SetRowHeight(i,0, globalUtils.ScaleByDPI(18),NULL,GX_UPDATENOW);
	}

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(8));
	
	double dFfor = 1.0/CUnitCtrl::ConvValueTgtUnit2CurUnit(m_ResData.DataUnit.nBase_Force, m_ResData.DataUnit.nBase_Length,D_UNITSYS_BASE_FORCE, 1.0); 
	double dFlen = 1.0/CUnitCtrl::ConvValueTgtUnit2CurUnit(m_ResData.DataUnit.nBase_Force, m_ResData.DataUnit.nBase_Length,D_UNITSYS_BASE_LENGTH, 1.0);

	int iRowCount = 1;
	int iRowMerge = 0;
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES MembUnitRes;
	BOOL bFistMemb, bFistType1, bFistType2;
	for(i=0 ; i<nSize ; i++)
	{
		if(!m_ResData.List[i].bIsRes) continue;
		
		style.SetTextColor(RGB(0,0,0));

		iRowMerge = 0;
		if(m_ResData.List[i].bTraverse)  iRowMerge += nEqTypeCount;
		if(m_ResData.List[i].bLongitude) iRowMerge += nEqTypeCount;		

		if(iRowMerge > 1)
		{
			m_wndGrid.SetCoveredCellsRowCol(iRowCount,D_LCR_COL_SEL,iRowCount+iRowMerge-1,D_LCR_COL_SEL);			
			m_wndGrid.SetCoveredCellsRowCol(iRowCount,D_LCR_COL_NAME,iRowCount+iRowMerge-1,D_LCR_COL_NAME);			
			m_wndGrid.SetCoveredCellsRowCol(iRowCount,D_LCR_COL_ELEM,iRowCount+iRowMerge-1,D_LCR_COL_ELEM);
		}
		if(m_ResData.List[i].bTraverse && m_ResData.List[i].bLongitude)
		{			
			if(RlcoD.bEarthquakeTypeI || RlcoD.bEarthquakeTypeII)
				m_wndGrid.SetCoveredCellsRowCol(iRowCount  ,D_LCR_COL_EQTYPE,iRowCount+1,D_LCR_COL_EQTYPE);			
			if(RlcoD.bEarthquakeTypeI && RlcoD.bEarthquakeTypeII)
				m_wndGrid.SetCoveredCellsRowCol(iRowCount+2,D_LCR_COL_EQTYPE,iRowCount+3,D_LCR_COL_EQTYPE);			
		}
		

		bFistMemb = bFistType1 = bFistType2 = TRUE;
		for(j=0 ; j<4 ; j++)
		{
			if(j==0) 
			{
				if(!RlcoD.bEarthquakeTypeI || !m_ResData.List[i].bTraverse) continue;
				MembUnitRes = m_ResData.List[i].ResType1Trav;
			}
			else if(j==1) 
			{
				if(!RlcoD.bEarthquakeTypeI || !m_ResData.List[i].bLongitude) continue;
				MembUnitRes = m_ResData.List[i].ResType1Long;
			}
			else if(j==2) 
			{
				if(!RlcoD.bEarthquakeTypeII || !m_ResData.List[i].bTraverse) continue;
				MembUnitRes = m_ResData.List[i].ResType2Trav;
			}
			else if(j==3) 
			{
				if(!RlcoD.bEarthquakeTypeII || !m_ResData.List[i].bLongitude) continue;
				MembUnitRes = m_ResData.List[i].ResType2Long;
			}

			if(bFistMemb)
			{				
				m_arRowPosi[i] = iRowCount;
				style.SetValue(_T(""));
				style.SetControl(GX_IDS_CTRL_CHECKBOX3D);
				style.SetChoiceList(_T(""));		  
				m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_SEL),style);

				style.SetControl(GX_IDS_CTRL_STATIC);

				title = m_ResData.List[i].strName;
				style.SetValue(title);
				m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_NAME),style);				
				title = GetStrElemList(m_ResData.List[i].KeyList);
				style.SetValue(title);
				m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_ELEM),style);		
				
				bFistMemb = FALSE;
			}
			// 지진동 Type
			if((j==0 || j==1) && bFistType1)
			{
				title = _T("Type I");
				style.SetValue(title);
				m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_EQTYPE),style);
				bFistType1 = FALSE;
			}
			if((j==2 || j==3) && bFistType2)
			{
				title = _T("Type II");
				style.SetValue(title);
				m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_EQTYPE),style);
				bFistType2 = FALSE;
			}
			//해석방향
			if(j==0 || j==2) 
			{
				title = _LS(IDS_DGN_RLC_GRID_TEXT_TRAV);
				style.SetValue(title);
				m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_DIR),style);
			}
			if(j==1 || j==3) 
			{
				title = _LS(IDS_DGN_RLC_GRID_TEXT_LONG);
				style.SetValue(title);
				m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_DIR),style);
			}
			//Pc
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(MembUnitRes.dPc, dFfor));
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_PC),style);
			//Pu
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(MembUnitRes.dPu, dFfor));
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_PU),style);
			//Ps
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(MembUnitRes.dPs, dFfor));
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_PS),style);
			//Ps0
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(MembUnitRes.dPs0, dFfor));
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_PS0),style);
			//파괴형태
			title = Get_FailureName(MembUnitRes.nFailureType);
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_DECISION),style);
			//Pa
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(MembUnitRes.dPa, dFfor));
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_PA),style);
			//khc*w
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(MembUnitRes.dKhc*MembUnitRes.dW, dFfor));
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_KHCW),style);			
			//판정
			if(MembUnitRes.bCapaChk)
			{
				title = _T("OK");  
				style.SetTextColor(RGB(0,0,200)); 
			}
			else 
			{ 
				title = _T("NG");
				style.SetTextColor(RGB(255,0,0));
			}
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_DECISION2),style);
			style.SetTextColor(RGB(0,0,0));
			
			int nLimState = GetLimStateAtElem(m_ResData.List[i].KeyList);
			//δRa
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(m_ResData.List[i].ddeltaRa, dFlen));
			if(nLimState == 0) title = _T("-");
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_DELTA_RA),style);
			//δR
			title.Format(_T("%s"),PrintObject.Get_6_1e_S1(MembUnitRes.ddeltaR, dFlen));
			if(nLimState == 0) title = _T("-");
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_DELTA_R),style);								
			//판정
			if(nLimState == 0)            { title = _T("-");  style.SetTextColor(RGB(0,0,0)); }
			else if(MembUnitRes.bDisChk)  { title = _T("OK");  style.SetTextColor(RGB(0,0,200)); }
			else                          { title = _T("NG");  style.SetTextColor(RGB(255,0,0)); }
			
			style.SetValue(title);
			m_wndGrid.SetStyleRange(CGXRange(iRowCount,D_LCR_COL_DECISION3),style);
			style.SetTextColor(RGB(0,0,0));
			style.SetEnabled(TRUE);
			iRowCount++;
		}		
	}
	
	/////////////////////////////////////////////////////////////////////////
	//	지우지 못하게 하자.
	m_wndGrid.SetStyleRange(CGXRange(0,0,iTotalRow,0),CGXStyle().SetReadOnly(TRUE));
	m_wndGrid.SetStyleRange(CGXRange(0,D_LCR_COL_NAME,iTotalRow,iTotalCol),CGXStyle().SetReadOnly(TRUE));
	
	m_wndGrid.GetParam()->EnableTrackRowHeight(FALSE);
	//m_wndGrid.GetParam()->EnableTrackColWidth(FALSE);
	
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);
	// Change by ZINU.('04.09.10). To Copy & Paste EnableSelection(FALSE -> TRUE).
	m_wndGrid.GetParam()->EnableSelection(TRUE);
	
	m_wndGrid.SetFrozenRows(0,0,GX_UPDATENOW,gxDo);
	//																		//
	//////////////////////////////////////////////////////////////////////////

	//if(RlcoD.nBridgeDgnLevel == 0) m_wndGrid.HideCols(D_LCR_COL_DELTA_RA,D_LCR_COL_DECISION3);
	
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
	m_wndGrid.GetParam()->SetLockReadOnly(TRUE);
	m_wndGrid.GetParam()->EnableUndo(TRUE);  
}

int CDgnLateralCapaResultDlg::GetLimStateAtElem(CArray<UINT, UINT>& aElemK)
{  
	if(aElemK.GetSize() == 0)
	{
		ASSERT(0);
		return 0;
	}

	T_ELEM_D ElemD;
	if(!m_pDoc->m_pAttrCtrl->GetElem(aElemK[0], ElemD)) { ASSERT(0); ElemD.Initialize(); }

	T_MPHG_D MphgD;
	if(!m_pDoc->m_pAttrCtrl->GetMphg(ElemD.elmat, MphgD)) { ASSERT(0); MphgD.Initialize(); }

	return MphgD.nLimitState;
}

void CDgnLateralCapaResultDlg::Set_Title()
{
	CString Temp;
	CString arGridTitle[D_LCR_COL_NUM + 1];
	arGridTitle[ 0] = _T("");
	arGridTitle[D_LCR_COL_SEL] = _LS(IDS_DGN_LIST_SEL);       //_T("SEL")
	arGridTitle[D_LCR_COL_NAME] = _LS(IDS_DGN_NAME);           //_T("NAME")
	arGridTitle[D_LCR_COL_ELEM] = _LS(IDS_DGN_CHK_ELEM_LIST);  //_T("Element List") 
	arGridTitle[D_LCR_COL_EQTYPE] = _LS(IDS_DGN_RLC_GRID_TEXT_EQ_TYPE); //_T("지지동\nType")
	arGridTitle[D_LCR_COL_DIR] = _LS(IDS_DGN_RLC_GRID_TEXT_DIR); //_T("해석방향")
	arGridTitle[D_LCR_COL_PC] = _LS(IDS_DGN_RLC_GRID_TEXT_PC); //Pc
	arGridTitle[D_LCR_COL_PU] = _LS(IDS_DGN_RLC_GRID_TEXT_PU); //Pu
	arGridTitle[D_LCR_COL_PS] = _LS(IDS_DGN_RLC_GRID_TEXT_PS); //Ps
	arGridTitle[D_LCR_COL_PS0] = _LS(IDS_DGN_RLC_GRID_TEXT_PS0); //Ps0
	arGridTitle[D_LCR_COL_DECISION] = _LS(IDS_DGN_RLC_DECISION); //_T("파괴형태")
	arGridTitle[D_LCR_COL_PA] = _T("Pa");
	arGridTitle[D_LCR_COL_KHCW] = _T("khc*W");	
	arGridTitle[D_LCR_COL_DECISION2] = _LS(IDS_DGN_RLC_DECISION); //_T("판정")
	arGridTitle[D_LCR_COL_DELTA_RA].Format(_T("%s Ra"), _LS(IDS_DGN_CHAR_DELTA));
	arGridTitle[D_LCR_COL_DELTA_R].Format(_T("%s R"), _LS(IDS_DGN_CHAR_DELTA));
	arGridTitle[D_LCR_COL_DECISION3] = _LS(IDS_DGN_RLC_DECISION); //_T("판정")
							
	int arGridWidth[D_LCR_COL_NUM + 1] = {0, 35, 80, 80, 60, 75, 75, 75, 75, 75, 75, 60, 60, 60, 60, 60, 60};
	// Add by ZINU.('00.10.28).
	DWORD dwColor1 = GetSysColor(COLOR_3DFACE);
	DWORD dwColor2 = GetSysColor(COLOR_3DHILIGHT);
	COLORREF Red	 = (GetRValue(dwColor1)+GetRValue(dwColor2))/2;
	COLORREF Green = (GetGValue(dwColor1)+GetRValue(dwColor2))/2;
	COLORREF Blue	 = (GetBValue(dwColor1)+GetRValue(dwColor2))/2;
	DWORD dwColor3 = RGB(Red,Green,Blue);

	int i;
	CString title = _T("");
	m_wndGrid.SetRowHeight(0,0, globalUtils.ScaleByDPI(35),NULL,GX_UPDATENOW);
	
	//////////////////////////////////////////////////////////////////////////////////////
	//	ResultView의 Title를 입력하는 곳												//
	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	// Change by ZINU.('00.10.28).
	style.SetInterior(dwColor1);	// RGB(192,192,192) -> GetSysColor(...).
	// Show Sub Title.
	for(i=0 ; i<D_LCR_COL_NUM + 1 ; i++)
	{	
		title = arGridTitle[i];
		style.SetValue(title);
		m_wndGrid.SetStyleRange(CGXRange(0,i),style);	
		m_wndGrid.SetColWidth(i,i,globalUtils.ScaleByDPI(arGridWidth[i]), nullptr,GX_UPDATENOW);
	}
}


CString CDgnLateralCapaResultDlg::GetStrElemList(CArray<UINT, UINT>& KeyList)
{	
	int nSize = KeyList.GetSize();
	long *aNum = new long[nSize];
	for (int i = 0; i < nSize; i++)
		aNum[i] = (long)(KeyList[i]);
	qsort((void*)aNum, nSize, sizeof(long), CNumericOptimizer::comparei);

	// Optimize한다.
	CNumericOptimizer optimizer;
	CString sOptimizedString = optimizer.Optimize(aNum, nSize);
	
	delete[] aNum;
	
	return sOptimizedString;
}

CString CDgnLateralCapaResultDlg::Get_FailureName(int nType)
{
	if(nType == 0) return _LS(IDS_DGN_RLC_GRID_TEXT_MOMENTFAILURE);
	if(nType == 1) return _LS(IDS_DGN_RLC_GRID_TEXT_MOMSHEARFAILURE);
	if(nType == 2) return _LS(IDS_DGN_RLC_GRID_TEXT_SHEARFAILURE);
	ASSERT(0);

	return _T("");
}
