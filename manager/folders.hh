#ifndef FOLDERS_HH
#define FOLDERS_HH

#include "folder.hh"

class Incoming : public Folder {
  Q_OBJECT

public:
  Incoming(QObject *parent = 0) : Folder("incoming", parent) {}
};

class Outbox : public Folder {
  Q_OBJECT

public:
  Outbox(QObject *parent = 0) : Folder("outbox", parent) {}
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

class Pushed : public Folder {
  Q_OBJECT

public:
  Pushed(QObject *parent = 0) : Folder("pushed", parent) {}
};

class Queue : public Folder {
  Q_OBJECT

public:
  Queue(QObject *parent = 0) : Folder("queue", parent) {}
};

#endif /* FOLDERS_HH */
