#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTimeDepMatFuncDlg.h"
#include "CMTdmfItemDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_base\wg_base_DlgUtil.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTimeDepMatFuncDlg dialog
CCMTimeDepMatFuncDlg::CCMTimeDepMatFuncDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CCMTimeDepMatFuncDlg::IDD, pParent)
{
	m_nCurSel = 0;
}

void CCMTimeDepMatFuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TAB                    , m_wndTab);
	DDX_Control(pDX, IDC_CMD_TIME_DEP_MAT_FLIST     , m_wndFuncList);
}

BEGIN_MESSAGE_MAP(CCMTimeDepMatFuncDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_ADD                       , OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_DELETE                    , OnCmdDelete)
	ON_BN_CLICKED(IDC_CMD_MODIFY                    , OnCmdModify)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB            , OnSelchangeCmdTab)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TIME_DEP_MAT_FLIST , OnDblclkCmdTimeDepMatFlist)
	ON_BN_CLICKED(IDC_CMD_COPY                      , OnCmdCopy)
END_MESSAGE_MAP()

BOOL CCMTimeDepMatFuncDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SetListCtrlHeader();	

	m_wndTab.InsertItem(0,_LS(IDS_CMD0417__Creep));
	m_wndTab.InsertItem(1,_LS(IDS_CMD0417__Shrinkage_Strain));
	m_wndTab.InsertItem(2,_LS(IDS_CMD_RELAXATION));

	m_wndTab.SetCurSel(m_nCurSel);

	UpdateFunctionList();

	return TRUE;
}

void CCMTimeDepMatFuncDlg::SetListCtrlHeader()
{
	CStringArray         Titles;
	CArray<float, float> HRatio;
	CString strTemp;

	strTemp = _LS(IDS_CMD0417__Function_Name);
	Titles.Add(strTemp);
	HRatio.Add(1.0f);

	CDlgUtil::_SetListCtrlHeader(&m_wndFuncList, Titles, &HRatio, NULL);
}

void CCMTimeDepMatFuncDlg::SetCurPage(int nIndex)
{
	m_nCurSel = nIndex;
}

void CCMTimeDepMatFuncDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(GetSafeHwnd())
	{
		switch(lHint)
		{
		case D_UPDATE_DBALL:
		case D_UPDATE_DEFAULT:
		case D_UPDATE_BUFFER_AFTER:
			UpdateBuffer();
			break;

		case D_UPDATE_BUFFER_BEFORE:
		case D_UPDATE_UNIT:
		case D_UPDATE_SEL_ADD: 
		case D_UPDATE_SEL_DEL:
		case D_UPDATE_EXE_START: 
		case D_UPDATE_EXE_END:
			break;
		
		default:
			break;
		}
	}
}

void CCMTimeDepMatFuncDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;

	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdmfCommand = FALSE;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_TDMF_ADD):
		case(UR_TDMF_DEL):
		case(UR_TDMF_MFD): 
		case(UR_TDMF_MFS):
					bIsTdmfCommand = TRUE;
				break;
		default:
			break;
		}
	}

	if(bIsTdmfCommand) 
	{
		UpdateFunctionList();   
	}
}

void CCMTimeDepMatFuncDlg::UpdateFunctionList()
{
	int nWhat = m_wndTab.GetCurSel()+1;

	CArray<T_TDMF_K,T_TDMF_K> rKeyList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmfKeyList(rKeyList);

	int nTdmf = rKeyList.GetSize();

	m_wndFuncList.DeleteAllItems();
	
	CStringArray Contents;
	int CurIndex = 0;
	T_TDMF_D TdmfD;

	for(int i=0 ; i<nTdmf ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmf(rKeyList[i], TdmfD))
		{
			Contents.RemoveAll();
			
			if(TdmfD.nMatlFuncType == nWhat) // Creep Function Type이 Current Tab과 일치하면 
			{
				Contents.Add(TdmfD.MatlFuncName);
				CDlgUtil::SetListItem(&m_wndFuncList, CurIndex, Contents, rKeyList[i]);
				CurIndex++;
			}
		}
	}
}

void CCMTimeDepMatFuncDlg::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateFunctionList();
	*pResult = 0;
}

void CCMTimeDepMatFuncDlg::OnCmdAdd() 
{
	CCMTdmfItemDlg Dlg;
	Dlg.SetType(m_wndTab.GetCurSel()+1);
	Dlg.DoModal();
}

void CCMTimeDepMatFuncDlg::OnCmdDelete() 
{
	CArray<DWORD,DWORD> arRet;
	CArray<CString, CString&> arFuncNames;
	T_TDMF_D TdmfD;

	CDlgUtil::GetSelectedListItemData(&m_wndFuncList, arRet);

	int nRet = arRet.GetSize();

	if(nRet > 0)
	{
		for(int i = 0 ; i < nRet; i++)
		{
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmf((T_TDMF_K)arRet[i], TdmfD))
			{
				arFuncNames.Add(TdmfD.MatlFuncName);    
			}
		}
		CDBDoc::GetDocPoint()->m_pDataCtrl->DelTdmf(arFuncNames);
	}
}

void CCMTimeDepMatFuncDlg::OnCmdModify() 
{
	CArray<int,int> arRet;
	CDlgUtil::GetListItemByMask(&m_wndFuncList, arRet, LVIS_FOCUSED);

	if(arRet.GetSize() > 0)
	{
		T_TDMF_D TdmfD;
		T_TDMF_K TdmfK;
		TdmfK = (T_TDMF_K)m_wndFuncList.GetItemData(arRet[0]);

		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmf(TdmfK, TdmfD))
		{
			CCMTdmfItemDlg Dlg; 
			Dlg.SetParamData(TdmfD);
			Dlg.DoModal();
		}
	}
	else
	{
		AfxMessageBox(_LS(IDS_CMD0417__Error___No_Selected_Function__nChoose_));
	}
}

void CCMTimeDepMatFuncDlg::OnDblclkCmdTimeDepMatFlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify();
	*pResult = 0;
}

void CCMTimeDepMatFuncDlg::OnCmdCopy() 
{
	int iItem = m_wndFuncList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if(iItem == -1) return;

	CString csName = m_wndFuncList.GetItemText(iItem, 0);
	CString _tcscpyName = GetCopyName(csName);

	T_TDMF_D TdmfSrc, TdmfCpy;
	T_TDMF_K Key;
	
	Key = (T_TDMF_K)m_wndFuncList.GetItemData(iItem);
	
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->m_pAttrCtrl->GetTdmf(Key, TdmfSrc)) 
	{
		return;
	}

	TdmfCpy = TdmfSrc;
	TdmfCpy.MatlFuncName = _tcscpyName;

	pDoc->m_pDataCtrl->AddTdmf(TdmfCpy);
}

CString CCMTimeDepMatFuncDlg::GetCopyName(CString strOrigin)
{
	CString str = _T("");
	int nIndex = strOrigin.ReverseFind('_');
	
	if (nIndex+1 == strOrigin.GetLength())
	{
		str = strOrigin + _T("1");
	}
	else if (nIndex == -1)
	{
		str = strOrigin + _T("_1");
	}
	else
	{
		BOOL bNumeric = TRUE;
		char cha;
		for (int i=nIndex+1; i<strOrigin.GetLength(); i++)
		{
			cha = strOrigin.GetAt(i);
			if (cha < '0' || cha > '9')
			{
				bNumeric = FALSE;
				break;
			}
		}

		if (bNumeric)
		{
			int nNum = _ttoi(strOrigin.Right(strOrigin.GetLength() - nIndex - 1));
			nNum++;
			CString tmp;
			tmp.Format(_T("_%d"), nNum);
			str = strOrigin.Left(nIndex) + tmp;
		}
		else
		{
			str = strOrigin + _T("_1");
		}
	}

	nIndex = str.ReverseFind('_');
	CString strHead = str.Left(nIndex+1);
	CString strTail;
	int nNum;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	while(pDoc->m_pAttrCtrl->ExistTdmf(str))
	{
		nNum = _ttoi(str.Right(str.GetLength() - nIndex - 1));
		nNum++;
		strTail.Format(_T("%d"), nNum);
		str = strHead + strTail;
	}

	return str;
}
