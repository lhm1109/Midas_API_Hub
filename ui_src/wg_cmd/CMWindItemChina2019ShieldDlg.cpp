//CMWindItemChina2019ShieldDlg
// Add by maxiao 2011-5-10  For china GB 50009-2011

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemChina2019ShieldDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_base\wg_base_MsgDll.h"


//#include "ShapeGrid.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMWindItemChina2019ShieldDlg dialog
CMWindItemChina2019ShieldDlg::CMWindItemChina2019ShieldDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMWindItemChina2019ShieldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMWindItemChina2019ShieldDlg)  
	//}}AFX_DATA_INIT
}

void CMWindItemChina2019ShieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMWindItemChina2019ShieldDlg)  
	DDX_Control(pDX, IDC_CMD_GRID_SHIELD, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMWindItemChina2019ShieldDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMWindItemChina2019ShieldDlg)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// Implementations
//////////////////////////////////////////////////////////////////////////////
BOOL CMWindItemChina2019ShieldDlg::OnInitDialog()//(CArray<T_CHINA_WARD, T_CHINA_WARD&>& arDataList) 
{
	CDialogMove::OnInitDialog();
	// TODO: Add extra initialization here
    InitGrid();
    Data2Grid();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMWindItemChina2019ShieldDlg::OnOK() 
{
	// TODO: Add extra validation here
    if (!Grid2Data())
    {
        return;
    }
	CDialogMove::OnOK();
}
void CMWindItemChina2019ShieldDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// For Grid Window

void CMWindItemChina2019ShieldDlg::InitGrid()
{
    m_wndGrid.Initialize();

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
    // Excel­c ¾b? É« «æ? : DoLButtonDblClk °Ò?
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
    
    m_wndGrid.SetColWidthByDPI(0, 0, 60);
    m_wndGrid.SetColWidthByDPI(1, 1, 150);
    m_wndGrid.SetColWidthByDPI(2, 2, 150);

    m_wndGrid.LockUpdate(bOldLock);
    if (!bOldLock) m_wndGrid.Redraw();
}

void CMWindItemChina2019ShieldDlg::SetGridTitle()
{
    BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

    m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_CMD_WIND_CHINA_STOR));
    m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD_WIND_CHINA_X_COEFF));
    m_wndGrid.SetValueRange(CGXRange(0, 2), _LS(IDS_CMD_WIND_CHINA_Y_COEFF));
    m_wndGrid.LockUpdate(bOldLock);

    if (!bOldLock) m_wndGrid.Redraw();
}

// Data -> Grid
void CMWindItemChina2019ShieldDlg::Data2Grid()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc);

    CArray<T_STOR_K, T_STOR_K> aStorK;
    pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
    int nStorNum = aStorK.GetSize();

    m_wndGrid.SetRowCount(nStorNum);
    if (nStorNum == 0) return;

    T_STOR_D StorD;
    CMap<T_STOR_K, T_STOR_K, int, int> mapWARDtor;
    mapWARDtor.InitHashTable(HASHSIZESTOR);
    mapWARDtor.RemoveAll();  

    double dFactorX = 0., dFactorY = 0.;
    for(int i = nStorNum - 1; i >= 0; i--)
    {
        pDoc->m_pAttrCtrl->GetStor(aStorK[i], StorD);      
        m_wndGrid.SetValueRange(CGXRange(nStorNum - i, 0), StorD.StoryName);
        m_wndGrid.SetValueRange(CGXRange(nStorNum - i, 1), StorD.dShieldCoefX);
        m_wndGrid.SetValueRange(CGXRange(nStorNum - i, 2), StorD.dShieldCoefY);		
    }
	m_wndGrid.SetValueRange(CGXRange(nStorNum, 1), _T("-"));
	m_wndGrid.SetValueRange(CGXRange(nStorNum, 2), _T("-"));
	m_wndGrid.SetStyleRange(CGXRange(nStorNum, 1), CGXStyle().SetEnabled(FALSE));
	m_wndGrid.SetStyleRange(CGXRange(nStorNum, 2), CGXStyle().SetEnabled(FALSE));
}

// Grid -> Data
BOOL CMWindItemChina2019ShieldDlg::Grid2Data()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    ASSERT(pDoc);
    int error;
    CString str, storName;
    // ??½Y ?Íù?? ¤½?¥¶ °s?ºN?½Y
    int nNum = m_wndGrid.GetRowCount();
    for (int nRow=1; nRow<nNum; nRow++)
    {    
        storName = m_wndGrid.GetValueRowCol(nRow, 0);
        int  KeyStor = pDoc->m_pAttrCtrl->GetStorKey(storName);
        
		T_STOR_D StorD;StorD.Initialize();
        pDoc->m_pAttrCtrl->GetStor(KeyStor, StorD);      

        str = m_wndGrid.GetValueRowCol(nRow, 1);
        if (!m_wndGrid.IsValidDoubleNumber(str, error)) 
            return m_wndGrid.InvalidDoubleNumber(error);
		StorD.dShieldCoefX = _tstof(str);

        str = m_wndGrid.GetValueRowCol(nRow, 2);
        if (!m_wndGrid.IsValidDoubleNumber(str, error)) 
            return m_wndGrid.InvalidDoubleNumber(error);
		StorD.dShieldCoefY = _tstof(str);
		pDoc->m_pDataCtrl->ModifyStor(storName, StorD);
    }
    return TRUE;
}

