// CMTimeDefMatTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTimeDefMatTypeDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "CMHcrpDlg.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTimeDefMatTypeDlg dialog
#define CDialog CDialogMove

CCMTimeDefMatTypeDlg::CCMTimeDefMatTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMTimeDefMatTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTimeDefMatTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}

void CCMTimeDefMatTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTimeDefMatTypeDlg)
	DDX_Control(pDX, IDC_CMD_TIME_DEP_MAT_LIST, m_wndList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMTimeDefMatTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CCMTimeDefMatTypeDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_ADD, OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_MODIFY, OnCmdModify)
	ON_BN_CLICKED(IDC_CMD_DELETE, OnCmdDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TIME_DEP_MAT_LIST, OnDblclkCmdTimeDepMatList)
	ON_BN_CLICKED(IDC_CMD_COPY, OnCmdCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTimeDefMatTypeDlg message handlers



BOOL CCMTimeDefMatTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetListHeader();
	UpdateTdmtList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTimeDefMatTypeDlg::SetListHeader()
{
	CStringArray Titles             ;
	CArray<float, float> HWidthRatio;
	Titles.Add(_LS(IDS_CMD0417__Name));
	Titles.Add(_LS(IDS_CMD_HSFC_code));
	Titles.Add(_LS(IDS_CMD_HMTP_type));
	
	HWidthRatio.Add(0.3f);
	HWidthRatio.Add(0.7f);
	HWidthRatio.Add(0.0f);

	CDlgUtil::_SetListCtrlHeader(&m_wndList,Titles,&HWidthRatio,NULL);
}

void CCMTimeDefMatTypeDlg::OnCmdClose() 
{
	OnOK();
}

void CCMTimeDefMatTypeDlg::OnCmdAdd() 
{
	// TODO: Add your control notification handler code here
	CCMHcrpDlg Dlg;
	Dlg.SetTimeDepMatMode(TRUE,0);  // Add Mode
	Dlg.DoModal();
	
}

void CCMTimeDefMatTypeDlg::OnCmdDelete() 
{
	CArray <int, int> arRet;
	CArray <T_TDMT_K, T_TDMT_K> arKeys;
	CArray <CString, CString&> arTNames;
	
	T_TDMT_K TdmtK;
	T_TDMT_D TdmtD;
	CDlgUtil::GetListItemByMask(&m_wndList,arRet,LVIS_SELECTED);

	int narRet = arRet.GetSize();
	if(narRet)
	{
		for(int i = 0 ; i < narRet; i++)
		{
			TdmtK = (T_TDMT_K) m_wndList.GetItemData(arRet[i]);
			if(m_pDoc->m_pAttrCtrl->GetTdmt(TdmtK,TdmtD))
			{
				arTNames.Add(TdmtD.strMatlName);
			}
		}
	}
	m_pDoc->m_pDataCtrl->DelTdmt(arTNames);
}

void CCMTimeDefMatTypeDlg::OnCmdModify() 
{
	// TODO: Add your control notification handler code here
	CCMHcrpDlg Dlg;
	Dlg.SetTimeDepMatMode(TRUE,1);  // Modify Mode

	CArray<int, int > arRet;
	CDlgUtil::GetListItemByMask(&m_wndList,arRet,LVIS_SELECTED);
	int nAr = arRet.GetSize();
	
	if(nAr != 1)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Error___No_Selected_Type__nChoose_one_));
		return;
	}

	T_TDMT_K TdmtK;
	T_TDMT_D TdmtD;
	TdmtK = (T_TDMT_K) m_wndList.GetItemData(arRet[0]);

	if(m_pDoc->m_pAttrCtrl->GetTdmt(TdmtK,TdmtD))
	{
		Dlg.SetCurTDMTData(&TdmtD);
		Dlg.DoModal();
	}
}

void CCMTimeDefMatTypeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(GetSafeHwnd())
	{
		switch(lHint)
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
}


void CCMTimeDefMatTypeDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdmtCommand = FALSE;	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		 
		case(UR_TDMT_ADD):
		case(UR_TDMT_DEL):
		case(UR_TDMT_MFD): 
		case(UR_TDMT_MFS):
					bIsTdmtCommand = TRUE;
				break;
		default:
			break;
		}
	} 
	
	if(bIsTdmtCommand) 
	{
		UpdateTdmtList();
	}
}


void CCMTimeDefMatTypeDlg::UpdateTdmtList()
{
	CArray<T_TDMT_K, T_TDMT_K> rKeyList;
	T_TDMT_D TdmtD;
	
	m_pDoc->m_pAttrCtrl->GetTdmtKeyList(rKeyList);

	int nTdmt = rKeyList.GetSize();
	CStringArray Contents;
	CStringArray CodeArr;
	CStringArray TypeArr;
	
	/* 1*/CodeArr.Add(_T("ACI")); 
	/* 2*/CodeArr.Add(_T("CEB-FIP(1990)"));
	/* 3*/CodeArr.Add(_LS(IDS_CMD0417__Modify_Elasticity_Modulus)); 
	/* 4*/CodeArr.Add(_LS(IDS_CMD0417__USER));
	/* 5*/CodeArr.Add(_LS(IDS_CMD0417__Korean_Standard));
	/* 6*/CodeArr.Add(_T("PCA"));
	/* 7*/CodeArr.Add(_LS(IDS_CMD0417__Combined_ACI_PCA_));
	/* 8*/CodeArr.Add(_T("JAPAN"));
	/* 9*/CodeArr.Add(_T("CHINA"));
	/*10*/CodeArr.Add(_T("JAPAN(JSCE)"));
	/*11*/CodeArr.Add(_LS(IDS_CMD_TDMT_CHJTG04));
	/*12*/CodeArr.Add(_T("AASHTO"));
	/*13*/CodeArr.Add(_T("CEB-FIP(1978)"));
	/*14*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__INDIA_STANDARD));
	/*15*/CodeArr.Add(_T("European"));
	/*16*/CodeArr.Add(_LS(IDS_CMD0417__Japane_JSCE07));
	/*17*/CodeArr.Add(_T(""));
	/*18*/CodeArr.Add(_T(""));
	/*19*/CodeArr.Add(_T(""));
	/*20*/CodeArr.Add(_T("KSCE 2010"));	
	/*21*/CodeArr.Add(_T("KCI-USD12"));	
	/*22*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009));
	/*23*/CodeArr.Add(_T(""));
	/*24*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011));
	/*25*/CodeArr.Add(_T("INDIA(IRC:112-2011)"));
	/*26*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__RUSSIAN));
	/*27*/CodeArr.Add(_T("CEB-FIP(2010)"));
	/*28*/CodeArr.Add(_LS(IDS_CMD_TDMT_CHJTG3362_18));
	/*29*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016));
	/*30*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__NEWZEALAND));
	/*31*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017));
	/*32*/CodeArr.Add(_T(""));
	/*33*/CodeArr.Add(_T("KDS-2016"));
	/*34*/CodeArr.Add(_LS(IDS_CMD0417__Japane_JSCE12));
    /*35*/CodeArr.Add(_T("INDIA(IRC:112-2020)"));
	/*36*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__NEWZEALAND_2022));
	/*37*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD));
	/*38*/CodeArr.Add(_LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_5100_2017_AMD));
	/*39*/CodeArr.Add(_LS(IDS_CMD_TDMT_CHJTG_T_D65_2015));
	/*40*/CodeArr.Add(_LS(IDS_CMD_TDMT_ABNT_NBR_6118_2023));

	TypeArr.Add(_LS(IDS_CMD0417__Creep));
	TypeArr.Add(_LS(IDS_CMD0417__Shrinkage));
	TypeArr.Add(_LS(IDS_CMD0417__Creep_Shrinkage));
	
	m_wndList.DeleteAllItems();
	
	for(int i = 0 ; i < nTdmt ; i++)
	{
		Contents.RemoveAll();
		if(m_pDoc->m_pAttrCtrl->GetTdmt(rKeyList[i],TdmtD))
		{
			Contents.Add(TdmtD.strMatlName);
			Contents.Add(CodeArr[TdmtD.COMMON.nCode-1]);
			Contents.Add(_T(""));
			///Contents.Add(TypeArr[TdmtD.COMMON.nType-1]);
		}
		CDlgUtil::SetListItem(&m_wndList,i,Contents,rKeyList[i]);
	}
}

void CCMTimeDefMatTypeDlg::OnDblclkCmdTimeDepMatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdModify(); 
	
	*pResult = 0;
}

void CCMTimeDefMatTypeDlg::OnCmdCopy() 
{
	int iItem = m_wndList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		return;
	}
	CString csName = m_wndList.GetItemText(iItem, 0);
	
	CString _tcscpyName = GetCopyName(csName);

	T_TDMT_D TdmtSrc, TdmtCpy;
	T_TDMT_K Key;
	
	Key = (T_TDMT_K)m_wndList.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetTdmt(Key, TdmtSrc)) 
	{
		return;
	}

	TdmtCpy = TdmtSrc;
	TdmtCpy.strMatlName = _tcscpyName;

	m_pDoc->m_pDataCtrl->AddTdmt(TdmtCpy);
}

CString CCMTimeDefMatTypeDlg::GetCopyName(CString strOrigin)
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

	while(m_pDoc->m_pAttrCtrl->ExistTdmt(str))
	{
		nNum = _ttoi(str.Right(str.GetLength() - nIndex - 1));
		nNum++;
		strTail.Format(_T("%d"), nNum);
		str = strHead + strTail;
	}

	return str;
}
