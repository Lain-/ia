#include "Credits.h"

#include <fstream>
#include <iostream>

#include "Input.h"
#include "Engine.h"
#include "TextFormatting.h"
#include "Renderer.h"

void Credits::readFile() {
  lines.resize(0);

  string curLine;
  ifstream file("credits.txt");

  vector<string> formatedLines;

  if(file.is_open()) {
    while(getline(file, curLine)) {
      if(curLine.empty()) {
        lines.push_back(curLine);
      } else {
        eng.textFormatting->lineToLines(
          curLine, MAP_W - 2, formatedLines);

        for(unsigned int i = 0; i < formatedLines.size(); i++) {
          lines.push_back(formatedLines.at(i));
        }
      }
    }
  }

  file.close();
}

void Credits::drawInterface() {
  const string decorationLine(MAP_W - 2, '-');

  eng.renderer->coverArea(
    panel_screen, Pos(0, 1), Pos(MAP_W, 2));

  eng.renderer->drawText(
    decorationLine, panel_screen, Pos(1, 1), clrWhite);

  eng.renderer->drawText(
    " Displaying credits.txt ", panel_screen, Pos(3, 1), clrWhite);

  eng.renderer->drawText(
    decorationLine, panel_char, Pos(1, 1), clrWhite);

  eng.renderer->drawText(
    " space/esc to exit ", panel_char, Pos(3, 1), clrWhite);
}

void Credits::run() {
  eng.renderer->clearScreen();

  string str;

  drawInterface();

  Pos pos(1, 2);
  for(unsigned int i = 0; i < lines.size(); i++) {
    eng.renderer->drawText(lines.at(i), panel_screen, pos, clrWhite);
    pos.y++;
  }

  eng.renderer->updateScreen();

  //Read keys
  bool done = false;
  while(done == false) {
    const KeyboardReadReturnData& d = eng.input->readKeysUntilFound();
    if(d.sdlKey_ == SDLK_SPACE || d.sdlKey_ == SDLK_ESCAPE) {
      done = true;
    }
  }
  eng.renderer->coverPanel(panel_screen);
}
