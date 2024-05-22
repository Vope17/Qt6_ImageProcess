#ifndef FILETREEITEM_H
#define FILETREEITEM_H

#include <QTreeWidgetItem>

class fileTreeItem : public QTreeWidgetItem
{
public:
    fileTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type = Type);
    fileTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type = Type);

    const QString &GetPath();
    QTreeWidgetItem *GetRoot();
    void SetPreItem(QTreeWidgetItem *item);
    void SetNextItem(QTreeWidgetItem *item);
    fileTreeItem *GetPreItem();
    fileTreeItem *GetNextItem();
    fileTreeItem *GetFirstImgChild();
    fileTreeItem *GetLastImgChild();
private:
    QString _path;
    QString _name;
    QTreeWidgetItem *_root;
    QTreeWidgetItem *_preItem;
    QTreeWidgetItem *_nextItem;

};

#endif // FILETREEITEM_H
