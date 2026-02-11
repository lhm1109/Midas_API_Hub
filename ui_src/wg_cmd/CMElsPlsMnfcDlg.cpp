// CMElsPlsMnfcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsMnfcDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMnfcDlg
CCMElsPlsMnfcDlg::CCMElsPlsMnfcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsMnfcDlg::IDD, pParent)
{
	m_nFibrAreaSize = 0;

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}

void CCMElsPlsMnfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_COVER_NY_EDT, m_edtnSectNum);
	DDX_Radio(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_FAREA_AUTO_SIZE_RDO, m_nFibrAreaSize);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NY_EDT, m_edtBeamDivNumNy);
	DDX_Control(pDX, IDC_WG_CMD_IEHC_FIBR_BEAM_CORE_NZ_EDT, m_edtBeamDivNumNz);
}

BEGIN_MESSAGE_MAP(CCMElsPlsMnfcDlg, CDialogMove)
END_MESSAGE_MAP()

BOOL CCMElsPlsMnfcDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (m_pDoc->m_pAttrCtrl2->ExistMnfc())
	{
		m_pDoc->m_pAttrCtrl2->GetMnfc(m_Data);
	}
	else
	{
		m_Data.Initialize();
	}
	Data2Dlg();

	return TRUE;
}

void CCMElsPlsMnfcDlg::Data2Dlg()
{
	m_edtnSectNum.SetEditUnit(m_Data.nSectNum);
	m_nFibrAreaSize = m_Data.nFibrAreaSize;
	m_edtBeamDivNumNy.SetEditUnit(m_Data.nBeamDivNumNy);
	m_edtBeamDivNumNz.SetEditUnit(m_Data.nBeamDivNumNz);

	UpdateData(FALSE);
}

BOOL CCMElsPlsMnfcDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.nSectNum      = m_edtnSectNum.GetEditValue();
	m_Data.nFibrAreaSize = m_nFibrAreaSize;
	m_Data.nBeamDivNumNy = m_edtBeamDivNumNy.GetEditValue();
	m_Data.nBeamDivNumNz = m_edtBeamDivNumNz.GetEditValue();

	return TRUE;
}

void CCMElsPlsMnfcDlg::OnOK()
{
	if(!Dlg2Data()) return;

	if(!m_pDoc->m_pDataCtrl->AddMnfc(m_Data)) return;
	CDialogMove::OnOK();
}