#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H // Hace que si se ha leido este archivo una vez, no se
                         // lea de nuevo

#include <QComboBox> // Crear listas desplegables
#include <QLabel> //Incluye las librerías que hacen falta para montar la página de registro
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class RegisterWindow : public QWidget {
Q_OBJECT // Hace que la clase sea interactiva (que cuando se pulsa el botón, se
         // registre el usuario)

    public : explicit RegisterWindow(QWidget *parent = nullptr);

private slots: // Slots son funciones que se ejecutan cuando ocurre un evento
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
