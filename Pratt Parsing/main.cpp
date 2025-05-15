#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum class charType{
	OPERATOR,
	SIGN,
	VALUE,
	VARIABLES,
	PARENTHESIS,
	UNKNOWN
};

charType getType(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/'||c=='%')
		return charType::OPERATOR;
	if (c >= '0' && c <= '9')
		return charType::VALUE;
	return charType::UNKNOWN;
}

bool isSign(char c)
{
	if(c=='-'||c=='+')
		return true;
	return false;
}

float getPriority(char c, bool right)
{
	switch (c)
	{
	case '+':
		return right ? 1.2f :1.1f;
	case '-':
		return right ? 1.2f : 1.1f;
	case '*':
		return right ? 2.2f : 2.1f;
	case '/':
		return right ? 2.2f : 2.1f;
	case '%':
		return right ? 2.2f : 2.1f;
	default:
		return 0;
	}
}

struct parsingTree
{
	vector<char> key;
	bool negative=false;

	parsingTree *left = nullptr;
	parsingTree *right = nullptr;

	~parsingTree()
	{
		delete left;
		delete right;
	}
};

void expressionParsing(parsingTree& tree,const string& expression,int &i,float priority)
{

	i++;
	parsingTree* left = new parsingTree();

	if (i < expression.size() && isSign(expression[i]))
	{
		if(expression[i] == '-') left->negative = !tree.negative;
		i++;
		while (i < expression.size() && isSign(expression[i]))
		{
			if (expression[i] == expression[i - 1])
			{
				return;
			}
			if (expression[i] == '-') left->negative = !left->negative;
			i++;
		}
	}

	if (i < expression.size() && expression[i] == '(')
	{
		expressionParsing(*left, expression, i, 0);
		i++;
	}
	else {
		while (i < expression.size() && charType::VALUE == getType(expression[i]))
		{
			left->key.push_back(expression[i]);
			i++;
		}
	}


	while (i < expression.size()) {

		if (charType::OPERATOR == getType(expression[i]))
		{
			if (priority < getPriority(expression[i], false))
			{
				parsingTree* branch = new parsingTree();

				branch->left = left;

				branch->key.push_back(expression[i]);
				branch->right = new parsingTree();
				expressionParsing(*branch->right, expression, i, getPriority(expression[i], true));
				left = branch;
			}
			else
			{
				break;
			}
		}
		else if (expression[i] == ')')
		{
			break;
		}
		else return;
	}

	tree = *left;
}

long long parsingPrint(parsingTree* tree)
{
	if (tree->negative) cout << "-";
	if (tree->left != nullptr)
		cout << "(";
	long long temp=0;
	for (int i = 0; i < tree->key.size(); i++)
	{
		cout << tree->key[i];
		temp =temp*10+ long long(tree->key[i] - '0');
	}


	if (tree->left != nullptr)
	{
		long long a, b;
		cout << " ";
		a=parsingPrint(tree->left);
		cout << " ";
		b = parsingPrint(tree->right);

		switch (tree->key[0])
		{
		case '+':
			temp = a + b;
			break;
		case '-':
			temp = a - b;
			break;
		case '*':
			temp = a * b;
			break;
		case '/':
			temp = a / b;
			break;
		case '%':
			temp = a % b;
			break;
		}
		cout << ")";
	}
	return tree->negative?-temp:temp;
}

int main()

{
	string expression;
	getline(cin, expression);

	parsingTree tree;

	int i = -1;
	expressionParsing(tree, expression, i, 0);
	long long ans=parsingPrint(&tree);
	cout << "\n" << ans;


	return 0;
}