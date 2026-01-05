/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QPushButton *headimgBtn;
    QPushButton *chatBtn;
    QPushButton *addressBtn;
    QPushButton *noticeBtn;
    QSpacerItem *verticalSpacer;
    QStackedWidget *stackedWidget;
    QWidget *chat_page;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_2;
    QLineEdit *searchEdit;
    QLabel *label;
    QPushButton *addBtn;
    QListWidget *listWidget;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QLabel *friendname_label;
    QListWidget *chatWidget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *imgsend_Btn;
    QPushButton *filesend_Btn;
    QSpacerItem *horizontalSpacer;
    QTextEdit *msgEdit;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QPushButton *sendBtn;
    QWidget *friend_page;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_6;
    QGroupBox *groupBox_3;
    QLineEdit *searchEdit_2;
    QLabel *label_3;
    QPushButton *addBtn_2;
    QGroupBox *groupBox_5;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *friend_switch_btn;
    QPushButton *group_switch_btn;
    QListWidget *friend_list;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_7;
    QLabel *friendimg_label;
    QHBoxLayout *horizontalLayout_8;
    QLabel *friendname_label_2;
    QHBoxLayout *horizontalLayout_10;
    QLabel *friendaccount_label;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *open_chat_btn;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *delete_friend_btn;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QWidget *group_page;
    QHBoxLayout *horizontalLayout_20;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_18;
    QGroupBox *groupBox_7;
    QLineEdit *searchEdit_3;
    QLabel *label_4;
    QPushButton *addBtn_3;
    QGroupBox *groupBox_8;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_19;
    QPushButton *friend_switch_btn_2;
    QPushButton *group_switch_btn_2;
    QListWidget *group_list;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_14;
    QLabel *groupimg_label;
    QHBoxLayout *horizontalLayout_15;
    QLabel *groupname_label;
    QHBoxLayout *horizontalLayout_16;
    QLabel *groupaccount_label;
    QHBoxLayout *horizontalLayout_21;
    QLabel *group_member_count_label;
    QHBoxLayout *horizontalLayout_17;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *open_chat_btn_2;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *delete_friend_btn_2;
    QSpacerItem *horizontalSpacer_8;
    QWidget *notice_page;
    QHBoxLayout *horizontalLayout_22;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_6;
    QListWidget *friend_notice_list;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_7;
    QListWidget *group_notice_list;
    QVBoxLayout *verticalLayout_10;
    QSpacerItem *verticalSpacer_5;
    QLabel *group_member_label;
    QListWidget *users_in_group_list;

    void setupUi(QWidget *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->resize(1435, 795);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainWindow->sizePolicy().hasHeightForWidth());
        mainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icons/echat.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainWindow->setWindowIcon(icon);
        mainWindow->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_5 = new QHBoxLayout(mainWindow);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, 9, -1, -1);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(15, -1, -1, -1);
        headimgBtn = new QPushButton(mainWindow);
        headimgBtn->setObjectName(QString::fromUtf8("headimgBtn"));
        headimgBtn->setMinimumSize(QSize(51, 51));
        headimgBtn->setMaximumSize(QSize(51, 51));
        headimgBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgba(255, 255, 255, 0);\n"
"	background-position: center;\n"
"	border-radius:12px;\n"
"}\n"
""));

        verticalLayout_2->addWidget(headimgBtn);

        chatBtn = new QPushButton(mainWindow);
        chatBtn->setObjectName(QString::fromUtf8("chatBtn"));
        chatBtn->setMinimumSize(QSize(51, 51));
        chatBtn->setMaximumSize(QSize(51, 51));
        chatBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/new/prefix1/icons/chat_unactive.png);\n"
"	background-color: rgba(255, 255, 255, 0);\n"
"	background-position: center;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	border-image: url(:/new/prefix1/icons/chat_unactive.png);\n"
"	background-color: rgba(200, 200, 200, 100);\n"
"	background-position: center;\n"
"	border-radius:12px;\n"
"\n"
"}\n"
""));

        verticalLayout_2->addWidget(chatBtn);

        addressBtn = new QPushButton(mainWindow);
        addressBtn->setObjectName(QString::fromUtf8("addressBtn"));
        addressBtn->setMinimumSize(QSize(51, 51));
        addressBtn->setMaximumSize(QSize(51, 51));
        addressBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/new/prefix1/icons/address_list_unactive.png);\n"
"	background-color: rgba(255, 255, 255, 0);\n"
"	background-position: center;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	border-image: url(:/new/prefix1/icons/address_list_unactive.png);\n"
"	background-color: rgba(200, 200, 200, 100);\n"
"	background-position: center;\n"
"	border-radius:12px;\n"
"\n"
"}\n"
""));

        verticalLayout_2->addWidget(addressBtn);

        noticeBtn = new QPushButton(mainWindow);
        noticeBtn->setObjectName(QString::fromUtf8("noticeBtn"));
        noticeBtn->setMinimumSize(QSize(51, 51));
        noticeBtn->setMaximumSize(QSize(51, 51));
        noticeBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/new/prefix1/icons/notice_unactive.png);\n"
"	background-color: rgba(255, 255, 255, 0);\n"
"	background-position: center;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	border-image: url(:/new/prefix1/icons/notice_active.png);\n"
"	background-color: rgba(200, 200, 200, 100);\n"
"	background-position: center;\n"
"	border-radius:12px;\n"
"\n"
"}\n"
""));

        verticalLayout_2->addWidget(noticeBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_5->addLayout(verticalLayout_2);

        stackedWidget = new QStackedWidget(mainWindow);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setPointSize(12);
        stackedWidget->setFont(font);
        chat_page = new QWidget();
        chat_page->setObjectName(QString::fromUtf8("chat_page"));
        horizontalLayout_2 = new QHBoxLayout(chat_page);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox_2 = new QGroupBox(chat_page);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        searchEdit = new QLineEdit(groupBox_2);
        searchEdit->setObjectName(QString::fromUtf8("searchEdit"));
        searchEdit->setGeometry(QRect(0, 0, 241, 31));
        searchEdit->setCursorPosition(0);
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 30, 30));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/icons/search.png")));

        horizontalLayout->addWidget(groupBox_2);

        addBtn = new QPushButton(chat_page);
        addBtn->setObjectName(QString::fromUtf8("addBtn"));
        addBtn->setMinimumSize(QSize(31, 0));
        addBtn->setMaximumSize(QSize(31, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icons/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        addBtn->setIcon(icon1);

        horizontalLayout->addWidget(addBtn);


        verticalLayout->addLayout(horizontalLayout);

        listWidget = new QListWidget(chat_page);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(237, 237, 237);\n"
"    border-top: 1px solid #cccccc;  \n"
"    border-right: 1px solid #cccccc;  \n"
"    border-bottom: 1px solid #cccccc; \n"
"    border-left: 1px solid #cccccc; \n"
"}\n"
""));

        verticalLayout->addWidget(listWidget);


        horizontalLayout_2->addLayout(verticalLayout);

        groupBox = new QGroupBox(chat_page);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border-color: rgba(0, 0, 0, 0);\n"
"	background-color: rgb(237, 237, 237);\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        friendname_label = new QLabel(groupBox);
        friendname_label->setObjectName(QString::fromUtf8("friendname_label"));
        friendname_label->setFont(font);

        verticalLayout_3->addWidget(friendname_label);

        chatWidget = new QListWidget(groupBox);
        chatWidget->setObjectName(QString::fromUtf8("chatWidget"));
        chatWidget->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(237, 237, 237);\n"
"    border-top: 1px solid #cccccc;  \n"
"    border-right: 1px solid #cccccc;  \n"
"    border-bottom: 1px solid #cccccc; \n"
"    border-left: 0px; \n"
"}"));

        verticalLayout_3->addWidget(chatWidget);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        imgsend_Btn = new QPushButton(groupBox);
        imgsend_Btn->setObjectName(QString::fromUtf8("imgsend_Btn"));
        imgsend_Btn->setMinimumSize(QSize(34, 34));
        imgsend_Btn->setMaximumSize(QSize(34, 34));
        imgsend_Btn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/new/prefix1/icons/send_img.png);\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	border-image: url(:/new/prefix1/icons/send_img_pressed.png);\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}"));

        horizontalLayout_3->addWidget(imgsend_Btn);

        filesend_Btn = new QPushButton(groupBox);
        filesend_Btn->setObjectName(QString::fromUtf8("filesend_Btn"));
        filesend_Btn->setMinimumSize(QSize(38, 38));
        filesend_Btn->setMaximumSize(QSize(38, 38));
        filesend_Btn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image: url(:/new/prefix1/icons/send_file.png);\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	border-image: url(:/new/prefix1/icons/send_file_pressed.png);\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}"));

        horizontalLayout_3->addWidget(filesend_Btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_3);

        msgEdit = new QTextEdit(groupBox);
        msgEdit->setObjectName(QString::fromUtf8("msgEdit"));
        msgEdit->setFont(font);
        msgEdit->setStyleSheet(QString::fromUtf8("QTextEdit{\n"
"	border:none;\n"
"	background-color: rgb(237, 237, 237);\n"
"}"));

        verticalLayout_3->addWidget(msgEdit);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        sendBtn = new QPushButton(groupBox);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sendBtn->sizePolicy().hasHeightForWidth());
        sendBtn->setSizePolicy(sizePolicy1);
        sendBtn->setMinimumSize(QSize(60, 40));
        sendBtn->setMaximumSize(QSize(40, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font1.setPointSize(10);
        sendBtn->setFont(font1);
        sendBtn->setLayoutDirection(Qt::LeftToRight);
        sendBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-radius:6px;\n"
"	background-color: rgb(225, 225, 225);\n"
"	color: rgb(157, 157, 157);\n"
"}"));

        horizontalLayout_4->addWidget(sendBtn);


        verticalLayout_3->addLayout(horizontalLayout_4);

        verticalLayout_3->setStretch(1, 4);
        verticalLayout_3->setStretch(3, 1);

        horizontalLayout_2->addWidget(groupBox);

        horizontalLayout_2->setStretch(1, 1);
        stackedWidget->addWidget(chat_page);
        friend_page = new QWidget();
        friend_page->setObjectName(QString::fromUtf8("friend_page"));
        horizontalLayout_9 = new QHBoxLayout(friend_page);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        groupBox_3 = new QGroupBox(friend_page);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        searchEdit_2 = new QLineEdit(groupBox_3);
        searchEdit_2->setObjectName(QString::fromUtf8("searchEdit_2"));
        searchEdit_2->setGeometry(QRect(0, 0, 221, 31));
        searchEdit_2->setCursorPosition(0);
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 0, 30, 30));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/icons/search.png")));

        horizontalLayout_6->addWidget(groupBox_3);

        addBtn_2 = new QPushButton(friend_page);
        addBtn_2->setObjectName(QString::fromUtf8("addBtn_2"));
        addBtn_2->setMinimumSize(QSize(31, 0));
        addBtn_2->setMaximumSize(QSize(31, 16777215));
        addBtn_2->setIcon(icon1);

        horizontalLayout_6->addWidget(addBtn_2);


        verticalLayout_4->addLayout(horizontalLayout_6);

        groupBox_5 = new QGroupBox(friend_page);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMinimumSize(QSize(0, 40));
        groupBox_5->setMaximumSize(QSize(16777215, 40));
        horizontalLayoutWidget_5 = new QWidget(groupBox_5);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(20, -10, 211, 61));
        horizontalLayout_13 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_13->setSpacing(4);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        friend_switch_btn = new QPushButton(horizontalLayoutWidget_5);
        friend_switch_btn->setObjectName(QString::fromUtf8("friend_switch_btn"));
        friend_switch_btn->setMinimumSize(QSize(0, 22));
        friend_switch_btn->setMaximumSize(QSize(16777215, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Microsoft YaHei"));
        friend_switch_btn->setFont(font2);
        friend_switch_btn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(245, 250, 255);\n"
"	color: rgb(86, 208, 250);\n"
"	border-radius:5px;\n"
"}\n"
""));

        horizontalLayout_13->addWidget(friend_switch_btn);

        group_switch_btn = new QPushButton(horizontalLayoutWidget_5);
        group_switch_btn->setObjectName(QString::fromUtf8("group_switch_btn"));
        group_switch_btn->setMinimumSize(QSize(0, 22));
        group_switch_btn->setMaximumSize(QSize(16777215, 22));
        group_switch_btn->setFont(font2);
        group_switch_btn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(235, 240, 245);\n"
"	color: rgb(99, 122, 137);\n"
"	border-radius:5px;\n"
"}\n"
""));

        horizontalLayout_13->addWidget(group_switch_btn);


        verticalLayout_4->addWidget(groupBox_5);

        friend_list = new QListWidget(friend_page);
        friend_list->setObjectName(QString::fromUtf8("friend_list"));
        friend_list->setMaximumSize(QSize(16777215, 16777215));
        friend_list->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(237, 237, 237);\n"
"    border-top: 1px solid #cccccc;  \n"
"    border-right: 1px solid #cccccc;  \n"
"    border-bottom: 1px solid #cccccc; \n"
"    border-left: 1px solid #cccccc; \n"
"}\n"
""));

        verticalLayout_4->addWidget(friend_list);


        horizontalLayout_9->addLayout(verticalLayout_4);

        groupBox_4 = new QGroupBox(friend_page);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border-color: rgba(0, 0, 0, 0);\n"
"	background-color: rgb(237, 237, 237);\n"
"}"));
        verticalLayout_5 = new QVBoxLayout(groupBox_4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        friendimg_label = new QLabel(groupBox_4);
        friendimg_label->setObjectName(QString::fromUtf8("friendimg_label"));
        friendimg_label->setMinimumSize(QSize(240, 240));
        friendimg_label->setMaximumSize(QSize(240, 240));
        friendimg_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_7->addWidget(friendimg_label);


        verticalLayout_5->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        friendname_label_2 = new QLabel(groupBox_4);
        friendname_label_2->setObjectName(QString::fromUtf8("friendname_label_2"));
        friendname_label_2->setMinimumSize(QSize(350, 50));
        friendname_label_2->setMaximumSize(QSize(350, 100));
        friendname_label_2->setFont(font);
        friendname_label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-color: rgb(0, 0, 0);\n"
"	border: 2px solid black;\n"
"	border-radius:12px;\n"
"}"));

        horizontalLayout_8->addWidget(friendname_label_2);


        verticalLayout_5->addLayout(horizontalLayout_8);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        friendaccount_label = new QLabel(groupBox_4);
        friendaccount_label->setObjectName(QString::fromUtf8("friendaccount_label"));
        friendaccount_label->setMinimumSize(QSize(350, 50));
        friendaccount_label->setMaximumSize(QSize(350, 100));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font3.setPointSize(12);
        font3.setBold(false);
        font3.setWeight(50);
        friendaccount_label->setFont(font3);
        friendaccount_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-color: rgb(0, 0, 0);\n"
"	border: 2px solid black;\n"
"	border-radius:12px;\n"
"}"));

        horizontalLayout_10->addWidget(friendaccount_label);


        verticalLayout_5->addLayout(horizontalLayout_10);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_4);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_3);

        open_chat_btn = new QPushButton(groupBox_4);
        open_chat_btn->setObjectName(QString::fromUtf8("open_chat_btn"));
        open_chat_btn->setMinimumSize(QSize(110, 60));
        open_chat_btn->setMaximumSize(QSize(110, 60));
        open_chat_btn->setFont(font1);
        open_chat_btn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(1, 188, 250);\n"
"	border-radius:12px;\n"
"	color: rgb(255, 255, 255);\n"
"}"));

        horizontalLayout_11->addWidget(open_chat_btn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_5);

        delete_friend_btn = new QPushButton(groupBox_4);
        delete_friend_btn->setObjectName(QString::fromUtf8("delete_friend_btn"));
        delete_friend_btn->setMinimumSize(QSize(110, 60));
        delete_friend_btn->setMaximumSize(QSize(110, 60));
        delete_friend_btn->setFont(font1);
        delete_friend_btn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-radius:12px;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(252, 14, 0);\n"
"}"));

        horizontalLayout_11->addWidget(delete_friend_btn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_4);


        verticalLayout_5->addLayout(horizontalLayout_11);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_2);


        horizontalLayout_9->addWidget(groupBox_4);

        horizontalLayout_9->setStretch(1, 1);
        stackedWidget->addWidget(friend_page);
        group_page = new QWidget();
        group_page->setObjectName(QString::fromUtf8("group_page"));
        horizontalLayout_20 = new QHBoxLayout(group_page);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        groupBox_7 = new QGroupBox(group_page);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        searchEdit_3 = new QLineEdit(groupBox_7);
        searchEdit_3->setObjectName(QString::fromUtf8("searchEdit_3"));
        searchEdit_3->setGeometry(QRect(0, 0, 221, 31));
        searchEdit_3->setCursorPosition(0);
        label_4 = new QLabel(groupBox_7);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(0, 0, 30, 30));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/icons/search.png")));

        horizontalLayout_18->addWidget(groupBox_7);

        addBtn_3 = new QPushButton(group_page);
        addBtn_3->setObjectName(QString::fromUtf8("addBtn_3"));
        addBtn_3->setMinimumSize(QSize(31, 0));
        addBtn_3->setMaximumSize(QSize(31, 16777215));
        addBtn_3->setIcon(icon1);

        horizontalLayout_18->addWidget(addBtn_3);


        verticalLayout_7->addLayout(horizontalLayout_18);

        groupBox_8 = new QGroupBox(group_page);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setMinimumSize(QSize(0, 40));
        groupBox_8->setMaximumSize(QSize(16777215, 40));
        horizontalLayoutWidget_6 = new QWidget(groupBox_8);
        horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(20, -10, 211, 61));
        horizontalLayout_19 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_19->setSpacing(4);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(0, 0, 0, 0);
        friend_switch_btn_2 = new QPushButton(horizontalLayoutWidget_6);
        friend_switch_btn_2->setObjectName(QString::fromUtf8("friend_switch_btn_2"));
        friend_switch_btn_2->setMinimumSize(QSize(0, 22));
        friend_switch_btn_2->setMaximumSize(QSize(16777215, 20));
        friend_switch_btn_2->setFont(font2);
        friend_switch_btn_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(235, 240, 245);\n"
"	color: rgb(99, 122, 137);\n"
"	border-radius:5px;\n"
"}\n"
""));

        horizontalLayout_19->addWidget(friend_switch_btn_2);

        group_switch_btn_2 = new QPushButton(horizontalLayoutWidget_6);
        group_switch_btn_2->setObjectName(QString::fromUtf8("group_switch_btn_2"));
        group_switch_btn_2->setMinimumSize(QSize(0, 22));
        group_switch_btn_2->setMaximumSize(QSize(16777215, 22));
        group_switch_btn_2->setFont(font2);
        group_switch_btn_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(245, 250, 255);\n"
"	color: rgb(86, 208, 250);\n"
"	border-radius:5px;\n"
"}\n"
""));

        horizontalLayout_19->addWidget(group_switch_btn_2);


        verticalLayout_7->addWidget(groupBox_8);

        group_list = new QListWidget(group_page);
        group_list->setObjectName(QString::fromUtf8("group_list"));
        group_list->setMaximumSize(QSize(16777215, 16777215));
        group_list->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(237, 237, 237);\n"
"    border-top: 1px solid #cccccc;  \n"
"    border-right: 1px solid #cccccc;  \n"
"    border-bottom: 1px solid #cccccc; \n"
"    border-left: 1px solid #cccccc; \n"
"	border-radius:12px;\n"
"}\n"
""));

        verticalLayout_7->addWidget(group_list);


        horizontalLayout_20->addLayout(verticalLayout_7);

        groupBox_6 = new QGroupBox(group_page);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border-color: rgba(0, 0, 0, 0);\n"
"	background-color: rgb(237, 237, 237);\n"
"}"));
        verticalLayout_6 = new QVBoxLayout(groupBox_6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        groupimg_label = new QLabel(groupBox_6);
        groupimg_label->setObjectName(QString::fromUtf8("groupimg_label"));
        groupimg_label->setMinimumSize(QSize(240, 240));
        groupimg_label->setMaximumSize(QSize(240, 240));
        groupimg_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_14->addWidget(groupimg_label);


        verticalLayout_6->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        groupname_label = new QLabel(groupBox_6);
        groupname_label->setObjectName(QString::fromUtf8("groupname_label"));
        groupname_label->setMinimumSize(QSize(350, 50));
        groupname_label->setMaximumSize(QSize(350, 100));
        groupname_label->setFont(font);
        groupname_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-color: rgb(0, 0, 0);\n"
"	border: 2px solid black;\n"
"	border-radius:12px;\n"
"}"));

        horizontalLayout_15->addWidget(groupname_label);


        verticalLayout_6->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        groupaccount_label = new QLabel(groupBox_6);
        groupaccount_label->setObjectName(QString::fromUtf8("groupaccount_label"));
        groupaccount_label->setMinimumSize(QSize(350, 50));
        groupaccount_label->setMaximumSize(QSize(350, 100));
        groupaccount_label->setFont(font3);
        groupaccount_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-color: rgb(0, 0, 0);\n"
"	border: 2px solid black;\n"
"	border-radius:12px;\n"
"}"));

        horizontalLayout_16->addWidget(groupaccount_label);


        verticalLayout_6->addLayout(horizontalLayout_16);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        group_member_count_label = new QLabel(groupBox_6);
        group_member_count_label->setObjectName(QString::fromUtf8("group_member_count_label"));
        group_member_count_label->setMinimumSize(QSize(350, 50));
        group_member_count_label->setMaximumSize(QSize(350, 100));
        group_member_count_label->setFont(font);
        group_member_count_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-color: rgb(0, 0, 0);\n"
"	border: 2px solid black;\n"
"	border-radius:12px;\n"
"}"));

        horizontalLayout_21->addWidget(group_member_count_label);


        verticalLayout_6->addLayout(horizontalLayout_21);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_6);

        open_chat_btn_2 = new QPushButton(groupBox_6);
        open_chat_btn_2->setObjectName(QString::fromUtf8("open_chat_btn_2"));
        open_chat_btn_2->setMinimumSize(QSize(110, 60));
        open_chat_btn_2->setMaximumSize(QSize(110, 60));
        open_chat_btn_2->setFont(font1);
        open_chat_btn_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(1, 188, 250);\n"
"	border-radius:12px;\n"
"	color: rgb(255, 255, 255);\n"
"}"));

        horizontalLayout_17->addWidget(open_chat_btn_2);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_7);

        delete_friend_btn_2 = new QPushButton(groupBox_6);
        delete_friend_btn_2->setObjectName(QString::fromUtf8("delete_friend_btn_2"));
        delete_friend_btn_2->setMinimumSize(QSize(110, 60));
        delete_friend_btn_2->setMaximumSize(QSize(110, 60));
        delete_friend_btn_2->setFont(font1);
        delete_friend_btn_2->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-radius:12px;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(252, 14, 0);\n"
"}"));

        horizontalLayout_17->addWidget(delete_friend_btn_2);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_8);


        verticalLayout_6->addLayout(horizontalLayout_17);


        horizontalLayout_20->addWidget(groupBox_6);

        horizontalLayout_20->setStretch(1, 1);
        stackedWidget->addWidget(group_page);
        notice_page = new QWidget();
        notice_page->setObjectName(QString::fromUtf8("notice_page"));
        horizontalLayout_22 = new QHBoxLayout(notice_page);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        label_6 = new QLabel(notice_page);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        verticalLayout_8->addWidget(label_6);

        friend_notice_list = new QListWidget(notice_page);
        friend_notice_list->setObjectName(QString::fromUtf8("friend_notice_list"));
        friend_notice_list->setMaximumSize(QSize(550, 16777215));
        friend_notice_list->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(237, 237, 237);\n"
"    border-top: 1px solid #cccccc;  \n"
"    border-right: 1px solid #cccccc;  \n"
"    border-bottom: 1px solid #cccccc; \n"
"    border-left: 1px solid #cccccc; \n"
"	border-radius:12px;\n"
"}\n"
""));

        verticalLayout_8->addWidget(friend_notice_list);


        horizontalLayout_22->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        label_7 = new QLabel(notice_page);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_9->addWidget(label_7);

        group_notice_list = new QListWidget(notice_page);
        group_notice_list->setObjectName(QString::fromUtf8("group_notice_list"));
        group_notice_list->setMaximumSize(QSize(16777215, 16777215));
        group_notice_list->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(237, 237, 237);\n"
"    border-top: 1px solid #cccccc;  \n"
"    border-right: 1px solid #cccccc;  \n"
"    border-bottom: 1px solid #cccccc; \n"
"    border-left: 1px solid #cccccc; \n"
"	border-radius:12px;\n"
"}\n"
""));

        verticalLayout_9->addWidget(group_notice_list);


        horizontalLayout_22->addLayout(verticalLayout_9);

        stackedWidget->addWidget(notice_page);

        horizontalLayout_5->addWidget(stackedWidget);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalSpacer_5 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_10->addItem(verticalSpacer_5);

        group_member_label = new QLabel(mainWindow);
        group_member_label->setObjectName(QString::fromUtf8("group_member_label"));
        group_member_label->setFont(font2);

        verticalLayout_10->addWidget(group_member_label);

        users_in_group_list = new QListWidget(mainWindow);
        users_in_group_list->setObjectName(QString::fromUtf8("users_in_group_list"));
        users_in_group_list->setStyleSheet(QString::fromUtf8("QListWidget{\n"
"	background-color: rgb(237, 237, 237);\n"
"    border-top: 1px solid #cccccc;  \n"
"    border-right: 1px solid #cccccc;  \n"
"    border-bottom: 1px solid #cccccc; \n"
"    border-left: 1px solid #cccccc; \n"
"}"));

        verticalLayout_10->addWidget(users_in_group_list);


        horizontalLayout_5->addLayout(verticalLayout_10);

        horizontalLayout_5->setStretch(1, 9);
        horizontalLayout_5->setStretch(2, 2);

        retranslateUi(mainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QWidget *mainWindow)
    {
        mainWindow->setWindowTitle(QCoreApplication::translate("mainWindow", "Echat", nullptr));
        headimgBtn->setText(QString());
        chatBtn->setText(QString());
        addressBtn->setText(QString());
        noticeBtn->setText(QString());
        groupBox_2->setTitle(QString());
        searchEdit->setText(QString());
        label->setText(QString());
        addBtn->setText(QString());
        groupBox->setTitle(QString());
        friendname_label->setText(QCoreApplication::translate("mainWindow", "\345\245\275\345\217\213\346\230\265\347\247\260", nullptr));
        imgsend_Btn->setText(QString());
        filesend_Btn->setText(QString());
        label_2->setText(QString());
        sendBtn->setText(QCoreApplication::translate("mainWindow", "\345\217\221\351\200\201", nullptr));
        groupBox_3->setTitle(QString());
        searchEdit_2->setText(QString());
        label_3->setText(QString());
        addBtn_2->setText(QString());
        groupBox_5->setTitle(QString());
        friend_switch_btn->setText(QCoreApplication::translate("mainWindow", "\345\245\275\345\217\213", nullptr));
        group_switch_btn->setText(QCoreApplication::translate("mainWindow", "\347\276\244\350\201\212", nullptr));
        groupBox_4->setTitle(QString());
        friendimg_label->setText(QCoreApplication::translate("mainWindow", "\345\244\264\345\203\217", nullptr));
        friendname_label_2->setText(QCoreApplication::translate("mainWindow", "\346\230\265\347\247\260\357\274\232", nullptr));
        friendaccount_label->setText(QCoreApplication::translate("mainWindow", "\350\264\246\345\217\267\357\274\232", nullptr));
        open_chat_btn->setText(QCoreApplication::translate("mainWindow", "\345\217\221\351\200\201\344\277\241\346\201\257", nullptr));
        delete_friend_btn->setText(QCoreApplication::translate("mainWindow", "\345\210\240\351\231\244\345\245\275\345\217\213", nullptr));
        groupBox_7->setTitle(QString());
        searchEdit_3->setText(QString());
        label_4->setText(QString());
        addBtn_3->setText(QString());
        groupBox_8->setTitle(QString());
        friend_switch_btn_2->setText(QCoreApplication::translate("mainWindow", "\345\245\275\345\217\213", nullptr));
        group_switch_btn_2->setText(QCoreApplication::translate("mainWindow", "\347\276\244\350\201\212", nullptr));
        groupBox_6->setTitle(QString());
        groupimg_label->setText(QCoreApplication::translate("mainWindow", "\345\244\264\345\203\217", nullptr));
        groupname_label->setText(QCoreApplication::translate("mainWindow", "\347\276\244\345\220\215\347\247\260\357\274\232", nullptr));
        groupaccount_label->setText(QCoreApplication::translate("mainWindow", "\350\264\246\345\217\267\357\274\232", nullptr));
        group_member_count_label->setText(QCoreApplication::translate("mainWindow", "\347\276\244\346\210\220\345\221\230\357\274\232", nullptr));
        open_chat_btn_2->setText(QCoreApplication::translate("mainWindow", "\345\217\221\351\200\201\344\277\241\346\201\257", nullptr));
        delete_friend_btn_2->setText(QCoreApplication::translate("mainWindow", "\351\200\200\345\207\272\347\276\244\350\201\212", nullptr));
        label_6->setText(QCoreApplication::translate("mainWindow", "\345\245\275\345\217\213\351\200\232\347\237\245", nullptr));
        label_7->setText(QCoreApplication::translate("mainWindow", "\347\276\244\351\200\232\347\237\245", nullptr));
        group_member_label->setText(QCoreApplication::translate("mainWindow", "\347\276\244\350\201\212\346\210\220\345\221\230 ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
