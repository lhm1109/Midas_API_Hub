// Coded by Seungjun MNet:No.2393 ('20060919)
// DgnGenLcomServTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLcomServTypeDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_QSort.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "DgnDataCtrl.h"
#include "..\wg_base\DlgUtil.h"

#define GMsgOutNF GSaveHistoryFormatNF

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLcomServTypeDlg dialog

//(2009.11.17) Modify by Unsang :: bElasticStage -> nServLcomType으로 대체!!
//
// Serviceability Load Combination Type을 정의하는 변수인 bElasticStage는 CVLw에서 하중조합 내 사용되는 변수이므로
// 해당 변수를 표현하기 위한 신규 변수 (nServLcomType)을 추가하여 이를 대체함.
// 

CDgnGenLcomServTypeDlg::CDgnGenLcomServTypeDlg(int nType, CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenLcomServTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenLcomServTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arServLcomMAP.RemoveAll();
	m_arQuasiLcomMAP.RemoveAll();
	m_arFrequentLcomMAP.RemoveAll();
	m_arRareLcomMAP.RemoveAll();
	m_nServTypeCategory = 0;

#if defined _DEBUG
    switch ( nType )
    {
    case D_LCOMTYPE_STEEL:
    case D_LCOMTYPE_CONCRETE:
        break;
    default:
        ASSERT(0);
        break;
    }
#endif
    m_nDgnType = nType;
}


void CDgnGenLcomServTypeDlg::SetServTypeCategory( int nCategory )
{
	m_nServTypeCategory = nCategory;
}

void CDgnGenLcomServTypeDlg::SetLoadCombinationType(int nType)
{
	m_nLoadType = 1;
}

void CDgnGenLcomServTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenLcomServTypeDlg)
	DDX_Control(pDX, IDC_DGN_SERV_RARE_LIST,  m_aRareList    );
	DDX_Control(pDX, IDC_DGN_SERV_QUASI_LIST, m_aQusaiList   );
	DDX_Control(pDX, IDC_DGN_SERV_LCOM_LIST,  m_aServLcomList);
	DDX_Control(pDX, IDC_DGN_SERV_FREQ_LIST,  m_aFrequentList);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenLcomServTypeDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnGenLcomServTypeDlg)
	ON_BN_CLICKED(IDC_DGN_SERV_QUASI_TERM1, OnDgnAddQuasiBtn   )
	ON_BN_CLICKED(IDC_DGN_SERV_QUASI_TERM2, OnDgnDelQuasiBtn   )
	ON_BN_CLICKED(IDC_DGN_SERV_FREQ_TERM1,  OnDgnAddFrequentBtn)
	ON_BN_CLICKED(IDC_DGN_SERV_FREQ_TERM2,  OnDgnDelFrequentBtn)
	ON_BN_CLICKED(IDC_DGN_SERV_RARE_TERM1,  OnDgnAddRareBtn    )
	ON_BN_CLICKED(IDC_DGN_SERV_RARE_TERM2,  OnDgnDelRareBtn    )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLcomServTypeDlg message handlers

void CDgnGenLcomServTypeDlg::OnDgnAddQuasiBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aServLcomList.GetSelectedCount();
	int nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aServLcomList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLcomList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLcomMAP.Lookup(iSeqNum, Load);
			m_arServLcomMAP.RemoveKey(iSeqNum);
			//Load.bElasticStage = 2;
			Load.nServLcomType  = 1;
			m_arQuasiLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aQusaiList, m_arQuasiLcomMAP);
	}
	UpdateData(FALSE);
}

void CDgnGenLcomServTypeDlg::OnDgnDelQuasiBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aQusaiList.GetSelectedCount();
	int  nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aQusaiList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aQusaiList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arQuasiLcomMAP.Lookup(iSeqNum, Load);
			m_arQuasiLcomMAP.RemoveKey(iSeqNum);
			
			//Load.bElasticStage = 0;
			Load.nServLcomType = 0;
			m_arServLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aQusaiList, m_arQuasiLcomMAP);
	}
	UpdateData(FALSE);
	
}

void CDgnGenLcomServTypeDlg::OnDgnAddFrequentBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aServLcomList.GetSelectedCount();
	int nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aServLcomList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLcomList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLcomMAP.Lookup(iSeqNum, Load);
			m_arServLcomMAP.RemoveKey(iSeqNum);
			
			//Load.bElasticStage = 3;
			Load.nServLcomType = 2;
			m_arFrequentLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aFrequentList, m_arFrequentLcomMAP);
	}
	UpdateData(FALSE);
	
}

void CDgnGenLcomServTypeDlg::OnDgnDelFrequentBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aFrequentList.GetSelectedCount();
	int  nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aFrequentList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aFrequentList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arFrequentLcomMAP.Lookup(iSeqNum, Load);
			m_arFrequentLcomMAP.RemoveKey(iSeqNum);
			
			//Load.bElasticStage = 0;
			Load.nServLcomType = 0;
			m_arServLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aFrequentList, m_arFrequentLcomMAP);
	}
	UpdateData(FALSE);
}

void CDgnGenLcomServTypeDlg::OnDgnAddRareBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aServLcomList.GetSelectedCount();
	int nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aServLcomList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aServLcomList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arServLcomMAP.Lookup(iSeqNum, Load);
			m_arServLcomMAP.RemoveKey(iSeqNum);
			
			//Load.bElasticStage = 4;
			Load.nServLcomType = 3;
			m_arRareLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aRareList, m_arRareLcomMAP);
	}
	UpdateData(FALSE);
	
}

void CDgnGenLcomServTypeDlg::OnDgnDelRareBtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iSelectedCount = m_aRareList.GetSelectedCount();
	int  nItem = -1;

	_DGN_LCOM_D Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aRareList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aRareList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arRareLcomMAP.Lookup(iSeqNum, Load);
			m_arRareLcomMAP.RemoveKey(iSeqNum);
			
			//Load.bElasticStage = 0;
			Load.nServLcomType = 0;
			m_arServLcomMAP.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);
		Write_ListCtrl(m_aRareList, m_arRareLcomMAP);
	}
	UpdateData(FALSE);
}

void CDgnGenLcomServTypeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CDgnGenLcomServTypeDlg::OnOK()
{
	// TODO: Add extra validation here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	_DGN_LCOM_D Load;
	T_LCOM_D LcomD;

	int Index = 0;
	POSITION Pos = m_arQuasiLcomMAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arQuasiLcomMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;

		//LcomD.bElasticStage = Load.bElasticStage;
		LcomD.nServLcomType = Load.nServLcomType;
		if(!pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD)) ASSERT(0);
	}

	Pos = m_arFrequentLcomMAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arFrequentLcomMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;

		//LcomD.bElasticStage = Load.bElasticStage;
		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}

	Pos = m_arRareLcomMAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arRareLcomMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;

		//LcomD.bElasticStage = Load.bElasticStage;
		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}

	Pos = m_arServLcomMAP.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LcomD.Initialize();
		m_arServLcomMAP.GetNextAssoc(Pos, Index, Load);
		int CON_OR_STL = (Load.bConc ? D_LCOMTYPE_CONCRETE : D_LCOMTYPE_STEEL); // 20071023 mylee
		if(!pDoc->m_pAttrCtrl->GetLcom(CON_OR_STL, Load.Lcomkey, LcomD)) continue;

		//LcomD.bElasticStage = Load.bElasticStage;
		LcomD.nServLcomType = Load.nServLcomType;
		pDoc->m_pDataCtrl->ModifyLcom(CON_OR_STL, LcomD.LoadCombName, LcomD);
	}

	if(CDBLib::GetConCodeName() != CONCODE_SP_63_13330_2018)
	{
		if(m_arRareLcomMAP.GetCount() < 1)
		{
			GMsgOutNF(_LS(IDS_DB_SERVCHECK_ERR));
		}
	}
	
	CDialogMove::OnOK();
}

BOOL CDgnGenLcomServTypeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	AlignControls();

	UpdateData(FALSE);
	return TRUE;
}

void CDgnGenLcomServTypeDlg::InitCtrl()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

#if defined(_CIVIL)
	T_PSCD_D rData;
	if(m_nServTypeCategory == 1)
	{
		if(pDoc->m_pAttrCtrl->GetDgnPscd(rData))
		{
			if( CDBLib::IsPscCodeLSD_KR(rData.iDgnCode) )
			{
				GetDlgItem(IDC_DGN_SERV_LCOM       )->SetWindowText(_LS(IDS_DGN_SERV_LCOM));
				GetDlgItem(IDC_DGN_SERV_QUASI_LCOM )->SetWindowText(_LS(IDS_DGN_SERV_LCOM_1));
				GetDlgItem(IDC_DGN_SERV_FREQUENT   )->SetWindowText(_LS(IDS_DGN_SERV_LCOM_3_4));
				GetDlgItem(IDC_DGN_SERV_RARE       )->SetWindowText(_LS(IDS_DGN_SERV_LCOM_5));
			}
			else if (rData.iDgnCode == ABNT_NBR_6118_2023_PSC)
			{
				GetDlgItem(IDC_DGN_SERV_RARE)->SetWindowText(_LS(IDS_DGN_SERV_RARE));
			}
		}
	}
#endif

	InitLcomData();

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_aServLcomList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aServLcomList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR* list[2] = {_T("Load Comb"), _T("Seq")};
	int width[2]	 = {120,0};
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aServLcomList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aServLcomList, m_arServLcomMAP);

	//Quasi-permanent
	DWORD QuasiStyle;
	QuasiStyle = ListView_GetExtendedListViewStyle(m_aQusaiList.GetSafeHwnd());
	QuasiStyle |= LVS_EX_FULLROWSELECT; 
	QuasiStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aQusaiList.GetSafeHwnd(),QuasiStyle);

	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aQusaiList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aQusaiList, m_arQuasiLcomMAP);

	//Frequent
	DWORD FrequentStyle;
	FrequentStyle = ListView_GetExtendedListViewStyle(m_aFrequentList.GetSafeHwnd());
	FrequentStyle |= LVS_EX_FULLROWSELECT; 
	FrequentStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aFrequentList.GetSafeHwnd(),FrequentStyle);

	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aFrequentList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aFrequentList, m_arFrequentLcomMAP);

	//Rare
	DWORD RareStyle;
	RareStyle = ListView_GetExtendedListViewStyle(m_aRareList.GetSafeHwnd());
	RareStyle |= LVS_EX_FULLROWSELECT; 
	RareStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aRareList.GetSafeHwnd(),RareStyle);

	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aRareList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aRareList, m_arRareLcomMAP);
}

void CDgnGenLcomServTypeDlg::AlignControls()
{
	const CString& strCodeName = CDBLib::GetConCodeName();
	if(strCodeName == CONCODE_SP_63_13330_2018)
	{
		if(m_nLoadType == 0)
		{
			GetDlgItem(IDC_DGN_SERV_QUASI_LCOM)->SetWindowText(_LS(IDS_DGN_CONC_Serviceability_Normative_short_term));
			GetDlgItem(IDC_DGN_SERV_FREQUENT)->SetWindowText(_LS(IDS_DGN_CONC_Serviceability_Normative_long_term));
		}
		else if(m_nLoadType == 1)
		{
			GetDlgItem(IDC_DGN_SERV_QUASI_LCOM)->SetWindowText(_LS(IDS_DGN_CONC_Strength_Design_short_term));
			GetDlgItem(IDC_DGN_SERV_FREQUENT)->SetWindowText(_LS(IDS_DGN_CONC_Strength_Design_long_term));
			GetDlgItem(IDC_DGN_SERV_LCOM)->SetWindowText(_LS(IDS_DGN_CONC_Strength_IDC_DGN_SERV_LCOM));
			SetWindowText(_LS(IDS_DGN_CONC_Strength_LoadCombination));
		}

		GetDlgItem(IDC_DGN_SERV_RARE_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SERV_RARE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SERV_RARE_TERM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SERV_RARE_TERM2)->ShowWindow(SW_HIDE);

		CRect rRef, rRect, rMove;
		GetDlgItem(IDC_DGN_SERV_LCOM_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_SERV_FREQ_LIST)->GetWindowRect(rRect);
		GetDlgItem(IDC_DGN_SERV_RARE_LIST)->GetWindowRect(rMove);
		rRef.bottom = rRect.bottom;
		ScreenToClient(rRef);
		GetDlgItem(IDC_DGN_SERV_LCOM_LIST)->SetWindowPos(NULL, rRef.left, rRef.top, rRef.Width(), rRef.Height(), SWP_DRAWFRAME);

		int nDistY = rRect.bottom - rMove.bottom;
		CArray<UINT, UINT> aMoveID;
		aMoveID.Add(IDOK);
		aMoveID.Add(IDCANCEL);
		CDlgUtil::CtrlMoveDistY(this, aMoveID, nDistY);

		GetWindowRect(rMove);
		rMove.bottom += nDistY;
		MoveWindow(rMove, TRUE);
	}
}

void CDgnGenLcomServTypeDlg::InitLcomData()
{
	m_arServLcomMAP.RemoveAll();
	m_arQuasiLcomMAP.RemoveAll();
	m_arFrequentLcomMAP.RemoveAll();
	m_arRareLcomMAP.RemoveAll();

	_DGN_LCOM_D DgnLcomD;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CArray<T_LCOM_K, T_LCOM_K> LcomKList;
	LcomKList.RemoveAll();

    switch ( m_nDgnType )
    {
    case D_LCOMTYPE_CONCRETE:
        pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, LcomKList);
        break;
    case D_LCOMTYPE_STEEL:
        pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_STEEL, LcomKList);
        break;
    default:
        ASSERT(0);
        break;
    }
	
    int iSeqNum=0;
	T_LCOM_D LcomD;
	for(int i=0; i<LcomKList.GetSize(); i++)
	{
		T_LCOM_K LcomK = LcomKList.GetAt(i);
		LcomD.Initialize();
	
        if ( m_nDgnType ==  D_LCOMTYPE_CONCRETE )
		{
			if(!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD)) continue;
		}
		else
		{
			if(!pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_STEEL, LcomK, LcomD)) continue;
		}
		
		if(m_nLoadType == 0 && LcomD.nActive != 2)	// service load combination
			continue;
		else if(m_nLoadType == 1 && LcomD.nActive != 1)	// russian use; strength load combination
			continue;

		DgnLcomD.Initialize();
		//if(LcomD.bElasticStage == 2)  // Quasi-permanent
		if(LcomD.nServLcomType == 1)  // Quasi-permanent
		{
			DgnLcomD.Lcomkey = LcomK;
			//DgnLcomD.bElasticStage = LcomD.bElasticStage;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
            DgnLcomD.bConc = m_nDgnType == D_LCOMTYPE_CONCRETE ? TRUE : FALSE;
			m_arQuasiLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
		//else if(LcomD.bElasticStage == 3)  // Frequent
		else if(LcomD.nServLcomType == 2)  // Frequent
		{
			DgnLcomD.Lcomkey = LcomK;
			//DgnLcomD.bElasticStage = LcomD.bElasticStage;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = m_nDgnType == D_LCOMTYPE_CONCRETE ? TRUE : FALSE;
			m_arFrequentLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;    
		}
		//else if(LcomD.bElasticStage == 4)  // Rare
		else if(LcomD.nServLcomType == 3)  // Rare
		{
			DgnLcomD.Lcomkey = LcomK;
			//DgnLcomD.bElasticStage = LcomD.bElasticStage;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = m_nDgnType == D_LCOMTYPE_CONCRETE ? TRUE : FALSE;
			m_arRareLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
		else  // not assigned.
		{
			DgnLcomD.Lcomkey = LcomK;
			//DgnLcomD.bElasticStage = LcomD.bElasticStage;
			DgnLcomD.nServLcomType = LcomD.nServLcomType;
			DgnLcomD.strLoadCombName = LcomD.LoadCombName;
			DgnLcomD.iSeqNum = iSeqNum;
			DgnLcomD.bConc = m_nDgnType == D_LCOMTYPE_CONCRETE ? TRUE : FALSE;
			m_arServLcomMAP.SetAt(iSeqNum, DgnLcomD);
			iSeqNum++;
		}
	}
}

void CDgnGenLcomServTypeDlg::Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap)
{
	LoadList.DeleteAllItems();

	LV_ITEM lvitem;
	LPTSTR szText[2];

	_DGN_LCOM_D Load; Load.Initialize();

	int iLoadCount = LoadMap.GetCount();
	int* pSortLoad = new int[iLoadCount];

	int Index = 0;
	int iCount = 0;
	POSITION Pos = LoadMap.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LoadMap.GetNextAssoc(Pos, Index, Load);
		pSortLoad[iCount] = Index;
		iCount++;
	}

	// Sorting by Ascending order.
	CQSort::QSortInt(pSortLoad, iLoadCount);

	for(iCount=0; iCount<iLoadCount; iCount++)
	{
		Load.Initialize();
		LoadMap.Lookup(pSortLoad[iCount], Load);

		CString strSeq = _T(""); strSeq.Format(_T("%d"), Load.iSeqNum);
	  szText[0] = (LPTSTR)(LPCTSTR)Load.strLoadCombName;
	  szText[1] = (LPTSTR)(LPCTSTR)strSeq;
	  // Input ListBox.
	  for(int i=0; i<2; i++)
	  {
		  lvitem.mask		  = LVIF_TEXT;
		  lvitem.iItem  	= iCount;
		  lvitem.iSubItem = i;
		  lvitem.pszText	= szText[i];
		  if(i==0)	LoadList.InsertItem(&lvitem);
		  else			LoadList.SetItem(&lvitem);
	  }
	}

	delete[] pSortLoad;
}