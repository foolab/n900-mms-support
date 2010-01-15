#include "gconf.hh"
#include <gconf/gconf-client.h>
#include <QDebug>
#include <QStringList>

GConf::GConf() {
  g_type_init();

  m_client = gconf_client_get_default();
}

GConf::~GConf() {
  g_object_unref(m_client);
  m_client = 0;
}

bool GConf::removeDir(const QString& dir) {
  if (!dirExists(dir)) {
    return true;
  }

  return gconf_client_recursive_unset(m_client, dir.toUtf8().data(), GCONF_UNSET_INCLUDING_SCHEMA_NAMES, NULL) == TRUE;
}

bool GConf::dirExists(const QString& dir) {
  return gconf_client_dir_exists(m_client, dir.toUtf8().data(), NULL) == TRUE;
}

bool GConf::getBool(const QString& key) {
  return gconf_client_get_bool(m_client, key.toUtf8().data(), NULL) == TRUE;
}

void GConf::setBool(const QString& key, bool value) {
  gconf_client_set_bool(m_client, key.toUtf8().data(), value == true ? TRUE : FALSE, NULL);
}

QString GConf::getString(const QString& key) {
  gchar *v = gconf_client_get_string(m_client, key.toUtf8().data(), NULL);
  if (!v) {
    return QString();
  }

  QString val = v;

  g_free(v);

  return val;
}

void GConf::setString(const QString& key, const QString& value) {
  gconf_client_set_string(m_client, key.toUtf8().data(), value.toUtf8().data(), NULL);
}

int GConf::getInt(const QString& key) {
  return gconf_client_get_int(m_client, key.toUtf8().data(), NULL);
}

void GConf::setInt(const QString& key, int value) {
  gconf_client_set_int(m_client, key.toUtf8().data(), value, NULL);
}

QString GConf::escape(const QString& text) {
  char *t = gconf_escape_key(text.toUtf8().data(), text.size());

  QString ret(t);

  g_free(t);

  return ret;
}

QString GConf::unescape(const QString& text) {
  char *t = gconf_unescape_key(text.toUtf8().data(), text.size());

  QString ret(t);

  g_free(t);

  return ret;
}

QStringList GConf::allDirs(const QString& root) {
  QStringList result;
  GSList *list = gconf_client_all_dirs(m_client, root.toUtf8().data(), NULL);
  if (!list) {
    return result;
  }

  for (guint x = 0; x < g_slist_length(list); x++) {
    result.append((gchar *)g_slist_nth(list, x)->data);
    g_free(g_slist_nth(list, x)->data);
  }

  g_slist_free(list);

  return result;
}
