#ifndef _UI_
#define _UI_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "SDL2/SDL2_gfxPrimitives.h" 

#include "GameObject.h"
#include "Game.h"
#include "InputHandler.h"

#include <iostream>
#include <vector>

//libs for opening webpage for README
#ifdef _WIN32
    #include <windows.h>
    #include <shellapi.h>
#else
    #include <stdio.h>
    #include <stdlib.h>

#endif

class Environment;

class UI
{
public:

  //the clock time when the UI and simulation was started
  unsigned simulationStartTime;

  //the last time the stats were updated
  unsigned lastStatsUpdateTime;

  //if true, the respective menu is rendered on screen
  bool displayStats;
  bool displayHelp;

  //the index of the current microbe having it's stats displayed
  int microbeIndex;
  //and the % of food it has eaten towards its next reproduction
  int foodEatenPercentage;

 //text
  TTF_Font* fontTTF; //this opens a font style and sets a size
  SDL_Color textColour = {0,0,0};  //color of the font in rgb format

  
  SDL_Surface* microbeNumberMessage;
  SDL_Texture* microbeNumberText; 
  SDL_Rect microbeNumberRect;

  SDL_Surface* widthLabelMessage;
  SDL_Texture* widthLabelText;
  SDL_Rect widthLabelRect;

  SDL_Surface* widthMessage;
  SDL_Texture* widthText;
  SDL_Rect widthRect;

  SDL_Surface* speedLabelMessage;
  SDL_Texture* speedLabelText;
  SDL_Rect speedLabelRect;

  SDL_Surface* speedMessage;
  SDL_Texture* speedText;
  SDL_Rect speedRect;

  SDL_Surface* childLabelMessage;
  SDL_Texture* childLabelText;
  SDL_Rect childLabelRect;

  SDL_Surface* childMessage;
  SDL_Texture* childText;
  SDL_Rect childRect;

  SDL_Surface* lifetimeMessage;
  SDL_Texture* lifetimeText;
  SDL_Rect lifetimeRect;

  SDL_Surface* lifetimeLabelMessage;
  SDL_Texture* lifetimeLabelText;
  SDL_Rect lifetimeLabelRect;

  SDL_Surface* statusLabelMessage;
  SDL_Texture* statusLabelText;
  SDL_Rect statusLabelRect;

  SDL_Surface* statusMessage;
  SDL_Texture* statusText;
  SDL_Rect statusRect;

  SDL_Surface* foodLabelMessage;
  SDL_Texture* foodLabelText;
  SDL_Rect foodLabelRect;

  SDL_Surface* foodMessage;
  SDL_Texture* foodText;
  SDL_Rect foodRect;

  SDL_Surface* fitnessLabelMessage;
  SDL_Texture* fitnessLabelText;
  SDL_Rect fitnessLabelRect;

  SDL_Surface* fitnessMessage;
  SDL_Texture* fitnessText;
  SDL_Rect fitnessRect;

  SDL_Surface* gene0Message;
  SDL_Texture* gene0Text;
  SDL_Rect gene0Rect;

  SDL_Surface* gene1Message;
  SDL_Texture* gene1Text;
  SDL_Rect gene1Rect;

  SDL_Surface* gene2Message;
  SDL_Texture* gene2Text;
  SDL_Rect gene2Rect;

  SDL_Surface* gene3Message;
  SDL_Texture* gene3Text;
  SDL_Rect gene3Rect;

  //stats text
  SDL_Surface* statsLabelMessage;
  SDL_Texture* statsLabelText;
  SDL_Rect statsLabelRect;

  SDL_Surface* maxFitnessAchievedMessage;
  SDL_Texture* maxFitnessAchievedText;
  SDL_Rect maxFitnessAchievedRect;
  
  SDL_Surface* maxFitnessAchievedLabelMessage;
  SDL_Texture* maxFitnessAchievedLabelText;
  SDL_Rect maxFitnessAchievedLabelRect;
  
  SDL_Surface* numMicrobesMessage;
  SDL_Texture* numMicrobesText;
  SDL_Rect numMicrobesRect;
  
  SDL_Surface* numMicrobesLabelMessage;
  SDL_Texture* numMicrobesLabelText;
  SDL_Rect numMicrobesLabelRect;
  
  SDL_Surface* avgFitnessMessage;
  SDL_Texture* avgFitnessText;
  SDL_Rect avgFitnessRect;
  
  SDL_Surface* avgFitnessLabelMessage;
  SDL_Texture* avgFitnessLabelText;
  SDL_Rect avgFitnessLabelRect;

  SDL_Surface* minFitnessMessage;
  SDL_Texture* minFitnessText;
  SDL_Rect minFitnessRect;
  
  SDL_Surface* minFitnessLabelMessage;
  SDL_Texture* minFitnessLabelText;
  SDL_Rect minFitnessLabelRect;

  SDL_Surface* maxFitnessMessage;
  SDL_Texture* maxFitnessText;
  SDL_Rect maxFitnessRect;
  
  SDL_Surface* maxFitnessLabelMessage;
  SDL_Texture* maxFitnessLabelText;
  SDL_Rect maxFitnessLabelRect;

  SDL_Surface* avgLifespanMessage;
  SDL_Texture* avgLifespanText;
  SDL_Rect avgLifespanRect;
  
  SDL_Surface* avgLifespanLabelMessage;
  SDL_Texture* avgLifespanLabelText;
  SDL_Rect avgLifespanLabelRect;

  SDL_Surface* timeElapsedMessage;
  SDL_Texture* timeElapsedText;
  SDL_Rect timeElapsedRect;
  
  SDL_Surface* timeElapsedLabelMessage;
  SDL_Texture* timeElapsedLabelText;
  SDL_Rect timeElapsedLabelRect;

  //help text
  SDL_Surface* helpLabelMessage;
  SDL_Texture* helpLabelText;
  SDL_Rect helpLabelRect;

  SDL_Surface* statsMessage;
  SDL_Texture* statsText;
  SDL_Rect statsRect;

  SDL_Surface* gridMessage;
  SDL_Texture* gridText;
  SDL_Rect gridRect;

  SDL_Surface* terrainMessage;
  SDL_Texture* terrainText;
  SDL_Rect terrainRect;

  SDL_Surface* pathMessage;
  SDL_Texture* pathText;
  SDL_Rect pathRect;

  SDL_Surface* READMEMessage;
  SDL_Texture* READMEText;
  SDL_Rect READMERect;
  
public:
  UI(); //constructor

  //renders all text and backgrounds for UI
  void draw();

  //updates the values put into text and rendered in draw()
  void update();

  //inits TTF and loads font. inits all text rects, messages, text
  void initText();

  //destroys textures for all Text, closes the font
  void clean();

  //checks for keypresses of:
  // -H key, and displays help if set to true
  // -S key, and displays stats if set to true
  // -R key, and opens README webpage if set to true
  void handleInput();
};

#endif
