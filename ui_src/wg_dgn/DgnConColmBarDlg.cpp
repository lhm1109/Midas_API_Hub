// DgnConColmBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConColmBarDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmBarDlg dialog


CDgnConColmBarDlg::CDgnConColmBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConColmBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConColmBarDlg)
	m_strNqrb = _T("");
	m_strNrow = _T("");
	//}}AFX_DATA_INIT
}


void CDgnConColmBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConColmBarDlg)
	DDX_Control(pDX, IDC_DGN_RC_SUBSPACE, m_SpaceCtrl);
	DDX_Control(pDX, IDC_DGN_RC_SUBSPACE_YNUM, m_SpaceYNumCtrl);
	DDX_Control(pDX, IDC_DGN_RC_SUBSPACE_ZNUM, m_SpaceZNumCtrl);
	DDX_Control(pDX, IDC_DGN_COLBARSIZE_COMBO, m_BarSize);
	DDX_Control(pDX, IDC_DGN_RC_NROWSPIN, m_NrowSpin);
	DDX_Control(pDX, IDC_DGN_RC_NQRBSPIN, m_NqrbSpin);
	DDX_Control(pDX, IDC_DGN_RC_CD0UNIT, m_D0Unit);
	DDX_Text(pDX, IDC_DGN_RC_NQRB, m_strNqrb);
	DDX_Text(pDX, IDC_DGN_RC_NROW, m_strNrow);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConColmBarDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConColmBarDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_RC_NQRBSPIN, OnDeltaposDgnRcNqrbspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_RC_NROWSPIN, OnDeltaposDgnRcNrowspin)
	ON_EN_CHANGE(IDC_DGN_RC_CD0, OnChangeDgnRcCd0)
	ON_EN_CHANGE(IDC_DGN_RC_NQRB, OnChangeDgnRcNqrb)
	ON_EN_CHANGE(IDC_DGN_RC_NROW, OnChangeDgnRcNrow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConColmBarDlg message handlers


BOOL CDgnConColmBarDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	if(m_BarSize.GetCount() > 0)	m_BarSize.ResetContent();
	CDBLib::GetRebarNameAtComboBox(&m_BarSize);

	if(m_SpaceYNumCtrl.GetCount() > 0)	m_SpaceYNumCtrl.ResetContent();
	CStringArray arSubRebarNum;
	CDBLib::GetSubRebarNumList(arSubRebarNum);
	for(int i=0; i<arSubRebarNum.GetSize(); i++) m_SpaceYNumCtrl.AddString(arSubRebarNum[i]);
		
	if(m_SpaceZNumCtrl.GetCount() > 0)	m_SpaceYNumCtrl.ResetContent();
	for(int i=0; i<arSubRebarNum.GetSize(); i++) m_SpaceZNumCtrl.AddString(arSubRebarNum[i]);
	

	// Initialize Window.
	CWnd* PlaceHolder0 = GetDlgItem(IDC_STATIC_VIEW);
	ASSERT(PlaceHolder0);
	m_SectWnd.m_SectionType = eDGN_COLM;
	m_SectWnd.Initial(PlaceHolder0);

	if(m_ElemType==eDGN_COLM)
	{
		T_REBC_D cData;
		cData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetRebc(m_RchkKey,cData);
		if(bCheck)  Initial_cExistData(cData);
		else        Initial_WithoutData();
	}
	else if(m_ElemType==eDGN_BRCE)
	{
		T_REBR_D brData;
		brData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetRebr(m_RchkKey,brData);
		if(bCheck)  Initial_brExistData(brData);
		else        Initial_WithoutData();
	}

	// Initialize Default Value...
	Initial_Data();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConColmBarDlg::Initial_brExistData(T_REBR_D Data)
{
	// Initialize Data.
	m_nNqrb = Data.nQrb;
	m_nNrow = Data.nRow;
	m_strNqrb.Format(_T("%d"),m_nNqrb);
	m_strNrow.Format(_T("%d"),m_nNrow);
	// Initialize Bar Size by Default.
	int Index = m_BarSize.FindStringExact(-1,Data.MainRebarName);
	m_BarSize.SetCurSel(Index);

	m_iSpaceYNum = Data.iSubRebarNum[0];
	CString strSubRebarNum=_T("");
	strSubRebarNum.Format(_T("%d"), m_iSpaceYNum);
	Index = m_SpaceYNumCtrl.FindStringExact(-1, strSubRebarNum);
	m_SpaceYNumCtrl.SetCurSel(Index);

	m_iSpaceZNum = Data.iSubRebarNum[1];
	strSubRebarNum.Format(_T("%d"), m_iSpaceZNum);
	Index = m_SpaceZNumCtrl.FindStringExact(-1, strSubRebarNum);
	m_SpaceZNumCtrl.SetCurSel(Index);


	m_nD0 = Data.dConcCenter;
	m_SpaceCtrl.SetEditUnit(m_Space);
}

void CDgnConColmBarDlg::Initial_cExistData(T_REBC_D Data)
{
	// Initialize Data.
	m_nNqrb = Data.nQrb;
	m_nNrow = Data.nRow;
	m_strNqrb.Format(_T("%d"),m_nNqrb);
	m_strNrow.Format(_T("%d"),m_nNrow);
	// Initialize Bar Size by Default.
	int Index = m_BarSize.FindStringExact(-1,Data.MainRebarName);
	m_BarSize.SetCurSel(Index);

	m_iSpaceYNum = Data.iSubRebarNum[0];
	m_iSpaceZNum = Data.iSubRebarNum[1];
	CString strSubRebarNum=_T("");
	strSubRebarNum.Format(_T("%d"), m_iSpaceYNum);
	Index = m_SpaceYNumCtrl.FindStringExact(-1, strSubRebarNum);
	m_SpaceYNumCtrl.SetCurSel(Index);

	strSubRebarNum.Format(_T("%d"), m_iSpaceZNum);
	Index = m_SpaceZNumCtrl.FindStringExact(-1, strSubRebarNum);
	m_SpaceZNumCtrl.SetCurSel(Index);

	m_nD0 = Data.dConcCenter;
	m_SpaceCtrl.SetEditUnit(m_Space);
}

void CDgnConColmBarDlg::Initial_WithoutData()
{
	// Initialize Data by Default.
	if(m_ColShape==_T("SB"))      
	{
		m_nNqrb = 4;
		m_nNrow = 2;
		m_strNqrb.Format(_T("%d"),m_nNqrb);
		m_strNrow.Format(_T("%d"),m_nNrow);
	}
	else if(m_ColShape==_T("SR"))
	{
		m_nNqrb = 6;
		m_nNrow = 0;
		m_strNqrb.Format(_T("%d"),m_nNqrb);
		m_strNrow.Format(_T("%d"),m_nNrow);
	}
	else if(m_ColShape==_T("P"))
	{
		m_nNqrb = 6;
		m_nNrow = 0;
		m_strNqrb.Format(_T("%d"),m_nNqrb);
		m_strNrow.Format(_T("%d"),m_nNrow);
	}
	else ASSERT(0);

	// Initialize Bar Size by Default.
	CString strMainRebarSize=_T("");
	if(m_ElemType==eDGN_COLM)	CDBLib::GetDefaultMainRebarSize(2, strMainRebarSize);	// Column.
	else											CDBLib::GetDefaultMainRebarSize(3, strMainRebarSize);	// Brace.
	int Index = m_BarSize.FindStringExact(-1, strMainRebarSize);
	m_BarSize.SetCurSel(Index);

	CDBLib::GetDefaultSubRebarNum(m_iSpaceYNum);
	CDBLib::GetDefaultSubRebarNum(m_iSpaceZNum);
	CString strSubRebarNum=_T("");
	if(m_ColShape==_T("SB"))
	{
		// Y axis
		strSubRebarNum.Format(_T("%d"), m_iSpaceYNum);
	  Index = m_SpaceYNumCtrl.FindStringExact(-1, strSubRebarNum);
	  m_SpaceYNumCtrl.SetCurSel(Index);  
		// Z axis
		strSubRebarNum.Format(_T("%d"), m_iSpaceZNum);
	  Index = m_SpaceZNumCtrl.FindStringExact(-1, strSubRebarNum);
	  m_SpaceZNumCtrl.SetCurSel(Index);
	}
	else if(m_ColShape==_T("SR") || m_ColShape==_T("P"))
	{
		// Y axis
		strSubRebarNum.Format(_T("%d"), m_iSpaceYNum);
	  Index = m_SpaceYNumCtrl.FindStringExact(-1, strSubRebarNum);
	  m_SpaceYNumCtrl.SetCurSel(Index);  
	}

	m_SpaceCtrl.SetEditUnit(m_Space);
	if(m_nD0==0.0)	m_nD0 = Get_SectionDimension();
}

double CDgnConColmBarDlg::Get_SectionDimension()
{
	T_SECT_D rData;
	rData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey,rData))	return 1.0;

	double H		= rData.SectBefore.SectI.Size[0];
	double B		= rData.SectBefore.SectI.Size[1];
	double maxN	= max(H,B);

	double Factor	= Get_ChangeUnitFactor();
	double ComN		= 6.35*Factor;

	maxN = max(maxN,ComN);
	double Max = 7.62*Factor;
	if(maxN >= Max) maxN = Max;

	return maxN;
}

double CDgnConColmBarDlg::Get_ChangeUnitFactor()
{
	// cm.
	T_UNIT_INDEX CurrentUnitIndex;
	// Get Current Unit Index.
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrentUnitIndex);

	double dLenFactor=0.0;
	if(CurrentUnitIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_MM)			dLenFactor = 10.0;
	else if(CurrentUnitIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_CM)	dLenFactor = 1.0;
	else if(CurrentUnitIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_M)	dLenFactor = 0.01;
	else if(CurrentUnitIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_IN)	dLenFactor = 1.0/2.54;
	else if(CurrentUnitIndex.nBase_Length==D_UNITSYS_LENGTH_INDEX_FT)	dLenFactor = 1.0/30.48;

	return dLenFactor;
}

void CDgnConColmBarDlg::Initial_Data()
{
	T_SECT_D rData;
	rData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey,rData))	return;

	CString strSubRebarNum=_T("");
	strSubRebarNum.Format(_T("%d"), m_iSpaceYNum);
	int iIndex = m_SpaceYNumCtrl.FindStringExact(-1, strSubRebarNum);
	m_SpaceYNumCtrl.SetCurSel(iIndex);

	strSubRebarNum.Format(_T("%d"), m_iSpaceZNum);
	iIndex = m_SpaceZNumCtrl.FindStringExact(-1, strSubRebarNum);
	m_SpaceZNumCtrl.SetCurSel(iIndex);

	if(m_ColShape==_T("SB"))
	{
		m_Hc = rData.SectBefore.SectI.Size[0];
		m_Bc = rData.SectBefore.SectI.Size[1];
		Show_RectColumnShape();
		m_SectWnd.Column_DrawData(eDGN_RECT,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
		m_SectWnd.Invalidate(TRUE);
	}
	else if(m_ColShape==_T("SR"))
	{
		m_Hc = rData.SectBefore.SectI.Size[0];
		Show_CircColumnShape();
		m_SectWnd.Column_DrawData(eDGN_CIR,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
		m_SectWnd.Invalidate(TRUE);
	}
	else if(m_ColShape==_T("P"))
	{
		m_Hc = rData.SectBefore.SectI.Size[0];
		m_Bc = rData.SectBefore.SectI.Size[1];
		Show_CircColumnShape();
		m_SectWnd.Column_DrawData(eDGN_PIPE,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
		m_SectWnd.Invalidate(TRUE);
	}

	m_SpaceCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_D0Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	// Initialize Spin Button.
	m_NrowSpin.SetRange(1,200);
	m_NqrbSpin.SetRange(1,400);
}

void CDgnConColmBarDlg::Show_RectColumnShape()
{
	GetDlgItem(IDC_DGN_COLNROW_TITLE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLNROW_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_NROW)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_NROWSPIN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_COLNROW_COLON)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_SUBSPACE_YNUM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_SUBSPACE_ZNUM)->ShowWindow(SW_SHOW);
}

void CDgnConColmBarDlg::Show_CircColumnShape()
{
	GetDlgItem(IDC_DGN_COLNROW_TITLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_COLNROW_STATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_NROW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_NROWSPIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_COLNROW_COLON)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_RC_SUBSPACE_YNUM)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_RC_SUBSPACE_ZNUM)->EnableWindow(FALSE);
}

void CDgnConColmBarDlg::OnDeltaposDgnRcNqrbspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str=_T("");
	GetDlgItem(IDC_DGN_RC_NQRB)->GetWindowText(str);

	int Factor = (int)(pNMUpDown->iDelta);
	m_NqrbSpinNo = _ttoi(str)+2*Factor;

	if(m_ColShape==_T("SB"))      
	{
		if(m_NqrbSpinNo < 4)
		{
			if(Factor==1)	m_NqrbSpinNo = 4;
			else					m_NqrbSpinNo = 400;
		}
		else if(m_NqrbSpinNo > 400)  m_NqrbSpinNo = 4;
	}
	else if(m_ColShape==_T("SR") || m_ColShape==_T("P"))
	{
		if(m_NqrbSpinNo < 6)
		{
			if(Factor==1)	m_NqrbSpinNo = 6;
			else					m_NqrbSpinNo = 400;
		}
		else if(m_NqrbSpinNo > 400)  m_NqrbSpinNo = 6;
	}
	str.Format(_T("%d"),m_NqrbSpinNo);
	GetDlgItem(IDC_DGN_RC_NQRB)->SetWindowText(str);
	
	*pResult = 0;
}

void CDgnConColmBarDlg::OnDeltaposDgnRcNrowspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str=_T("");
	GetDlgItem(IDC_DGN_RC_NROW)->GetWindowText(str);

	int Factor = (int)(pNMUpDown->iDelta);
	m_NrowSpinNo = _ttoi(str)+Factor;

	if(m_NrowSpinNo < 2)
	{
		if(Factor==1)	m_NrowSpinNo = 2;
		else					m_NrowSpinNo = 200;
	}
	else if(m_NrowSpinNo > 200)  m_NrowSpinNo = 2;

	str.Format(_T("%d"),m_NrowSpinNo);
	GetDlgItem(IDC_DGN_RC_NROW)->SetWindowText(str);

	*pResult = 0;
}

void CDgnConColmBarDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	BOOL bCheck1=TRUE;
	m_nNqrb = _ttoi(m_strNqrb);
	double dRem = fmod(m_nNqrb,2.0);
	if(dRem==0.0)
	{
		int Index	=	m_BarSize.GetCurSel();
		if(Index!=-1)	m_BarSize.GetLBText(Index,m_strBarSize);
		else					CDBLib::GetDefaultMainRebarSize(2, m_strBarSize);
		m_nNrow = _ttoi(m_strNrow);
	}
	else	bCheck1 = FALSE;

	BOOL bCheck2=TRUE;
	m_Space = m_SpaceCtrl.GetEditValue();
	if(m_Space <= 0.) bCheck2 = FALSE;

	BOOL bCheck3=TRUE;
	CString strSubRebarNum=_T("");
	int Index=0;
	if(m_ColShape==_T("SB"))
	{
		Index = m_SpaceYNumCtrl.GetCurSel();
	  if(Index != -1) m_SpaceYNumCtrl.GetLBText(Index, strSubRebarNum);
		else            bCheck3 = FALSE;
	  m_iSpaceYNum = _ttoi(strSubRebarNum);
	  if(m_iSpaceYNum < 2)	bCheck3 = FALSE;

		strSubRebarNum=_T("");
		Index = m_SpaceZNumCtrl.GetCurSel();
	  if(Index != -1) m_SpaceZNumCtrl.GetLBText(Index, strSubRebarNum);
		else            bCheck3 = FALSE;
	  m_iSpaceZNum = _ttoi(strSubRebarNum);
	  if(m_iSpaceZNum < 2)	bCheck3 = FALSE;
	}
	else if(m_ColShape==_T("SR") || m_ColShape==_T("P"))
	{
		Index = m_SpaceYNumCtrl.GetCurSel();
	  if(Index != -1) m_SpaceYNumCtrl.GetLBText(Index, strSubRebarNum);
		else            bCheck3 = FALSE;
	  m_iSpaceYNum = _ttoi(strSubRebarNum);
	  if(m_iSpaceYNum < 2)	bCheck3 = FALSE;
	}

	// Error Message.
	CString strMsg=_T("");
	if(!bCheck1)	strMsg.Format(_T("%s\n"), _LS(IDS_DGN_CHK_NQRB_NUMBER));
	if(!bCheck2)	strMsg.Format(_T("%s\n"), _LS(IDS_DGN_CHK_TIE_SPIRAL_SPACE));
	if(!bCheck3)	strMsg.Format(_T("%s\n"), _LS(IDS_DGN_CHK_TIE_SPIRAL_NUM));

	if(bCheck1 && bCheck2 && bCheck3)	CDialogMove::OnOK();
	else															AfxMessageBox(strMsg,MB_OK);
}

void CDgnConColmBarDlg::OnChangeDgnRcCd0() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nNrow = _ttoi(m_strNrow);
	m_nNqrb = _ttoi(m_strNqrb); 

	if(m_nD0 < 0)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_D0_CHK1),MB_OK);
		m_nD0 = 6.*Get_ChangeUnitFactor();
	}
	else
	{
		double minLenn=0.0;
		if(m_ColShape==_T("SB"))			minLenn = min(m_Hc,m_Bc)/2.;
		else if(m_ColShape==_T("SR"))	minLenn = m_Hc/2.;
		else if(m_ColShape==_T("P"))	minLenn = m_Hc/2.;

		if(m_nD0 >= minLenn)
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_D0_CHK2),MB_OK);
			m_nD0 = 6.*Get_ChangeUnitFactor();

			UpdateData(FALSE);
		}
	}
	if(m_ColShape==_T("SB"))			m_SectWnd.Column_DrawData(eDGN_RECT,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
	else if(m_ColShape==_T("SR"))	m_SectWnd.Column_DrawData(eDGN_CIR,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
	else if(m_ColShape==_T("P"))	m_SectWnd.Column_DrawData(eDGN_PIPE,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
	else ASSERT(0);
	m_SectWnd.Invalidate(TRUE);
}

void CDgnConColmBarDlg::OnChangeDgnRcNqrb() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nNrow = _ttoi(m_strNrow);
	m_nNqrb = _ttoi(m_strNqrb); 

	int Nrow2 = m_nNrow*2;
	if(Nrow2 > m_nNqrb)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_NROW_CHK1),MB_OK);
		m_nNqrb = m_nNrow*2;
		m_strNqrb.Format(_T("%d"),m_nNqrb);

		UpdateData(FALSE);
	}
	if(m_ColShape==_T("SB"))			m_SectWnd.Column_DrawData(eDGN_RECT,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
	else if(m_ColShape==_T("SR"))	m_SectWnd.Column_DrawData(eDGN_CIR,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
	else if(m_ColShape==_T("P"))	m_SectWnd.Column_DrawData(eDGN_PIPE,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
	else ASSERT(0);
	m_SectWnd.Invalidate(TRUE);
}

void CDgnConColmBarDlg::OnChangeDgnRcNrow() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nNrow = _ttoi(m_strNrow);
	m_nNqrb = _ttoi(m_strNqrb); 

	if(m_nNrow < 2)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_NROW_CHK2),MB_OK);
		m_nNrow = 2;
		m_strNrow.Format(_T("%d"),m_nNrow);

		UpdateData(FALSE);
	}
	else
	{
		int Nrow2 = m_nNrow*2;
		if(Nrow2 > m_nNqrb)
		{
	    AfxMessageBox(_LS(IDS_DGN_CHK_NROW_CHK1),MB_OK);
			m_nNrow = m_nNqrb/2;
			m_strNrow.Format(_T("%d"),m_nNrow);

			UpdateData(FALSE);
		}
	}
	m_SectWnd.Column_DrawData(eDGN_RECT,m_Hc,m_Bc,m_nNqrb,m_nNrow,m_nD0);
	m_SectWnd.Invalidate(TRUE);
}
