//////////
// CmdAutoLoadCombImperfectionLoadDlg.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombImperfectionLoadDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\ImperfectionLoadTool.h"
#include "..\wg_db\UndoCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\EditData.h"
#include "..\wg_db\DataCtrl.h"
#include "..\wg_db\AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombImperfectionLoadDlgdialog
CCmdAutoLoadCombImperfectionLoadDlg::CCmdAutoLoadCombImperfectionLoadDlg(LPCTSTR strCode, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadCombImperfectionLoadDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Grid.SetCode(strCode);
}

CCmdAutoLoadCombImperfectionLoadDlg::~CCmdAutoLoadCombImperfectionLoadDlg()
{

}

void CCmdAutoLoadCombImperfectionLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombImperfectionLoadDlg)	
	//DDX_Control(pDX, IDC_DGN_RATING_PARAM_STEEL_UNIT, m_TensSteelUnit);
	DDX_Control(pDX, IDC_IMPERFECTION_L_GRID, m_Grid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdAutoLoadCombImperfectionLoadDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombImperfectionLoadDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCmdAutoLoadCombImperfectionLoadDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SetControl();
	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdAutoLoadCombImperfectionLoadDlg::SetControl()
{
	//m_dAlwTendonStress.SetUnitType(D_UNITSYS_BASE_STRESS);  
	m_Grid.Initialize();	
}

void CCmdAutoLoadCombImperfectionLoadDlg::OnOk() 
{
	if(!Dlg2Data())
	{
		return;
	}

	CDialogMove::OnOK();
}

BOOL CCmdAutoLoadCombImperfectionLoadDlg::CheckDataLCase(int nAnalType, UINT uiKey) 
{
	T_STLD_D stldData;
	T_SPLC_D splcData;
	T_THIS_D thisData;
	T_MVLD_D mvldData;
	T_MVLDjp_D mvldjpData;
	T_MVLDch_D mvldchData;
	T_MVLDid_D mvldidData;
	T_MVLDbs_D mvldbsData;
	T_MVLDfr_D mvldfrData;
	T_MVLDtr_D mvldtrData;
	T_SMLC_D smlcData;
	T_SGLD_D sgldData;
	T_ESPC_D espcData;
	T_GILC_D gilcData;
	
	BOOL bError = FALSE;

	switch(nAnalType)
	{
	case D_LCOM_STATIC: 
		if (!m_pDoc->m_pAttrCtrl->GetStld(uiKey, stldData)) 
			bError = TRUE;
		break;
	case D_LCOM_SPECTRUM:
		if (!m_pDoc->m_pAttrCtrl->GetSplc(uiKey, splcData)) 
			bError = TRUE;
		break;
	case D_LCOM_HISTORY:
		if (!m_pDoc->m_pAttrCtrl->GetThis(uiKey, thisData)) 
			bError = TRUE;
		break;
	case D_LCOM_MOVING:
		{
			// 코드에 따른 방식으로 수정 
			T_MVCD_D DataMvcd;
			if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
			switch (DataMvcd.nCodeType)
			{
			case D_MOVE_CODE_NONE:
				break;
			case D_MOVE_CODE_AASHTO_STAN:
			case D_MOVE_CODE_AASHTO_LRFD:
			case D_MOVE_CODE_TAIWAN:
			case D_MOVE_CODE_KOREA:
			case D_MOVE_CODE_CANADA:
			case D_MOVE_CODE_PENDOT:
			case D_MOVE_CODE_EURO_BS:
			case D_MOVE_CODE_RUSSIA:
			case D_MOVE_CODE_KOREA_LRFD_2011:
			case D_MOVE_CODE_AUSTRALIA:
			case D_MOVE_CODE_POLAND:
			case D_MOVE_CODE_SOUTH_AFRICA:
			case D_MOVE_CODE_JAPAN_RAIL:
			case D_MOVE_CODE_NEWZEALAND:
			case D_MOVE_CODE_BRAZIL:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvld(uiKey, mvldData)) 
						bError = TRUE;
				}
				break;
			case D_MOVE_CODE_CHINA:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldch(uiKey, mvldchData)) 
						bError = TRUE;
				}
				break;
			case D_MOVE_CODE_JAPAN:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldjp(uiKey, mvldjpData)) 
						bError = TRUE;
				}
				break;
			case D_MOVE_CODE_INDIA:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldid(uiKey, mvldidData)) 
						bError = TRUE;
				}
				break;
			case D_MOVE_CODE_BS:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldbs(uiKey, mvldbsData)) 
						bError = TRUE;
				}
				break;
			case D_MOVE_CODE_FRANCE:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldfr(uiKey, mvldfrData)) 
						bError = TRUE;
				}
				break;
			case D_MOVE_CODE_TRANS:
				{
					if (!m_pDoc->m_pAttrCtrl->GetMvldtr(uiKey, mvldtrData)) 
						bError = TRUE;
				}
				break;
			default: ASSERT(0);
				break;
			}
		}
		break;
	case D_LCOM_SETTLEMENT:
		if (!m_pDoc->m_pAttrCtrl->GetSmlc(uiKey, smlcData)) 
			bError = TRUE;
		break;
	case D_LCOM_STAGE:
		if (!m_pDoc->m_pAttrCtrl->GetSgld(uiKey, sgldData))
			bError = TRUE;
		break;
	case D_LCOM_ECCENSPEC:
		if (!m_pDoc->m_pAttrCtrl->GetEspc(uiKey, espcData)) 
			bError = TRUE;
		break;
	case D_LCOM_GRIDMODEL:
		if (!m_pDoc->m_pAttrCtrl->GetGilc(uiKey, gilcData)) 
			bError = TRUE;
		break;
	case D_LCOM_CRANE:
		if (!m_pDoc->m_pAttrCtrl->IsExistCranBraking()) 
			bError = TRUE;
		break;
	default:
		bError = TRUE;
	}

	if(bError) return FALSE;

	return TRUE;
}

BOOL CCmdAutoLoadCombImperfectionLoadDlg::CheckData(_LCOM_IMPF& Data) 
{
	T_STLD_D StldD;
	if(!m_pDoc->m_pAttrCtrl->GetStld(Data.LCase1K, StldD))
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_IMP_LCOM_ER_L1));
		return FALSE;
	}

	if(!CheckDataLCase(Data.nAnalType, Data.LCase2K) )
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_IMP_LCOM_ER_L2));
		return FALSE;
	}

	if(Data.nDir < 0 || Data.nDir > 1)
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_IMP_LCOM_ER_DIR));
		return FALSE;
	}
	return TRUE;
}

BOOL CCmdAutoLoadCombImperfectionLoadDlg::Dlg2Data() 
{
	m_Grid.GetData(m_aData);

	for (int i = 0; i < m_aData.GetSize(); i++)
	{
		if(!CheckData(m_aData[i])) return FALSE;
	}

	return TRUE;
}

void CCmdAutoLoadCombImperfectionLoadDlg::Data2Dlg() 
{
	m_Grid.SetData(m_aData);
}

void CCmdAutoLoadCombImperfectionLoadDlg::GetData( CArray<_LCOM_IMPF, _LCOM_IMPF&>& aData )
{
	aData.RemoveAll();
	for (int i = 0; i < m_aData.GetSize(); i++)
	{
		aData.Add(m_aData[i]);
	}
}

void CCmdAutoLoadCombImperfectionLoadDlg::SetData( CArray<_LCOM_IMPF, _LCOM_IMPF&>& aData )
{
	m_aData.RemoveAll();
	for (int i = 0; i < aData.GetSize(); i++)
	{
		m_aData.Add(aData[i]);
	}
}

