#include "Market.h"
#include "Scene.h"

#include <iostream>
#include <assert.h>
#include <iomanip>      // std::setprecision
#include <math.h>
#include <sstream>



CMarket::CMarket(CScene* pScene):m_pScene(pScene)//:dParamMinAmount(100)
{
    std::string strFileName="MarketDebug";
    strFileName=strFileName+".txt";
    m_DebugFile.open (strFileName.c_str());
    m_DebugFile << "Writing in MarketDebug file.\n";
    m_DebugFile.flush();
    //ctor
    Init();
}

CMarket::~CMarket()
{
    m_DebugFile.close();
    //dtor
}

void CMarket::Init()
{
    COwner::Init();

    m_vdProductAmounts.clear();
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdProductAmounts.push_back(1000.0);
    }
    m_vdAmountsForPricesCalc.clear();
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdAmountsForPricesCalc.push_back(1000.0);
    }


}

double CMarket::GetPrice(ePRODUCTS eProduct, ePRODUCTS ePCurrency)
{

//    //std::cout << "AmountForPriceCalc: " << GetAmountForPriceCalcOnly(eProduct) << std::endl;
//    //std::cout << "AmountOfCurrency: " << GetAmountForPriceCalcOnly(ePCurrency) << std::endl;

    double dRateProductsPerCoin=GetAmountForPriceCalcOnly(eProduct)/GetAmountForPriceCalcOnly(ePCurrency);

//    std::cout << "RateProductsPerCoin: " << dRateProductsPerCoin << std::endl;

    //y = 1/x^2
    return 1.0/((dRateProductsPerCoin)*(dRateProductsPerCoin));

}

//new for refactoring
struct_Exchange CMarket::SellToMarket(COwner* pOwner, ePRODUCTS eProductToSell, double dAmountToSell, ePRODUCTS eProductCurrency)
{
    //std::cout << "Entering CMarket::SellToMarket " <<std::endl;

    double dMaxAmountOwnerToMarket=pOwner->GetAmount(eProductToSell);
    //std::cout << "Max Amount Owner To Market " << dMaxAmountOwnerToMarket <<std::endl;

    double dMaxAmountMarketToOwner=this->GetAmount(eProductCurrency);
    //std::cout << "Max Amount Market To Owner " << dMaxAmountMarketToOwner <<std::endl;

    double dRealAmountToSellToMarket=dAmountToSell;
    if (dAmountToSell>dMaxAmountOwnerToMarket) //El pOwner no tiene suficientes existencias
    {
        dRealAmountToSellToMarket=dMaxAmountOwnerToMarket;
    }
    //std::cout
    //std::cout << "Real Amount Sell To Market Limit 1 " << dRealAmountToSellToMarket <<std::endl;

    double dPrice = GetPrice(eProductToSell,eProductCurrency); //Ahora voy a ver si el mercado puede pagarlo
    double dPriceOfRealAmountToSellToMarket=dRealAmountToSellToMarket*dPrice;
    if (dPriceOfRealAmountToSellToMarket>dMaxAmountMarketToOwner) //El mercado no tiene existencias
    {
        dRealAmountToSellToMarket=dMaxAmountMarketToOwner/dPrice;
    }
    //std::cout << "Real Amount Sell To Market Limit 2 " << dRealAmountToSellToMarket <<std::endl;

    struct_Transaction strTransactionOwnerToMarket = pOwner->MakeTransactionTo(this,dRealAmountToSellToMarket,eProductToSell);
    double dAmountMarketToOwner=strTransactionOwnerToMarket.dAmount*dPrice;
    assert(dAmountMarketToOwner<=dMaxAmountMarketToOwner); //si salta esto, algo he programado mal
    struct_Transaction strTransactionMarketToOwner = this->MakeTransactionTo(pOwner,dAmountMarketToOwner,eProductCurrency);

    //std::cout << std::setprecision(30) << "strTransactionMarketToOwner.dAmount " << strTransactionMarketToOwner.dAmount <<std::endl;
//std::cout    //std::cout << std::setprecision(30) << "strTransactionOwnerToMarket.dAmount*dPrice " << strTransactionOwnerToMarket.dAmount*dPrice <<std::endl;
    //std::cout << std::setprecision(30) << "dPrice " << dPrice <<std::endl;
    //assert (strTransactionMarketToOwner.dAmount==strTransactionOwnerToMarket.dAmount*dPrice);
    if(strTransactionMarketToOwner.dAmount!=strTransactionOwnerToMarket.dAmount*dPrice)
    {
        double dError=strTransactionOwnerToMarket.dAmount*dPrice-strTransactionMarketToOwner.dAmount;
        double dMinPrecision=1.e-10;
        assert(fabs(dError)<dMinPrecision);
    }
    //Todo: Usar constructor de struct_Exchange
    struct_Exchange strExchange;
    strExchange.dAmountMarketToOwner=strTransactionMarketToOwner.dAmount;
    strExchange.dAmountOwnerToMarket=strTransactionOwnerToMarket.dAmount;
    strExchange.eProductMarketToOwner=strTransactionMarketToOwner.eProduct;
    strExchange.eProductOwnerToMarket=strTransactionOwnerToMarket.eProduct;
    strExchange.pMarket=this;
    strExchange.pOwner=pOwner;

    //std::cout << "Exiting  CMarket::SellToMarket " <<std::endl;

    return strExchange;
}

struct_Exchange CMarket::BuyFromMarket(COwner* pOwner, ePRODUCTS eProductToBuy, double dAmountToBuy, ePRODUCTS eProductCurrency)
{

    //std::cout << "Entering CMarket::BuyFromMarket " <<std::endl;

    double dMaxAmountOwnerToMarket=pOwner->GetAmount(eProductCurrency);
    double dMaxAmountMarketToOwner=this->GetAmount(eProductToBuy);

    //std::cout << "Max Amount Market To //std::coutOwner " << dMaxAmountMarketToOwner <<std::endl;

    double dRealAmountToBuyFromMarket=dAmountToBuy;
    if (dAmountToBuy>dMaxAmountMarketToOwner) //El Market no tiene suficientes existencias
    {
        dRealAmountToBuyFromMarket=dMaxAmountOwnerToMarket;
    }

    //std::cout << "dRealAmountToBuyFromMarket 1 " << dRealAmountToBuyFromMarket <<std::endl;

    double dPrice = GetPrice(eProductToBuy,eProductCurrency);
    double dPriceOfRealAmountBuyFromMarket=dPrice*dRealAmountToBuyFromMarket;
    if(dMaxAmountOwnerToMarket<dPriceOfRealAmountBuyFromMarket)
    {
        dRealAmountToBuyFromMarket=dMaxAmountOwnerToMarket/dPrice;
    }
    //std::cout << "dRealAmountToBuyFromMarket 2 " << dRealAmountToBuyFromMarket <<std::endl;
    double dRealAmountToPayToMarket=dRealAmountToBuyFromMarket*dPrice;

    //std::cout << "dRealAmountToPayToMarket " << dRealAmountToPayToMarket <<std::endl;
    struct_Transaction strTransactionOwnerToMarket = pOwner->MakeTransactionTo(this,dRealAmountToPayToMarket,eProductCurrency);

    assert(strTransactionOwnerToMarket.dAmount<=dMaxAmountOwnerToMarket); //si salta esto, algo he programado mal
    struct_Transaction strTransactionMarketToOwner = this->MakeTransactionTo(pOwner,dRealAmountToBuyFromMarket,eProductToBuy);

    //std::cout << "strTransactionMarketToOwner.dAmount " << strTransactionMarketToOwner.dAmount <<std::endl;
    //std::cout << "strTransactionOwnerToMarket.dAmount " << strTransactionOwnerToMarket.dAmount <<std::endl;
    //std::cout << "dPrice " << dPrice <<std::endl;
    //std::cout << "strTransactionOwnerToMarket.dAmount/dPrice " << strTransactionOwnerToMarket.dAmount/dPrice <<std::endl;

    //assert (strTransactionMarketToOwner.dAmount==strTransactionOwnerToMarket.dAmount/dPrice);
    if(strTransactionMarketToOwner.dAmount!=strTransactionOwnerToMarket.dAmount/dPrice)
    {
        double dError=strTransactionOwnerToMarket.dAmount/dPrice-strTransactionMarketToOwner.dAmount;
        double dMinPrecision=1.e-7;
        assert(fabs(dError)<dMinPrecision);
    }


    //Todo: Usar constructor de struct_Exchange
    struct_Exchange strExchange;
    strExchange.dAmountMarketToOwner=strTransactionMarketToOwner.dAmount;
    strExchange.dAmountOwnerToMarket=strTransactionOwnerToMarket.dAmount;
    strExchange.eProductMarketToOwner=strTransactionMarketToOwner.eProduct;
    strExchange.eProductOwnerToMarket=strTransactionOwnerToMarket.eProduct;
    strExchange.pMarket=this;
    strExchange.pOwner=pOwner;

    //std::cout << "Exiting  CMarket::BuyFromMarket " <<std::endl;

    return strExchange;
}

//StimatePrices
void CMarket::AddProducts(ePRODUCTS eProduct, double dAmount)
{
//    if(FrozenStocks())
//    {
//        assert(dAmount>=0);
//        m_vdAmountsForPricesCalc[eProduct]=GetAmount(eProduct)+dAmount;
//    }
//    else
//    {
    COwner::AddProducts(eProduct,dAmount);
//    }
}

void CMarket::SubstractProducts(ePRODUCTS eProduct, double dAmount)
{
    COwner::SubstractProducts(eProduct,dAmount);
}
//void CMarket::DuplicateProducts()
//{
//    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
//    {
//        m_vdAmountsForPricesCalc[nProduct]=m_vdAmountsForPricesCalc[nProduct]*2;
//    }
//}

double CMarket::GetAmount(ePRODUCTS eProduct)
{
    return COwner::GetAmount(eProduct);
}

double CMarket::GetAmountForPriceCalcOnly(ePRODUCTS eProduct)
{
    assert(m_vdAmountsForPricesCalc[eProduct]>=0);
    return m_vdAmountsForPricesCalc[eProduct];
}

void CMarket::RecoverStocksForPrices()
{
    SetTheseStocks(m_vdAmountsForPricesCalc);
}

void CMarket::SetStocksForPrices(std::vector<double> vdProductAmountsToBeSet)
{

    assert(m_vdAmountsForPricesCalc.size()==NUM_PRODUCTS);
    assert(vdProductAmountsToBeSet.size()==NUM_PRODUCTS);
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdAmountsForPricesCalc[nProduct]=vdProductAmountsToBeSet[nProduct];
    }

}

std::string CMarket::GetPricesInfoText()
{
    std::string strStocksInfo;
    strStocksInfo=strStocksInfo+"\n";
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        double dPrice = GetPrice(static_cast<ePRODUCTS>(nProduct));

        std::stringstream strStream;
        strStream.precision(2);
        strStream << std::fixed << dPrice;
        std::string strProductPrice = strStream.str();

        std::string strProductName = GetNameOfProduct(static_cast<ePRODUCTS>(nProduct));
        std::string strNewLine = "Product: " +strProductName+ ", \tPrice: "+strProductPrice+" \n";
        strStocksInfo=strStocksInfo+strNewLine;

    }
    //strStocksInfo=strStocksInfo+"/n";
    return strStocksInfo;
}


