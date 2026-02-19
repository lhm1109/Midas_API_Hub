/////////////////////////////////////////////////////////////////////////////////////////////////
//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 기존 방식으로 적용한다.
/////////////////////////////////////////////////////////////////////////////////////////////////

// DgnConBarMinSectSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBarMinSectSizeDlg.h"
#include "NationalAnnexTool.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarMinSectSizeDlg dialog


CDgnConBarMinSectSizeDlg::CDgnConBarMinSectSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBarMinSectSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarMinSectSizeDlg)
	m_strCode = _T("");
	m_strRCCode= _T("");

	m_bBeam = TRUE;
	m_bColm = TRUE;
	m_bBrce = TRUE;
	m_bWall = TRUE;
	m_bSlab = TRUE;
	m_bMat = TRUE;
	
	m_strNationalAnnex= _T("");
	//}}AFX_DATA_INIT
}


void CDgnConBarMinSectSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBarMinSectSizeDlg)
	DDX_Text(pDX, IDC_DGN_CON_BAR_MIN_SECT_RCCODE_STC, m_strRCCode);

	DDX_Check(pDX, IDC_DGN_CON_BAR_MIN_SECT_BEAM_CHK, m_bBeam);
	DDX_Check(pDX, IDC_DGN_CON_BAR_MIN_SECT_COLM_CHK, m_bColm);
	DDX_Check(pDX, IDC_DGN_CON_BAR_MIN_SECT_BRCE_CHK, m_bBrce);
	DDX_Check(pDX, IDC_DGN_CON_BAR_MIN_SECT_WALL_CHK, m_bWall);
	DDX_Check(pDX, IDC_DGN_CON_BAR_MIN_SECT_SLAB_CHK, m_bSlab);
	DDX_Check(pDX, IDC_DGN_CON_BAR_MIN_SECT_MAT_CHK, m_bMat);

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConBarMinSectSizeDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConBarMinSectSizeDlg)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarMinSectSizeDlg message handlers

BOOL CDgnConBarMinSectSizeDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// SET NATIONAL ANNEX
	T_DCON_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(rData);
		
	m_strNationalAnnex  = ConvertNationalAnnexbyCString(rData.nNationalAnnex);
	
	if(m_pDoc->m_pAttrCtrl->ExistDcon())  Initial_ExistData();
	else                                  Initial_DefaultData();

	return TRUE; 
}

void CDgnConBarMinSectSizeDlg::Initial_DefaultData()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	m_strCode = rPref.DgnCode.ConcCode;

	T_DCON_D  rDconD;
	if(!m_pDoc->m_pAttrCtrl->GetDcon(rDconD))
	{
		rDconD.Initialize();
		m_strNationalAnnex = ConvertNationalAnnexbyCString(rPref.DgnCode.nConcNationalAnnex);
	}
	else
	{
		m_strNationalAnnex = ConvertNationalAnnexbyCString(rDconD.nNationalAnnex);
	}

    if ( m_strCode==CONCODE_EC2_04 || m_strCode==CONCODE_EC2_2_05 )
		m_strRCCode.Format(_T("%s, %s"), m_strCode, m_strNationalAnnex);
	else
		m_strRCCode = m_strCode;

	m_bBeam = rDconD.bMinRebarBeam;
	m_bColm = rDconD.bMinRebarColumn;
	m_bBrce = rDconD.bMinRebarBrace;
	m_bWall = rDconD.bMinRebarWall;
	m_bSlab = rDconD.bMinRebarSlab;
	m_bMat = rDconD.bMinRebarMat;

    if ( m_strCode!=CONCODE_EC2_04 && m_strCode!=CONCODE_IS456_2000 )
	{
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_SLAB_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_MAT_CHK)->ShowWindow(SW_HIDE);

		if(m_strCode==CONCODE_EC2_2_05)
		{
			GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_COLM_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_BRCE_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_WALL_CHK)->ShowWindow(SW_HIDE);
		}
	}
    if ( m_strCode==CONCODE_IS456_2000 )
	{
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_SLAB_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_MAT_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_WALL_CHK)->ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);
}

void CDgnConBarMinSectSizeDlg::Initial_ExistData()
{
	T_DCON_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcon(rData);

	if(rData.DesignCode==_T(""))
	{
		T_PREFERENCE rPref;
		rPref.Initialize();
		m_pDoc->m_pInitCtrl->GetPreference(rPref);
		m_strCode = rPref.DgnCode.ConcCode;
	}
	else
	{
		m_strCode = rData.DesignCode;
	}

    if ( m_strCode == CONCODE_EC2_04 || m_strCode == CONCODE_EC2_2_05 )
		m_strRCCode.Format(_T("%s, %s"), m_strCode, m_strNationalAnnex);
	else
		m_strRCCode = m_strCode;

	m_bBeam = rData.bBeam;
	m_bColm = rData.bColumn;
	m_bBrce = rData.bBrace;
	m_bWall = rData.bWall;
	m_bSlab = rData.bSlab;
	m_bMat = rData.bMat;

	if(m_strCode != CONCODE_EC2_04 && m_strCode != CONCODE_IS456_2000 )
	{
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_SLAB_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_MAT_CHK)->ShowWindow(SW_HIDE);

        if ( m_strCode == CONCODE_EC2_2_05 )
		{
			GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_COLM_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_BRCE_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_WALL_CHK)->ShowWindow(SW_HIDE);
		}

	}
    if ( m_strCode == CONCODE_IS456_2000 )
	{
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_SLAB_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_MAT_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CON_BAR_MIN_SECT_WALL_CHK)->ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);
}

int CDgnConBarMinSectSizeDlg::GetDataStatus(CString strCode)
{
    if ( strCode==CONCODE_IS456_2000 ) { return 1; }
    if ( strCode==CONCODE_ACI318_08 )  { return 1; } // Seungjun '151126 PMS.5166 ACI318-08, 11 단면제한 옵션 추가
    if ( strCode==CONCODE_ACI318_11 )  { return 1; }
    if ( strCode==CONCODE_ACI318_14 )  { return 1; }
    if ( strCode==CONCODE_ACI318M_14) { return 1; }
    if ( strCode==CONCODE_ACI318_19 ) { return 1; }
    if ( strCode==CONCODE_ACI318M_19) { return 1; }
	if ( strCode==CONCODE_ACI318_25 ) { return 1; }
    if ( strCode==CONCODE_ACI318M_25) { return 1; }
    if ( strCode==CONCODE_NSCP_2015 )  { return 1; }
	if ( strCode==CONCODE_NTC_DCEC_2017 ) { return 1; }
	if ( strCode==CONCODE_NTC_DCEC_2023 ) { return 1; }
    if ( strCode==CONCODE_EC2_04 && m_strNationalAnnex==_LS(IDS_DGN_ITALY) ) { return 1; }// Eurocode2:04, Italy
    
	return 0;
}

void CDgnConBarMinSectSizeDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnConBarMinSectSizeDlg::OnOK() 
{
	UpdateData(TRUE);

	T_DCON_D rData;
	rData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDcon())	m_pDoc->m_pAttrCtrl->GetDcon(rData);
	else									rData.DesignCode = m_strCode;

	rData.bBeam = m_bBeam;
	rData.bColumn = m_bColm;
	rData.bBrace = m_bBrce;
	rData.bWall = m_bWall;
	rData.bSlab = m_bSlab;
	rData.bMat = m_bMat;

	// Save RC Control Data at DB.
	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDcon(rData);
	if(bCheck)  CDialogMove::OnOK();
}

// National Annex :: int to CString!!
CString CDgnConBarMinSectSizeDlg::ConvertNationalAnnexbyCString(int nNationalAnnex)
{
	CString strNationalAnnex = _T("");
 
#if defined(_CIVIL)
	if      (nNationalAnnex == 0) strNationalAnnex  = _LS(IDS_DGN_RECOMMENDED);
	else if (nNationalAnnex == 1) strNationalAnnex  = _LS(IDS_DGN_UK);
	else if (nNationalAnnex == 2) strNationalAnnex  = _LS(IDS_DGN_ITALY);
	else
	{
		ASSERT(0);
		return _LS(IDS_DGN_RECOMMENDED); // Default _T("Recommended")
	}
#elif defined(_MGEN)
	strNationalAnnex = CNationalAnnexTool::ConvertNationalAnnex(nNationalAnnex);
#endif

	return strNationalAnnex;
}