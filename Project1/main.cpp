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
//"\n"在processSeparator中处理，种别码是6
class Token
{
public:
	Token() { category = 0; value = ""; };
	Token(int cat, string val) :category(cat), value(val) {};
	//种别码
	int category;
	//值
	string value;
};
//辅助类型
struct Help
{
	//当前第几行
	int row;
	//1表示当前匹配成功，0表示失败
	bool flag;
	//记录报错原因
	string error;
	/*
	读入左括号就+1，读入右括号-1
	任何时候如果parenthesis<0,则报错
	如果词法分析器分析结束parenthesis!=0,则报错
	*/
	//小括号配对情况
	int parenthesis;
	//中括号配对情况
	int bracket;
	//大括号配对情况
	int brace;
};
class LEXER
{
public:
	//DATA存储文件内容
	string DATA;
	//Token结果
	queue<Token> RESULT;
	//关键字和保留字
	string* KEY_WORDS;
	int totalKeyNum;
	//辅助变量
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
		//拿到源代码文件内容
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
	//获得下一个字符
	bool getNext(char& ch);
	//获得下一个非空白符、制表符的字符
	bool getChar(char& ch);
	//判断是否是英文字母
	bool isLetter(char ch);
	//判断是否是关键字或保留字
	bool isKeyWord(string str);
	//判断是否是0
	bool isZero(char ch);
	//判断是否是非0数字
	bool isNotZero(char ch);
	//判断是否是数字
	bool isDigit(char ch);
	//判断是否是小数点
	bool isPoint(char ch);
	//判断是否是+-号
	bool isSign(char ch);
	//判断是否是分隔符
	bool isSeparator(char ch);
	//将一次判断中途遇到的非空格和非制表符的分隔符放入SESULT
	void processSeparator(char ch);
	//判断是否是单引号
	bool isSingleQuo(char ch);
	//判断是否是双引号
	bool isDoubleQuo(char ch);
	//判断是否是/
	bool isSlash(char ch);
	//判断是否是*
	bool isSasterisk(char ch);
	//判断是否是/或*
	bool isAnnotation(char ch);
	//处理注释符号
	void processAnnotation(char ch);
	//词法分析主函数的辅助函数
	void lexerHelp();
	//词法分析主函数
	void Lexer();
	//输出进行词法分析之后的结果
	void printToken();
};
//获得下一个字符
bool LEXER::getNext(char& ch)
{
	if (DATA.size() == 0)return false;
	ch = DATA[0];
	DATA = DATA.substr(1, DATA.size());
	return true;
}
//获得下一个非空白符、制表符的字符
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
//将一次判断中途遇到的非空格和非制表符的分隔符放入SESULT
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
//获得下一个非空白符、制表符的字符
bool LEXER::getChar(char& ch)
{
	char temp;
	do
	{
		//如果输入串没内容时，返回false
		if (!DATA.size())return false;
		getNext(temp);
	} while (temp == ' ' || temp == '\t');
	ch = temp;
	return true;
}
//判断是否是英文字母
bool LEXER::isLetter(char ch)
{
	return ch >= 65 && ch <= 90 || ch >= 97 && ch <= 122;
}
//判断是否是关键字或保留字
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
//判断是否是0
bool LEXER::isZero(char ch)
{
	return ch == 48;
}
//判断是否是非0数字
bool LEXER::isNotZero(char ch)
{
	return ch >= 49 && ch <= 57;
}
//判断是否是数字
bool LEXER::isDigit(char ch)
{
	return ch >= 48 && ch <= 57;
}
//判断是否是小数点
bool LEXER::isPoint(char ch)
{
	return ch == '.';
}
//判断是否是+-号
bool LEXER::isSign(char ch)
{
	return ch == '+' || ch == '-';
}
//判断是否是单引号
bool LEXER::isSingleQuo(char ch)
{
	return ch == 39;
}
//判断是否是双引号
bool LEXER::isDoubleQuo(char ch)
{
	return ch == 34;
}
//判断是否是/
bool LEXER::isSlash(char ch)
{
	return ch == '/';
}
//判断是否是*
bool LEXER::isSasterisk(char ch)
{
	return ch == '*';
}
//判断是否是/或*
bool LEXER::isAnnotation(char ch)
{
	return isSlash(ch) || isSasterisk(ch);
}
//处理注释符号
void LEXER::processAnnotation(char ch)
{
	//是“/”
	if (isSlash(ch))
	{
		//是“/*”
		if (isSasterisk(DATA[0]))
		{
			getNext(ch);
			do {
				if (!getChar(ch))
				{
					HELP.flag = 0;
					HELP.error = "Missing the right annotation“*/”";
					return;
				}
				if (ch == '\n')processSeparator(ch);
			} while (!(isSasterisk(ch) && DATA.size() && isSlash(DATA[0])));
			getNext(ch);
			return;
		}
		//是“//”
		else if (isSlash(DATA[0]))
		{
			getNext(ch);
			do {
				if (!getChar(ch))return;

			} while (ch != '\n');
			processSeparator(ch);
		}
		//是“/xxx”
		else
		{
			HELP.flag = 0;
			HELP.error = "非法标识符！";
		}
	}
	else
	{
		HELP.flag = 0;
		HELP.error = "非法标识符！";
	}

}
//词法分析主函数的辅助函数
void LEXER::lexerHelp()
{
	bool flag = 0;
	Token temp;
	char ch;
	if (!getChar(ch))return;
	//处理一开始读入的是换行的情况
	while (ch == '\n')
	{
		HELP.row++;
		RESULT.push(Token(6, "\n"));
		if (!DATA.size())return;
		getNext(ch);
	}
	//字母
	if (isLetter(ch) || ch == '_')
	{
		temp.value += ch;
		//判断是否为字母，数字或者下划线
		while ((flag = getNext(ch)) && (isDigit(ch) || isLetter(ch) || ch == '_'))
		{
			temp.value += ch;
		}
		//因为遇到分隔符或者输入串没有输入了为正常，否则erro
		HELP.flag = isSeparator(ch) || !flag;
		if (HELP.flag)
		{
			//判断是否为关键字
			if (isKeyWord(temp.value))temp.category = 3;
			else if (temp.value == "FALSE" || temp.value == "TRUE") temp.category = 2;
			else temp.category = 1;
			RESULT.push(temp);
			//处理分隔符
			if (isSeparator(ch))processSeparator(ch);
		}
		else
		{
			//回填不匹配的ch
			DATA = string(1, ch) + DATA;
			HELP.flag = 0;
			HELP.error = "Invalid identifier!";
		}
		return;
	}
	//0
	else if (isZero(ch))
	{
		//将0并入temp
		temp.value += ch;
		//如果下一个是分隔符
		if (!getNext(ch) || isSeparator(ch))
		{
			temp.category = 2;
			RESULT.push(temp);
			if (isSeparator(ch))processSeparator(ch);
			return;
		}
		//如果下一个是.
		else if (isPoint(ch))
		{
			//将.并入temp
			temp.value += ch;
			//如果是0.
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
			//因为遇到分隔符或者输入串没有输入了为正常，否则erro
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
	//整数
	else if (isNotZero(ch))
	{
		temp.value += ch;
		while ((flag = getNext(ch)) && isDigit(ch))
		{
			temp.value += ch;
		}
		//整数.
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
		//整数xxx(x为除了.外的其他符号)
		else
		{
			HELP.flag = 0;
			HELP.error = "Invalid identifier!";
		}
	}
	//.
	else if (isPoint(ch))
	{
		//将.并入temp
		temp.value += ch;
		//如果是.
		if (!getNext(ch))
		{
			DATA = string(1, ch) + DATA;
			HELP.flag = 0;
			HELP.error = "The decimal point cannot stand alone!";
			return;

		}
		//如果是.xxx
		else if (isDigit(ch))
		{
			temp.value += ch;
			//后面用来判断输出什么样的错误
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
	//+/-号
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
				//将.并入temp
				temp.value += ch;
				//如果是+.
				if (!getNext(ch))
				{
					DATA = string(1, ch) + DATA;
					HELP.flag = 0;
					HELP.error = "Invalid identifier!";
					return;

				}
				//如果是+.xxx
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
				//将0并入temp
				temp.value += ch;
				//如果下一个是分隔符
				if (isSeparator(ch) || !getNext(ch))
				{
					temp.category = 2;
					RESULT.push(temp);
					if (isSeparator(ch))processSeparator(ch);
					return;
				}
				//如果下一个是.
				else if (isPoint(ch))
				{
					//将.并入temp
					temp.value += ch;
					//如果是0.
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
					//因为遇到分隔符或者输入串没有输入了为正常，否则erro
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
	//双引号
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
	//单引号
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
	//注释符号
	else if (isSlash(ch) && (DATA.size() && (isSlash(DATA[0]) || isSasterisk(DATA[0]))))
	{
		processAnnotation(ch);
		return;
	}
	//分隔符
	else if (isSeparator(ch))
	{
		processSeparator(ch);
		return;
	}
	//非法字符
	else
	{
		HELP.flag = 0;
		HELP.error = "Do not allow other symbols (except in the annotations) except letters, numbers, decimal points, English single and double quotation marks, basic operators, bounds, space, tabs, and newlines!";
	}
}
//词法分析主函数
void LEXER::Lexer()
{
	do
	{
		lexerHelp();
	} while (HELP.flag && DATA.size() != 0);
}
//输出进行词法分析之后的结果
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
		cout << "ERROR[" << HELP.row << "]：" << HELP.error << endl;
	}
	else if (HELP.parenthesis > 0)
	{
		cout << "ERROR[" << HELP.row << "]：" << "Missing')'!" << endl;
		HELP.flag = 0;
	}
	else if (HELP.parenthesis < 0)
	{
		cout << "ERROR[" << HELP.row << "]：" << HELP.error << endl;
		HELP.flag = 0;
	}
	else if (HELP.bracket > 0)
	{
		cout << "ERROR[" << HELP.row << "]：" << "Missing']'!" << endl;
		HELP.flag = 0;
	}
	else if (HELP.bracket < 0)
	{
		cout << "ERROR[" << HELP.row << "]：" << HELP.error << endl;
		HELP.flag = 0;
	}
	else if (HELP.brace > 0)
	{
		cout << "ERROR[" << HELP.row << "]：" << "Missing'}'!" << endl;
		HELP.flag = 0;
	}
	else if (HELP.brace < 0)
	{
		cout << "ERROR[" << HELP.row << "]：" << HELP.error << endl;
		HELP.flag = 0;
	}
	else if (HELP.flag)
	{
		cout << "Lexical analysis is over successfully!" << endl;
	}
	cout << "------------------------------------------------------------" << endl;
}


//四元式
class QuaExp
{
public:
	//四元式第一个位置
	string op;
	//第二个位置
	string s1;
	//第三个位置
	string s2;
	//第四个位置
	string s3;
	//第四个位置
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
//对四元式类型<<重载
ostream& operator<<(ostream& output, QuaExp q)
{
	output << "(" << q.op << "," << q.s1 << "," << q.s2 << ",";
	if (q.s3 == "") output << q.t3;
	else output << q.s3;
	output << ")" << endl;
	return output;
}
//标识符/临时变量
class Identifier
{
public:
	Identifier(string name, string kind, int position = 0) :name(name), kind(kind), position(position) {};
	//名称
	string name;
	//属性(int/float)
	string kind;
	//值
	string value;
	//符号表中的位置
	int position;
};
//查看某标识符是否在符号表中，是返回位置，不是返回-1
int lookUp(string name, vector<Identifier>& SymTb)
{
	for (int i = 0; i < SymTb.size(); i++)
	{
		if (SymTb[i].name == name)return SymTb[i].position;
	}
	return -1;
}
//查看某临时变量是否在临时变量表中，是返回临时变量编码，不是返回-1
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
//int转string
string Itoa(int input)
{
	char* ch = new char[1000];
	_itoa_s(input, ch, 1000, 10);
	string t = ch;
	return t;
}
//float转string
string Ftoa(float input)
{
	ostringstream buf;
	buf << input;
	return buf.str();
}
//往符号表中新增标识符,如果name和kind都一样，则增加失败，其余成功
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
//往符号表中新增临时变量,返回临时变量的name
string enterTemp(string kind, string value, vector<Identifier>& TempTb)
{
	string name = "temp" + Itoa(TempTb.size() + 1);
	Identifier temp(name, kind);
	temp.value = value;
	TempTb.push_back(temp);
	return name;
}
//回填函数
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
//拉链函数
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
//符号栈中的元素类型,属性
class Symbol
{
public:
	//本身的符号(产生式中的符号，rop,id，i等)
	string it;
	//真实符号：rop对应的具体符号，id对应的具体name
	string realValueStr;
	//int类型的真值
	int realValueInt;
	//float类型的真值
	float realValFloat;
	int codebegin;
	int chain;
	int True;
	int False;
	//类型，int/float
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
//警告信息
struct warning
{
	//产生警告的行
	int row;
	//产生警告的原因
	string warn;
};
//语法和语义分析需要的辅助类型
struct ComHelp
{
	//当前行
	int row;
	//报错原因
	string error;
	//当前检测状态
	int flag;
	//产生的警告信息
	queue<warning> warnings;
};

//产生式
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
	//左部
	string Left;
	//右部
	string Right;

	//计算右部符号的多少（用于计算归约时弹出多少符号）
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
	//归约，语法制导翻译
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
			//查id是否存在
			int pos = lookUp(arr[3].realValueStr, SymTb);
			//cout << arr[3].realValueStr <<"  " <<arr[1].realValueStr<<endl;
			//id存在
			if (pos != -1)
			{
				//检查AE是否是标识符
				int AEPlace = lookUp(arr[1].realValueStr, SymTb);
				//不是标识符
				if (AEPlace == -1)
				{
					//检查AE是否是临时变量
					AEPlace = lookUpTemp(arr[1].realValueStr, TempTb);
					//是临时变量
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
				//是标识符
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
			//id不存在
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
			//类型不一致
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '*' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B是int，则C是float
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
			//类型一致
			else
			{
				//都是int
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
			//类型不一致
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '/' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B是int，则C是float
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
			//类型一致
			else
			{
				//都是int
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
			//类型不一致
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '+' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B是int，则C是float
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
			//类型一致
			else
			{
				//都是int
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
			//类型不一致
			if (arr[0].Att != arr[2].Att)
			{
				warning temp = { Help.row,"The left and right operand types of the operator '-' are inconsistent!The result is forced to be a float!" };
				Help.warnings.push(temp);
				L.Att = "float";
				L.it = "id";
				//B是int，则C是float
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
			//类型一致
			else
			{
				//都是int
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
	//若无移入，则goto值为-1，若无归约，则产生式左部和右部都是@
	ActGo(int g = -1, string l = "@", string r = "@")
	{
		goTo = g;
		pro.Left = l;
		pro.Right = r;

	}
	//要转入的状态
	int goTo;
	//要规约的式子
	Production pro;
};
//状态
class MyStatus
{
public:
	//存储一个状态中遇到不同的符号进行的不同的ActGo
	//每个元素存储形式为：（符号，相应操作）
	map<string, ActGo> status;
	//存储该状态中允许遇到的有效字符
	vector<string> charSet;
	//初始化charSet（非自动）tia
	void init()
	{
		for (map<string, ActGo>::iterator it = status.begin(); it != status.end(); it++)
		{
			charSet.push_back(it->first);
		}
	}
};
//切割字符串，处理分析表文件中的内容
void cut(string data, string& ch, int& got, string& l, string& r)
{
	string* arr = new string[data.size()];
	char split = ' ';
	if (data == "")  return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = data + split;
	size_t pos = strs.find(split);
	int i = 0;
	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		arr[i] = temp;
		i++;
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + 1, strs.size());
		pos = strs.find(split);
	}
	ch = arr[0];
	got = atoi(arr[1].c_str());
	l = arr[2];
	r = arr[3];
}
//将分析表的内容映射到vector<MyStatus>中
vector<MyStatus> initiate(int* arr, int n, string filename)
{
	//返回的变量
	vector<MyStatus> temp;
	fstream FI(filename);
	//第一层循环得到每一个状态
	for (int i = 0; i < n; i++)
	{
		MyStatus s1;
		//第二层循环得到每个状态的所有配对（有效符号，相应的操作（ActionOrGoto））
		for (int j = 0; j < arr[i]; j++)
		{
			string data;
			getline(FI, data);
			//有效符号
			string ch;
			//goto值
			int got;
			//归约式的左部
			string l;
			//规约式的右部
			string r;
			//利用函数得到以上的值
			cut(data, ch, got, l, r);
			//cout << ch << " " << got << " " << l << " " << r << endl;
			ActGo a1(got, l, r);
			pair<string, ActGo> p1(ch, a1);
			s1.status.insert(p1);
		}
		//得到一个状态中的所有内容后要初始化该状态中的charSet变量
		s1.init();
		temp.push_back(s1);
	}
	return temp;
}
//判断是否为实数
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
//判断是否为关系符号
bool isRelaSym(Token myData)
{
	string data = myData.value;
	if (data == "<" || data == ">" || data == "<=" || data == ">=" || data == "==" || data == "!=") return true;
	return false;
}
//判断是否是float
bool isFloat(string In)
{
	for (int i = 0; i < In.size(); i++)
	{
		if (In[i] == '.')return true;
	}
	return false;
}
//判断是否是标识符或关键字
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
	//词法分析器
	LEXER myLexer;
	//状态栈
	stack<int> StatusStack;
	//符号栈
	stack<Symbol> SymbolStack;
	//输入串
	queue<Token> Input;
	//中间代码
	vector< QuaExp> InterCode;
	//ActionGoto表
	vector<MyStatus> AnaSta;
	//语法分析，语义分析辅助变量
	ComHelp Help = { 1,"",0 };
	//符号表
	vector<Identifier> SymTb;
	//临时变量表
	vector<Identifier> TempTb;

	//语法分析和语义分析辅助函数
	void syAndSeAnaHelp();
	//语法分析和语义分析
	void syAndSeAna();
	//结果输出
	void printResult();

};
void Compiler::syAndSeAnaHelp()
{
	//当前状态
	int curSta = StatusStack.top();
	//当前输入符号
	string curSym;
	while (Input.front().value == "\n")
	{
		Input.pop();
		Help.row++;
	}
	//数字和其他符号分开处理，使得当前字符类型为char
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
		//先查找当前输入符号是否在当前状态的charSet中，不在的话，直接匹配失败
		if (AnaSta[curSta].charSet[i] == curSym)
		{
			//再在状态中寻找遇到当前符号对应的操作（ActionOrGoto）
			for (map<string, ActGo>::iterator it = AnaSta[curSta].status.begin(); it != AnaSta[curSta].status.end(); it++)
			{
				if (it->first == curSym)
				{
					//对应的操作为acc，匹配成功
					if (it->second.pro.Right == "acc")
					{
						//cout << "结果为：" << symbol.top().value << endl;
						Help.flag = 1;
						return;
					}
					//对应的操作为移进
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
					//对应的操作为归约
					else
					{
						//记录要从状态栈和符号栈弹出多少状态和符号
						int count = it->second.pro.rightSize();
						//记录符号栈弹出的内容
						Symbol* arr = new Symbol[count];
						for (int k = 0; k < count; k++)
						{
							StatusStack.pop();
							arr[k] = SymbolStack.top();
							SymbolStack.pop();
						}

						//更新当前状态和符号
						curSta = StatusStack.top();
						SymbolStack.push(Symbol(it->second.pro.Left, 0));//符号栈要先移进归约式的左部
						curSym = SymbolStack.top().it;

						//如果匹配当前符号（归约式的左部）则继续匹配，否则匹配失败
						for (map<string, ActGo>::iterator it1 = AnaSta[curSta].status.begin(); it1 != AnaSta[curSta].status.end(); it1++)
						{
							if (it1->first == curSym)
							{
								//非终结符对应的一定是移入操作，否则匹配失败
								if (it1->second.goTo == -1)
								{
									Help.flag = -1;
									Help.error = "Unable to match during parsing!";
									return;
								}
								it->second.pro.action(SymbolStack.top(), arr, InterCode, SymTb, TempTb, Help);//注意是对it操作，不是it1
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
	//返回0表示匹配还需继续，返回1表示匹配成功，返回-1表示匹配失败,返回-2表示分母为0
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