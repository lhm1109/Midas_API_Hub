// ETC_GDS_USER.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_USER.h"
#include "SpfcItemDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_USER dialog


CETC_GDS_USER::CETC_GDS_USER(T_SPFC_K SpfcK/*=0*/, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_USER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_GDS_USER)
	m_SpfcK = SpfcK;
	//}}AFX_DATA_INIT
}


void CETC_GDS_USER::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_USER)
	DDX_Control(pDX, IDC_ETC_GDS_USER_SPECTRUM_CMB, m_DsgnSpec);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CETC_GDS_USER, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_USER)
	ON_BN_CLICKED(IDC_ETC_GDS_USER_CALL_DETAIL, OnClickBtnShowSpectrum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_USER message handlers

BOOL CETC_GDS_USER::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;

	T_SPFC_K SpkcKey;
	T_SPFC_D SpkcData;
	CArray<T_SPFC_K,T_SPFC_K> SpfcKeyList;
	pAttrCtrl->GetSpfcKeyList(SpfcKeyList);
	m_arSpfcKey.RemoveAll();

	for(int i=0; i<SpfcKeyList.GetSize(); i++)
	{
		SpkcKey = SpfcKeyList[i];
		pAttrCtrl->GetSpfc(SpkcKey,SpkcData);
		if(SpkcData.nSpecFuncType==1 || SpkcData.nSpecFuncType==2) // Acceleration µµ Ãß°¡.
		{	
			m_arSpfcKey.Add(SpkcKey);
			m_DsgnSpec.AddString(SpkcData.SpecFuncName);
		}	
	}
	ChangeComboWidth();
	m_DsgnSpec.SetCurSel(m_SpfcK);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_USER::OnClickBtnShowSpectrum() 
{
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	
	T_SPFC_K SpkcKey;
	T_SPFC_D SpkcData;
	CArray<T_SPFC_K,T_SPFC_K> SpfcKeyList;
	pAttrCtrl->GetSpfcKeyList(SpfcKeyList);
	
	int nIndex = m_DsgnSpec.GetCurSel();
	if(nIndex<0 || nIndex>m_arSpfcKey.GetSize()-1) return;
	SpkcKey = m_arSpfcKey[nIndex];
	pAttrCtrl->GetSpfc(SpkcKey,SpkcData);
	m_SpfcK = SpkcKey;

	CSpfcItemDlg spfcDlg;		
	spfcDlg.SetParamData(SpkcData);
	spfcDlg.SetShowType(TRUE);
	spfcDlg.DoModal();
}

void CETC_GDS_USER::MakeSpectrumData(BOOL bOnlyCalc/*=FALSE*/)
{	
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;

	T_SPFC_K SpkcKey;
	T_SPFC_D SpkcData;
	CArray<T_SPFC_K,T_SPFC_K> SpfcKeyList;
	pAttrCtrl->GetSpfcKeyList(SpfcKeyList);

	int nIndex = m_DsgnSpec.GetCurSel();
	if(nIndex<0 || nIndex>m_arSpfcKey.GetSize()-1) return;
	SpkcKey = m_arSpfcKey[nIndex];
	pAttrCtrl->GetSpfc(SpkcKey,SpkcData);
	m_SpfcK = SpkcKey;
	int nSize = SpkcData.arSpecFuncData.GetSize();
	for(int i=0; i<nSize; i++)
	{
		m_parPeriod->Add(SpkcData.arSpecFuncData[i].dblPeriod);
		m_parAccel->Add(SpkcData.arSpecFuncData[i].dblValue);
	}
}

void CETC_GDS_USER::MakePOSpectrumData(double dDamping,double SRa,double SRv,CArray <double,double>& aPeriod,CArray <double,double>& aAccel)// for Pushover Curve
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();
 
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	
	T_SPFC_K SpkcKey;
	T_SPFC_D SpkcData;
	CArray<T_SPFC_K,T_SPFC_K> SpfcKeyList;
	pAttrCtrl->GetSpfcKeyList(SpfcKeyList);
	
	int nIndex = m_DsgnSpec.GetCurSel();
	if(nIndex<0 || nIndex>m_arSpfcKey.GetSize()-1) return;
	SpkcKey = m_arSpfcKey[nIndex];
	pAttrCtrl->GetSpfc(SpkcKey,SpkcData);
	m_SpfcK = SpkcKey;
	int nSize = SpkcData.arSpecFuncData.GetSize();
	for(int i=0; i<nSize; i++)
	{
		aPeriod.Add(SpkcData.arSpecFuncData[i].dblPeriod);
		aAccel.Add(SpkcData.arSpecFuncData[i].dblValue*SpkcData.ScaleFactor); 
	}
}
	
void CETC_GDS_USER::End()
{
	UpdateData(TRUE);
	CMyChildDialog::End();
}

void CETC_GDS_USER::EndNotClose()
{
	UpdateData(TRUE);
}

BOOL CETC_GDS_USER::ISVALID()
{
	return TRUE;  
}

void CETC_GDS_USER::ChangeComboWidth()
{
	int nCount = m_DsgnSpec.GetCount();
	if(nCount<1) return;
	
	CString data = _T("");
	m_DsgnSpec.GetLBText(0, data);
	
	CDC* pDC = GetDC();  
	CSize max, cur;
	max = pDC->GetTextExtent(data);
	m_DsgnSpec.SetDroppedWidth(max.cx);
	
	for(int i=1; i<nCount; i++)
	{
		m_DsgnSpec.GetLBText(i, data);
		cur = pDC->GetTextExtent(data); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if(max.cx > m_DsgnSpec.GetDroppedWidth()) 
		m_DsgnSpec.SetDroppedWidth(max.cx);    
	
	ReleaseDC(pDC);
}

BOOL CETC_GDS_USER::GetGravity_UserDefinedSpec(double& dGrav)
{
	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	
	T_SPFC_K SpkcKey;
	T_SPFC_D SpkcData;
	CArray<T_SPFC_K,T_SPFC_K> SpfcKeyList;
	pAttrCtrl->GetSpfcKeyList(SpfcKeyList);
	
	int nIndex = m_DsgnSpec.GetCurSel();
	if(nIndex<0 || nIndex>m_arSpfcKey.GetSize()-1) return FALSE;
	SpkcKey = m_arSpfcKey[nIndex];
	pAttrCtrl->GetSpfc(SpkcKey,SpkcData);
	if(SpkcData.nSpecFuncType != 1) return FALSE;

	dGrav = SpkcData.Gravity;
	return TRUE;
}