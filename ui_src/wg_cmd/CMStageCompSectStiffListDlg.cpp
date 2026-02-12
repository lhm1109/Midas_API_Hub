// CMStageCompSectStiffListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageCompSectStiffListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 4

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectStiffListDlg dialog


CCMStageCompSectStiffListDlg::CCMStageCompSectStiffListDlg(int nType, CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMStageCompSectStiffListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageCompSectStiffListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nType = nType; 
}


void CCMStageCompSectStiffListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageCompSectStiffListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMStageCompSectStiffListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageCompSectStiffListDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Control
void CCMStageCompSectStiffListDlg::SetHeaderTitle()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	CString aTitle[] = { _LS(IDS_CMD_SECTION_id), _LS(IDS_CMD_SECTION_name),
											 _LS(IDS_CMD_SECTION_type), _LS(IDS_CMD_SECTION_shape) };

	int aSize[] = { 30, 130, 88, 50 };

	CString title;
	LVCOLUMN lvcolumn;
	for(int i=0; i<COLCOUNT; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		if(i==0) lvcolumn.fmt = LVCFMT_RIGHT;
		else     lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aSize[i]);
		title = aTitle[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMStageCompSectStiffListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSect();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SECT_K Key;
		T_SECT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSect();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_SECT_K), CCompFunc::UINTAsc);

		int ix = 0;
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSect(Key, Data);

			if(m_nType == D_SECT_TYPE_TAPERED)
			{
				if(Data.nStype != D_SECT_TYPE_TAPERED) continue;
			}
			else
			{
				if(Data.nStype == D_SECT_TYPE_TAPERED) continue;
			}

			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=ix;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;

				if( i == 0 )
					m_List.InsertItem(&lvitem);
				else
					m_List.SetItem(&lvitem);

			  str.ReleaseBuffer();
			}
			ix++;
		}
		delete []KeyBuf;
	}
}

CString CCMStageCompSectStiffListDlg::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data)
{
	CString str;

	if(i==0)str.Format(_T("%4d"),Key);
	else if(i==1) str = Data.SName;
	else if(i==2) 
	{
		switch(Data.nStype)
		{
		case D_SECT_TYPE_REGULAR: 
			{
				if (Data.SectBefore.SectI.DBName == _T("")) str = _LS(IDS_WG_CMD__ADD2__User);
				else str =  _LS(IDS_WG_CMD__ADD2__DB);
			}
			break;
		case D_SECT_TYPE_USER:            str = _LS(IDS_WG_CMD__ADDD__Value);       break;
		case D_SECT_TYPE_SRC:             str = _LS(IDS_WG_CMD__ADDD_SECT_SRC);     break;
		case D_SECT_TYPE_COMBINED:        str = _LS(IDS_WG_CMD__ADDD__Combined);    break;
		case D_SECT_TYPE_TAPERED:         str = _LS(IDS_WG_CMD__ADDD__Tapered);     break;
		case D_SECT_TYPE_CONSTRUCTION:    str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_HIBEAM:          str = _LS(IDS_WG_CMD__ADDD__Hi_Beam);     break;
		case D_SECT_TYPE_COLDFORMED:      str = _LS(IDS_WG_CMD__ADDD__Cold_Formed); break;
		case D_SECT_TYPE_COMPO_B:         str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_I:         str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_TUB:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_B:    str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_I:    str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_TUB:  str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CI:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CT:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_PC:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_G:         str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_PSC:             str = _LS(IDS_WG_CMD__ADDD__PSC);         break;
		}
	}
	else if(i==3) 
	{
		str = Data.SectBefore.Shape;
		//if (CLocaleLib::IsEnglish())  // 영어권만 (일본은 H로 씀)
		#if defined(_US) || defined(_RUS)
			str.Replace('H', 'I');
		#endif
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}
/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectStiffListDlg message handlers

BOOL CCMStageCompSectStiffListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);

	CDialogMove::OnInitDialog();

	if(m_nType == 3) m_nType = D_SECT_TYPE_TAPERED;
	else             m_nType = D_SECT_TYPE_USER;
	
	SetHeaderTitle();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMStageCompSectStiffListDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetSectData();
	CDialogMove::OnOK();
}

void CCMStageCompSectStiffListDlg::SetSectData()
{
	T_SECT_K SectK;
	T_SECT_D SectD; SectD.Initialize();

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	SectK = _tstol(m_List.GetItemText(iItem, 0));
	
	T_SECT_SECTBASE_D Data; Data.Initialize();
	BOOL bSrc, bTap;
	int  nLy, nLz;
	
	m_pDoc->m_pAttrCtrl->GetSectStiffness(SectK, FALSE, bSrc, bTap, nLy, nLz, m_StiffDataI, m_StiffDataJ);

	if(bTap) m_nStiffListType = 3;
	else     m_nStiffListType = 2;

}