#ifndef TUTORWINDOW_H
#define TUTORWINDOW_H

#include "../database/DatabaseManager.h" // Para acceder a estructuras de datos como UserInfo
#include <QHBoxLayout> // Gestor de diseño que organiza los elementos horizontalmente
#include <QLabel>      // Widget para mostrar texto estático
#include <QLineEdit> // Widget para campos de entrada de texto de una línea
#include <QListWidget> // Widget para mostrar una lista de elementos seleccionables (lista de alumnos)
#include <QPushButton> // Widget para botones clicables
#include <QTextEdit>   // Widget de texto multilínea
#include <QVBoxLayout> // Gestor de diseño que organiza los elementos verticalmente
#include <QWidget>     // Clase base de todos los objetos de la interfaz gráfica
#include <vector>      // Para almacenar la lista de alumnos en memoria

class TutorWindow : public QWidget {
  Q_OBJECT

public:
  explicit TutorWindow(int tutorId, QWidget *parent = nullptr);

private slots:
  void onStudentSelected(QListWidgetItem *item);
  void onSendMessage();
  void refreshChat();
  void onLogoutClicked();

private:
  int tutorId;
  int currentStudentId;

  QListWidget *studentList;
  QTextEdit *chatDisplay;
  QLineEdit *messageInput;
  QPushButton *sendButton;
  QPushButton *logoutButton;
  QLabel *chatLabel;

  std::vector<DatabaseManager::UserInfo> students;

  void loadStudents();
  void loadChat();
};

#endif // TUTORWINDOW_H
