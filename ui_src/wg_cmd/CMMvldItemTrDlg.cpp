// CMMvldItemTrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemTrDlg.h"


#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_strParser.h"
#include "CMMvldItemDlg.h"
#include "CMMvhlListTrDlg.h"
#include "CMLlanTrDlg.h"
#include "ExportFunc.h"

using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_TOP_MARGIN 50
/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemTrDlg dialog


CCMMvldItemTrDlg::CCMMvldItemTrDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvldItemTrDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_bModify = FALSE;
	//{{AFX_DATA_INIT(CCMMvldItemTrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvldItemTrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvldItemTrDlg)
	//DDX_Control(pDX, IDC_CMD_MLC_NUM_SPN, m_spnNum);
	DDX_Control(pDX, IDC_CMD_MLC_NUM_EDT, m_edtNum);
	DDX_Control(pDX, IDC_CMD_MLC_SCALE_EDT, m_edtScale);
	DDX_Control(pDX, IDC_CMD_MLC_LANE_CMB, m_cmbLane);
	DDX_Control(pDX, IDC_CMD_MLC_VEHICLE_CMB, m_cmbVehicle);
	DDX_Control(pDX, IDC_CMD_MLC_DESC_EDT, m_wndDesc);
	DDX_Control(pDX, IDC_CMD_MLC_NAME_EDT, m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_GRID, m_wndMvldTrGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvldItemTrDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvldItemTrDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_EN_CHANGE(IDC_CMD_MLC_NUM_EDT, OnChangeCmdMlcNumEdt)
	ON_BN_CLICKED(IDC_CMD_MLC_VEHICLE_BTN, OnCmdMlcVehicleBtn)
	ON_BN_CLICKED(IDC_CMD_MLC_REMOVE_BTN, OnCmdMlcRemoveBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemTrDlg message handlers

BOOL CCMMvldItemTrDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	 
	if (!m_bModify)
	{
		m_Data.Initialize();
	}
	m_wndMvldTrGrid.InitGrid();
	m_wndMvldTrGrid.SetDataSource(&m_Data,this);
	m_edtNum.SetRange(0,1000);
	m_edtNum.SetInteger(TRUE);
	
	GetDlgItem(IDC_CMD_MLC_REMOVE_BTN)->EnableWindow(m_bModify);

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

}
void CCMMvldItemTrDlg::Data2Dlg()
{
	m_wndLoadCase.SetWindowText(m_Data.LoadCaseName);
	m_wndDesc.SetWindowText(m_Data.Description);
	CArray<CString , CString&> arMvhltrList;
	CArray<CString , CString&> arLlantrList;
	CArray<T_MVHLtr_K, T_MVHLtr_K> arMvhltrKey;
	CArray<T_LLANtr_K, T_LLANtr_K> arLlantrKey;

	m_pDoc->m_pAttrCtrl->GetMvhltrList(arMvhltrList);
	m_pDoc->m_pAttrCtrl->GetMvhltrKeyList(arMvhltrKey);
	m_pDoc->m_pAttrCtrl->GetLlantrList(arLlantrList);
	m_pDoc->m_pAttrCtrl->GetLlantrKeyList(arLlantrKey);
	m_cmbVehicle.ResetContent();
	m_cmbLane.ResetContent();
	int iMvhlIndex=0,iLlanIndex=0;
	for(int i=0;i<arMvhltrList.GetSize();i++)
	{
		m_cmbVehicle.SetItemData(m_cmbVehicle.AddString(arMvhltrList[i]), arMvhltrKey[i]);
		if(m_Data.MvhlK==arMvhltrKey[i])
			iMvhlIndex=i;
	}
	
	for(int i=0;i<arLlantrList.GetSize();i++)
	{
		m_cmbLane.SetItemData(m_cmbLane.AddString(arLlantrList[i]), arLlantrKey[i]);
		if(m_Data.LlanK==arLlantrKey[i])
			iLlanIndex=i;
	}
	m_cmbVehicle.SetCurSel(iMvhlIndex);
	m_cmbLane.SetCurSel(iLlanIndex);
	CString tempString;
	tempString.Format(_T("%.2f"),m_Data.dScaleFactor);
	m_edtScale.SetWindowText(tempString);
	m_edtNum.SetValue(m_Data.nNumLane);
}

BOOL CCMMvldItemTrDlg::Dlg2Data()
{
	m_wndLoadCase.GetWindowText(m_Data.LoadCaseName);
	if(m_pDoc->IsSymbol(m_Data.LoadCaseName)) { AfxMessageBox(_LS(IDS_CMD_MVLD_ERROR_CASE_NAME)); return FALSE; }
	m_wndDesc.GetWindowText(m_Data.Description);
	m_Data.MvhlK=m_cmbVehicle.GetItemData(m_cmbVehicle.GetCurSel());
	m_Data.LlanK=m_cmbLane.GetItemData(m_cmbLane.GetCurSel());
	CString tempString;
	m_edtScale.GetWindowText(tempString);
	m_Data.dScaleFactor=_tstof(tempString);
	m_Data.nNumLane= m_edtNum.GetEditValueInt();
	return TRUE;
}

BOOL CCMMvldItemTrDlg::ApplyOrOK()
{
	if (!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMvldtr(m_csOldName, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMvldtr(m_Data);
	if (!bSuccess) return FALSE;

	return TRUE;
}


void CCMMvldItemTrDlg::OnOK() 
{
	if (!ApplyOrOK()) return;
	
	CDialogMove::OnOK();
}

void CCMMvldItemTrDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}

void CCMMvldItemTrDlg::OnCmdApply() 
{
	ApplyOrOK();
}

void CCMMvldItemTrDlg::OnChangeCmdMlcNumEdt() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString cLaneNum;
	int iLaneNum;
	if(!IsWindow(m_edtNum))return;
	m_edtNum.GetWindowText(cLaneNum);
	if(CStrParser::GetINumber(cLaneNum,iLaneNum))
	{
		if(iLaneNum>0)
		{
			m_Data.nNumLane=iLaneNum;
			int gridLines=m_Data.aLaneFactor.GetSize();
			if(gridLines>iLaneNum+1)
			{
				CArray<double,double>tempArray;
				for(int i=0;i<gridLines;i++)
				{
					tempArray.Add(m_Data.aLaneFactor[i]);
				}
				m_Data.aLaneFactor.RemoveAll();
				for(int i=0;i<iLaneNum+1;i++)
				{
					m_Data.aLaneFactor.Add(tempArray[i]);
				}
			}
			else
			{
				int difference=iLaneNum+1-gridLines;
				for(int i=0;i<difference;i++)
				{
					m_Data.aLaneFactor.Add(1.0);
				}
			}
			m_wndMvldTrGrid.SetDataSource(&m_Data,this);
			return;   
		}
		
	}
	m_edtNum.SetValue(m_Data.nNumLane);
	
}

void CCMMvldItemTrDlg::OnCmdMlcVehicleBtn() 
{
	CCMMvhlListTrDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LT, 0, D_TOP_MARGIN);
	dlg.DoModal();
	Dlg2Data();
	Data2Dlg();
}

void CCMMvldItemTrDlg::OnCmdMlcRemoveBtn() 
{
	if(m_pDoc->m_pDataCtrl->DelMvldtr(m_Data.LoadCaseName))
	{
		CDialogMove::OnOK();
	}	
}