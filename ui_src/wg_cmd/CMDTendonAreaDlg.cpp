// CMDTendonAreaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDTendonAreaDlg.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
	

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonAreaDlg dialog
#define CDialog CChildDialogMove 


CCMDTendonAreaDlg::CCMDTendonAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMDTendonAreaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMDTendonAreaDlg)
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();

	m_TargetData.Initialize();
}

BOOL CCMDTendonAreaDlg::GetTendonAreaData(T_TENDON_AREA &data)
{
	data.Initialize();
	data = m_TargetData;
	return TRUE;
}

void CCMDTendonAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDTendonAreaDlg)
	DDX_Control(pDX, IDC_CMD_TENDON_AREA_COMBO, m_wndTendonAreaCombo);
	DDX_Control(pDX, IDC_CMD_TENDON_AREA_NUM_EDIT, m_strTAreaNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMDTendonAreaDlg, CDialog)
	//{{AFX_MSG_MAP(CCMDTendonAreaDlg)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_TENDON_AREA_SPIN, OnDeltaposCmdTendonAreaSpin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonAreaDlg message handlers


// IsKoreaCodeForSteel
// IsJapanCodeForSteel
BOOL CCMDTendonAreaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetTendonAreaCombo();
	m_strTAreaNum.SetRange(0, 10000);
	m_strTAreaNum.SetValue(1);
	m_strTAreaNum.SetInteger(TRUE);

// 	CDlgUtil::SetSpinValue(this, IDC_CMD_TENDON_AREA_NUM_EDIT, 0, -1, pNMHDR, pResult);

	/*
	static double TendonArea[]      =        {0.00009871,0.0001387,}; // metric unit

	m_wndTendonAreaCombo.AddString(_T("12.7mm(0.5\")"));
	m_wndTendonAreaCombo.SetItemData(0,(DWORD)(TendonArea));
	m_wndTendonAreaCombo.AddString(_T("15.2mm(0.6\")"));
	m_wndTendonAreaCombo.SetItemData(1,(DWORD)(TendonArea+1));
	m_wndTendonAreaCombo.SetCurSel(0);
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDTendonAreaDlg::OnOK() 
{
	if (!GetTargetData()) return;
	CDialog::OnOK();
}

void CCMDTendonAreaDlg::OnDeltaposCmdTendonAreaSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CDlgUtil::SetSpinValue(this,IDC_CMD_TENDON_AREA_NUM_EDIT,0,-1,pNMHDR,pResult);
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMDTendonAreaDlg::SetTendonAreaCombo()
{
	m_wndTendonAreaCombo.ResetContent();

	int nNum = m_aAreaDB.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		m_wndTendonAreaCombo.AddString(m_aAreaDB[i].csDiaName);
	}

	CString str;
	CSize   sz;
	int     dx=0;
	CDC* pDC = m_wndTendonAreaCombo.GetDC();
	for (int i=0;i < m_wndTendonAreaCombo.GetCount();i++)
	{
		m_wndTendonAreaCombo.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}  
	m_wndTendonAreaCombo.SetDroppedWidth(dx);
	m_wndTendonAreaCombo.ReleaseDC(pDC);

	m_wndTendonAreaCombo.SetCurSel(0);
}

BOOL CCMDTendonAreaDlg::GetTargetData()
{
	UpdateData(TRUE);
	int nSel = m_wndTendonAreaCombo.GetCurSel();
	if (nSel == CB_ERR) return FALSE;

	int nTendonNum;
	if (m_strTAreaNum.GetEditValue(nTendonNum))
	{
		m_TargetData = m_aAreaDB.GetAt(nSel); 
		m_TargetData.dArea *= nTendonNum;
		m_TargetData.dArea = m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_AREA, m_TargetData.dArea);
		m_TargetData.dW_Factor = m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_PERLENGTH, m_TargetData.dW_Factor);
		m_TargetData.dY_Strength = m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_ELAST, m_TargetData.dY_Strength);
		m_TargetData.dU_Strength = m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_ELAST, m_TargetData.dU_Strength);
		m_TargetData.dAnchorageSlip = m_pDoc->m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, m_TargetData.dAnchorageSlip);
		return TRUE;
	}
	return FALSE;
}

BOOL CCMDTendonAreaDlg::MakeTendonAreaDB_CH_GB(T_MATL_D& MatlD)
{
	if(MatlD.Data1.CodeName.Find(MATLCODE_STL_GB50917_13)<0) return FALSE;


	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Std1470; // GB 50917-13(S) : strand
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Std1570;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Std1720;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Std1860;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Std1960;

	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Wir1470; // GB 50917-13(S) : wire
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Wir1570;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Wir1770;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Wir1860;

	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Stb540; // GB 50917-13(S) : Steel bar
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Stb785;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Stb930;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB_Stb1080;


	aChAreaDB_Std1470.RemoveAll(); 
	aChAreaDB_Std1570.RemoveAll();
	aChAreaDB_Std1720.RemoveAll();
	aChAreaDB_Std1860.RemoveAll();
	aChAreaDB_Std1960.RemoveAll();
	aChAreaDB_Wir1470.RemoveAll(); 
	aChAreaDB_Wir1570.RemoveAll();
	aChAreaDB_Wir1770.RemoveAll();
	aChAreaDB_Wir1860.RemoveAll();
	aChAreaDB_Stb540.RemoveAll();
	aChAreaDB_Stb785.RemoveAll();
	aChAreaDB_Stb930.RemoveAll();
	aChAreaDB_Stb1080.RemoveAll();

	T_TENDON_AREA data;
	int nNum = 0;
	////////////////////////////
	// GB 50917-13(S) : strand
	////////////////////////////
	CString aDiaCh_Std1470[] = {_T("8mm(1x2)"),    _T("10mm(1x2)"),   _T("12mm(1x2)"),   
															_T("8.6mm(1x3)"),  _T("10.8mm(1x3)"), _T("12.9mm(1x3)")};
	double aAreaCh_Std1470[] = {0.0000251, 0.0000393, 0.0000565, 
															0.0000377, 0.0000589, 0.0000848};
	nNum = sizeof(aDiaCh_Std1470)/sizeof(CString);
	int i = 0;
	for(i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Std1470[i];
		data.dArea = aAreaCh_Std1470[i];
		aChAreaDB_Std1470.Add(data);
	}

	//
	CString aDiaCh_Std1570[] = {_T("8mm(1x2)"),    _T("10mm(1x2)"),   _T("12mm(1x2)"),   
															_T("8.6mm(1x3)"),  _T("10.8mm(1x3)"), _T("12.9mm(1x3)")};
	double aAreaCh_Std1570[] = {0.0000251, 0.0000393, 0.0000565, 
															0.0000377, 0.0000589, 0.0000848};
	nNum = sizeof(aDiaCh_Std1570)/sizeof(CString);
	for(i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Std1570[i];
		data.dArea = aAreaCh_Std1570[i];
		aChAreaDB_Std1570.Add(data);
	}

	//
	CString aDiaCh_Std1720[] = {_T("8mm(1x2)"),    _T("10mm(1x2)"),   _T("12mm(1x2)"),   
															_T("8.6mm(1x3)"),  _T("10.8mm(1x3)"), _T("12.9mm(1x3)"),
															_T("17.8mm(1x7)") };
	double aAreaCh_Std1720[] = {0.0000251, 0.0000393, 0.0000565, 
															0.0000377, 0.0000589, 0.0000848,
															0.0001910};

	nNum = sizeof(aDiaCh_Std1720)/sizeof(CString);
	for(i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Std1720[i];
		data.dArea = aAreaCh_Std1720[i];
		aChAreaDB_Std1720.Add(data);
	}

	//
	CString aDiaCh_Std1860[] = {_T("8mm(1x2)"),    _T("10mm(1x2)"),   _T("12mm(1x2)"),   
															_T("8.6mm(1x3)"),  _T("10.8mm(1x3)"), _T("12.9mm(1x3)"),
															_T("9.5mm(1x7)"),  _T("12.7mm(1x7)"), _T("15.2mm(1x7)"), _T("17.8mm(1x7)") };
	double aAreaCh_Std1860[] = {0.0000251, 0.0000393, 0.0000565, 
															0.0000377, 0.0000589, 0.0000848,
															0.0000548, 0.0000987, 0.0001400, 0.0001910};

	nNum = sizeof(aDiaCh_Std1860)/sizeof(CString);
	for(i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Std1860[i];
		data.dArea = aAreaCh_Std1860[i];
		aChAreaDB_Std1860.Add(data);
	}

	//
	CString aDiaCh_Std1960[] = {_T("8mm(1x2)"),    _T("10mm(1x2)"),  
															_T("8.6mm(1x3)"),  _T("10.8mm(1x3)"), _T("12.9mm(1x3)"),
															_T("9.5mm(1x7)"),  _T("12.7mm(1x7)"), _T("15.2mm(1x7)")};
	double aAreaCh_Std1960[] = {0.0000251, 0.0000393,
															0.0000377, 0.0000589, 0.0000848,
															0.0000548, 0.0000987, 0.0001400};

	nNum = sizeof(aDiaCh_Std1960)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Std1960[i];
		data.dArea = aAreaCh_Std1960[i];
		aChAreaDB_Std1960.Add(data);
	}

	//////////////////////////
	// GB 50917-13(S) : wire
	//////////////////////////
	//
	CString aDiaCh_Wir1470[] = {_T("9mm")};
	double aAreaCh_Wir1470[] = {0.00006362};
	nNum = sizeof(aDiaCh_Wir1470)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Wir1470[i];
		data.dArea = aAreaCh_Wir1470[i];
		aChAreaDB_Wir1470.Add(data);
	}

	//
	CString aDiaCh_Wir1570[] = {_T("5mm"), _T("7mm"), _T("9mm")};
	double aAreaCh_Wir1570[] = {0.00001963, 0.00003848, 0.00006362};
	nNum = sizeof(aDiaCh_Wir1570)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Wir1570[i];
		data.dArea = aAreaCh_Wir1570[i];
		aChAreaDB_Wir1570.Add(data);
	}

	//
	CString aDiaCh_Wir1770[] = {_T("5mm")};
	double aAreaCh_Wir1770[] = {0.00001963};
	nNum = sizeof(aDiaCh_Wir1770)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Wir1770[i];
		data.dArea = aAreaCh_Wir1770[i];
		aChAreaDB_Wir1770.Add(data);
	}

	//
	CString aDiaCh_Wir1860[] = {_T("5mm")};
	double aAreaCh_Wir1860[] = {0.00001963};
	nNum = sizeof(aDiaCh_Wir1860)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Wir1860[i];
		data.dArea = aAreaCh_Wir1860[i];
		aChAreaDB_Wir1860.Add(data);
	}


	//////////////////////////
	// GB 50917-13(S) : Steel bar
	//////////////////////////
	//
	CString aDiaCh_Stb540[] = {_T("18mm"), _T("25mm"), _T("32mm"), _T("40mm"), _T("50mm")};
	double aAreaCh_Stb540[] = {0.0002545, 0.0004909, 0.0008042, 0.0012566, 0.0019635};
	nNum = sizeof(aDiaCh_Stb540)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Stb540[i];
		data.dArea = aAreaCh_Stb540[i];
		aChAreaDB_Stb540.Add(data);
	}

	//
	CString aDiaCh_Stb785[] = {_T("18mm"), _T("25mm"), _T("32mm"), _T("40mm"), _T("50mm")};
	double aAreaCh_Stb785[] = {0.0002545, 0.0004909, 0.0008042, 0.0012566, 0.0019635};
	nNum = sizeof(aDiaCh_Stb785)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Stb785[i];
		data.dArea = aAreaCh_Stb785[i];
		aChAreaDB_Stb785.Add(data);
	}

	//
	CString aDiaCh_Stb930[] = {_T("18mm"), _T("25mm"), _T("32mm")};
	double aAreaCh_Stb930[] = {0.0002545, 0.0004909, 0.0008042};
	nNum = sizeof(aDiaCh_Stb930)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Stb930[i];
		data.dArea = aAreaCh_Stb785[i];
		aChAreaDB_Stb930.Add(data);
	}

	//
	CString aDiaCh_Stb1080[] = {_T("18mm"), _T("25mm"), _T("32mm")};
	double aAreaCh_Stb1080[] = {0.0002545, 0.0004909, 0.0008042};
	nNum = sizeof(aDiaCh_Stb1080)/sizeof(CString);
	for( i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh_Stb1080[i];
		data.dArea = aAreaCh_Stb1080[i];
		aChAreaDB_Stb1080.Add(data);
	}

	//////////////////////////////////////////////////////////////////////////
	if(MatlD.Data1.CodeMatlName.Find(_T("Strand1470"))>=0) 
	{
		 for(i = 0; i < aChAreaDB_Std1470.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Std1470[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Strand1570"))>=0)
	{
		for(i = 0; i < aChAreaDB_Std1570.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Std1570[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Strand1720"))>=0)
	{
		for(i = 0; i < aChAreaDB_Std1720.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Std1720[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Strand1860"))>=0)
	{
		for(i = 0; i < aChAreaDB_Std1860.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Std1860[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Strand1960"))>=0)
	{
		for(i = 0; i < aChAreaDB_Std1960.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Std1960[i]);
	}
	//
	else if(MatlD.Data1.CodeMatlName.Find(_T("Wire1470"))>=0)
	{
		for(i = 0; i < aChAreaDB_Wir1470.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Wir1470[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Wire1570"))>=0)
	{
		for(i = 0; i < aChAreaDB_Wir1570.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Wir1570[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Wire1770"))>=0)
	{
		for(i = 0; i < aChAreaDB_Wir1770.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Wir1770[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Wire1860"))>=0)
	{
		for(i = 0; i < aChAreaDB_Wir1770.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Wir1770[i]);
	}
	//
	else if(MatlD.Data1.CodeMatlName.Find(_T("Steelbar540"))>=0)
	{
		for(i = 0; i < aChAreaDB_Stb540.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Stb540[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Steelbar785"))>=0)
	{
		for(i = 0; i < aChAreaDB_Stb785.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Stb785[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Steelbar930"))>=0)
	{
		for(i = 0; i < aChAreaDB_Stb930.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Stb930[i]);
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Steelbar1080"))>=0)
	{
		for(i = 0; i < aChAreaDB_Stb1080.GetSize(); i++) m_aAreaDB.Add(aChAreaDB_Stb1080[i]);
	}
	else 
	{
		return FALSE;
	}

	if(m_aAreaDB.GetSize()<=0)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CCMDTendonAreaDlg::MakeTendonAreaDB_CH_JTG3362(T_MATL_D& MatlD)
{
	if(MatlD.Data1.CodeName.Find(MATLCODE_STL_JTG3362_18)<0) return FALSE;

	T_TENDON_AREA data;
	if(MatlD.Data1.CodeMatlName.Find(_T("Strand"))>=0) 
	{
			CString aDia[] = {_T("Strand 9.5mm(1x7)") ,  _T("Strand 12.7mm(1x7)"), _T("Strand 15.2mm(1x7)"), _T("Strand 17.8mm(1x7)"),  _T("Strand 21.6mm(1x7)") };
			double aArea[] = {54.8, 98.7, 139, 191, 285};
			int nNum = sizeof(aDia)/sizeof(CString);
			for(int i = 0; i < nNum; i++)
			{
					data.Initialize();
					data.csDiaName = aDia[i];
					data.dArea     = aArea[i]*1e-6;
					m_aAreaDB.Add(data);
			}
	}
	//
	else if(MatlD.Data1.CodeMatlName.Find(_T("Wire"))>=0) 
	{
			CString aDia[] = {_T("Wire 5mm") ,  _T("Wire 7mm"), _T("Wire 9mm")};
			double aArea[] = {19.63, 38.48, 63.62};
			int nNum = sizeof(aDia)/sizeof(CString);
			for(int i = 0; i < nNum; i++)
			{
					data.Initialize();
					data.csDiaName = aDia[i];
					data.dArea     = aArea[i]*1e-6;
					m_aAreaDB.Add(data);
			}
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Steelbar"))>=0) 
	{
			CString aDia[] = {_T("18mm") ,  _T("25mm"), _T("32mm"),  _T("40mm"), _T("50mm")};
			double aArea[] = {254.5, 490.9, 804.2, 1256.6, 1963.5};
			int nNum = sizeof(aDia)/sizeof(CString);
			for(int i = 0; i < nNum; i++)
			{
					data.Initialize();
					data.csDiaName = aDia[i];
					data.dArea     = aArea[i]*1e-6;
					m_aAreaDB.Add(data);
			}
	}
	else
			return FALSE;

	if(m_aAreaDB.GetSize()<=0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCMDTendonAreaDlg::MakeTendonAreaDB_CH_CJJ11_2019(T_MATL_D& MatlD)
{
	if(MatlD.Data1.CodeName.Find(MATLCODE_STL_CJJ11_2019)<0) return FALSE;

	T_TENDON_AREA data;
	if(MatlD.Data1.CodeMatlName.Find(_T("Strand"))>=0) 
	{
		CString aDia[] = {_T("Strand 9.5mm(1x7)") ,  _T("Strand 12.7mm(1x7)"), _T("Strand 15.2mm(1x7)"), _T("Strand 17.8mm(1x7)"),  _T("Strand 21.6mm(1x7)") };
		double aArea[] = {54.8, 98.7, 139, 191, 285};
		int nNum = sizeof(aDia)/sizeof(CString);
		for(int i = 0; i < nNum; i++)
		{
			data.Initialize();
			data.csDiaName = aDia[i];
			data.dArea     = aArea[i]*1e-6;
			m_aAreaDB.Add(data);
		}
	}
	//
	else if(MatlD.Data1.CodeMatlName.Find(_T("Wire"))>=0) 
	{
		CString aDia[] = {_T("Wire 5mm") ,  _T("Wire 7mm"), _T("Wire 9mm")};
		double aArea[] = {19.63, 38.48, 63.62};
		int nNum = sizeof(aDia)/sizeof(CString);
		for(int i = 0; i < nNum; i++)
		{
			data.Initialize();
			data.csDiaName = aDia[i];
			data.dArea     = aArea[i]*1e-6;
			m_aAreaDB.Add(data);
		}
	}
	else if(MatlD.Data1.CodeMatlName.Find(_T("Steelbar"))>=0) 
	{
		CString aDia[] = {_T("18mm") ,  _T("25mm"), _T("32mm"),  _T("40mm"), _T("50mm")};
		double aArea[] = {254.5, 490.9, 804.2, 1256.6, 1963.5};
		int nNum = sizeof(aDia)/sizeof(CString);
		for(int i = 0; i < nNum; i++)
		{
			data.Initialize();
			data.csDiaName = aDia[i];
			data.dArea     = aArea[i]*1e-6;
			m_aAreaDB.Add(data);
		}
	}
	else
		return FALSE;

	if(m_aAreaDB.GetSize()<=0)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CCMDTendonAreaDlg::MakeTendonAreaDB_CH_QCR9300_18(T_MATL_D& MatlD)
{
	if(MatlD.Data1.CodeName.Find(MATLCODE_STL_Q_CR9300_18)<0) return FALSE;

	const CString& strMatlName = MatlD.Data1.CodeMatlName;
	if (strMatlName == _T("PSB830") || strMatlName == _T("PSB980"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("18mm"), 0.0002545));
		m_aAreaDB.Add(T_TENDON_AREA(_T("25mm"), 0.0004909));
		m_aAreaDB.Add(T_TENDON_AREA(_T("32mm"), 0.0008042));
		m_aAreaDB.Add(T_TENDON_AREA(_T("40mm"), 0.0012566));
		m_aAreaDB.Add(T_TENDON_AREA(_T("50mm"), 0.0019635));
	}
	else if(strMatlName == _T("Wire1470") || 
		strMatlName == _T("Wire1570") ||
		strMatlName == _T("Wire1670") || 
		strMatlName == _T("Wire1770") || 
		strMatlName == _T("Wire1860"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 4mm"),   0.00001257));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 4.8mm"), 0.00001810));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 5mm"),   0.00001963));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 6mm"),   0.00002827));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 6.25mm"),0.00003068));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 7mm"),   0.00003848));
	}
	else if(strMatlName == _T("Strand1470") ||
		strMatlName == _T("Strand1570") ||
		strMatlName == _T("Strand1670") ||
		strMatlName == _T("Strand1720") ||
		strMatlName == _T("Strand1770") ||
		strMatlName == _T("Strand1820") ||
		strMatlName == _T("Strand1860") ||
		strMatlName == _T("Strand1960"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 12.7mm(1x7)"),   0.0000987));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 15.2mm(1x7)"),   0.0001400));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 15.7mm(1x7)"),   0.0001500));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 12.7mm(1x7)C"),  0.0001120));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 15.2mm(1x7)C"),  0.0001650));
	}
	else
	{
		return FALSE;
	}
#if 0
	if (strMatlName == _T("PSB830") || strMatlName == _T("PSB980"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("18mm"), 0.0002545));
		m_aAreaDB.Add(T_TENDON_AREA(_T("25mm"), 0.0004909));
		m_aAreaDB.Add(T_TENDON_AREA(_T("32mm"), 0.0008042));
		m_aAreaDB.Add(T_TENDON_AREA(_T("40mm"), 0.0012566));
		m_aAreaDB.Add(T_TENDON_AREA(_T("50mm"), 0.0019635));
	}	
	else if (strMatlName == _T("Strand1470") || strMatlName == _T("Strand1570"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12mm(1x2)"),   0.0000565));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
	}	
	else if (strMatlName == _T("Strand1670"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Strand1720"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12mm(1x2)"),   0.0000565));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("17.8mm(1x7)"), 0.0001910));
	}
	else if (strMatlName == _T("Strand1770"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Strand1820"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Strand1860"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12mm(1x2)"),   0.0000565));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("9.5mm(1x7)"),  0.0000548));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.7mm(1x7)"), 0.0000987));
		m_aAreaDB.Add(T_TENDON_AREA(_T("15.2mm(1x7)"), 0.0001400));
		m_aAreaDB.Add(T_TENDON_AREA(_T("17.8mm(1x7)"), 0.0001910));
	}
	else if (strMatlName == _T("Strand1960"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("9.5mm(1x7)"),  0.0000548));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.7mm(1x7)"), 0.0000987));
		m_aAreaDB.Add(T_TENDON_AREA(_T("15.2mm(1x7)"), 0.0001400));
	}
	else if (strMatlName == _T("Wire1470"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("9mm"), 0.00006362));
	}
	else if (strMatlName == _T("Wire1570"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("5mm"), 0.00001963));
		m_aAreaDB.Add(T_TENDON_AREA(_T("7mm"), 0.00003848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("9mm"), 0.00006362));
	}
	else if (strMatlName == _T("Wire1670"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Wire1770"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("5mm"), 0.00001963));
	}
	else if (strMatlName == _T("Wire1860"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("5mm"), 0.00001963));
	}
	else
	{
		return FALSE;
	}
#endif
	return m_aAreaDB.GetSize() > 0 ? TRUE : FALSE;
}

BOOL CCMDTendonAreaDlg::MakeTendonAreaDB_CH_TB10092_17(T_MATL_D& MatlD)
{
	if(MatlD.Data1.CodeName.Find(MATLCODE_STL_TB10092_17)<0) return FALSE;

	const CString& strMatlName = MatlD.Data1.CodeMatlName;
	if (strMatlName == _T("PSB830") || strMatlName == _T("PSB980"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("18mm"), 0.0002545));
		m_aAreaDB.Add(T_TENDON_AREA(_T("25mm"), 0.0004909));
		m_aAreaDB.Add(T_TENDON_AREA(_T("32mm"), 0.0008042));
		m_aAreaDB.Add(T_TENDON_AREA(_T("40mm"), 0.0012566));
		m_aAreaDB.Add(T_TENDON_AREA(_T("50mm"), 0.0019635));
	}
	else if(strMatlName == _T("Wire1470") || 
		strMatlName == _T("Wire1570") ||
		strMatlName == _T("Wire1670") || 
		strMatlName == _T("Wire1770") || 
		strMatlName == _T("Wire1860"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 4mm"),   0.00001257));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 4.8mm"), 0.00001810));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 5mm"),   0.00001963));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 6mm"),   0.00002827));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 6.25mm"),0.00003068));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Wire 7mm"),   0.00003848));
	}
	else if(strMatlName == _T("Strand1470") ||
		strMatlName == _T("Strand1570") ||
		strMatlName == _T("Strand1670") ||
		strMatlName == _T("Strand1720") ||
		strMatlName == _T("Strand1770") ||
		strMatlName == _T("Strand1820") ||
		strMatlName == _T("Strand1860") ||
		strMatlName == _T("Strand1960"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 12.7mm(1x7)"),   0.0000987));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 15.2mm(1x7)"),   0.0001400));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 15.7mm(1x7)"),   0.0001500));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 12.7mm(1x7)C"),  0.0001120));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 15.2mm(1x7)C"),  0.0001650));
	}
	else
	{
		return FALSE;
	}
#if 0
	if (strMatlName == _T("PSB830") || strMatlName == _T("PSB980"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("18mm"), 0.0002545));
		m_aAreaDB.Add(T_TENDON_AREA(_T("25mm"), 0.0004909));
		m_aAreaDB.Add(T_TENDON_AREA(_T("32mm"), 0.0008042));
		m_aAreaDB.Add(T_TENDON_AREA(_T("40mm"), 0.0012566));
		m_aAreaDB.Add(T_TENDON_AREA(_T("50mm"), 0.0019635));
	}	
	else if (strMatlName == _T("Strand1470") || strMatlName == _T("Strand1570"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12mm(1x2)"),   0.0000565));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
	}	
	else if (strMatlName == _T("Strand1670"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Strand1720"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12mm(1x2)"),   0.0000565));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("17.8mm(1x7)"), 0.0001910));
	}
	else if (strMatlName == _T("Strand1770"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Strand1820"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Strand1860"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12mm(1x2)"),   0.0000565));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("9.5mm(1x7)"),  0.0000548));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.7mm(1x7)"), 0.0000987));
		m_aAreaDB.Add(T_TENDON_AREA(_T("15.2mm(1x7)"), 0.0001400));
		m_aAreaDB.Add(T_TENDON_AREA(_T("17.8mm(1x7)"), 0.0001910));
	}
	else if (strMatlName == _T("Strand1960"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("8mm(1x2)"),    0.0000251));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10mm(1x2)"),   0.0000393));
		m_aAreaDB.Add(T_TENDON_AREA(_T("8.6mm(1x3)"),  0.0000377));
		m_aAreaDB.Add(T_TENDON_AREA(_T("10.8mm(1x3)"), 0.0000589));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.9mm(1x3)"), 0.0000848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("9.5mm(1x7)"),  0.0000548));
		m_aAreaDB.Add(T_TENDON_AREA(_T("12.7mm(1x7)"), 0.0000987));
		m_aAreaDB.Add(T_TENDON_AREA(_T("15.2mm(1x7)"), 0.0001400));
	}
	else if (strMatlName == _T("Wire1470"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("9mm"), 0.00006362));
	}
	else if (strMatlName == _T("Wire1570"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("5mm"), 0.00001963));
		m_aAreaDB.Add(T_TENDON_AREA(_T("7mm"), 0.00003848));
		m_aAreaDB.Add(T_TENDON_AREA(_T("9mm"), 0.00006362));
	}
	else if (strMatlName == _T("Wire1670"))
	{
		/// add available area list
	}
	else if (strMatlName == _T("Wire1770"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("5mm"), 0.00001963));
	}
	else if (strMatlName == _T("Wire1860"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("5mm"), 0.00001963));
	}
	else
	{
		return FALSE;
	}
#endif

	return m_aAreaDB.GetSize() > 0 ? TRUE : FALSE;
}

BOOL CCMDTendonAreaDlg::MakeTendonAreaDB_CH_GB19(T_MATL_D& MatlD)
{
	if(MatlD.Data1.CodeName.Find(MATLCODE_STL_GB19)<0) return FALSE;

// 	{"Strand1570","Strand1720", "Strand1860", "Strand1960",
// 		"MSW800","MSW970", "MSW1270", 
// 		"SRW1470", "SRW1570", "SRW1860", 
// 		"CRB650", "CRB800", "CRB970", 
// 		"Steelbar980", "Steelbar1080", "Steelbar1230"};

	const CString& strMatlName = MatlD.Data1.CodeMatlName;
	if (strMatlName == _T("Steelbar980") || 
		strMatlName == _T("Steelbar1080")||
		strMatlName == _T("Steelbar1230"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("18mm"), 254.5e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("25mm"), 490.9e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("32mm"), 804.2e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("40mm"), 1256.6e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("50mm"), 1963.5e-6));
	}
	else if (strMatlName == _T("MSW800") || 
		     strMatlName == _T("MSW970")||
		     strMatlName == _T("MSW1270"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("MSW 5mm"), 19.63e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("MSW 7mm"), 38.48e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("MSW 9mm"), 63.62e-6));
	}
	else if (strMatlName == _T("SRW1470") || 
		     strMatlName == _T("SRW1570")||
		     strMatlName == _T("SRW1860"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("SRW 5mm"), 19.63e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("SRW 7mm"), 38.48e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("SRW 9mm"), 63.62e-6));
	}
	else if (strMatlName == _T("CRB650") || 
		     strMatlName == _T("CRB800")||
		     strMatlName == _T("CRB970"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("CRB 5mm"), 19.63e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("CRB 6mm"), 28.27e-6));
	}
	else if(strMatlName == _T("Strand1570") ||
		    strMatlName == _T("Strand1720") ||
		    strMatlName == _T("Strand1860") ||
		    strMatlName == _T("Strand1960"))
	{
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 8.6mm(1x3)"),    37.7e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 10.8mm(1x3)"),   58.9e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 12.9mm(1x3)"),   84.8e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 9.5mm(1x7)"),    54.8e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 12.7mm(1x7)"),   98.7e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 15.2mm(1x7)"),   140.0e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 17.8mm(1x7)"),   191.0e-6));
		m_aAreaDB.Add(T_TENDON_AREA(_T("Strand 21.6mm(1x7)"),   285.0e-6));
	}
	else
	{
		return FALSE;
	}

	return m_aAreaDB.GetSize() > 0 ? TRUE : FALSE;
}


void CCMDTendonAreaDlg::MakeTendonAreaDB(T_MATL_K MatlK)
{
	m_aAreaDB.RemoveAll();

	int nDBType;    // (0) All  (1) Korea  (2) Japan

	T_MATL_D MatlD;
	if (!m_pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD)) nDBType = 0;
	else
	{
		if(MatlD.Type == _T("S"))
		{
			if (m_pDoc->m_pMatlDB->IsKoreaCodeForSteel(MatlD.Data1.CodeName)) nDBType = 1; // 국내기준
			else if (m_pDoc->m_pMatlDB->IsJapanCodeForSteel(MatlD.Data1.CodeName)) nDBType = 2; // 일본기준
			else if (m_pDoc->m_pMatlDB->IsChinaCodeForSteel(MatlD.Data1.CodeName)) nDBType = 3; // 중국기준
			else nDBType = 0;  // 그외  
		}
		else nDBType = 0;
	}

	// UNIT : 중립단위계 기준 (tonf, m)  
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aKrAreaDB;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aJpAreaDB;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB;
	CArray<T_TENDON_AREA, T_TENDON_AREA&> aChAreaDB2; // JTG15 일때 사용

	aKrAreaDB.RemoveAll();
	aJpAreaDB.RemoveAll();
	aChAreaDB.RemoveAll();
	aChAreaDB2.RemoveAll();

	T_TENDON_AREA data;

	// Korea 
	CString aDiaKr[] = {_T("12.7mm(0.5\")"), _T("15.2mm(0.6\")")};
	double aAreaKr[] = {0.00009871, 0.0001387};
	int nNum = sizeof(aDiaKr)/sizeof(CString);
	for(int i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaKr[i];
		data.dArea = aAreaKr[i];
		aKrAreaDB.Add(data);
	}

	// China
	CString aDiaCh[] = {_T("8mm(1x2)"),    _T("10mm(1x2)"),  _T("12mm(1x2)"),   _T("8.6mm(1x3)"),  _T("10.8mm(1x3)"), 
											_T("12.9mm(1x3)"), _T("9.5mm(1x7)"), _T("11.1mm(1x7)"), _T("12.7mm(1x7)"), _T("15.2mm(1x7)")};
	double aAreaCh[] = {0.0000251, 0.0000393, 0.0000565, 0.0000377, 0.0000589,
											0.0000848, 0.0000548, 0.0000742, 0.0000987, 0.0001400};
	nNum = sizeof(aDiaCh)/sizeof(CString);
	for(int i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh[i];
		data.dArea = aAreaCh[i];
		aChAreaDB.Add(data);
	}

	CString aDiaCh2[] = {_T("Strand 8mm(1x2)"),    _T("Strand 10mm(1x2)"),  _T("Strand 12mm(1x2)"),   _T("Strand 8.6mm(1x3)"),  _T("Strand 10.8mm(1x3)"), 
		                   _T("Strand 12.9mm(1x3)"), _T("Strand 9.5mm(1x7)"), _T("Strand 12.7mm(1x7)"), _T("Strand 15.2mm(1x7)"), _T("Strand 17.8mm(1x7)"),
	                     _T("Strand 21.6mm(1x7)"), _T("Wire 5mm"),          _T("Wire 7mm"),           _T("Wire 9mm"),           _T("Steel Bar 18mm"),
	                     _T("Steel Bar 25mm"),     _T("Steel Bar 32mm"),    _T("Steel Bar 40mm"),     _T("Steel Bar 50mm")};
	double aAreaCh2[] = {0.0000253, 0.0000395, 0.0000569, 0.0000374, 0.0000593,
		                   0.0000854, 0.0000548, 0.0000987, 0.0001390, 0.0001590,
								 			 0.0001790, 0.00001963,0.00003848,0.00006362,0.0002545,
								 			 0.0004909, 0.0008042, 0.0012470, 0.0016470};
	nNum = sizeof(aDiaCh2)/sizeof(CString);
	for(int i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName = aDiaCh2[i];
		data.dArea = aAreaCh2[i];
		aChAreaDB2.Add(data);
	}

	// Japan
	CString aDiaJp[] = {_T("12.4"),     _T("12.7B"),    _T("15.2B"),    _T("15.2"),     _T("28.6")};
	double dAreaJp[] = {0.0000929,  0.00009871, 0.0001387,  0.0001387,  0.0005324};
	double dWFJp[]   = {0.004,      0.004,      0.004,      0,          0.004};
	double dCFJp[]   = {0.3,        0.3,        0.3,        0.3,        0.3};
	double dRFJp[]   = {5,          5,          5,          1.5,        2.5};
	double dYSJp[]   = {1450000000, 1600000000, 1600000000, 1600000000, 1500000000};
	double dUSJp[]   = {1700000000, 1850000000, 1850000000, 1860000000, 1800000000};
	double dASJP[]   = {0.011,      0.012,      0.011,      0.005,      0.005};  
	nNum = sizeof(aDiaJp)/sizeof(CString);
	for(int i = 0; i < nNum; i++)
	{
		data.Initialize();
		data.csDiaName      = aDiaJp[i];
		data.dArea          = dAreaJp[i];
		data.dW_Factor      = dWFJp[i];
		data.dC_Factor      = dCFJp[i];
		data.dR_Factor      = dRFJp[i];
		data.dY_Strength    = dYSJp[i];
		data.dU_Strength    = dUSJp[i];
		data.dAnchorageSlip = dASJP[i];
		aJpAreaDB.Add(data);
	}


	if(nDBType == 1)  // Korea
	{
		for(int i = 0; i < aKrAreaDB.GetSize(); i++) m_aAreaDB.Add(aKrAreaDB[i]);
	}
	else if(nDBType == 2) // Japan
	{
		for(int i = 0; i < aJpAreaDB.GetSize(); i++) m_aAreaDB.Add(aJpAreaDB[i]);
	}
	else if(nDBType == 3) // China
	{
		if(MatlD.Data1.CodeName.Find(MATLCODE_STL_GB50917_13)>=0)
		{
			// China : GB 50917-13(S)
			if(!MakeTendonAreaDB_CH_GB(MatlD))
			{
				for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
			}
		}
		else if(MatlD.Data1.CodeName.Find(MATLCODE_STL_JTG3362_18)>=0)
		{
			// China : GB 50917-13(S)
			if(!MakeTendonAreaDB_CH_JTG3362(MatlD))
			{
				for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
			}
		}
		else if(MatlD.Data1.CodeName.Find(MATLCODE_STL_CJJ11_2019)>=0)
		{
			if(!MakeTendonAreaDB_CH_CJJ11_2019(MatlD))
			{
				for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
			}
		}
		else if(MatlD.Data1.CodeName.Find(MATLCODE_STL_TB10092_17)>=0)
		{
			if(!MakeTendonAreaDB_CH_TB10092_17(MatlD))
			{
				for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
			}
		}
		else if(MatlD.Data1.CodeName.Find(MATLCODE_STL_GB19)>=0)
		{
			if(!MakeTendonAreaDB_CH_GB19(MatlD))
			{
				for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
			}
		}
		else if(MatlD.Data1.CodeName.Find(_T("JTG15"))>=0)
		{
			for(int i = 0; i < aChAreaDB2.GetSize(); i++) m_aAreaDB.Add(aChAreaDB2[i]);
		}
		else if(MatlD.Data1.CodeName.Find(MATLCODE_STL_Q_CR9300_18)>=0)
		{
			if(!MakeTendonAreaDB_CH_QCR9300_18(MatlD))
			{
				for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
			}
		}
		else
		{
			for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
		}
	}
	else
	{
		if(CProduct::GetMovingType() == D_PRODUCT_MOVING_JP)
		{
			for(int i = 0; i < aJpAreaDB.GetSize(); i++) m_aAreaDB.Add(aJpAreaDB[i]);
			for(int i = 0; i < aKrAreaDB.GetSize(); i++) m_aAreaDB.Add(aKrAreaDB[i]);
			for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
		}
		else if(CProduct::GetMovingType() == D_PRODUCT_MOVING_CH)
		{
			for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
			for(int i = 0; i < aKrAreaDB.GetSize(); i++) m_aAreaDB.Add(aKrAreaDB[i]);
			for(int i = 0; i < aJpAreaDB.GetSize(); i++) m_aAreaDB.Add(aJpAreaDB[i]);
		}
		else
		{
			for(int i = 0; i < aKrAreaDB.GetSize(); i++) m_aAreaDB.Add(aKrAreaDB[i]);
			for(int i = 0; i < aJpAreaDB.GetSize(); i++) m_aAreaDB.Add(aJpAreaDB[i]);
			for(int i = 0; i < aChAreaDB.GetSize(); i++) m_aAreaDB.Add(aChAreaDB[i]);
		}
	} 
	/*
	if (nDBType == 0 || nDBType == 1)   // 국내기준 
	{
		CString aDia[] = {_T("12.7mm(0.5\")"), _T("15.2mm(0.6\")")};
		double aArea[] = {0.00009871, 0.0001387};

		int nNum = sizeof(aDia)/sizeof(CString);
		for (int i = 0; i < nNum; i++)
		{
			data.Initialize();
			data.csDiaName = aDia[i];
			data.dArea = aArea[i];
			m_aAreaDB.Add(data);
		}
	}

	if(nDBType == 0 || nDBType == 3)   // 중국기준 
	{
		CString aDia[] = {_T("8mm(1x2)"),    _T("10mm(1x2)"),  _T("12mm(1x2)"),   _T("8.6mm(1x3)"),  _T("10.8mm(1x3)"), 
											_T("12.9mm(1x3)"), _T("9.5mm(1x7)"), _T("11.1mm(1x7)"), _T("12.7mm(1x7)"), _T("15.2mm(1x7)")};
		double aArea[] = {0.0000251, 0.0000393, 0.0000565, 0.0000377, 0.0000589,
											0.0000848, 0.0000548, 0.0000742, 0.0000987, 0.0001400};

		int nNum = sizeof(aDia)/sizeof(CString);
		for (int i = 0; i < nNum; i++)
		{
			data.Initialize();
			data.csDiaName = aDia[i];
			data.dArea = aArea[i];
			m_aAreaDB.Add(data);
		}
	}

	if (nDBType == 0 || nDBType == 2)   // 일본기준 
	{
		CString aDia[] = {_T("12.4"),     _T("12.7B"),    _T("15.2B"),    _T("15.2"),     _T("28.6")};
		double dArea[] = {0.0000929,  0.00009871, 0.0001387,  0.0001387,  0.0005324};
		double dWF[]   = {0.004,      0.004,      0.004,      0,          0.004};
		double dCF[]   = {0.3,        0.3,        0.3,        0.3,        0.3};
		double dRF[]   = {5,          5,          5,          1.5,        2.5};
		double dYS[]   = {1450000000, 1600000000, 1600000000, 1600000000, 1500000000};
		double dUS[]   = {1700000000, 1850000000, 1850000000, 1860000000, 1800000000};
		double dAS[]   = {0.011,      0.012,      0.011,      0.005,      0.005};
		
		int nNum = sizeof(aDia)/sizeof(CString);
		for (int i = 0; i < nNum; i++)
		{
			data.Initialize();
			data.csDiaName      = aDia[i];
			data.dArea          = dArea[i];
			data.dW_Factor      = dWF[i];
			data.dC_Factor      = dCF[i];
			data.dR_Factor      = dRF[i];
			data.dY_Strength    = dYS[i];
			data.dU_Strength    = dUS[i];
			data.dAnchorageSlip = dAS[i];
			m_aAreaDB.Add(data);
		}
	}
	*/
}