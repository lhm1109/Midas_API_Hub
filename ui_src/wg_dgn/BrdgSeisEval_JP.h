#pragma once

#include "..\wg_db\wg_db_UnitCtrl.h"

#include "HeaderPre.h"

class CDBDoc;
class CDgnProgressDlg;
class iSeismicDesignSimd;
class iSeismicDesignSird;
class iSeismicDesignSibd;
class iSeismicDesignSidp;
class iSeismicDesignSisr;
class iSeismicDesignSiet;
class iSeismicDesignSumm;

class CUnitConversion : public CCurUnitSaver
{
public:
	CUnitConversion(const int& nForc, const int& nLeng);
	virtual ~CUnitConversion();
};

class __MY_EXT_CLASS__ CBrdgSeisEval_JP
{
public:
	CBrdgSeisEval_JP();
	virtual ~CBrdgSeisEval_JP();

	BOOL DoSeisEval(CDgnProgressDlg* pProgress);

private:
	void DoMemo();
	void AddProgressStep(CDgnProgressDlg* pProgress);

private:
	iSeismicDesignSimd* m_pSeismicDesignSimd;
	iSeismicDesignSird* m_pSeismicDesignSird;
	iSeismicDesignSibd* m_pSeismicDesignSibd;
	iSeismicDesignSidp* m_pSeismicDesignSidp;
	iSeismicDesignSisr* m_pSeismicDesignSisr;
	iSeismicDesignSiet* m_pSeismicDesignSiet;
	iSeismicDesignSumm* m_pSeismicDesignSumm;
};
#include "HeaderPost.h"