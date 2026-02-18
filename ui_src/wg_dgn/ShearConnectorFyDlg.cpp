// CShearConnectorFyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "ShearConnectorFyDlg.h"

//#include "..\wg_base\wg_base_DlgUtil.h"
//#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_db\wg_db_EditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorFyDlg dialog


CShearConnectorFyDlg::CShearConnectorFyDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CShearConnectorFyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShearConnectorFyDlg)	
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}

CShearConnectorFyDlg::~CShearConnectorFyDlg()
{
}

void CShearConnectorFyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShearConnectorFyDlg)
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_FY_CODE_CMB,        m_cmbCode);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_FY_GRADE_CMB,       m_cmbDia);
	//DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_LAT_NUM_EDT,    m_edtLatNum);
	//DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_AREA_LONG_NUM_EDT,   m_edtLongNum);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_FY_EDT,       m_edtFy);
	DDX_Control(pDX, IDC_DGN_DESIGN_CONNECTOR_FY_UNT,       m_untFy);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShearConnectorFyDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectionManagerStiffnessDlg)
	ON_CBN_SELCHANGE(IDC_DGN_DESIGN_CONNECTOR_FY_CODE_CMB,       OnShearConnectorFyCodeCmb)
	ON_CBN_SELCHANGE(IDC_DGN_DESIGN_CONNECTOR_FY_GRADE_CMB,      OnShearConnectorFyDiaCmb)
//   ON_EN_CHANGE    (IDC_DGN_DESIGN_CONNECTOR_AREA_LAT_NUM_EDT,  OnShearConnectorAreaLatNumEdt)
//   ON_EN_CHANGE    (IDC_DGN_DESIGN_CONNECTOR_AREA_LONG_NUM_EDT,  OnShearConnectorAreaLongNumEdt)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CShearConnectorFyDlg::InitUnit()
{
	m_untFy.SetUnitType(D_UNITSYS_BASE_STRESS);
}

void CShearConnectorFyDlg::InitCombo()
{
	// Data setting
	// Set Design Material Data using Analysis Material Data.
	CArray<T_MATL_K, T_MATL_K> arKey;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(arKey);

	T_MATD_D mData;
	int iListNo = 0;
	CString strRebarCode;
	for(int i=0; i<arKey.GetSize(); i++)
	{
		int iID = arKey.GetAt(i);
		mData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(iID, mData))	continue;
		if(mData.Type==_T("C") || mData.Type==_T("SRC"))
		{
			strRebarCode = mData.Rebar_CodeName;
			if(strRebarCode==_T("")) continue;
			else break;
		}
	}

	// Rebar Code Setting
	CArray<CString, CString&> arDesignCode;
	CString strMatlType = _T("C");
	m_pDoc->m_pMatlDB->GetDesignCodeList(strMatlType, arDesignCode);

	m_cmbCode.ResetContent();
	int nSizeCode = arDesignCode.GetSize();
	for (int i = 0; i < nSizeCode; i++)
	{
#if defined(_RUS)
		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(arDesignCode[i]);
#endif
		m_cmbCode.AddString(arDesignCode[i]);
	}

	CString CodeName;
	// Choice of Rebar Code 
	if(nSizeCode>0) 
	{
		int iIndex = 0;
		if(strRebarCode==_T("") || strRebarCode==_T("None"))
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			m_pDoc->m_pInitCtrl->GetPreference(rPref);
			strRebarCode = rPref.DgnCode.ConcRebarMatlCode;
			CodeName = strRebarCode;
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
			iIndex = m_cmbCode.FindStringExact(-1, CodeName);
			//iIndex = 0; // Design Material : None 으로 설계 시 Default code : ASTM
		}
		else
		{
			CodeName = strRebarCode;
#if defined(_RUS)
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(CodeName);
#endif
			iIndex = m_cmbCode.FindStringExact(-1, CodeName);
		}
		m_cmbCode.SetCurSel(iIndex);
	}
	else ASSERT(0);
	
	// Rebar Name Setting
	CArray<CString,CString&> aRBName;
	if (strRebarCode==_T("") || strRebarCode==_T("None"))
	{
		strRebarCode = MATLCODE_CON_ASTM; // Design Material : None 으로 설계 시 Default code : ASTM
		m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, aRBName);
	}
	else
	{
		m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, aRBName);
	}
	m_cmbDia.ResetContent();
	int nSize = aRBName.GetSize();
	for(int i=0; i<nSize; ++i) m_cmbDia.AddString(aRBName[i]);

	if(nSize>0) m_cmbDia.SetCurSel(0);
	else ASSERT(0);

}

void CShearConnectorFyDlg::GetFy()
{
	UpdateData(TRUE);

	T_UNIT_INDEX UnitIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	m_pDoc->m_pMatlDB->Create_RebarData(UnitIndex.nBase_Length, UnitIndex.nBase_Force, TRUE);	// Concrete=TRUE.

	// Data Check
	CString strRebarCode = _T("");
	m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), strRebarCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

	CString strRebarName = _T("");
	m_cmbDia.GetLBText(m_cmbDia.GetCurSel(), strRebarName);


	T_MATL_REBAR RebarData; RebarData.Initialize();
	m_pDoc->m_pMatlDB->GetRebarData(strRebarCode, strRebarName, RebarData);
	double dFy = RebarData.B_fy;

	m_edtFy.SetEditUnit(dFy);
}

void CShearConnectorFyDlg::GetValue(double &dFy)
{
	dFy = m_edtFy.GetEditValue();
}

/////////////////////////////////////////////////////////////////////////////
// CShearConnectorFyDlg message handlers

BOOL CShearConnectorFyDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo();
	GetFy();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShearConnectorFyDlg::OnOK() 
{
	CDialogMove::OnOK();
}

void CShearConnectorFyDlg::OnShearConnectorFyCodeCmb()
{
	// Rebar Name
	CString strRebarCode = _T("");
	m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), strRebarCode);
#if defined(_RUS)
	m_pDoc->m_pEditData->ConvertRusMatlToEngMatl(strRebarCode);
#endif

	CArray<CString,CString&> aRBName;
	m_pDoc->m_pMatlDB->GetRebarNameList(strRebarCode, aRBName);

	m_cmbDia.ResetContent();

	int nSize = aRBName.GetSize();
	for(int i=0; i<nSize; ++i) m_cmbDia.AddString(aRBName[i]);

	if(nSize>0) m_cmbDia.SetCurSel(0);
	else ASSERT(0);

	GetFy();
}

void CShearConnectorFyDlg::OnShearConnectorFyDiaCmb()
{
	GetFy();
}



