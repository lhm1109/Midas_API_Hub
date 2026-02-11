// ElemParamChangeRefVecDlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemParamChangeDlg.h"
#include "ElemParamChangeRefVecDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_main\wg_mainres2.h"

CElemParamChangeRefVecDlg::CElemParamChangeRefVecDlg(CWnd* pParent /*=NULL*/)
	: CDBDlgBase(CElemParamChangeRefVecDlg::IDD, pParent)
{
	m_pParent = (CElemParamChangeDlg*)pParent;
}


CElemParamChangeRefVecDlg::~CElemParamChangeRefVecDlg()
{
}

void CElemParamChangeRefVecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDBDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemParamChangeRefVecDlg)
	DDX_Control(pDX, IDC_TM_ELEM_PARAMETER_REF_VEC_EDT, m_wndSelectElemListEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CElemParamChangeRefVecDlg, CDBDlgBase)
	//{{AFX_MSG_MAP(CElemParamChangeRefVecDlg)
	//ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CElemParamChangeRefVecDlg::OnInitDialog()
{
	CDBDlgBase::OnInitDialog();

	m_wndSelectElemListEdit.Connect(SC_ID_ELEM, &m_wndSelectElemListEdit);
	GotoDlgCtrl(&m_wndSelectElemListEdit);

	// Select Single view mode
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(!pIGM) { ASSERT(0); return FALSE; }
	pIGM->SetModelingModeST(IG_STATE_INITALL);
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_SELECT_SINGLE,0));

	return TRUE;  // return TRUE unless you set the focus to a control
								// EXCEPTION: OCX Property Pages should return FALSE
}

void CElemParamChangeRefVecDlg::OnOK()
{
	CString strAverageVector = GetAverageVectorOfSelectedPlate();
	m_pParent->SetBetaVectorEdit(strAverageVector);
	
	CDBDlgBase::OnOK();
}

CString CElemParamChangeRefVecDlg::GetAverageVectorOfSelectedPlate()
{
	CString strAverageVector = _T("");

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(!pIGM) { ASSERT(0); return strAverageVector; }
	pIGM->GetSelectedElemKeyList(aElemK);

	double dPlateVector[3][3], dSumVector[3], dAveVector[3];
	memset(dSumVector,0,sizeof(double)*3);
	memset(dAveVector,0,sizeof(double)*3);
	T_ELEM_D ElemD;
	for(int i=0; i<aElemK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))       { ASSERT(0); continue; }
		if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))        { continue; }
		if(!m_pDoc->calcElemLocalVector(aElemK[i], dPlateVector)) { ASSERT(0); continue; }

		for(int j=0; j<3; j++)
			dSumVector[j] += dPlateVector[2][j];
	}

	if(!CMathFunc::mathNormalize(dSumVector,dAveVector)) { ASSERT(0); }
	strAverageVector.Format(_T("%g, %g, %g"), dAveVector[0],dAveVector[1],dAveVector[2]);

	return strAverageVector;
}

/////////////////////////////////////////////////////////////////////////////
// CPlateSelectEdit
const UINT rWM_SELECT_CTRL_UPDATE_TEXT = RegisterWindowMessage(_T("WM_SELECT_CTRL_UPDATE_TEXT"));
BEGIN_MESSAGE_MAP(CPlateSelectEdit, CEdit)
	//{{AFX_MSG_MAP(CSelectEdit)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(rWM_SELECT_CTRL_UPDATE_TEXT, OnUpdateText)
END_MESSAGE_MAP()

LRESULT CPlateSelectEdit::OnUpdateText(WPARAM wParam, LPARAM lParam)
{
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	T_ELEM_D ElemD;
	CArray<T_ELEM_K,T_ELEM_K> aElemK, aPlateK;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(!pIGM) { ASSERT(0); return 0L; }
	pIGM->GetSelectedElemKeyList(aElemK);

	aPlateK.RemoveAll();
	for(int i=0; i<aElemK.GetSize(); i++)
	{
		if(!pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD)) { ASSERT(0); continue; }
		if(!pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp)) continue;
		aPlateK.Add(aElemK[i]);
	}

	CString strText = _T("");
	int nPlateCnt = aPlateK.GetSize();
	if (nPlateCnt>0)
	{
		long *aNum = new long[nPlateCnt];
		for (int i = 0; i < nPlateCnt; i++)
			aNum[i] = aPlateK[i];
		qsort((void*)aNum, nPlateCnt, sizeof(long), CNumericOptimizer::comparei);

		CNumericOptimizer optimizer;
		strText = optimizer.Optimize(aNum, nPlateCnt);
	}

	SetWindowText(strText);

	return 0L;
}