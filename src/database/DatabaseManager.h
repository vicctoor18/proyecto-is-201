#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDebug> // Permite imprimir mensajes en la consola para depuración (qDebug())
#include <QSqlDatabase> // Gestiona la conexión con la base de datos SQLite
#include <QSqlError>    // Permite obtener información sobre errores de SQL
#include <QSqlQuery> // Permite ejecutar consultas SQL (SELECT, INSERT, UPDATE...)
#include <QString>   // Manejo de cadenas de texto
#include <QVariant> // Tipo de dato genérico de Qt, usado para leer valores de la base de datos
#include <vector>

class DatabaseManager {
private:
  DatabaseManager(); // Constructor privado. Se accede a traves del metodo instance()
  QSqlDatabase db; // Conexión entre la base de datos y el programa
public:
  static DatabaseManager &instance(); // Devuelve la instancia de la base de datos, para poder usarse en varias ventanas
  bool connect(); // Conecta la base de datos. Si ya lo esta, devuelve true.
  void initDB(); // Inicializa la base de datos. Asigna una base de datos default si esta vacía (nueva base de datos)

  // Creación y login del usuario
  bool login(const QString &username, const QString &password, QString &role,int &userId);
  bool registerUser(const QString &username, const QString &password,const QString &role, const QString &name, const QString &degree, const QString &nationality);

  // Asignación del tutor
  int getAssignedTutor(int studentId);
  bool assignTutor(int studentId, const QString &degree,const QString &nationality);

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
};

#endif // DATABASEMANAGER_H
