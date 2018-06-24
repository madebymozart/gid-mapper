/// Copyright (c) 2017 Mozart Louis
/// This code is licensed under MIT license (see LICENSE.txt for details)

#include <fstream>
#include "cmd/cmd.hxx"
#include "mapper/mapper.hxx"

void configure(cli::Parser &parser) {
  parser.set_optional<std::string>("o", "output", "output\\", "~ The output folder");
  parser.set_optional<std::string>("f", "file", "", "~ A Specific file to parse");
  parser.set_optional<std::string>("p", "prefix", "Prefix for the file name");
  parser.set_optional<std::string>("d", "directory", "", "Where the files are located");
}

int main(int argc, char **argv) {
  std::cout << std::endl
            << "###### gid-mapper - TileSet Mapping tool for Tiled"
            << std::endl;
  std::cout << "###### Copyright (c) 2017 Mozart Louis" << std::endl;

  /// Create command line parser to handle all the command things
  cli::Parser parser(argc, argv);
  configure(parser);

  /// Check to see if there are any errors with the user input
  parser.run_and_exit_if_error();

  /// Check for all the file in
  auto count = 0;
  const auto dir = parser.get<std::string>("d");
  const auto prefix = parser.get<std::string>("p");
  const auto output = parser.get<std::string>("o");
  const auto file = parser.get<std::string>("f");

  std::list<std::string> map_files;
  std::list<std::string>::iterator it;
  it = map_files.begin();

  /// Check for specific file first
  if (not file.empty()) {
    std::ifstream f(file);

    if (f.bad() || !f.good()) return 2;

    /// Add the file to the list
    map_files.insert(it, file);
    count++;
    f.close();
  } else {

    /// Cannot continue without a prefix
    if (prefix.empty()) {
      std::cout << "###### Copyright (c) 2017 Mozart Louis" << std::endl;
      return 1;
    }

    while (true) {
      const auto filename = prefix + std::to_string(count + 1) + ".tmx";
      std::ifstream f(dir + filename);
      if (f.bad() || !f.good()) break;

      /// Add the file to the list
      map_files.insert(it, filename);

      ///
      it++;
      count++;
      f.close();
    }
  }

  /// Initialize mapper and execute
  std::cout << "###### Mapping " << count << " file" << (count > 1 ? "s" : "") << std::endl;
  auto mapper = new Mapper();
  mapper->execute(map_files, dir, output);

  /// Clean up program
  delete (mapper);
  std::cout << "###### Done!" << std::endl;

  /// Exit
  return 0;
}
