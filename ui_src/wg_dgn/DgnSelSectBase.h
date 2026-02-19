// DgnDlgSelectBase.h: interface for the CDgnDlgSelectBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNSELSECTBASE_H__C697DCA9_8E72_4042_8C65_638221EAA515__INCLUDED_)
#define AFX_DGNSELSECTBASE_H__C697DCA9_8E72_4042_8C65_638221EAA515__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnSectGridWnd.h"
#include "..\wg_db\DB_ST_DT.h"

class CDBDoc;

struct T_SECT_D;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSelSectBase
{
public:
	CDgnSelSectBase();
	virtual ~CDgnSelSectBase();

	CDBDoc* m_pDoc;
	CMap<int,int,T_RCHK_K,T_RCHK_K> m_arKey;
	CMap<int,int,int,int> m_arElemData;   

	int m_iMode; //0: CHK,  1:DGN,  2:SRC-Column,  3:Beam,  4:Colm,   5:Brace,   6:SRC-Beam

	CDgnSectGridWnd m_Grid;

	void InitSectGrid(CMap<int,int,int,int>& arElemData, int Mode=0);
	void ResetSectGridData(BOOL bKeepChk = FALSE);
	BOOL IsExistEnabledSect();
	BOOL SelectSection();
	BOOL GetCurKeyData(T_RCHK_K& Key,T_SECT_D& Data);
	BOOL SelectAll();

protected:
	void WriteSectGridTitle();
	void UpdateRchkKeyArray();
	void CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult);
	BOOL IsValidSection();

};
#include "HeaderPost.h"

#endif // !defined(AFX_DGNSELSECTBASE_H__C697DCA9_8E72_4042_8C65_638221EAA515__INCLUDED_)
