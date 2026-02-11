// CMSdvePropDlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdvePropDlg.h"
#include "CMSdvePropGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_unitctrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\MIT_lib\IMSGInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSdvePropDlg dialog
CCMSdvePropDlg::CCMSdvePropDlg(CWnd* pParent)
	: CDialogMove(CCMSdvePropDlg::IDD, pParent)
{
	m_wndGrid = new CCMSdvePropGrid();
	memset(m_dValue,0,sizeof(m_dValue));
}

CCMSdvePropDlg::~CCMSdvePropDlg()
{
	if(NULL != m_wndGrid) delete m_wndGrid ;
	m_wndGrid = NULL;
}

BEGIN_MESSAGE_MAP(CCMSdvePropDlg, CDialogMove)
//{{AFX_MSG_MAP(CCMSdvePropDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMSdvePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdvePropDlg)
	DDX_Control(pDX, IDC_ETC_SDVE_PROP_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}

BOOL CCMSdvePropDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_wndGrid->Initialize();
	m_wndGrid->LockUpdate(TRUE); // 설정중 화면 update를 막는다.
	m_wndGrid->GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.
	m_wndGrid->SetRowCount(20);
	m_wndGrid->SetColCount(1); 

	if(m_nMatlType==D_SDVE_CST)
		this->SetWindowText(_LS(IDS_ETC_SDVE_TITLE_CST_PROP));

	SetTitleInGrid();
	SetDataInGrid();  

	// Grid의 style을 변경한다.
	m_wndGrid->ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetFaceName(_LS(IDS_CMD_GRID_FONT)).SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// column 크기를 맞춘다.
	int nLang = AfxGetApp()->GetProfileInt(_T("LANGUAGE"), _T("LANGUAGE"), 0);
    switch ( nLang )
    {
    case _LANGUAGE_JP_:
    case _LANGUAGE_CH_:
        {
            // 일문,중문인 경우, 텍스트가 짧아서..
            m_wndGrid->SetColWidth(0, 0, globalUtils.ScaleByDPI(230));
            CRect rDlg, rGrid, rClose;
            this->GetWindowRect(rDlg);
            GetDlgItem(IDC_ETC_SDVE_PROP_GRID)->GetWindowRect(rGrid);
            GetDlgItem(IDCANCEL)->GetWindowRect(rClose);
            int n = 135;
            rDlg.right   -= n;
            rGrid.right  -= n;
            rClose.left  -= n;
            rClose.right -= n;
            this->MoveWindow(rDlg);
            ScreenToClient(rGrid);
            ScreenToClient(rClose);
            GetDlgItem(IDC_ETC_SDVE_PROP_GRID)->SetWindowPos(NULL, rGrid.left, rGrid.top, rGrid.Width(), rGrid.Height(),                                                              SWP_NOMOVE | SWP_NOZORDER);
            GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rClose.left, rClose.top, rClose.Width(), rClose.Height(), SWP_NOSIZE);
        }
        break;
    case _LANGUAGE_RUS_:
        {
            const auto nExpand = 35;
            const auto nBotCustom = 40;

            m_wndGrid->SetColWidth(0, 0, globalUtils.ScaleByDPI(368));

            auto* pDlg      = this;
            auto* pGrid     = GetDlgItem(IDC_ETC_SDVE_PROP_GRID);
            auto* pCloseBtn = GetDlgItem(IDCANCEL);

            CRect rDlg;
            CRect rGrid;
            CRect rCloseBtn;
            pDlg->GetWindowRect(rDlg);
            pGrid->GetWindowRect(rGrid);
            pCloseBtn->GetWindowRect(rCloseBtn);

            rDlg.right      += nExpand + 2; //간격 추가 맞춤
            rGrid.right     += nExpand;
            rCloseBtn.left  += nExpand;
            rCloseBtn.right += nExpand;

            rDlg.bottom      -= nBotCustom;
            rGrid.bottom     -= nBotCustom;
            rCloseBtn.top    -= nBotCustom;
            rCloseBtn.bottom -= nBotCustom;

            ScreenToClient(rGrid);
            ScreenToClient(rCloseBtn);

            pDlg->MoveWindow(rDlg, TRUE);
            pGrid->SetWindowPos(NULL, rGrid.left, rGrid.top, rGrid.Width(), rGrid.Height(), SWP_NOMOVE | SWP_NOZORDER);
            pCloseBtn->SetWindowPos(NULL, rCloseBtn.left, rCloseBtn.top, rCloseBtn.Width(), rCloseBtn.Height(), SWP_NOSIZE);
        }
        break;
    default:
        {
            int n=368;
            m_wndGrid->SetColWidth(0, 0, globalUtils.ScaleByDPI(n));
        }
        break;
    }

	m_wndGrid->SetColWidth(1, 1, globalUtils.ScaleByDPI(80));
	m_wndGrid->SetRowHeight(0, 0, globalUtils.ScaleByDPI(23));
	m_wndGrid->SetRowHeight(1, m_wndGrid->GetRowCount(), globalUtils.ScaleByDPI(20));

	m_wndGrid->ColHeaderStyle()
		.SetReadOnly(TRUE).SetEnabled(FALSE);
	m_wndGrid->RowHeaderStyle()
		.SetReadOnly(TRUE).SetEnabled(FALSE);
	m_wndGrid->SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)    
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_GEN)
		.SetPlaces(5)
		.SetEnabled(FALSE)
		);
//   m_wndGrid->SetStyleRange(CGXRange().SetCols(2), 
//     CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));

	// cell의 width와 height 크기 변경을 금지한다.
	m_wndGrid->GetParam()->EnableTrackRowHeight(FALSE);
	m_wndGrid->GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	m_wndGrid->GetParam()->EnableMoveCols(FALSE);
	m_wndGrid->GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	m_wndGrid->GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

//   // 현재 셀을 설정한다.
//   m_wndGrid->SetCurrentCell(1, 1);

	m_wndGrid->LockUpdate(FALSE);
	m_wndGrid->Redraw();
	m_wndGrid->SetFocus();

	return TRUE;
}

void CCMSdvePropDlg::SetTitleInGrid()
{
	CString strTitle, strUntElast;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_ELAST, strUntElast);

	m_wndGrid->SetValueRange(CGXRange( 0, 0), _LS(IDS_SDVE_PROP_GRID_TXT1 ));
	m_wndGrid->SetValueRange(CGXRange( 1, 0), _LS(IDS_SDVE_PROP_GRID_TXT2 ));
	strTitle.Format(_T("%s(%s)"), _LS(IDS_SDVE_PROP_GRID_TXT3 ), strUntElast);
	m_wndGrid->SetValueRange(CGXRange( 2, 0), strTitle);
	m_wndGrid->SetValueRange(CGXRange( 3, 0), _LS(IDS_SDVE_PROP_GRID_TXT4 ));
	strTitle.Format(_T("%s(%s)"), _LS(IDS_SDVE_PROP_GRID_TXT5 ), strUntElast);
	m_wndGrid->SetValueRange(CGXRange( 4, 0), strTitle);
	strTitle.Format(_T("%s(%s)"), _LS(IDS_SDVE_PROP_GRID_TXT6 ), strUntElast);
	m_wndGrid->SetValueRange(CGXRange( 5, 0), strTitle);
	m_wndGrid->SetValueRange(CGXRange( 6, 0), _LS(IDS_SDVE_PROP_GRID_TXT7 ));
	m_wndGrid->SetValueRange(CGXRange( 7, 0), _LS(IDS_SDVE_PROP_GRID_TXT8 ));
	strTitle.Format(_T("%s(%s)"), _LS(IDS_SDVE_PROP_GRID_TXT9 ), strUntElast);
	m_wndGrid->SetValueRange(CGXRange( 8, 0), strTitle);
	m_wndGrid->SetValueRange(CGXRange( 9, 0), _LS(IDS_SDVE_PROP_GRID_TXT10));
	strTitle.Format(_T("%s(%s)"), _LS(IDS_SDVE_PROP_GRID_TXT11 ), strUntElast);
	m_wndGrid->SetValueRange(CGXRange(10, 0), strTitle);
	m_wndGrid->SetValueRange(CGXRange(11, 0), _LS(IDS_SDVE_PROP_GRID_TXT12));
	m_wndGrid->SetValueRange(CGXRange(12, 0), _LS(IDS_SDVE_PROP_GRID_TXT13));
	m_wndGrid->SetValueRange(CGXRange(13, 0), _LS(IDS_SDVE_PROP_GRID_TXT14));
	m_wndGrid->SetValueRange(CGXRange(14, 0), _LS(IDS_SDVE_PROP_GRID_TXT15));
	m_wndGrid->SetValueRange(CGXRange(15, 0), _LS(IDS_SDVE_PROP_GRID_TXT16));
	m_wndGrid->SetValueRange(CGXRange(16, 0), _LS(IDS_SDVE_PROP_GRID_TXT17));
	m_wndGrid->SetValueRange(CGXRange(17, 0), _LS(IDS_SDVE_PROP_GRID_TXT18));
	m_wndGrid->SetValueRange(CGXRange(18, 0), _LS(IDS_SDVE_PROP_GRID_TXT19));
	m_wndGrid->SetValueRange(CGXRange(19, 0), _LS(IDS_SDVE_PROP_GRID_TXT20));
	m_wndGrid->SetValueRange(CGXRange(20, 0), _LS(IDS_SDVE_PROP_GRID_TXT21));
}

void CCMSdvePropDlg::SetDataInGrid()
{  
	switch(m_nMatlType)
	{
	case D_SDVE_GR100: m_wndGrid->SetValueRange(CGXRange( 0, 1), _LS(IDS_SDVE_PROP_GRID_MATL_GR100 )); break;
	case D_SDVE_GR300: m_wndGrid->SetValueRange(CGXRange( 0, 1), _LS(IDS_SDVE_PROP_GRID_MATL_GR300 )); break;
	case D_SDVE_SR05 : m_wndGrid->SetValueRange(CGXRange( 0, 1), _LS(IDS_SDVE_PROP_GRID_MATL_SR05  )); break;
	case D_SDVE_GR400: m_wndGrid->SetValueRange(CGXRange( 0, 1), _LS(IDS_SDVE_PROP_GRID_MATL_GR400 )); break;
	case D_SDVE_CST  : m_wndGrid->SetValueRange(CGXRange( 0, 1), _LS(IDS_SDVE_PROP_GRID_MATL_CST   )); break;
	default: ASSERT(0); break;
	}

	for(int i=0; i<20; i++)
	{
		if(m_dValue[i]==0.) 
		{
			m_wndGrid->SetValueRange(CGXRange(i+1, 1), _T("-"));
			m_wndGrid->SetStyleRange(CGXRange(i+1, 1), CGXStyle().SetHorizontalAlignment(DT_CENTER));
		}
		else
		{
			m_wndGrid->SetValueRange(CGXRange(i+1, 1), m_dValue[i]);
		}
	}
}