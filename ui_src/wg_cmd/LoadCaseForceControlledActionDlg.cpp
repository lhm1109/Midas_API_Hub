#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCaseForceControlledActionDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLoadCaseForceControlledActionDlg::CLoadCaseForceControlledActionDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CLoadCaseForceControlledActionDlg::IDD, pParent)
{

}

void CLoadCaseForceControlledActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_GRID, m_grid);
}

BEGIN_MESSAGE_MAP(CLoadCaseForceControlledActionDlg, CDialogMove)
END_MESSAGE_MAP()

BOOL CLoadCaseForceControlledActionDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	m_grid.Initialize();

	Data2Grid();

	return TRUE;
}
void CLoadCaseForceControlledActionDlg::OnOK()
{
	// 저장하자
	CArray<T_LFCA_K,T_LFCA_K> aLfcaK;
	CArray<T_LFCA_D,T_LFCA_D&> aLfcaD;
	m_grid.GetData(aLfcaK, aLfcaD);
	CDBDoc::GetDocPoint()->m_pDataCtrl->AddLfca(aLfcaK,aLfcaD);

	CDialogMove::OnOK();
}

void CLoadCaseForceControlledActionDlg::Data2Grid()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_LFCA_K,T_LFCA_K> aLfcaK;
	CArray<T_LFCA_D,T_LFCA_D&> aLfcaD;
	CArray<CString,CString&> aName;

	// Seismic Load
	CArray<T_PSLL_K,T_PSLL_K> aPsllK;
	pDoc->m_pAttrCtrl->GetPsllKeyList(aPsllK);
	for(int i=0; i<aPsllK.GetSize(); i++)
	{
		T_LFCA_K LfcaK = {0};
		LfcaK.key.type = D_LFCA_TYPE_PSLL;
		LfcaK.key.objK = aPsllK[i];

		T_LFCA_D LfcaD;
		pDoc->m_pAttrCtrl->GetLfcaDefault(LfcaK, LfcaD);

		T_STLD_D StldD;
		pDoc->m_pAttrCtrl->GetStld(LfcaK.key.objK, StldD);

		aLfcaK.Add(LfcaK);
		aLfcaD.Add(LfcaD);
		aName.Add(StldD.LoadCaseName);
	}

	// Spectrum Load
	CArray<T_SPLC_K,T_SPLC_K> aSplcK;
	pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	for(int i=0; i<aSplcK.GetSize(); i++)
	{
		T_LFCA_K LfcaK = {0};
		LfcaK.key.type = D_LFCA_TYPE_SPLC;
		LfcaK.key.objK = aSplcK[i];

		T_LFCA_D LfcaD;
		pDoc->m_pAttrCtrl->GetLfcaDefault(LfcaK, LfcaD);

		T_SPLC_D SplcD;
		pDoc->m_pAttrCtrl->GetSplc(LfcaK.key.objK, SplcD);

		aLfcaK.Add(LfcaK);
		aLfcaD.Add(LfcaD);
		aName.Add(SplcD.LoadCaseName);
	}
	m_grid.SetData(aLfcaK, aLfcaD, aName);
}