/// Copyright (c) 2017 Mozart Louis
/// This code is licensed under MIT license (see LICENSE.txt for details)

#include <fstream>
#include "mapper.hxx"

Mapper::Mapper() = default;

Mapper::~Mapper() = default;

void Mapper::execute(std::list<std::string> map_files, const std::string &directory, const std::string &output) {
  /// As long as the file isn't bad and open, parse the json
  try {
    /// open the json file
    std::ifstream file(directory + "config.json");
    if (!file.bad() && file.is_open()) {
      file >> j_;
      file.close();
    } else throw std::exception();
  } catch (const std::exception &) {
    std::cout << std::endl << "JSON Config is either does not ""exist, is malformed or corrupt :(" << std::endl;
  }

  for (const auto &file : map_files) {
    /// Load xml documents
    tinyxml2::XMLDocument doc;
    doc.LoadFile((directory + file).c_str());

    /// Retrieve needed data from xml
    const auto dimension_x = doc.FirstChildElement("map")->FindAttribute("width")->IntValue();
    const auto dimension_y = doc.FirstChildElement("map")->FindAttribute("height")->IntValue();
    const auto data = doc.FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data")->GetText();

    /// Attempt to split
    auto ll = split(data, ',', dimension_x, dimension_y);
    map(ll, dimension_x, dimension_y);
    save(ll, doc, dimension_x, dimension_y, output, file);

  }
}

void Mapper::map(matrix &maze, const int dimension_x, int dimension_y) {
  /// Create the list of names for each temp direction
  std::list<std::string> x3m = {TOP_LEFT, TOP_CENTER, TOP_RIGHT,
                                CENTER_LEFT, CENTER, CENTER_RIGHT,
                                BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT};

  for (ul ii = 0; ii < dimension_x; ii++) {
    for (ul jj = 0; jj < dimension_y; jj++) {
      /// If the cell in the matrix is empty continue to the next cell
      if (maze.at(ii).at(jj) == 0) continue;

      /// Creating an unordered map for the lookup we will do to verify if the
      /// pattern should be applied
      std::unordered_set<std::string> x3lookup;

      auto x3m_iter = x3m.begin();

      for (auto i = -1; i <= 1; i++) {
        for (auto j = -1; j <= 1; j++) {
          try {
            if (maze.at(ii + i).at(jj + j)) x3lookup.emplace(*x3m_iter);
          } catch (const std::exception &e) {}
          ++x3m_iter;
        }
      }

      /// Compare
      if (isCross(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_CROSS);
      else if (isHorizontalDown(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_HORIZONTAL_DOWN);
      else if (isHorizontalUp(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_HORIZONTAL_UP);
      else if (isVerticalLeft(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_VERTICAL_LEFT);
      else if (isVerticalRight(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_VERTICAL_RIGHT);
      else if (isBottomLeft(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_BOTTOM_LEFT);
      else if (isBottomRight(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_BOTTOM_RIGHT);
      else if (isUpLeft(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_UP_LEFT);
      else if (isUpRight(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_UP_RIGHT);
      else if (isHorizontal(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_HORIZONTAL);
      else if (isVertical(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_VERTICAL);
      else if (isEndDown(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_END_DOWN);
      else if (isEndLeft(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_END_LEFT);
      else if (isEndRight(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_END_RIGHT);
      else if (isEndUp(x3lookup))maze.at(ii).at(jj) = getGIDFromJson(TMX_GID_END_UP);
    }
  }
}

matrix Mapper::split(const char *map, const char delim, const int dimension_x, const int dimension_y) {
  /// Create out matrix instance
  matrix m;
  for (auto i = 0; i < dimension_x; i++) m.emplace_back();

  ul row = 0;
  auto column = 0;
  do {
    const char *begin = map;
    while (*map != delim && *map) map++;
    m.at(row).push_back(std::stoi(begin));
    column++;

    if (column > dimension_y - 1) {
      row++;
      column = 0;
    }

  } while (0 != *map++);

  return m;
}

void Mapper::save(matrix &maze, tinyxml2::XMLDocument &doc, const int dimension_x, int dimension_y,
                  const std::string &dir, const std::string &name) {
  std::string data;

  /// Loop through the generated maze to create the string data in CSV format
  for (ul ii = 0; ii < dimension_x; ii++)
    for (ul jj = 0; jj < dimension_y; jj++)
      data += std::to_string(maze.at(ii).at(jj)) + ",";

  /// Stream into our data string. We need to pop the last char in the string
  /// because it will always be a leftover comma.
  data.pop_back();

  const auto final = getCurrentDirectory() + "\\" + dir;

  createDirectory(final);
  doc.FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data")->SetText(data.c_str());
  doc.SaveFile((final + name).data());
}

void Mapper::createDirectory(std::string dir) {
  mkdir(dir.data());
}

std::string Mapper::getCurrentDirectory(void) {
  char buff[FILENAME_MAX];
  GetCurrentDir(buff, FILENAME_MAX);
  std::string current_working_dir(buff);
  return current_working_dir;
}
