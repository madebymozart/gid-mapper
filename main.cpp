/// Copyright (c) 2017 Mozart Louis
/// This code is licensed under MIT license (see LICENSE.txt for details)

#include <fstream>
#include "cmd/cmd.hxx"
#include "mapper/mapper.hxx"

void configure(cli::Parser &parser) {
  parser.set_required<std::string>("p", "prefix", "Prefix for the file name");
  parser.set_required<std::string>("d", "directory", "Where the files are located");
}

int main(int argc, char **argv) {
  std::cout << std::endl
            << "###### GID Mapper -  Evermaze Tile Set Mapping tool for Tiled"
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

  std::list<std::string> map_files;
  std::list<std::string>::iterator it;
  it = map_files.begin();

  while (true) {
    const auto file = prefix + std::to_string(count + 1) + ".tmx";
    std::ifstream f(dir + file.c_str());
    if (f.bad() || !f.good()) break;

    /// Add the file to the list
    map_files.insert(it, file);

    ///
    it++;
    count++;
    f.close();
  }

  /// Initialize mapper and execute
  std::cout << "###### Mapping " << count << " file" << (count > 1 ? "s" : "") << std::endl;
  auto mapper = new Mapper();
  mapper->execute(map_files, dir);

  /// Clean up program
  delete (mapper);
  std::cout << "###### Done!" << std::endl;

  /// Exit
  return 0;
}
