#ifndef MARKET_H
#define MARKET_H

#include <vector>

#include "Defines.h"
#include "Owner.h"
#include <iostream>
#include <fstream>

class CScene;

class CMarket: public COwner
{
    public:
        CMarket(CScene* pScene);
        virtual ~CMarket();
    public:

        void Init();

        double GetPrice(ePRODUCTS eProduct, ePRODUCTS ePCurrency=GOLD);

        //double GetAmount(ePRODUCTS eProduct);
        virtual double GetAmount(ePRODUCTS eProduct);
        virtual double GetAmountForPriceCalcOnly(ePRODUCTS eProduct);
//        double GetPrice(ePRODUCTS eProduct); //SellsGold

//        void AddProduct(ePRODUCTS eProduct, double dAmount);
//        void SubstractProduct(ePRODUCTS eProduct, double dAmount);

        //double ExchangeProduct(ePRODUCTS ePToSell, double dAmountToSell,ePRODUCTS ePToBuy); //returns dBuyingAmount
        //double SellProducts(ePRODUCTS ePToSell, double dAmountToSell); // BuysGold
        //double BuyProducts(ePRODUCTS ePToBuy, double dAmountOfGoldToSpend); // SellsGold

//        void DuplicateProducts();

        //info methods


        //new for refactoring
        struct_Exchange SellToMarket(COwner* pOwner, ePRODUCTS eProductToSell, double dAmountToSell, ePRODUCTS eProductCurrency=GOLD);
        struct_Exchange BuyFromMarket(COwner* pOwner, ePRODUCTS eProductToBuy, double dAmountToBuy, ePRODUCTS eProductCurrency=GOLD);

        //void SetCalculatingPricesMode();
        //When calculatingMode, Owners don't see their stocks affected
        //void CalculatePrices();
        //void

        void RecoverStocksForPrices();
        void SetStocksForPrices(std::vector<double> vdProductAmountsToBeSet);

        std::string GetPricesInfoText();

    protected:

        virtual void AddProducts(ePRODUCTS eProduct, double dAmount);
        virtual void SubstractProducts(ePRODUCTS eProduct, double dAmount);

        //Resources
        std::vector<double> m_vdAmountsForPricesCalc;
        //const double dParamMinAmount;

        //bool m_bCalculatingPricesMode; //todo
        //std::vector<double> m_vdCalculatingPricesProductAmounts;
        //
        //m_vFactories
        //m_vPersons

        //Debug
        std::ofstream m_DebugFile;

    private:
        CScene* m_pScene;
};

#endif // MARKET_H
