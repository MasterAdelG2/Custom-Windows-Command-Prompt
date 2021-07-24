#undef UNICODE 
#define _CRT_SECURE_NO_WARNINGS
#include <bits\stdc++.h>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <locale>
#include <iostream>
#define deleteLastChar(str) str.erase(str.end()-1, str.end())
#define lastChar(str) str[str.length() - 1]
#define isDir(WFD) ((bool) (WFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
#define isHid(WFD) ((bool)(WFD.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
#define isArch(WFD) ((bool)(WFD.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
#define isSys(WFD) ((bool)(WFD.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
#define isReadOnly(WFD) ((bool)(WFD.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
using namespace std;
// (20) Main Commands
void DirCommand(string path);
void ChangeDirectoryCommand();
void ChangeDriveCommand();
void ColorCommand(); 
void HelpCommand();
void ChangeDateCommand();
void ChangeTimeCommand();
void TypeCommand();
void DeleteCommand(string PATH);
void RemoveDirCommand(string PATH);
void RenameCommand();
void makeDirCommand(string path);
void SortCommand();
void CopyCommand();
void MoveCommand();
void FindCommand();
void MoreCommand();
int XcopyCommand(string From, string To, bool MAIN, bool Confirm);
void CompCommand();
void AttribCommand();
// (13) Additional Functions
string seperate(string &STRING, char delim = ' ');
bool isExist(string &path);
bool isDirectory(string path);
void getPath(string &path);
int contain(string str, string key);
void toLowercase(string &s);
void printFile(string path);
int getFileSize(string Filename);
void printStream(stringstream &Output);
void printStreamInFile(stringstream &Output, string path);
void pipelineStream(stringstream &Output, int total);
char getColorNumber(string color);
bool inMainDrive();
// (3) Global Variables
string CURRENT_PATH = "E:\\";
string LINE;
string COMMAND;
int main()
	{
	cout << "Developed by Ahmed Adel & Mohamed Tharwat.\n"<<
		"For more information about a specific command, type HELP\nor Help [Command].\n";
	while (true)
	{
		
		cout << endl << CURRENT_PATH << ">";
		getline(cin, LINE);

		COMMAND = seperate(LINE);
		toLowercase(COMMAND);

		if (COMMAND == "dir")
			DirCommand(LINE);
		else if (COMMAND == "cd" || COMMAND == "chdir" || COMMAND == "cd.." || COMMAND == "cd\\")
			ChangeDirectoryCommand();
		else if (COMMAND.length() == 2 && COMMAND[1] == ':')
			ChangeDriveCommand();
		else if (COMMAND == "del" || COMMAND == "erase")
			DeleteCommand(LINE);
		else if (COMMAND == "mkdir" || COMMAND == "md")
			makeDirCommand(LINE);
		else if (COMMAND == "sort")
			SortCommand();
		else if (COMMAND == "attrib")
			AttribCommand();
		else if (COMMAND == "copy")
			CopyCommand();
		else if (COMMAND == "move")
			MoveCommand();
		else if (COMMAND == "more")
			MoreCommand();
		else if (COMMAND == "xcopy")			
			XcopyCommand(LINE, seperate(LINE),true,true);
		else if (COMMAND == "comp")				
			CompCommand();
		else if (COMMAND == "find")
			FindCommand();
		else if (COMMAND == "rmdir" || COMMAND == "rd")
			RemoveDirCommand(LINE);
		else if (COMMAND == "rename" || COMMAND == "ren")
			RenameCommand();
		else if (COMMAND == "type")
			TypeCommand();
		else if (COMMAND == "help")
			HelpCommand();
		else if (COMMAND == "color")
			ColorCommand();
		else if (COMMAND == "date") // Requires Adminstration to Change Date
			ChangeDateCommand();
		else if (COMMAND == "time") // Requires Adminstration to Change Time
			ChangeTimeCommand();
		else if (COMMAND == "cls")
			system("CLS");
		else if (COMMAND == "exit")
			exit(0);
		else
			cout << "\'" << COMMAND << "\' is not recognized as an internal or external command,\noperable program or batch file.\n";
	}
}
void CompCommand()
{
	stringstream Output;
	int first_option = MAX_PATH;
	bool PIPELINE = false;
	bool PRINTINFILE = false;
	bool ASCII = false;
	bool DECIMAL = false;
	bool DIS_LINE = false;
	bool NLINES = false;
	bool ALLCASE = false;
	int lines = 0;
	int total = 0;
	//check if | MORE
	if (contain(LINE, "|") != -1 && contain(LINE, "more") != -1)
	{
		PIPELINE = true;
		first_option = min(first_option, contain(LINE, "|"));
	}
	if (contain(LINE, "/C") != -1)
	{
		first_option = min(first_option, contain(LINE, "/C"));
		ALLCASE = true;
	}
	if (contain(LINE, "/D") != -1)
	{
		first_option = min(first_option, contain(LINE, "/D"));
		DECIMAL = true;
	}
	if (contain(LINE, "/A") != -1)
	{
		first_option = min(first_option, contain(LINE, "/A"));
		ASCII = true;
	}
	if (contain(LINE, "/L") != -1)
	{
		first_option = min(first_option, contain(LINE, "/L"));
		DIS_LINE = true;
	}
	if (contain(LINE, "/N=") != -1)
	{
		first_option = min(first_option, contain(LINE, "/N="));
		NLINES = true;
		for (int i = contain(LINE, "/N=") + 3; i < LINE.length() && LINE[i]!=' '; i++)
			Output << LINE[i];
		Output >> lines;
	}
	string printInPath = "";
	if (contain(LINE, ">") != -1)
	{
		first_option = min(first_option, contain(LINE, ">"));
		PRINTINFILE = true;
		for (int i = contain(LINE, ">") + 2; i < LINE.length(); i++)
			printInPath += LINE[i];
	}


	if (first_option != MAX_PATH)
		LINE.erase(LINE.begin() + first_option, LINE.end());

	
	string firstFile = seperate(LINE);
	string secondFile = LINE;
	getPath(firstFile);
	getPath(secondFile);
	//check iff is drive
	if (firstFile.length() == 3)
		firstFile += "*";
	if (secondFile.length() == 3)
		secondFile += "*";
	bool equal = true;
	//search first path
	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	HANDLE_FIND = FindFirstFile((firstFile).c_str(), &WFD);
	if (INVALID_HANDLE_VALUE == HANDLE_FIND) 
	{
		cout << "can't open/find file :" << firstFile << endl;
		return;
	}
	if (isDir(WFD) && WFD.cFileName[0] != '.' && WFD.cFileName[0] != '$')
	{
		firstFile += "\\*";
		HANDLE_FIND = FindFirstFile((firstFile).c_str(), &WFD);
	}

	while (lastChar(firstFile) != '\\')
		deleteLastChar(firstFile);
	do {
		if (!isDir(WFD))
		{
			//search second path
			WIN32_FIND_DATA WFD2;
			HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
			HANDLE_FIND = FindFirstFile((secondFile).c_str(), &WFD2);

			if (INVALID_HANDLE_VALUE == HANDLE_FIND) 
			{
				cout << "comparing " << firstFile + WFD.cFileName << "and" << secondFile << endl;
				cout << "Can't find/open " << secondFile << endl;
				return;
			}
			//check if is dir
			if (isDir(WFD2) && WFD2.cFileName[0] != '.' && WFD2.cFileName[0] != '$')
			{
				secondFile += "\\*";
				HANDLE_FIND = FindFirstFile((secondFile).c_str(), &WFD2);
			}
			while (lastChar(secondFile) != '\\')
				deleteLastChar(secondFile);
			do
			{
				if (!(isDir(WFD2)))
				{
					if (getFileSize(firstFile + WFD.cFileName) != getFileSize(secondFile + WFD2.cFileName))
					{
						Output << "Comparing " << firstFile + WFD.cFileName << " and " << secondFile + WFD2.cFileName << "...\ntwo files are different in size\n\n";
						continue;
					}
					else 
					{
						ifstream f1, f2;
						f1.open(firstFile + WFD.cFileName);
						f2.open(secondFile + WFD2.cFileName);
						int locallines = 1;
						char char1 = '.', char2 = '.';
						Output << "comparing " << firstFile + WFD.cFileName << " and " << secondFile + WFD2.cFileName << ".." << endl;
						while ((!f1.eof()) && (!f2.eof()))
						{
							if (char1 == '\r')
								locallines++;
							if (locallines > lines && NLINES)
								break;
							f1 >> noskipws >> char1;
							f2 >> noskipws >> char2;
							string temp1 = char1 + "", temp2 = char2 + "";
							toLowercase(temp1), toLowercase(temp2);
							if (temp1 != temp2 || (ALLCASE && char1 != char2))
							{
								equal = false;
								Output << "Compar error at ";
								if (DIS_LINE)
									Output << "LINE " << locallines << endl;
								else
									Output << "OFFSET " << (int)f1.tellg() - 1 << endl;
								if (ASCII)
								{
									if (char1 == '\r')
										Output << "file 1 = \'\\r\'" << endl;
									else if (char1 == ' ')
										Output << "file 1 = \' \'" << endl;
									else
										Output << "file 1 = " << char1 << endl;

									if (char2 == '\r')
										Output << "file 2 = \'\\r\'" << endl;
									else if (char2 == ' ')
										Output << "file 2 = \' \'" << endl;
									else 
										Output << "file 2 = " << char2 << endl;
								}
								else if (DECIMAL)
								{
									Output << "file 1 = " << (int)char1 << endl;
									Output << "file 2 = " << (int)char2 << endl;
								}
								else
								{
									Output << "file 1 = " << uppercase << hex << (int)char1 << endl;
									Output << "file 2 = " << uppercase << hex << (int)char2 << endl;
									Output << dec << nouppercase;
								}
							}
						}
						f1.close(), f2.close();
					}
				}
			} while (FindNextFile(HANDLE_FIND, &WFD2) != 0);
		}
	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);

	if (equal)
		Output << "Files Compare OK\n\n";

	if (PIPELINE)
		pipelineStream(Output, total);
	else if (PRINTINFILE)
		printStreamInFile(Output, LINE);
	else
		printStream(Output);
}
void AttribCommand()
{
	stringstream Output;
	if (LINE.length() == 0)
	{
		cout << "\nThe syntax of the command is incorrect.\n";
		return;
	}
	int first_option = MAX_PATH;
	bool PIPELINE = false;
	int total = 0;
	//check if | MORE
	if (contain(LINE, "|") != -1 && contain(LINE, "more") != -1)
	{
		PIPELINE = true;
		first_option = min(first_option, contain(LINE, "|"));
	}
	bool PRINTINFILE = false;
	string printInPath = "";
	if (contain(LINE, ">") != -1)
	{
		first_option = min(first_option, contain(LINE, ">"));
		PRINTINFILE = true;
		for (int i = contain(LINE, ">") + 2; i < LINE.length(); i++)
			printInPath += LINE[i];
	}
	if (first_option != MAX_PATH)
		LINE.erase(LINE.begin() + first_option, LINE.end());

	int last_option = -1;
	bool READONLY = false;
	bool RREADONLY = false;

	bool ARCHIVE = false;
	bool RARCHIVE = false;

	bool SYSTEM = false;
	bool RSYSTEM = false;

	bool HIDDEN = false;
	bool RHIDDEN = false;
	bool Change = false;
	for (int i = 0; i < LINE.length(); i++)
	{
		
		if (LINE[i] == '+')
		{
			i++;
			Change = true;
			last_option = max(last_option, i + 1);
			if (LINE[i] == 'R' || LINE[i] == 'r')
				READONLY = true, RREADONLY = false;
			else if (LINE[i] == 'A' || LINE[i] == 'a')
				ARCHIVE = true, RARCHIVE = false;
			else if (LINE[i] == 'S' || LINE[i] == 's')
				SYSTEM = true, RSYSTEM = false;
			else if (LINE[i] == 'H' || LINE[i] == 'h')
				HIDDEN = true, RHIDDEN = false;
		}
		if (LINE[i] == '-')
		{
			Change = true;
			i++;
			last_option = max(last_option, i + 1);
			if (LINE[i] == 'R' || LINE[i] == 'r')
				RREADONLY = true, READONLY = false;
			else if (LINE[i] == 'A' || LINE[i] == 'a')
				RARCHIVE = true, ARCHIVE = false;
			else if (LINE[i] == 'S' || LINE[i] == 's')
				RSYSTEM = true, SYSTEM = false;
			else if (LINE[i] == 'H' || LINE[i] == 'h')
				RHIDDEN = true, HIDDEN = false;
		}
	}
	if (last_option != -1)
		LINE.erase(LINE.begin(), LINE.begin() + last_option + 1);

	getPath(LINE);

	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	HANDLE_FIND = FindFirstFile((LINE).c_str(), &WFD);

	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
	{
		cout << "\nThe system cannot find the file specified.";
		return;
	}



	while (lastChar(LINE) != '\\')
		deleteLastChar(LINE);
	do
	{
		if (WFD.cFileName[0] == '.')		// Skip "." and ".."
			continue;

		if (Change)
		{
			DWORD attrib = WFD.dwFileAttributes;

			if (READONLY && !isReadOnly(WFD))
				attrib += FILE_ATTRIBUTE_READONLY;
			else if (RREADONLY && isReadOnly(WFD))
				attrib -= FILE_ATTRIBUTE_READONLY;
			if (ARCHIVE && !isArch(WFD))
				attrib += FILE_ATTRIBUTE_ARCHIVE;
			else if (RARCHIVE && isArch(WFD))
				attrib -= FILE_ATTRIBUTE_ARCHIVE;
			if (SYSTEM && !isSys(WFD))
				attrib += FILE_ATTRIBUTE_SYSTEM;
			else if (RSYSTEM && isSys(WFD))
				attrib -= FILE_ATTRIBUTE_SYSTEM;
			if (HIDDEN && !isHid(WFD))
				attrib += FILE_ATTRIBUTE_HIDDEN;
			else if (RHIDDEN && isHid(WFD))
				attrib -= FILE_ATTRIBUTE_HIDDEN;

			int success = SetFileAttributes((LINE + WFD.cFileName).c_str(), attrib);
			if (!success)
				Output << "Not resetting hidden file - " << LINE + WFD.cFileName << "\n",total++;
		}
		else	// Just Print attributes
		{
			if (isArch(WFD))
				Output << "A ";
			else Output << "  ";
			if (isReadOnly(WFD))
				Output << "R ";
			else Output << "  ";
			if (isSys(WFD))
				Output << "S ";
			else Output << "  ";
			if (isHid(WFD))
				Output << "H ";
			else Output << "  ";
			if (isDir(WFD))
				Output << "D ";
			else Output << "  ";
			Output << "  " << LINE + WFD.cFileName << "\n",total++;
		}

	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);

	if (PIPELINE)
		pipelineStream(Output, total);
	else if (PRINTINFILE)
		printStreamInFile(Output, LINE);
	else
		printStream(Output);
}
int XcopyCommand(string From,string To,bool MAIN,bool Confirm)
{
	bool COPYALL = true;
	if (MAIN)
	{
		swap(From, To);
		int first_option = MAX_PATH;
		if (contain(To, "/Y")!= -1)
		{
			first_option= min(first_option, contain(To, "/Y"));
			Confirm = false;
		}
		if (contain(To, "/E") != -1)
			first_option=min(first_option, contain(To, "/E"));
		else
			COPYALL = false;

		if (first_option != MAX_PATH)
			To.erase(To.begin() + first_option, To.end());
	}
	int Copied_Files = 0;
	bool sameName = false;

	getPath(From);
	getPath(To);
	if (To.length() == 3)
		To += "*";
	if (From.length() == 3)
		From += "*";

	WIN32_FIND_DATA WFD, WFD2;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;

	HANDLE_FIND = FindFirstFile((To).c_str(), &WFD2);

	if ((INVALID_HANDLE_VALUE != HANDLE_FIND) && (isDir(WFD2)))
	{
		while (lastChar(To) != '\\')
			deleteLastChar(To);

		if (WFD2.cFileName[0] == '$' || WFD2.cFileName[0] == '.')
			;
		else
			To = To + WFD2.cFileName + "\\";
		sameName = true;
	}

	HANDLE_FIND = FindFirstFile((From).c_str(), &WFD);
	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
	{
		cout << "\nThe system cannot find the file(s) specified.\n       0 file(s) copied.\n";
		return 0;
	}

	while (lastChar(From) != '\\')
		deleteLastChar(From);
	if ((isDir(WFD)) && WFD.cFileName[0] != '.' && WFD.cFileName[0] != '$')
	{
		From = From + WFD.cFileName + "\\";
		HANDLE_FIND = FindFirstFile((From + "*").c_str(), &WFD);
	}
	string tempTo = To;
	string tempFrom = From;
	do
	{
		tempFrom = From + WFD.cFileName;
		if (sameName)
			tempTo = To + WFD.cFileName;

		if (!(isDir(WFD)))
		{
			if (!isExist(tempTo))
			{
				if (CopyFile((tempFrom ).c_str(), tempTo.c_str(), FALSE))
					Copied_Files++;
			}
			else if (!Confirm)
			{
				if (CopyFile((tempFrom ).c_str(), tempTo.c_str(), FALSE))
					Copied_Files++;
			}
			else
			{
				string Answer;
				cout << "Overwrite " << To << "? (Yes/No/All):";
				getline(cin, Answer);
				toLowercase(Answer);
				if (Answer == "yes" || Answer == "y")
				{
					if (CopyFile((From + WFD.cFileName).c_str(), To.c_str(), FALSE))
						Copied_Files++;
				}
				else if (Answer == "all" || Answer == "a")
				{
					Confirm = false;
					if (CopyFile((From + WFD.cFileName).c_str(), To.c_str(), FALSE))
						Copied_Files++;
				}
			}
		}
		else if (COPYALL && WFD.cFileName[0] != '.' && WFD.cFileName[0] != '$')
		{
			if (!isExist(tempTo))
				makeDirCommand(tempTo);
			Copied_Files += XcopyCommand(tempFrom, tempTo, false, Confirm);
		}
	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);
	
	if (MAIN)
		cout << "       " << Copied_Files << " file(s) copied.\n";
	return Copied_Files;
}	   
void MoreCommand()
{
	LINE = LINE + " | MORE";
	TypeCommand();
}
void FindCommand()
{
	if (LINE.length() == 0)
	{
		cout << "\nThe syntax of the command is incorrect.\n";
		return;
	}

	stringstream Output;
	int first_option = MAX_PATH;
	bool PIPELINE = false;
	int total = 0;
	//check if | MORE
	if (contain(LINE, "|") != -1 && contain(LINE, "more") != -1)
	{
		PIPELINE = true;
		first_option = min(first_option, contain(LINE, "|"));
	}
	bool PRINTINFILE = false;
	string printInPath = "";
	if (contain(LINE, ">") != -1)
	{
		first_option = min(first_option, contain(LINE, ">"));
		PRINTINFILE = true;
		for (int i = contain(LINE, ">") + 2; i < LINE.length(); i++)
			printInPath += LINE[i];
	}
	if (first_option != MAX_PATH)
		LINE.erase(LINE.begin() + first_option, LINE.end());
	
	string keyWord = "";
	int i=0;
	while ( i!=LINE.length()-1 && LINE[i++]!='\"')
	{}
	while (i != LINE.length() - 1 && LINE[i] != '\"')
	{
		keyWord += LINE[i];
		i++;
	}
	LINE.erase(LINE.begin(), LINE.begin() + i+2);

	getPath(LINE);

	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	HANDLE_FIND = FindFirstFile((LINE).c_str(), &WFD);

	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
	{
		cout << "\nThe system cannot find the file specified.";
		return;
	}

	while (lastChar(LINE) != '\\')
		deleteLastChar(LINE);

	
	if (isDir(WFD) && WFD.cFileName[0] != '.' && WFD.cFileName[0] != '$')
	{
		cout << "Access denied - " << LINE + WFD.cFileName << endl;
		return;
	}

	do
	{
		if (!(isDir(WFD)))
		{
			ifstream File;
			File.open(LINE + WFD.cFileName);
			Output << "------------" << LINE + WFD.cFileName << endl, total++;
			string line;
			while (getline(File, line))
			{
				if (line.find(keyWord) != -1)
					Output << line << endl, total++;
			}
			File.close();
			Output << endl, total++;
		}
	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);

	if (PIPELINE)
		pipelineStream(Output, total);
	else if (PRINTINFILE)
		printStreamInFile(Output, LINE);
	else
		printStream(Output);
}
void MoveCommand()
{
	string path = LINE;
	CopyCommand();
	path = seperate(path);
	DeleteCommand(path);
}
void SortCommand() 
{
	string *FILE_CONTENTS=new string [1e5];
	string FILE_NAME=seperate(LINE);
	stringstream Output;
	getPath(FILE_NAME);
	
	//check if | MORE
	int first_option = MAX_PATH;
	bool PIPELINE = false;
	int total = 0;
	//check if | MORE
	if (contain(LINE, "|") != -1 && contain(LINE, "more") != -1)
	{
		PIPELINE = true;
		first_option = min(first_option, contain(LINE, "|"));
	}
	bool PRINTINFILE = false;
	string printInPath = "";
	if (contain(LINE, ">") != -1)
	{
		first_option = min(first_option, contain(LINE, ">"));
		PRINTINFILE = true;
		for (int i = contain(LINE, ">") + 2; i < LINE.length(); i++)
			printInPath += LINE[i];
	}
	if (first_option != MAX_PATH)
		LINE.erase(LINE.begin() + first_option, LINE.end());
	//reading data in file
	fstream file;
	file.open(FILE_NAME);
	if (file.fail())
	{
		cout << FILE_NAME << "The system cannot find the file specified." << endl;
		return;
	}
	//reading data in array
	string temp;
	while (getline(file, temp))
		FILE_CONTENTS[total++] = temp;
	file.close();
	//sort data
	sort(FILE_CONTENTS, FILE_CONTENTS + total);
	//print data on Stream
	for (int i = 0; i < total; i++)
		Output << FILE_CONTENTS[i] << "\n";

	if (PIPELINE)
		pipelineStream(Output, total);
	else if (PRINTINFILE)
		printStreamInFile(Output, LINE);
	else
		printStream(Output);
	
}
void CopyCommand() 
{
	int last_option = -1;
	bool Confirm = true;
	if (contain(LINE, "/Y") != -1)
	{
		max(last_option, contain(LINE, "/Y"));
		Confirm = false;
	}
	if (last_option != -1)
		LINE.erase(LINE.begin(), LINE.begin() + last_option + 2);

	string From = seperate(LINE);
	string To = seperate(LINE);
	int Copied_Files = 0;
	bool sameName = false;

	getPath(From);
	getPath(To);
	if (To.length() == 3)
		To += "*";
	if (From.length() == 3)
		From += "*";

	WIN32_FIND_DATA WFD,WFD2;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	
	HANDLE_FIND = FindFirstFile((To).c_str(), &WFD2);

	if ((INVALID_HANDLE_VALUE != HANDLE_FIND) && (isDir(WFD2)))
	{
		while (lastChar(To) != '\\')
			deleteLastChar(To);

		if (WFD2.cFileName[0] == '$' || WFD2.cFileName[0] == '.')
			;
		else
			To = To + WFD2.cFileName + "\\";
		sameName = true;
	}

	HANDLE_FIND = FindFirstFile((From).c_str(), &WFD);
	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
	{
		cout << "\nThe system cannot find the file(s) specified.\n       0 file(s) copied.\n";
		return;
	}
		
	while (lastChar(From) != '\\')
		deleteLastChar(From);
	if ((isDir(WFD)) && WFD.cFileName[0] != '.' && WFD.cFileName[0] != '$')
	{
		From = From + WFD.cFileName + "\\";
		HANDLE_FIND = FindFirstFile((From + "*").c_str(), &WFD);
	}
	string top=To;
	do
	{
		if (sameName)
			To = top + WFD.cFileName;

		if (!(isDir(WFD)))
		{
			if (!isExist(To))
			{
				if (CopyFile((From+WFD.cFileName).c_str(), To.c_str(), FALSE))
					Copied_Files++;
			}
			else if (!Confirm)
			{
				if (CopyFile((From + WFD.cFileName).c_str(), To.c_str(), FALSE))
					Copied_Files++;
			}
			else
			{
				string Answer;
				cout << "Overwrite " << To << "? (Yes/No/All):";
				getline(cin, Answer);
				toLowercase(Answer);
				if (Answer == "yes" || Answer=="y")
				{
					if (CopyFile((From + WFD.cFileName).c_str(), To.c_str(), FALSE))
						Copied_Files++;
				}
				else if (Answer == "all" || Answer=="a")
				{
					Confirm = false;
					if (CopyFile((From + WFD.cFileName).c_str(), To.c_str(), FALSE))
						Copied_Files++;
				}
			}
		}
	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);
	if (Copied_Files)
		cout << "       " << Copied_Files << " file(s) copied.\n";
	else
		cout << "       0 file(s) copied.\n";
}
void RemoveDirCommand(string PATH)
{
	if (PATH.length() == 0)
	{
		cout << "\nThe syntax of the command is incorrect.\n";
		return;
	}

	getPath(PATH);

	if (isExist(PATH))
	{
		if (!RemoveDirectory(PATH.c_str()))
			cout << "The directory is not empty.\n";
	}
	else
		cout << "\nThe system cannot find the file specified.";
}
void RenameCommand()
{
	if (LINE.length() <= 2)
	{
		cout << "\nThe syntax of the command is incorrect.\n";
		return;
	}

	string New_Name="";
	string Old_Name = "";
	for (int i = LINE.length()-1; i >=1; i--)
	{
		New_Name = lastChar(LINE) + New_Name;
		deleteLastChar(LINE);
		if (lastChar(LINE) == ' ')
		{
			deleteLastChar(LINE);
			break;
		}
	}

	getPath(LINE);

	while (lastChar(LINE) != '\\')
	{
		Old_Name = lastChar(LINE) + Old_Name;
		deleteLastChar(LINE);
	}

	if (isExist(LINE+ Old_Name))
	{
		int Err = rename((LINE+Old_Name).c_str(), (LINE+New_Name).c_str());
		if (Err)
			cout << "A duplicate file name exists, or the file cannot be found.\n";
	}
	else
		cout << "\nThe system cannot find the file specified.";
}
void DeleteCommand(string PATH)
{

	if (PATH.length() == 0)
	{
		cout << "\nThe syntax of the command is incorrect.\n";
		return;
	}

	getPath(PATH);

	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	HANDLE_FIND = FindFirstFile((PATH).c_str(), &WFD);

	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
	{
		cout << "\nThe system cannot find the file specified.";
		return;
	}

	while (lastChar(PATH) != '\\')
		deleteLastChar(PATH);

	do
	{
		if (!(isDir(WFD)))
			remove((PATH + WFD.cFileName).c_str());
		else if ( WFD.cFileName[0] != '.')
		{
			cout << (PATH + WFD.cFileName + "\\*") << ", Are you sure (Y/N)?";
			string confirm;
			getline(cin, confirm);
			if (confirm =="Y" || confirm=="y")
				DeleteCommand(PATH+ WFD.cFileName+"\\*");
		}
	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);
}
void makeDirCommand(string path)
{
	getPath(path);
	if (isExist(path))
		cout << "A subdirectory or file " << path << " already exists.";
	else 
		CreateDirectory(path.c_str(), NULL);
}
void TypeCommand()
{
	if (LINE.length() == 0)
	{
		cout << "\nThe syntax of the command is incorrect.\n";
		return;
	}

	stringstream Output;
	int first_option = MAX_PATH;
	bool PIPELINE = false;
	int total = 0;
	//check if | MORE
	if (contain(LINE, "|") != -1 && contain(LINE, "more") != -1)
	{
		PIPELINE = true;
		first_option = min(first_option, contain(LINE, "|"));
	}
	bool PRINTINFILE = false;
	string printInPath = "";
	if (contain(LINE, ">") != -1)
	{
		first_option = min(first_option, contain(LINE, ">"));
		PRINTINFILE = true;
		for (int i = contain(LINE, ">") + 2; i < LINE.length(); i++)
			printInPath += LINE[i];
	}
	if (first_option != MAX_PATH)
		LINE.erase(LINE.begin() + first_option, LINE.end());

	getPath(LINE);

	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	HANDLE_FIND = FindFirstFile((LINE).c_str(), &WFD);

	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
	{
		cout << "\nThe system cannot find the file specified.";
		return;
	}

	while (lastChar(LINE)!='\\')
		deleteLastChar(LINE);

	do
	{
		if (!(isDir(WFD)))
		{
			Output << "\n" << LINE << WFD.cFileName << "\n\n";
			fstream file(LINE + WFD.cFileName, ios::in | ios::out);
			string line;
			while (getline(file, line))
					Output << line << endl, total++;
			file.close();
			Output << "________________________________________________________________________", total += 4;
		}
	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);

	if (PIPELINE)
		pipelineStream(Output, total);
	else if (PRINTINFILE)
		printStreamInFile(Output, LINE);
	else
		printStream(Output);
}
void HelpCommand()
{
	if (LINE.length() == 0)
		cout << endl
		<< "ATTRIB         Displays or changes file attributes.\n"
		<< "CHDIR -CD      Changes the current directory.\n"
		<< "CLS            Clears the screen.\n"
		<< "COLOR          Sets the default console foreground and background colors.\n"
		<< "COMP           Compares the contents of two files or sets of files.\n"			//***
		<< "COPY           Copies one or more files to another location.\n"
		<< "DATE           Displays or sets the date.\n"
		<< "DIR            Displays a list of files and subdirectories in a directory.\n"
		<< "ERASE -DEL     Deletes one or more files.\n"
		<< "EXIT           Quits the CMD.EXE program (command interpreter).\n"
		<< "FIND           Searches for a text string in a file or files.\n"
		<< "MKDIR -MD      Creates a directory.\n"
		<< "MORE           Displays output one screen at a time.\n"
		<< "MOVE           Moves one or more files from one directory to another directory.\n"
		<< "RMDIR -RD      Removes a directory.\n"
		<< "RENAME -REN    Renames a file or files.\n"
		<< "SORT           Sorts input.\n"
		<< "TIME           Displays or sets the system time.\n"
		<< "TYPE           Displays the contents of a text file.\n"
		<< "XCOPY          Copies files and directory trees.\n"							//***
		<< "'>'            Prints Commands output in File. \n\n";
	else
	{
		string command = seperate(LINE);
		toLowercase(command);

		if (command == "dir")
		{
			cout << "Displays a list of files and subdirectories in a directory.\n\n"
				<< "DIR[drive:][path][filename][/ A[[:]attributes]][/ P][/ W][/T[[:]timefield]]\n\n"
				<< "[drive:][path][filename]\n"
				<< "Specifies drive, directory, and / or files to list.\n\n"
				<< "/ A          Displays files with specified attributes.\n"
				<< "attributes   D  Directories                R  Read - only files\n"
				<< "             H  Hidden files               A  Files ready for archiving\n"
				<< "             S  System files\n"
				<< "/P           Pauses after each screenful of information.\n"
				<< "/W           Uses wide list format.\n"
				<< "/T           Controls which time field displayed.\n"
				<< "timefield    C  Creation\n"
				<< "             A  Last Access\n"
				<< "             W  Last Written\n";
		}
		else if (command == "cd" || command == "chdir")
		{
			cout << "Displays the name of or changes the current directory.\n\n"
				<< "CHDIR[drive:][path]\n"
				<< "CHDIR[..]\n"
				<< "CD[drive:][path]\n"
				<< "CD[..]\n\n"
				<< " [..] Specifies that you want to change to the parent directory.\n\n";
		}
		else if (command == "del" || command == "erase")
		{
			cout << "Deletes one or more files.\n\n"
				<< "DEL name\n"
				<< "ERASE name\n\n"
				<< "names         Specifies a files or directorie.\n"
				<< "              Wildcards may be used to delete multiple files.If a\n"
				<< "              directory is specified, all files within the directory\n"
				<< "              will be deleted.\n\n";
		}
		else if (command == "mkdir" || command == "md")
		{
			cout << "Creates a directory.\n\n"

				<< "MKDIR[drive:]path\n"
				<< "MD[drive:]path\n\n";
		}
		else if (command == "sort")
		{
			cout << "SORT [[drive1:][path1]filename1] [/ O[drive3:][path2]filename2] \n\n"
				<< "/ O[UTPUT]\n"
				<< "	[drive3:][path3]filename3 Specifies the file where the sorted input is\n"
				<< "							to be stored.If not specified, the data is\n"
				<< "							written to the standard output.Specifying\n"
				<< "							the output file is faster than redirecting\n"
				<< "							standard output to the same file.\n\n";
		}
		else if (command == "copy")
		{
			cout << "Copies one or more files to another location.\n\n"
				<< "COPY [/Y] source destination\n\n"
				<< "/Y           Suppresses prompting to confirm you want to overwrite an\n"
				<< "             existing destination file.\n"
				<< "source       Specifies the file or files to be copied.\n"
				<< "destination  Specifies the directory and / or filename for the new file(s).\n\n";
		}
		else if (command == "move")
		{
			cout << "Moves files and renames files.\n\n"
				<< "To move one or more files :\n"
				<< "MOVE[drive:][path]filename1[, ...] destination\n\n";
		}
		else if (command == "attrib")
		{
			cout << "Displays or changes file attributes.\n\n"
				<< "ATTRIB [+R|-R][+A|-A][+S|-S][+H|-H][drive:][path][filename]\n\n"
				<< "	+ Sets an attribute.\n"
				<< "	- Clears an attribute.\n"
				<< "	R   Read - only file attribute.\n"
				<< "	A   Archive file attribute.\n"
				<< "	S   System file attribute.\n"
				<< "	H   Hidden file attribute.\n"
				<< "	[drive:][path][filename]\n"
				<< "	Specifies a file or files for attrib to process.\n\n";
		}
		else if (command == "more")
		{
			cout << "MORE  [drive:][path]filename\n"
				<< "command - name | MORE\n"
				<< "MORE [files]\n\n";
		}
		else if (command == "xcopy")
		{
			cout << "Copies files and directory trees.\n\n"
				<< "XCOPY source destination [/Y][/E] \n\n"
				<< "/Y           Suppresses prompting to confirm you want to overwrite an\n"
				<< "             existing destination file.\n"
				<< "/E           Copies directories and subdirectories, including empty ones.\n"
				<< "source       Specifies the file(s) to copy.\n"
				<< "destination  Specifies the location and / or name of new files.\n\n";
		}
		else if (command == "comp")
		{
			cout << "Compares the contents of two files or sets of files.\n\n"
				<< "COMP[data1][data2][/ A][/ N = number][/ C]\n\n"
				<< "data1        Specifies location and name(s) of first file(s) to compare.\n"
				<< "data2        Specifies location and name(s) of second files to compare.\n"
				<< " /D          Displays differences in decimal format.\n"
				<< " /A          Displays differences in ASCII characters.\n"
				<< " /L          Displays line numbers for differences.\n"
				<< " /N=number   Compares only the first specified number of lines in each file.\n"
				<< " /C          Disregards case of ASCII letters when comparing files.\n"
				<< "             To compare sets of files, use wildcards in data1 and data2 parameters.\n\n";
		}
		else if (command == "find")
		{
			cout << "Searches for a text string in a file or files.\n\n"
				<< "FIND[/ V][/ C][/ N][/ I][/ OFF[LINE]] \"string\"[[drive:][path]filename[...]]\n\n"
				<< " /V         Displays all lines NOT containing the specified string.\n"
				<< " /C         Displays only the count of lines containing the string.\n"
				<< " /N         Displays line numbers with the displayed lines.\n"
				<< " /I         Ignores the case of characters when searching for the string.\n"
				<< "\"string\"   Specifies the text string to find.\n"
				<< "[drive:][path]filename\n"
				<< "Specifies a file or files to search.\n\n";
		}
		else if (command == "rmdir" || command == "rd")
		{
			cout << "Removes (deletes) a directory.\n\n"
				<< "RMDIR [drive:]path\n"
				<< "RD [drive:]path\n\n";
		}
		else if (command == "rename" || command == "ren")
		{
			cout << "Renames a file or files.\n\n"
				<< "RENAME[drive:][path]filename1 filename2.\n"
				<< "REN[drive:][path]filename1 filename2.\n\n"
				<< "Note that you cannot specify a new drive or path for your destination file.\n\n";
		}
		else if (command == "type")
		{
			cout << "Displays the contents of a text file or files.\n\n"
				<< "TYPE[drive:][path]filename\n\n";
		}
		else if (command == "help")
		{
			cout << "Provides help information for Windows commands.\n\n"
				<< "HELP[command]\n\n"
				<< "command - displays help information on that command.\n\n";
		}
		else if (command == "color")
		{
			cout << "Sets the default console foreground and background colors.\n\n"
				<< "COLOR [Font Color] [Background Color]\n\n"
				<< "   Black       Gray\n"
				<< "   Blue        LightBlue\n"
				<< "   Green       LightGreen\n"
				<< "   Aqua        LightAqua\n"
				<< "   Red         LightRed\n"
				<< "   Purple      LightPurple\n"
				<< "   Yellow      LightYellow\n"
				<< "   White       BrightWhite\n"
				<< "If no argument is given, this command restores the color to what it was\n"
				<< "when CMD.EXE started.\n\n";
		}
		else if (command == "date")
		{
			cout << "Displays or sets the date.\n\n"
				<< "DATE [date]\n\n";
		}
		else if (command == "time")
		{
			cout << "Displays or sets the time.\n\n"
				<< "TIME [time]\n\n";
		}
		else if (command == "cls")
		{
			cout << "Clears the screen.\n\n"
				<< "CLS\n\n";
		}
		else if (command == "exit")
		{
			cout << "Quits the CMD.EXE program(command interpreter) or the current batch\n"
				<< "script.\n\n"
				<< "EXIT \n\n";
		}
	}
}
void ChangeTimeCommand()
{
	SYSTEMTIME NEW_TIME;
	stringstream ss;
	int tempInt;
	char c;
	if (!LINE.length() > 0)
	{
		GetSystemTime(&NEW_TIME);
		cout << "The current time is: " << NEW_TIME.wHour+2 << ':' << NEW_TIME.wMinute << ':' << NEW_TIME.wSecond << endl;
		cout << "Enter the new time: (hh:mm:ss) ";

		getline(cin, LINE);
		LINE = seperate(LINE);
	}
	ss << LINE;

	ss >> tempInt;
	NEW_TIME.wHour = tempInt-2;

	ss >> c;
	ss >> tempInt;
	NEW_TIME.wMinute = tempInt;

	ss >> c;
	ss >> tempInt;
	NEW_TIME.wSecond = tempInt;

	SetSystemTime(&NEW_TIME);
	return;
}
void ChangeDateCommand()
{
	SYSTEMTIME NEW_TIME;
	stringstream ss;
	int tempInt;
	char c;
	if (!LINE.length() > 0)
	{
		GetSystemTime(&NEW_TIME);
		cout << "The current date is: " << NEW_TIME.wYear << '-' << NEW_TIME.wMonth << '-' << NEW_TIME.wDay << endl;
		cout << "Enter the new date: (yy-mm-dd) ";
		getline(cin, LINE);
		LINE = seperate(LINE);
	}
	ss << LINE;
	
	ss >> tempInt;
	NEW_TIME.wYear = tempInt;
	
	ss >> c;
	ss >> tempInt;
	NEW_TIME.wMonth = tempInt;

	ss >> c;
	ss >> tempInt;
	NEW_TIME.wDay = tempInt;

	SetSystemTime(&NEW_TIME);
	return;
}
void ColorCommand()
{
	if (LINE.length() == 0)
	{
		system("color 07");
		cout <<"Enter color of Font then Color of Background\n"
			<< "   Black       Gray\n" 
			<< "   Blue        LightBlue\n" 
			<< "   Green       LightGreen\n" 
			<< "   Aqua        LightAqua\n"
			<< "   Red         LightRed\n" 
			<< "   Purple      LightPurple\n" 
			<< "   Yellow      LightYellow\n" 
			<< "   White       BrightWhite\n"
			<< "If no argument is given, this command restores the color to what it was\n"
			<< "when CMD.EXE started.\n\n";
	}
	else
	{
		string fontColor = seperate(LINE);
		string backgroundColor = LINE;
		char fc, bc;
		fc = getColorNumber(fontColor);
		bc = getColorNumber(backgroundColor);
		string temp = "color ";
		temp += bc;
		temp+=fc;
		system(temp.c_str());
	}
}
void ChangeDriveCommand()
{
	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	HANDLE_FIND = FindFirstFile((COMMAND + "\\*").c_str(), &WFD);

	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
		cout << "The system cannot find the drive specified.\n";
	else
	{
		if (COMMAND[0] >= 'a' && COMMAND[0] <= 'z')
			COMMAND[0] -= 'a' - 'A';
		CURRENT_PATH = COMMAND + '\\';
	}
}
void ChangeDirectoryCommand()
{

	if (LINE[0] == '.' || COMMAND=="cd..")	// if wants to return back
	{
		while (LINE.length() >= 2 && LINE[0] == '.' && LINE[1] == '.')
		{
			while (lastChar(CURRENT_PATH) != '\\')
				deleteLastChar(CURRENT_PATH);
			if (!inMainDrive())
				deleteLastChar(CURRENT_PATH);
			LINE.erase(LINE.begin(), LINE.begin() + 2);
			if (LINE.length() >= 1)
				LINE.erase(LINE.begin(), LINE.begin()+1);
		}
		return;
	}
	if (LINE == "\\" || COMMAND == "cd\\")
	{
		CURRENT_PATH = CURRENT_PATH[0];
		CURRENT_PATH += ":\\";
		return;
	}

	getPath(LINE);

	if (isExist(LINE) && isDirectory(LINE))
		CURRENT_PATH = LINE;
	else
		cout << "The system cannot find the path specified.\n";
}
void DirCommand(string path)
{
	stringstream Output;
	int total = 0;

	int first_option = MAX_PATH;
	bool READONLY = false;
	bool NOTCARE_R = true;

	bool ARCHIVE = false;
	bool NOTCARE_A = true;
	
	bool SYSTEM = false;
	bool NOTCARE_S = true;
	
	bool HIDDEN = false;
	bool NOTCARE_H = false;
	
	bool DIRECTORY = false;
	bool NOTCARE_D = true;
	if (contain(path, "/A") != -1)
	{
		first_option = min(first_option, contain(path, "/A"));
		NOTCARE_H = true;
		for (int i = contain(path, "/A") + 2; path[i] != ' ' && i < path.length(); i++)
		{
			if (path[i] == 'R' || path[i] == 'r')
				READONLY = true, NOTCARE_R = false;
			else if (path[i] == 'A' || path[i] == 'a')
				ARCHIVE = true, NOTCARE_A = false;
			else if (path[i] == 'S' || path[i] == 's')
				SYSTEM = true, NOTCARE_S = false;
			else if (path[i] == 'H' || path[i] == 'h')
				HIDDEN = true, NOTCARE_H = false;
			else if (path[i] == 'D' || path[i] == 'd')
				DIRECTORY = true, NOTCARE_D = false;
			else if (path[i] == '-')
			{
				i++;
				if (path[i] == 'R' || path[i] == 'r')
					READONLY = false, NOTCARE_R = false;
				else if (path[i] == 'A' || path[i] == 'a')
					ARCHIVE = false, NOTCARE_A = false;
				else if (path[i] == 'S' || path[i] == 's')
					SYSTEM = false, NOTCARE_S = false;
				else if (path[i] == 'H' || path[i] == 'h')
					HIDDEN = false, NOTCARE_H = false;
				else if (path[i] == 'D' || path[i] == 'd')
					DIRECTORY = false, NOTCARE_D = false;
			}
		}
	}
	bool PIPELINE = false;
	bool ONLYNAME = false;
	bool PRINTINFILE = false;
	int TIMETYPE = 0;		// 0 == (W)Modified time , 1 == Last (A)ccess Time , 2 == (C)reation Time
	string printInPath = "";
	if (contain(path, "/T") != -1)
	{
		first_option = min(first_option, contain(path, "/T"));
		if (path[contain(path, "/T") + 2] == 'A')
			TIMETYPE = 1;
		else if (path[contain(path, "/T") + 2] == 'C')
			TIMETYPE = 2;
		else
			TIMETYPE = 0;
	}
	if (contain(path, ">") != -1)
	{
		first_option = min(first_option, contain(path, ">"));
		PRINTINFILE = true;
		for (int i = contain(path, ">") + 2; i < path.length(); i++)
			printInPath += path[i];
	}
	if (contain(path, "/P") != -1)
	{
		first_option = min(first_option, contain(path,"/P"));
		PIPELINE = true;
	}
	if (contain(path, "|") !=-1 && (contain(path, "more")) != -1)
	{
		PIPELINE = true;
		first_option = min(first_option, contain(path, "|"));
	}
	if (contain(path, "/W") != -1)
	{
		first_option = min(first_option, contain(path, "/W"));
		ONLYNAME = true;
	}
	if (first_option != MAX_PATH)
		path.erase(path.begin() + first_option, path.end());

	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA WFD;

	getPath(path);

	if (path.length() == 3) // If Path  InMainDrive ex."C:\"
		path += "*";
	else if (isDirectory(path))
		path += "\\*";

	// Find the first file in the directory. 
	HANDLE_FIND = FindFirstFile((path).c_str(), &WFD); 

	if (INVALID_HANDLE_VALUE == HANDLE_FIND) 
	{ 		
		cout << "File Not Found.\n";
		return ; 
	} 

	char Label[MAX_PATH];
	DWORD VolumeSerialNumber = 0;
	string drive = path;
	drive.erase(drive.begin() + 3, drive.end());

	GetVolumeInformation(drive.c_str(),
		Label,
		sizeof(Label),
		&VolumeSerialNumber,
		NULL,
		NULL,
		NULL,
		NULL);

	while (lastChar(path) != '\\')
		deleteLastChar(path);

	Output << " Volume in drive " << drive[0] << " is " << Label;
	Output << "\n Volume Serial Number  " << uppercase << hex << HIWORD(VolumeSerialNumber) << "-" << LOWORD(VolumeSerialNumber) << "\n\n";
	Output << nouppercase << dec;
	Output << "  Directory of " << path << " \n\n";
	
	int COUNT_DIRS = 0; 	 
	int COUNT_FILES = 0; 
	unsigned long long ALL_FILES_SIZE = 0; 	
	if (!ONLYNAME)
	{
		if (TIMETYPE == 1)
			Output << " (Last Access Time)  \t    (Size)    (Name)\n";
		else if (TIMETYPE == 2)
			Output << " (Creation Time)     \t    (Size)    (Name)\n";
		else
			Output << " (Last Modified Time)\t    (Size)    (Name)\n";
	}
	do
	{
		if ((!(isHid(WFD) ^ HIDDEN) || NOTCARE_H)			//  /A[A,H,R,S,D] Attributes
			&& (!(isArch(WFD) ^ ARCHIVE) || NOTCARE_A)
			&& (!(isReadOnly(WFD) ^ READONLY) || NOTCARE_R)
			&& (!(isSys(WFD) ^ SYSTEM) || NOTCARE_S)
			&& (!(isDir(WFD) ^ DIRECTORY) || NOTCARE_D))
		{
		
			FILETIME filetime;
			SYSTEMTIME systemtime;
			if (TIMETYPE == 1)
				FileTimeToLocalFileTime(&WFD.ftLastAccessTime, &filetime);
			else if (TIMETYPE == 2)
				FileTimeToLocalFileTime(&WFD.ftCreationTime, &filetime);
			else
				FileTimeToLocalFileTime(&WFD.ftLastWriteTime, &filetime);

			FileTimeToSystemTime(&filetime, &systemtime);
			if (ONLYNAME)
			{
				if (isDir(WFD))
					Output << "[" << WFD.cFileName << "]\n", COUNT_DIRS++, total++;
				else
				{
					ALL_FILES_SIZE += WFD.nFileSizeHigh*(MAXDWORD)+WFD.nFileSizeLow;
					Output << WFD.cFileName << "\n", COUNT_FILES++, total++;
				}
			}
			else
			{
				Output << systemtime.wYear << "-" << setw(2) << setfill('0') << systemtime.wMonth << "-" << setw(2) << systemtime.wDay
					<< "  " << setw(2) << systemtime.wHour << ":" << setw(2) << systemtime.wMinute,total++;
				if (isDir(WFD))
					Output << "    <DIR>          " << WFD.cFileName << "\n", COUNT_DIRS++, total++;
				else
				{
					unsigned long long FILE_SIZE = WFD.nFileSizeHigh*(MAXDWORD)+WFD.nFileSizeLow;
					ALL_FILES_SIZE += FILE_SIZE;
					Output.imbue(locale(""));
					Output << setw(17) << setfill(' ') << FILE_SIZE <<"  "<< WFD.cFileName << "\n", COUNT_FILES++, total++;
					Output.imbue(locale());
				}
			}
		}
	} while (FindNextFile(HANDLE_FIND, &WFD) != 0);

	Output.imbue(locale(""));
	Output << "\t    " << COUNT_FILES << " File(s)\t" << ALL_FILES_SIZE << " Bytes\n";
	Output << "\t    " << COUNT_DIRS << " Dir(s) \t ";
	Output.imbue(locale());

	if (PIPELINE)
		pipelineStream(Output, total);
	else if (PRINTINFILE)
		printStreamInFile(Output, printInPath);
	else
		printStream(Output);
}
int contain(string str,string key)
{
	toLowercase(str);
	toLowercase(key);
	return str.find(key);
}
void toLowercase(string &s)
{ 	
	for (int i = 0; i < s.length(); i++) 
	{ 		
		if (s[i] >= 'A' && s[i] <= 'Z') 	
			s[i] += 'a' - 'A';
	}
}
bool inMainDrive()
{
	if (CURRENT_PATH[1] == ':' && CURRENT_PATH.length() == 3)
		return true;
	return false;
}
void getPath(string &filename)
{
	if (filename.length() >= 3 && filename[1] == ':')
		return;
	else if (inMainDrive() || filename.length() == 0)
		filename = CURRENT_PATH + filename;
	else
		filename = CURRENT_PATH + '\\' + filename;
}
bool isExist(string &path)
{
	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	if (path.length() == 0)
		path += "*";
	HANDLE_FIND = FindFirstFile((path).c_str(), &WFD);

	if (INVALID_HANDLE_VALUE == HANDLE_FIND)
		return false;
	//to Return Identical file Name
	while (lastChar(path) != '\\')
		deleteLastChar(path);

	path = path + WFD.cFileName;

	FindClose(HANDLE_FIND);

	return true;
}
bool isDirectory(string path)
{
	WIN32_FIND_DATA WFD;
	HANDLE HANDLE_FIND = INVALID_HANDLE_VALUE;
	HANDLE_FIND = FindFirstFile((path).c_str(), &WFD);

	if (WFD.cFileName[0] == '.' || WFD.cFileName[0] == '$' || lastChar(path) == '.')
		return false;
	if (isDir(WFD))
		return true;
	return false;
}
string seperate(string & STRING, char delim)
{
	string subString = "";
	if (STRING == "")
		return "";
	while ( STRING[0] == ' ')
		STRING.erase(STRING.begin(), STRING.begin() + 1);

	while (STRING.length() > 0 && lastChar(STRING ) == ' ')
		deleteLastChar(STRING);

	int take = -1;
	for (int i = 0; i < STRING.length(); i++)
	{
		if (STRING[i] == '\"')
			take *= -1;
		else if (STRING[i] != delim || take == 1)
			subString += STRING[i];
		else
		{
			STRING.erase(STRING.begin(), STRING.begin() + i + 1);
			return subString;
		}
	}
	STRING = "";
	return subString;
}
char getColorNumber(string color)
{
	toLowercase(color);
	if (color == "black")
		return '0';
	else if (color == "blue")
		return '1';
	else if (color == "green")
		return '2';
	else if (color == "aqua")
		return '3';
	else if (color == "red")
		return '4';
	else if (color == "purple")
		return '5';
	else if (color == "yellow")
		return '6';
	else if (color == "white")
		return '7';
	else if (color == "gray")
		return '8';
	else if (color == "lightblue")
		return '9';
	else if (color == "lightgreen")
		return 'A';
	else if (color == "lightaqua")
		return 'B';
	else if (color == "lightred")
		return 'C';
	else if (color == "lightpurple")
		return 'D';
	else if (color == "lightyellow")
		return 'E';
	else
		return 'F';
}
void printFile(string path)
{
	fstream file(path, ios::in | ios::out);
	if (file.fail())
	{
		cout << LINE << "The system cannot find the file specified." << endl;
		return;
	}
	string s;
	while (getline(file, s))
		cout << s << endl;
	file.close();
}
int getFileSize(string Filename) 
{
	ifstream f(Filename);
	int end;
	f.seekg(0, ios::end);
	end = f.tellg();
	f.close();
	return end;
}
void printStream(stringstream &Output)
{
	string line;
	while (getline(Output, line))
		cout << line << endl;
}
void pipelineStream(stringstream &Output, int total)
{
	string line;
	double num = 0;
	char in;
	int i = 0;
	while (true)
	{
		for (; i < 30; i++)
		{
			if (num >= total)
				return;
			getline(Output, line);
			cout << line << endl;
			num++;
		}
		cout << "-- More (" << int((num / total)*100.0) << "%) -- ";
		in = _getche();
		if (in == '\r')
			i = 29;
		if (in == ' ')
			i = 0;
		cout << endl;
	}
}
void printStreamInFile(stringstream &Output, string path)
{
	ofstream file;
	string line;
	file.open(path, std::ios::app);
	if (file.fail())
	{
		cout << LINE << "The system cannot find the file specified." << endl;
		return;
	}
	while (getline(Output, line))
		file << line << "\n";
	file.close();
}