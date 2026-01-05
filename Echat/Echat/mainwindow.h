#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QListWidgetItem>
#include "frienditem.h"
#include "bubble.h"
#include <QScrollBar>
#include <QWheelEvent>
#include <QDebug>
#include "noticeitem.h"
#include "datamanager.h"
#include "userinfopopup.h"
#include <QList>
#include "addfriend.h"
#include "custommessagebox.h"
#include <QMenu>
#include "registerdialog.h"
#include "groupmember.h"

namespace Ui {
class mainWindow;
}


class mainWindow : public QWidget
{
    Q_OBJECT

signals:
    void initializationComplete();  // 初始化完成信号

public:
    explicit mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

    void user_info_set();
    void setup_chat_session();
    void ui_setup();
    void setup_bubbleItem();
    void createBubblesWithWidth(int parent_width);
    void updateBubbleSizes();
    void setup_friendlist();
    void setup_noticelist();
    void onDataReach();
    void add_noticelist();
    void update_noticelist(int choice);
    void update2_noticelist(FRIEND_ADD_ASK &friend_add_ask);
    void update2_noticelist(GROUP_ADD_ASK &group_add_ask);
    void add2_noticelist(FRIEND_ADD_ASK &friend_add_ask);
    void add2_noticelist(GROUP_ADD_ASK &group_add_ask);
    void update_friendlist(USER_INFO &user_info);
    void add_friendlist(USER_INFO &user_info);
    void setupToolBarRedDot();
    void switchToSession(const QString &sessionId);
    void loadSessionChat(const QString &sessionId);
    void createSessionBubbles(const QString &sessionId, UPDATE_CHAT_MSG &msg);
    void loadBubbles(const QString &sessionId);
    void loadMessages(CHAT_MSG *chat_msg, int sesssionType);
    void clearChatBubbles();
    void update_session_item(const QString &sessionId);
    void add_chatsession(QString friendAccount, int sessionType);
    void pull_history_msg();
    void setupAutoHistoryLoading();
    void onChatScroll(int value);
    void load_more_history_msg();
    void insert_SessionBubbles(const QString &sessionId);
    void updateSessionChat(const QString &sessionId);
    void set_chatBtn_style();
    void setup_chat();
    void setup_friend_info();
    void show_chat();
    void show_friend_info();
    void setup_group_info();
    void show_group_info();
    void update_grouplist(GROUP_INFO &group_info);
    void add_grouplist(GROUP_INFO &group_info);
    void setup_users_in_group_list();
    void show_users_in_group_list();
    void create_chat_session(const QString& friendAccount, int sessionType, const QString& sessionId);
    bool fetch_group_members(const QString& groupAccount);
    void check_pending_sessions(const QString& groupAccount);
    void set_users_in_group_list(QString sessionId);
    void clear_users_in_group_list();


    // 判断是否需要显示时间
    bool shouldShowTime(const QString &sessionId, qint64 currentTime);
    // 创建时间显示item
    void createTimeItem(qint64 timestamp, bool insertAtTop = false);
    qint64 getCurrentTopTimestamp();

    void showMultiOptionMenu();

private slots:
    void onAddFriend();
    void onAddGroup();
    void onCreateGroup();

    void on_chatBtn_clicked();
    void on_addressBtn_clicked();
    void on_friend_switch_btn_clicked();
    void on_group_switch_btn_clicked();
    void on_friend_switch_btn_2_clicked();
    void on_group_switch_btn_2_clicked();
    void on_noticeBtn_clicked();
    void on_headimgBtn_clicked();
    void initializeMainWindow();
    void on_addBtn_clicked();

    void on_friend_list_itemClicked(QListWidgetItem *item);

    void on_sendBtn_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_open_chat_btn_clicked();

    void on_msgEdit_textChanged();

    void on_group_list_itemClicked(QListWidgetItem *item);

    void on_addBtn_2_clicked();

    void on_addBtn_3_clicked();

    void on_open_chat_btn_2_clicked();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::mainWindow *ui;
    DataManager& m_dataManager;
    UserInfoPopup user_infoPopup;
    AddFriend m_addFriend;
    UserBasicInfo m_userInfo;
    QList<FRIEND_ADD_ASK> m_friend_add_asks;
    QList<GROUP_ADD_ASK> m_group_add_asks;
    QLabel *noticeRedDot = nullptr;
    QLabel *msgRedDot = nullptr;
    QMap<QString, friendItem*> m_sessions = {};
    QMap<QString, QListWidgetItem*> m_sessionItems = {};  // 会话ID -> 列表项
    QMap<QString, QList<ChatMessage>> m_chat_msgs = {};
    QString m_currentSessionId = 0;  // 当前会话ID

    // 等待群成员信息的会话队列
    QSet<QString> m_pendingGroupSessions;


    int m_msgs_count;                         // 当前会话窗口的消息数量
    bool m_hasMoreHistory = true;            // 每个会话是否还有更多历史消息
    int m_currentPage = 1;                     // 当前会话的当前页码
    bool m_loadingHistory = false;             // 防止重复请求
    int m_scrollThreshold = 1;               // 触发加载的滚动阈值
    int m_lastScrollValue = 0;  // 记录上一次滚动位置
    bool m_isSendingMessage = false;  // 添加发送状态标志

    // 时间显示相关
    static const qint64 TIME_DISPLAY_INTERVAL = 1 * 60; // 1分钟间隔显示时间
    QMap<QString, qint64> m_lastMessageTime; // 记录每个会话最后一条消息的时间

};

#endif // MAINWINDOW_H
