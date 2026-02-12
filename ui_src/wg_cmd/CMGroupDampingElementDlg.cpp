// CMGroupDampingElementDlg.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGroupDampingElementDlg.h"
#include "CMGroupDampingOptionDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_Editdata.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingElementDlg dialog


CCMGroupDampingElementDlg::CCMGroupDampingElementDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGroupDampingElementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGroupDampingElementDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aDirectCalcDefRdo.Add(IDC_CMD_GRDP_DEFAULT_DIRECT_RDO);
	m_aDirectCalcDefRdo.Add(IDC_CMD_GRDP_DEFAULT_CALC_RDO);

	m_aFreqPeriodDefRdo.Add(IDC_CMD_GRDP_DEFAULT_FREQ_RDO);
	m_aFreqPeriodDefRdo.Add(IDC_CMD_GRDP_DEFAULT_PERIOD_RDO);

	m_aGroupTypeRdo.Add(IDC_CMD_GRDP_MATL_RDO);
	m_aGroupTypeRdo.Add(IDC_CMD_GRDP_STRUCTURE_RDO);
	m_aGroupTypeRdo.Add(IDC_CMD_GRDP_BOUNDARY_RDO);

	m_aDirectCalcRdo.Add(IDC_CMD_GRDP_DIRECT_RDO);
	m_aDirectCalcRdo.Add(IDC_CMD_GRDP_CALC_RDO);

	m_aFreqPeriodRdo.Add(IDC_CMD_GRDP_FREQ_RDO);
	m_aFreqPeriodRdo.Add(IDC_CMD_GRDP_PERIOD_RDO);

	m_aMaterialDirectRdo.Add(IDC_CMD_GRDP_DAMP_DEFAULT_RDO);
	m_aMaterialDirectRdo.Add(IDC_CMD_GRDP_DAMP_DIRECT_RDO);
	
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_FREQ_PERI_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_FREQ_MODE1_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_FREQ_MODE2_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_FREQ_RDO);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_PERIOD_RDO);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_F1_EDT);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_F2_EDT);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_T1_EDT);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_T2_EDT);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_SET_DEF_BTN);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_DAMP_RATIO_FRM_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_DAMP_DEFAULT_RDO);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_DAMP_DIRECT_RDO);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_DAMP_RATIO_FRM2_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_DAMP_RATIO_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_DAMP_RATIO2_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_MODE1_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_MODE2_STC);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_X1_EDT);
	m_aCtrlFreqDamp.Add(IDC_CMD_GRDP_X2_EDT);

	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_COEF_CALC_STC);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_MODE1_STC);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_MODE2_STC);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_FREQ_RDO);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_PERIOD_RDO);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_DAMP_RATIO_STC);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_DAMP_RATIO2_STC);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_F1_EDT);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_F2_EDT);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_T1_EDT);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_T2_EDT);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_X1_EDT);
	m_aCtrlDefCoefCalc.Add(IDC_CMD_GRDP_DEFAULT_X2_EDT);

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
}


void CCMGroupDampingElementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGroupDampingElementDlg)
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_MASS_CHK     , m_chkMassPropDef  );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_STIFF_CHK    , m_chkStiffPropDef );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_DIRECT_RM_EDT, m_edtDirectMassDef);
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_DIRECT_RK_EDT, m_edtDirectStifDef);
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_CALC_RM_EDT  , m_edtCalcMassDef  );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_CALC_RK_EDT  , m_edtCalcStifDef  );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_F1_EDT       , m_edtFreqDef1     );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_F2_EDT       , m_edtFreqDef2     );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_T1_EDT       , m_edtPeriodDef1   );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_T2_EDT       , m_edtPeriodDef2   );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_X1_EDT       , m_edtDampingDef1  );
	DDX_Control(pDX, IDC_CMD_GRDP_DEFAULT_X2_EDT       , m_edtDampingDef2  );

	DDX_Control(pDX, IDC_CMD_GRDP_NAME_CMB      , m_cmbGroupName    );
	DDX_Control(pDX, IDC_CMD_GRDP_MASS_CHK      , m_chkMassProp   );
	DDX_Control(pDX, IDC_CMD_GRDP_STIFF_CHK     , m_chkStiffProp   );
	DDX_Control(pDX, IDC_CMD_GRDP_DIRECT_RM_EDT , m_edtDirectMass   );
	DDX_Control(pDX, IDC_CMD_GRDP_DIRECT_RK_EDT , m_edtDirectStif   );
	DDX_Control(pDX, IDC_CMD_GRDP_CALC_RM_EDT   , m_edtCalcMass     );
	DDX_Control(pDX, IDC_CMD_GRDP_CALC_RK_EDT   , m_edtCalcStif     );
	DDX_Control(pDX, IDC_CMD_GRDP_F1_EDT        , m_edtFreq1        );
	DDX_Control(pDX, IDC_CMD_GRDP_F2_EDT        , m_edtFreq2        );
	DDX_Control(pDX, IDC_CMD_GRDP_T1_EDT        , m_edtPeriod1      );
	DDX_Control(pDX, IDC_CMD_GRDP_T2_EDT        , m_edtPeriod2      );
	DDX_Control(pDX, IDC_CMD_GRDP_X1_EDT        , m_edtDamping1     );
	DDX_Control(pDX, IDC_CMD_GRDP_X2_EDT        , m_edtDamping2     );

	DDX_Control(pDX, IDC_CMD_GRDP_LST, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGroupDampingElementDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGroupDampingElementDlg)
	ON_BN_CLICKED(IDC_CMD_GRDP_DEFAULT_DIRECT_RDO, OnChangeDefaultDirectCalcRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_DEFAULT_CALC_RDO, OnChangeDefaultDirectCalcRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_DEFAULT_FREQ_RDO, OnChangeDefaultFreqPeriodRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_DEFAULT_PERIOD_RDO, OnChangeDefaultFreqPeriodRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_DEFAULT_MASS_CHK, OnChkDefaultMassStiffBtn)
	ON_BN_CLICKED(IDC_CMD_GRDP_DEFAULT_STIFF_CHK, OnChkDefaultMassStiffBtn)
	ON_EN_CHANGE(IDC_CMD_GRDP_DEFAULT_F1_EDT, OnChangeDefaultAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_DEFAULT_F2_EDT, OnChangeDefaultAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_DEFAULT_T1_EDT, OnChangeDefaultAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_DEFAULT_T2_EDT, OnChangeDefaultAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_DEFAULT_X1_EDT, OnChangeDefaultAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_DEFAULT_X2_EDT, OnChangeDefaultAutoCalcCoef)

	ON_CBN_SELCHANGE(IDC_CMD_GRDP_NAME_CMB, OnSelChangeNameCmb)
	ON_BN_CLICKED(IDC_CMD_GRDP_MATL_RDO, OnCmdGroupTypeRadio)
	ON_BN_CLICKED(IDC_CMD_GRDP_STRUCTURE_RDO, OnCmdGroupTypeRadio)
	ON_BN_CLICKED(IDC_CMD_GRDP_BOUNDARY_RDO, OnCmdGroupTypeRadio)
	ON_BN_CLICKED(IDC_CMD_GRDP_DIRECT_RDO, OnChangeDirectCalcRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_CALC_RDO, OnChangeDirectCalcRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_FREQ_RDO, OnChangeFreqPeriodRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_PERIOD_RDO, OnChangeFreqPeriodRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_MASS_CHK, OnChkMassStiffBtn)
	ON_BN_CLICKED(IDC_CMD_GRDP_STIFF_CHK, OnChkMassStiffBtn)
	ON_EN_CHANGE(IDC_CMD_GRDP_F1_EDT, OnChangeAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_F2_EDT, OnChangeAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_T1_EDT, OnChangeAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_T2_EDT, OnChangeAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_X1_EDT, OnChangeAutoCalcCoef)
	ON_EN_CHANGE(IDC_CMD_GRDP_X2_EDT, OnChangeAutoCalcCoef)
	ON_BN_CLICKED(IDC_CMD_GRDP_SET_DEF_BTN, OnSetDefaultDataBtn)
	ON_BN_CLICKED(IDC_CMD_GRDP_DAMP_DEFAULT_RDO, OnChangeMaterialDirectRdo)
	ON_BN_CLICKED(IDC_CMD_GRDP_DAMP_DIRECT_RDO, OnChangeMaterialDirectRdo)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_GRDP_LST, OnItemchangedCmdList)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_SEL_OPT_BTN, OnCmdSelectOptionBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingElementDlg message handlers

BOOL CCMGroupDampingElementDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_edtDirectMassDef.SetUnitType(0);
	m_edtDirectStifDef.SetUnitType(0);
	m_edtCalcMassDef  .SetUnitType(0);
	m_edtCalcStifDef  .SetUnitType(0);
	m_edtFreqDef1     .SetUnitType(0);
	m_edtFreqDef2     .SetUnitType(0);
	m_edtPeriodDef1   .SetUnitType(0);
	m_edtPeriodDef2   .SetUnitType(0);
	m_edtDampingDef1  .SetUnitType(0);
	m_edtDampingDef2  .SetUnitType(0);
	
	m_edtDirectMass .SetUnitType(0);
	m_edtDirectStif .SetUnitType(0);
	m_edtCalcMass   .SetUnitType(0);
	m_edtCalcStif   .SetUnitType(0);
	m_edtFreq1      .SetUnitType(0);
	m_edtFreq2      .SetUnitType(0);
	m_edtPeriod1    .SetUnitType(0);
	m_edtPeriod2    .SetUnitType(0);
	m_edtDamping1   .SetUnitType(0);
	m_edtDamping2   .SetUnitType(0);

	SetHeaderTitle();
	
	if(!m_pDoc->m_pAttrCtrl->GetGrdp(m_Data))
	{
		m_Data.Initialize();
	}

	Data2Dlg();
	EnableDisableCtrls();
	EnableDisableDefaultCtrls();

	ChangeControlsText();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGroupDampingElementDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	m_Data.bExistElement = TRUE;
	if (!m_pDoc->m_pDataCtrl->AddGrdp(m_Data)) return;

	CDialogMove::OnOK();
}

void CCMGroupDampingElementDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMGroupDampingElementDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	T_GRDP_BASE GrdpBase;
	if (!Dlg2BaseData(GrdpBase)) return;

	if(GrdpBase.nGrupType == 2 && GrdpBase.GrupBngrKey == 0)
	{
		CArray<T_MATL_K, T_MATL_K> aMatlK;
		m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
		int nMatl = aMatlK.GetSize();
		for(int i = 0; i < nMatl; i++)
		{
			GrdpBase.GrupBngrKey = aMatlK[i];
			int nIndex = IsUsedGrupData(GrdpBase.nGrupType, GrdpBase.GrupBngrKey);
			
			T_MATL_D MatlD;
			if(!m_pDoc->m_pAttrCtrl->GetMatl(GrdpBase.GrupBngrKey, MatlD))      {  ASSERT(0);		return ;	}
			GrdpBase.dDampingRatio = GrdpBase.dDampingRatio2 = MatlD.dDampingRatio;
			
			T_THIS_D ThisD;     ThisD.Initialize();
			ThisD.PropDamping.nFreqPeriod = GrdpBase.nFreqPeriod + 1;
			ThisD.PropDamping.bMassProp   = GrdpBase.bMassProp;
			ThisD.PropDamping.bStiffProp  = GrdpBase.bStiffProp;
			ThisD.PropDamping.dFreqOne    = GrdpBase.dFreqOne;
			ThisD.PropDamping.dFreqTwo    = GrdpBase.dFreqTwo;
			ThisD.PropDamping.dPeriodOne  = GrdpBase.dPeriodOne;
			ThisD.PropDamping.dPeriodTwo  = GrdpBase.dPeriodTwo;
			ThisD.PropDamping.dDampingOne = GrdpBase.dDampingRatio;
			ThisD.PropDamping.dDampingTwo = GrdpBase.dDampingRatio2;
			
			if(!m_pDoc->m_pEditData->AutoCalcThisCoef(ThisD.PropDamping, ThisD.PropDamping.dRm, ThisD.PropDamping.dRk))    ThisD.PropDamping.Initialize();
			GrdpBase.dMassCoef = ThisD.PropDamping.dRm;
			GrdpBase.dStifCoef = ThisD.PropDamping.dRk;

			if (nIndex == -1) m_Data.aGrupDamping.Add(GrdpBase);
			else m_Data.aGrupDamping.SetAt(nIndex, GrdpBase);
		}
	}
	else
	{ 
		// add or modify
		int nIndex = IsUsedGrupData(GrdpBase.nGrupType, GrdpBase.GrupBngrKey);
		if (nIndex == -1) m_Data.aGrupDamping.Add(GrdpBase);
		else m_Data.aGrupDamping.SetAt(nIndex, GrdpBase);
	}
	
	SetGroupNameCombo();
	MakeItemEx();
}

void CCMGroupDampingElementDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here

	// 현재 선택된 아이템 가져오기 
	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Group_Damping));
		return;
	}
	if (iItem >= m_Data.aGrupDamping.GetSize())
	{
		ASSERT(0);
		return;
	}
	// delete
	m_Data.aGrupDamping.RemoveAt(iItem);

	SetGroupNameCombo();
	MakeItemEx();

	int nCount = m_wndList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	

}

void CCMGroupDampingElementDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Group_Damping));
		return;
	}
	if (iItem >= m_Data.aGrupDamping.GetSize())
	{
		ASSERT(0);
		return;
	}

	// modify
	T_GRDP_BASE GrdpBase;
	if (!Dlg2BaseData(GrdpBase)) return;
	if(GrdpBase.GrupBngrKey == 0)   return;
	m_Data.aGrupDamping.SetAt(iItem, GrdpBase);

	SetGroupNameCombo();
	MakeItemEx();	

	m_wndList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMGroupDampingElementDlg::OnSelChangeNameCmb() 
{
	int nGroupType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
	if(nGroupType == 0)
	{
		int ix = m_cmbGroupName.GetCurSel();
		if (ix == CB_ERR)    return;
		
		T_MATL_K MatlK = m_cmbGroupName.GetItemData(ix);
		if(MatlK == 0)
		{
			m_edtDamping1.SetWindowText(_T("--"));
			m_edtDamping2.SetWindowText(_T("--"));
			
			CDlgUtil::CtrlRadioSetCheck(this, m_aDirectCalcRdo, 1);   // Calculate from Modal Damping
			OnChangeDirectCalcRdo();
			m_edtCalcMass.SetWindowText(_T("--"));
			m_edtCalcStif.SetWindowText(_T("--"));
			
			CDlgUtil::CtrlRadioSetCheck(this, m_aMaterialDirectRdo, 0);   // Use Material Data
			OnChangeMaterialDirectRdo();

			GetDlgItem(IDC_CMD_BTN_ADD)->SetWindowText(_LS(IDS_CMD_DEFINE));
			return;
		}
		
		int nMaterialDirect = 0;
		CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
		if(nMaterialDirect == 0)
		{
			T_MATL_D MatlD;
			if(!m_pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))   MatlD.Initialize();
			m_edtDamping1.SetEditUnit(MatlD.dDampingRatio);
			m_edtDamping2.SetEditUnit(MatlD.dDampingRatio);
		}
	}
	GetDlgItem(IDC_CMD_BTN_ADD)->SetWindowText(_LS(IDS_CMD_ADD));

	EnableDisableCtrls();
	AutoCalcCoef();
}

void CCMGroupDampingElementDlg::OnCmdGroupTypeRadio() 
{
	// TODO: Add your control notification handler code here

	int nDirectCalc;
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcRdo, nDirectCalc);
	
	if(nDirectCalc != 0)
	{
		int nGroupType;
		CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
		
		if(nGroupType != 0)
		{
			int nMaterialDirect = 0;
			CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
			
			if(nMaterialDirect == 0)
			{
				CDlgUtil::CtrlRadioSetCheck(this, m_aMaterialDirectRdo, 1);
				OnChangeMaterialDirectRdo();
			}
		}
	}
	SetGroupNameCombo();
}

void CCMGroupDampingElementDlg::OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	T_GRDP_BASE BaseData;
	BaseData.Initialize();

	int iItem = m_wndList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem != -1) 
	{
		if (iItem < m_Data.aGrupDamping.GetSize())
			BaseData = m_Data.aGrupDamping.GetAt(iItem);
		BaseData2Dlg(BaseData);
	}

	*pResult = 0;
}

void CCMGroupDampingElementDlg::OnCmdSelectOptionBtn()
{
	CCMGroupDampingOptionDlg Dlg(0, &m_Data);
	Dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// User Defiend Init
#define COLCOUNT   6

void CCMGroupDampingElementDlg::SetGroupNameCombo()
{
	m_cmbGroupName.ResetContent();

	int nType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nType);

	CStringArray aGrupName;
	CArray<UINT, UINT> aGrupKey;
	if (nType == 0)  // Material
	{
		T_MATL_D MatlD;
		CArray<T_MATL_K, T_MATL_K> aMatlK;
		m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
		int nNum = aMatlK.GetSize();
		for (int i=0; i<nNum ; i++)
		{
			if (!m_pDoc->m_pAttrCtrl->GetMatl(aMatlK[i], MatlD)) continue;
			aGrupName.Add(MatlD.Name);
			aGrupKey.Add(aMatlK[i]);
		}
		if(nNum)
		{
			int nIndex = m_cmbGroupName.AddString(_LS(IDS_CMD_ALL_MATL_DATA));
			m_cmbGroupName.SetItemData(nIndex, 0);
		}
	}
	else if (nType == 1)       // Structure Group
	{
		T_GRUP_D GrupD;
		CArray<T_GRUP_K, T_GRUP_K> aGrupK;
		m_pDoc->m_pAttrCtrl->GetGrupKeyList(aGrupK);
		int nNum = aGrupK.GetSize();
		for (int i=0; i<nNum ; i++)
		{
			//if (IsUsedGrupData(0, aGrupK[i])) continue;;
			if (!m_pDoc->m_pAttrCtrl->GetGrup(aGrupK[i], GrupD)) continue;
			aGrupName.Add(GrupD.GroupName);
			aGrupKey.Add(aGrupK[i]);
		}
	}
	else if (nType == 2)  // Boundary Group
	{
		T_BNGR_D BngrD;
		CArray<T_BNGR_K, T_BNGR_K> aBngrK;
		m_pDoc->m_pAttrCtrl->GetBngrKeyList(aBngrK);
		int nNum = aBngrK.GetSize();
		for (int i=0; i<nNum ; i++)
		{
			//if (IsUsedGrupData(1, aBngrK[i])) continue;
			if (!m_pDoc->m_pAttrCtrl->GetBngr(aBngrK[i], BngrD)) continue;
			aGrupName.Add(BngrD.GroupName);
			aGrupKey.Add(aBngrK[i]);
		}
	}
	else ASSERT(0);

	int nIndex;
	for (int i=0; i<aGrupName.GetSize(); i++)
	{
		nIndex = m_cmbGroupName.AddString(aGrupName[i]);
		m_cmbGroupName.SetItemData(nIndex, aGrupKey[i]);
	}
	m_cmbGroupName.SetCurSel(0);
	OnSelChangeNameCmb();
}

int CCMGroupDampingElementDlg::IsUsedGrupData(int nType, UINT GrupK)
{
	for (int i=0; i<m_Data.aGrupDamping.GetSize(); i++)
	{
		if (m_Data.aGrupDamping[i].nGrupType == nType && 
				m_Data.aGrupDamping[i].GrupBngrKey == GrupK) return i;
	}
	return -1;
}

void CCMGroupDampingElementDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_CMD_NAME), _LS(IDS_CMD_GRDP_GROUP_TYPE), _LS(IDS_CMD_GRDP_GROUP_RATIO1), _LS(IDS_CMD_GRDP_GROUP_RATIO2),
											 _LS(IDS_CMD_GRDP_GROUP_ALPHA), _LS(IDS_CMD_GRDP_GROUP_BETA)};
	int nColWidth[] = { 60, 45, 50, 50, 70, 70 };

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_wndList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_wndList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_wndList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

// Data를 List에 넣을 수 있도록 String으로 변환
CString CCMGroupDampingElementDlg::DataToStr(int i, int nIndex, T_GRDP_BASE &Data)
{
	CString str;

	if (i == 0) str = GetGroupName(Data.nGrupType, Data.GrupBngrKey);
	else if (i == 1) str = GetGroupType(Data.nGrupType);
	else if (i == 2 || i == 3)
	{
		if(Data.nDirectCalc == 0)  str = _T("--");
		else if (i == 2) str.Format(_T("%g"), Data.dDampingRatio);
		else if (i == 3) str.Format(_T("%g"), Data.dDampingRatio2);
	}
	else if (i == 4)
	{
		if(Data.bMassProp)     str.Format(_T("%g"), Data.dMassCoef);
		else    str = _T("--");
	}
	else if (i == 5)
	{
		if(Data.bStiffProp)    str.Format(_T("%g"), Data.dStifCoef);
		else    str = _T("--");
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

CString CCMGroupDampingElementDlg::GetGroupName(int nGrupType, UINT GrupK)
{
	if (nGrupType == 0)
	{
		T_GRUP_D GrupD;
		if (m_pDoc->m_pAttrCtrl->GetGrup(GrupK, GrupD))
			return GrupD.GroupName;
	}
	else if (nGrupType == 1)
	{
		T_BNGR_D BngrD;
		if (m_pDoc->m_pAttrCtrl->GetBngr(GrupK, BngrD))
			return BngrD.GroupName;
	}
	else if (nGrupType == 2)
	{
		T_MATL_D MatlD;
		if (m_pDoc->m_pAttrCtrl->GetMatl(GrupK, MatlD))
			return MatlD.Name;
	}
	else ASSERT(0);

	return _T("");
}

CString CCMGroupDampingElementDlg::GetGroupType(int nGrupType)
{
	if (nGrupType == 0) return _LS(IDS_CMD_GRDP_TYPE_STRUCTURE);
	else if (nGrupType == 1) return _LS(IDS_CMD_GRDP_TYPE_BOUNDARY);
	else if (nGrupType == 2) return _LS(IDS_CMD_CSCS__Material);
	return _T("");
}

// List에 Item 넣기(한줄씩)
BOOL CCMGroupDampingElementDlg::InsertItem(int nIndex, T_GRDP_BASE &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_wndList.GetItemCount();

	lvitem.iItem = nIndex;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, nIndex, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;
		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)nIndex;
			nItem = m_wndList.InsertItem(&lvitem);
		}
		else m_wndList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	return TRUE;
}

// List에 Item 채워넣기(전체)
void CCMGroupDampingElementDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_wndList.DeleteAllItems();

	int nItemCount = m_Data.aGrupDamping.GetSize();
	if (nItemCount == 0) return;

	for (int nIndex = 0 ; nIndex < nItemCount ; nIndex++)
	{
		InsertItem(nIndex, m_Data.aGrupDamping[nIndex]);
	}
}

void CCMGroupDampingElementDlg::BaseData2Dlg(T_GRDP_BASE& rBaseData)
{
	int nGroupType = rBaseData.nGrupType+1;
	if(rBaseData.nGrupType == 2)    nGroupType = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aGroupTypeRdo, nGroupType);
	OnCmdGroupTypeRadio();

//  SetGroupNameCombo();
//  if (rBaseData.GrupBngrKey == 0) return;
	for (int i=0; i<m_cmbGroupName.GetCount(); i++)
	{
		if (rBaseData.GrupBngrKey == m_cmbGroupName.GetItemData(i))
		{
			m_cmbGroupName.SetCurSel(i);
			break;
		}
	}
	OnSelChangeNameCmb();

	m_chkMassProp.SetCheck(rBaseData.bMassProp);
	m_chkStiffProp.SetCheck(rBaseData.bStiffProp);

	CDlgUtil::CtrlRadioSetCheck(this, m_aDirectCalcRdo, rBaseData.nDirectCalc);
	if(rBaseData.nDirectCalc == 0) // Direct Specification
	{
		m_edtDirectMass.SetEditUnit(rBaseData.dMassCoef);
		m_edtDirectStif.SetEditUnit(rBaseData.dStifCoef);
		m_edtCalcMass.SetEditUnit(0.0);
		m_edtCalcStif.SetEditUnit(0.0);
	}
	else if(rBaseData.nDirectCalc == 1) // Calculate from Modal Damping
	{
		m_edtDirectMass.SetEditUnit(0.0);
		m_edtDirectStif.SetEditUnit(0.0);
		m_edtCalcMass.SetEditUnit(rBaseData.dMassCoef);
		m_edtCalcStif.SetEditUnit(rBaseData.dStifCoef);
	}
//  OnChangeDirectCalcRdo();
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aFreqPeriodRdo, rBaseData.nFreqPeriod);
	m_edtFreq1.SetEditUnit(rBaseData.dFreqOne);
	m_edtFreq2.SetEditUnit(rBaseData.dFreqTwo);
	m_edtPeriod1.SetEditUnit(rBaseData.dPeriodOne);
	m_edtPeriod2.SetEditUnit(rBaseData.dPeriodTwo);
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aMaterialDirectRdo, rBaseData.nDampInputType);
	OnChangeMaterialDirectRdo();

	int nMaterialDirect = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
	if(nMaterialDirect == 1)
	{
		m_edtDamping1.SetEditUnit(rBaseData.dDampingRatio);
		m_edtDamping2.SetEditUnit(rBaseData.dDampingRatio2);
	}

	EnableDisableCtrls();
}

BOOL CCMGroupDampingElementDlg::Dlg2BaseData(T_GRDP_BASE& rBaseData)
{
	int nGroupType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
	if(nGroupType == 0)   rBaseData.nGrupType = 2;
	else                  rBaseData.nGrupType = nGroupType-1;

	int ix = m_cmbGroupName.GetCurSel();
	if (ix == CB_ERR)
	{
		AfxMessageBox(_LS(IDS_CMD_GRDP_No_Selected_Name_of_Group));
		return FALSE;
	}
	rBaseData.GrupBngrKey = m_cmbGroupName.GetItemData(ix);

	rBaseData.bMassProp = m_chkMassProp.GetCheck();
	rBaseData.bStiffProp = m_chkStiffProp.GetCheck();

	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcRdo, rBaseData.nDirectCalc);
	if(rBaseData.nDirectCalc == 0) // Direct Specification
	{
		rBaseData.dMassCoef = m_edtDirectMass.GetEditValue();
		rBaseData.dStifCoef = m_edtDirectStif.GetEditValue();
	}
	else if(rBaseData.nDirectCalc == 1) // Calculate from Modal Damping
	{
		rBaseData.dMassCoef = m_edtCalcMass.GetEditValue();
		rBaseData.dStifCoef = m_edtCalcStif.GetEditValue();
	}

	CDlgUtil::CtrlRadioGetCheck(this, m_aFreqPeriodRdo, rBaseData.nFreqPeriod);
	rBaseData.dFreqOne = m_edtFreq1.GetEditValue();
	rBaseData.dFreqTwo = m_edtFreq2.GetEditValue();
	rBaseData.dPeriodOne = m_edtPeriod1.GetEditValue();
	rBaseData.dPeriodTwo = m_edtPeriod2.GetEditValue();

	CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, rBaseData.nDampInputType);

	rBaseData.dDampingRatio = m_edtDamping1.GetEditValue();
	rBaseData.dDampingRatio2 = m_edtDamping2.GetEditValue();

	return TRUE;
}

void CCMGroupDampingElementDlg::Data2Dlg()
{
	m_chkMassPropDef.SetCheck(m_Data.bMassPropDefault);
	m_chkStiffPropDef.SetCheck(m_Data.bStiffPropDefault);
	CDlgUtil::CtrlRadioSetCheck(this, m_aDirectCalcDefRdo, m_Data.nDirectCalcDefault);
	if(m_Data.nDirectCalcDefault == 0) // Direct Specification
	{
		m_edtDirectMassDef.SetEditUnit(m_Data.dMassCoefDefault);
		m_edtDirectStifDef.SetEditUnit(m_Data.dStifCoefDefault);
		m_edtCalcMassDef.SetEditUnit(0.0);
		m_edtCalcStifDef.SetEditUnit(0.0);
	}
	else if(m_Data.nDirectCalcDefault == 1) // Calculate from Modal Damping
	{
		m_edtDirectMassDef.SetEditUnit(0.0);
		m_edtDirectStifDef.SetEditUnit(0.0);
		m_edtCalcMassDef.SetEditUnit(m_Data.dMassCoefDefault);
		m_edtCalcStifDef.SetEditUnit(m_Data.dStifCoefDefault);
	}

	CDlgUtil::CtrlRadioSetCheck(this, m_aFreqPeriodDefRdo, m_Data.nFreqPeriodDefault);
	m_edtFreqDef1.SetEditUnit(m_Data.dFreqOneDefault);
	m_edtFreqDef2.SetEditUnit(m_Data.dFreqTwoDefault);
	m_edtPeriodDef1.SetEditUnit(m_Data.dPeriodOneDefault);
	m_edtPeriodDef2.SetEditUnit(m_Data.dPeriodTwoDefault);
	m_edtDampingDef1.SetEditUnit(m_Data.dDampingDefault);
	m_edtDampingDef2.SetEditUnit(m_Data.dDampingDefault2);
	
	MakeItemEx();

//   if (m_Data.aGrupDamping.GetSize() > 0)
//   {
//     m_wndList.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
//     BaseData2Dlg(m_Data.aGrupDamping[0]);
//   }
//   else
//   {
		T_GRDP_BASE BaseData;
		BaseData.Initialize();
		BaseData.nGrupType = 2;
		BaseData.nDirectCalc = 1;
		BaseData2Dlg(BaseData);
//  }
}

BOOL CCMGroupDampingElementDlg::Dlg2Data()
{
	m_Data.bMassPropDefault = m_chkMassPropDef.GetCheck();
	m_Data.bStiffPropDefault = m_chkStiffPropDef.GetCheck();
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcDefRdo, m_Data.nDirectCalcDefault);
	if(m_Data.nDirectCalcDefault == 0) // Direct Specification
	{
		m_Data.dMassCoefDefault = m_edtDirectMassDef.GetEditValue();
		m_Data.dStifCoefDefault = m_edtDirectStifDef.GetEditValue();
	}
	else if(m_Data.nDirectCalcDefault == 1) // Calculate from Modal Damping
	{
		m_Data.dMassCoefDefault = m_edtCalcMassDef.GetEditValue();
		m_Data.dStifCoefDefault = m_edtCalcStifDef.GetEditValue();
	}
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aFreqPeriodDefRdo, m_Data.nFreqPeriodDefault);
	m_Data.dFreqOneDefault = m_edtFreqDef1.GetEditValue();
	m_Data.dFreqTwoDefault = m_edtFreqDef2.GetEditValue();
	m_Data.dPeriodOneDefault = m_edtPeriodDef1.GetEditValue();
	m_Data.dPeriodTwoDefault = m_edtPeriodDef2.GetEditValue();
	m_Data.dDampingDefault = m_edtDampingDef1.GetEditValue();
	m_Data.dDampingDefault2 = m_edtDampingDef2.GetEditValue();

	return TRUE;
}

void CCMGroupDampingElementDlg::OnChangeDefaultDirectCalcRdo()
{
	EnableDisableDefaultCtrls();  
	AutoCalcDefaultCoef();  // Mass and Stiffness Coefficient Calculation
}

void CCMGroupDampingElementDlg::OnChangeDefaultFreqPeriodRdo()
{
	EnableDisableDefaultCtrls();
	AutoCalcDefaultCoef();  // Mass and Stiffness Coefficient Calculation
}

void CCMGroupDampingElementDlg::OnChkDefaultMassStiffBtn()
{
	EnableDisableDefaultCtrls();
	AutoCalcDefaultCoef();  // Mass and Stiffness Coefficient Calculation
}

void CCMGroupDampingElementDlg::OnChangeDefaultAutoCalcCoef()
{
	AutoCalcDefaultCoef();
}

void CCMGroupDampingElementDlg::AutoCalcDefaultCoef()
{
	UpdateData(TRUE);
	T_THIS_D rData;
	rData.Initialize();
	rData.nDampingType            = 2;
	int nType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcDefRdo, nType);
	rData.PropDamping.nCoefDefine = nType + 1;
	if(rData.nDampingType != 2 || rData.PropDamping.nCoefDefine != 2) return;

	CDlgUtil::CtrlRadioGetCheck(this, m_aFreqPeriodDefRdo, nType);
	rData.PropDamping.nFreqPeriod = nType + 1;
	rData.PropDamping.bMassProp   = m_chkMassPropDef.GetCheck();
	rData.PropDamping.bStiffProp  = m_chkStiffPropDef.GetCheck();
	rData.PropDamping.dFreqOne    = m_edtFreqDef1.GetEditValue();
	rData.PropDamping.dFreqTwo    = m_edtFreqDef2.GetEditValue();
	rData.PropDamping.dPeriodOne  = m_edtPeriodDef1.GetEditValue();
	rData.PropDamping.dPeriodTwo  = m_edtPeriodDef2.GetEditValue();
	rData.PropDamping.dDampingOne = m_edtDampingDef1.GetEditValue();
	rData.PropDamping.dDampingTwo = m_edtDampingDef2.GetEditValue();   
	
	if(m_pDoc->m_pEditData->AutoCalcThisCoef(rData.PropDamping, rData.PropDamping.dRm, rData.PropDamping.dRk))
	{
		m_edtCalcMassDef.SetEditUnit(rData.PropDamping.dRm);
		m_edtCalcStifDef.SetEditUnit(rData.PropDamping.dRk);
	}
	else
	{
		if(rData.PropDamping.bMassProp)  m_edtCalcMassDef.SetWindowText(_T("--"));
		if(rData.PropDamping.bStiffProp) m_edtCalcStifDef.SetWindowText(_T("--"));
	}
}

void CCMGroupDampingElementDlg::EnableDisableDefaultCtrls()
{
	BOOL bMassDef  = m_chkMassPropDef.GetCheck();
	BOOL bStiffDef = m_chkStiffPropDef.GetCheck();
	int nDirectCalcDef = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcDefRdo, nDirectCalcDef);
	int nFreqPeriodDef = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aFreqPeriodDefRdo, nFreqPeriodDef);

	GetDlgItem(IDC_CMD_GRDP_DEFAULT_DIRECT_RDO)->EnableWindow(bMassDef || bStiffDef);
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_CALC_RDO)->EnableWindow(bMassDef || bStiffDef);
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_CALC_STC)->EnableWindow(bMassDef || bStiffDef);

	GetDlgItem(IDC_CMD_GRDP_DEFAULT_DIRECT_RM_EDT)->EnableWindow(nDirectCalcDef == 0 && bMassDef);
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_DIRECT_RK_EDT)->EnableWindow(nDirectCalcDef == 0 && bStiffDef);
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_CALC_RM_EDT)->EnableWindow(nDirectCalcDef == 1 && bMassDef);
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_CALC_RK_EDT)->EnableWindow(nDirectCalcDef == 1 && bStiffDef);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDefCoefCalc, nDirectCalcDef == 1 && (bMassDef || bStiffDef));
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_F1_EDT)->EnableWindow(nDirectCalcDef == 1 && nFreqPeriodDef == 0 && (bMassDef || bStiffDef));
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_F2_EDT)->EnableWindow(nDirectCalcDef == 1 && nFreqPeriodDef == 0 && (bMassDef && bStiffDef));
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_T1_EDT)->EnableWindow(nDirectCalcDef == 1 && nFreqPeriodDef == 1 && (bMassDef || bStiffDef));
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_T2_EDT)->EnableWindow(nDirectCalcDef == 1 && nFreqPeriodDef == 1 && (bMassDef && bStiffDef));
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_X1_EDT)->EnableWindow(nDirectCalcDef == 1 && (bMassDef || bStiffDef));  
	GetDlgItem(IDC_CMD_GRDP_DEFAULT_X2_EDT)->EnableWindow(nDirectCalcDef == 1 && (bMassDef && bStiffDef));
}

void CCMGroupDampingElementDlg::OnChangeDirectCalcRdo()
{
	int nDirectCalc;
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcRdo, nDirectCalc);
	if(nDirectCalc != 0)
	{
		int nGroupType;
		CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
		
		if(nGroupType != 0)
		{
			int nMaterialDirect = 0;
			CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
			
			if(nMaterialDirect == 0)
			{
				CDlgUtil::CtrlRadioSetCheck(this, m_aMaterialDirectRdo, 1);
				OnChangeMaterialDirectRdo();
			}
		}
	}

	EnableDisableCtrls();  
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CCMGroupDampingElementDlg::OnChangeFreqPeriodRdo()
{
	EnableDisableCtrls();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CCMGroupDampingElementDlg::OnChkMassStiffBtn()
{
	EnableDisableCtrls();
	AutoCalcCoef();  // Mass and Stiffness Coefficient Calculation
}

void CCMGroupDampingElementDlg::OnChangeAutoCalcCoef()
{
	AutoCalcCoef();
}

void CCMGroupDampingElementDlg::OnChangeMaterialDirectRdo()
{
	int nMaterialDirect = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aMaterialDirectRdo, nMaterialDirect);
	m_edtDamping1.SetReadOnly(nMaterialDirect == 0);
	m_edtDamping2.SetReadOnly(nMaterialDirect == 0);

	if(nMaterialDirect == 1)
	{
		m_edtDamping1.SetEditUnit(m_edtDampingDef1.GetEditValue());
		m_edtDamping2.SetEditUnit(m_edtDampingDef2.GetEditValue());
		return;
	}
	
	int nGroupType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
	if(nGroupType != 0)     return ;
	
	int ix = m_cmbGroupName.GetCurSel();
	if (ix == CB_ERR)    return;

	T_MATL_K MatlK = m_cmbGroupName.GetItemData(ix);
	if(MatlK == 0)
	{
		m_edtDamping1.SetWindowText(_T("--"));
		m_edtDamping2.SetWindowText(_T("--"));
		return;
	}
	
	T_MATL_D MatlD;
	if(!m_pDoc->m_pAttrCtrl->GetMatl(MatlK, MatlD))   MatlD.Initialize();
	m_edtDamping1.SetEditUnit(MatlD.dDampingRatio);
	m_edtDamping2.SetEditUnit(MatlD.dDampingRatio);
	AutoCalcCoef();
}

void CCMGroupDampingElementDlg::OnSetDefaultDataBtn()
{
	int nType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aFreqPeriodDefRdo, nType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aFreqPeriodRdo, nType);

	if(nType == 0)
	{
		m_edtFreq1.SetEditUnit(m_edtFreqDef1.GetEditValue());
		m_edtFreq2.SetEditUnit(m_edtFreqDef2.GetEditValue());
	}
	else
	{
		m_edtPeriod1.SetEditUnit(m_edtPeriodDef1.GetEditValue());
		m_edtPeriod2.SetEditUnit(m_edtPeriodDef2.GetEditValue());
	}
	OnChangeFreqPeriodRdo();
}

void CCMGroupDampingElementDlg::AutoCalcCoef()
{
	UpdateData(TRUE);
	T_THIS_D rData;
	rData.Initialize();
	rData.nDampingType            = 2;
	int nType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcRdo, nType);
	rData.PropDamping.nCoefDefine = nType + 1;
	if(rData.nDampingType != 2 || rData.PropDamping.nCoefDefine != 2) return;

	int ix = m_cmbGroupName.GetCurSel();
	if (ix == CB_ERR)    return;
	T_MATL_K MatlK = m_cmbGroupName.GetItemData(ix);
	if(MatlK == 0)    return;
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aFreqPeriodRdo, nType);
	rData.PropDamping.nFreqPeriod = nType + 1;
	rData.PropDamping.bMassProp   = m_chkMassProp.GetCheck();
	rData.PropDamping.bStiffProp  = m_chkStiffProp.GetCheck();
	rData.PropDamping.dFreqOne    = m_edtFreq1.GetEditValue();
	rData.PropDamping.dFreqTwo    = m_edtFreq2.GetEditValue();
	rData.PropDamping.dPeriodOne  = m_edtPeriod1.GetEditValue();
	rData.PropDamping.dPeriodTwo  = m_edtPeriod2.GetEditValue();
	rData.PropDamping.dDampingOne = m_edtDamping1.GetEditValue();
	rData.PropDamping.dDampingTwo = m_edtDamping2.GetEditValue();   
	
	if(m_pDoc->m_pEditData->AutoCalcThisCoef(rData.PropDamping, rData.PropDamping.dRm, rData.PropDamping.dRk))
	{
		m_edtCalcMass.SetEditUnit(rData.PropDamping.dRm);
		m_edtCalcStif.SetEditUnit(rData.PropDamping.dRk);
	}
	else
	{
		if(rData.PropDamping.bMassProp)  m_edtCalcMass.SetWindowText(_T("--"));
		if(rData.PropDamping.bStiffProp) m_edtCalcStif.SetWindowText(_T("--"));
	}
}

void CCMGroupDampingElementDlg::EnableDisableCtrls()
{
	int nGroupType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aGroupTypeRdo, nGroupType);
	int ix = m_cmbGroupName.GetCurSel();
	if (ix == CB_ERR)    return;
	T_MATL_K MatlK = m_cmbGroupName.GetItemData(ix);

	BOOL bMass  = m_chkMassProp.GetCheck();
	BOOL bStiff = m_chkStiffProp.GetCheck();
	int nDirectCalc = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aDirectCalcRdo, nDirectCalc);
	int nFreqPeriod = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aFreqPeriodRdo, nFreqPeriod);

	GetDlgItem(IDC_CMD_GRDP_DIRECT_RDO)->EnableWindow(MatlK != 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_GRDP_CALC_RDO)->EnableWindow(bMass || bStiff);
	GetDlgItem(IDC_CMD_GRDP_CALC_STC)->EnableWindow(bMass || bStiff);
	
	GetDlgItem(IDC_CMD_GRDP_DIRECT_RM_EDT)->EnableWindow(nDirectCalc == 0 && bMass);
	GetDlgItem(IDC_CMD_GRDP_DIRECT_RK_EDT)->EnableWindow(nDirectCalc == 0 && bStiff);
	GetDlgItem(IDC_CMD_GRDP_CALC_RM_EDT)->EnableWindow(nDirectCalc == 1 && bMass);
	GetDlgItem(IDC_CMD_GRDP_CALC_RK_EDT)->EnableWindow(nDirectCalc == 1 && bStiff);

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlFreqDamp, nDirectCalc == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_GRDP_DAMP_DIRECT_RDO)->EnableWindow(nDirectCalc == 1 && MatlK != 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_GRDP_DAMP_DEFAULT_RDO)->EnableWindow(nDirectCalc == 1 && nGroupType  == 0 && (bMass || bStiff));

	GetDlgItem(IDC_CMD_GRDP_F1_EDT)->EnableWindow(nDirectCalc == 1 && nFreqPeriod == 0 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_GRDP_F2_EDT)->EnableWindow(nDirectCalc == 1 && nFreqPeriod == 0 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_GRDP_T1_EDT)->EnableWindow(nDirectCalc == 1 && nFreqPeriod == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_GRDP_T2_EDT)->EnableWindow(nDirectCalc == 1 && nFreqPeriod == 1 && (bMass && bStiff));
	GetDlgItem(IDC_CMD_GRDP_X1_EDT)->EnableWindow(nDirectCalc == 1 && (bMass || bStiff));
	GetDlgItem(IDC_CMD_GRDP_X2_EDT)->EnableWindow(nDirectCalc == 1 && (bMass && bStiff));

	GetDlgItem(IDC_CMD_BTN_MODIFY)->EnableWindow(MatlK != 0);
	GetDlgItem(IDC_CMD_BTN_DELETE)->EnableWindow(MatlK != 0);
}

void CCMGroupDampingElementDlg::ChangeControlsText()
{
#if defined(_CIVIL) && defined (_ORG)
	GetDlgItem(IDC_CMD_GRDP_DESC1_1_STC)->SetWindowText(_STR(_T("*. 'Default Values for Unspecified Elements and")));
	GetDlgItem(IDC_CMD_GRDP_DESC1_2_STC)->SetWindowText(_STR(_T("Boundaries' 설정은 Material Data / Group으로 정의되지")));
	GetDlgItem(IDC_CMD_GRDP_DESC1_3_STC)->SetWindowText(_STR(_T("않은 구조요소 및 경계에 적용됩니다.")));
	GetDlgItem(IDC_CMD_GRDP_DESC1_4_STC)->SetWindowText(_STR(_T(" ")));
	
	GetDlgItem(IDC_CMD_GRDP_DESC2_1_STC)->SetWindowText(_STR(_T("*. 절점은 Structure Group으로 설정한 감쇠를 반영하지")));
	GetDlgItem(IDC_CMD_GRDP_DESC2_2_STC)->SetWindowText(_STR(_T("않습니다. 절점에 정의된 Nodal Mass, Load to Mass 등")));
	GetDlgItem(IDC_CMD_GRDP_DESC2_3_STC)->SetWindowText(_STR(_T("은 'Default Values for Unspecified Elements and")));
	GetDlgItem(IDC_CMD_GRDP_DESC2_4_STC)->SetWindowText(_STR(_T("Boundaries' 에서 설정한 감쇠정수가 적용됨에 주의할 필")));
	GetDlgItem(IDC_CMD_GRDP_DESC2_5_STC)->SetWindowText(_STR(_T("요가 있습니다.")));

	GetDlgItem(IDC_CMD_GRDP_DESC3_1_STC)->SetWindowText(_STR(_T("*. Group Damping : Element Mass && Stiffness")));
	GetDlgItem(IDC_CMD_GRDP_DESC3_2_STC)->SetWindowText(_STR(_T("Proportional은 시간이력 해석에서 감쇠방법을 'Element")));
	GetDlgItem(IDC_CMD_GRDP_DESC3_3_STC)->SetWindowText(_STR(_T("&& Stiffness Proportional'을 선택 한 경우에만 적용됩니다.")));
#elif defined (_JP)
	GetDlgItem(IDC_CMD_GRDP_DESC1_4_STC)->SetWindowText(_STR(_T(" ")));
	GetDlgItem(IDC_CMD_GRDP_DESC2_5_STC)->SetWindowText(_STR(_T(" ")));
#endif  
}
