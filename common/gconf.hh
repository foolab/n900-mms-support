#ifndef GCONF_HH
#define GCONF_HH

#include <QString>
#include <QVariant>

typedef struct _GConfClient GConfClient;

class GConf {
public:
  GConf();
  ~GConf();

  bool removeDir(const QString& dir);
  bool dirExists(const QString& dir);

  int getInt(const QString& key);
  void setInt(const QString& key, int value);

  bool getBool(const QString& key);
  void setBool(const QString& key, bool value);

  QString getString(const QString& key);
  void setString(const QString& key, const QString& value);

  QString escape(const QString& text);
  QString unescape(const QString& text);

  QStringList allDirs(const QString& root);

private:
  GConfClient *m_client;
};

#endif /* GCONF_HH */
