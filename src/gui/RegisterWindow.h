#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H //Hace que si se ha leido este archivo una vez, no se lea de nuevo

#include <QComboBox>
#include <QLabel> //Incluye las librerías que hacen falta para montar la página de registro
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class RegisterWindow : public QWidget {
  Q_OBJECT //Hace que la clase sea interactiva (que cuando se pulsa el botón, se registre el usuario)

public:
  explicit RegisterWindow(QWidget *parent = nullptr);

private slots:
  void onRegisterClicked(); //Los slots en Qt reaccionan a algo. Hacen que la función esté conectada a una señal. Solo se llama desde dentro

private:
  QLineEdit *usernameEdit;
  QLineEdit *passwordEdit;
  QLineEdit *nameEdit;         //Lista de variables
  QLineEdit *degreeEdit;
  QLineEdit *nationalityEdit;
  QComboBox *roleCombo;
  QPushButton *registerButton;
  QLabel *statusLabel;
};

#endif // REGISTERWINDOW_H
