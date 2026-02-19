#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBarInfoDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarInfoDlg dialog


CDgnConBarInfoDlg::CDgnConBarInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBarInfoDlg::IDD, pParent)
{
	m_pMatlDB = CDBDoc::GetDocPoint()->m_pMatlDB;

	T_UNIT_INDEX CurIndex;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	m_pMatlDB->Create_RebarData(CurIndex.nBase_Length, CurIndex.nBase_Force, TRUE);

	m_pConBarInfoGrid = new CConBarInfoGrid(m_pMatlDB);
}

CDgnConBarInfoDlg::~CDgnConBarInfoDlg()
{
	_SAFE_DELETE(m_pConBarInfoGrid)
}

void CDgnConBarInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DGN_CON_BAR_INFO_STANDARD_CMB, m_cmbStandard);
	DDX_Control(pDX, IDC_DGN_CON_BAR_INFO_GRD, *m_pConBarInfoGrid);
}

BEGIN_MESSAGE_MAP(CDgnConBarInfoDlg, CDialogMove)
	//ON_CBN_SELCHANGE(IDC_DGN_CON_BAR_INFO_STANDARD_CMB, OnSelChangeStandardCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarInfoDlg message handlers

void CDgnConBarInfoDlg::OnOK()
{
	// 선택된 철근직경을 문자로 만듭니다.
	m_pConBarInfoGrid->Grid2Data(m_strTotalReBarSize);

	if (m_strTotalReBarSize == _T("Over"))
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_OVER_ITEM), MB_OK);
		return;
	}

	if (m_strTotalReBarSize == _T(""))
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM), MB_OK);
		return;
	}
	CDialogMove::OnOK();
}

void CDgnConBarInfoDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

BOOL CDgnConBarInfoDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	CRect rect;
	GetDlgItem(IDC_DGN_CON_BAR_INFO_GRD)->GetWindowRect(rect);

	m_pConBarInfoGrid->SetGridWndSize(rect.Width(), rect.Height());
	m_pConBarInfoGrid->Initialize(this);

	InitCmbStandard();

	UpdateData(FALSE);
	return TRUE;
}

void CDgnConBarInfoDlg::OnSelChangeStandardCombo()
{
	CString strCode = _T("");
	m_cmbStandard.GetLBText(m_cmbStandard.GetCurSel(), strCode);
	
	int iME = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RbarKindCon();
	if ( m_pMatlDB->IsValidRebarCode(iME) )
	{
		CStringArray rebarNameList;
		CDBDoc::GetDocPoint()->m_pMatlDB->GetRebarNameList(iME, rebarNameList);
		m_pConBarInfoGrid->Data2Grid(rebarNameList, m_strTotalReBarSize);
	}
}

void CDgnConBarInfoDlg::InitCmbStandard()
{
	//Add Item
	CArray<CString, CString&> MatCodeList;
	CString sMatType = _T("C");
	m_pMatlDB->GetDesignCodeList(sMatType, MatCodeList);

	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(KS), EN_BAR_KS);
    CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(CNS560-18), EN_BAR_CNS560_18);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(CNS), EN_BAR_CNS);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(JIS), EN_BAR_JIS);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(ASTM), EN_BAR_ASTM);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(BS/EN), EN_BAR_BSEN);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(IS), EN_BAR_IS);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(UNI), EN_BAR_UNI);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(SS), EN_BAR_SS);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(GB), EN_BAR_GB);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(CSA), EN_BAR_CSA);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(GOST-SP), EN_BAR_GOST_SP);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(GOST-SNIP), EN_BAR_GOST_SNIP);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(AS/NZS), EN_BAR_AS);
    CDlgUtil::CobxAddItem(m_cmbStandard, _ULS('US CUSTMARY(US)'), EN_BAR_US_C_US);
    CDlgUtil::CobxAddItem(m_cmbStandard, _ULS('US CUSTMARY(SI)'), EN_BAR_US_C_SI);
    CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(PNS49), EN_BAR_PNS49);
    CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(SNI), EN_BAR_SNI);
    CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(TIS), EN_BAR_TIS);
    CDlgUtil::CobxAddItem(m_cmbStandard, _ULS('TIS(MKS)'), EN_BAR_TIS_MKS);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(NMX-2013), EN_BAR_NMX2013);
	CDlgUtil::CobxAddItem(m_cmbStandard, _ULS('NMX-2013(MKS)'), EN_BAR_NMX2013_MKS);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(THM7), EN_BAR_TMH7);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(SP63.2018), EN_BAR_SP63_2018);
	CDlgUtil::CobxAddItem(m_cmbStandard, _LSX(TS), EN_BAR_TS);
	CDlgUtil::CobxAddItem(m_cmbStandard, _ULS('ABNT NBR 7480'), EN_BAR_ABNT_NBR_7480);

	int iME = CDBDoc::GetDocPoint()->m_pMatlDB->Get_RbarKindCon();
	CDlgUtil::CobxSetCurSelItemData(m_cmbStandard, iME);

	OnSelChangeStandardCombo();
}