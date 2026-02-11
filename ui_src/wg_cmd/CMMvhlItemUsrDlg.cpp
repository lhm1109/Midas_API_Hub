// CMMvhlItemUsrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUsrDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\VehlDB.h"

#include "CMMvhlItemUsrTrckDlg.h"
#include "CMMvhlItemUsrTrainDlg.h"
#include "CMMvhlItemUsrBSDlg.h"
#include "CMMvhlItemUsrBSHAPage.h"
#include "CMMvhlItemUsrBSHBPage.h"
#include "CMMvhlItemUsrBSHAHBPage.h"
#include "CMMvhlItemUsrBSBD3701Dlg.h"
#include "CMMvhlItemUsrBSPedDlg.h"
#include "CMMvhlItemUsrBSSpecialPage.h"
#include "CMMvhlItemUsrPermitDlg.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrDlg dialog


CCMMvhlItemUsrDlg::CCMMvhlItemUsrDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUsrDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMMvhlItemUsrDlg)
	m_nTrkRadio = 0;
	m_nTrkLegalRadio = 0;
	m_nBSType   = 0;	
	//}}AFX_DATA_INIT

	m_pTrnDlg = NULL;
	m_pTrainDlg = NULL;

	m_pBS5400Dlg = NULL;
	m_pBSHADlg   = NULL;
	m_pBSHBDlg   = NULL;
	m_pBSHAHBDlg = NULL;
	m_pBSHAHBAUTODlg = NULL;
	m_pBSPedDlg = NULL;
	m_pBSSpecialDlg = NULL;

	m_nMovingType = 0;
	m_bPermitLoadCall = FALSE;

	m_nDynaType = 0;
	m_nLegalPermitRadio = 0;
	m_dHTrnDlg =0.0;
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);

	m_bPatchLoad = FALSE;
		
}


void CCMMvhlItemUsrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUsrDlg)  
	DDX_Control(pDX, IDC_CMD_W_CRAWLER_UNIT, m_wndWCrawlerUnit);
	DDX_Control(pDX, IDC_CMD_W_CRAWLER_EDIT, m_wndWCrawler);
	DDX_Control(pDX, IDC_CMD_D_CRAWLER_UNIT, m_wndDCrawlerUnit);
	DDX_Control(pDX, IDC_CMD_D_CRAWLER_EDIT, m_wndDCrawler);
	DDX_Control(pDX, IDC_CMD_U_Qq_UNIT, m_stQqUnit);
	DDX_Control(pDX, IDC_CMD_U_Qm_UNIT, m_stQmUnit);
	DDX_Control(pDX, IDC_CMD_U_P_UNIT, m_stPUnit);
	DDX_Control(pDX, IDC_CMD_W_UNIT, m_stWUnit);
	DDX_Control(pDX, IDC_CMD_PLV_UNIT, m_stPLVUnit);
	DDX_Control(pDX, IDC_CMD_PLM_UNIT, m_stPLMUnit);
	DDX_Control(pDX, IDC_CMD_PL_UNIT, m_stPLUnit);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_NTDIST_UNIT, m_stIrcNtDist);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_dD1_UNIT, m_stIrcD1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_dD2_UNIT, m_stIrcD2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_P_UNIT,   m_stIrcP);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_D_UNIT,   m_stIrcD);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_PB_UNIT,  m_stIrcPb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DB_UNIT,  m_stIrcDb);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_U_Qq, m_wndQq);	
	DDX_Control(pDX, IDC_CMD_U_Qm, m_wndQm);
	DDX_Control(pDX, IDC_CMD_U_P, m_wndP);
	DDX_Control(pDX, IDC_CMD_PLV, m_wndPlvEdit);
	DDX_Control(pDX, IDC_CMD_PLM, m_wndPlmEdit);
	DDX_Control(pDX, IDC_CMD_PL, m_wndPlEdit);
	DDX_Control(pDX, IDC_CMD_W, m_wndWEdit);
	DDX_Control(pDX, IDC_CMD_VEHICLE_NAME, m_wndVehicleName);
	DDX_Control(pDX, IDC_CMD_D, m_wndDEdit);
	DDX_Control(pDX, IDC_CMD_P, m_wndPEdit);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_NTDIST, m_wndIrcNtDist);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_dD1, m_wndIrcD1);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_dD2, m_wndIrcD2);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_P, m_wndIrcP);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_D, m_wndIrcD);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_PB, m_wndIrcPb);
	DDX_Control(pDX, IDC_CMD_MVHL_IRC_DB, m_wndIrcDb);
	DDX_Radio(pDX, IDC_CMD_ML_MVHL_ITEMU_TRK_RD, m_nTrkRadio);	
	DDX_Radio(pDX, IDC_CMD_ML_MVHL_ITEMU_LEGAL_TRK_RD, m_nTrkLegalRadio);	  
	DDX_Control(pDX, IDC_CMD_MVHL_U_TRUCK_COMBO, m_cmbChTruck);
	DDX_Control(pDX, IDC_CMD_MVHL_U_TRAIN_COMBO, m_cmbChTrain);
	DDX_Control(pDX, IDC_CMD_MVHL_U_CROWD_COMBO, m_cmbChCrowd);
 	DDX_Control(pDX, IDC_CMD_APPLY, m_wndBtnApply);	
	DDX_Control(pDX, IDC_CMD_W_CROWD_EDIT, m_edtCrowd);
	DDX_Control(pDX, IDC_CMD_W_CROWD_UNIT, m_untCrowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_QK_UNIT2,  m_untQkLane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_PK1_UNIT2, m_untPk1Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_PK2_UNIT2, m_untPk2Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L1_UNIT3,  m_untL1Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L2_UNIT3,  m_untL2Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_W1_UNIT2,  m_untW1Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_W2_UNIT2,  m_untW2Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L1_UNIT4,  m_untL1Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L2_UNIT4,  m_untL2Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_WD_UNIT2,  m_untWdCrowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_QK_EDIT2,  m_wndQkLane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_PK1_EDIT2, m_wndPk1Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_PK2_EDIT2, m_wndPk2Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L1_EDIT3,  m_wndL1Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L2_EDIT3,  m_wndL2Lane);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_W1_EDIT2,  m_wndW1Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_W2_EDIT2,  m_wndW2Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L1_EDIT4,  m_wndL1Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_L2_EDIT4,  m_wndL2Crowd);
	DDX_Control(pDX, IDC_CMD_MVHL_CH_WD_EDIT2,  m_wndWdCrowd);
	DDX_Radio  (pDX, IDC_CMD_ML_MVHL_ITEMU_BS_RDO1, m_nBSType);	

	// Korea - Lane(Length Specifired (탱크 하중))
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_NUM_CMB,       m_cmbUniformLoadNum);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_EDT,  m_edtUniformLoadDistance);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_UNT,  m_untUniformLoadDistance);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_EDT,         m_edtUniformLoadW);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_UNT,         m_untUniformLoadW);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_EDT,  m_edtUniformLoadLengthL);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_UNT,  m_untUniformLoadLengthL);

	// Canada Dynamic Load Allowance
	DDX_Radio  (pDX, IDC_CMD_MVHL_DYNA_AUTO_RDO,      m_nDynaType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_1AXLE_EDT,     m_edt1Axle);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_2AXLE_EDT,     m_edt2Axle);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_3AXLE_EDT,     m_edt3Axle);

	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_3AXLE_EDT,     m_edt3Axle);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_3AXLE_EDT,     m_edt3Axle);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_3AXLE_EDT,     m_edt3Axle);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_3AXLE_EDT,     m_edt3Axle);
	
	DDX_Control(pDX, IDC_CMD_MVHL_IMP_EDT,            m_edtImpactFactor);
	DDX_Control(pDX, IDC_CMD_MVHL_IMP_UNIT,            m_untImpactFactor);  
	DDX_Control(pDX, IDC_CMD_U_W_EDT,                 m_edtW);
	DDX_Control(pDX, IDC_CMD_U_R_EDT,                 m_edtR);
	DDX_Control(pDX, IDC_CMD_U_DIST_EDT,              m_edtDist);
	DDX_Radio(pDX, IDC_CMD_ML_MVHL_ITEMU_LEGAL_TYPE_RD, m_nLegalPermitRadio);	

	DDX_Control(pDX, IDC_CMD_U_W_UNIT,                m_untW);
	DDX_Control(pDX, IDC_CMD_U_R_UNIT,                m_untR);
	DDX_Control(pDX, IDC_CMD_U_DIST_UNIT,             m_untDist);

	DDX_Control(pDX, IDC_CMD_MVHL_ADD_CF_CHK, m_chkCentF);
	DDX_Control(pDX, IDC_BITMAP_STATIC, m_wndPicture);
	DDX_Control(pDX, IDC_BITMAP_STATIC2, m_wndPicture2);

	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK, m_chkPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 3
//-------------------------------------------------------------------------
// Implementation
void CCMMvhlItemUsrDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};

	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");

	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 85;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUsrDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	if (nLoadCount == 0) return;

	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
}

BOOL CCMMvhlItemUsrDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;

	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i == 0) m_List.InsertItem(&lvitem);
		else       m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvhlItemUsrDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_List.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUsrDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
{
	int nItem = nPos;
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			if (i == 0) str.Format(_T("%d"), nNo);
			else if (i == 1) str.Format(_T("%g"), dLoad);
			else if (i == 2)
			{
				if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
				else str.Format(_T("%g"), dDist);
			}
			else str = _LS(IDS_WG_CMD__ADDD__Error);
			m_List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

void CCMMvhlItemUsrDlg::Data2Dlg()
{
	UpdateData(TRUE);
	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;
	if(MvcdD.nCodeType == D_MOVE_CODE_BS)
	{
		m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
		m_nBSType = m_Data.nSubType; //0:BS5400, 1:HA, 2:HB, 3:HA&HB, 4:HA&HB AUTO, 5:Pedestrian, 6:Special Vehicle
		
		if(!m_bModify)
		{
			// Default 를 받아오기 위한 처리
			m_Data.bStandard = TRUE;
			if     (m_nBSType==0) { m_Data.nStandardCode = D_MVHL_BS_5400;  m_Data.VehicleTypeName = _T("HA & HB (BS5400)"); }
			else if(m_nBSType==1) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HA"); }
			else if(m_nBSType==2) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HB"); }
			else if(m_nBSType==3) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HA & HB"); }
			else if(m_nBSType==4) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HA & HB(AUTO)"); }
			else if(m_nBSType==5) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("PEDESTRIAN"); }
			else if(m_nBSType==6) { } // Nothing
			else ASSERT(0);

			CVehlDB db(m_pDoc);
			db.GetStandardVehicleLoadValue(m_Data);

			m_Data.bStandard = FALSE;
			m_Data.nStandardCode = 0;
			m_Data.VehicleTypeName = _T("");
		}
	
		if(m_nBSType==0)
		{
			if(m_pBS5400Dlg) m_pBS5400Dlg->SetData2Dlg();
		}
		else if(m_nBSType==1)
		{
			if(m_pBSHADlg) m_pBSHADlg->SetData2Dlg();
		}
		else if(m_nBSType==2)
		{
			if(m_pBSHBDlg) m_pBSHBDlg->SetData2Dlg();
		}
		else if(m_nBSType==3)
		{
			if(m_pBSHAHBDlg) m_pBSHAHBDlg->SetData2Dlg();
		}
		else if(m_nBSType==4)
		{
			if(m_pBSHAHBAUTODlg) m_pBSHAHBAUTODlg->SetData2Dlg();
		}
		else if(m_nBSType==5)
		{
			if(m_pBSPedDlg) m_pBSPedDlg->SetData2Dlg();
		}
		else if(m_nBSType==6)
		{
			if(m_pBSSpecialDlg) m_pBSSpecialDlg->SetData2Dlg();
		}
		else ASSERT(0);

		UpdateData(FALSE);

		return;
	}
	else if (m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		T_MVHL_PATCH_LOAD PatchLoadDB;
		T_MVHL_D  TempData;
		// Default 를 받아오기 위한 처리
		TempData.bStandard = TRUE;
		TempData.nStandardCode = D_MVHL_AASHTO_LRFD;
		TempData.VehicleTypeName = _T("HL-93TRK");
		CVehlDB db(m_pDoc);
		if (db.GetStandardVehicleLoadValue(TempData, &PatchLoadDB))	MakeItemEx();

		if (!m_bModify)
		{
			m_Data.PatchLoad = PatchLoadDB;
		}
		else
		{
			if (m_Data.PatchLoad.aPatchLoad.GetSize() == 0)
			{
				m_Data.PatchLoad = PatchLoadDB;
			}
		}
	}

	// truck and lane load
	CString csVal = _T("");
	if(GetTrkRadio()==0)
	{
		m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
		if(m_nMovingType == D_MOVE_CODE_INDIA)
		{
			csVal.Format(_T("%g"), m_Data.dNoseTailDist); m_wndIrcNtDist.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dD1); m_wndIrcD1.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dD2); m_wndIrcD2.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dP);  m_wndIrcP.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dDTrailer);  m_wndIrcD.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dPb);  m_wndIrcPb.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dDb);  m_wndIrcDb.SetWindowText(csVal);
		}
		else if(m_nMovingType == D_MOVE_CODE_CHINA)
		{
			csVal.Format(_T("%g"), m_Data.dP);        m_wndP.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dQm);       m_wndQm.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dQq);       m_wndQq.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dDTrailer); m_wndDCrawler.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dWTrailer); m_wndWCrawler.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dQq);       m_wndQkLane.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dPa);       m_wndPk1Lane.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dPb);       m_wndPk2Lane.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dD1);       m_wndL1Lane.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dD2);       m_wndL2Lane.SetWindowText(csVal);
		}     
		else if(m_nMovingType == D_MOVE_CODE_CANADA)
		{		  
		  csVal.Format(_T("%g"), m_Data.dUniformLoad);  m_wndWEdit.SetWindowText(csVal);		  

			m_chkCentF.SetCheck(m_Data.bCentF);
			m_nDynaType = m_Data.nDynaFactor;
			m_edt1Axle.SetEditUnit(m_Data.dDynaFact1Axle);
			m_edt2Axle.SetEditUnit(m_Data.dDynaFact2Axle);
			m_edt3Axle.SetEditUnit(m_Data.dDynaFact3Axle);
		}
		else
		{
			csVal.Format(_T("%g"), m_Data.dUniformLoad);  m_wndWEdit.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dPL);     		  m_wndPlEdit.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dPLM);    		  m_wndPlmEdit.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dPLV);     		  m_wndPlvEdit.SetWindowText(csVal);
		}    
	}
	else if(GetTrkRadio()==1)  // train load
	{
		m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
		int nIndex = m_cmbChTrain.GetItemData(m_cmbChTrain.GetCurSel());		
		if(nIndex!=2)
			Data2DlgOfTrnLd();
		else
			Data2DlgOfTrainLd();
	}  
	else if(GetTrkRadio()==2)
	{
		m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
		if(m_Data.nLoadType == 2)
		{
			csVal.Format(_T("%g"), m_Data.dWCrowd); m_edtCrowd.SetWindowText(csVal);
		}
		else if(m_Data.nLoadType == 3)
		{
			csVal.Format(_T("%g"), m_Data.dWPres1); m_wndW1Crowd.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dWPres2); m_wndW2Crowd.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dD1);     m_wndL1Crowd.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dD2);     m_wndL2Crowd.SetWindowText(csVal);
			csVal.Format(_T("%g"), m_Data.dDb);     m_wndWdCrowd.SetWindowText(csVal);
		}
	}
	else if(GetTrkRadio()==3) // Korea - Lane(Length Specifired (탱크 하중))
	{ 
		m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
		m_cmbUniformLoadNum.SetCurSel(m_Data.nUniformLoadNum);
		m_edtUniformLoadDistance.SetEditUnit(m_Data.dUniformLoadDistance);
		m_edtUniformLoadW.SetEditUnit(m_Data.dUniformLoadW);
		m_edtUniformLoadLengthL.SetEditUnit(m_Data.dUniformLoadLengthL);
	}
	else ASSERT(FALSE);

	if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
		m_edtImpactFactor.SetEditUnit(m_Data.dDynLoadAllowance);
		m_edtW.SetEditUnit(m_Data.dCombinedUniformLoad);
		m_edtR.SetEditUnit(m_Data.dLegalR);
		m_edtDist.SetEditUnit(m_Data.dCombinedTruckSpace);
		m_nLegalPermitRadio = m_Data.nLegalPermitType;

		m_chkCentF.SetCheck(m_Data.bCentF);

		m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);
		m_edtPatchW.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dWidth);
		m_edtPatchL.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dLength);
		OnChkPatch();

		if (m_nLegalPermitRadio == 2)
		{
			GetDlgItem(IDC_CMD_U_DIST_STATIC)->SetWindowText(_LS(IDS_CMD_MVHL_MVHL_LEGAL_MIN_DIST));
		}
		else
		{
			GetDlgItem(IDC_CMD_U_DIST_STATIC)->SetWindowText(_LS(IDS_CMD_MVHL_MVHL_LEGAL_DIST));
		}
	}

	MakeItemEx();

	UpdateData(FALSE);
 
	if     (m_Data.nTruckType == 6) m_cmbChTruck.SetCurSel(0); // Crawler Type
	else if(m_Data.nTruckType == 1) m_cmbChTruck.SetCurSel(3); // Crawler Type
	else if(m_Data.nTruckType == 5) m_cmbChTruck.SetCurSel(2); // Lane Load2
	else if(m_Data.nTruckType == 2) m_cmbChTruck.SetCurSel(4); // Lane Load2
	else m_cmbChTruck.SetCurSel(1); // Lane Load1 etc.

	if     (m_Data.nTrainType == 2) m_cmbChTrain.SetCurSel(1); // Type2
	else if(m_Data.nTrainType == 4) m_cmbChTrain.SetCurSel(3); // China Train subway
	else if(m_Data.nTrainType == 5) m_cmbChTrain.SetCurSel(2); // Type3
	else m_cmbChTrain.SetCurSel(0); // Type1 etc.

	if(m_Data.nLoadType == 3) m_cmbChCrowd.SetCurSel(1); // Crowd Load Type 2
	else m_cmbChCrowd.SetCurSel(0); // Crowd Load Type 1

	EnableLegalPermitRadio();
}

BOOL CCMMvhlItemUsrDlg::Dlg2Data()
{	
	UpdateData(TRUE);  
	m_Data.bStandard = FALSE;    
	m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);	
	
	//AASHTO_LRFD - Truck & Legal/Permit 일때만 저장
	m_Data.bCentF = FALSE;
	if(GetTrkRadio()==0)  // truck and lane load    
	{    
		m_Data.nLoadType = 0;
		if(m_nMovingType == D_MOVE_CODE_CHINA)  // China
		{
			m_Data.nTrainType = 1;  
			m_Data.nTruckType = m_cmbChTruck.GetItemData(m_cmbChTruck.GetCurSel());
			m_Data.dP = 0.;
			m_Data.dQm = 0.;
			m_Data.dQq = 0.;
			m_Data.dPa = 0.;
			m_Data.dPb = 0.;
			m_Data.dD1 = 0.;
			m_Data.dD2 = 0.;
			m_Data.dDTrailer = 0.;
			m_Data.dWTrailer = 0.;
			m_Data.dUniformLoad = 0.;
			m_Data.dPL = 0.;
			m_Data.dPLM = 0.;
			m_Data.dPLV = 0.;
			if(m_Data.nTruckType == 1)  // Crawler Type
			{        
				CFormulaEdit::GetEditValue(&m_wndDCrawler, m_Data.dDTrailer);
				CFormulaEdit::GetEditValue(&m_wndWCrawler, m_Data.dWTrailer);
				for(int i=0; i<D_MVHL_NUMLOAD; i++)
				{
					m_Data.dPointLoad[i]=0.;
					m_Data.dPointDistance[i]=0.;
				}      
			}
			else if(m_Data.nTruckType == 0) // Lane Load 1
			{        
				CFormulaEdit::GetEditValue(&m_wndP, m_Data.dP);
				CFormulaEdit::GetEditValue(&m_wndQm, m_Data.dQm);
				CFormulaEdit::GetEditValue(&m_wndQq, m_Data.dQq);      
			}
			else if(m_Data.nTruckType == 5) // Lane Load 2
			{        
				CFormulaEdit::GetEditValue(&m_wndQkLane,  m_Data.dQq);
				CFormulaEdit::GetEditValue(&m_wndPk1Lane, m_Data.dPa);
				CFormulaEdit::GetEditValue(&m_wndPk2Lane, m_Data.dPb);
				CFormulaEdit::GetEditValue(&m_wndL1Lane,  m_Data.dD1);
				CFormulaEdit::GetEditValue(&m_wndL2Lane,  m_Data.dD2);
				for(int i=0; i<D_MVHL_NUMLOAD; i++)
				{
					m_Data.dPointLoad[i]=0.;
					m_Data.dPointDistance[i]=0.;
				}
			}
			else if(m_Data.nTruckType == 6) // Truck(JTG B01-2003)
			{
			}
			else if(m_Data.nTruckType == 2) // GC Type Load
			{
			}
		}
		else if(m_nMovingType == D_MOVE_CODE_INDIA)
		{
			m_Data.nTruckType = 3;
			m_Data.nTrainType = 0;
			CFormulaEdit::GetEditValue(&m_wndIrcNtDist, m_Data.dNoseTailDist);
			CFormulaEdit::GetEditValue(&m_wndIrcD1, m_Data.dD1);
			CFormulaEdit::GetEditValue(&m_wndIrcD2, m_Data.dD2);
			CFormulaEdit::GetEditValue(&m_wndIrcP,  m_Data.dP);
			CFormulaEdit::GetEditValue(&m_wndIrcD,  m_Data.dDTrailer);
			CFormulaEdit::GetEditValue(&m_wndIrcPb, m_Data.dPb);
			CFormulaEdit::GetEditValue(&m_wndIrcDb, m_Data.dDb);
			m_Data.dQm = 0.;
			m_Data.dQq = 0.;
			m_Data.dWTrailer = 0.;
			m_Data.dUniformLoad = 0.;
			m_Data.dPL = 0.;
			m_Data.dPLM = 0.;
			m_Data.dPLV = 0.;
		}
		else if(m_nMovingType == D_MOVE_CODE_CANADA)  // Canada
		{
			m_Data.nTrainType = 0;  
			m_Data.nTruckType = 0;
			m_Data.dP = 0.;
			m_Data.dQm = 0.;
			m_Data.dQq = 0.;
			m_Data.dPa = 0.;
			m_Data.dPb = 0.;
			m_Data.dD1 = 0.;
			m_Data.dD2 = 0.;
			m_Data.dDTrailer = 0.;
			m_Data.dWTrailer = 0.;
			m_Data.dUniformLoad = 0.;
			m_Data.dPL = 0.;
			m_Data.dPLM = 0.;
			m_Data.dPLV = 0.;
			CFormulaEdit::GetEditValue(&m_wndWEdit, m_Data.dUniformLoad);

			m_Data.bCentF = m_chkCentF.GetCheck();

			m_Data.nDynaFactor = m_nDynaType;
			m_Data.dDynaFact1Axle = m_edt1Axle.GetEditValue();
			m_Data.dDynaFact2Axle = m_edt2Axle.GetEditValue();
			m_Data.dDynaFact3Axle = m_edt3Axle.GetEditValue();
		}    
		else
		{
			if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
			{
				m_Data.bCentF = m_chkCentF.GetCheck();

				T_MVHL_PATCH_ITEM Patch;
				Patch.dWidth  = m_edtPatchW.GetEditValue();
				Patch.dLength = m_edtPatchL.GetEditValue();
				m_Data.PatchLoad.aPatchLoad.RemoveAll();
				m_Data.PatchLoad.aPatchLoad.Add(Patch);
				m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();
			}
			m_Data.nTruckType = 0;  // Korea는 0
			m_Data.nTrainType = 0;  // Korea는 0
			CFormulaEdit::GetEditValue(&m_wndWEdit, m_Data.dUniformLoad);
			CFormulaEdit::GetEditValue(&m_wndPlEdit, m_Data.dPL);
			CFormulaEdit::GetEditValue(&m_wndPlmEdit, m_Data.dPLM);
			CFormulaEdit::GetEditValue(&m_wndPlvEdit, m_Data.dPLV);
			m_Data.dP = 0.;
			m_Data.dQm = 0.;
			m_Data.dQq = 0.;
			m_Data.dDTrailer = 0.;
			m_Data.dWTrailer = 0.;
		}
	}
	else if(GetTrkRadio()==1)  // train load (m_nTrkRadio==1)
	{
		m_Data.nLoadType = 1;
		m_Data.bStandard = FALSE;
	  m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);
		m_Data.nTruckType = 0;    
		if(m_nMovingType == D_MOVE_CODE_INDIA) m_Data.nTrainType = 3; // India:3
		else if(m_nMovingType == D_MOVE_CODE_CHINA || m_nMovingType == D_MOVE_CODE_CANADA)
		{
			m_Data.nTrainType = m_cmbChTrain.GetItemData(m_cmbChTrain.GetCurSel());
		}
		else
		{
			m_Data.nTrainType = 0;  // Korea는 0
		}
		int nIndex = m_cmbChTrain.GetItemData(m_cmbChTrain.GetCurSel());		
		if(nIndex!=2)
			Dlg2DataOfTrnLd();
		else
			Dlg2DataOfTrainLd();

	}
	else if(GetTrkRadio()==2)  // Crowd Load
	{
		if (m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
		{
		}
		else
		{
			m_Data.nTruckType = 0;
			m_Data.nTrainType = 0;    
			m_Data.nLoadType = m_cmbChCrowd.GetItemData(m_cmbChCrowd.GetCurSel());
			if(m_Data.nLoadType == 2)  // Type 1
			{
				CFormulaEdit::GetEditValue(&m_edtCrowd, m_Data.dWCrowd);
			}
			else if(m_Data.nLoadType == 3)  // Type 2
			{
				CFormulaEdit::GetEditValue(&m_wndW1Crowd, m_Data.dWPres1);
				CFormulaEdit::GetEditValue(&m_wndW2Crowd, m_Data.dWPres2);
				CFormulaEdit::GetEditValue(&m_wndL1Crowd, m_Data.dD1);
				CFormulaEdit::GetEditValue(&m_wndL2Crowd, m_Data.dD2);
				CFormulaEdit::GetEditValue(&m_wndWdCrowd, m_Data.dDb);
			}
		}
	}	
	else if(GetTrkRadio()==3) // _T("Korea - Lane(Length Specifired (탱크 하중))") 또는 _T("Legal Load")
	{
		if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
		{
			m_Data.nLoadType = 9;
			m_Data.dDynLoadAllowance = m_edtImpactFactor.GetEditValue();
			m_Data.dCombinedUniformLoad = m_edtW.GetEditValue();
			m_Data.dUniformLoad = 0.0; // 이 한줄은 출시 후 삭제해도 됩니다.
			m_Data.dLegalR = m_edtR.GetEditValue();
			m_Data.dCombinedTruckSpace= m_edtDist.GetEditValue();
			m_Data.nLegalPermitType = m_nLegalPermitRadio;

			m_Data.bCentF = m_chkCentF.GetCheck(); 

			T_MVHL_PATCH_ITEM Patch;
			Patch.dWidth  = m_edtPatchW.GetEditValue();
			Patch.dLength = m_edtPatchL.GetEditValue();
			m_Data.PatchLoad.aPatchLoad.RemoveAll();
			m_Data.PatchLoad.aPatchLoad.Add(Patch);
			m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();
		}
		else
		{
			m_Data.nLoadType = 6;
			m_Data.nUniformLoadNum = m_cmbUniformLoadNum.GetCurSel();
			m_Data.dUniformLoadDistance = m_edtUniformLoadDistance.GetEditValue();
			m_Data.dUniformLoadW = m_edtUniformLoadW.GetEditValue();
			m_Data.dUniformLoadLengthL = m_edtUniformLoadLengthL.GetEditValue();
		}    
	}
	else ASSERT(FALSE);
	return TRUE;
}

void CCMMvhlItemUsrDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;	
	
	CArray<UINT, UINT> aCtrlChLane1;   // 하중입력 edit box  
	aCtrlChLane1.Add(IDC_CMD_MVHL_USR_LANE_FRM);
	aCtrlChLane1.Add(IDC_CMD_U_P_STATIC);
	aCtrlChLane1.Add(IDC_CMD_U_Qm_STATIC);
	aCtrlChLane1.Add(IDC_CMD_U_Qq_STATIC);
	aCtrlChLane1.Add(IDC_CMD_U_P);
	aCtrlChLane1.Add(IDC_CMD_U_Qm);
	aCtrlChLane1.Add(IDC_CMD_U_Qq);
	aCtrlChLane1.Add(IDC_CMD_U_P_UNIT);
	aCtrlChLane1.Add(IDC_CMD_U_Qm_UNIT);
	aCtrlChLane1.Add(IDC_CMD_U_Qq_UNIT);

	CArray<UINT, UINT> aCtrlChCrawl;   // Crawler Type의 Edit Box
	aCtrlChCrawl.Add(IDC_CMD_MVHL_USR_CRAWL_FRM);
	aCtrlChCrawl.Add(IDC_CMD_W_CRAWLER_STATIC);
	aCtrlChCrawl.Add(IDC_CMD_W_CRAWLER_EDIT);
	aCtrlChCrawl.Add(IDC_CMD_W_CRAWLER_UNIT);
	aCtrlChCrawl.Add(IDC_CMD_D_CRAWLER_STATIC);
	aCtrlChCrawl.Add(IDC_CMD_D_CRAWLER_EDIT);
	aCtrlChCrawl.Add(IDC_CMD_D_CRAWLER_UNIT);  

	CArray<UINT, UINT> aCtrlIrc;    
	aCtrlIrc.Add(IDC_CMD_IRC_TRACK_STATIC);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_P_STATIC);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_P);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_P_UNIT);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_D_STATIC);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_D);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_D_UNIT);
	aCtrlIrc.Add(IDC_CMD_IRC_BOGIE_STATIC);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_PB_STATIC);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_PB);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_PB_UNIT);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_DB_STATIC);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_DB);
	aCtrlIrc.Add(IDC_CMD_MVHL_IRC_DB_UNIT);

	CArray<UINT, UINT> aCtrlChLane2;  
	aCtrlChLane2.Add(IDC_CMD_MVHL_USR_LANE2_FRM);  
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_QK_EDIT2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_PK1_EDIT2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_PK2_EDIT2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_L1_EDIT3);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_L2_EDIT3);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_QK_UNIT2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_PK1_UNIT2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_PK2_UNIT2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_L1_UNIT3);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_L2_UNIT3);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_QK_STATIC2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_PK1_STATIC2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_PK2_STATIC2);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_L1_STATIC3);
	aCtrlChLane2.Add(IDC_CMD_MVHL_CH_L2_STATIC3);

	CArray<UINT, UINT> aCtrlChCrowd;      
	aCtrlChCrowd.Add(IDC_CMD_MVHL_USR_CROWD_FRM);  
	aCtrlChCrowd.Add(IDC_CMD_W_CROWD_STATIC);
	aCtrlChCrowd.Add(IDC_CMD_W_CROWD_EDIT);
	aCtrlChCrowd.Add(IDC_CMD_W_CROWD_UNIT);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_W1_EDIT2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_W2_EDIT2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_L1_EDIT4);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_L2_EDIT4);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_W1_UNIT2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_W2_UNIT2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_L1_UNIT4);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_L2_UNIT4);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_W1_STATIC2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_W2_STATIC2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_L1_STATIC4);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_L2_STATIC4);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_WD_STATIC2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_WD_EDIT2);
	aCtrlChCrowd.Add(IDC_CMD_MVHL_CH_WD_UNIT2);

	CArray<UINT, UINT> aCtrlKoreaLane;    // Korea - Lane(Length Specifired (탱크 하중))
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_GRP);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_NUM_STC);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_NUM_CMB);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_STC);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_EDT);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_UNT);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_STC);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_EDT);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_UNT);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_STC);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_EDT);
	aCtrlKoreaLane.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_UNT);

	m_aCtrlPatch.RemoveAll();
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_CHK);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_UNT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_UNT);

	CArray<UINT, UINT> aControls;
	UINT aFirstCtrlID[] = {IDC_CMD_MVHL_USR_LANE_FRM, IDC_CMD_MVHL_USR_CRAWL_FRM, 
												 IDC_CMD_MVHL_USR_LANE2_FRM, IDC_CMD_MVHL_USR_CROWD_FRM, 
												 IDC_CMD_IRC_TRACK_STATIC, IDC_CMD_ML_MVHL_ITEMU_LANE_GRP};
	CArray<UINT, UINT> *aCtrlSet[] = {&aCtrlChLane1, &aCtrlChCrawl, &aCtrlChLane2, 
																		&aCtrlChCrowd, &aCtrlIrc, &aCtrlKoreaLane};
	GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}

	// Bitmap 이동
	aControls.RemoveAll();
	aControls.Add(IDC_BITMAP_STATIC2);

	GetDlgItem(IDC_BITMAP_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_BITMAP_STATIC2)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	// BS
	CArray<UINT, UINT> aBS; 
	aBS.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO1);
	aBS.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO2);
	aBS.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO3);
	aBS.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO4);
	aBS.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO5);
	aBS.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO6);
	aBS.Add(IDC_CMD_ML_MVHL_ITEMU_BS_RDO7);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO1)->GetWindowRect(rToMove);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->GetWindowRect(rRef);
	nDistY = rRef.bottom - rToMove.top - globalUtils.ScaleByDPI(12);
	CDlgUtil::CtrlMoveDistY(this, aBS, nDistY);
	
	// Korea - Lane(Length Specifired (탱크 하중)) 
	// Korea - Lane(Length Specifired (탱크 하중)) 일때 Redio Button 이 Crowd Load 자리에 Lane (Length Specifired) 와야 한다.
	CArray<UINT, UINT> aCtrlKoreaLaneRdo;
	aCtrlKoreaLaneRdo.Add(IDC_CMD_ML_MVHL_ITEMU_LANE_RDO);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_CRD_RD)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_RDO)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrlKoreaLaneRdo, nDistY);

	// Dynamic Load Allownace
	CArray<UINT, UINT> aDyna; // Dynamic Load Allowance
	aDyna.Add(IDC_CMD_MVHL_DYNA_GRP);
	aDyna.Add(IDC_CMD_MVHL_DYNA_AUTO_RDO);
	aDyna.Add(IDC_CMD_MVHL_DYNA_USER_RDO);
	aDyna.Add(IDC_CMD_MVHL_DYNA_1AXLE_EDT);
	aDyna.Add(IDC_CMD_MVHL_DYNA_1AXLE_STC);
	aDyna.Add(IDC_CMD_MVHL_DYNA_2AXLE_EDT);
	aDyna.Add(IDC_CMD_MVHL_DYNA_2AXLE_STC);
	aDyna.Add(IDC_CMD_MVHL_DYNA_3AXLE_EDT);
	aDyna.Add(IDC_CMD_MVHL_DYNA_3AXLE_STC);
	GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_DYNA_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aDyna, nDistY);

	if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		aControls.RemoveAll(); // Load Type Radio 가 4개 일때 아래로 내려가야 하는 컨트럴 들
		aControls.Add(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES);
		aControls.Add(IDC_CMD_VEHICLE_NAME_STC);
		aControls.Add(IDC_CMD_VEHICLE_NAME);
		aControls.Add(IDC_BITMAP_STATIC);
		aControls.Add(IDC_CMD_TRCKLD_STATIC);
		aControls.Add(IDC_CMD_P_STATIC);
		aControls.Add(IDC_CMD_D_STATIC);
		aControls.Add(IDC_CMD_P);
		aControls.Add(IDC_CMD_D);
		aControls.Add(IDC_CMD_BTN_ADD);
		aControls.Add(IDC_CMD_BTN_INSERT);
		aControls.Add(IDC_CMD_BTN_MODIFY);
		aControls.Add(IDC_CMD_BTN_DELETE);
		aControls.Add(IDC_CMD_LIST);
		aControls.Add(IDC_CMD_MVHL_IRC_dD1_STATIC);
		aControls.Add(IDC_CMD_MVHL_IRC_dD1);
		aControls.Add(IDC_CMD_MVHL_IRC_dD1_UNIT);
		aControls.Add(IDC_CMD_MVHL_IRC_dD2_STATIC);
		aControls.Add(IDC_CMD_MVHL_IRC_dD2);
		aControls.Add(IDC_CMD_MVHL_IRC_dD2_UNIT);
		aControls.Add(IDC_CMD_LNLD_STATIC);
		aControls.Add(IDC_CMD_W_STATIC);
		aControls.Add(IDC_CMD_PL_STATIC);
		aControls.Add(IDC_CMD_PLM_STATIC);
		aControls.Add(IDC_CMD_PLV_STATIC);
		aControls.Add(IDC_CMD_W);
		aControls.Add(IDC_CMD_PL);
		aControls.Add(IDC_CMD_PLM);
		aControls.Add(IDC_CMD_PLV);
		aControls.Add(IDC_CMD_W_UNIT);
		aControls.Add(IDC_CMD_PL_UNIT);
		aControls.Add(IDC_CMD_PLM_UNIT);
		aControls.Add(IDC_CMD_PLV_UNIT);
		aControls.Add(IDOK);
		aControls.Add(IDCANCEL);
		aControls.Add(IDC_CMD_APPLY);
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_TYPE_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(35);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);  

		// Regal/Permit Type Radio를 Vehicular Load Name 아래로 이동
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TYPE_RD);
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_PERMIT_TYPE_RD);
		aControls.Add(IDC_CMD_ML_MVHL_ITEMU_HL_TYPE_RD);
		GetDlgItem(IDC_CMD_VEHICLE_NAME)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TYPE_RD)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);  
	}

	// Regal Load
	CArray<UINT, UINT> aCtrlLegalLoadType;
	aCtrlLegalLoadType.Add(IDC_CMD_MVHL_VEHICULAR_LEGAL_LOAD_TYPE_GRP);
	aCtrlLegalLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TRK_RD);
	aCtrlLegalLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD);
	aCtrlLegalLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TRN_RD);
	aCtrlLegalLoadType.Add(IDC_CMD_ML_MVHL_ITEMU_LEGAL_CRD_RD);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LEGAL_LOAD_TYPE_GRP)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aCtrlLegalLoadType, nDistY);
	
	// reSize
	if(m_nMovingType == D_MOVE_CODE_CANADA)
	{
		GetDlgItem(IDC_CMD_MVHL_DYNA_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);

		CArray<UINT, UINT> aCtrlCF;
		aCtrlCF.Add(IDC_CMD_MVHL_ADD_CF_CHK);
		CDlgUtil::CtrlMoveDistY(this, aCtrlCF, nDistY);

		CRect rectTarget;
		CRect rectRef1;
		CRect rectRef2;
		CRect rectRef3;
		CRect rectRef4;
		
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rectTarget);
		
		GetDlgItem(IDC_CMD_VEHICLE_NAME)->GetWindowRect(rectRef1);
		GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST_STATIC)->GetWindowRect(rectRef2);
		nDistY = rectRef2.bottom - rectRef1.top;
		
		GetDlgItem(IDC_CMD_MVHL_DYNA_GRP)->GetWindowRect(rectRef3);
		nDistY += rectRef3.bottom - rectRef3.top;
		
		GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rectRef4);
		nDistY += rectRef4.bottom - rectRef3.bottom;

		rectTarget.bottom = rectTarget.top + nDistY + globalUtils.ScaleByDPI(30);
		ScreenToClient(rectTarget);
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->MoveWindow(rectTarget);
	}

	AlignControlDynamic();  
}

void CCMMvhlItemUsrDlg::AlignControlDynamic()
{
	// 컨트럴을 동작할 때마다 움직여야 될 녀석은 여기에 몰아서 코딩하세요(대화상자 깜빡임 방지를 위해 한번만 불리는 것과 여러번 불리는 것을 분리했습니다.)

	CRect rRef;
	CRect rToMove;
	int nDistY=0;	
	int nDistX=0;	
	CArray<UINT, UINT> aControls;
	CArray<UINT, UINT> aControls2;

	if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		// Truck Load Group과 Image를 Regal/Permit Type Radio 아래로 이동
		aControls.RemoveAll();
		aControls.Add(IDC_CMD_TRCKLD_STATIC);
		aControls.Add(IDC_CMD_P_STATIC);
		aControls.Add(IDC_CMD_D_STATIC);
		aControls.Add(IDC_CMD_P);
		aControls.Add(IDC_CMD_D);
		aControls.Add(IDC_CMD_BTN_ADD);
		aControls.Add(IDC_CMD_BTN_INSERT);
		aControls.Add(IDC_CMD_BTN_MODIFY);
		aControls.Add(IDC_CMD_BTN_DELETE);
		aControls.Add(IDC_CMD_LIST);
		aControls.Add(IDC_CMD_MVHL_IRC_dD1_STATIC);
		aControls.Add(IDC_CMD_MVHL_IRC_dD1);
		aControls.Add(IDC_CMD_MVHL_IRC_dD1_UNIT);
		aControls.Add(IDC_CMD_MVHL_IRC_dD2_STATIC);
		aControls.Add(IDC_CMD_MVHL_IRC_dD2);
		aControls.Add(IDC_CMD_MVHL_IRC_dD2_UNIT);
		aControls.Add(IDC_CMD_LNLD_STATIC);
		aControls.Add(IDC_CMD_W_STATIC);
		aControls.Add(IDC_CMD_W);
		aControls.Add(IDC_CMD_W_UNIT);
		aControls.Add(IDC_CMD_PL_STATIC);
		aControls.Add(IDC_CMD_PL);
		aControls.Add(IDC_CMD_PL_UNIT);
		aControls.Add(IDC_CMD_PLM_STATIC);
		aControls.Add(IDC_CMD_PLM);
		aControls.Add(IDC_CMD_PLM_UNIT);
		aControls.Add(IDC_CMD_PLV_STATIC);
		aControls.Add(IDC_CMD_PLV);
		aControls.Add(IDC_CMD_PLV_UNIT);

		aControls2.Copy(aControls);

		aControls.Add(IDC_BITMAP_STATIC);

		if(GetTrkRadio() == 3) // AASHTO_LRFD & Legal/Permit
		{
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TYPE_RD)->GetWindowRect(rRef);
			GetDlgItem(IDC_BITMAP_STATIC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);  

			GetDlgItem(IDC_BITMAP_STATIC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistY(this, aControls2, nDistY);  
		}
		else if(GetTrkRadio() == 0) // AASHTO_LRFD & Truck
		{
			GetDlgItem(IDC_CMD_VEHICLE_NAME)->GetWindowRect(rRef);
			GetDlgItem(IDC_BITMAP_STATIC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);  

			GetDlgItem(IDC_BITMAP_STATIC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistY(this, aControls2, nDistY);  

		}    
		else // AASHTO_LRFD & Train
		{
			GetDlgItem(IDC_CMD_VEHICLE_NAME)->GetWindowRect(rRef);
			GetDlgItem(IDC_BITMAP_STATIC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);  

			aControls.RemoveAll();
			aControls.Add(IDC_CMD_DLGHOLDER);
			GetDlgItem(IDC_BITMAP_STATIC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);  
		}

		aControls.RemoveAll();
		aControls.Add(IDC_CMD_U_DIST_STATIC);
		aControls.Add(IDC_CMD_U_DIST_EDT);
		aControls.Add(IDC_CMD_U_DIST_UNIT);
		aControls.Add(IDC_CMD_MVHL_IMF_STATIC);
		aControls.Add(IDC_CMD_MVHL_IMP_EDT);
		aControls.Add(IDC_CMD_MVHL_IMP_UNIT);    
		aControls.Add(IDC_CMD_U_R_STATIC);    
		aControls.Add(IDC_CMD_U_R_EDT);    
		aControls.Add(IDC_CMD_U_R_UNIT);
		GetDlgItem(IDC_CMD_MVHL_IRC_dD1_STATIC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_U_DIST_STATIC)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		aControls.RemoveAll();
		aControls.Add(IDC_CMD_MVHL_USR_W_M_D_LANE_FRM);
		aControls.Add(IDC_CMD_U_W_STATIC);
		aControls.Add(IDC_CMD_U_W_EDT);
		aControls.Add(IDC_CMD_U_W_UNIT);
		GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_USR_W_M_D_LANE_FRM)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;  
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

		BOOL bLrfdCF = (GetTrkRadio() == 0 || GetTrkRadio() == 3)? TRUE : FALSE;

		// Group Box Size 수정
		if(GetTrkRadio() == 3)  // AASHTO_LRFD & Legal/Permit
		{
			GetDlgItem(IDC_CMD_U_R_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rToMove);      
			rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
			ScreenToClient(rToMove);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);
			
			GetDlgItem(IDC_CMD_U_R_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_USR_W_M_D_LANE_FRM)->GetWindowRect(rToMove);      
			rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
			ScreenToClient(rToMove);
			GetDlgItem(IDC_CMD_MVHL_USR_W_M_D_LANE_FRM)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);

			// Centrifugal force
			aControls.RemoveAll();
			aControls.Add(IDC_CMD_MVHL_ADD_CF_CHK);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

		}
		else if(GetTrkRadio() == 0)  // AASHTO_LRFD & Truck
		{
			GetDlgItem(IDC_CMD_MVHL_IRC_dD1)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rToMove);      
			rToMove.bottom =  rRef.top;  //rRef.bottom + 8;
			ScreenToClient(rToMove);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);

			GetDlgItem(IDC_CMD_MVHL_IRC_dD1)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_LNLD_STATIC)->GetWindowRect(rToMove);      
			rToMove.bottom = rRef.top;  //rRef.bottom + 8;
			ScreenToClient(rToMove);
			GetDlgItem(IDC_CMD_LNLD_STATIC)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);

			// Centrifugal force => Truck은 추후 포함될 가능성에 대비해 포함은 했지만 숨김.
			aControls.RemoveAll();
			aControls.Add(IDC_CMD_MVHL_ADD_CF_CHK);
			GetDlgItem(IDC_CMD_TRCKLD_STATIC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
			CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

			// Patch load
			CRect rRef2;
			GetDlgItem(IDC_CMD_BTN_DELETE)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rRef2);
			GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
			nDistX = rRef.left - rToMove.left;
			nDistY = rRef2.top - rToMove.top;
			CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPatch, nDistX, nDistY);
		}
		else // AASHTO_LRFD & Train
		{
			// 각 Page 를 이동한 Holder 에 맞게 새로 그림
			GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
			rToMove.bottom = rToMove.top + m_dHTrnDlg;
			ScreenToClient(rToMove);

			//m_pTrnDlg->GetWindowRect(rRef);
			if(m_pTrnDlg)
			{
				m_pTrnDlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rToMove.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
				m_pTrnDlg->GetWindowRect(rRef);
			}

			// Holder 변경 
			GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
			rToMove.bottom = rRef.bottom;
			ScreenToClient(rToMove);
			GetDlgItem(IDC_CMD_DLGHOLDER)->MoveWindow(rToMove);
		}

		if (m_bPatchLoad)
		{
			GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->GetWindowRect(rRef);
		}
		else if(bLrfdCF)
		{
			GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->GetWindowRect(rRef);
		}
		else
		{
			GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rRef);
		}
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rToMove);      
		rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
		ScreenToClient(rToMove);
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->SetWindowPos(NULL, rToMove.left, rToMove.top, rToMove.Width(), rToMove.Height(), SWP_SHOWWINDOW);
	}

	CArray<UINT, UINT> aOKCancel;          
	aOKCancel.Add(IDOK);
	aOKCancel.Add(IDCANCEL);
	aOKCancel.Add(IDC_CMD_APPLY);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aOKCancel, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

BOOL CCMMvhlItemUsrDlg::ApplyOrOK()
{
	T_MVHL_D data;
	BOOL bSuccess = FALSE;

	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return FALSE;
	if(MvcdD.nCodeType == D_MOVE_CODE_BS)
	{
		m_Data.bStandard = FALSE;
		m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);
		m_Data.nSubType = m_nBSType;
		if(m_nBSType==0)
		{
			if(m_pBS5400Dlg)
			{
				if(m_pBS5400Dlg->SetDlg2Data()) data = m_Data;
				else return FALSE;
			}
		}
		else if(m_nBSType==1)
		{
			if(m_pBSHADlg)
			{
				if(m_pBSHADlg->SetDlg2Data()) data = m_Data;
				else return FALSE;
			}
		}
		else if(m_nBSType==2)
		{
			if(m_pBSHBDlg)
			{
				if(m_pBSHBDlg->SetDlg2Data()) data = m_Data;
				else return FALSE;
			}
		}
		else if(m_nBSType==3)
		{
			if(m_pBSHAHBDlg)
			{
				if(m_pBSHAHBDlg->SetDlg2Data()) data = m_Data;
				else return FALSE;
			}
		}
		else if(m_nBSType==4)
		{
			if(m_pBSHAHBAUTODlg)
			{
				if(m_pBSHAHBAUTODlg->SetDlg2Data()) data = m_Data;
				else return FALSE;
			}
		}
		else if(m_nBSType==5)
		{
			if(m_pBSPedDlg)
			{
				if(m_pBSPedDlg->SetDlg2Data()) data = m_Data;
				else return FALSE;
			}
		}
		else if(m_nBSType==6)
		{
			if(m_pBSSpecialDlg)
			{
				if(m_pBSSpecialDlg->SetDlg2Data()) data = m_Data;
				else return FALSE;
			}
		}
		else ASSERT(0);
	}
	else
	{
	  if (!Dlg2Data()) return FALSE;
	  
	  data = m_Data;
	}

	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

int CCMMvhlItemUsrDlg::GetTruckLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUsrDlg::GetTruckDistanceCount()
{
	int i = 0;
	for (i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CCMMvhlItemUsrDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	if (!CFormulaEdit::GetEditValue(&m_wndPEdit, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_wndDEdit, dDist))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_distance_value_));
		return FALSE;
	}

	if (dLoad == 0.0) // Load 0은 허용 안함
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_value_must_not_be_ze));
		return FALSE;
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMvhlItemUsrDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUsrDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_INSERT, OnCmdBtnInsert)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemchangedCmdList)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_TRK_RD,                       OnCmdMlMvhlItemuTrkRd)  
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_U_TRUCK_COMBO,                      OnSelchangeChTruckCombo)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_U_TRAIN_COMBO,                      OnSelchangeChTrainCombo)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_U_CROWD_COMBO,                      OnSelchangeChCrowdCombo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_TRN_RD,                    OnCmdMlMvhlItemuTrkRd)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_CRD_RD,                    OnCmdMlMvhlItemuTrkRd)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_BS_RDO1,                   OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_BS_RDO2,                   OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_BS_RDO3,                   OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_BS_RDO4,                   OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_BS_RDO5,                   OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_BS_RDO6,                   OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_BS_RDO7,                   OnCmdMlMvhlItemuBSRdo)
	ON_BN_CLICKED   (IDC_CMD_ML_MVHL_ITEMU_LANE_RDO,                  OnCmdMlMvhlItemuTrkRd)
	ON_CBN_SELCHANGE(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_NUM_CMB, OnCmdMlMvhlItemuUniformLoadNumCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_AUTO_RDO,    OnCmdMlMvhlItemuDynaTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_USER_RDO,    OnCmdMlMvhlItemuDynaTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TRK_RD,                 OnCmdMlMvhlItemuTrkRd)  
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD,                     OnCmdMlMvhlItemuTrkRd)  
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TRN_RD,                 OnCmdMlMvhlItemuTrkRd)  
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LEGAL_CRD_RD,                 OnCmdMlMvhlItemuTrkRd)  
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TYPE_RD,                OnLegalPermitRd)  
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_PERMIT_TYPE_RD,               OnLegalPermitRd)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_HL_TYPE_RD,                   OnLegalPermitRd)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK,                             OnChkPatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrDlg message handlers

BOOL CCMMvhlItemUsrDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return TRUE;
	m_nMovingType = MvcdD.nCodeType;


	if(!m_bModify)
	{
		m_Data.Initialize();
		
		m_Data.nLoadType  = 0;
		m_Data.nTruckType = 0;
		m_Data.nTrainType = 0;
		
		SetTrkRadio(0);

		m_Data.dAddDataaL = M_InitValueCurUnit(3.65, N, M, D_UNITSYS_BASE_LENGTH);

		if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
		{
			m_Data.dLegalR = 75.0;
			m_Data.dCombinedTruckSpace = M_InitValueCurUnit(30, KIP, FT, D_UNITSYS_BASE_LENGTH);
			m_Data.dCombinedUniformLoad = M_InitValueCurUnit(0.2, KIP, FT, D_UNITSYS_BASE_LENGTH);

			T_MVHL_PATCH_ITEM Patch;
			Patch.dWidth  = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, 20.0);
			Patch.dLength = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, 10.0);
			m_Data.PatchLoad.aPatchLoad.RemoveAll();
			m_Data.PatchLoad.aPatchLoad.Add(Patch);
			m_Data.PatchLoad.bPatchLoad = FALSE;
		}
	}
	else
	{
		if (m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
		{
			if (m_Data.PatchLoad.aPatchLoad.GetSize() == 0)
			{
				T_MVHL_PATCH_ITEM Patch;
				Patch.dWidth  = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, 20.0);
				Patch.dLength = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KIP, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, 10.0);
				m_Data.PatchLoad.aPatchLoad.RemoveAll();
				m_Data.PatchLoad.aPatchLoad.Add(Patch);
				m_Data.PatchLoad.bPatchLoad = FALSE;
			}
		}
	}
	
	m_bPatchLoad = FALSE;
	if (m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		if (GetTrkRadio() == 0)
		{
			m_bPatchLoad = TRUE;
		}
	}
	AlignControl();		  // 코드타입에 따라 프레임 정렬
	ShowHideCtrl();

	if (m_nMovingType == D_MOVE_CODE_AASHTO_LRFD || m_nMovingType == D_MOVE_CODE_CANADA || m_nMovingType == D_MOVE_CODE_INDIA)
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_CRD_RD)->SetWindowText(_LS(IDS_CMD_MVHL_PERMIT_TRUCK));
	}

	if(m_nMovingType == D_MOVE_CODE_BS)
	{
		CreateBSLoadDlg();
		Data2Dlg();
		CtrlBSLoadDlg();
		LoadBSBitmap();
		goto GT_BS_RETURN;
	}
		
	SetInitChTruckCombo();
	SetInitChTrainCombo();
	SetInitChCrowdCombo();
	SetInitKoreaLaneCombo();

	ChangeBitMap(0);  // 먼저 한번 호출

	SetHeaderTitle();   // header title
	CreateTranLdDlg();  // child dialog를 만든다. 	
	Data2Dlg();	        // truck , lane load인 경우 

	UpdateData(FALSE);
	OnCmdMlMvhlItemuTrkRd();   
	OnCmdMlMvhlItemuUniformLoadNumCmb(); 
	OnCmdMlMvhlItemuDynaTypeRdo();
	SetUnit();

GT_BS_RETURN:
	if(m_bModify)	m_wndBtnApply.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUsrDlg::SetUnit()
{
	// 단위변환
	m_stWUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoad);
	m_stPLVUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPLV);
	m_stPLMUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPLM);
	m_stPLUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPL);
	m_stPUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dP);
	m_stQmUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dQm);
	m_stQqUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dQq);
	m_wndDCrawlerUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDTrailer);
	m_wndWCrawlerUnit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWTrailer);

	m_stIrcNtDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dNoseTailDist);
	m_stIrcD1.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_stIrcD2.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_stIrcP.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dP);
	m_stIrcD.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDTrailer);
	m_stIrcPb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_stIrcDb.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dDb);

	m_untCrowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWCrowd);

	m_untQkLane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dQq);
	m_untPk1Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPa);
	m_untPk2Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dPb);
	m_untL1Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_untL2Lane.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);
	m_untW1Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWPres1);
	m_untW2Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dWPres2);
	m_untL1Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD1);
	m_untL2Crowd.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dD2);

	// Korea - Lane(Length Specifired (탱크 하중))
	m_untUniformLoadDistance.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoadDistance);
	m_untUniformLoadW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoadW);
	m_untUniformLoadLengthL.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoadLengthL);

	m_edt1Axle.SetUnitType(D_UNITSYS_NONE);
	m_edt2Axle.SetUnitType(D_UNITSYS_NONE);
	m_edt3Axle.SetUnitType(D_UNITSYS_NONE);
	
	m_edtImpactFactor.SetUnitType(D_UNITSYS_BASE_PERCENT);
	m_untImpactFactor.SetUnitType(D_UNITSYS_BASE_PERCENT);
	m_edtW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_untW.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedUniformLoad);
	m_edtR.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
	m_untR.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dLegalR);
	m_edtDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedTruckSpace);
	m_untDist.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dCombinedTruckSpace);
}

void CCMMvhlItemUsrDlg::SetParamData(T_MVHL_D &ParamData) 
{ 
	m_Data.Initialize();

	m_csOldName = ParamData.VehicleLoadName; 

	m_bModify = TRUE;

	if(ParamData.nLoadType==0)       // Truck/Lane
	{
		m_Data    = ParamData;	
		SetTrkRadio(m_Data.nLoadType);
		m_nBSType = m_Data.nSubType;
	}
	else if(ParamData.nLoadType==1)  // Train
	{
		m_Data = ParamData;  
		SetTrkRadio(m_Data.nLoadType);
	}   
	else if(ParamData.nLoadType==2)  // Crowd(Type1)
	{
		m_Data    = ParamData;	
		SetTrkRadio(m_Data.nLoadType);
	}
	else if(ParamData.nLoadType==3)  // Crowd(Type2)
	{
		m_Data    = ParamData;	
		SetTrkRadio(m_Data.nLoadType - 1);
	}
	else if(ParamData.nLoadType==6) // Korea - Lane(Length Specifired (탱크 하중))
	{
		m_Data = ParamData;
		SetTrkRadio(m_Data.nLoadType - 3);
	}
	else if(ParamData.nLoadType==9) // Legal Load
	{
		m_Data = ParamData;
		SetTrkRadio(3);
	}
}

void CCMMvhlItemUsrDlg::ResizeBSDialogByFoldGroupBox() 
{
	CtrlBSLoadDlg();
}

void CCMMvhlItemUsrDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvhlItemUsrDlg::OnCmdApply() 
{
	ApplyOrOK();
}

void CCMMvhlItemUsrDlg::OnCmdBtnAdd() 
{
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	
	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	if (nLoadCount > nDistCount) // Last 입력된 경우 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Delete_last_item_and_try_));
		return;
	}

	if (dDist == 0.0) // Last
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_Data.dPointLoad[nLoadCount] = dLoad;
		m_Data.dPointDistance[nLoadCount] = dDist;
	}

	int no;
	no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist);

	int nCount = m_List.GetItemCount();
	m_List.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrDlg::OnCmdBtnInsert() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}
	
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MVHL_NUMLOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MVHL_NUMLOAD);
		AfxMessageBox(csErr);
		return;
	}
	
	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	// 데이타 이동하기
	for (int i = D_MVHL_NUMLOAD-1; i > iItem; i--)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i-1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i-1];
	}

	// 데이타 삽입한다.
	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;

	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_List.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUsrDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}
	
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (iItem != nLoadCount-1 && dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	m_Data.dPointLoad[iItem] = dLoad;
	m_Data.dPointDistance[iItem] = dDist;

	ModifyItem(iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUsrDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	int i = 0;
	for (i = iItem; i < D_MVHL_NUMLOAD-1; i++)
	{
		m_Data.dPointLoad[i] = m_Data.dPointLoad[i+1];
		m_Data.dPointDistance[i] = m_Data.dPointDistance[i+1];
	}
	m_Data.dPointLoad[i] = 0.0;
	m_Data.dPointDistance[i] = 0.0;

	m_List.DeleteItem(iItem);
	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_List.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUsrDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_wndPEdit.SetWindowText(csVal);
		m_wndDEdit.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data.dPointLoad[iItem]);
		m_wndPEdit.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data.dPointDistance[iItem]);
		m_wndDEdit.SetWindowText(csVal);
	}
	
	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////
// DATE : 2002.1.15. by KYE-HONG
// FUNC : train load type인 경우 data exchange
////////////////////////////////////////////////////////////////////////////
void CCMMvhlItemUsrDlg::Data2DlgOfTrnLd()
{
	if(m_pTrnDlg)m_pTrnDlg->SetData2Dlg();
}

BOOL CCMMvhlItemUsrDlg::Dlg2DataOfTrnLd()
{
	if(m_pTrnDlg)
	{
		m_pTrnDlg->m_nTrainType = m_Data.nTrainType;
		m_pTrnDlg->SetDlg2Data();
	}
	return TRUE; 
}

///////////////////////////////////////////////////////////////////////////////////
// v6.7.1 부터 적용......china code 일때만....
void CCMMvhlItemUsrDlg::Data2DlgOfTrainLd()
{
	if(m_pTrainDlg)		m_pTrainDlg->SetData2Dlg();
}

BOOL CCMMvhlItemUsrDlg::Dlg2DataOfTrainLd()
{
	if(m_pTrainDlg)   m_pTrainDlg->SetDlg2Data();
	return TRUE; 
}
///////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// DATE : 2002.1.15. by KYE-HONG
// FUNC : train load dialog의 생성.. 삭제.. 
//        
//				
////////////////////////////////////////////////////////////////////////////
BOOL CCMMvhlItemUsrDlg::CreateTranLdDlg(void)
{
	CRect rc;

	m_pTrnDlg	= new	CCMMvhlItemUsrTrckDlg(&m_Data);
	ASSERT(m_pTrnDlg);	
	if(!m_pTrnDlg) return FALSE;

	// holder
	CWnd *cwnd = (CWnd *)GetDlgItem(IDC_CMD_DLGHOLDER);	
	if(!cwnd)	return FALSE;
	cwnd->GetWindowRect(&rc);		// holder 위치.. 
	this->ScreenToClient(&rc);	// 현재 대화상자 기준좌표. 

	// create dlg
	if(m_pTrnDlg->GetSafeHwnd()==0)
	{
		m_pTrnDlg->Create(IDD_CMD_ML_MVHL_ITEMU_TRAIN,this);
		m_pTrnDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);

		m_dHTrnDlg =m_pTrnDlg->GetDlgH();
	}

	////////////////////////////////////////////////////////////////////
	// Trrain Load for China.....by HP...from v6.7.1부터...
	m_pTrainDlg	= new	CCMMvhlItemUsrTrainDlg(&m_Data);
	ASSERT(m_pTrainDlg);	
	if(!m_pTrainDlg) return FALSE;

	// holder
	CWnd *cwnd2 = (CWnd *)GetDlgItem(IDC_CMD_DLGHOLDER);	
	if(!cwnd2)	return FALSE;
	cwnd2->GetWindowRect(&rc);		// holder 위치.. 
	this->ScreenToClient(&rc);	// 현재 대화상자 기준좌표. 

	// create dlg
	if(m_pTrainDlg->GetSafeHwnd()==0)
	{
		m_pTrainDlg->Create(IDD_CMD_ML_MVHL_ITEM_TRAIN_CH,this);
		m_pTrainDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	/////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CCMMvhlItemUsrDlg::DestroyTranLdDlg(void)
{
	if(m_pTrnDlg)
	{
		m_pTrnDlg->SendMessage(WM_DESTROY,0,0);
		delete m_pTrnDlg;
		m_pTrnDlg = NULL;
	}
	////////////////////////////////////////////////////////////////////
	// Trrain Load for China.....by HP...from v6.7.1부터...
	if(m_pTrainDlg)
	{
		m_pTrainDlg->SendMessage(WM_DESTROY,0,0);
		delete m_pTrainDlg;
		m_pTrainDlg = NULL;
	}
	////////////////////////////////////////////////////////////////////
}

void CCMMvhlItemUsrDlg::OnDestroy() 
{
	CDialogMove::OnDestroy();

	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;
	if(MvcdD.nCodeType == D_MOVE_CODE_BS)
	{
		DestroyBSLoadDlg();	
	}
	else
	{
		DestroyTranLdDlg();	
	}
	
}

void CCMMvhlItemUsrDlg::OnLegalPermitRd() 
{
	EnableLegalPermitRadio();
	ChangeBitMap(GetTrkRadio());
}

void CCMMvhlItemUsrDlg::OnCmdMlMvhlItemuTrkRd() 
{
	UpdateData();
	
	m_bPatchLoad = FALSE;
	if (m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		if (GetTrkRadio() == 0)
		{
			m_bPatchLoad = TRUE;
		}
	}

	//m_wndVehicleName
	if ((m_nMovingType == D_MOVE_CODE_AASHTO_LRFD || m_nMovingType == D_MOVE_CODE_CANADA || m_nMovingType == D_MOVE_CODE_INDIA) && GetTrkRadio() == 2)
	{
		if (m_bPermitLoadCall)
		{
			m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);
			m_bPermitLoadCall = FALSE;
			CCMMvhlItemUsrPermitDlg dlg;
			dlg.SetInitPos(D_INIT_POS_LT);
			CString csRadioText[3];
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->GetWindowText(csRadioText[0]);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->GetWindowText(csRadioText[1]);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_CRD_RD)->GetWindowText(csRadioText[2]);
			dlg.SetModifyData(m_bModify, m_csOldName, &m_Data, m_pDoc, csRadioText, m_nMovingType);
			dlg.DoModal();
			//Type Change이면
			int nLastEvent = dlg.GetNextAction();
			if (nLastEvent == CCMMvhlItemUsrPermitDlg::CHANGE_TYPE)
			{
				CRect r = dlg.GetLastWindowPos();
				CRect rOrg;
				GetWindowRect(rOrg);
				r.right = r.left + rOrg.Width();
				r.bottom = r.top + rOrg.Height();
				MoveWindow(r);
				ShowWindow(SW_SHOW);
				m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);

				SetTrkRadio(dlg.GetType());
				UpdateData(FALSE);
				OnCmdMlMvhlItemuTrkRd();
			}
			else
			{
				CDialogMove::OnOK();
			}
		}
		else
		{
			m_bPermitLoadCall = TRUE;
			OnCmdMlMvhlItemuTrkRd();
		}
	}
	else
	{
		ShowChildDlg(GetTrkRadio());  
		ChangeBitMap(GetTrkRadio());
		EnableChinaRadio(GetTrkRadio());
	}

	if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		AlignControlDynamic();
	}  
}

void CCMMvhlItemUsrDlg::OnCmdMlMvhlItemuBSRdo()
{
	UpdateData(TRUE);

	if(m_nBSType==6)
	{
		if (m_bPermitLoadCall)
		{
			m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);
			m_bPermitLoadCall = FALSE;
			CCMMvhlItemUsrPermitDlg dlg;
			dlg.SetInitPos(D_INIT_POS_LT);
			CString csRadioText[3];
			dlg.SetModifyData(m_bModify, m_csOldName, &m_Data, m_pDoc, csRadioText, m_nMovingType);
			dlg.DoModal();
			//Type Change이면
			int nLastEvent = dlg.GetNextAction();
			if (nLastEvent == CCMMvhlItemUsrPermitDlg::CHANGE_TYPE)
			{
				CRect r = dlg.GetLastWindowPos();
				CRect rOrg;
				GetWindowRect(rOrg);
				r.right = r.left + rOrg.Width();
				r.bottom = r.top + rOrg.Height();
				MoveWindow(r);
				ShowWindow(SW_SHOW);
				m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
				m_nBSType = dlg.GetBSType();
				PostMessage(WM_COMMAND, MAKEWPARAM(0, BN_CLICKED), 0);

				ChangeBSDlg();
			}
			else
			{
				CDialogMove::OnOK();
			}
		}
		else
		{
			m_bPermitLoadCall = TRUE;
			PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_ML_MVHL_ITEMU_BS_RDO1, BN_CLICKED), 0);
		}
	}
	else
	{
		ChangeBSDlg();
	}
	
	UpdateData(FALSE);
}

void CCMMvhlItemUsrDlg::ChangeBSDlg()
{
	if(!m_bModify)
	{
		// Default 를 받아오기 위한 처리
		m_Data.bStandard = TRUE;
		if     (m_nBSType==0) { m_Data.nStandardCode = D_MVHL_BS_5400;  m_Data.VehicleTypeName = _T("HA & HB (BS5400)"); }
		else if(m_nBSType==1) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HA"); }
		else if(m_nBSType==2) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HB"); }
		else if(m_nBSType==3) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HA & HB"); }
		else if(m_nBSType==4) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("HA & HB(AUTO)"); }
		else if(m_nBSType==5) { m_Data.nStandardCode = D_MVHL_BS_BD3701;  m_Data.VehicleTypeName = _T("PEDESTRIAN"); }
		else if(m_nBSType==6) { } // Nothing
		else ASSERT(0);

		CVehlDB db(m_pDoc);
		db.GetStandardVehicleLoadValue(m_Data);

		m_Data.bStandard = FALSE;
		m_Data.nStandardCode = 0;
		m_Data.VehicleTypeName = _T("");
	}

	if(m_nBSType==0)
	{
		if(m_pBS5400Dlg) m_pBS5400Dlg->SetData2Dlg();
	}
	else if(m_nBSType==1)
	{
		if(m_pBSHADlg) m_pBSHADlg->SetData2Dlg();
	}
	else if(m_nBSType==2)
	{
		if(m_pBSHBDlg) m_pBSHBDlg->SetData2Dlg();
	}
	else if(m_nBSType==3)
	{
		if(m_pBSHAHBDlg) m_pBSHAHBDlg->SetData2Dlg();
	}
	else if(m_nBSType==4)
	{
		if(m_pBSHAHBAUTODlg) m_pBSHAHBAUTODlg->SetData2Dlg();
	}
	else if(m_nBSType==5)
	{
		if(m_pBSPedDlg) m_pBSPedDlg->SetData2Dlg();
	}
	else if(m_nBSType==6)
	{
		if(m_pBSSpecialDlg) m_pBSSpecialDlg->SetData2Dlg();
	}
	else ASSERT(0);

	if(m_pBS5400Dlg)     m_pBS5400Dlg    ->ShowWindow(m_nBSType==0);
	if(m_pBSHADlg)       m_pBSHADlg      ->ShowWindow(m_nBSType==1);
	if(m_pBSHBDlg)       m_pBSHBDlg      ->ShowWindow(m_nBSType==2);
	if(m_pBSHAHBDlg)     m_pBSHAHBDlg    ->ShowWindow(m_nBSType==3);
	if(m_pBSHAHBAUTODlg) m_pBSHAHBAUTODlg->ShowWindow(m_nBSType==4);
	if(m_pBSPedDlg)      m_pBSPedDlg     ->ShowWindow(m_nBSType==5);
	//if(m_pBSSpecialDlg)  m_pBSSpecialDlg ->ShowWindow(m_nBSType==6);

	ResizeBSDialog();
	LoadBSBitmap();
}

void CCMMvhlItemUsrDlg::ResizeBSDialog()
{
	CRect rRef;
	CRect rToMove;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;

	// Load Type Group Box
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO7)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_TYPE_GRP)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_TYPE_GRP)->MoveWindow(rToMove);

	// Vehiculer Load Properties Move~~
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES);
	aControls.Add(IDC_CMD_VEHICLE_NAME_STC);
	aControls.Add(IDC_CMD_VEHICLE_NAME);
	aControls.Add(IDC_BITMAP_STATIC2);
	
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// Holder 를 그림 밑으로 옮긴다.
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_DLGHOLDER);

	GetDlgItem(IDC_BITMAP_STATIC2)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	// 각 Page 를 이동한 Holder 에 맞게 새로 그리고..
	GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
	ScreenToClient(rToMove);

	if(m_nBSType == 0) 
	{
		m_pBS5400Dlg->GetWindowRect(rRef);
		m_pBS5400Dlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rRef.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		m_pBS5400Dlg->GetWindowRect(rRef);
	}
	else if(m_nBSType == 1) 
	{
		m_pBSHADlg->GetWindowRect(rRef);
		m_pBSHADlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rRef.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		m_pBSHADlg->GetWindowRect(rRef);
	}
	else if(m_nBSType == 2) 
	{
		m_pBSHBDlg->GetWindowRect(rRef);
		m_pBSHBDlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rRef.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		m_pBSHBDlg->GetWindowRect(rRef);
	}
	else if(m_nBSType == 3) 
	{
		m_pBSHAHBDlg->GetWindowRect(rRef);
		m_pBSHAHBDlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rRef.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		m_pBSHAHBDlg->GetWindowRect(rRef);
	}
	else if(m_nBSType == 4) 
	{
		m_pBSHAHBAUTODlg->GetWindowRect(rRef);
		m_pBSHAHBAUTODlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rRef.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		m_pBSHAHBAUTODlg->GetWindowRect(rRef);
	}
	else if(m_nBSType == 5) 
	{
		m_pBSPedDlg->GetWindowRect(rRef);
		m_pBSPedDlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rRef.Height(),SWP_NOZORDER|SWP_NOACTIVATE);	
		m_pBSPedDlg->GetWindowRect(rRef);
	}
	else if(m_nBSType == 6) 
	{
		m_pBSSpecialDlg->GetWindowRect(rRef);
		m_pBSSpecialDlg->SetWindowPos(NULL,rToMove.left,rToMove.top,rToMove.Width(),rRef.Height(),SWP_NOZORDER|SWP_NOACTIVATE);	
		m_pBSSpecialDlg->GetWindowRect(rRef);
	}
	else ASSERT(0);

	// Holder 변경 
	GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom;
	ScreenToClient(rToMove);
	GetDlgItem(IDC_CMD_DLGHOLDER)->MoveWindow(rToMove);
	
	// Vehicular Load Properties
	GetDlgItem(IDC_CMD_DLGHOLDER)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->MoveWindow(rToMove);

	// OK, CANCEL, APPLY
	CArray<UINT, UINT> aOKCancel;          
	aOKCancel.Add(IDOK);
	aOKCancel.Add(IDCANCEL);
	aOKCancel.Add(IDC_CMD_APPLY);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aOKCancel, nDistY);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CCMMvhlItemUsrDlg::OnCmdMlMvhlItemuUniformLoadNumCmb()
{
	UpdateData(TRUE);

	int nIndex = 0;
	nIndex = m_cmbUniformLoadNum.GetCurSel();

	if(nIndex == 0) // Load Num = 1
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_STC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_EDT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_UNT)->EnableWindow(FALSE);
	}
	else // Load Num = 2~10
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_STC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_UNT)->EnableWindow(TRUE);
	}

	ChangeBitMap(GetTrkRadio());
}

void CCMMvhlItemUsrDlg::OnCmdMlMvhlItemuDynaTypeRdo()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_DYNA_1AXLE_EDT)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_1AXLE_STC)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_2AXLE_EDT)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_2AXLE_STC)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_3AXLE_EDT)->EnableWindow(m_nDynaType == 1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_3AXLE_STC)->EnableWindow(m_nDynaType == 1);
}

void CCMMvhlItemUsrDlg::OnChkPatch()
{
	BOOL bPatchLoad = m_chkPatch.GetCheck();

	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(bPatchLoad);
}

void CCMMvhlItemUsrDlg::ShowHideCtrl()
{
	UpdateData(TRUE);
	int nTruckIndex = m_cmbChTruck.GetItemData(m_cmbChTruck.GetCurSel());

	int nTrkRdo = GetTrkRadio();

	// LIST 보이지 않는 경우 설정
	BOOL bShowList = FALSE;
	if((m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0) ||   // China Truck-Lane1 
		(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 6) ||   // China Truck(JTG B01 -2003) 
		(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 2) ||   // China Truck(GC Type Load) 
		(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && nTrkRdo == 0) ||   // KOREA       
		(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0)                     ||   // India
		(m_nMovingType == D_MOVE_CODE_CANADA && nTrkRdo == 0)                    ||      // Canada
		(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD && (nTrkRdo == 0 || nTrkRdo == 3)))                         // AASHTO_LRFD
		bShowList = TRUE;

	int nCrowdIndex = m_cmbChCrowd.GetItemData(m_cmbChCrowd.GetCurSel());

	// 기존 m_aCtrlCom1  
	GetDlgItem(IDC_CMD_LNLD_STATIC)->ShowWindow((m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0) || m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_PL_STATIC)->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PLM_STATIC)->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PLV_STATIC )->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PL)->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PLM)->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PLV  )->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PL_UNIT)->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PLM_UNIT)->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_PLV_UNIT)->ShowWindow(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA && m_nMovingType != D_MOVE_CODE_CANADA && m_nMovingType != D_MOVE_CODE_BS && nTrkRdo == 0);

	// 기존 m_aCtrlCom2  
	GetDlgItem(IDC_CMD_W_STATIC)->ShowWindow((m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA) && nTrkRdo == 0 && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_W)->ShowWindow((m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA) && nTrkRdo == 0 && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_W_UNIT)->ShowWindow((m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA) && nTrkRdo == 0 && m_nMovingType != D_MOVE_CODE_BS);

	// 기존 m_aCtrlChOnly(중국에서만 쓰는 컨트럴)
	GetDlgItem(IDC_CMD_MVHL_U_TRUCK_COMBO)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA);
	GetDlgItem(IDC_CMD_MVHL_U_TRAIN_COMBO)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA);
	GetDlgItem(IDC_CMD_MVHL_U_CROWD_COMBO)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA);  

	// 기존 m_aCtrl3rdOpt
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_CRD_RD)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA || m_nMovingType == D_MOVE_CODE_CANADA || m_nMovingType == D_MOVE_CODE_INDIA);  

	// 기존 m_aCtrlChLane1
	GetDlgItem(IDC_CMD_MVHL_USR_LANE_FRM)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_P_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_Qm_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_Qq_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_P)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_Qm)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_Qq)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_P_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_Qm_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);
	GetDlgItem(IDC_CMD_U_Qq_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 0);

	// 기존 m_aCtrlChLane2
	GetDlgItem(IDC_CMD_MVHL_USR_LANE2_FRM )->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_QK_EDIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_PK1_EDIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_PK2_EDIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_L1_EDIT3)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_L2_EDIT3)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_QK_UNIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_PK1_UNIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_PK2_UNIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_L1_UNIT3)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_L2_UNIT3)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_QK_STATIC2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_PK1_STATIC2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_PK2_STATIC2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_L1_STATIC3)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  
	GetDlgItem(IDC_CMD_MVHL_CH_L2_STATIC3)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 5);  

	// 기존 m_aCtrlChCrawl
	GetDlgItem(IDC_CMD_MVHL_USR_CRAWL_FRM)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 1);  
	GetDlgItem(IDC_CMD_W_CRAWLER_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 1);  
	GetDlgItem(IDC_CMD_W_CRAWLER_EDIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 1);  
	GetDlgItem(IDC_CMD_W_CRAWLER_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 1);  
	GetDlgItem(IDC_CMD_D_CRAWLER_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 1);  
	GetDlgItem(IDC_CMD_D_CRAWLER_EDIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 1);  
	GetDlgItem(IDC_CMD_D_CRAWLER_UNIT  )->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 0 && nTruckIndex == 1);  

	// 기존 m_aCtrlChCrowd
	GetDlgItem(IDC_CMD_MVHL_USR_CROWD_FRM)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2);  
	GetDlgItem(IDC_CMD_W_CROWD_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 2);  
	GetDlgItem(IDC_CMD_W_CROWD_EDIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 2);  
	GetDlgItem(IDC_CMD_W_CROWD_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 2);  
	GetDlgItem(IDC_CMD_MVHL_CH_W1_EDIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_W2_EDIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_L1_EDIT4)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_L2_EDIT4)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_W1_UNIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_W2_UNIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_L1_UNIT4)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_L2_UNIT4)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_W1_STATIC2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_W2_STATIC2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_L1_STATIC4)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_L2_STATIC4)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_WD_STATIC2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_WD_EDIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);  
	GetDlgItem(IDC_CMD_MVHL_CH_WD_UNIT2)->ShowWindow(m_nMovingType == D_MOVE_CODE_CHINA && nTrkRdo == 2 && nCrowdIndex == 3);

	// 기존 m_aCtrlIrc
	GetDlgItem(IDC_CMD_IRC_TRACK_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_P_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_P)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_P_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_D_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_D)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_D_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_IRC_BOGIE_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_PB_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_PB)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_PB_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_DB_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_DB)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  
	GetDlgItem(IDC_CMD_MVHL_IRC_DB_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);  

	// 기존 m_aCtrlIrc1
	GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_MVHL_IRC_NTDIST_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);

	// 기존 m_aCtrlIrc2
	GetDlgItem(IDC_CMD_MVHL_IRC_dD1_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_MVHL_IRC_dD1)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_MVHL_IRC_dD1_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_MVHL_IRC_dD2_STATIC)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_MVHL_IRC_dD2)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);
	GetDlgItem(IDC_CMD_MVHL_IRC_dD2_UNIT)->ShowWindow(m_nMovingType == D_MOVE_CODE_INDIA && nTrkRdo == 0);

	// 기존 m_aCtrlKoreaLane
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_GRP)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_NUM_STC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_NUM_CMB)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_STC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_EDT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_DISTANCE_UNT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_STC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_EDT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_W_UNT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_STC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_EDT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_UNIFORM_LOAD_LENGTH_L_UNT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_KOREA);

	// 기존 m_aCtrlKoreaLaneRdo
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LANE_RDO)->ShowWindow(m_nMovingType == D_MOVE_CODE_KOREA);

	// 기존 m_aDyna
	GetDlgItem(IDC_CMD_MVHL_DYNA_GRP)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_AUTO_RDO)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_USER_RDO)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_1AXLE_EDT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_1AXLE_STC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_2AXLE_EDT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_2AXLE_STC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_3AXLE_EDT)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);
	GetDlgItem(IDC_CMD_MVHL_DYNA_3AXLE_STC)->ShowWindow(m_nMovingType == D_MOVE_CODE_CANADA);

	//  CDlgUtil::CtrlShowHide(this, m_aCtrlImpactFactor, m_nMovingType == D_MOVE_CODE_AASHTO_LRFD && nTrkRdo == 3);
	//  CDlgUtil::CtrlShowHide(this, m_aCtrlLegalLaneLoad, m_nMovingType == D_MOVE_CODE_AASHTO_LRFD && nTrkRdo == 3);

	// 기존 m_aCtrlList
	GetDlgItem(IDC_CMD_TRCKLD_STATIC)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_P_STATIC)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_D_STATIC)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_P)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_D)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_BTN_ADD)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_BTN_INSERT)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_BTN_MODIFY)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_BTN_DELETE)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_LIST)->ShowWindow(bShowList && m_nMovingType != D_MOVE_CODE_BS);
	
	// BS관련
	GetDlgItem(IDC_BITMAP_STATIC)->ShowWindow(m_nMovingType != D_MOVE_CODE_BS);
	GetDlgItem(IDC_BITMAP_STATIC2)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO1)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO2)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO3)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO4)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO5)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO6)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_BS_RDO7)->ShowWindow(m_nMovingType == D_MOVE_CODE_BS);

	// 기존 m_aCtrlBS
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->ShowWindow(m_nMovingType != D_MOVE_CODE_BS && m_nMovingType != D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->ShowWindow(m_nMovingType != D_MOVE_CODE_BS && m_nMovingType != D_MOVE_CODE_AASHTO_LRFD);
	
	//Legal Load 관련
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LEGAL_LOAD_TYPE_GRP)->ShowWindow(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TRK_RD)->ShowWindow(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_RD)->ShowWindow(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TRN_RD)->ShowWindow(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_CRD_RD)->ShowWindow(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);

	GetDlgItem(IDC_CMD_MVHL_USR_W_M_D_LANE_FRM)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_W_STATIC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_W_EDT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_W_UNIT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_R_STATIC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_R_EDT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_R_UNIT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	
	GetDlgItem(IDC_CMD_MVHL_IMF_STATIC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_MVHL_IMP_EDT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_MVHL_IMP_UNIT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);  
	GetDlgItem(IDC_CMD_U_DIST_STATIC)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_DIST_EDT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_U_DIST_UNIT)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LEGAL_TYPE_RD)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_PERMIT_TYPE_RD)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_HL_TYPE_RD)->ShowWindow(nTrkRdo == 3 && m_nMovingType == D_MOVE_CODE_AASHTO_LRFD);

	// 그외
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_TYPE_GRP)->ShowWindow(m_nMovingType != D_MOVE_CODE_AASHTO_LRFD);    

	// Centrifugal Force
	BOOL bCenF =FALSE;
	if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		if(nTrkRdo == 0 || nTrkRdo == 3) bCenF = TRUE;
	}
	else if (m_nMovingType == D_MOVE_CODE_CANADA)
	{
		if (nTrkRdo == 0) bCenF = TRUE;
	}
	GetDlgItem(IDC_CMD_MVHL_ADD_CF_CHK)->ShowWindow(bCenF);

	CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, m_bPatchLoad);

}

////////////////////////////////////////////////////////////////////////////
// DATE : 2002.1.15. by KYE-HONG
// FUNC : truck,lane load는 child dlg로 만들지 않았다.. 
//        기존에 만들어진 데이타를 그대로 이용하기 위해 
//				control들만 show혹은 hide시킨다. 
////////////////////////////////////////////////////////////////////////////
void	CCMMvhlItemUsrDlg::ShowChildDlg(int which)
{	
	ShowHideCtrl();
			
	int nTrkRdo = GetTrkRadio();
	m_pTrainDlg->ShowWindow(FALSE);		// Added by HP.....
	m_pTrnDlg->ShowWindow(FALSE);
	if(m_nMovingType == D_MOVE_CODE_CHINA)  // China
	{
		if(nTrkRdo==0)      OnSelchangeChTruckCombo();
		else if(nTrkRdo==1) OnSelchangeChTrainCombo();
		else if(nTrkRdo==2) OnSelchangeChCrowdCombo();
	}

	// From v6.7.1....By HP......
	int nIndex = m_cmbChTrain.GetItemData(m_cmbChTrain.GetCurSel());		
	if(nTrkRdo==1&&nIndex!=2&&m_pTrnDlg)
	{
		UpdateData(TRUE);
		m_pTrnDlg->m_bChSubway = (nIndex==4); // Subway
		m_pTrnDlg->ShowWindow(nTrkRdo==1);
	}	
	
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->EnableWindow(m_nMovingType != D_MOVE_CODE_CANADA);

	Invalidate(FALSE);
}

void CCMMvhlItemUsrDlg::ChangeBitMap(int which)
{	
	CString aBitmapID[]   = { _T("cmd_mvhc.svg"),  _T("cmd_mvhc2.svg"), _T("cmd_mvhc8-kr.svg"), _T("cmd_mvhc9-kr.svg"), 
							_T("cmd_mvhc_legal_permit.svg"), _T("cmd_mvhc_legal_permit2.svg"), _T("cmd_mvhl_285.svg")};
	CString aBitmapIDid[] = { _T("cmd_mvhc3.svg"), _T("cmd_mvhc1.svg")};
	CString aBitmapIDca[] = { _T("cmd_mvhc4.svg")};
	CString aBitmapIDch[] = { _T("cmd_mvhc_ch.svg"),  _T("cmd_mvhc6_ch.svg"), _T("cmd_mvhc7_ch.svg"), _T("cmd_mvhc2_ch.svg"), 
							_T("cmd_mvhc3_ch.svg"), _T("cmd_mvhc4_ch.svg"), _T("cmd_mvhc5_ch.svg"), _T("cmd_mvhl_china.svg"), 
							_T("cmd_mvhc_ch_truck_only.svg")};
	
	UpdateData(TRUE);
	int nBitmap = 0;   
	int nKoreaLaneCombo = m_cmbUniformLoadNum.GetCurSel();
	if(m_nMovingType == D_MOVE_CODE_CHINA)  // China
	{    
		switch(which)
		{
			case 0: 
				{
					int nIndex = m_cmbChTruck.GetItemData(m_cmbChTruck.GetCurSel());
					if(nIndex == 0) nBitmap = 0;
					else if(nIndex == 6) nBitmap = 8;
					else if(nIndex == 2) nBitmap = 8;
					else if(nIndex == 1) nBitmap = 2;
					else if(nIndex == 5) nBitmap = 1;
				}
				break;
			case 1:  // Train/Exceptional Vehicle
				{
					int nIndex = m_cmbChTrain.GetItemData(m_cmbChTrain.GetCurSel());
					if(nIndex == 1) nBitmap = 3;
					//else if(nIndex == 2) nBitmap = 3;
					else if(nIndex == 2) nBitmap = 7;   // modified by HP...
					else if(nIndex == 4) nBitmap = 5;
					else if(nIndex == 5) nBitmap = 6;          
				}
				break;
			case 2: nBitmap = 4; break;
		}
	}
	else if(m_nMovingType == D_MOVE_CODE_INDIA)  // India
	{
		switch(which)
		{
			case 0: nBitmap = 0; break;
			case 1: nBitmap = 1; break;
		}
	}
	else if(m_nMovingType == D_MOVE_CODE_CANADA)  // Canada
	{
		nBitmap = 0;    
	}
	else if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		switch(which)
		{
		case 0: nBitmap = 0; break;
		case 1: nBitmap = 1; break;
		case 3: // Korea - Lane(Length Specifired (탱크 하중))
			{
				if(m_nLegalPermitRadio == 0)       nBitmap = 4;
				else if (m_nLegalPermitRadio == 1) nBitmap = 5;
				else                               nBitmap = 6;
			}
			break;
		}
	}
	else
	{
		switch(which)
		{
		case 0: nBitmap = 0; break;
		case 1: nBitmap = 1; break;
		case 3: 
			{
				if(nKoreaLaneCombo == 0) nBitmap = 2;
				else                     nBitmap = 3;
			}
			break;
		}
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	if(m_nMovingType == D_MOVE_CODE_CHINA)  m_wndPicture.SetImage(strSVGPath + aBitmapIDch[nBitmap]);   // China
	else if(m_nMovingType == D_MOVE_CODE_INDIA)  m_wndPicture.SetImage(strSVGPath + aBitmapIDid[nBitmap]);   // India
	else if(m_nMovingType == D_MOVE_CODE_CANADA) m_wndPicture.SetImage(strSVGPath + aBitmapIDca[nBitmap]);   // Canada
	else                                         m_wndPicture.SetImage(strSVGPath + aBitmapID[nBitmap]);     // Korea
	
	if(m_nMovingType == D_MOVE_CODE_INDIA)
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->SetWindowText(_LS(IDS_CMD_WHEEL_TRACK));
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->SetWindowText(_LS(IDS_CMD_WHEEL_LOAD));
		GetDlgItem(IDC_CMD_TRCKLD_STATIC)->SetWindowText(_LS(IDS_CMD_IRC_WHEEL_VEHICLE));    
	}
	else if(m_nMovingType != D_MOVE_CODE_CHINA && m_nMovingType != D_MOVE_CODE_INDIA)
	{
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRK_RD)->SetWindowText(_LS(IDS_CMD_TRUCK_LANE));
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_TRN_RD)->SetWindowText(_LS(IDS_CMD_TRAIN_LOAD));
		GetDlgItem(IDC_CMD_TRCKLD_STATIC)->SetWindowText(_LS(IDS_CMD_TRUCK_LOAD));    
	}
}

void CCMMvhlItemUsrDlg::EnableLegalPermitRadio()
{
	UpdateData();
	GetDlgItem(IDC_CMD_U_R_STATIC)->EnableWindow(m_nLegalPermitRadio != 1);
	GetDlgItem(IDC_CMD_U_R_EDT)->EnableWindow(m_nLegalPermitRadio != 1);
	GetDlgItem(IDC_CMD_U_R_UNIT)->EnableWindow(m_nLegalPermitRadio != 1);
	GetDlgItem(IDC_CMD_U_DIST_STATIC)->EnableWindow(m_nLegalPermitRadio != 1);
	GetDlgItem(IDC_CMD_U_DIST_EDT)->EnableWindow(m_nLegalPermitRadio != 1);
	GetDlgItem(IDC_CMD_U_DIST_UNIT)->EnableWindow(m_nLegalPermitRadio != 1);

	if (m_nLegalPermitRadio==2)
	{
		GetDlgItem(IDC_CMD_U_DIST_STATIC)->SetWindowText(_LS(IDS_CMD_MVHL_MVHL_LEGAL_MIN_DIST));
	}
	else
	{
		GetDlgItem(IDC_CMD_U_DIST_STATIC)->SetWindowText(_LS(IDS_CMD_MVHL_MVHL_LEGAL_DIST));
	}
}

void CCMMvhlItemUsrDlg::EnableChinaRadio(int which)
{
	CWnd *pWnd;
	pWnd = this->GetDlgItem(IDC_CMD_MVHL_U_TRUCK_COMBO); pWnd->EnableWindow(which == 0);
	pWnd = this->GetDlgItem(IDC_CMD_MVHL_U_TRAIN_COMBO); pWnd->EnableWindow(which == 1);
	pWnd = this->GetDlgItem(IDC_CMD_MVHL_U_CROWD_COMBO); pWnd->EnableWindow(which == 2);
}

void CCMMvhlItemUsrDlg::OnSelchangeChTruckCombo()
{
	UpdateData(TRUE);		  
	int nTruckIndex = m_cmbChTruck.GetItemData(m_cmbChTruck.GetCurSel());

	ShowHideCtrl();
				
	ChangeBitMap(GetTrkRadio());  
}

void CCMMvhlItemUsrDlg::OnSelchangeChTrainCombo()
{
	UpdateData();		
	CWnd *pChildWnd;
	int nIndex = m_cmbChTrain.GetItemData(m_cmbChTrain.GetCurSel());
	BOOL bTrain = (nIndex==1 || nIndex==5); // Type1 or Type3
	pChildWnd = m_pTrnDlg->GetDlgItem(IDC_CMD_dW1); pChildWnd->EnableWindow(bTrain);
	pChildWnd = m_pTrnDlg->GetDlgItem(IDC_CMD_dW2); pChildWnd->EnableWindow(bTrain);
	pChildWnd = m_pTrnDlg->GetDlgItem(IDC_CMD_dD2); pChildWnd->EnableWindow(bTrain);
	if(!bTrain)
	{
	 m_pTrnDlg->GetDlgItem(IDC_CMD_dW1)->SetWindowText(_T("0"));
	 m_pTrnDlg->GetDlgItem(IDC_CMD_dW2)->SetWindowText(_T("0"));
	 m_pTrnDlg->GetDlgItem(IDC_CMD_dD2)->SetWindowText(_T("0"));
	}

	if(m_pTrnDlg)
	{
		m_pTrnDlg->m_bChSubway = (nIndex==4); // Subway
		m_pTrnDlg->ShowWindow(GetTrkRadio()==1);
		m_pTrnDlg->CtrlShowHide();
	}
	ChangeBitMap(GetTrkRadio());  

	//////////////////////////////////////////////////////////
	// show new Train/exceptional vehicle Load........	
	m_pTrainDlg->ShowWindow(FALSE);		// Added by HP.....
	if(nIndex==2&&m_nMovingType == D_MOVE_CODE_CHINA&&m_pTrainDlg)
	{
		UpdateData(TRUE);        
		m_pTrnDlg->ShowWindow(FALSE);		// Added by HP.....
		m_pTrainDlg->ShowWindow(TRUE);
	}	
}

int CCMMvhlItemUsrDlg::GetTrkRadio()
{
	// ReturnVal : 0:Truck/Lane, 1:Train/Exceptional Vehicle, 2:Crown Load, 3:Legal Load
	if(m_nMovingType == D_MOVE_CODE_AASHTO_LRFD)
	{
		if(m_nTrkLegalRadio == 0) return 0;
		else if(m_nTrkLegalRadio == 1) return 3;
		else if(m_nTrkLegalRadio == 2) return 1;
		else if(m_nTrkLegalRadio == 3) return 2;
	}
	else
	{
		return m_nTrkRadio;
	}
	ASSERT(0);
	return 0;
}

void CCMMvhlItemUsrDlg::SetTrkRadio(int nVal)
{
	// nVal : 0:Truck/Lane, 1:Train/Exceptional Vehicle, 2:Crown Load, 3:Legal Load  
	m_nTrkRadio = nVal;

	if(nVal == 0) m_nTrkLegalRadio = 0;
	else if(nVal == 1) m_nTrkLegalRadio = 2;
	else if(nVal == 2) m_nTrkLegalRadio = 3;
	else if(nVal == 3) m_nTrkLegalRadio = 1;
}

void CCMMvhlItemUsrDlg::OnSelchangeChCrowdCombo()
{
	UpdateData(TRUE);		  
	
	ShowHideCtrl();

	ChangeBitMap(GetTrkRadio());  
}

BOOL CCMMvhlItemUsrDlg::CreateBSLoadDlg()
{
	CRect rc;

	m_pBS5400Dlg	= new	CCMMvhlItemUsrBSDlg(&m_Data);
	ASSERT(m_pBS5400Dlg);	
	if(!m_pBS5400Dlg) return FALSE;

	m_pBSHADlg	= new	CCMMvhlItemUsrBSHAPage(&m_Data, this);
	ASSERT(m_pBSHADlg);	
	if(!m_pBSHADlg) return FALSE;
	
	m_pBSHBDlg	= new	CCMMvhlItemUsrBSHBPage(&m_Data);
	ASSERT(m_pBSHBDlg);	
	if(!m_pBSHBDlg) return FALSE;
	
	m_pBSHAHBDlg	= new	CCMMvhlItemUsrBSHAHBPage(&m_Data, this);
	ASSERT(m_pBSHAHBDlg);	
	if(!m_pBSHAHBDlg) return FALSE;

	m_pBSHAHBAUTODlg	= new	CCMMvhlItemUsrBSBD3701Dlg(&m_Data, this);
	ASSERT(m_pBSHAHBAUTODlg);	
	if(!m_pBSHAHBAUTODlg) return FALSE;

	m_pBSPedDlg	= new	CCMMvhlItemUsrBSPedDlg(&m_Data);
	ASSERT(m_pBSPedDlg);	
	if(!m_pBSPedDlg) return FALSE;

	m_pBSSpecialDlg	= new	CCMMvhlItemUsrBSSpecialPage(&m_Data);
	ASSERT(m_pBSSpecialDlg);	
	if(!m_pBSSpecialDlg) return FALSE;

	// holder
	CWnd *cwnd = (CWnd *)GetDlgItem(IDC_CMD_DLGHOLDER);	
	if(!cwnd)	return FALSE;
	cwnd->GetWindowRect(&rc);		// holder 위치.. 
	this->ScreenToClient(&rc);	// 현재 대화상자 기준좌표. 

	// create dlg
	CRect rDlg;
	if(m_pBS5400Dlg->GetSafeHwnd()==0)
	{
		m_pBS5400Dlg->Create(IDD_CMD_ML_MVHL_ITEMU_BS,this);
		m_pBS5400Dlg->GetWindowRect(rDlg);
		m_pBS5400Dlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	if(m_pBSHADlg->GetSafeHwnd()==0)
	{
		m_pBSHADlg->Create(IDD_CMD_ML_MVHL_ITEMU_BS_HA_PAGE,this);
		m_pBSHADlg->GetWindowRect(rDlg);
		m_pBSHADlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	if(m_pBSHBDlg->GetSafeHwnd()==0)
	{
		m_pBSHBDlg->Create(IDD_CMD_ML_MVHL_ITEMU_BS_HB_PAGE,this);
		m_pBSHBDlg->GetWindowRect(rDlg);
		m_pBSHBDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	if(m_pBSHAHBDlg->GetSafeHwnd()==0)
	{
		m_pBSHAHBDlg->Create(IDD_CMD_ML_MVHL_ITEMU_BS_HAHB_PAGE,this);
		m_pBSHAHBDlg->GetWindowRect(rDlg);
		m_pBSHAHBDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	if(m_pBSHAHBAUTODlg->GetSafeHwnd()==0)
	{
		m_pBSHAHBAUTODlg->Create(IDD_CMD_ML_MVHL_ITEMU_BS_BD3701,this);
		m_pBSHAHBAUTODlg->GetWindowRect(rDlg);
		m_pBSHAHBAUTODlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	if(m_pBSPedDlg->GetSafeHwnd()==0)
	{
		m_pBSPedDlg->Create(IDD_CMD_ML_MVHL_ITEMU_PED,this);
		m_pBSPedDlg->GetWindowRect(rDlg);
		m_pBSPedDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	if(m_pBSSpecialDlg->GetSafeHwnd()==0)
	{
		m_pBSSpecialDlg->Create(IDD_CMD_ML_MVHL_ITEMU_BS_SPECIAL_PAGE,this);
		m_pBSSpecialDlg->GetWindowRect(rDlg);
		m_pBSSpecialDlg->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	return TRUE;
}

void CCMMvhlItemUsrDlg::DestroyBSLoadDlg()
{
	if(m_pBS5400Dlg)
	{
		m_pBS5400Dlg->SendMessage(WM_DESTROY,0,0);
		delete m_pBS5400Dlg;
		m_pBS5400Dlg = NULL;
	}
	if(m_pBSHADlg)
	{
		m_pBSHADlg->SendMessage(WM_DESTROY,0,0);
		delete m_pBSHADlg;
		m_pBSHADlg = NULL;
	}
	if(m_pBSHBDlg)
	{
		m_pBSHBDlg->SendMessage(WM_DESTROY,0,0);
		delete m_pBSHBDlg;
		m_pBSHBDlg = NULL;
	}
	if(m_pBSHAHBDlg)
	{
		m_pBSHAHBDlg->SendMessage(WM_DESTROY,0,0);
		delete m_pBSHAHBDlg;
		m_pBSHAHBDlg = NULL;
	}
	if(m_pBSHAHBAUTODlg)
	{
		m_pBSHAHBAUTODlg->SendMessage(WM_DESTROY,0,0);
		delete m_pBSHAHBAUTODlg;
		m_pBSHAHBAUTODlg = NULL;
	}
	if(m_pBSPedDlg)
	{
		m_pBSPedDlg->SendMessage(WM_DESTROY,0,0);
		delete m_pBSPedDlg;
		m_pBSPedDlg = NULL;
	}
	if(m_pBSSpecialDlg)
	{
		m_pBSSpecialDlg->SendMessage(WM_DESTROY,0,0);
		delete m_pBSSpecialDlg;
		m_pBSSpecialDlg = NULL;
	}
}

void CCMMvhlItemUsrDlg::CtrlBSLoadDlg()
{
	ShowHideCtrl();
	

	OnCmdMlMvhlItemuBSRdo();
}

void CCMMvhlItemUsrDlg::LoadBSBitmap()
{
	CString nBitmapID = _T("cmd_mvhl_132.svg");

	if     (m_nBSType==0) nBitmapID = _T("cmd_mvhl_132.svg");
	else if(m_nBSType==1) nBitmapID = _T("cmd_mvhl_133.svg");
	else if(m_nBSType==2) nBitmapID = _T("cmd_mvhl_134.svg");
	else if(m_nBSType==3) nBitmapID = _T("cmd_mvhl_135.svg");
	else if(m_nBSType==4) nBitmapID = _T("cmd_mvhl_132.svg");
	else if(m_nBSType==5) nBitmapID = _T("cmd_mvhl_136.svg");
	else if(m_nBSType==6) nBitmapID = _T("cmd_mvhl_137.svg");
	else ASSERT(0);

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture2.SetImage(strSVGPath + nBitmapID);
}

void CCMMvhlItemUsrDlg::SetInitChTruckCombo()
{
	m_cmbChTruck.ResetContent();
	//int nIndex = m_cmbChTruck.AddString(_LS(IDS_CMD_TRUCK_LOAD_JTG3));  // Truck Load(JTG B01-2003)
	int nIndex = m_cmbChTruck.AddString(_LS(IDS_CMD_TRUCK_LOAD));  // Truck Load(JTG B01-2003)
	m_cmbChTruck.SetItemData(nIndex, 6); 
	nIndex = m_cmbChTruck.AddString(_LS(IDS_CMD_LANE_LOAD1));  // Lane Load1
	m_cmbChTruck.SetItemData(nIndex, 0); 
	nIndex = m_cmbChTruck.AddString(_LS(IDS_CMD_LANE_LOAD2));      // Lane Load2
	m_cmbChTruck.SetItemData(nIndex, 5); 
	nIndex = m_cmbChTruck.AddString(_LS(IDS_CMD_MVHL_CRAWLER_TYPE));    // Crawler Type Load
	m_cmbChTruck.SetItemData(nIndex, 1); 
	nIndex = m_cmbChTruck.AddString(_LS(IDS_CMD_GC_TYPE_LOAD));    // GC Type Load
	m_cmbChTruck.SetItemData(nIndex, 2); 
}

void CCMMvhlItemUsrDlg::SetInitChTrainCombo()
{
	m_cmbChTrain.ResetContent();
	int nIndex = m_cmbChTrain.AddString(_LS(IDS_CMD_VHCL_USER_TRAIN_TYPE1));
	m_cmbChTrain.SetItemData(nIndex, 1); // Type1
	nIndex = m_cmbChTrain.AddString(_LS(IDS_CMD_VHCL_USER_TRAIN_TYPE2));
	m_cmbChTrain.SetItemData(nIndex, 2); // Type2
	nIndex = m_cmbChTrain.AddString(_LS(IDS_CMD_VHCL_USER_TRAIN_TYPE3));
	m_cmbChTrain.SetItemData(nIndex, 5); // Type3
	nIndex = m_cmbChTrain.AddString(_LS(IDS_CMD_VHCL_USER_TRAIN_SUBW));
	m_cmbChTrain.SetItemData(nIndex, 4); // Subway
}

void CCMMvhlItemUsrDlg::SetInitChCrowdCombo()
{
	m_cmbChCrowd.ResetContent();
	int nIndex = m_cmbChCrowd.AddString(_LS(IDS_CMD_VHCL_USER_CROWD_TYPE1));
	m_cmbChCrowd.SetItemData(nIndex, 2); // Type1
	nIndex = m_cmbChCrowd.AddString(_LS(IDS_CMD_VHCL_USER_CROWD_TYPE2));
	m_cmbChCrowd.SetItemData(nIndex, 3); // Type2  
}

void CCMMvhlItemUsrDlg::SetInitKoreaLaneCombo()
{
	m_cmbUniformLoadNum.ResetContent();
	
	m_cmbUniformLoadNum.AddString(_T("1"));
	m_cmbUniformLoadNum.AddString(_T("2"));
	m_cmbUniformLoadNum.AddString(_T("3"));
	m_cmbUniformLoadNum.AddString(_T("4"));
	m_cmbUniformLoadNum.AddString(_T("5"));
	m_cmbUniformLoadNum.AddString(_T("6"));
	m_cmbUniformLoadNum.AddString(_T("7"));
	m_cmbUniformLoadNum.AddString(_T("8"));
	m_cmbUniformLoadNum.AddString(_T("9"));
	m_cmbUniformLoadNum.AddString(_T("10"));

	m_cmbUniformLoadNum.SetCurSel(0);
}