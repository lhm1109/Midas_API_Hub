#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonChangeProfDlg.h"

#include "CMTendonTypeDlg.h"
#include "CMTendonDefDlg.h"
#include "ExportFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCMTendonChangeProfDlg::CCMTendonChangeProfDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMTendonChangeProfDlg::IDD, pParent)
{
	m_nParamType = 0;
	bName = FALSE;
	bProperty = FALSE;
	bPropertyGrp = FALSE;
	bTypicalTD = FALSE;
	m_aSelectedProfiles.RemoveAll();


	m_aCtrlList.RemoveAll();
	m_aCtrlList.Add(IDC_CMD_TENDON_STATIC);
	m_aCtrlList.Add(IDC_CMD_TENDON_SELECTED_STATIC);
	m_aCtrlList.Add(IDC_CMD_SEL_BUTTON);
	m_aCtrlList.Add(IDC_CMD_UNSEL_BUTTON);
	m_aCtrlList.Add(IDC_CMD_ALL_LIST);
	m_aCtrlList.Add(IDC_CMD_SELECTED_LIST);

	m_aCtrlParamTypeRadio.RemoveAll();
	m_aCtrlParamTypeRadio.Add(IDC_CMD_TENDON_NAME_RADIO);
	m_aCtrlParamTypeRadio.Add(IDC_CMD_TENDON_PROPERTY_RADIO);
	m_aCtrlParamTypeRadio.Add(IDC_CMD_TENDON_PROPERTY_GROUP_RADIO);
	m_aCtrlParamTypeRadio.Add(IDC_CMD_TYPICAL_TENDON_RADIO);
	m_aCtrlParamTypeRadio.Add(IDC_CMD_TENDON_GROUP_RADIO);
	m_aCtrlParamTypeRadio.Add(IDC_CMD_TRANS_LENGTH_RADIO);
	m_aCtrlParamTypeRadio.Add(IDC_CMD_DEBONDED_LENGTH_RADIO);

	m_aCtrlTdName.RemoveAll();
	m_aCtrlTdName.Add(IDC_CMD_NAME_STATIC);
	m_aCtrlTdName.Add(IDC_CMD_NAME_EDIT);
	m_aCtrlTdName.Add(IDC_CMD_SUFFIX_STATIC);
	m_aCtrlTdName.Add(IDC_CMD_SUFFIX_EDIT);
	m_aCtrlTdName.Add(IDC_CMD_SUFFIX_EXAMPLE_STATIC);

	m_aCtrlTdProperty.RemoveAll();
	m_aCtrlTdProperty.Add(IDC_CMD_PROPERTY_STATIC);
	m_aCtrlTdProperty.Add(IDC_CMD_PROPERTY_COMBO);
	m_aCtrlTdProperty.Add(IDC_CMD_PROPERTY_BUTTON);
	
	m_aCtrlTypicalTd.RemoveAll();
	m_aCtrlTypicalTd.Add(IDC_CMD_NO_TENDONS_STATIC);
	m_aCtrlTypicalTd.Add(IDC_CMD_NO_TENDONS_EDIT);
	//m_aCtrlTypicalTd.Add(IDC_CMD_NO_TENDONS_SPIN);

	m_aCtrlTdGroup.RemoveAll();
	m_aCtrlTdGroup.Add(IDC_CMD_GROUP_STATIC);
	m_aCtrlTdGroup.Add(IDC_CMD_GROUP_COMBO);
	m_aCtrlTdGroup.Add(IDC_CMD_GROUP_BUTTON);

	m_aCtrlTDebonded.RemoveAll();
	m_aCtrlTDebonded.Add(IDC_CMD_DEBONDED_LENGTH_STC);
	m_aCtrlTDebonded.Add(IDC_CMD_DEBONDED_LENGTH_BEGIN_STC);
	m_aCtrlTDebonded.Add(IDC_CMD_DEBONDED_LENGTH_BEGIN_EDIT);
	m_aCtrlTDebonded.Add(IDC_CMD_DEBONDED_LENGTH_END_STC);
	m_aCtrlTDebonded.Add(IDC_CMD_DEBONDED_LENGTH_END_EDIT);
	m_aCtrlTDebonded.Add(IDC_CMD_DEBONDED_LENGTH_END_UNIT);

	m_aCtrlTransfer.RemoveAll();
	m_aCtrlTransfer.Add(IDC_CMD_TRANSFER_LENGTH_STC);
	m_aCtrlTransfer.Add(IDC_CMD_TRANSFER_LENGTH_BEGIN_STC);
	m_aCtrlTransfer.Add(IDC_CMD_TRANSFER_LENGTH_BEGIN_EDIT);
	m_aCtrlTransfer.Add(IDC_CMD_TRANSFER_LENGTH_END_STC);
	m_aCtrlTransfer.Add(IDC_CMD_TRANSFER_LENGTH_END_EDIT);
	m_aCtrlTransfer.Add(IDC_CMD_TRANSFER_LENGTH_END_UNIT);

    m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

}

void CCMTendonChangeProfDlg::SetSelectedProfiles(CArray<UINT, UINT> &aSelectedK)
{
	m_aSelectedProfiles.Copy(aSelectedK);
}

void CCMTendonChangeProfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_PROPERTY_COMBO, m_cobxProperty);
	DDX_Control(pDX, IDC_CMD_GROUP_COMBO, m_cobxGroup);
	DDX_Control(pDX, IDC_CMD_SELECTED_LIST, m_listSelProfiles);
	DDX_Control(pDX, IDC_CMD_ALL_LIST, m_listAllProfiles);
	DDX_Radio(pDX, IDC_CMD_TENDON_NAME_RADIO, m_nParamType);

	DDX_Control(pDX, IDC_CMD_NO_TENDONS_EDIT, m_editNum);

	DDX_Control(pDX, IDC_CMD_TRANSFER_LENGTH_BEGIN_EDIT, m_editTransBegin);
	DDX_Control(pDX, IDC_CMD_TRANSFER_LENGTH_END_EDIT,   m_editTransEnd  );
	DDX_Control(pDX, IDC_CMD_TRANSFER_LENGTH_END_UNIT,   m_uniTransEnd   );

	DDX_Control(pDX, IDC_CMD_DEBONDED_LENGTH_BEGIN_EDIT, m_editDebondBegin);
	DDX_Control(pDX, IDC_CMD_DEBONDED_LENGTH_END_EDIT,   m_editDebondEnd  );
	DDX_Control(pDX, IDC_CMD_DEBONDED_LENGTH_END_UNIT,   m_uniDebondEnd   );


	DDX_Text(pDX, IDC_CMD_NAME_EDIT, m_strName);
	DDX_Text(pDX, IDC_CMD_SUFFIX_EDIT, m_strSuffix);
}


BEGIN_MESSAGE_MAP(CCMTendonChangeProfDlg, CDialogMove)
	ON_BN_CLICKED(IDC_APPLY_BTN, OnApply)
	ON_BN_CLICKED(IDC_CMD_SEL_BUTTON, OnCmdSelButton)
	ON_BN_CLICKED(IDC_CMD_UNSEL_BUTTON, OnCmdUnselButton)
	ON_BN_CLICKED(IDC_CMD_PROPERTY_BUTTON, OnCmdPropertyButton)
	ON_BN_CLICKED(IDC_CMD_GROUP_BUTTON, OnCmdGroupButton)
	ON_LBN_DBLCLK(IDC_CMD_ALL_LIST, OnDblclkCmdAllList)
	ON_LBN_DBLCLK(IDC_CMD_SELECTED_LIST, OnDblclkCmdSelectedList)
	ON_BN_CLICKED(IDC_CMD_TENDON_NAME_RADIO,           OnSelChangeCombo)
	ON_BN_CLICKED(IDC_CMD_TENDON_PROPERTY_RADIO,       OnSelChangeCombo)
	ON_BN_CLICKED(IDC_CMD_TYPICAL_TENDON_RADIO,        OnSelChangeCombo)
	ON_BN_CLICKED(IDC_CMD_TENDON_PROPERTY_GROUP_RADIO, OnSelChangeCombo)
	ON_BN_CLICKED(IDC_CMD_TENDON_GROUP_RADIO,          OnSelChangeCombo)
	ON_BN_CLICKED(IDC_CMD_DEBONDED_LENGTH_RADIO,       OnSelChangeCombo)
	ON_BN_CLICKED(IDC_CMD_TRANS_LENGTH_RADIO,          OnSelChangeCombo)

	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_NO_TENDONS_SPIN, OnDeltaposSpin)
END_MESSAGE_MAP()


BOOL CCMTendonChangeProfDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();


	InitAllList();
	InitSelList();
	InitCtrl();

	InitPropCombo();
	InitGroupCombo();
	OnSelChangeCombo();

	return TRUE;

}

void CCMTendonChangeProfDlg::OnOK()
{
	if (!DoChangeProperty()) return;

	CDialogMove::OnOK();
}

void CCMTendonChangeProfDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCMTendonChangeProfDlg::OnApply()
{
	if (!DoChangeProperty()) return;
}

void CCMTendonChangeProfDlg::OnCmdSelButton()
{
	DoSelectItem();
}

void CCMTendonChangeProfDlg::OnCmdUnselButton()
{
	DoUnselectItem();
}

void CCMTendonChangeProfDlg::OnDblclkCmdAllList()
{
	DoSelectItem();
}

void CCMTendonChangeProfDlg::OnDblclkCmdSelectedList()
{
	DoUnselectItem();
}

void CCMTendonChangeProfDlg::OnSelChangeCombo()
{
	int nValParamType;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlParamTypeRadio, nValParamType);

	bName			= nValParamType == 0 ? TRUE : FALSE;
	bProperty		= nValParamType == 1 ? TRUE : FALSE;
	bPropertyGrp	= nValParamType == 2 ? TRUE : FALSE;
	bTypicalTD		= nValParamType == 3 ? TRUE : FALSE;
	bGroup			= nValParamType == 4 ? TRUE : FALSE;
	bTransfer		= nValParamType == 5 ? TRUE : FALSE;
	bDebonded		= nValParamType == 6 ? TRUE : FALSE;

	CDlgUtil::CtrlShowHide(this, m_aCtrlTdName, bName);
	CDlgUtil::CtrlShowHide(this, m_aCtrlTdProperty, bProperty || bPropertyGrp);
	CDlgUtil::CtrlShowHide(this, m_aCtrlTypicalTd, bTypicalTD);
	CDlgUtil::CtrlShowHide(this, m_aCtrlTdGroup, bGroup);
	CDlgUtil::CtrlShowHide(this, m_aCtrlTransfer, bTransfer);
	CDlgUtil::CtrlShowHide(this, m_aCtrlTDebonded, bDebonded);

	if(bTypicalTD)
	{
		GetDlgItem(IDC_CMD_TENDON_STATIC)->SetWindowText(_LS(IDS_CMD_TYPICAL_TENDON));
		GetDlgItem(IDC_CMD_TENDON_SELECTED_STATIC)->SetWindowText(_LS(IDS_CMD_TYPICAL_TENDON_SELECTED));
	}
	else if(bPropertyGrp)
	{
		GetDlgItem(IDC_CMD_TENDON_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_GROUP));
		GetDlgItem(IDC_CMD_TENDON_SELECTED_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_GROUP_SELECTED));
	}
	else
	{
		GetDlgItem(IDC_CMD_PROPERTY_STATIC)->SetWindowText(_LS(IDS_CMD_PROPERTY));
		GetDlgItem(IDC_CMD_TENDON_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_PROFILES));
		GetDlgItem(IDC_CMD_TENDON_SELECTED_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_PROFILES_SELECTED));
	}

	AlignControlsDynamic();
	InitAllList();
	InitSelList();
}

void CCMTendonChangeProfDlg::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	CDlgUtil::SetSpinValue(this, IDC_CMD_NO_TENDONS_EDIT, 1, -1, pNMHDR, pResult);
}

void CCMTendonChangeProfDlg::OnCmdPropertyButton()
{
	CCMTendonTypeDlg dlg;
	dlg.DoModal();
}

void CCMTendonChangeProfDlg::OnCmdGroupButton()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMTendonDefDlg::IDD, this);
}

void CCMTendonChangeProfDlg::InitAllList()
{
	m_listAllProfiles.ResetContent();

	int ix;
	int nNum;
	
	if(bPropertyGrp)
	{ 
		T_TDGR_D data;
		CArray<T_TDGR_K, T_TDGR_K> aKey;
		m_pDoc->m_pAttrCtrl->GetTdgrKeyList(aKey);
		nNum = aKey.GetSize();
		for (int i = 0; i < nNum; i++)
		{
			int k = 0;
			for (k = 0; k < m_aSelectedProfiles.GetSize(); k++)
				if (m_aSelectedProfiles[k] == aKey[i]) break;
			if (k == m_aSelectedProfiles.GetSize())
			{
				m_pDoc->m_pAttrCtrl->GetTdgr(aKey[i], data);
				ix = m_listAllProfiles.AddString(data.GroupName);
				m_listAllProfiles.SetItemData(ix, aKey[i]);
			}
		}
	}
	else if (bTypicalTD)
	{
		T_TDNA_D data;
		CArray<T_TDNA_K, T_TDNA_K> aKey;
		m_pDoc->m_pAttrCtrl->GetTdnaKeyList(aKey);
		nNum = aKey.GetSize();
		for (int i = 0; i < nNum; i++)
		{
			int k = 0;
			for (k = 0; k < m_aSelectedProfiles.GetSize(); k++)
				if (m_aSelectedProfiles[k] == aKey[i]) break;
			if (k == m_aSelectedProfiles.GetSize())
			{
				m_pDoc->m_pAttrCtrl->GetTdna(aKey[i], data);

				if (data.bTypicalTendon)
				{
					ix = m_listAllProfiles.AddString(data.TendonName);
					m_listAllProfiles.SetItemData(ix, aKey[i]);
				}
			}
		}
	}
	else
	{
		T_TDNA_D data;
		CArray<T_TDNA_K, T_TDNA_K> aKey;
		m_pDoc->m_pAttrCtrl->GetTdnaKeyList(aKey);
		nNum = aKey.GetSize();
		for (int i = 0; i < nNum; i++)
		{
			int k = 0;
			for (k = 0; k < m_aSelectedProfiles.GetSize(); k++)
				if (m_aSelectedProfiles[k] == aKey[i]) break;
			if (k == m_aSelectedProfiles.GetSize())
			{
				m_pDoc->m_pAttrCtrl->GetTdna(aKey[i], data);
				ix = m_listAllProfiles.AddString(data.TendonName);
				m_listAllProfiles.SetItemData(ix, aKey[i]);
			}
		}
	}

	if (m_listAllProfiles.GetCount() > 0)
		m_listAllProfiles.SetCurSel(0);
}

void CCMTendonChangeProfDlg::InitSelList()
{
	m_listSelProfiles.ResetContent();

	int ix;
	int nNum;

	if (bPropertyGrp)
	{
		T_TDGR_K key;
		T_TDGR_D data;
		nNum = m_aSelectedProfiles.GetSize();
		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdgr(key, data);

			ix = m_listSelProfiles.AddString(data.GroupName);
			m_listSelProfiles.SetItemData(ix, key);
		}
	}
	else if (bTypicalTD)
	{
		T_TDNA_K key;
		T_TDNA_D data;
		nNum = m_aSelectedProfiles.GetSize();
		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);

			if (data.bTypicalTendon)
			{
				ix = m_listSelProfiles.AddString(data.TendonName);
				m_listSelProfiles.SetItemData(ix, key);
			}
		}
	}
	else
	{
		T_TDNA_K key;
		T_TDNA_D data;
		nNum = m_aSelectedProfiles.GetSize();
		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);

			ix = m_listSelProfiles.AddString(data.TendonName);
			m_listSelProfiles.SetItemData(ix, key);
		}
	}
	
	if (nNum > 0) m_listSelProfiles.SetCurSel(0);
}

void CCMTendonChangeProfDlg::InitCtrl()
{
	this->SetWindowText(_LS(IDS_CMD_TENDON_GROUP_DLG_TITLE));
	GetDlgItem(IDC_CMD_PARAM_TYPE_STATIC)->SetWindowText(_LS(IDS_CMD_PARAM_TYPE));
	GetDlgItem(IDC_CMD_TENDON_NAME_RADIO)->SetWindowText(_LS(IDS_CMD_TENDON_NAME));
	GetDlgItem(IDC_CMD_TENDON_PROPERTY_RADIO)->SetWindowText(_LS(IDS_CMD_TENDON_PROPERTY));
	GetDlgItem(IDC_CMD_TENDON_PROPERTY_GROUP_RADIO)->SetWindowText(_LS(IDS_CMD_TENDON_PROPERTY_GROUP));
	GetDlgItem(IDC_CMD_TYPICAL_TENDON_RADIO)->SetWindowText(_LS(IDS_CMD_TYPICAL_TENDON));
	GetDlgItem(IDC_CMD_MODE_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_GROUP_DLG_LIST));
	GetDlgItem(IDC_CMD_TENDON_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_PROFILES));
	GetDlgItem(IDC_CMD_TENDON_SELECTED_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_PROFILES_SELECTED));
	GetDlgItem(IDC_CMD_NAME_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_PROFILES_NAME));
	GetDlgItem(IDC_CMD_SUFFIX_STATIC)->SetWindowText(_LS(IDS_CMD_SUFFIX));
	GetDlgItem(IDC_CMD_SUFFIX_EXAMPLE_STATIC)->SetWindowText(_LS(IDS_CMD_SUFFIX_EXAMPLE));
	GetDlgItem(IDC_CMD_PROPERTY_STATIC)->SetWindowText(_LS(IDS_CMD_PROPERTY));
	GetDlgItem(IDC_CMD_NO_TENDONS_STATIC)->SetWindowText(_LS(IDS_CMD_NO_TENDONS));
	GetDlgItem(IDC_CMD_GROUP_STATIC)->SetWindowText(_LS(IDS_CMD_TENDON_GROUP));
	GetDlgItem(IDC_CMD_TENDON_GROUP_RADIO)->SetWindowText(_LS(IDS_CMD_TENDON_GROUP));

	m_editNum.SetRange(1, SHRT_MAX);
	m_editNum.SetValue(1);
	m_editNum.SetInteger(TRUE);

	m_editDebondBegin.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDebondEnd.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_uniDebondEnd.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editTransBegin.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editTransEnd.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_uniTransEnd.SetUnitType(D_UNITSYS_BASE_LENGTH);

}

void CCMTendonChangeProfDlg::InitPropCombo()
{
	m_cobxProperty.ResetContent();

	int ix;
	T_TDNT_D data;
	CArray<T_TDNT_K, T_TDNT_K> aKey;
	m_pDoc->m_pAttrCtrl->GetTdntKeyList(aKey);
	int nNum = aKey.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		m_pDoc->m_pAttrCtrl->GetTdnt(aKey[i], data);
		ix = m_cobxProperty.AddString(data.TendonTypeName);
		m_cobxProperty.SetItemData(ix, aKey[i]);
	}
	if (nNum > 0) m_cobxProperty.SetCurSel(0);
}

void CCMTendonChangeProfDlg::InitGroupCombo()
{
	m_cobxGroup.ResetContent();

	int ix;
	T_TDGR_D data;
	CArray<T_TDGR_K, T_TDGR_K> aKey;
	m_pDoc->m_pAttrCtrl->GetTdgrKeyList(aKey);
	int nNum = aKey.GetSize();
	for (int i = 0; i < nNum; i++)
	{
		m_pDoc->m_pAttrCtrl->GetTdgr(aKey[i], data);
		ix = m_cobxGroup.AddString(data.GroupName);
		m_cobxGroup.SetItemData(ix, aKey[i]);
	}
	if (nNum > 0) m_cobxGroup.SetCurSel(0);
}

void CCMTendonChangeProfDlg::DoSelectItem()
{
	int nCount = m_listAllProfiles.GetSelCount();
	if (nCount <= 0) return;

	T_TDNA_K key;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listAllProfiles.GetSelItems(nCount, aSelectedItem.GetData());

	for (int i = 0; i < nCount; i++)
	{
		key = m_listAllProfiles.GetItemData(aSelectedItem[i]);
		m_aSelectedProfiles.Add(key);
	}

	InitAllList();
	InitSelList();
}

void CCMTendonChangeProfDlg::DoUnselectItem()
{
	int nCount = m_listSelProfiles.GetSelCount();
	if (nCount <= 0) return;

	T_TDNA_K key;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listSelProfiles.GetSelItems(nCount, aSelectedItem.GetData());

	for (int i = m_aSelectedProfiles.GetSize() - 1; i >= 0; i--)
	{
		int k = 0;
		for (k = 0; k < nCount; k++)
		{
			key = m_listSelProfiles.GetItemData(aSelectedItem[k]);
			if (key == m_aSelectedProfiles[i]) break;
		}
		if (k != nCount) m_aSelectedProfiles.RemoveAt(i);
	}

	InitAllList();
	InitSelList();
}

BOOL CCMTendonChangeProfDlg::DoChangeProperty()
{
	int ix = m_cobxProperty.GetCurSel();
	if (ix == CB_ERR)
	{
		AfxMessageBox(_LS(IDS_CMD_ERR_Select_Tendon_Property));
		return FALSE;
	}

	int nNumTemp;
	int nNum = m_aSelectedProfiles.GetSize();
	if (nNum <= 0) return TRUE;


	T_TDNA_K key;
	T_TDNA_D data;
	CArray<T_TDNA_D, T_TDNA_D&> aModifyData;

	if (bName)
	{
		if (!RenameTendonProfiles()) return FALSE;
	}
	else if (bProperty)
	{
		T_TDNT_K PropKey = m_cobxProperty.GetItemData(ix);
		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);
			data.TendonTypeKey = PropKey;
			aModifyData.Add(data);
		}
		if (!m_pDoc->m_pDataCtrl->ModifyTdna(m_aSelectedProfiles, m_aSelectedProfiles, aModifyData)) return FALSE;
	}
	else if (bPropertyGrp)
	{
		T_TDGR_K GroupKey;
		T_TDGR_D GroupData;
		CArray<T_TDNA_K, T_TDNA_K> aProfileKey;
		T_TDNT_K PropKey = m_cobxProperty.GetItemData(ix);
		for (int i = 0; i < nNum; i++)
		{
			GroupKey = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdgr(GroupKey, GroupData);
			m_pDoc->m_pAttrCtrl->GetTdnaKeyList(aProfileKey);

			nNumTemp = aProfileKey.GetCount();
			
			for (int j = 0; j < nNumTemp; j++)
			{
				m_pDoc->m_pAttrCtrl->GetTdna(aProfileKey[j], data);
				if (data.GroupKey == GroupKey)
				{
					data.TendonTypeKey = PropKey;
					if (!m_pDoc->m_pDataCtrl->ModifyTdna(aProfileKey[j], aProfileKey[j], data)) return FALSE;
				}
			}
		}
	}
	else if (bTypicalTD)
	{
		int nNumTd = (int)m_editNum.GetEditValueInt();
		T_TDNT_K PropKey = m_cobxProperty.GetItemData(ix);
		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);
			data.dTendonNum = nNumTd;
			aModifyData.Add(data);
		}
		if (!m_pDoc->m_pDataCtrl->ModifyTdna(m_aSelectedProfiles, m_aSelectedProfiles, aModifyData)) return FALSE;
	}
	else if(bGroup)
	{
		ix = m_cobxGroup.GetCurSel();
		T_TDGR_K GroupKey = m_cobxGroup.GetItemData(ix);
		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);
			data.GroupKey = GroupKey;
			aModifyData.Add(data);
		}
		if (!m_pDoc->m_pDataCtrl->ModifyTdna(m_aSelectedProfiles, m_aSelectedProfiles, aModifyData)) return FALSE;
	}
	else if(bTransfer)
	{
		double dBegin = m_editTransBegin.GetEditValue();
		double dEnd   = m_editTransEnd.GetEditValue();

		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);
			data.nNoTensionCalc = 0; // User define
			data.dNoTensionBeginLen = dBegin;
			data.dNoTensionEndLen   = dEnd;
			aModifyData.Add(data);
		}
		if (!m_pDoc->m_pDataCtrl->ModifyTdna(m_aSelectedProfiles, m_aSelectedProfiles, aModifyData)) return FALSE;
	}
	else if(bDebonded)
	{
		double dBegin = m_editDebondBegin.GetEditValue();
		double dEnd   = m_editDebondEnd.GetEditValue();

		for (int i = 0; i < nNum; i++)
		{
			key = m_aSelectedProfiles.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);
			data.dDebondBeginLen = dBegin;
			data.dDebondEndLen   = dEnd;
			aModifyData.Add(data);
		}
		if (!m_pDoc->m_pDataCtrl->ModifyTdna(m_aSelectedProfiles, m_aSelectedProfiles, aModifyData)) return FALSE;
	}
	else
		ASSERT(0);

	return TRUE;
}

void CCMTendonChangeProfDlg::AlignControlsDynamic()
{
	CArray<UINT, UINT>  aCtrl;
	CRect rRef, rToMove;
	int nDistX, nDistY;
	int nValParamType;

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlParamTypeRadio, nValParamType);
	bName        = nValParamType == 0 ? TRUE : FALSE;
	bProperty    = nValParamType == 1 ? TRUE : FALSE;
	bPropertyGrp = nValParamType == 2 ? TRUE : FALSE;
	bTypicalTD   = nValParamType == 3 ? TRUE : FALSE;
	bGroup       = nValParamType == 4 ? TRUE : FALSE;
	bTransfer    = nValParamType == 5 ? TRUE : FALSE;
	bDebonded    = nValParamType == 6 ? TRUE : FALSE;
	
	GetDlgItem(IDC_CMD_MODE_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_TENDON_STATIC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlList, nDistY);

	if (bName)
	{
		GetDlgItem(IDC_CMD_ALL_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_NAME_STATIC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlTdName, nDistY);
	}
	else if (bProperty || bPropertyGrp)
	{
		GetDlgItem(IDC_CMD_ALL_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_PROPERTY_STATIC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlTdProperty, nDistY);
	}
	else if (bTypicalTD)
	{
		GetDlgItem(IDC_CMD_ALL_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_NO_TENDONS_STATIC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlTypicalTd, nDistY);
	}
	else if(bGroup)
	{
		GetDlgItem(IDC_CMD_ALL_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_GROUP_STATIC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlTdGroup, nDistY);
	}
	else if(bTransfer)
	{
		GetDlgItem(IDC_CMD_ALL_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_TRANSFER_LENGTH_STC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlTransfer, nDistY);
	}
	else if(bDebonded)
	{
		GetDlgItem(IDC_CMD_ALL_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_DEBONDED_LENGTH_STC)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlTDebonded, nDistY);
	}
	else 
		ASSERT(0);


	// Mode 그룹박스 크기 조정을 위해 그룹박스 안의 가장 아래 컨트롤 위치를 참조.
	if (bName)
	{
		int size = m_aCtrlTdName.GetCount();
		GetDlgItem(m_aCtrlTdName[size-1])->GetWindowRect(rToMove);
	}
	else if (bProperty || bPropertyGrp)
	{
		int size = m_aCtrlTdProperty.GetCount();
		GetDlgItem(m_aCtrlTdProperty[size - 1])->GetWindowRect(rToMove);
	}
	else if (bTypicalTD)
	{
		int size = m_aCtrlTypicalTd.GetCount();
		GetDlgItem(m_aCtrlTypicalTd[size - 1])->GetWindowRect(rToMove);
	}
	else if (bGroup)
	{
		int size = m_aCtrlTdGroup.GetCount();
		GetDlgItem(m_aCtrlTdGroup[size - 1])->GetWindowRect(rToMove);
	}
	else if (bTransfer)
	{
		int size = m_aCtrlTransfer.GetCount();
		GetDlgItem(m_aCtrlTransfer[size - 1])->GetWindowRect(rToMove);
	}
	else if (bDebonded)
	{
		int size = m_aCtrlTDebonded.GetCount();
		GetDlgItem(m_aCtrlTDebonded[size - 1])->GetWindowRect(rToMove);
	}

	// resize
	CRect rRect ;
	GetDlgItem(IDC_CMD_MODE_STATIC)->GetWindowRect(rRect);
	nDistY = rRect.bottom - rToMove.bottom;
	rRect.bottom -= nDistY;
	rRect.bottom += globalUtils.ScaleByDPI(10);
	ScreenToClient(rRect);
	GetDlgItem(IDC_CMD_MODE_STATIC)->MoveWindow(rRect);

	aCtrl.RemoveAll();
	aCtrl.Add(IDOK);
	aCtrl.Add(IDCANCEL);
	aCtrl.Add(IDC_APPLY_BTN);
	GetDlgItem(IDC_CMD_MODE_STATIC)->GetWindowRect(rRef);
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, aCtrl, nDistY);

	// resize self size
	CRect rWindow, rOK;
	GetDlgItem(IDOK)->GetWindowRect(rOK);
	GetWindowRect(rWindow);
	rWindow.bottom = rOK.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(rWindow);
}

BOOL CCMTendonChangeProfDlg::RenameTendonProfiles()
{
	UpdateData();

	if (m_strSuffix.IsEmpty())
	{
		// Suffix 컨트롤이 공란이면
		// 선택된 모든 profile을 같은 이름으로 변경하려는 문제 발생
		T_TDNA_K key;
		T_TDNA_D data;
		int nNum = m_aSelectedProfiles.GetSize();
		
		if (nNum > 1)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_ERR_TENDON_RENAME_DUPLICATED));
			return FALSE;
		}
		else if (nNum == 1)
		{
			key = m_aSelectedProfiles.GetAt(0);
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);
			data.TendonName = m_strName;
			if (!m_pDoc->m_pDataCtrl->ModifyTdna(key, key, data)) return FALSE;
		}
	}
	else
	{
		if (!GenBysuffixButton()) return FALSE;
	}
	return TRUE;
}

BOOL CCMTendonChangeProfDlg::GenBysuffixButton()
{
	UpdateData();
	CArray<int, int> SuffixList;
	CMap<int, int, int, int> SuffixMap;
	SuffixMap.InitHashTable(500);
	int TempInt;

	T_TDNA_K TdnaKey;
	T_TDNA_D TdnaData;

	if (CStrParser::ParsingListByTo(m_strSuffix, SuffixList))
	{
		CString TempStr;
		int nSuffix = SuffixList.GetSize();
		int nNum = m_aSelectedProfiles.GetSize();
		if (nSuffix != nNum)
		{
			m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_ERR_TENDON_RENAME_SUFFIX));
			return FALSE;
		}

		for (int i = 0; i < nSuffix; i++)
		{
			if (!SuffixMap.Lookup(i, TempInt))
			{
				SuffixMap.SetAt(i, i);
				TempStr.Format(_T("_%03d"), SuffixList[i]);
				TempStr = m_strName + TempStr;

				TdnaKey = m_aSelectedProfiles.GetAt(i);
				m_pDoc->m_pAttrCtrl->GetTdna(TdnaKey, TdnaData);
				TdnaData.TendonName = TempStr;

				if (!m_pDoc->m_pDataCtrl->ModifyTdna(TdnaKey, TdnaKey, TdnaData)) return FALSE;
			}
		}
	}
	return TRUE;
}

void CCMTendonChangeProfDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMTendonChangeProfDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChanged = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_TDNT_ADD):
		case(UR_TDNT_DEL):
		case(UR_TDNT_MFD):
		case(UR_TDNT_MFS):
		case(UR_TDGR_ADD):
		case(UR_TDGR_DEL):
		case(UR_TDGR_MFD):
		case(UR_TDGR_MFS):
			bChanged = TRUE;
			break;
		default:
			break;
		}
	}

	if (bChanged)
	{
		InitPropCombo();
		InitGroupCombo();
	}
}