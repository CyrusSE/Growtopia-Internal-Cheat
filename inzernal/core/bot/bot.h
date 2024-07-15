#pragma once
#include <iostream>
#include <vector>
#include <sdk/sdk.h>

class bot {
   public:
    // World
    vector<vector<int>> foreground;
    vector<vector<int>> background;
    vector<vector<int>> flags;
    bool rendered;

    vector<vector<int>> droppedItems;

    // Player
    int farmable;
    std::string worldName;

    //Game logic
    GameLogic* logic;
    NetAvatar* localPlayer;
    WorldTileMap* tileMap;
    WorldObjectMap* objectMap;

   public:

    void updateLogic();

    // Player
    bool isPunching;
    // TileMap
    Tile* getTile(int col, int row);
    void renderTileMap();
    void updateTile(Tile* tile, int col, int row);

    // Dropped Items
    void scanDroppedItems();

    public:
    // Actions
     void break_block(int item_id, int col, int row);
     void breakBlockA(Tile* tile, int item_id, int col, int row);
     void hit(int col, int row);
     void place(int item_id, int col, int row);

};
