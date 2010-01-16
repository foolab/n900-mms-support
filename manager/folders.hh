#ifndef FOLDERS_HH
#define FOLDERS_HH

#include "folder.hh"

class Incoming : public Folder {
  Q_OBJECT

public:
  Incoming(QObject *parent = 0) : Folder("incoming", parent) {}
};

class Outgoing : public Folder {
  Q_OBJECT

public:
  Outgoing(QObject *parent = 0) : Folder("outgoing", parent) {}
};

class Inbox : public Folder {
  Q_OBJECT

public:
  Inbox(QObject *parent = 0) : Folder("inbox", parent) {}
};

class Sent : public Folder {
  Q_OBJECT

public:
  Sent(QObject *parent = 0) : Folder("sent", parent) {}
};

class Reports : public Folder {
  Q_OBJECT

public:
  Reports(QObject *parent = 0) : Folder("reports", parent) {}
};

#endif /* FOLDERS_HH */
