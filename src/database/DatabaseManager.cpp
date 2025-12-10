#include "DatabaseManager.h"
#include <QCoreApplication> // Para acceder a la ruta de la aplicacion
#include <QDateTime>        // Para obtener la fecha y hora actual
#include <QDebug>           // Para imprimir mensajes en la consola
#include <QDir>             // Para manejar directorios
#include <QStandardPaths>   // Para obtener la ruta de la aplicacion

DatabaseManager &DatabaseManager::instance() {
  static DatabaseManager instance;
  return instance;
}

DatabaseManager::DatabaseManager() {
  db = QSqlDatabase::addDatabase("QSQLITE");

  // Base de datos en "build/data"
  QString dataDir = QCoreApplication::applicationDirPath() + "/data";
  QDir dir(dataDir);
  if (!dir.exists()) {
    dir.mkpath(".");
  }

  QString dbPath = dataDir + "/sistema_tutorizacion.db";
  db.setDatabaseName(dbPath);
  qDebug() << "Path de la base de datos:" << dbPath;
}

bool DatabaseManager::connect() {
  if (db.isOpen())
    return true;

  if (!db.open()) {
    qDebug() << "Error: No se ha podido conectar la base de datos"
             << db.lastError().text();
    return false;
  }
  qDebug() << "Base de datos conectada correctamente";
  return true;
}

void DatabaseManager::initDB() {
  if (!connect()) {
    qDebug() << "Failed to connect to DB during initialization";
    return;
  }

  QSqlQuery query;

  // Tabla de usuarios
  query.exec("CREATE TABLE IF NOT EXISTS users ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "username TEXT UNIQUE, "
             "password TEXT, "
             "role TEXT, "
             "name TEXT, "
             "degree TEXT, "
             "nationality TEXT)");

  // Relación tutor-alumno
  query.exec("CREATE TABLE IF NOT EXISTS tutor_student ("
             "tutor_id INTEGER, "
             "student_id INTEGER, "
             "PRIMARY KEY (tutor_id, student_id), "
             "FOREIGN KEY(tutor_id) REFERENCES users(id), "
             "FOREIGN KEY(student_id) REFERENCES users(id))");

  // Mensajes
  query.exec("CREATE TABLE IF NOT EXISTS messages ("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "sender_id INTEGER, "
             "receiver_id INTEGER, "
             "content TEXT, "
             "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
             "FOREIGN KEY(sender_id) REFERENCES users(id), "
             "FOREIGN KEY(receiver_id) REFERENCES users(id))");

  // Información predeterminada si la base de datos esta vacia
  query.exec("SELECT count(*) FROM users");
  if (query.next() && query.value(0).toInt() == 0) {
    // Tutores
    registerUser("alberto", "1234", "tutor", "Alberto", "Informatica",
                 "Espana");
    registerUser("roberto", "1234", "tutor", "Roberto", "Mecanica", "Cuba");
    registerUser("adela", "1234", "tutor", "Adela", "Electrica", "Espana");
    registerUser("manuela", "1234", "tutor", "Manuela", "Forestal", "Colombia");

    // Estudiantes
    registerUser("javier", "1234", "alumno", "Javier", "Informatica", "Cuba");
    registerUser("julian", "1234", "alumno", "Julian", "Informatica", "Espana");
    registerUser("david", "1234", "alumno", "David", "Informatica", "Espana");
    registerUser("victor", "1234", "alumno", "Victor", "Informatica", "Espana");
    registerUser("juan", "1234", "alumno", "Juan", "Mecanica", "Colombia");
    registerUser("alvaro", "1234", "alumno", "Alvaro", "Forestal", "Espana");
    registerUser("antonio", "1234", "alumno", "Antonio", "Electrica", "Espana");
  }

  // Asignación de tutor
  query.exec("SELECT id, degree, nationality FROM users WHERE role = 'alumno'");
  while (query.next()) {
    assignTutor(query.value(0).toInt(), query.value(1).toString(),
                query.value(2).toString());
  }
}

bool DatabaseManager::login(const QString &username, const QString &password,
                            QString &role, int &userId) {
  QSqlQuery query;
  query.prepare("SELECT id, role FROM users WHERE username = :username AND "
                "password = :password");
  query.bindValue(":username", username);
  query.bindValue(":password", password);

  if (query.exec() && query.next()) {
    userId = query.value(0).toInt();
    role = query.value(1).toString();
    return true;
  }
  return false;
}

bool DatabaseManager::registerUser(const QString &username,
                                   const QString &password, const QString &role,
                                   const QString &name, const QString &degree,
                                   const QString &nationality) {
  // Verificar si el usuario ya existe
  QSqlQuery checkQuery;
  checkQuery.prepare("SELECT id FROM users WHERE username = :u");
  checkQuery.bindValue(":u", username);
  if (checkQuery.exec() && checkQuery.next()) {
    qDebug() << "User already exists:" << username;
    return false;
  }

  QSqlQuery query;
  query.prepare("INSERT INTO users (username, password, role, name, degree, "
                "nationality) VALUES (:u, :p, :r, :n, :d, :nat)");
  query.bindValue(":u", username);
  query.bindValue(":p", password);
  query.bindValue(":r", role);
  query.bindValue(":n", name);
  query.bindValue(":d", degree);
  query.bindValue(":nat", nationality);
  bool success = query.exec();
  if (success && role == "alumno") {
    int userId = query.lastInsertId().toInt();
    assignTutor(userId, degree, nationality);
  }
  return success;
}

int DatabaseManager::getAssignedTutor(int studentId) {
  QSqlQuery query;
  query.prepare("SELECT tutor_id FROM tutor_student WHERE student_id = :id");
  query.bindValue(":id", studentId);
  if (query.exec() && query.next()) {
    return query.value(0).toInt();
  }
  return -1;
}

bool DatabaseManager::assignTutor(int studentId, const QString &degree,
                                  const QString &nationality) {
  // Tutor es inmutable, no se puede cambiar.
  if (getAssignedTutor(studentId) != -1) {
    return false;
  }

  struct TutorCandidate {
    int id;
    int matchScore; // 2 puntos si cumple ambas, 1 si cumple 1
    int studentCount;
  };

  std::vector<TutorCandidate> candidates;

  QSqlQuery query;
  // Buscar tutores que cumplan una o dos condiciones
  query.prepare("SELECT id, degree, nationality FROM users WHERE role = "
                "'tutor' AND (degree = :d OR nationality = :n)");
  query.bindValue(":d", degree);
  query.bindValue(":n", nationality);

  if (!query.exec()) {
    qDebug() << "Error searching for tutors:" << query.lastError().text();
    return false;
  }

  // Si no hay coincidencias, buscar TODOS los tutores
  if (!query.next()) {
    query.prepare(
        "SELECT id, degree, nationality FROM users WHERE role = 'tutor'");
    if (!query.exec()) {
      qDebug() << "Error searching for all tutors:" << query.lastError().text();
      return false;
    }
  } else {
    // Si hubo resultados, volver al principio
    query.seek(-1);
  }

  while (query.next()) {
    int id = query.value(0).toInt();
    QString tDegree = query.value(1).toString();
    QString tNationality = query.value(2).toString();

    int score = 0;
    if (tDegree == degree)
      score++;
    if (tNationality == nationality)
      score++;

    // Conseguir numero de alumnos de este tutor.
    QSqlQuery countQuery;
    countQuery.prepare(
        "SELECT COUNT(*) FROM tutor_student WHERE tutor_id = :tid");
    countQuery.bindValue(":tid", id);
    int count = 0;
    if (countQuery.exec() && countQuery.next()) {
      count = countQuery.value(0).toInt();
    }

    candidates.push_back({id, score, count});
  }

  if (candidates.empty()) {
    qDebug() << "No tutors found in the system.";
    return false;
  }

  // Algoritmo de seleccion de tutor
  TutorCandidate *best = nullptr;

  for (auto &candidate : candidates) {
    if (!best) {
      best = &candidate;
      continue;
    }

    // Comprueba cantidad de alumnos. Si el tutor A tiene la mitad o más de
    // alumnos que el B, se elegirá el B para no sobrecargar a A.
    bool bestIsOverloaded =
        best->studentCount > (2 * candidate.studentCount + 1);
    bool candidateIsOverloaded =
        candidate.studentCount > (2 * best->studentCount + 1);

    if (candidate.matchScore > best->matchScore) {
      // Candidato tiene más puntos que el mejor actual.
      if (!candidateIsOverloaded) {
        best = &candidate;
      }
    } else if (candidate.matchScore == best->matchScore) {
      // Mismos puntos, se elige el que menos estudiantes a cargo tenga.
      if (candidate.studentCount < best->studentCount) {
        best = &candidate;
      }
    } else {
      // Candidato tiene menos puntos
      if (bestIsOverloaded) {
        best = &candidate;
      }
    }
  }

  // Añadir enlace tutor-alumno.
  if (best) {
    query.prepare(
        "INSERT INTO tutor_student (tutor_id, student_id) VALUES (:tid, :sid)");
    query.bindValue(":tid", best->id);
    query.bindValue(":sid", studentId);
    if (query.exec()) {
      qDebug() << "Assigned tutor" << best->id << "to student" << studentId;
      return true;
    }
  }

  return false;
}

bool DatabaseManager::sendMessage(int senderId, int receiverId,
                                  const QString &content) {
  QSqlQuery query;
  query.prepare("INSERT INTO messages (sender_id, receiver_id, content) VALUES "
                "(:sid, :rid, :c)");
  query.bindValue(":sid", senderId);
  query.bindValue(":rid", receiverId);
  query.bindValue(":c", content);
  return query.exec();
}

std::vector<DatabaseManager::Message>
DatabaseManager::getMessages(int userId1, int userId2) {
  std::vector<Message> messages;
  QSqlQuery query;
  query.prepare("SELECT id, sender_id, content, timestamp FROM messages "
                "WHERE (sender_id = :u1 AND receiver_id = :u2) OR (sender_id = "
                ":u2 AND receiver_id = :u1) "
                "ORDER BY timestamp ASC");
  query.bindValue(":u1", userId1);
  query.bindValue(":u2", userId2);

  if (query.exec()) {
    while (query.next()) {
      messages.push_back({query.value(0).toInt(), query.value(1).toInt(),
                          query.value(2).toString(),
                          query.value(3).toString()});
    }
  }
  return messages;
}

DatabaseManager::UserInfo DatabaseManager::getUserInfo(int userId) {
  QSqlQuery query;
  query.prepare(
      "SELECT id, name, degree, nationality FROM users WHERE id = :id");
  query.bindValue(":id", userId);
  if (query.exec() && query.next()) {
    return {query.value(0).toInt(), query.value(1).toString(),
            query.value(2).toString(), query.value(3).toString()};
  }
  return {-1, "", "", ""};
}

std::vector<DatabaseManager::UserInfo>
DatabaseManager::getStudentsForTutor(int tutorId) {
  std::vector<UserInfo> students;
  QSqlQuery query;
  query.prepare("SELECT u.id, u.name, u.degree, u.nationality FROM users u "
                "JOIN tutor_student ts ON u.id = ts.student_id "
                "WHERE ts.tutor_id = :tid");
  query.bindValue(":tid", tutorId);

  if (query.exec()) {
    while (query.next()) {
      students.push_back({query.value(0).toInt(), query.value(1).toString(),
                          query.value(2).toString(),
                          query.value(3).toString()});
    }
  }
  return students;
}
