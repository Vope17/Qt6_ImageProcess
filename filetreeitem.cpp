#include "filetreeitem.h"

fileTreeItem::fileTreeItem(QTreeWidget *view, const QString &name,
                           const QString &path, int type)
    :QTreeWidgetItem(view, type), _root(this), _path(path), _name(name),_preItem(nullptr), _nextItem(nullptr)
{

}
fileTreeItem::fileTreeItem(QTreeWidgetItem *parent, const QString &name,
                           const QString &path, QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent, type), _root(root), _path(path), _name(name), _preItem(nullptr), _nextItem(nullptr)
{

}

const QString &fileTreeItem::GetPath()
{
    return _path;
}

QTreeWidgetItem *fileTreeItem::GetRoot()
{
    return _root;
}

void fileTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    _preItem = item;
}

void fileTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    _nextItem = item;
}

fileTreeItem *fileTreeItem::GetPreItem()
{
    return dynamic_cast<fileTreeItem*>(_preItem);
}

fileTreeItem *fileTreeItem::GetNextItem()
{
    return dynamic_cast<fileTreeItem*>(_nextItem);
}
