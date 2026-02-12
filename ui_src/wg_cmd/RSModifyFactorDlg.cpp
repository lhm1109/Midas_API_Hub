#include "stdafx.h"
#include "wg_cmd.h"
#include "RSModifyFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRSModifyFactorDlg::CRSModifyFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRSModifyFactorDlg::IDD, pParent)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	// Rsmf가 정의되어 있으면 체크온
	m_bUseModFactor = pDoc->m_pAttrCtrl->GetCountRsmf() > 0;
}

void CRSModifyFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_USE_RSMF_CHK, m_bUseModFactor);
	DDX_Control(pDX, IDC_CMD_GRID, m_grid);
}

BEGIN_MESSAGE_MAP(CRSModifyFactorDlg, CDialogMove)
	ON_BN_CLICKED(IDC_USE_RSMF_CHK, OnUseFactorChk)
END_MESSAGE_MAP()

BOOL CRSModifyFactorDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_grid.Initialize();

	Data2Grid();

	return TRUE;
}
void CRSModifyFactorDlg::OnOK()
{
	if(!Grid2Data())
		return;

	CDialogMove::OnOK();
}

void CRSModifyFactorDlg::OnUseFactorChk()
{
	UpdateData();
	m_grid.RemoveRecords();
	Data2Grid();
}

void CRSModifyFactorDlg::Data2Grid()
{
	CArray<T_RSMF_K,T_RSMF_K> aRsmfK;
	CArray<T_RSMF_D,T_RSMF_D&> aRsmfD;
	if(m_bUseModFactor)
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		const double dC_default = pDoc->m_pAttrCtrl->GetDefaultC4Rsmf();
		pDoc->m_pAttrCtrl->GetSplcKeyList(aRsmfK);
		aRsmfD.SetSize(aRsmfK.GetSize());
		for(int i=0; i<aRsmfK.GetSize(); i++)
		{
			if(!pDoc->m_pAttrCtrl->GetRsmf(aRsmfK[i],aRsmfD[i]))
			{
				aRsmfD[i].dC = dC_default;
			}
		}
	}

	m_grid.SetData(aRsmfK,aRsmfD);
}

BOOL CRSModifyFactorDlg::Grid2Data()
{
	UpdateData();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(m_bUseModFactor)
	{
		if (pDoc->m_pAttrCtrl2->IsSemeMethod_KISTEC_Series())
		{
			IUsageCounter::Use(_T("SPE_RSFK"));
		}
		else if (pDoc->m_pAttrCtrl2->IsSemeMethod_MOE_Series())
		{
			IUsageCounter::Use(_T("SPE_RSFM"));
		}

		CArray<T_RSMF_K,T_RSMF_K> aRsmfK;
		CArray<T_RSMF_D,T_RSMF_D&> aRsmfD;
		m_grid.GetData(aRsmfK, aRsmfD);
		if(!pDoc->m_pDataCtrl->AddRsmf(aRsmfK,aRsmfD))
			return FALSE;
	}
	else
	{
		// 체크가 꺼져 있으면 삭제
		CArray<T_RSMF_K,T_RSMF_K> aRsmfK;
		pDoc->m_pAttrCtrl->GetRsmfKeyList(aRsmfK);
		if(!pDoc->m_pDataCtrl->DelRsmf(aRsmfK))
			return FALSE;
	}

	return TRUE;
}