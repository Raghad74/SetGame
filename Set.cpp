#include "Set.h"


bool Card::operator==(const Card& other) const
{
 return (num==other.num)&&(filling==other.filling)&&(shape==other.shape)&&(color==other.color);
}
Feature Card::getN()const
{
    return num;
}
Feature Card::getF()const
{
    return filling;
}
Feature Card::getC()const
{
    return color;
}
Feature Card::getS()const
{
    return shape;
}
cv::Rect Card::getRect()const
{
  return boundingRect;
}

//////





void SetGame::addCard(Card* card){
   game.push_back(card);
   gameHashSet.insert(*card);
}


 void SetGame::getSet(std::vector<cv::Rect>* sol){
  Feature num,shape,filling,color;
  cv::Rect dummy_rectangle(0,0,1,2);

 
  for(int i=0;i<game.size();i++)
    {
        for(int j=0;j<game.size();j++)
       {
         if(i==j)
         {
            continue;
         }
         
         if(game[i]->getN()==game[j]->getN())
         {
            num=game[i]->getN();
         }
         else{
            num=static_cast<Feature>((static_cast<int>(game[i]->getN()))^(static_cast<int>(game[j]->getN()))
                                     ^(static_cast<int>(ONE))^(static_cast<int>(TWO))^(static_cast<int>(THREE)));
         }

         if(game[i]->getS()==game[j]->getS())
         {
            shape=game[i]->getS();
         }
         else{
            shape=static_cast<Feature>((static_cast<int>(game[i]->getS()))^(static_cast<int>(game[j]->getS()))
                                     ^(static_cast<int>(DALTON))^(static_cast<int>(CIRCLE))^(static_cast<int>(ZIGZAG)));
         }

         if(game[i]->getF()==game[j]->getF())
         {
            filling=game[i]->getF();
         }
         else{
            filling=static_cast<Feature>((static_cast<int>(game[i]->getF()))^(static_cast<int>(game[j]->getF()))
                                     ^(static_cast<int>(SOLID))^(static_cast<int>(EMPTY))^(static_cast<int>(HALF)));
         }

         if(game[i]->getC()==game[j]->getC())
         {
            color=game[i]->getC();
         }
         else{
            color=static_cast<Feature>((static_cast<int>(game[i]->getC()))^(static_cast<int>(game[j]->getC()))
                                     ^(static_cast<int>(RED))^(static_cast<int>(BLUE))^(static_cast<int>(GREEN)));
         }

         Card setMaking(num,filling,color,shape,dummy_rectangle);
         auto iterator=gameHashSet.find(setMaking);
         if(iterator!=gameHashSet.end())
         {
            sol->push_back(game[i]->getRect());
            sol->push_back(game[j]->getRect());
            sol->push_back(iterator->getRect());
            std::cout<<"setMakingCard "<<iterator->getC()<<" "<<iterator->getN()<<" "<<iterator->getS()<<std::endl;
            std::cout<<"i "<<game[i]->getC()<<" "<<game[i]->getN()<<" "<<game[i]->getS()<<std::endl;
            std::cout<<"j "<<game[j]->getC()<<" "<<game[j]->getN()<<" "<<game[j]->getS()<<std::endl;


            return;
         }

         
       }

    }
    return;
}