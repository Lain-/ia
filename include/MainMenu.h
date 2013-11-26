#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "MenuBrowser.h"
#include "CommonData.h"

class Engine;

class MainMenu {
public:
  MainMenu(Engine* engine) : eng(engine) {}

  GameEntry_t run(bool& quit, int& introMusChannel);

private:
  Engine* eng;
  void draw(const MenuBrowser& browser);

  string getHplQuote();
};

#endif