#include "stdafx.h"
#include "wg_cmd.h"
#include "SpfcEnvelopeDlg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 1

CSpfcEnvelopeDlg::CSpfcEnvelopeDlg(CWnd* pParent) : CDialogMove(CSpfcEnvelopeDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
}

void CSpfcEnvelopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPFC_LIST1, m_List1);
	DDX_Control(pDX, IDC_SPFC_LIST2, m_List2);
}

BEGIN_MESSAGE_MAP(CSpfcEnvelopeDlg, CDialogMove)
	ON_BN_CLICKED(IDC_SPFC_BTN_ADD, OnSpfcBtnAdd)
	ON_BN_CLICKED(IDC_SPFC_BTN_DELETE, OnSpfcBtnDel)
	ON_BN_CLICKED(IDC_SPFC_BTN_APPLY, OnSpfcBtnApply)
	ON_BN_CLICKED(IDC_SPFC_BTN_OK, OnSpfcBtnOK)
	ON_BN_CLICKED(IDC_SPFC_BTN_CLOSE, OnSpfcBtnClose)
END_MESSAGE_MAP()

BOOL CSpfcEnvelopeDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	SetHeaderTitle(true);
	InitSpfcList();
	return TRUE;
}

void CSpfcEnvelopeDlg::SetHeaderTitle(bool bInit)
{
	SetHeaderTitle(m_List1, bInit);
	SetHeaderTitle(m_List2, bInit);
}

void CSpfcEnvelopeDlg::SetHeaderTitle(CListCtrl& rListCtrl, bool bInit)
{
	if ( bInit )
	{
		DWORD dwStyle = ListView_GetExtendedListViewStyle(rListCtrl.GetSafeHwnd());
		dwStyle |= LVS_EX_FULLROWSELECT;
		ListView_SetExtendedListViewStyle(rListCtrl.GetSafeHwnd(), dwStyle);
	}

	int nColWidth[COLCOUNT] = { 0, };
	nColWidth[0] = 170;

	CString aTitle[] = { _LS(IDS_CMD_SPFC_spectrum_name), _LS(IDS_CMD_SPFC_type) };
	for ( int i = 0; i < COLCOUNT; i++ )
	{
		CString csTitle = aTitle[i];

		LV_COLUMN lvcolumn;
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText  = csTitle.GetBuffer(0);
		if ( bInit )
		{
			rListCtrl.InsertColumn(i, &lvcolumn);
		}
		rListCtrl.SetColumn(i, &lvcolumn);
		csTitle.ReleaseBuffer();
	}
}

void CSpfcEnvelopeDlg::InitSpfcList()
{
	CArray<CString, CString&> aSpfc;
	m_pDoc->m_pAttrCtrl->GetSpfcList(aSpfc);

	INT_PTR nSpfc = aSpfc.GetCount();
	if ( nSpfc < 2 ) { ASSERT(0); return; }

	for ( INT_PTR i = 0; i < nSpfc; ++i )
	{
		AddListItem(m_List1, aSpfc[i]);
	}
}

void CSpfcEnvelopeDlg::AddListItem(CListCtrl& rListCtrl, CString csName)
{
	int nCount = rListCtrl.GetItemCount();
	for ( int i = 0; i < nCount; ++i )
	{
		CString csText = rListCtrl.GetItemText(i, 0);
		if ( csText == csName )
		{
			return;
		}
	}

	LV_ITEM lvitem;
	lvitem.iItem    = rListCtrl.GetItemCount();
	lvitem.iSubItem = 0;
	lvitem.pszText  = csName.GetBuffer(0);
	lvitem.mask     = LVIF_TEXT;
	rListCtrl.InsertItem(&lvitem);
}

void CSpfcEnvelopeDlg::DelListItem(CListCtrl& rListCtrl, CString csName)
{
	int nCount = rListCtrl.GetItemCount();
	for ( int i = 0; i < nCount; ++i )
	{
		CString csText = rListCtrl.GetItemText(i, 0);
		if ( csText == csName )
		{
			rListCtrl.DeleteItem(i);
			return;
		}
	}
}

int CSpfcEnvelopeDlg::GetListItem(const CListCtrl& cListCtrl)
{
	int nItem = cListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	return nItem;
}

void CSpfcEnvelopeDlg::OnSpfcBtnAdd()
{
	int nItem = GetListItem(m_List1);
	if ( nItem < 0 ) return;

	CString csSelName = m_List1.GetItemText(nItem, 0);
	DelListItem(m_List1, csSelName);
	AddListItem(m_List2, csSelName);
	SetSpfcName();
}

void CSpfcEnvelopeDlg::OnSpfcBtnDel()
{
	int nItem = GetListItem(m_List2);
	if ( nItem < 0 ) return;

	CString csSelName = m_List2.GetItemText(nItem, 0);
	DelListItem(m_List2, csSelName);
	AddListItem(m_List1, csSelName);
	SetSpfcName();
}

void CSpfcEnvelopeDlg::SetSpfcName()
{
	int nItem = m_List2.GetItemCount();
	if ( nItem < 1 ) return;

	CString csSpfcName;
	if ( nItem == 1 )
	{
		csSpfcName = m_List2.GetItemText(0, 0);
	}
	else
	{
		CString csName1 = m_List2.GetItemText(0, 0);
		CString csName2 = m_List2.GetItemText(1, 0);
		csSpfcName.Format(_T("ENV(%s, %s)"), csName1, csName2);
	}
	GetDlgItem(IDC_SPFC_ENV_EDT_NAME)->SetWindowText(csSpfcName);
}

void CSpfcEnvelopeDlg::OnSpfcBtnClose()
{
	CDialogMove::OnCancel();
}

void CSpfcEnvelopeDlg::OnSpfcBtnApply()
{
	if ( !AddEnvelope() ) return;
	InitSpfcList();
	m_List2.DeleteAllItems();
}

void CSpfcEnvelopeDlg::OnSpfcBtnOK()
{
	if ( !AddEnvelope() ) return;
	CDialogMove::OnOK();
}

bool CSpfcEnvelopeDlg::AddEnvelope()
{
	auto L_ShowMsg = [ ] (const CString& csMsg) -> void
	{
		AfxMessageBox(csMsg, MB_ICONERROR);
	};

	CString csFuncName = GetFuncName();
	if ( csFuncName.IsEmpty() )
	{
		L_ShowMsg(_LS(IDS_DB_IREG_SPFCNAME_NOT_ENTERED));
		return false;
	}

	if ( m_List2.GetItemCount() < 2 )
	{
		L_ShowMsg(_LS(IDS_DB_IREG_SPFCFUNC_LESS_THAN_2));
		return false;
	}

	if ( !CheckSpfcData() )
	{
		L_ShowMsg(_LS(IDS_DB_IREG_SPFCFUNC_NOT_MATCH));
		return false;
	}

	T_SPFC_D SpfcENV;
	if ( !GetEnvelope(SpfcENV) ) { ASSERT(0); return false; }

	return m_pDoc->m_pDataCtrl->AddSpfc(SpfcENV);
}

CString CSpfcEnvelopeDlg::GetFuncName()
{
	CString csFuncName;
	GetDlgItem(IDC_SPFC_ENV_EDT_NAME)->GetWindowText(csFuncName);
	return csFuncName.Trim();
}

bool CSpfcEnvelopeDlg::CheckSpfcData()
{
	T_SPFC_D Data1, Data2;
	if ( !GetSpfcData(Data1, Data2) ) { ASSERT(0); return false; }

	auto L_GetPeriodEnd = [ ] (const T_SPFC_D& SpfcD) -> double
	{
		INT_PTR nData = SpfcD.arSpecFuncData.GetCount();
		if ( nData < 1 ) { ASSERT(0); return 0.0; }
		return SpfcD.arSpecFuncData[nData - 1].dblPeriod;
	};
	const double dT1 = L_GetPeriodEnd(Data1);
	const double dT2 = L_GetPeriodEnd(Data2);
	return IsEqual(dT1, dT2);
}

bool CSpfcEnvelopeDlg::GetSpfcData(T_SPFC_D& rData1, T_SPFC_D& rData2)
{
	rData1.Initialize();
	rData2.Initialize();

	CString csName1 = m_List2.GetItemText(0, 0);
	CString csName2 = m_List2.GetItemText(1, 0);
	if ( !m_pDoc->m_pAttrCtrl->GetSpfc(csName1, rData1) ) { ASSERT(0); return false; }
	if ( !m_pDoc->m_pAttrCtrl->GetSpfc(csName2, rData2) ) { ASSERT(0); return false; }
	return true;
}

bool CSpfcEnvelopeDlg::GetEnvelope(T_SPFC_D& rData)
{
	T_SPFC_D Data1, Data2;
	if ( !GetSpfcData(Data1, Data2) ) { ASSERT(0); return false; }

	std::vector<double> vdPeriod;
	if ( !AddPeriod(vdPeriod, Data1) ) { ASSERT(0); return false; }
	if ( !AddPeriod(vdPeriod, Data2) ) { ASSERT(0); return false; }
	if ( vdPeriod.empty() ) { ASSERT(0); return false; }

	if ( !AdjustSpectrum(Data1, vdPeriod) ) { ASSERT(0); return false; }
	if ( !AdjustSpectrum(Data2, vdPeriod) ) { ASSERT(0); return false; }

	rData = Data1;
	rData.SpecFuncName  = GetFuncName();
	rData.nSpecFuncId   = m_pDoc->m_pAttrCtrl->GetCountSpfc();
	rData.nSpecCodeType = D_SPFC_CODE_USER;

	INT_PTR nFuncData = Data1.arSpecFuncData.GetCount();
	for ( INT_PTR i = 0; i < nFuncData; ++i )
	{
		const T_SPFC_BASE& Base1 = Data1.arSpecFuncData[i];
		const T_SPFC_BASE& Base2 = Data2.arSpecFuncData[i];

		T_SPFC_BASE& rBase = rData.arSpecFuncData[i];
		rBase.dblValue = max(Base1.dblValue, Base2.dblValue);
	}
	return true;
}

bool CSpfcEnvelopeDlg::AddPeriod(std::vector<double>& rvdPeriod, const T_SPFC_D& cData)
{
	auto L_IsExist = [ this ] (const std::vector<double>& vdPeriod, const double& dT) -> bool
	{
		for ( const double dPeriod : vdPeriod )
		{
			if ( IsEqual(dPeriod, dT) )
			{
				return true;
			}
		}
		return false;
	};

	auto L_AddPeriod = [ ] (std::vector<double>& rvdPeriod, const double& dT)
	{
		if ( !rvdPeriod.empty() )
		{
			size_t nPeriod = rvdPeriod.size();
			for ( size_t i = 0; i < nPeriod; ++i )
			{
				if ( rvdPeriod[i] > dT )
				{
					rvdPeriod.insert(rvdPeriod.begin() + i, dT);
					return;
				}
			}
			rvdPeriod.push_back(dT);
		}
		else
		{
			rvdPeriod.push_back(dT);
		}
	};

	INT_PTR nData = cData.arSpecFuncData.GetCount();
	for ( INT_PTR i = 0; i < nData; ++i )
	{
		const double& dT = cData.arSpecFuncData[i].dblPeriod;
		if ( !L_IsExist(rvdPeriod, dT) )
		{
			L_AddPeriod(rvdPeriod, dT);
		}
	}
	return !rvdPeriod.empty();
}

bool CSpfcEnvelopeDlg::IsEqual(const double& dT1, const double& dT2)
{
	return ( fabs(dT1 - dT2) < 1.0e-7 ) ? true : false;
}

bool CSpfcEnvelopeDlg::AdjustSpectrum(T_SPFC_D& rData, const std::vector<double>& vdPeriod)
{
	size_t nPeriod = vdPeriod.size();
	if ( nPeriod < 1 ) { ASSERT(0); return false; }

	auto L_IsExist = [ this ] (const T_SPFC_D& SpfcD, const double& dT) -> bool
	{
		INT_PTR nData = SpfcD.arSpecFuncData.GetCount();
		for ( INT_PTR i = 0; i < nData; ++i )
		{
			if ( IsEqual(SpfcD.arSpecFuncData[i].dblPeriod, dT) )
			{
				return true;
			}
		}
		return false;
	};

	auto L_GetIndex = [ ] (const T_SPFC_D& SpfcD, const double& dT) -> INT_PTR
	{
		INT_PTR nData = SpfcD.arSpecFuncData.GetCount();
		for ( INT_PTR i = 0; i < nData; ++i )
		{
			if ( SpfcD.arSpecFuncData[i].dblPeriod > dT )
			{
				return i;
			}
		}
		ASSERT(0); return nData - 1;
	};

	for ( size_t i = 0; i < nPeriod; ++i )
	{
		const double& dT = vdPeriod[i];
		if ( L_IsExist(rData, dT) ) continue;

		INT_PTR nIndex = L_GetIndex(rData, dT);
		if ( nIndex < 1 ) { ASSERT(0); return false; }

		const T_SPFC_BASE& Base2 = rData.arSpecFuncData[nIndex];
		const T_SPFC_BASE& Base1 = rData.arSpecFuncData[nIndex - 1];

		double dRatio = ( dT - Base1.dblPeriod ) / ( Base2.dblPeriod - Base1.dblPeriod );
		T_SPFC_BASE newBase;
		newBase.dblPeriod = dT;
		newBase.dblValue  = CMathFunc::mathInterpolate(Base1.dblValue, Base2.dblValue, dRatio);

		rData.arSpecFuncData.InsertAt(nIndex, newBase);
	}

	return true;
}