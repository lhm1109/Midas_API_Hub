// CMDTendonTemplateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMDTendonTemplateExportDlg.h"

#include "..\wg_base\wg_base_MsgDll.h"


#include "..\wg_db\DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMDTendonTemplateExportDlg::CCMDTendonTemplateExportDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent /* = NULL*/)
	:CDialogMove(CCMDTendonTemplateExportDlg::IDD, pParent)
{
	m_pTemplateCtrl = pCtrl;
	m_mapSelectedKey.clear();
}

CCMDTendonTemplateExportDlg::~CCMDTendonTemplateExportDlg()
{
}


BEGIN_MESSAGE_MAP(CCMDTendonTemplateExportDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_EXPORT_EXCLUDE,	OnCmdExclude)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_EXPORT_INCLUDE,	OnCmdInclude)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_EXPORT_ALL,		  OnCmdAll)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_EXPORT_NONE,		OnCmdNone)
END_MESSAGE_MAP()

void CCMDTendonTemplateExportDlg::DoDataExchange( CDataExchange* pDX )
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_EXPORT_LIST_SEL,	m_listSelProfiles);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_EXPORT_LIST,		m_listAllProfiles);	
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_EXPORT_USE_RATIO,	m_chkUseRatio);
}

void CCMDTendonTemplateExportDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCMDTendonTemplateExportDlg::OnOK()
{
	int nCnt = (int)m_mapSelectedKey.size();
	BOOL bUseRatio = m_chkUseRatio.GetCheck();
	if(nCnt <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_TDNA));
		return;
	}

	CString csFileName = CDBLib::MakeFileNameWithNewExt(_T(".tdt"));
	LPCTSTR szFileName;
	if (csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;
	
	CFileDialog fd(FALSE, _T(".tdt"), szFileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
		_T("Tendon Template TDT File(*.tdt)|*.tdt|All File(*.*)|*.*||"), NULL);
	
	vecTndnKey nKeyList;
	GetSelectedKeyList(nKeyList);
	
	if(fd.DoModal() == IDOK)
	{
		CString filePath = fd.GetPathName();
		m_pTemplateCtrl->Export(filePath.GetBuffer(filePath.GetLength()), &nKeyList, bUseRatio);
		CDialogMove::OnOK();
	}
}

BOOL CCMDTendonTemplateExportDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	
	InitAllList();
	InitSelList();
	
	return TRUE;
}

void CCMDTendonTemplateExportDlg::GetSelectedKeyList( vecTndnKey& keyList )
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

void CCMDTendonTemplateExportDlg::InitAllList()
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

void CCMDTendonTemplateExportDlg::InitSelList()
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

void CCMDTendonTemplateExportDlg::OnCmdInclude()
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

void CCMDTendonTemplateExportDlg::OnCmdExclude()
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

void CCMDTendonTemplateExportDlg::OnCmdAll()
{
	vecTndnKey keyList;
	int nNum = m_pTemplateCtrl->GetKeyList(keyList);
	SetSelectedKeyList(keyList, TRUE);

	InitAllList();
	InitSelList();
}

void CCMDTendonTemplateExportDlg::OnCmdNone()
{
	m_mapSelectedKey.clear();
	InitAllList();
	InitSelList();
}

void CCMDTendonTemplateExportDlg::SetSelectedKeyList( const vecTndnKey& keyList , BOOL bSelect )
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

BOOL CCMDTendonTemplateExportDlg::IsSelectedProfile(int nKey)
{	
	std::map<int,BOOL>::iterator itr = m_mapSelectedKey.find(nKey);

	if(itr == m_mapSelectedKey.end()) //찾는항목이 없다.
		return FALSE;

	BOOL retVal = itr->second;	//찾았지만 FALSE/TRUE 일 수 있다.

	return retVal;
}

void CCMDTendonTemplateExportDlg::GetSelKeyByListBox( const CListBox& listBox, vecTndnKey& keyList )
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