#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#include <string>

enum ePRODUCTS{
    GOLD=0,
    LUMBER,
    FOOD,
    FABRIC,
    NUM_PRODUCTS
};

const long NUM_LEVELS_OF_CONSUMPTION=6; //Esto no me gusta...

class COwner;
struct struct_Transaction
{
    COwner* pOwnerGiver;
    COwner* pOwnerReceiver;
    double dAmount;
    ePRODUCTS eProduct;
};

class CMarket;
struct struct_Exchange
{
    COwner* pOwner;
    CMarket* pMarket;
    double dAmountOwnerToMarket;
    ePRODUCTS eProductOwnerToMarket;
    double dAmountMarketToOwner;
    ePRODUCTS eProductMarketToOwner;
};

static std::string GetNameOfProduct(ePRODUCTS eProduct)
{
    switch(eProduct)
    {
    case GOLD:
        return "GOLD";
        break;
    case LUMBER:
        return "LUMBER";
        break;
    case FOOD:
        return "FOOD";
        break;
    case FABRIC:
        return "FABRIC";
    default:
        return "UNKNOWN NAME";
    }
}


#endif // DEFINES_H_INCLUDED
