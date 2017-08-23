#ifndef OWNER_H
#define OWNER_H

#include "Defines.h"
#include <vector>
#include <string>

class COwner
{
    public:
        COwner();
        virtual ~COwner();

        void Init();
        void SetMarket(CMarket* pMarket){m_pMarket=pMarket;}

        std::vector<double> GetProductAmountsCopy(){return m_vdProductAmounts;}

        struct_Transaction MakeTransactionTo(COwner* pReceivingOwner, double dAmount, ePRODUCTS eProduct=GOLD);
        struct_Transaction MakeTransactionOfAll(COwner* pReceivingOwner,ePRODUCTS eProduct);

        double CreateProducts(ePRODUCTS eProduct, double dAmount);

        double ConsumeProducts(ePRODUCTS eProduct, double dAmount); //returns dAmountAbleToBeConsumed

        virtual double GetAmount(ePRODUCTS eProduct);

        //Market methods
        double SellProduct(ePRODUCTS eProductToSell, double dAmountToSell, ePRODUCTS eProdCurrency);
        double SellAllProduct(ePRODUCTS eProductToSell, ePRODUCTS eProdCurrency); //returns amount obtained
        double SellAllProducts(ePRODUCTS eProdCurrency);
        double GetValueOfAllProducts();

//        void FreezeStockChanges();
//        void UnfreezeStockChanges();

        //For the new way of calculating prices
        void SaveStocks();
        void RecoverSavedStocks();
        void FillStocks();
        void SetTheseStocks(std::vector<double> vdProductAmountsToBeSet);
        std::vector<double> GetStocks(){return m_vdProductAmounts;}
        //std::string GetNameOfProduct(ePRODUCTS eProduct);
        std::string GetStocksInfoText();


        static double DifferenceBetweenStocks(std::vector<double> vStocks1, std::vector<double> vStocks2);

    protected:

        virtual void AddProducts(ePRODUCTS eProduct, double dAmount);
        virtual void SubstractProducts(ePRODUCTS eProduct, double dAmount);

//        bool FrozenStocks(){return m_bFreezeStockChanges;}

    protected:

        std::vector<double> m_vdProductAmounts;
        std::vector<double> m_vdSavedProductAmounts;
//        bool m_bFreezeStockChanges;
        CMarket* m_pMarket;

    private:
};

#endif // OWNER_H
