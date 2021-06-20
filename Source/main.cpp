#include <QApplication>

#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "GUI/MainWindow.h"
#include "MemoryWatch/MemWatchEntry.h"
#include "DolphinProcess/Linux/LinuxDolphinProcess.h"
#include "DolphinProcess/IDolphinProcess.h"
#include "DolphinProcess/DolphinAccessor.h"
#include "Common/MemoryCommon.h"

void test()
{
  std::cout << "test" << std::endl;
}

int main(int argc, char** argv)
{
  CLI::App CLIapp{"App description"};
  u32 crashAddress{0};
  CLIapp.add_option("-c,--crash-address", crashAddress, "Quits the program");
  CLI11_PARSE(CLIapp, argc, argv);

  if (crashAddress)
  {
    DolphinComm::DolphinAccessor::hook();
    crashAddress = crashAddress - 0x80000000;
    u32 crashFlag;
    char crashFlagBuffer[sizeof(u32)] = {0};
    DolphinComm::DolphinAccessor::readFromRAM(crashAddress, crashFlagBuffer, sizeof(u32), true);
    std::memcpy(&crashFlag, crashFlagBuffer, sizeof(u32));

    if (crashFlag == 1) {
      // if this is 1 then myExceptionCallback ran
      std::cout << "Game crashed!" <<  std::endl;
      return 1;
    }
    return 0;
  }

  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
}