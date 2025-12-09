#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QComboBox> // Widget de lista desplegable (para seleccionar el Rol: Alumno/Tutor)
#include <QLabel> // Widget para mostrar texto estático (etiquetas como "Usuario:")
#include <QLineEdit> // Widget para campos de entrada de texto de una línea (usuario y contraseña)
#include <QPushButton> // Widget para botones clicables ("Registrarse")
#include <QVBoxLayout> // Gestor de diseño que organiza los elementos verticalmente
#include <QWidget>     // Clase base de todos los objetos de la interfaz gráfica

class RegisterWindow : public QWidget {
  Q_OBJECT

public:
  explicit RegisterWindow(QWidget *parent = nullptr);

private slots:
  void onRegisterClicked();

private:
  QLineEdit *usernameEdit;
  QLineEdit *passwordEdit;
  QLineEdit *nameEdit;
  QLineEdit *degreeEdit;
  QLineEdit *nationalityEdit;
  QComboBox *roleCombo;
  QPushButton *registerButton;
  QLabel *statusLabel;
};

#endif // REGISTERWINDOW_H
