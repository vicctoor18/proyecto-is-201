#ifndef TUTORWINDOW_H
#define TUTORWINDOW_H

#include "../database/DatabaseManager.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

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
