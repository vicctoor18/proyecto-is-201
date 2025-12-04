#include "TutorWindow.h"
#include "LoginWindow.h"
#include <QTimer>

TutorWindow::TutorWindow(int tutorId, QWidget *parent)
    : QWidget(parent), tutorId(tutorId), currentStudentId(-1) {
  setWindowTitle("Panel de Tutor");
  resize(1000, 700); // Increased size

  QHBoxLayout *mainLayout = new QHBoxLayout(this);

  // Left side: Student List
  QVBoxLayout *leftLayout = new QVBoxLayout();
  leftLayout->addWidget(new QLabel("Mis Alumnos:"));
  studentList = new QListWidget(this);
  leftLayout->addWidget(studentList);

  logoutButton = new QPushButton("Cerrar Sesion", this);
  leftLayout->addWidget(logoutButton);
  connect(logoutButton, &QPushButton::clicked, this,
          &TutorWindow::onLogoutClicked);

  mainLayout->addLayout(leftLayout, 1);

  // Right side: Chat
  QVBoxLayout *rightLayout = new QVBoxLayout();
  chatLabel = new QLabel("Seleccione un alumno para chatear", this);
  rightLayout->addWidget(chatLabel);

  chatDisplay = new QTextEdit(this);
  chatDisplay->setReadOnly(true);
  rightLayout->addWidget(chatDisplay);

  QHBoxLayout *inputLayout = new QHBoxLayout();
  messageInput = new QLineEdit(this);
  sendButton = new QPushButton("Enviar", this);
  inputLayout->addWidget(messageInput);
  inputLayout->addWidget(sendButton);
  rightLayout->addLayout(inputLayout);

  mainLayout->addLayout(rightLayout, 2);

  connect(studentList, &QListWidget::itemClicked, this,
          &TutorWindow::onStudentSelected);
  connect(sendButton, &QPushButton::clicked, this, &TutorWindow::onSendMessage);

  loadStudents();

  // Auto-refresh chat every 2 seconds
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &TutorWindow::refreshChat);
  timer->start(2000);
}

void TutorWindow::loadStudents() {
  students = DatabaseManager::instance().getStudentsForTutor(tutorId);
  studentList->clear();
  for (const auto &student : students) {
    QListWidgetItem *item = new QListWidgetItem(
        student.name + " (" + student.degree + ")", studentList);
    item->setData(Qt::UserRole, student.id);
  }
}

void TutorWindow::onStudentSelected(QListWidgetItem *item) {
  currentStudentId = item->data(Qt::UserRole).toInt();
  chatLabel->setText("Chat con: " + item->text());
  loadChat();
}

void TutorWindow::loadChat() {
  if (currentStudentId == -1)
    return;

  auto messages =
      DatabaseManager::instance().getMessages(tutorId, currentStudentId);
  QString html;
  for (const auto &msg : messages) {
    QString color = (msg.senderId == tutorId) ? "blue" : "green";
    QString sender = (msg.senderId == tutorId) ? "Yo" : "Alumno";
    html += "<div style='color:" + color + "'><b>" + sender + ":</b> " +
            msg.content + "</div>";
  }
  chatDisplay->setHtml(html);
  chatDisplay->moveCursor(QTextCursor::End);
}

void TutorWindow::refreshChat() { loadChat(); }

void TutorWindow::onSendMessage() {
  QString content = messageInput->text();
  if (content.isEmpty() || currentStudentId == -1)
    return;

  if (DatabaseManager::instance().sendMessage(tutorId, currentStudentId,
                                              content)) {
    messageInput->clear();
    loadChat();
  }
}

void TutorWindow::onLogoutClicked() {
  LoginWindow *loginWindow = new LoginWindow();
  loginWindow->show();
  this->close();
}
