// src/gui/contactwidget.cpp
#include "contactwidget.h"
#include "ui_contactwidget.h"
#include "src/utils/logger.h"
#include <QTreeWidgetItem>

ContactWidget::ContactWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ContactWidget)
{
    ui->setupUi(this);

    // 设置联系人树
    setupContactTree();

    // 连接信号槽
    connect(ui->treeWidgetContacts, &QTreeWidget::itemClicked,
            this, &ContactWidget::on_treeWidgetContacts_itemClicked);
    connect(ui->lineEditSearch, &QLineEdit::textChanged,
            this, &ContactWidget::on_lineEditSearch_textChanged);
    connect(ui->btnAddContact, &QToolButton::clicked,
            this, &ContactWidget::on_btnAddContact_clicked);
    connect(ui->btnCreateGroup, &QToolButton::clicked,
            this, &ContactWidget::on_btnCreateGroup_clicked);

    LOG_DEBUG("ContactWidget", "ContactWidget initialized");
}

ContactWidget::~ContactWidget()
{
    delete ui;
}

void ContactWidget::setUserInfo(const UserInfo& userInfo)
{
    m_currentUser = userInfo;

    // 更新UI
    ui->userNameLabel->setText(userInfo.nickname.isEmpty() ? userInfo.username : userInfo.nickname);

    // 更新状态
    QString statusText;
    switch (userInfo.status) {
    case UserStatus_Online:
        statusText = "在线";
        break;
    case UserStatus_Offline:
        statusText = "离线";
        break;
    case UserStatus_Busy:
        statusText = "忙碌";
        break;
    case UserStatus_Away:
        statusText = "离开";
        break;
    default:
        statusText = "未知";
    }

    ui->userStatusLabel->setText(statusText);

    LOG_DEBUG("ContactWidget", QString("User info set: %1").arg(userInfo.username));
}

void ContactWidget::updateContactList(const QList<ContactInfo>& contacts)
{
    // 清空树
    ui->treeWidgetContacts->clear();
    m_contactItems.clear();

    // 按分组组织联系人
    QMap<QString, QTreeWidgetItem*> groupItems;

    for (const ContactInfo& contact : contacts) {
        QString group = contact.group.isEmpty() ? "默认分组" : contact.group;

        // 获取或创建分组项
        QTreeWidgetItem* groupItem = nullptr;
        if (groupItems.contains(group)) {
            groupItem = groupItems[group];
        } else {
            groupItem = new QTreeWidgetItem(ui->treeWidgetContacts);
            groupItem->setText(0, group);
            groupItem->setIcon(0, QIcon(":/images/contacts.png"));
            groupItem->setExpanded(true);
            groupItems[group] = groupItem;
        }

        // 创建联系人项
        QTreeWidgetItem* contactItem = new QTreeWidgetItem(groupItem);
        contactItem->setText(0, contact.name);

        // 设置状态图标
        QString statusIcon;
        switch (contact.status) {
        case UserStatus_Online:
            statusIcon = ":/images/online.png";
            break;
        case UserStatus_Offline:
            statusIcon = ":/images/offline.png";
            break;
        case UserStatus_Busy:
            statusIcon = ":/images/busy.png";
            break;
        case UserStatus_Away:
            statusIcon = ":/images/away.png";
            break;
        default:
            statusIcon = ":/images/offline.png";
        }

        contactItem->setIcon(0, QIcon(statusIcon));

        // 如果有未读消息，显示数量
        if (contact.unreadCount > 0) {
            contactItem->setText(0, QString("%1 (%2)").arg(contact.name).arg(contact.unreadCount));
        }

        // 存储联系人ID
        contactItem->setData(0, Qt::UserRole, contact.id);

        // 保存到映射
        m_contactItems[contact.id] = contactItem;
    }

    LOG_DEBUG("ContactWidget", QString("%1 contacts updated").arg(contacts.size()));
}

void ContactWidget::updateUserStatus(int userId, UserStatus status)
{
    if (m_contactItems.contains(userId)) {
        QTreeWidgetItem* item = m_contactItems[userId];

        QString statusIcon;
        switch (status) {
        case UserStatus_Online:
            statusIcon = ":/images/online.png";
            break;
        case UserStatus_Offline:
            statusIcon = ":/images/offline.png";
            break;
        case UserStatus_Busy:
            statusIcon = ":/images/busy.png";
            break;
        case UserStatus_Away:
            statusIcon = ":/images/away.png";
            break;
        default:
            statusIcon = ":/images/offline.png";
        }

        item->setIcon(0, QIcon(statusIcon));

        LOG_DEBUG("ContactWidget", QString("User status updated: ID=%1, status=%2")
                                       .arg(userId).arg(static_cast<int>(status)));
    }
}

void ContactWidget::onContactAdded(const ContactInfo& contact)
{
    // 这里可以添加单个联系人
    QList<ContactInfo> contacts = {contact};
    updateContactList(contacts);
}

void ContactWidget::onContactRemoved(int contactId)
{
    if (m_contactItems.contains(contactId)) {
        QTreeWidgetItem* item = m_contactItems[contactId];
        delete item;
        m_contactItems.remove(contactId);

        LOG_DEBUG("ContactWidget", QString("Contact removed: ID=%1").arg(contactId));
    }
}

void ContactWidget::onUserStatusUpdated(const UserStatusUpdate& update)
{
    updateUserStatus(update.userId, update.status);
}

void ContactWidget::on_treeWidgetContacts_itemClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);

    // 检查是否是联系人项（不是分组项）
    if (item->parent() != nullptr) {
        int contactId = item->data(0, Qt::UserRole).toInt();
        emit contactSelected(contactId);

        LOG_DEBUG("ContactWidget", QString("Contact selected: ID=%1").arg(contactId));
    }
}

void ContactWidget::on_lineEditSearch_textChanged(const QString& text)
{
    emit searchTextChanged(text);

    // 简单的搜索过滤
    for (int i = 0; i < ui->treeWidgetContacts->topLevelItemCount(); ++i) {
        QTreeWidgetItem* groupItem = ui->treeWidgetContacts->topLevelItem(i);

        for (int j = 0; j < groupItem->childCount(); ++j) {
            QTreeWidgetItem* contactItem = groupItem->child(j);
            QString contactName = contactItem->text(0);

            if (text.isEmpty() || contactName.contains(text, Qt::CaseInsensitive)) {
                contactItem->setHidden(false);
            } else {
                contactItem->setHidden(true);
            }
        }
    }
}

void ContactWidget::on_btnAddContact_clicked()
{
    emit addContactRequested();
    LOG_DEBUG("ContactWidget", "Add contact button clicked");
}

void ContactWidget::on_btnCreateGroup_clicked()
{
    emit createGroupRequested();
    LOG_DEBUG("ContactWidget", "Create group button clicked");
}

void ContactWidget::setupContactTree()
{
    // 设置树控件属性
    ui->treeWidgetContacts->setHeaderHidden(true);
    ui->treeWidgetContacts->setIndentation(10);

    // 设置列宽
    ui->treeWidgetContacts->setColumnWidth(0, 250);
}
