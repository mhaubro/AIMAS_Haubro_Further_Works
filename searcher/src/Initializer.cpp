#include "Initializer.h"
#include "Location.h"
#include "Base.h"
#include "Agent.h"
#include "Goal.h"
#include "Box.h"
#include "Statics.h"
#include <string>
#include <sstream>
#include <list>
#include <unordered_map>
#include <map>
#include <regex>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using std::string;
using std::stringstream;
using std::pair;
using std::vector;
using std::unordered_map;

namespace Initializer {

  const char char_free = ' ';
  const char char_wall = '+';
  const char char_static = '-';

  char char_agent(const int agent_num){
    return agent_num + '0';
  }

  bool isAgent(const char c) {
  	return ('0' <= c && c <= '9');
  }

  bool isBox(const char c) {
  	return ('A' <= c && c <= 'Z');
  }

  bool isGoal(const char c) {
  	return ('a' <= c && c <= 'z');
  }

  bool isWall(const char c){
    return c == char_wall;
  }

  bool isFree(const char c){
    return c == char_free;
  }


  bool check_unmoveable(vector<string>& rows, std::map<char, string> color_map){
    std::set<string> agent_colors;
    for (auto s : rows)
      for (auto c : s)
        if (isAgent(c))
          agent_colors.insert(color_map[c]);

    bool changes = false;

    for (auto& s : rows)
      for (auto& c : s)
        if (isBox(c) && agent_colors.find(color_map[c])==agent_colors.end()){
          c = char_wall;
          changes = true;
        }

    return changes;
  }

  Location find_char(const char chr, const vector<string> rows){
    for (int y = 0; y < rows.size(); y++){
      for (int x = 0; x < rows[y].length(); x++){
        if (rows[y][x] == chr)
          return Location(x,y);
      }
    }
    return Location(-1,-1);
  }

  const uint16_t wall_code = 1 << 11;
  const uint16_t box_code = 1 << 10;
  const uint16_t free_code = 0;

  string map_to_string(vector<uint16_t> map, int width, int height){
    stringstream ss("");

    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
        uint16_t code = map[x + y*width];
        if (code == wall_code){
          ss << '+';
        } else if (code == box_code){
          ss << '-';
        } else {
          ss << ' ';
        }
      }
      ss << std::endl;
    }
    return ss.str();
  }

  bool is_agent_code(uint16_t code, int agent){
    return code & (1<<agent) > 0;
  }

  bool match_color(const uint16_t code, const string color, std::map<char, string> color_map){
    for (int i = 0; i < 10; i++){
      if (code & (1<<i)){
        if (color_map[char_agent(i)] == color)
          return true;
      }
    }
    return false;
  }

  string print_agent_code(uint16_t code){
    stringstream ss("");
    ss << "[";
    for (int i = 0; i < 10; i++){
      ss << (char)('0' + ((code & (1<<i)) > 0));
    }
    ss << "] " << code;
    return ss.str();
  }

  vector<vector<string>> split_regions(vector<string> rows, std::map<char, string> color_map){


    // check for unmoveable objects
    check_unmoveable(rows, color_map);

    int height = rows.size();
    int width = 0;
    for (string s : rows){
      int length = s.length();
      if (length > width)
        width = length;
    }



    vector<uint16_t> map(width*height);
    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
        int i = x + y*width;
        if (x >= rows[y].size()){
          map[i] = free_code;
          continue;
        }
        char c = rows[y][x];
        if (c == char_wall){
          map[i] = wall_code;
        }else if (isBox(c)){
          map[i] = box_code;
        }else{
          map[i] = free_code;
        }

      }
    }

    const vector<Location> neighbors = {Location(1,0),Location(0,1),Location(-1,0),Location(0,-1)};

    for (int agent = 0; agent < 10; agent++){
      std::queue<Location> frontier;

      // find the location of the agent
      Location agent_pos = find_char(char_agent(agent), rows);
      // if location is negative, the agent is not present
      if (agent_pos.getX() == -1 && agent_pos.getY() == -1){
        continue;
      }

      // push the agents location to begin the BFS
      frontier.push(agent_pos);

      // define initial search code
      uint16_t search_code = 1 << agent;

      // search as long as there are new locations being pushed
      while (!frontier.empty()){
        // get a location from the queue
        Location pos = frontier.front();
        frontier.pop();
        int index = pos.getX() + pos.getY() * width;
        uint16_t map_code = map[index];

        if (map_code == wall_code || map_code == search_code){
          // continue if the location is a wall or have already been visited
          continue;
        }else if (map_code == box_code){
          // if the location is a box, check if it's moveable
          const string b_color = color_map[rows[pos.getY()][pos.getX()]];
          if (!match_color(search_code, b_color, color_map))
            continue;
        }else if (map_code < box_code){
          // if lower than box code, its moveable space
          search_code |= map_code;
        }

        map[index] = search_code;

        // push neighbors for searchs
        for (Location n : neighbors){
          auto tmp = pos+n;
          if (tmp.getX() < 0 || tmp.getX() > width || tmp.getY() < 0 || tmp.getY() > height)
            continue;
          frontier.push(Location(tmp));
        }
      }
    }

    // print the analysed map with static boxes

    vector<vector<string>> regions;

    uint16_t agents_found = 0;

    for (int a = 0; a < 10; a++){
      vector<string> region;
      int region_size = 0;

      uint16_t agent_code = 1 << a;

      if (agents_found & agent_code)
        continue;

      for (int y = 0; y < height; y++){
        stringstream ss("");
        for (int x = 0; x < width; x++){

          if (x > rows[y].size()){
            ss << char_free;
            continue;
          }

          int index = x + y*width;
          uint16_t c = map[index];

          if (c & agent_code){
            agents_found |= c;
            region_size++;
            ss << rows[y][x];
          }else if (c == wall_code || c == box_code){
            ss << char_wall;
          }else{
            ss << char_free;
          }

        }
        region.push_back(ss.str());
      }
      if (region_size > 0){
        regions.push_back(region);
      }
    }

    return regions;
  }

  bool is_info_string(const string input){
    std::regex multi_regex("[a-z]+:");
    std::smatch match;
    return std::regex_search(input, match, multi_regex);
  }

  string remove_space(const string in){
    string out = in;
    out.erase(remove(out.begin(), out.end(), ' '), out.end());
    return out;
  }

  Base::COLOR parse_color(const string color){
    if (color == (std::string("blue"))){
      return Base::BLUE;
    } else if (color == (std::string("red"))){
      return Base::RED;
    } else if (color == (std::string("green"))){
      return Base::GREEN;
    } else if (color == (std::string("cyan"))){
      return Base::CYAN;
    } else if (color == (std::string("magenta"))){
      return Base::MAGENTA;
    } else if (color == (std::string("orange"))){
      return Base::ORANGE;
    } else if (color == (std::string("pink"))){
      return Base::PINK;
    } else if (color == (std::string("yellow"))){
      return Base::YELLOW;
    }
    return Base::BLUE;
  }


  std::map<char, string> map_colors(const vector<string> input){
    std::map<char, string> map;

    for (string line : input){
      stringstream ss(remove_space(line));
      string color;
      getline(ss,color,':');

      string c;
      while(!ss.eof()){
        getline(ss, c, ',');
        map[c[0]] = color;
      }
    }
    return map;
  }

  StateRepresentation parse_regions(const vector<vector<string>> regions, std::map<char, string> color_map){

    int width = 0;
    int height = 0;
    // find the maximum line width and height
    for (auto& region : regions){
      for (auto& line : region)
        if (line.size() > width)
          width = line.size();
      if (region.size() > height)
        height = region.size();
    }

    unordered_map<Location, Box, LocationHash> boxes;
    unordered_map<Location, Agent, LocationHash> agents;
    vector<bool> walls = vector<bool>(width*height);
    unordered_map<Location, Goal, LocationHash> goals;
    vector<Base> bases;

    int id_counter = 0;
    for (int r = 0; r < regions.size(); r++){
      auto& region = regions[r];
      for (int y = 0; y < region.size(); y++){
        auto& line = region[y];
        for (int x = 0; x < line.size(); x++){
          char c = line[x];
          if (isWall(c)){
            walls[x + y * width] = true;
          }else if (isAgent(c)){
        	bases.emplace_back(c, parse_color(color_map[c]), r, id_counter);
            agents.insert(pair<Location, Agent>(Location(x,y), Agent(bases[bases.size()], Location(x, y))));
          } else if (isBox(c)){
          	bases.emplace_back(c, parse_color(color_map[c]), r, id_counter);
            boxes.insert(pair<Location, Box>(Location(x,y), Box(bases[bases.size()], Location(x, y))));
          } else if (isGoal(c)){
          	bases.emplace_back(c, parse_color(color_map[c]), r, id_counter);
            goals.insert(pair<Location, Goal>(Location(x,y), Goal(bases[bases.size()], Location(x, y))));
          }
        }
      }
    }

    StateRepresentation initialstate(boxes, agents);

    statics = new Statics(goals, bases, walls, width, height, initialstate);

    return initialstate;
  }

  StateRepresentation read_level_string(const vector<string> lines){

    vector<string> info_strings;
    int i = 0;
    for(;i < lines.size();i++){
      if (!is_info_string(lines[i]))
        break;
      info_strings.push_back(lines[i]);
    }

    // create map of defined colors
    auto color_map = map_colors(info_strings);

    for (auto& c : color_map){
    }

    // The rest must be level lines
    vector<string> level_string;
    for (;i<lines.size();i++){
      level_string.push_back(lines[i]);
    }

    // Splitting level into regions
    vector<vector<string>> regions = split_regions(level_string, color_map);

    // parse all regions to a single node
    return parse_regions(regions, color_map);
  }

  StateRepresentation setupWorld(){

    vector<string> lines;

    string line;
    while(!std::cin.eof() && getline(std::cin, line) && line != ""){
      lines.push_back(line);
    }
    return read_level_string(lines);
  }
}
