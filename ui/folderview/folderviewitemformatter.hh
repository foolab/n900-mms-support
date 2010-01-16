#ifndef FOLDER_VIEW_ITEM_FORMATTER_HH
#define FOLDER_VIEW_ITEM_FORMATTER_HH

#include <QString>

class MmsMessage;

class FolderViewItemFormatter {
public:
  virtual QString format(const MmsMessage& message) = 0;
};

#endif /* FOLDER_VIEW_ITEM_FORMATTER_HH */
