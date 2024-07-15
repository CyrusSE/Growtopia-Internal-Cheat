#include <core/bot/bot.h>
#include <iostream>
#include <vector>
#include <thread>

void bot::updateLogic() {
    logic = sdk::GetGameLogic();
    localPlayer = logic->GetLocalPlayer();
    tileMap = logic->GetTileMap();
    objectMap = logic->GetObjectMap();
}

Tile* bot::getTile(int col, int row) {
    updateLogic();
    return &(tileMap->tiles[col + row * tileMap->size.x]);
}

void bot::renderTileMap() {
    vector<int> foregroundElements;
    vector<int> backgroundElements;
    vector<int> flagElements;
    int x = 0;
    int y = 0;
    while (y <= 99) {
        while (x <= 59) {
            Tile* tile = getTile(x, y);
            foregroundElements.push_back(tile->foreground);
            backgroundElements.push_back(tile->background);
            flagElements.push_back(tile->flags);
            x++;
        }
        foreground.push_back(foregroundElements);
        background.push_back(backgroundElements);
        flags.push_back(flagElements);
        foregroundElements.clear();
        backgroundElements.clear();
        flagElements.clear();
        x = 0;
        y += 1;
    }
    rendered = true;
}

void bot::updateTile(Tile* tile, int col, int row) {
    if (rendered) {
        foreground[col][row] = tile->foreground;
        background[col][row] = tile->background;
        flags[col][row] = tile->flags;
    }
}

void bot::scanDroppedItems() {
    updateLogic();
    vector<int> item;
    for (auto object = objectMap->objects.begin(); object != objectMap->objects.end(); object++) {
        item.push_back(object->item_id);
        item.push_back(object->amount);
        item.push_back(object->pos.x);
        item.push_back(object->pos.y);
        droppedItems.push_back(item);
        item.clear();
    }
}

// Actions
void bot::break_block(int item_id, int col, int row) {
    if (!isPunching) {
        Tile* tile = getTile(col, row);
        std::thread t(&bot::breakBlockA, this, tile, item_id, col, row);
        t.detach();
    }
}

void bot::breakBlockA(Tile* tile, int item_id, int col, int row) {
    if ((int)tile->foreground == 0 && (int)tile->background == 0) {
        return;
    }
    int damage = 1337;
    isPunching = true;
    while ((int)tile->foreground == item_id || (int)tile->background == item_id) {
        while (damage == (int)tile->health_or_damage) {
            Sleep(50);
        }
        if ((int)tile->foreground == item_id || (int)tile->background == item_id) {
            bot::hit(col, row);
            damage = (int)tile->health_or_damage;
        }
    }
    isPunching = false;
}

void bot::hit(int col, int row) {

    GameUpdatePacket packet{ 0 };
    packet.type = PACKET_TILE_CHANGE_REQUEST;
    packet.item_id = 18;
    packet.int_x = col;
    packet.int_y = row;
    updateLogic();
    packet.pos_x = localPlayer->pos.x;
    packet.pos_y = localPlayer->pos.y;

    gt::send(&packet);
}

void bot::place(int item_id, int col, int row) {
    GameUpdatePacket packet{ 0 };
    packet.type = PACKET_TILE_CHANGE_REQUEST;
    packet.item_id = item_id;
    packet.int_x = col;
    packet.int_y = row;
    updateLogic();
    packet.pos_x = localPlayer->pos.x;
    packet.pos_y = localPlayer->pos.y;
    gt::send(&packet);
}