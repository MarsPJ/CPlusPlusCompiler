#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<queue>
#include<stack>
#include<map>
#include<vector>
#include<list>
using namespace std;
//"\n"��processSeparator�д����ֱ�����6
class Token
{
public:
	Token() { category = 0; value = ""; };
	Token(int cat, string val) :category(cat), value(val) {};
	//�ֱ���
	int category;
	//ֵ
	string value;
};
//��������
struct Help
{
	//��ǰ�ڼ���
	int row;
	//1��ʾ��ǰƥ��ɹ���0��ʾʧ��
	bool flag;
	//��¼����ԭ��
	string error;
	/*
	���������ž�+1������������-1
	�κ�ʱ�����parenthesis<0,�򱨴�
	����ʷ���������������parenthesis!=0,�򱨴�
	*/
	//С����������
	int parenthesis;
	//������������
	int bracket;
	//������������
	int brace;
};
class LEXER
{
public:
	//DATA�洢�ļ�����
	string DATA;
	//Token���
	queue<Token> RESULT;
	//�ؼ��ֺͱ�����
	string* KEY_WORDS;
	int totalKeyNum;
	//��������
	Help HELP;
	LEXER() {};
	void operator=(const LEXER& L)
	{
		DATA = L.DATA;
		RESULT = L.RESULT;
		totalKeyNum = L.totalKeyNum;
		HELP = L.HELP;
		KEY_WORDS = new string[totalKeyNum];
		for (int i = 0; i < totalKeyNum; i++)
			KEY_WORDS[i] = L.KEY_WORDS[i];
	}
	LEXER(string filename)
	{
		//�õ�Դ�����ļ�����
		ifstream F(filename);
		istreambuf_iterator<char> Begin(F);
		istreambuf_iterator<char> End;
		string myDATA(Begin, End);
		DATA = myDATA;
		string temp[37] = {
		"auto", "enum" ,"restrict", "unsigned",
		"break" ,"extern", "return" ,"void",
		"case" ,"float" ,"short" ,"volatile",
		"char" ,"for" ,"signed" ,"while",
		"const", "goto" ,"sizeof" "_Bool",
		"continue", "if" ,"static", "_Complex",
		"default", "inline" ,"struct" ,"_Imaginary",
		"do" ,"int" ,"switch",
		"double", "long", "typedef",
		"else" ,"register" ,"union" };
		totalKeyNum = 37;
		KEY_WORDS = new string[totalKeyNum];
		for (int i = 0; i < totalKeyNum; i++)KEY_WORDS[i] = temp[i];
		HELP = { 1 ,true ,"" ,0,0,0 };
	}
	//�����һ���ַ�
	bool getNext(char& ch);
	//�����һ���ǿհ׷����Ʊ�����ַ�
	bool getChar(char& ch);
	//�ж��Ƿ���Ӣ����ĸ
	bool isLetter(char ch);
	//�ж��Ƿ��ǹؼ��ֻ�����
	bool isKeyWord(string str);
	//�ж��Ƿ���0
	bool isZero(char ch);
	//�ж��Ƿ��Ƿ�0����
	bool isNotZero(char ch);
	//�ж��Ƿ�������
	bool isDigit(char ch);
	//�ж��Ƿ���С����
	bool isPoint(char ch);
	//�ж��Ƿ���+-��
	bool isSign(char ch);
	//�ж��Ƿ��Ƿָ���
	bool isSeparator(char ch);
	//��һ���ж���;�����ķǿո�ͷ��Ʊ���ķָ�������SESULT
	void processSeparator(char ch);
	//�ж��Ƿ��ǵ�����
	bool isSingleQuo(char ch);
	//�ж��Ƿ���˫����
	bool isDoubleQuo(char ch);
	//�ж��Ƿ���/
	bool isSlash(char ch);
	//�ж��Ƿ���*
	bool isSasterisk(char ch);
	//�ж��Ƿ���/��*
	bool isAnnotation(char ch);
	//����ע�ͷ���
	void processAnnotation(char ch);
	//�ʷ������������ĸ�������
	void lexerHelp();
	//�ʷ�����������
	void Lexer();
	//������дʷ�����֮��Ľ��
	void printToken();
};
//�����һ���ַ�
bool LEXER::getNext(char& ch)
{
	if (DATA.size() == 0)return false;
	ch = DATA[0];
	DATA = DATA.substr(1, DATA.size());
	return true;
}
//�����һ���ǿհ׷����Ʊ�����ַ�
bool LEXER::isSeparator(char ch)
{
	return (ch == ' ' || ch == '\t'
		|| ch == '\n'
		|| ch == ';' || ch == ','
		|| ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '(' || ch == ')'
		|| ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%'
		|| ch == '<' || ch == '>' || ch == '=' || ch == '!' || ch == ':'
		|| ch == '&' || ch == '|');
}
//��һ���ж���;�����ķǿո�ͷ��Ʊ���ķָ�������SESULT
void LEXER::processSeparator(char ch)
{
	if (ch == ' ' || ch == '\t')return;
	else if (ch == '\n')
	{
		HELP.row++;
		RESULT.push(Token(6, "\n"));
		return;
	}
	else if (ch == ';' || ch == ',' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '(' || ch == ')')
	{
		if (ch == '(')HELP.parenthesis++;
		else if (ch == ')')
		{
			HELP.parenthesis--;
			if (HELP.parenthesis < 0)
			{
				HELP.flag = 0;
				HELP.error = "')'can't be matched!";
				return;
			}
		}
		if (ch == '[')HELP.bracket++;
		else if (ch == ']')
		{
			HELP.bracket--;
			if (HELP.bracket < 0)
			{
				HELP.flag = 0;
				HELP.error = "']'can't be matched!";
				return;
			}
		}
		if (ch == '{')HELP.brace++;
		else if (ch == '}')
		{
			HELP.brace--;
			if (HELP.brace < 0)
			{
				HELP.flag = 0;
				HELP.error = "'}'can't be matched!";
				return;
			}
		}
		RESULT.push(Token(5, string(1, ch)));
		return;
	}
	else if ((ch == '|' && DATA[0] == '|') || (ch == '&' && DATA[0] == '&'))
	{
		RESULT.push(Token(4, string(1, ch) += ch));
		DATA = DATA.substr(1, DATA.size());
		return;
	}
	if (DATA[0] == '=')
	{
		RESULT.push(Token(4, string(1, ch) += '='));
		DATA = DATA.substr(1, DATA.size());
	}
	else RESULT.push(Token(4, string(1, ch)));
}
//�����һ���ǿհ׷����Ʊ�����ַ�
bool LEXER::getChar(char& ch)
{
	char temp;
	do
	{
		//������봮û����ʱ������false
		if (!DATA.size())return false;
		getNext(temp);
	} while (temp == ' ' || temp == '\t');
	ch = temp;
	return true;
}
//�ж��Ƿ���Ӣ����ĸ
bool LEXER::isLetter(char ch)
{
	return ch >= 65 && ch <= 90 || ch >= 97 && ch <= 122;
}
//�ж��Ƿ��ǹؼ��ֻ�����
bool LEXER::isKeyWord(string str)
{
	bool flag = 0;
	for (int i = 0; i < totalKeyNum; i++)
	{
		//cout << KEY_WORDS[i] << endl;
		if (str == KEY_WORDS[i])
		{

			flag = 1;
			break;
		}
	}
	return flag;
}
//�ж��Ƿ���0
bool LEXER::isZero(char ch)
{
	return ch == 48;
}
//�ж��Ƿ��Ƿ�0����
bool LEXER::isNotZero(char ch)
{
	return ch >= 49 && ch <= 57;
}
//�ж��Ƿ�������
bool LEXER::isDigit(char ch)
{
	return ch >= 48 && ch <= 57;
}
//�ж��Ƿ���С����
bool LEXER::isPoint(char ch)
{
	return ch == '.';
}
//�ж��Ƿ���+-��
bool LEXER::isSign(char ch)
{
	return ch == '+' || ch == '-';
}
//�ж��Ƿ��ǵ�����
bool LEXER::isSingleQuo(char ch)
{
	return ch == 39;
}
//�ж��Ƿ���˫����
bool LEXER::isDoubleQuo(char ch)
{
	return ch == 34;
}
//�ж��Ƿ���/
bool LEXER::isSlash(char ch)
{
	return ch == '/';
}
//�ж��Ƿ���*
bool LEXER::isSasterisk(char ch)
{
	return ch == '*';
}
//�ж��Ƿ���/��*
bool LEXER::isAnnotation(char ch)
{
	return isSlash(ch) || isSasterisk(ch);
}
//����ע�ͷ���
void LEXER::processAnnotation(char ch)
{
	//�ǡ�/��
	if (isSlash(ch))
	{
		//�ǡ�/*��
		if (isSasterisk(DATA[0]))
		{
			getNext(ch);
			do {
				if (!getChar(ch))
				{
					HELP.flag = 0;
					HELP.error = "Missing the right annotation��*/��";
					return;
				}
				if (ch == '\n')processSeparator(ch);
			} while (!(isSasterisk(ch) && DATA.size() && isSlash(DATA[0])));
			getNext(ch);
			return;
		}
		//�ǡ�//��
		else if (isSlash(DATA[0]))
		{
			getNext(ch);
			do {
				if (!getChar(ch))return;

			} while (ch != '\n');
			processSeparator(ch);
		}
		//�ǡ�/xxx��
		else
		{
			HELP.flag = 0;
			HELP.error = "�Ƿ���ʶ����";
		}
	}
	else
	{
		HELP.flag = 0;
		HELP.error = "�Ƿ���ʶ����";
	}

}
//�ʷ������������ĸ�������
void LEXER::lexerHelp()
{
	bool flag = 0;
	Token temp;
	char ch;
	if (!getChar(ch))return;
	//����һ��ʼ������ǻ��е����
	while (ch == '\n')
	{
		HELP.row++;
		RESULT.push(Token(6, "\n"));
		if (!DATA.size())return;
		getNext(ch);
	}
	//��ĸ
	if (isLetter(ch) || ch == '_')
	{
		temp.value += ch;
		//�ж��Ƿ�Ϊ��ĸ�����ֻ����»���
		while ((flag = getNext(ch)) && (isDigit(ch) || isLetter(ch) || ch == '_'))
		{
			temp.value += ch;
		}
		//��Ϊ�����ָ����������봮û��������Ϊ����������erro
		HELP.flag = isSeparator(ch) || !flag;
		if (HELP.flag)
		{
			//�ж��Ƿ�Ϊ�ؼ���
			if (isKeyWord(temp.value))temp.category = 3;
			else if (temp.value == "FALSE" || temp.value == "TRUE") temp.category = 2;
			else temp.category = 1;
			RESULT.push(temp);
			//����ָ���
			if (isSeparator(ch))processSeparator(ch);
		}
		else
		{
			//���ƥ���ch
			DATA = string(1, ch) + DATA;
			HELP.flag = 0;
			HELP.error = "Invalid identifier!";
		}
		return;
	}
	//0
	else if (isZero(ch))
	{
		//��0����temp
		temp.value += ch;
		//�����һ���Ƿָ���
		if (!getNext(ch) || isSeparator(ch))
		{
			temp.category = 2;
			RESULT.push(temp);
			if (isSeparator(ch))processSeparator(ch);
			return;
		}
		//�����һ����.
		else if (isPoint(ch))
		{
			//��.����temp
			temp.value += ch;
			//�����0.
			if (!getNext(ch) || isSeparator(ch))
			{
				temp.category = 2;
				RESULT.push(temp);
				if (isSeparator(ch))processSeparator(ch);
				return;
			}
			//0.xxxx
			while (isDigit(ch))
			{
				temp.value += ch;
				if (!getNext(ch))
				{
					temp.category = 2;
					RESULT.push(temp);
					return;
				}
			}
			//��Ϊ�����ָ����������봮û��������Ϊ����������erro
			HELP.flag = isSeparator(ch) || !DATA.size();
			if (HELP.flag)
			{
				temp.category = 2;
				RESULT.push(temp);
				if (isSeparator(ch))processSeparator(ch);
			}
			else
			{
				DATA = string(1, ch) + DATA;
				HELP.flag = 0;
				HELP.error = "Invalid floating number!";
			}
			return;
		}
		else
		{
			DATA = string(1, ch) + DATA;
			HELP.flag = 0;
			HELP.error = "Invalid identifier!";
		}
	}
	//����
	else if (isNotZero(ch))
	{
		temp.value += ch;
		while ((flag = getNext(ch)) && isDigit(ch))
		{
			temp.value += ch;
		}
		//����.
		if (isPoint(ch))
		{
			temp.value += ch;
			while ((flag = getNext(ch)) && isDigit(ch))
			{
				temp.value += ch;
			}
			if (isSeparator(ch) || !flag)
			{
				temp.category = 2;
				RESULT.push(temp);
				if (isSeparator(ch))processSeparator(ch);
				return;
			}
			else
			{
				DATA = string(1, ch) + DATA;
				HELP.flag = 0;
				HELP.error = "Invalid floating number!";
			}
		}
		else if (isSeparator(ch) || !flag)
		{
			temp.category = 2;
			RESULT.push(temp);
			if (isSeparator(ch))processSeparator(ch);
			return;
		}
		//����xxx(xΪ����.�����������)
		else
		{
			HELP.flag = 0;
			HELP.error = "Invalid identifier!";
		}
	}
	//.
	else if (isPoint(ch))
	{
		//��.����temp
		temp.value += ch;
		//�����.
		if (!getNext(ch))
		{
			DATA = string(1, ch) + DATA;
			HELP.flag = 0;
			HELP.error = "The decimal point cannot stand alone!";
			return;

		}
		//�����.xxx
		else if (isDigit(ch))
		{
			temp.value += ch;
			//���������ж����ʲô���Ĵ���
			while ((flag = getNext(ch)) && isDigit(ch))
			{
				temp.value += ch;
			}
			HELP.flag = isSeparator(ch) || !flag;
			if (HELP.flag)
			{
				temp.category = 2;
				RESULT.push(temp);
				if (isSeparator(ch))processSeparator(ch);
			}
			else
			{
				DATA = string(1, ch) + DATA;
				HELP.error = "Invalid floating number!";

			}
		}
		else
		{
			DATA = string(1, ch) + DATA;
			HELP.flag = 0;
			HELP.error = "Invalid identifier!";
		}
	}
	//+/-��
	else if (isSign(ch))
	{
		if (isDigit(RESULT.back().value[RESULT.back().value.size() - 1]) || isPoint(RESULT.back().value[RESULT.back().value.size() - 1]))
		{
			processSeparator(ch);
			return;
		}
		else
		{
			temp.value += ch;
			if (!getNext(ch))
			{
				processSeparator(ch);
				return;
			}
			else if (isPoint(ch))
			{
				//��.����temp
				temp.value += ch;
				//�����+.
				if (!getNext(ch))
				{
					DATA = string(1, ch) + DATA;
					HELP.flag = 0;
					HELP.error = "Invalid identifier!";
					return;

				}
				//�����+.xxx
				else if (isDigit(ch))
				{
					temp.value += ch;
					while ((flag = getNext(ch)) && isDigit(ch))
					{
						temp.value += ch;
					}
					HELP.flag = isSeparator(ch) || !flag;
					if (HELP.flag)
					{
						temp.category = 2;
						RESULT.push(temp);
						if (isSeparator(ch))processSeparator(ch);
					}
					else
					{
						DATA = string(1, ch) + DATA;
						HELP.error = "Invalid floating number!";
					}
				}
				else
				{
					DATA = string(1, ch) + DATA;
					HELP.flag = 0;
					HELP.error = "Invalid identifier!";
				}
			}
			else if (isZero(ch))
			{
				//��0����temp
				temp.value += ch;
				//�����һ���Ƿָ���
				if (isSeparator(ch) || !getNext(ch))
				{
					temp.category = 2;
					RESULT.push(temp);
					if (isSeparator(ch))processSeparator(ch);
					return;
				}
				//�����һ����.
				else if (isPoint(ch))
				{
					//��.����temp
					temp.value += ch;
					//�����0.
					if (isSeparator(ch) || !getNext(ch))
					{
						temp.category = 2;
						RESULT.push(temp);
						if (isSeparator(ch))processSeparator(ch);
						return;
					}
					//0.xxxx
					while (isDigit(ch))
					{
						temp.value += ch;
						getNext(ch);
					}
					//��Ϊ�����ָ����������봮û��������Ϊ����������erro
					HELP.flag = isSeparator(ch) || !getNext(ch);
					if (HELP.flag)
					{
						temp.category = 2;
						RESULT.push(temp);
						if (isSeparator(ch))processSeparator(ch);
					}
					else
					{
						HELP.error = "Invalid floating number!";
					}
					return;
				}
				else
				{
					HELP.error = "Invalid real number!";
				}
			}
			else if (isNotZero(ch))
			{
				temp.value += ch;
				while ((flag = getNext(ch)) && isDigit(ch))
				{
					temp.value += ch;
				}
				if (isPoint(ch))
				{
					temp.value += ch;
					while ((flag = getNext(ch)) && isDigit(ch))
					{
						temp.value += ch;
					}
					if (isSeparator(ch) || !flag)
					{
						temp.category = 2;
						RESULT.push(temp);
						if (isSeparator(ch))processSeparator(ch);
						return;
					}
					else
					{
						DATA = string(1, ch) + DATA;
						HELP.flag = 0;
						HELP.error = "Invalid floating number!";
					}
				}
				else if (isSeparator(ch) || !flag)
				{
					temp.category = 2;
					RESULT.push(temp);
					if (isSeparator(ch))processSeparator(ch);
					return;
				}
				else
				{
					HELP.flag = 0;
					HELP.error = "Invalid identifier!";
				}
			}
			else
			{
				DATA = string(1, ch) + DATA;
				HELP.flag = 0;
				HELP.error = "Invalid identifier!";
			}
		}
	}
	//˫����
	else if (isDoubleQuo(ch))
	{
		temp.value += ch;
		while (getNext(ch) && !isDoubleQuo(ch))
		{
			temp.value += ch;
		}
		HELP.flag = isDoubleQuo(ch);
		if (HELP.flag)
		{
			temp.value += ch;
			temp.category = 2;
			RESULT.push(temp);
			return;
		}
		else
		{
			DATA = string(1, ch) + DATA;
			HELP.error = "The string is missing '\"'";
			return;
		}
	}
	//������
	else if (isSingleQuo(ch))
	{
		temp.value += ch;
		if (!getNext(ch))
		{
			DATA = string(1, ch) + DATA;
			HELP.flag = 0;
			HELP.error = "Single quotes can't match!";
			return;
		}
		if (ch == 39)
		{
			temp.value += ch;
			temp.category = 2;
			RESULT.push(temp);
			return;
		}
		else
		{
			temp.value += ch;
			if (!getNext(ch))
			{
				DATA = string(1, ch) + DATA;
				HELP.flag = 0;
				HELP.error = "Single quotes can't match!";
				return;
			}
			if (ch == 39)
			{
				temp.value += ch;
				temp.category = 2;
				RESULT.push(temp);
				return;
			}
			else
			{
				DATA = string(1, ch) + DATA;
				HELP.flag = 0;
				HELP.error = "Single quotes can't match!";
				return;
			}
		}
	}
	//ע�ͷ���
	else if (isSlash(ch) && (DATA.size() && (isSlash(DATA[0]) || isSasterisk(DATA[0]))))
	{
		processAnnotation(ch);
		return;
	}
	//�ָ���
	else if (isSeparator(ch))
	{
		processSeparator(ch);
		return;
	}
	//�Ƿ��ַ�
	else
	{
		HELP.flag = 0;
		HELP.error = "Do not allow other symbols (except in the annotations) except letters, numbers, decimal points, English single and double quotation marks, basic operators, bounds, space, tabs, and newlines!";
	}
}
//�ʷ�����������
void LEXER::Lexer()
{
	do
	{
		lexerHelp();
	} while (HELP.flag && DATA.size() != 0);
}
//������дʷ�����֮��Ľ��
void LEXER::printToken()
{
	//int t = RESULT.size();
	//queue<Token> temp = RESULT;
	//for (int i = 0; i < t; i++)
	//{
	//	cout << temp.front().category << " " << temp.front().value << endl;
	//	temp.pop();
	//}
	if (!HELP.flag)
	{
		cout << "ERROR[" << HELP.row << "]��" << HELP.error << endl;
	}
	else if (HELP.parenthesis > 0)
	{
		cout << "ERROR[" << HELP.row << "]��" << "Missing')'!" << endl;
		HELP.flag = 0;
	}
	else if (HELP.parenthesis < 0)
	{
		cout << "ERROR[" << HELP.row << "]��" << HELP.error << endl;
		HELP.flag = 0;
	}
	else if (HELP.bracket > 0)
	{
		cout << "ERROR[" << HELP.row << "]��" << "Missing']'!" << endl;
		HELP.flag = 0;
	}
	else if (HELP.bracket < 0)
	{
		cout << "ERROR[" << HELP.row << "]��" << HELP.error << endl;
		HELP.flag = 0;
	}
	else if (HELP.brace > 0)
	{
		cout << "ERROR[" << HELP.row << "]��" << "Missing'}'!" << endl;
		HELP.flag = 0;
	}
	else if (HELP.brace < 0)
	{
		cout << "ERROR[" << HELP.row << "]��" << HELP.error << endl;
		HELP.flag = 0;
	}
	else if (HELP.flag)
	{
		cout << "Lexical analysis is over successfully!" << endl;
	}
	cout << "------------------------------------------------------------" << endl;
}


//��Ԫʽ
class QuaExp
{
public:
	//��Ԫʽ��һ��λ��
	string op;
	//�ڶ���λ��
	string s1;
	//������λ��
	string s2;
	//���ĸ�λ��
	string s3;
	//���ĸ�λ��
	int t3;
	QuaExp(string op, string s1, string s2, int t)
	{
		this->op = op;
		this->s1 = s1;
		this->s2 = s2;
		this->t3 = t;
		this->s3 = "";
	}
	QuaExp(string op, string s1, string s2, string s3)
	{
		this->op = op;
		this->s1 = s1;
		this->s2 = s2;
		this->s3 = s3;
	}
};
//����Ԫʽ����<<����
ostream& operator<<(ostream& output, QuaExp q)
{
	output << "(" << q.op << "," << q.s1 << "," << q.s2 << ",";
	if (q.s3 == "") output << q.t3;
	else output << q.s3;
	output << ")" << endl;
	return output;
}
//��ʶ��/��ʱ����
class Identifier
{
public:
	Identifier(string name, string kind, int position = 0) :name(name), kind(kind), position(position) {};
	//����
	string name;
	//����(int/float)
	string kind;
	//ֵ
	string value;
	//���ű��е�λ��
	int position;
};
//�鿴ĳ��ʶ���Ƿ��ڷ��ű��У��Ƿ���λ�ã����Ƿ���-1
int lookUp(string name, vector<Identifier>& SymTb)
{
	for (int i = 0; i < SymTb.size(); i++)
	{
		if (SymTb[i].name == name)return SymTb[i].position;
	}
	return -1;
}
//�鿴ĳ��ʱ�����Ƿ�����ʱ�������У��Ƿ�����ʱ�������룬���Ƿ���-1
int lookUpTemp(string name, vector<Identifier>& TempTb)
{
	if (name.size() <= 4)return -1;
	string code = name.substr(4, name.size());
	istringstream change(code);
	int c;
	change >> c;
	if (c >= 1 && c <= TempTb.size())return c;
	else return -1;

}
//intתstring
string Itoa(int input)
{
	char* ch = new char[1000];
	_itoa_s(input, ch, 1000, 10);
	string t = ch;
	return t;
}
//floatתstring
string Ftoa(float input)
{
	ostringstream buf;
	buf << input;
	return buf.str();
}
//�����ű���������ʶ��,���name��kind��һ����������ʧ�ܣ�����ɹ�
bool enter(string name, string kind, vector<Identifier>& SymTb)
{
	int ret = lookUp(name, SymTb);
	if (ret != -1)
	{
		if (SymTb[ret].kind == kind)return false;
	}
	Identifier temp(name, kind, SymTb.size());
	SymTb.push_back(temp);
	return true;
}
//�����ű���������ʱ����,������ʱ������name
string enterTemp(string kind, string value, vector<Identifier>& TempTb)
{
	string name = "temp" + Itoa(TempTb.size() + 1);
	Identifier temp(name, kind);
	temp.value = value;
	TempTb.push_back(temp);
	return name;
}
//�����
void backpatch(int head, int target, vector<QuaExp>& InterCode)
{
	//if (target >= InterCode.size())return;
	if (head == -2)return;
	int before = head;
	int cur = head;
	int t = InterCode[cur].t3;
	while (t != -1)
	{
		if (t >= InterCode.size())return;
		before = cur;
		cur = t;
		t = InterCode[cur].t3;
		InterCode[before].t3 = target;
	}
	InterCode[cur].t3 = target;
}
//��������
int merge(int p1, int p2, vector<QuaExp>& InterCode)
{
	if (p2 == -2)return p1;
	int temp = p2;
	while (InterCode[temp].t3 != -1)
	{
		temp = InterCode[temp].t3;
	}
	InterCode[temp].t3 = p1;
	return p2;
}
//����ջ�е�Ԫ������,����
class Symbol
{
public:
	//����ķ���(����ʽ�еķ��ţ�rop,id��i��)
	string it;
	//��ʵ���ţ�rop��Ӧ�ľ�����ţ�id��Ӧ�ľ���name
	string realValueStr;
	//int���͵���ֵ
	int realValueInt;
	//float���͵���ֵ
	float realValFloat;
	int codebegin;
	int chain;
	int True;
	int False;
	//���ͣ�int/float
	string Att;
	Symbol()
	{
		it = ' ';
		realValueInt = 0;
		realValueStr = "";
	}
	Symbol(string Sym, int val)
	{
		Att = "int";
		it = Sym;
		realValueInt = val;
		realValueStr = "";
	}
	Symbol(string Sym, float val)
	{
		Att = "float";
		it = Sym;
		realValFloat = val;
		realValueStr = "";
	}
	Symbol(string Sym, string val)
	{
		it = Sym;
		realValueInt = 0;
		realValueStr = val;
		realValFloat = 0;
	}
};
//������Ϣ
struct warning
{
	//�����������
	int row;
	//���������ԭ��
	string warn;
};
//�﷨�����������Ҫ�ĸ�������
struct ComHelp
{
	//��ǰ��
	int row;
	//����ԭ��
	string error;
	//��ǰ���״̬
	int flag;
	//�����ľ�����Ϣ
	queue<warning> warnings;
};

//����ʽ
class Production
{
public:
	Production()
	{
		Left = ' ';
		Right = " ";
	};
	Production(string l, string r)
	{
		Left = l;
		Right = r;
	}
	//��
	string Left;
	//�Ҳ�
	string Right;

	//�����Ҳ����ŵĶ��٣����ڼ����Լʱ�������ٷ��ţ�
	int rightSize()
	{
		if (Right == "true" || Right == "true" || Right == "id" || Right == "L1"
			|| Right == "STA" || Right == "while" || Right == "AS" || Right == "DS")
		{
			return 1;
		}
		else if (Right == "STAS" || Right == "DDS;"
			|| Right == "intid" || Right == "floatid"
			|| Right == "L1S")
		{
			return 2;
		}
		else if (Right == "idropid" || Right == "idropi" || Right == "iropid" || Right == "iropi"
			|| Right == "T||BE" || Right == "F&&T" || Right == "(BE)"
			|| Right == "DDS,id" || Right == "(AE)"
			|| Right == "B+AE" || Right == "B-AE")
		{
			return 3;
		}
		else if (Right == "id=AE;" || Right == "if(BE)" || Right == "ISelse" || Right == "W(BE)")
		{
			return 4;
		}
		else
		{
			return Right.size();
		}
	}
	//��Լ���﷨�Ƶ�����
	void action(Symbol& L, Symbol* arr, vector<QuaExp>& InterCode, vector<Identifier>& SymTb, vector<Identifier>& TempTb, ComHelp& Help)
	{
		if (Left == "BE" && Right == "T||BE")
		{
			L.codebegin = arr[2].codebegin;
			backpatch(arr[2].False, arr[0].codebegin, InterCode);
			L.True = merge(arr[2].True, arr[0].True, InterCode);
			L.False = arr[0].False;
			//cout << "T||E" << endl;
		}
		else if (Left == "BE" && Right == "T")
		{
			L.codebegin = arr[0].codebegin;
			L.True = arr[0].True;
			L.False = arr[0].False;
			//cout << "T" << endl;
		}
		else if (Left == "T" && Right == "F&&T")
		{
			L.codebegin = arr[2].codebegin;
			backpatch(arr[2].True, arr[0].True, InterCode);
			L.True = arr[0].True;
			L.False = merge(arr[2].False, arr[0].False, InterCode);
		}
		else if (Left == "T" && Right == "F")
		{
			L.codebegin = arr[0].codebegin;
			L.True = arr[0].True;
			L.False = arr[0].False;
			//cout << "F" << endl;
		}
		else if (Left == "F" && Right == "!G")
		{
			L.codebegin = arr[0].codebegin;
			L.True = arr[0].False;
			L.False = arr[0].True;
			//cout << "!G" << endl;
		}
		else if (Left == "F" && Right == "G")
		{
			L.codebegin = arr[0].codebegin;
			L.True = arr[0].True;
			L.False = arr[0].False;
			//cout << "G" << endl;
		}
		else if (Left == "G" && Right == "idropid")
		{
			int pos1 = lookUp(arr[2].realValueStr, SymTb);
			int pos2 = lookUp(arr[0].realValueStr, SymTb);
			if (pos1 == -1)
			{
				Help.flag = -1;
				Help.error = "The identifier '" + arr[2].realValueStr + "' is undefined!";
				return;
			}
			if (pos2 == -1)
			{
				Help.flag = -1;
				Help.error = "The identifier '" + arr[0].realValueStr + "' is undefined!";
				return;
			}
			L.codebegin = InterCode.size();
			L.True = InterCode.size();
			L.False = InterCode.size() + 1;
			int t1 = arr[2].realValueInt;
			int t2 = arr[0].realValueInt;
			QuaExp temp1("j" + arr[1].realValueStr, arr[2].realValueStr, arr[0].realValueStr, -1);
			InterCode.push_back(temp1);
			//cout << temp1;
			QuaExp temp2("jump", "-", "-", -1);
			InterCode.push_back(temp2);
			//cout << temp2;
			//cout << "idropid" << endl;
		}
		else if (Left == "G" && Right == "idropi")
		{
			int pos1 = lookUp(arr[2].realValueStr, SymTb);
			if (pos1 == -1)
			{
				Help.flag = -1;
				Help.error = "The identifier '" + arr[2].realValueStr + "' is undefined!";
				return;
			}
			L.codebegin = InterCode.size();
			L.True = InterCode.size();
			L.False = InterCode.size() + 1;
			string i;
			if (arr[0].Att == "int")i = Itoa(arr[0].realValueInt);
			else i = Ftoa(arr[0].realValFloat);
			QuaExp temp1("j" + arr[1].realValueStr, arr[2].realValueStr, i, -1);
			InterCode.push_back(temp1);
			//cout << temp1;
			QuaExp temp2("jump", "-", "-", -1);
			InterCode.push_back(temp2);
			//cout << temp2;
			//cout << "idropid" << endl;
		}
		else if (Left == "G" && Right == "iropid")
		{
			int pos2 = lookUp(arr[0].realValueStr, SymTb);
			if (pos2 == -1)
			{
				Help.flag = -1;
				Help.error = "The identifier '" + arr[0].realValueStr + "' is undefined!";
				return;
			}
			L.codebegin = InterCode.size();
			L.True = InterCode.size();
			L.False = InterCode.size() + 1;
			string i;
			if (arr[2].Att == "int")i = Itoa(arr[2].realValueInt);
			else i = Ftoa(arr[2].realValFloat);
			QuaExp temp1("j" + arr[1].realValueStr, i, arr[0].realValueStr, -1);
			InterCode.push_back(temp1);
			//cout << temp1;
			QuaExp temp2("jump", "-", "-", -1);
			InterCode.push_back(temp2);
			//cout << temp2;
			//cout << "idropid" << endl;
		}
		else if (Left == "G" && Right == "iropi")
		{
			L.codebegin = InterCode.size();
			L.True = InterCode.size();
			L.False = InterCode.size() + 1;
			string i1;
			if (arr[2].Att == "int")i1 = Itoa(arr[2].realValueInt);
			else i1 = Ftoa(arr[2].realValFloat);
			string i2;
			if (arr[0].Att == "int")i2 = Itoa(arr[0].realValueInt);
			else i2 = Ftoa(arr[0].realValFloat);
			QuaExp temp1("j" + arr[1].realValueStr, i1, i2, -1);
			InterCode.push_back(temp1);
			//cout << temp1;
			QuaExp temp2("jump", "-", "-", -1);
			InterCode.push_back(temp2);
			//cout << temp2;
			//cout << "idropid" << endl;
		}
		else if (Left == "G" && Right == "(BE)")
		{
			L.codebegin = arr[1].codebegin;
			L.True = arr[1].True;
			L.False = arr[1].False;
			//cout << "(E)" << endl;
		}
		else if (Left == "G" && Right == "true")
		{
			L.codebegin = InterCode.size();
			L.True = InterCode.size();
			L.False = 0;
			QuaExp temp("jump", "-", "-", -1);
			InterCode.push_back(temp);
			//cout << temp;
			//cout << "true" << endl;
		}
		else if (Left == "G" && Right == "false")
		{
			L.codebegin = InterCode.size();
			L.False = InterCode.size();
			L.True = 0;
			QuaExp temp("jump", "-", "-", -1);
			InterCode.push_back(temp);
			//cout << temp;
			//cout << "false" << endl;
		}
		else if (Left == "I" && Right == "if(BE)")
		{
			backpatch(arr[1].True, InterCode.size(), InterCode);
			L.chain = arr[1].False;
		}
		else if (Left == "S" && Right == "IS")
		{
			L.chain = merge(arr[1].chain, arr[0].chain, InterCode);
			backpatch(arr[1].chain, InterCode.size(), InterCode);
		}
		else if (Left == "M" && Right == "ISelse")
		{
			int q = InterCode.size();
			QuaExp temp("jump", "-", "-", -1);
			InterCode.push_back(temp);
			backpatch(arr[2].chain, InterCode.size(), InterCode);
			L.chain = merge(q, arr[1].chain, InterCode);
		}
		else if (Left == "S" && Right == "MS")
		{
			L.chain = merge(arr[1].chain, arr[0].chain, InterCode);
			backpatch(arr[1].chain, InterCode.size(), InterCode);
		}
		else if (Left == "W" && Right == "while")
		{
			L.codebegin = InterCode.size();
		}
		else if (Left == "N" && Right == "W(BE)")
		{
			L.codebegin = arr[3].codebegin;
			backpatch(arr[1].True, InterCode.size(), InterCode);
			L.chain = arr[1].False;
		}
		else if (Left == "S" && Right == "NS")
		{
			backpatch(arr[0].chain, arr[1].codebegin, InterCode);
			QuaExp temp("jump", "-", "-", arr[1].codebegin);
			InterCode.push_back(temp);
			arr[0].chain = arr[1].chain;
			backpatch(arr[1].chain, InterCode.size(), InterCode);
		}
		else if (Left == "S" && Right == "{L}")
		{
			L.chain = arr[1].chain;
		}
		else if (Left == "S" && Right == "AS")
		{
			L.chain = -2;
		}
		else if (Left == "L" && Right == "S")
		{
			L.chain = arr[0].chain;
		}
		else if (Left == "L1" && Right == "L")
		{
			backpatch(arr[0].chain, InterCode.size(), InterCode);
		}
		else if (Left == "L" && Right == "L1S")
		{
			L.chain = arr[0].chain;
		}
		else if (Left == "DDS" && Right == "intid")
		{
			enter(arr[0].realValueStr, "int", SymTb);
			L.Att = "int";

		}
		else if (Left == "DDS" && Right == "floatid")
		{
			enter(arr[0].realValueStr, "float", SymTb);
			L.Att = "float";
		}
		else if (Left == "DDS" && Right == "DDS,id")
		{
			enter(arr[0].realValueStr, arr[2].Att, SymTb);
			L.Att = arr[2].Att;
		}
		else if (Left == "AS" && Right == "id=AE;")
		{
			//��id�Ƿ����
			int pos = lookUp(arr[3].realValueStr, SymTb);
			//cout << arr[3].realValueStr <<"  " <<arr[1].realValueStr<<endl;
			//id����
			if (pos != -1)
			{
				//���AE�Ƿ��Ǳ�ʶ��
				int AEPlace = lookUp(arr[1].realValueStr, SymTb);
				//���Ǳ�ʶ��
				if (AEPlace == -1)
				{
					//���AE�Ƿ�����ʱ����
					AEPlace = lookUpTemp(arr[1].realValueStr, TempTb);
					//����ʱ����
					if (AEPlace != -1)
					{
						//cout << SymTb[pos].kind << "****"<<arr[1].Att << endl;
						if (SymTb[pos].kind != TempTb[AEPlace - 1].kind)
						{
							warning temp = { Help.row - 1,"The left and right sides of the assignment are of different types!" };
							Help.warnings.push(temp);
							if (SymTb[pos].kind == "float")
							{
								istringstream change(TempTb[AEPlace - 1].value);
								float t;
								change >> t;
								arr[3].realValFloat = t;
								SymTb[pos].value = Ftoa(t);
							}
							else
							{
								istringstream change(TempTb[AEPlace - 1].value);
								int t;
								change >> t;
								arr[3].realValueInt = t;
								SymTb[pos].value = Itoa(t);
							}
							QuaExp temp1("=", TempTb[AEPlace - 1].name, "-", pos);
							InterCode.push_back(temp1);
						}
						else
						{
							SymTb[pos].value = TempTb[AEPlace - 1].value;
							QuaExp temp("=", TempTb[AEPlace - 1].name, "-", pos);
							InterCode.push_back(temp);
						}

					}
					else
					{
						Help.flag = -1;
						Help.row--;
						Help.error = "The identifier '" + arr[1].realValueStr + "' is undefined!";
					}

				}
				//�Ǳ�ʶ��
				else
				{
					if (SymTb[pos].kind != SymTb[AEPlace].kind)
					{
						warning temp = { Help.row - 1,"The left and right sides of the assignment are of different types!" };
						Help.warnings.push(temp);
						if (SymTb[pos].kind == "float")
						{
							istringstream change(SymTb[AEPlace].value);
							float t;
							change >> t;
							arr[3].realValFloat = t;
							SymTb[pos].value = Ftoa(t);
						}
						else
						{
							istringstream change(SymTb[AEPlace].value);
							int t;
							change >> t;
							arr[3].realValueInt = t;
							SymTb[pos].value = Itoa(t);
						}
						QuaExp temp1("=", Itoa(AEPlace), "-", pos);
						InterCode.push_back(temp1);
					}
					else
					{
						SymTb[pos].value = SymTb[AEPlace].value;
						QuaExp temp("=", Itoa(AEPlace), "-", pos);
						InterCode.push_back(temp);
					}

				}

			}
			//id������
			else
			{
				/*	for (int i = 0; i < SymTb.size(); i++)
						cout << SymTb[i].name << " ";
					cout << endl;*/
				Help.row--;
				Help.flag = -1;
				Help.error = "The identifier '" + arr[3].realValueStr + "' is undefined!";
			}
		}
		else if (Left == "C" && Right == "id")
		{
			int pos = lookUp(arr[0].realValueStr, SymTb);
			if (pos == -1)
			{
				Help.flag = -1;
				Help.error = "The identifier '" + arr[0].realValueStr + "' is undefined!";
				return;
			}
			arr[0].realValueStr = SymTb[pos].name;
			arr[0].Att = SymTb[pos].kind;
			if (SymTb[pos].kind == "int")
			{
				istringstream change(SymTb[pos].value);
				int c;
				change >> c;
				arr[0].realValueInt = c;
			}
			else
			{
				istringstream change(SymTb[pos].value);
				float c;
				change >> c;
				arr[0].realValFloat = c;
			}
			L = arr[0];
		}
		else if (Left == "C" && Right == "i")
		{
			L = arr[0];
			string value;
			if (arr[0].Att == "int")value = Itoa(arr[0].realValueInt);
			else value = Ftoa(arr[0].realValFloat);
			L.realValueStr = enterTemp(arr[0].Att, value, TempTb);

		}
		else if (Left == "B" && Right == "C")
		{
			L = arr[0];
		}
		else if (Left == "AE" && Right == "B")
		{
			L = arr[0];
		}
		else if (Left == "C" && Right == "(AE)")
		{
			L = arr[1];
		}
		else if (Left == "B" && Right == "C*B")
		{
			//���Ͳ�һ��
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '*' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B��int����C��float
				if (arr[0].Att == "int")
				{
					L.realValFloat = float(arr[0].realValueInt * arr[2].realValFloat);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
				else
				{
					L.realValFloat = float(arr[0].realValFloat * arr[2].realValueInt);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}

			}
			//����һ��
			else
			{
				//����int
				if (arr[0].Att == "int")
				{
					L.Att = "int";
					L.it = "id";
					L.realValueInt = arr[0].realValueInt * arr[2].realValueInt;
					L.realValueStr = enterTemp("int", Itoa(L.realValueInt), TempTb);
				}
				else
				{
					L.Att = "float";
					L.it = "id";
					L.realValFloat = arr[0].realValFloat * arr[2].realValFloat;
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
			}
			int pos1 = lookUp(arr[2].realValueStr, SymTb);
			int pos2 = lookUp(arr[0].realValueStr, SymTb);
			QuaExp temp("*", arr[2].realValueStr, arr[0].realValueStr, L.realValueStr);
			InterCode.push_back(temp);
		}
		else if (Left == "B" && Right == "C/B")
		{
			if ((arr[0].Att == "int" && arr[0].realValueInt == 0) || (arr[2].Att == "float" && arr[2].realValFloat == float(0)))
			{
				Help.flag = -1;
				Help.error = "The divisor cannot be '0' in a division operation!";
				return;
			}
			//���Ͳ�һ��
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '/' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B��int����C��float
				if (arr[0].Att == "int")
				{
					L.realValFloat = float(arr[0].realValueInt * arr[2].realValFloat);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
				else
				{
					L.realValFloat = float(arr[0].realValFloat * arr[2].realValueInt);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}

			}
			//����һ��
			else
			{
				//����int
				if (arr[0].Att == "int")
				{
					L.Att = "int";
					L.it = "id";
					L.realValueInt = arr[0].realValueInt * arr[2].realValueInt;
					L.realValueStr = enterTemp("int", Itoa(L.realValueInt), TempTb);
				}
				else
				{
					L.Att = "float";
					L.it = "id";
					L.realValFloat = arr[0].realValFloat * arr[2].realValFloat;
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
			}
			QuaExp temp("/", arr[2].realValueStr, arr[0].realValueStr, L.realValueStr);
			InterCode.push_back(temp);
		}
		else if (Left == "B" && Right == "C%B")
		{
			if (arr[0].Att == "float" || arr[2].Att == "float")
			{
				Help.flag = -1;
				Help.error = "Operands must be integers in mod operations!";
				return;
			}
			else
			{
				if (arr[0].realValueInt == 0)
				{
					Help.flag = -1;
					Help.error = "You can't take the modulus of '0'!";
				}
				else
				{
					L.Att = "int";
					L.it = "id";
					L.realValueInt = arr[2].realValueInt % arr[0].realValueInt;
					L.realValueStr = enterTemp("int", Itoa(L.realValueInt), TempTb);
				}
			}
			QuaExp temp("%", arr[2].realValueStr, arr[0].realValueStr, L.realValueStr);
			InterCode.push_back(temp);
		}
		else if (Left == "AE" && Right == "B+AE")
		{
			//���Ͳ�һ��
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '+' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B��int����C��float
				if (arr[0].Att == "int")
				{
					L.realValFloat = float(arr[0].realValueInt + arr[2].realValFloat);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
				else
				{
					L.realValFloat = float(arr[0].realValFloat + arr[2].realValueInt);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}

			}
			//����һ��
			else
			{
				//����int
				if (arr[0].Att == "int")
				{
					L.Att = "int";
					L.it = "id";
					L.realValueInt = arr[0].realValueInt + arr[2].realValueInt;
					L.realValueStr = enterTemp("int", Itoa(L.realValueInt), TempTb);
				}
				else
				{
					L.Att = "float";
					L.it = "id";
					L.realValFloat = arr[0].realValFloat + arr[2].realValFloat;
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
			}
			QuaExp temp("+", arr[2].realValueStr, arr[0].realValueStr, L.realValueStr);
			InterCode.push_back(temp);
		}
		else if (Left == "AE" && Right == "B-AE")
		{
			//���Ͳ�һ��
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '-' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B��int����C��float
				if (arr[0].Att == "int")
				{
					L.realValFloat = float(arr[0].realValueInt - arr[2].realValFloat);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
				else
				{
					L.realValFloat = float(arr[0].realValFloat - arr[2].realValueInt);
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}

			}
			//����һ��
			else
			{
				//����int
				if (arr[0].Att == "int")
				{
					L.Att = "int";
					L.it = "id";
					L.realValueInt = arr[0].realValueInt - arr[2].realValueInt;
					L.realValueStr = enterTemp("int", Itoa(L.realValueInt), TempTb);
				}
				else
				{
					L.Att = "float";
					L.it = "id";
					L.realValFloat = arr[0].realValFloat - arr[2].realValFloat;
					L.realValueStr = enterTemp("float", Ftoa(L.realValFloat), TempTb);
				}
			}
			QuaExp temp("-", arr[2].realValueStr, arr[0].realValueStr, L.realValueStr);
			InterCode.push_back(temp);
		}
		else if (Left == "BG" && Right == "STA")
		{
			L = arr[0];
		}
		else if (Left == "STA" && Right == "S")
		{
			L = arr[0];
		}
		else if (Left == "STA" && Right == "STAS")
		{

		}
		else if (Left == "DS" && Right == "DDS;")
		{
			L = arr[1];
		}
		else if (Left == "S" && Right == "DS")
		{
			L = arr[0];
			L.chain = -2;
		}
	}
};
//ActionOrGoto
class ActGo
{
public:
	//�������룬��gotoֵΪ-1�����޹�Լ�������ʽ�󲿺��Ҳ�����@
	ActGo(int g = -1, string l = "@", string r = "@")
	{
		goTo = g;
		pro.Left = l;
		pro.Right = r;

	}
	//Ҫת���״̬
	int goTo;
	//Ҫ��Լ��ʽ��
	Production pro;
};
//״̬
class MyStatus
{
public:
	//�洢һ��״̬��������ͬ�ķ��Ž��еĲ�ͬ��ActGo
	//ÿ��Ԫ�ش洢��ʽΪ�������ţ���Ӧ������
	map<string, ActGo> status;
	//�洢��״̬��������������Ч�ַ�
	vector<string> charSet;
	//��ʼ��charSet�����Զ���tia
	void init()
	{
		for (map<string, ActGo>::iterator it = status.begin(); it != status.end(); it++)
		{
			charSet.push_back(it->first);
		}
	}
};
//�и��ַ���������������ļ��е�����
void cut(string data, string& ch, int& got, string& l, string& r)
{
	string* arr = new string[data.size()];
	char split = ' ';
	if (data == "")  return;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	string strs = data + split;
	size_t pos = strs.find(split);
	int i = 0;
	// ���Ҳ����������ַ��������������� npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		arr[i] = temp;
		i++;
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
	}
	ch = arr[0];
	got = atoi(arr[1].c_str());
	l = arr[2];
	r = arr[3];
}
//�������������ӳ�䵽vector<MyStatus>��
vector<MyStatus> initiate(int* arr, int n, string filename)
{
	//���صı���
	vector<MyStatus> temp;
	fstream FI(filename);
	//��һ��ѭ���õ�ÿһ��״̬
	for (int i = 0; i < n; i++)
	{
		MyStatus s1;
		//�ڶ���ѭ���õ�ÿ��״̬��������ԣ���Ч���ţ���Ӧ�Ĳ�����ActionOrGoto����
		for (int j = 0; j < arr[i]; j++)
		{
			string data;
			getline(FI, data);
			//��Ч����
			string ch;
			//gotoֵ
			int got;
			//��Լʽ����
			string l;
			//��Լʽ���Ҳ�
			string r;
			//���ú����õ����ϵ�ֵ
			cut(data, ch, got, l, r);
			//cout << ch << " " << got << " " << l << " " << r << endl;
			ActGo a1(got, l, r);
			pair<string, ActGo> p1(ch, a1);
			s1.status.insert(p1);
		}
		//�õ�һ��״̬�е��������ݺ�Ҫ��ʼ����״̬�е�charSet����
		s1.init();
		temp.push_back(s1);
	}
	return temp;
}
//�ж��Ƿ�Ϊʵ��
bool isDigit(Token myData)
{
	string data = myData.value;
	int i = 0;
	if (data[0] == '-') i = 1;
	int flag = 1;
	for (; i < data.size(); i++)
	{
		if ((data[i] < 48) || data[i] > 57)
		{
			if (data[i] == '.') continue;
			flag = 0;
			break;
		}
	}
	return flag;
}
//�ж��Ƿ�Ϊ��ϵ����
bool isRelaSym(Token myData)
{
	string data = myData.value;
	if (data == "<" || data == ">" || data == "<=" || data == ">=" || data == "==" || data == "!=") return true;
	return false;
}
//�ж��Ƿ���float
bool isFloat(string In)
{
	for (int i = 0; i < In.size(); i++)
	{
		if (In[i] == '.')return true;
	}
	return false;
}
//�ж��Ƿ��Ǳ�ʶ����ؼ���
bool isID(Token data)
{
	return data.category == 1;
}
class Compiler
{
public:
	Compiler(string filename)
	{
		LEXER temp(filename);
		myLexer = temp;
		cout << "Lexical analysis starts!\n" << endl;
		myLexer.Lexer();
		myLexer.printToken();
		if (myLexer.HELP.flag == 1)
		{
			int arr[79] = { 15,15,7,9,14,1,14,14,9,2,1,1,1,16,1,1,7,9,10,9,9,1,9,6,10,7,7,14,2,2,6,1,3,3,2,6,3,3,1,10,1,2,1,4,7,7,6,7,1,9,7,6,9,10,3,2,1,2,9,6,6,5,5,5,1,6,2,1,3,3,3,3,3,2,2,4,4,4,7 };
			AnaSta = initiate(arr, 79, "anaTotal.txt");
			StatusStack.push(0);
			Input = myLexer.RESULT;
			Input.push(Token(7, "$"));
		}
	}
	//�ʷ�������
	LEXER myLexer;
	//״̬ջ
	stack<int> StatusStack;
	//����ջ
	stack<Symbol> SymbolStack;
	//���봮
	queue<Token> Input;
	//�м����
	vector< QuaExp> InterCode;
	//ActionGoto��
	vector<MyStatus> AnaSta;
	//�﷨���������������������
	ComHelp Help = { 1,"",0 };
	//���ű�
	vector<Identifier> SymTb;
	//��ʱ������
	vector<Identifier> TempTb;

	//�﷨���������������������
	void syAndSeAnaHelp();
	//�﷨�������������
	void syAndSeAna();
	//������
	void printResult();

};
void Compiler::syAndSeAnaHelp()
{
	//��ǰ״̬
	int curSta = StatusStack.top();
	//��ǰ�������
	string curSym;
	while (Input.front().value == "\n")
	{
		Input.pop();
		Help.row++;
	}
	//���ֺ��������ŷֿ�����ʹ�õ�ǰ�ַ�����Ϊchar
	if (isDigit(Input.front()))
	{
		curSym = "i";
	}
	else if (isID(Input.front()))
	{
		curSym = "id";
	}
	else if (isRelaSym(Input.front()))
	{
		curSym = "rop";
	}
	else curSym = Input.front().value;
	for (int i = 0; i < AnaSta[curSta].charSet.size(); i++)
	{
		//�Ȳ��ҵ�ǰ��������Ƿ��ڵ�ǰ״̬��charSet�У����ڵĻ���ֱ��ƥ��ʧ��
		if (AnaSta[curSta].charSet[i] == curSym)
		{
			//����״̬��Ѱ��������ǰ���Ŷ�Ӧ�Ĳ�����ActionOrGoto��
			for (map<string, ActGo>::iterator it = AnaSta[curSta].status.begin(); it != AnaSta[curSta].status.end(); it++)
			{
				if (it->first == curSym)
				{
					//��Ӧ�Ĳ���Ϊacc��ƥ��ɹ�
					if (it->second.pro.Right == "acc")
					{
						//cout << "���Ϊ��" << symbol.top().value << endl;
						Help.flag = 1;
						return;
					}
					//��Ӧ�Ĳ���Ϊ�ƽ�
					else if (it->second.goTo != -1)
					{
						StatusStack.push(it->second.goTo);
						if (curSym == "i")
						{
							int i;
							float f;
							istringstream change(Input.front().value);
							if (isFloat(Input.front().value))
							{
								change >> f;
								SymbolStack.push(Symbol("i", f));
							}
							else
							{
								change >> i;
								SymbolStack.push(Symbol("i", i));
							}
							//semanteme.push(atoi(input.front().c_str()));
						}
						else if (curSym == "rop")
						{
							SymbolStack.push(Symbol("rop", Input.front().value));
						}
						else if (curSym == "id")
						{
							SymbolStack.push(Symbol("id", Input.front().value));
						}
						else
						{
							SymbolStack.push(Symbol(curSym, 0));
						}
						Input.pop();
						return;
					}
					//��Ӧ�Ĳ���Ϊ��Լ
					else
					{
						//��¼Ҫ��״̬ջ�ͷ���ջ��������״̬�ͷ���
						int count = it->second.pro.rightSize();
						//��¼����ջ����������
						Symbol* arr = new Symbol[count];
						for (int k = 0; k < count; k++)
						{
							StatusStack.pop();
							arr[k] = SymbolStack.top();
							SymbolStack.pop();
						}

						//���µ�ǰ״̬�ͷ���
						curSta = StatusStack.top();
						SymbolStack.push(Symbol(it->second.pro.Left, 0));//����ջҪ���ƽ���Լʽ����
						curSym = SymbolStack.top().it;

						//���ƥ�䵱ǰ���ţ���Լʽ���󲿣������ƥ�䣬����ƥ��ʧ��
						for (map<string, ActGo>::iterator it1 = AnaSta[curSta].status.begin(); it1 != AnaSta[curSta].status.end(); it1++)
						{
							if (it1->first == curSym)
							{
								//���ս����Ӧ��һ�����������������ƥ��ʧ��
								if (it1->second.goTo == -1)
								{
									Help.flag = -1;
									Help.error = "Unable to match during parsing!";
									return;
								}
								it->second.pro.action(SymbolStack.top(), arr, InterCode, SymTb, TempTb, Help);//ע���Ƕ�it����������it1
								StatusStack.push(it1->second.goTo);
								return;
							}

						}
						Help.flag = -1;
						Help.error = "Unable to match during parsing!";
						return;
					}
				}
			}
			Help.flag = -1;
			Help.error = "Unable to match during parsing!";
			return;
		}
	}
	Help.flag = -1;
	Help.error = "Unable to match during parsing!";
	return;
}
void Compiler::syAndSeAna()
{
	cout << "Syntax and semantic analysis start!\n" << endl;
	//����0��ʾƥ�仹�����������1��ʾƥ��ɹ�������-1��ʾƥ��ʧ��,����-2��ʾ��ĸΪ0
	while (Help.flag == 0)
	{
		syAndSeAnaHelp();
	}
}
void Compiler::printResult()
{
	if (Help.flag == -1)
	{
		int total = Help.warnings.size();
		if (total != 0)
		{
			cout << "The result is as follows:\n\n";
			cout << " " << total << " warning(s) is(are) found:\n";
			for (int j = 0; j < total; j++)
			{
				cout << "WARNING[" << Help.warnings.front().row << "]:" << Help.warnings.front().warn << endl;
				Help.warnings.pop();
			}
			cout << endl;
		}
		cout << "ERROR:\n";
		cout << "ERROR[" << Help.row << "]:" << Help.error << endl;
	}
	else if (Help.flag == 1)
	{
		cout << "Syntax and semantic analysis are over successfully!" << endl;
		cout << "------------------------------------------------------------" << endl;
		int total = Help.warnings.size();
		if (total != 0)
		{
			cout << "The result is as follows:\n\n";
			cout << " " << total << " warning(s) is(are) found:\n";
			for (int j = 0; j < total; j++)
			{
				cout << "WARNING[" << Help.warnings.front().row << "]:" << Help.warnings.front().warn << endl;
				Help.warnings.pop();
			}
		}
		cout << "\nThe intermediate code is as follows:\n";
		for (int i = 0; i < InterCode.size(); i++)
		{
			cout << i << " " << InterCode[i];
		}

		cout << "\nThe symbol table is as follows:" << endl;
		for (int i = 0; i < SymTb.size(); i++)
		{
			cout << SymTb[i].position << " " << SymTb[i].name << " " << SymTb[i].kind << " " << SymTb[i].value << endl;
		}

		cout << "\nThe list of temporary variables is as follows:" << endl;
		for (int i = 0; i < TempTb.size(); i++)
		{
			cout << TempTb[i].name << " " << TempTb[i].kind << " " << TempTb[i].value << endl;
		}

	}
	else if (Help.flag != 1)
	{
		cout << "Unknown error!" << endl;
	}
	cout << "------------------------------------------------------------" << endl;
}
int main()
{
	
	Compiler myCompiler("./test2.txt");

	myCompiler.syAndSeAna();
	myCompiler.printResult();
}