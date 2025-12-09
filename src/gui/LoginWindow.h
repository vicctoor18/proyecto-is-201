#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class LoginWindow : public QWidget {
  Q_OBJECT

public:
  explicit LoginWindow(QWidget *parent = nullptr);

private slots:
  void onLoginClicked();
  void onRegisterClicked();

private:
  QLineEdit *usernameEdit;
  QLineEdit *passwordEdit;
  QPushButton *loginButton;
  QPushButton *registerButton;
  QLabel *statusLabel;
};

#endif // LOGINWINDOW_H
