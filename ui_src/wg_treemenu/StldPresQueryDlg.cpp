#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPresQueryDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\ChildFrmBase.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\ViewBuff.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_gr\ILabelMgr_CH.h"
#include "..\wg_gr\wg_gr.h"
#include "..\wg_gr\GRAMRUDFData.h"
#include "..\wg_gps\wg_gpsAll.h"

///////////////////////////////////////////////////////////////////////////////
//AutoAssignColor
AutoAssignColor::AutoAssignColor()
{
	InitColorTable();
}
void AutoAssignColor::InitColorTable()
{
	// R->G->B Plus Violet
	// Color Table Begins With Violet
	m_RGBColor[0][0]				= RGB(  0,   0, 255);	// Blue
	m_RGBColor[0][1]				= RGB(  0, 255, 255);
	m_RGBColor[0][2]				= RGB(  0, 255,   0);	// Green
	m_RGBColor[0][3]				= RGB(255, 255,   0);
	m_RGBColor[0][4]				= RGB(255,   0,   0);	// Red
	m_RGBColor[0][5]				= RGB(255,   0, 255);
	m_RGBColor[0][CONTOUR_RANK_NUM]	= RGB(170,   0, 170);	// Violet

	// R->G->B
	// R, B values extend through half range
	m_RGBColor[1][0]				= RGB(  0,   0, 255);	// Blue
	m_RGBColor[1][1]				= RGB(  0, 255, 255);
	m_RGBColor[1][2]				= RGB(  0, 255,   0);	// Green
	m_RGBColor[1][3]				= RGB(170, 255,   0);
	m_RGBColor[1][4]				= RGB(255, 255,   0);	// Yellow
	m_RGBColor[1][5]				= RGB(255, 127,   0);
	m_RGBColor[1][6]	= RGB(255,   0,   0);	// Red
	//m_RGBColor[1][CONTOUR_RANK_NUM]	= RGB(255,   0,   0);	// Red

	// R->B->G
	// R, G values extend through half range
	m_RGBColor[2][0]				= RGB(  0, 255,   0);	// Green
	m_RGBColor[2][1]				= RGB(  0, 255, 170);
	m_RGBColor[2][2]				= RGB(  0, 170, 255);
	m_RGBColor[2][3]				= RGB(  0,   0, 255);	// Blue
	m_RGBColor[2][4]				= RGB(170,   0, 255);
	m_RGBColor[2][5]				= RGB(255,   0, 170);
	m_RGBColor[2][CONTOUR_RANK_NUM]	= RGB(255,   0,   0);	// Red

	m_RGBColor[3][0]				= RGB(255, 255, 255);	// White
	m_RGBColor[3][1]				= RGB(212, 212, 212);
	m_RGBColor[3][2]				= RGB(170, 170, 170);
	m_RGBColor[3][3]				= RGB(127, 127, 127);
	m_RGBColor[3][4]				= RGB( 85,  85,  85);
	m_RGBColor[3][5]				= RGB( 42,  42,  42);
	m_RGBColor[3][CONTOUR_RANK_NUM]	= RGB(  0,   0,   0);	// Black

	//MakeColorTable()
	//int		i, j, k, div_num=CONTOUR_RANK_NUM/6;
	//for(int i=0;i<COLOR_TABLE_NUM;i++) 
	//{
	//	for (j=1;j<div_num;j++) 
	//	{
	//		for(k=0;k<6;k++) 
	//		{
	//			if (k == 5) 
	//			{
	//				m_RGBColor[i][j*6+k] = GetSubColor(m_RGBColor[i][k], m_RGBColor[i][CONTOUR_RANK_NUM], div_num, j);
	//			}
	//			else 
	//			{
	//				m_RGBColor[i][j*6+k] = GetSubColor(m_RGBColor[i][k], m_RGBColor[i][k+1], div_num, j);
	//			}
	//		}
	//	}
	//}
	m_CurrentColorTable = 1;//R->G->B
}

COLORREF AutoAssignColor::GetSubColor(COLORREF col1, COLORREF col2, double div_num, double order)
{
	BYTE	r_unit, g_unit, b_unit;

	if(fabs(div_num) < 1e-9)
	{
		ASSERT(FALSE);
		r_unit = 0.0;
		g_unit = 0.0;
		b_unit = 0.0;
	}
	else
	{
		r_unit = (BYTE) (((double) (GetRValue(col2)-GetRValue(col1)))/div_num);
		g_unit = (BYTE) (((double) (GetGValue(col2)-GetGValue(col1)))/div_num);
		b_unit = (BYTE) (((double) (GetBValue(col2)-GetBValue(col1)))/div_num);
	}
	return RGB(GetRValue(col1)+(int)(r_unit*order), GetGValue(col1)+(int)(g_unit*order), GetBValue(col1)+(int)(b_unit*order));
}

void AutoAssignColor::MakeRankMap(CArray<double>& aValule)
{
	memset(m_RankMap, 0, CONTOUR_RANK_NUM*sizeof(double));
	m_MinRange = 1e10;
	m_MaxRange = -1e10;
	m_Value.Copy(aValule);
	//qsort(m_Value.GetData(),m_Value.GetSize(),sizeof(double),CCompFunc::doubleAsc);//ASSER Sort
	for(int i = 0;i < m_Value.GetSize();i++)
	{
		m_MinRange = min(m_MinRange,aValule[i]);
		m_MaxRange = max(m_MaxRange,aValule[i]);
	}
	int nCount = m_Value.GetSize();

	for(int i = 0;i < 7;i++)
		m_RankMap[i] = -1e9;
	if (nCount < 7)
	{
		if (nCount > 1)
		{
			int n = nCount % 7;
			int m = 0;
			int t = (7 - n) / (n - 1);
			int r = (7 - n) % (n - 1);
			int p = 0;
			for (int i = 0; i < nCount; i++)
			{
				if (i + m < 0 || i + m > 6)
					break;
				double ba = m_Value[i];
				m_RankMap[i + m] = (double)m_Value[i];
				p += r;
				m += t + p / (n - 1);
			}
		}
		m_RankMap[0] = m_Value[0];
		m_RankMap[6] = m_Value[nCount - 1];
		double value = m_RankMap[0];
		for (int i = 1; i < 7; i++)
		{
			if (m_RankMap[i] < value)
				m_RankMap[i] = value;
			else
				value = m_RankMap[i];
		}
	}
	else
	{
		int m = 0;
		double t = (double)nCount / 7;
		for (int i = 0; i < 7; i++)
		{
			m_RankMap[i] = m_Value[(int)(t * i)];
		}
	}

	//if(nCount < 7)
	//{
	//	//int n = nCount - 2;
	//	if(nCount < 3)
	//	{
	//		for(int i = 1;i < 6;i++)
	//			m_RankMap[i] = m_MinRange;
	//	}
	//	if(nCount == 3)
	//	{
	//		for(int i = 1;i < 6;i++)
	//			m_RankMap[i] = m_Value[i/3];
	//	}
	//	else if(nCount == 4)
	//	{
	//		//2 4 ----1   2
	//		for(int i = 1;i < 6;i++)
	//		{
	//			m_RankMap[i] = m_Value[i/2];
	//		}
	//	}
	//	else if(nCount == 5)
	//	{
	//		//1 3 5	---	1 2 3
	//		for(int i = 1;i < 6;i++)
	//			m_RankMap[i] = m_Value[(i+1)/2];
	//	}
	//	else if(nCount == 6)
	//	{
	//		//1 2 3 4 --- 
	//		for(int i = 1;i < 5;i++)
	//			m_RankMap[i] = m_Value[i];
	//		m_RankMap[5] = m_MinRange;
	//	}
	//}
	//else
	//{
	//	//double div = (m_MaxRange - m_MinRange)/6.0;
	//	int k = (nCount-2)/5;
	//	for(int i = 1;i < 6;i++)
	//		m_RankMap[i] = m_Value[i*k];// div*i + m_MinRange;
	//}
	//m_RankMap[0] = m_MinRange;// + 1e-9;
	//m_RankMap[6] = m_MaxRange;

	m_CurrentRankNum = nCount;
}
COLORREF AutoAssignColor::GetColor(double val)
{
	int nRank = 0;
	for(int i=0;i < 7;i++) 
	{
		if(val < m_RankMap[i] + 1e-9)
		{
			nRank = i-1;
			break;
		}
	}
	if(m_CurrentRankNum < 7)
	{
		return m_RGBColor[m_CurrentColorTable][nRank+1];
	}
	return GetSubColor(m_RGBColor[m_CurrentColorTable][nRank],m_RGBColor[m_CurrentColorTable][nRank+1],m_RankMap[nRank+1]-m_RankMap[nRank],val-m_RankMap[nRank]);
}

///////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CStldPresQueryDlg dialog
#define CDialog CMenuBarChildDlg
typedef UINT   T_GROUP_K;

int CStldPresQueryDlg::CompareStlCaseColorValue(const void* arg1, const void* arg2)
{
	if( ((StldCaseColorValue*)arg1)->value > ((StldCaseColorValue*)arg2)->value)  return 1;
	return -1;
}

CStldPresQueryDlg::CStldPresQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldPresQueryDlg::IDD, pParent)
{
	m_nInputType = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	
}

CStldPresQueryDlg::~CStldPresQueryDlg()
{
	RemoveLColor();
	ILabelMgr_CH::instance()->UnDisplayPresTypeColor();
}

void CStldPresQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TM_LDGR_CBO, m_CobxLdgr);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_CobxLC);
	DDX_Control(pDX, IDC_TM_CBO_LTYPE, m_CobxPslt);  
	DDX_Radio(pDX, IDC_TM_OPT_LOAD_TYPE, m_nInputType);
	DDX_Control(pDX, IDC_TM_STLD_PRES_QUERY_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldPresQueryDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_TM_CBO_LC			, ChangeLoadCase)
	ON_BN_CLICKED(IDC_TM_OPT_LOAD_TYPE, ChangeTypeRadio)
	ON_BN_CLICKED(IDC_TM_OPT_LOAD_CASE, ChangeTypeRadio)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TM_STLD_PRES_QUERY_LIST, OnNMCustomdrawList)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_DEFINE_COLOR_BTN, OnShowNumDetDlg)
END_MESSAGE_MAP()


BOOL CStldPresQueryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitHeaderTitle();
	UpdateData(FALSE);
	m_CobxPslt.EnableWindow(m_nInputType == 0);
	m_CobxLC.EnableWindow(m_nInputType == 1);
	UpdataListData();
	return TRUE;
}

void CStldPresQueryDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);		
}

void CStldPresQueryDlg::ChangeTypeRadio()
{
	int PreInputType = m_nInputType;
	UpdateData();
	if(m_nInputType == PreInputType)
		return;
	BOOL bShowLType = m_nInputType == 0;
	m_CobxPslt.EnableWindow(bShowLType);
	m_CobxLC.EnableWindow(!bShowLType);
	InitHeaderTitle();
	UpdateListCtrl();
}

void CStldPresQueryDlg::UpdateListCtrl()
{
	LV_ITEM lvitem;
	CString str;
	m_mapColor.RemoveAll();
	m_List.DeleteAllItems();
	if(m_nInputType == 0)
	{
		//m_CurPsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
		int nItemCount = m_CobxPslt.GetCount();
		if(nItemCount == 0) return;
		T_PSLT_K Key;
		T_PSLT_D Data;
		for(int nItem = 1;nItem < nItemCount;nItem++)
		{
			Key = m_CobxPslt.GetItemData(nItem);
			m_pDoc->m_pAttrCtrl2->GetPslt(Key,Data);
			for(int i = 0; i < 3; i++)
			{
				lvitem.iItem = nItem-1;
				lvitem.iSubItem=i;
				str = DataToStr_LP(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
				if( i == 0 )
				{
					lvitem.mask |= LVIF_PARAM;
					lvitem.lParam = (LPARAM)Key;
					m_List.InsertItem(&lvitem);
					m_mapColor.SetAt(nItem-1,Data.color);
				}
				else
					m_List.SetItem(&lvitem);
				str.ReleaseBuffer();
			}
		}
	}
	else
	{
		ChangeLoadCase();
	}
}

void CStldPresQueryDlg::ChangeLoadCase()
{
	UpdateData();
	int nCurSel = m_CobxLC.GetCurSel();
	if(nCurSel == -1)
		nCurSel = 0;
	int nItemCount = m_CobxLC.GetCount();
	if(nItemCount == 0) return;
	T_STLD_K Key = m_CobxLC.GetItemData(nCurSel);
	D_STLDCOLOR_ARRAY* aStlColor = 0;
	if(!m_mapStlColor.Lookup(Key,aStlColor))
	{
		ASSERT(FALSE);
		return;
	}
	m_List.DeleteAllItems();
	m_mapColor.RemoveAll();
	CString str;
	LV_ITEM lvitem;
	for(int nItem = 0;nItem < aStlColor->GetSize();nItem++)
	{
		for(int i = 0; i < 2; i++)
		{
			lvitem.iItem = nItem;
			lvitem.iSubItem=i;
			lvitem.mask=LVIF_TEXT ;
			if( i == 0 )
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)Key;
				lvitem.pszText = _T("");
				m_List.InsertItem(&lvitem);
			}
			else
			{
				CString strFormat;
				if (m_NumDetParam.num_ExponentFlag)
					strFormat.Format(_T("%%.%de"), m_NumDetParam.num_DecimalPoint);
				else
					strFormat.Format(_T("%%.%df"), m_NumDetParam.num_DecimalPoint);

				str.Format(strFormat,aStlColor->GetAt(nItem).value);
				m_mapColor.SetAt(nItem,aStlColor->GetAt(nItem).color);
				lvitem.pszText=str.GetBuffer(0);
				m_List.SetItem(&lvitem);
			}
		}
	}
}


CString CStldPresQueryDlg::DataToStr_LP(int i, T_PSLT_K Key, T_PSLT_D &Data)
{
	CString str = _T("");
	if(i == 1) str = Data.strName;
	else if(i == 2)
	{
		T_STLD_D StlData;
		CString strNum = _T("");
		//
		CString strFormat;
		if(m_NumDetParam.num_ExponentFlag)
			strFormat.Format(_T("%%.%de"), m_NumDetParam.num_DecimalPoint);
		else
			strFormat.Format(_T("%%.%df"), m_NumDetParam.num_DecimalPoint);
		//
		for(int i = 0;i < PSLT_NUM;i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetStld(Data.LoadCaseKey[i],StlData))
				continue;
			if(Data.nElemType != 0)
			{
				ASSERT(FALSE);
				continue;
			}
			if(!str.IsEmpty())
				str += _T(",");
			if(Data.nLoadType[i] == 0)
			{
				strNum.Format(strFormat,Data.dLoadP1[i]);
				str += StlData.LoadCaseName + _T(":") + strNum;
			}
			else
			{
				str += StlData.LoadCaseName + _T(":") + _T("(");
				bool bexist = false;
				if (fabs(Data.dLoadP1[i]) > 1e-9)
				{ 
					strNum.Format(_T("P1:") + strFormat, Data.dLoadP1[i]); 
					str += strNum; 
					bexist = true; 
				};
				if (fabs(Data.dLoadP2[i]) > 1e-9)
				{ 
					strNum.Format(_T("P2:") + strFormat, Data.dLoadP2[i]); 
					if (bexist)str += _T(",");
					str += strNum;
					bexist = true;
				};
				if (fabs(Data.dLoadP3[i]) > 1e-9) 
				{
					strNum.Format(_T("P3:") + strFormat, Data.dLoadP3[i]);
					if (bexist)str += _T(",");
					str += strNum;
					bexist = true;
				};
				if (fabs(Data.dLoadP4[i]) > 1e-9) 
				{
					strNum.Format(_T("P4:") + strFormat, Data.dLoadP4[i]);
					if (bexist)str += _T(",");
					str += strNum;
					bexist = true;
				};
				str += _T(")");
			}
		}
	}
	return str;
}

void CStldPresQueryDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_UNIT:
		InitHeaderTitle();
		UpdataListData();
		OnTmExecute();
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	//case D_UPDATE_LOADCASE_CHANGED:
	//	break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CStldPresQueryDlg::UpdateBuffer()
{
	CString strNewTypeName;

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();


	BOOL bPresChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case LT_PRES_CMD:case UR_PRES_ADD:case UR_PRES_DEL:
		//case LT_PSLT_CMD:case UR_PSLT_ADD:case UR_PSLT_DEL:case UR_PSLT_MFS:case UR_PSLT_MFD://PsltCmd Will Trigger Pres
			bPresChanged = TRUE;
			break;
		default:
			break;
		}
	}
	if(bPresChanged)
	{	
		UpdataListData();
		if(m_nInputType == 0)
			SetLabelMgrLPData();
		else
			SetLabelMgrLCData();
		if(ILabelMgr_CH::instance()->m_bDrawPresTypeColor)
			ILabelMgr_CH::instance()->SetDrawPresQueryColorStatus();
	}
}

void CStldPresQueryDlg::UpdataListData()
{
	SetCoboxLType();
	{//UpdateLoad Case Data
		RemoveLColor();
		StldCaseColorValue StlColor;
		T_PSLT_K Key;
		T_PSLT_D Data;
		//m_aStlK.RemoveAll();
		//auto AddstldColorWithAddSamePslt = [&](D_STLDCOLOR_ARRAY* pStlColor,StldCaseColorValue& value)
		//{
		//	if(!pStlColor)
		//		return;
		//	if(pStlColor->IsEmpty())
		//	{
		//		pStlColor->Add(value);
		//		return;
		//	}
		//	for(int k = 0;k < pStlColor->GetSize();k++)
		//	{
		//		if(fabs(pStlColor->GetAt(k).value-value.value) < 1e-9)
		//		{
		//			pStlColor->GetAt(k).aPsltK.Add(value.aPsltK[0]);
		//			return;
		//		}
		//	}
		//	pStlColor->Add(value);
		//};
		//
		for(int nItem = 1;nItem < m_CobxPslt.GetCount();nItem++)
		{
			Key = m_CobxPslt.GetItemData(nItem);

			if(!m_pDoc->m_pAttrCtrl2->GetPslt(Key,Data))
			{
				ASSERT(FALSE);
				continue;
			}
			for(int i = 0;i < PSLT_NUM;i++)
			{
				if(Data.LoadCaseKey[i] != 0 /*&& Data.nLoadType[i] == 0*/)
				{
					D_STLDCOLOR_ARRAY* aStlColor = 0;

					if(!m_mapStlColor.Lookup(Data.LoadCaseKey[i],aStlColor))
					{
						aStlColor = new D_STLDCOLOR_ARRAY;
						m_mapStlColor[Data.LoadCaseKey[i]] = aStlColor;
					}

					StlColor.init();
					StlColor.aPsltK.Add(Key);
					StlColor.value = Data.dLoadP1[i];
					StlColor.color = Data.color;
					int nIndex = 0;
					BOOL bExistPstl = FALSE;
					int k = 0;
					for(;k < aStlColor->GetSize();k++)
					{
						if(aStlColor->GetAt(k).aPsltK[0] == Key)
						{
							bExistPstl = TRUE;
							break;
						}
					}
					if(bExistPstl)
						aStlColor->GetAt(k).value += StlColor.value;
					else
						aStlColor->Add(StlColor);
				}
			}
		}
		//Remove Same Data And Move to it StldCaseColorValueKeyList
		T_STLD_K stlK;
		POSITION pos = m_mapStlColor.GetStartPosition();
		while(pos)
		{
			D_STLDCOLOR_ARRAY* aStlColor;
			m_mapStlColor.GetNextAssoc(pos,stlK,aStlColor);
			CArray<double> aExistValue;
			CArray<T_STLD_K> aExistStldK;
			for(int i = aStlColor->GetSize()-1;i >=0;i--)
			{
				BOOL bExist = FALSE;
				for(int j = 0;j < aExistValue.GetSize();j++)
				{
					if(fabs(aExistValue[j]-aStlColor->GetAt(i).value) < 1e-9)
					{
						bExist = TRUE;
						break;
					}
				}
				if(bExist)
				{
					if(aStlColor->GetSize() > i+1)
						aStlColor->GetAt(i+1).aPsltK.Add(aStlColor->GetAt(i).aPsltK[0]);
					else
						ASSERT(FALSE);
					aStlColor->RemoveAt(i);
				}
				else
				{
					aExistValue.Add(aStlColor->GetAt(i).value);
				}
			}
		}
	}
	SetCoboxLCase();
	UpdataLCaseColor();
	UpdateListCtrl();
}

void CStldPresQueryDlg::SetCoboxLType()
{
	int nCurItem = m_CobxPslt.GetCurSel();
	if(nCurItem == -1)
		nCurItem = 0;
	m_CobxPslt.ResetContent();
	CArray<T_PSLT_K,T_PSLT_K> aPsltK;
	T_PSLT_D PsltD;
	CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPsltKeyList(aPsltK);
	m_CobxPslt.SetItemData(m_CobxPslt.AddString(_T("All")),0);
	for(int i=0; i<aPsltK.GetSize(); i++)
	{
		T_PSLT_K PsltK = aPsltK[i];
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) continue;
		if(PsltD.nElemType != 0)continue;//Only Add Pres
		int nIndex = m_CobxPslt.AddString(PsltD.strName);
		m_CobxPslt.SetItemData(nIndex,PsltK);
	}
	if(nCurItem >= aPsltK.GetSize())
		nCurItem = 0;
	m_CobxPslt.SetCurSel(nCurItem);
}

void CStldPresQueryDlg::SetCoboxLCase()
{
	int nCurItem = m_CobxLC.GetCurSel();
	if(nCurItem == -1)
		nCurItem = 0;
	m_CobxLC.ResetContent();
	int nCount = m_mapStlColor.GetCount();
	if(nCount == 0)
		return;
	T_PSLT_D PsltD;
	POSITION pos = m_mapStlColor.GetStartPosition();
	T_STLD_K stlK;
	T_STLD_D stlD;
	D_STLDCOLOR_ARRAY* aData;
	while(pos)
	{
		m_mapStlColor.GetNextAssoc(pos,stlK,aData);
		VERIFY(m_pDoc->m_pAttrCtrl->GetStld(stlK,stlD));
		m_CobxLC.SetItemData(m_CobxLC.AddString(stlD.LoadCaseName),stlK);
	}
	if(nCurItem >= nCount)
		nCurItem = 0;
	m_CobxLC.SetCurSel(nCurItem);
}

void CStldPresQueryDlg::UpdataLCaseColor()
{
	T_STLD_K stlK;
	POSITION pos = m_mapStlColor.GetStartPosition();
	while(pos)
	{
		D_STLDCOLOR_ARRAY* aStlColor;
		m_mapStlColor.GetNextAssoc(pos,stlK,aStlColor);
		qsort(aStlColor->GetData(),aStlColor->GetSize(),sizeof(StldCaseColorValue),CompareStlCaseColorValue);
		//Assign aStlColor Color from small to large
		CArray<double> aVal;
		for(int i = 0;i < aStlColor->GetSize();i++)
		{
			aVal.Add(aStlColor->GetAt(i).value);
		}
		m_CustomColor.MakeRankMap(aVal);
		for(int i = 0;i < aStlColor->GetSize();i++)
		{
			aStlColor->GetAt(i).color = m_CustomColor.GetColor(aStlColor->GetAt(i).value);
		}
	}
}

void CStldPresQueryDlg::RemoveLColor()
{
	POSITION pos = m_mapStlColor.GetStartPosition();
	D_STLDCOLOR_ARRAY* aData;
	T_STLD_K stlK;
	while(pos)
	{
		m_mapStlColor.GetNextAssoc(pos,stlK,aData);
		if(aData)
		{
			delete aData;
			aData = 0;
		}
	}
	m_mapStlColor.RemoveAll();
}

void CStldPresQueryDlg::InitHeaderTitle()
{
	m_List.DeleteAllItems();
	while(m_List.DeleteColumn(0));
	if(m_nInputType == 0)
	{	
		CString aTitle[] = { _LS(IDS_WG_TREEMENU_Color),_LS(IDS_WG_TREEMENU_Loads_Type),_LS(IDS_WG_TREEMENU_Load) };
		aTitle[2] += _T("(") + m_pDoc->m_pUnitCtrl->m_strStress + _T(")");
		int nColWidth[] = {40,60,160};
		CString title;
		int i;
		LV_COLUMN lvcolumn;

		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
		dwStyle |= LVS_EX_FULLROWSELECT; 
		ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
		// Set Title
		for(i = 0; i < 3; i++)
		{
			title = aTitle[i];
			lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_LEFT;

			lvcolumn.iSubItem = i;
			lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
			lvcolumn.pszText = title.GetBuffer(0);
			m_List.InsertColumn(i,&lvcolumn);
			title.ReleaseBuffer();
		}

	}
	else
	{
		CString aTitle[] = {_T("ÑÕÉ«"),_T("ºÉÔØ")};
		aTitle[1] += _T("(") + m_pDoc->m_pUnitCtrl->m_strStress + _T(")");
		int nColWidth[] = {40,150};
		CString title;
		int i;
		LV_COLUMN lvcolumn;

		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
		dwStyle |= LVS_EX_FULLROWSELECT; 
		ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
		// Set Title
		for(i = 0; i < 2; i++)
		{
			title = aTitle[i];
			lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_LEFT;

			lvcolumn.iSubItem = i;
			lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
			lvcolumn.pszText = title.GetBuffer(0);
			m_List.InsertColumn(i,&lvcolumn);
			title.ReleaseBuffer();
		}
	}
}

void CStldPresQueryDlg::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch (pNMCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		}
	case CDDS_ITEMPREPAINT:
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			break;
		}
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		{
			int nItem = pNMCD->nmcd.dwItemSpec;
			int nSubItem = pNMCD->iSubItem;
			if(nSubItem == 0)
			{
				
				if(!m_mapColor.Lookup(nItem,pNMCD->clrTextBk))
				{
					ASSERT(FALSE);
					pNMCD->clrTextBk = RGB(255, 255, 255);
				}
			}
			else
				pNMCD->clrTextBk = RGB(255, 255, 255);
			*pResult = CDRF_DODEFAULT;
		}

	default:
		break;
	}



}

void CStldPresQueryDlg::OnTmClose() 
{
	//if(CDBDoc::GetDocPoint())
	//	CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
	CloseDlg();
}

void CStldPresQueryDlg::OnTmExecute() 
{
	UpdateData(TRUE);
	bool bShowText = static_cast<CButton*>(GetDlgItem(IDC_TM_LOAD_VALUE_CHECK))->GetCheck();
	ILabelMgr_CH::instance()->m_bDrawPresTypeText = bShowText;
	if(m_nInputType == 0)
		SetLabelMgrLPData();
	else
		SetLabelMgrLCData();

	ILabelMgr_CH::instance()->m_bDrawPresTypeColor = true;
	ILabelMgr_CH::instance()->SetDrawPresQueryColorStatus();
}

void CStldPresQueryDlg::OnShowNumDetDlg()
{
	CTreeNumDetTargetDlg dlg(&m_NumDetParam);
	if (dlg.DoModal() == IDOK)
	{
		UpdateListCtrl();
		OnTmExecute();
	}
}

void CStldPresQueryDlg::SetLabelMgrLPData()
{
	CMap<T_ELEM_K, T_ELEM_K, T_PSLT_K, T_PSLT_K>& mapElemPslt = ILabelMgr_CH::instance()->m_DrawPresType_MapPslt;
	CMap<T_PSLT_K,T_PSLT_K,COLORREF,COLORREF>& mapElemColor = ILabelMgr_CH::instance()->m_DrawPresType_MapColor;
	CMap<T_PSLT_K,T_PSLT_K,CString,CString&>& mapElemText = ILabelMgr_CH::instance()->m_DrawPresType_MapText;
	mapElemPslt.RemoveAll();
	mapElemColor.RemoveAll();
	mapElemText.RemoveAll();
	bool bShowText = ILabelMgr_CH::instance()->m_bDrawPresTypeText;
	T_PSLT_K OriPsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
	T_PSLT_D PsltD;
	T_ELEM_K_LIST aElemList;
	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemList);
	for(int i = aElemList.GetSize()-1;i >= 0;i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aElemList[i],ElemD);
		if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
			aElemList.RemoveAt(i);
	}
	
	T_LDGR_K GroupKey = 0;
	m_CobxLdgr.GetSelectedLdgr(GroupKey);
	CArray<T_PRES_K, T_PRES_K> arPresKey;
	CArray<T_PRES_D, T_PRES_D&> arPresData;
	T_PSLT_K UpdatePsltK = OriPsltK;
	for(int i = 0;i < aElemList.GetSize();i++)
	{
		//m_pDoc->m_pAttrCtrl->GetElem(aElemList[i],ElemD);
		m_pDoc->m_pAttrCtrl->GetPres(aElemList[i], arPresKey,arPresData);
		for(int j = 0;j < arPresData.GetSize();j++)
		{
			if(arPresData[j].nSubLoadType != 0 || arPresData[j].GroupKey != GroupKey)
				continue;
			if(OriPsltK == 0)
				UpdatePsltK = arPresData[j].PSltKey;
			if(arPresData[j].PSltKey != 0 && UpdatePsltK == arPresData[j].PSltKey)
			{
				ASSERT(!mapElemPslt.PLookup(aElemList[i]));
				m_pDoc->m_pAttrCtrl2->GetPslt(UpdatePsltK,PsltD);
				mapElemPslt[aElemList[i]] = UpdatePsltK;
				if(!mapElemColor.PLookup(UpdatePsltK))
				{
					mapElemColor[UpdatePsltK] = PsltD.color;
					if(bShowText)
					{
						mapElemText.SetAt(UpdatePsltK,DataToStr_LP(2,UpdatePsltK,PsltD));
					}
				}
				break;
			}
		}
	}
}

void CStldPresQueryDlg::SetLabelMgrLCData()
{
	CMap<T_ELEM_K, T_ELEM_K, T_PSLT_K, T_PSLT_K>& mapElemPslt = ILabelMgr_CH::instance()->m_DrawPresType_MapPslt;
	CMap<T_PSLT_K,T_PSLT_K,COLORREF,COLORREF>& mapElemColor = ILabelMgr_CH::instance()->m_DrawPresType_MapColor;
	CMap<T_PSLT_K,T_PSLT_K,CString,CString&>& mapElemText = ILabelMgr_CH::instance()->m_DrawPresType_MapText;
	mapElemPslt.RemoveAll();
	mapElemColor.RemoveAll();
	mapElemText.RemoveAll();
	//
	bool bShowText = ILabelMgr_CH::instance()->m_bDrawPresTypeText;
	T_STLD_K stldK = m_CobxLC.GetItemData(m_CobxLC.GetCurSel());
	D_STLDCOLOR_ARRAY* pStldColor;
	if(!m_mapStlColor.Lookup(stldK,pStldColor))
	{
		return;
	}
	T_ELEM_K_LIST aElemList;
	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemList);
	for(int i = aElemList.GetSize()-1;i >= 0;i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aElemList[i],ElemD);
		if(!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
			aElemList.RemoveAt(i);
	}
	//
	auto QuickSetPsltColor = [&](T_PSLT_K PsltK)
	{
		if(mapElemColor.PLookup(PsltK))
			return;
		for(int i = 0;i < pStldColor->GetSize();i++)
		{
			if(pStldColor->GetAt(i).hasPsltK(PsltK))
			{
				mapElemColor.SetAt(PsltK,pStldColor->GetAt(i).color);
				if(bShowText)
				{
					CString strValue;
					CString strFormat;
					if(m_NumDetParam.num_ExponentFlag)
						strFormat.Format(_T("%%.%de"), m_NumDetParam.num_DecimalPoint);
					else
						strFormat.Format(_T("%%.%df"), m_NumDetParam.num_DecimalPoint);
					strValue.Format(strFormat,pStldColor->GetAt(i).value);
					mapElemText.SetAt(PsltK,strValue);
				}
			}
		}
	};
	//
	unsigned int GroupKey = 0;
	m_CobxLdgr.GetSelectedLdgr(GroupKey);
	CArray<T_PRES_K, T_PRES_K> arPresKey;
	CArray<T_PRES_D, T_PRES_D&> arPresData;
	for(int i = 0;i < aElemList.GetSize();i++)
	{
		//m_pDoc->m_pAttrCtrl->GetElem(aElemList[i],ElemD);
		m_pDoc->m_pAttrCtrl->GetPres(aElemList[i], arPresKey,arPresData);
		for(int j = 0;j < arPresData.GetSize();j++)
		{
			if(stldK != arPresData[j].LoadCaseKey || GroupKey != arPresData[j].GroupKey)
				continue;
			if(arPresData[j].PSltKey != 0)
			{
				ASSERT(!mapElemPslt.PLookup(aElemList[i]));
				mapElemPslt[aElemList[i]] = arPresData[j].PSltKey;
				QuickSetPsltColor(arPresData[j].PSltKey);
				break;
			}
		}
	}
}
	