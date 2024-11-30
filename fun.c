#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct elem {
    char data;
    struct elem * next;
} elem;

elem * push(elem * top, int data)
{
    elem * ptr = malloc(sizeof(elem));
    ptr->data = data;
    ptr->next = top;
    return ptr;
}

elem * pop(elem * top)
{
    if(top == NULL)return top;
 
    elem * ptr_next = top->next;
    free(top);
 
    return ptr_next;
}

void show_stack(const elem * top)
{
    const elem * current = top;
    while(current != NULL) 
    {
        printf("%hhd\n", current->data);
        current = current->next;
    }
    printf("\n");
}

void whatinside(char * a, size_t array_size)
{
    size_t i;

    for (i = 0; i < array_size; i++)
    {
        printf("%hhd\t", *(a + i));
    }
    printf("\n\n");
}

char get_order(char s) // получить приоритет операции для символа
{
    char ot;

    switch (s)
    {
    case(40):   // открывающая скобка
        ot = 0;
        break;
    case(41):   // закрывающая скобка
        ot = 1;
        break;
    case(42):   // умножение
        ot = 3;
        break;
    case(43):
    case(45):   // сложение и вычитание
        ot = 2;
        break;
    case(47):   // деление
        ot = 3;
        break;
    case(94):   // возведение в степень
        ot = 4;
        break;
    default:
        break;
    }
    return ot;
}

char* to_polsca(char* expr, size_t array_size)
{
    elem * top = NULL;
    char * par = malloc(array_size * sizeof(char));
    char flag_pushed = 1;
    size_t i, j;

    j = 0;
    for (i = 0; *(expr + i) != 10; i++)
    {
        //printf("flag: %d\n", flag_pushed);
        //show_stack(top);
        //whatinside(par, array_size);
        if (*(expr + i) == ' ')
        {
            if ((j != 0) && (par[j - 1] != ' '))
            {
                par[j] = ' ';
                j++;
            }
        }
        else if ((*(expr + i) == ',') || (*(expr + i) == '.'))
        {
            if ((j == 0) || !((par[j - 1] <= '9') && (par[j - 1] >= '0')))
            {
                strcpy(expr, "Error. Invalid position for pointer.\n");
                free(par);
                return expr;
            }
            par[j] = '.';
            j++;
            flag_pushed = 0;
        }
        else if ((*(expr + i) <= '9') && (*(expr + i) >= '0')) 
        {
            if ((j != 0) && 
            (!(((par[j - 1] <= '9') && (par[j - 1] >= '0')) || (par[j - 1] == ' ') || (par[j - 1] == ',') || (par[j - 1] == '.')) || 
            (flag_pushed && (par[j - 1] != ' '))))
            {
                par[j] = ' ';
                j++;
            }
            par[j] = *(expr + i);
            j++;
            flag_pushed = 0;
        }
        else if ((((*(expr + i) >= '(') && (*(expr + i) <= '/')) || (*(expr + i) == '^')) && !((*(expr + i) == ',') || (*(expr + i) == '.')))
        {
            if ((par[j - 1] == ',') || (par[j - 1] == '.'))
            {
                strcpy(expr, "Error. Invalid pointer and operation order.\n");
                free(par);
                return expr;
            }
            else if ((top == NULL) && ((*(expr + i) == ')') || ((j == 0) && (*(expr + i) != '('))))
            {
                //show_stack(top);
                //whatinside(par, array_size);
                strcpy(expr, "Error. Invalid operation order.\n");
                free(par);
                return expr;
            }
            else if ((top == NULL) && (*(expr + i) != '(')) 
            {
                top = push(top, *(expr + i));
                flag_pushed = 1;
            }
            else if (*(expr + i) == '(')
            {
                top = push(top, *(expr + i));
            }
            else if (*(expr + i) == ')')
            {
                if (top != NULL)
                {
                    while(top != NULL)
                    {
                        if (top->data == '(')
                        {
                            top = pop(top);
                            break;
                        }
                        if ((j != 0) && (par[j - 1] != ' '))
                        {
                            par[j] = ' ';
                            j++;
                        }
                        if (top->data != '(') 
                        {
                            par[j] = top->data;
                            j++;
                        }
                        top = pop(top);
                    }
                }
                else 
                {
                    //show_stack(top);
                    //whatinside(par, array_size);
                    strcpy(expr, "Error. Invalid brackets oreder.\n");
                    free(par);
                    return expr;
                }
            }
            else if ((get_order(top->data) < get_order(*(expr + i))) && (*(expr + i) != ')') && !flag_pushed)
            {
                top = push(top, *(expr + i));
                flag_pushed = 1;
            }
            else if ((*(expr + i) != ')') && !flag_pushed)
            {
                do 
                {
                    if ((j != 0) && (par[j - 1] != ' '))
                    {
                        par[j] = ' ';
                        j++;
                    }
                    if (top->data != '(') 
                    {
                        par[j] = top->data;
                        j++;
                    }
                    top = pop(top);
                }
                while((top != NULL) && (get_order(top->data) >= get_order(*(expr + i))));
                top = push(top, *(expr + i));
                flag_pushed = 1;
                //show_stack(top);
                //whatinside(par, array_size);
            }
            else 
            {
                //show_stack(top);
                //whatinside(par, array_size);
                strcpy(expr, "Error. All operations need 2 operands.\n");
                free(par);
                return expr;
            }
        }
        else 
        {
            //show_stack(top);
            //whatinside(par, array_size);
            strcpy(expr, "Error. You are trying to use undefined symbol.\n");
            free(par);
            return expr;
        }
    }
    if ((*(expr + i - 1) == ',') || *(expr + i - 1) == '.')
    {
        strcpy(expr, "Error. Invalid place for decimal separator.\n");
        free(par);
        return expr;
    }
    while(top != NULL) 
    {
        if (top->data == '(') 
        {
            //show_stack(top);
            //whatinside(par, array_size);
            strcpy(expr, "Error. Invalid brackets expression.\n");
            free(par);
            return expr;
        }
        if (top->data == par[j - 1])
        {
            //show_stack(top);
            //whatinside(par, array_size);
            strcpy(expr, "Error. Invalid operation order.\n");
            free(par);
            return expr;
        }
        if ((j != 0) && (par[j - 1] != ' '))
        {
            par[j] = ' ';
            j++;
        }
        par[j] = top->data;
        j++;
        top = pop(top);
    }

    par[j] = 10;
    par[j + 1] = 0;
    //show_stack(top);
    //whatinside(par, array_size);
    strcpy(expr, par);
    free(par);
    return expr;
}

char* calculate(char* pol_expr, size_t array_size)
{
    size_t i, j;
    float a, b, c;
    char op1[array_size] = {};
    char op2[array_size] = {};
    char ins[array_size];
    char* spases[array_size];
    char swap[array_size];
    char* op = NULL;

    while(strchr(pol_expr, ' ') != NULL)
    {
        op = NULL;
        memset(spases, 0, array_size);
        memset(op1, 0, array_size);
        memset(op2, 0, array_size);
        memset(ins, 0, array_size);
        if ((pol_expr[0] > '9' || pol_expr[0] < '0') && (pol_expr[0] != '~'))
        {
            strcpy(pol_expr, "Error. Invalid operation order or invalid symbol in polsca.\n");
            return pol_expr;
        }
        spases[0] = pol_expr - 1;
        j = 0;
        for (i = 0; (i < array_size) && (*(pol_expr + i) != 10); i++)
        {
            if ((*(pol_expr + i) == '+') || (*(pol_expr + i) == '-') || (*(pol_expr + i) == '*') || (*(pol_expr + i) == '/') || (*(pol_expr + i) == '^'))
            {
                op = (pol_expr + i);
                break;
            }
            if (*(pol_expr + i) == ' ')
            {
                j++;
                spases[j] = (pol_expr + i);
            }
        }
        if ((op == NULL) && (j > 0))
        {
            strcpy(pol_expr, "Error. Invalid number of operands in polsca. There are too many.\n");
            return pol_expr;
        }
        else if ((op != NULL) && (j < 2))
        {
            strcpy(pol_expr, "Error. Invalid number of operands in polsca. There are too few.\n");
            return pol_expr;
        }
        else if (op != NULL)
        {
            strncpy(op1, (spases[j - 2] + 1), (spases[j - 1] - spases[j - 2]));
            strncpy(op2, (spases[j - 1] + 1), (spases[j] - spases[j - 1]));
            if (op1[0] == '~') op1[0] = '-';
            if (op2[0] == '~') op2[0] = '-';
            a = atof(op1);
            b = atof(op2);
            switch (*op)
            {
            case '+':
                c = a + b;
                break;
            case '*':
                c = a * b;
                break;
            case '-':
                c = a - b;
                break;
            case '^':
                c = pow(a, b);
                break;
            case '/':
                if (b == 0)
                {
                    strcpy(pol_expr, "Error. Division by zero in polsca.\n");
                    return pol_expr;
                    break;
                }
                c = a / b;
                break;
            default:
                c = -12345.6789f;
                break;
            }
        }
        memset(swap, 0, array_size);
        strncpy(swap, pol_expr, (spases[j - 2] - pol_expr + 1));
        sprintf(ins, "%f", c);
        if (ins[0] == '-') ins[0] = '~';
        strcat(swap, ins);
        strcat(swap, (op + 1));
        strcpy(pol_expr, swap);
    }
    if (pol_expr[0] == '~') pol_expr[0] = '-';
    return pol_expr;
}

void help()
{
    printf("This is simple calculator, that capable to take string with expression as input.\n"
    "You can use \",\" and \".\" as decimal separators for fractions. Also number of spaces irrelevant for\n"
    "value of equation. There are no unary minus, so for negative numbers you need to use \"(0-n)\" construction,\n"
    "where \"n\" is module of your negative number.\n"
    "That calculator can do these list of operations:\n"
    "\t+ --- sum\n\t- --- subtraction\n\t* --- multiplication\n\t/ --- division\n\t^ --- exponentiation\n"
    "\t(...) --- changing the order of operations\n\nFor stopping work of a programm just tipe\"exit\" in lowercase.\n");
}

int main()
{
    size_t array_size = 101; // чтобы пользователь смог впихнуть в fgets 100 обещаных символов без учёта перевода строки 
    while(1)
    {
        char *expr = malloc(array_size * sizeof(char));

        memset(expr, 0, array_size);
        printf("Hello, dear user! This is a simple calculator."
        " You can tipe your expression for start your work or type \"help\" in lowercase for more information.\n");
        fgets(expr, array_size, stdin);
        //whatinside(expr, array_size);
        if (strcmp(expr, "exit\n") == 0) break;
        else if(strcmp(expr, "help\n") == 0) 
        {
            help();
            break;
        }
        expr = to_polsca(expr, array_size);
        expr = calculate(expr, array_size);
        printf("%s", expr);
        free(expr);
    }
    return 0;
}