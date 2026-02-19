#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConMndcDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDgnConMndcDlg::CDgnConMndcDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConMndcDlg::IDD, pParent)
{
	m_nOption = 0;
	m_nParametersType = 0;
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_COMPRESSION_GRP);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_TENSION_GRP);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_1_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_2_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_3_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_4_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_5_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_6_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_1_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_2_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_3_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_4_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_5_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_6_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_1_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_2_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_3_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_4_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_5_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_6_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_1_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_2_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_3_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_4_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_5_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_6_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_1_UNIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_1_UNIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_C_1_UNIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_1_UNIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_1_UNIT);
	m_arIDCtrl.Add(IDC_DGN_CON_MNDC_T_1_UNIT);

	m_arSigma2.Add(IDC_DGN_CON_MNDC_C_3_STATIC);
	m_arSigma2.Add(IDC_DGN_CON_MNDC_C_3_EDIT);
	m_arSigma2.Add(IDC_DGN_CON_MNDC_T_3_STATIC);
	m_arSigma2.Add(IDC_DGN_CON_MNDC_T_3_EDIT);

	m_arSigma0.Add(IDC_DGN_CON_MNDC_C_2_STATIC);
	m_arSigma0.Add(IDC_DGN_CON_MNDC_C_2_EDIT);
	m_arSigma0.Add(IDC_DGN_CON_MNDC_T_2_STATIC);
	m_arSigma0.Add(IDC_DGN_CON_MNDC_T_2_EDIT);
}

void CDgnConMndcDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_DGN_CON_MNDC_AR_RDO, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CON_MNDC_TYPE_1_RADIO, m_nParametersType);
	
	DDX_Control(pDX, IDC_DGN_CON_MNDC_DIAGRAM_TYPE_COMBOX, m_DiagramComb);

	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_1_EDIT, m_dCompression[0]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_2_EDIT, m_dCompression[1]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_3_EDIT, m_dCompression[2]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_4_EDIT, m_dCompression[3]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_5_EDIT, m_dCompression[4]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_6_EDIT, m_dCompression[5]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_6_EDIT, m_dCompression[5]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_1_EDIT, m_dTension[0]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_2_EDIT, m_dTension[1]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_3_EDIT, m_dTension[2]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_4_EDIT, m_dTension[3]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_5_EDIT, m_dTension[4]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_6_EDIT, m_dTension[5]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_1_UNIT, m_dSigmaUnit[0]); 
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_2_UNIT, m_dSigmaUnit[1]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_C_3_UNIT, m_dSigmaUnit[2]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_1_UNIT, m_dSigmaUnit[3]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_2_UNIT, m_dSigmaUnit[4]);
	DDX_Control(pDX, IDC_DGN_CON_MNDC_T_3_UNIT, m_dSigmaUnit[5]);
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CDgnConMndcDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConMndcDlg)
	ON_BN_CLICKED(IDC_DGN_CON_MNDC_TYPE_1_RADIO, OnMndcType)
	ON_BN_CLICKED(IDC_DGN_CON_MNDC_TYPE_2_RADIO, OnMndcType)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)

	ON_EN_KILLFOCUS(IDC_DGN_CON_MNDC_C_1_EDIT, OnUpdateEdit)
	ON_EN_KILLFOCUS(IDC_DGN_CON_MNDC_C_2_EDIT, OnUpdateEdit)
	ON_EN_KILLFOCUS(IDC_DGN_CON_MNDC_T_1_EDIT, OnUpdateEdit)
	ON_EN_KILLFOCUS(IDC_DGN_CON_MNDC_T_2_EDIT, OnUpdateEdit)

	ON_CBN_SELCHANGE(IDC_DGN_CON_MNDC_DIAGRAM_TYPE_COMBOX, OnSelChangeDiagramType)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


BOOL CDgnConMndcDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	for (int i = 0; i < 3; i++)
	{
		m_dCompression[i].SetUnitType(D_UNITSYS_BASE_STRESS);
		m_dCompression[i + 3].SetUnitType(D_UNITSYS_NONE);
		m_dTension[i].SetUnitType(D_UNITSYS_BASE_STRESS);
		m_dTension[i + 3].SetUnitType(D_UNITSYS_NONE);
		m_dSigmaUnit[i].SetUnitType(D_UNITSYS_BASE_STRESS);
		m_dSigmaUnit[i + 3].SetUnitType(D_UNITSYS_BASE_STRESS);
	}
	m_DiagramComb.ResetContent();
	m_DiagramComb.AddString(_LS(IDS_DGN_CONC_MNDC_TWO_LINE_STRESS_STRAIN));
	m_DiagramComb.AddString(_LS(IDS_DGN_CONC_MNDC_THREE_LINE_STRESS_STRAIN));
	m_DiagramComb.SetCurSel(0);
	CDlgUtil::CtrlEnableDisable(this, m_arSigma2, FALSE);
	OnMndcType();
	return TRUE;
}

void CDgnConMndcDlg::OnDgnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConMndcDlg::OnDgnExecute()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;  aSelKey.RemoveAll();
	CArray<T_ELEM_K, T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	// if Selected element is existed.
	if (aSelKey.GetSize() > 0)
	{
		for (int i = 0; i < aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			// Check Material.
			T_ELEM_D ElemD; ElemD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD))       
				continue;
			T_MATD_D MatlD; MatlD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatlD)) 
				continue;

			if (MatlD.Type != _T("C"))
				continue; 

			if (m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp) || m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp))
				rSelKey.Add(key);
		}

		T_MNDC_D mData;
		mData.Initialize();
		if (m_nOption == 0)	// add/replace
		{
			//if ((m_dk1 <= 0 || m_dk1 > 1) || (m_dk2 <= 0 || m_dk2 > 1) || (m_dk3 <= 0 || m_dk3 > 1) || (m_dk4 <= 0 || m_dk4 > 1))
			//{
			//	AfxMessageBox(_LS(IDS_DGN_SERV_STRESS_PARAMETER_ERROR));
			//	return;
			//}
			mData.nDiagramType = m_DiagramComb.GetCurSel();
			//mData.bCrackPossible = ((CButton*)GetDlgItem(IDC_DGN_CON_MNDC_CRACK_POSSIBLE_CHK))->GetCheck();
			mData.nParametersType = m_nParametersType;
			for (int i = 0; i < 6; i++)
			{
				mData.dCompression[i] = m_dCompression[i].GetEditValue();
				mData.dTension[i] = m_dTension[i].GetEditValue();
			}


			if (rSelKey.GetSize() > 0)
			{
				m_pDoc->m_pDataCtrl->AddMndc(rSelKey, mData);
			}
			else
			{
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only), MB_OK);
			}
		}
		else
		{
			m_pDoc->m_pDataCtrl->DelMndc(rSelKey);
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}

void CDgnConMndcDlg::OnSelChangeDiagramType()
{
	if (m_nParametersType == 0)
		return;
	CDlgUtil::CtrlEnableDisable(this, m_arSigma2, FALSE);
	CDlgUtil::CtrlEnableDisable(this, m_arSigma0, TRUE);
	if (m_DiagramComb.GetCurSel() == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arSigma0, FALSE);
	}
}

void CDgnConMndcDlg::OnUpdateEdit()
{
	CString strC, strT;
	if (m_DiagramComb.GetCurSel() == 0)
	{
		m_dTension[0].GetWindowText(strT);
		m_dCompression[0].GetWindowText(strC);
		m_dCompression[1].SetWindowText(strC);
		m_dTension[1].SetWindowText(strT);
	}
	m_dCompression[1].GetWindowText(strC);
	m_dTension[1].GetWindowText(strT);
	m_dCompression[2].SetWindowText(strC);
	m_dTension[2].SetWindowText(strT);
}

void CDgnConMndcDlg::OnMndcType()
{
	UpdateData(TRUE);
	if (m_nParametersType == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arIDCtrl, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_arIDCtrl, TRUE);
	}
	OnSelChangeDiagramType();
}

