// CMSectOffsetDlg.cpp : implementation file
//
// 2004. 03. 04   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectOffsetDlg.h"

#include "CMSectItemBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectOffsetDlg dialog


CCMSectOffsetDlg::CCMSectOffsetDlg(T_SECT_SECTION_D* pData, BOOL bTapered/*=FALSE*/, CWnd* pParent/*=NULL*/)
	: CDialogMove(CCMSectOffsetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectOffsetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pData = NULL;
	ASSERT(pData);
	m_pData = pData;

	m_bTapered = bTapered;

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemBase*)pParent;

	m_aCtrlCenter.Add(IDC_CMD_CENTROID_RD);
	m_aCtrlCenter.Add(IDC_CMD_CENTER_RD);
	m_aCtrlVertical.Add(IDC_CMD_VSIZE_RD);
	m_aCtrlVertical.Add(IDC_CMD_VUSER_RD);
	m_aCtrlHorizont.Add(IDC_CMD_HSIZE_RD);
	m_aCtrlHorizont.Add(IDC_CMD_HUSER_RD);
	m_aCtrlUserOffsetRef.Add(IDC_CMD_RDO_REF_CENT);
	m_aCtrlUserOffsetRef.Add(IDC_CMD_RDO_REF_BEDGE);
}


void CCMSectOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectOffsetDlg)
	DDX_Control(pDX, IDC_CMD_VJ_UNIT, m_unitVJ);
	DDX_Control(pDX, IDC_CMD_VJ_EDIT, m_editVJ);
	DDX_Control(pDX, IDC_CMD_VI_UNIT, m_unitVI);
	DDX_Control(pDX, IDC_CMD_VI_EDIT, m_editVI);
	DDX_Control(pDX, IDC_CMD_OFFSET_CB, m_wndOffset);
	DDX_Control(pDX, IDC_CMD_HJ_UNIT, m_unitHJ);
	DDX_Control(pDX, IDC_CMD_HJ_EDIT, m_editHJ);
	DDX_Control(pDX, IDC_CMD_HI_UNIT, m_unitHI);
	DDX_Control(pDX, IDC_CMD_HI_EDIT, m_editHI);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectOffsetDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectOffsetDlg)
	ON_BN_CLICKED(IDC_CMD_VUSER_RD, OnCmdVerticalRadio)
	ON_BN_CLICKED(IDC_CMD_HSIZE_RD, OnCmdHorizontalRadio)
	ON_CBN_SELCHANGE(IDC_CMD_OFFSET_CB, OnSelchangeCmdOffsetCb)
	ON_BN_CLICKED(IDC_CMD_VSIZE_RD, OnCmdVerticalRadio)
	ON_BN_CLICKED(IDC_CMD_HUSER_RD, OnCmdHorizontalRadio)
	ON_BN_CLICKED(IDC_CMD_OFFSET_BTN, OnCmdOffsetBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectOffsetDlg message handlers

BOOL CCMSectOffsetDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitOffsetCombo();

	m_unitVJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editVJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitVI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editVI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectOffsetDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}

void CCMSectOffsetDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMSectOffsetDlg::OnCmdVerticalRadio() 
{
	// TODO: Add your control notification handler code here
	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlVertical, nOption);
	m_editVI.EnableWindow(nOption==1);
	m_editVJ.EnableWindow(m_bTapered && nOption==1);
	EnableDisableUserOffsetRef();
}

void CCMSectOffsetDlg::OnCmdHorizontalRadio() 
{
	// TODO: Add your control notification handler code here
	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlHorizont, nOption);
	m_editHI.EnableWindow(nOption==1);
	m_editHJ.EnableWindow(m_bTapered && nOption==1);	
	EnableDisableUserOffsetRef();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMSectOffsetDlg::InitOffsetCombo()
{
	m_wndOffset.ResetContent();
	CStringArray arOffsetNameList;
	CSectUtil::GetOffsetNameList(arOffsetNameList);
	for(int i=0; i< arOffsetNameList.GetSize(); i++)
	{
		m_wndOffset.AddString(arOffsetNameList.GetAt(i));
	}
	m_wndOffset.SetCurSel(4);   
}

void CCMSectOffsetDlg::Data2Dlg()
{
	if (m_pData->nOffsetPoint > 0) m_wndOffset.SetCurSel(m_pData->nOffsetPoint-1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlCenter, m_pData->nOffsetCenter);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlHorizont, m_pData->nHorzOffsetOpt);
	m_editHI.SetEditUnit(m_pData->dUserDefOffsetY[0]);
	m_editHJ.SetEditUnit(m_pData->dUserDefOffsetY[1]);
	//OnCmdHorizontalRadio();

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlVertical, m_pData->nVertOffsetOpt);
	m_editVI.SetEditUnit(m_pData->dUserDefOffsetZ[0]);
	m_editVJ.SetEditUnit(m_pData->dUserDefOffsetZ[1]);
	//OnCmdVerticalRadio();

	if (m_pData->nUserOffsetRef != 0 && m_pData->nUserOffsetRef != 1) { ASSERT(0); m_pData->nUserOffsetRef = 0; }
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUserOffsetRef, m_pData->nUserOffsetRef);

	OnSelchangeCmdOffsetCb();
}

BOOL CCMSectOffsetDlg::Dlg2Data()
{
	m_pData->nOffsetPoint = m_wndOffset.GetCurSel()+1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlCenter, m_pData->nOffsetCenter);

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlHorizont, m_pData->nHorzOffsetOpt);
	if (m_pData->nHorzOffsetOpt == 1)
	{
		m_pData->dUserDefOffsetY[0] = m_editHI.GetEditValue();
		m_pData->dUserDefOffsetY[1] = m_editHJ.GetEditValue();
	}
	else
	{
		m_pData->dUserDefOffsetY[0] = 0.0;
		m_pData->dUserDefOffsetY[1] = 0.0;
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlVertical, m_pData->nVertOffsetOpt);
	if (m_pData->nVertOffsetOpt == 1)
	{
		m_pData->dUserDefOffsetZ[0] = m_editVI.GetEditValue();
		m_pData->dUserDefOffsetZ[1] = m_editVJ.GetEditValue();
	}
	else 
	{
		m_pData->dUserDefOffsetZ[0] = 0.0;
		m_pData->dUserDefOffsetZ[1] = 0.0;
	}
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUserOffsetRef, m_pData->nUserOffsetRef);
	return TRUE;
}

void CCMSectOffsetDlg::OnSelchangeCmdOffsetCb() 
{
	// TODO: Add your control notification handler code here
	BOOL bEanbleH = TRUE;
	BOOL bEnableV = TRUE;

	int nOffset = m_wndOffset.GetCurSel();
	nOffset += 1;

	switch(nOffset)
	{
	case 2: case 8: bEanbleH = FALSE; break;
	case 4: case 6: bEnableV = FALSE; break;
	case 5: bEanbleH = bEnableV = FALSE; break;
	}

	GetDlgItem(IDC_CMD_HUSER_RD)->EnableWindow(bEanbleH);
	GetDlgItem(IDC_CMD_HI_EDIT)->EnableWindow(bEanbleH);
	GetDlgItem(IDC_CMD_HJ_EDIT)->EnableWindow(bEanbleH);
	if (bEanbleH) OnCmdHorizontalRadio();
	else CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlHorizont, 0);

	GetDlgItem(IDC_CMD_VUSER_RD)->EnableWindow(bEnableV);
	GetDlgItem(IDC_CMD_VI_EDIT)->EnableWindow(bEnableV);
	GetDlgItem(IDC_CMD_VJ_EDIT)->EnableWindow(bEnableV);	
	if (bEnableV) OnCmdVerticalRadio();
	else CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlVertical, 0);

	EnableDisableCenterIs();
	EnableDisableUserOffsetRef();
}

void CCMSectOffsetDlg::OnCmdOffsetBtn() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data()) { ASSERT(0); return; }
	m_pParent->DisplayOffsetPoint();
}

void CCMSectOffsetDlg::EnableDisableCenterIs()
{
	int nOffset = m_wndOffset.GetCurSel() + 1;

	BOOL bEnable = TRUE;
	if (nOffset == 1 || nOffset == 3 || nOffset == 7 || nOffset == 9) bEnable = FALSE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCenter, bEnable);
}

void CCMSectOffsetDlg::EnableDisableUserOffsetRef()
{
	int nVert, nHorz;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlVertical, nVert);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlHorizont, nHorz);
	BOOL bEnable = FALSE;
	if (nVert == 1 || nHorz == 1) bEnable = TRUE;
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlUserOffsetRef, bEnable);
}
