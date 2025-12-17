#include <QApplication>
#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
  QApplication app(argc,
                   argv); // Requerido para QSqlDatabase y otras clases de Qt
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
