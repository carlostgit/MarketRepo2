#include "Owner.h"
#include "Market.h"
#include <assert.h>
#include <sstream>
#include <math.h>

COwner::COwner()
{
    //ctor

}

COwner::~COwner()
{
    //dtor
}

void COwner::Init()
{
    m_vdProductAmounts.clear();
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdProductAmounts.push_back(0.0);
    }

    m_vdSavedProductAmounts.clear();
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdSavedProductAmounts.push_back(0.0);
    }

}

struct_Transaction COwner::MakeTransactionTo(COwner* pReceivingOwner, double dAmount, ePRODUCTS eProduct)
{
    struct_Transaction strTransaction;
    strTransaction.pOwnerGiver=this;
    strTransaction.pOwnerReceiver=pReceivingOwner;
    strTransaction.eProduct=eProduct;

    double dAmountOfRemainingProduct=GetAmount(eProduct);

    if(dAmountOfRemainingProduct<dAmount)
    {
        strTransaction.dAmount=dAmountOfRemainingProduct;
        pReceivingOwner->AddProducts(eProduct, dAmountOfRemainingProduct);
        this->SubstractProducts(eProduct, dAmountOfRemainingProduct);
    }
    else
    {
        strTransaction.dAmount=dAmount;
        pReceivingOwner->AddProducts(eProduct, dAmount);
        this->SubstractProducts(eProduct, dAmount);
    }

    return strTransaction;
}

struct_Transaction COwner::MakeTransactionOfAll(COwner* pReceivingOwner,ePRODUCTS eProduct)
{
    return COwner::MakeTransactionTo(pReceivingOwner, this->GetAmount(eProduct), eProduct);
}

double COwner::CreateProducts(ePRODUCTS eProduct, double dAmount)
{
    double dAmountReallyCreated=0.0;
    if (dAmount>=0)
    {
        dAmountReallyCreated=dAmount;
    }

    AddProducts(eProduct, dAmountReallyCreated);
    return dAmountReallyCreated;
}

double COwner::ConsumeProducts(ePRODUCTS eProduct, double dAmount)
{
    double dAmountReallyConsumed=0.0;

    if (dAmount<0)
    {
        dAmountReallyConsumed=0.0;
    }
    else if ((dAmount<=GetAmount(eProduct)))// && (dAmount>=0))
    {
        dAmountReallyConsumed=dAmount;
    }
    else //dAmount>GetAmount(eProduct)
    {
        dAmountReallyConsumed=GetAmount(eProduct);
    }

    SubstractProducts(eProduct, dAmountReallyConsumed);
    return dAmountReallyConsumed;
}

void COwner::AddProducts(ePRODUCTS eProduct, double dAmount)
{
//    if(false==FrozenStocks())
//    {
    assert(dAmount>=0);
    m_vdProductAmounts[eProduct]=GetAmount(eProduct)+dAmount;
//    }
}

void COwner::SubstractProducts(ePRODUCTS eProduct, double dAmount)
{
//    if(false==FrozenStocks())
//    {
    assert(dAmount>=0);
    assert(dAmount<=GetAmount(eProduct));
    m_vdProductAmounts[eProduct]=GetAmount(eProduct)-dAmount;
//    }
}

double COwner::GetAmount(ePRODUCTS eProduct)
{
    assert(m_vdProductAmounts[eProduct]>=0);
    return m_vdProductAmounts[eProduct];
}
double COwner::SellAllProduct(ePRODUCTS eProductToSell, ePRODUCTS eProdCurrency)
{
    struct_Exchange strExchange = m_pMarket->SellToMarket(this, eProductToSell, this->GetAmount(eProductToSell), eProdCurrency);
    return strExchange.dAmountMarketToOwner;
}

double COwner::SellAllProducts(ePRODUCTS eProdCurrency)
{
    double dTotalAmountObtained=0.0;
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        ePRODUCTS eProd=static_cast<ePRODUCTS>(nProduct);
        dTotalAmountObtained= dTotalAmountObtained+SellAllProduct(eProd, eProdCurrency);
    }
    return dTotalAmountObtained;
}

double COwner::GetValueOfAllProducts()
{
    double dValueOfAllProducts=0.0;
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        ePRODUCTS eProd=static_cast<ePRODUCTS>(nProduct);
        dValueOfAllProducts=dValueOfAllProducts + GetAmount(eProd)*m_pMarket->GetPrice(eProd);
    }
    return dValueOfAllProducts;
}

double COwner::SellProduct(ePRODUCTS eProductToSell, double dAmountToSell, ePRODUCTS eProdCurrency)
{
    struct_Exchange strExchange = m_pMarket->SellToMarket(this, eProductToSell, dAmountToSell, eProdCurrency);
    return strExchange.dAmountMarketToOwner;
}

//void COwner::FreezeStockChanges()
//{
//    m_bFreezeStockChanges=true;
//}
//void COwner::UnfreezeStockChanges()
//{
//    m_bFreezeStockChanges=false;
//}

void COwner::SaveStocks()
{
    assert(m_vdProductAmounts.size()==NUM_PRODUCTS);
    assert(m_vdSavedProductAmounts.size()==NUM_PRODUCTS);
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdSavedProductAmounts[nProduct]=m_vdProductAmounts[nProduct];
    }
}

void COwner::RecoverSavedStocks()
{
    assert(m_vdProductAmounts.size()==NUM_PRODUCTS);
    assert(m_vdSavedProductAmounts.size()==NUM_PRODUCTS);
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdProductAmounts[nProduct]=m_vdSavedProductAmounts[nProduct];
    }

}

void COwner::FillStocks()
{
    assert(m_vdProductAmounts.size()==NUM_PRODUCTS);
    assert(m_vdSavedProductAmounts.size()==NUM_PRODUCTS);
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdProductAmounts[nProduct]=1000.0;
    }
}

void COwner::SetTheseStocks(std::vector<double> m_vdProductAmountsToBeSet)
{
    assert(m_vdProductAmounts.size()==NUM_PRODUCTS);
    assert(m_vdProductAmountsToBeSet.size()==NUM_PRODUCTS);
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        m_vdProductAmounts[nProduct]=m_vdProductAmountsToBeSet[nProduct];
    }
}

std::string COwner::GetStocksInfoText()
{
    std::string strStocksInfo;
    strStocksInfo=strStocksInfo+"\n";
    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        double dProductAmount = m_vdProductAmounts[nProduct];

        std::stringstream strStream;
        strStream.precision(2);
        strStream << std::fixed << dProductAmount ;
        std::string strProductAmount = strStream.str();

        std::string strProductName = GetNameOfProduct(static_cast<ePRODUCTS>(nProduct));
        std::string strNewLine = "Product: " +strProductName+ ", \tAmount: "+strProductAmount+" \n";
        strStocksInfo=strStocksInfo+strNewLine;

    }
    //strStocksInfo=strStocksInfo+"/n";
    return strStocksInfo;
}

double COwner::DifferenceBetweenStocks(std::vector<double> vStocks1, std::vector<double> vStocks2)
{
    double dAccumulatedDifference=0.0;

    for (int nProduct=0;nProduct<NUM_PRODUCTS; nProduct++)
    {
        dAccumulatedDifference=dAccumulatedDifference+fabs(vStocks1[nProduct]-vStocks2[nProduct]);
    }

    return dAccumulatedDifference;
}

