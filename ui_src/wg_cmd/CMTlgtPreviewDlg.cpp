// CMTlgtPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTlgtPreviewDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_QSort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMTlgtPreviewDlg::CCMTlgtPreviewDlg(CDBDoc* pDoc, CCMTlgtDataStore* pDStore, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMTlgtPreviewDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pDStore = pDStore;
}


void CCMTlgtPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ETC_TLGT_LIST, m_ctrlList);


	//if (pDX->m_bSaveAndValidate)
	//{
	//	m_ParamData.ThisFuncName = m_csFuncName;
	//	m_ParamData.nThisFuncType = (m_bSinusoidal) ? 2 : 1;
	//	m_ParamData.nThisDataType = m_nDataType + 1;
	//	m_ParamData.Description = m_csDescription;

	//	if (m_ParamData.nThisFuncType == 1)
	//	{
	//		//Time Function 일때
	//		m_ParamData.nScaleMethod = m_nScaleType;
	//		if (m_nScaleType == 0)
	//		{
	//			if (!m_wndScaleFactor.GetEditValue(m_ParamData.ScaleFactor))
	//			{
	//				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
	//				pDX->Fail();
	//			}
	//		}
	//		else
	//		{
	//			m_ParamData.dMaxValue = m_wndScaleMaxVal.GetEditValue();
	//		}
	//	}
	//	m_ParamData.Gravity = m_wndGravity.GetEditValue();

	//	m_ParamData.SinuA = m_wndSinA.GetEditValue();
	//	m_ParamData.SinuC = m_wndSinC.GetEditValue();
	//	m_ParamData.SinuFrequency = m_wndSinF.GetEditValue();
	//	m_ParamData.SinuDampingFactor = m_wndSinD.GetEditValue();
	//	m_ParamData.SinuPhaseAngle = m_wndSinPA.GetEditValue();
	//}
	//else
	//{
	//	if (!m_bGridInit) return;
	//	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	//	ROWCOL nCount = m_ParamData.arThisFuncData.GetSize();
	//	m_wndGrid.SetRowCount(nCount + 1);
	//	m_wndGrid.SetCurrentCell(nCount + 1, 1);

	//	m_wndGrid.LockUpdate(bOldLock);
	//	if (!bOldLock) m_wndGrid.Redraw();
	//}
}


BEGIN_MESSAGE_MAP(CCMTlgtPreviewDlg, CDialogMove)
	ON_NOTIFY(NM_CLICK, IDC_ETC_TLGT_LIST, OnClickListItem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTlgtPreviewDlg message handlers

BOOL CCMTlgtPreviewDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	SetControl();
	SetText();
	UpdateData(FALSE);
	return TRUE; 

}

void CCMTlgtPreviewDlg::OnClickListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_ctrlList.GetFirstSelectedItemPosition();
	if (pos)
	{
		T_NODE_K KeyNode;
		int nIndex = m_ctrlList.GetNextSelectedItem(pos);
		KeyNode = m_ctrlList.GetItemData(nIndex);
		CalcPreview(m_trainItem, KeyNode);
	}
}

BOOL CCMTlgtPreviewDlg::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYUP:
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			POSITION pos = m_ctrlList.GetFirstSelectedItemPosition();
			if (pos)
			{
				T_NODE_K KeyNode;
				int nIndex = m_ctrlList.GetNextSelectedItem(pos);
				KeyNode = m_ctrlList.GetItemData(nIndex);
				CalcPreview(m_trainItem, KeyNode);
			}
		}
		break;
	}

	return CDialogMove::PreTranslateMessage(pMsg);
}



void CCMTlgtPreviewDlg::SetData(TLGT_INFO& item)
{
	m_trainItem = item;
	ConvertInData(m_trainItem);
}

void CCMTlgtPreviewDlg::ConvertInData(TLGT_INFO& trainInfo)
{
	CUnitCtrl* pUnitCtrl = ((CDBDoc*)CDBDoc::GetDocPoint())->m_pUnitCtrl; ASSERT(pUnitCtrl);

	trainInfo.dVelocity = pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_VELOCITY, trainInfo.dVelocity);
	trainInfo.dTime = pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_TIME, trainInfo.dTime);

	for (long i = 0; i < trainInfo.arTlgtItem.GetSize(); ++i)
	{
		trainInfo.arTlgtItem[i].dLength = pUnitCtrl->ConvertUnitLengthIn(trainInfo.arTlgtItem[i].dLength);
		trainInfo.arTlgtItem[i].dForce = pUnitCtrl->ConvertUnitForceIn(trainInfo.arTlgtItem[i].dForce);
	}
}

void CCMTlgtPreviewDlg::InitChart()
{
	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground	*pB = new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *)pB);

	// Add Display Component
	//
	SRGraphDisplay *pDisplay = new SRGraphDisplay;
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetRect(0.2, 0, 100, 97.0);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);


	// Logarithmic Axis Scaling
	//
	pDisplay->GetStyle()->SetLogX(FALSE);
	pDisplay->GetStyle()->SetLog(FALSE);
	m_Graph.AddComponent(pDisplay);


	CString strUnit;
	CString strLabel;

	CUnitCtrl* pUnitCtrl = ((CDBDoc*)CDBDoc::GetDocPoint())->m_pUnitCtrl;
	// Add Label Component for X-Axis
	//
	SRGraphLabel *pLabel = new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
	pLabel->SetTextPosition(50.0, 93.5);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADD2__Time__sec_));

	// Add Label Component for Y-Axis
	//
	pLabel = new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(1.5, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent(pLabel);
	if (pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_FORCE, strUnit))
	{
		strLabel.Format(_T("%s (%s)"), _LS(IDS_CMD_TLGT_FORCE), strUnit);
		SetAxisLabelText(1, strLabel);
	}
	else
	{
		SetAxisLabelText(1, _LS(IDS_CMD_TLGT_FORCE));
	}

	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview
	CWnd* wnd = GetDlgItem(IDC_ETC_TLGT_PLACE_HOLDER);
	CRect rc;
	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	//rc.left += 2;
	//rc.top += 2;
	//rc.right -= 2;
	//rc.bottom -= 2;
	m_GraphView.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rc, this, IDC_ETC_TLGT_PLACE_HOLDER);
	m_GraphView.m_pGraph = &m_Graph;
	m_GraphView.Invalidate();
}

void CCMTlgtPreviewDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup * 2, x);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup * 2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup * 2 + 1, y);
}

void CCMTlgtPreviewDlg::SetText()
{
	this->SetWindowText(_LS(IDS_CMD_TLGT_PREVIEW_TITLE));
}

void CCMTlgtPreviewDlg::SetControl()
{
	InitChart();
	SetListCtrlHeader();
	SetListCtrlItem();
}

void CCMTlgtPreviewDlg::SetListCtrlItem()
{
	long lNodeSize = m_trainItem.arKeyNode.GetSize();
	CArray<T_NODE_K, T_NODE_K> arKeyNode;
	arKeyNode.Copy(m_trainItem.arKeyNode);

	//CArray<T_NODE_K, T_NODE_K> arKeyNodeSorted;
	//arKeyNodeSorted.SetSize(lNodeSize);
	//arKeyNodeSorted[0] = m_trainItem.arKeyNode[0]; // Start Node
	//arKeyNodeSorted[lNodeSize - 1] = m_trainItem.arKeyNode[lNodeSize - 1]; // End Node

	//CArray<double, double> arDistance;
	//CArray<int, int> arIndex;

	//arDistance.SetSize(lNodeSize - 2);
	//arIndex.SetSize(lNodeSize - 2);

	//// 시작노드와의 거리를 기준으로 오름차순 정렬
	//long nCount = 0;
	//for (long i = 0; i < lNodeSize; ++i)
	//{
	//	if (arKeyNode[i] == m_trainItem.arKeyNode[0] || arKeyNode[i] == m_trainItem.arKeyNode[lNodeSize - 1]) continue;
	//	T_NODE_D startNodeData, nodeData;
	//	m_pDoc->m_pAttrCtrl->GetNode(m_trainItem.arKeyNode[0], startNodeData);
	//	m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[i], nodeData);

	//	arDistance[nCount] = m_pDStore->GetNodeDist(startNodeData, nodeData);
	//	arIndex[nCount++] = i;
	//}
	//CQSort::QSortDblWithIndex(arIndex.GetData(), arDistance.GetData(), lNodeSize - 2);


	//for (long i = 0; i < lNodeSize - 2; ++i)
	//	arKeyNodeSorted[i + 1] = arKeyNode[arIndex[i]];

	m_ctrlList.DeleteAllItems();

	CStringArray aContents;
	CString   tmpStr;
	
	long lCount = arKeyNode.GetSize();

	for (long i = 0; i < lCount; ++i)
	{
		aContents.RemoveAll();
		tmpStr.Format(_T("%d"), arKeyNode[i]);
		aContents.Add(tmpStr);
		CDlgUtil::SetListItem(&m_ctrlList, i, aContents, (DWORD)arKeyNode[i]);
	}


	m_ctrlList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	POSITION pos = m_ctrlList.GetFirstSelectedItemPosition();
	if (pos)
	{
		T_NODE_K KeyNode;
		int nIndex = m_ctrlList.GetNextSelectedItem(pos);
		KeyNode = m_ctrlList.GetItemData(nIndex);
		CalcPreview(m_trainItem, KeyNode);
	}
}

void CCMTlgtPreviewDlg::SetListCtrlHeader()
{
	CStringArray aTitles;
	CArray<int, int> aWidths;
	CArray<float, float> aWRatio;
	aTitles.Add(_LS(IDS_CMD_TLGT_PREVIEW_NODE_ID));

	aWRatio.Add((float)0.99 / 1.0);
	CDlgUtil::_SetListCtrlHeader(&m_ctrlList, aTitles, &aWRatio, NULL);



	//// 속성
	//DWORD dwStyle;
	//dwStyle = ListView_GetExtendedListViewStyle(m_ctrlList.GetSafeHwnd());
	//dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;
	//ListView_SetExtendedListViewStyle(m_ctrlList.GetSafeHwnd(), dwStyle);

	//// 사이즈
	//RECT rt;
	//m_ctrlList.GetClientRect(&rt);
	//int width = (rt.right - rt.left)*.999;
	//int nColWidth; // id, length, force
	//CString strTitle = _LS(IDS_CMD_TLGT_PREVIEW_NODE_ID);
	//nColWidth = width;

	//// 타이틀
	//LV_COLUMN lvCol;

	//lvCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	//lvCol.fmt = LVCFMT_LEFT;
	//lvCol.iSubItem = 0;
	//lvCol.cx = nColWidth;
	//lvCol.pszText = strTitle.GetBuffer(0);
	//m_ctrlList.InsertColumn(0, &lvCol);
	//strTitle.ReleaseBuffer();
}

void CCMTlgtPreviewDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel = (SRGraphLabel *)m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

BOOL CCMTlgtPreviewDlg::CalcPreview(TLGT_INFO& trainInfo, T_NODE_K KeyNode)
{
	long lNodeSize = m_trainItem.arKeyNode.GetSize();
	CArray<T_NODE_K, T_NODE_K> arKeyNode;
	arKeyNode.Copy(m_trainItem.arKeyNode);

	CArray<double, double> arElemSize;
	CArray<double, double> arArrivalTime;
	arElemSize.SetSize(lNodeSize);
	arArrivalTime.SetSize(lNodeSize);


	T_NODE_D NodeData1, NodeData2;

	// Velocity 는 km/h 고정단위계로 받아오므로 현재단위계로 변경
	double dVelocity = (M_InitValueCurUnit(trainInfo.dVelocity * 1000, N, M, D_UNITSYS_BASE_LENGTH)); // 1000*m (km)를 현재단위 L로 가져옴. 시간단위는 h
	dVelocity /= 3600.0; // 시간단위를 sec로 변경

	for (long i = 0; i < lNodeSize; ++i)
	{
		if (i == 0)
		{
			arElemSize[i] = 0.0;
			arArrivalTime[i] = 0.0;
			continue;
		}
		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[i - 1], NodeData1);
		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[i], NodeData2);
		arElemSize[i] = m_pDStore->GetNodeDist(NodeData1, NodeData2);

		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[0], NodeData1);
		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[i - 1], NodeData2);
		arArrivalTime[i] = m_pDStore->GetNodeDist(NodeData1, NodeData2) / dVelocity;
	}

	CUnitCtrl* pUnitCtrl = ((CDBDoc*)CDBDoc::GetDocPoint())->m_pUnitCtrl;

	CArray<double, double> arTime, arValue;

	for (long i = 0; i < lNodeSize; ++i)
	{
		if (arKeyNode[i] != KeyNode) continue;

		double dLeftElemSize = 0.0;
		double dRightElemSize = 0.0;

		if (i == 0)
		{
			dLeftElemSize = 0.0;
			dRightElemSize = arElemSize[i + 1];
		}
		else if (i == lNodeSize-1)
		{
			dLeftElemSize = arElemSize[i];
			dRightElemSize = 0.0;
		}
		else
		{
			dLeftElemSize = arElemSize[i];
			dRightElemSize = arElemSize[i + 1];
		}

		m_pDStore->CalcDynLoad(trainInfo, dLeftElemSize, dRightElemSize, arArrivalTime[i], arTime, arValue, dVelocity, TRUE);

		long nSize = arTime.GetSize();
		m_Graph.KillData();
		for (long j = 0; j < nSize; j++)
		{
			arTime[j] = pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_TIME, arTime[j]);
			arValue[j] = pUnitCtrl->ConvertUnitForceOut(arValue[j]);
			SetValue(j, 0, arTime[j], arValue[j]);
		}
		m_GraphView.Invalidate();

		return TRUE;
	}

	return FALSE;
}
