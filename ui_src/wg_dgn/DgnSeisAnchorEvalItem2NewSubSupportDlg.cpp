// CDgnSeisAnchorEvalItem2NewSubSupportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalItem2NewSubSupportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubSupportDlg dialog

CDgnSeisAnchorEvalItem2NewSubSupportDlg::CDgnSeisAnchorEvalItem2NewSubSupportDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CDgnSeisAnchorEvalItem2NewSubSupportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalItem2NewSubSupportDlg)	
	//}}AFX_DATA_INIT
	m_pParent = (CDgnSeisAnchorEvalItem2NewDlg*)pParent;

	m_pGrid = new CDgnSeisAnchor2SupportGrid(this);
}

void CDgnSeisAnchorEvalItem2NewSubSupportDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalItem2NewSubSupportDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_SUPPORT2_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


void CDgnSeisAnchorEvalItem2NewSubSupportDlg::SetSupportData(T_ANEV_D* pData)
{
	m_pData = pData;
}

void CDgnSeisAnchorEvalItem2NewSubSupportDlg::Data2Dlg() 
{ 
	m_pGrid->SetData2Grid(m_pData->SuppType);

	UpdateData(FALSE);
}

BOOL CDgnSeisAnchorEvalItem2NewSubSupportDlg::Dlg2Data() 
{
	UpdateData(TRUE);

	m_pData->SuppType.Initialize();
	m_pGrid->GetGrid2Data(m_pData->SuppType);

	//////////////////////////////////////////////////////////////////////////
	int nNumX, nNumY;
	CDgnSeisAnchorShapeMgr ShapeMgr;
	ShapeMgr.GetBearingNumberNew2(*m_pData, nNumX, nNumY);
	int nBearing = nNumX;
	int nLayer   = nNumY;

	m_pData->Fail.ConcFailL.aFailSelLayer.RemoveAll();
	m_pData->Fail.ConcFailT.aFailSelLayer.RemoveAll();
	m_pData->Fail.PryoutL.aFailSelLayer.RemoveAll();
	m_pData->Fail.PryoutT.aFailSelLayer.RemoveAll();

	for (int j = 0; j < m_pData->SuppType.aLayer.GetSize(); j++)
	{
		int nSize = m_pData->SuppType.aLayer[j].aBrevType.GetSize();
		ASSERT(nBearing == nSize); // 다르면 확인하기!

		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerConcL; FailSelLayerConcL.aFailSel.SetSize(nSize);
		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerConcT; FailSelLayerConcT.aFailSel.SetSize(nSize);
		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerPrytL; FailSelLayerPrytL.aFailSel.SetSize(nSize);
		T_ANEV_FAIL_SEL_LAYER_D FailSelLayerPrytT; FailSelLayerPrytT.aFailSel.SetSize(nSize);

		for (int k = 0; k < nSize; k++)
		{
			// 0:고정단, 1:양방향 가동단, 2:교축 가동단, 3:교직 가동단
			int nBrevType = m_pData->SuppType.aLayer[j].aBrevType[k];

			FailSelLayerConcL.aFailSel[k] = (nBrevType == 0 || nBrevType == 3) ? TRUE : FALSE;
			FailSelLayerConcT.aFailSel[k] = (nBrevType == 0 || nBrevType == 2) ? TRUE : FALSE;
			FailSelLayerPrytL.aFailSel[k] = (nBrevType == 0 || nBrevType == 3) ? TRUE : FALSE;
			FailSelLayerPrytT.aFailSel[k] = (nBrevType == 0 || nBrevType == 2) ? TRUE : FALSE;
		}

		m_pData->Fail.ConcFailL.aFailSelLayer.Add(FailSelLayerConcL);
		m_pData->Fail.ConcFailT.aFailSelLayer.Add(FailSelLayerConcT);
		m_pData->Fail.PryoutL.aFailSelLayer.Add(FailSelLayerPrytL);
		m_pData->Fail.PryoutT.aFailSelLayer.Add(FailSelLayerPrytT);

	}
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDgnSeisAnchorEvalItem2NewSubSupportDlg::SetViewData()
{
	Dlg2Data();
	m_pParent->SetViewData();
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalItem2NewSubSupportDlg, CChildDialog)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalItem2NewSubSupportDlg message handlers

BOOL CDgnSeisAnchorEvalItem2NewSubSupportDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	m_pGrid->Initialize();

	Data2Dlg();
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

