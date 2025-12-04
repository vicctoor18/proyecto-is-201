#include "database/DatabaseManager.h"
#include "gui/LoginWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Initialize database
  DatabaseManager::instance().initDB();

  LoginWindow loginWindow;
  loginWindow.show();

  return app.exec();
}
