#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->MainUser->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// pre: none
// post: if the sign in feild is occupied with a valid username, the user is taken to the main page and both mainuser and currentuser are set to the associated user
void MainWindow::on_SignIn_clicked()
{
    QString username = ui->NameInput->text();
    bool validLogin = false;
    for(std::size_t x = 0; x < network->numUsers(); ++x){
        if(network->validLogin(username.toStdString())){
            mainUser = network->getUser(network->getId(username.toStdString()));
            ui->SingInScreen->hide();
            ui->MainUser->show();
            validLogin = true;
            break;
        }
    }
    if(validLogin){
        currentUser = mainUser;
        network->readPosts("/Users/cole03roberts/Desktop/Assignment4/posts.txt");
        ui->UserBack->hide();
        createMainFriends();
    }
}


// pre: none
// post: given a row and a column, if visit profile is clicked set current user to the user clicked and call handleprevious() and createUserFriends(). do nothing if the name is clicked.
void MainWindow::on_MainFriends_cellClicked(int row, int column)
{
    if(column == 0) return;
    handlePrevious();
    currentUser = network->getUser(network->getId((ui->MainFriends->item(row, column-1)->text()).toStdString()));
    createUserFriends();
}

// pre: none
// post: given a row and a column, if visitprofile is clicked set current user to the user clicked and call handleprevious() and createUserFriends(). If addfriend is clicked add it's id to the uiChanges stack
void MainWindow::on_UserFriends_cellClicked(int row, int column)
{
    if(column == 0) return;
    else if(column == 1){
        handlePrevious();
        currentUser = network->getUser(network->getId((ui->UserFriends->item(row, column-1)->text()).toStdString()));
        createUserFriends();
    } else if (column == 2){
        if((ui->UserFriends->item(row, 2)->text()).toStdString() == "Add Friend"){
            uiChanges.push(network->getId((ui->UserFriends->item(row, 0)->text()).toStdString()));
            ui->UserFriends->setItem(row, 2, new QTableWidgetItem(QString("Added")));
        }
    }
}


// pre: none
// post: given a row and a column, is clicked set current user to the user clicked and call handleprevious() and createUserFriends(). If addfriend add it's id to the uiChanges stack
void MainWindow::on_MainSuggFriends_cellClicked(int row, int column)
{
    if(column == 0) return;
    else if(column == 1){
        handlePrevious();
        currentUser = network->getUser(network->getId((ui->MainSuggFriends->item(row, 0)->text()).toStdString()));
        createUserFriends();
    } else if (column == 2){
        if((ui->MainSuggFriends->item(row, 2)->text()).toStdString() == "Add Friend"){
            uiChanges.push(network->getId(ui->MainSuggFriends->item(row, 0)->text().toStdString()));
            ui->MainSuggFriends->setItem(row, column, new QTableWidgetItem(QString("Added")));
            ui->UserBack->show();
        }
    }
}


// pre: none
// post: if there is previous user to go back to, the user is taken back to them calling either createMainFriend() or createUserFriends(), current user is set to the top of the previoususers stack. Regardless the changes from top of the previousuichanges are undone, popped off, and uichanges is cleared
void MainWindow::on_UserBack_clicked()
{
    while(!uiChanges.empty()){
        mainUser->deleteFriend(uiChanges.top());
        uiChanges.pop();
    }
    if(!previousUsers.empty()){
        currentUser = previousUsers.top();
        previousUsers.pop();
    }
    if(!previousuiChanges.empty()){
        uiChanges = previousuiChanges.top();
        previousuiChanges.pop();
    }
    if(currentUser == mainUser){
        createMainFriends();
        if(uiChanges.empty()) ui->UserBack->hide();
    } else {
        createUserFriends();
    }
}

// pre: none
// post: user is taken back to the login screen, all stacks cleared.
void MainWindow::on_LogOut_clicked()
{
    uiChanges = std::stack<size_t>();
    previousuiChanges = std::stack<std::stack<size_t>>();
    ui->MainUser->hide();
    ui->SingInScreen->show();
}


// pre: none
// post: the current user is set to main user, call handlePrevious() and createMainFriends()
void MainWindow::on_Home_clicked()
{
    handlePrevious();
    currentUser = mainUser;
    createMainFriends();
}

// pre: none
// post: update and display the mainfriends and mainsuggfriends table
void MainWindow::createMainFriends(){
    size_t rows = currentUser->getFriends().size();
    ui->MainFriends->setRowCount(rows);
    for(size_t y = 0; y < mainUser->getFriends().size(); ++y){
        std::string name = network->getUser(mainUser->getFriends()[y])->getName();
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(name));
        ui->MainFriends->setItem(y, 0, item);
        ui->MainFriends->setItem(y, 1, new QTableWidgetItem(QString("Visit Profile")));
    }
    size_t score = 0;
    rows = network->suggestFriends(mainUser->getUserId(), score).size();
    ui->MainSuggFriends->setRowCount(rows);
    for(size_t y = 0; y < rows; ++y){
        std::string name = network->getUser(network->suggestFriends(mainUser->getUserId(), score)[y])->getName();
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(name));
        ui->MainSuggFriends->setItem(y, 0, item);
        ui->MainSuggFriends->setItem(y, 1, new QTableWidgetItem(QString("Visit Profile")));
        ui->MainSuggFriends->setItem(y, 2, new QTableWidgetItem(QString("Add Friend")));
    }
    ui->MainPosts->setText(QString::fromStdString(mainUser->displayPosts(5, false)));
    ui->MainFriends->show();
    ui->MainSuggFriends->show();
    ui->UserFriends->hide();
    ui->UsersName->setText(QString::fromStdString("My Profile"));
    ui->Home->hide();
}

// pre: none
// post: update and display the userfriends table
void MainWindow::createUserFriends(){
    size_t rows = currentUser->getFriends().size();
    ui->Home->show();
    ui->UserBack->show();
    ui->MainFriends->hide();
    ui->MainSuggFriends->hide();
    ui->UserFriends->show();
    ui->UserFriends->setRowCount(rows);
    ui->UsersName->setText(QString::fromStdString(currentUser->getName() + "'s Profile"));
    for(size_t y = 0; y < currentUser->getFriends().size(); ++y){
        bool validFriend = true;
        std::string name = network->getUser(currentUser->getFriends()[y])->getName();
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(name));
        ui->UserFriends->setItem(y, 0, item);
        ui->UserFriends->setItem(y, 1, new QTableWidgetItem(QString("Visit Profile")));
        for(auto friends: mainUser->getFriends()){
            if(friends == currentUser->getFriends()[y]) validFriend = false;
        }
        if(validFriend) ui->UserFriends->setItem(y, 2, new QTableWidgetItem(QString("Add Friend")));
        else ui->UserFriends->setItem(y, 2, new QTableWidgetItem(QString("Added")));
    }
    ui->MainPosts->setText(QString::fromStdString(currentUser->displayPosts(5, true)));
}

// pre: none
// post: add all uichanges to previousuichanges and to mainuser friends, clear uichanges, push currentuser onto previoususers
void MainWindow::handlePrevious(){
    previousUsers.push(currentUser);
    previousuiChanges.push(uiChanges);
    while(!uiChanges.empty()){
        mainUser->addFriend(uiChanges.top());
        uiChanges.pop();
    }
}

