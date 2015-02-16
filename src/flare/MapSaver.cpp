/*
Copyright © 2015 Igor Paliychuk

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

#include "MapSaver.h"
#include "Settings.h"

MapSaver::MapSaver(Map *_map) : map(_map)
{
	dest_file = map->getFilename();
}


MapSaver::~MapSaver()
{
}

bool MapSaver::saveMap(std::string tileset_definitions)
{
	std::ofstream outfile;

	outfile.open(dest_file.c_str(), std::ios::out);

	if (outfile.is_open()) {

		outfile << "## flare-engine generated map file ##" << "\n";

		writeHeader(outfile);
		writeTilesets(outfile, tileset_definitions);
		writeLayers(outfile);

		writeEvents(outfile);
		writeNPCs(outfile);
		writeEnemies(outfile);

		if (outfile.bad())
		{
			logError("MapSaver: Unable to save the map. No write access or disk is full!");
			return false;
		}
		outfile.close();
		outfile.clear();

		return true;
	}
	else {
		logError("MapSaver: Could not open %s for writing", dest_file.c_str());
	}
	return false;
}

bool MapSaver::saveMap(std::string file, std::string tileset_definitions)
{
	dest_file = file;

	return saveMap(tileset_definitions);
}


void MapSaver::writeHeader(std::ofstream& map_file)
{
	map_file << "[header]" << std::endl;
	map_file << "width=" << map->w << std::endl;
	map_file << "height=" << map->h << std::endl;
	map_file << "tilewidth=" << "64" << std::endl;
	map_file << "tileheight=" << "32" << std::endl;
	map_file << "orientation=" << "isometric" << std::endl;
	map_file << "music=" << map->music_filename << std::endl;
	map_file << "tileset=" << map->getTileset() << std::endl;
	map_file << "title=" << map->title << std::endl;

	map_file << std::endl;
}

void MapSaver::writeTilesets(std::ofstream& map_file, std::string tileset_definitions)
{
	map_file << "[tilesets]" << std::endl;

	map_file << tileset_definitions << std::endl;

	map_file << std::endl;
}


void MapSaver::writeLayers(std::ofstream& map_file)
{
	for (unsigned short i = 0; i < map->layernames.size(); i++)
	{
		map_file << "[layer]" << std::endl;

		map_file << "type=" << map->layernames[i] << std::endl;
		map_file << "data=" << std::endl;

		std::string layer = "";
		for (int line = 0; line < map->h; line++)
		{
			std::stringstream map_row;
			for (int tile = 0; tile < map->w; tile++)
			{
                map_row << map->layers[i][tile][line] << ",";

			}
			layer += map_row.str();
			layer += '\n';
		}
		layer.erase(layer.end()-2, layer.end());
		layer += '\n';

		map_file << layer << std::endl;
	}
}


void MapSaver::writeEnemies(std::ofstream& map_file)
{
	std::queue<Map_Group> group = map->enemy_groups;

	while (!group.empty())
	{
		map_file << "[enemy]" << std::endl;
		map_file << "type=" << group.front().category << std::endl;

		if (group.front().levelmin != 0 || group.front().levelmax != 0)
		{
			map_file << "level=" << group.front().levelmin << "," << group.front().levelmax << std::endl;
		}

		map_file << "location=" << group.front().pos.x << "," << group.front().pos.y << "," << group.front().area.x << "," << group.front().area.y << std::endl;

		if (group.front().numbermin != 1 || group.front().numbermax != 1)
		{
			map_file << "number=" << group.front().numbermin << "," << group.front().numbermax << std::endl;
		}

		if (group.front().chance != 1.0f)
		{
			map_file << "chance=" << group.front().chance*100 << std::endl;
		}

		if (group.front().direction != -1)
		{
			map_file << "direction=" << group.front().direction << std::endl;
		}

		if (!group.front().waypoints.empty() && group.front().wander_radius == 0)
		{
			map_file << "waypoints=";
			std::queue<FPoint> points = group.front().waypoints;
			while (!points.empty())
			{
				map_file << points.front().x - 0.5f << "," << points.front().y - 0.5f;
				points.pop();
				if (!points.empty())
				{
					map_file << ";";
				}
			}
			map_file << std::endl;
		}

		if ((group.front().wander_radius != 4 && group.front().waypoints.empty()))
		{
			map_file << "wander_radius=" << group.front().wander_radius << std::endl;
		}

		for (unsigned i = 0; i < group.front().requires_status.size(); i++)
		{
			map_file << "requires_status=" << group.front().requires_status[i] << std::endl;
		}

		for (unsigned i = 0; i < group.front().requires_status.size(); i++)
		{
			map_file << "requires_not_status=" << group.front().requires_not_status[i] << std::endl;
		}

		map_file << std::endl;
		group.pop();
	}
}


void MapSaver::writeNPCs(std::ofstream& map_file)
{
	std::queue<Map_NPC> npcs = map->npcs;

	while (!npcs.empty())
	{
		map_file << "[npc]" << std::endl;
		map_file << "type=" << npcs.front().id << std::endl;
		map_file << "location=" << npcs.front().pos.x - 0.5f << "," << npcs.front().pos.y - 0.5f << ",1,1" << std::endl;

		for (unsigned j = 0; j < npcs.front().requires_status.size(); j++)
		{
			map_file << "requires_status=" << npcs.front().requires_status[j] << std::endl;
		}
		for (unsigned j = 0; j < npcs.front().requires_not_status.size(); j++)
		{
			map_file << "requires_not_status=" << npcs.front().requires_not_status[j] << std::endl;
		}

		map_file << std::endl;

		npcs.pop();
	}
}

void MapSaver::writeEvents(std::ofstream& map_file)
{
	for (unsigned i = 0; i < map->events.size(); i++)
	{
		map_file << "[event]" << std::endl;
		map_file << "type=" << map->events[i].type << std::endl;

		Rect location = map->events[i].location;
		map_file << "location=" << location.x << "," << location.y << "," << location.w << "," << location.h  << std::endl;

		Rect hotspot = map->events[i].hotspot;
		if (hotspot.x == location.x && hotspot.y == location.y && hotspot.w == location.w && hotspot.h == location.h)
		{
			map_file << "hotspot=" << "location" << std::endl;
		}
		else if (hotspot.x != 0 && hotspot.y != 0 && hotspot.w != 0 && hotspot.h != 0)
		{
			map_file << "hotspot=" << hotspot.x << "," << hotspot.y << "," << hotspot.w << "," << hotspot.h << std::endl;
		}

		if (map->events[i].cooldown != 0)
		{
			std::string suffix = "ms";
			int value = (int)(1000.f * map->events[i].cooldown / MAX_FRAMES_PER_SEC);
			if (value % 1000 == 0)
			{
				value = map->events[i].cooldown / MAX_FRAMES_PER_SEC;
				suffix = "s";
			}
			map_file << "cooldown=" << value << suffix << std::endl;
		}

		Rect reachable_from = map->events[i].reachable_from;
		if (reachable_from.x != 0 && reachable_from.y != 0 && reachable_from.w != 0 && reachable_from.h != 0)
		{
			map_file << "reachable_from=" << reachable_from.x << "," << reachable_from.y << "," << reachable_from.w << "," << reachable_from.h << std::endl;
		}
		writeEventComponents(map_file, i);

		map_file << std::endl;
	}
}

void MapSaver::writeEventComponents(std::ofstream &map_file, int eventID)
{
	std::vector<Event_Component> components = map->events[eventID].components;
	for (unsigned i = 0; i < components.size(); i++)
	{
		Event_Component e = components[i];

		map_file << e.type << "=";

		if (e.type == "tooltip") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "power_path") {
			map_file << e.x << "," << e.y << ",";
			if (e.s == "hero")
			{
				map_file << e.s << std::endl;
			}
			else
			{
				map_file << e.a << "," << e.b << std::endl;
			}
		}
		else if (e.type == "power_damage") {
			map_file << e.a << "," << e.b << std::endl;
		}
		else if (e.type == "intermap") {
			map_file << e.s << "," << e.x << "," << e.y << std::endl;
		}
		else if (e.type == "intramap") {
			map_file << e.x << "," << e.y << std::endl;
		}
		else if (e.type == "mapmod") {
			map_file << e.s << "," << e.x << "," << e.y << "," << e.z;

			while (i+1 < components.size() && components[i+1].type == "mapmod")
			{
				i++;
				e = components[i];
				map_file << ";" << e.s << "," << e.x << "," << e.y << "," << e.z;
			}
			map_file << std::endl;
		}
		else if (e.type == "soundfx") {
			map_file << e.s;
			if (e.x != -1 && e.y != -1)
			{
				map_file << "," << e.x << "," << e.y;
			}
			map_file << std::endl;
		}
		else if (e.type == "loot") {

			std::stringstream chance;

			if (e.z == 0)
				chance << "fixed";
			else
				chance << e.z;

			map_file << e.s << "," << chance.str() << "," << e.a << "," << e.b;

			while (i+1 < components.size() && components[i+1].type == "loot")
			{
				i++;
				e = components[i];

				if (e.z == 0)
					chance << "fixed";
				else
					chance << e.z;

				map_file << ";" << e.s << "," << chance.str() << "," << e.a << "," << e.b;
			}
			map_file << std::endl;
			// UNIMPLEMENTED
			// Loot tables not supported
		}
		else if (e.type == "msg") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "shakycam") {
			std::string suffix = "ms";
			int value = (int)(1000.f * e.x / MAX_FRAMES_PER_SEC);
			if (value % 1000 == 0)
			{
				value = e.x / MAX_FRAMES_PER_SEC;
				suffix = "s";
			}
			map_file << value << suffix << std::endl;
		}
		else if (e.type == "requires_status") {
			map_file << e.s;

			while (i+1 < components.size() && components[i+1].type == "requires_status")
			{
				i++;
				e = components[i];
				map_file << ";" << e.s;
			}
			map_file << std::endl;
		}
		else if (e.type == "requires_not_status") {
			map_file << e.s;

			while (i+1 < components.size() && components[i+1].type == "requires_not_status")
			{
				i++;
				e = components[i];
				map_file << ";" << e.s;
			}
			map_file << std::endl;
		}
		else if (e.type == "requires_level") {
			map_file << e.x << std::endl;
		}
		else if (e.type == "requires_not_level") {
			map_file << e.x << std::endl;
		}
		else if (e.type == "requires_currency") {
			map_file << e.x << std::endl;
		}
		else if (e.type == "requires_item") {
			map_file << e.x;

			while (i+1 < components.size() && components[i+1].type == "requires_item")
			{
				i++;
				e = components[i];
				map_file << "," << e.x;
			}
			map_file << std::endl;
		}
		else if (e.type == "requires_class") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "set_status") {
			map_file << e.s;

			while (i+1 < components.size() && components[i+1].type == "set_status")
			{
				i++;
				e = components[i];
				map_file << "," << e.s;
			}
			map_file << std::endl;
		}
		else if (e.type == "unset_status") {
			map_file << e.s;

			while (i+1 < components.size() && components[i+1].type == "unset_status")
			{
				i++;
				e = components[i];
				map_file << "," << e.s;
			}
			map_file << std::endl;
		}
		else if (e.type == "remove_currency") {
			map_file << e.x << std::endl;
		}
		else if (e.type == "remove_item") {
			map_file << e.x;

			while (i+1 < components.size() && components[i+1].type == "remove_item")
			{
				i++;
				e = components[i];
				map_file << "," << e.x;
			}
			map_file << std::endl;
		}
		else if (e.type == "reward_xp") {
			map_file << e.x << std::endl;
		}
		else if (e.type == "reward_currency") {
			map_file << e.x << std::endl;
		}
		else if (e.type == "reward_item") {
			map_file << e.x << ",";
			map_file << e.y << std::endl;
		}
		else if (e.type == "restore") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "power") {
			map_file << e.x << std::endl;
		}
		else if (e.type == "spawn") {
			map_file << e.s << "," << e.x << "," << e.y;

			while (i+1 < components.size() && components[i+1].type == "spawn")
			{
				i++;
				e = components[i];
				map_file << ";" << e.s << "," << e.x << "," << e.y;
			}
			map_file << std::endl;
		}
		else if (e.type == "stash") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "npc") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "music") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "cutscene") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "repeat") {
			map_file << e.s << std::endl;
		}
		else if (e.type == "save_game") {
			map_file << e.s << std::endl;
		}
	}
}
