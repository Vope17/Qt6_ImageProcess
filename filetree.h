#ifndef FILETREE_H
#define FILETREE_H

#include <QDialog>
#include <QTreeWidget>
namespace Ui {
class fileTree;
}

class fileTree : public QDialog
{
    Q_OBJECT

public:
    explicit fileTree(QWidget *parent = nullptr);
    ~fileTree();
    QTreeWidget *GetTreeWidget();
private:
    Ui::fileTree *ui;

public slots:
    void AddFileToTree(const QString name, const QString path);

};

#endif // FILETREE_H
