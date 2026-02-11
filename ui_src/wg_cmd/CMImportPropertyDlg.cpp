// CMImportPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMImportPropertyDlg.h"

//#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_Product.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ReadMGB.h"
#include "..\wg_db\wg_db_QSort.h"
#include <climits>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMImportPropertyDlg dialog

CCMImportPropertyDlg::CCMImportPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMImportPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMImportPropertyDlg)
	m_rdoImportType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_arNewID.Add(IDC_CMD_IMPROP_NUM_NEW_STATIC);
	m_arNewID.Add(IDC_CMD_IMPROP_NUM_NEW_EDIT);
}

void CCMImportPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMImportPropertyDlg)
	DDX_Control(pDX, IDC_CMD_IMPROP_TITLE1, m_strTitle);
	DDX_Control(pDX, IDC_CMD_IMPROP_NUM_NEW_EDIT, m_edtStartID);
	DDX_Control(pDX, IDC_CMD_IMPROP_SELPROPERTY_LIST, m_lstSelProperty);
	DDX_Control(pDX, IDC_CMD_IMPROP_PROPERTY_LIST, m_lstProperty);
	DDX_Radio(pDX, IDC_CMD_IMPROP_NUM_KEEP, m_rdoImportType);
	//}}AFX_DATA_MAP
}

void CCMImportPropertyDlg::InitPropertyData(CString strFileName, int nType, BOOL bIsMgbFile)
{
	m_pFile = new CFile();

	m_pFile->Open(strFileName, CFile::modeRead);
	m_nPropertyType = nType;

	int nITPF = 1;
	float FileVersion=1.0, RetFileVersion;
	char HeadName[4];
	memcpy(HeadName,D_MODEL_FILE_HEAD,4);

	if (bIsMgbFile) memcpy(HeadName, "MGEN", 4);

	BOOL bException = FALSE;
TRY
{
	m_BnFile.INIT_F(m_pFile, HeadName, nITPF, FileVersion);

	int nError = m_BnFile.CHKHED_F(RetFileVersion);
	if(nError != BN_ER_GOOD && nError != BN_ER_VRLO)
	{
		m_pDoc->m_pPostCtrl->DisplayErrorMessage(_LS(IDS_DB_MGB_ER_READ), nError);
		goto BAD;
	}
}
CATCH(CException, e)
{
	bException=TRUE;
	e->ReportError();
}
END_CATCH

	if(bException)	goto BAD;
BAD:
	return;
}

BOOL CCMImportPropertyDlg::RB_UNIT()
{
	int nBase_Heat =0;
	int nBase_Temper =0;
	if (!CReadMGB::RB_UNIT4Import(m_BnFile, nBase_Heat, nBase_Temper)) return FALSE;

	T_UNIT_INDEX  UnitIndexNew;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexNew);

	// 온도 단위만 다시 세팅
	UnitIndexNew.nBase_Heat   = nBase_Heat;
	UnitIndexNew.nBase_Temper = nBase_Temper;

	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrentNew(UnitIndexNew);

	return TRUE;
}
BOOL CCMImportPropertyDlg::RB_MATL()
{
	m_arMaterial.RemoveAll();

	T_MATERIAL Material;
	CArray<T_MATL_K, T_MATL_K> aKey;
	CArray<T_MATL_D, T_MATL_D&> aData;
	if (!CReadMGB::RB_MATL(m_BnFile, aKey, aData)) return FALSE;

	// Sorting (MATD와 순서 맞춘다.)
	CArray<int, int> aIndex;
	for (int n = 0; n < aKey.GetSize(); n++)
		aIndex.Add(n);
	CQSort::QSortIntWithIndex(aIndex.GetData(), (int*)aKey.GetData(), aKey.GetSize());

	int ix;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		ix = aIndex[i];
		m_pDoc->m_pUnitCtrl->ConvertUnitMatlOut(aData[ix]);
		Material.Key = aKey[i]; // ix 아님
		Material.Data = aData[ix];
		m_arMaterial.Add(Material);
	}

	return TRUE;
}

BOOL CCMImportPropertyDlg::RB_MATD()
{
	m_arMatlDesign.RemoveAll();

	T_MATLDESIGN MatlDesign;
	CArray<T_MATL_K, T_MATL_K> aKey;
	CArray<T_MATD_D, T_MATD_D&> aData;
	if (!CReadMGB::RB_MATD(m_BnFile, aKey, aData)) return FALSE;

	// Sorting (MATL와 순서 맞춘다.)
	CArray<int, int> aIndex;
	for (int n = 0; n < aKey.GetSize(); n++)
		aIndex.Add(n);
	CQSort::QSortIntWithIndex(aIndex.GetData(), (int*)aKey.GetData(), aKey.GetSize());

	int ix;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		ix = aIndex[i];
		m_pDoc->m_pUnitCtrl->ConvertUnitMatdOut(aData[ix]);
		MatlDesign.Key = aKey[i]; // ix 아님
		MatlDesign.Data = aData[ix];
		m_arMatlDesign.Add(MatlDesign);
	}

	return TRUE;
}

BOOL CCMImportPropertyDlg::RB_SECT()
{
	m_arSection.RemoveAll();

	T_SECTION SectData;
	CArray<T_SECT_K, T_SECT_K> aKey;
	CArray<T_SECT_D, T_SECT_D&> aData;
	if (!CReadMGB::RB_SECT(m_BnFile, aKey, aData)) return FALSE;

	// Sorting (SECD와 순서 맞춘다.)
	CArray<int, int> aIndex;
	for (int n = 0; n < aKey.GetSize(); n++)
		aIndex.Add(n);
	CQSort::QSortIntWithIndex(aIndex.GetData(), (int*)aKey.GetData(), aKey.GetSize());

	int ix;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		ix = aIndex[i];
		m_pDoc->m_pUnitCtrl->ConvertUnitSectOut(aData[ix]);
		SectData.Key = aKey[i]; // ix 아님
		SectData.Data = aData[ix];
		m_arSection.Add(SectData);
	}

	return TRUE;
}

BOOL CCMImportPropertyDlg::RB_SECD()
{
	m_arSectDesign.RemoveAll();

	T_SECTION SectData;
	CArray<T_SECT_K, T_SECT_K> aKey;
	CArray<T_SECT_D, T_SECT_D&> aData;
	if (!CReadMGB::RB_SECD(m_BnFile, aKey, aData)) return FALSE;

	// Sorting (SECT와 순서 맞춘다.)
	CArray<int, int> aIndex;
	for (int n = 0; n < aKey.GetSize(); n++)
		aIndex.Add(n);
	CQSort::QSortIntWithIndex(aIndex.GetData(), (int*)aKey.GetData(), aKey.GetSize());

	int ix;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		ix = aIndex[i];
		m_pDoc->m_pUnitCtrl->ConvertUnitSectOut(aData[ix]);
		SectData.Key = aKey[i]; // ix 아님
		SectData.Data = aData[ix];
		m_arSectDesign.Add(SectData);
	}

	return TRUE;
}

BOOL CCMImportPropertyDlg::RB_THIK() 
{
	m_arThickness.RemoveAll();	

	T_THIK_UDRD_D ThikData;
	CArray<T_THIK_K, T_THIK_K> aKey;
	CArray<T_THIK_D, T_THIK_D&> aData;
	if (!CReadMGB::RB_THIK(m_BnFile, aKey, aData)) return FALSE;
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		m_pDoc->m_pUnitCtrl->ConvertUnitThikOut(aData[i]);
		ThikData.key = aKey[i];
		ThikData.data = aData[i];
		m_arThickness.Add(ThikData);
	}

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMImportPropertyDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMImportPropertyDlg)
	ON_BN_CLICKED(IDC_CMD_IMPROP_SELECT_BTN, OnCmdImpropSelectBtn)
	ON_BN_CLICKED(IDC_CMD_IMPROP_REMOVE_BTN, OnCmdImpropRemoveBtn)
	ON_BN_CLICKED(IDC_CMD_IMPROP_SELECT_ALL, OnCmdImpropSelectAll)
	ON_BN_CLICKED(IDC_CMD_IMPROP_REMOVE_ALL, OnCmdImpropRemoveAll)
	/*ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_IMPROP_NUM_NEW_SPIN, OnDeltaposSpin)*/
	ON_BN_CLICKED(IDC_CMD_IMPROP_CANCEL, OnCmdImpropCancel)
	ON_BN_CLICKED(IDC_CMD_IMPROP_OK, OnCmdImpropOk)
	ON_LBN_DBLCLK(IDC_CMD_IMPROP_PROPERTY_LIST, OnDblclkCmdImpropPropertyList)
	ON_LBN_DBLCLK(IDC_CMD_IMPROP_SELPROPERTY_LIST, OnDblclkCmdImpropSelpropertyList)
	ON_BN_CLICKED(IDC_CMD_IMPROP_NUM_KEEP, OnCmdImpropNumKeep)
	ON_BN_CLICKED(IDC_CMD_IMPROP_NUM_NEW, OnCmdImpropNumNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMImportPropertyDlg message handlers

void CCMImportPropertyDlg::OnCmdImpropSelectBtn() 
{
	if(m_lstProperty.GetSelCount() == 0)
		return;

	int* lIndex = new int[m_lstProperty.GetSelCount()];	
	m_lstProperty.GetSelItems(m_lstProperty.GetSelCount(), lIndex);

	CString strSelString;

	for(int i=0; i<m_lstProperty.GetSelCount(); i++)
	{
		m_lstProperty.GetText(lIndex[i], strSelString);
		m_lstSelProperty.AddString(strSelString);
	}

	for(int i=m_lstProperty.GetSelCount()-1; i>=0; i--)
		m_lstProperty.DeleteString(lIndex[i]);

	m_lstProperty.SetCurSel(m_lstProperty.GetCount()-1);

	delete[] lIndex;		
}

void CCMImportPropertyDlg::OnCmdImpropRemoveBtn() 
{
	if(m_lstSelProperty.GetSelCount() == 0)
		return;

	int* lIndex = new int[m_lstSelProperty.GetSelCount()];	
	m_lstSelProperty.GetSelItems(m_lstSelProperty.GetSelCount(), lIndex);

	for(int i=m_lstSelProperty.GetSelCount()-1; i >= 0; i--)
		m_lstSelProperty.DeleteString(lIndex[i]);

	delete[] lIndex;

	CString strSelTemp;
	CString strTemp;


	m_lstProperty.ResetContent();
	switch(m_nPropertyType)
	{
		case 0:   //Material
			InitMaterialData(FALSE);
			break;
		case 1:   //Section
			InitSectionData(FALSE);
			break;
		case 2:   //Thickness
			InitThicknessData(FALSE);
			break;
	}
	
	for(int i=0; i<m_lstSelProperty.GetCount(); i++)
	{
		m_lstSelProperty.GetText(i, strSelTemp);		
		for(int j=0; j<m_lstProperty.GetCount(); j++)
		{
			m_lstProperty.GetText(j, strTemp);
			if(strTemp == strSelTemp)
			{
				m_lstProperty.DeleteString(j);
				break;
			}
		}
	}

	if(m_lstProperty.GetCount() > 0)
		m_lstProperty.SetCurSel(m_lstProperty.GetCount()-1);	
}

void CCMImportPropertyDlg::OnDblclkCmdImpropPropertyList() 
{
	OnCmdImpropSelectBtn();	
}

void CCMImportPropertyDlg::OnDblclkCmdImpropSelpropertyList() 
{
	OnCmdImpropRemoveBtn(); 
}

void CCMImportPropertyDlg::OnCmdImpropSelectAll() 
{
	CString strProperty;
	for(int i=0; i < m_lstProperty.GetCount(); i++)
	{
		m_lstProperty.GetText(i, strProperty);
		m_lstSelProperty.AddString(strProperty);
	}

	m_lstProperty.ResetContent();
}

void CCMImportPropertyDlg::OnCmdImpropRemoveAll() 
{
	m_lstProperty.ResetContent();
	m_lstSelProperty.ResetContent();

	switch(m_nPropertyType)
	{
		case 0:   //Material
			InitMaterialData(FALSE);
			break;
		case 1:   //Section
			InitSectionData(FALSE);
			break;
		case 2:   //Thickness
			InitThicknessData(FALSE);
			break;
	}
}

BOOL CCMImportPropertyDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_lstProperty.ResetContent();
	m_lstSelProperty.ResetContent();
	m_edtStartID.SetRange(1, SHRT_MAX);
	m_edtStartID.SetValue(1);
	UpdateData(FALSE);

	switch(m_nPropertyType)
	{
		case 0:  // Material;
			this->SetWindowText(_LS(IDS_WG_CMD__ADDD__Import_Material_from_other_Projec));
			m_strTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Material_List));
			RB_UNIT(); // 재료정보중 온도정보를 위해
			RB_MATL();
			RB_MATD();
			SortData(); //Sort
			InitMaterialData(TRUE);
			break;
		case 1:  // Section;
			this->SetWindowText(_LS(IDS_WG_CMD__ADDD__Import_Section_from_other_Project));
			m_strTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Section_List));
			RB_SECT();
			RB_SECD();
			SortData(); //Sort
			InitSectionData(TRUE);
			break;
		case 2:  // Thickness;
			this->SetWindowText(_LS(IDS_WG_CMD__ADDD__Import_Thickness_from_other_Proje));
			m_strTitle.SetWindowText(_LS(IDS_WG_CMD__ADDD__Thickness_List));
			RB_THIK();
			SortData(); //Sort
			InitThicknessData(TRUE);
			break;
	}	

	CDlgUtil::CtrlEnableDisable(this, m_arNewID, FALSE);
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMImportPropertyDlg::SortData()  //Sort
{
	int nSize;
	int nMinIndex=0;
	int nOldKey = 0;
	int nMinKey = 0;
	int i,j;

	T_SECTION SectData;
	T_SECTION SectDesignData;
	T_MATERIAL MatlData;
	T_MATLDESIGN MatlDesignData;
	T_THIK_UDRD_D TData;
	
	switch(m_nPropertyType)
	{
		case 0:  // Material
			nSize = m_arMaterial.GetSize();
			for(i=0; i < nSize-1; i++)
			{
				for(j=i; j < nSize; j++)
				{
					MatlData = m_arMaterial.GetAt(j);
					nOldKey = MatlData.Key;

					if(j == i)
					{
						nMinKey = nOldKey;
						nMinIndex = j;
						continue;
					}

					if(nOldKey < nMinKey)
					{
						nMinKey = nOldKey;
						nMinIndex = j;
					}
				}
				MatlData = m_arMaterial.GetAt(nMinIndex);
				m_arMaterial.RemoveAt(nMinIndex);
				m_arMaterial.InsertAt(i, MatlData);

				//Design
				MatlDesignData = m_arMatlDesign.GetAt(nMinIndex);
				m_arMatlDesign.RemoveAt(nMinIndex);
				m_arMatlDesign.InsertAt(i, MatlDesignData);
			}
			break;
		case 1:  // Section
			nSize = m_arSection.GetSize();
			for(i=0; i < nSize-1; i++)
			{
				for(j=i; j < nSize; j++)
				{
					SectData = m_arSection.GetAt(j);
					nOldKey = SectData.Key;

					if(j == i)
					{
						nMinKey = nOldKey;
						nMinIndex = j;
						continue;
					}

					if(nOldKey < nMinKey)
					{
						nMinKey = nOldKey;
						nMinIndex = j;
					}
				}
				SectData = m_arSection.GetAt(nMinIndex);
				m_arSection.RemoveAt(nMinIndex);
				m_arSection.InsertAt(i, SectData);

				//Design
				SectDesignData = m_arSectDesign.GetAt(nMinIndex);
				m_arSectDesign.RemoveAt(nMinIndex);
				m_arSectDesign.InsertAt(i, SectDesignData);
			}
			break;
		case 2:  // Thickness
			nSize = m_arThickness.GetSize();
			for(i=0; i < nSize-1; i++)
			{
				for(j=i; j < nSize; j++)
				{
					TData = m_arThickness.GetAt(j);
					nOldKey = TData.key;
							
					if(j == i)
					{
						nMinKey = nOldKey;
						nMinIndex = j;
						continue;
					}

					if(nOldKey < nMinKey)
					{
						nMinKey = nOldKey;
						nMinIndex = j;
					}
				}
				TData = m_arThickness.GetAt(nMinIndex);
				m_arThickness.RemoveAt(nMinIndex);
				m_arThickness.InsertAt(i, TData);
			}
			break;
	}
}

void CCMImportPropertyDlg::InitMaterialData(BOOL bSelectUnSelect)
{
	CString strTemp1;
	CString strTemp2;
	T_MATL_D Data;
	T_MATERIAL MatlData;
	
	if(m_arMaterial.GetSize())
	{
		for(int i=0; i<m_arMaterial.GetSize(); i++)
		{
			Data.Initialize();
			MatlData = m_arMaterial.GetAt(i);
			strTemp1.Format(_T("%d: "),MatlData.Key);
			strTemp2 = MatlData.Data.Name;

			strTemp1 = strTemp1 + strTemp2;
			
			if(bSelectUnSelect)
				m_lstSelProperty.AddString(strTemp1);
			else
				m_lstProperty.AddString(strTemp1);
		}
	}
}

void CCMImportPropertyDlg::InitSectionData(BOOL bSelectUnSelect)
{
	CString strTemp1;
	CString strTemp2;
	T_SECT_K Key;
	T_SECT_D Data;
	T_SECTION SectData;

	if(m_arSection.GetSize())
	{
		for(int i=0; i<m_arSection.GetSize(); i++)
		{
			Data.Initialize();
			SectData = m_arSection.GetAt(i);

			Key = SectData.Key;
			Data = SectData.Data;

			strTemp1.Format(_T("%d: "),Key);
			strTemp2 = Data.SName;

			strTemp1 = strTemp1 + strTemp2;

			if(bSelectUnSelect)
				m_lstSelProperty.AddString(strTemp1);
			else
				m_lstProperty.AddString(strTemp1);
		}
	}
}

void CCMImportPropertyDlg::InitThicknessData(BOOL bSelectUnSelect)
{
	CString strTemp1;
	CString strTemp2;
	T_THIK_UDRD_D Data;

	if(m_arThickness.GetSize())
	{
		for(int i=0; i<m_arThickness.GetSize(); i++)
		{
			Data.data.Initialize();
			Data = m_arThickness.GetAt(i);
			strTemp1.Format(_T("%d: "),Data.key);

			double dThickness;
			m_pDoc->m_pAttrCtrl->GetThikForThikName(Data.data, dThickness);
			strTemp2.Format(_T("%.4f"), dThickness);
			//if(Data.data.nThickType)
			//	strTemp2.Format(_T("%.4f(%.4f)"), Data.data.ThickIn,Data.data.ThickOut);
			//else
			//	strTemp2.Format(_T("%.4f"), Data.data.ThickIn);

			strTemp1 = strTemp1 + strTemp2;

			if(bSelectUnSelect)
				m_lstSelProperty.AddString(strTemp1);
			else
				m_lstProperty.AddString(strTemp1);
		}
	}
}

BOOL CCMImportPropertyDlg::DestroyWindow() 
{
	if(m_pFile)
		delete m_pFile;
	
	return CDialogMove::DestroyWindow();
}

void CCMImportPropertyDlg::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString strTemp;
	//int nTemp;
	m_edtStartID.GetWindowText(strTemp);
	
	//nTemp = _ttoi(strTemp);

	//if(pNMUpDown->iDelta < 0)
	//{
	//	strTemp.Format(_T("%d"), nTemp+1);
	//	m_edtStartID.SetWindowText(strTemp);
	//}	
	//else
	//{
	//	if(nTemp < 2)
	//		m_edtStartID.SetWindowText(_T("1"));
	//	else
	//	{
	//		strTemp.Format(_T("%d"), nTemp-1);
	//		m_edtStartID.SetWindowText(strTemp);
	//	}
	//}   

	*pResult = 0;
}

void CCMImportPropertyDlg::OnCmdImpropCancel() 
{
	CDialogMove::OnCancel();
}

void CCMImportPropertyDlg::OnCmdImpropOk() 
{
	if(ImportPropertyData())	
		CDialogMove::OnOK();
}

BOOL CCMImportPropertyDlg::ImportPropertyData()
{
	BOOL bCheck=TRUE;
	UpdateData(TRUE);

	switch(m_rdoImportType)
	{
		case 0:
			if(m_nPropertyType == 0) bCheck=ImportMaterialData(TRUE); 
			else if(m_nPropertyType == 1) bCheck=ImportSectionData(TRUE);
			else if(m_nPropertyType == 2) bCheck=ImportThicknessData(TRUE);
			break;
		case 1:
			if(m_nPropertyType == 0) bCheck=ImportMaterialData(FALSE); 
			else if(m_nPropertyType == 1) bCheck=ImportSectionData(FALSE);
			else if(m_nPropertyType == 2) bCheck=ImportThicknessData(FALSE);
			break;
	}

	return bCheck;
}

BOOL CCMImportPropertyDlg::ImportMaterialData(BOOL bNumType)
{
	T_MATL_K Key; 
	T_MATL_D Data;
	CArray<T_MATL_K, T_MATL_K> arAddKey;
	CArray<T_MATL_D, T_MATL_D&> arAddData;
	CArray<T_MATL_K, T_MATL_K> arDesignKey;
	CArray<T_MATD_D, T_MATD_D&> arDesignData;
	T_MATERIAL MatlData;

	int nKeyIndex;

	if(bNumType) //Replace	// bReplace = TRUE;
	{
		for(int i=0; i<m_lstSelProperty.GetCount(); i++)
		{
			nKeyIndex = GetIndexKey(i);

			MatlData.Data.Initialize();
			MatlData = m_arMaterial.GetAt(nKeyIndex);

			Key = MatlData.Key;
			Data = MatlData.Data;

			arAddKey.Add(Key);
			arAddData.Add(Data);
			// Design
			arDesignKey.Add(m_arMatlDesign.GetAt(nKeyIndex).Key);
			arDesignData.Add(m_arMatlDesign.GetAt(nKeyIndex).Data);
		}
		if(!m_pDoc->m_pDataCtrl->AddMatl(arAddKey, arAddData, TRUE))
			return FALSE;
		if(!m_pDoc->m_pDataCtrl->AddMatd(arDesignKey, arDesignData))
			return FALSE;
	}
	else  // define Start Number  // bReplace = FALSE;
	{
		CString strTemp;
		int nStartID;

		m_edtStartID.GetWindowText(strTemp);
		nStartID = _ttoi(strTemp);

		for(int i=0; i<m_lstSelProperty.GetCount(); i++)
		{
			nKeyIndex = GetIndexKey(i);

			MatlData.Data.Initialize();
			MatlData = m_arMaterial.GetAt(nKeyIndex);

			while(m_pDoc->m_pAttrCtrl->ExistMatl(nStartID))
				nStartID++;

			Key = nStartID;
			Data = MatlData.Data;

			arAddKey.Add(Key);
			arAddData.Add(Data);
			// Design
			arDesignKey.Add(Key);
			arDesignData.Add(m_arMatlDesign.GetAt(nKeyIndex).Data);
			nStartID++;
		}
		if(!m_pDoc->m_pDataCtrl->AddMatl(arAddKey, arAddData))
			return FALSE;
		if(!m_pDoc->m_pDataCtrl->AddMatd(arDesignKey, arDesignData))
			return FALSE;
	}

	return TRUE;
}

BOOL CCMImportPropertyDlg::ImportSectionData(BOOL bNumType)
{
	T_SECT_K Key; 
	T_SECT_D Data;
	CArray<T_SECT_K, T_SECT_K> arAddKey;
	CArray<T_SECT_D, T_SECT_D&> arAddData;
	CArray<T_SECT_K, T_SECT_K> arDesignKey;
	CArray<T_SECT_D, T_SECT_D&> arDesignData;
	T_SECTION SectData;

	int nKeyIndex;

	if(bNumType) //Replace
	{
		for(int i=0; i<m_lstSelProperty.GetCount(); i++)
		{
			nKeyIndex = GetIndexKey(i);

			SectData.Data.Initialize();
			SectData = m_arSection.GetAt(nKeyIndex);

			Key = SectData.Key;
			Data = SectData.Data;

			arAddKey.Add(Key);
			arAddData.Add(Data);
			// Design
			arDesignKey.Add(m_arSectDesign.GetAt(nKeyIndex).Key);
			arDesignData.Add(m_arSectDesign.GetAt(nKeyIndex).Data);
		}
		if(!m_pDoc->m_pDataCtrl->AddSect(arAddKey, arAddData,TRUE))
			return FALSE;
		if(!m_pDoc->m_pDataCtrl->AddSecd(arDesignKey, arDesignData))
			return FALSE;
	}
	else  // define Start Number
	{
		CString strTemp;
		int nStartID;

		m_edtStartID.GetWindowText(strTemp);
		nStartID = _ttoi(strTemp);

		for(int i=0; i<m_lstSelProperty.GetCount(); i++)
		{
			nKeyIndex = GetIndexKey(i);

			SectData.Data.Initialize();
			SectData = m_arSection.GetAt(nKeyIndex);

			while(m_pDoc->m_pAttrCtrl->ExistSect(nStartID))
				nStartID++;

			Key = nStartID;
			Data = SectData.Data;

			arAddKey.Add(Key);
			arAddData.Add(Data);
			// Design
			arDesignKey.Add(Key);
			arDesignData.Add(m_arSectDesign.GetAt(nKeyIndex).Data);
			nStartID++;
		}
		if(!m_pDoc->m_pDataCtrl->AddSect(arAddKey, arAddData))
			return FALSE;
		if(!m_pDoc->m_pDataCtrl->AddSecd(arDesignKey, arDesignData))
			return FALSE;
	}
	return TRUE;
}

BOOL CCMImportPropertyDlg::ImportThicknessData(BOOL bNumType)
{
	T_THIK_UDRD_D ThikData; 

	T_THIK_K Key;
	T_THIK_D Data;
	CArray<T_THIK_K, T_THIK_K> arAddKey;
	CArray<T_THIK_D, T_THIK_D&> arAddData;
	int nKeyIndex;

	if(bNumType) //Replace
	{
		for(int i=0; i<m_lstSelProperty.GetCount(); i++)
		{
			nKeyIndex = GetIndexKey(i);

			ThikData.data.Initialize();
			ThikData = m_arThickness.GetAt(nKeyIndex);

			Key = ThikData.key;
			Data = ThikData.data;
			
			arAddKey.Add(Key);
			arAddData.Add(Data);
		}
		if(!m_pDoc->m_pDataCtrl->AddThik(arAddKey, arAddData, TRUE))
			return FALSE;
	}
	else  // define Start Number
	{
		CString strTemp;
		int nStartID;

		m_edtStartID.GetWindowText(strTemp);
		nStartID = _ttoi(strTemp);

		for(int i=0; i<m_lstSelProperty.GetCount(); i++)
		{
			nKeyIndex = GetIndexKey(i);

			ThikData.data.Initialize();
			ThikData = m_arThickness.GetAt(nKeyIndex);

			while(m_pDoc->m_pAttrCtrl->ExistThik(nStartID))
				nStartID++;
				
			Key = nStartID;
			Data = ThikData.data;

			arAddKey.Add(Key);
			arAddData.Add(Data);
			nStartID++;
		}
		if(!m_pDoc->m_pDataCtrl->AddThik(arAddKey, arAddData))
			return FALSE;
	}
	return TRUE;
}

int CCMImportPropertyDlg::GetIndexKey(int nIndex)
{
	T_MATERIAL MatlData;
	T_SECTION SectData;
	T_THIK_UDRD_D ThikData; 

	CString strKey;
	CString strCboText;
	int nKeyIndex;
	int i;

	m_lstSelProperty.GetText(nIndex, strCboText);
	nKeyIndex = strCboText.Find(':', 0);
	strKey = strCboText.Left(nKeyIndex);

	switch(m_nPropertyType)
	{
		case 0: //Material
			for(i=0; i<m_arMaterial.GetSize(); i++)
			{
				MatlData = m_arMaterial.GetAt(i);
				if(_ttoi(strKey) == MatlData.Key)
				{
					nKeyIndex = i;
					break;
				}
			}		
			break;
		case 1: //Section
			for(i=0; i<m_arSection.GetSize(); i++)
			{
				SectData = m_arSection.GetAt(i);
				if(_ttoi(strKey) == SectData.Key)
				{
					nKeyIndex = i;
					break;
				}
			}		
			break;
		case 2: //Thickness
			for(i=0; i<m_arThickness.GetSize(); i++)
			{
				ThikData = m_arThickness.GetAt(i);
				if(_ttoi(strKey) == ThikData.key)
				{
					nKeyIndex = i;
					break;
				}
			}		
			break;
	}

	return nKeyIndex;
}

void CCMImportPropertyDlg::OnCmdImpropNumKeep() 
{
	CDlgUtil::CtrlEnableDisable(this, m_arNewID, FALSE);
}

void CCMImportPropertyDlg::OnCmdImpropNumNew() 
{
	CDlgUtil::CtrlEnableDisable(this, m_arNewID, TRUE);	
	m_edtStartID.SetFocus();
}
