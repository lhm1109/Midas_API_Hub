// CMSectItemPSCIDB.cpp : implementation file
//
// 2003. 09. 05  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC_I_DB.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMidDB 

CCMSectItemPSCIDB::CCMSectItemPSCIDB()
{
	m_aDBNameList.RemoveAll();
	m_aSize.RemoveAll();
	m_aOffset.RemoveAll();

	MakeData();
}

CCMSectItemPSCIDB::~CCMSectItemPSCIDB()
{
	CArray<double, double>* pData;
	int i;
	int nNum = m_aSize.GetSize();
	for (i = 0; i < nNum; i++)
	{
		pData = m_aSize[i];
	if(pData) delete pData;
	m_aSize[i] = NULL;	
	}

	//메모리릭 발생하여 추가함 by kys0522 141205
	CArray<BOOL, BOOL>* pDataOffset;
	nNum = m_aOffset.GetSize();
	for (i = 0; i < nNum; i++)
	{
	  pDataOffset = m_aOffset[i];
	  if(pDataOffset) delete pDataOffset;
	  m_aOffset[i] = NULL;
	}
}

// *^^* 데이터 추가되면 여기 수정 
void CCMSectItemPSCIDB::MakeData()
{
	// DB Name
	CString aDBName[] = { 
		_LS(IDS_CMD_PSC_I_None), 
		_LS(IDS_CMD_PSC_I_AASHTO_TYPE1),
		_LS(IDS_CMD_PSC_I_AASHTO_TYPE2), 
		_LS(IDS_CMD_PSC_I_AASHTO_TYPE3),
		_LS(IDS_CMD_PSC_I_AASHTO_TYPE4),
		_LS(IDS_CMD_PSC_I_AASHTO_TYPE5),
		_LS(IDS_CMD_PSC_I_AASHTO_TYPE6),
		_LS(IDS_CMD_PSC_I_MODIFIED_AASHTO_TYPE4_60), 
		_LS(IDS_CMD_PSC_I_MODIFIED_AASHTO_TYPE4_66), 
		_LS(IDS_CMD_PSC_I_MODIFIED_AASHTO_TYPE4_72),
		_LS(IDS_CMD_PSC_TEE_AASHTO_BT54),
		_LS(IDS_CMD_PSC_TEE_AASHTO_BT63),
		_LS(IDS_CMD_PSC_TEE_AASHTO_BT72),
		_LS(IDS_CMD_PSC_I_CALTRANS_3_0),
		_LS(IDS_CMD_PSC_I_CALTRANS_3_6),
		_LS(IDS_CMD_PSC_I_CALTRANS_4_0),
		_LS(IDS_CMD_PSC_I_CALTRANS_4_6),
		_LS(IDS_CMD_PSC_I_CALTRANS_5_0),
		_LS(IDS_CMD_PSC_I_CALTRANS_5_6),
		_LS(IDS_CMD_PSC_I_IADOT_A_BEAM_IA), 
		_LS(IDS_CMD_PSC_I_IADOT_B_BEAM_IA), 
		_LS(IDS_CMD_PSC_I_IADOT_C_BEAM_IA), 
		_LS(IDS_CMD_PSC_I_IADOT_D_BEAM_IA), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_29), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_37), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_45), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_53), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_61), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_69), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_77), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_85), 
		_LS(IDS_CMD_PSC_I_VADOT_PCBT_93), 
		_LS(IDS_CMD_PSC_I_ODOT_WF36_49), 
		_LS(IDS_CMD_PSC_I_ODOT_WF42_49), 
		_LS(IDS_CMD_PSC_I_ODOT_WF48_49), 
		_LS(IDS_CMD_PSC_I_ODOT_WF54_49),
		_LS(IDS_CMD_PSC_I_ODOT_WF60_49), 
		_LS(IDS_CMD_PSC_I_ODOT_WF66_49), 
		_LS(IDS_CMD_PSC_I_ODOT_WF72_49), 
		_LS(IDS_CMD_PSC_I_MODOT_GDR1_MO), 
		_LS(IDS_CMD_PSC_I_MODOT_GDR2_MO), 
		_LS(IDS_CMD_PSC_I_MODOT_GDR3_MO), 
		_LS(IDS_CMD_PSC_I_MODOT_GDR4_MO), 
		_LS(IDS_CMD_PSC_I_MODOT_GDR5_MO), 
		_LS(IDS_CMD_PSC_I_WIDOT_28_PSC_WI), 
		_LS(IDS_CMD_PSC_I_WIDOT_36_PSC_WI),
		_LS(IDS_CMD_PSC_I_TXDOT_IGD_T28), 
		_LS(IDS_CMD_PSC_I_TXDOT_IGD_T34), 
		_LS(IDS_CMD_PSC_I_TXDOT_IGD_T40), 
		_LS(IDS_CMD_PSC_I_TXDOT_IGD_T46),
		_LS(IDS_CMD_PSC_I_TXDOT_IGD_T54), 
		_LS(IDS_CMD_PSC_I_TXDOT_IGD_T62), 
		_LS(IDS_CMD_PSC_I_TXDOT_IGD_T70)
	};

	// Size (단위 : Inch)
	double aSize[][33] = 
	{
		{ 0, 0,   0,   0, 0, 0,  0,   0, 0, 0, 0,   0,  0,  0, 0,  0, 0, 0,   0,   0, 0, 0,  0,   0, 0, 0, 0,   0,  0,  0, 0,  0, 0},
		
		{ 0, 4,   3,   0, 0, 11, 5,   0, 0, 5, 3,   6,  0 , 0, 8,  0, 0, 4,   3,   0, 0, 11, 5,   0, 0, 5, 3,   6,  0,  0, 8,  0, 0},
		{ 0, 6,   3,   0, 0, 15, 6,   0, 0, 6, 3,   6,  0,  0, 9,  0, 0, 6,   3,   0, 0, 15, 6,   0, 0, 6, 3,   6,  0,  0, 9,  0, 0},
		{ 0, 7,   4.5, 0, 0, 19, 7.5, 0, 0, 7, 3.5, 8,  0,  0, 11, 0, 0, 7,   4.5, 0, 0, 19, 7.5, 0, 0, 7, 3.5, 8,  0,  0, 11, 0, 0},
		{ 0, 8,   6,   0, 0, 23, 9,   0, 0, 8, 4,   10, 0,  0, 13, 0, 0, 8,   6,   0, 0, 23, 9,   0, 0, 8, 4,   10, 0,  0, 13, 0, 0},
		{ 0, 5,   7,   3, 0, 33, 10,  0, 0, 8, 4,   21, 13, 0, 14, 0, 0, 5,   7,   3, 0, 33, 10,  0, 0,	8, 4,   21, 13, 0, 14, 0, 0},
		{ 0, 5,   7,   3, 0, 42, 10,  0, 0, 8, 4,   21, 13, 0, 14, 0, 0, 5,   7,   3, 0, 42, 10,  0, 0, 8, 4,   21, 13, 0, 14, 0, 0},
		
		{ 0, 4, 5, 2, 0, 34, 9, 0, 0, 8, 4, 18, 11, 0, 13, 0, 0, 4, 5, 2, 0, 34, 9, 0, 0, 8, 4, 18, 11, 0, 13, 0, 0 },
		{ 0, 4, 5, 2, 0, 40, 9, 0, 0, 8, 4, 18, 11, 0, 13, 0, 0, 4, 5, 2, 0, 40, 9, 0, 0, 8, 4, 18, 11, 0, 13, 0, 0 },
		{ 0, 4, 5, 2, 0, 46, 9, 0, 0, 8, 4, 24, 17, 0, 13, 0, 0, 4, 5, 2, 0, 46, 9, 0, 0, 8, 4, 24, 17, 0, 13, 0, 0 },
		
		{ 0, 3.5, 4,   2, 0, 36, 4.5, 0, 0, 6, 3,   21, 16, 0, 13, 0, 0, 3.5, 4,   2, 0, 36, 4.5, 0, 0, 6, 3,   21, 16, 0, 13, 0, 0},
		{ 0, 3.5, 4,   2, 0, 45, 4.5, 0, 0, 6, 3,   21, 16, 0, 13, 0, 0, 3.5, 4,   2, 0, 45, 4.5, 0, 0, 6, 3,   21, 16, 0, 13, 0, 0},
		{ 0, 3.5, 4,   2, 0, 54, 4.5, 0, 0, 6, 3,   21, 16, 0, 13, 0, 0, 3.5, 4,   2, 0, 54, 4.5, 0, 0, 6, 3,   21, 16, 0, 13, 0, 0},

		{ 0, 3, 6, 0, 0, 15, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0,	3, 6, 0, 0, 15, 6, 0, 0, 6,	3.5, 9.5, 0, 0, 9.5, 0, 0},
		{ 0, 3, 6, 0, 0, 21, 6,	0, 0,	6, 3.5,	9.5, 0, 0, 9.5, 0, 0,	3, 6,	0, 0,	21,	6, 0,	0, 6,	3.5, 9.5, 0, 0, 9.5, 0,	0},
		{ 0, 3, 6, 0, 0, 27, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0, 3, 6, 0, 0, 27, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0},
		{ 0, 3, 6, 0, 0, 33, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0, 3, 6, 0, 0, 33, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0},
		{ 0, 3, 6, 0, 0, 39, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0, 3, 6, 0, 0, 39, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0},
		{ 0, 3, 6, 0, 0, 45, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0, 3, 6, 0, 0, 45, 6, 0, 0, 6, 3.5, 9.5, 0, 0, 9.5, 0, 0},
		 
		{ 0, 4, 1, 0, 0,   16,   6, 0, 0, 5,   3, 6.5, 0, 0, 8.5, 0, 0, 4, 1, 0, 0,   16,   6, 0, 0, 5,   3, 6.5, 0, 0, 8.5, 0, 0},
		{ 0, 5, 1, 0, 0,   20,   6, 0, 0, 7,   3, 6.5, 0, 0, 8.5, 0, 0, 5, 1, 0, 0,   20,   6, 0, 0, 7,   3, 6.5, 0, 0, 8.5, 0, 0},
		{ 0, 5, 1, 0, 0,   25,   6, 0, 0, 8, 4.5,   8, 0, 0,  10, 0, 0, 5, 1, 0, 0,   25,   6, 0, 0, 8, 4.5,   8, 0, 0,  10, 0, 0},
		{ 0, 6, 1, 0, 0, 31.5, 7.5, 0, 0, 8, 3.5,  10, 0, 0,  11, 0, 0, 6, 1, 0, 0, 31.5, 7.5, 0, 0, 8, 3.5,  10, 0, 0,  11, 0, 0},

		{ 0, 4, 3.5, 0, 1.5,  8, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5,  8, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 16, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 16, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 24, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 24, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 32, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 32, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 40, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 40, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 48, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 48, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 56, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 56, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 64, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 64, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},
		{ 0, 4, 3.5, 0, 1.5, 72, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9, 4, 3.5, 0, 1.5, 72, 6.5, 0, 3.5, 7, 3.5, 23.5, 0, 18, 16, 0, 9},

		{ 0, 5, 6, 0, 3, 10.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14, 5, 6, 0, 3, 10.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14},
		{ 0, 5, 6, 0, 3, 16.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14, 5, 6, 0, 3, 16.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14},
		{ 0, 5, 6, 0, 3, 22.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14, 5, 6, 0, 3, 22.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14},
		{ 0, 5, 6, 0, 3, 28.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14, 5, 6, 0, 3, 28.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14},
		{ 0, 5, 6, 0, 3, 34.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14, 5, 6, 0, 3, 34.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14},
		{ 0, 5, 6, 0, 3, 40.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14, 5, 6, 0, 3, 40.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14},
		{ 0, 5, 6, 0, 3, 46.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14, 5, 6, 0, 3, 46.5, 9, 0, 7, 5.5, 4, 24.5, 0, 17.5, 20, 0, 14},

		{ 0, 4, 1, 0, 0, 16, 6, 0, 0, 5, 3, 6.5, 0, 0, 8.5, 0, 0, 4, 1, 0, 0, 16, 6, 0, 0, 5, 3, 6.5, 0, 0, 8.5, 0, 0},
		{ 0, 5, 1, 0, 0, 20, 6, 0, 0, 7, 3, 6.5, 0, 0, 8.5, 0, 0, 5, 1, 0, 0, 20, 6, 0, 0, 7, 3, 6.5, 0, 0, 8.5, 0, 0},
		{ 0, 5, 1, 0, 0, 25, 6, 0, 0, 8, 3, 6.5, 0, 0, 8.5, 0, 0, 5, 1, 0, 0, 25, 6, 0, 0, 8, 3, 6.5, 0, 0, 8.5, 0, 0},
		{ 0, 5, 4.5, 0, 0, 31.5, 7, 0, 0, 6, 3.25, 12, 0, 0, 12, 0, 0, 5, 4.5, 0, 0, 31.5, 7, 0, 0, 6, 3.25, 12, 0, 0, 12, 0, 0},
		{ 0, 4, 4, 2, 0, 54, 4.5, 0, 0, 6, 3, 21, 16, 0, 13, 0, 0, 4, 4, 2, 0, 54, 4.5, 0, 0, 6, 3, 21, 16, 0, 13, 0, 0},

		{ 0, 3, 4, 0, 0, 11, 6, 0, 0, 4, 3, 9, 0, 0, 9, 0, 0, 3, 4, 0, 0, 11, 6, 0, 0, 4, 3, 9, 0, 0, 9, 0, 0},
		{ 0, 6, 3, 0, 0, 15, 6, 0, 0, 6, 3, 6, 0, 0, 9, 0, 0, 6, 3, 0, 0, 15, 6, 0, 0, 6, 3, 6, 0, 0, 9, 0, 0},

		{ 0, 3.5, 4, 2, 0,  6, 7.75, 0, 4.75, 6.75, 3.5, 18, 12.5, 0, 16, 0, 9.5, 3.5, 4, 2, 0,  6, 7.75, 0, 4.75, 6.75, 3.5, 18, 12.5, 0, 16, 0, 9.5},
		{ 0, 3.5, 4, 2, 0, 12, 7.75, 0, 4.75, 6.75, 3.5, 18, 12.5, 0, 16, 0, 9.5, 3.5, 4, 2, 0, 12, 7.75, 0, 4.75, 6.75, 3.5, 18, 12.5, 0, 16, 0, 9.5},
		{ 0, 3.5, 4, 2, 0, 18, 7.75, 0, 4.75, 6.75, 3.5, 18, 12.5, 0, 16, 0, 9.5, 3.5, 4, 2, 0, 18, 7.75, 0, 4.75, 6.75, 3.5, 18, 12.5, 0, 16, 0, 9.5},
		{ 0, 3.5, 4, 2, 0, 22, 7.75, 0, 4.75, 8.75, 3.5, 18, 12.5, 0, 16, 0, 9.5, 3.5, 4, 2, 0, 22, 7.75, 0, 4.75, 8.75, 3.5, 18, 12.5, 0, 16, 0, 9.5},
		{ 0, 3.5, 4, 2, 0, 30, 7.75, 0, 4.75, 8.75, 3.5, 18, 12.5, 0, 16, 0, 9.5, 3.5, 4, 2, 0, 30, 7.75, 0, 4.75, 8.75, 3.5, 18, 12.5, 0, 16, 0, 9.5},
		{ 0, 3.5, 4.5, 2.5, 0, 37.5, 7.75, 0, 4.75, 8.75, 3.5, 21, 15.5, 0, 16, 0, 9.5, 3.5, 4.5, 2.5, 0, 37.5, 7.75, 0, 4.75, 8.75, 3.5, 21, 15.5, 0, 16, 0, 9.5},
		{ 0, 3.5, 4.5, 2.5, 0, 45.5, 7.75, 0, 4.75, 8.75, 3.5, 21, 15.5, 0, 16, 0, 9.5, 3.5, 4.5, 2.5, 0, 45.5, 7.75, 0, 4.75, 8.75, 3.5, 21, 15.5, 0, 16, 0, 9.5},
	};
	// Offset
	int aOffset[][9] = 
	{
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE,  TRUE, FALSE,  TRUE, FALSE, FALSE, FALSE, FALSE, FALSE },
	};
		

	// 치수는 모두 중립단위계로 저장 
	int nNum = sizeof(aDBName) / sizeof(CString);
	ASSERT(sizeof(aSize)/(sizeof(double)*33) >= nNum);
	ASSERT(sizeof(aOffset)/sizeof(BOOL)*9 >= nNum);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
 
	double dSize;
	CArray<double, double>* pSize;
	CArray<BOOL, BOOL>* pOffset;
	for (int i=0; i<nNum; i++)
	{
		m_aDBNameList.Add(aDBName[i]);

		pSize = new CArray<double, double>;
		pSize->SetSize(33);
		for (int k=0; k<33; k++) 
		{
			// 중립단위계로 단위변환 후 저장 
			dSize = pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, 
																														D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_IN,
																														D_UNITSYS_BASE_LENGTH, aSize[i][k]);
			pSize->SetAt(k, dSize);
		}
		m_aSize.Add(pSize);

		pOffset = new CArray<BOOL, BOOL>;
		pOffset->SetSize(9);
		for (int k=0; k<9; k++) 
		{
			pOffset->SetAt(k, aOffset[i][k]);
		}
		m_aOffset.Add(pOffset);  
	}
}

int CCMSectItemPSCIDB::GetTypeNameList(CStringArray& aTypeList)
{
	aTypeList.RemoveAll();
	aTypeList.Add(_LS(IDS_CMD_PSC_I_NONE));
	aTypeList.Add(_LS(IDS_CMD_PSC_I_AASHTO));
	//aTypeList.Add(_LS(IDS_CMD_PSC_I_MODIFIED_AASHTO)); // AASHTO 에 포함
	aTypeList.Add(_LS(IDS_CMD_PSC_I_CALTRANS));
	aTypeList.Add(_LS(IDS_CMD_PSC_I_IADOT));
	aTypeList.Add(_LS(IDS_CMD_PSC_I_MODOT));
	aTypeList.Add(_LS(IDS_CMD_PSC_I_ODOT));
	aTypeList.Add(_LS(IDS_CMD_PSC_I_TXDOT));
	aTypeList.Add(_LS(IDS_CMD_PSC_I_VADOT));	
	aTypeList.Add(_LS(IDS_CMD_PSC_I_WIDOT));

	return aTypeList.GetSize();
}

int CCMSectItemPSCIDB::GetDBNameList(CString &strTypeName, CStringArray& aNameList)
{
	aNameList.RemoveAll();

	if(strTypeName == _LS(IDS_CMD_PSC_I_NONE))
	{
		aNameList.Add(m_aDBNameList[0]);
	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_AASHTO))
	{
		aNameList.Add(m_aDBNameList[1]);
		aNameList.Add(m_aDBNameList[2]);
		aNameList.Add(m_aDBNameList[3]);
		aNameList.Add(m_aDBNameList[4]);
		aNameList.Add(m_aDBNameList[5]);
		aNameList.Add(m_aDBNameList[6]);
		aNameList.Add(m_aDBNameList[7]);
		aNameList.Add(m_aDBNameList[8]);
		aNameList.Add(m_aDBNameList[9]);
		aNameList.Add(m_aDBNameList[10]);
		aNameList.Add(m_aDBNameList[11]);
		aNameList.Add(m_aDBNameList[12]);
	}
// 	else if(strTypeName == _LS(IDS_CMD_PSC_I_MODIFIED_AASHTO)) // AASHTO 에 포함
// 	{
// 		aNameList.Add(m_aDBNameList[7]);
// 		aNameList.Add(m_aDBNameList[8]);
// 		aNameList.Add(m_aDBNameList[9]);
// 	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_CALTRANS))
	{
		aNameList.Add(m_aDBNameList[13]);
		aNameList.Add(m_aDBNameList[14]);
		aNameList.Add(m_aDBNameList[15]);
		aNameList.Add(m_aDBNameList[16]);
		aNameList.Add(m_aDBNameList[17]);
		aNameList.Add(m_aDBNameList[18]);
	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_IADOT))
	{
		aNameList.Add(m_aDBNameList[19]);
		aNameList.Add(m_aDBNameList[20]);
		aNameList.Add(m_aDBNameList[21]);
		aNameList.Add(m_aDBNameList[22]);
	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_VADOT))
	{
		aNameList.Add(m_aDBNameList[23]);
		aNameList.Add(m_aDBNameList[24]);
		aNameList.Add(m_aDBNameList[25]);
		aNameList.Add(m_aDBNameList[26]);
		aNameList.Add(m_aDBNameList[27]);
		aNameList.Add(m_aDBNameList[28]);
		aNameList.Add(m_aDBNameList[29]);
		aNameList.Add(m_aDBNameList[30]);
		aNameList.Add(m_aDBNameList[31]);
	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_ODOT))
	{
		aNameList.Add(m_aDBNameList[32]);
		aNameList.Add(m_aDBNameList[33]);
		aNameList.Add(m_aDBNameList[34]);
		aNameList.Add(m_aDBNameList[35]);
		aNameList.Add(m_aDBNameList[36]);
		aNameList.Add(m_aDBNameList[37]);
		aNameList.Add(m_aDBNameList[38]);
	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_MODOT))
	{
		aNameList.Add(m_aDBNameList[39]);
		aNameList.Add(m_aDBNameList[40]);
		aNameList.Add(m_aDBNameList[41]);
		aNameList.Add(m_aDBNameList[42]);
		aNameList.Add(m_aDBNameList[43]);
	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_WIDOT))
	{
		aNameList.Add(m_aDBNameList[44]);
		aNameList.Add(m_aDBNameList[45]);
	}
	else if(strTypeName == _LS(IDS_CMD_PSC_I_TXDOT))
	{
		aNameList.Add(m_aDBNameList[46]);
		aNameList.Add(m_aDBNameList[47]);
		aNameList.Add(m_aDBNameList[48]);
		aNameList.Add(m_aDBNameList[49]);
		aNameList.Add(m_aDBNameList[50]);
		aNameList.Add(m_aDBNameList[51]);
		aNameList.Add(m_aDBNameList[52]);
	}
	else
	{
		ASSERT(0);
	}

	return aNameList.GetSize();
}

BOOL CCMSectItemPSCIDB::GetDBData(CString &csName, CArray<BOOL, BOOL> &aOffset, CArray<double, double> &aSize)
{
	int nNum = m_aDBNameList.GetSize();
	int i = 0;
	for (i=0; i<nNum; i++)
		if (m_aDBNameList[i] == csName) break;
	if (i == nNum) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CArray<double, double>* pSize;
	CArray<BOOL, BOOL>* pOffset;
	pSize = m_aSize.GetAt(i);
	pOffset = m_aOffset.GetAt(i);
	aSize.SetSize(33);
	aOffset.SetSize(9);
	for (int k=0; k<33; k++)
	{
		// 현재 단위계로 변환 후 넘김 
		aSize[k] = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M,
																														 D_UNITSYS_BASE_LENGTH, pSize->GetAt(k));
	}

	for (int k=0; k<9; k++)
	{
		aOffset[k] = pOffset->GetAt(k);
	}
	
	return TRUE;
}

