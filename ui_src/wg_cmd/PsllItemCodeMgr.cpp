// PsllItemCodeMgr.cpp: implementation of the CPsllItemCodeMgr class.
//
// 2003. 12. 29  by TAE
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "PsllItemCodeMgr.h"
#include "PsllItemDlg.h"
#include "PsllItemKISTEC2013.h"
#include "PsllItemMOE2018.h"
#include "PsllItemKISTEC2019.h"

#include "..\wg_db\wg_db_DBLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPsllItemCodeMgr::CPsllItemCodeMgr(CWnd* pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CPsllItemDlg*)pParent;

	m_nCurCode = -1;
	m_aCodeName.RemoveAll();
	m_aCodeDlg.RemoveAll();

	CreateCodeDlg();
}

CPsllItemCodeMgr::~CPsllItemCodeMgr()
{
	DeleteCodeDlg();
}

//////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////

// [PsllCode] 추가시 수정
int  CPsllItemCodeMgr::GetCodeNameList(CStringArray& aCodeName)
{
	aCodeName.RemoveAll();
	CDBLib::GetSeisEvalCodeNameList(aCodeName);

	return aCodeName.GetSize();
}

void CPsllItemCodeMgr::ChangeCodeDlg(int nCode)
{
	if(m_nCurCode == nCode) return;

	HWND hWnd = NULL;
	hWnd = m_pParent->GetSafeHwnd();
	if (hWnd == NULL || !(::IsWindow(hWnd))) return;

	CString strCodeName = CDBLib::GetSeisEvalCodeNameByMethod(nCode);
	int nIndex = -1;
	for(int i=0; i<m_aCodeName.GetSize(); i++)
	{
		if(m_aCodeName[i]!=strCodeName) continue;
		nIndex = i;
		break;
	}
	
	if(nIndex < 0 || nIndex >= m_aCodeDlg.GetSize()) { ASSERT(0); return; }
	if(m_aCodeDlg[nIndex]==NULL) { ASSERT(0); return; }

	if(m_nCurCode >= 0)
	{
		CString strCurCodeName = CDBLib::GetSeisEvalCodeNameByMethod(m_nCurCode);
		int nCurIndex = -1;
		for(int i=0; i<m_aCodeName.GetSize(); i++)
		{
			if(m_aCodeName[i]!=strCurCodeName) continue;
			nCurIndex = i;
			break;
		}

		if(nCurIndex >= 0 && m_aCodeDlg.GetSize() > nCurIndex)
			m_aCodeDlg[nCurIndex]->End();
	}

	CRect WRect;
	m_pParent->m_wndHolder.GetWindowRect(&WRect);
	m_pParent->ScreenToClient(WRect);
	m_aCodeDlg[nIndex]->CreateInit(m_pParent, WRect.left,WRect.top);

	m_nCurCode = nCode;
}

// [PsllCode] 추가시 수정
void CPsllItemCodeMgr::Data2CodeDlg(T_PSLL_D& data, int nCode, BOOL bModify)
{
	CString strCodeName = CDBLib::GetSeisEvalCodeNameByMethod(nCode);
	int nIndex = -1;
	for(int i=0; i<m_aCodeName.GetSize(); i++)
	{
		if(m_aCodeName[i]!=strCodeName) continue;
		nIndex = i;
		break;
	}

	if(nIndex < 0 || nIndex >= m_aCodeDlg.GetSize()) { ASSERT(0); return; }
	if(m_aCodeDlg[nIndex]==NULL) { ASSERT(0); return; }

	switch(nCode)
	{
	case D_SEME_METHOD_KISTEC2013	:  ((CPsllItemKISTEC2013	*)m_aCodeDlg[nIndex])->SetData2Dlg(data.code.KISTEC2013, bModify, nCode); break;
	case D_SEME_METHOD_MOE2018		:  ((CPsllItemMOE2018		*)m_aCodeDlg[nIndex])->SetData2Dlg(data.code.Moe2018, bModify, nCode); break;
	case D_SEME_METHOD_MOE2019		:  ((CPsllItemMOE2018		*)m_aCodeDlg[nIndex])->SetData2Dlg(data.code.Moe2018, bModify, nCode); break;
	case D_SEME_METHOD_KISTEC2019	:  ((CPsllItemKISTEC2019	*)m_aCodeDlg[nIndex])->SetData2Dlg(data.code.KISTEC2019, bModify, nCode); break;
	default: ASSERT(0);
	}

	ChangeCodeDlg(nCode);
}

// // [PsllCode] 추가시 수정
BOOL CPsllItemCodeMgr::CodeDlg2Data(T_PSLL_D& data)
{
	CString strCodeName = CDBLib::GetSeisEvalCodeNameByMethod(m_nCurCode);
	int nIndex = -1;
	for(int i=0; i<m_aCodeName.GetSize(); i++)
	{
		if(m_aCodeName[i]!=strCodeName) continue;
		nIndex = i;
		break;
	}

	if(nIndex < 0 || nIndex >= m_aCodeDlg.GetSize()) { ASSERT(0); return FALSE; }
	if(m_aCodeDlg[nIndex]==NULL) { ASSERT(0); return FALSE; }

	BOOL bOK = TRUE;

	switch(m_nCurCode)
	{
	case D_SEME_METHOD_KISTEC2013	: bOK = ((CPsllItemKISTEC2013	*)m_aCodeDlg[nIndex])->SetDlg2Data(data.code.KISTEC2013); break;
	case D_SEME_METHOD_MOE2018		: bOK = ((CPsllItemMOE2018		*)m_aCodeDlg[nIndex])->SetDlg2Data(data.code.Moe2018); break;
	case D_SEME_METHOD_MOE2019		: bOK = ((CPsllItemMOE2018		*)m_aCodeDlg[nIndex])->SetDlg2Data(data.code.Moe2018); break;
	case D_SEME_METHOD_KISTEC2019	: bOK = ((CPsllItemKISTEC2019	*)m_aCodeDlg[nIndex])->SetDlg2Data(data.code.KISTEC2019); break;

	default: ASSERT(0);
	}

	return bOK;
}

//////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////

// [PsllCode] 추가시 수정
void CPsllItemCodeMgr::CreateCodeDlg()
{
	GetCodeNameList(m_aCodeName);
	for(int i=0; i<m_aCodeName.GetSize(); i++)
	{
		CString& strCodeName = m_aCodeName[i];
		int nCode = CDBLib::GetSeisEvalMethodByCodeName(strCodeName);
				
		switch(nCode)
		{
		case D_SEME_METHOD_KISTEC2013	: m_aCodeDlg.Add(new CPsllItemKISTEC2013()); break;
		case D_SEME_METHOD_MOE2018		: m_aCodeDlg.Add(new CPsllItemMOE2018()); break;
		case D_SEME_METHOD_MOE2019		: m_aCodeDlg.Add(new CPsllItemMOE2018()); break;
		case D_SEME_METHOD_KISTEC2019	: m_aCodeDlg.Add(new CPsllItemKISTEC2019()); break;

		default: ASSERT(0); m_aCodeDlg.Add(new CPsllItemKISTEC2013()); break;
		}
	}
}

void CPsllItemCodeMgr::DeleteCodeDlg()
{
	int nCount = m_aCodeDlg.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		if(m_aCodeDlg[i]==NULL) continue;
		delete m_aCodeDlg[i];
		m_aCodeDlg[i] = NULL;
	}
}
