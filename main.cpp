#include <iostream>
#include "Scene.h"
#include "Market.h"
#include "Factory.h"

//#include "koolplot.h"
#include "graphics.h"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;

 //   initwindow(400,300);
 //   line(0,0, getmaxx(),getmaxy());
 //   getch();
    //closegraph();


    //Plotdata x(-3.0, 3.0), y = sin(x) - 0.5*x;

    //Plotdata x2(-4.0, 4.0), y2 = sin(x) - 0.6*x;
    //addMark(x, y,1.0, 0.3);
    //plot(x, y);

    CScene myScene;
    cout << "myScene created " <<endl;

    myScene.CreateTestScene();

    CMarket* myMarket=myScene.GetMarket();

    std::vector<CFactory*> vFactories = myScene.GetFactories();
    //General loop
    long nCycleCounter=0;

    cout << "Before main loop: " <<endl;
    bool bExitApplication=false;
    while (false==bExitApplication)
    {

//        cout << "Cycle number: " << nCycleCounter <<endl;
//        cout << "GOLD Price:  " << myMarket->GetPrice(GOLD) << "  LUMBER Price:  " << myMarket->GetPrice(LUMBER) << "  FOOD Price:  " << myMarket->GetPrice(FOOD) << "  FABRIC Price:  " << myMarket->GetPrice(FABRIC) <<  endl;
//        cout << "GOLD Amount: " << myMarket->GetAmount(GOLD) <<"  LUMBER Amount: " << myMarket->GetAmount(LUMBER) <<"  FOOD Amount: " << myMarket->GetAmount(FOOD) <<"  FABRIC Amount: " << myMarket->GetAmount(FABRIC) << endl;
//
//        for (int nFact=0;nFact<vFactories.size();nFact++)
//        {
//            cout<< "Producing: " << vFactories[nFact]->GetLogLastProducedProduct() << " ";
//            cout<< "Gold Stock: " << vFactories[nFact]->GetStock(GOLD) << " ";
//
//        }
//        cout << endl;


        myScene.SetCycle(nCycleCounter);
        cout<< "Going to call SimulateStepsToEstimatePrices " << endl;
        myScene.SimulateStepsToEstimatePrices();

        cout<< "Going to call MakeAStep " << endl;
        myScene.MakeAStep();

//      GOLD=0,
//      LUMBER,
//      FOOD,
//      FABRIC,


        if (nCycleCounter>10)
        {
            bExitApplication=true;
        }
        nCycleCounter++;
    }

    return 0;
}
