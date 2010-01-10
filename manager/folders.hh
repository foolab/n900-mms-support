#ifndef FOLDERS_HH
#define FOLDERS_HH

#include "folder.hh"

class Incoming : public Folder {
  Q_OBJECT
  //  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.Manager.Incoming");

public:
  Incoming(QObject *parent = 0) : Folder("incoming", parent) {}
};

class Outgoing : public Folder {
  Q_OBJECT
  //  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.Manager.Outgoing");

public:
  Outgoing(QObject *parent = 0) : Folder("outgoing", parent) {}
};

class Inbox : public Folder {
  Q_OBJECT
  //  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.Manager.Inbox");

public:
  Inbox(QObject *parent = 0) : Folder("inbox", parent) {}
};

class Sent : public Folder {
  Q_OBJECT
  //  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.Manager.Sent");

public:
  Sent(QObject *parent = 0) : Folder("sent", parent) {}
};

class Reports : public Folder {
  Q_OBJECT
  //  Q_CLASSINFO("D-Bus Interface", "org.foolab.MMS.Manager.Reports");

public:
  Reports(QObject *parent = 0) : Folder("reports", parent) {}
};

#endif /* FOLDERS_HH */
