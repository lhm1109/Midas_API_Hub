#pragma once

class CSeisEvalTextOut;
class CSeisEvalTextOutMgr
{
public:
    CSeisEvalTextOutMgr(void);
    virtual ~CSeisEvalTextOutMgr(void);

public:
    bool PrintSeisEval1stCon(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);
    bool PrintSeisEval1stStl(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);
    bool PrintSeisEval1stMas(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);

    bool PrintSeisEvalUphgCon(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);
    bool PrintSeisEvalUphgStl(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);
    bool PrintSeisEvalUphgMas(const T_ELEM_K_LIST& aElemK, T_LCOM_K LcomK);

private:
    CSeisEvalTextOut* m_pTextOut;


};

