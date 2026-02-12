#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectSODDeckStiffenerDlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMSectSODDeckStiffenerDlgUtil::CCMSectSODDeckStiffenerDlgUtil()
{

}

CCMSectSODDeckStiffenerDlgUtil::~CCMSectSODDeckStiffenerDlgUtil()
{

}

void CCMSectSODDeckStiffenerDlgUtil::GetCbxDeckPartItem(T_SECT_D& SectD, int nDeckPos, std::vector<CString>* pArStrCbxDeckPartItem)
{
	if(!pArStrCbxDeckPartItem)
	{
		ASSERT(0);
		return;
	}
	GetSODStiffDlgData(SectD, nDeckPos, -1, -1, pArStrCbxDeckPartItem, NULL,  NULL, NULL, NULL);
}

void CCMSectSODDeckStiffenerDlgUtil::GetDeckName(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, CString* pDeckName)
{
	if(!pDeckName)
	{
		ASSERT(0);
		return;
	}
	GetSODStiffDlgData(SectD, nDeckPos, nDeckPart, nDeckPartCnt, NULL, pDeckName,  NULL, NULL, NULL);
}

void CCMSectSODDeckStiffenerDlgUtil::GetDefStiffName(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, CString* pDefStiffName)
{
	if(!pDefStiffName)
	{
		ASSERT(0);
		return;
	}
	GetSODStiffDlgData(SectD, nDeckPos, nDeckPart, nDeckPartCnt, NULL, NULL,  pDefStiffName, NULL, NULL);
}

void CCMSectSODDeckStiffenerDlgUtil::GetDefStiffPos(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int* pDefStiffPos)
{
	if(!pDefStiffPos)
	{
		ASSERT(0);
		return;
	}
	GetSODStiffDlgData(SectD, nDeckPos, nDeckPart, nDeckPartCnt, NULL, NULL,  NULL, pDefStiffPos, NULL);
}

void CCMSectSODDeckStiffenerDlgUtil::GetStiffPosItemList(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, CString* pStiffPosItemList)
{
	if(!pStiffPosItemList)
	{
		ASSERT(0);
		return;
	}
	GetSODStiffDlgData(SectD, nDeckPos, nDeckPart, nDeckPartCnt, NULL, NULL,  NULL, NULL, pStiffPosItemList);
}

void CCMSectSODDeckStiffenerDlgUtil::GetStiffDirItemList(T_SECT_D& SectD, int nDeckPos, CString* pStiffPosItemList)
{
	if(!pStiffPosItemList)
	{
		ASSERT(0);
		return;
	}
	GetSODStiffDirData(SectD, nDeckPos, pStiffPosItemList);
}

void CCMSectSODDeckStiffenerDlgUtil::GetSODStiffDirData(T_SECT_D& SectD, int nDeckPos, CString* pStiffPosItemList)
{
	if(pStiffPosItemList)
	{
		*pStiffPosItemList = _T("");
	}

	if(SectD.nStype == D_SECT_TYPE_STLG_B || SectD.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{
		if(nDeckPos == 1 || nDeckPos == 2) // web
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_TOP);    *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
		}
		else
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);    *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
		}
	}
	else if(SectD.nStype == D_SECT_TYPE_STLG_I || SectD.nStype == D_SECT_TYPE_COMPO_STLG_I ||
				 (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
				 (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
	{
		if(nDeckPos == 1) // web
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_TOP);    *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
		}
		else
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);   *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT);  *pStiffPosItemList += _T("\n");
		}
	}
	else if(SectD.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
				 (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		if(nDeckPos == 1 || nDeckPos == 2) // web
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_TOP);    *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
		}
		else
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);   *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT);  *pStiffPosItemList += _T("\n");
		}
	}
	else if(SectD.nStype == D_SECT_TYPE_STLG_MCELL ||
				 (SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{    
		if(nDeckPos == 1) // web
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_TOP);    *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
		}
		else
		{
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);   *pStiffPosItemList += _T("\n");
			*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT);  *pStiffPosItemList += _T("\n");
		}
	}

	//*pStiffPosItemList += _T("-");

}





void CCMSectSODDeckStiffenerDlgUtil::GetSODStiffDlgData(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, std::vector<CString>* pArStrCbxDeckPartItem, CString* pDeckName, CString* pDefStiffName, int* pDefStiffPos, CString* pStiffPosItemList)
{
	if(pArStrCbxDeckPartItem)
	{
		pArStrCbxDeckPartItem->clear();
	}
	if(pDeckName)
	{
		*pDeckName = _T("");
	}
	if(pDefStiffName)
	{
		*pDefStiffName = _T("");
	}
	if(pDefStiffPos)
	{
		*pDefStiffPos = -1;
	}
	if(pStiffPosItemList)
	{
		*pStiffPosItemList = _T("");
	}

	if(SectD.nStype == D_SECT_TYPE_STLG_B || 
		SectD.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{
		if(nDeckPos == 0)
		{      
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK3));
			}
			if(pStiffPosItemList)
			{
				*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
			}      
			if(pDefStiffPos)
			{
				*pDefStiffPos = 1;
			}

			if(nDeckPart == 0)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_LEFT);

				if(pDefStiffName) *pDefStiffName = _T("TL");  // Stiffener 이름 자동 부여 (MQC Alpha/Beta : 4870-18)
			}
			else if(nDeckPart == 1)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_CENTER);
				if(pDefStiffName) *pDefStiffName = _T("TC"); 
			}
			else if(nDeckPart == 2)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("TR"); 
			}
		}
		else if(nDeckPos == 1)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB));
			}      

			if(pStiffPosItemList)
			{
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 2;
			}

			if(nDeckPart == 0)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_WEB_LEFT);
				if(pDefStiffName) *pDefStiffName = _T("LW"); 
			}
		}
		else if(nDeckPos == 2)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 2;
			}

			if(nDeckPart == 0) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_WEB_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("RW"); 
			}
		}
		else if(nDeckPos == 3)
		{      
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK3));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_TOP); *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 0;
			}

			if(nDeckPart == 0)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_LEFT);
				if(pDefStiffName) *pDefStiffName = _T("BL"); 
			}
			else if(nDeckPart == 1) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_CENTER);
				if(pDefStiffName) *pDefStiffName = _T("BC"); 
			}
			else if(nDeckPart == 2) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("BR"); 
			}
		}
	}
	else if(SectD.nStype == D_SECT_TYPE_STLG_I || SectD.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
	{
		if(nDeckPos == 0)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
			}
			if(pStiffPosItemList)
			{
				*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 1;
			}

			if(nDeckPart == 0)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_LEFT);
				if(pDefStiffName) *pDefStiffName = _T("TL"); 
			}
			else if(nDeckPart == 1) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("TR"); 
			}
		}
		else if(nDeckPos == 1)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB));
			}   
			if(pStiffPosItemList)
			{
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 2;
			}

			if(nDeckPart == 0)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_WEB);
				if(pDefStiffName) *pDefStiffName = _T("W"); 
			}
		}
		else if(nDeckPos == 2)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_TOP); *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 1;
			}

			if(nDeckPart == 0)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_LEFT);
				if(pDefStiffName) *pDefStiffName = _T("BL"); 
			}
			else if(nDeckPart == 1) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("BR"); 
			}
		}
	}
	else if(SectD.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		if(nDeckPos == 0)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK3));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 1;
			}

			if(nDeckPart == 0)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_LEFT);
				if(pDefStiffName) *pDefStiffName = _T("TL"); 
			}
			else if(nDeckPart == 1) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_CENTER);
				if(pDefStiffName) *pDefStiffName = _T("TC"); 
			}
			else if(nDeckPart == 2)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_TOP_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("TR"); 
			}
		}
		else if(nDeckPos == 1)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 2;
			}

			if  (nDeckPart == 0) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_WEB_LEFT);
				if(pDefStiffName) *pDefStiffName = _T("LW"); 
			}
		}
		else if(nDeckPos == 2)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 2;
			}

			if  (nDeckPart == 0) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_WEB_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("RW"); 
			}
		}
		else if(nDeckPos == 3)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK3));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_TOP); *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 0;
			}

			if(nDeckPart == 0) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_LEFT);
				if(pDefStiffName) *pDefStiffName = _T("BL"); 
			}
			else if(nDeckPart == 1) 
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_CENTER);
				if(pDefStiffName) *pDefStiffName = _T("BC"); 
			}
			else if(nDeckPart == 2)
			{
				if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_BOT_RIGHT);
				if(pDefStiffName) *pDefStiffName = _T("BR"); 
			}
		}
	}
	else if(SectD.nStype == D_SECT_TYPE_STLG_MCELL ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{    
		GetSODStiffDlgDataMCell(SectD, nDeckPos, nDeckPart, nDeckPartCnt, pArStrCbxDeckPartItem, pDeckName, pDefStiffName, pDefStiffPos, pStiffPosItemList);
	}
}

void CCMSectSODDeckStiffenerDlgUtil::GetSODStiffDlgDataMCell(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, std::vector<CString>* pArStrCbxDeckPartItem, CString* pDeckName, CString* pDefStiffName, int* pDefStiffPos, CString* pStiffPosItemList)
{
	CString strTemp;
	switch (SectD.SectBefore.nFlangeShape)
	{  
	case 0: // General
	case 1: //Rebar-1
		{
			GetSODStiffDlgDataMCellGen(SectD, nDeckPos, nDeckPart, nDeckPartCnt, pArStrCbxDeckPartItem, pDeckName, pDefStiffName, pDefStiffPos, pStiffPosItemList);
		}
		break;
	case 2: //Rebar-2
		{
			if(0 <= nDeckPos && nDeckPos <= 2)
			{
				GetSODStiffDlgDataMCellGen(SectD, nDeckPos, nDeckPart, nDeckPartCnt, pArStrCbxDeckPartItem, pDeckName, pDefStiffName, pDefStiffPos, pStiffPosItemList);
			}
			else if(nDeckPos == 3) // Side Flange
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_TOP); *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 0;
				}

				if(SectD.SectBefore.nCellNum == 0)
				{
					if(pArStrCbxDeckPartItem)
					{
						pArStrCbxDeckPartItem->clear();
						pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_SIDE));
					}  

					if(nDeckPart == 0)
					{
						if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_SIDE);
						if(pDefStiffName) *pDefStiffName = _T("S");
					}         
				}
				else
				{
					if(pArStrCbxDeckPartItem)
					{
						pArStrCbxDeckPartItem->clear();
						pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_SIDE1));
						pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_SIDE2));
					}      

					if(nDeckPart == 0)  
					{
						if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_SIDE1);
						if(pDefStiffName) *pDefStiffName = _T("S1");
					}
					else if(nDeckPart == 1) 
					{
						if(pDeckName) *pDeckName = _LS(IDS_CMD_SECT_SOD_SIDE2);
						if(pDefStiffName) *pDefStiffName = _T("S2");
					}
				}
			}
		}
		break;
	case 3: // Closed
		{
			BOOL bJ1, bJ2, bJr1, bJr2;
			bJ1 = bJ2 = bJr1 = bJr2 = FALSE;
			if(SectD.SectBefore.nJoint & 0x00000001) bJ1 = TRUE;
			if(SectD.SectBefore.nJoint & 0x00000002) bJ2 = TRUE;
			if(SectD.SectBefore.nJoint & 0x00000004) bJr1 = TRUE;
			if(SectD.SectBefore.nJoint & 0x00000008) bJr2 = TRUE;

			if(nDeckPos == 0) // Top Flange
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 1;
				}

				if(SectD.SectBefore.nCellNum == 0 || SectD.SectBefore.nCellNum == 1)
				{
					if(pArStrCbxDeckPartItem)
					{
						pArStrCbxDeckPartItem->clear();
						pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK));
					}      
				}
				else
				{
					if(pArStrCbxDeckPartItem)
					{
						pArStrCbxDeckPartItem->clear();
						for (int i = 0; i < SectD.SectBefore.nCellNum; i++)
						{
							strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_NUM), i + 1);
							pArStrCbxDeckPartItem->push_back(strTemp);
						}
					}    
				}

				if(nDeckPartCnt == 1)
				{
					if(pDeckName)
					{
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_TOP));
						*pDeckName = strTemp;
					}
					if(pDefStiffName)
					{
						strTemp.Format(_T("TD"));
						*pDefStiffName = strTemp;
					}
				}
				else
				{
					if(pDeckName)
					{
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_TOP_NUM), nDeckPart + 1);
						*pDeckName = strTemp;
					}
					if(pDefStiffName)
					{
						strTemp.Format(_T("TD%d"), nDeckPart + 1);
						*pDefStiffName = strTemp;
					}
				} 
			}
			else if(nDeckPos == 1) // Web
			{
				int nDeckPartLeft, nDeckPartRight;
				nDeckPartLeft = nDeckPartRight = -1;

				std::vector<CString> aPartName;

				if(SectD.SectBefore.nCellNum == 0)
				{
					if(SectD.SectBefore.nCellType == 0)
					{
							aPartName.clear();
							aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB11));
							aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB12));
							if(bJ2)  aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB13));

						if(bJ2)
						{
							if(nDeckPart == 0)
							{
								nDeckPartLeft = 0;
								nDeckPartRight = -1;
							}
							else if(nDeckPart == 1)
							{
								nDeckPartLeft = 1;
								nDeckPartRight = -1;
							}
							else if(nDeckPart == 2)
							{
								nDeckPartLeft = 2;               
								nDeckPartRight = -1;
							}
						}
						else
						{
							ASSERT(bJ1); // J1은 항상 On
							if(nDeckPart == 0)
							{
								nDeckPartLeft = 0;
								nDeckPartRight = -1;
							}
							else if(nDeckPart == 1)
							{
								nDeckPartLeft = 1;
								nDeckPartRight = -1;
							}
						}

						if(nDeckPartLeft == 0)    
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 1;
							}
						}
						else if(nDeckPartLeft == 1) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 1;
							}
						}
						else if(nDeckPartLeft == 2) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 1;
							}
						}
					}
					else if(SectD.SectBefore.nCellType == 1)
					{
						aPartName.clear();
						aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB11));
						aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB12));
						if(bJr2)  aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB13));

						if(bJr2)
						{
							if(nDeckPart == 0)
							{
								nDeckPartLeft = -1;
								nDeckPartRight = 0;
							}
							else if(nDeckPart == 1)
							{
								nDeckPartLeft = -1;
								nDeckPartRight = 1;
							}
							else if(nDeckPart == 2)
							{
								nDeckPartLeft = -1;
								nDeckPartRight = 2;
							}
						}
						else
						{
							ASSERT(bJr1); // Jr1은 항상 On              
							if(nDeckPart == 0)
							{
								nDeckPartLeft = -1;
								nDeckPartRight = 0;
							}
							else if(nDeckPart == 1)
							{
								nDeckPartLeft = -1;
								nDeckPartRight = 1;
							}
						}

						if(nDeckPartRight == 0) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 0;
							}
						}
						else if(nDeckPartRight == 1) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 0;
							}
						}
						else if(nDeckPartRight == 2) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 0;
							}
						}
					}
				}
				else if(SectD.SectBefore.nCellNum >= 1)
				{
					int nInternalWebNum = SectD.SectBefore.nCellNum - 1;

					int nLeftPartNum = 0;
					if(bJ2)
					{
						nLeftPartNum = 3;
					}
					else
					{
						nLeftPartNum = 2;
					}

					if(bJ2 && bJr2)
					{
						if(nDeckPart == 0)
						{
							nDeckPartLeft = 0;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 1)
						{
							nDeckPartLeft = 1;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 2)
						{
							nDeckPartLeft = 2;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 3 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 0;
						}
						else if(nDeckPart == 4 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 1;
						}
						else if(nDeckPart == 5 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 2;
						}
						else
						{
							nDeckPartLeft = -1;
							nDeckPartRight = -1;
						}
					}
					else if(!bJ2 && bJr2)
					{
						if(nDeckPart == 0)
						{
							nDeckPartLeft = 0;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 1)
						{
							nDeckPartLeft = 1;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 2 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 0;
						}
						else if(nDeckPart == 3 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 1;
						}
						else if(nDeckPart == 4 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 2;
						}
						else
						{
							nDeckPartLeft = -1;
							nDeckPartRight = -1;
						}
					}
					else if(bJ2 && !bJr2)
					{
						if(nDeckPart == 0)
						{
							nDeckPartLeft = 0;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 1)
						{
							nDeckPartLeft = 1;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 2)
						{
							nDeckPartLeft = 2;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 3 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 0;
						}
						else if(nDeckPart == 4 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 1;
						}
						else
						{
							nDeckPartLeft = -1;
							nDeckPartRight = -1;
						}
					}
					else if(!bJ2 && !bJr2)
					{
						if(nDeckPart == 0)
						{
							nDeckPartLeft = 0;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 1)
						{
							nDeckPartLeft = 1;
							nDeckPartRight = -1;
						}
						else if(nDeckPart == 2 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 0;
						}
						else if(nDeckPart == 3 + nInternalWebNum)
						{
							nDeckPartLeft = -1;
							nDeckPartRight = 1;
						}
						else
						{
							nDeckPartLeft = -1;
							nDeckPartRight = -1;
						}
					}

					if(SectD.SectBefore.nCellNum == 1)
					{
						aPartName.clear();
						aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB11));
						aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB12));
						if(bJ2)   aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB13));
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB1_NUM), nInternalWebNum + 2);
						aPartName.push_back(strTemp);
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB2_NUM), nInternalWebNum + 2);
						aPartName.push_back(strTemp);
						if(bJr2)  
						{
							strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB3_NUM), nInternalWebNum + 2);
							aPartName.push_back(strTemp);
						}

						if(nDeckPartLeft == 0)      
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 1;
							}
						}
						else if(nDeckPartLeft == 1) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 1;
							}
						}
						else if(nDeckPartLeft == 2) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 1;
							}
						}
						else if(nDeckPartRight == 0) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 0;
							}
						}
						else if(nDeckPartRight == 1) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 0;
							}
						}
						else if(nDeckPartRight == 2) 
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 0;
							}
						}
					}
					else
					{
						aPartName.clear();
						aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB11));
						aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB12));
						if(bJ2) aPartName.push_back(_LS(IDS_CMD_SECT_SOD_WEB13));
						for (int i = 0; i < nInternalWebNum; i++)
						{
							strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB_NUM), i + 2);
							aPartName.push_back(strTemp);
						}
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB1_NUM), nInternalWebNum + 2);
						aPartName.push_back(strTemp);
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB2_NUM), nInternalWebNum + 2);
						aPartName.push_back(strTemp);
						if(bJr2) 
						{
							strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB3_NUM), nInternalWebNum + 2);
							aPartName.push_back(strTemp);
						}

						if(nDeckPartLeft != -1)
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");            
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 1;
							}
						}
						else if(nDeckPartRight != -1)
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT); *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 0;
							}
						}
						else
						{
							if(pStiffPosItemList)
							{
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
								*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
							}
							if(pDefStiffPos)
							{
								*pDefStiffPos = 2;
							}
						}
					}
				}

				if(pArStrCbxDeckPartItem)
				{
					for (int i = 0; i < aPartName.size(); i++)
					{
						pArStrCbxDeckPartItem->push_back(aPartName[i]);
					}
				}

				if(pDeckName)
				{
					*pDeckName = aPartName[nDeckPart];
				}
				if(pDefStiffName)
				{
					strTemp = aPartName[nDeckPart];
					strTemp = CString(_T("W")) + strTemp.Mid(4, strTemp.GetLength() - 1);

					CString strTemp2;
					strTemp2.Empty();
					for (int j = 0; j < strTemp.GetLength(); j++)
					{
						TCHAR EachChar = strTemp.GetAt(j);
						CString strEachChar = EachChar;
						if(strEachChar != _T("-"))
						{
							strTemp2 += strEachChar;
						}
					}

					*pDefStiffName = strTemp2;
				}
			}
			else if(nDeckPos == 2) // Bot Flange
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_TOP); *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 0;
				}

				if(SectD.SectBefore.nCellNum == 0 || SectD.SectBefore.nCellNum == 1)
				{
					if(pArStrCbxDeckPartItem)
					{
						pArStrCbxDeckPartItem->clear();
						pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK));
					}      
				}
				else
				{
					if(pArStrCbxDeckPartItem)
					{
						pArStrCbxDeckPartItem->clear();
						for (int i = 0; i < SectD.SectBefore.nCellNum; i++)
						{
							strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_NUM), i + 1);
							pArStrCbxDeckPartItem->push_back(strTemp);
						}
					}    
				}

				if(nDeckPartCnt == 1)
				{
					if(pDeckName)
					{
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_BOT));
						*pDeckName = strTemp;
					}
					if(pDefStiffName)
					{
						strTemp.Format(_T("BD"));
						*pDefStiffName = strTemp;
					}
				}
				else
				{
					if(pDeckName)
					{
						strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_BOT_NUM), nDeckPart + 1);
						*pDeckName = strTemp;
					}
					if(pDefStiffName)
					{
						strTemp.Format(_T("BD%d"), nDeckPart + 1);
						*pDefStiffName = strTemp;
					}
				} 
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

void CCMSectSODDeckStiffenerDlgUtil::GetSODStiffDlgDataMCellGen(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, std::vector<CString>* pArStrCbxDeckPartItem, CString* pDeckName, CString* pDefStiffName, int* pDefStiffPos, CString* pStiffPosItemList)
{
	CString strTemp;
	if(nDeckPos == 0) // Top Flange
	{
		if(pStiffPosItemList)
		{
			*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_BOTTOM); *pStiffPosItemList += _T("\n");
		}
		if(pDefStiffPos)
		{
			*pDefStiffPos = 1;
		}

		if(SectD.SectBefore.nCellNum == 0)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));      
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
			}      
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK3));
			}     
		}
		else
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				for (int i = 0; i < SectD.SectBefore.nCellNum; i++)
				{
					strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_NUM), i + 2);
					pArStrCbxDeckPartItem->push_back(strTemp);
				}
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_NUM), SectD.SectBefore.nCellNum + 2);
				pArStrCbxDeckPartItem->push_back(strTemp);
			}
		}

		if(nDeckPartCnt == 1)
		{
			if(pDeckName)
			{
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_TOP));
				*pDeckName = strTemp;
			}
			if(pDefStiffName)
			{
				strTemp.Format(_T("TD"));
				*pDefStiffName = strTemp;
			}
		}
		else
		{
			if(pDeckName)
			{
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_TOP_NUM), nDeckPart + 1);
				*pDeckName = strTemp;
			}
			if(pDefStiffName)
			{
				strTemp.Format(_T("TD%d"), nDeckPart + 1);
				*pDefStiffName = strTemp;
			}
		}    
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB));
			}      
			if(pStiffPosItemList)
			{
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
			}
			if(pDefStiffPos)
			{
				*pDefStiffPos = 2;
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB_1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB_2));
			}   

			if(nDeckPart == 0)      
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 1;
				}
			}
			else if(nDeckPart == 1) 
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 0;
				}
			}
		}
		else
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_WEB_1));
				for (int i = 0; i < SectD.SectBefore.nCellNum - 1; i++)
				{
					strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB_NUM), i + 2);
					pArStrCbxDeckPartItem->push_back(strTemp);
				}
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB_NUM), SectD.SectBefore.nCellNum + 1);
				pArStrCbxDeckPartItem->push_back(strTemp);
			}

			if(nDeckPart == 0)
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 1;
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1)
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 0;
				}
			}
			else
			{
				if(pStiffPosItemList)
				{
					*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_LEFT);  *pStiffPosItemList += _T("\n");
					*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_RIGHT); *pStiffPosItemList += _T("\n");
					*pStiffPosItemList += _LS(IDS_CMD_SECT_SOD_BOTH);  *pStiffPosItemList += _T("\n");
				}
				if(pDefStiffPos)
				{
					*pDefStiffPos = 2;
				}
			}
		}

		if(nDeckPartCnt == 1)
		{
			if(pDeckName)
			{
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB));
				*pDeckName = strTemp;
			}
			if(pDefStiffName)
			{
				strTemp.Format(_T("W"));
				*pDefStiffName = strTemp;
			}
		}
		else
		{
			if(pDeckName)
			{
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_WEB_NUM), nDeckPart + 1);
				*pDeckName = strTemp;
			}
			if(pDefStiffName)
			{
				strTemp.Format(_T("W%d"), nDeckPart + 1);
				*pDefStiffName = strTemp;
			}
		}    
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(pStiffPosItemList)
		{
			*pStiffPosItemList = _LS(IDS_CMD_SECT_SOD_TOP); *pStiffPosItemList += _T("\n");
		}
		if(pDefStiffPos)
		{
			*pDefStiffPos = 0;
		}

		if(SectD.SectBefore.nCellNum == 0)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));      
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
			}  
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK2));
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK3));
			}   
		}
		else
		{
			if(pArStrCbxDeckPartItem)
			{
				pArStrCbxDeckPartItem->clear();
				pArStrCbxDeckPartItem->push_back(_LS(IDS_CMD_SECT_SOD_DECK1));
				for (int i = 0; i < SectD.SectBefore.nCellNum; i++)
				{
					strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_NUM), i + 2);
					pArStrCbxDeckPartItem->push_back(strTemp);
				}
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_NUM), SectD.SectBefore.nCellNum + 2);
				pArStrCbxDeckPartItem->push_back(strTemp);
			}
		}

		if(nDeckPartCnt == 1)
		{
			if(pDeckName)
			{
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_BOT)); 
				*pDeckName = strTemp;
			}
			if(pDefStiffName)
			{
				strTemp.Format(_T("BD"));
				*pDefStiffName = strTemp;
			}
		}
		else
		{
			if(pDeckName)
			{
				strTemp.Format(_LS(IDS_CMD_SECT_SOD_DECK_BOT_NUM), nDeckPart + 1);
				*pDeckName = strTemp;
			}
			if(pDefStiffName)
			{
				strTemp.Format(_T("BD%d"), nDeckPart + 1);
				*pDefStiffName = strTemp;
			}
		} 
	}
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapID(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg"); //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(SectD.nStype == D_SECT_TYPE_STLG_B || 
		SectD.nStype == D_SECT_TYPE_COMPO_STLG_B ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_B) ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_B))
	{
		if     (nDeckPos==0 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerTopDeck1Left.svg");  //IDB_CMD_SECT_STIFFENER_B_TD1L;
		else if(nDeckPos==0 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerTopDeck1Right.svg"); //IDB_CMD_SECT_STIFFENER_B_TD1R;
		else if(nDeckPos==0 && nDeckPart==1 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerTopDeck2Left.svg");  //IDB_CMD_SECT_STIFFENER_B_TD2L;
		else if(nDeckPos==0 && nDeckPart==1 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerTopDeck2Right.svg"); //IDB_CMD_SECT_STIFFENER_B_TD2R;
		else if(nDeckPos==0 && nDeckPart==2 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerTopDeck3Left.svg");  //IDB_CMD_SECT_STIFFENER_B_TD3L;
		else if(nDeckPos==0 && nDeckPart==2 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerTopDeck3Right.svg"); //IDB_CMD_SECT_STIFFENER_B_TD3R;
		else if(nDeckPos==1 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerLeftWebTop.svg");    //IDB_CMD_SECT_STIFFENER_B_LWT;
		else if(nDeckPos==1 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerLeftWebBot.svg");    //IDB_CMD_SECT_STIFFENER_B_LWB;
		else if(nDeckPos==2 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerRightWebTop.svg");   //IDB_CMD_SECT_STIFFENER_B_RWT;
		else if(nDeckPos==2 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerRightWebBot.svg");   //IDB_CMD_SECT_STIFFENER_B_RWB;
		else if(nDeckPos==3 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerBotDeck1Left.svg");  //IDB_CMD_SECT_STIFFENER_B_BD1L;
		else if(nDeckPos==3 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerBotDeck1Right.svg"); //IDB_CMD_SECT_STIFFENER_B_BD1R;
		else if(nDeckPos==3 && nDeckPart==1 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerBotDeck2Left.svg");  //IDB_CMD_SECT_STIFFENER_B_BD2L;
		else if(nDeckPos==3 && nDeckPart==1 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerBotDeck2Right.svg"); //IDB_CMD_SECT_STIFFENER_B_BD2R;
		else if(nDeckPos==3 && nDeckPart==2 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerBotDeck3Left.svg");  //IDB_CMD_SECT_STIFFENER_B_BD3L;
		else if(nDeckPos==3 && nDeckPart==2 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Box_StiffenerBotDeck3Right.svg"); //IDB_CMD_SECT_STIFFENER_B_BD3R;
	}
	else if(SectD.nStype == D_SECT_TYPE_STLG_I || SectD.nStype == D_SECT_TYPE_COMPO_STLG_I ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_I) ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_I))
	{
		if     (nDeckPos==0 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerTopDeck1Left.svg");   //IDB_CMD_SECT_STIFFENER_I_TD1L;
		else if(nDeckPos==0 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerTopDeck1Right.svg");  //IDB_CMD_SECT_STIFFENER_I_TD1R;
		else if(nDeckPos==0 && nDeckPart==1 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerTopDeck2Left.svg");   //IDB_CMD_SECT_STIFFENER_I_TD2L;
		else if(nDeckPos==0 && nDeckPart==1 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerTopDeck2Right.svg");  //IDB_CMD_SECT_STIFFENER_I_TD2R;	
		else if(nDeckPos==1 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerWebTop.svg");         //IDB_CMD_SECT_STIFFENER_I_WT;
		else if(nDeckPos==1 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerWebBot.svg");         //IDB_CMD_SECT_STIFFENER_I_WB;	
		else if(nDeckPos==2 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerBotDeck1Left.svg");   //IDB_CMD_SECT_STIFFENER_I_BD1L;
		else if(nDeckPos==2 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerBotDeck1Right.svg");  //IDB_CMD_SECT_STIFFENER_I_BD1R;
		else if(nDeckPos==2 && nDeckPart==1 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerBotDeck2Left.svg");   //IDB_CMD_SECT_STIFFENER_I_BD2L;
		else if(nDeckPos==2 && nDeckPart==1 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\I_StiffenerBotDeck2Right.svg");  //IDB_CMD_SECT_STIFFENER_I_BD2R;	
	}
	else if(SectD.nStype == D_SECT_TYPE_COMPO_STLG_TUB ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_STLG_TUB))
	{
		//     if     (nDeckPos==0 && nDeckPart==0 && nRefPos==0) bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_SECT_STIFFENER_B_TD1L; Top은 일단 뺌
		//     else if(nDeckPos==0 && nDeckPart==0 && nRefPos==1) bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_SECT_STIFFENER_B_TD1R;
		//     else if(nDeckPos==0 && nDeckPart==1 && nRefPos==0) bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_SECT_STIFFENER_B_TD2L;
		//     else if(nDeckPos==0 && nDeckPart==1 && nRefPos==1) bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_SECT_STIFFENER_B_TD2R;
		//     else if(nDeckPos==0 && nDeckPart==2 && nRefPos==0) bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_SECT_STIFFENER_B_TD3L;
		//     else if(nDeckPos==0 && nDeckPart==2 && nRefPos==1) bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_SECT_STIFFENER_B_TD3R;
		if     (nDeckPos==1 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");     //IDB_CMD_SECT_STIFFENER_TUB_LWT;
		else if(nDeckPos==1 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebBot.svg");     //IDB_CMD_SECT_STIFFENER_TUB_LWB;
		else if(nDeckPos==2 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerRightWebTop.svg");    //IDB_CMD_SECT_STIFFENER_TUB_RWT;
		else if(nDeckPos==2 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerRightWebBot.svg");    //IDB_CMD_SECT_STIFFENER_TUB_RWB;
		else if(nDeckPos==3 && nDeckPart==0 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerBotDeck1Left.svg");   //IDB_CMD_SECT_STIFFENER_TUB_BD1L;
		else if(nDeckPos==3 && nDeckPart==0 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerBotDeck1Right.svg");  //IDB_CMD_SECT_STIFFENER_TUB_BD1R;
		else if(nDeckPos==3 && nDeckPart==1 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerBotDeck2Left.svg");   //IDB_CMD_SECT_STIFFENER_TUB_BD2L;
		else if(nDeckPos==3 && nDeckPart==1 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerBotDeck2Right.svg");  //IDB_CMD_SECT_STIFFENER_TUB_BD2R;
		else if(nDeckPos==3 && nDeckPart==2 && nRefPos==0) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerBotDeck3Left.svg");   //IDB_CMD_SECT_STIFFENER_TUB_BD3L;
		else if(nDeckPos==3 && nDeckPart==2 && nRefPos==1) bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerBotDeck3Right.svg");  //IDB_CMD_SECT_STIFFENER_TUB_BD3R;
	}
	else if(SectD.nStype == D_SECT_TYPE_STLG_MCELL ||
		(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.Shape == D_SECT_SHAPE_STLG_MCELL))
	{
		bitmapID = GetBitmapIDMCell(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else
	{
		ASSERT(0);
	}
	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCell(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(SectD.SectBefore.nFlangeShape == 0)
	{
		bitmapID = GetBitmapIDMCellGen(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else if(SectD.SectBefore.nFlangeShape == 1)
	{
		bitmapID = GetBitmapIDMCellRebar1(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else if(SectD.SectBefore.nFlangeShape == 2)
	{
		bitmapID = GetBitmapIDMCellRebar2(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else if(SectD.SectBefore.nFlangeShape == 3)
	{
		bitmapID = GetBitmapIDMCellClosed(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else
	{
		ASSERT(0);
	}
	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellGen(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(nDeckPos == 0) // Top Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_TD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_TD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_TD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_TD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_TD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_TD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_TD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_TD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_TD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_TD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_TD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_TD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1)  // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_TD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_TD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_TD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_TD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_TD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_TD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_TD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_TD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_TD3_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_TD3_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD3_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD3_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) // Deck4
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD4_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_TD4_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_TD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD3_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD3_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 뒤에서 2번째
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TDn+1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TDN1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Gen_Cell 3_TDn+1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TDN1_R;
				}
				else
				{
					ASSERT(0);
				}
			}    
			else if(nDeckPart == nDeckPartCnt - 1) // 제일 뒤
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TDn+1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TDN2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TDn+2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TDN2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD4_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_TD4_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_TD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_WW_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_WW_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_WW_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_WW_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_WW_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_WW_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_WW_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_WW_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Web Left
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_WW1_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_WW1_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web Right
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_WW2_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_WW2_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Web 1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_WW1_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_WW1_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_WW2_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_WW2_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_WW3_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_WW3_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Web 1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WW1_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WW1_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WW2_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WW2_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 끝에서 두번째
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WWn_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WWN_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WWn_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WWN_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WWn+1_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WWN1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WWn+1_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WWN1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WW3_T.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_WW3_B.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
		}  
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_BD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_BD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_BD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_BD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_BD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_BD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Left)_BD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_LEFT_BD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_BD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_BD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_BD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_BD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1)  // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_BD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_BD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 0(Right)_BD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL0_RIGHT_BD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_BD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_BD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_BD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_BD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_BD3_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 1_BD3_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL1_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD3_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD3_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) // Deck4
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD4_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 2_BD4_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL2_BD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD3_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD3_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 뒤에서 2번째
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BDn+1_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BDN1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BDn+1_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BDN1_R;
				}
				else
				{
					ASSERT(0);
				}
			}    
			else if(nDeckPart == nDeckPartCnt - 1) // 제일 뒤
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BDn+2_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BDN2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BDn+2_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BDN2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD4_L.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\Gen_Cell 3_BD4_R.svg");  //IDB_CMD_STLG_MCELL_GEN_CELL3_BD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellRebar1(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID = _T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(nDeckPos == 0) // Top Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_TD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_TD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_TD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_TD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_TD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_TD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1)  // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_TD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_TD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_TD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_TD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) // Deck4
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_TD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_TD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 뒤에서 2번째
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TDn+1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TDN1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TDn+1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TDN1_R;
				}
				else
				{
					ASSERT(0);
				}
			}    
			else if(nDeckPart == nDeckPartCnt - 1) // 제일 뒤
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TDn+2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TDN2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TDn+2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TDN2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_TD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_TD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\GenAARF-1_Cell 0(Left)_WW_TAAA.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_WW_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_WW_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_WW_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_WW_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_WW_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_WW_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_WW_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Web Left
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_WW1_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_WW1_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web Right
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_WW2_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 1_WW2_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Web 1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_WW1_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_WW1_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_WW2_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_WW2_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 3
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_WW3_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 2_WW3_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Web 1
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WW1_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WW1_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WW2_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WW2_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 끝에서 두번째
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WWn_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WWN_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WWn_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WWN_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WWn+1_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WWN1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WWn+1_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WWN1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WW3_T.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 3_WW3_B.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
		}  
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_BD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_BD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_BD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID = _T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Left)_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_LEFT_BD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_BD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_BD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1)  // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_BD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 0(Right)_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL0_RIGHT_BD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 1_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 1_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 1_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 1_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 1_BD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 1_BD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL1_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) // Deck4
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 2_BD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL2_BD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 뒤에서 2번째
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BDn+1_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BDN1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BDn+1_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BDN1_R;
				}
				else
				{
					ASSERT(0);
				}
			}    
			else if(nDeckPart == nDeckPartCnt - 1) // 제일 뒤
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BDn+2_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BDN2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BDn+2_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BDN2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-1_Cell 3_BD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_1_CELL3_BD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellRebar2(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID =_T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(nDeckPos == 0) // Top Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_TD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_TD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_TD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_TD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_TD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_TD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1)  // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_TD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_TD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_TD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_TD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) // Deck4
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_TD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_TD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 뒤에서 2번째
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TDn+1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TDN1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TDn+1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TDN1_R;
				}
				else
				{
					ASSERT(0);
				}
			}    
			else if(nDeckPart == nDeckPartCnt - 1) // 제일 뒤
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TDn+2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TDN2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TDn+2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TDN2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_TD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_TD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_WW_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_WW_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_WW_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_WW_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_WW_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_WW_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_WW_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_WW_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Web Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_WW1_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_WW1_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_WW2_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_WW2_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Web 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_WW1_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_WW1_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_WW2_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_WW2_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_WW3_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_WW3_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Web 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WW1_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WW1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WW1_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WW1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WW2_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WW2_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 끝에서 두번째
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WWn_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WWN_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WWn_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WWN_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WWn+1_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WWN1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WWn+1_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WWN1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WW3_T.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_WW3_B.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
		}  
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_BD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_BD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_BD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_BD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Deck1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_BD1_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_BD1_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1)  // Deck2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_BD2_L;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_BD2_R;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_BD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_BD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) // Deck4
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_BD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_BD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) // Deck3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD3_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD3_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 뒤에서 2번째
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BDn+1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BDN1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BDn+1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BDN1_R;
				}
				else
				{
					ASSERT(0);
				}
			}    
			else if(nDeckPart == nDeckPartCnt - 1) // 제일 뒤
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BDn+2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BDN2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BDn+2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BDN2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD4_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD4_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_BD4_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_BD4_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 3) // Side Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_SF_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_SF_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Left)_SF_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_LEFT_SF_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_SF_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_SF_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell 0(Right)_SF_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL0_RIGHT_SF_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Side Flange 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_SF1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_SF1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_SF1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_SF1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Side Flange 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_SF2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_SF2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell1_SF2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL1_SF2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Side Flange 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_SF1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_SF1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_SF1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_SF1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Side Flange 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_SF2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_SF2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell2_SF2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL2_SF2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Side Flange 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_SF1_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_SF1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_SF1_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_SF1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Side Flange 1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_SF2_L.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_SF2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\RF-2_Cell3_SF2_R.svg");  //IDB_CMD_STLG_MCELL_RF_2_CELL3_SF2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellClosed(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID =_T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	BOOL bJ2, bJr2;
	bJ2 = bJr2 = FALSE;
	if(SectD.SectBefore.nJoint & 0x00000002) bJ2 = TRUE;
	if(SectD.SectBefore.nJoint & 0x00000008) bJr2 = TRUE;

	if(SectD.SectBefore.nCellNum == 0 && SectD.SectBefore.nCellType == 0)
	{
		bJr2 = FALSE;
	}
	else if(SectD.SectBefore.nCellNum == 0 && SectD.SectBefore.nCellType == 1)
	{
		bJ2 = FALSE;
	}

	if(!bJ2 && !bJr2)
	{
		bitmapID = GetBitmapIDMCellClosedJ1J1(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else if(!bJ2 && bJr2)
	{
		bitmapID = GetBitmapIDMCellClosedJ1J2(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else if(bJ2 && !bJr2)
	{
		bitmapID = GetBitmapIDMCellClosedJ2J1(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else if(bJ2 && bJr2)
	{
		bitmapID = GetBitmapIDMCellClosedJ2J2(SectD, nDeckPos, nDeckPart, nDeckPartCnt, nRefPos);
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellClosedJ1J1(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID =_T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(nDeckPos == 0) // Top Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_TD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_TD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_TD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_TD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_TD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_TD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TDn_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TDn_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TD3_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_TD3_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 2-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW2-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW2_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW2-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW2_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW2-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW2_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_WW2-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_WW2_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 3-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW3-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW3_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW3-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW3_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 4) //Web 3-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW3-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW3_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_WW3-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_WW3_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 3) // 끝에서 2번째 Web
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WWn_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WWN_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WWn_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WWN_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 끝 WebN-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WWn+1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WWN1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WWn+1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝 WebN-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WWn+1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WWN1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WWn+1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WWN1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_WW3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
		}  
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_BD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell1_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL1_BD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_BD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_BD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_BD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell2_BD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL2_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BDn_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BDn_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BD3_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell3_BD3_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL3_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellClosedJ1J2(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID =_T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(nDeckPos == 0) // Top Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_TD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_TD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_TD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_TD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_TD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_TD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TDn_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TDn_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TD3_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_TD3_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 2) // Web1-3
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_3_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_3_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 2-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW2-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW2_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW2-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW2_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW2-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW2_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW2-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW2_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 4) //Web 2-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW2-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW2_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_WW2-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_WW2_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 3-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW3-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW3_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW3-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW3_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 4) //Web 3-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW3-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW3_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW3-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW3_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 5) //Web 3-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW3-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW3_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_WW3-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_WW3_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 4) // 끝에서 3번째 Web
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WWN_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WWN_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 3) // 끝 WebN-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn+1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WWN1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn+1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 끝 WebN-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn+1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WWN1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn+1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WWN1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn+1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WWN1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WWn+1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WWN1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_WW3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
		}  
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Left)_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_LEFT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_BD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell1_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL1_BD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_BD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_BD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_BD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell2_BD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL2_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BDn_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BDn_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BD3_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1j2_Cell3_BD3_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1J2_CELL3_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellClosedJ2J1(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID =_T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..

	if(nDeckPos == 0) // Top Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj1_Cell0(Right)_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell1_TD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_TD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell1_TD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_TD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell2_TD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell2_TD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell2_TD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell2_TD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TDn_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TDn_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TD3_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2j1_Cell3_TD3_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 2) // Web1-3
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_3_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_3_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 1-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW2_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW2_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 4) //Web 2-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW2_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_WW2_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 1-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 4) //Web 3-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_3_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_3_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 5) //Web 3-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_3_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_WW_3_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 1-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 3) // 끝에서 2번째 Web
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WWN_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WWN_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 끝 WebN-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WWN1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝 WebN-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WWN1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WWN1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
		}  
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ1_CELL0_RIGHT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_BD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL1_BD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL2_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2J1_CELL3_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}

CString CCMSectSODDeckStiffenerDlgUtil::GetBitmapIDMCellClosedJ2J2(T_SECT_D& SectD, int nDeckPos, int nDeckPart, int nDeckPartCnt, int nRefPos)
{
	CString bitmapID;
	bitmapID =_T("SVG\\illustration\\Dialog\\Tub_StiffenerLeftWebTop.svg");  //IDB_CMD_SECT_STIFFENER_TUB_LWT; // 버그가 있어도 죽지는 않도록 아무거나 세팅..
	
	if(nDeckPos == 0) // Top Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_TD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_TD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_TD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_TD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_TD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else if(nDeckPos == 1) // Web
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\GenAAAAA.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 2) // Web1-3
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_3_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_WW1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_WW1_3_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nDeckPart == 0) // Web1-1
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_1_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_1_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 1) // Web1-2
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_2_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_2_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else if(nDeckPart == 2) // Web1-3
				{
					if(nRefPos == 0)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_3_T;
					}
					else if(nRefPos == 1)
					{
						bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_WW1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_WW1_3_B;
					}
					else
					{
						ASSERT(0);
					}
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 1-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW2-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW2_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW2-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW2_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 4) //Web 2-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW2-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW2_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW2-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW2_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 5) //Web 2-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW2-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW2_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_WW2-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_WW2_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 1-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 4) //Web 3-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW3-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW3_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW3-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW3_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 5) //Web 3-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW3-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW3_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW3-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW3_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 6) //Web 3-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW3-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW3_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_WW3-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_WW3_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Web 1-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) //Web 1-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 2) //Web 1-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 3) //Web 2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 4) // 끝에서 2번째 Web
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WWN_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WWN_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 3) // 끝 WebN-1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn+1-1_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WWN1_1_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn+1-1_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW1_1_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 2) // 끝 WebN-2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn+1-2_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WWN1_2_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn+1-2_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WWN1_2_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝 WebN-3
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn+1-3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WWN1_3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WWn+1-3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WWN1_3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW3_T.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW3_T;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_WW3_B.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_WW3_B;
				}
				else
				{
					ASSERT(0);
				}
			}
		}  
	}
	else if(nDeckPos == 2) // Bot Flange
	{
		if(SectD.SectBefore.nCellNum == 0)
		{
			if(SectD.SectBefore.nCellType == 0) // Left
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Left)_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_LEFT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(SectD.SectBefore.nCellType == 1) // Right
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_BD_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell0(Right)_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL0_RIGHT_BD_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 1)
		{
			if(nRefPos == 0)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_BD_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_BD_L;
			}
			else if(nRefPos == 1)
			{
				bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell1_BD_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL1_BD_R;
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(SectD.SectBefore.nCellNum == 2)
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_BD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_BD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_BD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell2_BD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL2_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				ASSERT(0);
			}
		}
		else
		{
			if(nDeckPart == 0) // Deck1
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BD1_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BD1_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BD1_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BD1_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == 1) // Deck2
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BD2_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BD2_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BD2_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BD2_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else if(nDeckPart == nDeckPartCnt - 1) // 끝
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BDn_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BDN_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BDn_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BDN_R;
				}
				else
				{
					ASSERT(0);
				}
			}
			else
			{
				if(nRefPos == 0)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BD3_L.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BD3_L;
				}
				else if(nRefPos == 1)
				{
					bitmapID =_T("SVG\\illustration\\Dialog\\Clj2_Cell3_BD3_R.svg");  //IDB_CMD_STLG_MCELL_CLJ2_CELL3_BD3_R;
				}
				else
				{
					ASSERT(0);
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}

	return bitmapID;
}