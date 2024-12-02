#include "helper.h"

//ranges of colors
cv::Scalar blueLower(100, 150, 80), blueUpper(130, 255, 255);
cv::Scalar yellowLower(20, 150, 150), yellowUpper(30, 255, 255);
cv::Scalar greenLower(40, 50, 50), greenUpper(80, 255, 255);
cv::Scalar redLower1(0, 150, 150), redUpper1(10, 255, 255);
cv::Scalar redLower2(160, 150, 150), redUpper2(180, 255, 255);
cv::Scalar purpleLower(130, 120, 50), purpleUpper(155, 255, 180);
cv::Scalar pinkLower(160, 100, 100), pinkUpper(180, 255, 255);




std::vector<std::vector<cv::Point>>* getContours(cv::Mat& image,int filterVal)
{
  cv::Mat imBlur,imGray,imCanny,imDil,imEro;

  //preprocessing
  cv::cvtColor(image,imGray,cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(imGray,imBlur,cv::Size(3,3),0);
  cv::Canny(imBlur,imCanny,50,150);
  cv::Mat kernel=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
  cv::dilate(imCanny,imDil,kernel);
 
 //cv::imshow("dil",imDil);
 //cv::waitKey(0);
  std::vector<std::vector<cv::Point>> raw_contours,contours;
  cv::findContours(imDil,raw_contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
  for(int i=0;i<raw_contours.size();i++)
   {
      //std::cout<<"cA "<<cv::contourArea((raw_contours)[i])<<std::endl;
      if(cv::contourArea(raw_contours[i])>filterVal)
      {
       contours.push_back(raw_contours[i]);
      }
   }
  std::vector<std::vector<cv::Point>>* conPoly=new std::vector<std::vector<cv::Point>>(contours.size());
  float peri;
  for(int i=0;i<contours.size();i++)
  {
    peri=cv::arcLength(contours[i],true);
    cv::approxPolyDP(contours[i],(*conPoly)[i],0.02*peri,true); 
  }

  return conPoly;
}

std::vector<cv::Rect>* getCards(cv::Mat& img)
{
  cv::Mat filtered;
  cv::bilateralFilter(img, filtered, 9, 200, 200); 
  std::vector<std::vector<cv::Point>>* contours=getContours(filtered,10000);
  
  std::vector<cv::Rect>* boundRect=new std::vector<cv::Rect>(contours->size());
  int margin=11;
  for(int i=0;i<contours->size();i++)
  {
    (*boundRect)[i]=cv::boundingRect((*contours)[i]);
    (*boundRect)[i].x += margin;
    (*boundRect)[i].y += margin;
    (*boundRect)[i].width = std::max(1, (*boundRect)[i].width - 2 * margin);
    (*boundRect)[i].height = std::max(1, (*boundRect)[i].height - 2 * margin);
  }
  delete contours;
  return boundRect;

}

Feature getShape(cv::Mat& card)
{
  std::vector<std::vector<cv::Point>>* contours=getContours(card,1000);
  if(contours==nullptr)
  {
    delete contours;
    return ERROR;
  }
  if(contours->size()==0)
  {
    delete contours;
    return ERROR;
  }
  std::vector<cv::Point> shape=(*contours)[0];

  int num_points=shape.size();
  double perimeter = cv::arcLength(shape, true);
  double area=cv::contourArea(shape);
  double circularity = 4 * CV_PI * area / (perimeter * perimeter);
  delete contours;
 
 
 
  if(circularity>0.7)
  {
    return CIRCLE;
  }
  if(num_points<7)
  {
    return DALTON;
  }
  return ZIGZAG;
  
  
  
}

Feature getColor(cv::Mat& card,double* colored_pixeled_num)
{
  
  cv::Mat imHSV,blue,purple,green,red,red2,pink,yellow;
  cv::cvtColor(card,imHSV,cv::COLOR_BGR2HSV);
  cv::inRange(imHSV,blueLower,blueUpper,blue);
  cv::inRange(imHSV,purpleLower,purpleUpper,purple);
  cv::inRange(imHSV,greenLower,greenUpper,green);
  cv::inRange(imHSV,redLower1,redUpper1,red);
  cv::inRange(imHSV,redLower2,redUpper2,red2);
  cv::inRange(imHSV,pinkLower,pinkUpper,pink);
  cv::inRange(imHSV,yellowLower,yellowUpper,yellow);

  const int blue_count=cv::countNonZero(blue|purple);
  const int green_count=cv::countNonZero(green|yellow);
  const int red_count=cv::countNonZero(red|red2|pink);

  int max_color=std::max(blue_count,std::max(green_count,red_count));
  *colored_pixeled_num=(double)max_color;
  if(max_color==blue_count) return BLUE;
  if(max_color==green_count) return GREEN;
  if(max_color==red_count) return RED;
 return ERROR;
}

Feature getFilling(cv::Mat& item,double colored_pixels_num)
{
  cv::Mat imBlur,imGray,imCanny,imDil,imErode;

  //preprocessing
  cv::cvtColor(item,imGray,cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(imGray,imBlur,cv::Size(3,3),3,0);
  cv::Canny(imBlur,imCanny,50,150);
  cv::Mat kernel=cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(4,4));
  cv::dilate(imCanny,imDil,kernel);
  cv::erode(imDil,imErode,kernel);
  cv::imshow("fill",imDil);
  cv::waitKey(0);
  
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(imDil,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
  cv::drawContours(imDil,contours,-1,cv::Scalar(255,255,255),9);
  cv::imshow("fill",imDil);
  cv::waitKey(0);
  double coloredRatio=colored_pixels_num/cv::contourArea(contours[0],false);

  cv::findContours(imDil, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
/*
  int lastLevelContourCount = 0;
    for (size_t i = 0; i < hierarchy.size(); ++i) {
        // Check if contour has no child
        if (hierarchy[i][2] == -1) { // 'child' == -1 means no children, so it's a last-level contour
            lastLevelContourCount++;
        }
    }

    std::cout << "Number of contours on the last level: " << lastLevelContourCount << std::endl;
*/
  std::cout<<"colored "<<cv::contourArea(contours[0],false)<<std::endl;
  
   if(contours.size()>7)
   {
    return HALF;
   }
   if(coloredRatio<=0.7)
   {
    return EMPTY;
   }
   return SOLID;
  

}