#include "Microbe.h"
#include <limits.h>


Microbe::Microbe() :
    GameObject(
        rand() % (TheGame::Instance()->getWindowWidth()-30) + 15, //x
        rand() % (TheGame::Instance()->getWindowHeight()-30) + 15,//y
        15.0f,//width (used as the radius)
        0,    //height (not used)
        rand() % 225 + 30,//r
        rand() % 225 + 30,//g
        rand() % 225 + 30,//b
        rand() % 160 + 40 //a
               ),
    foodEaten(0), dying(false), speedMultiplier(10.0f), startingRadius(15.0f), childRadius(0.0f), radiusToShrinkBy(0.0f), framesToNextPathfind(0), childrenProduced(0), microbeSpawnTime(SDL_GetTicks()), currentStatus(0)
{
}

void Microbe::draw()
{
  //draw child
  if ( childRadius > 0)
  {
    filledCircleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), childRadius, colourR, colourG, colourB, colourA);
    circleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), childRadius, 0, 0, 0, 255);
  }

  //draw microbe itself
  filledCircleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width, colourR, colourG, colourB, colourA);
  circleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), width, 0, 0, 0, 255);
}

void Microbe::update()
{
  int resetVal = 4;
  
  //if the shrinkage is due to reproduction
  if (radiusToShrinkBy > 0 )
  {
    if (!dying) {
      width -=  startingRadius/100;
      radiusToShrinkBy -= startingRadius/100;
    }

    //else, shrinkage is due to death
    else {
      width -=  startingRadius/100;
      radiusToShrinkBy -= startingRadius/100;

      //when shrinkage of the microbe reaches the edges of the child, reduce the size
      //of the child with the microbe itself
      if (width < childRadius) childRadius = width;
    }
  }

  //if off the screen and not dying
  if ((position.getX() < 0 || position.getX() > TheGame::Instance()->getWindowWidth() ||
       position.getY() < 0 || position.getY() > TheGame::Instance()->getWindowHeight()) &&
      !dying)
  {
    //find a path towards the center of the screen
    moveTowards(Vector2D(TheGame::Instance()->getWindowWidth()/2,
                         TheGame::Instance()->getWindowHeight()/2));
  }
  //if has eaten enough food to reproduce
  else if (foodEaten >= foodRequiredToMate && !dying)
  {
    //check if there is another partner to reproduce with
    int nearestPartnerIndex = locateNearestReproductivePartner();

    if (nearestPartnerIndex != -1) //if there is an available partner
    {
      currentStatus = 2;
      //determine path to nearest partner
      if (framesToNextPathfind <= 0) {
        pathFinder.findPath(position, TheEnvironment::Instance()->microbes[nearestPartnerIndex]->position);
        framesToNextPathfind = resetVal;
      }
      if (pathFinder.pathway.size() > 0) {
        //continue to move towards the nearest partner //USING A*
        moveTowards(pathFinder.pathway.front()->getNodeCentralPosition());
      }
            
      //and if in contact with partner
      if (GameObject::checkForCollisionWithCircle(
              TheEnvironment::Instance()->microbes[nearestPartnerIndex]))
      {
        //reproduce - genetic algorithm
        completedReproduction();
        produceChild();
        TheEnvironment::Instance()->microbes[nearestPartnerIndex]->deathFromReproduction();
      }
    }
    else //no available partners
    {    //go to random location
      currentStatus = 3;
      if (framesToNextPathfind <= 0) {
        pathFinder.findPath(position, generateRandomNearLocation());
        framesToNextPathfind = resetVal*6;
      }
        
      if (pathFinder.pathway.size() > 0) {
        //move on path towards random location //USING A*
        moveTowards(pathFinder.pathway.front()->getNodeCentralPosition());
      }
    }
  }
  //else not eaten enough food, look for nearest food source
  else if (!dying/*not dying*/)  
  {
    //locate nearest food source
    int nearestFoodSource = locateNearestFoodSource();
    
    if (nearestFoodSource != -1) //if there is an available foodSource
    {
      currentStatus = 0;
      //determine path to nearest foodSource
      if (framesToNextPathfind <= 0) {
        pathFinder.findPath(position, TheEnvironment::Instance()->foodSources[nearestFoodSource]->position);
        framesToNextPathfind = resetVal;
      }
        
      if (pathFinder.pathway.size() > 0) {
        //continue to move towards the nearest foodSource //USING Astar
        moveTowards(pathFinder.pathway.front()->getNodeCentralPosition());
      }
            
      //and if in contact with this food source,
      if (GameObject::checkForCollisionWithCircle(
              TheEnvironment::Instance()->foodSources[nearestFoodSource]))
      {
        //run appropriate code to reduce size of food source
        TheEnvironment::Instance()->foodSources[nearestFoodSource]->consumedByMicrobe();
        //and increase size and consumuption counter for microbe
        consumedFoodSource();
      }
    }
    else //no food sources
    { //go to random location
      currentStatus = 1;
      if (framesToNextPathfind <= 0) {
        pathFinder.findPath(position, generateRandomNearLocation());
        framesToNextPathfind = resetVal;
      }
        
      if (pathFinder.pathway.size() > 0) {
        //move towards random location  //USING Astar
        moveTowards(pathFinder.pathway.front()->getNodeCentralPosition());
      }
      
    }
  }
  else //dying - randomly move about whilst shrinking
  {
    currentStatus = 4;
    if (framesToNextPathfind <= 0) {
      pathFinder.findPath(position, generateRandomNearLocation());
      framesToNextPathfind = resetVal;
    }
        
    if (pathFinder.pathway.size() > 0) {
      //move towards random location  //USING Astar
      moveTowards(pathFinder.pathway.front()->getNodeCentralPosition());
    }
  }
  
  framesToNextPathfind--;
  
  GameObject::update();
}

void Microbe::moveTowards(Vector2D target)
{
  //distance between microbe and target
  Vector2D dirToMove = (target - position);

  //magnitude
  float hyp = dirToMove.magnitude();

  dirToMove.setX(dirToMove.getX() / hyp);
  dirToMove.setY(dirToMove.getY() / hyp);

  acceleration = dirToMove * (speedMultiplier / 100);
}

void Microbe::clean()
{
  pathFinder.clean();
}

//calculates the distance between the microbe and each food source
//stores the index of the closest food source and returns it
int Microbe::locateNearestFoodSource()
{
  //std::cout <<  "foodSourcesA: " << TheEnvironment::Instance()->foodSources.size() << "\n";
  //index of closest found foodsource
  int nearestFoodSourceIndex = -1;
  //distance to nearest found food source
  float nearestFoodSourceDistance = UINT_MAX;

  //if there are currently foodsources
  if (TheEnvironment::Instance()->foodSources.size() > 0) {
    //for each food source
    for (size_t i = 0; i < TheEnvironment::Instance()->foodSources.size(); i++)
    {
      //std::cout << "looping " << i << "\n";
      //calculate the distance between the food source and microbe
      float distance = position.calcDistance(TheEnvironment::Instance()->foodSources[i]->position);

      //if the distance is less
      if (distance < nearestFoodSourceDistance)
      {
        //save the index and distance
        nearestFoodSourceDistance = distance;
        nearestFoodSourceIndex = i; 
      }   
    }
    //return index of closest found foodsource
    return nearestFoodSourceIndex;
  }
  else //no foodSources found, return -1
  {
    return -1;
  }
}

//check each microbe to see if they are ready to reproduce
int Microbe::locateNearestReproductivePartner()
{
  //index of closest found foodsource
  int nearestPartnerIndex = -1;
  //distance to nearest found food source
  float nearestPartnerDistance = UINT_MAX;

  //if there are currently foodsources
  if (TheEnvironment::Instance()->microbes.size() > 0) {
    //for each food source
    for (size_t i = 0; i < TheEnvironment::Instance()->microbes.size(); i++)
    {
      //if not looking at the current microbe searching for a partner
      if (TheEnvironment::Instance()->microbes[i] != this)
      {
        //if ready to reproduce
        if (TheEnvironment::Instance()->microbes[i]->isReadyToReproduce()){
          //calculate the distance between the food source and microbe
          float distance = position.calcDistance(TheEnvironment::Instance()->microbes[i]->position);

          //if the distance is less
          if (distance < nearestPartnerDistance)
          {
            //save the index and distance
            nearestPartnerDistance = distance;
            nearestPartnerIndex = i; 
          }
        }
      }
    }
    //return index of closest found foodsource
    return nearestPartnerIndex;
  }
  else //there is only a single microbe, so return -1
  {
    return -1;
  }
}


void Microbe::consumedFoodSource()
{
  //increase size of microbe
  GameObject::width += ((startingRadius/foodRequiredToMate)*1.1f);//0.01f;

  //decrease speed of microbe up until 10% of original speed
  if (speedMultiplier > 0.1f)
  {
    //std::cout << speedMultiplier << "\n";
    speedMultiplier -= (speedMultiplier/8000);
  }

  //increase mass of microbe
  
  //add value to total food consumption towards next reproduction
  foodEaten++;
  
  //increase child draw size
  childRadius +=  (startingRadius/foodRequiredToMate);

  //std::cout << foodEaten << " / " << foodRequiredToMate<< "\n";

  //if large enough to reproduce, is handled in update()
}

void Microbe::completedReproduction()
{
  //increment the counter of how many children the microbe has produced
  childrenProduced++;
  
  //reset food counter so will start collecting food once again
  foodEaten = 0;

  //set radiusToShrinkBy to size of child so over next frames will reduce down in size
  radiusToShrinkBy = startingRadius; //shrink by the size of the child now

  //the child has been removed and instantiated as its own class, so does not need to be drawn here anymore
  childRadius = 0.0f;
}

//returns true, ready to reproduce if it has consumed enough food and is not dying
bool Microbe::isReadyToReproduce()
{
  if (!dying) {
    if (foodEaten >= foodRequiredToMate)
    {
      return true;
    }
  } 
  return false;
}


void Microbe::deathFromReproduction()
{
  //increment the counter of how many children the microbe has produced
  childrenProduced++;
  
  //set radiusToShrinkBy to size of microbe so over next frames will shrink entirely
  radiusToShrinkBy = width;

  //set dying to true to disable reproduction
  dying = true;

  //remove all velocity so the user can clearly see the microbe is dying
  velocity.clear();

  //reduce the speed of the microbe as it dies
  speedMultiplier = 0.1f;
}


void Microbe::produceChild()
{
  Microbe* childMicrobe = new Microbe(); //potential memory leak

  float defaultDampingVal = 0.6f;

  float dampingVariation =  -(defaultDampingVal/10) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((defaultDampingVal/10)-(-(defaultDampingVal/10)))));

  //set to the position of the spawning parent
  childMicrobe->setPosition(position);
  
  childMicrobe->setVelocity(0,0);
  childMicrobe->setMass(1.0f);
  childMicrobe->setDamping(0.6f + dampingVariation);
  childMicrobe->setAcceleration(0.0f, 0.0f); //no gravity

  //set to retain the same colours as the parent
  childMicrobe->colourR = colourR;
  childMicrobe->colourG = colourG;
  childMicrobe->colourB = colourB;
  childMicrobe->colourA = colourA;

  childMicrobe->pathFinder.setGrid(&TheEnvironment::Instance()->grid);
    
  //add to the list of microbes
  TheEnvironment::Instance()->microbes.push_back(childMicrobe);
}

//randomly generate a location near to the microbe's current position that is plus/minus 1/6 of the window size away for both width and height
Vector2D Microbe::generateRandomNearLocation()
{
  //ints that will store rand gen coords
  int x, y;

  //amount of size to randomly add/minus from current position 
  int randSizeX = TheGame::Instance()->getWindowWidth() / 6;
  int randSizeY = TheGame::Instance()->getWindowHeight() / 6;


  do {
    x = position.getX() + (rand() % (randSizeX*2) - randSizeX);
  }
  while ( x >= TheGame::Instance()->getWindowWidth() || x < 0); //do until within the boundaries

  do {
    y = position.getY() + (rand() % (randSizeX*2) - randSizeX);
  }
  while ( y >= TheGame::Instance()->getWindowHeight() || y < 0);

  //return new random position
  return Vector2D(x,y);
}
