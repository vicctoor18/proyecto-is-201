#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

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
