// CMRposItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRposItemDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
	
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_common\wg_common_TBGrid.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRposItemDlg dialog


CCMRposItemDlg::CCMRposItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMRposItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRposItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bSetParam = FALSE;
	m_ParamData.Initialize();
	m_csOldName = _T("");

	m_pDoc = CDBDoc::GetDocPoint();
}

void CCMRposItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRposItemDlg)
	DDX_Control(pDX, IDC_CMD_EDIT_DESC, m_editDesc);
	DDX_Control(pDX, IDC_CMD_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_CMD_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
}


void CCMRposItemDlg::SetParamData(T_RPOS_D& data)
{
	m_bSetParam = TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.Name;
}

void CCMRposItemDlg::InitStagData()
{
	m_pDoc->m_pAttrCtrl->GetStagKeyList(m_aStagKey);
	m_mapStag2Index.InitHashTable(101);
	for (int i = 0; i < m_aStagKey.GetSize(); i++)
		m_mapStag2Index.SetAt(m_aStagKey[i], i);
}

void CCMRposItemDlg::InitGrid()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	// undo/redo를 금지한다.
	m_wndGrid.GetParam()->EnableUndo(FALSE);
	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	m_wndGrid.GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	// Row와 Column 이동을 금지시킨다.
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	// Row의 높이 조정을 금지한다.
	m_wndGrid.GetParam()->EnableTrackRowHeight(GX_TRACK_NOTHEADER);
	// 현재 셀이 있으면 Focus를 잃더라도 표시되게 한다.
	//m_wndGrid.GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	// Memory DC를 이용한 출력으로 설정
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);
	// column header가 current cell이 되는 것을 막는다.
	m_wndGrid.ColHeaderStyle().SetEnabled(FALSE);
	// row header가 current cell이 되는 것을 막는다.
	m_wndGrid.RowHeaderStyle().SetEnabled(FALSE);

	// Perperties를 수정한다.
	CGXProperties* pProp = m_wndGrid.GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE);                 // Turn off pressed button effect for column headers
	pProp->SetPrintRowHeaders(FALSE);               // Don't print column headers
	//pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // Don't draw border of current cell
	//       (CGXStyle) pProp->sInvertNoBorder);
	pProp->SetCenterHorizontal(FALSE);

	#if defined(_JP)
	m_wndGrid.ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	#endif

	// 기본 데이타 설정
	m_wndGrid.SetColCount(1);
	int nRowCount = m_aStagKey.GetSize();
	m_wndGrid.SetRowCount(nRowCount);

	T_STAG_D StagData;
	for (int i = 0; i < nRowCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStag(m_aStagKey[i], StagData);
		m_wndGrid.SetValueRange(CGXRange(i+1, 0), StagData.StageName);
	}
	m_wndGrid.SetColWidth(0, 0, globalUtils.ScaleByDPI(145));
	m_wndGrid.SetColWidth(1, 1, globalUtils.ScaleByDPI(60));

	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_CMD0417__Stage));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD0417__Node));
	m_wndGrid.SetStyleRange(CGXRange(0, 1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER));
	if (nRowCount > 0)
	{
		m_wndGrid.SetStyleRange(CGXRange(1, 0, nRowCount, 0), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC)
			);
	}
	m_wndGrid.SetStyleRange(CGXRange().SetCols(0, 1), CGXStyle()
		.SetVerticalAlignment(DT_VCENTER)
		);
	m_wndGrid.SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING)
		);
	m_wndGrid.SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0)
		);
	m_wndGrid.GetParam()->SetEnterKeyAction(GX_DOWN);
	m_wndGrid.SetCurrentCell(1, 1);

	m_wndGrid.SetScrollBarMode(SB_VERT|SB_HORZ, gxnEnabled | gxnEnhanced);
}

void CCMRposItemDlg::Data2Dlg()
{
	m_editName.SetWindowText(m_ParamData.Name);
	m_editDesc.SetWindowText(m_ParamData.Description);

	int ix;
	int nNode = m_ParamData.aReacPos.GetSize();
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = 0; i < nNode; i++)
	{
		if (m_mapStag2Index.Lookup(m_ParamData.aReacPos[i].Stag, ix))
			m_wndGrid.SetValueRange(CGXRange(ix+1, 1), (long)m_ParamData.aReacPos[i].Node);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMRposItemDlg::Dlg2Data()
{
	m_editName.GetWindowText(m_ParamData.Name);
	m_editDesc.GetWindowText(m_ParamData.Description);

	m_ParamData.aReacPos.RemoveAll();
	T_RPOS_BASE item;
	CString csValue;
	int NodeKey;
	int nStage = m_aStagKey.GetSize();
	for (int i = 0; i < nStage; i++)
	{
		csValue = m_wndGrid.GetValueRowCol(i+1, 1);
		csValue.TrimLeft(); csValue.TrimRight();
		if (csValue.IsEmpty()) continue;
		if (!CStrParser::GetINumber(csValue, NodeKey))
		{
			CString csMsg;
			csMsg.Format(_LS(IDS_CMD0417__Node_number__s__is_not_valid_), csValue);
			AfxMessageBox(csMsg);
			return FALSE;
		}
		item.Node = NodeKey;
		item.Stag = m_aStagKey[i];
		m_ParamData.aReacPos.Add(item);
	}

	return TRUE;
}

BOOL CCMRposItemDlg::DoApply()
{
	if (!Dlg2Data()) return FALSE;
	if (m_bSetParam)  // Modify
	{
		if (m_pDoc->m_pDataCtrl->ModifyRpos(m_csOldName, m_ParamData))
		{
			m_csOldName = m_ParamData.Name;
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return m_pDoc->m_pDataCtrl->AddRpos(m_ParamData);
	}
}

BEGIN_MESSAGE_MAP(CCMRposItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMRposItemDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_APPLY, OnCmdBtnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMRposItemDlg message handlers

BOOL CCMRposItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStagData();
	InitGrid();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMRposItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!DoApply()) return;
	CDialogMove::OnOK();
}

void CCMRposItemDlg::OnCmdBtnApply() 
{
	// TODO: Add your control notification handler code here
	DoApply();
}
