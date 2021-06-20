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
  bool quitFlag{false};
  CLIapp.add_option("--q", quitFlag, "Quits the program");
  CLI11_PARSE(CLIapp, argc, argv);

  if (quitFlag)
  {
    DolphinComm::DolphinAccessor::hook();
    char addressBuffer[4] = {0};
    std::cout << DolphinComm::DolphinAccessor::getEmuRAMAddressStart() << std::endl;
    u32 offset = Common::dolphinAddrToOffset(0x80000000, DolphinComm::DolphinAccessor::getMEM1ToMEM2Distance());
    DolphinComm::DolphinAccessor::readFromRAM(0x450580, addressBuffer, 4, false);
    std::cout << addressBuffer << std::endl;
    if (addressBuffer[3] == 1) {
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