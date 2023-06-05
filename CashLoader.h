#pragma once
#include "cl_base.h"
#include "KKMParts.h"
#include <map>
class CashLoader : public cl_base
{
    private:
        std::map<int, int> DebitStorage = { {5,0},{10,0},{50,0},{100,0},{500,0} };
    public:
        CashLoader(cl_base* parent, std::string name);
        void CashInformation(std::string command);
        void CollectCashInfo(std::string& message);
        void MoneyInsertionHandler(std::string command);
        void MoneyInsertionConfirmation(std::string& argument);
        int DebitSum();
        void DebitSumSignal(std::string& message);
        void MoneyTakeOut(std::string& message);
        void CompleteProcessing(std::string& message);
        void ReturnMoney(std::string& message);
        void InsertionPrecheck(std::string& argument);
};

