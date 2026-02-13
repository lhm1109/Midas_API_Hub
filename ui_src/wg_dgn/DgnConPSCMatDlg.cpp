// DgnConPscMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConPscMatDlg.h"

#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DgnConPscMatDlg dialog


CDgnConPscMatDlg::CDgnConPscMatDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConPscMatDlg::IDD, pParent)
{
	m_Fci = 0.0;
	m_Fck = 0.0;
	m_Age = 0.0;
	m_bCalc = FALSE;
	m_iCement = 0;
	m_strDgnCode = _T("");
	//{{AFX_DATA_INIT(DgnConPscMatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDgnConPscMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DgnConPscMatDlg)
	DDX_Control(pDX, IDC_DGN_CONC_PSCMAT_FCIUNIT, m_FciUnit);
	DDX_Control(pDX, IDC_DGN_CONC_PSCMAT_LIST, m_ListCtrl);
	DDX_Check(pDX, IDC_DGN_CONC_PSCMAT_CALCCHK, m_bCalc);
	DDX_Text(pDX, IDC_DGN_CONC_PSCMAT_FCI, m_Fci);
	DDX_Text(pDX, IDC_DGN_CONC_PSCMAT_AGE, m_Age);
	DDX_Radio(pDX, IDC_DGN_CONC_PSCMAT_CEMENT1, m_iCement);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConPscMatDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConPscMatDlg)
	ON_NOTIFY(NM_CLICK, IDC_DGN_CONC_PSCMAT_LIST, OnClickDgnConcPscMatList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_CONC_PSCMAT_LIST, OnItemChangedDgnConcPscMatList)
	ON_BN_CLICKED(IDC_DGN_CONC_PSCMAT_MODIFY, OnDgnConcPscMatModify)
	ON_BN_CLICKED(IDC_DGN_CONC_PSCMAT_CALCBTTN, OnDgnConcPscMatCalculate)
	ON_BN_CLICKED(IDC_DGN_CONC_PSCMAT_CALCCHK, OnDgnConcPscMatCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DgnConPscMatDlg message handlers
BOOL CDgnConPscMatDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	Init_Data();

	UpdateData(FALSE);

	return TRUE;
}

void CDgnConPscMatDlg::OnCancel() 
{
	// TODO: Psc extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnConPscMatDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialogMove::OnOK();
}

void CDgnConPscMatDlg::OnDgnConcPscMatCheck()
{
	UpdateData(TRUE);

	if(m_bCalc==TRUE)
		EnableEditBox(TRUE);
	else
		EnableEditBox(FALSE);

	UpdateData(FALSE);

}

void CDgnConPscMatDlg::OnClickDgnConcPscMatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnItemChangedDgnConcPscMatList(pNMHDR,pResult);
	*pResult = 0;
}

void CDgnConPscMatDlg::OnItemChangedDgnConcPscMatList(NMHDR* pNMHDR, LRESULT* pResult) 
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);

	int nItem = pNMListView->iItem;
	CString strID = m_ListCtrl.GetItemText(nItem,0);

	T_MATD_D mData;
	mData.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetMatlDesign(_ttoi(strID),mData))
	{
		m_Fck = mData.Data1.Design.C_fc;
		m_Fci = mData.Data1.Design.C_fci;
		ChangeDlgItemInfo();

		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CDgnConPscMatDlg::OnDgnConcPscMatCalculate()
{
	UpdateData(TRUE);

	if(m_bCalc)
	{
		CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);
	  if(m_Age <= 0.0)	
		{
			strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_PSCMATL_AGE_ERROR);
			AfxMessageBox(strMsg, MB_OK);
			return;  
		}
	}

	double dFci = CalcInitConcStrength(m_Fck,m_strDgnCode);
	m_Fci = dFci; 
	UpdateData(FALSE);
	
	return;    
}

void CDgnConPscMatDlg::OnDgnConcPscMatModify() 
{

	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_MATL));
		return;
	}

	CString strID=_T(""), strName=_T(""), strCode=_T("");
	CString strFc=_T(""), strFci=_T("");

	UpdateData(TRUE);
	if(ErrorCheck())
	{
		strID = m_ListCtrl.GetItemText(nItem,0);
		// Modify Material ID
		int nID = _ttoi(strID);
		T_MATD_D mData;
		mData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetMatlDesign(nID,mData);

		mData.Data1.Design.C_fci	= m_Fci;
		strFci.Format(_T("%g"),mData.Data1.Design.C_fci);
		strName = mData.Name;
		strFc.Format(_T("%g"),mData.Data1.Design.C_fc);

		bCheck = m_pDoc->m_pDataCtrl->AddMatd(nID,mData);
		if(bCheck)
		{
			m_ListCtrl.SetItemText(nItem,1,strName);	// Name.
			m_ListCtrl.SetItemText(nItem,2,strFc);		// fc'/fck.
			m_ListCtrl.SetItemText(nItem,3,strFci);	  // fci.
			m_ListCtrl.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	UpdateData(FALSE);

}

BOOL CDgnConPscMatDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	CString strMsg = _LS(IDS_DGN_CHK_TITLE_ERROR);

	if(m_Fci <= 0.0 || m_Fci > m_Fck)	
	{
		bCheck = FALSE;	
		strMsg = strMsg + _T("\n") + _LS(IDS_DGN_CHK_PSCMATL_FCI_ERROR);
	}

	if(!bCheck)	AfxMessageBox(strMsg, MB_OK);

	return bCheck;
}

void CDgnConPscMatDlg::Init_Data()
{
	m_strDgnCode = CDBLib::GetConCodeName();
	m_bCalc = FALSE;
	if(m_strDgnCode != _T("KCI-USD99"))
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CALCCHK)->EnableWindow(FALSE);

	// Initialize Grid, Unit, Matl Code.
	Init_GridData();
	Init_Unit();
	EnableEditBox(m_bCalc);
}

void CDgnConPscMatDlg::Init_GridData()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR* list[CONST_DGN_PSC_iGRIDCOLM] = {_T("ID"),
																		 (LPTSTR)_LS(IDS_DGN_LIST_NAME).GetBuffer(),
																		 _T("fc|fck|R"),
																		 _T("fci")};
	int width[CONST_DGN_PSC_iGRIDCOLM]	 = {60,95,85,85};
	for(int i=0; i<CONST_DGN_PSC_iGRIDCOLM; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_ListCtrl.InsertColumn(i,&lvcolumn);
	}
	// Set Design Material Data using Analysis Material Data.
	CArray<T_MATL_K, T_MATL_K> arKey;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(arKey);
	// Arrange Data for initializing Material Data.
	T_MATD_D mData;
	int iListNo = 0;
	for(int i=0; i<arKey.GetSize(); i++)
	{
		int iID = arKey.GetAt(i);
		mData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(iID, mData))	return;
		if(mData.Type==_T("C"))
		{
			CString strID=_T(""), strFc=_T(""), strFci=_T("");
			strID.Format(_T("%7d"), iID);
			CString strNa	= mData.Name;
			strFc.Format(_T("%g"),  mData.Data1.Design.C_fc);
			strFci.Format(_T("%g"),  mData.Data1.Design.C_fci);

			Write_ListBox(iListNo,strID,strNa,strFc,strFci);
			iListNo++;
		}
	}
}

void CDgnConPscMatDlg::Init_Unit()
{
	m_FciUnit.SetUnitType(D_UNITSYS_BASE_STRESS);
}

void CDgnConPscMatDlg::EnableEditBox(BOOL bCheck)
{
	if(bCheck)
	{
		GetDlgItem(IDC_DGN_CONC_PSCMAT_AGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CALCBTTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT4)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CONC_PSCMAT_AGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CALCBTTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_PSCMAT_CEMENT4)->EnableWindow(FALSE);
	}
}

void CDgnConPscMatDlg::Write_ListBox(int iIndex, CString strID, CString strNa, CString strFc, CString strFci)
{

	LV_ITEM lvitem;
	LPTSTR szText[CONST_DGN_PSC_iGRIDCOLM];
	szText[0] = (LPTSTR)(LPCTSTR)strID;
	szText[1] = (LPTSTR)(LPCTSTR)strNa;
	szText[2] = (LPTSTR)(LPCTSTR)strFc;
	szText[3] = (LPTSTR)(LPCTSTR)strFci;
	// Input ListBox.
	for(int i=0; i<CONST_DGN_PSC_iGRIDCOLM; i++)
	{
		lvitem.mask		  = LVIF_TEXT;
		lvitem.iItem  	= iIndex;
		lvitem.iSubItem = i;
		lvitem.pszText	= szText[i];
		if(i==0)	m_ListCtrl.InsertItem(&lvitem);
		else			m_ListCtrl.SetItem(&lvitem);
	}

}

double CDgnConPscMatDlg::CalcInitConcStrength(double dFck, CString strDgnCode)
{
	double dFci = 0.0;

	if(strDgnCode==_T("KCI-USD99"))
	{
		double dBsc=0.0;
		switch(m_iCement)
		{
			case 0: case 3:// Type 1,5 Cement
				dBsc = 0.25;
				break;
			case 1:        // Type 2 Cement  
				dBsc = 0.38;
				break;
			case 2:        // Type 3 Cement
				dBsc = 0.20;
				break;
		}
		double dBcc = (m_Age <=0.0 ? 1 : exp(dBsc*(1-sqrt(28./m_Age))));
		dFci = dBcc*dFck;
	}
	else if(strDgnCode==_T("ACI318-99") || strDgnCode==_T("ACI318-02"))
	{
	}
	else ASSERT(0);

	return dFci;
}

void CDgnConPscMatDlg::ChangeDlgItemInfo()
{
	m_iCement = 0;
}
