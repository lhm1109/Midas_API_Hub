// CMGridCrossReferenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridCrossReferenceDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_base\wg_base_I_GridColorMgr.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_NUM_COL   2
/////////////////////////////////////////////////////////////////////////////
// CCMGridCrossReferenceDlg dialog


CCMGridCrossReferenceDlg::CCMGridCrossReferenceDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMGridCrossReferenceDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMGridCrossReferenceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arCalcItem.Add(IDC_CMD_GRID_CALC_ITEM_RDO1);
	m_arCalcItem.Add(IDC_CMD_GRID_CALC_ITEM_RDO2);
	m_arCalcItem.Add(IDC_CMD_GRID_CALC_ITEM_RDO3);
	m_arCalcItem.Add(IDC_CMD_GRID_CALC_ITEM_RDO4);
	m_arCalcItem.Add(IDC_CMD_GRID_CALC_ITEM_RDO5);
	
	m_arCalcType.Add(IDC_CMD_GRID_CALC_TYPE_RDO1);
	m_arCalcType.Add(IDC_CMD_GRID_CALC_TYPE_RDO2);
	
	m_arCalcMasterPos.Add(IDC_CMD_CALC_POSITION);
	m_arCalcMasterPos.Add(IDC_CMD_CALC_POS_STATIC1);
	
	m_arCalcList.Add(IDC_CMD_CALC_POSITION2);
	m_arCalcList.Add(IDC_CMD_CALC_POS_STATIC2);
	m_arCalcList.Add(IDC_CMD_GRID_CALC_ADD_BTN);
	m_arCalcList.Add(IDC_CMD_PLACEHOLDER);

	m_arCalcTypeElem.Add(IDC_CMD_GRID_CALC_TYPE_MASTER_ELEM);
	m_arCalcTypeElem.Add(IDC_CMD_GRID_CALC_TYPE_LIST_ELEM);

	m_arCalcTypeLink.Add(IDC_CMD_GRID_CALC_TYPE_MASTER_LINK);
	m_arCalcTypeLink.Add(IDC_CMD_GRID_CALC_TYPE_LIST_LINK);
	
	m_arCalcTypeNode.Add(IDC_CMD_GRID_CALC_TYPE_MASTER);
	m_arCalcTypeNode.Add(IDC_CMD_GRID_CALC_TYPE_LIST);
}


void CCMGridCrossReferenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridCrossReferenceDlg)
	DDX_Control(pDX, IDC_CMD_CALC_POSITION2, m_cmbCalcPosition2);
	DDX_Control(pDX, IDC_CMD_CALC_POSITION, m_cmbCalcPosition1);
	DDX_Control(pDX, IDC_CMD_GRID_CROSS_REFERENCE_LIST, m_ctrlCRList);
	DDX_Control(pDX, IDC_CMD_GRID_INCLUDE_IMPACT_FACTOR, m_ctrlIncImpactFactor);
	DDX_Control(pDX, IDC_CMD_GRID_CALC_TYPE_MASTER, m_wndMasterNode);
	DDX_Control(pDX, IDC_CMD_GRID_CALC_TYPE_MASTER_ELEM, m_wndMasterElem);
	DDX_Control(pDX, IDC_CMD_GRID_CALC_TYPE_MASTER_LINK, m_cmbMasterLink);
	DDX_Control(pDX, IDC_CMD_GRID_NAME, m_ctrlName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_GRID_CALC_TYPE_LIST, m_editSelectNode);
	DDX_Control(pDX, IDC_CMD_GRID_CALC_TYPE_LIST_ELEM, m_editSelectElem);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndGrid);
	DDX_Control(pDX, IDC_CMD_GRID_CALC_TYPE_LIST_LINK, m_editSelectLink);
}


BEGIN_MESSAGE_MAP(CCMGridCrossReferenceDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMGridCrossReferenceDlg)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_ADD_BTN, OnCmdGridCalcAddBtn)
	ON_BN_CLICKED(IDC_CMD_GRID_OPER_ADD_BTN, OnCmdGridOperAddBtn)
	ON_BN_CLICKED(IDC_CMD_GRID_OPER_DEL_BTN, OnCmdGridOperDelBtn)
	ON_BN_CLICKED(IDC_CMD_GRID_OPER_MOD_BTN, OnCmdGridOperModBtn)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_TYPE_RDO1, OnCmdCalcTypeCtrl)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_TYPE_RDO2, OnCmdCalcTypeCtrl)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_ITEM_RDO1, OnCmdCalcItemCtrl)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_ITEM_RDO2, OnCmdCalcItemCtrl)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_ITEM_RDO3, OnCmdCalcItemCtrl)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_ITEM_RDO4, OnCmdCalcItemCtrl)
	ON_BN_CLICKED(IDC_CMD_GRID_CALC_ITEM_RDO5, OnCmdCalcItemCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_GRID_CROSS_REFERENCE_LIST, OnItemchangedCrefList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridCrossReferenceDlg message handlers

BOOL CCMGridCrossReferenceDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();
	AlignCtrl();
	SetListCtrlHeader();
	//SetSubListCtrlHeader();
	InitGrid();

	m_editSelectNode.Connect(SC_ID_NODE, &m_editSelectNode);
	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);
	m_editSelectLink.Connect(SC_ID_NODE, &m_editSelectLink);

	m_wndMasterNode.SetAttNodeList();
	m_wndMasterNode.SetLButtonDownNotifyWindow(this);
	m_wndMasterNode.SetEnterNotifyWindow(this);
	m_wndMasterNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndMasterNode.SetMaxNodeKeyNum(1);
	
	m_wndMasterElem.SetAttElemList();
	m_wndMasterElem.SetLButtonDownNotifyWindow(this);
	m_wndMasterElem.SetEnterNotifyWindow(this);
	m_wndMasterElem.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndMasterElem.SetMaxElemKeyNum(1);
	GetDlgItem(IDC_CMD_GRID_CLOSE_BTN)->ShowWindow(FALSE);

	m_cmbMasterLink.ResetContent();

	MakeCombo();
	MakeItemEx();

	T_CREF_D data;
	data.Initialize();
	Data2Dlg(data);
	OnCmdCalcCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridCrossReferenceDlg::InitGrid()
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
	pParam->EnableMoveRows(FALSE);                  // rows cannot be dragged
	pParam->EnableMoveCols(FALSE);                 // columns can be dragged
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

	m_wndGrid.SetRowHeightByDPI(0, 0, 18);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(D_NUM_COL);
	SetGridTitle();
	m_wndGrid.SetColWidthByDPI(0, 0, 25);
	m_wndGrid.SetColWidthByDPI(1, 1, 45);
	m_wndGrid.SetColWidthByDPI(2, 2, 90);

	//Combo
	CString TypeList;
	TypeList.Format(_T("%s\n%s\n%s\n"), 
		_LSX(I), _LSX(J), _LSX(Both));
	m_wndGrid.SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(TypeList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValue(_T("")));

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMGridCrossReferenceDlg::SetGridTitle()
{
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _T(""));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD__CROSS_REFERENCE_ELEM));
	m_wndGrid.SetValueRange(CGXRange(0, 2), _LS(IDS_CMD__CROSS_REFERENCE_POSITION));
	
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMGridCrossReferenceDlg::MakeCombo()
{
	m_cmbCalcPosition2.ResetContent();
	m_cmbCalcPosition1.ResetContent();

	CString str[] = {_LSX(I), _LSX(J), _LSX(Both)};
	for (int i=0; i<3; i++)
	{
		m_cmbCalcPosition1.AddString(str[i]);
		m_cmbCalcPosition1.SetItemData(i, i);
		m_cmbCalcPosition2.AddString(str[i]);
		m_cmbCalcPosition2.SetItemData(i, i);
	}
	m_cmbCalcPosition1.SetCurSel(2);
	m_cmbCalcPosition2.SetCurSel(2);
}

void CCMGridCrossReferenceDlg::AlignCtrl()
{
	CRect refRect, rect;
	int dX; int dY;

	// Align Elem Master Control
	m_wndMasterNode.GetWindowRect(refRect);
	CArray<UINT,UINT> arResId;

	arResId.RemoveAll();
	arResId.Add(m_wndMasterElem.GetDlgCtrlID());
	m_wndMasterElem.GetWindowRect(rect);
	
	dX = refRect.left - rect.left;
	dY = refRect.top - rect.top;
	CDlgUtil::CtrlMoveDistXY(this, arResId, dX, dY);

	// Align Link Master Control
	arResId.RemoveAll();
	arResId.Add(m_cmbMasterLink.GetDlgCtrlID());
	m_cmbMasterLink.GetWindowRect(rect);
	
	dX = refRect.left - rect.left;
	dY = refRect.top - rect.top;
	CDlgUtil::CtrlMoveDistXY(this, arResId, dX, dY);

	// Align Select Elem/Node Control
	m_editSelectNode.GetWindowRect(refRect);

	arResId.RemoveAll();
	arResId.Add(m_editSelectElem.GetDlgCtrlID());
	m_editSelectElem.GetWindowRect(rect);
	
	dX = refRect.left - rect.left;
	dY = refRect.top - rect.top;
	CDlgUtil::CtrlMoveDistXY(this, arResId, dX, dY);

	arResId.RemoveAll();
	arResId.Add(m_editSelectLink.GetDlgCtrlID());
	m_editSelectLink.GetWindowRect(rect);
	
	dX = refRect.left - rect.left;
	dY = refRect.top - rect.top;
	CDlgUtil::CtrlMoveDistXY(this, arResId, dX, dY);
}

BOOL CCMGridCrossReferenceDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	int nNum = m_ctrlCRList.GetItemCount();
	int i = 0;
	for (i=0; i<nNum; i++)
		if (m_ctrlCRList.GetItemData(i) == key) break;
	if (i == nNum) return FALSE;

	T_CREF_D data;
	if (m_pDoc->m_pAttrCtrl->GetCref(key, data)) 
	{
		m_ctrlCRList.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);  
		Data2Dlg(data);
	}
	return TRUE;
}

void CCMGridCrossReferenceDlg::OnCmdCalcItemCtrl()
{
	int nItemCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcItem, nItemCheck);

	m_editSelectElem.SetWindowText(_T(""));
	m_wndMasterElem.SetWindowText(_T(""));

	m_editSelectLink.SetWindowText(_T(""));
	m_cmbMasterLink.SetCurSel(-1);
	
	m_editSelectNode.SetWindowText(_T(""));
	m_wndMasterNode.SetWindowText(_T(""));

	m_wndGrid.SetRowCount(0);

	OnCmdCalcTypeCtrl();
}


void CCMGridCrossReferenceDlg::OnCmdCalcTypeCtrl()
{
	int nItemCheck, nTypeCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcItem, nItemCheck);
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcType, nTypeCheck);

	CString str;
	if (nTypeCheck == 1)
	{
		CArray<T_ELNK_K, T_ELNK_K> aElnkK;
		CArray<T_NLNK_K, T_NLNK_K> aNlnkK;
		switch (nItemCheck)
		{
		case 0: case 1: case 2: break;
		case 3:
			m_cmbMasterLink.ResetContent();
			m_pDoc->m_pAttrCtrl->GetElnkKeyList(aElnkK);
			for (int i = 0; i < aElnkK.GetSize(); i++)
			{
				CString strKey;
				strKey.Format(_T("%d"), aElnkK[i]);
				CDlgUtil::CobxAddItem(m_cmbMasterLink, strKey, aElnkK[i]);
			}
			break;
		case 4:
			m_cmbMasterLink.ResetContent();
			m_pDoc->m_pAttrCtrl->GetNlnkKeyList(aNlnkK);
			for (int i = 0; i < aNlnkK.GetSize(); i++)
			{
				CString strKey;
				strKey.Format(_T("%d"), aNlnkK[i]);
				CDlgUtil::CobxAddItem(m_cmbMasterLink, strKey, aNlnkK[i]);
			}
			break;
		default: ASSERT(0);
		}
	}

	OnCmdCalcCtrl();
}
void CCMGridCrossReferenceDlg::OnCmdCalcCtrl()
{
	int nItemCheck, nTypeCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcItem, nItemCheck);
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcType, nTypeCheck);

	CDlgUtil::CtrlShowHide(this, m_arCalcTypeNode, nItemCheck == 0 || nItemCheck == 1);
	CDlgUtil::CtrlShowHide(this, m_arCalcTypeElem, nItemCheck == 2);
	CDlgUtil::CtrlShowHide(this, m_arCalcTypeLink, nItemCheck == 3 || nItemCheck == 4);
	CDlgUtil::CtrlShowHide(this, m_arCalcMasterPos, nItemCheck == 2 || nItemCheck == 3 || nItemCheck == 4);
	CDlgUtil::CtrlShowHide(this, m_arCalcList, nItemCheck == 2 || nItemCheck == 3 || nItemCheck == 4);

	GetDlgItem(IDC_CMD_GRID_CALC_TYPE_MASTER)->EnableWindow(nTypeCheck==1);
	GetDlgItem(IDC_CMD_GRID_CALC_TYPE_MASTER_ELEM)->EnableWindow(nTypeCheck==1);
	GetDlgItem(IDC_CMD_GRID_CALC_TYPE_MASTER_LINK)->EnableWindow(nTypeCheck==1);

	CWnd* ListStc = GetDlgItem(IDC_WG_CMD_STATIC6);
	switch (nItemCheck)
	{
	//Node
	case 0: // Reaction
	case 1: // Displacement
		ListStc->SetWindowText(_LS(IDS_CMD__CROSS_REFERENCE_SELECT_NODE));
		break;
	//Element
	case 2: // Member Force
		m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD__CROSS_REFERENCE_ELEM));
		ListStc->SetWindowText(_LS(IDS_CMD__CROSS_REFERENCE_SELECT_ELEMENT));
		break;
	// Link
	case 3: // Elastic Link
	case 4: // General Link
		m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_CMD__CROSS_REFERENCE_NODE));
		ListStc->SetWindowText(_LS(IDS_CMD__CROSS_REFERENCE_SELECT_NODES));
		break;
	default: ASSERT(FALSE);
	}
}

BOOL CCMGridCrossReferenceDlg::Dlg2Data(T_CREF_D& data, int nSeq)
{
	data.Initialize();

	m_ctrlName.GetWindowText(data.Name);
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcItem, data.nCalcItem);
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcType, data.nCalcType);
	
	data.bIncImpFactor=(m_ctrlIncImpactFactor.GetCheck()==0)? FALSE:TRUE;

	CString str;
	if (data.nCalcType == 1)
	{
		if (data.nCalcItem == 0 || data.nCalcItem == 1)
		{
			m_wndMasterNode.GetWindowText(str);
			data.MasterK = _ttoi(str);
		}
		else if (data.nCalcItem == 2)
		{
			m_wndMasterElem.GetWindowText(str);
			data.MasterK = _ttoi(str);
		}
		else if (data.nCalcItem == 3 || data.nCalcItem == 4)
		{
			m_cmbMasterLink.GetWindowText(str);
			data.MasterK = _ttoi(str);
		}

		if (data.nCalcItem==2 || data.nCalcItem==3 || data.nCalcItem==4)
			data.nCalcPositionMaster = m_cmbCalcPosition1.GetCurSel();
	}

	if (data.nCalcItem==2 || data.nCalcItem==3 || data.nCalcItem==4)
	{
		GetSubListData(data.aElemBase);
	}
	else
	{
		//Node Selection일때
		m_editSelectNode.GetWindowText(str);
		
		CArray<int,int> aKey;
		if(!CStrParser::ParsingListByTo(str,aKey)) return FALSE;
		int nSize = aKey.GetSize();
		for (int i=0; i<nSize; i++)
			data.aList.Add(aKey[i]);
	}
	
	if (nSeq == -1)
		data.nSeq = m_ctrlCRList.GetItemCount() + 1;    //Add일 경우
	else
		data.nSeq = nSeq;     //Modify일 경우
		
	return TRUE;
}

void CCMGridCrossReferenceDlg::Data2Dlg(T_CREF_D& data)
{
	m_ctrlName.SetWindowText(data.Name);
	CDlgUtil::CtrlRadioSetCheck(this, m_arCalcItem, data.nCalcItem);
	CDlgUtil::CtrlRadioSetCheck(this, m_arCalcType, data.nCalcType);
	m_ctrlIncImpactFactor.SetCheck(data.bIncImpFactor);

	CString str;
	if (data.nCalcType == 1)
	{
		str.Format(_T("%d"), data.MasterK);
		CArray<T_ELNK_K, T_ELNK_K> aElnkK;
		CArray<T_NLNK_K, T_NLNK_K> aNlnkK;
		switch (data.nCalcItem)
		{
		case 0: case 1:
			m_wndMasterNode.SetWindowText(str);
			break;
		case 2:
			m_wndMasterElem.SetWindowText(str);
			m_cmbCalcPosition1.SetCurSel(data.nCalcPositionMaster);
			break;
		case 3:
			m_cmbMasterLink.ResetContent();
			m_pDoc->m_pAttrCtrl->GetElnkKeyList(aElnkK);
			for (int i = 0; i < aElnkK.GetSize(); i++)
			{
				CString strKey;
				strKey.Format(_T("%d"), aElnkK[i]);
				CDlgUtil::CobxAddItem(m_cmbMasterLink, strKey, aElnkK[i]);
			}
			CDlgUtil::CobxSetCurSelItemData(m_cmbMasterLink, (DWORD)data.MasterK);
			m_cmbCalcPosition1.SetCurSel(data.nCalcPositionMaster);
			break;
		case 4:
			m_cmbMasterLink.ResetContent();
			m_pDoc->m_pAttrCtrl->GetNlnkKeyList(aNlnkK);
			for (int i = 0; i < aNlnkK.GetSize(); i++)
			{
				CString strKey;
				strKey.Format(_T("%d"), aNlnkK[i]);
				CDlgUtil::CobxAddItem(m_cmbMasterLink, strKey, aNlnkK[i]);
			}
			CDlgUtil::CobxSetCurSelItemData(m_cmbMasterLink, (DWORD)data.MasterK);
			m_cmbCalcPosition1.SetCurSel(data.nCalcPositionMaster);
			break;
		default: ASSERT(0);
		}
	}
	else
	{
		m_editSelectElem.SetWindowText(_T(""));
		m_wndMasterElem.SetWindowText(_T(""));

		m_editSelectLink.SetWindowText(_T(""));
		m_cmbMasterLink.SetCurSel(-1);
		
		m_editSelectNode.SetWindowText(_T(""));
		m_wndMasterNode.SetWindowText(_T(""));

		m_cmbCalcPosition1.SetCurSel(2);
		m_cmbCalcPosition2.SetCurSel(2);
	}

	if (data.nCalcItem==2 || data.nCalcItem==3 || data.nCalcItem==4)
	{
		SetSubListData(data.aElemBase);
	}
	else
	{
		CNumericOptimizer optimizer;
		str = optimizer.OptimizeUnsortedArray(data.aList);
		m_editSelectNode.SelectByStr(str);
	}

	UpdateData(FALSE);
	//Control 처리
	OnCmdCalcCtrl();
}

void CCMGridCrossReferenceDlg::OnCmdGridOperAddBtn() 
{
	T_CREF_D data;
	
	if (!Dlg2Data(data)) return;
	if (!ValidData(data)) return;

	int nCount = m_ctrlCRList.GetItemCount();
	if (m_pDoc->m_pDataCtrl->AddCref(data))
	{
		m_wndGrid.SetRowCount(0);
		m_ctrlCRList.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
}

void CCMGridCrossReferenceDlg::OnCmdGridOperDelBtn() 
{
	int nIndex;
	CString strName;
	if (!GetSelectedData(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___CROSS_REFERENCE_NO_SELECTED));
		return;
	}
	int nCount = m_ctrlCRList.GetItemCount();
	strName = m_ctrlCRList.GetItemText(nIndex, 0);
	if (m_pDoc->m_pDataCtrl->DelCref(strName))
	{
		m_wndGrid.SetRowCount(0);
		if (nIndex >= nCount-1) nIndex--;
		m_ctrlCRList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
	}

	//SetDataSequence();
}

void CCMGridCrossReferenceDlg::SetDataSequence()
{
	T_CREF_D data;
	CString strName;
	int nSize = m_ctrlCRList.GetItemCount();
	if (nSize < 1)
		return;

	for (int i=0; i<nSize; i++)
	{
		strName = m_ctrlCRList.GetItemText(i, 0);
		m_pDoc->m_pAttrCtrl->GetCref(strName, data);
		data.nSeq = i+1;
		m_pDoc->m_pDataCtrl->ModifyCref(strName, data);
	}

}

void CCMGridCrossReferenceDlg::OnCmdGridOperModBtn() 
{
	int nIndex;
	CString strName;
	if (!GetSelectedData(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___CROSS_REFERENCE_NO_SELECTED));
		return;
	}

	T_CREF_D data;
	if (!Dlg2Data(data, nIndex+1)) return;
	if (!ValidData(data)) return;
	
	strName = m_ctrlCRList.GetItemText(nIndex, 0);
	if (m_pDoc->m_pDataCtrl->ModifyCref(strName, data))
		m_ctrlCRList.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMGridCrossReferenceDlg::OnTmClose()
{
	CloseDlg();
}

void CCMGridCrossReferenceDlg::SetListCtrlHeader() 
{
	CString aTitle[] = {_LS(IDS_CMD__CROSS_REFERENCE_NAME), _LS(IDS_CMD__CROSS_REFERENCE_ITEM)
										, _LS(IDS_CMD__CROSS_REFERENCE_TYPE)};
	int nColWidth[] = {50, 90, 60};
	int nColNum = 3;
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ctrlCRList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ctrlCRList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ctrlCRList.InsertColumn(i,&lvcolumn);
		m_ctrlCRList.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMGridCrossReferenceDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_ctrlCRList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMGridCrossReferenceDlg::MakeItemEx()
{
	m_ctrlCRList.DeleteAllItems();

	T_CREF_K key;
	T_CREF_D data;
	CArray<T_CREF_K, T_CREF_K> aKey;
	m_pDoc->m_pAttrCtrl->GetCrefKeyList(aKey);
	
	int nSize = aKey.GetSize();
	if (nSize == 0)
		return;
	
	for (int i=0; i<nSize; i++)
	{
		key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetCref(key, data);
		m_ctrlCRList.InsertItem(i, _T(""));
		SetItem(i, key, data);
	}
	//m_ctrlCRList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMGridCrossReferenceDlg::SetItem(int nIndex, T_CREF_K key, T_CREF_D &data)
{
	LVITEM lvitem;
	CString str;
	lvitem.iItem=nIndex;
	for(int i = 0; i < 5; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, key, data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ctrlCRList.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_ctrlCRList.SetItemData(nIndex, (DWORD)key);
}

CString CCMGridCrossReferenceDlg::DataToStr(int i, T_CREF_K key, T_CREF_D &data)
{
	CString value;
	
	switch(i)
	{
		case 0:
			value = data.Name;
			break;
		case 1:
			if (data.nCalcItem == 0)
				value = _LS(IDS_CMD__CROSS_REFERENCE_REACTION);
			else if (data.nCalcItem == 1)
				value = _LS(IDS_CMD__CROSS_REFERENCE_DISPLACEMENT);
			else if (data.nCalcItem == 2)
				value = _LS(IDS_CMD__CROSS_REFERENCE_MEMBERFORCE);
			else if (data.nCalcItem == 3)
				value = _LS(IDS_CMD__CROSS_REFERENCE_ELNK);
			else if (data.nCalcItem == 4)
				value = _LS(IDS_CMD__CROSS_REFERENCE_GLNK);
			break;
		case 2:
			if (data.nCalcType == 0)
				value = _LS(IDS_CMD__CROSS_REFERENCE_MATRIX);
			else
				value = _LS(IDS_CMD__CROSS_REFERENCE_MAX);
			break;
	}
	return value;
}

void CCMGridCrossReferenceDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemEx();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMGridCrossReferenceDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;

		switch(nCmd)
		{
		case(UR_CREF_ADD):
		case(UR_CREF_DEL):
		case(UR_CREF_MFD):
		case(UR_CREF_MFS):
			bMFD = TRUE;
			break;
		default:
			break;
		}
	}

	if (bMFD)
		MakeItemEx();
}

BOOL CCMGridCrossReferenceDlg::ValidData(T_CREF_D& data)
{
	BOOL bError = FALSE;
	if (data.Name == _T("")) bError = TRUE;
	if (data.nCalcType == 1 && data.MasterK < 1) bError = TRUE;
	if (data.nCalcItem == 2 || data.nCalcItem == 3 || data.nCalcItem == 4)
	{
		if (data.nCalcType == 1 && data.aElemBase.GetSize() == 0) bError = TRUE;
	}
	else
	{
		if (data.aList.GetSize() == 0) bError = TRUE;
	}

	if (bError)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD__ADDD__Error___CROSS_REFERENCE));
		return FALSE;
	}
	return TRUE;
}

void CCMGridCrossReferenceDlg::OnItemchangedCrefList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	T_CREF_K key;
	T_CREF_D data;
	int nIndex;

	if (!GetSelectedData(nIndex))
	{
		data.Initialize();
	}
	else 
	{
		key = m_ctrlCRList.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetCref(key, data);
	}

	// 값을 설정한다.
	Data2Dlg(data);

	*pResult = 0;
}

void CCMGridCrossReferenceDlg::OnCmdGridCalcAddBtn()
{
	int nItemCheck, nTypeCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcItem, nItemCheck);
	CDlgUtil::CtrlRadioGetCheck(this, m_arCalcType, nTypeCheck);

	CArray<UINT, UINT> aKey;

	if (nItemCheck == 2)
	{
		CString str;
		CArray<int, int> aElemKey;

		m_editSelectElem.GetWindowText(str);
		if (!CStrParser::ParsingListByTo(str, aElemKey))
			return;
		for (int i = 0; i < aElemKey.GetSize(); i++)
		{
			aKey.Add(aElemKey[i]);
		}
	}
	else if (nItemCheck == 3 || nItemCheck == 4)
	{
		CString str;
		CArray<T_NODE_K, T_NODE_K> aNodeKey;

		m_editSelectLink.GetWindowText(str);
		GetNodeList(str, aNodeKey);

		CArray<UINT, UINT> aLinkKey;
		CArray<T_ELNK_K, T_ELNK_K> aElnkKey;
		CArray<T_NLNK_K, T_NLNK_K> aNlnkKey;
		m_pDoc->m_pAttrCtrl2->GetLinkList(aNodeKey, aElnkKey, aNlnkKey);

		if (nItemCheck == 3)
		{
			aLinkKey.Copy(aElnkKey);
		}
		else if (nItemCheck == 4)
		{
			aLinkKey.Copy(aNlnkKey);
		}

		for (int i = 0; i < aLinkKey.GetSize(); i++)
		{
			aKey.Add(aLinkKey.GetAt(i));
		}
	}

	T_CREF_BASE CrefBase;
	CrefBase.Initialize();
	CrefBase.nCalcPosition = m_cmbCalcPosition2.GetCurSel();

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CrefBase.ElemK = aKey[i];
		int nIndex = Grid_ExistKey(aKey[i]);
		if (nIndex == -1)
		{
			Grid_InsertItem(nIndex, CrefBase);
		}
		else
		{
			BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
			m_wndGrid.SetValueRange(CGXRange(nIndex, 2), GetCalcPositionString(CrefBase.nCalcPosition));
			m_wndGrid.LockUpdate(bOldLock);
			if (!bOldLock) m_wndGrid.Redraw();
		}
	}
}

void CCMGridCrossReferenceDlg::GetSubListData(CArray<T_CREF_BASE, T_CREF_BASE&>& aElemBase)
{
	aElemBase.RemoveAll();
	
	int nSize = m_wndGrid.GetRowCount();
	aElemBase.SetSize(nSize);
	
	T_CREF_BASE CrefBase;
	CrefBase.Initialize();
	for (int i=0; i<nSize; i++)
	{
		CrefBase.ElemK = _tstol(m_wndGrid.GetValueRowCol(i+1, 1));
		CrefBase.nCalcPosition = GetCalcPosition(m_wndGrid.GetValueRowCol(i+1, 2));
		aElemBase.SetAt(i, CrefBase);
	}
}

void CCMGridCrossReferenceDlg::SetSubListData(CArray<T_CREF_BASE, T_CREF_BASE&>& aElemBase)
{
	int nSize = aElemBase.GetSize();
	m_wndGrid.SetRowCount(nSize);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i=0; i<nSize; i++)
	{
		m_wndGrid.SetValueRange(CGXRange(i+1, 1), (long)aElemBase[i].ElemK); 
		m_wndGrid.SetValueRange(CGXRange(i+1, 2), GetCalcPositionString(aElemBase[i].nCalcPosition)); 
	}
	if (nSize > 0) m_wndGrid.SetCurrentCell(1, 1);
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

CString CCMGridCrossReferenceDlg::GetCalcPositionString(int nPosition)
{
	CString str;
	switch(nPosition)
	{
	case 0:
		str = _LSX(I);
		break;
	case 1:
		str = _LSX(J);
		break;
	case 2:
		str = _LSX(Both);
		break;
	}
	return str;
}

int CCMGridCrossReferenceDlg::GetCalcPosition(CString str)
{
	int nReturn = 0;
	if (str.CompareNoCase(_LSX(I)) == 0)
		nReturn = 0;
	else if (str.CompareNoCase(_LSX(J)) == 0)
		nReturn = 1;
	else if (str.CompareNoCase(_LSX(Both)) == 0)
		nReturn = 2;
	else
		ASSERT(0);

	return nReturn;
}

int CCMGridCrossReferenceDlg::Grid_ExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i+1, 1))) 
			return i+1;
	}
	return -1;
}

void CCMGridCrossReferenceDlg::Grid_InsertItem(int nPos, T_CREF_BASE& data)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	m_wndGrid.InsertRows(nRow+1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 1), (long)data.ElemK); 
	m_wndGrid.SetValueRange(CGXRange(nRow+1, 2), GetCalcPositionString(data.nCalcPosition)); 

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMGridCrossReferenceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_DELETE && pWnd->GetDlgCtrlID() == IDC_CMD_PLACEHOLDER)
		{
			OnCmdOpDelete();
			return TRUE;
		}
	}	
	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

void CCMGridCrossReferenceDlg::OnCmdOpDelete()
{
	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	CRowColArray awRows;
	m_wndGrid.GetSelectedRows(awRows, TRUE, TRUE);
	if (awRows.GetSize() < 1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_item_to_delete_));
		return;
	}
	ROWCOL nRow;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = awRows.GetSize()-1; i >= 0; i--)
	{
		nRow = awRows.GetAt(i);
		if (nRow == 0) continue;
		m_wndGrid.RemoveRows(nRow, nRow);
	}

	int nRowCount = m_wndGrid.GetRowCount();
	if (bCurrent && nRowCount > 0)
	{
		if (nRowCount < ncRow) ncRow = nRowCount;
		m_wndGrid.SetCurrentCell(ncRow, ncCol);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
BOOL CCMGXCrossRefGridWnd::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CCMGXGridWnd2::OnEndEditing(nRow, nCol);

	ROWCOL nRowSearch;
	int nCount = GetRowCount();
	int nKey = _tstol(GetValueRowCol(nRow, 1));
	for (int i=0; i<nCount; i++)
	{
		nRowSearch = i+1;
		if (nRow == nRowSearch)
			continue;

		if (nKey == _tstol(GetValueRowCol(nRowSearch, 1)))
		{
			AfxMessageBox(_T("에러"));
			return FALSE;
		}
	}
	return TRUE;
}
