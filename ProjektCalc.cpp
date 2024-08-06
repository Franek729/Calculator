#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
using namespace std;
//names of functions that are handled by calculator
enum name
{
    NONE,
    MIN,
    MAX,
    N,
    IF
};

struct function
{
    enum name Name;
    int n;
};
//In Reverse Polish Notation the data in node may be a number, an operator, a bracket or a function
struct Data
{
    int number = -1;
    function Function;
    char Op;
    char bracket;
};

struct node
{
    Data data;
    node* next;
    node* previous;
};

struct stack
{
    struct node* start;
    struct node* end;
};

struct output_type
{
    char* out;
    int size;
    int occupied;
};

void cleanOutput(output_type& newOutput)
{
    newOutput.size = 8;
    newOutput.occupied = 0;
    newOutput.out = new char[newOutput.size];
    for (int i = 0; i < newOutput.size; i++)
    {
        newOutput.out[i] = '\0';
    }
}
//adding char to output
void addToOutput(output_type& Output1, char c)
{
    if (Output1.occupied == Output1.size)
    {
        char* newout = new char[Output1.size * 2];
        for (int i = 0; i < Output1.occupied; i++)
        {
            newout[i] = Output1.out[i];
        }
        for (int i = Output1.occupied; i < (Output1.occupied * 2) - 1; i++)
        {
            newout[i] = '\0';
        }
        Output1.size *= 2;
        delete[] Output1.out;
        Output1.out = newout;
    }
    Output1.out[Output1.occupied] = c;
    Output1.occupied++;
}

void deleteOutput(output_type OutputDel)
{
    delete[] OutputDel.out;
}

void initStack(stack& Stack)
{
    Stack.start = nullptr;
    Stack.end = nullptr;
}

void push(Data x, stack& Stack)
{
    node* newNode = new node;
    newNode->data = x;
    if (Stack.end != nullptr)
    {
        Stack.end->next = newNode;
        newNode->previous = Stack.end;
        Stack.end = newNode;
    }
    else
    {
        Stack.start = newNode;
        Stack.end = newNode;
        newNode->previous = nullptr;
        newNode->next = nullptr;
    }
}
//cleaning data so there are no 'garbage' values that may cause errors
void clean_data(Data& tmp)
{
    tmp.bracket = '\0';
    tmp.Op = '\0';
    tmp.Function.n = 0;
    tmp.Function.Name = NONE;
    tmp.number = -1;
}

Data pop(stack& Stack)
{
    if (Stack.end == Stack.start)
    {
        node* toPop = Stack.end;
        Stack.start = nullptr;
        Stack.end = nullptr;
        Data tmp = toPop->data;
        delete toPop;
        return tmp;
    }
    else if (Stack.end != nullptr)
    {
        node* toPop = Stack.end;
        Stack.end = toPop->previous;
        Stack.end->next = nullptr;
        Data tmp = toPop->data;
        delete toPop;
        return tmp;
    }
    else
    {
        Data tmp;
        clean_data(tmp);
        tmp.number = -1;
        return tmp;
    }
}
//printing current stack content
void PrintStack(const stack& Stack, char c)
{
    printf("%c ", c);
    node* current = Stack.end;
    while (current != Stack.start)
    {
        printf("%d ", current->data.number);
        current = current->previous;
    }
    printf("%d ", current->data.number);
    cout << endl;
}
//printing functions from the stack
void PrintStack_f(const stack& Stack, char* arr)
{
    printf("%s ", arr);
    node* current = Stack.end;
    while (current != Stack.start)
    {
        printf("%d ", current->data.number);
        current = current->previous;
    }
    printf("%d ", current->data.number);
    cout << endl;
}

bool isEmpty(stack& Stack)
{
    if (Stack.start == nullptr)
    {
        return true;
    }
    return false;
}

void clear_char_arr(char* array, int Size)
{
    for (int i = 0; i < Size; i++)
    {
        array[i] = 0;
    }
}

int Is_Operator(char c)
{
    if (c == '+' || c == '-')
    {
        return 1;
    }
    else if (c == '*' || c == '/')
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

bool IsNumber(const char* c)
{
    int i = 0;
    if ((c[i] > 47) && (c[i] < 58))
    {
        return true;
    }
    return false;
}

int IsFunction(const char* c)
{
    int i = 0;
    if (c[i] == 'M')
    {
        if (c[i + 1] == 'I' && c[i + 2] == 'N')
        {
            return 1;
        }
        else if (c[i + 1] == 'A' && c[i + 2] == 'X')
        {
            return 2;
        }
        return 0;
    }
    else if (c[i] == 'I' && c[i + 1] == 'F')
    {
        return 3;
    }
    else if (c[i] == 'N')
    {
        return 4;
    }
    return 0;
}

void IntToArray(Data& tmp, output_type& output, int& i)
{
    int temp = tmp.Function.n;
    int numDigits = 0;
    int k = 0;
    do {
        numDigits++;
        temp /= 10;
    } while (temp != 0);

    char* tempNumber = new char[numDigits + 1];
    sprintf(tempNumber, "%d", tmp.Function.n);
    while (tempNumber[k] != '\0')
    {
        addToOutput(output, tempNumber[k]);
        k++;
    }
    addToOutput(output, ' ');
}

void writeFunctionM(output_type& output, int& i, int n, Data& tmp)
{
    int j = 0;
    char arr[3];
    arr[0] = 'M';
    if (tmp.Function.Name == MIN)
    {
        arr[1] = 'I';
        arr[2] = 'N';
    }
    else if (tmp.Function.Name == MAX)
    {
        arr[1] = 'A';
        arr[2] = 'X';
    }
    while (arr[j] != '\0')
    {
        addToOutput(output, arr[j]);
        j++;
        if (arr[j] == 'N' || arr[j] == 'X')
        {
            addToOutput(output, arr[j]);
            break;
        }
    }
    IntToArray(tmp, output, i);
}
//handling the functions the calculator is capable of doing
//max function
int MAX_f(int* numbers, int i)
{
    int j = 0;
    int maxVal = numbers[j];
    for (j = 1; j < i; j++)
    {
        if (maxVal < numbers[j])
        {
            maxVal = numbers[j];
        }
    }
    return maxVal;
}
//min function
int MIN_f(int* numbers, int i)
{
    int j = 0;
    int minVal = numbers[j];
    for (j = 1; j < i; j++)
    {
        if (minVal > numbers[j])
        {
            minVal = numbers[j];
        }
    }
    return minVal;
}
//if works like this IF(a,b,c) => If a > 0 then return b, else return c
int IF_f(int* numbers)
{
    if (numbers[2] > 0)
    {
        return numbers[1];
    }
    return numbers[0];
}
//returns negation
int N_f(int n)
{
    return ((-1) * n);
}

void Ev_If(stack& Stack)
{
    int* numbers = new int[3];
    for (int k = 0; k < 3; k++)
    {
        Data tmp;
        clean_data(tmp);
        tmp = pop(Stack);
        numbers[k] = tmp.number;
    }
    int n = IF_f(numbers);
    delete[]numbers;
    Data tmp;
    clean_data(tmp);
    tmp.number = n;
    push(tmp, Stack);
}

void Ev_MinMax(char* token, stack& Stack)
{
    int n;
    int* numbers = new int[token[3]];
    int j = atoi(token + 3);
    for (int k = 0; k < j; k++)
    {
        Data tmp;
        clean_data(tmp);
        tmp = pop(Stack);
        numbers[k] = tmp.number;
    }
    if (token[2] == 'N')
    {
        n = MIN_f(numbers, j);
    }
    else if (token[2] == 'X')
    {
        n = MAX_f(numbers, j);
    }
    delete[]numbers;
    Data tmp;
    clean_data(tmp);
    tmp.number = n;
    push(tmp, Stack);
}

void Ev_Operator(int a, int b, Data& tmp, stack& Stack, char c)
{
    int n;
    switch (c)
    {
    case '+':
        n = a + b;
        tmp.number = n;
        push(tmp, Stack);
        break;

    case '-':
        n = a - b;
        tmp.number = n;
        push(tmp, Stack);
        break;

    case '*':
        n = a * b;
        tmp.number = n;
        push(tmp, Stack);
        break;

    case '/':
        n = a / b;
        tmp.number = n;
        push(tmp, Stack);
        break;
    }
}

int read_output(const char* arr, char* tmp)
{
    int j = 0, i = 0;
    if (arr[i] == ' ')
    {
        i++;
    }
    while (arr[i] != ' ')
    {
        if (arr[i] == '.')
        {
            break;
        }
        tmp[j] = arr[i];
        j++;
        i++;
    }
    tmp[j] = ' ';
    j = 0;
    return i;
}

int read_till_space(char* tmp, int size)
{
    int i = 0;
    tmp[i] = getchar();
    if (tmp[i] == '.' || tmp[i] == ')')
    {
        return i;
    }
    else if (tmp[i] == ' ')
    {
        tmp[i] = getchar();
        if (tmp[i] == '.')
        {
            return i;
        }
    }
    if (tmp[i] == '\n')
    {
        tmp[i] = getchar();
    }
    while (tmp[i] != ' ' && i < size - 1) // Check that i is less than size - 1
    {
        i++;
        tmp[i] = getchar();
        if (tmp[i] == '.' || tmp[i] == ')')
        {
            break;
        }
    }
    if (i < size - 1) // Check that i is less than size - 1
    {
        i++;
        tmp[i] = ' ';
    }
    return i;
}

int evaluation(const char* output, int n, stack& Stack)
{
    int i = 0;
    bool EndOfFile = false;
    char* token = new char[50];
    cout << endl;
    for (int j = 0; j < n; j++)
    {
        if (output[j] == '.')
        {
            break;
        }
        if (output[j] == ' ' && output[j + 1] != ' ')
        {
            printf(" ");
        }
        printf("%c", output[j]);
    }
    printf(" ");
    cout << endl;
    while (!EndOfFile)
    {
        clear_char_arr(token, 50);
        int size = read_output(output + i, token);
        if (IsNumber(token))
        {
            int n = atoi(token);
            Data tmp;
            clean_data(tmp);
            tmp.number = n;
            push(tmp, Stack);
        }
        else if (Is_Operator(token[0]) > 0)
        {
            PrintStack(Stack, token[0]);
            Data a, b, tmp;
            clean_data(a);
            clean_data(b);
            clean_data(tmp);
            b = pop(Stack);
            a = pop(Stack);
            if (b.number == 0 && token[0] == '/')
            {
                while (Stack.start != nullptr)
                {
                    Data tmp;
                    clean_data(tmp);
                    tmp = pop(Stack);
                }
                cout << "ERROR " << endl;
                delete[] token;
                return i;
                break;
            }
            Ev_Operator(a.number, b.number, tmp, Stack, token[0]);
        }
        else if (IsFunction(token) > 0)
        {
            if (IsFunction(token) == 1)
            {
                PrintStack_f(Stack, token);
                Ev_MinMax(token, Stack);
            }
            else if (IsFunction(token) == 2)
            {
                PrintStack_f(Stack, token);
                Ev_MinMax(token, Stack);
            }
            else if (IsFunction(token) == 3)
            {
                PrintStack_f(Stack, token);
                Ev_If(Stack);
            }
            else if (IsFunction(token) == 4)
            {
                PrintStack(Stack, token[0]);
                Data tmp = pop(Stack);
                int k = N_f(tmp.number);
                Data tmp1;
                clean_data(tmp1);
                tmp1.number = k;
                push(tmp1, Stack);
            }
        }
        i += size;
        if (i == n || output[i] == '.')
        {
            break;
        }
    }
    Data tmp;
    clean_data(tmp);
    tmp = pop(Stack);
    printf("%d ", tmp.number);
    cout << endl;
    delete[] token;
    return i;
}

int input1(stack& Stack, output_type& output, int& i)
{
    int count = 1, j = 0;
    bool EndOfFile = false;
    char* token = new char[60];
    while (!EndOfFile)
    {
        clear_char_arr(token, 50);
        int size = read_till_space(token, 50);
        if (IsNumber(token))
        {
            size += i;
            for (i; i < size; i++)
            {
                addToOutput(output, token[j]);
                j++;
            }
            j = 0;
            if (Stack.end != nullptr && Stack.end->data.Function.Name == N)
            {
                Data tmp;
                clean_data(tmp);
                tmp = pop(Stack);
                delete[] token;
                return count;
            }
        }
        else if (token[0] == '(')
        {
            Data tmp1;
            clean_data(tmp1);
            tmp1.bracket = '(';
            push(tmp1, Stack);
        }
        else if (Is_Operator(token[0]) > 0)
        {
            if (Stack.end != nullptr)
            {
                while (Stack.end->data.Op != '\0' && (Is_Operator(token[0]) <= Is_Operator(Stack.end->data.Op)))
                {
                    Data tmp = pop(Stack);
                    addToOutput(output, tmp.Op);
                    addToOutput(output, ' ');
                    if (Stack.end == nullptr)
                    {
                        Data tmp;
                        clean_data(tmp);
                        tmp.Op = token[0];
                        push(tmp, Stack);
                        break;
                    }
                }
                if (Stack.end->data.Op == '\0' || (Is_Operator(token[0]) > Is_Operator(Stack.end->data.Op)))
                {
                    Data tmp;
                    clean_data(tmp);
                    tmp.Op = token[0];
                    push(tmp, Stack);
                }
            }
            else
            {
                Data tmp;
                clean_data(tmp);
                tmp.Op = token[0];
                push(tmp, Stack);
            }
        }
        else if (IsFunction(token) > 0)
        {
            Data tmp;
            clean_data(tmp);
            if (IsFunction(token) == 1)
            {
                tmp.Function.Name = MIN;
                push(tmp, Stack);
                tmp.Function.n = input1(Stack, output, i);
                Stack.end->data.Function.n = tmp.Function.n;
                Data tmp1;
                clean_data(tmp1);
                tmp1 = pop(Stack);
                writeFunctionM(output, i, 1, tmp);
                if (Stack.end != nullptr && Stack.end->data.Function.Name == N)
                {
                    return count;
                }
            }
            else if (IsFunction(token) == 2)
            {
                tmp.Function.Name = MAX;
                push(tmp, Stack);
                tmp.Function.n = input1(Stack, output, i);
                Stack.end->data.Function.n = tmp.Function.n;
                Data tmp1;
                clean_data(tmp1);
                tmp1 = pop(Stack);
                writeFunctionM(output, i, 1, tmp);
                if (Stack.end != nullptr && Stack.end->data.Function.Name == N)
                {
                    return count;
                }
            }
            else if (IsFunction(token) == 3)
            {
                tmp.Function.Name = IF;
                push(tmp, Stack);
                tmp.Function.n = input1(Stack, output, i);
                Stack.end->data.Function.n = tmp.Function.n;
                Data tmp1;
                clean_data(tmp1);
                if (Stack.end != nullptr && Stack.end->data.Function.Name == IF)
                {
                    tmp1 = pop(Stack);
                }
                addToOutput(output, 'I');
                addToOutput(output, 'F');
                addToOutput(output, ' ');
                if (Stack.end != nullptr && Stack.end->data.Function.Name == N)
                {
                    delete[] token;
                    return count;
                }
            }
            else if (IsFunction(token) == 4)
            {
                int count_n = 1;
                if (Stack.end != nullptr && Stack.end->data.Function.Name == N)
                {
                    while (Stack.end != nullptr && Stack.end->data.Function.Name == N)
                    {
                        Data tmp1;
                        clean_data(tmp1);
                        tmp1 = pop(Stack);
                        count_n++;
                    }
                }
                tmp.Function.Name = N;
                push(tmp, Stack);
                tmp.Function.n = input1(Stack, output, i);
                Data tmp1;
                clean_data(tmp1);
                if (Stack.end != nullptr && Stack.end->data.Function.Name == N)
                {
                    tmp1 = pop(Stack);
                }
                char* token1 = new char[10];
                for (int k = 0; k < count_n; k++)
                {
                    addToOutput(output, 'N');
                    addToOutput(output, ' ');
                }
                delete[]token1;
            }
        }
        else if (token[0] == ',')
        {
            while (Stack.end->data.bracket != '(')
            {
                Data tmp;
                clean_data(tmp);
                tmp = pop(Stack);
                if (tmp.number != -1)
                {
                    IntToArray(tmp, output, i);
                }
                else if (tmp.Op != '\0')
                {
                    addToOutput(output, tmp.Op);
                    addToOutput(output, ' ');
                }
            }
            count++;
        }
        else if (token[0] == ')')
        {
            while (Stack.end->data.bracket != '(')
            {
                Data tmp;
                clean_data(tmp);
                tmp = pop(Stack);
                if (tmp.Op != '\0')
                {
                    addToOutput(output, tmp.Op);
                    addToOutput(output, ' ');
                }
            }
            Data tmp1;
            clean_data(tmp1);
            tmp1 = pop(Stack);
            if (Stack.end == nullptr)
            {
                continue;
            }
            else if (Stack.end != nullptr && Stack.end->data.Function.Name == MIN || Stack.end->data.Function.Name == MAX || Stack.end->data.Function.Name == IF || Stack.end->data.Function.Name == N)
            {
                delete[] token;
                return count;
            }
            else
            {
                continue;
            }
        }
        else if (token[0] == '.')
        {
            while (Stack.end != nullptr)
            {
                Data tmp = pop(Stack);
                addToOutput(output, tmp.Op);
                addToOutput(output, ' ');
            }
            if (Stack.end == nullptr)
            {
                addToOutput(output, '.');
                evaluation(output.out, output.occupied, Stack);
            }
            delete[] token;
            addToOutput(output, '.');
            return i;
        }
        else
        {
            delete[] token;
            return i;
        }
    }
    delete[] token;
    return i;
}

int main()
{
    char tmp[5];
    int i = 0;
    tmp[i] = getchar();
    while (tmp[i] != '\n')
    {
        i++;
        tmp[i] = getchar();
    }
    tmp[i] = '\0';
    int n = atoi(tmp);
    for (int i = 0; i < n; i++)
    {
        stack Stack;
        initStack(Stack);
        int j = 0;
        output_type output;
        cleanOutput(output);
        int k = input1(Stack, output, j);
        deleteOutput(output);
    }
}

