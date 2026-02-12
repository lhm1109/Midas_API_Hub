// SeisItemJapan2000Sub.cpp : implementation file
//
// 2004. 01. 26   by TAE
// 밑면전단력 분담계수(Ai)를 User 임의값으로 입력받는 Dialog
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisItemJapan2000Sub.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_QSort.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#include "..\wg_Common\wg_common_TBGrid.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisItemJapan2000Sub dialog


CSeisItemJapan2000Sub::CSeisItemJapan2000Sub(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisItemJapan2000Sub::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisItemJapan2000Sub)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dTX = 0.;
	m_dTY = 0.; 
	m_pData = NULL;
	m_bInit = FALSE;
}

void CSeisItemJapan2000Sub::SetData(double dTX, double dTY, 
																		CArray<T_SEIS_JIS_AI_BASE, T_SEIS_JIS_AI_BASE&>* pData, BOOL bInit)
{ 
	ASSERT(pData); 
	m_dTX = dTX;
	m_dTY = dTY; 
	m_pData = pData; 
	m_bInit = bInit; 
}

void CSeisItemJapan2000Sub::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisItemJapan2000Sub)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_SP_SD_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisItemJapan2000Sub, CDialogMove)
	//{{AFX_MSG_MAP(CSeisItemJapan2000Sub)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisItemJapan2000Sub message handlers

BOOL CSeisItemJapan2000Sub::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGrid();

	if (m_bInit) CalcDefaultData();
	
	Data2Grid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisItemJapan2000Sub::OnOK() 
{
	// TODO: Add extra validation here
	CArray<T_SEIS_JIS_AI_BASE, T_SEIS_JIS_AI_BASE&> OldData;
	OldData.Copy(*m_pData);

	if (!Grid2Data())
	{
		m_pData->Copy(OldData);
		return;
	}

	CDialogMove::OnOK();
}

void CSeisItemJapan2000Sub::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}


/////////////////////////////////////////////////////////////////////////////
// For Grid Window

void CSeisItemJapan2000Sub::InitGrid()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	CGXGridParam* pParam = m_wndGrid.GetParam();
	ASSERT_VALID(pParam);

	CGXProperties* pProp = pParam->GetProperties();
	ASSERT_VALID(pProp);

	pParam->EnableUndo(FALSE);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	// Row and column headers
	m_wndGrid.ColHeaderStyle().SetReadOnly(FALSE).SetEnabled(FALSE)
														.SetFont(CGXFont_GC().SetBold(FALSE));
	m_wndGrid.RowHeaderStyle().SetEnabled(FALSE);
	m_wndGrid.ChangeStandardStyle(CGXStyle().SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetAllowEnter(FALSE));

	// Special grid settings
	//pParam->EnableMoveRows(TRUE);                  // rows cannot be dragged
	//pParam->EnableMoveCols(FALSE);                 // columns can be dragged
	pParam->EnableTrackColWidth(FALSE);             // disable resizing
	pParam->EnableTrackRowHeight(FALSE);
	pParam->EnableThumbTrack(FALSE);               // thumb-tracking is disabled
	pParam->SetNumberedColHeaders(FALSE);
	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	pParam->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	pParam->SetHideCurrentCell(GX_HIDE_NEVER);

	// removing rows is not undoable for this kind of grid
	pParam->SetRemoveColsFlags(FALSE);
	pParam->SetRemoveRowsFlags(FALSE);

	pProp->SetMarkColHeader(FALSE);    // Turn off pressed button effect for column headers
	pProp->SetPrintRowHeaders(FALSE);  // Don't print column headers

	m_wndGrid.SetRowHeightByDPI(0, 0, 20);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(2);
	
	m_wndGrid.SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));

	SetGridTitle();
	
	m_wndGrid.SetColWidthByDPI(0, 0, 65);
	m_wndGrid.SetColWidthByDPI(1, 1, 65);
	m_wndGrid.SetColWidthByDPI(2, 2, 65);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CSeisItemJapan2000Sub::SetGridTitle()
{
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__Story));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__X_Dir));
	m_wndGrid.SetValueRange(CGXRange(0, 2), _LS(IDS_WG_CMD__Y_Dir));
	m_wndGrid.LockUpdate(bOldLock);

	if (!bOldLock) m_wndGrid.Redraw();
}

// Cacluate default Ai
void CSeisItemJapan2000Sub::CalcDefaultData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CArray<T_STOR_K, T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();

	CArray<double, double> aAi_X, aAi_Y;
	CLateralLoad load;
	load.GetSeismicLoad_JIS_Ai(m_dTX, m_dTY, &aAi_X, &aAi_Y);
	ASSERT(aAi_X.GetSize() == aAi_Y.GetSize());
	int nUpperStorNum = aAi_X.GetSize();

	T_SEIS_JIS_AI_BASE data;
	m_pData->SetSize(nStorNum);
	for (int i=0; i<nStorNum; i++)
	{
		data.KeyStor = aStorK[i];
		data.dAiX = (i < nUpperStorNum) ? aAi_X[i] : 0.0;
		data.dAiY = (i < nUpperStorNum) ? aAi_Y[i] : 0.0;
		m_pData->SetAt(i, data); // 아래층부터 저장 
	}
}

// Data -> Grid
void CSeisItemJapan2000Sub::Data2Grid()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CArray<T_STOR_K, T_STOR_K> aStorK;
	pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();

	double dBaseLevel = 0.;
	T_BLDC_D BldcD;	BldcD.Initialize();
	pDoc->m_pAttrCtrl->GetBldc(BldcD);

	if(pDoc->m_pAttrCtrl->IsUseBedrockLevel())
		dBaseLevel = BldcD.dBedrockLevel;
	else if(BldcD.bUseBaseLevel)
		dBaseLevel = BldcD.dBaseLevel;

	int nNum = m_pData->GetSize();
	m_wndGrid.SetRowCount(nStorNum);
	if (nStorNum == 0) return;

	T_STOR_D StorD;
	T_SEIS_JIS_AI_BASE* pBaseData;
	CMap<T_STOR_K, T_STOR_K, int, int> mapAiStor;
	mapAiStor.InitHashTable(HASHSIZESTOR);
	mapAiStor.RemoveAll();  
	for(int i = 0; i < nNum; i++)
	{
		pBaseData = &m_pData->GetAt(i);
		mapAiStor.SetAt(pBaseData->KeyStor, i);   
	}

	double dAiX = 0., dAiY = 0.;
	int nIndex;
	for(int i = nStorNum - 1; i >= 0; i--)
	{
		pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);      
		if(mapAiStor.Lookup(aStorK[i], nIndex))   // 기존에 입력된 Ai가 있는 층은 입력된 값 출력
		{
			pBaseData = &m_pData->GetAt(nIndex);
			dAiX = pBaseData->dAiX;
			dAiY = pBaseData->dAiY;
		}
		else  // 새로 추가된 층은 Ai를 0으로 설정
		{
			dAiX = 0.;
			dAiY = 0.;
		}    
		m_wndGrid.SetValueRange(CGXRange(nStorNum - i, 0), StorD.StoryName);
		m_wndGrid.SetValueRange(CGXRange(nStorNum - i, 1), dAiX);
		m_wndGrid.SetValueRange(CGXRange(nStorNum - i, 2), dAiY);

		if (StorD.dStoryLevel <= dBaseLevel)
		{
			m_wndGrid.SetStyleRange(CGXRange(nStorNum - i, 1, nStorNum - i, 2), CGXStyle()
				.SetEnabled(FALSE)
				.SetValue(_T("0.0000")));
		}
	}
}

// Grid -> Data
BOOL CSeisItemJapan2000Sub::Grid2Data()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	int error;
	CString value;
	T_SEIS_JIS_AI_BASE BaseD;

	// 데이터 저장시는 무조건 아랫층부터
	int nNum = m_wndGrid.GetRowCount();
	m_pData->SetSize(nNum);
	for (int nRow=1; nRow<=nNum; nRow++)
	{    
		value = m_wndGrid.GetValueRowCol(nRow, 0);
		BaseD.KeyStor = pDoc->m_pAttrCtrl->GetStorKey(value);
		
		value = m_wndGrid.GetValueRowCol(nRow, 1);
		if (!m_wndGrid.IsValidDoubleNumber(value, error)) 
			return m_wndGrid.InvalidDoubleNumber(error);
		BaseD.dAiX = _tstof(value);

		value = m_wndGrid.GetValueRowCol(nRow, 2);
		if (!m_wndGrid.IsValidDoubleNumber(value, error)) 
			return m_wndGrid.InvalidDoubleNumber(error);
		BaseD.dAiY = _tstof(value);

		m_pData->SetAt(nNum-nRow, BaseD);
	}
	return TRUE;
}

