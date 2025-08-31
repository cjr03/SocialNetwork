//
//  mainwindow.h
//  
//
//  Created by Cole Roberts on 10/10/23.
//
#pragma once
#include <QMainWindow>
#include "network.h"
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // pre: none
    // post: update and display the mainfriends and mainsuggfriends table
    void createMainFriends();
    // pre: none
    // post: update and display the userfriends table
    void createUserFriends();
    // pre: none
    // post: add all uichanges to previousuichanges and to mainuser friends, clear uichanges, push currentuser onto previoususers
    void handlePrevious();

private slots:
    // pre: none
    // post: if the sign in feild is occupied with a valid username, the user is taken to the main page and both mainuser and currentuser are set to the associated user
    void on_SignIn_clicked();

    // pre: none
    // post: given a row and a column, if visit profile is clicked set current user to the user clicked and call handleprevious() and createUserFriends(). do nothing if the name is clicked.
    void on_MainFriends_cellClicked(int row, int column);

    // pre: none
    // post: given a row and a column, if visitprofile is clicked set current user to the user clicked and call handleprevious() and createUserFriends(). If addfriend is clicked add it's id to the uiChanges stack
    void on_UserFriends_cellClicked(int row, int column);

    // pre: none
    // post: given a row and a column, is clicked set current user to the user clicked and call handleprevious() and createUserFriends(). If addfriend add it's id to the uiChanges stack
    void on_MainSuggFriends_cellClicked(int row, int column);

    // pre: none
    // post: if there is previous user to go back to, the user is taken back to them calling either createMainFriend() or createUserFriends(), current user is set to the top of the previoususers stack. Regardless the changes from top of the previousuichanges are undone, popped off, and uichanges is cleared
    void on_UserBack_clicked();

    // pre: none
    // post: user is taken back to the login screen, all stacks cleared.
    void on_LogOut_clicked();

    // pre: none
    // post: the current user is set to main user, call handlePrevious() and createMainFriends()
    void on_Home_clicked();

private:
    Ui::MainWindow *ui;
    Network *network -> new Network("../resources/social_network.txt");
    User* mainUser;
    User* currentUser;
    std::stack<User*> previousUsers = {};
    std::stack<size_t> uiChanges = {};
    std::stack<std::stack<size_t>> previousuiChanges = {};
};
