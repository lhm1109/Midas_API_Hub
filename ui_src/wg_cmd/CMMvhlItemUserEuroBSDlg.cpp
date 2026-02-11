// CMMvhlItemUserEuroBSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUserEuroBSDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#define D_MAX_POINT_LOAD  29
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserEuroBSDlg dialog


CCMMvhlItemUserEuroBSDlg::CCMMvhlItemUserEuroBSDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemUserEuroBSDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	m_pBitmap = 0;
	m_Data4[0].Initialize();
	m_Data4[1].Initialize();
	m_Data4[2].Initialize();
	m_Data4[3].Initialize();

	//{{AFX_DATA_INIT(CCMMvhlItemUserEuroBSDlg)
	//}}AFX_DATA_INIT


	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}


void CCMMvhlItemUserEuroBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemUserEuroBSDlg) 
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_NAME_EDT, m_wndVehicleName);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_D_EDT, m_LM1Dedt);	
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_D_UNIT, m_LM1Dunit);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_DAF_EDT, m_LM1DAFedt);	
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_P_EDT, m_LM2Pedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_D_EDT, m_LM2Dedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_LIST, m_LM2List);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_ADJUST_EDT, m_LM2AFedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_ADJUST_EDT2, m_LM2AFedt2);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_W_EDT, m_LM2Wedt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_W_UNIT, m_LM2Wunit);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_TLFACT_EDT, m_edtLM2TSFact);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM2_UDLFACT_EDT, m_edtLM2UDLFact);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK, m_LM3LCchk[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_NUM_SPN, m_LM3NOPspn[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_NUM_EDT, m_LM3NOPedt[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK, m_LM3DAFchk[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT, m_LM3DAFedt[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK, m_LM3LCchk[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_NUM_SPN, m_LM3NOPspn[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_NUM_EDT, m_LM3NOPedt[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK, m_LM3DAFchk[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT, m_LM3DAFedt[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK, m_LM3LCchk[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_NUM_SPN, m_LM3NOPspn[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_NUM_EDT, m_LM3NOPedt[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK, m_LM3DAFchk[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT, m_LM3DAFedt[2]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_EDT, m_FLM3Intervaledt);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK, m_FLM3DAFchk[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_EDT, m_FLM3DAFedt[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK, m_FLM3DAFchk[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_EDT, m_FLM3DAFedt[1]);
	DDX_Control(pDX, IDC_CMD_APPLY, m_wndBtnApply);
	DDX_Control(pDX, IDC_CMD_MVHL_TSFACT_EDT, m_edtTSFact);
	DDX_Control(pDX, IDC_CMD_MVHL_UDLFACT_EDT, m_edtUDLFact);

	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM1_GRID, m_wndLM1Grid);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_GRID, m_wndLM3Grid[0]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_GRID, m_wndLM3Grid[1]);
	DDX_Control(pDX, IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_GRID, m_wndLM3Grid[2]);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 3
//-------------------------------------------------------------------------
// Implementation
void CCMMvhlItemUserEuroBSDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_MVHL_NO), _LS(IDS_CMD_MVHL_P), _LS(IDS_CMD_MVHL_D)};

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
	dwStyle = ListView_GetExtendedListViewStyle(m_LM2List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_LM2List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 60; nColWidth[2] = 70; // nColWidth[3] = 40;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_LM2List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemUserEuroBSDlg::MakeItemEx()
{
	m_LM2List.DeleteAllItems();

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	if (nLoadCount == 0) return;
	int i = 0;
	for (i = 0; i < nDistCount; i++)
		InsertItem(i, i+1, m_Data4[1].dPointLoad[i], m_Data4[1].dPointDistance[i]);
	if (nLoadCount > nDistCount)
		InsertItem(i, i+1, m_Data4[1].dPointLoad[i], 0.0);
}

BOOL CCMMvhlItemUserEuroBSDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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

		if(i == 0) m_LM2List.InsertItem(&lvitem);
		else       m_LM2List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMMvhlItemUserEuroBSDlg::DeleteItem(int nPos)
{
	if (nPos != -1) m_LM2List.DeleteItem(nPos);
	return TRUE;
}

BOOL CCMMvhlItemUserEuroBSDlg::ModifyItem(int nPos, int nNo, double dLoad, double dDist)
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
			m_LM2List.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

void CCMMvhlItemUserEuroBSDlg::Data2Dlg(int nSubType)
{
	switch(nSubType)
	{
	case 0:
		Data2DlgLM1();
		break;
	case 1:
		Data2DlgLM2();
		break;
	case 2:
		Data2DlgLM3();
		break;
	case 3:
		Data2DlgFLM3();
		break;
	}
}

BOOL CCMMvhlItemUserEuroBSDlg::Dlg2Data(int nSubType)
{	
	switch(nSubType)
	{
	case 0:
		return Dlg2DataLM1();

	case 1:
		return Dlg2DataLM2();

	case 2:
		return Dlg2DataLM3();

	case 3:
		return Dlg2DataFLM3();
	default :
		ASSERT(0);
		return FALSE;
	}
}

void CCMMvhlItemUserEuroBSDlg::Data2DlgLM1()
{
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data4[0].dSpace);
	m_LM1Dedt.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data4[0].dAmplification);
	m_LM1DAFedt.SetWindowText(strTemp); 
	m_wndLM1Grid.SetDataSource(&m_Data4[0],this,FALSE);
	m_edtTSFact.SetEditUnit(m_Data4[0].dAmplification2[0]);
	m_edtUDLFact.SetEditUnit(m_Data4[0].dAmplification2[1]);
}
BOOL CCMMvhlItemUserEuroBSDlg::Dlg2DataLM1()
{
	CFormulaEdit::GetEditValue(&m_LM1Dedt, m_Data4[0].dSpace);
	CFormulaEdit::GetEditValue(&m_LM1DAFedt, m_Data4[0].dAmplification);
	m_Data4[0].dAmplification2[0] = m_edtTSFact.GetEditValue();
	m_Data4[0].dAmplification2[1] = m_edtUDLFact.GetEditValue();
	return TRUE;
}
void CCMMvhlItemUserEuroBSDlg::Data2DlgLM2()
{
	MakeItemEx();
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data4[1].dAdjustment);
	m_LM2AFedt.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data4[1].dAdjustment2);
	m_LM2AFedt2.SetWindowText(strTemp);


	strTemp.Format(_T("%g"),m_Data4[1].dUniformLoad);
	m_LM2Wedt.SetWindowText(strTemp);

	m_edtLM2TSFact.SetEditUnit(m_Data4[1].dAmplification2[0]);
	m_edtLM2UDLFact.SetEditUnit(m_Data4[1].dAmplification2[1]);
}
BOOL CCMMvhlItemUserEuroBSDlg::Dlg2DataLM2()
{
	CFormulaEdit::GetEditValue(&m_LM2AFedt, m_Data4[1].dAdjustment);
	CFormulaEdit::GetEditValue(&m_LM2AFedt2, m_Data4[1].dAdjustment2);
	CFormulaEdit::GetEditValue(&m_LM2Wedt, m_Data4[1].dUniformLoad);

	m_Data4[1].dAmplification2[0] = m_edtLM2TSFact.GetEditValue();
	m_Data4[1].dAmplification2[1] = m_edtLM2UDLFact.GetEditValue();

	return TRUE;
}
void CCMMvhlItemUserEuroBSDlg::Data2DlgLM3()
{
	CArray<UINT,UINT> arTemp;
	m_LM3LCchk[0].SetCheck(m_Data4[2].bUsePointLoad3[0]);
	m_LM3LCchk[1].SetCheck(m_Data4[2].bUsePointLoad3[1]);
	m_LM3LCchk[2].SetCheck(m_Data4[2].bUsePointLoad3[2]);


	//edit box도 값이 반영되는 지 확인해봐야 한다.

	m_LM3NOPspn[0].SetPos(m_Data4[2].nNumLoad3[0]);
	m_LM3NOPspn[1].SetPos(m_Data4[2].nNumLoad3[1]);
	m_LM3NOPspn[2].SetPos(m_Data4[2].nNumLoad3[2]);

	m_wndLM3Grid[0].SetDataSource(&m_Data4[2],0,1);
	m_wndLM3Grid[1].SetDataSource(&m_Data4[2],1,1);
	m_wndLM3Grid[2].SetDataSource(&m_Data4[2],2,1);

	m_LM3DAFchk[0].SetCheck(m_Data4[2].bDynamicFactor3[0]);
	m_LM3DAFchk[1].SetCheck(m_Data4[2].bDynamicFactor3[1]);
	m_LM3DAFchk[2].SetCheck(m_Data4[2].bDynamicFactor3[2]);

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	CDlgUtil::CtrlRadioSetCheck(this,arTemp,m_Data4[2].bUserInput3[0]);

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	CDlgUtil::CtrlRadioSetCheck(this,arTemp,m_Data4[2].bUserInput3[1]);

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	CDlgUtil::CtrlRadioSetCheck(this,arTemp,m_Data4[2].bUserInput3[2]);


	CString strTemp;
	strTemp.Format(_T("%g"),m_Data4[2].dAmplification3[0]);
	m_LM3DAFedt[0].SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data4[2].dAmplification3[1]);
	m_LM3DAFedt[1].SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data4[2].dAmplification3[2]);
	m_LM3DAFedt[2].SetWindowText(strTemp);

}
BOOL CCMMvhlItemUserEuroBSDlg::Dlg2DataLM3()
{
	CArray<UINT,UINT> arTemp;
	m_Data4[2].bUsePointLoad3[0] = m_LM3LCchk[0].GetCheck();
	m_Data4[2].bUsePointLoad3[1] = m_LM3LCchk[1].GetCheck();
	m_Data4[2].bUsePointLoad3[2] = m_LM3LCchk[2].GetCheck();

	CFormulaEdit::GetEditValue(&m_LM3NOPedt[0], m_Data4[2].nNumLoad3[0]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[1], m_Data4[2].nNumLoad3[1]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[2], m_Data4[2].nNumLoad3[2]);


	m_Data4[2].bDynamicFactor3[0] = m_LM3DAFchk[0].GetCheck();
	m_Data4[2].bDynamicFactor3[1] = m_LM3DAFchk[1].GetCheck();
	m_Data4[2].bDynamicFactor3[2] = m_LM3DAFchk[2].GetCheck();

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,arTemp,m_Data4[2].bUserInput3[0]);

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,arTemp,m_Data4[2].bUserInput3[1]);

	arTemp.RemoveAll();
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,arTemp,m_Data4[2].bUserInput3[2]);

	CFormulaEdit::GetEditValue(&m_LM3DAFedt[0], m_Data4[2].dAmplification3[0]);
	CFormulaEdit::GetEditValue(&m_LM3DAFedt[1], m_Data4[2].dAmplification3[1]);
	CFormulaEdit::GetEditValue(&m_LM3DAFedt[2], m_Data4[2].dAmplification3[2]);

	return TRUE;

}
void CCMMvhlItemUserEuroBSDlg::Data2DlgFLM3()
{
	CArray<UINT,UINT> arTemp;
	m_LM3LCchk[0].SetCheck(m_Data4[3].bUsePointLoad3[0]);
	m_LM3LCchk[1].SetCheck(m_Data4[3].bUsePointLoad3[1]);
	m_LM3LCchk[2].SetCheck(m_Data4[3].bUsePointLoad3[2]);


	//edit box도 값이 반영되는 지 확인해봐야 한다.

	m_LM3NOPspn[0].SetPos(m_Data4[3].nNumLoad3[0]);
	m_LM3NOPspn[1].SetPos(m_Data4[3].nNumLoad3[1]);
	m_LM3NOPspn[2].SetPos(m_Data4[3].nNumLoad3[2]);

	m_wndLM3Grid[0].SetDataSource(&m_Data4[3],0,1);
	m_wndLM3Grid[1].SetDataSource(&m_Data4[3],1,1);
	m_wndLM3Grid[2].SetDataSource(&m_Data4[3],2,1);

	m_FLM3DAFchk[0].SetCheck(m_Data4[3].bDynamicFactor2[0]);
	m_FLM3DAFchk[1].SetCheck(m_Data4[3].bDynamicFactor2[1]);


	CString strTemp;
	strTemp.Format(_T("%g"),m_Data4[3].dAmplification2[0]);
	m_FLM3DAFedt[0].SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data4[3].dAmplification2[1]);
	m_FLM3DAFedt[1].SetWindowText(strTemp);

	strTemp.Format(_T("%g"),m_Data4[3].dInterval);
	m_FLM3Intervaledt.SetWindowText(strTemp);

}
BOOL CCMMvhlItemUserEuroBSDlg::Dlg2DataFLM3()
{
	CArray<UINT,UINT> arTemp;
	m_Data4[3].bUsePointLoad3[0] = m_LM3LCchk[0].GetCheck();
	m_Data4[3].bUsePointLoad3[1] = m_LM3LCchk[1].GetCheck();
	m_Data4[3].bUsePointLoad3[2] = m_LM3LCchk[2].GetCheck();

	CFormulaEdit::GetEditValue(&m_LM3NOPedt[0], m_Data4[3].nNumLoad3[0]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[1], m_Data4[3].nNumLoad3[1]);
	CFormulaEdit::GetEditValue(&m_LM3NOPedt[2], m_Data4[3].nNumLoad3[2]);


	m_Data4[3].bDynamicFactor2[0] = m_FLM3DAFchk[0].GetCheck();
	m_Data4[3].bDynamicFactor2[1] = m_FLM3DAFchk[1].GetCheck();

	CFormulaEdit::GetEditValue(&m_FLM3DAFedt[0], m_Data4[3].dAmplification2[0]);
	CFormulaEdit::GetEditValue(&m_FLM3DAFedt[1], m_Data4[3].dAmplification2[1]);
	CFormulaEdit::GetEditValue(&m_FLM3Intervaledt, m_Data4[3].dInterval);
	return TRUE;
}

void CCMMvhlItemUserEuroBSDlg::AlignControl()
{
	int nSubType = GetLoadType();
	CRect rRef;
	CRect rToMove;
	int nDistY;	
	UINT aFirstCtrlID[] = {IDC_CMD_ML_MVHL_ITEMU_ONE_GRUP, IDC_CMD_ML_MVHL_ITEMU_LM2_GRUP1, 
		IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK, IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK
	};

	CArray<UINT, UINT> *aCtrlSet[] = {&m_aCtrlCom1, &m_aCtrlCom2, &m_aCtrlCom3, &m_aCtrlCom3};
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP)->GetWindowRect(rRef);
	for (int i = 0; i < sizeof(aFirstCtrlID)/sizeof(UINT); i++)
	{
		GetDlgItem(aFirstCtrlID[i])->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, *aCtrlSet[i], nDistY);
	}

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_D_EDT)->GetWindowRect(rRef);

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_TXT)->GetWindowRect(rToMove);
	nDistY = (rRef.bottom + rRef.top)/2 - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlCom4, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+16;
	MoveWindow(r);
}


BOOL CCMMvhlItemUserEuroBSDlg::ApplyOrOK()
{
	T_MVHL_D data;
	int nSubType = GetLoadType();
	BOOL bSuccess = FALSE;
	m_Data.bStandard = FALSE;
	Dlg2Data(nSubType);
	m_Data = m_Data4[nSubType];
	m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);
	m_Data.nSubType = nSubType;
	m_Data.nLoadType = 4;

	data = m_Data;
	if (m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, data);
	else					 bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(data);
	if (!bSuccess) return FALSE;

	return TRUE;
}

int CCMMvhlItemUserEuroBSDlg::GetTruckLoadCount()
{
	int i = 0;
	for (i = 0; i < D_MAX_POINT_LOAD; i++)
	{
		if (m_Data4[1].dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemUserEuroBSDlg::GetTruckDistanceCount()
{
	int i = 0;
	for (i = 0; i < D_MAX_POINT_LOAD; i++)
	{
		if (m_Data4[1].dPointDistance[i] == 0.0) break;
	}
	return i;
}

BOOL CCMMvhlItemUserEuroBSDlg::GetLoadAndDistValue(double &dLoad, double &dDist)
{
	if (!CFormulaEdit::GetEditValue(&m_LM2Pedt, dLoad))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_load_value_));
		return FALSE;
	}
	if (!CFormulaEdit::GetEditValue(&m_LM2Dedt, dDist))
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

BEGIN_MESSAGE_MAP(CCMMvhlItemUserEuroBSDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemUserEuroBSDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_DELETE_BTN, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_INSERT_BTN, OnCmdBtnInsert)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_MODIFY_BTN, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_ADD_BTN, OnCmdBtnAdd)
	ON_EN_CHANGE(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_NUM_EDT, OnChangeCmdMlcNumEdt1)
	ON_EN_CHANGE(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_NUM_EDT, OnChangeCmdMlcNumEdt2)
	ON_EN_CHANGE(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_NUM_EDT, OnChangeCmdMlcNumEdt3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_ML_MVHL_ITEMU_LM2_LIST, OnItemchangedCmdList)
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK,OnThreeDynamicChk1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO,OnThreeCmdAutoRdo1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO,OnThreeCmdAutoRdo1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK  ,OnThreeDynamicChk2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO,OnThreeCmdAutoRdo2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO,OnThreeCmdAutoRdo2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK,OnThreeDynamicChk3)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO,OnThreeCmdAutoRdo3)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO,OnThreeCmdAutoRdo3)

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK,OnTwoDynamicChk1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK,OnTwoDynamicChk2)

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK,OnThreeUseChk1)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK,OnThreeUseChk2)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK,OnThreeUseChk3)

	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM2_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_LM3_RDO, OnCmdMlMvhlItemTypeRdo)
	ON_BN_CLICKED(IDC_CMD_ML_MVHL_ITEMU_FLM3_RDO, OnCmdMlMvhlItemTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserEuroBSDlg message handlers

BOOL CCMMvhlItemUserEuroBSDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_edtTSFact.SetUnitType(0);
	m_edtUDLFact.SetUnitType(0);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom1,IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP,TRUE);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom2,IDC_CMD_ML_MVHL_ITEMU_LM2_GRUP1,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom2,IDC_CMD_ML_MVHL_ITEMU_LM2_GRUP2,TRUE);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3_1,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1,FALSE);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3_2,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2,FALSE);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3,TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom3_3,IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3,FALSE);

	m_aCtrlCom3.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK);
	m_aCtrlCom3.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK);
	m_aCtrlCom3.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK);

	CDlgUtil::GetCtrlIDByIncRect(this,m_aCtrlCom4,IDC_CMD_ML_MVHL_ITEMU_FLM3_GRUP,TRUE);

	m_aCtrlCom4.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_TXT);
	m_aCtrlCom4.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_EDT);

	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI2_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT);

	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI2_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT);

	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI2_TXT);
	m_aCtrlCom5.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT);

	if(!m_bModify)
	{
		T_MVHL_D MvhlD; MvhlD.Initialize();
		MvhlD.bStandard = TRUE;    
		MvhlD.VehicleTypeName = _T("Load Model 1");
		MvhlD.nStandardCode = 19;
		CVehlDB db(m_pDoc);  
		if (!db.GetStandardVehicleLoadValue(MvhlD)) MvhlD.Initialize();
		MvhlD.bStandard = FALSE;
		MvhlD.nSubType  = 0;
		m_Data4[0] = MvhlD;

		m_Data.Initialize();
		m_Data.nSubType  = 0;
	}
	else
	{
		m_Data4[0] = m_Data;
	}
	m_Data4[1] = m_Data;
	m_Data4[2] = m_Data;
	m_Data4[3] = m_Data;
	if(!m_bModify)
	{
		m_Data4[0].dAmplification = 1.0;
		m_Data4[0].dAmplification2[0] = 0.75;
		m_Data4[0].dAmplification2[1] = 0.4;
		m_Data4[1].dAdjustment = 1.0;
		m_Data4[1].dAdjustment2 = 1.0;
	}

	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return TRUE;
	if(MvcdD.nCodeType != D_MOVE_CODE_EURO_BS)
	{
		ASSERT(0);
		return FALSE;
	}

	AlignControl();		  // 코드타입에 따라 프레임 정렬

	m_wndLM1Grid.InitGrid();

	m_wndLM3Grid[0].InitGrid();
	m_wndLM3Grid[1].InitGrid();
	m_wndLM3Grid[2].InitGrid();

	m_LM3NOPspn[0].SetRange(0,29);
	m_LM3NOPspn[1].SetRange(0,29);
	m_LM3NOPspn[2].SetRange(0,29);

	m_LM1Dunit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dSpace);
	m_LM2Wunit.SetUnitType(CUnitCtrl::m_MVHL_UNIT.dUniformLoad);



	ChangeBitMap(0);  // 먼저 한번 호출

	SetHeaderTitle();   // header title
	m_nSubType = m_Data.nSubType;
	SetLoadType(m_Data.nSubType);
	Data2Dlg(m_Data.nSubType);

	OnCmdMlMvhlItemTypeRdo();  

	if(m_bModify)
	{ 
		m_wndVehicleName.SetWindowText(m_csOldName);
		m_wndBtnApply.EnableWindow(FALSE);
	}

	ChangeCtrlText();

	// EuroCode Renewal 에 들어가는 Control 비활성화
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_STRADD_RDO)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemUserEuroBSDlg::SetParamData(T_MVHL_D &ParamData) 
{ 
	m_Data.Initialize();
	m_csOldName = ParamData.VehicleLoadName; 
	m_bModify = TRUE;
	m_Data    = ParamData;
	m_Data4[0] = m_Data4[1] = m_Data4[2] = m_Data4[3] = m_Data;
}

void CCMMvhlItemUserEuroBSDlg::OnOK() 
{
	if (!ApplyOrOK()) return;

	CDialogMove::OnOK();
}

void CCMMvhlItemUserEuroBSDlg::OnCmdApply() 
{
	ApplyOrOK();
}

void CCMMvhlItemUserEuroBSDlg::OnCmdBtnAdd() 
{
	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MAX_POINT_LOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MAX_POINT_LOAD);
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
		m_Data4[1].dPointLoad[nLoadCount] = dLoad;
	}
	else
	{
		m_Data4[1].dPointLoad[nLoadCount] = dLoad;
		m_Data4[1].dPointDistance[nLoadCount] = dDist;
	}

	int no;
	no = nLoadCount+1;
	InsertItem(nLoadCount, no, dLoad, dDist);

	int nCount = m_LM2List.GetItemCount();
	m_LM2List.SetItemState(nCount-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUserEuroBSDlg::OnCmdBtnInsert() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_LM2List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();

	double dLoad, dDist;
	if (!GetLoadAndDistValue(dLoad, dDist)) return;

	if (nLoadCount >= D_MAX_POINT_LOAD) // MAX 갯수 초과시
	{
		CString csErr;
		csErr.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_exceeded_l), D_MAX_POINT_LOAD);
		AfxMessageBox(csErr);
		return;
	}

	if (dDist == 0.0)  // middle and dist is zero!
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Intermediate_distance_mus));
		return;
	}

	// 데이타 이동하기
	for (int i = D_MAX_POINT_LOAD-1; i > iItem; i--)
	{
		m_Data4[1].dPointLoad[i] = m_Data4[1].dPointLoad[i-1];
		m_Data4[1].dPointDistance[i] = m_Data4[1].dPointDistance[i-1];
	}

	// 데이타 삽입한다.
	m_Data4[1].dPointLoad[iItem] = dLoad;
	m_Data4[1].dPointDistance[iItem] = dDist;

	// List에 Item 삽입
	InsertItem(iItem, iItem+1, dLoad, dDist);

	// List 번호 변경
	int nCount = m_LM2List.GetItemCount();
	CString csVal;
	for (int i = iItem+1; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_LM2List.SetItemText(i, 0, csVal);
	}
	m_LM2List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMvhlItemUserEuroBSDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_LM2List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
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

	m_Data4[1].dPointLoad[iItem] = dLoad;
	m_Data4[1].dPointDistance[iItem] = dDist;

	ModifyItem(iItem, iItem+1, dLoad, dDist);
}

void CCMMvhlItemUserEuroBSDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_LM2List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	int i = 0;
	for (i = iItem; i < D_MAX_POINT_LOAD-1; i++)
	{
		m_Data4[1].dPointLoad[i] = m_Data4[1].dPointLoad[i+1];
		m_Data4[1].dPointDistance[i] = m_Data4[1].dPointDistance[i+1];
	}
	m_Data4[1].dPointLoad[i] = 0.0;
	m_Data4[1].dPointDistance[i] = 0.0;

	m_LM2List.DeleteItem(iItem);
	int nCount = m_LM2List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_LM2List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);

	// List 번호 변경
	CString csVal;
	for (i = iItem; i < nCount; i++)
	{
		csVal.Format(_T("%d"), i+1);
		m_LM2List.SetItemText(i, 0, csVal);
	}
}

void CCMMvhlItemUserEuroBSDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_LM2List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	int nLoadCount = GetTruckLoadCount();
	int nDistCount = GetTruckDistanceCount();
	CString csVal;
	if (iItem == nLoadCount-1 && nLoadCount != nDistCount)  // fixed, last
	{
		csVal.Format(_T("%g"), m_Data4[1].dPointLoad[iItem]);
		m_LM2Pedt.SetWindowText(csVal);
		m_LM2Dedt.SetWindowText(_T(""));
	}
	else
	{
		csVal.Format(_T("%g"), m_Data4[1].dPointLoad[iItem]);
		m_LM2Pedt.SetWindowText(csVal);
		csVal.Format(_T("%g"), m_Data4[1].dPointDistance[iItem]);
		m_LM2Dedt.SetWindowText(csVal);
	}

	*pResult = 0;
}



void CCMMvhlItemUserEuroBSDlg::OnDestroy() 
{
	CDialogMove::OnDestroy();
	if(m_pBitmap) delete m_pBitmap;
	m_pBitmap = NULL;

	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return;


}


void CCMMvhlItemUserEuroBSDlg::ChangeBitMap(int which)
{	
	UINT aBitmapID[]   = {IDB_CMD_MVHL_70,  IDB_CMD_MVHL_71, IDB_CMD_MVHL_72, IDB_CMD_MVHL_73};

	UpdateData(TRUE);
	if (m_pBitmap != 0) {delete m_pBitmap; m_pBitmap=NULL; }
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_PIC_WND);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[which]);

	pImage->SetBitmap(HBITMAP(*m_pBitmap));

}

void CCMMvhlItemUserEuroBSDlg::OnCmdMlMvhlItemTypeRdo()
{
	Dlg2Data(m_nSubType);
	m_nSubType = GetLoadType();
	Data2Dlg(m_nSubType);
	ChangeBitMap(m_nSubType);  
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom1,m_nSubType==0);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom2,m_nSubType==1);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom3,m_nSubType==2 || m_nSubType==3);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom4,m_nSubType==3);
	CDlgUtil::CtrlShowHide(this,m_aCtrlCom5,m_nSubType==2);

	switch(m_nSubType)
	{
	case 0: break;
	case 1: break;
	case 2:
		{
			CRect rRef,rToMove;
			GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->GetWindowRect(rToMove);
			rToMove.bottom = rRef.bottom;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->GetWindowRect(rToMove);
			rToMove.bottom = rRef.bottom;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_PLACE_HOLDER)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->GetWindowRect(rToMove);
			rToMove.bottom = rRef.bottom;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_loadcase1));
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_loadcase2));

			if(!m_LM3LCchk[0].GetCheck())
			{
				m_LM3LCchk[1].SetCheck(FALSE);
				m_LM3LCchk[2].SetCheck(FALSE);
			}

			OnThreeUseChk1();
			break;
		}
	case 3:
		{
			CRect rRef,rToMove;
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_DYNAMIC_CHK)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->GetWindowRect(rToMove);
			rToMove.bottom = (rRef.top+rRef.bottom)/2;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP1)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_DYNAMIC_CHK)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->GetWindowRect(rToMove);
			rToMove.bottom = (rRef.top+rRef.bottom)/2;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP2)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_DYNAMIC_CHK)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->GetWindowRect(rToMove);
			rToMove.bottom = (rRef.top+rRef.bottom)/2;      
			this->ScreenToClient(&rToMove);
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_GRUP3)->MoveWindow(rToMove);

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_ONE_VEHICLE));
			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK)->SetWindowText(_LS(IDS_CMD_MVLD_TWO_VEHICLE));

			GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK)->ShowWindow(FALSE);

			OnThreeUseChk1();
			OnTwoDynamicChk1();
			OnTwoDynamicChk2();
			break;
		}
	}
	//GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_GRUP)->ShowWindow(FALSE);

}


void CCMMvhlItemUserEuroBSDlg::OnChangeCmdMlcNumEdt1() 
{
	if(!IsWindow(m_LM3NOPedt[0]))return;

	int nSubType = GetLoadType();
	int iLoadNum;
	if(CFormulaEdit::GetEditValue(&m_LM3NOPedt[0], iLoadNum))
	{
		if(iLoadNum>0)
		{
			int gridLines=m_Data4[nSubType].nNumLoad3[0];
			if(gridLines>iLoadNum)
			{
				for(int i=iLoadNum;i<gridLines;i++)
				{
					m_Data4[nSubType].dPointLoad3[0][i] = 0.0;
					m_Data4[nSubType].dPointDistance3[0][i] = 0.0;
				}
			}
			m_Data4[nSubType].nNumLoad3[0] = iLoadNum;
			m_wndLM3Grid[0].SetDataSource(&m_Data4[nSubType],0,1);
			return;   
		}

	}
	//cLoadNum.Format(_T("%d"),m_Data4[nSubType].nNumLoad3[0]);
	//m_LM3NOPedt[0].SetWindowText(cLoadNum);	

}
void CCMMvhlItemUserEuroBSDlg::OnChangeCmdMlcNumEdt2() 
{
	if(!IsWindow(m_LM3NOPedt[1]))return;

	int nSubType = GetLoadType();
	int iLoadNum = 0;
	if(CFormulaEdit::GetEditValue(&m_LM3NOPedt[1], iLoadNum))
	{
		if(iLoadNum>0)
		{
			int gridLines=m_Data4[nSubType].nNumLoad3[0];
			if(gridLines>iLoadNum)
			{
				for(int i=iLoadNum;i<gridLines;i++)
				{
					m_Data4[nSubType].dPointLoad3[1][i] = 0.0;
					m_Data4[nSubType].dPointDistance3[1][i] = 0.0;
				}
			}
			m_Data4[nSubType].nNumLoad3[1] = iLoadNum;
			m_wndLM3Grid[1].SetDataSource(&m_Data4[nSubType],1,1);
			return;   
		}

	}
	//cLoadNum.Format(_T("%d"),m_Data4[nSubType].nNumLoad3[1]);
	//m_LM3NOPedt[1].SetWindowText(cLoadNum);
}
void CCMMvhlItemUserEuroBSDlg::OnChangeCmdMlcNumEdt3() 
{
	if(!IsWindow(m_LM3NOPedt[2]))return;

	int iLoadNum;
	int nSubType = GetLoadType();
	if(CFormulaEdit::GetEditValue(&m_LM3NOPedt[2], iLoadNum))
	{
		if(iLoadNum>0)
		{
			int gridLines=m_Data4[nSubType].nNumLoad3[2];
			if(gridLines>iLoadNum)
			{
				for(int i=iLoadNum;i<gridLines;i++)
				{
					m_Data4[nSubType].dPointLoad3[2][i] = 0.0;
					m_Data4[nSubType].dPointDistance3[2][i] = 0.0;
				}
			}
			m_Data4[nSubType].nNumLoad3[2] = iLoadNum;
			m_wndLM3Grid[2].SetDataSource(&m_Data4[nSubType],2,1);
			return;   
		}

	}
	//cLoadNum.Format(_T("%d"),m_Data4[nSubType].nNumLoad3[2]);
	//m_LM3NOPedt[2].SetWindowText(cLoadNum);
}

void CCMMvhlItemUserEuroBSDlg::OnThreeDynamicChk1()
{
	CArray<UINT,UINT> aDynamicCtrls;
	BOOL bDynamic;
	bDynamic = m_LM3DAFchk[0].GetCheck();

	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI2_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3LCchk[0].GetCheck());
	OnThreeCmdAutoRdo1();

}
void CCMMvhlItemUserEuroBSDlg::OnThreeCmdAutoRdo1()
{
	CArray<UINT,UINT> aDynamicCtrls;
	CArray<UINT,UINT> aRadio;
	BOOL bDynamic;
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_AUTO_RDO);
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_USER_RDO);

	CDlgUtil::CtrlRadioGetCheck(this,aRadio,bDynamic);

	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3DAFchk[0].GetCheck() && m_LM3LCchk[0].GetCheck());
}
void CCMMvhlItemUserEuroBSDlg::OnThreeDynamicChk2()
{
	CArray<UINT,UINT> aDynamicCtrls;
	BOOL bDynamic;
	bDynamic = m_LM3DAFchk[1].GetCheck();

	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI2_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3LCchk[1].GetCheck());
	OnThreeCmdAutoRdo2();

}
void CCMMvhlItemUserEuroBSDlg::OnThreeCmdAutoRdo2()
{
	CArray<UINT,UINT> aDynamicCtrls;
	CArray<UINT,UINT> aRadio;
	BOOL bDynamic;
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_AUTO_RDO);
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_USER_RDO);

	CDlgUtil::CtrlRadioGetCheck(this,aRadio,bDynamic);

	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3DAFchk[1].GetCheck() && m_LM3LCchk[1].GetCheck());

}
void CCMMvhlItemUserEuroBSDlg::OnThreeDynamicChk3()
{
	CArray<UINT,UINT> aDynamicCtrls;
	BOOL bDynamic;
	bDynamic = m_LM3DAFchk[2].GetCheck();

	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI2_TXT);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT);

	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3LCchk[2].GetCheck());
	OnThreeCmdAutoRdo3();

}
void CCMMvhlItemUserEuroBSDlg::OnThreeCmdAutoRdo3()
{
	CArray<UINT,UINT> aDynamicCtrls;
	CArray<UINT,UINT> aRadio;
	BOOL bDynamic;
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_AUTO_RDO);
	aRadio.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,aRadio,bDynamic);
	aDynamicCtrls.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_EDT);
	CDlgUtil::CtrlEnableDisable(this,aDynamicCtrls,bDynamic && m_LM3DAFchk[2].GetCheck() && m_LM3LCchk[2].GetCheck());

}

void CCMMvhlItemUserEuroBSDlg::OnTwoDynamicChk1()
{
	BOOL bDynamic = m_FLM3DAFchk[0].GetCheck();
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_EDT)->EnableWindow(bDynamic);
}

void CCMMvhlItemUserEuroBSDlg::OnTwoDynamicChk2()
{
	BOOL bDynamic = m_FLM3DAFchk[1].GetCheck();

	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_EDT)->EnableWindow(bDynamic);
}

void CCMMvhlItemUserEuroBSDlg::OnThreeUseChk1()
{
	BOOL bDynamic;
	bDynamic = m_LM3LCchk[0].GetCheck();
	CDlgUtil::CtrlEnableDisable(this,m_aCtrlCom3_1,bDynamic);
	m_wndLM3Grid[0].SetEnable(bDynamic);

	int nLoadType = GetLoadType();
	if(nLoadType==2)
	{
		if(!bDynamic)
		{
			m_LM3LCchk[1].SetCheck(FALSE);
			m_LM3LCchk[2].SetCheck(FALSE);
		}
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_LOAD_CHK)->EnableWindow(bDynamic);
		OnThreeUseChk2();
		OnThreeUseChk3();
	}
	else if(nLoadType==3)
	{
		if(!bDynamic) m_FLM3DAFchk[0].SetCheck(FALSE);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK)->EnableWindow(bDynamic);

		OnThreeUseChk2(); // added by mylee
		OnThreeUseChk3(); // added by mylee

		OnTwoDynamicChk1();
	}

	OnThreeDynamicChk1();
}
void CCMMvhlItemUserEuroBSDlg::OnThreeUseChk2()
{
	BOOL bDynamic;
	bDynamic = m_LM3LCchk[1].GetCheck();
	CDlgUtil::CtrlEnableDisable(this,m_aCtrlCom3_2,bDynamic);
	m_wndLM3Grid[1].SetEnable(bDynamic);
	CArray<UINT,UINT> arTemp;
	arTemp.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_LOAD_CHK);
	if(GetLoadType() == 3)
	{
		m_LM3LCchk[2].SetCheck(bDynamic);
		if(!bDynamic)
		{
			m_FLM3DAFchk[1].SetCheck(FALSE);
		}
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK)->EnableWindow(bDynamic);
		GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_INTERVAL_EDT)  ->EnableWindow(bDynamic);
		OnTwoDynamicChk2();
	}
	else
	{
		if(!bDynamic)
			m_LM3LCchk[2].SetCheck(FALSE);
	}
	OnThreeUseChk3();
	CDlgUtil::CtrlEnableDisable(this,arTemp,bDynamic);
	OnThreeDynamicChk2();

}
void CCMMvhlItemUserEuroBSDlg::OnThreeUseChk3()
{
	BOOL bDynamic;
	bDynamic = m_LM3LCchk[2].GetCheck();
	CDlgUtil::CtrlEnableDisable(this,m_aCtrlCom3_3,bDynamic);
	m_wndLM3Grid[2].SetEnable(bDynamic);
	OnThreeDynamicChk3();
}
int CCMMvhlItemUserEuroBSDlg::GetLoadType()
{
	CArray<UINT,UINT> arRdo;
	int nSubType;
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO);
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_LM2_RDO);
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_RDO);
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_RDO);
	CDlgUtil::CtrlRadioGetCheck(this,arRdo,nSubType);
	return nSubType;
}
void CCMMvhlItemUserEuroBSDlg::SetLoadType(int nSubType)
{
	CArray<UINT,UINT> arRdo;
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_LM1_RDO);
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_LM2_RDO);
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_LM3_RDO);
	arRdo.Add(IDC_CMD_ML_MVHL_ITEMU_FLM3_RDO);
	CDlgUtil::CtrlRadioSetCheck(this,arRdo,nSubType);
}

void CCMMvhlItemUserEuroBSDlg::ChangeCtrlText()
{
#if defined(_US) || defined(_RUS)
	CString strText;
	strText.Format(_T("%s factor :"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_UDLFACT_STC)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM2_TLFACT_STC)->SetWindowText(strText);

	strText.Format(_T("%s factor for Tandem System"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_TSFACT_STC)->SetWindowText(strText);
	strText.Format(_T("%s factor for UDL System"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_UDLFACT_STC)->SetWindowText(strText);

	strText.Format(_T("%s :"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM1_DAF_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI2_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI2_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI2_TXT)->SetWindowText(strText);

	strText.Format(_T("%s1"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_FIRST_PHI_CHK)->SetWindowText(strText);
	strText.Format(_T("%s2"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_FLM3_SECOND_PHI_CHK)->SetWindowText(strText);

	strText.Format(_T("%s = 1.40 -L / 500  \n     (1<= %s<= 1.40 )"), CSpecialChar::m_csPhi, CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_FIRST_PHI_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_SECOND_PHI_TXT)->SetWindowText(strText);
	GetDlgItem(IDC_CMD_ML_MVHL_ITEMU_LM3_THIRD_PHI_TXT)->SetWindowText(strText);
#endif
}