// CMWindItemIBC2012Topography.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMWindItemIBC2012Topography.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LateralLoad.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIBC2012Topography dialog


CCMWindItemIBC2012Topography::CCMWindItemIBC2012Topography(T_WIND_IBC2012* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMWindItemIBC2012Topography::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindItemIBC2012Topography)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataIBC = NULL;
	m_pDataNSR = NULL;
	m_pDataASCE7 = NULL;
	ASSERT(pData);
	m_pDataIBC = pData;
}

CCMWindItemIBC2012Topography::CCMWindItemIBC2012Topography(T_WIND_NSR2010* pData, CWnd* pParent /*=NULL*/)
	: CInternationalDlg(CCMWindItemIBC2012Topography::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMWindItemIBC2012Topography)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDataIBC = NULL;
	m_pDataNSR = NULL;
	m_pDataASCE7 = NULL;
	ASSERT(pData);
	m_pDataNSR = pData;
}
CCMWindItemIBC2012Topography::CCMWindItemIBC2012Topography(T_WIND_ASCE7_16* pData, CWnd* pParent/* = NULL*/)   // standard constructor
	: CInternationalDlg(CCMWindItemIBC2012Topography::IDD, pParent)
{
	m_pDataIBC = NULL;
	m_pDataNSR = NULL;
	m_pDataASCE7 = NULL;
	ASSERT(pData);
	m_pDataASCE7 = pData;
}

void CCMWindItemIBC2012Topography::DoDataExchange(CDataExchange* pDX)
{
	CInternationalDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMWindItemIBC2012Topography)
	DDX_Control(pDX, IDC_CMD_ANAL_TOPOGRAPHY_X_CHECK, m_chkTopographic_X);
	DDX_Control(pDX, IDC_CMD_ANAL_TOPOGRAPHY_Y_CHECK, m_chkTopographic_Y);
	DDX_Control(pDX, IDC_CMD_ANAL_HLENGTH_X_EDIT    , m_editHillLeng_X  );
	DDX_Control(pDX, IDC_CMD_ANAL_HLENGTH_Y_EDIT    , m_editHillLeng_Y  );
	DDX_Control(pDX, IDC_CMD_ANAL_HILLSHAPE_X_COMBO , m_cobxHillShape_X );
	DDX_Control(pDX, IDC_CMD_ANAL_HILLSHAPE_Y_COMBO , m_cobxHillShape_Y );
	DDX_Control(pDX, IDC_CMD_ANAL_HHEIGHT_X_EDIT    , m_editHHeight_X   );
	DDX_Control(pDX, IDC_CMD_ANAL_HHEIGHT_Y_EDIT    , m_editHHeight_Y   );  
	DDX_Control(pDX, IDC_CMD_ANAL_CBDIST_X_EDIT     , m_editCBDistance_X);
	DDX_Control(pDX, IDC_CMD_ANAL_CBDIST_Y_EDIT     , m_editCBDistance_Y);  
	DDX_Control(pDX, IDC_CMD_ANAL_BLOCATION_X_COMBO , m_cobxBLocation_X );
	DDX_Control(pDX, IDC_CMD_ANAL_BLOCATION_Y_COMBO , m_cobxBLocation_Y );  
	DDX_Control(pDX, IDC_CMD_ANAL_HLENGTH_UNIT      , m_unitHillLeng    );  
	DDX_Control(pDX, IDC_CMD_ANAL_CBDIST_UNIT       , m_unitCBDistance  );
	DDX_Control(pDX, IDC_CMD_ANAL_HHEIGHT_UNIT      , m_unitHHeight     );  
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMWindItemIBC2012Topography, CInternationalDlg)
	//{{AFX_MSG_MAP(CCMWindItemIBC2012Topography)
	ON_BN_CLICKED(IDC_CMD_ANAL_TOPOGRAPHY_X_CHECK, OnCmdAnalTopoCheck_X)
	ON_BN_CLICKED(IDC_CMD_ANAL_TOPOGRAPHY_Y_CHECK, OnCmdAnalTopoCheck_Y)	
	ON_CBN_SELCHANGE(IDC_CMD_ANAL_HILLSHAPE_X_COMBO, OnSelchangeCmdAnalHillshapeCombo_X)
	ON_CBN_SELCHANGE(IDC_CMD_ANAL_HILLSHAPE_Y_COMBO, OnSelchangeCmdAnalHillshapeCombo_Y)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMWindItemIBC2012Topography message handlers

BOOL CCMWindItemIBC2012Topography::OnInitDialog() 
{
	CInternationalDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitControls();

	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMWindItemIBC2012Topography::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	CInternationalDlg::OnOK();
}

void CCMWindItemIBC2012Topography::OnCmdAnalTopoCheck_X() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic_X.GetCheck();
	m_cobxHillShape_X.EnableWindow(nCheck == 1);
	m_cobxBLocation_X.EnableWindow(nCheck == 1);
	m_editHHeight_X.EnableWindow(nCheck == 1);
	m_editHillLeng_X.EnableWindow(nCheck == 1);
	m_editCBDistance_X.EnableWindow(nCheck == 1);
	OnSelchangeCmdAnalHillshapeCombo_X();
}

void CCMWindItemIBC2012Topography::OnCmdAnalTopoCheck_Y() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic_Y.GetCheck();
	m_cobxHillShape_Y.EnableWindow(nCheck == 1);
	m_cobxBLocation_Y.EnableWindow(nCheck == 1);
	m_editHHeight_Y.EnableWindow(nCheck == 1);
	m_editHillLeng_Y.EnableWindow(nCheck == 1);
	m_editCBDistance_Y.EnableWindow(nCheck == 1);
	OnSelchangeCmdAnalHillshapeCombo_Y();
}

void CCMWindItemIBC2012Topography::OnSelchangeCmdAnalHillshapeCombo_X() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic_X.GetCheck();
	if (!nCheck) return;
	
	int nHillShape = m_cobxHillShape_X.GetCurSel();
	m_cobxBLocation_X.EnableWindow(nHillShape == 1);
}

void CCMWindItemIBC2012Topography::OnSelchangeCmdAnalHillshapeCombo_Y() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkTopographic_Y.GetCheck();
	if (!nCheck) return;
	
	int nHillShape = m_cobxHillShape_Y.GetCurSel();
	m_cobxBLocation_Y.EnableWindow(nHillShape == 1);
}

void CCMWindItemIBC2012Topography::InitHillShapeCombo()
{
	m_cobxHillShape_X.ResetContent();
	m_cobxHillShape_Y.ResetContent();
	
	CString aItem[] = {_LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_2DRIDGE), 
		_LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_2DESCARP),
		_LS(IDS_CMD_WIND_NBC_TOPOGRAPHY_3DAXISYM)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxHillShape_X.AddString(aItem[i]);  
		m_cobxHillShape_Y.AddString(aItem[i]);
	}  
	m_cobxHillShape_X.SetCurSel(0);  
	m_cobxHillShape_Y.SetCurSel(0);

	ChangeComboWidth();
}

void CCMWindItemIBC2012Topography::InitBLocationCombo()
{
	m_cobxBLocation_X.ResetContent();
	m_cobxBLocation_Y.ResetContent();
	
	CString aItem[] = {_LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_UP), _LS(IDS_CMD_WIND_NBC_BLDG_LOCATION_DN)};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
	{
		m_cobxBLocation_X.AddString(aItem[i]);
		m_cobxBLocation_Y.AddString(aItem[i]);
	}  
	m_cobxBLocation_X.SetCurSel(0);
	m_cobxBLocation_Y.SetCurSel(0);
}

//--------------------------------------------------------------------------------
void CCMWindItemIBC2012Topography::InitControls()
{
	m_editHillLeng_X.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillLength);  
	m_editHillLeng_Y.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillLength);
	m_editHHeight_X.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillHeight);
	m_editHHeight_Y.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillHeight);
	m_editCBDistance_X.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillDistance);	
	m_editCBDistance_Y.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillDistance);	
	m_unitHHeight.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillHeight);  
	m_unitHillLeng.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillLength);
	m_unitCBDistance.SetUnitType(CUnitCtrl::m_WIND_UNIT.IBC2012_dHillDistance);	  

	InitHillShapeCombo();
	InitBLocationCombo();
}

void CCMWindItemIBC2012Topography::Data2Dlg()
{
	if(m_pDataIBC != NULL)
	{
		m_chkTopographic_X.SetCheck(m_pDataIBC->bTopography[0]);
		m_chkTopographic_Y.SetCheck(m_pDataIBC->bTopography[1]);  
		OnCmdAnalTopoCheck_X();
		OnCmdAnalTopoCheck_Y();  
		m_cobxHillShape_X.SetCurSel(m_pDataIBC->nHillShape[0]);
		m_cobxHillShape_Y.SetCurSel(m_pDataIBC->nHillShape[1]);  
		OnSelchangeCmdAnalHillshapeCombo_X();
		OnSelchangeCmdAnalHillshapeCombo_Y();  
		m_cobxBLocation_X.SetCurSel(m_pDataIBC->nBldgLocation[0]);
		m_cobxBLocation_Y.SetCurSel(m_pDataIBC->nBldgLocation[1]);
		m_editHHeight_X.SetEditUnit(m_pDataIBC->dHillHeight[0]);
		m_editHHeight_Y.SetEditUnit(m_pDataIBC->dHillHeight[1]);
		m_editHillLeng_X.SetEditUnit(m_pDataIBC->dHillLength[0]);
		m_editHillLeng_Y.SetEditUnit(m_pDataIBC->dHillLength[1]);
		m_editCBDistance_X.SetEditUnit(m_pDataIBC->dHillDistance[0]);
		m_editCBDistance_Y.SetEditUnit(m_pDataIBC->dHillDistance[1]);
	}
	else if(m_pDataNSR != NULL)
	{
		m_chkTopographic_X.SetCheck(m_pDataNSR->bTopography[0]);
		m_chkTopographic_Y.SetCheck(m_pDataNSR->bTopography[1]);  
		OnCmdAnalTopoCheck_X();
		OnCmdAnalTopoCheck_Y();  
		m_cobxHillShape_X.SetCurSel(m_pDataNSR->nHillShape[0]);
		m_cobxHillShape_Y.SetCurSel(m_pDataNSR->nHillShape[1]);  
		OnSelchangeCmdAnalHillshapeCombo_X();
		OnSelchangeCmdAnalHillshapeCombo_Y();  
		m_cobxBLocation_X.SetCurSel(m_pDataNSR->nBldgLocation[0]);
		m_cobxBLocation_Y.SetCurSel(m_pDataNSR->nBldgLocation[1]);
		m_editHHeight_X.SetEditUnit(m_pDataNSR->dHillHeight[0]);
		m_editHHeight_Y.SetEditUnit(m_pDataNSR->dHillHeight[1]);
		m_editHillLeng_X.SetEditUnit(m_pDataNSR->dHillLength[0]);
		m_editHillLeng_Y.SetEditUnit(m_pDataNSR->dHillLength[1]);
		m_editCBDistance_X.SetEditUnit(m_pDataNSR->dHillDistance[0]);
		m_editCBDistance_Y.SetEditUnit(m_pDataNSR->dHillDistance[1]);
	}
	else if (m_pDataASCE7 != NULL)
	{
		m_chkTopographic_X.SetCheck(m_pDataASCE7->bTopography[0]);
		m_chkTopographic_Y.SetCheck(m_pDataASCE7->bTopography[1]);
		OnCmdAnalTopoCheck_X();
		OnCmdAnalTopoCheck_Y();
		m_cobxHillShape_X.SetCurSel(m_pDataASCE7->nHillShape[0]);
		m_cobxHillShape_Y.SetCurSel(m_pDataASCE7->nHillShape[1]);
		OnSelchangeCmdAnalHillshapeCombo_X();
		OnSelchangeCmdAnalHillshapeCombo_Y();
		m_cobxBLocation_X.SetCurSel(m_pDataASCE7->nBldgLocation[0]);
		m_cobxBLocation_Y.SetCurSel(m_pDataASCE7->nBldgLocation[1]);
		m_editHHeight_X.SetEditUnit(m_pDataASCE7->dHillHeight[0]);
		m_editHHeight_Y.SetEditUnit(m_pDataASCE7->dHillHeight[1]);
		m_editHillLeng_X.SetEditUnit(m_pDataASCE7->dHillLength[0]);
		m_editHillLeng_Y.SetEditUnit(m_pDataASCE7->dHillLength[1]);
		m_editCBDistance_X.SetEditUnit(m_pDataASCE7->dHillDistance[0]);
		m_editCBDistance_Y.SetEditUnit(m_pDataASCE7->dHillDistance[1]);
	}
}

BOOL CCMWindItemIBC2012Topography::Dlg2Data()
{
	if(m_pDataIBC != NULL)
	{
		m_pDataIBC->bTopography[0] = m_chkTopographic_X.GetCheck();
		m_pDataIBC->bTopography[1] = m_chkTopographic_Y.GetCheck();	
		if (m_pDataIBC->bTopography[0])
		{
			m_pDataIBC->nHillShape[0]    = m_cobxHillShape_X.GetCurSel();
			m_pDataIBC->nBldgLocation[0] = m_cobxBLocation_X.GetCurSel();
			m_pDataIBC->dHillHeight[0]   = m_editHHeight_X.GetEditValue();
			m_pDataIBC->dHillLength[0]   = m_editHillLeng_X.GetEditValue();
			m_pDataIBC->dHillDistance[0] = m_editCBDistance_X.GetEditValue();
		}

		if (m_pDataIBC->bTopography[1])
		{
			m_pDataIBC->nHillShape[1]    = m_cobxHillShape_Y.GetCurSel();
			m_pDataIBC->nBldgLocation[1] = m_cobxBLocation_Y.GetCurSel();
			m_pDataIBC->dHillHeight[1]   = m_editHHeight_Y.GetEditValue();
			m_pDataIBC->dHillLength[1]   = m_editHillLeng_Y.GetEditValue();
			m_pDataIBC->dHillDistance[1] = m_editCBDistance_Y.GetEditValue();
		}
	}
	else if(m_pDataNSR != NULL)
	{
		m_pDataNSR->bTopography[0] = m_chkTopographic_X.GetCheck();
		m_pDataNSR->bTopography[1] = m_chkTopographic_Y.GetCheck();	
		if (m_pDataNSR->bTopography[0])
		{
			m_pDataNSR->nHillShape[0]    = m_cobxHillShape_X.GetCurSel();
			m_pDataNSR->nBldgLocation[0] = m_cobxBLocation_X.GetCurSel();
			m_pDataNSR->dHillHeight[0]   = m_editHHeight_X.GetEditValue();
			m_pDataNSR->dHillLength[0]   = m_editHillLeng_X.GetEditValue();
			m_pDataNSR->dHillDistance[0] = m_editCBDistance_X.GetEditValue();
		}

		if (m_pDataNSR->bTopography[1])
		{
			m_pDataNSR->nHillShape[1]    = m_cobxHillShape_Y.GetCurSel();
			m_pDataNSR->nBldgLocation[1] = m_cobxBLocation_Y.GetCurSel();
			m_pDataNSR->dHillHeight[1]   = m_editHHeight_Y.GetEditValue();
			m_pDataNSR->dHillLength[1]   = m_editHillLeng_Y.GetEditValue();
			m_pDataNSR->dHillDistance[1] = m_editCBDistance_Y.GetEditValue();
		}
	}
	else if (m_pDataASCE7 != NULL)
	{
		m_pDataASCE7->bTopography[0] = m_chkTopographic_X.GetCheck();
		m_pDataASCE7->bTopography[1] = m_chkTopographic_Y.GetCheck();
		if (m_pDataASCE7->bTopography[0])
		{
			m_pDataASCE7->nHillShape[0] = m_cobxHillShape_X.GetCurSel();
			m_pDataASCE7->nBldgLocation[0] = m_cobxBLocation_X.GetCurSel();
			m_pDataASCE7->dHillHeight[0] = m_editHHeight_X.GetEditValue();
			m_pDataASCE7->dHillLength[0] = m_editHillLeng_X.GetEditValue();
			m_pDataASCE7->dHillDistance[0] = m_editCBDistance_X.GetEditValue();
		}

		if (m_pDataASCE7->bTopography[1])
		{
			m_pDataASCE7->nHillShape[1] = m_cobxHillShape_Y.GetCurSel();
			m_pDataASCE7->nBldgLocation[1] = m_cobxBLocation_Y.GetCurSel();
			m_pDataASCE7->dHillHeight[1] = m_editHHeight_Y.GetEditValue();
			m_pDataASCE7->dHillLength[1] = m_editHillLeng_Y.GetEditValue();
			m_pDataASCE7->dHillDistance[1] = m_editCBDistance_Y.GetEditValue();
		}
	}
	return TRUE;
}

void CCMWindItemIBC2012Topography::ChangeComboWidth()
{
	int nCount = m_cobxHillShape_X.GetCount();
	if(nCount<1) return;
	
	CString data = _T("");
	m_cobxHillShape_X.GetLBText(0, data);
	
	CDC* pDC = GetDC();  
	CSize max, cur;
	max = pDC->GetTextExtent(data);
	m_cobxHillShape_X.SetDroppedWidth(max.cx);
	m_cobxHillShape_Y.SetDroppedWidth(max.cx);
	
	for(int i=1; i<nCount; i++)
	{
		m_cobxHillShape_X.GetLBText(i, data);
		cur = pDC->GetTextExtent(data); 
		if (cur.cx > max.cx) max.cx = cur.cx;
	}
	if(max.cx > m_cobxHillShape_X.GetDroppedWidth())
	{
		m_cobxHillShape_X.SetDroppedWidth(max.cx);
		m_cobxHillShape_Y.SetDroppedWidth(max.cx);
	}
	
	ReleaseDC(pDC);
}