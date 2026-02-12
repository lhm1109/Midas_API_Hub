// CMDTendonTemplateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDTendonTemplateAssignElem.h"
#include "CMDTendonTemplateDlg.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\DlgUtil.h"

#include "../wg_db/DBLib.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/ViewCtrl.h"



#include "../wg_base/I_GENModelBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMDTendonTemplateAssignElem::CCMDTendonTemplateAssignElem(CWnd* pParent /* = NULL*/)
	:CCMDlgBase(CCMDTendonTemplateAssignElem::IDD, pParent)
{
	m_pParentWnd = (CCMDTendonTemplateDlg*)pParent;

	int i;
	int nSize;

	m_bMsgIgnore = FALSE;

	//취소를 위해서 최초에 복사를 한다.
	m_pAssignElems = m_pParentWnd->GetAssignElemAry();
	nSize = m_pAssignElems->GetSize();
	m_caAssignElems.SetSize(nSize);
	for(i = 0 ; i < nSize; i++)
	{
		m_caAssignElems[i] = (*m_pAssignElems)[i];
	}
}

CCMDTendonTemplateAssignElem::~CCMDTendonTemplateAssignElem()
{
	if(m_pParentWnd)
	{
		if(m_pParentWnd->GetSafeHwnd() && ::IsWindow(m_pParentWnd->GetSafeHwnd()))
		{
			m_pParentWnd->SetShowWnd(TRUE);
			m_pParentWnd->ShowWindow(SW_SHOW);
			m_pParentWnd->RefreshAssignElemCombo();
			m_pParentWnd->SetElemsCbxInit(TRUE);
		}
	}
}


BEGIN_MESSAGE_MAP(CCMDTendonTemplateAssignElem, CCMDlgBase)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_ADD,			OnCmdAdd)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_DEL,			OnCmdDel)
	ON_BN_CLICKED(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_DELALL,		OnCmdDellAll)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_LIST, OnCmdSelChanged)
	ON_EN_CHANGE(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_TXT_ASSIGN, OnCmdAssignElemList)
END_MESSAGE_MAP()

void CCMDTendonTemplateAssignElem::DoDataExchange( CDataExchange* pDX )
{
	CCMDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_LIST,		   m_listElems);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_TXT_ASSIGN, m_wndAssignElem);
}

void CCMDTendonTemplateAssignElem::OnCancel()
{
	DestroyWindow();
}

void CCMDTendonTemplateAssignElem::OnOK()
{
	int i;
	int nSize;
	nSize = m_caAssignElems.GetSize();

	m_pAssignElems->SetSize(nSize);

	for(i = 0 ; i < nSize; i++)
	{
		(*m_pAssignElems)[i] = m_caAssignElems[i];
	}
	DestroyWindow();
}

BOOL CCMDTendonTemplateAssignElem::OnInitDialog()
{
	CCMDlgBase::OnInitDialog();

	SetControl();

	return TRUE;
}

void CCMDTendonTemplateAssignElem::OnUpdate( CWnd* pSender, LPARAM lHint, CObject* pHint )
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_BUFFER_AFTER:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
	case D_UPDATE_EXE_START:
	case D_UPDATE_EXE_END:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_UNIT:
	default:
		break;
	}	    
}

void CCMDTendonTemplateAssignElem::PostNcDestroy()
{
	CCMDlgBase::PostNcDestroy();   
}

void CCMDTendonTemplateAssignElem::SetControl()
{

	m_wndAssignElem.Connect(SC_ID_ELEM, &m_wndAssignElem);  
	
	// Listbox의 헤더를 만든다.
	CStringArray         HTitles;	
	HTitles.Add(_LS(IDS_CMD_MVHL_NO));
	HTitles.Add(_LS(IDS_CMD_TDNA_LIST_Element_No__List));
	
	CArray<float, float> fHRatio;
	fHRatio.Add(0.1f);
	fHRatio.Add(0.9f);
	
	CDlgUtil::_SetListCtrlHeader(&m_listElems, HTitles, &fHRatio, NULL);

	RefreshList();
}

void CCMDTendonTemplateAssignElem::RefreshList()
{
	int i;
	int nSize;

	//리스트 박스를 체운다.
	m_listElems.DeleteAllItems();
	nSize = m_caAssignElems.GetSize();
	
	CStringArray Contents;
	Contents.SetSize(2);

	for(i = 0 ; i < nSize; i++)
	{
		Contents[0].Format(_T("%d"), i + 1);
		Contents[1].Format(_T("%s"), m_caAssignElems[i]);
		CDlgUtil::SetListItem(&m_listElems, i, Contents, 0);
	}
}

void CCMDTendonTemplateAssignElem::OnCmdAdd()
{
	CString strTemp;
	CArray<int, int> aElem;
	int i, nSize;

	m_wndAssignElem.GetWindowText(strTemp);

	if(strTemp.IsEmpty())
		return;
	
	if(!CStrParser::ParsingListByTo(strTemp, aElem))
	{
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_EXIST_ELEM));
		return;
	}

	m_pParentWnd->UnselectAll();

	nSize = m_caAssignElems.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		if( m_caAssignElems[i] == strTemp )
		{
			//이미 등록.
			SetElemsCbxInit();
			AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_SELECT_ALREADY_ADDED_ELEM));
			return;
		}
	}  
	
	T_SECT_K keySectCur = m_pParentWnd->GetTemplateCtrl()->GetSectKeyByElemsStr(strTemp);
	if(keySectCur > 0)
	{
		nSize = m_caAssignElems.GetSize();
		T_SECT_K keySectTemp;
		for(i = 0 ; i < nSize; i++)
		{
			keySectTemp = m_pParentWnd->GetTemplateCtrl()->GetSectKeyByElemsStr(m_caAssignElems[i]);
			
			if(keySectTemp != keySectCur)
			{
				SetElemsCbxInit();
				AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_DIFF_SECT));
				return;
			}
		}
	}
	else
	{
		SetElemsCbxInit();
		AfxMessageBox(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_DIFF_SECT));
		return;
	}

	m_caAssignElems.Add(strTemp);
	RefreshList();
	SetElemsCbxInit(TRUE);
}

void CCMDTendonTemplateAssignElem::OnCmdDel()
{
	int i;
	int nSize;
	CArray<int, int> arRetIdx;
		
	nSize = m_listElems.GetItemCount();
	if(nSize != m_caAssignElems.GetSize())
	{
		ASSERT(0);
		return;
	}
	
	nSize = m_listElems.GetItemCount();
	if(nSize != m_caAssignElems.GetSize())
	{
		ASSERT(0);
		return ;
	}

	
	for( i = nSize - 1 ; i >= 0 ; i--)
	{
		if(m_listElems.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_caAssignElems.RemoveAt(i);
		}
	}

	RefreshList();
}

void CCMDTendonTemplateAssignElem::OnCmdDellAll()
{
	m_listElems.DeleteAllItems();
	m_caAssignElems.RemoveAll();
}

void CCMDTendonTemplateAssignElem::OnCmdSelChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(m_bMsgIgnore)
		return;

	CArray<int, int> arIdx;
	CString strTemp;
	if(CDlgUtil::GetSelectedListItemIndex(&m_listElems, arIdx))
	{
		//선택한 리스트 항목이 1개라면. 실제로 선택한다....
		if(arIdx.GetSize() == 1)
		{
			strTemp = m_listElems.GetItemText(arIdx[0], 1);
			m_wndAssignElem.SetWindowText(strTemp);
			m_wndAssignElem.SelectByStr(strTemp);
		}
	}
}

void CCMDTendonTemplateAssignElem::OnCmdAssignElemList()
{
	CString strTemp;
	m_wndAssignElem.GetWindowText(strTemp);
	T_SECT_K keySect = m_pParentWnd->GetTemplateCtrl()->GetSectKeyByElemsStr(strTemp);
	if(keySect > 0) //키가 valid 하다면. 모든 요소가 하나의 단면.
	{
		GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_ADD)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CMD_TENDON_TEMPLATE_ASSIGN_DETAIL_ADD)->EnableWindow(FALSE);
	}
}

void CCMDTendonTemplateAssignElem::SetElemsCbxInit(BOOL bLast)
{
	int nCount = m_listElems.GetItemCount();
	if( nCount > 0 )
	{
		int nSel;
		nSel = nCount - 1;
		m_wndAssignElem.SetWindowText(m_caAssignElems[nSel]);
	}
}

BOOL CCMDTendonTemplateAssignElem::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_DELETE)
		{
			OnCmdDel(); //삭제.
		}    
	}
	
	return CCMDlgBase::PreTranslateMessage(pMsg);  
}
