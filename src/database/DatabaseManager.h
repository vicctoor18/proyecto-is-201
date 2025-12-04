#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <vector>

class DatabaseManager {
public:
  static DatabaseManager &instance();
  bool connect();
  void initDB();

  // User Management
  bool login(const QString &username, const QString &password, QString &role,
             int &userId);
  bool registerUser(const QString &username, const QString &password,
                    const QString &role, const QString &name,
                    const QString &degree, const QString &nationality);

  // Tutor Assignment
  int getAssignedTutor(int studentId);
  bool assignTutor(int studentId, const QString &degree,
                   const QString &nationality);

  // Chat
  bool sendMessage(int senderId, int receiverId, const QString &content);
  struct Message {
    int id;
    int senderId;
    QString content;
    QString timestamp;
  };
  std::vector<Message> getMessages(int userId1, int userId2);

  // Helpers
  struct UserInfo {
    int id;
    QString name;
    QString degree;
    QString nationality;
  };
  UserInfo getUserInfo(int userId);
  std::vector<UserInfo> getStudentsForTutor(int tutorId);

private:
  DatabaseManager();
  QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
