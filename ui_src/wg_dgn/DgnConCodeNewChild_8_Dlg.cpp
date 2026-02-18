// DgnConCodeNewChild_8_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_8_Dlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "DgnDataCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild8Dlg dialog


CDgnConCodeNewChild8Dlg::CDgnConCodeNewChild8Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild8Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild8Dlg)
// 	m_pData = pData;
	
	m_dTrft = 0.0;
	m_dMrft = 0.0;
	m_bTorsionDesign = FALSE;
	m_bShowTorsion = TRUE;
	m_nTorsionGap = 0;

	m_aSafetyLevel.RemoveAll();
	m_aSafetyLevel.Add(IDC_DGN_SAFETY_LEVEL0);
	m_aSafetyLevel.Add(IDC_DGN_SAFETY_LEVEL1);
	m_aSafetyLevel.Add(IDC_DGN_SAFETY_LEVEL2);
	m_aSafetyLevel.Add(IDC_DGN_SAFETY_LEVEL3);

	m_aEarthquake.RemoveAll();
	m_aEarthquake.Add(IDC_DGN_EQ_GRADE0);
	m_aEarthquake.Add(IDC_DGN_EQ_GRADE1);
	m_aEarthquake.Add(IDC_DGN_EQ_GRADE2);
	m_aEarthquake.Add(IDC_DGN_EQ_GRADE3);
	m_aEarthquake.Add(IDC_DGN_EQ_GRADE4);
	m_aEarthquake.Add(IDC_DGN_EQ_GRADE5);
	m_aEarthquake.Add(IDC_DGN_EQ_GRADE6);

	m_aSlabType.RemoveAll();
	m_aSlabType.Add(IDC_DGN_SLABTYPE0);
	m_aSlabType.Add(IDC_DGN_SLABTYPE1);
	m_aSlabType.Add(IDC_DGN_SLABTYPE2);

	m_aTorsionDesign.RemoveAll();
	m_aTorsionDesign.Add(IDC_DGN_TORSION_GROUP);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TORSION);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_STATIC);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_FACTOR);

	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

    m_pDgnDataCtrl = new CDgnDataCtrl;
}

CDgnConCodeNewChild8Dlg::~CDgnConCodeNewChild8Dlg()
{
    _SAFE_DELETE(m_pDgnDataCtrl);
}

void CDgnConCodeNewChild8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild8Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION, m_bTorsionDesign);

	DDX_Radio(pDX, IDC_DGN_SAFETY_LEVEL1, m_iSafeLevel);
	DDX_Radio(pDX, IDC_DGN_EQ_GRADE1, m_iEqGrade);
	DDX_Radio(pDX, IDC_DGN_SLABTYPE1, m_iSlabType);
	
	DDX_Text(pDX, IDC_DGN_RCCTRL_TRFT_FACTOR, m_dTrft);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);

	DDX_Control(pDX,  IDC_DGN_RCCTRL_TORSION, m_btnTorsion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild8Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild8Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_TORSION, OnChangeTorsion)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild8Dlg message handlers

BOOL CDgnConCodeNewChild8Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild8Dlg::InitCtrl()
{

	UINT nIDRef = IDC_DGN_SAFETY_LEVEL0, nIDTgt = IDC_DGN_EQ_GRADE0; 
	CRect RectRef, RectTgt;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);

	int nDistX = RectRef.left - RectTgt.left;

	// 우선 Select Safety Level of Building Structure와 Select Earthquake-Resisitant Grade를 같은 위치에 놓는다.
	CDlgUtil::CtrlMoveDistX(this, m_aEarthquake, nDistX);


	CRect RefRect, TgtRect;
	GetDlgItem(IDC_DGN_TORSION_GROUP)->GetWindowRect(&RefRect);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&TgtRect);

	m_nTorsionGap = RefRect.top - TgtRect.top;

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if(!m_bExistData)
	{
		m_pDgnDataCtrl->Get_DgnConChinaDef(strCode, m_iSafeLevel, m_iEqGrade);
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
		m_pDgnDataCtrl->Get_DgnConTrftDef(strCode, m_dTrft); //m_dTrft = 1.0;//TW에서만 사용
	}
	else
	{
		m_dMrft = m_pData->dMrft;
		m_dTrft = m_pData->dTrft;
		m_bTorsionDesign = m_pData->bTorsionDesign;
	}

	//SetComboBox();
	SetShowTorsion(strCode);
	UpdateData(FALSE);
	
	//OnChangeTorsion();

	ResizeChildDlg();

	//SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
	//EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild8Dlg::SetShowTorsion(const CString& strCode)
{
	BOOL bShowTorsion_new = (strCode == _T("GB/T50010-10") || strCode == _T("GB50010-19")) ? TRUE : FALSE;

	if(m_bShowTorsion == bShowTorsion_new) return;

	int nDistY=m_nTorsionGap;
	if(!m_bShowTorsion) nDistY *= -1;

	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);

	m_bShowTorsion = bShowTorsion_new;
}
void CDgnConCodeNewChild8Dlg::OnDgnRcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ResizeChildDlg();

//   CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
//   SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
}
void CDgnConCodeNewChild8Dlg::OnChangeTorsion()
{
	if(m_btnTorsion.GetCheck())
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(FALSE);
	}
}

void CDgnConCodeNewChild8Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_dMrft           = m_pData->dMrft;
	m_dTrft           = m_pData->dTrft;
	m_iSlabType       = m_pData->iSlabType;
	m_bTorsionDesign  = m_pData->bTorsionDesign;
	
}

void CDgnConCodeNewChild8Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);

	SetShowTorsion(strCode);
	ResizeChildDlg();

	m_pDgnDataCtrl->Get_DgnConChinaDef(strCode, m_iSafeLevel, m_iEqGrade);
	m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
	m_pDgnDataCtrl->Get_DgnConTrftDef(strCode, m_dTrft); //m_dTrft = 1.0;//TW에서만 사용

	UpdateData(FALSE);
	
}
void CDgnConCodeNewChild8Dlg::ResizeChildDlg()
{ 
	CDlgUtil::CtrlShowHide(this, m_aSafetyLevel, !m_bSpecialEQ);
	CDlgUtil::CtrlShowHide(this, m_aEarthquake,  m_bSpecialEQ);
	
	UINT nIDRef = IDC_DGN_SAFETY_LEVEL0, nIDTgt = IDC_DGN_SLABTYPE0; 
	if(m_bSpecialEQ) nIDRef = IDC_DGN_EQ_GRADE0;
	
	CRect RectRef, RectTgt, RectGap;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
	GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectGap);

	int nDistY = RectRef.bottom - RectTgt.top + RectGap.Height()/2;

	CDlgUtil::CtrlMoveDistY(this, m_aSlabType, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aTorsionDesign, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);

	CDlgUtil::CtrlShowHide(this, m_aTorsionDesign, m_bShowTorsion);

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);
	
	CDconChildDialog::ResizeChildDlg();
}

BOOL CDgnConCodeNewChild8Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft       = m_dMrft;
	m_pData->dTrft       = m_dTrft;
	m_pData->iSlabType   = m_iSlabType;
	
	m_pData->bTorsionDesign = m_bTorsionDesign;

	CDconChildDialog::Dlg2Data();
	return TRUE;
}