#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QLabel>      // Widget para mostrar texto estático
#include <QLineEdit>   // Widget para campos de entrada de texto de una línea
#include <QPushButton> // Widget para botones clicables
#include <QTextEdit> // Widget de texto multilínea (para mostrar el chat completo)
#include <QVBoxLayout> // Gestor de diseño que organiza los elementos verticalmente
#include <QWidget>     // Clase base de todos los objetos de la interfaz gráfica

class StudentWindow : public QWidget {
  Q_OBJECT

public:
  explicit StudentWindow(int studentId, QWidget *parent = nullptr);

private slots:
  void onSendMessage();
  void refreshChat();
  void onLogoutClicked();

private:
  int studentId;
  int tutorId;

  QLabel *tutorInfoLabel;
  QTextEdit *chatDisplay;
  QLineEdit *messageInput;
  QPushButton *sendButton;
  QPushButton *logoutButton;

  void loadTutorInfo();
  void loadChat();
};

#endif // STUDENTWINDOW_H
