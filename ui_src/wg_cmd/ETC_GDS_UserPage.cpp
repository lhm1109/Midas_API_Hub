// ETC_GDS_UserPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_UserPage.h"

#include "SpfcItemGrid.h"
#include "ETC_DESIGN_SPECT.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\UnitCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KBC2009 dialog
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UserPage dialog


CETC_GDS_UserPage::CETC_GDS_UserPage(T_SPFC_D *pData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_UserPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_GDS_UserPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSpfcD = pData;
	m_pParent = (CETC_DESIGN_SPECT*)pParent;
	m_pGrid = NULL;
}


void CETC_GDS_UserPage::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_UserPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_ETC_FUNC_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_UserPage, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_UserPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CETC_GDS_UserPage::Data2Dlg()
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd==NULL || !::IsWindow(hWnd)) { ASSERT(0); return; } // window 없으면 나가리..

	m_pGrid->MakeItemEx();
}

BOOL CETC_GDS_UserPage::Dlg2Data()
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd==NULL || !::IsWindow(hWnd)) { ASSERT(0); return FALSE; } // window 없으면 나가리..

	return TRUE;
}

void CETC_GDS_UserPage::ChangeHeaderTitle()
{
	HWND hWnd = GetSafeHwnd();
	if(hWnd==NULL || !::IsWindow(hWnd)) { ASSERT(0); return; } // window 없으면 나가리..

	T_UNIT_SYSTEM us;
	m_pGrid->SetHeaderTitle(us, FALSE);
}

void CETC_GDS_UserPage::SpectralDataChanged()
{
	m_pParent->SpectralDataChanged();
}
/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UserPage message handlers

BOOL CETC_GDS_UserPage::OnInitDialog() 
{
	m_pGrid = new CSpfcItemGrid(TRUE);

	CMyChildDialog::OnInitDialog();
	
	m_pGrid->Initialize(m_pSpfcD);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_UserPage::PostNcDestroy() 
{	
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}

	CMyChildDialog::PostNcDestroy();
}

void CETC_GDS_UserPage::MakeSpectrumData(BOOL bOnlyCalc/*=FALSE*/)
{
	int nSize = m_pSpfcD->arSpecFuncData.GetSize();
	if(nSize<1) { ASSERT(0); return; }

	for(int i=0; i<nSize; ++i)
	{
		m_parPeriod->Add(m_pSpfcD->arSpecFuncData[i].dblPeriod);
		m_parAccel->Add(m_pSpfcD->arSpecFuncData[i].dblValue);
	}
	if(bOnlyCalc) return;

	m_strFuncName = _T("User Type");
}

void CETC_GDS_UserPage::MakePOSpectrumData(double dDamping,double SRa,double SRv,
																					 CArray <double,double>& aPeriod,
																					 CArray <double,double>& aAccel) // For Pushover Curve
{
	int nSize = m_pSpfcD->arSpecFuncData.GetSize();
	if(nSize<1) { ASSERT(0); return; }
	aPeriod.SetSize(nSize);
	aAccel.SetSize(nSize);
	for(int i=0; i<nSize; ++i)
	{
		aPeriod[i] = m_pSpfcD->arSpecFuncData[i].dblPeriod;
		aAccel[i]  = m_pSpfcD->arSpecFuncData[i].dblValue;
	}

}