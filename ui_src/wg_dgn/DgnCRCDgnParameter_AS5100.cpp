#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCRCDgnParameter_AS5100.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnCRCDgnParameter_AS5100::CDgnCRCDgnParameter_AS5100(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CDgnCRCDgnParameter_AS5100::IDD, pParent)
{
	m_pParamData = pParamData;

	m_bTransfer = FALSE;
	m_bFlexural = FALSE;
	m_bShear = FALSE;
	m_bTorsional = FALSE;
	m_bCrack = FALSE;
	m_bSimplified = FALSE;
}

CDgnCRCDgnParameter_AS5100::~CDgnCRCDgnParameter_AS5100()
{
}


void CDgnCRCDgnParameter_AS5100::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_AS5100)  
	DDX_Control(pDX, IDC_AS5100_DESIGN_PARAMETER9,          m_edtAggregateSize);  
	DDX_Control(pDX, IDC_AS5100_DESIGN_PARAMETER10,         m_untAggregateSize);  
	DDX_Control(pDX, IDC_AS5100_DESIGN_MAX_STL_STRESS_EDT,  m_edtMaxStlStress);  
	DDX_Control(pDX, IDC_AS5100_DESIGN_MAX_STL_STRESS_UNT,  m_untMaxStlStress);  
	DDX_Check  (pDX, IDC_AS5100_PRINT_PARAMETER12,          m_bTransfer);
	DDX_Check  (pDX, IDC_AS5100_PRINT_PARAMETER9,           m_bFlexural);
	DDX_Check  (pDX, IDC_AS5100_PRINT_PARAMETER10,          m_bShear);
	DDX_Check  (pDX, IDC_AS5100_PRINT_PARAMETER11,          m_bTorsional);
	DDX_Check  (pDX, IDC_AS5100_PRINT_PARAMETER16,          m_bCrack);
	DDX_Check  (pDX, IDC_AS5100_DESIGN_SHEAR_STRENGTH_SIMPLIFIED_CHK, m_bSimplified);

	//}}AFX_DATA_MAP
}


void CDgnCRCDgnParameter_AS5100::InitUnit()
{
	m_edtAggregateSize.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dAggregateSize);
	m_untAggregateSize.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dAggregateSize);
	m_edtMaxStlStress.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dMaxStlStress);
	m_untMaxStlStress.SetUnitType(CUnitCtrl::m_PSCD_UNIT.dMaxStlStress);
}

void CDgnCRCDgnParameter_AS5100::Data2Dlg()
{
	m_edtAggregateSize.SetEditUnit(m_pParamData->dAggregateSize);
	m_edtMaxStlStress.SetEditUnit(m_pParamData->dMaxStlStress);

	// 변수 저장 순서 주의
	m_bTransfer  = m_pParamData->bStrePrint4AASHTO[3];
	m_bFlexural  = m_pParamData->bStrePrint4AASHTO[0];
	m_bShear     = m_pParamData->bStrePrint4AASHTO[1];
	m_bTorsional = m_pParamData->bStrePrint4AASHTO[2];

	m_bCrack     = m_pParamData->bServPrint4AASHTO[0];
	m_bSimplified = m_pParamData->bSimplified4Shear;

	UpdateData(FALSE);
}

BOOL CDgnCRCDgnParameter_AS5100::Dlg2Data()
{
	UpdateData(TRUE);

	m_pParamData->dAggregateSize = m_edtAggregateSize.GetEditValue();
	m_pParamData->dMaxStlStress = m_edtMaxStlStress.GetEditValue();

	// 변수 저장 순서 주의
	m_pParamData->bStrePrint4AASHTO[3] = m_bTransfer;
	m_pParamData->bStrePrint4AASHTO[0] = m_bFlexural;
	m_pParamData->bStrePrint4AASHTO[1] = m_bShear;
	m_pParamData->bStrePrint4AASHTO[2] = m_bTorsional;

	m_pParamData->bServPrint4AASHTO[0] = m_bCrack;

	m_pParamData->bSimplified4Shear = m_bSimplified;
	return TRUE;
}


BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_AS5100, CMyChildDialog)
	//{{AFX_MSG_MAP(CDgnCRCDgnParameter_AS5100)
	ON_BN_CLICKED(IDC_AS5100_PRINT_PARAMETER13,   OnDgnAS5100SelAllBtn)
	ON_BN_CLICKED(IDC_AS5100_PRINT_PARAMETER14,   OnDgnAS5100UnSelAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

BOOL CDgnCRCDgnParameter_AS5100::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc->m_pAttrCtrl->ExistPscd())
	{
		m_pParamData->dAggregateSize = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 16.0);
		m_pParamData->dMaxStlStress = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 160.0); 
	}	
	
	InitUnit();
	Data2Dlg();

	return TRUE;  
}

void CDgnCRCDgnParameter_AS5100::OnDgnAS5100SelAllBtn()
{
	m_bTransfer  = TRUE;
	m_bFlexural  = TRUE;
	m_bShear     = TRUE;
	m_bTorsional = TRUE;
	m_bCrack     = TRUE;

	UpdateData(FALSE);
}

void CDgnCRCDgnParameter_AS5100::OnDgnAS5100UnSelAllBtn()
{
	m_bTransfer  = FALSE;
	m_bFlexural  = FALSE;
	m_bShear     = FALSE;
	m_bTorsional = FALSE;
	m_bCrack     = FALSE;

	UpdateData(FALSE);
}