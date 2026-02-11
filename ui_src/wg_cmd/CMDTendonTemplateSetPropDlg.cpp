// CMDTendonTemplateSetPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMDTendonTemplateSetPropDlg.h"
#include "CMTendonTypeDlg.h"
#include "CMTendonDefDlg.h"
#include "ExportFunc.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_db\DBLib.h"
#include "..\wg_db\DBDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMDTendonTemplateSetPropDlg::CCMDTendonTemplateSetPropDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent /* = NULL*/)
	:CDialogMove(CCMDTendonTemplateSetPropDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pTemplateCtrl = pCtrl;
	m_mapSelectedKey.clear();
}

CCMDTendonTemplateSetPropDlg::~CCMDTendonTemplateSetPropDlg()
{
}


BEGIN_MESSAGE_MAP(CCMDTendonTemplateSetPropDlg, CDialogMove)
 	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_SETPROP_BTN_DEL,		OnCmdExclude)
 	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_SETPROP_BTN_ADD,		OnCmdInclude)
 	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_SETPROP_BTN_ALL,		OnCmdAll)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_SETPROP_BTN_NONE,		OnCmdNone)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_SETPROP_BTN_PROP,		OnCmdProp)
 	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_SETPROP_BTN_GRP,		OnCmdGrp)
END_MESSAGE_MAP()

void CCMDTendonTemplateSetPropDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	
 	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_SETPROP_LIST_TNDN_SEL,	m_listSelProfiles);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_SETPROP_LIST_TNDN,		m_listAllProfiles);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_SETPROP_CBX_PROP,		m_wndTdntCobx);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_SETPROP_CBX_GRP,		m_wndTdgrCobx);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_SETPROP_RDO_REFAXIS_CURVE,		m_rdoOutTypeCurve);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_SETPROP_RDO_REFAXIS_ELEM,		m_rdoOutTypeElem);
}

void CCMDTendonTemplateSetPropDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCMDTendonTemplateSetPropDlg::OnOK()
{
 	int nCnt = (int)m_mapSelectedKey.size();
 	if(nCnt <= 0)
 	{
 		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_TDNA));
 		return;
 	}

	int i;
 	vecTndnKey vecKeyList;
 	GetSelectedKeyList(vecKeyList);
	nCnt = (int)vecKeyList.size();
	
	T_TDGR_K tdgrK;
	if(!m_wndTdgrCobx.GetSelectedTdgr(tdgrK))
	{
		tdgrK = 0;
	}
	
	T_TDNT_K tdntK;
	if(!m_wndTdntCobx.GetSelectedTdnt(tdntK))
	{
 		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_TDNT));
		return;
	}

	
	BOOL bCurveModify = FALSE;
	BOOL bCurveModifyVal = FALSE;
	if( m_rdoOutTypeCurve.GetCheck() || m_rdoOutTypeElem.GetCheck() )
	{
		bCurveModify = TRUE;
		bCurveModifyVal = m_rdoOutTypeCurve.GetCheck() ? TRUE : FALSE;
	}

	TndnTemplate tndnInfo;
	for(i = 0 ; i < nCnt; i++)
	{
		if(!m_pTemplateCtrl->GetTemplateNU(vecKeyList[i], tndnInfo))
		{
			ASSERT(0);
			continue;
		}

		tndnInfo.keyGroup = (unsigned int) tdgrK;
		tndnInfo.keyProperty = (unsigned int) tdntK;
		if(bCurveModify)
			tndnInfo.bCurveTypeOut = bCurveModifyVal;

		m_pTemplateCtrl->ModTemplateNU(vecKeyList[i], tndnInfo);
	}
	CDialogMove::OnOK();
}

BOOL CCMDTendonTemplateSetPropDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	
	SetControl();
	InitAllList();
	InitSelList();
	
	return TRUE;
}

void CCMDTendonTemplateSetPropDlg::GetSelectedKeyList( vecTndnKey& keyList )
{
	keyList.clear();

	std::map<int, BOOL>::iterator itr;
	for(itr = m_mapSelectedKey.begin(); itr != m_mapSelectedKey.end(); itr++)
	{
		if(itr->second)
		{
			keyList.push_back(itr->first);
		}
	}
}

void CCMDTendonTemplateSetPropDlg::InitAllList()
{
	m_listAllProfiles.ResetContent();
	
	TndnTemplate tempInfo;	
	vecTndnKey keyList;
	int nNum = m_pTemplateCtrl->GetKeyList(keyList);
	int idx = 0;

	for(int i = 0; i < nNum; i++)
	{
		if(!IsSelectedProfile(keyList[i]))
		{
			if(!m_pTemplateCtrl->GetTemplateNU(keyList[i], tempInfo))
			{
				ASSERT(0);
				continue;
			}
			LPCTSTR strInput = tempInfo.strName;
			idx = m_listAllProfiles.AddString(strInput);
			m_listAllProfiles.SetItemData(idx, keyList[i]);
		}
	}

	nNum = m_listAllProfiles.GetCount();
	if(nNum > 0)
	  m_listAllProfiles.SetCurSel(0);
}

void CCMDTendonTemplateSetPropDlg::InitSelList()
{
	m_listSelProfiles.ResetContent();
	
	TndnTemplate tempInfo;
	int idx = 0;
	std::map<int, BOOL>::iterator itr;
	for(itr = m_mapSelectedKey.begin(); itr != m_mapSelectedKey.end(); itr++)
	{
		int nKey = itr->first;
		if(IsSelectedProfile(nKey))
		{
			m_pTemplateCtrl->GetTemplateNU(nKey, tempInfo);
			LPCTSTR strInput = tempInfo.strName;
			idx = m_listSelProfiles.AddString(strInput);
			m_listSelProfiles.SetItemData(idx, nKey);
		}
	}

	int nNum = m_listSelProfiles.GetCount();
	if (nNum > 0)
		m_listSelProfiles.SetCurSel(0);
}

void CCMDTendonTemplateSetPropDlg::OnCmdInclude()
{
	vecTndnKey keyList;
	GetSelKeyByListBox(m_listAllProfiles, keyList);
	if(keyList.size() > 0)
	{
		SetSelectedKeyList(keyList, TRUE);
		
		InitAllList();
		InitSelList();
	}
}

void CCMDTendonTemplateSetPropDlg::OnCmdExclude()
{
	vecTndnKey keyList;
	GetSelKeyByListBox(m_listSelProfiles, keyList);
	if(keyList.size() > 0)
	{
		SetSelectedKeyList(keyList, FALSE);

		InitAllList();
		InitSelList();
	}
}

void CCMDTendonTemplateSetPropDlg::OnCmdAll()
{
	vecTndnKey keyList;
	int nNum = m_pTemplateCtrl->GetKeyList(keyList);
	SetSelectedKeyList(keyList, TRUE);

	InitAllList();
	InitSelList();
}

void CCMDTendonTemplateSetPropDlg::OnCmdNone()
{
	m_mapSelectedKey.clear();
	InitAllList();
	InitSelList();
}

void CCMDTendonTemplateSetPropDlg::SetSelectedKeyList( const vecTndnKey& keyList , BOOL bSelect )
{
	for(int i = 0 ; i < keyList.size(); i++)
	{
		int nKey = keyList[i];
		std::pair<int,BOOL> mapPair;
		mapPair.first = nKey;
		mapPair.second = bSelect;

		std::map<int, BOOL>::iterator itrFound = m_mapSelectedKey.find(nKey);
		if(itrFound == m_mapSelectedKey.end())
		{
 			m_mapSelectedKey.insert(mapPair);
		}
		else
		{
			itrFound->second = bSelect;
		}
	}
}

BOOL CCMDTendonTemplateSetPropDlg::IsSelectedProfile(int nKey)
{	
	std::map<int,BOOL>::iterator itr = m_mapSelectedKey.find(nKey);

	if(itr == m_mapSelectedKey.end()) //찾는항목이 없다.
		return FALSE;

	BOOL retVal = itr->second;	//찾았지만 FALSE/TRUE 일 수 있다.

	return retVal;
}

void CCMDTendonTemplateSetPropDlg::GetSelKeyByListBox( const CListBox& listBox, vecTndnKey& keyList )
{
	keyList.clear();

	int nCount = listBox.GetSelCount();
	if (nCount <= 0)
		return;
	
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	listBox.GetSelItems(nCount, aSelectedItem.GetData()); 
	
	int i, nKey;
	for (i = 0; i < nCount; i++)
	{
		nKey = listBox.GetItemData(aSelectedItem[i]);
		keyList.push_back(nKey);
	}	
}

void CCMDTendonTemplateSetPropDlg::ComboBoxDynamicChange(CComboBox* pCombo)
{
	CString str1;
	CSize sz;
	int dx = 0;
	CDC*  pDC = GetDC();
	
	for(int h = 0; h < pCombo->GetCount(); h++)
	{
		pCombo->GetLBText(h, str1);
		sz = pDC->GetTextExtent(str1);
		
		if(sz.cx > dx)
		{
			dx = sz.cx;
		}
	}
	
	if ( dx > pCombo->GetDroppedWidth() ) 
	{
		pCombo->SetDroppedWidth(dx);
	}
}

void CCMDTendonTemplateSetPropDlg::OnCmdProp()
{
	CCMTendonTypeDlg Dlg;
	Dlg.DoModal();
	ComboBoxDynamicChange(&m_wndTdntCobx);
}

void CCMDTendonTemplateSetPropDlg::OnCmdGrp()
{
	CreateOrActivateDlg(m_pDoc, CCMTendonDefDlg::IDD, this);
	ComboBoxDynamicChange(&m_wndTdgrCobx);	
}

void CCMDTendonTemplateSetPropDlg::SetControl()
{
	BOOL bIsCurve = m_pTemplateCtrl->IsCurveElem();
	m_rdoOutTypeCurve.EnableWindow(bIsCurve);
	
	if(bIsCurve)
	{    
		m_rdoOutTypeElem.SetCheck(FALSE);
		m_rdoOutTypeCurve.SetCheck(FALSE);
	}
	else
	{
		m_rdoOutTypeElem.SetCheck(TRUE);
		m_rdoOutTypeCurve.SetCheck(FALSE);
	}  
}
