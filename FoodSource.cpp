#include "FoodSource.h"
#include "Game.h"


FoodSource::FoodSource() :
    startingRadius(6.0f), remainingRadiusToGrow(startingRadius),
    GameObject(
        rand() % (TheGame::Instance()->getWindowWidth()-30) +15,//x
        rand() % (TheGame::Instance()->getWindowHeight()-30) +15,//y
        startingRadius,//width
        0,//height
        rand() % 255,//r
        rand() % 255,//g
        rand() % 255,//b
        255 //a
               )
{
  
}

void FoodSource::draw()
{
  filledCircleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width, 0, 0, 0, colourA);
  filledCircleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width-1, colourR, colourG, colourB, colourA);

}

void FoodSource::update()
{
  if (remainingRadiusToGrow > 0)
  {
    remainingRadiusToGrow -= startingRadius/100;
    GameObject::width += startingRadius/100;
  }
}

void FoodSource::clean()
{
  
}


void FoodSource::consumedByMicrobe()
{
  //reduce the size of the foodSource
  GameObject::width -= 0.05f;

  //if the foodSource has been entirely consumed
  if (GameObject::width <= 0.3f)
  {
    respawn();
  }
}

void FoodSource::respawn()
{
  position.setX(rand() % (TheGame::Instance()->getWindowWidth()-30) +15);//x
  position.setY(rand() % (TheGame::Instance()->getWindowHeight()-30) +15);//y

  //set the width to 0, and reset the remainingRadiusToGrow so the fod will grow to its default size
  width = 0;//width
  remainingRadiusToGrow = startingRadius;

  colourR = rand() % 255;//r
  colourG = rand() % 255;//g
  colourB = rand() % 255;//b
      }
