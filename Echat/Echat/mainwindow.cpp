#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "networkmanager.h"
#include <vector>

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWindow),
    m_dataManager(DataManager::instance())
{
    ui->setupUi(this);
    // 设置窗口最小尺寸
    setMinimumSize(964, 600);

    // 安装事件过滤器
    ui->listWidget->viewport()->installEventFilter(this);
    ui->chatWidget->viewport()->installEventFilter(this);

    connect(m_dataManager.m_networkmanager, &NetworkManager::DataReach, this, &mainWindow::onDataReach);

    setupAutoHistoryLoading();

    // 使用延迟初始化，确保所有组件都创建完成后再发出信号
    QTimer::singleShot(0, this, [this]() {
        initializeMainWindow();
    });
}

mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::initializeMainWindow()
{
    qDebug() << "开始延迟初始化主窗口...";
    m_userInfo = DataManager::instance().getCurrentUser();

    ui_setup();
//    on_chatBtn_clicked();
    set_chatBtn_style();
    ui->stackedWidget->setCurrentIndex(0);


//    setup_chat_session();


    setup_friendlist();
    setup_noticelist();

    user_info_set();

    pull_history_msg();

    qDebug() << "主窗口所有初始化完成，发出信号";

    // 现在发出初始化完成信号
    emit initializationComplete();
}

void mainWindow::onDataReach()
{
    while(true){
        MSG_HEADER msg_header;
        // 尝试读取消息头
        if (!m_dataManager.m_networkmanager->read_message(msg_header)) {
            qDebug() << "没有更多消息或读取消息头失败";
            break;
        }

//   m_dataManager.m_networkmanager->read_message(msg_header);
   qDebug() <<  "msg_length:"  << msg_header.msg_length;
   RESPONSE_MSG response_msg;
   USER_QUERY_RESPONSE_MSG user_query_response_msg;
   FRIEND_ASK_NOTICE_MSG *friend_ask_notice_msg = NULL;
   FRIEND_LIST_MSG *friend_list_msg = NULL;
   CHAT_MSG *chat_msg = NULL;

   GROUP_QUERY_RESPONSE_MSG group_query_response_msg;
   GROUP_ASK_NOTICE_MSG *group_ask_notice_msg = NULL;
   GROUP_LIST_MSG *group_list_msg = NULL;
   GROUP_MEMBER_QUERY_RESPONSE_MSG *users_in_group_list_msg  = NULL;

   int total_size = sizeof(MSG_HEADER) + msg_header.msg_length;
    QString session_id;
    QString groupAccount;

   switch(msg_header.msg_type)
   {
   case ADD_FRIEND_NOTICE:     //单个好友添加通知
       friend_ask_notice_msg = (FRIEND_ASK_NOTICE_MSG*)malloc(total_size);
       m_dataManager.m_networkmanager->read_add_friend_list(msg_header, friend_ask_notice_msg);
       m_dataManager.update_friend_add_asks(friend_ask_notice_msg->asks[0]);
       update2_noticelist(friend_ask_notice_msg->asks[0]);
       // 释放内存
       free(friend_ask_notice_msg);
       break;
   case USER_QUERY_RESPONSE:   //添加好友时查询用户响应
        m_dataManager.m_networkmanager->read_user_query_response(msg_header, user_query_response_msg);
        if (user_query_response_msg.success_flag == 0)
        {
            qDebug() <<  user_query_response_msg.response;
            m_addFriend.label_clear();
            // 使用自定义消息框
            CustomMessageBox::showWarning(
                &m_addFriend,  // 父窗口
                "查询失败",
                user_query_response_msg.response
            );
        }else{
            qDebug() << user_query_response_msg.response;
            m_addFriend.set_label(user_query_response_msg.user_info);
//            emit loginSuccess();
        }
       break;
   case FRIEND_ADD_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);

       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               &m_addFriend,  // 父窗口
               "添加失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;
           // 发送添加请求成功，更新
           CustomMessageBox::showInformation(
               &m_addFriend,  // 父窗口
               "添加成功",
               response_msg.response
           );
//           add_noticelist();
       }
       break;
   case FRIEND_ACCPET_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);
       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               this,  // 父窗口
               "接受好友申请失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;
       }
       break;
   case FRIEND_REJECT_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);
       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               this,  // 父窗口
               "拒绝好友申请失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;
       }
       break;
   case FRIEND_STATUS_NOTICE:
       friend_list_msg = (FRIEND_LIST_MSG*)malloc(total_size);
       m_dataManager.m_networkmanager->read_friend_list_msg(msg_header, friend_list_msg);
       m_dataManager.update_friendlist(friend_list_msg->friends[0]);
       update_friendlist(friend_list_msg->friends[0]);
       // 释放内存
       free(friend_list_msg);
       break;
   case SEND_CHAT_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);
       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               this,  // 父窗口
               "发送失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;

       }
       break;
   case CHAT_MSG_NOTICE:
       chat_msg = (CHAT_MSG*)malloc(total_size);
       m_dataManager.m_networkmanager->read_chat_msg(msg_header, chat_msg);
       loadMessages(chat_msg, 1);
       if(strcmp(m_userInfo.account.toUtf8().constData(), chat_msg->sender_account) == 0){
           session_id = QString::fromUtf8(chat_msg->receiver_account);
       }else{
           session_id = QString::fromUtf8(chat_msg->sender_account);
       }

       add_chatsession(session_id, 1);

       if(m_currentSessionId == session_id){     //如果正处于与发送消息好友的会话中
//            loadSessionChat(session_id);
            updateSessionChat(session_id);
       }else{
            update_session_item(session_id);
       }

       if(ui->stackedWidget->currentIndex() != 0){   // 如果不处于聊天会话页面
           msgRedDot->show();
       }
       break;

   case GROUP_QUERY_RESPONSE:   //添加好友时查询用户响应
        m_dataManager.m_networkmanager->read_group_query_response(msg_header, group_query_response_msg);
        if (group_query_response_msg.success_flag == 0)
        {
            qDebug() <<  group_query_response_msg.response;
            m_addFriend.label_clear();
            // 使用自定义消息框
            CustomMessageBox::showWarning(
                &m_addFriend,  // 父窗口
                "查询失败",
                group_query_response_msg.response
            );
        }else{
            qDebug() << group_query_response_msg.response;
            m_addFriend.set_label(group_query_response_msg.group_info);
//            emit loginSuccess();
        }
       break;

   case GROUP_ADD_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);

       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               &m_addFriend,  // 父窗口
               "添加失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;
           // 发送添加请求成功，更新
           CustomMessageBox::showInformation(
               &m_addFriend,  // 父窗口
               "添加成功",
               response_msg.response
           );
       }
       break;

   case GROUP_ACCPET_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);
       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               this,  // 父窗口
               "接受群聊申请失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;
       }
       break;

   case GROUP_REJECT_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);
       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               this,  // 父窗口
               "拒绝群聊申请失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;
       }
       break;

   case ADD_GROUP_NOTICE:     //单个群聊添加通知
       group_ask_notice_msg = (GROUP_ASK_NOTICE_MSG*)malloc(total_size);
       m_dataManager.m_networkmanager->read_add_group_list(msg_header, group_ask_notice_msg);
       m_dataManager.update_group_add_asks(group_ask_notice_msg->asks[0]);
       update2_noticelist(group_ask_notice_msg->asks[0]);
       // 释放内存
       free(group_ask_notice_msg);
       break;

   case GROUP_STATUS_NOTICE:
       qDebug() << "接收到群聊状态更新通知";
       group_list_msg = (GROUP_LIST_MSG*)malloc(total_size);
       m_dataManager.m_networkmanager->read_group_list_msg(msg_header, group_list_msg);
       m_dataManager.update_grouplist(group_list_msg->groups[0]);
       update_grouplist(group_list_msg->groups[0]);
       // 释放内存
       free(group_list_msg);
       break;

   case CREATE_GROUP_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);

       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               this,  // 父窗口
               "创建失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;
           CustomMessageBox::showInformation(this, "创建成功", "群聊创建成功");
       }
       break;

   case SEND_GROUP_CHAT_RESPONSE:
       m_dataManager.m_networkmanager->read_remain_message(msg_header, response_msg);
       if (response_msg.success_flag == 0)
       {
           qDebug() <<  response_msg.response;
           // 使用自定义消息框
           CustomMessageBox::showWarning(
               this,  // 父窗口
               "发送失败",
               response_msg.response
           );
       }else{
           qDebug() <<  response_msg.response;

       }
       break;

   case GROUP_CHAT_MSG_NOTICE:
       chat_msg = (CHAT_MSG*)malloc(total_size);
       m_dataManager.m_networkmanager->read_chat_msg(msg_header, chat_msg);
       loadMessages(chat_msg, 2);

       session_id = QString::fromUtf8(chat_msg->receiver_account);

       add_chatsession(session_id, 2);

       session_id.append("_g");
       if(m_currentSessionId == session_id){     //如果正处于与发送消息好友的会话中
//            loadSessionChat(session_id);
           qDebug() << "处于群聊中";
            updateSessionChat(session_id);
       }else{
            update_session_item(session_id);
       }

       if(ui->stackedWidget->currentIndex() != 0){   // 如果不处于聊天会话页面
           msgRedDot->show();
       }

       break;

   case GROUP_MEMBER_QUERY_RESPONSE:
       users_in_group_list_msg = (GROUP_MEMBER_QUERY_RESPONSE_MSG*)malloc(total_size);
       m_dataManager.m_networkmanager->read_users_in_group_list_msg(msg_header, users_in_group_list_msg);
       m_dataManager.update_users_in_group_list(users_in_group_list_msg);
       groupAccount = QString::fromUtf8(users_in_group_list_msg->group_account);
//       // 如果之前有因为等待群成员而暂停的会话添加，现在可以继续
//       check_pending_sessions(groupAccount);

       session_id = QString::fromUtf8(users_in_group_list_msg->group_account);
       session_id.append("_g");
       if(m_currentSessionId == session_id){     //如果正处于与发送消息好友的会话中
//            loadSessionChat(session_id);
           qDebug() << "处于群聊中";
           clear_users_in_group_list();
           set_users_in_group_list(session_id);
       }

       update_session_item(session_id);

       // 释放内存
       free(users_in_group_list_msg);
       break;
   default:
       break;
   }
 }
}

void mainWindow::user_info_set()
{
    QPixmap roundedAvatar = Business::getRoundedPixmap(m_userInfo.avatarPixmap, 14); // 12是圆角半径
    ui->headimgBtn->setIcon(QIcon(roundedAvatar));
    ui->headimgBtn->setIconSize(QSize(51, 51));
    ui->headimgBtn->setText("");
}

void mainWindow::ui_setup()
{
    ui->friendname_label->clear();
    ui->listWidget->setStyleSheet(
        "QListWidget::item:selected{background-color:rgba(200,200,200,240);}"
        "QListWidget"
        "{"
        "background-color: rgb(237, 237, 237);"
        "border-top: 1px solid #cccccc;"
        "border-right: 1px solid #cccccc;"
        "border-bottom: 1px solid #cccccc;"
        "border-left: 1px solid #cccccc;"
        "}"
        "QScrollBar:vertical {"
        "   background: transparent;"           // 滚动条背景透明
        "   width: 8px;"                       // 滚动条宽度
        "   margin: 0px;"
        "   border: none;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: rgba(150, 150, 150, 50);"  // 滑块颜色（半透明灰色）
        "   border-radius: 6px;"               // 滑块圆角
        "   height: 15px;"                 // 滑块最小高度
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background: rgba(120, 120, 120, 180);"  // 鼠标悬停时颜色
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "   border: none;"
        "   background: none;"                 // 上下箭头区域透明
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "   background: none;"                 // 滑块前后区域透明
        "}"
    );

    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->searchEdit->setStyleSheet(
        "QLineEdit {"
        "   padding-left: 35px;"  // 左侧内边距，让光标向右偏移
        "   border: 1px solid #ccc;"
        "   border-radius: 10px;"
        "   background-color: white;"
        "}"
      );

    ui->msgEdit->setStyleSheet(
        "QTextEdit{"
        "padding-left: 10px;"  // 左侧内边距，让光标向右偏移
        "border:none;"
        "background-color: rgb(237, 237, 237);"
        "}"
       );

    ui->chatWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->chatWidget->setFocusPolicy(Qt::NoFocus);
    // 设置编辑触发器为无，防止任何选择
    ui->chatWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    setupToolBarRedDot();
    setup_chat();
    setup_friend_info();
    setup_group_info();
    setup_users_in_group_list();
}

void mainWindow::setup_users_in_group_list()
{
    ui->horizontalLayout_5->setStretch(2, 0);
    ui->horizontalLayout_5->setStretch(3, 0);
    ui->group_member_label->hide();
    ui->users_in_group_list->hide();
}

void mainWindow::show_users_in_group_list()
{
    ui->horizontalLayout_5->setStretch(2, 2);
    ui->horizontalLayout_5->setStretch(3, 9);
    ui->group_member_label->show();
    ui->users_in_group_list->show();
//    QString group_member = "群聊成员  ";
//    group_member.append(QString::number(m_sessions[m_currentSessionId]->m_group_info.member_count));
//    ui->group_member_label->setText(group_member);
    // 使用实际的群成员数据而不是会话信息中的旧数据
    if (m_sessions.contains(m_currentSessionId)) {
        QString group_account = m_currentSessionId;
        if (group_account.endsWith("_g")) {
            group_account.chop(2);

            // 检查是否有群成员数据
            if (m_dataManager.m_groupMembers.contains(group_account)) {
                int actualMemberCount = m_dataManager.m_groupMembers[group_account].size();
                QString group_member = "群聊成员  ";
                group_member.append(QString::number(actualMemberCount));
                ui->group_member_label->setText(group_member);
                qDebug() << "显示群成员数量:" << actualMemberCount;
            } else {
                // 如果没有群成员数据，使用会话信息中的数量
                QString group_member = "群聊成员  ";
                group_member.append(QString::number(m_sessions[m_currentSessionId]->m_group_info.member_count));
                ui->group_member_label->setText(group_member);
                qDebug() << "使用会话信息中的群成员数量:" << m_sessions[m_currentSessionId]->m_group_info.member_count;
            }
        }
    }
}

void mainWindow::setup_chat()
{
    ui->friendname_label->hide();
    ui->filesend_Btn->hide();
    ui->imgsend_Btn->hide();
    ui->sendBtn->hide();
    ui->msgEdit->hide();
    ui->chatWidget->hide();
}


void mainWindow::show_chat()
{
    ui->friendname_label->show();
    ui->filesend_Btn->show();
    ui->imgsend_Btn->show();
    ui->sendBtn->show();
    ui->msgEdit->show();
    ui->chatWidget->show();
}

void mainWindow::setup_friend_info()
{
    ui->friendimg_label->hide();
    ui->friendname_label_2->hide();
    ui->friendaccount_label->hide();
    ui->open_chat_btn->hide();
    ui->delete_friend_btn->hide();
}

void mainWindow::show_friend_info()
{
    ui->friendimg_label->show();
    ui->friendname_label_2->show();
    ui->friendaccount_label->show();
    ui->open_chat_btn->show();
    ui->delete_friend_btn->show();
}

void mainWindow::setup_group_info()
{
    ui->groupimg_label->hide();
    ui->groupaccount_label->hide();
    ui->groupname_label->hide();
    ui->group_member_count_label->hide();
    ui->open_chat_btn_2->hide();
    ui->delete_friend_btn_2->hide();
}

void mainWindow::show_group_info()
{
    ui->groupimg_label->show();
    ui->groupaccount_label->show();
    ui->groupname_label->show();
    ui->group_member_count_label->show();
    ui->open_chat_btn_2->show();
    ui->delete_friend_btn_2->show();
}
// 为工具栏按钮添加小红点
void mainWindow::setupToolBarRedDot()
{
    // 为通知按钮添加小红点
    noticeRedDot = new QLabel(ui->noticeBtn);
  //    noticeRedDot->setText("5");
    noticeRedDot->setAlignment(Qt::AlignCenter);
    noticeRedDot->setFixedSize(15, 15);
    noticeRedDot->setStyleSheet(
        "QLabel {"
        "   background-color: #FF3B30;"
        "   color: white;"
        "   border-radius: 7px;"
        "   font-size: 10px;"
        "   font-weight: bold;"
        "}"
    );
    noticeRedDot->move(ui->noticeBtn->width() - 16, 5);
    noticeRedDot->hide();

    // 为消息按钮添加小红点
    msgRedDot = new QLabel(ui->chatBtn);
   //    noticeRedDot->setText("5");
    msgRedDot->setAlignment(Qt::AlignCenter);
    msgRedDot->setFixedSize(15, 15);
    msgRedDot->setStyleSheet(
        "QLabel {"
        "   background-color: #FF3B30;"
        "   color: white;"
        "   border-radius: 7px;"
        "   font-size: 10px;"
        "   font-weight: bold;"
        "}"
    );
    msgRedDot->move(ui->chatBtn->width() - 16, 5);
    msgRedDot->hide();
}


void mainWindow::setup_friendlist()
{
    for(int i = 0; i< m_dataManager.m_friends.size(); i++)
    {
        //创建好友列表项
        friendItem * friItem = new friendItem();
        friItem->setHeadImage(m_dataManager.m_friends[i]);

        friItem->setfriendlist_item(m_dataManager.m_friends[i]);

        //QListWidgetItem
        QListWidgetItem * m_Item = new QListWidgetItem();

        //设置item宽高
        m_Item->setSizeHint(QSize(230, 113));

        ui->friend_list->insertItem(0, m_Item);
        //将自定义的Qwidget--friItem, 设置为m_Item的小部件
        ui->friend_list->setItemWidget(m_Item, friItem);
    }

    for(int i = 0; i< m_dataManager.m_groups.size(); i++)
    {
        //创建群聊列表项
        friendItem * friItem = new friendItem();

        friItem->setgrouplist_item(m_dataManager.m_groups[i]);

        //QListWidgetItem
        QListWidgetItem * m_Item = new QListWidgetItem();

        //设置item宽高
        m_Item->setSizeHint(QSize(230, 113));

        ui->group_list->insertItem(0, m_Item);
        //将自定义的Qwidget--friItem, 设置为m_Item的小部件
        ui->group_list->setItemWidget(m_Item, friItem);
    }
}

void mainWindow::update_friendlist(USER_INFO &user_info)
{
    bool flag = false;
    for (int i = 0; i < ui->friend_list->count(); i++) {
        QListWidgetItem* item = ui->friend_list->item(i);
        if (!item) continue;
        friendItem *friItem = qobject_cast<friendItem*>(ui->friend_list->itemWidget(item));
        if(strcmp(friItem->m_friend_info.user_account, user_info.user_account) == 0)
        {
            friItem->setfriendlist_item(user_info);

            QString notice = "您的好友：";
            notice.append(user_info.user_name);
            notice.append("   已上线");
            if(user_info.status == 1){
                CustomMessageBox::showInformation(
                    this,  // 父窗口
                    "好友上线提醒",
                    notice
                );
            }
            flag = true;
            break;
        }
    }

    if(!flag){
        add_friendlist(user_info);
    }
}

void mainWindow::update_grouplist(GROUP_INFO &group_info)
{
    bool flag = false;
    for (int i = 0; i < ui->group_list->count(); i++) {
        QListWidgetItem* item = ui->group_list->item(i);
        if (!item) continue;
        friendItem *friItem = qobject_cast<friendItem*>(ui->group_list->itemWidget(item));
        if(strcmp(friItem->m_group_info.group_account, group_info.group_account) == 0)
        {
            friItem->setgrouplist_item(group_info);
            flag = true;
            break;
        }
    }

    if(!flag){
        add_grouplist(group_info);
    }
}

void mainWindow::add_friendlist(USER_INFO &user_info)
{
    friendItem * friItem = new friendItem();
    friItem->setHeadImage(user_info);

    friItem->setfriendlist_item(user_info);

    //QListWidgetItem
    QListWidgetItem * m_Item = new QListWidgetItem();

    //设置item宽高
    m_Item->setSizeHint(QSize(230, 113));

    // 插入到列表顶部
    ui->friend_list->insertItem(0, m_Item);
    ui->friend_list->setItemWidget(m_Item, friItem);
}

void mainWindow::add_grouplist(GROUP_INFO &group_info)
{
    friendItem * friItem = new friendItem();

    friItem->setgrouplist_item(group_info);

    //QListWidgetItem
    QListWidgetItem * m_Item = new QListWidgetItem();

    //设置item宽高
    m_Item->setSizeHint(QSize(230, 113));

    // 插入到列表顶部
    ui->group_list->insertItem(0, m_Item);
    ui->group_list->setItemWidget(m_Item, friItem);
}

void mainWindow::setup_noticelist()
{
    m_friend_add_asks = m_dataManager.getCurrentfriend_add_asks();

//    qDebug() << "size: " << m_friend_add_asks.size();
    for(int i = 0; i< m_friend_add_asks.size(); i++)
    {
        noticeitem * notice = new noticeitem();
        notice->setHeadImage(m_friend_add_asks[i], m_userInfo.account);
        notice->set_notice_text(m_friend_add_asks[i], m_userInfo.account);

        //QListWidgetItem
        QListWidgetItem * m_Item = new QListWidgetItem();

        //设置item宽高
        m_Item->setSizeHint(QSize(500, 113));

        // 插入到列表顶部
        ui->friend_notice_list->insertItem(0, m_Item);
        //将自定义的Qwidget--friItem, 设置为m_Item的小部件
        ui->friend_notice_list->setItemWidget(m_Item, notice);
    }

    m_group_add_asks = m_dataManager.getCurrentgroup_add_asks();

    for(int i = 0; i< m_group_add_asks.size(); i++)
    {
        noticeitem * notice = new noticeitem();

        notice->set_group_notice(m_group_add_asks[i], m_userInfo.account);

        //QListWidgetItem
        QListWidgetItem * m_Item = new QListWidgetItem();

        //设置item宽高
        m_Item->setSizeHint(QSize(500, 113));

        // 插入到列表顶部
        ui->group_notice_list->insertItem(0, m_Item);
        //将自定义的Qwidget--friItem, 设置为m_Item的小部件
        ui->group_notice_list->setItemWidget(m_Item, notice);
    }
}

//void mainWindow::add_noticelist()
//{
//    noticeitem * notice = new noticeitem();
//    notice->setHeadImage("border-radius:12; border-image:url(:/new/prefix1/HeadImages/1.png);");
//    notice->set_notice_text(*m_dataManager.m_friend_add_ask, m_userInfo.account);

//    //QListWidgetItem
//    QListWidgetItem * m_Item = new QListWidgetItem();

//    //设置item宽高
//    m_Item->setSizeHint(QSize(500, 113));

//    //将自定义的Qwidget--friItem, 设置为m_Item的小部件

//    // 插入到列表顶部
//    ui->friend_notice_list->insertItem(0, m_Item);
//    ui->friend_notice_list->setItemWidget(m_Item, notice);
//}

void mainWindow::add2_noticelist(FRIEND_ADD_ASK &friend_add_ask)
{
    noticeitem * notice = new noticeitem();
    notice->setHeadImage(friend_add_ask, m_userInfo.account);
    notice->set_notice_text(friend_add_ask, m_userInfo.account);

    //QListWidgetItem
    QListWidgetItem * m_Item = new QListWidgetItem();

    //设置item宽高
    m_Item->setSizeHint(QSize(500, 113));

    //将自定义的Qwidget--friItem, 设置为m_Item的小部件

    // 插入到列表顶部
    ui->friend_notice_list->insertItem(0, m_Item);
    ui->friend_notice_list->setItemWidget(m_Item, notice);
}

void mainWindow::add2_noticelist(GROUP_ADD_ASK &group_add_ask)
{
    noticeitem * notice = new noticeitem();
    notice->set_group_notice(group_add_ask, m_userInfo.account);

    //QListWidgetItem
    QListWidgetItem * m_Item = new QListWidgetItem();

    //设置item宽高
    m_Item->setSizeHint(QSize(500, 113));

    //将自定义的Qwidget--friItem, 设置为m_Item的小部件

    // 插入到列表顶部
    ui->group_notice_list->insertItem(0, m_Item);
    ui->group_notice_list->setItemWidget(m_Item, notice);
}


//void mainWindow::update_noticelist(int choice)
//{
//    for (int i = 0; i < ui->friend_notice_list->count(); i++) {
//        QListWidgetItem* item = ui->friend_notice_list->item(i);
//        if (!item) continue;
//        noticeitem* notice = qobject_cast<noticeitem*>(ui->friend_notice_list->itemWidget(item));
//        if (notice->m_choice != 0) {
//            notice->update_text(choice);
//            break;
//        }
//    }
//}

void mainWindow::update2_noticelist(FRIEND_ADD_ASK &friend_add_ask)
{
    qDebug() << "接收到的friend_status:" << friend_add_ask.friend_status;
    QString notice = "用户：";
    if(friend_add_ask.friend_status != 0){
        notice.append(friend_add_ask.friend_name);
        notice.append("(");
        notice.append(friend_add_ask.friend_acccount);
        notice.append(")");
        if(friend_add_ask.friend_status == 1){
            notice.append("   已同意您的好友请求！");
        }else if(friend_add_ask.friend_status == -1){
            notice.append("   已拒绝您的好友请求！");
        }

        for (int i = 0; i < ui->friend_notice_list->count(); i++) {
            QListWidgetItem* item = ui->friend_notice_list->item(i);
            if (!item) continue;
            noticeitem* notice = qobject_cast<noticeitem*>(ui->friend_notice_list->itemWidget(item));
            int a = strcmp(notice->m_friend_add_ask_info.user_account, friend_add_ask.user_account);
            int b = strcmp(notice->m_friend_add_ask_info.friend_acccount, friend_add_ask.friend_acccount);
            if (a==0 && b==0) {
                notice->set_notice_text(friend_add_ask, m_userInfo.account);
                break;
            }
        }

        if(strcmp(friend_add_ask.user_account, m_userInfo.account.toUtf8().constData()) == 0){
            CustomMessageBox::showInformation(
                this,  // 父窗口
                "好友添加提醒",
                notice
            );
         }
    }else{
        add2_noticelist(friend_add_ask);
        notice.append(friend_add_ask.user_name);
        notice.append("(");
        notice.append(friend_add_ask.user_account);
        notice.append(")");
        notice.append("   请求添加您为好友");

        if(strcmp(friend_add_ask.friend_acccount, m_userInfo.account.toUtf8().constData()) == 0){
            if(ui->stackedWidget->currentIndex() != 3){
                noticeRedDot->show();
            }
            CustomMessageBox::showInformation(
                this,  // 父窗口
                "好友添加提醒",
                notice
            );
        }
    }
}

void mainWindow::update2_noticelist(GROUP_ADD_ASK &group_add_ask)
{
    qDebug() << "接收到的group_status:" << group_add_ask.status;
    QString notice;
    if(group_add_ask.status != 0){
        notice = "群聊：";
        notice.append(group_add_ask.group_name);
        notice.append("(");
        notice.append(group_add_ask.group_acccount);
        notice.append(")");
        if(group_add_ask.status == 1){
            notice.append("   已通过您的加入群聊请求！");
        }else if(group_add_ask.status == -1){
            notice.append("   已拒绝您的加入群聊请求！");
        }

        for (int i = 0; i < ui->group_notice_list->count(); i++) {
            QListWidgetItem* item = ui->group_notice_list->item(i);
            if (!item) continue;
            noticeitem* notice = qobject_cast<noticeitem*>(ui->group_notice_list->itemWidget(item));
            int a = strcmp(notice->m_group_add_ask_info.user_account, group_add_ask.user_account);
            int b = strcmp(notice->m_group_add_ask_info.group_acccount, group_add_ask.group_acccount);
            if (a==0 && b==0) {
                notice->set_group_notice(group_add_ask, m_userInfo.account);
                break;
            }
        }

        if(strcmp(group_add_ask.user_account, m_userInfo.account.toUtf8().constData()) == 0){
            CustomMessageBox::showInformation(
                this,  // 父窗口
                "群聊添加提醒",
                notice
            );
         }
    }else{
        add2_noticelist(group_add_ask);
        notice = "用户：";
        notice.append(group_add_ask.user_account);
        notice.append("   请求加入群聊：");
        notice.append(group_add_ask.group_name);
        notice.append("(");
        notice.append(group_add_ask.group_acccount);
        notice.append(")");

        if(strcmp(group_add_ask.creator_account, m_userInfo.account.toUtf8().constData()) == 0){
            if(ui->stackedWidget->currentIndex() != 3){
                noticeRedDot->show();
            }
            CustomMessageBox::showInformation(
                this,  // 父窗口
                "群聊添加提醒",
                notice
            );
        }
    }
}


void mainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 延迟处理，确保布局已完成
    QTimer::singleShot(10, [this]() {
        updateBubbleSizes();
    });
}

void mainWindow::updateBubbleSizes()
{
    int parent_width = ui->groupBox->width();
//    qDebug() << "窗口大小变化，更新气泡宽度:" << parent_width;

    // 遍历所有气泡项并更新尺寸
    for (int i = 0; i < ui->chatWidget->count(); ++i) {
        QListWidgetItem* item = ui->chatWidget->item(i);
        QWidget* widget = ui->chatWidget->itemWidget(item);

        Bubble* bubble = qobject_cast<Bubble*>(widget);

        if (bubble) {
            // 更新气泡内容（重新计算尺寸）
            QString currentContent = bubble->getContent(); // 假设Bubble类有getContent方法
            bubble->setContent(currentContent, parent_width);
            item->setSizeHint(bubble->size());
        }
    }

    // 刷新列表显示
    ui->chatWidget->update();
}

// 实现事件过滤器
bool mainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);

        if (obj == ui->listWidget->viewport() || obj == ui->chatWidget->viewport()) {
            QAbstractScrollArea *scrollArea = nullptr;
            if (obj == ui->listWidget->viewport()) {
                scrollArea = ui->listWidget;
            } else if (obj == ui->chatWidget->viewport()) {
                scrollArea = ui->chatWidget;
            }

            if (scrollArea) {
                QScrollBar *vScrollBar = scrollArea->verticalScrollBar();
                int delta = wheelEvent->angleDelta().y();

                // 超细腻滚动：每次只滚动1像素
                int step = (delta > 0) ? -1 : 1;
                vScrollBar->setValue(vScrollBar->value() + step);

                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void mainWindow::on_friend_switch_btn_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}

void mainWindow::on_friend_switch_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void mainWindow::on_group_switch_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void mainWindow::on_group_switch_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void mainWindow::on_chatBtn_clicked()
{
    if(m_sessions.contains(m_currentSessionId) && m_sessions[m_currentSessionId]->m_session_info.sessionType == 2){
        show_users_in_group_list();
    }

    if(m_currentSessionId!=0){
        UPDATE_CHAT_MSG msg;
        for (ChatMessage &chat_msg : m_chat_msgs[m_currentSessionId]) {
            if(chat_msg.sender != m_userInfo.account && chat_msg.status != 1){
                chat_msg.status = 1;
                msg.chat_msgs.emplace_back(chat_msg.messageId);
            }
        }

        update_session_item(m_currentSessionId);

        // 使用单次定时器，确保在事件循环处理完添加的项目后再滚动
        QTimer::singleShot(0, this, [this]() {
            ui->chatWidget->scrollToBottom();
        });

        // 发送消息，将已读消息更新状态发送到服务器端更新
        if(msg.chat_msgs.size()>0){
            int total_size = 0;
            const char* data;
            if(m_sessions[m_currentSessionId]->m_session_info.sessionType == 1){
                data = Business::construct_update_chatmsg_message(&msg, total_size);
            }else{
                data = Business::construct_update_group_chatmsg_message(&msg, total_size);
            }
            m_dataManager.m_networkmanager->send_message(data, total_size);
            Business::free_message_buffer(data); // 发送完成后释放内存
        }
    }

    set_chatBtn_style();
    ui->stackedWidget->setCurrentIndex(0);

}

void mainWindow::set_chatBtn_style()
{
    msgRedDot->hide();

//    // 当切换回聊天页面时，更新阅读状态
//    if (!m_currentSessionId.isEmpty()) {
//        updateSessionChat(m_currentSessionId);
//    }

    ui->chatBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/chat_active.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
                "}"
               "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/chat_active.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
                "}"
                );

    ui->addressBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/address_list_unactive.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
                "}"
               "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/address_list_unactive.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
                "}"
                );

    ui->noticeBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/notice_unactive.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
            "}"
            "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/notice_unactive.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
            "}"
             );
}


void mainWindow::on_addressBtn_clicked()
{
    setup_users_in_group_list();
    ui->stackedWidget->setCurrentIndex(1);
    ui->addressBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/address_list_active.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
                "}"
               "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/address_list_active.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
                "}"
                );

    ui->chatBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/chat_unactive.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
                "}"
               "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/chat_unactive.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
                "}"
                );
    ui->noticeBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/notice_unactive.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
            "}"
            "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/notice_unactive.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
            "}"
             );
}

void mainWindow::on_noticeBtn_clicked()
{
    setup_users_in_group_list();
    noticeRedDot->hide();
    ui->stackedWidget->setCurrentIndex(3);
    ui->addressBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/address_list_unactive.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
                "}"
               "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/address_list_unactive.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
                "}"
                );

    ui->chatBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/chat_unactive.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
                "}"
               "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/chat_unactive.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
                "}"
                );

    ui->noticeBtn->setStyleSheet(
                "QPushButton{"
                "border-image: url(:/new/prefix1/icons/notice_active.png);"
                "background-color: rgba(255, 255, 255, 0);"
                "background-position: center;"
            "}"
            "QPushButton:hover {"
                "border-image: url(:/new/prefix1/icons/notice_active.png);"
                "background-color: rgba(200, 200, 200, 100);"
                "background-position: center;"
                "border-radius:12px;"
            "}"
             );
}

void mainWindow::on_headimgBtn_clicked()
{
    QString imagepath = "border-radius:12; border-image:url(:/new/prefix1/HeadImages/1.png);";
    qDebug() << "nickname:" << m_userInfo.nickname;
    user_infoPopup.set_userinfo(m_userInfo.avatarPixmap, m_userInfo.nickname, m_userInfo.account);
    user_infoPopup.showAtWidgetSide(ui->headimgBtn);

}

// 在实现文件中
void mainWindow::showMultiOptionMenu()
{
    QMenu *menu = new QMenu(this);

    // 设置菜单样式
    menu->setStyleSheet(
        "QMenu {"
        "   background-color: white;"
        "   border: 1px solid #dcdfe6;"
        "   border-radius: 4px;"
        "   padding: 5px;"
        "}"
        "QMenu::item {"
        "   padding: 8px 20px;"
        "   border-radius: 4px;"
        "}"
        "QMenu::item:selected {"
        "   background-color: #ecf5ff;"
        "}"
    );

    // 添加三个选项
    QAction *addFriendAction = menu->addAction("👤 添加好友");
    QAction *addGroupAction = menu->addAction("👥 添加群聊");
    QAction *createGroupAction = menu->addAction("➕ 创建群聊");

    // 连接信号
    connect(addFriendAction, &QAction::triggered, this, &mainWindow::onAddFriend);
    connect(addGroupAction, &QAction::triggered, this, &mainWindow::onAddGroup);
    connect(createGroupAction, &QAction::triggered, this, &mainWindow::onCreateGroup);

    // 显示菜单在按钮下方
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        menu->exec(button->mapToGlobal(QPoint(0, button->height())));
    } else {
        menu->exec(QCursor::pos());
    }

    // 菜单使用完后会自动删除
    menu->deleteLater();
}

void mainWindow::on_addBtn_clicked()
{
    showMultiOptionMenu();
}

void mainWindow::onAddFriend()
{
    m_addFriend.label_clear();
    m_addFriend.show();
    m_addFriend.m_flag = 0;
}

void mainWindow::onAddGroup()
{
    m_addFriend.label_clear();
    m_addFriend.show();
    m_addFriend.m_flag = 1;
}

void mainWindow::onCreateGroup()
{
    // 创建注册对话框实例
    RegisterDialog registerDialog(this);
    registerDialog.setModal(true);
    registerDialog.set_group_register();
    registerDialog.setWindowTitle("创建群聊");

    // 显示注册对话框并等待用户操作
    if (registerDialog.exec() == QDialog::Accepted) {
        // 获取注册信息
        QString name = registerDialog.m_name;
        QString account = registerDialog.m_account;
        QByteArray avatarData = registerDialog.m_avatarData;

        if (!name.isEmpty() && !account.isEmpty() ) {
            qDebug() << "注册";
            CREATE_GROUP_MSG msg;
            const char* data =  Business::construct_create_group_message(name, account, avatarData,  m_userInfo.account, &msg);
            m_dataManager.m_networkmanager->send_message(data, sizeof (CREATE_GROUP_MSG));
        }
    }
}

void mainWindow::on_friend_list_itemClicked(QListWidgetItem *item)
{
    if (!item) return;
    // 获取点击的 item 对应的自定义 widget
    friendItem *friItem = qobject_cast<friendItem*>(ui->friend_list->itemWidget(item));
    if (!friItem) return;
    QString friend_name = "昵称：";
    QString friend_account = "账号：";

    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(friItem->m_friend_info.avatar_data, friItem->m_friend_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);
    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->friendimg_label->width(), ui->friendimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 40); // 12是圆角半径
    ui->friendimg_label->setPixmap(roundedAvatar);

    ui->friendname_label_2->setText(friend_name.append(friItem->m_friend_info.user_name));
    ui->friendaccount_label->setText(friend_account.append(friItem->m_friend_info.user_account));

    show_friend_info();
}

void mainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if (!item) return;
    // 获取点击的 item 对应的自定义 widget
    friendItem *chat_session = qobject_cast<friendItem*>(ui->listWidget->itemWidget(item));
    if (!chat_session) return;

    QString clicked_sessionId = chat_session->m_session_info.sessionId;
    if(clicked_sessionId == m_currentSessionId){
        qDebug() << "正处于当前会话中...";
        return;
    }

    switchToSession(clicked_sessionId);
}


void mainWindow::on_sendBtn_clicked()
{
    int sessionType = m_sessions[m_currentSessionId]->m_session_info.sessionType;

    if (m_isSendingMessage) {
        return; // 防止重复发送
    }
    m_isSendingMessage = true; // 设置发送状态

    QString sending_msg = ui->msgEdit->toPlainText().trimmed();
    ui->msgEdit->clear();

    size_t content_length = sending_msg.toUtf8().size() + 1; // +1 for null terminator
    // 正确计算大小：offsetof获取content字段的偏移量
    size_t total_size = offsetof(CHAT_MSG, content) + content_length;

    CHAT_MSG *chat_msg = (CHAT_MSG *)malloc(total_size);
    if (!chat_msg) {
        qDebug() << "内存分配失败";
        return;
    }

    memset(chat_msg, 0, total_size);

    if(sessionType == 1){
        // 初始化消息头
        chat_msg->msg_header.msg_type = SEND_CHAT_MSG;  // 你定义的聊天消息类型
        strncpy(chat_msg->receiver_account, m_currentSessionId.toUtf8().constData(), ACCOUNT_SIZE-1);
    }else{
        chat_msg->msg_header.msg_type = SEND_GROUP_CHAT_MSG;  // 你定义的聊天消息类型
        QString group_account = m_currentSessionId;
        if(group_account.endsWith("_g")){
            group_account.chop(2);
        }
        strncpy(chat_msg->receiver_account, group_account.toUtf8().constData(), ACCOUNT_SIZE-1);
    }

    chat_msg->msg_header.msg_length = total_size - sizeof(MSG_HEADER);  // 关键：body长度
    chat_msg->msg_header.timestamp = (int)time(nullptr);
    chat_msg->msg_header.total_count = 0;  // 对于单条消息，通常为0或1

    // 安全拷贝
    strncpy(chat_msg->sender_account, m_userInfo.account.toUtf8().constData(), ACCOUNT_SIZE-1);
    strncpy(chat_msg->content, sending_msg.toUtf8().constData(), content_length-1);

    chat_msg->content_type = MSG_CHAT_TEXT;           // 文本消息
    chat_msg->read_status = 0;                        // 阅读状态
    chat_msg->file_size = content_length;

    loadMessages(chat_msg, sessionType);
//    loadSessionChat(m_currentSessionId);
    updateSessionChat(m_currentSessionId);

    const char* data =  Business::construct_chat_message(chat_msg);
    qDebug() << "开始发送聊天消息：" << chat_msg->content;
    if(m_dataManager.m_networkmanager->send_message(data, total_size)){
        qDebug() << "聊天消息发送成功";
    }else{
        qDebug() << "聊天消息发送失败";
    }

    free(chat_msg);

    // 延迟重置发送状态，确保UI更新完成
    QTimer::singleShot(500, [this]() {
        m_isSendingMessage = false;
    });
}

void mainWindow::on_open_chat_btn_clicked()
{
    // 获取纯净的账号（移除可能的前缀）
    QString friendAccount = ui->friendaccount_label->text();
    QString friendName = ui->friendname_label_2->text();

    // 移除前缀（如果存在）
    friendAccount = friendAccount.remove("账号：");
    friendName = friendName.remove("昵称：");

    if(friendAccount.isEmpty() || friendName.isEmpty()){
        return;
    }

    qDebug() <<  "friendAccount"  << friendAccount;
    if(m_sessions.contains(friendAccount)){
       qDebug() <<  "已经存在这个会话："  << friendAccount;
        switchToSession(friendAccount);
        return;
    }

    add_chatsession(friendAccount, 1);

    // 切换到该会话
    switchToSession(friendAccount);
}

void mainWindow::add_chatsession(QString friendAccount, int sessionType)
{
    QString sessionId = friendAccount;
    if(sessionType == 2){
        sessionId.append("_g");
    }

    if(m_sessions.contains(sessionId)){
        return;
    }

    // 创建会话的代码...
    create_chat_session(friendAccount, sessionType, sessionId);

    // 对于群聊类型，检查群成员信息是否已准备好
    if(sessionType == 2){
//        if(!m_dataManager.m_groupMembers.contains(friendAccount)) {
//            // 群成员信息未准备好，先添加到等待队列
////            m_pendingGroupSessions.insert(friendAccount);
//            qDebug() << "群成员信息未就绪，等待拉取:" << friendAccount;

        // 触发拉取群成员信息
        fetch_group_members(friendAccount);
//            return; // 暂时不创建会话
//        }
    }

}

void mainWindow::create_chat_session(const QString& friendAccount, int sessionType, const QString& sessionId)
{
    qDebug() << "create_chat_session";
    friendItem * chat_session = new friendItem();

    if(m_chat_msgs.contains(sessionId) && !m_chat_msgs[sessionId].isEmpty()){
        ChatMessage lastmessage = m_chat_msgs[sessionId].last();
        chat_session->m_session_info.lastMessage = lastmessage.content;
        chat_session->m_session_info.lastTime = lastmessage.timestamp;
        chat_session->m_session_info.sender = lastmessage.sender;
    }else{
        chat_session->m_session_info.lastMessage = "";
        chat_session->m_session_info.lastTime = 0;
        chat_session->m_session_info.sender = "";
    }

    if(sessionType == 1){
        for(int i = 0; i<m_dataManager.m_friends.size(); i++){
            USER_INFO user_info = m_dataManager.m_friends[i];
            if(strcmp(user_info.user_account, friendAccount.toUtf8().constData()) == 0){
                chat_session->m_friend_info = user_info;
            }
        }

        chat_session->m_session_info.name = QString::fromUtf8(chat_session->m_friend_info.user_name);
    }else{
        for(int i = 0; i<m_dataManager.m_groups.size(); i++){
            GROUP_INFO group_info = m_dataManager.m_groups[i];
            if(strcmp(group_info.group_account, friendAccount.toUtf8().constData()) == 0){
                chat_session->m_group_info = group_info;
            }
        }

        chat_session->m_session_info.name = QString::fromUtf8(chat_session->m_group_info.group_name);
    }

    chat_session->m_session_info.sessionId = sessionId;
    chat_session->m_session_info.sessionType = sessionType;

    m_sessions[sessionId] = chat_session;

    if(sessionType == 1){
        //创建好友列表项
        chat_session->setHeadImage(chat_session->m_friend_info);
        chat_session->setSession_item();
    }else{
        chat_session->setHeadImage(chat_session->m_group_info);
        chat_session->setSession_item();
    }

    //QListWidgetItem
    QListWidgetItem * m_Item = new QListWidgetItem();

    //设置item宽高
    m_Item->setSizeHint(QSize(230, 113));

    //将自定义的Qwidget--friItem, 设置为m_Item的小部件
    ui->listWidget->insertItem(0, m_Item);
    ui->listWidget->setItemWidget(m_Item, chat_session);

    // 关键：保存到 m_sessionItems 映射
    m_sessionItems[sessionId] = m_Item;
}

// 新增函数：拉取群成员信息
bool mainWindow::fetch_group_members(const QString& groupAccount)
{
//    // 检查是否已经拉取过该群的成员信息
//    if(m_dataManager.m_groupMembers.contains(groupAccount)) {
//        qDebug() << "群成员信息已存在，无需重复拉取:" << groupAccount;
//        return true;
//    }

    // 构造请求消息
    ACCOUNT_QUERY_MSG msg;
    const char* data =  Business::construct_query_message(m_dataManager.getCurrentUser().account, groupAccount, &msg, 3);
    m_dataManager.m_networkmanager->send_message(data, sizeof (ACCOUNT_QUERY_MSG));

    qDebug() << "已发送群成员查询请求:" << groupAccount;

    // 等待响应（这里需要根据你的网络通信框架来处理）
    // 假设你有一个消息处理机制，当收到 GROUP_MEMBER_QUERY_RESPONSE 时会调用相应的处理函数

    return true; // 返回true表示请求已发送，实际成功与否在响应处理中判断
}

//// 可选：处理等待中的会话添加请求
//void mainWindow::check_pending_sessions(const QString& groupAccount)
//{
//    // 检查是否有等待该群成员信息的会话添加请求
//    // 你可以维护一个等待队列
//    if(m_pendingGroupSessions.contains(groupAccount)) {
//        // 重新尝试添加会话
//        add_chatsession(groupAccount, 2); // 2表示群聊类型
//        m_pendingGroupSessions.remove(groupAccount);
//    }
//}

void mainWindow::switchToSession(const QString &sessionId)
{
    if (!m_sessions.contains(sessionId)) {
        qWarning() << "会话不存在:" << sessionId;
        return;
    }

    // 重置该会话的时间记录
    m_lastMessageTime.remove(sessionId);

    // 在左侧列表中选中对应的会话项
    if (m_sessions.contains(sessionId)) {
        QListWidgetItem *item = m_sessionItems[sessionId];
        ui->listWidget->setCurrentItem(item);

//        // 确保该项可见（滚动到视图内）
//        ui->listWidget->scrollToItem(item, QAbstractItemView::PositionAtCenter);
    }

   m_currentSessionId = sessionId;

   if(m_sessions[sessionId]->m_session_info.sessionType == 1){
        setup_users_in_group_list();
   }else{
        show_users_in_group_list();
   }

   loadSessionChat(sessionId);

   QTimer::singleShot(1, [this]() {
       updateBubbleSizes();
   });

    // 切换到聊天界面
   set_chatBtn_style();
    ui->stackedWidget->setCurrentIndex(0);
//    on_chatBtn_clicked();
}

void mainWindow::loadSessionChat(const QString &sessionId)
{
    // 先删除现有的Bubble对象
    clearChatBubbles();
//    // 清空当前聊天内容
//    ui->chatWidget->clear();

    ui->friendname_label->setText(m_sessions[sessionId]->m_session_info.name);
    // 这里应该从数据管理器加载真实的聊天记录

    UPDATE_CHAT_MSG msg;

    QString group_sessionId = sessionId;
    group_sessionId.chop(2);
    if(m_sessions[sessionId]->m_session_info.sessionType == 2 && m_dataManager.m_groupMembers.contains(group_sessionId)){
        set_users_in_group_list(sessionId);
    }

    createSessionBubbles(sessionId, msg);

    update_session_item(sessionId);

    show_chat();
    // 使用单次定时器，确保在事件循环处理完添加的项目后再滚动
    QTimer::singleShot(0, this, [this]() {
        ui->chatWidget->scrollToBottom();
    });

    // 发送消息，将已读消息更新状态发送到服务器端更新
    if(msg.chat_msgs.size()>0){
        int total_size = 0;
        const char* data;
        if(m_sessions[sessionId]->m_session_info.sessionType == 1){
            data = Business::construct_update_chatmsg_message(&msg, total_size);
        }else{
            data = Business::construct_update_group_chatmsg_message(&msg, total_size);
        }
        m_dataManager.m_networkmanager->send_message(data, total_size);
        Business::free_message_buffer(data); // 发送完成后释放内存
    }
}

void mainWindow::updateSessionChat(const QString &sessionId)
{
    insert_SessionBubbles(sessionId);

    if(ui->stackedWidget->currentIndex() == 0){
        UPDATE_CHAT_MSG msg;
        ChatMessage &chat_msg = m_chat_msgs[sessionId].last();
        if(chat_msg.sender != m_userInfo.account && chat_msg.status!= 1){
            qDebug() << "更新消息状态,id:" << chat_msg.messageId;
            chat_msg.status = 1;
            msg.chat_msgs.emplace_back(chat_msg.messageId);
        }
        update_session_item(sessionId);

        // 使用单次定时器，确保在事件循环处理完添加的项目后再滚动
        QTimer::singleShot(0, this, [this]() {
            ui->chatWidget->scrollToBottom();
        });


        // 发送消息，将已读消息更新状态发送到服务器端更新
        if(msg.chat_msgs.size()>0){
            int total_size = 0;
            const char* data;
            if(m_sessions[sessionId]->m_session_info.sessionType == 1){
                data = Business::construct_update_chatmsg_message(&msg, total_size);
            }else{
                qDebug() << "发送更新群组消息,消息id：" << chat_msg.messageId;
                data = Business::construct_update_group_chatmsg_message(&msg, total_size);
            }
            m_dataManager.m_networkmanager->send_message(data, total_size);
            qDebug() << "发送完成";
            Business::free_message_buffer(data); // 发送完成后释放内存
        }
    }
}

void mainWindow::update_session_item(const QString &sessionId)
{
    QListWidgetItem *item = m_sessionItems[sessionId];
    friendItem * chat_session = qobject_cast<friendItem*>(ui->listWidget->itemWidget(item));
    int unread_count = 0;
    // 添加空指针检查
    if (!chat_session) {
        qDebug() << "错误：chat_session 为空指针，sessionId:" << sessionId;
        return;
    }
    if(m_chat_msgs.contains(sessionId) && !m_chat_msgs[sessionId].isEmpty())
    {
        ChatMessage lastmessage = m_chat_msgs[sessionId].last();
        chat_session->m_session_info.lastMessage = lastmessage.content;
        chat_session->m_session_info.lastTime = lastmessage.timestamp;
        chat_session->m_session_info.sender = lastmessage.sender;
    }

    for (const ChatMessage &chat_msg : m_chat_msgs[sessionId]) {
        if(chat_msg.status == 0 && chat_msg.sender != m_userInfo.account)
        {
            unread_count++;
        }
    }

    chat_session->m_session_info.unreadCount = unread_count;
    chat_session->setSession_item();
}

void mainWindow::loadMessages(CHAT_MSG *chat_msg, int sessionType)
{
    // 模拟从数据源加载消息
    // 实际应该从 DataManager 或其他数据源获取
    ChatMessage chatmessage;
    chatmessage.sender = chat_msg->sender_account;
    chatmessage.content = chat_msg->content;
    chatmessage.receiver = chat_msg->receiver_account;
    chatmessage.contentType = chat_msg->content_type;
    chatmessage.fileSize = chat_msg->file_size;
    chatmessage.status = chat_msg->read_status;
    chatmessage.messageId = chat_msg->chat_msg_id;

    // 这行代码Debug了很长时间，最后发现是因为结构体定义进行了修改，但是Qt creator没有及时更新，需要先清理项目，再重新构建才行
    chatmessage.timestamp = chat_msg->msg_header.timestamp;  // 问题代码：导致程序卡死崩溃

    QString chat_session_id;
    if(sessionType == 1){
        if(strcmp(chat_msg->sender_account, m_userInfo.account.toUtf8().constData()) == 0){
            chat_session_id = chat_msg->receiver_account;
        }else{
            chat_session_id = chat_msg->sender_account;
        }
    }else{
        chat_session_id = chat_msg->receiver_account;
    }

    QString sessionId = chat_session_id;
    if(sessionType == 2){
        sessionId.append("_g");
    }

    if(!m_chat_msgs.contains(sessionId)){
        m_chat_msgs[sessionId] = QList<ChatMessage>{};
    }
    m_chat_msgs[sessionId].append(chatmessage);
}

void mainWindow::insert_SessionBubbles(const QString &sessionId)
{
    int sessionType = m_sessions[sessionId]->m_session_info.sessionType;

    if(!m_chat_msgs.contains(sessionId) || m_chat_msgs[sessionId].isEmpty())
    {
        return;
    }

    int parent_width = ui->groupBox->width();

    ChatMessage &chat_msg = m_chat_msgs[sessionId].last();
//    if(chat_msg.receiver == m_userInfo.account && chat_msg.status!= 1){
//        chat_msg.status = 1;
//        msg.chat_msgs.emplace_back(chat_msg.messageId);
//    }

    // 检查是否需要显示时间
    if (shouldShowTime(sessionId, chat_msg.timestamp)) {
        createTimeItem(chat_msg.timestamp, false);
    }

    Bubble *bubble = new Bubble();
    bubble->m_chat_msg = chat_msg;

    // ----------------------------------    需要改 ----------------------------------
    QString name;
    if(sessionType == 1){
        name = m_sessions[sessionId]->m_friend_info.user_name;
        if(chat_msg.sender == m_userInfo.account){
            bubble->setHeadImage(m_userInfo.avatarPixmap);
        }else{
            bubble->setHeadImage(m_sessions[sessionId]->m_friend_info);
        }
    }else{
        QString group_account = m_currentSessionId;
        if(group_account.endsWith("_g")){
            group_account.chop(2);
        }
        QVector<USER_INFO>users = m_dataManager.m_groupMembers[group_account];
        for(int i = 0; i < users.size(); i++){
            if(QString::fromUtf8(users[i].user_account) == chat_msg.sender)
            {
                name = QString::fromUtf8(users[i].user_name);
                if(chat_msg.sender == m_userInfo.account){
                    bubble->setHeadImage(m_userInfo.avatarPixmap);
                }else{
                    bubble->setHeadImage(users[i]);
                }
                break;
            }
        }
    }

    bubble->setBubble(parent_width, name);
    QListWidgetItem * m_Item = new QListWidgetItem(ui->chatWidget);
    //设置item宽高
    m_Item->setSizeHint(bubble->size());
    ui->chatWidget->setItemWidget(m_Item, bubble);
}

void mainWindow::createSessionBubbles(const QString &sessionId, UPDATE_CHAT_MSG &msg)
{
    if(!m_chat_msgs.contains(sessionId) || m_chat_msgs[sessionId].isEmpty())
    {
        return;
    }

    m_loadingHistory = true;
    m_currentPage = 1;
    m_hasMoreHistory = true;

    int parent_width = ui->groupBox->width();

    // 重置最后消息时间
    m_lastMessageTime.remove(sessionId);

    for (ChatMessage &chat_msg : m_chat_msgs[sessionId]) {
        if(chat_msg.sender != m_userInfo.account && chat_msg.status != 1){
            chat_msg.status = 1;
            msg.chat_msgs.emplace_back(chat_msg.messageId);
        }
    }

    m_msgs_count = m_chat_msgs[sessionId].size();

    // 用于时间间隔判断的变量
    qint64 lastTimestamp = -1;

    int startIndex = qMax(0, m_msgs_count - 10);
//    qDebug() << "startIndex:" << startIndex;
//    qDebug() << "endIndex:" << m_msgs_count;
    for(int i = startIndex; i < m_msgs_count; i++){
        ChatMessage &chat_msg = m_chat_msgs[sessionId][i];

        // 第一条消息总是显示时间，或者与上一条消息间隔超过5分钟
        if (lastTimestamp == -1 || (chat_msg.timestamp - lastTimestamp) > 300) {
            createTimeItem(chat_msg.timestamp, false); // 添加到列表底部
        }

        lastTimestamp = chat_msg.timestamp;

//        if(chat_msg.receiver == m_userInfo.account && chat_msg.status != 1){
//            chat_msg.status = 1;
//        }
        Bubble *bubble = new Bubble();
        bubble->m_chat_msg = chat_msg;

        //   ------------------------需要改 --------------------------------------------------
        QString name;
        if(m_sessions[sessionId]->m_session_info.sessionType == 1){
            name = m_sessions[sessionId]->m_friend_info.user_name;
            if(chat_msg.sender == m_userInfo.account){
                bubble->setHeadImage(m_userInfo.avatarPixmap);
            }else{
                bubble->setHeadImage(m_sessions[sessionId]->m_friend_info);
            }
        }else{
            QString group_account = sessionId;
            if(group_account.endsWith("_g")){
                group_account.chop(2);
            }
            QVector<USER_INFO>users = m_dataManager.m_groupMembers[group_account];
            for(int i = 0; i < users.size(); i++){
                if(QString::fromUtf8(users[i].user_account) == chat_msg.sender)
                {
                    name = QString::fromUtf8(users[i].user_name);
                    if(chat_msg.sender == m_userInfo.account){
                        bubble->setHeadImage(m_userInfo.avatarPixmap);
                    }else{
                        bubble->setHeadImage(users[i]);
                    }
                    break;
                }
            }
        }

        bubble->setBubble(parent_width, name);
        QListWidgetItem * m_Item = new QListWidgetItem(ui->chatWidget);
        //设置item宽高
        m_Item->setSizeHint(bubble->size());
        ui->chatWidget->setItemWidget(m_Item, bubble);
    }

    m_loadingHistory = false;
}

void mainWindow::set_users_in_group_list(QString sessionId)
{
    qDebug() << "开始设置群成员信息列表";
    QString group_account = sessionId;
    group_account.chop(2);
    qDebug() << "group_account:" << group_account;
    QVector<USER_INFO> users = m_dataManager.m_groupMembers[group_account];
    USER_INFO m_user_info;
    memset(&m_user_info, 0 ,sizeof(m_user_info));
    strcpy(m_user_info.user_account, m_userInfo.account.toUtf8().constData());
    strcpy(m_user_info.user_name, m_userInfo.nickname.toUtf8().constData());

    for(int i = 0; i< users.size(); i++)
    {
        if(strcmp(users[i].user_account, m_dataManager.getCurrentUser().account.toUtf8().constData())==0){
//            m_user_info = users[i];
            continue;
        }
        //创建好友列表项
        GroupMember * friItem = new GroupMember();
        friItem->set_users_in_group_list_item(users[i]);

        //QListWidgetItem
        QListWidgetItem * m_Item = new QListWidgetItem();

        //设置item宽高
        m_Item->setSizeHint(QSize(165, 60));

        ui->users_in_group_list->insertItem(0, m_Item);
        //将自定义的Qwidget--friItem, 设置为m_Item的小部件
        ui->users_in_group_list->setItemWidget(m_Item, friItem);
    }

    GroupMember * friItem = new GroupMember();

    friItem->set_users_in_group_list_item(m_user_info, m_userInfo.avatarPixmap);

    //QListWidgetItem
    QListWidgetItem * m_Item = new QListWidgetItem();

    //设置item宽高
    m_Item->setSizeHint(QSize(240, 60));

    ui->users_in_group_list->insertItem(0, m_Item);
    //将自定义的Qwidget--friItem, 设置为m_Item的小部件
    ui->users_in_group_list->setItemWidget(m_Item, friItem);

    QString group_member = "群聊成员  ";
    group_member.append(QString::number(users.size()));
    qDebug() << group_member;
//    ui->group_member_label->clear();
    ui->group_member_label->setText(group_member);
//    // 强制更新布局和重绘
//    ui->group_member_label->update();
//    ui->group_member_label->repaint();

//    ui->group_member_label->parentWidget()->update();
}


void mainWindow::clear_users_in_group_list()
{
    // 删除所有friItem对象
    for (int i = 0; i < ui->users_in_group_list->count(); ++i) {
        QListWidgetItem* item = ui->users_in_group_list->item(i);
        if (QWidget* widget = ui->users_in_group_list->itemWidget(item)) {
            delete widget;  // 删除friItem对象
        }
    }
    ui->users_in_group_list->clear();  // 删除QListWidgetItem
}

void mainWindow::clearChatBubbles()
{
    // 删除所有Bubble对象
    for (int i = 0; i < ui->chatWidget->count(); ++i) {
        QListWidgetItem* item = ui->chatWidget->item(i);
        if (QWidget* widget = ui->chatWidget->itemWidget(item)) {
            delete widget;  // 删除Bubble对象
        }
    }
    ui->chatWidget->clear();  // 删除QListWidgetItem

    clear_users_in_group_list();
}


void mainWindow::on_msgEdit_textChanged()
{
    // 获取去除空白字符后的文本
    QString msg_text = ui->msgEdit->toPlainText().trimmed();

    // 启用/禁用发送按钮
    if(!msg_text.isEmpty()){
        ui->sendBtn->setStyleSheet("QPushButton{border-radius:6px;background-color: rgb(1, 188, 250);color: rgb(255, 255, 255);}");
        ui->sendBtn->setEnabled(true);
    }else{
        ui->sendBtn->setStyleSheet("QPushButton{border-radius:6px;background-color: rgb(225, 225, 225);color: rgb(157, 157, 157);}");
        ui->sendBtn->setEnabled(false);
    }
}

void mainWindow::pull_history_msg()
{
    HISTORY_MSG_GET msg;
    const char* data =  Business::construct_HistroyMsgGet_message(m_dataManager.getCurrentUser().account, &msg);
    m_dataManager.m_networkmanager->send_message(data, sizeof (HISTORY_MSG_GET));
}


void mainWindow::load_more_history_msg()
{
    if (m_loadingHistory || !m_hasMoreHistory) {
        return;
    }
    m_loadingHistory = true;

    qDebug() << "拉取历史消息";

    int sessionType = m_sessions[m_currentSessionId]->m_session_info.sessionType;
    // 记录当前在视口顶部的第一个可见 item
    QListWidgetItem* topVisibleItem = ui->chatWidget->itemAt(0, 0);
    int originalTopIndex = topVisibleItem ? ui->chatWidget->row(topVisibleItem) : 0;
    qDebug() << "加载前 - 顶部item索引:" << originalTopIndex
             << "总item数:" << ui->chatWidget->count();

    int parent_width = ui->groupBox->width();

    QString friendName;
    if(sessionType == 1){
       friendName = m_sessions[m_currentSessionId]->m_friend_info.user_name;
    }else{
//        friendName = m_sessions[m_currentSessionId]->m_group_info.group_name;   // 需要修改
    }

    int insertedCount = 0;

    m_msgs_count = m_chat_msgs[m_currentSessionId].size();
    // 计算要加载的消息范围
    int startIndex, endIndex;
    if(m_msgs_count-(m_currentPage+1)*10 >= 0){
        startIndex = m_msgs_count - (m_currentPage+1)*10;
        endIndex = m_msgs_count - m_currentPage*10 - 1;
    } else if(m_msgs_count-m_currentPage*10-1 >= 0){
        startIndex = 0;
        endIndex = m_msgs_count - m_currentPage*10 - 1;
    } else {
        m_hasMoreHistory = false;
        m_loadingHistory = false;
        return;
    }

    // 临时存储要插入的消息（按时间顺序）
    QList<ChatMessage> messagesToInsert;
//    qDebug() << "startIndex:" << startIndex;
//    qDebug() << "endIndex:" << endIndex;
    for(int i = endIndex; i >= startIndex; i--){
        messagesToInsert.append(m_chat_msgs[m_currentSessionId][i]);
    }


    // 如果是第一次加载历史消息，检查是否需要跳过第一条消息的时间项
    bool isFirstHistoryLoad = (m_currentPage == 0);

    // 用于时间间隔判断的变量
    qint64 lastTimestamp = -1;

    // 按时间顺序插入消息
    for (const ChatMessage &chat_msg : messagesToInsert) {

        // 检查是否需要显示时间
        bool shouldCreateTime = false;

        if (lastTimestamp == -1) {
            // 第一条消息：只有在不是第一次加载历史消息时才显示时间
            // 或者与当前顶部消息时间间隔超过5分钟
            if (!isFirstHistoryLoad || (chat_msg.timestamp - getCurrentTopTimestamp()) > 300) {
                shouldCreateTime = true;
            }
        } else if ((chat_msg.timestamp - lastTimestamp) > 300) {
            // 与上一条消息间隔超过5分钟，显示时间
            shouldCreateTime = true;
        }

        if (shouldCreateTime) {
            createTimeItem(chat_msg.timestamp, true);
            insertedCount++;
        }

        lastTimestamp = chat_msg.timestamp;

        // 创建消息气泡
        Bubble *bubble = new Bubble();
        bubble->m_chat_msg = chat_msg;

        if(sessionType == 1){
            if(chat_msg.sender == m_userInfo.account){
                bubble->setHeadImage(m_userInfo.avatarPixmap);
            }else{
                bubble->setHeadImage(m_sessions[m_currentSessionId]->m_friend_info);
            }
        }else{
            QString group_account = m_currentSessionId;
            if(group_account.endsWith("_g")){
                group_account.chop(2);
            }
            QVector<USER_INFO>users = m_dataManager.m_groupMembers[group_account];
            for(int i = 0; i < users.size(); i++){
                if(QString::fromUtf8(users[i].user_account) == chat_msg.sender)
                {
                    friendName = QString::fromUtf8(users[i].user_name);
                    if(chat_msg.sender == m_userInfo.account){
                        bubble->setHeadImage(m_userInfo.avatarPixmap);
                    }else{
                        bubble->setHeadImage(users[i]);
                    }
                    break;
                }
            }
        }

        bubble->setBubble(parent_width, friendName);
        QListWidgetItem *m_Item = new QListWidgetItem();
        m_Item->setSizeHint(bubble->size());
        ui->chatWidget->insertItem(0, m_Item);
        ui->chatWidget->setItemWidget(m_Item, bubble);
        insertedCount++;
    }

    m_currentPage++;

    qDebug() << "加载后 - 插入数量:" << insertedCount
             << "总item数:" << ui->chatWidget->count();

    // 恢复滚动位置
    if (topVisibleItem && insertedCount > 0) {
        int newIndex = originalTopIndex + insertedCount;
        if (newIndex < ui->chatWidget->count()) {
            QListWidgetItem* itemToShow = ui->chatWidget->item(newIndex);
            ui->chatWidget->scrollToItem(itemToShow, QAbstractItemView::PositionAtTop);
            qDebug() << "滚动到新位置 - 原索引:" << originalTopIndex
                     << "新索引:" << newIndex;
        } else {
            ui->chatWidget->scrollToBottom();
            qDebug() << "新索引超出范围，滚动到底部";
        }
    }

    m_loadingHistory = false;
}


void mainWindow::setupAutoHistoryLoading()
{
    // 监听滚动条位置变化
    connect(ui->chatWidget->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &mainWindow::onChatScroll);

    // 初始化上次滚动位置
    m_lastScrollValue = ui->chatWidget->verticalScrollBar()->value();
}

void mainWindow::onChatScroll(int value)
{
    // 如果正在发送消息，跳过历史消息加载
    if (m_isSendingMessage) {
        m_lastScrollValue = value;
        return;
    }

    // 计算滚动方向：正值表示向下滚动，负值表示向上滚动
    int scrollDelta = value - m_lastScrollValue;
    m_lastScrollValue = value;

    // 只有当向上滚动且到达顶部时才加载历史消息
    bool isScrollingUp = scrollDelta < 0;  // 向上滚动
    bool isAtTop = value <= m_scrollThreshold;  // 接近顶部

    if (isScrollingUp && isAtTop &&
        m_hasMoreHistory &&
        !m_loadingHistory &&
        !m_chat_msgs.value(m_currentSessionId).isEmpty()) {

        load_more_history_msg();
    }
}



// 在mainWindow.cpp中添加
bool mainWindow::shouldShowTime(const QString &sessionId, qint64 currentTime)
{
    if (!m_lastMessageTime.contains(sessionId)) {
        // 第一条消息总是显示时间
        m_lastMessageTime[sessionId] = currentTime;
        return true;
    }

    qint64 lastTime = m_lastMessageTime[sessionId];
    qint64 timeDiff = currentTime - lastTime;

    // 如果时间间隔超过阈值，显示时间
    if (timeDiff > TIME_DISPLAY_INTERVAL) {
        m_lastMessageTime[sessionId] = currentTime;
        return true;
    }

    return false;
}

// 创建时间显示项的函数
void mainWindow::createTimeItem(qint64 timestamp, bool insertAtTop)
{
    // 创建时间显示widget
    QWidget *timeWidget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(timeWidget);
    layout->setContentsMargins(0, 10, 0, 10);

    QLabel *timeLabel = new QLabel();
    timeLabel->setAlignment(Qt::AlignCenter);

    // 将 Unix 时间戳（秒）转换为 QDateTime
    QDateTime dateTime = QDateTime::fromSecsSinceEpoch(timestamp);
    QString timeText;

    QDate currentDate = QDate::currentDate();
    QDate messageDate = dateTime.date();

    if (messageDate == currentDate) {
        // 今天显示具体时间
        timeText = dateTime.toString("hh:mm");
    } else if (messageDate == currentDate.addDays(-1)) {
        // 昨天显示"昨天"
        timeText = "昨天 " + dateTime.toString("hh:mm");
    } else if (messageDate.year() == currentDate.year()) {
        // 今年显示月日
        timeText = dateTime.toString("MM月dd日 hh:mm");
    } else {
        // 跨年显示完整日期
        timeText = dateTime.toString("yyyy年MM月dd日 hh:mm");
    }

    timeLabel->setText(timeText);
    timeLabel->setStyleSheet(
        "QLabel {"
        "border-radius:5px;"
        "color: rgb(172, 185, 192);"
        " background-color: rgba(228, 246, 255, 0);"
        "   padding: 3px 10px;"
        "   font: 75 9pt '微软雅黑';"
        "}"
    );

    // 根据文本内容计算合适的宽度
    QFontMetrics metrics(timeLabel->font());
    int textWidth = metrics.horizontalAdvance(timeText) + 20; // 加上边距
    timeLabel->setFixedWidth(qMin(textWidth, 120)); // 不超过200像素

    layout->addWidget(timeLabel);
    timeWidget->setLayout(layout);

    // 创建列表项
    QListWidgetItem *timeItem = new QListWidgetItem();
    timeItem->setSizeHint(timeWidget->sizeHint());
    timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsSelectable); // 不可选择

    // 根据参数决定插入位置
    if (insertAtTop) {
        // 插入到列表顶部（用于历史消息加载）
        ui->chatWidget->insertItem(0, timeItem);
    } else {
        // 添加到列表底部（用于正常消息添加）
        ui->chatWidget->addItem(timeItem);
    }
    ui->chatWidget->setItemWidget(timeItem, timeWidget);
}

// 添加辅助函数获取当前顶部消息的时间戳
qint64 mainWindow::getCurrentTopTimestamp()
{
    if (ui->chatWidget->count() == 0) {
        return -1;
    }

//    return m_chat_msgs[m_currentSessionId][m_chat_msgs.size()-10].timestamp;

    // 从第一个item开始查找，跳过时间项
      for (int i = ui->chatWidget->count()-1; i >=0; i--) {
        QListWidgetItem* item = ui->chatWidget->item(i);
        if (!item) continue;

        QWidget* widget = ui->chatWidget->itemWidget(item);
        Bubble* bubble = qobject_cast<Bubble*>(widget);
        if (bubble) {
            return bubble->m_chat_msg.timestamp;
        }
    }

    return -1;
}

void mainWindow::on_group_list_itemClicked(QListWidgetItem *item)
{
    if (!item) return;
    // 获取点击的 item 对应的自定义 widget
    friendItem *friItem = qobject_cast<friendItem*>(ui->group_list->itemWidget(item));
    if (!friItem) return;
    QString friend_name = "群名称：";
    QString friend_account = "群账号：";
    QString group_member_count = "群人数：";

    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(friItem->m_group_info.avatar_data, friItem->m_group_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);
    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->friendimg_label->width(), ui->friendimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 40); // 12是圆角半径
    ui->groupimg_label->setPixmap(roundedAvatar);

    ui->groupname_label->setText(friend_name.append(friItem->m_group_info.group_name));
    ui->groupaccount_label->setText(friend_account.append(friItem->m_group_info.group_account));
    ui->group_member_count_label->setText(group_member_count.append(QString::number(friItem->m_group_info.member_count)));
    show_group_info();
}

void mainWindow::on_addBtn_2_clicked()
{
    showMultiOptionMenu();
}

void mainWindow::on_addBtn_3_clicked()
{
    showMultiOptionMenu();
}

void mainWindow::on_open_chat_btn_2_clicked()
{
    // 获取纯净的账号（移除可能的前缀）
    QString groupAccount = ui->groupaccount_label->text();
    QString groupName = ui->groupname_label->text();

    // 移除前缀（如果存在）
    groupAccount = groupAccount.remove("群账号：");
    groupName = groupName.remove("群名称：");

    if(groupAccount.isEmpty() || groupName.isEmpty()){
        return;
    }

    QString sessionId = groupAccount;
    sessionId.append("_g");
    if(m_sessions.contains(sessionId)){
        switchToSession(sessionId);
        return;
    }

    add_chatsession(groupAccount, 2);

    // 切换到该会话
    switchToSession(sessionId);
}
