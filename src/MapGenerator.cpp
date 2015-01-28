/*
    Random Map Generator.
    Copyright (C) 2014  Paul Wortmann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    paul.wortmann@gmail.com
*/

#include <iostream>
#include <fstream>
#include "MapGenerator.h"
#include "MapHelper.h"
#include "MapSaver.h"


void MapGenerator::Initialize(Map* map_pointer, int dimension_x, int dimension_y)
{
    map_pointer->clearEvents();
    map_pointer->clearQueues();
    map_pointer->clearLayers();
}

void MapGenerator::Export(Map* map_pointer, std::string file_name)
{
    MapSaver* mapSaver = new MapSaver(map_pointer);
    mapSaver->saveMap(file_name);
    delete mapSaver;
}

void MapGenerator::map_to_flare_map (Map* map_pointer, flare_map_type* flare_map_pointer, int tile_set)
{
    map_gen_redundant_tile_check (map_pointer);
    flare_map_pointer->width = map_pointer->w;
    flare_map_pointer->height = map_pointer->h;
    flare_map_pointer->no_of_tiles = map_pointer->size();
    flare_map_pointer->layer_background = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_fringe = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_object = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_foreground = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->layer_collision = new int[flare_map_pointer->no_of_tiles];
    flare_map_pointer->tileset = tile_set;
    for (int i = 0; i < map_pointer->size(); i++)
    {
        flare_map_pointer->layer_background[i] = 0;
        flare_map_pointer->layer_fringe[i] = 0;
        flare_map_pointer->layer_object[i] = 0;
        flare_map_pointer->layer_foreground[i] = 0;
        flare_map_pointer->layer_collision[i] = 0;
    }
    switch (tile_set)
    {
        case TILESET_CAVE:
            flare_map_pointer->map_name = "Randomly generated cave";
            flare_map_pointer->tileset_file_name = "tilesetdefs/tileset_cave.txt";
            flare_map_pointer->tile_width = 64;
            flare_map_pointer->tile_height = 32;
            flare_map_pointer->music_file_name = "music/cave_theme.ogg";
            for (int i = 0; i < flare_map_pointer->no_of_tiles; i++)
            {
                int temp_tile[8];
                int temp_tile_ok[8];
                temp_tile[0] = i-1;
                temp_tile[1] = i+1;
                temp_tile[2] = i+map_pointer->w;
                temp_tile[3] = i+map_pointer->w+1;
                temp_tile[4] = i+map_pointer->w-1;
                temp_tile[5] = i-map_pointer->w;
                temp_tile[6] = i-map_pointer->w+1;
                temp_tile[7] = i-map_pointer->w-1;
                for (int j = 0; j < 8; j++) temp_tile_ok[j] = ((temp_tile[j] >= 0)&&(temp_tile[j] < map_pointer->size()));
                int rand_value = 0;
                switch (map_pointer->tile[i].data)
                {
                    case Tile_Type::TILE_FLOOR:
                        flare_map_pointer->layer_background[i] = 16 + rand() % 15;
                        flare_map_pointer->layer_collision[i] = 0;
                        //random chance to place an object
                        rand_value = rand() % 400;
                        if      (rand_value ==  0) flare_map_pointer->layer_object[i] = 132;
                        else if (rand_value ==  1) flare_map_pointer->layer_object[i] = 133;
                        else if (rand_value ==  2) flare_map_pointer->layer_object[i] = 134;
                        else if (rand_value ==  3) flare_map_pointer->layer_object[i] = 135;
                        else if (rand_value ==  4) flare_map_pointer->layer_object[i] = 48;
                        else if (rand_value ==  5) flare_map_pointer->layer_object[i] = 49;
                        else if (rand_value ==  6) flare_map_pointer->layer_object[i] = 50;
                        else if (rand_value ==  7) flare_map_pointer->layer_object[i] = 51;
                        else if (rand_value ==  8) flare_map_pointer->layer_object[i] = 52;
                        else if (rand_value ==  9) flare_map_pointer->layer_object[i] = 53;
                        else if (rand_value == 10) flare_map_pointer->layer_object[i] = 54;
                        else if (rand_value == 11) flare_map_pointer->layer_object[i] = 55;
                        else if (rand_value == 12) flare_map_pointer->layer_object[i] = 144;
                        else if (rand_value == 13) flare_map_pointer->layer_object[i] = 145;
                        else if (rand_value == 14) flare_map_pointer->layer_object[i] = 146;
                        else if (rand_value == 15) flare_map_pointer->layer_object[i] = 147;
                        else if (rand_value == 16) flare_map_pointer->layer_object[i] = 148;
                        else if (rand_value == 17) flare_map_pointer->layer_object[i] = 149;
                        else if (rand_value == 18) flare_map_pointer->layer_object[i] = 150;
                        else if (rand_value == 19) flare_map_pointer->layer_object[i] = 151;
                        else if (rand_value == 10) flare_map_pointer->layer_object[i] = 152;
                        else if (rand_value == 11) flare_map_pointer->layer_object[i] = 153;
                    break;
                    case Tile_Type::TILE_WALL:
                        flare_map_pointer->layer_collision[i] = 1;
                        //wall up
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[5])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 16;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 107;
                            else if (rand_value == 1) flare_map_pointer->layer_object[i] = 71;
                            else                      flare_map_pointer->layer_object[i] = 67;
                        }
                        //wall down
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 21;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 113;
                            else if (rand_value == 1) flare_map_pointer->layer_object[i] = 109;
                            else if (rand_value == 2) flare_map_pointer->layer_object[i] = 105;
                            else if (rand_value == 3) flare_map_pointer->layer_object[i] = 101;
                            else if (rand_value == 4) flare_map_pointer->layer_object[i] = 97;
                            else if (rand_value == 5) flare_map_pointer->layer_object[i] = 69;
                            else                      flare_map_pointer->layer_object[i] = 65;
                            flare_map_pointer->layer_background[i] = 16;
                        }
                        //wall left
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 16;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 106;
                            else if (rand_value == 1) flare_map_pointer->layer_object[i] = 70;
                            else                      flare_map_pointer->layer_object[i] = 66;
                        }
                        //wall right
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 21;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 112;
                            else if (rand_value == 1) flare_map_pointer->layer_object[i] = 108;
                            else if (rand_value == 2) flare_map_pointer->layer_object[i] = 104;
                            else if (rand_value == 3) flare_map_pointer->layer_object[i] = 100;
                            else if (rand_value == 4) flare_map_pointer->layer_object[i] = 96;
                            else if (rand_value == 5) flare_map_pointer->layer_object[i] = 68;
                            else                      flare_map_pointer->layer_object[i] = 64;
                            flare_map_pointer->layer_background[i] = 16;
                        }
                        //wall convex down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 21;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 114;
                            else if (rand_value == 1) flare_map_pointer->layer_object[i] = 102;
                            else if (rand_value == 2) flare_map_pointer->layer_object[i] = 98;
                            else if (rand_value == 3) flare_map_pointer->layer_object[i] = 76;
                            else                      flare_map_pointer->layer_object[i] = 72;
                            flare_map_pointer->layer_background[i] = 16;
                        }
                        //wall concave down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 21;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 115;
                            else if (rand_value == 1) flare_map_pointer->layer_object[i] = 103;
                            else if (rand_value == 2) flare_map_pointer->layer_object[i] = 99;
                            else if (rand_value == 3) flare_map_pointer->layer_object[i] = 84;
                            else                      flare_map_pointer->layer_object[i] = 80;
                            flare_map_pointer->layer_background[i] = 16;
                        }
                        //wall convex left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 9;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 77;
                            else                      flare_map_pointer->layer_object[i] = 73;
                        }
                        //wall concave left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 9;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 85;
                            else                      flare_map_pointer->layer_object[i] = 81;
                            flare_map_pointer->layer_background[i] = 57;
                        }
                        //wall convex up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 9;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 78;
                            else                      flare_map_pointer->layer_object[i] = 74;
                        }
                        //wall concave up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 9;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 86;
                            else                      flare_map_pointer->layer_object[i] = 82;
                        }
                        //wall convex right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 9;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 79;
                            else                      flare_map_pointer->layer_object[i] = 75;
                        }
                        //wall concave right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 9;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 87;
                            else                      flare_map_pointer->layer_object[i] = 83;
                            flare_map_pointer->layer_background[i] = 56;
                        }
                    break;
                    case Tile_Type::TILE_EXIT:
                    break;
                    case Tile_Type::TILE_PATH:
                    break;
                case Tile_Type::TILE_NONE:
                    default:
                        flare_map_pointer->layer_background[i] = 0;
                        flare_map_pointer->layer_collision[i] = 3;
                    break;
                }
            }
        break;
        case TILESET_DUNGEON:
            flare_map_pointer->map_name = "Randomly generated dungeon";
            flare_map_pointer->tileset_file_name = "tilesetdefs/tileset_dungeon.txt";
            flare_map_pointer->tile_width = 64;
            flare_map_pointer->tile_height = 32;
            flare_map_pointer->music_file_name = "music/dungeon_theme.ogg";
            for (int i = 0; i < flare_map_pointer->no_of_tiles; i++)
            {
                int temp_tile[8];
                int temp_tile_ok[8];
                temp_tile[0] = i-1;
                temp_tile[1] = i+1;
                temp_tile[2] = i+map_pointer->w;
                temp_tile[3] = i+map_pointer->w+1;
                temp_tile[4] = i+map_pointer->w-1;
                temp_tile[5] = i-map_pointer->w;
                temp_tile[6] = i-map_pointer->w+1;
                temp_tile[7] = i-map_pointer->w-1;
                for (int j = 0; j < 8; j++) temp_tile_ok[j] = ((temp_tile[j] >= 0)&&(temp_tile[j] < map_pointer->size()));
                int rand_value = 0;
                switch (map_pointer->tile[i].data)
                {
                    case Tile_Type::TILE_FLOOR:
                        rand_value = rand() % 100;
                        if (rand_value < 25) flare_map_pointer->layer_background[i] = 36 + rand() % 12;
                        else  flare_map_pointer->layer_background[i] = 16 + rand() % 3;
                        flare_map_pointer->layer_collision[i] = 0;
                        //random chance to place an object
                        rand_value = rand() % 400;
                        if      (rand_value ==  0) flare_map_pointer->layer_object[i] = 176;
                        else if (rand_value ==  1) flare_map_pointer->layer_object[i] = 177;
                        else if (rand_value ==  2) flare_map_pointer->layer_object[i] = 178;
                        else if (rand_value ==  3) flare_map_pointer->layer_object[i] = 179;
                        else if (rand_value ==  4) flare_map_pointer->layer_object[i] = 180;
                        else if (rand_value ==  5) flare_map_pointer->layer_object[i] = 181;
                        else if (rand_value ==  4) flare_map_pointer->layer_object[i] = 182;
                        else if (rand_value ==  5) flare_map_pointer->layer_object[i] = 183;
                    break;
                    case Tile_Type::TILE_WALL:
                        flare_map_pointer->layer_background[i] = 16;
                        flare_map_pointer->layer_collision[i] = 1;
                        //wall up
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[5])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 67;
                        }
                        //wall down
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 21;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 109;
                            else if (rand_value == 1) flare_map_pointer->layer_object[i] = 107;
                            else if (rand_value == 2) flare_map_pointer->layer_object[i] = 105;
                            else if (rand_value == 3) flare_map_pointer->layer_object[i] = 103;
                            else if (rand_value == 4) flare_map_pointer->layer_object[i] = 101;
                            else if (rand_value == 5) flare_map_pointer->layer_object[i] = 99;
                            else if (rand_value == 6) flare_map_pointer->layer_object[i] = 97;
                            else if (rand_value == 7) flare_map_pointer->layer_object[i] = 69;
                            else                      flare_map_pointer->layer_object[i] = 65;
                        }
                        //wall left
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 66;
                        }
                        //wall right
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 21;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 108;
                            else if (rand_value == 2) flare_map_pointer->layer_object[i] = 106;
                            else if (rand_value == 3) flare_map_pointer->layer_object[i] = 104;
                            else if (rand_value == 4) flare_map_pointer->layer_object[i] = 102;
                            else if (rand_value == 5) flare_map_pointer->layer_object[i] = 100;
                            else if (rand_value == 6) flare_map_pointer->layer_object[i] = 96;
                            else if (rand_value == 7) flare_map_pointer->layer_object[i] = 68;
                            else                      flare_map_pointer->layer_object[i] = 64;
                        }
                        //wall convex down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 77;
                        }
                        //wall concave down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 73;
                        }
                        //wall convex left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 78;
                        }
                        //wall concave left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 74;
                        }
                        //wall convex up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 79;
                        }
                        //wall concave up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 75;
                        }
                        //wall convex right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 76;
                        }
                        //wall concave right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 72;
                        }
                    break;
                    case Tile_Type::TILE_EXIT:
                    break;
                    case Tile_Type::TILE_PATH:
                    break;
                case Tile_Type::TILE_NONE:
                    default:
                        flare_map_pointer->layer_background[i] = 0;
                        flare_map_pointer->layer_collision[i] = 3;
                    break;
                }
            }
        break;
        case TILESET_GRASSLAND:
            flare_map_pointer->map_name = "Randomly generated grassland";
            flare_map_pointer->tileset_file_name = "tilesetdefs/tileset_grassland.txt";
            flare_map_pointer->tile_width = 64;
            flare_map_pointer->tile_height = 32;
            flare_map_pointer->music_file_name = "music/forest_theme.ogg";
            for (int i = 0; i < flare_map_pointer->no_of_tiles; i++)
            {
                int temp_tile[8];
                int temp_tile_ok[8];
                temp_tile[0] = i-1;
                temp_tile[1] = i+1;
                temp_tile[2] = i+map_pointer->w;
                temp_tile[3] = i+map_pointer->w+1;
                temp_tile[4] = i+map_pointer->w-1;
                temp_tile[5] = i-map_pointer->w;
                temp_tile[6] = i-map_pointer->w+1;
                temp_tile[7] = i-map_pointer->w-1;
                for (int j = 0; j < 8; j++) temp_tile_ok[j] = ((temp_tile[j] >= 0)&&(temp_tile[j] < map_pointer->size()));
                int rand_value = 0;
                switch (map_pointer->tile[i].data)
                {
                    case Tile_Type::TILE_FLOOR:
                        flare_map_pointer->layer_background[i] = 16 + rand() % 16;
                        flare_map_pointer->layer_collision[i] = 0;
                        //random chance to place an object
                        rand_value = rand() % 400;
                        if      (rand_value ==  0) flare_map_pointer->layer_object[i] = 112;
                        else if (rand_value ==  1) flare_map_pointer->layer_object[i] = 113;
                        else if (rand_value ==  2) flare_map_pointer->layer_object[i] = 114;
                        else if (rand_value ==  3) flare_map_pointer->layer_object[i] = 115;
                        else if (rand_value ==  4) flare_map_pointer->layer_object[i] = 116;
                        else if (rand_value ==  5) flare_map_pointer->layer_object[i] = 117;
                        else if (rand_value ==  6) flare_map_pointer->layer_object[i] = 118;
                        else if (rand_value ==  7) flare_map_pointer->layer_object[i] = 119;
                        else if (rand_value ==  8) flare_map_pointer->layer_object[i] = 120;
                        else if (rand_value ==  9) flare_map_pointer->layer_object[i] = 121;
                        else if (rand_value == 10) flare_map_pointer->layer_object[i] = 122;
                        else if (rand_value == 11) flare_map_pointer->layer_object[i] = 123;
                        else if (rand_value == 12) flare_map_pointer->layer_object[i] = 124;
                        else if (rand_value == 13) flare_map_pointer->layer_object[i] = 125;
                        else if (rand_value == 14) flare_map_pointer->layer_object[i] = 126;
                        else if (rand_value == 15) flare_map_pointer->layer_object[i] = 127;
                        else if (rand_value == 16) flare_map_pointer->layer_object[i] = 128;
                        else if (rand_value == 17) flare_map_pointer->layer_object[i] = 129;
                        else if (rand_value == 18) flare_map_pointer->layer_object[i] = 130;
                        else if (rand_value == 19) flare_map_pointer->layer_object[i] = 131;
                        else if (rand_value == 20) flare_map_pointer->layer_object[i] = 136;
                        else if (rand_value == 21) flare_map_pointer->layer_object[i] = 137;
                        else if (rand_value == 22) flare_map_pointer->layer_object[i] = 240;
                        else if (rand_value == 23) flare_map_pointer->layer_object[i] = 241;
                        else if (rand_value == 24) flare_map_pointer->layer_object[i] = 242;
                        else if (rand_value == 25) flare_map_pointer->layer_object[i] = 243;
                        else if (rand_value == 26) flare_map_pointer->layer_object[i] = 244;
                        else if (rand_value == 27) flare_map_pointer->layer_object[i] = 245;
                        else if (rand_value == 28) flare_map_pointer->layer_object[i] = 246;
                        else if (rand_value == 29) flare_map_pointer->layer_object[i] = 247;
                        else if (rand_value == 30) flare_map_pointer->layer_object[i] = 248;
                        else if (rand_value == 31) flare_map_pointer->layer_object[i] = 249;
                        else if (rand_value == 32) flare_map_pointer->layer_object[i] = 250;
                        else if (rand_value == 33) flare_map_pointer->layer_object[i] = 241;
                        else if (rand_value == 34) flare_map_pointer->layer_object[i] = 242;
                        else if (rand_value == 35) flare_map_pointer->layer_object[i] = 243;
                        else if (rand_value == 36) flare_map_pointer->layer_object[i] = 244;
                        else if (rand_value == 37) flare_map_pointer->layer_object[i] = 245;
                    break;
                    case Tile_Type::TILE_WALL:
                        flare_map_pointer->layer_background[i] = 16;
                        flare_map_pointer->layer_collision[i] = 1;
                        //wall up
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[5])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 51;
                        }
                        //wall down
                        if ((temp_tile_ok[0])&&(temp_tile_ok[1])&&(temp_tile_ok[2])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 53;
                            else                      flare_map_pointer->layer_object[i] = 49;
                        }
                        //wall left
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[0])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR))
                        {
                            flare_map_pointer->layer_object[i] = 50;
                        }
                        //wall right
                        if ((temp_tile_ok[2])&&(temp_tile_ok[5])&&(temp_tile_ok[1])&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 52;
                            else                      flare_map_pointer->layer_object[i] = 48;
                        }
                        //wall convex down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 60;
                            else                      flare_map_pointer->layer_object[i] = 56;
                        }
                        //wall concave down
                        if ((temp_tile_ok[1])&&(temp_tile_ok[2])&&(temp_tile_ok[3])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[3]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 68;
                            else                      flare_map_pointer->layer_object[i] = 64;
                        }
                        //wall convex left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 61;
                            else                      flare_map_pointer->layer_object[i] = 57;
                        }
                        //wall concave left
                        if ((temp_tile_ok[0])&&(temp_tile_ok[2])&&(temp_tile_ok[4])&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[2]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[4]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 69;
                            else                      flare_map_pointer->layer_object[i] = 65;
                        }
                        //wall convex up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 62;
                            else                      flare_map_pointer->layer_object[i] = 58;
                        }
                        //wall concave up
                        if ((temp_tile_ok[5])&&(temp_tile_ok[0])&&(temp_tile_ok[7])&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[0]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[7]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 70;
                            else                      flare_map_pointer->layer_object[i] = 66;
                        }
                        //wall convex right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_WALL)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 63;
                            else                      flare_map_pointer->layer_object[i] = 59;
                        }
                        //wall concave right
                        if ((temp_tile_ok[1])&&(temp_tile_ok[5])&&(temp_tile_ok[6])&&(map_pointer->tile[temp_tile[1]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[5]].data == Tile_Type::TILE_FLOOR)&&(map_pointer->tile[temp_tile[6]].data == Tile_Type::TILE_FLOOR))
                        {
                            rand_value = rand() % 6;
                            if      (rand_value == 0) flare_map_pointer->layer_object[i] = 71;
                            else                      flare_map_pointer->layer_object[i] = 67;
                        }
                    break;
                    case Tile_Type::TILE_EXIT:
                    break;
                    case Tile_Type::TILE_PATH:
                    break;
                    case Tile_Type::TILE_NONE:
                    default:
                        flare_map_pointer->layer_background[i] = 0;
                        flare_map_pointer->layer_collision[i] = 3;
                    break;
                }
            }
        break;
    }
}

