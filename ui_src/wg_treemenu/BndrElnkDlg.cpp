// BndrElnkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrElnkDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\ViewBuff.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_cmd2\MlfcListDlg.h"
#include "..\wg_cmd2\RlfcListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrElnkDlg dialog

CBndrElnkDlg::CBndrElnkDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrElnkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrElnkDlg)
	//}}AFX_DATA_INIT

	// Add, Delete
	m_aCtrlOption.Add(IDC_TM_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE);

	//m_aCtrlDisable.Add(IDC_TM_RIGD_CHK);
	m_aCtrlDisable.Add(IDC_TM_SDX_EDIT);
	m_aCtrlDisable.Add(IDC_TM_SDY_EDIT);
	m_aCtrlDisable.Add(IDC_TM_SDZ_EDIT);
	m_aCtrlDisable.Add(IDC_TM_SRX_EDIT);
	m_aCtrlDisable.Add(IDC_TM_SRY_EDIT);
	m_aCtrlDisable.Add(IDC_TM_SRZ_EDIT);
	m_aCtrlDisable.Add(IDC_TM_BETA_ANGLE);
	m_aCtrlDisable.Add(IDC_TM_2NODES_EDIT);
	m_aCtrlDisable.Add(IDC_TM_COPY_LOAD);
	m_aCtrlDisable.Add(IDC_TM_SHEAR_CHECK);
	m_aCtrlDisable.Add(IDC_TM_DR_SDY_EDIT);
	m_aCtrlDisable.Add(IDC_TM_DR_SDZ_EDIT);

	m_aCtrlCopyLoad.Add(IDC_TM_RADIO_NODE_INC);
	m_aCtrlCopyLoad.Add(IDC_TM_RADIO_DISTANCE);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_TITLE);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_X);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_Y);
	m_aCtrlCopyLoad.Add(IDC_TM_AXIS_Z);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_TITLE);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_EDIT);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_UNIT);
	m_aCtrlCopyLoad.Add(IDC_TM_DIST_EXAM);
	m_aCtrlCopyLoad.Add(IDC_TM_TEXT_NTIMES);
	m_aCtrlCopyLoad.Add(IDC_TM_EDIT_NTIMES);
	//m_aCtrlCopyLoad.Add(IDC_TM_SPIN_NTIMES);
	m_aCtrlCopyLoad.Add(IDC_TM_TEXT_NODEINC);
	m_aCtrlCopyLoad.Add(IDC_TM_EDIT_NODEINC);
	//m_aCtrlCopyLoad.Add(IDC_TM_SPIN_NODEINC);

	m_aCtrlCopyType.Add(IDC_TM_RADIO_NODE_INC);
	m_aCtrlCopyType.Add(IDC_TM_RADIO_DISTANCE);

	m_aCtrlCopyDist.Add(IDC_TM_AXIS_TITLE);
	m_aCtrlCopyDist.Add(IDC_TM_AXIS_X);
	m_aCtrlCopyDist.Add(IDC_TM_AXIS_Y);
	m_aCtrlCopyDist.Add(IDC_TM_AXIS_Z);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_TITLE);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_EDIT);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_UNIT);
	m_aCtrlCopyDist.Add(IDC_TM_DIST_EXAM);

	m_aCtrlCopyNodeInc.Add(IDC_TM_TEXT_NTIMES);
	m_aCtrlCopyNodeInc.Add(IDC_TM_EDIT_NTIMES);
	//m_aCtrlCopyNodeInc.Add(IDC_TM_SPIN_NTIMES);
	m_aCtrlCopyNodeInc.Add(IDC_TM_TEXT_NODEINC);
	m_aCtrlCopyNodeInc.Add(IDC_TM_EDIT_NODEINC);
	//m_aCtrlCopyNodeInc.Add(IDC_TM_SPIN_NODEINC);

	m_aCtrlAxis.Add(IDC_TM_AXIS_X);
	m_aCtrlAxis.Add(IDC_TM_AXIS_Y);
	m_aCtrlAxis.Add(IDC_TM_AXIS_Z);


	m_aCtrlSpring.Add(IDC_TM_SDX_EDIT);
	m_aCtrlSpring.Add(IDC_TM_SDY_EDIT);
	m_aCtrlSpring.Add(IDC_TM_SDZ_EDIT);
	m_aCtrlSpring.Add(IDC_TM_SRX_EDIT);
	m_aCtrlSpring.Add(IDC_TM_SRY_EDIT);
	m_aCtrlSpring.Add(IDC_TM_SRZ_EDIT);
	m_aCtrlSpring.Add(IDC_TM_SDX_UNIT);
	m_aCtrlSpring.Add(IDC_TM_SDY_UNIT);
	m_aCtrlSpring.Add(IDC_TM_SDZ_UNIT);
	m_aCtrlSpring.Add(IDC_TM_SRX_UNIT);
	m_aCtrlSpring.Add(IDC_TM_SRY_UNIT);
	m_aCtrlSpring.Add(IDC_TM_SRZ_UNIT);
	m_aCtrlNoLimited.Append(m_aCtrlSpring);
	m_aCtrlSpring.Add(IDC_TM_SDX_STC);
	m_aCtrlSpring.Add(IDC_TM_SDY_STC);
	m_aCtrlSpring.Add(IDC_TM_SDZ_STC);
	m_aCtrlSpring.Add(IDC_TM_SRX_STC);
	m_aCtrlSpring.Add(IDC_TM_SRY_STC);
	m_aCtrlSpring.Add(IDC_TM_SRZ_STC);
	m_aCtrlSpring.Add(IDC_TM_ELNK_FIXED_GRP);
	m_aCtrlSpring.Add(IDC_TM_ELNK_SDX_CHK);
	m_aCtrlSpring.Add(IDC_TM_ELNK_SDY_CHK);
	m_aCtrlSpring.Add(IDC_TM_ELNK_SDZ_CHK);
	m_aCtrlSpring.Add(IDC_TM_ELNK_SRX_CHK);
	m_aCtrlSpring.Add(IDC_TM_ELNK_SRY_CHK);
	m_aCtrlSpring.Add(IDC_TM_ELNK_SRZ_CHK);

	m_aCtrlLiDisp.Add(IDC_TM_SDX_EDIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SDY_EDIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SDZ_EDIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SRX_EDIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SRY_EDIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SRZ_EDIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SDX_UNIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SDY_UNIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SDZ_UNIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SRX_UNIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SRY_UNIT2);
	m_aCtrlLiDisp.Add(IDC_TM_SRZ_UNIT2);

	m_aCtrlDistRatio.Add(IDC_TM_DR_DIST_ENDI_STC);
	m_aCtrlDistRatio.Add(IDC_TM_DR_SDY_STC);
	m_aCtrlDistRatio.Add(IDC_TM_DR_SDY_EDIT);
	m_aCtrlDistRatio.Add(IDC_TM_DR_SDZ_STC);
	m_aCtrlDistRatio.Add(IDC_TM_DR_SDZ_EDIT);

	m_aCtrlMultiLinear.Add(IDC_TM_ELNK_DIR_STC);
	m_aCtrlMultiLinear.Add(IDC_TM_ELNK_DIR_CMB);
	m_aCtrlMultiLinear.Add(IDC_TM_ELNK_MLFC_STC);
	m_aCtrlMultiLinear.Add(IDC_TM_ELNK_MLFC_COMBO);
	m_aCtrlMultiLinear.Add(IDC_TM_ELNK_MLFC_BUTTON);

	m_aCtrlShearSpringLoc.Add(IDC_TM_SHEAR_CHECK);
	m_aCtrlShearSpringLoc.Add(IDC_TM_DR_DIST_ENDI_STC);
	m_aCtrlShearSpringLoc.Add(IDC_TM_DR_SDY_STC);
	m_aCtrlShearSpringLoc.Add(IDC_TM_DR_SDY_EDIT);
	m_aCtrlShearSpringLoc.Add(IDC_TM_DR_SDZ_STC);
	m_aCtrlShearSpringLoc.Add(IDC_TM_DR_SDZ_EDIT);
	m_aCtrlShearSpringLoc.Add(IDC_TM_ELNK_DIST_ENDI_STC);
	m_aCtrlShearSpringLoc.Add(IDC_TM_ELNK_DIST_ENDI_EDT);
	m_aCtrlShearSpringLoc.Add(IDC_TM_SHEAR_CHECK_GROUP);

	m_aCtrlBetaAngle.Add(IDC_TM_BETA_ANGLE_STC);
	m_aCtrlBetaAngle.Add(IDC_TM_BETA_ANGLE);
	m_aCtrlBetaAngle.Add(IDC_TM_BETA_ANGLE_UNT);
	
	m_aCtrlSelNodes.Add(IDC_WG_TREEMENU_STATIC2);
	m_aCtrlSelNodes.Add(IDC_TM_2NODES_EDIT);

	m_aCtrlCopyElstLink.Add(IDC_TM_COPY_LOAD);
	m_aCtrlCopyElstLink.Add(IDC_WG_TREEMENU_STATIC3);
	m_aCtrlCopyElstLink.Add(IDC_TM_RADIO_NODE_INC);
	m_aCtrlCopyElstLink.Add(IDC_TM_RADIO_DISTANCE);
	m_aCtrlCopyElstLink.Add(IDC_TM_AXIS_TITLE);
	m_aCtrlCopyElstLink.Add(IDC_TM_AXIS_X);
	m_aCtrlCopyElstLink.Add(IDC_TM_AXIS_Y);
	m_aCtrlCopyElstLink.Add(IDC_TM_AXIS_Z);
	m_aCtrlCopyElstLink.Add(IDC_TM_DIST_TITLE);
	m_aCtrlCopyElstLink.Add(IDC_TM_DIST_EDIT);
	m_aCtrlCopyElstLink.Add(IDC_TM_DIST_UNIT);
	m_aCtrlCopyElstLink.Add(IDC_TM_DIST_EXAM);

	m_aCtrlApplyClose.Add(IDC_TM_EXECUTE);
	m_aCtrlApplyClose.Add(IDC_TM_CLOSE);

	m_aCtrlDistRatioEndI.Add(IDC_TM_ELNK_DIST_ENDI_STC);
	m_aCtrlDistRatioEndI.Add(IDC_TM_ELNK_DIST_ENDI_EDT);

	m_aCtrlDisable.Append(m_aCtrlCopyLoad);
	m_aCtrlDisable.Append(m_aCtrlMultiLinear);

	//m_pBitmap = 0;
	m_bSelectByLabelMode = FALSE;
	m_CurElnkKeyBySelectLabel = 0;
}

CBndrElnkDlg::~CBndrElnkDlg()
{  
	//if (m_pBitmap) delete m_pBitmap;
}

//
// 기존 구현에 Modify 모드가 없으므로 , Select by Label로 Apply할 때는 
// 현재 선택된 Elastic Link를 삭제하고 현재 설정으로 Add한다. 
// 
void CBndrElnkDlg::Execute()
{
	BOOL bSelectByLabelMode = m_bSelectByLabelMode;
	unsigned int CurElnkKeyBySelectLabel = m_CurElnkKeyBySelectLabel;
	// ---------------------------------------------------------------------------
	// Apply를 누르면 SelectBy Label 모드를 해제한다. 
	// 다시 Label을 선택해야 활성화 된다.
	// 모호한 상태가 있는데 , 이건 마일스톤 끝나고 기술팀과 협의해서 정리를 해야 할 것 같다. 
	// ---------------------------------------------------------------------------
	m_bSelectByLabelMode = FALSE;
	m_CurElnkKeyBySelectLabel = 0;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	T_ELNK_K key;
	T_ELNK_D data;
	data.Initialize();
	CArray<int, int> aNodes;

	BOOL bCopyLoad;
	double ux = 0.0, uy = 0.0, uz = 0.0;
	double wx = 0.0, wy = 0.0, wz = 0.0;
	CArray<double, double> aDistance;

	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);
	if (nOption == 0 || bSelectByLabelMode ) // Add
	{
		key = m_pDoc->m_pAttrCtrl->GetStartNumElnk();

		CString csNodeList;
		m_wnd2Nodes.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aNodes) || aNodes.GetSize() != 2)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Select_2_nodes_));
			return;
		}
		data.Node1 = aNodes[0]; data.Node2 = aNodes[1];

		data.nSubOption = m_wndELNKTypeCmb.GetItemData(m_wndELNKTypeCmb.GetCurSel());

		if (data.nSubOption == 0) // General Type
		{
			for (int i = 0; i < 6; i++) data.Spring[i] = m_wndEdit[i].GetEditValue();
			for (int i = 0; i < 6; i++) data.bFixed[i] = m_bFixed[i];
		}
		else if (data.nSubOption == 1) // Rigid Link Type
		{
			// Nothing
		}
		else if (data.nSubOption == 2) // Tension Only Type
		{
			for (int i = 0; i < 1; i++) data.Spring[i] = m_wndEdit[i].GetEditValue();
		}
		else if (data.nSubOption == 3) // Compression Only Type
		{
			for (int i = 0; i < 1; i++) data.Spring[i] = m_wndEdit[i].GetEditValue();
		}
		else if (data.nSubOption == 4 || data.nSubOption == 6) // Multi-Linear Type
		{
			// 기존에 쓰던 Data들은 기본값으로 채워 넣음
			data.nSymmetric = FALSE;
			data.nGridNum = 0;
			for (int i = 0; i < 10; i++)
			{
				data.dDistance[i] = 0.0;
				data.dForce[i] = 0.0;
				data.dRadian[i] = 0.0;
				data.dMoment[i] = 0.0;
			}

			data.nDirection = m_wndDirectionCmb.GetItemData(m_wndDirectionCmb.GetCurSel());

			if (data.nSubOption == 4)
				data.MlfcK = CDlgUtil::CobxGetCurSelItemData(m_wndMlfccmbx, m_wndMlfccmbx.GetCurSel());
			else if (data.nSubOption == 6)
				data.RlfcK = CDlgUtil::CobxGetCurSelItemData(m_wndMlfccmbx, m_wndMlfccmbx.GetCurSel());
		}
		else if (data.nSubOption == 5) // Saddle Type
		{
			for (int i = 0; i < 6; i++) data.bFixed[i] = m_bFixed[i];
		}
		else if (data.nSubOption == 7) // Saddle Type
		{
			AddLdsb();
			return;
		}

		data.bShearSprLoc = m_chkShearSpr.GetCheck();
		if (data.bShearSprLoc)
		{
			if (data.nSubOption == 4 || data.nSubOption == 6)
			{
				data.dDistRatioEndI = m_wndDistRatioEndI.GetEditValue();
			}
			else
			{
				data.dDistRatioSDy = m_editDistRatioSDy.GetEditValue();
				data.dDistRatioSDz = m_editDistRatioSDz.GetEditValue();
			}
		}

		if (!CFormulaEdit::GetEditValue(&m_wndBetaAngle, data.dBetaAngle))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_angle_));
			return;
		}

		bCopyLoad = m_wndCopyLoad.GetCheck();
		if (bCopyLoad)
		{
			int nAxis = 0;
			CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAxis, nAxis);
			switch (nAxis)
			{
			case 0: ux = 1.0; break;
			case 1: uy = 1.0; break;
			case 2: uz = 1.0; break;
			}
			pIGM->GetWcsVector(ux, uy, uz, wx, wy, wz);

			CString csDistance;
			m_wndDistEdit.GetWindowText(csDistance);
			if (!GetUEDistance(csDistance, aDistance) ||
				aDistance.GetSize() <= 0)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_value_));
				return;
			}
		}
	}
	else // Delete
	{
		pIGM->GetSelectedNodeKeyList(aSelKey);
		if (aSelKey.GetSize() == 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_nodes_));
			return;
		}
	}

	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	data.GroupKey = BngrK;

	int nCopyType = 0;
	int nCopyNum = 0;
	int nNodeInc = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCopyType, nCopyType);
	if (nCopyType == 0) // node inc
	{
		CFormulaEdit::GetEditValue(&m_editNtimes, nCopyNum);
		CFormulaEdit::GetEditValue(&m_editNodeInc, nNodeInc);
	}

	BOOL bSuccess = FALSE;
	if (nOption == 0 || bSelectByLabelMode) // Add or
	{
		if (bSelectByLabelMode)
		{
			unsigned int CurElnkKey = CurElnkKeyBySelectLabel;
			bSuccess = m_pDoc->m_pDataCtrl->DelElnk(CurElnkKey);
		}

		bSuccess = m_pDoc->m_pDataCtrl->AddElnk(key, data, bCopyLoad, nCopyType, nCopyNum, nNodeInc, wx, wy, wz, aDistance);
	}
	else if (nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelElnk1(aSelKey, BngrK);

	if (bSuccess)	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CBndrElnkDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrElnkDlg)
	DDX_Control(pDX, IDC_TM_SHEAR_CHECK, m_chkShearSpr);
	DDX_Control(pDX, IDC_TM_DR_SDY_EDIT, m_editDistRatioSDy);
	DDX_Control(pDX, IDC_TM_DR_SDZ_EDIT, m_editDistRatioSDz);
	DDX_Control(pDX, IDC_TM_DIST_UNIT, m_wndDistUnit);
	DDX_Control(pDX, IDC_TM_COPY_LOAD, m_wndCopyLoad);
	DDX_Control(pDX, IDC_TM_DIST_EDIT, m_wndDistEdit);
	DDX_Control(pDX, IDC_TM_BETA_ANGLE, m_wndBetaAngle);
	DDX_Control(pDX, IDC_TM_2NODES_EDIT, m_wnd2Nodes);
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_TM_EDIT_NTIMES, m_editNtimes);
	DDX_Control(pDX, IDC_TM_EDIT_NODEINC, m_editNodeInc);

	DDX_Control(pDX, IDC_TM_ELNK_TYPE_CMB, m_wndELNKTypeCmb);
	DDX_Control(pDX, IDC_TM_ELNK_DIR_CMB,  m_wndDirectionCmb);
		
	DDX_Control(pDX, IDC_TM_ELNK_DIST_ENDI_EDT, m_wndDistRatioEndI);

	DDX_Control(pDX, IDC_TM_ELNK_MLFC_COMBO, m_wndMlfccmbx);
	DDX_Control(pDX, IDC_TM_ELNK_TYPE_BMP, m_wndPicture);

	//}}AFX_DATA_MAP
	UINT aID[5][6] = { 
		{ IDC_TM_SDX_EDIT, IDC_TM_SDY_EDIT, IDC_TM_SDZ_EDIT,
			IDC_TM_SRX_EDIT, IDC_TM_SRY_EDIT, IDC_TM_SRZ_EDIT, },
		{ IDC_TM_SDX_UNIT, IDC_TM_SDY_UNIT, IDC_TM_SDZ_UNIT, 
			IDC_TM_SRX_UNIT, IDC_TM_SRY_UNIT, IDC_TM_SRZ_UNIT, },
		{ IDC_TM_ELNK_SDX_CHK, IDC_TM_ELNK_SDY_CHK, IDC_TM_ELNK_SDZ_CHK, 
			IDC_TM_ELNK_SRX_CHK, IDC_TM_ELNK_SRY_CHK, IDC_TM_ELNK_SRZ_CHK, },
		{ IDC_TM_SDX_EDIT2, IDC_TM_SDY_EDIT2, IDC_TM_SDZ_EDIT2,
			IDC_TM_SRX_EDIT2, IDC_TM_SRY_EDIT2, IDC_TM_SRZ_EDIT2, },
		{ IDC_TM_SDX_UNIT2, IDC_TM_SDY_UNIT2, IDC_TM_SDZ_UNIT2,
			IDC_TM_SRX_UNIT2, IDC_TM_SRY_UNIT2, IDC_TM_SRZ_UNIT2, },
	};

	for (int i = 0; i < 6; i++)
	{
		DDX_Control(pDX, aID[0][i], m_wndEdit[i]);
		DDX_Control(pDX, aID[1][i], m_wndUnit[i]);
		DDX_Check(pDX, aID[2][i], m_bFixed[i]);
		DDX_Control(pDX, aID[3][i], m_wndDispEdit[i]);
	}
	for(int i = 0;i < 3;i++)
		DDX_Control(pDX, aID[4][i], m_wndDispUnit[i]);
}

void CBndrElnkDlg::AlignControlDynamic()
{
	CRect rRef;
	CRect rToMove;
	CRect rcContainer;  
	int nDistY;

	int nType = m_wndELNKTypeCmb.GetItemData(m_wndELNKTypeCmb.GetCurSel());
		
	if(nType == 4 || nType == 6)// Multi Linear
	{
		GetDlgItem(IDC_TM_ELNK_MLFC_COMBO)->GetWindowRect(rRef); 
	}
	else
	{
		GetDlgItem(IDC_TM_ELNK_FIXED_GRP)->GetWindowRect(rRef);
	}  
	GetDlgItem(IDC_TM_SHEAR_CHECK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlShearSpringLoc, nDistY);

	GetDlgItem(IDC_TM_SHEAR_CHECK_GROUP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_BETA_ANGLE)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlBetaAngle, nDistY);
	
	GetDlgItem(IDC_TM_BETA_ANGLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_WG_TREEMENU_STATIC1)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.bottom + globalUtils.ScaleByDPI(10);  
	GetDlgItem(IDC_WG_TREEMENU_STATIC1)->GetWindowRect(rcContainer);
	rcContainer.bottom = rcContainer.bottom + nDistY;
	ScreenToClient(rcContainer);
	GetDlgItem(IDC_WG_TREEMENU_STATIC1)->MoveWindow(rcContainer);

	GetDlgItem(IDC_WG_TREEMENU_STATIC1)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_2NODES_EDIT)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlSelNodes, nDistY);

	GetDlgItem(IDC_TM_2NODES_EDIT)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_COPY_LOAD)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCopyElstLink, nDistY);

	GetDlgItem(IDC_TM_AXIS_TITLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_TEXT_NTIMES)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCopyNodeInc, nDistY);

	GetDlgItem(IDC_WG_TREEMENU_STATIC3)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_EXECUTE)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlApplyClose, nDistY);
}

void CBndrElnkDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// Multi Linear
	GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_ELNK_DIR_CMB)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMultiLinear, nDistY + globalUtils.ScaleByDPI(1));

	// Distance Ratio From End I
	GetDlgItem(IDC_TM_DR_DIST_ENDI_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_ELNK_DIST_ENDI_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlDistRatioEndI, nDistY+globalUtils.ScaleByDPI(10));

	// Copy Elastic Link Move
	GetDlgItem(IDC_TM_AXIS_TITLE)->GetWindowRect(rRef);
	GetDlgItem(m_aCtrlCopyNodeInc[0])->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCopyNodeInc, nDistY);


	// Limited-displacement-sliding-bearing Position
	GetDlgItem(IDC_TM_SDX_EDIT2)->GetWindowRect(rRef);
	GetDlgItem(IDC_TM_SDX_EDIT)->GetWindowRect(rToMove);
	int nDistX = rToMove.left - rRef.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlLiDisp, nDistX);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_CLOSE);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(5);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

void CBndrElnkDlg::InitComboBoxData()
{
	// Elastic Link Data Type
	m_wndELNKTypeCmb.ResetContent();
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_GENERAL))         , 0);
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_RIGID))           , 1);
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_TENSION_ONLY))    , 2);
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_COMPRESSION_ONLY)), 3);
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_MULTI_LINEAR))    , 4);

 #if defined (_CIVIL)
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_SADDLE)), 5);
 #endif

//Rail_Track_Analysis 와 같은 조건
 #if defined(_CIVIL_ORG) || defined(_CIVIL_US) || defined(_CIVIL_RUS) || defined(_CIVIL_CH)
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_RLFC)), 6);
 #endif

#if defined(_MGEN_CH)
	m_wndELNKTypeCmb.SetItemData(m_wndELNKTypeCmb.AddString(_LS(IDS_TM_ELNK_TYPE_LDSB)), 7);
#endif

	m_wndELNKTypeCmb.SetCurSel(0);
	
	InitComboBoxDataDirection();

	RestoreMlfcCbx();
}

void CBndrElnkDlg::InitComboBoxDataDirection()
{
	int nTypeIndex = m_wndELNKTypeCmb.GetItemData(m_wndELNKTypeCmb.GetCurSel());

	if(nTypeIndex == 6)
	{
		m_wndDirectionCmb.ResetContent();
		//m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_DX)), 0);
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ENLK_DY)), 1);
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_DZ)), 2);
		//m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_RX)), 3);
		//m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_RY)), 4);
		//m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ENLK_RZ)), 5);

		m_wndDirectionCmb.SetCurSel(0);
	}
	else
	{
		// Direction
		m_wndDirectionCmb.ResetContent();
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_DX)), 0);
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ENLK_DY)), 1);
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_DZ)), 2);
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_RX)), 3);
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ELNK_RY)), 4);
		m_wndDirectionCmb.SetItemData(m_wndDirectionCmb.AddString(_LS(IDS_TM_ENLK_RZ)), 5);
		m_wndDirectionCmb.SetCurSel(0);
	}
}

BEGIN_MESSAGE_MAP(CBndrElnkDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrElnkDlg)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_COPY_LOAD, OnTmCopyLoad)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_RADIO_NODE_INC, OnTmCopyType)
	ON_BN_CLICKED(IDC_TM_RADIO_DISTANCE, OnTmCopyType)
	ON_BN_CLICKED(IDC_TM_SHEAR_CHECK, OnTmShearCheck)
	ON_BN_CLICKED(IDC_TM_ELNK_MLFC_BUTTON, OnMlfcButton)  
	ON_CBN_SELCHANGE(IDC_TM_ELNK_TYPE_CMB, OnTmElasticLinkDataTypeChange)
	ON_BN_CLICKED(IDC_TM_ELNK_SDX_CHK, OnTmElnkFixedChk)
	ON_BN_CLICKED(IDC_TM_ELNK_SDY_CHK, OnTmElnkFixedChk)
	ON_BN_CLICKED(IDC_TM_ELNK_SDZ_CHK, OnTmElnkFixedChk)
	ON_BN_CLICKED(IDC_TM_ELNK_SRX_CHK, OnTmElnkFixedChk)
	ON_BN_CLICKED(IDC_TM_ELNK_SRY_CHK, OnTmElnkFixedChk)
	ON_BN_CLICKED(IDC_TM_ELNK_SRZ_CHK, OnTmElnkFixedChk)
	ON_CBN_SELCHANGE(IDC_TM_ELNK_DIR_CMB,  OnTmDirectionChange)
	ON_BN_CLICKED(IDC_TM_START_LINK_NUM_BTN, OnViewStartLinkNum)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrElnkDlg message handlers

BOOL CBndrElnkDlg::OnInitDialog()  
{
	m_pDoc = CDBDoc::GetDocPoint();

	CMenuBarChildDlg::OnInitDialog();

	AlignControl();

	InitComboBoxData();

	OnTmElasticLinkDataTypeChange();
	
	OnTmDirectionChange();

	//OnTmSymNumChange();

	// 사용되지 않는 컨트롤
	GetDlgItem(IDC_TM_START_LINK_NUM_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_START_LINK_NUM_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_START_LINK_NUM_BTN)->ShowWindow(SW_HIDE);

	// TODO: Add extra initialization here
	for (int i = 0; i < 6; i++)
	{
		m_wndEdit[i].SetUnitType(CUnitCtrl::m_ELNK_UNIT.Spring[i]);
		m_wndUnit[i].SetUnitType(CUnitCtrl::m_ELNK_UNIT.Spring[i]);
		m_wndDispEdit[i].SetUnitType(CUnitCtrl::m_ELNK_UNIT.dDistance);
		//m_wndDispUnit[i].SetUnitType(CUnitCtrl::m_ELNK_UNIT.dDistance);
	}
	for(int i = 0;i < 3;i++)
	{
		m_wndDispUnit[i].SetUnitType(CUnitCtrl::m_ELNK_UNIT.dDistance);
		m_wndDispUnit[i+3].SetUnitType(D_UNITSYS_NONE);
		//m_wndDispUnit[i + 3].SetWindowText(_LSX([rad]));
	}

	m_wndDistUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDistRatioSDy.SetUnitType(D_UNITSYS_NONE);
	m_editDistRatioSDz.SetUnitType(D_UNITSYS_NONE);
	m_editDistRatioSDy.SetEditUnit(0.5);
	m_editDistRatioSDz.SetEditUnit(0.5);
	m_wndDistRatioEndI.SetUnitType(D_UNITSYS_NONE);
	m_wndDistRatioEndI.SetEditUnit(0.5);
	
	m_wnd2Nodes.SetAttNodeList();
	m_wnd2Nodes.SetLButtonDownNotifyWindow(this);
	m_wnd2Nodes.SetEnterNotifyWindow(this);
	m_wnd2Nodes.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wnd2Nodes.SetMaxNodeKeyNum(2);

	m_wndBetaAngle.ResetContent();
	m_wndBetaAngle.AddString(_T("0"));
	m_wndBetaAngle.AddString(_LS(IDS_WG_TREEMENU_90));
	m_wndBetaAngle.AddString(_LS(IDS_WG_TREEMENU_180));
	m_wndBetaAngle.SetCurSel(0);

	int nOpt = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, nOpt);
	CtrlEnableDisable(m_aCtrlDisable, nOpt==0);
	int nAxis = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlAxis, nAxis);

	BOOL bCopyLoad = m_wndCopyLoad.GetCheck();
	CtrlEnableDisable(m_aCtrlCopyLoad, bCopyLoad);
	OnTmShearCheck();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCopyType, 1);
	OnTmCopyType();
	m_editNtimes.SetRange(1, SHRT_MAX);
	m_editNtimes.SetValue(1);
	m_editNtimes.SetInteger(TRUE);
	m_editNodeInc.SetRange(1, SHRT_MAX);
	m_editNodeInc.SetValue(1);
	m_editNodeInc.SetInteger(TRUE);

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(FALSE);

	m_bSelectByLabelMode = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrElnkDlg::SortMultiLinearValue(double dDistance[10], double dForce[10], int nCount)
{
	ASSERT( nCount >=0 && nCount <= 10 );
	for( int i=0; i<nCount; i++ )
	{
		for( int k=0; k<nCount-1; k++ )
		{
			if( dDistance[k] > dDistance[k+1] )
			{
				double dDistTmp   = dDistance[k];
				double dForceTmp  = dForce[k];

				dDistance[k]      = dDistance[k+1];
				dDistance[k+1]    = dDistTmp;

				dForce[k]         = dForce[k+1];
				dForce[k+1]       = dForceTmp;
			}
		}
	}
}

void CBndrElnkDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nCheck);
	CtrlEnableDisable(m_aCtrlDisable, nCheck == 0);
	if (nCheck == 0)  // add
	{
		BOOL bCopyLoad = m_wndCopyLoad.GetCheck();
		CtrlEnableDisable(m_aCtrlCopyLoad, bCopyLoad);
		OnTmElasticLinkDataTypeChange();
		// PMS:XXXX-THBAE-20110524 : 아래 세 함수 OnTmElasticLinkDataTypeChange() 내에서 Type에 따라 처리.
		// 각 Type에서 "Shear Spring Location" 입력부분 활성화 동작 이상 문제.
		//OnTmDirectionChange();  
		//OnTmShearCheck();
	}

	if(nCheck == 2) //Delete
	{
		this->m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		this->m_wndGroupCombo.ShowHideAll(FALSE);
	}
}

void CBndrElnkDlg::OnTmCopyLoad() 
{
	// TODO: Add your control notification handler code here
	BOOL bCopyLoad = m_wndCopyLoad.GetCheck();
	CtrlEnableDisable(m_aCtrlCopyLoad, bCopyLoad);		
}

LRESULT CBndrElnkDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();

	if(m_wnd2Nodes.GetSafeHwnd() == (HWND)lParam)
	{
		CArray<int, int> aKeyNode;
		CString csNodeList;
		m_wnd2Nodes.GetWindowText(csNodeList);
		if (!GetNodeList(csNodeList, aKeyNode) || aKeyNode.GetSize() != 2)
			return 0L;
		Execute();
		m_wnd2Nodes.ClearContents();
	}
	return 0L;  
}

LRESULT CBndrElnkDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_wnd2Nodes.GetSafeHwnd() == (HWND)lParam)
	{
		Execute();
	}
	return 0L;
}

void CBndrElnkDlg::OnTmCopyType()
{
	int nCopyType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCopyType, nCopyType);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCopyNodeInc, nCopyType == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlCopyDist, nCopyType == 1);
}

void CBndrElnkDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);
}

void CBndrElnkDlg::OnMlfcButton()
{
	int nTypeIndex = m_wndELNKTypeCmb.GetItemData(m_wndELNKTypeCmb.GetCurSel());
	if (nTypeIndex == 6)
	{
		CRlfcListDlg Dlg;
		Dlg.DoModal();
	}
	else 
	{
		CMlfcListDlg Dlg;
		Dlg.DoModal();
	}
}

void CBndrElnkDlg::OnTmShearCheck()
{
	UpdateData(TRUE);

	int nOpt = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOpt);
	int nCheck = m_chkShearSpr.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDistRatio,     nOpt==0 && nCheck!=0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDistRatioEndI, nOpt==0 && nCheck!=0);
}

void CBndrElnkDlg::OnTmElasticLinkDataTypeChange()
{
	UpdateData(TRUE);

	InitComboBoxDataDirection();

	int nType = m_wndELNKTypeCmb.GetItemData(m_wndELNKTypeCmb.GetCurSel());
	CtrlShowHide(m_aCtrlLiDisp, FALSE);

	if(nType == 0) // General Type
	{
		// Control Show Hide
		CtrlShowHide(m_aCtrlMultiLinear,   FALSE);
		CtrlShowHide(m_aCtrlDistRatioEndI, FALSE);
		CtrlShowHide(m_aCtrlSpring,        TRUE);
		CtrlShowHide(m_aCtrlDistRatio,     TRUE);
		GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->ShowWindow(TRUE);

		// Control Enable Disable
		m_chkShearSpr.EnableWindow(TRUE);
		CtrlEnableDisable(m_aCtrlDistRatio, TRUE);
		CtrlEnableDisable(m_aCtrlSpring,    TRUE);

		OnTmShearCheck();

		OnTmElnkFixedChk();
	}
	else if(nType == 1) // Rigid Link Type
	{
		// Control Show Hide
		CtrlShowHide(m_aCtrlMultiLinear,   FALSE);
		CtrlShowHide(m_aCtrlDistRatioEndI, FALSE);
		CtrlShowHide(m_aCtrlSpring,        TRUE);
		CtrlShowHide(m_aCtrlDistRatio,     TRUE);
		GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->ShowWindow(TRUE);

		// Control Enable Disable
		m_chkShearSpr.EnableWindow(FALSE);
		CtrlEnableDisable(m_aCtrlDistRatio, FALSE);
		CtrlEnableDisable(m_aCtrlSpring,    FALSE);
	}
	else if(nType == 2) // Tension Only Type
	{
		// Control Show Hide
		CtrlShowHide(m_aCtrlMultiLinear,   FALSE);
		CtrlShowHide(m_aCtrlDistRatioEndI, FALSE);
		CtrlShowHide(m_aCtrlSpring,        TRUE);
		CtrlShowHide(m_aCtrlDistRatio,     TRUE);
		GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->ShowWindow(TRUE);

		// Control Enable Disable
		m_chkShearSpr.EnableWindow(FALSE);
		CtrlEnableDisable(m_aCtrlDistRatio, FALSE);
		CtrlEnableDisable(m_aCtrlSpring,    FALSE);

		// SDx Enable
		GetDlgItem(IDC_TM_SDX_STC )->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_SDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_SDX_UNIT)->EnableWindow(TRUE);
	}
	else if(nType == 3) // Compression Only Type
	{
		// Control Show Hide
		CtrlShowHide(m_aCtrlMultiLinear,   FALSE);
		CtrlShowHide(m_aCtrlDistRatioEndI, FALSE);
		CtrlShowHide(m_aCtrlSpring,        TRUE);
		CtrlShowHide(m_aCtrlDistRatio,     TRUE);
		GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->ShowWindow(TRUE);

		// Control Enable Disable
		m_chkShearSpr.EnableWindow(FALSE);
		CtrlEnableDisable(m_aCtrlDistRatio, FALSE);
		CtrlEnableDisable(m_aCtrlSpring,    FALSE);

		// SDx Enable
		GetDlgItem(IDC_TM_SDX_STC )->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_SDX_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_SDX_UNIT)->EnableWindow(TRUE);
	}
	else if(nType == 4 || nType == 6) // Multi-Linear Type
	{
		// Control Show Hide
		CtrlShowHide(m_aCtrlSpring,        FALSE);
		CtrlShowHide(m_aCtrlDistRatio,     FALSE);
		CtrlShowHide(m_aCtrlMultiLinear,   TRUE);
		CtrlShowHide(m_aCtrlDistRatioEndI, TRUE);
		GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->ShowWindow(FALSE);
		
		// Control Enable Disable
		m_chkShearSpr.EnableWindow(TRUE);
		CtrlEnableDisable(m_aCtrlMultiLinear,   TRUE);
		CtrlEnableDisable(m_aCtrlDistRatioEndI, TRUE);

		if(nType == 4)
			GetDlgItem(IDC_TM_ELNK_MLFC_STC)->SetWindowText(_LS(IDS_TB_ELNK_MLFC));
		else if(nType == 6)
			GetDlgItem(IDC_TM_ELNK_MLFC_STC)->SetWindowText(_LS(IDS_TB_ELNK_RLFC));
		else ASSERT(0);

		OnTmDirectionChange();
		OnTmShearCheck();

		//OnTmSymNumChange();
	}
	else if(nType == 5) // Saddle Type
	{
		// Control Show Hide
		CtrlShowHide(m_aCtrlMultiLinear,   FALSE);
		CtrlShowHide(m_aCtrlDistRatioEndI, FALSE);
		CtrlShowHide(m_aCtrlSpring,        TRUE);
		CtrlShowHide(m_aCtrlDistRatio,     TRUE);
		GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->ShowWindow(TRUE);

		// Control Enable Disable
		m_chkShearSpr.EnableWindow(FALSE);
		CtrlEnableDisable(m_aCtrlDistRatio, FALSE);
		CtrlEnableDisable(m_aCtrlSpring,    FALSE);

		for(int i=0; i< 6; i++)
		{
			m_bFixed[i] = TRUE;
		}
	}
	else if (nType == 7)
	{
		for (int i = 0; i < 3; i++)
			m_bFixed[i + 3] = TRUE;
		UpdateData(FALSE);
		// Control Show Hide
		CtrlShowHide(m_aCtrlMultiLinear, FALSE);
		CtrlShowHide(m_aCtrlDistRatioEndI, FALSE);
		CtrlShowHide(m_aCtrlSpring, TRUE);
		CtrlShowHide(m_aCtrlDistRatio, TRUE);
		GetDlgItem(IDC_TM_ELNK_TYPE_BMP)->ShowWindow(TRUE);

		CtrlShowHide(m_aCtrlNoLimited, FALSE);
		CtrlShowHide(m_aCtrlLiDisp, TRUE);
		// Control Enable Disable
		m_chkShearSpr.EnableWindow(FALSE);
		CtrlEnableDisable(m_aCtrlDistRatio, TRUE);
		CtrlEnableDisable(m_aCtrlSpring, TRUE);

		OnTmShearCheck();

		OnTmElnkFixedChk();
	}
	else // Error
	{
		ASSERT(0); 
	}

	UpdateData(FALSE);

	ChangeBitMap();
	//OnTmShearCheck();
	AlignControlDynamic();
}

void CBndrElnkDlg::OnTmElnkFixedChk()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_TM_SDX_EDIT)->EnableWindow(!m_bFixed[0]);
	GetDlgItem(IDC_TM_SDY_EDIT)->EnableWindow(!m_bFixed[1]);
	GetDlgItem(IDC_TM_SDZ_EDIT)->EnableWindow(!m_bFixed[2]);
	GetDlgItem(IDC_TM_SRX_EDIT)->EnableWindow(!m_bFixed[3]);
	GetDlgItem(IDC_TM_SRY_EDIT)->EnableWindow(!m_bFixed[4]);
	GetDlgItem(IDC_TM_SRZ_EDIT)->EnableWindow(!m_bFixed[5]);

	GetDlgItem(IDC_TM_SDX_EDIT2)->EnableWindow(!m_bFixed[0]);
	GetDlgItem(IDC_TM_SDY_EDIT2)->EnableWindow(!m_bFixed[1]);
	GetDlgItem(IDC_TM_SDZ_EDIT2)->EnableWindow(!m_bFixed[2]);
	GetDlgItem(IDC_TM_SRX_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_TM_SRY_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_TM_SRZ_EDIT2)->EnableWindow(FALSE);
}

void CBndrElnkDlg::OnTmDirectionChange()
{
	UpdateData(TRUE);

	int nIndex = m_wndDirectionCmb.GetItemData(m_wndDirectionCmb.GetCurSel());

	if(nIndex == 0 || nIndex == 3) // Dx, Rx
	{
		GetDlgItem(IDC_TM_SHEAR_CHECK)->EnableWindow(FALSE);
		CtrlEnableDisable(m_aCtrlDistRatioEndI, FALSE);
		m_chkShearSpr.SetCheck(FALSE);
	}
	else if(nIndex == 1 || nIndex == 2 || nIndex == 4 || nIndex == 5 || nIndex == 6) // Dy, Dz, Ry, Rz
	{
		GetDlgItem(IDC_TM_SHEAR_CHECK)->EnableWindow(TRUE);
		CtrlEnableDisable(m_aCtrlDistRatioEndI, TRUE);
	}
	else
	{
		ASSERT(0);
	}

	ChangeBitMap();
	OnTmShearCheck();
	RestoreMlfcCbx();
}

void CBndrElnkDlg::ChangeBitMap()
{
	UpdateData(TRUE);
	//여기에 작성
	CString aBitmapID[] = {
		_T("SVG\\Illustration\\Dialog\\tm_elnk 1.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nspr_sym.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nspr_unsym.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nspr_sym_r.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_nspr_unsym_r.svg")
	};

	CString sPath;

	int nBitmap = 0;
	int nTypeIndex = m_wndELNKTypeCmb.GetItemData(m_wndELNKTypeCmb.GetCurSel());
	int nDirection = m_wndDirectionCmb.GetItemData(m_wndDirectionCmb.GetCurSel());

	if(nTypeIndex == 0 || nTypeIndex == 1 || nTypeIndex == 2 || nTypeIndex == 3) // General Type, Rigid Link Type, Tension Only Type, Compression Only Type
	{
		nBitmap = 0;
	}
	else if(nTypeIndex == 4 || nTypeIndex == 6) // Multi-Linear Type
	{
		// 이미지를 Mlfc 대화상자에서 보여주도록 수정함
		//if(nSymIndex == 0) // Symmetric
		//{
		//  if      (nDirection == 0 || nDirection == 1 || nDirection == 2) nBitmap = 1;
		//  else if (nDirection == 3 || nDirection == 4 || nDirection == 5) nBitmap = 3;
		//}
		//else if(nSymIndex == 1) // UnSymmetric
		//{
		//  if      (nDirection == 0 || nDirection == 1 || nDirection == 2) nBitmap = 2;
		//  else if (nDirection == 3 || nDirection == 4 || nDirection == 5) nBitmap = 4;
		//}
		//else
		//{
			//ASSERT(0);
		//}
	}
	else if(nTypeIndex == 5) // Saddle Type
	{

	}
	else if (nTypeIndex == 7)
	{

	} 
	else 
	{
		ASSERT(0);
	}

	sPath = aBitmapID[nBitmap];
	m_wndPicture.SetImage(sPath);
}

void CBndrElnkDlg::RestoreMlfcCbx()
{
	int nTypeIndex = m_wndELNKTypeCmb.GetItemData(m_wndELNKTypeCmb.GetCurSel());
	if (nTypeIndex == 6)
	{
		RestoreRlfcData();
	}
	else 
	{
		RestoreMlfcData();
	}
}

void CBndrElnkDlg::RestoreMlfcData()
{
	T_MLFC_K MlfcKLast;
	MlfcKLast = CDlgUtil::CobxGetCurSelItemData(m_wndMlfccmbx, m_wndMlfccmbx.GetCurSel());

	m_wndMlfccmbx.ResetContent();

	int nDir = m_wndDirectionCmb.GetItemData(m_wndDirectionCmb.GetCurSel());

	CArray<T_MLFC_K, T_MLFC_K> aMlfcK;
	m_pDoc->m_pAttrCtrl->GetMlfcKeyList(aMlfcK);
	for (int i = 0; i < aMlfcK.GetSize(); i++)
	{
		T_MLFC_D MlfcD;
		MlfcD.Initialize();
		m_pDoc->m_pAttrCtrl->GetMlfc(aMlfcK[i], MlfcD);

		BOOL bAddCBxItem = FALSE;
		if (nDir == 0 || nDir == 1 || nDir == 2)
		{
			if (MlfcD.nFuncType == 0)
			{
				bAddCBxItem = TRUE;
			}
		}
		else
		{
			if (MlfcD.nFuncType == 1)
			{
				bAddCBxItem = TRUE;
			}
		}

		if (bAddCBxItem)
		{
			CDlgUtil::CobxAddItem(m_wndMlfccmbx, MlfcD.FuncName, aMlfcK[i]);
		}
	}
	if (CDlgUtil::CobxSetCurSelItemData(m_wndMlfccmbx, MlfcKLast) == CB_ERR)
	{
		m_wndMlfccmbx.SetCurSel(0);
	}
}

void CBndrElnkDlg::RestoreRlfcData()
{
	T_RLFC_K RlfcKLast;
	RlfcKLast = CDlgUtil::CobxGetCurSelItemData(m_wndMlfccmbx, m_wndMlfccmbx.GetCurSel());

	m_wndMlfccmbx.ResetContent();

	int nDir = m_wndDirectionCmb.GetItemData(m_wndDirectionCmb.GetCurSel());

	CArray<T_RLFC_K, T_RLFC_K> aRlfcK;
	m_pDoc->m_pAttrCtrl->GetRlfcKeyList(aRlfcK);
	for (int i = 0; i < aRlfcK.GetSize(); i++)
	{
		T_RLFC_D RlfcD;
		RlfcD.Initialize();
		m_pDoc->m_pAttrCtrl->GetRlfc(aRlfcK[i], RlfcD);

		CDlgUtil::CobxAddItem(m_wndMlfccmbx, RlfcD.FuncName, aRlfcK[i]);
	}

	if (CDlgUtil::CobxSetCurSelItemData(m_wndMlfccmbx, RlfcKLast) == CB_ERR)
	{
		m_wndMlfccmbx.SetCurSel(0);
	}
}

void CBndrElnkDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break; 
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CBndrElnkDlg::UpdateBuffer()
{

	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount ==0 ) return;

	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdmtCommand = FALSE;
	BOOL bIsTmatCommand = FALSE;
	BOOL bIsMatlCommand = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_MLFC_ADD):
		case(UR_MLFC_DEL):
		case(UR_MLFC_MFD):
		case(UR_MLFC_MFS):
			RestoreMlfcCbx();
			break;
		case(UR_RLFC_ADD):
		case(UR_RLFC_DEL):
		case(UR_RLFC_MFD):
		case(UR_RLFC_MFS):
			RestoreMlfcCbx();
			break;
		default:
			break;
		}
	}
}

void CBndrElnkDlg::OnViewStartLinkNum() 
{
	
}

void CBndrElnkDlg::AddLdsb()
{
	T_ELNK_K ElnkK;
	T_ELNK_D ElnkD;
	T_BNGR_K BngrK;
	ElnkD.Initialize();
	CArray<int, int> aNodes;
	CString csNodeList;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	m_wnd2Nodes.GetWindowText(csNodeList);
	GetNodeList(csNodeList, aNodes);
	ElnkD.Node1 = aNodes[0]; ElnkD.Node2 = aNodes[1];

	BOOL bNeedAddGeneral = FALSE;
	BOOL bEditError = FALSE;
	BOOL bTranslate = FALSE;


	double disp[6] = { 0.0 };
	for (int i = 0; i < 6; i++)
	{
		disp[i] = m_wndDispEdit[i].GetEditValue();
		disp[i] *= m_pDoc->m_pUnitCtrl->GetConvertFactorCurrent(D_UNITSYS_BASE_LENGTH);
		disp[i] /= m_pDoc->m_pUnitCtrl->GetConvertFactorTgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM,D_UNITSYS_BASE_LENGTH);
		ElnkD.bFixed[i] = m_bFixed[i];
		if (m_bFixed[i])
			bNeedAddGeneral = TRUE;
		if (disp[i] < 0 || disp[i] > 1000)
			bEditError = TRUE;
	}
	CAutoRecoveryUnit unitSet(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N, m_pDoc->m_pUnitCtrl, FALSE);
	if (bEditError)
	{
		GSaveHistoryFormatNF(_LS(IDS_DB_ERR_MSG_560));
		return;
	}
	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Elastic_Link)), CMDTYPE_REMOVE_ANALYSIS))
		return;
	if (bNeedAddGeneral)
	{
		ElnkK = m_pDoc->m_pAttrCtrl->GetStartNumElnk();
		ElnkD.GroupKey = BngrK;
		ElnkD.nSubOption = 0;
		if (!AddLdsb_Elnk(ElnkK, ElnkD))
		{
			m_pDoc->m_pUndoCtrl->CancelEditDB();
			return;
		}
		bTranslate = TRUE;
	}
	/*Convert MultiLinear Data*/
	T_MLFC_K mlfcK;
	T_MLFC_D mlfcD;
	mlfcD.Initialize();
	mlfcD.bSymmetric = TRUE;
	mlfcD.arFuncData.SetSize(3);
	
	CString strFixedName = _T("MElnk_func");
	for (int i = 0; i < 3; i++)
	{
		if (!m_bFixed[i] && disp[i] > 0)
		{
			/*Init mlfc.arFuncData*/
			mlfcD.arFuncData[0].dValueX = 0.0;
			mlfcD.arFuncData[0].dValueY = 0.0;
			mlfcD.arFuncData[1].dValueX = disp[i];
			mlfcD.arFuncData[1].dValueY = 1.0;
			mlfcD.arFuncData[2].dValueX = disp[i]+ 0.00001;
			mlfcD.arFuncData[2].dValueY = 100000000000.0;
			mlfcK = m_pDoc->m_pAttrCtrl->GetMlfcByData(mlfcD.arFuncData);
			if (mlfcK == 0)
			{
				CString mlfcName = strFixedName;// strFixedName[i];//Equivalent function
				int nIndex = 1;
				while(m_pDoc->m_pAttrCtrl->ExistMlfc(mlfcName))
				{
					mlfcName.Format(_T("%s_%d"), strFixedName,++nIndex);
				}
				mlfcD.FuncName = mlfcName;
				mlfcK = m_pDoc->m_pAttrCtrl->GetStartNumMlfc();
				if (!m_pDoc->m_pEditData->AddMlfc(mlfcD))
				{
					m_pDoc->m_pUndoCtrl->CancelEditDB();
					return;
				}
			}
			ElnkD.MlfcK = mlfcK;
			ElnkK = m_pDoc->m_pAttrCtrl->GetStartNumElnk();
			ElnkD.nSubOption = 4;
			ElnkD.nDirection = i;
			if (!AddLdsb_Elnk(ElnkK, ElnkD))
			{
				m_pDoc->m_pUndoCtrl->CancelEditDB();
				return;
			}
			bTranslate = TRUE;
		}
	}

	if(bTranslate)
		m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, LT_ELNK_CMD);
	else
		m_pDoc->m_pUndoCtrl->CancelEditDB();
}

BOOL CBndrElnkDlg::AddLdsb_Elnk(T_ELNK_K Key, T_ELNK_D& rData)
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<double, double> aDistance;
	BOOL bCopyLink = m_wndCopyLoad.GetCheck();
	double ux = 0.0, uy = 0.0, uz = 0.0;
	double wx = 0.0, wy = 0.0, wz = 0.0;
	if (bCopyLink)
	{
		int nAxis = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlAxis, nAxis);
		switch (nAxis)
		{
		case 0: ux = 1.0; break;
		case 1: uy = 1.0; break;
		case 2: uz = 1.0; break;
		}
		pIGM->GetWcsVector(ux, uy, uz, wx, wy, wz);
		CString csDistance;
		m_wndDistEdit.GetWindowText(csDistance);
		if (!GetUEDistance(csDistance, aDistance) || aDistance.GetSize() <= 0)
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_distance_value_));
			return FALSE;
		}
	}
	int nCopyType = 0;
	int nCopyNum = 0;
	int nNodeInc = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCopyType, nCopyType);
	if (nCopyType == 0) // node inc
	{
		CFormulaEdit::GetEditValue(&m_editNtimes, nCopyNum);
		CFormulaEdit::GetEditValue(&m_editNodeInc, nNodeInc);
	}

	if (!m_pDoc->m_pEditData->AddElnk(Key, rData))
		return FALSE;

	int nCopy = 0;
	if (bCopyLink)
	{
		if (nCopyType == 1) // distance
		{
			nCopy = aDistance.GetSize();
			if (ux == 0. && uy == 0. && uz == 0.)
			{
				GSaveHistoryFormatNF(_LS(IDS_DB_IRG_COPY_AXIS));
				return FALSE;
			}
			CMathFunc::mathNormalize(ux, uy, uz, ux, uy, uz);   //단위벡터로 변환
			for (int i = 0; i < nCopy; i++)
			{
				if (aDistance[i] == 0.)
				{
					//GSaveHistoryNF("복제 거리가 올바르지 않습니다.");
					GSaveHistoryNF(_LS(IDS_DB_ERR_MSG_518));
					return FALSE;
				}
			}
		}
		else  // node inc
		{
			if (nNodeInc == 0)
			{
				GSaveHistoryFormatNF(_LS(IDS_DB_IRG_NODE_INC));
				return FALSE;
			}
			nCopy = nCopyNum;
		}
	}

	T_NODE_D DataNode1, DataNode2, DataTemp[2];
	m_pDoc->m_pAttrCtrl->GetNode(rData.Node1, DataNode1);
	m_pDoc->m_pAttrCtrl->GetNode(rData.Node2, DataNode2);

	T_ELNK_K KeyElnk;
	T_ELNK_D DataElnk;
	if (nCopyType == 1) // distance
	{
		for (int i = 0; i < nCopy; i++)
		{
			double dDistance = 0.;
			for (int j = 0; j <= i; j++)dDistance += aDistance[j];
			DataTemp[0].x = DataNode1.x + ux * dDistance;
			DataTemp[0].y = DataNode1.y + uy * dDistance;
			DataTemp[0].z = DataNode1.z + uz * dDistance;
			DataTemp[1].x = DataNode2.x + ux * dDistance;
			DataTemp[1].y = DataNode2.y + uy * dDistance;
			DataTemp[1].z = DataNode2.z + uz * dDistance;
			T_NODE_K KeyMaster[2];
			for (int j = 0; j < 2; j++)
			{
				KeyMaster[j] = m_pDoc->m_pDataCtrl->GetSamePointNodeFlag(DataTemp[j]);
				if (KeyMaster[j] == 0)
				{
					//GSaveHistoryNF("[오류] 복제될 위치의 Node를 찾을 수 없습니다.");
					GSaveHistoryNF(_LS(IDS_DB_ERR_MSG_519));
					return FALSE;
				}
			}

			KeyElnk = m_pDoc->m_pAttrCtrl->GetStartNumElnk();
			DataElnk = rData;
			DataElnk.Node1 = KeyMaster[0];
			DataElnk.Node2 = KeyMaster[1];
			if (!m_pDoc->m_pEditData->AddElnk(KeyElnk, DataElnk))
				return FALSE;
		}
	}
	else  // node inc
	{
		for (int i = 0; i < nCopy; i++)
		{
			KeyElnk = m_pDoc->m_pAttrCtrl->GetStartNumElnk();
			DataElnk = rData;
			DataElnk.Node1 += (i + 1)*nNodeInc;
			DataElnk.Node2 += (i + 1)*nNodeInc;
			if (!m_pDoc->m_pAttrCtrl->ExistNode(DataElnk.Node1) || !m_pDoc->m_pAttrCtrl->ExistNode(DataElnk.Node2))
			{
				//GSaveHistoryNF("[오류] 복제될 위치의 Node를 찾을 수 없습니다.");
				GSaveHistoryNF(_LS(IDS_DB_ERR_MSG_519));
				return FALSE;
			}
			if (!m_pDoc->m_pEditData->AddElnk(KeyElnk, DataElnk))
				return FALSE;
		}
	}
	return TRUE;
	
}

BOOL CBndrElnkDlg::ExternalInit_by_SelectLabel(UINT key)
{
	// 기존 ExternalInit 함수가 작성되어 있지 않아서 , 마일스톤 시연용으로 급하게 작업하였다. 
	// Elastic Link Option별로 변경되는 값들도 많고, T_ELNK_D 의 설정값 조건에 따라서 
	// 변경되는 컨트롤 들이 굉장히 많고 Select & Modify용으로 다시 한번 정리하여야 한다.
	// =====>> 이건 임시 작업물. <<=====
	m_bSelectByLabelMode = TRUE;

	//UNUSED(key);
	//ASSERT(0);
	T_ELNK_K _Key;
	T_ELNK_D _Data;

	_Key = key;
	if (FALSE == m_pDoc->m_pAttrCtrl->GetElnk(_Key, _Data))
		return FALSE;

	m_wndGroupCombo.ChangeSelect(_Data.GroupKey);

	_Data.nSubOption;
	m_wndELNKTypeCmb.SetCurSel(_Data.nSubOption);
	m_chkShearSpr.SetCheck(_Data.bShearSprLoc);
	UpdateData(FALSE);
	//_Data.
	// Bla~ Bla~Bla

	CString strNodes;
	strNodes.Format(_T("%d,%d"), _Data.Node1, _Data.Node2);
	m_wnd2Nodes.SetWindowText(strNodes);
	//m_wnd2Nodes.AddNodeKey(_Data.Node1);
	//m_wnd2Nodes.AddNodeKey(_Data.Node2);

	CString strBetaAngle;
	strBetaAngle.Format(_T("%d"), (int)_Data.dBetaAngle);
	m_wndBetaAngle.SelectString(0, strBetaAngle);
	m_wndBetaAngle.SetWindowText(strBetaAngle);
	

	//UpdateData(FALSE);

	_Data.dBetaAngle;


	m_bFixed[0] = _Data.bFixed[0];
	m_bFixed[1] = _Data.bFixed[1];
	m_bFixed[2] = _Data.bFixed[2];
	m_bFixed[3] = _Data.bFixed[3];
	m_bFixed[4] = _Data.bFixed[4];
	m_bFixed[5] = _Data.bFixed[5];

	UpdateData(FALSE);

	CString strSDx,strSDy, strSDz, strSRx, strSRy, strSRz;
	CString _strSDSRFormat = _T("%5.3f");
	strSDx.Format(_strSDSRFormat, _Data.Spring[0]); m_wndEdit[0].SetWindowText(strSDx);
	strSDy.Format(_strSDSRFormat, _Data.Spring[1]); m_wndEdit[1].SetWindowText(strSDy);
	strSDz.Format(_strSDSRFormat, _Data.Spring[2]); m_wndEdit[2].SetWindowText(strSDz);
	strSRx.Format(_strSDSRFormat, _Data.Spring[3]); m_wndEdit[3].SetWindowText(strSRx);
	strSRy.Format(_strSDSRFormat, _Data.Spring[4]); m_wndEdit[4].SetWindowText(strSRy);
	strSRz.Format(_strSDSRFormat, _Data.Spring[5]); m_wndEdit[5].SetWindowText(strSRz);
	
	_Data.MlfcK;

	_Data.nDirection;

	_Data.nSubOption;
	
	

	if (_Data.bShearSprLoc)
	{
		if (_Data.nSubOption == 4 || _Data.nSubOption == 6)
		{
			_Data.dDistRatioEndI;
			m_wndDistRatioEndI.SetEditUnit(_Data.dDistRatioEndI);
		}
		else
		{
			m_editDistRatioSDy.SetEditUnit(_Data.dDistRatioSDy);
			m_editDistRatioSDz.SetEditUnit(_Data.dDistRatioSDz);
			//data.dDistRatioSDy = m_editDistRatioSDy.GetEditValue();
			//data.dDistRatioSDz = m_editDistRatioSDz.GetEditValue();
		}
	}

	//nType = _Data.nSubOption;

	OnTmElasticLinkDataTypeChange();

	//OnTmDirectionChange();

	//OnTmShearCheck();
	int nOpt = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOpt);
	int nCheck = m_chkShearSpr.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDistRatio, nOpt==0 && nCheck!=0);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDistRatioEndI, nOpt==0 && nCheck!=0);
	
	
	
	return FALSE; 
}

/*
BOOL CBndrFrlsDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	T_FRLS_K Key;
	T_FRLS_D data;
	Key.keymap = key;
	m_pDoc->m_pAttrCtrl->GetFrls(Key, data);

	CDlgUtil::CtrlRadioSetCheck(this, m_aTypeCtrl, data.nType);
	OnTmChangeType();

	for (int k=0; k < 2; k++)
	{
		for (int i=0; i < FRLS_SIZE; i++)
		{
			if (data.Release[k][i] == '1')
			{
				m_Flag[k][i].SetCheck(TRUE);
				m_Value[k][i].SetEditUnit(data.Fixity[k][i]);
			}
			else
				m_Flag[k][i].SetCheck(FALSE);
		}
	}
	OnToggleFlag();

	m_nOption = 0;

	// Group도 Setting
	T_BNGR_D  bngrD;
	m_pDoc->m_pAttrCtrl->GetBngr(data.GroupKey, bngrD);

	for (int i=0; i<m_wndGroupCombo.GetCount(); i++)
	{
		CString str;
		m_wndGroupCombo.GetLBText(i, str);
		if (str.CompareNoCase(bngrD.GroupName)==0) { m_wndGroupCombo.SetCurSel(i); break; }
	}
	UpdateData(FALSE);
	return TRUE;
}

*/ 