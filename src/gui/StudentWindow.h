#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

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
