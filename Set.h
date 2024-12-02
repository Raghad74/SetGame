#ifndef SET_H
#define SET_H
#include <string>
#include <vector>
#include <unordered_set>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <functional>
using std::string;

enum Feature{
    ERROR=0,
    ONE=1,
    TWO=2,
    THREE=3,
    SOLID=4,
    EMPTY=5,
    HALF=6,
    DALTON=7,
    CIRCLE=8,
    ZIGZAG=9,
    BLUE=10,
    GREEN=11,
    RED=12
};

class Card
{
private:
    Feature num;
    Feature filling;
    Feature color;
    Feature shape;
    cv::Rect boundingRect;
public:
    Card(/* args */)=default;
    Card(Feature num1,Feature fill,Feature col,Feature sh,cv::Rect rect):num(num1),filling(fill),color(col),shape(sh),boundingRect(rect){
    }
    ~Card()=default;
    Card(const Card& other) = default;
    Card& operator=(const Card& other) = default;
    bool operator==(const Card& other) const;
    Feature getN()const;
    Feature getF()const;
    Feature getC()const;
    Feature getS()const;
    cv::Rect getRect()const;
};

namespace std {
    template <>
    struct hash<Card> {
        std::size_t operator()(const Card& obj) const {
            int hashing_value=static_cast<int>(obj.getC())+
                              static_cast<int>(obj.getF())+
                              static_cast<int>(obj.getS())+
                              static_cast<int>(obj.getN());

            std::size_t h1 = std::hash<int>{}(hashing_value);
            return h1;
        }
    };
}

class SetGame
{
    private:
     std::vector<Card*> game;
     std::unordered_set<Card> gameHashSet;
    public:
    SetGame(/* args */)=default;
    ~SetGame()
    {
        for(int i=0;i<game.size();i++)
        {
            delete game[i];
        }
    }
    void addCard(Card* card);
    void getSet(std::vector<cv::Rect>* sol);
};








#endif