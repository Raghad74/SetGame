#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Set.h"
#include "helper.h"


std::string translate(Feature f)
{
  switch (f)
  {
  case ONE:
    return "1";
    break;
  case TWO:
    return "2";
    break;
  case THREE:
    return "3";
    break;
  case SOLID:
    return "solid";
    break;
  case EMPTY:
    return "empty";
    break;
  case HALF:
    return "half";
    break;
  case DALTON:
    return "dalton";
    break;
  case CIRCLE:
    return "circle";
    break;
  case ZIGZAG:
    return "zigzag";
    break;
  case GREEN:
    return "green";
    break;
  case BLUE:
    return "blue";
    break;
  case RED:
    return "red";
    break;
  
  default:
    break;
  }
  return "";
}
Card* analyzeCard(cv::Mat& card,cv::Rect& boundingRect){
  Feature num,color,filling,shape;
  std::vector<std::vector<cv::Point>>* contours=getContours(card,1000);
  num=static_cast<Feature>(contours->size());
  if(contours->size()==0)
  {
    delete contours;
    return nullptr;
  }
  cv::Rect item_rect=cv::boundingRect((*contours)[0]);
  cv::Mat item=card(item_rect);
  cv::Mat filteredItem;
  cv::bilateralFilter(item, filteredItem, 5, 150, 150); 
  cv::imshow("item cut",filteredItem);
  cv::waitKey(0);
  double colored_pixles_num;
  color=getColor(filteredItem,&colored_pixles_num);
  shape=getShape(filteredItem);
  filling=getFilling(filteredItem,colored_pixles_num);

  Card* setCard=new Card(num,filling,color,shape,boundingRect);
  std::cout<<translate(num)<<std::endl;
  std::cout<<translate(color)<<std::endl;
  std::cout<<translate(shape)<<std::endl;
  std::cout<<translate(filling)<<std::endl;
  return setCard;
  

}

//sets/setBrown2.jpeg
int main()
{
  std::string imPath;
  std::cout<<"Enter image Path"<<std::endl;
  std::cin>>imPath;
  cv::Mat image=cv::imread(imPath);
  cv::resize(image, image, cv::Size(1500, static_cast<int>(image.rows * 1500.0 / image.cols))); 
  cv::Mat copy=image;
  std::vector<cv::Rect>* rects=getCards(image);
  SetGame setGame;
  for(int i=0;i<rects->size();i++)
  {
    std::cout<<"card"<<i<<std::endl;
    Card* card=analyzeCard(image((*rects)[i]),(*rects)[i]);
    //cv::rectangle(copy, (*rects)[i], cv::Scalar(0, 0, 255), 3);

    if(card!=nullptr)
    {
      setGame.addCard(card);
    }
    
  }

  std::vector<cv::Rect>* solution=new std::vector<cv::Rect>();
  setGame.getSet(solution);
  for(int i=0;i<solution->size();i++)
  {
    cv::rectangle(image, (*solution)[i], cv::Scalar(0, 255, 0), 2);
  }
  delete solution;
  cv::resize(image, image, cv::Size(500, static_cast<int>(image.rows * 500.0 / image.cols))); 
  cv::imshow("set",image);
  cv::waitKey(0);
  return 0;
}