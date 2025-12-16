#include "database/DatabaseManager.h"
#include "gui/LoginWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Inicializar base de datos
  DatabaseManager::instance().initDB();

  LoginWindow loginWindow;
  loginWindow.show();

  return app.exec();
}
