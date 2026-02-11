// CMMvhlItemUsrTrckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrTrckDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\AttrCtrl.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CInternationalDlg

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrTrckDlg dialog

CCMMvhlItemUsrTrckDlg::CCMMvhlItemUsrTrckDlg(T_MVHL_D  *pData, CWnd* pParent/*=NULL*/)
	: CDialog(CCMMvhlItemUsrTrckDlg::IDD, pParent)
{
	m_pData=NULL;
	ASSERT(pData);
	m_pData = pData;
	m_DataBuffForGrid = *m_pData;
	if(pData->nLoadType == 2) m_bChSubway = TRUE;
	else m_bChSubway = FALSE;

	CDBDoc* pDoc;
	pDoc = CDBDoc::GetDocPoint();
	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) ASSERT(0);
	m_nMovingType = MvcdD.nCodeType;

	m_dDlgH = 0.0;
	//{{AFX_DATA_INIT(CCMMvhlItemUsrTrckDlg)
	m_sD1 = _T("");
	m_sD2 = _T("");
	m_sW1 = _T("");
	m_sW2 = _T("");
	m_sIrcD1 = _T("");
	m_sIrcD2 = _T("");
	m_sIrcP  = _T("");
	m_sIrcD  = _T("");
	//}}AFX_DATA_INIT
	
	m_aCtrlCom.Add(IDC_CMD_dW1_STATIC);
	m_aCtrlCom.Add(IDC_CMD_dW1);
	m_aCtrlCom.Add(IDC_CMD_dW1_UNIT);
	m_aCtrlCom.Add(IDC_CMD_dD1_STATIC);
	m_aCtrlCom.Add(IDC_CMD_dD1);
	m_aCtrlCom.Add(IDC_CMD_dD1_UNIT);
	m_aCtrlCom.Add(IDC_CMD_dW2_STATIC);
	m_aCtrlCom.Add(IDC_CMD_dW2);
	m_aCtrlCom.Add(IDC_CMD_dW2_UNIT);
	m_aCtrlCom.Add(IDC_CMD_dD2_STATIC);
	m_aCtrlCom.Add(IDC_CMD_dD2);
	m_aCtrlCom.Add(IDC_CMD_dD2_UNIT);
	
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_dD1_STATIC);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_dD1);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_dD1_UNIT);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_dD2_STATIC);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_dD2);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_dD2_UNIT);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_P_STATIC);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_P);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_P_UNIT);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_D_STATIC);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_D);
	m_aCtrlIrc.Add(IDC_CMD_MVHL_IRC_D_UNIT);

	m_aCtrlSub.Add(IDC_CMD_MVHL_P1_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P1_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P1_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P2_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P2_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P2_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P3_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P3_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P3_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P4_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P4_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_P4_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_PO_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_PO_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_PO_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D1_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D1_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D1_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D2_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D2_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D2_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D3_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D3_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_D3_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_DD_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_DD_EDIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_DD_UNIT);
	m_aCtrlSub.Add(IDC_CMD_MVHL_N_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_N_EDIT);
	//m_aCtrlSub.Add(IDC_CMD_MVHL_N_SPIN);
	m_aCtrlSub.Add(IDC_CMD_MVHL_IFR_STATIC);
	m_aCtrlSub.Add(IDC_CMD_MVHL_IFR_EDIT);
}


void CCMMvhlItemUsrTrckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrTrckDlg)
	DDX_Control(pDX, IDC_CMD_dW2_UNIT, m_stW2UT);
	DDX_Control(pDX, IDC_CMD_dW1_UNIT, m_stW1UT);
	DDX_Control(pDX, IDC_CMD_dD2_UNIT, m_stD2UT);
	DDX_Control(pDX, IDC_CMD_dD1_UNIT, m_stD1UT);
	DDX_Text(pDX, IDC_CMD_dD1, m_sD1);
	DDX_Text(pDX, IDC_CMD_dD2, m_sD2);
	DDX_Text(pDX, IDC_CMD_dW1, m_sW1);
	DDX_Text(pDX, IDC_CMD_dW2, m_sW2);

	DDX_Control(pDX, IDC_CMD_MVHL_IRC_NTDIST_UNIT, m_untIrcNtDist);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_dD1_UNIT, m_untIrcD1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_dD2_UNIT, m_untIrcD2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_P_UNIT,   m_untIrcP);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_D_UNIT,   m_untIrcD);
	DDX_Text(pDX, IDC_CMD_MVHL_IRC_NTDIST, m_sIrcNtDist);
	DDX_Text(pDX, IDC_CMD_MVHL_IRC_dD1, m_sIrcD1);
	DDX_Text(pDX, IDC_CMD_MVHL_IRC_dD2, m_sIrcD2);
	DDX_Text(pDX, IDC_CMD_MVHL_IRC_P, m_sIrcP);
	DDX_Text(pDX, IDC_CMD_MVHL_IRC_D, m_sIrcD);
	DDX_Control(pDX, IDC_CMD_MVHL_P1_EDIT, m_edtP1);
	DDX_Control(pDX, IDC_CMD_MVHL_P2_EDIT, m_edtP2);
	DDX_Control(pDX, IDC_CMD_MVHL_P3_EDIT, m_edtP3);
	DDX_Control(pDX, IDC_CMD_MVHL_P4_EDIT, m_edtP4);
	DDX_Control(pDX, IDC_CMD_MVHL_PO_EDIT, m_edtPo);
	DDX_Control(pDX, IDC_CMD_MVHL_D1_EDIT, m_edtD1);
	DDX_Control(pDX, IDC_CMD_MVHL_D2_EDIT, m_edtD2);
	DDX_Control(pDX, IDC_CMD_MVHL_D3_EDIT, m_edtD3);
	DDX_Control(pDX, IDC_CMD_MVHL_DD_EDIT, m_edtD);
	DDX_Control(pDX, IDC_CMD_MVHL_P1_UNIT, m_untP1);
	DDX_Control(pDX, IDC_CMD_MVHL_P2_UNIT, m_untP2);
	DDX_Control(pDX, IDC_CMD_MVHL_P3_UNIT, m_untP3);
	DDX_Control(pDX, IDC_CMD_MVHL_P4_UNIT, m_untP4);
	DDX_Control(pDX, IDC_CMD_MVHL_PO_UNIT, m_untPo);
	DDX_Control(pDX, IDC_CMD_MVHL_D1_UNIT, m_untD1);
	DDX_Control(pDX, IDC_CMD_MVHL_D2_UNIT, m_untD2);
	DDX_Control(pDX, IDC_CMD_MVHL_D3_UNIT, m_untD3);
	DDX_Control(pDX, IDC_CMD_MVHL_DD_UNIT, m_untD);
	//DDX_Control(pDX, IDC_CMD_MVHL_N_SPIN, m_spnCrowd);

	DDX_Control(pDX, IDC_CMD_MVHL_N_EDIT,   m_edtN);
	DDX_Text(pDX, IDC_CMD_MVHL_IFR_EDIT, m_strIfr);

	DDX_Control(pDX, IDC_CMD_MVLD_TLGRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvhlItemUsrTrckDlg, CDialog)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrTrckDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrTrckDlg message handlers


/////////////////////////////////////////////////////////////////////////////
// DATE : 2002.1.15. by KYE-HONG
// FUNC : data exchange
/////////////////////////////////////////////////////////////////////////////
void	CCMMvhlItemUsrTrckDlg::SetData2Dlg()
{
	ASSERT(m_pData); if(!m_pData) return;

	//if (!m_wndGrid.MakeItemEx(D_TB_SAVE_ALWAYS)) return FALSE;
	
	m_wndGrid.MakeItemEx();

	m_sD1.Format(_T("%g"),m_pData->dD1);
	m_sD2.Format(_T("%g"),m_pData->dD2);
	m_sW1.Format(_T("%g"),m_pData->dW1);
	m_sW2.Format(_T("%g"),m_pData->dW2);

	m_sIrcNtDist.Format(_T("%g"), m_pData->dNoseTailDist);
	m_sIrcD1.Format(_T("%g"),m_pData->dD1);
	m_sIrcD2.Format(_T("%g"),m_pData->dD2);
	m_sIrcP.Format(_T("%g"), m_pData->dP);
	m_sIrcD.Format(_T("%g"), m_pData->dDTrailer);

	m_edtP1.SetEditUnit(m_pData->dPSubway[0]);
	m_edtP2.SetEditUnit(m_pData->dPSubway[1]);
	m_edtP3.SetEditUnit(m_pData->dPSubway[2]);
	m_edtP4.SetEditUnit(m_pData->dPSubway[3]);
	m_edtPo.SetEditUnit(m_pData->dPopposite);
	m_edtD1.SetEditUnit(m_pData->dDSubway[0]);
	m_edtD2.SetEditUnit(m_pData->dDSubway[1]);
	m_edtD3.SetEditUnit(m_pData->dDSubway[2]);
	m_edtD.SetEditUnit(m_pData->dCarriageDistance);
	m_edtN.SetValue(m_pData->nNumCarriage);
	m_strIfr.Format(_T("%g"), m_pData->dImpactCoef);

	UpdateData(FALSE);
}

double  CCMMvhlItemUsrTrckDlg::GetDlgH()
{
	return m_dDlgH;
}

BOOL	CCMMvhlItemUsrTrckDlg::SetDlg2Data()
{
	ASSERT(m_pData); if(!m_pData) return FALSE;

	// 만약 편집중이면 편집중인 데이타 m_DataBuffForGrid에 저장 + Parent대화상자의 m_Data에 적용
	if (!m_wndGrid.OnTerminate(D_TB_SAVE_ALWAYS)) return FALSE;
	for (int i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		m_pData->dPointLoad[i] = m_DataBuffForGrid.dPointLoad[i];
		m_pData->dPointDistance[i] = m_DataBuffForGrid.dPointDistance[i];
	}

	UpdateData(TRUE);

	if(m_nTrainType == 2)
	{
		m_pData->dD1 = _tstof(m_sD1);
		m_pData->dD2 = 0.;
		m_pData->dW1 = 0.;
		m_pData->dW2 = 0.;
	}
	else if(m_nTrainType == 3) // IRC:6-2000
	{
		m_pData->dNoseTailDist = _tstof(m_sIrcNtDist);
		m_pData->dD1 = _tstof(m_sIrcD1);
		m_pData->dD2 = _tstof(m_sIrcD2);
		m_pData->dP = _tstof(m_sIrcP);
		m_pData->dDTrailer = _tstof(m_sIrcD);
	}
	else if(m_nTrainType == 4) // China(Train(Subway))
	{
		m_pData->dPSubway[0] = m_edtP1.GetEditValue();
		m_pData->dPSubway[1] = m_edtP2.GetEditValue();
		m_pData->dPSubway[2] = m_edtP3.GetEditValue();
		m_pData->dPSubway[3] = m_edtP4.GetEditValue();
		m_pData->dPopposite  = m_edtPo.GetEditValue();
		m_pData->dDSubway[0] = m_edtD1.GetEditValue();
		m_pData->dDSubway[1] = m_edtD2.GetEditValue();
		m_pData->dDSubway[2] = m_edtD3.GetEditValue();
		m_pData->dCarriageDistance = m_edtD.GetEditValue();
		m_pData->nNumCarriage = m_edtN.GetEditValueInt();
		m_pData->dImpactCoef  = _tstof(m_strIfr);
	}
	else
	{
		m_pData->dD1 = _tstof(m_sD1);
		m_pData->dD2 = _tstof(m_sD2);
		m_pData->dW1 = _tstof(m_sW1);
		m_pData->dW2 = _tstof(m_sW2);
	}
	return TRUE;
}

BOOL CCMMvhlItemUsrTrckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AlignCtrl();
	// unit 초기화 
	SetUnitCtrl();
	
	// grid 초기화 
	m_wndGrid.Initialize(&m_DataBuffForGrid);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	
	m_edtN.SetRange(1, 15);
	m_edtN.SetValue(1);
	m_edtN.SetInteger(TRUE);

	// data initialize
	SetData2Dlg();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrTrckDlg::AlignCtrl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;	

	GetDlgItem(IDC_CMD_dW1)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_IRC_dD1)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlIrc, nDistY);
		
	// Crowd Load
	GetDlgItem(IDC_CMD_MVLD_TLGRID)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_P1_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSub, nDistY);

	// resize self size
	CRect rectLast;
	
	if(m_nMovingType == D_MOVE_CODE_INDIA)
		GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST)->GetWindowRect(rectLast);
	else
		GetDlgItem(IDC_CMD_STATIC)->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+2;
	m_dDlgH = r.bottom - r.top;

	MoveWindow(r);

	CtrlShowHide();

}
void	CCMMvhlItemUsrTrckDlg::SetUnitCtrl(void)
{
	m_stW2UT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW2);
	m_stW1UT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dW1);
	m_stD2UT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_stD1UT.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);

	m_untIrcNtDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_untIrcD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_untIrcD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untIrcP.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dP);
	m_untIrcD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);

	m_untP1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_untP2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_untP3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_untP4.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_untPo.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPopposite);
	m_untD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDSubway);
	m_untD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDSubway);
	m_untD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDSubway);
	m_untD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCarriageDistance);

	m_edtP1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_edtP2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_edtP3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_edtP4.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPSubway);
	m_edtPo.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPopposite);
	m_edtD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDSubway);
	m_edtD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDSubway);
	m_edtD3.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDSubway);
	m_edtD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCarriageDistance);
	
}

BOOL CCMMvhlItemUsrTrckDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		pMsg->wParam=9;
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
		pMsg->wParam=VK_TAB;	
	return CInternationalDlg::PreTranslateMessage(pMsg);
}

void CCMMvhlItemUsrTrckDlg::CtrlShowHide()
{
	CDlgUtil::CtrlShowHide(this, m_aCtrlCom, m_nMovingType != D_MOVE_CODE_INDIA);
	CDlgUtil::CtrlShowHide(this, m_aCtrlIrc, m_nMovingType == D_MOVE_CODE_INDIA);
	CDlgUtil::CtrlShowHide(this, m_aCtrlSub, m_nMovingType == D_MOVE_CODE_CHINA && m_bChSubway);
	GetDlgItem(IDC_CMD_MVLD_TLGRID)->ShowWindow(!(m_nMovingType == D_MOVE_CODE_CHINA && m_bChSubway));
	GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA);
	GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST)       ->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA);
	GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST_UNIT)  ->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA);
	if(m_nMovingType == D_MOVE_CODE_INDIA)
		GetDlgItem(IDC_CMD_STATIC)->SetWindowText(_LS(IDS_CMD_IRC_WHEEL_VEHICLE));
	else if(m_nMovingType == D_MOVE_CODE_CHINA && m_bChSubway)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlCom, FALSE);
		GetDlgItem(IDC_CMD_STATIC)->SetWindowText(_LS(IDS_CMD_TRAIN_SUB_LOAD));
	}
	else
		GetDlgItem(IDC_CMD_STATIC)->SetWindowText(_LS(IDS_CMD_TRAIN_LOAD));
}