#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConRrtrDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDgnConRrtrDlg::CDgnConRrtrDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConRrtrDlg::IDD, pParent)
{
	m_nOption = 0;
	//Set Init
	m_nPunchReinforcement = 0;
	m_nShearReinforcement = 0;

	m_arIDCtrl.Add(IDC_DGN_CON_RRTR_PUNCH_1_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_RRTR_PUNCH_1_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_RRTR_PUNCH_1_UNIT);
	m_arIDCtrl.Add(IDC_DGN_CON_RRTR_PUNCH_2_EDIT);
	m_arIDCtrl.Add(IDC_DGN_CON_RRTR_PUNCH_2_STATIC);
	m_arIDCtrl.Add(IDC_DGN_CON_RRTR_PUNCH_2_UNIT);
	


	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_1_EDIT);
	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_1_STATIC);
	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_1_UNIT);
	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_1_UNIT2);
	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_2_EDIT);
	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_2_STATIC);
	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_2_UNIT);
	m_arIDCtr2.Add(IDC_DGN_CON_RRTR_SHEAR_2_UNIT2);
}

void CDgnConRrtrDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_DGN_CON_RRTR_AR_RDO, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CON_RRTR_PUNCH_1_RADIO, m_nPunchReinforcement);
	DDX_Radio(pDX, IDC_DGN_CON_RRTR_SHEAR_1_RADIO, m_nShearReinforcement);

	DDX_Control(pDX, IDC_DGN_CON_RRTR_PUNCH_1_EDIT, m_ValueEdit[0]);
	DDX_Control(pDX, IDC_DGN_CON_RRTR_PUNCH_2_EDIT, m_ValueEdit[1]);
	DDX_Control(pDX, IDC_DGN_CON_RRTR_SHEAR_1_EDIT, m_ValueEdit[2]);
	DDX_Control(pDX, IDC_DGN_CON_RRTR_SHEAR_2_EDIT, m_ValueEdit[3]);

	DDX_Control(pDX, IDC_DGN_CON_RRTR_PUNCH_1_UNIT, m_ValueUnit[0]);
	DDX_Control(pDX, IDC_DGN_CON_RRTR_PUNCH_2_UNIT, m_ValueUnit[1]);
	DDX_Control(pDX, IDC_DGN_CON_RRTR_SHEAR_1_UNIT, m_ValueUnit[2]);
	DDX_Control(pDX, IDC_DGN_CON_RRTR_SHEAR_2_UNIT, m_ValueUnit[3]);

	DDX_Control(pDX, IDC_DGN_CON_RRTR_SHEAR_1_UNIT2, m_ValueUnit2[0]);
	DDX_Control(pDX, IDC_DGN_CON_RRTR_SHEAR_2_UNIT2, m_ValueUnit2[1]);
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CDgnConRrtrDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConRrtrDlg)
	ON_BN_CLICKED(IDC_DGN_CON_RRTR_PUNCH_1_RADIO, OnPunchType)
	ON_BN_CLICKED(IDC_DGN_CON_RRTR_PUNCH_2_RADIO, OnPunchType)
	ON_BN_CLICKED(IDC_DGN_CON_RRTR_SHEAR_1_RADIO, OnShearType)
	ON_BN_CLICKED(IDC_DGN_CON_RRTR_SHEAR_2_RADIO, OnShearType)
	ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)

	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


BOOL CDgnConRrtrDlg::OnInitDialog()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	m_ValueEdit[0].SetUnitType(D_UNITSYS_BASE_AREA);
	m_ValueEdit[1].SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ValueEdit[2].SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ValueEdit[3].SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_ValueUnit[0].SetUnitType(D_UNITSYS_BASE_AREA);
	m_ValueUnit[1].SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ValueUnit[2].SetUnitType(D_UNITSYS_BASE_AREA);
	m_ValueUnit[3].SetUnitType(D_UNITSYS_BASE_AREA);

	m_ValueUnit2[0].SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_ValueUnit2[1].SetUnitType(D_UNITSYS_BASE_LENGTH);

	T_RRTR_D RrtrData;
	RrtrData.Initialize();
	double dAswArea = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_AREA, RrtrData.dAswArea);
	double dStep = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_LENGTH, RrtrData.dStep);
	double dXaxis = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_LENGTH, RrtrData.dXaxis);
	double dYaxis = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_LENGTH, RrtrData.dYaxis);

	m_ValueEdit[0].SetEditUnit(dAswArea);
	m_ValueEdit[1].SetEditUnit(dStep);
	m_ValueEdit[2].SetEditUnit(dXaxis);
	m_ValueEdit[3].SetEditUnit(dYaxis);

	OnPunchType();
	OnShearType();
	return TRUE;
}

void CDgnConRrtrDlg::OnDgnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConRrtrDlg::OnDgnExecute()
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

			if (m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp))
				rSelKey.Add(key);
		}

		T_RRTR_D mData;
		mData.Initialize();
		if (m_nOption == 0)	// add/replace
		{
			mData.bPunchReinforcement = m_nPunchReinforcement == 1;
			mData.bShearReinforcement = m_nShearReinforcement == 1;
			mData.dAswArea = m_ValueEdit[0].GetEditValue();
			mData.dStep = m_ValueEdit[1].GetEditValue();
			mData.dXaxis = m_ValueEdit[2].GetEditValue();
			mData.dYaxis = m_ValueEdit[3].GetEditValue();

			if (rSelKey.GetSize() > 0)
			{
				m_pDoc->m_pDataCtrl->AddRrtr(rSelKey, mData);
			}
			else
			{
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_Plate_Con_only), MB_OK);
			}
		}
		else
		{
			m_pDoc->m_pDataCtrl->DelRrtr(rSelKey);
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}

void CDgnConRrtrDlg::OnPunchType()
{
	UpdateData(TRUE);
	if (m_nPunchReinforcement == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arIDCtrl, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_arIDCtrl, TRUE);
	}
}

void CDgnConRrtrDlg::OnShearType()
{
	UpdateData(TRUE);
	if (m_nShearReinforcement == 0)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arIDCtr2, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_arIDCtr2, TRUE);
	}
}