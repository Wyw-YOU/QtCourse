#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include "src/utils/global.h"

namespace Ui {
class ContactWidget;
}

class ContactWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContactWidget(QWidget *parent = nullptr);
    ~ContactWidget();

    void setUserInfo(const UserInfo &userInfo);
    void updateContactList(const QList<ContactInfo> &contacts);
    void updateUserStatus(int userId, UserStatus status);

signals:
    void contactSelected(int contactId);
    void addContactRequested();
    void createGroupRequested();
    void searchTextChanged(const QString &text);

public slots:
    void onContactAdded(const ContactInfo &contact);
    void onContactRemoved(int contactId);
    void onUserStatusUpdated(const UserStatusUpdate &update);

private slots:
    void on_treeWidgetContacts_itemClicked(QTreeWidgetItem *item, int column);
    void on_lineEditSearch_textChanged(const QString &text);
    void on_btnAddContact_clicked();
    void on_btnCreateGroup_clicked();

private:
    Ui::ContactWidget *ui;
    void setupContactTree();
    void addContactToTree(const ContactInfo &contact);
    void updateContactInTree(const ContactInfo &contact);
    void removeContactFromTree(int contactId);
    QTreeWidgetItem* findContactItem(int contactId);

    QMap<int, QTreeWidgetItem*> m_contactItems;
    UserInfo m_currentUser;
};

#endif // CONTACTWIDGET_H
