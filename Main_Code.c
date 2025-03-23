#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define CR        0x0d  //carriage return
#define TAB       0x09
#define LF        0x0a  //line feed
#define BACKSPACE 0x08
#define NUL       '\0'  //empty character
#define QUIT      1
#define TRUE      1
#define FALSE     0
#define LENGTH    12    //size of the string

int input(char *string, int length);

double multiply(char *, int , int, int, double[*][*]);

void print_table(int , int , char , char);

void Clear_previous_messages(COORD,HANDLE);

COORD Handle_input(char *);

double median(char *, int, int, int, double[*][*]);

void Add_num_to_array(char *, int , int , double[*][*], COORD, COORD, HANDLE);

int table_width = 0, table_height = 0;

double add(char *, int , int , int , double[*][*]);

double average(char *, int , int , int , double[*][*]);

double min_value(char *str,int k,int a, int b,double c[a][b]);

double max_value(char *str,int k,int a, int b,double c[a][b]);

void print_result(int, char *, double, HANDLE, COORD);

void visual_clear(COORD, HANDLE);


int main(void)
{

    //������� ��� ������������ ������������� ����������


    int exit_flag = 1;
    char string[LENGTH];
    float a;
    char str[20];
    double d;
    int flag_2 = 1;
    int counter = 0;

    //����������� �� ���������� ����������� ��� �� �������� ���� ���� ���
    //������������ ���� ��� ����������� windows.h ��� winuser.h


    HANDLE hwin = GetConsoleWindow();                   //https://docs.microsoft.com/en-us/windows/console/getconsolewindow
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);      //https://docs.microsoft.com/en-us/windows/console/getstdhandle
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD InputRecord;
    DWORD fdwSaveOldMode;
    DWORD Events;
    DWORD fdwNewMode = ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    COORD coord;
    COORD inserted_coord;
    COORD default_coord;
    COORD cursor_temp_coord;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD Screen_buffer_size = {2000,2000};
    SetConsoleScreenBufferSize(hout,Screen_buffer_size);

    //�� �������� ������� Fullscreen

    ShowWindow(hwin,3);

    //����������� �� ���������� ��� ������

    printf("Please insert the table dimensions in the form [width]x[height]\n");
    scanf("%dx%d", &table_width, &table_height);
    fflush(stdin);
    COORD beginning = {0,0};

    //������� ��� � ������� ������� ������ ��� ������ �����

    while(flag_2)
    {
        if((table_width <= 0 || table_width > 25) || (table_height <= 0 || table_height > 25) )
        {

             SetConsoleCursorPosition(hout,beginning);
             printf("                                                                      \n\
                                                                                            ");
             SetConsoleCursorPosition(hout,beginning);
             printf("Please enter integer values between 1 and 25\n");
             scanf("%d x %d", &table_width, &table_height);
        }else
        {flag_2 = 0;}
    }
    default_coord.X = 0;
    default_coord.Y = 5 + 2*(table_height+1);
    double Actual_Table[table_height][table_width];
    char column_notation='A';
    int row_notation=1;
    print_table(table_width+1,table_height+1,column_notation,row_notation);

    //������������ ������

    for(int i = 0; i < table_height; i++)
    {
        for(int j = 0; j < table_width; j++)
        {
            Actual_Table[i][j] = 1.11111111111;
        }
    }

    GetConsoleMode(hin,&fdwSaveOldMode);
    SetConsoleMode(hin, fdwNewMode);

    while(exit_flag)
    {
        SetConsoleScreenBufferSize(hout,Screen_buffer_size); //https://docs.microsoft.com/en-us/windows/console/setconsolescreenbuffersize
        ReadConsoleInput(hin, &InputRecord,1,&Events);
        if(counter <= 20)
            SetConsoleCursorPosition(hout,default_coord);
        //Event handler (��� windows.h) ��� �������� ������ ��� ��� ������

        switch(InputRecord.EventType)                       //https://stackoverflow.com/questions/35797336/how-to-find-the-mouse-button-click-position-x-and-y
        {

         case KEY_EVENT: // �������� ��� �� ������������

                    switch (InputRecord.Event.KeyEvent.wVirtualKeyCode)
                    {
                        //��� �� ������� ����� �� Control
                        case VK_CONTROL:
                            SetConsoleMode(hin,fdwSaveOldMode);
                            fgets(str,20,stdin);
                            fflush(stdin);

                            //���� ���� �������� ��� if ��� ������� �� ������������� str ��� ����� ��� ����������
                            //�����������, �������� ��� input ��� ������

                            if(strncmp(str,"help",4) == 0 )
                            {
                                Clear_previous_messages(default_coord,hout);
                                printf("\n\nHelp:\n-Type: \"add row\" or \"add column\" and specify the row or column to add\
 all its elements\n-Type: \"median row\" or \"median column\" and specify the row or column to calculate its median\n\
-Type: \"average row\" or \"average column\" and specify the row or column to calculate its average\n\
-Type: \"min-max column-row and specify the row or column to calculate its minimum and maximum value\n\
-Type: \"mult column-row\" and specify the row or column to multiply all its elements\n\
-Type: \"clear\" to clear the whole table\n\
-Type: \"exit\" to close the program" );
                            }
                            else if(strncmp(str,"median",6) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double m = median(c[0],k,table_height,table_width,Actual_Table);
                                print_result(k,c[0],m,hout,default_coord);
                            }
                            else if(strncmp(str,"add",3) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double result = add(c[0] , k , table_width , table_height , Actual_Table);
                                print_result(k,c[0],result,hout,default_coord);
                            }
                            else if(strncmp(str,"mult",4) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double result = multiply(c[0] , k , table_height , table_width , Actual_Table);
                                print_result(k,c[0],result,hout,default_coord);
                            }
                            else if(strncmp(str,"clear",5) == 0)
                            {
                                for(int i = 0; i < table_height; i++)
                                {
                                    for(int j = 0; j < table_width; j++)
                                    {
                                        Actual_Table[i][j] = 1.11111111111;
                                    }
                                }
                                visual_clear(default_coord,hout);
                            }
                            else if(strncmp(str,"average",7) == 0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double result = average(c[0] , k , table_width , table_height , Actual_Table);
                                print_result(k,c[0],result,hout,default_coord);
                            }
                            else if(strncmp(str,"min",3)==0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double minim = min_value(c[0], k, table_height, table_width, Actual_Table);
                                print_result(k,c[0],minim,hout,default_coord);
                            }
                            else if(strncmp(str,"max",3)==0)
                            {
                                int j = 0;
                                char *c[2];
                                for(int i = 0; i < 15; i++)
                                {
                                    if(str[i] == 32 && j < 2)
                                    {
                                        c[j] = &str[i+1];
                                        j++;
                                    }
                                }
                                int k = atoi(c[1]);
                                double maxim = max_value(c[0], k, table_height, table_width, Actual_Table);
                                print_result(k,c[0],maxim,hout,default_coord);
                            }
                            else if(strncmp(str,"exit",4) == 0)
                            {
                                Clear_previous_messages(default_coord,hout);
                                exit_flag  = 0;
                            }

                            //��� ������� � ������� ��� �� ��� � ������� ��������� �� ��������� ���� ����
                            //������ ���� ��� ������� �������, �.�. �1: 32.4. ������ �� ��������� ���
                            //��������� �� �� �1: 32.4 �������� ��� �� 1�: 32.4

                            for(int i = 0; i < 12; i++)
                            {
                                if(str[i] == 58) //��������� ��� �� ������������� ���� ��� ��������� ':' ����.
                                {
                                    inserted_coord = Handle_input(str);

                                    //��������� ��� �� ������������� ��� ����� � ������� ����� ���� ��� ���� ��� ������
                                    //��� ��� ��� ������ ������ ������ ��� ��������� Handle_input().

                                    if(inserted_coord.X != 100 && inserted_coord.Y != 100)
                                    {
                                        if(inserted_coord.Y < table_height && inserted_coord.X < table_width)
                                        {
                                            Add_num_to_array(&str[i+1],table_width,table_height, Actual_Table, inserted_coord, default_coord, hout);
                                        }
                                    }
                                }
                            }

                            //������� ��� ������� ���� ���� ������������� ��� ���� (��� ������ �������)

                            SetConsoleCursorPosition(hout,default_coord);
                            //����������� ��� ������ ������� ��� ����� ���������� ��� ����� ��������� ��� ����������� input.
                            for(int i=0;i<table_width*13;i++)
                                printf(" ");
                            SetConsoleCursorPosition(hout,default_coord);
                            SetConsoleMode(hin,fdwNewMode);
                            break;

                        //Debug ������� ��� ������� ��� ������ ��� ��� ����� ��� ���� ������� �� ������ SHIFT.

                        /*case VK_SHIFT:
                            printf("\n");
                            for(int i = 0; i < table_height; i++)
                            {
                                for(int j = 0; j < table_width; j++)
                                {
                                    printf("%14f",Actual_Table[i][j]);
                                }
                                printf("\n");
                            }
                            break;*/
                    }

        case MOUSE_EVENT:  //Input ��� �� �������.

            //��� ��������� ���� ��� ���� ��� ������, ����� ��� ��� � ������� ���� ������� �������� ����.

             if(InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED &&
                InputRecord.Event.MouseEvent.dwMousePosition.Y < 5 + 2*table_height && InputRecord.Event.MouseEvent.dwMousePosition.X > 3 &&
                InputRecord.Event.MouseEvent.dwMousePosition.X < 3 + 13*table_width && InputRecord.Event.MouseEvent.dwMousePosition.Y%2 == 1 &&
                InputRecord.Event.MouseEvent.dwMousePosition.Y > 4)
                    {
                        coord.X = (((InputRecord.Event.MouseEvent.dwMousePosition.X-4)/13)*13 + 4); //���������� ����������� ��� offset ���� ���� ��� ���� ������ ��� ��� ��� ������ ���� ��������.
                        coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
                        SetConsoleCursorPosition(hout,coord);
                        printf("            ");                                                    //���������� ��� ������ �������� ��� ����� ����������� �������
                        SetConsoleCursorPosition(hout,coord);
                        SetConsoleMode(hin, fdwSaveOldMode);
                        input(string,LENGTH);
                        coord.X = (coord.X-3)/13;
                        coord.Y = (coord.Y-5)/2;
                        Actual_Table[coord.Y][coord.X] = atof(string);                             //����������� ��� ���� ��� ������ ��� ��������� ������ ������ ��� ������.
                        fflush(stdin);
                        SetConsoleCursorPosition(hout,default_coord);
                        SetConsoleMode(hin,fdwNewMode);
                    }
        }
        ++counter;
    }
}

//��������� ��� ����� ������������� ������� ������ ��� �������, ������� �� ��� ������� ��� ������.

/*������� �� input string ��� ���� ������� � ������� ��� ����� ��� ������ column-row X, ���� � � �������
��� ������-������� ��� ������ ���� ����� �������� �� ����� � ���������������. � �������� k �������������� ���
���� X. ������ � ��������� ������� �� ������ ��� ������ ��� ��� ���������� ���, ��� �� ������ �� ��������� ��������
���� ����� ��� �� ��� ������ �� ����� � ���������������. ��� �� �������� ��� ��� ����� �� �������� �������� ��� 1.1111111111,
���� �� ����� ��� ����������������� ������ ���� ��� ������������ �� ���������� ����� double, ���� ����� ������� �� str ���
�� ����������� ��� � ����� �� ����� �� ������ � �����*/
double multiply(char *str, int k, int table_height, int table_width , double table[table_height][table_width])
{
    double product = 1;
    int j = 0;
    if(strncmp(str,"column",6) == 0)
    {
        for(int i = 0; i < table_height; i++)
            {
                if(table[i][k-1] != 1.11111111111)
                {
                    j++;
                    product *= table[i][k-1];
                }
            }
        if(j!=0)
            return product;
        else return 0;
    }
    else if(strncmp(str,"row",3) == 0)
    {
        for(int i = 0; i < table_width; i++)
        {
            if(table[k-1][i] != 1.11111111111)
            {
                j++;
                product *= table[k-1][i];
            }

        }
        if(j!=0)
            return product;
        else return 0;
    }
    else
    {
        return 0;
    }
}

/* "���������" �������� ��� ������ ��� ��� ������������ ����� ��� ���������� ������ ����������
�� ���� ���� ���. ������� ��� ������������� ��� ������� ������� ��� �� output handle ��� ���������,
����������� ��� ����������� ��� �� ��� ��� �� ����� ��� ���������� SetConsoleCursorPosition()*/
void visual_clear(COORD default_coord, HANDLE hout)
{
    COORD temp_coord;
    for(int i = 0; i < table_width+1; ++i)
    {
        for(int j = 0; j < table_height+1; ++j)
        {
            temp_coord.X = 4+i*13;
            temp_coord.Y = 5+2*j;
            SetConsoleCursorPosition(hout,temp_coord);
            printf("            ");
        }
    }
    SetConsoleCursorPosition(hout,default_coord);

}

/*�������� �� ������������� ��� ������ ��� ������ "<������> row/column <������� �������/������>"
(�.�. median row 1), ��� ������ k ��� ����������� ���� ������ �������/������, ����� ��� ���� ��������
��� ������� ��� ������ ��� ������ ������, �� ������ ������� ���������������� ��� ��� ��� ����������
��� ����� ��� ������ ���� ���������, ���� ���� �� ���� �������� ��� �������� ���.
� ��������� �������� ���� if statements �� �� �� ��������� ��� ��������� ��� �� ������ � �����
������� �� �� ������������� ��� ������ ��� ��������. ���������� ������� cnt, ��� ����� ������������
���� ������� ���� ������ ���������� ��� ��������� ��������� 1.11111111111 ��� �������� ���� ������ � ����� ��� ������ ���
������� ������� �� ������. ���� �������� ��� �� �������� ����� ��� ������� � ������ �������������� �� ����
��������� placeholder ������������ ������, ��� � ��������� ���������� ���� �� ������ �� ����� ��
�������� �� ����������� ����� ����� �� ��������� ��� ������ � ����� ��� ������ ������. ���� ������������
������� ���������� ��� �� ����� �� �������� �������� ��� ����� ��� ������������� ������, ��� ������
������� ���������� ��� �� ����� �� �������� �������� �� ������� �����. ���� if statements, ����������
����� ���������� �� �����������: �� � ������� ��� ��-��������� ��������� ����� �����, ������� ��� ���������� ��
������ ��� ���� ��������, ��� �� ����� �����, ������� ��� ���������� ��� ���� ��� ��� ��� ������� ��� ���� ��������.*/
double median(char *str, int k, int a, int b, double c[a][b])
{
    int cnt=0;
    double m,cmp;
    if(strncmp(str,"row",3) == 0)
    {
        for(int i=0;i<b;i++)
        {
            if(c[k-1][i]==1.11111111111)
            {
                cnt++;
            }
        }
        double temp[b];
        for(int i=0;i<b;i++)
        {
            temp[i]=c[k-1][i];
        }
        for(int j=0;j<b;j++)
        {
            for(int i=0;i<b;i++)
            {
                if(i!=b-1)
                {
                    if(temp[i]==1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        for(int j=0;j<b-cnt;j++)
        {
            for(int i=0;i<b-cnt;i++)
            {
                if(i!=b-cnt-1)
                {
                    if(temp[i]>temp[i+1] && temp[i+1]!=1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        if((b-cnt)%2!=0)
        {
            m=temp[(b-cnt-1)/2];
        }
        else
        {
            m=(temp[(b-cnt-1)/2]+temp[(b-cnt)/2])/2;
        }
    }
    else if(strncmp(str,"column",6) == 0)
    {
        for(int i=0;i<a;i++)
        {
            if(c[i][k-1]==1.11111111111)
            {
                cnt++;
            }
        }
        double temp[a];
        for(int i=0;i<a;i++)
        {
            temp[i]=c[i][k-1];
        }
        for(int j=0;j<a;j++)
        {
            for(int i=0;i<a;i++)
            {
                if(i!=a-1)
                {
                    if(temp[i]==1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        for(int j=0;j<a-cnt;j++)
        {
            for(int i=0;i<a-cnt;i++)
            {
                if(i!=a-cnt-1)
                {
                    if(temp[i]>temp[i+1] && temp[i+1]!=1.11111111111)
                    {
                        cmp=temp[i];
                        temp[i]=temp[i+1];
                        temp[i+1]=cmp;
                    }
                }
            }
        }
        if((a-cnt)%2!=0)
        {
            m=temp[(a-cnt-1)/2];
        }
        else
        {
            m=(temp[(a-cnt-1)/2]+temp[(a-cnt)/2])/2;
        }
    }
    return m;
}

//��������� ��� ����� �������� ������� ������ ��� �������, ������� �� ��� ������� ��� ������.
double add(char *str, int k, int table_width, int table_height, double table[table_height][table_width])

{
    double sum = 0;
    if(strncmp(str,"column",6) == 0)
    {
        for(int i = 0; i < table_height; i++)
            {
                if(table[i][k-1] != 1.11111111111)
                {
                    sum = sum + table[i][k-1];
                }
                //printf("%f\n",table[k-1][i]);
                //printf("\n true");
            }

        return sum;
    }
    else if(strncmp(str,"row",3) == 0)
    {
        for(int i = 0; i < table_width; i++)
        {
            if(table[k-1][i] != 1.11111111111)
            {
                sum = sum + table[k-1][i];
            }

        }
        return sum;
    }
    else
    {
        return 0;
    }
}

//������� ��� ���� ��� ��� ������� ������ ��� �������, ������� �� ��� ������� ��� ������.

/*� ��������� ����� � ���� ������� �� ��� add(), �� ���� ������� �� ��������� j. ����, ��������� ���� ���� ���
��������� ��� ���� ��� ������ � ����� ��� ����� ��� �� �� �������� ��� ���� �������� 1.1111111111 ��� ��� �����,
�� ���������� �������� �� �������� ���� ��� ����� ����� ��� ��� ������ ���� j.*/
double average(char *str, int k, int table_width, int table_height, double table[table_height][table_width])
{
    double sum = 0;
    int j = 0;
    if(strncmp(str,"column",6) == 0)
    {
        for(int i = 0; i < table_height; i++)
            {
                if(table[i][k-1] != 1.11111111111)
                {
                    sum = sum + table[i][k-1];
                    j++;
                }
            }
        if(j !=0)
            return (sum/j);
    }
    else if(strncmp(str,"row",3) == 0)
    {
        for(int i = 0; i < table_width; i++)
        {
            if(table[k-1][i] != 1.11111111111)
            {
                sum = sum + table[k-1][i];
                j++;
            }

        }
        if(j != 0)
            return (sum/j);
    }
    return 0;
}

//���������� �� ��������� ���� ��� ���� ����� � ������� ���� ������-����� ��� ����� �������� (�� ��� ���� ������� �������� ���������� 0)
/*������� �� input string ��� ���� ������� � ������� ��� ����� ��� ������ column-row X, ���� � � �������
��� ������-������� ��� ������ ���� ����� �������� �� ����� � ���������������. � �������� k �������������� ���
���� X. ������ � ��������� ������� �� ������ ��� ������ ��� ��� ���������� ���, ��� �� ������ �� ��������� ��������
���� ����� ��� ������ �� ��������� ��� �� ���� ��� ��������. ������ �������������� ��� � ������� ������� � �������� �� �����
�� ����� � ������ ��� ������ ��� ������ � ��������� min ������� ��� �������� ���� �� ������ ����� ��� �� ������������ ��� �. ����������.
*/
double min_value(char *str,int k,int a, int b,double c[a][b])
{
    --k;
    double min;
    if(strncmp(str,"row",3) == 0)
    {
         for(int j = 0; j < table_width; j++)
        {
            if(c[k][j] != 1.11111111111)
            {
                min = c[k][j];
                break;
            }
            else min = 0;
        }
        for(int j = 0; j < table_width; j++)
        {
            if(min > c[k][j] && c[k][j] != 1.11111111111)
            {
                min = c[k][j];
            }
        }
    }
    else if(strncmp(str,"column",6) == 0)
    {
         for(int j = 0; j < table_width; j++)
        {
            if(c[j][k] != 1.11111111111)
            {
                min = c[j][k];
                break;
            }
            else min = 0;
        }
        for(int j = 0; j < table_height; j++)
        {
            if(min > c[j][k]&& c[j][k] != 1.11111111111)
            {
                min = c[j][k];
            }
        }
    }
    if(min != 1.11111111111)
        return min;
    else
        return 0;
}
//���������� �� ������� ���� ��� ���� ����� � ������� ���� ������-����� ��� ����� �������� (�� ��� ���� ������� �������� ���������� 0
/*�������� �� ��� min_value ��� ����������� �� ���� ��������, � ��������� max_value ���������� �� ������� ���� ��� ������� � ������ ���
������ � ������� ��� ��� ���������� ��� �� �������. � ��������� max ����� ���� ��� ��� ����� �������� �� ������� ���� ��� ��� � �������
��� ���� ������� ������� ������ ������������ � ���� 0.
*/
double max_value(char *str,int k,int a, int b,double c[a][b])
{
    --k;
    double max;
    if(strncmp(str,"row",3) == 0)
    {
        for(int j = 0; j < table_width; j++)
        {
            if(c[k][j] != 1.11111111111)
            {
                max = c[k][j];
                break;
            }
            else max = 0;
        }
        for(int j = 0; j < table_width; j++)
        {
            if(max < c[k][j] && c[k][j] != 1.11111111111)
            {
                max = c[k][j];
            }
        }
    }
    else if(strncmp(str,"column",6) == 0)
    {
        for(int j = 0; j < table_width; j++)
        {
            if(c[j][k] != 1.11111111111)
            {
                max = c[j][k];
                break;
            }
            else max = 0;
        }
        for(int j = 0; j < table_height; j++)
        {
            if(max < c[j][k] && c[j][k] != 1.11111111111)
            {
                max = c[j][k];
            }
        }
    }
    if(max != 1.11111111111)
        return max;
    else
        return 0;
}

/*�������� �� ���������� ���� ������ ��� ������� �� ���������, �� ������������� ��� ������ "<������> row/column <������� �������/������>"
��� ���� ������� � ������� (�.�. add row 1), ��� ������ k ��� �������������� ��� ������ �������/������, �� output
handle ��� ��������� ��� ��� �������������� ������������� (������������� ����� ������� �������).
������ �������� ����� COORD result_coord, �� ����� ������� ��� ���������� ����� ������������� ������� ��
�� �� �������� ��� ����� ������ � �������, ������� �� if statements ��� ������������� �� �������������
��� ������, ����� ��� �� ����������� ������������ ������� �� ��� ������ ���������� "-" (��� ���������)
��� "|" (��� ������). ���� ����� ��� ������� ���� ������ �������������, � ��������� ��������� 12 ����������
����� ���� �� ������ ������ ����������� ����������� ����������. ��� �������� ��������� ���� ��� �������,
��������� �� ���������� ��� ����� ����� ��� ������� ���� ������������� ���� (���� ������� �������).*/
void print_result(int k,char *str, double res, HANDLE hout, COORD default_coord)
{
    COORD result_coord;
    if(strncmp(str,"row",3) == 0 && k<=table_height)
    {
        result_coord.X=4+13*table_width;
        result_coord.Y=3+2*k;
        SetConsoleCursorPosition(hout,result_coord);
        printf("            ");
        SetConsoleCursorPosition(hout,result_coord);
        printf("%g",res);
        SetConsoleCursorPosition(hout,default_coord);
    }
    else if(strncmp(str,"column",6) == 0 && k<=table_width)
    {
        result_coord.X=4+13*(k-1);
        result_coord.Y=5+2*table_height;
        SetConsoleCursorPosition(hout,result_coord);
        printf("            ");
        SetConsoleCursorPosition(hout,result_coord);
        printf("%g",res);
        SetConsoleCursorPosition(hout,default_coord);
    }

}

//������� �� ������� �������� ��� ������ ���� �������� ������, ����� ��� ��� ������ ������.
/*�������� �� ������������� ��� ���� ������� � ������� ��� �������� ��� ���� �������� ��� ��������� ':' ���� ��� (��� ����� ������ ���� �� ����� ����,
���� ���� ����� � ������� � ������ ���� �������� ��� ��� ������.). ������, ������� ��� ���������� ��� ������, ��� ���� ��� ������,
 ��� ������������� coord �� ������ ����������� �� ���� ����� ��� ������ [index] ������ �� ����������� � �������, ��� �������������
 ��� ������� ������� ��� �� output handle ��� ���������. ���� ���������� ��� ������ ���� �� float, � ��������� ��� ������� ��� ��������� ������ ��� ������.
 ������, �� ����������� ������� �������������� �� ������ ���� ����� (�� ���� ��� ��������� ������) ���� ��� ����������� � �������� ��� �� ������ ��� ���� ����.
 ����� ���� ��������� �� ���� ���������� ������ ����������, ���� ��������� ��� ������ ��� ��� ���� ����. � ���������� ����� ���������, ��� �� ������ ��� �����
 ���� ��� ������ ������ ���� ������� �� ������������� ��� ���������, ���� �������������� ��� ��� ���������. �����, ���� ���� ������� � ����������, � ��������
 ���������� ��� ������ ��� ������� ������� ��� ��������� ������ ����� ��� ��� ������.*/
void Add_num_to_array(char *str, int table_width, int table_height, double table[table_height][table_width], COORD coord, COORD default_coord, HANDLE hout)
{
    double c = atof(str);
    //printf("%d,%d", coord.X,coord.Y);
    table[coord.Y][coord.X] = c;
    //printf("%d,%d", coord.X,coord.Y);
    coord.X = 4 + coord.X*13;
    coord.Y = 5 + coord.Y*2;
    SetConsoleCursorPosition(hout,coord);
    printf("            ");
    SetConsoleCursorPosition(hout,coord);
    printf("%g",c);
    SetConsoleCursorPosition(hout,default_coord);

}

//������������������ �� ���������� ����������� ��� ������������� ���� ��. �3: ���� � 3�: ���� , ���� ���� � ������� ��� �������� � �������
//�� ������� ���� ��������� ������������� ��� ������. ���������� COORD = {100,100} ��� ������� ������ ������ ����������� ��� ��������������
/*� ��������� �������� �� string input ��� ������ �� ���� ���������. ������ ��������� ��� � ������ ���������� ��� ���� ������ ����� �������
� ������ ���� ��� ������ ASCII. ��� � ���������� ����� ������, ������� ��� ��������� �������. ��� ������� ��� ��������� user_coords ���� ����
�� ����� ��� ���������� ����� ��� ��������������� �� index ��� ������ ��� ����� ������ �� ����������� � �������. ������������ ����� ��� � ���������
atoi() ������� �������� �� �������� input ���� ���� ���� ��������� ��� ��� ����� �������, ��� �������� �� ���������� ��� ����������� ��� ��������� ��������
����� ������ ��������. ��� � ������ ���������� ����� �������, � ��������� ����� ��� �������. ������ �� ������ �� index ��� ������ str ��� �� ����� �� �����������
�� ���������� ��� �������� �� ����� ��� ����� ����� ���� ������ � ������� ������ �� ����� ��������� � ������� ������. �� �� ����� ��� while loop ��� ������, � ���������
j ������� ���� 1 � 2 ����������, ������� �� �� �� � ������� ����� ���������� � �������� ��� ���� �� ���� ����������� ������������ �������� �� ������������������ ��
������ ��� ������*/
COORD Handle_input(char *str)
{
    COORD user_coords = {100,100};
    if(str[0] > 64 && str[0] < 91)
    {
        user_coords.X = str[0]-65;
        user_coords.Y = atoi(&str[1])-1;
    }
    else if(str[0] > 47 && str[0] < 58)
    {
        int j = 0;
        int temp = atoi(str)/pow(10,j);
        user_coords.Y = atoi(str)-1;
        while(temp != 0)
        {
            j++;
            temp = atoi(str)/pow(10,j);
        }
        user_coords.X = str[j] - 65;
    }
    else
    {
        user_coords.X = 100;
        user_coords.Y = 100;
        //printf("There was an error processing your request");
    }
    return user_coords;
}

//������� ��� ������� ������ �� ��� ����� � ������� ������ �� �������������� ��� �� ��������� ���������� ��� ������ ��� �����.
/*�������� �� ����� ��� ���� ��� ��������������� ������, ����� ��� ��� ������ char ��� ��� ������������ ��� ������� ��� ������.
������������ �� ������ �������� ��� �� ���� ���� ���� �������������� ������� ������ ���������� ��������� ��� ���� ��������� ������
��� �� ��������� �� ��� ����� ������ ���������� �� ������� "-" ��� "|", ������� �� ��� ���������� ��� ������� � �������. �� ��������
char ���������� ���� 1 �� ��� ��������� ��� ������ ��� �� ������������� ��� ������� ��� ������.*/
void print_table(int table_width,int table_height, char column_notation , char row_notation)
{
    printf("\n   |");
    for(int i=0;i<table_width;i++)
    {
        printf("     %c      |",column_notation++);
    }
    printf("\n");
    for(int j=0;j<table_height;j++)
    {
        for(int i=0;i<table_width;i++)
        {
            printf("-------------");
        }
        printf("----\n");
        if(row_notation<10)
            printf(" %d |",row_notation++);
        else
            printf("%d |",row_notation++);
        for(int i=0;i<table_width;i++)
        {
            printf("            |");
        }
        printf("\n");
    }
    for(int i=0;i<table_width;i++)
        {
            printf("-------------");
        }
    printf("----\n");
    printf("\n \nWelcome to \"Pepega's Computational Table\". You can press \"CONTROL\" to type your commands.\n\
For a list of all commands, please press \"CONTROL\" to begin typing, and then type \"help\".\nThank you for using \"Pepega's Computational Table\"");
}

//https://cboard.cprogramming.com/c-programming/14497-limiting-number-characters-typed.html
//������� ��� ��� �������� ��������, ���� ��� ���� C ����� ������� �� ��������� ���� ��� �� ������, ������ �� ������������
//��� ������ ���� ���� �� ��� �������� �� ������ ���� ��� ������������ ������ ����������. ���'��� ��� �� �������������� �����
//���������, �������������� ��� �������� ���� �� ��� ������� ���������� ���� ����� �� ��� ��������� ����� ���.
int input(char *string, int length)
{
   int  done  = FALSE;
   int  index = 0;
   char ch;

   string[0] = NUL;

   do
   {
      ch = getch();

      //check to see whether the buffer is full
      if(index == length)
      {
         switch(ch)
         {
            case CR:
               break;
            case BACKSPACE:
               break;
            default:
               ch = NUL;
         }
      }

      //process the keyboard input
      switch(ch)
      {
         case CR:
            putchar(ch);
            putchar(LF);
            string[index] = NUL;
            done = TRUE;
            break;
         case BACKSPACE:
            if(index == 0)
            {
               break;
            }
            else
            {
               putchar(ch);
               putchar(' ');
               putchar(ch);
               index--;
            }
            break;
         case TAB:
            break;
         case NUL:
            break;
         default:        //display & store
            putchar(ch);
            string[index] = ch;
            index++;
       }
   }
   while(!done);
}

//��������� �� �������� ������������� ��� ���������� ��� ������� ���� ��� ������ �������.
void Clear_previous_messages(COORD default_coord, HANDLE hout)
{
    default_coord.Y++;
    SetConsoleCursorPosition(hout,default_coord);
    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 150; j++ )
        {
                printf(" ");
        }
        printf("\n");
    }
    default_coord.Y--;
    SetConsoleCursorPosition(hout, default_coord);
}
