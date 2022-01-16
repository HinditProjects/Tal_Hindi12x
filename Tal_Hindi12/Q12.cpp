#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <string>
using namespace std;
#define SIZE 40
#define MAX 10

//add struct code here

typedef struct User
{
	char id[MAX];
	char* userName;
}user;

int countUsersFromFile(const char* fileName);
void FromFileToStruct(const char* fname, user* arr);
user* searchUser(user* userArr, int noOfUsers, char* userName);

typedef struct Message {
	char* content;
	const User* creator;
}Message;

void createMessage(const user* u, Message* msg);

typedef struct Post {
	Message postContent;
	Message* replies;
	int repliesNo;
}Post;

void createPost(const user* u, Post* newPost);
void addComment(const user* u, Post* p);
void printPost(const Post* p);


char loggedMenu();
void main()
{
	int size = countUsersFromFile("listUsers.txt");
	if (!size) {
		cout << "No system users\n";
		exit(0);
	}
	else {
		user* users = new user[size];
		FromFileToStruct("listUsers.txt", users);
		Post posts[MAX];
		int noOfPosts = 0;
		char userName[SIZE];
		char internalRes;
		int choosenPost = 0;
		user* userexist;
		internalRes = loggedMenu();
		while (internalRes != 0) {
			switch (internalRes) {
			case('1'):
				cout << "Please enter search user name: ";
				fseek(stdin, 0, SEEK_END);
				cin >> userName;
				userexist = searchUser(users, size, userName);
				if (userexist) {
					createPost(userexist, posts + noOfPosts);
					noOfPosts++;
				}
				break;
			case('2'):
				if (noOfPosts == 0)
					cout << "no posts";
				else
				{
					for (int i = 0; i < noOfPosts; i++) {
						cout << "________________________" << i + 1 << "________________________" << endl;
						printPost(posts + i);
					}
					cout << "Please enter search user name: ";
					fseek(stdin, 0, SEEK_END);
					cin >> userName;
					userexist = searchUser(users, size, userName);
					if (userexist != NULL) {
						cout << "choose number of the post you want to comment (0=non):  ";
						cin >> choosenPost;
						if (choosenPost > 0 && choosenPost <= noOfPosts)
							addComment(userexist, posts + choosenPost - 1);
					}
					else
						cout << ", unable to comment on the post you selected\n";

				}
				break;
			case('0'):
				cout << "user logged out" << endl;
				delete[SIZE]users;
				exit(1);
			}
			internalRes = loggedMenu();
		}
	}
}

char loggedMenu() {
	char choise = 0;
	cout << "\n*****What Would you like to do:****\n";
	cout << "1-add Post to User\n";
	cout << "2-View all posts to add a comment\n";
	cout << "0-logOut\n";

	do {
		cin >> choise;
		if ((choise < '0' || choise > '2'))
			cout << "wrong choise - try again\n";
	} while (choise < '0' || choise > '2');

	return choise;
}

void createMessage(const user* u, Message* msg)  // לאכלס את ההודעות
{
	int size_of_content;
	char temp_Content[SIZE];
	msg->creator = u;

	cout << "Enter a message : ";
	cin.ignore();
	cin.getline(temp_Content, SIZE);
	size_of_content = strlen(temp_Content) + 1;
	msg->content = new char[size_of_content];
	strcpy(msg->content, temp_Content);


}


void createPost(const user* u, Post* newPost)
{

	createMessage(u, &(newPost->postContent));
	newPost->replies = NULL;
	newPost->repliesNo = 0;
}

void addComment(const user* u, Post* p)
{
	int size = p->repliesNo;
	Message* newReplies = new Message[size + 1];
	for (int i = 0; i < size; i++)
	{
		newReplies[i] = p->replies[i];
		delete[] & (p->replies[i]);
	}
	createMessage(u, &(newReplies[size]));
	p->replies = newReplies;
	p->repliesNo += 1;

}

void printMsg(const Message* msg,int size)
{
	int i = 0;
	if (msg != NULL)
	{
		cout << "Post replies : ";

		while (i<size)
		{
			cout << "User name : " << ((msg + i)->creator->userName) << endl;
			cout << "Content : " << ((msg + i)->content) << endl;
			i++;
		}

	}

}

void printPost(const Post* p)
{
	cout << "User name : " << p->postContent.creator->userName << endl;
	cout << "Post Content : " << p->postContent.content << endl;
	cout << "**********************************" << endl;
	printMsg(p->replies,p->repliesNo);
}

user* searchUser(user* userArr, int noOfUsers, char* userName)
{
	for (int i = 0; i < noOfUsers; i++)
	{
		if (strcmp(userArr[i].userName, userName) == 0)
		{
			return userArr;
		}
	}
	cout << "Username not found";
	return NULL;
}


int countUsersFromFile(const char* fileName)
{

	int count = 0;
	char c;
	FILE* pf = fopen(fileName, "r"); //פותח קובץ
	if (pf == NULL)// מוודא שנפתח
	{
		cout << "Error-file read error !\n";
		return NULL;
	}
	else
	{
		do
		{
			c = getc(pf); // עובר תו תו
			if (c == '\n')// אם התו עבר שורה מקדם את הספירה ב-1
				count++;
		} while (c != EOF); // תמשיך לעשות את זה עד שתגיע לסוף הקובץ


	}return count;  //מחזיר את מספר השורות של הקובץ
}


void FromFileToStruct(const char* fname, user* arr)
{
	int size_flen = countUsersFromFile(fname);
	char buffer[SIZE]; // מערך ארוך  עזר של צ'ארים
	FILE* pf = fopen(fname, "r");
	if (pf != NULL)
	{
		for (int i = 0; i < size_flen; i++)
		{
			fscanf(pf, "%s %s", arr[i].id, buffer);
			int size_Username = strlen(buffer + 1);
			arr[i].userName = new char[size_Username];
			strcpy(arr[i].userName, buffer);
		}

	}
	else {
		cout << "Error to open file";
	}
	fclose(pf);
}
